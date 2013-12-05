
// LogPingDlg.h :
//

#pragma once


// CLogPingDlg
class CLogPingDlg : public CDialogEx
{
public:
	CLogPingDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_LOGPING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

private:
	HANDLE hIcmpFile;   /**< ICMP Socket Handler */
	char oldIp[128];    /**< Destination IP Address */
	CFile *logFile;     /** Log File Handler */
	char logName[4096]; /**< Log File Name */

	const char *getLogName() const;
	void WriteLog();
	int ping(const char *ip);
	const char *getIpAddress() const;
	void writeLog(const char *msg);
	void closeLog();

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPause();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


