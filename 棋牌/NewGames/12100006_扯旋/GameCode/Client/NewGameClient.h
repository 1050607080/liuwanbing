#include "IGameImpl.h"
#include "..\GameMessage\UpGradeLogic.h"
#include <irrKlang.h>

//��ʱ��
#define		TIME_GAME_BEGIN                10    //��ʼʱ��

//��ʱ�� ID
#define ID_BEGIN_TIME				    100					//��ʼ��ʱ��
#define ID_OUT_CARD_TIME				110					//���Ƶȴ�ʱ��
#define ID_TIME_CLOCK                   115                 //�������Ӽ�ʱ��
#define ID_BOBO_TIME					116					//����������
#define ID_SEPARET_CARD_TIME			117					//����ʱ��

#define  SOUND_FOLLOW					0x00               //��
#define  SOUND_SET_ALL		            0x01               //��
#define  SOUND_BIG		                0x02               //��
#define  SOUND_STOP						0x03               //��
#define  SOUND_GIVE_UP		            0x04               //��
#define	 SOUND_COLOR					0x05			   //ʱ��
#define  SOUND_SEND_CARD				0x06			   //����
#define  SOUND_CLICK_BUTTON				0x07			   //������Ť
#define  SOUND_MOVEBUTTON				0x08			   //�ƶ�����������
#define  SOUND_WIN					    0x09			   //ʤ������
#define  SOUND_LOSE						0x0A			   //ʧ������
#define  SOUND_OPEN_CARD				0x0B			   //����
#define  SOUND_SEPARE_CARD				0x0C			   //����
#define  SOUND_READY					0x0D				//׼������
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����
#define GF_EARLY_END				12				//��ǰ����

using namespace irrklang;
#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\
interface IGameFrame;
interface IUserInterface;
class CNewGameClient : public IGameImpl
{
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);
private:
	IGameFrame                  *m_pGameFrame;
	IUserInterface              *m_pUI;
	CUpGradeGameLogic		    m_Logic;					//�߼�����
	ISoundEngine               *m_SoundEngine;
	ISoundEngine			   *m_SoundEnginePlay;			//����������Ч����
	ISoundEngine				*m_SoundEnginBg ;           ///������Ч
	bool						m_bSoundPlay ;				//�Ƿ���Բ�������
	bool						m_bSoundPlayBG;				//�Ƿ�ű�������
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
	bool					   m_bSepartorFlag;						//������Ϣ�Ѿ�����
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

public:
	/// ��ʼ��
	virtual int Initial();
	/// ��ʼ��UI
	virtual int InitUI();
	/// ��Ϸ��Ϣ
	virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
	/// ������Ϣ
	virtual int OnUIMessage(TUIMessage* pMessage);
	//
	virtual int ResetGameFrame(void);
	//��ҽ���
	virtual int GameUserCome(void);
	//����뿪
	virtual int GameUserLeft(int nIndex); 
	//��Ҷ���
	virtual int GameUserOffLine(void);
	// �Ƿ�������Ϸ������Ϸʵ��
	// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
	virtual bool IsPlayingGame();          
	/// ��ʱ����Ϣ
	virtual void OnTimer(int nTimerID);
	/// ��״̬
	virtual void SetStationParameter(BYTE bGameStation);
	///������Ϸ״̬����Ҫ�Ƕ����������������Ϣ
	virtual 	void SetGameStation(void * pBuffer,int nLen);
	virtual bool OnGameQuiting();
private:     
	//����
	void OnGameUpgrade(void *buffer);
	//����
	void OnHandleSendCardBegin(void *buffer);
	//��ʼ
	void OnHandleGameBegin(void *buffer);
	//Ѻע
	void OnHandleBet(void *buffer,int iStyle);
	//��������
	void OnHandleNormalFinish(void *buffer);
	//ȡ����һ����ң����ƽ�������Ҫ��ȡ�����ĸ���Ҵﵽ��ע����
	BYTE GetNextNoteStation(BYTE bCurDeskStation);
	///�ر��������ͷ��
	void CloseAllLogo(BYTE bDesk = 255);
	int ViewStation(int desk);
	//��ϷUI����
	void InitGameUI();
	//��������
	void SetHandCard(BYTE bDesk, BYTE iCardList[], BYTE iUpCard[], int iCardCount, int iUpCardCount);
	// ��ʾ/���� �ؼ�
	BOOL ShowControl(int iControlID, bool bShow);
	/// ��ѯ��ť����/���� �ؼ���ֻ֧�ְ�ť��
	BOOL GetControlEnable(int iControlID);
	/// ���ÿ���/���� �ؼ���ֻ֧�ְ�ť��
	BOOL EnableControl(int iControlID, bool bShow);
	/// �趨��Ϸ��ʱ��
	BOOL SetGameTimer(BYTE bDesk, int TimeLen, int TimerID);
	/// ������ʱ��
	BOOL KillGameTimer(int TimerID = 0);
	/// FlashWindowEx��գ�
	void FlashWindow();
	/// ��ѯ��ť��ʾ/���� �ؼ���ֻ֧�ְ�ť��
	BOOL GetControlShow(int iControlID);
	//�й�ֱ�ӵ��ô˺���
	BOOL UserOutCard();
	/// ��ʾ �����
	void ShowResult(GameFinishNotify szResultInfo);
	/// ������ʾׯ��
	void SetNT(int iNTDesk);
	//��ʼ��Ϸ,��ʼ����
	bool OnControlHitBegin();
	//���������Ϣ
	void SetUserInfo(int iDesk = -1);
	//��Ϸ�߼���������
	void InitGameData();
	///���������ļ�
	void LoadData(void);
	///������Ч
	void SetPlaySound(int iSoundID, BOOL bISLoop = FALSE);
	//�����������
	BOOL PlaySound(bool isLoop);
	//���÷�����Ϣ
	void UpdateRoomInfo();
	//�����Լ��Ŀ������
	void UpdateMyLeftMoney();
	//�����Լ�����Ϣ
	void UpDateMyUserInfo();   
	//��ʾ�������ע״��
	void UpdateNoteInfo(BYTE bDeskStation, int nNoteValue = 0);
	//���ŷ��ƶ���
	void PlaySendCard(BYTE Station);
	void StartPlaySendCard();
	int GetTotalPlayNum();
	void SetCtrlBtn(bool bIsFirstNote);
	bool OnCompareCard(BYTE bViewStation);	// ������ĳ��ҵı��ư�ť
	//��ע���붯��
	void PlayXianShiChouMa(int nDeskStation, __int64 iNoteMoney,bool bFlag);
	//�ռ�Ӯ�ĳ��붯��
	void PlayGetWinChouMa(int nDeskStation);
	//��ȡ��Ϸ״̬
	BYTE GetStationParameter(void);
