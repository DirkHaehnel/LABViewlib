/*
        File:   Pixelclk.c

        Description:
                This file includes all the necessary funcitons for the Input
                Clock Dialog box.  The following functions are included in this
                file:

                PixelClockProc() -      dialog proc for Setup PixelClode dialog

                InitPixelClockDlg() -   performs initialization of GDI controls in
                                        PixelClock dialog

                SetPixelClockInfo() -   sets the frame grabber's pixel clock
                                        controls as indicated by the user

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

/*      Function:       PixelClockProc

        Description:
                This function is the dialog procedure for the dialog box used to
                set up a frame grabber's pixel clock.  The pixel clock determines
                how the incoming video data is sampled.

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

BOOL CALLBACK __export PixelClockProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        static BOOL bInternal = FALSE;
        static BOOL bExtClockOnLowToHigh = FALSE;
        static ULNG ulCurrentFreq = 0L;

    /* What's the message? */

        switch (message)
          {
           case WM_INITDIALOG:
                {
                    /* Center the dialog box */

                    CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));

                    /* Initialize the edit controls and check box */

                    InitPixelClockDlg(hDlg, &bInternal, &bExtClockOnLowToHigh, &ulCurrentFreq);

                    return (TRUE);
                }
                break;


           /* message: received a command */

           case WM_COMMAND:
                switch (wParam)
                  {
                   case IDC_CK_LO_TO_HIGH:
                        bExtClockOnLowToHigh = !bExtClockOnLowToHigh;
                        return TRUE;
                        break;

                   case IDC_RB_INTERNAL:
                        bInternal = TRUE;
                        EnableWindow(GetDlgItem(hDlg, IDC_ED_FREQ), bInternal);
                        EnableWindow(GetDlgItem(hDlg, IDC_CK_LO_TO_HIGH), !bInternal);
                        return TRUE;
                        break;

                   case IDC_RB_EXTERNAL:
                        bInternal = FALSE;
                        EnableWindow(GetDlgItem(hDlg, IDC_ED_FREQ), bInternal);
                        EnableWindow(GetDlgItem(hDlg, IDC_CK_LO_TO_HIGH), !bInternal);
                        return TRUE;
                        break;


                   case IDOK:

                        /* Set the Pixel Clock controls based on user's input */

                        SetPixelClockInfo(hDlg, bInternal, bExtClockOnLowToHigh, &ulCurrentFreq);
                        return (TRUE);
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


/**************************************************************************/
/**************************************************************************/

/*      Function:       InitPixelClockDlg

        Description:
                This function initializes the controls in the Pixel Clock
                dialog based upon the frame grabber's capabilities and current
                state.


        Input Arguments:
                HWND hDlg                               handle of owning dialog

        Output Arguments:
                LPBOOL lpInternal                       set to TRUE if internal clock is in use;
                                                           FALSE otherwise
                LPBOOL lpExtClockOnLowToHigh            set to TRUE if external clock is in use
                                                        and a Low-To-High transition on the
                                                        external clock line is considered a
                                                        "clock"; FALSE otherwise

                LPULNG lpCurFreq                        static to hold current frequency in case
                                                        it needs to reset

        Return Value (void):
                None.
*/

void PRIVATE InitPixelClockDlg(HWND hDlg, LPBOOL lpInternal, LPBOOL lpExtClockOnLowToHigh, LPULNG lpCurFreq)
{
        ULNG ulData = 0L;
        CHR  szText[20];
        HWND hFreq = GetDlgItem(hDlg, IDC_ED_FREQ);


    /* Check to see what input clock capabilities the board supports */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_CLOCK_SOURCE_LIMITS,
                                        &ulData,
                                        sizeof (ULNG));

    /* supports internal pixel clock */

        if (ulData & (ULNG) OLC_FG_CLOCK_INTERNAL)
          {
              ULNG ulTempData;

              /* Enable radio button since its disabled on start up */

              EnableWindow(GetDlgItem(hDlg, IDC_RB_INTERNAL), TRUE);


              /* Get the current frequency */

              (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_CLOCK_FREQ,
                                                &ulTempData);
              wsprintf(szText, "%ld", ulTempData);
              Edit_SetText(hFreq, (LPSTR) szText);

              /* save current frequency in a static variable  */

              *lpCurFreq = ulTempData;
          }


    /* supports external pixel clock */

        if (ulData & (ULNG) OLC_FG_CLOCK_EXTERNAL)
          {
              ULNG ulTempData;

              /* Enable radio button since its disabled on start up */

              EnableWindow(GetDlgItem(hDlg, IDC_RB_EXTERNAL), TRUE);

              /* Get the current flag settings */

              (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_CLOCK_FLAGS,
                                                &ulTempData);

              /* save flag in a static boolean */

              if ( ulTempData & OLC_FG_CLOCK_EXT_ON_LO_TO_HI )
                *lpExtClockOnLowToHigh = TRUE;
              else
                *lpExtClockOnLowToHigh = FALSE;

              CheckDlgButton(hDlg, IDC_CK_LO_TO_HIGH, *lpExtClockOnLowToHigh);
          }


    /* Internal or External clock? */

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_CLOCK_SOURCE,
                                        &ulData);

        if (ulData & (ULNG) OLC_FG_CLOCK_INTERNAL)
          {

              /* Now, enable/disable the appropriate controls for an internal clock */

              EnableWindow(GetDlgItem(hDlg, IDC_ED_FREQ), TRUE);
              EnableWindow(GetDlgItem(hDlg, IDC_CK_LO_TO_HIGH), FALSE);
              CheckRadioButton(hDlg, IDC_RB_INTERNAL, IDC_RB_EXTERNAL, IDC_RB_INTERNAL);
              *lpInternal = TRUE;
          }

        else
          {
              /* Now, enable/disable the appropriate controls for an external clock */

              EnableWindow(GetDlgItem(hDlg, IDC_CK_LO_TO_HIGH), TRUE);
              EnableWindow(GetDlgItem(hDlg, IDC_ED_FREQ), FALSE);
              CheckRadioButton(hDlg, IDC_RB_INTERNAL, IDC_RB_EXTERNAL, IDC_RB_EXTERNAL);
              *lpInternal = FALSE;
          }

        return;
}


