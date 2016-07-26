#pragma once

/**
* �ٱ�����Ʒ�����
*
* @author duanxiaohui
* @version 1.0
* @date 2009.12.10
*/

#include "StdAfx.h"

#include "..\GameMessage\GameMessage.h"
#include "..\GameMessage\UpGradeLogic.h"

/** ���߶�ʱ�� ID */
#define IDT_USER_CUT				1L
///��ׯģʽ�еȴ���ʱ��
#define ID_WIAT_GAME_BEGIN          25 
///֪ͨ�ͻ���ϴ��
#define TIME_RAND_CARD              28 
/**��ׯ��ʱ��*/
#define TIME_JIAO_ZHUANG            29
/** ��ע��ʱ�� */
#define TIME_XIAZHU                 30
/** ���ƶ�ʱ�� */
#define TIME_SEND_CARD				31	
/** ��Ϸ������ʱ�� */
#define TIME_GAME_FINISH			32
/** ����ʱ�䶨ʱ�� */
#define TIME_EMPTY                  33

#define TIMT_APPLY_NT               34  



/** ��Ϸ�������� */
#define GF_NORMAL					10
/** ��Ϸ��ȫ���� */
#define GF_SALE						11	
/** ��ǰ���� */
#define GF_AHEAD					12
/** δ�ܹ�����ʤ�� */
#define GF_NO_WINER					13

//�����˽��ؿ��ƾ�̬ȫ�ֱ���
static __int64			G_i64AIHaveWinMoney;		//������ӮǮ�� ��������Ϊ��̬ȫ�ֱ�������Ϊ���������Ҫ��ʱ�޸ĸñ���
static __int64			G_i64ReSetAIHaveWinMoney;	/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ  �ñ���ҲҪ����Ϊ��̬ȫ�ֱ��� ����ÿ�����Ӷ���ȥ����*/


/// ��Ϸ������
class CServerGameDesk : public CGameDesk
{
public:
	GameLogic               m_gameLogic;                         /**< ��Ϸ�߼� */

protected:
	EZP_Xiazhu_Data         m_UserXiaZhu;                        /**< ��ע���� */

	CArray <GameLuziInfo, GameLuziInfo&> m_GameLizuInfo;         /**< ��¼��Ϸ·�� */

	CArray <BYTE, BYTE&>    m_GameKPInfo;                        /**< ��¼ÿ�ֿ�����Ϣ��ֻ��¼����X��(��4λ��ʾ: 0x01:˳�š�0x02:���š�0x04:����) */

	CArray <BYTE, BYTE&>    m_GameXZInfo[PLAY_COUNT];            /**< ��¼ÿ�������ע��¼, �뿪�Ƽ�¼��Ӧ(��4λ��ʾ: 0x01:˳�š�0x02:���š�0x04:����)*/

	CArray <BYTE, BYTE&>    m_ArrUserList;                       /**< ������ׯ������б� */

	int                     m_iApplyMoneyLimit;                  /**< ������ׯ�������*/

	int                     m_iZhuangCount;                      /**< ��ǰׯ����ׯ���� */

	int						m_iZhuangMaxNum;                     /**< ׯ�������ׯ���� */
	
	int                     m_iZhuangStation;                    /**< ��ǰׯ�� */
	
	__int64                 m_iZhuangResult;                     /**< ׯ����Ӯ�ɼ� */

	__int64                 m_iUserResult[PLAY_COUNT];           /**< ÿ����ҵ���Ӯ�ɼ� */
	
	__int64                 m_iAllMoney;                         /**< ���������ע�ܶ� */	
	
	BYTE                    m_bSendCardPos;                      /**< ��ʼ����λ�� */

	BOOL                    m_bNTGameMode;                       /**< ��Ϸģʽ TRUE:��ׯģʽ  FALSE:��ׯģʽ */

	BOOL                    m_bUserXZ;                           /**< ��ǰ��ׯ״̬ */	

	BOOL                    m_bInNoteStage[PLAY_COUNT];                      ///�Ƿ�����ע�׶�

	BOOL                    m_bPrintEnding;                      /**< �Ƿ��ӡ��� */

	BOOL                    m_bUseCard;                          /**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */

	BOOL                    m_bKingAndThreeBig;                  /**< TRUE: �������3��� FALSE: ˫��Q���*/

	BYTE                    m_bGameVersion;                      /**< ��Ϸ�汾 0: ��ͨ 1: ������ 2: ���«2�� */

