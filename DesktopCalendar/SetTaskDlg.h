#pragma once


// CSetTaskDlg 对话框

class CSetTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetTaskDlg)

public:
	CSetTaskDlg(CalProperty *cal, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetTaskDlg();

// 对话框数据
	enum { IDD = IDD_TAB4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void SetProperty();
private:
	// 日历属性指针
	CalProperty* m_cal;
	// 第一优先任务
	CString m_firstTask;
	// 第二优先任务
	CString m_secondTask;
	// 第三优先任务
	CString m_thirdTask;
	// 第四优先任务
	CString m_fourthTask;
public:
	virtual BOOL OnInitDialog();
};
