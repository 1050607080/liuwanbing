#pragma once

//ͬ¥ ��ͨ��/��Ƶ�� ����  release/��Ƶ��  ��������������������
#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
//---------------------------------------------------------------------------------------//
//�ļ����ֶ���
#define GAMENAME						TEXT("������")
#ifdef VIDEO
#define NAME_ID							10321800								//���� ID
#else
#define NAME_ID							10321800								//���� ID
#endif
//---------------------------------------------------------------------------------------//
//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾
//---------------------------------------------------------------------------------------//
//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//---------------------------------------------------------------------------------------//
//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				2				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0				//�ڲ������Ͱ汾��
//---------------------------------------------------------------------------------------//
//��Ϸ��Ϣ
#define PLAY_COUNT						180								//��Ϸ����
#define SKIN_CARD						TEXT(".\\image\\cardskin")		//����ͼƬ�ļ���
static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_CLIENT_BCF_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".bcf"))
//---------------------------------------------------------------------------------------//
//��Ϸ״̬����
static	const	int	GS_WAIT_SETGAME		=	0;			//�ȴ���������״̬
static	const	int	GS_WAIT_ARGEE		=	1;			//�ȴ�ͬ������

static	const	int	GS_GAME_COMPARE		=	19;			//��Ϸ׼���׶�

static	const	int	GS_FREE_STATUS		=	20;			//��Ϸ���н׶�
static	const	int	GS_BET_STATUS		=	21;			//��Ϸ��ע�׶�
static	const	int	GS_SEND_STATUS		=	22;			//���ƽ׶�

static	const	int	GS_WAIT_NEXT		=	23;			//�ȴ���һ�̿�ʼ�׶�


static	const	BYTE	BET_ARES	=	3;			//��ע������
#define ASS_SUPER_USER					70				//�����û���Ϣ

#define ASS_AHEAD_END					95				/** ��ǰ���� */


#define ASS_CUT_END				  103		 // �û�ǿ���뿪
#define ASS_SAFE_END			  104		 // �û���ȫ����

//��ʱ������
#define TIME_FREE_TIME		32	//���ж�ʱ��
#define TIME_XIAZHU_TIME	33 //��ע��ʱ��
#define TIME_SEND_TIME		34 //���ƶ�ʱ��

#define BETSTYLE 6  //��������
// ���ݰ���������ʶ(50-99)
//warning:79��IDԤ���������ͻ���,����ʹ��
/********************************************************************************/
#define ASS_UG_USER_SET					50				//������Ϸ
#define ASS_BEGIN_APPLY					51				//��ʼ������ׯ��Ϣ
#define ASS_CHAOGUO				52				//��ע���������Լ��Ľ��

#define ASS_GAME_FREE					53				//������Ϣ
#define ASS_BEGIN_BET					54				//��ʼ��ע
#define ASS_GAME_SEND					55				//��ʼ����
#define ASS_GAME_COME					56				//��ҽ�����Ϸ��Ϣ

#define ASS_START					57				//��ʼ



#define ASS_GAME_SENDCARD				58				//��ʼ����
#define ASS_GAME_ACCOUNT                    59              //��Ϸ����

#define ASS_APPLY_NT					60				//������ׯ��Ϣ
#define ASS_APPLY_RESULT				61				//������ׯ�����Ϣ
#define ASS_APPLY_LEAVE					62				//������ׯ��Ϣ
#define ASS_LEAVE_RESULT				63				//������ׯ�����Ϣ
#define ASS_USER_BET					64				//�����ע��Ϣ
#define ASS_BET_RESULT					65				//�����ע�����Ϣ
#define ASS_SEL_AREA					66				//ׯ�Ҳ�ǩ��Ϣ
#define ASS_GM_STATUS_GAME				67			//��ȡ��Ϸ״̬��Ϣ
#define ASS_GAME_SHOWRESULT             68              //��ʾ�����
#define ASS_GAME_DATA                   69              //��Ϸ��������
#define ASS_SUPER_RESULT				70				//�����û����ƽ����Ϣ��


//��ע��������
static	const	int		G_iArePeiLv[3] = {1,1,8};

//��������ֵ
static	const	int		G_iChouMaValue[6] = {100,1000,10000,100000,1000000,10000000};

//������ׯ��Ϣ
struct	ApplyNtMsg
{
	BYTE	byDeskStation;		//������λ��
	ApplyNtMsg()
	{
		memset(this,-1,sizeof(ApplyNtMsg));
		
	}
};

//�����û����ƵĽ��
struct	SuperUserControlReslut
{
	BYTE	byDeskStation;		//�����û���λ��
	BYTE    bySuperReslut;		//�����û����ƵĽ��
	SuperUserControlReslut()
	{
		memset(this,-1,sizeof(SuperUserControlReslut));

	}
};

