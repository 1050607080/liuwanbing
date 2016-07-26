#pragma once
#include<vector>
#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#ifdef VIDEO
#define GAMENAME						TEXT("ţţ")
#define NAME_ID							10900405							//���� ID
#define PLAY_COUNT						4								//��Ϸ����
#else
#ifdef TWO_NN
#define GAMENAME						TEXT("ţţ(VIP)")
#define NAME_ID							10900200							//���� ID
#define PLAY_COUNT						2								//��Ϸ����
#else
#define GAMENAME						TEXT("21��")
#define NAME_ID							10500400							//���� ID
#define PLAY_COUNT						4								//��Ϸ����
#endif
#endif
//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"MatchTable"
//��Ϸ��Ϣ

#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // �ͻ���Ŀ¼
#define SKIN_CARD						TEXT("CardSkin")		                 // ����ͼƬ�ļ���


//��Ϸ�����汾
#define DEV_HEIGHT_VERSION				3								//�ڲ������߰汾��
#define DEV_LOW_VERSION					10 								//�ڲ������Ͱ汾��

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_BET			                20              //��ע״̬
#define GS_SEND							21				//����״̬
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 



const int iNote[18]=
{
1,2,5,10,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000
};

const int iDouble[12]={1,5,10,50,100,500,1000,5000,10000,50000,100000,500000};

//��Ϸ���ݰ�
/********************************************************************************/
//��Ϸ״̬���ݰ� ����Ϸ�ȴ���ʼ״̬��
struct GameStation_1	
{

	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	BYTE				bGameStation;					//��Ϸ״̬
	BYTE				bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	BYTE				bGameBetTime;					//��Ϸ��עʱ��
	BYTE                bCallCardTime;					//��Ϸ����ʱ��
	int					bUserReady[PLAY_COUNT] ;		//���׼��״̬
	GameStation_1()
	{
		memset(this,0,sizeof(GameStation_1));
	}
};

//��Ϸ״̬���ݰ�	�� ��Ϸ��ע״̬ ��
struct GameStation_2
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	BYTE				bGameStation;					//��Ϸ״̬
	BYTE				bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	BYTE				bGameBetTime;					//��Ϸ��עʱ��
	BYTE                bCallCardTime;					//��Ϸ����ʱ��
	int	                bUserReady[PLAY_COUNT] ;		//���׼��״̬
	GameStation_2()
	{
		memset(this,0,sizeof(GameStation_2));
	}
};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_3
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	BYTE				bGameStation;					//��Ϸ״̬
	BYTE				bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	BYTE				bGameBetTime;					//��Ϸ��עʱ��
	BYTE                bCallCardTime;					//��Ϸ����ʱ��
	int	                bUserReady[PLAY_COUNT] ;		//���׼��״̬

	GameStation_3()
	{
		memset(this,0,sizeof(GameStation_3));
	}
};

//��Ϸ״̬���ݰ�	�� �ȴ���һ�̿�ʼ״̬ ��
struct GameStation_4	
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	BYTE				bGameStation;					//��Ϸ״̬
	BYTE				bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	BYTE				bGameBetTime;					//��Ϸ��עʱ��
	BYTE                bCallCardTime;					//��Ϸ����ʱ��
	int	                bUserReady[PLAY_COUNT] ;		//���׼��״̬
	GameStation_4()
	{
		memset(this,0,sizeof(GameStation_4));
	}

};
struct AgreeGame
{
	BOOL flag;									//����ʶ
	BOOL bUserReady[PLAY_COUNT];				//����Ƿ�׼��
	AgreeGame()
	{
		memset(this,0,sizeof(AgreeGame));
	}
};
//��ť���ݰ�
struct GameStationButton
{
	BYTE		bDeskStation;				//�����
	int			iBtType;					//��ť����
	int			iBtParam;					//��ť����
	__int64		i64Note;					//��ע����
	GameStationButton()
	{
		memset(this,0,sizeof(GameStationButton));
	}
};
//ׯ����Ϣ��
struct GameMessageNt
{
	BOOL flag;				//����ׯ�ұ�ʶ
	BYTE bNtStation;		//ׯ��λ��
	GameMessageNt()
	{
		memset(this,0,sizeof(GameMessageNt));
		bNtStation=255;
	}
};
//��ע��Ϣ��
struct GameMessageBet
{
	BOOL        flag;			//��ע��ʾ
	BYTE		bDeskStation;	//��λ��
	__int64		i64Not;			//����ֵ
	__int64		i64BetLimit[PLAY_COUNT];	//��ע����
	__int64		i64Bet[PLAY_COUNT][3];		//��ע����
	GameMessageBet()
	{
		memset(this,0,sizeof(GameMessageBet));
		bDeskStation=255;
	}
};
//������Ϣ��
struct GameMessageSendCard
{
	BOOL flag;					//���Ʊ�ʶ
	BYTE bFirstPeople;			//��һ��������
	BYTE bUserCard[PLAY_COUNT][2];		//�û��˿�
	GameMessageSendCard()
	{
		memset(this,0,sizeof(GameMessageSendCard));
		bFirstPeople=255;
	}
};
//������Ϣ��
struct GameMessageStart
{
	BOOL flag;					//���Ʊ�ʶ
	BYTE bFirstScoreStation;	//��һ���з���
	GameMessageStart()
	{
		memset(this,0,sizeof(GameMessageStart));
		bFirstScoreStation=255;
	}
};
//��Ϸ��ȫ�������ݰ�
struct GameMessageEnd
{
	BOOL flag;					//���Ʊ�ʶ
	GameMessageEnd()
	{
		memset(this,0,sizeof(GameMessageEnd));
	}
};
//��Ϸ�����������ݰ�
struct GameMessageNormalFinish
{
	BOOL		flag;								//���Ʊ�ʶ
	int			iUserPoint[PLAY_COUNT];			//��ҵ���
	__int64		i64UserScore[PLAY_COUNT];		//�����Ӯ���
	GameMessageNormalFinish()
	{
		memset(this,0,sizeof(GameMessageNormalFinish));
	}
};
//��ע���
struct GameMessageBetResult
{
	BYTE		bDeskStation;		//��ǰ��עλ��
	__int64		i64Note;			//��ע���
	GameMessageBetResult()
	{
		memset(this,0,sizeof(GameMessageBetResult));
	}
};

