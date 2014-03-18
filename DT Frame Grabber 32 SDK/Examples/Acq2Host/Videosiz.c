/*
        File:   Videosiz.C

        Description:
                This file contains all of the necessary functions for the
                Setup VideoArea dialog box.  The following functions are
                included in this file:

                VideoAreaProc() -
                        dialog proc for Setup VideoArea dialog

                InitVideoAreaDlg() -
                        performs initialization of GDI controls in
                        VideoArea dialog

                IncrementVideoControl() -
                        updates edit controls in response to user actions

                DecrementVideoControl() -
                        updates edit controls in response to user actions

                MinMaxVideoControl() -
                        updates edit controls in response to user actions

                SetVideoAreaInfo() -
                        sets the frame grabber's video area controls as
                        indicated by the user

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

/*      Function:       VideoAreaProc

        Description:
                This function is the dialog procedure for the dialog box used to
                define the "active video area" for the frame grabber.

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

BOOL CALLBACK __export VideoAreaProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        static VIDEOAREAINFO Info = {0};


    /* What's the message? */

        switch (message)
          {
           case WM_INITDIALOG:
                {
                 /* Center the dialog box */

                 CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));

                 /* Initialize the dialog box controls and static info */

                 InitVideoAreaDlg(hDlg, &Info);

                 return (TRUE);
                }

           /* message: received a command */

           case WM_COMMAND:
                switch (wParam)
                  {
                   case IDOK:

                        /* Set the video area controls per the user */

                        SetVideoAreaInfo(hDlg, &Info);
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
                 WORD wScrollCode = (WORD)wParam;

                 switch (wScrollCode)
                        {
                         case SB_PAGEUP:
                         case SB_LINEUP:

                        /* Increment the appropriate control */

                        IncrementVideoControl(hDlg, hControl, &Info);
                        return (TRUE);

                    case SB_PAGEDOWN:
                    case SB_LINEDOWN:

                        /* Decrement the appropriate control */

                        DecrementVideoControl(hDlg, hControl, &Info);
                        return (TRUE);

                    case SB_TOP:

                        /* Maximize the appropriate control */

                        MinMaxVideoControl(hDlg, hControl, &Info, TRUE);
                        return (TRUE);

                         case SB_BOTTOM:

                        /* Minimize the appropriate control */

                        MinMaxVideoControl(hDlg, hControl, &Info, FALSE);
                        return (TRUE);
                  }
                }
                break;

          }  /* end of switch(message) */


    /* Didn't process the message */

        return (FALSE);
}


/**************************************************************************/
/**************************************************************************/

/*      Function:       InitVideoAreaDlg

        Description:
                This function initializes the controls in the Video Area
                dialog based upon the frame grabber's capabilities and current
                state.


        Input Arguments:
                HWND hDlg                       handle of owning dialog

        Output Arguments:
                LPVIDEOAREAINFO lpInfo          filled in with control ranges and current
                                                   settings.

        Return Value (void):
                None.
*/

