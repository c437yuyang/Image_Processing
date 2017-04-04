// DlgFilterAVG.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgFilterAVG.h"
#include "afxdialogex.h"


// CDlgFilterAVG dialog

IMPLEMENT_DYNAMIC(CDlgFilterAVG, CDialogEx)

CDlgFilterAVG::CDlgFilterAVG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFilterAVG::IDD, pParent)
{

	m_nFtAvgPara = 1;
	//1为0填充，0为255填充,2为复制边界填充
	m_FtAvgFillMode = 2;
	m_nFTAVGMode = 0;
	m_dFTAVGNiXieQ = 0;
}

CDlgFilterAVG::~CDlgFilterAVG()
{
}

void CDlgFilterAVG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FT_AVGPARA, m_nFtAvgPara);//Dialog Data Exchange
	DDV_MinMaxUInt(pDX, m_nFtAvgPara, 1, 10);//Dialog Data Validate
	DDX_Radio(pDX, IDC_RADIO1, m_FtAvgFillMode);
	DDV_MinMaxInt(pDX, m_FtAvgFillMode, 0, 2);
	DDX_Radio(pDX, IDC_RADIO4, m_nFTAVGMode);
	DDX_Text(pDX, IDC_FT_NIXIE_AVGPARA, m_dFTAVGNiXieQ);
}


BEGIN_MESSAGE_MAP(CDlgFilterAVG, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgFilterAVG::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgFilterAVG::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CDlgFilterAVG message handlers



void CDlgFilterAVG::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	UpdateData();

	CDialogEx::OnOK();
}


void CDlgFilterAVG::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
}
