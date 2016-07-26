/// ServerManage.h: Headers and defines for CServerGameDesk
///


#if !defined(_SERVERMANAGE_H__INCLUDED_)
#define _SERVERMANAGE_H__INCLUDED_

#include "StdAfx.h"
#include "../GameMessage/UpGradeLogic.h"
#include <deque>

///��Ϸ������־����
#define GF_NORMAL					10			   /**<  ��Ϸ��������   */
#define GF_SAFE						11			   /**<  ��Ϸ��ȫ����   */


/*-------------------------------------------------------------------------*/
#define ID_TIMER_SEND_CARD		30				//��ʼ����
#define ID_TIMER_SHOW_RESULT	31				//���ƽ�����ʾ����		
#define ID_TIMER_SHOW_FINISH	32				//������ʾ����		
#define ID_TIMER_GAME_BEGIN		33				//��Ϸ��ʼ	
/*-------------------------------------------------------------------------*/


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
#define GF_Z_DAN				8			   /**<  ׯ��   */
#define GF_Z_SHUANG				9			   /**<  ׯ˫   */
#define GF_X_DAN				10			   /**<	 �е�   */
#define GF_X_SHUANG				11			   /**<	 ��˫   */
/*----------------------------------------------------------------------------*/
//6������ֵ
const	int	G_iChouMaValues[6] = {100,1000,10000,100000,1000000,10000000};
/*----------------------------------------------------------------------------*/
///��Ϸ������������
class CServerGameDesk :public CGameDesk
{
public:
	CServerGameDesk(void);
public:
	~CServerGameDesk(void);

	/// ���������ļ�
	BOOL LoadIni();
	BOOL LoadExtIni(int iRoomID);
	/// ������ж�ʱ��
	void KillAllTimer(void);	
public:
	///��Ϸ��ʼ
	virtual bool	GameBegin(BYTE bBeginFlag);
	///��Ϸ����
	virtual bool	GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	///�ж��Ƿ�������Ϸ
	virtual bool	IsPlayGame(BYTE bDeskStation);
	///��Ϸ���ݰ�������
	virtual bool	HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// �����Ϣ������
	virtual bool	HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	///�û��뿪��Ϸ��
	virtual BYTE	UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//�����ı��û����������ĺ���
	virtual bool	CanNetCut(BYTE bDeskStation);
	///�û������뿪
	virtual bool	UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
public:
	/// ��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	///������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	/// ��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	///��ʼ����Ϸ�߼�
	virtual bool InitDeskGameStation();



	///���������е����ݣ������õ�����Ԫ�����
	void MoveData(int type);

	///������ҵ���ע
	void	HanleXiaZhu(BYTE Station,void * pData, UINT uSize);
	///������ҵ���ׯ��Ϣ
	void	HanleSzhuang(BYTE Station,void * pData, UINT uSize);
	///������ҵ���ׯ��Ϣ
	void	HanleXzhuang(BYTE Station,void * pData, UINT uSize);


	///�����ͻ������û����˿��Ʋ���
	void SetAICtrl(BYTE Station,void * pData, UINT uSize);

	
	///�������
	void CountFen();
	//��ȡһ����
	void	GetACardValue(BYTE &byCard);
	///������ֵ
	void	CountPaiZhi(int count);
	///���ƿ�ׯ����
	void ConTrolZX();
	///����
	void SendCard(int iIndex=1);

	///�м����˵�����������£�ׯ����Ҫ����lym0511
	///pai:�мҵĵ�������ֵ
	bool	IsGetTheThird();

	///��ȡ��������
	bool	GetTheThird();

	///��ȡӮǮ������
	void	GetWinAres();


	///��ȡӮǮ������
	void GetWin();

	///ͬ����
	///@param num,���ǰ������
	bool	CheckTongDian(int num);

	///�����Ѽ�����Ƶ�����lym0511
	void ClearWinData(bool all);


	///����ׯ�е�ǰ������lym0511
	void ChangeCard();

	///һ�ֽ��������˿���ʱ������Ƿ��ܹ�������ʼ��Ϸ
	bool CheckCanBegin();

	///ׯ����ׯ���ȡ��һ��ׯ���б��е�ׯ��
	bool GetNextNT();

	///ͨ��ȫ�����ֵķ�ʽ��ʼ��Ϸ
	bool StartGame();



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
	void AuthPermissions(BYTE bDeskStation); 
	//�ж��Ƿ񳬶����
	bool IsSuperUser(BYTE byDeskStation);

	/// ��ȡ������
	int GetMeFanFen(int station);



	///��������������Ӯ�Զ�����
	void IAWinAutoCtrl();
	//-------ADD-BY-DWJ-20121108--------------------------------
	void CheckNewDayReSetIni(CINIFile *f, CString *key);//����Ƿ��Ѿ��µ�һ���ˣ���Ҫ�����趨�����˿������ò���

