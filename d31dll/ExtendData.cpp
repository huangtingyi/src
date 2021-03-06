#pragma warning(disable:4996)

#include "stdafx.h"
#include "ExtendData.h"
#include <fstream>
#include <math.h>
#include <io.h>
#include <stdlib.h>
#include<memory>
#include <cctype>
#include <string>  
#include <algorithm> 

#include "wwtiny.h"

using namespace std;

int iTotalCount = 0;

//L2DHandle dhandle;
//#define SHARE_POOL	dhandle.publicsharedatapool
//#define SHARE_TDFDATA	dhandle.publicsharedatapool->tdf_data

SharePool POOL;
#define SHARE_POOL	POOL.m_pSharePool
#define SHARE_TDFDATA	POOL.m_pSharePool->tdf_data

//初始化逐K和序列标志
//初始化金字塔标志
//初始化Mem标志
void InitDllCallInfo(string strCall,string strTips,DllCallInfo_t *p)
{
	CIniFile ini;

	ini.SetFileName("FMLDLL.INI");

	memset((void*)p, 0, sizeof(DllCallInfo_t));

	if (SHARE_POOL->tdfdoing == 1)
		p->bMemFlag = true;
	else	p->bMemFlag = false;

	strCall = strCall.substr(strCall.find_last_of("\\") + 1);
	transform(strCall.begin(), strCall.end(), strCall.begin(), ::toupper);

	if (strCall == "WINSTOCK.EXE") {
		p->bJztFlag = true;
		strcpy(p->sSoftTag, "JZT");
	}
	else {
		p->bJztFlag = false;
		strcpy(p->sSoftTag, "DZH");
	}

	strTips = strTips.substr(strTips.find_last_of("\\") + 1);
	transform(strTips.begin(), strTips.end(), strTips.begin(), ::toupper);
	
	if (strTips == "EFZ.DLL")
		p->bZkFlag = true;
	else if (strTips == "EFX.DLL")
		p->bZkFlag = false;
	else if (p->bJztFlag == false)
		p->bZkFlag = false;
	else{//这是是金字塔且文件名可能就是EF.DLL了
		if (ini.GetInt("ef", "zk_flag", 0) == 0)
			p->bZkFlag = false;
		else    p->bZkFlag = true;
	}

	p->iLogLevel = ini.GetInt("ef", "log_level", 0);

	if (p->bZkFlag == true)
		strcpy(p->sZkXlTag, "ZK");
	else    strcpy(p->sZkXlTag, "XL");
	
}
BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	TCHAR pCallFileName[MAX_PATH];
	TCHAR pDllFileName[MAX_PATH];

	::GetModuleFileName(NULL, pCallFileName, MAX_PATH);
	::GetModuleFileName(hModule, pDllFileName, MAX_PATH);

	string strCallType, strCall, strTips;

	strCall = string(pCallFileName);
	strTips=  string(pDllFileName);

	static bool bFirstFlag = true;

	log_printf(4, "firstflag=%d\n", bFirstFlag);

	if (bFirstFlag == true) {
		bFirstFlag = false;
		InitDllCallInfo(strCall, strTips, &CALLINFO);
		log_printf(4, "first call dllmain\n");
	}

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:strCallType = "PROCESS_ATTACH"; break;
	case DLL_THREAD_ATTACH: strCallType = "THREAD_ATTACH"; break;
	case DLL_THREAD_DETACH: strCallType = "THREAD_DETACH"; break;
	case DLL_PROCESS_DETACH:strCallType = "PROCESS_DETACH"; break;
	default: strCallType = "OTHER_CALL";break;
	}

	log_printf(4,"调用EXE:%s 被调用DLL：%s 调用方式 %s %s %s\n", 
		strCall.c_str(), strTips.c_str(),strCallType.c_str(),
		CALLINFO.sSoftTag, CALLINFO.sZkXlTag);

	return TRUE;
}
__declspec(dllexport) int WINAPI D31FS(CALCINFO* pData)
{
	int iRes;
	static D31MemCache *MC = NULL;

	//如果数据类型不支持，则直接退出
	if (CheckDataTypeValid(pData) == false)	return -4;

	//第一次调用函数时进行初始化
	if (MC == NULL) {

		log_printf(1, "hello xbegin %s(%s) xxxx.--------------%d,%d,MC=%x,MYMC=%x,cnt=%d,dt=%d,s=%s.\n",
			CALLINFO.sSoftTag, CALLINFO.sZkXlTag,SHARE_POOL->tdfdoing,
			sizeof(MYPUBLICSHAERDATA),MC,MYMC,iTotalCount,pData->m_dataType,pData->m_strStkLabel);

		MC = new D31MemCache(CALLINFO.bMemFlag);
	}
	else
		log_printf(1, "hello ybegin %s(%s) yyyy.--------------%d,%d,MC=%x,MYMC=%x cnt=%d,dt=%d s=%s.\n",
			CALLINFO.sSoftTag, CALLINFO.sZkXlTag,SHARE_POOL->tdfdoing,
			sizeof(MYPUBLICSHAERDATA), MC, MYMC,iTotalCount, pData->m_dataType,pData->m_strStkLabel);

	MYMC = MC;
	iTotalCount++;

	//如果是逐K模式，直接调用Zk模式
	if(CALLINFO.bZkFlag==true)	return _YMD31FS(pData,MC);

	iRes=_YMD31FS_XL(pData,MC, CALLINFO.sSoftTag);
	//因为金字塔逐序列调用每次都会释放，所以直接释放内存
	//大智慧则直接保留MC变量，以及所有的静态变量，供下次调用使用
	if (CALLINFO.bJztFlag == true) {
		delete MC;
		MC = NULL;
	}

	return iRes;
}
__declspec(dllexport) int WINAPI D31FS1(CALCINFO* pData)
{
	return _YMD31FS_1(pData);
}
void fs_printf(FILE *fp, char *fmt, ...)
{
	va_list argptr;
	char sTemp[4096];

	va_start(argptr, fmt);
	vsprintf(sTemp, fmt, argptr);
	va_end(argptr);

	fprintf(fp, "%s", sTemp);

	printf("%s", sTemp);
}

