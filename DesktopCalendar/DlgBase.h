
// DlgBase.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

//����ģʽ
#define HM_NONE	0//������
#define HM_TOP	1//��������
#define HM_BOTTOM	2//��������
#define HM_LEFT	3//��������
#define HM_RIGHT	4//��������
#define HM_ALL 6 //ȫ������������
#define CM_ELAPSE	200 //�������Ƿ��뿪���ڵ�ʱ����
#define HS_ELAPSE	5//��������ÿ����ʱ����
#define HS_STEPS	10//�������̷ֳɶ��ٲ����
#define INTERVAL	20//����ճ��ʱ�������Ļ�߽����С���,��λΪ����
#define INFALTE	10//��������ʱ����봰�ڱ߽����С���,��λΪ����
#define MINCY 540

#define STARTX 30
#define STARTY 180   
// CDlgBase �Ի���
class CDlgBase : public CDialogEx
{
// ����
public:
	CDlgBase(UINT = 0, CWnd* pParent = NULL);	// ��׼���캯��
	~CDlgBase(){ } 
	bool isFinished(){ return m_hsFinished;}
// �Ի�������
//	enum { IDD = IDD_LOVECALENDAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT  nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	/*�Զ�����ģ��*/
protected:
	//�����ƶ�ʱ���ڵĴ�С
	void FixMoving(UINT fwSide, LPRECT pRect);
	//������״̬��ʾ����
	void DoShow();
	//����ʾ״̬��������
	void DoHide();
	//���غ���,ֻ��Ϊ�˷�����ã�ʵ�ʵ���CWnd��SetWindowPos(��)
	BOOL SetWindowPos(const CWnd* pWndInsertAfter,LPCRECT pCRect, UINT nFlags = SWP_SHOWWINDOW);

protected:
	BOOL m_isSizeChanged;//���ڴ�С�Ƿ�ı���
	BOOL m_isSetTimer;//�Ƿ������˼������Timer
	int m_oldWndHeight;//�ɵĴ��ڿ��
	int m_taskBarHeight;//�������߶�
	int m_edgeHeight;//��Ե�߶�
	int m_edgeWidth;//��Ե���
	int m_hideMode;//����ģʽ
	BOOL m_hsFinished;//���ػ���ʾ�����Ƿ����
	BOOL m_hiding;//�ò���ֻ����!m_hsFinished����Ч
	BOOL m_IsBiggest;
	int m_height;
	int m_closeMode;
	//��:��������,��:������ʾ

public:
	afx_msg void OnClose();
};
