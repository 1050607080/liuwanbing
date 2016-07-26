#pragma once
//ͬ¥ ��ͨ��/��Ƶ�� ����  release/��Ƶ��  ��������������������

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#define GAMENAME						TEXT("����")

#ifdef VIDEO
#define NAME_ID							10200404								//���� ID
#else
#define NAME_ID							10200404								//���� ID
#endif


//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME


//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				2				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0				//�ڲ������Ͱ汾��



//��Ϸ��Ϣ
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define PLAY_COUNT						4								//��Ϸ����
#define SKIN_FOLDER						TEXT(INT_TO_STR(NAME_ID))		// �ͻ���Ŀ¼
#define SKIN_CARD						TEXT(".\\image\\cardskin")		//����ͼƬ�ļ���
static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))

#define GET_CLIENT_BCF_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".bcf"))



//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_WASHING_CARD					19				//ϴ��
#define GS_SEND_CARD					20				//����״̬
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

//-------------------------------------------------------------------------------
/********************************************************************************/

// ���ݰ���������ʶ
//warning:79��IDԤ���������ͻ���,����ʹ��
/********************************************************************************/
#define ASS_UG_USER_SET					50				//������Ϸ
#define ASS_GAME_BEGIN					51				//��Ϸ��ʼ
#define ASS_GAME_PREPARE				52				//׼������
#define ASS_SEND_CARD					53				//������Ϣ
#define ASS_SEND_CARD_MSG				54				//���ƹ����д�����Ϣ
#define ASS_SEND_FINISH					56				//�������
#define ASS_GAME_PLAY					76				//��ʼ��Ϸ
#define ASS_OUT_CARD					77				//�û�����
#define ASS_OUT_CARD_RESULT				78				//���ƽY��
#define	ASS_SUPER_USER					79				//������Ϣ
#define ASS_REPLACE_OUT_CARD			80				//�������(79���������ͻ��˷�����)
#define ASS_CONTINUE_END				84				//��Ϸ����
#define ASS_NO_CONTINUE_END				85				//��Ϸ����

#define ASS_GETREADY					86				//׼��OK

#define ASS_WASHINGCARD					88				//ϴ��
#define ASS_GAMELEVEL					89				//��Ϸ����
#define	ASS_CALL						90				//����
#define ASS_SHOWCALLLEVELCARD			91				//��ʾ��������
#define ASS_CALL_COMPLETED				92				//�������
#define	ASS_SENDFINISH					93				//ׯ��
#define ASS_SENDBACKCARD				94				//������
#define ASS_BURYCARD					95				//����
#define ASS_BACKCARDBURIED				96				//�������
#define ASS_OUTCARD_FAILED				97				//����ʧ��
#define ASS_OUTCARD_SUCCESS				98				//���Ƴɹ�
#define ASS_OUTMIXEDCARD_FAILED			99				//˦��ʧ��
#define ASS_SCORE						100				//�÷�
#define ASS_GAMEOVER					101				//��Ϸ����
#define ASS_CONFIGINFO					102				//��Ϸ������Ϣ
#define ASS_SENDOTHERCARD				103				//����������ҵ���
#define ASS_LOOK_TURN					104				//�鿴��һ������
#define ASS_LOOK_TURNRES				105				//�鿴��һ�����ݽ��
#define ASS_NEW_TURN					106				//�µ�һ��
enum eCardShape
{
	E_CS_INVALIED,E_CS_SINGLE/*����*/, E_CS_DOUBLE/*����*/,E_CS_SERIALDOUBLE/*������*/,E_CS_MIXED/*����*/
};
//////////////////////////////////////////////////////////////////////////
//ʱ�ӵδ�
#define COLOCKCICK					15			//15S
/********************************************************************************/
struct TFirstData//������ҳ�������
{
	BYTE byFlower;//��ɫ
	BYTE byCards[25];		//������
	int	cardShape;	//����
	int iCardNums;//������
	int iThrowStyle;//˦������(0:˦����;1:˦ͬһ��ɫ��)
	TFirstData()
	{
		memset(this,0,sizeof(TFirstData));
		iThrowStyle=-1;
	}
};
//���ݰ�
struct TGameStation
{
	TGameStation()
	{
		byGameStation = 255;
	}
	BYTE byGameStation;
};

//׼����
struct TGetReady
{
	TGetReady()
	{
		byDeskStation = 255;
		memset(bUserGetReady,0,sizeof(bUserGetReady));
	}
	BYTE byDeskStation;
	bool bUserGetReady[PLAY_COUNT];
};

