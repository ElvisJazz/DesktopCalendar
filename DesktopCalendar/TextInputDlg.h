#pragma once
#include "afxcolorbutton.h"
#include "afxwin.h"


// CTextInputDlg �Ի���

class CTextInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTextInputDlg)

public:
	CTextInputDlg(CalProperty* pPro, const CString& str, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTextInputDlg();

// �Ի�������
	enum { IDD = IDD_TEXTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_text;
	CMFCColorButton m_colorCtr;
	CString m_strCaption;
	CString m_origText;
	bool m_close;
	// ָ���ֿ�ı�����ɫ
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
