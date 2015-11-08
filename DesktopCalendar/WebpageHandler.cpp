// WebpageHandler.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "WebpageHandler.h"
#include "MyHttpClient.h"
#include <ctype.h>
#include "string.h"
#include "afxinet.h" //加入下载网页要用的头文件
#include <vector>
using namespace std;

//
//#pragma comment(lib, "json_vc71_libmtd.lib")
// 
//#include <fstream>
//#include <cassert>
//#include "json/json.h"

// CWebpageHandler

IMPLEMENT_DYNAMIC(CWebpageHandler, CWnd)

CWebpageHandler::CWebpageHandler()
{
	m_htmlStr = _T("");
}

CWebpageHandler::~CWebpageHandler()
{
}



BEGIN_MESSAGE_MAP(CWebpageHandler, CWnd)
END_MESSAGE_MAP()


bool CWebpageHandler::GetSourceHtml(CString theUrl,CString fileName)
{
    CInternetSession session;
	CHttpFile *file = NULL; 
	CString strURL = theUrl;
	CString strHtml = _T("");   //存放网页数据 

	try{
		   file = (CHttpFile*)session.OpenURL(strURL);

	}catch(CInternetException * m_pException){
		   file = NULL;
		   m_pException->m_dwError;
		   m_pException->Delete();
		   session.Close();

		   MessageBox(_T("网络连接失败！"));
		   return false;
	}

	CString strLine;
	if(file != NULL){
		   while(file->ReadString(strLine) != NULL){
		   strHtml += strLine;
		   }
 	}else{
		MessageBox(_T("读取网络数据失败！"));
		return false;
	}
 
	CFile file0(fileName, CFile::modeCreate|CFile::modeWrite);
	int len = strHtml.GetLength()*2;
	file0.Write(strHtml, len);
	session.Close();
	file->Close();
	file0.Close();
	delete file;
	file = NULL;

	m_htmlStr = strHtml;
	m_url = theUrl;
	return true;
}

bool CWebpageHandler::GetLocalHtml(CString fileName)
{
	CFile file(fileName, CFile::modeRead);
	char *pBuf;
	int iLen=file.GetLength();
	pBuf=new char[iLen+1];
	file.Read(pBuf,iLen);
	pBuf[iLen]=0;
	CString str1(pBuf); 
	int index;
	CString charset = _T("");
	if((index=str1.Find(_T("charset="))) != -1)
	{
		str1.Delete(0, index+8);
		index = str1.Find(_T("\""));
		if(index == 0)
		{
			str1.Delete(0, 1);
			index = str1.Find(_T("\""));
		}
		charset = str1.Left(index);
	}

	if(charset==_T("UTF-8") || charset==_T("urf-8"))
		str1=CA2W(pBuf,CP_UTF8); //Utf8格式文件用此方法
	delete[] pBuf;
	file.Close();
	//MessageBox(str1);
	m_htmlStr = str1;
	return true;
}

bool CWebpageHandler::ReadExtractionIni(LPTSTR fileName)
{
	CString tmpS;

	int num = GetPrivateProfileInt(_T("numOfFeature"), _T("num"), -2, fileName);
	if(num == -2)
		return false;

	for(int i=0; i<num; i++)
	{
		CString s;
		ExtractionInfo exInfo;
	
		s.Format(_T("%d"), i+1);
		exInfo.isGroup = GetPrivateProfileInt(s, _T("isGroup"), -2, fileName);
		GetPrivateProfileString(s, _T("title"), _T(""), exInfo.title.GetBuffer(MAX_PATH), MAX_PATH, fileName);
		GetPrivateProfileString(s, _T("feature"), _T(""), exInfo.feature.GetBuffer(MAX_PATH), MAX_PATH, fileName);
		GetPrivateProfileString(s, _T("field"), _T(""), exInfo.field.GetBuffer(MAX_PATH), MAX_PATH, fileName);
		GetPrivateProfileString(s, _T("pos"), _T(""), tmpS.GetBuffer(MAX_PATH), MAX_PATH, fileName);
		GetPrivateProfileString(s, _T("startToken"), _T(""), exInfo.startToken.GetBuffer(MAX_PATH), MAX_PATH, fileName);
		GetPrivateProfileString(s, _T("endToken"), _T(""), exInfo.endToken.GetBuffer(MAX_PATH), MAX_PATH, fileName);
		GetPrivateProfileString(s, _T("endFeature"), _T(""), exInfo.endFeature.GetBuffer(MAX_PATH), MAX_PATH, fileName);

		if(tmpS == _T("0"))
			exInfo.pos.push_back(0);
		else
		{
			int index = tmpS.Find(_T(","));
			CString str;
			while(index != -1)
			{
				str = tmpS.Left(index);
				tmpS.Delete(0, index+1);
				exInfo.pos.push_back(_ttoi(str));
				index = tmpS.Find(_T(","));
			}
			exInfo.pos.push_back(_ttoi(tmpS));
		}

		m_extractionInfo.push_back(exInfo);
	}

	return true;
}

