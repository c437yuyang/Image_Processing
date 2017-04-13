
// Image_ProcessingView.h : CImage_ProcessingView 类的接口
//

#pragma once
#include "Image_ProcessingDoc.h"
#include "NoiseDlg.h"
#include "DlgFilterMedium.h"
#include "DlgFilterAVG.h"
#include "DlgHist.h"
#include "DlgFilterHPFSet.h"
#include "DlgFilterLPFSet.h"
#include "DlgShowImg.h"
#include "DlgInterPolationSet.h"
#include "DlgCannyThresh.h"
#include "DlgThresh.h"
#include "DlgFilterTplSet.h"
#include "DlgChooseParam.h"

#include "PropSheetRGB.h"
#include "Common.h"
#include "MyImage_.h"
#include "MyImage_double.h"
#include "ImagesStock.h"
#include "MotionFilter.h"
#include "HoughTransformer.h"
#include "Huffman.h"
#include "MyHuffman.h"
#include "HuffmanCoding.h"

#include <cmath>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>
#include <fstream>
#include<stdlib.h>
#include<time.h>
#include <memory>



class CImage_ProcessingView : public CScrollView
{
	friend class CNoiseDlg;
	friend class CDlgCannyThresh;
	friend class CDlgThresh;
	friend class CDlgFilterTplSet;
protected: // 仅从序列化创建
	CImage_ProcessingView();
	DECLARE_DYNCREATE(CImage_ProcessingView)

// 特性
public:
	CImage_ProcessingDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImage_ProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
public:
	CCommon m_com;
	MyImage_ m_Image,m_ImageAfter,m_ImageGrayed,m_ImageFT,m_ImageToDlgShow,m_ImageInterpolationBackUp;
	//std::map<int,double> m_dMapHist;
	std::map<int,double> m_dMapHistR,m_dMapHistG,m_dMapHistB; //用于直方图存储RGB强度信息
	std::map<int,double> m_dMapHistH,m_dMapHistS,m_dMapHistI; //用于直方图存储HSI强度信息
	std::vector<double> m_vecHistProb;//中间变量，存储直方图概率信息

	complex<double>* m_pFD;//用于在fft后存储频谱信息,rfft后会被删掉
	
	
	CImagesStock m_imgStock; //图像历史信息存储

	UINT m_nResDownRate; 
	UINT m_nGrayRate;
	UINT m_nCycleTimes;
	UINT m_nQuantiStart;
	UINT m_nQuantiIncrease;
	UINT m_nWidth;
	UINT m_nHeight;

	BOOL m_bIsProcessed; //状态信息
	BOOL m_bIsGrayed;
	BOOL m_bIsQuatilized;
	BOOL m_bMenuEnable;
	bool m_bIsFFTed;


	CPoint m_ptImgSegOrigin; //图像分割参数设置
	BOOL m_bIsImgSEGing;
	BOOL m_bImgSegRectSelectStart;
	int m_nImgSegType;

	CNoiseDlg m_dlgNoiseSet; //噪声参数设置
	double m_dNoiseCount;
	UINT m_nBitCoefficient;
	double m_dNoisePara1;
	double m_dNoisePara2;
	
	
	CDlgFilterMedium m_dlgFtMidSet;//中值滤波参数设置(统计排序滤波)
	UINT m_nFtMidPara;
	int m_nFtMidFillMode;
	int m_nFTSortMode;
	int m_nFTAlphaAvg_d;

	CDlgFilterAVG m_dlgFtAvgSet;//均值滤波参数设置
	int m_nFtAvgFillMode;
	UINT m_nFtAvgPara;
	int m_nFTAVGMode;
	double m_dFTAVGNiXieQ;


	CDlgFilterHPFSet m_dlgFtHPFSet;//高通滤波器参数设置;
	double m_dHPFRadius; 
	double m_dHPFEnforcePara;
	double m_dHPFOriImgPara;
	double m_nBHPFOrder;
	int m_nHPFMode;

	CDlgFilterLPFSet m_dlgFtLPFSet;//高通滤波器参数设置;
	double m_dLPFRadius; 
	double m_nBLPFOrder;
	int m_nLPFMode;

	CDlgInterPolationSet m_dlgInterpolationSet; //插值参数设置
	UINT m_nInterPolationHeight;
	UINT m_nInterPolationWidth;
	int m_nInterpolationType;

