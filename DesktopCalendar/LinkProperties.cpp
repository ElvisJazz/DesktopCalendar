// LinkProperties.cpp: implementation of the CLinkProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifndef IDC_HAND
	#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif

CLinkProperties::CLinkProperties()
{
	//Initialize date with most commonly used values
	m_bUnderlinedLink	= DEFAULT_UNDERLINED;
	m_clrLink			= DEFAULT_UNVISITED_COLOR;
	m_clrVisited		= DEFAULT_VISITED_COLOR;
	m_hCursorHotSpot	= AfxGetApp()->LoadStandardCursor(IDC_HAND); //Icon hand
}

CLinkProperties::~CLinkProperties()
{

}

void CLinkProperties::SetLinkProperties(COLORREF clrLink /*= DEFAULT_UNDERLINED*/, 
					COLORREF clrVisited /*= DEFAULT_UNVISITED_COLOR*/, 
					   HCURSOR hCursorHotSpot /*= DEFAULT_HOTSPOT_CURSOR*/, 
					   BOOL bUnderlinedLink /*= DEFAULT_UNDERLINED*/) {
	//Initialize link data.
	//hCursorHotSpot = NULL means the default IDC_ICONHAND will be used.
	m_bUnderlinedLink = bUnderlinedLink;
	m_clrLink = clrLink;
	m_clrVisited = clrVisited;

	if (hCursorHotSpot == NULL) {
		m_hCursorHotSpot = AfxGetApp()->LoadStandardCursor(IDC_HAND);
	} else {
		m_hCursorHotSpot = hCursorHotSpot;
	}
}