	void GetAIContrlSetFromIni(int iRoomID);			/**�������ļ��ж�ȡ�����˿������õĲ���ֵ*/

	///���������㵱ǰ�ƻ����˵���ӮǮ
	int CountAIWinMoney();

	///��������ȡһ������������ׯ�����Ļ�����λ��
	int GetAnAIApplyStation();

	//���Ϳ�����Ϣ
	void	OnSendOpenCard();
	//��Ϸ����
	void	OnSendGameResult();
	
	//���Ϳ�����Ϣ
	void OnSenFree();
	//��Ϸ���¿�ʼ
	void RestartGame();
	///��������ȡÿ�����������ע
	void GetQuYuCanNote();


protected:
	///ͳ�ƻ�������ׯ����
	int CountRobortNum();

	//����ׯ��
	void ClearNt();
protected:
	__int64  m_iMax[BET_ARES];							/**< ����ÿ��������Ƶ������ע��*/



	__int64  m_iAIMoney[BET_ARES];						/**< �����˵���ע*/



			  


	

	
	int  m_iWinner;					/**< Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��*/
	int  m_iKaiPai;					/**< ���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ�7�ͣ�8ͬ���*/





	bool	m_bCanNote;				//�Ƿ�����ע
	bool    m_IsAIStation[PLAY_COUNT];/**< ��¼�����˵�λ�� */

	bool	m_bAIWinAndLostAutoCtrl;///��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
	__int64		m_iAIWantWinMoney;		///��������Ӯ���ƣ�������ҪӮ��Ǯ
	__int64		m_iAIMaxWinInOne;		///��������Ӯ���ƣ�������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	

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

	int     m_iLevelBase;                          ///ƽ�ı���

private:
	CUpGradeGameLogic		m_Logic;     						//��Ϸ�߼�
private:	//��Ҫ���õı���
	int  m_iXiaZhuTime;				/**< ��עʱ��	*/		
	int  m_iKaiPaiTime;     	    /**< ����ʱ��*/
	int	 m_iShowResult;				//��ʾ�����ʱ��
	int  m_iFreeTime;				/**< ����ʱ��*/

	int  m_iMaxZhuang;				/**< ׯ��һ����������پ֣�Ĭ��30�� */

	__int64  m_iShangZhuangLimit; 		/**< ��ׯ��Ҫ�����ٽ��*/
private:	//һ�ֽ�������Ҫ���õı���
	bool			m_bGamePlaying;					/*��¼��Ϸ�Ƿ����ڽ����� */
	BYTE			m_iTotalCard[52];					//�ܵ���
	bool			m_bThreeCard;						/**< ׯ��������3����*/

	BYTE			m_byUserCard[2][3];					//ׯ�е�������0-�м� 1-ׯ��

	int				m_iZPaiXing[5];    					/**< ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��*/
	int				m_iXPaiXing[5];    					/**< �м�����*/

	int				m_iWinQuYu[BET_ARES];				/**< ��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ��ͣ�8ׯ�� 9ׯ˫ 10�е� 11��˫*/

	__int64			m_i64MaxZhu[BET_ARES];  				/**< ����ÿ������������µ�ע��*/
	__int64			m_i64ZhongZhu;   					/**< ���ѵ�ǰ��ע��*/
	__int64			m_iQuYuZhu[BET_ARES]; 				/**< ����ÿ�������µ�ע��*/
	__int64			m_iUserXiaZhuData[PLAY_COUNT][BET_ARES+1]; /**< �����Ϣ0-11��ҵ���ע��BET_ARES����µ���ע�9��ҵ÷֣�10��ҵ������ʣ�*/

	__int64			m_i64UserFen[PLAY_COUNT];			/**< ������ұ��ֵĵ÷�*/

	int				m_iUserPlayCount[PLAY_COUNT][2];		//��Ӧ��λ��������˶��پ� 0-��ʾ���˶����� 1-��ʾӮ�˶�����

	bool			m_bXiaZhuang;				/**< ��ǰׯ��������ׯ*/		


private:
	deque <LuziData> m_DqLuziData;							//·������
	__int64			m_i64UserWin[PLAY_COUNT];				//��ҵ���Ӯ���
	__int64			m_i64NtWin;            					/**< ��ǰׯ��Ӯ�Ľ��*/
	int				m_iNowNtStation;						/**< ��ǰׯ��λ��*/
	int				m_iZhuangBaShu;      					/**< ׯ�ҽ����˼���*/
	int				m_iZhuangStation[PLAY_COUNT+1]; 		/// ׯ���б��ܵ�ׯ��λ�� ���һ������ӱ�����


	vector	<long int>	m_vtSuperUserID;
};
#endif 