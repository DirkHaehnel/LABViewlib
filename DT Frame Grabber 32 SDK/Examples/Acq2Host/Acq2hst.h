#if !defined(ACQ2HST_H__)
#define ACQ2HST_H__ 1

#include <windows.h>                    /* required for all Windows applications */
#include <windowsx.h>                   /* some nice macros, etc. */

#include <assert.h>                     /* for asertion testing */
#include <stdlib.h>                     /* for atoi() prototype */
#include <memory.h>                     /* for _fmemset() */
#include <stdio.h>                      /* for sprintf() */
#include <ctype.h>                      /* for ascii test macros */
#include <string.h>

#include "olwintyp.h"                   /* standard DT-Open Layers data types and symbolic constants */
#include "olimgapi.h"                   /* interface to DT-Open Layers Imaging API */
#include "olfgapi.h"                    /* interface to DT-Open Layers Frame Grabber API */
#include "resource.h"

/* Assume C declarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif



/* Data types used by app */


/* This structure holds information about the range of capabilities supported */
/*   by the current device                                                    */

typedef struct tagDEVCAPS
        {
         BOOL DoesInputFilter;
         BOOL DoesA2D;
         BOOL DoesVideoType;
         BOOL DoesActiveVideo;
         BOOL DoesFrameSize;
         BOOL DoesClock;
         BOOL DoesSyncSentinel;
         ULNG FrameTypes;
         ULNG SourceCount;
        } DEVCAPS, FAR *LPDEVCAPS;


/* This structure is intended to hold current settings and other info about */
/*    the current device                                                    */

typedef struct tagDEVINFO
        {
         char Alias[OLC_MAX_ALIAS_STR_SIZE];
         OLT_IMG_DEV_ID DevId;
         USHRT InputSource;
        } DEVINFO, FAR *LPDEVINFO;



/* This structure is used to hold the limits and current values for the video */
/*   area controls of the frame grabber.                                      */

typedef struct tagVIDEOAREAINFO
        {
         OLT_LNG_RANGE BackPorchStartRange;
         OLT_LNG_RANGE ClampStartRange;
         OLT_LNG_RANGE ClampEndRange;
         OLT_LNG_RANGE PixelsPerLineRange;
         OLT_LNG_RANGE FirstPixelRange;
         OLT_LNG_RANGE ActivePixelsRange;
         OLT_LNG_RANGE LinesPerFieldRange;
         OLT_LNG_RANGE FirstLineRange;
         OLT_LNG_RANGE ActiveLinesRange;
         ULNG BackPorchStart;
         ULNG ClampStart;
         ULNG ClampEnd;
         ULNG PixelsPerLine;
         ULNG FirstPixel;
         ULNG ActivePixels;
         ULNG LinesPerField;
         ULNG FirstLine;
         ULNG ActiveLines;
        } VIDEOAREAINFO, FAR *LPVIDEOAREAINFO;



/* This structure is used to hold the limits and current values for the frame */
/*   size controls of the frame grabber.                                      */

typedef struct tagFRAMESIZEINFO
        {
         OLT_LNG_RANGE LeftRange;
         OLT_LNG_RANGE TopRange;
         OLT_LNG_RANGE WidthRange;
         OLT_LNG_RANGE HeightRange;
         OLT_LNG_RANGE HorizontalIncRange;
         OLT_LNG_RANGE VerticalIncRange;
         ULNG Left;
         ULNG Top;
         ULNG Width;
         ULNG Height;
         ULNG HorizontalInc;
         ULNG VerticalInc;
        } FRAMESIZEINFO, FAR *LPFRAMESIZEINFO;

/* This structure is used to store the values read from the sync
   sentinel dialog box edit controls. */

typedef struct tagSYNCSENTINFO
        {
         char szHSearch[20];
         char szVSearch[20];
         char szHInsert[20];
         char szVInsert[20];
        } SYNCSENTINFO, FAR * LPSYNCSENTINFO;

