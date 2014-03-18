/*	File:	olfg.h

	Description:
		This file contains macros, data types and prototypes for
	the DT-Open Layers Monochrome Frame Grabber Device Interface for
	Windows 3.1.

	

				History
	------------------------------------------------------------------
	01/13/94	bsm	Moved generic stuff to OLIMGDEV.H.
				Driver messages now relative to each other to
				take less of total range.
	06/09/93	bsm	Changed file to OLFG.
				Modified naming conventions to more closely
				match OLFG/NT naming conventions.
	02/19/92	bsm	genesis
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

#if !defined(OLFG_H__)
#define OLFG_H__

/* Include required include DT-Open Layers include files */
#include "olwintyp.h"
#include "olimgdev.h"


/* Assume C delclarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif



/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*- DT-Open Layers Frame Grabber Driver Messages and Data Structures  -*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

/* All pre-defined DT-Open Layers Monochrome frame grabber driver messages */
/*    begin at DRV_OLFG_BASE                                               */


/*=======================================================*/
/*======================= General =======================*/
/*=======================================================*/

/* LUT Data Types */
typedef struct olOLT_FG_LUT_RANGE
	{
	 USHRT Lut;			/* ILUT number */
	 ULNG Start;			/* First index to access */
	 ULNG Count;			/* Number of sequential indecies to access */
	} OLT_FG_LUT_RANGE, *POLT_FG_LUT_RANGE, FAR *LPOLT_FG_LUT_RANGE;

/* Palette Matching LUT Data Types (Only one palette matching lut available) */
typedef struct olOLT_FG_PMLUT_RANGE
	{
	 ULNG Start;			/* First index to access */
	 ULNG Count;			/* Number of sequential indecies to access */
	} OLT_FG_PMLUT_RANGE, *POLT_FG_PMLUT_RANGE, FAR *LPOLT_FG_PMLUT_RANGE;


/* "Handle" definition macro */
#define DECLARE_OLFG_HANDLE(name)	DECLARE_OL_HDL(FG, name, HANDLE)



/*=======================================================*/
/*====================  Input  ==========================*/
/*=======================================================*/

#define DRV_OLFG_INPUT_BASE					(DRV_OLFG_BASE)
#define DRV_OLFG_QUERY_INPUT_CAPS			(DRV_OLFG_INPUT_BASE + 0x0001)
#define DRV_OLFG_QUERY_INPUT_SUPPORT		(DRV_OLFG_INPUT_BASE + 0x0002)
#define DRV_OLFG_SET_INPUT_SOURCE			(DRV_OLFG_INPUT_BASE + 0x0010)
#define DRV_OLFG_QUERY_INPUT_SOURCE			(DRV_OLFG_INPUT_BASE + 0x0011)
#define DRV_OLFG_SET_INPUT_CONTROL			(DRV_OLFG_INPUT_BASE + 0x0012)
#define DRV_OLFG_QUERY_INPUT_CONTROL		(DRV_OLFG_INPUT_BASE + 0x0013)
#define DRV_OLFG_READ_ILUT					(DRV_OLFG_INPUT_BASE + 0x0014)
#define DRV_OLFG_WRITE_ILUT					(DRV_OLFG_INPUT_BASE + 0x0015)
#define DRV_OLFG_ENABLE_BASED_SOURCE		(DRV_OLFG_INPUT_BASE + 0x0016)
#define DRV_OLFG_QUERY_BASED_STATE			(DRV_OLFG_INPUT_BASE + 0x0017)
#define DRV_OLFG_SET_TRIGGER				(DRV_OLFG_INPUT_BASE + 0x0018)
#define DRV_OLFG_QUERY_TRIGGER				(DRV_OLFG_INPUT_BASE + 0x0019)
#define DRV_OLFG_SET_MULT_TRIGGER			(DRV_OLFG_INPUT_BASE + 0x0020)
#define DRV_OLFG_QUERY_MULT_TRIGGER			(DRV_OLFG_INPUT_BASE + 0x0021)
#define DRV_OLFG_START_EVENT_COUNTER		(DRV_OLFG_INPUT_BASE + 0x0030)
#define DRV_OLFG_STOP_EVENT_COUNTER			(DRV_OLFG_INPUT_BASE + 0x0031)
#define DRV_OLFG_ACQUIRE_HST_FRAME			(DRV_OLFG_INPUT_BASE + 0x0040)
#define DRV_OLFG_ACQUIRE_DEV_FRAME			(DRV_OLFG_INPUT_BASE + 0x0041)
#define DRV_OLFG_QUERY_ACQJOB_DONE			(DRV_OLFG_INPUT_BASE + 0x0050)
#define DRV_OLFG_CANCEL_ACQJOB				(DRV_OLFG_INPUT_BASE + 0x0051)
#define DRV_OLFG_ASYNC_ACQ_HST_FRAME		(DRV_OLFG_INPUT_BASE + 0x0052)
#define DRV_OLFG_ASYNC_ACQ_DEV_FRAME		(DRV_OLFG_INPUT_BASE + 0x0053)
#define DRV_OLFG_ACQ_MULT_DEV_FRAME			(DRV_OLFG_INPUT_BASE + 0x0054)
#define DRV_OLFG_ASYNC_ACQ_MULT_DEV_FRAME	(DRV_OLFG_INPUT_BASE + 0x0055)
#define	DRV_OLFG_DRAW_ACQUIRED_FRAME		(DRV_OLFG_INPUT_BASE + 0x0056)
#define	DRV_OLFG_QUERY_VIDEO_TYPE			(DRV_OLFG_INPUT_BASE + 0x0057)
#define	DRV_OLFG_SET_VIDEO_TYPE				(DRV_OLFG_INPUT_BASE + 0x0058)
#define	DRV_OLFG_DRAW_ACQUIRED_FRAME_EX		(DRV_OLFG_INPUT_BASE + 0x0059)
#define	DRV_OLFG_QUERY_STROBE				(DRV_OLFG_INPUT_BASE + 0x0060)
#define	DRV_OLFG_SET_STROBE					(DRV_OLFG_INPUT_BASE + 0x0061)