//ǿ�˽ṹ add by wlr 20090807
struct tagGameForceQuit
{
	BYTE				byDeskStation;					// �˳�λ��
	tagGameForceQuit()
	{
		byDeskStation = 0;
	}
};
//������ׯ��Ϣ
struct	ChaoguoMsg
{
	BYTE	byDeskStation;		//������λ��		
	ChaoguoMsg()
	{
		memset(this,-1,sizeof(ChaoguoMsg));
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

//-------------------------------------------------------------------------------
//��ע��Ϣ��
struct BeginBetMsg
{
	bool		bFlag;			//��ע��ʶ
	bool		bLuziClearFlag;			//·�������־
	BYTE		byNtStation;	//ׯ��λ��
	BYTE		byBetTime;		//��עʱ��
	BYTE		byGameStation;//��Ϸ״̬
	BYTE		byNtWaitList[PLAY_COUNT];//ׯ���б�
	BYTE		byEvent;//��Ϸ����
	BYTE		byNtcount;//ׯ����ׯ����
	__int64		i64NtScore;//ׯ�ҳɼ�

	BeginBetMsg()
	{
		memset(this,0,sizeof(BeginBetMsg));
		memset(byNtWaitList,255,sizeof(byNtWaitList));
		byNtStation=255;
		bLuziClearFlag=FALSE;
	}
};
//������Ϣ��
struct BeginSendCard
{
	bool bFlag;//���Ʊ�ʶ
	BYTE Bcard;//��������
	BYTE cardSort;//�����ڼ�����
	BYTE cardCount;//���Ƹ���
	BYTE byNtStation;//ׯ��λ��
	BYTE byPlayStation;//���λ��
	BeginSendCard()
	{
		memset(this,0,sizeof(BeginSendCard));
	}
};
//�������Ϣ��
struct BeginShowResultData
{
		BYTE      bDeskStation;
	  __int64     i64ChangeMoney;//��ҵ÷�
	  __int64     i64ReturnMoney;//�����Ľ����
	  __int64     i64PlayerGrade;//��ҳɼ�
	  BeginShowResultData()
	  {
		  memset(this,0,sizeof(BeginShowResultData));
	  }
};
//�������Ϣ��
struct BeginShowResult
{
	  BYTE        byGameStation;
	  BeginShowResult()
	  {
		  memset(this,0,sizeof(BeginShowResult));
		 
	  }
};
//��Ϸ��ʼ��Ϣ��
struct StartMsg
{
	BYTE    startTime;   //ׯ��λ�� 
	StartMsg()
	{
		memset(this,0,sizeof(StartMsg));
	}
};
//-------------------------------------------------------------------------------
//������Ϣ��
struct BeginSendCardMsg
{
	BYTE	byWinArea;//��������
	BYTE    byCardCount;//���Ƹ���
	BYTE    byCardStyle[52];//��������
	BYTE	byGameStation;
	BYTE	byNtWaitList[PLAY_COUNT];
	BYTE    byLuZi[72];//·��

	BeginSendCardMsg()
	{
		memset(this,0,sizeof(BeginSendCardMsg));
		memset(byNtWaitList,255,sizeof(byNtWaitList));
		memset(byLuZi,255,sizeof(byLuZi));
		byWinArea=255;
	}
};
//��ҽ�����Ϸ��
struct UserComePlayerMsg
{
	BYTE    byPlayStation;//���λ��
	UserComePlayerMsg()
	{
		byPlayStation=255;
	}
};
//��Ϸ�ȴ���ʼ��Ϣ��
struct GameWaitStartMsg
{
	bool flag;
	GameWaitStartMsg()
	{
		flag=false;
	}
};
//����״̬��Ϣ��
struct BeginKaiPaiStation
{
	bool	bFlag;			//���б�ʶ
	BYTE	byFreeTime;		//����ʱ��
	BYTE    byNtStation;//ׯ��λ��
	BYTE    byPlayStation;//���λ��
	BYTE    byRemaindTime;//ʣ��ʱ��
	BYTE    bGameStation;	//��Ϸ״̬
	BYTE	byWaitNtList[PLAY_COUNT];	//������ׯ�б�
	int     iBetSumLimite;	//��ע��������
	int     iApplyNtLimite;	//��ׯ����
	int     iNtPlayCount;		//ׯ����ׯ����
	BeginKaiPaiStation()
{
		memset(this,0,sizeof(BeginKaiPaiStation));
	}
};
//-------------------------------------------------------------------------------
//������Ϣ��
struct BeginFreeMsg
{
	bool	bFlag;			//���б�ʶ
	BYTE	byGameStation ;         ///��ǰ����Ϸ״̬
	BYTE	byFreeTime;		//����ʱ��
	BYTE    byNtStation;//ׯ��λ��
	BYTE    byPlayStation;//���λ��
	BYTE    byNtcount;//ׯ����ׯ����
	BYTE    byWaitNtList[PLAY_COUNT];	//��ׯ�б�
	__int64 i64NtScore;//ׯ�ҳɼ�
	BeginFreeMsg()
	{
		memset(this,0,sizeof(BeginFreeMsg));
		byNtStation = 255;
		memset(byWaitNtList, 255, sizeof(byWaitNtList));
	}
};
//׼���׶��˳��ṹ
struct GamePrepareStageQuit
{
	BYTE				byNtStation;	//ׯ��λ��
	BYTE				byNtWaitList[PLAY_COUNT];//ׯ���б�
	BYTE                byNtcount;//��ׯ����
	__int64             i64NtScore;//ׯ�ҳɼ�
	GamePrepareStageQuit()
	{
		memset(this,0,sizeof(GamePrepareStageQuit));
		memset(byNtWaitList,255,sizeof(byNtWaitList));
		byNtStation=255;
	}
};
//-------------------------------------------------------------------------------
//����״̬��Ϣ��
struct BeginFreeStation
{
	bool	bFlag;			//���б�ʶ
	BYTE	byFreeTime;		//����ʱ��
	BYTE    byNtStation;//ׯ��λ��
	BYTE    byPlayStation;//���λ��
	BYTE    byRemaindTime;//ʣ��ʱ��
	BYTE    bGameStation;	//��Ϸ״̬
	BYTE	byWaitNtList[PLAY_COUNT];	//������ׯ�б�
	int     iBetSumLimite;	//��ע��������
	int     iApplyNtLimite;	//��ׯ����
	int iNtPlayCount;//ׯ����ׯ����
	
	BeginFreeStation()
	{
		memset(this,0,sizeof(BeginFreeStation));
	}
};
//������ׯ�����Ϣ
struct	ApplyNtResultMsg
{
	BOOL    bNtFlag;			//��û��ׯ�ҵ��������ׯ�ı�־
	bool	bApplay;			//�Ƿ���Ϊ���û�дﵽ��ׯ��Ҷ���ׯ���ɹ�
	BYTE	byDeskStation;		//������λ��
	BYTE	byWaitNtList[PLAY_COUNT];	//������ׯ�б�
	BYTE	byNtStation;		//ׯ�ҵ�λ��
	BYTE    byNtcount;//ׯ����ׯ����
	BYTE    bGameStation;
	BYTE    bEvent;//��Ϸ����
	__int64 i64NtScore;//ׯ�ҳɼ�
	ApplyNtResultMsg()
	{
		memset(this,-1,sizeof(ApplyNtResultMsg));
		memset(byWaitNtList,255,sizeof(byWaitNtList));
		byNtStation=255;
		bApplay=false;
		bNtFlag=false;
	}
};
//������ׯ��Ϣ��

struct	ApplyLiveNtMsg
{
	BYTE	byDeskStation;		//������λ��
	ApplyLiveNtMsg()
	{
		memset(this,-1,sizeof(ApplyLiveNtMsg));
	}
};
//������ׯ�����Ϣ��

struct	ApplyLiveNtResultMsg
{
	
	BYTE    byNtStation;//ׯ��λ��
	BYTE	byDeskStation;		//������λ��
	BYTE    byWaitNtList[PLAY_COUNT];	//��ׯ�б�
	BYTE    byNtcount;//ׯ����ׯ����
	__int64 i64NtScore;//ׯ�ҳɼ�
	ApplyLiveNtResultMsg()
	{
		memset(this,-1,sizeof(ApplyLiveNtResultMsg));
		memset(byWaitNtList,255,sizeof(byWaitNtList));
		byNtStation=255;
		byDeskStation=255;
		byNtcount=0;
		i64NtScore=0;
	}
};
//��Ϸ������Ϣ
struct GameFreeMsg
{
	BYTE	byFreeTime;		//����ʱ��
	BYTE	byNtStation;	//ׯ��λ��
	BYTE	byWaitNtList[PLAY_COUNT];	//��ׯ�б�
	BYTE	byLuziList[10];		//·��
	BYTE	byCurrResult;		//���ֿ������
	BYTE	byLastResult;		//�Ͼֿ������
	int		iNtPlayCount;	//ׯ����ׯ����
	__int64	i64NtHaveWin;	//ׯ����Ӯ���
	__int64	i64MeHaveWin;	//�Լ�����Ӯ���

	GameFreeMsg()
	{
		memset(this,0,sizeof(GameFreeMsg));
	}	
};



//�����ע
struct	UserBetMsg
{
	BYTE	byDeskStation;		//��ע��λ��
	BYTE	byBetArea;			//��ע����
	BYTE	byChouMaType;		//��ע��������
	UserBetMsg()
	{
		memset(this,0,sizeof(UserBetMsg));
	}
};


//������Ϣ��
struct GameEndMsg
{
	BYTE		byLuzi[13];							//���10��·�����
	BYTE		byShowResultTime;					//������ʾʱ��
	BYTE  byWaitNtList[PLAY_COUNT];
	int			iNtPlayCount;						//ׯ����ׯ�Ĵ���
	__int64		i64NtHaveWin;						//ׯ����Ӯ���
	__int64		i64MeHaveWin;						//�Լ�������Ӯ���
	__int64		i64TurePoint[PLAY_COUNT];			//��ҵ÷�
	__int64		i64ChangeMoney[PLAY_COUNT];			//�����Ӯ�Ľ��
	__int64		i64ReturnMoney[PLAY_COUNT];			//�����Ľ����
	 

	GameEndMsg()
	{
		memset(this,0, sizeof(GameEndMsg));
	}
};
struct GameAccountMsg
{
	BYTE        winArea;     //�н�����
	BYTE        byNtStation;//ׯ��λ��
	BYTE        byPlayStation;//���λ��
	BYTE        byWaitNtList[PLAY_COUNT];//��ׯ�б�
	__int64     i64PlayScore;//�мҵ÷�
	__int64     i64PlayReturnMoney;//�мҷ��������
	__int64     i64NtScore;//ׯ�ҵ÷�
	__int64     i64NtReturnMoney;//ׯ�Ҽҷ��������
	GameAccountMsg()
	{
		memset(this,0, sizeof(GameAccountMsg));
		byNtStation=255;
		byPlayStation=255;
		memset(byWaitNtList,255, sizeof(byWaitNtList));
	}
};
struct GameBetMsg
{
	BYTE	byFreeTime;		//����ʱ��
	BYTE    byNtStation;//ׯ��λ��
	BYTE    byPlayStation;//���λ��
	BYTE    byRemaindTime;//ʣ��ʱ��

	BYTE    bGameStation;	//��Ϸ״̬
	BYTE	byWaitNtList[PLAY_COUNT];	//������ׯ�б�
	int     iBetSumLimite;	//��ע��������
	int     iApplyNtLimite;	//��ׯ����
	int     iNtPlayCount;		//ׯ����ׯ����
	GameBetMsg()
	{
		memset(this,0, sizeof(GameBetMsg));
	}
};

//-------------------------------------------------------------------------------
//��Ϸ״̬�������ݰ�
struct	GameStation_Base
{
	//��Ϸ�汾
	BYTE		iVersion;						//��Ϸ�汾��
	BYTE		iVersion2;						//��Ϸ�汾��

	BYTE		bGameStation;		           //��Ϸ״̬

	BYTE		byFreeTime;					   //����ʱ��
	BYTE		byBetTime;			           //��עʱ��
	BYTE		bySendTime;	                  //����ʱ��

	BYTE		bySendCardTime;	        //����ʱ��
	BYTE		byAccountTime;	        //����ʱ��
	BYTE        byShowResult;	        //չʾ���ʱ��
	BYTE		bySendCardSpaceTime;	//���Ƽ��ʱ��
	BYTE		bySpeedTime;		   //�ڵ����ƶ��ٶ�
	BYTE        byShowWinTime;//��ʾ��Ӯ
	BYTE		byRemaindTime;		   //ʣ��ʱ��

	BYTE		byNtStation;		//ׯ��λ��

	
	BYTE		byLuziList[72];		//·��
	BYTE		byWaitNtList[PLAY_COUNT];	//��ׯ�б�
	BYTE		byMaxNtPeople;		//�����ׯ����
	BYTE        byEvent;//��Ϸ����
	int			iNtPlayCount;		      //ׯ����ׯ����
	int			iApplyNtLimite;		     //��ׯ����
	__int64		i64NtGrade;		        //ׯ�ҳɼ�
	__int64     i64PlayerGrade;			//�e�ҳɿ�
	
	GameStation_Base()
	{
		memset(this,0, sizeof(GameStation_Base));
	}
};


//-------------------------------------------------------------------------------
//��Ϸ��ע״̬���ݰ�
struct	GameStation_Bet:public GameStation_Base
{
	BYTE		byCurrResult;	//ׯ�Ҳ�ǩ�Ľ��
	__int64		i64UserBetCount[PLAY_COUNT][BET_ARES];	//�������ÿ���������ע���
};

//-------------------------------------------------------------------------------
//��Ϸ��ǩ״̬���ݰ�
struct	GameStation_OpenResult:public GameStation_Base
{
	BYTE		byCurrResult;		//��ǰ�������
	__int64		i64UserBetCount[PLAY_COUNT][BET_ARES];	//�������ÿ���������ע���
};
//-------------------------------------------------------------------------------
//��Ϸ��ǩ״̬���ݰ�
struct	GameStation_ShowResult:public GameStation_Base
{
	__int64		i64UserBetCount[PLAY_COUNT][BET_ARES];	//�������ÿ���������ע���
	__int64		i64UserChangeMoney[PLAY_COUNT];			//�����Ӯ���
	__int64		i64ReturnMoney[PLAY_COUNT];				//�����Ľ����
};

//-------------------------------------------------------------------------------
//��ע���
struct	UserBetResultMsg
{
	BYTE	    byDeskStation;		//��ע��λ��
	BYTE		byBetArea;			//��ע����
	BYTE		byChouMaType;		//��ע��������
	BYTE	    byBetNumFlag;		//��ע100W��500W����500W�ı�־
	__int64     i64UserBetCount[BET_ARES]; //�м���ÿ���������ע���
	__int64		i64AreaBetAllSum[BET_ARES];		//�����������ע
	__int64     i64SumAllBet;
	UserBetResultMsg()
	{
		memset(this,0,sizeof(UserBetResultMsg));
	}
};

//�l�������Ɣ���
struct SendAllStruct
{
	BYTE             iUserCardCount;		//��������
	BYTE             iUserCardList[108];				//���ƶ���
};


///��������
enum BackCardType
{
	TYPE_NONE        = 0 ,//ʲô���Ͷ�����
	TYPE_DOUBLE_CARD = 10 , 
	TYPE_SAME_HUA    = 11,
	TYPE_SMALL_KING  = 12 ,
	TYPE_BIG_KING    = 13 , //����
	TYPE_TRIPLE_CARD = 14 ,
	TYPE_STRAIT      = 15,
	TYPE_ROCKET      = 16
};
///��������
enum LastTaskType
{
	TYPE_LAST_NONE         = 0 ,    ///���κ����� 
	TYPE_HAVE_A_CARD       = 100 ,  ///��ĳ����
	TYPE_SOME_SHAPE        = 101 ,  ///��ĳ������
	TYPE_SINGLE_SOME_CARD  = 102 ,  ///���ĳ����
	TYPE_DOUBLE_SOME_CARD  = 103   ///���һ��ĳ����
};

///��Ϸ����ṹ
struct GameTaskStruct
{
	BYTE     byBackCardType ; 
	BYTE     byTaskType ; 
	BYTE     bySpecifyShape ; 
	BYTE     bySpecifyCard ;
	BYTE     byBackCardMutiple ;  ///���Ʊ���

	GameTaskStruct()
	{
		memset(this , 0 , sizeof(GameTaskStruct)) ; 
	}
	void IniData(void)
	{
		memset(this , 0 ,sizeof(GameTaskStruct)) ;
	}

};
///��Ϸ�еı���
struct  GameMutipleStruct
{
	int       sBaseMutiple ;                 ///��Ϸ�еĻ�����ע
	int       sBackCardMutiple ;             ///���Ʊ���
	int       sBombCount ;                   ///ը������
	int       sSprintMutiple ;               ///����
	int       sCardShapeMutiple  ;           ///���ͱ�����һ��ָ�����еı�����

	BYTE      sAddGameMutiple[PLAY_COUNT] ;  /// ��Ϸ�мӱ�
	BYTE      sRobNtMutiple[PLAY_COUNT] ;    ///����������
	BYTE      sMingPaiMutiple[PLAY_COUNT] ;  ///ÿ���˵����Ʊ���

	GameMutipleStruct()
	{
		sBaseMutiple = 1 ; 
		sBackCardMutiple = 1 ; 
		sBombCount = 0 ; 
		sSprintMutiple = 1;
		sCardShapeMutiple = 100 ; 

		memset(sAddGameMutiple , 0 , sizeof(sAddGameMutiple)) ; 
		memset(sRobNtMutiple , 0 , sizeof(sRobNtMutiple)) ; 
		memset(sMingPaiMutiple , 0 , sizeof(sMingPaiMutiple)) ; 
	};
	void  IniData(int  iBaseMutiple)
	{
		sBaseMutiple = iBaseMutiple ; 
		sBombCount = 0 ; 
		sBackCardMutiple = 1 ; 
		sSprintMutiple = 1;
		sCardShapeMutiple = 100; 

		memset(sAddGameMutiple , 0 , sizeof(sAddGameMutiple)) ; 
		memset(sRobNtMutiple , 0 , sizeof(sRobNtMutiple)) ; 
		memset(sMingPaiMutiple , 0 , sizeof(sMingPaiMutiple)) ; 
	}
	///��ȡ���������
	int  GetMingMaxMutiple(void)
	{
		int iMingMutiple = max(max(sMingPaiMutiple[0] , sMingPaiMutiple[1]) ,sMingPaiMutiple[2]) ; 

		return (iMingMutiple >0?iMingMutiple :1) ; 
	}
	///��ȡ������������
	int  GetRobNtMutiple(void)
	{
		int  iRobMutiple = 0 ; 
		int  iRobCount   = 0 ; 

		for(int  i = 0  ; i <PLAY_COUNT ; i++)
		{
			iRobCount += sRobNtMutiple[i] ; 
		}

		iRobMutiple = pow(2.0, iRobCount) ; 

		return iRobMutiple ; 
	}
    ///��ȡ��������
    int  GetPublicMutiple(void)
	{
		int  iBombMutiple = pow(2.0 , sBombCount) ; 

		int  iGameMutiple  = sBaseMutiple*sBackCardMutiple*iBombMutiple*sSprintMutiple*sCardShapeMutiple*GetRobNtMutiple()*GetMingMaxMutiple()/100 ;
			
		return iGameMutiple ; 
		
	}
};


/********************************************************************************/
//��Ϸ���ݰ�

/********************************************************************************/
//�û��������ݰ� �������������
struct OutCardStruct
{
	int					iCardCount;						//�˿���Ŀ
	BYTE				iCardList[45];					//�˿���Ϣ
};

//������ҳ���
struct ReplaceOutCardStruct
{
	BYTE				bDeskStation;					//������Ƶ�λ��
	int					iCardCount;						//�˿���Ŀ
	BYTE				iCardList[45];					//�˿���Ϣ
};

//�û��������ݰ� ������ͻ��ˣ�
struct OutCardMsg
{
	int					iNextDeskStation;				//��һ������
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������	
	BYTE				iCardList[45];					//�˿���Ϣ
};
//��һ��
struct NewTurnStruct
{
	BYTE				bDeskStation;					//����
	BYTE				bReserve;						//����
};
//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
struct GameStation_1
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	bool                bUserReady[PLAY_COUNT] ;        ///����Ƿ���׼��

	BYTE				iBeginTime;						//��ʼ׼��ʱ��
	BYTE				iThinkTime;						//����˼��ʱ��
	BYTE				iCallScoreTime;					//�зּ�ʱ
	BYTE                iRobNTTime ;                    //������ʱ��
	BYTE				iAddDoubleTime;					//�Ӱ�ʱ��

	int                 iGameMutiple   ;             ///��Ϸ�еı���
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

	DWORD				iCardShape;						//��������
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	__int64				iRunPublish;					//���ܿ۷�
	
};


//��Ϸ״̬���ݰ�	�� �ȴ���Ѻ����״̬ ��
struct GameStation_3
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	BYTE				iBackCount;						//������
	BYTE				iBeginTime;						//��ʼ׼��ʱ��
	BYTE				iThinkTime;						//����˼��ʱ��
	BYTE                iRobNTTime ;                    //������ʱ��
	BYTE				iCallScoreTime;					//�зּ�ʱ
	BYTE				iAddDoubleTime;					//�Ӱ�ʱ��

