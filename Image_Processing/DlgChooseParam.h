#pragma once


// CDlgChooseParam 对话框

class CDlgChooseParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChooseParam)

public:
	CDlgChooseParam(CWnd* pParent,CString strDlgName,CString strStaticText, double dDefaultParam);   // 标准构造函数
	virtual ~CDlgChooseParam();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CHOOSE_PARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	double m_dParam1;
	CString m_strDlgName;
	CString m_strStaticText;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
