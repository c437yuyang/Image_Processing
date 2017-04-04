// DlgFilterHPFSet.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgFilterHPFSet.h"
#include "afxdialogex.h"


// CDlgFilterHPFSet dialog

IMPLEMENT_DYNAMIC(CDlgFilterHPFSet, CDialogEx)

CDlgFilterHPFSet::CDlgFilterHPFSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFilterHPFSet::IDD, pParent)
	, m_dHPFRadius(50)
	, m_dHPFEnforcePara(0.5)
	, m_nHPFMode(0)
	, m_nBHPFOrder(2)
	, m_dOriImgPara(1.0)
{

}

CDlgFilterHPFSet::~CDlgFilterHPFSet()
{
}

void CDlgFilterHPFSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FT_HPF_RADIUS, m_dHPFRadius);
	DDX_Text(pDX, IDC_EDIT_FT_HPF_ENFORCEPARA, m_dHPFEnforcePara);
	DDX_Radio(pDX, IDC_RADIO_ILPF, m_nHPFMode);
	DDX_Text(pDX, IDC_EDIT_FT_BHPF_ORDER, m_nBHPFOrder);
	DDV_MinMaxUInt(pDX, m_nBHPFOrder, 0, 20);
	DDX_Text(pDX, IDC_EDIT_FT_HPF_ORIIMGPARA, m_dOriImgPara);
}


BEGIN_MESSAGE_MAP(CDlgFilterHPFSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgFilterHPFSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgFilterHPFSet message handlers


void CDlgFilterHPFSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDialogEx::OnOK();
}
