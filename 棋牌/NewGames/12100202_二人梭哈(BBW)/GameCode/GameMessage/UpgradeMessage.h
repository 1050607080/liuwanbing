#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE


//�ļ����ֶ���
#define GAMENAME						TEXT("���")

//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//��Ϸ��Ϣ
#define PLAY_COUNT						4								//��Ϸ����

#define SKIN_CARD						TEXT("CardSkin")		//����ͼƬ�ļ���

#define MAX_NAME_INFO                   256

//��Ϸ�����汾
#define DEV_HEIGHT_VERSION				8								//�ڲ������߰汾��
#define DEV_LOW_VERSION					4								//�ڲ������Ͱ汾��

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_SEND_CARD					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

//��ע����
#define TYPE_GIVE_UP					0x00			//����
#define TYPE_NOTE						0x01			//��ע
#define TYPE_ADD						0x02			//��ע
#define TYPE_FOLLOW						0x03			//��ע
#define TYPE_SHUTTLE					0x04			//���


#define SHOWHA_TIME						2				//�������Ʊ������
//#define FIRST_CAN_GIVEUP				true			//��һ�ѿɷ���
#define NOTE_LIMIT						200				//��ע����

#ifdef  SSYJ //˭��Ӯ��
#define CLIENT_DLL_NAME					TEXT("10409400.ico")
#define SERVER_DLL_NAME					TEXT("10409400.DLL")
#define NAME_ID							10400400							//���� ID
#define PLAY_COUNT						4							//��Ϸ����
#define SKIN_FOLDER						TEXT("sh")					//ͼƬ�ļ���
#define SKIN							TEXT("cardskin1")					//����ͼƬ����һ��
#endif

#ifdef FOURSH
#define CLIENT_DLL_NAME					TEXT("10409400.ico")
#define SERVER_DLL_NAME					TEXT("10409400.DLL")
#define NAME_ID							10409400							//���� ID
#define PLAY_COUNT						4							//��Ϸ����
#define SKIN_FOLDER						TEXT("sh")					//ͼƬ�ļ���
#define SKIN							TEXT("cardskin1")					//����ͼƬ����һ��
#endif

#ifdef TWOSH
#define SERVER_DLL_NAME					TEXT("10409200.DLL")
#define CLIENT_DLL_NAME					TEXT("10409200.ico")
#define NAME_ID							12100202									//���� ID
#define PLAY_COUNT						2											//��Ϸ����

#define SKIN_FOLDER						TEXT("sh")									//ͼƬ�ļ���
#define SKIN							TEXT("cardskin1")							//����ͼƬ����һ��
#endif


#ifdef VIDEO
#define CLIENT_DLL_NAME					TEXT("10409405.ico")
#define SERVER_DLL_NAME					TEXT("10409405.DLL")
#define NAME_ID							10409405									//���� ID
#define SKIN_FOLDER						TEXT("sh")									//ͼƬ�ļ���
#define SKIN							TEXT("cardskin1")							//����ͼƬ����һ��
#endif

static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)          (sprintf(A,"%d",NAME_ID),strcat(A,".ico")) //��ȡ�ͻ��� 
#define GET_SERVER_DLL_NAME(A)          (sprintf(A,"%d",NAME_ID),strcat(A,".dll")) //��ȡ�������� 
#define GET_SKIN_FOLDER(A)              (sprintf(A,"%d",NAME_ID),strcat(A,""))     //��ȡͼƬ�ļ���
/********************************************************************************/
//#define SUPER_VERSION					//�����ͻ���
#define SET_JUDGE						//����ʤ�ʰ���

/********************************************************************************/

//��Ϸ��ʼ
struct GameBeginStruct
{
	BYTE				iBeenPlayGame;					//��Ϸ�غ�
	__int64				iBaseNote;						//��ע	
	__int64				iLimitNote;						//���ע��
	bool				bUserState[PLAY_COUNT];			//���״̬
	bool				bIsSuperUser;					//�ж��Լ��Ƿ񳬼��ͻ��ˣ��ɷ������������ݣ�ÿ�ֶ�У��
	bool				bChangeRobotCard;				//�Ƿ�ı��˻����˵��� true �ı��� false û�иı�
};

//�ռ����붯��
struct CollectJetonStruct
{
	__int64				JetonTotal;
	BYTE				bDeskStation;
};

