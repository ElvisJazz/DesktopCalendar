#ifndef _CHTTPCLIENT_H
#define _CHTTPCLIENT_H

#include   "wininet.h"
#include   "afxinet.h"

class CHttpClient : public CWnd
{
public:
	void   addParam(CString   name,CString   value);
	CString   doPost(CString   href);
	CString   doGet(CString   href);
	CHttpClient();
	virtual   ~CHttpClient();
private:
	CString   CONTENT;
	int   CL;
	CStringList   values;
	CStringList   names;
};
#endif   //   !defined(AFX_HTTPCLIENT_H__EA769DCB_AAB9_47CD_BD87_FBD6913592C5__INCLUDED_)   
    