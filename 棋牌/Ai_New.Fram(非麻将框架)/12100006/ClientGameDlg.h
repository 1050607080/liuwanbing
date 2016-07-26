#pragma once

//#include "ClientView.h"
//#include "../../../../sdk/include/GameLanchSdk/LoveSendClassInExe.h"
#include "UpGradeLogic.h"
//#include "FinishDlg.h"
//#include "../../../../sdk/include/GameLanchSdk/BZAnima.h"    //����Ͷ�ļ�
//#include "OpenSelDlg.h"
//#include "AFCMessageBox.h"
#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա
//��Ϣ����
#define IDM_BEGIN			WM_USER+120					//��ʼ��ť��Ϣ
//#define IDM_USER_NT		WM_USER+121					//�û�����
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
//#define IDM_JIAO_FEN		WM_USER+134					//�зְ�ť��Ϣ
//#define IDM_GIVE_UP_FEN		WM_USER+135					//���а�ť��Ϣ
#define IDM_AUTO			WM_USER+134					//�й���Ϣ
#define	IDM_ACTIVE_RESULT	WM_USER+136					//�û�������
#define IDM_SHOW_NOTE		WM_USER+137					//�û�ѡ����ע,��ע
#define IDM_SELECT_NOTE_NUM	WM_USER+138					//�û���ע
#define	IDM_SOUND			WM_USER+139					//�û���������
#define IDM_BGSOUND			WM_USER+140					//��������
//��ʱ�� ID
#define ID_SET_BOBOTIME				101					//��������ʱ��
#define	ID_FOLLOW_TIME				102					//��ʱ���͸���Ϣ
#define	ID_SET_ALL_TIME				103					//��ʱ��������Ϣ
#define ID_BIG_TIME					104					//��ʱ���ʹ���Ϣ
#define	ID_STOP_TIME				105					//��ʱ��������Ϣ
#define	ID_GIVE_UP_TIME				106					//��ʱ���Ͷ���Ϣ
#define	ID_SEPARATE_TIME			107					//��ʱ���ͷ�����Ϣ
#define ID_BEGIN_TIME				108					//��ʼ��ʱ��
//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����

//��������̎��Ĉ�����
#define SOUND_WIN					0x01				//ʤ��
#define SOUND_LOSE					0x02				//��
#define SOUND_WAITING				0x03				//�ȴ�
#define SOUND_ERROR					0x04				//���Ƴ�����
#define SOUND_READY					0x05				//׼������
#define SOUND_START					0x06				//����һ����
#define SOUND_JIAO_FEN				0x07				//�з�
#define SOUND_OUT_CARD_FINISH		0x09				//ĳ��ҳ��ƽY��
#define SOUND_GAME_PASS				0x0A				//����
#define SOUND_FRIEND_APPEAR			0x0B				//���ѳ��F
#define SOUND_USER_RUNAWARY			0x0C				//��������
#define SOUND_USER_LEFT				0x0D				//�����뿪
#define SOUND_GIVE_BACK				0x0E				//���

//����������

#define SOUND_SEND_CARD				0x22					//����
#define SOUND_WARNING				0x23					//����
#define SOUND_INVALIDATE			0x24				//��ʾʧ��

#define SOUND_SHUTTLE				0x11					//���
#define SOUND_FOLLOW				0x12					//��
#define SOUND_ADD					0x13					//��
#define SOUND_NOTE					0x14					//��ע
#define SOUND_GIVE_UP				0x15					//����


#define MAX_FLASH_COUNT             5               //flash������

#define FLSAH_CLOCK_ID           2    //ʱ�ӵ�id��Ϊ2

#define					NOTE_NUM				18

static int arrNoteEnun[NOTE_NUM] = 
{
	1,2,5,
	10,20,50,
	100,200,500,
	1000,2000,5000,
	10000,20000,50000,
	100000,200000,500000
};

