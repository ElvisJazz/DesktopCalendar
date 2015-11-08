#pragma once


// CFindMusic
#ifndef _FINDMUSIC_H
#define _FINDMUSIC_H
#include<iostream>   
#include<string>  
#include<fstream>
#include<io.h>   //定义了结构体struct _finddata_t（该结构体包含文件的相关属性，如文件名，文件的属性等
				//定义函数: long _findfirst(char* fileName,struct _finddata_t *fileinf0);
				//定义函数: int _findnext(long handle,struct _finddata_t *fileinfo);
				//定义函数: int _findclose(long handle);
using namespace std; 

//定义链表结点
struct fileInfoNode
{
	struct _finddata_t fileInfo;  //保存文件信息的结构体
	string fileName;
	struct fileInfoNode* left;
};

class CFindMusic
{
	DECLARE_DYNAMIC(CFindMusic)
public:
	CFindMusic();
	virtual ~CFindMusic();

	//把文件信息连接到链表head中
	int saveToLink(struct fileInfoNode*& head,   //链表的头结点,引用参量
		const string& fileName,   //IN:文件名(包括路径)
		const struct _finddata_t& fileInfo); //IN:文件信息结构体,引用参量


	//显示整个查找到的文件的信息
	void displayLink(struct fileInfoNode* head);//IN:头结点，值传递参数

	//把文件信息存储到文件 fileName  中
	void saveLinkToFile(struct fileInfoNode* head,string saveFileName,int counter);

	int searchAllFile(string filePath,//IN:文件所在的路径,如：f:\example
		int layer,//层次，只有层次为0时，才完成链表中文件信息的显示和存储
		string fileInfoOut);   //IN:存储的文件名

};

#endif