	BOOL                    m_bRedJOrBlackJ;                      /**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */

	char                    m_chGameCount;                       /**< ��Ϸ���� 72��Ϊһ�� */

	char                    m_chGameBeen;                        /**< ��Ϸ���� */

	CTime                   m_RecordTime;                        /**< ��¼������Ϸʱ�� */

	int                     m_iRobotSZCount;                     /**< �����������ׯ���� */

	int                     m_iXiaZhuTime;                       /**< ��עʱ�� */

	int                     m_iKaipaiTime;                       /**< ����ʱ�� */

	int                     m_iEmptyTime;                        /**< ����ʱ�� */

	int                     m_iTime;                             /**< ��ǰʱ�� ����Ϊ��λ */


	int                     m_iMaxZhuangCount;                   /**< �����ׯ���� */

	int                     m_iMinZhuangCount;                   /**< ��С��ׯ���� */

	int                     m_iNtTax;                            /**< ��ˮ���� 0��ֻ��ׯ��˰ 1����ȫ��Ӯ�ҵ�˰ Ĭ��ֵΪ0 */

	__int64                 m_iMaxNote;                          /**< �м������ע��(����������ע) */
	__int64                 m_iRectNoteMax[REGION_COUNT] ;       /**< ����ÿ������������ע 0-�������1-�Һ�����2-˳�ţ�3-���ţ�4-��ǣ�5-���ţ�6-�ҽ�*/

	int                     m_iUserNoteZeroCount[PLAY_COUNT];   ///�������û����ע�ļ�¼����


public:
	static BYTE m_iAllCardCount;								 /**< ��Ϸ���õ����� */
	int m_iLimitNote;											 /**< ���� */
	int m_iBaseNote;											 /**< ��ע */

	BYTE m_iSendCardTime;										 /**< ��Ϸ˼��ʱ�� */
	BYTE m_iThinkTime;											 /**< ��Ϸ˼��ʱ�� */
	BYTE m_iBeginTime;											 /**< ��Ϸ��ʼʱ�� */
	bool m_bStartGameWithOutNt;					/* ��ׯ�Ƿ�ʼ��Ϸ;true:��ʼ��Ϸ;false:����ʼ��Ϸ add xyh 2012-1-2*/

	/// �����˿�����Ӯ��������
private:
	bool                    m_bIsRobot[PLAY_COUNT];              /**< ��־�Ƿ�Ϊ������ */

	

	int		                m_iAIWantWinMoney;		             /**< ��������Ӯ���ƣ�������ҪӮ��Ǯ */	//dwjdelete

	int		                m_iAIHaveWinMoney;		             /**< ��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ */	//dwjdelete

	int		                m_iAIMaxWinInOne;		             /**< ��������Ӯ���ƣ�������ƽ��ÿ�����ӮǮ����С�ڵ���0ʱΪ���ޣ� */	//dwjdelete

	int		                m_iAICtrlCount;			             /**< ��������Ӯ���ƣ���ǰ�������Ѿ��������Զ����Ƶİ��� */

	int                     m_iWinQuYu[6];				         /**< ��Ϸ��ӮǮ���� 0˳�ţ�1˳��-�ǣ�2���ţ�3�ţ�4���ţ�5����-�� */

private:	//�����˽�����Ӯ���Ʋ���	ADD BY DWJ
	bool	        m_bAIWinAndLostAutoCtrl;    /**< ��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ����� */
	__int64			m_i64AIWantWinMoneyA1;		/**<��������Ӯ���ƣ�������ӮǮ����1  */
	__int64			m_i64AIWantWinMoneyA2;		/**<��������Ӯ���ƣ�������ӮǮ����2  */
	__int64			m_i64AIWantWinMoneyA3;		/**<��������Ӯ���ƣ�������ӮǮ����3  */
	int				m_iAIWinLuckyAtA1;			/**<��������Ӯ���ƣ�������������1ӮǮ�ĸ���  */
	int				m_iAIWinLuckyAtA2;			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	int				m_iAIWinLuckyAtA3;			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	int				m_iAIWinLuckyAtA4;			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */


	/// �����ͻ��˱�������
private:
	BOOL                    m_bSuperSetting;                     /**< ��ʶ�����Ƿ��п��� */	

	char                    m_bSuperState[3];                    /**< 0: ����˳�� 1: ������� 2: ������ (TRUE: ѡ�� FALSE: δѡ��) */

