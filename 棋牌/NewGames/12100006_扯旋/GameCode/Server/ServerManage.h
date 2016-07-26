#pragma once

#include "../../../../sdk/include/ServerCommon/GameDesk.h"
#include "../GameMessage/UpGradeLogic.h"

// ����������̫��Ķ�ʱ������Ӱ�����ܣ��ʴ������Ż���ֻ��һ����ʱ��������ʹ�õ��Ķ�ʱ�����������ʱ�����������ڴ����м����ж�

// ��ʱ�����ýṹ
struct TimerStruct 
{
	UINT nId;			//��ʱ����־��
	int nTickCount;		//�Ѿ������˴�
	int nTriggerCount;	// �������ٴκ󴥷��¼�
	TimerStruct(UINT nTimerID, int uElapse) : nId(nTimerID),nTickCount(0), nTriggerCount(uElapse)
	{
	}
	TimerStruct(): nId(0),nTickCount(0), nTriggerCount(0){}
};


//��ʱ�� ID
#define TIME_MY_TIMER				20				//���ö�ʱ����ֻ�������ʱ����ʵ�ʶ�ʱ�������඼�Ǵ������ʱ�������ж�����
#define MY_ELAPSE					250				// ��ʱ�����
#define TIMER_COUNT					11				// �õ��Ķ�ʱ����Ŀ

// ����Ϊ��Ϸ��ʹ�õ��Ķ�ʱ�����������TIME_MY_TIMER��������ͳһ����

#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIMER_SEND_END              31				//���ƽ���
#define TIMER_WAIT_NOTE             32				//�ȴ���ע 
#define TIME_SEND_ONE_CARD			33				//��һ����
#define TIME_SEPARATE			    34				//����ʱ��
#define TIME_GAME_FINISH			35				//��Ϸ������ʱ��
#define TIME_CUT_AI                 36              //�йܶ�ʱ��
#define	TIME_BOBO_RESET_TIME		37				//��ʱ���������µ�ʱ��

#define DEFAULT_WAIT_FREQUENCE      1000            //һ���м��һ��

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����
#define GF_EARLY_END				12				//��ǰ����
#define  TIMER_CUT_AI_LONG                  1000


// ��������ؼ����ͽ�������
class BZWLockForCriticalSection
{
	CRITICAL_SECTION *m_pSection;
	BZWLockForCriticalSection();
public:
	BZWLockForCriticalSection(CRITICAL_SECTION *pSection)
	{
		m_pSection = pSection;
		EnterCriticalSection(m_pSection);
	}
	~BZWLockForCriticalSection()
	{
		LeaveCriticalSection(m_pSection);
	}
};


//��Ϸ����
class CServerGameDesk : public CGameDesk
{
private:
	int						   m_iTimerCount;      //��¼���ʱ��
	__int64					   m_i64BoBase;					//����
	__int64					   m_i64SmallBobo;				//��С����
	//------------------------------���й���صĹ���------------------
	bool m_arybCutUser[PLAY_COUNT];     //����Ƿ����
protected:
	enum 
	{
		FINISH_TYPE_UNFINISH = 0,     //û�н���		
		FINISH_TYPE_FISH = 1,         //��������      
		FINISH_TYPE_REACH_LIMITE_SELF = 2,  //�ﵽ���ޣ�����
		FINISH_TYPE_REACH_LIMITE_NEXT = 3   //��һ�Ҹ�ע�ͻᳬ������
	};
	TimerStruct		m_arTimer[TIMER_COUNT];
	CRITICAL_SECTION m_csForIPCMessage;
	//����״̬����
	BYTE					m_iNtPeople;                         //Ӯ��
	int						m_iUpGradePeople;					//ׯ��λ��
	BYTE					m_iHaveThingPeople;					//����Ҫ�����
	BYTE					m_iAllCardCount;					//��Ϸ���õ�����
	BYTE					m_iSendCardTime;					//��Ϸ˼��ʱ��
	BYTE					m_iSendCardRes;						//�������µ�ʱ��
	BYTE					m_iThinkTime;						//��Ϸ˼��ʱ��
	BYTE					m_iBeginTime;						//��Ϸ��ʼʱ��
	BYTE					m_bySeparateCardTime;				//����ʱ��
	//״̬��Ϣ
	int						m_iUserCardShap[PLAY_COUNT][3];				//��ҵ�������Ϣ0:���,1:�ұ� ����������Ϊ aabbcc;aa:����,bb:��,cc:�������ֵ��2:�������	
	BYTE					m_bySendCardCount[PLAY_COUNT];				///�Ѿ����Ƶ�����
	BYTE					m_iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE					m_iUserCard[PLAY_COUNT][MAX_CARD_COUNT];	//�û����ϵ��˿�
	BYTE					m_byCardShap[PLAY_COUNT][2];				//���� 0:����������� 1:�����ұ�����
	bool					m_bSeparatCardFale[PLAY_COUNT];				//����Ƿ��Ѿ�������
	//������Ϣ
	int						m_iOutCardPeople;							//���ڳ����û�
	int						m_iFirstOutPeople;							//�ȳ��Ƶ��û�
	bool					m_bSetAllFlag;						//�ñ�־λ
	bool					m_bFollowFlag;						//����־λ
	//��������
	BYTE					m_iTotalCard[32];					//�ܵ���
	BYTE					m_iSendCardPos;						//����λ��
	CUpGradeGameLogic		m_Logic;     						//��Ϸ�߼�
	int						m_iLastOutPeople;
	int						m_iUserStation[PLAY_COUNT];			//��¼�û�״̬
	__int64					m_iPerJuTotalNote[PLAY_COUNT];      //�û�����ע	
	__int64					m_i64WinMoney[PLAY_COUNT];			//��Ӯ�õĽ��(������)
	__int64					m_i64NetIncome[PLAY_COUNT];			//������
	__int64					m_iTotalNote;					    //��ע
	__int64                 m_iFrontNote;                       //ǰ��Ѻע
	__int64					m_i64SaveFront;						//����ǰ���µ�ע
	__int64					m_i64Bobo[PLAY_COUNT];				//ÿ����ҵ�������
	__int64					m_iNowNote[PLAY_COUNT];			    //��ǰѺע��
	__int64					m_i64NoteMoney[PLAY_COUNT][3];		//���ÿ�����µĽ��ֵ

