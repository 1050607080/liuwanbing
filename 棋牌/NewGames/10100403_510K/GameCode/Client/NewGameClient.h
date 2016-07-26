#include "IGameImpl.h"
#include "../GameMessage/UpGradeLogic.h"
#include <irrKlang.h>
#include "CommonDefine.h"
#include<deque>
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
#define ID_SCORE_TIME				101					//�зֶ�ʱ��
#define ID_TIP_NORULE				102					//���򲻷�����ʾ��ʱ��
#define ID_SEND_CARD				103					//���ƶ�ʱ��

//��Ϸ��Ч
#define SOUND_BEGIN_GAME			  1				//��Ϸ��ʼ
#define SOUND_TIME_0				  2				//ʱ����ʾ��
#define SOUND_TIME_1			      3
#define SOUND_TIME_2			      4
#define SOUND_TIME_3			      5
#define SOUND_TIME_4			      6
#define SOUND_TIME_5			      7
#define SOUND_WIN				      8				//ʤ��
#define SOUND_LOSE				      9				//ʧ��
#define SOUND_CHONGPAI			      10			//����
#define SOUND_SENDCARD			      11			//����
#define SOUND_TIP			          12			//��ʾ
#define SOUND_TANPAI			      13			//����
#define SOUND_CLICKCARD			      14			//�����				
#define SOUND_HIT_BEGIN				  15			//�����ʼ��ť
#define SOUND_GAME_RUN				  16			//��Ϸ������
#define SOUND_HIT_LEAVE				  17			//�뿪	
#define SOUND_HIT_BOMB				  18			//ը��	
#define SOUND_HIT_DANI				  19			//����	
#define SOUND_HIT_BUYAO				  20			//��Ҫ
#define SOUND_HIT_FEIJI				  21			//�ɻ�	
#define SOUND_HIT_LIANDUI			  22			//����	
#define SOUND_HIT_SHUNZI			  23			//˳��
#define SOUND_HIT_SIDAIER			  24			//�Ĵ���	
#define SOUND_HIT_SIDAILIANGDUI		  25			//�Ĵ�����
#define SOUND_HIT_SANDAIYI			  26			//����һ	
#define SOUND_HIT_SANDAIDUI		      27			//������
#define SOUND_HIT_SANGE		          28			//����
//����
#define SOUND_HIT_SIGNAL_2		      190	
#define SOUND_HIT_SIGNAL_3		      191
#define SOUND_HIT_SIGNAL_4		      192
#define SOUND_HIT_SIGNAL_5		      193
#define SOUND_HIT_SIGNAL_6		      194
#define SOUND_HIT_SIGNAL_7		      195
#define SOUND_HIT_SIGNAL_8		      196
#define SOUND_HIT_SIGNAL_9		      197
#define SOUND_HIT_SIGNAL_10		      198
#define SOUND_HIT_SIGNAL_11		      199
#define SOUND_HIT_SIGNAL_12		      200
#define SOUND_HIT_SIGNAL_13		      201
#define SOUND_HIT_SIGNAL_14		      202
#define SOUND_HIT_SIGNAL_15		      203
#define SOUND_HIT_SIGNAL_16		      204
//����
#define SOUND_HIT_DOUBLE_2		      300			
#define SOUND_HIT_DOUBLE_3		      301
#define SOUND_HIT_DOUBLE_4		      302
#define SOUND_HIT_DOUBLE_5		      303
#define SOUND_HIT_DOUBLE_6		      304
#define SOUND_HIT_DOUBLE_7		      305
#define SOUND_HIT_DOUBLE_8		      306
#define SOUND_HIT_DOUBLE_9		      307
#define SOUND_HIT_DOUBLE_10		      308
#define SOUND_HIT_DOUBLE_11		      309
#define SOUND_HIT_DOUBLE_12		      310
#define SOUND_HIT_DOUBLE_13		      311
#define SOUND_HIT_DOUBLE_14		      312

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


