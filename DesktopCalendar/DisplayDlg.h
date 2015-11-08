#pragma once
#include "afxwin.h"
#include "afxcolorbutton.h"


// CDisplayDlg �Ի���

class CDisplayDlg : public CDialog
{
	DECLARE_DYNAMIC(CDisplayDlg)

public:
	CDisplayDlg(CalProperty *cal, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDisplayDlg();
	void Init();
	void SetFrame();
	void SetProperty();
	bool SetAutoRun();

// �Ի�������
	enum { IDD = IDD_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	// ��������ָ��
	CalProperty* m_cal;
	/* �������*/
	BOOL m_isDisplayWeek;
	int  m_firstWeek;
	int  m_displayMode;

	int  m_maxRows;
	int  m_currentGoneRows;

	COLORREF m_commonColor;
	COLORREF m_currentColor;
	int  m_transparentLever;
	int  m_grap;

	CFont *m_font; 
	COLORREF m_color;
	bool m_isSetFont;

	// ��������ѡ��
	int  m_startXinqi;
	COLORREF m_holidayColor;
	bool m_isHoliday[7];

	bool m_displayMore[3];

public:
	afx_msg void OnSetFirstWeek();
	afx_msg void OnSetMore(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CComboBox m_displayModeCtr;
	CComboBox m_maxRowCtr;
	CComboBox m_hasGoneRowCtr;
	CComboBox m_transparentLeverCtr;
	CComboBox m_grapCtr;
	CMFCColorButton m_commonColorCtr;
public:
	afx_msg void OnReturnDefaultSet();
	afx_msg void OnSetCommonFont();
	afx_msg void OnSetMaxRows();
	afx_msg void OnSetDisplayMode();
	CMFCColorButton m_currentColorCtr;
	// �Ƿ񿪻�������
	BOOL m_isAutoRun;
	afx_msg void OnSetAutoRun();
};
