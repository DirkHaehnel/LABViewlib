/*======================================================================

Copyright (C) 1997.  Data Translation, Inc., 100 Locke
Drive, Marlboro, Massachusetts 01752-1192.
   	
All rights reserved.  This software is furnished to purchaser
under a license for use on a single computer system and can be
copied (with the inclusion of DTI's copyright notice) only for
use in such system, except as may be otherwise provided in
writing by Data Translation, Inc., 100 Locke Drive, Marlboro,
Massachusetts 01752-1192.

The information in this document is subject to change without
notice and should not be construed as a commitment by Data
Translation, Inc.  Data Translation, Inc. assumes no
responsibility for any errors that may appear in this document.

Data Translation cannot assume any responsibility for the use
of any portion of this software on any equipment not supplied
by Data Translation, Inc.

=======================================================================*/

// Includes from windows API
#include <windows.h>            /* required for all Windows applications */
#include <windowsx.h>
#include <stdio.h>
#include <COMMCTRL.H>

// Includes from the project
#include "olfgapi.h"
#include "resource.h"
#include "DtLineScan.h"
#include "LsAcquire.h"


//****************************************************
// Change this value if you want to use another
// video channel
//****************************************************
#define VIDEO_CHANNEL 0
//****************************************************

#define CHECK_FOR_DEVICE if(!DeviceId1) {\
MessageBox(hMainWnd,"Open a device first.","LS-Acquire",0);\
break;\
}

// Global variables used
OLT_FG_FRAME_INFO	FrameInfo;
CHAR		cmd_line[20];
HINSTANCE	hInst;
HWND		hMainWnd;
OLT_IMG_DEV_ID		DeviceId1=NULL;

#define NUM_PASS_BUFFERS	3

void*		lplpBuffers[NUM_PASS_BUFFERS];
DWORD		lpDone[NUM_PASS_BUFFERS];
HANDLE		hSyncEvent = NULL;
HANDLE		hSyncThread = NULL;
BOOL		bPassthru = FALSE;

DWORD	SyncThread(DWORD Parameter);
void OnSelectDevice(HWND hWnd);
void PRIVATE CloseCurrentDevice(void);
BOOL CALLBACK __export SelectDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL PRIVATE InitDeviceList(HWND hComboBox);
BOOL PRIVATE NewDevice(LPCSTR lpcszAlias, HWND hWnd);