void log_printf(int iLevel,char *fmt, ...)
{
	if (CALLINFO.iLogLevel == 0) return;

	//参数指定打印所有日志，或参数指定程序中规定的日志
	if (CALLINFO.iLogLevel == 1 ||iLevel==1|| CALLINFO.iLogLevel==iLevel) {

		static FILE *fp = NULL;
		if (fp == NULL) fp = fopen("D:\\ymd31.log", "a+");

		va_list argptr;
		char sTemp[4096], sHostTime[15], sMSec[4];

		GetHostTimeX(sHostTime, sMSec);

		va_start(argptr, fmt);
		vsprintf(sTemp, fmt, argptr);
		va_end(argptr);

		fprintf(fp, "%s.%s %s", sHostTime, sMSec, sTemp);

		fflush(fp);
	}
}
bool ValidShStockCode(char sStockCode[])
{
	/*
	去掉11开头的债券
	if((sStockCode[0] == '6' && sStockCode[1] == '0')||
	(sStockCode[0] == '1' && sStockCode[1] == '1')) return true;
	*/
	if (sStockCode[0] == '6' && sStockCode[1] == '0') return true;

	return false;
}

bool ValidSzStockCode(char sStockCode[])
{
	/*
	去掉12开头的债券
	if((sStockCode[0] == '3' && sStockCode[1] == '0')||
	(sStockCode[0] == '0' && sStockCode[1] == '0')||
	(sStockCode[0] == '1' && sStockCode[1] == '2')) return true;
	*/
	if ((sStockCode[0] == '3' && sStockCode[1] == '0') ||
		(sStockCode[0] == '0' && sStockCode[1] == '0')) return true;

	return false;
}
void GetD31LongCode(char sStockCode[], char sLongCode[], int *piGzInd)
{
	int iStockCode = atoi(sStockCode);

	*piGzInd = -1;
	strcpy(sLongCode, "");

	if (iStockCode == 0) {
		if (strncmp(sStockCode, "IF", 2) == 0 ||
			strncmp(sStockCode, "IH", 2) == 0 ||
			strncmp(sStockCode, "IC", 2) == 0 ||
			strncmp(sStockCode, "CN", 2) == 0 ||
			strncmp(sStockCode, "RB", 2) == 0) {

			strcpy(sLongCode, "SH");
			strncpy(sLongCode + 2, sStockCode, 2);
			strcpy(sLongCode + 4, "00");
		}
		else if (strncmp(sStockCode, "RU", 2) == 0 ||
			strncmp(sStockCode, "PP", 2) == 0)
			strcpy(sLongCode, "SHIC00");
		else if (strncmp(sStockCode, "ZC5",3) == 0 ||
			strncmp(sStockCode, "ZP5",3) == 0)
			strcpy(sLongCode, "SHIH00");
	}
	else if (ValidShStockCode(sStockCode))
		sprintf(sLongCode, "SH%s", sStockCode);
	else if (ValidSzStockCode(sStockCode))
		sprintf(sLongCode, "SZ%s", sStockCode);
	else if (iStockCode >= 900000)
		sprintf(sLongCode, "B$%s", sStockCode);

	if (strcmp(sLongCode, "SHIC00") == 0)		*piGzInd = 0;
	else if (strcmp(sLongCode, "SHIH00") == 0)	*piGzInd = 1;
	else if (strcmp(sLongCode, "SHIF00") == 0)	*piGzInd = 3;
	else if (strcmp(sLongCode, "SHCN00") == 0)	*piGzInd = 4;

}
/*
BOOL PathExist(CString sPath)
{
	DWORD dwAttr = GetFileAttributes(sPath);

	if (dwAttr&FILE_ATTRIBUTE_DIRECTORY)	return TRUE;

	return FALSE;
}
bool MyPathExist(char sPath[])
{
	CString tmpPath = CString(sPath);

	if (PathExist(tmpPath) == TRUE) return true;
	return false;
}
**/
bool MyPathExist(char sPath[])
{
	if (_access(sPath, 0) == 0) return true;
	return false;
}

