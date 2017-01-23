
// ProcessDlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
UINT CommunicationMessage(LPVOID lParam);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CProcessDlg ��ȭ ����




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


// CProcessDlg �޽��� ó����

BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	if (!m_TrayIcon.Create(this, 
				           WM_ICON_NOTIFY, 
						   _T("Barcode Interface Program"), 
					       NULL, 
						   IDR_MENU))
		return -1;
	
	// ������ ����
	// ������ ���̵� �ڽ��� �������� ��������...
	m_TrayIcon.SetIcon(IDI_ICON3);

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	clsMemCtrl.ConfigOpen();

	if (m_bThread == false)
	{
		m_bThread			= true;
		m_thrHandle = AfxBeginThread(CommunicationMessage, this);
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (m_bThread) m_bThread = false;

	clsMemCtrl.ConfigSave();

	return CDialogEx::DestroyWindow();
}


void CProcessDlg::OnView()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_SHOW);
}


void CProcessDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CProcessDlg::OnBnClickedBtnHide()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}


void CProcessDlg::OnListMsgClear()
{
	int nIndex = m_listData.GetCount();

	if (nIndex < 100) return;

	m_listData.ResetContent();   // ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ��� ������ ���� 
}

void CProcessDlg::OnMessageWirte(CString strMsg)
{
	int nIndex;	

	OnListMsgClear();

	if (m_strOldMsg == strMsg)
	{
		//2011.0504 �߰� ���� �޼����� ������ ���� ó�� 
		m_lNormalWaitTime[1] = GetCurrentTime();
		m_lNormalWaitTime[2] = m_lNormalWaitTime[1] - m_lNormalWaitTime[0];
		
		if(m_lNormalWaitTime[2] < 0)
		{
			m_lNormalWaitTime[0] = GetTickCount();
			return;
		}
		if (m_lNormalWaitTime[2] > 10000) 
		{
			//OK ���Ϸ� ���� ���� 
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
	
	nIndex = m_listData.AddString(strMsg, RGB(0, 0, 255));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_listData.SetCurSel(nIndex);
}

void CProcessDlg::OnBnClickedBtnExit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bThread = false;

	CDialog::OnOK();
}