void PRIVATE InitVideoAreaDlg(HWND hDlg, LPVIDEOAREAINFO lpInfo)
{
        char szText[20];
	  	STRING	DeviceName = {0};


    /* Verify the pointer */
        assert(lpInfo != NULL);

    /* Get the limits and current value of the TotalPixelsPerLine control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_TOTAL_PIX_PER_LINE_LIMITS,
                                        &(lpInfo->PixelsPerLineRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_TOTAL_PIX_PER_LINE,
                                        &(lpInfo->PixelsPerLine));

        wsprintf(szText, "%ld", lpInfo->PixelsPerLine);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOTAL_PIX), (LPSTR) szText);

	 //if a 3157, disable the following unsupported controls
		 OlImgQueryDeviceCaps(CurDevInfo.DevId,OLC_IMG_DC_DEVICE_NAME,DeviceName,sizeof(STRING));
		 if ((strcmp("DT3157",DeviceName)) == 0)
		 {
			 EnableWindow(GetDlgItem(hDlg, IDC_ED_BACK_PORCH), FALSE);
			 EnableWindow(GetDlgItem(hDlg, IDC_SB_BACK_PORCH), FALSE);
			 EnableWindow(GetDlgItem(hDlg, IDC_ED_CLAMP_START), FALSE);
			 EnableWindow(GetDlgItem(hDlg, IDC_SB_CLAMP_START), FALSE);
			 EnableWindow(GetDlgItem(hDlg, IDC_ED_CLAMP_END), FALSE);
			 EnableWindow(GetDlgItem(hDlg, IDC_SB_CLAMP_END), FALSE);
		 }

    /* Get the limits and current value of the BackPorchStart control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_BACK_PORCH_START_LIMITS,
                                        &(lpInfo->BackPorchStartRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_BACK_PORCH_START,
                                        &(lpInfo->BackPorchStart));

        wsprintf(szText, "%ld", lpInfo->BackPorchStart);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_BACK_PORCH), (LPSTR) szText);


    /* Get the limits and current value of the ClampStart control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_CLAMP_START_LIMITS,
                                        &(lpInfo->ClampStartRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_CLAMP_START,
                                        &(lpInfo->ClampStart));

        wsprintf(szText, "%ld", lpInfo->ClampStart);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_CLAMP_START), (LPSTR) szText);


    /* Get the limits and current value of the ClampEnd control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_CLAMP_END_LIMITS,
                                        &(lpInfo->ClampEndRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_CLAMP_END,
                                        &(lpInfo->ClampEnd));

        wsprintf(szText, "%ld", lpInfo->ClampEnd);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_CLAMP_END), (LPSTR) szText);


    /* Get the limits and current value of the First Active Pixel control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_ACTIVE_PIXEL_LIMITS,
                                        &(lpInfo->FirstPixelRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FIRST_ACTIVE_PIXEL,
                                        &(lpInfo->FirstPixel));

        wsprintf(szText, "%ld", lpInfo->FirstPixel);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_FIRST_PIX), (LPSTR) szText);


    /* Get the limits and current value of the Active Pixel Count control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_ACTIVE_WIDTH_LIMITS,
                                        &(lpInfo->ActivePixelsRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_ACTIVE_PIXEL_COUNT,
                                        &(lpInfo->ActivePixels));

        wsprintf(szText, "%ld", lpInfo->ActivePixels);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_PIX_COUNT), (LPSTR) szText);


    /* Get the limits and current value of the TotalLinesPerField control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_TOTAL_LINES_PER_FLD_LIMITS,
                                        &(lpInfo->LinesPerFieldRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_TOTAL_LINES_PER_FLD,
                                        &(lpInfo->LinesPerField));

        wsprintf(szText, "%ld", lpInfo->LinesPerField);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOTAL_LINES), (LPSTR) szText);


    /* Get the limits and current value of the FirstActiveLine control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_ACTIVE_LINE_LIMITS,
                                        &(lpInfo->FirstLineRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FIRST_ACTIVE_LINE,
                                        &(lpInfo->FirstLine));

        wsprintf(szText, "%ld", lpInfo->FirstLine);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_FIRST_LINE), (LPSTR) szText);


    /* Get the limits and current value of the ActiveLineCount control */

        (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                                        OLC_FG_IC_ACTIVE_HEIGHT_LIMITS,
                                        &(lpInfo->ActiveLinesRange),
                                        sizeof(OLT_LNG_RANGE));

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_ACTIVE_LINE_COUNT,
                                        &(lpInfo->ActiveLines));

        wsprintf(szText, "%ld", lpInfo->ActiveLines);
        Edit_SetText(GetDlgItem(hDlg, IDC_ED_LINE_COUNT), (LPSTR) szText);

        return;
}

/**************************************************************************/
/**************************************************************************/

/*      Function:       IncrementVideoControl

        Description:
                This function reads the edit control and checks the value returned
                for range requirements, invalid characters and for granularity.
                If these requirements are met, the function increments the value
                for the specified control and writes the updated value to the buddy edit
                control.


        Input Arguments:
                HWND hDlg                       handle of owning dialog
                HWND hControl                   handle of control to imcrement
                LPVIDEOAREAINFO lpInfo          contains current settings and range limits

        Output Arguments:
                LPVIDEOAREAINFO lpInfo          filled in with requested setting.

        Return Value (void):
                None.
*/

