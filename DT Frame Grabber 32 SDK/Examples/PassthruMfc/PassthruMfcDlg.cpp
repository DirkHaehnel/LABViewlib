// PassthruMfcDlg.cpp : implementation file

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
#include "olfgapi.h"
#include "olimgapi.h"
#include "resource.h"
#include "Display.h"
#include "PassthruMfc.h"
#include "PassthruMfcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
//
// CONSTRUCTOR: CDlgSelectDevice::CDlgSelectDevice()
//
// DESCRIPTION: Constructor for the IDD_DEVICELIST ("Select Device") dialog 
//      box.  This is called when the class is first being instantiated 
//      (constructed) and is used to initialize member variables.
//
///////////////////////////////////////////////////////////////////////////////
CDlgSelectDevice::CDlgSelectDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectDevice)
	m_strAlias = _T("");   // Initialize the alias string to an empty string
	//}}AFX_DATA_INIT
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CDlgSelectDevice::DoDataExchange()
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
//      In this example, when the dialog box is first created, the value of 
//      m_strAlias gets placed in the Alias combo box.  When the OnOK method
//      (the "Done") button is pressed, the string in the Alias combo box is
//      placed in m_strAlias.  
//
///////////////////////////////////////////////////////////////////////////////
void CDlgSelectDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectDevice)

    // This associates the m_strAlias member variable with the "Alias"
    // combo box control.
	DDX_CBString(pDX, IDC_CB_ALIAS, m_strAlias);
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////
//
// CLASS: CDlgSelectDevice Message Map
//
// This is used to call certain functions when the user makes changes to the 
// various controls in the dialog.  
//
// This is not used in  the IDD_DEVICELIST ("Select Alias") dialog.
//
///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDlgSelectDevice, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CDlgSelectDevice::OnInitDialog()
//
// DESCRIPTION: This gets called by the Windows framework in response to the
//      Create, CreateIndirect, or DoModal calls.  It is used to initialize
//      the dialog box controls.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CDlgSelectDevice::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // This centers the "Select Alias" dialog over its parent window (the main
    // app window for this application.
    CenterWindow();
    
    // Get the number of devices (aliases) available in the system
    int iCount = 0;
    OLT_APISTATUS Status = OlImgGetDeviceCount(&iCount);
    // Check for errors
    if (Status != OLC_STS_NORMAL)
    {
        AfxMessageBox("Unable to get Alias count");
        // Close the dialog box and set -1 as the return value of 
        // the DoModal call
        EndDialog(-1);
    }
    if (!iCount)
    {
        AfxMessageBox("No apparent Aliases have been established.");
        // Close the dialog box and set -1 as the return value of 
        // the DoModal call
        EndDialog(-1);
    }
    
    // Create a pointer to an array of structures containing frame grabber
    // board information.  The array has the same number of elements as the
    // current number of aliases.
    LPOLT_IMGDEVINFO lpDevInfoList = new OLT_IMGDEVINFO[iCount];

    // Fill in StructSize member of the first OLT_IMGDEVINFO structure 
    // in the array of structures.  Required by OlImgGetDeviceInfo().
    lpDevInfoList->StructSize = sizeof(OLT_IMGDEVINFO);
    // Get information about each of the DT-Open Layers frame grabber boards 
    // installed in your system.  This information is placed in the 
    // lpDevInfoList array pointer.
    Status = OlImgGetDeviceInfo(lpDevInfoList, iCount * sizeof(OLT_IMGDEVINFO));
    // Check for errors
    if (Status != OLC_STS_NORMAL)
    {
        AfxMessageBox("Unable to get the device information.");
        // Close the dialog box and set -1 as the return value of 
        // the DoModal call
        EndDialog(-1);
    }
    
    // Add the aliases to the combo box
    for (int idx = 0; idx < iCount; idx++)
    {
        ((CComboBox*)GetDlgItem(IDC_CB_ALIAS))->AddString(lpDevInfoList[idx].Alias);
    }
    
    // Set the current selection in the combo box to the first
    // alias in the list of items in the combo box.
    ((CComboBox*)GetDlgItem(IDC_CB_ALIAS))->SetCurSel(0);
    
    // We are done with this array pointer, so delete it. 
    // This frees the memory.
    delete [] lpDevInfoList;
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
// CONSTRUCTOR: CPassthruMfcDlg::CPassthruMfcDlg()
//
// DESCRIPTION: Constructor for the IDD_PASSTHRUMFC_DIALOG dialog 
//      box.  This is called when the class is first being instantiated 
//      (constructed) and is used to initialize member variables.
//
///////////////////////////////////////////////////////////////////////////////
CPassthruMfcDlg::CPassthruMfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassthruMfcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassthruMfcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon     = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pDisplay  = NULL;
    m_DevId     = 0;
    m_Flags     = 0;
    m_strAlias.Empty();
}

