// DisplayExDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DisplayExDlg.h"
#include "afxdialogex.h"


// CDisplayExDlg 对话框

IMPLEMENT_DYNAMIC(CDisplayExDlg, CDialogEx)

CDisplayExDlg::CDisplayExDlg(CalProperty *cal, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplayExDlg::IDD, pParent)
	, m_subName(_T(""))
	, m_subAddress(_T(""))
{
	m_cal = cal;
	m_mode = 0;
	m_index = 0;
	m_isAdd = false;
	m_numOfSub = m_cal->numOfSubscription;
	m_content = _T("");

	for(int i=0; i<m_numOfSub; i++)
		m_isValid.push_back(m_cal->isValid[i]);

	ReadExtractionINI();
}

CDisplayExDlg::~CDisplayExDlg()
{
}

void CDisplayExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_subCtr);
	DDX_Text(pDX, IDC_NAME_EDIT, m_subName);
	DDX_Text(pDX, IDC_ADDRESS_EDIT, m_subAddress);
}


BEGIN_MESSAGE_MAP(CDisplayExDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CDisplayExDlg::OnDeleteSub)
	ON_BN_CLICKED(IDC_BUTTON8, &CDisplayExDlg::OnAddSub)
	ON_BN_CLICKED(IDC_BUTTON9, &CDisplayExDlg::OnIntroduction)
	ON_BN_CLICKED(IDC_BUTTON10, &CDisplayExDlg::OnModify)
	ON_BN_CLICKED(IDC_BUTTON11, &CDisplayExDlg::OnSubmit)
	ON_BN_CLICKED(IDC_BUTTON12, &CDisplayExDlg::OnCancel)
	ON_BN_CLICKED(IDC_BUTTON13, &CDisplayExDlg::OnAddTemplate)
END_MESSAGE_MAP()


// CDisplayExDlg 消息处理程序


