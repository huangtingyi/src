#ifndef __EXTEND_DATA_H__
#define __EXTEND_DATA_H__

#include "StockFunc.h"
#include "PublicVariable.h"
#include "IniFile.h"
#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
	__declspec(dllexport) BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
	__declspec(dllexport) int WINAPI D31FS(CALCINFO* pData);
	__declspec(dllexport) int WINAPI D31FS1(CALCINFO* pData);

#ifdef __cplusplus
}
#endif //__cplusplus

int GetHostTimeX(char sHostTime[15], char sMSec[4]);
time_t tGetHostTime();

typedef struct TDFYMD31 D31Item_t;
typedef vector <TDFYMD31>            VecYmd31_t;
typedef struct LongCodeStruct
{
	string  strLongCode;	//长股票代码
	time_t  tLastAccess;	//最后访问时间
	VecYmd31_t vecMin;
} LongCode_t;

typedef std::map<string, LongCode_t> MapLongCode;

//按datatime索引数据
typedef map <int, D31Item_t>            MapD31_t;
typedef struct StockCacheStruct
{
	int	nMaxTime;	//内存模式加载数据的最大时间
	int	iCurPos;	//内存模式加载数据的位置
	string  strLongCode;	//长股票代码
	MapD31_t mapMin;
} StockCache_t;
//按stockcode索引数据
typedef std::map<string, StockCache_t> MapStockCache;

class D31MemCache
{
public:
	string m_realPath;
	string m_hisPath;
	int	m_displayDays;
	int	m_displayBegin;
	int	m_displayEnd;
	bool	m_MemFlag;
	vector<string> m_vecHisDate;

	MapStockCache m_mapCache;

	D31MemCache(bool bMemFlag);
	void FreeStockCache(StockCache_t *pStockCache);
	virtual ~D31MemCache();

	int LoadFile2MapCache(StockCache_t &c, string strFileName);
	int LoadHisData2MapCache(StockCache_t &c);

	int LoadMemData2MapCache(StockCache_t &c, int iGzInd, int iIndex);

	//	D31Item_t *GetD31Item(string longCode, int nTradeTime, int iGzInd, int iINdex);
	//	MapD31_t::iterator GetD31It(string longCode, int nTradeTime, int iGzInd, int iIndex);

	StockCache_t * GetD31StockCache(string longCode, int iGzInd, int iIndex);
};

#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
void fs_printf(FILE *fp, char *fmt, ...);
/*
loglevel
0 not log
1 all log
2 init path
3 load file name
4 init callinfo
5 d31fs main
6 other log
*/
void log_printf(int iLevel,char *fmt, ...);

bool CheckDataTypeValid(CALCINFO* pData);

//根据输入参数的情况，得到标准K线的time_t时间
time_t GetKLineStdTime(time_t tKLineTime);
//根据输入参数的time_t 判断是否是标准的股票K线time_t时间
bool IsValidKLineStdTime(time_t tKLineTime);

int GetSerialOffsetByDataType(CALCINFO* pData);
int GetSerialDataPos(CALCINFO* pData, int iOffset, int iBgnPos, int nTradeTime);

//逐K模式调用
int _YMD31FS(CALCINFO* pData,D31MemCache *MC);
//序列模式调用
int  _YMD31FS_XL(CALCINFO* pData,D31MemCache *MC, char sSoftTag[]);

int  _YMD31FS_1(CALCINFO* pData);

extern D31MemCache *MYMC;

#define MAX_KLINE_CNT 8192



typedef MYPUBLICSHAERDATA SHARE_POOL_t;

class SharePool
{
public:
	SharePool(void);
	~SharePool(void);
	int InitSharePool();
	void DetachSharePool();

	//共享内存池
	HANDLE m_hHandleSharePool;
	SHARE_POOL_t * m_pSharePool;

};

#endif

