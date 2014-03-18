/*	File:	olfgapi.h

	Description:
		This file contains the public interface for the DT-Open
	Layers Frame Grabber Application Programming Interface.  This interface
	provides device dependent and operating system independent access
	to Data Translation frame grabbers.


*/
/*=======================================================================++
||	Copyright (C) 1994.         Data Translation, Inc., 100 Locke    ||
||	Drive, Marlboro, Massachusetts 01752-1192.			 ||
||									 ||
||	All rights reserved.  This software is furnished to purchaser	 ||
||	under a license for use on a single computer system and can be	 ||
||	copied (with the inclusion of DTI's copyright notice) only for	 ||
||	use in such system, except as may be otherwise provided in	 ||
||	writing by Data Translation, Inc., 100 Locke Drive, Marlboro,	 ||
||	Massachusetts 01752-1192.					 ||
||									 ||
||	The information in this document is subject to change without	 ||
||	notice and should not be construed as a commitment by Data	 ||
||	Translation, Inc.  Data Translation, Inc. assumes no		 ||
||	responsibility for any errors that may appear in this document.	 ||
||									 ||
||	Data Translation cannot assume any responsibility for the use	 ||
||	of any portion of this software on any equipment not supplied	 ||
||	by Data Translation, Inc.					 ||
||									 ||
++=======================================================================*/

#if !defined(OLFGAPI_H__)
#define OLFGAPI_H__ 1

#include "olwintyp.h"
#include "olimgapi.h"


/* Assume C declarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif



/*=======================================================================*/
/*========================  Type Defintitions  ==========================*/
/*=======================================================================*/

/* "IDs" used by frame grabbers */
#define DECLARE_OL_FG_ID(type)		DECLARE_OL_HDL(FG, type, ID)

/* Frame ID */
DECLARE_OL_FG_ID(FRAME);

/* Async acquire job ID */
DECLARE_OL_FG_ID(ACQJOB);

/* Async passthru job ID */
DECLARE_OL_FG_ID(PASSJOB);



/*=======================================================================*/
/*=======================================================================*/
/*=======================  Prototypes  ==================================*/
/*=======================================================================*/
/*=======================================================================*/

#ifndef DT_MIDL_PASS
OLT_APISTATUS OLIMGAPI OlFgAcquireFrameToDevice(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId);
OLT_APISTATUS OLIMGAPI OlFgAcquireFrameToHost(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId, HPVOID hpBuffer, ULNG ulBufSize);
OLT_APISTATUS OLIMGAPI OlFgAsyncAcquireFrameToDevice(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId,
					LPOLT_FG_ACQJOB_ID lpJobId);
OLT_APISTATUS OLIMGAPI OlFgAsyncAcquireFrameToHost(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId, HPVOID hpBuffer, ULNG ulBufSize,
					LPOLT_FG_ACQJOB_ID lpJobId);
OLT_APISTATUS OLIMGAPI OlFgCancelAsyncAcquireJob(OLT_IMG_DEV_ID DeviceId, OLT_FG_ACQJOB_ID JobId, LPOLT_APISTATUS lpJobStatus);
OLT_APISTATUS OLIMGAPI OlFgEnableBasedSourceMode(OLT_IMG_DEV_ID DeviceId, BOOL bEnable, USHRT usBasedSource);
OLT_APISTATUS OLIMGAPI OlFgIsAsyncAcquireJobDone(OLT_IMG_DEV_ID DeviceId, OLT_FG_ACQJOB_ID JobId, LPBOOL lpDone,
					LPOLT_APISTATUS lpJobStatus, LPULNG lpBytesWrittenToHost);
OLT_APISTATUS OLIMGAPI OlFgQueryBasedSourceMode(OLT_IMG_DEV_ID DeviceId, LPBOOL lpEnabled, LPUSHRT lpBasedSource);
OLT_APISTATUS OLIMGAPI OlFgQueryInputCaps(OLT_IMG_DEV_ID DeviceId, OLT_FG_INPUT_CAP_KEY Key, LPVOID lpData, ULNG ulDataSize);
OLT_APISTATUS OLIMGAPI OlFgQueryInputControlValue(OLT_IMG_DEV_ID DeviceId, USHRT usSource, OLT_FG_INPUT_CONTROL Control,
					LPLNG lpData);
OLT_APISTATUS OLIMGAPI OlFgQueryInputVideoSource(OLT_IMG_DEV_ID DeviceId, LPUSHRT lpSource);
OLT_APISTATUS OLIMGAPI OlFgQueryMultipleTriggerInfo(OLT_IMG_DEV_ID DeviceId, LPOLT_FG_TRIGGER lpTrigger,
					LPBOOL lpTriggerOnLowToHigh, LPOLT_FG_TRIGGER_MODE lpMode);
