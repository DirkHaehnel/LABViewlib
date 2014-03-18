/*	File:	3152api.h

	Description:
		This file defines the public interface to the DT3152
      Frame Grabber Application Programming Interface.

	Prerequisites:
		#include <windows.h>
		#include <windowsx.h>
		#include <olwintyp.h>
		#include <olimgapi.h>
		#include <3152typ.h>

*/
/*=======================================================================++
||	Copyright (C) 1993 - 1994.  Data Translation, Inc., 100 Locke    ||
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

#if !defined(API3152_H__)
#define API3152_H__


/* include required DT include files */
#include <olwintyp.h>
#include <olimgapi.h>
#include <dt3152\3152typ.h>



/* Assume C delclarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************/
/***************************  Macros  **********************************/
/***********************************************************************/

/* DT3152-specific API status messages */
#define DT3152C_STS_MAPADDR		OL_MAKE_NONOL_ERROR_APISTATUS(0x1)	/* Driver received invalid 48-bit map address */
#define DT3152C_STS_HFREQ		OL_MAKE_NONOL_ERROR_APISTATUS(0x2)	/* Horiz Freq > Input CLK */
#define DT3152C_STS_VFREQ		OL_MAKE_NONOL_ERROR_APISTATUS(0x3)	/* Vert Freq > Horiz Freq */
#define DT3152C_STS_HPULSEWIDTH	OL_MAKE_NONOL_ERROR_APISTATUS(0x4)	/* HPulseWidth > Horiz Freq */
#define DT3152C_STS_VPULSEWIDTH	OL_MAKE_NONOL_ERROR_APISTATUS(0x5)	/* VPulseWidth > Vertical Freq */




/***********************************************************************/
/**************************  Data Types  *******************************/
/***********************************************************************/




/***********************************************************************/
/***********************  Function Prototypes  *************************/
/***********************************************************************/


OLT_APISTATUS OLIMGAPI Dt3152QuerySyncMasterControlValue(OLT_IMG_DEV_ID DeviceId, USHRT usSource, DT3152_SYNC_CONTROL Control, LPULNG lpData);
OLT_APISTATUS OLIMGAPI Dt3152SetSyncMasterControlValue(OLT_IMG_DEV_ID DeviceId, USHRT usSource, DT3152_SYNC_CONTROL Control, ULNG ulNewData, LPULNG lpOldData);
OLT_APISTATUS OLIMGAPI Dt3152EnableSyncMasterMode(OLT_IMG_DEV_ID DeviceId, BOOL bEnable, USHRT usInputSource);

OLT_APISTATUS OLIMGAPI Dt3152QueryInputControlValue(OLT_IMG_DEV_ID DeviceId, USHRT InputSource, DT3152_INPUT_CONTROL Control, LPULNG lpData );
OLT_APISTATUS OLIMGAPI Dt3152SetInputControlValue(OLT_IMG_DEV_ID DeviceId, USHRT InputSource, DT3152_INPUT_CONTROL Control, ULNG ulNewData, LPULNG lpOldData);


/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* API3152_H__ */
