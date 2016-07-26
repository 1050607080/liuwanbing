#pragma once

#include "../../../../sdk/include/ServerCommon/GameDesk.h"


//��ʱ�� ID
#define TIME_MY_TIMER				20				//���ö�ʱ����ֻ�������ʱ����ʵ�ʶ�ʱ�������඼�Ǵ������ʱ�������ж�����
#define MY_ELAPSE					250				// ��ʱ�����
#define TIMER_COUNT					11				// �õ��Ķ�ʱ����Ŀ

/*---------------------------------------------------------------------*/
#define FISH_ALIVE_TIME				50*1000								//���ʱ��
/*---------------------------------------------------------------------*/
// ����Ϊ��Ϸ��ʹ�õ��Ķ�ʱ�����������TIME_MY_TIMER��������ͳһ����
#define	TIME_MIN					30				//��С��ʱ��ID
#define TIME_BUILD_TRACE			31				//������켣��ʱ��
#define	TIME_CREAD_FISH				32				//���������ʱ��
#define	TIME_CLEAR_TRACE			33				//������ټ�ʱ��
#define	TIME_CHANGE_SCENE			34				//�л�������ʱ��
#define	TIME_REGULAR_FISH			35				//�����ʱ��

#define	TIME_SUPER_PAO_0			40				//0��λ�ó����ڼ�ʱ��
#define	TIME_SUPER_PAO_1			41				//1��λ�ó����ڼ�ʱ��
#define	TIME_SUPER_PAO_2			42				//2��λ�ó����ڼ�ʱ��
#define	TIME_SUPER_PAO_3			43				//3��λ�ó����ڼ�ʱ��
#define	TIME_SUPER_PAO_4			44				//4��λ�ó����ڼ�ʱ��
#define	TIME_SUPER_PAO_5			45				//5��λ�ó����ڼ�ʱ��



#define	TIME_MAX					50				//����ʱ��ID
/*---------------------------------------------------------------------*/

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����

/*---------------------------------------------------------------------*/
static	const int G_iFishScore[26]={2,2,3,4,5,6,7,8,9,12,10,15,18,20,25,30,40,100,40,400,300,120,200,150,80,800};
/*---------------------------------------------------------------------*/
//���ؿ��Ʋ���
//���صȼ�
static	__int64		G_i64RewardLevel[3] = {100000,1000000,1000000};
//���صȼ���Ӧ�ĸ���ֵ
static	float		G_fRewardLevelProbability[3]={0,0,0};
//��ǰ���صĽ��
static	__int64		G_i64CurrPoolMoney = 0;
//��һ�εĽ��ؽ��
static	__int64		G_i64PrePoolMoney = 0;

/*---------------------------------------------------------------------*/


// ��������ؼ����ͽ�������
class BZWLockForCriticalSection
{
	CRITICAL_SECTION *m_pSection;
	BZWLockForCriticalSection();
public:
	BZWLockForCriticalSection(CRITICAL_SECTION *pSection)
	{
		m_pSection = pSection;
		EnterCriticalSection(m_pSection);
	}
	~BZWLockForCriticalSection()
	{
		LeaveCriticalSection(m_pSection);
	}
};


//��Ϸ����
class CServerGameDesk : public CGameDesk
{
	CRITICAL_SECTION m_csForIPCMessage;
private:		/*��������*/
	int			m_iCreateFishTime;	//�������ʱ��
	int			m_iChangeScene;		//�л�����ʱ��
	int			m_iFireBase;		//�����ڻ���
	int			m_iMaxFireBase;		//����ڻ���
	int			m_iMinFire;			//��С�ڻ���
	int         m_fishShoot[26][3];	//��ɱǹ��
	int			m_iFishBase[26];	//�������
	int         m_iBiliDuihuan;		//�һ�����

	int			m_iUserPercent[PLAY_COUNT];	//��Ҹ��ʿ���

	vector	<long>	m_lvSuperUserID;	//�����û��б�
	vector	<long>	m_lvBlackListID;	//�������б�

private:		
	bool		m_bHaveHitFlag;					//�Ƿ���ڻ����˻���������ж�
	BYTE		m_byHitDesk;						//�����˻���������жϵ�λ�ú�
	bool		m_bIsPlaying;					//��Ϸ�Ƿ��Ѿ���ʼ
	int         m_iBgindex;						//��������
	int         m_iRegfishcount;				//���������

	int			m_iUserFireBase[PLAY_COUNT];	//��ҵ��ڻ���
	int			m_iUserScore[PLAY_COUNT];		//����Ϸ���

	__int64		m_i64UserMoney[PLAY_COUNT];		//������ϵĽ����
	__int64		m_i64UserAllScore[PLAY_COUNT];	//������Ϸ�

	int			m_iUserShootCount[PLAY_COUNT];	//��ҷ����ӵ��ĸ���
	BulletInfo  m_UserShoot[PLAY_COUNT][100];	//����ӵ���Ϣ
	int			m_SendTime[PLAY_COUNT];			//��ҷ����ӵ����


	bool		m_bUserIsSuperPao[PLAY_COUNT];	//�Ƿ񳬼��ڵ�

private:
	FishTrace                       m_fishtrace[250][5];//��Ӧ���������·�ߣ�5������ͽǶ�
	DWORD							m_dwFishID;	//��Ⱥ��ʶ





public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	//��Ϸ��ʼ
	virtual bool	GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool	GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool	IsPlayGame(BYTE bDeskStation);
	///�жϴ���Ϸ���Ƿ�ʼ��Ϸ
	virtual BOOL	IsPlayingByGameStation();
	//��Ϸ���ݰ�������
	virtual bool	HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�����Ϣ������
	virtual bool	HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE	UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	///�û�������Ϸ��
	virtual BYTE	UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	//�û�����
	virtual bool	UserNetCut(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//��ʼ��������Ϣ
	virtual bool	InitDeskGameStation();
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool	OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool	ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool	OnTimer(UINT uTimerID);

private:
	//������·�
	void	OnHandleUserAddOrMoveBullet(BYTE byStation,void * pData, UINT uSize);
	//��ҷ����ӵ�
	void	OnHandleUserShot(BYTE byStation,void * pData, UINT uSize);
	//��ҼӼ���
	void	OnHandleUserAddFire(BYTE byStation,void * pData, UINT uSize);
	//�������
	void	OnHandleHitFish(BYTE byStation,void * pData, UINT uSize);
	bool	OnSubHitFish(BYTE byStation, int iFishID, int iBulletID);
	//�Ƿ�����Դ�����
	bool	IsProbalityForDead(int iFishIndex);

	//������㷢�͹㲥
	void	SendCatchBroadCast(int iFishType, CMD_S_CaptureFish TCaptureFish);
	

private:
	//��ʼ������Ϸ
	void	StartGame();
	//������
	void	CreadFish();
	//���ɹ���С��
	void	CreatRegSmalFish();
	//����С��
	void	CreatSmalFish();
	//�������1
	void	CreateFishTraceOne();
	//�������2
	void	CreateFishTraceTwo();
	//�������3
	void	CreateFishTraceThree();
	//�������4
	void	CreateFishTraceFour();
	//�������ID��ʶ
	DWORD	GetNewFishID();
	//������
	void	ClearFishTrace();
	//�л�����
	void	ChangeScence();
	//��������һ
	void	RegFishone();
	//�������Ͷ�
	void	RegFishtwo();
	//����������
	void	RegFishthree();
	//ȡ����ҵĳ�����
	void	CanCelUserSuperPao(BYTE	byStation);

	//������
private:
	//��ո�λ�õ�����
	void	IniUserData(BYTE byStation);
	//��ȡ��̬����
	BOOL	LoadIni();	
	//���ݷ���ID���ص�ע������
	BOOL	LoadExtIni(int iRoomID);
	//�Ƿ񳬼��û�
	bool	IsSuperUser(BYTE byStation);
	//�Ƿ�������û�
	bool	IsBlackListUser(BYTE byStation);

	//���潱�ؼ�¼
	void	RecordPool();

	//�ж��Ƿ�ȫ�����ǻ�����
	bool	IsAllRobot();

	//�û�������Ϸ

};

/******************************************************************************************************/
