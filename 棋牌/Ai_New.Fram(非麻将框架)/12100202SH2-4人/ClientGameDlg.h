#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpGradeLogic.h"

#define MAST_PEOPLE				-1							/**< �Ƿ�Ϊ����Ա */
//��Ϣ����
#define IDM_BEGIN				WM_USER+120					/**< ��ʼ��ť��Ϣ */
#define IDM_HAVE_THING			WM_USER+127					/**< ����Ҫ�� */
#define IDM_SEND_THING			WM_USER+128					/**< �����뿪���� */
#define IDM_ARGEE_LEAVE			WM_USER+129					/**< ͬ���뿪��Ϣ */
#define IDM_STOP				WM_USER+130					/**< ��ǰ������ť */
#define IDM_STOP_THING			WM_USER+131					/**< ������ǰ���� */
#define IDM_ARGEE_STOP			WM_USER+132					/**< ͬ����ǰ���� */
#define IDM_PASS				WM_USER+133					/**< ������ť��Ϣ */
#define IDM_AUTO				WM_USER+134					/**< �й���Ϣ */
#define	IDM_ACTIVE_RESULT		WM_USER+136					/**< �û������� */
#define IDM_SHOW_NOTE			WM_USER+137					/**< �û�ѡ����ע,��ע */
#define IDM_SELECT_NOTE_NUM		WM_USER+138					/**< �û���ע */
#define	IDM_SOUND				WM_USER+139					/**< �û��������� */
#define IDM_BGSOUND				WM_USER+140					/**< �������� */
//colin
#define IDM_LOCK				WM_USER+141                 /**< ������Ϣ */
//��ʱ�� ID
#define ID_BEGIN_TIME			100							/**< ��ʼ��ʱ�� */
#define ID_RESULT_TIME			101	
#define ID_OUT_CARD				103							/**< ���Ƶȴ�ʱ�� */
#define ID_LOOK_BACK			105							/**< ������ */
#define ID_CHANGE_MUSIC			106							/**< ���������� */
//���ú�������
#define RS_ALL_VIEW				1							/**< �������в��� */
#define RS_GAME_CUT				2							/**< �û��뿪���� */
#define RS_GAME_END				3							/**< ��Ϸ�������� */
#define RS_GAME_BEGIN			4							/**< ��Ϸ��ʼ���� */
#define GAME_RAND_TIME			5							/**< ϵͳʱ�� */

/// ��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	//��������
public:
	BYTE					m_iThinkTime;					/**< ������Ϸ���� */
	BYTE					m_iBeginTime;					/**< ��Ϸ��ʼʱ�� */
	int						m_iBeenPlayGame;				/**< ��Ϸ���� */

	int						m_iRoomBasePoint;				/**< ���䱶�� */
	int						m_iRunPublish;					/**< ���ܿ۷� */
	__int64					m_iLimitNote;					/**< ����޶�ע */
	__int64					m_iBaseNote;					/**< ��ע */
	__int64					m_iThisTurnLimit;				/**< ������ע */
	BYTE					m_iCardCount;					/**< ������Ϸ���˿���Ŀ */
	BYTE					m_iAfterWaitAction;  			/**< ��ʼ����ʱ������Ķ��� 1 �Զ���ʼ 2 �Զ��뿪 0900311a1 JJ */
	//������Ϣ
	BYTE					m_iDeskCardCount[4];			/**< ÿ�������˿˵���Ŀ */
	BYTE					m_DeskCard[4][5];				/**< ÿ��������˿� */
	BYTE					m_UserCard[4][5];				/**< �û��˿� */
	//������Ϣ
	int						m_iNowOutPeople;				/**< ���ڳ����� */
	BYTE					m_iFirstOutPeople;				/**< ��һ�������� */
	__int64					m_iNowBigNote;					/**< ��ǰ���Ѻע�� */
	__int64					m_iThisGameNote[4];				/**< ������Ϸ��ע�� */
	__int64					m_iThisTurnNote[4];				/**< ������ע */
	BYTE					m_iThisNoteTimes;				/**< Ѻע���� */
	BYTE					m_bTimeOutCount;				/**< ��ʱ */
	BYTE					m_bShowAnimalStation;			/**< ������ʾλ�� */
	int						m_iUserStation[PLAY_COUNT];		/**< ��¼�û�״̬ */
	TCHAR					m_szName[PLAY_COUNT][61];		/**< ����ǳ� */
	int						m_iDoublePointTime[PLAY_COUNT];	/**< ˫��ʱ�� */
	int						m_iiProtectTime[PLAY_COUNT];	/**< �����ʱ�� */

	bool					m_bSeeCardAnimal;				/**< colin�ܷ񲥷Ŷ��� */
	int                     m_desk;							/**< colin���Ŷ�����λ�� */
    UINT                    m_uKanID;						/**< ���ƶ���ID�� */

	__int64                 m_TempNote[PLAY_COUNT];        /**< colin090418��ҵ�����ע�� */
	__int64                 m_iTempThisTurnNote[PLAY_COUNT];
	//CClientPlayView			m_PlayView;						/**< ��Ϸ��ͼ */
	bool					m_bUserState[PLAY_COUNT];		/**<���״̬(������Ϸ��ʼ�������ɽ��빦��ʱ��ӵ����ݳ�Ա)*/	
	__int64					m_iPerJuTotalNote[PLAY_COUNT];	/**< �û�ÿ��ѹ��ע ����ռ���������ʾ��ע����ʵ����ע������������ xyh */
	BYTE					m_iOutCardPeople;				/**< ���ڳ����û� */				

	//�ؼ�����
private:		
	int						m_iVersion;
	int						m_iVersion2;

	//��������
	//HWND					m_hMciWnd1;						/**< ��������(����) */
	//HWND					m_hMciWnd2;						/**< ��������(����) */
	BYTE					m_byUserFollow;					/**< �ڼ��ѱظ� */
	BYTE                    m_byCurStyle;           	    /**<  ��ǰ��ע���� */
	bool					m_bCanHitLock;					/**< colin�ܷ����� */

	BOOL					m_bNetCutPlayer[PLAY_COUNT];

	BYTE                    m_iSendAnimaCardPos;     ///���ƶ���λ��
	int                     m_iSendCardTotle;         ///���Ƶ�����
	int                     m_iNeedSendCardTotle ; 
	BYTE                    m_bShowHa;               ///�ڼ��ſ�����

	bool					m_bIsSuperUser;		//�ж��Լ��Ƿ񳬼��ͻ��ˣ��ɷ������������ݣ�ÿ�ֶ�У��//lym 2011-11-28
  	BYTE					m_byAddNum;												/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
	BYTE                    m_bPK;                ///�Ƿ�PK
	CUpGradeGameLogic		m_Logic;					//�����߼�
	bool					m_bChangeRobotCard;			/**<�Ƿ�ı��˻����˵��� true �ı��� false û�иı�  */
	int						m_iGameTimes;				/**<��Ϸ����*/	
	bool					m_bLeftOrGoOn;				/**<��Ϸ���������뿪���Ǽ�����Ϸ true:������Ϸ false:�뿪*/
	BYTE					m_byGameTimes;				/**<���پ���Ϸ���뿪��Ϸ*/
	BYTE					m_byLeaveThinkTime;			/**<�뿪˼��ʱ��*/
	//��������
public:
	/// ���캯��
	CClientGameDlg();
	/// ��������
	virtual ~CClientGameDlg();
    int GetGameStatus();

protected:
	/// ��ʼ������
	virtual BOOL OnInitDialog();
	/// ���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	/// �Թ۸���
	virtual void OnWatchSetChange(void);
	virtual void OnGameSetting();

	//���غ���
public:
	/// ������Ϸ״̬
	/// @param pStationData ״̬����
	/// @param uDataSize ���ݰ���С
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	
	/// ��Ϸ��Ϣ������
	/// @param pNetHead ��Ϣͷ
	/// @param pNetData ��Ϣ����
	/// @param uDataSize ��Ϣ��С
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	
	/// �������ý���
	virtual void ResetGameFrame();
	
	/// ��ʱ����Ϣ
	/// @param bDeskStation ���ID
	/// @param uTimeID ��ʱ��ID
	/// @param uTimeCount ��ǰ��������
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	

	/// ������ж�ʱ��
	void KillAllTimer();
	/// ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	/// ��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};
	
	/// �û��뿪
	/// @param bDeskStation ���ID
	/// @param pUserItem �û���Ϣ�ṹ
	/// @param bWatchUser �Ƿ��Թ�
	virtual bool GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	/// �ܷ��뿪����
	virtual BOOL CanLeftDesk();
    /// colin090414�õ����ֵ
	__int64 GetMaxNote(__int64 a[4]);
	/// colin090511��ֵת��ΪFLASH����ֵ
	BYTE GetFlashCard(BYTE iCard);
	/// ��ת
	BYTE LogicStation(BYTE deskStation);

	//��Ϣ����
public:
	

	
	/// ��ʼ��ť����
	/// @param wparam ����
	/// @param lparam ����
	/// @return LRESULT ����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	
	/// �����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);
	/// �����˿˰�ť����
	LRESULT	OnMouseMove(WPARAM wparam, LPARAM lparam);
	/// �����˿˰�ť����
	LRESULT	OnMouseLeft(WPARAM wparam, LPARAM lparam);
	/// �������°�ť����
	LRESULT	OnHitHaveThing(WPARAM wparam, LPARAM lparam);
	/// �����뿪������
	LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	/// �û������뿪
	LRESULT	OnArgeeUserLeft(WPARAM wparam, LPARAM lparam);
	/// ��ǰ������ť
	LRESULT	OnStop(WPARAM wparam, LPARAM lparam);
	/// ������ǰ����
	LRESULT	OnStopThing(WPARAM wparam, LPARAM lparam);
	/// ͬ����ǰ����
	LRESULT	OnAgreeStop(WPARAM wparam, LPARAM lparam);
	/// �û�������
	LRESULT OnHitActiveResult(WPARAM wparam, LPARAM lparam);
	/// �й�
	LRESULT OnHitAuto(WPARAM wparam,LPARAM lparam);
	/// ��ʾ�û���ע����
	LRESULT OnShowNote(WPARAM wparam,LPARAM lparam);
	/// �û���ע��Ϣ
	LRESULT OnSelectNote(WPARAM wparam,LPARAM lparam);	
	/// NT ��ť����
	LRESULT	OnHitNt(WPARAM wparam, LPARAM lparam);
	/// ���ƶ�����ɺ���ʾ������
	LRESULT SendCardShowCard(WPARAM wparam,LPARAM lparam);
	/// ������ע�б�
	BOOL UpdateNtList();
	/// ������Ϸ
	/// @param iGameStation ����ʲô״̬
	void ResetGameStation(int iGameStation);	
	/// colin09423����
	LRESULT	OnHitLock(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

public:
	/// �˳�
	virtual void OnCancel();
	
	/// ���汾����������汾�Ƚϣ�
	/// @param iVersion �Ͱ汾
	/// @param iVersion2 �߰汾
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);
	
	/// �޸��û�Ѻע����
	BOOL ModifyNote(BYTE iViewStation,__int64 iCurNote,BOOL bExtVal=false);
	/// �����ע
	BOOL UserNote();
	/// ������������ע
	BOOL ReplaceUserNote(BYTE bDeskStation);
public:
	/// ���Ż��㵽��ͼλ��
	virtual BYTE ViewStation(BYTE bDeskStation);
	

};

extern CClientGameDlg * m_pCClientGameDlg;