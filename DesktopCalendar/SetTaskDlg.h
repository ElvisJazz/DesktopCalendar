#pragma once


// CSetTaskDlg �Ի���

class CSetTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetTaskDlg)

public:
	CSetTaskDlg(CalProperty *cal, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetTaskDlg();

// �Ի�������
	enum { IDD = IDD_TAB4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void SetProperty();
private:
	// ��������ָ��
	CalProperty* m_cal;
	// ��һ��������
	CString m_firstTask;
	// �ڶ���������
	CString m_secondTask;
	// ������������
	CString m_thirdTask;
	// ������������
	CString m_fourthTask;
public:
	virtual BOOL OnInitDialog();
};
