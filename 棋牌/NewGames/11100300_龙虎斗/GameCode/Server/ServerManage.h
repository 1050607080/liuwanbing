#pragma once

#include "GameDesk.h"
//#include "../GameMessage/UpGradeLogic.h"
#include "GameTaskLogic.h"
#include <afxmt.h>
#include <deque>
#include<list>
#include<map>
//��ʱ�� ID
#define IDT_USER_CUT				1L				//���߶�ʱ�� ID

//#define TIME_SEND_CARD				30				//���ƶ�ʱ��

//#define TIME_SEND_CARD_ANI      	32				//�ȴ�����
//#define TIME_SEND_CARD_FINISH       33              //���ƽ���
//#define TIME_ROB_NT					34				//������
//#define TIME_ADD_DOUBLE				35				//�Ӱ�
//#define TIME_OUT_CARD				36				//����
//#define TIME_WAIT_NEWTURN			37				//��һ��ʱ������
//#define TIME_GAME_FINISH			38				//��Ϸ������ʱ��
//#define TIME_JIAO_FEN               39              //�зֶ�ʱ��
//#define TIME_SHOW_CARD              40              //����

#define	TIME_APPLY_NT				33				//������ׯ��ʱ��
#define TIME_SEND_CARD              34               //���ƶ�ʱ��
#define TIME_START_GAME				41				//������ʼʱ������


//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SAFE						11				//��Ϸ��ȫ����
#define GF_NO_CALL_SCORE			13				//���˽з�
#define GF_TERMINATE				14				//����ֹͣ��Ϸ
#define GF_AHEAD_END				15				//��ǰ����

//*****************************************************************************************
//��Ϸ����
class CServerGameDesk : public CGameDesk
{
private:
	CGameTaskLogic          m_Logic ;								//��Ϸ�е��߼������������߼���

	static 	bool  m_bHaveKing;      								//�Ƿ�����(DEF=1,0)
	static 	BOOL  m_bKingCanReplace;								//���ɷ������(DEF=1,0)
	static 	bool  m_bRobnt;         								//�Ƿ����������
	static 	bool  m_bAdddouble;    									//�Ƿ���ԼӰ�
	static 	bool  m_bShowcard;     									//�Ƿ���Լӱ�
	static 	UINT  m_iPlayCard;     									//�����˿˸���(1,DEF=2,3
	static 	UINT  m_iPlayCount;    									//ʹ���˿���(52,54,104,DEF=108)
	static 	UINT  m_iSendCount;    									//������(48,51,DEF=100,108)
	static 	UINT  m_iBackCount;    									//������(3,6,DEF=8,12)
	static 	UINT  m_iUserCount;    									//�����������(12,13,DEF=25,27)
private://��Ϸ������
	BYTE			m_byFreeTime;									//����׼��ʱ��
	BYTE			m_byBetTime;									//��עʱ��
	BYTE			m_byOpenCardTime;								//����ʱ��
	BYTE			m_bySendSpaceTime;								//���Ƽ��ʱ��
	BYTE			m_bySpeedTime;									//�ڵ����ٶ�
	BYTE			m_byShowWinTime;								//�ж���Ӯ
	BYTE			m_iPlayerCount;									//��Ϸ����
	BYTE			m_byEvent;										//��Ϸ����
	BYTE			m_byOpenTime;									//����ʱ��
	BYTE			m_byShowResultTime;								//����ʱ��
	BYTE			m_byShowResult;									//������ʾʱ��
	BYTE			m_bySendCardTime;								//����ʱ��
	BYTE			m_byWinArea;									//�н�����,0������Ӯ��1����Ӯ��2������
	BYTE			m_byMaxNtPeople;								//��ׯ��������
	BYTE			m_iGameEven;									//���ٰ�Ϊһ��
	BYTE            m_iRobotWinLuckyAtA1;							//<������������1ӮǮ�ĸ���  
	BYTE			m_iRobotWinLuckyAtA2;							//<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  
	BYTE			m_iRobotWinLuckyAtA3;							//<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  
	BYTE			m_iRobotWinLuckyAtA4;							//<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  
	//BYTE			m_iRobotNtCountLimit;							//�����������ׯ����

