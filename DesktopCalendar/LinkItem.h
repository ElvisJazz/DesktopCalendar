// LinkItem.h: interface for the CLinkItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKITEM_H__B8534FCE_F519_4F1B_A091_95A6CE390BE3__INCLUDED_)
#define AFX_LINKITEM_H__B8534FCE_F519_4F1B_A091_95A6CE390BE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LinkProperties.h"

class CLinkProperties;

class CLinkItem : public CObject  
{
public:
	CLinkItem();
	virtual ~CLinkItem();

//Attributes
public:
	BOOL GetVisited() { return m_bVisited; }
	void SetVisited(BOOL bVisited) { m_bVisited = bVisited; }
	
	CString GetUrl() { return m_strUrl; }
	void SetUrl(CString strUrl) { m_strUrl = strUrl; }

	CLinkProperties* GetLinkProperties() { return m_pLinkProp;}
	void SetLinkProperties(CLinkProperties* pLinkProp) { m_pLinkProp = pLinkProp;}

	BOOL GetUnderlinedLink();

	HCURSOR GetCursor();

	COLORREF GetLinkColor(); 

protected:
	BOOL m_bVisited;
	CString m_strUrl;
	CLinkProperties* m_pLinkProp;
};

#endif // !defined(AFX_LINKITEM_H__B8534FCE_F519_4F1B_A091_95A6CE390BE3__INCLUDED_)
