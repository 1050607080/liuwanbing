#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpgradeMessage.h"

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա

//��ʱ�� ID100����

#define		TIME_BET			102			//��ע��ʱ��
#define		TIME_APPLY			103			//������ׯ��ʱ��
#define TIME_LEAVE			105			//������ׯ


//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{

	//�ؼ�����
public:

	int						m_iVersion;
	int						m_iVersion2;
private://�������п����õı���
	BYTE                m_iRobotMaxNtLimit;									//�����������ׯ����
	__int64             m_iRobotBetSumLimit;								//��������ע����
	int					m_iBetChouMaPro[6];				//��ע����ĸ���
public: 
	BYTE                m_byGameStation ;									///��ǰ����Ϸ״̬
	int					m_iXiaZhuTime;										///��עʱ��				
	int					m_iKaiPaiTime;										///����ʱ��       
	int					m_iFreeTime;										///����ʱ��	
	__int64				m_i64ChoumaValue[6];					///��������ֵDWJ
	__int64				m_i64MaxNote;					/// ÿ����������ע��DWJ
	__int64             m_i64ApplyNtLimite ;								//ׯ����ׯ����
	__int64				m_iMyBetSum;				//�Լ�����ע����
	BYTE				m_byWaitNtList[PLAY_COUNT+1];						//��ׯ�б�
	BYTE				m_byNtStation;										//ׯ��λ��
	
	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();
 

protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��ʼ��Ϸ
	void		OnHandleBet(void * buffer,int nLen);
	void		OnHandleFree(void * buffer,int nLen);			//���յ��ȴ���Ϣ
	//�������ý���
	virtual void ResetGameFrame();
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	virtual bool SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUseMillSecond = false) ; 
	//������ж�ʱ��
	void KillAllTimer();
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};
	//�ܷ��뿪����
	virtual BOOL CanLeftDesk();
	//���Ż��㵽��ͼλ��
	virtual BYTE ViewStation(BYTE bDeskStation);
	//��Ϣ������
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//�˳�����
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT nIDEvent);

public:
	//��������ׯ
	void	RobortApplyNt();
	//��������ע
	void	RobortBet();

	//��ʼ����Ϸ����
	void InitGameData();

	//���������ļ�
	void LoadGameIni();

public:
	///��ʼ����������
	void	ResetGameData();

	//��������
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);

	//������Ϸ
	void ResetGameStation(int iGameStation);

public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);



public:
	//�ж��Ƿ�������ׯ
	void JudgeApplyNt(bool bFlag);


};
