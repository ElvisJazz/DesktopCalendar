// MoreSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "MoreSetDlg.h"
#include "afxdialogex.h"


// CMoreSetDlg 对话框

IMPLEMENT_DYNAMIC(CMoreSetDlg, CDialogEx)

CMoreSetDlg::CMoreSetDlg(int* startXinqi, COLORREF* holidayColor, bool* holiday, bool* displayMore, CWnd* pParent /*=NULL*/)
	: CDialogEx(CMoreSetDlg::IDD, pParent)
{
	m_startXinqi = startXinqi;
	m_holidayColor = holidayColor;
	m_holiday = holiday;
	m_displayMore = displayMore;

	
	m_week0 = m_holiday[0];
	m_week1 = m_holiday[1];
	m_week2 = m_holiday[2];
	m_week3 = m_holiday[3];
	m_week4 = m_holiday[4];
	m_week5 = m_holiday[5];
	m_week6 = m_holiday[6];

	m_displayFestival = m_displayMore[0];
	m_displayAgrCal = m_displayMore[1];
	m_displayJieqi = m_displayMore[2];
}

CMoreSetDlg::~CMoreSetDlg()
{
}

void CMoreSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_startXinqiCtr);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, m_holidayColorCtr);
	DDX_Check(pDX, IDC_CHECK7, m_week0);
	DDX_Check(pDX, IDC_CHECK1, m_week1);
	DDX_Check(pDX, IDC_CHECK2, m_week2);
	DDX_Check(pDX, IDC_CHECK4, m_week3);
	DDX_Check(pDX, IDC_CHECK5, m_week4);
	DDX_Check(pDX, IDC_CHECK6, m_week5);
	DDX_Check(pDX, IDC_CHECK3, m_week6);
	DDX_Check(pDX, IDC_CHECK8, m_displayFestival);
	DDX_Check(pDX, IDC_CHECK9, m_displayAgrCal);
	DDX_Check(pDX, IDC_CHECK10, m_displayJieqi);
}


BEGIN_MESSAGE_MAP(CMoreSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMoreSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMoreSetDlg 消息处理程序


BOOL CMoreSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_startXinqiCtr.SetCurSel(*m_startXinqi-1);
	m_holidayColorCtr.SetColor(*m_holidayColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMoreSetDlg::OnBnClickedOk()
{
	UpdateData(true);
	*m_startXinqi = m_startXinqiCtr.GetCurSel()+1;
	*m_holidayColor = m_holidayColorCtr.GetColor();
	m_holiday[0] = m_week0;
	m_holiday[1] = m_week1;
	m_holiday[2] = m_week2;
	m_holiday[3] = m_week3;
	m_holiday[4] = m_week4;
	m_holiday[5] = m_week5;
	m_holiday[6] = m_week6;

	m_displayMore[0] = m_displayFestival;
	m_displayMore[1] = m_displayAgrCal;
	m_displayMore[2] = m_displayJieqi;

	CDialogEx::OnOK();
}
