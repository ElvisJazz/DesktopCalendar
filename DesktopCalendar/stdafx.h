
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��
#include <afxdisp.h>
#include <vector>
#include <map>
#include <utility>
#include "DlgBase.h"
#include "MyButton.h"
#include "FileOperation.h"
using namespace std;



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define TRANSPARENT_COLOR RGB(255,235,205)
#define COMMON_TEXT_COLOR RGB(255,255,255)

class CDesktopMostDlg;
class CDesktopDlg;
class CTextInputDlg;

typedef struct info
{
	CString text;
	COLORREF color;
} Info;


typedef struct memo
{
	CStatic *static0;
	CMyButton *button0;
	int year;
	int month;
	int day;
	int dayOfWeek;

} Memo;

typedef struct paramete
{
	bool openAero; // ����ë����
	int  transparentLevel; // ͸����
	int  row; // ����
	int currentGoneRows;
	int  grap; // ���
	CRect rect; // ����������Ϣ
	int startXinqi; // ��ʼ����
	int currentX; // ��ǰλ������
	int currentY; // ��ǰλ������
	int widthTmp; // ˮƽ�������
	int widthTmp2;// ��ֱ�������
	int numOfUnion; // �ϲ�����Ŀ
	vector<pair<int, int> > posOfUnion; // �ϲ��е���תλ��

	int displayMode; // ��ʾģʽ��1.��ʾ�����·ݣ�2.��ʾ�̶��·ݣ�3.��ʾ�����·�(PS:�̶��������Ĺ�ͬ����ǵ�ǰ�·ݶ��񣬲�ͬ������Ƿ��к����·ݣ��̶��У������ޣ�
	bool isDisplayWeeks; // �Ƿ���ʾ����
	bool isCalTime; // �Ƿ���ѡ������ʱ��
	bool isSetFirstWeek;// �Ƿ������õ�һ��
	bool isDisplayMore[3]; // ������ʾ�����գ�ũ������ʮ�Ľ�����

	// ��Ϣ��ȡ����
	int numOfSubscription; // ��ҳ��ȡ����
	CStringArray webpageName; // ��ҳ����
	CStringArray url; // ��������
	CStringArray config; // ��������
	vector<bool> isValid; // �Ƿ���Ч

    bool isOpenAni; // �Ƿ���Ƭͷ����
	bool isOpenInfoWin; // �Ƿ�����Ϣ��ʾ
	bool isOpenSubscription; // �Ƿ���������Ϣ��ʾ
	int mode; // 1����ģʽ2����ģʽ3����ģʽ 
	CTime time; // ��ǰ���ڣ���ʾ�����ϵģ�
	int firstWeek; // �Զ����һ�ܶ�Ĭ�ϵ����ܵ�ƫ����

	int nextPage; // ��һҳ����һ��
	int prePage; // ��һ�л���һҳ

	int startMonth; // ��ʼ�·�
	int startDay; // ��ʼ����

	CFont *font;
	bool isSetFont;

	COLORREF commonColor; // ��ͨ���ڵ�Ԫ����ɫ
	COLORREF holidayColor; // ָ�����ڵ�Ԫ����ɫ
	COLORREF currentColor; // ��ǰ������ɫ
	COLORREF fontColor; // ������ɫ
	bool isHolidayColor[7]; // ָ��������ɫ

	CDesktopMostDlg *pDlg; // �洢��ǰ��������ָ��
	CDesktopDlg *pDlg0; // �洢��Ͳ����ָ�� 
	CTextInputDlg* pTextDlg; // �洢��ǰ�ı���ָ��
	CString   strCode; // �洢Ҫ���µ�ʱ���ַ���������
	int   xiqiCode; // �洢Ҫ���µ����ڵ�����
	CStatic*  pStatic; // �洢Ҫ���µľ�̬�ı���ָ��
	CMyButton* pButton; // �洢Ҫ���µ�ɫ�鰴ťָ��  


	map<CString, Info*> infoMap; // �洢�ֿ鱸��¼������Ϣ ��ʱ���ַ�������������ʾ��Ϣ
	map<CString, Memo*> *memoMap; // �洢ɫ��λ�õ���Ϣ
	map<int, CFileOperation*> fileOpMap; // �������ļ�¼�����洢

	bool isAutoRun; // �Ƿ񿪻�������

	CString task[4]; // ��������

} CalProperty;

static int g_days[12] = {31, 28, 31, 30, 31, 30 ,31, 31, 30, 31, 30, 31};