OLT_APISTATUS OLIMGAPI OlFgQueryTriggerInfo(OLT_IMG_DEV_ID DeviceId, LPOLT_FG_TRIGGER lpTrigger, LPBOOL lpTriggerOnLowToHigh);
OLT_APISTATUS OLIMGAPI OlFgReadInputLUT(OLT_IMG_DEV_ID DeviceId, USHRT usIlut, ULNG ulStart, ULNG ulCount, LPLNG lpLutData,
					ULNG ulLutDataSize);
OLT_APISTATUS OLIMGAPI OlFgSetInputControlValue(OLT_IMG_DEV_ID DeviceId, USHRT usSource, OLT_FG_INPUT_CONTROL Control,
					LNG lNewData, LPLNG lpOldData);
OLT_APISTATUS OLIMGAPI OlFgSetInputVideoSource(OLT_IMG_DEV_ID DeviceId, USHRT usNewSource, LPUSHRT lpOldSource);
OLT_APISTATUS OLIMGAPI OlFgSetMultipleTriggerInfo(OLT_IMG_DEV_ID DeviceId, OLT_FG_TRIGGER NewTrigger,
					BOOL bTriggerOnLowToHigh, OLT_FG_TRIGGER_MODE NewMode,
					LPOLT_FG_TRIGGER lpOldTrigger, LPBOOL lpWasTriggerOnLowToHigh,
					LPOLT_FG_TRIGGER_MODE lpOldMode);
OLT_APISTATUS OLIMGAPI OlFgSetTriggerInfo(OLT_IMG_DEV_ID DeviceId, OLT_FG_TRIGGER NewTrigger, BOOL bTriggerOnLowToHigh,
					LPOLT_FG_TRIGGER lpOldTrigger, LPBOOL lpWasTriggerOnLowToHigh);
OLT_APISTATUS OLIMGAPI OlFgStartEventCounter(OLT_IMG_DEV_ID DeviceId, OLT_FG_EVENT Event, ULNG ulCount, BOOL bWaitForTrigger,
					BOOL bTriggerOnLowToHigh, BOOL bOutputHighOnEvent);
OLT_APISTATUS OLIMGAPI OlFgStopEventCounter(OLT_IMG_DEV_ID DeviceId);
OLT_APISTATUS OLIMGAPI OlFgWriteInputLUT(OLT_IMG_DEV_ID DeviceId, USHRT usIlut, ULNG ulStart, ULNG ulCount, LPCLNG lpcLutData);
OLT_APISTATUS OLIMGAPI OlFgPing(OLT_IMG_DEV_ID DeviceId, DOUBLE dPulseWidth, BOOL bPulseIsHigh, BOOL bWaitForTrigger,
					BOOL bTriggerOnLowToHigh);
OLT_APISTATUS OLIMGAPI OlFgQueryCameraControlCaps(OLT_IMG_DEV_ID DeviceId, OLT_FG_CAMCTL_CAP_KEY Key, LPVOID lpData,
					ULNG ulDataSize);
OLT_APISTATUS OLIMGAPI OlFgSetDigitalOutputMask(OLT_IMG_DEV_ID DeviceId, ULNG ulNewMask, LPULNG lpOldmask);
OLT_APISTATUS OLIMGAPI OlFgAllocateBuiltInFrame(OLT_IMG_DEV_ID DeviceId, OLT_FG_DEV_MEM MemType, USHRT usBufNum,
					LPOLT_FG_FRAME_ID lpFrameId);
OLT_APISTATUS OLIMGAPI OlFgCopyFrameRect(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID SourceFrameId,
					ULNG ulSrcLeft, ULNG ulSrcTop, ULNG ulSrcWidth, ULNG ulSrcheight,
					OLT_FG_FRAME_ID DestFrameId, ULNG ulDestLeft, ULNG ulDestTop);
OLT_APISTATUS OLIMGAPI OlFgDestroyFrame(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId);
OLT_APISTATUS OLIMGAPI OlFgMapFrame(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId, LPOLT_FG_FRAME_INFO lpFrameInfo);
OLT_APISTATUS OLIMGAPI OlFgQueryFrameInfo(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId, LPOLT_FG_FRAME_INFO lpFrameInfo);
OLT_APISTATUS OLIMGAPI OlFgQueryMemoryCaps(OLT_IMG_DEV_ID DeviceId, OLT_FG_MEM_CAP_KEY Key, LPVOID lpData,
					ULNG ulDataSize);
OLT_APISTATUS OLIMGAPI OlFgReadContiguousPixels(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId,
					ULNG ulX, ULNG ulY, ULNG ulCount, HPVOID hpBuffer, ULNG ulBufSize);
OLT_APISTATUS OLIMGAPI OlFgReadFrameRect(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId,
					ULNG ulX, ULNG ulY, ULNG ulWidth, ULNG ulHeight,
					HPVOID hpBuffer, ULNG ulBufSize);
