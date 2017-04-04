// PropHistG.cpp : implementation file
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "PropHistG.h"
#include "afxdialogex.h"
#include "Image_ProcessingView.h"
#include "MainFrm.h"

// CPropHistG dialog

IMPLEMENT_DYNAMIC(CPropHistG, CPropertyPage)

CPropHistG::CPropHistG()
	: CPropertyPage(CPropHistG::IDD)
{
	CImage_ProcessingView *m_pView= (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	m_dMapHistG = m_pView->m_dMapHistG;
}

CPropHistG::~CPropHistG()
{
}

void CPropHistG::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropHistG, CPropertyPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPropHistG message handlers


void CPropHistG::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

	UINT nClientWidth = 590; //�Լ��������
	UINT nClientHeight = 251;
	CPoint ptOrigin;
	ptOrigin.x = 20;
	ptOrigin.y = nClientHeight - 20;
	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(0,0,rect.right - rect.left,rect.bottom - rect.top);//�����ο�
	CPen *pPenBlack=new CPen();//�������ʶ���
	pPenBlack->CreatePen(PS_SOLID,2,RGB(0,0,0));//��ɫ����
	CPen *pPenBlue=new CPen();
	pPenBlue->CreatePen(PS_SOLID,2,RGB(0,0,255));//��ɫ����
	CGdiObject *pOldPen=dc.SelectObject(pPenBlack);//ѡ���ɫ���ʣ���������ǰ����
	/*
	setwindowpos����ΪnWidth��nHeight�Ļ�
	���ϱ߿��С��560,260
	����߿�Ŀͻ�����С��nClientWidth,nClientHeight
	*/
	dc.MoveTo(20,nClientHeight-20);
	dc.LineTo(nClientWidth-5,nClientHeight-20); //����ϵ���ÿ��Ϊ(20��(584-5-20-10)=549),��529,��Ҫ256*2=512.

	dc.MoveTo(20,nClientHeight-20);
	dc.LineTo(20,5);//����ϵ���ø߶�Ϊ(5��(242-20-5-10)=207)����202����Ҫ200

	dc.MoveTo(nClientWidth-5,nClientHeight-20); //x�������ͷ
	dc.LineTo(nClientWidth-15,nClientHeight-30);
	dc.MoveTo(nClientWidth-5,nClientHeight-20);
	dc.LineTo(nClientWidth-15,nClientHeight-10);

	dc.MoveTo(20,5); //y�������ͷ
	dc.LineTo(20-10,15);
	dc.MoveTo(20,5);
	dc.LineTo(20+10,15);


	for(int i = 0;i!=13;++i) //����X������̶�
	{
		dc.MoveTo(ptOrigin.x+i*40,ptOrigin.y);
		dc.LineTo(ptOrigin.x+i*40,ptOrigin.y-2);
	}

	for(int i = 0;i!=10;++i) //����Y������̶�
	{
		dc.MoveTo(ptOrigin.x,ptOrigin.y-20*i);
		dc.LineTo(ptOrigin.x+2,ptOrigin.y-20*i);
	}

	//����x����̶�ֵ
	CString str;UINT j;
	for (UINT i=0;i!=7;++i)
	{
		j = 40*i;
		str.Format(_T("%d"),j);
		dc.TextOutW(ptOrigin.x-5 + 40*2*i,ptOrigin.y + 2 ,str);
	}

	//�������Ȳ�����


	dc.SelectObject(pPenBlue);
	//��ʼ����ֱ��ͼ������

	for(auto it=m_dMapHistG.begin();it!=m_dMapHistG.end();++it)
	{
		dc.MoveTo(ptOrigin.x+(int)(2*(it->first))+2,ptOrigin.y-2);
		dc.LineTo(ptOrigin.x+(int)(2*(it->first))+2,ptOrigin.y-(int)(200*(it->second))-2);
	}

	// Do not call CPropertyPage::OnPaint() for painting messages
}


void CPropHistG::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	//delete this; 
	CPropertyPage::PostNcDestroy();
}
