/*
    File:   Utils.C
    
    Description:
        This fils contains a group of simple, handy functions that can be used
        in any module.  These functions are not to illustrate any Open Layers
        concepts; they are strictly general purpose subroutines.  
        The following functions are included in this file:
          
        About() - processes messages for "About" dialog box
        CenterWindow() - handy function to center one window
                with respect to any other window
        GetDlgItemLong() - simple function to retrieve a long
                integer value from a dialog box control
        PrintStatus() - a function to print out a specific status 
                message in a MessageBox format
        ResetControlFocus() -   a function that resets focus to an edit
                control in a dialog box.
    Comments:

*/

/*=========================================================================++
||                                     ||
||  Copyright (C) 1996.   Data Translation, Inc., 100 Locke Drive,     ||
||  Marlborough Massachusetts 01752-1192.                  ||
||                                     ||
||  The information in this document is subject to change without      ||
||  notice and should not be construed as a commitment by Data     ||
||  Translation, Inc.  Data Translation, Inc. does not assume any      ||
||  responsibility for any errors that may appear in this document.    ||
||                                     ||
||  This document, or source code, is provided solely as an example    ||
||  of how to utilize Data Translation software libraries in a sample  ||
||  application.  Data Translation, Inc. does not assume any       ||
||  responsibility for the use or reliability of any portion of this   ||
||  document or the described software.                ||
||                                     ||
||  General permission to copy or modify, but not for profit, is       ||
||  hereby granted,  provided that the above copyright notice is       ||
||  included and reference made to the fact that reproduction      ||
||  privileges were granted by Data Translation, Inc.          ||
||                                     ||
||  Data Translation cannot assume any responsibility for the use,     ||
||  or misuse, of any portion of this software for other than its      ||
||  intended diagnostic purpose in calibrating and testing Data    ||
||  Translation manufactured image processing boards and software.     ||
||                                     ||
++=========================================================================*/


/* Let's use strict type checking in the example */
#define STRICT

#include "passthru.h"            /* specific to this program */

/**************************************************************************/
/**************************************************************************/

/*
    Function:   About

    Description:
        This function is the dialog procedure for the About dialog.

    Input Arguments:
        HWND hDlg       dialog handle
        UINT message        message
        WPARAM wParam       16-bit message-specific data
        LPARAM lParam       32-bit message-specific data

    Output Arguments:
        None

    Returns:  BOOL
        Except in response to WM_INITDIALOG, TRUE is returned if the
        dialog proc processes the message, FALSE if it does not.  For
        WM_INITDIALOG, FALSE is returned if the dialog proc calls
        SetFocus to set the input focus to a specific control.  If
        not, TRUE should be returned to indicate that the input focus
        should be given to the first control in the dialog that can
        receive focus.
*/


BOOL CALLBACK __export About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    /* What's the message? */
    switch (message)
      {
       /* message: initialize dialog box */
       case WM_INITDIALOG:
        {
         HWND hOwner = GetWindow(hDlg, GW_OWNER);
         CenterWindow(hDlg, hOwner);
         return (TRUE);
        }
        break;


       /* message: received a command */
       case WM_COMMAND:
        switch (LOWORD(wParam))
          {
           case IDCANCEL:
           case IDOK:
            EndDialog(hDlg, TRUE);
            return (TRUE);
            break;
          }
        break;

      }  /* end of switch(message) */


    /* Didn't process the message */
    return (FALSE);
}


/********************************************************************/
/********************************************************************/

/*  Function:   CenterWindow

    Description:
        This handy function centers window 1 in window 2.  If hOwner
        is NULL, the window is centered on the screen.


    Input Arguments:
        HWND hWnd           handle of window to center
        HWND hOwner         handle of owner/parent over which window
                           is centered

    Output Arguments:
        None

    Return Value:
        None
*/

