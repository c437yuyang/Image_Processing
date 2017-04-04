#pragma once


// CDlgFilterLPFSet dialog

class CDlgFilterLPFSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilterLPFSet)

public:
	CDlgFilterLPFSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFilterLPFSet();

// Dialog Data
	enum { IDD = IDD_DLG_FILTER_LFP_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	double m_dLPFRadius;
	int m_nBLPFOrder;
	int m_nLPFMode;
	afx_msg void OnBnClickedOk();
};
