// MyStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "MyStatic.h"


// CMyStatic

IMPLEMENT_DYNAMIC(CMyStatic, CStatic)

CMyStatic::CMyStatic()
{

}

CMyStatic::~CMyStatic()
{
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyStatic ��Ϣ�������


void  CMyStatic::OnPaint()      
{    
  CPaintDC   dc(this);   //   device   context   for   painting     
     
   
  //����7�д����Ǹ��ݽ����пؼ��Ĵ�С�Զ����е�   
  CRect   rect;   
  this->GetWindowRect(&rect);   
  ScreenToClient(&rect);   
  CString   str;   
  this->GetWindowText(str);   
  dc.DrawText(str, rect, DT_EDITCONTROL|DT_WORDBREAK);   
  return;   
   
    /* 
  //����������Ǹ����û�ָ���ı������Զ����У���15��ȫ���ַ�����     
  //���ѽ����пؼ��Ŀ���Զ����������ʵĴ�С     
  CRect   rect;    
  CSize   size(0,0);    
     
  this->GetWindowRect(rect);    
  GetParent()->ScreenToClient(&rect);    
  //���15��ȫ���ַ����Ա���㳤�ȣ�������15��ȫ�ǵĿո�     
  CString   str=_T("������������������������������");        
  CFont   *pOldFont=dc.SelectObject(GetParent()->GetFont());    
  ::GetTextExtentPoint32((HDC)dc,str,str.GetLength(),&size);    
  rect.right=rect.left+size.cx;    
  this->MoveWindow(rect);    
  rect.right-=rect.left;    
  rect.bottom-=rect.top;    
  rect.left=0;    
  rect.top=0;    
  this->GetWindowText(str);    
  dc.DrawText(str,rect,DT_EDITCONTROL|DT_WORDBREAK);    
  dc.SelectObject(pOldFont);    
  */   
  //   Do   not   call   CStatic::OnPaint()   for   painting   messages     
}  