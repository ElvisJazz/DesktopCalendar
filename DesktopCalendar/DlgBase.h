
// DlgBase.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

//收缩模式
#define HM_NONE	0//不收缩
#define HM_TOP	1//向上收缩
#define HM_BOTTOM	2//向下收缩
#define HM_LEFT	3//向左收缩
#define HM_RIGHT	4//向右收缩
#define HM_ALL 6 //全部向中心收缩
#define CM_ELAPSE	200 //检测鼠标是否离开窗口的时间间隔
#define HS_ELAPSE	5//伸缩过程每步的时间间隔
#define HS_STEPS	10//伸缩过程分成多少步完成
#define INTERVAL	20//触发粘附时鼠标与屏幕边界的最小间隔,单位为象素
#define INFALTE	10//触发收缩时鼠标与窗口边界的最小间隔,单位为象素
#define MINCY 540

#define STARTX 30
#define STARTY 180   
// CDlgBase 对话框
class CDlgBase : public CDialogEx
{
// 构造
public:
	CDlgBase(UINT = 0, CWnd* pParent = NULL);	// 标准构造函数
	~CDlgBase(){ } 
	bool isFinished(){ return m_hsFinished;}
// 对话框数据
//	enum { IDD = IDD_LOVECALENDAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT  nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	/*自动伸缩模块*/
protected:
	//修正移动时窗口的大小
	void FixMoving(UINT fwSide, LPRECT pRect);
	//从收缩状态显示窗口
	void DoShow();
	//从显示状态收缩窗口
	void DoHide();
	//重载函数,只是为了方便调用，实际调用CWnd的SetWindowPos(…)
	BOOL SetWindowPos(const CWnd* pWndInsertAfter,LPCRECT pCRect, UINT nFlags = SWP_SHOWWINDOW);

protected:
	BOOL m_isSizeChanged;//窗口大小是否改变了
	BOOL m_isSetTimer;//是否设置了检测鼠标的Timer
	int m_oldWndHeight;//旧的窗口宽度
	int m_taskBarHeight;//任务栏高度
	int m_edgeHeight;//边缘高度
	int m_edgeWidth;//边缘宽度
	int m_hideMode;//隐藏模式
	BOOL m_hsFinished;//隐藏或显示过程是否完成
	BOOL m_hiding;//该参数只有在!m_hsFinished才有效
	BOOL m_IsBiggest;
	int m_height;
	int m_closeMode;
	//真:正在隐藏,假:正在显示

public:
	afx_msg void OnClose();
};
