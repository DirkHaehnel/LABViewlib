/*	File:	olimgapi.h

	Description:
		This file contains the public interface for the DT-Open
	Layers Imaging Device Application Programming Interface.  This interface
	provides device dependent and operating system independent access
	to Data Translation imaging devices.



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

#if !defined(OLIMGAPI_H__)
#define OLIMGAPI_H__ 1

#include "olwintyp.h"


/* Assume C declarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif


/* Define the calling convention used by this API */
#define OLIMGAPI PUBLIC

/*=======================================================================*/
/*==================  Status Code Definitions  ==========================*/
/*=======================================================================*/

// DT_MIDL_PASS is only defined when compiling a typelib via an idl file (e.g project Ruby).
// It is used here so that the standard standard SDK doesnt change while providing appropriate types for typelibs.
// Eventually we may want to eliminate the old types.

#ifdef DT_MIDL_PASS

// For typelibs, define the status codes to be custom HRESULTS, which are required to be
// in the 0x80040200 - 0x8004ffff range. This is done by moving the severity mask into 0x7000
// and using OLC_STATUS_IS_APISTATUS_MASK and OLC_STATUS_IS_OL_MASK to generate 0x80040000.
// The intention is to leave as much of the original code intact as possible.

/* Masks for parsing status values */
#define OLC_STATUS_CODE_MASK            0x00000fff // stays the same
#define OLC_STATUS_SEV_MASK             0x00007000 // moved into the 0x1000 to 0x4000 range
#define OLC_STATUS_IS_APISTATUS_MASK    0x00040000 // no longer related to OL, but used to build a valid HRESULT
#define OLC_STATUS_IS_OL_MASK           0x80000000 // no longer related to OL, but used to build a valid HRESULT

/* Status Code Severity Levels - bit masks */
#define OLC_STATUS_SEV_SHIFT		12             // the severity mask is now 0x7000
#define OLC_STATUS_SEV_ERROR_MASK	( (ULNG) (((ULNG)OLC_IMG_SEV_ERROR) << OLC_STATUS_SEV_SHIFT) )
#define OLC_STATUS_SEV_WARNING_MASK	( (ULNG) (((ULNG)OLC_IMG_SEV_WARNING) << OLC_STATUS_SEV_SHIFT) )
#define OLC_STATUS_SEV_INFO_MASK	( (ULNG) (((ULNG)OLC_IMG_SEV_INFO) << OLC_STATUS_SEV_SHIFT) )


/* Macros for creating DT-Open Layers API status codes */
#define OL_MAKE_APISTATUS(Sev, Code, OlMask)							\
		( ( ((ULNG)(Sev) & OLC_STATUS_SEV_MASK) |			\
				    ((ULNG)(Code) & OLC_STATUS_CODE_MASK) |			\
				    (ULNG) (OlMask) |						\
				    OLC_STATUS_IS_APISTATUS_MASK ) )

#define OL_MAKE_OL_ERROR_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_ERROR_MASK, (Code), OLC_STATUS_IS_OL_MASK) )
#define OL_MAKE_OL_INFO_APISTATUS(Code)		( OL_MAKE_APISTATUS(OLC_STATUS_SEV_INFO_MASK, (Code), OLC_STATUS_IS_OL_MASK) )
#define OL_MAKE_OL_WARNING_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_WARNING_MASK, (Code), OLC_STATUS_IS_OL_MASK) )


/* Macros for creating non-OL status codes with OL format */
#define OL_MAKE_NONOL_ERROR_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_ERROR_MASK, (Code), 0L) )
#define OL_MAKE_NONOL_INFO_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_INFO_MASK, (Code), 0L) )
#define OL_MAKE_NONOL_WARNING_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_WARNING_MASK, (Code), 0L) )


/* Status Code data type */

