/*
        File:   Syncsent.C

        Description:
                This file contains the necessary functions for the
                Sync Sentinel dialog box.  The following functions
                are found in this file:

                SyncSentinelProc() -
                        dialog proc for Setup SyncSentinel dialog

                InitSyncSentinelDlg() -
                        performs initialization of GDI controls in
                        SyncSentinel dialog

                SetSyncSentinelInfo() -
                        sets the frame grabber's Sync Sentinel
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

/*      Function:       SyncSentinelProc

        Description:
                This function is the dialog procedure for the dialog box used to
                determine whether, or how, Sync Sentinel is used by the frame grabber.
                This information is used by the frame grabber to determine how
                to interpret sync information.  Specifically, it indicates where
                in the video signal the frame grabber should look for sync
                information.  It also defines where/when the frame grabber should
                insert a sync signal if none is encountered.

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

BOOL CALLBACK __export SyncSentinelProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
        static BOOL bSyncSentinel = FALSE;

        static SYNCSENTINFO Info = {0};

    /* What's the message? */
        switch (message)
          {
           case WM_INITDIALOG:
                {
                 /* Center the dialog box */

                 CenterWindow(hDlg, GetWindow(hDlg, GW_OWNER));

                 /* Initialize the edit controls and check box */

                 InitSyncSentinelDlg(hDlg, &bSyncSentinel, &Info);

                 return (TRUE);
                }

                /* message: received a command */
                case WM_COMMAND:
                switch (wParam)
                  {
                        case IDC_CK_ENABLE:
                        bSyncSentinel = !bSyncSentinel;
                        return TRUE;

                        case IDOK:

                        /* Set the Sync Sentinel based on user's input */

                        SetSyncSentinelInfo(hDlg, bSyncSentinel, &Info);
                        return (TRUE);

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
/**************************************************************************/

/*      Function:       InitSyncSentinelDlg

        Description:
                This function initializes the edit controls and check box in
                the Sync Sentinel dialog based upon the frame grabber's
                capabilities.


        Input Arguments:
                HWND hDlg                               handle of owning dialog

        Output Arguments:
                LPBOOL lpSyncSentinel                  set to TRUE if SyncSentinel is enabled;
                                                       FALSE otherwise

        Return Value (void):
                None.
*/

void PRIVATE InitSyncSentinelDlg(HWND hDlg, LPBOOL lpSyncSentinel, LPSYNCSENTINFO lpInfo)
{
        ULNG ulData = 0L;
        char szText[20], szTemp[20];
        double dData = 0;
        HWND hVInsert = GetDlgItem(hDlg, IDC_ED_VINSERT);
        HWND hHInsert = GetDlgItem(hDlg, IDC_ED_HINSERT);
        HWND hVSearch = GetDlgItem(hDlg, IDC_ED_VSEARCH);
        HWND hHSearch = GetDlgItem(hDlg, IDC_ED_HSEARCH);

        BOOL bSyncSentinel;    /* is sync sentinel enabled ? */
        BOOL bSyncVariable;    /* can we change positioning  */


    /* Determine the current settings on the device */

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_SYNC_SENTINEL,
                                                &ulData);
        bSyncSentinel = ulData ? TRUE : FALSE;
        CheckDlgButton(hDlg, IDC_CK_ENABLE, (UINT) bSyncSentinel);

    (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                      OLC_FG_IC_SYNC_SENTINEL_TYPE_LIMITS,
                      &ulData,
                      sizeof(ulData));

        if (ulData == OLC_FG_SYNC_SENTINEL_VARIABLE)
          bSyncVariable = TRUE;
        else
          bSyncVariable = FALSE;


    /* Initialize the edit controls */

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_HSYNC_SEARCH_POS,
                                                &ulData);

        dData = (double) ulData / 100;
        sprintf(szTemp, "%.2f", dData);
        wsprintf(szText, "%s", szTemp);
        Edit_SetText(hHSearch, (LPSTR) szText);
        strcpy(lpInfo->szHSearch, (LPSTR)szText);

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_HSYNC_INSERT_POS,
                                                &ulData);
        dData = (double) ulData / 100;
        sprintf(szTemp, "%.2f", dData);
        wsprintf(szText, "%s", szTemp);
        Edit_SetText(hHInsert, (LPSTR) szText);
        strcpy(lpInfo->szHInsert, (LPSTR)szText);

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_VSYNC_SEARCH_POS,
                                                &ulData);
        dData = (double) ulData / 100;
        sprintf(szTemp, "%.2f", dData);
        wsprintf(szText, "%s", szTemp);
        Edit_SetText(hVSearch, (LPSTR) szText);
        strcpy(lpInfo->szVSearch, (LPSTR)szText);

        (void) OlFgQueryInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_VSYNC_INSERT_POS,
                                                &ulData);

        dData = (double) ulData / 100;
        sprintf(szTemp, "%.2f", dData);
        wsprintf(szText, "%s", szTemp);
        Edit_SetText(hVInsert, (LPSTR) szText);
        strcpy(lpInfo->szVInsert, (LPSTR)szText);


    /* Now, enable the edit controls based on the capabilities */

        EnableWindow(hVInsert, bSyncVariable);
        EnableWindow(hHInsert, bSyncVariable);
        EnableWindow(hVSearch, bSyncVariable);
        EnableWindow(hHSearch, bSyncVariable);

    /* Set the return variable */

        *lpSyncSentinel = bSyncSentinel;

        return;
}
/**************************************************************************/
/**************************************************************************/

