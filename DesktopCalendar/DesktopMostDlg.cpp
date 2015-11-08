// DesktopMostDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DesktopDlg.h"
#include "DesktopMostDlg.h"
#include "ScreenCutDlg.h"
#include "TextInputDlg.h"
#include "MyStatic.h"
#include "afxdialogex.h"

/*调用C#库函数引用模块*/
#include <stdlib.h>
#include <vcclr.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Globalization;
using namespace System::Text;
using namespace System::Runtime::InteropServices;

#using <mscorlib.dll>
#using "CalLib.dll"
using namespace CalLib;
/*end module */


HHOOK g_hkb;
CalProperty* g_pPro;
bool g_lock = false;
bool g_openInfoWin = false;

bool IsCoveredByOtherWindow(HWND hWnd, CPoint point) 
{ 
    RECT rcTarget; 
    GetWindowRect(hWnd, &rcTarget); 
	bool isChild = (WS_CHILD == (GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)); 
	if ( GetDesktopWindow() == hWnd ) 
		 hWnd = GetWindow(GetTopWindow(hWnd), GW_HWNDLAST);

    do 
    { 
        HWND hCurWnd = hWnd; 
        while ( NULL != (hWnd = GetNextWindow(hWnd, GW_HWNDPREV) ) ) 
        { 
            if ( IsWindowVisible(hWnd) ) 
            { 
				RECT rcWnd; 
				GetWindowRect(hWnd, &rcWnd); 

                if ( (rcWnd.right > point.x) && (rcWnd.left < point.x) && 

                    (rcWnd.bottom > point.y) && (rcWnd.top < point.y)) 
					 return true; 
            } 
        } 

        if ( isChild ) 
        { 
            hWnd = GetParent(hCurWnd); 
            isChild = hWnd ? (WS_CHILD == (GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)) : false; 
        } 
        else 
		  break; 

    }while ( true ); 
  return false; 
}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{ 
	if(g_pPro->mode == 1)
	{
		CRect rect;
		CPoint point;
		static DWORD t1 = 0, t2 = 0;

		/*static bool isInfo = false;

		if(g_pPro->pDlg == GetForegroundWindow())
			g_lock = false;
		else
			g_lock = true;*/

		 if(g_lock && g_pPro->pTextDlg!=NULL && g_pPro->pTextDlg->IsClose())
		{
			g_pPro->pDlg->UpdateInfo();
			g_lock = false;
			delete g_pPro->pTextDlg;
			g_pPro->pTextDlg = NULL;
		
		}
	
		if(!g_lock)
		switch(wParam)  
		{
			case WM_LBUTTONDOWN:
									t2 = GetTickCount();
									if(t2 - t1 < 300)
									{
										GetCursorPos(&point);
										rect = g_pPro->rect; 
										if(!IsCoveredByOtherWindow(g_pPro->pDlg->m_hWnd, point) && rect.PtInRect(point) && g_pPro->pDlg->SetMemo(point)) //判断是否在本应用程序窗口内
										{
											g_lock = true;
										}
									
										return NULL;
									}
									else
										t1 = t2;

									g_pPro->pDlg->DisplayInfo(point, true);
									break;
			case WM_RBUTTONDOWN:
									if(g_openInfoWin)
									{
										GetCursorPos(&point);
										rect = g_pPro->rect;
										if(rect.PtInRect(point))
											g_pPro->pDlg->DisplayInfo(point);
									}
									break;

		 }
	}
    // LRESULT RetVal = CallNextHookEx(g_hkb, nCode, wParam, lParam );
     return false;
}
// CDesktopMostDlg 对话框

IMPLEMENT_DYNAMIC(CDesktopMostDlg, CDialogEx)

CDesktopMostDlg::CDesktopMostDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDesktopMostDlg::IDD, pParent)
{
	
}

CDesktopMostDlg::~CDesktopMostDlg()
{
	if(g_hkb)   
		UnhookWindowsHookEx(g_hkb);
	Release();
}

void CDesktopMostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDesktopMostDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_32809, &CDesktopMostDlg::OnOpenInfoWin)
END_MESSAGE_MAP()


// CDesktopMostDlg 消息处理程序


