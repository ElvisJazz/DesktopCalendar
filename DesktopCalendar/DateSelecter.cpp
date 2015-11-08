// DateSelecter.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DateSelecter.h"
#include "afxdialogex.h"


// CDateSelecter 对话框

IMPLEMENT_DYNAMIC(CDateSelecter, CDialogEx)

CDateSelecter::CDateSelecter(CalProperty *calPro, int* firstWeek, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDateSelecter::IDD, pParent)
{
	m_calPro = calPro;
	m_firstWeek = firstWeek;
}

CDateSelecter::~CDateSelecter()
{
}

void CDateSelecter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_cal);
}


BEGIN_MESSAGE_MAP(CDateSelecter, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDateSelecter::OnBnClickedOk)
END_MESSAGE_MAP()


// CDateSelecter 消息处理程序


void CDateSelecter::OnBnClickedOk()
{
	CTime selectTime;
	m_cal.GetCurSel(selectTime);
	if(!m_calPro->isSetFirstWeek)
	{
		m_calPro->isCalTime = true;
		m_calPro->time = selectTime;
	}
	else
	{
		m_calPro->isSetFirstWeek = false;
		CTime startTime(selectTime.GetYear(), 1, 1, 0, 0, 0);
		CTimeSpan timeOffset = selectTime - startTime;
		*m_firstWeek = timeOffset.GetDays() / 7 + 1;
	}

	CDialogEx::OnOK();
}