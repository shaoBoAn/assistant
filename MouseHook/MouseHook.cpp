// MouseHook.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MouseHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MYMSG WM_USER + 305 //�Զ�����Ϣ�����ں�������ͨ��
#define WM_MYMSGKEY WM_USER + 306
//���������ڹ����,�������̹߳������±���;
#pragma data_seg(".SHARED")
static HHOOK  hhkMouse=NULL; //��깳�Ӿ��;
static HHOOK  hhkKey=NULL;   //���̾��
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
		if(wParam == WM_LBUTTONDOWN)
		{
			 g_isdrow = true;
			 m_ptOld.x = mouseHookStruct->pt.x;
			 m_ptOld.y = mouseHookStruct->pt.y;
		}
		if(wParam == WM_LBUTTONUP)
		{
			g_isdrow = false;
		}
		if(wParam == WM_MOUSEMOVE)
		{
			if(g_isdrow == true)
			{
				CPoint point;
				point.x = mouseHookStruct->pt.x;
				point.y = mouseHookStruct->pt.y;
				CWindowDC dc(CWnd::FromHandle(GetDesktopWindow()));
				dc.SetROP2(R2_MASKPEN);
				CPen pen(PS_SOLID, 5, RGB(0,255,0)); 
				CPen  *pOldPen=dc.SelectObject(&pen);
				dc.MoveTo(m_ptOld); //�ƶ�����ʼ�� 
				dc.LineTo(point);   //���ߵ���ע�������ÿһ�λ��߶��Ǻ̵ܶġ��̵ܶĶ�λ��߾͵õ�������
				m_ptOld=point;
			}
		}
		//::SendMessage(g_hWnd,WM_MYMSG,wParam,lParam);
		if(wParam == WM_LBUTTONDOWN && g_bDown == true)
		{
			return true;
		}
		if(wParam == WM_MOUSEHOVER && g_bDown == true)
		{
			return true;
		}
		//WM_MOUSELEASE
	}
	return CallNextHookEx(hhkMouse,nCode,wParam,lParam);
}

//��װ�ͼ���깳�ӣ��Ӷ��ػ�ϵͳ���е������Ϣ
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

//ж�صͼ���깳��
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


LRESULT CALLBACK LowLevelKeyProc(
						   int nCode,      // hook code
						   WPARAM wParam,  // message identifier
						   LPARAM lParam   // mouse coordinates
						   )
{
    KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lParam;
	//�������Ϣʱ�����䷢��������
	if (nCode >= HC_ACTION && wParam==WM_KEYDOWN)   
	{   
		BOOL bctrl = GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) *8)-1);  
		if(pkh->vkCode==VK_ESCAPE && bctrl)// && bctrl)
		{
			 if( g_bDown == false)	
			 {
				 g_bDown = true;
			 }
		 	 else
		 	 {
			 	 g_bDown = false;
			 }
			 ::SendMessage(g_hWnd, WM_MYMSGKEY, g_bDown, 0);
			 return true;
		}  
	} 
	return CallNextHookEx(hhkKey,nCode,wParam,lParam);
}

BOOL WINAPI StartHookKey(HWND hWnd)
{
	g_hWnd=hWnd;
	hhkKey=::SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyProc,g_hInstDll,0);
	//hhkMouse=::SetWindowsHookEx(WH_KEYBOARD,LowLevelMouseProc,g_hInstDll,0);
	if (hhkKey==NULL)
	{
		return FALSE;
	} 
	else
	{  
		return TRUE;
	}
}

//ж�صͼ���깳��
VOID WINAPI StopHookKey()
{
	if (hhkKey!=NULL)
	{
		::UnhookWindowsHookEx(hhkKey);
	}
}