typedef enum OLT_APISTATUS
{
    OLC_STS_NORMAL      = 0L,

	/* General DT-Open Layers Informational status codes (0x1 -> 0xff  */
    OLC_STS_PENDING		= OL_MAKE_OL_INFO_APISTATUS(0x1),	 /* Job is pending and has not started executing */
    OLC_STS_ACTIVE		= OL_MAKE_OL_INFO_APISTATUS(0x2),	 /* Job has started executing, but has not completed */
    OLC_STS_CANCELJOB	= OL_MAKE_OL_INFO_APISTATUS(0x3),	 /* Job was canceled prior to completion */

	/* General DT-Open Layers warnings (0x1 -> 0xff, */
    OLC_STS_CLIP		= OL_MAKE_OL_WARNING_APISTATUS(0x1), /* A data value exceeded the legal range and was */
    OLC_STS_NONOLMSG	= OL_MAKE_OL_WARNING_APISTATUS(0x2), /* A unit opened for DT-Open Layers received a message  */
													         /*     that was not handled.  The message was passed to */
													         /*     DefDriverProc(,.                                 */
    OLC_STS_LOADSTRWARN	= OL_MAKE_OL_WARNING_APISTATUS(0x3), /* LoadString failed, unable to load intended string.  Default string used. */

	/* DT-Open Layers general errors (0x1 -> 0xff) */
    OLC_STS_NOSHARE		    = OL_MAKE_OL_ERROR_APISTATUS(0x1), /* Device is in use and not shareable */
    OLC_STS_NOMEM		    = OL_MAKE_OL_ERROR_APISTATUS(0x2), /* Unable to allocate required memory */
    OLC_STS_NOMEMLOCK	    = OL_MAKE_OL_ERROR_APISTATUS(0x3), /* Unable to lock down required memory */
    OLC_STS_RANGE		    = OL_MAKE_OL_ERROR_APISTATUS(0x4), /* Argument out of range */
    OLC_STS_STRUCTSIZ	    = OL_MAKE_OL_ERROR_APISTATUS(0x5), /* Structure is wrong size */
    OLC_STS_NULL		    = OL_MAKE_OL_ERROR_APISTATUS(0x6), /* Attempt to follow NULL pointer or HANDLE */
    OLC_STS_BUSY		    = OL_MAKE_OL_ERROR_APISTATUS(0x7), /* Device is BUSY and can not process requested */
													           /*    operation                                 */
    OLC_STS_BUFSIZ		    = OL_MAKE_OL_ERROR_APISTATUS(0x8), /* Output buffer was not the correct size */
    OLC_STS_UNSUPKEY	    = OL_MAKE_OL_ERROR_APISTATUS(0x9), /* Unsupported Key Indicator */
    OLC_STS_NOASYNC		    = OL_MAKE_OL_ERROR_APISTATUS(0xa), /* Unable to accept asynchronous I/O request - queue is */
													           /*    probably full                                     */
    OLC_STS_TIMEOUT		    = OL_MAKE_OL_ERROR_APISTATUS(0xb), /* Operation timed out */
    OLC_STS_GRANULARITY	    = OL_MAKE_OL_ERROR_APISTATUS(0xc), /* Argument within linear range, but not on legal */
													           /*    increment                                   */
    OLC_STS_NODRIVERS	    = OL_MAKE_OL_ERROR_APISTATUS(0xd), /* No OL imaging devices installed in system */
    OLC_STS_NOOPENDEVICE	= OL_MAKE_OL_ERROR_APISTATUS(0xe), /* Unable to open required device driver */
    OLC_STS_NOCLOSEDEVICE	= OL_MAKE_OL_ERROR_APISTATUS(0xf), /* Unable to close specified device driver */
    OLC_STS_GETSTATUSFAIL	= OL_MAKE_OL_ERROR_APISTATUS(0x10),/* Unable to retreive status from specified device driver */
    OLC_STS_NONOLSTATUS	    = OL_MAKE_OL_ERROR_APISTATUS(0x11),/* The specified status was not an OL status code and could not be translated */
    OLC_STS_UNKNOWNSTATUS	= OL_MAKE_OL_ERROR_APISTATUS(0x12),/* The specified status appears to be an unknown OL status */
    OLC_STS_LOADSTRERR	    = OL_MAKE_OL_ERROR_APISTATUS(0x13),/* LoadString failed, unable to load required string. */
    OLC_STS_SYSERROR	    = OL_MAKE_OL_ERROR_APISTATUS(0x14),/* Internal driver error. */
    OLC_STS_FIFO_OVERFLOW	= OL_MAKE_OL_ERROR_APISTATUS(0x15),/* Internal FIFO overflow. */
    OLC_STS_FIELD_OVERFLOW	= OL_MAKE_OL_ERROR_APISTATUS(0x16),/* Internal field overflow. */

	/* DT-Open Layers frame grabber errors (0x100 -> 0x1ff, */
    OLC_STS_UNSUPMEMTYPE		= OL_MAKE_OL_ERROR_APISTATUS(0x100),/* Memory type not known or supported by this */
											/*    driver                                  */
    OLC_STS_FRAMENOTAVAILABLE	= OL_MAKE_OL_ERROR_APISTATUS(0x102),/* Frame not available */
    OLC_STS_INVALIDFRAMEID		= OL_MAKE_OL_ERROR_APISTATUS(0x103),/* Frame identifier is invalid */
    OLC_STS_INVALIDFRAMEHANDLE	= OL_MAKE_OL_ERROR_APISTATUS(0x104),/* Frame handle is not valid */
    OLC_STS_INVALIDFRAMERECT	= OL_MAKE_OL_ERROR_APISTATUS(0x105),/* Invalid frame rectangle */
    OLC_STS_FRAMENOTALLOCATED	= OL_MAKE_OL_ERROR_APISTATUS(0x106),/* Frame not allocated */
    OLC_STS_MAPERROR			= OL_MAKE_OL_ERROR_APISTATUS(0x107),/* Unable to map frame */
    OLC_STS_UNMAPERROR			= OL_MAKE_OL_ERROR_APISTATUS(0x108),/* Unable to unmap frame */
    OLC_STS_FRAMEISMAPPED		= OL_MAKE_OL_ERROR_APISTATUS(0x109),/* Frame is currently mapped */
    OLC_STS_FRAMENOTMAPPED		= OL_MAKE_OL_ERROR_APISTATUS(0x10a),/* Frame is not mapped */
    OLC_STS_FRAMELIMITEXCEEDED	= OL_MAKE_OL_ERROR_APISTATUS(0x10b),/* Frame boundary exceeded */
    OLC_STS_FRAMEWIDTH			= OL_MAKE_OL_ERROR_APISTATUS(0x10c),/* Frame width is illegal for current */
															        /*    acquisition setup               */
    OLC_STS_CLAMP				= OL_MAKE_OL_ERROR_APISTATUS(0x10d),/* Clamp area is illegal for current */
															        /*    acquisition setup              */
    OLC_STS_VERTICALINC			= OL_MAKE_OL_ERROR_APISTATUS(0x10e),/* Vertical frame increment is illegal for */
															        /*    current acquisition setup            */
    OLC_STS_FIRSTACTPIX			= OL_MAKE_OL_ERROR_APISTATUS(0x10f),/* First active pixel position is illegal for */
															        /*    current acquisition setup               */
    OLC_STS_ACTPIXCOUNT			= OL_MAKE_OL_ERROR_APISTATUS(0x110),/* Active pixel count is illegal for current */
															        /*     acquisition setup                     */
    OLC_STS_FRAMELEFT			= OL_MAKE_OL_ERROR_APISTATUS(0x111),/* Left side of frame is illegal for current */
															        /*     acquisition setup                     */
    OLC_STS_FRAMETOP			= OL_MAKE_OL_ERROR_APISTATUS(0x112),/* Top of frame is illegal for current */
															        /*    acquisition setup                */
    OLC_STS_FRAMEHEIGHT			= OL_MAKE_OL_ERROR_APISTATUS(0x113),/* Frame height is illegal for current */
															        /*    acquisition setup                */
    OLC_STS_ACTLINECOUNT		= OL_MAKE_OL_ERROR_APISTATUS(0x114),/* Active line count is illegal for current */
															        /*     acquisition setup                    */
    OLC_STS_HSYNCSEARCHPOS		= OL_MAKE_OL_ERROR_APISTATUS(0x115),/* Horizontal sync search position is illegal */
															        /*     for current acquisition setup          */
    OLC_STS_VSYNCSEARCHPOS		= OL_MAKE_OL_ERROR_APISTATUS(0x116),/* Vertical sync search position is illegal */
															        /*     for current acquisition setup        */
    OLC_STS_INPUTSOURCE			= OL_MAKE_OL_ERROR_APISTATUS(0x117),/* Returned if input source channel out of */
															        /*    range                                */
    OLC_STS_CONTROL       		= OL_MAKE_OL_ERROR_APISTATUS(0x118),/* Returned if set input control function value */
															        /*     is undefined.                            */
    OLC_STS_LUT           		= OL_MAKE_OL_ERROR_APISTATUS(0x119),	/* Returned if LUT value out of range */
    OLC_STS_BWINVERSION			= OL_MAKE_OL_ERROR_APISTATUS(0x11a),/* Returned if Black Level > White Level */
    OLC_STS_WHITELEVEL			= OL_MAKE_OL_ERROR_APISTATUS(0x11b),/* Returned if white level cannot be set */
    OLC_STS_INTERLACEDHGTGRAN	= OL_MAKE_OL_ERROR_APISTATUS(0x11c),/* Returned if frame height granularity is   */
														        	/*     illegal when frame type is interlaced */
    OLC_STS_INTERLACEDTOPGRAN	= OL_MAKE_OL_ERROR_APISTATUS(0x11d),/* Returned if frame top granularity is illegal */
															        /*     when frame type is interlaced            */
    OLC_STS_INVALIDJOBHANDLE	= OL_MAKE_OL_ERROR_APISTATUS(0x11e),/* Returned if job handle is invalid */
    OLC_STS_MODECONFLICT		= OL_MAKE_OL_ERROR_APISTATUS(0x11f),/* Returned if attempted operation conflicts with current mode of operation  */
    OLC_STS_INVALIDHWND			= OL_MAKE_OL_ERROR_APISTATUS(0x120),/* Invalid window handle */
    OLC_STS_INVALIDWNDALIGN		= OL_MAKE_OL_ERROR_APISTATUS(0x121),/* Invalid window alignment */
    OLC_STS_PALETTESIZE			= OL_MAKE_OL_ERROR_APISTATUS(0x122),/* Invalid system palette size */
    OLC_STS_NODCI				= OL_MAKE_OL_ERROR_APISTATUS(0x123),/* DCI could not be properly accessed */
    OLC_STS_PASSTHRULUTRANGE	= OL_MAKE_OL_ERROR_APISTATUS(0x124),/* Invalid range passed to PMLut */
    OLC_STS_PASSTHRUPALRANGE    = OL_MAKE_OL_ERROR_APISTATUS(0x125),/* Invalid range passed to extend palette during passthru */


#ifndef EXCLUDE_DDI
	/* DT-Open Layers Frame Grabber DDI Error status codes (0x126 -> 0x131, */
    OLC_STS_SYS_RES					= OL_MAKE_OL_ERROR_APISTATUS(0x126), /* System resource error */
    OLC_STS_INVALID_SURFACE_HANDLE	= OL_MAKE_OL_ERROR_APISTATUS(0x127), /* Surface Handle invalid */
    OLC_STS_FIXED_COLOR				= OL_MAKE_OL_ERROR_APISTATUS(0x128), /* Key color can't be changed */
    OLC_STS_INVALID_FLAGS			= OL_MAKE_OL_ERROR_APISTATUS(0x129), /* Some of the flags are illegal */
    OLC_STS_NO_MORE_SURFACE			= OL_MAKE_OL_ERROR_APISTATUS(0x12A), /* Driver's Max surfaces reached */
    OLC_STS_PASSTHRU_STOPPED		= OL_MAKE_OL_ERROR_APISTATUS(0x12B), /* Not in passthru mode */
    OLC_STS_NO_DDI					= OL_MAKE_OL_ERROR_APISTATUS(0x12C), /* DDI not supported */
    OLC_STS_SURFACE_TOO_SMALL		= OL_MAKE_OL_ERROR_APISTATUS(0x12D), /* Surface chosen was too small */
    OLC_STS_PITCH_TOO_SMALL			= OL_MAKE_OL_ERROR_APISTATUS(0x12E), /* Pitch declared was too small */
    OLC_STS_NO_IMAGE_IN_FRAME		= OL_MAKE_OL_ERROR_APISTATUS(0x12F), /* Pitch declared was too small */
    OLC_STS_INVALID_SURFACE_DC		= OL_MAKE_OL_ERROR_APISTATUS(0x130), /* Surface Handle DC */
    OLC_STS_SURFACE_NOT_SET			= OL_MAKE_OL_ERROR_APISTATUS(0x131), /* Surface selected yet */
#endif // EXCLUDE_DDI

    OLC_STS_NO_VIDEO_SIGNAL     = OL_MAKE_OL_ERROR_APISTATUS(0x132),


} OLT_APISTATUS, *POLT_APISTATUS, *LPOLT_APISTATUS;

