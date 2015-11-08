// MySubscription.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "MySubscription.h"
#include "afxdialogex.h"
#include "WebpageHandler.h"

// CMySubscription 对话框

IMPLEMENT_DYNAMIC(CMySubscription, CDialogEx)

CMySubscription::CMySubscription(CalProperty* pro, CWnd* pParent /*=NULL*/)
	: CDialogEx(CMySubscription::IDD, pParent)
{
	m_pro = pro;
	GetDisplayExtractionInfo(m_pro->url, m_pro->numOfSubscription);
}

CMySubscription::~CMySubscription()
{
}

void CMySubscription::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_subCtr);
}


BEGIN_MESSAGE_MAP(CMySubscription, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMySubscription::OnRefreshAll)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMySubscription::OnSelectSubscription)
END_MESSAGE_MAP()


// CMySubscription 消息处理程序
void CMySubscription::GetDisplayExtractionInfo(CStringArray &url, int size)
{
	CString s = _T("");
	CString tmpPath = _T("");
	LPTSTR path;

	for(int i=1; i<=size; i++)
	{
		s.Format(_T("%d"), i);
		tmpPath = _T("..//INI//exTmp")+s+_T(".txt");
		m_pageHandler.GetSourceHtml(url[i-1], tmpPath);
		m_pageHandler.GetLocalHtml(tmpPath);

		tmpPath = _T("..//INI//extraction")+s+_T(".ini");
		path = tmpPath.GetBuffer(0);
		tmpPath.ReleaseBuffer();
		m_pageHandler.ReadExtractionIni(path);
		m_pageHandler.ExtractInfo(i);
	}
}

void CMySubscription::DisplayRequiredInfo()
{

}

BOOL CMySubscription::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetClientRect(&m_rect);

	LONG lStyle;
    lStyle = GetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
    lStyle |= LVS_REPORT; //设置style
    SetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style

	for(int i=0; i<m_pro->numOfSubscription; i++)
		m_subCtr.AddString(m_pro->webpageName[i]);
	m_subCtr.SetCurSel(0);

	m_listCtrl.DeleteAllItems();
	m_listCtrl.InsertColumn(0, _T("WebpageName"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(1, _T("Title"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(2, _T("Content"), LVCFMT_LEFT, 400);

	m_listCtrl.InsertItem(0, m_pro->webpageName[0]);
	m_listCtrl.SetItemText(0, 1, _T("访问") ,m_pro->url[0]);

	map<int, CString>::iterator iter = m_pageHandler.m_displayExtractionInfo.find(1);
	CString displayInfo = iter->second;
	int i = 0, index = 0;
	CString str = _T("");
	while(displayInfo != _T("\n"))
	{
		i++;
		m_listCtrl.InsertItem( i, _T(""));

		// 显示title
		displayInfo.Delete(0, 7);
		index = displayInfo.Find(_T("\n"));
		str = displayInfo.Left(index);
		displayInfo.Delete(0, index+1);
		m_listCtrl.SetItemText(i, 1, str);
		while(displayInfo.Left(7) != _T("<title>") && displayInfo != _T("\n"))
		{
			i++;
			m_listCtrl.InsertItem( i, _T(""));
			// 检测是否有链接
			str = displayInfo.Left(6);
			if(str == _T("<href>"))
			{
				CString address = _T("");;
				displayInfo.Delete(0, 6);
				index = displayInfo.Find(_T("\n"));
				address = displayInfo.Left(index);
				if(address.Left(4) != _T("http"))
				{
					CString homeAdd = m_pro->url[0];
					homeAdd.ReleaseBuffer();
					address = homeAdd+address;
				}

				displayInfo.Delete(0, index+1);
				index = displayInfo.Find(_T("\n"));
				str = displayInfo.Left(index);
				displayInfo.Delete(0, index+1);
				m_listCtrl.SetItemText(i, 2, str, address);
			}
			else
			{
				index = displayInfo.Find(_T("\n"));
				str = displayInfo.Left(index);
				displayInfo.Delete(0, index+1);
				m_listCtrl.SetItemText(i, 2, str);
			}
		}
	}
	m_listCtrl.Init();
	m_listCtrl.SetItemData(0, 123);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMySubscription::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CWnd *pWnd = GetDlgItem(IDC_LIST1);    
	GetClientRect(&m_rect);

    if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
    {
    //获取控件变化前的大小 
     CRect rect;     

     pWnd->GetWindowRect(&rect);
	 //将控件大小转换为在对话框中的区域坐标
     ScreenToClient(&rect);  

     //调整控件大小
     rect.left = 15;
     rect.right = m_rect.Width() - 15;
     rect.top = 65;
     rect.bottom = m_rect.Height() - 20;

     //设置控件大小
     pWnd->MoveWindow(rect);
  }

  GetClientRect(&m_rect);
}


void CMySubscription::OnRefreshAll()
{
	GetDisplayExtractionInfo(m_pro->url, m_pro->numOfSubscription);
}


void CMySubscription::OnSelectSubscription()
{
	int row = m_subCtr.GetCurSel();
	m_listCtrl.DeleteAllItems();
	m_listCtrl.InsertItem(0, m_pro->webpageName[row]);
	m_listCtrl.SetItemText(0, 1, _T("访问") ,m_pro->url[row]);

	map<int, CString>::iterator iter = m_pageHandler.m_displayExtractionInfo.find(row+1);
	CString displayInfo = iter->second;
	int i = 0, index = 0;
	CString str = _T("");
	while(displayInfo != _T("\n"))
	{
		i++;
		m_listCtrl.InsertItem( i, _T(""));

		// 显示title
		displayInfo.Delete(0, 7);
		index = displayInfo.Find(_T("\n"));
		str = displayInfo.Left(index);
		displayInfo.Delete(0, index+1);
		m_listCtrl.SetItemText(i, 1, str);
		while(displayInfo.Left(7) != _T("<title>") && displayInfo != _T("\n"))
		{
			i++;
			m_listCtrl.InsertItem( i, _T(""));
			// 检测是否有链接
			str = displayInfo.Left(6);
			if(str == _T("<href>"))
			{
				CString address = _T("");;
				displayInfo.Delete(0, 6);
				index = displayInfo.Find(_T("\n"));
				address = displayInfo.Left(index);
				if(address.Left(4) != _T("http"))
				{
					CString homeAdd = m_pro->url[row];
					homeAdd.ReleaseBuffer();
					address = homeAdd+address;
				}

				displayInfo.Delete(0, index+1);
				index = displayInfo.Find(_T("\n"));
				str = displayInfo.Left(index);
				displayInfo.Delete(0, index+1);
				m_listCtrl.SetItemText(i, 2, str, address);
			}
			else
			{
				index = displayInfo.Find(_T("\n"));
				str = displayInfo.Left(index);
				displayInfo.Delete(0, index+1);
				m_listCtrl.SetItemText(i, 2, str);
			}
		}
	}
}
