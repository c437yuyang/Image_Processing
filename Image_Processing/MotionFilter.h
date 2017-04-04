#pragma once

#include "MyImage_double.h"
#include "Common.h"
class CMotionFilter
{
public:
	CMotionFilter(void);
	~CMotionFilter(void);

public:
	double m_dDistance;
	double m_dAngle;
	double m_dZoom;

	void doMotionFilter(CMyImage_double &srcImg,CMyImage_double &dstImg);

};

