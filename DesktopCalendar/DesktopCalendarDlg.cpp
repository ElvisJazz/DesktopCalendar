
// DesktopCalendarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <TlHelp32.h>
#include "DesktopCalendar.h"
#include "DesktopCalendarDlg.h"
#include "PropertyOp.h"
#include "DateSelecter.h"
#include "MySubscription.h"
#include "afxdialogex.h"
#include <dwmapi.h> 
#include <psapi.h>
#include "PictureEx.h"

#pragma comment(lib,"dwmapi.lib")
#pragma comment (lib,"psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_USER_NOTIFYICON WM_USER+1  
 

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	~CAboutDlg();
	void Release();
// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	bool DeleteButton(void);
	CPictureEx m_gif0;
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

CAboutDlg::~CAboutDlg()
{
	m_gif0.UnLoad();
}

void CAboutDlg::Release()
{
	m_gif0.UnLoad();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_STYLE) | WS_EX_LAYERED);  // set extended style
	SetLayeredWindowAttributes(RGB(252,252,252),180,1);
	BOOL bDwn;
	DwmIsCompositionEnabled(&bDwn);
	if (bDwn)
	{
		MARGINS mrg = { -1 };
		DwmExtendFrameIntoClientArea(m_hWnd,&mrg);
		SetBackgroundColor(RGB(252,252,252));
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		//CMFCButton::EnableWindowsTheming();
		//如果 按钮 类型是CButton，则需要关联 CMFCButton 类型按钮，然后使用此函数设定视觉风格
	}

	 if(m_gif0.Load(MAKEINTRESOURCE(IDR_GIF1),_T("gif")))//IDR_GIF1是添加的GIF动画的ID号。gif即是在添加时所键入的文件类型
        m_gif0.Draw();

	
	 return true;
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_gif0);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDesktopCalendarDlg 对话框



CDesktopCalendarDlg::CDesktopCalendarDlg(CWnd* pParent /*=NULL*/)
	: CDlgBase(CDesktopCalendarDlg::IDD, pParent)
{
	m_dlg = NULL;
	//m_dlg1 = NULL;
	m_dlg2 = NULL;
	m_dlg02 = NULL;
	m_calPro = NULL;
	m_pBackBitmap = NULL;
	m_displayDlg = NULL;
	m_displayExDlg = NULL;
	m_advanceDlg = NULL;
	m_taskDlg = NULL;
	m_screenDlg = NULL;

	SetProperty();
	if(m_calPro->isOpenAni && m_dlg == NULL)
	{
		m_dlg = new CFlashDlg();
		//m_dlg->Create(IDD_LOGIN_DIALOG, this);
		//m_dlg->Open();
		m_dlg->DoModal();
		
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDesktopCalendarDlg::~CDesktopCalendarDlg()
{
	
	CPropertyOp::WriteProperty(m_calPro);

	if(m_dlg != NULL)
		delete m_dlg;
	if(m_calPro->mode == 2)
		ReleaseDrawerDlg(_T("LoveCalendar.exe"));
	if(m_dlg2 != NULL)
		delete m_dlg2;
	if(m_dlg02 != NULL)
		delete m_dlg02;
	if(m_calPro != NULL)
		delete m_calPro;
	if(m_displayDlg != NULL)
		delete m_displayDlg;
	if(m_displayExDlg != NULL)
		delete m_displayExDlg;
	if(m_advanceDlg != NULL)
		delete m_advanceDlg;
	if(m_screenDlg != NULL)
		delete m_screenDlg;	
	if(m_taskDlg != NULL)
		delete m_taskDlg;

}

void CDesktopCalendarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CDesktopCalendarDlg, CDlgBase)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CDesktopCalendarDlg::OnBnClickedCancel)
	ON_COMMAND(ID_32771, &CDesktopCalendarDlg::OnDesktopMode)
	ON_COMMAND(ID_32772, &CDesktopCalendarDlg::OnDrawerMode)
	ON_COMMAND(ID_32792, &CDesktopCalendarDlg::OnSetProperty)
	ON_COMMAND(ID_32787, &CDesktopCalendarDlg::OnHideOrShow)
	ON_COMMAND(ID_32789, &CDesktopCalendarDlg::OnLastPage)
	ON_COMMAND(ID_32786, &CDesktopCalendarDlg::OnNextPage)
	ON_COMMAND(ID_32793, &CDesktopCalendarDlg::OnReturnToday)
	ON_COMMAND(ID_32790, &CDesktopCalendarDlg::OnSelectDate)
	ON_COMMAND(ID_32791, &CDesktopCalendarDlg::OnAbout)
	ON_COMMAND(ID_32794, &CDesktopCalendarDlg::OnExit)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDesktopCalendarDlg::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDOK, &CDesktopCalendarDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL2, &CDesktopCalendarDlg::OnBnClickedCancel2)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_32809, &CDesktopCalendarDlg::OnOpenInfoWin)
	ON_COMMAND(ID_32801, &CDesktopCalendarDlg::OnOpenAni)
	ON_COMMAND(ID_32810, &CDesktopCalendarDlg::OnOpenSubscription)
	ON_COMMAND(ID_32811, &CDesktopCalendarDlg::OnTaskMode)
