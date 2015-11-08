// FlashDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DesktopCalendarDlg.h"
#include "FlashDlg.h"
#include "afxdialogex.h"
#include <ctime>

//static UINT Loop(void* app0)
//{ 
//	Sleep(8000);
//	CFlashDlg* flashDlg = (CFlashDlg*) app0;;
//	flashDlg->~CFlashDlg();
//    return 0;
//}
// CFlashDlg �Ի���

IMPLEMENT_DYNAMIC(CFlashDlg, CDlgBase)

CFlashDlg::CFlashDlg(CWnd* pParent /*=NULL*/)
: CDlgBase(CFlashDlg::IDD,pParent)
{
	//m_pDlg = dlg;
}

CFlashDlg::~CFlashDlg()
{
	//m_pDlg->OpenCal();
}

void CFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_FlashShow);
}

BOOL CFlashDlg::OnInitDialog()
{
	CDlgBase::OnInitDialog();
	//�޸ķ��ʹ����������������ʾ
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	RECT rc;

	this->GetClientRect(&rc);//��ÿͻ���������

	TCHAR strCurDrt[500];
	int nLen = ::GetCurrentDirectory(500,strCurDrt);

	if( strCurDrt[nLen]!='\\' )
	{
		strCurDrt[nLen++] = '\\';
		strCurDrt[nLen] = '\0';
	}

	CString strFileName = strCurDrt;
	srand(time(0));
	int index = rand() % 4 + 1;
	CString indexStr;
	indexStr.Format(_T("%d.swf"), index);
	strFileName += indexStr;//XXX.swf��flash�ļ������֣���flash�ļ����ڹ���Ŀ¼�¡�

	m_FlashShow.LoadMovie(0, strFileName);
	m_FlashShow.Play();
	CenterWindow();
	Open();
	return true;
}

void CFlashDlg::Open()
{
	CRect tRect;
	GetWindowRect(tRect);
	tRect.bottom = tRect.top;
	SetWindowPos(&wndTopMost,tRect);

	ShowWindow(true);

	m_hsFinished = FALSE;
	m_hideMode = HM_ALL;
	m_hiding = FALSE;
	SetTimer(2, HS_ELAPSE, NULL);
	m_closeMode = 1;
}

BEGIN_MESSAGE_MAP(CFlashDlg, CDlgBase)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOVING()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CFlashDlg ��Ϣ�������
