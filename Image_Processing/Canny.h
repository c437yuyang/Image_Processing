#pragma once

#include "MyImage_.h"
#include "MyImage_double.h"

class CCanny
{
public:
	CCanny(void);
	~CCanny(void);
	void doCanny(MyImage_ &srcImg,MyImage_ &dstImg,int nLowThresh,int nHighThresh);
};

