// ScreenCutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "DesktopCalendarDlg.h"
#include "ScreenCutDlg.h"
#include "afxdialogex.h"
#include "winuser.h"
#include <dwmapi.h>  
#pragma comment(lib,"dwmapi.lib")



// CScreenCutDlg 对话框

IMPLEMENT_DYNAMIC(CScreenCutDlg, CDialogEx)

CScreenCutDlg::CScreenCutDlg(CDesktopCalendarDlg *dlg, CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenCutDlg::IDD, pParent)
{
	m_dlg = dlg;
	m_displayInfo = false;
}

CScreenCutDlg::CScreenCutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenCutDlg::IDD, pParent)
{
	m_displayInfo = true;
}

CScreenCutDlg::~CScreenCutDlg()
{
}

void CScreenCutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScreenCutDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_MOVING()
	ON_WM_SIZING()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CScreenCutDlg 消息处理程序


void CScreenCutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(!m_displayInfo)
	{
		CRect rect0;
		GetClientRect(&rect0);
		drawBitmapFormFile(_T("..//INI//miniCal.bmp"), GetDC(), rect0, 2);
	}
	CDialogEx::OnPaint();
}


void CScreenCutDlg::CutDesktop(void)
{
	 //获得任务栏窗口
	 CWnd* pWnd = CWnd::FindWindow(_T("Shell_TrayWnd"), NULL);

	 //发送ID为0x1F5（Win + M）的WM_HOTKEY消息
	 pWnd->SendMessage(WM_HOTKEY, 0x1F5);
	 Sleep(500);

	HBITMAP bmap;
	HDC hDCMemory;
	HDC myDC=::GetDC(0);     
	int x=GetSystemMetrics(SM_CXSCREEN); 
	int y=GetSystemMetrics(SM_CYSCREEN);    
		
	hDCMemory   =   CreateCompatibleDC(myDC)  ;  
	bmap   =   CreateCompatibleBitmap(myDC,   x,  y)  ; 
	HGDIOBJ hBmpPrev   =   SelectObject(hDCMemory,   bmap)   ;   
	BitBlt(hDCMemory,   0,   0,   x,   y,   myDC,   0,   0,   SRCCOPY);  
	::ReleaseDC(0,myDC);
		
	SaveBmp(bmap, _T("..//INI//background.bmp"), 16);
	CRect rect(0, 0, 1584, 816);
}