END_MESSAGE_MAP()


// CDesktopCalendarDlg 消息处理程序

BOOL CDesktopCalendarDlg::OnInitDialog()
{
	CDlgBase::OnInitDialog();   // 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(false);
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	// 托盘
	m_notify.cbSize=sizeof NOTIFYICONDATA;  
	m_notify.hWnd=this->m_hWnd;    
	m_notify.uID=IDR_MAINFRAME;  
	m_notify.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));  
	_tcscpy(m_notify.szTip,_T("桌面日历"));  
	m_notify.uCallbackMessage=WM_USER_NOTIFYICON;  
	m_notify.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	Shell_NotifyIcon(NIM_ADD,&m_notify);  

	// 属性窗口架构
	m_tab.InsertItem(0, _T("显示"));
	m_tab.InsertItem(1, _T("订阅"));
	m_tab.InsertItem(2, _T("高级"));
	m_tab.InsertItem(3, _T("任务"));

	m_displayDlg->Create(IDD_TAB1, &m_tab);
	m_displayExDlg->Create(IDD_TAB2, &m_tab);
	m_advanceDlg->Create(IDD_TAB3, &m_tab);
	m_taskDlg->Create(IDD_TAB4, &m_tab);

	m_screenDlg->CutDesktop(); // 截屏
	//获得IDC_TABTEST客户区大小 
	CRect rs; 
	m_tab.GetClientRect(&rs); 
	//ClientToScreen(&rs);
	//调整子对话框在父窗口中的位置 
	rs.top += 23; 
	rs.left += 3;
	rs.right -= 3;
	rs.bottom -= 5;
 
	//设置子对话框尺寸并移动到指定位置 
	m_displayDlg->MoveWindow(&rs); 
	m_displayExDlg->MoveWindow(&rs); 
	m_advanceDlg->MoveWindow(&rs); 
	m_taskDlg->MoveWindow(&rs);

	//分别设置隐藏和显示 
	m_displayDlg->ShowWindow(true); 
	m_displayExDlg->ShowWindow(false); 
	m_advanceDlg->ShowWindow(false);
	m_taskDlg->ShowWindow(false);
 
	//设置默认的选项卡 
	m_tab.SetCurSel(0);

	// 设置桌面截屏显示矩阵
	float width = (float)GetSystemMetrics(SM_CXSCREEN);
	float height = (float)GetSystemMetrics(SM_CYSCREEN);
	CRect rect0;
	GetWindowRect(&rect0);
	m_rect.left = 0;
	m_rect.right = rect0.Width();
	m_rect.top = 0;
	m_rect.bottom = m_rect.top + height/width*rect0.Width();

	int halfGrap = 200 - m_rect.Height()/2;
	m_rect.top += halfGrap;
	m_rect.bottom += halfGrap;

	m_screenDlg = new CScreenCutDlg(this);
	m_screenDlg->Create(IDD_SCREEN_DIALOG, this);
	m_screenDlg->ShowWindow(true);

	float ratio = (float)m_rect.Width()/width;
	rect0.left = m_rect.left + ratio*m_calPro->rect.left;
	rect0.right = m_rect.left + ratio*m_calPro->rect.right;
	rect0.top =  m_rect.top + ratio*m_calPro->rect.top;
	rect0.bottom =  m_rect.top + ratio*m_calPro->rect.bottom;

	m_screenDlg->MoveWindow(rect0);

	CenterWindow();
	
	if(!m_calPro->isOpenAni)
	{
		ShowWindow(false);
		OpenCal();
	}
	else
	{
		OpenCal();
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDesktopCalendarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDlgBase::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDesktopCalendarDlg::OnPaint()
{
	static int n = 0;
	if(n == 0)
	{
		ShowWindow(false);
		n = 1;
		ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
		return;
	}
	else if (IsIconic())
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
		CScreenCutDlg::drawBitmapFormFile(_T("..//INI//background.bmp"), GetDC(), m_rect, 1);
		CDlgBase::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDesktopCalendarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CDesktopCalendarDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)   
{  
	CRect rect; 
    switch(message) //判断消息类型  
    {   
    case WM_USER_NOTIFYICON:   
        //如果是用户定义的消息   
		if(lParam==WM_LBUTTONDBLCLK && m_calPro->mode!=2)  
              
        {   
            //鼠标双击时主窗口出现   
            if(AfxGetApp()->m_pMainWnd->IsWindowVisible()) //判断窗口当前状态  
            {  
                AfxGetApp()->m_pMainWnd->ShowWindow(false); //隐藏窗口  
            }  
            else  
            {  
                AfxGetApp()->m_pMainWnd->ShowWindow(true); //显示窗口  
            }  
              
        }   
        else if(lParam==WM_RBUTTONDOWN)
        { //鼠标右键单击弹出选单   
	        CMenu m,*pm;
			if(!m.LoadMenu(IDR_MENU1))
				MessageBox(_T("获取菜单失败"));//IDR_MENU1是相应的资源id
			pm = m.GetSubMenu(0); //获取子菜单
			CPoint pot;
			GetCursorPos(&pot);//获取鼠标当前位置

			MENUINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUINFO);
			menuInfo.fMask = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND; //子菜单也变色
			HBRUSH br = CreateSolidBrush(RGB(173,201,243));
			menuInfo.hbrBack = br;
			SetMenuInfo(pm->m_hMenu,&menuInfo);
	
			// 设置菜单可选项
			if(m_calPro->isOpenAni)
				m.CheckMenuItem(ID_32801, MF_CHECKED);
			if(m_calPro->isOpenInfoWin)
				m.CheckMenuItem(ID_32809, MF_CHECKED);

			if(m_calPro->mode >= 2)
			{
				if(m_calPro->mode == 3)
				{
					pm->CheckMenuItem(ID_32811,MF_CHECKED);
					m.EnableMenuItem(ID_32792, MF_DISABLED|MF_GRAYED);
				}
				else
					pm->CheckMenuItem(ID_32772,MF_CHECKED);
				
				
				m.EnableMenuItem(ID_32789, MF_DISABLED|MF_GRAYED);
				m.EnableMenuItem(ID_32786, MF_DISABLED|MF_GRAYED);
				m.EnableMenuItem(ID_32790, MF_DISABLED|MF_GRAYED);
				m.EnableMenuItem(ID_32793, MF_DISABLED|MF_GRAYED);
				m.EnableMenuItem(ID_32809, MF_DISABLED|MF_GRAYED);

			}
			else if(m_calPro->mode == 1)
				pm->CheckMenuItem(ID_32771,MF_CHECKED);


			if(m_calPro->displayMode == 2)
			{
				m.EnableMenuItem(ID_32789, MF_DISABLED|MF_GRAYED);
				m.EnableMenuItem(ID_32786, MF_DISABLED|MF_GRAYED);
			}
			
			pm->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pot.x,pot.y,AfxGetMainWnd());//在鼠标位置弹出菜单		 
            //资源回收     
            HMENU    hmenu=m.Detach();     
            m.DestroyMenu();     
             
        }   
        break;   
	

    case WM_SYSCOMMAND:   
        //如果是系统消息   
        if(wParam==SC_CLOSE)  
        {   
            //接收到最小化消息时主窗口隐藏   
            AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);   
            return 0;   
        } 
		else if(wParam==SC_MINIMIZE)
			return 0;
        break;  
   
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