#else // DT_MIDL_PASS

typedef ULNG OLT_APISTATUS;
typedef OLT_APISTATUS FAR	*POLT_APISTATUS;
typedef OLT_APISTATUS FAR	*LPOLT_APISTATUS;

/* Masks for parsing status values */
#define OLC_STATUS_CODE_MASK		0x00000fff
#define OLC_STATUS_SEV_MASK		0x07000000
#define OLC_STATUS_IS_APISTATUS_MASK	0x10000000
#define OLC_STATUS_IS_OL_MASK		0x80000000

/* Status Code Severity Levels - bit masks */
#define OLC_STATUS_SEV_SHIFT		24
#define OLC_STATUS_SEV_ERROR_MASK	( (ULNG) (((ULNG)OLC_IMG_SEV_ERROR) << OLC_STATUS_SEV_SHIFT) )
#define OLC_STATUS_SEV_WARNING_MASK	( (ULNG) (((ULNG)OLC_IMG_SEV_WARNING) << OLC_STATUS_SEV_SHIFT) )
#define OLC_STATUS_SEV_INFO_MASK	( (ULNG) (((ULNG)OLC_IMG_SEV_INFO) << OLC_STATUS_SEV_SHIFT) )


/* Macros for creating DT-Open Layers API status codes */
#define OL_MAKE_APISTATUS(Sev, Code, OlMask)							\
		( (OLT_APISTATUS) ( ((ULNG)(Sev) & OLC_STATUS_SEV_MASK) |			\
				    ((ULNG)(Code) & OLC_STATUS_CODE_MASK) |			\
				    (ULNG) (OlMask) |						\
				    OLC_STATUS_IS_APISTATUS_MASK ) )

