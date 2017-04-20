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
	void GenerateDCTMat(double * DCTMat, int nBlockSize); //需要传入已分配好内存的数组(这样更安全一点)
	void doDCTTransform(const double * DCTMat, const double *nBlockData, double *nIDCTData, int nBlockSize); //其实就是执行矩阵相乘
	void doIDCTTransform(const double * DCTMat, const double *pDCTData, double *pIDCTData, int nBlockSize);
	void doQuantilization(const double *SampleMat, const double * pBlockData, double *pSampledData, int nBlockSize);
	void doInverseQuantilization(const double *SampleMat, const double * pSampled, double *pRecoverdData, int nBlockSize);

};

