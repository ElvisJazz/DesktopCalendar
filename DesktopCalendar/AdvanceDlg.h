#pragma once

// 定义任务信息
typedef struct info0
{
	enum timeType{SPECIALTIME, AFTERTIME, EVERYTIME};
	enum timeType type0;
	enum taskType{LOGOFF, SHUTDOWN, REBOOT, CLOCK, PROGRAM};
	enum taskType type1;
	int month;
	int day;
	int hour;
	int minute;

	CString name; // 名称
	CString content; // 提示内容
	CString music; // 音乐标号

	bool isChanged; // 是否被修改了
	bool isTerminate; // 是否终止
	map<info0*, CWinThread*> *mapPointer;
} taskInfo;
// CAdvanceDlg 对话框

class CAdvanceDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdvanceDlg)

public:
	CAdvanceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdvanceDlg();

// 对话框数据
	enum { IDD = IDD_TAB3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	// 设置任务执行时间
	afx_msg void OnSetRunTime();
	afx_msg void OnSetTask();
	afx_msg void OnStnClickedStatic5();
	afx_msg void SubmitTask();
	afx_msg void DeleteTask();

private:
	map<taskInfo*, CWinThread*> m_taskInfoMap;
public:
	// 检查任务设置是否正确
	bool CheckTaskSet(int type);
	// 设置时间
	void SetTime();
private:
	// 月或小时变量
	int m_monthOrHour;
	// 天或分钟变量
	int m_dayOrMinute;
	// 时变量
	int m_hour;
	// 分变量
	int m_minute;
	// 提示内容
	CString m_content;
	// 标题
	CString m_caption;
public:
	afx_msg void OnTip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTryMusic();
	afx_msg void OnMusicList();
private:
	// 运行程序地址
	CString m_pathOfProgram;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
