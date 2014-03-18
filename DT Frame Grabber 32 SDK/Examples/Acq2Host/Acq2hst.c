/*	File:	acq2hst.c

	Description:
		This file contains the Windows-related portion of the
		DTOL sample application Acq2Hst.

		Acq2Hst is a simple Windows application that allows you to
		acquire an image from a DT-Open Layers monochrome frame
		grabber and display that image in the client area as a DIB.
		The application allows you to set up the frame grabber's
		input if the frame grabber supports this capability.

		This file includes the following functions:

			WinMain() - 
				calls initialization function, processes
				message loop               
				
			InitApplication() - 
				initializes window data and registers window
			
			InitInstance() - 
				saves instance handle and creates main window
				
			MainWndProc() - 
				processes messages 
				
			SelectDeviceProc() - 
				dialog procedure for SelectDevice dialog box 
				
			InitDeviceList() - 
				initializes the device list for the Select Device 
				dialog box.
				
			CloseCurrentDevice() -
				closes the currently open device and frees associated 
				resources
				
			NewDevice() - 
				opens a new connection to a device driver and initializes 
				global information
				
			CreateDIBdisplay() -
				Uses data obtained from an acquire, puts into DIB form and
				displays it in the client area of the application.
			
			AcquireToHost() - 
				Performs the actual acquire 

	Comments:
		Windows can have several copies of your application running
		at the same time.  The variable hInst keeps track of which
		instance this application is so that processing will be to
		the correct window.

	Notes:
		This example program is based on the GENERIC sample included
		with the Microsoft Windows SDK.  All Microsoft disclaimers
		still apply.

		The source code in this file exceeds 80 columns so you may
		wish to print this file in "landscape" mode if your printer
		allows it.

*/
/*=========================================================================++
||									   ||
||	Copyright (C) 1996.   Data Translation, Inc., 100 Locke Drive,	   ||
||	Marlborough Massachusetts 01752-1192.				   ||
||									   ||
||	The information in this document is subject to change without	   ||
||	notice and should not be construed as a commitment by Data	   ||
||	Translation, Inc.  Data Translation, Inc. does not assume any	   ||
||	responsibility for any errors that may appear in this document.	   ||
||									   ||
||	This document, or source code, is provided solely as an example	   ||
||	of how to utilize Data Translation software libraries in a sample  ||
||	application.  Data Translation, Inc. does not assume any	   ||
||	responsibility for the use or reliability of any portion of this   ||
||	document or the described software.				   ||
||									   ||
||	General permission to copy or modify, but not for profit, is	   ||
||	hereby granted,  provided that the above copyright notice is	   ||
||	included and reference made to the fact that reproduction	   ||
||	privileges were granted by Data Translation, Inc.		   ||
||									   ||
||	Data Translation cannot assume any responsibility for the use,	   ||
||	or misuse, of any portion of this software for other than its	   ||
||	intended diagnostic purpose in calibrating and testing Data	   ||
||	Translation manufactured image processing boards and software.	   ||
||									   ||
++=========================================================================*/

/* Let's use strict type checking in the example */
#define STRICT

#define GLOBAL__
#include <math.h>
#include "acq2hst.h"			/* specific to this program */

/* Static variable used in this module */

static LPCSTR lpcszMainMenu = "Acq2HstMenu";				/* Name of menu resource in .RC file. */
static LPCSTR lpcszMainWinClass = "Acq2HstWClass";			/* Name used in call to CreateWindow. */
static LPCSTR lpcszMainWinTitle = "Acq2Hst Sample Application";		/* Text for window title bar. */
static HINSTANCE hInst;
static HDC hDCSave;; 										/* used for restoring the state upon exit */
static BOOL RGBDataFlag =0;
HWND hWnd;			/* Main window handle. */

// These have been moved from Acquire to this place for Drawing RGB data , hence they need 
// to be initialised in Acquire
	ULNG ulHeight = 0L;
	ULNG ulWidth = 0L;
	HGLOBAL hAcquireBuf = NULL;  
	HPUCHR hpAcquireBuf = NULL;              
	OLT_FG_FRAME_ID FrameId = 0;	
/* These are the pointer to the function related to the Dt3157 */

FARPROC	Dt3157EnableExposureMode;                           
FARPROC	Dt3157SetDigitalCameraType;             
FARPROC	Dt3157SetDigitalIOConfiguration;         
FARPROC	Dt3157SetDigitalIO;
FARPROC	Dt3157SetSyncMasterControlValue;
FARPROC	Dt3157SetExposure;
FARPROC	Dt3157EnableSyncMasterMode;                      
FARPROC	Dt3157QueryDigitalCameraType;                      
WORD DrawRGBFrame (HWND hWnd, VOID* FrameAddr, ULNG Width, ULNG Height);

#define VSRC0 0		// Video source

// Prototypes internal to this module
OLT_APISTATUS WriteCameraSetupToBoard ( HWND hWndDlg );
BOOL AsciiHexToDec (char * szBuff, ULNG * ulDecVal);
void MyPrintf(char* fmt, ...);		// Send to debug console with Printf format;


/**************************************************************************/
/**************************************************************************/

/*
	Function:	WinMain

	Description:
		This function calls the initialization function and processes
		the message loop.

		Windows recognizes this function by name as the initial entry
		point for the program.  This function calls the application
		initialization routine, if no other instance of the program is
		running.  It then executes a message retrieval and dispatch loop
		that is the top-level control structure for the remainder of 
		execution.  The loop is terminated when a WM_QUIT message is 
		received, at which time this function exits the application instance 
		by returning the value passed by PostQuitMessage().

		If this function must abort before entering the message loop,
		it returns the conventional value NULL.

		Refer to the "Functions" section of the Microsoft Windows SDK
		Programmers Reference for more detailed information.


	Input Arguments:
		HINSTANCE hInstance		Current instance
		HINSTANCE hPrevInstance		Previous instance
		LPSTR lpCmdLine			Points to a NULL-teminated string
						   specifying the command line for
						   the application.
		int nCmdShow			Specifies how the window is to be shown

	Output Arguments:
		None

	Returns: int
		The return value of PostQuitMessage or 0 if the function
		terminates prior to entering the message loop.
*/
HINSTANCE hDCIInst;

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;			/* message */
	HWND hExistingWnd = NULL;	/* handle to existing Window, if any */


    /* If we're running already, activate that one rather than starting again */

	if ( (hExistingWnd = FindWindow(lpcszMainWinClass, lpcszMainWinTitle)) != NULL )
	  {
	      ShowWindow(hExistingWnd, SW_RESTORE);
	      return 0;
	  }


    /* If no other instances are running, init the app.  This check should be    */
    /*    redundant due to the above check, but it doesn't really hurt anything. */

	if (!hPrevInstance)
	   if (!InitApplication(hInstance))
	      return 0;
	

    /* Perform initializations that apply to a specific instance */

	if (!InitInstance(hInstance, nCmdShow))
	   return 0;

    /* Acquire and dispatch messages until a WM_QUIT message is received. */

	while ( GetMessage(&msg, NULL, 0, 0) )
	  {
	      /* Translates virtual key codes */
	      TranslateMessage(&msg);

	      /* Dispatches message to window */
	      DispatchMessage(&msg);
	  }


    /* Returns the value from PostQuitMessage */

	return msg.wParam;
}


/**************************************************************************/
/**************************************************************************/

/*
	Function:	InitApplication

	Description:
		This function is called at initialization time only if no
		other instances of the application are running.  This function
		performs initialization tasks that can be done once for any
		number of running instances.

		In this case, we initialize a window class by filling out a
		data structure of type WNDCLASS and calling the Windows
		RegisterClass() function.  Since all instances of this
		application use the same window class, we only need to do
		this when the first instance is initialized.


	Input Arguments:
		HINSTANCE hInstance		Current (first) instance

	Output Arguments:
		None

	Returns:  BOOL
		TRUE if initialization was successfull, FALSE otherwise.
		It is common (especially in simple, pre-Win 3.1 apps, to
		return the value return by RegisterClass.
*/

BOOL PRIVATE InitApplication(HINSTANCE hInstance)
{
	WNDCLASS  wc;


    /* Fill in window class structure with parameters that describe the */
    /* main window.                                                     */

	wc.style = 0;					/* Class style(s). */
	wc.lpfnWndProc = MainWndProc;				/* Function to retrieve messages for */
								/*    windows of this class.         */
	wc.cbClsExtra = 0;					/* No per-class extra data. */
	wc.cbWndExtra = 0;					/* No per-window extra data. */
	wc.hInstance = hInstance;				/* Application that owns the class. */
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);		/* Default app icon */
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		/* Default arrow cursor */
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);		/* Use system defined background */
	wc.lpszMenuName = lpcszMainMenu;			/* Name of menu resource in .RC file. */
	wc.lpszClassName = lpcszMainWinClass;			/* Name used in call to CreateWindow. */


    /* Register the window class and return success/failure code. */

	return ( (BOOL) RegisterClass(&wc) );
}


