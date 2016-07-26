#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���

#define GAMENAME						TEXT("����ţţ")
#define NAME_ID							12100604						//���� ID
#define PLAY_COUNT						4								//��Ϸ����
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
#define GS_NOTE                         21     //�µ�ע״̬
#define GS_SEND_CARD					22		//����״̬
#define GS_OPEN_CARD					24	//��ţ״̬
#define GS_WAIT_NEXT					23	//�ȴ���һ�̿�ʼ 



//��ע����
#define TYPE_GIVE_UP					0x00			//����
#define TYPE_RESET                      0x01            //Ҫ��
#define TYPE_OPEN                       0x02            //����
#define TYPE_BULL						0x03			//ţ
#define TYPE_PLAY_GAME                  0x04            //�����
#define TYPE_NOTE						0x06			//��ע

#define MAX_SHAPE_COUNT                 15              //������������
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
	BYTE	byMaxDesk;			//���������ҵ�λ��
	BYTE	byMinDesk;			//������С��ҵ�λ��
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
	BYTE				iThinkTime;						//̯��ʱ��
	BYTE				iBeginTime;						//׼��ʱ��
	BYTE				iSendCardTime;					//����ʱ��
	BYTE				iCallScoreTime;					//��ׯʱ��
	BYTE				iXiaZhuTime;					//��עʱ��

	BYTE				iAllCardCount;					//�˿���Ŀ

	int					iRoomBasePoint;					//���䱶��
	int					iBaseNote;						//��ע

	DWORD				iCardShape;						//��������
	__int64             i64AcculateGrade[PLAY_COUNT];	//�ۻ��ĳɼ�
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
	int				iUserStation[PLAY_COUNT];				//�������ע״̬ -1-��ʾ��û���� 0-��ʾ���� 1-��ʾ�Ѿ�����
	BYTE			iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE			iUserCard[PLAY_COUNT][SH_USER_CARD];	//�û����ϵ��˿�

	BYTE			byOpenUnderCount[PLAY_COUNT];			//��������
	BYTE			byOpenUnderCard[PLAY_COUNT][3];			//���Ƶ�������
	BYTE			byOpenUpCard[PLAY_COUNT][2];			//��������2����
	int				iOpenShape[PLAY_COUNT];					//��ţ����

	__int64			i64PerJuTotalNote[PLAY_COUNT];			//�û�ÿ��ѹ��ע
	
	GameStation_OpenCard()
	{
		memset(this,0,sizeof(GameStation_OpenCard));
	}
};
/*------------------------------------------------------------------------------*/

//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	BYTE				AllCardCount;					//һ��ʹ�õ�����	
	__int64				iLimitNote[PLAY_COUNT];						//���ע��
	__int64				i64UserNoteLimite[PLAY_COUNT][4];	//����ĸ���ע����
	int                 iNt;                            //ׯ��
	BeginUpgradeStruct()
	{
		memset(this,0,sizeof(BeginUpgradeStruct));
	}
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
	BeginPlayStruct()
	{
		memset(this,0,sizeof(BeginPlayStruct));
	}
};








//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	int					iUserState[PLAY_COUNT];					//�ļ�״̬(��ǰ����,������)

	BYTE				iCardShape[PLAY_COUNT];
	__int64				iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64				iChangeMoney[8];
	BYTE				iUpGradeStation;				//ׯ��λ��
	BYTE                iUpBullCard[PLAY_COUNT][3];      //�������
	BYTE				iCardList[PLAY_COUNT][10];					//�˿���Ϣ
	__int64				i64AcculateGrade[PLAY_COUNT];//����ۻ��ĳɼ�
	GameEndStruct()
	{
		memset(this,0,sizeof(GameEndStruct));
	}
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					bDeskStation;					//�˳�λ��
	__int64					iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64				iChangeMoney[8];
	BYTE                iQuitType;                     //�˳�����
	GameCutStruct()
	{
		memset(this,0,sizeof(GameCutStruct));
	}
};