D31MemCache::D31MemCache(bool bMemFlag)
{
	int iNotFoundCnt = 0;
	string strCur, strPath;
	int iWeekDays;
	CIniFile ini;

	m_MemFlag = bMemFlag;
	ini.SetFileName("FMLDLL.INI");

	m_realPath = ini.GetString("ef", "realtime_path", "d:\\ymdll\\gp\\nymd31fs");
	m_hisPath = ini.GetString("ef", "history_path", "d:\\ymdll\\gxp\\nymd31fs");

	log_printf(2, "realPath=%s,hisPath=%s.\n", m_realPath.c_str(), m_hisPath.c_str());
	//支持目录配置或带反斜杠、不带反斜杆
	if (m_realPath.substr(m_realPath.size() -1, 1) == "\\")
		m_realPath = m_realPath.substr(0, m_realPath.size() - 1);
	if (m_hisPath.substr(m_hisPath.size() -1, 1) == "\\")
		m_hisPath = m_hisPath.substr(0, m_hisPath.size() - 1);

	log_printf(2, "-----------realPath=%s,hisPath=%s.\n", m_realPath.c_str(), m_hisPath.c_str());

	m_displayDays = ini.GetInt("ef", "display_days", 0);
	m_displayBegin = ini.GetInt("ef", "display_begin", 20180701);
	m_displayEnd = ini.GetInt("ef", "display_end", 20180704);

	if (m_displayDays == 0) {
		string strBegin, strEnd;
		time_t tBeginTime, tCurTime;

		strBegin = to_string(m_displayBegin) + "000000";
		strEnd = to_string(m_displayEnd) + "000000";

		tBeginTime = tGetTime((char*)strBegin.c_str());
		tCurTime = tGetTime((char*)strEnd.c_str());

		while (tCurTime >= tBeginTime) {

			iWeekDays = GetWeekDays(tCurTime);

			//log_printf(2,"curtime=%s bgntime=%s sesb=(%s,%s) weekdays=%d,dd=%d,db=%d,de=%d\n",
			//	ArcTimeS(tCurTime).c_str(), ArcTimeS(tCurTime).c_str(),
			//	strEnd.c_str(),strBegin.c_str(),iWeekDays,m_displayDays,m_displayBegin,m_displayEnd);

			//跳过周六和周日；；节假日以后再支持了
			if (iWeekDays == 0 || iWeekDays == 6) {
				tCurTime = tCurTime - 86400;
				continue;
			}

			strCur = ArcTimeS(tCurTime);

			strPath = m_hisPath + "\\" + strCur.substr(0, 8);

			if (MyPathExist((char*)strPath.c_str()) == false) {

				tCurTime = tCurTime - 86400;

				//如果居然有5天的数据都缺了，就不做了
				if (++iNotFoundCnt >= 5) break;

				continue;
			}

			m_vecHisDate.push_back(strCur.substr(0, 8));
			tCurTime = tCurTime - 86400;
		}

	}
	else {
		int iDays = m_displayDays;
		time_t tCurTime = tGetHostTime() - 86400;

		while (iDays > 0) {
			iWeekDays = GetWeekDays(tCurTime);

			//跳过周六和周日；；节假日以后再支持了
			if (iWeekDays == 0 || iWeekDays == 6) {
				tCurTime = tCurTime - 86400;
				continue;
			}

			strCur = ArcTimeS(tCurTime);

			strPath = m_hisPath + "\\" + strCur.substr(0, 8);


			log_printf(2,"path=%s,curtime=%s bgntime=%s weekdays=%d,dd=%d,db=%d,de=%d\n",
				strPath.c_str(),
				ArcTimeS(tCurTime).c_str(), ArcTimeS(tCurTime).c_str(),
				iWeekDays,m_displayDays,m_displayBegin,m_displayEnd);

			if (MyPathExist((char*)strPath.c_str()) == false) {

				log_printf(2,"access fail path=%s.\n", strPath.c_str());

				tCurTime = tCurTime - 86400;

				//如果居然有连续7天的数据都缺了，就不做了
				if (++iNotFoundCnt >= 7) break;

				continue;
			}
			//如果找到了，就说找到了
			iNotFoundCnt = 0;

			log_printf(2,"---------path=%s,curtime=%s bgntime=%s weekdays=%d,dd=%d,db=%d,de=%d\n",
				strPath.c_str(),
				ArcTimeS(tCurTime).c_str(), ArcTimeS(tCurTime).c_str(),
				iWeekDays, m_displayDays, m_displayBegin, m_displayEnd);

			m_vecHisDate.push_back(strCur.substr(0, 8));

			tCurTime = tCurTime - 86400;
			iDays--;
		}
	}
}
void D31MemCache::FreeStockCache(StockCache_t *pStockCache)
{
	D31Item_t *d;

	for (auto it = pStockCache->mapMin.begin(); it != pStockCache->mapMin.end(); it++) {
		d = &it->second;
		delete d;
//		it->second = NULL;
	}
	
	pStockCache->mapMin.clear();
}
D31MemCache::~D31MemCache()
{
//	StockCache_t *pStockCache;
/*
	for (int i = 0; i < m_vecHisDate.size(); i++) {
		delete &m_vecHisDate[i];
	}
*/
	m_vecHisDate.clear();

/*	for (auto it = m_mapCache.begin(); it != m_mapCache.end(); it++) {

		pStockCache = &it->second;
		
		FreeStockCache(pStockCache);

		delete  pStockCache;

	}
*/
	m_mapCache.clear();

}
int D31MemCache::LoadFile2MapCache(StockCache_t &c, string strFileName)
{
	int i = 0;
	D31Item_t d;

	ifstream f(strFileName, ios::binary);

	if (!f.is_open()) {
		log_printf(3, "Error opening file %s for read\n", strFileName.c_str());
		return 0;
	}

	//将IniFile文件数据读到FileContainer
	while (!f.eof()) {

		f.read((char*)&d, sizeof(d));

		d.datatime = tLocal2StdTime(d.datatime);
		c.mapMin.insert({ d.datatime,d });

		if (i % 50 == 0 || i == 241)
			log_printf(3, "file name =%s time=%d(%s)\n",
				strFileName.c_str(), d.datatime, ArcTimeS(d.datatime).c_str());
		i++;
	}

	f.close();

	return 0;
}