// 清理内存
BOOL EmptyAllSet()
{
    HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(SnapShot==NULL)
    {
      return FALSE;
    }
    PROCESSENTRY32 ProcessInfo;//声明进程信息变量
    ProcessInfo.dwSize=sizeof(ProcessInfo);//设置ProcessInfo的大小
    //返回系统中第一个进程的信息
    BOOL Status=Process32First(SnapShot, &ProcessInfo);
    while(Status)
    {
      HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,ProcessInfo.th32ProcessID);
      if(hProcess)
      {
            SetProcessWorkingSetSize(hProcess,-1,-1);
            //内存整理
            EmptyWorkingSet(hProcess);
            CloseHandle(hProcess);
      }
      //获取下一个进程的信息
      Status=Process32Next(SnapShot,&ProcessInfo);
    }
    return TRUE;
}

void CDesktopCalendarDlg::OnBnClickedCancel()
{
	::Shell_NotifyIcon(NIM_DELETE,&m_notify); //关闭时删除系统托盘图标 
	LONG lStyle=::GetWindowLong(this->m_hWnd,GWL_STYLE);
	::SetWindowLong(this->m_hWnd,GWL_STYLE,lStyle&~WS_CAPTION);
	::SetWindowPos(this->m_hWnd,NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED);	

	if(m_dlg2)
	{
		m_dlg2->ShowWindow(false);
		m_dlg02->ShowWindow(false);
	}
	CDlgBase::OnClose();
}

