// conv_stk.cpp : 定义控制台应用程序的入口点。

#include "inifile.h"

int main()
{
	printf("hello world.\n");

	printf("press any key to exit!\n");

	CIniFile ini;

	ini.SetFileName("e:\\vc\\bin\\FMLDLL.INI");

	int  iDisplayDays, iDisplayBegin, iDisplayEnd;
	string strRealPath, strHisPath;
	
	strRealPath = ini.GetString("ef", "realtime_path", "d:\\ymdll\\gp\\nymd31fs");
	strHisPath = ini.GetString("ef", "history_path", "d:\\ymdll\\gxp\\nymd31fs");

	iDisplayDays = ini.GetInt("ef", "display_days", 0);
	iDisplayBegin = ini.GetInt("ef", "display_begin", 20180701);
	iDisplayEnd = ini.GetInt("ef", "display_end", 20180704);


	printf("realpath=%s,hispath=%s,displaydays=%d,displaybegin=%d,displayend=%d.\n",
		strRealPath.c_str(), strHisPath.c_str(), iDisplayDays, iDisplayBegin, iDisplayEnd);


	getchar();

	return 0;
}

