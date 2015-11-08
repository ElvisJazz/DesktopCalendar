
// DlgBase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgBase.h"
#include "afxdialogex.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDlgBase �Ի���


CDlgBase::CDlgBase(UINT x, CWnd* pParent /*=NULL*/)
	: CDialogEx(x, pParent)
{
	m_isSizeChanged = FALSE;
	m_isSetTimer = FALSE;
	m_hsFinished = TRUE;
	m_hiding = FALSE;
	m_oldWndHeight = MINCY;
	m_taskBarHeight = 30;
	m_edgeHeight = 0;
	m_edgeWidth=0;
	m_hideMode = HM_NONE;
	m_closeMode = 0;

	m_IsBiggest = false;
}

void CDlgBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgBase, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOVING()
	ON_WM_NCHITTEST()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgBase ��Ϣ�������

BOOL CDlgBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;
	GetClientRect(&rect);
	m_height = rect.Height();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	ShowWindow(SW_HIDE);
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW,0);
	return false;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDlgBase::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDlgBase::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDlgBase::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*��Чģ��*/
int CDlgBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//����������߶�
	CWnd* p;
	p = this->FindWindow(CString("Shell_TrayWnd"), NULL);
	if(p != NULL)
	{
		CRect tRect;
		p->GetWindowRect(tRect);
		m_taskBarHeight = tRect.Height();
	}
	
	//ȥ���رհ���(����뻭3�������Ļ�)
	//ModifyStyle(WS_SYSMENU,NULL);
	//��ñ�Ե�߶ȺͿ��
	m_edgeHeight = GetSystemMetrics(SM_CYEDGE)/2;
	m_edgeWidth = GetSystemMetrics(SM_CXFRAME)/2;
	return 0;
}

LRESULT CDlgBase::OnNcHitTest(CPoint point)
{
	CString str;
	str.Format(_T("Mouse (%d,%d)"),point.x,point.y);
	//GetDlgItem(IDC_CURSOR)->SetWindowText(str);
	if(m_hideMode != HM_ALL &&m_hideMode != HM_NONE && !m_isSetTimer &&
	//��ֹ��곬����Ļ�ұ�ʱ���ұ����������˸
	point.x < GetSystemMetrics(SM_CXSCREEN) + INFALTE)
	{ //������ʱ,����Ǵ�����״̬����ʾ״̬����Timer
		SetTimer(1,CM_ELAPSE,NULL);
		m_isSetTimer = TRUE;
		m_hsFinished = FALSE;
		m_hiding = FALSE;
		SetTimer(2,HS_ELAPSE,NULL); //������ʾ����
	}
	CRect tRect;
	GetWindowRect(tRect);
	UINT   nHitTest   =   CDialog::OnNcHitTest(point);   
	if((nHitTest== HTCLIENT)  && (::GetAsyncKeyState(MK_LBUTTON)<0) && point.x>tRect.left && point.x<tRect.right
		&& point.y>tRect.top && point.y<tRect.top+100)   
		 nHitTest   =   HTCAPTION;   
	return   nHitTest;  
}

void CDlgBase::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1 )
	{
		POINT curPos;
		GetCursorPos(&curPos);
		CString str;
		str.Format(_T("Timer On(%d,%d)"),curPos.x,curPos.y);
		//GetDlgItem(IDC_TIMER)->SetWindowText(str);
		CRect tRect;
		//��ȡ��ʱ���ڴ�С
		GetWindowRect(tRect);
		//����tRect,�Դﵽ����뿪���ڱ���һ������Ŵ����¼�
		tRect.InflateRect(INFALTE,INFALTE);

		if(!tRect.PtInRect(curPos)) //�������뿪���������
		{
			KillTimer(1); //�رռ�����Timer
			m_isSetTimer = FALSE;
			//GetDlgItem(IDC_TIMER)->SetWindowText(CString("Timer Off"));
			m_hsFinished = FALSE;
			m_hiding = TRUE;
			SetTimer(2,HS_ELAPSE,NULL); //������������
		}
	}

	if(nIDEvent == 2)
	{
		static int num = 0;
		if(m_closeMode==1 && m_hiding)
		{
			num++;
			if(num == 1000)
			{
				num = 0;
				m_closeMode = 0;
				m_hiding = true;
			}
			else
				return;
		}

		if(m_hsFinished) //�����������ʾ���������ر�Timer
		{
			KillTimer(2);
			if(m_hideMode==HM_ALL && m_hiding)
				CDialogEx::OnCancel();
		}
		else
			m_hiding ? DoHide() : DoShow();
	}
}