#define OL_MAKE_OL_ERROR_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_ERROR_MASK, (Code), OLC_STATUS_IS_OL_MASK) )
#define OL_MAKE_OL_INFO_APISTATUS(Code)		( OL_MAKE_APISTATUS(OLC_STATUS_SEV_INFO_MASK, (Code), OLC_STATUS_IS_OL_MASK) )
#define OL_MAKE_OL_WARNING_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_WARNING_MASK, (Code), OLC_STATUS_IS_OL_MASK) )


/* Macros for creating non-OL status codes with OL format */
#define OL_MAKE_NONOL_ERROR_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_ERROR_MASK, (Code), 0L) )
#define OL_MAKE_NONOL_INFO_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_INFO_MASK, (Code), 0L) )
#define OL_MAKE_NONOL_WARNING_APISTATUS(Code)	( OL_MAKE_APISTATUS(OLC_STATUS_SEV_WARNING_MASK, (Code), 0L) )



/* Normal completion status */
#define OLC_STS_NORMAL		0L


/* DT-Open Layers general errors (0x1 -> 0xff) */
#define OLC_STS_NOSHARE		OL_MAKE_OL_ERROR_APISTATUS(0x1)		/* Device is in use and not shareable */
#define OLC_STS_NOMEM		OL_MAKE_OL_ERROR_APISTATUS(0x2)		/* Unable to allocate required memory */
#define OLC_STS_NOMEMLOCK	OL_MAKE_OL_ERROR_APISTATUS(0x3)		/* Unable to lock down required memory */
#define OLC_STS_RANGE		OL_MAKE_OL_ERROR_APISTATUS(0x4)		/* Argument out of range */
#define OLC_STS_STRUCTSIZ	OL_MAKE_OL_ERROR_APISTATUS(0x5)		/* Structure is wrong size */
#define OLC_STS_NULL		OL_MAKE_OL_ERROR_APISTATUS(0x6)		/* Attempt to follow NULL pointer or HANDLE */
#define OLC_STS_BUSY		OL_MAKE_OL_ERROR_APISTATUS(0x7)		/* Device is BUSY and can not process requested */
									/*    operation                                 */
