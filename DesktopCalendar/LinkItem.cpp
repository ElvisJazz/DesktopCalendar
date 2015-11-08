// LinkItem.cpp: implementation of the CLinkItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinkItem::CLinkItem()
{
	m_pLinkProp = NULL; 
	m_bVisited = FALSE;
}

CLinkItem::~CLinkItem()
{

}

COLORREF CLinkItem::GetLinkColor() { 
	//Returns the link color (it changes if link was visited).
	//In case of error, returns black: no color ;-)
	if (m_pLinkProp == NULL) return RGB(0,0,0);

	if (GetVisited()) {
		return m_pLinkProp->GetVisitedLinkColor();
	}

	return m_pLinkProp->GetLinkColor();
}

BOOL CLinkItem::GetUnderlinedLink() {
	//Wrapper to retrieve the underlined property.

	//if structure is invalid returns the default
	if (m_pLinkProp == NULL) return DEFAULT_UNDERLINED;

	return m_pLinkProp->GetUnderlinedLink();
}

HCURSOR CLinkItem::GetCursor() {
	//Wrapper to retrieve the cursor.

	//if structure is unvalid returns the default cursor
	if (m_pLinkProp == NULL) return DEFAULT_HOTSPOT_CURSOR;

	return m_pLinkProp->GetCursor();
}

