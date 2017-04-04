// NoiseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "NoiseDlg.h"
#include "afxdialogex.h"
#include "Image_ProcessingView.h"

// CNoiseDlg dialog

IMPLEMENT_DYNAMIC(CNoiseDlg, CDialog)

CNoiseDlg::CNoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoiseDlg::IDD, pParent)
	, m_dNoisePara1(0.0)
	, m_dNoisePara2(20)
{

	m_NoiseCount = 0.01;
	m_nNoiseType = 0;
}

CNoiseDlg::~CNoiseDlg()
{
}

void CNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NOISE_PARA, m_NoiseCount);
	DDV_MinMaxDouble(pDX, m_NoiseCount, 0, 0.99);
	DDX_Radio(pDX, IDC_RADIO1, m_nNoiseType);
	DDX_Text(pDX, IDC_EDIT_NOISE_PARA1, m_dNoisePara1);
	DDX_Text(pDX, IDC_EDIT_NOISE_PARA2, m_dNoisePara2);
}


BEGIN_MESSAGE_MAP(CNoiseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNoiseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO3, &CNoiseDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO8, &CNoiseDlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CNoiseDlg::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &CNoiseDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &CNoiseDlg::OnBnClickedRadio11)
END_MESSAGE_MAP()


// CNoiseDlg message handlers


void CNoiseDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	//CImage_ProcessingView *pView = (CImage_ProcessingView*)GetDlgItem(IDD_DIALOG1);
	//CImage_ProcessingView *pView = new CImage_ProcessingView();
	
	//pView->m_dNoiseCount = m_NoiseCount;
	//CString str;
	//str.Format(_T("%f"),m_NoiseCount);
	//MessageBox(str);
	//delete pView;
	//pView = NULL;
	
	UpdateData();

	if (m_NoiseCount == 0)
	{
		MessageBox(_T("请输入一个介于0到1之间的小数！"));
		return ;
	}
	CDialog::OnOK();
}


void CNoiseDlg::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	SetDlgItemInt(IDC_EDIT_NOISE_PARA1,0);
	SetDlgItemInt(IDC_EDIT_NOISE_PARA2,20);
}


void CNoiseDlg::OnBnClickedRadio8()
{
	// TODO: Add your control notification handler code here
	SetDlgItemInt(IDC_EDIT_NOISE_PARA1,-10);
	SetDlgItemInt(IDC_EDIT_NOISE_PARA2,300);
}


void CNoiseDlg::OnBnClickedRadio9()
{
	// TODO: Add your control notification handler code here
	SetDlgItemInt(IDC_EDIT_NOISE_PARA1,20);
	SetDlgItemInt(IDC_EDIT_NOISE_PARA2,-1);
}


void CNoiseDlg::OnBnClickedRadio10()
{
	// TODO: Add your control notification handler code here
	SetDlgItemInt(IDC_EDIT_NOISE_PARA1,20);
	SetDlgItemInt(IDC_EDIT_NOISE_PARA2,0);
}


void CNoiseDlg::OnBnClickedRadio11()
{
	// TODO: Add your control notification handler code here
	SetDlgItemInt(IDC_EDIT_NOISE_PARA1,10);
	SetDlgItemInt(IDC_EDIT_NOISE_PARA2,1);
}
