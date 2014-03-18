// PassthruExMfc.h : main header file for the PASSTHRUMFC application

/*=========================================================================++
||                                                                         ||
||  Copyright (C) 2001.   Data Translation, Inc., 100 Locke Drive,         ||
||  Marlborough Massachusetts 01752-1192.                                  ||
||                                                                         ||
||  The information in this document is subject to change without          ||
||  notice and should not be construed as a commitment by Data             ||
||  Translation, Inc.  Data Translation, Inc. does not assume any          ||
||  responsibility for any errors that may appear in this document.        ||
||                                                                         ||
||  This document, or source code, is provided solely as an example        ||
||  of how to utilize Data Translation software libraries in a sample      ||
||  application.  Data Translation, Inc. does not assume any               ||
||  responsibility for the use or reliability of any portion of this       ||
||  document or the described software.                                    ||
||                                                                         ||
||  General permission to copy or modify, but not for profit, is           ||
||  hereby granted,  provided that the above copyright notice is           ||
||  included and reference made to the fact that reproduction              ||
||  privileges were granted by Data Translation, Inc.                      ||
||                                                                         ||
||  Data Translation cannot assume any responsibility for the use,         ||
||  or misuse, of any portion of this software for other than its          ||
||  intended diagnostic purpose in calibrating and testing Data            ||
||  Translation manufactured image processing boards and software.         ||
||                                                                         ||
++=========================================================================*/

#if !defined(AFX_PASSTHRUEXMFC_H__99D142EC_3A78_4546_9A79_609BB6D665E6__INCLUDED_)
#define AFX_PASSTHRUEXMFC_H__99D142EC_3A78_4546_9A79_609BB6D665E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPassthruExMfcApp:
// See PassthruExMfc.cpp for the implementation of this class
//

class CPassthruExMfcApp : public CWinApp
{
public:
    CPassthruExMfcApp();
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPassthruExMfcApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL
    
    // Implementation
    
    //{{AFX_MSG(CPassthruExMfcApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSTHRUEXMFC_H__99D142EC_3A78_4546_9A79_609BB6D665E6__INCLUDED_)
