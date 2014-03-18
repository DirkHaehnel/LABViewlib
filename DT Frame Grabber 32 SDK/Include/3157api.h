/*      File:   3157api.h
||
||	Description:
||		This file defines the public interface to the DT3157
||      Frame Grabber Application Programming Interface.
||
||	Prerequisites:
||		#include <windows.h>
||		#include <windowsx.h>
||		#include <olwintyp.h>
||		#include <olimgapi.h>
||		#include <3157typ.h>
||
*/
/*=======================================================================++
||      Copyright (C) 1993 - 1996.  Data Translation, Inc., 100 Locke    ||
||      Drive, Marlboro, Massachusetts 01752-1192.                       ||
||                                                                       ||
||      All rights reserved.  This software is furnished to purchaser    ||
||      under a license for use on a single computer system and can be   ||
||      copied (with the inclusion of DTI's copyright notice) only for   ||
||      use in such system, except as may be otherwise provided in       ||
||      writing by Data Translation, Inc., 100 Locke Drive, Marlboro,    ||
||      Massachusetts 01752-1192.                                        ||
||                                                                       ||
||      The information in this document is subject to change without    ||
||      notice and should not be construed as a commitment by Data       ||
||      Translation, Inc.  Data Translation, Inc. assumes no             ||
||      responsibility for any errors that may appear in this document.  ||
||                                                                       ||
||      Data Translation cannot assume any responsibility for the use    ||
||      of any portion of this software on any equipment not supplied    ||
||      by Data Translation, Inc.                                        ||
||                                                                       ||
++=======================================================================*/

#if !defined(API3157_H__)
#define API3157_H__


/* include required DT include files */
#include <olwintyp.h>
#include <olimgapi.h>
#include <dt3157\3157typ.h>



/* Assume C delclarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************/
/***************************  Macros  **********************************/
/***********************************************************************/

/* DT3157-specific API status messages */
#define DT3157C_STS_HFREQ               OL_MAKE_NONOL_ERROR_APISTATUS(0x1)      /* Horiz Freq > Input CLK */
#define DT3157C_STS_VFREQ               OL_MAKE_NONOL_ERROR_APISTATUS(0x2)      /* Vert Freq > Horiz Freq */
#define DT3157C_STS_HPULSEWIDTH OL_MAKE_NONOL_ERROR_APISTATUS(0x3)      /* HPulseWidth > Horiz Freq */
#define DT3157C_STS_VPULSEWIDTH OL_MAKE_NONOL_ERROR_APISTATUS(0x4)      /* VPulseWidth > Vertical Freq */
#define DT3157C_STS_OVERRUN     OL_MAKE_NONOL_ERROR_APISTATUS(0x5)      




/***********************************************************************/
/**************************  Data Types  *******************************/
/***********************************************************************/




/***********************************************************************/
/***********************  Function Prototypes  *************************/
/***********************************************************************/

OLT_APISTATUS OLIMGAPI Dt3157ResetCamera(OLT_IMG_DEV_ID DeviceId);
OLT_APISTATUS OLIMGAPI Dt3157SetDigitalCameraType(OLT_IMG_DEV_ID DeviceId, USHRT usSource, DT3157_DIGCAM_TYPE CameraType);
OLT_APISTATUS OLIMGAPI Dt3157QueryDigitalCameraType(OLT_IMG_DEV_ID DeviceId, USHRT usSource, LPDT3157_DIGCAM_TYPE lpCameraType);
OLT_APISTATUS OLIMGAPI Dt3157SetDigitalIOConfiguration(OLT_IMG_DEV_ID DeviceId, ULNG ulDigitalIOMask);
OLT_APISTATUS OLIMGAPI Dt3157QueryDigitalIOConfiguration(OLT_IMG_DEV_ID DeviceId, LPULNG lpDigitalIOMask);
OLT_APISTATUS OLIMGAPI Dt3157SetDigitalIO(OLT_IMG_DEV_ID DeviceId, ULNG ulDigitalOut); 
OLT_APISTATUS OLIMGAPI Dt3157QueryDigitalIO(OLT_IMG_DEV_ID DeviceId, LPULNG lpDigitalIn);
OLT_APISTATUS OLIMGAPI Dt3157EnableExposureMode(OLT_IMG_DEV_ID DeviceId, USHRT usSource, BOOL bEnable);
OLT_APISTATUS OLIMGAPI Dt3157SetExposure(OLT_IMG_DEV_ID DeviceId, USHRT usSource, ULNG ulDesiredDuration, ULNG ulPolarity, LPULNG lpActualDuration);
OLT_APISTATUS OLIMGAPI Dt3157QueryExposure(OLT_IMG_DEV_ID DeviceId, USHRT usSource, LPULNG lpDuration, LPULNG lpPolarity);
OLT_APISTATUS OLIMGAPI Dt3157SetSyncMasterControlValue(OLT_IMG_DEV_ID DeviceId, USHRT usSource, DT3157_SYNC_CONTROL Control, ULNG ulNewData, LPULNG lpOldData);
OLT_APISTATUS OLIMGAPI Dt3157QuerySyncMasterControlValue(OLT_IMG_DEV_ID DeviceId, USHRT usSource, DT3157_SYNC_CONTROL Control, LPULNG lpData);
OLT_APISTATUS OLIMGAPI Dt3157EnableSyncMasterMode(OLT_IMG_DEV_ID DeviceId, BOOL bEnable, USHRT usInputSource);

/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* API3157_H__ */
