#pragma once

#include "StdAfx.h"

#include <vector>

struct FishTraceInfo //��켣��Ϣ�ṹ��
{
	FishKind fish_kind;
	int fish_id;
	DWORD build_tick;
};

struct SweepFishInfo //��켣��Ϣ�ṹ��
{
	FishKind fish_kind;
	int fish_id;
	BulletKind bullet_kind;
	int bullet_mulriple;
};

struct ServerBulletInfo //������ӵ���Ϣ�ṹ��
{
	BulletKind bullet_kind;
	int bullet_id;
	int bullet_mulriple;
};

struct SuperManInfo//�����û�����
{
	int iUserID;//�û�ID
	int iCellScore;//��λ����
};

static std::vector<DWORD> g_balck_list_;
static std::vector<DWORD> g_white_list_;

struct Fish20Config {
	DWORD game_id;
	int catch_count;
	double catch_probability;
};
static std::vector<Fish20Config> g_fish20_config_;

const int kBulletIonTimer=							20;

const int kLockTimer		=					31;
const int kClearTraceTimer				=		32;		//�����������ʱ��
const int kBuildSmallFishTraceTimer		=		33;		//С���ʱ��
const int kBuildMediumFishTraceTimer	=		34;		//�е����ʱ��
const int kBuildFish18TraceTimer		=		35;		//18�����ʱ��
const int kBuildFish19TraceTimer		=		36;		//19�����ʱ��
const int kBuildFish20TraceTimer		=		37;		//20�����ʱ��
const int kBuildFishLKTraceTimer		=		38;		//���Ӽ�ʱ��
const int kBuildFishBombTraceTimer		=		39;		//ը�����ʱ��
const int kBuildFishSuperBombTraceTimer	=		40;		//����ը�����ʱ��
const int kBuildFishLockBombTraceTimer	=		41;		//����ը�����ʱ��
const int kBuildFishSanTraceTimer		=		42;
const int kBuildFishSiTraceTimer		=		43;
const int kBuildFishKingTraceTimer		=		44;
const int kSwitchSceneTimer				=		45;
const int kSceneEndTimer				=		46;
const int kLKScoreTimer					=		47;

const int kClearTraceElasped			=	60;
//const int kBuildSmallFishTraceElasped			=	4;//С�����ʱ��
//const int kBuildMediumFishTraceElasped		=	5;//�е������ʱ��
//const int kBuildFish18TraceElasped			=	33;//18�������ʱ��
//const int kBuildFish19TraceElasped			=	43;//19�������ʱ��
//const int kBuildFish20TraceElasped			=	41;//20�������ʱ��
//const int kBuildFishLKTraceElasped			=	58;//���ӳ���ʱ��
//const int kBuildFishBombTraceElasped			=	68;//ը�������ʱ��
//const int kBuildFishSuperBombTraceElasped		=	77;//����ը�������ʱ��
//const int kBuildFishLockBombTraceElasped		=	64 + 10;//����ը�������ʱ��
//const int kBuildFishSanTraceElasped			=	80 + 28;//����Ԫ�����ʱ��
//const int kBuildFishSiTraceElasped			=	90 + 17;//����ϲ�����ʱ��
//const int kBuildFishKingTraceElasped			=	34;//��������ʱ��
const int kSwitchSceneElasped					=	600;
const int kSceneEndElasped						=	63;
const int kLKScoreElasped						=	1;


const int kFishAliveTime = 180000;

static SCORE g_stock_score_ = 0;
static SCORE g_revenue_score = 0;

#define GF_NORMAL					10								/**< ��Ϸ�������� */
#define GF_SALE						11								/**< ��Ϸ��ȫ���� */

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
//��������
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//���ؿ����Ϣ
	virtual	bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);;
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//�����ػ�
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	///�û����½���
	virtual  bool UserReCome(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	///�û�������Ϸ��
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);

	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//������
