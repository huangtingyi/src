#pragma once

#ifndef __PublicVariable_H_INCLUDE
#define __PublicVariable_H_INCLUDE
///////////////////////////////////////////////////////////////////////////

/*
���ǻ���չ�����淶V1.10
1.���淶�����ڴ��ǻ�1.10��׼���רҵ�湫ʽϵͳ.
2.��չ��������ʵ��ϵͳ��������ʵ�ֵ������㷨.
3.��չ������windows 32λ��̬���ӿ�ʵ��,����ʹ��Microsoft Visual C++���.
4.����ʱ�ڹ�ʽ�༭����д"��̬������@��������"(������)����,�������溯������дΪ"DZHFUNC@MYCMALOSE"(5)
5.��̬���ӿ����ƺͺ������ƿ����Լ�����.
6.ʹ��ʱ���Խ���̬�⿽�������ǻ�Ŀ¼��ʹ��.
*/
/////////////////////////////////////////////////////////////////////////////

//�Զ���ȫ������---->
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
	time_t datetime; //����
	float zb; //ָ��
}  ;

struct REC_DZHREALDATA
{ 
	int zbtype;//ָ�����
	char gpdm[8];
	float zb[256]; //ָ��
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
struct strfszj//���ڶ���
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
	D31V zbzbv;//���������
	D31V zbzsv;//���������
	D31V zdzbv;//��������
	D31V zdzsv;//��������	
	D31N zbzbn;//����������
	D31N zbzsn;//�����������
	D31N zdzbn;//���������
	D31N zdzsn;//����������			

} ;//L2�����ʽ�
struct L2FSZJ//���ڶ���
{
	long dC;
	strfszj fsl2zj[243];
};
struct L2FSZB//���ڷ�ʱ��Ʊ��
{
	char gpdm[9];
	long refv;
	long refD31zb;
	long refFSDDXzb;
	long dC;
	strfszj fszb[243];
};
struct INDEXDATA//��Ʊ������
{
	char dm[9];
	long start;
	long end;
};	

static const int CSHGPSL=1000;//��Ʊ����
static const int CSZGPSL=1400;//��Ʊ����
static const int CD31TS=150;//�������
struct DAYMAPPINGPOOL//��̬��Ʊ�� �������ʽ�
{
	long gpcount;
	INDEXDATA gpindex[(CSHGPSL+CSZGPSL)];
	D31ALL d31zj[(CSHGPSL+CSZGPSL)*CD31TS];
};
struct REALMAPPINGPOOL//��Ʊ�أ���ʱָ��
{
	long gpcount;
	L2FSZB fszb[2400];
};

