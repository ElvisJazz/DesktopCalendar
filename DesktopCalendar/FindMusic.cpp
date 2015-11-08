// FindMusic.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopCalendar.h"
#include "FindMusic.h"


// CFindMusic

IMPLEMENT_DYNAMIC(CFindMusic, CWnd)

CFindMusic::CFindMusic()
{

}

CFindMusic::~CFindMusic()
{
}


/* 文件名：searchAllFile.cpp
 * int searchAllFile(string filePath,       //要搜索的文件路径
 *					int layer   //layer==0   //搜索的层次，输入路径的层次应该为0
 *					string fileNameToSave);   //存储文件信息的文件名，包括路径
 *
 *
 *
 *
 *
 *
 * <io.h>中定义了结构体  struct _finddata_t   (C风格),用来存储文件的各种信息
 *详细如下:
 *struct _finddata_t 
 *	{
 *      unsigned    attrib;
 *       __time64_t  time_create;    // -1 for FAT file systems 
 *       __time64_t  time_access;    // -1 for FAT file systems 
 *       __time64_t  time_write;
 *       _fsize_t    size;
 *       char        name[260];
 *	};
 *各参数意义如下:
 *unsigned attrib  :4个字节，存储文件的属性
 *	_A_ARCH  （存档) 0x20 
 *	_A_SUBDIR（文件夹）0x10
 *  _A_SYSTEM（系统）0x04
 *	_A_HIDDEN（隐藏）0x02
 *	_A_RDONLY（只读）0x01
 *	_A_NORMAL（正常）0X00
 *这些都是<io.h>中定义的宏,每一个都是一个unsigned int,各属性叠加时进行或运算，如_A_HIDDEN|_A_RDONLY
 *
 *__time64_t  time_create:  文件创建的时间
 *__time64_t  time_access:  文件最后一次访问的时间
 *__time64_t  time_write:   文件最后以此修改的时间
 *_fsize_t    size:         文件的大小，字节为单位
 *char        name[260]:    文件名
 *
 *--------------------------------------------------------------------------------------------------------------------------------
 *<io.h>  中定义了两个函数
 *long _findfirst(const char * _Filename,struct _finddata_t * _FindData);　,查找第一个_Filename的信息，存储到结构体_FindData中　 　　 　　 　　 　　
 *																					查找成功，返回一个用于继续查找的句柄（一个唯一的编号)
 *																					查找 失败，返回-1		
 *int _findnext(long handle,struct _finddata_t *fileinfo) ;   根据句柄handle查找下一个文件，存放在 fileinfo中
 *																		查找成功，返回0，失败返回-1
 *ing _findclose(long handle);       关闭句柄handle,成功返回0，失败返回-1
 *---------------------------------------------------------------------------------------------------------------------------------------
 */

//把文件信息连接到链表head中
int CFindMusic::saveToLink(struct fileInfoNode*& head,   //链表的头结点,引用参量
	const string& fileName,   //IN:文件名(包括路径)
	const struct _finddata_t& fileInfo) //IN:文件信息结构体,引用参量
{
	//建立一个结点
	 fileInfoNode* p;
	 p=new fileInfoNode();
	 p->fileInfo=fileInfo;  //把传入的文件信息复制进结点
	 p->fileName=fileName;
	 p->left=head;
	 head=p;
	 return 0;
}

//显示整个查找到的文件的信息
void CFindMusic::displayLink(struct fileInfoNode* head)//IN:头结点，值传递参数
{
	while(head!=NULL)
	{
		cout<<"fileName: "<<head->fileName<<endl;
		cout<<"fileSize: "<<dec<<head->fileInfo.size<<"字节"<<endl;
		cout<<"fileAttrib: "<<"0x"<<hex<<head->fileInfo.attrib<<endl;
		cout<<"-------------------------------------------------------------------------------------------"<<endl;
		head=head->left;
	}
}

//把文件信息存储到文件 fileName  中

void CFindMusic::saveLinkToFile(struct fileInfoNode* head,string saveFileName,int counter)
{
	ofstream fout;

	//打开文件
	fout.open(saveFileName.c_str(), ios::out);
	if((fout.is_open())==false)
	{
		cout<<"存储文件打开失败！"<<endl;
		exit(-1);
	}

	fout<<counter<<endl; //the file number
	while(head!=NULL)
	{
		fout<<head->fileName<<endl;
		head=head->left;
	}
	//关闭文件
	fout.close();
}

//
int CFindMusic::searchAllFile(string filePath,//IN:文件所在的路径,如：f:\example
	int layer,//层次，只有层次为0时，才完成链表中文件信息的显示和存储
			string fileInfoOut)   //IN:存储的文件名
{
	struct _finddata_t fileInfo;//保存文件信息的结构体

	static fileInfoNode* head=NULL; //fileInfoNode链表的头结点,静态存储
	static int counter=0;  //记录文件数目
	long handle;//句柄
	int done;//查找nextfile是否成功
	string fileName=filePath+"\\*.*"; //要搜索的文件名

	//查找第一个文件，返回句柄
	handle=_findfirst(fileName.c_str(),&fileInfo);
	if(handle==-1)
	{
		cout<<"该目录为空!"<<endl;
		//cin.get();
		 return -1;
	}
	do
	{
	//	cout<<"查找成功"<<endl;
	//	cin.get();
	//	cout<<fileInfo.name<<endl;
		//如果是文件夹".",或者".."，则进行判断下一个文件
		if((strcmp(fileInfo.name,".")==0)|(strcmp(fileInfo.name,"..")==0))
		{
			//cout<<"丢弃！"<<endl;
			//cin.get();
			continue;
		}
		//如果是文件夹，则进入下一层文件夹搜索
		if((fileInfo.attrib&_A_SUBDIR)==_A_SUBDIR)
		{
	//		cout<<"是文件夹"<<endl;
	//		cin.get();
			string filePathSub=filePath+"\\"+fileInfo.name;
			//递归调用
			searchAllFile(filePathSub,++layer,fileInfoOut);
			layer--;
		}
		//把搜集到的信息连接到文件
		else
		{
//			cout<<"是文件，存储信息!"<<endl;
//			cin.get();
			counter++;
			string fileNameTure=filePath+"\\"+fileInfo.name;
			saveToLink(head,fileNameTure,fileInfo);   //存储到链表中
		}

	}while(!(done=_findnext(handle,&fileInfo)));
	_findclose(handle);
	//layer==时，完成链表的存储
	if(layer==0)
	{
		//显示链表中的内容
			displayLink(head);
		//存储链表中的内容
			saveLinkToFile(head,fileInfoOut,counter);
	}

	head=NULL; 
	counter=0; 
	return 0;

}

