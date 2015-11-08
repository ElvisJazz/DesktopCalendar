// LinkProperties.h: interface for the CLinkProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKPROPERTIES_H__7C27ADE8_AF75_472C_974B_CEBE80A3C924__INCLUDED_)
#define AFX_LINKPROPERTIES_H__7C27ADE8_AF75_472C_974B_CEBE80A3C924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_UNVISITED_COLOR RGB(0, 0, 255)
#define DEFAULT_VISITED_COLOR RGB(128,0,128)
#define DEFAULT_UNDERLINED TRUE
#define DEFAULT_HOTSPOT_CURSOR NULL

class CLinkProperties  
{
public:
	CLinkProperties();
	virtual ~CLinkProperties();

	void SetLinkProperties(COLORREF clrLink = DEFAULT_UNVISITED_COLOR, 
		COLORREF clrVisited = DEFAULT_VISITED_COLOR, 
		HCURSOR hCursorHotSpot = DEFAULT_HOTSPOT_CURSOR, 
		BOOL bUnderlinedLink = DEFAULT_UNDERLINED);

	BOOL		GetUnderlinedLink() {return m_bUnderlinedLink;}

	COLORREF	GetLinkColor() { return m_clrLink;}

	COLORREF	GetVisitedLinkColor() { return m_clrVisited;}

	HCURSOR		GetCursor() { return m_hCursorHotSpot;}

private:
	BOOL		m_bUnderlinedLink;
	COLORREF	m_clrLink, m_clrVisited;
	HCURSOR		m_hCursorHotSpot;
};

#endif // !defined(AFX_LINKPROPERTIES_H__7C27ADE8_AF75_472C_974B_CEBE80A3C924__INCLUDED_)
