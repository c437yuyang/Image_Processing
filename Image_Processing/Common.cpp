#include "StdAfx.h"
#include "Common.h"


CCommon::CCommon(void)
{
}


CCommon::~CCommon(void)
{
}


void CCommon::FFT(complex<double> * TD, complex<double> * FD, int index)
{
	int dotcount = 0;//傅立叶变换点数
	int bfsize = 0, p = 0;//中间变量
	double angle;//角度
	complex<double> *W, *X1, *X2, *X;//定义四个复数

	dotcount = 1 << index;//傅立叶变换总点数。 index相当于log2N

	W = new complex<double>[sizeof(complex<double>)*dotcount / 2];//分配内存空间
	X1 = new complex<double>[sizeof(complex<double>)*dotcount];
	X2 = new complex<double>[sizeof(complex<double>)*dotcount];

	for (int i = 0; i < dotcount / 2; i++)//计算加权系数Wn
	{
		angle = -i * 2 * pi / dotcount;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	memcpy(X1, TD, sizeof(complex<double>)*dotcount);////////////////////////////////要标准

	for (int k = 0; k < index; k++)//采用蝶形运算进行快速傅立叶变换，共index级
	{
		for (int j = 0; j < 1 << k; j++)//蝶形节排序，从0开始排序
		{
			bfsize = 1 << (index - k);//每个蝶形节的点数
			for (int i = 0; i < bfsize / 2; i++)//排序后蝶形节的一半，运用周期性
			{
				p = j*bfsize;//j个蝶形节的总点数。
				X2[p + i] = X1[p + i] + X1[p + i + bfsize / 2];
				X2[p + i + bfsize / 2] = (X1[p + i] - X1[p + i + bfsize / 2])*W[i*(1 << k)];//按频域抽选的基-2 FFT算法，加权系数仅挑选一部分
			}
		}
		X = X1;
		X1 = X2;
		X2 = X; //X1,X2交换值位置
	}

	for (int j = 0; j < dotcount; j++)//倒序  变  顺序
	{
		p = 0;
		for (int i = 0; i < index; i++)
		{
			if (j&(1 << i))
			{
				p += 1 << (index - i - 1);//////////////p=p+1<<(index-i-1)?????
			}
		}
		FD[j] = X1[p];
	}

	delete W;
	delete X1;
	delete X2;

}

void CCommon::RFFT(complex<double> * TD, complex<double> * FD, int index)
{
	int dotcount = 0;//傅立叶变换点数
	int bfsize = 0, p = 0;//中间变量
	double angle;//角度
	complex<double> *W, *X1, *X2, *X;//定义四个复数

	dotcount = 1 << index;//傅立叶变换总点数。 index相当于log2N

	W = new complex<double>[sizeof(complex<double>)*dotcount / 2];//分配内存空间
	X1 = new complex<double>[sizeof(complex<double>)*dotcount];
	X2 = new complex<double>[sizeof(complex<double>)*dotcount];

	for (int i = 0; i < dotcount / 2; i++)//计算加权系数Wn
	{
		angle = i * 2 * pi / dotcount;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	memcpy(X1, TD, sizeof(complex<double>)*dotcount);////////////////////////////////要标准

	for (int k = 0; k < index; k++)//采用蝶形运算进行快速傅立叶变换，共index级
	{
		for (int j = 0; j < 1 << k; j++)//蝶形节排序，从0开始排序
		{
			bfsize = 1 << (index - k);//每个蝶形节的点数
			for (int i = 0; i < bfsize / 2; i++)//排序后蝶形节的一半，运用周期性
			{
				p = j*bfsize;//j个蝶形节的总点数。
				X2[p + i] = X1[p + i] + X1[p + i + bfsize / 2];
				X2[p + i + bfsize / 2] = (X1[p + i] - X1[p + i + bfsize / 2])*W[i*(1 << k)];//按频域抽选的基-2 FFT算法，加权系数仅挑选一部分
			}
		}
		X = X1;
		X1 = X2;
		X2 = X;
	}

	for (int j = 0; j < dotcount; j++)//倒序  变  顺序
	{
		p = 0;
		for (int i = 0; i < index; i++)
		{
			if (j&(1 << i))
			{
				p += 1 << (index - i - 1);//////////////p=p+1<<(index-i-1)?????
			}
		}

		FD[j]._Val[0] = X1[p]._Val[0] / dotcount;
		FD[j]._Val[1] = X1[p]._Val[1] / dotcount;
	}
	delete W;
	delete X1;
	delete X2;

}

double CCommon::Gauss(double m, double var)
{
	std::default_random_engine random((unsigned long)time(NULL));
	std::normal_distribution<double> dis(m, var);
	return dis(random);
}


void CCommon::PicZoom0(MyImage_& Dst, const MyImage_& Src)
{
	if ((0 == Dst.GetWidth()) || (0 == Dst.GetHeight())
		|| (0 == Src.GetWidth()) || (0 == Src.GetHeight())) return;
	for (long x = 0; x < Dst.GetWidth(); ++x)
	{
		for (long y = 0; y < Dst.GetHeight(); ++y)
		{
			long srcx = (x*Src.GetWidth() / Dst.GetWidth());
			long srcy = (y*Src.GetHeight() / Dst.GetHeight());
			//std::cout << srcx << "," << srcy << std::endl;
			for (int ch = 0; ch != 3; ++ch)
				Dst.m_pBits[ch][y][x] = Src.m_pBits[ch][srcy][srcx];
		}
	}
}

void CCommon::PicZoom1(MyImage_& Dst, const MyImage_& Src)
{
	if ((0 == Dst.GetWidth()) || (0 == Dst.GetHeight())
		|| (0 == Src.GetWidth()) || (0 == Src.GetHeight())) return;
	for (long y = 0; y < Dst.GetHeight(); ++y)
	{
		for (long x = 0; x < Dst.GetWidth(); ++x)
		{
			long srcx = (x*Src.GetWidth() / Dst.GetWidth());
			long srcy = (y*Src.GetHeight() / Dst.GetHeight());
			for (int ch = 0; ch != 3; ++ch)
				Dst.m_pBits[ch][y][x] = Src.m_pBits[ch][srcy][srcx];
		}
	}
}


void CCommon::PicZoom2(MyImage_& Dst, const MyImage_& Src)
{
	if ((0 == Dst.GetWidth()) || (0 == Dst.GetHeight())
		|| (0 == Src.GetWidth()) || (0 == Src.GetHeight())) return;

	unsigned long xrIntFloat_16 = (Src.GetWidth() << 16) / Dst.GetWidth() + 1; //16.16格式定点数
	unsigned long yrIntFloat_16 = (Src.GetHeight() << 16) / Dst.GetHeight() + 1; //16.16格式定点数

	for (long y = 0; y < Dst.GetHeight(); ++y)
	{
		for (long x = 0; x < Dst.GetWidth(); ++x)
		{
			unsigned long srcx = (x*xrIntFloat_16) >> 16;
			unsigned long srcy = (y*yrIntFloat_16) >> 16;
			for (int ch = 0; ch != 3; ++ch)
				Dst.m_pBits[ch][y][x] = Src.m_pBits[ch][srcy][srcx];
		}
	}
}


void CCommon::PicZoom3(MyImage_& Dst, const MyImage_& Src)
{
	//if (  (0==Dst.GetWidth())||(0==Dst.GetHeight())
	//	||(0==Src.GetWidth())||(0==Src.GetHeight())) return;

	//unsigned long xrIntFloat_16=(Src.GetWidth()<<16)/Dst.GetWidth()+1; //16.16格式定点数
	//unsigned long yrIntFloat_16=(Src.GetHeight()<<16)/Dst.GetHeight()+1; //16.16格式定点数
	//unsigned long dst_width=Dst.GetWidth();
	//int nrow=Dst.m_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	//BYTE *pDstLine=(BYTE *)Dst.m_CImage.GetBits();//获得m_CImage第一行的像素地址
	////TARGB32* pDstLine=Dst.pdata;
	//unsigned long srcy_16=0;
	//for (long y=0;y<Dst.GetHeight();++y)
	//{
	//	BYTE *pSrcLine = 
	//	TARGB32* pSrcLine=((TARGB32*)((TUInt8*)Src.pdata+Src.byte_width*(srcy_16>>16)));
	//	for (long x=0;x<Dst.GetWidth();++x)
	//	{
	//		unsigned long srcx=(x*xrIntFloat_16)>>16;
	//		unsigned long srcy=(y*yrIntFloat_16)>>16;
	//		for (int ch=0;ch!=3;++ch)
	//			Dst.m_pBits[ch][y][x] = Src.m_pBits[ch][srcy][srcx];
	//	}
	//}
}


TARGB32 CCommon::Pixels_Bound(const MyImage_& pic, long x, long y)
{
	//assert((pic.width>0)&&(pic.height>0));
	bool IsInPic = true;
	if (x < 0)
	{
		x = 0; IsInPic = false;
	}
	else if (x >= pic.GetWidth())
	{
		x = pic.GetWidth() - 1;
		IsInPic = false;
	}
	if (y < 0)
	{
		y = 0; IsInPic = false;
	}
	else if (y >= pic.GetHeight())
	{
		y = pic.GetHeight() - 1;
		IsInPic = false;
	}

	TARGB32 result;
	result.b = pic.m_pBits[0][y][x];
	result.g = pic.m_pBits[1][y][x];
	result.r = pic.m_pBits[2][y][x];
	/*	if (!IsInPic) result.a=0;*/
	return result;
}

TARGB32 CCommon::Bilinear0(const MyImage_& pic, double fx, double fy)
{
	long x = (long)fx; if (x > fx) --x; //x=floor(fx);    
	long y = (long)fy; if (y > fy) --y; //y=floor(fy);

	TARGB32 Color0 = Pixels_Bound(pic, x, y);
	TARGB32 Color2 = Pixels_Bound(pic, x + 1, y);
	TARGB32 Color1 = Pixels_Bound(pic, x, y + 1);
	TARGB32 Color3 = Pixels_Bound(pic, x + 1, y + 1);

	double u = fx - x;
	double v = fy - y;
	double pm3 = u*v;
	double pm2 = u*(1 - v);
	double pm1 = v*(1 - u);
	double pm0 = (1 - u)*(1 - v);

	TARGB32 result;
	result.r = (BYTE)(pm0*Color0.r + pm1*Color1.r + pm2*Color2.r + pm3*Color3.r);
	result.g = (BYTE)(pm0*Color0.g + pm1*Color1.g + pm2*Color2.g + pm3*Color3.g);
	result.b = (BYTE)(pm0*Color0.b + pm1*Color1.b + pm2*Color2.b + pm3*Color3.b);
	return result;
}


void CCommon::PicZoom_Bilinear0(const MyImage_& Dst, const MyImage_& Src)
{

	if ((0 == Dst.GetWidth()) || (0 == Dst.GetHeight())
		|| (0 == Src.GetWidth()) || (0 == Src.GetHeight())) return;

	for (long y = 0; y < Dst.GetHeight(); ++y)
	{
		double srcy = (y + 0.4999999)*Src.GetHeight() / Dst.GetHeight() - 0.5;
		for (long x = 0; x < Dst.GetWidth(); ++x)
		{
			double srcx = (x + 0.4999999)*Src.GetWidth() / Dst.GetWidth() - 0.5;
			Dst.m_pBits[0][y][x] = (Bilinear0(Src, srcx, srcy)).b;
			Dst.m_pBits[1][y][x] = (Bilinear0(Src, srcx, srcy)).g;
			Dst.m_pBits[2][y][x] = (Bilinear0(Src, srcx, srcy)).r;
		}
	}
}



TARGB32 CCommon::Bilinear1(const MyImage_& pic, const long x_16, const long y_16)
{
	long x = x_16 >> 16;
	long y = y_16 >> 16;
	TARGB32 Color0 = Pixels_Bound(pic, x, y);
	TARGB32 Color2 = Pixels_Bound(pic, x + 1, y);
	TARGB32 Color1 = Pixels_Bound(pic, x, y + 1);
	TARGB32 Color3 = Pixels_Bound(pic, x + 1, y + 1);

	unsigned long u_8 = (x_16 & 0xFFFF) >> 8;
	unsigned long v_8 = (y_16 & 0xFFFF) >> 8;
	unsigned long pm3_16 = (u_8*v_8);
	unsigned long pm2_16 = (u_8*(unsigned long)(256 - v_8));
	unsigned long pm1_16 = (v_8*(unsigned long)(256 - u_8));
	unsigned long pm0_16 = ((256 - u_8)*(256 - v_8));

	TARGB32 result;
	result.r = (BYTE)((pm0_16*Color0.r + pm1_16*Color1.r + pm2_16*Color2.r + pm3_16*Color3.r) >> 16);
	result.g = (BYTE)((pm0_16*Color0.g + pm1_16*Color1.g + pm2_16*Color2.g + pm3_16*Color3.g) >> 16);
	result.b = (BYTE)((pm0_16*Color0.b + pm1_16*Color1.b + pm2_16*Color2.b + pm3_16*Color3.b) >> 16);
	return result;
}

void CCommon::PicZoom_Bilinear1(const MyImage_& Dst, const MyImage_& Src)
{
	if ((0 == Dst.GetWidth()) || (0 == Dst.GetHeight())
		|| (0 == Src.GetWidth()) || (0 == Src.GetHeight())) return;

	long xrIntFloat_16 = ((Src.GetWidth()) << 16) / Dst.GetWidth() + 1;
	long yrIntFloat_16 = ((Src.GetHeight()) << 16) / Dst.GetHeight() + 1;
	const long csDErrorX = -(1 << 15) + (xrIntFloat_16 >> 1);
	const long csDErrorY = -(1 << 15) + (yrIntFloat_16 >> 1);

	unsigned long dst_width = Dst.GetWidth();

	long srcy_16 = csDErrorY;
	long y;
	for (y = 0; y < Dst.GetHeight(); ++y)
	{
		long srcx_16 = csDErrorX;
		for (unsigned long x = 0; x < dst_width; ++x)
		{
			Dst.m_pBits[0][y][x] = (Bilinear1(Src, srcx_16, srcy_16)).b;
			Dst.m_pBits[1][y][x] = (Bilinear1(Src, srcx_16, srcy_16)).g;
			Dst.m_pBits[2][y][x] = (Bilinear1(Src, srcx_16, srcy_16)).r;
			srcx_16 += xrIntFloat_16;
		}
		srcy_16 += yrIntFloat_16;
	}
}




inline double SinXDivX(double x)
{
	//该函数计算插值曲线sin(x*PI)/(x*PI)的值 //PI=3.1415926535897932385; 
	//下面是它的近似拟合表达式
	const double a = -1; //a还可以取 a=-2,-1,-0.75,-0.5等等，起到调节锐化或模糊程度的作用

	if (x < 0) x = -x; //x=abs(x);
	double x2 = x*x;
	double x3 = x2*x;
	if (x <= 1)
		return (a + 2)*x3 - (a + 3)*x2 + 1;
	else if (x <= 2)
		return a*x3 - (5 * a)*x2 + (8 * a)*x - (4 * a);
	else
		return 0;
}

inline BYTE border_color(long Color)
{
	if (Color <= 0)
		return 0;
	else if (Color >= 255)
		return 255;
	else
		return (BYTE)Color;
}

TARGB32 CCommon::ThreeOrder0(const MyImage_& pic, const double fx, const double fy)
{
	long x0 = (long)fx; if (x0 > fx) --x0; //x0=floor(fx);    
	long y0 = (long)fy; if (y0 > fy) --y0; //y0=floor(fy);
	double fu = fx - x0;
	double fv = fy - y0;

	TARGB32 pixel[16];
	long i, j;

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			long x = x0 - 1 + j;
			long y = y0 - 1 + i;
			pixel[i * 4 + j] = Pixels_Bound(pic, x, y);
		}
	}

	double afu[4], afv[4];
	//
	afu[0] = SinXDivX(1 + fu);
	afu[1] = SinXDivX(fu);
	afu[2] = SinXDivX(1 - fu);
	afu[3] = SinXDivX(2 - fu);
	afv[0] = SinXDivX(1 + fv);
	afv[1] = SinXDivX(fv);
	afv[2] = SinXDivX(1 - fv);
	afv[3] = SinXDivX(2 - fv);

	double sR = 0, sG = 0, sB = 0;
	for (i = 0; i < 4; ++i)
	{
		double aR = 0, aG = 0, aB = 0;
		for (long j = 0; j < 4; ++j)
		{
			aR += afu[j] * pixel[i * 4 + j].r;
			aG += afu[j] * pixel[i * 4 + j].g;
			aB += afu[j] * pixel[i * 4 + j].b;
		}
		sR += aR*afv[i];
		sG += aG*afv[i];
		sB += aB*afv[i];
	}
	TARGB32 result;
	result.r = border_color((long)(sR + 0.5));
	result.g = border_color((long)(sG + 0.5));
	result.b = border_color((long)(sB + 0.5));
	return result;
}

