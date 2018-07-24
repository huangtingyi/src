/*//////////////////////////////////////////////////////////////////
使用方法:
1.先把IniFile.h，IniFile.ccp加入Project，在要使用该类的头文件中加入
	#include "IniFile.h"，及CIniFile IniFile;

2.读取和写入:
	string = IniFile.GetString("section","item","默认");
	int = IniFile.GetInt("section","item",666);

	IniFile.SetString("section","item","设置值");
	IniFile.SetInt("section","item",666);

3.ini文件名及路径在初始化时，用SetFileName()设置

注意事项:ini文件如果太长，读取速度会变慢，可以使用多个ini文件

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