/**************************************************************************/
/**************************************************************************/

/*
	Function:	InitInstance

	Description:
		This function is called at initialization time for every
		instance of this application.  This function performs
		initialization tasks that cannot be shared by multiple
		instances.

		In this case, we save the instance handle in a static
		variable and create and display the main program window.


	Input Arguments:
		HINSTANCE hInstance		Current (first) instance
		int nCmdShow			Param for first ShowWindow() call.

	Output Arguments:
		None

	Returns:  BOOL
		TRUE if we successfully created the window; FALSE otherwise.
*/

BOOL PRIVATE InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    /* Save the instance handle in static variable, which will be used in */
    /* many subsequence calls from this application to Windows.           */

	hInst = hInstance;


     /* Create a main window for this application instance. */

	hWnd = CreateWindow(
    			lpcszMainWinClass,		/* See RegisterClass() call. */
    			lpcszMainWinTitle,		/* Text for window title bar. */
    			WS_OVERLAPPEDWINDOW,            /* Window style. */
    			CW_USEDEFAULT,                  /* Default horizontal position. */
    			CW_USEDEFAULT,                  /* Default vertical position. */
    			CW_USEDEFAULT,                  /* Default width. */
    			CW_USEDEFAULT,                  /* Default height. */
    			NULL,                           /* Overlapped windows have no parent. */
    			NULL,                           /* Use the window class menu. */
    			hInstance,                      /* This instance owns this window. */
    			NULL);                          /* Pointer not needed. */

    /* If window could not be created, return "failure" */

	if (!hWnd)
    	   return (FALSE);

    /* Make the window visible; update its client area; and return "success" */

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	return (TRUE);
}


/**************************************************************************/
/**************************************************************************/

/*
	Function:	MainWndProc

	Description:
		This function is the primary message handler for the
		application.

	Input Arguments:
		HWND hWnd		window handle
		UINT message		message
		WPARAM wParam		16-bit message-specific data
		LPARAM lParam		32-bit message-specific data

	Output Arguments:
		None

	Returns:  LRESULT
		Message dependent return value is result of message
		processing.
*/


LRESULT CALLBACK __export MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  /* What message are we handling? */

	switch (message)
	  {   

	   /* message: command from application menu */

	   case WM_COMMAND:
		switch ( LOWORD(wParam) )
		  {

		   /*close the application*/
		   case ID_FILE_EXIT:
		   	DestroyWindow(hWnd);
		   	return 0;
		   	
		   /* Display the about box */
		   case IDM_ABOUT:
			DialogBox(hInst, 		/* current instance */
                		"AboutBox",		/* resource to use */
                		hWnd,			/* parent handle */
                		About);			/* About() instance address */
			break;


		   /* Select a device */
		   case IDM_SETUP_SELECT_DEVICE:
			{
			 char Alias[OLC_MAX_ALIAS_STR_SIZE];
			 BOOL bGotAlias = 0;

			 /* User picks the device */
			 if ( (bGotAlias = (BOOL) DialogBoxParam(hInst,
			 					MAKEINTRESOURCE(IDD_DEVICE_LIST),
			 					hWnd,
								SelectDeviceProc,
								(LPARAM)(LPSTR)Alias)) == -1 )
			   {
			       char msg[128];
			       wsprintf(msg, "Unable to display dialog box.");
			       MessageBox(hWnd, msg, "Select Device", MB_ICONEXCLAMATION | MB_OK);
			       break;
			   }

			 /* No alias means that no alias was selected or Cancel was pushed */

			 if ( !bGotAlias )
			    break;


			 /* Okay, we have a device alias.  Now it's time to open this */
			 /*   device.  Check to see if we already have a device open. */
			 /*   If so, see if user really wants to close it and open a  */
			 /*   new one.                                                */

			 if ( CurDevInfo.DevId != NULL )
			   {
			       char msg[128];
			       wsprintf(msg, "Do you want to close the currently open device (\"%s\")?",
			       		(LPSTR) CurDevInfo.Alias);
			       if ( MessageBox(hWnd, msg, "Select Device", MB_ICONEXCLAMATION | MB_YESNO) == IDNO )
			          break;
			       else
			          CloseCurrentDevice();
			   }


			 /* Okay, we have a device alias.  Now try to open it and read it's */
			 /*   capabilities.                                                 */

			 if ( !NewDevice(Alias) )
			   {
			       char msg[128];
			       wsprintf(msg, "Unable to open and initialize device (\"%s\").",
			       		(LPSTR) Alias);
			       MessageBox(hWnd, msg, "Select Device", MB_ICONSTOP | MB_OK);
			       break;
			   } 
			 EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SELECT_DEVICE, MF_GRAYED | MF_BYCOMMAND);
			 EnableMenuItem(GetMenu(hWnd), IDM_ACQUIRE, MF_ENABLED | MF_BYCOMMAND);
			 EnableMenuItem(GetMenu(hWnd), IDM_CLOSE_DEVICE, MF_ENABLED | MF_BYCOMMAND);  
			 DrawMenuBar(hWnd);
			
			}
			break; 
			
			

                   /*Close the currently opened device */
                   
           case IDM_CLOSE_DEVICE:
            CloseCurrentDevice();
			EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SELECT_DEVICE, MF_ENABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd), IDM_CLOSE_DEVICE, MF_GRAYED | MF_BYCOMMAND);  
			EnableMenuItem(GetMenu(hWnd), IDM_ACQUIRE, MF_GRAYED | MF_BYCOMMAND); 
			DrawMenuBar(hWnd);
			break;
                   	
		   /* Display the input source selection dialog box */

		   case IDM_SETUP_SOURCE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_INPUT_SOURCE), hWnd, InputSourceProc);
			break;


		   case IDM_SET_TIMEOUT:
		   	DialogBox(hInst, MAKEINTRESOURCE(IDD_TIMEOUT), hWnd, TimeoutProc);  
		   	break;

		   /* Display the input filter selection dialog box */

		   case IDM_SETUP_INPUT_FILTER:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_INPUT_FILTER), hWnd, InputFilterProc);
			break;


		   /* Display the A/D setup dialog box */

		   case IDM_SETUP_A2D:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_A2D), hWnd, A2DProc);
			break;


		   /* Display the video type selection dialog box */

		   case IDM_SETUP_VIDEO_SELECTION:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_VIDEO_TYPE), hWnd, VideoSelectProc);
			break;


		   /* Display the frame type selection dialog box */

		   case IDM_SETUP_FRAME_TYPE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_FRAME_TYPE), hWnd, FrameTypeProc);
			break;


		   /* Display the Sync Sentinel setup dialog box */

		   case IDM_SETUP_SYNC_SENTINEL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SYNC_SENTINEL), hWnd, SyncSentinelProc);
			break;


		   /* Display the pixel clock setup dialog box */

		   case IDM_SETUP_PIXEL_CLOCK:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CLOCK), hWnd, PixelClockProc);
			break;


		   /* Display the active video area setup dialog box */

		   case IDM_SETUP_ACTIVE_VIDEO:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_VIDEO_AREA), hWnd, VideoAreaProc);
			break;


		   /* Display the frame size setup dialog box */

		   case IDM_SETUP_FRAME_SIZE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_FRAME_SIZE), hWnd, FrameSizeProc);
			break;

		   /* Perform an acquire and display the image */
		    
            case IDM_ACQUIRE:
               	AcquireToHost(CurDevInfo);
            break;       
                    	
		   /* Let Windows process it */
		   
		   default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		  }


	   /* message: popup menu about to be displayed */

	   case WM_INITMENUPOPUP:
		{
		 HMENU hMenu = (HMENU) wParam;
		 HMENU hMenuSetup = GetSubMenu(GetMenu(hWnd), 1);
		 
		 /* Enable/Disable items on the Setup menu based on the capabilities
		    of the device (determined in NewDevice) and stored in CurDevCaps. */

		 if ( hMenu == hMenuSetup )
		   {
			UINT uiFlags;  

			uiFlags = (CurDevCaps.SourceCount > 1) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_SETUP_SOURCE, uiFlags);

		    EnableMenuItem(hMenu, IDM_SET_TIMEOUT, uiFlags);
		    
		    uiFlags = (CurDevCaps.DoesInputFilter) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
		    EnableMenuItem(hMenu, IDM_SETUP_INPUT_FILTER, uiFlags);
		    
		    uiFlags = (CurDevCaps.DoesA2D) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
		    EnableMenuItem(hMenu, IDM_SETUP_A2D, uiFlags);
		    
		    uiFlags = (CurDevCaps.DoesVideoType) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
		    EnableMenuItem(hMenu, IDM_SETUP_VIDEO_SELECTION, uiFlags);
		    
		    uiFlags = (CurDevCaps.FrameTypes) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
		    EnableMenuItem(hMenu, IDM_SETUP_FRAME_TYPE, uiFlags);
		    
		    uiFlags = (CurDevCaps.DoesSyncSentinel) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
		    EnableMenuItem(hMenu, IDM_SETUP_SYNC_SENTINEL, uiFlags);
		    
		    uiFlags = (CurDevCaps.DoesClock) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
		    EnableMenuItem(hMenu, IDM_SETUP_PIXEL_CLOCK, uiFlags);
		    
		    uiFlags = (CurDevCaps.DoesActiveVideo) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
		    EnableMenuItem(hMenu, IDM_SETUP_ACTIVE_VIDEO, uiFlags);
		    
		    uiFlags = (CurDevCaps.DoesFrameSize) ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED);
		    EnableMenuItem(hMenu, IDM_SETUP_FRAME_SIZE, uiFlags);
		   }
		 else
        	    return DefWindowProc(hWnd, message, wParam, lParam);
		}
        	break;


	   /* message: window being created */

	   case WM_CREATE:
		{
		 HMENU hMainMenu = GetMenu(hWnd);
		 
		 /* Disable Acquire! on the Main menu to start out. It becomes
		    enabled when a device is opened. */

		 EnableMenuItem(hMainMenu, IDM_ACQUIRE, (MF_BYCOMMAND | MF_GRAYED));

		 /* This will allow us to restore the original palette upon exit since we change it during */
		 /* acquries.  In most cases this is done with the value returned from SelectPalette but for */
		 /* the purposes of this example Saving the DC will provide the same service */
			
		 hDCSave = GetDC(hWnd);
		 SaveDC( hDCSave);
		}
        	break;

	   case WM_PAINT:
			   {
				PAINTSTRUCT ps;
				BeginPaint(hWnd, &ps);
				if(RGBDataFlag )
					DrawRGBFrame(hWnd,hpAcquireBuf,ulWidth , ulHeight  );
				EndPaint(hWnd, &ps);
				}
			   break;

	   /* message: window being destroyed */

	   case WM_DESTROY:
		if (CurDevInfo.DevId != NULL)
		   CloseCurrentDevice();
		RestoreDC( hDCSave, -1);
		ReleaseDC(hWnd, hDCSave);
		PostQuitMessage(0);
        	break;


	   /* Pass it on if unproccessed */

	   default:
        	return DefWindowProc(hWnd, message, wParam, lParam);
	}


    /* By default, return zero for any message we handled that didn't require */
    /*   a specific return value.                                             */

	return (LRESULT) 0L;
}




