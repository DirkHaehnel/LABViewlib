/*
        File:   Framesiz.c

        Description:
                This file contains all the necessary functions for the
                Frame Size dialog box.  The following functions are found
                in this file:

                FrameSizeProc() -
                        dialog proc for Setup Frame Size dialog

                InitFrameSizeDlg() -
                        performs initialization of GDI controls in
                        Frame SizeVideoArea dialog

                IncrementFrameControl() -
                        updates edit controls in response to user
                        actions

                DecrementFrameControl() -
                        updates edit controls in response to user
                        actions

                MinMaxFrameControl() -
                        updates edit controls in response to user
                        actions

                SetFrameSizeInfo() -
                        sets the frame grabber's frame size
                        controls as indicated by the user

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

/*      Function:       FrameSizeProc

        Description:
                This function is the dialog procedure for the dialog box used to
                define the "frame size" for the frame grabber.

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

BOOL CALLBACK __export FrameSizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        static FRAMESIZEINFO Info = {0};


    /* What's the message? */

        switch (message)
          {
           case WM_INITDIALOG:
                {
                 /* Center the dialog box */
                 CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));

                 /* Initialize the dialog box controls */
                 InitFrameSizeDlg(hDlg, &Info);

                 return (TRUE);
                }


           /* message: received a command */

           case WM_COMMAND:
                switch (wParam)
                  {
                   case IDOK:

                        /* Set the frame size controls per the user */

                        SetFrameSizeInfo(hDlg, &Info);
                        return (TRUE);

                   case IDCANCEL:
                        EndDialog(hDlg, FALSE);
                        return (TRUE);
                  }
                break;


           /* message: someone hit a vertical scrollbar */

           case WM_VSCROLL:
                {
                 HWND hControl = (HWND) lParam;
                 WPARAM wScrollCode = wParam;

                 switch (wScrollCode)
                   {
                    case SB_PAGEUP:
                    case SB_LINEUP:

                        /* Increment the appropriate control */

                        IncrementFrameControl(hDlg, hControl, &Info);
                        return (TRUE);
                        break;


                    case SB_PAGEDOWN:
                    case SB_LINEDOWN:

                        /* Decrement the appropriate control */

                        DecrementFrameControl(hDlg, hControl, &Info);
                        return (TRUE);
                        break;


                    case SB_TOP:

                        /* Maximize the appropriate control */

                        MinMaxFrameControl(hDlg, hControl, &Info, TRUE);
                        return (TRUE);
                        break;


                    case SB_BOTTOM:

                        /* Minimize the appropriate control */

                        MinMaxFrameControl(hDlg, hControl, &Info, FALSE);
                        return (TRUE);
                        break;
                  }
                }
                break;

          }  /* end of switch(message) */


    /* Didn't process the message */

        return (FALSE);
}


/**************************************************************************/
/**************************************************************************/

/*      Function:       InitFrameSizeDlg

        Description:
                This function initializes the controls in the Frame Size
                dialog based upon the frame grabber's capabilities and current
                state.


        Input Arguments:
                HWND hDlg                       handle of owning dialog

        Output Arguments:
                LPFRAMESIZEINFO lpInfo          filled in with control ranges and current
                                                   settings.

        Return Value (void):
                None.
*/

