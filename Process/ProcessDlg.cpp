
// ProcessDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Process.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"
#include "MemoryCtrl.h"
#include "LogVariable.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
UINT CommunicationMessage(LPVOID lParam);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProcessDlg 대화 상자




CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProcessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bThread = false;
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
}

BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_MESSAGE(WM_ICON_NOTIFY,					OnTrayNotification)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW, &CProcessDlg::OnView)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_HIDE, &CProcessDlg::OnBnClickedBtnHide)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CProcessDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CProcessDlg 메시지 처리기

BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	if (!m_TrayIcon.Create(this, 
				           WM_ICON_NOTIFY, 
						   _T("Barcode Interface Program"), 
					       NULL, 
						   IDR_MENU))
		return -1;
	
	// 아이콘 설정
	// 아이콘 아이디에 자신의 아이콘을 넣으세요...
	m_TrayIcon.SetIcon(IDI_ICON3);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	clsMemCtrl.ConfigOpen();

	if (m_bThread == false)
	{
		m_bThread			= true;
		m_thrHandle = AfxBeginThread(CommunicationMessage, this);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

LRESULT CProcessDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}


void CProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	
	CDialogEx::OnTimer(nIDEvent);
}


UINT CommunicationMessage(LPVOID lParam)
{
	CProcessDlg *pMsg = (CProcessDlg *)lParam;

	int nNum = 0;

	CString strMsg;

	while(pMsg->m_bThread)
	{
		if (clsMemCtrl.m_hMemMapLogW != NULL && clsMemCtrl.m_pQueueLogW != NULL)
		{
			if (clsMemCtrl.m_pQueueLogW->m_nCount > 0)
			{
				strMsg = clsMemCtrl.LogWrite();
				pMsg->OnMessageWirte(strMsg);
			}
		}
	}

	return 0;
}

BOOL CProcessDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_bThread) m_bThread = false;

	clsMemCtrl.ConfigSave();

	return CDialogEx::DestroyWindow();
}


void CProcessDlg::OnView()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ShowWindow(SW_SHOW);
}


void CProcessDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CProcessDlg::OnBnClickedBtnHide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}


void CProcessDlg::OnListMsgClear()
{
	int nIndex = m_listData.GetCount();

	if (nIndex < 100) return;

	m_listData.ResetContent();   // 리스트 박스 컨트롤에 추가된 모든 데이터 삭제 
}

void CProcessDlg::OnMessageWirte(CString strMsg)
{
	int nIndex;	

	OnListMsgClear();

	if (m_strOldMsg == strMsg)
	{
		//2011.0504 추가 같은 메세지가 나오는 문제 처리 
		m_lNormalWaitTime[1] = GetCurrentTime();
		m_lNormalWaitTime[2] = m_lNormalWaitTime[1] - m_lNormalWaitTime[0];
		
		if(m_lNormalWaitTime[2] < 0)
		{
			m_lNormalWaitTime[0] = GetTickCount();
			return;
		}
		if (m_lNormalWaitTime[2] > 10000) 
		{
			//OK 파일로 내용 저장 
		}
	}
	
	if (strMsg != _T(""))
	{
		m_lNormalWaitTime[0]	= GetCurrentTime();
		
		m_strOldMsg				= strMsg;
	}
	else
	{
		return;
	}
	
	nIndex = m_listData.AddString(strMsg, RGB(0, 0, 255));  // 현재 입력된 정보 리스트 박스 컨트롤에 추가 
	m_listData.SetCurSel(nIndex);
}

void CProcessDlg::OnBnClickedBtnExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThread = false;

	CDialog::OnOK();
}
