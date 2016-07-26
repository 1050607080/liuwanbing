#pragma once

#include "resource.h"
#include "afxcmn.h"
// CResult �Ի���

class CResult : public CDialog
{
	DECLARE_DYNAMIC(CResult)

public:
	CResult(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResult();

	CGameImage m_img;	//����ͼƬ

// �Ի�������
	enum { IDD = IDD_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	//��������
	void ClearAll();

public:
	int		m_iNtStation;//ׯ��λ��
	int		m_iMeStation;//�Լ���λ��
	__int64     m_iNtWinMoney;//ׯӮ��Ǯ
	__int64		m_iXianMoney;//��Ӯ��Ǯ
	__int64     m_iMeWinMoney;//�Լ�Ӯ��Ǯ
	int     m_iPaiDian[2];  //ׯ�е��Ƶ�
	int     m_iZPaiXing[2]; //ׯ�ҵ�����
	int     m_iXPaiXing[2]; //�мҵ�����


};
