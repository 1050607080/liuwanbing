#pragma once

#include "IGameImpl.h"
#include "IGameFrame.h"
#include "../GameMessage/UpGradeLogic.h"
#include "irrKlang.h"
using namespace irrklang;
interface IGameFrame;
interface IUserInterface;

#define	 SOUND_START						50					//��Ϸ��ʼ

#define  SOUND_HIT_CARD						52					//����
#define  SOUND_OUT_CARD						53					//����
#define	 SOUND_CLOCK						54					//����
#define	 SOUND_ESCAPLE						55					//����
#define	 SOUND_WIN							56					//Ӯ
#define	 SOUND_LOSE							57					//��
#define	 SOUND_HE							58					//ţţ
//===========================================================================
#define  SOUND_NOTE							59					//��ע����
#define	 SOUND_SEND_CARD					60					//����
#define  SOUND_TONG_SHA						61					//ͨɱ
#define  SOUND_TONG_PEI						62					//ͨ��	
#define  SOUND_GAME_OVER					63					//��Ϸ����
#define  SOUND_NOTE_BG						64					//��������
#define  SOUND_XIA_ZHU_BEING				65					//��ʼ��ע��ʾ��Ч

#define SOUND_NOTE_10w       0x14                       ///��ע��������
#define SOUND_NOTE_100w      0x15                      ///��ע��������
#define SOUND_NOTE_1000w     0x16                       ///��ע��������


//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_THINK_TIME				103					//˼����ʱ��
#define ID_SCARD_TIME				104					//���ƶ�ʱ��

#define ID_SUPER_TIME				106					//����������ʾ��ʱ��
#define ID_SUPER2_TIME				107					//���˰�ť��ʾ��ʱ��
#define ID_TESTUSER_TIME			108					//����ʱ��ʹ��


#define ID_XIAZHU_TIME				10001			/// ��ע��ʱ�� 
#define ID_KAIPAI_TIME				10002			/// ���ƶ�ʱ�� 


#define PROMPT_CHANGE_NT_TIME		10010			/// ��ʾ��ׯ��Ϣ
#define SHOW_ZHAUNGLIST_TIME		10011			/// ��ʾׯ�б���Ϣ��ʱ��
#define SHOW_NOTIC_TEXT_TIME		10012			//������ʾ����


#define ID_SEND_CARD_TIMER          100				/// ���ƶ�ʱ��ID
#define ID_PLAY_ANIMAL_TIMER		101				/// ����
#define ID_SHOW_WIN_AREAS			102				/// �н�������˸
#define ID_XIAZHU_TIME				103				//��עʱ��

#define ID_XIANPAI					20004			/// ���Ʒ��мҵ���
#define ID_ZHUANGPAI				20005			/// ��ׯ�ҵ���
#define ID_PLAY_SOUND				20006			/// ��������

#define ID_XIAZHUBUTTON_NUM			0x06			///��������ע��ť
#define ID_XIAZHUAREA_NUM			0x03			///��������ע����(���ţ����ţ�����)

#define TYPE_DOWN			0x01		//�ֱ���δ�������
#define TYPE_UP				0x02		//�ֱ����������

struct SoundNews
{
	BYTE				byDeskStation;		//����λ��
	BYTE				byCureentAction;	//��ǰ����
	BYTE				byCardShape;		//����
	int					iCardCount;			//������
	BYTE				byCardPoint;		//��ֵ
	bool				bExitProess;		//�˳��߳�

	SoundNews()
	{
		memset(this,NULL,sizeof(SoundNews));
	}
};

