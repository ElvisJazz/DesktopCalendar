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
	//int     *m_MarkInfo; // �Զ����Ƶķ�ʽ������־��Ϣ���������0000��1111
	int     *m_Month;
	int     *m_Day;

	// Ԥ��ȡ��Ϣ
	void PreRead(int year, void* pro, bool create=true);
	// ��������Ϣ��
	void Create();
	// ��ȡ���ۼ���
	//CString GetLoveMemory(int index);
	// ��ȡ����¼
	CString GetMemorandum(int index);
	//CString GetSweetMessage(int index);
	// ��Ӻ���ģ��
	void AddMessage(int month, int day, bool check1, bool check2, CString s1, CString s2, CString s3, COLORREF color);
private:
	//CString *m_Memory1;
	CString *m_Memory2;
	//CString *m_SweetMessage;
	//CString *m_ExtralSM;
	CString  m_RecordName;
	bool     m_EnableAdd;
};


