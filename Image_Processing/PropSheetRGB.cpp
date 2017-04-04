// PropSheetRGB.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "PropSheetRGB.h"


// CPropSheetRGB

IMPLEMENT_DYNAMIC(CPropSheetRGB, CPropertySheet)

CPropSheetRGB::CPropSheetRGB(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_propHistR);
	AddPage(&m_propHistG);
	AddPage(&m_propHistB);
}

CPropSheetRGB::CPropSheetRGB(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	pszCaption = _T("RGBÖ±·½Í¼");
	AddPage(&m_propHistR);
	AddPage(&m_propHistG);
	AddPage(&m_propHistB);
}

CPropSheetRGB::~CPropSheetRGB()
{
}


BEGIN_MESSAGE_MAP(CPropSheetRGB, CPropertySheet)
END_MESSAGE_MAP()


// CPropSheetRGB message handlers


BOOL CPropSheetRGB::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);  
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);  
	GetDlgItem(ID_APPLY_NOW)->ShowWindow(SW_HIDE);  

	return bResult;
}


void CPropSheetRGB::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CPropertySheet::PostNcDestroy();
}
