#pragma once

//ͬ¥ ��ͨ��/��Ƶ�� ����  release/��Ƶ��  ��������������������

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#define GAMENAME						TEXT("����")


#define NAME_ID							12100006						//���� ID


//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//��Ϸ��Ϣ

#define PLAY_COUNT						6								//��Ϸ����
#define MAX_CARD_COUNT                  4


//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				3				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					1				//�ڲ������Ͱ汾��

//#define SKIN_FOLDER						TEXT("ddz")	//ͼƬ�ļ���
//#define SKIN							TEXT("cardskin1")	//����ͼƬ����һ��
#define SKIN_CARD						TEXT("cardskin")		//����ͼƬ�ļ���
static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_SKIN_FOLDER(A)              (sprintf(A,"%d",NAME_ID),strcat(A,""))     //��ȡͼƬ�ļ���
#define GET_KEY_NAME(A, B, C, D)			(sprintf(A,"%s%d%s", B, C, D),strcat(A,""))
#define GET_CLIENT_BCF_NAME(A)          (sprintf(A,"%d",NAME_ID),strcat(A,".bcf"))    

#define SUPER_PLAYER                    10                         //�������
#define MAX_NAME_INFO                   256

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_AGREE					1				//�ȴ�ͬ������
#define GS_SEND_CARD					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 


//----------------------------------------------------------------------
#define GS_SET_BOBO_TIME						0x00			//��������ʱ��
#define GS_SEND_CARD_TIME						0x01			//����ʱ��
#define GS_PLAY_GAME_TIME						0x02			//����Ϸʱ��
#define GS_SPARE_CARD_TIME						0x03			//����ʱ��
//----------------------------------------------------------------------
//��ע����




#define TYPE_OPENCARD					0x04			//����
#define TYPE_LOOKCARD					0x05			//����
#define TYPE_NORMAL                     0x06            //����״̬

#define TYPE_OPEN_LOSE                  0x06            //����ʧ��
#define TYPE_OPEN_WIN                   0x07            //����ʧ��
#define TYPE_REACH_LIMITE_NEXT          0x08            //��һ����ע��ﵽ����
#define TYPE_REACH_LIMITE_SELF          0x09            //�Լ���ע��ﵽ����
#define TYPE_COMPARE_CARD               0x09            //���Ʋ���

//��ע����
#define TYPE_FOLLOW						0x00			//��
#define TYPE_ALL_NOTE					0x01			//��
#define TYPE_ADD						0x02			//��
#define TYPE_PAUSE						0x03			//��
#define TYPE_GIVE_UP					0x04			//��
/********************************************************************************/

//��Ϸ���ݰ�

/********************************************************************************/
//�����û����
struct tagUserResult
{
	BYTE iNowBigPeople;
	BYTE iCutOutLastpeople;
	BYTE iOutPeople;			//��ע��
	int iTotalNote;				//����ע
    int iUserStation[PLAY_COUNT] ;//�����ע����
	int iFirstOutPeople;         //��һλ�����
	__int64 iUserNote[PLAY_COUNT];   //�û�������ע
};

struct GameStationBase
{
	BYTE bGameStation;
};
//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��

struct GameStation_1 : GameStationBase
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2 : GameStationBase
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
	BYTE				iThinkTime;						//��Ϸ����
	BYTE				iBeginTime;						//��Ϸ����
	BYTE				bySeparateCardTime;				//����ʱ��
	BYTE				byBoBoTime;						//��������ʱ��
	bool                bUserReady[PLAY_COUNT] ;		///���׼��
	//   ���������Ϣ
	__int64					i64BoBase;						//��Ƥ
	__int64					i64SmallBobo;					//��С����
};

//��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
struct GameStation_3 : GameStationBase
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	BYTE				iBeginTime;						//��ʼ��Ϸʱ��
	BYTE				byBoBoTime;						//��������ʱ��
	BYTE				iThinkTime;						//˼��ʱ��
	BYTE				bySeparateCardTime;				//����ʱ��
	BYTE				iTimeRest;						//ʵ������ʱ��
	//״̬��Ϣ
	BYTE				byGameStation;									//״̬С�ֽ�
	BYTE				byAlreadySendCardCount[PLAY_COUNT];				//�Ѿ�������
	BYTE				iUserCardCount[PLAY_COUNT];						//�û����ϵ�����
	BYTE				iUserCard[PLAY_COUNT][MAX_CARD_COUNT];			//�û����ϵ���	
	//����״̬����
	int					iUpGradePeople;						//ׯ��λ��
	bool				bAlreadSetBobo[PLAY_COUNT];		//�����������Ƿ��Ѿ�������
	bool                bPlayer[PLAY_COUNT];
	__int64				i64Bobo[PLAY_COUNT];			//�û������õ�������ֵ
	__int64				i64BoBase;						//��Ƥ
	__int64				i64SmallBobo;					//��С����
};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4 : GameStationBase
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
	BYTE				iBeginTime;						//��ʼʱ��
	BYTE				byBoBoTime;						//��������ʱ��
	BYTE				iThinkTime;						//˼��ʱ��
	BYTE				bySeparateCardTime;				//����ʱ��
	BYTE                bNtPeople;                      //ׯ��λ��
	BYTE				iUpGradePeople;					//��עλ��
	BYTE				iOutCardPeople;					//���ڳ����û�
	BYTE				iFirstOutPeople;				//�ȳ��Ƶ��û�
	BYTE				byGameStation;					//����С�ֽ�
	BYTE				byOperationState[PLAY_COUNT];	//�üǵĲ���
	BYTE				iUserCardCount[PLAY_COUNT];					//�û����ϵ�����
	BYTE				iUserCard[PLAY_COUNT][MAX_CARD_COUNT];		//�û����ϵ���
	BYTE				byAlreadySendCardCount[PLAY_COUNT];			//�Ѿ�������
	BYTE				byNoteTime[PLAY_COUNT];						//��ע�Ĵ���
	int                 iCurNote;									//��һλ�����עֵ���Ѱ�ע����
	int				    iTimeRest;									// ��ʱ��ʵ��ʣ�µ�ʱ�䣬���ڱ�ʾ��������ʱʣ��ʱ��ֵ
	bool                bPlayer[PLAY_COUNT];	
	//����״̬����
	__int64				i64NoteMoney[PLAY_COUNT][3];	//ÿ�����µ�ע��Ϣ
	__int64				iTotalNote[PLAY_COUNT];			//�û���ǰ��ע
	__int64             iThisGameNote[PLAY_COUNT];		
	__int64				iTotalGameNote[PLAY_COUNT];		//������Ϸ��ע��
	__int64				i64Bobo[PLAY_COUNT];			//�û������õ�������ֵ
	__int64				i64BoBase;						//��Ƥ
	__int64				i64SmallBobo;					//��С����
	tagUserResult		tag;
};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5 : GameStationBase
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
	BYTE				iBeginTime;							//��ʼʱ��
	BYTE				byBoBoTime;							//����ʱ��
	BYTE				iThinkTime;							//˼��ʱ��
	BYTE				bySeparateCardTime;					//����ʱ��
	
	//����״̬����
	int					iUpGradePeople;					//ׯ��λ��
	__int64				i64BoBase;						//��Ƥ
	__int64				i64SmallBobo;					//��С����
};

//�û�ͬ����Ϸ
struct UserArgeeGame
{
	BYTE				iBeginStation;					//��ʼλ��
	BYTE				iEndStation;					//����λ��
	BYTE				iPlayCount;						//��Ϸ����
	BYTE				iCardCount;						//�˿���Ŀ
};

//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	bool				bPlay[PLAY_COUNT];				//��¼�Ƿ�����;�����
	BYTE				AllCardCount;					//һ��ʹ�õ�����
	BYTE                bNtStation;                     //ׯ��
	__int64				i64BoBase;						//��Ƥ
	__int64				i64SmallBobo;					//��С����
	__int64				i64BoboData[PLAY_COUNT];		//��������
};
//�ռ����붯��
struct CollectJetonStruct
{
	int JetonTotal;
	BYTE bDeskStation;
};
//�������ݰ�
struct SendCardStruct
{
	BYTE				bCard[PLAY_COUNT][MAX_CARD_COUNT];					//�Ʊ��
	BYTE                bCardCount[PLAY_COUNT];                             //������
	BYTE				byNoteTime[PLAY_COUNT];								//�ڼ�����ע
	__int64				i64BoboMoney[PLAY_COUNT];
	__int64				i64dNoteData[PLAY_COUNT][3];						//���µĳ�������
	SendCardStruct()
	{
		memset(bCard,0,sizeof(bCard));
		memset(bCardCount,0,sizeof(bCardCount));
		memset(i64BoboMoney,0,sizeof(i64BoboMoney));
		memset(byNoteTime,0,sizeof(byNoteTime));
		memset(i64dNoteData,0,sizeof(i64dNoteData));
	}
};


