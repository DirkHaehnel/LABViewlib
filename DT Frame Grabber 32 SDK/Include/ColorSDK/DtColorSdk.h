/****************************************************************************
	Description:
		This file defines the public interface to the DT3153
      Frame Grabber Application Programming Interface.
*****************************************************************************/

/****************************************************************************
	Copyright (C) 1996.  Data Translation, Inc., 100 Locke    
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
*****************************************************************************/

#if !defined(APICOLOR_SDK_H__)
#define APICOLOR_SDK_H__

/****************************************************************************
/****************************************************************************
	Since we are getting the sources from Source Safe, When you do a get,
	since everything is under the same project, we don't need anymore to use
	the <...> to get our include. This will prevent the user to have to set
	a whole bunch of include directories in his environment.
*****************************************************************************/

/* include required DT include files */
#include "olwintyp.h"
#include "olimgapi.h"
#include "olfgapi.h"


/* Assume C delclarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************/
/**************************  Data Types  *******************************/
/***********************************************************************/
typedef enum OLT_CONTROL_TYPE
	{
	OLC_QUERY_CONTROL_MIN			= 0x0,
	OLC_QUERY_CONTROL_MAX			= 0x1,
	OLC_QUERY_CONTROL_GRANULARITY	= 0x2,
	OLC_QUERY_CONTROL_NOMINAL		= 0x3,
	OLC_QUERY_CAPABILITY				,
	OLC_QUERY_CONFIGURATION				,
	OLC_CONFIGURE_CONTROL				,
	OLC_READ_CONTROL					,
	OLC_WRITE_CONTROL					,
	}OLT_CONTROL_TYPE, FAR * FP_OLT_CONTROL_TYPE;
	
typedef enum OLT_SIGNAL_TYPE
{
	OLC_SIGNAL_UNSUPPORTED	,
	OLC_MONO_SIGNAL			,
	OLC_YC_SIGNAL			,
	OLC_COMPOSITE_SIGNAL	,
	OLC_RGB_SIGNAL			,
	OLC_TRIPLE_MONO_SIGNAL	,
	OLC_DUAL_MONO_SIGNAL	,
}OLT_SIGNAL_TYPE, FAR *FP_OLT_SIGNAL_TYPE;

typedef ULNG   OLT_CHANNEL_ID;

typedef enum OLT_IMAGE_MODE 
{
	OLC_IMAGE_UNSUPPORTED	=0,
	OLC_IMAGE_MONO			,
	OLC_IMAGE_YUV			,
	OLC_IMAGE_RGB			,
	OLC_IMAGE_RGB_16		,
	OLC_IMAGE_RGB_15		,
	OLC_IMAGE_RGB_24		,
	OLC_IMAGE_RGB_32
}OLT_IMAGE_MODE, FAR *FP_OLT_IMAGE_MODE; 

typedef enum OLT_COLOR_PARAMETER
{
	OLC_COLOR_UNSUPPORTED	= 0x0,
	OLC_SET_BRIGHTNESS		,
	OLC_SET_CONTRAST		,
	OLC_SET_V_SAT			,
	OLC_SET_U_SAT			,
	OLC_SET_HUE				,
	OLC_SET_RED_LEVEL		,
	OLC_SET_GREEN_LEVEL		,
	OLC_SET_BLUE_LEVEL		,
	OLC_SET_RED_REF			,
	OLC_SET_GREEN_REF		,
	OLC_SET_BLUE_REF		,
	OLC_SET_RED_OFF			,
	OLC_SET_GREEN_OFF		,
	OLC_SET_BLUE_OFF
}OLT_COLOR_PARAMETER, FAR *FP_OLT_COLOR_PARAMETER;

typedef enum OLT_SYNC_MASTER_PARAMETER
{
	OLC_SYNC_MASTER_UNSUPPORTED	= 0x00,
	OLC_SYNC_MASTER_ENABLE		,
}OLT_SYNC_MASTER_PARAMETER,FAR *FP_OLT_SYNC_MASTER_PARAMETER;

typedef enum OLT_QUERY_COLOR_INTERFACE
{
	OLC_COLOR_INTERFACE_UNSUPPORTED					= 0x00,
	OLC_QUERY_COLOR_INTERFACE_SIGNAL_TYPE,
	OLC_QUERY_COLOR_INTERFACE_STORAGE_MODE,
	OLC_QUERY_COLOR_INTERFACE_IMAGE_PARAMETER,
	OLC_QUERY_COLOR_INTERFACE_HARDWARE_SCALING,
	OLC_QUERY_COLOR_INTERFACE_DIGITAL_IO,
	OLC_QUERY_COLOR_INTERFACE_DRAW_ACQUIRED_FRAME,
	OLC_QUERY_COLOR_INTERFACE_SYNC_MASTER_MODE,
	OLC_QUERY_COLOR_INTERFACE_EXTRACT_FRAME,
	OLC_QUERY_COLOR_INTERFACE_DRAW_BUFFER,
}OLT_QUERY_COLOR_INTERFACE,FAR *FP_OLT_QUERY_COLOR_INTERFACE;

typedef ULNG OLT_COLOR;
typedef OLT_COLOR* FP_OLT_COLOR;
typedef ULNG OLT_SET_VALUE;
typedef OLT_SET_VALUE* FP_OLT_SET_VALUE;

#ifdef DT_MIDL_PASS

typedef struct OLT_SCALE_PARAM
{
	short hscale,vscale;
}OLT_SCALE_PARAM,FAR *FP_OLT_SCALE_PARAM;


