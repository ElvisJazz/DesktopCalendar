#pragma once
#include "afxwin.h"
#include "afxcolorbutton.h"


// CMoreSetDlg �Ի���

class CMoreSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoreSetDlg)

public:
	CMoreSetDlg(int* startXinqi, COLORREF* holidayColor, bool* holiday, bool* displayMore, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMoreSetDlg();

// �Ի�������
	enum { IDD = IDD_MORE_SET_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	int* m_startXinqi;
	COLORREF* m_holidayColor;
	bool* m_holiday;
	bool* m_displayMore;
public:
	virtual BOOL OnInitDialog();
private:
	CComboBox m_startXinqiCtr;
	CMFCColorButton m_holidayColorCtr;
	BOOL m_week0;
	BOOL m_week1;
	BOOL m_week2;
	BOOL m_week3;
	BOOL m_week4;
	BOOL m_week5;
	BOOL m_week6;
	BOOL m_displayFestival;
	BOOL m_displayAgrCal;
	BOOL m_displayJieqi;
public:
	afx_msg void OnBnClickedOk();
};
