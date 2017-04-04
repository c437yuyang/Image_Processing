
// Image_ProcessingView.cpp : CImage_ProcessingView 类的实现
//

#include "stdafx.h"

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Image_Processing.h"
#endif

#include "Image_ProcessingDoc.h"
#include "Image_ProcessingView.h"
#include "MainFrm.h"

using std::cout;
using std::endl;
using std::vector;
using std::map;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//std::ofstream ofs("log.txt",std::ofstream::out);
/*------------自己定义的一系列函数---------------*/

UINT myPow(UINT x, UINT y); //求次方

template <class T> //返回vector最大值
T maxVecValue(vector<T> vecT);

template <class T>
T countVecAverage(vector<T> &vecT);

template <class T> //vector冒泡排序
void vecSort(vector<T> &vecT);

UINT getMapMid(map<UINT, UINT> &nMap); //返回map->first的中值
BYTE getMap2rdMax1st(map<BYTE, double> &dmap); //返回map->second最大的first值

template<class T>
void printMap(map<T, T> &nMap); //打印一个map

UINT getMapLowerCount(map<UINT, UINT> &nMapHist, UINT nMidValue); //返回map->second小于nMidValue的数目

#define random(x) (rand()%x) //自定义的取随机数函数

template<class T> //自定义溢出处理函数
T SaturateCast(T value, const T minT = 0, const T maxT = 255);

template <class T>//自定义归一化函数
void VecNormalized(vector<T> &vecT, T normMin, T normMax);


int GetDistance(int R, int G, int B, int RTaget, int GTaget, int BTaget);//自定义计算像素颜色距离函数

//int round(double value); //vs2015版本的math库自带了round函数不需要重新自定义了

// CImage_ProcessingView

IMPLEMENT_DYNCREATE(CImage_ProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImage_ProcessingView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImage_ProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CImage_ProcessingView::OnFileOpen)
	ON_COMMAND(IDM_DRAWLINE, &CImage_ProcessingView::OnDrawline)
	ON_COMMAND(ID_FILE_SAVE_AS, &CImage_ProcessingView::OnFileSaveAs)
	ON_COMMAND(IDM_SHOWRED, &CImage_ProcessingView::OnShowred)
	//	ON_COMMAND(ID_VIEW_TOOLBAR, &CImage_ProcessingView::OnViewToolbar)
	ON_COMMAND(ID_DoubleImage, &CImage_ProcessingView::OnDoubleimage)
	ON_COMMAND(ID_REVERSE, &CImage_ProcessingView::OnReverse)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOGRAY, &CImage_ProcessingView::OnTogray)
	ON_COMMAND(ID_RESDWON, &CImage_ProcessingView::OnResdwon)
	ON_COMMAND(ID_RETRIEVE, &CImage_ProcessingView::OnRetrieve)
	ON_COMMAND(ID_TEST, &CImage_ProcessingView::OnTest)
	ON_COMMAND(ID_GrayToHalf, &CImage_ProcessingView::OnGraytohalf)
	ON_COMMAND(ID_GRAY2, &CImage_ProcessingView::OnGray2)
	ON_COMMAND(ID_GRAY4, &CImage_ProcessingView::OnGray4)
	ON_COMMAND(ID_Gray64, &CImage_ProcessingView::OnGray64)
	ON_COMMAND(ID_GRAY128, &CImage_ProcessingView::OnGray128)
	ON_COMMAND(ID_GRAY32, &CImage_ProcessingView::OnGray32)
	ON_COMMAND(ID_GRAY16, &CImage_ProcessingView::OnGray16)
	ON_COMMAND(ID_GRAY8, &CImage_ProcessingView::OnGray8)
	ON_UPDATE_COMMAND_UI(ID_GRAY2, &CImage_ProcessingView::OnUpdateGray2)
	ON_UPDATE_COMMAND_UI(ID_GRAY4, &CImage_ProcessingView::OnUpdateGray4)
	ON_UPDATE_COMMAND_UI(ID_GRAY8, &CImage_ProcessingView::OnUpdateGray8)
	ON_UPDATE_COMMAND_UI(ID_GRAY16, &CImage_ProcessingView::OnUpdateGray16)
	ON_UPDATE_COMMAND_UI(ID_GRAY32, &CImage_ProcessingView::OnUpdateGray32)
	ON_UPDATE_COMMAND_UI(ID_Gray64, &CImage_ProcessingView::OnUpdateGray64)
	ON_UPDATE_COMMAND_UI(ID_GRAY128, &CImage_ProcessingView::OnUpdateGray128)
	ON_UPDATE_COMMAND_UI(ID_TOGRAY, &CImage_ProcessingView::OnUpdateTogray)
	ON_COMMAND(ID_NOISE_SALT, &CImage_ProcessingView::OnNoiseSalt)
	ON_COMMAND(ID_NOISE_SET, &CImage_ProcessingView::OnNoiseSet)
	ON_COMMAND(ID_BIT1, &CImage_ProcessingView::OnBit1)
	ON_COMMAND(ID_BIT2, &CImage_ProcessingView::OnBit2)
	ON_COMMAND(ID_BIT3, &CImage_ProcessingView::OnBit3)
	ON_COMMAND(ID_BIT4, &CImage_ProcessingView::OnBit4)
	ON_COMMAND(ID_BIT5, &CImage_ProcessingView::OnBit5)
	ON_COMMAND(ID_BIT6, &CImage_ProcessingView::OnBit6)
	ON_COMMAND(ID_BIT7, &CImage_ProcessingView::OnBit7)
	ON_COMMAND(ID_BIT8, &CImage_ProcessingView::OnBit8)
	ON_COMMAND(ID_HIST_SHOW, &CImage_ProcessingView::OnHistShow)
	ON_COMMAND(ID_FILTER_AVG, &CImage_ProcessingView::OnFilterAvg)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_FILTER_MEDIUM, &CImage_ProcessingView::OnFilterMedium)
	ON_COMMAND(ID_FtMid_Set, &CImage_ProcessingView::OnFtmidSet)
	ON_COMMAND(ID_FtAvg_Set, &CImage_ProcessingView::OnFtavgSet)
	ON_COMMAND(ID_HIST_EQ_RGB, &CImage_ProcessingView::OnHistEqRGB)
	ON_COMMAND(ID_CALC_ENTROPY, &CImage_ProcessingView::OnCalcEntropy)
	ON_COMMAND(ID_CALC_GRADIENT, &CImage_ProcessingView::OnCalcGradient)
	ON_COMMAND(ID_ID_FILTER_MEDIUM2, &CImage_ProcessingView::OnIdFilterMedium2)
	ON_COMMAND(ID_FILTER_MEDIUM_vecSort, &CImage_ProcessingView::OnFilterMediumvecsort)
	ON_COMMAND(ID_FILTER_MEDIUM_HIST, &CImage_ProcessingView::OnFilterMediumHist)
	ON_COMMAND(ID_FFT, &CImage_ProcessingView::OnFft)
	ON_COMMAND(ID_FFT_REVERSE, &CImage_ProcessingView::OnFftReverse)
	ON_COMMAND(ID_FILTER, &CImage_ProcessingView::OnFilter)
	ON_COMMAND(ID_Sharp, &CImage_ProcessingView::OnSharp)
	ON_COMMAND(ID_HIST_GDH, &CImage_ProcessingView::OnHistGdh)
	ON_COMMAND(ID_FILTER_ILPF, &CImage_ProcessingView::OnFilterIlpf)
	ON_COMMAND(ID_REBUILT_FROM_PHASE, &CImage_ProcessingView::OnRebuiltFromPhase)
	ON_COMMAND(ID_FILTER_IHPF, &CImage_ProcessingView::OnFilterIhpf)
	ON_COMMAND(ID_FILTER_HPF_SET, &CImage_ProcessingView::OnFilterHpfSet)
	ON_COMMAND(ID_FILTER_LPF_SET, &CImage_ProcessingView::OnFilterLpfSet)
	ON_COMMAND(ID_FILTER_SORT, &CImage_ProcessingView::OnFilterSort)
	ON_COMMAND(ID_REBUILT_FROM_AMPLITUDE, &CImage_ProcessingView::OnRebuiltFromAmplitude)
	ON_COMMAND(ID_GENERATEIMG, &CImage_ProcessingView::OnGenerateimg)
	ON_COMMAND(ID_Image_Sub, &CImage_ProcessingView::OnImageSub)
	ON_COMMAND(ID_FLITER_MOTION_BLUR, &CImage_ProcessingView::OnFliterMotionBlur)
	ON_COMMAND(ID_FILTER_TURBULENCE_BLUR, &CImage_ProcessingView::OnFilterTurbulenceBlur)
	ON_COMMAND(ID_INTERPOLATION_NEAREST, &CImage_ProcessingView::OnInterpolationNearest)
	ON_COMMAND(ID_INTERPOLATION_SET, &CImage_ProcessingView::OnInterpolationSet)
	ON_COMMAND(ID_ROTATE, &CImage_ProcessingView::OnRotate)
	ON_COMMAND(ID_HIST_EQ_HSI, &CImage_ProcessingView::OnHistEqHsi)
	ON_COMMAND(ID_32904, &CImage_ProcessingView::On32904)
	ON_COMMAND(ID_SHOW_R, &CImage_ProcessingView::OnShowR)
	ON_COMMAND(ID_SHOW_G, &CImage_ProcessingView::OnShowG)
	ON_COMMAND(ID_SHOW_B, &CImage_ProcessingView::OnShowB)
	ON_COMMAND(ID_SHOW_H, &CImage_ProcessingView::OnShowH)
	ON_COMMAND(ID_SHOW_S, &CImage_ProcessingView::OnShowS)
	ON_COMMAND(ID_SHOW_I, &CImage_ProcessingView::OnShowI)
	ON_COMMAND(ID_IMGSEG_RGB, &CImage_ProcessingView::OnImgsegRgb)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_IMGSEG_HSI, &CImage_ProcessingView::OnImgsegHsi)
	ON_COMMAND(ID_GrayWorld, &CImage_ProcessingView::OnGrayworld)
	ON_COMMAND(ID_FILTER_GRADIENT, &CImage_ProcessingView::OnFilterGradient)
	ON_COMMAND(ID_FILTER_THRESHOLD, &CImage_ProcessingView::OnFilterThreshold)
	ON_COMMAND(ID_FILTER_GAUSS_LAPLACIAN, &CImage_ProcessingView::OnFilterGaussLaplacian)
	ON_COMMAND(ID_EDIT_UNDO, &CImage_ProcessingView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CImage_ProcessingView::OnEditRedo)
	ON_COMMAND(ID_FILTER_GRADIENT_AM_L2, &CImage_ProcessingView::OnFilterGradientAmL2)
	ON_COMMAND(ID_FILTER_GRADIENT_ANGLE, &CImage_ProcessingView::OnFilterGradientAngle)
	ON_COMMAND(ID_Canny, &CImage_ProcessingView::OnCanny)
	ON_COMMAND(ID_test1, &CImage_ProcessingView::Ontest1)
	ON_COMMAND(ID_cannyDouble, &CImage_ProcessingView::Oncannydouble)
	ON_COMMAND(ID_THRESH_GLOBALBASIC, &CImage_ProcessingView::OnThreshGlobalbasic)
	ON_COMMAND(ID_THRESH_OTSU, &CImage_ProcessingView::OnThreshOtsu)
	ON_COMMAND(ID_DRAW_ANY, &CImage_ProcessingView::OnDrawAny)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ANY, &CImage_ProcessingView::OnUpdateDrawAny)
	ON_COMMAND(ID_FILTER_MOTION, &CImage_ProcessingView::OnFilterMotion)
	ON_COMMAND(ID_DRAW_ELLIPS, &CImage_ProcessingView::OnDrawEllips)
	ON_COMMAND(ID_DRAW_RECT, &CImage_ProcessingView::OnDrawRect)
	ON_COMMAND(ID_HOUGH_LINE, &CImage_ProcessingView::OnHoughLine)
	ON_COMMAND(ID_FILTER_GAUSSIAN, &CImage_ProcessingView::OnFilterGaussian)
	ON_COMMAND(ID_MORPH_DILATE, &CImage_ProcessingView::OnMorphDilate)
	ON_COMMAND(ID_MORPH_ERODE, &CImage_ProcessingView::OnMorphErode)
	ON_COMMAND(ID_MORPH_OPEN, &CImage_ProcessingView::OnMorphOpen)
	ON_COMMAND(ID_MORPH_CLOSE, &CImage_ProcessingView::OnMorphClose)
	ON_COMMAND(ID_MORPH_TOPHAT, &CImage_ProcessingView::OnMorphTophat)
	ON_COMMAND(ID_MORPH_BLACKHAT, &CImage_ProcessingView::OnMorphBlackhat)
	ON_COMMAND(ID_MORPH_BORDER_EXTRACT, &CImage_ProcessingView::OnMorphBorderExtract)
	ON_COMMAND(ID_FILTER_MEDIUM_AUTOADAPTIVE, &CImage_ProcessingView::OnFilterMediumAutoadaptive)
	ON_WM_HSCROLL()
	ON_COMMAND(ID_ENCODE_HUFFMAN, &CImage_ProcessingView::OnEncodeHuffman)
	ON_COMMAND(ID_ENCODE_SHANNON, &CImage_ProcessingView::OnEncodeShannon)
END_MESSAGE_MAP()

// CImage_ProcessingView 构造/析构

CImage_ProcessingView::CImage_ProcessingView()
	: m_strFileNameSave(_T(""))
	, m_nNoiseType(0)
{
	// TODO: 在此处添加构造代码
	m_bIsImgSEGing = FALSE;
	m_bIsProcessed = FALSE;
	m_bIsGrayed = FALSE;
	m_bMenuEnable = FALSE;
	m_bIsFFTed = false;
	m_bIsQuatilized = FALSE;
	m_bImgSegRectSelectStart = FALSE;

	m_nResDownRate = 1;

	m_nGrayRate = 1;

	m_nFTSortMode = 0;
	m_nFTAlphaAvg_d = 4;
	m_dNoiseCount = 0.01;
	m_nNoiseType = 0;
	m_nFtMidPara = 3;
	m_dFTAVGNiXieQ = 0;
	m_nFtAvgPara = 1;
	m_nFTAVGMode = 0;
	m_nFtAvgFillMode = 2; //1位填充0，0填充255，2复制边界填充
	m_nFtMidFillMode = 2; //1位填充0，0填充255，2复制边界填充
	m_dHPFRadius = 50;
	m_dHPFEnforcePara = 0.5;
	m_nHPFMode = 0;
	m_nBHPFOrder = 2;
	m_dHPFOriImgPara = 1.0;
	m_dLPFRadius = 100;
	m_nLPFMode = 0;
	m_nBLPFOrder = 2;
	m_dNoisePara1 = 0.0;
	m_dNoisePara2 = 20;

	m_nInterPolationHeight = 100;
	m_nInterPolationWidth = 100;
	m_nInterpolationType = 2;

	//m_bIsDrawAny = false; // 画图相关
	m_bIsLBDown = false;
	m_nDrawType = -1; //
	m_bIsDrawing = false;

	m_pFD = NULL;
}

CImage_ProcessingView::~CImage_ProcessingView()
{
	if (m_pFD)
	{
		delete[] m_pFD;
	}

}


BOOL CImage_ProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	//打开后自动读入一张图
	//CString str("测试图_椒盐噪声.jpg");
	CString str("0.jpg");
	m_Image.Load(str);
	m_nWidth = m_Image.GetWidth();
	m_nHeight = m_Image.GetHeight();
	m_nInterPolationWidth = m_nWidth;
	m_nInterPolationHeight = m_nHeight;
	m_imgStock.clear();
	m_imgStock.AddImageToStock(m_Image);


	//AfxMessageBox(L"View->PreCreateWindow");
	//ChangeScrollSize();
	//cs.cx = m_nWidth;
	//cs.cy = m_nHeight;
	//cs.cx = 200;
	//cs.cy = 300;
	//MoveWindow(150,0,m_nWidth,m_nHeight);
	//BOOL a = SetWindowPos(NULL,0,0,m_nWidth,m_nHeight,SWP_NOMOVE);  
	//cout << a << endl;
	//str.Format(_T("%d"),a);
	//MessageBox(str);
	//Invalidate(TRUE);



	//SetWindowLong
	return CScrollView::PreCreateWindow(cs);
}

// CImage_ProcessingView 绘制

void CImage_ProcessingView::OnDraw(CDC* pDC)
{
	CImage_ProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	ChangeScrollSize();
	if (m_bIsProcessed == TRUE)
	{
		if (!m_ImageAfter.IsNull())
		{
			//m_Image.Draw(pDC->m_hDC,0,0);
			//m_ImageAfter.Draw(pDC->m_hDC,m_nWidth+10,0);
			m_ImageAfter.Draw(pDC->m_hDC, 0, 0);
		}
		else
		{
			cout << "图像空" << endl;
		}
	}
	else
	{
		if (!m_Image.IsNull())
		{
			m_Image.Draw(pDC->m_hDC, 0, 0);
		}
		else
		{
			cout << "图像空" << endl;
		}
	}

	CString strImgSize;
	if (m_bIsProcessed)
	{
		strImgSize.Format(_T("长:%d,宽:%d"), m_ImageAfter.GetHeight(), m_ImageAfter.GetWidth());
	}
	else
	{
		strImgSize.Format(_T("长:%d,宽:%d"), m_Image.GetHeight(), m_Image.GetWidth());
	}

	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(strImgSize);
	//std::cout << sz.cx << std::endl;
	//CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;  
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFCStatusBar *pStatusBar = (CMFCStatusBar *)&(pFrame->m_wndStatusBar);

	int index = pStatusBar->CommandToIndex(ID_INDICATOR_IMGSIZE);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_IMGSIZE, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strImgSize);
	ReleaseDC(pDC);

	return;
}

void CImage_ProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//AfxMessageBox(L"View->OnInitialUpdate");
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 300; //起始时未打开图片的情况下 View客户区小于300x300就设置滚轮
	SetScrollSizes(MM_TEXT, sizeTotal);

	//这里cout 会导致cout 缓冲区出错？？？
	//cout << SetWindowPos(NULL,0,0,m_nWidth,m_nHeight,SWP_NOMOVE) << endl; 
}


// CImage_ProcessingView 打印


void CImage_ProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImage_ProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImage_ProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImage_ProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImage_ProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImage_ProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	// 平时单击右键显示剪切菜单
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImage_ProcessingView 诊断

#ifdef _DEBUG
void CImage_ProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImage_ProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImage_ProcessingDoc)));
	return (CImage_ProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImage_ProcessingView 消息处理程序


void CImage_ProcessingView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if (IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if (!m_Image.IsNull()) m_Image.Destroy();//判断是否已经有图片，有的话进行清除

		if (!m_ImageAfter.IsNull()) m_ImageAfter.Destroy();//清除after变量的数据

		if (!m_ImageGrayed.IsNull()) m_ImageGrayed.Destroy();

		if (!m_ImageFT.IsNull()) m_ImageFT.Destroy();

		m_dMapHistR.clear(); m_dMapHistG.clear(); m_dMapHistB.clear();
		m_vecHistProb.clear();

		m_bIsProcessed = FALSE;
		m_bIsGrayed = FALSE;
		m_bMenuEnable = FALSE;
		m_bIsFFTed = false;
		m_strFileNameSave = dlg.GetPathName(); //不能用getfilename
		//cout << m_strFileNameSave << endl;
		//CString str = dlg.GetPathName();
		m_Image.Load(m_strFileNameSave);//获得图片的地址，并且加载图片
		//这里只是加载到数组里，后面的Invalidate(1)再来调用Ondraw函数再来调用MyImage_的Draw方法来画出图片
		//获得图片的大小，并按其大小设置滚动条的初始窗口大小等参数
		m_nWidth = m_Image.GetWidth();
		m_nHeight = m_Image.GetHeight();
		m_nInterPolationWidth = m_nWidth;
		m_nInterPolationHeight = m_nHeight;

		CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
		pMainFrame->SetWindowPos(NULL, 0, 0, m_nWidth + 50, m_nHeight + 150, SWP_NOMOVE);


		m_imgStock.clear();
		m_imgStock.AddImageToStock(m_Image);

		//cout << m_nInterPolationWidth << endl;
		ChangeScrollSize();
		//Window_Image_w=m_Image.GetWidth();//获得图片的初始大小，为放大缩小功能做准备
		//Window_Image_h=m_Image.GetHeight();//		
		Invalidate(1); //强制调用ONDRAW函数
	}
}


void CImage_ProcessingView::OnDrawline()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) return;

	//此方法是直接在客户区画横线，但是窗口的重绘消息调用后会消失
	//CClientDC dc(this);
	//CPen pen(BS_SOLID,3,RGB(255,0,0));
	//dc.SelectObject(&pen);
	//CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	//dc.SelectObject(pBrush);
	//CPoint ptStart,ptEnd;
	//CRect rect;
	//GetClientRect(&rect);
	//ptStart.x = 0;
	//ptStart.y = 10;
	//ptEnd.x = w;
	//ptEnd.y = 10;
	//dc.MoveTo(ptStart);
	//dc.LineTo(ptEnd);
	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	//在第十行画横线 此方法是在读入的图片数组里进行画图，而不是在客户区上画图
	for (UINT k = 0; k < m_nWidth; k++)
	{
		m_ImageAfter.m_pBits[0][10][k] = 0;//B
		m_ImageAfter.m_pBits[1][10][k] = 0;//G
		m_ImageAfter.m_pBits[2][10][k] = 0;//R
	}
	UpdateState();
}


void CImage_ProcessingView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) {
		MessageBox(_T("你还没有打开一个要保存的图像文件！"));
		return;
	}
	CString strFilter;
	strFilter = "BMP 位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||";   //
	//	strFilter ="所有文件|*.*||";   //

	CFileDialog dlg(FALSE, NULL, NULL, NULL, strFilter);
	//CFileDialog dlg(FALSE,NULL,NULL,NULL);

	if (IDOK != dlg.DoModal())
		return;
	// 如果用户没有指定文件扩展名，则为其添加一个
	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;   //  获得文件名
	//if(dlg.m_ofn.nFileExtension == 0)     //获得文件扩展名
	//{
	switch (dlg.m_ofn.nFilterIndex)
	{
	case 1:
		strExtension = "bmp"; break;
	case 2:
		strExtension = "jpg"; break;
	case 3:
		strExtension = "gif"; break;
	case 4:
		strExtension = "png"; break;
	default:
		break;
	}
	strFileName = strFileName + _T(".") + strExtension;
	//	//strFileName="C:\\Users\\Lenovo\\Desktop\\QSanguosha-Qingming\\abc.bmp";
	//}
	// 图像保存
	HRESULT hResult;
	if (m_bIsProcessed)
		hResult = m_ImageAfter.Save(strFileName);
	else
		hResult = m_Image.Save(strFileName);
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}

}


void CImage_ProcessingView::OnShowred()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_ImageAfter.m_pBits[0][j][k] = 0;//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_ImageAfter.m_pBits[1][j][k] = 0;//G

		}
	}
	UpdateState();
}


//void CImage_ProcessingView::OnViewToolbar()
//{
//	// TODO: 在此添加命令处理程序代码
//}




void CImage_ProcessingView::OnDoubleimage()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ Image_Second;
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if (IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if (!Image_Second.IsNull()) Image_Second.Destroy();//判断是否已经有图片，有的话进行清除

		Image_Second.Load(dlg.GetPathName());//调用对话框的GetPathName获得图片的位置，并且调用Load加载图片
		//m_bIsProcessed = TRUE;

		if (Image_Second.IsNull()) return;
		if (m_nWidth != Image_Second.GetWidth() || m_nHeight != Image_Second.GetHeight())//判断两幅图像大小是否一致
		{
			MessageBox(_T("两幅图像大小不一致！"));
			//Image_Second.Destroy();
			return;
		}
		for (UINT j = 0; j < m_nHeight; j++)
			for (UINT i = 0; i < m_nWidth; i++)//两幅图像做简单相加操作
			{
				m_ImageAfter.m_pBits[0][j][i] = SaturateCast(m_ImageAfter.m_pBits[0][j][i] + Image_Second.m_pBits[0][j][i], 0, 255);
				m_ImageAfter.m_pBits[1][j][i] = SaturateCast(m_ImageAfter.m_pBits[1][j][i] + Image_Second.m_pBits[1][j][i], 0, 255);
				m_ImageAfter.m_pBits[2][j][i] = SaturateCast(m_ImageAfter.m_pBits[2][j][i] + Image_Second.m_pBits[2][j][i], 0, 255);
			}
	}
	UpdateState();
}


void CImage_ProcessingView::OnReverse()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_ImageAfter.m_pBits[0][j][k] = 255 - m_ImageAfter.m_pBits[0][j][k];//B   用循环访问图像的像素值
			m_ImageAfter.m_pBits[1][j][k] = 255 - m_ImageAfter.m_pBits[1][j][k];//G
			m_ImageAfter.m_pBits[2][j][k] = 255 - m_ImageAfter.m_pBits[2][j][k];//R

		}
	}
	UpdateState();
}


int CImage_ProcessingView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//AfxMessageBox(L"View->OnCreate");
// 	CString str;
// 	str.Format(L"x:%d,y:%d,cx:%d,cy:%d",lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx,lpCreateStruct->cy);
// 	AfxMessageBox(str);
	//ChangeScrollSize();
	return 0;
}


void CImage_ProcessingView::OnTogray()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);
	//	if(m_bIsGrayed == TRUE)
	//		m_Image.CopyTo(m_ImageAfter);
	//	

	//
	//// 	MyImage_ temp;
	//// 	doToGray(m_ImageAfter,temp);
	//// 	temp.CopyTo(m_ImageAfter);
	//// 	UpdateState();
	//// 	return;
	//
	//	UINT average = 0;
	//	for (UINT j=0;j<m_nHeight;j++)
	//	{
	//		for (UINT k=0;k<m_nWidth;k++)
	//		{
	//			average = (m_ImageAfter.m_pBits[0][j][k]+m_ImageAfter.m_pBits[1][j][k]+m_ImageAfter.m_pBits[2][j][k])/3;
	//			m_ImageAfter.m_pBits[0][j][k]=average;//B   用循环访问图像的像素值
	//			m_ImageAfter.m_pBits[1][j][k]=average;//G
	//			m_ImageAfter.m_pBits[2][j][k]=average;//R
	//
	//		}
	//	}
	//
	//	m_ImageAfter.CopyTo(m_ImageGrayed); //备份一次

	MyImage_ imgGrayed;
	doToGray(m_ImageAfter, imgGrayed);
	imgGrayed.CopyTo(m_ImageAfter);
	m_ImageAfter.CopyTo(m_ImageGrayed); //备份,用于灰度级变化处理中
	m_bMenuEnable = TRUE;
	m_bIsGrayed = TRUE;
	UpdateState();
}

//支持In-Place操作
void CImage_ProcessingView::doToGray(const MyImage_ &srcImg, MyImage_ &dstImg) //只是让图像灰度化，不考虑状态变量
{

	if (srcImg.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (dstImg.IsNull())
		srcImg.CopyTo(dstImg);

	if (srcImg.IsGrayed() == true)
	{
		srcImg.CopyTo(dstImg);
		dstImg.SetGrayed(true);
		return;
	}

	UINT average = 0;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			average = (srcImg.m_pBits[0][j][k] + srcImg.m_pBits[1][j][k] + srcImg.m_pBits[2][j][k]) / 3;
			dstImg.m_pBits[0][j][k] = average;//B   用循环访问图像的像素值
			dstImg.m_pBits[1][j][k] = average;//G
			dstImg.m_pBits[2][j][k] = average;//R
		}
	}

}



void CImage_ProcessingView::UpdateState(bool bIsStoreImage)
{
	if (bIsStoreImage)
	{
		m_imgStock.AddImageToStock(m_ImageAfter);
	}
	//cout << m_imgStock.getStockCount() << "," << m_imgStock.getCurIndex() << endl;
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();
	m_bIsProcessed = TRUE;
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}

void CImage_ProcessingView::OnResdwon()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	m_nResDownRate *= 2;

	UINT w1 = m_nWidth / m_nResDownRate;
	UINT h1 = m_nHeight / m_nResDownRate;

	UINT rate = m_nResDownRate;

	//两种方法：1.建立w1,h1的图，但是其他程序里得再加变量来判断
	//			2.直接建立w,h的图，背景色用255填充，但是其他算法无法叠加

	m_ImageAfter.Create(m_nWidth, m_nHeight, 255);

	for (UINT j = 0; j != h1; j++)
	{
		for (UINT i = 0; i != w1; i++)
		{
			m_ImageAfter.m_pBits[0][j][i] = m_Image.m_pBits[0][j*rate][i*rate];
			m_ImageAfter.m_pBits[1][j][i] = m_Image.m_pBits[1][j*rate][i*rate];
			m_ImageAfter.m_pBits[2][j][i] = m_Image.m_pBits[2][j*rate][i*rate];
		}
	}
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();

	ChangeScrollSize();
	UpdateState();

	//for (int j=0;j!=h;j++)
	//{
	//	for (int i=0;i!=w;i++)
	//	{
	//		if(j<h1 && i<w1)
	//		{
	//			m_Image.m_pBits[0][j][i] = m_Image.m_pBits[0][j*2][i*2];
	//			m_Image.m_pBits[1][j][i] = m_Image.m_pBits[1][j*2][i*2];
	//			m_Image.m_pBits[2][j][i] = m_Image.m_pBits[2][j*2][i*2];
	//		}
	//		else
	//		{
	//			m_Image.m_pBits[0][j][i] = 255;
	//			m_Image.m_pBits[1][j][i] = 255;
	//			m_Image.m_pBits[2][j][i] = 255;	
	//		}

	//	}
	//}
	//Invalidate();


	//m_Image.CopyTo(m_ImageAfter);
	//m_bIsProcessed = TRUE;
	//m_ImageAfter.Creat(200,200,120);
	//Invalidate();
	//UINT average = 0;
	//for (int j=0;j<h;j++)
	//{
	//	for (int k=0;k<w;k++)
	//	{
	//		average = (m_Image.m_pBits[0][j][k]+m_Image.m_pBits[1][j][k]+m_Image.m_pBits[2][j][k])/3;
	//		m_Image.m_pBits[0][j][k]=average;//B   用循环访问图像的像素值
	//		m_Image.m_pBits[1][j][k]=average;//G
	//		m_Image.m_pBits[2][j][k]=average;//R

	//	}
	//}
}


void CImage_ProcessingView::OnRetrieve()
{
	// TODO: Add your command handler code here
	if (m_bIsProcessed)
	{
		m_bIsProcessed = FALSE;
		m_bIsGrayed = FALSE;
		m_bImgSegRectSelectStart = FALSE;
		m_bIsFFTed = FALSE;
		m_bIsImgSEGing = FALSE;
		m_bMenuEnable = FALSE;


		m_ImageAfter.Destroy();
		m_ImageFT.Destroy();
		m_ImageGrayed.Destroy();

		m_nHeight = m_Image.GetHeight();
		m_nWidth = m_Image.GetWidth();

		m_imgStock.clear();
		m_imgStock.AddImageToStock(m_Image);

		Invalidate();
	}
	else return;
}


void CImage_ProcessingView::OnTest()
{
	//// TODO: Add your command handler code here
	///*CRect rcWindow,rcClient;*/
	//m_Image.CopyTo(m_ImageToDlgShow);
	//CDlgShowImg *pDlg = new CDlgShowImg;
	//pDlg->Create(IDD_DLG_SHOW_IMG, this);
	//pDlg->ShowWindow(SW_SHOW);
	////m_ImageAfter.Creat(200,200,0);
	////m_bIsProcessed = TRUE;
	//Invalidate();

	vector<char> code;
	code = m_com.DecimalDec2Bin(0.95333099365234375,6);
	code = m_com.DecimalDec2Bin(0.96465301513671875, 6);



	double a[] = { 0.2,0.1,0.5,0.8,0.05 };
	int p[] = { 0,1,2,3,4 };
	m_com.sortindex(a, p, 5);

	cout << log2(0.00033950805664062500) << endl;
	cout << log(0.00033950805664062500) / log(2) << endl;
	cout << floor(1.0) << endl;
}