/* Control Data Types */
typedef struct olOLT_FG_CTL
	{
	 USHRT InputSource;		/* Identifies the input stream */
	 OLT_FG_INPUT_CONTROL Control;	/* Control Identifier */
	} OLT_FG_CTL, *POLT_FG_CTL, FAR *LPOLT_FG_CTL;

typedef struct olOLT_FG_SET_CTL_INFO
	{
	 OLT_FG_CTL Control;
	 ULNG Data;
	} OLT_FG_SET_CTL_INFO, *POLT_FG_SET_CTL_INFO, FAR *LPOLT_FG_SET_CTL_INFO;


/* Event Counter Data Types */
typedef struct olOLT_FG_EVENT_INFO
	{
	 OLT_FG_EVENT Event;				/* Event type */
	 UINT Count;					/* Number of events to count */
	 BOOL WaitForTrigger;				/* Start event counting on hw trigger? */
	 BOOL TriggerOnLowToHigh;			/* Use Low-to-High transition on event trigger line as trigger? */
	 BOOL OutputHighOnEvent;			/* Generate a rising edge on the output line when event occurs? */
	} OLT_FG_EVENT_INFO, *POLT_FG_EVENT_INFO, FAR *LPOLT_FG_EVENT_INFO;


/* Trigger data types */
typedef struct olOLT_FG_TRIGGER_INFO
	{
	 OLT_FG_TRIGGER Type;			/* Identifies the type of trigger desired */
	 BOOL TriggerOnLowToHigh;		/* Indicates the type of transition that indicates a trigger */
	} OLT_FG_TRIGGER_INFO, *POLT_FG_TRIGGER_INFO, FAR *LPOLT_FG_TRIGGER_INFO;

typedef struct olOLT_FG_MULT_TRIGGER_INFO
	{
	 OLT_FG_TRIGGER_INFO Trigger;		/* Identifies the type of trigger desired */
	 OLT_FG_TRIGGER_MODE Mode;		/* Specifies the mode used for multiple acquisitions */
	} OLT_FG_MULT_TRIGGER_INFO, *POLT_FG_MULT_TRIGGER_INFO, FAR *LPOLT_FG_MULT_TRIGGER_INFO;


/* Based Source Data Type */
typedef struct olOLT_FG_BASED_SOURCE
	{
	 BOOL Enable;				/* TRUE to enable based source mode */
	 USHRT InputSource;			/* Input source on which to base contols */
	} OLT_FG_BASED_SOURCE, *POLT_FG_BASED_SOURCE, FAR *LPOLT_FG_BASED_SOURCE;


