// PassthruMfcDlg.h : header file

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

#if !defined(AFX_PASSTHRUMFCDLG_H__75F37F28_9655_4E8E_9728_862102AF0AC9__INCLUDED_)
#define AFX_PASSTHRUMFCDLG_H__75F37F28_9655_4E8E_9728_862102AF0AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectDevice dialog
///////////////////////////////////////////////////////////////////////////////
//
// CLASS: CDlgSelectDevice
//
// This is the class for the IDD_DEVICELIST ("Select Device") dialog box.
//
///////////////////////////////////////////////////////////////////////////////
class CDlgSelectDevice : public CDialog
{
// Construction
public:
	CDlgSelectDevice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelectDevice)
	enum { IDD = IDD_DEVICELIST };
	CString	m_strAlias;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectDevice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectDevice)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPassthruMfcDlg dialog
///////////////////////////////////////////////////////////////////////////////
//
// CLASS: CDlgSelectDevice
//
// This is the class for the IDD_DEVICELIST ("Select Device") dialog box.
//
///////////////////////////////////////////////////////////////////////////////
class CPassthruMfcDlg : public CDialog
{
    // Construction
public:
    CPassthruMfcDlg(CWnd* pParent = NULL);	// standard constructor
    ~CPassthruMfcDlg();
    
    // Dialog Data
    OLT_IMG_DEV_ID m_DevId;
    CDisplay *m_pDisplay;
    DWORD m_Frames[10];
    OLT_FG_PASSJOB_ID m_Job;
    int m_framecount;
    CString m_strAlias;
    DWORD m_Flags;
    enum    {   FLG_DEVICESELECTED  = 0x00000001,
        FLG_PASSTHRU        = 0x00000002,
    };
    //{{AFX_DATA(CPassthruMfcDlg)
    enum { IDD = IDD_PASSTHRUMFC_DIALOG };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPassthruMfcDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    HICON m_hIcon;
    
    void DlgControls();
    BOOL OpenDevice(LPCSTR alias);
    void ShowDisplayWindow();
    void HideDisplayWindow();
    BOOL DestroyFrames();
    BOOL CloseDevice();
    // Generated message map functions
    //{{AFX_MSG(CPassthruMfcDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSelectDevice();
    afx_msg void OnCloseDevice();
    afx_msg void OnStartPassthru();
    afx_msg void OnStopPassthru();
	afx_msg void OnSingleFrame();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSTHRUMFCDLG_H__75F37F28_9655_4E8E_9728_862102AF0AC9__INCLUDED_)
