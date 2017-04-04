#pragma once
#include "afxcmn.h"
#include <iostream>
// CDlgInterPolationSet dialog

class CDlgInterPolationSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInterPolationSet)

public:
	CDlgInterPolationSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInterPolationSet();

// Dialog Data
	enum { IDD = IDD_DLG_INTERPOLATION_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nInterPolationHeight;
	UINT m_nInterPolationWidth;
	UINT m_nInterPolationHeight_origin;
	UINT m_nInterPolationWidth_origin;
	int m_nInterpolationType;
	afx_msg void OnBnClickedOk();
	CSliderCtrl m_Slider;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
