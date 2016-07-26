#pragma once

#include "StdAfx.h"
#include "Resource.h"
//#include "ClientView.h"
#include "UpGradeLogic.h"
#include "UpGradeMessage.h"
//#include "MidiSample.h"

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա
//��Ϣ����
#define IDM_BEGIN			WM_USER+120					//��ʼ��ť��Ϣ
//#define IDM_USER_NT			WM_USER+121					//�û�����
//#define IDM_GIVE_BACK		WM_USER+123					//�����ư�ť
//#define IDM_OUT_CARD		WM_USER+124					//�û�����
//#define IDM_LOOK_LAST		WM_USER+125					//�������˿�
//#define IDM_BACK_CARD		WM_USER+126					//������Ϣ
#define IDM_HAVE_THING		WM_USER+127					//����Ҫ��
#define IDM_SEND_THING		WM_USER+128					//�����뿪����
#define IDM_ARGEE_LEAVE		WM_USER+129					//ͬ���뿪��Ϣ
#define IDM_STOP			WM_USER+130					//��ǰ������ť
#define IDM_STOP_THING		WM_USER+131					//������ǰ����
#define IDM_ARGEE_STOP		WM_USER+132					//ͬ����ǰ����

#define IDM_PASS			WM_USER+133					//������ť��Ϣ
#define IDM_CALL_SCORE		WM_USER+135					//�з�
#define IDM_AUTO			WM_USER+134					//�й���Ϣ
#define	IDM_ACTIVE_RESULT	WM_USER+136					//�û�������
#define IDM_SHOW_NOTE		WM_USER+137					//�û�ѡ����ע,��ע
#define IDM_SELECT_NOTE_NUM	WM_USER+138					//�û���ע
#define	IDM_SOUND			WM_USER+139					//�û���������
#define IDM_BGSOUND			WM_USER+140					//��������
//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_RESULT_TIME				101					//������ʱ��
#define ID_OUT_CARD					103					//���Ƶȴ�ʱ��
#define ID_LOOK_BACK				105					//������
#define ID_CHANGE_MUSIC			    106					//����������
#define ID_CALL_SCORE_TIME			107					//��ׯ��ʱ��
#define ID_NOTE_TIME                110                 //�ȴ��м���ע
#define ID_TAKE_CARD_TIME			111					//Ҫ�ƶ�ʱ��


#define GAME_RAND_TIME				2					//����ʱ��
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
	BYTE					m_iThinkTime;				//Ҫ����Ϸʱ��
	BYTE					m_iBeginTime;				//��Ϸ��ʼʱ��
	BYTE					m_iCallScoreTime;			//��ׯʱ��

	BYTE					m_iSendCardTime;			//��һ����ʱ��(�����ٶ�)
	BYTE					m_iXiaZhuTime;				//��עʱ��
	BYTE					m_byLostTime;				//��ʱ��ʣ��ʱ��
	
	int						m_iRoomBasePoint;			//���䱶��
	int						m_iRunPublish;				//���ܿ۷�
	int						m_iVersion;
	int						m_iVersion2;
	int						m_iMyDeskStation ;			//����Լ�����Ϸλ��

	BYTE					m_byGameStation ;			//��ǰ����Ϸ״̬
	BYTE					m_iCurrentCardCount;		//��ǰҪ�����Ƶ���Ŀ
	BYTE					m_iCardCount;				//������Ϸ���˿���Ŀ
	__int64					m_iLimitNote[PLAY_COUNT];				//����޶�ע
	__int64					m_i64UserNoteLimite[PLAY_COUNT][4];	//����ĸ���ע����
	__int64					m_iThisTurnLimit;			//������ע
	

	BYTE					m_iPalyCardCount;			//��Ϸ�˿���Ŀ


	//�������
	//deque<BYTE>                 m_sendQueue;			//����˳������
	BYTE						m_bySendCardPos;		//��ǰ����λ��
	BYTE						m_bySendCardCount;		//���ƴ���
	BYTE						m_bySendCount[PLAY_COUNT];		//�Ѿ����˶�������
	//�Լ��˿�����
	//BYTE					m_iMeCardCount;				//�Լ��˿˵���Ŀ
	//BYTE					m_iMeCardList[45];			//�Լ��˿��б�
	//BYTE					m_iBackCard[8];				//�����˿�
	//������Ϣ