//��Ϸ����� 
class CNewGameClient : public IGameImpl
{
private:
	IGameFrame				*m_pGameFrame;						///��Ϸ���
	IUserInterface			*m_pUI;								///��ϷUI
	CPokerGameLogic			m_Logic;							///�߼�
	UserInfoForGame			m_UserInfo[PLAY_COUNT];				/**<�����Ϣ*/
	ISoundEngine			*m_SoundEngine;						///��Чָ��
	ISoundEngine            *m_soundEngineBg ;					/// ������Ч	


private:
	int						m_iXiaZhuTime;											///��עʱ��				
	int						m_iKaiPaiTime;											///����ʱ��       
	int						m_iFreeTime;											///����ʱ��		
	BYTE					m_bGameStation;				///��Ϸ״̬		DWJ
	BYTE					m_byLgStation;				///����߼�λ��
	int						m_iNowNTstation;			///��ǰׯ�ҵ�λ��	DWJ
	int						m_iNTju;					///��ǰׯ�ҽ��еľ���������)	DWJ
	__int64					m_iNTmoney;					///��ǰׯ�ҵ���Ӯ��ң���¼��	DWJ
	__int64					m_iNTwin;					///��ǰׯ�ҵĳɼ���Ӯ�Ľ�һ��ƣ�	DWJ
	bool					m_bNtApplyXZ;				//ׯ���Ƿ���������ׯ	DWJ

	__int64					m_i64MyChengji;				///��ҳɼ�	DWJ

	int						m_iPlayCount;				///�Ѿ������˼�����			DWJ
	int						m_iGameCount;				///�ܹ������˼���		DWJ

	BYTE					m_byMaxBase;					/// ������������������ע	DWJ
	__int64					m_i64LimitMoney;				///��ׯ��Ҫ�����ٽ��DWJ
	__int64					m_i64MaxNote;					/// ÿ����������ע��DWJ

	__int64					m_i64RectNoteMax[MAX_NOTE_COUNT];	//�ĸ�������ע����(0-������)DWJ
	__int64					m_i64ChoumaValue[6];					///��������ֵDWJ
	short int				m_idZhuangList[PLAY_COUNT+1][2];	///��ׯ�б�	DWJ
	int						m_iZhuangListInedx;					///ׯ������ֵ	DWJ
	int						m_iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];	//��¼�м������������Ӯ(���ڽ��շ�������������)	DWJ
	int						m_iLuZiIndex;						//·���±�

private:	//ÿ����Ϸ��ʼ��Ҫ��ʼ���ı���
	__int64				m_iMaxZhu;							///������󻹿����µ�ע��
	__int64				m_iZhongZhu;		///���ѵ�ǰ��ע��			 	
	__int64				m_iQuYuZhu[MAX_NOTE_COUNT];		///����ÿ�������µ�ע�	
	__int64				m_iQuYuZhuTrue[MAX_NOTE_COUNT];		///ÿ��������ʵ��ע	
	__int64				m_iMyZhu[MAX_NOTE_COUNT+1];			///��������Լ���ÿ�������µ�ע�
//-------------------------------------------------------------------------------------------

	bool					m_bWatchMode;						///�Ƿ�Ϊ�Թ���
	bool					m_bWatch;							///�Ƿ�����Թ�
	bool					m_bSound;							//�Ƿ񲥷���Ч
	bool					m_bSoundPlay;						//�Ƿ񲥷���Ч


	BYTE					m_UserSendCard[5][5];				//������е���,����ʱ�õ�һ������
	BYTE					m_CardSendCount[5];					//��������Ƶ���Ŀ,����ʱ�õ�һ������
	int						m_iSendCardIndex;					//���Ʊ�ʶ

	BYTE					m_wBullCard[5][5];				//3����ɵ�ţ��
	BYTE					m_wResultCard[5][5];			//2���Ľ����

	int						m_iWinQuYu[MAX_NOTE_COUNT];			///��Ϸ��ӮǮ���� 
	bool					m_bShowRect;					///����Ӯ�������˸
	

	int                     m_iChooseNoteType;					///��������
	int                     m_iChooseNoteRect ;                 //ѡ��ĳ�������
	BYTE					m_UserCard[5][5];					//������е���
	BYTE					m_CardCount[5];						//��������Ƶ���Ŀ
	
	TagSuperSet				m_TSuperSetResult;						//�������ý��
public:
	int		m_iSendCardTime;										/// ���ƶ�������ʱ�䣬���ƶ�����	



	int		m_iMoveSpaceCount;				//����ʱ�����Ƽ���ƶ�����
	

	__int64				m_iZhuangFen;					///����ׯ�ҵĵ÷�	
	__int64				m_iXianFen;					///�����мҵĵ÷�	
	__int64				m_iUseFen;					///�м����õķ�
	__int64				m_iMeFen;					///��������Լ��ĵ÷�

	
	
	int					m_bNowNtListIndex;			/// ��ǰׯ�б�����
	int					m_bNowGameState;			///��ǰ��Ϸ״̬

	BYTE				m_byCardShap;				//���ڲ�������
	

	int  m_iControlDecOrAdd;						//���ƶ�����ر��� 0x00:�ӣ�0x01:��,0x02:�������
	int  m_iCardWith;

	int  m_iXuaZhuTimeCount;						//����ʱ��

public:
	int  m_TongShaOrTongBeiFlag;			///ͨɱ��ͨ��ı�־λ(0:ͨɱ,1:ͨ��,-1:������Ӯ)

	//���˱���
	bool					m_bIsSuperUser;			//�Ƿ񳬶�	DWJ
	bool					m_bShowRobortNote;			//�Ƿ����λ�������ע��	
	BYTE					m_bSuperTag[PLAY_COUNT];			//���˰�ť����(0Ϊ��ʼֵ��1ΪӮ��2Ϊ��)
	bool					m_bSuperSet;						//���������Ƿ�ɹ�
	bool					m_bSuperShow;						//��ʾ���˸������ð�ť
	bool					m_bCanSuper;						//�Լ��������ó���

	//��������
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);

#define SAFE_CTRL_OPERATE(CTRL_TYPE,CTRL_ID, OPERATE)\
	{\
		CTRL_TYPE* PRE=NULL; \
		PRE = dynamic_cast<CTRL_TYPE *>(m_pUI->GetIControlbyID(CTRL_ID));\
		if (PRE!=NULL) PRE->OPERATE; \
	}
public:  ///����ƽ̨����
    /// ��ʼ��
    virtual int Initial();
    /// ��ʼ��UI
    virtual int InitUI();
	/**
	* ���շ���˷��͹�������Ϣ����������Ϸ�ĵ���״̬��
	* ����������ʱ����룬��������Ϸ�ĵ�ǰ״̬
	* @param [in] void * pBuffer ���ݰ�
	* @param [in] int nLen ���ݰ��ĳ���
	* @return void 
	*/
	virtual void SetGameStation(void * pBuffer,int nLen);
	//���ֽ׶�
	void	SetGameStation_WaitNext(void * pBuffer);
	//��ע�׶�
	void	SetGameStation_Bet(void * pBuffer);
	//���ƽ׶�
	void	SetGameStation_SendCard(void * pBuffer);
	//����׶�
	void	SetGameStation_ShowResult(void * pBuffer);
	
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
	///ֹͣ��Ϸ
	virtual bool OnGameQuiting();

