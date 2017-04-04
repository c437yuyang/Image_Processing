#pragma once


// CDlgFilterAVG dialog

class CDlgFilterAVG : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilterAVG)

public:
	CDlgFilterAVG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFilterAVG();

// Dialog Data
	enum { IDD = IDD_DLG_FILTER_AVG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//1Îª0Ìî³ä£¬0Îª255Ìî³ä
	int m_FtAvgFillMode;
	UINT m_nFtAvgPara;
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnBnClickedRadio3();
	int m_nFTAVGMode;
	double m_dFTAVGNiXieQ;
};