int D31MemCache::LoadHisData2MapCache(StockCache_t &c)
{
	string strFileName;

	for (int i = 0; i < (int)m_vecHisDate.size(); i++) {

		strFileName = m_hisPath + "\\" + m_vecHisDate[i] + "\\" + c.strLongCode + ".ymd31";

		log_printf(3,"load file name =%s\n",strFileName.c_str() );

		LoadFile2MapCache(c, strFileName);
	}
	return 0;
}
int D31MemCache::LoadMemData2MapCache(StockCache_t &c, int iGzInd, int iIndex)
{
	int i, iLastTime = c.nMaxTime, iCurPos = c.iCurPos;
	D31Item_t *d, *p, t;

	for (i = iCurPos; i < 241; i++) {

		if (iGzInd >= 0)
			d = &(SHARE_TDFDATA.d_newgz[iGzInd].daydata[i]);
		else
			d = &(SHARE_TDFDATA.d_new[iIndex].daydata[i]);


		if (d->datatime == 0) continue;

		if (d->datatime < iLastTime) continue;

		log_printf(3, "dt=%d(%s) lt=%d(%s) mt=%d(%s)\n",
			tLocal2StdTime(d->datatime), ArcTimeS(tLocal2StdTime(d->datatime)).c_str(),
			tLocal2StdTime(iLastTime), ArcTimeS(tLocal2StdTime(iLastTime)).c_str(),
			tLocal2StdTime(c.nMaxTime), ArcTimeS(tLocal2StdTime(c.nMaxTime)).c_str());

		//如果最后一分钟的数据是合适合适的，则刷新
		if (d->datatime == iLastTime) {
			//接收程序将这个程序转换为Local格式了
			p = &(c.mapMin.find(tLocal2StdTime(iLastTime))->second);

			log_printf(3, "p=%x,d=%x pt=%d(%s) bt=%d(%s).\n",
				(void*)p, (void*)d,
				p->datatime,ArcTimeS(p->datatime).c_str(),
				d->datatime, ArcTimeS(d->datatime).c_str());
			*p = *d;
			p->datatime= tLocal2StdTime(p->datatime);
			continue;
		}

		//如果还有比iLastTime大的数据，则更新时间和下标
		if (d->datatime>c.nMaxTime) {
			c.nMaxTime = d->datatime;
			c.iCurPos = i;
		}
		t = *d;
		//接收程序将这个程序转换为Local格式了
		t.datatime = tLocal2StdTime(t.datatime);
		c.mapMin.insert({ t.datatime,t });

	}
	return 0;
}
StockCache_t * D31MemCache::GetD31StockCache(string longCode, int iGzInd, int iIndex)
{
	StockCache_t c;
	MapStockCache::iterator it = m_mapCache.find(longCode);

	//如果完全找不到这个map
	if (it == m_mapCache.cend()) {
		//从文件和共享内存中，加载这些数据

		c.strLongCode = longCode;
		c.iCurPos = 0;
		c.nMaxTime = 0;

		LoadHisData2MapCache(c);

		if (m_MemFlag == false) {
			string strFileName = m_realPath + "\\" + longCode + ".ymd31";

			log_printf(3,"load curfile name =%s\n", strFileName.c_str());

			LoadFile2MapCache(c, strFileName);
		}

		m_mapCache.insert({ c.strLongCode,c });
		it = m_mapCache.find(longCode);

		log_printf(3,"stockcode=%s memcachecnt=%d\n", longCode.c_str(), c.mapMin.size());
	}


	log_printf(3,"----------------------stockcode=%s memcachecnt=%d\n", longCode.c_str(), c.mapMin.size());

	//如果是内存模式，每次都需要刷新最新记录之后的内容
	if (m_MemFlag == true)
		LoadMemData2MapCache(it->second, iGzInd, iIndex);

	return &(it->second);
}
/**
//四个参数的绝对值,number的缩写
n1 = n2 = n3 = n4 = 0;
//四个参数的符号，signed的缩写
s1 = s2 = s3 = s4 = 1;

//从数据结构中取出参数
if (pData->m_pfParam1 == NULL) return -1;
n1 = (int)*pData->m_pfParam1;
if (pData->m_pfParam2 != NULL)	n2 = (int)(*pData->m_pfParam2);
if (pData->m_pfParam3 != NULL)	n3 = (int)(*pData->m_pfParam3);
if (pData->m_pfParam4 != NULL)	n4 = (int)(*pData->m_pfParam4);

//将参数的符号的绝对值分开，便于后续计算
if (n1 < 0) { s1 = -1; n1 = -n1; }
if (n2 < 0) { s2 = -1; n2 = -n2; }
if (n3 < 0) { s3 = -1; n3 = -n3; }
if (n4 < 0) { s4 = -1; n4 = -n4; }
**/
int InitMyParamArray(CALCINFO* pData, int n[4], int s[4])
{
	n[0] = n[1] = n[2] = n[3] = 0;
	s[0] = s[1] = s[2] = s[3] = 1;

	//从数据结构中取出参数
	if (pData->m_pfParam1 == NULL) return -1;
	n[0] = (int)*pData->m_pfParam1;
	if (pData->m_pfParam2 != NULL)	n[1] = (int)(*pData->m_pfParam2);
	if (pData->m_pfParam3 != NULL)	n[2] = (int)(*pData->m_pfParam3);
	if (pData->m_pfParam4 != NULL)	n[3] = (int)(*pData->m_pfParam4);

	//将参数的符号的绝对值分开，便于后续计算
	if (n[0] < 0) { s[0] = -1; n[0] = -n[0]; }
	if (n[1] < 0) { s[1] = -1; n[1] = -n[1]; }
	if (n[2] < 0) { s[2] = -1; n[2] = -n[2]; }
	if (n[3] < 0) { s[3] = -1; n[3] = -n[3]; }

	return 0;

}
float GetD31IndexValue(D31Item_t *d, int n[4], int s[4])
{
	float fValue = 0;

	fValue += d->data[n[0] - 2] * s[0];
	if (n[1] != 0)fValue += d->data[n[1] - 2] * s[1];
	if (n[2] != 0)fValue += d->data[n[2] - 2] * s[2];
	if (n[3] != 0)fValue += d->data[n[3] - 2] * s[3];

	return fValue;
}
//只支持金字塔软件，不支持大智慧，大智慧没有逐K模式
int  _YMD31FS(CALCINFO* pData, D31MemCache *MC)
{
	D31Item_t *d;
	StockCache_t *pStockCache;
	float fValue;
	char sLongCode[128];
	int  kBgnTime, kPosTime,n[4],s[4], iGzInd, iIndex = 0, iStockCode,iOffset;

	//如果初始化参数失败则，返回错误
	if (InitMyParamArray(pData, n, s) < 0) return -1;

	//通过传如的短的股票代码，生成长命名方式的股票代码，用于生成文件名
	//通过股票代码，得到股指的索引号，iGzInd==-1表示，非制定股指股票
	GetD31LongCode((char*)pData->m_strStkLabel, sLongCode, &iGzInd);

	if (strlen(sLongCode) == 0)return -2;

	iStockCode = atoi(pData->m_strStkLabel);

	//如果是制定的上海和深圳的规范股票代码，
	//则从共享内存中通过股票代码，得到索引的数值,娶不到则index=0;
	if (ValidShStockCode((char*)pData->m_strStkLabel) ||
		ValidSzStockCode((char*)pData->m_strStkLabel)) {

		if (SHARE_POOL->tdfdoing == 1)
			iIndex = SHARE_TDFDATA.codeindex[iStockCode];
	}

	kPosTime = pData->m_pData[pData->m_dwBarpos].m_time;

	if (pData->m_dwBarpos > 0)
		kBgnTime = pData->m_pData[pData->m_dwBarpos - 1].m_time;
	else	kBgnTime = kPosTime;

	log_printf(5,"hellozk %d(%s) %d(%s) %s memflag=%d barpos=%d.\n", 
		kBgnTime,ArcTimeS(kBgnTime).c_str(),kPosTime, ArcTimeS(kPosTime).c_str(),sLongCode,
		(int)MC->m_MemFlag, pData->m_dwBarpos);

	fValue = 0;

	//找到开始时间和结束时间的map iterator，循环取数累计
	MapD31_t::iterator itCur, itPos, itBgn;

	pStockCache = MC->GetD31StockCache(string(sLongCode), iGzInd, iIndex);

	//对于是日线、周线等已日期整数倍的周期,获得时间偏移常量
	//将K线的开始和结束时间变换为指定合适的偏移
	iOffset = GetSerialOffsetByDataType(pData);
	kBgnTime += iOffset;	kPosTime += iOffset;

#ifndef  OLD_CALC_MAP_FIND
	//返回键值>给定元素的第一个位置
	itCur = pStockCache->mapMin.upper_bound(kBgnTime);
	//返回键值>=给定元素的第一个位置
	itPos = pStockCache->mapMin.lower_bound(kPosTime);

	while (itCur != pStockCache->mapMin.cend()) {

		d = &(itCur->second);

		//如果是特定指标，则取一个最终的数值
		if (n[0] >= 122 && n[0] <= 135)	fValue = 0;

		fValue += GetD31IndexValue(d, n, s);

		if (itCur == itPos) break;

		itCur++;
	}
	*pData->m_pResultBuf = fValue;
#else

	itBgn = pStockCache->mapMin.find(kBgnTime);
	itPos = pStockCache->mapMin.find(kPosTime);
	//如果k线的开始或结束找不到，则直接返回为0
	if (itPos == pStockCache->mapMin.cend()) {

		if (itBgn == pStockCache->mapMin.cend()) {
			*pData->m_pResultBuf = 0.0;
			return 1;
		}
		
		itCur = itBgn;
		itCur++;

		while (1) {
			if (itCur == pStockCache->mapMin.cend()) break;

			d = &(itCur->second);

//			if (strLongCode == "SHIC00") {
				log_printf(5,"hellozk pos0 %d(%s) %d(%s) %s n0=%d,s0=%d v=%8.3f.\n",
					d->datatime, ArcTimeS(d->datatime).c_str(), kPosTime, ArcTimeS(kPosTime).c_str(), sLongCode,
					n[0], s[0], d->data[n[0] - 2] * s[0]);
//			}
			//如果是特定指标，则取一个最终的数值
			if (n[0] >= 122 && n[0] <= 135)	fValue = 0;

			fValue += GetD31IndexValue(d, n, s);

			if (itCur == itPos) break;

			itCur++;
		}
	}
	else{
		itCur = itPos;

		while (1) {
			if (itCur == pStockCache->mapMin.cbegin()) break;

			if (itCur == itBgn) break;

			d = &(itCur->second);

//			if (strLongCode == "SHIC00") {
				log_printf(5,"hellozk pos1 %d(%s) %d(%s) %s n0=%d,s0=%d,v=%-8.3f,v62=%-8.3f,v12=%-8.3f.\n",
					d->datatime, ArcTimeS(d->datatime).c_str(), kPosTime, ArcTimeS(kPosTime).c_str(), sLongCode,
					n[0],s[0],d->data[n[0] - 2] * s[0], d->data[60], d->data[10]);
//			}

			//如果是特定指标，则取一个最终的数值
			if (n[0] >= 122 && n[0] <= 135)	fValue = 0;

			fValue += GetD31IndexValue(d, n, s);

			itCur--;
		}
	}
	*pData->m_pResultBuf = fValue;
#endif
	log_printf(5, "hellozk_v %d(%s) %d(%s) %s n0=%d,value=%6.2f offset=%d.\n",
		kBgnTime, ArcTimeS(kBgnTime).c_str(), kPosTime, ArcTimeS(kPosTime).c_str(), sLongCode, n[0],
		fValue, iOffset);
	return 1;
}

