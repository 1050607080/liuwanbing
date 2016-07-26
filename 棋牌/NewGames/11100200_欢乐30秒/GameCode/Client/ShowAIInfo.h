#pragma once
#include "Resource.h"

// CShowAIInfo �Ի���

class CShowAIInfo : public CDialog
{
	DECLARE_DYNAMIC(CShowAIInfo)

public:
	CShowAIInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowAIInfo();

// �Ի�������
	enum { IDD = IDD_SHOW_AI_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//��ʼ������
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

public:
	
	tagAICtrlInfo	m_AIInfo;	//��������Ϣ
	

	bool	bIsClearAIWinMoney;		//��ӮǮ����
	bool	bIsSetAICtrl;			//�޸�����
	bool    bAIWinAndLostAutoCtrl;	//��������

	//��ʾ�Ի���
	void ShowAIInfoWindow();

public:
	afx_msg void OnBnClickedButSendChange();
public:
	afx_msg void OnBnClickedChChange();
public:
	afx_msg void OnBnClickedChClearWin();
public:
	afx_msg void OnBnClickedChChangeSet();
public:
	int m_GoalMoney;
public:
	int m_iAveMoney;
public:
	afx_msg void OnEnChangeEditGoalMoney();
public:
	afx_msg void OnEnChangeEditAveMoney();
};
