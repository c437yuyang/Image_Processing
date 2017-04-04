#pragma once

#include <map>

// CPropHistB dialog

class CPropHistB : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropHistB)

public:
	CPropHistB();
	virtual ~CPropHistB();

	// Dialog Data
	enum { IDD = IDD_HIST_PROP_B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	std::map<int,double> m_dMapHistB;
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PostNcDestroy();
};
