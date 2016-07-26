#include "IGameImpl.h"
#include <irrKlang.h>
#include <deque>


using namespace irrklang;

#define GetControlNode(A ,IControl , IControlID ) \
	IControl *A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A


/*------------------------------------------------------------------------*/
const  int  TIME_CARDBG_FALL		= 200 ; ///���Ʊ���������
const  int  TIME_CARDBG_UP			= 201 ; ///���Ʊ�������ȥ
const  int  TIME_SEND_CARD			= 202 ;	///��ʼ����
const  int  TIME_SHOW_WIN_ARES		= 203 ;	///��ʾ�н�����
const  int  TIME_SHOW_RESULT		= 204 ;	///��ʾ�����



/*------------------------------------------------------------------------*/

//������������
#define SOUND_GAME_BEGIN			0x10				//��Ϸ��ʼ
#define SOUND_BEGIN_BET				0x20				//��ע��ʼ����
#define SOUND_COUNTDWON				0x30				//��ע����ʱ
#define SOUND_MOUSE_ENTER			0x40				//����ƶ�
#define SOUND_WIN					0x50				//Ӯ����Ч
#define	SOUN_LOST					0x60				//������Ч
#define SOUND_BET					0x70				//��ע��Ч
/*------------------------------------------------------------------------*/

#define SOUND_GAME_BG               0xff                 //������Ч
/*------------------------------------------------------------------------*/


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

	ISoundEngine       *m_SoundEngineGame;		//��Ϸ��Ч

	ISoundEngine       *m_SoundEnginBg ;		///������Ч

	
public: 
	bool				m_bIsPlaying;
	BYTE                m_byGameStation ;       ///��ǰ����Ϸ״̬
	int                 m_iMyDeskStation ;      ///����Լ�����Ϸλ��
	bool				m_bWatchMode;			//�Ƿ��Թ�

	int					m_iXiaZhuTime;			///��עʱ��	
	int					m_iKaiPaiTime;			///����ʱ��  
	int					m_iShowResult;			///��ʾ�����ʱ��
	int					m_iFreeTime;			///����ʱ��	

	__int64				m_iLimitMoney;			///<��ׯ��Ҫ�����ٽ��
	__int64				m_iMaxNote;				///��ǰ����������µ���ע


	__int64				m_i64MaxZhu[BET_ARES];       //ÿ���������µ����ע	
	
	short int			m_iZhuangList[PLAY_COUNT+1];		///ׯ���б�

	deque				<LuziData> m_DqLuziData;							//·������


	int					m_iNTju;				///<��ǰׯ�ҽ��еľ���
	int					m_iNowNTstation;		///<��ǰׯ�ҵ�λ��

	__int64				m_i64MyBetCount;		//�Լ���ע����
	__int64				m_i64NtWinMoney;		//ׯ�ҵĳɼ�
	__int64				m_i64MyWinMoney;		//�Լ���Ӯ���
	__int64				m_i64ZhuangScore;		//ׯ�ұ��ֵ÷�
	__int64				m_i64MeScore;			//��ұ��ֵĵ÷�
	__int64				m_i64MeFanFen;			//�����Լ��ķ�������

	int					m_iPlayCount;			//���˶�����
	int					m_iWinCount;			//Ӯ�˶�����

	BYTE				m_byNowCard[2][3];		///<��ǰ����//ׯ�е��ƣ�0Ϊ�У�1Ϊׯ
	int					m_iZCardInfo[5];		///<ׯ������Ϣ��ׯ�ҵ�������1���Ƶĵ�����ǰ�����Ƶĵ���
	int					m_iXCardInfo[5];		///<�е�����Ϣ��ׯ�ҵ��������мҵ�������
	int					m_iWinQuYu[BET_ARES];	///<��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���,8ׯ����9ׯ˫��10�е���11��˫
	int					m_iWinShowIndex;		//�н�������˸��ʶ


	BYTE				m_bySelChouMaType;			//��ǰѡ��ĳ�������
	BYTE				m_bySelBetAres;				//ѡ�����ע��������
	CRgn				m_rgnNoteRect[BET_ARES];	///12�����������ע����

	int					m_iLuziIndex;			//·���±�
	int					m_iWaitIndex;			//��ׯ�б��±�

	bool				m_bIsSuper;

	

	
private:
	int					m_iSendCardBGPoint[2];	//���Ʊ���λ��
 public:
    /// ��ʼ��
    virtual		int Initial();
    /// ��ʼ��UI
    virtual		int InitUI();
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
	void		HandleGameBegin(BEGIN_DATA *psGameBegin);
	//����
	void		OnHandleKaiPai(KAI_PAI *pKaipai);
	//�յ���ע��Ϣ
	void		OnHandleXiaZhu(XIA_ZHU *pXiaZhu);
	//��Ϸ����
	void		HandleGameResult(JIE_SUAN *pJieSuan);
	//������Ϣ
	void		HandleGameFree(DENG_DAI *pDengDai);
	//������ׯ�����Ϣ
	void		HandleApplyResult(SHANG_ZHUANG *pShangZhuang);
