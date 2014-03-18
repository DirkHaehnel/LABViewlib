#include "passthru.h"            /* specific to this program */

LRESULT CALLBACK _export SnapWindowProc(HWND, UINT, WPARAM, LPARAM);
void RegisterSnapWindowClass(HWND);

extern HANDLE hSurface1;
extern HINSTANCE hInst;
extern OLT_FG_FRAME_ID	FrameId;

/****************************************************************************/
/****************************************************************************/
/*
 FUNCTION			 :	ShowBasicWindow

 Description        :  This routine is to help the developer to build
						in a quick way a window where you can display, for
						example, a frame buffer. The window has only a system
						menu to allow the user to shut it down.

						There is no REDRAW handler so anything displayed
						in such window will disappear as soon as the window
						is clipped by some other window.

 Comments           :  This function has development purpose only. 
 See also           : 
 Author             :  Dominic Cote, Data Translation inc. 08/29/96
*/
/****************************************************************************/


HWND ShowBasicWindow (HWND hWnd, WORD X1, WORD Y1, WORD X2, WORD Y2)
{
	HWND		NewWindow = 0;		
	WORD		SizeX = X2 - X1;
	WORD		SizeY = Y2 - Y1;

	NewWindow = CreateWindow("SNAPSHOT_WINDOW",				// Default class
							"SnapShot Window",				// title
							WS_SYSMENU|WS_OVERLAPPED,		// style
							X1, Y1,							// x, y position
							SizeX, SizeY,					// size 
							hWnd,							// No Parent
							NULL,							// menu
							NULL, 
							NULL);


	ShowWindow(NewWindow,SW_SHOWDEFAULT);

	return NewWindow;
};


LRESULT CALLBACK _export SnapWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{  

	switch(Msg)
	{
		case WM_CREATE:
		{
			// set values to zero
			SetWindowLong(hWnd,0,0);
			SetWindowLong(hWnd,4,0);
			SetWindowLong(hWnd,8,0);

			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;

			BeginPaint(hWnd,&Paint);

			// if there is a frame id storage here
			if (GetWindowLong(hWnd,0))
			{
				// retrieve frame id 
				OLT_FG_FRAME_ID	FrameId = (OLT_FG_FRAME_ID) GetWindowLong(hWnd,0);
		
				// Draw "snapped" image
				OlFgDrawAcquiredFrame(CurDevInfo.DevId, hWnd, FrameId);
			}

			EndPaint(hWnd,&Paint);

			break;
		}

		default:
		{
			return (DefWindowProc(hWnd,Msg,wParam,lParam));
		}
	}

return TRUE;
}

/****************************************************************************/
/****************************************************************************/

/*  Function:   SnapShotProc

    Description:
	This function is the dialog procedure for the dialog box used to
	select the DT-Open Layers Imaging SnapShot options.
    
    Initialization:
	This dialog box should be invoked using CreateDialog.  

    Input Arguments:
	HWND hDlg       dialog handle
	UINT message        message
	WPARAM wParam       16-bit message-specific data
	LPARAM lParam       32-bit message-specific data

    Output Arguments:
	None

    EndDialog Return:  BOOL

    Returns:  BOOL
	Except in response to WM_INITDIALOG, TRUE is returned if the
	dialog proc processes the message, FALSE if it does not.  For
	WM_INITDIALOG, FALSE is returned if the dialog proc calls
	SetFocus to set the input focus to a specific control.  If
	not, TRUE should be returned to indicate that the input focus
	should be given to the first control in the dialog that can
	receive focus. 
	
	
    
*/

