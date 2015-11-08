#pragma once
#include "CSpreadSheet.h"

// CFileOperation

class CFileOperation : public CWnd
{
	DECLARE_DYNAMIC(CFileOperation)

public:
	CFileOperation();
	virtual ~CFileOperation();

protected:
	DECLARE_MESSAGE_MAP()
	
public:
	int      m_NumOfArray;
	//int     *m_MarkInfo; // 以二进制的方式来看标志信息的种类情况0000—1111
	int     *m_Month;
	int     *m_Day;

	// 预读取信息
	void PreRead(int year, void* pro, bool create=true);
	// 创建新信息表
	void Create();
	// 获取甜蜜记忆
	//CString GetLoveMemory(int index);
	// 获取备忘录
	CString GetMemorandum(int index);
	//CString GetSweetMessage(int index);
	// 添加函数模块
	void AddMessage(int month, int day, bool check1, bool check2, CString s1, CString s2, CString s3, COLORREF color);
private:
	//CString *m_Memory1;
	CString *m_Memory2;
	//CString *m_SweetMessage;
	//CString *m_ExtralSM;
	CString  m_RecordName;
	bool     m_EnableAdd;
};


