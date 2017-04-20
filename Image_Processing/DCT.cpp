#include "stdafx.h"
#include "DCT.h"


DCT::DCT()
{
	pQtMatY = new double[64]{ //Y分量量化
		16,11,10,16,24,40,51,61 ,
		12,12,14,19,26,58,60,55 ,
		14,13,16,24,40,57,69,56 ,
		14,17,22,29,51,87,80,62 ,
		18,22,37,56,68,109,103,77 ,
		24,35,55,64,81,104,113,92 ,
		49,64,78,87,103,121,120,101 ,
		72,92,95,98,112,100,103,99
	};

	pQtMatUV = new double[64]{
		17,18,24,47,99,99,99,99
		,18,21,26,66,99,99,99,99
		,24,26,56,99,99,99,99,99
		,47,66,99,99,99,99,99,99
		,99,99,99,99,99,99,99,99
		,99,99,99,99,99,99,99,99
		,99,99,99,99,99,99,99,99
		,99,99,99,99,99,99,99,99
	};
}


DCT::~DCT()
{
	delete []pQtMatY;
	pQtMatY = NULL;
	delete[]pQtMatUV; 
	pQtMatUV = NULL;
}

void DCT::GenerateDCTMat(double * Mat, int nBlockSize)
{
	double a = 0.0;
	for (int i = 0; i != nBlockSize; ++i)
	{
		for (int j = 0; j != nBlockSize; ++j)
		{
			if (i == 0)
				a = sqrt(1.0 / nBlockSize);
			else
				a = sqrt(2.0 / nBlockSize);
			Mat[i*nBlockSize + j] = a*cos(pi*(j + 0.5)*i / nBlockSize);
		}
	}
}

void DCT::doDCTTransform(const double * Mat, const double *pBlockData, double *pDCTData, int nBlockSize)
{
	//Y=A*X*A'

	const double *A = Mat;
	const double *X = pBlockData;

	double *AX = new double[nBlockSize*nBlockSize]();
	double *A_T = new double[nBlockSize*nBlockSize]();
	//两个矩阵A*X
	double sum = 0.0;
	for (int i = 0; i != nBlockSize; ++i)
	{
		for (int j = 0; j != nBlockSize; ++j)
		{
			sum = 0.0;
			for (int k = 0; k != nBlockSize; ++k)//A的对应行，X的对应列
			{
				sum += A[i*nBlockSize + k] * X[k*nBlockSize + j];
			}
			AX[i*nBlockSize + j] = sum;
		}
	}

	//求A'
	for (int i = 0; i != nBlockSize; ++i)
	{
		for (int j = 0; j != nBlockSize; ++j)
		{
			A_T[i*nBlockSize + j] = A[j*nBlockSize + i];
		}
	}

	//AX*A'
	for (int i = 0; i != nBlockSize; ++i)
	{
		for (int j = 0; j != nBlockSize; ++j)
		{
			sum = 0.0;
			for (int k = 0; k != nBlockSize; ++k)//A的对应行，X的对应列
			{
				sum += AX[i*nBlockSize + k] * A_T[k*nBlockSize + j];
			}
			pDCTData[i*nBlockSize + j] = sum;
		}
	}

	delete[]AX;
	delete[]A_T;
	AX = NULL;
	A_T = NULL;
}



void DCT::doIDCTTransform(const double * Mat, const double *pDCTData, double *pIDCTData, int nBlockSize)
{
	//X_rec = A' * Y * A

	const double *A = Mat;
	const double *Y = pDCTData;

	double *ATY = new double[nBlockSize*nBlockSize]();
	double *A_T = new double[nBlockSize*nBlockSize]();

	//求A'
	for (int i = 0; i != nBlockSize; ++i)
	{
		for (int j = 0; j != nBlockSize; ++j)
		{
			A_T[i*nBlockSize + j] = A[j*nBlockSize + i];
		}
	}

	//两个矩阵A'*Y
	double sum = 0.0;
	for (int i = 0; i != nBlockSize; ++i)
	{
		for (int j = 0; j != nBlockSize; ++j)
		{
			sum = 0.0;
			for (int k = 0; k != nBlockSize; ++k)//A的对应行，X的对应列
			{
				sum += A_T[i*nBlockSize + k] * Y[k*nBlockSize + j];
			}
			ATY[i*nBlockSize + j] = sum;
		}
	}



	//A'Y*A
	for (int i = 0; i != nBlockSize; ++i)
	{
		for (int j = 0; j != nBlockSize; ++j)
		{
			sum = 0.0;
			for (int k = 0; k != nBlockSize; ++k)//A的对应行，X的对应列
			{
				sum += ATY[i*nBlockSize + k] * A[k*nBlockSize + j];
			}
			pIDCTData[i*nBlockSize + j] = sum;
		}
	}
	delete[]ATY;
	delete[]A_T;
	ATY = NULL;
	A_T = NULL;
}

//支持inplace操作的
void DCT::doQuantilization(const double *QtMat, const double * pBlockData, double *pSampledData, int nBlockSize)
{
	for (int i = 0; i != nBlockSize*nBlockSize; ++i)
	{
		pSampledData[i] = round(pBlockData[i] / QtMat[i]); // 直接除
	}

}

void DCT::doInverseQuantilization(const double *SampleMat, const double * pSampled, double *pRecoverdData, int nBlockSize)
{
	for (int i = 0; i != nBlockSize*nBlockSize; ++i)
	{
		pRecoverdData[i] = round(pSampled[i] * SampleMat[i]); // 直接除
	}
}