void PRIVATE IncrementVideoControl(HWND hDlg, HWND hControl, LPVIDEOAREAINFO lpInfo)
{
        char szText[20];
        int id = GetDlgCtrlID(hControl);
        BOOL bWorked = TRUE;
        LPCHR lpszErrorTitle = "Video Area Error";
        LNG lResult = 0L;

    /* Verify the pointer */
        assert(lpInfo != NULL);

    /* Which control? */
        switch (id)
          {
           case IDC_SB_TOTAL_PIX:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_TOTAL_PIX,
                                        &(lpInfo->PixelsPerLineRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_TOTAL_PIX, lpInfo->PixelsPerLine);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->PixelsPerLine = lResult + lpInfo->PixelsPerLineRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->PixelsPerLine);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOTAL_PIX), (LPSTR) szText);
                break;


           case IDC_SB_BACK_PORCH:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_BACK_PORCH,
                                        &(lpInfo->BackPorchStartRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_BACK_PORCH, lpInfo->BackPorchStart);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->BackPorchStart = lResult + lpInfo->BackPorchStartRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->BackPorchStart);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_BACK_PORCH), (LPSTR) szText);
                break;


           case IDC_SB_CLAMP_START:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_CLAMP_START,
                                        &(lpInfo->ClampStartRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_CLAMP_START, lpInfo->ClampStart);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->ClampStart = lResult + lpInfo->ClampStartRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->ClampStart);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_CLAMP_START), (LPSTR) szText);
                break;


           case IDC_SB_CLAMP_END:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_CLAMP_END,
                                        &(lpInfo->ClampEndRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_CLAMP_END, lpInfo->ClampEnd);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->ClampEnd = lResult + lpInfo->ClampEndRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->ClampEnd);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_CLAMP_END), (LPSTR) szText);
                break;


           case IDC_SB_FIRST_PIX:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_FIRST_PIX,
                                        &(lpInfo->FirstPixelRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_FIRST_PIX, lpInfo->FirstPixel);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->FirstPixel = lResult + lpInfo->FirstPixelRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->FirstPixel);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_FIRST_PIX), (LPSTR) szText);
                break;


           case IDC_SB_PIX_COUNT:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_PIX_COUNT,
                                        &(lpInfo->ActivePixelsRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_PIX_COUNT, lpInfo->ActivePixels);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->ActivePixels = lResult + lpInfo->ActivePixelsRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->ActivePixels);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_PIX_COUNT), (LPSTR) szText);
                break;


           case IDC_SB_TOTAL_LINES:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_TOTAL_LINES,
                                        &(lpInfo->LinesPerFieldRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_TOTAL_LINES, lpInfo->LinesPerField);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->LinesPerField = lResult + lpInfo->LinesPerFieldRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->LinesPerField);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOTAL_LINES), (LPSTR) szText);
                break;


           case IDC_SB_FIRST_LINE:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_FIRST_LINE,
                                        &(lpInfo->FirstLineRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_FIRST_LINE, lpInfo->FirstLine);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->FirstLine = lResult + lpInfo->FirstLineRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->FirstLine);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_FIRST_LINE), (LPSTR) szText);
                break;


           case IDC_SB_LINE_COUNT:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_LINE_COUNT,
                                        &(lpInfo->ActiveLinesRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        INCREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_LINE_COUNT, lpInfo->ActiveLines);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then increment */

                lpInfo->ActiveLines = lResult + lpInfo->ActiveLinesRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->ActiveLines);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_LINE_COUNT), (LPSTR) szText);
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

/*      Function:       DecrementVideoControl

        Description:
                This function decrements the value for the specified
                control and writes the updated value to the buddy edit
                control.


        Input Arguments:
                HWND hDlg                       handle of owning dialog
                HWND hControl                   handle of control to imcrement
                LPVIDEOAREAINFO lpInfo          contains current settings and range limits

        Output Arguments:
                LPVIDEOAREAINFO lpInfo          filled in with requested setting.

        Return Value (void):
                None.
*/

