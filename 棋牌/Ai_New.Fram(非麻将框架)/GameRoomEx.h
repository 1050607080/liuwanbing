#pragma once
#ifndef AFC_GAMEROOMEX_HEAD_FILE
#define AFC_GAMEROOMEX_HEAD_FILE
#include "resource.h"
#include "Stdafx.h"

#include "BaseRoom.h"
#include "clientcomstruct.h"
#include "gameusermanage.h"
#include "gameroommessage.h"
#include "gameframedlg.h"
#include "map"
#include "string"

#include "CTime_day.h"

//�ṹ˵��
struct RoomInfoStruct;
//std::map<std::string,int>       g_ErrorUser;  


#define IDM_SET_GAME_TIME				WM_USER+100
#define IDM_KILL_GAME_TIME				WM_USER+101
#define IDM_CLOSE                       WM_USER+102  //20090211


#define IDT_GAMEEXIT					52
#define TIME_CHECK_GAMEEXIT				2000


#define ID_CHECK_STATE					50				//״̬��ʱ��
#define ID_CLEAR_ERRORDESK              60              //����������Ӽ�ʱ��
#define ID_CHECK_AIBANK                 51 

#define TIME_CHECK_STATE				20000			//���Լ�ʱ����



//��Ϸ������
class CGameRoomEx : public CBaseRoom
{
	//��Ϣ����
protected:
	HINSTANCE						m_hGameInstance;					//��Ϸ����
protected:
	
	CGameUserManage					m_UserManage;						//�û�����
	ComRoomInfo						m_ComRoomInfo;						//������Ϣ
	
	GameInfoStruct					m_GameInfo;							//��Ϸ��Ϣ
	IFrameInterface					* m_IGameFrame;						//��Ϸ���
	RoomInfoStruct                  * m_pRoomInfo;                      //������Ϣ
	BYTE							m_bEnterStyle;	
	bool                            m_bMachineAndPlay;                  //������������Ƿ�ͬ��
	bool                            m_bAutoDesk;
	int                             m_iMachineCheckTime;                   //�����˼����
	BYTE                            m_bMachineDeskCount;                //ÿ�������˸���
	BYTE                            m_bNowDesk;                         //��ǰҪ��������
	BYTE                            m_bErrorDesk[50];                   //����������
	int                             m_bErrorDeskCount;                  //���������Ÿ���������20��ʱ������
	__int64                         m_i64MaxAiMoney;                    //AI��������Ǯ�����ﵽ������ʹ�Ǯ
	__int64                         m_i64CheckMoneyMax;                 //ȡǮ�����
	__int64                         m_i64CheckMoneyMix;                 //ȡǮ��С��
	__int64							m_i64SaveAndOutMoneyBase;			//��ȡǮ������������ȡ������
	BOOL                            m_bAiBank;                          //�Ƿ�����˲�������
	BOOL							m_bOpenBankBase;					//�Ƿ���������������ȡ
	BOOL                            m_UseBank;
	int                             m_CheckCountMax;                    //��߼�����//�������������һֱռ��
	int                             m_CheckCount;                       //��ǰ������
	bool							m_bErrorDeskFlag[256];					//�������� 
	int								m_iContestState;					//0-δ������1-�ѱ�����2-�ѿ���
public:
	MSG_GP_R_LogonResult			m_pLogonInfo;						//��½��Ϣ
	//static __int64                  m_i64BeginMoney;                    //��ʼʱ�����ֵ
	//static __int64                  m_i64NowMoney;                      //��ǰʱ�����ֵ

	static  int                     m_nOnLinePeople;                    //��������

	__int64                         m_iMyMoney;                         //���ѵ�Ǯ



//��½����
	//��������
public:
	//���캯��
	CGameRoomEx(TCHAR * szGameName, RoomInfoStruct * pRoomInfo, MSG_GP_R_LogonResult * pLogonInfo,BYTE bEnterStyle, bool bMachineAndPlay, int iMachineUserId, BYTE bMachineDeskCount, bool m_bAutoDesk); 
	//��������
	virtual ~CGameRoomEx();

	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ӿں��� 
public:
	//����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent();

	///���û������Զ���ȡ������
	void SetBankInfo(BOOL bAiBand, BOOL bOpenBankBase, __int64 i64MaxAiMoney, __int64 i64CheckMoneyMax, __int64 i64CheckMoneyMix, __int64 i64SaveAndOutMoneyBase);

	//������Ϣ����
protected:
	//�����û���Ϣ
	bool SendUserInfo();
	//������Ϣ����
	bool OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�����½����
	bool OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�û��б���
	bool OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�û���������
	bool OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ����
	bool OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
#ifdef MY_MDM
	//������Ϣ������
	virtual bool OnItemDataMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//ʹ�õ���
	virtual bool OnUserUseItemResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
#endif
	//Ǯ����
	bool OnAboutMoney(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

	//������Ϸ����
	void ConnectToGameServer();
	//�رշ���
	void CloseGameRoom();
	//����
	bool OnSitDesk();
	//��������
	bool OnFindRandDesk(BYTE &bDeskIndex,BYTE &bDeskStation);
	//��������
	bool OnFindNullDesk(BYTE &bDeskIndex,BYTE &bDeskStation);
	//����������������
	bool OnFindTureDesk(BYTE &bDeskIndex,BYTE &bDeskStation);
	//���һ���������
	bool OnFindMachineDesk(BYTE &bDeskIndex,BYTE &bDeskStation);
	//��������
	BYTE GetNullStation(BYTE bDeskIndex);
	//������Ϸ�߼�
	bool StartGameClient();
	//���Ϳ�����Ϣ 
	inline UINT ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize);
	//��ʱ����Ϣ
	LRESULT SetGameTime(WPARAM,LPARAM);
	//��ʱ����Ϣ
	LRESULT KillGameTime(WPARAM,LPARAM);
    //�ر���Ϸ
    LRESULT OnCloseGame(WPARAM wparam,LPARAM lparam);//20090211
	//��ʱ��
	void OnTimer(UINT_PTR uIDEvent);
	//��Ҫ������
	bool NeedChange(BYTE bDeskIndex);
	//������
	bool FindOtherDesk();
	//����
	bool StandingDesk();
	//����״̬����ʱ��
	bool SetCheckStationTime();
	//�������
	void OnInvitePlay();
	//��ȡ�������
	LONG GetInvertID();
	//ͳ��ĳ������������
	BYTE CountMachine(BYTE bDeskIndex);
	//�Ƿ�Ϊ������ID
	//bool IsMachineID(LONG dwUserID);
	//ͳ��ĳ����������
	BYTE CountTruePlayer(BYTE bDeskIndex);
	//�Ƿ�Ϊ������ID
	//bool IsPlayerID(LONG dwUserID);
	//�Ƿ�Ϊ����������
	bool IsErrorDesk(BYTE desk);

	///��Ǯ
	void CheckMoney(__int64 iMoney);

	///ȡǮ
	void CheckMoneyWithPwd(__int64 iMoney);

	//�����˸��ݽ�Ҳ�������
	void AiControlBank();
public:
	void CheckEditMoney(__int64 iMoney, int UserID);




public:
	TTime_day		m_ExitTime;
	void SetExitTime(TTime_day time);
};
#endif