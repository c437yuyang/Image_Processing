#pragma once

#include <map>
// CPropHistG dialog

class CPropHistG : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropHistG)

public:
	CPropHistG();
	virtual ~CPropHistG();

// Dialog Data
	enum { IDD = IDD_HIST_PROP_G };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	std::map<int,double> m_dMapHistG;
	virtual void PostNcDestroy();
};
