// GameMsgPkg.h: game message package and define const.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_UPGRADEMESSAGE_H__INCLUDED_)
#define _UPGRADEMESSAGE_H__INCLUDED_

#pragma pack(push,1)

#define CHECK_DEBUG   1

// ��Ϸ������Ϣ
#define GAMENAME						TEXT("�ټ���")                          // ��Ϸ����
#define GAME_DLL_AUTHOR                 TEXT("���ں�������Ƽ�")            //
#define GAME_DLL_NOTE                   TEXT("���� -- ��Ϸ���")

#ifdef VIDEO
#define NAME_ID							11100200								 // ���� ID
#else
#define NAME_ID							11100200								 // ���� ID


#endif
#define MAX_NAME_INFO                   256
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)

#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // �ͻ���Ŀ¼
#define SKIN_CARD						TEXT("CardSkin")		                 // ����ͼƬ�ļ���

// �汾����
#define GAME_MAX_VER					1								        // ������߰汾
#define GAME_LESS_VER					1								        // ������Ͱ汾
#define GAME_CHANGE_VER					0								        // �޸İ汾

// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

#define PLAY_COUNT						180							        // ��Ϸ����

//��Ϸ�ڲ������汾��
#define DEV_HIGH_VERSION				2				                        // �ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0				                        // �ڲ������Ͱ汾��

// ��Ϸ״̬����
#define GS_WAIT_SETGAME					0				// �ȴ���������״̬
#define GS_WAIT_AGREE					1				// �ȴ�ͬ������
#define GS_SEND_CARD					20				// ����״̬
//#define GS_PLAY_GAME					21				// ��Ϸ��״̬
#define GS_PLAY_GAME					22				// ����״̬
#define GS_WAIT_NEXT					23				// �ȴ���һ�̿�ʼ 

#define NO_WAITING                      0               // �����ڵȴ�״̬
#define WAITING_USER_BEGIN              1               // �ȴ�������ҿ�ʼ
 
#define ID_VIEW_CLOCKWISE               1               // ˳ʱ��
#define ID_VIEW_ANTICLOCKWISE           0               // ��ʱ��

#define USER_HAND_CARD_COUNT            27              // �û����е���
#define HAVE_THING_LENGHT               61              // ����Ҫ�ߵ���Ϣ����
#define USER_AGREE_LEAVE                1               // ͬ������뿪
#define USER_DISAGREE_LEAVE             0               // ��ͬ������뿪

#define ONE_SECOND                      1000            // 1 ��ʱ��

//72��һ��
#ifdef SET72 
//lym2010-03-17ÿ�ֵ�������
#define MAXCOUNT						72             
#else
//lym2010-03-17ÿ�ֵ�������
#define MAXCOUNT						30            
#endif
/// ��Ϸ������Ϣ
typedef struct tagGameBaseInfo
{
	//��Ϸ�汾
	BYTE byHighVersion;		// ��Ϸ�汾��
	BYTE byLowVersion;		// ��Ϸ�汾��
	BYTE bySendCardOrder;    // ����˳��
	int iSendCardInterval;   // ����ʱ����(ms)	
	int iThinkTime;          // ����˼��ʱ��(s)
	int iAlarmTime;          // �澯ʱ��(s)
	int iBeginTime;          // ��ʼʱ��(s)
	int iSendCardTime;		 // ����ʱ��(ms)
	int iSendCardZhen;		 // ����֡��
	tagGameBaseInfo()
	{
		memset(this,0, sizeof(tagGameBaseInfo));
		byHighVersion = DEV_HIGH_VERSION;
		byLowVersion = DEV_LOW_VERSION;
	}
}GAME_BASE_INFO;

// ֪ͨ������ͼ
typedef struct tagUpdateClientView
{	
	BYTE abySeatID[PLAY_COUNT];
}UPDATE_CLIENT_VIEW;

// ��Ϸ��ʼʱ�ķ������ݰ�
typedef struct tagBeginSendCard
{	
	WORD wSentCount;                  // �ѷ�������
	WORD wMingCard;                   // ����	
	WORD wUserCard[USER_HAND_CARD_COUNT]; // ����Լ�����
	BYTE byUserCardCount[PLAY_COUNT]; // ��������е�������
	BYTE byFirstSendPos;              // ��һλ�������
	tagBeginSendCard()
	{
		memset(this, 0, sizeof(tagBeginSendCard));
	}
}BEGIN_SEND_CARD;