/* Frame Data Types */
DECLARE_OLFG_HANDLE(FRAME);		/* "Frame" handle */


/* Asynchronous Acquire Data Types */
typedef struct olOLT_FG_DEV_FRAME_LIST
	{
	 ULNG Count;					/* Number of frames to acquire */
	 OLT_FG_FRAME_HANDLE FrameHandle[1];		/* Array of frame handles */
	} OLT_FG_DEV_FRAME_LIST, *POLT_FG_DEV_FRAME_LIST, FAR *LPOLT_FG_DEV_FRAME_LIST;


typedef struct olOLT_FG_HST_FRAME_EXTENTS_INFO
	{
	 SIZE Size;				/* Height and width of rectangle */
	 OLT_OUTBUF HstFrame;			/* Structure containing pointer to user buffer */
	} OLT_FG_HST_FRAME_EXTENTS_INFO, *POLT_FG_HST_FRAME_EXTENTS_INFO, FAR *LPOLT_FG_HST_FRAME_EXTENTS_INFO;


typedef struct olOLT_FG_DEV_FRAME_EXTENTS_INFO
	{
	 SIZE Size;				/* Height and width of rectangle */
	 OLT_FG_FRAME_HANDLE DevFrame;		/* Frame handle of device frame */
	} OLT_FG_DEV_FRAME_EXTENTS_INFO, *POLT_FG_DEV_FRAME_EXTENTS_INFO, FAR *LPOLT_FG_DEV_FRAME_EXTENTS_INFO;

typedef struct olOLT_FG_FRAME_TRANSFER_INFO
	{
	 OLT_FG_FRAME_HANDLE DevFrame;			/* Frame handle of device frame */
	 LPOLT_OUTBUF lpOutBuffer;		/* pointer to Structure containing pointer to host buffer */
	} OLT_FG_FRAME_TRANSFER_INFO, *POLT_FG_FRAME_TRANSFER_INFO, FAR *LPOLT_FG_FRAME_TRANSFER_INFO;



/* Acquire Job Types */
DECLARE_OLFG_HANDLE(ACQJOB);			/* "Acquire Job" handle */

typedef struct olOLT_FG_ACQJOB_STATUS
	{
	 BOOL Done;				/* Flag indicating state of job */
	 OLT_SCODE JobStatus;			/* Completion status of job */
	 ULNG BytesWrittenToHost;		/* Number of bytes written to host during execution of job */
	} OLT_FG_ACQJOB_STATUS, *POLT_FG_ACQJOB_STATUS, FAR *LPOLT_FG_ACQJOB_STATUS;


/*=======================================================*/
/*================  Camera Control  =====================*/
/*=======================================================*/

#define DRV_OLFG_CAMCTL_BASE		(DRV_OLFG_BASE + 0x0200)
#define DRV_OLFG_QUERY_CAMCTL_CAPS	(DRV_OLFG_CAMCTL_BASE + 0x0001)
#define DRV_OLFG_SET_DIG_OUT		(DRV_OLFG_CAMCTL_BASE + 0x0010)
#define DRV_OLFG_PING			(DRV_OLFG_CAMCTL_BASE + 0x0020)


/* Describes a "pulse" */
typedef struct olOLT_FG_PULSE
	{
	 double PulseWidth;		/* Pulse duration in nanoseconds */
	 BOOL PulseIsHigh;		/* TRUE if pulse signal goes high for duration of pulse */
	} OLT_FG_PULSE, *POLT_FG_PULSE, FAR *LPOLT_FG_PULSE;


/* Pulse control structure for "ping" operation */
typedef struct olOLT_FG_PING
	{
	 OLT_FG_PULSE Pulse;		/* Pulse information */
	 BOOL WaitForTrigger;		/* Start pulse on external trigger? */
	 BOOL TriggerOnLowToHigh;	/* Use Low-to-High transition on external trigger line as trigger? */
	} OLT_FG_PING, *POLT_FG_PING, FAR *LPOLT_FG_PING;


/*=======================================================*/
/*================  Memory Management  ==================*/
/*=======================================================*/

