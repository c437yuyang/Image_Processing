#include "StdAfx.h"
#include "Canny.h"


CCanny::CCanny(void)
{
}


CCanny::~CCanny(void)
{
}


void CCanny::doCanny(MyImage_ &srcImg,MyImage_ &dstImg,int nLowThresh,int nHighThresh)
{
	//int nFtPara = 1;
	//if(srcImg.IsNull())
	//	return;

	//MyImage_ imgGrayed;
	//doToGray(srcImg,imgGrayed);

	//MyImage_ imgFilled;
	//imgGrayed.BorderFillTo(imgFilled,nFtPara,2);
	//UINT w = imgFilled.GetWidth();
	//UINT h = imgFilled.GetHeight();

	//CMyImage_double imgdGradient,imgdGradientX,imgdGradientY,imgdGaussed; //全部是填充后的图像
	//imgdGaussed.Create(w,h,0);

	//double gtpl[3][3] = {{0.0113,0.0838,0.0113},{0.0838,0.6193,0.0838},{0.0113,0.0838,0.0113}}; //高斯模板

	////先进行高斯滤波
	//for (UINT i=nFtPara;i<h-nFtPara;++i) //循环次数只有h-2
	//{
	//	for (UINT j=nFtPara;j<w-nFtPara;++j)//循环w-2次
	//	{
	//		double sum[3]={0,0,0};//三个通道分别的和
	//		//应用模板
	//		UINT r=0,c=0;
	//		for (UINT i1=i-nFtPara;i1<=i+nFtPara;i1++) //行
	//		{
	//			for (UINT j1=j-nFtPara;j1<=j+nFtPara;j1++) //列
	//			{
	//				double temp = gtpl[r][c];
	//				sum[0]+=(double)imgFilled.m_pBits[0][i1][j1]*temp;
	//				sum[1]+=(double)imgFilled.m_pBits[1][i1][j1]*temp;
	//				sum[2]+=(double)imgFilled.m_pBits[2][i1][j1]*temp;
	//				c++;
	//			}
	//			c=0;
	//			r++;
	//		}
	//		imgdGaussed.m_pBits[0][i][j] = sum[0];
	//		imgdGaussed.m_pBits[1][i][j] = sum[1];
	//		imgdGaussed.m_pBits[2][i][j] = sum[2];
	//	}
	//}

	////imgBackUp.RemoveFillTo(m_ImageAfter,nFtPara);
	////UpdateState();

	//imgdGaussed.CopyTo(imgdGradientX);
	//imgdGaussed.CopyTo(imgdGradientY);
	//imgdGaussed.CopyTo(imgdGradient);

	//double tplX[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}}; //sobel 水平
	//double tplY[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}}; //sobel 垂直

	////求出梯度图像
	//for (UINT i=nFtPara;i<h-nFtPara;++i) //循环次数只有h-2
	//{
	//	for (UINT j=nFtPara;j<w-nFtPara;++j)//循环w-2次
	//	{
	//		double sumX[3]={0,0,0};//三个通道分别的和
	//		double sumY[3]={0,0,0};
	//		UINT r=0,c=0;
	//		for (UINT i1=i-nFtPara;i1<=i+nFtPara;i1++) //行
	//		{
	//			for (UINT j1=j-nFtPara;j1<=j+nFtPara;j1++) //列
	//			{
	//				double tempX = tplX[r][c];
	//				double tempY = tplY[r][c];
	//				sumX[0]+=imgdGaussed.m_pBits[0][i1][j1]*tempX;
	//				sumX[1]+=imgdGaussed.m_pBits[1][i1][j1]*tempX;
	//				sumX[2]+=imgdGaussed.m_pBits[2][i1][j1]*tempX;
	//				imgdGradientX.m_pBits[0][i][j] = sumX[0];
	//				imgdGradientX.m_pBits[1][i][j] = sumX[1];
	//				imgdGradientX.m_pBits[2][i][j] = sumX[2];

	//				sumY[0]+=imgdGaussed.m_pBits[0][i1][j1]*tempY;
	//				sumY[1]+=imgdGaussed.m_pBits[1][i1][j1]*tempY;
	//				sumY[2]+=imgdGaussed.m_pBits[2][i1][j1]*tempY;

	//				imgdGradientY.m_pBits[0][i][j] = sumY[0];
	//				imgdGradientY.m_pBits[1][i][j] = sumY[1];
	//				imgdGradientY.m_pBits[2][i][j] = sumY[2];
	//				c++;
	//			}
	//			c=0;
	//			r++;
	//		}

	//		double temp = sqrt(sumX[0]*sumX[0] + sumY[0]*sumY[0]);
	//		imgdGradient.m_pBits[0][i][j] = temp;
	//		imgdGradient.m_pBits[1][i][j] = temp;
	//		imgdGradient.m_pBits[2][i][j] = temp;
	//	}
	//}

	////进行非极大值抑制
	//CMyImage_double imgdNewEdge,imgdGradient1,imgdGradientX1,imgdGradientY1; //带1的是去除边缘后的
	//imgdGradient.RemoveFillTo(imgdGradient1,nFtPara);
	//imgdGradientX.RemoveFillTo(imgdGradientX1,nFtPara);
	//imgdGradientY.RemoveFillTo(imgdGradientY1,nFtPara);
	//imgdNewEdge.Create(imgdGradientX1.m_nWidth,imgdGradientX1.m_nWidth,0);
	//vector<double> dVecMag,dVecPhase;
	//double angle = 0.0,mag = 0.0;
	//for (int i=0;i<imgdGradient1.m_nHeight;++i) //循环次数只有h-2
	//{
	//	for (int j=0;j<imgdGradient1.m_nWidth;++j)//循环w-2次
	//	{

	//		double Mx = imgdGradientX1.m_pBits[0][i][j]; //只取一个通道就行了，都一样的
	//		double My = imgdGradientY1.m_pBits[0][i][j];		
	//		dVecMag.push_back(sqrt(Mx*Mx+My*My));
	//		if (Mx == 0)
	//		{
	//			if (My>0)
	//			{
	//				angle = 90;
	//			}
	//			if (My < 0)
	//			{
	//				angle = -90;
	//			}
	//		}
	//		else if(My == 0)
	//		{
	//			angle = 0.0;
	//		}
	//		else
	//		{
	//			angle = atan(My/Mx) * 180 /pi;
	//		}

	//		angle+=90;
	//		dVecPhase.push_back(angle);
	//	}
	//}

	////这里最好是填充一下边缘，用0填充
	//int index = 0;
	//vector<double> dVecMag1 = dVecMag;
	//for (int i=1;i<imgdGradient1.m_nHeight-1;++i) 
	//{
	//	for (int j=1;j<imgdGradient1.m_nWidth-1;++j)
	//	{
	//		index = i*imgdGradient1.m_nWidth+j;
	//		angle = dVecPhase[index];
	//		mag = dVecMag[index];
	//		dVecMag1[index] = mag;
	//		if (angle>=0 && angle <22.5)
	//		{
	//			double m1 = dVecMag[index-1];
	//			double m2 = dVecMag[index+1];

	//			if (mag<m1 || mag < m2)
	//			{
	//				dVecMag1[index] = 0;
	//			}

	//		}
	//		else if(angle>=22.5 && angle <67.5)
	//		{
	//			double m1 = dVecMag[index+1-imgdGradient1.m_nWidth];
	//			double m2 = dVecMag[index-1+imgdGradient1.m_nWidth];
	//			if (mag<m1 || mag < m2)
	//			{
	//				dVecMag1[index] = 0;
	//			}
	//		}
	//		else if(angle>=67.5 && angle <112.5)
	//		{
	//			double m1 = dVecMag[index+imgdGradient1.m_nWidth];
	//			double m2 = dVecMag[index-imgdGradient1.m_nWidth];
	//			if (mag<m1 || mag < m2)
	//			{
	//				dVecMag1[index] = 0;
	//			}
	//		}
	//		else if(angle>=112.5 && angle <157.5)
	//		{
	//			double m1 = dVecMag[index-1-imgdGradient1.m_nWidth];
	//			double m2 = dVecMag[index+1+imgdGradient1.m_nWidth];
	//			if (mag<m1 || mag < m2)
	//			{
	//				dVecMag1[index] = 0;
	//			}
	//		}
	//		else if(angle >=157.5)
	//		{
	//			double m1 = dVecMag[index+1];
	//			double m2 = dVecMag[index-1];
	//			if (mag<m1 || mag < m2)
	//			{
	//				dVecMag1[index] = 0;
	//			}
	//		}
	//	}
	//}
}