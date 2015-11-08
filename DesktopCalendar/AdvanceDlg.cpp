// AdvanceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "AdvanceDlg.h"
#include "TextInputDlg.h"
#include "afxdialogex.h"
#include "windows.h"
#include <fstream>
#include "Mmsystem.h"
#include "Digitalv.h"
#include "afxwin.h"
#include "FindMusic.h"

using namespace std;

#pragma comment( lib, "winmm.lib" )

CString g_CurrentMusic;
bool g_IsPlay;
bool g_ChangeMusic;
HANDLE g_pNewThread;

// 音乐循环函数

static DWORD WINAPI Loop(void* app0)
{ 
	CString music;
	int mark = 0;
	//use   mciSendCommand   
	MCI_OPEN_PARMS   mciOpen;   
	MCIERROR   mciError;   
	UINT   DeviceID;
	while(true)
	{
		if(g_ChangeMusic)
		{
			g_ChangeMusic = false;
			mciSendCommand (DeviceID, MCI_CLOSE, NULL, NULL); 
			g_IsPlay = true;
			mark = 0;
		}

		if(g_IsPlay)
		{
			if(mark == 0)
			{
				mark = 1;
				music = CString(g_CurrentMusic);
				music.Delete(music.GetLength()-1);
				mciOpen.lpstrElementName   =   music; 
				mciOpen.lpstrDeviceType   =   _T("mpegvideo");   
				mciError   =   mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE   |   MCI_OPEN_ELEMENT,(DWORD)&mciOpen);   
				DeviceID =   mciOpen.wDeviceID   ;
			
			    
				MCI_PLAY_PARMS   mciPlay;   

				mciError   =   mciSendCommand(DeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, 
					(DWORD)   (LPMCI_PLAY_PARMS)&mciPlay);  //MCI_DGV_PLAY_REPEAT, 要 #include "Digitalv.h"
			
			}
			else if(mark == 2)
				mark = 0;
		}
		else if(!g_IsPlay)
		{
			if(mark==1)
			{
				mark = 2;
				mciSendCommand (DeviceID, MCI_CLOSE, NULL, NULL); 
			}
		}
		
		SuspendThread(g_pNewThread);
	}
    return 0;
}

//使能关机特权函数
BOOL EnableShutdownPrivilege()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = {0};
	TOKEN_PRIVILEGES stToken_Privileges = {0};

	hProcess = ::GetCurrentProcess(); //获取当前应用程序进程句柄

	if(!::OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken)) //打开当前进程的访问令牌句柄(OpenProcessToken函数调用失败返回值为零)
		return FALSE;

	if(!::LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&uID)) //获取权限名称为"SeShutdownPrivilege"的LUID(LookupPrivilegeValue函数调用失败返回值为零)
		return FALSE;

	stToken_Privileges.PrivilegeCount = 1; //欲调整的权限个数
	stToken_Privileges.Privileges[0].Luid = uID; //权限的LUID
	stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; //权限的属性,SE_PRIVILEGE_ENABLED为使能该权限

	if(!::AdjustTokenPrivileges(hToken,FALSE,&stToken_Privileges,sizeof stToken_Privileges,NULL,NULL)) //调整访问令牌里的指定权限(AdjustTokenPrivileges函数调用失败返回值为零)
		return FALSE;

	if(::GetLastError() != ERROR_SUCCESS) //查看权限是否调整成功
		return FALSE;

	::CloseHandle(hToken);
	return TRUE;
}

//关机函数
BOOL Shutdown(BOOL bForce)
{
	EnableShutdownPrivilege(); //使能关机特权函数
	if(bForce)
		return ::ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,0); //强制关机
	else
		return ::ExitWindowsEx(EWX_SHUTDOWN,0);
}

//注销函数
BOOL Logoff(BOOL bForce)
{
	if(bForce)
		return ::ExitWindowsEx(EWX_LOGOFF | EWX_FORCE,0); //强制注销
	else
		return ::ExitWindowsEx(EWX_LOGOFF,0);
}

//重启函数
BOOL Reboot(BOOL bForce)
{
	EnableShutdownPrivilege(); //使能关机特权函数
	if(bForce)
		return ::ExitWindowsEx(EWX_REBOOT | EWX_FORCE,0); //强制重启
	else
		return ::ExitWindowsEx(EWX_REBOOT,0);
}


