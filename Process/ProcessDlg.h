
// ProcessDlg.h : ��� ����
//

#pragma once

#include "TrayIcon.h"
#include "ColorListBox.h"		// Į�� ����Ʈ �ڽ� Ŭ����

// CProcessDlg ��ȭ ����
class CProcessDlg : public CDialogEx
{
public:
	CColorListBox		m_listData;

	CString				m_strOldMsg;

	long				m_lNormalWaitTime[3];

	bool				m_bThread;

	HANDLE				m_thrHandle;

	CTrayIcon			m_TrayIcon; 
	void				OnListMsgClear();
	void				OnMessageWirte(CString strMsg);
// �����Դϴ�.
public:
	CProcessDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PROCESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LONG OnTrayNotification(UINT wParam, LONG lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnView();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnHide();
	afx_msg void OnBnClickedBtnExit();
};
