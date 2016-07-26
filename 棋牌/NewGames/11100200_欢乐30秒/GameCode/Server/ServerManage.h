/// ServerManage.h: Headers and defines for CServerGameDesk
///


#if !defined(_SERVERMANAGE_H__INCLUDED_)
#define _SERVERMANAGE_H__INCLUDED_

#include "StdAfx.h"
#include "../GameMessage/UpGradeLogic.h"

///��Ϸ������־����
#define GF_NORMAL					10			   /**<  ��Ϸ��������   */
#define GF_SAFE						11			   /**<  ��Ϸ��ȫ����   */

#define ID_GAME_TIMER_FALG_START    0x10           /**<   �ܻ�ȡ���ʣ��ʱ��Ķ�ʱ����־   */


#define ID_TIMER_SEND_CARD          0x10           /**<  ��ȡʣ����ٵĶ�ʱ����0x10~0x20��Χ�ڣ�ֻ��Ҫ�������һ���ֽ�   */
#define ID_TIMER_OUT_CARD           0x11           /**<  ���ƶ�ʱ��ID   */
#define ID_TIMER_GAME				0x12		   /**<  ��Ϸʱ��   */
#define ID_TIMER_BEGIN_TIME			0x13		   /**<  ��Ϸ��ʼ��ʱ��   */
#define ID_GAME_TIMER_FALG_END      0x20           /**< �ܻ�ȡ���ʣ��ʱ��Ķ�ʱ����־   */

#define ID_TIMER_AI_APPLY_NT     0x21           /**<  ������������ׯ��ʱ�� ����������ׯ���ƣ�  */
#define ID_TIMER_XIA_ZHU       0x22           /**<  ��ע   */
#define ID_TIMER_KAI_PAI       0x23           /**<  ����   */
#define ID_TIMER_FREE_TIME     0x24           /**<  ����ʱ��   */
#define ID_TIMER_GAME_TIME     37			  //��עʱ��
#define ID_TIMER_GAME_SENDCARD 38			  //����ʱ��   
#define ID_TIMER_GAME_FREE     39			  //����ʱ��   

#define ID_TIMER_GAME_NEW_GAME 0x28			  /**<  ��һ��   */
#define ID_TIMER_GAME_SHOW_JS  0x29			  /**<  ��ʾ�����ʱ��   */
	 

///�ټ�����ע�����嶨��
#define GF_ZHUANG				0			   /**<  ׯ   */
#define GF_Z_TIANWANG			1			   /**<  ׯ����   */
#define GF_Z_DUIZI				2			   /**<  ׯ����   */
#define GF_XIAN					3			   /**<  ��   */
#define GF_X_TIANWANG			4			   /**<  ������   */
#define GF_X_DUIZI				5			   /**<  �ж���   */
#define GF_HE					6			   /**<  ��   */
#define GF_T_HE					7			   /**<  ͬ���   */

///��Ϸ������������
class CServerGameDesk :public CGameDesk
{
public:
	CServerGameDesk(void);
public:
	~CServerGameDesk(void);

	/// ���������ļ�
	BOOL LoadIni();
	BOOL LoadExtIni();
	BOOL LoadExtIni(int iRoomID);

	/// ԭ�����������λ��
	void InitUserSeatID(void);

	/// ���ö�ʱ��
	bool SetGameTimer(BYTE byTimerID, int uElapse);

	/// ɾ����ʱ��
	bool KillGameTimer(BYTE byTimerID);

	/// ������ж�ʱ��
	void KillAllTimer(void);

	/// ������ҵ��߼�λ�û�ȡ��������λ��(������ƶ�λ��ʱ��Ҳ�һ���������߼�λ����)
	virtual BYTE GetSeatID(BYTE byDeskStation);

	/// ������ҵ�������λ�û�ȡ�߼�λ��(������ƶ�λ��ʱ��Ҳ�һ���������߼�λ����)
	virtual BYTE GetDeskStation(BYTE bySeatID);

	/// ��õ�ǰ�������е���Ϸ��ʱ����ID
	virtual BYTE GetGameTimerID();

	/// ��õ�ǰ�������е���Ϸ��ʱ����ʣ��ʱ��
	virtual int GetGameRemainTime();

	///��ʼ����Ϸ�߼�zht20100205
	virtual bool InitDeskGameStation();
	
// ����
public:
	///��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	///��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	///�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	///��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// �����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	///�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	///�û������뿪
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	///�û�������Ϸ��
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	///�Թ۲����仯
	virtual bool OnWatchSetChange(BYTE byDeskStation, long int dwTargetUserID, bool bEnableWatch);

