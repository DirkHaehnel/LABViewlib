/*======================================================================

Copyright (C) 1997.  Data Translation, Inc., 100 Locke
Drive, Marlboro, Massachusetts 01752-1192.
   	
All rights reserved.  This software is furnished to purchaser
under a license for use on a single computer system and can be
copied (with the inclusion of DTI's copyright notice) only for
use in such system, except as may be otherwise provided in
writing by Data Translation, Inc., 100 Locke Drive, Marlboro,
Massachusetts 01752-1192.

The information in this document is subject to change without
notice and should not be construed as a commitment by Data
Translation, Inc.  Data Translation, Inc. assumes no
responsibility for any errors that may appear in this document.

Data Translation cannot assume any responsibility for the use
of any portion of this software on any equipment not supplied
by Data Translation, Inc.

=======================================================================*/

#if !defined(DT_LINESCAN_API_H__)
#define DT_LINESCAN_API_H__


/* include required DT include files */
#include "olwintyp.h"
#include "olimgapi.h"

/* Assume C delclarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
	DWORD	dwSize;						// For revision verification
	DWORD	dwPixelDepth;				// Pixel depth in bytes
	DWORD	dwWidth;					// Width oh the image in buffer
	DWORD	dwHeight;					// Number of lines in the image
	DWORD	dwOriginX;					// Where to start to draw in X
	DWORD	dwOriginY;					// Where to start to draw in Y
	void*	lpBuffer;
} LS_BUFFER_INFO, *LPLS_BUFFER_INFO;
														 
#define	ASYNC_ACQUIRE	1
#define	SYNC_ACQUIRE 	2

#define LS_SYNC_HI_TO_LO	0
#define LS_SYNC_LO_TO_HI	1

#define LS_INTGR_HI_TO_LO	0
#define LS_INTGR_LO_TO_HI	1

#define LS_PASS_DONT_STOP_ON_OVERFLOW 0
#define LS_PASS_STOP_ON_OVERFLOW 1


/***********************************************************************/
/***********************  Function Prototypes  *************************/
/***********************************************************************/

OLT_APISTATUS OLIMGAPI OlFgAcquireLines(OLT_IMG_DEV_ID DeviceId, LPVOID lpUserBuffer, DWORD dwUserBufferSize, DWORD AcqFlag);
OLT_APISTATUS OLIMGAPI OlFgEnableLsMode			(OLT_IMG_DEV_ID DeviceId, BOOL bEnableLineScan);
OLT_APISTATUS OLIMGAPI OlFgStartAsyncLsPassthru	(OLT_IMG_DEV_ID DeviceId, HWND hWnd, LPVOID* lplpBuffer, DWORD dwDataBufferSize, 
							DWORD dwPointerBufferSize, DWORD dwNumBuffers, HANDLE* lphEvent, LPBOOL lpDone, DWORD dwDoneBufferSize, DWORD dwFlags);
OLT_APISTATUS OLIMGAPI OlFgStopAsyncLsPassthru	(OLT_IMG_DEV_ID DeviceId);
OLT_APISTATUS OLIMGAPI OlFgIsAcquireLinesDone	(OLT_IMG_DEV_ID DeviceId, BOOL* Complete, DWORD* lpdwLinesCompleted, DWORD* lpdwResolution);

OLT_APISTATUS OLIMGAPI OlFgSetLsIntegration		(OLT_IMG_DEV_ID DeviceId, DWORD Edge, DWORD   Value);
OLT_APISTATUS OLIMGAPI OlFgGetLsIntegration		(OLT_IMG_DEV_ID DeviceId, DWORD Edge, LPDWORD lpValue);
OLT_APISTATUS OLIMGAPI OlFgSetLsLineDrive		(OLT_IMG_DEV_ID DeviceId, DWORD Edge, DWORD	  Value);
OLT_APISTATUS OLIMGAPI OlFgGetLsLineDrive		(OLT_IMG_DEV_ID DeviceId, DWORD Edge, LPDWORD lpValue);
OLT_APISTATUS OLIMGAPI OlFgSetLsDigIo			(OLT_IMG_DEV_ID DeviceId, DWORD Value);
OLT_APISTATUS OLIMGAPI OlFgGetLsDigIo			(OLT_IMG_DEV_ID DeviceId, LPDWORD lpValue);
OLT_APISTATUS OLIMGAPI OlFgSetLsDriveClkDiv		(OLT_IMG_DEV_ID DeviceId, DWORD dwValue);
OLT_APISTATUS OLIMGAPI OlFgGetLsDriveClkDiv		(OLT_IMG_DEV_ID DeviceId, LPDWORD lpdwValue);


OLT_APISTATUS OLIMGAPI OlFgDrawAcquiredLines	(OLT_IMG_DEV_ID DeviceId, HWND hWnd, LS_BUFFER_INFO BufferInfo);


/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* DT_LINESCAN_API_H__ */
