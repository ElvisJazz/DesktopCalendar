#pragma once


// CDrawerDlg 对话框

class CDrawerDlg : public CDlgBase
{
	DECLARE_DYNAMIC(CDrawerDlg)

public:
	CDrawerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDrawerDlg();

// 对话框数据
	enum { IDD = IDD_DRAWER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 动态添加我的按钮
	CMyButton* NewMyButton(int nID, CRect rect, int nStyle);
	HBRUSH m_hBrush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