/****************************************************************************/
/****************************************************************************/

/*	Function:	SelectDeviceProc

	Description:
		This function is the dialog procedure for the dialog box used to
		select the DT-Open Layers Imaging frame grabber to use.
	
	Initialization:
		This dialog box should be invoked using DialogBoxParam.  The
		lParamInit argument should be a far pointer to a string at least
		MAX_ALIAS_STR_SIZE characters long that will receive the selected
		device alias.

	Input Arguments:
		HWND hDlg		dialog handle
		UINT message		message
		WPARAM wParam		16-bit message-specific data
		LPARAM lParam		32-bit message-specific data

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

		 assert(lParam != 0L);
		 lpszCurAlias = (LPSTR) lParam;
		 *lpszCurAlias = '\0';

		 /* Center the dialog box */

		 CenterWindow(hDlg, hOwner);

		 /* Initialize the list box */

		 InitDeviceList(GetDlgItem(hDlg, IDC_CB_ALIAS));
		 return (TRUE);
		}


	   /* message: received a command */

	   case WM_COMMAND:
		switch (wParam)
		  {
		   case IDOK:

			/* Get the current selection from the control */

			{
			 HWND hComboBox = GetDlgItem(hDlg, IDC_CB_ALIAS);
			 ComboBox_GetText(hComboBox, lpszCurAlias, OLC_MAX_ALIAS_STR_SIZE);
			 
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

/*	Function:	InitDeviceList

	Description:
		This function initializes the specified combobox with the
		list of DT-Open Layers devices currently installed in the
		system.


	Input Arguments:
		HWND hComboBox			handle of the combo box

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

/*	Function:	CloseCurrentDevice

	Description:
		This function closes the current device and initializes the
		two global device info structures to zeros.

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

	if(RGBDataFlag)
	{
		ulHeight = 0L;
		ulWidth = 0L;
		GlobalUnlock(hAcquireBuf);
		GlobalFree(hAcquireBuf);
		hAcquireBuf = NULL;  
		hpAcquireBuf = NULL;              
	}

	if (CurDevInfo.DevId != NULL)
	   (void) OlImgCloseDevice(CurDevInfo.DevId);

    /* Clear the device info structures */

	_fmemset((LPVOID)&CurDevCaps, 0, sizeof(DEVCAPS));
	_fmemset((LPVOID)&CurDevInfo, 0, sizeof(DEVINFO));
}


/**************************************************************************/
/**************************************************************************/

/*	Function:	NewDevice

	Description:
		This function attempts to open the specified device.  If
		successful, it then queries the device for its capabilities
		and initializes the global data structures for future reference.

	Input Arguments:
		LPCSTR lpcszAlias	Alias of device to open

	Output Arguments:
		None

	Return Value (BOOL):
		TRUE if device was successfully opened, FALSE otherwise.

	Comments:  This example only supports a single ILUT.  We assume this
		to be ILUT 0 and initialize only this ILUT.  Should future
		devices support more than 1 ILUT, they can be initialized in a 
		similar manner. 

*/

BOOL PRIVATE NewDevice(LPCSTR lpcszAlias)
                                                                                                                                       {
	OLT_IMG_DEV_ID DevId = NULL;
	OLT_APISTATUS Status = OLC_STS_NORMAL;
    LPULNG lpLutData = NULL;
    HLOCAL hLutData = NULL;
    ULNG ulResult = 0L;
    ULNG ulLutCount = 0L;
    ULNG ulMaxIndex = 0L;
    ULNG ulMaxValue = 0L;
    ULNG ulOldData = 0L;
	ULNG ulPixelDepth = 0L;
	ULNG i, j;
	UINT uStep;	
	STRING	   DeviceName = {0};

         
    /* Try to open the device */

	if ( ! OlImgIsOkay( OlImgOpenDevice(lpcszAlias, &DevId) ) )
	   return FALSE;
	CurDevInfo.DevId = DevId;
        CurDevInfo.InputSource = 0;

	 /**************************************************************************
	  This section has been added to detect a DT3157 board to allow the user
	  to supply a *.cam file to configure the board for the current digital
	  camera used.
	 
	  We query the driver for the kind of board that has been opened. 
	  (No matter the previous ones). We query for the key "device name".
	  If the board is a DT3157, then we launch do the following:
	  
		  - load the DT315732.DLL that contains the 3157 specific functions
		  - check if the loader succeeded
		  - ask for the address of the functions we need
		  - check if the DLL contained all the functions we needed
		  - launch the dialog box and then supply the file name 
			to the configuration routine.

	 **************************************************************************/

	 OlImgQueryDeviceCaps(CurDevInfo.DevId,OLC_IMG_DC_DEVICE_NAME,DeviceName,sizeof(STRING));
	 if (!strcmp("DT3157",DeviceName)) 
	 {
		  HINSTANCE		DllHandle = NULL;
		  		
		  // Open the DLL
		  DllHandle = LoadLibrary("DT315732.DLL");

		  if (DllHandle) // Did the loader succeed
		  {	   // If yes, get the addresses of the functions

			   Dt3157EnableExposureMode	= GetProcAddress(DllHandle,"Dt3157EnableExposureMode");
			   Dt3157SetDigitalCameraType =	GetProcAddress(DllHandle,"Dt3157SetDigitalCameraType");
			   Dt3157SetDigitalIOConfiguration = GetProcAddress(DllHandle,"Dt3157SetDigitalIOConfiguration");
			   Dt3157SetDigitalIO = GetProcAddress(DllHandle,"Dt3157SetDigitalIO");
			   Dt3157SetSyncMasterControlValue = GetProcAddress(DllHandle,"Dt3157SetSyncMasterControlValue");
			   Dt3157SetExposure = GetProcAddress(DllHandle,"Dt3157SetExposure");
			   Dt3157EnableSyncMasterMode =	GetProcAddress(DllHandle,"Dt3157EnableSyncMasterMode");
			   Dt3157QueryDigitalCameraType = GetProcAddress(DllHandle,"Dt3157QueryDigitalCameraType");

			   if (!(	 Dt3157EnableExposureMode			&&
						 Dt3157SetDigitalCameraType			&&
						 Dt3157SetDigitalIOConfiguration	&&
						 Dt3157SetDigitalIO					&&
						 Dt3157SetSyncMasterControlValue	&&
						 Dt3157SetExposure					&&
						 Dt3157EnableSyncMasterMode			&&
						 Dt3157QueryDigitalCameraType) ) // One of those failed ?
			   {	// Yes, display a message box and exit
					MessageBox (   NULL,
								   "You do not have the correct version of DT315732.DLL",
								   "Error loading function",
								   MB_APPLMODAL );
					exit(0);
			   }
		  }
		  else
		  {	   // No, display a message box and exit
			   MessageBox (   NULL,
			   				  "File dt315732.dll not found",
			   				  "Error loading DLL",
			   				  MB_APPLMODAL );
			   exit(0);
		  }

		  Status = WriteCameraSetupToBoard (hWnd);
		  MyPrintf("Loading Camera File Status = %x",Status);
		  if (Status == OLC_STS_CANCELJOB)
		  {
			  if (CurDevInfo.DevId != NULL)
				   Status = OlImgCloseDevice(CurDevInfo.DevId);

				/* Clear the device info structures */

			  if(Status == OLC_STS_NORMAL)
			  {
				_fmemset((LPVOID)&CurDevCaps, 0, sizeof(DEVCAPS));
				_fmemset((LPVOID)&CurDevInfo, 0, sizeof(DEVINFO));
			  }
			  return FALSE;
		  }
	 }

	 if (	(!strcmp("DT3153",DeviceName)) || 
			(!strcmp("DT3154",DeviceName)) || 
			(!strcmp("DT3133",DeviceName)) ||
			(!strcmp("DT3132",DeviceName)) ||
			(!strcmp("DT3131",DeviceName)) ||
			(!strcmp("DT3130",DeviceName)) ||
			(!strcmp("DT3120",DeviceName)) 
		) 
	 {

		// This is harcoded to avoid having to include 
		// header files that are related only to the
		// color SDK.
		#define	WRITE	8
		#define RGB_32	7
	
		OLT_APISTATUS (*StorageMode) (OLT_IMG_DEV_ID,unsigned int,unsigned int,unsigned int*);
		HINSTANCE DtColorLib;
		unsigned int	value = RGB_32;

		StorageMode = NULL;
		DtColorLib = LoadLibraryEx("DtColorSDK",NULL,0);

		StorageMode = (void*) GetProcAddress(DtColorLib,"DtColorStorageMode");

		if (StorageMode)
		{
			// Set the board in 32 bits RGB
			StorageMode(CurDevInfo.DevId,0,WRITE,&value);
		}

		 RGBDataFlag=TRUE;
	 }
	 //**************************************************************************
	 //**************************************************************************

    /* Got it, now query to see which sections are supported by the device.
       This information is stored in the GLOBAL CurDevCaps and is used to 
       enable/disable menus in WM_INITMENUPOPUP */

	Status = OlImgQueryDeviceCaps(DevId,
				OLC_IMG_DC_SECTIONS,
				&ulResult,
				sizeof(ULNG));

	if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }

    /*  Check to see if the board has an INPUT Section.  All devices should
        have an INPUT section but the same sort of check would be needed to
        determine if the device had CAMERA CTL, LINEAR MEMORY or on board 
        MEMORY sections. */ 

	if (! (ulResult & OLC_FG_SECTION_INPUT))
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }
	  
    /*  Okay the board has Input Capabilities, now check for ILUT's */

	Status = OlFgQueryInputCaps(DevId, 
				OLC_FG_IC_ILUT_COUNT,
				&ulLutCount,
				sizeof(ULNG)); 
	
	if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }
	

        /* Do we have at least 1 ILUT? */
        
	if ( ulLutCount > 0 )
	  {

	     /* Check the max ILUT index */
	
	     Status = OlFgQueryInputCaps(DevId, 
				OLC_FG_IC_MAX_ILUT_INDEX,
				&ulMaxIndex,
				sizeof(ULNG)); 
	
	     if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	       {
		  CloseCurrentDevice();
		  return FALSE;
	       }

	     /* Get the number of bytes per pixel for loading the ILUT */

             Status = OlFgQueryInputCaps(DevId,
             				OLC_FG_IC_PIXEL_DEPTH,
             				&ulPixelDepth,
             				sizeof(ULNG));
             				
	     if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	       {
	         CloseCurrentDevice();
	         return FALSE;
	       }  

	     /* Allocate the array to store the ILUT values */
	     
             hLutData =  LocalAlloc(LHND, (UINT)((ulMaxIndex + 1) * sizeof(ULNG)) );
             if (!hLutData)
               {
                 CloseCurrentDevice();
                 return FALSE;
               }
               
             lpLutData = (LPULNG) LocalLock(hLutData);
             if (!lpLutData)
               {
	         	 LocalFree(hLutData);
	         	 CloseCurrentDevice();
	         	 return FALSE;
               }
             
	     /* Check the max value for an ILUT entry */

	     Status = OlFgQueryInputCaps(DevId, 
				OLC_FG_IC_MAX_ILUT_VALUE,
				&ulMaxValue,
				sizeof(ULNG)); 
	
	     if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	       {
	         LocalUnlock(hLutData);
	         LocalFree(hLutData);
	         CloseCurrentDevice();
	         return FALSE;
	       }  
	  
	     /* Calculate the increment step for the case that there are more 
	        Indexes than Values in the table (i.e. 16 bit to 8 bit). In most 
	        cases this value will be 1 since the number of table Indexes is 
	        usually equal to the Max possible value (i.e. 16 bit to 16 bit or
	        8 bit to 8 bit). */
	         
             uStep = (UINT)(ulMaxIndex / ulMaxValue);  

	     /* Once the step is calculated the ILUT will be loaded as follows:
	     
	     	  0	----------
	     		|	0    |
	     		|	.    |	     	
	     		|	.    |
	     		|	.    |
	     1 * step--------|
	     		|	1    |
	     		|	.    |
	     		|	.    |
	     		|	.    |
	     2 * step--------|
	     		|	2    |
	     		|	.    |
	     		|   .    |
	     		|	.    |
	     3 * step--------|
	     		|	3    |
	     		|	.    |
	     		|	.    |
	     		|	.    |
	     		----------
	                                                        */

             for (i = 0; i < ulMaxIndex; i += uStep)
                for (j = 0; j < uStep; j++)
		  
		  /* Load with the identity */
                lpLutData[i + j] = i / uStep;

	     /* Write the values to ILUT 0. */
	    
	     Status = OlFgWriteInputLUT(DevId,
	    				0, 
	    				0,
	    				ulMaxIndex,
	    				lpLutData);
	    				

	     if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	       {
	         LocalUnlock(hLutData);
	         LocalFree(hLutData);
	         CloseCurrentDevice();
	         return FALSE;
	       }  

	     /* Query for the current Video Input Source for setting the ILUT
	        input control value */

	     Status  = OlFgQueryInputVideoSource(DevId,
	     					&CurDevInfo.InputSource);
	     				

	     if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	       {
	         LocalUnlock(hLutData);
	         LocalFree(hLutData);
	         CloseCurrentDevice();
	         return FALSE;
	       }  

	     /* Tell the board to use ILUT 0 */
	    
	     Status = OlFgSetInputControlValue(DevId,
					CurDevInfo.InputSource,
	    				OLC_FG_CTL_ILUT,
					0,
					&ulOldData);
						
	     if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	       {
	         LocalUnlock(hLutData);
	         LocalFree(hLutData);
	         CloseCurrentDevice();
	         return FALSE;
	       }  


		/* Free the memory */
		
         LocalUnlock(hLutData);
         LocalFree(hLutData);


	  }     /* end if ILUT > 0 */

	   
    /* Query input filter caps */

	Status = OlFgQueryInputCaps(DevId, 
				OLC_FG_IC_DOES_INPUT_FILTER,
				&CurDevCaps.DoesInputFilter,
				sizeof(BOOL)); 
				
	if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }


    /* Query A/D */  
	
	Status = OlFgQueryInputCaps(DevId,
				OLC_FG_IC_DOES_PROG_A2D,
				&CurDevCaps.DoesA2D,
				sizeof(BOOL));
				
	if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }


    /* Query video type stuff */
	
	Status = OlFgQueryInputCaps(DevId,
				OLC_FG_IC_DOES_VIDEO_SELECT,
				&CurDevCaps.DoesVideoType,
				sizeof(BOOL)); 
				
	if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }


    /* Query active video area stuff */
	
	Status = OlFgQueryInputCaps(DevId,
				OLC_FG_IC_DOES_ACTIVE_VIDEO,
				&CurDevCaps.DoesActiveVideo,
				sizeof(BOOL));
				
	if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }


    /* Query frame size stuff */ 
	
	Status = OlFgQueryInputCaps(DevId,
				OLC_FG_IC_DOES_FRAME_SELECT,
				&CurDevCaps.DoesFrameSize,
				sizeof(BOOL)); 
				
	if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }


    /* Query clock stuff */
	
	Status = OlFgQueryInputCaps(DevId,
				OLC_FG_IC_DOES_PIXEL_CLOCK,
				&CurDevCaps.DoesClock,
				sizeof(BOOL));
				
	if ( ! OlImgIsOkay(Status)  &&  (Status != OLC_STS_UNSUPKEY) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }
		   

    /* Query frame type stuff */

	Status = OlFgQueryInputCaps(DevId,
				OLC_FG_IC_FRAME_TYPE_LIMITS,
				&CurDevCaps.FrameTypes, 
				sizeof(ULNG)); 
				
	if ( ! OlImgIsOkay(Status) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }
		   

   /* Query Sync Sentinel stuff */
	
	Status = OlFgQueryInputCaps(DevId,
				OLC_FG_IC_DOES_SYNC_SENTINEL,
				&CurDevCaps.DoesSyncSentinel,
				sizeof(BOOL));  
				
	if ( ! OlImgIsOkay(Status) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }

	/* Query Input Source Channels  */
    Status = OlFgQueryInputCaps(CurDevInfo.DevId,
                OLC_FG_IC_INPUT_SOURCE_COUNT,
                &CurDevCaps.SourceCount,
                sizeof(ULNG));

	if ( ! OlImgIsOkay(Status) )
	  {
	      CloseCurrentDevice();
	      return FALSE;
	  }


	return TRUE;
}


