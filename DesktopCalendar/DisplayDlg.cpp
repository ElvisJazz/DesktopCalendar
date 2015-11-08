// DisplayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DateSelecter.h"
#include "MoreSetDlg.h"
#include "DisplayDlg.h"
#include "PropertyOp.h"
#include "afxdialogex.h"

#include <stdio.h>
#include <windows.h>
#include <shlobj.h>

#pragma comment(lib, "shell32.lib")

int g_rows[18] = {1,2,3,4,5,6,7,8,9,10,12,14,16,20,25,30,35,40};
// CDisplayDlg 对话框

IMPLEMENT_DYNAMIC(CDisplayDlg, CDialog)

CDisplayDlg::CDisplayDlg(CalProperty *cal, CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayDlg::IDD, pParent)
	, m_isAutoRun(FALSE)
{
	m_cal = cal;
	m_isSetFont = false;
	m_font = NULL;
	Init();
}

CDisplayDlg::~CDisplayDlg()
{
}

void CDisplayDlg::Init()
{
	m_isDisplayWeek = m_cal->isDisplayWeeks;
	m_firstWeek = m_cal->firstWeek;
	m_displayMode = m_cal->displayMode;

	m_maxRows = m_cal->row;
	m_currentGoneRows = m_cal->currentGoneRows;

	m_commonColor = m_cal->commonColor;
	m_currentColor = m_cal->currentColor;
	m_transparentLever = m_cal->transparentLevel;
	m_grap = m_cal->grap;

	m_startXinqi = m_cal->startXinqi;
	m_holidayColor = m_cal->holidayColor;

	for(int i=0; i<7; i++)
		m_isHoliday[i] = m_cal->isHolidayColor[i];

	for(int j=0; j<3; j++)
		m_displayMore[j] = m_cal->isDisplayMore[j];

	m_isAutoRun = m_cal->isAutoRun;
}

void CDisplayDlg::SetFrame()
{
	
	int j = 0;
	for(; j<18; j++)
	{
		if(g_rows[j] == m_maxRows)
			break;
	}
	m_displayModeCtr.SetCurSel(m_displayMode-1);
	if(m_displayMode != 3)
		m_maxRowCtr.SetCurSel(j);
	else
		m_maxRowCtr.EnableWindow(false);

	m_transparentLeverCtr.SetCurSel(10-m_transparentLever/10);
	m_grapCtr.SetCurSel(m_grap-1);
	m_commonColorCtr.SetColor(m_commonColor);
	m_currentColorCtr.SetColor(m_currentColor);

	if(m_displayMode == 1)
	{
		CString s;
		for(int i=0; i<14; i++)
		{
			s.Format(_T("%d"), i);
			if(i < m_maxRows-1)
				m_hasGoneRowCtr.AddString(s);
			else
				break;
		}
		m_hasGoneRowCtr.SetCurSel(m_currentGoneRows);
	}
	else
		m_hasGoneRowCtr.EnableWindow(false);
}

void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_isDisplayWeek);
	DDX_Control(pDX, IDC_COMBO2, m_displayModeCtr);
	DDX_Control(pDX, IDC_COMBO4, m_maxRowCtr);
	DDX_Control(pDX, IDC_COMBO5, m_hasGoneRowCtr);
	DDX_Control(pDX, IDC_COMBO6, m_transparentLeverCtr);
	DDX_Control(pDX, IDC_COMBO7, m_grapCtr);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_commonColorCtr);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, m_currentColorCtr);
	DDX_Check(pDX, IDC_CHECK2, m_isAutoRun);
}


BEGIN_MESSAGE_MAP(CDisplayDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &CDisplayDlg::OnSetFirstWeek)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CDisplayDlg::OnSetMore)
	ON_BN_CLICKED(IDC_BUTTON3, &CDisplayDlg::OnReturnDefaultSet)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CDisplayDlg::OnSetCommonFont)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDisplayDlg::OnSetMaxRows)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDisplayDlg::OnSetDisplayMode)
	ON_BN_CLICKED(IDC_CHECK2, &CDisplayDlg::OnSetAutoRun)
END_MESSAGE_MAP()


// CDisplayDlg 消息处理程序


BOOL CDisplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT| WS_EX_LAYERED); 
	SetLayeredWindowAttributes(RGB(255,255,255), 255, 3);

	// 设置面板 
	SetFrame();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CDisplayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDisplayDlg::OnSetFirstWeek()
{
	int firstWeek = -1;
	m_cal->isSetFirstWeek = true;
	CDateSelecter dateDlg(m_cal, &firstWeek);
	dateDlg.DoModal();

	if(firstWeek != -1)
		m_firstWeek = firstWeek;
}


void CDisplayDlg::OnSetMore(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMoreSetDlg dlg(&m_startXinqi, &m_holidayColor, m_isHoliday, m_displayMore);
	dlg.DoModal();
	*pResult = 0;
}


void CDisplayDlg::OnReturnDefaultSet()
{
	CPropertyOp::ReadProperty(m_cal, true);
	Init();
	SetFrame();
}


