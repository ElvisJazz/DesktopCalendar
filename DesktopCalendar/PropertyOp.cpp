// PropertyOp.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "PropertyOp.h"


// CPropertyOp
CFile g_file;

IMPLEMENT_DYNAMIC(CPropertyOp, CWnd)

CPropertyOp::CPropertyOp()
{
}

CPropertyOp::~CPropertyOp()
{
}


LPTSTR CPropertyOp::lpPath = _T("..//INI//IniFileName.ini");
LPTSTR CPropertyOp::lpDefaultPath = _T("..//INI//DefaultIniFileName.ini");

BEGIN_MESSAGE_MAP(CPropertyOp, CWnd)
END_MESSAGE_MAP()

// 将日历属性写到外部配置文件中
void CPropertyOp::WriteProperty(CalProperty* calProperty)
{
	CString tmp = _T("");
	// 配置窗口
	tmp.Format(_T("%d"), calProperty->rect.bottom);
	WritePrivateProfileString(_T("Rect"), _T("bottom"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->rect.top);
	WritePrivateProfileString(_T("Rect"), _T("top"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->rect.right);
	WritePrivateProfileString(_T("Rect"), _T("right"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->rect.left);
	WritePrivateProfileString(_T("Rect"), _T("left"), tmp, lpPath);

	// 配置信息
	tmp.Format(_T("%d"), calProperty->displayMode);
	WritePrivateProfileString(_T("Info"), _T("displayMode"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->startXinqi);
	WritePrivateProfileString(_T("Info"), _T("startXinqi"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->firstWeek);
	WritePrivateProfileString(_T("Info"), _T("firstWeek"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isDisplayWeeks);	
	WritePrivateProfileString(_T("Info"), _T("isDisplayWeeks"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->currentGoneRows);
	WritePrivateProfileString(_T("Info"), _T("currentGoneRows"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isDisplayMore[0]);
	WritePrivateProfileString(_T("Info"), _T("displayFestival"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isDisplayMore[1]);	
	WritePrivateProfileString(_T("Info"), _T("displayAgriCal"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isDisplayMore[2]);
	WritePrivateProfileString(_T("Info"), _T("displayJieqi"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isHolidayColor[0]);
	WritePrivateProfileString(_T("Info"), _T("week0"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isHolidayColor[1]);
	WritePrivateProfileString(_T("Info"), _T("week1"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isHolidayColor[2]);
	WritePrivateProfileString(_T("Info"), _T("week2"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isHolidayColor[3]);
	WritePrivateProfileString(_T("Info"), _T("week3"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isHolidayColor[4]);
	WritePrivateProfileString(_T("Info"), _T("week4"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isHolidayColor[5]);
	WritePrivateProfileString(_T("Info"), _T("week5"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isHolidayColor[6]);
	WritePrivateProfileString(_T("Info"), _T("week6"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isOpenAni);
	WritePrivateProfileString(_T("Info"), _T("isOpenAni"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isOpenInfoWin);
	WritePrivateProfileString(_T("Info"), _T("isOpenInfoWin"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->isOpenSubscription);
	WritePrivateProfileString(_T("Info"), _T("isOpenSubscription"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->mode);
	WritePrivateProfileString(_T("Info"), _T("mode"), tmp, lpPath);

	tmp.Format(_T("%d"), calProperty->isSetFont);
	WritePrivateProfileString(_T("Info"), _T("isSetFont"), tmp, lpPath);

	tmp.Format(_T("%d"), calProperty->isAutoRun);
	WritePrivateProfileString(_T("Info"), _T("isAutoRun"), tmp, lpPath);

	if(calProperty->isSetFont)
	{
		LOGFONT lf;
		calProperty->font->GetLogFont(&lf);
		
		g_file.Open (_T("..//INI//font.ini"), CFile::modeWrite | CFile::modeCreate);
		g_file.Write(&lf, sizeof(LOGFONT));
		g_file.Close();
	}
	// 配置优先级任务
	CString tmp0;
	for(int i=0; i<4; i++)
	{
		tmp.Format(_T("%s"), calProperty->task[i]);
		tmp.Replace(_T("\r\n"), _T("\t"));
		tmp0.Format(_T("task%d"), i+1);
		WritePrivateProfileString(_T("Info"), tmp0, tmp, lpPath);
	}

	// 配置外观
	tmp.Format(_T("%d"), calProperty->grap);
	WritePrivateProfileString(_T("Shape"), _T("grap"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->row);
	WritePrivateProfileString(_T("Shape"), _T("row"), tmp, lpPath);	
	tmp.Format(_T("%d"), calProperty->transparentLevel);
	WritePrivateProfileString(_T("Shape"), _T("transparentLevel"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->openAero);
	WritePrivateProfileString(_T("Shape"), _T("openAero"), tmp, lpPath);
	
	tmp.Format(_T("%d"), calProperty->commonColor);
	WritePrivateProfileString(_T("Shape"), _T("commonColor"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->holidayColor);
	WritePrivateProfileString(_T("Shape"), _T("holidayColor"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->currentColor);
	WritePrivateProfileString(_T("Shape"), _T("currentColor"), tmp, lpPath);
	tmp.Format(_T("%d"), calProperty->fontColor);
	WritePrivateProfileString(_T("Shape"), _T("fontColor"), tmp, lpPath);


	// 配置订阅设置
	tmp.Format(_T("%d"), calProperty->numOfSubscription);
	WritePrivateProfileString(_T("Subscription"), _T("numOfSubscription"), tmp, lpPath);
	int j = 0;
	for(vector<bool>::iterator iter=(calProperty->isValid).begin(); iter!=(calProperty->isValid).end(); ++iter)
	{
		if(*iter)
		{
			tmp.Format(_T("webpageName%d"), j+1);
			WritePrivateProfileString(_T("Subscription"), tmp, calProperty->webpageName[j], lpPath);

			tmp.Format(_T("url%d"), j+1);
			WritePrivateProfileString(_T("Subscription"), tmp, calProperty->url[j], lpPath);


			j++;
		}
	}

}


// 读取配置文件
void CPropertyOp::ReadProperty(CalProperty* calProperty, bool isDefault)
{
	LPTSTR path;
	if(!isDefault)
	    path = lpPath;
	else
		path = lpDefaultPath;

	CString tmp = _T("");
	// 配置窗口
	calProperty->rect.bottom = GetPrivateProfileInt(_T("Rect"), _T("bottom"), -2, path);
	calProperty->rect.top = GetPrivateProfileInt(_T("Rect"), _T("top"), -2, path);
	calProperty->rect.right = GetPrivateProfileInt(_T("Rect"), _T("right"), -2, path);
	calProperty->rect.left = GetPrivateProfileInt(_T("Rect"), _T("left"), -2, path);

	// 配置信息
	calProperty->displayMode = GetPrivateProfileInt(_T("Info"), _T("displayMode"), -2, path);
	calProperty->startXinqi = GetPrivateProfileInt(_T("Info"), _T("startXinqi"), -2, path);
	calProperty->firstWeek = GetPrivateProfileInt(_T("Info"), _T("firstWeek"), -2, path);
	calProperty->isDisplayWeeks = GetPrivateProfileInt(_T("Info"), _T("isDisplayWeeks"), -2, path);
	calProperty->currentGoneRows = GetPrivateProfileInt(_T("Info"), _T("currentGoneRows"), -2, path);
	calProperty->isDisplayMore[0] = GetPrivateProfileInt(_T("Info"), _T("displayFestival"), -2, path);
	calProperty->isDisplayMore[1] = GetPrivateProfileInt(_T("Info"), _T("displayAgriCal"), -2, path);
	calProperty->isDisplayMore[2] = GetPrivateProfileInt(_T("Info"), _T("displayJieqi"), -2, path);
	calProperty->isHolidayColor[0] = GetPrivateProfileInt(_T("Info"), _T("week0"), -2, path);
	calProperty->isHolidayColor[1] = GetPrivateProfileInt(_T("Info"), _T("week1"), -2, path);
	calProperty->isHolidayColor[2] = GetPrivateProfileInt(_T("Info"), _T("week2"), -2, path);
	calProperty->isHolidayColor[3] = GetPrivateProfileInt(_T("Info"), _T("week3"), -2, path);
	calProperty->isHolidayColor[4] = GetPrivateProfileInt(_T("Info"), _T("week4"), -2, path);
	calProperty->isHolidayColor[5] = GetPrivateProfileInt(_T("Info"), _T("week5"), -2, path);
	calProperty->isHolidayColor[6] = GetPrivateProfileInt(_T("Info"), _T("week6"), -2, path);
	calProperty->isOpenAni = GetPrivateProfileInt(_T("Info"), _T("isOpenAni"), -2, path);
	calProperty->isOpenInfoWin = GetPrivateProfileInt(_T("Info"), _T("isOpenInfoWin"), -2, path);
	calProperty->isOpenSubscription = GetPrivateProfileInt(_T("Info"), _T("isOpenSubscription"), -2, path);
	calProperty->mode = GetPrivateProfileInt(_T("Info"), _T("mode"), -2, path);
	calProperty->isSetFont = GetPrivateProfileInt(_T("Info"), _T("isSetFont"), -2, path);
	calProperty->isAutoRun= GetPrivateProfileInt(_T("Info"), _T("isAutoRun"), -2, path);
	
	if(calProperty->isSetFont)
	{		
		g_file.Open(_T("..//INI//font.ini"), CFile::modeRead);
		LOGFONT lf;
	    g_file.Read(&lf, sizeof(LOGFONT));
		calProperty->font = new CFont();
        calProperty->font->CreateFontIndirect(&lf);
		g_file.Close();
	}

	CString str = _T("");

	// 配置任务
	for(int i=0; i<4; i++)
	{
		tmp.Format(_T("task%d"), i+1);
		GetPrivateProfileString(_T("Info"), tmp,  _T(""), str.GetBuffer(MAX_PATH), MAX_PATH, path);
		str.Replace(_T("\t"), _T("\r\n"));
		calProperty->task[i] = str;
	}

	// 配置外观
	calProperty->grap = GetPrivateProfileInt(_T("Shape"), _T("grap"), -2, path);
	calProperty->row = GetPrivateProfileInt(_T("Shape"), _T("row"), -2, path);	
	calProperty->transparentLevel = GetPrivateProfileInt(_T("Shape"), _T("transparentLevel"), -2, path);
	calProperty->openAero = GetPrivateProfileInt(_T("Shape"), _T("openAero"), -2, path);
	
	calProperty->commonColor = GetPrivateProfileInt(_T("Shape"), _T("commonColor"), -2, path);
	calProperty->holidayColor = GetPrivateProfileInt(_T("Shape"), _T("holidayColor"), -2, path);
	calProperty->currentColor = GetPrivateProfileInt(_T("Shape"), _T("currentColor"), -2, path);
	calProperty->fontColor = GetPrivateProfileInt(_T("Shape"), _T("fontColor"), -2, path);
	
	// 配置订阅
	calProperty->numOfSubscription = GetPrivateProfileInt(_T("Subscription"), _T("numOfSubscription"), -2, path);
	int num = calProperty->numOfSubscription;

	if(!(calProperty->url).IsEmpty())
		(calProperty->url).RemoveAll();
	for(int i=1; i<=calProperty->numOfSubscription; i++)
	{
		(calProperty->isValid).push_back(true);

		tmp.Format(_T("url%d"), i);
		GetPrivateProfileString(_T("Subscription"), tmp, _T(""), str.GetBuffer(MAX_PATH), MAX_PATH, path);
		(calProperty->url).Add(str); 

		tmp.Format(_T("webpageName%d"), i);
		GetPrivateProfileString(_T("Subscription"), tmp, _T(""), str.GetBuffer(MAX_PATH), MAX_PATH, path);
		(calProperty->webpageName).Add(str); 
	}
}