//Ҫ����Ϣ��
struct GameMessageNeedCard
{
	bool bFlag;							//�ܷ�Ҫ��
	__int64	i64Note;					//�ӱ�����ע
	int  iStyle;						//Ҫ������
	int  iPoints;						//Ҫ�Ƶ���
	BYTE bDeskStation;					//Ҫ��λ��
	BYTE bNextStation;					//��һ��Ҫ��λ��
	BYTE bCard;							//��Ҫ�Ƶ�����
	GameMessageNeedCard()
	{
		memset(this,0,sizeof(GameMessageNeedCard));
		bFlag=true;
	}
};

//-------------------------------------------------------------------------------
/// ֪ͨ�ͻ����Ƿ�Ϊ�����ͻ���״̬��Ϣ�ṹ
typedef struct SuperUserState
{
	BYTE byDeskStation;      /**< ���λ�� */
	bool bEnable;           /**< �Ƿ�ͨ */
	SuperUserState()
	{
		memset(this,0,sizeof(SuperUserState));
	}
}SUPERSTATE;

//�����û����ƵĽ��
struct	SuperUserControlReslut
{
	bool    bySuperReslut;		//�����û�����
	BYTE	byDeskStation;		//�����û���λ��
	SuperUserControlReslut()
	{
		memset(this,0,sizeof(SuperUserControlReslut));

	}
};
/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/
//��Ϸ��Ϣ
#define GAME_BUTTON					100				//��ť��Ϣ
#define ASS_AGREE					101				//��ע��Ϣ
#define ASS_NT						102				//ׯ��
#define ASS_BET						103				//��ע��Ϣ
#define ASS_SEND					104				//������Ϣ
#define ASS_PLAY					105				//��ʼ��Ϸ
#define ASS_BETRESULT				106				//��ע���
#define ASS_SAFEEND					107				//��Ϸ��ȫ����
#define ASS_CALL					108				//������Ϣ
#define ASS_CALLFINISH				109				//�з���Ϣ
#define ASS_NORMALFINISH			110				//��Ϸ��������
#define ASS_RESTART					111				//���¿�ʼ��Ϸ
/********************************************************************************/



//�����Ϣ
#define CLICK_START                 500				//��ʼ
#define CLICK_OPERATE               501				//����
#define CLICK_BET					502				//��ע
#define CLICK_BETOPERATE			503				//��ע����
#define CLICK_PLAYOPERATE			504				//��Ϸ�в���


//������������Ϣ�ĸ�����ʶ

#define SERVER_EXTRA_DOUBLE							500			//�ӱ�
#define SERVER_EXTRA_CLICK_GIVEUP                   501			//����
#define SERVER_EXTRA_CLICK_NEED					    502			//Ҫ��
#define SERVER_EXTRA_CLICK_STOP			            503			//ͣ��
#define SERVER_EXTRA_CLICK_TIMEEND                  504			//ʱ�䵽��һ���˽з�

#define ASS_SUPER_USER					70				//�����û���Ϣ
#define ASS_SUPER_RESULT				71				//�����û����ƽ����Ϣ��
#endif
