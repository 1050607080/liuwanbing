
#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
// ������

#define KIND_ID               2010
#define GAME_NAME             TEXT("��������")
#define GAME_PLAYER           8
#define VERSION_SERVER        100728832       // PROCESS_VERSION(1,0,0)
#define VERSION_CLIENT        100728832       // PROCESS_VERSION(1,0,0)

// �򵥵İ汾���
// ����ͻ����и���Ӧ�ø������ֵ.���ֻ�޸�EXE�İ汾���������ݰ�û�޸ĵĻ������¿ͻ�����ûʲô����ģ���ΪEXE�İ汾�ǿ���ֱ�ӱ��޸ĵ�
// ֻҪ������ǰ��EXE�汾,������֮���ٻ���ȥ�������BUG�Ļ����ڻ���û�޸���
#define GAME_VERSION          30

#ifndef SCORE
#define SCORE __int64/*LONGLONG*/
#endif

const int kResolutionWidth = 1366;
const int kResolutionHeight = 768;

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#define M_PI_2  1.57079632679489661923f
#define M_PI_4  0.785398163397448309616f
#define M_1_PI  0.318309886183790671538f
#define M_2_PI  0.636619772367581343076f
#endif
const float kChairDefaultAngle[GAME_PLAYER] = { M_PI, M_PI, M_PI, 0, 0, 0, -M_PI_2, M_PI_2 };

enum TraceType {
  TRACE_LINEAR = 0,
  TRACE_BEZIER
};

struct FPoint {
  float x;
  float y;
};

struct FPointAngle {
  float x;
  float y;
  float angle;
};

//////////////////////////////////////////////////////////////////////////
// ��Ϸ����

/*
// ��λ��
-------------
    0   1   2 
7               3                
    6   5   4
-------------
*/

enum SceneKind {
  SCENE_KIND_1 = 0,
  SCENE_KIND_2,
  SCENE_KIND_3,
  SCENE_KIND_4,
  SCENE_KIND_5,

  SCENE_KIND_COUNT
};

enum FishKind {
  FISH_KIND_1 = 0,
  FISH_KIND_2,
  FISH_KIND_3,
  FISH_KIND_4,
  FISH_KIND_5,
  FISH_KIND_6,
  FISH_KIND_7,
  FISH_KIND_8,
  FISH_KIND_9,
  FISH_KIND_10,
  FISH_KIND_11,
  FISH_KIND_12,
  FISH_KIND_13,
  FISH_KIND_14,
  FISH_KIND_15,
  FISH_KIND_16,
  FISH_KIND_17,
  FISH_KIND_18,
  FISH_KIND_19,
  FISH_KIND_20, // ���
  FISH_KIND_21, // ����
  FISH_KIND_22, // ����ը��
  FISH_KIND_23, // �ֲ�ը��
  FISH_KIND_24, // ����ը��
  FISH_KIND_25, // ����Ԫ1
  FISH_KIND_26, // ����Ԫ2
  FISH_KIND_27, // ����Ԫ3
  FISH_KIND_28, // ����ϲ1
  FISH_KIND_29, // ����ϲ2
  FISH_KIND_30, // ����ϲ3
  FISH_KIND_31, // ����1
  FISH_KIND_32, // ����2
  FISH_KIND_33, // ����3
  FISH_KIND_34, // ����4
  FISH_KIND_35, // ����5
  FISH_KIND_36, // ����6
  FISH_KIND_37, // ����7
  FISH_KIND_38, // ����8
  FISH_KIND_39, // ����9
  FISH_KIND_40, // ����10

  FISH_KIND_COUNT
};

enum BulletKind {
  BULLET_KIND_1_NORMAL = 0,
  BULLET_KIND_2_NORMAL,
  BULLET_KIND_3_NORMAL,
  BULLET_KIND_4_NORMAL,
  BULLET_KIND_1_ION,
  BULLET_KIND_2_ION,
  BULLET_KIND_3_ION,
  BULLET_KIND_4_ION,

  BULLET_KIND_COUNT
};

const DWORD kBulletIonTime = 20;
const DWORD kLockTime = 10;

const int kMaxCatchFishCount = 2;

// ���ƿͻ��˵�����������
//const int kFishKindCount[FISH_KIND_COUNT] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

//////////////////////////////////////////////////////////////////////////
// ���������

#define SUB_S_GAME_CONFIG                   100
#define SUB_S_FISH_TRACE                    101//��������Ĺ켣
#define SUB_S_EXCHANGE_FISHSCORE            102
#define SUB_S_USER_FIRE                     103//������
#define SUB_S_CATCH_FISH                    104
#define SUB_S_BULLET_ION_TIMEOUT            105//�ӵ���ΧʧЧ
#define SUB_S_LOCK_TIMEOUT                  106//������ʧЧʱ��
#define SUB_S_CATCH_SWEEP_FISH              107
#define SUB_S_CATCH_SWEEP_FISH_RESULT       108
#define SUB_S_HIT_FISH_LK                   109
#define SUB_S_SWITCH_SCENE                  110
#define SUB_S_STOCK_OPERATE_RESULT          111
#define SUB_S_SCENE_END                     112

#define SUB_S_CATCH_SWEEP_FISH_RESULT_EX    114//�㲥֪ͨ������Ҳ�������

#define SUB_S_GAMEOUT						113//�����ǿ���ÿͻ����˳�

struct CMD_S_GameStatus {
  DWORD game_version;
  SCORE fish_score[GAME_PLAYER];
  SCORE exchange_fish_score[GAME_PLAYER];
};

struct CMD_S_GameConfig {
  int exchange_ratio_userscore;
  int exchange_ratio_fishscore;
  int exchange_count;

  int min_bullet_multiple;
  int max_bullet_multiple;

  int bomb_range_width;
  int bomb_range_height;

  int fish_multiple[FISH_KIND_COUNT];
  int fish_speed[FISH_KIND_COUNT];
  int fish_bounding_box_width[FISH_KIND_COUNT];
  int fish_bounding_box_height[FISH_KIND_COUNT];
  int fish_hit_radius[FISH_KIND_COUNT];

  int bullet_speed[BULLET_KIND_COUNT];
  int net_radius[BULLET_KIND_COUNT];
};

struct GameStation//�����ػ��õ����ݰ�sdp
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	BYTE				game_status;//״̬��ʶ

	CMD_S_GameStatus	GameStatus;//״̬����
	CMD_S_GameConfig	GameConfig;//��������

	BYTE GameMessage[MAX_PATH];//��Ϸ��Ϣ
};

struct CMD_S_FishTrace 
{
	FPoint init_pos[5];
	int init_count;
	FishKind fish_kind;
	int fish_id;
	TraceType trace_type;

	void init()
	{
		memset(this,0,sizeof(CMD_S_FishTrace));
	}
};

struct CMD_S_ExchangeFishScore {
  WORD chair_id;
  SCORE swap_fish_score;//��ǰ���ʵ������� ����������
  SCORE exchange_fish_score;
};

struct CMD_S_UserFire {
  BulletKind bullet_kind;
  int bullet_id;
  WORD chair_id;
  WORD android_chairid;
  float angle;
  int bullet_mulriple;
  int lock_fishid;
  SCORE fish_score;
};

struct CMD_S_CatchFish {
  WORD chair_id;
  int fish_id;
  FishKind fish_kind;
  bool bullet_ion;
  SCORE fish_score;
};

struct CMD_S_BulletIonTimeout {
  WORD chair_id;
};

struct CMD_S_CatchSweepFish {
  WORD chair_id;
  int fish_id;
};

struct CMD_S_CatchSweepFishResult {
  WORD chair_id;
  int fish_id;
  SCORE fish_score;
  int catch_fish_count;
  int catch_fish_id[300];
};

struct CMD_S_CatchSweepFishResultEx 
{
	char    cRoomName[61];//��������
	int		exchange_ratio_userscore_;	//����������û����
	int		exchange_ratio_fishscore_;	//������������
	int		iDeskID;			//����ID
	char	szNickName[61]; //�ǳ�
	int		fish_kind;		//������ID
	SCORE	fish_score;		//������׬����Ǯ
	char    cFishName[MAX_PATH];//�������
};

struct CMD_S_HitFishLK {
  WORD chair_id;
  int fish_id;
  int fish_mulriple;
};

struct CMD_S_SwitchScene 
{
  SceneKind scene_kind;
  int fish_count;
  FishKind fish_kind[250];
  int fish_id[250];
};

struct CMD_S_StockOperateResult {
  unsigned char operate_code;
  SCORE stock_score;
};

//////////////////////////////////////////////////////////////////////////
// �ͻ�������

#define SUB_C_EXCHANGE_FISHSCORE            1
#define SUB_C_USER_FIRE                     2//��ҿ���
#define SUB_C_CATCH_FISH                    3
#define SUB_C_CATCH_SWEEP_FISH              4
#define SUB_C_HIT_FISH_I                    5
#define SUB_C_STOCK_OPERATE                 6
#define SUB_C_USER_FILTER                   7
#define SUB_C_ANDROID_STAND_UP              8
#define SUB_C_FISH20_CONFIG                 9
#define SUB_C_ANDROID_BULLET_MUL            10//�������ӵ���Ŀ

struct CMD_C_ExchangeFishScore {
  bool increase;
};

struct CMD_C_UserFire {
  BulletKind bullet_kind;
  float angle;
  int bullet_mulriple;
  int lock_fishid;
};

struct CMD_C_CatchFish {
  WORD chair_id;
  int fish_id;
  BulletKind bullet_kind;
  int bullet_id;
  int bullet_mulriple;
};

struct CMD_C_CatchSweepFish {
  WORD chair_id;
  int fish_id;
  int catch_fish_count;
  int catch_fish_id[300];
};

struct CMD_C_HitFishLK {
  int fish_id;
};

struct CMD_C_StockOperate {
  unsigned char operate_code; // 0��ѯ 1 ��� 2 ���� 3 ��ѯ��ˮ
};

struct CMD_C_UserFilter {
  DWORD game_id;
  unsigned char operate_code; // 0 ������ 1 ������ 2 ���
};

struct CMD_C_Fish20Config {
  DWORD game_id;
  int catch_count;
  double catch_probability;
};

struct CMD_C_AndroidBulletMul {
  WORD chair_id;
  int bullet_id;
  int bullet_mulriple;
  BulletKind bullet_kind;
};

#pragma pack()

#endif // CMD_FISH_H_