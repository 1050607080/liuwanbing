#include "IGameImpl.h"
#include "../GameMessage/UpGradeLogic.h"
#include <irrKlang.h>
#include "CommonDefine.h"
#include "../common/IGameFrame.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "UI_ForLogic/iuserinterface.h"
#include "Common/UIMessageDefines.h"
using namespace std;
using namespace irrklang;

#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\

#define SOUND_GAME_BG               0xff                 //������Ч

/////��ʱ��ID/////////////////////////////////////////////////////
#define TIMER_TIMETOREADY			101			//׼����ʱ��ID
#define TIMER_SHOWSINGLECARD		102			//��ʾ�����ƶ�ʱ��ID
#define TIMER_SHOWFAILINFO			103			//��ʾ����ʧ����Ϣ
#define TIMER_TRUN_HIDE			    104			//�鿴�������ݶ�ʱ��
#define TIMER_AUTO					105         //�йܳ���

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
	CUpGradeGameLogic   m_cLogic ;             ///�ͻ����߼�
private:
	///�����Ϸָ��
	IGameFrame         *m_pGameFrame;
	///������Ϸָ��
	IUserInterface     *m_pUI;
	ISoundEngine       *m_SoundEngine;				 ///��Ϸ��Ч
	ISoundEngine       *m_SoundEnginBg ;             ///������Ч
	BYTE				m_bGameStation;							///< ��Ϸ״̬
	int					m_timeToGetReady;			//׼��ʱ��
	int					m_timeToThink;				//˼��ʱ��
	int					m_iSendCardSpeed;			//�����ٶ�
	bool				m_bIsSuper;					//�Ƿ�Ϊ����
	bool				m_isAuto;					//�Ƿ��й�
	bool				m_isFirstOutCard;			//�Ƿ��һ������
	int					m_tipIndex;					//��ʾ����
	BYTE				m_byCards[33];				//������
	vector<BYTE>		m_vbyCards;					//������
	vector<BYTE>		m_vbyBigJor;				//����
	vector<BYTE>		m_vbySmallJor;				//С��
	vector<BYTE>		m_vbyLevelCard;				//����
	BYTE				m_byBackCard[8];			//����
	BYTE				m_byGameLevel[2];			//����
	BYTE				m_byLevel;				//��Ϸ����
	BYTE				m_byGameLevelColor;			//���ƻ�ɫ
	BYTE				m_byOldCard[33];			//��ʾ��ʱ�洢
	TFirstData			m_tFirstPlayerCard;			//һȦ�е�һ�����Ƶ���ҳ�����
	T_CS_OutCard		m_tCurrentOutCard;			//��ǰ������
	BYTE				m_byCurrentOutCardPlayer;	//��ǰ���Ƶ������λ��
	int					m_CardCounts;				//25��Ҫ��ʾ����
	IOperateCard	   *m_pIOperateCard;			//�ɲ�����ָ��
	INoMoveCard		   *m_pINoMoveCard;				//���ɲ�����ָ��
	IText			   *m_pITextFailInfo;			//����ʧ������
	IText			   *m_PITextScore;				//�÷�
	//����������
	TOtherCards			m_tOtherCard;				//������ҵ���
	bool				m_bIsGetOtherCard;			//�Ƿ��ѻ��������ҵ���
	bool				m_bSortFuc;					//����
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
public:
	//��������Ϣ
	void OnReceiveSendCard(void *buffer);
	//���������Ϣ
	void OnReceiveOutCard(void *buffer);
	//����������Ϣ
	void OnReceiveConfig(void *buffer);
	//���������Ϣ
	void OnReceiveFinish(void *buffer);
	//����׼����Ϣ
	void OnReceiveReady(void *buffer);
	//��������������Ϣ
	void OnReceiveBackCardFinish(void *buffer);
	//�����µ�һ����Ϣ
	void OnReceiveNewTurn(void *buffer);
	//���ܷ��������Ϣ
	void OnReceiveSendFinish(void *buffer);
	//����������Ϣ
	void OnReceiveBackCard(void *buffer);
	//���ܵ÷���Ϣ
	void OnReceiveScore(void *buffer);
	//��ʾ����
	void OnTipOutCard();
	//��Ϸ״̬����
	void GameStationProc(TGameStation& tGameStation);
	//����������¼�
	void OnUIClicked(UINT32 ControlID);
	//��ʾ׼��ͼƬ����λ��ת��ͼ����ʾ
	void ShowGetReadyPic(BYTE byDeskStaion,bool bFlag);
	//����׼��,���˳���Ϸ��Ϣ��Ϣ	 , isReadyOrQuit - false�� �˳���Ϸ�� true:׼����
	void ReadyOrQuit(bool bReadyOrQuit);
	//���Ŷ�ʱ��
	void SetTimerVisibleAndPlay(const BYTE byDeskStation,bool bFlag,BYTE byMaxTime,BYTE byMinTime);
	//��ʾ���ڵĵȼ�
	void ShowCurrentGameLevel(TGameLevel *pTGameLevel);
	//����
	void ShowLevelCard(BYTE *pbyCards,int iCount);
	//����
	void CallLevelCard(BYTE byLevelCardColor);
	//��������Ƶ�����
	int GetUpNums(const int iControlID);
	//��ȡ�������
	void GetUpCard(const int iControlID,BYTE bCardList[]);
	//���þ�̬�ı��ؼ����ı�����
	void SetTextContent(const int iControlID,WCHAR *pwText);
	//��ӡ������
	void PirntCards();
	//������Ϸ״̬
	void ResetGameStatus();
protected:
	void SetControlVisible(const int iControlID,bool bFlag)
	{
		IBCInterFace *pControl = nullptr;
		pControl = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(iControlID));
		if(pControl != nullptr)
		{
			pControl->SetControlVisible(bFlag);
		}
	}
	void SetBtEnable(const int iButtonID, bool bFlag);
};