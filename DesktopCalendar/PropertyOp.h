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
	// ����������д���ⲿ�����ļ���
	static void WriteProperty(CalProperty* calProperty);
	// ��ȡ�����ļ�
	static void ReadProperty(CalProperty* calProperty,  bool isDefault = false);
};