	int				m_iApplyNtLimite;								//��ׯ����
	int				m_iNtPlayLimite;								//ׯ����ׯ��������
	
	__int64			m_iAreaBetLimite;								//������ע����
	__int64			m_i64UserBetCount[PLAY_COUNT][BET_ARES];		//�������ÿ���������ע���
	__int64			m_i64AreaBetAllSum[BET_ARES];					//�����������ע
	__int64			m_i64UserBetSum[PLAY_COUNT];					//����ҵ�����ע
	__int64			m_i64SumAllBet;									//����ע��

	__int64         m_iRobotWinMoneyA1;								//<������ӮǮ����1  
	__int64         m_iRobotWinMoneyA2;								//<������ӮǮ����2  
	__int64         m_iRobotWinMoneyA3;								//<������ӮǮ����3  
	__int64         m_iRobotAddMoneyEveryDay;						//<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  
	__int64         m_iResetRobotHaveWinMoney;						//<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  
	__int64			m_iRobotMaxWinInOne;							//������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�
private:
	bool			m_bGameIsRun;									//��Ϸ�Ƿ��Ѿ�����
	bool			m_bGameIsPlaying;								//��Ϸ�Ƿ��Ѿ���ʼ��
	bool			m_bIsCanBet;									//�Ƿ������ע
	bool			m_bIsSuperControl;								//�Ƿ��ǳ����û����ƽ��
	BOOL			m_bIsRobotWinAndLostAutoCtrl;					//�Ƿ�����������Ӯ�Զ�����

	__int64			i64NtGrade;										//ׯ�ҳɼ�

	__int64			i64PlayerGrade[PLAY_COUNT];						//�мҳɼ�
	__int64			i64PlayCoin[PLAY_COUNT];						//�����Ӯ���
	__int64			i64ChangeMoney[PLAY_COUNT];						//��ҵ÷�
	
	__int64			i64ReturnMoney[PLAY_COUNT];						//�����Ľ����
	
	__int64			m_i64NtHaveWin;									//ׯ����Ӯ���
	
	BYTE			m_byNtStation;									//ׯ��λ��
	BYTE			m_byCurrResult;									//���ֿ������
	BYTE			m_byLastResult;									//�Ͼֿ������
	//�����û���Ϣ
	BYTE			m_bySuperReslut;								//�����û����ƵĽ��
	BYTE			iCardArray[52];									//Ҫ������
	BYTE			m_sendCardCount;								//��������
	BYTE			m_byRemaindTime;								//�����ʣ��ʱ��
	BYTE			SendCardCount;									//�����Ƶ�����
	BYTE            m_bRobotNtCount;								//Ŀǰ��������ׯ����

	vector <long>	m_vlSuperID;									//���泬����ҵ�����
	list<BYTE>      m_blNtWaitList;									//��ׯ�б�
	deque <BYTE>	m_dbyLuzi;										//·��˫�˶���
	CTime			m_RecordTime;									//��ǰ��Ϸ����

	int				m_iClock;										//ʱ��
	int				m_iNtPlayCount;									//ׯ����ׯ����
	
	//��������ע��Ϣ
	__int64         m_RobotAreaBetSum[PLAY_COUNT][BET_ARES];		//��������ÿ���������ע��
	__int64			m_RobotAreaAllSum[BET_ARES];					//ÿ�������������ע����
	__int64			m_RobotBetSum[PLAY_COUNT];						//ÿ�������˵���ע����
	__int64			m_RobotAllSum;									//��������ע����
protected:
	GameMutipleStruct       m_GameMutiple ;							//��Ϸ�еı���
	//��������
public:
	//���캯��
	CServerGameDesk(); //����������ʱ��һ�ε���
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:

	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	
    ///���ش�����Ϣ���ͷ�������ʱ�ڶ��ε��ã�
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	
	//����ȡ���������й�
	virtual BOOL SetAIMachine(BYTE bDeskStation,BOOL bState = FALSE);