	BYTE				iCallScorePeople;				//��ǰ�з���
	BYTE				iGameFlag;						//�зֱ��
	BYTE				iCallScoreResult;				//���еķ�
	
	int					iUpGradePeople;					//ׯ��λ��
	int                 iCurOperator ;                 ///��ǰ��������

	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��

	DWORD				iCardShape;						//��������
	int                 iGameMutiple   ;             ///��Ϸ�еı���
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

	bool				bAuto[PLAY_COUNT];				//�й����
	bool				bCanleave[PLAY_COUNT];			//�ܷ���˳�
	BYTE				iCallScore[PLAY_COUNT];			//���ҽз����
	int 				iRobNT[PLAY_COUNT];				//���������
	int                 iUserDoubleValue[PLAY_COUNT] ;  ///��Ҽӱ����
	BYTE				iUserCardCount[PLAY_COUNT];		//�û������˿���Ŀ
	BYTE				iUserCardList[195];				//�û����ϵ��˿�
	BYTE                iGameBackCard[12] ;                 ///����
	BYTE 				iBackCardCount;						//��������
	__int64				iRunPublish;					//���ܿ۷�

	GameMutipleStruct   gameMutiple ;                      ///��Ϸ����  
	GameTaskStruct      gameTask ;                         ///��Ϸ����


};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4	
{
	bool				bIsLastCard;						//�Ƿ�����������
	
	BYTE				iVersion;							//��Ϸ�汾��
	BYTE				iVersion2;							//��Ϸ�汾��
	BYTE				iBackCount;							//������

	BYTE				iBeginTime;							//��ʼ׼��ʱ��
	BYTE				iThinkTime;							//����˼��ʱ��
	BYTE                iRobNTTime ;                    //������ʱ��
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iAddDoubleTime;						//�Ӱ�ʱ��

	BYTE				bIsPass;							//�Ƿ񲻳�

	int					iRunPublish;						//���ܿ۷�	
	int					iBase;								//��ǰը������
	int					iUpGradePeople;						//ׯ��λ��
	
	int 				iCallScoreResult;					//�зֽ��
	int					iOutCardPeople;						//���ڳ����û�
	int					iFirstOutPeople;					//�ȳ��Ƶ��û�
	int					iBigOutPeople;						//�ȳ��Ƶ��û�

	UINT				iDeskBasePoint;						//���������
	UINT				iRoomBasePoint;						//���䱶��

	int                 iGameMutiple   ;             ///��Ϸ�еı���
	DWORD				iCardShape;							//��������
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

	BYTE				iAwardPoint[PLAY_COUNT];			//����
	BYTE				iPeopleBase[PLAY_COUNT];			//�Ӱ�
	BYTE				iRobNT[PLAY_COUNT];					//������


	bool				bAuto[PLAY_COUNT];					//�й����
	bool				bCanleave[PLAY_COUNT];				//�ܷ���˳�

	BYTE				iUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
	BYTE				iUserCardList[195];					//�û����ϵ��˿�

	BYTE				iBaseOutCount;						//���Ƶ���Ŀ
	BYTE                iBaseCardList[45] ;                //�����ϵ���

	BYTE				iDeskCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
	BYTE                iDeskCardList[PLAY_COUNT][45] ;    ///�����ϵ���

	BYTE				iLastCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
	BYTE				iLastOutCard[PLAY_COUNT][45];		//���ֵ��˿�

	bool                bPass[PLAY_COUNT] ;                //����
	bool                bLastTurnPass[PLAY_COUNT] ;         //��һ�ֲ���

	BYTE                iGameBackCard[12] ;                 ///����
	BYTE 				iBackCardCount;						//��������

	GameMutipleStruct   gameMutiple ;                      ///��Ϸ����  
	GameTaskStruct      gameTask ;                         ///��Ϸ����

};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5
{
	BYTE				iVersion;							//��Ϸ�汾��
	BYTE				iVersion2;							//��Ϸ�汾��
	BYTE				iBeginTime;							//��ʼ׼��ʱ��
	BYTE				iThinkTime;							//����˼��ʱ��
	BYTE                iRobNTTime ;                    //������ʱ��
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iAddDoubleTime;						//�Ӱ�ʱ��

	int                 iGameMutiple   ;               ///��Ϸ�еı���
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

	bool                bUserReady[PLAY_COUNT] ;     ///���׼��
	DWORD				iCardShape;							//��������

	UINT				iDeskBasePoint;						//���������
	UINT				iRoomBasePoint;						//���䱶��
	__int64				iRunPublish;						//���ܿ۷�
};

