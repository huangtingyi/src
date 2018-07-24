/*//////////////////////////////////////////////////////////////////
ʹ�÷���:
1.�Ȱ�IniFile.h��IniFile.ccp����Project����Ҫʹ�ø����ͷ�ļ��м���
	#include "IniFile.h"����CIniFile IniFile;

2.��ȡ��д��:
	string = IniFile.GetString("section","item","Ĭ��");
	int = IniFile.GetInt("section","item",666);

	IniFile.SetString("section","item","����ֵ");
	IniFile.SetInt("section","item",666);

3.ini�ļ�����·���ڳ�ʼ��ʱ����SetFileName()����

ע������:ini�ļ����̫������ȡ�ٶȻ����������ʹ�ö��ini�ļ�

//////////////////////////////////////////////////////////////////*/
// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_INIFILE_H__33CAF5C6_7820_43EB_8D84_42CC1856C35C__INCLUDED_)
#define AFX_INIFILE_H__33CAF5C6_7820_43EB_8D84_42CC1856C35C__INCLUDED_

using namespace std;

#include <string>
#include <vector>
#include <string.h>
#include <fstream>



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniFile  
{
public:
	void SetFileName(string FileName);

	void WriteFileString(string Section, string Item, string Value);
	string GetFileString(string Section, string Item, string Value);

	string GetString(string Section, string Item, string Value);
	int GetInt(string Section, string Item, int Value);

	void SetString(string Section, string Item, string Value);
	void SetInt(string Section, string Item, int Value);

	bool WriteIniFile();
	bool ReadIniFile();

	void ClearFileContainer();

	CIniFile();
	virtual ~CIniFile();

	//ini file
	string m_sFileName;
	bool  bFileExist;
	vector <string> m_vecFileContainer;
};

#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#endif // !defined(AFX_INIFILE_H__33CAF5C6_7820_43EB_8D84_42CC1856C35C__INCLUDED_)