/**********************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

This is the entry point into the application.  It contains the Window class structure and the 
Main message loop.

hInstance: 		The instance of the application
hPrevInstance: 	The previous instance of the application
lpCmdLine: 		Command line parameter passed in on application load.
nCmdShow:		Contains an enumerated value defining how the window should be displayed.

Return Value:
If the function succeeds, terminating when it receives a WM_QUIT message, it should return
the exit value contained in that message's wParam parameter. If the function terminates
before entering the message loop, it should return 0. 
**********************************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS  wc;
    MSG msg;
    HACCEL hAccTable = NULL;

	// Save command line parameters
	lstrcpy(cmd_line, lpCmdLine);

    if (!hPrevInstance)
    {
		wc.style = CS_HREDRAW | CS_VREDRAW;
	    wc.lpfnWndProc = MainWndProc;
    	wc.cbClsExtra = 0;
    	wc.cbWndExtra = DLGWINDOWEXTRA;
    	wc.hInstance = hInstance;
    	wc.hIcon = LoadIcon(NULL,MAKEINTRESOURCE(IDI_ICON1));
    	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    	wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    	wc.lpszMenuName =  NULL;
    	wc.lpszClassName = "LsAcquire";
    }
                                        
    if (!RegisterClass(&wc))
        return (FALSE);
    	
    hInst = hInstance;
	 InitCommonControls();

    hMainWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAIN_WINDOW) ,NULL,0);
                                        
    if (!hMainWnd)
        return (FALSE);
        
    ShowWindow(hMainWnd, nCmdShow);
                                                
    while (GetMessage(&msg, NULL, 0, 0))
    {   
        if (!TranslateAccelerator(hMainWnd, hAccTable, &msg)==TRUE)
        {
            TranslateMessage(&msg);            
            DispatchMessage(&msg);
        }
    }
    return (msg.wParam);

	return TRUE;
}


/**********************************************************************************************
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

This is the Main message processing procedure  It controls all the dispatching of the Board
Dialog and the Main Menu selections.

hWnd: 		The handle of the calling window.
message:	The message to be processed.
wParam: 	The WORD parameter of the message.
lParam:		The LONG parameter of the message.

Return Value:
The return value is dependant on the message being processed
**********************************************************************************************/                     
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	static	HWND				hWndDisplay,hWndScroll,hBlack,hWhite, hLut, hTrigger;
	static	SIZE				Size;
			USHORT				OldSource;
			BOOL				bTranslated;
			LONG				OldData;
			UINT				NumLines, FirstActPix, ActivePixCount;
			UINT				LineHI,LineLO,IntegrationHI,IntegrationLO;
			UINT				ClampStart, ClampEnd;
			DWORD				BufferSize,Clock;
			BYTE*				lpUserBuffer;
	static	WINDOWINFO			WindowInfo;

    switch (message)
    	{
			case WM_CREATE:

				// No buffer to draw associtated to the window yet
				WindowInfo.hUserData	= NULL;
				WindowInfo.Height		= 0;
				WindowInfo.Width		= 0;
				
				return 1;

			case WMUSER_INIT:

				OlFgSetInputVideoSource(DeviceId1, VIDEO_CHANNEL, &OldSource);
				
				// Write the default values in the edit controls
				SetDlgItemInt(hWnd, IDC_FIRST_ACT_PIX,120, FALSE);
				SetDlgItemInt(hWnd, IDC_TOT_PIX_PER_LINE, 2048, FALSE);

				SetDlgItemInt(hWnd, IDC_ACTIVE_PIX_COUNT, 1500, FALSE);
				SetDlgItemInt(hWnd, IDC_NUM_LINES, 100, FALSE);

				SetDlgItemInt(hWnd, IDC_CLOCK, 6000, FALSE);

				SetDlgItemInt(hWnd, IDC_CLAMP_START, 58, FALSE);
				SetDlgItemInt(hWnd, IDC_CLAMP_END, 50, FALSE);

				SetDlgItemInt(hWnd, IDC_LINE_HI, 0x1B00, FALSE);
				SetDlgItemInt(hWnd, IDC_LINE_LO, 0x1B58, FALSE);
				SetDlgItemInt(hWnd, IDC_INTG_HI, 0, FALSE);
				SetDlgItemInt(hWnd, IDC_INTG_LO, 0, FALSE);

				hWndDisplay = ShowBasicWindow (100, 100, 640, 480);						

				SetWindowLong(hWndDisplay, GWL_USERDATA, (LONG)&WindowInfo);					

				//****************************************************
				//****************************************************
				// Set the default values for the track bars
				//****************************************************
				hBlack = GetDlgItem(hWnd,IDC_BLACK);
				hWhite = GetDlgItem(hWnd,IDC_WHITE);

				SendMessage(hBlack,TBM_SETRANGEMAX,0,1075200);
				SendMessage(hBlack,TBM_SETRANGEMIN,0,-1066800);

				SendMessage(hWhite,TBM_SETRANGEMAX,0,1275000);
				SendMessage(hWhite,TBM_SETRANGEMIN,0, 0);

				SendMessage(hWhite,TBM_SETPOS,1,1000000);
				SendMessage(hBlack,TBM_SETPOS,1,-200000);
				//****************************************************

				//****************************************************
				//****************************************************
				// Get the handles for the check boxes
				//****************************************************
				hLut = GetDlgItem(hWnd,IDC_LUT);
				hTrigger = GetDlgItem(hWnd,IDC_TRIGGER);
				//****************************************************
				break;

			case WMUSER_CALLBACK:
			{

				// **************************************************
				// Do whatever you need to do here...
				// This code is just sending in the debug console
				// which buffer is ready whenver it receives a message
				// **************************************************

				char	String[255];
				int		i;

				// Check which buffer is ready
				for (i=0; i<NUM_PASS_BUFFERS; i++)
				{
					if (lpDone[i])
					{
						// Reset the flag
						lpDone[i] = 0;
						break;	// exit the loop;
					}
				}

				
				sprintf(String,"Buffer %d ready...\n",i);
				OutputDebugString(String);
				break;
			}

			case WM_COMMAND:
				
				switch(LOWORD(wParam))
				{
					/* Select a device */
					case IDM_SETUP_SELECT_DEVICE:
					    OnSelectDevice(hWnd);
						break; 
					    
					/*Close the currently opened device */
					case IDM_SETUP_CLOSE_DEVICE:
					
						CloseCurrentDevice();
					  
					    /* Set menu items to ensure a correct sequence of user actions */
					    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SELECT_DEVICE, MF_ENABLED | MF_BYCOMMAND);
					    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_CLOSE_DEVICE, MF_GRAYED | MF_BYCOMMAND);

						 DestroyWindow(hWndDisplay);
					    break;

					case IDM_HELP:
						WinHelp(hWnd, "LSACQ.HLP", HELP_CONTENTS, 0);
						break;

					case IDC_ACQUIRE:
					{
						ULONG	i,Error;
						ULONG	lpLut[256];
						int	Black, White;

						CHECK_FOR_DEVICE
							
						//****************************************************
						//****************************************************
						// Check to see if we want to invert the LUT
						//****************************************************
						for (i=0; i<256; i++)
						{
							if (IsDlgButtonChecked(hWnd,IDC_LUT) == BST_CHECKED)
							{
								lpLut[i] = 255-i;
							}
							else
							{
								lpLut[i] = i;
							}
						}
						
						Error = OlFgWriteInputLUT(DeviceId1,0,0,256,lpLut);
						//****************************************************

						/****************************************************
							This portion of the code retrieves all the values
							of the parameters being set in the edit controls
							of the main window
						*****************************************************/
						FirstActPix = GetDlgItemInt(hWnd, IDC_FIRST_ACT_PIX, &bTranslated, FALSE);
						ActivePixCount = GetDlgItemInt(hWnd, IDC_ACTIVE_PIX_COUNT, &bTranslated, FALSE);
						NumLines = GetDlgItemInt(hWnd, IDC_NUM_LINES, &bTranslated, FALSE);
						ClampStart = GetDlgItemInt(hWnd, IDC_CLAMP_START, &bTranslated, FALSE);
						ClampEnd = GetDlgItemInt(hWnd, IDC_CLAMP_END, &bTranslated, FALSE);
						LineLO = GetDlgItemInt(hWnd, IDC_LINE_LO, &bTranslated, FALSE);
						LineHI = GetDlgItemInt(hWnd, IDC_LINE_HI, &bTranslated, FALSE);
						IntegrationLO = GetDlgItemInt(hWnd, IDC_INTG_LO, &bTranslated, FALSE);
						IntegrationHI = GetDlgItemInt(hWnd, IDC_INTG_HI, &bTranslated, FALSE);
						Clock = GetDlgItemInt(hWnd, IDC_CLOCK, &bTranslated, FALSE);
						//****************************************************


						//****************************************************
						// This section sets everything that can be set with
						// the core SDK calls.
						//****************************************************
						Error = OlFgSetInputVideoSource(DeviceId1, VIDEO_CHANNEL, (USHORT*)&OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_FIRST_ACTIVE_PIXEL,
															FirstActPix, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_ACTIVE_PIXEL_COUNT,
															ActivePixCount, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_CLAMP_START,
															ClampStart, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_CLAMP_END,
															ClampEnd, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_FRAME_WIDTH,
															ActivePixCount, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_FRAME_HEIGHT,
															NumLines, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_VIDEO_TYPE,
															OLC_FG_VID_VARSCAN, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_VARSCAN_FLAGS,
															0, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_CLOCK_SOURCE,
															OLC_FG_CLOCK_INTERNAL, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_CLOCK_FREQ,
															Clock*1000, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_INPUT_FILTER,
															OLC_FG_FILT_DC_NONE, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_SYNC_SENTINEL,
															FALSE, &OldData);
						//****************************************************

						//****************************************************
						//	This section sets what has to be set with the
						//	extension DLL.
						//****************************************************
						OlFgSetLsLineDrive (DeviceId1,LS_SYNC_HI_TO_LO,LineLO);
						OlFgSetLsLineDrive (DeviceId1,LS_SYNC_LO_TO_HI,LineHI);
						OlFgSetLsIntegration (DeviceId1,LS_SYNC_HI_TO_LO,IntegrationLO);
						OlFgSetLsIntegration (DeviceId1,LS_SYNC_LO_TO_HI,IntegrationHI);
						//****************************************************

						//****************************************************
						//	Verify if the "triggered" check box is checked
						//	and program the trigger according to ti
						//****************************************************
						if ( IsDlgButtonChecked(hWnd,IDC_TRIGGER) )
						{
							Error = OlFgSetTriggerInfo (DeviceId1,OLC_FG_TRIGGER_EXTERNAL_LINE,FALSE,
														(LPOLT_FG_TRIGGER) &OldData, &OldData);
						}
						else
						{
							Error = OlFgSetTriggerInfo (DeviceId1,OLC_FG_TRIGGER_NONE, FALSE,
														(LPOLT_FG_TRIGGER) &OldData, &OldData);
						}
						/****************************************************/

						//****************************************************
						//	Get the position of the trackbars for the
						//	black and white levels and set them.
						//****************************************************
						White = SendMessage(hWhite,TBM_GETPOS,0,0);
						Black = SendMessage(hBlack,TBM_GETPOS,0,0);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_WHITE_LEVEL,
														   White, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_BLACK_LEVEL,
														   Black, &OldData);
						//****************************************************

						Error = OlFgSetLsLineDrive (DeviceId1,LS_SYNC_LO_TO_HI,0x1B00);
						Error = OlFgSetLsLineDrive (DeviceId1,LS_SYNC_HI_TO_LO,0x1B58);

						lpUserBuffer = GlobalAlloc(GPTR, ActivePixCount * NumLines);
						BufferSize = ActivePixCount * NumLines;
						
						if (WindowInfo.hUserData)
						{
							GlobalFree(WindowInfo.hUserData);
						}
						
						WindowInfo.hUserData = lpUserBuffer;
						WindowInfo.Height = NumLines;
						WindowInfo.Width = ActivePixCount;

						SetWindowLong(hWndDisplay, GWL_USERDATA, (LONG)&WindowInfo);					
						
						OlFgAcquireLines(DeviceId1, lpUserBuffer, (ActivePixCount * NumLines), SYNC_ACQUIRE);
						InvalidateRect(hWndDisplay, NULL, TRUE);
						PostMessage(hWndDisplay, WM_PAINT, 0, 0);

						break;
					}

					case IDC_START_PASSTHRU:
					{
						ULONG	i,Error;
						ULONG	lpLut[256];
						int	Black, White;
						DWORD	ThreadId;

						CHECK_FOR_DEVICE

						//****************************************************
						//****************************************************
						// Check to see if we want to invert the LUT
						//****************************************************
						for (i=0; i<256; i++)
						{
							if (IsDlgButtonChecked(hWnd,IDC_LUT) == BST_CHECKED)
							{
								lpLut[i] = 255-i;
							}
							else
							{
								lpLut[i] = i;
							}
						}
						
						Error = OlFgWriteInputLUT(DeviceId1,0,0,256,lpLut);
						//****************************************************

						/****************************************************
							This portion of the code retrieves all the values
							of the parameters being set in the edit controls
							of the main window
						*****************************************************/
						FirstActPix = GetDlgItemInt(hWnd, IDC_FIRST_ACT_PIX, &bTranslated, FALSE);
						ActivePixCount = GetDlgItemInt(hWnd, IDC_ACTIVE_PIX_COUNT, &bTranslated, FALSE);
						NumLines = GetDlgItemInt(hWnd, IDC_NUM_LINES, &bTranslated, FALSE);
						ClampStart = GetDlgItemInt(hWnd, IDC_CLAMP_START, &bTranslated, FALSE);
						ClampEnd = GetDlgItemInt(hWnd, IDC_CLAMP_END, &bTranslated, FALSE);
						LineLO = GetDlgItemInt(hWnd, IDC_LINE_LO, &bTranslated, FALSE);
						LineHI = GetDlgItemInt(hWnd, IDC_LINE_HI, &bTranslated, FALSE);
						IntegrationLO = GetDlgItemInt(hWnd, IDC_INTG_LO, &bTranslated, FALSE);
						IntegrationHI = GetDlgItemInt(hWnd, IDC_INTG_HI, &bTranslated, FALSE);
						Clock = GetDlgItemInt(hWnd, IDC_CLOCK, &bTranslated, FALSE);
						//****************************************************


						//****************************************************
						// This section sets everything that can be set with
						// the core SDK calls.
						//****************************************************
						Error = OlFgSetInputVideoSource(DeviceId1, VIDEO_CHANNEL, (USHORT*)&OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_FIRST_ACTIVE_PIXEL,
															FirstActPix, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_ACTIVE_PIXEL_COUNT,
															ActivePixCount, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_CLAMP_START,
															ClampStart, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_CLAMP_END,
															ClampEnd, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_FRAME_HEIGHT,
															NumLines, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_VIDEO_TYPE,
															OLC_FG_VID_VARSCAN, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_VARSCAN_FLAGS,
															0, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_CLOCK_SOURCE,
															OLC_FG_CLOCK_INTERNAL, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_CLOCK_FREQ,
															Clock * 1000, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_INPUT_FILTER,
															OLC_FG_FILT_DC_NONE, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_SYNC_SENTINEL,
															FALSE, &OldData);
						//****************************************************

						//****************************************************
						//	This section sets what has to be set with the
						//	extension DLL.
						//****************************************************
						OlFgSetLsLineDrive (DeviceId1,LS_SYNC_HI_TO_LO,LineLO);
						OlFgSetLsLineDrive (DeviceId1,LS_SYNC_LO_TO_HI,LineHI);
						OlFgSetLsIntegration (DeviceId1,LS_SYNC_HI_TO_LO,IntegrationLO);
						OlFgSetLsIntegration (DeviceId1,LS_SYNC_LO_TO_HI,IntegrationHI);
						//****************************************************

						//****************************************************
						//	Verify if the "triggered" check box is checked
						//	and program the trigger according to ti
						//****************************************************
						if ( IsDlgButtonChecked(hWnd,IDC_TRIGGER) )
						{
							Error = OlFgSetTriggerInfo (DeviceId1,OLC_FG_TRIGGER_EXTERNAL_LINE,FALSE,
														(LPOLT_FG_TRIGGER) &OldData, &OldData);
						}
						else
						{
							Error = OlFgSetTriggerInfo (DeviceId1,OLC_FG_TRIGGER_NONE, FALSE,
														(LPOLT_FG_TRIGGER) &OldData, &OldData);
						}
						/****************************************************/

						//****************************************************
						//	Get the position of the trackbars for the
						//	black and white levels and set them.
						//****************************************************
						White = SendMessage(hWhite,TBM_GETPOS,0,0);
						Black = SendMessage(hBlack,TBM_GETPOS,0,0);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_WHITE_LEVEL,
														   White, &OldData);

						Error = OlFgSetInputControlValue(DeviceId1, VIDEO_CHANNEL, OLC_FG_CTL_BLACK_LEVEL,
														   Black, &OldData);
						//****************************************************

						Error = OlFgSetLsLineDrive (DeviceId1,LS_SYNC_LO_TO_HI,0x1B00);
						Error = OlFgSetLsLineDrive (DeviceId1,LS_SYNC_HI_TO_LO,0x1B58);


						// Create NUM_PASS_BUFFERS buffers for passthru
						for (i=0; i<NUM_PASS_BUFFERS; i++)
						{
							lplpBuffers[i] = GlobalAlloc(GPTR, ActivePixCount * NumLines);
							lpDone[i] = 0;
						}

						Error = OlFgStartAsyncLsPassthru(	DeviceId1,
													hWndDisplay,
													lplpBuffers,
													(ActivePixCount * NumLines),
													NUM_PASS_BUFFERS * sizeof(char*),
													NUM_PASS_BUFFERS,
													&hSyncEvent,
													lpDone,
													NUM_PASS_BUFFERS * sizeof(DWORD),
													0
												);

						bPassthru = TRUE;

						hSyncThread = CreateThread ( NULL, 0, (LPTHREAD_START_ROUTINE)SyncThread,
													0, 0, &ThreadId);

						break;
					}

					case IDC_STOP_PASSTHRU:
					{
						int i;

						CHECK_FOR_DEVICE

						OlFgStopAsyncLsPassthru(DeviceId1);
						bPassthru = FALSE;

						// In case the user press this button twice
						if (hSyncThread)
						{
							TerminateThread(hSyncThread,0);
							hSyncThread = NULL;
						}

						// Free the buffer they were allocated
						for (i=0; i<NUM_PASS_BUFFERS; i++)
						{
							if (lplpBuffers[i]) 
							{
								GlobalFree(lplpBuffers[i]);
								lplpBuffers[i] = NULL;
							}
						}

						break;
					}

					case IDC_CLOSE:
						PostMessage(hWnd,WM_CLOSE,0,0l);
						break;

				}
				break;			  

			case WM_CLOSE:

				// Close the device
				if (DeviceId1)
				{
					OlImgCloseDevice(DeviceId1);
					DeviceId1 = NULL;
				}
				
				// Destroy the child window
				DestroyWindow(hWndDisplay);

				// Post the WM_DESTROY message
				PostMessage(hWnd,WM_DESTROY,0,0l);
				return 0;

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

		}
	return DefWindowProc(hWnd,message,wParam,lParam);

 }