int GetSerialOffsetByDataType(CALCINFO* pData)
{
	int iOffset = 0;

	switch (pData->m_dataType)
	{
	case DAY_DATA://日线
	case WEEK_DATA://周线
	case MONTH_DATA://月线
	case YEAR_DATA://年线
	case MULTIDAY_DATA://多日线
	case QUARTER_DATA://季线
	case SEMIYEAR_DATA://半年线
	case SOLARTERM_DATA://节气线
		iOffset = 15 * 3600; //直接将时间切换到收盘时间
		break;
	default:
		break;
	}
	return iOffset;
}
int GetSerialDataPos(CALCINFO* pData, int iOffset,int iBgnPos, int nTradeTime)
{
	//对于是日线、周线等已日期整数倍的周期
	//将nTradeTime向前平移15小时，相当于nEndTime直接从每天的00:00:00-->15:00:00
	nTradeTime -=  iOffset;

	//如果不是标准的KLine的 time_t，说明是大智慧的软件,
	//将nTradeTime转换为非标准的time_t
	if (IsValidKLineStdTime(pData->m_pData[0].m_time) == false)
		nTradeTime = tStd2LocalTime(nTradeTime);

	if (nTradeTime <= pData->m_pData[0].m_time) return -1;

	for (int i = iBgnPos; i<pData->m_nNumData ; i++) {

		if (iBgnPos >= (pData->m_nNumData-4))
			log_printf(5, "i=%d,nd=%d,tt=%d(%d,%s),off=%d,ct=%d(%d,%s) ct1=%d(%d,%s) %d.\n",
				i, pData->m_nNumData,
				nTradeTime,
				tLocal2StdTime(nTradeTime),
				ArcTimeS(tLocal2StdTime(nTradeTime)).c_str(),
				iOffset,
				pData->m_pData[i].m_time,
				tLocal2StdTime(pData->m_pData[i].m_time),
				ArcTimeS(tLocal2StdTime(pData->m_pData[i].m_time)).c_str(),
				pData->m_pData[i + 1].m_time,
				tLocal2StdTime(pData->m_pData[i+1].m_time),
				ArcTimeS(tLocal2StdTime(pData->m_pData[i + 1].m_time)).c_str(),
				(int)IsValidKLineStdTime(pData->m_pData[0].m_time));

		if (nTradeTime > pData->m_pData[i].m_time){
			if (i == pData->m_nNumData - 1) return i;
			if (nTradeTime <= pData->m_pData[i + 1].m_time) return i;
		}
	}
	return -1;
}
bool CheckDataTypeValid(CALCINFO* pData)
{
	if (pData->m_dataType == TICK_DATA ||
		pData->m_dataType == MULTISEC_DATA) return false;

	//大智慧半年线会退出，这里屏蔽调这个功能
	if (pData->m_dataType == SEMIYEAR_DATA&&
		CALLINFO.bJztFlag == false) return false;

	return true;
}
//自动支持大智慧和金字塔软件
//在GetSerialDataPos内支持
int  _YMD31FS_XL(CALCINFO* pData, D31MemCache *MC,char sSoftTag[])
{
	D31Item_t *d;
	StockCache_t *pStockCache;
	float fValue;
	char sLongCode[128];
	int n[4], s[4], iGzInd, iIndex = 0, iStockCode, j = 0,k=0,iOffset;

	//如果初始化参数失败则，返回错误
	if (InitMyParamArray(pData, n, s) < 0) return -1;

	//通过传如的短的股票代码，生成长命名方式的股票代码，用于生成文件名
	//通过股票代码，得到股指的索引号，iGzInd==-1表示，非制定股指股票
	GetD31LongCode((char*)pData->m_strStkLabel, sLongCode, &iGzInd);

	if (strlen(sLongCode) == 0)	return -2;

	iStockCode = atoi(pData->m_strStkLabel);

	//如果是指定的上海和深圳的规范股票代码，
	//则从共享内存中通过股票代码，得到索引的数值,娶不到则index=0;
	if (ValidShStockCode((char*)pData->m_strStkLabel) ||
		ValidSzStockCode((char*)pData->m_strStkLabel)) {

		if (SHARE_POOL->tdfdoing == 1)
			iIndex = SHARE_TDFDATA.codeindex[iStockCode];
	}

	pStockCache = MC->GetD31StockCache(string(sLongCode), iGzInd, iIndex);

//	log_printf(1,"hello %s(XL) %s memflag=%d nd=%d,tb=%d(%s),te=%d(%s).\n", 
//		sSoftFlag,sLongCode,(int)MC->m_MemFlag,pData->m_nNumData,
//		pData->m_pData[0].m_time,(char*)"",
//		pData->m_pData[pData->m_nNumData-1].m_time, (char*)"");

	//做一个循环，清空序列化输出
	for (j = 0; j < pData->m_nNumData; j++)		pData->m_pResultBuf[j] = 0;

	j = 0;
	//对于是日线、周线等已日期整数倍的周期,获得时间偏移常量
	iOffset = GetSerialOffsetByDataType(pData);

	//做一个循环，将序列化的东西重新填充一遍
	for (auto it = pStockCache->mapMin.begin(); it != pStockCache->mapMin.end(); it++) {

		d = &(it->second);

		//找到K线图的位置
		j = GetSerialDataPos(pData, iOffset, j,d->datatime);

		if (j == -1) {
			j = 0;
			continue;
		}

		if(j>= (pData->m_nNumData-5))
			log_printf(5,"hello j=%d t=%d(%s) size=%ld nd=%d.\n",
				j, d->datatime, ArcTimeS(d->datatime).c_str(),
				pStockCache->mapMin.size(),pData->m_nNumData);

		k = j + 1;

		//如果已到最大值了，就不能写pData->m_pResultBuf了
		if (k == pData->m_nNumData) continue;

		fValue = GetD31IndexValue(d, n, s);

		if (n[0] >= 122 && n[0] <= 135)
			pData->m_pResultBuf[k] = fValue;
		else	pData->m_pResultBuf[k] += fValue;
	}

	log_printf(5,"hello end  %s(XL).-----------------------------------------------------.\n",
		sSoftTag);

	return 0;
}

