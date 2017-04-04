// DlgInterPolationSet.cpp : implementation file
//

#include "stdafx.h"
//#include "Image_ProcessingView.h" //包含就出错,why?
#include "MainFrm.h"
#include "Image_Processing.h"
#include "DlgInterPolationSet.h"
#include "afxdialogex.h"


// CDlgInterPolationSet dialog

IMPLEMENT_DYNAMIC(CDlgInterPolationSet, CDialogEx)

CDlgInterPolationSet::CDlgInterPolationSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInterPolationSet::IDD, pParent)
	, m_nInterPolationHeight(100)
	, m_nInterPolationWidth(100)
	, m_nInterpolationType(2) //默认双三次内插
{

}

CDlgInterPolationSet::~CDlgInterPolationSet()
{
}

void CDlgInterPolationSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INTERPOLATION_HEIGHT, m_nInterPolationHeight);
	DDX_Text(pDX, IDC_EDIT_INTERPOLATION_WIDTH, m_nInterPolationWidth);
	DDX_Radio(pDX, IDC_RADIO_nearest, m_nInterpolationType);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
}


BEGIN_MESSAGE_MAP(CDlgInterPolationSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgInterPolationSet::OnBnClickedOk)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgInterPolationSet message handlers


void CDlgInterPolationSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDialogEx::OnOK();
}


BOOL CDlgInterPolationSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Slider.SetRange(0,300);
	m_Slider.SetTicFreq(5);
	m_Slider.SetPos(100);
	CString str;
	str.Format(_T("%%%d"),m_Slider.GetPos());
	SetDlgItemText(IDC_STATIC_SCALE,str);
	//CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	m_nInterPolationWidth = (UINT)(m_nInterPolationWidth_origin * ((double)m_Slider.GetPos()/100));
	m_nInterPolationHeight = (UINT)(m_nInterPolationHeight_origin * ((double)m_Slider.GetPos()/100));
 	SetDlgItemInt(IDC_EDIT_INTERPOLATION_WIDTH,m_nInterPolationWidth);
 	SetDlgItemInt(IDC_INTERPOLATION_HEIGHT,m_nInterPolationHeight);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgInterPolationSet::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	if(pScrollBar->GetDlgCtrlID() == IDC_SLIDER1)
	{
		str.Format(_T("%%%d"),((CSliderCtrl*)pScrollBar)->GetPos());
		SetDlgItemText(IDC_STATIC_SCALE,str);
		m_nInterPolationWidth = (UINT)(m_nInterPolationWidth_origin * ((double)m_Slider.GetPos()/100));
		m_nInterPolationHeight = (UINT)(m_nInterPolationHeight_origin * ((double)m_Slider.GetPos()/100));
		SetDlgItemInt(IDC_EDIT_INTERPOLATION_WIDTH,m_nInterPolationWidth);
		SetDlgItemInt(IDC_INTERPOLATION_HEIGHT,m_nInterPolationHeight);
	}
		
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
