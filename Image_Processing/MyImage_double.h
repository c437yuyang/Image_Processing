#pragma once


#include "MyImage_.h"

//1.这个类只实现了MyImage到这个类的拷贝构造函数，没实现类自身到类自身的拷贝构造函数
class CMyImage_double
{
public:
	CMyImage_double(void);
	CMyImage_double(MyImage_ &srcImg);
	~CMyImage_double(void);
	double*** m_pBits;//三维数组，用于保存图像的BGR像素值
	int m_nHeight;
	int m_nWidth;
	bool m_bIsGrayed;

	void  CMyImage_double::Create(
		_In_ int width ,
		_In_ int height,
		_In_ UINT color);
	void Destroy();
	void CMyImage_double::CopyTo(CMyImage_double &img1) const;
	void CMyImage_double::CopyTo(MyImage_ &img1,bool bIsSaturateCast = true) const;
	void CMyImage_double::RemoveFillTo(MyImage_ &img1,UINT nFillPara) const;
	void CMyImage_double::RemoveFillTo(MyImage_ &img1,UINT nFillPara,bool bIs=true) const;
	void CMyImage_double::RemoveFillTo(CMyImage_double &img1,UINT nFillPara) const ;
	void CMyImage_double::BorderFillTo(CMyImage_double &img1,UINT nFillPara,int FillMode) const;
};