// 闹钟
void Clock(CString content, CString music)
{
	g_IsPlay = true;
	g_CurrentMusic = music;

	MessageBox(NULL, content, _T("自定义闹钟"), MB_OK);
	g_IsPlay = false;
}

// 运行自定义程序
void RunProgram(CString program)
{
	for(int i=0; i<program.GetLength(); i++)
	{
		if(program[i] == '\\')
		{
			program.Insert(i, '\\');
			i++;
		}
	}
	USES_CONVERSION;
	LPCSTR lpstr=(LPCSTR)T2A(program); 
	WinExec(lpstr, SW_NORMAL);
}

// CAdvanceDlg 对话框

IMPLEMENT_DYNAMIC(CAdvanceDlg, CDialog)

CAdvanceDlg::CAdvanceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvanceDlg::IDD, pParent)
	, m_monthOrHour(0)
	, m_dayOrMinute(0)
	, m_hour(0)
	, m_minute(0)
	, m_content(_T(""))
	, m_caption(_T(""))
	, m_pathOfProgram(_T(""))
{

}

CAdvanceDlg::~CAdvanceDlg()
{
}

void CAdvanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_monthOrHour);
	DDX_Text(pDX, IDC_EDIT2, m_dayOrMinute);
	DDX_Text(pDX, IDC_EDIT3, m_hour);
	DDX_Text(pDX, IDC_EDIT4, m_minute);
	DDX_Text(pDX, IDC_EDIT7, m_content);
	DDX_Text(pDX, IDC_EDIT5, m_caption);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_pathOfProgram);
}

BOOL CAdvanceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT| WS_EX_LAYERED); 
	SetLayeredWindowAttributes(RGB(255,255,255), 255, 3);

	SetTime();

	((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(true);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	GetDlgItem(IDC_STATIC4)->ShowWindow(false);
	GetDlgItem(IDC_STATIC9)->ShowWindow(false);
	GetDlgItem(IDC_EDIT7)->ShowWindow(false);
	GetDlgItem(IDC_EDIT5)->ShowWindow(false);
	GetDlgItem(IDC_CHECK1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK2)->ShowWindow(false);
	((CComboBox*)GetDlgItem(IDC_COMBO8))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO9))->SetCurSel(0);
	GetDlgItem(IDC_COMBO9)->ShowWindow(false);
	GetDlgItem(IDC_MFCEDITBROWSE1)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(false);

	// 音乐初始化	
	(new CFindMusic())->searchAllFile("Music", 0, "temp.txt");
	CComboBox* chooseBox = (CComboBox*)GetDlgItem(IDC_COMBO9);
	
	CString str = _T("");
	int num;
	char s[100];
	FILE* fp = NULL;
	fp = fopen("temp.txt", "r");
	ifstream read("temp.txt", ios::in);
	if(read!=NULL && fp!=NULL)
	{
		read>>num;
		fgets(s, 100, fp);

		for(int i=0; i<num; i++)
		{
			str.Format(_T("%s"), CString(fgets(s, 100, fp)));
			chooseBox->AddString(str);
		}
	
		fclose(fp);
	}

	chooseBox->SetCurSel(0);
	int index = chooseBox->GetCurSel();
	if(index >= 0)
		chooseBox->GetLBText(index, g_CurrentMusic);
	
	g_ChangeMusic = false;
	g_IsPlay = false;

	g_pNewThread = CreateThread(NULL, 0, Loop, this, CREATE_SUSPENDED, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BEGIN_MESSAGE_MAP(CAdvanceDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO7, &CAdvanceDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CAdvanceDlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_CHECK1, &CAdvanceDlg::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAdvanceDlg::OnSetRunTime)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CAdvanceDlg::OnSetTask)
	ON_BN_CLICKED(IDC_BUTTON1, &CAdvanceDlg::SubmitTask)
	ON_BN_CLICKED(IDC_BUTTON8, &CAdvanceDlg::DeleteTask)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAdvanceDlg::OnTip)
	ON_BN_CLICKED(IDC_CHECK2, &CAdvanceDlg::OnTryMusic)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CAdvanceDlg::OnMusicList)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAdvanceDlg 消息处理程序