// User's Buffer information 
typedef struct OLT_BUFFER_INFO
{
	long	StructSize;	// Size of structure; filled in by caller
	long	BufferAddr;	// Pointer to the user allocated memory
	long	BufferSize;	// Size (bytes) of buffer point to by BufferAddr
	long	Width;		// Number of pixels per line
	long	Height;		// Number of lines per frame
	long	PixelDepth;	// How many byter per pixel
	long	Flags;		// Flags defining certain Buffer characteristics
	long	Reserved;		// Reserved for future use
} OLT_BUFFER_INFO, *LPOLT_BUFFER_INFO;
#else

typedef struct OLT_SCALE_PARAM
{
	unsigned short hscale,vscale;
}OLT_SCALE_PARAM,FAR *FP_OLT_SCALE_PARAM;


// User's Buffer information 
typedef struct tagOLT_BUFFER_INFO
{
	DWORD	StructSize;	// Size of structure; filled in by caller
	LPVOID	BufferAddr;	// Pointer to the user allocated memory
	DWORD	BufferSize;	// Size (bytes) of buffer point to by BufferAddr
	DWORD	Width;		// Number of pixels per line
	DWORD	Height;		// Number of lines per frame
	DWORD	PixelDepth;	// How many byter per pixel
	DWORD	Flags;		// Flags defining certain Buffer characteristics
	DWORD	Reserved;		// Reserved for future use
} OLT_BUFFER_INFO, *LPOLT_BUFFER_INFO;

#endif // DT_MIDL_PASS

// #define's to simplify SetInputControlValue -> CSYNC for RGB Frame Grabbers
#define OLC_RVID0_CSYNC		0x00000002	// Sync on channel 0 Red   
#define OLC_GVID0_CSYNC		0x00010002	// Sync on channel 0 Green - default
#define OLC_BVID0_CSYNC		0x00020002	// Sync on channel 0 Blue  
#define OLC_RVID1_CSYNC		0x00030002	// Sync on channel 1 Red   
#define OLC_GVID1_CSYNC		0x00040002	// Sync on channel 1 Green 
#define OLC_BVID1_CSYNC		0x00050002	// Sync on channel 1 Blue  

// #define's to simplify ReadILut/WriteILut for RGB Frame Grabbers
#define OLC_RVID0_ILUT	0x0000	// channel 0 Red   
#define OLC_GVID0_ILUT	0x0001	// channel 0 Green 
#define OLC_BVID0_ILUT	0x0002	// channel 0 Blue   
#define OLC_RVID1_ILUT	0x0003	// channel 0 Red
#define OLC_GVID1_ILUT	0x0004	// channel 0 Green
#define OLC_BVID1_ILUT	0x0005	// channel 1 Blue
#define OLC_RGB0_ILUT	0x0006	// channel 0 RGB write only
#define OLC_RGB1_ILUT	0x0007	// channel 1 RGB write only    

#define	OLC_EF_EXTRACT_1ST			   0x01
#define	OLC_EF_EXTRACT_2ND			   0x02
#define	OLC_EF_EXTRACT_3RD			   0x04
#define	OLC_EF_EXTRACT_ALL			   0x07
#define	OLC_EF_BUILD_INTERLACED_FRAME  0x08

/***********************************************************************/
/***********************  Function Prototypes  *************************/
/***********************************************************************/

#ifndef DT_MIDL_PASS
OLT_APISTATUS OLIMGAPI DtColorSignalType(OLT_IMG_DEV_ID DeviceId, OLT_CHANNEL_ID Channel, OLT_CONTROL_TYPE Control, FP_OLT_SIGNAL_TYPE mode);
OLT_APISTATUS OLIMGAPI DtColorStorageMode(OLT_IMG_DEV_ID DeviceId, OLT_CHANNEL_ID Channel,OLT_CONTROL_TYPE Control,FP_OLT_IMAGE_MODE image_mode);
OLT_APISTATUS OLIMGAPI DtColorImageParameters(OLT_IMG_DEV_ID DeviceId,OLT_CHANNEL_ID Channel,OLT_CONTROL_TYPE Control,FP_OLT_COLOR_PARAMETER Color,FP_OLT_COLOR value);
OLT_APISTATUS OLIMGAPI DtColorHardwareScaling(OLT_IMG_DEV_ID DeviceId, OLT_CHANNEL_ID  Channel,OLT_CONTROL_TYPE Control,FP_OLT_SCALE_PARAM ScaleValue);
OLT_APISTATUS OLIMGAPI DtColorDigitalIOControl(OLT_IMG_DEV_ID DeviceId, OLT_CONTROL_TYPE Control,FP_OLT_SET_VALUE Value);
OLT_APISTATUS OLIMGAPI DtColorDrawAcquiredFrame(OLT_IMG_DEV_ID DeviceId, HWND handle,OLT_FG_FRAME_ID FrameId);
OLT_APISTATUS OLIMGAPI DtColorSyncMasterMode(OLT_IMG_DEV_ID DeviceId, OLT_CHANNEL_ID  Channel, OLT_CONTROL_TYPE Control,FP_OLT_SYNC_MASTER_PARAMETER Action,FP_OLT_SET_VALUE Value);
OLT_APISTATUS OLIMGAPI DtColorQueryInterface(OLT_IMG_DEV_ID DeviceId,FP_OLT_QUERY_COLOR_INTERFACE Interface);
OLT_APISTATUS OLIMGAPI DtColorExtractFrametoBuffer(OLT_IMG_DEV_ID DeviceId, OLT_FG_FRAME_ID FrameId, LPOLT_BUFFER_INFO lpBuffer, DWORD Flags);
OLT_APISTATUS OLIMGAPI DtColorDrawBuffer(OLT_IMG_DEV_ID DeviceId, HWND handle,LPOLT_BUFFER_INFO lpBuffer);
#endif // DT_MIDL_PASS

/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* APICOLOR_SDK_H__ */
