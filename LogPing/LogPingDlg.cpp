// LogPingDlg.cpp 

#include "stdafx.h"
#include "LogPing.h"
#include "LogPingDlg.h"
#include "afxdialogex.h"

#include <winsock.h>
#include <Ipexport.h>
#include <Icmpapi.h>

#pragma comment( lib, "Iphlpapi" )
#pragma comment( lib, "wsock32" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) {}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CLogPingDlg
CLogPingDlg::CLogPingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogPingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogPingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLogPingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLogPingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogPingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogPingDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDCANCEL, &CLogPingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDBTN_PAUSE, &CLogPingDlg::OnBnClickedPause)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLogPingDlg 
BOOL CLogPingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// TODO: 
	logFile = 0;
	SetDlgItemText(IDC_EDIT_DIR, "D:\\ping");
	SetDlgItemText(IDC_IP, "211.115.75.243");
//	SetDlgItemText(IDC_IP, "192.168.0.1");
	hIcmpFile = INVALID_HANDLE_VALUE;

	return TRUE;  
}

void CLogPingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CLogPingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CLogPingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* Start Button */
void CLogPingDlg::OnBnClickedOk()
{
	SetTimer(IDH_TIMER, 1000, NULL );
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_IP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	//CDialogEx::OnOK();
}

static int CALLBACK BrowseCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    char   szInitPath[MAX_PATH] = "D:\\";
    if (uMsg == BFFM_INITIALIZED)
    {
        ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (DWORD)szInitPath);         
	}
    return 0;
}

/* Dir Button */
void CLogPingDlg::OnBnClickedButton1()
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];
 
	::ZeroMemory(&BrInfo,sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);
 
	BrInfo.hwndOwner = GetSafeHwnd();

	BrInfo.lpfn = /*(BFFCALLBACK)*/BrowseCallback;
	BrInfo.lpszTitle = _T("Select Log Directory");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
 
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);

	::SHGetPathFromIDList(pItemIdList, szBuffer);

	SetDlgItemText(IDC_EDIT_DIR, szBuffer );
}

/* test button */
void CLogPingDlg::OnBnClickedButton2()
{
	ping(getIpAddress());
}

/* write Log */
void CLogPingDlg::writeLog(const char *msg)
{
	SYSTEMTIME time;
	char szBufTime[512] = "";
	if ( strcmp(logName, getLogName()) != 0 || logFile ==0 )
	{
		if ( logFile == 0 ) {
			logFile = new CFile();
		} 
		else if (logFile->m_hFile != INVALID_HANDLE_VALUE)
		{
			logFile->Close();
		}
		strcpy_s(logName, getLogName());

		if ( logFile->Open(logName, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyNone) == FALSE ) {
			sprintf_s(szBufTime, "%s File Open Error.", logName);
			MessageBox(szBufTime, MB_OK);
			return;
		};
	}

	logFile->SeekToEnd();

	::GetLocalTime(&time);
	sprintf_s(szBufTime, "[%04d-%02d-%02d %02d:%02d:%02d] ", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond );

	strcat_s(szBufTime, msg);
	logFile->Write(szBufTime, strlen(szBufTime));
	logFile->Write("\r\n", 2);

	SetDlgItemText(IDC_LOG, szBufTime);
}

const char *CLogPingDlg::getLogName() const
{
	static char szBuffer[4098] = "";
	char szBufTime[128] = "";
	SYSTEMTIME time;

	GetDlgItemText(IDC_EDIT_DIR, szBuffer, sizeof(szBuffer));

	::GetLocalTime(&time);
	sprintf_s(szBufTime, "\\%04d%02d%02d.%s.log", time.wYear, time.wMonth, time.wDay,  getIpAddress());

	strcat_s(szBuffer, szBufTime );

	return szBuffer;
}

const char *CLogPingDlg::getIpAddress() const
{
	static char szBuffer[128] ="";

	GetDlgItemText(IDC_IP, szBuffer, sizeof(szBuffer));
	return szBuffer;
}

/* ping function */
int CLogPingDlg::ping(const char *ip)
{
	unsigned long ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char  SendData[] = "Data Buffer";
	char  szReplyBuffer[1500];
	char  logBuffer[512] ="";

	if ( strcmp(oldIp, ip) != 0 ) {
		ipaddr = inet_addr(ip);
		if( ipaddr == INADDR_NONE ) {
			return -1;
		}

		hIcmpFile = IcmpCreateFile();
		if( hIcmpFile == INVALID_HANDLE_VALUE )	{
			sprintf_s(logBuffer, "IcmpCreatefile error: %ld", GetLastError());
			writeLog(logBuffer);
			return -2;
		}    

		strcpy_s( oldIp, ip);
	}

	dwRetVal = IcmpSendEcho( hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, szReplyBuffer, sizeof(szReplyBuffer), 1000 );
	if( dwRetVal != 0 ) {
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)szReplyBuffer;
		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		sprintf_s(logBuffer, "%s: %ld msec", ip, pEchoReply->RoundTripTime);
		writeLog(logBuffer);
	} else {
		DWORD errCode = GetLastError();
		switch ( errCode ) 
		{
		case 11010: 
			sprintf_s(logBuffer, "%s: Request Timeout", ip);
			break;
		default:
			sprintf_s(logBuffer, "%s: IcmpSendEcho error %ld", ip, errCode);
			break;
		}
		
		writeLog(logBuffer);
	}

	IcmpCloseHandle(hIcmpFile);
	oldIp[0] = 0;
	hIcmpFile = INVALID_HANDLE_VALUE;

	return 0;
}



void CLogPingDlg::OnBnClickedCancel()
{
	closeLog();
	CDialogEx::OnCancel();
}


void CLogPingDlg::OnBnClickedPause()
{
	KillTimer(IDH_TIMER);
	closeLog();

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_IP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}

void CLogPingDlg::OnTimer(UINT_PTR nIDEvent)
{
	ping(getIpAddress());

	CDialogEx::OnTimer(nIDEvent);
}

void CLogPingDlg::closeLog()
{
	if ( logFile ) {
		if( logFile->m_hFile != INVALID_HANDLE_VALUE ) {
			logFile->Close();
		}
		delete logFile;
		logFile = 0;
		logName[0] = 0;
	}
}