void CDlgBase::FixMoving(UINT fwSide, LPRECT pRect)
{
	POINT curPos;
	GetCursorPos(&curPos);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int height = pRect->bottom - pRect->top;
	int width = pRect->right - pRect->left;

	if (curPos.y <= INTERVAL)
	{ //ճ�����ϱ�
		pRect->bottom = height - m_edgeHeight;
		pRect->top = -m_edgeHeight;
		m_hideMode = HM_TOP;
	}
	else if(curPos.y >= (screenHeight - INTERVAL - m_taskBarHeight))
	{ //ճ�����±�
		pRect->top = screenHeight - m_taskBarHeight - height;
		pRect->bottom = screenHeight - m_taskBarHeight;
		m_hideMode = HM_BOTTOM;
	}
	else if (curPos.x < INTERVAL)
	{ //ճ�������
		if(!m_isSizeChanged)
		{
		CRect tRect;
		GetWindowRect(tRect);
		m_oldWndHeight = tRect.Height();
		}
		pRect->right = width;
		pRect->left = 0;
		pRect->top = -m_edgeHeight;
		pRect->bottom = screenHeight - m_taskBarHeight;
		m_isSizeChanged = TRUE;
		m_hideMode = HM_LEFT;
	}
	else if(curPos.x >= (screenWidth - INTERVAL))
	{ //ճ�����ұ�
		if(!m_isSizeChanged)
		{
			CRect tRect;
			GetWindowRect(tRect);
			m_oldWndHeight = tRect.Height();
		}
		pRect->left = screenWidth - width;
		pRect->right = screenWidth;
		pRect->top = -m_edgeHeight;
		pRect->bottom = screenHeight - m_taskBarHeight;
		m_isSizeChanged = TRUE;
		m_hideMode = HM_RIGHT;
	}
	else
	{ //��ճ��
		if(m_isSizeChanged)
		{	//�������������,���ϳ��������ԭ����С
			//��"�϶�����ʾ����������"ֻ�й�դ���ԭ����С
			pRect->bottom = pRect->top + m_oldWndHeight;
			m_isSizeChanged = FALSE;
		}
		if(m_isSetTimer)
		{ //���Timer������,��ر�֮
			if(KillTimer(1) == 1)
				m_isSetTimer = FALSE;
		}
		m_hideMode = HM_NONE;
	//	GetDlgItem(IDC_TIMER)->SetWindowText(CString("Timer off"));
	}
}

void CDlgBase::DoHide()
{
	if(m_hideMode == HM_NONE)
		return;

	CRect tRect;
	GetWindowRect(tRect);
	int height = tRect.Height();
	int width = tRect.Width();
	int steps = 0;
	
	switch(m_hideMode)
	{
	case HM_TOP:
				steps = height/HS_STEPS;
				tRect.bottom -= steps;
				if(tRect.bottom <= m_edgeWidth)
				{ //����԰�����һ���滻����� ...+=|-=steps �ﵽȡ������Ч��
				//���õİ취����Ӹ�BOOLֵ������,����caseͬ��.
					tRect.bottom = m_edgeWidth;
					m_hsFinished = TRUE; //������ع���
				}
				tRect.top = tRect.bottom - height;
				break;
	case HM_BOTTOM:
				steps = height/HS_STEPS;
				tRect.top += steps;
				if(tRect.top >= (GetSystemMetrics(SM_CYSCREEN) - m_edgeWidth))
				{
					tRect.top = GetSystemMetrics(SM_CYSCREEN) - m_edgeWidth;
					m_hsFinished = TRUE;
				}
				tRect.bottom = tRect.top + height;
				break;
	case HM_LEFT:
				steps = width/HS_STEPS;
				tRect.right -= steps;
				if(tRect.right <= m_edgeWidth)
				{
					tRect.right = m_edgeWidth;
					m_hsFinished = TRUE;
				}
				tRect.left = tRect.right - width;
				tRect.top = -m_edgeHeight;
				tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
				break;
	case HM_RIGHT:
				steps = width/HS_STEPS;
				tRect.left += steps;
				if(tRect.left >= (GetSystemMetrics(SM_CXSCREEN) - m_edgeWidth))
				{
					tRect.left = GetSystemMetrics(SM_CXSCREEN) - m_edgeWidth;
					m_hsFinished = TRUE;
				}
				tRect.right = tRect.left + width;
				tRect.top = -m_edgeHeight;
				tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
				break;
	case HM_ALL:
				steps = m_height/HS_STEPS/4;
			    tRect.bottom -= steps;
				if(m_closeMode == 2)
					tRect.top += steps;
				//SetLayeredWindowAttributes(RGB(255,235,205), height/m_height*255, 2); 
				if(tRect.bottom - tRect.top <= 10)
				{
					m_hsFinished = TRUE;
					//CDialogEx::OnClose();
				}
				break;
	default:
				break;	
	}																																
	SetWindowPos(&wndTopMost,tRect);
}

