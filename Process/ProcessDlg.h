
// ProcessDlg.h : 헤더 파일
//

#pragma once

#include "TrayIcon.h"
#include "ColorListBox.h"		// 칼라 리스트 박스 클래스

// CProcessDlg 대화 상자
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
// 생성입니다.
public:
	CProcessDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROCESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