/***************************************************************************/
/***************************************************************************/

/*	Function:	CreateDIBDisplay

	Description:
		This function allocates memory for the DIB header and
		associated pallete.


	Input Arguments:
		HPUCHR hpAcquireBuf		Huge pointer to the actual acquire
						data.
		ULNG ulHeight			The height the acquire buffer
		
		ULNG ulWidth			The width of the acquire buffer
		
		ULNG ulPixelDepth		The size of each pixel

	Output Arguments:
		None.

	Return Value (LRESULT):
		0 for success; -1 otherwise


	Comments:
		If the client area is resized or another application is placed 
		over it, the DIB data that has been blasted to the display 
		(using SetDIBbits) will no longer appear.  The actual blasting
		bits to the display should be done in response to a WM_PAINT message
		so the client area of the application is updated appropriately. For
		the purposes of this example, everything necessary to create a DIB
		and display it in the client area is done in this function.  The intent
		of this function was merely to show a sequence of necessary steps rather 
		than repainting issues.  This function assumes 8 bit DIB data.

*/

LRESULT PRIVATE CreateDIBDisplay(HPUCHR hpAcquireBuf, ULNG ulHeight, ULNG ulWidth, ULNG ulPixelDepth)                                                                                        

{
 	int i = 0, r = 0;
	HDC hdc;
	HPALETTE hGrayscalePal;
	HWND hWnd;
	HPUCHR hpTempAcquireBuf = NULL;
	HPUCHR hpTempDIBdata = NULL;  
	HPUSHRT hpTempAcquireWordBuf = NULL; // used when pixel depth = 2
	HGLOBAL hPal = NULL;
	LPLOGPALETTE lpPal = NULL;
	HGLOBAL hDIBdata = NULL;
	HPUCHR hpDIBdata = NULL;
	HGLOBAL hDIBheader = NULL;
	BITMAPINFO * lpDIBheader = NULL;
	LONG lCameraType;
	ULNG ulBitsGreaterThan8, ulBitsPerSample;
	OLT_APISTATUS Status = 0L;
	POINTERLIST PointerList = {0};		/* used strictly for cleanup */
          
    /* Allocate space for a DIB header */   
	hDIBheader = GlobalAlloc(GHND, sizeof(BITMAPINFO) + (255 * sizeof(RGBQUAD)));    
	PointerList.hDIBheader = hDIBheader;
        if (!hDIBheader)
          {
             CleanupDIBPointers(&PointerList);
             return -1; 
        }

    /* Allocate space for a logical palette */
	
	hPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + (255 * sizeof(PALETTEENTRY)));
	PointerList.hPal = hPal;
	if (!hPal)
	  {                          
	     CleanupDIBPointers(&PointerList);
	     return -1;
	  }

     /* Allocate space for the actual DIB data */
        
        hDIBdata = GlobalAlloc(GHND, ulWidth * ulHeight); // only want an 8bit DIB * ulPixelDepth);  
        PointerList.hDIBdata = hDIBdata;
	if (!hDIBdata )
	  {
             CleanupDIBPointers(&PointerList);
             return -1; 
 	  }


        /* Lock the space so we can get pointers to the data */

	lpDIBheader = GlobalLock(hDIBheader);  
	PointerList.lpDIBheader = lpDIBheader;
	if (!lpDIBheader)
	  {
             CleanupDIBPointers(&PointerList);
             return -1; 
 	  }

	lpPal = GlobalLock(hPal);             
	PointerList.lpPal = lpPal;
	if (!lpPal)
	  {
             CleanupDIBPointers(&PointerList);
             return -1; 
 	  }

	hpDIBdata = (HPUCHR) GlobalLock(hDIBdata);
	PointerList.hpDIBdata = hpDIBdata;
        if (!hDIBdata)
          {
             CleanupDIBPointers(&PointerList);
             return -1; 
          }

    /* Fill in the static parts of the DIB header */

	lpDIBheader->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        lpDIBheader->bmiHeader.biWidth = ulWidth;
        lpDIBheader->bmiHeader.biHeight = ulHeight;
	lpDIBheader->bmiHeader.biPlanes = 1;

	/* this value must be 1, 4, 8 or 24 so PixelDepth can only be a 1 or 3; we assume a 1 here */
	lpDIBheader->bmiHeader.biBitCount = (WORD)(8);// only want 8 bit DIB * ulPixelDepth);                  
	lpDIBheader->bmiHeader.biCompression = BI_RGB; 

	/* Fill in the static parts of the logical palette */

	lpPal->palVersion = 0x300;
	lpPal->palNumEntries = 256;

    /* Initialize the logical and DIB palettes to grayscale; 128 grayscales are sufficient */

	for (i = 0; i < 256; i+=2)
	  {
	   lpDIBheader->bmiColors[i].rgbRed = lpDIBheader->bmiColors[i + 1].rgbRed = (BYTE) i;
	   lpDIBheader->bmiColors[i].rgbGreen = lpDIBheader->bmiColors[i + 1].rgbGreen = (BYTE)i;
	   lpDIBheader->bmiColors[i].rgbBlue = lpDIBheader->bmiColors[i + 1].rgbBlue = (BYTE)i;
	   lpPal->palPalEntry[i].peFlags = lpDIBheader->bmiColors[i].rgbReserved = 0;
	   lpPal->palPalEntry[i].peRed =  lpPal->palPalEntry[i + 1].peRed = (BYTE)i;
	   lpPal->palPalEntry[i].peGreen = lpPal->palPalEntry[i + 1].peGreen = (BYTE)i;
	   lpPal->palPalEntry[i].peBlue = lpPal->palPalEntry[i + 1].peBlue = (BYTE)i;
	  }

    /* Try and create a palette */

	hGrayscalePal = CreatePalette(lpPal);
	PointerList.hGrayscalePal = hGrayscalePal;
	if ( !hGrayscalePal )
	  {
         CleanupDIBPointers(&PointerList);
	     return -1;
	  }
	

   /* use temporary pointers to move through acquire and bitmap data */
    hpTempDIBdata = hpDIBdata;
	
	if (ulPixelDepth != 2)
	{
		hpTempAcquireBuf = hpAcquireBuf;
		hpTempAcquireBuf += (ulHeight - 1) * (ulWidth * ulPixelDepth);

		/* Load bitmap top to bottom starting with the bottom row of the acquire data. */
	
		for (i = 0; i < (int) ulHeight; i++)
        {
			 _fmemcpy(hpTempDIBdata, hpTempAcquireBuf, (size_t) (ulWidth * ulPixelDepth));
			 hpTempDIBdata += ulWidth * ulPixelDepth;		
			 hpTempAcquireBuf -= ulWidth * ulPixelDepth;
        }

	}
	else 
	{	// by getting word pointers, the data will now inc/dec by two bytes(one pixel) per inc/dec
		hpTempAcquireWordBuf = (HPUSHRT)hpAcquireBuf;
		// get pointer to beginning of last line in acquire buffer
		hpTempAcquireWordBuf += (ulHeight - 1) * ulWidth;

	/* Load bitmap top to bottom starting with the bottom row of the acquire data.
       Since the function assumes an 8 bit DIB, if ulPixelDepth is greater than 1
       we must render the data to fit into the 8 bit DIB. */

	   // find the difference between bits per pixel and 1 byte (8 bits). This
	   // tells us how much we have to shift to display the most significant 8
	   // bits

      Status = Dt3157QueryDigitalCameraType(CurDevInfo.DevId, VSRC0, &lCameraType);
	  if(Status != OLC_STS_NORMAL)
	  {
		  return -1;
	  }


	   switch (lCameraType)
	   {
	   case DT3157_DIGCAM_16BIT_INPUT:
		   ulBitsPerSample = 16;
		   break;

	   case DT3157_DIGCAM_14BIT_INPUT:
		   ulBitsPerSample = 14;
		   break;

	   case DT3157_DIGCAM_12BIT_INPUT:
		   ulBitsPerSample = 12;
		   break;

	   case DT3157_DIGCAM_10BIT_INPUT:
		   ulBitsPerSample = 10;
		   break;

	   case DT3157_DIGCAM_8BIT_SINGLE_CHANNEL_INPUT:
		   ulBitsPerSample = 8;
		   break;

	   case DT3157_DIGCAM_8BIT_DUAL_CHANNEL_INPUT:
		   ulBitsPerSample = 8;
		   break;

       default:
		   ulBitsPerSample = 32;
	   }
	   ulBitsGreaterThan8 = ulBitsPerSample - 8;	

	   // now loop through the whole image pixel by pixel so that they
	   // can be shifted if necessary
	   // divide Mem siz by width of a line, to determine rows(height)
	   for (r = 0; r < (int)ulHeight; r++)  
	    {
		   // copy each individual line of rendered data to the new buffer
		   for (i = 0; i < (int)ulWidth; i++)  
		  {
			*hpTempDIBdata = (BYTE)(*hpTempAcquireWordBuf >> ulBitsGreaterThan8);
			hpTempAcquireWordBuf++;  //increment on the current line
	        hpTempDIBdata ++;
		  }

		  hpTempAcquireWordBuf -= (ulWidth * 2); // now lets decrement back to the start of the 
		  // next line, because we incremented the pointer a whole line during the transfer
		  // process, we need to go back two lines worth.
		  //
		}
     }

	/* Lets Select and Realize the palette we created earlier (this has to be done
	   before creating the device dependent bitmap from the DIB info  */ 

	hWnd = GetActiveWindow();
	hdc = GetDC(hWnd);

	/* save the old palette to restore later */

	SelectPalette(hdc, hGrayscalePal, FALSE);
	RealizePalette(hdc);

	/* blast the bitmap bits directly to the device context surface (our client area) */

	SetDIBitsToDevice(hdc,		/* handle of device context */
			0,		/* x-coordinate origin of destination rect */
			0,		/* y-coordinate origin of destination rect */
			(int)ulWidth,	/* rectangle width */
			(int)ulHeight,	/* rectangle height */
			0,		/* x-coordinate origin of source rect */
			0,		/* y-coordinate origin of source rect */
			0,		/* number of first scan line in array */
			(UINT)ulHeight,	/* number of scan lines	*/
			hpDIBdata,	/* address of array with DIB bits */
			lpDIBheader,	/* address of structure with bitmap info */
			DIB_RGB_COLORS);/* RGB or palette indices */
	
	/* clean up */

	ReleaseDC(hWnd,hdc);
    CleanupDIBPointers(&PointerList);

	return 0;
}



