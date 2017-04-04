#pragma once


// CDlgFilterMedium dialog

class CDlgFilterMedium : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilterMedium)

public:
	CDlgFilterMedium(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFilterMedium();

// Dialog Data
	enum { IDD = IDD_DLG_FILTER_MEDIUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	UINT m_nFtMidPara;
	int m_FtMidFillMode;
	int m_nFTSortMode;
	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnBnClickedOk();
//	CButton m_ftMidFillMode;
	
	
	int m_nFTAlphaAVG_D;
};