bool CScreenCutDlg::SaveBmp(HBITMAP hbitmap, LPCWSTR filename, int nColor) //FileName包括路径和文件的全名
{ 
	 BITMAP Bitmap;
	 HDC         hDC;                         
	 DWORD     dwPaletteSize=0,dwBmBitsSize,dwDIBSize,   dwWritten;     
	 BITMAPFILEHEADER       bmfHdr;                   
	 BITMAPINFOHEADER       bi;                           
	 LPBITMAPINFOHEADER   lpbi;           
	 HANDLE   fh,   hDib,   hPal,hOldPal=NULL;   

	 if   (nColor<=   8)   
	  dwPaletteSize   =   (1<<nColor)   *sizeof(RGBQUAD);   
	 GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	 bi.biSize = sizeof(BITMAPINFOHEADER);
	 bi.biWidth = Bitmap.bmWidth;
	 bi.biHeight = Bitmap.bmHeight;
	 bi.biPlanes = 1;
	 bi.biBitCount = nColor;
	 bi.biCompression = BI_RGB;
	 bi.biSizeImage = 0;
	 bi.biXPelsPerMeter = 0;
	 bi.biYPelsPerMeter = 0;
	 bi.biClrUsed = 0;
	 bi.biClrImportant = 0; 
	 dwBmBitsSize   =   ((Bitmap.bmWidth  *   nColor+31)/32*4)*Bitmap.bmHeight;  
	 //为位图分配内存
	 hDib     =   GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));   
	 lpbi   =   (LPBITMAPINFOHEADER)GlobalLock(hDib);   
	 *lpbi   =   bi; 
	 //设置调色板
	 hPal   =   GetStockObject(DEFAULT_PALETTE);       
	 if   (hPal)   
	 {   
	  hDC     =   ::GetDC(NULL);   
	  hOldPal   =   SelectPalette(hDC,   (HPALETTE)hPal,   FALSE);   
	  RealizePalette(hDC);   
	 }  
	 // 获取该调色板下新的像素值
	 GetDIBits(hDC,   hbitmap,   0,   Bitmap.bmHeight, 
	  (LPSTR)lpbi   +   sizeof(BITMAPINFOHEADER)+dwPaletteSize,   
	  (BITMAPINFO   *)lpbi,   DIB_RGB_COLORS);   
	 if   (hOldPal)   
	 {   
	  SelectPalette(hDC,   (HPALETTE)hOldPal,   TRUE);   
	  RealizePalette(hDC);   
	  ::ReleaseDC(NULL,hDC);   
	 }   

	 fh   =   CreateFile(filename,   GENERIC_WRITE,     
	  0,//not   be   shared   
	  NULL,   //cannot   be   inherited   
	  CREATE_ALWAYS,   
	  FILE_ATTRIBUTE_NORMAL   |   FILE_FLAG_SEQUENTIAL_SCAN,     
	  NULL);   

	 if   (fh   ==   INVALID_HANDLE_VALUE)   
	  return   FALSE;   
	 //设置位图文件头
	 bmfHdr.bfType   =   0x4D42;     //   "BM"   
	 dwDIBSize           =   sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;       
	 bmfHdr.bfSize   =   dwDIBSize;   
	 bmfHdr.bfReserved1   =   0;   
	 bmfHdr.bfReserved2   =   0;   
	 bmfHdr.bfOffBits   =   (DWORD)sizeof(BITMAPFILEHEADER)     
	  +   (DWORD)sizeof(BITMAPINFOHEADER)+   dwPaletteSize;   

	 //write   file   header   
	 WriteFile(fh,   (LPSTR)&bmfHdr,   sizeof(BITMAPFILEHEADER),   &dwWritten,   NULL);   

	 //write   bmp   data   
	 WriteFile(fh,   (LPSTR)lpbi,   dwDIBSize,   &dwWritten,   NULL);   

	 GlobalUnlock(hDib);   
	 GlobalFree(hDib);   
	 CloseHandle(fh);   
	 DeleteObject(hbitmap);    
	 return   TRUE;  


}

//通过BMP文件画图  
void   CScreenCutDlg::drawBitmapFormFile(CString   BitmapFilePath,CDC   *pDC,CRect   rect, int order)  
  {  
	  static HANDLE   filehandle1;
	  static HANDLE   filehandle2;
	  static int num = 0;
	  HANDLE filehandle;

	  if(num < 2)
	  {
		  if(order == 1)
			  filehandle1 = ::LoadImage(NULL,BitmapFilePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  
		  else if(order == 2)
			  filehandle2 = ::LoadImage(NULL,BitmapFilePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  
	  }
	  num++;

	  if(order == 1)
		  filehandle = filehandle1;
	  else if(order == 2)
		  filehandle = filehandle2;

	  if(filehandle1!=NULL)  
	  {  
		  CBitmap   bmp;  
		  if(bmp.Attach(filehandle))  
		  {  
			  BITMAP   bmpInfo;  
			  bmp.GetBitmap(&bmpInfo);  
			  CDC   dcMemory;  
			  dcMemory.CreateCompatibleDC(pDC);  
			  dcMemory.SelectObject(&bmp);  
			  SetBkColor(pDC->m_hDC, RGB(0,0,0));
			  pDC->SetStretchBltMode(HALFTONE);  
			  pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&dcMemory,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);  
			  bmp.Detach();  
		  }  
	  }  
  }

void CScreenCutDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	if(!m_displayInfo)
		FixMoving(fwSide, pRect);
	CDialogEx::OnMoving(fwSide, pRect);
}

