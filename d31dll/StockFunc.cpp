// StockFunc.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"

#include "stdio.h"
#include <fstream>
#include "StockFunc.h" 
#include <vector>
#include <iostream>

#include "IniFile.h"
#include "ExtendData.h"
using namespace std;

DllCallInfo_t CALLINFO;
D31MemCache *MYMC;

void log_printf(int iLevel, char *fmt, ...);

//DLL��ʽ������ģʽ��ϵͳϵͳ���ø�DLLʱ����֪��ʽϵͳ��DLL��ʽ�����ں���ģʽ��
//����ֵ:	  0��DLL��������ģʽ 1��DLL����������ģʽ
__declspec(dllexport) int WINAPI RUNMODE()
{
	//��DLL��������ģʽ�����������K��ģʽ���˺�������1����ʱÿִ��һ��K�߶��ǵ��ñ��ӿ�һ�Ρ�
	log_printf(1, "runmode1\n");

	if (CALLINFO.bZkFlag == true) return 1;

	return 0;
}

//�������̼۵ľ���,һ����������,��ʾ��������
//���÷���:
//	"STOCKFUNC@MYMACLOSE"(5)
__declspec(dllexport) int WINAPI MYMACLOSE(CALCINFO* pData)
{
	if (pData->m_pfParam1 &&				//����1��Ч
		pData->m_nParam1Start<0 &&			//����1Ϊ����
		pData->m_pfParam2 == NULL) {			//����һ������

		float fParam = *pData->m_pfParam1;
		int nPeriod = (int)fParam;			//����1

		if (nPeriod>0) {
			int i, j;
			float fTotal;

			for (i = nPeriod - 1; i < pData->m_nNumData; i++) {//����nPeriod���ڵľ���,���ݴ�nPeriod-1��ʼ��Ч

				fTotal = 0.0f;
				for (j = 0; j < nPeriod; j++)				//�ۼ�
					fTotal += pData->m_pData[i - j].m_fClose;
				pData->m_pResultBuf[i] = fTotal / nPeriod;	//ƽ��
			}
			return nPeriod - 1;
		}
	}

	return -1;
}

//�������,2������,����1Ϊ������ߵ�����,����2��ʾ��������
//���÷���:
//	"STOCKFUNC@MYMAVAR"(CLOSE-OPEN,5)

__declspec(dllexport) int WINAPI MYMAVAR(CALCINFO* pData)
{
	if (pData->m_pfParam1 && pData->m_pfParam2 && 	//����1,2��Ч
		pData->m_nParam1Start >= 0 &&					//����1Ϊ������
		pData->m_pfParam3 == NULL) {					//��2������

		const float*  pValue = pData->m_pfParam1;	//����1
		int nFirst = pData->m_nParam1Start;			//��Чֵ��ʼλ
		float fParam = *pData->m_pfParam2;			//����2
		int nPeriod = (int)fParam;

		if (nFirst >= 0 && nPeriod > 0) {

			int i, j;
			float fTotal;

			for (i = nFirst + nPeriod - 1; i < pData->m_nNumData; i++) {

				fTotal = 0.0f;
				for (j = 0; j < nPeriod; j++)			//�ۼ�
					fTotal += pValue[i - j];
				pData->m_pResultBuf[i] = fTotal / nPeriod;//ƽ��
			}
			return nFirst + nPeriod - 1;
		}
	}
	return -1;

	/*������K��ģʽ���Ĵ���ʽ��ʾ������
	if(pData->m_pfParam1 && pData->m_pfParam2){

	//������K��ģʽ�����ڴ������ݶ�����ֵ�������Ҫһ�������������洫�ݹ����Ĳ�������������.
	//����ֻ�Ǽ���ʾ����÷�������û��ڹ�ʽ��ε����˽ӿڣ��������ظ�ʹ�øñ������¼������
	//���Ҫ�����������������ȫ�־�̬�������ڱ���������ݡ�
	static std::vector<double> arMaData;

	//��һ�����ڳ�ʼ������
	if(pData->m_dwBarpos == 0)	arMaData.clear();

	double dbData = *pData->m_pfParam1;

	//��ֹʹ�ý�ˢ���K�ߵķ���ˢ������
	if(arMaData.size() >= pData->m_nNumData)
	arMaData[arMaData.size()-1] = dbData; //�����������һ������
	else	arMaData.push_back(dbData);

	int cycle = (DWORD) *pData->m_pfParam2;

	if(pData->m_dwBarpos < cycle-1) return -1;

	//���ۼ�
	float fAdd = 0;
	for(DWORD i = pData->m_dwBarpos-(cycle-1); i <= pData->m_dwBarpos; i++)
	fAdd += arMaData[i];

	*pData->m_pResultBuf = fAdd / dwCyc;
	}

	return 1;
	*/
}