// ���ƺ���Ϸ��ʼ���ݰ�
typedef struct tagGameBegin
{
	BYTE byOutCardSeatID;       // �������	
	BYTE abySeatID[PLAY_COUNT];  //(����е�������Ϊ�������λ��)
	tagGameBegin()
	{
		memset(this, 0, sizeof(tagGameBegin));
	}
}GAME_BEGIN;


// ��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
typedef struct tagGameStationWaiting
{
	// ��Ϸ�Ļ�����Ϣ
	GAME_BASE_INFO  sBaseInfo;
}GAME_STATION_WAITING;

// ��Ϸ״̬���ݰ�	�� ����״̬ ��
typedef struct tagGameStationSendCard
{
	// ��Ϸ�Ļ�����Ϣ
	GAME_BASE_INFO  sBaseInfo;
	// �������ݰ�
	BEGIN_SEND_CARD sSendCard;
}GAME_STATION_SEND_CARD;

// ��Ϸ״̬���ݰ�	�� ��Ϸ�� ��
typedef struct tagGameStationPlaying
{
	// ��Ϸ�Ļ�����Ϣ
	GAME_BASE_INFO  sBaseInfo;

	// �������ݰ�
	BEGIN_SEND_CARD sSendCard;

	// ���λ��
	UPDATE_CLIENT_VIEW sUpdateClientView;

	// ������Ϸ��Ϣ ����Ϸ���������

}GAME_STATION_PLAYING;



// �й���Ϣ
typedef struct tagTrusteeCard
{
	BYTE byDeskStation;            // ��λ��
	bool bIsTrustee;               // �Ƿ�Ϊ�й�״̬
	tagTrusteeCard()
	{
		memset(this, 0, sizeof(tagTrusteeCard));
	}
}TRUSTEE_CARD;

// ��Ϸǿ�ˡ����߽���ͳ�����ݰ�
typedef struct tagGameCutFinish
{
	BYTE				byDeskStation;					// �˳�λ��
	int					iTurePoint[PLAY_COUNT];			// ����ҵ÷�
	int					iChangeMoney[PLAY_COUNT];		// �������
	int					iAwardPoint[PLAY_COUNT];		// ������
	tagGameCutFinish()
	{
		memset(this, 0, sizeof(tagGameCutFinish));
	}
}GAME_CUT_FINISH;

//ǿ�˽ṹ add by wlr 20090807
typedef struct tagGameForceQuit
{
	BYTE				byDeskStation;					// �˳�λ��
	tagGameForceQuit()
	{
		byDeskStation = 0;
	}
}GAME_FORCE_QUIT;
//end of add by wlr 20090807

typedef struct tagHaveThing
{
	BYTE byDeskStation;                      // ����Ҫ�ߵ���λ
	char szMessage[HAVE_THING_LENGHT];       // ��Ϣ����
	tagHaveThing()
	{
		memset(this, 0, sizeof(tagHaveThing));
	}
}S_HAVE_THING;

typedef struct tagAgreeUserLeave
{
	BYTE byDeskStation;                      // �𸴵���λ
	BYTE byAgreeLeave;                       // 1 ͬ��,������ͬ��
	tagAgreeUserLeave()
	{
		memset(this, 0, sizeof(tagAgreeUserLeave));
	}
}S_AGREE_USER_LEAVE;

typedef struct tagWatchSetChange
{
	BYTE byDeskStation;
	BYTE byCardCount;
	WORD wCardList[USER_HAND_CARD_COUNT];
	tagWatchSetChange()
	{
		memset(this, 0, sizeof(tagWatchSetChange));
	}
}S_WATCH_SET_CHANGE;

