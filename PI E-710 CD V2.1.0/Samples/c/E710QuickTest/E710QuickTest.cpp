// E710QuickTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "E710QuickTest.h"
#include "..\..\Release\E7XX_GCS_DLL\E7XX_GCS_DLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CE710QuickTestApp

BEGIN_MESSAGE_MAP(CE710QuickTestApp, CWinApp)
	//{{AFX_MSG_MAP(CE710QuickTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CE710QuickTestApp construction

CE710QuickTestApp::CE710QuickTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CE710QuickTestApp object

CE710QuickTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CE710QuickTestApp initialization

BOOL CE710QuickTestApp::InitInstance()
{
	CString		sMessage;
	CString		sIDN;
double pos[4];

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

//	CE710QuickTestDlg dlg;
//	m_pMainWnd = &dlg;
	int nResponse = E7XX_InterfaceSetupDlg(NULL);
	if (nResponse < 0)
	{
		sMessage.Format("ERROR: The controller couldn't be found.\nPlease be sure, that the controller is connected to the selected port!");
		MessageBox(NULL, (LPCTSTR)sMessage, "E710 QuickTest", MB_ICONERROR);
	}
	else
	{
		if(E7XX_qIDN(nResponse, sIDN.GetBuffer(1023), 1024) == FALSE)
		{
			E7XX_TranslateError(E7XX_GetError(nResponse), sMessage.GetBuffer(1023), 1024);
			sMessage.ReleaseBuffer();
			MessageBox(NULL, (LPCTSTR)sMessage, "E710 QuickTest", MB_ICONERROR);
		}
		else
		{
			sIDN.ReleaseBuffer();
			
			sMessage = (CString)"The connection to the controller was successful.\n\n The identification returned: " + 
						sIDN;

			MessageBox(NULL, (LPCTSTR)sMessage, "E710 QuickTest", MB_ICONINFORMATION);
		}
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
