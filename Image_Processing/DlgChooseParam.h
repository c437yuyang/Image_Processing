#pragma once


// CDlgChooseParam �Ի���

class CDlgChooseParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChooseParam)

public:
	CDlgChooseParam(CWnd* pParent,CString strDlgName,CString strStaticText, double dDefaultParam);   // ��׼���캯��
	virtual ~CDlgChooseParam();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CHOOSE_PARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	double m_dParam1;
	CString m_strDlgName;
	CString m_strStaticText;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
