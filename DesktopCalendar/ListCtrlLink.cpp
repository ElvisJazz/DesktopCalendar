// ListCtrlLink.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlLink.h"
#include "LinkItem.h"

#include <AFXINET.H>
#include ".\listctrllink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlLink

CListCtrlLink::CListCtrlLink()
{
}

CListCtrlLink::~CListCtrlLink()
{
	POSITION pos;
	CString strKey;
	CLinkItem *pLinkItem;

	//Delete the entire list
	for (pos = m_linkMap.GetStartPosition(); pos != NULL; ) {
		m_linkMap.GetNextAssoc( pos, strKey, (CObject*&)pLinkItem);
		if (pLinkItem) {
			delete pLinkItem;
			pLinkItem = NULL;
		}
   }

   m_linkMap.RemoveAll();
}


BEGIN_MESSAGE_MAP(CListCtrlLink, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlLink)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemchanging)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
//	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlLink message handlers

void CListCtrlLink::Init() {
	//Initialize whatever is needed. Should be called inside 
	//On initial update for now...

	m_hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	CListCtrl::GetItemPosition(0, &m_pointOriginalPosition);

	//CGOLDSCH: Windows 9X does not change the hot spot icon properly so
	//I decided to use only the arrow icon, even for hot spots.
	unsigned long ulStyle = 0;
	DWORD dwVersion = GetVersion();

	if (dwVersion > 0x80000000) {
		//Win9X
		ulStyle = LVS_EX_FULLROWSELECT;
	} else {
		//NT based systems
		ulStyle = LVS_EX_ONECLICKACTIVATE  | LVS_EX_FULLROWSELECT;
	}

	CListCtrl::SetExtendedStyle(ulStyle);

	//Set link properties to look like there is no link.
	m_noLink.SetLinkProperties(RGB(0,0,0), RGB(0,0,0), m_hCursorArrow, FALSE);

	//Set font according to link properties
	InitFont(m_link, &m_fontLink);
	InitFont(m_noLink, &m_fontNoLink);

}

void CListCtrlLink::OnCustomDraw(NMHDR* pNMHDR, LRESULT *pResult) {
	//Coordinates how the items should be draw.
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	CLinkItem *pLink = NULL;
    *pResult = 0;

	//Switch between different draw stages
	switch (pLVCD->nmcd.dwDrawStage) {

	case CDDS_PREPAINT:
		// If this is the beginning of the control's paint cycle, request
		// notifications for each item.
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		//Ask for subitem notification.
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		//Get the link data
		pLink = GetLink(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem);


		if (pLink != NULL) {

			//Set item color
			pLVCD->clrText = pLink->GetLinkColor();

			//Setup font
			if (pLink->GetUnderlinedLink()) {
				::SelectObject(pLVCD->nmcd.hdc, m_fontLink.m_hObject);
			} else {
				::SelectObject(pLVCD->nmcd.hdc, m_fontNoLink.m_hObject);
			}
		}

		*pResult = CDRF_DODEFAULT;
		break;

	default:
			break;

	}

	//Set the hot cursor (when hovering over data)
	if (pLVCD->nmcd.uItemState == CDIS_HOT) {
		pLink = GetHotLink(pLVCD->nmcd.dwItemSpec);

		if (pLink != NULL) { 
			CListCtrl::SetHotCursor(pLink->GetCursor());
		}
	}
}

void CListCtrlLink::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CLinkItem* pLink = NULL;
	CString strUrl;
	LRESULT result = TRUE;

	//Allows SetItemData to work
	if (pNMListView->uChanged == LVIF_PARAM) {
		*pResult = FALSE;
		return;
	}

	pLink = GetHotLink(pNMListView->iItem);

	if (pLink != NULL) {
		strUrl = pLink->GetUrl();

		if (!strUrl.IsEmpty()) {

			ShellExecute(m_hWnd, NULL, strUrl, NULL, NULL, SW_SHOWNORMAL);
			pLink->SetVisited(TRUE);
		}
	}

	//Prevents change
	*pResult = TRUE;
}

