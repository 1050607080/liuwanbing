#pragma once
#include "Resource.h"
#include "TransparentStatic.h"

// CLeaveMessage �Ի���

class CLeaveMessage : public CGameFaceGo
{
	DECLARE_DYNAMIC(CLeaveMessage)

public:
	CLeaveMessage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLeaveMessage();

// �Ի�������
	enum { IDD = IDD_THING_MESSAGE };

	//������Ϣ
	void SetLeaveMessage(TCHAR * szMessage, TCHAR * szUserName);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CTransparentStatic m_NickName;       // �����뿪��ҵ��ǳ�
	CNormalBitmapButtonEx m_btnOK;          // ͬ�ⰴť
	CNormalBitmapButtonEx m_btnCancel;      // ��ͬ��
	CGameImage    m_imgBackground;          // ����ͼƬ
public:
	virtual BOOL OnInitDialog();

	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
};
