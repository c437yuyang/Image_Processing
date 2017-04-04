#include "StdAfx.h"
#include "ImagesStock.h"


CImagesStock::CImagesStock(void)
{
	m_nStockSize = 10;
	m_nImageCount = 0;
	m_nIndexCur = -1;
}


CImagesStock::~CImagesStock(void)
{
}

//废弃，有问题，改为下面的复制数据版本，不传MyImage_参数了
int CImagesStock::addImage(MyImage_ &img) //必须传引用，不然ImageAfter数据会出错，why？？？
{
	MyImage_ imgTemp;
	img.CopyTo(imgTemp);

	if(m_nImageCount>m_nStockSize-1)
	{
		m_queImages.pop_front();
		m_queImages.push_back(imgTemp);
	}
	else
	{
		m_nImageCount++;
		m_nIndexCur++;
		m_queImages.push_back(imgTemp);
	}

	return m_nImageCount;
}

bool CImagesStock::getPreImage(vector<unsigned char> &vecValues)
{
	if (m_nIndexCur<=m_nImageCount && m_nIndexCur>0)
	{
		//cout << "1" << endl;
		m_nIndexCur--;
		vecValues = m_queImageStock[m_nIndexCur]; //vector改写了=号的
		return true;
	}
	else 
	{
		//cout << m_nImageCount << m_nIndexCur << endl;
		//cout << "2" << endl;
		return false;
	}
		
}

bool CImagesStock::getNextImage(vector<unsigned char> &vecValues)
{
	if (m_nIndexCur<m_nImageCount-1)
	{
		m_nIndexCur++;
		vecValues = m_queImageStock[m_nIndexCur];
		return true;
	}
	else
		return false;
	
}

int CImagesStock::AddImageToStock(MyImage_ &img)
{
	vector<unsigned char> vecValues;
	for (int i=0;i!=img.GetHeight();++i)
	{
		for (int j=0;j!=img.GetWidth();++j)
		{
			vecValues.push_back(img.m_pBits[0][i][j]);
			vecValues.push_back(img.m_pBits[1][i][j]);
			vecValues.push_back(img.m_pBits[2][i][j]);
		}
	}


	if (m_nIndexCur!=m_nImageCount-1) //在中间位置执行添加，先删除中间位置之后的存储
	{
		for (int i=0;i!=m_nImageCount-m_nIndexCur-1;++i)
		{
			m_queImageStock.pop_back();
			//cout << "删除了一次" << endl;
		}
		m_nImageCount-=(m_nImageCount-m_nIndexCur-1);
		m_nImageCount++;
		m_nIndexCur++;
		m_queImageStock.push_back(vecValues);
	}
	else//常规添加
	{
		if(m_nImageCount>m_nStockSize-1) //达到了最大存储数目
		{
			m_queImageStock.pop_front();
			m_queImageStock.push_back(vecValues);
		}
		else //常规添加且未达到最大数目
		{
			m_nImageCount++;
			m_nIndexCur++;
			m_queImageStock.push_back(vecValues);
		}
	}
	//cout << m_nImageCount << "," << m_nIndexCur << endl;
	return m_nImageCount;
}