///lym0511�ټ�����Ϸ��ʼ��Ϣ
typedef struct tagBeginData
{
	BYTE  m_iResultInfo[MAXCOUNT][4];//���MAXCOUNT�ѵ���Ϣ	
	short int  m_iNTlist[PLAY_COUNT+1]; 
	BOOL bHaveUserInList;      //�Ƿ��зǻ�������ׯ���б���
	int  m_iShangZhuangLimit; //��ׯ��Ҫ�����ٽ��
	int  m_iBaSHu;            //���֣�30��һ�֣����еİ���
	int  m_iZhuangBaShu;      //ׯ�ҽ����˼���
	int  m_iNowNtStation;     //��ǰׯ�ҵ�λ��
	int  m_iGameCount;        //��ǰ�Ѿ������˼���
	int  m_iXiaZhuTime ;		//��עʱ��
	int  m_iKaiPaiTime ;		//����ʱ��
	int  m_iFreeTime ;			//����ʱ��
	int  m_iNowJuShu;			//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
	__int64 	 iMaxNote;			//������ע�ⶥ
	__int64      m_iNTdata[4]; //ׯ�ҵ�λ��,�ܷ֣���ǰ����������ɼ�(Ӯ���ܽ��)����������ׯ�ľ�����
	__int64	     m_iMaxZhu[8];        //ÿ���������µ����ע	
	tagBeginData()
	{
		memset(this, 0, sizeof(tagBeginData));	
		m_iNowNtStation=-1;
	}
}BEGIN_DATA;
///lym0512
///��ע���ݰ�
typedef struct tagXiaZhu
{
	bool m_bIsAI;//�Ƿ��������ע	
	int station;//λ��
	__int64 money;//���
	int type;//��ע����0-7
	int moneytype;//�������ͣ�1���٣�2��ǧ��3����4��ʮ��5������6�������
	__int64 m_iMaxZhu[8];//ÿ���������¶���ע
	__int64 m_iMeZhongZhu;      //���ѵ�ǰ��ע��
	__int64 m_iQuYuZhu[8];    //����ÿ�������µ�ע��
	__int64 m_iAIMoney[8];		///�����˵���ע	
	__int64 m_iUserXiaZhu[8];   //��ҵ�ǰ��ÿ�������µ�ע��
	int  m_nPowerOfGold;//�Ŵ���
	CPoint point;
	tagXiaZhu()
	{
		memset(this, 0, sizeof(tagXiaZhu));
	}
}XIA_ZHU;
///lym0512
///�������ݰ�
typedef struct tagKaiPai
{

	WORD    pai[2][3];        //ׯ�е��ƣ�0Ϊׯ��1Ϊ��
	BYTE    m_CardCount[2];//˫����������  
	BYTE    m_iResultInfo[MAXCOUNT][4]; //����Ϣ

	short   int  zhuangstation[PLAY_COUNT+1]; 	

	int  	 m_iHistoryPlayData[MAXCOUNT]; //������ʷ��ׯ����Ϣ	
	int  	 m_iWinner;			//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	int  	 m_iKaiPai;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���	
	int  	 m_iZPaiXing[5];   //ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	int      m_iXPaiXing[5];   //�м�����
	int      m_iWinQuYu[8];			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���	

	__int64  m_iZhuangFen;     //ׯ�ҵĵ÷�
	__int64  m_iXianFen;	   //�мҵĵ÷�
	__int64  m_iUserFen;		//��ǰ��ҵĵ÷�
	
	tagKaiPai()
	{
		memset(this, 0, sizeof(tagKaiPai));
		m_CardCount[0]=255;
		m_CardCount[1]=255;
		memset(zhuangstation, -1, sizeof(zhuangstation));		
	}
}KAI_PAI;
///�������ݰ�lym1204
typedef struct tagJieSuan
{
	BYTE  m_iResultInfo[MAXCOUNT][4]; //����Ϣ

	__int64  m_iZhuangFen;		//ׯ�ҵĵ÷�
	__int64  m_iXianFen;		//�мҵĵ÷�

	__int64  m_iUserFen;		//��ǰ��ҵĵ÷� (�ɼ�)
	__int64  m_iNtWin;          //��ǰׯ��Ӯ�Ľ�ң��ɼ���


	int  m_iZPaiXing[5];	//ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	int  m_iXPaiXing[5];	//�м�����	
	int  zhuangstation[4];	//ׯ���б�
	__int64   m_iWinQuYu[8];			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	__int64   m_iMeFanFen;			//������ҷ����ķ֣�����ʱ����
	int m_iUserXiaZhuInfo[MAXCOUNT];	//�����ע��Ӯ�͵���Ϣ
	int m_iResultInfoArea[MAXCOUNT];	//�����ע��Ӯ�͵���Ϣ
	tagJieSuan()
	{
		memset(this, 0, sizeof(tagJieSuan));
		memset(m_iUserXiaZhuInfo, 0, sizeof(m_iUserXiaZhuInfo));
		memset(m_iResultInfoArea, 0, sizeof(m_iResultInfoArea));
	}
}JIE_SUAN;
///lym0512
///��ׯ���ݰ�
typedef struct tagShangZhuang
{
	bool shang;		//trueΪ��ׯ��false Ϊ��ׯ
	bool bXiaZhu;   //�Ƿ���ע״̬
	short int  zhuangstation[PLAY_COUNT+1]; 	
	bool isAI;		//�Ƿ��������ׯ
	int station;   //�����λ��
	int  m_iNowNtStation;		//��ǰׯ�ҵ�λ��	
	int  success;	//�Ƿ�ʧ��	
	__int64	 m_iMaxZhu[8];        //ÿ���������µ����ע
	tagShangZhuang()
	{
		memset(this, 0, sizeof(tagShangZhuang));
		memset(zhuangstation, -1, sizeof(zhuangstation));
		m_iNowNtStation = -1;
	}

}SHANG_ZHUANG;

