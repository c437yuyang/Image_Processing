#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgThresh dialog

class CDlgThresh : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThresh)

public:
	CDlgThresh(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgThresh();

// Dialog Data
	enum { IDD = IDD_DLG_THRESH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_SliderThresh;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMReleasedcaptureSliderThresh(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_comboThreshMode;
	int m_nThresh;
	virtual void PostNcDestroy();
	afx_msg void OnCbnSelchangeComboThreshmode();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
