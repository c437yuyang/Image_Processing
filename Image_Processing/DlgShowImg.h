#pragma once

#include "Image_ProcessingView.h"
#include "MyImage_.h"
// CDlgShowImg dialog

class CDlgShowImg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowImg)

public:
	CDlgShowImg(CString WndName,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShowImg();

// Dialog Data
	enum { IDD = IDD_DLG_SHOW_IMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	MyImage_ m_ImageToShow;
	CString m_strWndName;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
