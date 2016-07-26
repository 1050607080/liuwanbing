#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "afxwin.h"

// CHaveThing �Ի���

class CHaveThing : public CGameFaceGo
{
	DECLARE_DYNAMIC(CHaveThing)

public:
	CHaveThing(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHaveThing();

// �Ի�������
	enum { IDD = IDD_HAVE_THING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CNormalBitmapButtonEx m_btnOK;          // ȷ��
	CNormalBitmapButtonEx m_btnCancel;      // ȡ��

	CGameImage    m_imgBackground;          // ����ͼƬ
	bool	      m_bIsClick;               // 
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnSetfocusEdit();
	afx_msg void OnPaint();
public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
};