	/// ��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);

	/// ��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE byTimerID);

	///���������е����ݣ������õ�����Ԫ�����
	void MoveData(int type);

	///������ҵ���ע
	void HanleXiaZhu(BYTE Station,void * pData, UINT uSize);
	///������ҵ���ׯ��Ϣ
	void HanleSzhuang(BYTE Station,void * pData, UINT uSize);

	///������ҵ���ׯ��Ϣ
	void HanleXzhuang(BYTE Station,void * pData, UINT uSize);

	///�������Ϸ���ݣ���Ҷ���������ᷢ������������Ϣ���������յ�����Ϣ��ͨ�����ӿڣ�
	void OnGetGameData(BYTE Station,void * pData, UINT uSize);
	///�����ͻ������û����˿��Ʋ���
	void SetAICtrl(BYTE Station,void * pData, UINT uSize);

	///��ӡ��������
	void PrintEnding();
	///�������
	void CountFen();
	///������ֵ
	void CountPaiZhi(int count);
	///���ƿ�ׯ����
	void ConTrolZX();
	///����
	void SendCard();

	///�м����˵�����������£�ׯ����Ҫ����lym0511
	///pai:�мҵĵ�������ֵ
	bool IsGetTheThird();

	///��ȡ��������
	bool GetTheThird();

	///��ȡǰ������
	void GetTwoCard();

	///��ȡӮǮ������
	void GetWin();

	///ͬ����
	///@param num,���ǰ������
	bool CheckTongDian(int num);

	///�����Ѽ�����Ƶ�����lym0511
	void ClearWinData(bool all);

	///�ı�Ӯ��
	///zhuang:trueΪ����ׯӮ��falseΪ������Ӯ
	void ChangeWinner(bool zhuang);

	///����ׯ�е�ǰ������lym0511
	void ChangeCard();

	///һ�ֽ��������˿���ʱ������Ƿ��ܹ�������ʼ��Ϸ
	bool CheckCanBegin();

	///ׯ����ׯ���ȡ��һ��ׯ���б��е�ׯ��
	bool GetNextNT();

	///ͨ��ȫ�����ֵķ�ʽ��ʼ��Ϸ
	bool StartGame();

	///��¼���ֵĳɼ�����
	void RecordData();

	///��ʼ����������������ym0514
	///��ʼ���󲿷����ݣ���Щ���ݲ��ܳ�ʼ���ģ���ǰׯ�ҽ����˼��֣�ǰ30�ֵ����ݣ�
	void ServerInit();

	///lym������Ϸ�ܷ�ʼ����������ֹ��ׯ�ļٿ�ʼ
	bool CanBeginGame();

	///vip���˫����ע��Ȩ���ж�
	BOOL CanDouble(BYTE bDeskStation);

	///��־��¼�ӿ�
	void  SrvTestlog(TCHAR *pMessage);

	///������Ӯ
	BOOL  m_bSuperSetting;       // ��ʶ�����Ƿ��п���
	BYTE  m_bSuperState;         // 0:����  1��ׯӮ 2:ׯ��

	// ������Ӯ����
	void OnSuperSetting(BYTE bDeskStation, BYTE bState);  

	// ��֤�����û�Ȩ��
	void AuthPermissions();   

	/// ��ȡ������
	int GetMeFanFen(int station);

	///����72�������ݣ���¼���õ������ݣ�
	void SetResultData(int num);
	///��ȡ������
	void GetResultData(int num);

	///��ȡ��Ǯ���͵�Ǯ��
	int inline GetMoneyFormType(int type,int power=1)
	{
		return m_iNoteMoney[type-1]/power;
	};

	///m_byQuYuJinBi
	BYTE inline GetWeiZhiRand(BYTE num)
	{
		return num/15+4;
	};

	///��������������Ӯ�Զ�����
	void IAWinAutoCtrl();
	//-------ADD-BY-DWJ-20121108--------------------------------
	void CheckNewDayReSetIni(CINIFile *f, CString *key);//����Ƿ��Ѿ��µ�һ���ˣ���Ҫ�����趨�����˿������ò���

	void GetAIContrlSetFromIni(int iRoomID);			/**�������ļ��ж�ȡ�����˿������õĲ���ֵ*/

	///���������㵱ǰ�ƻ����˵���ӮǮ
	int CountAIWinMoney();

	///��������ȡһ������������ׯ�����Ļ�����λ��
	int GetAnAIApplyStation();