//�û�ͬ����Ϸ
struct UserArgeeGame
{
	BYTE				iPlayCount;							//��Ϸ����
	BYTE				iCardCount;							//�˿���Ŀ
};
///���ƿ�ʼ
struct  UserMingStruct
{
	bool    bStart  ;       ///�Ƿ�Ϊ��ʼ
	bool    bMing ;         ///����Ƿ�����
	BYTE    bDeskStaion ;   ///��ҵ�λ��
	BYTE    byCardCount ;   ///����ʱ����˿˵�����
};

//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	BYTE				iPlayLimit;							//��Ϸ�ܾ���
	BYTE				iBeenPlayGame;						//�Ѿ����˶��پ�
	BYTE                bUserMing[PLAY_COUNT] ;            ///����������
};
//��Ϸ׼�����ݰ�
struct PrepareStruct
{
	BYTE				bCard;									//����
};
//�������ݰ�
struct SendCardStruct
{
	BYTE				bDeskStation;							//���
	BYTE				bCard;									//�Ʊ��
};


//���ƽ���
struct SendCardFinishStruct
{
	BYTE		bReserve;									
};

//�������ݰ�
struct BackCardExStruct
{
	BYTE				iGiveBackPeople;				//�������
	BYTE				iBackCardCount;					//�˿���Ŀ
	BYTE				iBackCard[12];					//��������

	GameTaskStruct      gameTask ;                      //��Ϸ����  

};
//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;				//���Ƶ�λ��
};


//�û�˦��ʧ�����ݰ� ������ͻ��ˣ�
struct OutShowCardStruct
{
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������
	BYTE				iResultCount;					//����˿���Ŀ
	BYTE				iCardList[78];					//�˿���Ϣ
};

//���濪ʼ
struct ReplaceAgreeStruct
{
	BYTE bDeskStation;									
};

//�з����ݰ�
struct CallScoreStruct
{	
	BYTE				bDeskStation;							//��ǰ�з���
	int					iValue;									//�з�����
	bool 				bCallScoreflag;							//�зֱ��								
};
//������
struct RobNTStruct
{
	BYTE   bDeskStation;										//����������
	BYTE   byRobCount ;                                         //�������������
	int    iValue;												//���������
};

//����
struct ShowCardStruct
{
	BYTE bDeskStation;										//����
	BYTE iCardList[54];					                   //�˿���Ϣ
	BYTE iCardCount;						              //�˿���Ŀ
	int iValue;											  //����
	int iBase;											  //���� 20081204
};

//��������
struct ReplaceShowCardStruct
{
	BYTE bDeskStation;										//����
	BYTE bRepDeskStation;									//����λ��
	int iValue;												//����
};
//�Ӱ�
struct AddDoubleStruct
{
	BYTE bDeskStation;										//�Ӱ�λ��
	int iValue;												//�Ӱ����
};