void CCommon::PicZoom_ThreeOrder0(const MyImage_& Dst, const MyImage_& Src)
{
	if ((0 == Dst.GetWidth()) || (0 == Dst.GetHeight())
		|| (0 == Src.GetWidth()) || (0 == Src.GetHeight())) return;

	long dst_width = Dst.GetWidth();

	for (long y = 0; y < Dst.GetHeight(); ++y)
	{
		double srcy = (y + 0.4999999)*Src.GetHeight() / Dst.GetHeight() - 0.5;
		for (long x = 0; x < dst_width; ++x)
		{
			double srcx = (x + 0.4999999)*Src.GetWidth() / Dst.GetWidth() - 0.5;

			Dst.m_pBits[0][y][x] = (ThreeOrder0(Src, srcx, srcy)).b;
			Dst.m_pBits[1][y][x] = (ThreeOrder0(Src, srcx, srcy)).g;
			Dst.m_pBits[2][y][x] = (ThreeOrder0(Src, srcx, srcy)).r;
		}
	}
}






//static long SinXDivX_Table_8[(2<<8)+1];
//class _CAutoInti_SinXDivX_Table 
//{
//private: 
//	void _Inti_SinXDivX_Table()
//	{
//		for (long i=0;i<=(2<<8);++i)
//			SinXDivX_Table_8[i]=long(0.5+256*SinXDivX(i*(1.0/(256))))*1;
//	};
//public:
//	_CAutoInti_SinXDivX_Table() { _Inti_SinXDivX_Table(); }
//};
//static _CAutoInti_SinXDivX_Table __tmp_CAutoInti_SinXDivX_Table;
//
//
////颜色查表
//static TUInt8 _color_table[256*3];
//static const TUInt8* color_table=&_color_table[256];
//class _CAuto_inti_color_table
//{
//public:
//	_CAuto_inti_color_table() {
//		for (int i=0;i<256*3;++i)
//			_color_table[i]=border_color(i-256);
//	}
//};
//
//static _CAuto_inti_color_table _Auto_inti_color_table;


