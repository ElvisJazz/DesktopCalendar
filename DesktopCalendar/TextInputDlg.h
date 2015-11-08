#pragma once
#include "afxcolorbutton.h"
#include "afxwin.h"


// CTextInputDlg 对话框

class CTextInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTextInputDlg)

public:
	CTextInputDlg(CalProperty* pPro, const CString& str, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTextInputDlg();

// 对话框数据
	enum { IDD = IDD_TEXTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_text;
	CMFCColorButton m_colorCtr;
	CString m_strCaption;
	CString m_origText;
	bool m_close;
	// 指定字块的背景颜色
	COLORREF m_blockColor;
	CalProperty* m_pPro;

public:
	virtual BOOL OnInitDialog();
	bool IsClose() { return m_close;}
	void SetText(CString s){ m_text = s;}

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit();
};
