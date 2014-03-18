/*
        File:   Inp_filt.c

        Description:
                This file contains all the necessary functions for the Input
                Filter dialog box.  The following functions are included in
                this file:

                InputFilterProc() - dialog proc for Setup InputFilter dialog

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

/*      Function:       InputFilterProc

        Description:
                This function is the dialog procedure for the dialog box used to
                select the input filter that the frame grabber will use.

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


BOOL CALLBACK __export InputFilterProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        static ULNG ulNewFilter = 0L;

    /* What's the message? */
        switch (message)
          {
           case WM_INITDIALOG:
             {
                ULNG ulFilters = 0L;
                ULNG ulCurFilter = 0L;


                /* Center the dialog box */

                CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));


                /* Enable the radio buttons for the supported filters */

                (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                                OLC_FG_IC_INPUT_FILTER_LIMITS,
                                                &ulFilters,
                                                sizeof(ULNG));

                EnableWindow(GetDlgItem(hDlg, IDC_RB_ACNONE), (BOOL)(ulFilters & OLC_FG_FILT_AC_NONE));
                EnableWindow(GetDlgItem(hDlg, IDC_RB_AC50), (BOOL)(ulFilters & OLC_FG_FILT_AC_50));
                EnableWindow(GetDlgItem(hDlg, IDC_RB_AC60), (BOOL)(ulFilters & OLC_FG_FILT_AC_60));
                EnableWindow(GetDlgItem(hDlg, IDC_RB_DCNONE), (BOOL)(ulFilters & OLC_FG_FILT_DC_NONE));


                /* Get the current filter */

                (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_INPUT_FILTER,
                                                &ulCurFilter);


                /* Initialize the radio buttons */

                switch (ulCurFilter)
                  {
                   case OLC_FG_FILT_AC_50:
                        CheckRadioButton(hDlg, IDC_RB_ACNONE, IDC_RB_DCNONE, IDC_RB_AC50);
                        ulNewFilter = OLC_FG_FILT_AC_50;
                        break;

                   case OLC_FG_FILT_AC_60:
                        CheckRadioButton(hDlg, IDC_RB_ACNONE, IDC_RB_DCNONE, IDC_RB_AC60);
                        ulNewFilter = OLC_FG_FILT_AC_60;
                        break;

                   case OLC_FG_FILT_DC_NONE:
                        CheckRadioButton(hDlg, IDC_RB_ACNONE, IDC_RB_DCNONE, IDC_RB_DCNONE);
                        ulNewFilter = OLC_FG_FILT_DC_NONE;
                        break;

                   case OLC_FG_FILT_AC_NONE:
                        CheckRadioButton(hDlg, IDC_RB_ACNONE, IDC_RB_DCNONE, IDC_RB_ACNONE);
                        ulNewFilter = OLC_FG_FILT_AC_NONE;
                        break;

                   default:

                        /* Current filter not supported by this application */

                        ulNewFilter = 0L;
                        break;
                  }
                return (TRUE);
             }
           break;


           /* message: received a command */

           case WM_COMMAND:
                switch (wParam)
                  {
                   case IDC_RB_ACNONE:
                        ulNewFilter = OLC_FG_FILT_AC_NONE;
                        return TRUE;

                   case IDC_RB_AC50:
                        ulNewFilter = OLC_FG_FILT_AC_50;
                        return TRUE;

                   case IDC_RB_AC60:
                        ulNewFilter = OLC_FG_FILT_AC_60;
                        return TRUE;

                   case IDC_RB_DCNONE:
                        ulNewFilter = OLC_FG_FILT_DC_NONE;
                        return TRUE;

                   case IDOK:

                        /* Set the desired filter */

                        {
                         if ( ulNewFilter )
                           {
                               ULNG ulOldData;
                               OLT_APISTATUS Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                                                        CurDevInfo.InputSource,
                                                                        OLC_FG_CTL_INPUT_FILTER,
                                                                        ulNewFilter,
                                                                        &ulOldData);
                               if ( ! OlImgIsOkay(Status) )
                                  PrintStatus(hDlg, Status, "Unable to set input filter", "Input Filter Error");

                               /* If we successfully set the filter return TRUE */

                               EndDialog(hDlg, (!Status ? TRUE : FALSE));
                               return (TRUE);
                           }
                        }

                        /* If no legal filter was selected, fall through to Cancel */

                   case IDCANCEL:
                        EndDialog(hDlg, FALSE);
                        return (TRUE);
                  }
                break;

          }  /* end of switch(message) */


    /* Didn't process the message */

        return (FALSE);
}

