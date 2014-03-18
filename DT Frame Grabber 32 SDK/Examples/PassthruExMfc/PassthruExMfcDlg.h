// PassthruExMfcDlg.h : header file

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

#if !defined(AFX_PASSTHRUEXMFCDLG_H__75F37F28_9655_4E8E_9728_862102AF0AC9__INCLUDED_)
#define AFX_PASSTHRUEXMFCDLG_H__75F37F28_9655_4E8E_9728_862102AF0AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define FLG_FRAMEDONE_OLSDK     0x08000000 // Driver[0] contains array of Frame-Done variables (OL-SDK)
#define FLG_FRAMEDONE_LASTIDX   0x04000000 // Driver[0] contains address of DWORD holding the last filled frame index
#define FLG_TRIGGERSTART        0x02000000 // Start the continuous grab after receiving an external trigger
#define FLG_TRIGGEREACH         0x01000000 // Wait for external trigger on each grab

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
// CPassthruExMfcDlg dialog
///////////////////////////////////////////////////////////////////////////////
//
// CLASS: CDlgSelectDevice
//
// This is the class for the IDD_DEVICELIST ("Select Device") dialog box.
//
///////////////////////////////////////////////////////////////////////////////
class CPassthruExMfcDlg : public CDialog
{
    // Construction
public:
    CPassthruExMfcDlg(CWnd* pParent = NULL);	// standard constructor
    ~CPassthruExMfcDlg();
    
    // Dialog Data
    CDisplay            *m_pDisplay;    // Pointer the the display window object
    CString     m_strAlias;             // String containing the alias name of the device.
    OLT_IMG_DEV_ID      m_DevId;        // Device Id
    OLT_FG_FRAME_ID     *m_pFrameId;    // Pointer to a frame Id or array of frame Ids
    OLT_FG_PASSJOB_ID   m_PassJobId;    // PassthruEx Job Id
    DWORD       m_Flags;                // Flags for controlling the user interface
    DWORD       m_PassthruFlags;        // Flags for controlling the PassthruEx operation
    int         *m_lpDone;              // Pointer to either a single int, or an array
                                        // of ints (depending on the type of PassthruEx
                                        // operation being performed).  
    HANDLE      m_hEvent;               // Handle to a PassthruEx event.  Is signaled when
                                        // a frame has been acquired.
    HANDLE      m_hStopPassthruEvent;   // Handle to an event that is signaled when
                                        // PassthruEx is stopped.
    CWinThread  *m_pThread;             // Pointer to a thread object
    BOOL        m_bIsArray;             // Indicates the type of lpDone parameter used
    int         m_nNumFramesAllocated;  // The number of frames that have been allocated

    // This structure contains a "this" pointer so that the thread can access
    // various member variables/functions.
    struct DISPLAY_THREAD 
    {
        CPassthruExMfcDlg *pThis;
    };

    DISPLAY_THREAD m_DisplayThread;     // Structure containing a "this" pointer for the thread

    // Flags used in conjunction with m_Flags for controlling the user interface. 
    enum    
    {   
        FLG_DEVICESELECTED  = 0x00000001,
        FLG_PASSTHRU        = 0x00000002,
    };

    //{{AFX_DATA(CPassthruExMfcDlg)
	enum { IDD = IDD_PASSTHRUEXMFC_DIALOG };
	int		m_nNumFrames;   // Corresponds with the "frame count" edit control
	int		m_nFrameIndex;  // Corresponds with the "current frame index" edit control
	DWORD	m_dwTimeout;    // Corresponds with the "timeout" edit control
	//}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPassthruExMfcDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    HICON m_hIcon;
    
    // Dialog Member Functions
    void DlgControls();
    BOOL OpenDevice(LPCSTR alias);
    BOOL CloseDevice();
    void ShowDisplayWindow();
    void HideDisplayWindow();
    void AllocMemory();
    void DeallocMemory();
    BOOL CreateFrames(int nNumFrames);
    BOOL DestroyFrames();
    static UINT DisplayCurrentFrameIndex(LPVOID pParam);
    BOOL SetTriggerParams();

    // Generated message map functions
    //{{AFX_MSG(CPassthruExMfcDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSelectDevice();
    afx_msg void OnCloseDevice();
    afx_msg void OnStartPassthruEx();
    afx_msg void OnStopPassthruEx();
    afx_msg void OnDoNotOverwrite();
    afx_msg void OnTrigExternal();
    afx_msg void OnSetLpDoneParams();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSTHRUEXMFCDLG_H__75F37F28_9655_4E8E_9728_862102AF0AC9__INCLUDED_)
