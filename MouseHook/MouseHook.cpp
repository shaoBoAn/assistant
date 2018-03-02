// MouseHook.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MouseHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MYMSG WM_USER + 305 //�Զ�����Ϣ�����ں�������ͨ��
//���������ڹ����,�������̹߳������±���;
#pragma data_seg(".SHARED")
static HHOOK  hhkMouse=NULL; //��깳�Ӿ��;
static HINSTANCE g_hInstDll=NULL; //��dll��ʵ�����;
static HWND g_hWnd=NULL;//����dll�������ھ��
static bool g_bDown = false;
static PMSLLHOOKSTRUCT mouseHookStruct;
static CPoint m_ptOld;
static bool g_isdrow;
#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,rws")




//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CMouseHookApp

BEGIN_MESSAGE_MAP(CMouseHookApp, CWinApp)
END_MESSAGE_MAP()


// CMouseHookApp ����

CMouseHookApp::CMouseHookApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMouseHookApp ����

CMouseHookApp theApp;


// CMouseHookApp ��ʼ��

BOOL CMouseHookApp::InitInstance()
{
	CWinApp::InitInstance();

	//��ȡ����dll���
	g_hInstDll=::AfxGetInstanceHandle();

	return TRUE;
}

//�ͼ���깳�ӹ���
LRESULT CALLBACK LowLevelMouseProc(
						   int nCode,      // hook code
						   WPARAM wParam,  // message identifier
						   LPARAM lParam   // mouse coordinates
						   )
{

	//�������Ϣʱ�����䷢��������
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

/*��װ�ͼ���깳�ӣ��Ӷ��ػ�ϵͳ���е������Ϣ*/
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

/**ж�صͼ���깳��**/
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