/*****************************************************************************
 FUNCTION			 :	ShowBasicWindow

 Description        :  This routine is to help the developper to build
						in a quick way a window where you can display, for
						example, a frame buffer. The window has only a system
						menu to allow the user to shut it down.

						There is no REDRAW handler so anything displayed
						in such window will disappear as soon as the window
						is clipped by some other window.

 Comments           :  This function has development purpose only. 
****************************************************************************/
HWND ShowBasicWindow (WORD X1, WORD Y1, WORD X2, WORD Y2)
{
	HWND		NewWindow = 0;		
	WORD		SizeX = X2 - X1;
	WORD		SizeY = Y2 - Y1;
	WNDCLASS    wc    = {0};


   wc.style           = CS_VREDRAW | CS_VREDRAW;
   wc.lpfnWndProc     = DisplayWndProc;
   wc.cbClsExtra      = 0;
   wc.cbWndExtra      = 0;
   wc.hInstance       = hInst;
   wc.hIcon           = 0;
   wc.hCursor         = 0;
   wc.hbrBackground   = (HBRUSH) (COLOR_WINDOW + 1);
   wc.lpszMenuName    = NULL;
   wc.lpszClassName   = "DEFAULT_WINDOW";
      
   RegisterClass(&wc);    // may fail if class already exists.


	NewWindow = CreateWindow(
					"DEFAULT_WINDOW",		// Default class
					"LS-Acquire display window",		// title
					WS_OVERLAPPED | WS_HSCROLL| WS_VSCROLL | WS_SIZEBOX, // style
					X1, Y1,							// x, y position
					SizeX, SizeY,					// size 
					hMainWnd,					
					NULL,							// menu
					NULL, 
					NULL);

	ShowWindow(NewWindow,SW_SHOWDEFAULT);

	return NewWindow;
}

