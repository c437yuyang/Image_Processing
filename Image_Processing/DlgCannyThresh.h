#pragma once
#include "afxcmn.h"


// CDlgCannyThresh dialog

class CDlgCannyThresh : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCannyThresh)

public:
	CDlgCannyThresh(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCannyThresh();

// Dialog Data
	enum { IDD = IDD_DLG_CANNY_THRESH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_SliderCanny;
	int m_nLowThresh;
	int m_nHighThresh;

	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlidercanny(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMReleasedcaptureSlidercanny(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SliderBinaryThresh;
	afx_msg void OnNMReleasedcaptureSliderBinary(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PostNcDestroy();
};