private:
	/**
	* ���ط����������ļ�
	* return   void
	**/
	void LoadBcf();
	//����UI����
	void	ResetUi();

	/**
	*��ʼ����Ϸ����
	*return bool ��ʼ����Ϸ�����Ƿ�ɹ�
	*/
	bool InitGameData();

	/**
	* ��������UI��״̬
	* @param BYTE bDeskStation [in] ���λ��
	* @param bool bSetCommon [in] �Ƿ����ù�ͬ����
	*return bool true:�������óɹ���false:��������ʧ��
	*/
	bool  ReSetGameUIStation(BYTE bDeskStation=255,bool bSetCommon = true,bool bAgreeStation = false); 
	/**
	* ��ȡ��Ϸ״̬
	*return BYTE ��Ϸ״̬
	*/
	BYTE  GetStationParameter();
	/**
	* ����й� �� ȡ���й� ��ť
	* @param [in] bool bAutoCard; true: �й�;false:ȡ���й�
	* @return void
	*/
	void OnHitOkOrCancleAutoBtn(bool bAutoCard);	
	BYTE ViewStation(BYTE bDeskStation);
	/**
	* ������ͨ��Ϣ
	* @param CString strTemp [in] ��Ϣ����
	* @param BYTE bDeskStation [in] ˵�����λ��
	* @return void
	*/
	void InsertNormalMessage(const char *p, ...);
	/**
	* ���Ŷ�����Ч
	* @return void
	*/
	void PlayMotionSoundOgg(irrklang::ISoundEngine *engine,BYTE bySoundType,int iSelect);
	/*
	*  ���ű�������
	*  @return void
	*/
	void SetPlaySoundBgByOgg(irrklang::ISoundEngine *engine,BYTE bSoundType);

