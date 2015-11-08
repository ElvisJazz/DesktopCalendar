// TextInputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DesktopMostDlg.h"
#include "TextInputDlg.h"
#include "afxdialogex.h"
#include "DesktopDlg.h"

// CTextInputDlg 对话框

IMPLEMENT_DYNAMIC(CTextInputDlg, CDialogEx)

CTextInputDlg::CTextInputDlg(CalProperty* pPro,const CString& str,CWnd* pParent /*=NULL*/)
	: CDialogEx(CTextInputDlg::IDD, pParent)
	, m_text(_T(""))
{
	m_pPro = pPro;
	m_strCaption = str;
	m_close = false;

	map<CString, Info*>::iterator iter1 = m_pPro->infoMap.find(m_pPro->strCode);
	if(iter1 != m_pPro->infoMap.end())
	{
		if(iter1->second->color!= -1)
			m_blockColor = iter1->second->color;
		m_text = iter1->second->text;
	}
	
	if(iter1==m_pPro->infoMap.end() || iter1->second->color==-1)
	{
		if(m_pPro->isHolidayColor[m_pPro->xiqiCode-1])
			m_blockColor = m_pPro->holidayColor;
		else
			m_blockColor = m_pPro->commonColor;
	}
	
	m_origText = m_text;
}

CTextInputDlg::~CTextInputDlg()
{
}

void CTextInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_text);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_colorCtr);
}


BEGIN_MESSAGE_MAP(CTextInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTextInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTextInputDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CTextInputDlg::OnEnChangeEdit)
END_MESSAGE_MAP()


// CTextInputDlg 消息处理程序

BOOL CTextInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		
	m_colorCtr.SetColor(m_blockColor);
	SetWindowText(m_strCaption);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTextInputDlg::OnBnClickedOk()
{
	UpdateData(true);
	COLORREF color = m_colorCtr.GetColor();
	int isFresh = 0;

	map<CString, Info*>::iterator iter = m_pPro->infoMap.find(m_pPro->strCode);
	if(iter != m_pPro->infoMap.end())
	{
		iter->second->text = m_text;
		if(color != m_blockColor)
		{
			iter->second->color = color;
			isFresh = 1;
		}
	}
	else if(m_text!=_T("") || color!=m_blockColor)
	{
		Info *info = new Info();
		if(color != m_blockColor)
		{
			info->color = color;
			isFresh = 1;
		}
		else
			info->color = -1;
		info->text = m_text;
		m_pPro->infoMap.insert(make_pair(m_pPro->strCode, info));
	}

	if(m_text != m_origText)
		isFresh = 2;

	int index = m_pPro->strCode.Find(' ');
	if(index != -1)
		m_text = m_pPro->strCode.Left(index+1)+"\n"+m_text;

	if(isFresh == 2)
		m_pPro->pStatic->SetWindowText(m_text);
	m_pPro->pButton->SetUpColor(color);

	ShowWindow(false);

	if(isFresh ==1)
	{
		m_pPro->pDlg0->ShowWindow(false);
		m_pPro->pDlg0->ShowWindow(true);
	}
	else if(isFresh==2)
	{
		CString sTem = m_pPro->strCode;
		int n = sTem.Find(' ');
		int day = _ttoi(sTem.Left(n));

		sTem.Delete(0, n+1);
		n = sTem.Find(' ');
		int month = _ttoi(sTem.Left(n));
		sTem.Delete(0, n+1);
		int year = _ttoi(sTem);

		m_pPro->pDlg->AddExtralDisplay(m_pPro->pStatic,year, month, day);
	}

	m_close = true;
	UpdateData(false);
	//CDialogEx::OnOK();
}


void CTextInputDlg::OnBnClickedCancel()
{
	m_close = true;
	//CDialogEx::OnCancel();
}

void CTextInputDlg::OnEnChangeEdit()
{
	CString message = _T("");
	CString numOfWordsForInput = _T("");
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(message);
	int num = 250 - message.GetLength();
	if(num <= 0)
		num = 0;
	numOfWordsForInput.Format(_T("%d字"), num);
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(numOfWordsForInput);

	if(num <= 0)
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(message.Left(250));

	UpdateData(true);
}
