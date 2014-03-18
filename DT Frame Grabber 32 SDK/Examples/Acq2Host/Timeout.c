/*
        File:   Timeout.c

        Description:
                This file contains all the necessary functions for the Timeout
                Dialog Box procedure.

                TimeoutProc() - dialog proc for Timeout dialog

        Comments:

*/

/*=========================================================================++
||                                                                         ||
||      Copyright (C) 1996.   Data Translation, Inc., 100 Locke Drive,     ||
||      Marlborough Massachusetts 01752-1192.                              ||
||                                                                         ||
||      The information in this document is subject to change without      ||
||      notice and should not be construed as a commitment by Data         ||
||      Translation, Inc.  Data Translation, Inc. does not assume any      ||
||      responsibility for any errors that may appear in this document.    ||
||                                                                         ||
||      This document, or source code, is provided solely as an example    ||
||      of how to utilize Data Translation software libraries in a sample  ||
||      application.  Data Translation, Inc. does not assume any           ||
||      responsibility for the use or reliability of any portion of this   ||
||      document or the described software.                                ||
||                                                                         ||
||      General permission to copy or modify, but not for profit, is       ||
||      hereby granted,  provided that the above copyright notice is       ||
||      included and reference made to the fact that reproduction          ||
||      privileges were granted by Data Translation, Inc.                  ||
||                                                                         ||
||      Data Translation cannot assume any responsibility for the use,     ||
||      or misuse, of any portion of this software for other than its      ||
||      intended diagnostic purpose in calibrating and testing Data        ||
||      Translation manufactured image processing boards and software.     ||
||                                                                         ||
++=========================================================================*/


/* Let's use strict type checking in this program */
#define STRICT

#include "acq2hst.h"                    /* specific to the acq2hst example */


/**************************************************************************/
/**************************************************************************/

/*      Function:       TimeoutProc

        Description:
                This function is the dialog procedure for the dialog box used to
                set the amount of time the device driver will wait for an operation
                to complete before generating an error.

        Initialization:
                This dialog box should be invoked using DialogBox.

        Input Arguments:
                HWND hDlg               dialog handle
                UINT message            message
                WPARAM wParam           16-bit message-specific data
                LPARAM lParam           32-bit message-specific data

        Output Arguments:
                None

        EndDialog Return:  BOOL
                TRUE if everything went okay, FALSE otherwise.

        Returns:  BOOL
                Except in response to WM_INITDIALOG, TRUE is returned if the
                dialog proc processes the message, FALSE if it does not.  For
                WM_INITDIALOG, FALSE is returned if the dialog proc calls
                SetFocus to set the input focus to a specific control.  If
                not, TRUE should be returned to indicate that the input focus
                should be given to the first control in the dialog that can
                receive focus.

        Comments:
                The dialog box is initialized to be disabled the first time the
                it is invoked.  This assumption is based upon the fact that
                the driver's default state upon opening a device, is to disable
                timeouts.

*/

BOOL CALLBACK __export TimeoutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

   ULNG ulTimeout;
   OLT_APISTATUS Status;


   /* What's the message? */

        switch (message)
          {
           case WM_INITDIALOG:
                {

                    char szText[20];

                    /* get current timeout */

                    Status = OlImgQueryTimeoutPeriod(CurDevInfo.DevId,
                                                        &ulTimeout);

            if (Status)
                           {
                             PrintStatus(hDlg,
                                        Status,
                                        "Unable to get current timeout",
                                        "Timeout Error");

                             EndDialog(hDlg, FALSE);
                             break;
                    }



                    /* Center the dialog box */

                    CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));
                    wsprintf(szText,"%lu", ulTimeout);
                    Edit_SetText(GetDlgItem(hDlg, IDC_TIMEOUT_ED), szText);
                    return (TRUE);
                }
                break;


           /* message: received a command */

           case WM_COMMAND:
                switch (wParam)
                  {


                   case IDOK:
                    {
                        BOOL bWorked = TRUE;
                        LNG lResult = 0;
                        ULNG ulOldTimeout = 0L;

                        lResult = GetDlgItemLong(hDlg, IDC_TIMEOUT_ED, &bWorked);

                        if (!bWorked || (lResult < 0)) {

                                 /* Value entered was non numeric or less than 0 */

                                     char errormsg[100];
                                     wsprintf(errormsg, "%s","Invalid entry for Timeout. Please enter a numeric value greater > 0");
                                     MessageBox(hDlg, errormsg, "Invalid Entry", MB_ICONSTOP | MB_OK);
                                     ResetControlFocus(hDlg, IDC_TIMEOUT_ED, ulTimeout);
                                     break;
                        }



                        ulTimeout = (ULNG) lResult;


                        Status = OlImgSetTimeoutPeriod(CurDevInfo.DevId,
                                                        ulTimeout,
                                                        &ulOldTimeout);


                        if (Status)
                           {
                             PrintStatus(hDlg,
                                        Status,
                                        "Unable to set current timeout",
                                        "Timeout Error");

                             EndDialog(hDlg, FALSE);
                             break;
                        }


                        EndDialog(hDlg, (!Status ? TRUE : FALSE));
                        return (TRUE);
            }


                   case IDCANCEL:
                        EndDialog(hDlg, FALSE);
                        return (TRUE);
                        break;

                   default:
                        return FALSE;
                  }  /*   end switch on WM_COMMAND */
                return TRUE;

          }  /* end of switch(message) */


    /* Didn't process the message */

        return (FALSE);
}
