
// DlgBase.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgBase.h"
#include "afxdialogex.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDlgBase 对话框


CDlgBase::CDlgBase(UINT x, CWnd* pParent /*=NULL*/)
	: CDialogEx(x, pParent)
{
	m_isSizeChanged = FALSE;
	m_isSetTimer = FALSE;
	m_hsFinished = TRUE;
	m_hiding = FALSE;
	m_oldWndHeight = MINCY;
	m_taskBarHeight = 30;
	m_edgeHeight = 0;
	m_edgeWidth=0;
	m_hideMode = HM_NONE;
	m_closeMode = 0;

	m_IsBiggest = false;
}

void CDlgBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgBase, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOVING()
	ON_WM_NCHITTEST()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgBase 消息处理程序

BOOL CDlgBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;
	GetClientRect(&rect);
	m_height = rect.Height();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	ShowWindow(SW_HIDE);
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW,0);
	return false;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDlgBase::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDlgBase::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDlgBase::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*特效模块*/
int CDlgBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//获得任务栏高度
	CWnd* p;
	p = this->FindWindow(CString("Shell_TrayWnd"), NULL);
	if(p != NULL)
	{
		CRect tRect;
		p->GetWindowRect(tRect);
		m_taskBarHeight = tRect.Height();
	}
	
	//去掉关闭按键(如果想画3个按键的话)
	//ModifyStyle(WS_SYSMENU,NULL);
	//获得边缘高度和宽度
	m_edgeHeight = GetSystemMetrics(SM_CYEDGE)/2;
	m_edgeWidth = GetSystemMetrics(SM_CXFRAME)/2;
	return 0;
}

LRESULT CDlgBase::OnNcHitTest(CPoint point)
{
	CString str;
	str.Format(_T("Mouse (%d,%d)"),point.x,point.y);
	//GetDlgItem(IDC_CURSOR)->SetWindowText(str);
	if(m_hideMode != HM_ALL &&m_hideMode != HM_NONE && !m_isSetTimer &&
	//防止鼠标超出屏幕右边时向右边收缩造成闪烁
	point.x < GetSystemMetrics(SM_CXSCREEN) + INFALTE)
	{ //鼠标进入时,如果是从收缩状态到显示状态则开启Timer
		SetTimer(1,CM_ELAPSE,NULL);
		m_isSetTimer = TRUE;
		m_hsFinished = FALSE;
		m_hiding = FALSE;
		SetTimer(2,HS_ELAPSE,NULL); //开启显示过程
	}
	CRect tRect;
	GetWindowRect(tRect);
	UINT   nHitTest   =   CDialog::OnNcHitTest(point);   
	if((nHitTest== HTCLIENT)  && (::GetAsyncKeyState(MK_LBUTTON)<0) && point.x>tRect.left && point.x<tRect.right
		&& point.y>tRect.top && point.y<tRect.top+100)   
		 nHitTest   =   HTCAPTION;   
	return   nHitTest;  
}

void CDlgBase::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1 )
	{
		POINT curPos;
		GetCursorPos(&curPos);
		CString str;
		str.Format(_T("Timer On(%d,%d)"),curPos.x,curPos.y);
		//GetDlgItem(IDC_TIMER)->SetWindowText(str);
		CRect tRect;
		//获取此时窗口大小
		GetWindowRect(tRect);
		//膨胀tRect,以达到鼠标离开窗口边沿一定距离才触发事件
		tRect.InflateRect(INFALTE,INFALTE);

		if(!tRect.PtInRect(curPos)) //如果鼠标离开了这个区域
		{
			KillTimer(1); //关闭检测鼠标Timer
			m_isSetTimer = FALSE;
			//GetDlgItem(IDC_TIMER)->SetWindowText(CString("Timer Off"));
			m_hsFinished = FALSE;
			m_hiding = TRUE;
			SetTimer(2,HS_ELAPSE,NULL); //开启收缩过程
		}
	}

	if(nIDEvent == 2)
	{
		static int num = 0;
		if(m_closeMode==1 && m_hiding)
		{
			num++;
			if(num == 1000)
			{
				num = 0;
				m_closeMode = 0;
				m_hiding = true;
			}
			else
				return;
		}

		if(m_hsFinished) //如果收缩或显示过程完毕则关闭Timer
		{
			KillTimer(2);
			if(m_hideMode==HM_ALL && m_hiding)
				CDialogEx::OnCancel();
		}
		else
			m_hiding ? DoHide() : DoShow();
	}
}