BOOL CDisplayExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT| WS_EX_LAYERED); 
	SetLayeredWindowAttributes(RGB(255,255,255), 255, 3);

	GetDlgItem(IDC_CONFIG_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON11)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON12)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON13)->ShowWindow(false);
	GetDlgItem(IDC_NAME_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_NAME_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_ADDRESS_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_ADDRESS_STATIC)->ShowWindow(false);

	for(int i=0; i<m_numOfSub; i++)
	{
		if(m_cal->isValid[i])
			m_subCtr.AddString(m_cal->webpageName[i]);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CDisplayExDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CDisplayExDlg::ReadExtractionINI()
{
	CString s = _T("");
	for(int i=0; i<m_cal->numOfSubscription; i++)
	{
		s.Format(_T("%d"), i+1);
		CFile file(_T("..//INI//extraction")+s+_T(".ini"), CFile::modeRead);
		char *pBuf;
		int iLen=file.GetLength();
		pBuf=new char[iLen+1];
		file.Read(pBuf,iLen);
		pBuf[iLen]=0;
		CString str(pBuf); 
		m_cal->config.Add(str);
		delete[] pBuf;
		file.Close();
	}
}


void CDisplayExDlg::WriteExtractionINI(CString fileName, CString content)
{
	CString str = _T("");
	int num = 0;

	int index= content.Find(_T("\nnum"));
	content.Delete(0, index+5);
	index = content.Find(_T("\n"));
	str = content.Left(index);
	WritePrivateProfileString(_T("numOfFeature"), _T("num"), str, fileName);
	num = _ttoi(str);

	for(int i=0; i<num; i++)
	{
		CString s = _T("");
		s.Format(_T("%d"), i+1);

		index = content.Find(_T("title"));
		content.Delete(0, index+6);
		index = content.Find(_T("\n"));
		str = content.Left(index);
		WritePrivateProfileString(s, _T("title"), str, fileName);

		index = content.Find(_T("isGroup"));
		content.Delete(0, index+8);
		index = content.Find(_T("\n"));
		str = content.Left(index);
		WritePrivateProfileString(s, _T("isGroup"), str, fileName);

		index = content.Find(_T("feature"));
		content.Delete(0, index+8);
		index = content.Find(_T("\n"));
		str = content.Left(index);
		WritePrivateProfileString(s, _T("feature"), str, fileName);

		index = content.Find(_T("field"));
		content.Delete(0, index+6);
		index = content.Find(_T("\n"));
		str = content.Left(index);
		WritePrivateProfileString(s, _T("field"), str, fileName);
	
		index = content.Find(_T("pos"));
		content.Delete(0, index+4);
		index = content.Find(_T("\n"));
		str = content.Left(index);
		WritePrivateProfileString(s, _T("pos"), str, fileName);

		index = content.Find(_T("startToken"));
		content.Delete(0, index+11);
		index = content.Find(_T("\n"));
		str = content.Left(index);
		WritePrivateProfileString(s, _T("startToken"), str, fileName);

		index = content.Find(_T("endToken"));
		content.Delete(0, index+9);
		index = content.Find(_T("\n"));
		str = content.Left(index);
		WritePrivateProfileString(s, _T("endToken"), str, fileName);

		index = content.Find(_T("endFeature"));
		content.Delete(0, index+11);
		index = content.Find(_T("\n"));
		str = content.Left(index);
		WritePrivateProfileString(s, _T("endFeature"), str, fileName);
	}
}

void CDisplayExDlg::OnDeleteSub()
{
	int selectItem = m_subCtr.GetCurSel();
	CString str = _T("");

	if(m_isValid[selectItem])
	{
		m_subCtr.GetLBText(selectItem, str);
		m_subCtr.DeleteString(selectItem);
		m_subCtr.InsertString(selectItem,_T("[删]")+str);
		m_mode = 2;
		m_isValid[selectItem] = false;
	}
	else
	{
		m_subCtr.GetLBText(selectItem, str);
		m_subCtr.DeleteString(selectItem);
		m_subCtr.InsertString(selectItem, m_cal->webpageName[selectItem]);
		m_mode = 0;
		m_isValid[selectItem] = true;
	}
	UpdateData(false);
}


void CDisplayExDlg::OnAddSub()
{
	GetDlgItem(IDC_CONFIG_EDIT)->ShowWindow(true);
	GetDlgItem(IDC_CONFIG_EDIT)->SetWindowText(_T("[numOfFeature]\r\nnum=\r\n\r\n"));
	GetDlgItem(IDC_SUB_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON8)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON10)->ShowWindow(false);
	m_subCtr.ShowWindow(false);
	GetDlgItem(IDC_BUTTON11)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON12)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON13)->ShowWindow(true);
	GetDlgItem(IDC_NAME_EDIT)->ShowWindow(true);
	GetDlgItem(IDC_NAME_STATIC)->ShowWindow(true);
	GetDlgItem(IDC_ADDRESS_EDIT)->ShowWindow(true);
	GetDlgItem(IDC_ADDRESS_STATIC)->ShowWindow(true);
	m_mode = 1;
	m_isValid.push_back(true);
	m_index = 0;
}


void CDisplayExDlg::OnIntroduction()
{
	ShellExecute(NULL,_T("open"),_T("NOTEPAD.EXE"),_T("..//INI//Introduction.txt"),NULL,SW_SHOWNORMAL);
}


void CDisplayExDlg::OnModify()
{
	int index = m_subCtr.GetCurSel();

	GetDlgItem(IDC_CONFIG_EDIT)->ShowWindow(true);
	GetDlgItem(IDC_SUB_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON8)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON10)->ShowWindow(false);
	m_subCtr.ShowWindow(false);
	GetDlgItem(IDC_BUTTON11)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON12)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON13)->ShowWindow(true);
	GetDlgItem(IDC_NAME_EDIT)->ShowWindow(true);
	GetDlgItem(IDC_NAME_STATIC)->ShowWindow(true);
	GetDlgItem(IDC_ADDRESS_EDIT)->ShowWindow(true);
	GetDlgItem(IDC_ADDRESS_STATIC)->ShowWindow(true);

	GetDlgItem(IDC_NAME_EDIT)->SetWindowTextW(m_cal->webpageName[index]);
	GetDlgItem(IDC_ADDRESS_EDIT)->SetWindowTextW(m_cal->url[index]);
	GetDlgItem(IDC_CONFIG_EDIT)->SetWindowTextW(m_cal->config[index]);

	m_mode = 3;
	m_index = 0;
}


