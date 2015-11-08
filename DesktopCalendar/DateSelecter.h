#pragma once
#include "afxdtctl.h"


// CDateSelecter �Ի���

class CDateSelecter : public CDialogEx
{
	DECLARE_DYNAMIC(CDateSelecter)

public:
	CDateSelecter(CalProperty *calPro, int *firstWeek, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDateSelecter();


// �Ի�������
	enum { IDD = IDD_DATEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CMonthCalCtrl m_cal;
	CalProperty *m_calPro;
	int *m_firstWeek;
public:
	afx_msg void OnBnClickedOk();
};
