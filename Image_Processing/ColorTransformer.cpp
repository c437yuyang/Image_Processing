#include "stdafx.h"
#include "ColorTransformer.h"


CColorTransformer::CColorTransformer()
{
}


CColorTransformer::~CColorTransformer()
{
}

void CColorTransformer::RGB2YUV(int nR, int nG, int nB, double &Y, double &U, double &V)
{
	Y = 0.299 * nR + 0.587*nG + 0.114*nB;
	U = -0.147 * nR - 0.289*nG + 0.436*nB;
	V = 0.615 * nR - 0.515*nG - 0.1*nB;
}

void CColorTransformer::RGB2YIQ(int nR, int nG, int nB, double &Y, double &I, double &Q)
{
	Y = 0.299 * nR + 0.587*nG + 0.114*nB;
	I = 0.596 * nR - 0.275*nG - 0.321*nB;
	Q = 0.212 * nR - 0.523*nG + 0.311*nB;
}

void CColorTransformer::RGB2YCbCr(int nR, int nG, int nB, double & Y, double & Cb, double & Cr)
{
	Y = 0.299 * nR + 0.587*nG + 0.114*nB;
	Cr = -0.1687 * nR - 0.3313*nG + 0.5*nB + 128;
	Cb = 0.5 * nR - 0.4187*nG - 0.0813*nB + 128;
}

void CColorTransformer::YCbCr2RGB(double Y, double Cr, double Cb, double &R, double &G, double & B)
{
	R = Y + 1.402*(Cr - 128);

	G = Y - 0.34414 *(Cb - 128) - 0.71414* (Cr - 128);

	B = Y + 1.772 *(Cb - 128);
}