//����з����ݰ�
struct ReplaceCallScoreStruct
{	
	BYTE		bDeskStation;								//����з���
	int		    NowCallScorePeople;							//��ǰ�з���
	int			CallScore;									//�з�����
	bool 		CallScoreFlag;								//�зֱ��								
};

//����
struct AwardPointStruct
{
	BYTE	iAwardPoint;									//����
	BYTE	bDeskStation;									//����
	int		iBase;											//����
};

//�й����ݽṹ
struct AutoStruct
{
	BYTE bDeskStation;
	bool bAuto;
};

//�������й�
struct UseAIStation
{
	BYTE bDeskStation;					//����
	BOOL bState;						//״̬
};

//�ھ����
struct ChampionStruct
{
	BYTE bDeskStation ;
};

//�û��������ݰ� ������ͻ��ˣ�
struct BatchOutCardMsg
{
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				iCardList[156];					//�˿���Ϣ
};

////����
//struct PropStruct
//{
//	BYTE				sendDeskStation;				//����λ��
//	BYTE				recvDeskStation;				//����λ��
//	int					iValue;
//};
//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	bool                bFinishTask ;                    //�Ƿ����������
	BYTE				iUpGradeStation;				//ׯ��λ��
	BYTE			    iUserCardCount[PLAY_COUNT];	    //�û������˿���Ŀ
	BYTE			    iUserCard[PLAY_COUNT][45];		//�û����ϵ��˿�

	GameMutipleStruct   gameMutiple ; 

    __int64  			iTurePoint[PLAY_COUNT];			//��ҵ÷�
	__int64				iChangeMoney[8];				//��ҽ��

	GameEndStruct()
	{
		memset(this , 0 ,sizeof(GameEndStruct)) ; 
	}

};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					iRoomBasePoint;					//����
	int					iDeskBasePoint;					//���汶��
	int					iHumanBasePoint;				//��ͷ����

	int					bDeskStation;					//�˳�λ��
	__int64				iChangeMoney[8];				//��ҽ��
	__int64				iTurePoint[PLAY_COUNT];			//ׯ�ҵ÷�

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

