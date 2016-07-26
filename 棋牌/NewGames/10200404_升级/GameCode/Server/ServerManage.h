#pragma once

#include "GameDesk.h"
#include "../GameMessage/UpGradeLogic.h"

#include <afxmt.h>
//��ʱ�� ID
#define TIMER_SENDBACKCARD			32				//���͵���
#define TIMER_ROUNDBOSSOUTCARD		33				//��Ȧ����������ҳ��ƶ�ʱ��
#define TIMER_GAMEOVER				34				//��Ϸ����
#define TIMER_SENDOTHERCARD			35				//����������������ݸ�����
#define TIMER_SENDCARD   			36				//���ƶ�ʱ��
#define TIMER_THINK   			    37				//˼����ʱ��
#define TIMER_BACK   			    38				//����ƶ�ʱ��


//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SAFE						11				//��Ϸ��ȫ����
#define GF_TERMINATE				12				//����ֹͣ��Ϸ


#define SingleDeckCard				54				//һ��������
#define NumsOfDeckCard				2				//������
const BYTE sByFaceDeskStation[PLAY_COUNT]={2,3,0,1};//�Լ�λ��
//ÿһ��������
struct TTurnCardData
{
	int iCardNums[PLAY_COUNT];		//������
	BYTE byCards[PLAY_COUNT][25];		//������
	TTurnCardData()
	{
		memset(this,0,sizeof(TTurnCardData));
	}
};
//*****************************************************************************************
//��Ϸ����

class CServerGameDesk : public CGameDesk
{
	//��̬����
private:
	
	CMutex  m_Mutex;

	vector<BYTE>			m_vbyAllCards;		//������
	CUpGradeGameLogic		m_Logic;			//


protected:
	bool					m_bStartFlag;//������Ϸ��־
	bool					m_bQiangNt;//�Ƿ������ׯ
	//���׼��״̬
	bool                    m_bUserReady[PLAY_COUNT];		//���׼��״̬
	vector <long>			m_vlSuperID;					//���泬����ҵ�����
	bool					m_bUserNetCut[PLAY_COUNT];		//��ʶ����Ƿ����
	//��Ϸ��Ϣ
	BYTE					m_byGroupOneevel;				//��һ����ҵĵ�ǰ���� -- ׯ��
	BYTE					m_byGoupTwolevel;				//�ڶ�����ҵĵ�ǰ���� -- �м�
	bool					m_bIsCalled;					//�Ƿ��Ѿ����˽���
	BYTE					m_byRoundOutCardCount;			//ÿȦ��������
	BYTE					m_byFirstOutCardDeskStation;	//ÿȦ�׼ҵ���λ��
	TFirstData				m_tRoundCard[PLAY_COUNT];		//ÿȦÿ����ҵĳ�������
	vector<BYTE>			m_vPlayerCards[PLAY_COUNT];		//��ǰÿ��������е�������
	T_CS_OutCard			m_tRoundFirstCard;				//��Ȧ�е�һ����ҳ�����
	TTurnCardData           m_tTurnCardData[30];			//ÿһ��������
	int						m_iTurnNums;					//��¼����
	TCallLevelCard			m_tCurrentCall;					//��ǰ����
	BYTE					m_byBackCard[8];				//����
	int						m_iNoMoreCardCount;				//û���Ƶ�����

	BYTE					m_byNextOutCardPlayer;			//�¸����Ƶ������λ��
	int						m_iRoundScore;					//��Ȧ�÷�
	int						m_iTotalScore;					//�ܷ�
	int                     m_iBackCardScore;				//���Ʒ���
	__int64					m_i64Score[PLAY_COUNT];			//������					
	//��Ϸ������Ϣ
	int						m_iTimeToReady;					//׼��ʱ��
	int						m_iTimeToCallNT;				//����ʱ��
	int						m_iTimeToThink;					//˼��ʱ��
	int						m_iSendCardSpeed;				//�����ٶ�
	//����
	vector<long>			m_vSuperUsers;						//�����û�
	bool					m_bySuperDeskStation[PLAY_COUNT];	//�����û���λ��

	//��������
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGameEx(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	///�û������뿪
	bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
    ///���ش�����Ϣ
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�����ı��û����������ĺ���
	virtual bool CanNetCut(BYTE bDeskStation);//JAdd-20090116
	//��ȡ��̬����
	BOOL LoadIni();	
	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//����INI�ļ�
	virtual bool InitDeskGameStation();
	//����׼����Ϣ
	bool OnHandleAgree(BYTE byDeskStation,void *pBuffer,int iSize);
	//�����������Ϣ
	bool OnHandleBackCard(BYTE byDeskStation,void *pBuffer,int iSize);
	//���������Ϣ
	bool OnHandleOutCard(BYTE byDeskStation,void *pBuffer,int iSize);
	//�������
	void SendCardFinish();
	//�������
	bool OnHandleLookCard(BYTE byDeskStation,void *pBuffer,int iSize);
	
	//����
	void SendCards();
	//����ϴ����Ϣ
	void SendWashCardMsg();
	//������Ϸ����
	void ResetGameLevel();
	//������Ϸ������Ϣ
	void SendGameLevelMsg();
	// for debug
	void ForDebugging(TSendCards& tSendCards);
	//��������Ƿ�׼����Ϣ
	void SendPlayerReadyMsg(BYTE byDeskStation=255);
	//����
	void CallGameLevelCard(TCallLevelCard *ptCallLevelCard,BYTE byDeskStation);
	//����ҷ�����������
	void SendCallGameLevelCard(TCallLevelCard *ptCallLevelCard);
	//����������ҵ��Ƹ�����
	void SendAllCardsToSuper();
	//��¼ÿ����ҵ�������
	void RecordTurnCardData(BYTE byDeskStation,BYTE bCardList[],int iCardCount);
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
	//��־��¼
	void Debuglog(const char *p, ...);
};

/******************************************************************************************************/