//�з����ݰ�
struct CallScoreStruct
{	

	BYTE				bDeskStation;							//��ǰ�з���
	int					iValue;									//�з����ͣ�1�з�,0���з֣�
	bool 				bCallScoreflag;							//�зֱ��		
	bool				bPlayer;
	BYTE				byUserState[PLAY_COUNT];				//����ҵ�״̬(����Ƿ���;����� ����һֱ����Ϸ���е����)
	CallScoreStruct()
	{
		memset(this,0,sizeof(CallScoreStruct));
		memset(byUserState,255,sizeof(byUserState));
	}
	
};
/*----------------------------------------------------------*/
//���̯�ƽṹ��
struct UserTanPai
{
	BYTE	byDeskStation;		//̯����ҵ�λ��
	BOOL	bReadyOpenCard[PLAY_COUNT];//�Ѿ�̯�Ƶ����
	BYTE	byUnderCount;		//��������
	BYTE	byUnderCard[3];		//���Ƶ�������
	BYTE	byUpCard[2];		//��������2����
	int		iShape;				//��ţ����

	UserTanPai()
	{
		memset(this,0,sizeof(UserTanPai));
		byDeskStation = 255;
	}
};

//�û�������Ϣ
struct tagUserProcess
{
	BOOL bReadyOpenCard[PLAY_COUNT];//�Ѿ�̯�Ƶ����
	BYTE iVrebType;//������İ�ť
	BYTE bUpCard[3];
	int  iNoteType;//��ע��
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
	NoteResult()
	{
		memset(this,0,sizeof(NoteResult));
	}
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
	GameFinishNotify()
	{
		memset(this,0,sizeof(GameFinishNotify));
	}
};
//�뿪����Ϣ��
struct LeaveDeskMes
{
	BYTE byDeskStation;
	LeaveDeskMes()
	{
		byDeskStation=255;
	}
};
//�뿪����Ϣ��
struct NotifyComeMes
{
	BYTE byDeskStation;
	__int64 i64AcculateGrade;//����ۻ��ĳɼ�
	NotifyComeMes()
	{
		byDeskStation=255;
	}
};
//-------------------------------------------------------------------------------


/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					50				//�û�������Ϸ

#define ASS_CALL_SCORE					51				//֪ͨ�з�		DWJ
#define ASS_USER_SCORE					52				//��ҽз�		DWJ
#define ASS_CALL_SCORE_RESULT			53				//֪ͨ�зֽ��	DWJ
#define ASS_CALL_SCORE_FINISH			54				//֪ͨ�зֽ���	DWJ
#define ASS_CALL_NOTE					55				//֪ͨ��ע		DWJ
#define ASS_USER_NOTE					56				//�����ע		DWJ
#define ASS_CALL_NOTE_RESULT			57				//֪ͨ��ע���	DWJ
#define ASS_CALL_SEND_CARD				58				//֪ͨ������Ϣ	DWJ
#define ASS_CALL_SEND_FINISH			59				//�������		DWJ
#define ASS_CALL_OPEN					60				//֪ͨ���ư�ţ	DWJ
#define ASS_USER_OPEN					61				//��Ұ�ţ		DWJ
#define ASS_CALL_OPEN_RESULT			62				//��Ұ�ţ���	DWJ



#define ASS_CONTINUE_END				65				//��Ϸ����
#define ASS_NO_CONTINUE_END				66				//��Ϸ����
#define ASS_CUT_END						67				//�û�ǿ���뿪
#define ASS_SALE_END					68				//��Ϸ��ȫ����
#define ASS_LEAVE_DESK					69				//����뿪��Ϸ����Ϣ
#define ASS_COME_DESK					70				//��ҽ�����Ϸ��




#define ASS_SUPER_USER					79				//�����û���Ϣ
#define ASS_SUPER_USER_SET				80				//�����û�������Ϣ
#define ASS_SUPER_USER_SET_RESULT		81				//�����û����ý����Ϣ







/********************************************************************************/
#endif