#define DRV_OLFG_MEM_BASE			(DRV_OLFG_BASE + 0x0400)
#define DRV_OLFG_QUERY_MEMORY_CAPS		(DRV_OLFG_MEM_BASE + 0x0001)
#define DRV_OLFG_ALLOC_BUILT_IN_FRAME		(DRV_OLFG_MEM_BASE + 0x0011)
#define DRV_OLFG_DESTROY_FRAME			(DRV_OLFG_MEM_BASE + 0x0012)
#define DRV_OLFG_MAP_FRAME			(DRV_OLFG_MEM_BASE + 0x0013)
#define DRV_OLFG_UNMAP_FRAME			(DRV_OLFG_MEM_BASE + 0x0014)
#define DRV_OLFG_QUERY_FRAME_INFO		(DRV_OLFG_MEM_BASE + 0x0015)
#define DRV_OLFG_READ_FRAME_RECT		(DRV_OLFG_MEM_BASE + 0x0030)
#define DRV_OLFG_WRITE_FRAME_RECT		(DRV_OLFG_MEM_BASE + 0x0031)
#define DRV_OLFG_READ_CONTIG_PIXELS		(DRV_OLFG_MEM_BASE + 0x0032)
#define DRV_OLFG_WRITE_CONTIG_PIXELS		(DRV_OLFG_MEM_BASE + 0x0033)
#define DRV_OLFG_READ_RANDOM_PIXELS		(DRV_OLFG_MEM_BASE + 0x0034)
#define DRV_OLFG_WRITE_RANDOM_PIXELS		(DRV_OLFG_MEM_BASE + 0x0035)
#define DRV_OLFG_COPY_FRAME_RECT		(DRV_OLFG_MEM_BASE + 0x0036)


/* Pixel Access Data Types */
typedef struct olOLT_FG_DEV_FRAME_RECT
	{
	 OLT_FG_FRAME_HANDLE Frame;			/* Frame handle */
	 OLT_RECT Rect;					/* Rectangluar region in frame */
	} OLT_FG_DEV_FRAME_RECT, *POLT_FG_DEV_FRAME_RECT, FAR *LPOLT_FG_DEV_FRAME_RECT;


typedef struct olOLT_FG_DEV_FRAME_POINTS
	{
	 OLT_FG_FRAME_HANDLE Frame;			/* Frame handle */
	 ULNG Count;					/* Number of points */
	 LPPOINT PointList;				/* Array of points to read/write */
	} OLT_FG_DEV_FRAME_POINTS, *POLT_FG_DEV_FRAME_POINTS, FAR *LPOLT_FG_DEV_FRAME_POINTS;


typedef struct olOLT_FG_COPY_RECT_INFO
	{
	 OLT_FG_FRAME_HANDLE SourceFrame;		/* Source Frame handle */
	 OLT_RECT SourceRect;				/* Rectangle in source frame */
	 OLT_FG_FRAME_HANDLE DestFrame;			/* Destination Frame handle */
	 POINT DestPoint;				/* Upper-left corner of destination rectangle */
	} OLT_FG_COPY_RECT_INFO, *POLT_FG_COPY_RECT_INFO, FAR *LPOLT_FG_COPY_RECT_INFO;


/*=======================================================*/
/*===============  Linear Memory  =======================*/
/*=======================================================*/

#define DRV_OLFG_LINEAR_BASE			(DRV_OLFG_BASE + 0x0600)
#define DRV_OLFG_QUERY_LINEAR_CAPS		(DRV_OLFG_LINEAR_BASE + 0x0001)
#define DRV_OLFG_ASYNC_MULTACQ_TO_LINEAR	(DRV_OLFG_LINEAR_BASE + 0x0020)


/* Multiple acquire to linear memory information */
typedef struct olOLT_FG_LINEAR_ACQ_INFO
	{
	 ULNG Count;				/* Number of frames to acquire */
	 ULNG Offset;				/* Offset, in bytes, into linear memory area where first pixel goes */
	} OLT_FG_LINEAR_ACQ_INFO, *POLT_FG_LINEAR_ACQ_INFO, FAR *LPOLT_FG_LINEAR_ACQ_INFO;


/*=======================================================*/
/*==================  Passthru  =========================*/
/*=======================================================*/  