#define OLC_STS_BUFSIZ		OL_MAKE_OL_ERROR_APISTATUS(0x8)		/* Output buffer was not the correct size */
#define OLC_STS_UNSUPKEY	OL_MAKE_OL_ERROR_APISTATUS(0x9)		/* Unsupported Key Indicator */
#define OLC_STS_NOASYNC		OL_MAKE_OL_ERROR_APISTATUS(0xa)		/* Unable to accept asynchronous I/O request - queue is */
									/*    probably full                                     */
#define OLC_STS_TIMEOUT		OL_MAKE_OL_ERROR_APISTATUS(0xb)		/* Operation timed out */
#define OLC_STS_GRANULARITY	OL_MAKE_OL_ERROR_APISTATUS(0xc)		/* Argument within linear range, but not on legal */
									/*    increment                                   */
#define OLC_STS_NODRIVERS	OL_MAKE_OL_ERROR_APISTATUS(0xd)		/* No OL imaging devices installed in system */
#define OLC_STS_NOOPENDEVICE	OL_MAKE_OL_ERROR_APISTATUS(0xe)		/* Unable to open required device driver */
#define OLC_STS_NOCLOSEDEVICE	OL_MAKE_OL_ERROR_APISTATUS(0xf)		/* Unable to close specified device driver */
#define OLC_STS_GETSTATUSFAIL	OL_MAKE_OL_ERROR_APISTATUS(0x10)	/* Unable to retreive status from specified device driver */
#define OLC_STS_NONOLSTATUS	OL_MAKE_OL_ERROR_APISTATUS(0x11)	/* The specified status was not an OL status code and could not be translated */
#define OLC_STS_UNKNOWNSTATUS	OL_MAKE_OL_ERROR_APISTATUS(0x12)	/* The specified status appears to be an unknown OL status */
#define OLC_STS_LOADSTRERR	OL_MAKE_OL_ERROR_APISTATUS(0x13)	/* LoadString failed, unable to load required string. */
#define OLC_STS_SYSERROR	OL_MAKE_OL_ERROR_APISTATUS(0x14)	/* Internal driver error. */
#define OLC_STS_FIFO_OVERFLOW	OL_MAKE_OL_ERROR_APISTATUS(0x15)	/* Internal FIFO overflow. */
#define OLC_STS_FIELD_OVERFLOW	OL_MAKE_OL_ERROR_APISTATUS(0x16)	/* Internal field overflow. */


