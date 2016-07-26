#pragma once

#include "StdAfx.h"
#include"..\GameMessage\UpgradeLogic.h"
#include "GameServerData.h"

#define	CARDCOUNT						52			//ʹ�õ��˿���������Ҫ��С����
//��ʱ�� ID
#define TIME_BET						30				//��ע��ʱ��
#define TIME_SENDCARD					31				//���ƶ�ʱ��
#define TIME_CALL						32				//�зֶ�ʱ��
#define TIME_FINISH						33				//��Ϸ����ʱ��
#define TIME_STARTGAME					34				//��ʼ��Ϸ��ʱ��
/*
//��Ϸ������Ϣ
#define PLAY_MAX_NOTE				500			//�����ע
#define PLAY_BASE_NOTE				5				//��ע
#define THINK_TIME					30				//��Ϸ˼��ʱ��
#define PLAY_CARD_COUNT				1				//�˿˸���

*/

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����

//���״̬����
#define PLAYER_READY						299				//�û�׼��״̬
#define PLAYER_GIVEUP						301				//����
#define PLAYER_JOIN							302				//��;����״̬

static __int64					m_iAIHaveWinMoney;	//������ӮǮ��20121126	dwj ��������Ϊ��̬ȫ�ֱ�������Ϊ���������Ҫ��ʱ�޸ĸñ���
static __int64					m_iReSetAIHaveWinMoney;	/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ  �ñ���ҲҪ����Ϊ��̬ȫ�ֱ��� ����ÿ�����Ӷ���ȥ����*/
static int						m_iRecFroDayTime;	/**<��¼��������ǰʱ�䣬�����ж��Ƿ����24�㣬����24�����A1��A2��A3�����ϵ���m_iAddMoneyEveryDay  */

const BYTE bCardA[4]={0x0D,0x1D,0x2D,0x3D};
const BYTE bCardB[16]=
{0x09, 0x0A, 0x0B, 0x0C,
 0x19, 0x1A, 0x1B, 0x1C,
 0x29, 0x2A, 0x2B, 0x2C,
 0x39, 0x3A, 0x3B, 0x3C
};

//#define SEND_CARD_TIME				1000			//����ʱ�����ò�����10����,������1000

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
	//21��
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();

	//��Ա����
private:
	CUpGradeGameLogic		m_Logic;							//��Ϸ�߼�
	GameServerData          m_GameData;							//��Ϸ����

private:
	//��Ϸ��չ����
	static BYTE m_bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
	static BYTE m_bGameBetTime;											//��Ϸ��עʱ��
	static BYTE m_bCallCardTime;										//��Ϸ����ʱ��
	static BYTE m_bSendCardTime;										//��Ϸ����ʱ��
	static BYTE m_bFinishTime;											//��Ϸ����ʱ��
	//����״̬����
	bool					m_bIsSuperControl;						//�Ƿ��ǳ����û����ƽ��
	int						m_iNtPeople;							//ׯ��λ��
	int						m_iGameStation;							//��Ϸ״̬
	int                     m_iGameFlag;							//��ǰ��Ϸ��״̬
	int						m_iPlayerStation[PLAY_COUNT];			//���״̬
	bool                    m_bIsCut[PLAY_COUNT];					//�Ƿ��������ҪΪ����������

	int						m_bUserReady[PLAY_COUNT] ;				//���׼��״̬
	int						m_iCallLabel;							//�зֱ�ǩ
	int						m_iNeedLabel;							//Ҫ�Ʊ�ǩ
	int						m_iScoreNum;							//��������
	BYTE					m_bUserCard[PLAY_COUNT][13];			//�û��˿�
	BYTE					m_bUserAlreadySendCard[PLAY_COUNT][13];	//���û��Ѿ������˿�
	BYTE					m_bScoreSqueue[PLAY_COUNT];				//����˳��
	BYTE					m_bCurrentCallPeople;					//��ǰ�з���
	BYTE					m_bLastSuperStation;					//�����Ƶĳ����û�
	int						m_iUserPoints[PLAY_COUNT];				//�û�����
	__int64					m_i64UserScore[PLAY_COUNT];				//�û���Ӯ���
	vector<long>			m_vlSuperID;									//���泬����ҵ�����
	
	//���ñ���
private:
	//--------------add by DWJ--20121122----------------------------------
	bool		m_bIsFirstLoadIni;			//�Ƿ��һ�ζ�ȡ�����ļ� ���ڱ����һ���������ڣ����û����һ��m_iAIWantWinMoneyA1��A3����Ҫ����
	__int64		m_iAIWantWinMoneyA1;		/**<��������Ӯ���ƣ�������ӮǮ����1  */
	__int64		m_iAIWantWinMoneyA2;		/**<��������Ӯ���ƣ�������ӮǮ����2  */
	__int64		m_iAIWantWinMoneyA3;		/**<��������Ӯ���ƣ�������ӮǮ����3  */
	int			m_iAIWinLuckyAtA1;			/**<��������Ӯ���ƣ�������������1ӮǮ�ĸ���  */
	int			m_iAIWinLuckyAtA2;			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	int			m_iAIWinLuckyAtA3;			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	int			m_iAIWinLuckyAtA4;			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	__int64		m_iAddMoneyEveryDay;		/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */


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

	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);

public:
	///�������м�ʱ��
	void KillAllTimer();

	//����Ƿ����
	//bool CheckFinish();

	//��ʼ����������Ϸ����
	BOOL InitData();

	//��ȡ��̬����
	BOOL LoadIni();	

	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);

	//���AI����
	BOOL UserAI(BYTE bDeskStation,BYTE bCutType,bool bNet);

	//������Ϸ״̬
	//�ȴ�ͬ��״̬
	void OnSendGameStationAgree(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//��ע״̬
	void OnSendGameStationBet(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//����״̬
	void OnSendGameStationSendCard(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//��Ϸ��״̬
	void OnSendGameStationPlaying(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//�ȴ���һ����Ϸ��ʼ
	void OnSendGameStationNext(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//��Ϸ��Ϣ����

	//��ť��Ϣ������
	void OnButtonMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);

	//�������û����ƽ��
	void	DealSuperUserMessage(void *pBuffer);

	//��ͷ��˷�����Ϸ��Ϣ����

	//����ȷ��ׯ����Ϣ
	void SendHaveNt();

	//ѡׯ��
	int HaveNt(BYTE bNtStation);

	//��ʼ��ע
	void StartNote();

	//��ʼ����
	void SendCard();

	//������ע���޺�����
	__int64 AccountLimit();

	//����Ƿ��ܷ���
	void CheckSend();

	//��ʼ��Ϸ
	void StartGame();

	//�жϵ���
	int GetPoints(BYTE bCardList[],int iSize);

	//�����û���֤
	void	SuperUserExamine(BYTE bDeskStation);

	//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
	bool	IsSuperUser(BYTE byDeskStation);

	//ͳ����Ϸ����
	BYTE   CountPlayer();

	//ͳ�ƶ�������
	BYTE   CountCut();
};

/******************************************************************************************************/
