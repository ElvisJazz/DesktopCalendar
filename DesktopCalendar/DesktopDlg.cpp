// DesktopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DesktopDlg.h"
#include "DesktopMostDlg.h"
#include "afxdialogex.h"
#include "winuser.h"
#include <dwmapi.h>  
#pragma comment(lib,"dwmapi.lib")

// CDesktopDlg 对话框

IMPLEMENT_DYNAMIC(CDesktopDlg, CDlgBase)
	HBRUSH hBrush;

CDesktopDlg::CDesktopDlg(CWnd* pParent /*=NULL*/)
	: CDlgBase(CDesktopDlg::IDD, pParent)
{
	m_pPro = NULL;
}

CDesktopDlg::~CDesktopDlg()
{
	Release();
}

void CDesktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDesktopDlg, CDlgBase)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDesktopDlg 消息处理程序


BOOL CDesktopDlg::OnInitDialog()
{
	CDlgBase::OnInitDialog();
	
	for(int i=0; i<8; i++)
		m_staticBlock[i] = NULL;
	if(m_pPro != NULL)
		ControlAeroStyle();

	BringWindowToTop();
	::SetWindowPos(m_hWnd, wndBottom, m_pPro->rect.left, m_pPro->rect.top, m_pPro->rect.Width(), m_pPro->rect.Height(), SWP_SHOWWINDOW);

	if(m_pPro->mode == 1)
		SetBlock();
	else if(m_pPro->mode == 3)
		SetTaskBlock();
	return true;
}

HBRUSH CDesktopDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(nCtlColor == CTLCOLOR_DLG) 
	{ 
		pDC-> SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(COMMON_TEXT_COLOR);
		hBrush = CreateSolidBrush(TRANSPARENT_COLOR);
		return hBrush; 
	}
	/*	else if(nCtlColor == CTLCOLOR_STATIC)
    { 
		pDC-> SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COMMON_TEXT_COLOR);
		hBrush = CreateSolidBrush(TRANSPARENT_COLOR);
		return hBrush; 
	}
	else if(nCtlColor == CTLCOLOR_BTN)
    { 
		pDC->SetTextColor(COMMON_TEXT_COLOR);
		hBrush = CreateSolidBrush(RGB(0,0,0));
		return hBrush; 
	}*/
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


// 开启毛玻璃效果
void CDesktopDlg::ControlAeroStyle()
{
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED); 
	SetLayeredWindowAttributes(TRANSPARENT_COLOR, m_pPro->transparentLevel, 3);
	if(!m_pPro->openAero)
		ModifyStyle(WS_CAPTION,0,0); // 去除标题栏
	else
	{ 
		ModifyStyle(0,WS_CAPTION,0); // 还原标题栏
		BOOL bDwn;
		DwmIsCompositionEnabled(&bDwn);
		if (bDwn)
		{ 
			MARGINS mrg = { -1 };
			DwmExtendFrameIntoClientArea(m_hWnd,&mrg);
			SetBackgroundColor(TRANSPARENT_COLOR);
			//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
			CMyButton::EnableWindowsTheming();
			//如果 按钮 类型是CButton，则需要关联 CMyButton 类型按钮，然后使用此函数设定视觉风格
		}
	}
}

LRESULT CDesktopDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)   
{
	
	switch(message) //判断消息类型  
    {     
    case WM_SYSCOMMAND:  
		{
			//如果是系统消息   
			if(wParam==SC_MINIMIZE)
				return 0;
			break;  
		
			return 0;
		}
    }  
    return CDialog::WindowProc(message, wParam, lParam);  
}