//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;							//���Ƶ�λ��
	BYTE                bNtPeople;									 //ׯ��λ��
	BYTE				byUserOperatorType[PLAY_COUNT];				//��ҵĲ���
	__int64				i64dBobo[PLAY_COUNT];						//�û���������
	BeginPlayStruct()
	{
		memset(byUserOperatorType,0xff,sizeof(byUserOperatorType));
		memset(i64dBobo,0x00,sizeof(i64dBobo));
	}
};

//֪ͨ����
struct T_Notice_SendCard
{
	BYTE bySendCardFirstDesk;							//���Ƶ���ʼλ��
	BYTE bySendCardCount;								//Ҫ���Ƶ�����
};

//��Ϸ����ͳ�����ݰ�
struct T_SC_GameEndStruct
{
	BYTE				byFinishType;							//��������
	__int64				i64TotalNote[PLAY_COUNT];				//�û����µĳ�������
	__int64				i64WinMoney[PLAY_COUNT];				//�����Ӯ�õĽ����
	__int64				i64NetIncome[PLAY_COUNT];				//������
	T_SC_GameEndStruct()
	{
		memset(i64TotalNote,0x00,sizeof(PLAY_COUNT));
		memset(i64WinMoney,0x00,sizeof(PLAY_COUNT));
		memset(i64NetIncome,0x00,sizeof(PLAY_COUNT));
	}
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int						bDeskStation;								//�˳�λ��
	__int64					iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64					iChangeMoney[8];
};

struct HaveThingStruct
{
	BYTE pos;
	char szMessage[61];
};

struct LeaveResultStruct
{
	BYTE bDeskStation;
	int bArgeeLeave;
};

struct MessageStruct	//�ͻ��˵�������
{
	char Message[200];
};

struct StopResultStruct
{
	bool bArgee;
};
//�������ݰ�
struct SetCardStruct
{
	BYTE bCardCount;//������
	BYTE bPlayCount;//�����
	BYTE bPlayCard[8][60]; // ÿ����ҵ���
	int  nPlayCardCount[8]; //ÿ�����������
	SetCardStruct()
	{
		memset(this,0,sizeof(SetCardStruct));
	}
};

//�û�������Ϣ
struct tagUserProcess
{
	BYTE iVrebType;		//������İ�ť
	__int64  iNote;		//��ע��
	tagUserProcess()
	{
		memset(this,0,sizeof(tagUserProcess));
	}
};

struct tagCompareCard :  public tagUserProcess 
{
	BYTE byComparedStation ;  //���Ա��Ƶ����   
};

//��������û�������Ϣ
struct SupersedeUserNote
{
	BYTE bDeskStation;//�����λ��
	BYTE iVrebType;//������İ�ť
	int  iNote;		//��ע��
};
//�Q���Ɣ����Y��
struct ChangeBackStruct
{
	BYTE bDeskStation;//λ��
	BYTE bSourceCard;//Դ��
	BYTE bResultCard;//�Y����
	BOOL bExtVal;	//�Ƿ���Q�ɹ�
};

//�����û�Ѻע
struct NoteResult
{
	BYTE iOutPeople;									//������ע��
	BYTE bNextDeskStation;								//��һ�����
	BYTE byOperatType;									//������������
	BYTE byNoteTime;									//�ڼ��μ�ע
	BYTE byFirstOutPeople;								//��һ�����Ƶ���
	__int64	i64FrontNote;								//ǰ������ע��ֵ
	__int64	i64NoteMoney;								//��עֵ
};
struct GameFinishNotify
{
	TCHAR name[PLAY_COUNT][61];
	__int64				i64TotalNote[PLAY_COUNT];				//�û����µĳ�������
	__int64				i64WinMoney[PLAY_COUNT];				//�����Ӯ�õĽ����
	__int64				i64NetIncome[PLAY_COUNT];				//������
};

