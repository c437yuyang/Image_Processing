#include "StdAfx.h"
#include "MyImage_.h"
#include <iostream>

MyImage_::MyImage_(void)
{
	m_bIsGrayed = false;
	m_pBits=NULL;
}


MyImage_::MyImage_(int w,int h)
{
	m_bIsGrayed = false;
	m_pBits=NULL;
	Create(w,h,RGB(0,0,0));
}

MyImage_::MyImage_(int w,int h,COLORREF color)
{
	m_bIsGrayed = false;
	m_pBits=NULL;
	Create(w,h,color);
}

MyImage_::MyImage_(const MyImage_ &img1)
{
	m_pBits=NULL;
	img1.CopyTo(*this);
}

bool MyImage_::IsGrayed() const
{
	return m_bIsGrayed;
}

void MyImage_::SetGrayed(bool flag)
{
	m_bIsGrayed=flag;
}

MyImage_::~MyImage_(void)
{

	if ( m_pBits!=NULL)
	{
		int h=m_CImage.GetHeight();
		//for   (int   i=0;   i<2;   i++)   //comment by yxp 2016_12_11
		for   (int   i=0;   i<3;   i++)   //three dimension
		{          
			for   (int   j=0;   j<h;   j++)         
			{   

				delete[] m_pBits[i][j];   
				m_pBits[i][j]=NULL;
			}
		}
		for (int   i=0;   i<3;   i++)
		{
			delete[] m_pBits[i];  
			m_pBits[i]=NULL;
		}
		delete[]  m_pBits;
		m_pBits=NULL;
	}
}

HRESULT MyImage_::Load(_In_z_ LPCTSTR pszFileName) 
{
	m_CImage.Load(pszFileName);
	if(m_CImage.IsNull()) return 0;
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	m_nChannelCount = m_CImage.GetBPP()/8;
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//���m_CImage���һ�е����ص�ַ
	/*������ά���鲢��m_CImage���ƽ���ά����*/
	// ͨ�� �� ��
	m_pBits= (BYTE***)new   BYTE**[3];   
	for(int i=0;i<3;i++)  
	{ 
		m_pBits[i] = (BYTE**)new  BYTE*[h];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<h;   j++)         
		{   

			m_pBits[i][j]=new BYTE[w];   
		}
	}

	/*��m_CImage���ƽ���ά����*/
	//���������Ǽ���ͨ����֮���ImageAfter�ﶼ����ͨ���ģ����㴦��
	if (GetChannels() == 3)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				m_pBits[0][j][k]=psrc[j*nrow+k*3];//B
				m_pBits[1][j][k]=psrc[j*nrow+k*3+1];//G
				m_pBits[2][j][k]=psrc[j*nrow+k*3+2];//R
			}
		}
	}
	else if (GetChannels()==1)
	{
		m_bIsGrayed = true;
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				m_pBits[0][j][k]=psrc[j*nrow+k];//B
				m_pBits[1][j][k]=psrc[j*nrow+k];//G
				m_pBits[2][j][k]=psrc[j*nrow+k];//R
			}
		}
	}
	else if (GetChannels()==4)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				m_pBits[0][j][k]=psrc[j*nrow+k*4];//B
				m_pBits[1][j][k]=psrc[j*nrow+k*4+1];//G
				m_pBits[2][j][k]=psrc[j*nrow+k*4+2];//R
			}
		}
	}
	/*������ά���鲢��m_CImage���ƽ���ά����*/
	return( S_OK );//Ϊ����CImge���Load��������ֵ��ͬ
}

void MyImage_::Create(
	_In_ int width ,
	_In_ int height ,
	_In_ COLORREF color)
{
	if (!IsNull()) Destroy();

	int w = width;
	int h = height;

	// ͨ�� �� ��,ch,y,x
	m_pBits= (BYTE***)new   BYTE**[3];   
	for(int i=0;i<3;i++)  
	{ 
		m_pBits[i] = (BYTE**)new  BYTE*[h];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<h;   j++)         
		{   

			m_pBits[i][j]=new BYTE[w];   
		}
	}


	int nCb = (color & 0x00ff0000)>>16;
	int nCg = (color & 0x0000ff00)>>8;
	int nCr = color & 0x000000ff;

	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_pBits[0][j][k]=nCb;//B
			m_pBits[1][j][k]=nCg;//G
			m_pBits[2][j][k]=nCr;//R
		}
	}
	m_CImage.Create(w,h,24); //������ͨ��ͼƬ
}

// HRESULT MyImage_::Load(_In_z_ LPCTSTR pszFileName) 
//{
//	m_CImage.Load(pszFileName);
//
//}