	///��������һ�������˷ŵ�ׯ���б��У���������ׯ���ƣ�
	bool SetAIToApplyList(int station);
	//���Ϳ�����Ϣ
	void OnSendOpenCard();
	//���ͽ�����Ϣ
	void OnSendEnd();
	//���Ϳ�����Ϣ
	void OnSenFree();
	//��Ϸ���¿�ʼ
	void RestartGame();
	///��������ȡÿ�����������ע
	void GetQuYuCanNote();

public:		//��Ϸ������ע���������� ��� ���� �� ���� add xyh 2011-12-6
	///�ж���ע����ע�Ƿ�����
	bool JudgeXiaZhuIsFull(void);

	///������ע������Ϣ
	void SendXiaZhuFullNews(void);

	bool m_bNoteFullOpenCard;	//��ע�����Ƿ��� true:��ע�����̿��� false:��ע�������̿���

protected:
	/// ��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	///������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);

	/// �й���Ϣ��Ӧ����
	virtual void OnChangeTrustee(BYTE byDeskStation);

	/// �û������뿪
	virtual bool OnUserHaveThing(S_HAVE_THING &sHaveThing);

	/// ����Ƿ�ͬ���뿪����
	virtual bool OnAgreeUserLeave(S_AGREE_USER_LEAVE &sAgreeUserLeave);
	///ͳ�ƻ�������ׯ����
	int CountRobortNum();
	///�Ƿ��л�������ׯ���б���
	BOOL IsHaveUserInList();
	//����
	void MatchCard(WORD (*wCardList)[3]);
	//����ׯ��
	void ClearNt();
protected:
	CPokerGameLogic m_PokerGameLogic;  /**< ������Ϸ�߼�*/
	GAME_BASE_INFO m_GameBaseInfo;     /**< ��Ϸ�Ļ�����Ϣ*/
	
	BYTE m_abySeatID[PLAY_COUNT];      /**< �������λ��*/
	BYTE m_bySendStartPos;             /**< ��һ�����Ƶ����*/
	BYTE m_byFirstOutPos;              /**< ��һ�������*/
	WORD m_wMingCard;                  /**< һ������*/
	bool m_bShuangKou;                 /**< �Ͼ��Ƿ�˫�۳ɹ� Ĭ���ǳɹ���   */
	//������ר��
	int m_iIsMatch;	//�Ƿ�����
	BYTE m_byLeaveAgree;				   /**< �뿪ͬ���־*/
	BYTE m_byUserAgreed;				   /**< �뿪ͬ���־(�Ƿ��Ѿ�ͬ�����)*/
	BYTE m_byHaveThingPeople;		       /**< ����Ҫ�����λ��*/

	// ״̬��Ϣ (���ݰ����߼�λ�ñ���)
	BYTE  m_byBaseOutCount;					/**< ���Ƶ���Ŀ*/
	WORD  m_wBaseOutCard[45];				/**< ����*/
	BYTE  m_byDeskCardCount[PLAY_COUNT];	/**< �����˿˵���Ŀ*/
	WORD  m_wDeskCard[PLAY_COUNT][45];		/**< ������˿�*/
	BYTE  m_byUserCardCount[PLAY_COUNT];	/**< �û������˿���Ŀ*/
	WORD  m_wUserCard[PLAY_COUNT][45];		/**< �û����ϵ��˿�*/
	BYTE  m_bySequence[PLAY_COUNT];			/**< ������˳��*/
	bool  m_bIsTrustee[PLAY_COUNT];         /**< �й�*/

	BYTE  m_byLastGameTimer;                /**< ���һ����Ϸ��ʱ��ID  (0x10 ~ 0x20)*/
	DWORD m_dwLastGameStartTime;            /**< ���һ����Ϸ��ʱ������ʱ��*/
	int   m_iLastTimerTime;                 /**< ���һ����Ϸ��ʱ��������ʱ��(��ʱ��)*/

	//���ѱ�������
	__int64  m_iZhongZhu;   					/**< ���ѵ�ǰ��ע��*/
	__int64  m_iQuYuZhu[8]; 					/**< ����ÿ�������µ�ע��*/
	__int64  m_iMaxZhu[8];  					/**< ����ÿ������������µ�ע��*/
	__int64  m_iMax[8];							/**< ����ÿ��������Ƶ������ע��*/
	__int64  m_iPeiLv[8];   					/**< ÿ����������ʣ������ѿ��ƵĽ���㣬Ӯ����Ϊ����������Ϊ����*/
	__int64  m_iZhuangFen;  					/**< ׯ�ҵĵ÷֣�����*/
	__int64  m_iXianFen;						/**< �мҵĵ÷֣�����*/
	__int64  m_iUserXiaZhuData[PLAY_COUNT][11]; /**< �����Ϣ0-7��ҵ���ע��8����µ���ע�9��ҵ÷֣�10��ҵ������ʣ�*/
	int  m_iQuYuJinBi[8][6];				/**< ÿ�����¸��ࣨ��6�ࣩ��ҵĸ���*/
	int  m_iCurrentMessage;					/**< ��ǰ�������Ϣ*/
	__int64  m_iAIMoney[8];						/**< �����˵���ע*/
	__int64  m_iNoteMoney[6];					/**< �������Ͷ�Ӧ�Ľ����*/
	int  m_nPowerMoney;					    /**<�Ŵ���*/
	bool m_bIsJieSuan;						/**<�Ƿ��Ѿ�����2010-05-21*/

