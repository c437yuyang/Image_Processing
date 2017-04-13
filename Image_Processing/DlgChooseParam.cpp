// DlgChooseParam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgChooseParam.h"
#include "afxdialogex.h"


// CDlgChooseParam �Ի���

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


// CDlgChooseParam ��Ϣ�������


void CDlgChooseParam::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CDialogEx::OnOK();
}


BOOL CDlgChooseParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->SetWindowTextW(m_strStaticText);
	GetDlgItem(IDC_STATIC)->SetWindowTextW(m_strStaticText);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