OLT_APISTATUS OLIMGAPI OlFgReadPixelList(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId,
					ULNG ulCount, LPPOINT lpPointList, HPVOID hpBuffer, ULNG ulBufSize);
OLT_APISTATUS OLIMGAPI OlFgUnmapFrame(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId, HPVOID VirtAddr);
OLT_APISTATUS OLIMGAPI OlFgWriteContiguousPixels(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId,
					ULNG ulX, ULNG ulY, ULNG ulCount, HPCVOID hpcPixelData);
OLT_APISTATUS OLIMGAPI OlFgWriteFrameRect(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId,
					ULNG ulX, ULNG ulY, ULNG ulWidth, ULNG ulHeight, HPCVOID hpcPixelData);
OLT_APISTATUS OLIMGAPI OlFgWritePixelList(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId,
					ULNG ulCount, LPPOINT lpPointList, HPCVOID hpcPixelData);
OLT_APISTATUS OLIMGAPI OlFgQueryLinearMemoryCaps(OLT_IMG_DEV_ID DeviceId, OLT_FG_LINEAR_CAP_KEY Key, LPVOID lpData,
					ULNG ulDataSize);
OLT_APISTATUS OLIMGAPI OlFgAsyncAcquireMultipleToLinear(OLT_IMG_DEV_ID DeviceId, UINT uiCount,
					ULNG ulOffset, LPOLT_FG_ACQJOB_ID lpAcqJobId);
OLT_APISTATUS OLIMGAPI OlFgAcquireMultipleToDevice(OLT_IMG_DEV_ID DeviceId, ULNG ulCount,LPOLT_FG_FRAME_ID lpFrameIdList);
OLT_APISTATUS OLIMGAPI OlFgAsyncAcquireMultipleToDevice(OLT_IMG_DEV_ID DeviceId, ULNG ulCount,
					LPOLT_FG_FRAME_ID lpFrameIdList, LPOLT_FG_ACQJOB_ID lpAcqJobId);
OLT_APISTATUS OLIMGAPI OlFgSetPassthruSimScaling(OLT_IMG_DEV_ID DeviceId, LPSIZE lpRequested, LPSIZE lpActual);
OLT_APISTATUS OLIMGAPI OlFgStartSyncPassthruDirect(OLT_IMG_DEV_ID DeviceId, HWND hwnd);
OLT_APISTATUS OLIMGAPI OlFgStartAsyncPassthruDirect(OLT_IMG_DEV_ID DeviceId, HWND hwnd, LPOLT_FG_PASSJOB_ID lpPassJobId);
OLT_APISTATUS OLIMGAPI OlFgStartSyncPassthruBitmap(OLT_IMG_DEV_ID DeviceId, HWND hwnd, OLT_FG_FRAME_ID FrameId);
OLT_APISTATUS OLIMGAPI OlFgStartAsyncPassthruBitmap(OLT_IMG_DEV_ID DeviceId, HWND hwnd, OLT_FG_FRAME_ID FrameId, LPOLT_FG_PASSJOB_ID lpPassJobId);
OLT_APISTATUS OLIMGAPI OlFgSetPassthruSourceOrigin(OLT_IMG_DEV_ID DeviceId, LPPOINT lpSourceOrigin);
OLT_APISTATUS OLIMGAPI OlFgQueryPassthruSourceOrigin(OLT_IMG_DEV_ID DeviceId, LPPOINT lpSourceOrigin);
OLT_APISTATUS OLIMGAPI OlFgSetPassthruScaling(OLT_IMG_DEV_ID DeviceId, LPSIZE lpRequested, LPSIZE lpActual);
OLT_APISTATUS OLIMGAPI OlFgQueryPassthruScaling(OLT_IMG_DEV_ID DeviceId, LPSIZE lpActual);
OLT_APISTATUS OLIMGAPI OlFgStopAsyncPassthru(OLT_IMG_DEV_ID DeviceId, OLT_FG_PASSJOB_ID PassJobId, LPOLT_APISTATUS lpJobStatus);
OLT_APISTATUS OLIMGAPI OlFgQueryPassthruCaps(OLT_IMG_DEV_ID DeviceId, OLT_FG_PASSTHRU_CAP_KEY Key, LPVOID lpData, ULNG ulDataSize);
OLT_APISTATUS OLIMGAPI OlFgExtendPassthruPalette(OLT_IMG_DEV_ID DeviceId, UINT uiStart, UINT uiCount, LPRGBTRIPLE lpRGBTripleArray);
OLT_APISTATUS OLIMGAPI OlFgLoadDefaultPassthruLut(OLT_IMG_DEV_ID DeviceId);
OLT_APISTATUS OLIMGAPI OlFgLoadPassthruLut(OLT_IMG_DEV_ID DeviceId, UINT uiStart, UINT uiCount, LPRGBTRIPLE lpRGBTripleArray);

