#pragma once

#include "StdAfx.h"
#include"..\GameMessage\UpgradeLogic.h"
#define CardCount			36							//�˿�����
//��ʱ�� ID

#define S_TIMER_WASHCARD			31				//ϴ�ƶ�ʱ��
#define S_TIMER_					201				//

#define TIMER_SETTLE				40				//���㶨ʱ��

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����


static __int64		m_iAddMoneyEveryDay;		/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
static __int64		m_iAddMoneyAllDay;			/**<��������Ӯ���ƣ���������ӮǮA1��A2��A3�����ۻ���Ǯ����ֵ  */
static CTime		m_iRecFroDayTime;	/**<��¼��������ǰʱ�䣬�����ж��Ƿ����24�㣬����24�����A1��A2��A3�����ϵ���m_iAddMoneyEveryDay  */

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
public:
	//enum GameMode {GAMEMODE_BANKER_BET,GAMEMODE_BANKER_INTURN,GAMEMODE_BANKER_CONST,GAMEMODE_INVALIDE};//��Ϸģʽ����ׯ����ׯ����ׯ,��Ч
	CUpGradeGameLogic		m_Logic;												//��Ϸ�߼�

private:
	//��Ϸ����
	ENUM_GameMode			m_GameMode;												//��Ϸģʽ
	ENUM_GameStation		m_EGameStation;											//��Ϸ״̬
	bool					m_IsModeSet;											//��Ϸģʽ�Ƿ�����
	int						m_iBasePoint[3];										//������
	int						m_PlayersCountInGame;									//��Ϸ���������
	int						m_iPKPlayerCount;										//�Ѿ�������ҵ�����
	int						m_iGamesCounter;										//������ľ���
	int						m_iBetPlayerCount;										//��ע�������

	//�������
	BYTE					m_byBankerDeskStation;									//ׯ�ҵ���λ��
	__int64					m_iBetIn[PLAY_COUNT];									//ÿ����ҵ���ע���
	bool					m_isBetBanker[PLAY_COUNT];								//�Ƿ���ׯ
	int						m_sendBetBankerPlayer;									//
	

	//��ҵ�������
	EnCardShape				m_CardResult[PLAY_COUNT];								//��ҵ����ͣ���ʼ��
	BYTE					m_byUserCard[PLAY_COUNT][5];							//������
	
	//����
	vector<long>			m_vSuperClient;											//�����û�
	BOOL					m_isWinOrLose[PLAY_COUNT];								//��Ӯ
	TSuper					m_tSuper;												//��Ӯ����
	
	//////////////////////////////////////////////////////////////////////////

	
	int						m_iStylePrize[8];										//���ͽ���
	int						m_iUnit;												//�����һ���ұ���
	BYTE				    m_iThinkTime;											//��Ϸ˼��ʱ��
	BYTE				    m_iBeginTime;											//��Ϸ��ʼʱ��
	bool					m_bIsSuperControl;										//�Ƿ񳬶˿���
	bool					m_bAuto[PLAY_COUNT];									//�Ƿ��й�
	bool					m_bIsPrepare[PLAY_COUNT];								//�Ƿ�׼��
	bool					m_bIsCompare[3][PLAY_COUNT];							//�Ƿ��Ѿ��ȹ���
	int					    m_iGetPoints[3][PLAY_COUNT];							//ÿһ�Ѹ���ҵĵ÷�
	
	int						m_iCardCount[PLAY_COUNT];								//������
	BYTE					m_bCardList[PLAY_COUNT][EveryCount];					//������ϴ����
	int					    m_bOutCardCount;										//��������
	int						m_iWhichHeap;											//��ǰ�ȵڼ�����
	int						m_iEveryPeopleStyle[PLAY_COUNT][3];						//ÿ�����ÿһ�ѵ�����

	BYTE					m_bCompareRecord[PLAY_COUNT][PLAY_COUNT];				//ÿ����ұ��Ƽ�¼
	BYTE					m_byWinSuperControl;									//���˿���Ӯλ��
	//��������
	bool					m_bIsOpenCard[PLAY_COUNT];								//�Ƿ��Ѿ�����
	bool					m_byIsCompareCard[PLAY_COUNT][3];						//��ұȹ��Ķ���
	int						m_iCompareStyle[PLAY_COUNT][3];							//�ȹ��Ķ�������
	vector<long>			m_vlSuperID;											//���泬����ҵ�����

	
	BYTE				   m_byPlayerCards[PLAY_COUNT][5];							//�ĸ���ҵ���
	EnCardShape			   m_cardShape[PLAY_COUNT];									//�ĸ���ҵ�����
	int					   m_iArrMaxOrder[PLAY_COUNT];								//�ĸ�������ʹ�С˳��
	BYTE                   m_byUserStation[PLAY_COUNT];                             //��ҵ�״̬��������Ϸ�У���;����
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
	//�û�����
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	//�û���������
	virtual bool UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo);
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
public:
	//��ʼ����������Ϸ����
	BOOL InitData();
	//��ȡ��̬����
	BOOL LoadIni();	
	//������Ϸ״̬
	//�ȴ�ͬ��״̬
	void OnSendGameStationAgree(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//ϴ��
	void OnSendGameStationWash(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//��Ϸ��ʼ״̬
	void OnSendGameStationPlaying(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//��Ϸ��Ϣ����
	//��Ϸģʽѡ��
	void OnGameMode(void * pData,BYTE bDeskStation);
	//������Ϸģʽ
	void SetGameMode(void *pData);

	//������Ϸ״̬
	void SetGameStation(void *pData);
	//��Ϸ״̬�ı�
	void OnGameStationChanged(void *pData);
	//
	/*void OnXuanZhuang(BYTE byDesakStation,void *pBuffer);*/
	//��ȡ��ǰ�����������
	int GetPlayersCountInDesk();

	//��ȡ��������Ϸ����ҵ�����������ȥ�Թ�������
	int GetInGamePlayerCount();


	//׼����Ϣ������
	void OnGMGetReady(BYTE byDeskStation,void *pData);

	//����

	void SendCard(int bDeskStation);

	//�����û����͹�������
	void ReceiveClientCards(BYTE byDeskStation,void *buffer);

	//��������
	void analyseCardShape(BYTE byDeskStation,T_CS_OutCard* ptCoutCards);

	//���ֺ��Ƶ������ݹ㲥���������
	void ShowCardsToAll(BYTE byDeskStation,void * pOutCards);

	//����
	void Settle();

	//������Ϸ״̬������
	void GameOverRest();

	//�й�
	void AutoPlay(BYTE byDeskStation);

	//�й��ж����ͺ���
	EnCardShape AutoPlayGetCardShape(BYTE *pCard,int iLength,T_CS_OutCard* outCards);

	//��ׯ
	void BetBanker();
	
	// ������� 
	EnCardShape GetCardShape(BYTE *pbyCards, int iCounter);
	//�޸Ľ�������1(ƽ̨->��Ϸ)
	/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//�޸Ľ�������2(ƽ̨->��Ϸ)
	/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ�
	/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ���
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//�޸Ľ�������3(ƽ̨->��Ϸ)
	/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);

	//����ʤ��
	/// �ж�
	virtual BOOL Judge();
	/// ʤ
	virtual BOOL JudgeWiner();
	/// ��
	virtual BOOL JudgeLoser();
	//GRM����
	//���½�������(��Ϸ->ƽ̨)
	/// @return ������ӮǮ��
	virtual void UpDataRoomPond(__int64 iAIHaveWinMoney);
	void OnHandleUserNetCut(BYTE bDeskStation, CGameUserInfo* pLostUserInfo);
	void AutoChoiseOutCard(BYTE bDeskStation,T_CS_OutCard &tOutCards);
};
