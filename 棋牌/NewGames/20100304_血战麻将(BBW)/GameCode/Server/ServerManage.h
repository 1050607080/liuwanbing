#pragma once

/**
* �ٱ�����Ʒ�����
*/

#include "StdAfx.h"
#include "..\GameMessage\CLogicBase.h"

#include "..\ChangeFile\GameDataEx.h"
#include "..\ChangeFile\UserDataEx.h"
#include "..\ChangeFile\CheckHuPaiEx.h"
#include "..\ChangeFile\CheckCPGActionEx.h"
#include "..\ChangeFile\ThingHandleEx.h"


// ��ʱ�� ID
#define TIME_GAME_FINISH			91				// ��Ϸ������ʱ��
#define TIME_CHECK_CONNECT          92              // ������
#define DEFAULT_CHECK_TIME          10000           //���һ�������ղ�����Ϣ

//��Ϸ������־����
#define GF_NORMAL					10				// ��Ϸ��������
#define GF_SALE						11				// ��Ϸ��ȫ����
#define GF_NET_CAUSE                12              //����ԭ����Ϸ����   yjj 081204


#define GFF_FORCE_CONTINUE          2               //add by yjj

#define  DEFAULT_AIOUT_TIMER        600
#define  DEFAULT_AUTO_HU_TIME       600
#define  DEFAULT_AIOUT_HANDLE_TIMER 601          //Ĭ���Զ������ʱ��


#define TIME_CHECK_GAME_MSG          30              // �����Ϸ��Ϣ���ݰ�����ʱû���յ����ݰ�����Ϊ�ǿ�ס�ˣ�ϵͳ�Զ�����
#define TIME_DEFAULT_HANDLE          60000			 // Ĭ��ʱ��60��

const int TIME_STEP = 31;           /// ʱ��
const int TIME_STEP_TIME = 500;     /// 

const int TIME_NETWORK = 1000;      /// �����ӳ�ʱ��

/** ��Ϸ�������� */
#define GF_NORMAL					10
/** ��Ϸ��ȫ���� */
#define GF_SALE						11	

/// ��Ϸ������
class CServerGameDesk : public CGameDesk
{
public:

protected:

public:

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


protected:

	// ��Ϸ��������
	BYTE					m_iLessCount;						//������Ϸ����
	BYTE					m_iEnableWatch;						//�Ƿ������Թ�
	// ����״̬����
	BYTE					m_iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	BYTE					m_iLeaveArgee;						//�뿪ͬ���־

	// ������
public:
	// �û������뿪
	BOOL H041_UserHaveThing(BYTE bDeskStation, TCHAR * szMessage);
	// ͬ���û��뿪
	BOOL H044_ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee);

public:
	////////////�������ݶ���/////////////////////////////////////////////////////////
	///ͬ�������Ϸ���������
	BYTE			m_byAgreeLeaveNum;
    BYTE			m_bGameStation;
    int				m_RoomId;//����id

	//
	GameDataEx sGameData;//��Ϸ����
	UserDataEx	sUserData;//�û�����
	CheckCPGActionEx sCPGAction;//�����ܼ�ⷽ��
	CheckHuPaiEx sCheckHuPai;//���Ƽ�ⷽ��
	CountUserFenEx sCountFen;//��Ϸ���
	ThingHandleEx sThingHandle;//�¼�����
public:

	UINT m_uSocketID;

	//CSrvLogicEx		m_SrvLogic;//�������߼�
	///������Ϸ��ʱ��
	void SetGameTimer(UINT iId,int iTime);
	//ɾ��ָ����ʱ��
	void KillGameTimer(UINT iId);

    ///������Ϸ״̬
    void SendGameStationEx(BYTE bDeskStation, bool bWatchUser, void * pData, UINT uSize);
    ///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
    void SendGameDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///�����Թ����� ��������Ϣ���Թ��ߣ�
    void SendWatchDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
    void SendGameDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///�����Թ����� ��������Ϣ���Թ��ߣ�
    void SendWatchDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);



	///��ȡ�������
	UINT GetRoomRule();
	///��ȡ����id
	UINT GetRoomID();
	//��ȡ��Ϸ״̬
	BYTE GetGameStation(){return m_bGameStation;};
	//������Ϸ״̬
	void SetGameStation(BYTE state){m_bGameStation = state;};
	//��ȡ���ͬ��״̬
	void GetUserAgree(BYTE &agree);
	//��ȡ���䱶��
	int GetRoomPoint();
	//��ȡ��ҵ��ǳ�
	void GetUserNickName(BYTE station,char name[]);

};


/******************************************************************************************************/