	//中间图像备份
	//CDlgCannyThresh m_dlgCannyThreshSet; //canny算法相关
	MyImage_ m_ImageCanny;
	MyImage_ m_ImageThresh; //threshDlg相关
	MyImage_ m_ImageTpl;	//tplSetDlg相关
	MyImage_ m_ImageSegShowRect;


	//bool m_bIsDrawAny; //画图菜单相关
	int m_nDrawType;
	bool m_bIsLBDown;
	CPoint m_ptDrawOrigin;
	CRect m_rectDraw;
	bool m_bIsDrawing;

	double m_dParam1; //选择参数对话框相关

	afx_msg void OnDrawline();
	afx_msg void OnFileSaveAs();
	afx_msg void OnShowred();
//	afx_msg void OnViewToolbar();
	afx_msg void OnDoubleimage();
private:
	// save the original file's name
	CString m_strFileNameSave;
	void Im2Gray();
	void Im2Bit();
	void CImage_ProcessingView::CalcHistPorb(UINT nCalcFlag);
	void CImage_ProcessingView::FFT_IMG(MyImage_ &imgInput,MyImage_ &imgOutput,int flag=0);
	void CImage_ProcessingView::ImageNormalize(const MyImage_ &InputImage,MyImage_ &OutputImage);
	void CImage_ProcessingView::vecValuesToImage(vector<BYTE> &vecValues); //用于恢复历史记录的时候
	void CImage_ProcessingView::UpdateState(bool bIsStoreImage = true);
	void CImage_ProcessingView::UpdateStateButNotStoreImage();
	void ChangeScrollSize();
	void calcHistLocal(MyImage_ &img,map<UINT,UINT> &nMapHistB,
		map<UINT,UINT> &nMapHistG,
		map<UINT,UINT> &nMapHistR,
		UINT x,UINT y,int nFtPara);

	void CImage_ProcessingView::doToGray(const MyImage_ &srcImg,MyImage_ &dstImg);//灰度化，支持In-Plaece操作
	void CImage_ProcessingView::doCanny(MyImage_ &srcImg,MyImage_ &dstImg,int nCannyThresh,int nBinaryThresh); //实现方式1
	void CImage_ProcessingView::doCanny1(MyImage_ &srcImg,MyImage_ &dstImg,int nLowThresh,int nHighThresh); //实现方式2
	void CImage_ProcessingView::doCanny2(MyImage_ &srcImg,MyImage_ &dstImg,int nLowThresh,int nHighThresh); //方式2解决边缘
	void CImage_ProcessingView::doThreshold(const MyImage_ &srcImg,MyImage_ &dstImg,int nThresh,double dThresh);//基本的基于灰度值的阈值分割，支持IN-Place
	void CImage_ProcessingView::doImFilter(const MyImage_ &srcImg,vector<double> &tpl,MyImage_ &dst,int nFillMode=2,bool bIsGrayed=false,bool bIs = TRUE);
	void CImage_ProcessingView::doImFilter(const MyImage_ &srcImg,double *pTpl ,int nTplSize,
		MyImage_ &dstImg,int nFillMode=2,bool bIsGrayed=false,
		bool bIsSaturated=true);
	bool CImage_ProcessingView::CalcImageAverage(MyImage_ &srcImg,bool bIsCalcGray,vector<int> &nVevAvg);
	int CImage_ProcessingView::getGlobalThresh(MyImage_ &srcImg);
	int CImage_ProcessingView::GetOtsuThresh(MyImage_ &srcImg);
	void CImage_ProcessingView::CopyViewImage(CRect rect);
	void CImage_ProcessingView::edgeLink(int x1,int y1,int index,double lowThresh,vector<double> &dVecMid,vector<double> &dVecHigh,vector<bool> &bVec,vector<bool> &bVec1);
	void doMorphDilate(const MyImage_ &srcImg,MyImage_ &dstImg,int nSize);
	void doMorphErode(const MyImage_ &srcImg,MyImage_ &dstImg,int nSize);