void CImage_ProcessingView::OnGraytohalf()
{
	//if(!m_bIsGrayed) 
	//{
	//	CString str("请先执行灰度化！");
	//	MessageBox(str);
	//	return;
	//}
	//if (m_ImageAfter.IsNull())
	//	return;

	//m_nGrayRate *=2;
	//double temp;
	//UINT nTrehshold = 256/2 -1;
	//for (UINT j=0;j<m_nHeight;j++)
	//{
	//	for (UINT k=0;k<m_nWidth;k++)
	//	{
	//		temp = (double)m_ImageAfter.m_pBits[0][j][k]/m_nGrayRate;
	//		if(m_ImageAfter.m_pBits[0][j][k] > 127)
	//			if(temp > (UINT)temp) 
	//				temp = (UINT)temp + 1;
	//		temp *= m_nGrayRate;
	//		m_ImageAfter.m_pBits[0][j][k] = temp ;
	//		m_ImageAfter.m_pBits[1][j][k] = temp ;
	//		m_ImageAfter.m_pBits[2][j][k] = temp ;
	//	}
	//}
	////m_bIsGrayed = TRUE;
	//m_bIsProcessed = TRUE;
	//Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像

	// TODO: Add your command handler code here
}


void CImage_ProcessingView::OnGray2()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);

	m_nCycleTimes = 2;
	m_nQuantiStart = 0;
	m_nQuantiIncrease = 256 / m_nCycleTimes;
	Im2Gray();

	//for (int j=0;j<h;j++)
	//{
	//	for (int k=0;k<w;k++)
	//	{
	//		if (m_ImageAfter.m_pBits[0][j][k]>127)
	//		{
	//			m_ImageAfter.m_pBits[0][j][k] = 255 ;
	//			m_ImageAfter.m_pBits[1][j][k] = 255 ;
	//			m_ImageAfter.m_pBits[2][j][k] = 255 ;
	//		}
	//		else 
	//		{
	//			m_ImageAfter.m_pBits[0][j][k] = 0 ;
	//			m_ImageAfter.m_pBits[1][j][k] = 0 ;
	//			m_ImageAfter.m_pBits[2][j][k] = 0 ;
	//		}
	//	}
	//}
	//m_bIsGrayed = TRUE;
	UpdateState();
}


void CImage_ProcessingView::OnGray4()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);

	m_nCycleTimes = 4;
	m_nQuantiStart = 96;
	m_nQuantiIncrease = 256 / m_nCycleTimes;

	Im2Gray();


	UpdateState();
}


void CImage_ProcessingView::OnGray64()
{
	// TODO: Add your command handler code here

	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nCycleTimes = 64;
	m_nQuantiStart = 5;
	m_nQuantiIncrease = 4;
	Im2Gray();
	//m_bIsGrayed = TRUE;
	UpdateState();
}


void CImage_ProcessingView::OnGray128()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nCycleTimes = 128;
	m_nQuantiStart = 3;
	m_nQuantiIncrease = 256 / m_nCycleTimes;
	//CString str;
	//str.Format(_T("%d"),m_nQuantiIncrease);
	//MessageBox(str);
	Im2Gray();
	//m_bIsGrayed = TRUE;
	UpdateState();


}

void CImage_ProcessingView::Im2Gray()
{
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_bIsQuatilized = FALSE;
			if (m_ImageAfter.m_pBits[0][j][k] >= 0 && m_ImageAfter.m_pBits[0][j][k] < m_nQuantiIncrease && !m_bIsQuatilized)
			{
				m_ImageAfter.m_pBits[0][j][k] = 0;
				m_ImageAfter.m_pBits[1][j][k] = 0;
				m_ImageAfter.m_pBits[2][j][k] = 0;
				m_bIsQuatilized = TRUE;
			}

			for (UINT i1 = 1; i1 != m_nCycleTimes - 1 && !m_bIsQuatilized; ++i1)
			{
				if (m_ImageAfter.m_pBits[0][j][k] >= m_nQuantiIncrease*i1 && m_ImageAfter.m_pBits[0][j][k] < (i1 + 1)*m_nQuantiIncrease)
				{
					m_ImageAfter.m_pBits[0][j][k] = m_nQuantiStart + (i1 - 1)*m_nQuantiIncrease;
					m_ImageAfter.m_pBits[1][j][k] = m_nQuantiStart + (i1 - 1)*m_nQuantiIncrease;
					m_ImageAfter.m_pBits[2][j][k] = m_nQuantiStart + (i1 - 1)*m_nQuantiIncrease;
					m_bIsQuatilized = TRUE;
				}
			}

			if (m_ImageAfter.m_pBits[0][j][k] >= 256 - m_nQuantiIncrease && m_ImageAfter.m_pBits[0][j][k] < 256 && !m_bIsQuatilized)
			{
				m_ImageAfter.m_pBits[0][j][k] = 255;
				m_ImageAfter.m_pBits[1][j][k] = 255;
				m_ImageAfter.m_pBits[2][j][k] = 255;
			}
		}
	}
}


void CImage_ProcessingView::OnGray32()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nCycleTimes = 32;
	m_nQuantiStart = 12;
	m_nQuantiIncrease = 256 / m_nCycleTimes;
	//CString str;
	//str.Format(_T("%d"),m_nQuantiIncrease);
	//MessageBox(str);
	Im2Gray();
	//m_bIsGrayed = TRUE;
	UpdateState();
}


void CImage_ProcessingView::OnGray16()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nCycleTimes = 16;
	m_nQuantiStart = 24;
	m_nQuantiIncrease = 256 / m_nCycleTimes;
	//CString str;
	//str.Format(_T("%d"),m_nQuantiIncrease);
	//MessageBox(str);
	Im2Gray();
	//m_bIsGrayed = TRUE;
	UpdateState();
}


void CImage_ProcessingView::OnGray8()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nCycleTimes = 8;
	m_nQuantiStart = 48;
	m_nQuantiIncrease = 256 / m_nCycleTimes;
	//CString str;
	//str.Format(_T("%d"),m_nQuantiIncrease);
	//MessageBox(str);
	Im2Gray();
	//m_bIsGrayed = TRUE;
	UpdateState();
}


void CImage_ProcessingView::OnUpdateGray2(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bMenuEnable);
}


void CImage_ProcessingView::OnUpdateGray4(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bMenuEnable);
}

void CImage_ProcessingView::OnUpdateGray8(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bMenuEnable);
}
void CImage_ProcessingView::OnUpdateGray16(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bMenuEnable);
}
void CImage_ProcessingView::OnUpdateGray32(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bMenuEnable);
}
void CImage_ProcessingView::OnUpdateGray64(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bMenuEnable);
}
void CImage_ProcessingView::OnUpdateGray128(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bMenuEnable);
}

void CImage_ProcessingView::OnUpdateTogray(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (!m_bMenuEnable)
	{
		pCmdUI->SetText(_T("到灰度图"));
	}
	else
	{
		pCmdUI->SetText(_T("恢复256级灰度图"));
	}
}


void CImage_ProcessingView::OnNoiseSalt()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	switch (m_nNoiseType)
	{
	case 0: //椒盐噪声255
	{
		UINT nSaltCount = static_cast<UINT>(m_nHeight * m_nWidth * m_dNoiseCount);
		srand((int)time(0));
		for (UINT i = 0; i != nSaltCount; i++)
		{
			UINT m = random(m_nWidth);
			UINT n = random(m_nHeight);
			m_ImageAfter.m_pBits[0][n][m] = 255;
			m_ImageAfter.m_pBits[1][n][m] = 255;
			m_ImageAfter.m_pBits[2][n][m] = 255;
		}
	}
	break;
	case 1: //胡椒噪声0
	{
		UINT nSaltCount = static_cast<UINT>(m_nHeight * m_nWidth * m_dNoiseCount);
		srand((int)time(0));
		for (UINT i = 0; i != nSaltCount; i++)
		{
			UINT m = random(m_nWidth);
			UINT n = random(m_nHeight);
			m_ImageAfter.m_pBits[0][n][m] = 0;
			m_ImageAfter.m_pBits[1][n][m] = 0;
			m_ImageAfter.m_pBits[2][n][m] = 0;
		}
	}
	break;
	case 2: //高斯噪声
	{

		std::default_random_engine ranEngine((unsigned long)time(NULL)); //只能置一个随机数种子，每1s内重新设置的随机数种子产生的会是一样的随机数
		std::normal_distribution<double> dis(m_dNoisePara1, m_dNoisePara2);
		for (int c = 0; c != 3; ++c)
		{
			for (UINT i = 0; i != m_nHeight; ++i)
			{
				for (int j = 0; j != m_nWidth; ++j)
				{
					double dGauss = dis(ranEngine);
					double val = SaturateCast(m_ImageAfter.m_pBits[c][i][j] + dGauss, 0.0, 255.0);
					m_ImageAfter.m_pBits[c][i][j] = (BYTE)val;
				}
			}
		}
	}
	break;
	case 3: //瑞利分布，需要将参数二值设大才明显
	{
		CCommon com;
		std::default_random_engine ranEngine((unsigned long)time(NULL)); //只能置一个随机数种子，每1s内重新设置的随机数种子产生的会是一样的随机数
		std::uniform_real_distribution<double> dis(0, 1);
		for (int c = 0; c != 3; ++c)
		{
			for (UINT i = 0; i != m_nHeight; ++i)
			{
				for (int j = 0; j != m_nWidth; ++j)
				{
					double temp = dis(ranEngine);
					//数字图像处理(Matlab版)上找到的
					double dRuili = m_dNoisePara1 + sqrt(-m_dNoisePara2 * log(1 - temp));//log是e为底，log10才是10为底
					double val = SaturateCast(m_ImageAfter.m_pBits[c][i][j] + dRuili, 0.0, 255.0);
					m_ImageAfter.m_pBits[c][i][j] = (BYTE)val;
				}
			}
		}
	}
	break;
	case 4: //指数分布,参数一是a值
	{

		std::default_random_engine ranEngine((unsigned long)time(NULL)); //只能置一个随机数种子，每1s内重新设置的随机数种子产生的会是一样的随机数
		std::uniform_real_distribution<double> dis(0, 1);
		for (int c = 0; c != 3; ++c)
		{
			for (UINT i = 0; i != m_nHeight; ++i)
			{
				for (int j = 0; j != m_nWidth; ++j)
				{
					double temp = dis(ranEngine);
					//数字图像处理(Matlab版)上找到的
					double dZhishu = -1 / m_dNoisePara1*log(1 - temp) * 255;//log是e为底，log10才是10为底
					//cout << dZhishu << endl;
					double val = SaturateCast(m_ImageAfter.m_pBits[c][i][j] + dZhishu, 0.0, 255.0);
					m_ImageAfter.m_pBits[c][i][j] = (BYTE)val;
				}
			}
		}
	}
	break;

	case 5: //均匀分布,参数一是a值,参数二b值
	{

		std::default_random_engine ranEngine((unsigned long)time(NULL)); //只能置一个随机数种子，每1s内重新设置的随机数种子产生的会是一样的随机数
		std::uniform_real_distribution<double> dis(0, 1);
		for (int c = 0; c != 3; ++c)
		{
			for (UINT i = 0; i != m_nHeight; ++i)
			{
				for (int j = 0; j != m_nWidth; ++j)
				{
					double temp = dis(ranEngine);
					//数字图像处理(Matlab版)上找到的
					double dJunyun = m_dNoisePara1 + (m_dNoisePara2 - m_dNoisePara1)*temp;//log是e为底，log10才是10为底
					//cout << dZhishu << endl;
					double val = SaturateCast(m_ImageAfter.m_pBits[c][i][j] + dJunyun, 0.0, 255.0);
					m_ImageAfter.m_pBits[c][i][j] = (BYTE)val;
				}
			}
		}
	}
	break;

	case 6: //伽马分布,参数一是a值,参数二b值,默认噪声放大100倍
	{
		CCommon com;
		std::default_random_engine ranEngine((unsigned long)time(NULL)); //只能置一个随机数种子，每1s内重新设置的随机数种子产生的会是一样的随机数
		std::uniform_real_distribution<double> dis(0, 1);
		for (int c = 0; c != 3; ++c)
		{
			for (UINT i = 0; i != m_nHeight; ++i)
			{
				for (int j = 0; j != m_nWidth; ++j)
				{
					double dGama = 0.0;
					//数字图像处理(Matlab版)上找到的
					for (int b = 0; b != m_dNoisePara2; b++)
						dGama += -1 / m_dNoisePara1*log(1 - dis(ranEngine)) * 255;
					//	cout << dGama << endl;
					double val = SaturateCast(m_ImageAfter.m_pBits[c][i][j] + dGama, 0.0, 255.0);
					m_ImageAfter.m_pBits[c][i][j] = (BYTE)val;
				}
			}
		}
	}
	break;
	}

	UpdateState();
}


void CImage_ProcessingView::OnNoiseSet()
{
	// TODO: Add your command handler code here
	if (m_dlgNoiseSet.DoModal() == IDOK)
	{
		m_dNoiseCount = m_dlgNoiseSet.m_NoiseCount;
		m_nNoiseType = m_dlgNoiseSet.m_nNoiseType;
		m_dNoisePara1 = m_dlgNoiseSet.m_dNoisePara1;
		m_dNoisePara2 = m_dlgNoiseSet.m_dNoisePara2;
	}
}


void CImage_ProcessingView::OnBit1()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//UINT i = 0;CString str;
	//i  = myPow(5,2);
	//str.Format(_T("%d"),i);
	//MessageBox(str);

	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nBitCoefficient = 1;
	Im2Bit();
	UpdateState();
}


void CImage_ProcessingView::Im2Bit()
{
	UINT coefficient = myPow(2, m_nBitCoefficient - 1);
	UINT i = 0; CString str;
	str.Format(_T("%d"), coefficient);
	MessageBox(str);
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_ImageAfter.m_pBits[0][j][k] &= coefficient;
			m_ImageAfter.m_pBits[1][j][k] &= coefficient;
			m_ImageAfter.m_pBits[2][j][k] &= coefficient;
		}
	}
}

UINT myPow(UINT x, UINT y)
{
	UINT j = x;
	if (y != 0)
	{
		for (int i = 0; i != y - 1; i++)
		{
			x = j*x;
		}
		return x;
	}
	else {
		return 1;
	}

}

void CImage_ProcessingView::OnBit2()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nBitCoefficient = 2;
	Im2Bit();
	UpdateState();
}


void CImage_ProcessingView::OnBit3()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nBitCoefficient = 3;
	Im2Bit();
	UpdateState();
}


void CImage_ProcessingView::OnBit4()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nBitCoefficient = 4;
	Im2Bit();
	UpdateState();
}


void CImage_ProcessingView::OnBit5()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nBitCoefficient = 5;
	Im2Bit();
	UpdateState();
}


void CImage_ProcessingView::OnBit6()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nBitCoefficient = 6;
	Im2Bit();
	UpdateState();
}


void CImage_ProcessingView::OnBit7()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nBitCoefficient = 7;
	Im2Bit();
	UpdateState();
}


void CImage_ProcessingView::OnBit8()
{
	// TODO: Add your command handler code here
	if (!m_bIsGrayed)
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return; //执行了灰度化肯定是不空的了

	m_ImageGrayed.CopyTo(m_ImageAfter);
	m_nBitCoefficient = 8;
	Im2Bit();
	UpdateState();
}


void CImage_ProcessingView::OnHistShow()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
	{
		return;
	}
	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	CalcHistPorb(0);	//为0是计算直方图用的，为1是计算均衡化用的 //归一化的系数不同

	CPropSheetRGB *pPropSheet = new CPropSheetRGB(_T("rgb分量"));
	pPropSheet->Create();
	pPropSheet->ShowWindow(SW_SHOW);
	// 	m_bIsProcessed = TRUE;
	// 	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像

}


void CImage_ProcessingView::CalcHistPorb(UINT nCalcFlag)
{
	UINT nGrayScale; double dMaxR, dMaxG, dMaxB, dMax, dPixTotal = m_nHeight * m_nWidth;
	m_dMapHistR.clear();
	m_dMapHistG.clear();
	m_dMapHistB.clear();
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			nGrayScale = m_ImageAfter.m_pBits[0][j][k];
			++m_dMapHistB[nGrayScale];
			nGrayScale = m_ImageAfter.m_pBits[1][j][k];
			++m_dMapHistG[nGrayScale];
			nGrayScale = m_ImageAfter.m_pBits[2][j][k];
			++m_dMapHistR[nGrayScale];
		}
	}
	//R分量
	m_vecHistProb.clear();
	for (auto it = m_dMapHistR.begin(); it != m_dMapHistR.end(); ++it)
	{
		m_vecHistProb.push_back(static_cast<double>((it->second)));
	}
	dMaxR = maxVecValue(m_vecHistProb);
	std::cout << "最大红色分量灰度级数目:" << dMaxR << std::endl;

	//G分量
	m_vecHistProb.clear();
	//std::cout << m_vecHistProb.size()<< std::endl;
	for (auto it = m_dMapHistG.begin(); it != m_dMapHistG.end(); ++it)
	{
		m_vecHistProb.push_back(static_cast<double>((it->second)));
	}
	dMaxG = maxVecValue(m_vecHistProb);
	std::cout << "最大蓝色分量灰度级数目:" << dMaxG << std::endl;

	//B分量
	m_vecHistProb.clear();
	for (auto it = m_dMapHistB.begin(); it != m_dMapHistB.end(); ++it)
	{
		m_vecHistProb.push_back(static_cast<double>((it->second)));
	}
	dMaxB = maxVecValue(m_vecHistProb);
	std::cout << "最大绿色分量灰度级数目:" << dMaxB << std::endl;

	dMax = dMaxR > dMaxG ? dMaxR : dMaxG;
	dMax = dMaxB > dMax ? dMaxB : dMax; //找到最大值
	std::cout << "最大灰度级数目:" << dMax << std::endl;

	//为0是计算直方图用的，为1是计算均衡化用的
	dMax = (nCalcFlag == 0) ? dMax : dPixTotal;
	//分别归一化

	for (auto it = m_dMapHistR.begin(); it != m_dMapHistR.end(); ++it)
		it->second = it->second / dMax;

	for (auto it = m_dMapHistG.begin(); it != m_dMapHistG.end(); ++it)
		it->second = it->second / dMax;

	for (auto it = m_dMapHistB.begin(); it != m_dMapHistB.end(); ++it)
		it->second = it->second / dMax;
}


void CImage_ProcessingView::OnFilterAvg()
{
	// TODO: Add your command handler code here
	int  nPara = m_nFtAvgPara;

	int nPixCount = (nPara * 2 + 1) * (nPara * 2 + 1);

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	MyImage_ imgBackUp, imgFilled;
	m_ImageAfter.BorderFillTo(imgBackUp, m_nFtAvgPara, m_nFtAvgFillMode); //备份一下
	m_ImageAfter.BorderFillTo(imgFilled, m_nFtAvgPara, m_nFtAvgFillMode);

	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();
	//取均值
	switch (m_nFTAVGMode)
	{
	case 0://算数均值
		for (UINT i = nPara; i < h - nPara; ++i) //循环次数只有h-2
		{
			for (UINT j = nPara; j < w - nPara; ++j)//循环w-2次
			{
				int sum[3] = { 0,0,0 };//三个通道分别的和
				//应用模板

				for (UINT m = i - nPara; m <= i + nPara; m++)
				{
					for (UINT n = j - nPara; n <= j + nPara; n++)
					{
						sum[0] += imgFilled.m_pBits[0][m][n];
						sum[1] += imgFilled.m_pBits[1][m][n];
						sum[2] += imgFilled.m_pBits[2][m][n];
					}
				}
				sum[0] /= nPixCount;
				sum[1] /= nPixCount;
				sum[2] /= nPixCount;
				imgBackUp.m_pBits[0][i][j] = sum[0];
				imgBackUp.m_pBits[1][i][j] = sum[1];
				imgBackUp.m_pBits[2][i][j] = sum[2];
			}
		}
		break;
	case 1://几何均值
		for (UINT i = nPara; i < h - nPara; ++i) //循环次数只有h-2
		{
			for (UINT j = nPara; j < w - nPara; ++j)//循环w-2次
			{
				long double product[3] = { 1.0,1.0,1.0 };//三个通道分别的和
				//应用模板

				for (UINT m = i - nPara; m <= i + nPara; m++)
				{
					for (UINT n = j - nPara; n <= j + nPara; n++)
					{
						product[0] *= imgFilled.m_pBits[0][m][n];
						product[1] *= imgFilled.m_pBits[1][m][n];
						product[2] *= imgFilled.m_pBits[2][m][n];
					}
				}
				product[0] = pow(product[0], (long double)1.0 / nPixCount);
				product[1] = pow(product[1], (long double)1.0 / nPixCount);
				product[2] = pow(product[2], (long double)1.0 / nPixCount);

				imgBackUp.m_pBits[0][i][j] = (UINT)product[0];
				imgBackUp.m_pBits[1][i][j] = (UINT)product[1];
				imgBackUp.m_pBits[2][i][j] = (UINT)product[2];
			}
		}
		break;
	case 2: //谐波均值
		for (UINT i = nPara; i < h - nPara; ++i) //循环次数只有h-2
		{
			for (UINT j = nPara; j < w - nPara; ++j)//循环w-2次
			{
				long double sum[3] = { 0,0,0 };//三个通道分别的和
				//应用模板

				for (UINT m = i - nPara; m <= i + nPara; m++)
				{
					for (UINT n = j - nPara; n <= j + nPara; n++)
					{
						sum[0] += (long double)1 / (eps + imgFilled.m_pBits[0][m][n]);
						sum[1] += (long double)1 / (eps + imgFilled.m_pBits[1][m][n]);
						sum[2] += (long double)1 / (eps + imgFilled.m_pBits[2][m][n]);
					}
				}
				// 				for (int i=0;i!=3;++i)
				// 				{
				// 					cout << sum[i] << endl;
				// 				}
				sum[0] = nPixCount / sum[0];
				sum[1] = nPixCount / sum[1];
				sum[2] = nPixCount / sum[2];

				imgBackUp.m_pBits[0][i][j] = (BYTE)sum[0];
				imgBackUp.m_pBits[1][i][j] = (BYTE)sum[1];
				imgBackUp.m_pBits[2][i][j] = (BYTE)sum[2];
			}
		}
		break;
	case 3://逆谐波均值滤波
		for (UINT i = nPara; i < h - nPara; ++i) //循环次数只有h-2
		{
			for (UINT j = nPara; j < w - nPara; ++j)//循环w-2次
			{
				long double sum[3] = { 0,0,0 };//三个通道分别的和,分子
				long double sum1[3] = { 0,0,0 };//三个通道分别的和,分母
				//应用模板

				for (UINT i1 = i - nPara; i1 <= i + nPara; i1++)
				{
					for (UINT j1 = j - nPara; j1 <= j + nPara; j1++)
					{
						sum[0] += pow(imgFilled.m_pBits[0][i1][j1], (long double)m_dFTAVGNiXieQ + 1);
						sum[1] += pow(imgFilled.m_pBits[1][i1][j1], (long double)m_dFTAVGNiXieQ + 1);
						sum[2] += pow(imgFilled.m_pBits[2][i1][j1], (long double)m_dFTAVGNiXieQ + 1);

						sum1[0] += pow(imgFilled.m_pBits[0][i1][j1], (long double)m_dFTAVGNiXieQ);
						sum1[1] += pow(imgFilled.m_pBits[1][i1][j1], (long double)m_dFTAVGNiXieQ);
						sum1[2] += pow(imgFilled.m_pBits[2][i1][j1], (long double)m_dFTAVGNiXieQ);
					}
				}
				// 				for (int i=0;i!=3;++i)
				// 				{
				// 					cout << sum[i] << endl;
				// 				}

				imgBackUp.m_pBits[0][i][j] = (BYTE)(sum[0] / (sum1[0] + eps));
				imgBackUp.m_pBits[1][i][j] = (BYTE)(sum[1] / (sum1[1] + eps));
				imgBackUp.m_pBits[2][i][j] = (BYTE)(sum[2] / (sum1[2] + eps));
			}
		}
		break;
	}

	imgBackUp.RemoveFillTo(m_ImageAfter, m_nFtAvgPara);
	imgBackUp.Destroy();
	imgFilled.Destroy();

	UpdateState();
}


void CImage_ProcessingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//实时显示鼠标坐标值
	CString strMousePos, strMouseRGB, strMouseHSI;
	strMousePos.Format(_T("x:%d,y:%d"), point.x, point.y);
	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(strMousePos);
	//std::cout << sz.cx << std::endl;
	//CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;  
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFCStatusBar *pStatusBar = (CMFCStatusBar *)&(pFrame->m_wndStatusBar);

	int index = pStatusBar->CommandToIndex(ID_INDICATOR_MOUSE_POS);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_MOUSE_POS, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strMousePos);

	//实时显示RGB值
	//CDC *pDC = GetDC();
	COLORREF color = dc.GetPixel(point.x, point.y);
	int nCb = (color & 0x00ff0000) >> 16;
	int nCg = (color & 0x0000ff00) >> 8;
	int nCr = color & 0x000000ff;
	strMouseRGB.Format(_T("R:%d,G:%d,B:%d"), nCr, nCg, nCb);
	sz = dc.GetTextExtent(strMouseRGB);
	index = pStatusBar->CommandToIndex(ID_INDICATOR_RGB);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_RGB, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strMouseRGB);

	//实时显示HSI值 H在0-360之间，S在0-100之间，I在0-255之间
	/*double eps = 1e-20;
	double num1 = 0.5*((nCr-nCg)+(nCr-nCb));
	double num2 = sqrt(pow(double(nCr-nCg),2) + (nCr-nCb)*(nCg-nCb)+eps);
	double theta = acos(num1/num2)*(180/pi);
	double nCh = theta;

	if (nCb>nCg)
	{
		nCh = 360-theta;
	}

	DWORD dwMinRGB = ((nCr<nCg ? nCr : nCg) < nCb ? (nCr<nCg ? nCr : nCg) : nCb);
	int nCs = (1-3/(nCr+nCg+nCb+eps)*dwMinRGB)*100;
	int nCi = (nCr+nCb+nCg)/3;*/

	/*double dCh,dCs,dCi;*/
	int dCh, dCs, dCi;
	m_com.ChangedColourFromRGBStandardtoHSL(nCr, nCg, nCb, &dCh, &dCs, &dCi);

	// 	DWORD nCr1,nCg1,nCb1;
	// 	m_com.ChangedColourFromHSLStandardtoRGB(dCh,dCs,dCi,&nCr1,&nCg1,&nCb1);

		/*cout << dCh << endl;*/
	strMouseHSI.Format(_T("H:%d,S:%d,I:%d"), dCh, dCs, int(dCi*2.55));

	/*strMouseHSI.Format(_T("H:%d,S:%d,I:%d"),(int)(dCh*360.0),(int)(dCs*100.0),(int)dCi);*/
	//strMouseHSI.Format(_T("H:%d,S:%d,I:%d"),nCr1,nCg1,nCb1);
	sz = dc.GetTextExtent(strMouseHSI);
	index = pStatusBar->CommandToIndex(ID_INDICATOR_HSI);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_HSI, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strMouseHSI);



	if (m_bIsImgSEGing && m_bImgSegRectSelectStart)
	{
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *pOldBrush;
		pOldBrush = dc.SelectObject(pBrush);

		dc.Rectangle(CRect(m_ptImgSegOrigin.x, m_ptImgSegOrigin.y, point.x, point.y));
		dc.SelectObject(pOldBrush);
		CPoint pt1, pt2;
		pt1.x = m_ptImgSegOrigin.x + 1;
		pt1.y = m_ptImgSegOrigin.y + 1;
		pt2.x = point.x - 1;
		pt2.y = point.y - 1;
		// 
		// 		cout << m_ptImgSegOrigin.x << "," << m_ptImgSegOrigin.y << " " << point.x << "," << point.y << endl;
		// 		cout << pt1.x << "," << pt1.y << " " << pt2.x << "," << pt2.y << endl;
				/*ReleaseDC(pDC);*/
		InvalidateRect(CRect(pt1, pt2)); //更新内部区域，保留矩形框的显示
	}

	int nPenWidth = 2;
	if (m_nDrawType != -1 && m_bIsLBDown && CRect(CPoint(0, 0), CPoint(m_nWidth, m_nHeight)).PtInRect(point))
	{
		switch (m_nDrawType)
		{
		case 0:
		{
			CPen *pPen = new CPen(PS_SOLID, nPenWidth, RGB(255, 0, 0));
			CPen *pOldPen = dc.SelectObject(pPen);
			dc.MoveTo(m_ptDrawOrigin);
			dc.LineTo(point);
			if (point.x > m_rectDraw.right)
				m_rectDraw.right = point.x;
			if (point.y > m_rectDraw.bottom)
				m_rectDraw.bottom = point.y;
			if (point.x < m_rectDraw.left)
				m_rectDraw.left = point.x;
			if (point.y < m_rectDraw.top)
				m_rectDraw.top = point.y;

			dc.SelectObject(pOldPen);
			m_ptDrawOrigin = point;
			delete pPen;
			break;
		}

		case 1: //画圆  (这个方式不行,更新太多数据)
		{
			//CPen *pPen = new CPen(PS_SOLID,nPenWidth,RGB(255,0,0));
			//CPen *pOldPen = dc.SelectObject(pPen);

			//CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			//CBrush *pOldBrush = dc.SelectObject(pBrush);


			//dc.Ellipse(&CRect(m_ptDrawOrigin,point));
			//if (point.x > m_rectDraw.right)
			//	m_rectDraw.right = point.x;
			//if (point.y > m_rectDraw.bottom)
			//	m_rectDraw.bottom = point.y;
			//if (point.x < m_rectDraw.left)
			//	m_rectDraw.left = point.x;
			//if (point.y < m_rectDraw.top)
			//	m_rectDraw.top = point.y;


			//CPoint pt1,pt2;
			//pt1.x = m_ptDrawOrigin.x+10;
			//pt1.y = m_ptDrawOrigin.y+10;
			//pt2.x = point.x-10;
			//pt2.y = point.y-10;

			//InvalidateRect(CRect(pt1,pt2)); //更新内部区域，保留矩形框的显示

			//dc.SelectObject(pOldPen);
			//dc.SelectObject(pOldBrush);
			////m_ptDrawOrigin = point;
			//delete pPen;
			//delete pBrush;
			break;
		}
		default:
			break;
		}

	}

	CScrollView::OnMouseMove(nFlags, point);
}

