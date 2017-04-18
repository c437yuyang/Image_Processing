#include "stdafx.h"
#include "DCT.h"


DCT::DCT()
{
}


DCT::~DCT()
{
}

void DCT::GenerateDCTMat(double * Mat, int nBlockSize)
{
	double a = 0.0;
	for (int i=0;i!= nBlockSize;++i)
	{
		for (int j=0;j!=nBlockSize;++j)
		{
			if (i == 0)
				a = sqrt(1.0 / nBlockSize);
			else
				a = sqrt(2.0 / nBlockSize);
			Mat[i*nBlockSize + j] = a*cos(pi*(j + 0.5)*i / nBlockSize);
		}
	}
}

void DCT::doDCTTransform(double * Mat, double *pBlockData,double *pDCTData,int nBlockSize)
{
	//Y=A*X*A'

	double *A = Mat;
	double *X = pBlockData;

	double *AX = new double[nBlockSize*nBlockSize]();
	double *A_T = new double[nBlockSize*nBlockSize]();
	//两个矩阵A*X
	double sum = 0.0;
	for (int i=0;i!=nBlockSize;++i)
	{
		for (int j=0;j!=nBlockSize;++j)
		{
			sum = 0.0;
			for (int k=0;k!=nBlockSize;++k)//A的对应行，X的对应列
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



void DCT::doIDCTTransform(double * Mat, double *pDCTData, double *pIDCTData, int nBlockSize)
{
	//X_rec = A' * Y * A

	double *A = Mat;
	double *Y = pDCTData;

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