//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	enum
	{
		FLASH_ID_CLOCK_INDEX = 0,       //���Ӷ���id			
		FLASH_ID_NT_INDEX=1 ,             //ׯ��flash��������id
		FLASH_ID_LOOK_CARD_IDNEX=2,      //���ƶ�������
		FLASH_ID_OPEN_CARD_INDEX = 3,   //���ƶ���id
		FLASH_ID_SEND_CARD_INDEX = 4  //���ƶ���id
	};
	//�̶���Ϣ
	//=====================ʱ�����Ϣ===================
	int						  m_iBoboPercentage;				//�������������������Ǯ�İٷ�֮��
	CUpGradeGameLogic		  m_Logic;							//�����߼�
	int						  m_iAfterWaitAction;
	BYTE                         m_bGameStation;               ///<��Ϸ״̬
	///����״̬����
	BYTE					  m_iUserCardCount[PLAY_COUNT];					///<�û������˿���Ŀ
	BYTE					  m_iUserCard[PLAY_COUNT][MAX_CARD_COUNT];		///<�û����ϵ��˿�
	BYTE					  m_iDeskCardCount[PLAY_COUNT];					///<�����˿˵���Ŀ
	BYTE					  m_iDeskCard[PLAY_COUNT][MAX_CARD_COUNT];		///<������˿�
	///��Ϸ��������
	BYTE                      m_iBeginTime;                     ///<��Ϸ��ʼʱ��
	BYTE					  m_byBoBoTime;						///��������ʱ��
	BYTE                      m_iThinkTime;                     ///<��Ϸ˼��ʱ��
	BYTE					  m_bySeparateCardTime;				///����ʱ��
	BYTE                      m_iEndWaitTime;                   ///<��Ϸ������ȴ�ʱ��
	int                       m_iRunPublish;                    ///<���ܿ۷���
	BYTE					  m_byGameStation;					//��Ϸ��ǰ��״̬()
	int                       m_iCurrentTimeId;                 ///��ǰ��ʱ��ID
	BOOL                      m_bSound;                        ///�Ƿ�����Ч
	int                       m_TimeClockCount;                ///���ӵ���ʱʱ��
	bool					  m_bInLisureRoom;					//����Ϸ�Ƿ������з�
	BYTE					  m_iThisNoteTimes;						//Ѻע����
	__int64                   m_i64FrontNote;						//��ǰ��Ч��ע����ǰλ�����Ч��ע��;
	__int64                   m_i64MyAddNote;						//�Ҽ�ע��ֵ 
	int						   m_iFirstOutPeople;					//��һ��������
	int						   m_iNowOutPeople;						//���ڳ�����
	BYTE                       m_bNtPeople;							//ׯ��
	BYTE                       m_bSendCardPos;						//��ǰ���ƶ�������
	BYTE                       m_bSendCardCount[PLAY_COUNT];		//��ǰ���ƶ���������
	bool                       m_bPlayer[PLAY_COUNT];				//�Ƿ���һ���;�����ս��
	bool					   m_bUserReady[PLAY_COUNT];			//���׼����־λ
	int						   m_iMyLogDesk;						//��ҵ�λ��

	__int64					   m_i64Bobo[PLAY_COUNT];				//������
	__int64					   m_i64NoteMoney[PLAY_COUNT][3];		//����ÿ����ע����Ϣ
	__int64					   m_i64UseAllNote[PLAY_COUNT];			//�û����µ���ע
	//������Ϣ
	__int64					   m_i64BoBase;									//����
	__int64					   m_i64SmallBobo;								//��С����
	__int64					   m_i64TempBobo;								//����������ʱ����
	BYTE					   m_byUserOperatorType[PLAY_COUNT];			//��ҵĲ���
	int							m_iCardShowCount;							//Ҫ��ʾ��������
	BYTE						m_byOneOparateHand[MAX_CARD_COUNT];			//��һ���ƿɲ����ؼ�����
	int							m_iOneOparateHandCount;						//�Ƶ�����
	BYTE						m_byTwoOparateHand[MAX_CARD_COUNT];			//�ڶ����ƿɲ����ؼ�����
	int							m_iTwoOparateHandCount;						//�Ƶ�����
	BYTE						m_byNoteTime[PLAY_COUNT];					//�ڼ����³���
	
	BYTE						m_bySeparatorCard[MAX_CARD_COUNT];			//Ҫ�ֵ���

	bool						m_bSepartorFlag;							//�Ƿ��Ѿ����Է�����
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();
	
	//�����Ϸ�汾	
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