//��Ϸ����
struct GameFinishNotify
{
	TCHAR name[PLAY_COUNT][61];
	//	int	iBasePoint;
	BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
	int	iBasePoint;			//ϵͳ����
	int	iDeskBasePoint;		//���������
	int iUpGradePeople;		//�f��
	int iGrade2;			//�f������
	__int64	iWardPoint[PLAY_COUNT];
	__int64 iAwardPoint[PLAY_COUNT];	//����
	__int64 iTotalScore[PLAY_COUNT];	//�ܻ���
	__int64 iMoney[PLAY_COUNT];			//��������Ǯ��
	BYTE iGameStyle;					//��Ϸ����
};
struct UserleftDesk
{
	BYTE bDeskStation;                //�������
};
/********************************************************************************/

// ���ݰ���������ʶ
//warning:79��IDԤ���������ͻ���,����ʹ��
/********************************************************************************/
#define ASS_UG_USER_SET					50				//������Ϸ
#define ASS_GAME_BEGIN					51				//��Ϸ��ʼ
#define ASS_GAME_PREPARE				52				//׼������

#define ASS_SEND_CARD					53				//������Ϣ
#define ASS_SEND_CARD_MSG				54				//���ƹ����д�����Ϣ
#define ASS_SEND_ALL_CARD				55				//����������(һ�·���ȫ��)
#define ASS_SEND_FINISH					56				//�������
#define ASS_CALL_SCORE					57				//�з�
#define ASS_CALL_SCORE_RESULT			58				//�зֽ��
#define ASS_CALL_SCORE_FINISH			59				//�зֽ���
#define ASS_REPLACE_CALL_SCORE			60				//����з�
#define ASS_XIAZHU				61				//��ע��Ϣ