void CScreenCutDlg::FixMoving(UINT fwSide, LPRECT pRect)
{
	int height = pRect->bottom - pRect->top;
	int width = pRect->right - pRect->left;

	CRect rect0;
	CRect rect = m_dlg->GetDisplayRect();
	m_dlg->GetWindowRect(&rect0);
	rect0.top += 24;

	rect.bottom += (rect0.top-15);
	rect.top += rect0.top;
	rect.right += rect0.left;
	rect.left += rect0.left;

	if(pRect->left < rect.left)
	{
		pRect->left = rect.left;
		pRect->right = rect.left + width;
	}
	if(pRect->right > rect.right)
	{
		pRect->right = rect.right;
		pRect->left = rect.right - width;
	}
	if(pRect->bottom > rect.bottom)
	{
		pRect->bottom = rect.bottom;
		pRect->top = rect.bottom - height;
	}
	if(pRect->top < rect.top)
	{
		pRect->top = rect.top;
		pRect->bottom = rect.top + height;
	}

	m_rect = pRect;
}

void CScreenCutDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	if(!m_displayInfo)
	{
		CRect rect0;
		CRect rect = m_dlg->GetDisplayRect();
		m_dlg->GetWindowRect(&rect0);
		rect0.top += 24;

		rect.bottom += (rect0.top-15);
		rect.top += rect0.top;
		rect.right += rect0.left;
		rect.left += rect0.left;

		if(pRect->left < rect.left)
		{
			pRect->left = rect.left;
			pRect->right = pRect->right;
		}
		if(pRect->right > rect.right)
		{
			pRect->right = rect.right;
			pRect->left = pRect->left;
		}
		if(pRect->bottom > rect.bottom)
		{
			pRect->bottom = rect.bottom;
			pRect->top = pRect->top;
		}
		if(pRect->top < rect.top)
		{
			pRect->top = rect.top;
			pRect->bottom = pRect->bottom;
		}
		
	}
	OnPaint();
	//CDialogEx::OnSizing(fwSide, pRect);
}


BOOL CScreenCutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(m_displayInfo)
	{
		SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_STYLE) | WS_EX_LAYERED);  // set extended style
		SetLayeredWindowAttributes(RGB(252,252,252),0,1);
		ModifyStyle(0,WS_CAPTION,0); // 还原标题栏
		BOOL bDwn;
		DwmIsCompositionEnabled(&bDwn);
		if (bDwn)
		{
			MARGINS mrg = { -1 };
			DwmExtendFrameIntoClientArea(m_hWnd,&mrg);
			SetBackgroundColor(RGB(252,252,252));
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
			//CMFCButton::EnableWindowsTheming();
			//如果 按钮 类型是CButton，则需要关联 CMFCButton 类型按钮，然后使用此函数设定视觉风格
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CScreenCutDlg::OnNcHitTest(CPoint point)
{
	UINT   nHitTest   =   CDialog::OnNcHitTest(point); 

	CRect tRect;
	GetWindowRect(tRect);  
	if((nHitTest== HTCLIENT)  && (::GetAsyncKeyState(MK_LBUTTON)<0) && point.x>tRect.left && point.x<tRect.right
		&& point.y>tRect.top && point.y<tRect.bottom)   
		 nHitTest   =   HTCAPTION;   

	return nHitTest;
}


// 修改显示属性
void CScreenCutDlg::SetProperty(void)
{
	CalProperty *pro = m_dlg->GetProperty();
	float width = (float)GetSystemMetrics(SM_CXSCREEN);
	CRect rect = m_dlg->GetDisplayRect();
	CRect parentRect;
	m_dlg->GetWindowRect(&parentRect);
	CRect rect0;
	GetWindowRect(&rect0);
	float ratio = (float)rect.Width() / width;

	pro->rect.left = (rect0.left - parentRect.left - rect.left) / ratio;
	pro->rect.right = (rect0.right - parentRect.left - rect.left) / ratio;
	pro->rect.top = (rect0.top - parentRect.top - 25 - rect.top) / ratio;
	pro->rect.bottom = (rect0.bottom -parentRect.top - 25 - rect.top) / ratio;

}

