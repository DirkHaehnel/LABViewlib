/*
        File:   Frametyp.c

        Description:
                This file contains all the necessary functions for the
                Frame Type Setup dialog box.   The following functions
                are described in this file:

                        FrameTypeProc() - dialog proc for Setup FrameType dialog

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

/*      Function:       FrameTypeProc

        Description:
                This function is the dialog procedure for the dialog box used to
                select/define the type of frame being acquire by the frame grabber.
                This information is used by the frame grabber to determine how
                to interpret and digitize the incoming video signal.  For example,
                this information determines whether the final digitized image
                comes from non-interlaced or interlaced video, and, if interlaced,
                whether an entire frame is sampled or just a single field.

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

BOOL CALLBACK __export FrameTypeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        static ULNG ulFrameType = 0L;

    /* What's the message? */
        switch (message)
          {
           case WM_INITDIALOG:
                {
                 ULNG ulFrameTypes = 0L;
                 ULNG ulCurFrameType = 0L;

                 /* Center the dialog box */
                 CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));

                 /* Enable the radio buttons for the supported frame types */
                 (void) OlFgQueryInputCaps(CurDevInfo.DevId, OLC_FG_IC_FRAME_TYPE_LIMITS, &ulFrameTypes, sizeof(ULNG));
                 EnableWindow(GetDlgItem(hDlg, IDC_RB_IFRAME_EVEN), (BOOL)(ulFrameTypes & OLC_FG_FRM_IL_FRAME_EVEN));
                 EnableWindow(GetDlgItem(hDlg, IDC_RB_IFRAME_ODD), (BOOL)(ulFrameTypes & OLC_FG_FRM_IL_FRAME_ODD));
                 EnableWindow(GetDlgItem(hDlg, IDC_RB_IFRAME_NEXT), (BOOL)(ulFrameTypes & OLC_FG_FRM_IL_FRAME_NEXT));
                 EnableWindow(GetDlgItem(hDlg, IDC_RB_FIELD_EVEN), (BOOL)(ulFrameTypes & OLC_FG_FRM_FIELD_EVEN));
                 EnableWindow(GetDlgItem(hDlg, IDC_RB_FIELD_ODD), (BOOL)(ulFrameTypes & OLC_FG_FRM_FIELD_ODD));
                 EnableWindow(GetDlgItem(hDlg, IDC_RB_FIELD_NEXT), (BOOL)(ulFrameTypes & OLC_FG_FRM_FIELD_NEXT));
                 EnableWindow(GetDlgItem(hDlg, IDC_RB_NIFRAME), (BOOL)(ulFrameTypes & OLC_FG_FRM_NON_INTERLACED));

                 /* Get the current frame type */
                 (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                        CurDevInfo.InputSource,
                                                        OLC_FG_CTL_FRAME_TYPE,
                                                        &ulCurFrameType);

                  /* initialize static variable with current Frame type */

                  ulFrameType = ulCurFrameType;

                 /* Initialize the radio buttons */
                 switch ( (USHRT)ulCurFrameType )
                   {
                    case OLC_FG_FRM_IL_FRAME_EVEN:
                        CheckRadioButton(hDlg, IDC_RB_IFRAME_EVEN, IDC_RB_NIFRAME, IDC_RB_IFRAME_EVEN);
                        break;

                    case OLC_FG_FRM_IL_FRAME_ODD:
                        CheckRadioButton(hDlg, IDC_RB_IFRAME_EVEN, IDC_RB_NIFRAME, IDC_RB_IFRAME_ODD);
                        break;

                    case OLC_FG_FRM_IL_FRAME_NEXT:
                        CheckRadioButton(hDlg, IDC_RB_IFRAME_EVEN, IDC_RB_NIFRAME, IDC_RB_IFRAME_NEXT);
                        break;

                    case OLC_FG_FRM_FIELD_EVEN:
                        CheckRadioButton(hDlg, IDC_RB_IFRAME_EVEN, IDC_RB_NIFRAME, IDC_RB_FIELD_EVEN);
                        break;

                    case OLC_FG_FRM_FIELD_ODD:
                        CheckRadioButton(hDlg, IDC_RB_IFRAME_EVEN, IDC_RB_NIFRAME, IDC_RB_FIELD_EVEN);
                        break;

                    case OLC_FG_FRM_FIELD_NEXT:
                        CheckRadioButton(hDlg, IDC_RB_IFRAME_EVEN, IDC_RB_NIFRAME, IDC_RB_FIELD_EVEN);
                        break;

                    case OLC_FG_FRM_NON_INTERLACED:
                        CheckRadioButton(hDlg, IDC_RB_IFRAME_EVEN, IDC_RB_NIFRAME, IDC_RB_NIFRAME);
                        break;

                    default:
                        /* Unknown frame type - could only happen if more frame */
                        /*   types are added to DT-Open Layers                  */
                        break;
                   }

                 return (TRUE);
                }
                break;


           /* message: received a command */
           case WM_COMMAND:
                switch (wParam)
                  {
                   case IDC_RB_IFRAME_EVEN:
                        ulFrameType = OLC_FG_FRM_IL_FRAME_EVEN;
                        return TRUE;

                   case IDC_RB_IFRAME_ODD:
                        ulFrameType = OLC_FG_FRM_IL_FRAME_ODD;
                        return TRUE;

                   case IDC_RB_IFRAME_NEXT:
                        ulFrameType = OLC_FG_FRM_IL_FRAME_NEXT;
                        return TRUE;

                   case IDC_RB_FIELD_EVEN:
                        ulFrameType = OLC_FG_FRM_FIELD_EVEN;
                        return TRUE;

                   case IDC_RB_FIELD_ODD:
                        ulFrameType = OLC_FG_FRM_FIELD_ODD;
                        return TRUE;
                        break;

                   case IDC_RB_FIELD_NEXT:
                        ulFrameType = OLC_FG_FRM_FIELD_NEXT;
                        return TRUE;

                   case IDC_RB_NIFRAME:
                        ulFrameType = OLC_FG_FRM_NON_INTERLACED;
                        return TRUE;

                   case IDOK:
                        {
                         OLT_APISTATUS Status = OLC_STS_NORMAL;
                         ULNG ulOldData = 0L;

                         // Set the frame type
                         Status = OlFgSetInputControlValue(CurDevInfo.DevId, CurDevInfo.InputSource,
                                                OLC_FG_CTL_FRAME_TYPE, ulFrameType, &ulOldData);

                         if ( !OlImgIsOkay(Status) )
                            PrintStatus(hDlg, Status, "Unable to set frame type.", "Frame Type Error");
                        }

                        EndDialog(hDlg, TRUE);
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
