#include "IGameImpl.h"
#include "../GameMessage/UpGradeLogic.h"
#include <irrKlang.h>
#include <deque>
using namespace irrklang;

#define GetControlNode(A ,IControl , IControlID ) \
	IControl *A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A
#define  TIME_SENDLONG_CARD     201     //��ҷ����ƶ�ʱ��
#define  TIME_SENDHU_CARD       202     //��ҷ����ƶ�ʱ��
#define  TIME_SHOW_LONG			203		//��ҳ�����־�ƶ�ʱ��
#define  TIME_SHOW_HU			204		//��ҳ�����־�ƶ�ʱ��
#define  TIME_SHOW_WIN_TIME		205		//�����ʾ��Ӯ��ʱ��
#define  TIME_SHOW_HUANZHUANG   206		//��ׯ��ʱ��
#define  TIME_SHOW_MONEY		207		//������Ѷ�ʱ��
#define  TIME_SHOW_JIEPAI		208		//��ҳ�����־�ƶ�ʱ��
#define  TIME_AWARDSHOW			209		//�н�����ʾ
#define  TIME_ClOCK				210		//��ҷ����ƶ�ʱ��
#define SOUND_GAME_A			  0x01
#define SOUND_GAME_B			  0x02
#define SOUND_GAME_C			  0x03
#define SOUND_GAME_D			  0x04
#define SOUND_GAME_E			  0x05
#define SOUND_GAME_F			  0x06
#define SOUND_GAME_G			  0x07
#define SOUND_GAME_H			  0x08
#define SOUND_GAME_I			  0x09
#define SOUND_GAME_J			  0x0A
#define SOUND_GAME_K			  0x0B
#define SOUND_GAME_L			  0x0C
#define SOUND_GAME_M			  0x0D
#define SOUND_GAME_N			  0x0E
#define SOUND_GAME_O			  0x0F
#define SOUND_GAME_P			  0x1F



interface IGameFrame;
interface IUserInterface;
class CNewGameClient : public IGameImpl
{
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);//�����캯����һ�ε��ã�
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);


private:
	
	IGameFrame         *m_pGameFrame;											//�����Ϸָ��
	
	IUserInterface     *m_pUI;													//������Ϸָ��

	ISoundEngine       *m_SoundEngineGame;										//��Ϸ��Ч

	ISoundEngine       *m_SoundEnginBg ;										//������Ч
private:
	BOOL		m_bIsSuperUser;													//�Ƿ񳬼��û�
	BOOL		m_bIsReady;														//��Ϸ�Ƿ�׼����
	bool		m_bIsPlaying;													//��Ϸ�Ƿ��Ѿ���ʼ
	BYTE		m_byCardStyle[2];												//��������
	BYTE		m_byCardSort;													//�����ڼ�������
	BYTE		m_iPlayJuShu;													//��Ϸ����

	deque<BYTE> m_dbyLuzi;														//·��˫�˶���
	
	
	
	int			m_iPlayBaShu;													//�Լ��İ���
	int			m_iPlayGrade;													//�Լ��ĳɼ�
	
	__int64		m_i64MeHaveWin;													//�����Ӯ���
	//����˷������Ľ�������
	__int64     i64ChangeMoney[PLAY_COUNT];										//��ҵ÷�
	__int64     i64ReturnMoney[PLAY_COUNT];										//�����Ľ����
	__int64     i64PlayerGrade[PLAY_COUNT];										//��ҳɼ�
public: 

	CUpGradeGameLogic   m_cLogic ;												///�ͻ����߼�
	BOOL				m_bAwardDisplayFlag;									//����λ����ʾ��ǩ
	BOOL				m_bSoundIsClose;										//�Ƿ�ر�����
	BOOL				m_bUserExitIsClose;										//�Ƿ�ر��û�������ʾ��Ϣ

	//����ʱ��
	BYTE				m_bySelChouMaType;										//��ǰѡ��ĳ�������
	BYTE				m_byArea;												//�н�����
	BYTE				m_byWaitNtList[PLAY_COUNT];								//��ׯ�б�
	BYTE				m_byLuziList[52];										//·��
	BYTE                m_byGameStation ;										///��ǰ����Ϸ״̬
	BYTE                m_byFreeTime;											//����ʱ��
	BYTE                m_byBetTime;											//��עʱ��
	BYTE                m_byOpenCardTime;										//����ʱ��
	BYTE                byAccountTime;											//����ʱ��
	BYTE                m_byShowResult;											//չʾ���ʱ��
	BYTE                m_bySendSpaceTime;										//���Ƽ��ʱ��
	BYTE                m_byShowWinTime;										//��ʾ��Ӯ
	BYTE                m_bySpeedTime;											//�ڵ����ƶ��ٶ�
	BYTE            	m_iClock;												//ʣ��ʱ��
	BYTE            	m_iClockLable;											//ʱ�ӱ�ǩ
	BYTE				m_byNtStation;											//ׯ��λ��
	BYTE				m_byNtWaitListCounter;									//��ׯ�б������
	BYTE				m_byMaxNtPeople;										//�����ׯ����
	int					m_iNtPlayCount;											//ׯ����ׯ����
	int                 m_iMyDeskStation ;										///����Լ�����Ϸλ��
	int					m_iList;												//�б���ʾ������

	__int64             m_i64ApplyNtLimite ;									//ׯ����ׯ����
	__int64             i64NtGrade;												//ׯ�ҳɼ�
	__int64				m_i64NtHaveWin;											//ׯ����Ӯ���
	__int64             m_i64NtScore;											//ׯ�ҳɼ�
	__int64             m_i64PlayerCurrentMoney;								//��ҵ�ǰ���
	

	bool				m_bShowOrHideRobot;										//�Ƿ���ʾ��������ע��

	__int64				m_i64AreaAllSum[BET_ARES];								//����������ע
	__int64				m_i64RealityAreaAllSum[BET_ARES];						//������������ע
