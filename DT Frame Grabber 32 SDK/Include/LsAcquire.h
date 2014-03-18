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


#ifndef _LS_ACQUIRE_H
#define _LS_ACQUIRE_H

// Structure declaration
typedef struct tagPointerList
       {
         HGLOBAL hPal;
         HPALETTE hGrayscalePal;
         LPLOGPALETTE lpPal;
         HGLOBAL hDIBdata;
         HPUCHR hpDIBdata;
         HGLOBAL hDIBheader;
         BITMAPINFO * lpDIBheader;
        } POINTERLIST, FAR *LPPOINTERLIST;

typedef struct	tagWindowInfo
		{
			HANDLE	hUserData;
			USHORT	Width;
			USHORT	Height;
		}WINDOWINFO;

typedef char STRING[255];		// Standard array of char for a lpstr

// User defined window messages
#define WMUSER_INIT (WM_USER + 1000)
#define WMUSER_CHAN_CHANGE (WM_USER + 1100)
#define WMUSER_CALLBACK (WM_USER + 1200)


// Function prototype neede in the application
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DisplayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DioDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AcqScalingDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PassScalingDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HWND ShowBasicWindow (WORD X1, WORD Y1, WORD X2, WORD Y2);
void SaveAcquiredImage(HWND hWnd, HWND hWndDisplay);
void PRIVATE CleanupDIBPointers(LPPOINTERLIST);
LRESULT PRIVATE CreateDIBDisplay(HWND, HPVOID, DWORD, DWORD, USHRT, USHRT, USHRT);

#endif //_LS_ACQUIRE_H
