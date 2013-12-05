
// LogPing.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CLogPingApp:
// �� Ŭ������ ������ ���ؼ��� LogPing.cpp�� �����Ͻʽÿ�.
//

class CLogPingApp : public CWinApp
{
public:
	CLogPingApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CLogPingApp theApp;
