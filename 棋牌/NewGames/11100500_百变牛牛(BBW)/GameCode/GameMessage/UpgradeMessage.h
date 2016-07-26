// GameMsgPkg.h: game message package and define const.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_UPGRADEMESSAGE_H_INCLUDE_)
#define _UPGRADEMESSAGE_H_INCLUDE_

#pragma pack(push,1)

#define CHECK_DEBUG   1

// ��Ϸ������Ϣ
#define GAMENAME						TEXT("�ٱ�ţţ")                          // ��Ϸ����
#define GAME_DLL_AUTHOR                 TEXT("���ں������Ƽ����޹�˾")            //
#define GAME_DLL_NOTE                   TEXT("���� -- ��Ϸ���")


#define NAME_ID							11100500								 // ���� ID


#define MAX_NAME_INFO                   256
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)

#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // �ͻ���Ŀ¼
#define SKIN_CARD						TEXT("CardSkin")		                 // ����ͼƬ�ļ���
//-------------------------------------------------------------------------
#define GET_SKIN_FOLDER(A)				(sprintf(A,"%d",NAME_ID),strcat(A,""))
static TCHAR szTempStr[MAX_NAME_INFO] =		{0};

// �汾����
#define GAME_MAX_VER					1								        // ������߰汾
#define GAME_LESS_VER					1								        // ������Ͱ汾
#define GAME_CHANGE_VER					0								        // �޸İ汾

// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

#define PLAY_COUNT						180							        // ��Ϸ����

//��Ϸ�ڲ������汾��
#define DEV_HIGH_VERSION				2				                        // �ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					1				                        // �ڲ������Ͱ汾��

// ��Ϸ״̬����
#define GS_WAIT_SETGAME					0				// �ȴ���������״̬
#define GS_WAIT_AGREE					1				// �ȴ�ͬ������
#define GS_FREE							20				// ����״̬
#define GS_BET							21				// ��ע״̬
#define GS_SEND_CARD					22				// ����״̬
#define GS_PLAY_GAME					23				// ����״̬
#define GS_WAIT_NEXT					24				// �ȴ���һ�̿�ʼ 

//�ٱ�ţţ��ע�����嶨��
#define GF_TIANG				0			   // ��
#define GF_DI			        1			   // ��
#define GF_XUAN  				2			   // ��
#define GF_HUANG				3			   // ��

#define MAX_NOTE_COUNT          4              //��ע������  ����Ϸ���� ���� ���� ���� �ĸ���ע��

/** ����·�Ӱ��� */
#define  GAME_SAVE_COUNT  72

/*------------------------------------------------------------------------*/
//���������������ݰ�
struct	TagGameStation_Base
{
	
	BYTE		byGameStaion;	//��Ϸ״̬

	BYTE		bMaxBase;		/// ����ʼ�ţţ�ı���
	
	int			iXiaZhuTime ;	///��עʱ��
	int			iKaiPaiTime ;	///����ʱ��		
	int			iFreeTime ;		///����ʱ��	
	int			iRemaindTime;	///ʣ��ʱ��

	int			iNowNtStation;	///��ǰׯ�ҵ�λ��
	int			iGameBeen;		///��ǰ����		
	int			iZhuangBaShu;	///ׯ�ҽ����˼���	
	int			iGameCount;		///��ǰ�Ѿ������˼���
	__int64		i64NtWin;		///ׯ����Ӯ�ɼ�
	