///�����������ݰ�
typedef struct tagChongLian
{
	//int  zhuangstation[4];		//ׯ���б�
	/// ׯ���б��ܵ�ׯ��λ�� ���һ������ӱ�����
	short int  zhuangstation[PLAY_COUNT+1]; 	
	WORD  pai[2][3];			//ׯ�е��ƣ�0Ϊׯ��1Ϊ��
	int  m_iZPaiXing[5];		//ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	int  m_iXPaiXing[5];		//�м�����
	__int64  m_iZhuangFen;			//ׯ�ҵĵ÷�
	__int64  m_iXianFen;			//�мҵĵ÷�
	__int64  m_iUserFen;			//��ǰ��ҵĵ÷�
	int  m_iWinner;				//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	int  m_iKaiPai;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	BYTE m_CardCount[2];		//˫����������   
	int  m_iNTdata[4];			//ׯ�ҵ�λ��,�ܷ֣���ǰ����������ɼ�(Ӯ���ܽ��)����������ׯ�ľ�����
	BYTE  m_iResultInfo[MAXCOUNT][4];	//���30�ֵ���Ϣ
	__int64  m_iShangZhuangLimit;	//��ׯ��Ҫ�����ٽ��
	int  m_iBaSHu;				//���֣�30��һ�֣����еİ���
	int  m_iZhuangBaShu;		//ׯ�ҽ����˼���
	int  m_iNowNtStation;		//��ǰׯ�ҵ�λ��
	int  m_iGameCount;			//��ǰ�Ѿ������˼���
	int  m_iSYTime;             //ʣ��ʱ��

	__int64 	 iMaxZhu[8];			//ÿ���������µ����ע
	__int64      iZhongZhu;			//���ѵ�ǰ��ע��
	__int64  m_iQuYuZhu[8];			//����ÿ�������µ�ע��
	__int64  m_iQuYuJinBi[8][6];	//ÿ�����¸��ࣨ��6�ࣩ��ҵĸ���
	int	 m_iMeZhu[9];			//����ÿ�������µ�ע ����ע 0-7ÿ���������ע��8��עlym 
	int  m_iAIMoney[8];			///�����˵���ע
	int  m_iXiaZhuTime ;		//��עʱ��
	int  m_iKaiPaiTime ;		//����ʱ��
	int  m_iFreeTime ;			//����ʱ��
	int  m_iSendInterval;		//���Ƽ��ʱ��
	int  m_iSendCardTime;		//����ʱ��(ms)
	int  m_iSendCardZhen;		//����֡��
	BYTE m_bGameStation;		///< ��Ϸ״̬lym2010-03-19
	int  m_iNowJuShu;			//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
	bool m_bIsJieSuan;			//�Ƿ��Ѿ�����2010-05-21
	__int64   iMaxNote;			//������ע�ⶥ
	__int64  m_iBaseMaxNote;		//��ͨ��������ע
	BOOL bIsVipDouble;               /**<�Ƿ�Ϊ˫����ע����*/
	int m_iUserXiaZhuInfo[MAXCOUNT];	//�����ע��Ӯ�͵���Ϣ
	int m_iResultInfoArea[MAXCOUNT];	//��ʷ��ׯ�кͺ͵���Ϣ
	tagChongLian()
	{
		memset(this, 0, sizeof(tagChongLian));
		memset(zhuangstation, -1, sizeof(zhuangstation));	
		m_iNowNtStation=-1;
		m_bGameStation= 255;

	}

}CHONG_LIAN;

