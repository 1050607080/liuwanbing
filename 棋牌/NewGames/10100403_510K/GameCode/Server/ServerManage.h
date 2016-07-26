#pragma once

#include "StdAfx.h"
#include"..\GameMessage\UpgradeLogic.h"

//��ʱ�� ID
#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIME_FINISH			        31				//������ʱ��
#define TIMER_SCORE					32              //�зֶ�ʱ��
//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����
//���״̬
#define LW_NO						20				//û�г�����
#define LW_PASS						21				//�Ѿ�������
#define LW_NOOUT					22				//����

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
public:
	CUpGradeGameLogic		m_Logic;							//��Ϸ�߼�
private:
	//�̶���Ϣ
	static BYTE				m_iThinkTime;											//��Ϸ˼��ʱ��
	static BYTE				m_iBeginTime;											//��Ϸ��ʼʱ��
	//������Ϣ
	int						bUserPrepare[PLAY_COUNT];								//���׼��
	int						m_iAddNoOPerate[PLAY_COUNT];							//�޲����Ĵ������ﵽ�����й�
	BYTE					m_bPeopleHandleCard[PLAY_COUNT][CARD_NUMS];				//��������˿�
	BYTE					bLastFistOutPeople;										//��һ�ֵ�һ��������
	BYTE                    m_bNextStation;											//���ֽз���
	BYTE					bNtPeople;												//ׯ��
	bool					bIsCanStart;											//�Ƿ���Կ�ʼ��Ϸ
	bool					bIsStart;												//��һ�ֳ����Ƿ�ʼ
	bool					bEndPress;												//���˳����ƣ�������������Ƿ�������ѹס
	BYTE					bDeskCard[52];											//�����û��˿�
	int						bPeopleStation[PLAY_COUNT];								//���״̬
	__int64					m_iPoint[PLAY_COUNT];									//��ҷ���
	BYTE					bHavePointCard[12];										//�����з���
	BYTE					m_bPeoplePoint[PLAY_COUNT][12];							//ÿ���������з���
	int						m_iPeopleHandCount[PLAY_COUNT];							//��������˿�����
	BYTE					m_bLeftCard[CARD_NUMS];							        //�ϼҳ�����
	BYTE					bOutBigPeople;											//����ӵ������Ƶ����
	BYTE					m_DeskBehave[PLAY_COUNT][CARD_NUMS];					//ÿ���������ϵı��֣����ڶ�������
	int						m_iClockLabel;											//ʱ�ӱ�ǩ
	bool					m_IsCut[PLAY_COUNT];									//�Ƿ����
	bool					m_IsAuto[PLAY_COUNT];									//�Ƿ��й�
	//��������
	int						m_iIsMatch;										//�Ƿ�����
	BYTE					m_bMatchCard[CARD_NUMS];						//�������
	BYTE					m_bMatchPeople;									//������
	vector<BYTE>            m_vFinishSort;									//�����Ƶ��������
	//��������
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	//��ʼ����Ϸ�߼�
	virtual bool InitDeskGameStation();
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	//�����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	//�û���������
	virtual bool UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo);

	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//������
public:
	//��ȡ��̬����
	BOOL LoadIni();	//׼�����ƹ���
	//������ע
	BOOL LoadExtIni();
	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);
	//�����û���֤
	void SuperUserExamine(BYTE bDeskStation);
	//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
	bool IsSuperUser(BYTE byDeskStation);
	///�������м�ʱ��
	void KillAllTimer();
	//��ť��Ϣ������
	void OnButtonMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	
	//�й�
	void Auto(BYTE bDestStation);
	//ȡ���й�
	void CancelAuto(BYTE bDestStation);
	//ͳ�ƶ�������
	int GetCutNum();
	//ͳ���й�����
	int GetAutoNum();
	//�Զ�����
	BOOL AutoOutCard(BYTE bDeskStation,bool bFlag=true);
	//��Ϸ����������ж������
	void CleanUpCut();
public:
	//��ʼ����
	void SendCard();
	//��ʼ��Ϸ
	void StartGame();
	//�õ���һ��������
	BYTE GetFirstOutCard();
	//��ʼ����һ�ֳ�������
	void InitRound();
	//����Ƿ���Կ�ʼ�µ�һ��
	bool CheckRound();
	//�õ���һ���������
	BYTE GetNextPeople(BYTE bCurrentPeople);
	//����Ƿ���Խ���
	bool CheckFinish(int bPeopleCount[],int iCount);
	//֪ͨ�ͷ�����ʾ���͵÷�
	void InformShowStyle();
	////���һ��̯��
	void InformShowCard();
	//�Ƿ�����
	bool IsMatch();
	//����Ұ��÷ֶ��ٽ�������
	void Ranking(BYTE rank[],int iRandCount,__int64 i64Point[],int iPointCount);
	//�õ������Ƶ����
	int GetOutputEndCard();
	//���������˿�
	void ResetCard(BYTE (* bHandleList)[CARD_NUMS]);
	//�����շ�
	void CollectPoint(BYTE bDeskStation);
};

/******************************************************************************************************/