	int			iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];	///���X�ֵ���Ϣ	
	short int	idzhuangstation[PLAY_COUNT+1][2];			//����һ����ά���飬��һ�б�ʾ��ׯ��,�ڶ��б�ʾ�Ƿ���ׯ;

	__int64		i64ShangZhuangLimit;			///��ׯ��Ҫ�����ٽ��
	__int64		i64MaxNote;						///������Ϸ�����ע��
	__int64		i64RectNoteMax[MAX_NOTE_COUNT]; ///������ע����
	__int64		i64ChoumaValue[6];				// ���������ֵ
	
	
	TagGameStation_Base()
	{
		memset(this,0,sizeof(TagGameStation_Base));
	}
};
/*------------------------------------------------------------------------*/
//�����������н׶����ݰ�
struct	TagGameStation_WaiteAgee:public TagGameStation_Base
{
	TagGameStation_WaiteAgee()
	{
		memset(this,0,sizeof(TagGameStation_WaiteAgee));
	}
};
/*------------------------------------------------------------------------*/
//����������ע�׶����ݰ�
struct	TagGameStation_Bet
{
	BYTE		byGameStaion;	//��Ϸ״̬
	BYTE		bMaxBase;		/// ����ʼ�ţţ�ı���

	int			iXiaZhuTime ;	///��עʱ��
	int			iKaiPaiTime ;	///����ʱ��		
	int			iFreeTime ;		///����ʱ��	
	int			iRemaindTime;	///ʣ��ʱ��

	int			iNowNtStation;	///��ǰׯ�ҵ�λ��
	int			iGameBeen;		///��ǰ����		
	int			iZhuangBaShu;	///ׯ�ҽ����˼���	
	int			iGameCount;		///��ǰ�Ѿ������˼���
	
	short int	idzhuangstation[PLAY_COUNT+1][2];			//����һ����ά���飬��һ�б�ʾ��ׯ��,�ڶ��б�ʾ�Ƿ���ׯ;
	char		iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];	///���X�ֵ���Ϣ	
	
	__int64		i64NtWin;		///ׯ����Ӯ�ɼ�
	__int64		i64ShangZhuangLimit;			///��ׯ��Ҫ�����ٽ��
	__int64		i64MaxNote;						///������Ϸ�����ע��
	__int64		i64RectNoteMax[MAX_NOTE_COUNT]; ///������ע����
	__int64		i64ChoumaValue[6];				// ���������ֵ
	
	__int64  iZhongZhu;						///���ѵ�ǰ��ע��		
	__int64  iQuYuZhu[MAX_NOTE_COUNT];		///����ÿ�������µ�ע��
	__int64  iQuYuZhuTrue[MAX_NOTE_COUNT];	///������ʵÿ�������µ�ע��
	__int64  iMyZhu[MAX_NOTE_COUNT];		///����������ע
	TagGameStation_Bet()
	{
		memset(this,0,sizeof(TagGameStation_Bet));
	}
};
/*------------------------------------------------------------------------*/
//�����������ƽ׶����ݰ�
struct	TagGameStation_OpenCard
{
	BYTE		byGameStaion;	//��Ϸ״̬
	BYTE		bMaxBase;		/// ����ʼ�ţţ�ı���

	
	BYTE		byCard[5][5];	
	BYTE		byCardCount[5];

	int			iXiaZhuTime ;	///��עʱ��
	int			iKaiPaiTime ;	///����ʱ��		
	int			iFreeTime ;		///����ʱ��	
	int			iRemaindTime;	///ʣ��ʱ��

	int			iNowNtStation;	///��ǰׯ�ҵ�λ��
	int			iGameBeen;		///��ǰ����		
	int			iZhuangBaShu;	///ׯ�ҽ����˼���	
	int			iGameCount;		///��ǰ�Ѿ������˼���

	int			iComputePoint[MAX_NOTE_COUNT];	
	