// 	BYTE					m_DeskCard[PLAY_COUNT][10];			//ÿ��������˿�
// 	BYTE					m_iDeskCardCount[PLAY_COUNT];		//ÿ�������˿˵���Ŀ
// 	BYTE					m_UserCard[PLAY_COUNT][10];			//�û��˿�

	int						m_iUserStation[PLAY_COUNT];						//��¼�û�״̬(��ʶ����Ƿ���;����� ����һֱ�������)
	BYTE					m_DeskReadyCard[PLAY_COUNT][SH_USER_CARD];		//����˿�������(�߼�λ��)
	BYTE					m_iDeskCardCount[PLAY_COUNT];					//����˿�������(�߼�λ��)
	BYTE					m_byOpenCardCount[PLAY_COUNT];					//�Ѿ�Ҫ�Ƶ�����


	BYTE                    m_bSendCard[PLAY_COUNT];		//��������
	BYTE					m_byNtStation;				//ׯ��λ��

	//������Ϣ
	BYTE				    	m_bCurrentOperationStation;		//�F�ڽз���
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
	int							m_bCurrentClockLabel;			//��ǰʱ�ӱ�ǩ
	int							m_iCurrentClockControlID;		//��ǰ���Ƶ�ʱ��ID

	BYTE					m_iGameNoteSetType;				//�Ƿ�����ţţ�����е����͵����ֵ 
	int						m_iGameTimes;					/**<��Ϸ����*/					
	bool					m_bLeftOrGoOn;					/**<��Ϸ���������뿪���Ǽ�����Ϸ true:������Ϸ false:�뿪*/
	BYTE					m_byGameTimes;					/**<���پ���Ϸ���뿪��Ϸ*/
	BYTE					m_byLeaveThinkTime;				/**<�뿪˼��ʱ��*/
	int						m_iProNote[4];					/**<�ĸ�����ֵ ��ע���� �� Ϊ 100%*/
	//�ؼ�����
public:
	CUpGradeGameLogic		m_Logic;					//�����߼�
//	CClientPlayView			m_PlayView;					//��Ϸ��ͼ
	//��������
	HWND					m_hMciWnd1;						//��������(����)
	HWND					m_hMciWnd2;						//��������(����)
	BYTE                    m_GameType;

	bool					m_bQuick;							//�����˿��ٲ�������
	bool					m_bMiddForQuick;					//�����˿��ٲ������� �� ��ʱ�洢����

	bool					m_bUseMilliSecond;					///ʹ�ú���Ϊ��λ
	int						m_iMilliSecond;						///ʹ�ú���Ϊ��λ

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
	virtual bool SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUseMillSecond = false) ;