//TARGB32 ThreeOrder_Fast_Common(const MyImage_& pic,const long x_16,const long y_16)
//{
//	TARGB32 result,pixel;
//	unsigned long u_8=(unsigned char)((x_16)>>8);
//	unsigned long v_8=(unsigned char)((y_16)>>8);
//	//TARGB32* pixel=&Pixels(pic,(x_16>>16)-1,(y_16>>16)-1);
//
//	pixel.r = pic.m_pBits[2][(y_16>>16)-1][(x_16>>16)-1];
//	pixel.g = pic.m_pBits[1][(y_16>>16)-1][(x_16>>16)-1];
//	pixel.b = pic.m_pBits[0][(y_16>>16)-1][(x_16>>16)-1];
//
//	long pic_byte_width=pic.byte_width;
//
//	long au_8[4],av_8[4];
//	//
//	au_8[0]=SinXDivX_Table_8[(1<<8)+u_8];
//	au_8[1]=SinXDivX_Table_8[u_8];
//	au_8[2]=SinXDivX_Table_8[(1<<8)-u_8];
//	au_8[3]=SinXDivX_Table_8[(2<<8)-u_8];
//	av_8[0]=SinXDivX_Table_8[(1<<8)+v_8];
//	av_8[1]=SinXDivX_Table_8[v_8];
//	av_8[2]=SinXDivX_Table_8[(1<<8)-v_8];
//	av_8[3]=SinXDivX_Table_8[(2<<8)-v_8];
//
//	long sR=0,sG=0,sB=0;
//	for (long i=0;i<4;++i)
//	{
//		long aR=au_8[0]*pixel[0].r + au_8[1]*pixel[1].r + au_8[2]*pixel[2].r + au_8[3]*pixel[3].r;
//		long aG=au_8[0]*pixel[0].g + au_8[1]*pixel[1].g + au_8[2]*pixel[2].g + au_8[3]*pixel[3].g;
//		long aB=au_8[0]*pixel[0].b + au_8[1]*pixel[1].b + au_8[2]*pixel[2].b + au_8[3]*pixel[3].b;
//		sR+=aR*av_8[i];
//		sG+=aG*av_8[i];
//		sB+=aB*av_8[i];
//		((TUInt8*&)pixel)+=pic_byte_width;
//	}
//
//	result.r=color_table[sR>>16];
//	result.g=color_table[sG>>16];
//	result.b=color_table[sB>>16];
//	return result;
//}
//
//void ThreeOrder_Border_Common(const TPicRegion& pic,const long x_16,const long y_16,TARGB32* result)
//{
//	long x0_sub1=(x_16>>16)-1;
//	long y0_sub1=(y_16>>16)-1;
//	unsigned long u_16_add1=((unsigned short)(x_16))+(1<<16);
//	unsigned long v_16_add1=((unsigned short)(y_16))+(1<<16);
//
//	TARGB32 pixel[16];
//	long i;
//
//	for (i=0;i<4;++i)
//	{
//		long y=y0_sub1+i;
//		pixel[i*4+0]=Pixels_Bound(pic,x0_sub1+0,y);
//		pixel[i*4+1]=Pixels_Bound(pic,x0_sub1+1,y);
//		pixel[i*4+2]=Pixels_Bound(pic,x0_sub1+2,y);
//		pixel[i*4+3]=Pixels_Bound(pic,x0_sub1+3,y);
//	}
//
//	TPicRegion npic;
//	npic.pdata     =&pixel[0];
//	npic.byte_width=4*sizeof(TARGB32);
//	//npic.width     =4;
//	//npic.height    =4;
//	ThreeOrder_Fast_Common(npic,u_16_add1,v_16_add1,result);
//}
//
//void PicZoom_ThreeOrder_Common(const TPicRegion& Dst,const TPicRegion& Src)
//{
//	if (  (0==Dst.width)||(0==Dst.height)
//		||(0==Src.width)||(0==Src.height)) return;
//
//	long xrIntFloat_16=((Src.width)<<16)/Dst.width+1; 
//	long yrIntFloat_16=((Src.height)<<16)/Dst.height+1;
//	const long csDErrorX=-(1<<15)+(xrIntFloat_16>>1);
//	const long csDErrorY=-(1<<15)+(yrIntFloat_16>>1);
//
//	unsigned long dst_width=Dst.width;
//
//	//计算出需要特殊处理的边界
//	long border_y0=((1<<16)-csDErrorY)/yrIntFloat_16+1;//y0+y*yr>=1; y0=csDErrorY => y>=(1-csDErrorY)/yr
//	if (border_y0>=Dst.height) border_y0=Dst.height;
//	long border_x0=((1<<16)-csDErrorX)/xrIntFloat_16+1;
//	if (border_x0>=Dst.width ) border_x0=Dst.width;
//	long border_y1=(((Src.height-3)<<16)-csDErrorY)/yrIntFloat_16+1; //y0+y*yr<=(height-3) => y<=(height-3-csDErrorY)/yr
//	if (border_y1<border_y0) border_y1=border_y0;
//	long border_x1=(((Src.width-3)<<16)-csDErrorX)/xrIntFloat_16+1;; 
//	if (border_x1<border_x0) border_x1=border_x0;
//
//	TARGB32* pDstLine=Dst.pdata;
//	long srcy_16=csDErrorY;
//	long y;
//	for (y=0;y<border_y0;++y)
//	{
//		long srcx_16=csDErrorX;
//		for (unsigned long x=0;x<dst_width;++x)
//		{
//			ThreeOrder_Border_Common(Src,srcx_16,srcy_16,&pDstLine[x]); //border
//			srcx_16+=xrIntFloat_16;
//		}
//		srcy_16+=yrIntFloat_16;
//		((TUInt8*&)pDstLine)+=Dst.byte_width;
//	}
//	for (y=border_y0;y<border_y1;++y)
//	{
//		long srcx_16=csDErrorX;
//		long x;
//		for (x=0;x<border_x0;++x)
//		{
//			ThreeOrder_Border_Common(Src,srcx_16,srcy_16,&pDstLine[x]);//border
//			srcx_16+=xrIntFloat_16;
//		}
//		for (x=border_x0;x<border_x1;++x)
//		{
//			ThreeOrder_Fast_Common(Src,srcx_16,srcy_16,&pDstLine[x]);//fast  !
//			srcx_16+=xrIntFloat_16;
//		}
//		for (x=border_x1;x<dst_width;++x)
//		{
//			ThreeOrder_Border_Common(Src,srcx_16,srcy_16,&pDstLine[x]);//border
//			srcx_16+=xrIntFloat_16;
//		}
//		srcy_16+=yrIntFloat_16;
//		((TUInt8*&)pDstLine)+=Dst.byte_width;
//	}
//	for (y=border_y1;y<Dst.height;++y)
//	{
//		long srcx_16=csDErrorX;
//		for (unsigned long x=0;x<dst_width;++x)
//		{
//			ThreeOrder_Border_Common(Src,srcx_16,srcy_16,&pDstLine[x]); //border
//			srcx_16+=xrIntFloat_16;
//		}
//		srcy_16+=yrIntFloat_16;
//		((TUInt8*&)pDstLine)+=Dst.byte_width;
//	}
//}