	short int	idzhuangstation[PLAY_COUNT+1][2];			//����һ����ά���飬��һ�б�ʾ��ׯ��,�ڶ��б�ʾ�Ƿ���ׯ;
	char		iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];	///���X�ֵ���Ϣ	
	
	__int64		i64NtWin;		///ׯ����Ӯ�ɼ�
	__int64		i64ShangZhuangLimit;			///��ׯ��Ҫ�����ٽ��
	__int64		i64MaxNote;						///������Ϸ�����ע��
	__int64		i64RectNoteMax[MAX_NOTE_COUNT]; ///������ע����
	__int64		i64ChoumaValue[6];				// ���������ֵ
	
	__int64  iZhongZhu;						///���ѵ�ǰ��ע��		
	__int64  iQuYuZhu[MAX_NOTE_COUNT];		///����ÿ�������µ�ע��
	__int64  iQuYuZhuTrue[MAX_NOTE_COUNT];	///������ʵÿ�������µ�ע��
	__int64  iMyZhu[MAX_NOTE_COUNT];		///����������ע
	TagGameStation_OpenCard()
	{
		memset(this,0,sizeof(TagGameStation_OpenCard));
	}
};
/*------------------------------------------------------------------------*/
//������������׶����ݰ�
struct	TagGameStation_ShowResult
{
	BYTE		byGameStaion;	//��Ϸ״̬
	BYTE		bMaxBase;		/// ����ʼ�ţţ�ı���

	BYTE		byCard[5][5];	
	BYTE		byCardCount[5];

	int			iXiaZhuTime ;	///��עʱ��
	int			iKaiPaiTime ;	///����ʱ��		
	int			iFreeTime ;		///����ʱ��	
	int			iRemaindTime;	///ʣ��ʱ��

	int			iNowNtStation;	///��ǰׯ�ҵ�λ��
	int			iGameBeen;		///��ǰ����		
	int			iZhuangBaShu;	///ׯ�ҽ����˼���	
	int			iGameCount;		///��ǰ�Ѿ������˼���

	
	
	short int	idzhuangstation[PLAY_COUNT+1][2];			//����һ����ά���飬��һ�б�ʾ��ׯ��,�ڶ��б�ʾ�Ƿ���ׯ;
	char		iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];	///���X�ֵ���Ϣ	
	
	__int64		i64NtWin;		///ׯ����Ӯ�ɼ�
	__int64		i64ShangZhuangLimit;			///��ׯ��Ҫ�����ٽ��
	__int64		i64MaxNote;						///������Ϸ�����ע��
	__int64		i64RectNoteMax[MAX_NOTE_COUNT]; ///������ע����
	__int64		i64ChoumaValue[6];				// ���������ֵ
	
	__int64  iZhongZhu;						///���ѵ�ǰ��ע��		
	__int64  iQuYuZhu[MAX_NOTE_COUNT];		///����ÿ�������µ�ע��
	__int64  iQuYuZhuTrue[MAX_NOTE_COUNT];	///������ʵÿ�������µ�ע��
	__int64  iMyZhu[MAX_NOTE_COUNT];		///����������ע
	
	__int64  iZhuangFen;		///ׯ�ҵĵ÷�		
	__int64  iXianFen;			///�мҵĵ÷�	
	__int64  iUserFen;			///��ǰ��ҵĵ÷�
	TagGameStation_ShowResult()
	{
		memset(this,0,sizeof(TagGameStation_ShowResult));
	}
};
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------*/
///��Ϸ��ʼ��Ϣ
typedef		struct tagBeginData
{	
	__int64  iNTdata[4];		//ׯ�ҵ�λ��,�ܷ֣���ǰ����������ɼ�(Ӯ���ܽ��)����������ׯ�ľ�����
	__int64  iShangZhuangLimit;		///��ׯ��Ҫ�����ٽ��	
	__int64  iQiangZhuangMoney;	//20121116dwj ��ׯ���ѵĽ��
	__int64	 iMaxZhu;     ///�������µ����ע
	__int64	 i64ChoumaValue[6];	// ���������ֵ


	short int iNTlist[PLAY_COUNT+1][2];		//ׯ���б�λ�� ��һ�б�ʾׯ���б��ڶ��б�ʾ�Ƿ���ׯ��

	int  iGameBeen;       ///��ǰ����     	 
	int  iZhuangBaShu;     ///ׯ�ҽ����˼���	
	int  iNowNtStation;    ///��ǰׯ�ҵ�λ�� 	
	int  iGameCount;      ///��Ϸ����  		      	
	int  iXiaZhuTime ;	///��עʱ��		
	int  iKaiPaiTime ;	///����ʱ��		
	int  iFreeTime ;	///����ʱ��

	BYTE iMaxBase;   //��Ϸ�����
	


	tagBeginData()
	{
		memset(this, 0, sizeof(tagBeginData));
		iNowNtStation=-1;
		memset(i64ChoumaValue, 0, sizeof(i64ChoumaValue));
	}
}BEGIN_DATA;
/*----------------------------------------------------------------------------------*/
///��ע���ݰ�
struct	TagXiaZhu
{	
	bool		bIsAi;		/// �Ƿ��������ע
	BYTE		byStation;	///λ��	
	int			iBetArea;	///��ע����1-4
	int			iChouMaType;//��������

	int			iMoney;		//��עֵ
	__int64  iMaxZhu;		///���������ע	
	__int64  iZhongZhu;    ///���ѵ�ǰ��ע��  
	__int64		iUserXiaZhu[MAX_NOTE_COUNT+1];	//���������ע
	__int64		iQuYuZhu[MAX_NOTE_COUNT];	///����ÿ�������µ�ע��	
	__int64		iQuYuZhuTrue[MAX_NOTE_COUNT];		/// ��ʵ����ע	
   

	TagXiaZhu()
	{
		memset(this, 0, sizeof(TagXiaZhu));
	}
};

/*---------------------------------------------------------------------*/
///�������ݰ�
typedef		struct tagResult
{
	__int64		iZhuangFen;    ///ׯ�ҵĵ÷� 	
	__int64		iXianFen;		///�мҵĵ÷�	
	__int64		iUserFen;		///��ǰ��ҵĵ÷�	
	__int64		iNtWin;			///��ǰׯ��Ӯ�Ľ��
	int			iResultInfo[GAME_SAVE_COUNT][MAX_NOTE_COUNT];	//·����Ϣ
	tagResult()
	{
		memset(this,0,sizeof(tagResult));
	}
};

/*---------------------------------------------------------------------*/
///�������ݰ�
typedef		struct tagKaiPai
{
	BYTE	pai[5][5];			//������     
	int		iCardCount[5];		//������
	int		iComputePoint[MAX_NOTE_COUNT];		///��Ϸ��ӮǮ���� 		

	tagKaiPai()
	{
		memset(this, 0, sizeof(tagKaiPai));
		memset(pai, 255, sizeof(pai));
	}
}KAI_PAI;

/*-------------------------------------------------------------------------*/
///��ׯ���ݰ�
typedef		struct tagShangZhuang
{
	///�����λ��
	int station;   
	bool shang;		
	bool qiang;
	__int64 iQiangZhuangMoney;
	//20121119dwj ����һ����ά���飬��һ�б�ʾ��ׯ��,�ڶ��б�ʾ�Ƿ���ׯ;
	short int  dzhuangstation[PLAY_COUNT+1][2];
	///�Ƿ�ʧ��
	bool success;  
	/// �Ƿ������
	BOOL bIsAi;      
	tagShangZhuang()
	{
		memset(this, 0, sizeof(tagShangZhuang));
		memset(dzhuangstation, -1, sizeof(dzhuangstation));
	}
}SHANG_ZHUANG;
/*-------------------------------------------------------------------------*/
///��ׯ
typedef struct tagChangeZhuang
{    ///��һ��ׯ��
	int bLastStation;
	/// ��ǰׯ��
	int bNowStation;  
	// ����һ����ά���飬��һ�б�ʾ��ׯ��,�ڶ��б�ʾ�Ƿ���ׯ;
	short int  dzhuangstation[PLAY_COUNT+1][2];
	tagChangeZhuang()
	{
		memset(this, 0, sizeof(tagChangeZhuang));
		memset(dzhuangstation,-1,sizeof(dzhuangstation));
	}

}CHANGE_ZHUANG;