/********************************************************************/
/********************************************************************/


/*	Function:	AcquireToHost

	Description:
		This will acquire one frame of data (a frame being defined in the 
		frame type dialog) to host memory.  It then uses the acquire data
		from the board and calls CreateDIBDIsplay to actually display it
		in the client area of the application.
			
	Initialization:
		
	Input Arguments:
		HWND hDlg		dialog handle
		UINT message		message
		WPARAM wParam		16-bit message-specific data
		LPARAM lParam		32-bit message-specific data

	Output Arguments:
		None

	Return: void 
		None

	Comments:
		Before Performing an Acquire, a call to OlImgQueryDeviceCaps should be
		made using the OLC_IMG_DC_SECTIONS key.  If this query returns TRUE then
		separate input queries should be made to find out if the board supports
		INPUT operations, LINEAR (memory) operations, CAMCTL (camera control) operations
		and MEMORY (on board) operations.  If the board does support any of the above
		operations, operation specific queries should be made to determine specific
		features (ie, if the board supports CAMCTL, a call to OlImgQueryCameraControlCaps
		should be made). Since this program assumes the board is capapble of INPUT operations
		and specific queries to OlImgQueryInputControlCaps have already been made (the results 
		of these queries were used to Enable or Disable menu options under Setup in the main menu),
		a call to OlImgQueryDeviceCaps is not necessary unless support for LINEAR, CAMCTL and 
		MEMORY options is desired.
		  
		              
**************************************************************************************/