//���������еľ�ֵ,5������,����1-4Ϊ����������,����5���ھ���˵����ֵ�������÷���ʵ���ڴ���������ò���
/*
���÷���:
MA1:=MA(CLOSE,3);
MA2:=MA(CLOSE,6);
MA3:=MA(CLOSE,12);
MA4:=MA(CLOSE,24);
MYBBI: "STOCKFUNC@MYBBI"(MA1, MA2, MA3, MA4, 4);
*/
__declspec(dllexport) int WINAPI MYBBI(CALCINFO* pData)
{
	if (pData->m_pCalcParam[0].m_nParamStart >= 0 &&
		pData->m_pCalcParam[1].m_nParamStart >= 0 &&
		pData->m_pCalcParam[2].m_nParamStart >= 0 &&
		pData->m_pCalcParam[3].m_nParamStart >= 0) {		//4�����ж�����Ч��ֵ

									//���㷵�ص����еĵ�һ����Чֵλ��
									//��֪���ص����еĵ�һ����Чֵλ�����4������һ��
		int nFirst = pData->m_pCalcParam[3].m_nParamStart;

		//����֪����
		/*
		int nFirst = pData->m_pCalcParam[0].m_nParamStart;
		if ( nFirst < pData->m_pCalcParam[1].m_nParamStart )
		nFirst = pData->m_pCalcParam[1].m_nParamStart;
		if ( nFirst < pData->m_pCalcParam[2].m_nParamStart )
		nFirst = pData->m_pCalcParam[2].m_nParamStart;
		if ( nFirst < pData->m_pCalcParam[3].m_nParamStart )
		nFirst = pData->m_pCalcParam[3].m_nParamStart;
		*/

		const float* pValue1 = pData->m_pCalcParam[0].m_pfParam;
		const float* pValue2 = pData->m_pCalcParam[1].m_pfParam;
		const float* pValue3 = pData->m_pCalcParam[2].m_pfParam;
		const float* pValue4 = pData->m_pCalcParam[3].m_pfParam;

		//ʵ���ϸ�������֪nNum=4���ڴ�����˵����ֵ�������÷�
		int nNum = (int)*(pData->m_pCalcParam[4].m_pfParam);

		for (int i = nFirst; i < pData->m_nNumData; i++) {
			pData->m_pResultBuf[i] =
				(pValue1[i] + pValue2[i] + pValue3[i] + pValue4[i]) / nNum;
		}
		return nFirst;
	}
	return -1;
}

std::string times[240] = {
	"09:31:00","09:32:00","09:33:00","09:34:00","09:35:00","09:36:00","09:37:00","09:38:00","09:39:00","09:40:00",
	"09:41:00","09:42:00","09:43:00","09:44:00","09:45:00","09:46:00","09:47:00","09:48:00","09:49:00","09:50:00",
	"09:51:00","09:52:00","09:53:00","09:54:00","09:55:00","09:56:00","09:57:00","09:58:00","09:59:00","10:00:00",
	"10:01:00","10:02:00","10:03:00","10:04:00","10:05:00","10:06:00","10:07:00","10:08:00","10:09:00","10:10:00",
	"10:11:00","10:12:00","10:13:00","10:14:00","10:15:00","10:16:00","10:17:00","10:18:00","10:19:00","10:20:00",
	"10:21:00","10:22:00","10:23:00","10:24:00","10:25:00","10:26:00","10:27:00","10:28:00","10:29:00","10:30:00",
	"10:31:00","10:32:00","10:33:00","10:34:00","10:35:00","10:36:00","10:37:00","10:38:00","10:39:00","10:40:00",
	"10:41:00","10:42:00","10:43:00","10:44:00","10:45:00","10:46:00","10:47:00","10:48:00","10:49:00","10:50:00",
	"10:51:00","10:52:00","10:53:00","10:54:00","10:55:00","10:56:00","10:57:00","10:58:00","10:59:00","11:00:00",
	"11:01:00","11:02:00","11:03:00","11:04:00","11:05:00","11:06:00","11:07:00","11:08:00","11:09:00","11:10:00",
	"11:11:00","11:12:00","11:13:00","11:14:00","11:15:00","11:16:00","11:17:00","11:18:00","11:19:00","11:20:00",
	"11:21:00","11:22:00","11:23:00","11:24:00","11:25:00","11:26:00","11:27:00","11:28:00","11:29:00","11:30:00",
	"13:01:00","13:02:00","13:03:00","13:04:00","13:05:00","13:06:00","13:07:00","13:08:00","13:09:00","13:10:00",
	"13:11:00","13:12:00","13:13:00","13:14:00","13:15:00","13:16:00","13:17:00","13:18:00","13:19:00","13:20:00",
	"13:21:00","13:22:00","13:23:00","13:24:00","13:25:00","13:26:00","13:27:00","13:28:00","13:29:00","13:30:00",
	"13:31:00","13:32:00","13:33:00","13:34:00","13:35:00","13:36:00","13:37:00","13:38:00","13:39:00","13:40:00",
	"13:41:00","13:42:00","13:43:00","13:44:00","13:45:00","13:46:00","13:47:00","13:48:00","13:49:00","13:50:00",
	"13:51:00","13:52:00","13:53:00","13:54:00","13:55:00","13:56:00","13:57:00","13:58:00","13:59:00","14:00:00",
	"14:01:00","14:02:00","14:03:00","14:04:00","14:05:00","14:06:00","14:07:00","14:08:00","14:09:00","14:10:00",
	"14:11:00","14:12:00","14:13:00","14:14:00","14:15:00","14:16:00","14:17:00","14:18:00","14:19:00","14:20:00",
	"14:21:00","14:22:00","14:23:00","14:24:00","14:25:00","14:26:00","14:27:00","14:28:00","14:29:00","14:30:00",
	"14:31:00","14:32:00","14:33:00","14:34:00","14:35:00","14:36:00","14:37:00","14:38:00","14:39:00","14:40:00",
	"14:41:00","14:42:00","14:43:00","14:44:00","14:45:00","14:46:00","14:47:00","14:48:00","14:49:00","14:50:00",
	"14:51:00","14:52:00","14:53:00","14:54:00","14:55:00","14:56:00","14:57:00","14:58:00","14:59:00","15:00:00" };

