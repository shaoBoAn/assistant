// MouseHook.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MouseHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MYMSG WM_USER + 305 //自定义消息，用于和主程序通信
//将变量放在共享段,及所有线程共享以下变量;
#pragma data_seg(".SHARED")
static HHOOK  hhkMouse=NULL; //鼠标钩子句柄;
static HINSTANCE g_hInstDll=NULL; //本dll的实例句柄;
static HWND g_hWnd=NULL;//调用dll的主窗口句柄
static bool g_bDown = false;
static PMSLLHOOKSTRUCT mouseHookStruct;
static CPoint m_ptOld;
static bool g_isdrow;
#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")




//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMouseHookApp

BEGIN_MESSAGE_MAP(CMouseHookApp, CWinApp)
END_MESSAGE_MAP()


// CMouseHookApp 构造

CMouseHookApp::CMouseHookApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMouseHookApp 对象

CMouseHookApp theApp;


// CMouseHookApp 初始化

BOOL CMouseHookApp::InitInstance()
{
	CWinApp::InitInstance();

	//获取自身dll句柄
	g_hInstDll=::AfxGetInstanceHandle();

	return TRUE;
}

//低级鼠标钩子过程
LRESULT CALLBACK LowLevelMouseProc(
						   int nCode,      // hook code
						   WPARAM wParam,  // message identifier
						   LPARAM lParam   // mouse coordinates
						   )
{

	//有鼠标消息时，将其发给主程序
	if (g_hWnd!=NULL&&nCode==HC_ACTION)
	{
		mouseHookStruct=(PMSLLHOOKSTRUCT)lParam;
		if(wParam == WM_MOUSEMOVE)
		{
				CPoint point;
				point.x = mouseHookStruct->pt.x;
				point.y = mouseHookStruct->pt.y;
				::SendMessage(g_hWnd,WM_MYMSG,wParam,MAKELPARAM(point.x,point.y));	
		}
		
		
		//WM_MOUSELEASE
	}
	return CallNextHookEx(hhkMouse,nCode,wParam,lParam);
}

/*安装低级鼠标钩子，从而截获系统所有的鼠标消息*/
BOOL WINAPI StartHookMouse(HWND hWnd)
{
	g_hWnd=hWnd;
	hhkMouse=::SetWindowsHookEx(WH_MOUSE_LL,LowLevelMouseProc,g_hInstDll,0);
	//hhkMouse=::SetWindowsHookEx(WH_KEYBOARD,LowLevelMouseProc,g_hInstDll,0);
	if (hhkMouse==NULL)
	{
		return FALSE;
	} 
	else
	{  
		return TRUE;
	}
}

/**卸载低级鼠标钩子**/
VOID WINAPI StopHookMouse()
{
	if (hhkMouse!=NULL)
	{
		::UnhookWindowsHookEx(hhkMouse);
	}
}

VOID WINAPI SetBDown(bool flag)
{
	 g_bDown = flag;
}