/* General DT-Open Layers Informational status codes (0x1 -> 0xff) */
#define OLC_STS_PENDING		OL_MAKE_OL_INFO_APISTATUS(0x1)	/* Job is pending and has not started executing */
#define OLC_STS_ACTIVE		OL_MAKE_OL_INFO_APISTATUS(0x2)	/* Job has started executing, but has not completed */
#define OLC_STS_CANCELJOB	OL_MAKE_OL_INFO_APISTATUS(0x3)	/* Job was canceled prior to completion */


/* General DT-Open Layers warnings (0x1 -> 0xff) */
#define OLC_STS_CLIP		OL_MAKE_OL_WARNING_APISTATUS(0x1)	/* A data value exceeded the legal range and was */
#define OLC_STS_NONOLMSG	OL_MAKE_OL_WARNING_APISTATUS(0x2)	/* A unit opened for DT-Open Layers received a message  */
									/*     that was not handled.  The message was passed to */
									/*     DefDriverProc().                                 */
#define OLC_STS_LOADSTRWARN	OL_MAKE_OL_WARNING_APISTATUS(0x3)	/* LoadString failed, unable to load intended string.  Default string used. */




/* DT-Open Layers frame grabber errors (0x100 -> 0x1ff) */
#define OLC_STS_UNSUPMEMTYPE		OL_MAKE_OL_ERROR_APISTATUS(0x100)	/* Memory type not known or supported by this */
										/*    driver                                  */
#define OLC_STS_FRAMENOTAVAILABLE	OL_MAKE_OL_ERROR_APISTATUS(0x102)	/* Frame not available */
#define OLC_STS_INVALIDFRAMEID		OL_MAKE_OL_ERROR_APISTATUS(0x103)	/* Frame identifier is invalid */
#define OLC_STS_INVALIDFRAMEHANDLE	OL_MAKE_OL_ERROR_APISTATUS(0x104)	/* Frame handle is not valid */
#define OLC_STS_INVALIDFRAMERECT	OL_MAKE_OL_ERROR_APISTATUS(0x105)	/* Invalid frame rectangle */
#define OLC_STS_FRAMENOTALLOCATED	OL_MAKE_OL_ERROR_APISTATUS(0x106)	/* Frame not allocated */
#define OLC_STS_MAPERROR		OL_MAKE_OL_ERROR_APISTATUS(0x107)	/* Unable to map frame */
#define OLC_STS_UNMAPERROR		OL_MAKE_OL_ERROR_APISTATUS(0x108)	/* Unable to unmap frame */
#define OLC_STS_FRAMEISMAPPED		OL_MAKE_OL_ERROR_APISTATUS(0x109)	/* Frame is currently mapped */
#define OLC_STS_FRAMENOTMAPPED		OL_MAKE_OL_ERROR_APISTATUS(0x10a)	/* Frame is not mapped */
#define OLC_STS_FRAMELIMITEXCEEDED	OL_MAKE_OL_ERROR_APISTATUS(0x10b)	/* Frame boundary exceeded */
#define OLC_STS_FRAMEWIDTH		OL_MAKE_OL_ERROR_APISTATUS(0x10c)	/* Frame width is illegal for current */
										/*    acquisition setup               */
#define OLC_STS_CLAMP			OL_MAKE_OL_ERROR_APISTATUS(0x10d)	/* Clamp area is illegal for current */
										/*    acquisition setup              */
#define OLC_STS_VERTICALINC		OL_MAKE_OL_ERROR_APISTATUS(0x10e)	/* Vertical frame increment is illegal for */
										/*    current acquisition setup            */
#define OLC_STS_FIRSTACTPIX		OL_MAKE_OL_ERROR_APISTATUS(0x10f)	/* First active pixel position is illegal for */
                                                                                /*    current acquisition setup               */
#define OLC_STS_ACTPIXCOUNT		OL_MAKE_OL_ERROR_APISTATUS(0x110)	/* Active pixel count is illegal for current */
										/*     acquisition setup                     */
#define OLC_STS_FRAMELEFT		OL_MAKE_OL_ERROR_APISTATUS(0x111)	/* Left side of frame is illegal for current */
										/*     acquisition setup                     */
#define OLC_STS_FRAMETOP		OL_MAKE_OL_ERROR_APISTATUS(0x112)	/* Top of frame is illegal for current */
                                                                                /*    acquisition setup                */