inline bool PixelsIsInPic(const MyImage_& pic, const long x, const long y)
{
	return ((x >= 0) && (x < pic.GetWidth()) && (y >= 0) && (y < pic.GetHeight()));
}


void CCommon::PicRotary0(const MyImage_& Dst, const MyImage_& Src, double RotaryAngle, double ZoomX, double ZoomY, double move_x, double move_y)
{
	if ((fabs(ZoomX*Src.GetWidth()) < 1.0e-4) || (fabs(ZoomY*Src.GetHeight()) < 1.0e-4)) return; //太小的缩放比例认为已经不可见
	double rx0 = Src.GetWidth()*0.5;  //(rx0,ry0)为旋转中心 
	double ry0 = Src.GetHeight()*0.5;
	for (long y = 0; y < Dst.GetHeight(); ++y)
	{
		for (long x = 0; x < Dst.GetWidth(); ++x)
		{
			long srcx = (long)((x - move_x - rx0) / ZoomX*cos(RotaryAngle) - (y - move_y - ry0) / ZoomY*sin(RotaryAngle) + rx0);
			long srcy = (long)((x - move_x - rx0) / ZoomX*sin(RotaryAngle) + (y - move_y - ry0) / ZoomY*cos(RotaryAngle) + ry0);
			if (PixelsIsInPic(Src, srcx, srcy))
			{
				Dst.m_pBits[0][y][x] = Src.m_pBits[0][srcy][srcx];
				Dst.m_pBits[1][y][x] = Src.m_pBits[1][srcy][srcx];
				Dst.m_pBits[2][y][x] = Src.m_pBits[2][srcy][srcx];
			}
		}
	}
}


