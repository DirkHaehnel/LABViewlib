/*	File:	olimgdev.h

	Description:
		This file contains macros, data types and prototypes for
	the DT-Open Layers Imaging Device Interface for Windows 3.1.  The definitions
	that appear in this file are generic to all DT-Open Layers Imaging devices.

	
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

#if !defined(OLIMGDEV_H__)
#define OLIMGDEV_H__


/* Include required include DT-Open Layers include files */
#include "olwintyp.h"


/* Assume C delclarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif



/*---------------------------------------------------------------------*/
/*--------------  General DT-Open Layers Data Types, etc.  ------------*/
/*---------------------------------------------------------------------*/

/* Status Code data type */
typedef ULNG OLT_SCODE;
typedef OLT_SCODE FAR	*POLT_SCODE;
typedef OLT_SCODE FAR	*LPOLT_SCODE;

/* Union for retreiving parts of status code */
typedef union olOLT_SCODE_PARTS
	{
	 OLT_SCODE scStatus;
	 struct olParts
	 	{
	 	 unsigned int Code : 16;
	 	 unsigned int : 8;
	 	 unsigned int Severity : 3;
	 	 unsigned int : 4;
	 	 unsigned int IsOl : 1;
	 	};
	} OLT_SCODE_PARTS;




/*---------------------------------------------------------------------*/
/*-------------------  DT-Open Layers Status Codes  -------------------*/
/*---------------------------------------------------------------------*/

/* Status Code Owners - bit masks */
#define OLC_SC_OWNER_SHIFT		31
#define OLC_SC_OWNER_MASK		0x80000000

/* Status Code Severity Levels - bit masks */
#define OLC_SC_SEV_SHIFT		24
#define OLC_SEV_INFO			( (ULNG) (((ULNG)OLC_IMG_SEV_ERROR) << OLC_SC_SEV_SHIFT) )
#define OLC_SEV_WARNING			( (ULNG) (((ULNG)OLC_IMG_SEV_WARNING) << OLC_SC_SEV_SHIFT) )
#define OLC_SEV_ERROR			( (ULNG) (((ULNG)OLC_IMG_SEV_INFO) << OLC_SC_SEV_SHIFT) )
#define OLC_SEV_MASK			0x07000000


/* Macros for creating DT-Open Layers status codes */
#define OL_MAKE_SCODE(Own, Sev, Code)		( (OLT_SCODE) ( (((ULNG)(Own) & 0x00000001) << OLC_SC_OWNER_SHIFT) |	\
								(((ULNG)(Sev) & 0x00000007) << OLC_SC_SEV_SHIFT) |	\
								(((ULNG)(Code) & 0x0000ffff)) ) )

#define OL_MAKE_OL_ERROR_SCODE(Code)		( OL_MAKE_SCODE(1, OLC_IMG_SEV_ERROR, (Code)) )
#define OL_MAKE_OL_INFO_SCODE(Code)		( OL_MAKE_SCODE(1, OLC_IMG_SEV_INFO, (Code)) )
#define OL_MAKE_OL_WARNING_SCODE(Code)		( OL_MAKE_SCODE(1, OLC_IMG_SEV_WARNING, (Code)) )


/* Macros for creating non-OL status codes with OL format */
#define OL_MAKE_NONOL_ERROR_SCODE(Code)		( OL_MAKE_SCODE(0, OLC_IMG_SEV_ERROR, (Code)) )
#define OL_MAKE_NONOL_INFO_SCODE(Code)		( OL_MAKE_SCODE(0, OLC_IMG_SEV_INFO, (Code)) )
#define OL_MAKE_NONOL_WARNING_SCODE(Code)	( OL_MAKE_SCODE(0, OLC_IMG_SEV_WARNING, (Code)) )



/* Normal completion status */
#define OLC_SC_NORMAL		0L


