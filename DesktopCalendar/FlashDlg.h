#pragma once
#include "CShockwaveFlash.h"
#include "DlgBase.h"

// CFlashDlg �Ի���

class CFlashDlg : public CDlgBase
{
	DECLARE_DYNAMIC(CFlashDlg)

public:
	CFlashDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlashDlg();

// �Ի�������
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CShockwaveFlash m_FlashShow;
	void Open();
};
