
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
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
	bool openAero; // 开启毛玻璃
	int  transparentLevel; // 透明度
	int  row; // 行数
	int currentGoneRows;
	int  grap; // 间隔
	CRect rect; // 窗口坐标信息
	int startXinqi; // 起始星期
	int currentX; // 当前位置列数
	int currentY; // 当前位置行数
	int widthTmp; // 水平间隔参数
	int widthTmp2;// 垂直间隔参数
	int numOfUnion; // 合并行数目
	vector<pair<int, int> > posOfUnion; // 合并行的跳转位置

	int displayMode; // 显示模式：1.显示浮动月份；2.显示固定月份；3.显示完整月份(PS:固定和完整的共同点就是当前月份顶格，不同点就是是否有后续月份，固定有，完整无）
	bool isDisplayWeeks; // 是否显示周数
	bool isCalTime; // 是否是选择日历时间
	bool isSetFirstWeek;// 是否在设置第一周
	bool isDisplayMore[3]; // 更多显示（节日，农历，二十四节气）

	// 信息抽取部分
	int numOfSubscription; // 网页抽取数量
	CStringArray webpageName; // 网页名称
	CStringArray url; // 链接数组
	CStringArray config; // 配置内容
	vector<bool> isValid; // 是否有效

    bool isOpenAni; // 是否开启片头动画
	bool isOpenInfoWin; // 是否开启信息显示
	bool isOpenSubscription; // 是否开启订阅信息显示
	int mode; // 1桌面模式2抽屉模式3任务模式 
	CTime time; // 当前日期（显示意义上的）
	int firstWeek; // 自定义第一周对默认第零周的偏移量

	int nextPage; // 下一页或下一行
	int prePage; // 上一行或上一页

	int startMonth; // 起始月份
	int startDay; // 起始日期

	CFont *font;
	bool isSetFont;

	COLORREF commonColor; // 普通日期单元格颜色
	COLORREF holidayColor; // 指定星期单元格颜色
	COLORREF currentColor; // 当前日期颜色
	COLORREF fontColor; // 字体颜色
	bool isHolidayColor[7]; // 指定星期颜色

	CDesktopMostDlg *pDlg; // 存储当前日历桌面指针
	CDesktopDlg *pDlg0; // 存储最低层面板指针 
	CTextInputDlg* pTextDlg; // 存储当前文本框指针
	CString   strCode; // 存储要更新的时间字符串索引码
	int   xiqiCode; // 存储要更新的日期的星期
	CStatic*  pStatic; // 存储要更新的静态文本框指针
	CMyButton* pButton; // 存储要更新的色块按钮指针  


	map<CString, Info*> infoMap; // 存储字块备忘录内容信息 按时间字符串代码索引显示信息
	map<CString, Memo*> *memoMap; // 存储色块位置等信息
	map<int, CFileOperation*> fileOpMap; // 各个年表的记录操作存储

	bool isAutoRun; // 是否开机自启动

	CString task[4]; // 优先任务

} CalProperty;

static int g_days[12] = {31, 28, 31, 30, 31, 30 ,31, 31, 30, 31, 30, 31};