#ifdef VIDEO
	/// Ϊ��Ƶģ�����ش˺���
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	virtual bool AFCCloseFrame();
#endif 

	
public:
	//��һ����������������ִ�д˺���
	void HandleStartStation(void *buffer);
	//��ׯ״̬
	void	HandleRobNtStation(void *buffer);
	//��ע״̬
	void	HandleNoteStation(void *buffer);
	//����״̬
	void	HandleSendCardStation(void *buffer);
	//��Ϸ������
	void	HandleTakeCardStation(void *buffer);

	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	LRESULT OnHitCallScore(WPARAM wparam,LPARAM lparam);
	LRESULT OnHitNote(WPARAM wparam,LPARAM lparam);
	//�û���ע��Ϣ
	LRESULT OnHandleHitBet(int iHitButton);
	//Ҫ����Ϣ
	LRESULT OnHandleHitTakeCard();
	//ͣ����Ϣ
	LRESULT OnHandleHitStopCard();

	//������ע���
	LRESULT OnHandleBetResult(void *buffer);
	//NT ��ť����
	LRESULT	OnHitNt(WPARAM wparam, LPARAM lparam);
	//�з�
	LRESULT	OnCallScore(WPARAM wparam, LPARAM lparam);

	//�����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);
	//�����˿˰�ť����
	LRESULT	OnMouseMove(WPARAM wparam, LPARAM lparam);
	//�����˿˰�ť����
	LRESULT	OnMouseLeft(WPARAM wparam, LPARAM lparam);

	//�������ư�ť����
	//LRESULT	OnHitOutCard(WPARAM wparam, LPARAM lparam);
	//�������ְ�ť����
	//LRESULT	OnHitLastTurn(WPARAM wparam, LPARAM lparam);
	//���������ư�ť����
	//LRESULT	OnHitLookBack(WPARAM wparam, LPARAM lparam);
	//�������°�ť����
	LRESULT	OnHitHaveThing(WPARAM wparam, LPARAM lparam);
	//�����뿪������
	LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	//�û������뿪
	LRESULT	OnArgeeUserLeft(WPARAM wparam, LPARAM lparam);
	//�Ҽ�����
	//LRESULT	OnRightHitCard(WPARAM wparam, LPARAM lparam);
	//��ǰ������ť
	LRESULT	OnStop(WPARAM wparam, LPARAM lparam);
	//������ǰ����
	LRESULT	OnStopThing(WPARAM wparam, LPARAM lparam);
	//ͬ����ǰ����
	LRESULT	OnAgreeStop(WPARAM wparam, LPARAM lparam);
	//������ť
	//LRESULT OnHitPass(WPARAM wparam, LPARAM lparam);
	//�û�������
	//LRESULT OnHitActiveResult(WPARAM wparam, LPARAM lparam);
	//�й�
	LRESULT OnHitAuto(WPARAM wparam,LPARAM lparam);
	//��ʾ�û���ע����
	//LRESULT OnShowNote(WPARAM wparam,LPARAM lparam);
	
	//���ƶ�����ɺ���ʾ������
	LRESULT SendCardShowCard(WPARAM wparam,LPARAM lparam);
	//������ע�б�
	BOOL UpdateNtList();

	void ResetGameStation(int iGameStation);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	//	afx_msg void OnClose();
protected:
	virtual void OnCancel();
	//virtual BYTE ViewStation(BYTE bDeskStation);
	//��ʾ��ע��ť,iAddNote��ʾ���������ע��.bTimes��ʾ��ע����
	//	BOOL ShowHandAddNote(int iNowBigNote=0,BYTE bTimes=1,BYTE iNowBigNoteStyle=0,BOOL bExtVal=false);
	//��������
	//BOOL SetPlaySound(BYTE bSoundStyle,BOOL bExtVal=false);
	//��������
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);
	//�޸��û�Ѻע����
	BOOL ModifyNote(BYTE iViewStation,int iCurNote,BOOL bExtVal=false);
	//����Ѻע����
	//BOOL SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy = true);
	//������������ע
	BOOL UserSupersedeNote(BYTE bDeskStation);
	////�������������O��
	//BOOL SetOutCardTypeSound(BYTE iCardList[],int iCardCount,BOOL IsBoy = true);
	////����mp3����
	//BOOL PlayOutCardType(TCHAR FileName[],HWND hmciwnd);
	////���볡������
	//BOOL SetSenceSound(BYTE iSenceSound = 0,BOOL IsBoy = TRUE,int iVal = -1);
public:
	//�����͑��˓Q����
	LRESULT ChangeBackCard();
	//�Q���ƽY��
	BOOL ChangeBackCardResult(BYTE bDeskStation,BYTE bSourceBackCard,BYTE bResultCard,BOOL bExtVal=false);
	//�Qϵ�y��
	BOOL ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard);
public:
	int GetFrontPlay(int NowPaly);
	//ȥ������ָʾ�Ʊ�ʾ
	void DelShowThis();
	//���ؽ�ׯ��ť
	BOOL HideButton();
	//�з�
	void ShowCallScoreBt(int CallScore,BOOL bExtVol=false);
	//BOOL GetBull(BYTE iCardList[], int iCardCount, BYTE iBullCard[]);
};
