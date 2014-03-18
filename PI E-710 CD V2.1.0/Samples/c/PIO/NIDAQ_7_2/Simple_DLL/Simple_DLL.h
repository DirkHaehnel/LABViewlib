// Simple_DLL.h : Hauptheaderdatei für die Simple_DLL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error 'stdafx.h' muss vor dieser Datei in PCH eingeschlossen werden.
#endif

#include "resource.h"		// Hauptsymbole


// CSimple_DLLApp
// Siehe Simple_DLL.cpp für die Implementierung dieser Klasse
//

class CSimple_DLLApp : public CWinApp
{
public:
	CSimple_DLLApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