private:
	
	IGameFrame							*m_pGameFrame;									//�����Ϸָ��
	GameInfoStructInExe					*m_pGameInfo;									//��Ϸ��Ϣָ��
	IUserInterface						*m_pUI;											//������Ϸָ��
	ISoundEngine						*m_SoundEngineGame;								//��Ϸ��Ч
	ISoundEngine						*m_SoundEnginBg ;								//������Ч
	CUpGradeGameLogic					m_Logic;										//�ͻ����߼�
	//�̶���Ϣ
	BYTE								m_iVersion;
	BYTE								m_iVersion2;
	BYTE                                m_iMyDeskStation;                               //�Լ�λ��
	BYTE								m_iThinkTime;									//��Ϸ˼��ʱ��
	BYTE								m_iBeginTime;									//��Ϸ��ʼʱ��
	//������Ϣ
	int								    m_bUserPrepare[PLAY_COUNT];						//���׼��
	BYTE								m_byGameStation;                                //��Ϸ������Ϣ
	BYTE					            m_bNtPeople;									//ׯ��
	BYTE								m_bNtViewStation;								//ׯ�ҵ���ͼλ��
	BYTE								m_DeskCard[PLAY_COUNT][CARD_NUMS];			    //ÿ���������ʵ�˿�
	BYTE								m_bOwnCard[CARD_NUMS];							//�Լ����˿�
	BYTE								m_iDeskCardCount[PLAY_COUNT];					//ÿ�������˿˵���Ŀ
	BYTE								m_bHaveCard[12];								//�����з���
	int									m_bCurrentClockLabel;							//��ǰʱ�ӱ�ǩ
	int									m_iCurrentClockControlID;						//��ǰ���Ƶ�ʱ��ID
	int									m_iCurrentContainControlID;						//��ǰ���Ƶ�ʱ������ID
	BYTE								m_bLeftCard[CARD_NUMS];							//�ϼҳ�����
	BYTE								m_bCurrentCall;									//��ǰ�������
	bool								m_bIsOutCard;									//ʱ�䵽���Ƿ��Լ�һ��Ҫ����
	bool								m_bIsAuto;										//�Ƿ��й�
	//�������
	deque<BYTE>							m_sendQueue;
	//����λ��
	POINT								m_timeXY[5];
	//��ʾ��ť���
	bool								m_bIsHaveBig;									//�Ƿ��б��ϼҴ����
	BYTE								m_bCompareCard[CARD_NUMS];						//Ҫ�ȵ���
	//��Ϸ�������
	bool								m_bIsPlayAction;								//�Ƿ񲥷Ŷ���
	bool								m_bIsPlaySound;									//�Ƿ񲥷���Ϸ��Ч
	bool								m_bIsWatching;									//�Ƿ������Թ�
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
private:
	//���ð�ť�Ƿ����
	void SetBtEnable(const int iButtonID, bool bFlag);
	//���ð�ť�Ƿ�ɼ�
	void SetBtVisible(const int iButtonID, bool bFlag);
	//����ʱ�����ʾ����ʱ
	void SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime = 0);
	//����ͼƬ�ؼ���λ��
	void SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag);
	//����ͼƬ�ؼ��Ƿ�ɼ�
	void SetImageVisible(const int iImageID, bool bFlag);
	//����ͼƬ�ؼ�������ʾָ�����ļ�
	void SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag);
	//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
	void SetPicSize(const int iButtonID,CString sImagSrc, bool bFlag,int x,int y,int w,int h);
	//���ͼƬ�ؼ��Ƿ���ʾ
	BOOL GetImageIsVisible(const int iImageID);
	//���������ؼ��ɼ�
	void SetContainVisible(const int iContainID,bool bFlag);
	//��������ؼ��ɼ�
	bool GetContainVisible(const int iContainID);
	//���������ؼ�����Ӧ�κ����ʱ��
	void SetContainEnable(const int iContainID,bool bFlag);
	//�����ı��ؼ���ʾ����
	void SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);
	//�����ı��ؼ���ʾ������Ϣ
	void SetTextShowText(const int iTextID,TCHAR ch[], bool bFlag);
	//�������ֿռ���ʾ����
	void SetInumInfo(const int , __int64 ,bool );
	//�������ֿռ���ʾ����
	void SetInumPro(const int iNumID, __int64 i64Num,BYTE bPro,bool bFlag);
	//�������ֿռ���ʾ����(�����Ӻźͼ��ż�����)
	void SetInumInfo_1(const int iNumID, __int64 i64Num, bool bFlag);
	//���ó���ؼ���ʾֵ
	void SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);
	//���ÿؼ�λ��
	void SetControlPos(const int iControlID,bool bFlag,int x,int y);
	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void SetRButtonSelected(const int iRButtonID,bool bSelected);
	//��ȡ��ѡ��ؼ��Ƿ�ѡ��
	BOOL GetRButtonSelected(const int iRButtonID,bool *bSelected);
	//���Լ�����
	void ShowCurrentCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//�����˵���
	void ShowCurrentNoMoveCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//չʾ̯����ҵ���
	void ShowOpenCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//������ʾ������
	void ShowUpCard(const int iControlID1,BYTE bUpCardList[],BYTE bUpCardCount);
	//��ȡ�������
	void GetUpCard(const int iControlID,BYTE bCardList[]);
	//��ȡ������Ƶ�����
	int GetUpCardNum(const int iControlID);
	//���ÿɲ������Ƿ�ɼ�
	void SetOperateVisible(const int iControlID,bool bFlag);
	//���ò��ɲ������Ƿ�ɼ�
	void SetNoOperateVisible(const int iControlID,bool bFlag);
	//���ŷ��ƶ���
	void ShowSendCardAction(const int ACTION_ID,const int END_ID);
	//���û�����
	void SendCard(BYTE bCardValue,BYTE bSendPeople);
	//�����ⲿ����
	void PlayAnimate(const int iID,int x,int y, bool bFlag);
	//���ݴ���
	//CStringתTCHAR
	TCHAR* CStringToCHAR(CString &str);
	//ɾ���ƶ�����
	void RemoveCard(BYTE bCardList[],BYTE bTarCard,int iCount);
	//����������
	void RestartSort(BYTE bCardList[],int iCount);
