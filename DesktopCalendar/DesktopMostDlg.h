#pragma once


// CDesktopMostDlg �Ի���

class CDesktopMostDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDesktopMostDlg)

public:
	CDesktopMostDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDesktopMostDlg();

// �Ի�������
	enum { IDD = IDD_DESKTOP_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT  nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// ����ͬ�����ڲ���
	void SetCalProperty(CalProperty* pDlg);
	bool InstallHook();

private:
	CStatic* m_staticBlock[8]; // 8���̶���ɫ��
	vector<CStatic*> m_sideBlock;
	vector<CStatic*> m_dayBlock;
	
	COLORREF m_staticBlockColor; // 8���̶�ɫ����ɫ
	CalProperty* m_pPro; //��������

	map<CString, Memo*> *m_copyMemoMap; // �洢�ֿ鱸��¼ʱ����Ϣ �������ַ�����������ʱ����Ϣ
	


public: 
	// ����ɫ�������
	void SetText(void);
	void SetTaskText();
	// ��ʼ��memo�ṹ�岢���뵽map��ȥ
	void AddMemo(int x, int y, CStatic* static0, int year, int month, int day);
	// ������������ñ���¼����
	bool SetMemo(const CPoint &point);
	// ��Ӿ�̬�ı��������
	CStatic* NewMyStatic(UINT nID, const CRect& rect, LPCTSTR lpszText, DWORD dwStyle = SS_LEFT);
	// ���±���¼��Ϣ����ʾ��Ϣ
	void UpdateInfo();
	// �ͷ���Դ
	void Release();
	// ��Ӷ���Ľ��յ���ʾ
	void AddExtralDisplay(CStatic* static0, int year, int month, int day);
	// ��ʾ��ϸ��Ϣ
	void DisplayInfo(const CPoint &point, bool close = false);
	// ����������ʾλ��
	void ResetDisplayPoint(CPoint &point, CRect &rect);
	// ����Ϣ����
	void OnOpenInfoWin();
};