BOOL CImage_ProcessingView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	//std::cout << "mousewheel，nFlags:"<<nFlags<<",zDelta:"<<zDelta<<",Point:" << pt.x<<","<<pt.y << std::endl;
	//向上为正，向下为负，120我这里是

	if (m_Image.IsNull())
		return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);
	if (m_ImageInterpolationBackUp.IsNull())
		m_ImageAfter.CopyTo(m_ImageInterpolationBackUp);

	m_ImageInterpolationBackUp.CopyTo(m_ImageAfter);

	if (nFlags == MK_CONTROL && zDelta < 0)
	{
		m_nInterPolationHeight = (UINT)(m_nHeight*0.9);
		m_nInterPolationWidth = (UINT)(m_nWidth*0.9);
		OnInterpolationNearest();
	}
	else if (nFlags == MK_CONTROL && zDelta > 0)
	{
		m_nInterPolationHeight = (UINT)(m_nHeight*1.1);
		m_nInterPolationWidth = (UINT)(m_nWidth*1.1);
		OnInterpolationNearest();
	}
	else if (nFlags == MK_SHIFT && zDelta < 0) //朝右
	{
		//OnHScroll()
		int minpos, maxpos, curpos;
		GetScrollRange(SB_HORZ, &minpos, &maxpos);
		maxpos = GetScrollLimit(SB_HORZ);
		curpos = GetScrollPos(SB_HORZ);
		if (curpos + 10 < maxpos)
		{
			SetScrollPos(SB_HORZ, curpos + 10);
			Invalidate(FALSE);
			//UpdateState(false);
		}
		/*	cout << npos << endl;*/
	}
	else if (nFlags == MK_SHIFT && zDelta > 0) //朝左
	{
		int minpos, maxpos, curpos;
		GetScrollRange(SB_HORZ, &minpos, &maxpos);
		maxpos = GetScrollLimit(SB_HORZ);
		curpos = GetScrollPos(SB_HORZ);
		if (curpos - 10 > minpos)
		{
			SetScrollPos(SB_HORZ, curpos - 10);
			Invalidate(FALSE);

			//UpdateState(false);
		}
	}
	else
	{

	}

	//暂时先不实现这个
	//OnResdwon();
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CImage_ProcessingView::OnFilterMedium()
{
	// TODO: Add your command handler code here

	//MyImage_类中新增CopyTo，Create，BorderFillTo，RemoveFillTo方法，
	//修改Destroy方法和MyImage析构函数

	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();
	int nFtPara = m_nFtMidPara;
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ imgBackUp, imgFilled;
	cout << m_nFtMidFillMode << endl;
	//复制备份,防止产生迭代,处理过程中对此图处理，完成后再拷贝回ImgAfter
	m_ImageAfter.BorderFillTo(imgFilled, m_nFtMidPara, m_nFtMidFillMode);
	m_ImageAfter.BorderFillTo(imgBackUp, m_nFtMidPara, m_nFtMidFillMode);


	//测试边缘填充效果

	//m_ImageAfter.BorderFillTo(imgFilled,m_nFtMidPara,m_nFtMidFillMode);
	//m_ImageAfter.BorderFillTo(imgBackUp,m_nFtMidPara,m_nFtMidFillMode);
	//imgFilled.CopyTo(m_ImageAfter);
	//m_bIsProcessed = TRUE;
	//Invalidate(1);
	//return;

	UINT w = imgFilled.GetWidth(); //重新获得填充后的长和宽
	UINT h = imgFilled.GetHeight();

	vector<UINT>  nVecPixelB; //新建三个容器，分别用来容纳三个通道用来排序的像素
	vector<UINT>  nVecPixelG;
	vector<UINT>  nVecPixelR;

	UINT nMidPos = (nFtPara * 2 + 1) * (nFtPara * 2 + 1) / 2; //获得模板中心位置

	//对整张填充后的图进行遍历
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数h-nFtPara
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-nFtPara次
		{
			nVecPixelB.clear();
			nVecPixelG.clear();
			nVecPixelR.clear();
			//对每个像素把模板对应的领域像素添加进容器中
			for (UINT m = i - nFtPara; m <= i + nFtPara; ++m)
			{
				for (UINT n = j - nFtPara; n <= j + nFtPara; ++n)
				{
					nVecPixelB.push_back(imgFilled.m_pBits[0][m][n]); //B
					nVecPixelG.push_back(imgFilled.m_pBits[1][m][n]); //G
					nVecPixelR.push_back(imgFilled.m_pBits[2][m][n]); //R
				}
			}
			sort(nVecPixelB.begin(), nVecPixelB.end()); //进行排序
			sort(nVecPixelG.begin(), nVecPixelG.end());
			sort(nVecPixelR.begin(), nVecPixelR.end());
			imgBackUp.m_pBits[0][i][j] = nVecPixelB[nMidPos]; //得到中值
			imgBackUp.m_pBits[1][i][j] = nVecPixelG[nMidPos];
			imgBackUp.m_pBits[2][i][j] = nVecPixelR[nMidPos];
		}
	}
	imgBackUp.RemoveFillTo(m_ImageAfter, m_nFtMidPara); //去除边缘填充
	imgBackUp.Destroy(); //销毁中间变量的图像
	imgFilled.Destroy();
	ldTimeEnd = GetTickCount();
	std::cout << "中值滤波(标准库排序)完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s"
		<< " ,模板大小：" << (nFtPara * 2 + 1) << "x" << (nFtPara * 2 + 1) << std::endl;
	UpdateState();
}


void CImage_ProcessingView::OnFtmidSet()
{
	// TODO: Add your command handler code here
	if (m_dlgFtMidSet.DoModal() == IDOK)
	{
		m_nFtMidPara = m_dlgFtMidSet.m_nFtMidPara;
		m_nFtMidFillMode = m_dlgFtMidSet.m_FtMidFillMode;
		m_nFTSortMode = m_dlgFtMidSet.m_nFTSortMode;
	}
}


void CImage_ProcessingView::OnFtavgSet()
{
	// TODO: Add your command handler code here
	if (m_dlgFtAvgSet.DoModal() == IDOK)
	{
		m_nFtAvgPara = m_dlgFtAvgSet.m_nFtAvgPara;
		m_nFtAvgFillMode = m_dlgFtAvgSet.m_FtAvgFillMode;
		m_nFTAVGMode = m_dlgFtAvgSet.m_nFTAVGMode;
		m_dFTAVGNiXieQ = m_dlgFtAvgSet.m_dFTAVGNiXieQ;
	}
}


//int round(double value) //vs2015版本的math库自带了round函数不需要重新自定义了
//{
//	double num1 = value - floor(value);
//	if (num1>=0.5)
//	{
//		return (int)floor(value) + 1;
//	}
//	else return (int)floor(value);
//}

void CImage_ProcessingView::OnHistEqRGB()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
	{
		return;
	}
	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	CalcHistPorb(1);//为0是计算直方图用的，为1是计算均衡化用的

	//开始直方图均衡化
	std::map<UINT, UINT> nMapTableR, nMapTableG, nMapTableB;
	double dHistSum = 0.0; UINT nIndex;
	//R分量
	for (auto it = m_dMapHistR.begin(); it != m_dMapHistR.end(); ++it)
	{
		dHistSum += it->second;
		nIndex = it->first;

		nMapTableR[nIndex] = (UINT)round(dHistSum * 255);
	}

	//G分量
	dHistSum = 0.0;
	for (auto it = m_dMapHistG.begin(); it != m_dMapHistG.end(); ++it)
	{
		dHistSum += it->second;
		nIndex = it->first;
		nMapTableG[nIndex] = (UINT)round(dHistSum * 255);
	}
	//B分量
	dHistSum = 0.0;
	for (auto it = m_dMapHistB.begin(); it != m_dMapHistB.end(); ++it)
	{
		dHistSum += it->second;
		nIndex = it->first;
		nMapTableB[nIndex] = (UINT)round(dHistSum * 255);
	}

	UINT nTemp;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			nTemp = m_ImageAfter.m_pBits[0][j][k];
			m_ImageAfter.m_pBits[0][j][k] = nMapTableB[nTemp];//b   用循环访问图像的像素值
			nTemp = m_ImageAfter.m_pBits[1][j][k];
			m_ImageAfter.m_pBits[1][j][k] = nMapTableG[nTemp];//g   用循环访问图像的像素值
			nTemp = m_ImageAfter.m_pBits[2][j][k];
			m_ImageAfter.m_pBits[2][j][k] = nMapTableR[nTemp];//r   用循环访问图像的像素值
		}
	}

	UpdateState();
}


void CImage_ProcessingView::OnCalcEntropy()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	CalcHistPorb(1); //除以总像素归一化

	double dEntropy = 0.0;

	for (auto it = m_dMapHistR.begin(); it != m_dMapHistR.end(); ++it)
	{
		dEntropy += (it->second * log((1 / (it->second))));
	}
	cout << "R通道信息熵为：" << dEntropy << endl;

	dEntropy = 0.0;
	for (auto it = m_dMapHistG.begin(); it != m_dMapHistG.end(); ++it)
	{
		dEntropy += (it->second * log((1 / (it->second))));
	}
	cout << "G通道信息熵为：" << dEntropy << endl;

	dEntropy = 0.0;
	for (auto it = m_dMapHistB.begin(); it != m_dMapHistB.end(); ++it)
	{
		dEntropy += (it->second * log((1 / (it->second))));
	}
	cout << "B通道信息熵为：" << dEntropy << endl;

}


void CImage_ProcessingView::OnCalcGradient()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	double dGradientR = 0.0;
	double dGradientG = 0.0;
	double dGradientB = 0.0;

	for (UINT j = 0; j < m_nHeight - 1; j++)
	{
		for (UINT k = 0; k < m_nWidth - 1; k++)
		{
			dGradientB += abs(m_ImageAfter.m_pBits[0][j][k] - m_ImageAfter.m_pBits[0][j + 1][k]);
			dGradientB += abs(m_ImageAfter.m_pBits[0][j][k] - m_ImageAfter.m_pBits[0][j][k + 1]);

			dGradientG += abs(m_ImageAfter.m_pBits[1][j][k] - m_ImageAfter.m_pBits[1][j + 1][k]);
			dGradientG += abs(m_ImageAfter.m_pBits[1][j][k] - m_ImageAfter.m_pBits[1][j][k + 1]);

			dGradientR += abs(m_ImageAfter.m_pBits[2][j][k] - m_ImageAfter.m_pBits[2][j + 1][k]);
			dGradientR += abs(m_ImageAfter.m_pBits[2][j][k] - m_ImageAfter.m_pBits[2][j][k + 1]);
		}
	}
	dGradientB /= ((m_nHeight - 1)*(m_nWidth - 1));
	dGradientG /= ((m_nHeight - 1)*(m_nWidth - 1));
	dGradientR /= ((m_nHeight - 1)*(m_nWidth - 1));

	cout << "R通道平均梯度为：" << dGradientR << endl;
	cout << "G通道平均梯度为：" << dGradientG << endl;
	cout << "B通道平均梯度为：" << dGradientB << endl;

}


void CImage_ProcessingView::OnIdFilterMedium2()
{
	// TODO: Add your command handler code here
	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();
	int nFtPara = m_nFtMidPara;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ imgBackUp, imgFilled;
	//复制备份,防止产生迭代,处理过程中对此图处理，完成后再拷贝回ImgAfter
	m_ImageAfter.BorderFillTo(imgFilled, m_nFtMidPara, m_nFtMidFillMode);
	m_ImageAfter.BorderFillTo(imgBackUp, m_nFtMidPara, m_nFtMidFillMode);


	//测试边缘填充效果
	//m_ImageAfter.BorderFillTo(imgFilled,m_nFtMidPara,2);
	//m_ImageAfter.BorderFillTo(imgBackUp,m_nFtMidPara,2);
	//imgFilled.CopyTo(m_ImageAfter);
	//m_bIsProcessed = TRUE;
	//Invalidate(1);
	//return;

	UINT w = imgFilled.GetWidth(); //重新获得填充后的长和宽
	UINT h = imgFilled.GetHeight();

	vector<UINT>  nVecPixelB; //新建三个容器，分别用来容纳三个通道用来排序的像素
	vector<UINT>  nVecPixelG;
	vector<UINT>  nVecPixelR;

	UINT nMidPos = (nFtPara * 2 + 1) * (nFtPara * 2 + 1) / 2; //获得模板中心位置

	for (UINT i = nFtPara; i < h - nFtPara; i += (nFtPara * 2 + 1)) //循环次数h-nFtPara
	{
		for (UINT j = nFtPara; j < w - nFtPara; j += (nFtPara * 2 + 1))//循环w-nFtPara次
		{
			nVecPixelB.clear();
			nVecPixelG.clear();
			nVecPixelR.clear();
			//对每个像素把模板对应的领域像素添加进容器中
			for (UINT m = i - nFtPara; m <= i + nFtPara; ++m)
			{
				for (UINT n = j - nFtPara; n <= j + nFtPara; ++n)
				{
					nVecPixelB.push_back(imgFilled.m_pBits[0][m][n]); //B
					nVecPixelG.push_back(imgFilled.m_pBits[1][m][n]); //G
					nVecPixelR.push_back(imgFilled.m_pBits[2][m][n]); //R
				}
			}
			sort(nVecPixelB.begin(), nVecPixelB.end()); //进行排序
			sort(nVecPixelG.begin(), nVecPixelG.end());
			sort(nVecPixelR.begin(), nVecPixelR.end());
			for (int c = 0 - nFtPara; c != nFtPara + 1; ++c) //行
			{
				for (int r = 0 - nFtPara; r != nFtPara + 1; ++r)//列
				{
					imgBackUp.m_pBits[0][i + c][j + r] = nVecPixelB[nMidPos]; //得到中值
					imgBackUp.m_pBits[1][i + c][j + r] = nVecPixelG[nMidPos];
					imgBackUp.m_pBits[2][i + c][j + r] = nVecPixelR[nMidPos];
				}
			}
			/*i+=3;*/
		}
		/*j+=3;*/
	}

	imgBackUp.RemoveFillTo(m_ImageAfter, m_nFtMidPara); //去除边缘填充
	imgBackUp.Destroy(); //销毁中间变量的图像
	imgFilled.Destroy();

	ldTimeEnd = GetTickCount();
	std::cout << "中值滤波(取近似)完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s"
		<< " ,模板大小：" << (nFtPara * 2 + 1) << "x" << (nFtPara * 2 + 1) << std::endl;
	UpdateState();
}





void CImage_ProcessingView::OnFilterMediumvecsort()
{
	// TODO: Add your command handler code here

	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();
	int nFtPara = m_nFtMidPara;
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ imgBackUp, imgFilled;
	//复制备份,防止产生迭代,处理过程中对此图处理，完成后再拷贝回ImgAfter
	m_ImageAfter.BorderFillTo(imgFilled, m_nFtMidPara, m_nFtMidFillMode);
	m_ImageAfter.BorderFillTo(imgBackUp, m_nFtMidPara, m_nFtMidFillMode);


	//测试边缘填充效果
	//m_ImageAfter.BorderFillTo(imgFilled,m_nFtMidPara,2);
	//m_ImageAfter.BorderFillTo(imgBackUp,m_nFtMidPara,2);
	//imgFilled.CopyTo(m_ImageAfter);
	//m_bIsProcessed = TRUE;
	//Invalidate(1);
	//return;

	UINT w = imgFilled.GetWidth(); //重新获得填充后的长和宽
	UINT h = imgFilled.GetHeight();

	vector<UINT>  nVecPixelB; //新建三个容器，分别用来容纳三个通道用来排序的像素
	vector<UINT>  nVecPixelG;
	vector<UINT>  nVecPixelR;

	UINT nMidPos = (nFtPara * 2 + 1) * (nFtPara * 2 + 1) / 2; //获得模板中心位置

	//对整张填充后的图进行遍历
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数h-nFtPara
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-nFtPara次
		{
			nVecPixelB.clear();
			nVecPixelG.clear();
			nVecPixelR.clear();
			//对每个像素把模板对应的领域像素添加进容器中
			for (UINT m = i - nFtPara; m <= i + nFtPara; ++m)
			{
				for (UINT n = j - nFtPara; n <= j + nFtPara; ++n)
				{
					nVecPixelB.push_back(imgFilled.m_pBits[0][m][n]); //B
					nVecPixelG.push_back(imgFilled.m_pBits[1][m][n]); //G
					nVecPixelR.push_back(imgFilled.m_pBits[2][m][n]); //R
				}
			}
			// 			sort(nVecPixelB.begin(),nVecPixelB.end()); //进行排序
			// 			sort(nVecPixelG.begin(),nVecPixelG.end());
			// 			sort(nVecPixelR.begin(),nVecPixelR.end());
			vecSort(nVecPixelB); vecSort(nVecPixelG); vecSort(nVecPixelR);

			imgBackUp.m_pBits[0][i][j] = nVecPixelB[nMidPos]; //得到中值
			imgBackUp.m_pBits[1][i][j] = nVecPixelG[nMidPos];
			imgBackUp.m_pBits[2][i][j] = nVecPixelR[nMidPos];
		}
	}
	imgBackUp.RemoveFillTo(m_ImageAfter, m_nFtMidPara); //去除边缘填充
	imgBackUp.Destroy(); //销毁中间变量的图像
	imgFilled.Destroy();
	ldTimeEnd = GetTickCount();
	std::cout << "中值滤波(冒泡)完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s"
		<< " ,模板大小：" << (nFtPara * 2 + 1) << "x" << (nFtPara * 2 + 1) << std::endl;
	UpdateState();

}


void CImage_ProcessingView::OnFilterMediumHist()
{
	// TODO: Add your command handler code here


	//MyImage_类中新增CopyTo，Create，BorderFillTo，RemoveFillTo方法，
	//修改Destroy方法和MyImage析构函数


	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();
	int nFtPara = m_nFtMidPara;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ imgBackUp, imgFilled;
	//复制备份,防止产生迭代,处理过程中对此图处理，完成后再拷贝回ImgAfter
	m_ImageAfter.BorderFillTo(imgFilled, nFtPara, m_nFtMidFillMode);
	m_ImageAfter.BorderFillTo(imgBackUp, nFtPara, m_nFtMidFillMode);

	UINT w = imgFilled.GetWidth(); //重新获得填充后的长和宽
	UINT h = imgFilled.GetHeight();

	map<UINT, UINT> nMapHistR, nMapHistG, nMapHistB;
	vector<map<UINT, UINT>> nMapHist; //改成vector来装载三个通道
	nMapHist.push_back(nMapHistB);
	nMapHist.push_back(nMapHistG);
	nMapHist.push_back(nMapHistR);

	UINT nMidPos = (nFtPara * 2 + 1) * (nFtPara * 2 + 1) / 2; //获得模板中心位置

	UINT nMidValue[3] = { 0,0,0 }, nLowerCount[3] = { 0,0,0 }, nTempPre[3] = { 0,0,0 }, nTempNext[3] = { 0,0,0 };//初始化数组
	//对整张填充后的图进行遍历
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数h-nFtPara，行
	{
		nMapHist[0].clear();
		nMapHist[1].clear();
		nMapHist[2].clear();

		calcHistLocal(imgFilled, nMapHist[0], nMapHist[1], nMapHist[2], i, nFtPara, nFtPara); //计算第一次的直方图(第一次刚好y值就等于nFtPara)
		//printMap(nMapHist);

		for (UINT ch = 0; ch != 3; ++ch) //三通道
		{
			//cout << nMapHist[0].size() << endl;
			nMidValue[ch] = getMapMid(nMapHist[ch]);
			imgBackUp.m_pBits[ch][i][nFtPara] = nMidValue[ch]; //第一次计算过程直接赋中值
			nLowerCount[ch] = getMapLowerCount(nMapHist[ch], nMidValue[ch]); //计算小于中值的灰度级数目
		}

		for (UINT f = 0; f != w - (nFtPara * 2 + 1); ++f) //模板开始在某一行进行平移
		{
			for (UINT g = 0; g != (nFtPara * 2 + 1); ++g) //移出一列和加入一列,分别对着第一列和最后一列计算
			{
				for (UINT c = 0; c != 3; ++c)
				{
					nTempPre[c] = imgFilled.m_pBits[c][g + i - nFtPara][f];
					nTempNext[c] = imgFilled.m_pBits[c][g + i - nFtPara][f + (nFtPara * 2 + 1)];
					if (nTempPre[c] <= nMidValue[c]) //移出的灰度小于mid，则nLowerCount-1
						nLowerCount[c] -= 1;
					if (nTempNext[c] <= nMidValue[c])//移入的灰度小于mid，则nLowerCount+1				
						nLowerCount[c] += 1;
					--nMapHist[c][nTempPre[c]]; //直方图减掉移出的灰度
					++nMapHist[c][nTempNext[c]];//直方图加上移入的灰度	
				}
			}

			//接下来进入while循环，纠正中值位置

			for (UINT ch = 0; ch != 3; ++ch)
			{
				while (nLowerCount[ch] > nMidPos) //右边数目不够，中值偏大，则让中值减一 ，最终达到两边相等则是中值
				{
					nLowerCount[ch] -= nMapHist[ch][nMidValue[ch]];
					--nMidValue[ch];
				}

				while (nLowerCount[ch] < nMidPos) //左边数目不够，中值偏小，则让中值加一 ，最终达到两边相等则是中值
				{
					++nMidValue[ch];
					nLowerCount[ch] += nMapHist[ch][nMidValue[ch]];
				}
				imgBackUp.m_pBits[ch][i][nFtPara + f + 1] = nMidValue[ch]; //把产生的中值进行赋值
			}
		}

	}

	imgBackUp.RemoveFillTo(m_ImageAfter, nFtPara); //去除边缘填充
	imgBackUp.Destroy(); //销毁中间变量的图像
	imgFilled.Destroy();
	ldTimeEnd = GetTickCount();
	std::cout << "中值滤波(直方图)完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s"
		<< " ,模板大小：" << (nFtPara * 2 + 1) << "x" << (nFtPara * 2 + 1) << std::endl;
	UpdateState();

}


UINT getMapMid(map<UINT, UINT> &nMap)
{
	vector<UINT> nVec;
	UINT nMidPos = nMap.size() / 2; //奇数正确，偶数的话有偏差
	for (auto it = nMap.begin(); it != nMap.end(); ++it)
	{
		nVec.push_back(it->first);
	}
	sort(nVec.begin(), nVec.end());

	return nVec[nMidPos];
}

template<class T>
void printMap(map<T, T> &nMap)
{
	cout << "this is a new map:" << endl;
	for (auto it = nMap.begin(); it != nMap.end(); ++it)
	{
		cout << it->first << "," << it->second << endl;
	}
}

void CImage_ProcessingView::calcHistLocal(MyImage_ &img, map<UINT, UINT> &nMapHistB,
	map<UINT, UINT> &nMapHistG,
	map<UINT, UINT> &nMapHistR,
	UINT x, UINT y, int nFtPara)
{
	UINT nTemp = 0;
	for (UINT i = x - nFtPara; i <= x + nFtPara; ++i) //行
	{
		for (UINT j = y - nFtPara; j <= y + nFtPara; ++j) //列
		{
			nTemp = img.m_pBits[0][i][j];
			++nMapHistB[nTemp];
			nTemp = img.m_pBits[1][i][j];
			++nMapHistG[nTemp];
			nTemp = img.m_pBits[2][i][j];
			++nMapHistR[nTemp];
		}
	}
}

UINT getMapLowerCount(map<UINT, UINT> &nMapHist, UINT nMidValue)
{
	UINT nLowerCount = 0;
	for (auto it = nMapHist.begin(); it != nMapHist.end(); ++it)
	{
		if (it->first <= nMidValue)
		{
			nLowerCount += it->second;
		}
	}
	return nLowerCount;
}



template <class T> //返回vector最大值
T maxVecValue(vector<T> vecT)
{
	std::sort(vecT.begin(), vecT.end());
	return	vecT.back();
}

template <class T> //vector冒泡排序
void vecSort(vector<T> &vecT)
{
	for (size_t cmpnum = vecT.size() - 1; cmpnum != 0; --cmpnum)
	{
		for (size_t i = 0; i != cmpnum; ++i)
		{
			if (vecT[i] > vecT[i + 1])
			{
				T temp;
				temp = vecT[i];
				vecT[i] = vecT[i + 1];
				vecT[i + 1] = temp;
			}
		}
	}
}

void CImage_ProcessingView::OnFft()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull()) //仍然是处理After里的数据，原图备份
		m_Image.CopyTo(m_ImageAfter);

	int h_extend = 1;//图像进行扩展,寻找2的幂次方
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;

	while ((UINT)h_extend < m_nHeight)
	{
		h_extend *= 2;
		h_index++;
	}

	while ((UINT)w_extend < m_nWidth)
	{
		w_extend *= 2;
		w_index++;
	}

	if (!m_bIsFFTed) //若未进行过变换，初始化内存
		m_pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	/*unique_ptr<complex<double> []> */ //这里不能用uniqptr，若设为成员变量，没法初始化(大小不确定)，若设为局部变量，此范围一完成就销毁了
	MyImage_ imgGrayed;//先灰度化
	doToGray(m_ImageAfter, imgGrayed);
	imgGrayed.CopyTo(m_ImageAfter);


	complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间
	complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//后面带个括号则全部初始化为默认值

	for (UINT i = 0; i < m_nHeight; i++)//把图像的值传给pTD
	{
		for (UINT j = 0; j < m_nWidth; j++)
		{
			pTD[i*w_extend + j] = m_ImageAfter.m_pBits[0][i][j] * (pow(-1.0, (int)(i + j)));//把频谱搬移到中心 
		}
	}

	for (int i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		m_com.FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];//////////////////////////////////////////错误点！！！！
		}
	}

	for (int i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		m_com.FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}

	//到这一步  pTD里存的就是傅里叶变换的全部数据

	//在m_pFD中存一份频谱信息
	for (int i = 0; i < h_extend*w_extend; i++)//把所有频域数据拷贝到m_pfd进行存储
	{
		m_pFD[i] = pTD[i];
	}

	//开始频谱图像的显示
	if (!m_ImageFT.IsNull())
	{
		m_ImageFT.Destroy();
	}
	m_ImageFT.Create(w_extend, h_extend, 24);
	vector<double> dVecMag;
	for (int i = 0; i < h_extend; i++) //显示幅度谱
	{
		for (int j = 0; j < w_extend; j++)
		{
			double dMagnitude = sqrt(pTD[i*w_extend + j].real()*pTD[i*w_extend + j].real()
				+ pTD[i*w_extend + j].imag()*pTD[i*w_extend + j].imag());
			double dLogMgntd = log(1.0 + dMagnitude);
			dVecMag.push_back(dLogMgntd);
		}
	}
	//找到最大最小值
	//vector<double> temp(dVecMag.begin(),dVecMag.end());
	//sort(temp.begin(),temp.end());
	//double dMax = temp[temp.size()-1];
	//double dMin = temp[0];

	////归一化
	//for (auto it=dVecMag.begin();it!=dVecMag.end();++it)
	//{
	//	*it = ((*it)-dMin)/(dMax-dMin)*255;
	//}

	VecNormalized(dVecMag, 0.0, 255.0);

	for (int i = 0; i < h_extend; i++) //显示幅度谱
	{
		for (int j = 0; j < w_extend; j++)
		{
			double temp = dVecMag[i*w_extend + j];
			m_ImageFT.m_pBits[0][i][j] = (BYTE)temp;
			m_ImageFT.m_pBits[1][i][j] = (BYTE)temp;
			m_ImageFT.m_pBits[2][i][j] = (BYTE)temp;
		}
	}

	delete pTD; //一定要去掉，不然多占几十兆内存
	delete pFD;

	m_ImageFT.CopyTo(m_ImageToDlgShow);
	ShowImgInDlg(_T("傅里叶频谱："));

	m_bIsFFTed = true;
	ChangeScrollSize();
	UpdateState();
}

void CImage_ProcessingView::ChangeScrollSize()
{
	CSize sizeTotal;
	sizeTotal.cx = m_nWidth;
	sizeTotal.cy = m_nHeight; //起始时未打开图片的情况下 View客户区小于300x300就设置滚轮
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImage_ProcessingView::OnFftReverse()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull()) //仍然是处理After里的数据，原图备份
		return;

	if (!m_bIsFFTed)
	{
		MessageBox(_T("请先执行FFT操作！"));
		return;
	}

	CCommon com;

	int h_extend = 1;//图像进行扩展,寻找2的幂次方
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;

	while ((UINT)h_extend < m_nHeight)
	{
		h_extend *= 2;
		h_index++;
	}
	while ((UINT)w_extend < m_nWidth)
	{
		w_extend *= 2;
		w_index++;
	}

	//开始逆向变换
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间
	complex<double>* pFD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	for (int i = 0; i < h_extend; ++i)//把所有频域数据拷贝到pTD1
	{
		for (int j = 0; j < w_extend; ++j)
		{
			//pTD1[i*w_extend+j]=m_pFD[i*w_extend+j]*(pow(-1.0,(i+j)));
			pTD1[i*w_extend + j] = m_pFD[i*w_extend + j]; //应该在得到时域信号之后再来乘这个东西
		}
	}
	//让图像均值为0
	/*pTD1[h_extend/2*h_extend+w_extend/2] = 0.0;*/
	for (int i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		m_com.RFFT(&pTD1[w_extend*i], &pFD1[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = pFD1[w_extend*i + j];//////////////////////////////////////////错误点！！！！
		}
	}

	for (int i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		m_com.RFFT(&pTD1[h_extend*i], &pFD1[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pFD1[h_extend*j + i];
		}
	}

	for (int i = 0; i < h_extend; i++)//对变换结果乘以之前的频域平移因子，不然会出现之前的相位出错
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(i + j)));
		}
	}

	//到此pTD1存的就全部都是时域数据

	m_ImageAfter.Create(w_extend, h_extend, 24);//此时m_ImageAfter的宽度和长度还没有变化，因此要重新建一张图
	for (int i = 0; i < h_extend; i++) //显示反变换得到的图像
	{
		for (int j = 0; j < w_extend; j++)
		{
			double temp1 = pTD1[i*w_extend + j].real();
			/*			cout << temp1 << endl;*/
			BYTE temp = (BYTE)pTD1[i*w_extend + j].real();
			/*			cout << temp << endl;*/
						//这里必须加abs，不然出来的数据是一正一负，是否和前面的未乘-1de x+y次方有关？
						/*BYTE temp=abs(pTD1[i*w_extend+j].real());*/

			m_ImageAfter.m_pBits[0][i][j] = temp;
			m_ImageAfter.m_pBits[1][i][j] = temp;
			m_ImageAfter.m_pBits[2][i][j] = temp;
		}
	}
	delete pTD1; //一定要去掉，不然多占几十兆内存
	delete pFD1;
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();

	/*	m_bIsFFTed = false;*/
	ChangeScrollSize();
	UpdateState();

}


