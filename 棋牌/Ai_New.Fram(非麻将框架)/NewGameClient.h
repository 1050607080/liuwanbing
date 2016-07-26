



#define GetControlNode(A ,IControl , IControlID ) \
	IControl *A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A

const   int  GAME_WAIT_STATION    = 1 ;
const   int  GAME_BEGIN_STATION   = 2 ; 
const   int  GAME_FINISH_STATION  = 3 ; 

const  int  TIME_GAME_BEGIN     = 200 ; ///��Ϸ��ʼ��ʱ��
const  int  TIME_NO_BET			= 201 ;	///�����벻����ע��ʱ��
const  int  TIME_RESULT_SHOW    = 202 ; ///�н�������˸
const  int  TIME_ONE_TIME		= 203 ; ///1�뵹��ʱ


//������������
#define SOUND_GAME_BEGIN			0x10				//��Ϸ��ʼ
#define SOUND_BEGIN_BET				0x20				//��ע��ʼ����
#define SOUND_COUNTDWON				0x30				//��ע����ʱ
#define SOUND_MOUSE_ENTER			0x40				//����ƶ�
#define SOUND_WIN					0x50				//Ӯ����Ч
#define	SOUN_LOST					0x60				//������Ч
#define SOUND_BET					0x70				//��ע��Ч


#define SOUND_GAME_BG               0xff                 //������Ч



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
	///�����Ϸָ��
	IGameFrame         *m_pGameFrame;
	///������Ϸָ��
	IUserInterface     *m_pUI;

	ISoundEngine       *m_SoundEngineGame;        ///��Ϸ��Ч

	ISoundEngine       *m_SoundEnginBg ;      ///������Ч


public: 

	BYTE                m_byGameStation ;         ///��ǰ����Ϸ״̬
	int                 m_iMyDeskStation ;       ///����Լ�����Ϸλ��
  
	BYTE				m_byFreeTime;		//����ʱ��
	BYTE				m_byBetTime;		//��עʱ��
	BYTE				m_byNoBetTime;		//�����벻����ע
	BYTE				m_byBallotTime;		//��ǩʱ��
	BYTE				m_byOpenTime;		//����ʱ��
	BYTE				m_byShowResultTime;	//������ʾʱ��

	BYTE				m_byRemaindTime;		//ʣ��ʱ��
	
	BYTE				m_byNtStation;		//ׯ��λ��
	BYTE				m_byNtSelArea;		//ׯ�Ҳ�ǩ���
	BYTE				m_byLastResult;		//���ֿ������
	BYTE				m_byCurrResult;		//���ֿ������
	bool				m_bShowResult;		//�Ƿ���ʾ�н�����

	BYTE				m_byWaitNtList[PLAY_COUNT];	//��ׯ�б�
	BYTE				m_byLuziList[10];	//·��

	int					m_iApplyNtLimite;	//��ׯ����

	BYTE				m_bySelChouMaType;	//��ǰѡ��ĳ�������
	__int64				m_i64SumAllBet;		//��������ע
	__int64				m_i64MyBetCount;	//�Լ�������ע��
	__int64				m_i64AreaBetAllSum[BET_ARES];		//�����������ע
	__int64				m_i64UserBetCount[PLAY_COUNT][BET_ARES];	//�������ÿ���������ע���

	int					m_iBetSumLimite;			//����ע����
	int					m_iBetAreaLimite[BET_ARES];	//��ע����������

	int					m_iCurrWaitNtTag;			//��ǰ·�ӱ�ǩ

	bool				m_bIsSuperUser;				//�Ƿ񳬼��û�
	bool				m_bIsReady;					//�Ƿ�����׼��


	bool                m_bSoundPlay ;		//�Ƿ���Բ�������
	bool                m_bSoundBgPlay ;	//������Ч
	bool                m_bWatchMode ;		//�Ƿ����Թ�ģʽ	DWJDELETE



 public:
    /// ��ʼ��
    virtual int Initial();
    /// ��ʼ��UI
    virtual int InitUI();
    /// ��Ϸ��Ϣ
    virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
    /// ������Ϣ
    virtual int OnUIMessage(TUIMessage* pMessage);
    /// ��������״̬
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
    ///������Ϸ״̬
	virtual BYTE GetStationParameter();
	///��Ϸ����
	virtual bool OnGameQuiting();

