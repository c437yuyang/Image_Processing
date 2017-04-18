#pragma once
#include "stdafx.h"
#include <vector>
using std::vector;
#include <algorithm>
using std::sort;
#include <math.h>
#include <complex>
using std::complex;
#include <random>
#include <iostream>
#include "MyImage_.h"
#include "MyImage_double.h"
//#define pi 3.1415926535898
//#define PI 3.1415926535898
//#define eps 0.000001
typedef BYTE TUInt8;

#define asm __asm

struct TARGB32      //32 bit color
{
	BYTE  b,g,r;          //a is alpha
};

class CCommon
{
public:
	CCommon(void);
	~CCommon(void);
	void FFT(complex<double> * TD,complex<double> * FD,int index);
	void RFFT(complex<double> * TD,complex<double> * FD,int index);
	double Gauss(double m,double var);
	void PicZoom0(MyImage_& Dst,const MyImage_& Src);
	void PicZoom1(MyImage_& Dst,const MyImage_& Src);
	void PicZoom2(MyImage_& Dst,const MyImage_& Src);
	void PicZoom3(MyImage_& Dst,const MyImage_& Src);
	TARGB32 Bilinear0(const MyImage_& pic,double fx,double fy);
	TARGB32 Pixels_Bound(const MyImage_& pic,long x,long y);
	void PicZoom_Bilinear0(const MyImage_& Dst,const MyImage_& Src);
	
	TARGB32 Bilinear1(const MyImage_& pic,const long x_16,const long y_16);
	void PicZoom_Bilinear1(const MyImage_& Dst,const MyImage_& Src);

	TARGB32 ThreeOrder0(const MyImage_& pic,const double fx,const double fy);
	void PicZoom_ThreeOrder0(const MyImage_& Dst,const MyImage_& Src);

	void PicRotary0(const MyImage_& Dst,const MyImage_& Src,double RotaryAngle,double ZoomX,double ZoomY,double move_x,double move_y);
	void PicRotary1(const MyImage_& Dst,const MyImage_& Src,double RotaryAngle,double ZoomX,double ZoomY,double move_x,double move_y);
	void PicRotary2(const MyImage_& Dst,const MyImage_& Src,double RotaryAngle,double ZoomX,double ZoomY,double move_x,double move_y);

// 	void RGBToHSI(const DWORD nCr,const DWORD nCg,const DWORD nCb,double *pCh,double *pCs,double *pCi);
// 	void HSIToRGB(const double nCh,const double nCs,const double nCi,DWORD *pCr,DWORD *pCg,DWORD *pCb);
// 	void HSIToRGB(const double nCh,const double nCs,const double nCi,DWORD *pCr,DWORD *pCg,DWORD *pCb,DWORD r,DWORD g,DWORD b);
	void ChangedColourFromRGBStandardtoHSL(int nR,int nG,int nB,int *pH,int *pS,int *pI);
	void ChangedColourFromHSLStandardtoRGB(int nH,int nS,int nI,int *pR,int *pG,int *pB);


	template <class T>
	void VecNormalized(vector<T> &vecT,T normMin,T normMax);
	vector<int> CCommon::get_coords(double angle);
	void CCommon::Connect(MyImage_ &img,int y,int x,int nLowThresh);
	void CCommon::Connect(CMyImage_double &img,int y,int x,int nLowThresh);


	int TextSegment(const CString &strInput,const CString &strSeg,vector<CString> &vecOutput);
	double* CCommon::getGussianKernalData(int n, double sigma);
	vector<char> DecimalDec2Bin(double num, int digit=3);
	void sortindex(double a[], int p[], int n); //排序数组a并在数组b中得到排序后的数在原数组的下标
};
