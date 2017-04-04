#pragma once

#include "MyImage_.h"
#include <deque>
#include <vector>
#include <iostream>
using namespace std;

class CImagesStock
{
public:
	CImagesStock(void);
	~CImagesStock(void);
	int addImage(MyImage_ &img);
	int AddImageToStock(MyImage_ &img);
	int getCurIndex() {return m_nIndexCur;}
	int getStockCount() {return m_nImageCount;}
	bool getPreImage(vector<unsigned char> &vecValues);
	bool getNextImage(vector<unsigned char> &vecValues);
	void clear() {m_queImageStock.clear();m_nIndexCur=-1;m_nImageCount=0;}
private:
	deque<MyImage_> m_queImages;
	deque<vector<unsigned char>> m_queImageStock;
	int m_nIndexCur;
	int m_nImageCount;
	int m_nStockSize;
};