// 选择桌面模式
void CDesktopCalendarDlg::OnDesktopMode()
{
	if(m_dlg2 && m_calPro->mode == 1)
		return;

	if(m_calPro->mode == 2)
	{
		ReleaseDrawerDlg(_T("LoveCalendar.exe"));
	}
	
	if(m_dlg2 && m_dlg02 && m_calPro->mode!=1)
	{
		m_dlg2->ShowWindow(false);
		m_dlg02->ShowWindow(false);

		delete m_dlg2;
		delete m_dlg02;
		
		m_dlg02 = NULL;
		m_dlg2 = NULL;
	}

	if(m_dlg2==NULL && m_dlg02==NULL)
	{
		m_dlg2 = new CDesktopDlg();
		m_dlg02 = new CDesktopMostDlg();
	}
	m_calPro->mode = 1;

	m_dlg02->SetOwner(m_dlg2);
	if(m_calPro == NULL)
		SetProperty();
	m_calPro->pDlg = m_dlg02;
	m_calPro->pDlg0 = m_dlg2;
	m_dlg02->SetCalProperty(m_calPro);
	m_dlg2->SetCalProperty(m_calPro);

	m_dlg2->Create(IDD_DESKTOP_DIALOG, GetDesktopWindow());
	m_dlg02->Create(IDD_DESKTOP_DIALOG1, m_dlg2);

	m_dlg2->ShowWindow(true);
	m_dlg02->ShowWindow(true);
	(new CPropertyOp())->WriteProperty(m_calPro);
	// 释放内存
	EmptyAllSet();
	// 释放CPU占用
	Sleep(0);
	
}

// 选择抽屉模式
void CDesktopCalendarDlg::OnDrawerMode()
{
	m_calPro->mode = 2;

	if(m_dlg2 && m_dlg02)
	{
		m_dlg2->ShowWindow(false);
		m_dlg02->ShowWindow(false);

		delete m_dlg2;
		delete m_dlg02;
		
		m_dlg02 = NULL;
		m_dlg2 = NULL;
		m_calPro->pDlg = NULL;
		m_calPro->pDlg0 = NULL;
	}
	if(NULL == CheckProcess(_T("LoveCalendar.exe")))
	{
		//m_dlg1 = new CDrawerDlg();
		//m_dlg1->Create(IDD_DRAWER_DIALOG, this);
		//m_dlg1->ShowWindow(true);
		WinExec("LoveCalendar.exe", SW_SHOW);
		(new CPropertyOp())->WriteProperty(m_calPro);
	}
	// 释放内存
	EmptyAllSet();
	// 释放CPU占用
	Sleep(0);
}