//���������ṹ��
struct tSetBoboStruct
{
	BYTE byMyDesk;
	__int64 i64BoboMoney;
	tSetBoboStruct()
	{
		byMyDesk = 0xff;
		i64BoboMoney = 0x00;
	}
};

//���ƽ����ṹ��
struct tSendOneCardFinsh
{
	BYTE iOutDeskStation;								//��ǰҪ����֮��
	BYTE byUserOperatorType[PLAY_COUNT];				//��ҵĲ���
	tSendOneCardFinsh()
	{
		iOutDeskStation = 0x00;
		memset(byUserOperatorType,0xff,sizeof(byUserOperatorType));
	}
};

//֪ͨ���Է���
struct tNoticeSeparetCard
{
	BYTE bySeparetCard;
	tNoticeSeparetCard()
	{
		bySeparetCard = 0xff;
	}
};

//���ƽṹ��
struct T_CS_SeparatCard
{
	BYTE byDeskStation;						//������λ��
	BYTE bySeparatCard[MAX_CARD_COUNT];		//���Ƶ�����
	BYTE byLeftCardShap;					//�������
	BYTE byRightCardShap;					//�ұ�����
	T_CS_SeparatCard()
	{
		byDeskStation = 0xff;
		byLeftCardShap = 0xff;
		byRightCardShap = 0xff;
		memset(bySeparatCard,0x00,sizeof(bySeparatCard));
	}
};

//���Ƴɹ���Ϣ
struct T_SC_SeparatSuccess
{
	BYTE byDeskStation;
	T_SC_SeparatSuccess()
	{
		byDeskStation = 0xff;
	}
};

//����������ݰ�
struct T_SC_SeparatFinish
{
	BYTE  bySeparatCard[PLAY_COUNT][MAX_CARD_COUNT];			//������
	BYTE  byCardCount[PLAY_COUNT];
	T_SC_SeparatFinish()
	{
		memset(&bySeparatCard,0xff,sizeof(bySeparatCard));
		memset(byCardCount,0x00,sizeof(byCardCount));
	}
};
/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					50				//�û�������Ϸ
#define ASS_BEGIN_UPGRADE				51				//������Ϸ��ʼ(����������)
#define ASS_SEND_CARD					52				//������Ϣ
#define ASS_SEND_FINISH					54				//�������
#define ASS_GAME_PLAY					59				//��ʼ��Ϸ
#define ASS_NEW_TURN					64				//��һ�ֿ�ʼ
#define ASS_CONTINUE_END				65				//��Ϸ����
#define ASS_NO_CONTINUE_END				66				//��Ϸ����
#define ASS_CUT_END						67				//�û�ǿ���뿪
#define ASS_SALE_END					68				//��Ϸ��ȫ����
#define ASS_EARLY_END					69				//��ǰ����

#define ASS_VREB_CHECK					179				//�û�����
#define ASS_SET_BOBO_DATA				180				//����������
#define ASS_CS_SEPARAT_CARD				181				//������Ϣ
#define ASS_SC_SEPARAT_CARD				182				//���Ƴɹ���Ϣ

#define ASS_SHUTTLE						80				//����
#define ASS_ADD_NOTE					82				//��ע
#define ASS_GIVE_UP						86				//����
#define ASS_MODIFY_PLAY_NOTE			90				//�����û�����ע
#define ASS_NOTE						91				//��ע
#define ASS_GM_SUPERCLIENT				100				//�����͑���           
#define ASS_AI_WIN                      102             //�����˱�Ӯ

#define ASS_SET_TEST_CARD               120             //�������趨��

#define ASS_SC_NOTICE_SEND_CARD			121				//֪ͨ����
#define ASS_SC_SEND_ONE_CARD_FINSH		122				//��һ���ƽ���
#define ASS_SC_NOTIC_SEPARET_CARD		123				//֪ͨ���Է���
#define ASS_SC_SHOW_ALL_SEPARAT_CARD	124				//֪ͨ������ҷ������
/********************************************************************************/
#endif
