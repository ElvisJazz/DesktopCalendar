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

	// 存储待显示信息
	map<int, CString> m_displayExtractionInfo;
	// 获取网页源代码
	bool GetSourceHtml(CString theUrl,CString fileName);
	// 获取本地源代码
	bool GetLocalHtml(CString fileName);
	// 获取本地网页抽取设置
	bool ReadExtractionIni(LPTSTR fileName);
	// 根据配置抽取信息
	bool ExtractInfo(int order);
	// 获取申请的返回信息
	CString GetRequiredInfo(CString fileName, CString url, vector<CString> filed, bool isGroup=true);
	// 解析返回的json
	bool AnalyseJson(CString fileName);
protected:
	DECLARE_MESSAGE_MAP()
private:
	CString m_htmlStr;
	CString m_url;
	vector<ExtractionInfo> m_extractionInfo;

};


