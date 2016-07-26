#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpgradeMessage.h"

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա

//��ʱ�� ID
#define TIME_AGREE			101			//ͬ���ʱ��
#define TIME_BET			102			//��ע��ʱ��
#define	TIME_APPLY			103			//������ׯ��ʱ��
#define TIME_BALLOT			104			//��ǩ��ʱ��
#define TIME_LEAVE			105			//������ׯ


//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{

	//�ؼ�����
public:

	int						m_iVersion;
	int						m_iVersion2;
public: 

	BOOL				m_RobotCanBet;											//�������ܷ���ע
	BYTE                m_byGameStation ;										///��ǰ����Ϸ״̬
	int                 m_iMyDeskStation ;										///����Լ�����Ϸλ��

	BYTE				m_byFreeTime;											//����ʱ��
	BYTE				m_byBetTime;											//��עʱ��
	BYTE				m_byNoBetTime;											//�����벻����ע
	BYTE				m_byOpenTime;											//����ʱ��
	BYTE				m_byShowResultTime;										//������ʾʱ��

	BYTE				m_byRemaindTime;										//ʣ��ʱ��
	BYTE				m_iRobotMaxNtLimit;										//�����������ׯ��������
	BYTE				m_byWaitNtList[PLAY_COUNT];								//��ׯ�б�
	
	BYTE				m_byMaxNtPeople;										//�����ׯ����
	BYTE				m_byNtStation;											//ׯ��λ��
	__int64             m_i64ApplyNtLimite ;									//��������ׯ����
	__int64				m_iRobotBetSumLimit;									//��������ע����
	
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
	virtual bool OnControlSafeFinish(){OutputDebugString("Robot::OnControlSafeFinish");return TRUE;};
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

private:
	//��ʼ����Ϸ����
	void InitGameData();

	//���������ļ�
	void LoadGameIni();

	//������������ׯ
	void	RobortApplyNt();
	//��������ׯ
	void	RobortApplyLeave();
	//��������ע
	void	RobortBet();
private:
	//��ȡָ����ҵ�ʣ����
	__int64	GetUserRemaindMoney(BYTE byDestStation);
	//��ȡ���ʣ��Ľ��
	__int64	GetMyRemaindMoney();
	//�Ƿ�����ׯ�б���
	bool	IsInWaitNtList();
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
	
    ///���ó����б�
	void  SetChoumaValueList(void) ;

public:
	//������ע���
	void HandleUserBetResult(void *buffer);

	//������ׯ�������
	void HandleApplyResult(void *buffer);

	//������Ϣ
	void	HandleGameFree(void * pBuffer);

	//����
	void HandleKaiPai(void *buffer);

	//�ж��Ƿ�������ׯ
	void JudgeApplyNt(BYTE ,BOOL);

	//�յ���ʼ��ע��Ϣ
	void	HandleBeginBet(void * pBuffer);

	//����������ׯ�����Ϣ
	void HandleLeaveNtResult(void *buffer);
};
