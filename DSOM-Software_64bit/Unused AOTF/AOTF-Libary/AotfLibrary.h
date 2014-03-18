/****************************************************************
 *																*
 * C Dynamic Link Library (DLL) the AOTF Driver.				*
 * Copyright (C) 2004, Crystal Technology, Inc.					*
 *																*
 * Revision History:											*
 * Date			Who				Comments						*
 * 2004/09/13	Dale Gifford	Genesis.						*
 *																*
 ****************************************************************/
#ifndef __AOTFLIBRARY_H__
#define __AOTFLIBRARY_H__

//
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AOTFLIBRARY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AOTFLIBRARY_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#ifdef AOTFLIBRARY_EXPORTS
#define AOTFLIBRARY_API __declspec (dllexport)
#else
#define AOTFLIBRARY_API __declspec (dllimport)
#endif

extern "C" {
	AOTFLIBRARY_API HANDLE AotfOpen (int iInstance);
	AOTFLIBRARY_API UINT AotfGetInstance (HANDLE hAotfController);
	AOTFLIBRARY_API BOOL AotfClose (HANDLE hAotfController);
	AOTFLIBRARY_API BOOL AotfWrite (HANDLE hAotfController, UINT uiLength, PVOID pData);
	AOTFLIBRARY_API BOOL AotfRead (HANDLE hAotfController, UINT uiLength, PVOID pData, PUINT puiBytesRead);
	AOTFLIBRARY_API BOOL AotfIsReadDataAvailable (HANDLE hAotfController);
	};

#endif
