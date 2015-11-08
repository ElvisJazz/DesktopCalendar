#pragma once
#include "afxwin.h"

// CDisplayExDlg �Ի���

class CDisplayExDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplayExDlg)

public:
	CDisplayExDlg(CalProperty *cal, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDisplayExDlg();
	void ReadExtractionINI();
	void SetProperty();
    void WriteExtractionINI(CString fileName, CString content);

// �Ի�������
	enum { IDD = IDD_TAB2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CComboBox m_subCtr;
	afx_msg void OnDeleteSub();
	afx_msg void OnAddSub();
	afx_msg void OnIntroduction();
	afx_msg void OnModify();
	afx_msg void OnSubmit();
	afx_msg void OnCancel();
	afx_msg void OnAddTemplate();

private:
	CalProperty *m_cal;
	int m_mode; // ��1,ɾ2,��3,����0
	int m_numOfSub;
	vector<bool> m_isValid;
	int m_index;
	CString m_content;
	bool m_isAdd;

public:
	CString m_subName;
	CString m_subAddress;
};
