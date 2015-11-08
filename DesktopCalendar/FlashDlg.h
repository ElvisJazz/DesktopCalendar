#pragma once
#include "CShockwaveFlash.h"
#include "DlgBase.h"

// CFlashDlg 对话框

class CFlashDlg : public CDlgBase
{
	DECLARE_DYNAMIC(CFlashDlg)

public:
	CFlashDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlashDlg();

// 对话框数据
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CShockwaveFlash m_FlashShow;
	void Open();
};