/*      Function:       SetSyncSentinelInfo

        Description:
                This function sets the frame grabber according to the user's
                dialog selections.


        Input Arguments:
                HWND hDlg                               handle of owning dialog
                BOOL bSyncSentinel                      TRUE if user enabled it

        Output Arguments:
                None

        Return Value (void):
                None.
*/

void PRIVATE SetSyncSentinelInfo(HWND hDlg, BOOL bSyncSentinel, LPSYNCSENTINFO lpInfo)
{
        ULNG ulOldData = 0L;
        OLT_APISTATUS Status;
        ULNG uiData = 0;
        ULNG ulData;
        BOOL bWorked = FALSE;
        double dData = 0.0;
        LPCHR lpszErrorTitle = "Sync Sentinel Error";

    BOOL bSyncVariable;


    /* Enable/Disable Sync Sentinel */

        Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_SYNC_SENTINEL,
                                                (ULNG) bSyncSentinel,
                                                &ulOldData);

        if ( !OlImgIsOkay(Status) )
           {
             PrintStatus(hDlg, Status, "Unable to set Sync Sentinel.", lpszErrorTitle);
             EndDialog(hDlg, FALSE);
             return;
           }



    /*  can we change the settings of the sync sentinel position ?? */

    (void) OlFgQueryInputCaps(CurDevInfo.DevId,
                      OLC_FG_IC_SYNC_SENTINEL_TYPE_LIMITS,
                      &ulData,
                      sizeof(ulData));

        if (ulData == OLC_FG_SYNC_SENTINEL_VARIABLE)
          bSyncVariable = TRUE;
        else
          bSyncVariable = FALSE;


    /* Read the edit controls and set the frame grabber controls */
    /* check for negative value or non numerical input */

        if (bSyncVariable)
         {
           dData = GetDlgItemDouble(hDlg, IDC_ED_HSEARCH, &bWorked);
           if ( !bWorked || dData < 0 )
            {
              char msg[OLC_MAX_STATUS_MESSAGE_SIZE + 60];
              HWND hControl = GetDlgItem(hDlg, IDC_ED_HSEARCH);
              wsprintf(msg, "Invalid value entered for Horizontal Search Position.");
              MessageBox(hDlg, msg, lpszErrorTitle, MB_ICONSTOP | MB_OK);
              SetDlgItemText(hDlg, IDC_ED_HSEARCH, lpInfo->szHSearch);
              SetFocus(hControl);
              Edit_SetSel(hControl, 0, lstrlen(lpInfo->szHSearch));
              return;
            }

           uiData = (ULNG) (dData * 100.0);
           Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_HSYNC_SEARCH_POS,
                                                uiData,
                                                &ulOldData);
          if ( !OlImgIsOkay(Status) )
             {
               PrintStatus(hDlg, Status, "Unable to set horizontal search position.", lpszErrorTitle);
               EndDialog(hDlg, FALSE);
               return;
             }

      /* check for negative value or non numerical input */

          dData = GetDlgItemDouble(hDlg, IDC_ED_HINSERT, &bWorked);
          if ( !bWorked  || dData < 0)
           {
             char msg[OLC_MAX_STATUS_MESSAGE_SIZE + 60];
             HWND hControl = GetDlgItem(hDlg, IDC_ED_HINSERT);
             wsprintf(msg, "Invalid value entered for Horizontal Insert Position.");
             MessageBox(hDlg, msg, lpszErrorTitle, MB_ICONSTOP | MB_OK);
             SetDlgItemText(hDlg, IDC_ED_HINSERT, lpInfo->szHInsert);
             SetFocus(hControl);
             Edit_SetSel(hControl, 0, lstrlen(lpInfo->szHInsert));
             return;
           }

          uiData = (ULNG) (dData * 100.0);
          Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_HSYNC_INSERT_POS,
                                                uiData,
                                                &ulOldData);
          if ( !OlImgIsOkay(Status) )
           {
             PrintStatus(hDlg, Status, "Unable to set horizontal insert position.", lpszErrorTitle);
             EndDialog(hDlg, FALSE);
             return;
           }


          /* check for negative value or non numerical input */

          dData = GetDlgItemDouble(hDlg, IDC_ED_VSEARCH, &bWorked);
          if ( !bWorked || dData < 0)
            {
              char msg[OLC_MAX_STATUS_MESSAGE_SIZE + 60];
              HWND hControl = GetDlgItem(hDlg, IDC_ED_VSEARCH);
              wsprintf(msg, "Invalid value entered for Vertical Search Position.");
              MessageBox(hDlg, msg, lpszErrorTitle, MB_ICONSTOP | MB_OK);
              SetDlgItemText(hDlg, IDC_ED_VSEARCH, lpInfo->szVSearch);
              SetFocus(hControl);
              Edit_SetSel(hControl, 0, lstrlen(lpInfo->szVSearch));
              return;
            }

          uiData = (ULNG) (dData * 100.0);
          Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_VSYNC_SEARCH_POS,
                                                uiData,
                                                &ulOldData);

          if ( !OlImgIsOkay(Status) )
           {
             PrintStatus(hDlg, Status, "Unable to set vertical search position.", lpszErrorTitle);
             EndDialog(hDlg, FALSE);
             return;
           }

          /* check for negative value or non numerical input */

          dData = GetDlgItemDouble(hDlg, IDC_ED_VINSERT, &bWorked);
          if ( !bWorked || dData < 0)
           {
            char msg[OLC_MAX_STATUS_MESSAGE_SIZE + 60];
            HWND hControl = GetDlgItem(hDlg, IDC_ED_VINSERT);
            wsprintf(msg, "Invalid value entered for Vertical Insert Position.");
            MessageBox(hDlg, msg, lpszErrorTitle, MB_ICONSTOP | MB_OK);
            SetDlgItemText(hDlg, IDC_ED_VINSERT, lpInfo->szVInsert);
            SetFocus(hControl);
            Edit_SetSel(hControl, 0, lstrlen(lpInfo->szVInsert));
            return;
           }

          uiData = (ULNG) (dData * 100.0);
          Status = OlFgSetInputControlValue(CurDevInfo.DevId,
                                                CurDevInfo.InputSource,
                                                OLC_FG_CTL_VSYNC_INSERT_POS,
                                                uiData,
                                                &ulOldData);

          if ( !OlImgIsOkay(Status) )
           {
             PrintStatus(hDlg, Status, "Unable to set vertical insert position.", lpszErrorTitle);
             EndDialog(hDlg, FALSE);
             return;
           }

    }
        EndDialog(hDlg,TRUE);
}