void PRIVATE DecrementVideoControl(HWND hDlg, HWND hControl, LPVIDEOAREAINFO lpInfo)
{
        char szText[20];
        int id = GetDlgCtrlID(hControl);
        BOOL bWorked = TRUE;
        LPCHR lpszErrorTitle = "Video Area Error";
        LNG lResult = 0L;


    /* Verify the pointer */
        assert(lpInfo != NULL);

    /* Which control? */
        switch (id)
          {
           case IDC_SB_TOTAL_PIX:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_TOTAL_PIX,
                                        &(lpInfo->PixelsPerLineRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_TOTAL_PIX, lpInfo->PixelsPerLine);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->PixelsPerLine = lResult - lpInfo->PixelsPerLineRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->PixelsPerLine);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOTAL_PIX), (LPSTR) szText);
                break;


           case IDC_SB_BACK_PORCH:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_BACK_PORCH,
                                        &(lpInfo->BackPorchStartRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_BACK_PORCH, lpInfo->BackPorchStart);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->BackPorchStart = lResult - lpInfo->BackPorchStartRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->BackPorchStart);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_BACK_PORCH), (LPSTR) szText);
                break;


           case IDC_SB_CLAMP_START:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_CLAMP_START,
                                        &(lpInfo->ClampStartRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_CLAMP_START, lpInfo->ClampStart);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->ClampStart = lResult - lpInfo->ClampStartRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->ClampStart);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_CLAMP_START), (LPSTR) szText);
                break;


           case IDC_SB_CLAMP_END:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_CLAMP_END,
                                        &(lpInfo->ClampEndRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_CLAMP_END, lpInfo->ClampEnd);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->ClampEnd = lResult - lpInfo->ClampEndRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->ClampEnd);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_CLAMP_END), (LPSTR) szText);
                break;


           case IDC_SB_FIRST_PIX:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_FIRST_PIX,
                                        &(lpInfo->FirstPixelRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_FIRST_PIX, lpInfo->FirstPixel);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->FirstPixel = lResult - lpInfo->FirstPixelRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->FirstPixel);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_FIRST_PIX), (LPSTR) szText);
                break;


           case IDC_SB_PIX_COUNT:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_PIX_COUNT,
                                        &(lpInfo->ActivePixelsRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_PIX_COUNT, lpInfo->ActivePixels);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->ActivePixels = lResult;
                lpInfo->ActivePixels -= lpInfo->ActivePixelsRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->ActivePixels);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_PIX_COUNT), (LPSTR) szText);
                break;


           case IDC_SB_TOTAL_LINES:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_TOTAL_LINES,
                                        &(lpInfo->LinesPerFieldRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_TOTAL_LINES, lpInfo->LinesPerField);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->LinesPerField = lResult - lpInfo->LinesPerFieldRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->LinesPerField);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOTAL_LINES), (LPSTR) szText);
                break;


           case IDC_SB_FIRST_LINE:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_FIRST_LINE,
                                        &(lpInfo->FirstLineRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_FIRST_LINE, lpInfo->FirstLine);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->FirstLine = lResult - lpInfo->FirstLineRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->FirstLine);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_FIRST_LINE), (LPSTR) szText);
                break;


           case IDC_SB_LINE_COUNT:

                /* Check current value in Edit Control */

                bWorked = RdAndChkEditCtlVal(hDlg,
                                        IDC_ED_LINE_COUNT,
                                        &(lpInfo->ActiveLinesRange),
                                        lpszErrorTitle,
                                        &lResult,
                                        DECREMENT);

                if (!bWorked)
                  {
                      ResetControlFocus(hDlg, IDC_ED_LINE_COUNT, lpInfo->ActiveLines);
                      MessageBeep((UINT)-1);
                      return;
                  }

                /* Set lpInfo with current value and then decrement */

                lpInfo->ActiveLines = lResult - lpInfo->ActiveLinesRange.Granularity;
                wsprintf(szText, "%ld", lpInfo->ActiveLines);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_LINE_COUNT), (LPSTR) szText);
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

