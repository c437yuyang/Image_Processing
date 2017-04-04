#pragma once
#include "PropHistR.h"
#include "PropHistG.h"
#include "PropHistB.h"

// CPropSheetRGB

class CPropSheetRGB : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheetRGB)

public:
	CPropSheetRGB(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheetRGB(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropSheetRGB();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CPropHistR m_propHistR;
	CPropHistG m_propHistG;
	CPropHistB m_propHistB;
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
};


