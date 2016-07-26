#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpgradeMessage.h"
#include"UpGradeLogic.h"


//��ʱ�� ID
#define ID_BEGIN_TIME		100										//��ʼ��ʱ��
#define ID_SCORE_TIME       101 									//�зֶ�ʱ��


//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	/////////////////////////
public:

	CUpGradeGameLogic					m_Logic;										//�ͻ����߼�
	//�̶���Ϣ
	BYTE								m_iVersion;
	BYTE								m_iVersion2;
	BYTE                                m_iMyDeskStation;                               //�Լ�λ��
	BYTE								m_iThinkTime;									//��Ϸ˼��ʱ��
	BYTE								m_iBeginTime;									//��Ϸ��ʼʱ��
	//������Ϣ
	int								    m_bUserPrepare[PLAY_COUNT];								//���׼��
	BYTE								m_byGameStation;                                //��Ϸ������Ϣ
	BYTE					            m_bNtPeople;									//ׯ��
	BYTE								m_DeskCard[PLAY_COUNT][CARD_NUMS];			    //ÿ���������ʵ�˿�
	BYTE								m_bOwnCard[CARD_NUMS];							//�Լ����˿�
	BYTE								m_iDeskCardCount[PLAY_COUNT];					//ÿ�������˿˵���Ŀ
	BYTE								m_bLeftCard[CARD_NUMS];							//�ϼҳ�����
	BYTE								m_bCurrentCall;									//��ǰ�������
	bool								m_bIsOutCard;									//ʱ�䵽���Ƿ��Լ�һ��Ҫ����
	
	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();
    int GetGameStatus();

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
	//��������
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);

	//������Ϸ
	void ResetGameStation(int iGameStation);
	//ɾ���ƶ�����
	void RemoveCard(BYTE bCardList[],BYTE bTarCard,int iCount);
	//����������
	void RestartSort(BYTE bCardList[],int iCount);
public:
	//����ͬ����Ϸ��Ϣ
	void OnAgreeGame(void *buffer);
	//������
	void OnHandleSendCardBegin(void *buffer);
	//������Ϸ��ʼ
	void OnHandleGameBegin(void *buffer);
	//����������������Ľ�����Ϣ
	void OnHandleCall(void *buffer);
	//��ʾ�÷���
	void OnHandleShowStyle(void *buffer);
	//�������������������Ϸ����������Ϣ
	void OnHandleNormalFinish(void *buffer);
	//�������������������Ϸ�й���Ϣ
	void OnHandleAuto(void *buffer);

	//�������ж��Ƿ����
	void RobotOutCard();
};
