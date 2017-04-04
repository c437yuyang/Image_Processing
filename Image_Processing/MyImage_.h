#pragma once
class MyImage_
{
	friend class CCommon;
public:
	MyImage_(void);
	MyImage_(int w,int h); ////������ɫ
	MyImage_::MyImage_(int w,int h,COLORREF color); //������ɫ
	MyImage_::MyImage_(const MyImage_ &srcImg); //�������캯��
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


	HRESULT MyImage_::Load(_In_z_ LPCTSTR pszFileName);//����ͼ�񣬲���pszFileName���ļ�·�����ļ���
	BOOL  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest,
		_In_ int nDestWidth,
		_In_ int nDestHeight) const;//��ʾͼ��hDestDC�ǻ�ͼ�豸�����ĵľ����xDest,yDest�ǻ�ͼ���Ͻ���ʼ������꣬
									//nDestWidth��Ҫ��Ŀ�ȣ�nDestHeight��Ҫ��ĸ߶�

	BOOL  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest) const;//��ʾͼ�񣬲���ͬ��һ��Draw����;�൱����һ��Draw����������Ϊͼ��Ŀ�Ⱥ͸߶ȵ����

	HRESULT MyImage_::Save(
		_In_z_ LPCTSTR pszFileName,
		_In_ REFGUID guidFileType = GUID_NULL) const;//����ͼ��pszFileNameΪ·�������ļ����� guidFileType��û���˽��

	bool MyImage_::IsNull() const ;//�ж�ͼ���Ƿ�Ϊ�գ����ǿշ���0
	void MyImage_::Destroy() ;//����MyImage_����ص��ڴ��е�ͼ��
	int MyImage_::GetWidth()const;//����ͼ��Ŀ��,(��������͵ĸ߶ȺͿ�ȶ�����ȫ��CImage����������,����CopyTo�Ⱥ����б���ҲҪCImage��ҪCreateһ�£���Ȼ��ò�����ȷ�ĸߺͿ�)
	int MyImage_::GetHeight() const;//����ͼ��ĸ߶�
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
	BYTE*** m_pBits;//��ά���飬���ڱ���ͼ���BGR����ֵ
private:
	int m_nChannelCount;
	bool m_bIsGrayed;
	CImage m_CImage;
};

