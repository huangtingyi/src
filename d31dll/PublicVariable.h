#pragma once

#ifndef __PublicVariable_H_INCLUDE
#define __PublicVariable_H_INCLUDE
///////////////////////////////////////////////////////////////////////////

/*
大智慧扩展函数规范V1.10
1.本规范适用于大智慧1.10标准版和专业版公式系统.
2.扩展函数用于实现系统函数不能实现的特殊算法.
3.扩展函数用windows 32位动态连接库实现,建议使用Microsoft Visual C++编程.
4.调用时在公式编辑器中写"动态库名称@函数名称"(参数表)即可,例如下面函数可以写为"DZHFUNC@MYCMALOSE"(5)
5.动态连接库名称和函数名称可以自己定义.
6.使用时可以将动态库拷贝到大智慧目录下使用.
*/
/////////////////////////////////////////////////////////////////////////////

//自定义全局数据---->
#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h> 
#include <direct.h>

//#pragma   warning(disable:4146)   
//#import   "c:\program files\common files\system\ado\msado15.dll"   named_guids   rename("EOF","adoEOF"),   rename("BOF","adoBOF")   
//#pragma   warning(default:4146)  

using namespace std;

struct REC_DZHDATA
{ 
	time_t datetime; //日期
	float zb; //指标
}  ;

struct REC_DZHREALDATA
{ 
	int zbtype;//指标分类
	char gpdm[8];
	float zb[256]; //指标
}  ;

struct BKINFO
{ 
	char  m[2];
	char  gp[10];	
};

struct BKSTRUCT
{ 
	char  gpdm[12];	
};

struct D31V
{
	long d[31];
} ;
struct D31N
{
	short d[31];
} ;
struct strfszb
{
	short transactnum1[4];
	long transactvol1[4];
	short transactnum2[4];
	long transactvol2[4];
	short ordernum1[4];
	long ordervol1[4];
	short ordernum2[4];
	long ordervol2[4];
};
struct strfszj//用于读数
{
	long fstime;
	long avgbidprice;
	long ALLBIDVOL;
	long NEWORDER1;
	long WIDTHDRAWORDER1;
	long avgaskprice;
	long ALLASKVOL;
	long NEWORDER2;
	long WIDTHDRAWORDER2;
	strfszb fszbdata;
	char temp[38];
};
struct strfsdata
{
	long fstime;
	long fsC;		
	long fsV;
	strfszb fszb;
};
struct D31ALL
{
	long day;
	int dC;
	D31V zbzbv;//逐笔主买量
	D31V zbzsv;//逐笔主卖量
	D31V zdzbv;//逐单主买量
	D31V zdzsv;//逐单主卖量	
	D31N zbzbn;//逐笔主买笔数
	D31N zbzsn;//逐笔主卖笔数
	D31N zdzbn;//逐单主买笔数
	D31N zdzsn;//逐单主卖笔数			

} ;//L2分类资金
struct L2FSZJ//用于读数
{
	long dC;
	strfszj fsl2zj[243];
};
struct L2FSZB//用于分时股票池
{
	char gpdm[9];
	long refv;
	long refD31zb;
	long refFSDDXzb;
	long dC;
	strfszj fszb[243];
};
struct INDEXDATA//股票索引表
{
	char dm[9];
	long start;
	long end;
};	

static const int CSHGPSL=1000;//股票数量
static const int CSZGPSL=1400;//股票数量
static const int CD31TS=150;//最大天数
struct DAYMAPPINGPOOL//动态股票池 ，日线资金
{
	long gpcount;
	INDEXDATA gpindex[(CSHGPSL+CSZGPSL)];
	D31ALL d31zj[(CSHGPSL+CSZGPSL)*CD31TS];
};
struct REALMAPPINGPOOL//股票池，分时指标
{
	long gpcount;
	L2FSZB fszb[2400];
};