//�������ݰ�
struct SendCardStruct
{
	BYTE				iCard;							//�Ʊ��
	BYTE				bDeskStatin;					//��һ�ֳ�����
};
//�������ݰ�
struct SendOneTurnCardStruct
{
	BYTE    iStartPos;                    ///������ʼλ��
	BYTE    iCardList[PLAY_COUNT][5];     ///����
	BYTE    iCardCount[PLAY_COUNT];       /// ��ǰÿ����ҷ��Ƶ�����
	int		iFirstSendCardCount;		  ///��һ�η���������
	SendOneTurnCardStruct()
	{
		memset(this,0,sizeof(SendOneTurnCardStruct));
		iStartPos = 255; 
	}

};
//����һ���˿����ݰ�
struct SendOneTurnCard
{
	BYTE                iTurnCard[PLAY_COUNT];     //һ���˿�����
	BYTE                bStartPos;                 //��ʼλ��
	SendOneTurnCard()
	{
		memset(iTurnCard,0,sizeof(iTurnCard));
		bStartPos = 255 ;
	}
};

//��ע
struct LimitStruct
{
	BYTE				iNoteTime;						//��ע����
	__int64				iLimit;							//��ע
};

//���ƽY�����ݰ�
struct SendCardFinishStruct
{
	BYTE				bDeskStatin;					//�������
};

//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				bDeskStaton;					//���Ƶ�λ��
};

//�����û����
struct tagUserResult
{
	BYTE				bAddTimes;						//�ӱ����
	BYTE				bNoteTimes;						//Ѻע����
	BYTE				iNowBigNoteStyle;				//��ע����
	BYTE				iOutPeople;						//��ע��
	BYTE				m_byUserAddNum[PLAY_COUNT];		//���ָ�����Ҽ�ע���� 2011-12-13
	__int64				iNowBigNote;					//��ǰ��ע
	__int64				iTotalNote;						//����ע
	bool				bNewTurn;						//��һ��

	tagUserResult()
	{
		memset(m_byUserAddNum,0,sizeof(m_byUserAddNum));
		bNewTurn = false;
	}
};

//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{	
	BYTE				iCardShape[PLAY_COUNT];					//����
	BOOL				bShowCardShape[PLAY_COUNT];				//��ʾ����
	int					iUserState[PLAY_COUNT];					//�ļ�״̬(��ǰ����,������)
	__int64				iTurePoint[PLAY_COUNT];					//��ҵ÷�
	__int64				iChangeMoney[PLAY_COUNT];
};

//��Ϸ��ǰ������
struct GameAheadStruct
{
	BYTE				bDeskStation;					//ʤ��
	__int64				iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64				iChangeMoney[PLAY_COUNT];				//���
};
//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					bDeskStation;					//�˳�λ��
	__int64				iTurePoint[PLAY_COUNT];			//ׯ�ҵ÷�
	__int64				iChangeMoney[PLAY_COUNT];
};

struct HaveThingStruct
{
	BYTE				pos;
	char				szMessage[61];
};

struct LeaveResultStruct
{
	BYTE				bDeskStation;
	int					bArgeeLeave;
};

struct MessageStruct							
{
	char				 Message[200];
};

struct StopResultStruct
{
	bool				bArgee;
};


//�û�������Ϣ
struct tagUserProcess
{
	BYTE				iVrebType;					//������İ�ť
	__int64				iNote;						//��ע��
};

//��������û�������Ϣ
struct ReplaceUserNoteStruct
{
	BYTE				bDeskStation;				//�����λ��
	BYTE				iVrebType;					//������İ�ť
	__int64				iNote;						//��ע��
};

//�������й�
struct UseAIStation
{
	BYTE				bDeskStation;				//����
	BOOL				bState;						//״̬
};

//�����û�Ѻע
struct NoteResult
{
	BYTE				bNoteStyle;					//��ע����
	BYTE				iOutPeople;					//��ע��
	__int64				iCurNote;					//��ǰ��ע��
	__int64				iPlayerTotalNote[PLAY_COUNT];	//������Ϸ�и������ע������  xyh 20110818
	
};

//����
struct GameFinishNotify
{
	TCHAR				name[PLAY_COUNT][61];
	BYTE				iCardShape[PLAY_COUNT];		//����
	__int64			    iBasePoint;
	BYTE				iStyle;			
	__int64				iWardPoint[PLAY_COUNT];
	__int64				iMoney[PLAY_COUNT];

	GameFinishNotify()
	{
		memset(this,0,sizeof(GameFinishNotify));
	}
};
//colin90511����Ϣ
struct CardInfo
{
    BYTE                bDeskStation;              //��λ��
	BYTE                iUserCard[5];               //������ֵ                 
};

struct GameStation_Base
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	int					iRoomBasePoint;					//���䱶��
	int				    iRunPublish;					//���ܿ۷�

	BYTE				bGameStation;					//��Ϸ״̬
	//��Ϸ��Ϣ
	BYTE				iThinkTime;						//��Ϸ����
	BYTE				iBeginTime;						//��Ϸ����
	BYTE				iAllCardCount;					//�˿���Ŀ
	__int64				iLimitNote;						//���ע
	__int64				iBaseNote;						//��ע
	BYTE                byUserFollow;					//�ڼ��ѱظ�
	BYTE                bShowHa;                        //�ڼ��ֿ������
	bool				bUserState[PLAY_COUNT];			//���״̬
	bool				bIsSuperUser;					//�ж��Լ��Ƿ񳬼��ͻ��ˣ��ɷ������������ݣ�ÿ�ֶ�У��
	BYTE				m_byAddNum;					/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
	BYTE                bPK;                            ///�Ƿ�PK

	GameStation_Base()
	{
		memset(this,0,sizeof(GameStation_Base));
	}
};

