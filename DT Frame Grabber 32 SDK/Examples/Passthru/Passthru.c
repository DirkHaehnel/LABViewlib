/*  File:   Passthru.c

    Description:
	This file contains the main portion of the frame grabber SDK
	passthru example application.

	Passthru is a simple Windows application that alows you to
	display live vidio from a DT-Open Layers monochrome frame
	grabber.  Passthru can also acquires and displays a single frame
	of vidio data.  This program demonstrates the use of the frame grabber
	SDK passthru and acquire to device functions.  It also shows how to build
	a DIB to display an acquired image.
	
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
		
	    OnSelectDevice() -
		called when user selects a device.  This function calls
		NewDevice() to open the device, and updates the menu
		to reflect the device's capablities.
	    
	    SetPal128Grey() -
		selects and realizes a palette with 128 grey scale levels.
		
	    RestorePalette() -
		restores the origonal palette. (this function is used after
		SetPal128Grey() )
		
	    DrawDisplayBitmap() -
		Draws the current image in the device frame to the client
		area of the window.
	    
	    SetupScrollBars() -
		Sets the scroll bar state to reflect current conditions.
	    
	    UpdateScrollBar() -
		Updates the specified scroll bar.  This function is called in response
		to WM_VSCROLL or WM_HSCROLL messages.
	    
    Comments:
	Windows can have several copies of your application running
	at the same time.  The variable hInst keeps track of which
	instance this application is so that processing will be to
	the correct window.

	Windows has macros defined for accessing child window data. 
	These are used throughout the code and can be found in Windowsx.h.

    Notes:
	This example program is based on the GENERIC sample included
	with the Microsoft Windows SDK.  All Microsoft disclaimers
	still apply.

	The source code in this file exceeds 80 columns so you may
	wish to print this file in "landscape" mode if your printer
	allows it.

*/
/*=========================================================================++
||                                                                         ||
||  Copyright (C) 1996.   Data Translation, Inc., 100 Locke Drive,         ||
||  Marlborough Massachusetts 01752-1192.                                  ||
||                                                                         ||
||  The information in this document is subject to change without          ||
||  notice and should not be construed as a commitment by Data             ||
||  Translation, Inc.  Data Translation, Inc. does not assume any          ||
||  responsibility for any errors that may appear in this document.        ||
||                                                                         ||
||  This document, or source code, is provided solely as an example        ||
||  of how to utilize Data Translation software libraries in a sample      ||
||  application.  Data Translation, Inc. does not assume any               ||
||  responsibility for the use or reliability of any portion of this       ||
||  document or the described software.                                    ||
||                                                                         ||
||  General permission to copy or modify, but not for profit, is           ||
||  hereby granted,  provided that the above copyright notice is           ||
||  included and reference made to the fact that reproduction              ||
||  privileges were granted by Data Translation, Inc.                      ||
||                                                                         ||
||  Data Translation cannot assume any responsibility for the use,         ||
||  or misuse, of any portion of this software for other than its          ||
||  intended diagnostic purpose in calibrating and testing Data            ||
||  Translation manufactured image processing boards and software.         ||
||                                                                         ||
++=========================================================================*/

/* Let's use strict type checking in the example */
#define STRICT

#include "passthru.h"    /* specific to this program */
#include <commdlg.h>
#include <math.h>

#define RUN_MENU	2		  /* position of run menu*/
#define VSRC0		0		  // Video source 0

/*global variables*/
DEVCAPS CurDevCaps = {0};     /* capabilities of current device */
DEVINFO CurDevInfo = {0};     /* current device information */

OLT_FG_FRAME_ID	FrameId = NULL;	/* Frame for snap shot*/


/* These are the pointer to the function related to the Dt3157 */
FARPROC Dt3157EnableExposureMode = NULL;
FARPROC Dt3157SetDigitalCameraType = NULL;
FARPROC Dt3157SetDigitalIOConfiguration = NULL;
FARPROC Dt3157SetDigitalIO = NULL;
FARPROC Dt3157SetSyncMasterControlValue = NULL;
FARPROC Dt3157SetExposure = NULL;
FARPROC Dt3157EnableSyncMasterMode = NULL;
WORD DrawRGBFrame (HWND hWnd, VOID* FrameAddr, WORD Width, WORD Height);

/* Static variable used in this module */

static LPCSTR lpcszMainMenu = "PassThruMenu";                    /* Name of menu resource in .RC file. */
static LPCSTR lpcszMainWinClass = "PassThruWClass";              /* Name used in call to CreateWindow. */
static LPCSTR lpcszMainWinTitle = "PassThru Sample Application"; /* Text for window title bar. */
 HINSTANCE hInst;

static HPALETTE hPalGrey = NULL;     /* windows handle to logical palette with 128 grey entries*/
static HPALETTE hPalOld = NULL;      /* save old palette, so we can restore the hdc*/
static BITMAPINFO FAR *lpBmi = NULL; /* storage for bitmap info which is used to create DIBs*/

									// This information would be used to determine the draw startegy
static char tstr[OLC_MAX_STATUS_MESSAGE_SIZE];    /*temporary string, used for error messages etc.*/

HANDLE	hSurface1 = NULL;


// Prototypes internal to this module
OLT_APISTATUS WriteCameraSetupToBoard ( HWND hWndDlg );
BOOL AsciiHexToDec (char * szBuff, ULNG * ulDecVal);
void MyPrintf(char* fmt, ...);		// Send to debug console with Printf format;

/**************************************************************************/
/**************************************************************************/

/*
    Function:   WinMain

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
	HINSTANCE hInstance     Current instance
	HINSTANCE hPrevInstance     Previous instance
	LPSTR lpCmdLine         Points to a NULL-teminated string
			   specifying the command line for
			   the application.
	int nCmdShow            Specifies how the window is to be shown

    Output Arguments:
	None

    Returns: int
	The return value of PostQuitMessage or 0 if the function
	terminates prior to entering the message loop.
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;                    /* message */
    HWND hExistingWnd = NULL;   /* handle to existing Window, if any */

    /* If no other instances are running, init the app.  This check should be    */
    /*    redundant due to the above check, but it doesn't really hurt anything. */

    if (!hPrevInstance)
       if (!InitApplication(hInstance))
	  return 0;
    

    /* Perform initializations that apply to a specific instance */

    if (!InitInstance(hInstance, nCmdShow))
       return 0;

    /* Acquire and dispatch messages until a WM_QUIT message is received. */

    while ( GetMessage(&msg, NULL, 0, 0) ) {
	/* Translates virtual key codes */
	
	if(msg.hwnd && IsDialogMessage(msg.hwnd, &msg))  continue; 

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
    Function:   InitApplication

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
	HINSTANCE hInstance     Current (first) instance

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

    wc.style = 0;	                                 /* Class style(s). */
    wc.lpfnWndProc = MainWndProc;                    /* Function to retrieve messages for */
						     /*    windows of this class.         */
    wc.cbClsExtra = 0;                               /* No per-class extra data. */
    wc.cbWndExtra = 0;                               /* No per-window extra data. */
    wc.hInstance = hInstance;                        /* Application that owns the class. */
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);      /* Default app icon */
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);        /* Default arrow cursor */
    wc.hbrBackground =  CreateSolidBrush(RGB(0,0,0));  /* Use system defined background */
    wc.lpszMenuName = lpcszMainMenu;                 /* Name of menu resource in .RC file. */
    wc.lpszClassName = lpcszMainWinClass;            /* Name used in call to CreateWindow. */

			 
    /* Register the window class and return success/failure code. */

    return ( (BOOL) RegisterClass(&wc) );
}


