#pragma once
#include <math.h>
//#define pi 3.14159265358979
class DCT
{
public:
	DCT();
	~DCT();

	void GenerateDCTMat(double * Mat,int nBlockSize); //需要传入已分配好内存的数组(这样更安全一点)
	void doDCTTransform(double * Mat, double *nBlockData, double *nIDCTData,int nBlockSize); //其实就是执行矩阵相乘
	void doIDCTTransform(double * Mat, double *pDCTData, double *pIDCTData, int nBlockSize);
};

