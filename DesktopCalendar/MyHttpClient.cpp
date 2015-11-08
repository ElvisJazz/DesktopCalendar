// MyHttpClient.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "MyHttpClient.h"

#include   "wininet.h"
#include   "afxinet.h"

// CMyHttpClient

IMPLEMENT_DYNAMIC(CMyHttpClient, CWnd)

CMyHttpClient::CMyHttpClient()
{

}

CMyHttpClient::~CMyHttpClient()
{
}


CString CMyHttpClient::doGet(CString href)
{
	CString httpsource=_T("");
	CInternetSession session1(NULL,0);
	CHttpFile* pHTTPFile=NULL;
	try{
		pHTTPFile = (CHttpFile*)session1.OpenURL(href);
	}catch(CInternetException){
		pHTTPFile=NULL;
	}
	
	if(pHTTPFile)
	{
		CString text;
		for(int i=0;pHTTPFile->ReadString(text);i++)
		{
			httpsource = httpsource + text + _T("\r\n");
		}
		pHTTPFile->Close();
		delete   pHTTPFile;
	}
	
	return   httpsource;
}

CString CMyHttpClient::doPost(CString href)
{
	CString   httpsource=_T("");
	CInternetSession   session1;
	CHttpConnection*   conn1=NULL;
	CHttpFile*   pFile   =   NULL;
	CString   strServerName;
	CString   strObject;
	INTERNET_PORT   nPort;
	DWORD   dwServiceType;
	AfxParseURL((LPCTSTR)href, dwServiceType, strServerName, strObject, nPort);
	DWORD   retcode;
	wchar_t *wStr = CONTENT.GetBuffer(1000);
	size_t len = wcslen(wStr) + 1;
	size_t converted = 0;
	char*   outBuff;
	outBuff = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, outBuff, len, wStr, _TRUNCATE);
	
	try
	{
		conn1 = session1.GetHttpConnection(strServerName,nPort);
		pFile = conn1->OpenRequest(0,strObject,NULL,1,NULL,_T("HTTP/1.1"),INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_AUTO_REDIRECT);
		pFile->AddRequestHeaders(_T("Content-Type:   application/x-www-form-urlencoded"));
		pFile->AddRequestHeaders(_T("Accept:   */*"));
		pFile->SendRequest(NULL,0,outBuff,strlen(outBuff)+1);
		pFile->QueryInfoStatusCode(retcode);
	}catch (CInternetException* e){};
	
	if(pFile)
	{
		CString text;
		for(int i=0;pFile->ReadString(text);i++)
		{
			httpsource=httpsource+text+_T("\r\n");
		}
		pFile->Close();
		delete   pFile;
		delete   conn1;
		session1.Close();
	}
	
	return   httpsource;   
  } 

void   CMyHttpClient::addParam(CString   name,   CString   value)
{
	names.AddTail((LPCTSTR)name);
	values.AddTail((LPCTSTR)value);
	CString   eq= _T("=");
	CString   an= _T("&");
	CONTENT=CONTENT+name+eq+value+an;
	CL=CONTENT.GetLength();
}  
BEGIN_MESSAGE_MAP(CMyHttpClient, CWnd)
END_MESSAGE_MAP()



// CMyHttpClient 消息处理程序