/**************************************************************************/
/**************************************************************************/

/*
    Function:   InitInstance

    Description:
	This function is called at initialization time for every
	instance of this application.  This function performs
	initialization tasks that cannot be shared by multiple
	instances.

	In this case, we save the instance handle in a static
	variable and create and display the main program window.


    Input Arguments:
	HINSTANCE hInstance     Current (first) instance
	int nCmdShow            Param for first ShowWindow() call.

    Output Arguments:
	None

    Returns:  BOOL
	TRUE if we successfully created the window; FALSE otherwise.
*/

BOOL PRIVATE InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND  hWnd;          /* Main window handle. */
	RECT  Desktop = {0};
	int	  X0,X1,Y0,Y1;


    /* Save the instance handle in static variable, which will be used in */
    /* many subsequence calls from this application to Windows.           */

	 hInst = hInstance;

	 GetClientRect(GetDesktopWindow(),&Desktop);

	 X0 = (Desktop.right -  WINDOW_WIDTH)/2;
	 Y0 = (Desktop.bottom - WINDOW_HEIGHT)/2;
	 X1 = WINDOW_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2;
	 Y1 = WINDOW_HEIGHT+ GetSystemMetrics(SM_CYCAPTION) 
		  + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYDLGFRAME)*2;

     /* Create a main window for this application instance. */

    hWnd = CreateWindow(
		lpcszMainWinClass,              /* See RegisterClass() call. */
		lpcszMainWinTitle,              /* Text for window title bar. */
		WS_CAPTION|WS_SYSMENU
		|WS_MINIMIZEBOX,            /* Window style. */
		X0,						   /* Default horizontal position. */
		Y0,						   /* Default vertical position. */
		X1,                  /* Default width. */
		Y1,                  /* Default height. */
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
    Function:   MainWndProc

    Description:
	This function is the primary message handler for the
	application.

    Input Arguments:
	HWND hWnd       window handle
	UINT message        message
	WPARAM wParam       16-bit message-specific data
	LPARAM lParam       32-bit message-specific data

    Output Arguments:
	None

    Returns:  LRESULT
	Message dependent return value is result of message
	processing.
*/


LRESULT CALLBACK __export MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // keep track of Overlay Control Window
	static	HWND OverlayhWnd = NULL;

	// keep track of SnapShot Window
	static	HWND hWndSnapShot = NULL;
	
	/* What message are we handling? */

    switch (message) {

    /* message: command from application menu */
    case WM_COMMAND:
	switch (LOWORD(wParam))  {

	/*close the application*/
	case IDM_FILE_EXIT:
	    DestroyWindow(hWnd);
	    return 0;
	    
	/* Display the about box */
	case IDM_HELP_ABOUT:
	    DialogBox(hInst,    /* current instance */
			"AboutBox",     /* resource to use */
			hWnd,           /* parent handle */
			About);         /* About() instance address */
	    break;

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
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SOURCE, MF_GRAYED | MF_BYCOMMAND);
			 
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SYNCHRONOUS, MF_GRAYED | MF_BYCOMMAND);
	    CheckMenuItem(GetMenu(hWnd), IDM_SETUP_SYNCHRONOUS, MF_UNCHECKED | MF_BYCOMMAND);
			 
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_ASYNCHRONOUS, MF_GRAYED | MF_BYCOMMAND);
	    CheckMenuItem(GetMenu(hWnd), IDM_SETUP_ASYNCHRONOUS, MF_UNCHECKED | MF_BYCOMMAND);
	    
	    EnableMenuItem(GetMenu(hWnd), RUN_MENU, MF_GRAYED | MF_BYPOSITION);

		EnableMenuItem(GetMenu(hWnd), IDM_OVERLAY_CONTROL, MF_GRAYED | MF_BYCOMMAND);
		EnableMenuItem(GetMenu(hWnd), IDM_SETUP_EXTERNALCSYNCLINE, MF_GRAYED | MF_BYCOMMAND);

		DrawMenuBar(hWnd);
	    
	    ShowScrollBar(hWnd, SB_HORZ, FALSE);
	    ShowScrollBar(hWnd, SB_VERT, FALSE);
	    
	    break;
		    
	/* Display the input source selection dialog box */
	case IDM_SETUP_SOURCE:
	    DialogBox(hInst, MAKEINTRESOURCE(IDD_INPUT_SOURCE), hWnd, InputSourceProc);
	    break;

	/* setup for synchronous passthru*/
	case IDM_SETUP_SYNCHRONOUS:
	    CheckMenuItem(GetMenu(hWnd), IDM_SETUP_SYNCHRONOUS, MF_CHECKED | MF_BYCOMMAND);
	    CheckMenuItem(GetMenu(hWnd), IDM_SETUP_ASYNCHRONOUS, MF_UNCHECKED | MF_BYCOMMAND);
	    CurDevInfo.AsyncPassthru = FALSE;
	    break;

	/* setup for asynchronous passthru*/
	case IDM_SETUP_ASYNCHRONOUS:
	    CheckMenuItem(GetMenu(hWnd), IDM_SETUP_ASYNCHRONOUS, MF_CHECKED | MF_BYCOMMAND);
	    CheckMenuItem(GetMenu(hWnd), IDM_SETUP_SYNCHRONOUS, MF_UNCHECKED | MF_BYCOMMAND);
	    CurDevInfo.AsyncPassthru = TRUE;
	    break;

	case IDM_SETUP_EXTERNALCSYNCLINE:
	{
		ULNG	ulOldData;
		DWORD	Error;

		if (CurDevInfo.ulCSyncSource == CSYNC_CURRENT)
		{
			CheckMenuItem(GetMenu(hWnd), IDM_SETUP_EXTERNALCSYNCLINE, MF_CHECKED | MF_BYCOMMAND);
			Error = OlFgSetInputControlValue(CurDevInfo.DevId,CurDevInfo.InputSource, OLC_FG_CTL_CSYNC_SOURCE,OLC_FG_CSYNC_EXTERNAL_LINE,&ulOldData);
			CurDevInfo.ulCSyncSource = CSYNC_EXTERNAL;
		}
		else
		{
			CheckMenuItem(GetMenu(hWnd), IDM_SETUP_EXTERNALCSYNCLINE, MF_UNCHECKED | MF_BYCOMMAND);
			Error = OlFgSetInputControlValue(CurDevInfo.DevId,CurDevInfo.InputSource, OLC_FG_CTL_CSYNC_SOURCE,OLC_FG_CSYNC_SPECIFIC_SRC,&ulOldData);
			CurDevInfo.ulCSyncSource = CSYNC_CURRENT;
		}
	}

	break;
			

	/* Start passthru*/
	case IDM_RUN_START_PASSTHRU:
	    OnStartPassthru(hWnd);
		
		// if passthru is running br
		if(CurDevInfo.PassthruActive == TRUE && CurDevCaps.DoesPassthruSnapshot == TRUE)
		{
			if (!IsWindow(hWndSnapShot))
			{			
				hWndSnapShot = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SNAPSHOT), hWnd, SnapShotProc);
				ShowWindow(hWndSnapShot, SW_SHOW);
			}
			else
			{
				ShowWindow(hWndSnapShot, SW_RESTORE);            	                   			
				SetFocus(hWndSnapShot);        	                   			
			}

		EnableMenuItem(GetMenu(hWnd), IDM_RUN_SINGLE_FRAME,			 MF_GRAYED | MF_BYCOMMAND);
		EnableMenuItem(GetMenu(hWnd), IDM_SETUP_EXTERNALCSYNCLINE,	 MF_GRAYED | MF_BYCOMMAND);
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_ASYNCHRONOUS,		 MF_GRAYED | MF_BYCOMMAND);
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SYNCHRONOUS,		 MF_GRAYED | MF_BYCOMMAND);
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SOURCE,				 MF_GRAYED | MF_BYCOMMAND);
		EnableMenuItem(GetMenu(hWnd), IDM_SETUP_EXTERNALCSYNCLINE,	 MF_GRAYED | MF_BYCOMMAND);

		}


		
	    break;

	/* Stop passthru*/
	case IDM_RUN_STOP_PASSTHRU: {
	    OLT_APISTATUS JobStatus;

	    /*stop asyncronous passthru*/
	    OlFgStopAsyncPassthru(CurDevInfo.DevId, CurDevInfo.PassJobId, &JobStatus);

		// Close SnapShot Window
		if(hWndSnapShot != NULL)
		{
			DestroyWindow(hWndSnapShot);
		}

	    /*update menu and passthru active flag*/ 
	    CurDevInfo.PassthruActive = FALSE;
	    EnableMenuItem(GetMenu(hWnd), IDM_RUN_START_PASSTHRU, MF_ENABLED | MF_BYCOMMAND);
	    EnableMenuItem(GetMenu(hWnd), IDM_RUN_STOP_PASSTHRU,  MF_GRAYED  | MF_BYCOMMAND);
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_CLOSE_DEVICE, MF_ENABLED | MF_BYCOMMAND);

	    /*grab & display a single frame, we need this so that we can 
	    have data to paint the screen*/
	    AcquireSingleFrame(hWnd);

		EnableMenuItem(GetMenu(hWnd), IDM_SETUP_EXTERNALCSYNCLINE,	 MF_ENABLED | MF_BYCOMMAND);
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_ASYNCHRONOUS,		 MF_ENABLED | MF_BYCOMMAND);
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SYNCHRONOUS,		 MF_ENABLED | MF_BYCOMMAND);
	    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SOURCE,				 MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(GetMenu(hWnd), IDM_SETUP_EXTERNALCSYNCLINE,	 MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(GetMenu(hWnd), IDM_RUN_SINGLE_FRAME,			 MF_ENABLED | MF_BYCOMMAND);
	    break;
	}
		
	/* perform single frame acquire*/
	case IDM_RUN_SINGLE_FRAME:
	    AcquireSingleFrame(hWnd);
	    break;

		/* Display the Overlay Control modeless dialog box */
	case IDM_OVERLAY_CONTROL:
		if (!IsWindow(OverlayhWnd))
		{			
			ShowWindow(OverlayhWnd, SW_SHOW);
			OverlayhWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_OVERLAY_CONTROL),
															hWnd, OverlayControlProc);
			ShowWindow(OverlayhWnd, SW_SHOW);
		}
		else
	    {
	        ShowWindow(OverlayhWnd, SW_RESTORE);            	                   			
	        SetFocus(OverlayhWnd);        	                   			
	    }

	    break;
																								 
	
	/* Let Windows process it */
	default:
	    return DefWindowProc(hWnd, message, wParam, lParam);
	}  /*end switch (wParam)*/
	break;  /*end case WM_COMMAND */


	case WM_PALETTECHANGED:
	    /*if this app changed the palette, ignore the message*/
	    if ((HWND) wParam == hWnd)
		return 0;
	    /* Otherwise, fall through to WM_QUERYNEWPALETTE. */

	case WM_QUERYNEWPALETTE:
		//case WM_ACTIVATE:
	    if (CurDevInfo.PassthruActive || !CurDevInfo.FrameDataValid)
		return 0;    /*exit if passthru is active or if we don't yet have a frame of data
		
	    /* If realizing our logical palette causes the system
	       palette to change, redraw completely. */
	    if (wParam ==  WA_ACTIVE || wParam == WA_CLICKACTIVE) {
		int NumChanged;                HDC hdc = GetDC(hWnd);
			NumChanged = SetPal128Grey(hdc);
			RestorePalette(hdc);
			ReleaseDC(hWnd, hdc);
		
		/* If any palette entries changed, repaint the window. */
		if (NumChanged > 0)
		    InvalidateRect(hWnd, NULL, TRUE);
	     }  
	     return 0;

	case WM_DESTROY:
	    // destroy frame used for snapshot
		if(FrameId != NULL)
		{
			OlFgDestroyFrame(CurDevInfo.DevId, FrameId);
		}

			
		if (CurDevInfo.DevId != NULL)
			CloseCurrentDevice();
		
	    /* free global data */
	    if (hPalGrey != NULL) {
		DeleteObject(hPalGrey);   /*delete grey palette object*/
		hPalGrey = NULL;
	    }
	    if (lpBmi != NULL) {
		GlobalFreePtr(lpBmi);
		lpBmi = NULL;             /*delete the bitmap header data*/
	    }
	
	    PostQuitMessage(0);
	    break;

	case WM_PAINT:
		if (CurDevInfo.FrameDataValid &&!CurDevInfo.PassthruActive){
		/*only paint if we have valid data, and passthru is inactive*/
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		if(CurDevCaps.DoesDrawAcqFrame)
			OlFgDrawAcquiredFrame(CurDevInfo.DevId, hWnd, CurDevInfo.FrameId);

		else
			DrawDisplayBitmap(hWnd, ps.hdc);
		EndPaint(hWnd, &ps);
		return 0;
	    }
	    else
		return DefWindowProc(hWnd, message, wParam, lParam);

	case WM_SIZE:
	
	/* WM_SIZE and WM_MOVE get called from DCI during Async passthru */
	/* have to be carefull what we do                                */
		
	    if (CurDevInfo.DevId != NULL) {

		SetupScrollBars(hWnd, LOWORD(lParam), HIWORD(lParam));
    
		/* force the scrollbars to get updated              */
		/* - scrollbars appear to change the client size    */
		/* AFTER getting drawn. This gives the DCI passthru */
		/* time to write over them. So redraw the window    */ 
		/* frame next to client area, this will make sure   */
		/* the scrollbars are re-drawn AFTER client size    */
		
		RedrawWindow(hWnd,NULL,NULL,
			   RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW);                
	    }
	    break;

	case WM_HSCROLL:
	    /*update origin and scroll bar*/
	    CurDevInfo.OriginX = UpdateScrollBar(hWnd, SB_HORZ, wParam, LOWORD(lParam));

	    if (CurDevInfo.PassthruActive) {
		/*if passthru active: stop passthru */
		OLT_APISTATUS JobStatus;
		OlFgStopAsyncPassthru(CurDevInfo.DevId, CurDevInfo.PassJobId, &JobStatus);

		/*update origin & re-start passthru*/
		OnStartPassthru(hWnd); 
	    }
	    else {
		/*if passthru is not active, simply redraw the bitmap*/
		InvalidateRect(hWnd, NULL, FALSE);
	    }
	    break;
	
	case WM_VSCROLL:
	    /*update origin and scroll bar*/
	    CurDevInfo.OriginY = UpdateScrollBar(hWnd, SB_VERT, wParam, LOWORD(lParam));

	    if (CurDevInfo.PassthruActive) {
		/*if passthru active: stop passthru */
		OLT_APISTATUS JobStatus;
		OlFgStopAsyncPassthru(CurDevInfo.DevId, CurDevInfo.PassJobId, &JobStatus);

		/*update origin & re-start passthru*/
		OnStartPassthru(hWnd); 
	    }
	    else {
		/*if passthru is not active, simply redraw the bitmap*/
		InvalidateRect(hWnd, NULL, FALSE);
	    }
	    break;

	case WM_MOUSEMOVE:
		if(OverlayhWnd != NULL)
		{
			if(wParam == MK_LBUTTON)
			{
				DrawOnSurface(OverlayhWnd, lParam, TRUE);
			}
			else
			{
				DrawOnSurface(OverlayhWnd, lParam, FALSE);
			}
		}
		break;



	
	default:
	    /* Pass it on if unproccessed */
	    return DefWindowProc(hWnd, message, wParam, lParam);
    }  /*end switch (message)*/


    /* By default, return zero for any message we handled that didn't require */
    /*   a specific return value.                                             */

    return 0;
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
    UINT MenuEnable;

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

    /* Okay, we have a device alias.  Now it's time to open this */
    /*   device.  Check to see if we already have a device open. */
    /*   If so, see if user really wants to close it and open a  */
    /*   new one.                                                */

    if (CurDevInfo.DevId != NULL )
	{
		char msg[128];
		wsprintf(msg, "Do you want to close the currently open device (\"%s\")?", CurDevInfo.Alias);

		if ( MessageBox(hWnd, msg, "Select Device", MB_ICONEXCLAMATION | MB_YESNO) == IDNO )
		{
			return;
		}
		else
		{
			CloseCurrentDevice();
		}
    }


    /* try to open it and read it's capabilities.  */

    if ( !NewDevice(Alias, hWnd) ) 
	{
		char msg[128];
		wsprintf(msg, "Unable to open and initialize device (\"%s\").", CurDevInfo.Alias);
		MessageBox(hWnd, msg, "Select Device", MB_ICONSTOP | MB_OK);
		return;
    } 

	/* Set menu items to match board caps */
	EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SELECT_DEVICE, MF_GRAYED | MF_BYCOMMAND);
	
	EnableMenuItem(GetMenu(hWnd), IDM_SETUP_CLOSE_DEVICE, MF_ENABLED | MF_BYCOMMAND);

	OlImgQueryDeviceCaps(CurDevInfo.DevId,OLC_IMG_DC_DEVICE_NAME,DeviceName,sizeof(STRING));
	if ((strcmp("DT3157",DeviceName)) == 0) 
	{
		EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SOURCE, MF_GRAYED | MF_BYCOMMAND);
	}
	else 
	{		
		EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SOURCE, MF_ENABLED | MF_BYCOMMAND);
	}    
     
	MenuEnable = CurDevCaps.DoesSyncPassthru ? MF_ENABLED : MF_GRAYED;
    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_SYNCHRONOUS, MenuEnable | MF_BYCOMMAND);
    MenuEnable = (CurDevCaps.DoesSyncPassthru && !CurDevInfo.AsyncPassthru)
		    ? MF_CHECKED : MF_UNCHECKED;
    CheckMenuItem(GetMenu(hWnd), IDM_SETUP_SYNCHRONOUS, MenuEnable | MF_BYCOMMAND);
	     
    MenuEnable = CurDevCaps.DoesAsyncPassthru ? MF_ENABLED : MF_GRAYED;
    EnableMenuItem(GetMenu(hWnd), IDM_SETUP_ASYNCHRONOUS, MenuEnable | MF_BYCOMMAND);
    MenuEnable = (CurDevCaps.DoesAsyncPassthru && CurDevInfo.AsyncPassthru)
		    ? MF_CHECKED : MF_UNCHECKED;
    CheckMenuItem(GetMenu(hWnd), IDM_SETUP_ASYNCHRONOUS, MenuEnable | MF_BYCOMMAND);

    MenuEnable = ( CurDevCaps.DoesAsyncPassthru || CurDevCaps.DoesSyncPassthru  ||
		    CurDevCaps.DoesDeviceAcquire )   ? MF_ENABLED : MF_GRAYED;
    EnableMenuItem(GetMenu(hWnd), RUN_MENU, MenuEnable | MF_BYPOSITION);
    MenuEnable = ( CurDevCaps.DoesAsyncPassthru || 
		    CurDevCaps.DoesSyncPassthru  )   ? MF_ENABLED : MF_GRAYED;
    EnableMenuItem(GetMenu(hWnd), IDM_RUN_START_PASSTHRU, MenuEnable | MF_BYCOMMAND);
    EnableMenuItem(GetMenu(hWnd), IDM_RUN_STOP_PASSTHRU, MF_GRAYED | MF_BYCOMMAND);
    MenuEnable = CurDevCaps.DoesDeviceAcquire ? MF_ENABLED : MF_GRAYED;
    EnableMenuItem(GetMenu(hWnd), IDM_RUN_SINGLE_FRAME, MenuEnable | MF_BYCOMMAND);

    /* Enable the overlay menu item if Overlays is supported */
	if(CurDevCaps.DoesOverlays == TRUE)
	{
		EnableMenuItem(GetMenu(hWnd), IDM_OVERLAY_CONTROL, MF_ENABLED | MF_BYCOMMAND);
	}
	else
	{
		EnableMenuItem(GetMenu(hWnd), IDM_OVERLAY_CONTROL, MF_GRAYED | MF_BYCOMMAND);
	}

	if(CurDevCaps.DoesCSyncSource & OLC_FG_CSYNC_EXTERNAL_LINE)
	{
		EnableMenuItem(GetMenu(hWnd), IDM_SETUP_EXTERNALCSYNCLINE, MF_ENABLED | MF_BYCOMMAND);
	}
	      
	DrawMenuBar(hWnd);
     
     {  /* setup initial scrollbars */
	RECT CRect;
	GetClientRect(hWnd,&CRect);
	SetupScrollBars(hWnd,CRect.right,CRect.bottom);
     }


	     
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
	switch (LOWORD(wParam))
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
	This function closes the current device and initializes the
	global device info structures to zeros.

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
    if (CurDevInfo.DevId != NULL)
       (void) OlImgCloseDevice(CurDevInfo.DevId);
    /*free bitmap data buffer*/   
    if (CurDevInfo.lpBmpData != NULL)
	GlobalFreePtr(CurDevInfo.lpBmpData);

    /* Clear the device info structures */
    _fmemset((LPVOID)&CurDevCaps, 0, sizeof(DEVCAPS));
    _fmemset((LPVOID)&CurDevInfo, 0, sizeof(DEVINFO));
	
	FrameId = NULL;
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
    ULNG sections,modes;
    BOOL Support;
    OLT_FG_SINGLE_FRAME_OPS ops;
    OLT_APISTATUS result;
	OLT_IMG_DEV_ID DevId = NULL;

	STRING			DeviceName = {0};
	OPENFILENAMEW	CamFileDlg = {0};
	OLT_APISTATUS	Status = 0;


    /* Try to open the device */
    if ( ! OlImgIsOkay( OlImgOpenDevice(lpcszAlias, &DevId) ) )
	return FALSE;
	
    /*set a 5 second timeout for driver calls */
    if (!OlImgIsOkay (OlImgSetTimeoutPeriod(DevId,5,&ulOldTimeout)) )
	return FALSE;

    /*init device info*/
    CurDevInfo.DevId = DevId;
    CurDevInfo.InputSource = 0;
    
 	/* if the device is a 3157, load the .cam file first, to set the frame size correctly,
		before allocating memory     */
	 /**************************************************************************
	  This section has been added to detect a DT3157 board to allow the user
	  to supply a *.cam file to configure the board for the current digital
	  camera used.
	 
	  We query the driver for the kind of board that has been opened. 
	  (No matter the previous ones). We query for the key "device name".
	  If the board is a DT3157, then we launch doing the following:
	  
		  - load the DT315732.DLL that contains the 3157 specific functions
		  - check if the loader succeeded
		  - ask for the address of the functions we need
		  - check if the DLL contained all the functions we needed
		  - launch the dialog box and then supply the file name 
			to the configuration routine.

	 **************************************************************************/

	 Status = OlImgQueryDeviceCaps(CurDevInfo.DevId,OLC_IMG_DC_DEVICE_NAME,DeviceName,sizeof(STRING));
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
			   Dt3157SetDigitalIO =	GetProcAddress(DllHandle,"Dt3157SetDigitalIO");
			   Dt3157SetSyncMasterControlValue = GetProcAddress(DllHandle,"Dt3157SetSyncMasterControlValue");
			   Dt3157SetExposure = GetProcAddress(DllHandle,"Dt3157SetExposure");
			   Dt3157EnableSyncMasterMode =	GetProcAddress(DllHandle,"Dt3157EnableSyncMasterMode");

			   if (!(	 Dt3157EnableExposureMode			&&
						 Dt3157SetDigitalCameraType			&&
						 Dt3157SetDigitalIOConfiguration	&&
						 Dt3157SetDigitalIO					&&
						 Dt3157SetSyncMasterControlValue	&&
						 Dt3157SetExposure					&&
						 Dt3157EnableSyncMasterMode) ) // One of those failed ?
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
	 }
	 //**************************************************************************
   
	/*fetch passthru modes*/
    sections = modes = 0L;  /*init to no passthru supported*/
    OlImgQueryDeviceCaps(DevId, OLC_IMG_DC_SECTIONS, &sections, sizeof(sections));
    if (sections & OLC_FG_SECTION_PASSTHRU)
	OlFgQueryPassthruCaps(DevId, OLC_FG_PC_PASSTHRU_MODE_LIMITS, &modes, sizeof(modes)); 

    /*load device caps*/
    CurDevCaps.DoesSyncPassthru =        modes & OLC_FG_PASSTHRU_SYNC_BITMAP
				      || modes & OLC_FG_PASSTHRU_SYNC_DIRECT;
    CurDevCaps.DoesSyncPassthruDirect =  (modes & OLC_FG_PASSTHRU_SYNC_DIRECT) != 0;
    CurDevCaps.DoesAsyncPassthru =       modes & OLC_FG_PASSTHRU_ASYNC_BITMAP
				      || modes & OLC_FG_PASSTHRU_ASYNC_DIRECT;
    CurDevCaps.DoesAsyncPassthruDirect = (modes & OLC_FG_PASSTHRU_ASYNC_DIRECT) != 0;
    
    Support = FALSE;  /*init to false*/
    OlFgQueryPassthruCaps(DevId, OLC_FG_PC_DOES_SOURCE_ORIGIN, &Support, sizeof(Support)); 
    CurDevCaps.DoesSoruceOrigin = Support;

    /*fetch acquire to device caps and info*/
    ops.ToDevSync = 0;  /*init to no support*/
    OlFgQueryInputCaps(DevId, OLC_FG_IC_SINGLE_FRAME_OPS , &ops, sizeof(ops));
    CurDevCaps.DoesDeviceAcquire = (ops.ToDevSync & OLC_FG_ACQ_FRAME) != 0;
    
    if (CurDevCaps.DoesDeviceAcquire) 
	{
		/*we can aquire to device, lets allocate a frame and fetch it's info*/
		result = OlFgAllocateBuiltInFrame(  DevId,        
						    OLC_FG_DEV_MEM_VOLATILE,
						    OLC_FG_NEXT_FRAME,
						    &CurDevInfo.FrameId);
		if (!OlImgIsOkay(result)) 
		{
		    /*if we can't get a volatile frame, try a non-volatile*/
		    result = OlFgAllocateBuiltInFrame(  DevId,        
							OLC_FG_DEV_MEM_NONVOLATILE,
							OLC_FG_NEXT_FRAME,
							&CurDevInfo.FrameId);
		}

		if (OlImgIsOkay(result)) 
		{
		    /*if we have a frame, let's fetch it's info */
		    result = OlFgMapFrame(DevId, CurDevInfo.FrameId, &CurDevInfo.FrameInfo);
		}
	
	
		if (!OlImgIsOkay(result)) 
		{
		    /* if there is any problem getting or mapping the frame,
		       turn off support flag*/
		    CurDevCaps.DoesDeviceAcquire = FALSE;
		}

		else 
		{
	    
			/*if all ok, allocate buffer to store inverted data needed for bitmap*/
			if (CurDevInfo.lpBmpData != NULL)
			{
			  GlobalFreePtr(CurDevInfo.lpBmpData);
			}


			CurDevInfo.lpBmpData = GlobalAllocPtr(GHND, 
				(long)CurDevInfo.FrameInfo.Width * (long)CurDevInfo.FrameInfo.BytesPerSample * 
					(long)CurDevInfo.FrameInfo.Height);
			
			if (CurDevInfo.lpBmpData == NULL)
			{
			  CurDevCaps.DoesDeviceAcquire = FALSE;
			} 
		}
    }
    
    if (!CurDevCaps.DoesDeviceAcquire &&
		(CurDevCaps.DoesSyncPassthru || CurDevCaps.DoesSyncPassthru) )
	{
		/* if we can do passthru, but can't get a device frame,
		   then we must fill in frame width and height info from passthru caps*/
		OLT_LNG_RANGE range;
		OlFgQueryPassthruCaps(DevId, OLC_FG_PC_SCALE_HEIGHT_LIMITS, &range, sizeof(range));
		CurDevInfo.FrameInfo.Height = (USHRT)range.Max;
		OlFgQueryPassthruCaps(DevId, OLC_FG_PC_SCALE_WIDTH_LIMITS, &range, sizeof(range));
		CurDevInfo.FrameInfo.Width = (USHRT)range.Max;
    }
    
    /*init to asynchronous passthru mode if supported*/
    CurDevInfo.AsyncPassthru = CurDevCaps.DoesAsyncPassthru;
    /*init to frame data not valid*/
    CurDevInfo.FrameDataValid = FALSE;


	// Find out if driver supports Overlays
	OlFgQueryDDICaps(DevId, OLC_FG_DDI_OVERLAYS, &CurDevCaps.DoesOverlays);

	// Find out if driver supports Passthru SnapShot
	OlFgQueryPassthruCaps(DevId, OLC_FG_PC_DOES_PASSTHRU_SNAPSHOT, &CurDevCaps.DoesPassthruSnapshot, sizeof(BOOL));

	// Find out if driver supports Drawing Acquired Frame
	OlFgQueryInputCaps(DevId, OLC_FG_IC_DOES_DRAW_ACQUIRED_FRAME, &CurDevCaps.DoesDrawAcqFrame, sizeof(BOOL));

	// Find out if we can use an external csync line
	OlFgQueryInputCaps(DevId,OLC_FG_IC_CSYNC_SOURCE_LIMITS,&CurDevCaps.DoesCSyncSource, sizeof(BOOL));


	 
    return TRUE;
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   SetPal128Grey

    Description:
	This function selects a palette of 128 grey levels into the
	specified device context.  This palette is then realized.
	Use RestorePalette() to restore the device context to
	the original palette

    Input Arguments:
	HDC hdc    handle to device context

    Output Arguments:
	None

    Return Value (UINT):
	The number of entries that changed in the system palette
	(see RealizePalette() in the Win SDK documentation)
	

