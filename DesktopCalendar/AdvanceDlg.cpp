// AdvanceDlg.cpp : ʵ���ļ�
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

// ����ѭ������

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
					(DWORD)   (LPMCI_PLAY_PARMS)&mciPlay);  //MCI_DGV_PLAY_REPEAT, Ҫ #include "Digitalv.h"
			
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

//ʹ�ܹػ���Ȩ����
BOOL EnableShutdownPrivilege()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = {0};
	TOKEN_PRIVILEGES stToken_Privileges = {0};

	hProcess = ::GetCurrentProcess(); //��ȡ��ǰӦ�ó�����̾��

	if(!::OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken)) //�򿪵�ǰ���̵ķ������ƾ��(OpenProcessToken��������ʧ�ܷ���ֵΪ��)
		return FALSE;

	if(!::LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&uID)) //��ȡȨ������Ϊ"SeShutdownPrivilege"��LUID(LookupPrivilegeValue��������ʧ�ܷ���ֵΪ��)
		return FALSE;

	stToken_Privileges.PrivilegeCount = 1; //��������Ȩ�޸���
	stToken_Privileges.Privileges[0].Luid = uID; //Ȩ�޵�LUID
	stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; //Ȩ�޵�����,SE_PRIVILEGE_ENABLEDΪʹ�ܸ�Ȩ��

	if(!::AdjustTokenPrivileges(hToken,FALSE,&stToken_Privileges,sizeof stToken_Privileges,NULL,NULL)) //���������������ָ��Ȩ��(AdjustTokenPrivileges��������ʧ�ܷ���ֵΪ��)
		return FALSE;

	if(::GetLastError() != ERROR_SUCCESS) //�鿴Ȩ���Ƿ�����ɹ�
		return FALSE;

	::CloseHandle(hToken);
	return TRUE;
}

//�ػ�����
BOOL Shutdown(BOOL bForce)
{
	EnableShutdownPrivilege(); //ʹ�ܹػ���Ȩ����
	if(bForce)
		return ::ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,0); //ǿ�ƹػ�
	else
		return ::ExitWindowsEx(EWX_SHUTDOWN,0);
}

//ע������
BOOL Logoff(BOOL bForce)
{
	if(bForce)
		return ::ExitWindowsEx(EWX_LOGOFF | EWX_FORCE,0); //ǿ��ע��
	else
		return ::ExitWindowsEx(EWX_LOGOFF,0);
}

//��������
BOOL Reboot(BOOL bForce)
{
	EnableShutdownPrivilege(); //ʹ�ܹػ���Ȩ����
	if(bForce)
		return ::ExitWindowsEx(EWX_REBOOT | EWX_FORCE,0); //ǿ������
	else
		return ::ExitWindowsEx(EWX_REBOOT,0);
}


// ����
void Clock(CString content, CString music)
{
	g_IsPlay = true;
	g_CurrentMusic = music;

	MessageBox(NULL, content, _T("�Զ�������"), MB_OK);
	g_IsPlay = false;
}

// �����Զ������
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

// CAdvanceDlg �Ի���

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

	// ���ֳ�ʼ��	
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
	// �쳣: OCX ����ҳӦ���� FALSE
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


// CAdvanceDlg ��Ϣ�������
// ����ʱ��
void CAdvanceDlg::SetTime()
{
	CTime time = CTime::GetCurrentTime();
	int year = time.GetYear();
	int month = time.GetMonth();
	int day = time.GetDay();
	CString tmpStr;
	tmpStr.Format(_T("%d��"), year);
	GetDlgItem(IDC_STATIC2)->SetWindowText(tmpStr);
	tmpStr.Format(_T("%d"), month);
	GetDlgItem(IDC_EDIT1)->SetWindowText(tmpStr);
	tmpStr.Format(_T("%d"), day);
	GetDlgItem(IDC_EDIT2)->SetWindowText(tmpStr);
}