BOOL CALLBACK SnapShotProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
			BOOL	Overlay				= FALSE;
			BOOL	Translucent			= FALSE;
	static	BOOL	DoesTranslucent		= FALSE;
	static	BOOL	DoesOverlayOnFrame	= FALSE;
			DWORD	SnapFlags			= 0;
	static	HWND	SnaphWnd			= NULL;
    OLT_APISTATUS	Status;
	char			StatusMsg[OLC_MAX_STATUS_MESSAGE_SIZE];


	switch (message)
    {

       /* message: initialize dialog box */
       
		case WM_INITDIALOG:
			// Register Snap Window
			RegisterSnapWindowClass(hDlg);

			// Find out what the driver supports 
			OlFgQueryDDICaps(CurDevInfo.DevId, OLC_FG_DDI_TRANSLUCENT_OVERLAYS, &DoesTranslucent);
			OlFgQueryDDICaps(CurDevInfo.DevId, OLC_FG_DDI_OVERLAY_ON_FRAME, &DoesOverlayOnFrame);
		

			// Disable check box initially
			EnableWindow(GetDlgItem(hDlg,IDC_TRANSLUCENT), FALSE);
			
			if(hSurface1 != NULL)
			{
				// Enable the Overlay check box based on capability
				EnableWindow(GetDlgItem(hDlg, IDC_OVERLAY), DoesOverlayOnFrame);
			}
			
			// Disable overlays Initially
			CheckDlgButton(hDlg, IDC_OVERLAY, Overlay);
	
			if(FrameId == NULL)
			{
				Status = OlFgAllocateBuiltInFrame(CurDevInfo.DevId, OLC_FG_DEV_MEM_VOLATILE,
																	OLC_FG_NEXT_FRAME, &FrameId);
				if(Status != OLC_STS_NORMAL)
				{
					//Get the error message from the driver and display it
					OlImgGetStatusMessage(Status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
					MessageBox(hDlg, StatusMsg, "SnapShot", MB_ICONHAND);
					
					// close dialog and Snapshot window
					DestroyWindow(hDlg);
					return FALSE;
				}
			}

			return TRUE;
	   		
       /* message: received a command */
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{

				case IDC_OVERLAY:
					//Toggle Overlay Status
					Overlay ^= IsDlgButtonChecked(hDlg, IDC_OVERLAY);
					
					if(Overlay == TRUE && DoesTranslucent == TRUE)
					{
			  			// Enable check box 
						EnableWindow(GetDlgItem(hDlg,IDC_TRANSLUCENT), TRUE);
					}
					else
					{
			  			// Disable check box
						EnableWindow(GetDlgItem(hDlg,IDC_TRANSLUCENT), FALSE);
					}

					break;

				case IDC_TRANSLUCENT:
					//Toggle Translucent Status
					break;

				case IDC_SNAP:
					if(IsWindow(SnaphWnd) == FALSE)
					{
						// location of left hand side of window 
						USHRT y = 50;
						
						// Create a window to display snapshot
						SnaphWnd = ShowBasicWindow(GetParent(hDlg),0,y,CurDevInfo.FrameInfo.Width,
														 (WORD)	(CurDevInfo.FrameInfo.Height + y));


						// set the Window info with the frame Id to be retrieved during Paint 
						SetWindowLong(SnaphWnd,0,(LONG)FrameId);
					}
					
					// if a valid handle to the snapshot window
					if(SnaphWnd != NULL)
					{
						// snap the current frame
						OlFgPassthruSnapShot(CurDevInfo.DevId, FrameId);
						
						// if the ovelay button is checked and a valid surface exists
						if(IsDlgButtonChecked(hDlg, IDC_OVERLAY) == 1 && hSurface1 != NULL)
						{
							// if translucent button is checked
							if(IsDlgButtonChecked(hDlg, IDC_TRANSLUCENT) == 1)
							{
								// Apply the translucent overlay 
								OlFgAddOverlayToFrame(CurDevInfo.DevId, FrameId, hSurface1, OLC_SS_TRANSLUCENT);
							}
							else
							{
								// Apply the overlay 
								OlFgAddOverlayToFrame(CurDevInfo.DevId, FrameId, hSurface1, OLC_SS_OPAQUE);
							}
						}
						
						// setup to repaint entire window
						InvalidateRect(SnaphWnd,NULL, TRUE);

						// tell snapshot window to repaint
						UpdateWindow(SnaphWnd);
					}
					break;

				case IDC_CLOSE:
					PostMessage(hDlg, WM_CLOSE, 0, 0);
					break;
	
		    }
			return FALSE;

	    case WM_CLOSE:
		//	DestroyWindow(hDlg);
			return 0;

		case WM_DESTROY:
			// close the snapshot window when closing
			DestroyWindow(SnaphWnd);

			//unregister the window class to be memory friendly
			UnregisterClass("SNAPSHOT_WINDOW", hInst);


			break;

	}

   	return FALSE;
}



void RegisterSnapWindowClass(HWND hWnd)
{
	WNDCLASS    wc    = {0};

	wc.style           = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc     = SnapWindowProc;
	wc.cbClsExtra      = 0;
	wc.cbWndExtra      = DLGWINDOWEXTRA;
	wc.hInstance       = hInst;
	wc.hIcon           = 0;
	wc.hCursor         = 0;
	wc.hbrBackground   = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName    = NULL;
	wc.lpszClassName   = "SNAPSHOT_WINDOW";
      
    if (RegisterClass(&wc) == 0)
	{
		MessageBox(hWnd, "Error Registering Snap Window", "RegisterSnapWindowClass", MB_ICONSTOP);
	
	}
	
}