/**************************************************************************/
/**************************************************************************/

/*      Function:       SetPixelClockInfo

        Description:
                This function sets the frame grabber according to the user's
                dialog selections.


        Input Arguments:
                HWND hDlg                               handle of owning dialog
                BOOL bInternal                          TRUE if user chose Internal timing
                BOOL bExtClockOnLowToHigh               TRUE if user chose External timing and
                                                           wants a low to high transition on
                                                           the external clock line to indicate
                                                           a "clock"
                LPULNG lpCurFreq                        static storing the current frequency in
                                                        case it needs to be reset

        Output Arguments:
                None

        Return Value (void):
                None.
*/

void PRIVATE SetPixelClockInfo(HWND hDlg, BOOL bInternal, BOOL bExtClockOnLowToHigh, LPULNG lpCurFreq)
{
        ULNG ulOldData = 0L;
        OLT_APISTATUS Status;


    /* Set the pixel clock source */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_CLOCK_SOURCE,
                                        (ULNG) (bInternal ? OLC_FG_CLOCK_INTERNAL : OLC_FG_CLOCK_EXTERNAL),
                                        &ulOldData);

         if ( !OlImgIsOkay(Status) )
           {
               PrintStatus(hDlg, Status, "Unable to set clock source.", "Pixel Clock Error");
               EndDialog(hDlg, FALSE);
               return;
           }


    /* Internal clock? */

        if (bInternal)
          {
              BOOL bWorked = FALSE;
              ULNG ulFreq = 0L;
              LONG lFreq = 0L;
              OLT_NL_LNG_RANGE Range = {0};

           /* Yup, query for internal's clock valid range */

           Status = OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_CLOCK_FREQ_LIMITS,
                                        &Range,
                                        sizeof(OLT_NL_LNG_RANGE));

           if ( !OlImgIsOkay(Status) )
            {
               PrintStatus(hDlg, Status,"Unable to get Internal Clock Range.", "Internal Clock Error");
               EndDialog(hDlg,FALSE);
               return;
            }

           /*Got the range, now read the edit control */

           lFreq = GetDlgItemLong(hDlg, IDC_ED_FREQ, &bWorked);
           if ( !bWorked )
             {
                char msg[OLC_MAX_STATUS_MESSAGE_SIZE + 60];
                wsprintf(msg, "Invalid value entered for Internal clock frequency.");
                MessageBox(hDlg, msg, "Pixel Clock Error", MB_ICONSTOP | MB_OK);
                ResetControlFocus(hDlg, IDC_ED_FREQ, *lpCurFreq);
                return;
             }

           if (lFreq < Range.Min || lFreq > Range.Max)
             {

                /* Out of range ... */

                char msg[80];
                wsprintf(msg, "Please enter a frequency value between %ld and %ld Hz.",
                                Range.Min, Range.Max);
                MessageBox(hDlg, msg, "Internal Clock Range Error", MB_ICONINFORMATION | MB_OK);

                /* Set focus back to the edit control and display previous value */

                ResetControlFocus(hDlg, IDC_ED_FREQ, *lpCurFreq);
                return;
             }


           /* Set the frequency */

           ulFreq = (ULNG) lFreq;
           Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_CLOCK_FREQ,
                                        ulFreq,
                                        &ulOldData);

            if ( !OlImgIsOkay(Status) )
              {
                 PrintStatus(hDlg, Status,"Unable to Set Internal Clock.", "Internal Clock Error");
                 EndDialog(hDlg,FALSE);
                 return;
              }
          }


        /* Nope, get the current external clock flags */

        else
          {
              ULNG ulFlags = 0L;

              /* Set, or clear flag */

              if ( bExtClockOnLowToHigh )
                 ulFlags |= OLC_FG_CLOCK_EXT_ON_LO_TO_HI;
              else
                 ulFlags &= ~OLC_FG_CLOCK_EXT_ON_LO_TO_HI;

              /* Set the new value */

              Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_CLOCK_FLAGS,
                                        ulFlags,
                                        &ulOldData);

              if ( !OlImgIsOkay(Status) )
                {
                   PrintStatus(hDlg, Status,"Unable to set External Clock Flags.", "External Clock Error");
                   EndDialog(hDlg,FALSE);
                   return;
                }
          }

        EndDialog(hDlg, TRUE);
        return;
}


