#pragma once


// CPropertyOp

class CPropertyOp : public CWnd
{
	DECLARE_DYNAMIC(CPropertyOp)

public:
	CPropertyOp();
	virtual ~CPropertyOp();

private:
	static LPTSTR lpPath;
	static LPTSTR lpDefaultPath;

protected:
	DECLARE_MESSAGE_MAP()
public:
//	void GetProperty(CalProperty* calProperty);
	// 将日历属性写到外部配置文件中
	static void WriteProperty(CalProperty* calProperty);
	// 读取配置文件
	static void ReadProperty(CalProperty* calProperty,  bool isDefault = false);
};