BOOL  MyImage_::Draw(
	_In_ HDC hDestDC,
	_In_ int xDest,
	_In_ int yDest,
	_In_ int nDestWidth,
	_In_ int nDestHeight) const 
{
	if(m_CImage.IsNull()) return 0;
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//���m_CImage���һ�е����ص�ַ

	/*����ά���鸴�ƽ�m_CImage*/
	int ch = GetChannels();
	if (GetChannels()==3)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				psrc[j*nrow+k*3]=m_pBits[0][j][k];//B
				psrc[j*nrow+k*3+1]=m_pBits[1][j][k];//G
				psrc[j*nrow+k*3+2]=m_pBits[2][j][k];//R
			}
		}
	}
	else if(GetChannels()==1)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				psrc[j*nrow+k]=m_pBits[0][j][k];//���������⣬��֪��Ϊʲô��ͨ����ʱ��,����������ȷ����ʾҲ��ȷ������֮������ˣ�����Ҳ����ȷ�ģ�������ʾ��������ȫ��(ȫ0)�������ͨ���ô�����ͼ(ImageAfter)ǿ��ת��Ϊ3ͨ����ͼ�����
			}
		}
	}
	/*����ά���鸴�ƽ�m_CImage*/

	return m_CImage.Draw(hDestDC,xDest, yDest,nDestWidth, nDestHeight);//����CImage��Draw;
}

bool MyImage_::IsNull() const 
{
	//return (m_CImage.IsNull()); //comment by yxp 2016_12_11
	return (m_CImage.IsNull() && m_pBits==NULL);
}

void MyImage_::Destroy() 
{
	if ( m_pBits!=NULL)
	{
		int h=m_CImage.GetHeight();
		//for   (int   i=0;   i<2;   i++)   //comment by yxp 2016_12_11
		for   (int   i=0;   i<3;   i++)   
		{          
			for   (int   j=0;   j<h;   j++)         
			{   

				delete[] m_pBits[i][j];   
				m_pBits[i][j]=NULL;
			}
		}
		for (int   i=0;   i<3;   i++)
		{
			delete[] m_pBits[i];  
			m_pBits[i]=NULL;
		}
		delete[]  m_pBits;
		m_pBits=NULL;
	}
	m_CImage.Destroy();
}

int MyImage_::GetWidth() const 
{
	return m_CImage.GetWidth();
}
int MyImage_::GetHeight() const
{
	return m_CImage.GetHeight();
}


BOOL  MyImage_::Draw(
	_In_ HDC hDestDC,
	_In_ int xDest,
	_In_ int yDest) const
{
	if(m_CImage.IsNull()) return 0;
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//���m_CImage���һ�е����ص�ַ

	/*����ά���鸴�ƽ�m_CImage*/
	int ch = GetChannels();
	if (GetChannels()==3)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				psrc[j*nrow+k*3]=m_pBits[0][j][k];//B
				psrc[j*nrow+k*3+1]=m_pBits[1][j][k];//G
				psrc[j*nrow+k*3+2]=m_pBits[2][j][k];//R
			}
		}
	}
	else if(GetChannels()==1)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				psrc[j*nrow+k]=m_pBits[0][j][k];//���������⣬��֪��Ϊʲô��ͨ����ʱ��,����������ȷ����ʾҲ��ȷ������֮������ˣ�����Ҳ����ȷ�ģ�������ʾ��������ȫ��(ȫ0)�������ͨ���ô�����ͼ(ImageAfter)ǿ��ת��Ϊ3ͨ����ͼ�����
			}
		}
	}
	/*����ά���鸴�ƽ�m_CImage*/
	return m_CImage.Draw(hDestDC,xDest,yDest);
}

HRESULT MyImage_::Save(
	_In_z_ LPCTSTR pszFileName,
	_In_ REFGUID guidFileType) const
{
	if(m_CImage.IsNull()) return 0;
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//���m_CImage���һ�е����ص�ַ
	for (int j=0;j<h;j++)
	{
	
		for (int k=0;k<w;k++)
		{
			psrc[j*nrow+k*3]=m_pBits[0][j][k];//B
			psrc[j*nrow+k*3+1]=m_pBits[1][j][k];//G
			psrc[j*nrow+k*3+2]=m_pBits[2][j][k];//R
		}
	}
	return m_CImage.Save(pszFileName,guidFileType);

}


