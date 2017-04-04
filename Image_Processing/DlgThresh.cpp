// DlgThresh.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "Image_ProcessingView.h"
#include "MainFrm.h"
#include "DlgThresh.h"
#include "afxdialogex.h"


// CDlgThresh dialog

IMPLEMENT_DYNAMIC(CDlgThresh, CDialogEx)

CDlgThresh::CDlgThresh(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgThresh::IDD, pParent)
{
	m_nThresh = 100;
}

CDlgThresh::~CDlgThresh()
{
}

void CDlgThresh::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_THRESH, m_SliderThresh);
	DDX_Control(pDX, IDC_COMBO_THRESHMODE, m_comboThreshMode);
}


BEGIN_MESSAGE_MAP(CDlgThresh, CDialogEx)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_THRESH, &CDlgThresh::OnNMReleasedcaptureSliderThresh)
	ON_CBN_SELCHANGE(IDC_COMBO_THRESHMODE, &CDlgThresh::OnCbnSelchangeComboThreshmode)
	ON_BN_CLICKED(IDCANCEL, &CDlgThresh::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgThresh::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgThresh message handlers


BOOL CDlgThresh::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_comboThreshMode.AddString(_T("otsu(最大方差法)"));
	m_comboThreshMode.AddString(_T("基本全局阈值法"));
	m_comboThreshMode.AddString(_T("自定义"));

	m_comboThreshMode.SetCurSel(0);


	GetParent()->EnableWindow(FALSE);
	this->EnableWindow();

	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	


	if (m_comboThreshMode.GetCurSel()==0)
	{
		m_nThresh = pView->GetOtsuThresh(pView->m_ImageThresh);
	}
	else if (m_comboThreshMode.GetCurSel()==1)
	{
		m_nThresh = pView->getGlobalThresh(pView->m_ImageThresh);
	}
	else
	{
		m_nThresh = 100;
	}

	pView->doThreshold(pView->m_ImageThresh,pView->m_ImageAfter,m_nThresh,0.0);
	pView->UpdateState(false);


	m_SliderThresh.SetRange(1,254);
	m_SliderThresh.SetTicFreq(1);
	m_SliderThresh.SetPos(m_nThresh);

	CString str;
	str.Format(_T("阈值:%d"),m_SliderThresh.GetPos());
	SetDlgItemText(IDC_STATIC_THRESHDLG,str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgThresh::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgThresh::OnNMReleasedcaptureSliderThresh(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	m_comboThreshMode.SetCurSel(2); //设置为自定义
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	m_nThresh = m_SliderThresh.GetPos();
	CString str;
	str.Format(_T("阈值:%d"),m_SliderThresh.GetPos());
	SetDlgItemText(IDC_STATIC_THRESHDLG,str);

	pView->doThreshold(pView->m_ImageThresh,pView->m_ImageAfter,m_nThresh,0.0);
	pView->UpdateState(false);
	*pResult = 0;
}


void CDlgThresh::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDlgThresh::OnCbnSelchangeComboThreshmode()
{
	// TODO: Add your control notification handler code here

	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	if (m_comboThreshMode.GetCurSel()==0)
	{
		m_nThresh = pView->GetOtsuThresh(pView->m_ImageThresh);
	}
	else if (m_comboThreshMode.GetCurSel()==1)
	{
		m_nThresh = pView->getGlobalThresh(pView->m_ImageThresh);
	}
	else
	{
		m_nThresh = m_SliderThresh.GetPos();
	}

	CString str;
	str.Format(_T("阈值:%d"),m_nThresh);
	SetDlgItemText(IDC_STATIC_THRESHDLG,str);
	pView->doThreshold(pView->m_ImageThresh,pView->m_ImageAfter,m_nThresh,0.0);
	pView->UpdateState(false);
	m_SliderThresh.SetPos(m_nThresh);
}


void CDlgThresh::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	GetParent()->EnableWindow(TRUE);
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_ImageThresh.CopyTo(pView->m_ImageAfter);
	pView->UpdateState(false);

	CDialogEx::OnCancel();
}


void CDlgThresh::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->UpdateState(true);
	CDialogEx::OnOK();
}
