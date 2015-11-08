// MyStatic.cpp : 实现文件
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



// CMyStatic 消息处理程序


void  CMyStatic::OnPaint()      
{    
  CPaintDC   dc(this);   //   device   context   for   painting     
     
   
  //下面7行代码是根据界面中控件的大小自动换行的   
  CRect   rect;   
  this->GetWindowRect(&rect);   
  ScreenToClient(&rect);   
  CString   str;   
  this->GetWindowText(str);   
  dc.DrawText(str, rect, DT_EDITCONTROL|DT_WORDBREAK);   
  return;   
   
    /* 
  //下面代码则是根据用户指定文本长度自动换行，如15个全角字符长度     
  //并把界面中控件的宽度自动调整到合适的大小     
  CRect   rect;    
  CSize   size(0,0);    
     
  this->GetWindowRect(rect);    
  GetParent()->ScreenToClient(&rect);    
  //随便15个全角字符，以便计算长度，这里是15个全角的空格     
  CString   str=_T("　　　　　　　　　　　　　　　");        
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