// 设置色块
void CDesktopDlg::SetBlock(void)
{
	m_staticBlockColor = m_pPro->commonColor;
	m_currentBlockColor = m_pPro->currentColor;

	CTime timeOfToday = CTime::GetCurrentTime();

	int widthTmp = m_pPro->widthTmp;
	int widthTmp2 = m_pPro->widthTmp2;

	int currentDay = m_pPro->startDay;
	int currentMonth = m_pPro->startMonth;

	int yearTmp = m_pPro->time.GetYear();

	// 年月标题栏
	CRect c1(24, 0, m_pPro->rect.Width()-4, 20);
	if(m_staticBlock[0] != NULL)
		delete m_staticBlock[0];
	m_staticBlock[0] = NewMyButton(IDC_MYBUTTON1,c1 , 1);
	m_staticBlock[0]->SetUpColor(m_staticBlockColor);

	// 星期标题栏
	for(int i=1; i<8; i++)
	{
		if(m_staticBlock[i] != NULL)
			delete m_staticBlock[i];
		CRect c2((i-1)*(widthTmp+m_pPro->grap)+24, 22, i*(widthTmp+m_pPro->grap)-m_pPro->grap+24, 42);
		m_staticBlock[i] =  NewMyButton(IDC_MYBUTTON1+i, c2 , 0);
		m_staticBlock[i]->SetUpColor(m_staticBlockColor);
	}

	// 侧栏
	if(m_pPro->isDisplayWeeks)
	{
		CTimeSpan span = CTime(m_pPro->time.GetYear(), m_pPro->time.GetMonth(), m_pPro->time.GetDay(), 0, 0, 0)  - CTime(m_pPro->time.GetYear(), 1, 1, 0, 0, 0);
		int firstWeekTmp = (span.GetDays()-m_pPro->currentX)/7+1;
		if((span.GetDays()-m_pPro->currentX)%7 != 0)
			firstWeekTmp++;

		int tmp  = firstWeekTmp-m_pPro->currentY+m_pPro->numOfUnion+1;
		if(tmp <= 0)
			yearTmp--;


		vector<CMyButton*>::iterator iter=m_sideBlock.begin();
		for(; iter!=m_sideBlock.end(); ++iter)
			delete *iter;
		m_sideBlock.clear();
		vector<pair<int, int> >::iterator iter1 = m_pPro->posOfUnion.begin();
		while((*iter1).second<=0 && iter1+1!=m_pPro->posOfUnion.end())
			iter1++;

		CMyButton* button;
		for(int j=1; j<=m_pPro->row; j++)
		{
			if(j == (*iter1).second)
			{
				CRect c2;
				c2.SetRect(2, (j-1)*(widthTmp2+m_pPro->grap)+44, 22, (j+1)*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);

				button = NewMyButton(IDC_MYBUTTON8+j, c2, 0);
				button->SetUpColor(m_staticBlockColor);
				m_sideBlock.push_back(button);
				if(iter1+1 != m_pPro->posOfUnion.end())
					++iter1;
				j++;
			}
			else
			{
				CRect c2(2, (j-1)*(widthTmp2+m_pPro->grap)+44, 22, j*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);
				button = NewMyButton(IDC_MYBUTTON8+j, c2, 0);
				button->SetUpColor(m_staticBlockColor);
				m_sideBlock.push_back(button);
			}
		}
	}
	
	// 日期色块
	bool nextLine = false; // 默认有换行
	bool setLight = false;
	vector<CMyButton*>::iterator iter2=m_dayBlock.begin();
	for(; iter2!=m_dayBlock.end(); ++iter2)
		delete *iter2;
	m_dayBlock.clear();

	vector<pair<int,int> >::iterator iter3 = m_pPro->posOfUnion.begin();
	int col = 1;
	int row = 1;

	if((*iter3).second==0 && iter3+1 != m_pPro->posOfUnion.end())
			iter3++;
	if((*iter3).second == -1)
	{
		col = (*iter3).first;
		if(iter3+1 != m_pPro->posOfUnion.end())
			++iter3;
	}

	if((yearTmp%400==0 || yearTmp%100!=0) && yearTmp%4==0)
		g_days[1] = 29;
	else
		g_days[1] = 28;

	// 根据当前年份读取年表记录
	ReadInfo(yearTmp);

	if(m_pPro->startDay == 0)
	{
		nextLine = true;
		currentMonth--;
	}

	for(int k=1; k<=7*m_pPro->row && row<=m_pPro->row; k++)
	{
		CString st;

		if(nextLine)
		{
			if(iter3+1 != m_pPro->posOfUnion.end())
				++iter3;

			nextLine = false;
			++currentMonth;
			if(currentMonth > 12)
			{
				currentMonth = 1;
				yearTmp++;
				ReadInfo(yearTmp);
				if((yearTmp%400==0 || yearTmp%100!=0) && yearTmp%4==0)
					g_days[1] = 29;
				else
					g_days[1] = 28;
			}

			if(m_pPro->displayMode != 3)
			{
				CRect c30((col-1)*(widthTmp+m_pPro->grap)+24, (row-1)*(widthTmp2+m_pPro->grap)+44+widthTmp2/2, col*(widthTmp+m_pPro->grap)-m_pPro->grap+24, row*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);
				CMyButton* button0 = NewMyButton(IDC_MYBUTTON8+m_pPro->row+k, c30, 0);
				m_dayBlock.push_back(button0);
				button0->SetUpColor(m_staticBlockColor);
			}
			k += 6;
			row++;
			if(row > m_pPro->row)
				break;
			currentDay = 1;

			if(currentMonth==m_pPro->time.GetMonth() && currentDay==m_pPro->time.GetDay())
				setLight = true;

			st.Format(_T("%d %d %d"), currentDay, currentMonth, yearTmp);
			CRect c31((col-1)*(widthTmp+m_pPro->grap)+24, (row-1)*(widthTmp2+m_pPro->grap)+44, col*(widthTmp+m_pPro->grap)-m_pPro->grap+24, row*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);
			CMyButton* button = NewMyButton(IDC_MYBUTTON8+m_pPro->row+k, c31, 0);
			m_dayBlock.push_back(button);

			CString s01;
			Memo* memo = new Memo();
			s01.Format(_T("%d%d"), col, row);
			memo->button0 = button;
			m_pPro->memoMap->insert(make_pair(s01, memo));

			// 设置颜色
			if(!setLight)
			{
				map<CString, Info*>::iterator iter01 = m_pPro->infoMap.find(st);
				if(iter01!=m_pPro->infoMap.end() && iter01->second->color!=-1)
					button->SetUpColor(iter01->second->color);
				else
				{
					if(m_pPro->isHolidayColor[(col+m_pPro->startXinqi-2)%7])
						button->SetUpColor(m_pPro->holidayColor);
					else
						button->SetUpColor(m_staticBlockColor);
				}
			}
			else
			{
				setLight = false;
				button->SetUpColor(m_currentBlockColor);
			}
		}
		else
		{
			st.Format(_T("%d %d %d"), currentDay, currentMonth, yearTmp);
			CRect c3((col-1)*(widthTmp+m_pPro->grap)+24, (row-1)*(widthTmp2+m_pPro->grap)+44, col*(widthTmp+m_pPro->grap)-m_pPro->grap+24, row*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);
			CMyButton* button = NewMyButton(IDC_MYBUTTON8+m_pPro->row+k, c3, 0);
			m_dayBlock.push_back(button);
			
			CString s01;
			Memo* memo = new Memo();
			s01.Format(_T("%d%d"), col, row);
			memo->button0 = button;
			m_pPro->memoMap->insert(make_pair(s01, memo));
			
			if(currentMonth==timeOfToday.GetMonth() && currentDay==timeOfToday.GetDay())
				setLight = true;

			// 设置颜色
			map<CString, Info*>::iterator iter01 = m_pPro->infoMap.find(st);
			if(!setLight)
			{
				if(iter01!=m_pPro->infoMap.end() && iter01->second->color!=-1)
					button->SetUpColor(iter01->second->color);
				else
				{
					if(m_pPro->isHolidayColor[(col+m_pPro->startXinqi-2)%7])
						button->SetUpColor(m_pPro->holidayColor);
					else
						button->SetUpColor(m_staticBlockColor);
				}
			}
			else
			{
				setLight = false;
				button->SetUpColor(m_currentBlockColor);
			}

			if(currentDay==g_days[currentMonth-1])
			{
				if(m_pPro->displayMode == 3)
					return;
				else
					nextLine = true;
			}
		}

		currentDay++;
		if(col == 7)
			row++;
		col = col % 7 + 1;


	}
	
}