//�����ݰ��� ��������
struct TSendCards
{
	BYTE byDeskStation;	//��λ��
	BYTE byCards[25];	//������
	BYTE byCardCount;	//������
	BYTE byBackCard[8];//����
	BYTE byBackCount;//��������
	TSendCards()
	{
		memset(this,0,sizeof(TSendCards));
	}
};
//�������������
struct TOtherCards
{
	TOtherCards()
	{
		memset(this,0,sizeof(TOtherCards));
	}
	TSendCards tsendCards[3];
};

//��Ϸ�����
struct TGameLevel
{
	TGameLevel()
	{
		byDeskStation = 255;
		byGoup1Level = 255;
		byGoup2Level = 255;	
	}
	BYTE byDeskStation;	//��λ��
	BYTE byGoup1Level;	//��һ�鼶��
	BYTE byGoup2Level;	//�ڶ��鼶��
};

//��������
struct TCallLevelCard
{
	BYTE byDeskStation;					//��λ��
	BYTE byCallCard[2];					//��������
	BYTE bySize;						//��������
	BYTE byLevelColor;					//���ƻ�ɫ
	BYTE byLevel;						//����
	TCallLevelCard()
	{
		byLevel=2;
		bySize = 0;
		byDeskStation = 255;
		byLevelColor = 255;
		memset(byCallCard,0,sizeof(byCallCard));
	}
	void init()
	{
		byLevel=2;
		bySize = 0;
		byDeskStation = 255;
		byLevelColor = 255;
		memset(byCallCard,0,sizeof(byCallCard));
	}
};

//�ͻ��˷��͸������������Ƶ�����
struct T_CS_OutCard
{
	T_CS_OutCard()
	{
		byCardNums = 0;
		memset(byCards,0,sizeof(BYTE)*25);
		byDeskStation = 255;
		byFlower=255;
		isFirst = false;
		cardShape = E_CS_INVALIED;
	}

	BYTE byFlower;//��ɫ
	BYTE byCardNums;		//������
	BYTE byCards[25];		//������
	BYTE byDeskStation;		//��λ��
	bool isFirst;			//�Ƿ��һ������
	int	cardShape;	//����
	int iThrowStyle;//˦������(0:˦����;1:˦ͬһ��ɫ��)
};

//�����������ͻ���֪ͨ�������ݰ�, ������һ�ҳ����ƣ�
struct T_SC_OutCardPackage
{
	T_SC_OutCardPackage()
	{
		memset(&tPreoutCards,0,sizeof(T_CS_OutCard));
		byOutCardPlayerDeskStation = 255;
	}
	T_CS_OutCard tPreoutCards;
	BYTE byOutCardPlayerDeskStation;
};
struct T_SC_NewTurn//�µ�һ��
{
	BYTE byDeskStation;//����λ��
	T_SC_NewTurn()
	{
		memset(this,0,sizeof(T_SC_NewTurn));
	}
};
//����ʧ�����ݰ�
struct T_SC_OutCardFailed
{
	T_SC_OutCardFailed()
	{
		memset(this,0,sizeof(T_SC_OutCardFailed));
	}
	WCHAR MsgFailReason[100];
	BYTE byDeskStation;
};

//˦��ʧ�����ݰ�
struct T_SC_MixedCardFailed
{
	T_SC_MixedCardFailed()
	{
		memset(this,0,sizeof(T_SC_MixedCardFailed));
	}
	BYTE byMinCard[25];
	BYTE byCardCount;
	BYTE byDeskStation;
};

//�÷�
struct T_SC_Score
{
	BYTE byDeskStation[2];//�÷�λ��
	int iScore;
	T_SC_Score()
	{
		memset(this,0,sizeof(T_SC_Score));
		memset(byDeskStation,255,sizeof(byDeskStation));
	}
};

//��Ϸ������Ϣ
struct TGameConfigs
{
	TGameConfigs()
	{
		memset(this,0,sizeof(TGameConfigs));
	}
	int iTimeToGetReady;	//׼��ʱ��
	int iTimeToCallNT;		//����ʱ��
	int iTimeToThink;		//˼��ʱ��
	int iSendCardSpeed;		//�����ٶȣ�����
};
//����
struct TGameResult
{
    int iTotalScore;//�ܵ÷�
	int iBackCardScore;//���Ʒ���
	__int64 i64Money[PLAY_COUNT];//��Ӯ���
	TGameResult()
	{
		memset(i64Money,0,sizeof(i64Money));
	}
};
//�鿴ÿһ��������
struct TLookTurnData
{
	bool bFlag;
	TLookTurnData()
	{
		memset(this,0,sizeof(TLookTurnData));
	}
};
//�鿴ÿһ�������ݽ��
struct TLookTurnDataRes
{
	int iCardNums[PLAY_COUNT];		//������
	BYTE byCards[PLAY_COUNT][25];	//������
	TLookTurnDataRes()
	{
		memset(this,0,sizeof(TLookTurnDataRes));
	}
};

#endif
