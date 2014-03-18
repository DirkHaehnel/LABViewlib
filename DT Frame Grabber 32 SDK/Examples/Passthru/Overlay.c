
#include "passthru.h"            /* specific to this program */
#include <time.h>

// the application's instance
extern	HINSTANCE hInst;

// handles to surfaces surface1 is also used in snapshots (global)
extern	HANDLE hSurface1;
HANDLE	hSurface2 = NULL;

//Handle to the animation update thread
HANDLE	AnimateThread;

// Handle the driver event - indicates it is OK to update bitmap
HANDLE	AnimateEvent; 

// Handles and HDCs to animation bitmaps
HBITMAP	hBmp = NULL;
HBITMAP	hBlackBmp = NULL;
HDC		hdcBmp = NULL;
HDC		hdcBlackBmp = NULL;

// Square size and bitmap sizes of animation bitmaps
INT		Size, Width, Height;

// Flag to indicate if a timer is running
BOOL	TimerOn = FALSE;

// Flag to indicate if the animation thread is running
BOOL	Animate = FALSE;


// Variables to hold frame dimensions
DWORD FrameWidth,FrameHeight;

// Prototypes of function used to create overlay examples
BOOL  CreateSurfaces(HWND, BOOL);
BOOL  PutBitmapOnSurface(HWND, INT, HANDLE, DWORD, DWORD);
BOOL  AnimateBitmap(HWND, INT);
BOOL  DestroySurfaces(HWND);
DWORD UpdateAnimation(HWND);
BOOL  DisplayTime(HWND);
/****************************************************************************/
/****************************************************************************/

