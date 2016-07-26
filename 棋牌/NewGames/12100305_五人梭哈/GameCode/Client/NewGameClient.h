#include "IGameImpl.h"
#include "..\GameMessage\UpGradeLogic.h"
#include "..\GameInterface\MUI.h"
#include <irrKlang.h>

//��ʱ�� ID
#define ID_BEGIN_TIME				    100					//��ʼ��ʱ��
#define ID_END_TIME						101					//������ʱ��
#define ID_TIME_SEND_CARD				102					//���Ƽ�ʱ��
#define ID_TIME_THINK                   103                 //���˼����ʱ��
#define ID_TIME_BASENOTE				104					//ÿ����������µ�ע��ʱ��
#define ID_TIME_SEE_CARD				105					//���Ƽ�ʱ��
#define ID_TIME_TIP						106					//��ʾ��ʱ��
#define ID_TIME_AUTOSTART				107					//�Զ���ʼ��ʱ��

//������������
#define SOUND_OUT_CARD				1				//��������
#define SOUND_WARNING				2				//����
#define SOUND_SEND_CARD				3				//��������
#define SOUND_START_DEF				4				//�ĸ��˿�ʼ��Ϸ��

#define SOUND_WIN					5				//ʤ��
#define SOUND_LOSE					6				//��
#define SOUND_WAITING				7				//�ȴ�
#define SOUND_USER_RUNAWARY			8				//��������
#define SOUND_USER_LEFT				9				//�����뿪
#define SOUND_MOVEBUTTON            10              //�ƶ�����ť

//��ע����(�������)
#define SOUND_GIVE_UP				11				//����
#define SOUND_NOTE					12				//��ע
#define SOUND_ADD					13				//��ע
#define SOUND_FOLLOW				14				//��ע
#define SOUND_SHUTTLE				15				//���

using namespace irrklang;

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


public:
	/// ��ʼ��
	virtual int Initial();
	/// ��ʼ��UI
	virtual int InitUI();
	/// ��Ϸ��Ϣ
	virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
	/// ������Ϣ
	virtual int OnUIMessage(TUIMessage* pMessage);
	/// ��Ϸ����
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
	virtual 	void SetGameStation(void * pBuffer,int nLen);
	//��Ϸ�˳�
	virtual bool OnGameQuiting();