void CDlgBase::DoShow()
{
	if(m_hideMode == HM_NONE)
		return;
	CRect tRect;
	GetWindowRect(tRect);
	int height = tRect.Height();
	int width = tRect.Width();
	int steps = 0;
	
	switch(m_hideMode)
	{
	case HM_TOP:
				steps = height/HS_STEPS;
				tRect.top += steps;
				if(tRect.top >= -m_edgeHeight)
				{ //����԰�����һ���滻����� ...+=|-=steps �ﵽȡ������Ч��
					//���õİ취����Ӹ�BOOLֵ������,����caseͬ��.
					tRect.top = -m_edgeHeight;
					m_hsFinished = TRUE; //�����ʾ����
				}
				tRect.bottom = tRect.top + height;
				break;
	case HM_BOTTOM:
				steps = height/HS_STEPS;
				tRect.top -= steps;
				if(tRect.top <= (GetSystemMetrics(SM_CYSCREEN) - height))
				{
					tRect.top = GetSystemMetrics(SM_CYSCREEN) - height;
					m_hsFinished = TRUE;
				}
				tRect.bottom = tRect.top + height;
				break;
	case HM_LEFT:
				steps = width/HS_STEPS;
				tRect.right += steps;
				if(tRect.right >= width)
				{
					tRect.right = width;
					m_hsFinished = TRUE;
				}
				tRect.left = tRect.right - width;
				tRect.top = -m_edgeHeight;
				tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
				break;
	case HM_RIGHT:
				steps = width/HS_STEPS;
				tRect.left -= steps;
				if(tRect.left <= (GetSystemMetrics(SM_CXSCREEN) - width))
				{
					tRect.left = GetSystemMetrics(SM_CXSCREEN) - width;
					m_hsFinished = TRUE;
				}
				tRect.right = tRect.left + width;
				tRect.top = -m_edgeHeight;
				tRect.bottom = GetSystemMetrics(SM_CYSCREEN) - m_taskBarHeight;
				break;
	case HM_ALL:steps = m_height/HS_STEPS/4;
			    tRect.bottom += steps;
				//SetLayeredWindowAttributes(RGB(255,235,205), height/m_height*255, 2); 
				if(tRect.bottom - tRect.top >= m_height-5)
				{
					m_hsFinished = TRUE;
					
					if(m_closeMode == 1)
					{
						m_hsFinished = FALSE;
						m_hideMode = HM_ALL;
						m_hiding = TRUE;
						SetTimer(2, HS_ELAPSE, NULL);
					}
				}
				break;
	default:
				break;
	}
	SetWindowPos(&wndTopMost,tRect);
}

BOOL CDlgBase::SetWindowPos(const CWnd* pWndInsertAfter, LPCRECT pCRect, UINT nFlags)
{
	return CDialog::SetWindowPos(pWndInsertAfter,pCRect->left, pCRect->top,
		pCRect->right - pCRect->left, pCRect->bottom - pCRect->top, nFlags);
}

void CDlgBase::OnMoving(UINT fwSide, LPRECT pRect)
{
	FixMoving(fwSide, pRect);
	CDialog::OnMoving(fwSide, pRect);
}


void CDlgBase::OnClose()
{
	if(IsWindowVisible())
	{
		m_closeMode = 2;
		m_hsFinished = FALSE;
		m_hideMode = HM_ALL;
		m_hiding = TRUE;
		SetTimer(2, HS_ELAPSE, NULL);
	}
	else
		CDialogEx::OnCancel();
}