// �������
void CAdvanceDlg::OnBnClickedRadio7()
{
	SetTime();

	CComboBox* combo8 = (CComboBox*)GetDlgItem(IDC_COMBO8);
	combo8->ResetContent();
	combo8->AddString(_T("ע��"));
	combo8->AddString(_T("�ػ�"));
	combo8->AddString(_T("����"));
	combo8->AddString(_T("����"));
	combo8->AddString(_T("�Զ������"));

	GetDlgItem(IDC_STATIC1)->SetWindowText(_T("�������"));
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

// �޸�����
void CAdvanceDlg::OnBnClickedRadio8()
{
	GetDlgItem(IDC_STATIC1)->SetWindowText(_T("�޸�����"));
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
		GetDlgItem(IDC_STATIC5)->SetWindowText(_T("��"));
		GetDlgItem(IDC_STATIC6)->SetWindowText(_T("��"));

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
		GetDlgItem(IDC_STATIC5)->SetWindowText(_T("Сʱ"));
		GetDlgItem(IDC_STATIC6)->SetWindowText(_T("��"));
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
	if(content0!=_T("ע��") && content0!=_T("�ػ�") && content0!=_T("����"))
	{
		GetDlgItem(IDC_STATIC4)->ShowWindow(true);
		GetDlgItem(IDC_STATIC9)->ShowWindow(true);
		GetDlgItem(IDC_EDIT5)->ShowWindow(true);
		
		
		if(content0.Left(5) != _T("�Զ������"))
		{
			GetDlgItem(IDC_STATIC4)->SetWindowTextW(_T("�Զ�����������"));
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
			GetDlgItem(IDC_STATIC4)->SetWindowTextW(_T("�����ַ"));
			GetDlgItem(IDC_EDIT7)->ShowWindow(false);
			GetDlgItem(IDC_MFCEDITBROWSE1)->ShowWindow(true);			
			GetDlgItem(IDC_CHECK1)->ShowWindow(false);
			GetDlgItem(IDC_CHECK2)->ShowWindow(false);
			GetDlgItem(IDC_COMBO9)->ShowWindow(false);
		}


		if(info != NULL)
		{
			if(content0.Left(5) != _T("�Զ������"))
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



 // �̺߳���
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
				
				if(pInfo->name.Left(5) != _T("�Զ������"))
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
		if(pInfo->name == _T("ע��"))
			Logoff(true);
		else if(pInfo->name == _T("�ػ�"))
			Shutdown(true);
		else if(pInfo->name == _T("����"))
			Reboot(true);
		else if(pInfo->name.Left(5) == _T("�Զ������"))
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

// ������������Ƿ���ȷ
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
		MessageBox(_T("���������������飡"), _T("�����ύ"));
		return;
	}

	if(!info->isChanged)
	switch(((CComboBox*)GetDlgItem(IDC_COMBO8))->GetCurSel())
	{
	case 0: 
		info->name = _T("ע��");
		info->type1 = info->LOGOFF;
		break;
	case 1: 
		info->name = _T("�ػ�");
		info->type1 = info->SHUTDOWN;
		break;
	case 2:
		info->name = _T("����");
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
		info->name = _T("�Զ������")+m_caption;
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
	
	MessageBox(_T("�����ύ�ɹ���"), _T("�����ύ"));

}


void CAdvanceDlg::DeleteTask()
{
	UpdateData(true);

	if(MessageBox(_T("�Ƿ�ȷ���Ƴ�������"), _T("����ɾ��"), MB_OKCANCEL) == IDCANCEL)
		return;

	map<taskInfo*, CWinThread*>::iterator iter = m_taskInfoMap.begin();
	for(; iter!=m_taskInfoMap.end(); ++iter)
	{
		if((iter->first->type1!=iter->first->CLOCK && iter->first->name==m_caption) || (iter->first->type1==iter->first->PROGRAM  && iter->first->name.Mid(5)==m_caption))
		{
			iter->first->isTerminate = true;
			MessageBox(_T("ɾ���ɹ���"), _T("����ɾ��"));
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

	MessageBox(_T("�޴�����ɾ��ʧ�ܣ�"), _T("����ɾ��"));
}



void CAdvanceDlg::OnTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	MessageBox(_T("��������Զ������������ԱȨ�޵�exe�ļ�������˳�������������"), _T("������ʾ"));
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
