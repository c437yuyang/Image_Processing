// DlgShowImg.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgShowImg.h"
#include "afxdialogex.h"
#include <iostream>
using namespace std;


// CDlgShowImg dialog

IMPLEMENT_DYNAMIC(CDlgShowImg, CDialogEx)

CDlgShowImg::CDlgShowImg(CString WndName,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgShowImg::IDD, pParent)
{
	CImage_ProcessingView *m_pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	m_pView->m_ImageToDlgShow.CopyTo(m_ImageToShow);
	m_strWndName = WndName;

}

//�Լ��ֶ��رմ���,�Ȳ��ᴥ��PostNcDestroy��Ҳ���������������
CDlgShowImg::~CDlgShowImg()
{
	m_ImageToShow.Destroy();
	//cout << "����" << endl;
}

void CDlgShowImg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgShowImg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgShowImg message handlers


void CDlgShowImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	m_ImageToShow.Draw(::GetDC(this->m_hWnd),0,0);
	// Do not call CDialogEx::OnPaint() for painting messages
}


int CDlgShowImg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rcWindow,rcClient;
	int borderWidth,borderHeight;
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);
	borderWidth = (rcWindow.right-rcWindow.left) - (rcClient.right-rcClient.left);
	borderHeight = (rcWindow.bottom-rcWindow.top) - (rcClient.bottom-rcClient.top);
	this->SetWindowPos(NULL,0,0,m_ImageToShow.GetWidth()+borderWidth,m_ImageToShow.GetHeight()+borderHeight,SWP_NOMOVE);
	return 0;
}

//�Լ��ֶ��رմ���,�Ȳ��ᴥ��PostNcDestroy��Ҳ���������������
void CDlgShowImg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CDlgShowImg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->SetWindowTextW(m_strWndName);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//
void CDlgShowImg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//cout << "onclose" << endl;
	this->m_ImageToShow.Destroy();
	/*delete this;*/ //�ᱨ��

	CDialogEx::OnClose();
}
