/*	File:	otwintyp.h

	Description:
		This file contains type definitions used for programming
	within Windows at Data Translation.  These data types are defined
	so that we can isolate ourselves from portability (16-bit to 32-bit)
	issues and still maintain the highest level of efficiency allowed
	by a language implementation.

	Requirements:
		#include <limits.h>
		#include <windows.h>

*/
/*=======================================================================++
||	Copyright (C) 1990 - 1994.  Data Translation, Inc., 100 Locke    ||
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


#if !defined(OLWINTYP_H__)
#define OLWINTYP_H__

/* People seem to forget to include limits.h, so we will */
#if !defined(INT_MAX)
#include <limits.h>
#endif

/* In WIN32 export and huge don't exist */
#ifdef WIN32
 #include <wingdi.h>

 #ifndef _export
  #define __export
 #endif

 #ifndef __loadds
  #define __loadds
 #endif

 #ifndef __huge
  #define __huge
 #endif

 #ifndef __far
  #define __far 
 #endif

 #ifndef _export
  #define _export
 #endif
 

 #ifndef _loadds
  #define _loadds
 #endif

 #ifndef _huge
  #define _huge
 #endif

 #ifndef _far
  #define _far
 #endif

   typedef RGBTRIPLE *LPRGBTRIPLE;

#endif

#ifdef _NTDDK_
  typedef unsigned long ECODE;
#endif

