
// DrowScreenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DrowScreen.h"
#include "DrowScreenDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrowScreenDlg 对话框




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


// CDrowScreenDlg 消息处理程序

BOOL CDrowScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_START_STOP)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDrowScreenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		GetDlgItem(IDC_START_STOP)->SetWindowText(L"关闭键盘钩子");
	}
	else
	{
		if(StopKeyHook() == false)
			return;
		StopMouseHook();
		m_bButtonStat = false;
		GetDlgItem(IDC_START_STOP)->SetWindowText(L"启动键盘钩子");
	}
	// TODO: 在此添加控件通知处理程序代码
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
	    GetDlgItem(IDC_TISHI)->SetWindowText(L"启动键盘钩子成功");
	}
	else
	{
		GetDlgItem(IDC_TISHI)->SetWindowText(L"启动键盘钩子失败");
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
		GetDlgItem(IDC_TISHI)->SetWindowText(L"启动鼠标钩子成功");
		return true;
	}
	else
	{
		GetDlgItem(IDC_TISHI)->SetWindowText(L"关闭鼠标钩子成功");
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
		GetDlgItem(IDC_TISHI)->SetWindowText(L"停止键盘钩子失败");
		return false;
	}
	else
	{
		StopHook();
		GetDlgItem(IDC_TISHI)->SetWindowText(L"停止键盘钩子成功");
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
		GetDlgItem(IDC_TISHI)->SetWindowText(L"停止鼠标钩子成功");
		return false;
	}
	else
	{
		StopHook();
		GetDlgItem(IDC_TISHI)->SetWindowText(L"停止鼠标钩子成功");
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
		GetDlgItem(IDC_LOADDLL)->SetWindowText(L"卸载动态库");
		GetDlgItem(IDC_START_STOP)->EnableWindow(TRUE);
	}
	else
	{
		DelDLL();
		m_bLoadDll = false;
		GetDlgItem(IDC_LOADDLL)->SetWindowText(L"加载动态库");
		GetDlgItem(IDC_START_STOP)->EnableWindow(FALSE);
	}
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