	//��Ϸ����(�ͷ����˳�ʱ����)
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGameEx(BYTE bDeskStation);

	//�ж��Ƿ�������Ϸ���ͷ����˳�ʱ�ڶ��ε��ã�
	virtual bool IsPlayGame(BYTE bDeskStation);

	//�û��뿪��Ϸ��(�ͷ����˳�ʱ��һ�ε���)
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//���ش��麯��
public:

	//��ȡ��Ϸ״̬��Ϣ���ͷ�������ʱ�����ε��ã�
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//������Ϸ״̬���ͷ����˳�ʱ���ã�
	virtual bool ReSetGameState(BYTE bLastStation);

	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);

	//����INI�ļ�������������ʱ�ڶ��ε��ã�
	virtual bool InitDeskGameStation();
	//������
public:

	//������Զ���ʼ���ɷ���˿���
	BOOL StartGame();

	//��ʼ������
	void InitGameData();

	//��Ϸ׼������
	void GamePrepare();

	//��ȡ��̬����
	BOOL LoadIni();

	//��Ϸ��ʼ
	BOOL BeginPlay();

	 //����
	void  SendCard();     

	//������Ӌ�r��
	void KillAllTimer();

	//������������
	void ResetGameData();

	//һ�ν�������ȫ������
	BOOL SendAllCard();

	//�l�ͽY��
	BOOL SendCardFinish();

	//���ͽ�����Ϣ
	void sendResult();

private:
	//��ͷ��˷�����ע�׶���Ϣ
	void sendBetMsg();

	//��ȡ���ʣ�����Ǯ
	__int64	GetUserRemaindMoney(BYTE byDeskStation);

	//��ȡ�����ע�ܶ�
	__int64	GetUserBetCount(BYTE byDeskStation);

	//�������������ׯ��Ϣ
	void DealUserApplyNt(void *pBuffer);

	//�������������ׯ��Ϣ
	void DealUserApplyLeaveNt(void *pBuffer);

	//���������ע
	void DealUserBet(void *pBuffer);
	
	//�Ƿ�����ׯ�б���
	bool IsInWaitNtList(BYTE byDeskStation);

	//�ж���Ӯ
	BYTE DealWinner();

	//������ʾ�����Ϣ
	void sendShowResult();

	//��������˶�ʱ��
	void launchTimer();

	//���˿�ת�������ִ�С
	void trandformNum(BYTE card[]);

	//ͳ����ҽ��
	BOOL accountPlayCoin(BYTE whoWin,__int64 i64PlayCoin[]);

	//�õ�����ʣ����ע��
	__int64 RemainingBettingNumber(BYTE);

	//�����û���֤
	void SuperUserExamine(BYTE bDeskStation);

	//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
	bool IsSuperUser(BYTE byDeskStation);

	//�������û����ƽ��
	void DealSuperUserMessage(void *pBuffer);

	//��Ϸ����
	void DoGameResult(__int64 i64TurePoint[]);

	//�����˿�
	void ExchangePoker(BYTE card[],int para);

	//�������Զ�����
	BYTE RobotAutoControl();

	//�������Զ�����ѡ����ʵ��н�����
	BYTE RobotAutoSelectWinArea();

	//��ʱ���㿪�����������˺���Ҹ������ܵ÷�
	__int64 TempAccountRobotAndRealPlayer(BYTE byWinArea,BYTE para);

	//������Ϣ
	void PrintLog();
	///�жϴ���Ϸ���Ƿ�ʼ��Ϸ
	//virtual BOOL IsPlayingByGameStation();
	/*����false������UserNetCut
	����true������CanNetCut
	*/
	//��Ҷ��ߴ����ͷ����˳�ʱ���ã�
	//virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	//�����ı��û����������ĺ���
	//virtual bool CanNetCut(BYTE bDeskStation);//JAdd-20090116
};

/******************************************************************************************************/