public:
	///��������
	void ResetGameData();
	//������Ϸ��Ч
	void PlayGameSound(int SoundID,bool IsBoy=true,bool bISLoop = false);
	//���ű�����Ч
	void PlayBgSound(bool bISLoop);
	//����UI����
	void ResetGameDialog();
	///��ʼ����Ϸ�еı���
	void InitialGameData();  
	///ɾ�����ж�ʱ��
	void KillAllGameTime() ;
	///��ҽ�����Ϸ
	void SetGameStation(void * pBuffer,int nLen);
	//���汾��
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);
	//ʱ�侯ʾ��
	void TimeWarning(int iTime); 
	//���������
	void OnButton(const int iBtType,int iParam,bool bFlag=true);
	//��һ����������������ִ�д˺���
	void HandleStartStation(void *buffer);
	//��Ϸ��ʼ״̬
	void HandleGameRuningStation(void *buffer);
	//���ֶ���
	//���㵱ǰ�Ƶ�ӵ����
	BYTE GetSendStation(BYTE (*BCardList)[5],BYTE bCardValue,int iCardCount);
	//����ͬ����Ϸ��Ϣ
	void OnAgreeGame(void *buffer);
	//������
	void OnHandleSendCardBegin(void *buffer);
	//������Ϸ��ʼ
	void OnHandleGameBegin(void *buffer);
	//����������������Ľ�����Ϣ
	void OnHandleCall(void *buffer);
	//��ʾ�÷���
	void OnHandleShowStyle(void *buffer);
	//���һ��̯��
	void OnHandleLastShowCard(void *buffer);
	//�������������������Ϸ����������Ϣ
	void OnHandleNormalFinish(void *buffer);
	//�������������������Ϸ�й���Ϣ
	void OnHandleAuto(void *buffer);
	//������ͼλ��������Ӧ��ʱ��
	void StartTime(BYTE bViewStation,int iClockLabel);
	//������ͼλ������Ӧ510kͼƬ
	void Paste510kPhoto(BYTE bViewStation,BYTE bCardData);
	//�鿴�Ʒָ���
	void UpdateSeeStyle(BYTE bCardData,int iView);
	//����������ʾ��
	void PlayCardTip(BYTE bDeskStation,BYTE bCardList[],int iCount);
};