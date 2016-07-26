#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpGradeLogic.h"
#include "UpGradeMessage.h"


#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա

//��ʱ�� ID
#define ID_BEGIN_TIME				101					//��ʼ��ʱ��
#define ID_ROB_NT_TIMER				102					//��ׯ��ʱ��
#define ID_NOTE_TIMER               103	                //��ע��ʱ��
#define ID_OPEN_CARD_TIMER			104					//��ţ��ʱ��


//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����

//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	//��������
public:
	//�̶���Ϣ
	//=====================ʱ�����Ϣ===================
	BYTE					m_iThinkTime;				//������Ϸ����
	BYTE					m_iBeginTime;				//��Ϸ��ʼʱ��
	BYTE					m_iCallScoreTime;			//��Ϸ��ʼʱ��
	BYTE					m_iXiaZhuTime;				//��עʱ��
	int						m_iRoomBasePoint;			//���䱶��
	int						m_iRunPublish;				//���ܿ۷�
	int						m_iVersion;
	int						m_iVersion2;

	BYTE					m_iCardCount;				//������Ϸ���˿���Ŀ
	BYTE					m_iPalyCardCount;			//��Ϸ�˿���Ŀ


	__int64						m_iThisTurnLimit;			//������ע

	//������Ϣ
	BYTE					m_iDeskCardCount[PLAY_COUNT];		//ÿ�������˿˵���Ŀ
	BYTE					m_UserCard[PLAY_COUNT][SH_USER_CARD];			//�û��˿�
	BYTE                    m_iUpBullCard[PLAY_COUNT][3];//�����ţ��

	BYTE					m_bUpCard[2];				//�������
	BYTE                    m_bUnderCard[3];			//�������

	//������Ϣ
	BYTE						m_bTimeOutCount;			//��ʱ
	BYTE						m_bShowAnimalStation;		//������ʾλ��
	int					    	m_iNowOutPeople;			//���ڳ�����
	int						    m_iFirstOutPeople;			//��һ��������
	__int64					    	m_iNowBigNote;				//��ǰ���Ѻע��
	__int64						    m_iThisGameNote[PLAY_COUNT];			//������Ϸ��ע��(��ע)
	__int64						    m_iTotalGameNote[PLAY_COUNT];			//������Ϸ��ע��(ʵ��)
	int						    m_iUpGradePeople;					//ׯ��λ��
	int                         m_iNote;                    //��ǰ��Ч��ע����ǰλ�����Ч��ע��;
	BOOL						m_bBgSound;					//��������
	BOOL                        m_bMustGiveup;
//	BYTE					m_iGameNoteSetType;				//�Ƿ�����ţţ�����е����͵����ֵ 
	int						m_iGameTimes;					/**<��Ϸ����*/					
	bool					m_bLeftOrGoOn;					/**<��Ϸ���������뿪���Ǽ�����Ϸ true:������Ϸ false:�뿪*/
	BYTE					m_byGameTimes;					/**<���پ���Ϸ���뿪��Ϸ*/
	BYTE					m_byLeaveThinkTime;				/**<�뿪˼��ʱ��*/
	int						m_iProNote[4];					/**<�ĸ�����ֵ ��ע���� �� Ϊ 100%*/
	//�ؼ�����
public:
	CUpGradeGameLogic		m_Logic;					//�����߼�


	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();

protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	virtual void OnWatchSetChange(void);
	virtual void OnGameSetting();
	//���غ���
public:
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������ý���
	virtual void ResetGameFrame();
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	//������ж�ʱ��
	void KillAllTimer();
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};
	//�û��뿪
	virtual bool GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);


	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);


	void ResetGameStation(int iGameStation);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	//	afx_msg void OnClose();
protected:
	virtual void OnCancel();



};
