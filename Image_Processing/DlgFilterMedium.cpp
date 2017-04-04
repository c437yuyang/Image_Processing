// DlgFilterMedium.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgFilterMedium.h"
#include "afxdialogex.h"


// CDlgFilterMedium dialog

IMPLEMENT_DYNAMIC(CDlgFilterMedium, CDialogEx)

CDlgFilterMedium::CDlgFilterMedium(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFilterMedium::IDD, pParent)
	, m_nFtMidPara(3)
	, m_nFTSortMode(0) //统计排序滤波模式
	, m_nFTAlphaAVG_D(4)
{
	//为1是填充0，为0是填充255,2是复制边界填充
	m_FtMidFillMode = 2;//填充模式
}

CDlgFilterMedium::~CDlgFilterMedium()
{

}

void CDlgFilterMedium::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FT_MIDPARA, m_nFtMidPara);
	DDV_MinMaxUInt(pDX, m_nFtMidPara, 1, 10);
	//  DDX_Control(pDX, IDC_RADIO1, m_ftMidFillMode);
	DDX_Radio(pDX, IDC_RADIO1, m_FtMidFillMode);
	DDV_MinMaxInt(pDX, m_FtMidFillMode, 0, 2);
	DDX_Radio(pDX, IDC_FT_SORT_MAX, m_nFTSortMode);
	DDX_Text(pDX, IDC_EDIT_FT_ALPHA_AVG_D, m_nFTAlphaAVG_D);
}


BEGIN_MESSAGE_MAP(CDlgFilterMedium, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgFilterMedium::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgFilterMedium message handlers


void CDlgFilterMedium::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDialogEx::OnOK();
}
