#pragma once

#include "StdAfx.h"
#include"..\GameMessage\UpgradeLogic.h"

//��ʱ�� ID
#define TIME_WAIT_THINK				30				//�ȴ�������Ϸ״̬
#define TIME_THINK					31				//˼����ʱ��


//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����
#define SH_USER_CARD                5               //�Ƶ�����

static __int64					m_iAIHaveWinMoney;			//������Ӯ��Ǯ	20121104dwj	����Ϊ��̬ȫ�ֱ�������Ϊ�ö������ͬʱ��������ʱҪ�ı����ֵ;	
static __int64					m_iReSetAIHaveWinMoney;		/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ ����Ϊȫ�ֱ�������ֻ֤����һ�� */
static int						m_iRecFroDayTime;			/**<��¼��������ǰʱ�䣬�����ж��Ƿ����24�㣬����24�����A1��A2��A3�����ϵ���m_iAddMoneyEveryDay  */

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
protected:
	CUpGradeGameLogic		m_Logic;							//��Ϸ�߼�

	bool					m_bWatchOther[PLAY_COUNT];			/**<�Ƿ�����Թ�*/
	bool					m_bIsPrepare[PLAY_COUNT];		//�Ƿ�׼��
	//��������
	int						m_iUCCount[PLAY_COUNT];					//�û������˿���Ŀ

	BYTE					m_byUCard[PLAY_COUNT][USER_HAND_CARD];	//�û����ϵ��˿�

	BYTE					m_iTotalCard[ALL_CARD_COUNT];			//�ܵ���

	BYTE                    m_bBullCard[PLAY_COUNT][USER_HAND_CARD];//��Ұڵ�ţ��

	int						m_byCardShape[PLAY_COUNT];				//�������

	bool					m_bHaveBaiPai[PLAY_COUNT];				//�Ƿ��Ѿ����� false:û�� true:�Ѿ�����

	bool					m_bIsInGame[PLAY_COUNT];				//����Ƿ�����Ϸ��

	bool                    m_bIsCut[PLAY_COUNT];				//�Ƿ��������ҪΪ����������

	BOOL                    m_bIsAuto[PLAY_COUNT];				//�Ƿ��й�

	bool                    m_bIsSuper[PLAY_COUNT];				//�Ƿ����ʹ�÷���

	int						m_iSurplusTime;						//ʣ���ʱ��

	BYTE                    m_bControlWin;						//������Ӯλ��

	BYTE                    m_bControlLost;						//������Ӯλ��

	BYTE					  m_byCowUserNum;							//ţţ�������
	bool						m_bIsCowUser[PLAY_COUNT];				//�Ƿ�ţţ���
	//--------------add by DWJ--20121203----------------------------------
	BOOL					m_bAIWinAndLostAutoCtrl;	//�����˿�����Ӯ	20121122	dwj	;
	bool					m_bIsFirstLoadIni;			//�Ƿ��һ�ζ�ȡ�����ļ� ���ڱ����һ���������ڣ����û����һ��m_iAIWantWinMoneyA1��A3����Ҫ����;
	__int64					m_iAIWantWinMoneyA1;		/**<��������Ӯ���ƣ�������ӮǮ����1  */
	__int64					m_iAIWantWinMoneyA2;		/**<��������Ӯ���ƣ�������ӮǮ����2  */
	__int64					m_iAIWantWinMoneyA3;		/**<��������Ӯ���ƣ�������ӮǮ����3  */
	int						m_iAIWinLuckyAtA1;			/**<��������Ӯ���ƣ�������������1ӮǮ�ĸ���  */
	int						m_iAIWinLuckyAtA2;			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	int						m_iAIWinLuckyAtA3;			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	int						m_iAIWinLuckyAtA4;			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	__int64					m_iAddMoneyEveryDay;		/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	//__int64					m_iReSetAIHaveWinMoney;		/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ  */
	//__int64					m_iAIHaveWinMoney;			//������Ӯ��Ǯ			
	//int						m_iRecFroDayTime;			/**<��¼��������ǰʱ�䣬�����ж��Ƿ����24�㣬����24�����A1��A2��A3�����ϵ���m_iAddMoneyEveryDay  */
	BYTE					m_bCardShapeBase[MAX_SHAPE_COUNT];//0-10������ţ��ţţ��ı���
	bool					m_bWriteLog;							/**< �Ƿ��ӡ��־ */