void CImage_ProcessingView::OnFilter()
{
	// TODO: Add your command handler code here

	//int nFtPara = 1;
	//
	//if(m_Image.IsNull())
	//	return;

	//if(m_ImageAfter.IsNull())
	//	m_Image.CopyTo(m_ImageAfter);

	////OnTogray(); //这里还是不判断m_bIsGrayed比较好
	//MyImage_ imgGrayed;//先灰度化
	//doToGray(m_ImageAfter,imgGrayed);
	//imgGrayed.CopyTo(m_ImageAfter);


	//MyImage_ imgBackUp,imgFilled;
	//m_ImageAfter.BorderFillTo(imgBackUp,nFtPara,2); //备份一下
	//m_ImageAfter.BorderFillTo(imgFilled,nFtPara,2);

	//UINT w = imgFilled.GetWidth();
	//UINT h = imgFilled.GetHeight();


	////for (int i=0;i!=h;++i)
	////{
	////	for (int j=0;j!=w;++j)
	////	{
	////		printf("%d,",imgBackUp.m_pBits[0][i][j]);
	////	}
	////	printf("\n");
	////}


	////值时

	////int tpl[3][3] = {{0,1,1},{-1,0,1},{-1,-1,0}};//prewitt左下右上对角
	////int tpl[3][3] = {{-1,-1,0},{-1,0,1},{0,1,1}};//prewitt左上右下对角

	//
	////int tpl[3][3] = {{-1,-1,-1},{0,0,0},{1,1,1}}; //prewitt 垂直

	////int tpl[3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}}; //prewitt 水平
	//
	////int tpl[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}}; //sobel 垂直
	//int tpl[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; //sobel 水平

	////int tpl[3][3] = {{1,1,1},{1,-8,1},{1,1,1}}; //laplace 各角度
	////int tpl[3][3] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};

	////int tpl[3][3] = {{-1,-1,-1},{2,2,2},{-1,-1,-1}}; //水平
	////int tpl[3][3] = {{-1,2,-1},{-1,2,-1},{-1,2,-1}};// 垂直
	////int tpl[3][3] = {{2,-1,-1},{-1,2,-1},{-1,-1,2}};//45

	///*int tpl[3][3] = {{0,1,0},{1,-4,1},{0,1,0}};*/

	///*int tpl[3][3] = {{0,-1,0},{-1,4,-1},{0,-1,0}};*/

	///*int tpl[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};*/

	///*int tpl[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};*/

	//for (UINT i=nFtPara;i<h-nFtPara;++i) //循环次数只有h-2
	//{
	//	for (UINT j=nFtPara;j<w-nFtPara;++j)//循环w-2次
	//	{
	//		int sum[3]={0,0,0};//三个通道分别的和
	//		//应用模板
	//		UINT r=0,c=0;
	//		for (UINT i1=i-nFtPara;i1<=i+nFtPara;i1++) //行
	//		{
	//			for (UINT j1=j-nFtPara;j1<=j+nFtPara;j1++) //列
	//			{
	//				int temp = tpl[r][c];
	//				//printf("%d\n",temp);
	//				sum[0]+=imgFilled.m_pBits[0][i1][j1]*temp;
	//				sum[1]+=imgFilled.m_pBits[1][i1][j1]*temp;
	//				sum[2]+=imgFilled.m_pBits[2][i1][j1]*temp;
	//				//printf("%d\n",imgFilled.m_pBits[0][i1][j1]);
	//				c++;
	//			}
	//			c=0;
	//			r++;
	//		}
	//		//取绝对值（能看到双边缘效应）
	//		//imgBackUp.m_pBits[0][i][j] = SaturateCast(abs(sum[0]),0,255);
	//		//imgBackUp.m_pBits[1][i][j] = SaturateCast(abs(sum[1]),0,255);
	//		//imgBackUp.m_pBits[2][i][j] = SaturateCast(abs(sum[2]),0,255);

	//		//取正值
	//		imgBackUp.m_pBits[0][i][j] = SaturateCast(sum[0],0,255);
	//		imgBackUp.m_pBits[1][i][j] = SaturateCast(sum[1],0,255);
	//		imgBackUp.m_pBits[2][i][j] = SaturateCast(sum[2],0,255);

	//	}
	//}
	//imgBackUp.RemoveFillTo(m_ImageAfter,nFtPara);
	//imgBackUp.Destroy();
	//imgFilled.Destroy();

	double tpl[9] = { -1,0,1,-2,0,2,-1,0,1 }; //sobel 竖直
	//double tpl[9] = {1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9};
	vector<double> vecTpl(tpl, tpl + 9);
	cout << vecTpl.size() << endl;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	m_ImageAfter.CopyTo(m_ImageTpl);
	CDlgFilterTplSet *pDlg = new CDlgFilterTplSet;
	pDlg->Create(IDD_DLG_FILTER_TPL_SET, NULL);
	pDlg->ShowWindow(SW_SHOW);

	return;

	MyImage_ imgTemp;
	//doImFilter(m_ImageAfter,vecTpl,imgTemp,2,true,true);
	doImFilter(m_ImageAfter, tpl, 3, imgTemp, 2, true, true);

	imgTemp.CopyTo(m_ImageAfter);

	UpdateState();

}

//若要传数组参数，则需要指定数组大小,中间采用的是MyImage_double类型,不用担心精度损失
void CImage_ProcessingView::doImFilter(const MyImage_ &srcImg, vector<double> &tpl,
	MyImage_ &dstImg, int nFillMode, bool bIsGrayed,
	bool bIsSaturated)
{
	//vector可以用double去初始化
	int nTplSize = (int)sqrt((double)tpl.size());

	if (nTplSize % 2 != 1)
	{
		AfxMessageBox(_T("滤波器模板必须大于为大于等于3的奇数！"));
		return;
	}

	int nFtPara = nTplSize / 2;

	if (srcImg.IsNull())
		return;

	if (dstImg.IsNull())
		srcImg.CopyTo(dstImg);

	MyImage_ imgTemp(srcImg); //不能对原图进行操作!

	MyImage_ imgGrayed;//先灰度化
	if (bIsGrayed)
	{
		doToGray(imgTemp, imgGrayed);
		imgGrayed.CopyTo(imgTemp);
	}

	MyImage_ imgFilled;

	imgTemp.BorderFillTo(imgFilled, nFtPara, nFillMode); //备份一下

	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();

	CMyImage_double dImgAfter;
	dImgAfter.Create(w, h, 0);

	//应用模板
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			double sum[3] = { 0,0,0 };//三个通道分别的和
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					double temp = tpl[r*nTplSize + c];
					//cout << temp << endl;
					//printf("%d\n",temp);
					sum[0] += imgFilled.m_pBits[0][i1][j1] * temp;
					sum[1] += imgFilled.m_pBits[1][i1][j1] * temp;
					sum[2] += imgFilled.m_pBits[2][i1][j1] * temp;
					//printf("%d\n",imgFilled.m_pBits[0][i1][j1]);
					c++;
				}
				c = 0;
				r++;
			}

			//取正值
			dImgAfter.m_pBits[0][i][j] = sum[0];
			dImgAfter.m_pBits[1][i][j] = sum[1];
			dImgAfter.m_pBits[2][i][j] = sum[2];

		}
	}
	//MyImage_Double类里来进行溢出处理
	if (bIsSaturated)
	{
		dImgAfter.RemoveFillTo(dstImg, nFtPara, true);
	}
	else
	{
		dImgAfter.RemoveFillTo(dstImg, nFtPara, false);
	}
	imgFilled.Destroy();
	imgTemp.Destroy();
}

void CImage_ProcessingView::doImFilter(const MyImage_ &srcImg, double *pTpl, int nTplSize,
	MyImage_ &dstImg, int nFillMode, bool bIsGrayed,
	bool bIsSaturated)
{
	if (nTplSize % 2 != 1 || nTplSize < 3)
	{
		AfxMessageBox(_T("滤波器模板必须大于为大于等于3的奇数！"));
		return;
	}

	int nFtPara = nTplSize / 2;

	if (srcImg.IsNull())
		return;

	if (dstImg.IsNull())
		srcImg.CopyTo(dstImg);

	MyImage_ imgTemp(srcImg); //不能对原图进行操作!

	MyImage_ imgGrayed;//先灰度化
	if (bIsGrayed)
	{
		doToGray(imgTemp, imgGrayed);
		imgGrayed.CopyTo(imgTemp);
	}

	MyImage_ imgFilled;

	imgTemp.BorderFillTo(imgFilled, nFtPara, nFillMode); //备份一下

	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();

	CMyImage_double dImgAfter;
	dImgAfter.Create(w, h, 0);

	//应用模板
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			double sum[3] = { 0,0,0 };//三个通道分别的和
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					double temp = pTpl[r*nTplSize + c];
					//cout << temp << endl;
					//printf("%d\n",temp);
					sum[0] += imgFilled.m_pBits[0][i1][j1] * temp;
					sum[1] += imgFilled.m_pBits[1][i1][j1] * temp;
					sum[2] += imgFilled.m_pBits[2][i1][j1] * temp;
					//printf("%d\n",imgFilled.m_pBits[0][i1][j1]);
					c++;
				}
				c = 0;
				r++;
			}

			//取正值
			dImgAfter.m_pBits[0][i][j] = sum[0];
			dImgAfter.m_pBits[1][i][j] = sum[1];
			dImgAfter.m_pBits[2][i][j] = sum[2];

		}
	}
	//MyImage_Double类里来进行溢出处理
	if (bIsSaturated)
	{
		dImgAfter.RemoveFillTo(dstImg, nFtPara, true);
	}
	else
	{
		dImgAfter.RemoveFillTo(dstImg, nFtPara, false);
	}

	imgFilled.Destroy();
	imgTemp.Destroy();
}


void CImage_ProcessingView::OnSharp()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	//预置一系列模板

	double tpl[9] = { 0,-1,0,-1,5,-1,0,-1,0 };
	//double tpl[9] = {-1,-1,-1,-1,9,-1,-1,-1,-1};
	MyImage_ imgTemp;
	doImFilter(m_ImageAfter, tpl, 3, imgTemp, 2, false, true);
	imgTemp.CopyTo(m_ImageAfter);
	UpdateState();

}


void CImage_ProcessingView::OnHistGdh()
{
	// TODO: Add your command handler code here

	// TODO: Add your command handler code here
	if (m_Image.IsNull())
	{
		return;
	}
	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	CalcHistPorb(1);//为0是计算直方图用的，为1是计算均衡化用的

	//开始直方图均衡化
	std::map<UINT, UINT> nMapTableR, nMapTableG, nMapTableB;
	double dHistSum = 0.0; UINT nIndex;
	//R分量
	for (auto it = m_dMapHistR.begin(); it != m_dMapHistR.end(); ++it)
	{
		dHistSum += it->second;
		nIndex = it->first;
		nMapTableR[nIndex] = (UINT)(dHistSum * 255);
	}

	//G分量
	dHistSum = 0.0;
	for (auto it = m_dMapHistG.begin(); it != m_dMapHistG.end(); ++it)
	{
		dHistSum += it->second;
		nIndex = it->first;
		nMapTableG[nIndex] = (UINT)(dHistSum * 255);
	}
	//B分量
	dHistSum = 0.0;
	for (auto it = m_dMapHistB.begin(); it != m_dMapHistB.end(); ++it)
	{
		dHistSum += it->second;
		nIndex = it->first;
		nMapTableB[nIndex] = (UINT)(dHistSum * 255);
	}

	UINT nTemp;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			nTemp = m_ImageAfter.m_pBits[0][j][k];
			m_ImageAfter.m_pBits[0][j][k] = nMapTableB[nTemp];//b   用循环访问图像的像素值
			nTemp = m_ImageAfter.m_pBits[1][j][k];
			m_ImageAfter.m_pBits[1][j][k] = nMapTableG[nTemp];//g   用循环访问图像的像素值
			nTemp = m_ImageAfter.m_pBits[2][j][k];
			m_ImageAfter.m_pBits[2][j][k] = nMapTableR[nTemp];//r   用循环访问图像的像素值
		}
	}

	UpdateState();
}


void CImage_ProcessingView::OnFilterIlpf()
{
	// TODO: Add your command handler code here
#define n1 m_nBLPFOrder

	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull()) //仍然是处理After里的数据，原图备份
		m_Image.CopyTo(m_ImageAfter);


	int h_extend = 1;//图像进行扩展,寻找2的幂次方
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;

	while ((UINT)h_extend < m_nHeight)
	{
		h_extend *= 2;
		h_index++;
	}
	while ((UINT)w_extend < m_nWidth)
	{
		w_extend *= 2;
		w_index++;
	}


	if (!m_bIsFFTed)//若还未进行傅里叶变换进行正变换
	{
		// 		if(!m_bIsGrayed)
		// 			OnTogray(); //先灰度化
		MyImage_ imgGrayed;//先灰度化
		doToGray(m_ImageAfter, imgGrayed);
		imgGrayed.CopyTo(m_ImageAfter);

		complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间并初始化
		complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

		for (UINT i = 0; i < m_nHeight; i++)//把图像的值传给pTD
		{
			for (UINT j = 0; j < m_nWidth; j++)
			{
				pTD[i*w_extend + j] = m_ImageAfter.m_pBits[0][i][j] * (pow(-1.0, (int)(i + j)));//把频谱搬移到中心 
			}
		}

		for (int i = 0; i < h_extend; i++) //横着也就是按行进行fft
		{
			m_com.FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对x方向进行快速傅立叶变换
		}

		for (int i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
		{
			for (int j = 0; j < w_extend; j++)
			{
				pTD[h_extend*j + i] = pFD[w_extend*i + j];//////////////////////////////////////////错误点！！！！
			}
		}

		for (int i = 0; i < w_extend; i++)//竖着也就是按列进行fft
		{
			m_com.FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
		}

		for (int i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
		{
			for (int j = 0; j < w_extend; j++)
			{
				pTD[w_extend*i + j] = pFD[h_extend*j + i];
			}
		}

		//到这一步  pTD里存的就是傅里叶变换的全部数据

		m_pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();
		//在m_pFD中存一份频谱信息
		for (int i = 0; i < h_extend*w_extend; i++)//把所有频域数据拷贝到m_pfd进行存储
		{
			m_pFD[i] = pTD[i];
		}
		delete[] pTD;
		delete[] pFD;
	}

	int w1 = (int)(w_extend - m_dHPFRadius) / 2;
	int h1 = (int)(h_extend - m_dHPFRadius) / 2;
	//int ptCenterX = h_extend/2*h_extend+w_extend/2;
	//int ptCenterY = h_extend/2*h_extend+w_extend/2;
	int ptCenterX = w_extend / 2;
	int ptCenterY = h_extend / 2;
	//开始逆向变换
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];//分配内存空间(变换过程中临时变量)
	complex<double>* pFD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];//分配内存空间

	//生成频域滤波器模板
	double *pLFilter = new double[w_extend*h_extend];

	for (int i = 0; i < h_extend; ++i)//对滤波器模板赋值
	{
		for (int j = 0; j < w_extend; ++j)
		{
			double dDist = sqrt(pow((double)i - ptCenterY, 2) + pow((double)j - ptCenterX, 2));
			switch (m_nLPFMode)
			{
			case 0://理想低通滤波
				if (dDist >= m_dLPFRadius)
				{
					pLFilter[i*w_extend + j] = 0;
				}
				else
				{
					pLFilter[i*w_extend + j] = 1;
				}
				break;
			case 1: //巴特沃斯低通滤波
				pLFilter[i*w_extend + j] = 1.0 / (1.0 + pow((dDist / m_dLPFRadius), 2 * n1));
				break;
			case 2://高斯低通滤波
				pLFilter[i*w_extend + j] = exp(-0.5*pow((dDist / m_dLPFRadius), 2));
				break;
			default:
				break;
			}
		}
	}


	for (int i = 0; i < h_extend; ++i)//把所有频域数据拷贝到pTD1并应用模板
	{
		for (int j = 0; j < w_extend; ++j)
		{
			pTD1[i*w_extend + j] = m_pFD[i*w_extend + j] * pLFilter[i*w_extend + j];
			m_pFD[i*w_extend + j] = pTD1[i*w_extend + j]; //同时也更新m_pfd
		}
	}

	//反变换开始
	for (int i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		m_com.RFFT(&pTD1[w_extend*i], &pFD1[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = pFD1[w_extend*i + j];//////////////////////////////////////////错误点！！！！
		}
	}

	for (int i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		m_com.RFFT(&pTD1[h_extend*i], &pFD1[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pFD1[h_extend*j + i];
		}
	}

	for (int i = 0; i < h_extend; i++)//对变换结果乘以之前的频域平移因子，不然会出现之前的相位出错
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(i + j)));
		}
	}

	//到此pTD1存的就全部都是时域数据

	for (int i = 0; i < h_extend; i++) //显示反变换后的图像
	{
		for (int j = 0; j < w_extend; j++)
		{
			BYTE temp = (BYTE)pTD1[i*w_extend + j].real();
			m_ImageAfter.m_pBits[0][i][j] = temp;
			m_ImageAfter.m_pBits[1][i][j] = temp;
			m_ImageAfter.m_pBits[2][i][j] = temp;
		}
	}

	delete pTD1; //一定要去掉，不然多占几十兆内存
	delete pFD1;
	delete pLFilter;
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();

	m_bIsFFTed = true;
	ChangeScrollSize();
	UpdateState();
}


void CImage_ProcessingView::OnRebuiltFromPhase()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull()) //仍然是处理After里的数据，原图备份
		return;

	if (!m_bIsFFTed)
	{
		MessageBox(_T("请先执行FFT操作！"));
		return;
	}



	int h_extend = 1;//图像进行扩展,寻找2的幂次方
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;

	while ((UINT)h_extend < m_nHeight)
	{
		h_extend *= 2;
		h_index++;
	}
	while ((UINT)w_extend < m_nWidth)
	{
		w_extend *= 2;
		w_index++;
	}

	//开始逆向变换
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间
	complex<double>* pFD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	for (int i = 0; i < h_extend; ++i)//把所有频域数据拷贝到pTD1
	{
		for (int j = 0; j < w_extend; ++j)
		{
			//pTD1[i*w_extend+j]=m_pFD[i*w_extend+j]*(pow(-1.0,(i+j)));
			pTD1[i*w_extend + j]._Val[0] = atan(m_pFD[i*w_extend + j]._Val[1] / m_pFD[i*w_extend + j]._Val[0]); //应该在得到时域信号之后再来乘这个东西
		}
	}
	//让图像均值为0
	/*pTD1[h_extend/2*h_extend+w_extend/2] = 0.0;*/
	for (int i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		m_com.RFFT(&pTD1[w_extend*i], &pFD1[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = pFD1[w_extend*i + j];//////////////////////////////////////////错误点！！！！
		}
	}

	for (int i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		m_com.RFFT(&pTD1[h_extend*i], &pFD1[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pFD1[h_extend*j + i];
		}
	}

	for (int i = 0; i < h_extend; i++)//对变换结果乘以之前的频域平移因子，不然会出现之前的相位出错
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(i + j)));
		}
	}

	//到此pTD1存的就全部都是时域数据

	vector<double> dVecPhase;
	for (int i = 0; i < h_extend; i++) //显示幅度谱
	{
		for (int j = 0; j < w_extend; j++)
		{
			double temp1 = pTD1[i*w_extend + j].real();
			dVecPhase.push_back(temp1);
		}
	}

	//找到最大最小值
	vector<double> tempVec(dVecPhase.begin(), dVecPhase.end());
	sort(tempVec.begin(), tempVec.end());
	double dMax = tempVec[tempVec.size() - 1];
	double dMin = tempVec[0];

	double dMax1 = tempVec[tempVec.size() - 2];
	double dMin1 = tempVec[1];

	int nMaxPos = find(dVecPhase.begin(), dVecPhase.end(), dMax) - dVecPhase.begin();//0和256
	int nMinPos = find(dVecPhase.begin(), dVecPhase.end(), dMin) - dVecPhase.begin();
	cout << "maxPos:" << nMaxPos << ",minPos" << nMinPos << endl;
	cout << dMax << "," << dMin << endl;
	//归一化
	for (auto it = dVecPhase.begin(); it != dVecPhase.end(); ++it)
	{
		if (*it == dMax)
		{
			*it = 255;
		}
		else if (*it == dMin)
		{
			*it = 0;
		}
		else
		{
			*it = ((*it) - dMin1) / (dMax1 - dMin1) * 255;
		}
		/**it = ((*it)-dMin)/(dMax-dMin)*255;*/
	}

	m_ImageAfter.Create(w_extend, h_extend, 24);//此时m_ImageAfter的宽度和长度还没有变化，因此要重新建一张图
	for (int i = 0; i < h_extend; i++) //显示反变换得到的图像
	{
		for (int j = 0; j < w_extend; j++)
		{
			//cout << i << "," << j << endl;
			m_ImageAfter.m_pBits[0][i][j] = (BYTE)dVecPhase[i*w_extend + j];
			m_ImageAfter.m_pBits[1][i][j] = (BYTE)dVecPhase[i*w_extend + j];
			m_ImageAfter.m_pBits[2][i][j] = (BYTE)dVecPhase[i*w_extend + j];
		}
	}

	delete pTD1; //一定要去掉，不然多占几十兆内存
	delete pFD1;
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();
	/*	m_bIsFFTed = false;*/
	ChangeScrollSize();
	UpdateState();

}


void CImage_ProcessingView::OnFilterIhpf()
{
	// TODO: Add your command handler code here
	double K2 = m_dHPFEnforcePara;
	double K1 = m_dHPFOriImgPara;
	int n = (int)m_nBHPFOrder;

	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull()) //仍然是处理After里的数据，原图备份
		m_Image.CopyTo(m_ImageAfter);

	CCommon com;

	UINT h_extend = 1;//图像进行扩展,寻找2的幂次方
	UINT w_extend = 1;
	UINT h_index = 0;
	UINT w_index = 0;

	while (h_extend < m_nHeight)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend < m_nWidth)
	{
		w_extend *= 2;
		w_index++;
	}


	if (!m_bIsFFTed)//若还未进行傅里叶变换进行正变换
	{
		// 		if(!m_bIsGrayed)
		// 			OnTogray(); //先灰度化
		MyImage_ imgGrayed;//先灰度化
		doToGray(m_ImageAfter, imgGrayed);
		imgGrayed.CopyTo(m_ImageAfter);

		complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间
		complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

		for (UINT i = 0; i < m_nHeight; i++)//把图像的值传给pTD
		{
			for (UINT j = 0; j < m_nWidth; j++)
			{
				pTD[i*w_extend + j] = m_ImageAfter.m_pBits[0][i][j] * (pow(-1.0, (int)(i + j)));//把频谱搬移到中心 
			}
		}

		for (UINT i = 0; i < h_extend; i++) //横着也就是按行进行fft
		{
			m_com.FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对x方向进行快速傅立叶变换
		}

		for (UINT i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
		{
			for (UINT j = 0; j < w_extend; j++)
			{
				pTD[h_extend*j + i] = pFD[w_extend*i + j];//////////////////////////////////////////错误点！！！！
			}
		}

		for (UINT i = 0; i < w_extend; i++)//竖着也就是按列进行fft
		{
			m_com.FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
		}

		for (UINT i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
		{
			for (UINT j = 0; j < w_extend; j++)
			{
				pTD[w_extend*i + j] = pFD[h_extend*j + i];
			}
		}

		//到这一步  pTD里存的就是傅里叶变换的全部数据

		m_pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];//这里之所以能够new，是因为确定之前没有fft过，否则之前m_pfd有内容的话，再new，原来的会丢失(没有指针管理，也就是溢出)
		//在m_pFD中存一份频谱信息
		for (UINT i = 0; i < h_extend*w_extend; i++)//把所有频域数据拷贝到m_pfd进行存储
		{
			m_pFD[i] = pTD[i];
		}
		delete[] pTD;
		delete[] pFD;
	}

	int w1 = (int)(w_extend - m_dHPFRadius) / 2;
	int h1 = (int)(h_extend - m_dHPFRadius) / 2;
	//int ptCenterX = h_extend/2*h_extend+w_extend/2;
	//int ptCenterY = h_extend/2*h_extend+w_extend/2;
	int ptCenterX = w_extend / 2;
	int ptCenterY = h_extend / 2;
	//开始逆向变换
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];//分配内存空间(变换过程中临时变量)
	complex<double>* pFD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];//分配内存空间

	//生成频域滤波器模板
	double *pHFilter = new double[w_extend*h_extend];

	for (UINT i = 0; i < h_extend; ++i)//对滤波器模板赋值
	{
		for (UINT j = 0; j < w_extend; ++j)
		{
			double dDist = sqrt(pow((double)i - ptCenterY, 2) + pow((double)j - ptCenterX, 2));
			switch (m_nHPFMode)
			{
			case 0://理想高通滤波
				if (dDist >= m_dHPFRadius)
				{
					pHFilter[i*w_extend + j] = K1 + K2 * 1;
				}
				else
				{
					pHFilter[i*w_extend + j] = K1;
				}
				break;
			case 1: //巴特沃斯高通滤波
				pHFilter[i*w_extend + j] = K1 + K2 * (1.0 / (1.0 + pow((m_dHPFRadius / dDist), 2 * n)));
				break;
			case 2://高斯高通滤波
				pHFilter[i*w_extend + j] = K1 + K2 * (1 - exp(-0.5*pow((dDist / m_dHPFRadius), 2)));
				break;
			default:
				break;
			}
		}
	}

	for (UINT i = 0; i < h_extend; ++i)//把所有频域数据拷贝到pTD1并应用模板
	{
		for (UINT j = 0; j < w_extend; ++j)
		{
			pTD1[i*w_extend + j] = m_pFD[i*w_extend + j] * pHFilter[i*w_extend + j];
			m_pFD[i*w_extend + j] = pTD1[i*w_extend + j]; //同时也更新m_pfd
		}
	}

	//应用模板后，进行反变化
	for (UINT i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		m_com.RFFT(&pTD1[w_extend*i], &pFD1[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (UINT i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = pFD1[w_extend*i + j];//////////////////////////////////////////错误点！！！！
		}
	}

	for (UINT i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		m_com.RFFT(&pTD1[h_extend*i], &pFD1[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (UINT i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pFD1[h_extend*j + i];
		}
	}

	for (UINT i = 0; i < h_extend; i++)//对变换结果乘以之前的频域平移因子，不然会出现之前的相位出错
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(int)(i + j)));
		}
	}

	//到此pTD1存的就全部都是时域数据

	for (UINT i = 0; i < h_extend; i++) //显示反变换后的图像
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			//cout << "ptd1: i:" << i << " j:" << j <<  " " <<pTD1[i*w_extend+j] << endl;
			BYTE temp = (BYTE)pTD1[i*w_extend + j].real();
			m_ImageAfter.m_pBits[0][i][j] = temp;
			m_ImageAfter.m_pBits[1][i][j] = temp;
			m_ImageAfter.m_pBits[2][i][j] = temp;
		}
	}

	delete pTD1; //一定要去掉，不然多占几十兆内存
	delete pFD1;
	delete pHFilter;

	m_bIsFFTed = true;
	UpdateState();
}


void CImage_ProcessingView::OnFilterHpfSet()
{
	// TODO: Add your command handler code here
	if (m_dlgFtHPFSet.DoModal() == IDOK)
	{
		m_dHPFEnforcePara = m_dlgFtHPFSet.m_dHPFEnforcePara;
		m_dHPFRadius = m_dlgFtHPFSet.m_dHPFRadius;
		m_nHPFMode = m_dlgFtHPFSet.m_nHPFMode;
		m_nBHPFOrder = m_dlgFtHPFSet.m_nBHPFOrder;
		m_dHPFOriImgPara = m_dlgFtHPFSet.m_dOriImgPara;
	}
}


void CImage_ProcessingView::ShowImgInDlg(CString strWindowName)
{
	CDlgShowImg *pDlg = new CDlgShowImg(strWindowName);
	pDlg->Create(IDD_DLG_SHOW_IMG, this);
	pDlg->SetWindowTextW(strWindowName);
	pDlg->ShowWindow(SW_SHOW);
}


void CImage_ProcessingView::FFT_IMG(MyImage_ &imgInput, MyImage_ &imgOutput, int flag)
{

}

void CImage_ProcessingView::OnFilterLpfSet()
{
	// TODO: Add your command handler code here
	if (m_dlgFtLPFSet.DoModal() == IDOK)
	{
		m_dLPFRadius = m_dlgFtLPFSet.m_dLPFRadius;
		m_nLPFMode = m_dlgFtLPFSet.m_nLPFMode;
		m_nBLPFOrder = m_dlgFtLPFSet.m_nBLPFOrder;
	}
}

//对图像本身(本来就在0-255)进行归一化是没有意义的，只有说对一组数据(范围大于255的)进行归一化
void CImage_ProcessingView::ImageNormalize(const MyImage_ &InputImage, MyImage_ &OutputImage)
{
	UINT nWidth = InputImage.GetWidth();
	UINT nHeight = InputImage.GetHeight();
	vector<vector<BYTE>> nVecBits;
	for (int c = 0; c != 3; ++c) //先分别加入三个通道的像素值
	{
		nVecBits.push_back(vector<BYTE>());
		for (UINT i = 0; i < nHeight; i++) //显示幅度谱
		{
			for (UINT j = 0; j < nWidth; j++)
			{
				nVecBits[c].push_back(InputImage.m_pBits[c][i][j]);
			}
		}
	}

	//找到最大最小值
	BYTE nMax[3], nMin[3];
	for (int c = 0; c != 3; ++c)
	{
		vector<BYTE> temp(nVecBits[c].begin(), nVecBits[c].end());
		sort(temp.begin(), temp.end());
		nMax[c] = temp[temp.size() - 1];
		nMin[c] = temp[0];
	}


	//归一化
	for (int c = 0; c != 3; ++c)
	{
		for (auto it = nVecBits[c].begin(); it != nVecBits[c].end(); ++it)
		{
			*it = ((*it) - nMin[c]) / (nMax[c] - nMin[c]) * 255;
		}
	}

	for (int c = 0; c != 3; ++c)
	{
		for (UINT i = 0; i < nHeight; i++) //显示幅度谱
		{
			for (UINT j = 0; j < nWidth; j++)
			{
				BYTE temp = nVecBits[c][i*nWidth + j];
				OutputImage.m_pBits[c][i][j] = temp;
			}
		}
	}
}


template<class T>
T SaturateCast(T value, const T minT, const T maxT)
{
	if (value > maxT)
	{
		value = maxT;
	}
	else if (value < minT)
	{
		value = minT;
	}
	return value;
}

void CImage_ProcessingView::OnFilterSort()
{
	// TODO: Add your command handler code here
	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();
	int nFtPara = m_nFtMidPara;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ imgBackUp, imgFilled;
	//复制备份,防止产生迭代,处理过程中对此图处理，完成后再拷贝回ImgAfter
	m_ImageAfter.BorderFillTo(imgFilled, m_nFtMidPara, m_nFtMidFillMode);
	m_ImageAfter.BorderFillTo(imgBackUp, m_nFtMidPara, m_nFtMidFillMode);

	UINT w = imgFilled.GetWidth(); //重新获得填充后的长和宽
	UINT h = imgFilled.GetHeight();

	vector<UINT>  nVecPixelB; //新建三个容器，分别用来容纳三个通道用来排序的像素
	vector<UINT>  nVecPixelG;
	vector<UINT>  nVecPixelR;

	UINT nMidPos = (nFtPara * 2 + 1) * (nFtPara * 2 + 1) / 2; //获得模板中心位置

	//对整张填充后的图进行遍历
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数h-midpara
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-midpara次
		{
			nVecPixelB.clear();
			nVecPixelG.clear();
			nVecPixelR.clear();
			//对每个像素把模板对应的领域像素添加进容器中
			for (UINT m = i - nFtPara; m <= i + nFtPara; ++m)
			{
				for (UINT n = j - nFtPara; n <= j + nFtPara; ++n)
				{
					nVecPixelB.push_back(imgFilled.m_pBits[0][m][n]); //B
					nVecPixelG.push_back(imgFilled.m_pBits[1][m][n]); //G
					nVecPixelR.push_back(imgFilled.m_pBits[2][m][n]); //R
				}
			}
			sort(nVecPixelB.begin(), nVecPixelB.end()); //进行排序
			sort(nVecPixelG.begin(), nVecPixelG.end());
			sort(nVecPixelR.begin(), nVecPixelR.end());
			switch (m_nFTSortMode)
			{
			case 0://最大值滤波
				imgBackUp.m_pBits[0][i][j] = nVecPixelB[nVecPixelB.size() - 1]; //得到最大值
				imgBackUp.m_pBits[1][i][j] = nVecPixelG[nVecPixelG.size() - 1];
				imgBackUp.m_pBits[2][i][j] = nVecPixelR[nVecPixelR.size() - 1];
				break;
			case 1://最小值滤波
				imgBackUp.m_pBits[0][i][j] = nVecPixelB[0]; //得到最小值
				imgBackUp.m_pBits[1][i][j] = nVecPixelG[0];
				imgBackUp.m_pBits[2][i][j] = nVecPixelR[0];
				break;
			case 2://中点滤波
				imgBackUp.m_pBits[0][i][j] = (nVecPixelB[0] + nVecPixelB[nVecPixelB.size() - 1]) / 2; //得到中点值
				imgBackUp.m_pBits[1][i][j] = (nVecPixelG[0] + nVecPixelG[nVecPixelG.size() - 1]) / 2;
				imgBackUp.m_pBits[2][i][j] = (nVecPixelR[0] + nVecPixelR[nVecPixelR.size() - 1]) / 2;
				break;
			case 3://修正alpha均值滤波
			{
				int nPixLeftCount = (m_nFtMidPara * 2 + 1) * (m_nFtMidPara * 2 + 1) - m_nFTAlphaAvg_d;
				int nSum[3] = { 0 };
				for (int i1 = m_nFTAlphaAvg_d / 2; i1 != nVecPixelB.size() - 2; ++i1)
				{
					nSum[0] += nVecPixelB[i1];
					nSum[1] += nVecPixelG[i1];
					nSum[2] += nVecPixelR[i1];
				}
				nSum[0] /= nPixLeftCount;
				nSum[1] /= nPixLeftCount;
				nSum[2] /= nPixLeftCount;
				imgBackUp.m_pBits[0][i][j] = nSum[0];
				imgBackUp.m_pBits[1][i][j] = nSum[1];
				imgBackUp.m_pBits[2][i][j] = nSum[2];
			}
			case 4://自适应中值滤波
			{
				int Zmin[3] = { 0 }, Zmax[3] = { 0 }, Zmed[3] = { 0 }, Smax = 49;
				Zmin[0] = nVecPixelB[0]; Zmin[1] = nVecPixelG[0]; Zmin[2] = nVecPixelR[0];
				Zmax[0] = nVecPixelB[nVecPixelB.size()]; Zmin[1] = nVecPixelG[nVecPixelG.size()]; Zmin[2] = nVecPixelR[nVecPixelR.size()];
				Zmed[0] = nVecPixelB[nVecPixelB.size() / 2]; Zmed[1] = nVecPixelG[nVecPixelG.size() / 2]; Zmed[2] = nVecPixelR[nVecPixelR.size() / 2];

			}
			default:break;
			}

		}
	}
	imgBackUp.RemoveFillTo(m_ImageAfter, m_nFtMidPara); //去除边缘填充
	imgBackUp.Destroy(); //销毁中间变量的图像
	imgFilled.Destroy();
	ldTimeEnd = GetTickCount();
	std::cout << "统计排序滤波(标准库排序)完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s"
		<< " ,模板大小：" << (nFtPara * 2 + 1) << "x" << (nFtPara * 2 + 1) << std::endl;
	UpdateState();
}


