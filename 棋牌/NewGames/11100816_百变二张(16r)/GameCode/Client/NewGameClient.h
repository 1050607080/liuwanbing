#include "IGameImpl.h"
#include "..\GameMessage\UpGradeLogic.h"
#include <irrKlang.h>

using namespace irrklang;

interface IGameFrame;
interface IUserInterface;

///��ʱ��
#define    TIME_GAME_BEGIN                10    ///��ʼʱ��
#define    TIME_GAME_NOTE                 11    ///��עʱ��
#define    TIME_GAME_SEND_CARD            12    ///���ƶ�ʱ�� 
#define    TIME_DICE_ANIMAL               13    ///ɫ�Ӷ�����ʱ��
#define    TIME_XIPAI_ANIMAL              14    ///ϴ�ƶ���
#define    TIME_SEND_CARD_ANIMA           15    ///���ƶ�����ʱ��
#define    TIME_KAI_PAI_ANIMAL            16   //���ƶ�ʱ��
#define    TIME_HAND_ANIMAL               17   //���ֶ�ʱ��
#define    TIME_INTERVAL_TIME             18   //���Ƽ��ʱ�䶨ʱ��
#define    TIMER_FORCE_LEAVE              19   //ǿ���뿪���Ӷ�ʱ�� 
#define    TIMER_WIN_RECT_WINK            20   //�滭Ӯ����˸����


/// ������������					
#define SOUND_WARNING		0x10						/** ���� */
#define SOUND_SEND_CARD		0x20						/** �������� */
#define SOUND_INVALIDATE	0x30						/** ��ʾʧ�� */
#define SOUND_START_DEF		0x40						/** �ĸ��˿�ʼ��Ϸ�� */
#define SOUND_GAME_OVER		0x50						/** ��Ϸ���� */
#define SOUND_WIN           0x60                        /** ʤ�� */
#define SOUND_LOST          0x70                        /** ʧ�� */
#define SOUND_NOTE          0x80                        /** ��ע������ */
#define SOUND_START         0x90                        /** ��Ϸ��ʼ */
#define SOUND_KAIPAI        0xA0                        /** ���� */
#define SOUND_NOTE_OVER     0xB0                        /** ��ע���� */
#define SOUND_1000W         0xC0                        /** 1000W���� */
#define SOUND_100W          0xD0                        /** 100W���� */
#define SOUND_10W           0xE0                        /** 10W���� */
#define SOUND_DICE          0x11                          ///ɫ������ 
#define SOUND_XIPAI         0x12
#define SOUND_NOTE_BG       0x13                       ///��ע��������

#define GetControlNode(A ,IControl , IControlID ) \
	IControl * A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A
#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));

#define SAFECTRL(CTRL_TYPE,pUI,CTRL_ID, OPERATE)\
{\
	CTRL_TYPE * PRE=NULL; \
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\
	if (PRE!=NULL)\
{\
	PRE->OPERATE; \
}\
}
#define SAFE_CTRL_OPERATE(CTRL_TYPE,CTRL_ID, OPERATE)\
{\
	CTRL_TYPE* PRE=NULL; \
	PRE = dynamic_cast<CTRL_TYPE *>(m_pUI->GetIControlbyID(CTRL_ID));\
	if (PRE!=NULL) PRE->OPERATE; \
}

class CNewGameClient : public IGameImpl
{
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);


public:
    /// ��ʼ��
    virtual int Initial();
    /// ��ʼ��UI
    virtual int InitUI();
    /// ��Ϸ��Ϣ
    virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
    /// ������Ϣ
    virtual int OnUIMessage(TUIMessage* pMessage);
    /// 
    virtual int ResetGameFrame(void);
    /// ��ҽ���
    virtual int GameUserCome(void);
    /// ����뿪
    virtual int GameUserLeft(int nIndex); 
    /// ��Ҷ���
    virtual int GameUserOffLine(void);
    // �Ƿ�������Ϸ������Ϸʵ��
    // �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
    virtual bool IsPlayingGame();          
    /// ��ʱ����Ϣ
    virtual void OnTimer(int nTimerID);
    /// ��״̬
    virtual void SetStationParameter(BYTE bGameStation);
	///������Ϸ״̬����Ҫ�Ƕ����������������Ϣ
    virtual void SetGameStation(void * pBuffer,int nLen);

	virtual bool OnGameQuiting();