///�ȴ���Ϣ���ݰ�
typedef struct tagDengDai
{
	//����һ����ά���飬��һ�б�ʾ��ׯ��,�ڶ��б�ʾ�Ƿ���ׯ;
	short int  dzhuangstation[PLAY_COUNT+1][2];
	int  iZhuangBaShu;//ׯ�ҽ����˼���      
	int  iZhuang;//ׯ��λ��
	__int64  iNtWin; //��ǰׯ��Ӯ�Ľ��    
	tagDengDai()
	{
		memset(dzhuangstation, -1, sizeof(dzhuangstation));
		iNtWin = 0;              //��ǰׯ��Ӯ�Ľ��
		iZhuangBaShu = 0;        //ׯ�ҽ����˼���
		iZhuang=-1;
	}
}DENG_DAI;

///�������ݰ�
typedef struct tagTest
{ 
	///��������
	int type;  
	tagTest()
	{
		memset(this, 0, sizeof(tagTest));
	}

}BJL_TEST;

//������Ӯ���ݰ�
struct CONTROL_WIN
{	
	BYTE bGameWinStation;   ///Ӯ��λ�ã�0-4 ����ׯλ���죬�أ�������	
	BYTE bGameLoseStation;  ///���λ�ã�0-4 ����ׯλ���죬�أ�������	
	BYTE bGameWinQueue[MAX_NOTE_COUNT];///����������ҿ��Ƶ���Ӯ0-3�����죬�أ������ƣ�0�����䡣1����Ӯ 255��ʾ������
	CONTROL_WIN()
	{
		memset((void *)this,0,sizeof(CONTROL_WIN));
		memset(bGameWinQueue,255,sizeof(bGameWinQueue));
	}
};

/*--------------------------------------------------------------*/
///�������ݰ�
struct	TagSuperUser
{	
	BYTE		byStation;			///λ��
	bool        bIsSuper;           /**< �Ƿ�ͨ */

	TagSuperUser()
	{
		byStation = -1;
		bIsSuper = false;
	}
};
/*--------------------------------------------------------------*/
//�����������ݰ�
struct	TagSuperSet
{
	BYTE		byStation;			///λ��
	bool		bSuccesse;			//�����Ƿ�ɹ�
	int			iAreaContrl[4];		///��������������  -1��ʾ�� 0-��ʾ������ 1��ʾӮ

	TagSuperSet()
	{
		memset(this,0,sizeof(TagSuperSet));
		byStation = 255;
		bSuccesse = false;
	}
	void	init()
	{
		memset(this,0,sizeof(TagSuperSet));
		byStation = 255;
		bSuccesse = false;
	}
};
/*--------------------------------------------------------------*/

#define ASS_SUPER_SET_CARD         79        /// ��������Ϣid��ƽ̨��������Ϸ�����޸�

// ��ϢID
#define ASS_GAME_BEGIN				111			/// ��Ϸ��ʼ	DWJ
#define ASS_KAI_PAI					112			// ������Ϣ	DWJ
#define ASS_RESULT					113			///����		DWJ
#define ASS_FREE_TIME				114			/// ������Ϣ		DWJ
#define ASS_WU_ZHUANG				115			/// ��Ϸ��ׯ������Ϣ��Ϣ	DWJ

#define ASS_IS_SUPER_USER			120			//֪ͨ����ǳ���	DWJ
#define ASS_SUPER_SET				121			//��������		

#define ASS_XIA_ZHU					130        /// ��ע��Ϣ
#define ASS_SHANG_ZHUANG			131			/// ��ׯ��Ϣ		DWJ


#pragma pack(pop)
#endif // !defined(_POKERMESSAGE_H__INCLUDED_)