void CCommon::PicRotary1(const MyImage_& Dst, const MyImage_& Src, double RotaryAngle, double ZoomX, double ZoomY, double move_x, double move_y)
{
	if ((fabs(ZoomX*Src.GetWidth()) < 1.0e-4) || (fabs(ZoomY*Src.GetHeight()) < 1.0e-4)) return; //太小的缩放比例认为已经不可见
	double rZoomX = 1.0 / ZoomX;
	double rZoomY = 1.0 / ZoomY;
	double sinA = sin(RotaryAngle);
	double cosA = cos(RotaryAngle);
	double Ax = (rZoomX*cosA);
	double Ay = (rZoomX*sinA);
	double Bx = (-rZoomY*sinA);
	double By = (rZoomY*cosA);
	double rx0 = Src.GetWidth()*0.5;  //(rx0,ry0)为旋转中心 
	double ry0 = Src.GetHeight()*0.5;
	double Cx = (-(rx0 + move_x)*rZoomX*cosA + (ry0 + move_y)*rZoomY*sinA + rx0);
	double Cy = (-(rx0 + move_x)*rZoomX*sinA - (ry0 + move_y)*rZoomY*cosA + ry0);

	/*TARGB32* pDstLine=Dst.pdata;*/
	double srcx0_f = (Cx);
	double srcy0_f = (Cy);
	for (long y = 0; y < Dst.GetHeight(); ++y)
	{
		double srcx_f = srcx0_f;
		double srcy_f = srcy0_f;
		for (long x = 0; x < Dst.GetWidth(); ++x)
		{
			long srcx = (long)(srcx_f);
			long srcy = (long)(srcy_f);
			if (PixelsIsInPic(Src, srcx, srcy))
			{
				Dst.m_pBits[0][y][x] = Src.m_pBits[0][srcy][srcx];
				Dst.m_pBits[1][y][x] = Src.m_pBits[1][srcy][srcx];
				Dst.m_pBits[2][y][x] = Src.m_pBits[2][srcy][srcx];
			}
			srcx_f += Ax;
			srcy_f += Ay;
		}
		srcx0_f += Bx;
		srcy0_f += By;
		/*((TUInt8*&)pDstLine)+=Dst.byte_width;*/

	}
}


void __declspec(naked) __stdcall SinCos(const double Angle, double& sina, double& cosa)
{
	asm
	{
		fld  qword ptr[esp + 4]//Angle   
		mov  eax,[esp + 12]//&sina
		mov  edx,[esp + 16]//&cosa
		fsincos
			fstp qword ptr[edx]
		fstp qword ptr[eax]
		ret 16
	}
}

void CCommon::PicRotary2(const MyImage_& Dst, const MyImage_& Src, double RotaryAngle, double ZoomX, double ZoomY, double move_x, double move_y)
{
	if ((fabs(ZoomX*Src.GetWidth()) < 1.0e-4) || (fabs(ZoomY*Src.GetHeight()) < 1.0e-4)) return; //太小的缩放比例认为已经不可见
	double tmprZoomXY = 1.0 / (ZoomX*ZoomY);
	double rZoomX = tmprZoomXY*ZoomY;
	double rZoomY = tmprZoomXY*ZoomX;
	double sinA, cosA;
	SinCos(RotaryAngle, sinA, cosA);


	long Ax_16 = (long)(rZoomX*cosA*(1 << 16));
	long Ay_16 = (long)(rZoomX*sinA*(1 << 16));
	long Bx_16 = (long)(-rZoomY*sinA*(1 << 16));
	long By_16 = (long)(rZoomY*cosA*(1 << 16));
	double rx0 = Src.GetWidth()*0.5;  //(rx0,ry0)为旋转中心 
	double ry0 = Src.GetHeight()*0.5;
	long Cx_16 = (long)((-(rx0 + move_x)*rZoomX*cosA + (ry0 + move_y)*rZoomY*sinA + rx0)*(1 << 16));
	long Cy_16 = (long)((-(rx0 + move_x)*rZoomX*sinA - (ry0 + move_y)*rZoomY*cosA + ry0)*(1 << 16));

	/*	TARGB32* pDstLine=Dst.pdata;*/
	long srcx0_16 = (Cx_16);
	long srcy0_16 = (Cy_16);
	for (long y = 0; y < Dst.GetHeight(); ++y)
	{
		long srcx_16 = srcx0_16;
		long srcy_16 = srcy0_16;
		for (long x = 0; x < Dst.GetWidth(); ++x)
		{
			long srcx = (srcx_16 >> 16);
			long srcy = (srcy_16 >> 16);
			if (PixelsIsInPic(Src, srcx, srcy))
			{
				Dst.m_pBits[0][y][x] = Src.m_pBits[0][srcy][srcx];
				Dst.m_pBits[1][y][x] = Src.m_pBits[1][srcy][srcx];
				Dst.m_pBits[2][y][x] = Src.m_pBits[2][srcy][srcx];
			}
			srcx_16 += Ax_16;
			srcy_16 += Ay_16;
		}
		srcx0_16 += Bx_16;
		srcy0_16 += By_16;
		/*((TUInt8*&)pDstLine)+=Dst.byte_width;*/
	}
}



