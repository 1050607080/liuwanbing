#pragma once
#include<vector>
#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define GAMENAME						TEXT("���")
#define NAME_ID							10100204						//���� ID
#define PLAY_COUNT						4								//��Ϸ����
//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾
//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//��Ϸ��Ϣ
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // �ͻ���Ŀ¼
#define SKIN_CARD						TEXT("CardSkin")		                 // ����ͼƬ�ļ���


//��Ϸ��Ϣ
#define GM_GAMESTATIONCHANGED		50		//��Ϸ״̬��Ϣ
#define GM_GAMEMODECHANGED			51		//��Ϸģʽ
#define GM_GAMEMODESELECTED			52		//��Ϸģʽѡ��ɹ�
#define GM_GETREADY					53		//׼����Ϣ
#define GM_SENDCARD					54		//������Ϣ
#define GM_PK						55		//������Ϣ
#define GM_GETREADY					65		//׼��
#define GM_WASHCARD					66		//ϴ��
#define GM_RESULT					67		//���ƽ��
#define GM_BET						68		//��ע
#define GM_SHOWTOALL				69		//����
#define GM_SETTLE					70		//����
#define GM_GAMEOVER					71		//��Ϸ����
#define GM_BETBANKER			72			//ѡ��ׯ��
#define ASS_HANDSUP					73		//֪ͨ��Ҿ���
#define ASS_SUPER					74		//����
#define ASS_WINORLOSE				75		//��Ӯ



//��Ϸģʽ
enum ENUM_GameMode		
{ 
  GAMEMODE_BANKER_INTURN,	//��ׯ
  GAMEMODE_BANKER_CONST,	//��ׯ
  GAMEMODE_BANKER_BET,		//��ׯ
  GAMEMODE_INVALIDE,		//��Ч
};


//��Ϸ״̬
enum ENUM_GameStation
{
	GS_NOMODE = 80,		//ģʽδѡ��
	GS_MODESETTING,		//ѡ��ģʽ��
	GS_WITHMODE,		//ģʽ��ѡ��
	GS_READY,			//׼��
	GS_GAMEBEGIN,		//��Ϸ��ʼ
	GS_FINDBANKER,		//ȷ��ׯ��
	GS_BETTING,			//��ע
	GS_WASHCARDS,		//ϴ��
	GS_SENDCARDS,		//����
	GS_PK,				//����
	GS_SETTLE,			//����
	GS_GAMEOVER		//��Ϸ����
};

//��Ϸ��λ
enum ENUM_TablePosition
{
	TP_BANKER,		//ׯ��
	TP_SECOND,		//˳��
	TP_THIRD,		//����
	TP_LAST,		//β��
	TP_NOPOSITION	//δ����
};

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_AGREE					1				//�ȴ�ͬ������
#define GS_PLAY_GAME					21				//��Ϸ��
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 
#define EveryCount					    13				//ÿ���������˿�����


#include "UpGradeLogic.h"

//�ͻ��˷��͸������������ݰ�



//�������ݰ�
struct TSuper
{
	TSuper()
	{
		IsSetSuper = false;
		isWinOrLose = FALSE;
	}
	bool IsSetSuper;
	BOOL isWinOrLose;
};

// 1. �������ݰ�
struct T_CS_Base 
{ 
	T_CS_Base()
	{
		m_PositionId = -1;
		m_GameMode = GAMEMODE_INVALIDE;
	}
	int m_PositionId;					//��Ϸ��λ��
	ENUM_GameMode	   m_GameMode;		//��Ϸģʽ
};

// 2.�޸���Ϸ״̬���ݰ�
struct T_CS_ModifyGameStation : public T_CS_Base	
{
	T_CS_ModifyGameStation()
	{
		m_GameStation = GS_NOMODE;
	}
	ENUM_GameStation m_GameStation;
};

//��Ϸ״̬��
struct TGameStation
{
	TGameStation()
	{
		byGameDeskNO = 0;
		enGameStation = GS_NOMODE;
		enGameMode = GAMEMODE_INVALIDE;
	}
	BYTE byGameDeskNO;					//��λ��
	ENUM_GameStation enGameStation;		//��Ϸ״̬
	ENUM_GameMode enGameMode;			//��Ϸģʽ
};