void CDlgBase::FixMoving(UINT fwSide, LPRECT pRect)
{
	POINT curPos;
	GetCursorPos(&curPos);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int height = pRect->bottom - pRect->top;
	int width = pRect->right - pRect->left;

	if (curPos.y <= INTERVAL)
	{ //粘附在上边
		pRect->bottom = height - m_edgeHeight;
		pRect->top = -m_edgeHeight;
		m_hideMode = HM_TOP;
	}
	else if(curPos.y >= (screenHeight - INTERVAL - m_taskBarHeight))
	{ //粘附在下边
		pRect->top = screenHeight - m_taskBarHeight - height;
		pRect->bottom = screenHeight - m_taskBarHeight;
		m_hideMode = HM_BOTTOM;
	}
	else if (curPos.x < INTERVAL)
	{ //粘附在左边
		if(!m_isSizeChanged)
		{
		CRect tRect;
		GetWindowRect(tRect);
		m_oldWndHeight = tRect.Height();
		}
		pRect->right = width;
		pRect->left = 0;
		pRect->top = -m_edgeHeight;
		pRect->bottom = screenHeight - m_taskBarHeight;
		m_isSizeChanged = TRUE;
		m_hideMode = HM_LEFT;
	}
	else if(curPos.x >= (screenWidth - INTERVAL))
	{ //粘附在右边
		if(!m_isSizeChanged)
		{
			CRect tRect;
			GetWindowRect(tRect);
			m_oldWndHeight = tRect.Height();
		}
		pRect->left = screenWidth - width;
		pRect->right = screenWidth;
		pRect->top = -m_edgeHeight;
		pRect->bottom = screenHeight - m_taskBarHeight;
		m_isSizeChanged = TRUE;
		m_hideMode = HM_RIGHT;
	}
	else
	{ //不粘附
		if(m_isSizeChanged)
		{	//如果收缩到两边,则拖出来后会变回原来大小
			//在"拖动不显示窗口内容下"只有光栅变回原来大小
			pRect->bottom = pRect->top + m_oldWndHeight;
			m_isSizeChanged = FALSE;
		}
		if(m_isSetTimer)
		{ //如果Timer开启了,则关闭之
			if(KillTimer(1) == 1)
				m_isSetTimer = FALSE;
		}
		m_hideMode = HM_NONE;
	//	GetDlgItem(IDC_TIMER)->SetWindowText(CString("Timer off"));
	}
}

void CDlgBase::DoHide()
{
	if(m_hideMode == HM_NONE)
		return;

	CRect tRect;
	GetWindowRect(tRect);
	int height = tRect.Height();
	int width = tRect.Width();
	int steps = 0;
	
	switch(m_hideMode)
	{
	case HM_TOP:
				steps = height/HS_STEPS;
				tRect.bottom -= steps;
				if(tRect.bottom <= m_edgeWidth)
				{ //你可以把下面一句替换上面的 ...+=|-=steps 达到取消抽屉效果
				//更好的办法是添加个BOOL值来控制,其他case同样.
					tRect.bottom = m_edgeWidth;
					m_hsFinished = TRUE; //完成隐藏过程
				}
				tRect.top = tRect.bottom - height;
				break;
	case HM_BOTTOM:
				steps = height/HS_STEPS;
				tRect.top += steps;
				if(tRect.top >= (GetSystemMetrics(SM_CYSCREEN) - m_edgeWidth))
				{
					tRect.top = GetSystemMetrics(SM_CYSCREEN) - m_edgeWidth;
					m_hsFinished = TRUE;
				}
				tRect.bottom = tRect.top + height;
				break;
	case HM_LEFT:
				steps = width/HS_STEPS;
				tRect.right -= steps;
				if(tRect.right <= m_edgeWidth)
				{
					tRect.right = m_edgeWidth;
					m_hsFinished = TRUE;
				}
				tRect.left = tRect.right - width;
				tRect.top = -m_edgeHeight;
				tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
				break;
	case HM_RIGHT:
				steps = width/HS_STEPS;
				tRect.left += steps;
				if(tRect.left >= (GetSystemMetrics(SM_CXSCREEN) - m_edgeWidth))
				{
					tRect.left = GetSystemMetrics(SM_CXSCREEN) - m_edgeWidth;
					m_hsFinished = TRUE;
				}
				tRect.right = tRect.left + width;
				tRect.top = -m_edgeHeight;
				tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
				break;
	case HM_ALL:
				steps = m_height/HS_STEPS/4;
			    tRect.bottom -= steps;
				if(m_closeMode == 2)
					tRect.top += steps;
				//SetLayeredWindowAttributes(RGB(255,235,205), height/m_height*255, 2); 
				if(tRect.bottom - tRect.top <= 10)
				{
					m_hsFinished = TRUE;
					//CDialogEx::OnClose();
				}
				break;
	default:
				break;	
	}																																
	SetWindowPos(&wndTopMost,tRect);
}