BOOL CDesktopMostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for(int i=0; i<8; i++)
		m_staticBlock[i] = NULL;

	if(!InstallHook())
		AfxMessageBox(_T("初始化失败"));

	BringWindowToTop();
	ModifyStyle(WS_CAPTION,0,0); // 去除标题栏
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT| WS_EX_LAYERED); 
	SetLayeredWindowAttributes(TRANSPARENT_COLOR, 255, 3);

	::SetWindowPos(m_hWnd, wndBottom, m_pPro->rect.left, m_pPro->rect.top, m_pPro->rect.Width(), m_pPro->rect.Height(), SWP_SHOWWINDOW);
	if(m_pPro->mode == 1)
		SetText();
	else if(m_pPro->mode == 3)
		SetTaskText();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CDesktopMostDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(nCtlColor == CTLCOLOR_DLG) 
	{ 
		pDC-> SetBkMode(TRANSPARENT);
		pDC->SetTextColor(COMMON_TEXT_COLOR);
		HBRUSH hBrush = CreateSolidBrush(TRANSPARENT_COLOR);
		return hBrush; 
	}
	else if(nCtlColor == CTLCOLOR_STATIC)
    { 
		pDC-> SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_pPro->fontColor);
		HBRUSH hBrush = CreateSolidBrush(TRANSPARENT_COLOR);
		return hBrush; 
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


// 设置同步窗口参数，即获得父窗口指针
void CDesktopMostDlg::SetCalProperty(CalProperty* pPro)
{
	m_pPro = pPro;
	g_pPro = pPro;
	m_copyMemoMap = m_pPro->memoMap;
	g_openInfoWin = m_pPro->isOpenInfoWin;
}


