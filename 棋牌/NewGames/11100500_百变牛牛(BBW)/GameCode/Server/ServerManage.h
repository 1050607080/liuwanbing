// ServerManage.h: Headers and defines for CServerGameDesk
//////////////////////////////////////////////////////////////////////
#if !defined(_SERVERMANAGE_H__INCLUDED_)
#define _SERVERMANAGE_H__INCLUDED_

#include "StdAfx.h"
#include "../GameMessage/UpGradeLogic.h"
#include <vector>
#include <deque>

//��Ϸ������־����
#define GF_NORMAL					10				// ��Ϸ��������
#define GF_SAFE						11				// ��Ϸ��ȫ����

#define ID_GAME_TIMER_FALG_START    0x10			// �ܻ�ȡ���ʣ��ʱ��Ķ�ʱ����־
#define ID_GAME_TIMER_FALG_END      0x20			// �ܻ�ȡ���ʣ��ʱ��Ķ�ʱ����־

#define ID_TIMER_SEND_CARD          0x10			// ��ȡʣ����ٵĶ�ʱ����0x10~0x20��Χ�ڣ�ֻ��Ҫ�������һ���ֽ�
#define ID_TIMER_OUT_CARD           0x11			// ���ƶ�ʱ��ID
#define ID_TIMER_GAME				0x12			// ��Ϸʱ��

#define ID_TIMER_SEND_HAVETHING     0x21			// ����Ҫ��

#define TIMER_REMAIND_COUNT		30		//ʣ��ʱ�䵹��ʱ
#define ID_TIMER_XIA_ZHU       31           // ��ע
#define ID_TIMER_KAI_PAI       32           // ����
#define ID_TIMER_SHOW_RESULT   33           // ��ʾ�����ʱ��
#define ID_TIMER_FREE_TIME     34           // ����ʱ��
/*--------------------------------------------------------------------------------*/
// ���������ֵ
static __int64	G_i64ChoumaValue[6]={100,1000,10000,100000,1000000,10000000};
/*--------------------------------------------------------------------------------*/
//·����Ϣ�ṹ��
struct	GameLuziInfo
{
	int		iResultInfo[MAX_NOTE_COUNT];	//0-�� 1-�� 2-�� 3-��
	GameLuziInfo()
	{
		memset(this,0,sizeof(GameLuziInfo));
	}
};
/*--------------------------------------------------------------------------------*/
///��������Ϸ����
class CServerGameDesk :public CGameDesk
{
protected:
	CPokerGameLogic				m_PokerGameLogic;//������Ϸ�߼�

private: //�����õı���
	int							m_iXiaZhuTime;		//��עʱ��	
	int							m_iKaiPaiTime;		//����ʱ��     	    
	int							m_iFreeTime;		//����ʱ��	
	__int64						m_iShangZhuangLimit;//��ׯ��Ҫ�����ٽ�� 
	int							m_iMaxZhuang;		//ׯ��һ����������پ֣�Ĭ��30��
	bool						m_bCompareKind ;	//��Ϸ���Ƿ�Ƚϻ�ɫ 1 �Ƚϻ�ɫ �� 0 ���Ƚϻ�ɫ
	bool						m_bHaveKing ;		//�Ƿ�Ҫ��С��
	BOOL						m_bNtTax;			//�Ƿ�ֻ��ׯ��˰
	int							m_iRobotSZCount;	//�����������ׯ����(0��ʾ������)
	__int64						m_iMaxNote;			//��������ע��
	__int64						m_iRectNoteMax[MAX_NOTE_COUNT];//�ĸ�������ע����(0-������)
	BYTE						m_bCardShapeBase[11];//0-10������ţ��ţţ��ı���
	/*--------------------------�����˽��ؿ��Ʋ�������----------------------------------*/
	bool						m_bAIWinAndLostAutoCtrl;///��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
	int							m_iAIWinLuckyAtA1;			/**<��������Ӯ���ƣ�������������1ӮǮ�ĸ���  */
	int							m_iAIWinLuckyAtA2;			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	int							m_iAIWinLuckyAtA3;			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	int							m_iAIWinLuckyAtA4;			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	__int64						m_iAIWantWinMoneyA1;		/**<��������Ӯ���ƣ�������ӮǮ����1  */
	__int64						m_iAIWantWinMoneyA2;		/**<��������Ӯ���ƣ�������ӮǮ����2  */
	__int64						m_iAIWantWinMoneyA3;		/**<��������Ӯ���ƣ�������ӮǮ����3  */
	__int64						m_iReSetAIHaveWinMoney;		/**<��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ  */
	__int64						m_iAIHaveWinMoney;			///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ


private:
	vector	<long>				m_vlSuperUserId;			//����ID����
	int							m_iRemaindTime;				//���׶ε�ʣ��ʱ��
private:
	int							m_iNowNtStation;	//��ǰׯ��λ��	
	bool						m_bXiaZhuang;		//��ǰׯ��������ׯ	
	int							m_iZhuangBaShu;		//ׯ�ҽ����˼���  
	int							m_iGameCount;		//��Ϸ���� 72��Ϊһ��                        
	int							m_iGameBeen;		//��Ϸ���� 

	__int64						m_iNtWin;//��ǰׯ��Ӯ�Ľ�� 