*/

UINT SetPal128Grey(HDC hdc)
{
    LOGPALETTE FAR * lpPal;       /* pointer to logical palette*/
    int  iPalCount;                /* number of palette entries*/
    int iPalSize;                 /* size of palette to allocate*/
    int i;                        /* loop counter*/

    if (hPalGrey == NULL) { 
	
	/*if our 128 grey palette doesn't exist, create it*/      
	
	iPalCount =  128;//GetDeviceCaps(hdc,SIZEPALETTE);

	iPalSize = sizeof(LOGPALETTE) + (iPalCount-1) * sizeof(PALETTEENTRY);
		  
	/* allocate  LOGPALETTE*/
	lpPal = GlobalAllocPtr(GHND,iPalSize);
		
	lpPal->palVersion = 0x300;
	lpPal->palNumEntries = iPalCount;
	
	/* use 128 values for display*/
	for (i=0;i<iPalCount;i++){
	
	    lpPal->palPalEntry[i].peRed =   2*i;
	    lpPal->palPalEntry[i].peGreen = 2*i;
	    lpPal->palPalEntry[i].peBlue = 2*i;
	    lpPal->palPalEntry[i].peFlags = 0; 
	}

	/* create the logical palette */
	hPalGrey = CreatePalette(lpPal);
	GlobalFreePtr(lpPal);
	
	if (hPalGrey == NULL)
	    return 0;   /*exit if CreatePalette failed*/

    }
		
    // realize the palette
    if (hPalOld == NULL)
	hPalOld = SelectPalette(hdc,hPalGrey,FALSE);   /*save old palette*/
    else
	SelectPalette(hdc,hPalGrey,FALSE);
	
    return RealizePalette(hdc);
   
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   RestorePalette

    Description:
	This function restores the original palette in the specified device context.
	This function is designed to be called after SetPal128Grey()
	
    Input Arguments:
	HDC hdc    handle to device context

    Output Arguments:
	None

    Return Value:
	None
	

*/

void RestorePalette(HDC hdc)
{
   
    if (hPalOld != NULL) {
	SelectPalette(hdc,hPalOld,FALSE);
	hPalOld = NULL;
    }
    return;
}
 

/**************************************************************************/
/**************************************************************************/

/*  Function:   DrawDisplayBitmap

    Description:
	This function draws the current frame to the specified window and
	hdc.
	
    Input Arguments:
	HWND hWnd    -- handle of window to draw bitmap into 
	HDC hdc      -- HDC of window to fill with bitmap
	
    Output Arguments:
	None

    Return Value:
	None
	

*/

void DrawDisplayBitmap(HWND hWnd,HDC hdc)
{
    int i;              /* generic counter*/
    
    HDC hDCMemory;      /* handle of memory DC to hold frame buffer bitmap*/
    HBITMAP hBmpMem;    /* Handle of bitmaps used to display frame buffer*/
    HBITMAP hBmpOld;    /* old bitmap - save for clean-up*/
    int height,width;   /* height and width of image*/
    int orgx,orgy;      /* source origin */

	orgx = orgx;
    SetPal128Grey(hdc);     /* setup palette*/
    
    if (lpBmi != NULL) {
	/*if bit map info exists, update height & width*/
	lpBmi->bmiHeader.biWidth = (long)CurDevInfo.FrameInfo.Width;
	lpBmi->bmiHeader.biHeight = (long)CurDevInfo.FrameInfo.Height;
    }
    else { 
	/* Otherwise, allocate and build a bitmap info structure */
	lpBmi = GlobalAllocPtr(GHND,sizeof(BITMAPINFOHEADER) + (256*sizeof(RGBQUAD)) );
	     
	/* initialize the DIB header */
	    
	lpBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpBmi->bmiHeader.biWidth = (long)CurDevInfo.FrameInfo.Width;
	lpBmi->bmiHeader.biHeight = (long)CurDevInfo.FrameInfo.Height;
	lpBmi->bmiHeader.biPlanes = 1;
	lpBmi->bmiHeader.biBitCount = 8;
	lpBmi->bmiHeader.biCompression = BI_RGB;
	lpBmi->bmiHeader.biSizeImage = 0;
	lpBmi->bmiHeader.biXPelsPerMeter = 0;    
	lpBmi->bmiHeader.biYPelsPerMeter = 0;
	lpBmi->bmiHeader.biClrUsed = 0;
	lpBmi->bmiHeader.biClrImportant = 0;
    
	/* initialize the DIB's palette as indexes into the logical palette*/
    
	for (i=0;i<128;i++){        
	    lpBmi->bmiColors[i].rgbRed   = i;  
	    lpBmi->bmiColors[i].rgbGreen =  0;  
	    lpBmi->bmiColors[i].rgbBlue  = i;  
	    lpBmi->bmiColors[i].rgbReserved = 0;
	}
    }
    
    /****** Create the Windows DIB *******/
    
    hBmpMem = CreateDIBitmap(hdc, (BITMAPINFOHEADER FAR *)lpBmi,
			 CBM_INIT, CurDevInfo.lpBmpData, lpBmi, DIB_PAL_COLORS);

    /**** draw the bitmap to the display ****/

    hDCMemory = CreateCompatibleDC(hdc);
    hBmpOld = SelectObject(hDCMemory,hBmpMem);

    height =  CurDevInfo.FrameInfo.Height;
    width =  CurDevInfo.FrameInfo.Width;
    orgx = CurDevInfo.OriginX;
    orgy = CurDevInfo.OriginY;
    BitBlt(hdc,0,0,width-orgx,height-orgy,hDCMemory,orgx,orgy,SRCCOPY);

    /* clean-up all the fun windows resources*/
    RestorePalette(hdc);

    SelectObject(hDCMemory,hBmpOld);
    DeleteDC(hDCMemory);              /* cleanup display handles*/
    DeleteObject(hBmpMem);
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   SetupScrollBars

    Description:
	This function adgusts the scroll bars based on the specified window
	size information and the current origin and image size settings.
	
    Input Arguments:
	HWND hWnd    -- handle of window 
	UINT width   -- width of the window's client area
	UINT height  -- height of the window's client area
	
    Output Arguments:
	None

    Return Value:
	None
	

*/

void SetupScrollBars(HWND hWnd, UINT width, UINT height)
{
    int ScrollLength;
        
    /*adjust the horizontal scroll bar*/
    ScrollLength = CurDevInfo.FrameInfo.Width - width;
    if (ScrollLength > 0) {
	/*the image doesn't fit, turn on, and adjust the scroll bar*/
	SetScrollRange(hWnd, SB_HORZ, 0, ScrollLength, FALSE);
	CurDevInfo.OriginX = min(ScrollLength, CurDevInfo.OriginX);
	SetScrollPos(hWnd, SB_HORZ, CurDevInfo.OriginX, FALSE);
	ShowScrollBar(hWnd, SB_HORZ, TRUE);
    }
    else {
	/*the image fits, so turn off the scroll bar*/
	CurDevInfo.OriginX = 0;
	ShowScrollBar(hWnd, SB_HORZ, FALSE);
    }
    
    /*adjust the vertical scroll bar*/
    ScrollLength = CurDevInfo.FrameInfo.Height - height;
    if (ScrollLength > 0) {
	/*the image doesn't fit, turn on, and adjust the scroll bar*/
	SetScrollRange(hWnd, SB_VERT, 0, ScrollLength, FALSE);
	CurDevInfo.OriginY = min(ScrollLength, CurDevInfo.OriginY);
	SetScrollPos(hWnd, SB_VERT, CurDevInfo.OriginY, FALSE);
	ShowScrollBar(hWnd, SB_VERT, TRUE);
    }
    else {
	/*the image fits, so turn off the scroll bar*/
	CurDevInfo.OriginY = 0;
	ShowScrollBar(hWnd, SB_VERT, FALSE);
    }
    
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   UpdateScrollBar

    Description:
	This function is called in response to a WM_HSCROLL or WM_VSCROLL
	message.  It updates the scroll bar based on the specified scroll
	code.  The function returns the new scroll position.
	
    Input Arguments:
	HWND hWnd        -- handle of window 
	int  fnBar       -- type of scroll bar (SB_HORZ or SB_VERT)
	UINT ScrollCode  -- type of scroll bar update (see WM_VSCROLL windows SDK)
	int  NewPos      -- new position (for ScrollCodes of SB_THUMBPOSITION and SB_THUMBTRACK only) 
	
    Output Arguments:
	None

    Return Value:
	None
	

*/

int UpdateScrollBar(HWND hWnd, int fnBar, UINT ScrollCode, int NewPos)
{
    int MinScroll, MaxScroll, CurPos;
    int Granularity;
    int MaskGranularity;
    
    /* passthru must be done on groups of 4 pixels */
    
    if (CurDevInfo.PassthruActive) {
       Granularity     = 4;
       MaskGranularity = 0xFFFC;
    } 
    else {
       Granularity     = 1;
       MaskGranularity = 0xFFFF;
    }
    

    /*fetch current scroll bar info*/
    GetScrollRange(hWnd, fnBar, &MinScroll, &MaxScroll);
    CurPos = GetScrollPos(hWnd, fnBar);
    
    /*compute the new position*/
    switch (ScrollCode) {
    case SB_BOTTOM:
	CurPos = MaxScroll;
	break;
    case SB_TOP:
	CurPos = MinScroll;
	break;
    case SB_LINEDOWN:
	CurPos += Granularity;
	break;
    case SB_LINEUP:
	CurPos -= Granularity;
	break;
    case SB_PAGEUP:
	CurPos -= (MaxScroll-MinScroll)/10;   /*scroll 10% */
	break;
    case SB_PAGEDOWN:
	CurPos += (MaxScroll-MinScroll)/10;   /*scroll 10% */
	break;
    case SB_THUMBPOSITION:
	CurPos = NewPos;
	break;
    default:
	break;
    }
    
    /*insure that the new position is in range*/
    CurPos = max(CurPos,MinScroll); 
    CurPos = min(CurPos,MaxScroll);
		   
    /*insure granularity*/               
    CurPos = CurPos & MaskGranularity;
		   
    /*update scroll bar*/
    SetScrollPos(hWnd, fnBar, CurPos, TRUE);
    
    return CurPos; 
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   AcquireSingleFrame

    Description:
	This function acquires a single frame of image data.  It then inverts the
	data so it can be used to build a DIB.  This function also updates the
	windows scroll bars based on the image origin & size and the window size.
	
    Input Arguments:
	HWND hWnd        -- handle of window
	
    Output Arguments:
	None

    Return Value:
	None
*/

void AcquireSingleFrame(HWND hWnd)
{
    OLT_APISTATUS result;
    HCURSOR hOldCur;
	LONG ulBitsGreaterThan8;
    
    /*exit if single frame acquire not supported*/
    if (!CurDevCaps.DoesDeviceAcquire)
	return;
		
    /*acquire frame*/    
    hOldCur = SetCursor(LoadCursor(NULL,IDC_WAIT));
    result = OlFgAcquireFrameToDevice(CurDevInfo.DevId, CurDevInfo.FrameId);
    if (!OlImgIsOkay(result)) {
	/*show error string if acquire failed*/
	OlImgGetStatusMessage(result,tstr,sizeof(tstr));
	MessageBox(hWnd,tstr,"Acquisition Error",MB_OK | MB_ICONEXCLAMATION);
	CurDevInfo.FrameDataValid = FALSE;
    }
    else/*acquire was ok, so copy & invert image data*/
	{
	if(CurDevCaps.DoesDrawAcqFrame)
	{
		OlFgDrawAcquiredFrame(CurDevInfo.DevId, hWnd, CurDevInfo.FrameId);

	}
	else
	{
		int i, r;
		RECT rect;
		int height =  CurDevInfo.FrameInfo.Height;
		int width =  CurDevInfo.FrameInfo.Width;
	

		/* get point to last line of DIB data memory  (DIB is upside down !)*/
		// this will be our destination and will always be 8 bit data
		BYTE _huge * lpBmpDataPtr = CurDevInfo.lpBmpData +
				((long)height * (long)width) - (long)width;
	
		if (CurDevInfo.FrameInfo.BytesPerSample != 2)
			{
		// obtain a byte pointer to the acquire region
			BYTE __huge * lpFramePtr = CurDevInfo.FrameInfo.BaseAddress;  /*get frame address*/
		
		/* load & invert DIB data from the device frame buffer*/
			for (i=0;i<height;i++){
				hmemcpy(lpBmpDataPtr, lpFramePtr, width);
				lpFramePtr += width;
				lpBmpDataPtr -= width;   
				}
			}
		   else
			{

			// obtain a word pointer to the acquire region
		   WORD __huge * lpFramePtr = CurDevInfo.FrameInfo.BaseAddress;  /*get frame address*/


		   ulBitsGreaterThan8 = CurDevInfo.FrameInfo.BitsPerSample - 8;	

			// now loop through the whole image pixel by pixel so that they
			// can be shifted if necessary
			// divide Mem siz by width of a line, to determine rows(height)
			   for (r = 0; r < (int)height; r++)  
				{
			   // copy each individual line of rendered data to the new buffer
			   for (i = 0; i < (int)width; i++)  
					  {
						*lpBmpDataPtr = (BYTE)(*lpFramePtr >> ulBitsGreaterThan8);
						lpFramePtr++;  //increment on the current line
						lpBmpDataPtr ++;
						}

				  lpBmpDataPtr -= (width * 2); // now lets decrement back to the start of the 
					// next line, because we incremented the pointer a whole line during the transfer
					// process, we need to go back two lines worth.
					//
					}
			 }

			/*force a repaint to show the data*/
			InvalidateRect(hWnd, NULL, TRUE);
			GetClientRect(hWnd,&rect);
			SetupScrollBars(hWnd, rect.right, rect.bottom);   
	}
			/*setup scroll bars*/
			CurDevInfo.FrameDataValid = TRUE;
	}
		
    SetCursor(hOldCur);
}

/**************************************************************************/
/**************************************************************************/

/*  Function:   OnStartPassthru

    Description:
	This function is called in response to a startpassthru command.
	It sets passthru origin and starts passthru based on the settings 
	in the CurDevInfo global structure.  If syncronous passthru is selected,
	the function also grabs a single frame of data once passthru is canceled.
	If asyncronous passthru is selected, the function updates the enabled state
	of the start and stop items of the run menu. 
	
    Input Arguments:
	HWND hWnd        -- handle of window
	
    Output Arguments:
	None

    Return Value:
	None
*/

void OnStartPassthru(HWND hWnd)
{
    OLT_APISTATUS result;
    POINT OrgPoint;
	    
    /* force to x4 boundary */
    /* set passthru origin  */
    OrgPoint.x = CurDevInfo.OriginX = CurDevInfo.OriginX & 0xFFFC; 
    OrgPoint.y = CurDevInfo.OriginY = CurDevInfo.OriginY & 0xFFFC;

    if(CurDevCaps.DoesSoruceOrigin)
	{
		result = OlFgSetPassthruSourceOrigin(CurDevInfo.DevId, &OrgPoint);
	
		if (!OlImgIsOkay(result))
		{	
		OlImgGetStatusMessage(result,tstr,sizeof(tstr));
		MessageBox(hWnd,tstr,"Passthru Error",MB_OK | MB_ICONEXCLAMATION);
		return;
		}

	}
  /*start passthru*/
    
	if (CurDevInfo.AsyncPassthru) {	
	    /*start asyncronous passthru*/
	    if (CurDevCaps.DoesAsyncPassthruDirect)
		result = OlFgStartAsyncPassthruDirect(CurDevInfo.DevId, hWnd, &CurDevInfo.PassJobId);
	    else
		result = OlFgStartAsyncPassthruBitmap(CurDevInfo.DevId,
											hWnd, CurDevInfo.FrameId, &CurDevInfo.PassJobId);
	    if (OlImgIsOkay(result))
		{
			/*if all ok, update menu and passthru active flag*/ 
			CurDevInfo.PassthruActive = TRUE;
			EnableMenuItem(GetMenu(hWnd), IDM_RUN_START_PASSTHRU, MF_GRAYED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd), IDM_RUN_STOP_PASSTHRU, MF_ENABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd), IDM_SETUP_CLOSE_DEVICE, MF_GRAYED | MF_BYCOMMAND);
		}
	}
	else
	{
	    /*start syncronous passthru*/
	    CurDevInfo.PassthruActive = TRUE;
	    if (CurDevCaps.DoesSyncPassthruDirect)
		result = OlFgStartSyncPassthruDirect(CurDevInfo.DevId, hWnd);
	    else
		result = OlFgStartSyncPassthruBitmap(CurDevInfo.DevId, hWnd,
			    CurDevInfo.FrameId);
	    CurDevInfo.PassthruActive = FALSE;
		    
	    /*grab & display a single frame, we need this so that we can 
	    have data to paint the screen*/
//            AcquireSingleFrame(hWnd);
	}
   
    /*show error string if passthru failed*/
    if (!OlImgIsOkay(result)) {
	OlImgGetStatusMessage(result,tstr,sizeof(tstr));
	MessageBox(hWnd,tstr,"Passthru Error",MB_OK | MB_ICONEXCLAMATION);
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
     ofn.lpstrTitle        = "Load Camera File" ;
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
     GetOpenFileName (&ofn);
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
      // iTemp = GetPrivateProfileInt( "camvals", "Pixel count", -1, ofn.lpstrFile);
	  // if(iTemp == -1)
		   iTemp = 640;	// if camera file is unsuccessful, load default value
       Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_ACTIVE_PIXEL_COUNT,
                        (ULNG)iTemp, &ulOldData);
	   CurDevInfo.FrameInfo.Width = iTemp;  // update the FrameInfo Structure
    }


    if (Status == OLC_STS_NORMAL)
    {
       // set up sync sentinel (usually OFF)
        iTemp = GetPrivateProfileInt( "camvals", "Sync sentinel", -1, ofn.lpstrFile);
        Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_SYNC_SENTINEL,
                        (ULNG)iTemp, &ulOldData);
    }

	// if Sync Sentinel is enabled then set Vert/Horz Search and Insert Values
	if(iTemp)
	{
		if (Status == OLC_STS_NORMAL)
		{
		   // set up horiz sync ignore     
			GetPrivateProfileString( "camvals", "Sync ignore length", "?", lpstrReturnedString,
											nSize, ofn.lpstrFile);
			fTemp = (float)atof( lpstrReturnedString );
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
      // iTemp = GetPrivateProfileInt( "camvals", "Row length", -1, ofn.lpstrFile);
	  // if(iTemp == -1)
		   iTemp = 480;	// if camera file was unsuccessful, load default value
       Status = OlFgSetInputControlValue ( CurDevInfo.DevId, VSRC0, OLC_FG_CTL_ACTIVE_LINE_COUNT,
                        (ULNG)iTemp, &ulOldData);
   	   CurDevInfo.FrameInfo.Height = iTemp;  // update the FrameInfo Structure

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
       //ScaleDataReq.cx = GetPrivateProfileInt( "camvals", "Passthru Scaling X", -1, ofn.lpstrFile);
	   //if(ScaleDataReq.cx == -1)
		   ScaleDataReq.cx = 640;  // if camera file is unsuccessful, load default
       //ScaleDataReq.cy = GetPrivateProfileInt( "camvals", "Passthru Scaling Y", -1, ofn.lpstrFile);
	   //if(ScaleDataReq.cy == -1)
		   ScaleDataReq.cy = 480;  // if camera file is unsuccessful, load default
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


WORD DrawRGBFrame (HWND hWnd, VOID* FrameAddr, WORD Width, WORD Height)
{

	HDC			hDC;            // used for bitblt call					 
	HDC			hDCMemory;      // used for settting up the bitmap before bitblting
	HBITMAP		hBmpMem;        // bitmap of image data
	
	WORD		BytesPerPixel = 0;
	DWORD		FrameSize = 0;
	HWND		TargetWindow = hWnd;
	WORD		PixelDepth = 4;		// RGB 
	
	#ifdef _16M_COLORS
		HGLOBAL		hTemp;
		BYTE*		Source = (BYTE*) FrameAddr;
		BYTE*		Dest;
		VOID*		Temp;
		DWORD		i;
	#endif
	

	//_ASSERT(FrameAddr);
	if (!FrameAddr) return FALSE;
	if(!TargetWindow)
	{
		RECT		Desktop = {0};
		WORD		X0,X1,Y0,Y1;
		
		FrameSize		= Width * Height * PixelDepth;		// *4 -> RGB...


		
		 GetClientRect(GetDesktopWindow(),&Desktop);
		
		 X0 = (Desktop.right -  Width)/2;
		 Y0 = (Desktop.bottom - Height)/2;
		 X1 = X0 + Width + 2;
		 Y1 = Y0 + Height + GetSystemMetrics(SM_CYCAPTION)  +  1;
		
	//	TargetWindow = ShowBasicWindow (X0,Y0,X1,Y1);
		SetWindowLong(TargetWindow,0,(LONG) FrameAddr);
		SetWindowLong(TargetWindow,4,(LONG) Width);
		SetWindowLong(TargetWindow,8,(LONG) Height);
	}

	#ifdef _16M_COLORS
		hTemp = GlobalAlloc(GMEM_MOVEABLE,(641*481*3));

		if (!hTemp)
		{
			DtMessageBoxPrintf("Draw frame in 16M colors, out of memory!");
			return FALSE;
		}
		Dest = GlobalLock(hTemp);
		Temp = Dest;

		for(i=0; i<(DWORD)(Width*Height*4);i+=4)
		{
			*Dest++ = *Source++;
			*Dest++ = *Source++;
			*Dest++ = *Source++;
			Source++;
		}

		FrameAddr = Temp;
		PixelDepth = 3;
	#endif

	FrameSize		= Width * Height * PixelDepth;

	hDC = GetDC(TargetWindow);
	hBmpMem = CreateCompatibleBitmap(hDC,Width, Height);
    hDCMemory = CreateCompatibleDC(hDC);

	if (!(hDC && hBmpMem && hDCMemory))
	{
		DeleteDC(hDCMemory);
		DeleteObject(hBmpMem);
		ReleaseDC(TargetWindow,hDC);


       return FALSE;
	}

    SetBitmapBits(hBmpMem, FrameSize, FrameAddr);
    SelectObject(hDCMemory, hBmpMem);
    StretchBlt( hDC,0, 0, Width, Height, 
				hDCMemory,0, 0, Width, Height,SRCCOPY);
//    BitBlt( hDC,0, 0, Width, Height, 
//				hDCMemory,0, 0,SRCCOPY);


    DeleteDC(hDCMemory);			 
    DeleteObject(hBmpMem);
    ReleaseDC(TargetWindow,hDC);

#ifdef _16M_COLORS
	if (Dest) GlobalUnlock (Dest);
	if (hTemp) GlobalFree (hTemp);
#endif

    return TRUE;
}