// 选择任务模式
void CDesktopCalendarDlg::OnTaskMode()
{
	if(m_dlg2 && m_calPro->mode==3)
		return;

	if(m_dlg2 && m_dlg02)
	{
		m_dlg2->ShowWindow(false);
		m_dlg02->ShowWindow(false);

		delete m_dlg2;
		delete m_dlg02;
		
		m_dlg02 = NULL;
		m_dlg2 = NULL;
	}
	if(m_dlg2==NULL && m_dlg02==NULL)
	{
		
		m_dlg2 = new CDesktopDlg();
		m_dlg02 = new CDesktopMostDlg();
	}
	m_calPro->mode = 3;

	m_dlg02->SetOwner(m_dlg2);
	if(m_calPro == NULL)
		SetProperty();
	m_calPro->pDlg = m_dlg02;
	m_calPro->pDlg0 = m_dlg2;
	m_dlg02->SetCalProperty(m_calPro);
	m_dlg2->SetCalProperty(m_calPro);

	m_dlg2->Create(IDD_DESKTOP_DIALOG, GetDesktopWindow());
	m_dlg02->Create(IDD_DESKTOP_DIALOG1, m_dlg2);

	m_dlg2->ShowWindow(true);
	m_dlg02->ShowWindow(true);
	(new CPropertyOp())->WriteProperty(m_calPro);
	// 释放内存
	EmptyAllSet();
	// 释放CPU占用
	Sleep(0);
}

// 设置属性
void CDesktopCalendarDlg::OnSetProperty()
{
	ShowWindow(true);
}


// 显示或隐藏日历
void CDesktopCalendarDlg::OnHideOrShow()
{
	if(m_dlg2)
	{
		if(m_dlg2->IsWindowVisible())
		{
			m_dlg2->ShowWindow(false);
			m_dlg02->ShowWindow(false);
		}
		else
		{
			m_dlg2->ShowWindow(true);
			m_dlg02->ShowWindow(true);
		}
	}
	else if(m_calPro->mode == 2)
	{
		if(CheckProcess(_T("LoveCalendar.exe")))
			ReleaseDrawerDlg(_T("LoveCalendar.exe"));
		else		
			WinExec("LoveCalendar.exe", SW_SHOW);			
	}
}

// 上一页
void CDesktopCalendarDlg::OnLastPage()
{
	m_calPro->prePage--;
	SetProperty();
	Fresh();
}

// 下一页
void CDesktopCalendarDlg::OnNextPage()
{
	m_calPro->nextPage++;
	SetProperty();
	Fresh();
}

// 返回今日
void CDesktopCalendarDlg::OnReturnToday()
{
	m_calPro->nextPage = 0;
	m_calPro->prePage = 0;
	m_calPro->isCalTime = false;
	SetProperty();
	Fresh();
}

// 选择日期
void CDesktopCalendarDlg::OnSelectDate()
{
	CDateSelecter dlg(m_calPro, NULL);
	dlg.DoModal();

	m_calPro->isCalTime = true;
	m_calPro->nextPage = 0;
	m_calPro->prePage = 0;

	SetProperty();
	Fresh();
}

// 相关
void CDesktopCalendarDlg::OnAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

// 退出
void CDesktopCalendarDlg::OnExit()
{
	OnBnClickedCancel();
}

CDesktopDlg* CDesktopCalendarDlg::GetDesktopDlg(void)
{
	return m_dlg2;
}

