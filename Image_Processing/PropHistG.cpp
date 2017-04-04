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

	UINT nClientWidth = 590; //自己测出来的
	UINT nClientHeight = 251;
	CPoint ptOrigin;
	ptOrigin.x = 20;
	ptOrigin.y = nClientHeight - 20;
	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(0,0,rect.right - rect.left,rect.bottom - rect.top);//画矩形框
	CPen *pPenBlack=new CPen();//创建画笔对象
	pPenBlack->CreatePen(PS_SOLID,2,RGB(0,0,0));//红色画笔
	CPen *pPenBlue=new CPen();
	pPenBlue->CreatePen(PS_SOLID,2,RGB(0,0,255));//蓝色画笔
	CGdiObject *pOldPen=dc.SelectObject(pPenBlack);//选择红色画笔，并保存以前画笔
	/*
	setwindowpos设置为nWidth，nHeight的话
	加上边框大小是560,260
	不算边框的客户区大小是nClientWidth,nClientHeight
	*/
	dc.MoveTo(20,nClientHeight-20);
	dc.LineTo(nClientWidth-5,nClientHeight-20); //坐标系可用宽度为(20到(584-5-20-10)=549),共529,需要256*2=512.

	dc.MoveTo(20,nClientHeight-20);
	dc.LineTo(20,5);//坐标系可用高度为(5到(242-20-5-10)=207)，共202，需要200

	dc.MoveTo(nClientWidth-5,nClientHeight-20); //x坐标轴箭头
	dc.LineTo(nClientWidth-15,nClientHeight-30);
	dc.MoveTo(nClientWidth-5,nClientHeight-20);
	dc.LineTo(nClientWidth-15,nClientHeight-10);

	dc.MoveTo(20,5); //y坐标轴箭头
	dc.LineTo(20-10,15);
	dc.MoveTo(20,5);
	dc.LineTo(20+10,15);


	for(int i = 0;i!=13;++i) //画出X坐标轴刻度
	{
		dc.MoveTo(ptOrigin.x+i*40,ptOrigin.y);
		dc.LineTo(ptOrigin.x+i*40,ptOrigin.y-2);
	}

	for(int i = 0;i!=10;++i) //画出Y坐标轴刻度
	{
		dc.MoveTo(ptOrigin.x,ptOrigin.y-20*i);
		dc.LineTo(ptOrigin.x+2,ptOrigin.y-20*i);
	}

	//画出x坐标刻度值
	CString str;UINT j;
	for (UINT i=0;i!=7;++i)
	{
		j = 40*i;
		str.Format(_T("%d"),j);
		dc.TextOutW(ptOrigin.x-5 + 40*2*i,ptOrigin.y + 2 ,str);
	}

	//纵坐标先不绘制


	dc.SelectObject(pPenBlue);
	//开始绘制直方图各分量

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