/**********************************************************************************************
LRESULT CALLBACK DisplayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

This procedure processes the messages for the basic display window. It takes care of adujsting
the 


hWnd: 		The handle of the calling window.
message:		The message to be processed.
wParam: 		The WORD parameter of the message.
lParam:		The LONG parameter of the message.

Return Value:
The return value is dependant on the message being processed
**********************************************************************************************/                     
LRESULT CALLBACK DisplayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WINDOWINFO* lpWindowInfo;
	BYTE*		lpUserData;
	RECT		rect;
	PAINTSTRUCT ps;
	LONG		WndWidth, WndHeight;
	static LONG		x=0,y=0;
	POINT		Org;
	
	lpWindowInfo = (WINDOWINFO*)GetWindowLong(hWnd, GWL_USERDATA);
	
	// Get the height and width of the window
	GetClientRect(hWnd, &rect);
	WndWidth = rect.right;
	WndHeight = rect.bottom;

	// Set the scroll bar range
	if(lpWindowInfo)
	{
		// Do we need a scroll bar?
		if((lpWindowInfo->Height-WndHeight) > 0)
		{
			SetScrollRange(hWnd, SB_VERT, 0, (int)lpWindowInfo->Height-WndHeight, TRUE);			
			EnableScrollBar(hWnd, SB_VERT, ESB_ENABLE_BOTH);
		}
		else
		{
			EnableScrollBar(hWnd, SB_VERT, ESB_DISABLE_BOTH);	
		}

		// Do we need a scroll bar?
		if((lpWindowInfo->Width-WndWidth) > 0)
		{
			SetScrollRange(hWnd, SB_HORZ, 0, (int)lpWindowInfo->Width-WndWidth, TRUE);
			EnableScrollBar(hWnd, SB_HORZ, ESB_ENABLE_BOTH);	
		}
		else
		{
			EnableScrollBar(hWnd, SB_HORZ, ESB_DISABLE_BOTH);	
		}		
	}

	switch (message)
    	{
			// Do nothing but returns 0
			case WM_CREATE:
				return 0;
			
			// Modify the origin "y" of the picture according
			// to the action that has been taken on the scroll bar			
			case WM_VSCROLL:

				if(lpWindowInfo)
				{
					switch(LOWORD(wParam))
					{
						case SB_LINEUP:
							if(y>0)
							{
								y--;
								SetScrollPos(hWnd, SB_VERT, y, TRUE);
								InvalidateRect(hWnd, NULL, TRUE);
							}
							break;

						case SB_LINEDOWN:
							if(y<lpWindowInfo->Height-WndHeight)
							{
								y++;
								SetScrollPos(hWnd, SB_VERT, y, TRUE);
								InvalidateRect(hWnd, NULL, TRUE);
							}
							break;

						case SB_PAGEUP:
							if(y>WndHeight)
							{
								y-=100;
							}
							else
							{
								y=0;
							}
							SetScrollPos(hWnd, SB_VERT, y, TRUE);
							InvalidateRect(hWnd, NULL, TRUE);
							break;

						case SB_PAGEDOWN:
							if(y<lpWindowInfo->Height-WndHeight)
								y+=100;
							else
								y=lpWindowInfo->Height-WndHeight;
							SetScrollPos(hWnd, SB_VERT, y, TRUE);
							InvalidateRect(hWnd, NULL, TRUE);
							break;

						case SB_THUMBPOSITION:
							y = HIWORD(wParam);				
							SetScrollPos(hWnd, SB_VERT, y, TRUE);
							InvalidateRect(hWnd, NULL, TRUE);
							break;

					}				

					// Set the origin for passthru also.
					Org.x = x;
					Org.y = y;
					OlFgSetPassthruSourceOrigin (DeviceId1,&Org);

				}
				break;


			// Modify the origin "x" of the picture according
			// to the action that has been taken on the scroll bar			
			case WM_HSCROLL:
				if(lpWindowInfo)
				{
					switch(LOWORD(wParam))
					{
						case SB_LINELEFT:
							if(x>0)
								x--;
							SetScrollPos(hWnd, SB_HORZ, x, TRUE);
							InvalidateRect(hWnd, NULL, TRUE);
							break;

						case SB_LINERIGHT:
							if(x<lpWindowInfo->Width-100)
								x++;
							SetScrollPos(hWnd, SB_HORZ, x, TRUE);
							InvalidateRect(hWnd, NULL, TRUE);
							break;

						case SB_PAGELEFT:
							if(x>WndWidth)
								x-=WndWidth;
							else
								x=0;
							SetScrollPos(hWnd, SB_HORZ, x, TRUE);
							InvalidateRect(hWnd, NULL, TRUE);
							break;

						case SB_PAGERIGHT:
							if(x<lpWindowInfo->Width-WndWidth)
								x+=100;
							else
								x=lpWindowInfo->Width-WndWidth;
							SetScrollPos(hWnd, SB_HORZ, x, TRUE);
							InvalidateRect(hWnd, NULL, TRUE);
							break;

						case SB_THUMBPOSITION:
							x = HIWORD(wParam);				
							SetScrollPos(hWnd, SB_HORZ, x, TRUE);
							InvalidateRect(hWnd, NULL, TRUE);
							break;

					}

					// Set the origin for passthru also.
					Org.x = x;
					Org.y = y;
					OlFgSetPassthruSourceOrigin (DeviceId1,&Org);

				}

				break;

			case WM_SIZE:				
				break;

			case WM_PAINT:
				
				// Don't paint if we doing passthru.
				if(GetUpdateRect(hWnd, &rect, TRUE) && !bPassthru)
				{
					BeginPaint(hWnd, &ps);
					if(lpWindowInfo)
					{
						LS_BUFFER_INFO	BufferInfo;

						BufferInfo.dwSize		=	sizeof(LS_BUFFER_INFO);
						BufferInfo.dwPixelDepth	=	1;
						BufferInfo.dwWidth		=	lpWindowInfo->Width;
						BufferInfo.dwHeight		=	lpWindowInfo->Height;
						BufferInfo.dwOriginX	=	x;
						BufferInfo.dwOriginY	=	y;
						BufferInfo.lpBuffer		=	lpUserData = lpWindowInfo->hUserData;

						OlFgDrawAcquiredLines(DeviceId1,hWnd,BufferInfo);
					}	
					EndPaint(hWnd, &ps);
				}				
				break;

			case WM_COMMAND:
				
				switch(LOWORD(wParam))
				{				

				}
				break;			  
		}
	return DefWindowProc(hWnd,message,wParam,lParam);

 }