#define ASS_ROB_NT						61				//������
#define ASS_ROB_NT_RESULT				62				//���������
#define ASS_REPLACE_ROB_NT				63				//����������
#define ASS_GAME_MULTIPLE				64				//��Ϸ����(���������ӱ�)
#define ASS_ROB_NT_FINISH				65				//���������

#define ASS_BACK_CARD					66				//��������
#define ASS_BACK_CARD_EX				67				//��չ��������(������ҿɼ�)
#define ASS_ADD_DOUBLE					68				//�Ӱ�
#define ASS_ADD_DOUBLE_RESULT			69				//�Ӱ����
#define ASS_REPLACE_ADD_DOUBLE			70				//����Ӱ�
#define ASS_ADD_DOUBLE_FINISH			71				//�Ӱ�����
#define ASS_SHOW_CARD					72				//����
#define ASS_SHOW_CARD_RESULT			73				//���ƽ��
#define ASS_REPLACE_SHOW_CARD			74				//��������
#define ASS_SHOW_CARD_FINISH			75				//���ƽ���

#define ASS_GAME_PLAY					76				//��ʼ��Ϸ
#define ASS_OUT_CARD					77				//�û�����
#define ASS_OUT_CARD_RESULT				78				//���ƽY��
#define ASS_REPLACE_OUT_CARD			80				//�������(79���������ͻ��˷�����)
#define ASS_ONE_TURN_OVER				81				//һ�����(ʹ�ͻ�����һ�ֿ���)
#define ASS_NEW_TURN					82				//��һ�ֿ�ʼ
#define ASS_AWARD_POINT					83				//����(ը�����)

#define ASS_CONTINUE_END				84				//��Ϸ����
#define ASS_NO_CONTINUE_END				85				//��Ϸ����
#define ASS_NO_CALL_SCORE_END			86				//���˽з�
#define ASS_CUT_END						87				//�û�ǿ���뿪
#define ASS_SAFE_END					88				//��Ϸ��ȫ����
#define ASS_TERMINATE_END				89				//�������
#define ASS_AHEAD_END					90				//��ǰ����
#define ASS_AUTO						91				//�û��й�
#define ASS_HAVE_THING					92				//����
#define ASS_LEFT_RESULT					93				//�����뿪���
#define ASS_MESSAGE						94				//������Ϣ
#define ASS_AI_STATION					95				//�������й�(���߻���)
#define ASS_REPLACE_GM_AGREE_GAME		96				//������ҿ�ʼ
#define ASS_USER_LEFTDESK               97               //����뿪���ӻ����
#define ASS_PREPARE_END					98				//��Ϸ׼���׶��û��˳�
/********************************************************************************/
#endif