/* DT-Open Layers internal management errors (0x1 -> 0xff) */
#define OLC_SC_NOSHARE		OL_MAKE_OL_ERROR_SCODE(0x1)	/* Device is in use and not shareable */
#define OLC_SC_NOMEM		OL_MAKE_OL_ERROR_SCODE(0x2)	/* Unable to allocate required memory */
#define OLC_SC_NOMEMLOCK	OL_MAKE_OL_ERROR_SCODE(0x3)	/* Unable to lock down required memory */
#define OLC_SC_SYSERROR	OL_MAKE_OL_ERROR_SCODE(0x14)	/* Internal driver error */
#define OLC_SC_FIFO_OVERFLOW	OL_MAKE_OL_ERROR_SCODE(0x15)	/* Internal FIFO overflowed */
#define OLC_SC_FIELD_OVERFLOW	OL_MAKE_OL_ERROR_SCODE(0x16)	/* Internal Field overflowed */

/* DT-Open Layers general errors (0x100 -> 0x1ff) */
#define OLC_SC_RANGE		OL_MAKE_OL_ERROR_SCODE(0x100)	/* Argument out of range */
#define OLC_SC_STRUCTSIZ	OL_MAKE_OL_ERROR_SCODE(0x101)	/* Structure is wrong size */
#define OLC_SC_NULL		OL_MAKE_OL_ERROR_SCODE(0x102)	/* Attempt to follow NULL pointer or HANDLE */
#define OLC_SC_BUSY		OL_MAKE_OL_ERROR_SCODE(0x103)	/* Device is BUSY and can not process requested operation */
#define OLC_SC_BUFSIZ		OL_MAKE_OL_ERROR_SCODE(0x104)	/* Output buffer was not the correct size */
#define OLC_SC_UNSUPKEY		OL_MAKE_OL_ERROR_SCODE(0x105)	/* Unsupported Key Indicator */
#define OLC_SC_NOASYNC		OL_MAKE_OL_ERROR_SCODE(0x106)	/* Unable to accept asynchronous I/O request - queue is */
								/*    probably full                                     */
#define OLC_SC_TIMEOUT		OL_MAKE_OL_ERROR_SCODE(0x107)	/* Operation timed out */
#define OLC_SC_GRANULARITY	OL_MAKE_OL_ERROR_SCODE(0x108)	/* Argument within linear range, but not on legal imcrement */


/* DT-Open Layers frame grabber errors (0x200 -> 0x400) */
#define OLC_SC_UNSUPMEMTPYE		OL_MAKE_OL_ERROR_SCODE(0x200)	/* Memory type not known or supported by this driver */
#define OLC_SC_FRAMENOTAVAILABLE	OL_MAKE_OL_ERROR_SCODE(0x202)	/* Frame not available */
#define OLC_SC_INVALIDFRAMEID		OL_MAKE_OL_ERROR_SCODE(0x203)	/* Frame identifier is invalid */
#define OLC_SC_INVALIDFRAMEHANDLE	OL_MAKE_OL_ERROR_SCODE(0x204)	/* Frame handle is not valid */
#define OLC_SC_INVALIDFRAMERECT		OL_MAKE_OL_ERROR_SCODE(0x206)	/* Invalid frame rectangle */
#define OLC_SC_FRAMENOTALLOCATED	OL_MAKE_OL_ERROR_SCODE(0x207)	/* Frame not allocated */
#define OLC_SC_MAPERROR			OL_MAKE_OL_ERROR_SCODE(0x208)	/* Unable to map frame */
#define OLC_SC_UNMAPERROR		OL_MAKE_OL_ERROR_SCODE(0x209)	/* Unable to unmap frame */
#define OLC_SC_FRAMEISMAPPED		OL_MAKE_OL_ERROR_SCODE(0x20a)	/* Frame is currently mapped */
#define OLC_SC_FRAMENOTMAPPED		OL_MAKE_OL_ERROR_SCODE(0x20b)	/* Frame is not mapped */
#define OLC_SC_FRAMELIMITEXCEEDED	OL_MAKE_OL_ERROR_SCODE(0x20c)	/* Frame boundary exceeded */
#define OLC_SC_FRAMEWIDTH		OL_MAKE_OL_ERROR_SCODE(0x20d)	/* Frame width is illegal for current acquisition setup */
#define OLC_SC_CLAMP			OL_MAKE_OL_ERROR_SCODE(0x20e)	/* Clamp area is illegal for current acquisition setup */
#define OLC_SC_VERTICALINC		OL_MAKE_OL_ERROR_SCODE(0x20f)	/* Vertical frame increment is illegal for current */
									/*     acquisition setup                           */