struct TRotaryClipData {
public:
	long src_width;
	long src_height;
	long dst_width;
	long dst_height;
	long Ax_16;
	long Ay_16;
	long Bx_16;
	long By_16;
	long Cx_16;
	long Cy_16;
	long border_width;//插值边界宽度
private:
	long cur_dst_up_x0;
	long cur_dst_up_x1;
	long cur_dst_down_x0;
	long cur_dst_down_x1;
	inline bool is_border_src(long src_x_16, long src_y_16)
	{
		return (((src_x_16 >= (-(border_width << 16))) && ((src_x_16 >> 16) < (src_width + border_width)))
			&& ((src_y_16 >= (-(border_width << 16))) && ((src_y_16 >> 16) < (src_height + border_width))));
	}
	inline bool is_in_src(long src_x_16, long src_y_16)
	{
		return (((src_x_16 >= (border_width << 16)) && ((src_x_16 >> 16) < (src_width - border_width)))
			&& ((src_y_16 >= (border_width << 16)) && ((src_y_16 >> 16) < (src_height - border_width))));
	}
	void find_begin_in(long dst_y, long& out_dst_x, long& src_x_16, long& src_y_16)
	{
		src_x_16 -= Ax_16;
		src_y_16 -= Ay_16;
		while (is_border_src(src_x_16, src_y_16))
		{
			--out_dst_x;
			src_x_16 -= Ax_16;
			src_y_16 -= Ay_16;
		}
		src_x_16 += Ax_16;
		src_y_16 += Ay_16;
	}
	bool find_begin(long dst_y, long& out_dst_x0, long dst_x1)
	{
		long test_dst_x0 = out_dst_x0 - 1;
		long src_x_16 = Ax_16*test_dst_x0 + Bx_16*dst_y + Cx_16;
		long src_y_16 = Ay_16*test_dst_x0 + By_16*dst_y + Cy_16;
		for (long i = test_dst_x0; i <= dst_x1; ++i)
		{
			if (is_border_src(src_x_16, src_y_16))
			{
				out_dst_x0 = i;
				if (i == test_dst_x0)
					find_begin_in(dst_y, out_dst_x0, src_x_16, src_y_16);
				if (out_dst_x0 < 0)
				{
					src_x_16 -= (Ax_16*out_dst_x0);
					src_y_16 -= (Ay_16*out_dst_x0);
				}
				out_src_x0_16 = src_x_16;
				out_src_y0_16 = src_y_16;
				return true;
			}
			else
			{
				src_x_16 += Ax_16;
				src_y_16 += Ay_16;
			}
		}
		return false;
	}
	void find_end(long dst_y, long dst_x0, long& out_dst_x1)
	{
		long test_dst_x1 = out_dst_x1;
		if (test_dst_x1 < dst_x0) test_dst_x1 = dst_x0;

		long src_x_16 = Ax_16*test_dst_x1 + Bx_16*dst_y + Cx_16;
		long src_y_16 = Ay_16*test_dst_x1 + By_16*dst_y + Cy_16;
		if (is_border_src(src_x_16, src_y_16))
		{
			++test_dst_x1;
			src_x_16 += Ax_16;
			src_y_16 += Ay_16;
			while (is_border_src(src_x_16, src_y_16))
			{
				++test_dst_x1;
				src_x_16 += Ax_16;
				src_y_16 += Ay_16;
			}
			out_dst_x1 = test_dst_x1;
		}
		else
		{
			src_x_16 -= Ax_16;
			src_y_16 -= Ay_16;
			while (!is_border_src(src_x_16, src_y_16))
			{
				--test_dst_x1;
				src_x_16 -= Ax_16;
				src_y_16 -= Ay_16;
			}
			out_dst_x1 = test_dst_x1;
		}
	}

	inline void update_out_dst_x_in()
	{
		if ((0 == border_width) || (out_dst_x0_boder >= out_dst_x1_boder))
		{
			out_dst_x0_in = out_dst_x0_boder;
			out_dst_x1_in = out_dst_x1_boder;
		}
		else
		{
			long src_x_16 = out_src_x0_16;
			long src_y_16 = out_src_y0_16;
			long i = out_dst_x0_boder;
			while (i < out_dst_x1_boder)
			{
				if (is_in_src(src_x_16, src_y_16)) break;
				src_x_16 += Ax_16;
				src_y_16 += Ay_16;
				++i;
			}
			out_dst_x0_in = i;

			src_x_16 = out_src_x0_16 + (out_dst_x1_boder - out_dst_x0_boder)*Ax_16;
			src_y_16 = out_src_y0_16 + (out_dst_x1_boder - out_dst_x0_boder)*Ay_16;
			i = out_dst_x1_boder;
			while (i > out_dst_x0_in)
			{
				src_x_16 -= Ax_16;
				src_y_16 -= Ay_16;
				if (is_in_src(src_x_16, src_y_16)) break;
				--i;
			}
			out_dst_x1_in = i;
		}
	}
	inline void update_out_dst_up_x()
	{
		if (cur_dst_up_x0 < 0)
			out_dst_x0_boder = 0;
		else
			out_dst_x0_boder = cur_dst_up_x0;
		if (cur_dst_up_x1 >= dst_width)
			out_dst_x1_boder = dst_width;
		else
			out_dst_x1_boder = cur_dst_up_x1;
		update_out_dst_x_in();
	}
	inline void update_out_dst_down_x()
	{
		if (cur_dst_down_x0 < 0)
			out_dst_x0_boder = 0;
		else
			out_dst_x0_boder = cur_dst_down_x0;
		if (cur_dst_down_x1 >= dst_width)
			out_dst_x1_boder = dst_width;
		else
			out_dst_x1_boder = cur_dst_down_x1;
		update_out_dst_x_in();
	}

public:
	long out_src_x0_16;
	long out_src_y0_16;

	long out_dst_up_y;
	long out_dst_down_y;

	long out_dst_x0_boder;
	long out_dst_x0_in;
	long out_dst_x1_in;
	long out_dst_x1_boder;

public:
	bool inti_clip(double move_x, double move_y, unsigned long aborder_width)
	{
		border_width = aborder_width;

		//计算src中心点映射到dst后的坐标
		out_dst_down_y = (long)(src_height*0.5 + move_y);
		cur_dst_down_x0 = (long)(src_width*0.5 + move_x);
		cur_dst_down_x1 = cur_dst_down_x0;
		//得到初始扫描线
		if (find_begin(out_dst_down_y, cur_dst_down_x0, cur_dst_down_x1))
			find_end(out_dst_down_y, cur_dst_down_x0, cur_dst_down_x1);
		out_dst_up_y = out_dst_down_y;
		cur_dst_up_x0 = cur_dst_down_x0;
		cur_dst_up_x1 = cur_dst_down_x1;
		update_out_dst_up_x();
		return (cur_dst_down_x0 < cur_dst_down_x1);
	}
	bool next_clip_line_down()
	{
		++out_dst_down_y;
		if (!find_begin(out_dst_down_y, cur_dst_down_x0, cur_dst_down_x1)) return false;
		find_end(out_dst_down_y, cur_dst_down_x0, cur_dst_down_x1);
		update_out_dst_down_x();
		return (cur_dst_down_x0 < cur_dst_down_x1);
	}
	bool next_clip_line_up()
	{
		--out_dst_up_y;
		if (!find_begin(out_dst_up_y, cur_dst_up_x0, cur_dst_up_x1)) return false;
		find_end(out_dst_up_y, cur_dst_up_x0, cur_dst_up_x1);
		update_out_dst_up_x();
		return (cur_dst_up_x0 < cur_dst_up_x1);
	}
};



//废弃，有问题，暂不用
//void CCommon::RGBToHSI(const DWORD nCr,const DWORD nCg,const DWORD nCb,double *pCh,double *pCs,double *pCi)
//{
//	//double eps = 1e-20; //改为宏定义
//	double num1 = 0.5*((nCr-nCg)+(nCr-nCb));
//	double num2 = sqrt(pow(double(nCr-nCg),2) + (nCr-nCb)*(nCg-nCb))+eps;
//	double theta = acos(num1/num2)/**(180/pi)*/;
//	/*double theta = acos(num1/num2)/2*pi;*/
//	double tempH = theta;
//
//	if (nCb>nCg)
//	{
//		tempH = 2*pi-theta;
//	}
//	tempH /= 2*pi; //归一化
//	DWORD dwMinRGB = ((nCr<nCg ? nCr : nCg) < nCb ? (nCr<nCg ? nCr : nCg) : nCb);
//	*pCs = (1-3/(nCr+nCg+nCb+eps)*dwMinRGB)/**100*/;
//	*pCi = (nCr+nCb+nCg)/3;
//	*pCh = tempH;
//}