void CListCtrlLink::InitFont(CLinkProperties link, CFont *pFont) {
	//Initializes font according to link properties

	if (pFont == NULL) return;

	//Creates font
	if (!(HFONT)(*pFont)) {
		// first time init: create font
		LOGFONT lf;
		GetFont()->GetObject(sizeof(lf), &lf);
		lf.lfUnderline = link.GetUnderlinedLink();
		pFont->CreateFontIndirect(&lf);
	}
}

int CListCtrlLink::GetScrollCompensation() {
	//Retrieves the horizontal scroll deslocation
	//for the first item.
	int iDelta;
	CPoint point;

	CListCtrl::GetItemPosition(0, &point);

	iDelta = point.x - m_pointOriginalPosition.x;

	return iDelta;
}

CLinkItem* CListCtrlLink::GetHotLink(int iRow) {
	//Retrieves the link to the item the mouse is passing over.
	//Returns NULL in case we are not over any item.

	CPoint mouse;
	CRect rect;
	int iSubItem = 0;
	int iColumnPosition = 0;

    //Current mouse position
	GetCursorPos(&mouse);

	//Current control position
	CListCtrl::GetWindowRect(&rect);

	//If mouse is not inside the list control, exists this method
	if (!rect.PtInRect(mouse)) return NULL;

	//Sets position where the column is located in the screen.
	iColumnPosition = rect.left + GetScrollCompensation();

	for (iSubItem = 0; iSubItem < 100; iSubItem++) {
		iColumnPosition += CListCtrl::GetColumnWidth(iSubItem);

		if (iColumnPosition > mouse.x) break;
	}

	//TRACE("Item %d, SubItem %d\n", iRow, iSubItem);
	return GetLink(iRow, iSubItem);
}

CLinkItem* CListCtrlLink::GetLink(int i, int j) {
	//Returns the location of an item by looking up at
	//a map
	CString strLocation;

	strLocation.FormatMessage(_T("%1!d! x %2!d!"), i, j);

	return (CLinkItem*) m_linkMap[strLocation];
}

void CListCtrlLink::SetLink(int i, int j, CLinkItem* pItemLink) {
	//Insterts an item in a container structure.
	CString strLocation;

	strLocation.FormatMessage(_T("%1!d! x %2!d!"), i, j);

	m_linkMap[strLocation] = (CObject*) pItemLink;
}

int CListCtrlLink::InsertItem(int nItem, LPCTSTR lpctszItem, 
							LPCTSTR lpctszUrl /*= NULL*/) {

	//Wrapper on CListCtrl to include link on insertion operation.
	CLinkItem *pLinkItem = NULL;
	int iRtn = -1;

	pLinkItem = new CLinkItem();

	//can't allocate enough memory, can't insert data in the list control.
	if (pLinkItem == NULL) return -1; 

	iRtn = CListCtrl::InsertItem(nItem, lpctszItem);

	if (lpctszUrl && _tcslen(lpctszUrl) != 0) {

		pLinkItem->SetUrl(lpctszUrl);
		pLinkItem->SetLinkProperties(&m_link);

	} else {

		pLinkItem->SetLinkProperties(&m_noLink);
	}

	SetLink(nItem, 0, pLinkItem);

	return iRtn;
}

BOOL CListCtrlLink::SetItemText(int nItem, int nSubItem, LPCTSTR lpctszText,
							 LPCTSTR lpctszUrl /*= NULL*/) {

	CLinkItem *pLinkItem = NULL;
	BOOL bRtn = FALSE;

	pLinkItem = new CLinkItem();

	//can't allocate enough memory, can't set data in the list control.
	if (pLinkItem == NULL) return FALSE;

	bRtn = CListCtrl::SetItemText(nItem, nSubItem, lpctszText);

	
	if (lpctszUrl && _tcslen(lpctszUrl) != 0) {

		pLinkItem->SetUrl(lpctszUrl);
		pLinkItem->SetLinkProperties(&m_link);

	} else {

		pLinkItem->SetLinkProperties(&m_noLink);
	}

	SetLink(nItem, nSubItem, pLinkItem);

	return bRtn;
}