#define OLC_SC_FIRSTACTPIX		OL_MAKE_OL_ERROR_SCODE(0x210)	/* First active pixel position is illegal for current */
									/*     acquisition setup                              */
#define OLC_SC_ACTPIXCOUNT		OL_MAKE_OL_ERROR_SCODE(0x211)	/* Active pixel count is illegal for current */
									/*     acquisition setup                     */
#define OLC_SC_FRAMELEFT		OL_MAKE_OL_ERROR_SCODE(0x212)	/* Left side of frame is illegal for current */
									/*     acquisition setup                     */
#define OLC_SC_FRAMETOP			OL_MAKE_OL_ERROR_SCODE(0x213)	/* Top of frame is illegal for current acquisition */
									/*     setup                                       */
#define OLC_SC_FRAMEHEIGHT		OL_MAKE_OL_ERROR_SCODE(0x214)	/* Frame height is illegal for current acquisition */
									/*     setup                                       */
#define OLC_SC_ACTLINECOUNT		OL_MAKE_OL_ERROR_SCODE(0x215)	/* Active line count is illegal for current */
									/*     acquisition setup                    */
#define OLC_SC_HSYNCSEARCHPOS		OL_MAKE_OL_ERROR_SCODE(0x216)	/* Horizontal sync search position is illegal for */
									/*     current acquisition setup                  */
#define OLC_SC_VSYNCSEARCHPOS		OL_MAKE_OL_ERROR_SCODE(0x217)	/* Vertical sync search position is illegal for */
									/*     current acquisition setup                */
#define OLC_SC_INPUTSOURCE   		OL_MAKE_OL_ERROR_SCODE(0x218)	/* Returned if input source channel out of range */
#define OLC_SC_CONTROL       		OL_MAKE_OL_ERROR_SCODE(0x219)	/* Returned if set input control function value */
									/*     is undefined.                            */
#define OLC_SC_LUT           		OL_MAKE_OL_ERROR_SCODE(0x21a)	/* Returned if LUT value out of range */
#define OLC_SC_BWINVERSION           	OL_MAKE_OL_ERROR_SCODE(0x21b)	/* Returned if Black Level is higher that White Level */
#define OLC_SC_WHITELEVEL           	OL_MAKE_OL_ERROR_SCODE(0x21c)	/* Returned if white level is cannot be set */
#define OLC_SC_INTERLACEDHGTGRAN	OL_MAKE_OL_ERROR_SCODE(0x21d)	/* Returned if frame height granularity is illegal */
									/*     when frame type is interlaced               */
#define OLC_SC_INTERLACEDTOPGRAN	OL_MAKE_OL_ERROR_SCODE(0x21e)	/* Returned if frame top granularity is illegal */
									/*     when frame type is interlaced            */
#define OLC_SC_INVALIDJOBHANDLE	OL_MAKE_OL_ERROR_SCODE(0x21f)	/* Returned if job handle in invalid */
#define OLC_SC_MODECONFLICT		OL_MAKE_OL_ERROR_SCODE(0x220)	/* Returned if attempted operation conflicts with current mode of operation */
#define OLC_SC_INVALIDHWND		OL_MAKE_OL_ERROR_SCODE(0x221)	/* Invalid window handle */
#define OLC_SC_INVALIDWNDALIGN		OL_MAKE_OL_ERROR_SCODE(0x222)	/* Invalid window alignment */
#define OLC_SC_PALETTESIZE		OL_MAKE_OL_ERROR_SCODE(0x223)	/* Invalid system palette size */ 
#define OLC_SC_NODCI			OL_MAKE_OL_ERROR_SCODE(0x224)	/* DCI could not be properly accessed */
#define OLC_SC_PASSTHRULUTRANGE OL_MAKE_OL_ERROR_SCODE(0x225)	/* invalid range for altering the PMLut */
#define OLC_SC_PASSTHRUPALRANGE OL_MAKE_OL_ERROR_SCODE(0x226)  /* invalid range for extending the passthru palette */
#define OLC_SC_NOSYSRESOURCE  OL_MAKE_OL_ERROR_SCODE(0x227)       /* hDC,bitmap, palette, timer, window, process not available. */