public:		
	//��ʾ��ҵ���Ϣ
	void		ShowUserInfo(bool bShow);
	//��ʾׯ�ҵ���Ϣ
	void		ShowNTInfo(bool bShow);	
	//��ʾ��ׯ�б�
	void		ShowWaitNtList(bool bShow,bool bShowNew = false);
	//��ʾ·��
	void		ShowLuZiInfo(bool bShow,bool bShowNew = false);
	//��ʾ��ǰ��Ϣ����ʾ��ǰ�����������
	void		ShowCurrentInfo(bool bShow);
	///��ʾ������
	int			SetCurrentCursor(int uID);
	//��ʾ��ׯ��ʾ��Ϣ
	void		ShowShangZhuangNotice();
	//��ʾ���˶Ի���
	void		ShowSuperDlag(bool bShow);
	//���ͳ�������
	void		SendSuperSet();
	
	///ÿ�ֽ������ʼ����ͼ����
	void	ResetGameData();
	//������������
	void	EnableXiaZhuButton(bool bFlage);	
	//����һ����ע��ť��ʹ��
	void	EnableOneXiaZhuButton(const int iButtonNum,bool bFlag);
	//������ע��ť�Ŀ���(ֻ���㵥������ʹ�ܶ�)
	void	SetXiaZhuButtonEnable(bool bFlage);	
	//��ʼ��Ϸ
	void		OnHandleBet(BYTE * buffer,int nLen);	
	//��ҵ����ע
	void		OnXiaZhu(int iChooseRect);	
	//��ע�����Ϣ
	void		OnHandleBetResult(BYTE * buffer,int nLen);			
	//��ʾ��ע��Ϣ
	void		ShowXiaZhuInfo(bool bShow);


	void		OnHandleFree(BYTE * buffer,int nLen);			//���յ��ȴ���Ϣ
	void		OnHandleKaiPai(BYTE * buffer,int nLen);			//������Ϣ����
	void		ClearAllDeskSHAndMoneyNumber();					//�������ؼ�����Ϸ�������Ϸ
	void		RecResultData(BYTE * buffer,int nLen);						//���ս�����Ϣ

	void	SetINumSingle(const int iNumID,__int64 iNum,bool bFlag);			//�������ֿؼ��Ƿ�Ҫ��ʾ����λ
	void	SetShowOrHidePai(int Desk,BYTE CardList[],int CardNum,bool flag);	//������ʾ������	
	void	ShowOrHideResultCard(int Desk,BYTE CardList[],int CardNum,bool flag,BYTE UpOrDownType);
	void		SetShowOrHideClock(int iMaxNum,int iMinNum,bool bflag);			//������ʾʱ��	


	void	RecXiaZhuDataAfterEnableXiaZhuButton(const int count);		//���յ���ע��Ϣ֮��ť�Ĵ���
	void		CheckIsCanXiaZhu();				//���ڼ���Ƿ񻹿�����ע

	void	ShowOrHideSettlementFrame(bool bFlag);				//��ʾ�����ؽ����
	void	SendCardTimer();									//����ʱ�����

	void	AutoMoveCardSpacing(BYTE iStation,int m_iMoveSpaceCount,bool flag); //�Զ��ƶ��Ƽ��
	//������
	void	RectXiaZhuGuangFlash(bool bShow);	
	//������ע�������ʾ�벻��ʾ
	void	SetXiaZhuAreaShowOrHide(bool bFlage);
	
	void	SetTextShowText(const int iTextID,TCHAR ch[], bool bFlag);		//���ַ�����ʾ�ھ�̬�ı��ؼ���	



	//--------------------------------------------------------------------------------------------------
	bool CheckMeQiangZhuang();									//����ܷ���ׯ
	void	ShowShangZhuangOrXiaZhuangBt();							//��ʾ������ׯ����Ҫ��ׯ
	void ShangZhuangOrXiaZhuangButton(const int MsgNum);		//�������µĴ���
	bool CheckMeZhuang(void);									//��������Ƿ���ׯ��
	void OnHandleShangZhuang(BYTE * buffer,int nLen);					//��ׯ��Ϣ
	//--------------------------------------------------------------------------------------------------

													
	//--------------------------------------------------------------------------------------------------
	void SetGameInfordDlg(bool bSoundEnable,bool bBGSoundEnable,bool bShow);		//������ʾ��Ϸ���öԻ���
	void ButtonDownOK();													//���ȷ����ť
	void ButtonCancel();													//���������ť
	//--------------------------------------------------------------------------------------------------------
	int ShowOrHideHandImg(BYTE iStation,bool flag);    //��ʾ��������ͼƬ
	int	ShowUserCardShape(BYTE iStaion , bool bshow ,bool bAllHand);   //��ʾ��ҵ�����
	int GetCardShapePath(BYTE iStation ,CString &str,bool bAllHand);		//ȡ��ͼƬ��·��
	int PlayAllWinOrLostAni(BYTE iGameStyle);					//����ͨɱ��ͨ��
	int CheckGameResult();										//�����Ϸ���
	
	//--------------------------------------------------------




	void SetOneRButton(const int iRbuttonID,bool bSelect);	//���õ�ѡ��ť


	bool GetOneRButton(const int iRbuttonID);				//ȡ�õ�ǰ�ĵ�ѡ��ť�Ƿ���ѡ


	
	void PutBullCard(BYTE iDesk);		//��ţ

	

	void PlayAnimal(int iDesk,int iRunCount);											//���Ŷ���
	void ControlPlayAnimal();													//�ؼ����Ŷ���


	//���ÿɲ������ƿؼ���Ϣ
	void	SetOperateCardInfot(const int iID,BYTE CardList[],int CardNum,bool bShow);
	//���ò��ɲ����ƿؼ���Ϣ
	void	SetNoOperateCardInfo(const int iID,BYTE CardList[],int CardNum,bool bShow);
	//���ð�ť�Ƿ����
	void	SetButtonEnable(const int iID,bool bEnable);
	//���ð�ť�Ƿ�ɼ�
	void	SetButtonVisible(const int iID,bool bVisible);
	//�����ı��ؼ���Ϣ
	void	SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);		
	//�����ı��ؼ���Ϣ
	void	SetTextInfoSz(const int iTextID,CString sText, bool bFlag);
	//����ͼƬ�Ƿ�ɼ�
	void	SetImageVisible(const int iImageID,bool bShow);
	//����ͼƬ��Ϣ
	void	SetImageSrcXY(const int iImageID, int iSrcX, int iSrcY,bool bShow);
	//�����������ؼ���Ϣ
	void	SetShCtrlInfo(const int iShCrtlID,int iNum,bool bShow);
	//�����޷������ֿؼ�����
	void	SetINumInfoNoSymbol(const int iINumID,__int64 i64Num,bool bShow);
	//���������Ƿ�ɼ�
	void	SetContainVisible(const int iID,bool bVisible);
};
