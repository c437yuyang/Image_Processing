#include "StdAfx.h"
#include "MyImage_double.h"

//一个简单的类，用来中间计算的时候用(数据为double,便于计算)
//实现了CopyTo(MyImage_)

CMyImage_double::CMyImage_double(void)
{
	m_bIsGrayed = false;
	m_pBits=NULL;
}


CMyImage_double::~CMyImage_double(void)
{
	int h = m_nHeight;
	int w = m_nWidth;
	if ( m_pBits!=NULL)
	{
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

void CMyImage_double::Create(int width,int height,UINT color)
{
	if (m_pBits!=NULL)
	{
		Destroy();
	}

	m_nWidth = width;
	m_nHeight = height;
	int w = width;
	int h = height;

	// 通道 行 列,ch,y,x
	m_pBits= (double***)new   double**[3];   
	for(int i=0;i<3;i++)  
	{ 
		m_pBits[i] = (double**)new  double*[h];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<h;   j++)         
		{   

			m_pBits[i][j]=new double[w];   
		}
	}


	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_pBits[0][j][k]=color;//B
			m_pBits[1][j][k]=color;//G
			m_pBits[2][j][k]=color;//R
		}
	}
}


CMyImage_double::CMyImage_double(MyImage_ &srcImg)
{
	m_nHeight = srcImg.GetHeight();
	m_nWidth = srcImg.GetWidth();
	m_bIsGrayed = srcImg.IsGrayed();
	// 通道 行 列,ch,y,x
	m_pBits= (double***)new   double**[3];   
	for(int i=0;i<3;i++)  
	{ 
		m_pBits[i] = (double**)new  double*[m_nHeight];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<m_nHeight;   j++)         
		{   

			m_pBits[i][j]=new double[m_nWidth];   
		}
	}


	for (int j=0;j<m_nHeight;j++)
	{
		for (int k=0;k<m_nWidth;k++)
		{
			m_pBits[0][j][k]=(double)srcImg.m_pBits[0][j][k];//B
			m_pBits[1][j][k]=(double)srcImg.m_pBits[1][j][k];//G
			m_pBits[2][j][k]=(double)srcImg.m_pBits[2][j][k];//R
		}
	}

	
}


void CMyImage_double::Destroy()
{
	if ( m_pBits!=NULL)
	{
		int h=m_nHeight;
		//for   (int   i=0;   i<2;   i++)   //comment by yxp 2016_12_11
		for (int   i=0;   i<3;   i++)
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


void CMyImage_double::CopyTo(MyImage_ &img1,bool bIsSaturateCast) const
{
	int w=m_nWidth;
	int h=m_nHeight;
	
	img1.Create(w,h,RGB(0,0,0)); //creat里面负责destroy

	if (bIsSaturateCast = true)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				img1.m_pBits[0][j][k]=(BYTE)SaturateCast(m_pBits[0][j][k],0.0,255.0);//B
				img1.m_pBits[1][j][k]=(BYTE)SaturateCast(m_pBits[1][j][k],0.0,255.0);//G
				img1.m_pBits[2][j][k]=(BYTE)SaturateCast(m_pBits[2][j][k],0.0,255.0);//R
			}
		}
	}
	else
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				img1.m_pBits[0][j][k]=(BYTE)m_pBits[0][j][k];//B
				img1.m_pBits[1][j][k]=(BYTE)m_pBits[1][j][k];//G
				img1.m_pBits[2][j][k]=(BYTE)m_pBits[2][j][k];//R
			}
		}
	}

}


