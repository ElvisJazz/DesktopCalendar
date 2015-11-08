#pragma once

// ����������Ϣ
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

	CString name; // ����
	CString content; // ��ʾ����
	CString music; // ���ֱ��

	bool isChanged; // �Ƿ��޸���
	bool isTerminate; // �Ƿ���ֹ
	map<info0*, CWinThread*> *mapPointer;
} taskInfo;
// CAdvanceDlg �Ի���

class CAdvanceDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdvanceDlg)

public:
	CAdvanceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdvanceDlg();

// �Ի�������
	enum { IDD = IDD_TAB3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	// ��������ִ��ʱ��
	afx_msg void OnSetRunTime();
	afx_msg void OnSetTask();
	afx_msg void OnStnClickedStatic5();
	afx_msg void SubmitTask();
	afx_msg void DeleteTask();

private:
	map<taskInfo*, CWinThread*> m_taskInfoMap;
public:
	// ������������Ƿ���ȷ
	bool CheckTaskSet(int type);
	// ����ʱ��
	void SetTime();
private:
	// �»�Сʱ����
	int m_monthOrHour;
	// �����ӱ���
	int m_dayOrMinute;
	// ʱ����
	int m_hour;
	// �ֱ���
	int m_minute;
	// ��ʾ����
	CString m_content;
	// ����
	CString m_caption;
public:
	afx_msg void OnTip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTryMusic();
	afx_msg void OnMusicList();
private:
	// ���г����ַ
	CString m_pathOfProgram;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
