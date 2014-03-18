/*	File:	3157typ.h
||
||	Description:
||		This file contains macros, data structures, and prototypes
||		for the DT3157 Frame Grabber software.  The information in
||		this file is device dependent, but operating system independent.
||
||	Prerequisites:
||		#include <windows.h>
||		#include <windowsx.h>
||		#include <olwintyp.h>
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

#if !defined(TYP3157_H__)
#define TYP3157_H__


/* include required DT include files */
#include <olwintyp.h>


/* Assume C delclarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************/
/***************************  Macros  **********************************/
/***********************************************************************/

 
 
 /***********************************************************************/
/**************************  Data Types  *******************************/
/***********************************************************************/



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


/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* TYP3157_H__ */
