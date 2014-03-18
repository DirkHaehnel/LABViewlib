/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2002. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  main_panel                      1       /* callback function: cb_main_panel */
#define  main_panel_INPUT_POSITION4      2
#define  main_panel_INPUT_POSITION3      3
#define  main_panel_INPUT_POSITION2      4
#define  main_panel_INPUT_POSITION1      5
#define  main_panel_DISPLAY_POSITION4    6
#define  main_panel_DISPLAY_POSITION3    7
#define  main_panel_DISPLAY_POSITION2    8
#define  main_panel_DISPLAY_POSITION1    9
#define  main_panel_CB_COPY_POSITION1    10      /* callback function: cb_copy_position1 */
#define  main_panel_CB_WRITE_POSITION    11      /* callback function: cb_write_position */
#define  main_panel_CB_READ_POSITION     12      /* callback function: cb_read_position */
#define  main_panel_DECORATION_2         13
#define  main_panel_VERSIONDISPLAY       14
#define  main_panel_DECORATION_3         15


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK cb_copy_position1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cb_main_panel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cb_read_position(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cb_write_position(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
