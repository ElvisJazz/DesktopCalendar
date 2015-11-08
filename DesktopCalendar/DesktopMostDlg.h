#pragma once


// CDesktopMostDlg 对话框

class CDesktopMostDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDesktopMostDlg)

public:
	CDesktopMostDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDesktopMostDlg();

// 对话框数据
	enum { IDD = IDD_DESKTOP_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT  nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 设置同步窗口参数
	void SetCalProperty(CalProperty* pDlg);
	bool InstallHook();

private:
	CStatic* m_staticBlock[8]; // 8个固定的色块
	vector<CStatic*> m_sideBlock;
	vector<CStatic*> m_dayBlock;
	
	COLORREF m_staticBlockColor; // 8个固定色块颜色
	CalProperty* m_pPro; //窗口属性

	map<CString, Memo*> *m_copyMemoMap; // 存储字块备忘录时间信息 按行列字符串代码索引时间信息
	


public: 
	// 设置色块的文字
	void SetText(void);
	void SetTaskText();
	// 初始化memo结构体并加入到map中去
	void AddMemo(int x, int y, CStatic* static0, int year, int month, int day);
	// 生成输入框并设置备忘录内容
	bool SetMemo(const CPoint &point);
	// 添加静态文本框和文字
	CStatic* NewMyStatic(UINT nID, const CRect& rect, LPCTSTR lpszText, DWORD dwStyle = SS_LEFT);
	// 更新备忘录信息和显示信息
	void UpdateInfo();
	// 释放资源
	void Release();
	// 添加额外的节日灯显示
	void AddExtralDisplay(CStatic* static0, int year, int month, int day);
	// 显示详细信息
	void DisplayInfo(const CPoint &point, bool close = false);
	// 调整窗口显示位置
	void ResetDisplayPoint(CPoint &point, CRect &rect);
	// 打开信息窗口
	void OnOpenInfoWin();
};