	//�������Ҹ�����ע������ע����Ӯ���, 0:Ϊû����ע, 1Ϊ��עͬʱӮ��, 2Ϊ��ע��������, 3Ϊ��עͬʱ��Ҳ��䲻Ӯ
	int  m_iUserXiaZhuInfo[PLAY_COUNT][MAXCOUNT];	
	int	 m_iResultInfoArea[MAXCOUNT];			//������ʷ��ׯ�պ��������ʷ��¼

	int  m_iZongTime;				/**< ��ʱ��*/
    int  m_iSYTime;					/**< ʣ��ʱ��*/
	int  m_iTimeLabel;				//ʱ�ӱ�ǩ							   
	int  m_iXiaZhuTime;				/**< ��עʱ��	*/		
	int  m_iKaiPaiTime;     	    /**< ����ʱ��*/
	int  m_iFreeTime;				/**< ����ʱ��*/
	int  m_iCtrTime;				/**<������ǰ���벻����ע��ʱ��*/

	int  m_iNowNtStation;			/**< ��ǰׯ��λ��*/
	int  m_iLastNtStation;			/**< �ϸ�ׯ�ҵ�λ��*/
	bool m_bXiaZhuang;				/**< ��ǰׯ��������ׯ*/

	int       m_iCardCount;				/**< �Ѿ����Ƶ�������*/
	__int64   m_iWinQuYu[8];				/**< ��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���*/
									   
	bool m_bThreeCard;				/**< ׯ��������3����*/
	int  m_iZPaiXing[5];    	    /**< ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��*/
	int  m_iXPaiXing[5];    	    /**< �м�����*/
	int  m_iWinner;					/**< Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��*/
	int  m_iKaiPai;					/**< ���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ�7�ͣ�8ͬ���*/

	int  m_iXiaZhuType[6];			/**< �����ҵ���ע����*/
	WORD  m_UserCard[2][3];			/**< ��ҵ���*/
	BYTE  m_CardCount[2];			/**< ׯ��˫��������*/
	__int64  m_iUserFen[PLAY_COUNT];	/**< ������ұ��ֵĵ÷�*/
	__int64  m_iUserMoney[PLAY_COUNT];	/**< ������ұ���Ӧ�Ľ��*/
	BYTE m_byQuYuJinBi[8];			/**< ���������Ҹ���*/

	//30�ѱ�������
	int  m_iBaSHu;              	/**< ���֣�30��һ�֣����еİ��������ׯ�ң�*/
	int  m_iResultInfo[MAXCOUNT][4];/**< ���30�ѵĿ�����Ϣ:ׯ�еĵ����������ׯ���кͣ�Ӯ���ͣ�*/
	int  m_iGameCount;          	/**< �Ѿ����еľ�����ֻ����30�֡�*/

	int  m_iHaveGameCount;			//��Ϸ�����˶��ٰ�

	bool  m_bIsSuperStation[PLAY_COUNT];          	/**< ��¼������ҵ�λ�á�*/

	//���ñ�������
	int  m_iZhuangBaShu;      		/**< ׯ�ҽ����˼���*/
	__int64  m_iShangZhuangLimit; 		/**< ��ׯ��Ҫ�����ٽ��*/
	//int  m_iZhuangStation[4]; 	/**< ׯ���б��ܵ�ׯ��λ��*/
	/// ׯ���б��ܵ�ׯ��λ�� ���һ������ӱ�����
	int  m_iZhuangStation[PLAY_COUNT+1]; 	

	__int64  m_iNtWin;            		/**< ��ǰׯ��Ӯ�Ľ��*/
	int  m_iMaxZhuang;				/**< ׯ��һ����������پ֣�Ĭ��30�� */

	WORD wCardList[POKER_CARD_NUM*POKER_SET_NUM];  //���б�

	int   m_iNowJuShu;				/**<���ֽ��еľ��������е��ڼ��֣�72��һ�֣�*/