void CDesktopCalendarDlg::SetProperty()
{
	int tmp0,tmp1,tmp2;
	int cx = 0;
	int cy = 0;
	int cd = 0;
	int cm = 0;
	// 第一次初始化操作
	static bool isInitial = false;
	if(!isInitial)
	{	
		m_calPro = new CalProperty();
		m_calPro->memoMap = new map<CString, Memo*>();
		m_calPro->nextPage = 0;
		m_calPro->prePage = 0;
		m_calPro->pDlg = NULL;
		m_calPro->pDlg0 = NULL;
		m_calPro->pTextDlg = NULL;
		m_calPro->font = NULL;
		m_calPro->isCalTime = false;
		m_calPro->isSetFirstWeek = false;
		m_calPro->isSetFont = false;
		m_calPro->fontColor = COMMON_TEXT_COLOR;

		CPropertyOp::ReadProperty(m_calPro);
		m_displayDlg = new CDisplayDlg(m_calPro);
		m_displayExDlg = new CDisplayExDlg(m_calPro);
		m_advanceDlg = new CAdvanceDlg();
		m_taskDlg = new CSetTaskDlg(m_calPro);
		isInitial = true;
		

	}
	
	m_calPro->posOfUnion.clear();
	int offset = 0;
	if(m_calPro->displayMode != 2)
		offset = m_calPro->nextPage+m_calPro->prePage;

	//m_calPro->nextPage = m_calPro->prePage = 0;

	if(!m_calPro->isCalTime)
		m_calPro->time = CTime::GetCurrentTime();
	
	if(offset != 0)
	{	
		if(m_calPro->displayMode != 3)
		{
			CTimeSpan timeSpan(abs(offset)*7, 0, 0, 0);
		
			if(offset > 0)
				m_calPro->time += timeSpan;
			else
				m_calPro->time -= timeSpan;
		}
		else
		{
			int year = m_calPro->time.GetYear();
			int month = m_calPro->time.GetMonth();
			int day = m_calPro->time.GetDay();

			month += offset;
			if(month < 1)
			{
				year--;
				month += 12;
			}
			else if(month > 12)
			{
				year++;
				month -= 12;
			}

			m_calPro->time = CTime(year, month, day, 0, 0, 0); 
		}
	}
	

	int year = m_calPro->time.GetYear();
	if(year%400==0 || year%100!=0 && year%4==0)
		g_days[1] = 29;
	else
		g_days[1] = 28;

	if(m_calPro->time.GetDayOfWeek() >= m_calPro->startXinqi)
		m_calPro->currentX = m_calPro->time.GetDayOfWeek()-m_calPro->startXinqi+1;
	else
		m_calPro->currentX = m_calPro->time.GetDayOfWeek()-m_calPro->startXinqi+8;

	if(m_calPro->displayMode == 1)
	{
		if(m_calPro->currentGoneRows != -1)
			m_calPro->currentY = m_calPro->currentGoneRows+1;
		else
		{
			m_calPro->currentY = m_calPro->row%2==0? m_calPro->row/2:m_calPro->row/2+1;
			m_calPro->currentGoneRows = m_calPro->currentY-1;
		}
	}
	else
	{
		int tmp00 = m_calPro->time.GetDay()-m_calPro->currentX;
		if(tmp00 <= 0)
			m_calPro->currentY = 1;
		else
		{
			m_calPro->currentY = tmp00 / 7;
			if(tmp00 % 7 > 0)
				m_calPro->currentY += 2;
			else 
				m_calPro->currentY += 1;
		}
		
		if(m_calPro->displayMode == 3)
		{
			cx = m_calPro->currentX;
			cy = m_calPro->currentY;
			cd = m_calPro->time.GetDay();
			cm = m_calPro->time.GetMonth();

			int t = g_days[cm-1]-cd+cx-7;
			tmp0 = t/7;
			tmp1 = t%7;

			if(t >= 0)
			{
				cy += tmp0;
				cx = tmp1+1;
			}
			else
				cx = abs(t) + 1;

			if(t>=0 && tmp1!=0)
				cy++;

			m_calPro->row = cy;
		}
	}
	
	
    // 设置合并行参数
	m_calPro->numOfUnion = 0;
	int i = 0;
	cx = m_calPro->currentX;
	cy = m_calPro->currentY;
	cd = m_calPro->time.GetDay();
	cm = m_calPro->time.GetMonth();
	do
	{
		if(cd < cx)
		{
			cx -= cd;
			cy--;
			cd = g_days[cm-2];
			cm--;
			if(m_calPro->displayMode==1)
			{
				m_calPro->numOfUnion++;
				m_calPro->posOfUnion.insert(m_calPro->posOfUnion.begin(), make_pair(cx, cy));

				if(cy == 1)
					m_calPro->startDay = 1;
			}
			
		}
		
		if(cy == 0)
		{
			m_calPro->posOfUnion.insert(m_calPro->posOfUnion.begin(), make_pair(1, -1));
			m_calPro->startMonth = m_calPro->time.GetMonth()-m_calPro->numOfUnion;
			m_calPro->startDay = 1;
			break;
		}

		tmp0 = (cd-cx)/7;
		tmp1 = (cd-cx)%7;
		if(tmp1 == 0)
			tmp2 = cy-tmp0-1;
		else
			tmp2 = cy-tmp0-2;

		if(m_calPro->displayMode == 3)
		{
			m_calPro->startMonth = m_calPro->time.GetMonth()-m_calPro->numOfUnion;
			break;
		}
		if((tmp2==1 && tmp1==0) || tmp2==0) // 这个月顶格
		{
			if(tmp1==0 && tmp2==1)
			{
				cx = 1;
				cy = 1;
				m_calPro->startDay = 0;
				m_calPro->numOfUnion++;
				m_calPro->startMonth = m_calPro->time.GetMonth()-m_calPro->numOfUnion+1;
			}
			else
			{
				cx = 8-tmp1;
				cy = -1;
				if(cx == 8)
					cx = 1;
				m_calPro->startDay = 1;
				m_calPro->startMonth = m_calPro->time.GetMonth()-m_calPro->numOfUnion;
			}
			m_calPro->posOfUnion.insert(m_calPro->posOfUnion.begin(), make_pair(cx, cy));
			break;
		}
		else if(tmp2 > 0) // 这个月有前面的月份
		{
			cx = 7-tmp1;
			cy = tmp2;
			m_calPro->numOfUnion++;
			if(cx == 7)
				cx = 0;
			m_calPro->posOfUnion.insert(m_calPro->posOfUnion.begin(), make_pair(cx+1, cy));
			cd = (cm-1>0)? g_days[cm-2]:g_days[10+cm];
			if(cm > 1)
				cm--;
			else
				cm = 12;
		}
		else // 此月不是顶格
		{
			/*if(cx==0 && cy<=1)
			{
				if(cy==1)
				{
					m_calPro->startDay = 0;
					m_calPro->posOfUnion.insert(m_calPro->posOfUnion.begin(), make_pair(1, 1));
				}
			}
			else
			{*/
				m_calPro->startDay = cd-cx-(cy-1)*7+1;
				m_calPro->posOfUnion.insert(m_calPro->posOfUnion.begin(), make_pair(0, 0));
			//}
			m_calPro->startMonth = m_calPro->time.GetMonth()-m_calPro->numOfUnion;
			break;
		}
		i++;
	}while(i<12); // 最多显示一年
	
	if(m_calPro->startMonth <= 0)
		m_calPro->startMonth += 12;

	if(m_calPro->displayMode != 1)
	{
		if(tmp1 == 0)
			tmp1 = 7;
		m_calPro->posOfUnion.push_back(make_pair(8-tmp1, -1));
		m_calPro->startDay = 1;
	}
	
	if(m_calPro->displayMode != 3)
	{	
		cx = m_calPro->currentX;
		cy = m_calPro->currentY;
		cd = m_calPro->time.GetDay();
		cm = m_calPro->time.GetMonth();

		do
		{
			int t = g_days[cm-1]-cd+cx-7;
			tmp0 = t/7;
			tmp1 = t%7;

			if(t >= 0)
			{
				cy += tmp0;
				cx = tmp1+1;
			}
			else
				cx = abs(t) + 1;

			if(t>=0 && tmp1!=0)
				cy++;
			if(cy < m_calPro->row)
			{
				if(cx == 1)
					cy++;
				m_calPro->posOfUnion.push_back(make_pair(cx, cy));
			}
			else
				break;

			cy++;
			cm = (cm==12)? 1:cm+1;
			cd = 1;
		}while(true);
	}

	// 修正startday
	if(m_calPro->startDay > g_days[m_calPro->startMonth-1])
		m_calPro->startDay = g_days[m_calPro->startMonth-1];

	// 设置显示的间隔宽度
	m_calPro->widthTmp = ((m_calPro->rect.Width())- 24 - (m_calPro->grap)*6) / 7;
	m_calPro->widthTmp2 = (m_calPro->rect.Height()- 54) / m_calPro->row;

}

