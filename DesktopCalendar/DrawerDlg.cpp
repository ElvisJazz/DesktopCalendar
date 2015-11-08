// DrawerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DrawerDlg.h"
#include "afxdialogex.h"
#include "DlgBase.h"
#include "MyButton.h"


// CDrawerDlg �Ի���

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


// CDrawerDlg ��Ϣ�������


BOOL CDrawerDlg::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	//SetLayeredWindowAttributes(RGB(220,238,212),100,LWA_COLORKEY);
	//����͸��
	ModifyStyleEx(0,0x00080000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//ȡ��SetLayeredWindowAttributes��������ָ�� 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),RGB(255,235,205),220,2); 
		FreeLibrary(hInst); 
	} 

	CRect tRect;
	GetWindowRect(tRect);
	tRect.right = tRect.left + 327;
	tRect.bottom =tRect.top + 680;
	SetWindowPos(&wndTopMost,tRect);
	//�������Ч��ʼ��
	m_hBrush=CreateSolidBrush(RGB(255,235,205));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// ��̬����ҵİ�ť
CMyButton* CDrawerDlg::NewMyButton(int nID, CRect rect, int nStyle)
{
	CString m_Caption;
	m_Caption.LoadString(nID); //ȡ��ť����
	CMyButton *p_Button = new CMyButton();
	//ASSERT_VALID(p_Button);

	if(!p_Button->Create(m_Caption, WS_VISIBLE|BS_DEFPUSHBUTTON|WS_CHILD|BS_AUTORADIOBUTTON|BS_FLAT|nStyle, rect, this, nID))
		AfxMessageBox(_T("�����쳣��"));
	return p_Button;
}

HBRUSH CDrawerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(nCtlColor==CTLCOLOR_DLG || nCtlColor==CTLCOLOR_STATIC) 
	{ 
		pDC-> SetBkMode(TRANSPARENT); 
		//�˴������������ɫ 
		pDC->SetTextColor(RGB(0,0,255));
		return m_hBrush; 
	} 
	else 
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