void CDisplayDlg::OnSetCommonFont()
{
	CFontDialog dlg;
	if(IDOK == dlg.DoModal())
	{
		if(m_font)
			delete m_font;
   		m_font = new CFont();
		
		m_isSetFont = true;
		LOGFONT lf;
        memcpy(&lf,dlg.m_cf.lpLogFont,sizeof(LOGFONT));
        m_font->CreateFontIndirect(&lf);

		m_color = dlg.GetColor();
	}
}

void CDisplayDlg::SetProperty()
{
	UpdateData(true);
	m_cal->isDisplayWeeks = m_isDisplayWeek;
	m_cal->firstWeek = m_firstWeek;
	m_cal->displayMode = m_displayModeCtr.GetCurSel()+1;

	m_cal->row = g_rows[m_maxRowCtr.GetCurSel()];
	m_cal->currentGoneRows = m_hasGoneRowCtr.GetCurSel();

	m_cal->commonColor = m_commonColorCtr.GetColor();
	m_cal->currentColor = m_currentColorCtr.GetColor();
	m_cal->transparentLevel = 100-m_transparentLeverCtr.GetCurSel()*10;
	m_cal->grap = m_grapCtr.GetCurSel()+1;

	if(m_isSetFont)
	{
		if(m_cal->font)
			delete m_cal->font;
		m_cal->font = new CFont();

		m_cal->isSetFont = true;
		LOGFONT logFont;
		m_font->GetLogFont(&logFont);
		m_cal->font->CreateFontIndirect(&logFont);
		m_isSetFont = false;
		m_cal->fontColor = m_color;
	}

	// 开机启动设置
	 m_cal->isAutoRun = m_isAutoRun;
	 

	// 更多设置选项
	m_cal->startXinqi = m_startXinqi;
	m_cal->holidayColor = m_holidayColor;
	for(int i=0; i<7; i++)
		m_cal->isHolidayColor[i] = m_isHoliday[i];

	for(int j=0; j<3; j++)
		m_cal->isDisplayMore[j] = m_displayMore[j];

}

void CDisplayDlg::OnSetMaxRows()
{
	int maxRows = m_maxRowCtr.GetCurSel();

	m_hasGoneRowCtr.ResetContent();
	CString s;
	int i = 0;
	for(; i<14; i++)
	{
		s.Format(_T("%d"), i);
		if(i < maxRows-1)
			m_hasGoneRowCtr.AddString(s);
		else
			break;
	}
	if(m_currentGoneRows < i)
		m_hasGoneRowCtr.SetCurSel(m_currentGoneRows);
	else
		m_hasGoneRowCtr.SetCurSel(maxRows / 2);
}


void CDisplayDlg::OnSetDisplayMode()
{
	if(m_displayModeCtr.GetCurSel() >= 1)
		m_hasGoneRowCtr.EnableWindow(false);
	else
		m_hasGoneRowCtr.EnableWindow(true);

	if(m_displayModeCtr.GetCurSel() == 2)
		m_maxRowCtr.EnableWindow(false);
	else
		m_maxRowCtr.EnableWindow(true);

	if(m_displayModeCtr.GetCurSel() == 0)
	{
		CString s;
		for(int i=0; i<14; i++)
		{
			s.Format(_T("%d"), i);
			if(i < m_maxRows-1)
				m_hasGoneRowCtr.AddString(s);
			else
				break;
		}
		m_hasGoneRowCtr.SetCurSel(1);
	}
	m_maxRowCtr.SetCurSel(4);
}

bool CDisplayDlg::SetAutoRun()
{
	if (m_isAutoRun)
    {
		//开机自动启动
		TCHAR m_fileName[MAX_PATH];
		CString pszPath;
		GetModuleFileName(NULL,m_fileName,MAX_PATH);
		CString MySubKey=_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
		HKEY MyKey;
		CString MyValue(m_fileName);
		CString MyName=_T("DesktopCalendar");
		::RegCreateKey(HKEY_CURRENT_USER,MySubKey,&MyKey);
		long MyRet=RegSetValueEx(MyKey,MyName,NULL,REG_SZ,(BYTE*)MyValue.GetBuffer(),MAX_PATH);
		if(MyRet==0)
		{
			MessageBox(_T("已经新增在开机时自动运行，注销或重新启动计算机后即可生效！\n若无效，请在启动目录中添加快捷方式！"),_T("信息提示"),0);
		}
		::RegCloseKey(MyKey);		
		return true;
        
    }
    else
    {
		CString MySubKey=_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
		HKEY MyKey;
		CString MyName=_T("DesktopCalendar");
		::RegCreateKey(HKEY_CURRENT_USER,MySubKey,&MyKey);
		long MyRet=::RegDeleteValue(MyKey,MyName);
		if(MyRet==0)
		{
			MessageBox(_T("已经删除在开机时自动运行，注销或重新启动计算机后即可生效！\n若无效，请在启动目录中删除快捷方式！"),_T("信息提示"),0);
		}
		::RegCloseKey(MyKey);		
		return true;

    }
}

void CDisplayDlg::OnSetAutoRun()
{
	UpdateData(true);
	SetAutoRun();
}