CalProperty* CDesktopCalendarDlg::GetProperty()
{
	return m_calPro;
}

 
// 刷新日历
void CDesktopCalendarDlg::Fresh(void)
{
	if(m_dlg2)
	{
		m_dlg2->ShowWindow(false);
		m_dlg02->ShowWindow(false);
	
		delete m_dlg2;
		delete m_dlg02;

		map<CString, Memo*>::iterator iter = m_calPro->memoMap->begin();
		for(; iter!=m_calPro->memoMap->end(); iter++)
			delete (*iter).second;
		m_calPro->memoMap->clear();
	}
	m_dlg02 = NULL;
	m_dlg2 = NULL;

	if(m_calPro->mode == 1)
		OnDesktopMode();
	else if(m_calPro->mode == 3)
		OnTaskMode();
}


void CDesktopCalendarDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int curSel = m_tab.GetCurSel(); 
	switch(curSel) 
	{ 
	 case 0: 
			 m_displayDlg->ShowWindow(true); 
			 m_displayExDlg->ShowWindow(false); 
			 m_advanceDlg->ShowWindow(false); 
			 m_taskDlg->ShowWindow(false);
			 break; 
	 case 1: 
			 m_displayDlg->ShowWindow(false); 
			 m_displayExDlg->ShowWindow(true); 
			 m_advanceDlg->ShowWindow(false); 
			 m_taskDlg->ShowWindow(false);
			break; 
	 case 2: 
			 m_displayDlg->ShowWindow(false); 
			 m_displayExDlg->ShowWindow(false); 
			 m_advanceDlg->ShowWindow(true); 
			 m_taskDlg->ShowWindow(false);
			break; 
	 case 3: 
			 m_displayDlg->ShowWindow(false); 
			 m_displayExDlg->ShowWindow(false); 
			 m_advanceDlg->ShowWindow(false); 
			 m_taskDlg->ShowWindow(true);
			break; 
	 default: 
            break; 
     }     

	*pResult = 0;
}