	bool					m_bIsVipDouble;						//VIP��ҿ���˫����ע--( true )

	bool                    m_bUserNetStation[PLAY_COUNT] ;          ///�û�����״̬

	bool                     m_bWriteLog ;           ///�Ƿ��ӡ��־

	bool					m_bIsRunning;						//��Ϸ�Ƿ��Ѿ�����


	/// ������Ӯ����
	void OnSuperSetting(BYTE bDeskStation,  SUPERSETTING * pMsg);  

	/// ��֤�����û�Ȩ��
	void AuthPermissions();

	/// ִ�г����ͻ�������
	BOOL RunSuperSetting();

public:
	/// ���캯��
	CServerGameDesk(); 
	/// ��������
	virtual ~CServerGameDesk();

public:
	/// ��ʼ��Ϸ
	virtual bool InitDeskGameStation();
	/// ��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	/// ��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	/// �ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	/// �����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// ��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// �û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);	
	//��Ҷ��ߴ���
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	/// �������
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	/// ��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	/// ������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	/// ��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);

	// ���VIP ˫����ע����(��������Ϸ)
	BOOL  CanDouble(BYTE bDeskStation);

public:
	/// ��ȡ��̬����
	BOOL LoadIni();
	/// ���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);
	/// �������޺͵�ע
	BOOL SetLimitNote();
	/// ͳ����Ϸ����
	BYTE CountPlayer();
	/// ɱ���м�ʱ��
	void KillAllTimer();
	/// ɱ���м�ʱ��
	void KillGameAllTimer();
	/// ���ݳ�ʼ��
	void InitData();
	/// ���Ƴ����ͻ��˺�����
	void InitSuperConfig();
	/// �ж�ׯ���Ƿ������Ϸ����
	bool IsZhuangRule();
	/// �ж���ׯ�б�
	bool IsZhuangList();
	/// ����
	void FaPaiToAllUser();
	/// ���䱶��
	int	 GetRoomBasePoint();
	/// ���ܿ۷�
	int	 GetRunPublish();
	/// ���������ׯ
	void OnApplyZhuang(BYTE bDeskStation, EZP_APPLY_ZHUANG * pApplySZ);
	/// �����ע
	void OnUserXiaZhu(BYTE bDeskStation, EZP_XIAZHU_INFO * pXiazhu);
	/// ��Ϸ����
	void DoGameResult(BYTE bResult, __int64  iMoney[], __int64  iReturnMoney[]);
	/// ��Ϸ��ʼ
	void StartGame();
	///֪ͨ�ͻ��˿�ʼ��ע
    void  NotifyClientNote();
	/// ֪ͨ�ͻ��˿�ʼ��ׯ
	/// ��ע���
	void OnUserXiaZhuFinish();
	/// ��ʱ��
	void SetGameTimer(UINT uTimeID, UINT uMaxTime);
	/// �ж���Ϸģʽ
	void DecideGameMode();
	/// ��ӡ���
	void PrintEnding();
    ///��ӡ���
	void DebugPrintf2(int iRoomID, const char *p, ...) ; 
	/// �����������
	void ClearUserInfo(BYTE bDeskStation);
	/// ÿ����Ϸ�����󣬼����ׯ�б�����ҽ���Ƿ񻹹���ׯ����
	void CheckAppalyList();
	/// ��ȡӮǮ������
	void GetWin();
	/// ��������������Ӯ�Զ�����
	void IAWinAutoCtrl();
	//�жϸ�Ӯ���Ǹ���
	bool	JudgeShouldWinOrLost();
	//����ָ�����ҵ���
	void	ExChangeCard(BYTE byFirstStation, BYTE bySecondStation);
	/// ���������㵱ǰ�ƻ����˵���ӮǮ
	__int64 CountAIWinMoney();

	//��¼��������Ӯֵ
	void	RecordAiHaveWinMoney(__int64 *ChangeMoney);

	///�������ƹ���
	void SetCardFun();

	/*
	* ��ӡ��־�ļ�
	*/
	void DebugPrintf(const char *p, ...);

	//�Զ���ȡ�����ļ���ز���
#ifdef AUTOLOADINI
private:

	void	AutoLoadIni(int iRoomID);		//�Զ����������ļ� 

	void	LoadModifiedIni();		//�����޸ĵ������ļ� 

	void	LoadModifiedIni(int iRoomID);	//���ݷ�������޸ĵ������ļ�
#endif
};


/******************************************************************************************************/