/**********************************************************************************************
DWORD	SyncThread(DWORD Parameter)

This function is a thread of execution used to monitor whenver a buffer is ready.
When such buffer is ready, it posts a message to the main application window.
The application signals the thread when it stops passthru operations.

Return Value:
The return value is dependant on the message being processed
**********************************************************************************************/                     
DWORD	SyncThread(DWORD Parameter)
{
	// This is a blocking call, which means that the thread will be removed
	// from the running queue until this object is being signaled. The object
	// is being signaled by the driver.
	while (WaitForSingleObject(hSyncEvent,INFINITE) != WAIT_FAILED)
	{
		PostMessage(hMainWnd,WMUSER_CALLBACK,0,0);
	}

	return TRUE;
}


/**************************************************************************/
/**************************************************************************/

/*  Function:   OnSelectDevice

    Description:
	This function is called when the select device menu item is 
	selected.  The function shows a dialog box to fetch the alias name of the 
	desired board.  This board is then opened, and the enable and checked state
	of the menu is updated to match the new board's capabilities.

    Input Arguments:
	HWND hWnd   window handle of main window

    Output Arguments:
	None

    Return Value:
	None


*/

void OnSelectDevice(HWND hWnd)
{
    char Alias[OLC_MAX_ALIAS_STR_SIZE];
    BOOL bGotAlias = 0;

	STRING			DeviceName = {0};

    /* User picks the device */
    bGotAlias = (BOOL) DialogBoxParam(  hInst,
					MAKEINTRESOURCE(IDD_DEVICE_LIST),
					hWnd,
					SelectDeviceProc,
					(LPARAM)(LPSTR)Alias );
			 
	if ( bGotAlias == -1 ) 
	{ 
		char msg[128];
		wsprintf(msg, "Unable to display dialog box.");
		MessageBox(hWnd, msg, "Select Device", MB_ICONEXCLAMATION | MB_OK);
		return;
    }              

    /* No alias means that no alias was selected or Cancel was pushed */

    if ( !bGotAlias )
	{
		return;
	}

    /* try to open it and read it's capabilities.  */
    if ( !NewDevice(Alias, hWnd) ) 
	{
		char msg[128];
		wsprintf(msg, "Unable to open and initialize device (\"%s\").",Alias);
		MessageBox(hWnd, msg, "Select Device", MB_ICONSTOP | MB_OK);
		return;
    } 

	/* Set menu items to match board caps */
	EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SELECT_DEVICE, MF_GRAYED | MF_BYCOMMAND);
	EnableMenuItem(GetMenu(hWnd), IDM_SETUP_CLOSE_DEVICE, MF_ENABLED | MF_BYCOMMAND);
	     
}