// 设置时间
void CAdvanceDlg::SetTime()
{
	CTime time = CTime::GetCurrentTime();
	int year = time.GetYear();
	int month = time.GetMonth();
	int day = time.GetDay();
	CString tmpStr;
	tmpStr.Format(_T("%d年"), year);
	GetDlgItem(IDC_STATIC2)->SetWindowText(tmpStr);
	tmpStr.Format(_T("%d"), month);
	GetDlgItem(IDC_EDIT1)->SetWindowText(tmpStr);
	tmpStr.Format(_T("%d"), day);
	GetDlgItem(IDC_EDIT2)->SetWindowText(tmpStr);
}

// 添加任务
void CAdvanceDlg::OnBnClickedRadio7()
{
	SetTime();

	CComboBox* combo8 = (CComboBox*)GetDlgItem(IDC_COMBO8);
	combo8->ResetContent();
	combo8->AddString(_T("注销"));
	combo8->AddString(_T("关机"));
	combo8->AddString(_T("重启"));
	combo8->AddString(_T("闹钟"));
	combo8->AddString(_T("自定义程序"));

	GetDlgItem(IDC_STATIC1)->SetWindowText(_T("添加任务"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	GetDlgItem(IDC_STATIC4)->ShowWindow(false);
	GetDlgItem(IDC_EDIT7)->ShowWindow(false);
	GetDlgItem(IDC_CHECK1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK2)->ShowWindow(false);
	((CComboBox*)GetDlgItem(IDC_COMBO8))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO9))->SetCurSel(0);
	GetDlgItem(IDC_COMBO9)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(false);
	GetDlgItem(IDC_STATIC9)->ShowWindow(false);
	GetDlgItem(IDC_EDIT5)->ShowWindow(false);
	GetDlgItem(IDC_MFCEDITBROWSE1)->ShowWindow(false);
	GetDlgItem(IDC_MFCEDITBROWSE1)->SetWindowText(_T(""));
	
	
	OnSetRunTime();
	OnSetTask();

	m_caption = _T("");
	m_content = _T("");
	UpdateData(false);
}

// 修改任务
void CAdvanceDlg::OnBnClickedRadio8()
{
	GetDlgItem(IDC_STATIC1)->SetWindowText(_T("修改任务"));
	GetDlgItem(IDC_BUTTON8)->EnableWindow(true);	
	GetDlgItem(IDC_STATIC4)->ShowWindow(false);
	GetDlgItem(IDC_EDIT7)->ShowWindow(false);
	GetDlgItem(IDC_CHECK1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK2)->ShowWindow(false);
	GetDlgItem(IDC_COMBO9)->ShowWindow(false);
	GetDlgItem(IDC_STATIC9)->ShowWindow(false);
	GetDlgItem(IDC_EDIT5)->ShowWindow(false);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(true);
	GetDlgItem(IDC_MFCEDITBROWSE1)->ShowWindow(false);

	CComboBox* combo8 = (CComboBox*)GetDlgItem(IDC_COMBO8);
	combo8->ResetContent();
	for(map<taskInfo*, CWinThread*>::iterator iter=m_taskInfoMap.begin(); iter!=m_taskInfoMap.end(); ++iter)
	{
		combo8->AddString(iter->first->name);
	}
	combo8->SetCurSel(0);

	OnSetRunTime();
	OnSetTask();
}




void CAdvanceDlg::OnBnClickedCheck1()
{
	if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		GetDlgItem(IDC_CHECK2)->ShowWindow(true);
		GetDlgItem(IDC_COMBO9)->ShowWindow(true);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(false);
		GetDlgItem(IDC_COMBO9)->ShowWindow(false);
		GetDlgItem(IDC_CHECK2)->ShowWindow(false);
		OnTryMusic();
	}
}


void CAdvanceDlg::OnSetRunTime()
{
	UpdateData(true);
	int index = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	
	switch(index)
	{
	case 0: 
		SetTime();
		GetDlgItem(IDC_STATIC2)->ShowWindow(true);
		GetDlgItem(IDC_STATIC3)->ShowWindow(true);
		GetDlgItem(IDC_STATIC7)->ShowWindow(true);
		GetDlgItem(IDC_EDIT3)->ShowWindow(true);
		GetDlgItem(IDC_EDIT4)->ShowWindow(true);
		GetDlgItem(IDC_STATIC5)->SetWindowText(_T("月"));
		GetDlgItem(IDC_STATIC6)->SetWindowText(_T("日"));

		break;
	case 1:
	case 2:
		m_monthOrHour = 0;
		m_dayOrMinute = 0;
		GetDlgItem(IDC_STATIC2)->ShowWindow(false);
		GetDlgItem(IDC_STATIC3)->ShowWindow(false);
		GetDlgItem(IDC_STATIC7)->ShowWindow(false);
		GetDlgItem(IDC_EDIT3)->ShowWindow(false);
		GetDlgItem(IDC_EDIT4)->ShowWindow(false);
		GetDlgItem(IDC_STATIC5)->SetWindowText(_T("小时"));
		GetDlgItem(IDC_STATIC6)->SetWindowText(_T("分"));
		break;
	default: break;
	}
	UpdateData(false);
}