void CDesktopCalendarDlg::OnBnClickedOk()
{
	ShowWindow(false);
	m_displayDlg->SetProperty();
	m_displayExDlg->SetProperty();
	m_screenDlg->SetProperty();
	m_taskDlg->SetProperty();
	SetProperty();
	Fresh();
}


void CDesktopCalendarDlg::OnBnClickedCancel2()
{
	ShowWindow(false);
	// 释放内存
	EmptyAllSet();
	// 释放CPU占用
	Sleep(0);
}


// 获取截屏矩阵
CRect CDesktopCalendarDlg::GetDisplayRect(void)
{
	return m_rect;
}

void CDesktopCalendarDlg::OpenCal()
{
	if(m_calPro->mode == 2)
		OnDrawerMode();
	else if(m_calPro->mode == 1)
		OnDesktopMode();
	else
		OnTaskMode();
	
}

HBRUSH CDesktopCalendarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	if(nCtlColor == CTLCOLOR_DLG) 
	{ 
		pDC-> SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COMMON_TEXT_COLOR);
		HBRUSH hBrush = CreateSolidBrush(TRANSPARENT_COLOR);
		return hBrush; 
	}

	return  CDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);
}



void CDesktopCalendarDlg::OnOpenInfoWin()
{
	if(m_dlg02)
		m_dlg02->OnOpenInfoWin();
}


void CDesktopCalendarDlg::OnOpenAni()
{
	if(!m_calPro->isOpenAni)
		m_calPro->isOpenAni = true;
	else
		m_calPro->isOpenAni = false;
}


void CDesktopCalendarDlg::OnOpenSubscription()
{
	CMySubscription mySubscriptionDlg(m_calPro);
	mySubscriptionDlg.DoModal();
}


// 删除抽屉式日历
bool CDesktopCalendarDlg::ReleaseDrawerDlg(CString name)
{
	HANDLE hProcess = CheckProcess(name);
	return ::CloseHandle(hProcess);

}

// 检测进程是否存在
HANDLE CDesktopCalendarDlg::CheckProcess(CString name)
{
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	PROCESSENTRY32 info = {0};
	info.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcess, &info))
	{
		return NULL;
	}
	BOOL bFound = FALSE;
	while (1)
	{
		if (!Process32Next(hProcess, &info))
		{
			break;
		}
		if (info.szExeFile == name)
		{
			bFound = TRUE;
			HANDLE hClose = ::OpenProcess(PROCESS_TERMINATE, FALSE, info.th32ProcessID);
			if (hClose == NULL)
			{
				return NULL;
			}
			if (!::TerminateProcess(hClose, 0))
			{
				return NULL;
			}
			else
			{
				return hClose;
			}
		}
	}

	return NULL;
}