void PRIVATE InitFrameSizeDlg(HWND hDlg, LPFRAMESIZEINFO lpInfo)
{
        char szText[20];


    /* Verify the pointer */
        assert(lpInfo != NULL);


    /* Get the limits and current value of the FrameLeft control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_FRAME_LEFT_LIMITS,
                                        &(lpInfo->LeftRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FRAME_LEFT,
                                        &(lpInfo->Left));

        wsprintf(szText, "%ld", lpInfo->Left);

        Edit_SetText(GetDlgItem(hDlg, IDC_ED_LEFT), (LPSTR) szText);

    /* Get the limits and current value of the FrameTop control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_FRAME_TOP_LIMITS,
                                        &(lpInfo->TopRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FRAME_TOP,
                                        &(lpInfo->Top));

        wsprintf(szText, "%ld", lpInfo->Top);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOP), (LPSTR) szText);


    /* Get the limits and current value of the FrameWidth control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_FRAME_WIDTH_LIMITS,
                                        &(lpInfo->WidthRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FRAME_WIDTH,
                                        &(lpInfo->Width));

        wsprintf(szText, "%ld", lpInfo->Width);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_WIDTH), (LPSTR) szText);


    /* Get the limits and current value of the FrameHeight control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_FRAME_HEIGHT_LIMITS,
                                        &(lpInfo->HeightRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FRAME_HEIGHT,
                                        &(lpInfo->Height));

        wsprintf(szText, "%ld", lpInfo->Height);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_HEIGHT), (LPSTR) szText);


    /* Get the limits and current value of the HorizontalIncrement control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_FRAME_HINC_LIMITS,
                                        &(lpInfo->HorizontalIncRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_HOR_FRAME_INC,
                                        &(lpInfo->HorizontalInc));

        wsprintf(szText, "%ld", lpInfo->HorizontalInc);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_HINC), (LPSTR) szText);


    /* Get the limits and current value of the Vertical Increment control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_FRAME_VINC_LIMITS,
                                        &(lpInfo->VerticalIncRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_VER_FRAME_INC,
                                        &(lpInfo->VerticalInc));

        wsprintf(szText, "%ld", lpInfo->VerticalInc);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_VINC), (LPSTR) szText);


        return;
}
/**************************************************************************/
/**************************************************************************/

/*      Function:       IncrementFrameControl

        Description:
                This function increments the value for the specified
                control and writes the updated value to the buddy edit
                control.


        Input Arguments:
                HWND hDlg                       handle of owning dialog
                HWND hControl                   handle of control to imcrement
                LPFRAMESIZEINFO lpInfo          contains current settings and range limits

        Output Arguments:
                LPFRAMESIZEINFO lpInfo          filled in with requested setting.

        Return Value (void):
                None.
*/

void PRIVATE IncrementFrameControl(HWND hDlg, HWND hControl, LPFRAMESIZEINFO lpInfo)
{
        char szText[20];
        int id = GetDlgCtrlID(hControl);
        BOOL bWorked = TRUE;
        LPCHR lpszErrorTitle = "Frame Size Error";
        LNG lResult = 0L;

    /* Verify the pointer */
        assert(lpInfo != NULL);

    /* Which control? */

        switch (id)
          {
           case IDC_SB_LEFT:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_LEFT,
                                        &(lpInfo->LeftRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_LEFT, lpInfo->Left);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->Left = lResult + lpInfo->LeftRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->Left);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_LEFT), (LPSTR) szText);
                break;


           case IDC_SB_TOP:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_TOP,
                                        &(lpInfo->TopRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_TOP, lpInfo->Top);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->Top = lResult + lpInfo->TopRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->Top);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOP), (LPSTR) szText);
                break;


           case IDC_SB_WIDTH:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_WIDTH,
                                        &(lpInfo->WidthRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_WIDTH, lpInfo->Width);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->Width = lResult + lpInfo->WidthRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->Width);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_WIDTH), (LPSTR) szText);
                break;


           case IDC_SB_HEIGHT:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_HEIGHT,
                                        &(lpInfo->HeightRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_HEIGHT, lpInfo->Height);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->Height = lResult + lpInfo->HeightRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->Height);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_HEIGHT), (LPSTR) szText);
                break;


           case IDC_SB_HINC:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_HINC,
                                        &(lpInfo->HorizontalIncRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_HINC, lpInfo->HorizontalInc);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->HorizontalInc = lResult + lpInfo->HorizontalIncRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->HorizontalInc);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_HINC), (LPSTR) szText);
                break;


           case IDC_SB_VINC:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_VINC,
                                        &(lpInfo->VerticalIncRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_VINC, lpInfo->VerticalInc);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->VerticalInc = lResult + lpInfo->VerticalIncRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->VerticalInc);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_VINC), (LPSTR) szText);
                break;


           default:
                /* should never happen since these are the only scroll bars */
                assert(FALSE);
                break;
          }

        return;
}


/**************************************************************************/
/**************************************************************************/

/*      Function:       DecrementFrameControl

        Description:
                This function decrements the value for the specified
                control and writes the updated value to the buddy edit
                control.


        Input Arguments:
                HWND hDlg                       handle of owning dialog
                HWND hControl                   handle of control to imcrement
                LPFRAMESIZEINFO lpInfo          contains current settings and range limits

        Output Arguments:
                LPFRAMESIZEINFO lpInfo          filled in with requested setting.

        Return Value (void):
                None.
*/

