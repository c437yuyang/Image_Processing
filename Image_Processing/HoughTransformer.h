#pragma once

#include "Common.h"
#include "MyImage_double.h"
#include "MyImage_.h"


class HoughTransformer
{
public:
	HoughTransformer(void);
	~HoughTransformer(void);
	void HoughTransformer::HoughTransform(const MyImage_ &srcImg,MyImage_ &dstImg);

private:
	int hough_space;
	int width;
	int height;
	int *hough_1d;
	int **hough_2d;
	double threshold;
	double scale;
	double offset;


};

