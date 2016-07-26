#pragma once
#include "GameDesk.h"
#include <afxmt.h>
#include "StdAfx.h"
#include"..\GameMessage\UpgradeLogic.h"
#include<vector>
//��ʱ�� ID
#define TIME_CALL_NT				30				//��ׯ
#define TIME_NOTE                   31              //��ע��ʱ��
#define TIME_NOTE_FINISH			32				//��ע��ɶ�ʱ��
#define TIME_SEND_CARD				33				//���ƶ�ʱ��
#define TIME_TAKE_CARD				34				//Ҫ�Ƽ�ʱ��
#define TIME_GAME_FINISH			35				//��Ϸ������ʱ��
#define TIME_LOST_TIME				36				//��ʧʱ��(ÿ��ִ��һ��)

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����


static __int64					m_iAIHaveWinMoney;	//������ӮǮ��20121126	dwj ��������Ϊ��̬ȫ�ֱ�������Ϊ���������Ҫ��ʱ�޸ĸñ���
static __int64					m_iReSetAIHaveWinMoney;	/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ  �ñ���ҲҪ����Ϊ��̬ȫ�ֱ��� ����ÿ�����Ӷ���ȥ����*/
static int						m_iRecFroDayTime;	/**<��¼��������ǰʱ�䣬�����ж��Ƿ����24�㣬����24�����A1��A2��A3�����ϵ���m_iAddMoneyEveryDay  */


//��Ϸ����
class CServerGameDesk : public CGameDesk
{
protected:
	//����״̬����
	int						m_iNtPeople;                          //Ӯ��
	int						m_iUpGradePeople;					//ׯ��λ��

	BYTE					m_iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	//BYTE					m_iLeaveArgee;						//�뿪ͬ���־
	BYTE					m_iHaveThingPeople;					//����Ҫ�����
	
	bool                    m_bUserReady[PLAY_COUNT] ;         ///���׼��״̬
	BOOL					m_bReadyOpenCard[PLAY_COUNT];			//ͣ��״̬
	//------------------------------------------------------------------------------------
	//״̬��Ϣ
	BYTE					m_iUserCardCount[PLAY_COUNT];					//�û������˿���Ŀ
	BYTE					m_iUserCard[PLAY_COUNT][SH_USER_CARD];			//�û����ϵ��˿�
	BYTE					m_byGetCardCount[PLAY_COUNT];					//�Ѿ����Ƶ�����
	BYTE					m_byCanGetCard[PLAY_COUNT];						//�Ƿ����Ҫ�Ƶı�־λ
	int						m_iCallScore[PLAY_COUNT];						//��ׯ״��


	//��������
	BYTE					m_iTotalCard[54];					//�ܵ���

	CUpGradeGameLogic		m_Logic;							//��Ϸ�߼�

	int						m_iStopCardCount;                  //ͣ����ҵ�����

	int                     m_MaxPlayer;                        //��ע�мҵ�����;
	int						m_iUserStation[PLAY_COUNT];				//��¼�û�״̬
	int						m_iSuperCount;							//��������
	__int64					m_iPerJuTotalNote[PLAY_COUNT];			//�û�ÿ��ѹ��ע

	bool                    m_bIsCut[PLAY_COUNT];              //�Ƿ��������ҪΪ����������
	BYTE					m_iIngPlayNote;					//��ǰ��ע�ߣ����Թۣ�
	BYTE					m_iSendPos;				//��һ�ַ��ƿ�ʼλ��
	BYTE                    m_iPrepareNT;              //Ԥ��ׯ��
	BYTE					m_bCurrentCallScore;		//��ǰ�з���
	BYTE                    m_iFirstJiao;
	BYTE                    m_iGameFlag;             //��ǰ��Ϸ��״̬
	//BOOL                    m_bIsSuper[PLAY_COUNT];   //�Ƿ����ʹ�÷���

	//��������
	SuperUserSetData		m_SuperSetData;				//�������õ�����

	//__int64					m_iAIHaveWinMoney;		 //������ӮǮ��20121122	dwj
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



private:
	__int64 m_iLimitNote[PLAY_COUNT];	//����
	__int64	m_i64UserNoteLimite[PLAY_COUNT][4];	//����ĸ���ע����
	__int64 m_iLimit;//��������

	//=============��չ
	static 	int m_iSendCardTime;//��Ϸ˼��ʱ��
	static 	BYTE m_iThinkTime;//��Ϸ˼��ʱ��
	static 	BYTE m_iBeginTime;//��Ϸ��ʼʱ��
	static 	BYTE m_iCallScoreTime;//��ׯʱ��
	static 	BYTE m_iXiaZhuTime;  //��עʱ��
	static  BOOL m_bTurnRule;	//��Ϸ˳��
	BYTE	m_byLostTime;		//��¼��ʱ��ʣ��ʱ��
	DWORD m_iCardShape;	//����

	/// ���ͱ���
	BYTE m_bCardShapeBase[MAX_SHAPE_COUNT];//0-5�����㵽��С���ı���

	int  m_iGoodCard;//�����ƻ���ʮ������
	bool m_bHaveKing;//�Ƿ�����


	//�������� 
	vector<long>			m_vlSuperID;							//���泬����ҵ�����


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
	virtual bool	GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool	GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
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
//	virtual bool UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo);
	//�޸Ľ�������1(ƽ̨->��Ϸ)
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//�޸Ľ�������2(ƽ̨->��Ϸ)
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//�޸Ľ�������3(ƽ̨->��Ϸ)
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);
	//����ʤ��
	/// �ж�
	virtual BOOL Judge();
	/// ʤ
	virtual BOOL JudgeWiner();
	/// ��
	virtual BOOL JudgeLoser();
	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//20121203dwj ��¼��������Ӯֵ
	void RecordAiHaveWinMoney(GameEndStruct *GameEnd);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//������
public:
	//��ȡ��̬����
	BOOL	LoadIni();	

	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);


	//-------ADD-BY-DWJ-20121108--------------------------------
	//����Ƿ��Ѿ��µ�һ���ˣ���Ҫ�����趨�����˿������ò���
	void CheckNewDayReSetIni(CINIFile *f, CString *key);

	/**�������ļ��ж�ȡ�����˿������õĲ���ֵ*/
	void GetAIContrlSetFromIni(int iRoomID);			
	//�����û���֤
	void	SuperUserExamine(BYTE bDeskStation);
	//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
	bool	IsSuperUser(BYTE byDeskStation);

	//ͳ����Ϸ����
	BYTE CountPlayer();

	//ͳ�Ƶ�ǰδ�������
	BYTE CountNotePeople();

	//���䱶��
	int GetRoomBasePoint();

	//������ҷ���
	int ComputePoint(BYTE DeskStation);
	//�õ�����ƺ���С��,winֵΪ1ʱ�õ����,����Ϊ��С
	BYTE GetIndex(int win);

	//����
	BOOL ChangeCard(BYTE bDestStation,BYTE bWinStation);
	//����ָ��������ҵ�����
	void	ChangeTwoUserCard(BYTE byFirstDesk,BYTE bySecondDesk);
	
	//��ȡ��һ�����λ��
	BYTE GetNextDeskStation(BYTE bDeskStation);
	//���͸���һ����ׯ
	BOOL	SendCallScore(BYTE bDeskStation);
	//��ׯ
	BOOL	UserCallScore(BYTE bDeskStation, int iVal);
	//�зֽ���
	BOOL	CallScoreFinish();
	//֪ͨ�û���ע
	BOOL	NoticeUserNote();
	//�����û���ע���
	BOOL	UserNoteResult(BYTE bDeskStation, BYTE iVerbType,int iNoteType = 0);
	//�����˿˸��û�
	BOOL	SendCard();
	//���ƽ���
	BOOL	SendCardFinish();
	//��Ϸ��ʼ
	BOOL	BeginPlayUpGrade();

	//��ҿ�ʼ��Ϸ��Ҫ�ƣ�
	BOOL    UserYaoPai(BYTE byDeskStation,void * pData);
	//ͣ��
	BOOL    UserStopCard(BYTE byDeskStation,void * pData);

	BOOL	CheckFinish();
	//������Ͷע�������
	__int64 GetLimit(int bDeskStation);
	//ͳ�Ƶ������
	BYTE    CountCutPlayer();
	//���AI����
	BOOL    UserAI(BYTE bDeskStation,BYTE bCutType,bool bNet);

	//����ǿ�����
	void    ClearQuitPlayer();
	///�������м�ʱ��
	void    KillAllTimer();;

	///��ȡ��ǰ�����������ͣ�
	int     GetMaxCardShape(BYTE iCard[], int iCardCount);

	///20121122dwj��������Ӯ�Զ�����,
	void    IAWinAutoCtrl();
	///20121122dwj���㵱ǰ�ƻ����˵���ӮǮ
	int     CountAIWinMoney();

};

/******************************************************************************************************/