// 3.׼��״̬���ݰ�
struct T_CS_GetReady : public T_CS_Base
{
	T_CS_GetReady()
	{
		m_GetReady = false;
	}
	bool	m_GetReady;
};


// �������ݰ�
struct T_SC_Settle
{
	T_SC_Settle()
	{
		memset(this,0,sizeof(T_SC_Settle));
	}
	char cbNickName[4][61];		//�ǳ�
	bool bIsBanker[4];			//�Ƿ�ׯ��
	EnCardShape enCardShape[4];	//����
	__int64  iScore[4];			//�÷�
};

struct T_SC_WhoIsBanker
{
	T_SC_WhoIsBanker()
	{
		byDeskStation = 255;
	}
	BYTE byDeskStation;
};

// ��ҷ��ƽ�����ݰ�
struct T_CS_OutCard
{
	T_CS_OutCard()
	{
		memset(firstHeap,0,sizeof(firstHeap));
		memset(SecondHeap,0,sizeof(SecondHeap));
		memset(allCards,0,sizeof(allCards));
		byDeskStation = 99;
		cardCount = 0;
	}
	BYTE firstHeap[3];	//��һ��
	BYTE SecondHeap[2];	//�ڶ���	
	BYTE allCards[5];	//���������ݣ���������ʱ���õ�����ʮ������
	BYTE byDeskStation;	//��λ��
	BYTE cardCount;		//�Ƶ�����
};

//�������㲥�������ݰ�
struct T_SC_OutCard
{
	T_SC_OutCard()
	{
		memset(this,0,sizeof(T_SC_OutCard));
	}
	BYTE firstHeap[3];
	BYTE secondHeap[2];
	BYTE byDeskStation;
	EnCardShape enCardShape;
};

// ���ƽ�����ݰ�
struct T_SC_CardResult
{
	T_SC_CardResult()
	{
		enCardShape = CS_INVALID;
		byDeskStation = 99;
	}
	EnCardShape enCardShape;
	BYTE byDeskStation;
};

// Ͷע���ݰ�
struct T_CS_Bet
{
	T_CS_Bet()
	{
		iBetMony = 0;
		byDeskStation = 0;
	}
	__int64 iBetMony;
	BYTE byDeskStation;
};

struct TBetBanker
{
	TBetBanker()
	{
		byDeskStation = 99;
		isBet = false;
	}
	BYTE byDeskStation;
	bool isBet;
};

struct T_SC_Cards
{
	T_SC_Cards()
	{
		memset(m_cards,0,sizeof(m_cards));
	}
	BYTE m_cards[5];
};



//�����Ϸ�е�״̬
struct TPlayerInfo
{
	TPlayerInfo()
	{
		byDeskNumber = 0;
		bIsBanker = false;
	}

	BYTE byDeskNumber;	//��λ��
	bool bIsBanker;		//�Ƿ�Ϊׯ��
};

#define ASS_GAMEMODE					70					//������Ϸģʽ
#define ASS_GAMEMODE_RESULT					71				//������Ϸģʽ���

//��Ϸ��Ϣ
 #define ASS_RESET_STATION					        50					//����
// #define ASS_WASHCARD					        51					//ϴ��
// #define ASS_SEND						        52					//����
// #define ASS_PREPARE								53					//׼�����
// #define ASS_NORMALFINISH				        54					//��Ϸ����������Ϣ
// #define ASS_OUT_CARD_RESULT			            55					//���ƽ��
// #define ASS_SUPER					            56					//������֤
// #define ASS_COMPARE								57					//����
// #define ASS_FIRE						        58					//��ǹ
// #define ASS_AUTO								59					//�й�
// #define ASS_AUTO_RESULT							60					//�йܽ��
// #define ASS_SUPER_CONTROL					    61					//���˿���
// #define ASS_SUPER_CONTROLRES					62					//���˿��ƽ��

#endif
