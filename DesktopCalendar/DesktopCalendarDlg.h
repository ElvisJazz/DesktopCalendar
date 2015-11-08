
// DesktopCalendarDlg.h : 头文件
//
#ifndef _DESKTOPCALENDARDLG_H
#define _DESKTOPCALENDARDLG_H 

#pragma once
#include "DlgBase.h"
#include "FlashDlg.h"
#include "DrawerDlg.h"
#include "DesktopDlg.h"
#include "DesktopMostDlg.h"
#include "DisplayDlg.h"
#include "DisplayExDlg.h"
#include "AdvanceDlg.h"
#include "ScreenCutDlg.h"
#include "SetTaskDlg.h"
#include "afxcmn.h"

// CDesktopCalendarDlg 对话框
class CDesktopCalendarDlg : public CDlgBase
{
// 构造
public:
	CDesktopCalendarDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDesktopCalendarDlg();

// 对话框数据
	enum { IDD = IDD_DESKTOPCALENDAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	NOTIFYICONDATA m_notify; 
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//afx_msg LRESULT OnNotifyMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDesktopMode();
	afx_msg void OnDrawerMode();
	afx_msg void OnSetProperty();
	afx_msg void OnHideOrShow();
	afx_msg void OnLastPage();
	afx_msg void OnNextPage();
	afx_msg void OnReturnToday();
	afx_msg void OnSelectDate();
	afx_msg void OnAbout();
	afx_msg void OnExit();
	afx_msg void OnBnClickedButton1();

	CFlashDlg *m_dlg;
private:
	CDesktopDlg *m_dlg2;
	CDesktopMostDlg *m_dlg02;
	HBITMAP m_pBackBitmap;
	HANDLE m_pNewThread;
	CalProperty* m_calPro;

	CRect m_rect;

public:
	CDesktopDlg* GetDesktopDlg(void);
	void SetProperty();
	CalProperty* GetProperty();
	// 刷新日历
	void Fresh(void);
	// 获取截屏矩阵
	CRect GetDisplayRect(void);
	// 开启日历
    void OpenCal();
	// 删除抽屉式日历
	bool ReleaseDrawerDlg(CString name);
	// 检测进程是否存在
	HANDLE CheckProcess(CString name);
private:
	// // 菜单控制
	CTabCtrl m_tab;
	CDisplayDlg *m_displayDlg;
	CDisplayExDlg *m_displayExDlg;
	CAdvanceDlg *m_advanceDlg;
	CSetTaskDlg *m_taskDlg;
	CScreenCutDlg *m_screenDlg;
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOpenInfoWin();
	afx_msg void OnOpenAni();
	afx_msg void OnOpenSubscription();
	afx_msg void OnTaskMode();
};
#endif