	RESULT_DATA  m_ResultData;		/**<72�ѵĽ������lym2010-03-17 */

	bool	m_bGamePlaying;			/**<lym2010-03-19��¼��Ϸ�Ƿ����ڽ����� */
	bool	m_bIsStart;				//��Ϸ�Ƿ��Ѿ����й�
	bool	m_bPrintEnding;			/**< �Ƿ��ӡ��� */
	CTime	m_RecordTime;			/**< ��¼������Ϸʱ�� */

	bool	m_bCanNote;				//�Ƿ�����ע
	bool    m_IsAIStation[PLAY_COUNT];/**< ��¼�����˵�λ�� */

	bool	m_bAIWinAndLostAutoCtrl;///��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
	__int64		m_iAIWantWinMoney;		///��������Ӯ���ƣ�������ҪӮ��Ǯ
	__int64		m_iAIMaxWinInOne;		///��������Ӯ���ƣ�������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	

	int		m_iAICtrlCount;			///��������Ӯ���ƣ���ǰ�������Ѿ��������Զ����Ƶİ���
	__int64		m_iAIHaveWinMoney;		///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ
	//--------------add by DWJ--20121108----------------------------------
	bool	m_bIsFirstLoadIni;			//�Ƿ��һ�ζ�ȡ�����ļ� ���ڱ����һ���������ڣ����û����һ��m_iAIWantWinMoneyA1��A3����Ҫ����
	__int64 m_iAIWantWinMoneyA1;		/**<��������Ӯ���ƣ�������ӮǮ����1  */
	__int64 m_iAIWantWinMoneyA2;		/**<��������Ӯ���ƣ�������ӮǮ����2  */
	__int64 m_iAIWantWinMoneyA3;		/**<��������Ӯ���ƣ�������ӮǮ����3  */
	int		m_iAIWinLuckyAtA1;			/**<��������Ӯ���ƣ�������������1ӮǮ�ĸ���  */
	int		m_iAIWinLuckyAtA2;			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	int		m_iAIWinLuckyAtA3;			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	int		m_iAIWinLuckyAtA4;			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	__int64	m_iAddMoneyEveryDay;		/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	__int64 m_iReSetAIHaveWinMoney;		/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ  */
	int		m_iRecFroDayTime;			/**<��¼��������ǰʱ�䣬�����ж��Ƿ����24�㣬����24�����A1��A2��A3�����ϵ���m_iAddMoneyEveryDay  */
	int     m_iApplyNtAINum;		///��Ҫ��ׯ�Ļ��������������ƻ�������ׯ��
	int     m_iAINtZhuangCount;		///���������ڵİ��������ƻ�������ׯ��

	///ׯ�ҳ�ˮ����
	BYTE	m_byNtTax;	

	///��ǰ����������µ���ע
	__int64		m_iMaxNote;	

	int   m_iRobotSZCount;                                          /**< �����������ׯ���� */
	BOOL  m_bIsVipDouble;                                           /**< ����Ƿ�Ϊvip�û�����˫����ע */
	BOOL  m_bDoubleWin;												/**< �Ƿ����ֻҪ������ׯ�ж�Ӯ */


	__int64	m_iUserOldMoney[PLAY_COUNT];			/* ��ҵĴ����� */

	bool	m_bStartGameWithOutNt;					/* ��ׯ�Ƿ�ʼ��Ϸ;true:��ʼ��Ϸ;false:����ʼ��Ϸ add xyh 2011-12-30*/

	bool    m_bShowAllResultInfo;                   ///�Ƿ���ʾ���е��˵���Ϣ

	int     m_iLevelBase;                          ///ƽ�ı���

#ifdef SUPER_VERSION
	WORD  m_wSuperCardList[8][54];				// �����ͻ���
	int   m_iSuperCardCount[8];					//
	BYTE  m_bySuperStation;						// �����ͻ���λ��
	typedef struct tagSetCardStruct	            // �ͻ��˵�������
	{
		BYTE abyPai[8][54];
		int	aiPaiCount[8];
		tagSetCardStruct()
		{
			memset(this, 0, sizeof(tagSetCardStruct));
		}
	}S_SUPER_SET_CARD;
#endif

//�Զ���ȡ�����ļ���ز���
#ifdef AUTOLOADINI
private:

	void	AutoLoadIni(int iRoomID);		//�Զ����������ļ� 

	void	LoadModifiedIni();		//�����޸ĵ������ļ� 

	void	LoadModifiedIni(int iRoomID);	//���ݷ�������޸ĵ������ļ�

#endif
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

};
#endif // !defined(_SERVERMANAGE_H__INCLUDED_)