void MyImage_::CopyTo(MyImage_ &img1) const
{
	if (!img1.IsNull()) 
		img1.Destroy();
	
	img1.SetGrayed(m_bIsGrayed);
	img1.SetChannels(3); //���ô˷����϶�����ԭͼ(M_image)�����ֱ����λ��Ϊ24��ͨ��Ϊ3�����˷�����Ҫ��Ϊ�˽����ͨ����ʾ������֪��Ϊʲô��
	int w=GetWidth();
	int h=GetHeight();
	//printf("%d",m_CImage.GetBPP());
	/*img1.m_CImage.Create(w,h,m_CImage.GetBPP());*/ //����һ��Ҫ����һ�£���Ȼ�����getWidth��height�����
	img1.m_CImage.Create(w,h,24);
	/*������ά���鲢��m_CImage���ƽ���ά����*/

	// ͨ�� �� ��
	img1.m_pBits = (BYTE***)new   BYTE**[3];   
	for(int i=0;i<3;i++)  
	{ 
		img1.m_pBits[i] = (BYTE**)new  BYTE*[h];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<h;   j++)         
		{   

			img1.m_pBits[i][j]=new BYTE[w];   
		}
	}


	//����ͼ����������

	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{

			img1.m_pBits[0][j][k]=m_pBits[0][j][k];//B
			img1.m_pBits[1][j][k]=m_pBits[1][j][k];//G
			img1.m_pBits[2][j][k]=m_pBits[2][j][k];//R
		}
	}

}

void MyImage_::BorderFillTo(MyImage_ &img1,UINT nFillPara) const
{
	int N = nFillPara;
	if (!img1.IsNull()) img1.Destroy();
	img1.SetGrayed(m_bIsGrayed);

	int w=GetWidth()+2*N;
	int h=GetHeight()+2*N;
	img1.m_CImage.Create(w,h,m_CImage.GetBPP());
	//m_CImage�����ݲ����Լ����ƣ���Draw������ʵ����
	// ͨ�� �� ��
	img1.m_pBits = (BYTE***)new   BYTE**[3];   
	for(int i=0;i<3;i++)  
	{ 
		img1.m_pBits[i] = (BYTE**)new  BYTE*[h];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<h;   j++)         
		{   
			img1.m_pBits[i][j]=new BYTE[w];   
		}
	}

	/*��m_CImage���ƽ���ά����*/

	//ԭͼ���ݽ��и���
	for (int j=N;j<h-N;j++) //��
	{
		for (int k=N;k<w-N;k++) //��
		{
			//img.m_pBits[0][j][k]=psrc[j*nrow+k*3];//B
			//img.m_pBits[1][j][k]=psrc[j*nrow+k*3+1];//G
			//img.m_pBits[2][j][k]=psrc[j*nrow+k*3+2];//R
			img1.m_pBits[0][j][k]=m_pBits[0][j-N][k-N];//B
			img1.m_pBits[1][j][k]=m_pBits[1][j-N][k-N];//G
			img1.m_pBits[2][j][k]=m_pBits[2][j-N][k-N];//R
		}
	}

	//�Ա�Ե���д���
	//��ʵ���²��������Ĭ��Ϊ0�Ļ�����Ϊ����������Ĭ�Ͼ���0
	for (int k=0;k!=N;++k)
	{
		for (int i=0;i!=w;++i)
		{
			for (int j=0;j!=3;++j)
			{
				img1.m_pBits[j][N][i] = 0;//��һ��
				img1.m_pBits[j][h-1-N][i] = 0;//���һ��
			}
		}
	}

	for (int k=0;k!=N;++k) 
	{
		for (int i=N;i!=h-N;++i)
		{
			for(int j=0;j!=3;++j)
			{
				img1.m_pBits[j][i][k] = 0;//���Ե
				img1.m_pBits[j][i][w-k-1] = 0;//�ұ�Ե
			}
		}
	}

}