// 设置色块的文字
void CDesktopMostDlg::SetText(void)
{
	m_staticBlockColor = m_pPro->commonColor;
	CTime now = CTime::GetCurrentTime();

	int widthTmp = m_pPro->widthTmp;
	int widthTmp2 = m_pPro->widthTmp2;

	int currentMonth = m_pPro->startMonth;
	int currentDay = m_pPro->startDay;
	CString s=_T("");
	
	// 年月标题栏
	CString s0[7] = {_T("日"),_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六")};
	int year = m_pPro->time.GetYear();
    int month = m_pPro->time.GetMonth();
	int day = m_pPro->time.GetDay();
	
	int yearTmp = year;

	String ^sss = CalLib::CalLibClass::GetChineseDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 1);
	pin_ptr <const wchar_t> wch = PtrToStringChars(sss);
	CString lunarStr(wch);

	s.Format(_T("%d年%d月 / 今天是 %d年%d月%d日 星期%s %s"), year, month, now.GetYear(), now.GetMonth(), now.GetDay(), s0[now.GetDayOfWeek()-1], lunarStr);
	CRect c1(24, 0, m_pPro->rect.Width()-4, 20);
	if(m_staticBlock[0] != NULL)
		delete m_staticBlock[0];
	m_staticBlock[0] = NewMyStatic(IDC_MYSTATIC1,c1 , s, SS_CENTERIMAGE|SS_CENTER);

	// 星期标题栏
	for(int i=1, j=m_pPro->startXinqi; i<8; i++)
	{
		if(m_staticBlock[i] != NULL)
			delete m_staticBlock[i];

		CRect c2((i-1)*(widthTmp+m_pPro->grap)+24, 22, i*(widthTmp+m_pPro->grap)-m_pPro->grap+24, 42);
		m_staticBlock[i] =  NewMyStatic(IDC_MYSTATIC1+i, c2 , s0[j-1], SS_CENTERIMAGE|SS_CENTER);
		j = j % 7 + 1;
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

		vector<CStatic*>::iterator iter=m_sideBlock.begin();
		for(; iter!=m_sideBlock.end(); ++iter)
			delete *iter;
		m_sideBlock.clear();

		vector<pair<int, int> >::iterator iter1= m_pPro->posOfUnion.begin();
		while((*iter1).second<=0 && iter1+1!=m_pPro->posOfUnion.end())
			iter1++;
		/*while((*iter1).second==-1 && iter1+1!=m_pPro->posOfUnion.end())
			iter1++;
*/
		int tmp0;
		tmp -= (m_pPro->firstWeek-1);

		for(int j=1; j<=m_pPro->row; j++)
		{
			if(tmp <= 0)
				tmp0 = tmp+52;
			else
				tmp0 = tmp;
			
			if(j == (*iter1).second)
			{

				CRect c2;
				c2.SetRect(2, (j-1)*(widthTmp2+m_pPro->grap)+44, 22, (j+1)*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);

				s.Format(_T("%d"),tmp0);
				CStatic* static0 = NewMyStatic(IDC_MYSTATIC1+7+j, c2, s, SS_CENTERIMAGE|SS_CENTER);
				m_sideBlock.push_back(static0);
				if(iter1+1 != m_pPro->posOfUnion.end())
					++iter1;
				j++;
				tmp++;
			}
			else
			{
				CRect c3(2, (j-1)*(widthTmp2+m_pPro->grap)+44, 22, j*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);
				s.Format(_T("%d"), tmp0);
				tmp++;
				CStatic* static1 = NewMyStatic(IDC_MYSTATIC1+7+j, c3, s, SS_CENTERIMAGE|SS_CENTER);
				m_sideBlock.push_back(static1);
			}
		}
	}
	
	// 日期色块
	bool nextLine = false; // 默认无换行
	vector<CStatic*>::iterator iter2=m_dayBlock.begin();
	for(; iter2!=m_dayBlock.end(); ++iter2)
		delete *iter2;
	m_dayBlock.clear();
	vector<pair<int,int> >::iterator iter3 = m_pPro->posOfUnion.begin();
	map<CString, Info*>::iterator iter4;
	CString strTmp;

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

	//// 根据当前年份读取年表记录
	//ReadInfo(yearTmp);

	if(m_pPro->startDay == 0)
	{
		nextLine = true;
		currentMonth--;
	}

	for(int k=1; k<=7*m_pPro->row && row<=m_pPro->row; k++)
	{
		if(nextLine)
		{
			if(iter3+1 != m_pPro->posOfUnion.end())
				++iter3;
			
			nextLine = false;

			if(currentMonth == 12)
			{
				currentMonth = 0;
				yearTmp++;
				/*ReadInfo(yearTmp);*/

				if((yearTmp%400==0 || yearTmp%100!=0) && yearTmp%4==0)
					g_days[1] = 29;
				else
					g_days[1] = 28;
			}
			s.Format(_T("%d月"), ++currentMonth);

			if(m_pPro->displayMode != 3)
			{
				CRect c30((col-1)*(widthTmp+m_pPro->grap)+24, (row-1)*(widthTmp2+m_pPro->grap)+44+widthTmp2/2, col*(widthTmp+m_pPro->grap)-m_pPro->grap+24, row*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);
				CStatic* static00 = NewMyStatic(IDC_MYSTATIC1+7+m_pPro->row+k, c30, s, SS_CENTERIMAGE|SS_CENTER);
				m_dayBlock.push_back(static00);
			}
			k += 6;
			row++;
			if(row > m_pPro->row)
				break;
			currentDay = 1;

			strTmp.Format(_T("%d %d %d"), currentDay, currentMonth, year);
			iter4 = m_pPro->infoMap.find(strTmp);

			s.Format(_T("%d"), currentDay);
			if(iter4 != m_pPro->infoMap.end())
				s += (_T("\n") + iter4->second->text);
			
			CRect c31((col-1)*(widthTmp+m_pPro->grap)+24, (row-1)*(widthTmp2+m_pPro->grap)+44, col*(widthTmp+m_pPro->grap)-m_pPro->grap+24, row*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);
			CStatic* static0 = NewMyStatic(IDC_MYSTATIC1+7+m_pPro->row+k, c31, s);
		    AddExtralDisplay(static0, yearTmp, currentMonth, currentDay);

			if(m_pPro->isSetFont)
				static0->SetFont(m_pPro->font);
			m_dayBlock.push_back(static0);

			AddMemo(col, row, static0, yearTmp, currentMonth, currentDay);
			currentDay++;
		}
		else
		{	
			strTmp.Format(_T("%d %d %d"), currentDay, currentMonth, year);
			iter4 = m_pPro->infoMap.find(strTmp);
		
			s.Format(_T("%d"), currentDay);
			if(iter4 != m_pPro->infoMap.end())
				s += (_T("\n") + iter4->second->text);

			CRect c3((col-1)*(widthTmp+m_pPro->grap)+24, (row-1)*(widthTmp2+m_pPro->grap)+44, col*(widthTmp+m_pPro->grap)-m_pPro->grap+24, row*(widthTmp2+m_pPro->grap)-m_pPro->grap+44);
			CStatic* static0 = NewMyStatic(IDC_MYSTATIC1+7+m_pPro->row+k, c3, s);
		    AddExtralDisplay(static0, yearTmp, currentMonth, currentDay);

			if(m_pPro->isSetFont)
				static0->SetFont(m_pPro->font);
			m_dayBlock.push_back(static0);

			AddMemo(col, row, static0, yearTmp, currentMonth, currentDay);

			if(currentDay == g_days[currentMonth-1])
			{
				if(m_pPro->displayMode == 3)
					return;
				else
					nextLine = true;
			}
			currentDay++;

		}
		if(col == 7)
			row++;
		col = col % 7 + 1;


	}
}

