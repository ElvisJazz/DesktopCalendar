#pragma once


// CDrawerDlg �Ի���

class CDrawerDlg : public CDlgBase
{
	DECLARE_DYNAMIC(CDrawerDlg)

public:
	CDrawerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDrawerDlg();

// �Ի�������
	enum { IDD = IDD_DRAWER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// ��̬����ҵİ�ť
	CMyButton* NewMyButton(int nID, CRect rect, int nStyle);
	HBRUSH m_hBrush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
