#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���

#define GAMENAME						TEXT("ʮ���")
#define NAME_ID							12100005 						//���� ID
#define PLAY_COUNT						5								//��Ϸ����
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
#define GS_ROB_NT						20              //��ׯ״̬
#define GS_NOTE                         21    //�µ�ע״̬
#define GS_SEND_CARD					22		//����״̬
#define GS_PLAY_GAME					23	 //��Ϸ��״̬

#define GS_PLAY_FINISH					24   //��Ϸ����״̬
#define GS_WAIT_NEXT					26	 //�ȴ���һ�̿�ʼ 


//��ע����
#define TYPE_GIVE_UP					0x00			//����
#define TYPE_NOTE						0x01			//��ע
#define TYPE_TAKE_CARD                  0x02            //Ҫ��
#define TYPE_STOP_CARD					0x03			//ͣ��
#define TYPE_PLAY_GAME                  0x04            //�����


#define MAX_SHAPE_COUNT                 6              //������������
#define SH_USER_CARD					5               //�Ƶ�����



/********************************************************************************/

//��Ϸ���ݰ�
/********************************************************************************/
/*------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------*/
typedef struct SuperUserSetData
{
	BYTE	byDeskStation;      /**< ���λ�� */
	bool	bSetSuccese;		//�Ƿ����óɹ���
	int		byMaxDesk[PLAY_COUNT];			//����Ӯ����ҵ�λ��
	int		byMinDesk[PLAY_COUNT];			//����������ҵ�λ��
	SuperUserSetData()
	{
		memset(this,255,sizeof(SuperUserSetData));
		bSetSuccese = false;
	}
};


/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
struct GameStation_Base
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	BYTE				byGameStation;					//��Ϸ״̬

	//��Ϸ��Ϣ
	BYTE				iThinkTime;						//Ҫ��ʱ��
	BYTE				iBeginTime;						//׼��ʱ��
	BYTE				iSendCardTime;					//����ʱ��
	BYTE				iCallScoreTime;					//��ׯʱ��
	BYTE				iXiaZhuTime;					//��עʱ��
	BYTE				byLostTime;						//��ʱ��ʵ��ʣ�µ�ʱ�䣬���ڱ�ʾ��������ʱʣ��ʱ��ֵ
	int					iRoomBasePoint;					//���䱶��
	int					iBaseNote;						//��ע

	DWORD				iCardShape;						//��������

	GameStation_Base()
	{
		memset(this,0,sizeof(GameStation_Base));
	}
};
/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_WaiteAgree : public GameStation_Base
{
	bool                bUserReady[PLAY_COUNT] ;        //����Ƿ���׼��

	GameStation_WaiteAgree()
	{
		memset(this,0,sizeof(GameStation_WaiteAgree));
	}
};
/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ��ׯ״̬ ��
struct GameStation_RobNt : public GameStation_Base
{
	BYTE			byCurrentCallScore;					//��ǰ��ׯ�� 
	BYTE			byUserCallStation[PLAY_COUNT];		//�������ׯ���	255-��ʾ��û���� 0-��ʾ���� 1-��ʾ�Ѿ�����
	int				iUserStation[PLAY_COUNT];			//�����״̬ ����Ƿ���;�����
	BYTE			byLostTime;							//��ʱ��ʵ��ʣ�µ�ʱ�䣬���ڱ�ʾ��������ʱʣ��ʱ��ֵ

	GameStation_RobNt()
	{
		memset(this,0,sizeof(GameStation_RobNt));
		memset(byUserCallStation,255,sizeof(byUserCallStation));
	}
};
/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ��ע״̬ ��
struct GameStation_Note : public GameStation_Base
{
	BYTE			byNtStation;							//ׯ��λ��
	BYTE			byLostTime;								//��ʱ��ʵ��ʣ�µ�ʱ�䣬���ڱ�ʾ��������ʱʣ��ʱ��ֵ
	int				iUserStation[PLAY_COUNT];				//�������ע״̬ -1-��ʾ��û���� 0-��ʾ���� 1-��ʾ�Ѿ�����
	__int64			i64PerJuTotalNote[PLAY_COUNT];			//�û�ÿ��ѹ��ע
	__int64			iLimitNote[PLAY_COUNT];					//���ע��
	__int64			i64UserNoteLimite[PLAY_COUNT][4];		//����ĸ���ע����
	
	
	GameStation_Note()
	{
		memset(this,0,sizeof(GameStation_Note));
		memset(iUserStation,-1,sizeof(iUserStation));
	}
};

