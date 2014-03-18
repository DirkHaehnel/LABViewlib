#if !defined(DIBACQ_H__)
#define DIBACQ_H__ 1

#include <windows.h>            /* required for all Windows applications */
#include <windowsx.h>           /* some nice macros, etc. */

#include <assert.h>         /* for asertion testing */
#include <stdlib.h>         /* for atoi() prototype */
#include <memory.h>         /* for _fmemset() */
#include <stdio.h>                      /* for sprintf() */
#include <ctype.h>          /* for ascii test macros */
#include <string.h>

#include "olwintyp.h"           /* standard DT-Open Layers data types and symbolic constants */
#include "olimgapi.h"           /* interface to DT-Open Layers Imaging API */
#include "olfgapi.h"			/* interface to DT-Open Layers Frame Grabber API */
#include "resource.h"

/* Assume C declarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif

#define WINDOW_WIDTH	 768
#define WINDOW_HEIGHT	 576

#define CSYNC_CURRENT	0
#define CSYNC_EXTERNAL	1


/* Type definition for the program */
typedef char STRING[255];		// Standard array of char for a lpstr

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
typedef	DWORD (*DT3157_ENABLE_EXPOSURE_MODE)			(OLT_IMG_DEV_ID,USHRT,BOOL);
typedef DWORD (*DT3157_SET_DIGITAL_CAMERA_TYPE)			(OLT_IMG_DEV_ID,USHRT,DT3157_DIGCAM_TYPE);
typedef DWORD (*DT3157_SET_DIGITAL_IO_CONFIGURATION)	(OLT_IMG_DEV_ID,ULNG);
typedef DWORD (*DT3157_SET_DIGITAL_IO)					(OLT_IMG_DEV_ID,ULNG); 
typedef DWORD (*DT3157_SET_SYNC_MASTER_CONTROL_VALUE)	(OLT_IMG_DEV_ID,USHRT,DT3157_SYNC_CONTROL,ULNG,LPULNG);
typedef DWORD (*DT3157_SET_EXPOSURE)					(OLT_IMG_DEV_ID,USHRT,ULNG,ULNG,LPULNG);
typedef DWORD (*DT3157_ENABLE_SYNC_MASTER_MODE)			(OLT_IMG_DEV_ID,BOOL,USHRT);

// ****************************************************************************
// ****************************************************************************


/* Data types used by app */

/* This structure holds information about the range of capabilities supported */
/*   by the current device                                                    */

typedef struct tagDEVCAPS
    {
     BOOL DoesSyncPassthru;         /* is any sync passthru mode supported?*/
     BOOL DoesSyncPassthruDirect;   /* is sync passthru direct mode supported?*/
     BOOL DoesAsyncPassthru;        /* is any async passthru mode supported?*/
     BOOL DoesAsyncPassthruDirect;  /* is async passthru direct mode supported?*/
     BOOL DoesSoruceOrigin;			/* can we move the source origin?*/
     BOOL DoesDeviceAcquire;		/* can do single frame acquire to device, and can map device memory*/
	 BOOL DoesOverlays;				/* can do overlays*/
	 BOOL DoesDrawAcqFrame;			/* can draw acquired frame*/
	 BOOL DoesPassthruSnapshot;		/* can do Passthru Snapshot*/
	 BOOL DoesCSyncSource;			/* can we choose the csync input */
    } DEVCAPS, FAR *LPDEVCAPS;

     
/* This structure is intended to hold current settings and other info about */
/*    the current device                                                    */

typedef struct tagDEVINFO
    {
     char Alias[OLC_MAX_ALIAS_STR_SIZE];
     OLT_IMG_DEV_ID DevId;
     USHRT InputSource;
     BOOL AsyncPassthru;            /* TRUE -> asynchronous passthru mode; FALSE -> synchronous */
     BOOL PassthruActive;           /* TRUE if passthru is currently in progress*/
     OLT_FG_PASSJOB_ID PassJobId;   /* ID of current passtrhu job */
     int  OriginX;                  /* current x origin of source data */
     int  OriginY;                  /* current y origin of source data */
     BOOL FrameDataValid;           /* TRUE if current data in frame is valid*/
     BYTE __huge * lpBmpData;       /* data buffer to hold inverted frame data for making DIB*/
     OLT_FG_FRAME_ID FrameId;       /* ID of our device frame*/
     OLT_FG_FRAME_INFO FrameInfo;   /* info on our device frame*/
	 ULNG ulCSyncSource;
    } DEVINFO, FAR *LPDEVINFO;


/*references to globals allocted in passthru.h*/
extern DEVCAPS CurDevCaps;     /* capabilities of current device */
extern DEVINFO CurDevInfo;     /* current device information */


/* Function Prototypes for passthru.c */  

int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL PRIVATE InitApplication(HINSTANCE);
BOOL PRIVATE InitInstance(HINSTANCE, int);
LPARAM CALLBACK __export MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SelectDeviceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL PRIVATE InitDeviceList(HWND hComboBox);
void PRIVATE CloseCurrentDevice(void);
BOOL PRIVATE NewDevice(LPCSTR lpcszAlias, HWND hWnd);
void OnSelectDevice(HWND hWnd);
UINT SetPal128Grey(HDC hdc);
void RestorePalette(HDC hdc);
void DrawDisplayBitmap(HWND hWnd,HDC hdc);
void SetupScrollBars(HWND hWnd, UINT width, UINT height);
int UpdateScrollBar(HWND hWnd, int fnBar, UINT ScrollCode, int NewPos);
void AcquireSingleFrame(HWND hWnd);
void OnStartPassthru(HWND hWnd);

/* Function prototypes for utils.c */

BOOL CALLBACK __export About(HWND, UINT, WPARAM, LPARAM);
void PRIVATE CenterWindow(HWND hWnd, HWND hOwner);
void PRIVATE PrintStatus (HWND hDlg, OLT_APISTATUS status, LPSTR lpszOpMsg, LPSTR lpszMBTitle);
void PRIVATE ResetControlFocus (HWND hDlg, int idControl, long lValue);
LNG PRIVATE GetDlgItemLong(HWND hDlg, int idControl, LPBOOL lpbWorked);

/* Function prototypes for dialog box procedures */

BOOL CALLBACK __export InputSourceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OverlayControlProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SnapShotProc(HWND, UINT, WPARAM, LPARAM);


/* Function prototypes for Overlay.c */
BOOL DrawOnSurface(HWND, LPARAM, BOOL);

/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif


#endif  /* ACQ2HST_H__ */