	BYTE					m_byGameStation;					//��Ϸ״̬()
	BYTE					m_byNoteTime[PLAY_COUNT];			//��ע�Ĵ���
	BYTE					m_byNoteFlag[PLAY_COUNT];			//��һ����ע
	BYTE					m_byOperationState[PLAY_COUNT];		//��ҵĲ�����¼		
	int						m_iAlreadySendCard;					//�Ѿ����Ƶ�����
	BYTE					m_iSendPos;							//��һ�ַ��ƿ�ʼλ��
	///���׼��״̬
	bool                    m_bUserReady[PLAY_COUNT] ;				//�û�ͬ���־λ
	bool					m_bAgree[PLAY_COUNT];					//��ʼ��Ϸ��־
	bool					m_bAlreadSetBobo[PLAY_COUNT];			//�������������

	bool					m_bSpecialCardGiveUp[PLAY_COUNT];		//�����Ƶ����(����ʮ��������)
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();
//���غ���
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//�û�����
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	virtual bool InitDeskGameStation();
//���ش��麯��
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//������
public:
	void InitAIData();
	virtual bool OnBeginUserNetCutAI(BYTE bDeskStation);  //���ش����йܺ���
	void ExcutNetCutAI();  //ִ���йܲ���
	//�����ж�
	void SuperUserIDJudge();
	// ɱ����ʱ����־
	void KillMyTimer(UINT nTimeID);
	int SetMyTimer(UINT nTimeID, int uElapse);
	bool OnMyTimer(UINT uTimerID);
	//��ȡ��̬����
	BOOL LoadIni();	//׼�����ƹ���
	//������ע
	BOOL LoadExtIni();
	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);
	//�û�������Ϸ
	BOOL UserSetGame(UserArgeeGame * pGameSet);
	//�����˿˸��û�
	BOOL SendCard();
	//���ƽ���
	BOOL SendCardFinish();
	//��Ϸ��ʼ
	BOOL BeginPlayUpGrade();
	//�����û����
	BOOL UserActiveResult(BYTE bDeskStation, BYTE iVerbType,__int64 iNoteVal=0,BYTE bComparedStation=255);
	//����λ��
	BOOL CanSendCard(BYTE bDeskStation);
	//ͳ����Ϸ����
	BYTE CountPlayer();
	//�Q��
	BOOL ChangeBackCard(BYTE bDeskStation,BYTE bSourceCard,BYTE bResultCard);
	//������Ƿ���ĳ���Ƿ���Q�ɹ�
	BOOL ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard);
	//���ܿ۷�
	int GetRunPublish();
	//�õ���һ��������ҵ�λ��
	BYTE GetNextNoteStation(BYTE bCurDeskStation);
	//----������Ӯ--------
	BOOL ChangeCard(BYTE bDestStation,BYTE bWinStation);						//����	
	bool SetUserCard();			// ��ȡ�����ļ������ڵ��Է���	Add by zxd 20090803
	//ƽ̨���ع��ܾ���ʵ����Ҫ��Ϸ����
	//�޸Ľ�������1(ƽ̨->��Ϸ)
	//@param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
	//@return �Ƿ�ɹ�
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);
	//�޸Ľ�������2(ƽ̨->��Ϸ)
	//@param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ�
	//@param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ���
	//@return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);
	//�޸Ľ�������3(ƽ̨->��Ϸ)
	//@param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
	//@return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);
	//����ʤ��
	//�ж�
	virtual BOOL Judge();
	//ʤ
	virtual BOOL JudgeWiner();
	//��
	virtual BOOL JudgeLoser();

	bool Ass_Set_Bobo_Data(void * pData, UINT uSize);						//������յ���������
	void SendOneCardFinsh();												//��һ�������
	void SeparateCardFinsh();												//����ʱ�䵽(��ʱ��)
	bool Ass_Cs_Separat_Card(void * pData, UINT uSize);
	void FinshCountScore();													//�������
	int	 CmpTwoValue(int iFirstList[],int iSecondList[]);					//�������͵Ĵ�С
	bool CheckGameFinish();													//�����Ϸ�Ƿ������

	void SpecialCardGive(int iDesk);										//�Ѿ���������֮��,��������ʮ��������֮������
};

/******************************************************************************************************/