private:
	//������ׯ
	void		UserApplyNtStation();
	//������ׯ
	void		UserApplyLiveNt();
	//�����ע
	void		UserBet();
	

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
	void		ShowMyInfo(bool bFalg);
	//��ʾׯ����Ϣ
	void		ShowNtInfo(bool bFalg);
	//��ʾ����ע����Ϣ
	void		ShowCanBetInfo(bool bFalg);
	//������ע��ť����
	void		SetBetBtEnable(bool bFalg);
	//������ʾ��ʱ��
	void		ShowOrHideTimer(bool bShow,BYTE byMaxTime, BYTE byMinTime = 0);
	//��ׯ�б�
	void		ShowWaitList(bool bFalg);
	//��ʾ�н�����
	void		ShowWinAres(bool bFalg);
	//������ʾ·��
	void		ShowLuziPoint(bool bFlag);
	//��ʾ·������
	void		ShowLuziAres(bool bFlag);

	//��ʾ��ׯ�б�
	void	ShowWaitNtInfo(bool bFlag);
	//��ʾ��Ϸ�����
	void		ShowGameResult(bool bFlag) ; 


	//��ʾ���˽������
	void		ShowSuperResult(bool bFlag);

private:
	//��ȡ�Ƶĵ���
	int		GetCardNum(BYTE byCard);

private:
	//���ö����ؼ����Ŷ���
	void	SetAnimatePlay(const int AnimateID,bool bFlag,BYTE byPlayNum = 1, bool bIsLoop = false);
	//���ö����ؼ�����ָ������
	void	SetAnimatePlay(const int AnimateID,wchar_t wsFilePath[],bool bFlag,BYTE byPlayNum = 1, bool bIsLoop = false);
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
	void	SetImageVisible(const int iImageID, bool bFlag,bool bMouseMove = false);
	//����ͼƬ�ؼ�������ʾָ�����ļ�
	void	SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag);
	//����ͼƬ�ؼ�������ʾָ�����ļ�
	void	SetImageXYInfo(const int iImageID,int iSrcX,bool bFlag);
	//���������ؼ��ɼ�
	void	SetContainVisible(const int iContainID,bool bFlag);
	//���������ؼ��ɼ�
	void	SetContainPosition(const int iContainID,int iX,int iY,bool bFlag);
	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,__int64 iNum, bool bFlag,bool bSymbol=false);
	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,TCHAR sChar[], bool bFlag);
	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,CString sText, bool bFlag);
	//�������ֿռ���ʾ����
	void	SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag,bool bSymbol=false);
	//���ó���ؼ���ʾֵ
	void	SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);
	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void	SetRButtonSelected(const int iRButtonID,bool bSelected, bool bFlag);
	//�����ƶ��ؼ�����
	void	SetMoveActionPlay(const int iMoveActionID,const int iControlID,POINT pStart, POINT pEnd,unsigned int uTime,bool bPlay);
	//���ò��ɲ����ƿؼ�����
	void	SetNoMoveCardInfo(const int iNoMoveCardID,BYTE byCardList[],BYTE byCardCount,POINT pPT, bool bShow);
	//���ò��ɲ����ƿؼ�����
	void	SetNoMoveCardInfo(const int iNoMoveCardID,BYTE byCardList[],BYTE byCardCount,bool bShow);
	//���õ�ѡ��
	void	SetRadioButtonSeled(const int iRadioID,bool bSeled);
	//��ȡ��ѡ���Ƿ�ѡ��
	bool	GetRadioBtIsSel(const int iRadioID);
	//��ȡ��ѡ���Ƿ�ѡ��
	void	SetRadioVisible(const int iRadioID,bool bShow);

public:
	//����UI����
	void	ReSetUiDlag();
	///��ʼ����Ϸ�еı���
	void	InitialGameData();  
	//������������ע����
	void		CreateBetRgn();
	///��ʼ����������
	void	ResetGameData();
	///ɾ�����ж�ʱ��
	void	KillAllGameTime() ;



	///��ҽ�����Ϸ
	void	SetGameStation(void * pBuffer,int nLen);
	//���ű�����Ч
	void	PlayBgSound(bool bISLoop);
	//������Ϸ��Ч
	void	PlayGameSound(int SoundID, bool bISLoop = false);

	///������������
	void  SetOutCardPlaySound(BYTE iDeskStation,int SoundID,BYTE bCardType , bool bISLoop) ;


};