void CImage_ProcessingView::OnRebuiltFromAmplitude()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull()) //仍然是处理After里的数据，原图备份
		return;

	if (!m_bIsFFTed)
	{
		MessageBox(_T("请先执行FFT操作！"));
		return;
	}



	UINT h_extend = 1;//图像进行扩展,寻找2的幂次方
	UINT w_extend = 1;
	UINT h_index = 0;
	UINT w_index = 0;

	while (h_extend < m_nHeight)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend < m_nWidth)
	{
		w_extend *= 2;
		w_index++;
	}

	//开始逆向变换
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间
	complex<double>* pFD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	for (UINT i = 0; i < h_extend; ++i)//把所有频域数据拷贝到pTD1
	{
		for (UINT j = 0; j < w_extend; ++j)
		{
			//pTD1[i*w_extend+j]=m_pFD[i*w_extend+j]*(pow(-1.0,(i+j)));
			pTD1[i*w_extend + j]._Val[0] = pow(-1.0, (int)(i + j))*sqrt(m_pFD[i*w_extend + j].imag()*m_pFD[i*w_extend + j].imag() + m_pFD[i*w_extend + j].real()*m_pFD[i*w_extend + j].real());
			//atan(m_pFD[i*w_extend+j].imag() / m_pFD[i*w_extend+j].real()); //应该在得到时域信号之后再来乘这个东西
		}
	}
	//让图像均值为0
	pTD1[h_extend / 2 * h_extend + w_extend / 2] = 0.0;
	for (UINT i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		m_com.RFFT(&pTD1[w_extend*i], &pFD1[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (UINT i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = pFD1[w_extend*i + j];//////////////////////////////////////////错误点！！！！
		}
	}

	for (UINT i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		m_com.RFFT(&pTD1[h_extend*i], &pFD1[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (UINT i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pFD1[h_extend*j + i];
		}
	}

	for (UINT i = 0; i < h_extend; i++)//对变换结果乘以之前的频域平移因子，不然会出现之前的相位出错
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(int)(i + j)));
		}
	}

	//到此pTD1存的就全部都是时域数据

	vector<double> dVecPhase;
	for (UINT i = 0; i < h_extend; i++) //显示幅度谱
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			double temp1 = pTD1[i*w_extend + j].real();
			dVecPhase.push_back(temp1);
		}
	}

	//找到最大最小值
	vector<double> tempVec(dVecPhase.begin(), dVecPhase.end());
	sort(tempVec.begin(), tempVec.end());
	double dMax = tempVec[tempVec.size() - 1];
	double dMin = tempVec[0];
	cout << dMax << "," << dMin << endl;
	//归一化(之前是让均值归0了的，均值太大了，导致其他地方看不到，或者求个log?)
	for (auto it = dVecPhase.begin(); it != dVecPhase.end(); ++it)
	{
		*it = ((*it) - dMin) / (dMax - dMin) * 255;
	}

	m_ImageAfter.Create(w_extend, h_extend, 24);//此时m_ImageAfter的宽度和长度还没有变化，因此要重新建一张图
	for (UINT i = 0; i < h_extend; i++) //显示反变换得到的图像
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			m_ImageAfter.m_pBits[0][i][j] = (BYTE)dVecPhase[i*w_extend + j];
			m_ImageAfter.m_pBits[1][i][j] = (BYTE)dVecPhase[i*w_extend + j];
			m_ImageAfter.m_pBits[2][i][j] = (BYTE)dVecPhase[i*w_extend + j];
		}
	}


	delete pTD1; //一定要去掉，不然多占几十兆内存
	delete pFD1;
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();
	m_bIsProcessed = TRUE;
	/*	m_bIsFFTed = false;*/
	UpdateState();
}


void CImage_ProcessingView::OnGenerateimg()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) m_Image.Create(512, 512, 24);

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	//for (UINT j=0;j<m_nHeight;j++)
	//{
	//	for (UINT k=0;k<m_nWidth;k++)
	//	{
	//		m_ImageAfter.m_pBits[0][j][k]=0;//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
	//		m_ImageAfter.m_pBits[1][j][k]=0;//G
	//		m_ImageAfter.m_pBits[2][j][k]=255-j/2;
	//	}
	//}

	m_ImageAfter.Create(m_nWidth, m_nHeight, RGB(255, 0, 0));

	UpdateState();
}


void CImage_ProcessingView::OnImageSub()
{
	// TODO: Add your command handler code here
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ Image_Second;
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if (IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if (!Image_Second.IsNull()) Image_Second.Destroy();//判断是否已经有图片，有的话进行清除

		Image_Second.Load(dlg.GetPathName());//调用对话框的GetPathName获得图片的位置，并且调用Load加载图片
		//m_bIsProcessed = TRUE;

		if (Image_Second.IsNull()) return;
		if (m_nWidth != Image_Second.GetWidth() || m_nHeight != Image_Second.GetHeight())//判断两幅图像大小是否一致
		{
			MessageBox(_T("两幅图像大小不一致！"));
			//Image_Second.Destroy();
			return;
		}
		for (UINT j = 0; j < m_nHeight; j++)
			for (UINT i = 0; i < m_nWidth; i++)//两幅图像做简单相加操作
			{
				m_ImageAfter.m_pBits[0][j][i] = abs(m_ImageAfter.m_pBits[0][j][i] - Image_Second.m_pBits[0][j][i]);
				m_ImageAfter.m_pBits[1][j][i] = abs(m_ImageAfter.m_pBits[1][j][i] - Image_Second.m_pBits[1][j][i]);
				m_ImageAfter.m_pBits[2][j][i] = abs(m_ImageAfter.m_pBits[2][j][i] - Image_Second.m_pBits[2][j][i]);
			}
	}
	UpdateState();
}


void CImage_ProcessingView::OnFliterMotionBlur()
{
	// TODO: Add your command handler code here
#define n1 m_nBLPFOrder

	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull()) //仍然是处理After里的数据，原图备份
		m_Image.CopyTo(m_ImageAfter);



	UINT h_extend = 1;//图像进行扩展,寻找2的幂次方
	UINT w_extend = 1;
	UINT h_index = 0;
	UINT w_index = 0;

	while (h_extend < m_nHeight)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend < m_nWidth)
	{
		w_extend *= 2;
		w_index++;
	}


	if (!m_bIsFFTed)//若还未进行傅里叶变换进行正变换
	{
		// 		if(!m_bIsGrayed)
		// 			OnTogray(); //先灰度化
		MyImage_ imgGrayed;//先灰度化
		doToGray(m_ImageAfter, imgGrayed);
		imgGrayed.CopyTo(m_ImageAfter);

		complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间并初始化
		complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

		for (UINT i = 0; i < m_nHeight; i++)//把图像的值传给pTD
		{
			for (UINT j = 0; j < m_nWidth; j++)
			{
				pTD[i*w_extend + j] = m_ImageAfter.m_pBits[0][i][j] * (pow(-1.0, (int)(i + j)));//把频谱搬移到中心 
			}
		}

		for (UINT i = 0; i < h_extend; i++) //横着也就是按行进行fft
		{
			m_com.FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对x方向进行快速傅立叶变换
		}

		for (UINT i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
		{
			for (UINT j = 0; j < w_extend; j++)
			{
				pTD[h_extend*j + i] = pFD[w_extend*i + j];//////////////////////////////////////////错误点！！！！
			}
		}

		for (UINT i = 0; i < w_extend; i++)//竖着也就是按列进行fft
		{
			m_com.FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
		}

		for (UINT i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
		{
			for (UINT j = 0; j < w_extend; j++)
			{
				pTD[w_extend*i + j] = pFD[h_extend*j + i];
			}
		}

		//到这一步  pTD里存的就是傅里叶变换的全部数据

		m_pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();
		//在m_pFD中存一份频谱信息
		for (UINT i = 0; i < h_extend*w_extend; i++)//把所有频域数据拷贝到m_pfd进行存储
		{
			m_pFD[i] = pTD[i];
		}
		delete[] pTD;
		delete[] pFD;
	}

	UINT w1 = (UINT)(w_extend - m_dHPFRadius) / 2;
	UINT h1 = (UINT)(h_extend - m_dHPFRadius) / 2;
	//int ptCenterX = h_extend/2*h_extend+w_extend/2;
	//int ptCenterY = h_extend/2*h_extend+w_extend/2;
	UINT ptCenterX = w_extend / 2;
	UINT ptCenterY = h_extend / 2;
	//开始逆向变换
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间(变换过程中临时变量)
	complex<double>* pFD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间

	//生成频域滤波器模板
	complex<double>* pMotionBlurTemplate = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间
	double dT = 0.1, dA = 0.03, dB = 0.03;
	for (UINT i = 0; i < h_extend; ++i)//对滤波器模板赋值
	{
		for (UINT j = 0; j < w_extend; ++j)
		{
			double num1 = pi*(i*dA + j*dB + 1e-20);
			double num2 = sin(num1);
			complex<double> cplxNum3(cos(num1), -sin(num1));
			pMotionBlurTemplate[i*w_extend + j] = dT / num1*num2*cplxNum3;
			//cout << "i:" << i << " j:" << j <<" "<<"," << num1 <<  " " <<pMotionBlurTemplate[i*w_extend+j] << endl;
			//pMotionBlurTemplate[i*w_extend+j] = complex<double>(1.0,1.0);
		}
	}

	for (UINT i = 0; i < h_extend; ++i)//把所有频域数据拷贝到pTD1并应用模板
	{
		for (UINT j = 0; j < w_extend; ++j)
		{
			/*cout << "mpfd: i:" << i << " j:" << j <<  " " <<m_pFD[i*w_extend+j] << endl;*/
			pTD1[i*w_extend + j] = m_pFD[i*w_extend + j] * pMotionBlurTemplate[i*w_extend + j];
			/*			cout << "ptd1: i:" << i << " j:" << j <<  " " <<pTD1[i*w_extend+j] << endl;*/
			m_pFD[i*w_extend + j] = pTD1[i*w_extend + j]; //同时也更新m_pfd
		}
	}

	//反变换开始
	for (UINT i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		m_com.RFFT(&pTD1[w_extend*i], &pFD1[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (UINT i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = pFD1[w_extend*i + j];//////////////////////////////////////////错误点！！！！
		}
	}

	for (UINT i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		m_com.RFFT(&pTD1[h_extend*i], &pFD1[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (UINT i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pFD1[h_extend*j + i];
		}
	}

	for (UINT i = 0; i < h_extend; i++)//对变换结果乘以之前的频域平移因子，不然会出现之前的相位出错
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(int)(i + j)));
		}
	}

	//到此pTD1存的就全部都是时域数据
	vector<double> dVec;
	for (UINT i = 0; i < h_extend; i++) //显示反变换后的图像
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			//反变换得到的复数，实部虚部都有，并且实部很小
			//cout << "ptd1: i:" << i << " j:" << j <<  " " <<pTD1[i*w_extend+j] << endl;
			//double temp = pTD1[i*w_extend+j].real();
			double temp = sqrt(pTD1[i*w_extend + j].real()*pTD1[i*w_extend + j].real() + pTD1[i*w_extend + j].imag()*pTD1[i*w_extend + j].imag());
			dVec.push_back(temp);
		}
	}


	//找到最大最小值
	vector<double> tempVec(dVec.begin(), dVec.end());
	sort(tempVec.begin(), tempVec.end());
	double dMax = tempVec[tempVec.size() - 1];
	double dMin = tempVec[0];

	//归一化
	for (auto it = dVec.begin(); it != dVec.end(); ++it)
	{
		*it = ((*it) - dMin) / (dMax - dMin) * 255;
	}

	for (UINT i = 0; i < h_extend; i++) //显示幅度谱
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			double dTemp = dVec[i*w_extend + j];
			m_ImageAfter.m_pBits[0][i][j] = (BYTE)dTemp;
			m_ImageAfter.m_pBits[1][i][j] = (BYTE)dTemp;
			m_ImageAfter.m_pBits[2][i][j] = (BYTE)dTemp;
		}
	}



	delete pTD1; //一定要去掉，不然多占几十兆内存
	delete pFD1;
	delete pMotionBlurTemplate;
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();
	m_bIsFFTed = true;
	ChangeScrollSize();
	UpdateState();
}


void CImage_ProcessingView::OnFilterTurbulenceBlur()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull()) //仍然是处理After里的数据，原图备份
		m_Image.CopyTo(m_ImageAfter);



	UINT h_extend = 1;//图像进行扩展,寻找2的幂次方
	UINT w_extend = 1;
	UINT h_index = 0;
	UINT w_index = 0;

	while (h_extend < m_nHeight)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend < m_nWidth)
	{
		w_extend *= 2;
		w_index++;
	}


	if (!m_bIsFFTed)//若还未进行傅里叶变换进行正变换
	{
		// 		if(!m_bIsGrayed)
		// 			OnTogray(); //先灰度化
		MyImage_ imgGrayed;//先灰度化
		doToGray(m_ImageAfter, imgGrayed);
		imgGrayed.CopyTo(m_ImageAfter);

		complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间并初始化
		complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

		for (UINT i = 0; i < m_nHeight; i++)//把图像的值传给pTD
		{
			for (UINT j = 0; j < m_nWidth; j++)
			{
				pTD[i*w_extend + j] = m_ImageAfter.m_pBits[0][i][j] * (pow(-1.0, (int)(i + j)));//把频谱搬移到中心 
			}
		}

		for (UINT i = 0; i < h_extend; i++) //横着也就是按行进行fft
		{
			m_com.FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对x方向进行快速傅立叶变换
		}

		for (UINT i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
		{
			for (UINT j = 0; j < w_extend; j++)
			{
				pTD[h_extend*j + i] = pFD[w_extend*i + j];//////////////////////////////////////////错误点！！！！
			}
		}

		for (UINT i = 0; i < w_extend; i++)//竖着也就是按列进行fft
		{
			m_com.FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
		}

		for (UINT i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
		{
			for (UINT j = 0; j < w_extend; j++)
			{
				pTD[w_extend*i + j] = pFD[h_extend*j + i];
			}
		}

		//到这一步  pTD里存的就是傅里叶变换的全部数据

		m_pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();
		//在m_pFD中存一份频谱信息
		for (UINT i = 0; i < h_extend*w_extend; i++)//把所有频域数据拷贝到m_pfd进行存储
		{
			m_pFD[i] = pTD[i];
		}
		delete[] pTD;
		delete[] pFD;
	}

	UINT w1 = (UINT)(w_extend - m_dHPFRadius) / 2;
	UINT h1 = (UINT)(h_extend - m_dHPFRadius) / 2;
	//int ptCenterX = h_extend/2*h_extend+w_extend/2;
	//int ptCenterY = h_extend/2*h_extend+w_extend/2;
	UINT ptCenterX = w_extend / 2;
	UINT ptCenterY = h_extend / 2;
	//开始逆向变换
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间(变换过程中临时变量)
	complex<double>* pFD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间

	//生成频域滤波器模板
	complex<double>* pMotionBlurTemplate = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间
	double dK = 0.00025;
	for (UINT i = 0; i < h_extend; ++i)//对滤波器模板赋值
	{
		for (UINT j = 0; j < w_extend; ++j)
		{

			double num1 = pow((double)(i*i + j*j), double(5) / 6);
			//cout << "i:" << i << " j:" << j <<" "<<"," << num1<< endl;
			double num2 = exp(-dK*num1);
			//cout << "i:" << i << " j:" << j <<" "<<"," << num2<< endl;
			pMotionBlurTemplate[i*w_extend + j] = num2;
			//cout << "i:" << i << " j:" << j <<" "<<"," << pMotionBlurTemplate[i*w_extend+j]<< endl;
			//pMotionBlurTemplate[i*w_extend+j] = complex<double>(1.0,1.0);
		}
	}

	for (UINT i = 0; i < h_extend; ++i)//把所有频域数据拷贝到pTD1并应用模板
	{
		for (UINT j = 0; j < w_extend; ++j)
		{
			/*cout << "mpfd: i:" << i << " j:" << j <<  " " <<m_pFD[i*w_extend+j] << endl;*/
			pTD1[i*w_extend + j] = m_pFD[i*w_extend + j] * pMotionBlurTemplate[i*w_extend + j];
			/*			cout << "ptd1: i:" << i << " j:" << j <<  " " <<pTD1[i*w_extend+j] << endl;*/
			m_pFD[i*w_extend + j] = pTD1[i*w_extend + j]; //同时也更新m_pfd
		}
	}

	//反变换开始
	for (UINT i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		m_com.RFFT(&pTD1[w_extend*i], &pFD1[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (UINT i = 0; i < h_extend; i++)//吧pFD进行转置，即进行 行列变换，即使不是矩形
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = pFD1[w_extend*i + j];//////////////////////////////////////////错误点！！！！
		}
	}

	for (UINT i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		m_com.RFFT(&pTD1[h_extend*i], &pFD1[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (UINT i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pFD1[h_extend*j + i];
		}
	}

	for (UINT i = 0; i < h_extend; i++)//对变换结果乘以之前的频域平移因子，不然会出现之前的相位出错
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(int)(i + j)));
		}
	}

	//到此pTD1存的就全部都是时域数据
	vector<double> dVec;
	for (UINT i = 0; i < h_extend; i++) //显示反变换后的图像
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			//反变换得到的复数，实部虚部都有，并且实部很小
			//cout << "ptd1: i:" << i << " j:" << j <<  " " <<pTD1[i*w_extend+j] << endl;
			//double temp = pTD1[i*w_extend+j].real();
			double temp = sqrt(pTD1[i*w_extend + j].real()*pTD1[i*w_extend + j].real() + pTD1[i*w_extend + j].imag()*pTD1[i*w_extend + j].imag());
			dVec.push_back(temp);

			//m_ImageAfter.m_pBits[0][i][j]=temp;
			//m_ImageAfter.m_pBits[1][i][j]=temp;
			//m_ImageAfter.m_pBits[2][i][j]=temp;
		}
	}


	//找到最大最小值
	vector<double> tempVec(dVec.begin(), dVec.end());
	sort(tempVec.begin(), tempVec.end());
	double dMax = tempVec[tempVec.size() - 1];
	double dMin = tempVec[0];

	//归一化
	for (auto it = dVec.begin(); it != dVec.end(); ++it)
	{
		*it = ((*it) - dMin) / (dMax - dMin) * 255;
	}

	for (UINT i = 0; i < h_extend; i++) //显示幅度谱
	{
		for (UINT j = 0; j < w_extend; j++)
		{
			double dTemp = dVec[i*w_extend + j];
			m_ImageAfter.m_pBits[0][i][j] = (BYTE)dTemp;
			m_ImageAfter.m_pBits[1][i][j] = (BYTE)dTemp;
			m_ImageAfter.m_pBits[2][i][j] = (BYTE)dTemp;
		}
	}



	delete pTD1; //一定要去掉，不然多占几十兆内存
	delete pFD1;
	delete pMotionBlurTemplate;
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();

	m_bIsFFTed = true;
	ChangeScrollSize();
	UpdateState();


}


void CImage_ProcessingView::OnInterpolationNearest()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	//cout << m_nInterPolationHeight << endl;

	if (m_nInterPolationHeight == m_nHeight && m_nInterPolationWidth == m_nWidth) //大小一样不执行了
		return;

	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();
	MyImage_ ImgTemp;
	ImgTemp.Create(m_nInterPolationWidth, m_nInterPolationHeight, RGB(0, 0, 0));
	switch (m_nInterpolationType)
	{
	case 0:
		m_com.PicZoom2(ImgTemp, m_ImageAfter);
		break;
	case 1:
		m_com.PicZoom_Bilinear1(ImgTemp, m_ImageAfter);
		break;
	case 2:
		m_com.PicZoom_ThreeOrder0(ImgTemp, m_ImageAfter);
		break;
	}
	ldTimeEnd = GetTickCount();

	std::cout << "图像缩放完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s"
		<< "从  " << m_ImageAfter.GetWidth() << "x" << m_ImageAfter.GetHeight()
		<< "到" << ImgTemp.GetWidth() << "x" << ImgTemp.GetHeight() << std::endl;
	ImgTemp.CopyTo(m_ImageAfter);
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();
	ChangeScrollSize();
	UpdateState();
}


void CImage_ProcessingView::OnInterpolationSet()
{
	// TODO: Add your command handler code here
	m_dlgInterpolationSet.m_nInterPolationHeight_origin = m_nHeight;
	m_dlgInterpolationSet.m_nInterPolationWidth_origin = m_nWidth;

	if (m_dlgInterpolationSet.DoModal() == IDOK)
	{
		m_nInterpolationType = m_dlgInterpolationSet.m_nInterpolationType;
		m_nInterPolationWidth = m_dlgInterpolationSet.m_nInterPolationWidth;
		m_nInterPolationHeight = m_dlgInterpolationSet.m_nInterPolationHeight;
	}

	//if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	//if(m_ImageAfter.IsNull())
	//	m_Image.CopyTo(m_ImageAfter);

	//for (UINT j=0;j<m_nHeight;j++)
	//{
	//	for (UINT k=0;k<m_nWidth;k++)
	//	{
	//		m_ImageAfter.m_pBits[0][j][k]=0;//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
	//		m_ImageAfter.m_pBits[1][j][k]=0;//G

	//	}
	//}
	//m_bIsProcessed = TRUE;
	//Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


void CImage_ProcessingView::OnRotate()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();
	MyImage_ ImgTemp, ImgTemp1;
	double RotateAngle = PI / 6;
	double RotateScale = 1.0;
	double RotateScaleX = RotateScale, RotateScaleY = RotateScale;
	ImgTemp.Create((int)(m_nWidth*RotateScaleX), (int)(m_nHeight*RotateScaleY), RGB(255, 255, 255));
	//m_com.PicRotary0(ImgTemp,m_ImageAfter,RotateAngle,RotateScaleX,RotateScaleY,
	//				(ImgTemp.GetWidth()-m_ImageAfter.GetWidth())*0.5,
	//				(ImgTemp.GetHeight()-m_ImageAfter.GetHeight())*0.5);
	//m_com.PicRotary1(ImgTemp,m_ImageAfter,RotateAngle,RotateScaleX,RotateScaleY,
	//	(ImgTemp.GetWidth()-m_ImageAfter.GetWidth())*0.5,
	//	(ImgTemp.GetHeight()-m_ImageAfter.GetHeight())*0.5);
	m_com.PicRotary2(ImgTemp, m_ImageAfter, RotateAngle, RotateScaleX, RotateScaleY,
		(ImgTemp.GetWidth() - m_ImageAfter.GetWidth())*0.5,
		(ImgTemp.GetHeight() - m_ImageAfter.GetHeight())*0.5);
	//m_com.PicRotary0(ImgTemp,m_ImageAfter,RotateAngle,RotateScaleX,RotateScaleY,
	//	0,
	//	0);

	ldTimeEnd = GetTickCount();
	std::cout << "图像旋转完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s"
		<< ",旋转了" << RotateAngle*(180 / PI) << "度,"
		<< "从  " << m_ImageAfter.GetWidth() << "x" << m_ImageAfter.GetHeight()
		<< "到" << ImgTemp.GetWidth() << "x" << ImgTemp.GetHeight() << std::endl;
	ImgTemp.CopyTo(m_ImageAfter);
	m_nWidth = m_ImageAfter.GetWidth();
	m_nHeight = m_ImageAfter.GetHeight();
	ChangeScrollSize();
	UpdateState();
}


void CImage_ProcessingView::OnHistEqHsi()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
		return;
	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	double dPixTotal = m_nHeight * m_nWidth;
	int H, S, I;
	m_dMapHistI.clear();
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_com.ChangedColourFromRGBStandardtoHSL(m_ImageAfter.m_pBits[2][j][k], m_ImageAfter.m_pBits[1][j][k], m_ImageAfter.m_pBits[0][j][k]
				, &H, &S, &I);

			++m_dMapHistI[I];
		}
	}

	UINT nPixTotal = m_nWidth*m_nHeight;
	cout << nPixTotal << endl;

	for (auto it = m_dMapHistI.begin(); it != m_dMapHistI.end(); ++it)
	{
		it->second = it->second / nPixTotal;
	}


	//开始直方图均衡化
	std::map<UINT, UINT> nMapTableI;
	double dHistSum = 0.0; UINT nIndex;
	//R分量
	for (auto it = m_dMapHistI.begin(); it != m_dMapHistI.end(); ++it)
	{
		dHistSum += it->second;
		nIndex = it->first;
		nMapTableI[nIndex] = (UINT)round(dHistSum * 99);
	}

	int R, G, B;
	int r, g, b;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			b = m_ImageAfter.m_pBits[0][j][k];
			g = m_ImageAfter.m_pBits[1][j][k];
			r = m_ImageAfter.m_pBits[2][j][k];
			//cout << (r+g+b)/3 << endl;
			m_com.ChangedColourFromRGBStandardtoHSL(r, g, b, &H, &S, &I); //I是规定划到了1-100的
			//cout << H <<" " <<S << " "<< I<< endl;
			m_com.ChangedColourFromHSLStandardtoRGB(H, S, nMapTableI[I], &R, &G, &B);//转换回的R分量会超过255，WHY?
			//cout << R <<" " <<G << " "<< B<< endl;
			m_ImageAfter.m_pBits[0][j][k] = SaturateCast((int)B, 0, 255);//b   用循环访问图像的像素值
			m_ImageAfter.m_pBits[1][j][k] = SaturateCast((int)G, 0, 255);//g   用循环访问图像的像素值
			m_ImageAfter.m_pBits[2][j][k] = SaturateCast((int)R, 0, 255);//r   用循环访问图像的像素值
		}
	}

	UpdateState();
}


void CImage_ProcessingView::On32904()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			int h, s, i;
			int r, g, b;
			b = m_ImageAfter.m_pBits[0][j][k];
			g = m_ImageAfter.m_pBits[1][j][k];
			r = m_ImageAfter.m_pBits[2][j][k];
			m_com.ChangedColourFromRGBStandardtoHSL(r, g, b, &h, &s, &i);
			int r1, g1, b1;
			m_com.ChangedColourFromHSLStandardtoRGB(h, s, i, &r1, &g1, &b1);
			//m_ImageAfter.m_pBits[0][j][k]=abs((int)(b1-b));//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			//m_ImageAfter.m_pBits[1][j][k]=abs((int)(g1-g));//G
			//m_ImageAfter.m_pBits[2][j][k]=abs((int)(r1-r));//r
			m_ImageAfter.m_pBits[0][j][k] = b1;//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_ImageAfter.m_pBits[1][j][k] = g1;//G
			m_ImageAfter.m_pBits[2][j][k] = r1;//r
		}
	}
	UpdateState();
}


void CImage_ProcessingView::OnShowR()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_ImageAfter.m_pBits[0][j][k] = m_ImageAfter.m_pBits[2][j][k];//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_ImageAfter.m_pBits[1][j][k] = m_ImageAfter.m_pBits[2][j][k];//G
		}
	}
	UpdateState();
}

void CImage_ProcessingView::OnShowG()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_ImageAfter.m_pBits[0][j][k] = m_ImageAfter.m_pBits[1][j][k];//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_ImageAfter.m_pBits[2][j][k] = m_ImageAfter.m_pBits[1][j][k];//G
		}
	}
	UpdateState();
}

void CImage_ProcessingView::OnShowB()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_ImageAfter.m_pBits[2][j][k] = m_ImageAfter.m_pBits[0][j][k];//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_ImageAfter.m_pBits[1][j][k] = m_ImageAfter.m_pBits[0][j][k];//G
		}
	}
	UpdateState();
}

void CImage_ProcessingView::OnShowH()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);
	int r, g, b; int h, s, i;
	vector<int> nVecHValue;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			b = m_ImageAfter.m_pBits[0][j][k];
			g = m_ImageAfter.m_pBits[1][j][k];
			r = m_ImageAfter.m_pBits[2][j][k];
			m_com.ChangedColourFromRGBStandardtoHSL(r, g, b, &h, &s, &i);
			nVecHValue.push_back(h);
		}
	}

	VecNormalized(nVecHValue, 0, 255);//写成CCommon类成员函数要报unresoloved external错误

	for (UINT i = 0; i < m_nHeight; i++) //显示幅度谱
	{
		for (UINT j = 0; j < m_nWidth; j++)
		{
			int temp = nVecHValue[i*m_nWidth + j];
			m_ImageAfter.m_pBits[0][i][j] = temp;
			m_ImageAfter.m_pBits[1][i][j] = temp;
			m_ImageAfter.m_pBits[2][i][j] = temp;
		}
	}


	UpdateState();

}

void CImage_ProcessingView::OnShowS()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);
	int r, g, b; int h, s, i;
	vector<int> nVecSValue;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			b = m_ImageAfter.m_pBits[0][j][k];
			g = m_ImageAfter.m_pBits[1][j][k];
			r = m_ImageAfter.m_pBits[2][j][k];
			m_com.ChangedColourFromRGBStandardtoHSL(r, g, b, &h, &s, &i);
			nVecSValue.push_back(s);
		}
	}

	VecNormalized(nVecSValue, 0, 255);//写成CCommon类成员函数要报unresoloved external错误

	for (UINT i = 0; i < m_nHeight; i++) //显示幅度谱
	{
		for (UINT j = 0; j < m_nWidth; j++)
		{
			int temp = nVecSValue[i*m_nWidth + j];
			m_ImageAfter.m_pBits[0][i][j] = temp;
			m_ImageAfter.m_pBits[1][i][j] = temp;
			m_ImageAfter.m_pBits[2][i][j] = temp;
		}
	}
	UpdateState();
}

void CImage_ProcessingView::OnShowI()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);
	int r, g, b; int h, s, i;
	vector<int> nVecIValue;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			b = m_ImageAfter.m_pBits[0][j][k];
			g = m_ImageAfter.m_pBits[1][j][k];
			r = m_ImageAfter.m_pBits[2][j][k];
			m_com.ChangedColourFromRGBStandardtoHSL(r, g, b, &h, &s, &i);
			nVecIValue.push_back(i);
		}
	}

	VecNormalized(nVecIValue, 0, 255);//写成CCommon类成员函数要报unresoloved external错误

	for (UINT i = 0; i < m_nHeight; i++) //显示幅度谱
	{
		for (UINT j = 0; j < m_nWidth; j++)
		{
			int temp = nVecIValue[i*m_nWidth + j];
			m_ImageAfter.m_pBits[0][i][j] = temp;
			m_ImageAfter.m_pBits[1][i][j] = temp;
			m_ImageAfter.m_pBits[2][i][j] = temp;
		}
	}
	UpdateState();
}