#define DRV_OLFG_PASSTHRU_BASE				(DRV_OLFG_BASE + 0x0800)
#define DRV_OLFG_QUERY_PASSTHRU_CAPS		(DRV_OLFG_PASSTHRU_BASE + 0x0001)
#define DRV_OLFG_SYNC_PASS_DIRECT			(DRV_OLFG_PASSTHRU_BASE + 0x0010)
#define DRV_OLFG_ASYNC_PASS_DIRECT			(DRV_OLFG_PASSTHRU_BASE + 0x0011)
#define DRV_OLFG_SYNC_PASS_BITMAP			(DRV_OLFG_PASSTHRU_BASE + 0x0012)
#define DRV_OLFG_ASYNC_PASS_BITMAP			(DRV_OLFG_PASSTHRU_BASE + 0x0013)
#define DRV_OLFG_SET_PASS_SRC_ORIGIN		(DRV_OLFG_PASSTHRU_BASE + 0x0021)
#define DRV_OLFG_QUERY_PASS_SRC_ORIGIN		(DRV_OLFG_PASSTHRU_BASE + 0x0022)
#define DRV_OLFG_SET_PASS_SCALING			(DRV_OLFG_PASSTHRU_BASE + 0x0031)
#define DRV_OLFG_QUERY_PASS_SCALING			(DRV_OLFG_PASSTHRU_BASE + 0x0032)
#define DRV_OLFG_STOP_ASYNC_PASSTHRU		(DRV_OLFG_PASSTHRU_BASE + 0x0040)
#define DRV_OLFG_LOAD_PASSTHRU_LUT			(DRV_OLFG_PASSTHRU_BASE + 0x0050)
#define DRV_OLFG_LOAD_DEFAULT_PASSTHRU_LUT  (DRV_OLFG_PASSTHRU_BASE + 0x0051)
#define DRV_OLFG_EXTEND_PASSTHRU_PALETTE    (DRV_OLFG_PASSTHRU_BASE + 0x0052)
#define DRV_OLFG_SET_PASS_SIM_SCALING       (DRV_OLFG_PASSTHRU_BASE + 0x0053)
#define	DRV_OLFG_PASSTHRU_SNAPSHOT			(DRV_OLFG_PASSTHRU_BASE + 0x0054)
#define DRV_OLFG_ASYNC_PASS_EX				(DRV_OLFG_PASSTHRU_BASE + 0x0055)


/* Passthru Job Types */
DECLARE_OLFG_HANDLE(PASSJOB);			/* "Passthru Job" handle */

/* Passthru information */
typedef struct olOLT_FG_PASSTHRU_INFO
	{
	 HWND					hWnd;			/* window handle in which passthru image data will be placed */
	 OLT_FG_FRAME_HANDLE	FrameHandle;	/* handle of the frame to for use if bitmap transfer */
	} OLT_FG_PASSTHRU_INFO, *POLT_FG_PASSTHRU_ACQ_INFO, FAR *LPOLT_FG_PASSTHRU_INFO;


/* Extended passthru information */
typedef struct olOLT_FG_PASSTHRU_INFO_EX
{
	HWND					hWnd;
	void*					lpFrameList;
	LPBOOL					lpDone;
	ULNG					ulCount;
	HANDLE*					lphEvent;
	ULNG					ulFlags;
	void*					lpPassJobId;

} OLT_FG_PASSTHRU_INFO_EX, *LPOLT_FG_PASSTHRU_INFO_EX;

typedef struct olOLT_FG_STROBE_INFO
{
	ULNG			ulSize;
	ULNG			ulDuration;
	OLT_FG_STROBE	oltType;
	BOOL			bActive;
	BOOL			bEnable;

}OLT_FG_STROBE_INFO;

typedef struct
{
	HWND	hWnd;
	POINT	SrcOrigin;
	HANDLE	hFrame;

} OLT_FG_DRAW_ACQUIRED_INFO;
                                             

/*=======================================================*/
/*==  Passthru LUTs (palette matching - data passthru) ==*/
/*=======================================================*/  

#define DRV_OLFG_PLUT_BASE		(DRV_OLFG_BASE + 0x0C00)
#define DRV_OLFG_READ_PMLUT		(DRV_OLFG_PLUT_BASE + 0x0001)
#define DRV_OLFG_WRITE_PMLUT	(DRV_OLFG_PLUT_BASE + 0x0002)
#define DRV_OLFG_READ_DPLUT		(DRV_OLFG_PLUT_BASE + 0x0003)
#define DRV_OLFG_WRITE_DPLUT	(DRV_OLFG_PLUT_BASE + 0x0004)

                                             
/*---------------------------------------------------------------------*/
/*--------------------------  Prototypes  -----------------------------*/
/*---------------------------------------------------------------------*/

/* DT-Open Layers */




/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* OLFG_H__ */

