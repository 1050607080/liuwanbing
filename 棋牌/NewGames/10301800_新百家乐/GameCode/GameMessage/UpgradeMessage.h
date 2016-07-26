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
#define NAME_ID							10301800								 // ���� ID
#else
#define NAME_ID							10301800								 // ���� ID


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

#define	UG_VALUE_MASK					0x0F			//0000 1111
// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

#define PLAY_COUNT						180							        // ��Ϸ����

//��Ϸ�ڲ������汾��
#define DEV_HIGH_VERSION				2				                        // �ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0				                        // �ڲ������Ͱ汾��

// ��Ϸ״̬����
#define GS_WAIT_SETGAME					0				// �ȴ���������״̬
#define GS_WAIT_AGREE					1				// �ȴ�ͬ������
#define GS_NOTE_STATE					20				// ��ע״̬
#define GS_SEND_CARD					21				// ����״̬
#define GS_OPEN_RESULT					22				// ����״̬
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


#define MAXCOUNT						30            


const	int	BET_ARES = 12;	//8����ע����

struct LuziData
{
	BYTE		byZPoint;
	BYTE		byXpoint;

	BYTE		byWinResult;	//Ӯ������ 0=�� 1-�� 2-��

	LuziData()
	{
		memset(this,255,sizeof(LuziData));
	}
};

/// ��Ϸ������Ϣ
typedef struct tagGameBaseInfo
{
	//��Ϸ�汾
	BYTE byHighVersion;		// ��Ϸ�汾��
	BYTE byLowVersion;		// ��Ϸ�汾��



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
	short int  m_iNTlist[PLAY_COUNT+1]; 

	int  m_iShangZhuangLimit; //��ׯ��Ҫ�����ٽ��
	
	int  m_iXiaZhuTime ;		//��עʱ��
	int  m_iKaiPaiTime ;		//����ʱ��
	int	 m_iShowResult;			//�������ʾʱ��
	int  m_iFreeTime ;			//����ʱ��

	__int64 	 iMaxNote;			//������ע�ⶥ
	__int64	     m_iMaxZhu[BET_ARES];        //ÿ���������µ����ע	

	int		m_iNowNtStation;     //��ǰׯ�ҵ�λ��
	int		m_iZhuangBaShu;      //ׯ�ҽ����˼���
	__int64	m_i64NtMoney;		//ׯ�ҵĽ����
	__int64	m_i64NtWin;			//ׯ����Ӯ���

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

	int			station;//λ��
	__int64		money;//���
	int			type;//��ע����0-11
	int			moneytype;//�������ͣ�1���٣�2��ǧ��3����4��ʮ��5������6�������

	__int64 iMeZhongZhu;      //���ѵ�ǰ��ע��
	__int64	iAllZhongZhu;		//������ע

	__int64 m_iMaxZhu[BET_ARES];		//ÿ���������¶���ע
	__int64 m_iQuYuZhu[BET_ARES];    //����ÿ�������µ�ע��
	__int64 m_iAIMoney[BET_ARES];		///�����˵���ע	
	__int64 m_iUserXiaZhu[BET_ARES];   //��ҵ�ǰ��ÿ�������µ�ע��
	
	
	tagXiaZhu()
	{
		memset(this, 0, sizeof(tagXiaZhu));
	}
}XIA_ZHU;
///lym0512
///�������ݰ�
typedef struct tagKaiPai
{

	BYTE    pai[2][3];        //ׯ�е��ƣ�0Ϊׯ��1Ϊ��

	int  	 iZPaiXing[5];   //ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	int      iXPaiXing[5];   //�м�����
	int      iWinQuYu[BET_ARES];			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���	

	
	tagKaiPai()
	{
		memset(this, 0, sizeof(tagKaiPai));
	}
}KAI_PAI;
///�������ݰ�lym1204
typedef struct tagJieSuan
{
	//20141020 dwjtest ���������⣿
	LuziData	TLuziData[MAXCOUNT];	//·����Ϣ
	__int64  i64ZhuangScore;		//ׯ�ұ��ֵĵ÷�
	__int64  i64MeScore;			//�Լ����ֵĵ÷�
	__int64  i64MeFanFen;		//������ҷ����ķ֣�����ʱ����
	__int64  i64UserWin;		//��ǰ��ҵĵ÷� ���ɼ���
	__int64  i64NtWin;          //��ǰׯ��Ӯ�Ľ�ң��ɼ���
	int		 iPlayCount;		//���˶�����
	int		 iWinCount;			//Ӯ�˶�����

	tagJieSuan()
	{
		memset(this, 0, sizeof(tagJieSuan));
		memset(TLuziData,255,sizeof(TLuziData));
	}
}JIE_SUAN;
///lym0512
///��ׯ���ݰ�
typedef struct tagShangZhuang
{
	bool shang;		//trueΪ��ׯ��false Ϊ��ׯ
	short int  zhuangstation[PLAY_COUNT+1]; 	
	bool isAI;		//�Ƿ��������ׯ
	int station;   //�����λ��
	int  m_iNowNtStation;		//��ǰׯ�ҵ�λ��	
	int  success;	//�Ƿ�ʧ��	 0-��ʾ�ɹ� 1-��Ҳ��� 2-�Ѿ���ׯ���б���

	tagShangZhuang()
	{
		memset(this, 0, sizeof(tagShangZhuang));
		memset(zhuangstation, -1, sizeof(zhuangstation));
		m_iNowNtStation = -1;
	}

}SHANG_ZHUANG;