// 动态添加我的按钮
CMyButton* CDesktopDlg::NewMyButton(int nID, CRect rect, int nStyle)
{
	CString m_Caption;
	m_Caption.LoadString(nID); //取按钮标题
	CMyButton *p_Button = new CMyButton();
	//ASSERT_VALID(p_Button);

	if(!p_Button->Create(m_Caption, WS_VISIBLE|BS_DEFPUSHBUTTON|WS_CHILD|BS_AUTORADIOBUTTON|BS_FLAT|nStyle, rect, this, nID))
		AfxMessageBox(_T("创建异常！"));
	return p_Button;
}

// 设置同步窗口参数，即获得父窗口指针
void CDesktopDlg::SetCalProperty(CalProperty* pPro)
{
	m_pPro = pPro;
}

// 读取备忘录信息
void CDesktopDlg::ReadInfo(int year)
{
	map<int, CFileOperation*>::iterator iter = m_pPro->fileOpMap.find(year);
	if(iter != m_pPro->fileOpMap.end())
		return;

	CFileOperation *fileOp = new CFileOperation();
	fileOp->PreRead(year, m_pPro);
	m_pPro->fileOpMap.insert(make_pair(year, fileOp));
}

// 释放资源
void CDesktopDlg::Release()
{
	for(int i=0; i<8; i++)
	{
		delete m_staticBlock[i];
		m_staticBlock[i] = NULL;
	}

	vector<CMyButton*>::iterator iter1 =  m_sideBlock.begin();
	for(; iter1!=m_sideBlock.end(); iter1++)
	{
		if(*iter1)
			delete (*iter1);
	}
	m_sideBlock.clear();

	vector<CMyButton*>::iterator iter2 = m_dayBlock.begin();
	for(; iter2!=m_dayBlock.end(); iter2++)
	{
		if(*iter2)
			delete (*iter2);
	}
	m_dayBlock.clear();
}

