#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

////�ļ����ֶ���
//#ifdef VIDEO
//#define GAMENAME						TEXT("ţţ")
//#define NAME_ID							10900405							//���� ID
//#define PLAY_COUNT						4								//��Ϸ����
//#else
//#ifdef TWO_NN
//#define GAMENAME						TEXT("ţţ(VIP)")
//#define NAME_ID							10900200							//���� ID
//#define PLAY_COUNT						2								//��Ϸ����
//#else
//
//#endif
//#endif

#define GAMENAME						TEXT("ͨ��ţţ")
#define NAME_ID							12100706							//���� ID
#define PLAY_COUNT						6								//��Ϸ����
//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"MatchTable"

//��Ϸ��Ϣ
static TCHAR szTempStr[MAX_PATH] =		{0};	
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // �ͻ���Ŀ¼	                 // ����ͼƬ�ļ���
#define GET_SKIN_FOLDER(A)				(sprintf(A,"%d",NAME_ID),strcat(A,""))
#define SEC								1000			/**<һ��*/
#define USER_HAND_CARD					5				/**<�û���������*/
#define ALL_CARD_COUNT					54				/**<ʹ��������*/

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_SEND_CARD					20				//����״̬
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

#define MAX_SHAPE_COUNT                 15              //������������
/********************************************************************************/

//��Ϸ���ݰ�
//#define SUPER_VERSION
/********************************************************************************/

///��ȡ�����ļ�������
typedef struct IniDataStruct
{
	int		iBaseNote;					//��Ϸ�׷�
	BYTE	byBTime;					//��ʼ˼��ʱ��
	BYTE	bySCTime;					//��Ϸ����ʱ��
	BYTE	byTTime;					//����˼��ʱ��
	BYTE	byRTime;					//����ȴ�ʱ��
	DWORD	iCardShape;					//��������
	BYTE	iACCount;					//�˿���Ŀ

	IniDataStruct()
	{
		::memset(this,0,sizeof(IniDataStruct));
	}

	IniDataStruct operator=(IniDataStruct IDate)
	{
		this->iBaseNote = IDate.iBaseNote;
		this->byBTime = IDate.byBTime;
		this->bySCTime = IDate.bySCTime;
		this->byTTime = IDate.byTTime;
		this->byRTime = IDate.byRTime;
		this->iCardShape = IDate.iCardShape;
		this->iACCount = IDate.iACCount;
		return *this;
	}
}IniData;

typedef struct GameBaseData
{
	BYTE	bGameStation;				//��Ϸ״̬
	int		iRBPoint;					//���䱶��
	int		iRPublish;					//���ܿ۷�
	bool	bIsInGame[PLAY_COUNT];		//����Ƿ�����Ϸ��
	bool	bHaveBaiPai[PLAY_COUNT];	//����Ƿ����
	bool	bIsAuto[PLAY_COUNT];		//����Ƿ��й�
	bool    bIsSuper;					//�Ƿ����ʹ��������

	GameBaseData()
	{
		bGameStation = GS_WAIT_SETGAME;
		iRBPoint = 0;
		iRPublish = 0;
		::memset(bIsInGame,false,sizeof(bIsInGame));
		::memset(bHaveBaiPai,false,sizeof(bHaveBaiPai));
		::memset(bIsAuto,false,sizeof(bIsAuto));
		bIsSuper = FALSE;
	}

	GameBaseData operator=(GameBaseData _BDate)
	{
		this->bGameStation = _BDate.bGameStation;
		this->iRBPoint = _BDate.iRBPoint;
		this->iRPublish = _BDate.iRPublish;
		this->bIsSuper = _BDate.bIsSuper;
		for(int i = 0; i < PLAY_COUNT; ++i)
		{
			this->bIsInGame[i] = _BDate.bIsInGame[i];
			this->bHaveBaiPai[i] = _BDate.bHaveBaiPai[i];
			this->bIsAuto[i] = _BDate.bIsAuto[i];		
		}

		return *this;
	}

}BData;

struct GameStationBase
{
	BData	sBData;				//��Ϸ��������
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2 : public  GameStationBase
{
	IniData				s_Data;							//����˻�������
};

//��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
struct GameStation_3 : public  GameStationBase
{
	IniData				s_Data;							//����˻�������

	int					iUCCount[PLAY_COUNT];			//�����������

	BYTE				byUCard[USER_HAND_CARD];		//�������

	GameStation_3()
	{
		::memset(iUCCount,0,sizeof(iUCCount));
		::memset(byUCard,0,sizeof(byUCard));
	}
};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4 : public  GameStation_3	
{
	IniData				s_Data;							//����˻�������

	BYTE                byDownCard[USER_HAND_CARD];		//��Ұڵ�ţ��

	int					iSurplusTime;					//ʣ���ʱ��

	GameStation_4()
	{
		::memset(byDownCard,0,sizeof(byDownCard));
		iSurplusTime = 0;
	}
};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5 : public  GameStationBase
{
	IniData				s_Data;							//����˻�������
};


//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	bool				bIsInGame[PLAY_COUNT];			//����Ƿ�����Ϸ��

	BeginUpgradeStruct()
	{
		::memset(bIsInGame,false,sizeof(bIsInGame));
	}
};

/// �������ݰ���һ�ν��˿�ȫ�������ͻ���
struct SendCardStruct
{
	BYTE		byUCard[PLAY_COUNT][USER_HAND_CARD];	//�û��˿�
	int			iUCCount[PLAY_COUNT];

	SendCardStruct()
	{
		memset(byUCard,0,sizeof(byUCard));
		memset(iUCCount,0,sizeof(iUCCount));
	}
};

//�û�������Ϣ
struct TanPaiStruct
{
	BYTE	byStaiton;							//�������λ��
	BYTE	byDownCard[USER_HAND_CARD];			//�¶���
	BYTE	byShape;							//����


	TanPaiStruct()
	{
		byStaiton = 255;
		byShape = 255;
		::memset(byDownCard,0,sizeof(byDownCard));
	}
};

//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	bool				bIsPrepare[PLAY_COUNT];//�Ƿ�׼��
	bool				bIsCowUser[PLAY_COUNT];//�Ƿ�ţţ���
	BYTE                byDownCard[PLAY_COUNT][USER_HAND_CARD];//�¶���
	BYTE				byUCard[PLAY_COUNT][USER_HAND_CARD];//����
	BYTE				byCowUserNum;//ţţ�������
	int					iCardShape[PLAY_COUNT];
	__int64				iMoney[PLAY_COUNT];//������

	GameEndStruct()
	{
		::memset(iCardShape,-1,sizeof(iCardShape));
		::memset(bIsPrepare,0,sizeof(bIsPrepare));
		::memset(byDownCard,0,sizeof(byDownCard));
		::memset(byUCard,0,sizeof(byUCard));
		::memset(iMoney,0,sizeof(iMoney));
		::memset(iMoney,0,sizeof(iMoney));
		::memset(bIsCowUser, false, sizeof(bIsCowUser));
		byCowUserNum = 0;
	}
};

//�й����ݰ�
struct AutoCardStruct
{
	BYTE				bDeskStation;				//�����λ��			
	bool				bAutoCard;					//�Ƿ��й�;true:�й�;false:ȡ���й�

	AutoCardStruct()
	{
		::memset(this,0,sizeof(AutoCardStruct));
	}
};

//�Թ����ݰ�
struct WatchOtherStruct
{
	bool				bWatch;			/**<�Ƿ�����Թ�*/			

	WatchOtherStruct()
	{
		memset(this,0,sizeof(WatchOtherStruct));
	}
};

//���ǿ�����ݰ�
struct UserQuitStruct
{
	BYTE				bDeskStation;				//�����λ��		
	UserQuitStruct()
	{
		memset(this,0,sizeof(UserQuitStruct));
	}
};

//�����������ݰ�
struct SuperStruct
{
	BYTE				bSuper;				//���������λ��
	BYTE				bDeskStation;		//�����������λ��
	BYTE				bSuperTag;			//��������(0Ϊ��ʼֵ��1ΪӮ��2Ϊ��)
	SuperStruct()
	{
		memset(this,0,sizeof(SuperStruct));
		bSuper = 255;
		bDeskStation = 255;
	}
};

/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					50				//�û�������Ϸ
#define ASS_BEGIN_UPGRADE				51				//��Ϸ��ʼ
#define ASS_SEND_CARD					52				//������Ϣ
#define ASS_TAN_PAI						53				//̯����Ϣ
#define ASS_CONTINUE_END				65				//��Ϸ����
#define ASS_AUTO						66				/**<�й�*/
#define ASS_SUPER						67				//����������Ϣ

#define ASS_WATCH_OTHER					102				/**<�Թ���Ϣ*/
#define ASS_USER_QUIT_GAME				103				/**<���ǿ��*/

/********************************************************************************/
#endif