/*      Function:       MinMaxVideoControl

        Description:
                This function either maximizes or minimizes the value for
                the specified control and writes the updated value to the
                buddy edit control.


        Input Arguments:
                HWND hDlg                       handle of owning dialog
                HWND hControl                   handle of control to imcrement
                LPVIDEOAREAINFO lpInfo          contains current settings and range limits
                BOOL bMaximize                  TRUE to maximize, FALSE to minimize

        Output Arguments:
                LPVIDEOAREAINFO lpInfo          filled in with requested setting.

        Return Value (void):
                None.
*/

void PRIVATE MinMaxVideoControl(HWND hDlg, HWND hControl, LPVIDEOAREAINFO lpInfo, BOOL bMaximize)
{
        char szText[20];
        int id = GetDlgCtrlID(hControl);

    /* Verify the pointer */
        assert(lpInfo != NULL);

    /* Which control? */
        switch (id)
          {
           case IDC_SB_TOTAL_PIX:
                lpInfo->PixelsPerLine = bMaximize ? lpInfo->PixelsPerLineRange.Max : lpInfo->PixelsPerLineRange.Min;
                wsprintf(szText, "%ld", lpInfo->PixelsPerLine);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOTAL_PIX), (LPSTR) szText);
                break;


           case IDC_SB_BACK_PORCH:
                lpInfo->BackPorchStart = bMaximize ? lpInfo->BackPorchStartRange.Max : lpInfo->BackPorchStartRange.Min;
                wsprintf(szText, "%ld", lpInfo->BackPorchStart);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_BACK_PORCH), (LPSTR) szText);
                break;


           case IDC_SB_CLAMP_START:
                lpInfo->ClampStart = bMaximize ? lpInfo->ClampStartRange.Max : lpInfo->ClampStartRange.Min;
                wsprintf(szText, "%ld", lpInfo->ClampStart);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_CLAMP_START), (LPSTR) szText);
                break;


           case IDC_SB_CLAMP_END:
                lpInfo->ClampEnd = bMaximize ? lpInfo->ClampEndRange.Max : lpInfo->ClampEndRange.Min;
                wsprintf(szText, "%ld", lpInfo->ClampEnd);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_CLAMP_END), (LPSTR) szText);
                break;


           case IDC_SB_FIRST_PIX:
                lpInfo->FirstPixel = bMaximize ? lpInfo->FirstPixelRange.Max : lpInfo->FirstPixelRange.Min;
                wsprintf(szText, "%ld", lpInfo->FirstPixel);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_FIRST_PIX), (LPSTR) szText);
                break;


           case IDC_SB_PIX_COUNT:
                lpInfo->ActivePixels = bMaximize ? lpInfo->ActivePixelsRange.Max : lpInfo->ActivePixelsRange.Min;
                wsprintf(szText, "%ld", lpInfo->ActivePixels);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_PIX_COUNT), (LPSTR) szText);
                break;


           case IDC_SB_TOTAL_LINES:
                lpInfo->LinesPerField = bMaximize ? lpInfo->LinesPerFieldRange.Max : lpInfo->LinesPerFieldRange.Min;
                wsprintf(szText, "%ld", lpInfo->LinesPerField);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_TOTAL_LINES), (LPSTR) szText);
                break;


           case IDC_SB_FIRST_LINE:
                lpInfo->FirstLine = bMaximize ? lpInfo->FirstLineRange.Max : lpInfo->FirstLineRange.Min;
                wsprintf(szText, "%ld", lpInfo->FirstLine);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_FIRST_LINE), (LPSTR) szText);
                break;


           case IDC_SB_LINE_COUNT:
                lpInfo->ActiveLines = bMaximize ? lpInfo->ActiveLinesRange.Max : lpInfo->ActiveLinesRange.Min;
                wsprintf(szText, "%ld", lpInfo->ActiveLines);
                Edit_SetText(GetDlgItem(hDlg, IDC_ED_LINE_COUNT), (LPSTR) szText);
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