//----------------------------------------------------------------------
	void SOrHReadyMark(BYTE bDeskStation,bool bFlag);				//��ʾ׼����ť
	void HideNoteAndPic();											//������Ϸ����İ�ť����ʾ����
	void HideAllReadImag();											//��������׼��ͼƬ
	void SetShowOrHideBoboAndResetnoteBNT(bool bFlag);
	void SetNoteEnable(__int64 i64Money);							//���ݽ�������ó���
	void SetBoboData();												//����������
	void Ass_SetBobo_Data(BYTE * buffer,int nLen);					//�������˷�������������
	void SetBoBoValue(int iNoteId);									//����������ֵ
	void SetShowBoboValue(int iDesk,__int64 i64dMoney);				//������ʾ������ֵ
	void Bnt_Reset_Bobo();											//��������������
	void Bnt_Reset_Note();											//�������������������
	void SetEnableNoteBNT();										//������ע��ťʹ����Ч
	void SetNoteValue(int iNoteId);									//������ע��
//----------------------------------------------------------------------------------------
	void Bnt_Follow();												//������
	void Bnt_Set_All();												//�ò���
	void Bnt_Big();													//�����
	void Bnt_Stop();												//�ݲ���
	void Bnt_Give_Up();												//������
	void Ass_Sc_Notice_Send_Card(BYTE * buffer,int nLen);			//֪ͨ����
	void Ass_Sc_Send_One_Card_Finsh(BYTE * buffer,int nLen);		//��һ���ƽ���
	void Ass_Sc_Notic_Separe_Card(BYTE * buffer,int nLen);			//������Է�����Ϣ
	void Ass_Sc_Separat_Card(BYTE * buffer,int nLen);				//��ҷ��Ƴɹ���	
	void Ass_Sc_Show_All_Separat_Card(BYTE * buffer,int nLen);		//���е��ƶ��Ѿ��ֺ���
	void Ass_Sc_UserGiveUp(BYTE * buffer,int nLen);					//�û��������
	void SetShowSeparetCardBntAndImageBottomCard(bool bFlage);					//������ʾ���ư�ť�͵�ͼ
	void Operator_Card_One();			//����ƵĲ���(��һ���ɲ����ؼ�)
	void Operator_Card_Two();			//����ƵĲ���(�ڶ����ɲ����ؼ�)
	void SetShowCardShap(BYTE byCardList[],int iCardCount,int iImagID,bool bShow);			//������ʾ��������
	int GetCardShapePath(BYTE byCardList[],int iCardCount,CString &str);
	void Separate_Card();																	//���ƴ���
	void SetShowLeftAndRighCardAndShap(bool bShow);											//������ʾ������ɺ��������
	void SetShowOneCard(BYTE byCardList[],int CardCount,int iINopID,bool bShow);			//������ʾһ����
	void HideOrShowOperatCard(bool bShow);													//��ʾ�����ؿɲ����ƿؼ�
	void HideINoperateCard(int iCardID);
	void SetInumPro(const int iNumID, __int64 i64Num,bool bFlag,bool bSingle);				//������ʾͼƬ���ֿؼ�
	void SetShowNotePic(const int iImagID,__int64 iNoteMoney);								//������ʾ����ͼƬ
	void GetNotePicPath(__int64 i64Money,CString &str,int *iNum);							//ȡ�ó���ͼƬ·��

	void FinishMoveNote(int iMoveID);														//�ƶ���ɵ�ID�ؼ�
	void SetShowNoticPic(int iImageID,int iOperType);										//Ҫ��ʾ����ʾ�ļ��ؼ�
	void GetOperatePicPath(int iOperatType,CString &str);									//��ȡͼ������·��
	void GetGiveUpPicPath(int iCardNum,CString &str);										//��ȡ�����Ƶ�����
	void SetOperateNote(bool bFollow,bool bSetAll,bool bBig,bool bStop,bool bGiveUp);		//���ò�����Ť��ʹ��

	void SetTextShow(int iTextID,__int64 iMoney,bool bShow);								//������ת��������ʾ

	void ShowGiveUpPic(int iCardNum,int iPicID,bool bShow);
	void PlayGameSetShowAllNote();															//�������ʱ����������ʾ�ĳ���ͼƬ

	void OperatorPlaySound(BYTE iDeskStation,int SoundID,BYTE bCardType , bool bISLoop);	//��������

	void SetShowCardSpaceCard(int iImagID,int iCardShap,bool bShow,int iSelectShow);		//��ʾ����ʮ��������ͼƬ
};