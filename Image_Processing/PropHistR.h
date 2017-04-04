#pragma once


// CPropHistR dialog
#include <map>


class CPropHistR : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropHistR)

public:
	CPropHistR();
	virtual ~CPropHistR();

// Dialog Data
	enum { IDD = IDD_HIST_PROP_R };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	std::map<int,double> m_dMapHistR;
	virtual void PostNcDestroy();
};
