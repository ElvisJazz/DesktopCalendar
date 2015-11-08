#if !defined(AFX_LISTCTRLLINK_H__5B5B1DAD_10B9_4C10_9C79_EAF19081A3F5__INCLUDED_)
#define AFX_LISTCTRLLINK_H__5B5B1DAD_10B9_4C10_9C79_EAF19081A3F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlLink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlLink window
#include "LinkProperties.h"
#include "LinkItem.h"

class CLinkProperties;
class CLinkItem;

class CListCtrlLink : public CListCtrl
{
// Construction
public:
	CListCtrlLink();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlLink)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlLink();

	void Init();

	int InsertItem(int nItem, LPCTSTR lpszItem, LPCTSTR lpctszUrl = NULL);

	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText, 
		LPCTSTR lpctszUrl = NULL);

//Methods
protected:

	//Internal initialization
	void InitFont(CLinkProperties link, CFont *pFont);

//Attributes
protected:
	CLinkProperties m_link;
	CLinkProperties m_noLink;
	CFont m_fontLink;
	CFont m_fontNoLink;
	HCURSOR m_hCursorArrow;
	CPoint m_pointOriginalPosition;

	CMapStringToOb m_linkMap;

protected:
	//Gets and Sets
	int GetScrollCompensation();
	CLinkItem* GetHotLink(int iRow);
	CLinkItem* GetLink(int i, int j);
	void SetLink(int i, int j, CLinkItem* pItemLink);

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlLink)
	afx_msg void OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLLINK_H__5B5B1DAD_10B9_4C10_9C79_EAF19081A3F5__INCLUDED_)
