#pragma once
#include <math.h>
//#define pi 3.14159265358979
class DCT
{
public:
	DCT();
	~DCT();
	const double *pQtMatY;
	const double *pQtMatUV;
	void GenerateDCTMat(double * DCTMat, int nBlockSize); //��Ҫ�����ѷ�����ڴ������(��������ȫһ��)
	void doDCTTransform(const double * DCTMat, const double *nBlockData, double *nIDCTData, int nBlockSize); //��ʵ����ִ�о������
	void doIDCTTransform(const double * DCTMat, const double *pDCTData, double *pIDCTData, int nBlockSize);
	void doQuantilization(const double *SampleMat, const double * pBlockData, double *pSampledData, int nBlockSize);
	void doInverseQuantilization(const double *SampleMat, const double * pSampled, double *pRecoverdData, int nBlockSize);

};

