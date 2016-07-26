#pragma once
#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE


#define GAMENAME						TEXT("510K")
#define NAME_ID							10100403						//���� ID
#define PLAY_COUNT						3								//��Ϸ����

//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"MatchTable"

#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // �ͻ���Ŀ¼


//��Ϸ�����汾
#define DEV_HEIGHT_VERSION				3								//�ڲ������߰汾��
#define DEV_LOW_VERSION					10 								//�ڲ������Ͱ汾��

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_SEND							20				//����״̬
#define GS_PLAY_GAME					21				//��Ϸ��״̬
#define GS_WAIT_NEXT					25				//�ȴ���һ�̿�ʼ 

#define CARD_NUMS						18				//ÿ�������ӵ���˿���Ŀ


//����˷����ͷ������ݰ�

struct GameStation
{

	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	BYTE				iThinkTime;						//˼��ʱ��
	BYTE				iBeginTime;						//��ʼ�ȴ�ʱ��
	int					iGameStation;					//��Ϸ״̬
	int                 bUserReady[PLAY_COUNT] ;        //����Ƿ���׼��
	GameStation()
	{
		memset(this,0,sizeof(GameStation));
	}
};

struct GameStation_0
{

	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	BYTE				iThinkTime;						//˼��ʱ��
	BYTE				iBeginTime;						//��ʼ�ȴ�ʱ��
	BYTE                bNextPeople;					//��ǰ�з���
	BYTE				bCardList[CARD_NUMS];			//���е���
	BYTE				bLeftCard[CARD_NUMS];			//ҪѺ����
	BYTE				bHavePoint[PLAY_COUNT][12];		//����з���
	BYTE				DeskBehave[PLAY_COUNT][CARD_NUMS];	//��ұ���
	BYTE                bRoundHaveCard[12];				//�����з���
	int					iPeopleHandCount[PLAY_COUNT];	//���������
    int					iGameStation;					//��Ϸ״̬
	int				    iClockLabel;					//ʱ�ӱ�ǩ
	__int64				iPoint[PLAY_COUNT];				//��ҷ���
	GameStation_0()
	{
		memset(this,0,sizeof(GameStation_0));
	}
};


//�û�ͬ����Ϸ
struct GameServerToClient_ArgeeGame
{
	BYTE								bDeskNO;							///��Ϸ����
	BYTE								bDeskStation;						///λ�ú���
	BYTE								bAgreeGame;							///ͬ���־
	GameServerToClient_ArgeeGame()
	{
		memset(this,0,sizeof(GameServerToClient_ArgeeGame));
	}
};
//�й�
struct GameServerToClient_Auto
{
	bool								bIsAuto;							//�Ƿ��й�
	BYTE								bDeskStation;						///λ��
	GameServerToClient_Auto()
	{
		memset(this,0,sizeof(GameServerToClient_Auto));
	}
};
/// �������ݰ���һ�ν��˿�ȫ�������ͻ���
struct GameServerToClient_SendCard
{
	BYTE      iUserCard[PLAY_COUNT][CARD_NUMS];   //�û��˿�
	BYTE      iUserCardCount[PLAY_COUNT];		  //�û��˿�����
	BYTE      bNtPeople;						  //��ǰׯ��
	GameServerToClient_SendCard()
	{
		memset(this,0,sizeof(GameServerToClient_SendCard));
		memset(iUserCard,255,sizeof(iUserCard));
		memset(iUserCardCount,0,sizeof(iUserCardCount));
	}
};

//��Ϸ��ʼ���ݰ�
struct GameServerToClient_StartGame
{
	BYTE				iOutDeskStation;				//������ʼλ��
	GameServerToClient_StartGame()
	{
		memset(this,0,sizeof(GameServerToClient_StartGame));
		iOutDeskStation=255;
	}
};

//֪ͨ��һ����ҳ������ݰ�
struct GameServerToClient_NextPeople
{
	bool				bLeftFlag;					//�ϼ��Ƿ����
	BYTE				bNextStation;				//Ҫ���Ƶ����
	BYTE				bLeftStation;				//�ϼ�
	BYTE				bCardList[CARD_NUMS];		//�ϼ���������
	BYTE				bHaveCard[12];				//�����з���
	int					iPeopleCount[PLAY_COUNT];	//��ǰ���������
	__int64				i64PeoplePoint[PLAY_COUNT];			//��ǰ��ҵ÷�

	GameServerToClient_NextPeople()
	{
		memset(this,0,sizeof(GameServerToClient_NextPeople));
		bNextStation=255;
	}
};
//�ͷ�����ʾ�÷���
struct GameServerToClient_StyleCard
{
	BYTE				bDeskStation;				//����Ӯ��
	BYTE				bCardList[12];				//����Ӯ����Ӯ����
	GameServerToClient_StyleCard()
	{
		memset(this,0,sizeof(GameServerToClient_StyleCard));
		bDeskStation=255;
	}
};
//���һ��̯��
struct GameServerToClient_ShowCard
{
	BYTE				bLastStation;				//�������һ��
	BYTE				bRemainCard[CARD_NUMS];		//���һ��ʣ�����
	GameServerToClient_ShowCard()
	{
		memset(this,0,sizeof(GameServerToClient_ShowCard));
		bLastStation=255;
	}
};
//����
struct GameServerToClient_Finish
{
	__int64	         i64Point[PLAY_COUNT];		//��ҵ÷�
	BYTE			 iRanking[PLAY_COUNT];		//����
	GameServerToClient_Finish()
	{
		memset(this,0,sizeof(GameServerToClient_Finish));
		
	}
};
//���߽���
struct GameServerToClient_CutEnd
{
	bool bFlag;
	GameServerToClient_CutEnd()
	{
		memset(this,0,sizeof(GameServerToClient_CutEnd));

	}
};
//�鿴���������Ʒ�
struct GameServerToClient_SeeStyle
{
	__int64	         i64Point;		        //���������Ʒ�
	BYTE			 bStyleCard[12];       //�����з���
	GameServerToClient_SeeStyle()
	{
		memset(this,0,sizeof(GameServerToClient_SeeStyle));
		
	}
};
//�ͷ��˷������������ݰ�
//�û��������ݰ�
struct GameClientToServer_OutCardMsg
{
	int					iNextDeskStation;				//��һ������
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������
	BYTE				iCardList[39];					//�˿���Ϣ
};
//��ť���ݰ�
struct GameClientToServer_Button
{
	bool		bFlag;						//�Ƿ��Լ�����(��־λ���ݰ�ť���Ͷ��в�ͬ����)
	BYTE		bDeskStation;				//�����
	int			iBtType;					//��ť����
	int			iBtParam;					//��ť����
	int			iCardCount;					//������������
	BYTE		bCardList[CARD_NUMS];		//��������
	GameClientToServer_Button()
	{
		memset(this,0,sizeof(GameClientToServer_Button));
	}
};
//��Ϸ��Ϣ(�ͷ��˸������)
#define CLIENTTOSERVER_BUTTON					100				//��ť��Ϣ
#define CLIENTTOSERVER_AGREE                    101             //ͬ����Ϸ		
//��Ϸ��Ϣ(����˸��ͷ���)
#define SERVERTOCLIENT_AGREE					200					//������Ϣ
#define SERVERTOCLIENT_SEND						201					//������Ϣ
#define SERVERTOCLIENT_START					202					//��ʼ��Ϸ��Ϣ
#define SERVERTOCLIENT_NEXT					    203					//֪ͨ��һ����ҳ�����Ϣ
#define SERVERTOCLIENT_SHOW					    204					//չʾ�÷���Ϣ
#define SERVERTOCLIENT_NORMALFINISH				205					//��Ϸ����������Ϣ
#define SERVERTOCLIENT_AUTO					    206					//�й���Ϣ
#define SERVERTOCLIENT_HANDUP					207					//���һ��̯����Ϣ
#define SERVERTOCLIENT_CUTEND					208					//���߽���
//�����Ϣ
#define CLICK_START								500				//��ʼ
#define CLICK_OPERATE							501				//����
#define CLICK_OUTCARD							502				//����
#define CLICK_NOOUT							    503				//����
#define CLICK_TIP							    504				//��ʾ
#define CLICK_AUTO							    505				//�й�,�鿴�Ʒ֣��������
#define CLICK_SET							    506			    //��Ϸ����
#endif
