
// DesktopCalendar.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDesktopCalendarApp:
// �йش����ʵ�֣������ DesktopCalendar.cpp
//

class CDesktopCalendarApp : public CWinApp
{
public:
	CDesktopCalendarApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDesktopCalendarApp theApp;