struct daytorealzb
{
	long t;
	float data[4];
};
//上海日线转分时池
struct DAYTOREAL
{
	char dm[9];
	D31ALL fszb[240];
};
struct SHDAYTOREALMAPPINGPOOL
{
	long gpcount;
	DAYTOREAL daytorealzb[CSHGPSL];
};
//深圳日线转分时池
struct SZDAYTOREALMAPPINGPOOL
{
	long gpcount;
	DAYTOREAL daytorealzb[CSZGPSL];
};
struct	REALSTKINFO70//startAddress = 17574;//0x68A8A6;
{
	char dm[10];//代码;
	char jc[8];//简称;
	char space1[42];
	long rq;//更新时间	;//datetime;//4;//5;//60;//;float  +
	char space2[4];
	float zs;//昨收	le;//4;//7;//68;//;float  +
	float kp;//今开	le;//4;//8;//72;//;float  +
	float zg;//最高	le;//4;//9;//76;//;float  +
	float zd;//最低	le;//4;//10;//80;//;float  +
	float sp;//最新	le;//4;//11;//84;//;float  +
	float sl;//总手数	le;//4;//12;//88;//;float  +
	float je;//金额	le;//4;//13;//92;//;float  +
	float xss;//现手数	le;//4;//14;//96;//;float  +
	char space3[84];
	float ztj;//涨停价	le;//4;//27;//184;//;float  +
	float dtj;//跌停价	le;//4;//28;//188;//;float  +
	float np;//内盘	le;//4;//27;//192;//;float  +
	float wp;//外盘	le;//4;//28;//196;//;float  +
	float mrjg1;//买一价	le;//4;//15;//100;//;float  +
	float mrjg2;//买二价	le;//4;//16;//104;//;float  +
	float mrjg3;//买三价	le;//4;//17;//108;//;float  +
	float mrjg4;//买四价	le;//4;//32;//112;//;float  +
	float mrjg5;//买五价	le;//4;//33;//116;//;float  +
	float mrsl1;//买一量	le;//4;//18;//120;//;float  +
	float mrsl2;//买二量	le;//4;//19;//124;//;float  +
	float mrsl3;//买三量	le;//4;//20;//128;//;float  +
	float mrsl4;//买四量	le;//4;//34;//132;//;float  +
	float mrsl5;//买五量	le;//4;//35;//136;//;float  +
	float mcjg1;//卖一价	le;//4;//21;//140;//;float  +
	float mcjg2;//卖二价	le;//4;//22;//144;//;float  +
	float mcjg3;//卖三价	le;//4;//23;//148;//;float  +
	float mcjg4;//卖四价	le;//4;//36;//152;//;float  +
	float mcjg5;//卖五价	le;//4;//37;//156;//;float  +
	float mcsl1;//卖一量	le;//4;//24;//160;//;float  +
	float mcsl2;//卖二量	le;//4;//25;//164;//;float  +
	float mcsl3;//卖三量	le;//4;//26;//168;//;float  +
	float mcsl4;//卖四量	le;//4;//38;//172;//;float  +
	float mcsl5;//卖五量	le;//4;//39;//176;//float ;
	char space4[40];
};
const int d31ResultNum=5;
struct D31RESULTDATA
{
	long datetime;
	float zbzbv[d31ResultNum];
	float zbzsv[d31ResultNum];
	float zbzbn[d31ResultNum];
	float zbzsn[d31ResultNum];
	float zdzbv[d31ResultNum];
	float zdzsv[d31ResultNum];
	float zdzbn[d31ResultNum];
	float zdzsn[d31ResultNum];
	float zbzba[d31ResultNum];
	float zbzsa[d31ResultNum];
	float zdzba[d31ResultNum];
	float zdzsa[d31ResultNum];
	long gpcount;
};
struct L2DRESULTDATA
{
	long datetime;
	float avgbidprice;
	float ALLBIDVOL;
	float NEWORDER1;
	float WIDTHDRAWORDER1;
	float avgaskprice;
	float ALLASKVOL;
	float NEWORDER2;
	float WIDTHDRAWORDER2;
	float transactnum1[4];
	float transactvol1[4];
	float transactnum2[4];
	float transactvol2[4];
	float ordernum1[4];
	float ordervol1[4];
	float ordernum2[4];
	float ordervol2[4];
};
struct MINDATASTRUCT//1分钟和日线传统数据结构
{
	long datetime;
	float data[7];
};

