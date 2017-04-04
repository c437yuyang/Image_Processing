#pragma once


// CDlgFilterHPFSet dialog

class CDlgFilterHPFSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilterHPFSet)

public:
	CDlgFilterHPFSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFilterHPFSet();

// Dialog Data
	enum { IDD = IDD_DLG_FILTER_HFP_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dHPFRadius;
	double m_dHPFEnforcePara;
	int m_nHPFMode;
	afx_msg void OnBnClickedOk();
	UINT m_nBHPFOrder;
	double m_dOriImgPara;
};
