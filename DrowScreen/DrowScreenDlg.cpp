
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
	ON_MESSAGE(WM_MYMSG,OnMyMouseMsg)
	ON_BN_CLICKED(IDC_START_STOP, &CDrowScreenDlg::OnBnClickedStartStop)
	ON_BN_CLICKED(IDC_LOADDLL, &CDrowScreenDlg::OnBnClickedLoaddll)
	ON_BN_CLICKED(IDOK, &CDrowScreenDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLICK, &CDrowScreenDlg::OnBnClickedClick)
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
		if(StartMouseHook() == false)
			return;
		m_bButtonStat = true;
		GetDlgItem(IDC_START_STOP)->SetWindowText(L"�ر���깳��");
	}
	else
	{
		if(StopMouseHook() == false)
			return;
		m_bButtonStat = false;
		GetDlgItem(IDC_START_STOP)->SetWindowText(L"������깳��");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

LRESULT CDrowScreenDlg::OnMyMouseMsg(WPARAM wParam,LPARAM lParam)
{
	
	CPoint mpoint;
	mpoint.x = LOWORD(lParam);
	mpoint.y = HIWORD(lParam);

	CString x;
	CString y;
	
	CEdit *editx = (CEdit *)GetDlgItem(IDC_MRX);
	CEdit *edity = (CEdit *)GetDlgItem(IDC_MRY);
	
	x.Format(L"%d",mpoint.x);
	y.Format(L"%d",mpoint.y);

	editx->SetWindowTextW(x);
	edity->SetWindowTextW(y);

	return 1;
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
		return true;
	}
	else
	{
		return false;
	}
}


bool CDrowScreenDlg::StopMouseHook()
{
	typedef VOID (CALLBACK *StopHookMouse)(); 
	StopHookMouse StopHook;
	StopHook=(StopHookMouse)::GetProcAddress(g_hInstDll,"StopHookMouse");
	if(StopHook==NULL)
	{
		return false;
	}
	else
	{
		StopHook();
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
	StopMouseHook();
	DelDLL();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CDrowScreenDlg::OnBnClickedClick()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CPoint pt;

	GetCursorPos(&pt);//��ȡ�������Ļ�ĵ�ǰλ��

	CString x1;
	CString y1;

	CEdit *editx1 = (CEdit *)GetDlgItem(IDC_LDX);
	CEdit *edity1 = (CEdit *)GetDlgItem(IDC_LDY);
	
	editx1->GetWindowTextW(x1);
	edity1->GetWindowTextW(y1);


	SetCursorPos(_ttoi(x1),_ttoi(y1));//�ƶ���ĳ������

	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);//�������
}
