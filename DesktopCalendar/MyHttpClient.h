#pragma once


// CMyHttpClient

class CMyHttpClient : public CWnd
{
	DECLARE_DYNAMIC(CMyHttpClient)

public:
	void   addParam(CString   name,CString   value);
	CString   doPost(CString   href);
	CString   doGet(CString   href);
	CMyHttpClient();
	virtual ~CMyHttpClient();
private:
	CString   CONTENT;
	int   CL;
	CStringList   values;
	CStringList   names;

protected:
	DECLARE_MESSAGE_MAP()
};


