#pragma once
#include <vector>
// CWebpageHandler
typedef struct extractionInfo
{
	bool isGroup;
	CString title;
	CString feature;
	CString field;
	vector<int> pos;
	CString startToken;
	CString endToken;
	CString endFeature;

} ExtractionInfo;

class CWebpageHandler : public CWnd
{
	DECLARE_DYNAMIC(CWebpageHandler)

public:
	CWebpageHandler();
	virtual ~CWebpageHandler();

	// �洢����ʾ��Ϣ
	map<int, CString> m_displayExtractionInfo;
	// ��ȡ��ҳԴ����
	bool GetSourceHtml(CString theUrl,CString fileName);
	// ��ȡ����Դ����
	bool GetLocalHtml(CString fileName);
	// ��ȡ������ҳ��ȡ����
	bool ReadExtractionIni(LPTSTR fileName);
	// �������ó�ȡ��Ϣ
	bool ExtractInfo(int order);
	// ��ȡ����ķ�����Ϣ
	CString GetRequiredInfo(CString fileName, CString url, vector<CString> filed, bool isGroup=true);
	// �������ص�json
	bool AnalyseJson(CString fileName);
protected:
	DECLARE_MESSAGE_MAP()
private:
	CString m_htmlStr;
	CString m_url;
	vector<ExtractionInfo> m_extractionInfo;

};