///////////////////////////////////////////////////////////////////////////////
//
// CDESTRUCTOR: CPassthruMfcDlg::~CPassthruMfcDlg()
//
// DESCRIPTION: Make sure any open device is closed.
//
///////////////////////////////////////////////////////////////////////////////
CPassthruMfcDlg::~CPassthruMfcDlg()     
{
    CloseDevice();
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::DoDataExchange()
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
//      the IDD_PASSTHRUMFC_DIALOG UI and its associated member variables.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassthruMfcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////
//
// CLASS: CDlgSelectDevice Message Map
//
// This is used to call certain functions when the user makes changes to the 
// various controls in the dialog.  
//
// For example, when the IDC_SELECTDEVICE ("Select Device") button is 
// pressed (clicked), the OnSelectDevice() method is called.
//
///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPassthruMfcDlg, CDialog)
	//{{AFX_MSG_MAP(CPassthruMfcDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTDEVICE, OnSelectDevice)
	ON_BN_CLICKED(IDC_CLOSEDEVICE,  OnCloseDevice)
	ON_BN_CLICKED(IDC_PASSTHRU,     OnStartPassthru)
	ON_BN_CLICKED(IDC_STOPPASSTHRU, OnStopPassthru)
	ON_BN_CLICKED(IDC_SINGLEFRAME,  OnSingleFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OnInitDialog()
//
// DESCRIPTION: This gets called by the Windows framework in response to the
//      Create, CreateIndirect, or DoModal calls.  It is used to initialize
//      the dialog box controls.  
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruMfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    // Set the enable state of the buttons.
    DlgControls();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::DlgControls()
//
// DESCRIPTION: 
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::DlgControls()
{
    BOOL bSelected = m_Flags & FLG_DEVICESELECTED;
    BOOL bPassthru = m_Flags & FLG_PASSTHRU;
    
    GetDlgItem(IDC_SELECTDEVICE)->EnableWindow(!bSelected);
    GetDlgItem(IDC_CLOSEDEVICE)->EnableWindow(bSelected && !bPassthru);
    GetDlgItem(IDC_PASSTHRU)->EnableWindow(!bPassthru && bSelected);
    GetDlgItem(IDC_STOPPASSTHRU)->EnableWindow(bPassthru && bSelected);
    GetDlgItem(IDC_SINGLEFRAME)->EnableWindow(bSelected && !bPassthru);
    GetDlgItem(IDOK)->EnableWindow(!bPassthru);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OnPaint()
//
// DESCRIPTION: If you add a minimize button to your dialog, you will need the
//      code below to draw the icon.  For MFC applications using the 
//      document/view model, this is automatically done for you by the 
//      framework.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OnQueryDragIcon()
//
// DESCRIPTION: The system calls this to obtain the cursor to display while 
//      the user drags the minimized window.
//
///////////////////////////////////////////////////////////////////////////////
HCURSOR CPassthruMfcDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OpenDevice()
//
// DESCRIPTION: This method opens the device whose alias name is passed in.
//
// ARGUMENTS: LPCSTR alias - Alias name 
// 
// RETURN: TRUE if able to open the device.  FALSE otherwise.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruMfcDlg::OpenDevice(LPCSTR alias)
{
    // Open the device.  It returns a value in m_DevId, which
    // is a device ID that is unique to the board just opened.
    if (OlImgOpenDevice(alias, &m_DevId) == OLC_STS_NORMAL)
    {
        // Get the Dialog window's text and concatenate the alias name
        // of the device that has been opened.
        m_strAlias = alias;
        CString title;
        GetWindowText(title);
        title += ": :";
        title += m_strAlias;
        SetWindowText(title);
        return TRUE;
    }
    // Error: unable to open the device
    else
    {
        m_strAlias.Empty();
        m_DevId = 0;
        return FALSE;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OnSelectDevice()
//
// DESCRIPTION: Called when the user presses the IDC_SELECTDEVICE button.
//      This creates the IDD_DEVICELIST ("Select Device") dialog box.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::OnSelectDevice() 
{
    // Create an instance of the CDlgSelectDevice class
    CDlgSelectDevice dlg;
    // Show the dialog
    if (dlg.DoModal() == IDOK)
    {
        // User pressed the IDOK ("Done") button, so take the alias string
        // that was selected in the combo box and open that device.
        if (OpenDevice(dlg.m_strAlias))
        {
            // Set the showing that the device has been selected
            m_Flags |= FLG_DEVICESELECTED;
            // Update the enable state of the dialog box's controls
            DlgControls();
        }
        else
            MessageBox("Unable to Open the device.");
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::ShowDisplayWindow()
//
// DESCRIPTION: Show the window that will display the image.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::ShowDisplayWindow() 
{
    // If the display window has not already been created, create it.
    if (m_pDisplay == NULL)
    {
        m_pDisplay = new CDisplay(this);
        m_pDisplay->Create(IDD_DISPLAY);
        m_pDisplay->MoveWindow(0,0,640, 480);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::HideDisplayWindow()
//
// DESCRIPTION: Closes the window that displays the image.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::HideDisplayWindow() 
{
    // If the window has not already been destroyed, destroy it now.
    if (m_pDisplay)
    {
        m_pDisplay->DestroyWindow();
        
        delete m_pDisplay;
    }
    m_pDisplay = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::CloseDevice()
//
// DESCRIPTION: Closes the device currently open.
//
// RETURN: TRUE if able to close the device.  FALSE otherwise.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruMfcDlg::CloseDevice()
{
    if (OlImgCloseDevice(m_DevId) == OLC_STS_NORMAL)
        return TRUE;
    else
        return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OnCloseDevice()
//
// DESCRIPTION: Called when the user presses the IDC_CLOSEDEVICE
//      ("Close Device") button.  This closes the current open device,
//      resets the windows text, and hides the display window.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::OnCloseDevice() 
{
    // No device is open
    if (m_strAlias.IsEmpty())
        return;     
    // Close the device
    if (CloseDevice())
    {
        m_strAlias.Empty();
        m_DevId = 0;
        // Update the flags
        m_Flags &= ~(FLG_DEVICESELECTED);
        // Update the enable state of the buttons
        DlgControls();
        // Remove the alias name from the window text
        CString title;
        GetWindowText(title);
        SetWindowText(title.SpanExcluding("-:"));
        // Hide the display window
        HideDisplayWindow();
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OnSingleFrame()
//
// DESCRIPTION: Called when the user presses the IDC_SINGLEFRAME ("Single  
//      Frame") button.  It creates the display window and grabs a single
//      frame.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::OnSingleFrame() 
{
    OLT_FG_FRAME_ID frameid;
    
    // Allocate a frame.  The frameid is passed back if successful.
    if (OlFgAllocateBuiltInFrame(m_DevId, OLC_FG_DEV_MEM_VOLATILE, 0, 
        &frameid) != OLC_STS_NORMAL)
    {
        MessageBox("OlFgAllocateBuiltInFrame() failed");   
        return;
    }
    
    // Set the member variable
    m_Frames[0] = (DWORD)frameid;
    m_framecount = 1;
    
    // Create/show the display window
    ShowDisplayWindow();
    
    // Acquire a single frame
    if (OlFgAcquireFrameToDevice(
        m_DevId,                // device handle
        frameid                 // ptr to frame ID array
        ) != OLC_STS_NORMAL)
    {
        // Error: unable to acquire frame
        DestroyFrames();
        HideDisplayWindow();
        MessageBox("OlFgAcquireFrameToDevice() failed");
        return;
    }
    
    // Draw the acquired frame
    if (OlFgDrawAcquiredFrame(
        m_DevId,                // device handle
        m_pDisplay->m_hWnd,     // display window
        frameid                 // ptr to frame ID array
        ) != OLC_STS_NORMAL)
    {
        // Error: unable to draw the frame
        DestroyFrames();
        HideDisplayWindow();
        MessageBox("OlFgAcquireFrameToDevice() failed");
        return;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OnStartPassthru()
//
// DESCRIPTION: Called when the user presses the IDC_PASSTHRU ("Start 
//      Passthru") button.  It creates the display window and starts the
//      passthru operation.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::OnStartPassthru() 
{
    OLT_FG_FRAME_ID frameid;
    
    // Allocate a frame.  The frameid is passed back if successful.
    if (OlFgAllocateBuiltInFrame(m_DevId, OLC_FG_DEV_MEM_VOLATILE, 0, 
        &frameid) != OLC_STS_NORMAL)
    {
        MessageBox("OlFgAllocateBuiltInFrame() failed");   
        return;
    }
    
    // Set the member variable
    m_Frames[0] = (DWORD)frameid;
    m_framecount = 1;
    
    // Create/show the display window
    ShowDisplayWindow();
    
    // Start passthru
    if (OlFgStartAsyncPassthruBitmap(
        m_DevId,                // device handle
        m_pDisplay->m_hWnd,     // display window
        frameid,                // ptr to frame ID array
        &m_Job                  // Asynch job
        ) != OLC_STS_NORMAL)
    {
        // Error: unable to run passthru
        DestroyFrames();
        HideDisplayWindow();
        MessageBox("OlFgStartAsyncPassthruBitmap() failed");
        return;
    }
    // Set passthru flag 
    m_Flags |= FLG_PASSTHRU;
    // Update the enable state of the buttons
    DlgControls();
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::OnStopPassthru()
//
// DESCRIPTION: Called when the user presses the IDC_STOPPASSTHRU ("Stop 
//      Passthru") button. 
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruMfcDlg::OnStopPassthru() 
{
    OLT_APISTATUS result;
    // Stop the passthru operation
    if (OlFgStopAsyncPassthru(m_DevId, m_Job, &result) != OLC_STS_NORMAL)
    { 
        MessageBox("OlFgStopAsyncPassthru() failed");   
        return; 
    }
    // Close display
    HideDisplayWindow();
    // Destroy any allocated frames
    DestroyFrames();
    // Reset the flags
    m_Flags &= ~(FLG_PASSTHRU);
    // Update the enable state of the buttons
    DlgControls();
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruMfcDlg::DestroyFrames()
//
// DESCRIPTION: Destroys all allocated frames.
//
// RETURN: TRUE if able to destroy all of the allocated frame.  
//         FALSE otherwise.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruMfcDlg::DestroyFrames()
{
    BOOL bresult = TRUE;
    // Loop through all frames and destroy them.
    for (int idx = 0; idx < m_framecount; idx++)
    {
        if (OlFgDestroyFrame(m_DevId, (OLT_FG_FRAME_ID)(m_Frames[idx])) != OLC_STS_NORMAL)
            bresult = FALSE;
    }
    if (bresult == FALSE)
        MessageBox("Unable to destroy all the allocated frames");
    return bresult;
}

