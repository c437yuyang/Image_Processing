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

//�����������⣬��Ϊ����ĸ������ݰ汾������MyImage_������
int CImagesStock::addImage(MyImage_ &img) //���봫���ã���ȻImageAfter���ݻ����why������
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
		vecValues = m_queImageStock[m_nIndexCur]; //vector��д��=�ŵ�
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


	if (m_nIndexCur!=m_nImageCount-1) //���м�λ��ִ����ӣ���ɾ���м�λ��֮��Ĵ洢
	{
		for (int i=0;i!=m_nImageCount-m_nIndexCur-1;++i)
		{
			m_queImageStock.pop_back();
			//cout << "ɾ����һ��" << endl;
		}
		m_nImageCount-=(m_nImageCount-m_nIndexCur-1);
		m_nImageCount++;
		m_nIndexCur++;
		m_queImageStock.push_back(vecValues);
	}
	else//�������
	{
		if(m_nImageCount>m_nStockSize-1) //�ﵽ�����洢��Ŀ
		{
			m_queImageStock.pop_front();
			m_queImageStock.push_back(vecValues);
		}
		else //���������δ�ﵽ�����Ŀ
		{
			m_nImageCount++;
			m_nIndexCur++;
			m_queImageStock.push_back(vecValues);
		}
	}
	//cout << m_nImageCount << "," << m_nIndexCur << endl;
	return m_nImageCount;
}