//��Ϸ״̬���ݰ�
struct GameStation_1 : GameStation_Base
{
	BYTE				bOtherdata;						//Ԥ������
	GameStation_1()
	{
		memset(this,0,sizeof(GameStation_1));
	}
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2 : GameStation_Base
{
	bool				bAgree[PLAY_COUNT];				//����Ƿ�׼��
	BYTE				bOtherdata;						//Ԥ������
	GameStation_2()
	{
		memset(this,0,sizeof(GameStation_2));
	}
};

//��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
struct GameStation_3 : GameStation_Base
{
	__int64				iTotalNote[PLAY_COUNT];			//�û���ǰ��ע
	int					iUserStation[PLAY_COUNT];		/**< ��¼�û�״̬ */
	//����״̬����
	int					iUpGradePeople;					//ׯ��λ��
	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	//״̬��Ϣ
	BYTE				iUserCardCount[PLAY_COUNT];		//�û����ϵ�����
	BYTE				iUserCard[PLAY_COUNT][5];		//�û����ϵ���
	GameStation_3()
	{
		memset(this,0,sizeof(GameStation_3));
	}
};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4 : GameStation_Base	
{
	//����״̬����
	int					iUpGradePeople;					//ׯ��λ��
	int					iOutCardPeople;					//���ڳ����û�
	int					iFirstOutPeople;				//�ȳ��Ƶ��û�
	__int64				iTotalNote[PLAY_COUNT];			//�û���ǰ��ע
	__int64				iThisTurnNote[PLAY_COUNT];		//��ǰ������ע
	
	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	tagUserResult		tag;
	BYTE				iUserCardCount[PLAY_COUNT];		//�û����ϵ�����
	BYTE				iUserCard[PLAY_COUNT][5];		//�û����ϵ���
	
	BOOL				ibNetCutPlayer[PLAY_COUNT];	    //�������
	int					iUserStation[PLAY_COUNT];		/**< ��¼�û�״̬ */
	BYTE				m_byUserAddNum[PLAY_COUNT];		//���ָ�����Ҽ�ע���� 2011-12-13
	GameStation_4()
	{
		memset(this,0,sizeof(GameStation_4));
	}
};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5 : GameStation_Base
{
	int					iUserStation[PLAY_COUNT];		/**< ��¼�û�״̬ */
	bool				bAgree[PLAY_COUNT];				//����Ƿ�׼��
	//����״̬����
	int					iUpGradePeople;					//ׯ��λ��

	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	GameStation_5()
	{
		memset(this,0,sizeof(GameStation_5));
	}
};
/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					50				//�û�������Ϸ
#define ASS_UG_USER_SET_CHANGE			51				//������ø���
#define ASS_GAME_BEGIN					52				//��Ϸ��ʼ

#define ASS_SEND_CARD					53				//������Ϣ
#define ASS_SEND_FINISH					54				//�������
#define ASS_GAME_PLAY					55				//��ʼ��Ϸ
#define ASS_NOTE_LIMIE					56				//������ע

#define ASS_NOTE						60				//��ע
#define ASS_VREB_CHECK					61				//�û�����
#define ASS_REPLACE_VREB_CHECK			62				//����(����)�û�����
#define ASS_NOTE_RESULT					63				//��ע���

#define ASS_COLLECT_JETON				70				//�ռ�����
#define ASS_AI_STATION					71				//�������й�

#define ASS_NEW_TURN					80				//��һ�ֿ�ʼ
#define ASS_SEND_A_CARD					81				//�·�һ����

#define ASS_CONTINUE_END				90				//��Ϸ����
#define ASS_NO_CONTINUE_END				91				//��Ϸ����
#define ASS_CUT_END						92				//�û�ǿ���뿪
#define ASS_SALE_END					93				//��Ϸ��ȫ����
#define ASS_NO_WINER					94				//��Ϸ������ʤ����
#define ASS_AHEAD_END					95				//��ǰ����

#define ASS_HAVE_THING					100
#define ASS_LEFT_RESULT					101
#define ASS_MESSAGE						102
#define ASS_STOP_THING					103				//������ǰ����
#define ASS_AGREE_STOP					104
#define ASS_PLAY_FLASH                  105             //colin����Flash����
#define ASS_BEGIN_FLASH                 106 

/********************************************************************************/
#endif