#ifndef EXCLUDE_DDI

/* DT-Open Layers Frame Grabber DDI Error status codes (0x228 -> 0x233) */
#define OLC_SC_SYS_RES					OL_MAKE_OL_ERROR_SCODE(0x228) /* System resource error */
#define OLC_SC_INVALID_SURFACE_HANDLE	OL_MAKE_OL_ERROR_SCODE(0x229) /* Surface Handle invalid */
#define OLC_SC_FIXED_COLOR				OL_MAKE_OL_ERROR_SCODE(0x22A) /* Key color can't be changed */
#define OLC_SC_INVALID_FLAGS			OL_MAKE_OL_ERROR_SCODE(0x22B) /* Some of the flags are illegal */
#define OLC_SC_NO_MORE_SURFACE			OL_MAKE_OL_ERROR_SCODE(0x22C) /* Driver's Max surfaces reached */
#define OLC_SC_PASSTHRU_STOPPED			OL_MAKE_OL_ERROR_SCODE(0x22D) /* Not in passthru mode */
#define OLC_SC_NO_DDI					OL_MAKE_OL_ERROR_SCODE(0x22E) /* DDI not supported */
#define OLC_SC_SURFACE_TOO_SMALL		OL_MAKE_OL_ERROR_SCODE(0x22F) /* Surface chosen was too small */
#define OLC_SC_PITCH_TOO_SMALL			OL_MAKE_OL_ERROR_SCODE(0x230) /* Pitch declared was too small */
#define OLC_SC_NO_IMAGE_IN_FRAME		OL_MAKE_OL_ERROR_SCODE(0x231) /* Pitch declared was too small */
#define OLC_SC_INVALID_SURFACE_DC		OL_MAKE_OL_ERROR_SCODE(0x232) /* Surface DC invalid */
#define OLC_SC_SURFACE_NOT_SET			OL_MAKE_OL_ERROR_SCODE(0x233) /* Surface selected yet */

#endif

#define OLC_SC_NO_VIDEO_SIGNAL          OL_MAKE_OL_ERROR_SCODE(0x234)


/* General DT-Open Layers Informational status codes */
#define OLC_SC_PENDING		OL_MAKE_OL_INFO_SCODE(0x1)	/* Job is pending and has not started executing */
#define OLC_SC_ACTIVE		OL_MAKE_OL_INFO_SCODE(0x2)	/* Job has started executing, but has not completed */
#define OLC_SC_CANCELJOB	OL_MAKE_OL_INFO_SCODE(0x3)	/* Job was canceled prior to completion */


/* General DT-Open Layers warnings (0x1 -> 0xff) */
#define OLC_SC_CLIP		OL_MAKE_OL_WARNING_SCODE(0x1)	/* A data value exceeded the legal range and was clipped */
#define OLC_SC_NONOLMSG		OL_MAKE_OL_WARNING_SCODE(0x2)	/* A unit opened for DT-Open Layers received a message  */
								/*     that was not handled.  The message was passed to */
								/*     DefDriverProc().                                 */


/* DT-Open Layers Frame Grabber Warnings (0x100 -> 0x2ff) */
#define OLC_SC_INTERVAL		OL_MAKE_OL_WARNING_SCODE(0x100)	/* Unable to perfrom at requested interval */



/* These are the values returned by SendDriverMessage for the majority   */
/*   of the messages handled.  They are not OLT_SCODEs, they are general */
/*   purpose return values.                                              */

#define OLC_RV_FAIL	0L			/* return 0 for fail */
#define OLC_RV_SUCCESS	( !OLC_RV_FAIL )	/* return non-zero for success */