private:
	//��Ϸ��ʼ
	void	HandleGameBegin();
	//�յ���ʼ��ע��Ϣ
	void	HandleBeginBet(void * pBuffer);
	//�յ�������ǩ��Ϣ
	void	HandleOpenResult(void * pBuffer);
	//��Ϸ����
	void	HandleGameResult(void * pBuffer);
	//������Ϣ
	void	HandleGameFree(void * pBuffer);
	//������ׯ�����Ϣ
	void	HandleApplyResult(void * pBuffer);
	//������ׯ�����Ϣ
	void	HandleLeaveNtResult(void * pBuffer);
	//�����ע�����Ϣ
	void	HandleUserBetResult(void * pBuffer);
private:
	//������ׯ
	void	UserApplyNtStation();
	//������ׯ
	void	UserApplyLiveNt();
	//�����ע
	void	UserBet(int iBetArea);
	///ׯ��ѡ���ǩ
	void	NtSelBallot(int iBetArea);
	//����ׯ�Ҳ�ǩȷ�� 
	void 	NtSureBallot();
	//��ȡ���ʣ��Ľ��
	__int64	GetMyRemaindMoney();
	//��ȡָ����ҵ�ʣ����
	__int64	GetUserRemaindMoney(BYTE byDestStation);
	//��ȡĳλ��ҵ�����ע���
	__int64	GetUserBetSum(BYTE byDestStation);
	//�Ƿ�����ׯ�б���
	bool	IsInWaitNtList();

private:
	//��ʾ��ʾ������ׯ��Ϣ
	void	ShowPromptDlg(bool bFlag);
	//��ʾ�Լ�����Ϣ
	void	ShowMyInfo(__int64 i64MyWinMoney,bool bFalg);
	//��ʾׯ����Ϣ
	void	ShowNtInfo(__int64 i64NtHaveWin, int iNtPlayCount,bool bFalg);
	//������ע��ť����
	void	SetBetBtEnable(bool bFalg);
	//���ÿ������ͼƬλ��
	void	SetResultPosition(bool bFlag);
	//������ʾ·��
	void	ShowLuziList(bool bFlag);
	//��ʾ��ׯ�б�
	void	ShowWaitNtInfo(bool bFlag);
	///��ʾ��Ϸ�����
	void	ShowGameResult(__int64 iMyPoint, __int64 iNtPoint, __int64 iMyReturn,bool bFlag) ; 
	//��ʾ�Լ�����ע���
	void	ShowBetInfo(bool bFlag);
	//��ʾ��ǩȷ����ʾ�Ի���
	void	ShowBallotDlg(int iBetArea, bool bFlag);
	//��ʾ���˽������
	void	ShowSuperResult(bool bFlag);
	//��ʾ��Ϸ���öԻ���
	void	ShowGameSetDlg(bool bFlag);
	


private:
	//���ö����ؼ����Ŷ���
	void	SetAnimatePlay(const int AnimateID,bool bFlag,BYTE byPlayNum = 1, bool bIsLoop = false);
	//���ö�����ʾĳһ֡
	void	SetAnimateShowFrame(const int AnimateID,bool bFlag,int iFrame);
	//���ð�ť�Ƿ����
	void	SetBtEnable(const int iButtonID, bool bFlag);
	//���ð�ť�Ƿ�ɼ�
	void	SetBtVisible(const int iButtonID, bool bFlag);
	//����ʱ�����ʾ����ʱ
	void	SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime = 0);
	//����ͼƬ�ؼ���λ��
	void	SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag);
	//����ͼƬ�ؼ��Ƿ�ɼ�
	void	SetImageVisible(const int iImageID, bool bFlag);
	//����ͼƬ�ؼ�������ʾָ�����ļ�
	void	SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag);
	//���������ؼ��ɼ�
	void	SetContainVisible(const int iContainID,bool bFlag);
	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);
	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,TCHAR sChar[], bool bFlag);
	//�������ֿռ���ʾ����
	void	SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag);
	//���ó���ؼ���ʾֵ
	void	SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);
	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void	SetRButtonSelected(const int iRButtonID,bool bSelected, bool bFlag);


public:
	//����UI����
	void	ReSetUiDlag();
	///��ʼ����Ϸ�еı���
	void	InitialGameData();  
	///��ʼ����������
	void	ResetGameData();
	///ɾ�����ж�ʱ��
	void	KillAllGameTime() ;
	//��ȡ��Ϸ����
	void	LoadGameSet();
	//������Ϸ����
	void	SaveGameSet();

	///��ҽ�����Ϸ
	void	SetGameStation(void * pBuffer,int nLen);
	//���ű�����Ч
	void	PlayBgSound(bool bISLoop);
	//������Ϸ��Ч
	void	PlayGameSound(int SoundID, bool bISLoop = false);

	///������������
	void  SetOutCardPlaySound(BYTE iDeskStation,int SoundID,BYTE bCardType , bool bISLoop) ;


};