public:                       ///��Ϸ�Լ��Ĳ���

	//��Ϣ������------------------------
	//�û�ǿ���뿪
	void OnHandleASS_CUT_END(BYTE * buffer,int nLen);
	//���׼����Ϣ
	void OnHandleGM_AGREE_GAME(BYTE * buffer,int nLen);
	//52��Ϸ��ʼ
	void OnHandleGM_ASS_GAME_BEGIN(BYTE * buffer,int nLen);
	//53������Ϣ
	void OnHandleGM_ASS_SEND_CARD(BYTE * buffer,int nLen);
	//54�������
	void OnHandleGM_ASS_SEND_FINISH(BYTE * buffer,int nLen);
	//55��ʼ��Ϸ
	void OnHandleGM_ASS_GAME_PLAY(BYTE * buffer,int nLen);
	//60��ע
	void OnHandleGM_ASS_NOTE(BYTE * buffer,int nLen);
	//63��ע���
	void OnHandleGM_ASS_NOTE_RESULT(BYTE * buffer,int nLen);
	//70�ռ�����
	void OnHandleGM_ASS_COLLECT_JETON(BYTE * buffer,int nLen);
	//71�������й�
	void OnHandleGM_ASS_AI_STATION(BYTE * buffer,int nLen);
	//81�·�һ����
	void OnHandleGM_ASS_SEND_A_CARD(BYTE * buffer,int nLen);
	//90��Ϸ����
	void OnHandleGM_ASS_CONTINUE_END(BYTE * buffer,int nLen);
	//93��Ϸ��ȫ����
	void OnHandleGM_ASS_SALE_END(BYTE * buffer,int nLen);
	//94��Ϸ������ʤ����
	void OnHandleGM_ASS_NO_WINER(BYTE * buffer,int nLen);

	//״̬������------------------------
	void SetGameStat_GameStation_Base(void * pBuffer,int nLen);
	void SetGameStat_GameStation_1(void * pBuffer,int nLen);
	void SetGameStat_GameStation_2(void * pBuffer,int nLen);
	void SetGameStat_GameStation_3(void * pBuffer,int nLen);
	void SetGameStat_GameStation_4(void * pBuffer,int nLen);
	void SetGameStat_GameStation_5(void * pBuffer,int nLen);

	//UI��Ϣ��Ӧ����--------------------
	//�������������Ϣ
	void OnUIMess_CATOONFINISH(UINT32 uControlID);
	//������������Ϣ
	void OnUIMess_LBUTTONUP(UINT32 uControlID);
	//�������/�Ƴ��ؼ���Ϣ
	void OnUIMess_MOUSEENTER(UINT32 uControlID);
	
	/// �û�������
	void OnHitActiveResult(BYTE btag, __int64 iNote);
	/// �û���ע��Ϣ
	void OnSelectNote(__int64 i64Num);	
	//ͬ�⿪ʼ
	bool OnControlHitBegin();

	//UI��ʾ����------------------------
	//���÷�����Ϣ
	void ShowRoomInfo();
	//��ʾ׼�������
	void ShowAgree(BYTE bDeskStation);
	//��Ϸ��ʼʱ��ʼ������
	void ShowBegin();
	//��ʾ��ע
	void ShowTotalNote(bool bFlag);
	//��ʾ����µ�ע
	void ShowPlayerNote(BYTE bDeskStation, bool bFlag);
	//��ʾ�������
	void ShowHandCard(BYTE bDeskStation, BYTE iCard[], int iCount, bool bFlag);
	//�����ע����
	void ShowNoteAnima(BYTE bDeskStation, __int64 iNote, int iTime = 0);
	//��ʾ�������ұ��
	void ShowCardMaxMark(bool bFlag);
	//��ʾ��ע��ť�ϵ���Ŀ
	void ShowNoteButText(bool bFlag);
	//��ʾ������ע��ť
	void ShowNoteBtnEnable(bool bFollow = false,bool bAllNote = false,bool bNote = false,bool bGiveUp = false);
	/// ��ʾ �����
	void ShowResult(GameFinishNotify szResultInfo, bool bFlag);
	//��ʾ���ÿ�
	void ShowInitSet(bool bFlag);
	//��ʾ����
	void ShowGiveUp(BYTE bDeskStation);

	//��Ϸ���ܺ���----------------------
	///���������ļ�
	void LoadData(void);
	//��������ͼλ��
	int ViewStation(int desk);
	//�Ƿ���ڸ����
	bool IsPlayer(BYTE bDeskStation);
	//��ϷUI����
	void InitGameUI();
	//��Ϸ�߼���������
	void InitGameData();
	/// FlashWindowEx��գ�
	void FlashWindow();
	/// ��ȡ��Ϸ״̬
	BYTE GetStationParameter(void);

	/// �趨��Ϸ��ʱ��(ר��������ʾ˼��ʱ��ģ���UI�й�)
	void SetGameTimer(BYTE bDesk);
	/// ������ʱ��(ר��������ʾ˼��ʱ��ģ���UI�й�)
	void KillGameTimer(BYTE bDesk);
	//ɱ�����м�ʱ��
	void KillAllTimer();

	//������ע
	void SetTotalNote(__int64 iNoteTotal);
	//��ע�ۼ�
	void AddTotalNote(__int64 iNoteTotal);
	//������ע��ť�ϵ���Ŀ
	void SetNoteButText(__int64 iBaseNote);


	/// ������������ע
	BOOL ReplaceUserNote(BYTE bDeskStation);
	/// colin090414�õ����ֵ
	__int64 GetMaxNote(__int64 a[4]);

	//��������---------------------------------
	//��ѭ��������������
	void PlaySound(BYTE bDeskStation, BYTE bStyle);
	//��ѭ��������������(�����ж��������Ů)
	void PlaySound(BYTE bStyle);
	//�����������
	void PlaySceneSound();
	
private:
    IGameFrame                *m_pGameFrame;

	CUpGradeGameLogic		  m_Logic;					//�߼�����

	ISoundEngine              *m_SoundEngine;

	IUserInterface *		 m_pIUI;
	MUI			              m_MyUI;

	BYTE                      m_bGameStation;               ///<��Ϸ״̬

	//��������
private:
	bool                      m_bNewTurn;
	int						  m_bMyDesk;						///�Լ����߼�ID

	BYTE					m_iThinkTime;					/**< ������Ϸ���� */
	BYTE					m_iBeginTime;					/**< ��Ϸ��ʼʱ�� */
	int						m_iBeenPlayGame;				/**< ��Ϸ���� */
	bool					m_bIsTTProject;				///�Ƿ���TT��Ŀ TT��ĿҪ����һЩ�޸�

	int						m_iRoomBasePoint;				/**< ���䱶�� */
	int						m_iRunPublish;					/**< ���ܿ۷� */
	__int64					m_iLimitNote;					/**< ����޶�ע */
	__int64					m_iBaseNote;					/**< ��ע */
	__int64					m_iNoteTotalPoint;				//��ע
	__int64					m_iThisTurnLimit;				/**< ������ע */
	//������Ϣ
	BYTE					m_iDeskCardCount[PLAY_COUNT];			/**< ÿ�������˿˵���Ŀ */
	BYTE					m_DeskCard[PLAY_COUNT][5];				/**< ÿ��������˿� */
	BYTE					m_UserCard[PLAY_COUNT][5];				/**< �û��˿� */
	//������Ϣ
	int						m_iNowOutPeople;				/**< ���ڳ����� */
	BYTE					m_iFirstOutPeople;				/**< ��һ�������� */
	__int64					m_iNowBigNote;					/**< ��ǰ���Ѻע�� */
	__int64					m_iThisGameNote[PLAY_COUNT];				/**< ������Ϸ��ע�� */
	__int64					m_iThisTurnNote[PLAY_COUNT];				/**< ������ע */
	BYTE					m_iThisNoteTimes;				/**< Ѻע���� */
	BYTE					m_bTimeOutCount;				/**< ��ʱ */
	BYTE					m_bShowAnimalStation;			/**< ������ʾλ�� */
	int						m_iUserStation[PLAY_COUNT];		/**< ��¼�û�״̬ */
	TCHAR					m_szName[PLAY_COUNT][61];		/**< ����ǳ� */
	int						m_iDoublePointTime[PLAY_COUNT];	/**< ˫��ʱ�� */
	int						m_iiProtectTime[PLAY_COUNT];	/**< �����ʱ�� */

	__int64                 m_TempNote[PLAY_COUNT];        /**< colin090418��ҵ�����ע�� */
	__int64                 m_iTempThisTurnNote[PLAY_COUNT];
	bool					m_bUserState[PLAY_COUNT];		/**<���״̬(������Ϸ��ʼ�������ɽ��빦��ʱ��ӵ����ݳ�Ա)*/	
	__int64					m_iPerJuTotalNote[PLAY_COUNT];	/**< �û�ÿ��ѹ��ע ����ռ���������ʾ��ע����ʵ����ע������������ xyh */

	__int64					m_iAddButtonNum[4];		//��ע��ťֵ
	__int64					m_iSelAddMoney;			//�϶���עֵ

	int						m_Time_BASENOTE_Count;			//�µ�ע�����ü�ʱ��

	int						m_iFirstSendCardCount;			/**< ��һ�η��������� */
	__int64					m_i64HalfLessMoney;	
	//�ؼ�����
private:	
	int						m_iVersion;
	int						m_iVersion2;

	//��������
	BOOL                    m_bSound;                        ///�Ƿ�����Ч
	HWND					m_hMciWnd1;						/**< ��������(����) */
	HWND					m_hMciWnd2;						/**< ��������(����) */
	BYTE					m_byUserFollow;					/**< �ڼ��ѱظ� */
	BYTE                    m_byCurStyle;           	    /**<  ��ǰ��ע���� */
	bool					m_bCanHitLock;					/**< colin�ܷ����� */

	BOOL					m_bNetCutPlayer[PLAY_COUNT];

	BYTE                    m_iSendAnimaCardPos;     ///���ƶ���λ��
	int                     m_iSendCardTotle;         ///���Ƶ�����
	int                     m_iNeedSendCardTotle;	 ///��ǰ��Ҫ��������Ŀ
	int						m_iSendCardNum[PLAY_COUNT];//��ǰ����ҷ�������Ŀ
	BYTE                    m_bShowHa;               ///�ڼ��ſ�����

	bool					m_bIsSuperUser;		//�ж��Լ��Ƿ񳬼��ͻ��ˣ��ɷ������������ݣ�ÿ�ֶ�У��//lym 2011-11-28
	BYTE					m_byAddNum;												/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
	BYTE                    m_bPK;                ///�Ƿ�PK

	BOOL                    m_bPlayer[PLAY_COUNT]; //�Ƿ���һ���;�����ս��

	bool					m_bSendCard;//�Ƿ����ڷ���

	bool					m_bIsWatching;//�Ƿ�Ϊ�Թ����

	CRgn					m_crMoveRect;		//�ƶ���ע�İ�ť����
	bool					m_bCanMove;			//�Ƿ�����ƶ���
	POINT					m_ptMouse;			//����λ��
	bool					m_bAutoStart;//�Ƿ�����Զ���ʼ
	__int64					m_iTempBt[4];
private:
	//���������Ӧ
	void	SetButtonOnMoseMove(const int iID,bool bFlag);
	//���������Ӧ
	void	SetImageOnMoseMove(const int iID,bool bFlag);
	//��ȡͼƬ��λ��
	POINT	GetImagePosition(const int iID);
	//����ͼƬλ��
	void	SetImagePosition(const int iID,int iX, int iY);
	//������괩͸
	void	SetImageMoseThough(const int iID,bool bFlag);
	//����ͼƬ���ؿ��
	void	SetImageSrcX(const int iID,int isrcW);
	//���ÿؼ��Ƿ�ɼ�
	void SetControlVisible(const int iControlID,bool bFlag);
	//�����ı��ؼ���ʾ������Ϣ
	void	SetTextShowText(const int iTextID,TCHAR ch[], bool bFlag);
	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);
	//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
	void SetPicSize(const int iID,CString sImagSrc, bool bFlag,int x,int y,int w,int h);
	//�������ֿؼ���ʾ����
	void SetInumInfo(const int ,wchar_t *,__int64 ,bool );
	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void SetRButtonSelected(const int iID,bool bSelected);
	//��ȡ��ѡ��ؼ��Ƿ�ѡ��
	BOOL GetRButtonSelected(const int iID,bool *bSelected);
};