/* Assume C delclarations for C++ callers */
#ifdef __cplusplus
extern "C" {
#endif


/* Calling convention type qualifiers */
#define	DLLEXPORT   __declspec(dllexport) // used for VisualC++ 4.0
#define DLLIMPORT	__declspec(dllimport) // used for VisualC++ 4.0
#define	PUBLIC		FAR PASCAL __export	  // used for VisualC++ 2.0
#define	PRIVATE		PASCAL
#define	CPUBLIC		FAR CDECL __export
#define	CPRIVATE	CDECL

/* Memory model type qualifiers */
#if defined(_HUGE)
	#undef _HUGE
#endif
#define _HUGE	__huge


/* Define the generic data types */
typedef	char		CHR;		/* 8-bit signed value */
typedef	unsigned char	UCHR;		/* 8-bit unsigned value */


#if (INT_MAX == SHRT_MAX)
typedef	int		SHRT;		/* 16-bit signed value */
typedef	unsigned int	USHRT;		/* 16-bit unsigned value */
typedef	long		LNG;		/* 32-bit signed value */
typedef	unsigned long	ULNG;		/* 32-bit unsigned value */
#else
typedef	short		SHRT;		/* 16-bit signed value */
typedef	unsigned short	USHRT;		/* 16-bit unsigned value */
typedef	int		LNG;		/* 32-bit signed value */
typedef	unsigned int	ULNG;		/* 32-bit unsigned value */
#endif

typedef float		FLOAT;		/* single-precision floating point value */
typedef double		DOUBLE;		/* double-precision floating point value */


/* Define pointers to the basic data types */
typedef VOID			*PVOID;		/* generic pointer */
typedef VOID NEAR		*NPVOID;	/* generic NEAR pointer */
typedef VOID FAR		*LPVOID;	/* generic FAR pointer */
typedef VOID _HUGE		*HPVOID;	/* generic _HUGE pointer */
typedef const VOID		*PCVOID;	/* generic pointer to constant data */
typedef const VOID NEAR		*NPCVOID;	/* generic NEAR pointer to constant data */
typedef const VOID FAR		*LPCVOID;	/* generic FAR pointer to constant data */
typedef const VOID _HUGE	*HPCVOID;	/* generic _HUGE pointer to constant data */

typedef CHR			*PCHR;		/* generic pointer to CHR */
typedef CHR NEAR		*NPCHR;		/* generic NEAR pointer to CHR */
typedef CHR FAR			*LPCHR;		/* generic FAR pointer to CHR */
typedef CHR _HUGE		*HPCHR;		/* generic _HUGE pointer to CHR */
typedef const CHR		*PCCHR;		/* generic pointer to constant CHR */
typedef const CHR NEAR		*NPCCHR;	/* generic NEAR pointer to constant CHR */
typedef const CHR FAR		*LPCCHR;	/* generic FAR pointer to constant CHR */
typedef const CHR _HUGE		*HPCCHR;	/* generic _HUGE pointer to constant CHR */

typedef UCHR			*PUCHR;		/* generic pointer to UCHR */
typedef UCHR NEAR		*NPUCHR;	/* generic NEAR pointer to UCHR */
typedef UCHR FAR		*LPUCHR;	/* generic FAR pointer to UCHR */
typedef UCHR _HUGE		*HPUCHR;	/* generic _HUGE pointer to UCHR */
typedef const UCHR		*PCUCHR;	/* generic pointer to constant UCHR */
typedef const UCHR NEAR		*NPCUCHR;	/* generic NEAR pointer to constant UCHR */
typedef const UCHR FAR		*LPCUCHR;	/* generic FAR pointer to constant UCHR */
typedef const UCHR _HUGE	*HPCUCHR;	/* generic _HUGE pointer to constant UCHR */

typedef SHRT			*PSHRT;		/* generic pointer to SHRT */
typedef SHRT NEAR		*NPSHRT;	/* generic NEAR pointer to SHRT */
typedef SHRT FAR		*LPSHRT;	/* generic FAR pointer to SHRT */
typedef SHRT _HUGE		*HPSHRT;	/* generic _HUGE pointer to SHRT */
typedef const SHRT		*PCSHRT;	/* generic pointer to constant SHRT */
typedef const SHRT NEAR		*NPCSHRT;	/* generic NEAR pointer to constant SHRT */
typedef const SHRT FAR		*LPCSHRT;	/* generic FAR pointer to constant SHRT */
typedef const SHRT _HUGE	*HPCSHRT;	/* generic _HUGE pointer to constant SHRT */

typedef USHRT			*PUSHRT;	/* generic pointer to USHRT */
typedef USHRT NEAR		*NPUSHRT;	/* generic NEAR pointer to USHRT */
typedef USHRT FAR		*LPUSHRT;	/* generic FAR pointer to USHRT */
typedef USHRT _HUGE		*HPUSHRT;	/* generic _HUGE pointer to USHRT */
typedef const USHRT		*PCUSHRT;	/* generic pointer to constant USHRT */
typedef const USHRT NEAR	*NPCUSHRT;	/* generic NEAR pointer to constant USHRT */
typedef const USHRT FAR		*LPCUSHRT;	/* generic FAR pointer to constant USHRT */
typedef const USHRT _HUGE	*HPCUSHRT;	/* generic _HUGE pointer to constant USHRT */

typedef LNG			*PLNG;		/* generic pointer to LNG */
typedef LNG NEAR		*NPLNG;		/* generic NEAR pointer to LNG */
typedef LNG FAR			*LPLNG;		/* generic FAR pointer to LNG */
typedef LNG _HUGE		*HPLNG;		/* generic _HUGE pointer to LNG */
typedef const LNG		*PCLNG;		/* generic pointer to constant LNG */
typedef const LNG NEAR		*NPCLNG;	/* generic NEAR pointer to constant LNG */
typedef const LNG FAR		*LPCLNG;	/* generic FAR pointer to constant LNG */
typedef const LNG _HUGE		*HPCLNG;	/* generic _HUGE pointer to constant LNG */

typedef ULNG			*PULNG;		/* generic pointer to ULNG */
typedef ULNG NEAR		*NPULNG;	/* generic NEAR pointer to ULNG */
typedef ULNG FAR		*LPULNG;	/* generic FAR pointer to ULNG */
typedef ULNG _HUGE		*HPULNG;	/* generic _HUGE pointer to ULNG */
typedef const ULNG		*PCULNG;	/* generic pointer to constant ULNG */
typedef const ULNG NEAR		*NPCULNG;	/* generic NEAR pointer to constant ULNG */
typedef const ULNG FAR		*LPCULNG;	/* generic FAR pointer to constant ULNG */
typedef const ULNG _HUGE	*HPCULNG;	/* generic _HUGE pointer to constant ULNG */

typedef FLOAT			*PFLOAT;	/* generic pointer to FLOAT */
typedef FLOAT NEAR		*NPFLOAT;	/* generic NEAR pointer to FLOAT */
typedef FLOAT FAR		*LPFLOAT;	/* generic FAR pointer to FLOAT */
typedef FLOAT _HUGE		*HPFLOAT;	/* generic _HUGE pointer to FLOAT */
typedef const FLOAT		*PCFLOAT;	/* generic pointer to constant FLOAT */
typedef const FLOAT NEAR	*NPCFLOAT;	/* generic NEAR pointer to constant FLOAT */
typedef const FLOAT FAR		*LPCFLOAT;	/* generic FAR pointer to constant FLOAT */
typedef const FLOAT _HUGE	*HPCFLOAT;	/* generic _HUGE pointer to constant FLOAT */

typedef DOUBLE			*PDOUBLE;	/* generic pointer to DOUBLE */
typedef DOUBLE NEAR		*NPDOUBLE;	/* generic NEAR pointer to DOUBLE */
typedef DOUBLE FAR		*LPDOUBLE;	/* generic FAR pointer to DOUBLE */
typedef DOUBLE _HUGE		*HPDOUBLE;	/* generic _HUGE pointer to DOUBLE */
typedef const DOUBLE		*PCDOUBLE;	/* generic pointer to constant DOUBLE */
typedef const DOUBLE NEAR	*NPCDOUBLE;	/* generic NEAR pointer to constant DOUBLE */
typedef const DOUBLE FAR	*LPCDOUBLE;	/* generic FAR pointer to constant DOUBLE */
typedef const DOUBLE _HUGE	*HPCDOUBLE;	/* generic _HUGE pointer to constant DOUBLE */

#ifndef _NTDDK_

typedef BOOL			*PBOOL;		/* generic pointer to a BOOL */
typedef BOOL NEAR		*NPBOOL;	/* generic NEAR pointer to a BOOL */
typedef BOOL FAR		*LPBOOL;	/* generic FAR pointer to a BOOL */
typedef BOOL _HUGE		*HPBOOL;	/* generic _HUGE pointer to a BOOL */
typedef const BOOL		*PCBOOL;	/* generic pointer to constant BOOL */
typedef const BOOL NEAR		*NPCBOOL;	/* generic NEAR pointer to constant BOOL */
typedef const BOOL FAR		*LPCBOOL;	/* generic FAR pointer to constant BOOL */
typedef const BOOL _HUGE	*HPCBOOL;	/* generic _HUGE pointer to constant BOOL */

#endif


/*=======================================================================*/
/*=================  DT-Open Layers Imaging Devices  ====================*/
/*=======================================================================*/

/* Macro to define ID's, Handles, and other opaque data types.  For example, the */
/*   call DECLARE_OL_HDL(PROC, BUF, ID) creates the "handle" OLT_PROC_BUF_ID.    */
/*   This is simply a 32-bit opaque data type whose meaning is known only to     */
/*   the implementation, not to the user.                                        */

#define DECLARE_OL_HDL(type, name, tag)								\
			typedef struct olOLT_##type##_##name##_##tag				\
			   { LPVOID Unsused; } FAR *OLT_##type##_##name##_##tag;		\
			typedef OLT_##type##_##name##_##tag     *POLT_##type##_##name##_##tag;	\
			typedef OLT_##type##_##name##_##tag FAR *LPOLT_##type##_##name##_##tag



#ifndef DT_MIDL_PASS
/* 48-bit selector:offset pair */
typedef struct dtDT_ADDR_48
	{
	 USHRT Selector;		/* 16-bit selector */
	 ULNG Offset;			/* 32-bit offset */
	} DT_ADDR_48, *PDT_ADDR_48, FAR *LPDT_ADDR_48;
typedef const DT_ADDR_48 FAR *LPCDT_ADDR_48;
#endif // DT_MIDL_PASS


/* Data types describing a range of values */

/* Linear range of long integers */
typedef struct olOLT_LNG_RNG
	{
	 LNG Min;			/* Minimum value in range */
	 LNG Max;			/* Maximum value in range */
	 LNG Granularity;		/* Increment between consecutive units */
	 LNG Nominal;			/* Nominal/Default value in range */
	} OLT_LNG_RANGE, *POLT_LNG_RANGE, FAR *LPOLT_LNG_RANGE;

#ifndef DT_MIDL_PASS
/* Segmented linear range of long integers */
typedef struct olOLT_SEG_LNG_RANGE
	{
	 ULNG SegmentCount;		/* Number of segments in this segmented linear range */
	 OLT_LNG_RANGE Range;		/* Description of overall range */
	} OLT_SEG_LNG_RANGE, *POLT_SEG_LNG_RANGE, FAR *LPOLT_SEG_LNG_RANGE;

/* Linear Range of doubles */
typedef struct olOLT_DBL_RANGE
	{
	 double Min;			/* Minimum value in range */
	 double Max;			/* Maximum value in range */
	 double Granularity;		/* Increment between consecutive units */
	 double Nominal;		/* Nominal/Default value in range */
	} OLT_DBL_RANGE, *POLT_DBL_RANGE, FAR *LPOLT_DBL_RANGE;

/* Segmented linear range of doubles */
typedef struct olOLT_SEG_DBL_RANGE
{
	 ULNG SegmentCount;		/* Number of segments in this segmented linear range */
	 OLT_DBL_RANGE Range;		/* Description of overall range */
} OLT_SEG_DBL_RANGE, *POLT_SEG_DBL_RANGE, FAR *LPOLT_SEG_DBL_RANGE;

#endif // DT_MIDL_PASS

/* Non-linear range of long integers */
typedef struct olOLT_NL_LNG_RNG
	{
	 LNG Min;			/* Minimum value in range */
	 LNG Max;			/* Maximum value in range */
	 LNG Nominal;			/* Nominal/Default value in range */
	} OLT_NL_LNG_RANGE, *POLT_NL_LNG_RANGE, FAR *LPOLT_NL_LNG_RANGE;


/* Rectangle Data Type */
typedef struct olOLT_RECT
	{
	 LNG x, y;			/* Upper-left corner */
	 LNG Width;			/* Number of columns */
	 LNG Height;			/* Number of rows */
	} OLT_RECT, *POLT_RECT, FAR *LPOLT_RECT;


/* List data types */
typedef struct olOLT_LIST_LIMITS
	{
	 LNG Count;			/* Number of elements in list */
	 LNG Min;			/* Minimum value in range */
	 LNG Max;			/* Maximum value in range */
	 LNG Nominal;			/* Nominal/Default value in range */
	} OLT_LIST_LIMITS, *POLT_LIST_LIMITS, FAR *LPOLT_LIST_LIMITS;



/* DT-Open Layers Severity Levels */
typedef enum olOLT_SEVERITY
	{
	 OLC_IMG_SEV_NORMAL = 0,
	 OLC_IMG_SEV_INFO = 1,
	 OLC_IMG_SEV_WARNING = 2,
	 OLC_IMG_SEV_ERROR = 4
	} OLT_SEVERITY;


/* Imaging Device Capability ("DC") Keys */
typedef enum olOLT_DEV_CAP_KEY
	{
	 OLC_IMG_DC_UNKNOWN = 0,
	 OLC_IMG_DC_OL_SIGNATURE = 1,
	 OLC_IMG_DC_DEVICE_ID = 2,
	 OLC_IMG_DC_DEVICE_NAME = 3,
	 OLC_IMG_DC_OL_DEVICE_TYPE = 4,
	 OLC_IMG_DC_SECTIONS = 5,
	 OLC_IMG_DC_LINEAR_MEM_SIZE = 6,
	 OLC_IMG_DC_DEVICE_MEM_SIZE = 7
	} OLT_DEV_CAP_KEY, *POLT_DEV_CAP_KEY, FAR *LPOLT_DEV_CAP_KEY;


/* DT-Open Layers imaging device signature, returned by OLC_IMG_DC_OL_SIGNATURE dev cap query */
#define OLC_IMG_DEV_SIGNATURE	0x44544F4C


/* Imaging device types, returned by OLC_IMG_DC_OL_DEVICE_TYPE dev cap query */
typedef enum olOLT_IMGDEVTYPE
	{
	 OLC_IMG_DEV_BOGUS = 0,
	 OLC_IMG_DEV_MONO_FRAME_GRABBER = 1,
	 OLC_IMG_DEV_COLOR_FRAME_GRABBER = 2
	} OLT_IMGDEVTYPE, *POLT_IMGDEVTYPE, FAR *LPOLT_IMGDEVTYPE;



/*=======================================================================*/
/*==================  Monochrome Frame Grabbers  =======================*/
/*=======================================================================*/


/* Device Section bit flags for use with value returned by OLC_IMG_DC_SECTIONS dev */
/*   cap query.  These flags should be used when the device type returned by the   */
/*   OLC_IMG_DC_OL_DEVICE_TYPE dev cap query equals OLC_IMG_DEV_MONO_FRAME_GRABBER */

#define OLC_FG_SECTION_INPUT	0x00000001
#define OLC_FG_SECTION_CAMCTL	0x00000002
#define OLC_FG_SECTION_MEMORY	0x00000004
#define OLC_FG_SECTION_LINEAR	0x00000008
#define OLC_FG_SECTION_PASSTHRU	0x00000010
#define OLC_FG_SECTION_DDI		0x00000020



/* Mono Frame Grabber Input Capability ("IC") Keys */
typedef enum olOLT_FG_INPUT_CAP_KEY
	{
	 OLC_FG_IC_UNKNOWN						= 0x0000,
	 OLC_FG_IC_INPUT_SOURCE_COUNT			= 0x0001,
	 OLC_FG_IC_ILUT_COUNT					= 0x0008,
	 OLC_FG_IC_MAX_ILUT_INDEX				= 0x0009,
	 OLC_FG_IC_MAX_ILUT_VALUE				= 0x000A,
	 OLC_FG_IC_MAX_FRAME_SIZE				= 0x0010,
	 OLC_FG_IC_PIXEL_DEPTH					= 0x0018,
	 OLC_FG_IC_FRAME_TYPE_LIMITS			= 0x0020,
	 OLC_FG_IC_DOES_INPUT_FILTER			= 0x0100,
	 OLC_FG_IC_INPUT_FILTER_LIMITS			= 0x0101,
	 OLC_FG_IC_DOES_PROG_A2D				= 0x0110,
	 OLC_FG_IC_BLACK_LEVEL_LIMITS			= 0x0113,
	 OLC_FG_IC_WHITE_LEVEL_LIMITS			= 0x0115,
	 OLC_FG_IC_DOES_VIDEO_SELECT			= 0x0120,
	 OLC_FG_IC_VIDEO_TYPE_LIMITS			= 0x0121,
	 OLC_FG_IC_CSYNC_SOURCE_LIMITS			= 0x0122,
	 OLC_FG_IC_CSYNC_THRESH_LIST_LIMITS		= 0x0123,
	 OLC_FG_IC_CSYNC_THRESH_LIST			= 0x0124,
	 OLC_FG_IC_DOES_ACTIVE_VIDEO			= 0x0130,
	 OLC_FG_IC_CLAMP_START_LIMITS			= 0x0131,
	 OLC_FG_IC_CLAMP_END_LIMITS				= 0x0132,
	 OLC_FG_IC_ACTIVE_PIXEL_LIMITS			= 0x0133,
	 OLC_FG_IC_ACTIVE_WIDTH_LIMITS			= 0x0134,
	 OLC_FG_IC_ACTIVE_LINE_LIMITS			= 0x0135,
	 OLC_FG_IC_ACTIVE_HEIGHT_LIMITS			= 0x0136,
	 OLC_FG_IC_BACK_PORCH_START_LIMITS		= 0x0137,
	 OLC_FG_IC_TOTAL_PIX_PER_LINE_LIMITS	= 0x0138,
	 OLC_FG_IC_TOTAL_LINES_PER_FLD_LIMITS	= 0x0139,
	 OLC_FG_IC_DOES_SYNC_SENTINEL			= 0x0140,
	 OLC_FG_IC_SYNC_SENTINEL_TYPE_LIMITS	= 0x0141,
	 OLC_FG_IC_DOES_FRAME_SELECT			= 0x0150,
	 OLC_FG_IC_FRAME_TOP_LIMITS				= 0x0151,
	 OLC_FG_IC_FRAME_LEFT_LIMITS			= 0x0152,
	 OLC_FG_IC_FRAME_WIDTH_LIMITS			= 0x0153,
	 OLC_FG_IC_FRAME_HEIGHT_LIMITS			= 0x0154,
	 OLC_FG_IC_FRAME_HINC_LIMITS			= 0x0155,
	 OLC_FG_IC_FRAME_VINC_LIMITS			= 0x0156,
	 OLC_FG_IC_DOES_PIXEL_CLOCK				= 0x0160,
	 OLC_FG_IC_CLOCK_FREQ_LIMITS			= 0x0161,
	 OLC_FG_IC_CLOCK_SOURCE_LIMITS			= 0x0162,
	 OLC_FG_IC_DOES_EVENT_COUNTING			= 0x0170,
	 OLC_FG_IC_EVENT_TYPE_LIMITS			= 0x0171,
	 OLC_FG_IC_EVENT_COUNT_LIMITS			= 0x0172,
	 OLC_FG_IC_DOES_TRIGGER					= 0x0180,
	 OLC_FG_IC_TRIGGER_TYPE_LIMITS			= 0x0181,
	 OLC_FG_IC_MULT_TRIGGER_TYPE_LIMITS		= 0x0182,
	 OLC_FG_IC_MULT_TRIGGER_MODE_LIMITS		= 0x0183,
	 OLC_FG_IC_SINGLE_FRAME_OPS				= 0x0200,
	 OLC_FG_IC_MULT_FRAME_OPS				= 0x0201,
     OLC_FG_IC_DOES_QUERY_INPUT_FILTER		= 0x0202,
	 OLC_FG_IC_DOES_QUERY_PROG_A2D			= 0x0203,
	 OLC_FG_IC_DOES_QUERY_VIDEO_SELECT		= 0x0204,
	 OLC_FG_IC_DOES_QUERY_SYNC_SENTINEL		= 0x0205,
	 OLC_FG_IC_DOES_QUERY_FRAME_SELECT		= 0x0206,
	 OLC_FG_IC_DOES_QUERY_PIXEL_CLOCK		= 0x0207,
	 OLC_FG_IC_DOES_QUERY_ACTIVE_VIDEO		= 0x0208,
	 OLC_FG_IC_DOES_DRAW_ACQUIRED_FRAME		= 0x0209,
	 OLC_FG_IC_STROBE_PULSE_WIDTH_LIST_LIMITS = 0x0210,
	 OLC_FG_IC_STROBE_PULSE_WIDTH_LIST		= 0x0211,
	 OLC_FG_IC_STROBE_TYPE_LIMITS			= 0x0212,
	 OLC_FG_IC_DOES_DRAW_ACQUIRED_FRAME_EX	= 0x0213,
	 OLC_FG_IC_DOES_STROBE					= 0x214,
	 OLC_FG_IC_DOES_COLOR					= 0x215

	} OLT_FG_INPUT_CAP_KEY, *POLT_FG_INPUT_CAP_KEY, FAR *LPOLT_FG_INPUT_CAP_KEY;


/* Single Frame Acquire Operations bit masks.  Used when testing return from */
/*   OLC_FG_IC_SINGLE_FRAME_OPS IC query.                                    */

#define OLC_FG_ACQ_FRAME		0x00000001	/* Supports single frame acquisitions */
#define OLC_FG_ACQ_SUBFRAME		0x00000002	/* Supports single sub frame acquisitions */
#define OLC_FG_ACQ_FRAME_TO_FIT	0x00000004	/* Supports single frame to fit acquisitions */


#ifndef DT_MIDL_PASS
/* Single Frame Acquire Operations Data Type.  This is the data type returned */
/*   by the OLC_FG_IC_SINGLE_FRAME_OPS IC query.                              */
typedef struct olOLT_FG_SINGLE_FRAME_OPS
{
	 ULNG ToDevSync;		/* Synchronous acquisitions to device memory */
	 ULNG ToDevAsync;		/* Asynchronous acquisitions to device memory */
	 ULNG ToHostSync;		/* Synchronous acquisitions to host memory */
	 ULNG ToHostAsync;		/* Asynchronous acquisitions to host memory */
} OLT_FG_SINGLE_FRAME_OPS, *POLT_FG_SINGLE_FRAME_OPS, FAR *LPOLT_FG_SINGLE_FRAME_OPS;

#else

typedef struct olOLT_FG_SINGLE_FRAME_OPS
{
	 long ToDevSync;		/* Synchronous acquisitions to device memory */
	 long ToDevAsync;		/* Asynchronous acquisitions to device memory */
	 long ToHostSync;		/* Synchronous acquisitions to host memory */
	 long ToHostAsync;		/* Asynchronous acquisitions to host memory */
} OLT_FG_SINGLE_FRAME_OPS, *POLT_FG_SINGLE_FRAME_OPS, FAR *LPOLT_FG_SINGLE_FRAME_OPS;
#endif // DT_MIDL_PASS

/* multiple Frame Acquire Operations bit masks.  Used when testing return from */
/*   OLC_FG_IC_MULT_FRAME_OPS IC query.                                    */

#define OLC_FG_ACQ_MULTIPLE			0x00000001	/* Supports Multiple frame acquisitions to device */
#define OLC_FG_ACQ_CONSECUTIVE		0x00000002	/* Supports Consecutive frame acquisitions to device */

#ifndef DT_MIDL_PASS
/* Multiple Frame Acquire Operations Data Type.  This is the data type returned */
/*   by the OLC_FG_IC_MULT_FRAME_OPS IC query.                              */
typedef struct olOLT_FG_MULT_FRAME_OPS
{
	ULNG ToDevSync;		/* Synchronous acquisitions to device memory */
	ULNG ToDevAsync;		/* Asynchronous acquisitions to device memory */
	ULNG ToHostSync;		/* Synchronous acquisitions to host memory */
	ULNG ToHostAsync;		/* Asynchronous acquisitions to host memory */
} OLT_FG_MULT_FRAME_OPS, *POLT_FG_MULT_FRAME_OPS, FAR *LPOLT_FG_MULT_FRAME_OPS;

#else

typedef struct olOLT_FG_MULT_FRAME_OPS
{
	long ToDevSync;		/* Synchronous acquisitions to device memory */
	long ToDevAsync;		/* Asynchronous acquisitions to device memory */
	long ToHostSync;		/* Synchronous acquisitions to host memory */
	long ToHostAsync;		/* Asynchronous acquisitions to host memory */
} OLT_FG_MULT_FRAME_OPS, *POLT_FG_MULT_FRAME_OPS, FAR *LPOLT_FG_MULT_FRAME_OPS;
#endif // DT_MIDL_PASS


/* Input Controls */
typedef enum olOLT_FG_INPUT_CONTROL
	{
	 OLC_FG_CTL_UNKNOWN		= 0x0000,
	 OLC_FG_CTL_ILUT		= 0x0001,
	 OLC_FG_CTL_FRAME_TYPE		= 0x0002,
	 OLC_FG_CTL_INPUT_FILTER	= 0x0100,
	 OLC_FG_CTL_BLACK_LEVEL		= 0x0111,
	 OLC_FG_CTL_WHITE_LEVEL		= 0x0113,
	 OLC_FG_CTL_VIDEO_TYPE		= 0x0120,
	 OLC_FG_CTL_VARSCAN_FLAGS	= 0x0121,
	 OLC_FG_CTL_CSYNC_SOURCE	= 0x0122,
	 OLC_FG_CTL_CSYNC_THRESH	= 0x0123,
	 OLC_FG_CTL_CLAMP_START		= 0x0130,
	 OLC_FG_CTL_CLAMP_END		= 0x0131,
	 OLC_FG_CTL_FIRST_ACTIVE_PIXEL	= 0x0132,
	 OLC_FG_CTL_ACTIVE_PIXEL_COUNT	= 0x0133,
	 OLC_FG_CTL_FIRST_ACTIVE_LINE	= 0x0134,
	 OLC_FG_CTL_ACTIVE_LINE_COUNT	= 0x0135,
	 OLC_FG_CTL_TOTAL_PIX_PER_LINE	= 0x0136,
	 OLC_FG_CTL_TOTAL_LINES_PER_FLD	= 0x0137,
	 OLC_FG_CTL_BACK_PORCH_START	= 0x0138,
	 OLC_FG_CTL_FRAME_TOP		= 0x0150,
	 OLC_FG_CTL_FRAME_LEFT		= 0x0151,
	 OLC_FG_CTL_FRAME_WIDTH		= 0x0152,
	 OLC_FG_CTL_FRAME_HEIGHT	= 0x0153,
	 OLC_FG_CTL_HOR_FRAME_INC	= 0x0154,
	 OLC_FG_CTL_VER_FRAME_INC	= 0x0155,
	 OLC_FG_CTL_CLOCK_FREQ		= 0x0160,
	 OLC_FG_CTL_CLOCK_SOURCE	= 0x0161,
	 OLC_FG_CTL_CLOCK_FLAGS		= 0x0162,
	 OLC_FG_CTL_SYNC_SENTINEL	= 0x0170,
	 OLC_FG_CTL_HSYNC_INSERT_POS	= 0x0171,
	 OLC_FG_CTL_HSYNC_SEARCH_POS	= 0x0172,
	 OLC_FG_CTL_VSYNC_INSERT_POS	= 0x0173,
	 OLC_FG_CTL_VSYNC_SEARCH_POS	= 0x0174
	} OLT_FG_INPUT_CONTROL, *POLT_FG_INPUT_CONTROL, FAR *LPOLT_FG_INPUT_CONTROL;


/* Input Filter Type bit masks */
#define OLC_FG_FILT_AC_NONE			0x00000001	/* Can do AC coupled, with no filter */
#define OLC_FG_FILT_AC_50			0x00000002	/* Can do AC coupled, with 50 Hz (4.43 MHz) filter */
#define OLC_FG_FILT_AC_60			0x00000004	/* Can do AC coupled, with 60 Hz (3.58 MHz) filter */
#define OLC_FG_FILT_DC_NONE			0x00000008	/* Can do DC coupled, with no filter */


/* Input Video Type bit masks */

// DT_MIDL_PASS is only defined when compiling a typelib via an idl file (e.g project Ruby).
// It is used here so that the standard standard SDK doesnt change while providing robust types for typelibs.
// Eventually we may want to eliminate the old types.
#ifdef DT_MIDL_PASS

typedef enum olOLC_FG_VID_BITMASK
{
    OLC_FG_VID_COMPOSITE	= 0x00000001,	/* Frame Grabber can acquire from composite video source */
    OLC_FG_VID_VARSCAN		= 0x00000002	/* Frame Grabber can acquire from variable scan video source */
} OLC_FG_VID_BITMASK;

#else  // DT_MIDL_PASS

typedef ULNG OLC_FG_VID_BITMASK;
#define OLC_FG_VID_COMPOSITE		0x00000001	/* Frame Grabber can acquire from composite video source */
#define OLC_FG_VID_VARSCAN			0x00000002	/* Frame Grabber can acquire from variable scan video source */

#endif // DT_MIDL_PASS


/* Variable Scan Input Flags bit masks */
#define OLC_FG_VS_LINE_ON_LO_TO_HI		0x00000001	/* Set if lo-to-hi transitions on the var. scan. line */
								/*    sync line are considered a "sync" indication.   */
#define OLC_FG_VS_FIELD_ON_LO_TO_HI		0x00000002	/* Set if lo-to-hi transitions on the var. scan. field */
								/*    sync line are considered a "sync" indication.    */


/* Input Composite Sync Source bit masks */
#ifdef DT_MIDL_PASS

typedef enum olOLT_FG_CSYNC_BITMASK
{
    OLC_FG_CSYNC_CURRENT_SRC		= 0x00000001,	/* Driver can get CSYNC from current input source */
    OLC_FG_CSYNC_SPECIFIC_SRC		= 0x00000002,	/* Driver can get CSYNC from any input source */
    OLC_FG_CSYNC_EXTERNAL_LINE		= 0x00000004,	/* Driver can get CSYNC from external sync line */
} OLT_FG_CSYNC_BITMASK;

#else  // DT_MIDL_PASS

typedef ULNG OLT_FG_CSYNC_BITMASK;
#define OLC_FG_CSYNC_CURRENT_SRC		0x00000001	/* Driver can get CSYNC from current input source */
#define OLC_FG_CSYNC_SPECIFIC_SRC		0x00000002	/* Driver can get CSYNC from any input source */
#define OLC_FG_CSYNC_EXTERNAL_LINE		0x00000004	/* Driver can get CSYNC from external sync line */

#endif // DT_MIDL_PASS


/* Input Clock Source bit masks */
#define OLC_FG_CLOCK_INTERNAL			0x00000001	/* Frame Grabber can generate timing for video input */
#define OLC_FG_CLOCK_EXTERNAL			0x00000002	/* Frame Grabber can utilize an external clock for input timing */


/* Input Clock Flags bit masks */
#define OLC_FG_CLOCK_EXT_ON_LO_TO_HI		0x00000001	/* Set if lo-to-hi transitions on the external clock line are */
								/*     considered a "clock"                                   */


/* Sync Sentinel type bit masks */
#ifdef DT_MIDL_PASS

typedef enum olOLT_FG_SYNC_SENTINEL_BITMASK
{
    OLC_FG_SYNC_SENTINEL_FIXED			= 0x00000001,	/* Frame Grabber has Sync Sentinel but has fixed settings */
    OLC_FG_SYNC_SENTINEL_VARIABLE		= 0x00000002	/* Frame Grabber has Sync Sentinel and settings can be set by the user */
} OLT_FG_SYNC_SENTINEL_BITMASK;

#else  // DT_MIDL_PASS

typedef ULNG OLT_FG_SYNC_SENTINEL_BITMASK;
#define OLC_FG_SYNC_SENTINEL_FIXED			0x00000001	/* Frame Grabber has Sync Sentinel but has fixed settings */
#define OLC_FG_SYNC_SENTINEL_VARIABLE		0x00000002	/* Frame Grabber has Sync Sentinel and settings can be set by the user */

#endif // DT_MIDL_PASS


/* Event bit masks */
#define OLC_FG_EC_FRAME				0x00000001	/* Driver can count frame events */
#define OLC_FG_EC_FIELD				0x00000002	/* Driver can count field events */
#define OLC_FG_EC_EXT_HI_TO_LO			0x00000004	/* Driver can count high-to-low transitions on an external line */
#define OLC_FG_EC_EXT_LO_TO_HI			0x00000008	/* Driver can count low-to-high transitions on an external line */


/* Input trigger bit masks */
#define OLC_FG_TRIG_EXTERNAL_LINE		0x00000001	/* Frame grabber supports externally triggered acquisitions */
#define OLC_FG_TRIG_EVENT			0x00000002	/* Frame grabber supports event triggered acquisitions */
#define OLC_FG_TRIG_ONE_EVENT_DELAY		0x00000004	/* Frame grabber supports acquisitions triggered on the event after the event counter fires */


/* Multiple frame acquire trigger mode bit masks */
#define OLC_FG_MODE_START			0x00000001	/* Frame grabber can start a series of multiple acquisitions with a single trigger */
#define OLC_FG_MODE_EACH			0x00000002	/* Frame grabber can acquire a series of where each image in the series requires a trigger */


/* Input "Frame" type bit masks */
#define OLC_FG_FRM_IL_FRAME_EVEN		0x00000001	/* Interlaced frame - acquisition on next even field */
#define OLC_FG_FRM_IL_FRAME_ODD			0x00000002	/* Interlaced frame - acquisition on next odd field */
#define OLC_FG_FRM_IL_FRAME_NEXT		0x00000004	/* Interlaced frame - acquisition on next field */
#define OLC_FG_FRM_FIELD_EVEN			0x00000008	/* Single field - acquisition on next even field */
#define OLC_FG_FRM_FIELD_ODD			0x00000010	/* Single field - acquisition on next odd field */
#define OLC_FG_FRM_FIELD_NEXT			0x00000020	/* Single field - acquisition on next field */
#define OLC_FG_FRM_NON_INTERLACED		0x00000040	/* Non-Interlaced frame - acquisition on next frame */


/* Events */
typedef enum olOLT_FG_EVENT   {	OLC_FG_EVENT_FRAME = 1,		/* Event is End of Frame */
				OLC_FG_EVENT_FIELD,		/* Event is End of Field (vsync) */
				OLC_FG_EVENT_EXT_HI_TO_LO,	/* Event is Low-to-High transition on external event line */
				OLC_FG_EVENT_EXT_LO_TO_HI	/* Event is High-to-Low transition on external event line */
			      } OLT_FG_EVENT, *POLT_FG_EVENT, FAR *LPOLT_FG_EVENT;


/* Triggers */
typedef enum olOLT_FG_TRIGGER	{OLC_FG_TRIGGER_NONE = 1,		/* There is no hw trigger; sw trigger */
				 OLC_FG_TRIGGER_EXTERNAL_LINE,		/* Trigger on external trigger line */
				 OLC_FG_TRIGGER_EVENT,			/* Trigger on event counter signal */
				 OLC_FG_TRIGGER_ONE_EVENT_DELAY		/* Trigger on next event after event counter fires */
			        } OLT_FG_TRIGGER, *POLT_FG_TRIGGER, FAR *LPOLT_FG_TRIGGER;


/* Triggers */
typedef enum olOLT_FG_TRIGGER_MODE	{OLC_FG_TRIGMODE_TO_START = 1,	/* Single trigger starts multiple acquire */
				 	 OLC_FG_TRIGMODE_FOR_EACH	/* One trigger for each acquisition in series */
				        } OLT_FG_TRIGGER_MODE, *POLT_FG_TRIGGER_MODE, FAR *LPOLT_FG_TRIGGER_MODE;

/* Strobes */
typedef enum olOLT_FG_STROBE	
{
	OLC_FG_STROBE_NOW= 1,
	OLC_FG_STROBE_FRAME_BASED= 2,
	OLC_FG_STROBE_FIELD_BASED= 4

} OLT_FG_STROBE, *POLT_FG_STROBE, FAR *LPOLT_FG_STROBE;


/* pixel element bit justification enumerations */
typedef enum olOLT_FG_JUSTIFY
	{
	 OLC_FG_JUSTIFY_RIGHT = 0,
	 OLC_FG_JUSTIFY_LEFT = 1
	} OLT_FG_JUSTIFY_KEY, *POLT_FG_JUSTIFY_KEY, FAR * LPOLT_FG_JUSTIFY_KEY;


/* Mono Frame Grabber Linear Memory Capability ("LC") Keys */
typedef enum olOLT_FG_LINEAR_CAP_KEY
	{
	 OLC_FG_LC_UNKNOWN = 0,
	 OLC_FG_LC_OPS = 1,
	 OLC_FG_LC_LINSIZE,
	 OLC_FG_LC_LINBASE32
	} OLT_FG_LINEAR_CAP_KEY, *POLT_FG_LINEAR_CAP_KEY, FAR *LPOLT_FG_LINEAR_CAP_KEY;


/* Linear memory operation bit masks */
#define OLC_FG_LINOP_MULTACQ		0x00000001


/* Mono Frame Grabber Camera Control Capability ("CC") Keys */
typedef enum olOLT_FG_CAMCTL_CAP_KEY
	{
	 OLC_FG_CC_UNKNOWN = 0,
	 OLC_FG_CC_DIG_OUT_COUNT = 1,
	 OLC_FG_CC_PULSE_OPS,
	 OLC_FG_CC_PULSE_RANGE,
	 OLC_FG_CC_PULSE_RANGE_EXT
	} OLT_FG_CAMCTL_CAP_KEY, *POLT_FG_CAMCTL_CAP_KEY, FAR *LPOLT_FG_CAMCTL_CAP_KEY;


/* Pulse operation bit masks */
#define OLC_FG_PULSE_PING		0x00000001

/* PassthruEx flags */
#define OLC_FG_PASSTHRU_DONT_OVERWRITE			0x00000001	/* Dont overwrite a buffer before it's  reset */

/* Mono Frame Grabber Passthru Capability ("PC") Keys */
typedef enum olOLT_FG_PASSTHRU_CAP_KEY
	{
	 OLC_FG_PC_UNKNOWN = 0,
	 OLC_FG_PC_DOES_PASSTHRU			= 0x0100,
	 OLC_FG_PC_PASSTHRU_MODE_LIMITS		= 0x0101,
	 OLC_FG_PC_DOES_SOURCE_ORIGIN		= 0x0110,
	 OLC_FG_PC_SRC_ORIGIN_X_LIMITS		= 0x0111,
	 OLC_FG_PC_SRC_ORIGIN_Y_LIMITS		= 0x0112,
	 OLC_FG_PC_DOES_SCALING				= 0x0120,
	 OLC_FG_PC_SCALE_HEIGHT_LIMITS		= 0x0121,
	 OLC_FG_PC_SCALE_WIDTH_LIMITS		= 0x0122,
	 OLC_FG_PC_DOES_PASSTHRU_LUT		= 0x0123,
	 OLC_FG_PC_MAX_PLUT_INDEX			= 0x0124,
	 OLC_FG_PC_MAX_PLUT_VALUE			= 0x0125,
	 OLC_FG_PC_MAX_PALETTE_INDEX		= 0x0126,
	 OLC_FG_PC_MAX_PALETTE_VALUE		= 0x0127,
	 OLC_FG_PC_DOES_PASSTHRU_SNAPSHOT	= 0x0128
	} OLT_FG_PASSTHRU_CAP_KEY, *POLT_FG_PASSTHRU_CAP_KEY, FAR *LPOLT_FG_PASSTHRU_CAP_KEY;

/* Passthru Type Bit Masks */

#ifdef DT_MIDL_PASS

typedef enum olOLT_FG_PASSTHRU_BITMASK
{
    OLC_FG_PASSTHRU_ASYNC_DIRECT    = 0x00000001,		/* Driver provides direct to display asynchronous passthru */
    OLC_FG_PASSTHRU_SYNC_DIRECT     = 0x00000002,		/* Driver provides direct to display synchronous passthru */
    OLC_FG_PASSTHRU_ASYNC_BITMAP    = 0x00000004,		/* Driver provides bitmap to display asynchronous passthru */
    OLC_FG_PASSTHRU_SYNC_BITMAP     = 0x00000008,		/* Driver provides bitmap to display synchronous passthru */
	OLC_FG_PASSTHRU_ASYNC_BITMAP_EX = 0x00000010,		/* Driver provides bitmap to display asynchronous passthruex */
} OLT_FG_PASSTHRU_BITMASK;

#else  // DT_MIDL_PASS

typedef ULNG OLT_FG_PASSTHRU_BITMASK;
#define OLC_FG_PASSTHRU_ASYNC_DIRECT    0x00000001		/* Driver provides direct to display asynchronous passthru */
#define OLC_FG_PASSTHRU_SYNC_DIRECT     0x00000002		/* Driver provides direct to display synchronous passthru */
#define OLC_FG_PASSTHRU_ASYNC_BITMAP    0x00000004		/* Driver provides bitmap to display asynchronous passthru */
#define OLC_FG_PASSTHRU_SYNC_BITMAP     0x00000008		/* Driver provides bitmap to display synchronous passthru */
#define OLC_FG_PASSTHRU_ASYNC_BITMAP_EX  0x00000010		/* Driver provides bitmap to display asynchronous passthruex */

#endif // DT_MIDL_PASS

/* Mono Frame Grabber Memory Capability ("MC") Keys */
typedef enum olOLT_FG_MEM_CAP_KEY
	{
	 OLC_FG_MC_UNKNOWN = 0,
	 OLC_FG_MC_MEMORY_TYPES = 1,
	 OLC_FG_MC_VOL_COUNT,
	 OLC_FG_MC_NONVOL_COUNT
	} OLT_FG_MEM_CAP_KEY, *POLT_FG_MEM_CAP_KEY, FAR *LPOLT_FG_MEM_CAP_KEY;


/* Memory Type Bit Masks */
#define OLC_FG_MEM_VOLATILE			0x00000001		/* Driver provides volatile frames */
#define OLC_FG_MEM_NON_VOLATILE			0x00000002		/* Driver provides non-volatile frames */


/* Used to indicate that next available built-in frame should be allocated. */
#define OLC_FG_NEXT_FRAME	0xffff


/* Memory Types */
typedef enum olOLT_FG_DEV_MEM { OLC_FG_DEV_MEM_VOLATILE = 1,
				OLC_FG_DEV_MEM_NONVOLATILE
			      } OLT_FG_DEV_MEM, *POLT_FG_DEV_MEM, FAR *LPOLT_FG_DEV_MEM;

#ifndef DT_MIDL_PASS
/* Frame information */
typedef struct olOLT_FG_FRAME_INFO
	{
	 ULNG StructSize;		/* Size of structure; filled in by caller */
	 LPVOID BaseAddress;		/* Pointer to first pixel in buffer */
	 ULNG Flags;			/* Flags defining certain frame characteristics */
	 OLT_FG_DEV_MEM MemType;	/* Type of device memory from which the frame was created */
	 USHRT Width;			/* Number of pixels per line */
	 USHRT Height;			/* Number of lines per frame */
	 USHRT BytesPerSample;		/* Number of bytes/pixel element */
	 USHRT SamplesPerPixel;		/* Number of pixel elements per pixel (ie: RGB color pixel has 3 elements) */
	 USHRT HorizontalPitch;		/* Number of pixels between sequentially pixels */
	 USHRT VerticalPitch;		/* Number of pixels between the first pixels in sequential rows */
	 USHRT BitsPerSample;		/* Number of bits in each element that make up a pixel */
	 OLT_FG_JUSTIFY_KEY BitJustification; /* Specifies whether bits in pixel element are right or left justified 
                                           OLC_FG_JUSTIFY_RIGHT or OLC_FG_JUSTIFY_LEFT */

	} OLT_FG_FRAME_INFO, *POLT_FG_FRAME_INFO, FAR *LPOLT_FG_FRAME_INFO;
#endif // DT_MIDL_PASS

/* Frame info flags */
#define OLC_FG_FRAME_CAN_MAP	0x0001

#ifndef EXCLUDE_DDI
/*----------------------- DDI additions --------------------------------*/

/* Frame Grabber DDI Query Keys Capability Keys */
typedef enum olOLT_FG_DDI_CAP_KEY
{
	OLC_FG_DDI_UNKNOWN,
	OLC_FG_DDI_FAST_PASSTHRU,
	OLC_FG_DDI_OVERLAYS,
	OLC_FG_DDI_TRANSLUCENT_OVERLAYS,
	OLC_FG_DDI_COLOR_OVERLAY,
	OLC_FG_DDI_MULTIPLE_SURFACES,
	OLC_FG_DDI_COLOR_KEY_CONTROL,
	OLC_FG_DDI_OVERLAY_ON_FRAME,
	OLC_FG_DDI_USER_SURFACE_PTR,
	OLC_FG_DDI_PASSTHRU_SYNC_EVENT
} OLT_FG_DDI_CAP_KEY, *POLT_FG_DDI_CAP_KEY, FAR *LPOLT_FG_DDI_CAP_KEY;




/* OlFgAddOverlayToFrame Flags */
#define OLC_SS_OPAQUE			0x00
#define	OLC_SS_TRANSLUCENT		0x01

// Number of circular buffers
#define OLC_FG_MAX_FRAMES		0x1000	
#endif


/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* OLWINTYP_H__ */