	void PaddingImage(const MyImage_ &srcImg, MyImage_ &dstImg, COLORREF color,int pads); //图像右下角填充指定颜色
	void CImage_ProcessingView::function(MyImage_ &img, int  nBlockSize, int xPos, int yPos);
public:
	afx_msg void OnReverse();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTogray();
	afx_msg void OnResdwon();
	afx_msg void OnRetrieve();
	afx_msg void OnTest();
	afx_msg void OnGraytohalf();
	afx_msg void OnGray2();
	afx_msg void OnGray4();
	afx_msg void OnGray64();
	afx_msg void OnGray128();
	afx_msg void OnGray32();
	afx_msg void OnGray16();
	afx_msg void OnGray8();
	afx_msg void OnUpdateGray2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGray4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGray8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGray16(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGray32(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGray64(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGray128(CCmdUI *pCmdUI);

	afx_msg void OnUpdateTogray(CCmdUI *pCmdUI);
	afx_msg void OnNoiseSalt();
	afx_msg void OnNoiseSet();
private:
	int m_nNoiseType;
public:
	afx_msg void OnBit1();
	afx_msg void OnBit2();
	afx_msg void OnBit3();
	afx_msg void OnBit4();
	afx_msg void OnBit5();
	afx_msg void OnBit6();
	afx_msg void OnBit7();
	afx_msg void OnBit8();
	afx_msg void OnHistShow();
	afx_msg void OnFilterAvg();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFilterMedium();
	afx_msg void OnFtmidSet();
	afx_msg void OnFtavgSet();
	afx_msg void OnHistEqRGB();
	afx_msg void OnCalcEntropy();
	afx_msg void OnCalcGradient();
	afx_msg void OnIdFilterMedium2();
	afx_msg void OnFilterMediumvecsort();
	afx_msg void OnFilterMediumHist();
	afx_msg void OnFft();
	afx_msg void OnFftReverse();
	afx_msg void OnFilter();
	afx_msg void OnSharp();
	afx_msg void OnHistGdh();
	afx_msg void OnFilterIlpf();
	afx_msg void OnRebuiltFromPhase();
	afx_msg void OnFilterIhpf();
	afx_msg void OnFilterHpfSet();
private:
	void ShowImgInDlg(CString strWindowName);
public:
	afx_msg void OnFilterLpfSet();
	afx_msg void OnFilterSort();
	afx_msg void OnRebuiltFromAmplitude();
	afx_msg void OnGenerateimg();
	afx_msg void OnImageSub();
	afx_msg void OnFliterMotionBlur();
	afx_msg void OnFilterTurbulenceBlur();
	afx_msg void OnInterpolationNearest();
	afx_msg void OnInterpolationSet();
	afx_msg void OnRotate();
	afx_msg void OnHistEqHsi();
	afx_msg void On32904();
	afx_msg void OnShowR();
	afx_msg void OnShowG();
	afx_msg void OnShowB();
	afx_msg void OnShowH();
	afx_msg void OnShowS();
	afx_msg void OnShowI();

	afx_msg void OnImgsegRgb();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnImgsegHsi();
	afx_msg void OnGrayworld();
	afx_msg void OnFilterGradient();
	afx_msg void OnFilterThreshold();
	afx_msg void OnFilterGaussLaplacian();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnFilterGradientAmL2();
	afx_msg void OnFilterGradientAngle();
	afx_msg void OnCanny();
	afx_msg void Ontest1();
	afx_msg void Oncannydouble();
	afx_msg void OnThreshGlobalbasic();
	afx_msg void OnThreshOtsu();
	afx_msg void OnDrawAny();
	afx_msg void OnUpdateDrawAny(CCmdUI *pCmdUI);
	afx_msg void OnFilterMotion();
	afx_msg void OnDrawEllips();
	afx_msg void OnDrawRect();
	afx_msg void OnHoughLine();
	afx_msg void OnFilterGaussian();
	afx_msg void OnMorphDilate();
	
	afx_msg void OnMorphErode();
	afx_msg void OnMorphOpen();
	afx_msg void OnMorphClose();
	afx_msg void OnMorphTophat();
	afx_msg void OnMorphBlackhat();
	afx_msg void OnMorphBorderExtract();
	afx_msg void OnFilterMediumAutoadaptive();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEncodeHuffman();
	afx_msg void OnEncodeShannon();
	afx_msg void OnEncodeBitPlane();
	afx_msg void OnEncodeHuffman2();
	afx_msg void OnEncodeRunlength();
	afx_msg void OnEncodeShivering();
	afx_msg void OnEncodeBlockCut();
	afx_msg void OnEncodeEnclosing();
};

#ifndef _DEBUG  // Image_ProcessingView.cpp 中的调试版本
inline CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const
   { return reinterpret_cast<CImage_ProcessingDoc*>(m_pDocument); }
#endif