//根据输入参数的情况，得到标准K线的time_t时间
time_t GetKLineStdTime(time_t tKLineTime)
{
	static time_t tOpenTime = 3600 * 9 + 1800, tCloseTime = 15 * 3600;
	time_t tTmp;

	tTmp = tKLineTime % 86400;

	if (tTmp == 0 || (tTmp >= tOpenTime) && (tTmp <= tCloseTime)) return tLocal2StdTime(tKLineTime);

	return tKLineTime;
}
//根据输入参数的time_t 判断是否是标准的股票K线time_t时间
bool IsValidKLineStdTime(time_t tKLineTime)
{
	static time_t tOpenTime = 3600 * 9 + 1800, tCloseTime = 15 * 3600;
	time_t tTmp;

	tTmp = tKLineTime % 86400;

	if (tTmp == 0 || (tTmp >= tOpenTime) && (tTmp <= tCloseTime)) return false;

	return true;
}


int MyLoadFile2MapCache(StockCache_t &c, string strFileName)
{
	int i = 0;
	D31Item_t d;

	ifstream f(strFileName,ios::binary);

	if (!f.is_open()) {
		log_printf(1,"Error opening file %s for read\n", strFileName.c_str());
		return 0;
	}

	//将IniFile文件数据读到FileContainer
	while(!f.eof()){

		f.read((char*)&d, sizeof(d));

		d.datatime = tLocal2StdTime(d.datatime);
		c.mapMin.insert({ d.datatime,d });

		//		if (i % 50 == 0 || i == 241)
		log_printf(1,"file name =%s time=%d(%s)\n",
			strFileName.c_str(), d.datatime, ArcTimeS(d.datatime).c_str());
		i++;
	}

	f.close();

	return 0;
}

