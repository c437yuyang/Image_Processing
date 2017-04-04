// DlgCannyThresh.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "Image_ProcessingView.h"
#include "MainFrm.h"
#include "DlgCannyThresh.h"
#include "afxdialogex.h"


// CDlgCannyThresh dialog

IMPLEMENT_DYNAMIC(CDlgCannyThresh, CDialogEx)

CDlgCannyThresh::CDlgCannyThresh(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCannyThresh::IDD, pParent)
{
	m_nLowThresh = 40;
	m_nHighThresh = 80;
	//CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	//m_pCanny = pView->doCanny1;
}

CDlgCannyThresh::~CDlgCannyThresh()
{

}

void CDlgCannyThresh::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_canny, m_SliderCanny);
	DDX_Control(pDX, IDC_SLIDER_BINARY, m_SliderBinaryThresh);
}


BEGIN_MESSAGE_MAP(CDlgCannyThresh, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_canny, &CDlgCannyThresh::OnNMCustomdrawSlidercanny)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CDlgCannyThresh::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCannyThresh::OnBnClickedCancel)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_canny, &CDlgCannyThresh::OnNMReleasedcaptureSlidercanny)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BINARY, &CDlgCannyThresh::OnNMReleasedcaptureSliderBinary)
END_MESSAGE_MAP()


// CDlgCannyThresh message handlers


BOOL CDlgCannyThresh::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_SliderCanny.SetRange(1,254);
	m_SliderCanny.SetTicFreq(1);
	m_SliderCanny.SetPos(40);
	m_SliderBinaryThresh.SetRange(1,254);
	m_SliderBinaryThresh.SetTicFreq(1);
	m_SliderBinaryThresh.SetPos(80);

	CString str;
	str.Format(_T("LowThresh:%d"),m_SliderCanny.GetPos());
	SetDlgItemText(IDC_STATIC_CANNY_THRESH,str);

	str.Format(_T("HighThresh:%d"),m_SliderBinaryThresh.GetPos());
	SetDlgItemText(IDC_STATIC_BINARY_THRESH,str);

	GetParent()->EnableWindow(FALSE);
	this->EnableWindow();

	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	pView->doCanny1(pView->m_ImageCanny,pView->m_ImageAfter,m_nLowThresh,m_nHighThresh);
	pView->UpdateState(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgCannyThresh::OnNMCustomdrawSlidercanny(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CDlgCannyThresh::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	if(pScrollBar->GetDlgCtrlID() == IDC_SLIDER_canny)
	{
		str.Format(_T("LowThresh:%d"),((CSliderCtrl*)pScrollBar)->GetPos());
		SetDlgItemText(IDC_STATIC_CANNY_THRESH,str);
		m_nLowThresh = ((CSliderCtrl*)pScrollBar)->GetPos();


	}
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_BINARY)
	{
		str.Format(_T("HighThresh:%d"),((CSliderCtrl*)pScrollBar)->GetPos());
		SetDlgItemText(IDC_STATIC_BINARY_THRESH,str);
		m_nHighThresh = ((CSliderCtrl*)pScrollBar)->GetPos();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgCannyThresh::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->UpdateState(true);
	CDialogEx::OnOK();
}

void CDlgCannyThresh::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_ImageCanny.CopyTo(pView->m_ImageAfter);
	pView->UpdateState(false);

	CDialogEx::OnCancel();
}


void CDlgCannyThresh::OnNMReleasedcaptureSlidercanny(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	pView->doCanny1(pView->m_ImageCanny,pView->m_ImageAfter,m_nLowThresh,m_nHighThresh);
	pView->UpdateState(false);

	*pResult = 0;
}


void CDlgCannyThresh::OnNMReleasedcaptureSliderBinary(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	pView->doCanny1(pView->m_ImageCanny,pView->m_ImageAfter,m_nLowThresh,m_nHighThresh);
	pView->UpdateState(false);

	*pResult = 0;
}


void CDlgCannyThresh::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_ImageCanny.Destroy();
	delete this;

	CDialogEx::PostNcDestroy();
}
