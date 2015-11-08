#pragma once
#include "afxbutton.h"
#include "MyButton.h"
#include <vector>
// CDesktopDlg 对话框

class CDesktopDlg : public CDlgBase
{
	DECLARE_DYNAMIC(CDesktopDlg)

public:
	CDesktopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDesktopDlg();
	virtual BOOL OnInitDialog();


// 对话框数据
	enum { IDD = IDD_DESKTOP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT  nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	CMyButton* m_staticBlock[8]; // 8个固定的色块
	vector<CMyButton*> m_sideBlock;
	vector<CMyButton*> m_dayBlock;
	
	COLORREF m_staticBlockColor; // 固定色块颜色
	COLORREF m_currentBlockColor; // 当前日期颜色
    CalProperty* m_pPro; //窗口属性

public:
	// 开启毛玻璃效果
	void ControlAeroStyle();
	// 设置色块
	void SetBlock(void);
	void SetTaskBlock();
	// 动态添加我的按钮
	CMyButton* NewMyButton(int nID, CRect rect, int nStyle);
	// 设置窗口参数
	void SetCalProperty(CalProperty* pCal);
	// 释放资源
	void Release();
	// 读取备忘录信息
	void ReadInfo(int year);
};