bool CWebpageHandler::ExtractInfo(int order)
{
	CString tmpDisplay = _T("");
	CString htmlStr = m_htmlStr;
	int endIndex, index, nextIndex;
	vector<ExtractionInfo>::iterator iter = m_extractionInfo.begin();
	// feature 内部解析循环feature
	for(; iter!=m_extractionInfo.end(); ++iter)
	{
		CString ss = iter->title;
		tmpDisplay.Format(_T("%s<title>%s"), tmpDisplay, ss);
		tmpDisplay += _T("\n");
		index = htmlStr.Find(iter->feature);
		htmlStr.Delete(0, index+1);
		index = htmlStr.Find(iter->field);
		htmlStr.Delete(0, index+1);
		index = htmlStr.Find(_T("href="));
		htmlStr.Delete(0, index+5);

		CString token = htmlStr.Left(1);
		htmlStr.Delete(0);
		index = htmlStr.Find(token);
		tmpDisplay += (_T("<href>")+htmlStr.Left(index)+_T("\n")); // 添加链接标记

		// field内部第一次解析信息（循环pos)
		vector<int>::iterator iter0 = iter->pos.begin();
		int i = 0;
		for(; iter0!=iter->pos.end(); ++iter0)
		{
			if(*iter0 != 0)
			{
				for(; i<(*iter0); i++)
					index = htmlStr.Find(iter->startToken);

			    (iter->startToken).ReleaseBuffer();
				int len = (iter->startToken).GetLength();
				htmlStr.Delete(0, index+len);
				index = htmlStr.Find(iter->endToken);
				tmpDisplay += (htmlStr.Left(index)+_T("\n"));
				htmlStr.Delete(0, index+1);
			}
			else
			{
				index=htmlStr.Find(iter->startToken);
				nextIndex = htmlStr.Find(iter->field);
				for(; index<nextIndex; index=htmlStr.Find(iter->startToken))
				{
					(iter->startToken).ReleaseBuffer();
					int len = (iter->startToken).GetLength();
					htmlStr.Delete(0, index+len);
					index = htmlStr.Find(iter->endToken);
					tmpDisplay += (htmlStr.Left(index)+_T("\n"));
					htmlStr.Delete(0, index+1);
					nextIndex = htmlStr.Find(iter->field);
				}
				break;
			}
		}
		// field内部循环field（循环pos）
		if(iter->isGroup)
		{
			index = htmlStr.Find(iter->field);
			endIndex = htmlStr.Find(iter->endFeature);

			while(index < endIndex)
			{
				htmlStr.Delete(0, index+1);
				index = htmlStr.Find(_T("href="));
				if(index >= endIndex)
					break;
				htmlStr.Delete(0, index+5);
				CString token = htmlStr.Left(1);
				htmlStr.Delete(0);
				index = htmlStr.Find(token);
				tmpDisplay += (_T("<href>")+htmlStr.Left(index)+_T("\n")); // 添加链接标记

				htmlStr.Delete(0, index+1);
				vector<int>::iterator iter1 = iter->pos.begin();
				int i = 0;
				for(; iter1!=iter->pos.end(); ++iter1)
				{
					if(*iter1 != 0)
					{
						for(; i<(*iter1); i++)
							index = htmlStr.Find(iter->startToken);

						(iter->startToken).ReleaseBuffer();
						int len = (iter->startToken).GetLength();
						htmlStr.Delete(0, index+len);
						index = htmlStr.Find(iter->endToken);
						tmpDisplay += (htmlStr.Left(index)+_T("\n"));
						htmlStr.Delete(0, index+1);
					}
					else
					{
						index=htmlStr.Find(iter->startToken);
						nextIndex = htmlStr.Find(iter->field);
						for(; index<nextIndex; index=htmlStr.Find(iter->startToken))
						{
							(iter->startToken).ReleaseBuffer();
							int len = (iter->startToken).GetLength();
							htmlStr.Delete(0, index+len);
							index = htmlStr.Find(iter->endToken);
							tmpDisplay += (htmlStr.Left(index)+_T("\n"));
							htmlStr.Delete(0, index+1);
							nextIndex = htmlStr.Find(iter->field);
						}
						break;
					}
				}

				index = htmlStr.Find(iter->field);
				endIndex = htmlStr.Find(iter->endFeature);
			}
		}

		tmpDisplay += _T("\n");
	}

	m_extractionInfo.clear();
	m_displayExtractionInfo.insert(make_pair(order, tmpDisplay));
	return true;
}

CString CWebpageHandler::GetRequiredInfo(CString fileName, CString url, vector<CString> filed, bool isGroup)
{
	CMyHttpClient conn;
	CFile file0(fileName, CFile::modeCreate|CFile::modeWrite);
	CString value = conn.doGet(url); 
	file0.Write(value, value.GetLength()*2);
	file0.Close();
	return value;
}

bool CWebpageHandler::AnalyseJson(CString fileName)
{
	/*ifstream ifs;
    ifs.open(fileName);
    assert(ifs.is_open());
 
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
		return false;
 
    string name = root["id"].asString();
    int age = root["time"].asInt();*/
	return true;
}