private:
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

	//�������ֿռ���ʾ����
	void	SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag);

	//�������ֿռ���ʾ����(�����Ӻźͼ��ż�����)
	void	SetInumInfo_1(const int iNumID, __int64 i64Num, bool bFlag);

	//���ó���ؼ���ʾֵ
	void	SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);

	//������Ϣ
	void	HandleGameFree(void * pBuffer);

	//����ʼ
	void    HandleStart(void *buffer);

	//��ȡָ����ҵ�ʣ����
	__int64	GetUserRemaindMoney(BYTE byDestStation);

	//�յ���ʼ��ע��Ϣ
	void	HandleBeginBet(void * pBuffer);

	//��ʾ˭��˭Ӯ
	void HandleWhoWin(void *pBuffer);

	//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
	void SetPicSize(const int iButtonID, CString sImagSrc,bool bFlag,int x,int y,int w,int h);

	void ChaoGuo(void *buffer);

	//�������ֿռ���ʾ����(����������:0:��ʾ�Ӻţ����ţ��Ͷ��ţ�1��ֻ��ʾ���ţ�2��������������)
	void	SetInumPro(const int , __int64 , BYTE ,bool );

	//��ʾ�Լ�����Ϣ
	void ShowMyInfo(bool bFalg);

	//��ʾׯ����Ϣ
	void	ShowNtInfo(bool bFalg);

	//������ע��ť����
	void	SetBetBtEnable(bool bFalg);

	//������ʾ·��
	void	ShowLuziList(bool bFlag);

	//��ʾ��Ϸ�����
	void	ShowGameResult(void *buffer) ; 

	//��ȡĳλ��ҵ�����ע���
	__int64	GetUserBetSum(BYTE byDestStation);
 public:
    /// ��ʼ�����ڶ��ε��ã�
    virtual int Initial();

    /// ��ʼ��UI�������ε��ã�
    virtual int InitUI();

    /// ��Ϸ��Ϣ
    virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);

    /// ������Ϣ
    virtual int OnUIMessage(TUIMessage* pMessage);

    /// ��������״̬
    virtual int ResetGameFrame(void);

    /// ��ҽ��루���Ĵε��ã�
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

	///��Ϸ��������Ϸ�˳�ʱ���ã�
	virtual bool OnGameQuiting();
	
	
	//����UI����
	void ReSetUiDlag();

	///��ʼ����Ϸ�еı���
	void InitialGameData();  

	///��ʼ����������
	void ResetGameData();

	///ɾ�����ж�ʱ��
	void KillAllGameTime() ;

	//���ű�����Ч
	void PlayBgSound(bool bISLoop);

	//������Ϸ��Ч
	void PlayGameSound(int SoundID, bool bISLoop = false);

	///��ҽ�����Ϸ
	void SetGameStation(void * pBuffer,int nLen);
	
	//�����ı��ؼ���ʾ���ݣ���ʾ�ַ���
	void SetChinfo(const int iTextID,TCHAR ch[61], bool bFlag);

	//������ׯ
	void UserApplyNtStation();

	//������ׯ�������
	void HandleApplyResult(void *buffer);

	//����
	void HandleKaiPai(void *buffer);

	///��ʾ��ҵķ��ƶ���
	void ShowUserSendCard(const int ID,BYTE cardStyle,BYTE cardCount,bool bShow);

	//������ע���
	void HandleUserBetResult(void *buffer);

	//���ܽ�������
	void HandleReceiveData(void *buffer);

	//��ʾ�����
	void HandleShowResult();

	//����״̬
	void HandleFreeStation(void * pBuffer);

	//��ע״̬
	void HandleXiaZhuStation(void * pBuffer);

	//����״̬
	void HandleKaiPaiStation(void * pBuffer);
	
	//������ׯ
	void UserApplyLiveNt();

	//����������ׯ�����Ϣ
	void HandleLeaveNtResult(void *buffer);

	/// ���ǿ�˽�����Ϸ
	bool UserExit(WORD nSubId,BYTE * buffer);

	//��ҽ�����Ϸ
	void PlayCome(void *buffer);
	

	//����
	//������ׯ��ׯ��ť��ʹ�ú���ʾ
	void BtNtDisp(BYTE );

	//�����ܷ���ע
	void BtEnable(BOOL);

	//������ע��ť����
	void SetBetBtEnable(bool bFalg,BYTE);

	//��ʾ��ׯ�б�
	void ShowNtWaitList(bool bFalg);

	//��ʾ�׶�ͼƬ
	void ShowSection(BYTE);

	//��ʾ�н���
	void ShowAwardFrame();

	//�û�׼���׶��˳���Ϣ
	void HandleGamePrepareQuit(void * pBuffer);

	//��ʾ��ׯ�б�
	void ShowWaitNtInfo(bool bFlag);

	//��ׯ�б��ƶ�
	void NtListMove(BYTE);

	//ͳ����ׯ�б�����
	BYTE NtListCount();

	//��ʾ·��
	void ShowLuzi();

	//����·��
	void HideLuzi();

	//���������͵ĸ���
	void AccountResult();

	//��������͵ĸ���
	void ClearAccountResult();

	//��ʾ���˽������
	void ShowSuperResult(bool bFlag);

	//��ճ����û�����
	void ClearShowSuperResult();

	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void SetRButtonSelected(const int iRButtonID,bool bSelected);

	//��ȡ��ѡ��ؼ��Ƿ�ѡ��
	BOOL GetRButtonSelected(const int iRButtonID,bool *bSelected);

	//������ʾ��ʾ�����˻���ʾ�����˰�ť
	void SetShowOrHideRobotBnt();

	//�������û���Ϣ
	void HandleSuperMessage(void *buffer);

	//�鿴·��
	void ViewLuzi(const int iContainID);

	//��Ϸ����ȷ��/ȡ��
	void GameSet(BYTE bButton);

	//������/��
	void SoundChange(BYTE bButton);
	//���ö����ؼ����Ŷ���
	//void	SetAnimatePlay(const int AnimateID,bool bFlag,BYTE byPlayNum = 1, bool bIsLoop = false);
	//���ö����ؼ�����ָ������
	//void	SetAnimatePlay(const int AnimateID,wchar_t wsFilePath[],bool bFlag,BYTE byPlayNum = 1, bool bIsLoop = false);
	//���ö�����ʾĳһ֡
	//void	SetAnimateShowFrame(const int AnimateID,bool bFlag,int iFrame);
	//�����Ƿ���ʾ��������ע��
	void SetShowRobotNote();

};