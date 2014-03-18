// Display.cpp : implementation file

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

#include "stdafx.h"
#include "resource.h"
#include "olfgapi.h"
#include "olimgapi.h"
#include "Display.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplay dialog

///////////////////////////////////////////////////////////////////////////////
//
// CONSTRUCTOR: CDisplay::CDisplay()
//
// DESCRIPTION: Constructor for the IDD_DISPLAY (display window) dialog 
//      box.  This is called when the class is first being instantiated 
//      (constructed) and is used to initialize member variables.
//
///////////////////////////////////////////////////////////////////////////////
CDisplay::CDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CDisplay::DoDataExchange()
//
// DESCRIPTION: This is a windows function that must be overriden.  It
//      exchanges data between the dialog UI and the class members associated
//      with the UI.  This method is never called directly, but gets called 
//      by the UpdateData method.  When the CDialog::OnInitDialog method gets 
//      called by the Windows framework, UpdateData is called with an input 
//      parameter of FALSE.  This calls CDialog::DoDataExchange, which 
//      initializes the control in the UI to the value  
//      in the corresponding member variable.  When the CDialog::OnOK method
//      gets called, the framework calls UpdateData with an input parameter of
//      TRUE.  This calls CDialog::DoDataExchange, which gets the value in
//      the UI control and sets the corresponding member variable.
//
//      This is not used because no data needs to be exchanged between the
//      the IDD_DISPLAY UI and its associated member variables.
//
///////////////////////////////////////////////////////////////////////////////
void CDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////
//
// CLASS: CDisplay Message Map
//
// This is used to call certain functions when the user makes changes to the 
// various controls in the dialog.  
//
// This is not used in  the IDD_DISPLAY (display window) dialog.
//
///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDisplay, CDialog)
	//{{AFX_MSG_MAP(CDisplay)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplay message handlers
