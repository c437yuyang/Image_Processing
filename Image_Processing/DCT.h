#pragma once
#include <math.h>
//#define pi 3.14159265358979
class DCT
{
public:
	DCT();
	~DCT();

	void GenerateDCTMat(double * Mat,int nBlockSize); //��Ҫ�����ѷ�����ڴ������(��������ȫһ��)
	void doDCTTransform(double * Mat, double *nBlockData, double *nIDCTData,int nBlockSize); //��ʵ����ִ�о������
	void doIDCTTransform(double * Mat, double *pDCTData, double *pIDCTData, int nBlockSize);
};

