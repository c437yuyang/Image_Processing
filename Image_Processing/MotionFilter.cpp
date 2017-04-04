#include "StdAfx.h"
#include "MotionFilter.h"

template<class T>
T SaturateCast(T value,const T minT,const T maxT);

CMotionFilter::CMotionFilter(void)
{
}


CMotionFilter::~CMotionFilter(void)
{
}


void CMotionFilter::doMotionFilter(CMyImage_double &srcImg,CMyImage_double &dstImg)
{
	int w = srcImg.m_nWidth;
	int h = srcImg.m_nHeight;
	int index=0;
	int cx = w / 2;
	int cy = h / 2;

	double sinAngle = sin(m_dAngle/180.0*pi);
	double cosAngle = cos(m_dAngle/180.0*pi);


	double imgRadius = sqrt((double)cx*cx + cy*cy);

	double maxDistance = m_dDistance + imgRadius*m_dZoom;

	int interation = (int)maxDistance;

	for(int row = 0;row != h; ++row)
	{
		int tr=0,tg=0,tb=0;
		for (int col=0;col!=w;++col)
		{
			int newX = col,count =0,newY = row;

			double m11=0.0,m22 = 0.0;

			for (int i=0;i<interation;++i)
			{
				newX = col;
				newY = row;
				if (m_dDistance > 0)
				{
					newY = (int)floor(newY+i*sinAngle);
					newX = (int)floor(newX+i*cosAngle);
				}
				double f = (double)i/interation;
				if (newX <0 || newX>=w) break;
				if (newY <0 || newY>=h) break;	

				double scale = 1-m_dZoom*f;
				m11 = cx - cx*scale;
				m22 = cy - cy*scale;

				newY = (int)(newY*scale + m22);
				newX = (int)(newX*scale + m11);

				count++;

				//tb += srcImg.m_pBits[0][newX][newY];
				//tg += srcImg.m_pBits[1][newX][newY];
				//tr += srcImg.m_pBits[2][newX][newY];

				tb += (int)srcImg.m_pBits[0][newY][newX];
				tg += (int)srcImg.m_pBits[1][newY][newX];
				tr += (int)srcImg.m_pBits[2][newY][newX];

			}

			if (count == 0)
			{
				dstImg.m_pBits[0][row][col] = srcImg.m_pBits[0][row][col];
				dstImg.m_pBits[1][row][col] = srcImg.m_pBits[1][row][col];
				dstImg.m_pBits[2][row][col] = srcImg.m_pBits[2][row][col];
			}
			else
			{
				tr = SaturateCast((int)(tr/count),0,255);
				tg = SaturateCast((int)(tg/count),0,255);
				tb = SaturateCast((int)(tb/count),0,255);
				dstImg.m_pBits[0][row][col] = tb;
				dstImg.m_pBits[1][row][col] = tg;
				dstImg.m_pBits[2][row][col] = tr;
			}
			index++;
		}
	}
}


template<class T>
T SaturateCast(T value,const T minT,const T maxT) 
{
	if (value>maxT)
	{
		value = maxT;
	} 
	else if(value < minT)
	{
		value = minT;
	}
	return value;
}