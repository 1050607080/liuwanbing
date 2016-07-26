#pragma once

#include "../../../../sdk/include/ServerCommon/GameDesk.h"
#include"..\GameMessage\UpgradeLogic.h"

//��ʱ�� ID
#define IDT_USER_CUT				1L								/**< ���߶�ʱ�� ID */
#define TIME_SEND_CARD				30								/**< ���ƶ�ʱ�� */
#define TIME_GAME_FINISH			32								/**< ��Ϸ������ʱ�� */
#define TIME_SEND_A_CARD			33								/**< ����һ���� */
#define TIME_BEGIN_ANIMAL			34								/**< ��Ϸ��ʼ���� */
#define TIME_COLLECT_JETON			35								/**< �ռ����� */
#define TIME_NOTE                   36                              /**< ��ע��ʱ�� */
#define TIME_WAIT                   37                              /**< �ȴ���ע������ɼ�ʱ�� */

//��Ϸ������־����
#define GF_NORMAL					10								/**< ��Ϸ�������� */
#define GF_SALE						11								/**< ��Ϸ��ȫ���� */
#define GF_AHEAD					12								/**< ��ǰ���� */
#define GF_NO_WINER					13								/**< δ�ܹ�����ʤ�� */
//#define SEND_CARD_TIME				1000						/**< ����ʱ�����ò�����10����,������1000 */
#define SH_USER_CARD				5

static __int64	m_iAIHaveWinMoney;		//������ӮǮ��20121126	dwj ��������Ϊ��̬ȫ�ֱ�������Ϊ���������Ҫ��ʱ�޸ĸñ���
static __int64	m_iReSetAIHaveWinMoney;	/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ  �ñ���ҲҪ����Ϊ��̬ȫ�ֱ��� ����ÿ�����Ӷ���ȥ����*/
static int		m_iRecFroDayTime;		/**<��¼��������ǰʱ�䣬�����ж��Ƿ����24�㣬����24�����A1��A2��A3�����ϵ���m_iAddMoneyEveryDay  */

/// ��Ϸ����
class CServerGameDesk : public CGameDesk
{
protected:
	//����״̬����
	int						m_iUpGradePeople;						/**< ׯ��λ�� */
	BYTE					m_iBeenPlayGame;						/**< �Ѿ���Ϸ�ľ��� */
	BYTE					m_iHaveThingPeople;						/**< ����Ҫ����� */

	//״̬��Ϣ
	BYTE					m_iUserCardCount[PLAY_COUNT];			/**< �û������˿���Ŀ */
	BYTE					m_iUserCard[PLAY_COUNT][SH_USER_CARD];	/**< �û����ϵ��˿� */

	//������Ϣ
	int						m_iOutCardPeople;						/**< ���ڳ����û� */
	int                     m_iLastOutCardPeople;					/**< �������û� */
	int						m_iFirstOutPeople;						/**< �ȳ��Ƶ��û� */
	int                     m_iFirstNotePeople;                     /**< ������ע��� */
	int						m_iOutCardOrder;				//����˳��
	//��������
	BYTE					m_iTotalCard[52];						/**< �ܵ��� */
	BYTE					m_iSendCardPos;							/**< ����λ�� */

	CUpGradeGameLogic		m_Logic;								/**< ��Ϸ�߼� */
	int						m_iUserStation[PLAY_COUNT];				/**< ��¼�û�״̬ */
	__int64					m_iUserNote[PLAY_COUNT];				/**< �û�ÿ��ѹע */
	__int64					m_iPerJuTotalNote[PLAY_COUNT];			/**< �û�ÿ��ѹ��ע */
	__int64					m_iNowNote[PLAY_COUNT];					/**< ��ǰѺע�� */
	__int64					m_iTotalNote;							/**< ��ע */
	BYTE					m_iLastShuttle;							/**< ��һ������ */
	BYTE					m_bNoteTimes;							/**< ��ע���� */
	BYTE					m_bAddTime;								/**< ��ע��� */
	BYTE					m_UserThisTurnAct[PLAY_COUNT];			/**< �ĸ�����ע��� */
	BYTE					m_iNowBigPeople;						/**< �������ѹע��(���һ�����Ѻ400,�ڶ�����Ҽӱ�,��Ϊ���Ѻע��) */
	BYTE					m_iSendPos;								/**< ��һ�ַ��ƿ�ʼλ�� */
	__int64					m_iThisNoteLimit;						/**< ÿ����ע */
	BYTE					m_byUserFollow;							/**<  �ڼ��ѱظ� */
	long					m_dwDeskOwer;							/**< ���� */
	bool					m_bNetCutPlayer[PLAY_COUNT];			/**< ǿ����� */
	__int64                 m_iGameAddBasePoint[13];				/**<��Ϸ���б����ĵȼ�*/
	bool					m_bUserState[PLAY_COUNT];				/**<���״̬(������Ϸ��ʼ�������ɽ��빦��ʱ��ӵ����ݳ�Ա)*/	
	BYTE					m_byUserAddNum[PLAY_COUNT];				//���ָ�����Ҽ�ע����
	__int64					m_i64JudgeScore[12];					/**<�жϵȼ��ķ�����*/ //add xyh 2012-1-10

