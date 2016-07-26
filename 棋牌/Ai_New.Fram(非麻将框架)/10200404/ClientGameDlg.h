#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpgradeMessage.h"
#include "UpGradeLogic.h"



//��ʱ�� ID
#define ID_BEGIN_TIME		100							//��ʼ��ʱ��

#define TIMER_GETREADY		101							//׼��
#define TIMER_CALLLEVELCARD 102							//����
#define TIMER_BURYBACKCARD  103							//����
#define TIMER_OUTCARD		104							//����

//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	/////////////////////////
public:
	CUpGradeGameLogic   m_cLogic ;             ///�ͻ����߼�
private:
	BYTE				m_byCards[33];				//������
	vector<BYTE>		m_vbyCards;					//������
	BYTE				m_byBackCard[8];			//����
	int					m_CardCounts;				//25��Ҫ��ʾ����
	
	int					m_timeToGetReady;			//׼��ʱ��
	int					m_timeToCallLevelCard;		//����ʱ��

	BYTE				m_byGameLevel[2];		//����
	BYTE				m_byGameLevelColor;		//���ƻ�ɫ
	vector<BYTE>		m_vbyBigJor;			//����
	vector<BYTE>		m_vbySmallJor;			//С��
	vector<BYTE>		m_vbyLevelCard;			//����

	T_CS_OutCard		m_tFirstPlayerCard;		//һȦ�е�һ�����Ƶ���ҳ�����
	T_CS_OutCard		m_tCurrentOutCard;		//��ǰ������
	bool				m_isFirstOutCard;		//�Ƿ��һ������
	
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
	
	void ResetGameStatus();

public:

// 	��Ϸ��ʼ
// 		void OnGameBegin(GameBeginStruct * pGameBegin);
	//��Ϸ״̬����
	void GameStationProc(TGameStation& tGameStation);

public:
	//�����ע
	void OnSuiJiXiaZhu();	
		
	//����һ�������
	int GenerateRand(int iBase);
	//���������ݣ�����ʾ
	void ReceiveAndShowCards(TSendCards* ptSendCards);
	//ʱ�䵽���Զ�����
	void OutTimeAutoOutCard();
	//��ׯ����
	void FindCardsCompatibleWithFirst(T_CS_OutCard &tFirstOutCard,BYTE *pOutCards,int* piCount);
	//��ʾ���ڵĵȼ�
	void ShowCurrentGameLevel(TGameLevel *pTGameLevel);
	//����׼��,���˳���Ϸ��Ϣ��Ϣ	 , isReadyOrQuit - false�� �˳���Ϸ�� true:׼����
	void ReadyOrQuit(bool bReadyOrQuit);
};

