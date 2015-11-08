#pragma once

#include "WebpageHandler.h"
#include "ListCtrlLink.h"
#include "afxwin.h"

// CMySubscription �Ի���

class CMySubscription : public CDialogEx
{
	DECLARE_DYNAMIC(CMySubscription)

public:
	CMySubscription(CalProperty* pro, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMySubscription();
	void GetDisplayExtractionInfo(CStringArray &url, int size);
	void DisplayRequiredInfo();

// �Ի�������
	enum { IDD = IDD_SUBSCRIPTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CWebpageHandler m_pageHandler;
	CalProperty* m_pro;
	CRect m_rect;
public:
	CListCtrlLink m_listCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefreshAll();
	afx_msg void OnSelectSubscription();
	CComboBox m_subCtr;
};
