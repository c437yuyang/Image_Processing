// DlgFilterTplSet.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgFilterTplSet.h"
#include "afxdialogex.h"
#include "Image_ProcessingView.h"
#include "MainFrm.h"

#include <iostream>
using namespace std;


int TextSegment(const CString &strInput,const CString &strSeg,vector<CString> &vecOutput);
// CDlgFilterTplSet dialog

IMPLEMENT_DYNAMIC(CDlgFilterTplSet, CDialogEx)

CDlgFilterTplSet::CDlgFilterTplSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFilterTplSet::IDD, pParent)
	, m_bIsDoGray(true)
	, m_nFilterType(0)
	, m_bIsOverLap(FALSE)
{

}

CDlgFilterTplSet::~CDlgFilterTplSet()
{
}

void CDlgFilterTplSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_TOGRAY, m_bIsDoGray);
	DDX_Radio(pDX, IDC_RADIO1, m_nFilterType);
	DDX_Check(pDX, IDC_CHECK_OVERLAY, m_bIsOverLap);
}


BEGIN_MESSAGE_MAP(CDlgFilterTplSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DOFILTER, &CDlgFilterTplSet::OnBnClickedBtnDofilter)
	ON_BN_CLICKED(IDOK, &CDlgFilterTplSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgFilterTplSet::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgFilterTplSet::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgFilterTplSet::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgFilterTplSet::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO8, &CDlgFilterTplSet::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CDlgFilterTplSet::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &CDlgFilterTplSet::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &CDlgFilterTplSet::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO12, &CDlgFilterTplSet::OnBnClickedRadio12)
	ON_BN_CLICKED(IDC_RADIO13, &CDlgFilterTplSet::OnBnClickedRadio13)
	ON_BN_CLICKED(IDC_CHECK_OVERLAY, &CDlgFilterTplSet::OnBnClickedCheckOverlay)
	ON_BN_CLICKED(IDC_RADIO14, &CDlgFilterTplSet::OnBnClickedRadio14)
	ON_BN_CLICKED(IDC_RADIO15, &CDlgFilterTplSet::OnBnClickedRadio15)
	ON_BN_CLICKED(IDC_RADIO16, &CDlgFilterTplSet::OnBnClickedRadio16)
	ON_BN_CLICKED(IDC_RADIO17, &CDlgFilterTplSet::OnBnClickedRadio17)
	ON_BN_CLICKED(IDC_RADIO18, &CDlgFilterTplSet::OnBnClickedRadio18)
	ON_BN_CLICKED(IDC_RADIO19, &CDlgFilterTplSet::OnBnClickedRadio19)
	ON_BN_CLICKED(IDC_RADIO20, &CDlgFilterTplSet::OnBnClickedRadio20)
	ON_BN_CLICKED(IDC_RADIO21, &CDlgFilterTplSet::OnBnClickedRadio21)
END_MESSAGE_MAP()


// CDlgFilterTplSet message handlers


void CDlgFilterTplSet::OnBnClickedBtnDofilter()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString str;
	GetDlgItemText(IDC_EDIT_TPL,str);
	vector<CString> strVecTpl;
	int nTplSize = m_com.TextSegment(str,_T(","),strVecTpl);
	vector<double> dVecTpl;
	for (auto it=strVecTpl.begin();it!=strVecTpl.end();++it)
	{
		//printf("%ls ",*it);
		dVecTpl.push_back(_ttof(*it));
	}
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	if (m_bIsOverLap)
	{
		if (m_bIsDoGray)
		{
			pView->doImFilter(imgBackup,dVecTpl,imgAfter,2,true,true);
			imgAfter.CopyTo(imgBackup);
			imgAfter.CopyTo(pView->m_ImageAfter);
		}
			
		else
		{
			pView->doImFilter(imgBackup,dVecTpl,imgAfter,2,false,true);
			imgAfter.CopyTo(imgBackup);
			imgAfter.CopyTo(pView->m_ImageAfter);
		}
	}
	else
	{
		if (m_bIsDoGray)
			pView->doImFilter(pView->m_ImageTpl,dVecTpl,pView->m_ImageAfter,2,true,true);
		else
			pView->doImFilter(pView->m_ImageTpl,dVecTpl,pView->m_ImageAfter,2,false,true);
	}

	pView->UpdateState(false);

}





