#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpGradeLogic.h"
#include "GameMessage.h"
#include "UpgradeMessage.h"

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա

//��Ϣ����
#define IDM_BEGIN			WM_USER+120					//��ʼ��ť��Ϣ
#define IDM_HAVE_THING		WM_USER+127					//����Ҫ��
#define IDM_SEND_THING		WM_USER+128					//�����뿪����
#define IDM_ARGEE_LEAVE		WM_USER+129					//ͬ���뿪��Ϣ
#define IDM_STOP			WM_USER+130					//��ǰ������ť
#define IDM_STOP_THING		WM_USER+131					//������ǰ����
#define IDM_ARGEE_STOP		WM_USER+132					//ͬ����ǰ����
#define IDM_PASS			WM_USER+133					//������ť��Ϣ
#define IDM_AUTO			WM_USER+134					//�й���Ϣ
#define	IDM_ACTIVE_RESULT	WM_USER+136					//�û�������
#define IDM_SHOW_NOTE		WM_USER+137					//�û�ѡ����ע,��ע
#define IDM_SELECT_NOTE_NUM	WM_USER+138					//�û���ע
#define	IDM_SOUND			WM_USER+139					//�û���������
#define IDM_BGSOUND			WM_USER+140					//��������

//��ʱ�� ID
#define ID_BEGIN_TIME		100							//��ʼ��ʱ��
#define TIME_XIAZHU         101							//��ע��ʱ��
#define TIME_EMPTY          102							//����ʱ�䶨ʱ��
#define TIME_KAIPAI         103						    //����ʱ��
#define TIME_WAIT           104                         //�ȴ���ʱ��
#define TIME_XZ_TIME        105                         //������ׯ��ʱ��
#define TIME_SZ_TIME        106                          //��ׯ��ʱ��

//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����
#define GAME_RAND_TIME				5				    //ϵͳʱ��

//������������
#define SOUND_WARNING				0x10				//����
#define SOUND_SEND_CARD				0x20				//��������
#define SOUND_INVALIDATE			0x30				//��ʾʧ��
#define SOUND_START_DEF				0x40				//�ĸ��˿�ʼ��Ϸ��
#define SOUND_GAME_OVER				0x50				//��Ϸ����
#define SOUND_WIN                   0x60                //ʤ��
#define SOUND_LOST                  0x70                //ʧ��
#define SOUND_NOTE                  0x80                //��ע������
#define SOUND_START                 0x90                //��Ϸ��ʼ
#define SOUND_KAIPAI                0xA0                //����


//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	/////////////////////////
public:
	//bool					m_bIsFinish;				//��Ϸ�Ƿ��Ѿ�����
	//����ׯ��
	BYTE                    m_zhunagStation;            //ׯ��
	int                     m_iGameBeen;                //��ǰׯ����Ϸ����
	__int64                 m_iMyZhu;                   //������ע
	int                     m_iZhuangMaxCount;          //����ׯ����ׯ������
	int                     m_iMaxDownMoney;            //ÿ�������ע	
	int                     m_iTempMoney;               //����
public:
	
	BYTE					m_iBeginTime;				//��Ϸ��ʼʱ��
	BYTE					m_iThinkTime;				//��ʱδ��
	BYTE                    m_iXiaZhuTime ;             ///��עʱ��
	BYTE                    m_iKaipaiTime ;             ///����ʱ��

	int						m_iRoomBasePoint;			//���䱶��
	int						m_iRunPublish;				//���ܿ۷�
	int						m_iLimitNote;				//����޶�ע
	int						m_iBaseNote;				//��ע

	__int64					m_iMyXiaZhuCount;			//������ע����	

	int						m_iNtMaxCount;				//�����������ׯ����
	int						m_iNtMinCount;				//������������ׯ����
	int						m_iMyNtCount;				//�ҵ�ׯ���˶��پ�

	//�ؼ�����
public:
	GameLogic               m_Logic;				    //��Ϸ�߼�

	int						m_iVersion;
	int						m_iVersion2;

	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();
    int GetGameStatus();

protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

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
	//�ܷ��뿪����
	virtual BOOL CanLeftDesk();
	//���Ż��㵽��ͼλ��
	virtual BYTE ViewStation(BYTE bDeskStation);
	//��Ϣ������
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//�˳�����
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT nIDEvent);

public:
	//��������
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);

	//������Ϸ
	void ResetGameStation(int iGameStation);

public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);

	// ��Ϸ��ʼ
	void OnGameBegin(GameBeginStruct * pGameBegin);

	// ���������ׯ����ׯ��Ϣ
	void OnUserApplyZhuang(EZP_APPLY_ZHUANG * pApplyZhuang);

	// �����ע��Ϣ
	void OnUserXiaZhuInfo(EZP_XIAZHU_INFO * userXiazhu);

	// ��ʼ��Ϸ(��Ϸ������...)
	void OnPlayGame(EZP_PLAY_GAME * pPlayGame);

public:
	//�����ע
	void OnSuiJiXiaZhu(int  &iNotetype);	
	///��ȡ�����
	int  OnGetRandNum(int iDeskStation,int iMaxNum,int iMinNum ) ; 
		

};