OLT_APISTATUS OLIMGAPI OlFgReadPaletteMatchingLUT(OLT_IMG_DEV_ID DeviceId, ULNG ulStart, ULNG ulCount,LPLNG lpLutData, ULNG ulLutDataSize);
OLT_APISTATUS OLIMGAPI OlFgWritePaletteMatchingLUT(OLT_IMG_DEV_ID DeviceId, ULNG ulStart, ULNG ulCount, LPCLNG lpcLutData);
OLT_APISTATUS OLIMGAPI OlFgReadDataPassthruLUT(OLT_IMG_DEV_ID DeviceId, ULNG ulStart, ULNG ulCount,LPLNG lpLutData, ULNG ulLutDataSize);
OLT_APISTATUS OLIMGAPI OlFgWriteDataPassthruLUT(OLT_IMG_DEV_ID DeviceId, ULNG ulStart, ULNG ulCount, LPCLNG lpcLutData);

/* Added for revision 2.03 */
OLT_APISTATUS OLIMGAPI OlFgStartAsyncPassthruEx(OLT_IMG_DEV_ID DevId,HWND hWnd,LPOLT_FG_FRAME_ID lpFrameList,LPBOOL lpDone,ULNG ulCount,HANDLE* lphEvent,ULNG ulFlags,LPOLT_FG_PASSJOB_ID lpPassJobId);

/* Added for revision 2.04 */
OLT_APISTATUS OLIMGAPI OlFgSetStrobeInfo(OLT_IMG_DEV_ID DeviceId,ULNG ulDuration,OLT_FG_STROBE Type,BOOL bActiveHigh, BOOL bEnable);
OLT_APISTATUS OLIMGAPI OlFgQueryStrobeInfo(OLT_IMG_DEV_ID DeviceId,LPULNG lpulDuration,LPOLT_FG_STROBE lpType,LPBOOL lpbActive, LPBOOL lpbEnable);
OLT_APISTATUS OLIMGAPI OlFgDrawAcquiredFrameEx(OLT_IMG_DEV_ID DeviceId, HWND hWnd , OLT_FG_FRAME_ID FrameId, POINT SourceOrigin);

#ifndef EXCLUDE_DDI
/* DDI API Call Prototypes  */

OLT_APISTATUS OLIMGAPI OlFgQueryDDICaps(OLT_IMG_DEV_ID, OLT_FG_DDI_CAP_KEY, LPBOOL);
OLT_APISTATUS OLIMGAPI OlFgGetPassthruSyncEvent(OLT_IMG_DEV_ID, HANDLE*);
OLT_APISTATUS OLIMGAPI OlFgSetOverlayColorKey(OLT_IMG_DEV_ID, HANDLE, RGBTRIPLE);
OLT_APISTATUS OLIMGAPI OlFgCreateSurface(OLT_IMG_DEV_ID, HANDLE*, DWORD, DWORD, DWORD);
OLT_APISTATUS OLIMGAPI OlFgDestroySurface(OLT_IMG_DEV_ID, HANDLE);
OLT_APISTATUS OLIMGAPI OlFgGetSurfaceDC(OLT_IMG_DEV_ID, HANDLE, HDC*);
OLT_APISTATUS OLIMGAPI OlFgReleaseSurfaceDC(OLT_IMG_DEV_ID, HANDLE, HDC);
OLT_APISTATUS OLIMGAPI OlFgPassthruSnapShot(OLT_IMG_DEV_ID, OLT_FG_FRAME_ID);
OLT_APISTATUS OLIMGAPI OlFgSetTranslucentOverlay(OLT_IMG_DEV_ID, BOOL);
OLT_APISTATUS OLIMGAPI OlFgSetVisibleSurface(OLT_IMG_DEV_ID, HANDLE);
OLT_APISTATUS OLIMGAPI OlFgSetVisibleBuffer(OLT_IMG_DEV_ID, LPVOID, DWORD, DWORD);
OLT_APISTATUS OLIMGAPI OlFgAddOverlayToFrame(OLT_IMG_DEV_ID, OLT_FG_FRAME_ID, HANDLE, DWORD);
OLT_APISTATUS OLIMGAPI OlFgEnableOverlays(OLT_IMG_DEV_ID, BOOL);
OLT_APISTATUS OLIMGAPI OlFgEraseSurface(OLT_IMG_DEV_ID, HANDLE);
OLT_APISTATUS OLIMGAPI OlFgDrawAcquiredFrame(OLT_IMG_DEV_ID, HWND, OLT_FG_FRAME_ID);

#endif // EXCLUDE_DDI

#endif // DT_MIDL_PASS

/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif


#endif   /* OLFGAPI_H__*/