struct TransSign
{
	int type;//策略类型
	int time;//时间
	int QCC;//提交信号
	int  CC;//持仓信号

};
struct AUTOTRADEINFO
{
	//自动交易部分
	int tradestatus;//0-未持仓，可买；1－持仓，可加仓或卖；2－发出挂单，未成交
	int signaldir;//0-未持仓，可卖；1－持仓，可加仓或买；2－发出挂单，未成交
	float signalprice;
	long signaltime;
	float inprice;
	long intime;
	float outprice;
	long outtime;
	int bscount;//持仓手数
	int bsdir;//持仓方向
	float bsprice;//持仓价格
	float currentprice;
	float highprice;
	float lowprice;
	float autooutprice;
	long currenttime;
	int tradeing;
	long allstatus;//1正常，2数据位置不对，3刷机数据滞后
	//止赢止损
	int jdzs;//绝对止损点数
	int zyd1,zys1;
	int zyd2,zys2;
	int zyd3,zys3;
	int zyd4,zys4;
	int zyd5,zys5;
	int zyzslevel;//止赢止损等级
	float ymdata;
	int znjhwnd;//是否正确切换到指南针OAMV页面
	int znjdatacount;
	long znjerrordata;
	char ymstocklabel[9];
	int ymhistorydataset[4];
	int ymhistorydatareal[4];
	int jztrunsecond;
	


};
struct YMMINL2SOURCE
{
	short ymtime;
	long data[64];//体验版64，正式版62
	short data1[2];
};

struct YMMINL2SOURCE_LXB
{
	long ymtime;
	long data[62];//体验版64，正式版62
	short data1[2];
};

struct znjMINSOURCE_LXB
{
	long ymtime;
	float data;
};

struct YMMINL2TARGET
{
	long datatime;
	float data[73];
};
struct TDFYMD31
{
	int datatime;
	float data[169];
};
struct TDFGPYMD31
{
	long datatime;
	float data[40];
};
struct GPPRP
{
	long datatime;
	float data[54];
};
struct GPYMD31
{
	long datatime;
	float data[199];
};
struct DZHZFMINDATA
{
	long datatime;
	int num[2];
	float data[24];//float data[20];
};

struct YMDAYDATA
{
	int daydatacount;
	YMMINL2TARGET daydata[240];
};
struct TDFDAYDATA
{
	int daydatacount;
	char stocklabel[10];
	TDFYMD31 daydata[241];
};
struct D31New
{	
	TDFYMD31 daydata[241];
};
struct Codetonum
{
	int code;
	int index;

};
struct TDFALL
{
	//TDFDAYDATA alldata[10];
	D31New d_newgz[10];
	D31New d_new[4000];
	int codeindex[700000];
};


struct YMDAYDATA240
{
	YMMINL2TARGET daydata[240];
};
struct MYPUBLICSHAERDATA
{
	int agbkcount;//A股板块股票数
	int gday;
	char dataday[9];
	//YMMINL2TARGET ymmin[24];
	AUTOTRADEINFO autotradeinfo;
	int ymfsdataselect[4];
	YMDAYDATA ymfsdata1[4],ymfsdata2[4];
	TDFALL tdf_data;	
	int tdfdoing;
	int saveymfsfile;
	TransSign ts[10];
};
struct CVADATA	
{
	time_t	m_time;			//时间,UCT
	//float	m_fOpen;		//开盘
	//float	m_fHigh;		//最高
	//float	m_fLow;			//最低
	float	m_fClose;		//收盘
	float	m_fVolume;		//成交量
	float	m_fAmount;		//成交额
} ;

struct QHBSINFO
{
	//自动交易记录
	float inprice;
	long intime;
	char intimestr[17];
	float outprice;
	long outtime;
	char outtimestr[17];
	char bsdir[3];//持仓方向
	int bscount;//持仓手数
	char intype[9];//开仓方式
	char outtype[9];//平仓方式
	float pointcount;
	float traderesult;
};

#endif //__PublicVariable_H_INCLUDE

