#pragma once


// CNoiseDlg dialog

class CNoiseDlg : public CDialog
{
	DECLARE_DYNAMIC(CNoiseDlg)

public:
	CNoiseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNoiseDlg();

// Dialog Data
	enum { IDD = IDD_DLG_NOISE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()



public:
	double m_NoiseCount;
	afx_msg void OnBnClickedOk();
	int m_nNoiseType;
	double m_dNoisePara1;
	double m_dNoisePara2;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
};