///�ȴ���Ϣ���ݰ�
typedef struct tagDengDai
{	
	//int  zhuangstation[4];//ׯ���б�
	/// ׯ���б��ܵ�ׯ��λ�� ���һ������ӱ�����
	short int  zhuangstation[PLAY_COUNT+1]; 	
	__int64  m_iNtWin;              //��ǰׯ��Ӯ�Ľ��
	int  m_iZhuangBaShu;        //ׯ�ҽ����˼���
	tagDengDai()
	{
		memset(zhuangstation, -1, sizeof(zhuangstation));
		m_iNtWin = 0;              //��ǰׯ��Ӯ�Ľ��
		m_iZhuangBaShu = 0;        //ׯ�ҽ����˼���
	}

}DENG_DAI;

///��ҵĵ÷���Ϣ
typedef struct tagUserScore
{
	bool     bShowInfo[PLAY_COUNT] ; 
	__int64  llUserFen[PLAY_COUNT] ; ///������ҵĵ÷����

	tagUserScore()
	{
		memset(this ,0 ,sizeof(tagUserScore)) ; 
	}

}USER_SCORE_STRUCT;  


///��ʼǰ��ϴ�õ�������lym2010-03-17
typedef struct tagResultData
{
	WORD  Z_Pai[MAXCOUNT][3];			//ׯ��72��������
	WORD  X_Pai[MAXCOUNT][3];			//�е�72��������
	BYTE  m_iWinner[MAXCOUNT];			//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	BYTE  m_iKaiPai[MAXCOUNT];			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	BYTE  m_iZPaiXing[MAXCOUNT][5];		//ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	BYTE  m_iXPaiXing[MAXCOUNT][5];		//�м�����
	BYTE  m_iWinQuYu[MAXCOUNT][8];		//Ӯ������
	BYTE  m_CardCount[MAXCOUNT][2];		//˫���������� 
	BYTE  m_iNowBaShu;					//��ǰ����

	tagResultData()
	{
		Init();
	}
	void Init()
	{
		memset(Z_Pai, 0, sizeof(Z_Pai));
		memset(X_Pai, 0, sizeof(X_Pai));
		memset(m_iWinner, 0, sizeof(m_iWinner));
		memset(m_iKaiPai, 0, sizeof(m_iKaiPai));
		memset(m_iZPaiXing, 0, sizeof(m_iZPaiXing));
		memset(m_iXPaiXing, 0, sizeof(m_iXPaiXing));
		memset(m_iWinQuYu, 0, sizeof(m_iWinQuYu));
		memset(m_CardCount, 0, sizeof(m_CardCount));
		m_iNowBaShu = 0;				//��ǰ����
	}
}RESULT_DATA;

///lym0512
///�������ݰ�
typedef struct tagTest
{
	int type;   //��������
	tagTest()
	{
		memset(this, 0, sizeof(tagTest));
	}

}BJL_TEST;

//����ׯ����Ӯ���ݰ�
typedef struct tagSuperSetting
{
	char name[256];	//������ҵ��ǳ�
	BYTE bDeskStation;//������ҵ�λ��
	BYTE bState;   // 0:����  1: ׯ��Ӯ 2: ׯ����
	tagSuperSetting()
	{
		memset(this, 0, sizeof(tagSuperSetting));
	}
}SUPERSETTING;

///֪ͨ�ͻ����Ƿ�Ϊ�����ͻ���״̬
typedef struct tagSuperState
{
	BYTE bDeskStation;
	BOOL bEnable;     // �Ƿ�ͨ
	__int64  m_iOldMomey[PLAY_COUNT];              //��ǰׯ��Ӯ�Ľ��
	tagSuperState()
	{
		memset(this, 0, sizeof(tagSuperState));
	}
}SUPERSTATE;