#define OLC_STS_FRAMEHEIGHT		OL_MAKE_OL_ERROR_APISTATUS(0x113)	/* Frame height is illegal for current */
                                                                                /*    acquisition setup                */
#define OLC_STS_ACTLINECOUNT		OL_MAKE_OL_ERROR_APISTATUS(0x114)	/* Active line count is illegal for current */
										/*     acquisition setup                    */
#define OLC_STS_HSYNCSEARCHPOS		OL_MAKE_OL_ERROR_APISTATUS(0x115)	/* Horizontal sync search position is illegal */
										/*     for current acquisition setup          */
#define OLC_STS_VSYNCSEARCHPOS		OL_MAKE_OL_ERROR_APISTATUS(0x116)	/* Vertical sync search position is illegal */
										/*     for current acquisition setup        */
#define OLC_STS_INPUTSOURCE   		OL_MAKE_OL_ERROR_APISTATUS(0x117)	/* Returned if input source channel out of */
                                                                                /*    range                                */
#define OLC_STS_CONTROL       		OL_MAKE_OL_ERROR_APISTATUS(0x118)	/* Returned if set input control function value */
										/*     is undefined.                            */
#define OLC_STS_LUT           		OL_MAKE_OL_ERROR_APISTATUS(0x119)	/* Returned if LUT value out of range */
#define OLC_STS_BWINVERSION           	OL_MAKE_OL_ERROR_APISTATUS(0x11a)	/* Returned if Black Level > White Level */
#define OLC_STS_WHITELEVEL           	OL_MAKE_OL_ERROR_APISTATUS(0x11b)	/* Returned if white level cannot be set */
#define OLC_STS_INTERLACEDHGTGRAN	OL_MAKE_OL_ERROR_APISTATUS(0x11c)	/* Returned if frame height granularity is   */
										/*     illegal when frame type is interlaced */
#define OLC_STS_INTERLACEDTOPGRAN	OL_MAKE_OL_ERROR_APISTATUS(0x11d)	/* Returned if frame top granularity is illegal */
										/*     when frame type is interlaced            */
#define OLC_STS_INVALIDJOBHANDLE	OL_MAKE_OL_ERROR_APISTATUS(0x11e)	/* Returned if job handle is invalid */
#define OLC_STS_MODECONFLICT		OL_MAKE_OL_ERROR_APISTATUS(0x11f)	/* Returned if attempted operation conflicts with current mode of operation  */
#define OLC_STS_INVALIDHWND		OL_MAKE_OL_ERROR_APISTATUS(0x120)	/* Invalid window handle */
#define OLC_STS_INVALIDWNDALIGN	OL_MAKE_OL_ERROR_APISTATUS(0x121)	/* Invalid window alignment */
#define OLC_STS_PALETTESIZE		OL_MAKE_OL_ERROR_APISTATUS(0x122)	/* Invalid system palette size */
#define OLC_STS_NODCI			OL_MAKE_OL_ERROR_APISTATUS(0x123)	/* DCI could not be properly accessed */
#define OLC_STS_PASSTHRULUTRANGE	OL_MAKE_OL_ERROR_APISTATUS(0x124) /* Invalid range passed to PMLut */
#define OLC_STS_PASSTHRUPALRANGE  OL_MAKE_OL_ERROR_APISTATUS(0x125) /* Invalid range passed to extend palette during passthru */


#ifndef EXCLUDE_DDI

/* DT-Open Layers Frame Grabber DDI Error status codes (0x126 -> 0x131) */
#define OLC_STS_SYS_RES					OL_MAKE_OL_ERROR_APISTATUS(0x126) /* System resource error */
#define OLC_STS_INVALID_SURFACE_HANDLE	OL_MAKE_OL_ERROR_APISTATUS(0x127) /* Surface Handle invalid */
#define OLC_STS_FIXED_COLOR				OL_MAKE_OL_ERROR_APISTATUS(0x128) /* Key color can't be changed */
#define OLC_STS_INVALID_FLAGS			OL_MAKE_OL_ERROR_APISTATUS(0x129) /* Some of the flags are illegal */
#define OLC_STS_NO_MORE_SURFACE			OL_MAKE_OL_ERROR_APISTATUS(0x12A) /* Driver's Max surfaces reached */
#define OLC_STS_PASSTHRU_STOPPED		OL_MAKE_OL_ERROR_APISTATUS(0x12B) /* Not in passthru mode */
#define OLC_STS_NO_DDI					OL_MAKE_OL_ERROR_APISTATUS(0x12C) /* DDI not supported */
#define OLC_STS_SURFACE_TOO_SMALL		OL_MAKE_OL_ERROR_APISTATUS(0x12D) /* Surface chosen was too small */
#define OLC_STS_PITCH_TOO_SMALL			OL_MAKE_OL_ERROR_APISTATUS(0x12E) /* Pitch declared was too small */
#define OLC_STS_NO_IMAGE_IN_FRAME		OL_MAKE_OL_ERROR_APISTATUS(0x12F) /* Pitch declared was too small */
#define OLC_STS_INVALID_SURFACE_DC		OL_MAKE_OL_ERROR_APISTATUS(0x130) /* Surface Handle DC */
#define OLC_STS_SURFACE_NOT_SET			OL_MAKE_OL_ERROR_APISTATUS(0x131) /* Surface selected yet */

