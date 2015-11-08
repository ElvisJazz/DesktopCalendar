// FileOperation.cpp : ʵ���ļ�
//

#include "DesktopCalendar.h"
#include "FileOperation.h"
#include "stdafx.h"

// CFileOperation

IMPLEMENT_DYNAMIC(CFileOperation, CWnd)

CFileOperation::CFileOperation()
{
	m_RecordName = _T("");
	m_Month = NULL;
	m_Day = NULL;
	m_Memory2 = NULL;
}

CFileOperation::~CFileOperation()
{
	delete []m_Month;
	delete []m_Day;
	//delete [m_NumOfArray]m_Memory1;
	delete []m_Memory2;
	//delete [m_NumOfArray]m_SweetMessage;
	//delete [m_NumOfArray]m_MarkInfo;
	//delete [m_NumOfArray]m_ExtralSM;
}


BEGIN_MESSAGE_MAP(CFileOperation, CWnd)
END_MESSAGE_MAP()



// CFileOperation ��Ϣ�������

// �������Ԥ�ȶ�ȡ���ű��




void CFileOperation::PreRead(int year, void* pro, bool create)
{
	CString s = _T("");
	s.Format(_T("%d"), year);
	s += ".xls";
	m_RecordName = s;
	CSpreadSheet SS(s, _T("Sheet1"));

	if(SS.OpenSuccess())
	{
		m_EnableAdd = true;
		CStringArray Rows, Column;
		m_NumOfArray = SS.GetTotalRows();

		if(m_Month != NULL)
		{
			delete[] m_Month;
			delete[] m_Day;
			delete[] m_Memory2;
		}
		m_Month = new int[m_NumOfArray];
		m_Day = new int[m_NumOfArray];
		//m_Memory1 = new CString[m_NumOfArray];
		m_Memory2 = new CString[m_NumOfArray];
		//m_SweetMessage = new CString[m_NumOfArray];
		//m_MarkInfo = new int[m_NumOfArray];
		//m_ExtralSM = new CString[m_NumOfArray];

		int tmpColor = -1;

		for (int i = 2; i <= m_NumOfArray; i++)
		{
			 // ��ȡһ��
			 SS.ReadRow(Rows, i);
			 CString strContents = _T("");
			// m_MarkInfo[i-2] = 0;
			 for (int j = 0; j < Rows.GetSize(); j++)
			 {
				 switch(j)
				 {
				 case 0:if(Rows.GetAt(j) != "")
							m_Month[i-2] = _ttoi(Rows.GetAt(j));
						else
							m_Month[i-2] = -1;
						break;
				 case 1:if(Rows.GetAt(j) != "")
							m_Day[i-2] = _ttoi(Rows.GetAt(j));
						else
							m_Day[i-2] = -1;
						break;
				 /*case 2: m_Memory1[i-2] = Rows.GetAt(j);
						if(m_Memory1[i-2] != "")
						{
							if(m_Memory1[i-2][0] == '@')
								m_MarkInfo[i-2] += 1000;
							else
								m_MarkInfo[i-2] += 1;
						}
						break;*/
				 case 3: m_Memory2[i-2] = Rows.GetAt(j);
						/*if(m_Memory2[i-2] != "")
							m_MarkInfo[i-2] += 10;*/
						break;
				/* case 4: m_SweetMessage[i-2] = Rows.GetAt(j);
						if(m_SweetMessage[i-2] != "")
							m_MarkInfo[i-2] += 100;
						break;
				 case 5: m_ExtralSM[i-2] = Rows.GetAt(j);
						break;*/
				 case 12:if(Rows.GetAt(j) != "")
							tmpColor = _ttoi(Rows.GetAt(j));
						else
							tmpColor = -1;
						break;
				 default: break;
				 }
			 }

			 // �������Բ����е�infoMap
			 CString dateCode;
			 dateCode.Format(_T("%d %d %d"), m_Day[i-2], m_Month[i-2], year); 
			 Info *info = new Info();
			 info->text = m_Memory2[i-2];
			 info->color = tmpColor;
			 ((CalProperty*)pro)->infoMap.insert(make_pair(dateCode, info));
		 
		}
		return;
	}
	else if(create)
	{
		Create();
	}
	m_EnableAdd = false;
	m_Month = new int[1];
	m_Day = new int[1];
	//m_Memory1 = new CString[1];
	m_Memory2 = new CString[1];
	//m_SweetMessage = new CString[1];
	//m_MarkInfo = new int[1];
	//m_ExtralSM = new CString[1];
	
}