/* This structure is used for the Cleanup function that is called
   if there is a memory allocation problem when creating a DIB. */

typedef struct tagPointerList
       {
         HGLOBAL hPal;
         HPALETTE hGrayscalePal;
         LPLOGPALETTE lpPal;
         HGLOBAL hDIBdata;
         HPUCHR hpDIBdata;
         HGLOBAL hDIBheader;
         BITMAPINFO * lpDIBheader;
        } POINTERLIST, FAR *LPPOINTERLIST;

// ****************************************************************************
//							  DT3157 - STUFF
// ****************************************************************************

/* DT3157 Digital camera types */
typedef enum DT3157_DIGITAL_CAMERA_TYPES
	{
	 DT3157_DIGCAM_16BIT_INPUT               = 0x0000,
	 DT3157_DIGCAM_14BIT_INPUT               = 0x0001,
	 DT3157_DIGCAM_12BIT_INPUT               = 0x0002,
	 DT3157_DIGCAM_10BIT_INPUT               = 0x0003,
	 DT3157_DIGCAM_8BIT_SINGLE_CHANNEL_INPUT = 0x0004,
	 DT3157_DIGCAM_8BIT_DUAL_CHANNEL_INPUT   = 0x0005
	} DT3157_DIGCAM_TYPE, *PDT3157_DIGCAM_TYPE, FAR *LPDT3157_DIGCAM_TYPE;

/* DT3157 Sync Controls */
typedef enum DT3157_SYNC_CONTROL
	{
	 DT3157_SYNC_CTL_UNKNOWN		= 0x0000,
	 DT3157_SYNC_CTL_HORIZ_FREQ		= 0x0001,
	 DT3157_SYNC_CTL_VERT_FREQ		= 0x0002,
	 DT3157_SYNC_CTL_HPULSE_WIDTH	= 0x0003,
	 DT3157_SYNC_CTL_VPULSE_WIDTH	= 0x0004,
	 DT3157_SYNC_CTL_PHASE			= 0x0005
	 	 
	} DT3157_SYNC_CONTROL, *PDT3157_SYNC_CONTROL, FAR *LPDT3157_SYNC_CONTROL;

 /* Integration pulse (exposure) settings */

typedef struct DT3157T_Exposure
	{
	 USHRT input_channel; /* input channel to apply exposure */
	 ULNG PulseDuration;  /* Integration pulse duration (in usec) */
	 ULNG Polarity;		  /* logic polarity of pulse to generate*/
	} DT3157T_EXPOSURE, *PDT3157T_EXPOSURE, FAR *LPDT3157T_EXPOSURE;

// These are the function pointer to use if we have a dt3157 board in
// the PC. Otherwise, they won't be used

typedef char STRING[255];		// Standard array of char for a lpstr

// ****************************************************************************
// ****************************************************************************




/* Global variables.  "GLOBAL__" is defined only in the main module.  In all    */
/*   other modules, it preprocesses to a declaration, rather than a definition. */

#if defined(GLOBAL__)
#define GLOBAL
#define EQU(init)       = init
#else
#define GLOBAL          extern
#define EQU(init)
#endif

/* Flags used by the RdAndChkEditCtlValue to determine the type of operation */

#define INCREMENT  1
#define DECREMENT  -1
#define SET        0

GLOBAL DEVCAPS CurDevCaps EQU({0});             /* capabilities of current device */
GLOBAL DEVINFO CurDevInfo EQU({0});             /* current device information */


/* Function Prototypes for acq2hst.c */
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL PRIVATE InitApplication(HINSTANCE);
BOOL PRIVATE InitInstance(HINSTANCE, int);
LPARAM CALLBACK __export MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK __export About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK __export SelectDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT PRIVATE CreateDIBDisplay(HPUCHR hpAcquireBuf, ULNG ulHeight, ULNG ulWidth, ULNG ulPixelDepth);
BOOL PRIVATE InitDeviceList(HWND hComboBox);
void PRIVATE CloseCurrentDevice(void);
BOOL PRIVATE NewDevice(LPCSTR lpcszAlias);
void PRIVATE AcquireToHost(DEVINFO DevInfo);