template <class T>
void VecNormalized(vector<T> &vecT, T normMin, T normMax)
{
	vector<T> temp(vecT.begin(), vecT.end());
	sort(temp.begin(), temp.end());
	T Tmax = temp[temp.size() - 1];
	T Tmin = temp[0];

	//归一化到0-255
	for (auto it = vecT.begin(); it != vecT.end(); ++it)
	{
		*it = (T)(((double)((*it) - Tmin)) / (Tmax - Tmin)*(normMax - normMin) + normMin);
	}
}

void CImage_ProcessingView::OnImgsegRgb()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	m_bIsImgSEGing = TRUE;
	m_nImgSegType = 0;
	m_ImageAfter.CopyTo(m_ImageSegShowRect);
	//Invalidate();
}


void CImage_ProcessingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bIsImgSEGing)
	{
		//InvalidateRect(CRect(m_ptImgSegOrigin.x,m_ptImgSegOrigin.y,point.x,point.y));
		m_bImgSegRectSelectStart = TRUE;
		m_ptImgSegOrigin = point;
	}

	if (m_nDrawType != -1)
	{
		m_bIsLBDown = true;
		m_ptDrawOrigin = point;
		m_rectDraw.top = point.y;
		m_rectDraw.left = point.x;
		m_rectDraw.bottom = point.y;
		m_rectDraw.right = point.x;
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


void CImage_ProcessingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bIsImgSEGing && m_bImgSegRectSelectStart)
	{
		/*CDC *pDC = GetDC();*/
		CPaintDC dc(this);
		m_bImgSegRectSelectStart = FALSE;
		m_bIsImgSEGing = FALSE;
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *pOldBrush;
		pOldBrush = dc.SelectObject(pBrush);

		dc.Rectangle(CRect(m_ptImgSegOrigin.x, m_ptImgSegOrigin.y, point.x, point.y));
		dc.SelectObject(pOldBrush);
		/*		ReleaseDC(pDC);*/
				//m_ptImgSegOrigin = point;

		int nThreshold = 50; //阈值设定
		MyImage_ imgSelected;
		imgSelected.Create(abs(m_ptImgSegOrigin.x - point.x), abs(m_ptImgSegOrigin.y - point.y), 0);
		int w_sel = imgSelected.GetWidth();
		int h_sel = imgSelected.GetHeight();

		if (m_nImgSegType == 0) //rgb分割
		{
			long nAvgR = 0, nAvgB = 0, nAvgG = 0;
			for (int y = 0; y != h_sel; ++y)
			{
				for (int x = 0; x != w_sel; ++x)
				{
					imgSelected.m_pBits[0][y][x] = m_ImageAfter.m_pBits[0][m_ptImgSegOrigin.y + y][m_ptImgSegOrigin.x + x];
					imgSelected.m_pBits[1][y][x] = m_ImageAfter.m_pBits[1][m_ptImgSegOrigin.y + y][m_ptImgSegOrigin.x + x];
					imgSelected.m_pBits[2][y][x] = m_ImageAfter.m_pBits[2][m_ptImgSegOrigin.y + y][m_ptImgSegOrigin.x + x];

					nAvgB += imgSelected.m_pBits[0][y][x];
					nAvgG += imgSelected.m_pBits[1][y][x];
					nAvgR += imgSelected.m_pBits[2][y][x];
				}
			}
			nAvgB /= (w_sel*h_sel); //区域平均值
			nAvgG /= (w_sel*h_sel);
			nAvgR /= (w_sel*h_sel);
			int nR, nG, nB;
			for (UINT i = 0; i < m_nHeight; i++) //遍历像素，计算距离
			{
				for (UINT j = 0; j < m_nWidth; j++)
				{
					nB = m_ImageAfter.m_pBits[0][i][j];
					nG = m_ImageAfter.m_pBits[1][i][j];
					nR = m_ImageAfter.m_pBits[2][i][j];
					//if (abs(nR-nAvgR) > nThreshold || abs(nG - nAvgG) > nThreshold || abs(nB - nAvgB) > nThreshold)
					if (GetDistance(nR, nG, nB, nAvgR, nAvgG, nAvgB) > nThreshold)
					{
						m_ImageAfter.m_pBits[0][i][j] = 0;
						m_ImageAfter.m_pBits[1][i][j] = 0;
						m_ImageAfter.m_pBits[2][i][j] = 0;
					}
					else
					{
						m_ImageAfter.m_pBits[0][i][j] = 255;
						m_ImageAfter.m_pBits[1][i][j] = 255;
						m_ImageAfter.m_pBits[2][i][j] = 255;
					}

				}
			}
		}
		else if (m_nImgSegType == 1) //HSI分割
		{
			long nAvgH = 0, nAvgS = 0, nAvgI = 0;
			int nR, nG, nB, nH, nS, nI;
			for (int y = 0; y != h_sel; ++y)
			{
				for (int x = 0; x != w_sel; ++x)
				{
					nR = m_ImageAfter.m_pBits[2][m_ptImgSegOrigin.y + y][m_ptImgSegOrigin.x + x];
					nG = m_ImageAfter.m_pBits[1][m_ptImgSegOrigin.y + y][m_ptImgSegOrigin.x + x];
					nB = m_ImageAfter.m_pBits[0][m_ptImgSegOrigin.y + y][m_ptImgSegOrigin.x + x];
					//这个选择区域图像暂时不用，备份
					imgSelected.m_pBits[0][y][x] = nB;
					imgSelected.m_pBits[1][y][x] = nG;
					imgSelected.m_pBits[2][y][x] = nR;
					m_com.ChangedColourFromRGBStandardtoHSL(nR, nG, nB, &nH, &nS, &nI);
					nAvgH += nH;
					nAvgS += nS;
					nAvgI += nI;
				}
			}
			nAvgH /= (w_sel*h_sel);
			nAvgS /= (w_sel*h_sel);
			nAvgI /= (w_sel*h_sel);

			for (UINT i = 0; i < m_nHeight; i++) //遍历像素，计算距离
			{
				for (UINT j = 0; j < m_nWidth; j++)
				{
					nB = m_ImageAfter.m_pBits[0][i][j];
					nG = m_ImageAfter.m_pBits[1][i][j];
					nR = m_ImageAfter.m_pBits[2][i][j];
					m_com.ChangedColourFromRGBStandardtoHSL(nR, nG, nB, &nH, &nS, &nI);
					//if (abs(nH - nAvgH) > nThreshold || abs(nS - nAvgS) > nThreshold || abs(nI - nAvgI) > nThreshold)
					if (GetDistance(nH, nS, nI, nAvgH, nAvgS, nAvgI) > nThreshold)
					{
						m_ImageAfter.m_pBits[0][i][j] = 0;
						m_ImageAfter.m_pBits[1][i][j] = 0;
						m_ImageAfter.m_pBits[2][i][j] = 0;
					}
					else
					{
						m_ImageAfter.m_pBits[0][i][j] = 255;
						m_ImageAfter.m_pBits[1][i][j] = 255;
						m_ImageAfter.m_pBits[2][i][j] = 255;
					}

				}
			}
		}
		UpdateState();

	}


	if (m_nDrawType != -1 && m_bIsLBDown)
	{
		m_bIsLBDown = false;

		CopyViewImage(m_rectDraw);
		UpdateState(true);
		//m_nDrawType = -1;
	}

	CScrollView::OnLButtonUp(nFlags, point);
}


int GetDistance(int R, int G, int B, int RTaget, int GTaget, int BTaget)
{
	return(abs(R - RTaget) + abs(G - GTaget) + abs(B - BTaget));
}

void CImage_ProcessingView::OnImgsegHsi()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	m_bIsImgSEGing = TRUE;
	m_nImgSegType = 1;
	m_ImageAfter.CopyTo(m_ImageSegShowRect);
	//Invalidate();
}


void CImage_ProcessingView::OnGrayworld()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);
	long nAvgR = 0, nAvgG = 0, nAvgB = 0;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			nAvgR += m_ImageAfter.m_pBits[2][j][k];
			nAvgG += m_ImageAfter.m_pBits[1][j][k];
			nAvgB += m_ImageAfter.m_pBits[0][j][k];
		}
	}
	//cout << nAvgR << " " << nAvgG << " " << nAvgB << endl;
	nAvgR /= (m_nHeight*m_nWidth);
	nAvgB /= (m_nHeight*m_nWidth);
	nAvgG /= (m_nHeight*m_nWidth);
	//cout << nAvgR << " " << nAvgG << " " << nAvgB << endl;
	int nGrayValue = (nAvgR + nAvgG + nAvgB) / 3;
	double dScaleValue[3] = { 0.0 };
	dScaleValue[0] = (double)nGrayValue / nAvgB;
	dScaleValue[1] = (double)nGrayValue / nAvgG;
	dScaleValue[2] = (double)nGrayValue / nAvgR;
	//cout << dScaleValue[0]<< " " << dScaleValue[1] << " " <<dScaleValue[2]<< endl;
	for (UINT j = 0; j < m_nHeight; j++)
	{
		for (UINT k = 0; k < m_nWidth; k++)
		{
			m_ImageAfter.m_pBits[0][j][k] = (BYTE)SaturateCast(m_ImageAfter.m_pBits[0][j][k] * dScaleValue[0], 0.0, 255.0);
			m_ImageAfter.m_pBits[1][j][k] = (BYTE)SaturateCast(m_ImageAfter.m_pBits[1][j][k] * dScaleValue[1], 0.0, 255.0);
			m_ImageAfter.m_pBits[2][j][k] = (BYTE)SaturateCast(m_ImageAfter.m_pBits[2][j][k] * dScaleValue[2], 0.0, 255.0);
		}
	}

	UpdateState();
}


void CImage_ProcessingView::OnFilterGradient()
{
	// TODO: Add your command handler code here
	int nFtPara = 1;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	// 	if (!m_bIsGrayed)
	// 		OnTogray();
	MyImage_ imgGrayed;//先灰度化
	doToGray(m_ImageAfter, imgGrayed);
	imgGrayed.CopyTo(m_ImageAfter);


	MyImage_ imgBackUp, imgFilled;
	m_ImageAfter.BorderFillTo(imgBackUp, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgFilled, nFtPara, 2);

	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();


	//for (int i=0;i!=h;++i)
	//{
	//	for (int j=0;j!=w;++j)
	//	{
	//		printf("%d,",imgBackUp.m_pBits[0][i][j]);
	//	}
	//	printf("\n");
	//}


	//int tpl[3][3] = {{0,1,1},{-1,0,1},{-1,-1,0}};//prewitt左下右上对角
	//int tpl[3][3] = {{-1,-1,0},{-1,0,1},{0,1,1}};//prewitt左上右下对角


	//int tpl[3][3] = {{-1,-1,-1},{0,0,0},{1,1,1}}; //prewitt 垂直

	//int tpl[3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}}; //prewitt 水平

	int tplY[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} }; //sobel 垂直
	int tplX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} }; //sobel 水平

	//int tpl[3][3] = {{1,1,1},{1,-8,1},{1,1,1}}; //laplace 各角度
	//int tpl[3][3] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};

	//int tpl[3][3] = {{-1,-1,-1},{2,2,2},{-1,-1,-1}}; //水平
	//int tpl[3][3] = {{-1,2,-1},{-1,2,-1},{-1,2,-1}};// 垂直
	//int tpl[3][3] = {{2,-1,-1},{-1,2,-1},{-1,-1,2}};//45

	/*int tpl[3][3] = {{0,1,0},{1,-4,1},{0,1,0}};*/

	/*int tpl[3][3] = {{0,-1,0},{-1,4,-1},{0,-1,0}};*/

	/*int tpl[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};*/

	/*int tpl[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};*/

	vector<UINT> nVecValue;
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			int sumX[3] = { 0,0,0 };//三个通道分别的和
			int sumY[3] = { 0,0,0 };
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					int tempX = tplX[r][c];
					int tempY = tplY[r][c];
					//printf("%d\n",temp);
					sumX[0] += imgFilled.m_pBits[0][i1][j1] * tempX;
					sumX[1] += imgFilled.m_pBits[1][i1][j1] * tempX;
					sumX[2] += imgFilled.m_pBits[2][i1][j1] * tempX;

					sumY[0] += imgFilled.m_pBits[0][i1][j1] * tempY;
					sumY[1] += imgFilled.m_pBits[1][i1][j1] * tempY;
					sumY[2] += imgFilled.m_pBits[2][i1][j1] * tempY;

					//printf("%d\n",imgFilled.m_pBits[0][i1][j1]);
					c++;
				}
				c = 0;
				r++;
			}

			////取绝对值
			//imgBackUp.m_pBits[0][i][j] = SaturateCast(abs(sumX[0])+abs(sumY[0]),0,255);
			//imgBackUp.m_pBits[1][i][j] = SaturateCast(abs(sumX[1])+abs(sumY[1]),0,255);
			//imgBackUp.m_pBits[2][i][j] = SaturateCast(abs(sumX[1])+abs(sumY[1]),0,255);

			//取正值
			imgBackUp.m_pBits[0][i][j] = SaturateCast(SaturateCast(sumX[0]) + SaturateCast(sumY[0]));
			imgBackUp.m_pBits[1][i][j] = SaturateCast(SaturateCast(sumX[1]) + SaturateCast(sumY[1]));
			imgBackUp.m_pBits[2][i][j] = SaturateCast(SaturateCast(sumX[2]) + SaturateCast(sumY[2]));
		}
	}
	imgBackUp.RemoveFillTo(m_ImageAfter, nFtPara);
	imgBackUp.Destroy();
	imgFilled.Destroy();
	UpdateState();
}


//基本的基于灰度值的阈值分割，支持IN-Place
void CImage_ProcessingView::doThreshold(const MyImage_ &srcImg, MyImage_ &dstImg, int nThresh, double dThresh)
{
	UINT w = srcImg.GetWidth();
	UINT h = srcImg.GetHeight();

	if (nThresh != 0 && dThresh != 0.0)
	{
		::MessageBoxW(NULL, _T("两个阈值不能同时为0!"), _T("错误"), MB_OK);
		return;
	}

	if (srcImg.IsNull())
		return;

	if (dstImg.IsNull())
		srcImg.CopyTo(dstImg);

	MyImage_ imgGrayed; //可以改为就地操作，为了观察更明显就不使用了
	doToGray(srcImg, imgGrayed);

	if (dThresh != 0.0)
	{
		vector<UINT> nVecValue;
		for (UINT j = 0; j < h; j++)
		{
			for (UINT k = 0; k < w; k++)
			{
				nVecValue.push_back(imgGrayed.m_pBits[0][j][k]);
			}
		}

		UINT nMaxValue = maxVecValue(nVecValue);
		int i = 0;
		for (auto it = nVecValue.begin(); it != nVecValue.end(); ++it, ++i)
		{
			if (*it < nMaxValue*dThresh)
				*it = 0;
			else
				*it = 255;
			dstImg.m_pBits[0][i / m_nWidth][i%m_nWidth] = *it;
			dstImg.m_pBits[1][i / m_nWidth][i%m_nWidth] = *it;
			dstImg.m_pBits[2][i / m_nWidth][i%m_nWidth] = *it;
		}
	}
	else
	{
		for (int i = 0; i != h; ++i)
		{
			for (int j = 0; j != w; ++j)
			{
				if (imgGrayed.m_pBits[0][i][j] >= nThresh)
				{
					dstImg.m_pBits[0][i][j] = 255;
					dstImg.m_pBits[1][i][j] = 255;
					dstImg.m_pBits[2][i][j] = 255;
				}
				else
				{
					dstImg.m_pBits[0][i][j] = 0;
					dstImg.m_pBits[1][i][j] = 0;
					dstImg.m_pBits[2][i][j] = 0;
				}
			}
		}

	}
}


void CImage_ProcessingView::OnFilterThreshold()
{
	// TODO: Add your command handler code here
	double dThresh = 0.33;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	CDlgThresh *pDlg = new CDlgThresh;
	m_ImageAfter.CopyTo(m_ImageThresh);
	pDlg->Create(IDD_DLG_THRESH, NULL);
	pDlg->ShowWindow(SW_SHOW);

	//MyImage_ imgTemp;
	//doThreshold(m_ImageAfter,imgTemp,0,0.33);
	//imgTemp.CopyTo(m_ImageAfter);


	//UpdateState();
}


void CImage_ProcessingView::OnFilterGaussLaplacian()
{
	int nFtPara = 2;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	/*OnTogray();*/
	MyImage_ imgGrayed;//先灰度化
	doToGray(m_ImageAfter, imgGrayed);
	imgGrayed.CopyTo(m_ImageAfter);


	MyImage_ imgBackUp, imgFilled;
	m_ImageAfter.BorderFillTo(imgBackUp, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgFilled, nFtPara, 2);

	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();

	int tpl[5][5] = { {0,0,-1,0,0},{0,-1,-2,-1,0},{-1,-2,16,-2,-1},{0,-1,-2,-1,0},{0,0,-1,0,0} }; //gaussian-laplacian


	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			int sum[3] = { 0,0,0 };//三个通道分别的和
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					int temp = tpl[r][c];
					//printf("%d\n",temp);
					sum[0] += imgFilled.m_pBits[0][i1][j1] * temp;
					sum[1] += imgFilled.m_pBits[1][i1][j1] * temp;
					sum[2] += imgFilled.m_pBits[2][i1][j1] * temp;
					//printf("%d\n",imgFilled.m_pBits[0][i1][j1]);
					c++;
				}
				c = 0;
				r++;
			}
			//取绝对值（能看到双边缘效应）
			//imgBackUp.m_pBits[0][i][j] = SaturateCast(abs(sum[0]),0,255);
			//imgBackUp.m_pBits[1][i][j] = SaturateCast(abs(sum[1]),0,255);
			//imgBackUp.m_pBits[2][i][j] = SaturateCast(abs(sum[2]),0,255);

			//取正值
			imgBackUp.m_pBits[0][i][j] = SaturateCast(sum[0], 0, 255);
			imgBackUp.m_pBits[1][i][j] = SaturateCast(sum[1], 0, 255);
			imgBackUp.m_pBits[2][i][j] = SaturateCast(sum[2], 0, 255);

		}
	}
	imgBackUp.RemoveFillTo(m_ImageAfter, nFtPara);
	imgBackUp.Destroy();
	imgFilled.Destroy();
	UpdateState();
}


void CImage_ProcessingView::OnEditUndo()
{
	// TODO: Add your command handler code here

	vector<BYTE> vecValues;
	if (m_imgStock.getPreImage(vecValues))
	{
		vecValuesToImage(vecValues);
		m_bIsProcessed = TRUE;
		Invalidate(1);
	}
	//cout << m_imgStock.getStockCount() << "," <<m_imgStock.getCurIndex() << endl;
}


void CImage_ProcessingView::vecValuesToImage(vector<BYTE> &vecValues)
{
	for (int i = 0; i != vecValues.size(); i = i + 3)
	{
		m_ImageAfter.m_pBits[0][i / 3 / m_nWidth][i / 3 % m_nWidth] = vecValues[i];
		m_ImageAfter.m_pBits[1][i / 3 / m_nWidth][i / 3 % m_nWidth] = vecValues[i + 1];
		m_ImageAfter.m_pBits[2][i / 3 / m_nWidth][i / 3 % m_nWidth] = vecValues[i + 2];
	}
}



void CImage_ProcessingView::OnEditRedo()
{
	// TODO: Add your command handler code here
	vector<BYTE> vecValues;
	if (m_imgStock.getNextImage(vecValues))
	{
		vecValuesToImage(vecValues);
		m_bIsProcessed = TRUE;
		Invalidate(1);
	}
	//cout << m_imgStock.getStockCount() << "," <<m_imgStock.getCurIndex() << endl;
}


void CImage_ProcessingView::OnFilterGradientAmL2()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	int nFtPara = 1;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	// 	if (!m_bIsGrayed)
	// 		OnTogray();
	MyImage_ imgGrayed;//先灰度化
	doToGray(m_ImageAfter, imgGrayed);
	imgGrayed.CopyTo(m_ImageAfter);


	MyImage_ imgBackUp, imgFilled;
	m_ImageAfter.BorderFillTo(imgBackUp, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgFilled, nFtPara, 2);

	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();

	int tplY[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} }; //sobel 垂直
	int tplX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} }; //sobel 水平


	vector<UINT> nVecValue;
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			int sumX[3] = { 0,0,0 };//三个通道分别的和
			int sumY[3] = { 0,0,0 };
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					int tempX = tplX[r][c];
					int tempY = tplY[r][c];
					//printf("%d\n",temp);
					sumX[0] += imgFilled.m_pBits[0][i1][j1] * tempX;
					sumX[1] += imgFilled.m_pBits[1][i1][j1] * tempX;
					sumX[2] += imgFilled.m_pBits[2][i1][j1] * tempX;

					sumY[0] += imgFilled.m_pBits[0][i1][j1] * tempY;
					sumY[1] += imgFilled.m_pBits[1][i1][j1] * tempY;
					sumY[2] += imgFilled.m_pBits[2][i1][j1] * tempY;

					//printf("%d\n",imgFilled.m_pBits[0][i1][j1]);
					c++;
				}
				c = 0;
				r++;
			}

			int temp = (int)sqrt((double)sumX[0] * sumX[0] + (double)sumY[0] * sumY[0]);

			imgBackUp.m_pBits[0][i][j] = SaturateCast(temp);
			imgBackUp.m_pBits[1][i][j] = SaturateCast(temp);
			imgBackUp.m_pBits[2][i][j] = SaturateCast(temp);
		}
	}
	imgBackUp.RemoveFillTo(m_ImageAfter, nFtPara);
	imgBackUp.Destroy();
	imgFilled.Destroy();
	UpdateState();
}


void CImage_ProcessingView::OnFilterGradientAngle()
{
	// TODO: Add your command handler code here
	int nFtPara = 1;

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	// 	if (!m_bIsGrayed)
	// 		OnTogray();
	MyImage_ imgGrayed;//先灰度化
	doToGray(m_ImageAfter, imgGrayed);
	imgGrayed.CopyTo(m_ImageAfter);


	MyImage_ imgBackUp, imgFilled;
	m_ImageAfter.BorderFillTo(imgBackUp, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgFilled, nFtPara, 2);

	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();

	int tplY[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} }; //sobel 垂直
	int tplX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} }; //sobel 水平


	vector<UINT> nVecValue;
	vector<double> dVecAngle;
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			int sumX[3] = { 0,0,0 };//三个通道分别的和
			int sumY[3] = { 0,0,0 };
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					int tempX = tplX[r][c];
					int tempY = tplY[r][c];
					//printf("%d\n",temp);
					sumX[0] += imgFilled.m_pBits[0][i1][j1] * tempX;
					sumX[1] += imgFilled.m_pBits[1][i1][j1] * tempX;
					sumX[2] += imgFilled.m_pBits[2][i1][j1] * tempX;

					sumY[0] += imgFilled.m_pBits[0][i1][j1] * tempY;
					sumY[1] += imgFilled.m_pBits[1][i1][j1] * tempY;
					sumY[2] += imgFilled.m_pBits[2][i1][j1] * tempY;

					//printf("%d\n",imgFilled.m_pBits[0][i1][j1]);
					c++;
				}
				c = 0;
				r++;
			}

			double temp = atan((double)sumY[0] / sumX[0]);
			dVecAngle.push_back(temp); //只加入了一个通道的值

		}
	}


	VecNormalized(dVecAngle, 0.0, 100.0);

	for (int i = 0; i != m_nHeight; ++i)
	{
		for (int j = 0; j != m_nWidth; ++j)
		{
			imgBackUp.m_pBits[0][i][j] = (BYTE)SaturateCast(dVecAngle[i*m_nWidth + j]);
			imgBackUp.m_pBits[1][i][j] = (BYTE)SaturateCast(dVecAngle[i*m_nWidth + j]);
			imgBackUp.m_pBits[2][i][j] = (BYTE)SaturateCast(dVecAngle[i*m_nWidth + j]);
		}
	}

	imgBackUp.RemoveFillTo(m_ImageAfter, nFtPara);
	imgBackUp.Destroy();
	imgFilled.Destroy();
	UpdateState();
}


void CImage_ProcessingView::OnCanny()
{
	// TODO: Add your command handler code here
	int nFtPara = 1;
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	/*OnTogray(); //这里还是不判断m_bIsGrayed比较好*/
	MyImage_ imgGrayed;//先灰度化
	doToGray(m_ImageAfter, imgGrayed);
	imgGrayed.CopyTo(m_ImageAfter);

	MyImage_ imgBackUp, imgFilled, imgGradient, imgGradientX, imgGradientY;
	m_ImageAfter.BorderFillTo(imgBackUp, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgFilled, nFtPara, 2);

	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();

	double tpl[3][3] = { {0.0113,0.0838,0.0113},{0.0838,0.6193,0.0838},{0.0113,0.0838,0.0113} }; //高斯模板

	//先进行高斯滤波
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			double sum[3] = { 0,0,0 };//三个通道分别的和
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					double temp = tpl[r][c];
					sum[0] += (double)imgFilled.m_pBits[0][i1][j1] * temp;
					sum[1] += (double)imgFilled.m_pBits[1][i1][j1] * temp;
					sum[2] += (double)imgFilled.m_pBits[2][i1][j1] * temp;
					c++;
				}
				c = 0;
				r++;
			}
			imgBackUp.m_pBits[0][i][j] = (BYTE)SaturateCast(sum[0], 0.0, 255.0);
			imgBackUp.m_pBits[1][i][j] = (BYTE)SaturateCast(sum[1], 0.0, 255.0);
			imgBackUp.m_pBits[2][i][j] = (BYTE)SaturateCast(sum[2], 0.0, 255.0);
		}
	}
	imgBackUp.RemoveFillTo(m_ImageAfter, nFtPara);
	imgBackUp.Destroy();
	imgFilled.Destroy();
	//UpdateState();

	m_ImageAfter.BorderFillTo(imgGradientX, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgGradientY, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgBackUp, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgGradient, nFtPara, 2); //备份一下
	m_ImageAfter.BorderFillTo(imgFilled, nFtPara, 2);

	//int tplY[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}}; //sobel 垂直
	//int tplX[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; //sobel 水平

	int tplX[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} }; //sobel 垂直
	int tplY[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} }; //sobel 水平

	//求出梯度图像
	vector<UINT> nVecValue;
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			int sumX[3] = { 0,0,0 };//三个通道分别的和
			int sumY[3] = { 0,0,0 };
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					int tempX = tplX[r][c];
					int tempY = tplY[r][c];
					sumX[0] += imgFilled.m_pBits[0][i1][j1] * tempX;
					sumX[1] += imgFilled.m_pBits[1][i1][j1] * tempX;
					sumX[2] += imgFilled.m_pBits[2][i1][j1] * tempX;
					imgGradientX.m_pBits[0][i][j] = SaturateCast(sumX[0]);
					imgGradientX.m_pBits[1][i][j] = SaturateCast(sumX[1]);
					imgGradientX.m_pBits[2][i][j] = SaturateCast(sumX[2]);

					sumY[0] += imgFilled.m_pBits[0][i1][j1] * tempY;
					sumY[1] += imgFilled.m_pBits[1][i1][j1] * tempY;
					sumY[2] += imgFilled.m_pBits[2][i1][j1] * tempY;

					imgGradientY.m_pBits[0][i][j] = SaturateCast(sumY[0]);
					imgGradientY.m_pBits[1][i][j] = SaturateCast(sumY[1]);
					imgGradientY.m_pBits[2][i][j] = SaturateCast(sumY[2]);

					c++;
				}
				c = 0;
				r++;
			}

			int temp = (int)sqrt((double)sumX[0] * sumX[0] + (double)sumY[0] * sumY[0]);
			imgGradient.m_pBits[0][i][j] = SaturateCast(temp);
			imgGradient.m_pBits[1][i][j] = SaturateCast(temp);
			imgGradient.m_pBits[2][i][j] = SaturateCast(temp);
		}
	}

	//imgGradient.RemoveFillTo(m_ImageAfter,nFtPara);
	//imgBackUp.RemoveFillTo(imgGradient,nFtPara);
	//imgBackUp.Destroy();
	//imgFilled.Destroy();
	//UpdateState();
	//return;


	//进行非极大值抑制

	MyImage_ imgNewEdge, imgGradient1, imgGradientX1, imgGradientY1; //带1的是去除边缘后的
	imgGradient.RemoveFillTo(imgGradient1, nFtPara);
	imgGradientX.RemoveFillTo(imgGradientX1, nFtPara);
	imgGradientY.RemoveFillTo(imgGradientY1, nFtPara);

	imgNewEdge.Create(imgGradient1.GetWidth(), imgGradient1.GetHeight(), RGB(0, 0, 0));

	for (int i = nFtPara; i < imgGradient1.GetHeight() - nFtPara; ++i) //循环次数只有h-2
	{
		for (int j = nFtPara; j < imgGradient1.GetWidth() - nFtPara; ++j)//循环w-2次
		{

			BYTE My = imgGradientY1.m_pBits[0][i][j]; //只取一个通道就行了，都一样的
			BYTE Mx = imgGradientX1.m_pBits[0][i][j];
			double angle = 0.0;
			if (My != 0)
			{
				angle = atan((double)Mx / My);
			}
			else if (My == 0 && Mx > 0)
			{
				angle = pi / 2;
			}
			else
			{
				angle = -pi / 2;
			}

			vector<int> nVecPads = m_com.get_coords(angle);
			double M1 = My*imgGradient1.m_pBits[0][i + nVecPads[1]][j + nVecPads[0]] +
				(My - Mx)*imgGradient1.m_pBits[0][i + nVecPads[3]][j + nVecPads[2]];
			nVecPads = m_com.get_coords(angle + pi);
			double M2 = My*imgGradient1.m_pBits[0][i + nVecPads[1]][j + nVecPads[0]] +
				(My - Mx)*imgGradient1.m_pBits[0][i + nVecPads[3]][j + nVecPads[2]];

			int flag = 0;
			flag = (Mx*imgGradient1.m_pBits[0][i][j] > M1) * (Mx*imgGradient1.m_pBits[0][i][j] >= M2)
				+ (Mx*imgGradient1.m_pBits[0][i][j] < M1) * (Mx*imgGradient1.m_pBits[0][i][j] <= M2);

			if (flag)
			{
				imgNewEdge.m_pBits[0][i][j] = imgGradient1.m_pBits[0][i][j];
				imgNewEdge.m_pBits[1][i][j] = imgGradient1.m_pBits[1][i][j];
				imgNewEdge.m_pBits[2][i][j] = imgGradient1.m_pBits[2][i][j];
			}
		}
	}

	/*	imgNewEdge.CopyTo(m_ImageAfter);
	   UpdateState();
	   return;*/

	   //滞后阈值处理
	MyImage_ imgConnected;
	imgNewEdge.CopyTo(imgConnected);
	int nUpThresh = 120;
	int nLowThresh = 50;

	for (int i = 0; i != imgNewEdge.GetHeight(); ++i)
	{
		for (int j = 0; j != imgNewEdge.GetWidth(); ++j)
		{
			if (imgConnected.m_pBits[0][i][j] >= nLowThresh && imgConnected.m_pBits[0][i][j] != 255)
			{
				imgConnected.m_pBits[0][i][j] = 255;
				m_com.Connect(imgConnected, i, j, nLowThresh);
			}
		}
	}

	for (int i = 0; i != imgNewEdge.GetHeight(); ++i)
	{
		for (int j = 0; j != imgNewEdge.GetWidth(); ++j)
		{
			imgConnected.m_pBits[1][i][j] = imgConnected.m_pBits[0][i][j];
			imgConnected.m_pBits[2][i][j] = imgConnected.m_pBits[0][i][j];
		}
	}


	imgConnected.CopyTo(m_ImageAfter);
	OnFilterThreshold();
	UpdateState();


}




