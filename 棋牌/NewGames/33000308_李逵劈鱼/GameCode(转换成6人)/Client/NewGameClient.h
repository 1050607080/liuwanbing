#include "IGameImpl.h"
#include <irrKlang.h>

#include "IGameFrame.h"
#include "hge.h"
#include "hgesprite.h"
#include "game_manager.h"
#include "gui_manager.h"
#include "MiniDumper.h"
#include "magic/platform_win.h"
#include "magic/mp_wrap.h"

using namespace irrklang;

/*------------------------------------------------------------------------*/
#define   TIME_WAIT_LOAD     101 ///�ȴ���Ϸ��Դ�������
#define	  TIME_CLOSE		 102 ///�رռ�ʱ��
/*------------------------------------------------------------------------*/
interface IGameFrame;
interface IUserInterface;
/*------------------------------------------------------------------------*/
class CNewGameClient : public IGameImpl
{
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);

public:
	///�����Ϸָ��
	IGameFrame         *m_pGameFrame;
	//����Ƿ�رտͻ���
	bool				m_bClose;
private:
	
	///������Ϸָ��
	IUserInterface     *m_pUI;

	ISoundEngine       *m_SoundEngineGame;    //��Ϸ��Ч

	ISoundEngine       *m_SoundEnginBg ;      ///������Ч

	//��������
private:
	int						m_iVersion;					//�ڲ������汾��
	int						m_iVersion2;				//�ڲ������汾��
	BYTE					m_iPalyCard;				//��Ϸ�˿˸���		DWJ
	int						m_iPlayCount;				//��Ϸ�˿�����		DWJ
	BYTE					m_iBackCount;				//������
	int						m_iRoomBasePoint;			//���䱶��			DWJ

	GameStation				m_bufferForGameStation;		//��ʱ�洢���ݰ�
	bool					m_GetGameStation;			//�����ػز������
public: 

	BYTE                m_byGameStation ;         ///��ǰ����Ϸ״̬
	int					m_iLogicDesk;				//��ҵ��߼�λ��
	int					m_iViewDesk;				//��ͼλ��
	int                 m_iMyDeskStation ;       ///����Լ�����Ϸλ��
	UserInfoForGame		m_UserInfo[PLAY_COUNT];			/**<�����Ϣ*/
	bool				m_bIsPlaying;			//��Ϸ�Ƿ��Ѿ���ʼ


	bool                m_bSoundPlay ;		//�Ƿ���Բ�������
	bool                m_bSoundBgPlay ;	//������Ч

	bool			m_bIsWatching;//�Ƿ�Ϊ�Թ����
 public:
    /// ��ʼ��
    virtual int	Initial();
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
    virtual int	GameUserLeft(int nIndex);
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
	static	CNewGameClient *GetMySelf();

	static CNewGameClient* m_pNewGameClient;

public:

	///��ʼ����Ϸ�еı���
	void	InitialGameData();  
	///��ʼ����������
	void	ResetGameData();
	//��ȡ��Ϸ����
	void	LoadGameSet();
	//������Ϸ����
	void	SaveGameSet();

	///��ҽ�����Ϸ
	void	SetGameStation();
	//���ű�����Ч
	void	PlayBgSound(bool bISLoop, int imusictag = 0);

	//��ȡ�����Ϣ
	//�����ͼλ��
	int ViewStation(int desk);
	//�Ƿ���ڸ����
	bool isPlayer(int desk);
	//�������
	bool GetPlayerInfo(int desk, UserInfoForGame& UserInfo);


public:
		//�л���λ
	int		SwitchChairID(int m_Currchairid);

};