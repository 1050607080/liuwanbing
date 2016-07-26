#include "IGameImpl.h"
#include "../GameMessage/UpGradeLogic.h"
#include <irrKlang.h>
#include "CommonDefine.h"
#include<vector>
using namespace irrklang;
#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\
//�ͷ���ʱ��
//��ʱ��
#define TIMER_SELECTGAMEMODE		  200	//ѡ����Ϸģʽ��ʱ��
#define TIMER_SHOWGAMEMODEINFO		  201	//��ʾ��Ϸģʽ��Ϣ��ʱ��
#define TIME_BEGIN                    101			//�ȴ�
#define TIMER_THINK					  102			//˼��
#define TIME_TIP_NOSTYLE			  103			//������
#define TIMER_BET					  104			//Ͷע��ʱ��
#define TIMER_NOTIFYMODE			  105			//ģʽ֪ͨ��ʱ��
#define TIMER_SHOWSETTLE			  106			//����������ʾʱ��
#define TIMER_BETBANKER				  107			//��ׯ��ʱ��
//��Ϸ��Ч
#define SOUND_BUTTON				  0x01			//��ť
//�����Ϣ
#define CLICK_START                 500				//��ʼ
#define CLICK_CARD                  501				//�����
#define CLICK_HAND                  502			    //����
#define CLICK_AUTO                  503				//�й�
#define CLICK_SUPER					505				//�����û�
#define CLICK_SET					506				//��Ϸ����
//��������
POINT	m_AnimateXY[3]={/*ϴ������*/{160,220},/*��ǹ����*/{5,28},/*��ʼ����*/{400,315}};
//���Ͷ���
POINT	m_StyleAnimateXY[PLAY_COUNT][3]={
	{{350,142},{350,217},{350,287}},
	{{660,142},{660,217},{660,287}},
	{{215,462},{435,462},{680,462}},
	{{0,143},{0,217},{0,287}}
};
POINT m_SendStartXY[PLAY_COUNT]={{450,280},{450,280},{450,280},{450,280}};//�����������
POINT m_SendEndXY[PLAY_COUNT]={{418,105},{744,105},{418,638},{62,105}};//�����յ�����

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
	IUserInterface						*m_pUI;											//������Ϸָ��
	ISoundEngine						*m_SoundEngineGame;								//��Ϸ��Ч
	ISoundEngine						*m_SoundEnginBg ;								//������Ч	
	CUpGradeGameLogic					m_Logic;										//�ͻ����߼�

	bool								m_isSuperDialogShow;							//���˽����Ƿ���ʾ
	BOOL								m_isWinOrLose;									//ׯ����Ӯ