/*--------------------------------------------------------------------------*/
//��Ϸ��������
struct	GameStationBase
{
	BYTE		m_bGameStation;			///< ��Ϸ״̬
	
	int			m_iXiaZhuTime ;			//��עʱ��
	int			m_iKaiPaiTime ;			//����ʱ��
	int			m_iShowResult;			//�������ʾʱ��
	int			m_iFreeTime ;			//����ʱ��

	int			m_iZhuangBaShu;			//ׯ�ҽ����˼���
	int			m_iNowNtStation;		//��ǰׯ�ҵ�λ��

	int			iZhuangWaitList[PLAY_COUNT+1];	//��ׯ�б� 

	__int64		i64NtWin;				//ׯ����Ӯ
	__int64		i64MeWin;				//�Լ���Ӯ

	__int64		iMaxNote;				//������ע�ⶥ
	__int64		m_iShangZhuangLimit;	//��ׯ��Ҫ�����ٽ��


	LuziData	TLuziData[MAXCOUNT];	//·����Ϣ

	GameStationBase()
	{
		memset(this, 0, sizeof(GameStationBase));
		memset(TLuziData,255,sizeof(TLuziData));
	}
};
/*--------------------------------------------------------------------------*/
//��ע״̬����
struct	GameStation_Bet:public GameStationBase
{
	__int64		i64AresMaxZhu[BET_ARES];			//ÿ���������¶���ע
	__int64		i64XiaZhuData[BET_ARES];			//�����������ע
	__int64		i64UserXiaZhuData[BET_ARES];		//����ڸ��������ע���
	__int64		i64UserXiaZhuSum;					//��ҵ�����ע
	__int64		i64XiaZhuSum;						//������ҵ�����ע

	GameStation_Bet()
	{
		memset(this,0,sizeof(GameStation_Bet));
		memset(TLuziData,255,sizeof(TLuziData));
	}
};
/*--------------------------------------------------------------------------*/
//����״̬
struct	GameStation_SendCard:public GameStationBase
{
	__int64		i64AresMaxZhu[BET_ARES];			//ÿ���������¶���ע
	__int64		i64XiaZhuData[BET_ARES];			//�����������ע
	__int64		i64UserXiaZhuData[BET_ARES];		//����ڸ��������ע���
	__int64		i64UserXiaZhuSum;					//��ҵ�����ע
	__int64		i64XiaZhuSum;						//������ҵ�����ע

	BYTE		byUserCard[2][3];					//ׯ�е�������
	BYTE		byZPaiXing;							//ׯ����
	BYTE		byXPaiXing;							//������
	BYTE		byWinAreas[BET_ARES];				//Ӯ������//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���

	GameStation_SendCard()
	{
		memset(this,0,sizeof(GameStation_Bet));
		memset(TLuziData,255,sizeof(TLuziData));
		memset(byUserCard,255,sizeof(byUserCard));
	}
};
/*--------------------------------------------------------------------------*/
//����״̬����
struct	GameStation_Result:public GameStationBase
{
	__int64		i64AresMaxZhu[BET_ARES];			//ÿ���������¶���ע
	__int64		i64XiaZhuData[BET_ARES];			//�����������ע
	__int64		i64UserXiaZhuData[BET_ARES];		//����ڸ��������ע���
	__int64		i64UserXiaZhuSum;					//��ҵ�����ע
	__int64		i64XiaZhuSum;						//������ҵ�����ע

	__int64		i64ZhuangScore;		//ׯ�ұ��ֵĵ÷�
	__int64		i64MeScore;			//�Լ����ֵĵ÷�
	__int64		i64MeFanFen;		//������ҷ����ķ֣�����ʱ����


	GameStation_Result()
	{
		memset(this,0,sizeof(GameStation_Result));
		memset(TLuziData,255,sizeof(TLuziData));
	}
};
/*--------------------------------------------------------------------------*/





///�ȴ���Ϣ���ݰ�
typedef struct tagDengDai
{	
	short int	zhuangstation[PLAY_COUNT+1]; 	
	int			iNtStation;
	__int64		i64NtWin;              //��ǰׯ��Ӯ�Ľ��
	int			iZhuangBaShu;        //ׯ�ҽ����˼���
	tagDengDai()
	{
		
		memset(zhuangstation, -1, sizeof(zhuangstation));
		i64NtWin = 0;              //��ǰׯ��Ӯ�Ľ��
		iZhuangBaShu = 0;        //ׯ�ҽ����˼���
		iNtStation = -1;
	}

}DENG_DAI;

///��ׯϢ���ݰ�
typedef struct tagNoZhuang
{	
	int			iNtStation;
	__int64		i64NtWin;              //��ǰׯ��Ӯ�Ľ��
	int			iZhuangBaShu;        //ׯ�ҽ����˼���
	tagNoZhuang()
	{
		i64NtWin = 0;              //��ǰׯ��Ӯ�Ľ��
		iZhuangBaShu = 0;        //ׯ�ҽ����˼���
		iNtStation = -1;
	}

}NO_ZHUANG;

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
	BYTE bDeskStation;//������ҵ�λ��
	BYTE bState;   // 0:����  1: ׯ��Ӯ 2: ׯ���� 3:��
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

	bool    bIsClearAIWinMoney;     ///������׬��Ǯ��������
	bool    bIsSetAICtrl;			///�������������û����˵�����
	tagAICtrlInfo()
	{
		memset(this, 0, sizeof(tagAICtrlInfo));
	}

}AICTRLINFO;









#define ASS_SUPER_SET_CARD         79        // ��������Ϣid��ƽ̨��������Ϸ�����޸�

/// ��ϢID


#define ASS_CUT_END				  103		 // �û�ǿ���뿪
#define ASS_SAFE_END			  104		 // �û���ȫ����

/// ��ע��Ϣ
#define ASS_XIA_ZHU               130    
/// ������Ϣ
#define ASS_KAI_PAI				  131        
/// ��Ϸ���� ������Ϣ 
#define ASS_FREE_STATE            132        
/// ��ׯ��Ϣ
#define ASS_SHANG_ZHUANG          133   
/// ��ׯ��Ϣ����
#define ASS_SHANG_ZHUANG_ERR      143         
/// �ټ��ֿ�ʼ��Ϣ
#define ASS_BJL_BEGIN             134        
/// ��Ϸ��ׯ������Ϣ��Ϣ
#define ASS_WU_ZHUANG             135    
/// ֪ͨ��ʾ������Ϣ��Ϣ
#define ASS_SHOW_JS				  136        


#define ASS_SUPER_SETTING         150        // �ɿͻ��˷�����Ϣ���Ʊ�����Ӯ
#define ASS_SUPER_STATE           151        // ֪ͨ�ͻ����Ƿ�Ϊ����״̬
#define ASS_SUPER_AI_INFO         152        /////��������Ӯ���� �� ֪ͨ�ͻ��˻����˵�ǰ����Ϣ

#define ASS_CHANGE_NT			  154        /////��ׯ��Ϣ add xyh 2011-12-8

#pragma pack(pop)
#endif // !defined(_POKERMESSAGE_H__INCLUDED_)
