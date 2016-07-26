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
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_OUT_CARD					101					//���Ƶȴ�ʱ��
#define ID_LOOK_BACK				102					//������
#define ID_CHANGE_MUSIC			    103					//����������
#define ID_BEGIN_ANIMAL				104				//��ʼ����
#define ID_NOTE_ANIMAL				105				//��ע����
#define ID_SEND_CARD_ANIMAL			106				//���ƶ���
#define ID_COLLECT_JETON_ANIMAL		107				//�ռ�����



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

class AI_CanOperate 
{

public:
	AI_CanOperate()
	{
		Init();

	}
	~AI_CanOperate()
	{
	}
	void Init()
	{
		bCanAdd = false;
		bCanNote = false;
		bCanFlollow = false;
		bCanGiveUp = false;
		bCanLook = false;
		bCanOpen = false;
		iNmuBtnEnableCount = -1;
	}
	void SetAIStation(bool bOpen,bool bAdd,bool bGiveUp,bool bFlollow,bool bLook,bool bChipIn)
	{
		bCanAdd = bAdd;
		bCanNote = bChipIn;
		bCanFlollow = bFlollow;
		bCanGiveUp = bGiveUp;
		bCanLook = bLook;
		bCanOpen = bOpen;
	}
	
	bool GetCanAdd() const {return bCanAdd;}
	bool GetCanNote()const {return bCanNote;}
	bool GetCanFlollow()const {return bCanFlollow;}
	bool GetCanGiveUp()const {return bCanGiveUp;}
	bool GetCanLook()const {return bCanLook;}
	bool GetCanOpen()const {return bCanOpen;}

	void SetNmuBtnEnableCount(int iCount = -1){iNmuBtnEnableCount = iCount;}
	int  GetNmuBtnEnableCount() const {return iNmuBtnEnableCount;}
protected:
	bool bCanAdd;
	bool bCanNote;
	bool bCanFlollow;
	bool bCanGiveUp;
	bool bCanLook;
	bool bCanOpen;

	int iNmuBtnEnableCount;
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
	BYTE					m_iThinkTime;				//������Ϸ����
	BYTE					m_iBeginTime;				//��Ϸ��ʼʱ��
	int						m_iRoomBasePoint;			//���䱶��
	int						m_iRunPublish;				//���ܿ۷�
	//	BYTE					m_iBeginStation;			//��ʼλ��
	//	BYTE					m_iEndStation;				//����λ��
	BYTE					m_iCardCount;				//������Ϸ���˿���Ŀ
	BYTE					m_iPalyCardCount;			//��Ϸ�˿���Ŀ
	
	//������Ϣ
	int						m_iLimitNote;				//����޶�ע
	int						m_iBaseNote;				//��ע
	int                     m_iGuoDi;                   //����
	int                     m_iLimitPerNote;            //ÿע���ƣ�Ϊ��ע�����ƣ�����ʾΪ��ע������
	
	int						m_iThisTurnLimit;			//������ע
	BYTE					m_iAfterWaitAction;  	//��ʼ����ʱ������Ķ��� 1 �Զ���ʼ 2 �Զ��뿪 0900311a1 JJ

	//������Ϣ
	BYTE					m_iDeskCardCount[PLAY_COUNT];		//ÿ�������˿˵���Ŀ
	BYTE					m_DeskCard[PLAY_COUNT][5];			//ÿ��������˿�
	BYTE					m_UserCard[PLAY_COUNT][5];			//�û��˿�
	
	int						m_iNowOutPeople;			//���ڳ�����
	
	int						m_iFirstOutPeople;			//��һ��������

	BYTE                    m_bNtPeople;   //ׯ��
	bool                    m_bFirstGame;
	
	
	int						m_iNowBigNote;				//��ǰ���Ѻע��
	int						m_iThisGameNote[PLAY_COUNT];			//������Ϸ��ע��(��ע)	
	int						m_iTotalGameNote[PLAY_COUNT];			//������Ϸ��ע��(ʵ��)

	
	BYTE					m_iThisNoteTimes;			//Ѻע����
	int                     m_iFrontNote;                  //��ǰ��Ч��ע����ǰλ�����Ч��ע��;
	int                     m_iMyAddNote;                //�Ҽ�ע��ֵ 
	
	BYTE					m_bTimeOutCount;			//��ʱ

	BOOL					m_bBgSound;					//��������
	BYTE					m_bShowAnimalStation;		//������ʾλ��
	BOOL                    m_Ming[PLAY_COUNT];          //����

	bool                    m_bOpenLose[PLAY_COUNT];     //��ҵı��ƽ��
	bool                    m_bIsGiveUp[PLAY_COUNT];     //����Ƿ����

	BYTE                    m_bReachLimitStation;


	BYTE                    m_iComWinStation;
	BYTE                    m_iComLoseStation;


