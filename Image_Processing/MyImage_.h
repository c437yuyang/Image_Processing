#pragma once
class MyImage_
{
	friend class CCommon;
public:
	MyImage_(void);
	MyImage_(int w,int h); ////产生黑色
	MyImage_::MyImage_(int w,int h,COLORREF color); //产生单色
	MyImage_::MyImage_(const MyImage_ &srcImg); //拷贝构造函数
	~MyImage_(void);
	//inline HRESULT MyImage_::Load(_In_z_ LPCTSTR pszFileName) throw();
	//inline BOOL  MyImage_::Draw(
	//	_In_ HDC hDestDC,
	//	_In_ int xDest,
	//	_In_ int yDest,
	//	_In_ int nDestWidth,
	//	_In_ int nDestHeight) const throw();
	//inline bool MyImage_::IsNull() const throw();
	//inline void MyImage_::Destroy() throw();
	//inline int MyImage_::GetWidth() const throw();
	//inline int MyImage_::GetHeight() const throw();


	HRESULT MyImage_::Load(_In_z_ LPCTSTR pszFileName);//加载图像，参数pszFileName是文件路径及文件名
	BOOL  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest,
		_In_ int nDestWidth,
		_In_ int nDestHeight) const;//显示图像，hDestDC是绘图设备上下文的句柄，xDest,yDest是绘图左上角起始点的坐标，
									//nDestWidth是要绘的宽度，nDestHeight是要绘的高度

	BOOL  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest) const;//显示图像，参数同上一个Draw函数;相当于上一个Draw后两个参数为图像的宽度和高度的情况

	HRESULT MyImage_::Save(
		_In_z_ LPCTSTR pszFileName,
		_In_ REFGUID guidFileType = GUID_NULL) const;//保存图像，pszFileName为路径名和文件名， guidFileType我没有了解过

	bool MyImage_::IsNull() const ;//判断图像是否为空，如是空返回0
	void MyImage_::Destroy() ;//销毁MyImage_类加载到内存中的图像
	int MyImage_::GetWidth()const;//返回图像的宽度,(这里此类型的高度和宽度都是完全由CImage类来决定的,所以CopyTo等函数中必须也要CImage类要Create一下，不然会得不到正确的高和宽)
	int MyImage_::GetHeight() const;//返回图像的高度
	void Create( _In_ int width , _In_ int height , _In_ COLORREF color);
	void MyImage_::BorderFillTo(MyImage_ &img1,UINT nFillPara) const;
	void MyImage_::BorderFillTo(MyImage_ &img1,UINT nFillPara,int nFillMode) const;
	void MyImage_::RemoveFillTo(MyImage_ &img1,UINT nFillPara) const;
	void MyImage_::CopyTo(MyImage_ &img) const;
	void MyImage_::operator=(MyImage_ &img);
	bool IsGrayed() const;
	void SetGrayed(bool flag=true);
	inline int GetChannels() const {return m_nChannelCount;}
	inline void SetChannels(int chs) { m_nChannelCount=chs;}
	BYTE*** m_pBits;//三维数组，用于保存图像的BGR像素值
private:
	int m_nChannelCount;
	bool m_bIsGrayed;
	CImage m_CImage;
};