void CDesktopDlg::SetTaskBlock(void)
{
	m_staticBlockColor = m_pPro->commonColor;
	m_currentBlockColor = m_pPro->currentColor;
	int width = (m_pPro->rect.Width()-m_pPro->grap) / 2;
	int height = (m_pPro->rect.Height()-m_pPro->grap) / 2;
	int dHeight, dWidth;

	if(width/4 < 88)
		dWidth = 88;
	else
		dWidth = width / 4;

	if(height/4 < 32)
		dHeight = 32;
	else
		dHeight = height / 4;

	CRect c, c1;
	c.SetRect(0, 0, width, height);
	c1.SetRect(width-dWidth,  height-dHeight, width, height);
	CMyButton *button = NewMyButton(IDC_MYBUTTON8+1, c, 0);
	CMyButton *button1 = NewMyButton(IDC_MYBUTTON8+5, c1, 0);
	button->SetUpColor(m_staticBlockColor);
	button1->SetUpColor(m_pPro->holidayColor);
	m_sideBlock.push_back(button);
	m_sideBlock.push_back(button1);

	c.SetRect(width+m_pPro->grap, 0, width*2, height);
	c1.SetRect(width+m_pPro->grap,  height-dHeight, width+dWidth+m_pPro->grap, height);
	button = NewMyButton(IDC_MYBUTTON8+2, c, 0);
	button1 = NewMyButton(IDC_MYBUTTON8+5, c1, 0);
	button->SetUpColor(m_currentBlockColor);
	button1->SetUpColor(m_pPro->holidayColor);
	m_sideBlock.push_back(button);
	m_sideBlock.push_back(button1);

	c.SetRect(0, height+m_pPro->grap, width, height*2);
	c1.SetRect(width-dWidth,  height+m_pPro->grap, width, height+dHeight+m_pPro->grap);
	button = NewMyButton(IDC_MYBUTTON8+3, c, 0);
	button1 = NewMyButton(IDC_MYBUTTON8+5, c1, 0);
	button->SetUpColor(m_staticBlockColor);
	button1->SetUpColor(m_pPro->holidayColor);
	m_sideBlock.push_back(button);
	m_sideBlock.push_back(button1);

	c.SetRect(width+m_pPro->grap, height+m_pPro->grap, width*2, height*2);
	c1.SetRect(width+m_pPro->grap,  height+m_pPro->grap, width+dWidth+m_pPro->grap, height+dHeight+m_pPro->grap);
	button = NewMyButton(IDC_MYBUTTON8+4, c, 0);
	button1 = NewMyButton(IDC_MYBUTTON8+5, c1, 0);
	button->SetUpColor(m_staticBlockColor);
	button1->SetUpColor(m_pPro->holidayColor);
	m_sideBlock.push_back(button);
	m_sideBlock.push_back(button1);
}