void PRIVATE AcquireToHost(DEVINFO DevInfo )
{       
	OLT_APISTATUS Status = 0L;
	ULNG ulPixelDepth = 0L;
	ULNG ulMinBufSize = 0L; 
	
	// if using RGB drawing release old resources from last acquire
	if((RGBDataFlag) && (ulHeight))
	{
		ulHeight = 0L;
		ulWidth = 0L;  
		GlobalUnlock(hAcquireBuf);
		GlobalFree(hAcquireBuf);	
	}			 
	hAcquireBuf = NULL;  
	hpAcquireBuf = NULL;              

	/* Query for Height, Width and Pixel Depth of the frame */

	(void) OlFgQueryInputControlValue(DevInfo.DevId,
		 			DevInfo.InputSource,
		 			OLC_FG_CTL_FRAME_HEIGHT,
		 			&ulHeight);

	(void) OlFgQueryInputControlValue(DevInfo.DevId,
		 			DevInfo.InputSource,
		 			OLC_FG_CTL_FRAME_WIDTH,
		 			&ulWidth);

	(void) OlFgQueryInputCaps(DevInfo.DevId,
		 		OLC_FG_IC_PIXEL_DEPTH,
		 		&ulPixelDepth,
		 		sizeof(ULNG));

	/* calculate minimum size of buffer in bytes */	 			

	ulMinBufSize = ulHeight * ulWidth * ulPixelDepth;
	
	hAcquireBuf = GlobalAlloc(GHND, ulMinBufSize); 

	if ( !hAcquireBuf ) 
	  {                                           
	      /* unable to allocate memory */

	      char msg[OLC_MAX_STATUS_MESSAGE_SIZE + 60];
	      wsprintf(msg, "Unable to allocate enough memory for acquire.");
	      MessageBox(NULL, msg, "Acquire Error", MB_ICONSTOP | MB_OK);
	      return; 
	  }

	hpAcquireBuf = (HPUCHR) GlobalLock(hAcquireBuf);
	if ( !hpAcquireBuf)
	  {                                  
	      /* unable to lock memory */

	      char msg[OLC_MAX_STATUS_MESSAGE_SIZE + 60];
	      wsprintf(msg, "Unable to lock memory for acquire.");
	      MessageBox(NULL, msg, "Frame Size Error", MB_ICONSTOP | MB_OK);
	      return; 
	  }                                       
	  

	Status = OlFgAllocateBuiltInFrame ( DevInfo.DevId,
									 OLC_FG_DEV_MEM_VOLATILE,
									 OLC_FG_NEXT_FRAME,
									 &FrameId);
	if ( !OlImgIsOkay(Status))
	{
	    PrintStatus(NULL, Status, "Unable to Allocate Frame.", "Acquire Error");
	}
      
	/* Have the memory, attempt the acquire */
	Status = OlFgAcquireFrameToHost(DevInfo.DevId, FrameId, hpAcquireBuf, ulMinBufSize);

	if ( !OlImgIsOkay(Status))
	{
	    PrintStatus(NULL, Status, "Unable to acquire.", "Acquire Error");
	    (void) OlFgDestroyFrame( DevInfo.DevId, FrameId);
        GlobalUnlock(hAcquireBuf);
		GlobalFree(hAcquireBuf);	
        return;
       
    }
	
    /* create a DIB and display it in the client area */
	
	if(RGBDataFlag)
	{
		DrawRGBFrame(hWnd,hpAcquireBuf, ulWidth,ulHeight);
		
	}
	else
	{
		CreateDIBDisplay(hpAcquireBuf, ulHeight, ulWidth, ulPixelDepth); 
		GlobalUnlock(hpAcquireBuf);
		GlobalFree(hAcquireBuf);	
	}
	/* we no longer need the frame, free it */

    Status = OlFgDestroyFrame( DevInfo.DevId, FrameId);

	if ( !OlImgIsOkay(Status))
	{
	    PrintStatus(NULL, Status, "Unable to free frame after acquire.", "Acquire Error");
	}
      

}