#define OLC_STS_NO_VIDEO_SIGNAL   OL_MAKE_OL_ERROR_APISTATUS(0x132) /* No video was detected on the front end */

#endif

#endif // DT_MIDL_PASS


/* DT-Open Layers Frame Grabber informational status codes (0x100 -> 0x1ff) */





/* The following macros can be used to test successful completion of OLIMGAPI functions */
#define OlImgIsOkay(sc)		( (sc) == OLC_STS_NORMAL )
#define OlImgIsError(sc)	( ((sc) & OLC_STATUS_SEV_ERROR_MASK) == OLC_STATUS_SEV_ERROR_MASK )
#define OlImgIsWarning(sc)	( ((sc) & OLC_STATUS_SEV_WARNING_MASK) == OLC_STATUS_SEV_WARNING_MASK )
#define OlImgIsInfo(sc)		( ((sc) & OLC_STATUS_SEV_INFO_MASK) == OLC_STATUS_SEV_INFO_MASK )




/*=======================================================================*/
/*========================  Type Defintitions  ==========================*/
/*=======================================================================*/

/* "IDs" used by imaging devices */
#define DECLARE_OL_IMG_ID(type)		DECLARE_OL_HDL(IMG, type, ID)

/* Imaging Device ID */
DECLARE_OL_IMG_ID(DEV);

/* Image Device Info */
#define OLC_MAX_ALIAS_STR_SIZE		20
#define OLC_MAX_DEVICE_NAME_STR_SIZE	20

#ifndef DT_MIDL_PASS
typedef struct olOLT_IMGDEVINFO
{
	 ULNG StructSize;
	 OLT_IMGDEVTYPE DeviceType;
	 ULNG DeviceId;
	 char Alias[OLC_MAX_ALIAS_STR_SIZE];
	 char DeviceName[OLC_MAX_DEVICE_NAME_STR_SIZE];
	 // The following 2 items are recent additions to allow the user to query memory sucessfully allocated by the driver
     ULNG HostDeviceMemSize;   // Size of the Device linear memory in bytes
     ULNG HostLinearMemSize;   // Size of the host linear memory in bytes
} OLT_IMGDEVINFO, *POLT_IMGDEVINFO, FAR *LPOLT_IMGDEVINFO;
#endif // DT_MIDL_PASS


/* Max size for a status message */
#define OLC_MAX_STATUS_MESSAGE_SIZE	256




/*=======================================================================*/
/*=======================================================================*/
/*=======================  Prototypes  ==================================*/
/*=======================================================================*/
/*=======================================================================*/

#ifndef DT_MIDL_PASS
OLT_APISTATUS OLIMGAPI OlImgCloseDevice(OLT_IMG_DEV_ID DeviceId);
OLT_APISTATUS OLIMGAPI OlImgGetDeviceCount(LPINT lpCount);
OLT_APISTATUS OLIMGAPI OlImgGetDeviceInfo(LPOLT_IMGDEVINFO lpDevList, ULNG ulListSize);
OLT_APISTATUS OLIMGAPI OlImgGetStatusMessage(OLT_APISTATUS Status, LPSTR lpMessageBuf, int iBufSize);
OLT_APISTATUS OLIMGAPI OlImgOpenDevice(LPCSTR lpcszAlias, LPOLT_IMG_DEV_ID lpDevId);
OLT_APISTATUS OLIMGAPI OlImgReset(OLT_IMG_DEV_ID DeviceId);
OLT_APISTATUS OLIMGAPI OlImgQueryDeviceCaps(OLT_IMG_DEV_ID DeviceId, OLT_DEV_CAP_KEY Key, LPVOID lpData, ULNG ulDataSize);
OLT_APISTATUS OLIMGAPI OlImgQueryTimeoutPeriod(OLT_IMG_DEV_ID DeviceId, LPULNG lpPeriod);
OLT_APISTATUS OLIMGAPI OlImgSetTimeoutPeriod(OLT_IMG_DEV_ID DeviceId, ULNG ulPeriod, LPULNG lpPeriod);
#endif






/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif


#endif   /* OLIMGAPI_H__*/

