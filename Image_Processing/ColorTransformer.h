#pragma once
class CColorTransformer
{
public:
	CColorTransformer();
	~CColorTransformer();
	
	void RGB2YUV(int nR, int nG, int nB, double &Y, double &U, double &V);
	void RGB2YIQ(int nR, int nG, int nB, double &Y, double &I, double &Q);
	void RGB2YCbCr(int nR, int nG, int nB, double &Y, double &Cb, double &Cr);

	void YCbCr2RGB(double Y, double Cr, double Cb, double &R, double &G, double & B);
};