/*****************************************************************************************************/
//LoadCameraSetup
//    
//      Purpose...
//            Use a common dlg box to load data from camera files into the 3157.
//
//      Inputs:  stdialog to open camera file; parameters from file are used to
//                program the board;
//
//      Outputs: true = success; false = failure
//
/*****************************************************************************************************/

OLT_APISTATUS WriteCameraSetupToBoard ( HWND hWndDlg )

{
   static OPENFILENAME ofn ;
   char szFilter[128] = "Camera Files (*.CAM)\0\0";
   char lpstrFileName[_MAX_PATH] = "*.cam";
   char lpstrTitleName[_MAX_FNAME + _MAX_EXT ];

   char    lpstrReturnedString[64]; // points to destination buffer 
   DWORD    nSize = 64;  // size of destination buffer 
   
   OLT_APISTATUS Status;
   POINT SourceData;
   SIZE  ScaleDataReq, ScaleDataAct;
   float fTemp;
   ULNG  ulTemp, ulTemp2, ulOldData;
   UINT  iTemp, iTemp2;; 
   
   // common dialog for "open" used to select camera file:

     ofn.lStructSize       = sizeof (OPENFILENAME) ;
     ofn.hwndOwner         = hWndDlg ;
     ofn.hInstance         = NULL ;
     ofn.lpstrFilter       = szFilter ;
     ofn.lpstrCustomFilter = NULL ;
     ofn.nMaxCustFilter    = 0 ;
     ofn.nFilterIndex      = 0 ;
     ofn.lpstrFile         = NULL ;          // Set in Open and Close functions
     ofn.nMaxFile          = _MAX_PATH ;
     ofn.lpstrFileTitle    = NULL ;          // Set in Open and Close functions
     ofn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT ;
     ofn.lpstrInitialDir   = NULL ;
     ofn.lpstrTitle        = "Select Camera File to Load" ;
     ofn.Flags             = 0 ;             // Set in Open and Close functions
     ofn.nFileOffset       = 0 ;
     ofn.nFileExtension    = 0 ;
     ofn.lpstrDefExt       = "CAM" ;
     ofn.lCustData         = 0L ;
     ofn.lpfnHook          = NULL ;
     ofn.lpTemplateName    = NULL ;

     ofn.lpstrFile         = lpstrFileName ;

     ofn.lpstrFileTitle    = lpstrTitleName ;
     ofn.Flags             = OFN_CREATEPROMPT ;
   
#ifdef WIN32
     Status = GetOpenFileName (&ofn);
	 if (Status != TRUE)
		 return Status = OLC_STS_CANCELJOB;
#endif

     Status = OLC_STS_NORMAL;       // initialize status to normal
	 MyPrintf("Name of cam file: %s\n",lpstrFileName);

  // set up camera type
        ulTemp = GetPrivateProfileInt( "camvals", "Camera type", -1, ofn.lpstrFile);
        Status = Dt3157SetDigitalCameraType ( CurDevInfo.DevId, VSRC0, ulTemp );

    if (Status == OLC_STS_NORMAL)
    {
       // see if DIO setup is required
        iTemp = GetPrivateProfileInt( "camvals", "DIO setup", -1, ofn.lpstrFile);
        if (iTemp)      //  DIO setup is required
        {
            GetPrivateProfileString( "camvals", "DIO config", "?", lpstrReturnedString,
                                        nSize, ofn.lpstrFile);
      //    AsciiHexToDec (lpstrReturnedString, &ulTemp);  Make VB Cam files work the same
			
			ulTemp = (ULNG)atol(lpstrReturnedString);

            Status = Dt3157SetDigitalIOConfiguration ( CurDevInfo.DevId, ulTemp );
            if (Status == OLC_STS_NORMAL)
            {
                GetPrivateProfileString( "camvals", "DIO write", "?", lpstrReturnedString,
                                            nSize, ofn.lpstrFile);
          //    AsciiHexToDec (lpstrReturnedString, &ulTemp);  Make VB Cam files work the same
				
				ulTemp = (ULNG)atol(lpstrReturnedString);

                Status = Dt3157SetDigitalIO ( CurDevInfo.DevId, ulTemp );
            }
        }
    }
    

    if (Status == OLC_STS_NORMAL)
    {
      // disable board's sync mastering capabilities
        Status = Dt3157EnableSyncMasterMode (CurDevInfo.DevId, FALSE, VSRC0);
    }


    if (Status == OLC_STS_NORMAL)
    {
       // set up total pixels per line
        iTemp = GetPrivateProfileInt( "camvals", "Total pixels per line", -1, ofn.lpstrFile);
        Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_TOTAL_PIX_PER_LINE,
                        (ULNG)iTemp, &ulOldData);
    }

    if (Status == OLC_STS_NORMAL)
    {
       // set up 1st active pixel
        iTemp = GetPrivateProfileInt( "camvals", "Pixel start", -1, ofn.lpstrFile);
        Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_FIRST_ACTIVE_PIXEL,
                        (ULNG)iTemp, &ulOldData);
    }

    if (Status == OLC_STS_NORMAL)
    {
       // set up active pixel count per line	   
        iTemp = GetPrivateProfileInt( "camvals", "Pixel count", -1, ofn.lpstrFile);
		if (iTemp == -1)
			iTemp = 640;	// If camera file unsuccessful, load default size
       Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_ACTIVE_PIXEL_COUNT,
                        (ULNG)iTemp, &ulOldData);
       Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_FRAME_WIDTH,
                        (ULNG)iTemp, &ulOldData);
    }


    if (Status == OLC_STS_NORMAL)
    {
       // set up sync sentinel (usually OFF)
        iTemp = GetPrivateProfileInt( "camvals", "Sync sentinel", -1, ofn.lpstrFile);
        Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_SYNC_SENTINEL,
                        (ULNG)iTemp, &ulOldData);
    }

	if(iTemp)
	{
		if (Status == OLC_STS_NORMAL)
		{
		   // set up horiz sync ignore     
			GetPrivateProfileString( "camvals", "Sync ignore length", "?", lpstrReturnedString,
											nSize, ofn.lpstrFile);
			fTemp = (float)atof( lpstrReturnedString );
			fTemp *= (float)1.E2;
			ulTemp = (ULNG)fTemp;
			Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_HSYNC_SEARCH_POS,
							ulTemp, &ulOldData);
		}


		if (Status == OLC_STS_NORMAL)
		{
		   // set up horiz sync insert     
			GetPrivateProfileString( "camvals", "Sync insert position", "?", lpstrReturnedString,
											nSize, ofn.lpstrFile);
			fTemp = (float)atof( lpstrReturnedString );
			fTemp *= (float)1.E2;
			ulTemp = (ULNG)fTemp;
			Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_HSYNC_INSERT_POS,
							ulTemp, &ulOldData);
		}

		if (Status == OLC_STS_NORMAL)
		{
		   // set up vert sync ignore
			GetPrivateProfileString( "camvals", "Sync ignore row", "?", lpstrReturnedString,
											nSize, ofn.lpstrFile);
			fTemp = (float)atof( lpstrReturnedString );
			fTemp *= (float)1.E2;
			ulTemp = (ULNG)fTemp;
			Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_VSYNC_SEARCH_POS,
							ulTemp, &ulOldData);
		}
     
		if (Status == OLC_STS_NORMAL)
		{
		   // set up vert sync insert
			GetPrivateProfileString( "camvals", "Sync insert row", "?", lpstrReturnedString,
											nSize, ofn.lpstrFile);
			fTemp = (float)atof( lpstrReturnedString );
			fTemp *= (float)1.E2;
			ulTemp = (ULNG)fTemp;
			Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_VSYNC_INSERT_POS,
							ulTemp, &ulOldData);
		}

	}
    if (Status == OLC_STS_NORMAL)
    {
       // set up total lines
        iTemp = GetPrivateProfileInt( "camvals", "Total lines", -1, ofn.lpstrFile);
        Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_TOTAL_LINES_PER_FLD,
                        (ULNG)iTemp, &ulOldData);
    }

    if (Status == OLC_STS_NORMAL)
    {
       // set up 1st active line
        iTemp = GetPrivateProfileInt( "camvals", "Line start", -1, ofn.lpstrFile);
        Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_FIRST_ACTIVE_LINE,
                        (ULNG)iTemp, &ulOldData);
    }
    
    if (Status == OLC_STS_NORMAL)
    {
       // set up nbr of active lines
        iTemp = GetPrivateProfileInt( "camvals", "Row length", -1, ofn.lpstrFile);
		if (iTemp == -1)
			iTemp = 480;	// If camera file unsuccessful, load default size
       Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_ACTIVE_LINE_COUNT,
                        (ULNG)iTemp, &ulOldData);
       Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_FRAME_HEIGHT,
                        (ULNG)iTemp, &ulOldData);
    }



    if (Status == OLC_STS_NORMAL)
    {
       // set up default passthrough source origin
        SourceData.x = GetPrivateProfileInt( "camvals", "Passthru Src Origin X", -1, ofn.lpstrFile);
        SourceData.y = GetPrivateProfileInt( "camvals", "Passthru Src Origin Y", -1, ofn.lpstrFile);
        Status = OlFgSetPassthruSourceOrigin ( CurDevInfo.DevId, &SourceData );
    }

    if (Status == OLC_STS_NORMAL)
    {
		// set up passthrough rectangle dimensions based upon camera's active pixel and line counts
        ScaleDataReq.cx = GetPrivateProfileInt( "camvals", "Passthru Scaling X", -1, ofn.lpstrFile);
		if (ScaleDataReq.cx == -1)
			ScaleDataReq.cx = 640;  // if camera file unsuccessful,load a default size
        ScaleDataReq.cy = GetPrivateProfileInt( "camvals", "Passthru Scaling Y", -1, ofn.lpstrFile);
		if (ScaleDataReq.cy == -1)
			ScaleDataReq.cy = 480;  // if camera file unsuccessful,load a default size
        Status = OlFgSetPassthruScaling ( CurDevInfo.DevId, &ScaleDataReq, &ScaleDataAct );
    }

    if (Status == OLC_STS_NORMAL)
    {
       // enable/disable exposure mode
        iTemp = GetPrivateProfileInt( "camvals", "Exposure enable", -1, ofn.lpstrFile);
        Status = Dt3157EnableExposureMode (CurDevInfo.DevId, VSRC0, (BOOL)iTemp);

        if (Status == OLC_STS_NORMAL)
        {
            if (iTemp == TRUE)
            {
               // get exposure time from cam file
                GetPrivateProfileString( "camvals", "Exposure time", "?", lpstrReturnedString,
                                                nSize, ofn.lpstrFile);
                fTemp = (float)atof( lpstrReturnedString );
                ulTemp = (ULNG)fTemp;
               // get exposure polarity from cam file
                iTemp2 = GetPrivateProfileInt( "camvals", "Exposure polarity", -1, ofn.lpstrFile);
               // write time and polarity to board 
                Status = Dt3157SetExposure (CurDevInfo.DevId, VSRC0, ulTemp, (ULNG)iTemp2, &ulTemp2);
            }
        }
    }

    if (Status == OLC_STS_NORMAL)
    {
      // set up internal/external pixel clock  
        iTemp = GetPrivateProfileInt( "camvals", "Ext Pixclk", -1, ofn.lpstrFile);
        if (!iTemp)
            ulTemp = OLC_FG_CLOCK_INTERNAL;
        else
            ulTemp = OLC_FG_CLOCK_EXTERNAL;
       
        Status = OlFgSetInputControlValue (CurDevInfo.DevId, VSRC0, OLC_FG_CTL_CLOCK_SOURCE,
                    ulTemp, &ulOldData);
    }

    if (Status == OLC_STS_NORMAL)
    {
      // set up internal/external pixel clock  
        iTemp = GetPrivateProfileInt( "camvals", "Ext Pixclk Edge", -1, ofn.lpstrFile);
        if (!iTemp)
            ulTemp = 0L;
        else
            ulTemp = OLC_FG_CLOCK_EXT_ON_LO_TO_HI;
       
        Status = OlFgSetInputControlValue (CurDevInfo.DevId, VSRC0, OLC_FG_CTL_CLOCK_FLAGS,
                    ulTemp, &ulOldData);
    }

    if (Status == OLC_STS_NORMAL )
    {
      // set up pixel clock frequency   
        GetPrivateProfileString( "camvals", "Pixel clock", "?", lpstrReturnedString,
                                        nSize, ofn.lpstrFile);
        ulTemp = atol( lpstrReturnedString );
        Status = OlFgSetInputControlValue (CurDevInfo.DevId, VSRC0, OLC_FG_CTL_CLOCK_FREQ,
                        ulTemp, &ulOldData);
    }

	return Status;
 }


 
 /*****************************************************************************************************
  AsciiHexToDec : converts ascii representation of hex string to ULNG.
/*****************************************************************************************************/