void CDesktopMostDlg::AddMemo(int x, int y, CStatic* static0, int year, int month, int day)
{
	CString s;
	s.Format(_T("%d%d"), x, y);

	map<CString, Memo*>::iterator iter = m_copyMemoMap->find(s);
	if(iter != m_copyMemoMap->end())
	{
		Memo *memo = iter->second;
		memo->static0 = static0;
		memo->year = year;
		memo->month = month;
		memo->day = day;
		memo->dayOfWeek = m_pPro->startXinqi+x-1;
	}
}

// 生成输入框并设置备忘录内容
bool CDesktopMostDlg::SetMemo(const CPoint &point)
{
	int x = (point.x-m_pPro->rect.left-24)/(m_pPro->widthTmp+m_pPro->grap)+1;
	int y = (point.y-m_pPro->rect.top-44)/(m_pPro->widthTmp2+m_pPro->grap)+1;

	CString s;
	s.Format(_T("%d%d"), x, y);

	map<CString, Memo*>::iterator iter = m_copyMemoMap->find(s);

	if(iter != m_copyMemoMap->end())
	{
		CString s0;	
		s0.Format(_T("%d %d %d"), iter->second->day, iter->second->month, iter->second->year);
		m_pPro->strCode = s0; // 存储字符串时间码
		m_pPro->xiqiCode = iter->second->dayOfWeek;
		m_pPro->pStatic = iter->second->static0; // 存储待更新的静态文本框指针
		m_pPro->pButton = iter->second->button0;

		s.Format(_T("%d年%d月%d日"), iter->second->year, iter->second->month, iter->second->day);	
		CTextInputDlg *dlg = new CTextInputDlg(m_pPro, s);

		dlg->Create(IDD_TEXTDIALOG);
		CRect rect;
		dlg->GetWindowRect(&rect);

		CPoint temPoint = point;
		ResetDisplayPoint(temPoint, rect);
		dlg->SetWindowPos(NULL, temPoint.x, temPoint.y, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		
		m_pPro->pTextDlg = dlg;
		return true;
	}

	return false;
}

// 添加静态文本框和文字
CStatic* CDesktopMostDlg::NewMyStatic(UINT nID, const CRect& rect,  LPCTSTR lpszText, DWORD dwStyle)
{
	CStatic* cs = new CStatic();
	if(!cs->Create(lpszText, WS_CHILD|WS_VISIBLE|dwStyle, rect, this, nID))
		AfxMessageBox(_T("异常！"));
	return cs;
}

bool CDesktopMostDlg::InstallHook()
{
	//HWND hDesktop = ::FindWindow(_T("SysListView32"), NULL);
	g_hkb = SetWindowsHookEx(WH_MOUSE_LL,&MouseProc,NULL,NULL);
    return TRUE;
}

	
// 更新备忘录信息和显示信息
void CDesktopMostDlg::UpdateInfo()
{
	// 将修改的部分写到硬盘中去
	int year, month, day;
	CString text;
	COLORREF color;
	int tmpIndex;
	int index = m_pPro->strCode.Find(_T(" "), 0);
	day =  _ttoi(m_pPro->strCode.Mid(0, index));
	tmpIndex = index;
	index = m_pPro->strCode.Find(_T(" "), index+1);
	month = _ttoi(m_pPro->strCode.Mid(tmpIndex+1, index-tmpIndex-1));
	year = _ttoi(m_pPro->strCode.Mid(index+1, 4));

	map<CString, Info*>::iterator iter1 = m_pPro->infoMap.find(m_pPro->strCode);
	if(iter1 != m_pPro->infoMap.end())
	{
		if(iter1->second->color!= -1)
			color = iter1->second->color;
		else
			color = -1;
		text = iter1->second->text;
	}
	else
	{
		color = -1;
		text = _T("");
	}
	map<int, CFileOperation*>::iterator iter = m_pPro->fileOpMap.find(year);
	if(iter != m_pPro->fileOpMap.end())
	{
		iter->second->AddMessage(month, day, false, true, _T(""), _T(""), text, color);
		iter->second->PreRead(year, m_pPro);
	}
	// 更新显示信息
	// 在TextDlg.cpp OK()处完成
}

// 释放资源
void CDesktopMostDlg::Release()
{
	for(int i=0; i<8; i++)
	{
		if(m_staticBlock[i])
		{
			delete m_staticBlock[i];
			m_staticBlock[i] = NULL;
		}
	}

	vector<CStatic*>::iterator iter1 =  m_sideBlock.begin();
	for(; iter1!=m_sideBlock.end(); iter1++)
	{
		if(*iter1)
			delete (*iter1);
	}
	m_sideBlock.clear();

	vector<CStatic*>::iterator iter2 = m_dayBlock.begin();
	for(; iter2!=m_dayBlock.end(); iter2++)
	{
		if(*iter2)
			delete (*iter2);
	}
	m_dayBlock.clear();

}

void CDesktopMostDlg::AddExtralDisplay(CStatic* static0, int year, int month, int day)
{
	CString origStr = _T(""), addStr = _T("");
	static0->GetWindowText(origStr);
	int index = origStr.Find('\n');

	if(year<0 || month<0 || day<0)
		return;

	if(m_pPro->isDisplayMore[0]) // 节日
	{
		CalLib::ChineseCalendar chiCal(year, month, day);
		String ^sss = chiCal.DateHoliday;
		pin_ptr <const wchar_t> wch = PtrToStringChars(sss);
		CString s(wch);
		addStr += (_T(" ")+s);
	}
	if(m_pPro->isDisplayMore[1]) // 农历
	{
		String ^sss = CalLib::CalLibClass::GetChineseDateTime(year, month, day, 2);
		pin_ptr <const wchar_t> wch = PtrToStringChars(sss);
		CString s(wch);
		addStr += (_T(" ")+s);
	}
	if(m_pPro->isDisplayMore[2]) // 二十四节气
	{
		String ^sss = CalLib::CalLibClass::GetChineseDateTime(year, month, day, 3);
		pin_ptr <const wchar_t> wch = PtrToStringChars(sss);
		CString s(wch);
		addStr += (_T(" ")+s);
	}
	
	if(index != -1)
		origStr.Insert(index, addStr);
	else
		origStr.Append(addStr);

	static0->SetWindowText(origStr);
}

void CDesktopMostDlg::DisplayInfo(const CPoint &point, bool close)
{		
	static int x = 0;
	static int y = 0;
	static CStatic *s_static0 = NULL;
	static CScreenCutDlg *s_infoDlg = NULL;

	if(!close)
	{
		int tx = point.x-m_pPro->rect.left-24;
		int ty = point.y-m_pPro->rect.top-44;

		if(tx<0 || ty<0)
		{
			if(s_static0)
			{
				delete s_static0;
				s_static0 = NULL;
			}
			delete s_infoDlg;
			s_infoDlg = NULL;
			return;
		}
		tx = tx/(m_pPro->widthTmp+m_pPro->grap)+1;
		ty = ty/(m_pPro->widthTmp2+m_pPro->grap)+1;
	
		if(tx!=x || ty!=y)
		{
			x = tx;
			y = ty;
			CString s;
			s.Format(_T("%d%d"), x, y);

			map<CString, Memo*>::iterator iter = m_copyMemoMap->find(s);

			if(iter != m_copyMemoMap->end())
			{
				if(g_openInfoWin)
				{
					CalLib::ChineseCalendar chiCal(iter->second->year, iter->second->month, iter->second->day);
					String ^sss = "   阳历：" +chiCal.DateString + "\n";
					sss += ("   属相：" +chiCal.AnimalString+"\n");
					sss += ("   农历：" +chiCal.ChineseDateString+"\n");
					if(chiCal.ChineseTwentyFourDay != "")
						sss += ("   节气：" +chiCal.ChineseTwentyFourDay+"\n");
					if(chiCal.DateHoliday != "")
						sss += ("   节日：" +chiCal.DateHoliday+"\n");
					sss += ("   前一个节气：" +chiCal.ChineseTwentyFourPrevDay+"\n");
					sss += ("   后一个节气：" +chiCal.ChineseTwentyFourNextDay+"\n");
					sss += ("   干支：" +chiCal.GanZhiDateString+"\n");
					sss += ("   星期：" +chiCal.WeekDayStr+"\n");
					sss += ("   星宿：" +chiCal.ChineseConstellation+"\n");
					sss += ("   星座：" +chiCal.Constellation+"\n");

					pin_ptr <const wchar_t> wch = PtrToStringChars(sss);
					CString s(wch);

					CRect rect(CPoint(x,y), CPoint(x+270, y+250));

					
					if(s_static0)
					{
						delete s_static0;
						s_static0 = NULL;
					}
					if(s_infoDlg)
					{
						delete s_infoDlg;
						s_infoDlg = NULL;
					}

					
					if(IsCoveredByOtherWindow(g_pPro->pDlg->m_hWnd, point))
						return;

					s_static0 = new CStatic();
					s_infoDlg = new CScreenCutDlg(); 
					s_infoDlg->Create(IDD_DESKTOP_DIALOG);

					CPoint temPoint = point;

					ResetDisplayPoint(temPoint, rect);
					s_infoDlg->SetWindowPos(NULL, temPoint.x, temPoint.y, rect.Width(), rect.Height(), SWP_SHOWWINDOW);

					 //m_pParentWnd = s_infoDlg->GetParent();
					 //// 改变窗体风格, 移除WS_CHILDWINDOW风格，添加WS_POPUP
					 //LONG lNewStyle = GetWindowLong(s_infoDlg->m_hWnd, GWL_STYLE);
					 //lNewStyle &= ~WS_CHILDWINDOW;
					 //lNewStyle |= WS_POPUP;
					 //SetWindowLong(s_infoDlg->m_hWnd, GWL_EXSTYLE, lNewStyle|WS_EX_TOOLWINDOW);
					 //// 设置父窗口为NULL
					 //s_infoDlg->SetParent(NULL);

					//SetWindowLong(s_infoDlg->m_hWnd, GWL_EXSTYLE,  WS_POPUP);   
					s_static0->Create(s, WS_CHILD|WS_VISIBLE|SS_RIGHT, rect, s_infoDlg, 10000);
					s_infoDlg->ShowWindow(true);
				}
			}
			else if(s_infoDlg)
			{
				if(s_static0)
				{
					delete s_static0;
					s_static0 = NULL;
				}
				delete s_infoDlg;
				s_infoDlg = NULL;
			}
		}
	}
	else
	{
		x = y = 0;
		if(s_static0)
		{
			delete s_static0;
			s_static0 = NULL;
		}
		delete s_infoDlg;
		s_infoDlg = NULL;
	}
}


void CDesktopMostDlg::ResetDisplayPoint(CPoint &point, CRect &rect)
{
	if(point.x + rect.Width() > GetSystemMetrics(SM_CXSCREEN))
		point.Offset(10-rect.Width(), 0);

	if(point.y + rect.Height() > GetSystemMetrics(SM_CYSCREEN))
		point.Offset(0, -rect.Height());

}

void CDesktopMostDlg::OnOpenInfoWin()
{
	if(!g_openInfoWin)
		g_openInfoWin = true;
	else
		g_openInfoWin = false;

	m_pPro->isOpenInfoWin = g_openInfoWin;
}

void CDesktopMostDlg::SetTaskText()
{
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

	CRect c, c1, c2;
	CString s;
	c.SetRect(0, 0, width-dWidth, height);
	c1.SetRect(width-dWidth,  height-dHeight, width, height-dHeight/2);
	c2.SetRect(width-dWidth,  height-dHeight/2, width, height);
	s = m_pPro->task[2];
	s.Replace(_T("\r\n"), _T("\t"));
	CStatic* static0 = NewMyStatic(IDC_MYSTATIC1+1, c, s, SS_LEFT);
	CStatic* static1 = NewMyStatic(IDC_MYSTATIC1+5, c1, _T("【第三优先】"), SS_CENTERIMAGE|SS_CENTER);
	CStatic* static2 = NewMyStatic(IDC_MYSTATIC1+9, c2, _T("不重要紧急"), SS_CENTERIMAGE|SS_CENTER);
	m_sideBlock.push_back(static0);
	m_sideBlock.push_back(static1);

	c.SetRect(width+dWidth+m_pPro->grap, 0, width*2+m_pPro->grap, height);
	c1.SetRect(width+m_pPro->grap,  height-dHeight, width+dWidth+m_pPro->grap, height-dHeight/2);
	c2.SetRect(width+m_pPro->grap,  height-dHeight/2, width+dWidth+m_pPro->grap, height);
	s = m_pPro->task[0];
	s.Replace(_T("\r\n"), _T("\t"));
	static0 = NewMyStatic(IDC_MYSTATIC1+2, c, s, SS_LEFT);
	static1 = NewMyStatic(IDC_MYSTATIC1+6, c1, _T("【第一优先】"), SS_CENTERIMAGE|SS_CENTER);
	static2 = NewMyStatic(IDC_MYSTATIC1+10, c2, _T("重要紧急"), SS_CENTERIMAGE|SS_CENTER);
	m_sideBlock.push_back(static0);
	m_sideBlock.push_back(static1);

	c.SetRect(0, height+m_pPro->grap, width-dWidth, height*2+m_pPro->grap);
	c1.SetRect(width-dWidth,  height+m_pPro->grap, width, height+dHeight/2+m_pPro->grap);
	c2.SetRect(width-dWidth,  height+dHeight/2+m_pPro->grap, width, height+dHeight+m_pPro->grap);
	s = m_pPro->task[3];
	s.Replace(_T("\r\n"), _T("\t"));
	static0 = NewMyStatic(IDC_MYSTATIC1+3, c, s, SS_LEFT);
	static1 = NewMyStatic(IDC_MYSTATIC1+7, c1, _T("不重要不紧急"), SS_CENTERIMAGE|SS_CENTER);
	static2 = NewMyStatic(IDC_MYSTATIC1+11, c2, _T("【第四优先】"), SS_CENTERIMAGE|SS_CENTER);
	m_sideBlock.push_back(static0);
	m_sideBlock.push_back(static1);

	c.SetRect(width+dWidth+m_pPro->grap, height+m_pPro->grap, width*2+m_pPro->grap, height*2+m_pPro->grap);
	c1.SetRect(width+m_pPro->grap,  height+m_pPro->grap, width+dWidth+m_pPro->grap, height+dHeight/2+m_pPro->grap);
	c2.SetRect(width+m_pPro->grap,  height+dHeight/2+m_pPro->grap, width+dWidth+m_pPro->grap, height+dHeight+m_pPro->grap);
	s= m_pPro->task[1];
	s.Replace(_T("\r\n"), _T("\t"));
	static0 = NewMyStatic(IDC_MYSTATIC1+4, c, s, SS_LEFT);
	static1 = NewMyStatic(IDC_MYSTATIC1+8, c1, _T("重要不紧急"), SS_CENTERIMAGE|SS_CENTER);
	static2 = NewMyStatic(IDC_MYSTATIC1+12, c2, _T("【第二优先】"), SS_CENTERIMAGE|SS_CENTER);
	m_sideBlock.push_back(static0);
	m_sideBlock.push_back(static1);
}