#pragma once


// CFindMusic
#ifndef _FINDMUSIC_H
#define _FINDMUSIC_H
#include<iostream>   
#include<string>  
#include<fstream>
#include<io.h>   //�����˽ṹ��struct _finddata_t���ýṹ������ļ���������ԣ����ļ������ļ������Ե�
				//���庯��: long _findfirst(char* fileName,struct _finddata_t *fileinf0);
				//���庯��: int _findnext(long handle,struct _finddata_t *fileinfo);
				//���庯��: int _findclose(long handle);
using namespace std; 

//����������
struct fileInfoNode
{
	struct _finddata_t fileInfo;  //�����ļ���Ϣ�Ľṹ��
	string fileName;
	struct fileInfoNode* left;
};

class CFindMusic
{
	DECLARE_DYNAMIC(CFindMusic)
public:
	CFindMusic();
	virtual ~CFindMusic();

	//���ļ���Ϣ���ӵ�����head��
	int saveToLink(struct fileInfoNode*& head,   //�����ͷ���,���ò���
		const string& fileName,   //IN:�ļ���(����·��)
		const struct _finddata_t& fileInfo); //IN:�ļ���Ϣ�ṹ��,���ò���


	//��ʾ�������ҵ����ļ�����Ϣ
	void displayLink(struct fileInfoNode* head);//IN:ͷ��㣬ֵ���ݲ���

	//���ļ���Ϣ�洢���ļ� fileName  ��
	void saveLinkToFile(struct fileInfoNode* head,string saveFileName,int counter);

	int searchAllFile(string filePath,//IN:�ļ����ڵ�·��,�磺f:\example
		int layer,//��Σ�ֻ�в��Ϊ0ʱ��������������ļ���Ϣ����ʾ�ʹ洢
		string fileInfoOut);   //IN:�洢���ļ���

};

#endif

