/*
        File:   A2D.C
        Description:
                This file contains all the necessary functions for the
                Setup A/D dialog.  This dialog box allows the user to
                set the white and black level reference points on the board.

                The following functions are included in this file:

                A2DProc()       the dialog box procedure
                SetA2DInfo      Reads the edit controls from the dialog box
                                and writes the data to the board.

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

/*      Function:       A2DProc

        Description:
                This function is the dialog procedure for the dialog box used to
                set up the frame grabber's A/D controls.

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

BOOL CALLBACK __export A2DProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        static LNG lBlackLevel = 0L;
        static LNG lWhiteLevel = 0L;

    /* What's the message? */
        switch (message)
          {
           case WM_INITDIALOG:
                {
                 HWND hEditBlack = GetDlgItem(hDlg, IDC_ED_BLACK);
                 HWND hEditWhite = GetDlgItem(hDlg, IDC_ED_WHITE);
                 char str[20];

                 /* Center the dialog box */
                 CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));

                 /* Get the current values and place them in the edit controls */
                 (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                        CurDevInfo.InputSource,
                                                        OLC_FG_CTL_BLACK_LEVEL,
                                                        (LPULNG)&lBlackLevel);
                 wsprintf(str, "%ld", lBlackLevel);
                 Edit_SetText(hEditBlack, (LPSTR)str);

                 (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                        CurDevInfo.InputSource,
                                                        OLC_FG_CTL_WHITE_LEVEL,
                                                        (LPULNG)&lWhiteLevel);
                 wsprintf(str, "%ld", lWhiteLevel);
                 Edit_SetText(hEditWhite, (LPSTR)str);

                 return (TRUE);
                }


                /* message: received a command */
                case WM_COMMAND:
                switch (wParam)
                  {
                        case IDOK:
                        /* Set the requested input source */
                        SetA2DInfo(hDlg, &lBlackLevel, &lWhiteLevel);
                        return(TRUE);

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

/*      Function:       SetA2DInfo

        Description:
                This function reads the edit controls and writes the values
                to the board.

        Initialization:
                None.

        Input Arguments:
                HWND hDlg               dialog handle
                LPLNG lpBlackLevel      pointer to a static to save black
                                        level data.
                LPLNG lpWhiteLevel      pointer to a static to save white
                                        level data.

        Output Arguments:
                LPLNG lpBlackLevel      pointer to static that receives new
                                        black level data.
                LPLNG lpWhiteLevel      pointer to static that receives new
                                        white level data.


        Returns:
                None.
*/
void PRIVATE SetA2DInfo(HWND hDlg, LPLNG lpBlackLevel, LPLNG lpWhiteLevel)
{
        LNG lNewBlack = 0L;
        LNG lNewWhite = 0L;
        OLT_LNG_RANGE Range = {0};
        OLT_APISTATUS Status = 0L;
        BOOL bBlackWorked = FALSE, bWhiteWorked = FALSE;
        LNG lOldData = 0L;


        /* check valid black level ranges */

        Status = OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_BLACK_LEVEL_LIMITS,
                                        &Range,
                                        sizeof(OLT_LNG_RANGE));
        if ( !OlImgIsOkay(Status) )
         {
           PrintStatus(hDlg, Status,"Unable to get black level range.", "Black Level Range Error");
           EndDialog(hDlg,FALSE);
           return;
         }


         /* Get the new black level */

        lNewBlack = GetDlgItemLong(hDlg, IDC_ED_BLACK, &bBlackWorked);
        if (!bBlackWorked)
          {
            char msg[80];
            wsprintf(msg, "Invalid value retrieved from black level from edit control.");
            MessageBox(hDlg, msg, "Black Level Error", MB_ICONSTOP | MB_OK);

            /* Set focus back to the edit control and display previous value */
            ResetControlFocus(hDlg, IDC_ED_BLACK, *lpBlackLevel);
            return;

          }

         /* Verify that the current black level is within the legal range */
        if ( lNewBlack <  Range.Min  ||  lNewBlack >  Range.Max )
          {
            /* Out of range ... */
            char msg[80];
            wsprintf(msg, "Please enter a black level value between %ld and %ld microVolts.",
                                Range.Min, Range.Max);
            MessageBox(hDlg, msg, "Black Level Range", MB_ICONINFORMATION | MB_OK);

            /* Set focus back to the edit control and display previous value */
            ResetControlFocus(hDlg, IDC_ED_BLACK, *lpBlackLevel);
            return;
           }


        /* check valid white level ranges */

        Status = OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_WHITE_LEVEL_LIMITS,
                                        &Range,
                                        sizeof(OLT_NL_LNG_RANGE));
        if ( !OlImgIsOkay(Status) )
         {
           PrintStatus(hDlg, Status,"Unable to get white level range.", "White Level Range Error");
           EndDialog(hDlg,FALSE);
           return;
          }


         /* Get the new white level */

        lNewWhite = GetDlgItemLong(hDlg, IDC_ED_WHITE, &bWhiteWorked);
        if (!bWhiteWorked)
          {
            char msg[80];
            wsprintf(msg, "Invalid value retrieved from white level from edit control.");
            MessageBox(hDlg, msg, "White Level Error", MB_ICONSTOP | MB_OK);

            /* set focus back to the edit control and display previous value */
            ResetControlFocus(hDlg, IDC_ED_WHITE, *lpWhiteLevel);
            return;
          }


         /* Verify that the current white level is within the legal range */

        if ( lNewWhite <  Range.Min  ||  lNewWhite >  Range.Max )
          {
            /* Out of range ... */
            char msg[80];
            wsprintf(msg, "Please enter a white level value between %ld and %ld microVolts.",
                        Range.Min, Range.Max);
            MessageBox(hDlg, msg, "White Level Range", MB_ICONINFORMATION | MB_OK);

            /* Set focus back to the edit control and dislay previous value */
            ResetControlFocus(hDlg, IDC_ED_WHITE, *lpWhiteLevel);
            return;
          }

        /* At this point both white and black level are valid values and within range. */
        /* Set 'em! */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_BLACK_LEVEL,
                                                lNewBlack,
                                                (LPULNG)&lOldData);
        if ( !OlImgIsOkay(Status) )
          {
            PrintStatus(hDlg, Status, "Unable to set black level.", "Black Level Error");
            bBlackWorked = FALSE;
          }
          else
            *lpBlackLevel = lNewBlack;

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_WHITE_LEVEL,
                                                lNewWhite,
                                                (LPULNG)&lOldData);
        if ( !OlImgIsOkay(Status) )
         {
           PrintStatus(hDlg, Status,"Unable to set white level.", "White Level Error");
           bWhiteWorked = FALSE;
         }
         else
           *lpWhiteLevel = lNewWhite;


         /* If we successfully set the A/D levels return TRUE */

        EndDialog(hDlg, bBlackWorked & bWhiteWorked);
        return;
}