void CDisplayExDlg::OnSubmit()
{	
	UpdateData(true);
	m_isAdd = true;
	int index = m_subCtr.GetCurSel();
	GetDlgItem(IDC_CONFIG_EDIT)->GetWindowTextW(m_content);

	if(m_mode == 1)
	{
		(m_cal->webpageName).Add(m_subName);
		(m_cal->url).Add(m_subAddress);
		(m_cal->config).Add(m_content);
		 m_subCtr.AddString(m_subName);
	}
	else if(m_mode == 3)
	{
		(m_cal->webpageName).SetAt(index, m_subName);
		(m_cal->url).SetAt(index, m_subAddress);
		(m_cal->config).SetAt(index, m_content);
		m_subCtr.DeleteString(index);
		m_subCtr.InsertString(index, m_subName);
	}
	MessageBox(_T("已提交，保存设置后即可生效！"));

	GetDlgItem(IDC_CONFIG_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_SUB_STATIC)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON8)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON10)->ShowWindow(true);
	m_subCtr.ShowWindow(true);
	GetDlgItem(IDC_BUTTON11)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON12)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON13)->ShowWindow(false);
	GetDlgItem(IDC_NAME_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_NAME_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_ADDRESS_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_ADDRESS_STATIC)->ShowWindow(false);
	
	GetDlgItem(IDC_CONFIG_EDIT)->SetWindowText(_T(""));
	GetDlgItem(IDC_NAME_EDIT)->SetWindowText(_T(""));
	GetDlgItem(IDC_ADDRESS_EDIT)->SetWindowText(_T(""));

	m_mode = 0;
}


void CDisplayExDlg::OnCancel()
{
	GetDlgItem(IDC_CONFIG_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_SUB_STATIC)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON8)->ShowWindow(true);
	GetDlgItem(IDC_BUTTON10)->ShowWindow(true);
	m_subCtr.ShowWindow(true);
	GetDlgItem(IDC_BUTTON11)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON12)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON13)->ShowWindow(false);
	GetDlgItem(IDC_NAME_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_NAME_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_ADDRESS_EDIT)->ShowWindow(false);
	GetDlgItem(IDC_ADDRESS_STATIC)->ShowWindow(false);
	GetDlgItem(IDC_CONFIG_EDIT)->SetWindowText(_T(""));
	GetDlgItem(IDC_NAME_EDIT)->SetWindowText(_T(""));
	GetDlgItem(IDC_ADDRESS_EDIT)->SetWindowText(_T(""));

	m_mode = 0;
}


void CDisplayExDlg::OnAddTemplate()
{
	m_index++;
	CString s, tmpStr;
	s.Format(_T("[%d]\r\ntitle=\r\nisGroup=\r\nfeature=\r\nfield=\r\npos=\r\nstartToken=\r\nendToken=\r\nendFeature=\r\n\r\n"), m_index);
	GetDlgItem(IDC_CONFIG_EDIT)->GetWindowText(tmpStr);
	GetDlgItem(IDC_CONFIG_EDIT)->SetWindowText(tmpStr+s);
}

void CDisplayExDlg::SetProperty()
{
	m_cal->isValid.clear();
	int i = 0, j = 0;
	CString fileName=_T(""), s = _T("");

	if(m_isAdd)
	{	
		m_subCtr.ResetContent();
		for(vector<bool>::iterator iter=m_isValid.begin(); iter!=m_isValid.end(); ++iter,i++)
		{
			if(*iter)
			{
				m_cal->isValid.push_back(true);
				m_cal->webpageName.SetAt(j, m_cal->webpageName.ElementAt(i)); 
				m_cal->url.SetAt(j, m_cal->url.ElementAt(i));
				m_cal->config.SetAt(j, m_cal->config.ElementAt(i));


				fileName.Format(_T("..//INI//extraction%d.ini"), j+1);
				s = m_cal->config.ElementAt(j);
				WriteExtractionINI(fileName, s);
				m_subCtr.AddString(m_cal->webpageName.ElementAt(j));
				j++;
			}
		}
		m_cal->numOfSubscription = j;
	}
	m_isAdd = false;
}