public:
	//��ʼ��һϵ�й���(���¿�ʼ��Ϸ�Ž��д��ֳ�ʼ��)
	//BOOL InitThisGame();
	//��ȡ��̬����
	static BOOL LoadIni();	//׼�����ƹ���
	BOOL LoadExtIni(UINT uRoomID);   //���ݷ���ID���������ļ�

	//�����м�ʱ��
	void KillAllTimer();

	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////��Ϸ��������/////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//������ǰ����
	BOOL UserStop(BYTE bDeskStation,BOOL bAgree= false);
	//ͳ����Ϸ����
	BYTE CountPlayer();

	//------------------------------------------------------------------------------
	//���㺯��----------------------------------------------------------------------
	//------------------------------------------------------------------------------

	//���
	void CalcScore(BYTE bDeskStation);
	//�������м�ʱ��
	void StartAllGameTimer();
	//������Ϸ״̬
	void SetGameStatus(BYTE game_status) { m_bGameStatus = game_status; }
	//��ȡ��Ϸ״̬
	BYTE GetGameStatus() const { return m_bGameStatus; }
	//�����ID
	int GetNewFishID();
	//����ӵ�ID
	int GetBulletID(WORD chairid);
	//����������Ϣ
	bool SendGameConfig(BYTE bDeskStation, GameStation& cmd);
	//���������ļ�
	bool LoadConfig();

	FishTraceInfo* ActiveFishTrace();
	bool FreeFishTrace(FishTraceInfo* fish_trace_info);
	void FreeAllFishTrace();
	FishTraceInfo* GetFishTraceInfo(int fish_id);


	void SaveSweepFish(FishKind fish_kind, int fish_id, BulletKind bullet_kind, int bullet_mulriple);
	bool FreeSweepFish(int fish_id);
	SweepFishInfo* GetSweepFish(int fish_id);

	//�Ӳֿ��м���һ���ӵ����ݸ�ָ�����
	ServerBulletInfo* ActiveBulletInfo(WORD chairid);
	//���ָ�����ָ���ӵ���Ϣ��������������ݱ��浽�ֿ���
	bool FreeBulletInfo(WORD chairid, ServerBulletInfo* bullet_info);
	//����ָ����������ӵ���Ϣ
	void FreeAllBulletInfo(WORD chairid);
	//��ȡ�ӵ���Ϣ
	ServerBulletInfo* GetBulletInfo(WORD chairid, int bullet_id);

	//��ʱ��-----------------------------------------------------------------
	bool OnTimerBuildSmallFishTrace();
	bool OnTimerBuildMediumFishTrace();
	bool OnTimerBuildFish18Trace();
	bool OnTimerBuildFish19Trace();
	bool OnTimerBuildFish20Trace();
	bool OnTimerBuildFishLKTrace();
	bool OnTimerBuildFishBombTrace();
	bool OnTimerBuildFishSuperBombTrace();
	bool OnTimerBuildFishLockBombTrace();
	bool OnTimerBuildFishSanTrace();
	bool OnTimerBuildFishSiTrace();
	bool OnTimerBuildFishKingTrace();
	bool OnTimerClearTrace();
	bool OnTimerBulletIonTimeout(WPARAM bind_param);
	bool OnTimerLockTimeout();
	//��ʱ��_ת������
	bool OnTimerSwitchScene();
	//��ʱ��_ת����������
	bool OnTimerSceneEnd();
	bool OnTimerLKScore();

	//�յ���Ϣ-----------------------------------------------------------------
	//���㲶���
	bool OnSubExchangeFishScore(BYTE bDeskStation, bool increase);
	bool OnSubUserFire(BYTE bDeskStation, BulletKind bullet_kind, float angle, int bullet_mul, int lock_fishid);
	bool OnSubCatchFish(BYTE bDeskStation, int fish_id, BulletKind bullet_kind, int bullet_id, int bullet_mul);
	bool OnSubCatchSweepFish(BYTE bDeskStation, int fish_id, int* catch_fish_id, int catch_fish_count);
	bool OnSubHitFishLK(BYTE bDeskStation, int fish_id);
	bool OnSubStockOperate(BYTE bDeskStation, unsigned char operate_code);
	bool OnSubUserFilter(BYTE bDeskStation, DWORD game_id, unsigned char operate_code);
	bool OnSubFish20Config(BYTE bDeskStation, DWORD game_id, int catch_count, double catch_probability);

	//�¼�����-----------------------------------------------------------------
	//��Ϸ��ʼ�¼�
	bool OnEventGameStart();
	//��Ϸ�����¼�
	bool OnEventGameConclude(WORD chair_id, BYTE reason);
	bool OnEventSendGameScene(WORD chair_id, BYTE game_status, GameStation& cmd);

	//����켣-----------------------------------------------------------------
	//�����켣
	void BuildInitTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type);
	//
	void BuildFishTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end);
	//����켣
	void ClearFishTrace(bool force = false);

	//��������-----------------------------------------------------------------
	void BuildSceneKind1();
	void BuildSceneKind2();
	void BuildSceneKind3();
	void BuildSceneKind4();
	void BuildSceneKind5();



	//������Ϣ������д-----------------------------------------------------------------
	//����������
	void SendDataToAllPlayers(void * pData, UINT uSize, BYTE bAssID);
	//����ָ�����
	void SendDataToPlayer(BYTE bDeskStation, void * pData, UINT uSize, BYTE bAssID);

	//�Ƿ��ǳ����û�
	bool IsSuperMan(BYTE bDesk);
	//��ȡ����ҵĵ�λ����
	int GetSuperCellscore(BYTE bDesk);
	//��������
	bool ExistUnAI();
	//�����ۻ��ﵽ����Ϸ�ʱ���õĴ���
	int GetTopFen(BYTE bDeskStation);

	//�ַ�ת����UTF_8->GB2312��
	void UTF_8ToUnicode(wchar_t* pOut,char *pText);
	void UnicodeToGB2312(char* pOut,wchar_t uData);
	void UTF_8ToGB2312(char*pOut, char *pText, int pLen);
	//�޸Ľ�������1(ƽ̨->��Ϸ)
	/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPond(bool	bAIWinAndLostAutoCtrl);

	//�޸Ľ�������2(ƽ̨->��Ϸ)
	/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ�
	/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ���
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[]);

	//�޸Ľ�������3(ƽ̨->��Ϸ)
	/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
	/// @return �Ƿ�ɹ�
	virtual bool SetRoomPondEx(__int64	iReSetAIHaveWinMoney);

	//����ʤ��
	/// �ж�
	virtual BOOL Judge();
	/// ʤ
	virtual BOOL JudgeWiner();
	/// ��
	virtual BOOL JudgeLoser();
	//GRM����
	//���½�������(��Ϸ->ƽ̨)
	/// @return ������ӮǮ��
	virtual void UpDataRoomPond(__int64 iAIHaveWinMoney);
