// PassthruExMfcDlg.cpp : implementation file

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
#include "PassthruExMfc.h"
#include "PassthruExMfcDlg.h"

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
// CONSTRUCTOR: CPassthruExMfcDlg::CPassthruExMfcDlg()
//
// DESCRIPTION: Constructor for the IDD_PASSTHRUMFC_DIALOG dialog 
//      box.  This is called when the class is first being instantiated 
//      (constructed) and is used to initialize member variables.
//
///////////////////////////////////////////////////////////////////////////////
CPassthruExMfcDlg::CPassthruExMfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassthruExMfcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassthruExMfcDlg)
	m_nNumFrames    = 10;
	m_nFrameIndex   = 0;
	m_dwTimeout     = 10;
	//}}AFX_DATA_INIT

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon         = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pDisplay      = NULL;
    m_DevId         = 0;
    m_Flags         = 0;
    m_strAlias.Empty();
    m_nNumFramesAllocated = 0;
    m_bIsArray      = FALSE;
    m_pFrameId      = NULL;
    m_lpDone        = NULL;
    m_PassJobId     = 0;
    m_PassthruFlags = 0;
    m_pThread       = NULL;
    m_hEvent                = INVALID_HANDLE_VALUE;
    m_hStopPassthruEvent    = INVALID_HANDLE_VALUE;

}