struct daytorealzb
{
	long t;
	float data[4];
};
//�Ϻ�����ת��ʱ��
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
//��������ת��ʱ��
struct SZDAYTOREALMAPPINGPOOL
{
	long gpcount;
	DAYTOREAL daytorealzb[CSZGPSL];
};
struct	REALSTKINFO70//startAddress = 17574;//0x68A8A6;
{
	char dm[10];//����;
	char jc[8];//���;
	char space1[42];
	long rq;//����ʱ��	;//datetime;//4;//5;//60;//;float  +
	char space2[4];
	float zs;//����	le;//4;//7;//68;//;float  +
	float kp;//��	le;//4;//8;//72;//;float  +
	float zg;//���	le;//4;//9;//76;//;float  +
	float zd;//���	le;//4;//10;//80;//;float  +
	float sp;//����	le;//4;//11;//84;//;float  +
	float sl;//������	le;//4;//12;//88;//;float  +
	float je;//���	le;//4;//13;//92;//;float  +
	float xss;//������	le;//4;//14;//96;//;float  +
	char space3[84];
	float ztj;//��ͣ��	le;//4;//27;//184;//;float  +
	float dtj;//��ͣ��	le;//4;//28;//188;//;float  +
	float np;//����	le;//4;//27;//192;//;float  +
	float wp;//����	le;//4;//28;//196;//;float  +
	float mrjg1;//��һ��	le;//4;//15;//100;//;float  +
	float mrjg2;//�����	le;//4;//16;//104;//;float  +
	float mrjg3;//������	le;//4;//17;//108;//;float  +
	float mrjg4;//���ļ�	le;//4;//32;//112;//;float  +
	float mrjg5;//�����	le;//4;//33;//116;//;float  +
	float mrsl1;//��һ��	le;//4;//18;//120;//;float  +
	float mrsl2;//�����	le;//4;//19;//124;//;float  +
	float mrsl3;//������	le;//4;//20;//128;//;float  +
	float mrsl4;//������	le;//4;//34;//132;//;float  +
	float mrsl5;//������	le;//4;//35;//136;//;float  +
	float mcjg1;//��һ��	le;//4;//21;//140;//;float  +
	float mcjg2;//������	le;//4;//22;//144;//;float  +
	float mcjg3;//������	le;//4;//23;//148;//;float  +
	float mcjg4;//���ļ�	le;//4;//36;//152;//;float  +
	float mcjg5;//�����	le;//4;//37;//156;//;float  +
	float mcsl1;//��һ��	le;//4;//24;//160;//;float  +
	float mcsl2;//������	le;//4;//25;//164;//;float  +
	float mcsl3;//������	le;//4;//26;//168;//;float  +
	float mcsl4;//������	le;//4;//38;//172;//;float  +
	float mcsl5;//������	le;//4;//39;//176;//float ;
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
struct MINDATASTRUCT//1���Ӻ����ߴ�ͳ���ݽṹ
{
	long datetime;
	float data[7];
};

struct TransSign
{
	int type;//��������
	int time;//ʱ��
	int QCC;//�ύ�ź�
	int  CC;//�ֲ��ź�

};
struct AUTOTRADEINFO
{
	//�Զ����ײ���
	int tradestatus;//0-δ�ֲ֣�����1���ֲ֣��ɼӲֻ�����2�������ҵ���δ�ɽ�
	int signaldir;//0-δ�ֲ֣�������1���ֲ֣��ɼӲֻ���2�������ҵ���δ�ɽ�
	float signalprice;
	long signaltime;
	float inprice;
	long intime;
	float outprice;
	long outtime;
	int bscount;//�ֲ�����
	int bsdir;//�ֲַ���
	float bsprice;//�ֲּ۸�
	float currentprice;
	float highprice;
	float lowprice;
	float autooutprice;
	long currenttime;
	int tradeing;
	long allstatus;//1������2����λ�ò��ԣ�3ˢ�������ͺ�
	//ֹӮֹ��
	int jdzs;//����ֹ�����
	int zyd1,zys1;
	int zyd2,zys2;
	int zyd3,zys3;
	int zyd4,zys4;
	int zyd5,zys5;
	int zyzslevel;//ֹӮֹ��ȼ�
	float ymdata;
	int znjhwnd;//�Ƿ���ȷ�л���ָ����OAMVҳ��
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
	long data[64];//�����64����ʽ��62
	short data1[2];
};

struct YMMINL2SOURCE_LXB
{
	long ymtime;
	long data[62];//�����64����ʽ��62
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
	int agbkcount;//A�ɰ���Ʊ��
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
	time_t	m_time;			//ʱ��,UCT
	//float	m_fOpen;		//����
	//float	m_fHigh;		//���
	//float	m_fLow;			//���
	float	m_fClose;		//����
	float	m_fVolume;		//�ɽ���
	float	m_fAmount;		//�ɽ���
} ;

struct QHBSINFO
{
	//�Զ����׼�¼
	float inprice;
	long intime;
	char intimestr[17];
	float outprice;
	long outtime;
	char outtimestr[17];
	char bsdir[3];//�ֲַ���
	int bscount;//�ֲ�����
	char intype[9];//���ַ�ʽ
	char outtype[9];//ƽ�ַ�ʽ
	float pointcount;
	float traderesult;
};

#endif //__PublicVariable_H_INCLUDE

