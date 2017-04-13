// DlgChooseParam.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgChooseParam.h"
#include "afxdialogex.h"


// CDlgChooseParam 对话框

IMPLEMENT_DYNAMIC(CDlgChooseParam, CDialogEx)

CDlgChooseParam::CDlgChooseParam(CWnd* pParent /*=NULL*/, CString strDlgName, CString strStaticText,double dDefaultParam=0.0)
	: CDialogEx(IDD_DLG_CHOOSE_PARAM, pParent)
	, m_dParam1(4)
{
	m_dParam1 = dDefaultParam;
	m_strDlgName = strDlgName;
	m_strStaticText = strStaticText;
}

CDlgChooseParam::~CDlgChooseParam()
{
}

void CDlgChooseParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PARAM1, m_dParam1);
}


BEGIN_MESSAGE_MAP(CDlgChooseParam, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgChooseParam::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChooseParam 消息处理程序


void CDlgChooseParam::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CDialogEx::OnOK();
}


BOOL CDlgChooseParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetWindowTextW(m_strStaticText);
	GetDlgItem(IDC_STATIC)->SetWindowTextW(m_strStaticText);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