/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ����״̬ ��
struct GameStation_SendCard : public GameStation_Base
{
	BYTE			byNtStation;							//ׯ��λ��
	BYTE			byLostTime;								//��ʱ��ʵ��ʣ�µ�ʱ�䣬���ڱ�ʾ��������ʱʣ��ʱ��ֵ
	BYTE			iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE			iUserCard[PLAY_COUNT][SH_USER_CARD];	//�û����ϵ��˿�
	int				iUserStation[PLAY_COUNT];				//�����״̬ ����Ƿ���;�����
	__int64			i64PerJuTotalNote[PLAY_COUNT];			//�û�ÿ��ѹ��ע
	
	GameStation_SendCard()
	{
		memset(this,0,sizeof(GameStation_SendCard));
	}
};
/*------------------------------------------------------------------------------*/
//��Ϸ״̬���ݰ�	�� ����״̬ ��
struct GameStation_OpenCard : public GameStation_Base
{
	BYTE			byNtStation;							//ׯ��λ��
	BYTE			byLostTime;								//��ʱ��ʵ��ʣ�µ�ʱ�䣬���ڱ�ʾ��������ʱʣ��ʱ��ֵ
	BYTE			iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE			iUserCard[PLAY_COUNT][SH_USER_CARD];	//�û����ϵ��˿�
	BYTE			byOpenCardCount[PLAY_COUNT];				//�û������Ѿ����Ƶ���Ŀ
	int				iOpenShape[PLAY_COUNT];					//����
	int				iUserStation[PLAY_COUNT];				//�������ע״̬ -1-��ʾ��û���� 0-��ʾ����ע 1-��ʾ�Ѿ���ע
	__int64			i64PerJuTotalNote[PLAY_COUNT];			//�û�ÿ��ѹ��ע

	
	GameStation_OpenCard()
	{
		memset(this,0,sizeof(GameStation_OpenCard));
	}
};
/*------------------------------------------------------------------------------*/

//��ע
struct UserNoteStruct
{
	int                 iNt;                               //ׯ��
	__int64				iLimitNote[PLAY_COUNT];				//���ע��
	__int64				i64UserNoteLimite[PLAY_COUNT][4];	//����ĸ���ע����
};


/*-------------------------------------------------------------------------------*/
/// �������ݰ���һ�ν��˿�ȫ�������ͻ���
struct SendAllCardStruct
{
	BYTE      iStartPos;                  //������ʼλ��
	BYTE      iUserCard[PLAY_COUNT][SH_USER_CARD];   //�û��˿�
	BYTE      iUserCardCount[PLAY_COUNT];

	SendAllCardStruct()
	{
		memset(iUserCard,0,sizeof(iUserCard));
		memset(iUserCardCount,0,sizeof(iUserCardCount));
		iStartPos = 255;
	}
};
/*-------------------------------------------------------------------------------*/
//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;				//���Ƶ�λ��
};


//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	__int64				i64TurePoint[PLAY_COUNT];		//��ҵ÷�
	__int64				iChangeMoney[8];
	int					iUserState[PLAY_COUNT];			//���״̬(��ǰ����,������)
	BYTE				iCardShape[PLAY_COUNT];			//����
	BYTE				iUpGradeStation;				//ׯ��λ��
	BYTE				iCardList[PLAY_COUNT][5];		//�˿���Ϣ
	BYTE				byOpenCardCount[PLAY_COUNT];	//��������
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	__int64				i64TurePoint[PLAY_COUNT];			//ׯ�ҵ÷�
	__int64				iChangeMoney[8];
	int					bDeskStation;					//�˳�λ��
	BYTE                iQuitType;                     //�˳�����
};


//��ׯ���ݰ�
struct CallScoreStruct
{	
	BYTE				bDeskStation;							//��ǰ��ׯ��
	BYTE				byUserState[PLAY_COUNT];				//����ҵ�״̬(����Ƿ���;����� ����һֱ����Ϸ���е����)
	int					iValue;									//��ׯ���ͣ�1�з�,0���з֣�
	bool 				bCallScoreflag;							//��ׯ���		
	bool				bPlayer;								//�Ƿ�Ϊ���
	CallScoreStruct()
	{
		memset(this,0,sizeof(CallScoreStruct));
		memset(byUserState,255,sizeof(byUserState));
	}	
};
/*----------------------------------------------------------*/
//���Ҫ�ƽṹ��
struct TakeCardStruct
{
	BYTE	byDeskStation;		//Ҫ����ҵ�λ��	
	BYTE	byYaoPaiCount;		//Ҫ������
	BYTE	byReadyYaoCard[PLAY_COUNT];//����׼��Ҫ�Ƶ����
	int		iShape;				//����

	TakeCardStruct()
	{
		memset(this,0,sizeof(TakeCardStruct));
		byDeskStation = 255;
	}
};
//���ͣ�ƽṹ��
struct StopCardStruct
{
	BYTE	byDeskStation;		//ͣ����ҵ�λ��	
	BYTE	bReadyStopCard[PLAY_COUNT];//�Ѿ�ͣ�Ƶ����	
	int		iShape;				//����

	StopCardStruct()
	{
		memset(this,0,sizeof(StopCardStruct));
		byDeskStation = 255;
	}
};

//�û���ע������Ϣ
struct tagUserProcess
{
	BOOL bReadyStopCard[PLAY_COUNT];//�Ѿ�ͣ�Ƶ����
	BYTE iVrebType;//������İ�ť
	//BYTE bUpCard[3];
	int  iNoteType;//��ע����
	tagUserProcess()
	{
		memset(this,0,sizeof(tagUserProcess));
	}
};

//�����û�Ѻע
struct NoteResult
{
	BYTE bAddStyle;		//��ע����
	BYTE iOutPeople;	//��ע��
	__int64 iCurNote;		//��ǰ�����ע��
	BYTE NoHitStation;          //ͣ��λ��
};
struct GameFinishNotify
{
	TCHAR name[PLAY_COUNT][21];
	int	iBasePoint;
	BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
	__int64	iWardPoint[PLAY_COUNT];
	//__int64 iTotalPoint[4];
	BYTE iCardShape[PLAY_COUNT];
	__int64 iMoney[PLAY_COUNT];
};


//���ͳ��˿�����Ϣ
struct T_CS_SuperControlData
{
	BYTE	byWinUser[PLAY_COUNT];
	BYTE	byLoseUser[PLAY_COUNT];
	T_CS_SuperControlData()
	{
		memset(byWinUser,0xff,sizeof(byWinUser));
		memset(byLoseUser,0xff,sizeof(byLoseUser));
	}
};
//-------------------------------------------------------------------------------


/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					50				//�û�������Ϸ

#define ASS_CALL_SCORE					51				//֪ͨ�з�		
#define ASS_USER_SCORE					52				//��ҽз�		
#define ASS_CALL_SCORE_RESULT			53				//֪ͨ�зֽ��	
#define ASS_CALL_SCORE_FINISH			54				//֪ͨ�зֽ���	
#define ASS_CALL_NOTE					55				//֪ͨ��ע		
#define ASS_USER_NOTE					56				//�����ע		
#define ASS_CALL_NOTE_RESULT			57				//֪ͨ��ע���	
#define ASS_CALL_SEND_CARD				58				//֪ͨ������Ϣ	
#define ASS_CALL_SEND_FINISH			59				//�������		
#define ASS_GAME_PLAY					60				//��ʼ��Ϸ	
#define ASS_USER_TAKE_CARD              61				//���Ҫ��
#define ASS_TAKE_CARD_RESULT			62				//���Ҫ�ƽ��	
#define ASS_USER_STOP					63				//���ͣ��		
#define ASS_USER_STOP_RESULT			64				//���ͣ�ƽ��
#define ASS_CONTINUE_END				65				//��Ϸ����
#define ASS_NO_CONTINUE_END				66				//��Ϸ����
#define ASS_CUT_END						67				//�û�ǿ���뿪
#define ASS_SALE_END					68				//��Ϸ��ȫ����


#define ASS_SUPER_USER					79				//�����û���Ϣ
#define ASS_SUPER_USER_SET				80				//�����û�������Ϣ
#define ASS_SUPER_USER_SET_RESULT		81				//�����û����ý����Ϣ

#define ASS_CONTROL_WIN					90       /// �����ͻ��˿�����Ӯ
#define ASS_CAN_CONTROL_WIN				91       /// ֪ͨ�����ͻ��˿��Կ�����Ӯ
#define ASS_CONTROL_SUCCEED				92      /** �ɷ���˷��Ϳͻ��˳������óɹ���Ϣ */







/********************************************************************************/
#endif