	BYTE						m_UserCard[5][5];	//��ҵ���	
	BYTE						m_CardCount[5];//ׯ��˫��������	
	BYTE						m_UserCardTotal[54];//�ܵ���	
	deque	<GameLuziInfo>		m_dqResultInfo;		//·������
	int							m_iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];//���X�ѵĿ�����Ϣ 	0123-�������

	TagSuperSet					m_TSuperSet;	//���˿��ƽ��


	



	bool						m_bIsStart;//��Ϸ�Ƿ���Կ�ʼ

           
         

	__int64						m_iZhongZhu;//���ѵ�ǰ��ע��   					
	__int64						m_iQuYuZhu[MAX_NOTE_COUNT];//����ÿ�������µ�ע�� 
	__int64						m_iQuYuZhuTrue[MAX_NOTE_COUNT];//������ʵÿ�������µ�ע�� 
	__int64						m_iUserXiaZhuData[PLAY_COUNT][MAX_NOTE_COUNT+1];//ÿ�������Ϣ0-3��ҵ���ע��4����µ���ע 
	__int64						m_iZhuangFen;//ׯ�ҵĵ÷֣�����  					
	__int64						m_i64Max;//���������ע                          
	__int64						m_iXianFen;//�����мҵĵ÷�						
	

	

									   
						
			
		
	
	int							m_iComputePoint[MAX_NOTE_COUNT];//����ҵ���Ӯ����   ���м�λ��ׯ��λ�ȷ�ֻ����λ0-3���������         
			
	__int64						m_iUserFen[PLAY_COUNT];//������ұ��ֵĵ÷�	
	__int64						m_iUserMoney[PLAY_COUNT];//������ұ���Ӧ�Ľ��	

	
	
	  	              	
	
			
	//����һ����ά���飬��һ�б�ʾ��ׯ��,�ڶ��б�ʾ�Ƿ���ׯ;
	short int					m_idzhuangstation[PLAY_COUNT+1][2];
	           		
	
	

	
	
	BOOL						m_bIsControlWin;//���˿�����

	
	



public:
	CServerGameDesk(void);
public:
	~CServerGameDesk(void);
	//���ط����������ļ�
	BOOL	LoadIni();
	/// ���ط����������ļ�
	BOOL	LoadExtIni(int iRoomID);
	/**�������ļ��ж�ȡ�����˿������õĲ���ֵ*/
	void	GetAIContrlSetFromIni(int iRoomID);			

	
	//ɾ����ʱ��
	bool KillGameTimer(BYTE byTimerID);
	//������ж�ʱ��
	void KillAllTimer(void);

	//��ʼ����Ϸ�߼�
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
	/// ��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	/// ��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE byTimerID);
	/// ��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	///������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);


protected:
	//����һ�뵹��ʱ��ʱ�� ���ڼ�����׶λ�ʣ����ʱ��
	void	SetTimeRemaindCount(int  iTime);
	//���������е����ݣ������õ�����Ԫ�����
	void MoveData(int type);
	//������װ�б�������ׯ˳�����з��ͳ�ȥ
	void ResetZhuangStation();
	///lym0512������ҵ���ע
	void HandleXiaZhu(BYTE Station,void * pData, UINT uSize);
	///lym0512������ҵ�����ׯ��Ϣ
	void HanleSXzhuang(BYTE Station,void * pData, UINT uSize);
	///�������lym0512
	void CountFen();
	//�յ����˿�����Ϣ
	void	OnHandleSuperUserSet(BYTE bDeskStation,void * pData, UINT uSize);

	///���ƿ�ׯ����zht22091120
	void ControlWin(BYTE bDeskStation, BYTE bGameStation, BYTE BGameWin[]);
	///����lym0512
	void	SendCard();	
	///��ȡӮǮ������lym0513
	void GetWin();
	///�����Ѽ�����Ƶ�����lym0511
	void ClearWinData(bool all);
	//������
	void ChangeCard(BYTE bDestStation,BYTE bWinStation);

	/*1 �õ���Ӯ��� zht
	* @param [in] win �õ������Ӯ��1Ϊ�õ�Ӯ�ң���1Ϊ���
	* @return �õ�Ӯ�ƻ�����λ��
	*/ 
	BYTE GetWiner(int win);
	///һ�ֽ��������˿���ʱ������Ƿ��ܹ�������ʼ��Ϸlym0512
	bool CheckCanBegin();
	///ׯ����ׯ���ȡ��һ��ׯ���б��е�ׯ��lym0512
	bool GetNextNT();
	///ͨ��ȫ�����ֵķ�ʽ��ʼ��Ϸlym0512
	bool StartGame();
	///��¼���ֵĳɼ�����ym0512
	void RecordData();
	//��ʼ����������������ym0514
	///��ʼ���󲿷����ݣ���Щ���ݲ��ܳ�ʼ���ģ���ǰׯ�ҽ����˼��֣�ǰ30�ֵ����ݣ�
	void ServerInit();
	///lym������Ϸ�ܷ�ʼ����������ֹ��ׯ�ļٿ�ʼ
	bool CanBeginGame();
	///��־��¼�ӿ�
	void  SrvTestlog(TCHAR *pMessage);
	//�õ�����Ƿ���Կ�����Ӯ
	bool CanControlWin(BYTE bDeskStation);
	//�Ƿ񳬶����
	bool	IsSuperUser(BYTE bDeskStation);
	///������ҷ���
	int ComputePoint(BYTE DeskStation);
	
	//����ׯ��
	void ClearNt();

	//���˿���
	void	SuperUserControl();
	//��������������Ӯ�Զ�����
	void IAWinAutoCtrl();
	//���������㵱ǰ�ƻ����˵���ӮǮ
	int CountAIWinMoney();
	BOOL  CanDouble(BYTE bDeskStation);
	//���Ϳ�����Ϣ
	void OnSendFree();
	//���Ϳ�����Ϣ
	void OnSendKaiPai();

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
#endif 