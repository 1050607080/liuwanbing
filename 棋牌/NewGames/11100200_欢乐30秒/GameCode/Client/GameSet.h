#pragma once

#include "Stdafx.h"
#include "Resource.h"
///#include "Control\\MacButtons.h"

///��Ϸ���öԻ����� 
class CGameSet : public CCoolView//CGameFaceGo
{

	//��������
public:
//����Ч��
	BOOL					m_bSound;						
///����Ч��
	BOOL					m_bBackMusic;						
///�Ƿ���ʾ�û� 
	BOOL					m_bShowUser;					
///�Ƿ������Թ�
	BOOL					m_bEnableWatch;					
///������������ �Ƿ�����������������Ч // add by ramon 09.05.11
	bool					m_bEnableTalkSound;				
	BOOL					m_bRightPower;
	CNormalBitmapButtonEx				m_Cancel;
	CNormalBitmapButtonEx				m_Ok;
	enum					{ IDD = IDD_GAME_SET1 };

	CButton							m_check_1;
	CButton							m_check_2;
	CButton							m_check_3;
	CButton							m_check_4;
	//CButton							m_check_5;
	CButton							m_check_6;


	CWnd				*m_pParent;
	CGameImage				m_bk;			//����ͼ
	///�������� 
public:
	///���캯��
	CGameSet(CWnd * pParent=NULL);
	///��������
	virtual ~CGameSet();
	//��ʼ���Ի���
	virtual BOOL OnInitDialog();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	///DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	///ȷ����ť
	virtual void OnOK();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	void AutoSize();
};