void CMyImage_double::CopyTo(CMyImage_double &img1) const
{

	if (img1.m_pBits!=NULL)
	{
		img1.Destroy();
	}
	int w=m_nWidth;
	int h=m_nHeight;
	img1.m_nHeight = m_nHeight;
	img1.m_nWidth = m_nWidth;

	// 通道 行 列
	img1.Create(w,h,0);

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



void CMyImage_double::RemoveFillTo(MyImage_ &img1,UINT nFillPara,bool bIs) const 
{

	int w=m_nWidth-2*nFillPara;
	int h=m_nHeight-2*nFillPara;
	img1.Create(w,h,0);
	//原图数据进行复制
	if (bIs)
	{
		for (int j=0;j<h;j++) //行
		{
			for (int k=0;k<w;k++) //列
			{
				img1.m_pBits[0][j][k]=(BYTE)SaturateCast(m_pBits[0][j+nFillPara][k+nFillPara],0.0,255.0);//B
				img1.m_pBits[1][j][k]=(BYTE)SaturateCast(m_pBits[1][j+nFillPara][k+nFillPara],0.0,255.0);//G
				img1.m_pBits[2][j][k]=(BYTE)SaturateCast(m_pBits[2][j+nFillPara][k+nFillPara],0.0,255.0);//R
			}
		}
	}
	else
	{
		for (int j=0;j<h;j++) //行
		{
			for (int k=0;k<w;k++) //列
			{
				img1.m_pBits[0][j][k]=(BYTE)m_pBits[0][j+nFillPara][k+nFillPara];//B
				img1.m_pBits[1][j][k]=(BYTE)m_pBits[1][j+nFillPara][k+nFillPara];//G
				img1.m_pBits[2][j][k]=(BYTE)m_pBits[2][j+nFillPara][k+nFillPara];//R
			}
		}
	}

}


void CMyImage_double::RemoveFillTo(CMyImage_double &img1,UINT nFillPara) const 
{

	int w=m_nWidth-2*nFillPara;
	int h=m_nHeight-2*nFillPara;

	img1.m_nWidth = w;
	img1.m_nHeight = h;

	if (img1.m_pBits!=NULL)
	{
		img1.Destroy();
	}
	img1.Create(w,h,0);
	//原图数据进行复制

		for (int j=0;j<h;j++) //行
		{
			for (int k=0;k<w;k++) //列
			{
				img1.m_pBits[0][j][k]=m_pBits[0][j+nFillPara][k+nFillPara];//B
				img1.m_pBits[1][j][k]=m_pBits[1][j+nFillPara][k+nFillPara];//G
				img1.m_pBits[2][j][k]=m_pBits[2][j+nFillPara][k+nFillPara];//R
			}
		}

}



//填充模式0代表255填充，1代表0填充，2代表复制边界像素填充
void CMyImage_double::BorderFillTo(CMyImage_double &img1,UINT nFillPara,int FillMode) const //函数里创建，且为destroy,所以之前不要create
{
	int  N =  nFillPara;
#define FILL_BLACK 0;
#define FILL_WHITE 1;
	int w=m_nWidth+2*N;
	int h=m_nHeight+2*N;

	img1.m_bIsGrayed = m_bIsGrayed;
	img1.m_nWidth = w;
	img1.m_nHeight = h;

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




	//m_CImage的数据不用自己复制，在Draw方法里实现了
	// 通道 行 列
	img1.m_pBits = (double***)new   double**[3];   
	for(int i=0;i<3;i++)  
	{ 
		img1.m_pBits[i] = (double**)new  double*[h];   
	}   
	for   (int   i=0;   i<3;   i++)   
	{          
		for   (int   j=0;   j<h;   j++)         
		{   

			img1.m_pBits[i][j]=new double[w];   
		}
	}

	//原图数据进行复制
	for (int j=N;j<h-N;j++) //行
	{
		for (int k=N;k<w-N;k++) //列
		{
			//img.m_pBits[0][j][k]=psrc[j*nrow+k*3];//B
			//img.m_pBits[1][j][k]=psrc[j*nrow+k*3+1];//G
			//img.m_pBits[2][j][k]=psrc[j*nrow+k*3+2];//R
			img1.m_pBits[0][j][k]=m_pBits[0][j-N][k-N];//B
			img1.m_pBits[1][j][k]=m_pBits[1][j-N][k-N];//G
			img1.m_pBits[2][j][k]=m_pBits[2][j-N][k-N];//R
		}
	}

	//对边缘进行处理

	//上下边缘
	//其实底下不用填充若默认为0的话，因为新数组里面默认就是0
	for (int k=0;k!=N;++k) //行
	{
		for (int i=0;i!=w;++i)//列
		{
			for (int j=0;j!=3;++j)
			{
				if (nFillColor == 0 || nFillColor ==255)
				{
					img1.m_pBits[j][k][i] = nFillColor;//上边缘
					img1.m_pBits[j][h-k-N][i] = nFillColor;//下边缘
				}
				else //nFillMode 在复制边缘像素的时候设为-1
				{
					if (i >=N && i<= w-1-N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][i-N];//上边缘
						img1.m_pBits[j][h-1-k][i] = m_pBits[j][h-2*N-1][i-N];//下边缘
					} 
					else if (i<N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][0];//上边缘
						img1.m_pBits[j][h-k-1][i] = m_pBits[j][h-2*N-1][0];//下边缘
					}
					else if (i>w-1-N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][w-2*N-1];//上边缘
						img1.m_pBits[j][h-k-1][i] = m_pBits[j][h-2*N-1][w-2*N-1];//下边缘
					}
					else{

					}
				}

			}
		}
	}

	//边缘的填充必须在这里面处理才行
	for (int k=0;k!=N;++k) //列
	{
		for (int i=N;i!=h-N;++i) //行
		{
			for(int j=0;j!=3;++j) 
			{

				if (nFillColor == 0 || nFillColor ==255)
				{
					img1.m_pBits[j][i][k] = nFillColor;//左边缘
					img1.m_pBits[j][i][w-k-1] = nFillColor;//右边缘
				}
				else //nFillMode 在复制边缘像素的时候设为-1
				{
					img1.m_pBits[j][i][k] = m_pBits[j][i-N][0]; //左边缘填充设为复制第一列像素
					img1.m_pBits[j][i][w-k-1] = m_pBits[j][i-N][w-2*N-1];  //底边缘填充设为复制最后一列像素
				}

			}
		}
	}
}