int  _YMD31FS_1(CALCINFO* pData)
{


	//	char sHostTime[15], sMSec[4];
	static int iCount = 0;

//	if (iCount == 0) {
		log_printf(1,"gettime(\"19700101\"=%d,arctime(0)=%s %d,%d count=%d extcnt=%d\n",
			tGetTime("19700101000000"), ArcTimeS(0).c_str(),
			tLocal2StdTime(0), tStd2LocalTime(0),iCount, iTotalCount);

		log_printf(1,"hosttime=%s hostxtime=%s.t=%d(%s)\n",
			GetHostTimeS().c_str(),
			ArcTimeS(tGetTime((char*)GetHostTimeS().c_str())).c_str(),
			pData->m_pData[pData->m_nNumData-1].m_time,
			ArcTimeS(pData->m_pData[pData->m_nNumData - 1].m_time).c_str());
		try {
			if (MyPathExist((char*)"z:\\20180718") == true)
				log_printf(1,"OK FOUND NEW RULE\n");
			else    log_printf(1,"TEST FAIL.\n");
		}
		catch (...) {
			log_printf(1,"SHENQI LULU\n");
		}

		StockCache_t c;
		string fileName = "Z:\\20180716\\SH600018.ymd31";

		MyLoadFile2MapCache(c, fileName);

//	}

	iCount++; iTotalCount++;


	//	GetHostTimeX(sHostTime, sMSec);

	//	if((iCount++%10)==0){
//	log_printf(1,"the d31call_cnt=%d,%d sz=%d ind=%d dn=%d,ps=%d,datatype=%d,barpos=%d,pn=%d,sc=%s,sn=%s.\n",
//		iCount, iTotalCount, pData->m_dwSize, pData->m_bIndex,
//		pData->m_nNumData, pData->m_nParam1Start,
//		pData->m_dataType, pData->m_dwBarpos, pData->m_nNumParam,
//		pData->m_strStkLabel, pData->m_strStkName);
	//	}

	return 0;
}