private:
	int						m_iBaseNote;						//��ע

	BYTE					m_byBTime;							//��Ϸ��ʼʱ��

	BYTE					m_bySCTime;							//��Ϸ����ʱ��

	BYTE					m_byTTime;							//��Ϸ˼��ʱ��

	BYTE					m_byRTime;							//�����ȴ�ʱ��

	BYTE					m_iAllCardCount;					//��Ϸ���õ�����

	DWORD					m_iCardShape;						//����

	int						m_iGoodCard;						//�����ƻ���ţ������

	bool					m_bHaveKing;						//�Ƿ�����

	//���˱���
	bool					m_bSuperSet;						//���������Ƿ�ɹ�
	int						m_iSuperSet[2];						//���趨�������Ӯ����
	/*
	0Ԫ�أ�����߼�λ�ã�255Ϊ��ʼֵ��
	1Ԫ�أ������Ӯ������(0Ϊ��ʼֵ��1ΪӮ��2Ϊ��)
	*/

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
	///��ʼ������
	void InitGameData();

	//��ȡ��̬����
	BOOL LoadIni();	

	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);
	//-------ADD-BY-DWJ-20121203--------------------------------
	//����Ƿ��Ѿ��µ�һ���ˣ���Ҫ�����趨�����˿������ò���
	void CheckNewDayReSetIni(CINIFile *f, CString *key);

	//20121203dwj ��¼��������Ӯֵ
	void RecordAiHaveWinMoney(__int64 iTempPoint[PLAY_COUNT]);

	/**�������ļ��ж�ȡ�����˿������õĲ���ֵ*/
	void GetAIContrlSetFromIni(int iRoomID);

	//�����˿˸��û�
	BOOL SendCard();

	//�������̯�ƶ���
	void HandTanPaiAction(BYTE byDStation,BYTE byDownCard[]);	

	//�Զ�̯��
	void AutoTanPai(BYTE byDStation);	

	//���̯���Ƿ����
	BOOL CheckTanPaiFinish();

	///���ʤ�����
	BYTE GetWinPeo();

	///����������
	void CountResultScore(BYTE bDeskStation,GameEndStruct* pGameEnd,BYTE bCloseFlag);

	//ͳ����Ϸ����
	BYTE CountPlayer();

	///�����й���Ϣ
	BOOL UserAuto(BYTE bDeskStation,AutoCardStruct *bAuto);

	//��������Ϣ
	BOOL HandleSuperInfo(BYTE bDeskStation,SuperStruct *pSuper);

	//��ȡ��������
	BOOL SuperLoad();

	//���ݳ���������
	void SuperSet();

	///���㵱ǰ��������ָ����ҵ���ӮǮ
	int CountSuperWinMoney();

	//������Ƿ���ĳ���Ƿ���Q�ɹ�
	BOOL ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard);

	//���汶��
	int GetDeskBasePoint();

	//���䱶��
	int GetRoomBasePoint();

	//���ܿ۷�
	int GetRunPublish();

	//�õ�����ƺ���С��,winֵΪ1ʱ�õ����,����Ϊ��С
	//BYTE GetIndex(int win);

	//�ж�ʤ��
	//BOOL JudgeWiner();

	//�ж����
	//BOOL JudgeLoser();

	//����
	BOOL ChangeCard(BYTE bDestStation,BYTE bWinStation);

	///20121203dwj��������Ӯ�Զ�����,
	void IAWinAutoCtrl();
	///20121203dwj���㵱ǰ�ƻ����˵���ӮǮ
	int CountAIWinMoney();

	//����ǿ�����
	void ClearQuitPlayer();

	/**
	* ������Ϸ ��ʱ�� ����Ӧ���� ����OnTimer
	*
	* @param [in] UINT uTimerID ��ʱ��ID
	*
	* @param [in] int uElapse ���ʱ��
	* 
	@param [in] bool bRecTime �Ƿ��¼ʣ��ʱ��
	* 
	* @return void 
	*/
	void SetGameTimer(UINT uTimerID, int uElapse,bool bRecTime);

	///�������м�ʱ��
	void KillAllTimer();

	///��ȡ��ǰ�����������ͣ�
	int GetMaxCardShape(BYTE iCard[], int iCardCount);

	void GetIniData(IniData &sData);	//��ȡ��ȡINI�ļ�����

	void GetBaseData(BData &sData, BYTE bDeskStation);		//��ȡ��Ϸ��������

	/**
	* ��ӡ��־
	*
	*/
	void DebugPrintf(const char *p, ...);
};
/******************************************************************************************************/