/*      Function:       SetVideoAreaInfo

        Description:
                This function sets the frame grabber input controls to
                reflect the user's dialog settings.


        Input Arguments:
                HWND hDlg                       handle of owning dialog
                LPVIDEOAREAINFO lpInfo          filled in with user's choices

        Output Arguments:
                None.

        Return Value (void):
                None.
*/

void PRIVATE SetVideoAreaInfo(HWND hDlg, LPVIDEOAREAINFO lpInfo)
{
        OLT_APISTATUS Status = 0L;
        ULNG ulOldData = 0L;
        BOOL bWorked = FALSE;
        LPCHR lpszErrorTitle = "Video Area Error";
        LNG lResult = 0L;
	  	STRING	DeviceName = {0};

    /* Verify the pointer */
        assert(lpInfo != NULL);


    /* Get and Set the current value of the TotalPixelsPerLine control */

        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_TOTAL_PIX,
                                &(lpInfo->PixelsPerLineRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_TOTAL_PIX, lpInfo->PixelsPerLine);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_TOTAL_PIX_PER_LINE,
                                        lResult,
                                        &ulOldData);
        if ( !OlImgIsOkay(Status) )
          {
              PrintStatus(hDlg, Status, "Unable to set Pixels Per Line.", lpszErrorTitle);
              EndDialog(hDlg, FALSE);
              return;
          }



	 //if a 3157, don't attempt to set the following unsupported controls
		 OlImgQueryDeviceCaps(CurDevInfo.DevId,OLC_IMG_DC_DEVICE_NAME,DeviceName,sizeof(STRING));
		 if ((strcmp("DT3157",DeviceName)) != 0)
		 {
		   /* Get and Set the current value of the BackPorchStart control */

			/* Check current value in Edit Control */
			bWorked = RdAndChkEditCtlVal(hDlg,
									IDC_ED_BACK_PORCH,
									&(lpInfo->BackPorchStartRange),
									lpszErrorTitle,
									&lResult,
									SET);

			if (!bWorked)
			  {
				  ResetControlFocus(hDlg, IDC_ED_BACK_PORCH, lpInfo->BackPorchStart);
				  return;
			  }

			/* Current value in edit control is valid and within range. Now set it */

			Status = OlFgSetInputControlValue(CurDevInfo.DevId,
											CurDevInfo.InputSource,
											OLC_FG_CTL_BACK_PORCH_START,
											lResult,
											&ulOldData);

			if ( !OlImgIsOkay(Status) )
			  {
				  PrintStatus(hDlg, Status, "Unable to set Back Porch.", lpszErrorTitle);
				  EndDialog(hDlg, FALSE);
				  return;
			  }

		/* Get and Set current value of the Clamp Start control */

			/* Check current value in Edit Control */
			bWorked = RdAndChkEditCtlVal(hDlg,
									IDC_ED_CLAMP_START,
									&(lpInfo->ClampStartRange),
									lpszErrorTitle,
									&lResult,
									SET);

			if (!bWorked)
			  {
				  ResetControlFocus(hDlg, IDC_ED_CLAMP_START, lpInfo->ClampStart);
				  return;
			  }

			/* Current value in edit control is valid and within range. Now set it */

			Status = OlFgSetInputControlValue(CurDevInfo.DevId,
											CurDevInfo.InputSource,
											OLC_FG_CTL_CLAMP_START,
											lResult,
											&ulOldData);

			if ( !OlImgIsOkay(Status) )
			  {
				  PrintStatus(hDlg, Status, "Unable to set ClampStart.", lpszErrorTitle);
				  EndDialog(hDlg, FALSE);
				  if (!(Status & (OLC_STATUS_IS_APISTATUS_MASK + OLC_STS_CONTROL)))
				  return;
			  }

		/* Get and Set the current value of the ClampEnd control */

			/* Check current value in Edit Control */
			bWorked = RdAndChkEditCtlVal(hDlg,
									IDC_ED_CLAMP_END,
									&(lpInfo->ClampEndRange),
									lpszErrorTitle,
									&lResult,
									SET);

			if (!bWorked)
			  {
				  ResetControlFocus(hDlg, IDC_ED_CLAMP_END, lpInfo->ClampEnd);
				  return;
			  }

			/* Current value in edit control is valid and within range. Now set it */

			Status = OlFgSetInputControlValue(CurDevInfo.DevId,
											CurDevInfo.InputSource,
											OLC_FG_CTL_CLAMP_END,
											lResult,
											&ulOldData);

			if ( !OlImgIsOkay(Status) )
			  {
				  PrintStatus(hDlg, Status, "Unable to set Clamp End.", lpszErrorTitle);
				  EndDialog(hDlg, FALSE);
				  if (!(Status & (OLC_STATUS_IS_APISTATUS_MASK + OLC_STS_CONTROL)))
				  return;
			  }
		}

    /* Get and Set the current value of the First Active Pixel control */

        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_FIRST_PIX,
                                &(lpInfo->FirstPixelRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_FIRST_PIX, lpInfo->FirstPixel);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FIRST_ACTIVE_PIXEL,
                                        lResult,
                                        &ulOldData);

        if ( !OlImgIsOkay(Status) )
          {
              PrintStatus(hDlg, Status, "Unable to set First Active Pixel.", lpszErrorTitle);
              EndDialog(hDlg, FALSE);
              return;
          }

    /* Get and Set the current value of the Active Pixel Count control */

        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_PIX_COUNT,
                                &(lpInfo->ActivePixelsRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_PIX_COUNT, lpInfo->ActivePixels);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_ACTIVE_PIXEL_COUNT,
                                        lResult,
                                        &ulOldData);

        if ( !OlImgIsOkay(Status) )
          {
              PrintStatus(hDlg, Status, "Unable to set Active Pixel Count.", lpszErrorTitle);
              EndDialog(hDlg, FALSE);
              return;
          }


    /* Get and Set the current value of the Total Lines Per Field control */

        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_TOTAL_LINES,
                                &(lpInfo->LinesPerFieldRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_TOTAL_LINES, lpInfo->LinesPerField);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_TOTAL_LINES_PER_FLD,
                                        lResult,
                                        &ulOldData);

        if ( !OlImgIsOkay(Status) )
          {
              PrintStatus(hDlg, Status, "Unable to set Video Area Error.", lpszErrorTitle);
              EndDialog(hDlg, FALSE);
              return;
          }

    /* Set the limits and current value of the First Active Line control */

        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_FIRST_LINE,
                                &(lpInfo->FirstLineRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_FIRST_LINE, lpInfo->FirstLine);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_FIRST_ACTIVE_LINE,
                                        lResult,
                                        &ulOldData);

        if ( !OlImgIsOkay(Status) )
          {
              PrintStatus(hDlg, Status, "Unable to set First Active Line.", lpszErrorTitle);
              EndDialog(hDlg, FALSE);
              return;
          }


    /* Get and Set the current value of the Active Line Count control */

        /* Check current value in Edit Control */
        bWorked = RdAndChkEditCtlVal(hDlg,
                                IDC_ED_LINE_COUNT,
                                &(lpInfo->ActiveLinesRange),
                                lpszErrorTitle,
                                &lResult,
                                SET);

        if (!bWorked)
          {
              ResetControlFocus(hDlg, IDC_ED_LINE_COUNT, lpInfo->ActiveLines);
              return;
          }

        /* Current value in edit control is valid and within range. Now set it */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                        CurDevInfo.InputSource,
                                        OLC_FG_CTL_ACTIVE_LINE_COUNT,
                                        lResult,
                                        &ulOldData);

        if ( !OlImgIsOkay(Status) )
          {
              PrintStatus(hDlg, Status, "Unable to set Back Porch.", lpszErrorTitle);
              EndDialog(hDlg, FALSE);
              return;
          }


        EndDialog(hDlg, TRUE);
        return;
}

