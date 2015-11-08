// SetTaskDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "SetTaskDlg.h"
#include "afxdialogex.h"


// CSetTaskDlg 对话框

IMPLEMENT_DYNAMIC(CSetTaskDlg, CDialogEx)

CSetTaskDlg::CSetTaskDlg(CalProperty *cal, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetTaskDlg::IDD, pParent)
	, m_firstTask(_T(""))
	, m_secondTask(_T(""))
	, m_thirdTask(_T(""))
	, m_fourthTask(_T(""))
{
	m_cal = cal;
}

CSetTaskDlg::~CSetTaskDlg()
{
}

void CSetTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT8, m_firstTask);
	DDX_Text(pDX, IDC_EDIT9, m_secondTask);
	DDX_Text(pDX, IDC_EDIT6, m_thirdTask);
	DDX_Text(pDX, IDC_EDIT1, m_fourthTask);
}


BEGIN_MESSAGE_MAP(CSetTaskDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSetTaskDlg 消息处理程序


HBRUSH CSetTaskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(nCtlColor == CTLCOLOR_DLG) 
	{ 
		pDC-> SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COMMON_TEXT_COLOR);
		HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
		return hBrush; 
	}
	else if(nCtlColor == CTLCOLOR_STATIC) 
	{ 
		pDC-> SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
		return hBrush; 
	}
	else
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CSetTaskDlg::SetProperty()
{
	UpdateData(true);
	m_cal->task[0] = m_firstTask;
	m_cal->task[1] = m_secondTask;
	m_cal->task[2] = m_thirdTask;
	m_cal->task[3] = m_fourthTask;
	UpdateData(false);
}

BOOL CSetTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_firstTask = m_cal->task[0];
	m_secondTask = m_cal->task[1];
	m_thirdTask = m_cal->task[2];
	m_fourthTask = m_cal->task[3];
	UpdateData(false);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
