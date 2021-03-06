#ifndef __WWTINY_H__
#define __WWTINY_H__

#include <time.h>

//#include <sys/time.h>

#include <string>
using namespace std;

#include "datatype.h"

time_t tGetTime(char sAnswerTime[15]);
int sFormatTime( time_t ttime,char sTime[15]);
int ArcTime(time_t t,char sTime[15]);
int ChkTime(char sAnswerTime[15]);
int ChkDate(char sDate[9]);
int GetHostTime(char sHostTime[15]);
int GetHostTimeX(char sHostTime[15],char sMiniSec[4]);

time_t tGetHostTime();

int LeapYear(char sTempDate[15]);

/* 将YYYYMMDDHH24MISS格式的字串增加iSecs秒(负值时，表示减) */
int AddTimes(char sTempDate[15], int iOffset);
int AddMinutes(char sTempDate[15], int iOffset);
int AddHours(char sTempDate[15], int iOffset);

int AddDates(char sTempDate[15], int iOffset);
int AddDays(char sTempDate[15], int iOffset);
int AddWeeks(char sTempDate[15], int iOffset);
int AddTen(char sTempDate[15]);

int AddMonths(char sTempDate[15], int iOffset);
int AddQuarters(char sTempDate[15], int iOffset);
int AddYears(char sTempDate[15], int iOffset);

/*yyyymmddhh24miss----------->yyyy/mm/dd hh24:mi:ss*/
/*注意本函数不加串结束符*/
int formatdatetime_simp_compl(char sAnswerTime[],char sOutput[]);
/*yyyy/mm/dd hh24:mi:ss------>yyyymmddhh24miss*/
/*注意本函数不加串结束符*/
int formatdatetime_compl_simp(char sAnswerTime[],char sOutput[]);
/*yyyy-mm-dd hh24:mi:ss------>yyyymmddhh24miss*/
/*注意本函数不加串结束符*/
int formatdatetime_long_simp(char sAnswerTime[],char sOutput[]);
/*yyyy-mm-dd------>yyyymmdd*/
int formatdate_long_simp(char sAnswerTime[],char sOutput[]);
/*hh24:mi:ss------>hh24miss*/
int formattime_long_simp(char sAnswerTime[],char sOutput[]);

void TrimLeft(char * s);
void TrimRight(char * s);
void AllTrim(char *s);
void TrimDate(char sDate[15]);

void TrimLeftX(char * s,char X);
void TrimRightX(char *s,char X);
void TrimAllX(char *s,char X);
void RightPad(char sTemp[],int iLen,char c);
void LeftPad(char sTemp[],int iLen,char c);
void TrimAllS(char *s,char *S);
void TrimAll(char *s);
void Upper(char *s);
void Lower(char *s);
void WaitSec(int iSec);
int  Str2Int(char sTemp[]);

int SearchOffString(char sRecord[],int iOffset,int iSeparator,char sTemp[]);
int ParseArgv(char sRecord[], int chr);

/*判断是否有其他的选项,TRUE存在,FALSE不存在*/
int ExistOtherOption(int argc,char *argv[],char sOptionFilter[]);
/*是否存在某个选项 TRUE存在,FALSE不存在*/
int ExistOption(int argc,char *argv[],char sOptionFilter[]);
/*获取某选项是否存在,FOUND,NOTFOUND,选项后的串给出*/
int SearchOptionStr(int argc,char *argv[],char sOptionFilter[],char sOption[]);
/*获取某选项是否存在,FOUND,NOTFOUND,选项后的串给出*/
int SearchOptionInt(int argc,char *argv[],char sOptionFilter[],int *piOption);

bool LLike(char sInput[],char sVal[]);
bool RLike(char sInput[],char sVal[]);

#define PARSE_ARRAY_LEN 50
#define PARSE_ITEM_LEN  60
extern char ParseToken[PARSE_ARRAY_LEN][PARSE_ITEM_LEN];

void ReverseAll(char *s);
void DelSubStr(char sInput[],char sSub[]);
void Replace(char sInput[],char x,char y);
void ReplaceStr(char sInput[],char x[],char y[]);

bool Like(char sStr[],char P[]);
bool LikeX(char sStr[],char P[]);

int HostTimeEff(char sEffDate[],char sExpDate[]);

#define MY_GET_MILLI_SEC(x)	(x%1000)
#define MY_GET_SEC(x)		((x%100000)/1000)
#define MY_GET_MIM(x)		((x%10000000)/100000)
#define MY_GET_HOUR(x)		(x/10000000)

/*某个时间加上iMilliSec数，iMillicSec参数最大支持*/
int iAddMilliSec(int iTime,int iMilliSec);
int nGetHostCurTime();
//某个求两个时间的diff差值
//int iSubMilliSec(int iTime,int iTime1);

//某个求两个时间的diff差值
int iDiffnTime(int iTime,int iTime1);

long nGetHostTime();

#include "stdint.h"

inline int64_t yuan2percentFen(const double yuan)
{
	return int64_t((yuan + 0.00005) * 10000);
}

string TrimLeft(string str);
string TrimRight(string str);
string TrimAll(string str);

string GetHostTimeS();
string GetHostDateS();
string ArcTimeS(time_t ttime);
int GetWeekDays(time_t ttime);
//返回东多少时区偏移秒
time_t tGetTimeZoneOff();
//标准定义的time_t转换为当地定义的time_t
time_t tStd2LocalTime(time_t tStdTime);
time_t tLocal2StdTime(time_t tStdTime);

class LogPrt
{
public:
	void SetLogName(string FileName);
	void log_printf(int iLevel, char *fmt, ...);

	LogPrt(int iLevel=0);
	virtual ~LogPrt() {
		if (m_fp != NULL) { fclose(m_fp); m_fp = NULL; }
	};

	//log file
	int m_iLogLevel;
	string m_sLogName;
	FILE *m_fp;
};


#endif