SharePool::SharePool(void)
{
	if (InitSharePool() < 0)
		log_printf(1, "全局内存初始化错误！请重新启动。\n");
}
void SharePool::DetachSharePool()
{
	if (m_pSharePool != NULL) {
		UnmapViewOfFile((void*)m_pSharePool);
		m_pSharePool = NULL;
	}

}
SharePool::~SharePool(void)
{
	DetachSharePool();
}
int SharePool::InitSharePool()
{
	SHARE_POOL_t *p;
	long l = sizeof(MYPUBLICSHAERDATA);//1024*1024*300;//300M

					   //根据文件名称，映射共享内存
	m_hHandleSharePool = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE, 0, l, "npublicsharedatamap3");//publicsharedatamap
	if (m_hHandleSharePool == NULL) return -1;

	m_pSharePool = (SHARE_POOL_t*)MapViewOfFile(m_hHandleSharePool,
		FILE_MAP_ALL_ACCESS, 0, 0, l);
	if (m_pSharePool == NULL) return -1;

	p = m_pSharePool;

	//	gdataDate = p->dataday;
	if (p->autotradeinfo.allstatus >= 1 &&
		p->autotradeinfo.allstatus <= 10) return 0;

	ZeroMemory((void*)p, l);

	//变量设为0的部分就不用做了，因为前面有ZeroMemory函数
	p->gday = 1;
	p->autotradeinfo.allstatus = 1;
	for (int i = 0; i <= 4; i++)
		p->autotradeinfo.ymhistorydatareal[i] = 1;

	return 0;
}