void PRIVATE DecrementFrameControl(HWND hDlg, HWND hControl, LPFRAMESIZEINFO lpInfo)
{
        char szText[20];
        BOOL bWorked = TRUE;
        LPCHR lpszErrorTitle = "Frame Size Error";
        LNG lResult = 0L;
        int id = GetDlgCtrlID(hControl);


    /* Verify the pointer */
        assert(lpInfo != NULL);

    /* Which control? */
        switch (id)
          {
           case IDC_SB_LEFT:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_LEFT,
                                        &(lpInfo->LeftRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_LEFT, lpInfo->Left);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->Left = lResult - lpInfo->LeftRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->Left);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_LEFT), (LPSTR) szText);
                break;


           case IDC_SB_TOP:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_TOP,
                                        &(lpInfo->TopRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_TOP, lpInfo->Top);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->Top = lResult - lpInfo->TopRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->Top);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOP), (LPSTR) szText);
                break;


           case IDC_SB_WIDTH:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_WIDTH,
                                        &(lpInfo->WidthRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_WIDTH, lpInfo->Width);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->Width = lResult - lpInfo->WidthRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->Width);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_WIDTH), (LPSTR) szText);
                break;


           case IDC_SB_HEIGHT:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_HEIGHT,
                                        &(lpInfo->HeightRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_HEIGHT, lpInfo->Height);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->Height = lResult - lpInfo->HeightRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->Height);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_HEIGHT), (LPSTR) szText);
                break;


           case IDC_SB_HINC:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_HINC,
                                        &(lpInfo->HorizontalIncRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_HINC, lpInfo->HorizontalInc);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->HorizontalInc = lResult - lpInfo->HorizontalIncRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->HorizontalInc);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_HINC), (LPSTR) szText);
                break;


           case IDC_SB_VINC:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_VINC,
                                        &(lpInfo->VerticalIncRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_VINC, lpInfo->VerticalInc);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->VerticalInc = lResult - lpInfo->VerticalIncRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->VerticalInc);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_VINC), (LPSTR) szText);
                break;


           default:
                /* should never happen since these are the only scroll bars */
                assert(FALSE);
                break;
          }

        return;
}
/**************************************************************************/
/**************************************************************************/

/*      Function:       MinMaxFrameControl

        Description:
                This function either maximizes or minimizes the value for
                the specified control and writes the updated value to the
                buddy edit control.


        Input Arguments:
                HWND hDlg                       handle of owning dialog
                HWND hControl                   handle of control to imcrement
                LPFRAMESIZEINFO lpInfo          contains current settings and range limits
                BOOL bMaximize                  TRUE to maximize, FALSE to minimize

        Output Arguments:
                LPFRAMESIZEINFO lpInfo          filled in with requested setting.

        Return Value (void):
                None.
*/

void PRIVATE MinMaxFrameControl(HWND hDlg, HWND hControl, LPFRAMESIZEINFO lpInfo, BOOL bMaximize)
{
        char szText[20];
        int id = GetDlgCtrlID(hControl);


    /* Verify the pointer */
        assert(lpInfo != NULL);

    /* Which control? */
        switch (id)
          {
           case IDC_SB_LEFT:
                lpInfo->Left = bMaximize ? lpInfo->LeftRange.Max : lpInfo->LeftRange.Min;
                wsprintf(szText, "%ld", lpInfo->Left);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_LEFT), (LPSTR) szText);
                break;


           case IDC_SB_TOP:
                lpInfo->Top = bMaximize ? lpInfo->TopRange.Max : lpInfo->TopRange.Min;
                wsprintf(szText, "%ld", lpInfo->Top);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOP), (LPSTR) szText);
                break;


           case IDC_SB_WIDTH:
                lpInfo->Width = bMaximize ? lpInfo->WidthRange.Max : lpInfo->WidthRange.Min;
                wsprintf(szText, "%ld", lpInfo->Width);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_WIDTH), (LPSTR) szText);
                break;


           case IDC_SB_HEIGHT:
                lpInfo->Height = bMaximize ? lpInfo->HeightRange.Max : lpInfo->HeightRange.Min;
                wsprintf(szText, "%ld", lpInfo->Height);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_HEIGHT), (LPSTR) szText);
                break;


           case IDC_SB_HINC:
                lpInfo->HorizontalInc = bMaximize ? lpInfo->HorizontalIncRange.Max : lpInfo->HorizontalIncRange.Min;
                wsprintf(szText, "%ld", lpInfo->HorizontalInc);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_HINC), (LPSTR) szText);
                break;


           case IDC_SB_VINC:
                lpInfo->VerticalInc = bMaximize ? lpInfo->VerticalIncRange.Max : lpInfo->VerticalIncRange.Min;
                wsprintf(szText, "%ld", lpInfo->VerticalInc);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_VINC), (LPSTR) szText);
                break;


           default:
                /* should never happen since these are the only scroll bars */
                assert(FALSE);
                break;
          }

        return;
}