/*  Function:   OverlayControlProc

    Description:
	This function is the dialog procedure for the dialog box used to
	select the DT-Open Layers Imaging Overlay options.
    
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

BOOL CALLBACK OverlayControlProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
			BOOL		DoesTranslucent		= FALSE;
			BOOL		DoesMultiSurfaces	= FALSE;
			BOOL		DoesHiResWhite		= FALSE;
			BOOL		Translucent			= FALSE;
			BOOL		DoesOverlayOnFrame	= FALSE;
			RGBTRIPLE	BackGndColor;		// Background
	static	HWND		SnaphWnd;

	switch (message)
    {

       /* message: initialize dialog box */
       
		case WM_INITDIALOG:

			// Center the window on the desktop
			CenterWindow(hDlg, GetParent(hDlg));
			
			// Find out what Overlays the driver supports 
			OlFgQueryDDICaps(CurDevInfo.DevId, OLC_FG_DDI_TRANSLUCENT_OVERLAYS, &DoesTranslucent);
			OlFgQueryDDICaps(CurDevInfo.DevId, OLC_FG_DDI_MULTIPLE_SURFACES, &DoesMultiSurfaces);
			OlFgQueryDDICaps(CurDevInfo.DevId, OLC_FG_DDI_OVERLAY_ON_FRAME, &DoesOverlayOnFrame);

			// Get handle(s) to the surface(s) FALSE means there was an error
			if(CreateSurfaces(hDlg, DoesMultiSurfaces) == FALSE)
			{
				//close dialog if couldn't create surfaces
				PostMessage(hDlg, WM_CLOSE, 0, 0);
				break;
			}

 
			// Disable check box if can't do translucent Overlays
			EnableWindow(GetDlgItem(hDlg,IDC_TRANSLUCENT), DoesTranslucent);
			
			// Set default state of tranluscent
			if(DoesTranslucent == TRUE)
			{
				OlFgSetTranslucentOverlay(CurDevInfo.DevId, Translucent);
			}

			
			// Enable the Radio button based on capabilities
			EnableWindow(GetDlgItem(hDlg, IDC_SURFACE_2), DoesMultiSurfaces);
			EnableWindow(GetDlgItem(hDlg, IDC_EARTH), DoesMultiSurfaces);
			EnableWindow(GetDlgItem(hDlg, IDC_DONUT), DoesMultiSurfaces);
			EnableWindow(GetDlgItem(hDlg, IDC_DRAWING), TRUE);
			
			// Disable overlays Initially
			CheckDlgButton(hDlg, IDC_NO_OVERLAYS, TRUE);

			// Find SnapShot window

			SnaphWnd = FindWindow(NULL,"Passthru SnapShot");
			
			if(SnaphWnd != NULL)
			{

				// Enable the Overlay check box on the snapshot dialog based on capability
				EnableWindow(GetDlgItem(SnaphWnd, IDC_OVERLAY), DoesOverlayOnFrame);
				if(IsDlgButtonChecked(SnaphWnd, IDC_OVERLAY) == 1)
				{
					// enable Translucent if available
					EnableWindow(GetDlgItem(SnaphWnd, IDC_TRANSLUCENT), DoesTranslucent);
				}
			}

			return TRUE;
	   		
       /* message: received a command */
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_DONUT:
					// If animation is already running shut it down first
					if(Animate == TRUE)
					{
						// Set Animate flag to FALSE to stop Animation thread
						Animate = FALSE;
						// Give the animation thread 5 seconds to terminate
						WaitForSingleObject(AnimateThread, 5000);
					}
					// Set the block size and Bitmap dimensions
					Size   = 64;
					Width  = 640;
					Height = 384;

					AnimateBitmap(hDlg,IDB_DONUT);
					break;

				case IDC_EARTH:
					// If animation is already running shut it down first
					if(Animate == TRUE)
					{
						// Set Animate flag to FALSE to stop Animation thread
						Animate = FALSE;
						// Give the animation thread 5 seconds to terminate
						WaitForSingleObject(AnimateThread, 5000);
					}
					// Set the block size and Bitmap dimensions
					Size   = 128;
					Width  = 512;
					Height = 512;
	
					AnimateBitmap(hDlg,IDB_EARTH);
					break;

				case IDC_DRAWING:/* set up a timer to display clock then trap mouse
									messages to draw on the overlayed surface */
					
					// shut down animation if on
					if(Animate == TRUE)
					{
						// Set Animate flag to FALSE to stop Animation thread
						Animate = FALSE;
						// Give the animation thread 5 seconds to terminate
						WaitForSingleObject(AnimateThread, 5000);
					}

					// if the timer isn't already running start it
					if(TimerOn == FALSE)
					{
					 	// set a timer to post messages every second to update clock
						SetTimer(hDlg, 1, 1000, NULL);

						// Set timer flag to TRUE
						TimerOn = TRUE;
					}
					// Set the color key to black background color 
					BackGndColor.rgbtRed	= 0x0;
					BackGndColor.rgbtGreen	= 0x0;
					BackGndColor.rgbtBlue	= 0x0;
					
					// Set the surface's color key
					OlFgSetOverlayColorKey(CurDevInfo.DevId, hSurface1, BackGndColor);
				
					// Erase the surface before adding new image
					OlFgEraseSurface(CurDevInfo.DevId, hSurface1);

					break;

				case IDC_BLUE_LOGO:
					// shut down animation if on
					if(Animate == TRUE)
					{
						// Set Animate flag to FALSE to stop Animation thread
						Animate = FALSE;

						// Give the animation thread 5 seconds to terminate
						WaitForSingleObject(AnimateThread, 5000);
					}
					
					// disable Overlay initially
					OlFgEnableOverlays(CurDevInfo.DevId, FALSE);

					// Set the color key to bitmap black background color 
					BackGndColor.rgbtRed	= 0;
					BackGndColor.rgbtGreen	= 0;
					BackGndColor.rgbtBlue	= 0;

					// Set the surface's color key
					OlFgSetOverlayColorKey(CurDevInfo.DevId, hSurface1, BackGndColor);
				
					// Erase the surface before adding new image
					OlFgEraseSurface(CurDevInfo.DevId, hSurface1);
					
					// switch DT logo bitmap
					PutBitmapOnSurface(hDlg, IDB_DT_LOGO, hSurface1,MAKELONG(636,106),
																	MAKELONG(620,106));
					break;

				case IDC_CLEAR_LOGO:
					// shut down animation if on
					if(Animate == TRUE)
					{
						// Set Animate flag to FALSE to stop Animation thread
						Animate = FALSE;

						// Give the animation thread 5 seconds to terminate
						WaitForSingleObject(AnimateThread, 5000);
					}

					// disable Overlay initially
					OlFgEnableOverlays(CurDevInfo.DevId, FALSE);
					
					// Set the color key to bitmap Black background color 
					BackGndColor.rgbtRed	= 0x0;
					BackGndColor.rgbtGreen	= 0x0;
					BackGndColor.rgbtBlue	= 0x0;

					// Set the surface's color key
					OlFgSetOverlayColorKey(CurDevInfo.DevId, hSurface1, BackGndColor);

					// Erase the surface 
					OlFgEraseSurface(CurDevInfo.DevId, hSurface1);

					// Set the color key to bitmap Blue background color 
					BackGndColor.rgbtBlue	= 0xFF;
					OlFgSetOverlayColorKey(CurDevInfo.DevId, hSurface1, BackGndColor);

					// switch DT logo bitmap
					PutBitmapOnSurface(hDlg, IDB_DT_LOGO, hSurface1,MAKELONG(636,106),
																	MAKELONG(620,106));
					break;

				case IDC_NO_OVERLAYS:
					// Shut down Overlays
					OlFgEnableOverlays(CurDevInfo.DevId, FALSE);

					// shut down animation if on
					if(Animate == TRUE)
					{
						// Set Animate flag to FALSE to stop Animation thread
						Animate = FALSE;
					}

					// shut down timer if on
					if(TimerOn == TRUE)
					{
						// stop timer
						KillTimer(hDlg, 1);
						
						// Set timer flag to FALSE
						TimerOn = FALSE;
					}
					break;

				case IDC_TRANSLUCENT:
					// Toggle Translucent state
					Translucent ^= IsDlgButtonChecked(hDlg, IDC_TRANSLUCENT);
					
					// Set the Translucent state to current value
					OlFgSetTranslucentOverlay(CurDevInfo.DevId, Translucent);
					break;


				case IDC_CLOSE:
					//
					PostMessage(hDlg, WM_CLOSE, 0, 0);
					break;
	
		    }
			return FALSE;

	    case WM_CLOSE:
			if(SnaphWnd != NULL)
			{
				// Disable the Overlay & translucent check boxes on the snapshot dialog on close
				EnableWindow(GetDlgItem(SnaphWnd, IDC_OVERLAY), FALSE);
				EnableWindow(GetDlgItem(SnaphWnd, IDC_TRANSLUCENT), FALSE);
			}
			// tell windows that this window is no longer needed
			DestroyWindow(hDlg);
			return TRUE;

		case WM_DESTROY:
			// shut down animation if on
			if(Animate == TRUE)
			{
				// Set Animate flag to FALSE to stop Animation thread
				Animate = FALSE;

				// Give the animation thread 5 seconds to terminate
				WaitForSingleObject(AnimateThread, 5000);
			}

			// Disable Overlays and destroy Surfaces
			OlFgEnableOverlays(CurDevInfo.DevId, FALSE);
			DestroySurfaces(hDlg);

			// shut down timer if on
			if(TimerOn == TRUE)
			{
				// stop timer
				KillTimer(hDlg, 1);
				
				// Set timer flag to FALSE
				TimerOn = FALSE;
			}
			
			break;

		case WM_TIMER:
			// every second (timer message) update clock on the surface
			DisplayTime(hDlg);
			break;
		
	}

   	return FALSE;
}