///////////////////////////////////////////////////////////////////////////////
//
// CDESTRUCTOR: CPassthruExMfcDlg::~CPassthruExMfcDlg()
//
// DESCRIPTION: Make sure any open device is closed.
//
///////////////////////////////////////////////////////////////////////////////
CPassthruExMfcDlg::~CPassthruExMfcDlg()     
{
    CloseDevice();
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::DoDataExchange()
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
//      the IDD_PASSTHRUEXMFC_DIALOG UI and its associated member variables.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassthruExMfcDlg)
	DDX_Text(pDX, IDC_NUM_FRAMES, m_nNumFrames);
	DDV_MinMaxInt(pDX, m_nNumFrames, 1, 100);
	DDX_Text(pDX, IDC_FRAME_INDEX, m_nFrameIndex);
	DDX_Text(pDX, IDC_TIMEOUT, m_dwTimeout);
	DDV_MinMaxDWord(pDX, m_dwTimeout, 0, 1000);
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
BEGIN_MESSAGE_MAP(CPassthruExMfcDlg, CDialog)
	//{{AFX_MSG_MAP(CPassthruExMfcDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTDEVICE, OnSelectDevice)
	ON_BN_CLICKED(IDC_CLOSEDEVICE,  OnCloseDevice)
	ON_BN_CLICKED(IDC_PASSTHRU,     OnStartPassthruEx)
	ON_BN_CLICKED(IDC_STOPPASSTHRU, OnStopPassthruEx)
	ON_BN_CLICKED(IDC_DO_NOT_OVERWRITE, OnDoNotOverwrite)
	ON_BN_CLICKED(IDC_TRIG_EXTERNAL, OnTrigExternal)
	ON_BN_CLICKED(IDC_SINGLE, OnSetLpDoneParams)
	ON_BN_CLICKED(IDC_ARRAY, OnSetLpDoneParams)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::OnInitDialog()
//
// DESCRIPTION: This gets called by the Windows framework in response to the
//      Create, CreateIndirect, or DoModal calls.  It is used to initialize
//      the dialog box controls.  
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruExMfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    // Set the enable state of the buttons.
    DlgControls();

    // Set the "lpDone type" radio button to "single"
    CheckDlgButton(IDC_ARRAY, BST_CHECKED);
    // Update the flags for the lpDone parameter
    OnSetLpDoneParams();
    // Set the trigger radio buttons
    CheckDlgButton(IDC_TRIG_START, BST_CHECKED);
    CheckDlgButton(IDC_TRIG_HITOLO, BST_CHECKED);
    CheckDlgButton(IDC_TRIG_EXTERNAL, BST_UNCHECKED);
    // Update the flags and call the necessary SDK functions
    OnTrigExternal();

    // Create an event that is set when the "Stop PassthruEx" button
    // is pressed.
    m_hStopPassthruEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::DlgControls()
//
// DESCRIPTION: Sets the enable state of the "select device", "close device", 
//              "start passthruex" and "stop passthruex" buttons.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::DlgControls()
{
    BOOL bSelected = m_Flags & FLG_DEVICESELECTED;
    BOOL bPassthru = m_Flags & FLG_PASSTHRU;
    BOOL bTrigger = FALSE;
    if (IsDlgButtonChecked(IDC_TRIG_EXTERNAL) == BST_CHECKED)
        bTrigger = TRUE;

    GetDlgItem(IDC_SELECTDEVICE)->EnableWindow(!bSelected);
    GetDlgItem(IDC_CLOSEDEVICE)->EnableWindow(bSelected && !bPassthru);
    GetDlgItem(IDC_PASSTHRU)->EnableWindow(!bPassthru && bSelected);
    GetDlgItem(IDC_STOPPASSTHRU)->EnableWindow(bPassthru && bSelected);

    // These all get disabled when passthru is running
    GetDlgItem(IDOK)->EnableWindow(!bPassthru);
    GetDlgItem(IDC_SINGLE)->EnableWindow(!bPassthru);
    GetDlgItem(IDC_ARRAY)->EnableWindow(!bPassthru);
    GetDlgItem(IDC_DO_NOT_OVERWRITE)->EnableWindow(!bPassthru);
    GetDlgItem(IDC_NUM_FRAMES)->EnableWindow(!bPassthru);

    GetDlgItem(IDC_TRIG_EXTERNAL)->EnableWindow(!bPassthru);
    GetDlgItem(IDC_TRIG_START)->EnableWindow(!bPassthru && bTrigger);
    GetDlgItem(IDC_TRIG_EACH)->EnableWindow(!bPassthru && bTrigger);
    GetDlgItem(IDC_TRIG_HITOLO)->EnableWindow(!bPassthru && bTrigger);
    GetDlgItem(IDC_TRIG_LOTOHI)->EnableWindow(!bPassthru && bTrigger);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::OnPaint()
//
// DESCRIPTION: If you add a minimize button to your dialog, you will need the
//      code below to draw the icon.  For MFC applications using the 
//      document/view model, this is automatically done for you by the 
//      framework.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::OnPaint() 
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
// METHOD CPassthruExMfcDlg::OnQueryDragIcon()
//
// DESCRIPTION: The system calls this to obtain the cursor to display while 
//      the user drags the minimized window.
//
///////////////////////////////////////////////////////////////////////////////
HCURSOR CPassthruExMfcDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::OpenDevice()
//
// DESCRIPTION: This method opens the device whose alias name is passed in.
//
// ARGUMENTS: LPCSTR alias - Alias name 
// 
// RETURN: TRUE if able to open the device.  FALSE otherwise.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruExMfcDlg::OpenDevice(LPCSTR alias)
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
// METHOD CPassthruExMfcDlg::OnSelectDevice()
//
// DESCRIPTION: Called when the user presses the IDC_SELECTDEVICE button.
//      This creates the IDD_DEVICELIST ("Select Device") dialog box.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::OnSelectDevice() 
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
// METHOD CPassthruExMfcDlg::ShowDisplayWindow()
//
// DESCRIPTION: Show the window that will display the image.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::ShowDisplayWindow() 
{
    // If the display window has not already been created, create it.
    if (m_pDisplay == NULL)
    {
        m_pDisplay = new CDisplay(this);
        m_pDisplay->Create(IDD_DISPLAY);
        m_pDisplay->MoveWindow(0,0,640,480);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::HideDisplayWindow()
//
// DESCRIPTION: Closes the window that displays the image.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::HideDisplayWindow() 
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
// METHOD CPassthruExMfcDlg::CloseDevice()
//
// DESCRIPTION: Closes the device currently open.
//
// RETURN: TRUE if able to close the device.  FALSE otherwise.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruExMfcDlg::CloseDevice()
{
    if (OlImgCloseDevice(m_DevId) == OLC_STS_NORMAL)
        return TRUE;
    else
        return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::OnCloseDevice()
//
// DESCRIPTION: Called when the user presses the IDC_CLOSEDEVICE
//      ("Close Device") button.  This closes the current open device,
//      resets the windows text, and hides the display window.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::OnCloseDevice() 
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
// METHOD CPassthruExMfcDlg::OnStartPassthruEx()
//
// DESCRIPTION: Called when the user presses the IDC_PASSTHRU ("Start 
//      PassthruEx") button.  It creates the display window and starts the
//      PassthruEx operation as well as a processing thread.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::OnStartPassthruEx() 
{
    // Get the current data out of the UI controls
    UpdateData();

    // Allocate the necessary memory
    AllocMemory();

    // Set the triggering parameters
    SetTriggerParams();

    // Create/show the display window
    ShowDisplayWindow();
   
    // Reset/initialize the event the triggers the thread to exit
    ResetEvent(m_hStopPassthruEvent);

    // Start passthru
    if (OlFgStartAsyncPassthruEx(
        m_DevId,                // Device handle
        m_pDisplay->m_hWnd,     // Display window
        m_pFrameId,             // Ptr to frame ID array
        m_lpDone,               // Ptr to an int or an array of ints (depending
                                // on the type of lpDone array selected in the UI
        m_nNumFrames,           // Number of frames used to store data 
        &m_hEvent,              // Handle to an event object
        m_PassthruFlags,        // Flags that control the passthru operation
        &m_PassJobId            // Async job id
        ) != OLC_STS_NORMAL)
    {
        // Error: unable to run passthru
        DeallocMemory();
        HideDisplayWindow();
        MessageBox("OlFgStartAsyncPassthruBitmap() failed");
        return;
    }

    // Copy the this pointer into the DISPLAY_THREAD structure
    m_DisplayThread.pThis = this;
    // Create a "worker" thread that will display the index of 
    // current frame 
    AfxBeginThread(DisplayCurrentFrameIndex, &m_DisplayThread);

    // Set passthru flag 
    m_Flags |= FLG_PASSTHRU;
    // Update the enable state of the buttons
    DlgControls();
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::OnStopPassthruEx()
//
// DESCRIPTION: Called when the user presses the IDC_STOPPASSTHRU ("Stop 
//      PassthruEx") button. 
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::OnStopPassthruEx() 
{
    OLT_APISTATUS result, retval;

    // Set the event so that the thread will know to stop
    SetEvent(m_hStopPassthruEvent);

    // Stop the passthru operation
    retval = OlFgStopAsyncPassthru(m_DevId, m_PassJobId, &result);
    if (retval != OLC_STS_NORMAL)
    { 
        // Ignore the return value if passthru has already been stopped
        if (retval != OLC_STS_PASSTHRU_STOPPED)
        {
            MessageBox("OlFgStopAsyncPassthru() failed");   
            return; 
        }
    }

    // Close display
    HideDisplayWindow();
    // Destroy any allocated frames
    DeallocMemory();
    // Reset the flags
    m_Flags &= ~(FLG_PASSTHRU);
    // Update the enable state of the buttons
    DlgControls();
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::DestroyFrames()
//
// DESCRIPTION: Destroys all allocated frames.  Resets the member variable
//              containing the number of allocated frames to 0 if ok, -1 
//              if an error occurred when attempting to destroy the frames.  
//
// RETURN: TRUE if able to destroy all of the allocated frame.  
//         FALSE otherwise.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruExMfcDlg::DestroyFrames()
{
    BOOL bresult = TRUE;
 
    // Loop through all frames that were previously allocated (stored in
    // m_nNumFramesAllocated) and destroy them.
    for (int idx = 0; idx < m_nNumFramesAllocated; idx++)
    {
        if (OlFgDestroyFrame(m_DevId, (OLT_FG_FRAME_ID)(m_pFrameId[idx])) != OLC_STS_NORMAL)
            bresult = FALSE;
    }
    // If there was an problem destroying the frames, warn the user
    if (bresult == FALSE)
    {
        MessageBox("Unable to destroy all the allocated frames");
        // Reset to error state
        m_nNumFramesAllocated = -1;
    }
    else
        // Reset to init state
        m_nNumFramesAllocated = 0;

    return bresult;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::CreateFrames()
//
// PARAMETERS: int nNumFrames:  indicates the # of frames to create/allocate
//
// DESCRIPTION: Allocates as many frames as indicated by the nNumFrames 
//              parameter.
//
// RETURN: TRUE if able to allocate all of the requested frames.  
//         FALSE otherwise.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruExMfcDlg::CreateFrames(int nNumFrames)
{    
    // Allocate a frame.  The frameid is passed back if successful.
    for (int idx = 0; idx < nNumFrames; idx++)
    {
        if (OlFgAllocateBuiltInFrame(m_DevId, OLC_FG_DEV_MEM_VOLATILE, idx, 
            &m_pFrameId[idx]) != OLC_STS_NORMAL)
        {
            MessageBox("OlFgAllocateBuiltInFrame() failed");   
            // Set the number of frames that *did* get allocated
            m_nNumFramesAllocated = idx;
            return FALSE;
        }
    }
    
    // Set the number of frames that were allocated
    m_nNumFramesAllocated = nNumFrames;
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::AllocMemory()
//
// DESCRIPTION: Allocates all memory needed to start passthru
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::AllocMemory()
{
    // Get the latest m_nNumFrames value from the 
    // "frame count" edit box.
    UpdateData();

    // If NULL, then the memory has not been allocated, so
    // allocate it.
    if (m_pFrameId == NULL)
    {
        // Create a pointer to an array of OLT_FG_FRAME_IDs
        m_pFrameId = new OLT_FG_FRAME_ID[m_nNumFrames];
        // Now that the memory has been allocated, allocated the 
        // frames with SDK call.
        CreateFrames(m_nNumFrames);
    }

    // If NULL, then the memory has not been allocated, so
    // allocate it.
    if (m_lpDone == NULL)
    {
        // If the user has selected the "array of indices" radio
        // button, then we must allocate an array of m_lpDone ints.
        // For more documentation on these two options, see the 
        // documentation for OnSetLpDoneParams().
        if (m_bIsArray)
        {
            m_lpDone = new int[m_nNumFrames];
            // Initialize the data to all zeros
            memset(m_lpDone, 0, sizeof(int)*m_nNumFrames);
        }
        // If the user has selected the "single index" radio button, 
        // then then we only need to allocate a single int
        else
        {
            m_lpDone = new int;
            // Initialize to zero
            *m_lpDone = 0;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::DeallocMemory()
//
// DESCRIPTION: Destroys any allocated frames and deletes all memory that 
//              has previously been allocated.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::DeallocMemory()
{
    if (m_pFrameId != NULL)
    {
        // Destroy all allocated frames
        DestroyFrames();

        // Deallocate the memory
        delete [] m_pFrameId;
        // Reset the pointer
        m_pFrameId = NULL;
    }

    if (m_lpDone != NULL)
    {
        // Deallocate the memory.  The delete depends on the 
        // the type of lpDone index that was selected by the user.
        if (m_bIsArray)
            delete [] m_lpDone;
        else
            delete m_lpDone;

        // Reset the pointer
        m_lpDone = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::OnDoNotOverwrite()
//
// DESCRIPTION: Sets/clears the OLC_FG_PASSTHRU_DONT_OVERWRITE flag.  If this
//              flag is set, then PassthruEx will only write to each frame
//              once.  If clear, PassthruEx will continue to loop through each
//              of the frame buffers.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::OnDoNotOverwrite() 
{
    // If the check box is checked, set the flag, else clear it.
    if (IsDlgButtonChecked(IDC_DO_NOT_OVERWRITE) == BST_CHECKED)
        m_PassthruFlags |= OLC_FG_PASSTHRU_DONT_OVERWRITE;
    else
        m_PassthruFlags &= ~(OLC_FG_PASSTHRU_DONT_OVERWRITE);
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::OnTrigExternal()
//
// DESCRIPTION: Enables/disables the state of the trigger controls based on
//              whether the "external trigger" checkbox is checked.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::OnTrigExternal() 
{
    BOOL bEnable = FALSE;

    // If the "external trigger" checkbox is checked, enable the controls.
    // Else, disable the controls.
    if (IsDlgButtonChecked(IDC_TRIG_EXTERNAL) == BST_CHECKED)
        bEnable = TRUE;
   
    GetDlgItem(IDC_TRIG_START)->EnableWindow(bEnable);
    GetDlgItem(IDC_TRIG_EACH)->EnableWindow(bEnable);
    GetDlgItem(IDC_TRIG_HITOLO)->EnableWindow(bEnable);
    GetDlgItem(IDC_TRIG_LOTOHI)->EnableWindow(bEnable);
    GetDlgItem(IDC_TIMEOUT)->EnableWindow(bEnable);
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::SetTriggerParams()
//
// DESCRIPTION: Set the trigger parameters and the timeout period based on
//              the settings in the user interface.
//
// RETURN: TRUE if able to set the trigger/timeout info.  
//         FALSE otherwise.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPassthruExMfcDlg::SetTriggerParams()
{
    BOOL bTrigLoToHi;
    BOOL bOldTrigLoToHi;
    OLT_FG_TRIGGER OldTrigger; 

    // Get the current "timeout" value from the "timeout" control
    UpdateData();

    // Get the transition type
    if (IsDlgButtonChecked(IDC_TRIG_LOTOHI) == BST_CHECKED)
        bTrigLoToHi = TRUE;
    else
        bTrigLoToHi = FALSE;
    
    // The user wants an external trigger
    if (IsDlgButtonChecked(IDC_TRIG_EXTERNAL) == BST_CHECKED)
    {
        // Set an external trigger with the proper transition type
        if (OlFgSetTriggerInfo(m_DevId, OLC_FG_TRIGGER_EXTERNAL_LINE,
            bTrigLoToHi, &OldTrigger, &bOldTrigLoToHi) != OLC_STS_NORMAL)
        {
            MessageBox("OlFgSetTriggerInfo() failed");
            return FALSE;
        }

        // If "start" is set, PassthruEx waits for a trigger to start, but
        // but then proceeds capturing frames.  These flags are sent set 
        // with the "Flags" parameter of OlFgStartAsyncPassthruEx() function.
        if (IsDlgButtonChecked(IDC_TRIG_START) == BST_CHECKED)
        {
            // Set the "start" flag
            m_PassthruFlags |= FLG_TRIGGERSTART;
            // Clear the "each" flag
            m_PassthruFlags &= ~(FLG_TRIGGEREACH);
        }
        // If "each" is set, PassthruEx waits for a trigger before capturing
        // each frame.
        else
        {
            // Clear the "start" flag
            m_PassthruFlags &= ~(FLG_TRIGGERSTART);
            // Set the "each" flag
            m_PassthruFlags |= FLG_TRIGGEREACH;
        }

        // Set the timeout period specified by the user
        ULNG oldTimeOut;
        if (OlImgSetTimeoutPeriod(m_DevId, m_dwTimeout, &oldTimeOut) 
            != OLC_STS_NORMAL)
        {
            MessageBox("OlImgSetTimeoutPeriod() failed");
            return FALSE;
        }
    }
    // The user does not want an external trigger
    else
    {
        if (OlFgSetTriggerInfo(m_DevId, OLC_FG_TRIGGER_NONE,
            bTrigLoToHi, &OldTrigger, &bOldTrigLoToHi) != OLC_STS_NORMAL)
        {
            MessageBox("OlFgSetTriggerInfo() failed");
            return FALSE;
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::OnSetLpDoneParams()
//
// DESCRIPTION: Set the flags that determine the type of lpDone parameter 
//              OlFgStartAsyncPassthruEx() will use.  
//
//              If the "array of indices" radio button 
//              is checked, then PassthruEx behaves in 
//              the old way, in which you must allocate an array of integers 
//              (one for each frame) and pass a pointer to that array into the 
//              "lpDone" parameter.  Each time a frame is captured, the array
//              member at the index corresponding with that frame is incremented.
//
//              In the new way (the user checks the "single index" radio button),
//              you only need to pass a pointer to a single integer into the 
//              "lpDone" parameter of OlFgStartAsyncPassthruEx().  Each time
//              a frame is captured, the frame's index is sent to the "lpDone"
//              parameter.  This implementation makes it much easier to figure
//              out which frame has just been captured.
//
///////////////////////////////////////////////////////////////////////////////
void CPassthruExMfcDlg::OnSetLpDoneParams() 
{
    // "Array of Indices" method
    if (IsDlgButtonChecked(IDC_ARRAY) == BST_CHECKED)
    {
        m_bIsArray = TRUE;
        // Set the flag for the old, "array of indices" method
        m_PassthruFlags |= FLG_FRAMEDONE_OLSDK;
        // Clear the flag for the new, "single index" method
        m_PassthruFlags &= ~(FLG_FRAMEDONE_LASTIDX);
    }
    // "Single Index" method
    else
    {
        m_bIsArray = FALSE;
        // Set the flag for the new, "single index" method
        m_PassthruFlags |= FLG_FRAMEDONE_LASTIDX;
        // Clear the flag for the old, "array of indices" method
        m_PassthruFlags &= ~(FLG_FRAMEDONE_OLSDK);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CPassthruExMfcDlg::DisplayCurrentFrameIndex()
//
// DESCRIPTION: This worker thread is started when PassthruEx is started.  It
//              finds the current frame index and displays it in the "Current
//              Frame Index" edit box in the user interface.  It continues
//              updating the index until either the driver has timed out or
//              the user pressed the "Stop PassthruEx" button.
//
///////////////////////////////////////////////////////////////////////////////
UINT CPassthruExMfcDlg::DisplayCurrentFrameIndex(LPVOID pParam)
{
    DISPLAY_THREAD *DisplayThread = (DISPLAY_THREAD*)pParam;
    CPassthruExMfcDlg *pThis = DisplayThread->pThis;
    DWORD wait;
    HANDLE hObject[2] = {pThis->m_hStopPassthruEvent, pThis->m_hEvent};
    
    // Keep looping until a "break" is reached
    while (TRUE)
    {
        // Wait for an event object to be fired.  If no event is fired, before
        // the timeout period, a message will be sent to the user and the 
        // thread will exit.  The two possible events are when the user presses
        // the "Stop PassthruEx" button or when the hEvent parameter of the 
        // OlFgStartAsyncPassthruEx() function is set (this occurs after each
        // frame is captured).
        wait = WaitForMultipleObjects(2, hObject, FALSE, pThis->m_dwTimeout*1000);
        // The Stop PassthruEx event was triggered (the user pressed
        // the "Stop PassthruEx" button), so exit the thread.
        if (wait == WAIT_OBJECT_0)
            break;
        // A frame was captured (m_hEvent was triggered), so based on the 
        // type of lpDone parameter being used (single or array), find the 
        // current frame index and display it.
        else if (wait == (WAIT_OBJECT_0 + 1))
        {
            // If using an "array of indices", search through each value in
            // the array until we come to one with a value greater than zero.
            // That index indicates the index of the last frame that was captured.
            if (pThis->m_bIsArray)
            {
                for (int idx = 0; idx < pThis->m_nNumFrames; idx++)
                {
                    if (pThis->m_lpDone[idx] > 0)
                    {
                        pThis->m_nFrameIndex = idx;
                        break;
                    }
                }
                // Reset the values in the array to zero
                memset(pThis->m_lpDone, 0, sizeof(int)*pThis->m_nNumFrames);
            }
            // Else, must be using a "single index", so just take the value
            // in the lpDone parameter and display it.  This is SO much easier!!
            else
            {
                pThis->m_nFrameIndex = *pThis->m_lpDone;
            }

            // Set the value in the edit box.
            pThis->SetDlgItemInt(IDC_FRAME_INDEX, pThis->m_nFrameIndex);
        }
        // We have timed out, so call OlFgIsAsyncAcquireJobDone() to see if 
        // the driver has timed out.
        else if (wait == WAIT_TIMEOUT)
        {
            OLT_APISTATUS JobStatus;
            ULNG BytesWrittenToHost;

            // Get the state of the Async Aquire
            if (OlFgIsAsyncAcquireJobDone(pThis->m_DevId, 
                (OLT_FG_ACQJOB_ID)pThis->m_PassJobId, pThis->m_lpDone, 
                &JobStatus, &BytesWrittenToHost) != OLC_STS_NORMAL)
            {
                pThis->MessageBox("OlFgIsAsyncAcquireJobDone() failed");
                break;
            }
            // If timed out, deallocate the memory and update the user 
            // interface to reflect the fact that PassthruEx has 
            // stopped/timed out.
            if (JobStatus == OLC_STS_TIMEOUT)
            {
                // Destroy any allocated frames
                pThis->DeallocMemory();
                // Reset the flags
                pThis->m_Flags &= ~(FLG_PASSTHRU);
                // Update the dialog's controls
                pThis->DlgControls();
                
                pThis->MessageBox("PassthruEx function timed out");
                break;
            }
        }
        else
            break;
    }

    AfxEndThread(1);

    return TRUE;
}