/**************************************************************************/
/**************************************************************************/

/*      Function:       SetFrameSizeInfo

        Description:
                This function sets the frame grabber input controls to
                reflect the user's dialog settings.


        Input Arguments:
                HWND hDlg                       handle of owning dialog
                LPFRAMESIZEINFO lpInfo          filled in with user's choices

        Output Arguments:
                None.

        Return Value (void):
                None.
*/

void PRIVATE SetFrameSizeInfo(HWND hDlg, LPFRAMESIZEINFO lpInfo)
{
        OLT_APISTATUS Status = 0L;
        ULNG ulOldData = 0L;
        BOOL bWorked = TRUE;
        LPCHR lpszErrorTitle = "Frame Size Error";
        LNG lResult = 0L;

    /* Verify the pointer */
        assert(lpInfo != NULL);

    /* Get and Set the current value of the Frame Left control */

        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_LEFT,
                                &(lpInfo->LeftRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_LEFT, lpInfo->Left);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FRAME_LEFT,
                                        lResult,
                                        &ulOldData);
        if ( !OlImgIsOkay(Status) )
          {
            PrintStatus(hDlg, Status,"Unable to set Frame Left.", "Frame Size Error");
            EndDialog(hDlg,FALSE);
              return;
          }


    /* Get and Set the current value of the Frame Top control */
        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_TOP,
                                &(lpInfo->TopRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_TOP, lpInfo->Top);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FRAME_TOP,
                                        lResult,
                                        &ulOldData);

        if ( !OlImgIsOkay(Status) )
          {
             PrintStatus(hDlg, Status,"Unable to set Frame Top.", lpszErrorTitle);
             EndDialog(hDlg,FALSE);
             return;
          }


    /* Get and Set the current value of the Frame Width control */

        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_WIDTH,
                                &(lpInfo->WidthRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_WIDTH, lpInfo->Width);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FRAME_WIDTH,
                                        lResult,
                                        &ulOldData);

        if ( !OlImgIsOkay(Status) )
          {
             PrintStatus(hDlg, Status,"Unable to set Frame Width.", lpszErrorTitle);
             EndDialog(hDlg,FALSE);
             return;
          }


    /* Get and Set the current value of the Frame Height control */
        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_HEIGHT,
                                &(lpInfo->HeightRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_HEIGHT, lpInfo->Height);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FRAME_HEIGHT,
                                        lResult,
                                        &ulOldData);
        if ( !OlImgIsOkay(Status) )
          {
             PrintStatus(hDlg, Status,"Unable to set Frame Height.", lpszErrorTitle);
             EndDialog(hDlg,FALSE);
             return;
          }



    /* Get and Set the current value of the Horizontal Increment control */
        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_HINC,
                                &(lpInfo->HorizontalIncRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_HINC, lpInfo->HorizontalInc);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_HOR_FRAME_INC,
                                        lResult,
                                        &ulOldData);
        if ( !OlImgIsOkay(Status) )
          {
             PrintStatus(hDlg, Status,"Unable to set Horizontal Increment.", lpszErrorTitle);
             EndDialog(hDlg,FALSE);
             return;
          }


    /* Get and Set the current value of the Vertical Increment control */
        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_VINC,
                                &(lpInfo->VerticalIncRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_VINC, lpInfo->VerticalInc);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_VER_FRAME_INC,
                                        lResult,
                                        &ulOldData);

        if ( !OlImgIsOkay(Status) )
          {
             PrintStatus(hDlg, Status,"Unable to set Vertical Increment.", lpszErrorTitle);
             EndDialog(hDlg,FALSE);
             return;
          }

        EndDialog(hDlg, TRUE);
        return;
}