void CImage_ProcessingView::Ontest1()
{
	// TODO: Add your command handler code here
	//int b = (int)ceil(2.1);

	//double p[3] = {1,2,3};

	//vector<double> dVec(p,p+ARRAYSIZE(p)-1);

	//for (auto it=dVec.begin();it!=dVec.end();++it)
	//{
	//	cout << *it << endl;
	//}

	//cout << b << endl;
	//m_Image.CopyTo(m_ImageAfter);

	//doToGray(m_ImageAfter, m_ImageAfter);
	//UpdateState();

}


void CImage_ProcessingView::Oncannydouble()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	m_ImageAfter.CopyTo(m_ImageCanny);

	CDlgCannyThresh *pDlg = new CDlgCannyThresh();
	pDlg->Create(IDD_DLG_CANNY_THRESH, NULL);
	pDlg->ShowWindow(SW_SHOW);
	return;

}

void CImage_ProcessingView::edgeLink(int xc, int yc, int index, double lowThresh, vector<double> &dVecMid, vector<double> &dVecHigh, vector<bool> &bVecHigh, vector<bool> &bVecMid)
{
	int xl = (xc == 0) ? xc : xc - 1;   //左
	int xr = (xc == m_nWidth - 1) ? xc : xc + 1; //右  
	int yu = yc == 0 ? yc : yc - 1;  //上
	int yd = yc == m_nHeight - 1 ? yc : yc + 1; //下

	//遍历8领域
	if (bVecHigh[index] == false)
	{
		for (int x = xl; x <= xr; x++)
		{
			for (int y = yu; y <= yd; y++)
			{
				int index2 = x + y * m_nWidth;
				if ((y != yc || x != xc)
					&& dVecMid[index2] != 0 && bVecMid[index2] == false)
				{
					dVecHigh[index2] = dVecMid[index2];
					bVecMid[index2] = true;
					edgeLink(x, y, index2, lowThresh, dVecMid, dVecHigh, bVecHigh, bVecMid);
				}
			}
		}
		bVecHigh[index] = true;
	}
}

void CImage_ProcessingView::doCanny1(MyImage_ &srcImg, MyImage_ &dstImg, int nLowThresh, int nHighThresh)
{

	int nFtPara = 1;
	if (srcImg.IsNull())
		return;

	MyImage_ imgGrayed;
	doToGray(srcImg, imgGrayed);

	MyImage_ imgFilled;
	imgGrayed.BorderFillTo(imgFilled, nFtPara, 2);
	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();

	CMyImage_double imgdGradient, imgdGradientX, imgdGradientY, imgdGaussed; //全部是填充后的图像
	imgdGaussed.Create(w, h, 0);

	//double gtpl[3][3] = {{0.0113,0.0838,0.0113},{0.0838,0.6193,0.0838},{0.0113,0.0838,0.0113}}; //高斯模板
	double gtpl[3][3] = { {0.0830517,0.121034,0.0830517},{0.121034,0.183657,0.121034},{0.0830517,0.121034,0.0830517} }; //高斯模板
	//先进行高斯滤波
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			double sum[3] = { 0,0,0 };//三个通道分别的和
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					double temp = gtpl[r][c];
					sum[0] += (double)imgFilled.m_pBits[0][i1][j1] * temp;
					sum[1] += (double)imgFilled.m_pBits[1][i1][j1] * temp;
					sum[2] += (double)imgFilled.m_pBits[2][i1][j1] * temp;
					c++;
				}
				c = 0;
				r++;
			}
			imgdGaussed.m_pBits[0][i][j] = sum[0];
			imgdGaussed.m_pBits[1][i][j] = sum[1];
			imgdGaussed.m_pBits[2][i][j] = sum[2];
		}
	}

	//imgBackUp.RemoveFillTo(m_ImageAfter,nFtPara);
	//UpdateState();

	imgdGaussed.CopyTo(imgdGradientX);
	imgdGaussed.CopyTo(imgdGradientY);
	imgdGaussed.CopyTo(imgdGradient);

	//按照matlab预置的所以选择上面的,其实效果差不多
	double tplX[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} }; //sobel 水平
	double tplY[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} }; //sobel 垂直

	//double tplX[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}}; //sobel 水平
	//double tplY[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; //sobel 垂直

	//求出梯度图像
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			double sumX[3] = { 0,0,0 };//三个通道分别的和
			double sumY[3] = { 0,0,0 };
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					double tempX = tplX[r][c];
					double tempY = tplY[r][c];
					sumX[0] += imgdGaussed.m_pBits[0][i1][j1] * tempX;
					sumX[1] += imgdGaussed.m_pBits[1][i1][j1] * tempX;
					sumX[2] += imgdGaussed.m_pBits[2][i1][j1] * tempX;
					imgdGradientX.m_pBits[0][i][j] = sumX[0];
					imgdGradientX.m_pBits[1][i][j] = sumX[1];
					imgdGradientX.m_pBits[2][i][j] = sumX[2];

					sumY[0] += imgdGaussed.m_pBits[0][i1][j1] * tempY;
					sumY[1] += imgdGaussed.m_pBits[1][i1][j1] * tempY;
					sumY[2] += imgdGaussed.m_pBits[2][i1][j1] * tempY;

					imgdGradientY.m_pBits[0][i][j] = sumY[0];
					imgdGradientY.m_pBits[1][i][j] = sumY[1];
					imgdGradientY.m_pBits[2][i][j] = sumY[2];
					c++;
				}
				c = 0;
				r++;
			}

			double temp = sqrt(sumX[0] * sumX[0] + sumY[0] * sumY[0]);
			imgdGradient.m_pBits[0][i][j] = temp;
			imgdGradient.m_pBits[1][i][j] = temp;
			imgdGradient.m_pBits[2][i][j] = temp;
		}
	}

	//进行非极大值抑制
	CMyImage_double imgdNewEdge, imgdGradient1, imgdGradientX1, imgdGradientY1; //带1的是去除边缘后的
	imgdGradient.RemoveFillTo(imgdGradient1, nFtPara);
	imgdGradientX.RemoveFillTo(imgdGradientX1, nFtPara);
	imgdGradientY.RemoveFillTo(imgdGradientY1, nFtPara);
	imgdNewEdge.Create(imgdGradientX1.m_nWidth, imgdGradientX1.m_nWidth, 0);
	vector<double> dVecMag, dVecPhase;
	double angle = 0.0, mag = 0.0;
	for (int i = 0; i < imgdGradient1.m_nHeight; ++i) //
	{
		for (int j = 0; j < imgdGradient1.m_nWidth; ++j)
		{

			double Mx = imgdGradientX1.m_pBits[0][i][j]; //只取一个通道就行了，都一样的
			double My = imgdGradientY1.m_pBits[0][i][j];
			dVecMag.push_back(sqrt(Mx*Mx + My*My));
			if (Mx == 0)
			{
				if (My > 0)
				{
					angle = 90;
				}
				if (My < 0)
				{
					angle = -90;
				}
			}
			else if (My == 0)
			{
				angle = 0.0;
			}
			else
			{
				angle = atan(My / Mx) * 180 / pi;
			}

			angle += 90;
			dVecPhase.push_back(angle);
		}
	}

	//这里最好是填充一下边缘，用0填充
	int index = 0;
	vector<double> dVecMagYZ = dVecMag;

	for (int i = 1; i < imgdGradient1.m_nHeight - 1; ++i)
	{
		for (int j = 1; j < imgdGradient1.m_nWidth - 1; ++j)
		{
			index = i*imgdGradient1.m_nWidth + j;
			angle = dVecPhase[index];
			mag = dVecMag[index];
			dVecMagYZ[index] = mag;
			if (angle >= 0 && angle < 22.5)
			{
				double m1 = dVecMag[index - 1];
				double m2 = dVecMag[index + 1];

				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
			else if (angle >= 22.5 && angle < 67.5)
			{
				double m1 = dVecMag[index + 1 - imgdGradient1.m_nWidth];
				double m2 = dVecMag[index - 1 + imgdGradient1.m_nWidth];
				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
			else if (angle >= 67.5 && angle < 112.5)
			{
				double m1 = dVecMag[index + imgdGradient1.m_nWidth];
				double m2 = dVecMag[index - imgdGradient1.m_nWidth];
				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
			else if (angle >= 112.5 && angle < 157.5)
			{
				double m1 = dVecMag[index - 1 - imgdGradient1.m_nWidth];
				double m2 = dVecMag[index + 1 + imgdGradient1.m_nWidth];
				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
			else if (angle >= 157.5)
			{
				double m1 = dVecMag[index + 1];
				double m2 = dVecMag[index - 1];
				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
		}
	}

	//for (int i=0;i!=m_nHeight;++i)
	//{
	//	for (int j=0;j!=m_nWidth;++j)
	//	{
	//		dstImg.m_pBits[0][i][j] = (BYTE)SaturateCast(dVecMagYZ[i*m_nWidth+j],0.0,255.0);
	//		dstImg.m_pBits[1][i][j] = (BYTE)SaturateCast(dVecMagYZ[i*m_nWidth+j],0.0,255.0);
	//		dstImg.m_pBits[2][i][j] = (BYTE)SaturateCast(dVecMagYZ[i*m_nWidth+j],0.0,255.0);
	//	}
	//}
	//return;

	////滞后阈值处理 my
	int nIndex = 0;
	vector<double> dVecHigh(dVecMagYZ.size(), 0.0);
	vector<double> dVecLow(dVecMagYZ.size(), 0.0);
	vector<double> dVecMid(dVecMagYZ.size(), 0.0);
	for (UINT row = 0; row < m_nHeight; row++) //得到高阈值，低阈值，或者二者中间部分的图像
	{
		for (UINT col = 0; col < m_nWidth; col++)
		{
			if (dVecMagYZ[nIndex] >= nLowThresh)
				dVecLow[nIndex] = dVecMagYZ[nIndex];
			if (dVecMagYZ[nIndex] >= nHighThresh)
				dVecHigh[nIndex] = dVecMagYZ[nIndex];

			dVecMid[nIndex] = dVecLow[nIndex] - dVecHigh[nIndex];
			nIndex++;
		}
	}


	MyImage_ imgTemp(m_nWidth, m_nHeight);
	nIndex = 0;
	vector<bool> bVecHigh(m_nWidth*m_nHeight, false); //标记数组
	vector<bool> bVecMid(m_nWidth*m_nHeight, false);

	for (UINT row = 0; row < m_nHeight; row++)
	{
		for (UINT col = 0; col < m_nWidth; col++)
		{
			//对于高阈值的图像进行连接
			if (dVecHigh[nIndex] != 0)
			{
				edgeLink(col, row, nIndex, nLowThresh, dVecMid, dVecHigh, bVecHigh, bVecMid);
			}
			nIndex++;
		}
	}

	for (int i = 0; i != m_nHeight; ++i)
	{
		for (int j = 0; j != m_nWidth; ++j)
		{
			imgTemp.m_pBits[0][i][j] = (BYTE)SaturateCast(dVecHigh[i*m_nWidth + j], 0.0, 255.0);
			imgTemp.m_pBits[1][i][j] = (BYTE)SaturateCast(dVecHigh[i*m_nWidth + j], 0.0, 255.0);
			imgTemp.m_pBits[2][i][j] = (BYTE)SaturateCast(dVecHigh[i*m_nWidth + j], 0.0, 255.0);
		}
	}
	doThreshold(imgTemp, dstImg, nLowThresh, 0.0);
}


void CImage_ProcessingView::doCanny2(MyImage_ &srcImg, MyImage_ &dstImg, int nLowThresh, int nHighThresh)
{

	int nFtPara = 1;
	if (srcImg.IsNull())
		return;

	MyImage_ imgGrayed;
	doToGray(srcImg, imgGrayed);

	MyImage_ imgFilled;
	imgGrayed.BorderFillTo(imgFilled, nFtPara, 2);
	UINT w = imgFilled.GetWidth();
	UINT h = imgFilled.GetHeight();

	CMyImage_double imgdGradient, imgdGradientX, imgdGradientY, imgdGaussed; //全部是填充后的图像
	imgdGaussed.Create(w, h, 0);

	double gtpl[3][3] = { {0.0113,0.0838,0.0113},{0.0838,0.6193,0.0838},{0.0113,0.0838,0.0113} }; //高斯模板

	//先进行高斯滤波
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			double sum[3] = { 0,0,0 };//三个通道分别的和
			//应用模板
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					double temp = gtpl[r][c];
					sum[0] += (double)imgFilled.m_pBits[0][i1][j1] * temp;
					sum[1] += (double)imgFilled.m_pBits[1][i1][j1] * temp;
					sum[2] += (double)imgFilled.m_pBits[2][i1][j1] * temp;
					c++;
				}
				c = 0;
				r++;
			}
			imgdGaussed.m_pBits[0][i][j] = sum[0];
			imgdGaussed.m_pBits[1][i][j] = sum[1];
			imgdGaussed.m_pBits[2][i][j] = sum[2];
		}
	}

	//imgBackUp.RemoveFillTo(m_ImageAfter,nFtPara);
	//UpdateState();

	imgdGaussed.CopyTo(imgdGradientX);
	imgdGaussed.CopyTo(imgdGradientY);
	imgdGaussed.CopyTo(imgdGradient);

	double tplX[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} }; //sobel 水平
	double tplY[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} }; //sobel 垂直

	//double tplX[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}}; //sobel 水平
	//double tplY[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; //sobel 垂直

	//求出梯度图像
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数只有h-2
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-2次
		{
			double sumX[3] = { 0,0,0 };//三个通道分别的和
			double sumY[3] = { 0,0,0 };
			UINT r = 0, c = 0;
			for (UINT i1 = i - nFtPara; i1 <= i + nFtPara; i1++) //行
			{
				for (UINT j1 = j - nFtPara; j1 <= j + nFtPara; j1++) //列
				{
					double tempX = tplX[r][c];
					double tempY = tplY[r][c];
					sumX[0] += imgdGaussed.m_pBits[0][i1][j1] * tempX;
					sumX[1] += imgdGaussed.m_pBits[1][i1][j1] * tempX;
					sumX[2] += imgdGaussed.m_pBits[2][i1][j1] * tempX;
					imgdGradientX.m_pBits[0][i][j] = sumX[0];
					imgdGradientX.m_pBits[1][i][j] = sumX[1];
					imgdGradientX.m_pBits[2][i][j] = sumX[2];

					sumY[0] += imgdGaussed.m_pBits[0][i1][j1] * tempY;
					sumY[1] += imgdGaussed.m_pBits[1][i1][j1] * tempY;
					sumY[2] += imgdGaussed.m_pBits[2][i1][j1] * tempY;

					imgdGradientY.m_pBits[0][i][j] = sumY[0];
					imgdGradientY.m_pBits[1][i][j] = sumY[1];
					imgdGradientY.m_pBits[2][i][j] = sumY[2];
					c++;
				}
				c = 0;
				r++;
			}

			double temp = sqrt(sumX[0] * sumX[0] + sumY[0] * sumY[0]);
			imgdGradient.m_pBits[0][i][j] = temp;
			imgdGradient.m_pBits[1][i][j] = temp;
			imgdGradient.m_pBits[2][i][j] = temp;
		}
	}

	//进行非极大值抑制
	CMyImage_double imgdNewEdge, imgdGradient1, imgdGradientX1, imgdGradientY1; //带1的是去除边缘后的
	imgdGradient.RemoveFillTo(imgdGradient1, nFtPara);
	imgdGradientX.RemoveFillTo(imgdGradientX1, nFtPara);
	imgdGradientY.RemoveFillTo(imgdGradientY1, nFtPara);
	imgdNewEdge.Create(imgdGradientX1.m_nWidth, imgdGradientX1.m_nWidth, 0);
	vector<double> dVecMag, dVecPhase;
	double angle = 0.0, mag = 0.0;
	for (int i = 0; i < imgdGradient1.m_nHeight; ++i) //
	{
		for (int j = 0; j < imgdGradient1.m_nWidth; ++j)
		{

			double Mx = imgdGradientX1.m_pBits[0][i][j]; //只取一个通道就行了，都一样的
			double My = imgdGradientY1.m_pBits[0][i][j];
			dVecMag.push_back(sqrt(Mx*Mx + My*My));
			if (Mx == 0)
			{
				if (My > 0)
				{
					angle = 90;
				}
				if (My < 0)
				{
					angle = -90;
				}
			}
			else if (My == 0)
			{
				angle = 0.0;
			}
			else
			{
				angle = atan(My / Mx) * 180 / pi;
			}

			angle += 90;
			dVecPhase.push_back(angle);
		}
	}

	//这里最好是填充一下边缘，用0填充
	int index = 0;
	vector<double> dVecMagYZ = dVecMag;

	for (int i = 1; i < imgdGradient1.m_nHeight - 1; ++i)
	{
		for (int j = 1; j < imgdGradient1.m_nWidth - 1; ++j)
		{
			index = i*imgdGradient1.m_nWidth + j;
			angle = dVecPhase[index];
			mag = dVecMag[index];
			dVecMagYZ[index] = mag;
			if (angle >= 0 && angle < 22.5)
			{
				double m1 = dVecMag[index - 1];
				double m2 = dVecMag[index + 1];

				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
			else if (angle >= 22.5 && angle < 67.5)
			{
				double m1 = dVecMag[index + 1 - imgdGradient1.m_nWidth];
				double m2 = dVecMag[index - 1 + imgdGradient1.m_nWidth];
				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
			else if (angle >= 67.5 && angle < 112.5)
			{
				double m1 = dVecMag[index + imgdGradient1.m_nWidth];
				double m2 = dVecMag[index - imgdGradient1.m_nWidth];
				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
			else if (angle >= 112.5 && angle < 157.5)
			{
				double m1 = dVecMag[index - 1 - imgdGradient1.m_nWidth];
				double m2 = dVecMag[index + 1 + imgdGradient1.m_nWidth];
				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
			else if (angle >= 157.5)
			{
				double m1 = dVecMag[index + 1];
				double m2 = dVecMag[index - 1];
				if (mag < m1 || mag < m2)
				{
					dVecMagYZ[index] = 0;
				}
			}
		}
	}

	////滞后阈值处理 my
	int nIndex = 0;
	vector<double> dVecHigh(dVecMagYZ.size(), 0.0);
	vector<double> dVecLow(dVecMagYZ.size(), 0.0);
	vector<double> dVecMid(dVecMagYZ.size(), 0.0);
	for (UINT row = 0; row < m_nHeight; row++) //得到高阈值，低阈值，或者二者中间部分的图像
	{
		for (UINT col = 0; col < m_nWidth; col++)
		{
			if (dVecMagYZ[nIndex] >= nLowThresh)
				dVecLow[nIndex] = dVecMagYZ[nIndex];
			if (dVecMagYZ[nIndex] >= nHighThresh)
				dVecHigh[nIndex] = dVecMagYZ[nIndex];

			dVecMid[nIndex] = dVecLow[nIndex] - dVecHigh[nIndex];
			nIndex++;
		}
	}


	MyImage_ imgTemp(m_nWidth, m_nHeight);
	nIndex = 0;
	vector<bool> bVecHigh(m_nWidth*m_nHeight, false); //标记数组
	vector<bool> bVecMid(m_nWidth*m_nHeight, false);

	for (UINT row = 0; row < m_nHeight; row++)
	{
		for (UINT col = 0; col < m_nWidth; col++)
		{
			//对于高阈值的图像进行连接
			if (dVecHigh[nIndex] != 0)
			{
				edgeLink(col, row, nIndex, nLowThresh, dVecMid, dVecHigh, bVecHigh, bVecMid);
			}
			nIndex++;
		}
	}

	for (int i = 0; i != m_nHeight; ++i)
	{
		for (int j = 0; j != m_nWidth; ++j)
		{
			imgTemp.m_pBits[0][i][j] = (BYTE)SaturateCast(dVecHigh[i*m_nWidth + j], 0.0, 255.0);
			imgTemp.m_pBits[1][i][j] = (BYTE)SaturateCast(dVecHigh[i*m_nWidth + j], 0.0, 255.0);
			imgTemp.m_pBits[2][i][j] = (BYTE)SaturateCast(dVecHigh[i*m_nWidth + j], 0.0, 255.0);
		}
	}
	doThreshold(imgTemp, dstImg, nLowThresh, 0.0);
}




int CImage_ProcessingView::getGlobalThresh(MyImage_ &srcImg)
{
	if (srcImg.IsNull())
		return -1;

	//MyImage_ imgTemp1(srcImg);
	MyImage_ imgTemp1;
	srcImg.CopyTo(imgTemp1);

	MyImage_ imgTemp;
	doToGray(imgTemp1, imgTemp);

	vector<int> nVecAvg;
	vector<BYTE> nVecValue1, nVecValue2;
	CalcImageAverage(imgTemp, true, nVecAvg);
	int nStopThresh = 1;
	int nThresh1 = nVecAvg[0];

	int nAvg1 = 0, nAvg2 = 0;
	//cout << "初始阈值为:" << nThresh1 << endl;

	for (int i = 0; i != m_ImageAfter.GetHeight(); ++i)
	{
		for (int j = 0; j != m_ImageAfter.GetWidth(); ++j)
		{
			if (imgTemp.m_pBits[0][i][j] > nThresh1)
				nVecValue1.push_back(imgTemp.m_pBits[0][i][j]);
			else
				nVecValue2.push_back(imgTemp.m_pBits[0][i][j]);
		}
	}

	nAvg1 = countVecAverage(nVecValue1);
	nAvg2 = countVecAverage(nVecValue2);

	int nThresh2 = (nAvg1 + nAvg2) / 2;
	//cout << nAvg1 << " " << nAvg2 << " " << nThresh2 << endl;

	int nCount = 1;
	while (abs(nThresh2 - nThresh1) >= nStopThresh)
	{
		nThresh1 = nThresh2;
		nVecValue1.clear();
		nVecValue2.clear();
		for (int i = 0; i != m_ImageAfter.GetHeight(); ++i)
		{
			for (int j = 0; j != m_ImageAfter.GetWidth(); ++j)
			{
				if (imgTemp.m_pBits[0][i][j] > nThresh2)
					nVecValue1.push_back(imgTemp.m_pBits[0][i][j]);
				else
					nVecValue2.push_back(imgTemp.m_pBits[0][i][j]);
			}
		}

		nAvg1 = countVecAverage(nVecValue1);
		nAvg2 = countVecAverage(nVecValue2);
		nThresh2 = (nAvg1 + nAvg2) / 2;

		//cout << "第" << nCount << "次计算得阈值:" << nThresh1 << "," <<nThresh2 << endl;
		//cout << abs(nThresh2-nThresh1) << endl;
		nCount++;
	}


	//cout << "全局基本阈值法计算得阈值:" << nThresh2 << endl;
	return nThresh2;
}


void CImage_ProcessingView::OnThreshGlobalbasic()
{
	// TODO: Add your command handler code here

	//CDlgThresh *pDlg = new CDlgThresh;
	//pDlg->Create(IDD_DLG_THRESH,NULL);
	//pDlg->ShowWindow(SW_SHOW);

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	int nThresh = getGlobalThresh(m_ImageAfter);
	MyImage_ imgTemp1(m_ImageAfter);

	doThreshold(imgTemp1, m_ImageAfter, nThresh, 0.0);
	UpdateState();
}

bool CImage_ProcessingView::CalcImageAverage(MyImage_ &srcImg, bool bIsCalcGray, vector<int> &nVevAvg)
{
	if (srcImg.IsNull())
	{
		return false;
	}
	//vector<int> nVevAvg;
	nVevAvg.clear();
	MyImage_ imgTemp;
	srcImg.CopyTo(imgTemp);
	if (bIsCalcGray)
	{
		long long nSumRow = 0, nSumAll = 0;
		for (int i = 0; i != imgTemp.GetHeight(); ++i)
		{
			for (int j = 0; j != imgTemp.GetWidth(); ++j)
			{
				nSumRow += imgTemp.m_pBits[0][i][j];
			}
			nSumRow /= imgTemp.GetWidth();
			nSumAll += nSumRow;
		}
		nSumAll /= imgTemp.GetHeight();
		nVevAvg.push_back((int)nSumAll);
		return true;

	}
	else
	{

		return true;
	}

}



template <class T>
T countVecAverage(vector<T> &vecT)
{
	long long sum = 0;
	for (auto it = vecT.begin(); it != vecT.end(); ++it)
	{
		sum += *it;
	}

	if (vecT.size() != 0)
	{
		sum /= vecT.size();
	}

	return (T)sum;
}



int CImage_ProcessingView::GetOtsuThresh(MyImage_ &srcImg)
{
	if (srcImg.IsNull())
		return -1;

	long long nPixTotal = srcImg.GetWidth()*srcImg.GetHeight();
	MyImage_ imgTemp;
	doToGray(srcImg, imgTemp);

	map<BYTE, double> dMapHist;

	for (int i = 0; i != imgTemp.GetHeight(); ++i)
	{
		for (int j = 0; j != imgTemp.GetWidth(); ++j)
		{
			BYTE temp = imgTemp.m_pBits[0][i][j];
			dMapHist[temp]++;
		}
	}

	//得到归一化直方图
	//double dSum;
	for (auto it = dMapHist.begin(); it != dMapHist.end(); ++it)
	{
		it->second /= nPixTotal;
		//dSum+=it->second;
	}
	//cout << dSum << endl;


	//计算累加和
	map<BYTE, double> dMapProb;
	double dProbSum = 0.0;
	for (auto it = dMapHist.begin(); it != dMapHist.end(); ++it)
	{
		dProbSum += it->second;
		dMapProb[it->first] = dProbSum;
	}

	//计算累计均值
	map<BYTE, int> nMapAvg;
	double dAvgGlobal = 0.0;
	for (auto it = dMapHist.begin(); it != dMapHist.end(); ++it)
	{
		dAvgGlobal += (it->first)*(it->second);
		nMapAvg[it->first] = (int)dAvgGlobal;
	}

	//for (auto it=nMapAvg.begin();it!=nMapAvg.end();++it)
	//{
	//	cout << (int)it->first << "   " << (int)it->second << endl;
	//}

	//cout << "全局均值:" << dAvgGlobal << endl;

	map<BYTE, double> dMapSigma;
	double num1 = 0.0, num2 = 0.0;
	for (auto it = dMapProb.begin(); it != dMapProb.end(); ++it)
	{
		if (it->second == 0)
		{
			continue;
		}
		if (it->second >= 0.999)
		{
			break;
		}
		num1 = pow(dAvgGlobal*(it->second) - nMapAvg[it->first], 2);
		num2 = (it->second)*(1 - (it->second));
		dMapSigma[it->first] = num1 / (num2 + eps);
	}

	// 	
	// 	for (auto it=dMapSigma.begin();it!=dMapSigma.end();++it)
	// 	{
	// 		cout << (int)it->first << "   " << it->second << endl;
	// 	}

	BYTE maxK = getMap2rdMax1st(dMapSigma);
	//cout << "最大方差法得到阈值为:" << (int)maxK << endl;
	return (int)maxK;

	//doThreshold(imgTemp,m_ImageAfter,maxK,0.0);
	//UpdateState();
}


void CImage_ProcessingView::OnThreshOtsu()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	//	long long nPixTotal = m_nHeight*m_nWidth;
	//	MyImage_ imgTemp;
	//	doToGray(m_ImageAfter,imgTemp);
	//
	//	map<BYTE,double> dMapHist;
	//
	//	for (int i=0;i!=imgTemp.GetHeight();++i)
	//	{
	//		for (int j=0;j!=imgTemp.GetWidth();++j)
	//		{
	//			BYTE temp = imgTemp.m_pBits[0][i][j];
	//			dMapHist[temp]++;
	//		}
	//	}
	//	
	//	//得到归一化直方图
	//	//double dSum;
	//	for (auto it=dMapHist.begin();it!=dMapHist.end();++it)
	//	{
	//		it->second /= nPixTotal;
	//		//dSum+=it->second;
	//	}
	//	//cout << dSum << endl;
	//
	//
	//	//计算累加和
	//	map<BYTE,double> dMapProb;
	//	double dProbSum = 0.0;
	//	for (auto it=dMapHist.begin();it!=dMapHist.end();++it)
	//	{
	//		dProbSum+=it->second;
	//		dMapProb[it->first] = dProbSum;
	//	}
	//
	//	//计算累计均值
	//	map<BYTE,int> nMapAvg;
	//	double dAvgGlobal=0.0;
	//	for (auto it=dMapHist.begin();it!=dMapHist.end();++it)
	//	{
	//		dAvgGlobal+=(it->first)*(it->second);
	//		nMapAvg[it->first] = dAvgGlobal;
	//	}
	//
	//	//for (auto it=nMapAvg.begin();it!=nMapAvg.end();++it)
	//	//{
	//	//	cout << (int)it->first << "   " << (int)it->second << endl;
	//	//}
	//
	//	cout << "全局均值:" << dAvgGlobal << endl;
	//
	//	map<BYTE,double> dMapSigma;
	//	double num1=0.0,num2 = 0.0;
	//	for (auto it=dMapProb.begin();it!=dMapProb.end();++it)
	//	{
	//		if (it->second == 0)
	//		{
	//			continue;
	//		}
	//		if (it->second >=0.999)
	//		{
	//			break;
	//		}
	//		num1 = pow(dAvgGlobal*(it->second)-nMapAvg[it->first],2);
	//		num2 = (it->second)*(1-(it->second));
	//		dMapSigma[it->first] = num1/(num2+eps);
	//	}
	//
	//// 	
	//// 	for (auto it=dMapSigma.begin();it!=dMapSigma.end();++it)
	//// 	{
	//// 		cout << (int)it->first << "   " << it->second << endl;
	//// 	}
	//
	//	BYTE maxK = getMapMax(dMapSigma); 
	//	cout << "最大方差法得到阈值为:" << (int)maxK << endl;

	int nThresh = GetOtsuThresh(m_ImageAfter);
	MyImage_ imgtemp(m_ImageAfter);
	doThreshold(imgtemp, m_ImageAfter, nThresh, 0.0);
	UpdateState();
}

BYTE getMap2rdMax1st(map<BYTE, double> &dmap)
{
	auto it = dmap.begin();
	BYTE maxIndex = it->first;
	double maxValue = it->second;
	it++;
	for (; it != dmap.end(); ++it)
	{
		if (it->second > maxValue)
		{
			maxIndex = it->first;
			maxValue = it->second;
		}
	}
	return maxIndex;
}



void CImage_ProcessingView::OnDrawAny()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
	{
		m_Image.Create(512, 512, 0);

	}

	//m_Image.Create(512,512,RGB(255,255,255));

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	//m_bIsDrawAny = 0;
	if (m_bIsDrawing)
	{
		m_bIsDrawing = false;
		m_nDrawType = -1;
	}
	else
	{
		m_bIsDrawing = true;
		m_nDrawType = 0;
	}

	UpdateState(false);

}

//用于画图后更新图像内容,只更新最小矩形内,从而提升速度
void CImage_ProcessingView::CopyViewImage(CRect rect)
{
	CDC *pDC = GetDC();

	//cout << nCr << "," << nCg <<  "," << nCb << endl;//
	for (int i = m_rectDraw.top - 1; i != m_rectDraw.bottom + 1; ++i)
	{
		for (int j = m_rectDraw.left - 1; j != m_rectDraw.right + 1; ++j)
		{
			COLORREF color = pDC->GetPixel(j, i);
			int nCb = (color & 0x00ff0000) >> 16;
			int nCg = (color & 0x0000ff00) >> 8;
			int nCr = color & 0x000000ff;
			//cout << (int)m_ImageAfter.m_pBits[0][i][j] << endl;
			m_ImageAfter.m_pBits[0][i][j] = (BYTE)nCb;
			m_ImageAfter.m_pBits[1][i][j] = (BYTE)nCg;
			m_ImageAfter.m_pBits[2][i][j] = (BYTE)nCr;

		}
	}
	ReleaseDC(pDC);
}

void CImage_ProcessingView::OnUpdateDrawAny(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bIsDrawing)
	{
		pCmdUI->SetText(_T("退出随意画"));
	}
	else
	{
		pCmdUI->SetText(_T("随意画"));
	}

	//pCmdUI->Enable()
}




