
// LogPing.h :
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH'stdafx.h'"
#endif

#include "resource.h"		


// CLogPingApp:
//
//

class CLogPingApp : public CWinApp
{
public:
	CLogPingApp();

// 
public:
	virtual BOOL InitInstance();

//

	DECLARE_MESSAGE_MAP()
};

extern CLogPingApp theApp;
