// MouseHook.h : MouseHook DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMouseHookApp
// �йش���ʵ�ֵ���Ϣ������� MouseHook.cpp
//

class CMouseHookApp : public CWinApp
{
public:
	CMouseHookApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
