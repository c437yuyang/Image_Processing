#include "StdAfx.h"
#include "HoughTransformer.h"




HoughTransformer::HoughTransformer(void)
{
	hough_space = 500;
	threshold = 0.5f;
	scale = 1.0f;
	offset = 0.0f;
}


HoughTransformer::~HoughTransformer(void)
{
}


template<class T>
T SaturateCast1(T value,const T minT,const T maxT) 
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


void HoughTransformer::HoughTransform(const MyImage_ &srcImg,MyImage_ &dstImg)
{
	if (dstImg.IsNull())
	{
		//srcImg.CopyTo(dstImg);
		dstImg.Create(srcImg.GetWidth(),srcImg.GetHeight(),0);
	}

	width = srcImg.GetWidth();
	height = srcImg.GetHeight();

	MyImage_ imgTemp(srcImg);

	int centerX = width / 2;
	int centerY = height / 2;
	double hough_interval = pi/(double)hough_space;

	//hough_space 代表theta轴,max_length代表p轴
	int max = max(width, height);
	int max_length = (int)(sqrt(2.0) * max);
	hough_1d = new int[2 * hough_space * max_length];

	hough_2d = new int *[hough_space];
	for (int i=0;i<hough_space;++i)
	{
		hough_2d[i] = new int[2*max_length];
	}


	// define temp hough 2D array and initialize the hough 2D
	for(int i=0; i<hough_space; i++) 
	{
		for(int j=0; j<2*max_length; j++) 
		{
			hough_2d[i][j] = 0;
		}
	}

	for (int row = 0; row < height; row++) 
	{
		for (int col = 0; col < width; col++) 
		{
			int p = imgTemp.m_pBits[0][row][col] & 0xff;
			if(p == 0) continue; // which means background color

			// since we does not know the theta angle and r value, 
			// we have to calculate all hough space for each pixel point
			// then we got the max possible theta and r pair.
			// r = x * cos(theta) + y * sin(theta)
			for(int cell=0; cell < hough_space; cell++ ) 
			{
				max = (int)((col - centerX) * cos(cell * hough_interval) + (row - centerY) * sin(cell * hough_interval));
				max += max_length; // start from zero, not (-max_length)
				if (max < 0 || (max >= 2 * max_length)) 
				{// make sure r did not out of scope[0, 2*max_lenght]
					continue;
				}
				hough_2d[cell][max] +=1;
			}
		}
	}


	// find the max hough value
	int max_hough = 0;
	for(int i=0; i<hough_space; i++) 
	{
		for(int j=0; j<2*max_length; j++) 
		{
			hough_1d[(i + j * hough_space)] = hough_2d[i][j];
			if(hough_2d[i][j] > max_hough) 
			{
				max_hough = hough_2d[i][j];
			}
		}
	}
	printf("MAX HOUGH VALUE = %d\n",max_hough);


	// transfer back to image pixels space from hough parameter space
	int hough_threshold = (int)(threshold * max_hough);
	for(int row = 0; row < hough_space; row++) 
	{
		for(int col = 0; col < 2*max_length; col++) 
		{
			if(hough_2d[row][col] < hough_threshold) // discard it
				continue;
			int hough_value = hough_2d[row][col];
			boolean isLine = true;
			for(int i=-1; i<2; i++) 
			{
				for(int j=-1; j<2; j++) 
				{
					if(i != 0 || j != 0) 
					{
						int yf = row + i;
						int xf = col + j;
						if(xf < 0) continue;
						if(xf < 2*max_length) 
						{
							if (yf < 0) 
							{
								yf += hough_space;
							}
							if (yf >= hough_space) 
							{
								yf -= hough_space;
							}
							if(hough_2d[yf][xf] <= hough_value) 
							{
								continue;
							}
							isLine = false;
							break;
						}
					}
				}
			}
			if(!isLine) continue;
			// transform back to pixel data now...
			double dy = sin(row * hough_interval);
			double dx = cos(row * hough_interval);
			if ((row <= hough_space / 4) || (row >= 3 * hough_space / 4)) 
			{
				for (int subrow = 0; subrow < height; ++subrow) 
				{
					int subcol = (int)((col - max_length - ((subrow - centerY) * dy)) / dx) + centerX;
					if ((subcol < width) && (subcol >= 0)) 
					{
						imgTemp.m_pBits[0][subrow][subcol] = /*-16776961*/ 0;
					}
				}
			} else {
				for (int subcol = 0; subcol < width; ++subcol)
				{
					int subrow = (int)((col - max_length - ((subcol - centerX) * dx)) / dy) + centerY;
					if ((subrow < height) && (subrow >= 0)) 
					{
						imgTemp.m_pBits[0][subrow][subcol] = /*-16776961*/ 0;
					}
				}
			}
		}
	}

	for (int i = 0; i < 2 * hough_space * max_length; i++)
	{
		int value = SaturateCast1(((int)(scale * hough_1d[i] + offset)),0,255); // scale always equals 1
		hough_1d[i] = (0xFF000000 | value + (value << 16) + (value << 8));
	}

	// convert to image 1D and return
	for (int row = 0; row < height; row++) 
	{
		for (int col = 0; col < width; col++) 
		{
			dstImg.m_pBits[0][row][col] = imgTemp.m_pBits[0][row][col];
			dstImg.m_pBits[1][row][col] = imgTemp.m_pBits[0][row][col];
			dstImg.m_pBits[2][row][col] = imgTemp.m_pBits[0][row][col];
		}
	}
	delete[] hough_1d;
	delete[] hough_2d;
	hough_1d = NULL;
	hough_2d = NULL;
}