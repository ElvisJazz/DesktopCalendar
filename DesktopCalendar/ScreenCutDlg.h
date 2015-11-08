#pragma once
#include "afxwin.h"

class CDesktopCalendarDlg;
// CScreenCutDlg �Ի���

class CScreenCutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenCutDlg)

public:
	CScreenCutDlg(CDesktopCalendarDlg *dlg, CWnd* pParent = NULL);   // ��׼���캯��
	CScreenCutDlg(CWnd* pParent = NULL);
	virtual ~CScreenCutDlg();

// �Ի�������
	enum { IDD = IDD_SCREEN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void CutDesktop(void);
	bool SaveBmp(HBITMAP hbitmap, LPCWSTR filename, int nColor) ;
	void FixMoving(UINT fwSide, LPRECT pRect);
	static void drawBitmapFormFile(CString BitmapFilePath, CDC *pDC,CRect rect, int order);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);

private:
	CDesktopCalendarDlg *m_dlg;
	CRect m_rect;
	bool m_displayInfo;
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	// �޸���ʾ����
	void SetProperty(void);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