BOOL CDlgFilterTplSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_ImageTpl.CopyTo(imgAfter);
	pView->m_ImageTpl.CopyTo(imgBackup);

	strVecTpls.push_back(_T("-1,-2,-1,0,0,0,1,2,1"));//sobel 水平
	strVecTpls.push_back(_T("-1,0,1,-2,0,2,-1,0,1"));//sobel 垂直
	strVecTpls.push_back(_T("-1,-1,-1,-1,9,-1,-1,-1,-1")); //laplace 各角度
	strVecTpls.push_back(_T("0,1,2,-1,0,1,-2,-1,0"));//sobel45
	strVecTpls.push_back(_T("-2,-1,0,-1,0,1,0,1,2"));//sobel135
	strVecTpls.push_back(_T("0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111"));//算数均值
	strVecTpls.push_back(_T("0,-1,0,-1,5,-1,0,-1,0")); //锐化
	strVecTpls.push_back(_T("0.0830517,0.121034,0.0830517,0.121034,0.183657,0.121034,0.0830517,0.121034,0.0830517"));//高斯模糊
	strVecTpls.push_back(_T("-1,0,1,-2,0,2,-1,0,1")); //sobel 垂直  (重复,未用)
	strVecTpls.push_back(_T("-1,-1,-1,0,0,0,1,1,1")); //prewitt 水平
	strVecTpls.push_back(_T("-1,0,1,-1,0,1,-1,0,1")); //prewitt 垂直
	strVecTpls.push_back(_T("0,1,1,-1,0,1,-1,-1,0")); //prewitt 45
	strVecTpls.push_back(_T("-1,-1,0,-1,0,1,0,1,1")); //prewitt 135
	strVecTpls.push_back(_T("-1,-1,-1,2,2,2,-1,-1,-1")); //线检测水平
	strVecTpls.push_back(_T("2,-1,-1,-1,2,-1,-1,-1,2")); //线检测45
	strVecTpls.push_back(_T("-1,2,-1,-1,2,-1,-1,2,-1")); //线检测垂直
	strVecTpls.push_back(_T("-1,-1,2,-1,2,-1,2,-1,-1")); //线检测135 共17

	// TODO:  Add extra initialization here
	SetDlgItemText(IDC_EDIT_TPL,_T("-1,-2,-1,0,0,0,1,2,1"));
	GetParent()->EnableWindow(FALSE);
	this->EnableWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgFilterTplSet::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDlgFilterTplSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->UpdateState(true);
	CDialogEx::OnOK();
}


void CDlgFilterTplSet::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_ImageTpl.CopyTo(pView->m_ImageAfter);
	pView->UpdateState(false);
	CDialogEx::OnCancel();
}


void CDlgFilterTplSet::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[0]);
}



void CDlgFilterTplSet::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[1]);
}


void CDlgFilterTplSet::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[2]);
}


void CDlgFilterTplSet::OnBnClickedRadio8()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[3]);
}


void CDlgFilterTplSet::OnBnClickedRadio9()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[4]);
}


void CDlgFilterTplSet::OnBnClickedRadio10()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[5]);
}


void CDlgFilterTplSet::OnBnClickedRadio11()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[6]);
}


void CDlgFilterTplSet::OnBnClickedRadio12()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[7]);
}


void CDlgFilterTplSet::OnBnClickedRadio13()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[8]);
}


void CDlgFilterTplSet::OnBnClickedCheckOverlay()
{
	// TODO: Add your control notification handler code here
	CImage_ProcessingView *pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_ImageTpl.CopyTo(imgAfter);
	pView->m_ImageTpl.CopyTo(imgBackup);

}


void CDlgFilterTplSet::OnBnClickedRadio14()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[9]);
}




void CDlgFilterTplSet::OnBnClickedRadio15()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[10]);
}


void CDlgFilterTplSet::OnBnClickedRadio16()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[11]);
}


void CDlgFilterTplSet::OnBnClickedRadio17()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[12]);
}


void CDlgFilterTplSet::OnBnClickedRadio18()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[13]);
}


void CDlgFilterTplSet::OnBnClickedRadio19()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[14]);
}


void CDlgFilterTplSet::OnBnClickedRadio20()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[15]);
}


void CDlgFilterTplSet::OnBnClickedRadio21()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TPL,strVecTpls[16]);
}
