
// DrowScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrowScreen.h"
#include "DrowScreenDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrowScreenDlg �Ի���




CDrowScreenDlg::CDrowScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDrowScreenDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bButtonStat = false;
	m_bLoadDll = false;
	g_hInstDll = NULL;
}

void CDrowScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrowScreenDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MYMSGKEY,OnMyKeyMsg)
	ON_BN_CLICKED(IDC_START_STOP, &CDrowScreenDlg::OnBnClickedStartStop)
	ON_BN_CLICKED(IDC_LOADDLL, &CDrowScreenDlg::OnBnClickedLoaddll)
	ON_BN_CLICKED(IDOK, &CDrowScreenDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDrowScreenDlg ��Ϣ�������

BOOL CDrowScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_START_STOP)->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDrowScreenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDrowScreenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDrowScreenDlg::OnBnClickedStartStop()
{
	if(m_bButtonStat == false)
	{
		if(StartKeyHook() == false)
			return;
		m_bButtonStat = true;
		GetDlgItem(IDC_START_STOP)->SetWindowText(L"�رռ��̹���");
	}
	else
	{
		if(StopKeyHook() == false)
			return;
		StopMouseHook();
		m_bButtonStat = false;
		GetDlgItem(IDC_START_STOP)->SetWindowText(L"�������̹���");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

LRESULT CDrowScreenDlg::OnMyKeyMsg(WPARAM wParam,LPARAM lParam)
{
	bool lb =(bool)wParam;
	if(lb == true)
	{
		StartMouseHook();
	}
	else
	{
		StopMouseHook();
	}
	return 1;
}

bool CDrowScreenDlg::StartKeyHook()
{
	typedef BOOL (CALLBACK *StartHookKey)(HWND hWnd); 
	StartHookKey StartHookK;
	StartHookK=(StartHookKey)::GetProcAddress(g_hInstDll, "StartHookKey");
	if(StartHookK == NULL)
	{
		AfxMessageBox(_T("func StartHookKey not found!"));
		return false;
	}
	if (StartHookK(this->m_hWnd))
	{
	    GetDlgItem(IDC_TISHI)->SetWindowText(L"�������̹��ӳɹ�");
	}
	else
	{
		GetDlgItem(IDC_TISHI)->SetWindowText(L"�������̹���ʧ��");
	}
	return true;
}

bool CDrowScreenDlg::StartMouseHook()
{
	typedef BOOL (CALLBACK *StartHookMouse)(HWND hWnd); 
	StartHookMouse StartHook;

	StartHook=(StartHookMouse)::GetProcAddress(g_hInstDll,"StartHookMouse");
	if(StartHook==NULL)
	{
		AfxMessageBox(_T("func StartHookMouse not found!"));
		return false;
	}
	if (StartHook(this->m_hWnd))
	{
		GetDlgItem(IDC_TISHI)->SetWindowText(L"������깳�ӳɹ�");
		return true;
	}
	else
	{
		GetDlgItem(IDC_TISHI)->SetWindowText(L"�ر���깳�ӳɹ�");
		return false;
	}
}

bool CDrowScreenDlg::StopKeyHook()
{
	typedef VOID (CALLBACK *StopHookKey)(); 
	StopHookKey StopHook;

	//g_hInstDll=LoadLibrary(_T("MouseHook.dll"));
	StopHook=(StopHookKey)::GetProcAddress(g_hInstDll,"StopHookKey");
	if(StopHook == NULL)
	{
		GetDlgItem(IDC_TISHI)->SetWindowText(L"ֹͣ���̹���ʧ��");
		return false;
	}
	else
	{
		StopHook();
		GetDlgItem(IDC_TISHI)->SetWindowText(L"ֹͣ���̹��ӳɹ�");
		return true;
	}
}

bool CDrowScreenDlg::StopMouseHook()
{
	typedef VOID (CALLBACK *StopHookMouse)(); 
	StopHookMouse StopHook;
	StopHook=(StopHookMouse)::GetProcAddress(g_hInstDll,"StopHookMouse");
	if(StopHook==NULL)
	{
		GetDlgItem(IDC_TISHI)->SetWindowText(L"ֹͣ��깳�ӳɹ�");
		return false;
	}
	else
	{
		StopHook();
		GetDlgItem(IDC_TISHI)->SetWindowText(L"ֹͣ��깳�ӳɹ�");
		return true;
	}

}

void CDrowScreenDlg::OnBnClickedLoaddll()
{
	if(m_bLoadDll == false)
	{
		if(LoadDll() == false)
			return;
		m_bLoadDll = true;
		GetDlgItem(IDC_LOADDLL)->SetWindowText(L"ж�ض�̬��");
		GetDlgItem(IDC_START_STOP)->EnableWindow(TRUE);
	}
	else
	{
		DelDLL();
		m_bLoadDll = false;
		GetDlgItem(IDC_LOADDLL)->SetWindowText(L"���ض�̬��");
		GetDlgItem(IDC_START_STOP)->EnableWindow(FALSE);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
bool CDrowScreenDlg::LoadDll()
{
	g_hInstDll = LoadLibrary(_T("MouseHook.dll"));
	if(g_hInstDll == NULL)
	{
		AfxMessageBox(_T("no MouseHook.dll"));
		return false;
	}
	return true;
}

void CDrowScreenDlg::DelDLL()
{
	if (g_hInstDll != NULL)
	{
		::FreeLibrary(g_hInstDll);
	}
}


void CDrowScreenDlg::OnBnClickedOk()
{
	StopKeyHook();
	DelDLL();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
