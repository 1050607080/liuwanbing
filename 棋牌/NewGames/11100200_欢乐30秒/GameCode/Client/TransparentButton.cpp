// TransparentButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TransparentButton.h"


// CTransparentButton

IMPLEMENT_DYNAMIC(CTransparentButton, CButton)

CTransparentButton::CTransparentButton()
{

}

CTransparentButton::~CTransparentButton()
{
}


BEGIN_MESSAGE_MAP(CTransparentButton, CButton)
	ON_WM_CTLCOLOR_REFLECT()
	ON_MESSAGE(WM_SETTEXT,OnSetText)
END_MESSAGE_MAP()



// CTransparentButton ��Ϣ�������



HBRUSH CTransparentButton::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  �����Ӧ���ø����Ĵ�������򷵻طǿջ���
	if (NULL != pDC)
	{
		pDC->SetBkMode(TRANSPARENT);
	}
	return (HBRUSH)GetStockObject(NULL_BRUSH);
	//return NULL;
}

LRESULT CTransparentButton::OnSetText(WPARAM wParam,LPARAM lParam)
{
	LRESULT lResult = Default();
	CWnd *pWnd = GetParent();
	if (NULL != pWnd)
	{
		CRect Rect;
		GetWindowRect(&Rect);
		pWnd->ScreenToClient(&Rect);
		pWnd->InvalidateRect(&Rect);
	}
	return lResult;
}