void CImage_ProcessingView::OnFilterMotion()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	CMotionFilter mf;
	mf.m_dDistance = 50;
	mf.m_dAngle = 60;
	mf.m_dZoom = 0.1;
	CMyImage_double dImgTemp(m_ImageAfter), dImgTemp1(m_ImageAfter);

	mf.doMotionFilter(dImgTemp, dImgTemp1);
	dImgTemp1.CopyTo(m_ImageAfter);

	UpdateState();

}


void CImage_ProcessingView::OnDrawEllips()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
	{
		m_Image.Create(512, 512, 0);

	}

	//m_Image.Create(512,512,RGB(255,255,255));

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	//m_bIsDrawAny = 0;
	if (m_bIsDrawing)
	{
		m_bIsDrawing = false;
		m_nDrawType = -1;
	}
	else
	{
		m_bIsDrawing = true;
		m_nDrawType = 1; //1是画圆
	}

	UpdateState(false);

}


void CImage_ProcessingView::OnDrawRect()
{
	// TODO: Add your command handler code here


	if (m_Image.IsNull())
	{
		m_Image.Create(512, 512, 0);

	}

	//m_Image.Create(512,512,RGB(255,255,255));

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	//m_bIsDrawAny = 0;
	if (m_bIsDrawing)
	{
		m_bIsDrawing = false;
		m_nDrawType = -1;
	}
	else
	{
		m_bIsDrawing = true;
		m_nDrawType = 2; //2画矩形
	}

	UpdateState(false);

}


void CImage_ProcessingView::OnHoughLine()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ imgGrayed;
	doToGray(m_ImageAfter, imgGrayed);

	HoughTransformer ht;
	ht.HoughTransform(imgGrayed, m_ImageAfter);


	UpdateState();

}


void CImage_ProcessingView::OnFilterGaussian()
{
	// TODO: Add your command handler code here
	//double gtpl[9] = {0.0113,0.0838,0.0113,0.0838,0.6193,0.0838,0.0113,0.0838,0.0113}; //高斯模板
	//double gtpl[9] = {0.0585498,0.0965322,0.0585498,0.0965322,0.159155,0.0965322,0.0585498,0.0965322,0.0585498}; //高斯模板
	//double gtpl[9] = {0.0830516888888889,0.121034088888889,0.0830516888888889,0.121034088888889,0.183656888888889,0.121034088888889,0.0830516888888889,0.121034088888889,0.0830516888888889}; //高斯模板



	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	int nSize = 5;
	double *pTpl = new double[nSize*nSize];
	pTpl = m_com.getGussianKernalData(nSize / 2, 1);

	MyImage_ imgTemp(m_ImageAfter);
	//doImFilter(m_ImageAfter,vecTpl,imgTemp,2,true,true);
	doImFilter(imgTemp, pTpl, nSize, m_ImageAfter, 2, false, true);

	UpdateState();
}


void CImage_ProcessingView::OnMorphDilate()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	int nSize = 5;

	doMorphDilate(m_ImageAfter, m_ImageAfter, nSize);

	UpdateState();

}


void CImage_ProcessingView::doMorphDilate(const MyImage_ &srcImg, MyImage_ &dstImg, int nSize)
{
	if (srcImg.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (dstImg.IsNull())
		srcImg.CopyTo(dstImg);

	int nFtPara = nSize / 2;

	MyImage_ imgBackUp, imgFilled;
	//复制备份,防止产生迭代,处理过程中对此图处理，完成后再拷贝回dstImg
	srcImg.BorderFillTo(imgFilled, nFtPara, 2);
	srcImg.BorderFillTo(imgBackUp, nFtPara, 2);

	UINT w = imgFilled.GetWidth(); //重新获得填充后的长和宽
	UINT h = imgFilled.GetHeight();

	vector<UINT>  nVecPixelB; //新建三个容器，分别用来容纳三个通道用来排序的像素
	vector<UINT>  nVecPixelG;
	vector<UINT>  nVecPixelR;

	UINT nMidPos = (nFtPara * 2 + 1) * (nFtPara * 2 + 1) / 2; //获得模板中心位置

	//对整张填充后的图进行遍历
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数h-midpara
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-midpara次
		{
			nVecPixelB.clear();
			nVecPixelG.clear();
			nVecPixelR.clear();
			//对每个像素把模板对应的领域像素添加进容器中
			for (UINT m = i - nFtPara; m <= i + nFtPara; ++m)
			{
				for (UINT n = j - nFtPara; n <= j + nFtPara; ++n)
				{
					nVecPixelB.push_back(imgFilled.m_pBits[0][m][n]); //B
					nVecPixelG.push_back(imgFilled.m_pBits[1][m][n]); //G
					nVecPixelR.push_back(imgFilled.m_pBits[2][m][n]); //R
				}
			}
			sort(nVecPixelB.begin(), nVecPixelB.end()); //进行排序
			sort(nVecPixelG.begin(), nVecPixelG.end());
			sort(nVecPixelR.begin(), nVecPixelR.end());


			imgBackUp.m_pBits[0][i][j] = nVecPixelB[nVecPixelB.size() - 1]; //得到最大值
			imgBackUp.m_pBits[1][i][j] = nVecPixelG[nVecPixelG.size() - 1];
			imgBackUp.m_pBits[2][i][j] = nVecPixelR[nVecPixelR.size() - 1];

		}
	}
	imgBackUp.RemoveFillTo(dstImg, nFtPara); //去除边缘填充
	imgBackUp.Destroy(); //销毁中间变量的图像
	imgFilled.Destroy();
}


void CImage_ProcessingView::doMorphErode(const MyImage_ &srcImg, MyImage_ &dstImg, int nSize)
{
	if (srcImg.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	if (dstImg.IsNull())
		srcImg.CopyTo(dstImg);

	int nFtPara = nSize / 2;

	MyImage_ imgBackUp, imgFilled;
	//复制备份,防止产生迭代,处理过程中对此图处理，完成后再拷贝回dstImg
	srcImg.BorderFillTo(imgFilled, nFtPara, 2);
	srcImg.BorderFillTo(imgBackUp, nFtPara, 2);

	UINT w = imgFilled.GetWidth(); //重新获得填充后的长和宽
	UINT h = imgFilled.GetHeight();

	vector<UINT>  nVecPixelB; //新建三个容器，分别用来容纳三个通道用来排序的像素
	vector<UINT>  nVecPixelG;
	vector<UINT>  nVecPixelR;

	UINT nMidPos = (nFtPara * 2 + 1) * (nFtPara * 2 + 1) / 2; //获得模板中心位置

	//对整张填充后的图进行遍历
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数h-midpara
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-midpara次
		{
			nVecPixelB.clear();
			nVecPixelG.clear();
			nVecPixelR.clear();
			//对每个像素把模板对应的领域像素添加进容器中
			for (UINT m = i - nFtPara; m <= i + nFtPara; ++m)
			{
				for (UINT n = j - nFtPara; n <= j + nFtPara; ++n)
				{
					nVecPixelB.push_back(imgFilled.m_pBits[0][m][n]); //B
					nVecPixelG.push_back(imgFilled.m_pBits[1][m][n]); //G
					nVecPixelR.push_back(imgFilled.m_pBits[2][m][n]); //R
					imgBackUp.m_pBits[0][i][j] = nVecPixelB[0]; //得到最小值
					imgBackUp.m_pBits[1][i][j] = nVecPixelG[0];
					imgBackUp.m_pBits[2][i][j] = nVecPixelR[0];
				}
			}
			sort(nVecPixelB.begin(), nVecPixelB.end()); //进行排序
			sort(nVecPixelG.begin(), nVecPixelG.end());
			sort(nVecPixelR.begin(), nVecPixelR.end());

			imgBackUp.m_pBits[0][i][j] = nVecPixelB[0]; //得到最小值
			imgBackUp.m_pBits[1][i][j] = nVecPixelG[0];
			imgBackUp.m_pBits[2][i][j] = nVecPixelR[0];

		}
	}
	imgBackUp.RemoveFillTo(dstImg, nFtPara); //去除边缘填充
	imgBackUp.Destroy(); //销毁中间变量的图像
	imgFilled.Destroy();
}

void CImage_ProcessingView::OnMorphErode()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	int nSize = 5;

	doMorphErode(m_ImageAfter, m_ImageAfter, nSize);

	UpdateState();
}


void CImage_ProcessingView::OnMorphOpen()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);


	int nSize = 5;

	doMorphErode(m_ImageAfter, m_ImageAfter, nSize);
	doMorphDilate(m_ImageAfter, m_ImageAfter, nSize);
	UpdateState();

}


void CImage_ProcessingView::OnMorphClose()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	int nSize = 5;

	doMorphDilate(m_ImageAfter, m_ImageAfter, nSize);
	doMorphErode(m_ImageAfter, m_ImageAfter, nSize);
	UpdateState();
}


void CImage_ProcessingView::OnMorphTophat()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	int nSize = 5;

	MyImage_ imgOpen(m_ImageAfter);
	doMorphErode(imgOpen, imgOpen, nSize);
	doMorphDilate(imgOpen, imgOpen, nSize);

	//for (UINT j=0; j<m_nHeight; j++)
	//	for (UINT i=0; i<m_nWidth; i++)//两幅图像做简单相加操作
	//	{
	//		m_ImageAfter.m_pBits[0][j][i]=abs(m_ImageAfter.m_pBits[0][j][i]-imgOpen.m_pBits[0][j][i]);
	//		m_ImageAfter.m_pBits[1][j][i]=abs(m_ImageAfter.m_pBits[1][j][i]-imgOpen.m_pBits[1][j][i]);
	//		m_ImageAfter.m_pBits[2][j][i]=abs(m_ImageAfter.m_pBits[2][j][i]-imgOpen.m_pBits[2][j][i]);
	//	}


	for (UINT j = 0; j < m_nHeight; j++)
		for (UINT i = 0; i < m_nWidth; i++)//两幅图像做简单相加操作
		{
			m_ImageAfter.m_pBits[0][j][i] = SaturateCast(m_ImageAfter.m_pBits[0][j][i] - imgOpen.m_pBits[0][j][i]);
			m_ImageAfter.m_pBits[1][j][i] = SaturateCast(m_ImageAfter.m_pBits[1][j][i] - imgOpen.m_pBits[1][j][i]);
			m_ImageAfter.m_pBits[2][j][i] = SaturateCast(m_ImageAfter.m_pBits[2][j][i] - imgOpen.m_pBits[2][j][i]);
		}

	UpdateState();

}


void CImage_ProcessingView::OnMorphBlackhat()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	int nSize = 5;

	MyImage_ imgClose(m_ImageAfter);
	doMorphDilate(imgClose, imgClose, nSize);
	doMorphErode(imgClose, imgClose, nSize);

	//for (UINT j=0; j<m_nHeight; j++)
	//	for (UINT i=0; i<m_nWidth; i++)//两幅图像做简单相加操作
	//	{
	//		m_ImageAfter.m_pBits[0][j][i]=abs(m_ImageAfter.m_pBits[0][j][i]-imgOpen.m_pBits[0][j][i]);
	//		m_ImageAfter.m_pBits[1][j][i]=abs(m_ImageAfter.m_pBits[1][j][i]-imgOpen.m_pBits[1][j][i]);
	//		m_ImageAfter.m_pBits[2][j][i]=abs(m_ImageAfter.m_pBits[2][j][i]-imgOpen.m_pBits[2][j][i]);
	//	}


	for (UINT j = 0; j < m_nHeight; j++)
		for (UINT i = 0; i < m_nWidth; i++)//两幅图像做简单相加操作
		{
			m_ImageAfter.m_pBits[0][j][i] = SaturateCast(imgClose.m_pBits[0][j][i] - m_ImageAfter.m_pBits[0][j][i]);
			m_ImageAfter.m_pBits[1][j][i] = SaturateCast(imgClose.m_pBits[1][j][i] - m_ImageAfter.m_pBits[1][j][i]);
			m_ImageAfter.m_pBits[2][j][i] = SaturateCast(imgClose.m_pBits[2][j][i] - m_ImageAfter.m_pBits[2][j][i]);
		}

	UpdateState();
}


void CImage_ProcessingView::OnMorphBorderExtract()
{
	// TODO: Add your command handler code here

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	int nSize = 5;

	MyImage_ imgErode(m_ImageAfter);
	doMorphErode(imgErode, imgErode, nSize);

	//for (UINT j=0; j<m_nHeight; j++)
	//	for (UINT i=0; i<m_nWidth; i++)//两幅图像做简单相加操作
	//	{
	//		m_ImageAfter.m_pBits[0][j][i]=abs(m_ImageAfter.m_pBits[0][j][i]-imgOpen.m_pBits[0][j][i]);
	//		m_ImageAfter.m_pBits[1][j][i]=abs(m_ImageAfter.m_pBits[1][j][i]-imgOpen.m_pBits[1][j][i]);
	//		m_ImageAfter.m_pBits[2][j][i]=abs(m_ImageAfter.m_pBits[2][j][i]-imgOpen.m_pBits[2][j][i]);
	//	}


	for (UINT j = 0; j < m_nHeight; j++)
		for (UINT i = 0; i < m_nWidth; i++)//两幅图像做简单相加操作
		{
			m_ImageAfter.m_pBits[0][j][i] = SaturateCast(m_ImageAfter.m_pBits[0][j][i] - imgErode.m_pBits[0][j][i]);
			m_ImageAfter.m_pBits[1][j][i] = SaturateCast(m_ImageAfter.m_pBits[1][j][i] - imgErode.m_pBits[1][j][i]);
			m_ImageAfter.m_pBits[2][j][i] = SaturateCast(m_ImageAfter.m_pBits[2][j][i] - imgErode.m_pBits[2][j][i]);
		}

	UpdateState();
}


void CImage_ProcessingView::OnFilterMediumAutoadaptive() //还没做
{
	// TODO: 在此添加命令处理程序代码
	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();
	int nFtPara = m_nFtMidPara;
	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	MyImage_ imgBackUp, imgFilled;
	cout << m_nFtMidFillMode << endl;
	//复制备份,防止产生迭代,处理过程中对此图处理，完成后再拷贝回ImgAfter
	m_ImageAfter.BorderFillTo(imgFilled, m_nFtMidPara, m_nFtMidFillMode);
	m_ImageAfter.BorderFillTo(imgBackUp, m_nFtMidPara, m_nFtMidFillMode);

	UINT w = imgFilled.GetWidth(); //重新获得填充后的长和宽
	UINT h = imgFilled.GetHeight();

	vector<UINT>  nVecPixelB; //新建三个容器，分别用来容纳三个通道用来排序的像素
	vector<UINT>  nVecPixelG;
	vector<UINT>  nVecPixelR;

	UINT nMidPos = (nFtPara * 2 + 1) * (nFtPara * 2 + 1) / 2; //获得模板中心位置

															  //对整张填充后的图进行遍历
	for (UINT i = nFtPara; i < h - nFtPara; ++i) //循环次数h-nFtPara
	{
		for (UINT j = nFtPara; j < w - nFtPara; ++j)//循环w-nFtPara次
		{
			nVecPixelB.clear();
			nVecPixelG.clear();
			nVecPixelR.clear();
			//对每个像素把模板对应的领域像素添加进容器中
			for (UINT m = i - nFtPara; m <= i + nFtPara; ++m)
			{
				for (UINT n = j - nFtPara; n <= j + nFtPara; ++n)
				{
					nVecPixelB.push_back(imgFilled.m_pBits[0][m][n]); //B
					nVecPixelG.push_back(imgFilled.m_pBits[1][m][n]); //G
					nVecPixelR.push_back(imgFilled.m_pBits[2][m][n]); //R
				}
			}
			sort(nVecPixelB.begin(), nVecPixelB.end()); //进行排序
			sort(nVecPixelG.begin(), nVecPixelG.end());
			sort(nVecPixelR.begin(), nVecPixelR.end());
			imgBackUp.m_pBits[0][i][j] = nVecPixelB[nMidPos]; //得到中值
			imgBackUp.m_pBits[1][i][j] = nVecPixelG[nMidPos];
			imgBackUp.m_pBits[2][i][j] = nVecPixelR[nMidPos];
		}
	}
	imgBackUp.RemoveFillTo(m_ImageAfter, m_nFtMidPara); //去除边缘填充
	imgBackUp.Destroy(); //销毁中间变量的图像
	imgFilled.Destroy();
	ldTimeEnd = GetTickCount();
	std::cout << "中值滤波(标准库排序)完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s"
		<< " ,模板大小：" << (nFtPara * 2 + 1) << "x" << (nFtPara * 2 + 1) << std::endl;
	UpdateState();
}


void CImage_ProcessingView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (pScrollBar == NULL)
	//	cout << "NULL" << endl;
	//cout << nSBCode << "," << nPos << endl;
	////#define SB_THUMBPOSITION    4
	////#define SB_THUMBTRACK       5
	////#define SB_ENDSCROLL        8
	////每次都是548
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CImage_ProcessingView::OnEncodeHuffman()
{
	//// TODO: 在此添加命令处理程序代码
	//if (m_Image.IsNull())
	//	return;

	//if (m_ImageAfter.IsNull())
	//	m_Image.CopyTo(m_ImageAfter);

	//doToGray(m_ImageAfter, m_ImageAfter);

	//double hist[256] = { 0.0 };
	//for (int i = 0; i != m_ImageAfter.GetHeight(); ++i)
	//{
	//	for (int j = 0; j != m_ImageAfter.GetWidth(); ++j)
	//	{
	//		++hist[m_ImageAfter.m_pBits[0][i][j]];
	//	}
	//}

	//for (int i = 0; i != 256; ++i)
	//	hist[i] /= (m_ImageAfter.GetHeight()*m_ImageAfter.GetWidth());

	//map<int, int> order;
	//double hist_cp[256];
	//memcpy(hist_cp, hist, sizeof(hist));
	//sort(begin(hist_cp), end(hist_cp));

	//for (int i = 0; i != 256; ++i)
	//{
	//	order[i] = find(begin(hist_cp), end(hist_cp), hist[i]) - begin(hist_cp);
	//}

	////int pre = -1,count=0;
	////for (int i=0;i!=256;++i)
	////{
	////	if (i==0)
	////	{
	////		pre = order[0];
	////		count = 0;
	////		continue;
	////	}
	////	if (i!=0 && order[i]!=pre)
	////	{
	////		pre == order[i];
	////		count = 0;
	////		continue;
	////	}
	////	if (pre==order[i])
	////	{
	////		++count;
	////		order[i] += count;
	////	}
	////}


	////double hist1[6] = { 0.3,0.1,0.15,0.2,0.05,0.2 },hist1_cp[6];
	////memcpy(hist1_cp, hist1, sizeof(hist1));
	////map<int, int> order1;
	////sort(begin(hist1_cp), end(hist1_cp));

	////for (int i = 0; i != 6; ++i)
	////{
	////	order1[i] = find(begin(hist1_cp), end(hist1_cp), hist1[i]) - begin(hist1_cp);
	////}


	//vector<char> dict[256];
	//double sum = 0.0;
	//int min1, min2;
	//int *p=NULL;
	//int i = 1;
	//while (sum < 1.0)
	//{
	//	
	//	p = new int[256-i];
	//	min1 = getMapMinIndex(order);
	//	order[min1] = INT_MAX;
	//	min2 = getMapMinIndex(order);
	//	order[min2] = INT_MAX;
	//	sum = sum + hist[min1] + hist[min2];
	//	dict[min1].push_back(0);
	//	dict[min2].push_back(1);
	//	++i;

	//}


	//UpdateState();


}

int getMapMinIndex(map<int, int> &imap)
{
	int minIndex = 0;
	int minValue = imap[0];
	for (int i = 0; i != imap.size(); ++i)
	{
		if (imap[i] <= minValue)
		{
			minValue = imap[i];
			minIndex = i;
		}
	}
	//imap[minIndex] = INT_MAX;
	return minIndex;
}

const int MaxN = 256;

//struct HaffNode
//	//哈夫曼树的结点结构
//{
//	int weight;								//权值
//	int flag;								//标记
//	int parent;								//双亲结点下标
//	int leftChild;							//左孩子下标
//	int rightChild;							//右孩子下标
//};
//
//struct Code
//	//存放哈夫曼编码的数据元素结构
//{
//	int bit[MaxN];							//数组
//	int start;								//编码的起始下标
//	int weight;								//字符的权值
//};
//
////void Haffman(int weight[], int n, HaffNode haffTree[])
//////建立叶结点个数为n权值为weight的哈夫曼树haffTree
////{
////	int j, m1, m2, x1, x2;
////
////	//哈夫曼树haffTree初始化。n个叶结点的哈夫曼树共有2n-1个结点
////	for (int i = 0; i < 2 * n - 1; i++)
////	{
////		if (i < n) haffTree[i].weight = weight[i];
////		else  haffTree[i].weight = 0;
////		haffTree[i].parent = 0;
////		haffTree[i].flag = 0;
////		haffTree[i].leftChild = -1;
////		haffTree[i].rightChild = -1;
////	}
////
////	//构造哈夫曼树haffTree的n-1个非叶结点
////	for (int i = 0; i < n - 1; i++)
////	{
////		m1 = m2 = MaxValue;
////		x1 = x2 = 0;
////		for (j = 0; j < n + i; j++)
////		{
////			if (haffTree[j].weight < m1 && haffTree[j].flag == 0)
////			{
////				m2 = m1;
////				x2 = x1;
////				m1 = haffTree[j].weight;
////				x1 = j;
////			}
////			else if (haffTree[j].weight < m2 && haffTree[j].flag == 0)
////			{
////				m2 = haffTree[j].weight;
////				x2 = j;
////			}
////		}
////
////		//将找出的两棵权值最小的子树合并为一棵子树
////		haffTree[x1].parent = n + i;
////		haffTree[x2].parent = n + i;
////		haffTree[x1].flag = 1;
////		haffTree[x2].flag = 1;
////		haffTree[n + i].weight = haffTree[x1].weight + haffTree[x2].weight;
////		haffTree[n + i].leftChild = x1;
////		haffTree[n + i].rightChild = x2;
////	}
////}
//
//void HaffmanCode(HaffNode haffTree[], int n, Code haffCode[])
////由n个结点的哈夫曼树haffTree构造哈夫曼编码haffCode
//{
//	Code *cd = new Code;
//	int child, parent;
//
//	//求n个叶结点的哈夫曼编码
//	for (int i = 0; i < n; i++)
//	{
//		cd->start = n - 1;					//不等长编码的最后一位为n-1
//		cd->weight = haffTree[i].weight;	//取得编码对应权值的字符
//		child = i;
//		parent = haffTree[child].parent;
//
//		//由叶结点向上直到根结点
//		while (parent != 0)
//		{
//			if (haffTree[parent].leftChild == child)
//				cd->bit[cd->start] = 0;				//左孩子结点编码0
//			else
//				cd->bit[cd->start] = 1;				//右孩子结点编码1
//			cd->start--;
//			child = parent;
//			parent = haffTree[child].parent;
//		}
//
//		//保存叶结点的编码和不等长编码的起始位
//		for (int j = cd->start + 1; j < n; j++)
//			haffCode[i].bit[j] = cd->bit[j];
//		haffCode[i].start = cd->start;
//		haffCode[i].weight = cd->weight;		//记住编码对应权值的字符
//	}
//}
//
//
//
//typedef struct HTNode {
//	int weight;
//	int parent;
//	int lchild;
//	int rchild;
//}HTNode, *HuffmanTree;
//typedef char** HuffmanCode;
//
//void Select(HuffmanTree HT, int num, int &child1, int &child2);
//
//void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, int n) {
//	//
//	int m, i;
//	int child1, child2;
//	if (n <= 1) {
//		return;
//	}
//	m = n * 2 - 1;//整棵树的节点数
//	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));//申请足够空间
//	for (i = 1; i <= n; i++, w++) {
//		HT[i] = { *w, 0, 0, 0 };
//	}
//	for (; i <= m; i++) {
//		HT[i] = { 0, 0, 0, 0 };
//	}
//	for (i = n + 1; i <= m; i++) {
//		Select(HT, i - 1, child1, child2);
//		HT[child1].parent = i;
//		HT[child2].parent = i;
//		HT[i].lchild = child1;
//		HT[i].rchild = child2;
//		HT[i].weight = HT[child1].weight + HT[child2].weight;
//		printf("%d==%d\n", child1, child2);
//	}
//
//	HC = (char**)malloc((n + 1) * sizeof(char *));
//
//	char *cd = (char*)malloc(n * sizeof(char));
//	//    memset(cd, '\0', n*sizeof(char));
//	int c = 0;
//
//	int tempParent, count;
//	for (i = 1; i <= n; i++) {
//		count = 0;
//		for (c = i, tempParent = HT[i].parent; tempParent != 0; c = tempParent, tempParent = HT[tempParent].parent) {
//			if (HT[tempParent].lchild == c) {
//				cd[count++] = '0';
//			}
//			else {
//				cd[count++] = '1';
//			}
//		}
//		cd[count] = '\0';
//		printf("%s~%d\n", cd, i);
//		HC[i] = (char *)malloc((count) * sizeof(char));
//
//		strcpy(HC[i], cd);
//		//memset(cd,'\0', n*sizeof(char));//error
//	}
//}
//
//void Select(HuffmanTree HT, int num, int &child1, int &child2) {
//	child1 = 0;
//	child2 = 0;
//	int w1 = 0;
//	int w2 = 0;
//	for (int i = 1; i <= num; i++) {
//		if (HT[i].parent == 0) {
//			if (child1 == 0) {
//				child1 = i;
//				w1 = HT[i].weight;
//				continue;
//			}
//			if (child2 == 0) {
//				child2 = i;
//				w2 = HT[i].weight;
//				continue;
//			}
//			if (w1 > w2 && w1 > HT[i].weight) {
//				w1 = HT[i].weight;
//				child1 = i;
//				continue;
//			}
//			if (w2 > w1 && w2 > HT[i].weight) {
//				w2 = HT[i].weight;
//				child2 = i;
//				continue;
//			}
//		}
//	}
//}
//
//
////int main(int argc, const char * argv[]) {
////	char a[] = "abcaab";
////	int i = (int)strlen(a);
////	printf("%d\n", i);
////
////	int b[] = { 1,2,3,4 };
////	HuffmanTree HT;
////	HuffmanCode HC;
////	HuffmanCoding(HT, HC, b, 4);
////	for (i = 1; i <= 7; i++) {
////		printf("%d-%d\n", HT[i].weight, HT[i].parent);
////	}
////	for (i = 1; i <= 4; i++) {
////		printf("%s\n", HC[i]);
////	}
////	return 0;
////}




void CImage_ProcessingView::OnEncodeShannon()
{
	// TODO: 在此添加命令处理程序代码
	long double ldTimeStart, ldTimeEnd;
	ldTimeStart = GetTickCount();

	if (m_Image.IsNull())
		return;

	if (m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	OnTogray();

	double hist[256] = { 0.0 }, hist_add[256] = { 0.0 }, hist_sort[256] = { 0.0 };
	for (int i = 0; i != m_ImageAfter.GetHeight(); ++i)
	{
		for (int j = 0; j != m_ImageAfter.GetWidth(); ++j)
		{
			++hist[m_ImageAfter.m_pBits[0][i][j]];
		}
	}

	for (int i = 0; i != 256; ++i)
		hist[i] /= (m_ImageAfter.GetHeight()*m_ImageAfter.GetWidth());

	int index[256]; //得到索引数组
	for (int i = 0; i != 256; ++i)
		index[i] = i;
	
	memcpy(hist_sort, hist, sizeof(hist));
	m_com.sortindex(hist_sort,index,256);

	//计算累计概率
	hist_add[1] = hist_sort[0];
	for (int i = 2; i != 256; ++i)
		hist_add[i] += hist_add[i - 1] + hist_sort[i - 1];

	int nCodeDigits[256];
	//计算Li(编码位数)
	for (int i = 0; i != 256; ++i)
	{
		int temp = (int)ceil(-log2(hist_sort[i]));
		nCodeDigits[i] = temp < 0 ? 0 : temp;
	}

	//计算码表
	vector<vector<char>> dict;
	for (int i = 0; i != 256; ++i)
	{
		vector<char> code;
		if (nCodeDigits[i]!=0)
		{
			code = m_com.DecimalDec2Bin(hist_add[i], nCodeDigits[i]);
		}
		dict.push_back(code); //若不需要编码的则推入0size的
	}


	//对图像进行编码
	vector<vector<char>> codes;
	int curIndex,temp;
	for (int i=0;i!=m_ImageAfter.GetHeight();++i)
	{
		for (int j=0;j!=m_ImageAfter.GetWidth();++j)
		{
			//通过现在的灰度级,找到对应码表的位置
			temp = m_ImageAfter.m_pBits[0][i][j];
			for (int k=0;k!=256;++k)
			{
				if (temp == index[k])
				{
					curIndex = k;
					break;
				}
			}
			codes.push_back(dict[curIndex]);
		}
	}
	
	//计算平均码长,信源熵,编码效率
	double HX = 0.0, L = 0.0;
	for (int i=0;i!=256;++i)
	{
		if (hist[i]!=0)
			HX += (hist[i] * -log2(hist[i]));
		if (hist_sort[i]!=0)
		{
			L += hist_sort[i] * nCodeDigits[i];
		}
	}
	cout << "信源熵为:" << HX << endl;
	cout << "平均码长为:" << L << endl;
	cout << "编码效率:" << HX / L << endl;


	//对图像进行解码
	//先创建空白图像
	MyImage_ imgDecode(m_ImageAfter.GetWidth(),m_ImageAfter.GetHeight());

	//开始解码
	int idx = 0,decodeValue=-1;
	int match_digits = 0;
	for (int i=0;i!=m_ImageAfter.GetHeight();++i)
	{
		for (int j=0;j!=m_ImageAfter.GetHeight();++j)
		{
			idx = i*m_ImageAfter.GetWidth() + j;
			decodeValue = -1; //首先让当前像素的解码值为-1表示还未找到
			//搜索码表找到编码对应原灰度值
			for (int k=0;k!=256 && decodeValue==-1;++k)
			{
				match_digits = 0;
				if (dict[k].size()==codes[idx].size() && decodeValue==-1) //首先要求长度一致
				{
					for (int it=0;it!= dict[k].size();++it) //然后对比每一位是否一致
					{
						if (dict[k][it] != codes[idx][it])
							break;
						else
							++match_digits;
					}
					if(match_digits== dict[k].size())
						decodeValue = k;
				}
			}
			imgDecode.m_pBits[0][i][j] = index[decodeValue]; //最终再回到索引数组里找到真正的原始灰度值
			imgDecode.m_pBits[1][i][j] = index[decodeValue]; //
			imgDecode.m_pBits[2][i][j] = index[decodeValue]; 
		}
	}

	imgDecode.Save(L"decoded.bmp"); //保存一张方便对比
	imgDecode.CopyTo(m_ImageToDlgShow);
	CDlgShowImg *pDlg = new CDlgShowImg(_T("香农编码解码结果"));
	pDlg->Create(IDD_DLG_SHOW_IMG, this);
	pDlg->ShowWindow(SW_SHOW);

	ldTimeEnd = GetTickCount();
	std::cout << "香农编码、解码完成，" << "耗时：" << (ldTimeEnd - ldTimeStart) / 1000 << "s" << endl;
}