public:
	bool m_bExitWatcher[PLAY_COUNT];//�Ƿ�����Թ���� flaseΪ �����ڣ� trueΪ �� ���Ϊ����߼�ID

	char m_cFishName[FISH_KIND_COUNT][MAX_PATH];

private:
	bool			m_bIsplaying;				//��Ϸ������

	BYTE			m_bGameStatus;				//��Ϸ״̬

	SceneKind		next_scene_kind_;			//��һ������ID

	bool			special_scene_;				//���ⳡ��

	WORD			android_chairid_;			//��������λ

	int				current_fish_lk_multiple_;	//��ǰ���ӵı���

	int				fish_id_;					//�������к�
	int				fish_id_LK;					//��ǰ�������к�
	SCORE			exchange_fish_score_[GAME_PLAYER];//�һ��˵ķ���
	SCORE			fish_score_[GAME_PLAYER];	//��һ�õķ���

	int bullet_id_[GAME_PLAYER];

	//ITableFrame* table_frame_;
	//tagGameServiceOption* game_service_option_;
	//tagGameServiceAttrib* game_service_attrib_;
	SCORE base_score_;

	int stock_crucial_score_[10];
	double stock_increase_probability_[10];
	int stock_crucial_count_;

	int				exchange_ratio_userscore_;	//����������û����
	int				exchange_ratio_fishscore_;	//������������
	int				exchange_count_;			//һ�ζһ�����Ŀ�������

	int				min_bullet_multiple_;		//�ӵ���С����
	int				max_bullet_multiple_;		//�ӵ������
	int				bomb_stock_;				//ը�����
	int				super_bomb_stock_;			//����ը�����
	int				fish_speed_[FISH_KIND_COUNT];//��������ٶ�
	int				fish_multiple_[FISH_KIND_COUNT];//������ķ���
	int				fish18_max_multiple_;//18���������
	int				fish19_max_multiple_;//19���������
	int				fishLK_max_multiple_;//���������
	int				fish_bounding_box_width_[FISH_KIND_COUNT];//������߿�Ŀ�
	int				fish_bounding_box_height_[FISH_KIND_COUNT];//������߿�ĸ�
	int				fish_hit_radius_[FISH_KIND_COUNT];//�������˺���Χ
	double			fish_capture_probability_[FISH_KIND_COUNT];//�����㱻����ĸ���
	int				bomb_range_width_;//ը����Χ�Ŀ�
	int				bomb_range_height_;//ը����Χ�ĸ�

	int				bullet_speed_[BULLET_KIND_COUNT];//�����ӵ��ٶ�
	int				net_radius_[BULLET_KIND_COUNT];//�����ӵ����������Ĵ�С
	int				net_bounding_box_width_[BULLET_KIND_COUNT];//�����ӵ����������Ŀ�
	int				net_bounding_box_height_[BULLET_KIND_COUNT];//�����ӵ����������ĸ�

	//��Ĺ켣����
	typedef std::vector<FishTraceInfo*> FishTraceInfoVecor;
	FishTraceInfoVecor active_fish_trace_vector_;//��ǰ�����Ĺ켣
	FishTraceInfoVecor storage_fish_trace_vector_;//��Ĺ켣���ݲֿ�

	std::vector<SweepFishInfo> sweep_fish_info_vector_;

	typedef std::vector<ServerBulletInfo*> ServerBulletInfoVector;
	//ÿ����ҵ��ӵ�����
	ServerBulletInfoVector server_bullet_info_vector_[GAME_PLAYER];
	//�ӵ����ݲֿ�
	ServerBulletInfoVector storage_bullet_info_vector_;

	//��ǰ������
	int m_iFishNum;

	//�����û�����
	std::vector<SuperManInfo> m_SuperMan;

	int kBuildSmallFishTraceElasped;//С�����ʱ��0
	int kBuildMediumFishTraceElasped;//�е������ʱ��1
	int kBuildFish18TraceElasped;//18�������ʱ��2
	int kBuildFish19TraceElasped;//19�������ʱ��3
	int kBuildFish20TraceElasped;//20�������ʱ��4
	int kBuildFishLKTraceElasped;//���ӳ���ʱ��5
	int kBuildFishBombTraceElasped;//ը�������ʱ��6
	int kBuildFishSuperBombTraceElasped;//����ը�������ʱ��7
	int kBuildFishLockBombTraceElasped;//����ը�������ʱ��8
	int kBuildFishSanTraceElasped;//����Ԫ�����ʱ��9
	int kBuildFishSiTraceElasped;//����ϲ�����ʱ��10
	int kBuildFishKingTraceElasped;//��������ʱ��11
};

/******************************************************************************************************/