/* The following API macros are used to determine the class of an OLT_SCODE */
#define OlScIsOlStatus(sc)	( ((sc) == OLC_SC_NORMAL) || (((sc) & OLC_SC_OWNER_MASK) == OLC_SC_OL) )
#define OlScIsOk(sc)		( (sc) == OLC_SC_NORMAL )
#define OlScIsInfo(sc)		( ((sc) & OLC_SEV_MASK) == OLC_SEV_INFO )
#define OlScIsWarning(sc)	( ((sc) & OLC_SEV_MASK) == OLC_SEV_WARNING )
#define OlScIsError(sc)		( ((sc) & OLC_SEV_MASK) == OLC_SEV_ERROR )


/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*- DT-Open Layers Frame Grabber Driver Messages and Data Structures  -*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

/* All pre-defined DT-Open Layers driver messages will fall in the */
/*   range of DRV_OLIMGDEV_BASE <= msg < DRV_OLIMGDEV_DEVICE_BASE. */

#define DRV_OLIMGDEV_BASE		(DRV_USER + 0x1000)
#define DRV_OLIMGDEV_DEVICE_BASE	(DRV_USER + 0xA000)


/* All pre-defined Monochrome Frame Grabber messages (OLFG) should */
/*   start beyond this point.                                      */

#define DRV_OLFG_BASE			(DRV_OLIMGDEV_BASE + 0x1000)



/*=======================================================*/
/*======================= General =======================*/
/*=======================================================*/

/* Messages */
#define DRV_OLIMGDEV_GET_STATUS		(DRV_OLIMGDEV_BASE + 0x0001)
#define DRV_OLIMGDEV_QUERY_CAPS		(DRV_OLIMGDEV_BASE + 0x0003)
#define DRV_OLIMGDEV_SET_TIMEOUT	(DRV_OLIMGDEV_BASE + 0x0004)
#define DRV_OLIMGDEV_QUERY_TIMEOUT	(DRV_OLIMGDEV_BASE + 0x0005)
#define DRV_OLIMGDEV_RESET			(DRV_OLIMGDEV_BASE + 0x0006)
#define DRV_OLIMGDEV_COLOR

/* Maximum number of characters in driver alias, used in OpenDriver */
#define OLC_MAX_ALIAS		21

/* Maximum number of characters (including NULL) in device name, returned by OLC_IMG_NAME */
#define OLC_DEV_NAME_SIZE	20


/* Note:  DT-Open Layers requires "far" (32-bit) pointers whenever  */
/*   a pointer type is required.  The model-dependent pointer       */
/*   definitions provided in this file are strictly for the benefit */
/*   of the application programmer in his/her own code.  Objects    */
/*   defined by such pointers should never be passed to a DT-Open   */
/*   Layers function or driver without the proper explicit type     */
/*   casting.                                                       */


/* Device Open Information */
typedef struct olOLT_DEV_OPEN_INFO
	{
	 ULNG StructSize;				/* sizeof(OLT_DEF_OPEN_INFO) */
	 char szAlias[OLC_MAX_ALIAS + 1];	/* alias used to open driver */
	} OLT_DEV_OPEN_INFO, *POLT_DEV_OPEN_INFO, FAR *LPOLT_DEV_OPEN_INFO;


/* I/O Data Types */
typedef struct olOLT_OUTBUF
	{
	 ULNG StructSize;		/* sizeof(OLT_OUTBUF) */
	 ULNG BufferBytes;		/* Number of bytes in buffer pointed to by BufPtr */
	 ULNG BytesWritten;		/* Number of bytes written into buffer by driver */
	 LPVOID BufPtr;			/* Pointer to appropriately sized data buffer */
	} OLT_OUTBUF, *POLT_OUTBUF, FAR *LPOLT_OUTBUF;





/*---------------------------------------------------------------------*/
/*--------------------------  Prototypes  -----------------------------*/
/*---------------------------------------------------------------------*/

/* DT-Open Layers */




/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* OLIMGDEV_H__ */

