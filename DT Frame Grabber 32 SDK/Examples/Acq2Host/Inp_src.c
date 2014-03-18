/*
        File:   Inp_src.c

        Description:
                This file contains all the necessary functions for the Input Setup
                dialog box.  The following functions are included in this file:

                InputSourceProc() - dialog proc for Setup InputSource dialog

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

/*      Function:       InputSourceProc

        Description:
                This function is the dialog procedure for the dialog box used to
                select the input video source that the frame grabber will use.

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
*/

BOOL CALLBACK __export InputSourceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    /* What's the message? */
        switch (message)
          {
           case WM_INITDIALOG:
                {
                    ULNG ulSourceCount = 1L;
                    USHRT usCurSource = 0;
                    int i = 0;
                    HWND hComboBox = GetDlgItem(hDlg, IDC_CB_SOURCE);

                    /* Center the dialog box */

                    CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));

                    /* How many input sources does the device support? */

                    (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                                OLC_FG_IC_INPUT_SOURCE_COUNT,
                                                &ulSourceCount,
                                                sizeof(ULNG));


                    /* Fill in the combo box with the choices (0 to (count-1)) */

                    for (i = 0; i < (int) ulSourceCount; i++)
                      {
                         char szSource[5];
                         wsprintf(szSource, "%d", i);
                         ComboBox_AddString(hComboBox, (LPSTR)szSource);
                      }

                    /* Get the current source and select it in the combo box */
                    (void) OlFgQueryInputVideoSource(CurDevInfo.DevId, &usCurSource);
                    ComboBox_SetCurSel(hComboBox, usCurSource);

                    return (TRUE);
                }
                break;


           /* message: received a command */

           case WM_COMMAND:
                switch (wParam)
                  {
                   case IDOK:

                        /* Set the requested input source */

                        {
                            USHRT usOldSource;
                            USHRT usNewSource = (USHRT) ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_CB_SOURCE));
                            OLT_APISTATUS Status = OlFgSetInputVideoSource(CurDevInfo.DevId,
                                                                                usNewSource,
                                                                                &usOldSource);

                            if ( ! OlImgIsOkay(Status) )
                               PrintStatus(hDlg, Status, "Unable to set input source.", "Input Source Error");
                            else
                               CurDevInfo.InputSource = usNewSource;

                            /* If we successfully set the filter return TRUE */

                            EndDialog(hDlg, (!Status ? TRUE : FALSE));
                            return (TRUE);
                        }
                        break;


                   case IDCANCEL:
                        EndDialog(hDlg, FALSE);
                        return (TRUE);
                        break;
                  }
                break;

          }  /* end of switch(message) */


    /* Didn't process the message */

        return (FALSE);
}