///��������Ӯ���� �������˿�����Ϣ
typedef struct tagAICtrlInfo
{
	BYTE	bDeskStation;			///֪ͨ��λ��
	bool	bAIWinAndLostAutoCtrl ;	///��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
	int		iAIWantWinMoney;		///��������Ӯ���ƣ�������ҪӮ��Ǯ
	int		iAIHaveWinMoney;		///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ
	int		iAIMaxWinInOne;			///��������Ӯ���ƣ�������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�		
	int		iAICtrlCount;			///��������Ӯ���ƣ���ǰ�������Ѿ��������Զ����Ƶİ��� 
	bool    bIsClearAIWinMoney;     ///������׬��Ǯ��������
	bool    bIsSetAICtrl;			///�������������û����˵�����
	tagAICtrlInfo()
	{
		memset(this, 0, sizeof(tagAICtrlInfo));
	}

}AICTRLINFO;




// ����GM��ע���
typedef struct tagSuperGM 
{
	__int64 iUserStation;		/* ��ҵ�λ�� */
	__int64	iOldMoney;			/* ��ҵĴ����� */
	__int64 iUserNote[8];		/* 0���������ע��� */
	__int64	iAllNote;			/* ����ܵ���ע��� */
	TCHAR cUserName[MAX_PATH];	/* ��ҵ����� */
	bool bUsed;					/* ʹ��������ṹ ����Ϊ�ж� */
	bool bIsRobot;				/* �Ƿ�Ϊ������ */
	void Init()					/* ��ʼ�� */
	{
		iUserStation = -1;
		iOldMoney = 0;
		memset(iUserNote, 0, sizeof(iUserNote));
		iAllNote = 0;
		memset(cUserName, 0, sizeof(TCHAR) * MAX_PATH);
		bUsed = false;
		bIsRobot = false;
	}
} SUPERGM;

typedef struct tagSuperData {

	SUPERGM sUserInfo[PLAY_COUNT];		/* ������ҵ���� */
	void Init()								/* ��ʼ�� */
	{
		for (int i = 0; i < PLAY_COUNT; i ++)
		{
			sUserInfo[i].Init();
		}
	}
} SUPERDATA;


#define ASS_SUPER_SET_CARD         79        // ��������Ϣid��ƽ̨��������Ϸ�����޸�

/// ��ϢID
#define ASS_UPDATE_CLIENT_VIEW    100        // ֪ͨ���¿ͻ��˽���  
#define ASS_SEND_CARD             101        // ����
#define ASS_GAME_BEGIN            102        // ��������Ϸ��ʼ
#define ASS_CUT_END				  103		 // �û�ǿ���뿪
#define ASS_SAFE_END			  104		 // �û���ȫ����

#define ASS_TRUSTEE               110        // �й�
#define ASS_HAVE_THING            120        // ����Ҫ����Ϣ
#define ASS_USER_AGREE_LEAVE      121        // ����Ҫ����Ϣ�𸴽��
#define ASS_ALL_USER_AGREEED      122        // ����Ҫ����Ϣ�����˴���
#define ASS_WATCH_SET_CHANGE      123        // ����޸��Թ�����
/// ��ע��Ϣ
#define ASS_XIA_ZHU               130    
/// ������Ϣ
#define ASS_KAI_PAI				  131        
/// �ȴ���Ϣ
#define ASS_DENG_DAI              132        
/// ��ׯ��Ϣ
#define ASS_SHANG_ZHUANG          133         
/// �ټ��ֿ�ʼ��Ϣ
#define ASS_BJL_BEGIN             134        
/// ��Ϸ��ׯ������Ϣ��Ϣ
#define ASS_WU_ZHUANG             135    
/// ֪ͨ��ʾ������Ϣ��Ϣ
#define ASS_SHOW_JS				  136        

/// ��������ʱ������Ϸ������Ϣ��lym2010-03-18
#define ASS_APPLY_GAME_DATA		  137   

#define ASS_USER_SCORE            138        ///��ҵ÷���Ϣ

#define ASS_BJL_TEST              140        // ������Ϣ

#define ASS_SUPER_SETTING         150        // �ɿͻ��˷�����Ϣ���Ʊ�����Ӯ
#define ASS_SUPER_STATE           151        // ֪ͨ�ͻ����Ƿ�Ϊ����״̬
#define ASS_SUPER_AI_INFO         152        /////��������Ӯ���� �� ֪ͨ�ͻ��˻����˵�ǰ����Ϣ

#define ASS_XIA_ZHU_FULL		  153        /////��ע������Ϣ add xyh 2011-12-6
#define ASS_CHANGE_NT			  154        /////��ׯ��Ϣ add xyh 2011-12-8

#pragma pack(pop)
#endif // !defined(_POKERMESSAGE_H__INCLUDED_)