	//��������Ӯ�ؼ���ر���
private:
	BOOL					m_bAIWinAndLostAutoCtrl;	//�����˿�����Ӯ	20121122	dwj	

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
	bool		m_bChangeRobotCard[PLAY_COUNT];	/**<�Ƿ�ı��˻����˵��� true �ı��� false û�иı�  */
	__int64     m_i64AcculateGrade[PLAY_COUNT];//����ۻ��ĳɼ�
	__int64     m_i64UpGrade[PLAY_COUNT];//��һ�ֵĳɼ�
#ifdef SUPER_VERSION
	BYTE					m_bSuperCardList[8][54];				/**< �����ͻ��� */
	int						m_iSuperCardCount[8];					/**<  */
	int						m_bSuperStation;						/**< �����ͻ���λ�� */
	struct SetCardStruct											/**< �ͻ��˵������� */
	{
		BYTE pai[8][54];
		int	paicount[8];
	};
#endif
public:
	static BYTE m_iAllCardCount;									/**< ��Ϸ���õ����� */
	__int64 m_iLimitNote;											/**< ���� */
	__int64 m_iBaseNote;											/**< ��ע */
	__int64 m_iBaseNoteEx;											/**< ��ע */
	int     m_iBaseScale;                                           /**< �Զ���ע���� */
	int		m_iFirstSendCardCount;									/**< ��һ�η��������� */
	BYTE    m_bShowHa;                                              /**< �ڼ��ֿ������ */    
	BYTE    m_byAddNum;												/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
	BYTE    m_bPK;                                                  ///�Ƿ�PK PK���ܣ���һ������ˣ���һ��ֻ�ܸ������ܷ���
	//��չ
	static 	BYTE m_iSendCardTime;									/**< ��Ϸ˼��ʱ�� */
	static 	BYTE m_iThinkTime;										/**< ��Ϸ˼��ʱ�� */
	static 	BYTE m_iBeginTime;										/**< ��Ϸ��ʼʱ�� */
	static 	DWORD m_dwCardShape;									/**< �������� */

	//��������
public:
	/// ���캯��
	CServerGameDesk(); 
	/// ��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	/// ��ʼ��Ϸ
	virtual bool InitDeskGameStation();

	/// ��Ϸ��ʼ
	/// @param bBeginFlag ��ʼ��ʽ
	virtual bool GameBegin(BYTE bBeginFlag);
	
	/// �������޺͵�ע
	BOOL SetLimitNote();
	
	/// ��Ϸ����
	/// @param bDeskStation ������Ϸ���������ID
	/// @param bCloseFlag ��Ϸ�����ķ�ʽ
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	
	/// �ж��Ƿ�������Ϸ
	/// @param bDeskStation ���ID
	virtual bool IsPlayGame(BYTE bDeskStation);

	/// �����Ϣ������
	/// @param bDeskStation ���ID
	/// @param pNetHead ��Ϣͷ
	/// @param pNetData ��Ϣ����
	/// @param uDataSize ��Ϣ��С
	/// @param bWatchUser �Թ�
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	
	/// ��Ϸ���ݰ�������
	/// @param bDeskStation ���ID
	/// @param pNetHead ��Ϣͷ
	/// @param pNetData ��Ϣ����
	/// @param uDataSize ��Ϣ��С
	/// @param bWatchUser �Թ�
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	
	/// �û��뿪��Ϸ��
	/// @param bDeskStation ���ID
	/// @param pUserInfo �����Ϣ
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);

	/// ����̨��
	virtual void ChangeDeskOwer(BYTE bDeskStation);

public:
	//��ȡ��̬����
	BOOL LoadIni();
	//���ݷ���ID���ص�ע������
	//@param iRoomID ����ID ���ݲ�ͬ�����ȡ��ͬ������
	BOOL LoadExtIni(int iRoomID);

	//���ش��麯��
public:
	/// ��ȡ��Ϸ״̬��Ϣ
	/// @param bDeskStation ���ID
	/// @param uSocketID ��Ϸ�ɲ���
	/// @param bWatchUser �Ƿ��Թ�
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	/// ������Ϸ״̬
	/// @param bLastStation ���õ�״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	/// ��ʱ����Ϣ
	/// @param uTimerID TimerID
	virtual bool OnTimer(UINT uTimerID);
	
	//������
