// DlgFilterLPFSet.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgFilterLPFSet.h"
#include "afxdialogex.h"


// CDlgFilterLPFSet dialog

IMPLEMENT_DYNAMIC(CDlgFilterLPFSet, CDialogEx)

CDlgFilterLPFSet::CDlgFilterLPFSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFilterLPFSet::IDD, pParent)
	, m_dLPFRadius(100)
	, m_nBLPFOrder(2)
	, m_nLPFMode(0)
{

}

CDlgFilterLPFSet::~CDlgFilterLPFSet()
{
}

void CDlgFilterLPFSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FT_HPF_RADIUS, m_dLPFRadius);
	DDX_Text(pDX, IDC_EDIT_FT_BHPF_ORDER, m_nBLPFOrder);
	DDX_Radio(pDX, IDC_RATIO_ILPF, m_nLPFMode);
}


BEGIN_MESSAGE_MAP(CDlgFilterLPFSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgFilterLPFSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgFilterLPFSet message handlers


void CDlgFilterLPFSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDialogEx::OnOK();
}