/****************************************************************************/
/****************************************************************************/

/*  Function:   CreateSurfaces

    Description:
	This function is used to Get up to two handles to
	Overlay surfaces depending on the DoesMultipleSurfaces
	flag.
    
    Initialization:

	Arguments:
		hWnd					handle to calling window
		DoesMultipleSurfaces	TRUE  = Create two surfaces
								FALSE = Create one surface

    
    
    Returns:  BOOL
		TRUE if all requested surfaces where allocated FALSE if not. 
	
	   
*/

BOOL CreateSurfaces(HWND hWnd, BOOL DoesMultipleSurfaces)
{
	USHRT			InputSrc;
    OLT_APISTATUS	Status;
	char			StatusMsg[OLC_MAX_STATUS_MESSAGE_SIZE];


    // Get the current source and select it in the combo box 
    OlFgQueryInputVideoSource(CurDevInfo.DevId, &InputSrc);

	// Get the current Height
	OlFgQueryInputControlValue(CurDevInfo.DevId, InputSrc, OLC_FG_CTL_FRAME_HEIGHT, &FrameHeight);

	// Get the current Width
	OlFgQueryInputControlValue(CurDevInfo.DevId, InputSrc, OLC_FG_CTL_FRAME_WIDTH, &FrameWidth);

	// Create first surface
	Status = OlFgCreateSurface(CurDevInfo.DevId, &hSurface1, FrameWidth, FrameHeight,0);

	if(Status != OLC_STS_NORMAL)
	{
		//Get the error message from the driver and display it
		OlImgGetStatusMessage(Status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
		MessageBox(hWnd, StatusMsg, "Create Surfaces", MB_ICONHAND);
		return FALSE;
	}

	// if driver says it can do multiple then create another
	if(DoesMultipleSurfaces == TRUE)
	{
		// Create Second surface
		Status = OlFgCreateSurface(CurDevInfo.DevId, &hSurface2, FrameWidth, FrameHeight,0);

		if(Status != OLC_STS_NORMAL)
		{
			//Get the error message from the driver and display it
			OlImgGetStatusMessage(Status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
			MessageBox(hWnd, StatusMsg, "Create Surfaces", MB_ICONHAND);
			return FALSE;
		}
	}

	return TRUE;
}

/****************************************************************************/
/****************************************************************************/

/*  Function:   DestroySurfaces

    Description:
	This function is used to Destroy up to two handles to
	Overlay surfaces.
    
    Initialization:

	Arguments:
		hWnd					handle to calling window
    
    
    Returns:  BOOL
		TRUE if all requested surfaces where Destroyed FALSE if not. 
	
	   
*/

BOOL DestroySurfaces(HWND hWnd)
{
    OLT_APISTATUS	Status;
	char			StatusMsg[OLC_MAX_STATUS_MESSAGE_SIZE];

	// Destroy first surface if not NULL
	if(hSurface1 != NULL)
	{
		Status = OlFgDestroySurface(CurDevInfo.DevId, hSurface1);

		if(Status != OLC_STS_NORMAL)
		{
			//Get the error message from the driver and display it
			OlImgGetStatusMessage(Status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
			MessageBox(hWnd, StatusMsg, "Destroy Surfaces", MB_ICONHAND);
		}
		hSurface1 = NULL;
	}

	// Destroy Second surface if not NULL
	if(hSurface2 != NULL)
	{
		Status = OlFgDestroySurface(CurDevInfo.DevId, hSurface2);

		if(Status != OLC_STS_NORMAL)
		{
			//Get the error message from the driver and display it
			OlImgGetStatusMessage(Status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
			MessageBox(hWnd, StatusMsg, "Destroy Surfaces", MB_ICONHAND);
			return FALSE;
		}
		hSurface2 = NULL;

	}

	return TRUE;
}

BOOL DisplayTime(HWND hWnd)
{
	char	String[9];
	HDC		hdcSurface;
	HFONT	Font;

	// Only display time if the Drawing button is selected
	if(IsDlgButtonChecked(hWnd, IDC_DRAWING) == 1) 
	{	
		// create a font to display the time in
		Font = CreateFont(	0,							// logical height of font 
							0,							// logical average character width 
							0,							// angle of escapement 
							0,							// base-line orientation angle 
							FW_NORMAL,					// font weight 
							FALSE,						// italic attribute flag 
							FALSE,						// underline attribute flag 
							FALSE,						// strikeout attribute flag 
							0,							// character set identifier 
							OUT_TT_PRECIS,				// output precision 
							CLIP_DEFAULT_PRECIS,		// clipping precision 
							DEFAULT_QUALITY,			// output quality 
							DEFAULT_PITCH|FF_DONTCARE,	// pitch and family 
							"Roman" 			// pointer to typeface name string 
						   );


		// ask the sysytem what time it is
		_strtime(String);
		
   		// Get an HDC to the surface from the driver
		OlFgGetSurfaceDC(CurDevInfo.DevId, hSurface1, &hdcSurface);
 	
		// Apply the font
		SelectObject(hdcSurface, Font);

		// use a black background color
		SetBkColor(hdcSurface, RGB(0,0,0));

		//use a white text color
		SetTextColor(hdcSurface, RGB(0xFF,0xFF,0xFF));
		
		// write the time string out
		TextOut(hdcSurface, FrameWidth-100, 0, String,8);

		// tell the driver to use this surface for overlays
		OlFgSetVisibleSurface(CurDevInfo.DevId, hSurface1);
		
		// enable overlays
		OlFgEnableOverlays(CurDevInfo.DevId, TRUE);

	//Clean up
		// release the surface's resources
		OlFgReleaseSurfaceDC(CurDevInfo.DevId, hSurface1, hdcSurface);
	
		// delete the font
		DeleteObject(Font);

	}
	else // if the drawing button isn't selected and the timer is running shut down the timer
	{
		// Is the timer running
		if(TimerOn == TRUE)
		{
			//Stop the timer
			KillTimer(hWnd, 1);

			// change Timer flag to FALSE
			TimerOn = FALSE;
		}

	}
	return TRUE;
}


BOOL DrawOnSurface(HWND hWnd, LPARAM Location, BOOL Draw)
{
		CHAR	String[80];
		HDC		hdcSurface;
		INT		x, y;
static	INT		x1=-1, y1=-1;

	// Only draw if the drawing button is selected
	if(IsDlgButtonChecked(hWnd, IDC_DRAWING) == 1) 
	{	
		// extract xy coordinates
		x = LOWORD(Location);
		y = HIWORD(Location);

		// if x1,xy <0 then there is not previous location to draw from
		if(x1<0 && y1<0)
		{
			x1=x;
			y1=y;
		}
		
		// debug stuff to watch the drawing
		wsprintf(String,"from %i,%i to %i,%i",x1,y1,x,y);
		OutputDebugString(String);

   		// Get an HDC to the surface from the driver
		OlFgGetSurfaceDC(CurDevInfo.DevId, hSurface1, &hdcSurface);
 		
		// select a white pen for drawing
		SelectObject(hdcSurface, GetStockObject(WHITE_PEN));

		// Move the cursor to last position
		MoveToEx(hdcSurface, x1, y1, NULL);

		// if the draw flag is TRUE draw to new position
		if(Draw == TRUE)
		{
			//draw to new position
			LineTo(hdcSurface, x, y);
		}

		// tell the driver to use this surface for overlays
		OlFgSetVisibleSurface(CurDevInfo.DevId, hSurface1);	// it likes this set first

		// enable overlays
		OlFgEnableOverlays(CurDevInfo.DevId, TRUE);
	//Clean up
		// release the surface's resources
		OlFgReleaseSurfaceDC(CurDevInfo.DevId, hSurface1, hdcSurface);

		// Set the new position to the old position for next line
		x1=x;
		y1=y;

	}

	return TRUE;
}
	
BOOL PutBitmapOnSurface(HWND hWnd,INT Bitmap, HANDLE hSurface, DWORD SrcWidthHeight, DWORD DstWidthHeight)
{
	HDC		hdcBitmap, hdcSurface;
	HBITMAP hBitmap;
	INT		x=0, y=0;

	// load the bitmap from the program resources
	hBitmap = LoadBitmap((HINSTANCE)GetModuleHandle("Passthru.exe"),MAKEINTRESOURCE(Bitmap));

	// Get an HDC for the bitmap
	hdcBitmap = CreateCompatibleDC(NULL);

    // apply the bitmap to the HDC
	SelectObject(hdcBitmap, hBitmap);

 	// Get an HDC to the surface from the driver
	OlFgGetSurfaceDC(CurDevInfo.DevId, hSurface, &hdcSurface);
  	
	// calculate the window horz center 
	if((FrameWidth) > LOWORD(DstWidthHeight))
	{
		x = ((FrameWidth) - LOWORD(DstWidthHeight))/2;
	}

	// calculate the window vert center 
	if((FrameHeight) > HIWORD(DstWidthHeight))
	{
		y = ((FrameHeight) - HIWORD(DstWidthHeight))/2;
	}


	// Display the bitmap on the current surface 
    StretchBlt(hdcSurface, x, y, LOWORD(DstWidthHeight), HIWORD(DstWidthHeight), hdcBitmap, 0, 0,
							     LOWORD(SrcWidthHeight), HIWORD(SrcWidthHeight), SRCCOPY);


	// tell the driver to use this surface for overlays
	OlFgSetVisibleSurface(CurDevInfo.DevId, hSurface);
	
	// enable overlays
	OlFgEnableOverlays(CurDevInfo.DevId, TRUE);

//Clean up
	// release the surface's resources
	OlFgReleaseSurfaceDC(CurDevInfo.DevId, hSurface, hdcSurface);
	
	//clead up bitmap resources
	DeleteObject(hBitmap);
	DeleteDC(hdcBitmap);

	return TRUE;
}

BOOL AnimateBitmap(HWND hWnd, INT Bitmap)
{
	RGBTRIPLE	BackGndColor = {0,0,0}; // Black Background
	DWORD dwThreadId; 

	// Set the color key to bitmap background color
	OlFgSetOverlayColorKey(CurDevInfo.DevId, hSurface1, BackGndColor);
	OlFgSetOverlayColorKey(CurDevInfo.DevId, hSurface2, BackGndColor);


	// Erase the surfaces before adding new image
	OlFgEraseSurface(CurDevInfo.DevId, hSurface1);
	OlFgEraseSurface(CurDevInfo.DevId, hSurface2);


	// load the donut up front to reduce overhead							IDB_EARTH	 IDB_DONUT
	hBmp = LoadBitmap((HINSTANCE)GetModuleHandle("Passthru.exe"),MAKEINTRESOURCE(Bitmap));
	hdcBmp = CreateCompatibleDC(NULL);
	SelectObject(hdcBmp, hBmp);

	// load the Black bitmap up front to reduce overhead
	hBlackBmp = LoadBitmap((HINSTANCE)GetModuleHandle("Passthru.exe"),MAKEINTRESOURCE(IDB_BLACK));
	hdcBlackBmp = CreateCompatibleDC(NULL);
	SelectObject(hdcBlackBmp, hBlackBmp);

	// set the Animate flag to TRUE
	Animate = TRUE;

	// tell the driver to use this surface for overlays
	OlFgSetVisibleSurface(CurDevInfo.DevId, hSurface1); 

	// enable overlays
	OlFgEnableOverlays(CurDevInfo.DevId, TRUE);

    // Create a thread to upadte the bitmap animation
	AnimateThread = CreateThread(NULL,			/* no security attributes        */ 
									0,			/* use default stack size        */ 
	(LPTHREAD_START_ROUTINE) UpdateAnimation,	/* thread function       */ 
								 hWnd,			/* argument to thread function   */ 
									0,			/* use default creation flags    */ 
							&dwThreadId);		/* returns the thread identifier */ 
 
    /* Check the return value for success. */ 
 
    if (AnimateThread == NULL) 
	{
		MessageBox(hWnd, "NULL Thread Handle", "AnimateBitmap", MB_ICONHAND);
	}
 

	return TRUE;
}

DWORD UpdateAnimation(HWND hWnd)
{
   	INT		i=0,j=0,k=4,l=4,x,y;
	HANDLE	CurrentSurface = NULL, OldSurface;
	HDC		Currenthdc, Oldhdc;
	OLT_APISTATUS	Status;
	char			StatusMsg[OLC_MAX_STATUS_MESSAGE_SIZE];

	__try{

		// ask the driver for a handle to the synchro event
		Status = OlFgGetPassthruSyncEvent(CurDevInfo.DevId, &AnimateEvent);  
		
		//make sure everything went well
		if(Status != OLC_STS_NORMAL)
		{
			//Get the error message from the driver and display it
			OlImgGetStatusMessage(Status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
			MessageBox(hWnd, StatusMsg, "Update Animation - GetSyncEvent", MB_ICONHAND);
			
			// Set flag to flag to stop animation
			Animate = FALSE;
		}

		// set bitmap section to initial location
		x = Width - Size;
		y = Height - Size;

		// stay in the loop until told to stop
		while(Animate == TRUE)
		{
			// toggle surfaces based on current one
			if(CurrentSurface == hSurface1)
			{
				CurrentSurface	= hSurface2;
				OldSurface		= hSurface1;
			}
			else
			{
				CurrentSurface	= hSurface1;
				OldSurface		= hSurface2;
			}

	   		// Get an HDC to the surface from the driver
			Status = OlFgGetSurfaceDC(CurDevInfo.DevId, CurrentSurface, &Currenthdc);

			//make sure everything went well
			if(Status != OLC_STS_NORMAL)
			{
				//Get the error message from the driver and display it
				OlImgGetStatusMessage(Status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
				MessageBox(hWnd, StatusMsg, "Animate Bitmap - GetSurfaceDC(1st)", MB_ICONHAND);
				return FALSE;
			}
			
			// update the bitmap view don't go beyond edge
			x = (x>=Width-Size) ? 0 : (x+=Size);

			if(x == 0) 
			{
				y = (y>=Height-Size) ? 0 : (y+=Size);
			}

			// display bitmap section
			//		  |	   Destination		 |			Source		 | Action
			StretchBlt(Currenthdc, i,j,Size,Size, hdcBmp, x, y, Size, Size, SRCCOPY);
  
			// debug to show where we are
			OutputDebugString("Wait for single object\n");
			
			// Release the hdc
			OlFgReleaseSurfaceDC(CurDevInfo.DevId, CurrentSurface, Currenthdc);
			
			// wait for a chance to update surface
			WaitForSingleObject(AnimateEvent, 100);

 			// tell the driver to use this surface for overlays
			OlFgSetVisibleSurface(CurDevInfo.DevId, CurrentSurface);
		
	   		// Get an HDC to the surface from the driver for erase bitmap
			Status = OlFgGetSurfaceDC(CurDevInfo.DevId, OldSurface, &Oldhdc);

			//make sure everything went well
			if(Status != OLC_STS_NORMAL)
			{
				//Get the error message from the driver and display it
				OlImgGetStatusMessage(Status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
				MessageBox(hWnd, StatusMsg, "Animate Bitmap - GetSurfaceDC(2nd)", MB_ICONHAND);
				return FALSE;
			}

 			// Erase old bitmap
			StretchBlt(Oldhdc, i,j,75,75, hdcBlackBmp, 0, 0, 74, 74, SRCCOPY);
		
		//Clean up
			// release the surface's resources
			OlFgReleaseSurfaceDC(CurDevInfo.DevId, OldSurface, Oldhdc);
 			

			/* Increment to next  location check to make sure that it is on screen and 
			   decrement if off screen.*/
			i +=k;
			j +=l;
			if(i+Size > (INT)FrameWidth || i < 0)
			{
				k *=-1;
				i +=k;
			}
			if(j+Size > (INT)FrameHeight || j < 0)
			{
				l *=-1;
				j +=l;
			}

			// debug to track code
			OutputDebugString("End while\n");
		
		
		}
		// debug to track code
		OutputDebugString("Exit thread\n");

		// temporary beep to signify thread termination
		Beep(3000,25);

		// terminate thread
		ExitThread(0);

		//clean up bitmap objects
		DeleteObject(hBmp);
		DeleteDC(hdcBmp);
		DeleteObject(hBlackBmp);
		DeleteDC(hdcBlackBmp);

	} __finally{MessageBox(hWnd, "Try - finally", "Update Animation", MB_ICONHAND);}


	return TRUE; 
}

