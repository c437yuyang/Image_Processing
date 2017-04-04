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
	int nrow=m_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	m_nChannelCount = m_CImage.GetBPP()/8;
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//获得m_CImage最后一行的像素地址
	/*创建三维数组并将m_CImage复制进三维数组*/
	// 通道 行 列
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

	/*将m_CImage复制进三维数组*/
	//无论这里是几个通道，之后的ImageAfter里都是三通道的，方便处理
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
	/*创建三维数组并将m_CImage复制进三维数组*/
	return( S_OK );//为了与CImge类的Load函数返回值相同
}

void MyImage_::Create(
	_In_ int width ,
	_In_ int height ,
	_In_ COLORREF color)
{
	if (!IsNull()) Destroy();

	int w = width;
	int h = height;

	// 通道 行 列,ch,y,x
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
	m_CImage.Create(w,h,24); //创建三通道图片
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
	int nrow=m_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//获得m_CImage最后一行的像素地址

	/*将三维数组复制进m_CImage*/
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
				psrc[j*nrow+k]=m_pBits[0][j][k];//这里有问题，不知道为什么单通道的时候,读入数据正确，显示也正确，但是之后处理后了，数据也是正确的，但是显示出来就是全黑(全0)，最后是通过让处理后的图(ImageAfter)强行转换为3通道的图解决的
			}
		}
	}
	/*将三维数组复制进m_CImage*/

	return m_CImage.Draw(hDestDC,xDest, yDest,nDestWidth, nDestHeight);//调用CImage的Draw;
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
	int nrow=m_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//获得m_CImage最后一行的像素地址

	/*将三维数组复制进m_CImage*/
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
				psrc[j*nrow+k]=m_pBits[0][j][k];//这里有问题，不知道为什么单通道的时候,读入数据正确，显示也正确，但是之后处理后了，数据也是正确的，但是显示出来就是全黑(全0)，最后是通过让处理后的图(ImageAfter)强行转换为3通道的图解决的
			}
		}
	}
	/*将三维数组复制进m_CImage*/
	return m_CImage.Draw(hDestDC,xDest,yDest);
}

HRESULT MyImage_::Save(
	_In_z_ LPCTSTR pszFileName,
	_In_ REFGUID guidFileType) const
{
	if(m_CImage.IsNull()) return 0;
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//获得m_CImage最后一行的像素地址
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
	img1.SetChannels(3); //调用此方法肯定不是原图(M_image)，因此直接让位深为24，通道为3，（此方法主要是为了解决单通道显示出错，不知道为什么）
	int w=GetWidth();
	int h=GetHeight();
	//printf("%d",m_CImage.GetBPP());
	/*img1.m_CImage.Create(w,h,m_CImage.GetBPP());*/ //这里一定要创建一下，不然后面的getWidth和height会出错
	img1.m_CImage.Create(w,h,24);
	/*创建三维数组并将m_CImage复制进三维数组*/

	// 通道 行 列
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


	//复制图像数据数组

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
	//m_CImage的数据不用自己复制，在Draw方法里实现了
	// 通道 行 列
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

	/*将m_CImage复制进三维数组*/

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
	//其实底下不用填充若默认为0的话，因为新数组里面默认就是0
	for (int k=0;k!=N;++k)
	{
		for (int i=0;i!=w;++i)
		{
			for (int j=0;j!=3;++j)
			{
				img1.m_pBits[j][N][i] = 0;//第一行
				img1.m_pBits[j][h-1-N][i] = 0;//最后一行
			}
		}
	}

	for (int k=0;k!=N;++k) 
	{
		for (int i=N;i!=h-N;++i)
		{
			for(int j=0;j!=3;++j)
			{
				img1.m_pBits[j][i][k] = 0;//左边缘
				img1.m_pBits[j][i][w-k-1] = 0;//右边缘
			}
		}
	}

}

//填充模式0代表255填充，1代表0填充，2代表复制边界像素填充
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
	//m_CImage的数据不用自己复制，在Draw方法里实现了
	// 通道 行 列
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

	/*将m_CImage复制进三维数组*/

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
						std::cout << "else" << std::endl;
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



void MyImage_::RemoveFillTo(MyImage_ &img1,UINT nFillPara) const
{
	if (!img1.IsNull()) img1.Destroy();
	img1.SetGrayed(m_bIsGrayed);

	int w=GetWidth()-2*nFillPara;
	int h=GetHeight()-2*nFillPara;
	img1.m_CImage.Create(w,h,m_CImage.GetBPP());
	//m_CImage的数据不用自己复制，在Draw方法里实现了
	// 通道 行 列
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

	/*将m_CImage复制进三维数组*/

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


void MyImage_::operator=(MyImage_ &img)
{
	img.CopyTo(*this);
}