BOOL AsciiHexToDec (char * szBuff, ULNG * ulDecVal)
{

char	szStr[16];
int		iCt, iCCt, iTest;
double  dblSum = 0;

    iCt = strlen (szBuff);

  // check for valid hex character...
	for (iCCt=0; iCCt < iCt; iCCt++)
	{
        szStr[0] = szBuff[iCCt];
        szStr[1] = '\0';
            
  //  if character is zero or non-int...
        if (atoi (szStr) == 0)	
        {

  // test for actual zero...
            if (szStr[0] == '0')
                iTest = 0;
            else
            {
                   
  // test for invalid character...
                iTest = (int)(szStr[0] & 0xBF);
                if (iTest > 6)

  // return failure if invalid character is found
                    return FALSE;
                else

  // convert A thru F to 10 thru 15
                    iTest = iTest+9;
            }
        }
        else
        {

  // character is 1 thru 9...
            iTest = atoi(szStr);
        }

        dblSum = dblSum + (double)iTest * pow ( 16., (double)(iCt - iCCt -1) );
    }
    * ulDecVal = (long)dblSum;
    return TRUE;
}


void MyPrintf(char* fmt, ...)
{
    char        Buffer[255];
                    
    va_list marker;
        va_start(marker, fmt);
        vsprintf (Buffer, fmt, marker);
    va_end(marker);

	OutputDebugString(Buffer);
}	


WORD DrawRGBFrame (HWND hWnd, VOID* FrameAddr, ULNG Width, ULNG Height)
{

	HDC			hDC;            // used for bitblt call					 
	HDC			hDCMemory;      // used for settting up the bitmap before bitblting
	HBITMAP		hBmpMem;        // bitmap of image data
	
	WORD		BytesPerPixel = 0;
	DWORD		FrameSize = 0;
	HWND		TargetWindow = hWnd;
	WORD		PixelDepth = 4;		// RGB 
	BITMAPINFO* lpBmi;
	HGLOBAL    	hBmpInfo;

	FrameSize		= Width * Height * PixelDepth;

	hDC = GetDC(TargetWindow);

	if((hBmpInfo = GlobalAlloc(GPTR,sizeof(BITMAPINFOHEADER) + (256*sizeof(RGBQUAD))))==NULL)
	{
		return 0;
	}
	if((lpBmi = (BITMAPINFO FAR *)GlobalLock(hBmpInfo))==NULL)
	{
		return 0;				
	}
	
	    
	// initialize the DIB header        
	lpBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpBmi->bmiHeader.biWidth = Width;			
	lpBmi->bmiHeader.biHeight = Height*-1;			
	lpBmi->bmiHeader.biPlanes = 1;
	lpBmi->bmiHeader.biBitCount = 32;
	lpBmi->bmiHeader.biCompression = BI_RGB;
	lpBmi->bmiHeader.biSizeImage = 0;
	lpBmi->bmiHeader.biXPelsPerMeter = 0;    
	lpBmi->bmiHeader.biYPelsPerMeter = 0;
	lpBmi->bmiHeader.biClrUsed = 0;
	lpBmi->bmiHeader.biClrImportant = 0;


	hBmpMem = CreateDIBitmap(hDC,(LPBITMAPINFOHEADER)lpBmi,CBM_INIT,FrameAddr,(LPBITMAPINFO)lpBmi,DIB_RGB_COLORS);


    hDCMemory = CreateCompatibleDC(hDC);

	if (!(hDC && hBmpMem && hDCMemory))
	{
		DeleteDC(hDCMemory);
		DeleteObject(hBmpMem);
		ReleaseDC(TargetWindow,hDC);

	    return FALSE;
	}

    SelectObject(hDCMemory, hBmpMem);
    StretchBlt( hDC,0, 0, Width, Height, 
				hDCMemory,0, 0, Width, Height,SRCCOPY);


    DeleteDC(hDCMemory);			 
    DeleteObject(hBmpMem);
    ReleaseDC(TargetWindow,hDC);
	GlobalUnlock(hBmpInfo);
	GlobalFree(hBmpInfo);

    return TRUE;
}