private:
	///��������UI��״̬
	int  ReSetGameUIStation(); 
	///����������Ϸ�пͻ��˵�����
	int  ResetGameData() ;
	//��ע���
	int EnableNoteFinish() ;
	///���ݵ�ǰ��������ʾ��ע��ť
	int EnableNoteBtnByCondition(bool bShow);
	///���õ�ǰ�Ĺ��
	int SetCurrentCursor(int uID);
	///��ע��ť����ʾ״̬
	int SetChooseRectStation(int uID,bool bShow ,bool  bAllHideAndShow = false ) ; 
	///��ʾׯ����Ϣ
	int ShowNTInfo(bool bShow);
	///��ʾ�����Ϣ
	int ShowPlayerInfo(bool bShow = true); 
	///�����ע�������ʾ
    int ShowUserNoteInfo(bool bShow);
	///��ʾ�����ϵ������Ϣ
	int ShowResultInfo(bool bshow) ;
	///������ҵĽ����
	int  ChangeUserMoney(bool bshow); 
	///��ʾ��ʷ��Ϣ
	int ShowHistoryInfo(bool  bshow);
	///��ʾׯ���б��е���Ϣ
	int  ShowNtListInfo(bool bShow);
	///��ʾͨ��ͨ��ͼƬ
	int  ShowAllWinOrLost(BYTE iStyle,BOOL bShow);

	///ɫ�Ӷ���
	int PlayDiceAnimal(bool bStarPlay,bool bFinish,bool bHide =false);
	///ϴ�ƶ���
	int  XiPaiAnimal(bool bStartPlay,bool bFinish,bool bHide) ; 
	///̯�ƶ���
	int  TanPaiAnimal(bool bStartPlay,bool bFinish,bool bHide) ; 
	///���ƶ���
	int  SendCardAnima(BYTE iDir,bool bPlay,bool bFinish = false);
	///���ƶ���
	int  OpenCardAnima(BYTE iDir,bool bPlay,bool bFinish) ;
	///��ʾ��ҵ���
	int  ShowUserHandCard(BYTE iDir,BYTE iType,bool bShow,bool bAllCard);
	///��ʾ��ҵ���
	int  ShowUserAllHandCard(BYTE iDir,bool bShow);    
	///��ʾ��ҵ���
    int  ShowUserHand(BYTE iStaion,BYTE imgType, bool bshow ,bool bAllHand = false); 
	///������ҵ�״̬����
	int  ShowSuperSettingDlg(bool bShow) ;
	  ///��ȡ���͵�·��
    int  GetCardShapePath(BYTE iStation ,CString &str);
	///��ʾ�û�������
	int  ShowUserCardShape(BYTE iStaion , bool bshow ,bool bAllHand) ; 

	int  ShowWinRect(int iWinValue,bool bShow,bool bAllRect);
  	///������ׯ��Ϣ
	int   OnhitApplyAndDowmNt(bool bApply);
	///��ҵ����ע
	int   OnUserHitNoteRect(int iChooseRect); 
	///��ׯ����ׯ��ť����ʾ
	int   ShowApplyAndDownNtButton(bool bUp,bool bShow);
	///��ʾ�����ׯ����ׯͼƬ
	int   ShowApplyNtStation(bool bInlist);
	///��Ҵ�����Ϣ
	int  ShowAutoAndHandCP(bool bAuto) ; 

	//�����������������Ϣ����������������������������
	///�ͻ��˿�ʼ��Ϣ
	int OnHandleGameBegin(GameBeginStruct *pBeginData);
	///���������ׯ��Ϣ����
	int OnHandleUserApplyZhuang(EZP_APPLY_ZHUANG * pApplyZhuang);
	///�����ע��Ϣ
	int OnHandleUserNote(EZP_XIAZHU_INFO *pNoteData);
	///����������ķ��� ��Ϣ
	int OnHandleSendCard(GameFaPaiData *pCardData);
    ///���������������Ϣ
	int OnHandleGameFinish(GameEndData *pFinishData);
	int OnHandleGameAheadFinish(void);
	//�����������������Ϣ����������������������������

	//������Ч
	int  SetPlaySound(BYTE bSoundType);
	//ɾ����Ϸ�����еĶ�ʱ��
	int  KillAllInGameTimer() ;
    ///����ogg��Ч
	int  SetPlaySoundByOgg(irrklang::ISoundEngine *engine,BYTE bSoundType);
	///���������Ϸ��Ϣ
	int  UserSetGameInfo(bool bSoundEnable,bool bWatchEnable,bool bShow);