void CAdvanceDlg::OnSetTask()
{
	UpdateData(true);
	taskInfo* info = NULL;

	if(((CButton*)GetDlgItem(IDC_RADIO8))->GetCheck())
	{
		CString name = _T("");
		GetDlgItem(IDC_COMBO8)->GetWindowTextW(name);
		for(map<taskInfo*, CWinThread*>::iterator iter=m_taskInfoMap.begin(); iter!=m_taskInfoMap.end(); ++iter)
		{
			if(iter->first->name == name)
				info = iter->first;
		}
	}
	int index = ((CComboBox*)GetDlgItem(IDC_COMBO8))->GetCurSel();
	CString content0 = _T("");
	if(index >= 0)
		((CComboBox*)GetDlgItem(IDC_COMBO8))->GetLBText(index, content0);
	if(content0!=_T("注销") && content0!=_T("关机") && content0!=_T("重启"))
	{
		GetDlgItem(IDC_STATIC4)->ShowWindow(true);
		GetDlgItem(IDC_STATIC9)->ShowWindow(true);
		GetDlgItem(IDC_EDIT5)->ShowWindow(true);
		
		
		if(content0.Left(5) != _T("自定义程序"))
		{
			GetDlgItem(IDC_STATIC4)->SetWindowTextW(_T("自定义提醒内容"));
			GetDlgItem(IDC_EDIT7)->ShowWindow(true);
			GetDlgItem(IDC_CHECK1)->ShowWindow(true);
			GetDlgItem(IDC_MFCEDITBROWSE1)->ShowWindow(false);	
			
			if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
			{
				GetDlgItem(IDC_CHECK2)->ShowWindow(true);
				GetDlgItem(IDC_COMBO9)->ShowWindow(true);
			}
		}
		else
		{
			GetDlgItem(IDC_STATIC4)->SetWindowTextW(_T("程序地址"));
			GetDlgItem(IDC_EDIT7)->ShowWindow(false);
			GetDlgItem(IDC_MFCEDITBROWSE1)->ShowWindow(true);			
			GetDlgItem(IDC_CHECK1)->ShowWindow(false);
			GetDlgItem(IDC_CHECK2)->ShowWindow(false);
			GetDlgItem(IDC_COMBO9)->ShowWindow(false);
		}


		if(info != NULL)
		{
			if(content0.Left(5) != _T("自定义程序"))
			{
				m_caption = info->name;
				m_content = info->content;
			}
			else
			{
				m_caption = info->name.Mid(5);
				m_pathOfProgram = info->content;
			}

			
			if(info->music != _T(""))
			{
				((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(true);
				GetDlgItem(IDC_CHECK2)->ShowWindow(true);
				((CComboBox*)GetDlgItem(IDC_COMBO9))->SetWindowText(info->music); 
			}
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC4)->ShowWindow(false);
		GetDlgItem(IDC_EDIT7)->ShowWindow(false);
		GetDlgItem(IDC_CHECK1)->ShowWindow(false);
		GetDlgItem(IDC_CHECK2)->ShowWindow(false);
		GetDlgItem(IDC_COMBO9)->ShowWindow(false);
		GetDlgItem(IDC_STATIC9)->ShowWindow(false);
		GetDlgItem(IDC_EDIT5)->ShowWindow(false);
		GetDlgItem(IDC_MFCEDITBROWSE1)->ShowWindow(false);
	}

	if(info != NULL)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(info->type0);
		switch(info->type0)
		{
		case 0:
			m_monthOrHour = info->month;
			m_dayOrMinute = info->day;
			m_hour = info->hour;
			m_minute = info->minute;
			break;
		case 1:
		case 2:
			m_monthOrHour = info->hour;
			m_dayOrMinute = info->minute;
			break;
		default: break;
		}
	}
	UpdateData(false);
}



 // 线程函数
UINT ThreadForTaskFunc(LPVOID lpParam)
{
	taskInfo* pInfo=(taskInfo*)lpParam;
	CTime time;
	int hour = 0;
	int minute = 0;
	int second = 0;
	switch(pInfo->type0)
	{
	case pInfo->SPECIALTIME: 
		while(true)
		{
			time = CTime::GetCurrentTime();
			Sleep(1000);
			if(pInfo->isTerminate)
				break;

			if(time.GetMonth()==pInfo->month && time.GetDay()==pInfo->day && time.GetHour()==pInfo->hour &&
				time.GetMinute()==pInfo->minute)
				break;
			
		}
		break;
	case pInfo->AFTERTIME:
		while(true)
		{
			Sleep(1000);
			if(pInfo->isTerminate)
				break;

			if(second != 0)
			{
				second--;
			}
			else if(pInfo->minute != 0)
			{
				pInfo->minute--;
				second = 59;
			}
			else if(pInfo->hour != 0)
			{
				pInfo->hour--;
				pInfo->minute = 59;
				second = 59;
			}
			else
				break;
		}
		break;
	case pInfo->EVERYTIME:
		hour = pInfo->hour;
		minute = pInfo->minute;
		while(true)
		{
			Sleep(1000);
			if(pInfo->isTerminate)
				break;

			if(second != 0)
			{
				second--;
			}
			else if(pInfo->minute != 0)
			{
				pInfo->minute--;
				second = 59;
			}
			else if(pInfo->hour != 0)
			{
				pInfo->hour--;
				pInfo->minute = 59;
				second = 59;
			}
			else
			{
				hour = pInfo->hour;
				minute = pInfo->minute;
				
				if(pInfo->name.Left(5) != _T("自定义程序"))
					Clock(pInfo->content, pInfo->music);
				else
					RunProgram(pInfo->content);
			}

		}
		break;
	default:
		break;
	}
	if(!pInfo->isTerminate)
	{
		if(pInfo->name == _T("注销"))
			Logoff(true);
		else if(pInfo->name == _T("关机"))
			Shutdown(true);
		else if(pInfo->name == _T("重启"))
			Reboot(true);
		else if(pInfo->name.Left(5) == _T("自定义程序"))
			RunProgram(pInfo->content);
		else
			Clock(pInfo->content, pInfo->music);
	}
	
	map<taskInfo*, CWinThread*> *mapPointer = pInfo->mapPointer;
	map<taskInfo*, CWinThread*>::iterator iter = mapPointer->begin();
	for(; iter!=mapPointer->end(); ++iter)
	{
		if(iter->first->name == pInfo->name)
		{
			delete iter->first;
			mapPointer->erase(iter);
			break;
		}
	}

	return 0;
}

// 检查任务设置是否正确
bool CAdvanceDlg::CheckTaskSet(int type0)
{
	UpdateData(true);
	if(type0 == 0)
	{
		int index = ((CComboBox*)GetDlgItem(IDC_COMBO8))->GetCurSel();
		if(index >= 3)
		{
			if(m_caption==_T(""))
				return false;
			else if(index==3 && m_content==_T(""))
				return false;
		}

		CTime time = CTime::GetCurrentTime();
		int month = time.GetMonth();
		int day = time.GetDay();
		int hour = time.GetHour();
		int minute = time.GetMinute();

		switch(((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel())
		{
		case 0:
			if(m_monthOrHour < month)
				return false;
			else if(m_monthOrHour==month && m_dayOrMinute<day)
				return false;
			else if(m_monthOrHour==month && m_dayOrMinute==day && m_hour<hour)
				return false;
			else if(m_monthOrHour==month && m_dayOrMinute==day && m_hour==hour && m_minute<minute)
				return false;
			return true;
		case 1:
		case 2:
			if(m_monthOrHour < 0)
				return false;
			else if(m_monthOrHour==0 && m_dayOrMinute<=0)
				return false;
			return true;
		default: return false;
		}
	}
	
	return true;
}


void CAdvanceDlg::SubmitTask()
{
	UpdateData(true);
	
	taskInfo *info;
	if(((CButton*)GetDlgItem(IDC_RADIO8))->GetCheck())
	{
		for(map<taskInfo*, CWinThread*>::iterator iter=m_taskInfoMap.begin(); iter!=m_taskInfoMap.end(); ++iter)
		{
			if(iter->first->name == m_caption)
			{
				info = iter->first;
				break;
			}
		}
		info->isChanged = true;
	}
	else
	{
		info = new taskInfo();
		info->isChanged = false;
		info->isTerminate = false;
		info->music = _T("");
	}

	if(!CheckTaskSet(0))
	{
		MessageBox(_T("任务设置有误，请检查！"), _T("任务提交"));
		return;
	}

	if(!info->isChanged)
	switch(((CComboBox*)GetDlgItem(IDC_COMBO8))->GetCurSel())
	{
	case 0: 
		info->name = _T("注销");
		info->type1 = info->LOGOFF;
		break;
	case 1: 
		info->name = _T("关机");
		info->type1 = info->SHUTDOWN;
		break;
	case 2:
		info->name = _T("重启");
		info->type1 = info->REBOOT;
		break;
	case 3: 
		info->content = m_content;
		info->name = m_caption;
		info->type1 = info->CLOCK;
		if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
		{
			int index = ((CComboBox*)GetDlgItem(IDC_COMBO9))->GetCurSel();
			((CComboBox*)GetDlgItem(IDC_COMBO9))->GetLBText(index, info->music);
		}
		else
			info->music = _T("");
		break;
	case 4:
		info->content = m_pathOfProgram;
		info->name = _T("自定义程序")+m_caption;
		info->type1 = info->PROGRAM;
		break;
	default: break;
	}

	switch(((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel())
	{
	case 0: 
		info->type0 = info->SPECIALTIME;
		info->month = m_monthOrHour;
		info->day = m_dayOrMinute;
		info->hour = m_hour;
		info->minute = m_minute;
		break;
	case 1:
		info->type0 = info->AFTERTIME;
		info->hour = m_monthOrHour;
		info->minute = m_dayOrMinute;
		break;
	case 2:
		info->type0 = info->EVERYTIME;
		info->hour = m_monthOrHour;
		info->minute = m_dayOrMinute;
		break;
	default: break;
	}
	info->mapPointer = &m_taskInfoMap;
	if(!info->isChanged)
	{
		CWinThread* pThread= ::AfxBeginThread(ThreadForTaskFunc, info);
		m_taskInfoMap.insert(make_pair(info, pThread));
	}
	
	MessageBox(_T("任务提交成功！"), _T("任务提交"));

}


void CAdvanceDlg::DeleteTask()
{
	UpdateData(true);

	if(MessageBox(_T("是否确定移除该任务？"), _T("任务删除"), MB_OKCANCEL) == IDCANCEL)
		return;

	map<taskInfo*, CWinThread*>::iterator iter = m_taskInfoMap.begin();
	for(; iter!=m_taskInfoMap.end(); ++iter)
	{
		if((iter->first->type1!=iter->first->CLOCK && iter->first->name==m_caption) || (iter->first->type1==iter->first->PROGRAM  && iter->first->name.Mid(5)==m_caption))
		{
			iter->first->isTerminate = true;
			MessageBox(_T("删除成功！"), _T("任务删除"));
			int index = ((CComboBox*)GetDlgItem(IDC_COMBO8))->GetCurSel();
			((CComboBox*)GetDlgItem(IDC_COMBO8))->DeleteString(index);
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(false);
			m_monthOrHour = 0;
			m_dayOrMinute =0;
			m_caption = _T("");
			m_content = _T("");
			m_pathOfProgram = _T("");
			UpdateData(false);
			return;
		}
	}

	MessageBox(_T("无此任务，删除失败！"), _T("任务删除"));
}



void CAdvanceDlg::OnTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	MessageBox(_T("本软件可自动运行无需管理员权限的exe文件，软件退出后，任务将消除！"), _T("任务提示"));
	*pResult = 0;
}


void CAdvanceDlg::OnTryMusic()
{
	if(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() && g_CurrentMusic!="")
	{
		g_IsPlay = true;
		g_ChangeMusic = true;
		ResumeThread(g_pNewThread);
	}
	else
	{
		g_IsPlay = false;
		g_ChangeMusic = false;
		ResumeThread(g_pNewThread);
	}
}


void CAdvanceDlg::OnMusicList()
{
	UpdateData(true);
	if(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		((CButton*)GetDlgItem(IDC_COMBO9))->GetWindowText(g_CurrentMusic);
		if(g_CurrentMusic != "")
		{
			g_IsPlay = true;
			g_ChangeMusic = true;
			ResumeThread(g_pNewThread);
		}
	}
}


HBRUSH CAdvanceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
