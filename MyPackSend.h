// MyPackSend.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyPackSendApp:
// �йش����ʵ�֣������ MyPackSend.cpp
//

class CMyPackSendApp : public CWinApp
{
public:
	CMyPackSendApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyPackSendApp theApp;