void PRIVATE CenterWindow(HWND hWnd, HWND hOwner)
{
    RECT rcScreen = {0};
    RECT rcParent = {0};
    int xMid;
    int yMid;
    RECT rcDlg = {0};
    int xLeft;                               
    int yTop;
    LPRECT lprcParent = NULL;


    // Current screent size
    rcScreen.left = 0;
    rcScreen.top = 0;
    rcScreen.right = GetSystemMetrics(SM_CXSCREEN);
    rcScreen.bottom = GetSystemMetrics(SM_CYSCREEN);

    // lprcParent points to the rectange we should center ourself in
    if (hOwner == NULL)
       lprcParent = &rcScreen;
    else
      {
       GetWindowRect(hOwner, &rcParent);
       lprcParent = &rcParent;
      }

    // find ideal center point
    xMid = (lprcParent->left + lprcParent->right) / 2;
    yMid = (lprcParent->top + lprcParent->bottom) / 2;

    // find dialog's upper left based on that
    GetWindowRect(hWnd, &rcDlg);
    xLeft = xMid - ((rcDlg.right - rcDlg.left) / 2);
    yTop = yMid - ((rcDlg.bottom - rcDlg.top) / 2);

    // if the dialog is outside the screen, move it inside
    if (xLeft < 0)
       xLeft = 0;
    else
       if (xLeft + (rcDlg.right - rcDlg.left) > rcScreen.right)
          xLeft = rcScreen.right - (rcDlg.right - rcDlg.left);

    if (yTop < 0)
       yTop = 0;
    else
       if (yTop + (rcDlg.bottom - rcDlg.top) > rcScreen.bottom)
          yTop = rcScreen.bottom - (rcDlg.bottom - rcDlg.top);

    SetWindowPos(hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
}


//=====================================================================================
//=====================================================================================

/*  Function:   GetDlgItemLong            

    Description:
        This function reads the text from the given control and converts
        it to a long value.


    Input Arguments:
        HWND hDlg           handle of dialog
        int idControl           id of control to read

    Output Arguments:
        LPBOOL lpbWorked        set to TRUE if everything worked

    Return Value (long):
        long integer value read from control
*/

LNG PRIVATE GetDlgItemLong(HWND hDlg, int idControl, LPBOOL lpbWorked)
{
    CHR szText[20];
    NPCHR pChr = szText;
    LNG lValue = 0L;                
    BOOL bNonZero = FALSE;


    // Internal function, better get the parameters right!
    assert(hDlg != NULL);
    assert(lpbWorked != NULL);

    // Initialize the return flag
    *lpbWorked = FALSE;
    
    // Get the string
    if ( !GetDlgItemText(hDlg, idControl, szText, 20) )
       return 0;


    // Check the string for format (  [+|-][0-9]... )

    // ignore spaces and tabs
    while ( *pChr )
      {
       if ( ((*pChr == ' ')  ||  (*pChr == '\t')) )
          pChr++;
       else
          break;
      }

    // Did we reach the end?
    if ( ! *pChr )
       return 0;

    // Ignore a single plus or minus sign
    if ( *pChr == '+'  ||  *pChr == '-' )
       pChr++;

    // Did we reach end now?
    if ( ! *pChr )
       return 0;

    // Everything else should be a number if format is okay
    while ( *pChr )
      {
       if ( !isdigit(*pChr) )
          break;
       if ( *pChr != '0' )
          bNonZero = TRUE;
       pChr++;
      }

    // Did we find a bogus character?
    if ( *pChr  &&  !isdigit(*pChr) )
       return 0;


    // Looks okay, call atol to translate into long
    lValue = atol(szText);
    if ( !(bNonZero &&  (lValue == 0L)) )
       *lpbWorked = TRUE;
    
    return lValue;
}

//=====================================================================================
//=====================================================================================

/*  Function:   Print Status

    Description:
        This function converts a status value to a string and 
        displays it in a Message Box along with a message describing
        the operation attempted.

    Input Arguments:
        HWND hDlg           handle of dialog
        OLT_APISTATUS status        status value returned from an OL function
        LPSTR lpszOpMsg         more specific message describing the operation
                        atttempted
        LPSTR lpszMBTitile      title of the message box 

    Output Arguments: void
        None
    Return Value: void
        None
*/
void PRIVATE PrintStatus (HWND hDlg, OLT_APISTATUS status, LPSTR lpszOpMsg, LPSTR lpszMBTitle)
{ 
  CHR msg[OLC_MAX_STATUS_MESSAGE_SIZE + 60];
  CHR StatusMsg[OLC_MAX_STATUS_MESSAGE_SIZE];

  (void) OlImgGetStatusMessage(status, StatusMsg, OLC_MAX_STATUS_MESSAGE_SIZE);
  wsprintf(msg, "%s  Status message:\n\n%s", lpszOpMsg, (LPSTR) StatusMsg);
  MessageBox(hDlg, msg, lpszMBTitle, MB_ICONSTOP | MB_OK);
}





//=====================================================================================
//=====================================================================================

/*  Function:   ResetControlFocus

    Description:
        This function sets focus to an edit control and sets the value
        displayed in the control to lValue.

    Input Arguments:
        HWND hDlg           handle of dialog
        int idControl           id of control to read
                long lValue         value to display in control
                                
    Output Arguments:
        void : None 
    Return Value :
        None
*/
void PRIVATE ResetControlFocus (HWND hDlg, int idControl, long lValue)
{
  HWND hControl = GetDlgItem(hDlg, idControl);
  CHR msg[80];                           
  
  SetFocus(hControl);
  wsprintf(msg, "%ld", lValue);
  Edit_SetText(hControl, (LPSTR)msg);
  Edit_SetSel(hControl, 0, lstrlen(msg)); 
}

 