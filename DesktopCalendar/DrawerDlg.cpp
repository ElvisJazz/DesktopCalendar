// DrawerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DrawerDlg.h"
#include "afxdialogex.h"
#include "DlgBase.h"
#include "MyButton.h"


// CDrawerDlg 对话框

IMPLEMENT_DYNAMIC(CDrawerDlg, CDlgBase)

CDrawerDlg::CDrawerDlg(CWnd* pParent /*=NULL*/)
	: CDlgBase(CDrawerDlg::IDD, pParent)
{

}

CDrawerDlg::~CDrawerDlg()
{
}

void CDrawerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDrawerDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDrawerDlg 消息处理程序


BOOL CDrawerDlg::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	//SetLayeredWindowAttributes(RGB(220,238,212),100,LWA_COLORKEY);
	//设置透明
	ModifyStyleEx(0,0x00080000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes（）函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),RGB(255,235,205),220,2); 
		FreeLibrary(hInst); 
	} 

	CRect tRect;
	GetWindowRect(tRect);
	tRect.right = tRect.left + 327;
	tRect.bottom =tRect.top + 680;
	SetWindowPos(&wndTopMost,tRect);
	//界面和音效初始化
	m_hBrush=CreateSolidBrush(RGB(255,235,205));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 动态添加我的按钮
CMyButton* CDrawerDlg::NewMyButton(int nID, CRect rect, int nStyle)
{
	CString m_Caption;
	m_Caption.LoadString(nID); //取按钮标题
	CMyButton *p_Button = new CMyButton();
	//ASSERT_VALID(p_Button);

	if(!p_Button->Create(m_Caption, WS_VISIBLE|BS_DEFPUSHBUTTON|WS_CHILD|BS_AUTORADIOBUTTON|BS_FLAT|nStyle, rect, this, nID))
		AfxMessageBox(_T("创建异常！"));
	return p_Button;
}

HBRUSH CDrawerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(nCtlColor==CTLCOLOR_DLG || nCtlColor==CTLCOLOR_STATIC) 
	{ 
		pDC-> SetBkMode(TRANSPARENT); 
		//此处设置字体的颜色 
		pDC->SetTextColor(RGB(0,0,255));
		return m_hBrush; 
	} 
	else 
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