//废弃，有问题，暂不用
//void CCommon::HSIToRGB(const double nCh,const double nCs,const double nCi,DWORD *pCr,DWORD *pCg,DWORD *pCb)
//{
//	//int H = nCh;
//	//int S = nCs/100;
//	//int I = nCi/255;
//
//	//if(H>=0 && H<120)
//	//	*pCr = I*(1+S*)
//	
//	double H = nCh*2*pi;
//	double S = nCs;
//	double I = nCi;
//
//	if (H>=0 && H<2*pi/3)
//	{
//		*pCb = I*(1-S);
//		*pCr = I*(1+S*cos(H)/cos(pi/3-H));
//		*pCg = 3*I-(*pCr+*pCb);
//		
//	}else if(H>=2*pi/3 && H<4*pi/3)
//	{
//		*pCr = I*(1-S);
//		*pCg = I*(1+S*cos(H-2*pi/3)/cos(pi-H));
//		*pCb = 3*I-(*pCr+*pCg);
//
//	}else if(H>=4*pi/3 && H<=2*pi)
//	{
//		*pCg = I*(1-S);
//		*pCb = I*(1+S*cos(H-4*pi/3)/cos(5*pi/3-H));
//		*pCr = 3*I-(*pCg+*pCb);
//
//	} else
//	{
//		*pCb = 0;
//		*pCg = 0;
//		*pCr = 0;
//	}
//
//}

//废弃，有问题，暂不用
//void CCommon::HSIToRGB(const double nCh,const double nCs,const double nCi,DWORD *pCr,DWORD *pCg,DWORD *pCb,DWORD r,DWORD g,DWORD b)
//{
//	double H = nCh*2*pi;
//	double S = nCs;
//	double I = nCi;
//
//	if (H>=0 && H<2*pi/3)
//	{
//		*pCb = I*(1-S);
//		*pCr = I*(1+S*cos(H)/cos(pi/3-H));
//		*pCg = 3*I-(*pCr+*pCb);
//		//if(abs(int(*pCr-r))>5 || abs(int(*pCg-g))>5 ||abs(int(*pCr-r))>5)
//			//std::cout << "1 do" << std::endl;
//
//
//	}else if(H>=2*pi/3 && H<4*pi/3)
//	{
//		*pCr = I*(1-S);
//		*pCg = I*(1+S*cos(H-2*pi/3)/cos(pi-H));
//		*pCb = 3*I-(*pCr+*pCg);
//		//if(abs(int(*pCr-r))>5 || abs(int(*pCg-g))>5 ||abs(int(*pCr-r))>5)
//			std::cout << "2 do" << std::endl;
//
//	}else if(H>=4*pi/3 && H<=2*pi)
//	{
//		*pCg = I*(1-S);
//		*pCb = I*(1+S*cos(H-4*pi/3)/cos(5*pi/3-H));
//		/**pCr = 3*I-(*pCg+*pCb);*/
//		*pCr = 3*I-(*pCg+b);
//		if(abs(int(*pCr-r))>5 || abs(int(*pCg-g))>5 ||abs(int(*pCb-b))>5)
//		{
//			std::cout << "3 error" << std::endl;
//			std::cout << *pCr << "," << r << " " << *pCb << "," <<b << std::endl;
//			std::cout << abs(int(*pCr-r)) + abs(int(*pCr-r)) << std:: endl;
//		}
//			
//
//	} else
//	{
//		
//		*pCb = 0;
//		*pCg = 0;
//		*pCr = 0;
//		if(abs(int(*pCr-r))>5 || abs(int(*pCg-g))>5 ||abs(int(*pCr-r))>5)
//			std::cout << "4 error" << "H is:" << H << std::endl;
//	}
//}



void CCommon::ChangedColourFromRGBStandardtoHSL(int nR, int nG, int nB, int *pH, int *pS, int *pI)
{
	double	R = ((double)nR) / 255;
	double	G = ((double)nG) / 255;
	double	B = ((double)nB) / 255;

	if (nR == nG && nG == nB)
	{
		int	nHValue = 0;
		int	nSValue = 0;
		int	nLValue = (int)(((double)nR) / 255 * 100);

		*pH = nHValue;
		*pS = nSValue;
		*pI = nLValue;
		return;
	}
	double max1, min1;
	max1 = R > G ? R : G;
	max1 = max1 > B ? max1 : B;
	min1 = R < G ? R : G;
	min1 = min1 < B ? min1 : B;
	double	max_value_of_rgb = max1;

	double	min_value_of_rgb = min1;

	double	fSumRGB = R + G + B;
	if (fSumRGB <= 0.0)
		fSumRGB = 0.001;

	double	I = (R + G + B) / 3;
	double	S = 1.0 - 3.0 * min_value_of_rgb / fSumRGB;
	double	H = acos(((R - G) + (R - B)) / 2 / sqrt((R - G)*(R - G) + (R - B) * (G - B) + 0.0001));

	double	fHAngle = H / PI * 180;
	//int	nHAngle = fHAngle;
	if (G < B)
		fHAngle = 360 - fHAngle;
	if (fHAngle > 360.0)
		fHAngle = 360.0;

	*pH = int(fHAngle / 360 * 255);
	*pS = int(S * 100);
	*pI = int(I * 100);
}

void CCommon::ChangedColourFromHSLStandardtoRGB(int nHValue, int nSValue, int nLValue, int *pR, int *pG, int *pB)
{
	double	fHAngle = ((double)nHValue) / 255 * 360;

	double	H = fHAngle / 180 * PI;
	double	S = ((double)nSValue) / 100;
	double	I = ((double)nLValue) / 100;

	BOOL	bLocalEndFlag = FALSE;

	double	R = -1;
	double	G = -1;
	double	B = -1;
	if (bLocalEndFlag == FALSE && fHAngle >= 0 && fHAngle < 120)
	{
		B = I * (1.0 - S);
		R = I * (1.0 + (S * cos(H) / cos(60.0 / 180 * PI - H)));
		G = 3.0 * I - (B + R);
		bLocalEndFlag = TRUE;
	}
	if (bLocalEndFlag == FALSE && fHAngle >= 120 && fHAngle < 240)
	{
		R = I * (1.0 - S);
		G = I * (1.0 + S * cos(H - 120.0 / 180 * PI) / cos(180.0 / 180 * PI - H));
		B = 3.0 * I - (R + G);
		bLocalEndFlag = TRUE;
	}
	if (bLocalEndFlag == FALSE && fHAngle >= 240 && fHAngle <= 360)
	{
		G = I * (1.0 - S);
		B = I * (1.0 + S * cos(H - 240.0 / 180 * PI) / cos(300.0 / 180 * PI - H));
		R = 3.0 * I - (G + B);
		bLocalEndFlag = TRUE;
	}
	*pR = (int)(R * 255);
	*pG = (int)(G * 255);
	*pB = (int)(B * 255);
}


template <class T>
void CCommon::VecNormalized(vector<T> &vecT, T normMin, T normMax)
{
	vector<T> temp(vecT.begin(), vecT.end());
	sort(temp.begin(), temp.end());
	T Tmax = temp[temp.size() - 1];
	T Tmin = temp[0];

	//归一化到0-255
	for (auto it = vecT.begin(); it != vecT.end(); ++it)
	{
		*it = ((*it) - Tmin) / (Tmax - Tmin)*(normMax - normMin) + normMin;
	}
}



