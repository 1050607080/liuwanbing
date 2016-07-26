#include "IGameImpl.h"
#include <irrKlang.h>

/*-----------------------------------------------------------------------*/
//��ʱ�� ID
#define ID_BEGIN_TIME				    100					//��ʼ��ʱ��
#define ID_OUT_CARD_TIME				110					//���Ƶȴ�ʱ��
#define ID_TIME_CLOCK                   115                 //�������Ӽ�ʱ��
#define ID_PLAY_MOVE_VSCARD             116                 //��ʼ�ƻر��Ƶ��Ƽ�ʱ��
#define ID_TIME_LOOKCARD                117                 //��ʼ�ƻر��Ƶ��Ƽ�ʱ��
/*-----------------------------------------------------------------------*/

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
	virtual int		Initial();
	/// ��ʼ��UI
	virtual int		InitUI();
	/// ��Ϸ��Ϣ
	virtual int		HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
	/// ������Ϣ
	virtual int OnUIMessage(TUIMessage* pMessage);
	/// 
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

	virtual		bool OnGameQuiting();



public:
	//������·�
	void	AddOrRemoveScore(bool bAdd,int iScore);
	//��ҷ����ӵ�
	void	OnUserShot(bool bIsSuper,float fRoalation);
	//��ҼӼ�����
	void	OnUserAddFire(bool bAdd);
	//�������
	void	OnUserHitFish(int iFishID,int iBulletID,int iShootUSER,bool IsAndroid);


private:
	//������·ֽ��
	void	CDealUserBuyBullet(BYTE * buffer,int nLen);
	//��ҷ����ӵ���Ϣ
	void	CDealUserShot(BYTE * buffer,int nLen);
	//��ҷ����ӵ�����
	void	CDealUserShotError(BYTE * buffer,int nLen);
	//��ҼӼ��ڻ�
	void	CDealUserAddFire(BYTE * buffer,int nLen);
	//��һ��������
	void	CDealUserHitFish(BYTE * buffer,int nLen);
	//ȡ�������ڵ�
	void	CDealCanCelSuperPao(BYTE * buffer,int nLen);
	//�������㲥
	void	CDealCatchBroadCast(BYTE * buffer,int nLen);

private:
	//�л���λ
	int		SwitchChairID(int m_Currchairid);

public:/*�ƶ���������ĺ���*/
	//��¼��갴��ʱ�������
	CPoint	m_point;

	//�ƶ�����
	void	OnMoveWindow(CPoint);
	//��С������
	void	OnMinWondow();
	//�˳��ر���Ϸ
	void	OnCloseWindow();

	//��ȡ�����ļ�
	void	LoadGameSet();
	//������Ϸ����
	void	SaveGameSet();

private:
	//��Ϸ������
	GameScreenWnd		m_GameScreen;
	//��Ϸ�����
    IGameFrame          *m_pGameFrame;
public:
    IUserInterface              *m_pUI;

	//��Ϸ����
	GameStation_Base	m_GameData;



	ISoundEngine               *m_SoundEngine;

	BYTE                         m_bGameStation;               ///<��Ϸ״̬

	int			m_iMyDeskStation;	//����Լ����߼�λ��

};