void PRIVATE CenterWindow(HWND hWnd, HWND hOwner);
void PRIVATE CleanupDIBPointers(LPPOINTERLIST lpPointerList);
BOOL RdAndChkEditCtlVal(HWND hdlg, int Ctl_ID, LPOLT_LNG_RANGE lpRange, LPCHR lpszTitle, LPLNG lpResult, int IncDecSet);
LNG PRIVATE GetDlgItemLong(HWND hDlg, int idControl, LPBOOL lpbWorked);
double PRIVATE GetDlgItemDouble(HWND hDlg, int idControl, LPBOOL lpbWorked);
void PRIVATE PrintStatus (HWND hDlg, OLT_APISTATUS status, LPSTR lpszOpMsg, LPSTR lpszMBTitle);
void PRIVATE ResetControlFocus (HWND hDlg, int idControl, long lValue);


BOOL CALLBACK __export InputFilterProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK __export InputSourceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK __export A2DProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void PRIVATE SetA2DInfo(HWND, LPLNG , LPLNG );

BOOL CALLBACK __export VideoSelectProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void PRIVATE InitComboBoxFromControlList(HWND hComboBox,
                        OLT_FG_INPUT_CAP_KEY ListLimitsKey, OLT_FG_INPUT_CAP_KEY ListKey);
void PRIVATE InitCSyncSourceList(HWND hComboBox);
void PRIVATE SetCsyncInfo(HWND hDlg);
void PRIVATE SetVarscanInfo(BOOL bLSyncLowToHigh, BOOL bFSyncLowToHigh);

BOOL CALLBACK __export FrameTypeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK __export SyncSentinelProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void PRIVATE InitSyncSentinelDlg(HWND hDlg, LPBOOL lpSyncSentinel, LPSYNCSENTINFO lpInfo);
void PRIVATE SetSyncSentinelInfo(HWND hDlg, BOOL bSyncSentinel, LPSYNCSENTINFO lpInfo);

BOOL CALLBACK __export PixelClockProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void PRIVATE InitPixelClockDlg(HWND hDlg, LPBOOL lpInternal, LPBOOL lpExtClockOnLowToHigh, LPULNG lpCurFreq);
void PRIVATE SetPixelClockInfo(HWND hDlg, BOOL bInternal, BOOL bExtClockOnLowToHigh, LPULNG lpCurFreq);

BOOL CALLBACK __export VideoAreaProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void PRIVATE InitVideoAreaDlg(HWND hDlg, LPVIDEOAREAINFO lpInfo);
void PRIVATE IncrementVideoControl(HWND hDlg, HWND hControl, LPVIDEOAREAINFO lpInfo);
void PRIVATE DecrementVideoControl(HWND hDlg, HWND hControl, LPVIDEOAREAINFO lpInfo);
void PRIVATE MinMaxVideoControl(HWND hDlg, HWND hControl, LPVIDEOAREAINFO lpInfo, BOOL bMaximize);
void PRIVATE SetVideoAreaInfo(HWND hDlg, LPVIDEOAREAINFO lpInfo);

BOOL CALLBACK __export FrameSizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void PRIVATE InitFrameSizeDlg(HWND hDlg, LPFRAMESIZEINFO lpInfo);
void PRIVATE IncrementFrameControl(HWND hDlg, HWND hControl, LPFRAMESIZEINFO lpInfo);
void PRIVATE DecrementFrameControl(HWND hDlg, HWND hControl, LPFRAMESIZEINFO lpInfo);
void PRIVATE MinMaxFrameControl(HWND hDlg, HWND hControl, LPFRAMESIZEINFO lpInfo, BOOL bMaximize);
void PRIVATE SetFrameSizeInfo(HWND hDlg, LPFRAMESIZEINFO lpInfo);

BOOL CALLBACK __export TimeoutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif


#endif  /* ACQ2HST_H__ */