void CDlgBase::DoShow()
{
	if(m_hideMode == HM_NONE)
		return;
	CRect tRect;
	GetWindowRect(tRect);
	int height = tRect.Height();
	int width = tRect.Width();
	int steps = 0;
	
	switch(m_hideMode)
	{
	case HM_TOP:
				steps = height/HS_STEPS;
				tRect.top += steps;
				if(tRect.top >= -m_edgeHeight)
				{ //你可以把下面一句替换上面的 ...+=|-=steps 达到取消抽屉效果
					//更好的办法是添加个BOOL值来控制,其他case同样.
					tRect.top = -m_edgeHeight;
					m_hsFinished = TRUE; //完成显示过程
				}
				tRect.bottom = tRect.top + height;
				break;
	case HM_BOTTOM:
				steps = height/HS_STEPS;
				tRect.top -= steps;
				if(tRect.top <= (GetSystemMetrics(SM_CYSCREEN) - height))
				{
					tRect.top = GetSystemMetrics(SM_CYSCREEN) - height;
					m_hsFinished = TRUE;
				}
				tRect.bottom = tRect.top + height;
				break;
	case HM_LEFT:
				steps = width/HS_STEPS;
				tRect.right += steps;
				if(tRect.right >= width)
				{
					tRect.right = width;
					m_hsFinished = TRUE;
				}
				tRect.left = tRect.right - width;
				tRect.top = -m_edgeHeight;
				tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
				break;
	case HM_RIGHT:
				steps = width/HS_STEPS;
				tRect.left -= steps;
				if(tRect.left <= (GetSystemMetrics(SM_CXSCREEN) - width))
				{
					tRect.left = GetSystemMetrics(SM_CXSCREEN) - width;
					m_hsFinished = TRUE;
				}
				tRect.right = tRect.left + width;
				tRect.top = -m_edgeHeight;
				tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
				break;
	case HM_ALL:steps = m_height/HS_STEPS/4;
			    tRect.bottom += steps;
				//SetLayeredWindowAttributes(RGB(255,235,205), height/m_height*255, 2); 
				if(tRect.bottom - tRect.top >= m_height-5)
				{
					m_hsFinished = TRUE;
					
					if(m_closeMode == 1)
					{
						m_hsFinished = FALSE;
						m_hideMode = HM_ALL;
						m_hiding = TRUE;
						SetTimer(2, HS_ELAPSE, NULL);
					}
				}
				break;
	default:
				break;
	}
	SetWindowPos(&wndTopMost,tRect);
}

BOOL CDlgBase::SetWindowPos(const CWnd* pWndInsertAfter, LPCRECT pCRect, UINT nFlags)
{
	return CDialog::SetWindowPos(pWndInsertAfter,pCRect->left, pCRect->top,
		pCRect->right - pCRect->left, pCRect->bottom - pCRect->top, nFlags);
}

void CDlgBase::OnMoving(UINT fwSide, LPRECT pRect)
{
	FixMoving(fwSide, pRect);
	CDialog::OnMoving(fwSide, pRect);
}


void CDlgBase::OnClose()
{
	if(IsWindowVisible())
	{
		m_closeMode = 2;
		m_hsFinished = FALSE;
		m_hideMode = HM_ALL;
		m_hiding = TRUE;
		SetTimer(2, HS_ELAPSE, NULL);
	}
	else
		CDialogEx::OnCancel();
}