#include "IGameImpl.h"
#include "../GameMessage/UpGradeLogic.h"
#include <irrKlang.h>
#include "CommonDefine.h"
#include<vector>
using namespace irrklang;

#define GetControlNode(A ,IControl , IControlID ) \
	IControl *A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A

const   int  GAME_WAIT_STATION    = 1 ;
const   int  GAME_BEGIN_STATION   = 2 ; 
const   int  GAME_FINISH_STATION  = 3 ; 

//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_WAIT_NEXT				101					//��һ�ֶ�ʱ��
#define ID_OUT_CARD					102					//���Ƶȴ�ʱ��
#define ID_LOOK_BACK				103					//������
#define ID_CHANGE_MUSIC			    104					//����������
#define ID_NOTE                     105                 //�ȴ��м���ע
#define ID_SHOW_CARD                106                 //�ȴ��мҳ���

//��Ϸ��Ч
#define SOUND_BEGIN_GAME			  0x01			//��Ϸ��ʼ
#define SOUND_TIME_0				  0x02			//ʱ����ʾ��
#define SOUND_TIME_1			      0x03
#define SOUND_TIME_2			      0x04
#define SOUND_TIME_3			      0x05
#define SOUND_TIME_4			      0x06
#define SOUND_TIME_5			      0x07
#define SOUND_WIN				      0x08			//ʤ��
#define SOUND_LOSE				      0x09			//ʧ��
#define SOUND_ADDDOUBLE			      0x0A			//�ӱ�
#define SOUND_XIAZHU			      0x0B			//��ע
#define SOUND_GIVEUP			      0x0C			//����
#define SOUND_SENDCARD			      0x0D			//����
#define SOUND_CLICKCARD			      0x0E			//�����				
#define SOUND_HIT_BEGIN				  0x0F			//�����ʼ��ť
#define SOUND_GAME_RUN				  0x1F			//��Ϸ������
#define SOUND_HIT_LEAVE				  0x2F			//�뿪


#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\

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

//��Ա����

private:
	IGameFrame							*m_pGameFrame;									///�����Ϸָ��

	GameInfoStructInExe					*m_pGameInfo;									//��Ϸ��Ϣָ��
	
	IUserInterface						*m_pUI;											///������Ϸָ��

	ISoundEngine						*m_SoundEngineGame;								//��Ϸ��Ч

	ISoundEngine						*m_SoundEnginBg ;								//������Ч
		
	CUpGradeGameLogic					m_Logic;										//�ͻ����߼�

public:
	//�̶���Ϣ
	BYTE					m_byGameStation ;										//��ǰ����Ϸ״̬
	int						m_iMyDeskStation ;										//����Լ�����Ϸλ��
	BYTE					m_bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
	BYTE					m_bGameBetTime;											//��Ϸ��עʱ��
	BYTE					m_bCallCardTime;										//��Ϸ����ʱ��

	//������Ϣ
	bool					m_IsNote;												//�Ƿ�����ע
	bool					m_IsJoin;												//�Ƿ���;����
	BYTE					m_bNtPeople;											//����ׯ��
	BYTE					m_bScorePeople;											//��ǰ�з���
	int						m_iClockLabel;											//��ǰʱ�ӱ�ǩ
	int						m_iCurrentID;											//��ǰʱ��ID
	int						m_iNoteLabel;											//ע�ӱ�ǩ
	int						m_iSendCardLabel;										//���Ʊ�ǩ
	__int64					m_i64CurrentNote;										//��ǰѡ�����ע
	__int64					m_i64UserBet[PLAY_COUNT];								//�û�����ע
	__int64					m_i64UserCanBet[3];										//�û���ע�����ֵ
	__int64					mi64UserMaxBet;											//�û������ע��
	BYTE					m_iUserCard[PLAY_COUNT][2];								//�û���ʼ�Ķ����˿�
	BYTE					m_iUserAlreadySend[PLAY_COUNT][13];						//�û��Ѿ������˿�
	BYTE					m_iUserAlreadySendCount[PLAY_COUNT];					//�û������ڼ�����
	vector<BYTE>			m_vCardSqueue;											//ÿ���Ƶķ���˳������
	vector<int>				m_vSendSqueue;											//ÿ���˵ķ���˳��
	BOOL					m_bIsSuperUser;											//�Ƿ񳬼��û�

	//��Ϸ����
	bool					m_iGameSet[3];											//��Ϸ���õ�ѡ��
	//����
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

	//�ؼ�ʹ�ú���
private:
	//���ð�ť�Ƿ����
	void	SetBtEnable(const int iButtonID, bool bFlag);

	//���ð�ť�Ƿ�ɼ�
	void	SetBtVisible(const int iButtonID, bool bFlag);

	//���ð�ť�ؼ������ƶ�ͼƬ
	void SetBtLoadSrc(const int iButtonID,CString sImagSrc,bool bFlag);

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

	//���������ؼ�����Ӧ�κ����ʱ��
	void  SetContainEnable(const int iContainID,bool bFlag);

	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);

	//�����ı��ؼ���ʾ������Ϣ
	void	SetTextShowText(const int iTextID,TCHAR ch[], bool bFlag);

	//�������ֿռ���ʾ����
	void	SetInumInfo(const int , __int64 ,bool );

	//�������ֿؼ���ʾ����
	void SetInumShow(const int,__int64,bool);

	//���ó���ؼ���ʾֵ
	void	SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);

	//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
	void SetPicSize(const int iButtonID, CString sImagSrc,bool bFlag,int x,int y,int w,int h);

	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void	SetRButtonSelected(const int iRButtonID,bool bSelected);

	//��ȡ��ѡ��ؼ��Ƿ�ѡ��
	BOOL	GetRButtonSelected(const int iRButtonID,bool *bSelected);

	//���ÿؼ��Ƿ���ʾ
	void SetControlVisible(const int iID,bool bFlag);
//��ʼ������
private:

	///��ʼ����Ϸ�еı���
	void	InitialGameData();  

	//����UI����
	void	ResetGameDialog();

	//������Ϸ����
	void ResetGameData();

	//���ܺ���
public:

	///ɾ�����ж�ʱ��
	void	KillAllGameTime() ;

	//���ű�����Ч
	void	PlayBgSound(bool bISLoop);

	///��ҽ�����Ϸ
	void	SetGameStation(void * pBuffer,int nLen);

	//������Ϸ��Ч
	void	PlayGameSound(int SoundID, bool bISLoop = false);

	//���汾��
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);

	//ʱ�侯ʾ��
	void TimeWarning(int iTime); 

	//չʾ����������ť
	void DisplayButton();

	//������ע����
	void ShowBetAction(const int ACTION_ID,const int CONTROL_ID,const int START_ID,const int END_ID,int second);

	//���Ŷ���
	void ShowAction(const int ACTION_ID,const int CONTROL_ID,const int START_ID,const int END_ID,int second);
	
	//���������
	void OnButton(const int iBtType,int iParam);

	//����������
	void OnAction(int iParam);

	//�ɲ����Ʒ���
	void OperateSendCardList(const int id,BYTE bCardList[]);

	//���ɲ����Ʒ���
	void NoOperateSendCardList(const int id,BYTE bCardList[]);

	//���������ͬ����Ϸ��Ϣ
	void OnHandleAgreeGame(void *buffer);

	//�����������������ȷ��ׯ����Ϣ
	void OnHandleNt(void *buffer);

	//�������������������ע��Ϣ
	void OnHandleBet(void *buffer);

	//����������������ķ�����Ϣ
	void OnHandleSendCard(void *buffer);

	//����������������Ŀ�ʼ��Ϸ��Ϣ
	void OnHandlePlay(void *buffer);

	//�������������������Ϸ��ȫ������Ϣ
	void OnHandleSafeFinish(void *buffer);

	//�������������������ע��Ϣ
	void OnHandleBetResult(void *buffer);

	//����������������Ľ�����Ϣ
	void OnHandleCall(void *buffer);

	//�������������������Ϸ����������Ϣ
	void OnHandleNormalFinish(void *buffer);

	//����������������ĳ����û���Ϣ
	void HandleSuperMessage(void *buffer);

	//�жϵ���
	int GetPoints(BYTE bCardList[],int iSize);

	
	//û����һ���з�����Ϸ����
	void GameEnd();

	//��ճ����û�����
	void	ClearShowSuperResult();

	//��ʾ���˽������
	void	ShowSuperResult(bool bFlag);

	//���ݳ���ֵ�õ�ͼƬ�±�
	int   GetBetIndex(int iValue);

	//��ע
	void SetNote(__int64 i64Bet,int iView);
};