	int                     m_iOldCardCtrlDistH;        //ԭ���ƿؼ��ĺ�����
	int                     m_iNewCardCtrlDistH;
	//�ؼ�����

	
	
	

	CUpGradeGameLogic		m_Logic;					//�����߼�
	//CClientPlayView			m_PlayView;					//��Ϸ��ͼ
	int						m_iVersion;
	int						m_iVersion2;

	//��������
	HWND					m_hMciWnd1;						//��������(����)
	HWND					m_hMciWnd2;						//��������(����)

	//int                     m_iWaitForGroupID;

	//--------------------------------------ԭ��view�µ�һЩ����---------------------------------

	int								m_iNoteTotalPoint;					//��ע
	int								m_iNoteLimit;						//��������
	BOOL                            m_bMing[PLAY_COUNT];                        //����
	//CFinishDlg				        m_FinishDlg;					//�����
	//COpenSelDlg                     m_SelOpenDlg;          //ѡ���ƶ໭��
	//CAFCMessageBox                  m_MsgBox;

	UINT                            m_arrUFlashID[MAX_FLASH_COUNT];


	int                             m_arrCartoonCount[PLAY_COUNT];

	bool                            m_bGameFinishAndComp;


	bool m_bWatchMode;

    int                             m_mingPaiNum;                       //���ڼ�¼�Լ����ƺ�Ĳ�������

    double                          m_shenglv;


	BYTE							byUserData[PLAY_COUNT][2];	// ������ݣ��Ƿ�����ˣ��ƴ�С����
	BYTE							m_byAddCount;				//���˼���
	BYTE							m_byMaxCount;				//�����Ѻ��ƣ����ֵ��
	BYTE							m_byCtrPet;					//���ưٷֱȣ�������ֵ��
	bool							m_bIsCtrl;					//�����Ƿ���ȫ����
	bool							m_bMaxPaiAI;				//��������Ƿ������
	bool							m_bMaxPaiMe;				//��������Ƿ��Լ�
	
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();
	
	//�����Ϸ�汾	
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);

	//// ��BzMainFrame�̳������ĺ���
//	virtual bool OnPostMessage(UINT pId,BBaseParameter* pParam);


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

	void UpdateShowNoteBtn(bool bOpen=false,bool bAdd=false,bool bGiveUp=false,bool bFollow=false,bool bLook=false,bool bChipIn=false);

	void SetCtrlBtn(bool bIsFirstNote);
	//���ü�ע���ٵİ�ť״̬
	bool UpdatAddNumBtn(bool bFirst);

	//void DrawUserInfo();
	//�������е���Ϸ������ť
	void DisableAllOperaBtn();
	//
	void EnableAddOrNote();

	//���������Ϣ
	//void SetUserInfo(BYTE bDeskStation, UserItemStruct * pUserItem,bool bIsCome);
	//�����Լ�����Ϣ
	//void UpDateMyUserInfo();
	//�����Լ��Ŀ������
	//void UpdateMyLeftMoney();
	//�����Լ��ķ���
	//void UpdateFinshiPoint(int iAdd);
	//���÷�����Ϣ
	//void UpdateRoomInfo();
	//���üӳ�����ʾ�Ĳ���
	//void UpdateAddChipShow(int bMutBase);
	//���������ע��Ϣ
	//void UpdateNoteInfo(BYTE bDeskStation);

	//-------------------------------------��ʾ��ز���-----------------------------------
	//���������ʾ��Ϣ
	//void SetTipInfo(BYTE bDeskStation,int iOperateType);
    //�������е���ʾ��Ϣ
	//void DisVisibleAllTip(bool bIsShowGaveUpAndLose=false);  
	//
	//
 //   //�������еļ�ע��ʾ
 // 	void DisVisibleAllAddTip();
	////�������е���ע��ʾ
	//void DisVisibleAllChipinTip();
	////�������еĸ�ע��ť
	//void DisVisibleAllFollowTip();
	////�������еķ�����ʾ
	//void DisVisibleAllGiveUpTip();    //������ʾ
	//void DisVisibleAllOpenLoseTip();  //����ʧ����ʾ
	//void DisVisibleAllLookTip();      //������ʾ
	//void DisVisibleReachLimiteTip();  //�ﵽ������ʾ
	//void DisVisibleAllOpenWinTip();     //�������еı���ʤ����ʾ
	
	//void ClearAllUserInfo();
	//void ClearAllInfo();   //��ս��������е���ʾ��Ϣ

	//void InvalidateAllTipCtrl();

	//-----------------------------------------------------------------
	//--���Ŷ�������
	//void PlayFlash(int iIndex,BYTE bDeskStation1=255,BYTE bDeskStation2=255);   //���Ŷ����������ź͸��Ǹ���Ҳ��ţ������λ��Ϊ255����Ϊȫ�嶼�еĶ���
	//UINT PlaySendCard();
	//UINT PlayLookCard(BYTE bDeskStation);
	//UINT PlayOpenCard(BYTE bDeskStation1,BYTE bDeskStation2,int iCmpRet);
	//UINT PlayClock(BYTE bDeskStation,int iThinkTime);
	//UINT PlayNt(BYTE bDeskStation);

	//����flash��������
//	void OnFinishFlash(FlashMsgData* pData);
	//ֹͣ���еķ�ѭ�����ŵĶ���
	//void DelAllUnReCycFlase();   //ɾ�����еķ�ѭ�����ŵĶ������˴�����ׯ�⣬�����Ķ�ɾ��
	//���ƺ����Ӧ����
	void OnFinshLookCard();
    //���ƶ�������
	void OnFinishOpenCard();  //

	//void PlayWaitGroup(); //���ŵȴ����鶯��
	//void CloseWiatGroup();
	//---------------------------------���Ŷ����������˶�����oyh���ƣ���flash--------------------
	//void PlayNoteCartoon(BYTE bDeskStation,int iNoteNum); //������ע����
	//bool GetStartPoint(BYTE bDeskStation,CPoint& pt);//��עʱ�Ŀ�ʼλ��
	//bool GetNoteEndRect(CRect& rc);   //��עʱ����Ľ�������
	//bool GetCollectEndRect(BYTE bDeskStation,CRect& rc);//�ճ���ʱ�Ľ�������
	//-----------------------------------��ȡ����----------------------

	BYTE GetViewStaionToCtr(BYTE bDeskStation);
    //�õ��ƿؼ�������
	//BBaseContainer::Node* GetCardCtrContainer(BYTE bDeskStation);
	//�õ��ƿؼ�
	//CUpGradeGameCard* GetCardCtrl(BYTE bDeskStation);
	//�õ��û���Ϣ�ؼ������ڵ�
	//BBaseContainer::Node* GetUserCtrlContainer(BYTE bDeskStation);

	//BBaseContainer::Node* GetClockContainer(BYTE bDeskStation);
	//BBaseContainer::Node* GetNtContainer(BYTE bDeskStation);






	//-----------------------------------��ť����--------------------------------------
    bool OnCloseGame();           //�ر���Ϸ
     //��Ϸ��ť����
	bool OnChipIn();   //��ע����
	bool OnFllow();    //��ע����
    bool OnAdd();      //��ע����
	bool OnGiveUp();   //��������
	bool OnLook();     //���Ʋ���  ��Ϊ�������  
	bool OnOpen();     //���Ʋ���

	//��ע���
	bool OnAddCount(int bMutBase);  //���x1,x2,x3��ť
	bool OnAddCancle();              //�����ע�Աߵ�ȡ����ť

	//------------------------------------�����˲���-------------------------------------------
    void AutoAI();             //�����˵��Զ���ע
	void getShengLv();         //��ȡ�Լ��������Ƶ�ʤ�ʣ����������Լ����ƵĽ�����м��㣬���漰���ݶԷ��Ĳ������²�Է���ʵ�����
	bool noPeopleKnow();       //Ŀǰ��û����֪���Լ����ƣ����ƹ�״̬
	int GetAIThinkTime();
	AI_CanOperate m_AiOperateStation;  //��һ����˲���	
	
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
	//������Ϣ
	//virtual void ShowUserTalk(MSG_GR_RS_NormalTalk *pNormalTalk);

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
	//��ע
	void AddTotalNote(BYTE iViewStation,int iNoteTotal);
	
	
	//�滭���Ʊ�־
	void SetMing(BYTE bDeskStation,BOOL iGiveup);
	//---------------------------------------------
	inline int GetLimit() {return (m_iLimitPerNote < m_iLimitNote ? m_iLimitPerNote : m_iLimitNote);}


	//�����߼������ƣ�ת����flash����
	
	int GetFlashCard(int iCard);

	int GetTotalPlayNum();  //�õ�������ҵ�����


	//������ע���������ע����ת���ɿ������еĳ�ı��ʽ  �� 90  ��ת���� 50 + 20 + 20
	int AnalyseNote(int iTotalNote,int arryHaveNote[],BYTE bAnayResult[]);      //��������

	//�����¼ҵ�����  //
	void PalyExtAddSound(int iSoundType);
    
	


	

	//���ñ���Ѻע����
	//void SetLimitNote(BYTE iViewStation,int iNoteLimit,int iBaseNote);
	//���üӳ�����
	//void SetAddJeton(int iBaseNote);
	//���ӱ�����ע��
	//void AddThisTurnNotePoint(BYTE bDeskStation,int iThisTurnPoint);
	//���ñ��������
	//void SetAddNote(int iAddNote){m_iThisTurnAddNote=iAddNote;}

public:

};
