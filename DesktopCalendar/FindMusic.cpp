// FindMusic.cpp : ʵ���ļ�
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


/* �ļ�����searchAllFile.cpp
 * int searchAllFile(string filePath,       //Ҫ�������ļ�·��
 *					int layer   //layer==0   //�����Ĳ�Σ�����·���Ĳ��Ӧ��Ϊ0
 *					string fileNameToSave);   //�洢�ļ���Ϣ���ļ���������·��
 *
 *
 *
 *
 *
 *
 * <io.h>�ж����˽ṹ��  struct _finddata_t   (C���),�����洢�ļ��ĸ�����Ϣ
 *��ϸ����:
 *struct _finddata_t 
 *	{
 *      unsigned    attrib;
 *       __time64_t  time_create;    // -1 for FAT file systems 
 *       __time64_t  time_access;    // -1 for FAT file systems 
 *       __time64_t  time_write;
 *       _fsize_t    size;
 *       char        name[260];
 *	};
 *��������������:
 *unsigned attrib  :4���ֽڣ��洢�ļ�������
 *	_A_ARCH  ���浵) 0x20 
 *	_A_SUBDIR���ļ��У�0x10
 *  _A_SYSTEM��ϵͳ��0x04
 *	_A_HIDDEN�����أ�0x02
 *	_A_RDONLY��ֻ����0x01
 *	_A_NORMAL��������0X00
 *��Щ����<io.h>�ж���ĺ�,ÿһ������һ��unsigned int,�����Ե���ʱ���л����㣬��_A_HIDDEN|_A_RDONLY
 *
 *__time64_t  time_create:  �ļ�������ʱ��
 *__time64_t  time_access:  �ļ����һ�η��ʵ�ʱ��
 *__time64_t  time_write:   �ļ�����Դ��޸ĵ�ʱ��
 *_fsize_t    size:         �ļ��Ĵ�С���ֽ�Ϊ��λ
 *char        name[260]:    �ļ���
 *
 *--------------------------------------------------------------------------------------------------------------------------------
 *<io.h>  �ж�������������
 *long _findfirst(const char * _Filename,struct _finddata_t * _FindData);��,���ҵ�һ��_Filename����Ϣ���洢���ṹ��_FindData�С� ���� ���� ���� ����
 *																					���ҳɹ�������һ�����ڼ������ҵľ����һ��Ψһ�ı��)
 *																					���� ʧ�ܣ�����-1		
 *int _findnext(long handle,struct _finddata_t *fileinfo) ;   ���ݾ��handle������һ���ļ�������� fileinfo��
 *																		���ҳɹ�������0��ʧ�ܷ���-1
 *ing _findclose(long handle);       �رվ��handle,�ɹ�����0��ʧ�ܷ���-1
 *---------------------------------------------------------------------------------------------------------------------------------------
 */

//���ļ���Ϣ���ӵ�����head��
int CFindMusic::saveToLink(struct fileInfoNode*& head,   //�����ͷ���,���ò���
	const string& fileName,   //IN:�ļ���(����·��)
	const struct _finddata_t& fileInfo) //IN:�ļ���Ϣ�ṹ��,���ò���
{
	//����һ�����
	 fileInfoNode* p;
	 p=new fileInfoNode();
	 p->fileInfo=fileInfo;  //�Ѵ�����ļ���Ϣ���ƽ����
	 p->fileName=fileName;
	 p->left=head;
	 head=p;
	 return 0;
}

//��ʾ�������ҵ����ļ�����Ϣ
void CFindMusic::displayLink(struct fileInfoNode* head)//IN:ͷ��㣬ֵ���ݲ���
{
	while(head!=NULL)
	{
		cout<<"fileName: "<<head->fileName<<endl;
		cout<<"fileSize: "<<dec<<head->fileInfo.size<<"�ֽ�"<<endl;
		cout<<"fileAttrib: "<<"0x"<<hex<<head->fileInfo.attrib<<endl;
		cout<<"-------------------------------------------------------------------------------------------"<<endl;
		head=head->left;
	}
}

//���ļ���Ϣ�洢���ļ� fileName  ��

void CFindMusic::saveLinkToFile(struct fileInfoNode* head,string saveFileName,int counter)
{
	ofstream fout;

	//���ļ�
	fout.open(saveFileName.c_str(), ios::out);
	if((fout.is_open())==false)
	{
		cout<<"�洢�ļ���ʧ�ܣ�"<<endl;
		exit(-1);
	}

	fout<<counter<<endl; //the file number
	while(head!=NULL)
	{
		fout<<head->fileName<<endl;
		head=head->left;
	}
	//�ر��ļ�
	fout.close();
}

//
int CFindMusic::searchAllFile(string filePath,//IN:�ļ����ڵ�·��,�磺f:\example
	int layer,//��Σ�ֻ�в��Ϊ0ʱ��������������ļ���Ϣ����ʾ�ʹ洢
			string fileInfoOut)   //IN:�洢���ļ���
{
	struct _finddata_t fileInfo;//�����ļ���Ϣ�Ľṹ��

	static fileInfoNode* head=NULL; //fileInfoNode�����ͷ���,��̬�洢
	static int counter=0;  //��¼�ļ���Ŀ
	long handle;//���
	int done;//����nextfile�Ƿ�ɹ�
	string fileName=filePath+"\\*.*"; //Ҫ�������ļ���

	//���ҵ�һ���ļ������ؾ��
	handle=_findfirst(fileName.c_str(),&fileInfo);
	if(handle==-1)
	{
		cout<<"��Ŀ¼Ϊ��!"<<endl;
		//cin.get();
		 return -1;
	}
	do
	{
	//	cout<<"���ҳɹ�"<<endl;
	//	cin.get();
	//	cout<<fileInfo.name<<endl;
		//������ļ���".",����".."��������ж���һ���ļ�
		if((strcmp(fileInfo.name,".")==0)|(strcmp(fileInfo.name,"..")==0))
		{
			//cout<<"������"<<endl;
			//cin.get();
			continue;
		}
		//������ļ��У��������һ���ļ�������
		if((fileInfo.attrib&_A_SUBDIR)==_A_SUBDIR)
		{
	//		cout<<"���ļ���"<<endl;
	//		cin.get();
			string filePathSub=filePath+"\\"+fileInfo.name;
			//�ݹ����
			searchAllFile(filePathSub,++layer,fileInfoOut);
			layer--;
		}
		//���Ѽ�������Ϣ���ӵ��ļ�
		else
		{
//			cout<<"���ļ����洢��Ϣ!"<<endl;
//			cin.get();
			counter++;
			string fileNameTure=filePath+"\\"+fileInfo.name;
			saveToLink(head,fileNameTure,fileInfo);   //�洢��������
		}

	}while(!(done=_findnext(handle,&fileInfo)));
	_findclose(handle);
	//layer==ʱ���������Ĵ洢
	if(layer==0)
	{
		//��ʾ�����е�����
			displayLink(head);
		//�洢�����е�����
			saveLinkToFile(head,fileInfoOut,counter);
	}

	head=NULL; 
	counter=0; 
	return 0;

}

