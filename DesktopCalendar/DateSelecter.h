#pragma once
#include "afxdtctl.h"


// CDateSelecter 对话框

class CDateSelecter : public CDialogEx
{
	DECLARE_DYNAMIC(CDateSelecter)

public:
	CDateSelecter(CalProperty *calPro, int *firstWeek, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDateSelecter();


// 对话框数据
	enum { IDD = IDD_DATEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CMonthCalCtrl m_cal;
	CalProperty *m_calPro;
	int *m_firstWeek;
public:
	afx_msg void OnBnClickedOk();
};
