#pragma once
#include "Common.h"
#include "MyImage_.h"
// CDlgFilterTplSet dialog

class CDlgFilterTplSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilterTplSet)

public:
	CDlgFilterTplSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFilterTplSet();

// Dialog Data
	enum { IDD = IDD_DLG_FILTER_TPL_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDofilter();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	CCommon m_com;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL m_bIsDoGray;
	int m_nFilterType;
	vector<CString> strVecTpls;
	MyImage_ imgAfter,imgBackup;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedRadio12();
	afx_msg void OnBnClickedRadio13();
	BOOL m_bIsOverLap;
	afx_msg void OnBnClickedCheckOverlay();
	afx_msg void OnBnClickedRadio14();
	afx_msg void OnBnClickedRadio15();
	afx_msg void OnBnClickedRadio16();
	afx_msg void OnBnClickedRadio17();
	afx_msg void OnBnClickedRadio18();
	afx_msg void OnBnClickedRadio19();
	afx_msg void OnBnClickedRadio20();
	afx_msg void OnBnClickedRadio21();
};
