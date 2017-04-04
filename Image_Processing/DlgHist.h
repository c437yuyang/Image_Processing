#pragma once


// CDlgHist dialog

#include "Image_ProcessingView.h"
#include "MainFrm.h"

class CDlgHist : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHist)

public:
	CDlgHist(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHist();

// Dialog Data
	enum { IDD = IDD_DLG_HIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PostNcDestroy();
};