void CFileOperation::Create()
{
	// �½�Excel�ļ�����·����TestSheetΪ�ڲ�����
	CSpreadSheet SS(m_RecordName, _T("Sheet1"));

	CStringArray sampleArray, testRow;
	
	SS.BeginTransaction();
	
	// �������
	sampleArray.RemoveAll();
	sampleArray.Add(_T("Month"));
	sampleArray.Add(_T("Day"));
	sampleArray.Add(_T("Love_Memory"));
	sampleArray.Add(_T("Memorandum"));
	sampleArray.Add(_T("Sweet_Message"));
	sampleArray.Add(_T("Extral_Sweet_Message"));
	sampleArray.Add(_T("Extral_Sweet_Message1"));
	sampleArray.Add(_T("Extral_Sweet_Message2"));
	sampleArray.Add(_T("Extral_Sweet_Message3"));
	sampleArray.Add(_T("Extral_Sweet_Message4"));
	sampleArray.Add(_T("Extral_Sweet_Message5"));
	sampleArray.Add(_T("Extral_Sweet_Message6"));
	sampleArray.Add(_T("Color"));
	SS.AddHeaders(sampleArray);

	SS.Commit();	
}

// ��ȡ���ۼ���
//CString CFileOperation::GetLoveMemory(int index)
//{
//	return m_Memory1[index];
//}


// ��ȡ����¼
CString CFileOperation::GetMemorandum(int index)
{
	return m_Memory2[index];
}

// ��ȡ����
//CString CFileOperation::GetSweetMessage(int index)
//{
//	return m_SweetMessage[index]+m_ExtralSM[index];
//}


// ��Ӻ���ģ��
// check1����Ӷ��� check2����ӱ���¼ 
void CFileOperation::AddMessage(int month, int day, bool check1, bool check2, CString s1, CString s2, CString s3, COLORREF color)
{
	if(!m_EnableAdd)
	{
		m_EnableAdd = true;
		Create();
	}
	// �½�Excel�ļ�����·����TestSheetΪ�ڲ�����
	CSpreadSheet SS(m_RecordName, _T("Sheet1"));
	SS.BeginTransaction();
	
	// ��������
	int i;
	bool insertMark=true; // ��־�ǲ����µ�һ�У������滻ֵ
	CString colorStr;
	colorStr.Format(_T("%d"), color);

	for(i=0; i<m_NumOfArray; i++)
	{
		if(m_Month[i] == month)
		{
			if(m_Day[i] == day)
			{
				insertMark = false; // �滻
				break;
			}
			else if(m_Day[i] > day)
			{
				insertMark = true; // ����
				break;
			}
			else
				continue;
		}
		else if(m_Month[i] > month)
		{
			insertMark = true; // ����
			break;
		}
	}

	if(insertMark)
	{
		CString month1, day1;
		CStringArray array1;
		month1.Format(_T("%d"), month);
		day1.Format(_T("%d"), day);
		array1.RemoveAll();
		array1.Add(month1);
		array1.Add(day1);
		array1.Add(s1);
		array1.Add(s3);

		for(int i=0; i<8; i++)
			array1.Add(_T(""));
		array1.Add(colorStr);

		if(i == m_NumOfArray)
			i--;
		SS.AddRow(array1, i+2);
	}
	else
	{
		//if(check1)
		//{
		//	SS.AddCell(s1, 3, i+2);    // ���(������)���滻(����)��i+2�У���3�е�Ԫ������
		//	/*if(s2.GetLength() > 250)
		//	{
		//		CString s20 = s2.Mid(0, 250);
		//		CString s21 = s2.Mid(250);
		//		SS.AddCell(s20, 5, i+2);
		//		SS.AddCell(s21, 6, i+2);
		//	}
		//	else
		//	{
		//		SS.AddCell(s2, 5, i+2); 
		//		SS.AddCell(_T(""), 6, i+2); 
		//	}*/
		//	/*cout<<s1<<endl;
		//	cout<<s2<<endl;*/
		//}
		if(check2)
		{
			SS.AddCell(s3, 4, i+2); 
			SS.AddCell(colorStr, 13, i+2);
		}
	}
	SS.Commit();
}