/****************************************************************************/
/****************************************************************************/

/*  Function:   SelectDeviceProc

    Description:
	This function is the dialog procedure for the dialog box used to
	select the DT-Open Layers Imaging frame grabber to use.
    
    Initialization:
	This dialog box should be invoked using DialogBoxParam.  The
	lParamInit argument should be a far pointer to a string at least
	MAX_ALIAS_STR_SIZE characters long that will receive the selected
	device alias.

    Input Arguments:
	HWND hDlg       dialog handle
	UINT message        message
	WPARAM wParam       16-bit message-specific data
	LPARAM lParam       32-bit message-specific data

    Output Arguments:
	None

    EndDialog Return:  BOOL
	TRUE if non-zero length string was chosen, FALSE otherwise.

    Returns:  BOOL
	Except in response to WM_INITDIALOG, TRUE is returned if the
	dialog proc processes the message, FALSE if it does not.  For
	WM_INITDIALOG, FALSE is returned if the dialog proc calls
	SetFocus to set the input focus to a specific control.  If
	not, TRUE should be returned to indicate that the input focus
	should be given to the first control in the dialog that can
	receive focus. 
	
	
    
*/

BOOL CALLBACK __export SelectDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static LPSTR lpszCurAlias = NULL;

    /* What's the message? */

    switch (message)
      {

       /* message: initialize dialog box; lParam is argument to DialogBoxParam. */
       /*   lParam is far pointer to string to hold selected Alias.             */
       
       case WM_INITDIALOG:
			{
			 HWND hOwner = GetWindow(hDlg, GW_OWNER);
			 
			 /* Store string pointer for OK */
			
			 lpszCurAlias = (LPSTR) lParam;
			 *lpszCurAlias = '\0';
			
			
			 /* Initialize the list box */
			 InitDeviceList(GetDlgItem(hDlg, IDC_CB_ALIAS));
			 return (TRUE);
			}


       /* message: received a command */

       case WM_COMMAND:
	switch (LOWORD(wParam))
	  {
	   case IDOK:

	    /* Get the current selection from the control */

	    {
	     HWND hComboBox = GetDlgItem(hDlg, IDC_CB_ALIAS);
	     GetWindowText(hComboBox, lpszCurAlias, OLC_MAX_ALIAS_STR_SIZE);
	     
	     /* If we have a non-zero length string return TRUE */

	     EndDialog(hDlg, (lstrlen(lpszCurAlias) ? TRUE : FALSE));
	     return (TRUE);
	    }

	    case IDCANCEL:
	    EndDialog(hDlg, FALSE);
	    return (TRUE);
	  }
	break;

      }  /* end of switch(message) */


     /* Didn't process the message */

    return (FALSE);
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   InitDeviceList

    Description:
	This function initializes the specified combobox with the
	list of DT-Open Layers devices currently installed in the
	system.


    Input Arguments:
	HWND hComboBox          handle of the combo box

    Output Arguments:
	None

    Return Value (BOOL):
	if successful, TRUE; otherwise FALSE.
