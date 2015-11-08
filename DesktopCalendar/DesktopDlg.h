#pragma once
#include "afxbutton.h"
#include "MyButton.h"
#include <vector>
// CDesktopDlg �Ի���

class CDesktopDlg : public CDlgBase
{
	DECLARE_DYNAMIC(CDesktopDlg)

public:
	CDesktopDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDesktopDlg();
	virtual BOOL OnInitDialog();


// �Ի�������
	enum { IDD = IDD_DESKTOP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT  nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	CMyButton* m_staticBlock[8]; // 8���̶���ɫ��
	vector<CMyButton*> m_sideBlock;
	vector<CMyButton*> m_dayBlock;
	
	COLORREF m_staticBlockColor; // �̶�ɫ����ɫ
	COLORREF m_currentBlockColor; // ��ǰ������ɫ
    CalProperty* m_pPro; //��������

public:
	// ����ë����Ч��
	void ControlAeroStyle();
	// ����ɫ��
	void SetBlock(void);
	void SetTaskBlock();
	// ��̬����ҵİ�ť
	CMyButton* NewMyButton(int nID, CRect rect, int nStyle);
	// ���ô��ڲ���
	void SetCalProperty(CalProperty* pCal);
	// �ͷ���Դ
	void Release();
	// ��ȡ����¼��Ϣ
	void ReadInfo(int year);
};

