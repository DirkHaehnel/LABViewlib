/*	File:	3152typ.h

	Description:
		This file contains macros, data structures, and prototypes
		for the DT3152 Frame Grabber software.  The information in
        this file is device dependent, but operating system independent.

	Prerequisites:
		#include <windows.h>
		#include <windowsx.h>
		#include <olwintyp.h>
		#include <3152typ.h>

*/
/*=======================================================================++
||	Copyright (C) 1993 - 1995.  Data Translation, Inc., 100 Locke    ||
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

#if !defined(TYP3152_H__)
#define TYP3152_H__


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

#ifdef DTCII
/* DT-Connect II operation bit masks.  Used when enabling or disabling  */
/* DT-Connect II operational characteristics.                           */

#define DT3152C_DTCII_DISABLE					0x00000001	/* Disable the ability to transfer date over the DT-Connect II port */
#define DT3152C_DTCII_ENABLE					0x00000002	/* Enable the ability to transfer date over the DT-Connect II port */
#define DT3152C_DTCII_ENABLE_SLAVE_TO_ACQUIRE	0x00000004	/* Enable the ability to transfer date over the DT-Connect II port during an acquire */
#define DT3152C_DTCII_DISABLE_SLAVE_TO_ACQUIRE	0x00000008	/* Disable the ability to transfer date over the DT-Connect II port during an acquire */
#endif


/* DT3152 Sync Controls */
typedef enum DT3152_SYNC_CONTROL
	{
	 DT3152_SYNC_CTL_UNKNOWN		= 0x0000,
	 DT3152_SYNC_CTL_HORIZ_FREQ		= 0x0001,
	 DT3152_SYNC_CTL_VERT_FREQ		= 0x0002,
	 DT3152_SYNC_CTL_HPULSE_WIDTH	= 0x0003,
	 DT3152_SYNC_CTL_VPULSE_WIDTH	= 0x0004,
	 DT3152_SYNC_CTL_PHASE			= 0x0005
	 	 
	} DT3152_SYNC_CONTROL, *PDT3152_SYNC_CONTROL, FAR *LPDT3152_SYNC_CONTROL;


typedef enum DT3152_INPUT_CONTROL
	{
	 DT3152_INPUT_CTL_UNKNOWN		= 0x0000,
	 DT3152_INPUT_CTL_GAIN		= 0x0001,
	 DT3152_INPUT_CTL_OFFSET		= 0x0002,
	 DT3152_INPUT_CTL_REFERENCE	= 0x0003,
	 	 
	} DT3152_INPUT_CONTROL, *PDT3152_INPUT_CONTROL, FAR *LPDT3152_INPUT_CONTROL;

/* End of extern "C" {  declaration for C++ callers */
#ifdef __cplusplus
}
#endif

#endif   /* TYP3152_H__ */