*/

BOOL PRIVATE InitDeviceList(HWND hComboBox)
{
    int iCount = 0;
    OLT_APISTATUS Status = OLC_STS_NORMAL;
    HGLOBAL hDevInfoList = NULL;
    LPOLT_IMGDEVINFO lpDevInfoList = NULL;
    int i = 0;


    /* How many aliases do we have */

    if ( (Status = OlImgGetDeviceCount(&iCount)) != OLC_STS_NORMAL )
      {
	  char msg[128];
	      wsprintf(msg, "OlImgGetDeviceCount failed (status = %#08lx).", Status);
	      MessageBox(NULL, msg, NULL, MB_ICONSTOP | MB_OK);
	  return FALSE;
      }

    /* Anything? */

    if ( !iCount )
       return TRUE;

    /* Allocate space for info */

    hDevInfoList = GlobalAlloc(GHND, iCount * sizeof(OLT_IMGDEVINFO));
    if ( !hDevInfoList )
      {
	  char msg[128];
	      wsprintf(msg, "Unable to allocate memory for device list; GlobalAlloc failed.");
	      MessageBox(NULL, msg, NULL, MB_ICONSTOP | MB_OK);
	  return FALSE;
      }   
      
    lpDevInfoList = (LPOLT_IMGDEVINFO) GlobalLock(hDevInfoList);
    if ( !lpDevInfoList )
      {
	  char msg[128];
	  GlobalFree(hDevInfoList);
	      wsprintf(msg, "Unable to lock memory for device list; GlobalLock failed.");
	      MessageBox(NULL, msg, NULL, MB_ICONSTOP | MB_OK);
	  return FALSE;
      }

    /* Fill in the device info */
    
    lpDevInfoList->StructSize = sizeof(OLT_IMGDEVINFO);
    if ( (Status = OlImgGetDeviceInfo(lpDevInfoList, iCount * sizeof(OLT_IMGDEVINFO))) )
      {
	  char msg[128];
	  GlobalUnlock(hDevInfoList);
	  GlobalFree(hDevInfoList);
	      wsprintf(msg, "OlImgGetDeviceInfo failed (status = %#08lx).", Status);
	      MessageBox(NULL, msg, NULL, MB_ICONSTOP | MB_OK);
	  return FALSE;
      }

    /* Add the aliases to the list box */

    for (i = 0; i < iCount; i++)
	ComboBox_AddString(hComboBox, lpDevInfoList[i].Alias);
    
    /* Select the first thing in list box (why not) */

    ComboBox_SetCurSel(hComboBox,0);

    /* Free the memory */

    GlobalUnlock(hDevInfoList);
    GlobalFree(hDevInfoList);

    return TRUE;
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   CloseCurrentDevice

    Description:
	This function closes the current device.

    Input Arguments:
	None

    Output Arguments:
	None

    Return Value (void):
	None
*/

void PRIVATE CloseCurrentDevice(void)
{

	/* Close current device */
	if (DeviceId1)
	   (void) OlImgCloseDevice(DeviceId1);
	
	DeviceId1 = NULL;
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   NewDevice

    Description:
	This function attempts to open the specified device.  If
	successful, it then queries the device for its capabilities
	and initializes the global data structures for future reference.

    Input Arguments:
	LPCSTR lpcszAlias   Alias of device to open

    Output Arguments:
	None

    Return Value (BOOL):
	TRUE if device was successfully opened, FALSE otherwise.


*/

BOOL PRIVATE NewDevice(LPCSTR lpcszAlias, HWND hWnd)
{
	ULNG ulOldTimeout;
	OLT_IMG_DEV_ID DevId = NULL;

	STRING			DeviceName = {0};
	OPENFILENAMEW	CamFileDlg = {0};
	OLT_APISTATUS	Status = 0;


   /* Try to open the device */
   if ( ! OlImgIsOkay( OlImgOpenDevice(lpcszAlias, &DeviceId1) ) )
	  return FALSE;
	
   /*set a 5 second timeout for driver calls */
   if (!OlImgIsOkay (OlImgSetTimeoutPeriod(DeviceId1,10,&ulOldTimeout)) )
		return FALSE;

	OlFgEnableLsMode(DeviceId1,TRUE);
	PostMessage(hWnd, WMUSER_INIT, 0, 0);		

   return TRUE;
}