//���ģʽ0����255��䣬1����0��䣬2�����Ʊ߽��������
void MyImage_::BorderFillTo(MyImage_ &img1,UINT nFillPara,int FillMode) const
{
	int  N =  nFillPara;
#define FILL_BLACK 0;
#define FILL_WHITE 1;
	if (!img1.IsNull()) img1.Destroy();
	img1.SetGrayed(m_bIsGrayed);

	int nFillColor = 0;
	if (FillMode==0)
	{
		nFillColor = 255;
	}else if(FillMode ==1)
	{
		nFillColor = 0;
	}else // ==2
	{
		//std::cout << FillMode <<std::endl;
		nFillColor = -1;
	}


	int w=GetWidth()+2*N;
	int h=GetHeight()+2*N;
	img1.m_CImage.Create(w,h,m_CImage.GetBPP());
	//m_CImage�����ݲ����Լ����ƣ���Draw������ʵ����
	// ͨ�� �� ��
	img1.m_pBits = (BYTE***)new   BYTE**[3];   
	for(int i=0;i<3;i++)  
	{ 
		img1.m_pBits[i] = (BYTE**)new  BYTE*[h];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<h;   j++)         
		{   

			img1.m_pBits[i][j]=new BYTE[w];   
		}
	}

	/*��m_CImage���ƽ���ά����*/

	//ԭͼ���ݽ��и���
	for (int j=N;j<h-N;j++) //��
	{
		for (int k=N;k<w-N;k++) //��
		{
			//img.m_pBits[0][j][k]=psrc[j*nrow+k*3];//B
			//img.m_pBits[1][j][k]=psrc[j*nrow+k*3+1];//G
			//img.m_pBits[2][j][k]=psrc[j*nrow+k*3+2];//R
			img1.m_pBits[0][j][k]=m_pBits[0][j-N][k-N];//B
			img1.m_pBits[1][j][k]=m_pBits[1][j-N][k-N];//G
			img1.m_pBits[2][j][k]=m_pBits[2][j-N][k-N];//R
		}
	}

	//�Ա�Ե���д���

	//���±�Ե
	//��ʵ���²��������Ĭ��Ϊ0�Ļ�����Ϊ����������Ĭ�Ͼ���0
	for (int k=0;k!=N;++k) //��
	{
		for (int i=0;i!=w;++i)//��
		{
			for (int j=0;j!=3;++j)
			{
				if (nFillColor == 0 || nFillColor ==255)
				{
					img1.m_pBits[j][k][i] = nFillColor;//�ϱ�Ե
					img1.m_pBits[j][h-k-N][i] = nFillColor;//�±�Ե
				}
				else //nFillMode �ڸ��Ʊ�Ե���ص�ʱ����Ϊ-1
				{
					if (i >=N && i<= w-1-N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][i-N];//�ϱ�Ե
						img1.m_pBits[j][h-1-k][i] = m_pBits[j][h-2*N-1][i-N];//�±�Ե
					} 
					else if (i<N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][0];//�ϱ�Ե
						img1.m_pBits[j][h-k-1][i] = m_pBits[j][h-2*N-1][0];//�±�Ե
					}
					else if (i>w-1-N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][w-2*N-1];//�ϱ�Ե
						img1.m_pBits[j][h-k-1][i] = m_pBits[j][h-2*N-1][w-2*N-1];//�±�Ե
					}
					else{
						std::cout << "else" << std::endl;
					}
				}

			}
		}
	}

	//��Ե���������������洦�����
	for (int k=0;k!=N;++k) //��
	{
		for (int i=N;i!=h-N;++i) //��
		{
			for(int j=0;j!=3;++j) 
			{
				
				if (nFillColor == 0 || nFillColor ==255)
				{
					img1.m_pBits[j][i][k] = nFillColor;//���Ե
					img1.m_pBits[j][i][w-k-1] = nFillColor;//�ұ�Ե
				}
				else //nFillMode �ڸ��Ʊ�Ե���ص�ʱ����Ϊ-1
				{
					img1.m_pBits[j][i][k] = m_pBits[j][i-N][0]; //���Ե�����Ϊ���Ƶ�һ������
					img1.m_pBits[j][i][w-k-1] = m_pBits[j][i-N][w-2*N-1];  //�ױ�Ե�����Ϊ�������һ������
				}
				
			}
		}
	}

}



void MyImage_::RemoveFillTo(MyImage_ &img1,UINT nFillPara) const
{
	if (!img1.IsNull()) img1.Destroy();
	img1.SetGrayed(m_bIsGrayed);

	int w=GetWidth()-2*nFillPara;
	int h=GetHeight()-2*nFillPara;
	img1.m_CImage.Create(w,h,m_CImage.GetBPP());
	//m_CImage�����ݲ����Լ����ƣ���Draw������ʵ����
	// ͨ�� �� ��
	img1.m_pBits = (BYTE***)new   BYTE**[3];   
	for(int i=0;i<3;i++)  
	{ 
		img1.m_pBits[i] = (BYTE**)new  BYTE*[h];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<h;   j++)         
		{   

			img1.m_pBits[i][j]=new BYTE[w];   
		}
	}

	/*��m_CImage���ƽ���ά����*/

	//ԭͼ���ݽ��и���
	for (int j=0;j<h;j++) //��
	{
		for (int k=0;k<w;k++) //��
		{
			img1.m_pBits[0][j][k]=m_pBits[0][j+nFillPara][k+nFillPara];//B
			img1.m_pBits[1][j][k]=m_pBits[1][j+nFillPara][k+nFillPara];//G
			img1.m_pBits[2][j][k]=m_pBits[2][j+nFillPara][k+nFillPara];//R
		}
	}
}


void MyImage_::operator=(MyImage_ &img)
{
	img.CopyTo(*this);
}