public:
	/// �����˿˸��û�
	BOOL SendCard();
	/// �l�ƽY��
	BOOL SendCardFinish();
	/// ��Ϸ��ʼ
	BOOL BeginPlay();
	/// ������ע
	BOOL SendLimit();
	/// ��һ�ֿ�ʼ
	/// @param bDeskStation ���ID
	BOOL NewPlayTurn(BYTE bDeskStation);
	/// ��һ������
	BOOL SendACard ();
	/// �Ѿ����
	BOOL AlreadyShow();
	/// �����û���ע
	BOOL SendUserNote();
	/// �û������뿪
	/// @param bDeskStation ���ID
	/// @param szMessage �뿪����
	bool UserHaveThing(BYTE bDeskStation, char * szMessage);
	
	/// ͬ���û��뿪
	/// @param bDeskStation ���ID
	/// @param bArgee �Ƿ�ͬ��
	bool ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee);
	
	/// ��������
	//int GetUpGrade(int iGamePoint, int iPlayCardCount);
	/// �����û����
	bool UserActiveResult(BYTE bDeskStation, BYTE iVerbType,__int64 iNoteVal=0);
	/// ����Ƿ����
	BOOL CheckFinish();
	//�����û�����ע
	//void ModifyPlayNote(BYTE bDeskStation);
	
	/// ��ȡ������λ��
	/// @param bExtVal  ��ʾ�Ƚ����ƻ��ǱȽ�ȫ����
	int GetCardGreater(BOOL bExtVal=FALSE);
	
	/// ����λ��
	BOOL CanSendCard(BYTE bDeskStation);
	/// ͳ����Ϸ����
	BYTE CountPlayer();
	/// ͳ����Ϸ����
	BYTE CountActivePlayer();
	/// �ռ�����
	BOOL CollectJeton(BOOL bOnly=false);
	/// �Q��
	BOOL ChangeBackCard(BYTE bDeskStation,BYTE bSourceCard,BYTE bResultCard);
	/// ������Ƿ���ĳ���Ƿ���Q�ɹ�
	BOOL ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard);
	/// ͳ�Ʊ���(��ͷ)
	__int64 GetHumanBasePoint();
	/// ���汶��
	int GetDeskBasePoint();
	/// ���䱶��
	int GetRoomBasePoint();
	/// ���ܿ۷�
	int GetRunPublish();
	/// ���ܿ۽��
	/// @param bDeskStation �������ID
	__int64 GetRunPublishMoney(BYTE bDeskStation);
	/// ��ǿ����ҵ÷�
	/// @param bDeskStation ���ID
	int GetRunAwayOtherGetPoint(BYTE bDeskStation,BYTE bRunDeskStation);
	/// �������й�
	/// @param bDeskStation ���ID
	/// @param bType 1��ʾ����,2��ʾǿ��
	BOOL UserAI(BYTE bDeskStation ,BYTE bType = 1);
	/// ������Ϸ�ж��Ƿ�����Ϸ��
	BOOL  IsPlayingByGameStation() ;
	/// ��һ�������
	BOOL FistShutle();
	/// �ж�
	BOOL Judge();
	/// ʤ
	BOOL JudgeWiner();
	/// ��
	BOOL JudgeLoser();	
	/// ˭��
	BYTE GetWiner();
	/// ����
	BOOL ChangeCard(BYTE bDestStation,BYTE bWinerStation);
	/// ���ǿ��
	BOOL UserFourceQuit(BYTE bDeskStation);
	/// ɱ���м�ʱ��
	void KillAllTimer();
	/// �������
	/// @param pUserSit ����λ�õ�������Ϣ
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	/// ����
	virtual long GetTableOwnerID(){return m_dwDeskOwer;};
	//��ȡ����Ļ�������
	int  GetRoomLimitBasePoint();

	//�ж��Ƿ񳬼��û�
	bool IsSuperUser(BYTE station);//lym 2011-11-28
	//���������
	int MyRand();
	//��ȡȨ�����ֵ
	int GetWeight(unsigned int iPoint[], int iCount);
	//�����˿ؼ���Ӯ��ط���
public:
	void GetAIContrlSetFromIni(int iRoomID);				//�������ļ��ж�ȡ�����˿������õĲ���ֵ

	void CheckNewDayReSetIni(CINIFile *f, CString *key);	//����Ƿ��Ѿ��µ�һ���ˣ���Ҫ�����趨�����˿������ò���

	void RecordAiHaveWinMoney(GameEndStruct *GameEnd);		//20121203dwj ��¼��������Ӯֵ

	void IAWinAutoCtrl();									//20121122dwj��������Ӯ�Զ�����,
	
	int GameWinerIsRobot(int &iWiner);						///��ϷӮ���Ƿ��ǻ�����
	__int64 GetHalfforLessUserMoney();//������ٽ��������Ͻ�ҵ�һ����
	int getFirstGreate(void);
};
/******************************************************************************************************/