public:	//--------------------------�����Ľ���-------------------------	
	
	//������ж�ʱ��
	void KillAllTimer();
	// �����Ƿ��Թ���ʾ����ť
	void ShowStartButtons();
	// ����������ϵ��˿���
	//void SetHandCard(BYTE bDeskStation, BYTE *pCard,int nCount, BYTE *pUpList=NULL,int nUpCount=0);	
	//�����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);	
	//ͬ�⿪ʼ��Ϸ 
	bool OnControlHitBegin();

	//--------------------------���潻����������ť���ƺ���---------------------

	BYTE GetViewStaionToCtr(BYTE bDeskStation);
	//-----------------------------------��ť����--------------------------------------
    bool OnCloseGame();           //�ر���Ϸ
	//--------------------------------���غ���-----------------------------------------
     virtual void OnCancel();//{OnCloseGame(); __super::OnCancel();};
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	//��ͼת��
	virtual BYTE ViewStation(BYTE bDeskStation);
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������ý���
	virtual void ResetGameFrame();
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	//�û��뿪
	//virtual bool GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	/// Ϊ��Ƶģ�����ش˺���
	//virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};
	//�رմ���
	virtual bool AFCCloseFrame();
	//������Ϸ״̬
	virtual void ResetGameStation(int iGameStation=1);
	//�ػ���Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    //��ʼ������
	virtual BOOL OnInitDialog();
	//-----------------------------------------------------------------------�����Ѿ�ʵ�֣�ZXD
	// ��ȡ��Ƶ���ھ���
	BOOL GetVideoRect(CString  strNickname, RECT& rect, CString& pic){return FALSE;};
	// ������Flash����ʹ�õĻص�����
	static void FlashEndMessage(CWnd * pWnd,UINT uID,CString msgString);
	//----------------------------------------ԭ��view�µ�һЩ����----------------------------
	//������ע
	void SetTotalNote(BYTE iViewStation,int iNoteTotal);
	//�����߼������ƣ�ת����flash����
	int GetFlashCard(int iCard);
	int GetTotalPlayNum();  //�õ�������ҵ�����
	//���������
	int MyRand();
//================================================================================================
public:
	void LoadIniConfig();
	void SetBoboTime( void * pNetData, UINT uDataSize);				//����������
	void SendBoboData();											//������������
	void Ass_SetBobo_Data(void * pNetData, UINT uDataSize);			//������յ�����������
	void Ass_Send_Card(void * pNetData, UINT uDataSize);			//������շ��Ƶ�����
	void OnHandleGameBegin(void * pNetData, UINT uDataSize);		//��ʼ����Ϸ��Ϣ
	int CheckFrontUserOperator();									//���֮ǰ�������ʲô������


	void Bnt_Follow();												//������
	void Bnt_Set_All();												//�ò���
	void Bnt_Big();													//�����
	void Bnt_Stop();												//�ݲ���
	void Bnt_Give_Up();												//������
	void SendSeparatorCardData();									//���ͷ�����Ϣ
	void HavaFollow(BYTE byCardType);									//����ѡ���
	void HaveSetAllNote(BYTE byCardType);								//����ѡ����
	void HaveSetBigNote(BYTE byCardType);								//����ѡ���
	void NoOneOperator(BYTE	byCardType);								//��û���˲�����ǰ�������ݻ�
	void OnHandleBet(void * pNetData, UINT uDataSize);					//�����û����µ�ע��Ϣ
	void Ass_Sc_Send_One_Card_Finsh(void * pNetData, UINT uDataSize);	//��һ���ƽ�����
	void Ass_Sc_Notice_Send_Card(void * pNetData, UINT uDataSize);		//֪ͨ����
	int  GetMaxCardShap(int iCardNum,BYTE byOperator);					//��ȡ��ǰ����������
	void Ass_Sc_Notic_Separe_Card( void * pNetData, UINT uDataSize);	//������տɷ�����Ϣ
	void GetSeparatorCardShap(int iMyDesk);
	void SelectOperator(int iOperatorType,BYTE byCardType);				//Ҫ����Ĳ�������
};