vector<int> CCommon::get_coords(double angle)
{
	double sigma = 0.000000001;
	int x1 = (int)ceil(cos(angle + pi / 8)*sqrt(2.0) - 0.5 - sigma);
	int y1 = (int)ceil(-sin(angle - pi / 8)*sqrt(2.0) - 0.5 - sigma);
	int x2 = (int)ceil(cos(angle - pi / 8)*sqrt(2.0) - 0.5 - sigma);
	int y2 = (int)ceil(-sin(angle - pi / 8)*sqrt(2.0) - 0.5 - sigma);

	vector<int> vecInt;
	vecInt.push_back(x1);
	vecInt.push_back(y1);
	vecInt.push_back(x2);
	vecInt.push_back(y2);
	return vecInt;
}

static int s_nLoopTime = 0;
void CCommon::Connect(MyImage_ &img, int y, int x, int nLowThresh)
{
	s_nLoopTime++;
	int neighbour[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
	int nWidth = img.GetWidth();
	int nHeight = img.GetHeight();
	for (int k = 0; k != 8; ++k)
	{
		int yy = y + neighbour[k][0];
		int xx = x + neighbour[k][1];
		if (yy >= 0 && yy <= nHeight - 1 && xx >= 0 && xx <= nWidth - 1) //这里需要注意边界在C中的下标
		{
			if (img.m_pBits[0][yy][xx] >= nLowThresh && img.m_pBits[0][yy][xx] != 255)
			{
				img.m_pBits[0][yy][xx] = 255;
				Connect(img, yy, xx, nLowThresh);
			}
		}
	}
	// 	if (s_nLoopTime>10000)
	// 	{
	// 		std::cout << "递归大于100000次！" << std::endl;
	// 	}
}


void CCommon::Connect(CMyImage_double &img, int y, int x, int nLowThresh)
{
	s_nLoopTime++;
	int neighbour[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
	int nWidth = img.m_nHeight;
	int nHeight = img.m_nWidth;
	for (int k = 0; k != 8; ++k)
	{
		int yy = y + neighbour[k][0];
		int xx = x + neighbour[k][1];
		if (yy >= 0 && yy <= nHeight - 1 && xx >= 0 && xx <= nWidth - 1) //这里需要注意边界在C中的下标
		{
			if (img.m_pBits[0][yy][xx] >= (double)nLowThresh && img.m_pBits[0][yy][xx] != 255.0)
			{
				img.m_pBits[0][yy][xx] = 255.0;
				Connect(img, yy, xx, nLowThresh);
			}
		}
	}

	// 	if (s_nLoopTime>10000)
	// 	{
	// 		std::cout << "递归大于100000次！" << std::endl;
	// 	}

}



//void findCross(double *src,double *dst,int width,int height,double threshold){
//	double *dsttemp=(double *)malloc(sizeof(double)*width*height);
//	Zero(dst, width, height);
//	double c_value=0.0;
//	int flag=1;
//	Zero(dsttemp, width, height);
//	for(int j=1;j<height-1;j++){
//		for(int i=1;i<width-1;i++){
//			c_value=src[j*width+i];
//			flag=1;
//			for(int m=-1;m<=1&&flag;m++)
//				for(int n=-1;n<=1;n++){
//					if(c_value>threshold&&src[(j+m)*width+i+n]<-0.1){
//						flag=0;
//						dsttemp[j*width+i]=255.0;
//						break;
//					}
//
//				}
//		}
//	}
//	matrixCopy(dsttemp, dst,width, height);
//	free(dsttemp);
//
//
//}


int CCommon::TextSegment(const CString &strInput, const CString &strSeg, vector<CString> &vecOutput)
{
	vecOutput.clear();
	CString strCopy(strInput);
	//strCopy.Replace('\n',' ');
	strCopy.Replace(_T(" "), _T("")); //去掉所有空格

	int nPos = 0;
	vector<int> vecSegPos;
	while (nPos != -1)
	{
		nPos = strCopy.Find(strSeg, nPos);
		if (nPos != -1)
		{
			//cout << nPos << "," << endl;
			vecSegPos.push_back(nPos);
			nPos++;
		}
	}

	vecOutput.push_back(strCopy.Mid(0, vecSegPos[0]));

	CString strTemp;
	for (int i = 0; i != vecSegPos.size() - 1; ++i)
	{
		strTemp = strCopy.Mid(vecSegPos[i] + 1, vecSegPos[i + 1] - vecSegPos[i] - 1);
		vecOutput.push_back(strTemp);
	}

	vecOutput.push_back(strCopy.Right(strCopy.GetLength() - vecSegPos[vecSegPos.size() - 1] - 1));

	return vecOutput.size();
}


double* CCommon::getGussianKernalData(int n, double sigma) {
	int size = 2 * n + 1;
	double sigma22 = 2 * sigma*sigma;
	double sigma22PI = (double)PI * sigma22;
	double *kernalData = new double[size*size];
	double sum = 0.0;
	int row = 0;
	for (int i = -n; i <= n; i++)
	{
		int column = 0;
		for (int j = -n; j <= n; j++)
		{
			double xDistance = i*i;
			double yDistance = j*j;
			kernalData[row*size + column] = (double)exp(-(xDistance + yDistance) / sigma22) / sigma22PI;
			sum += kernalData[row*size + column];
			column++;
		}
		row++;
	}

	//归一化，防止图像亮度提升或者降低
	double mean = sum / (double)(size*size);
	double mu = 1 / (double)(size*size);
	for (int i = 0; i != size; ++i)
	{
		for (int j = 0; j != size; ++j)
		{
			kernalData[i*size + j] = kernalData[i*size + j] - mean + mu;
		}
	}

	return kernalData;
}

vector<char> CCommon::DecimalDec2Bin(double num, int digit)
{
	vector<char> code;
	if (digit < 1)
	{
		return code;
	}
	int getDigit = 0;
	bool equal0 = false;
	while (getDigit < digit)
	{
		if (!equal0)
		{
			num *= 2;
			if (num >= 1.0)
			{
				num -= 1.0;
				code.push_back(1);
				if (num<eps && num > -eps)
				{
					equal0 = true;
				}
			}
			else
			{
				code.push_back(0);
			}
		}
		else
		{
			code.push_back(0);
		}

		getDigit += 1;
	}
	return code;

}


void CCommon::sortindex(double a[], int p[], int nSize)
{
	double temp;
	int tempIndex;

	for (int i = 0; i != nSize - 1; ++i)
	{
		for (int j = nSize - 1; j != i; --j)
		{
			if (a[j] > a[j - 1])
			{
				temp = a[j];
				a[j] = a[j - 1];
				a[j - 1] = temp;

				tempIndex = p[j - 1];
				p[j - 1] = p[j];
				p[j] = tempIndex;
			}
		}
	}

}