public:
	int						m_iMyDeskStation ;										//����Լ�����Ϸλ��
	ENUM_GameStation		m_byGameStation;										//��Ϸ״̬
	BYTE				    m_iThinkTime;											//��Ϸ˼��ʱ��
	BYTE				    m_iBeginTime;											//��Ϸ��ʼʱ��
	bool					m_bUserPrepare[PLAY_COUNT];						        //���׼��
	BYTE					m_byOwnCards[5];										//��ҵ���

	short					m_ClockTickCount;										//ʱ�Ӽ�ʱ��

	BYTE					m_byFirstHeap[3];										//��һ����
	BYTE					m_bySecondHeap[2];										//�ڶ�����

	__int64					m_iBetMoney[4];											//�ĸ���ע���

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
public:
	//���ÿؼ��Ƿ�ɼ�
	void SetControlVisible(const int iControlID,bool bFlag);
	//���ð�ť�Ƿ����
	void SetBtEnable(const int iButtonID, bool bFlag);
	//����ʱ�����ʾ����ʱ
	void SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime = 0);
	//����ͼƬ�ؼ���λ��
	void SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag);
	//����ͼƬ�ؼ�������ʾָ�����ļ�
	void SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag);
	//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
	void SetPicSize(const int iButtonID,CString sImagSrc, bool bFlag,int x,int y,int w,int h);
	//���������ؼ�����Ӧ�κ����ʱ��
	void SetContainEnable(const int iContainID,bool bFlag);
	//�����ı��ؼ���ʾ����
	void SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);
	//�����ı��ؼ���ʾ������Ϣ
	void SetTextShowText(const int iTextID,TCHAR ch[], bool bFlag);
	//�������ֿؼ���ʾ����
	void SetInumInfo(const int , __int64 ,bool );
	//��������ʽ���ֿؼ���ʾ���ݣ����Ӻš����š����ţ�
	void SetNewInumInfo(const int iNumID, __int64 i64Num,bool bFlag);
	//���ó���ؼ���ʾֵ
	void SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);
	//���ÿؼ�λ��
	void SetControlPos(const int iControlID,int x,int y);
	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void SetRButtonSelected(const int iRButtonID,bool bSelected);
	//��ȡ��ѡ��ؼ��Ƿ�ѡ��
	BOOL GetRButtonSelected(const int iRButtonID,bool *bSelected);
	//�ɲ����Ʒ���
	void ShowOperateCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//���ɲ����Ʒ���
	void ShowNoMoveCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//��ò��ɲ����Ʒ����Ƿ���ʾ
	BOOL GetNoMoveIsVisible(const int iControlID);
	//չʾ̯����ҵ���
	void ShowOpenCard(const int iControlID,BYTE bCardList[],BYTE cardCount);
	//������ʾ������
	void ShowUpCard(const int iControlID1,BYTE bUpCardList[],BYTE bUpCardCount);
	//��ȡ�������
	void GetUpCard(const int iControlID,BYTE bCardList[]);
	//��ȡ������Ƶ�����
	int GetUpNums(const int iControlID);
	//���ŷ��ƶ���
	void ShowSendCardAction(const int ACTION_ID,const int END_ID);
	//���û�����
	void SendCard(BYTE bCardValue,BYTE bSendPeople);
	//�����ⲿ����
	void PlayIExAnimate(const int iID,int x,int y, bool bFlag);
	//�ر��ⲿ����
	void CloseAnimate(const int iID);
	//���ݴ���
	//CStringתTCHAR
	TCHAR* CStringToTCHAR(CString &str);
	//��¡ͼƬ
	void CloneImage(const int iID, int iGoalID);
	//����move����
	void PlayIMoveAction(const int ACTION_ID,const int CONTROL_ID,POINT startXY,POINT endXY,int second);
	//�ƶ��ؼ��Ƿ���ʾ
	bool GetControlShow(const int iID);
	//�趨�ؼ���x,y,w,h
	void SetControlPosition(const int iID,int x,int y,int w,int h);

	/////////////////////
	//��Ϸģʽѡ��ȷ����ť
	void OnBtnSelectGameModeOK();
	// ��Ϸģʽѡ��ɹ�
	void GameModeSelectedMessage(BYTE *pData);
	//�˳�����Ϸģʽѡ��
	void ExitWithoutSelectMode();
	//���ֳ����Ʒ��͸�������
	void SendCardToServer();
	//��ע
	void Betting(int iButtonID);

	//�ָ���Ϸ״̬
	void GameOverReset();

	void SetWinOrLose(int iControlID,BOOL IsWin);


//���ܺ���
private:
	///��ʼ����Ϸ�еı���
	void InitialGameData();  
	///��ҽ�����Ϸ
	void SetGameStation(void * pBuffer,int nLen);
	//����UI����
	void ResetGameDialog();
	//������Ϸ����
	void ResetGameData();
	//���ű�����Ч
	void PlayBgSound(bool bISLoop);
	//������Ϸ��Ч
	void PlayGameSound(int SoundID, bool bISLoop = false);
	//���ܷ�����Ϣ������
	void OnReveiveSendCard(void *pBuffer);

	//��ʾ���ƽ��
	void ShowCardsResult(void *pBuffer);

	//��ע������ʾ
	void ShowBet();

	//��ʾ������ҵ���
	void ShowCards(BYTE* pData);

	//��ʾ��������
	void ShowCardShapeInfo(EnCardShape enCardShape,int ControlID);

	//����
	void OnSettle(void *pData);

	//�����ַ���
	void GetCardShapeStr(EnCardShape enCardShape,WCHAR *buffer);

	//��ҽ�ɫ
	void GetPlayerRole(bool bBanker,WCHAR* wbuffer);
	
};