private:
	 BYTE  GetStationParameter();
	 ///��ʾ�����ؿ�ʼ����ʱʱ��
	void ShowOrHideTime(bool bFlag,int nShowMaxNum,int nShowMinNum=0);

	int  ShowForceUserLeaveInfo(int iRevTime,bool bShow) ;

	void GetCardIndexPos(int iIndex,POINT &pPos);				//��ȡ��ֵ����ͼƬ�е�λ��

	void ShowCardImage(int iIndex,bool bShow,bool bShowAnimCard);	//��ʾ��ͼƬ
	int GetCardPoint(BYTE byCardList[]);
private:


	GameLogic                     m_Logic;				    /**< ��Ϸ�߼� */

    IGameFrame                   *m_pGameFrame;
    IUserInterface               *m_pUI;

	ISoundEngine                 *m_SoundEngine;     

	BYTE                         m_bGameStation;
	bool                         m_bGameBegin;                  ///<��Ϸ��ʼ��־

	BYTE                         m_iBeginTime;
	BYTE                         m_iThinkTime;
	BYTE                         m_iXiaZhuTime;
	BYTE                         m_iKaipaiTime; 
	BYTE                         m_iEmptyTime;

	BOOL                         m_bNtApplyDown;

	///��Ϸ��ص�����
	int                          m_iNowNtStation ;                ///��ǰ��ׯ��
	int                          m_iApplyNTLimit;                  ///��ׯ����
	__int64                      m_iMaxNote ;                      ///��ҵ������ע

	int                          m_iChooseNoteRect ;                 //ѡ��ĳ�������
	int                          m_iChooseNoteType ;                ///ѡ��ĳ�������

	CArray <GameInfoList, GameInfoList&> m_ArrGameInfoList;   /**< ÿ����Ϸ��Ϣ�б� */
	CArray <BYTE, BYTE&>         m_ArrUserList;     /**< ������ׯ������б� */

	INT_PTR                      m_iNtListInDex ; ///�û�ׯ���б�����
	INT_PTR                      m_iHistoryInfoInDex ; ///��ʷ��Ϣ����

	bool                         m_iAutoCP;        /**< �Ƿ�Ϊ�Զ����� */ 

	BYTE                         m_HandCard[4][2];
	BYTE                         m_DeskCard[4][2];

	PaiData                      m_iUserCard[4];


	__int64                     m_iMyNote[7] ;			///�Լ���ÿ���������ע
	__int64                     m_iQuYuNote[7] ;		/// ������ע
	__int64						m_iRobotSunNotes[7];	///������ÿ�����������

	__int64                     m_iMyNoteTotle ;     //��ע
	__int64                     m_iMyHistoryScore;  // ��ʷ�ɼ�


	__int64                     m_iAllUserNote;       //��������µ���ע

	__int64                     m_iNtHistoryScore;  //ׯ�ҳɼ�
	int                         m_iNtCount;          //ׯ�ҵ�ׯ������

	int                         m_iForceLeaveTimeRev;  ///ǿ������뿪ʣ��ʱ��

	bool                        m_bInNoteStage ; //��ע�Ƿ����

	BYTE                        m_iSendCardPos;
	BYTE                        m_iSendCardCount;

	BYTE                        m_iKaiPaiPos ;      ///��ǰ����λ��
	BYTE                        m_iKaiPaiCount ;    ///���ƴ���
 
	BYTE                        m_iDiceNum[2] ;     ///ɫ�ӵ���

	BYTE                        m_iCurrentMsg ;     ///��ǰ�������Ϸ��Ϣ

	int                         m_iWinkcount;   ///��˸����
	bool                        m_bShowWink;    /// �Ƿ���ʾ��˸
	BYTE                        m_iOpenStyle;   ///��������

	GameEndData                 m_gameEndData;  //��Ϸ����������

public:
	bool                        m_bSoundPlay ;  //�Ƿ���Բ�������
	bool                        m_bAllowWatch ; // �Ƿ������Թ�
	bool                        m_bWatchMode ; //�Ƿ�Ϊ�Թ�ģʽ
	bool                        m_bSuperClient; //����Ƿ�Ϊ�����ͻ���

	CString                     m_strCoinName ; ///��������

	int                         m_iAnimalCount ; ///ϴ�ƺ�̯�ƵĴ���
	BOOL						m_bIsSpace;		 ///�����ʾ�Ƿ�Ҫ�ָ���

	bool						m_bShowRobotChip;		/**< �Ƿ�����ʾ��������ע TRUE:����*/
	bool						m_bLButtonDownOrUp;		/**< �������ǰ��»��ǵ��� TRUE:Down �����ֶ�����*/
	int							m_iCurrentCardControl;	/**< ��ǰ�ƿؼ�*/
	POINT						m_pCureentCardPoint[4];	/**< ��ǰ�ƿؼ���λ��*/	
};