// ����ר��
__declspec(dllexport)int WINAPI TST(CALCINFO* pData)
{
	//time_t��long���ͣ���ȷ���룬�ǵ�ǰʱ���1970��1��1�����ʱ��Ĳ�
	const time_t t = time(NULL);

	/*����ʱ�䣺���ڣ�ʱ�� �����գ����ڣ�ʱ����*/
	struct tm* current_time = localtime(&t);
	ofstream myfile;

	//ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
	myfile.open("c:\\jztlog.txt", ios::app);

	if (myfile.fail()) return -1;

	myfile << "time is\t" << 1900 + current_time->tm_year <<
		"-" << 1 + current_time->tm_mon << "-" << current_time->tm_mday;

	myfile << " " << current_time->tm_hour << ":" << current_time->tm_min <<
		":" << current_time->tm_sec << endl;

	//myfile << "m_nNumData=" << pData->m_nNumData << "\tm_nNumParam=" << pData->m_nNumParam << endl;
	int nFirst = -1;

	if (pData->m_nNumParam > 0) {
		// �����׼�ӿڲ������� �� ��չ�ӿڲ�������
		//myfile << m_strStkName ;	
		myfile << "\tm_nNumData=" << pData->m_nNumData <<
			"\tm_nNumParam=" << pData->m_nNumParam << endl;

		for (int i = 0; i < pData->m_nNumParam; ++i) {

			int parmIndex = pData->m_pCalcParam[i].m_nParamStart;
			if (parmIndex >= 0) {

				nFirst = pData->m_pCalcParam[i].m_nParamStart;
				myfile << "p" << i << "=" << endl;

				for (int j = parmIndex; j < pData->m_nNumData; j++) {

					int v = (int)pData->m_pCalcParam[i].m_pfParam[j];
					myfile << v << endl;
				}
			}
		}
		myfile << "nFirst=" << nFirst << endl << endl;
	}
	else
		myfile << "can't get parms" << endl << endl;


	myfile.close();

	/*
	if ( pData->m_pCalcParam[0].m_nParamStart >= 0 &&
	pData->m_pCalcParam[1].m_nParamStart >= 0 &&
	pData->m_pCalcParam[2].m_nParamStart >= 0 &&
	pData->m_pCalcParam[3].m_nParamStart >= 0 )			//4�����ж�����Ч��ֵ
	{
	//���㷵�ص����еĵ�һ����Чֵλ��
	int nFirst = pData->m_pCalcParam[3].m_nParamStart;		//��֪���ص����еĵ�һ����Чֵλ�����4������һ��
	//����֪����
	//int nFirst = pData->m_pCalcParam[0].m_nParamStart;
	//if ( nFirst < pData->m_pCalcParam[1].m_nParamStart )
	//	nFirst = pData->m_pCalcParam[1].m_nParamStart;
	//if ( nFirst < pData->m_pCalcParam[2].m_nParamStart )
	//	nFirst = pData->m_pCalcParam[2].m_nParamStart;
	//if ( nFirst < pData->m_pCalcParam[3].m_nParamStart )
	//	nFirst = pData->m_pCalcParam[3].m_nParamStart;


	const float* pValue1 = pData->m_pCalcParam[0].m_pfParam;
	const float* pValue2 = pData->m_pCalcParam[1].m_pfParam;
	const float* pValue3 = pData->m_pCalcParam[2].m_pfParam;
	const float* pValue4 = pData->m_pCalcParam[3].m_pfParam;
	int nNum = (int)*(pData->m_pCalcParam[4].m_pfParam);		//ʵ���ϸ�������֪nNum=4���ڴ�����˵����ֵ�������÷�
	for( int i = nFirst; i < pData->m_nNumData; i++ )
	{
	pData->m_pResultBuf[i] =
	(pValue1[i] + pValue2[i] + pValue3[i] + pValue4[i])/nNum;
	}
	return nFirst;
	}*/
	return -1;
}
