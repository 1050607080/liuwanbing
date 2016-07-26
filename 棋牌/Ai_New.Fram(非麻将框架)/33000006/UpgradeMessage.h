#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#define GAMENAME						TEXT("�������")


#define NAME_ID							30000600								//���� ID


//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//��Ϸ��Ϣ

#define PLAY_COUNT						6								//��Ϸ����


const	int CLIENT_WIDTH		=	1280;	//��Ļ��
const	int CLIENT_HEIGHT		=	740;	//��Ļ��


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
#define GET_KEY_NAME(A, B, C, D)		(sprintf(A,"%s%d%s", B, C, D),strcat(A,""))
#define GET_CLIENT_BCF_NAME(A)          (sprintf(A,"%d",NAME_ID),strcat(A,".bcf"))    

#define SUPER_PLAYER                    10                         //�������
#define MAX_NAME_INFO                   256

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_PLAY_GAME					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

//��ע����


/********************************************************************************/

//��Ϸ���ݰ�

/********************************************************************************/
/*-------------------------------------------------------------------------------*/
//��Ϸ״̬�������ݰ�
struct	GameStation_Base
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	bool		bHitFlag;		//�Ƿ���Ϊ�����˻�������߼��ж�
	BYTE		bHitDesk;
	BYTE		byGameStation;					//��Ϸ״̬
	int			iBgindex;		//��������


	int			iFireBase;								//�ڻ��ʼ����
	int			iUserFireBase[PLAY_COUNT];				//����ҵ��ڻ���
	int			iUserScore[PLAY_COUNT];					//����Ϸ���

	GameStation_Base()
	{
		memset(this, 0 , sizeof(GameStation_Base));
	}
};
/*-------------------------------------------------------------------------------*/
//��ҽ�����Ϸ���ݰ�
struct	GameUserComeData
{
	BYTE		byDeskStation;	//�����λ��
	bool		bComeOrLeave;	//����/�뿪
	bool		bIsRobot;		//�Ƿ������
	int			iFireBase;		//�ڻ���
	int			iScore;			//�Ϸ���
	GameUserComeData()
	{
		memset(this, 255, sizeof(GameUserComeData));
		iFireBase	= 0;
		iScore		= 0;
		bComeOrLeave = true;
	}
};
/*-------------------------------------------------------------------------------------*/
struct BulletInfo
{
	int			iBeilv;			//�ӵ�����
	bool 		bHave;			//�ӵ�����
	bool		bIsSuper;		//�Ƿ񳬼��ڵ�

	BulletInfo()
	{
		memset(this , 0, sizeof(BulletInfo));
	}
};
/*-------------------------------------------------------------------------------------*/
//��ʼ�켣��ͽǶ�
struct	FishTrace
{
	float x;    //���X����
	float y;    //Y����
	float rotation;//��ǰ�Ƕ�
	float movetime;//�ƶ�ʱ�䱣������켣�˶���ʱ��
	float changetime;//�ı�ʱ�䣬��֮ǰ�ĽǶȸı䵽���ڽǶȹ涨�����ʱ�������
	bool  m_isHave ;//�����Ƿ���ڣ�Ĭ��Ϊ���ھ����������Ĭ�ϲ�����
	int   m_fishid;//��������Ӧ��FISH��ID��
	int   m_fishtype;//��������Ӧ��FISH��ID��
	int   m_ptindex;
	int   m_BuildTime;
	int   m_shootCount;
	int   m_mustShoot;
	int   m_fudaifishtype;//����Ϊ21��22������Ը���С��
	int   m_speed;

	FishTrace()
	{
		memset(this,0,sizeof(FishTrace));
	}
};
/*-------------------------------------------------------------------------------------*/
//��Ϸ״̬
struct	CMD_S_FishTrace
{
	bool							bRegular;								//�����ʶ
	FishTrace                       m_fishtrace[5];

	CMD_S_FishTrace()
	{
		memset(this, 0, sizeof(CMD_S_FishTrace));
	}
};
/*-------------------------------------------------------------------------------------*/
//�л�����
struct	CMD_S_ChangeScene
{
	WORD     					    SceneIndex;						        //��ǰ����
	WORD                            RmoveID;                                //Ԥ������

	CMD_S_ChangeScene()
	{
		memset(this, 0, sizeof(CMD_S_ChangeScene));
	}
};










/*-------------------------------------------------------------------------------------*/
//�����ӵ�
struct	CMD_C_BuyBullet
{
	bool			bAddOrMove;		//�ϻ�������
	int				iScore;			//���·���		

	CMD_C_BuyBullet()
	{
		memset(this, 0, sizeof(CMD_C_BuyBullet));
	}
};
/*-------------------------------------------------------------------------------------*/
//�����ӵ���Ŀ
struct	CMD_S_BulletCount
{
	bool			bAddOrMove;			//�ϻ�������
	BYTE			byDeskStation;		//���·���λ��
	__int64			i64UserScore;		//��������ܷ���
	__int64			i64UserMoney;		//������ϵĽ��
	CMD_S_BulletCount()
	{
		memset(this, 0, sizeof(CMD_S_BulletCount));
	}
};
/*-------------------------------------------------------------------------------------*/
//��ҷ����ӵ�
struct	CMD_C_UserShoot
{
	int				iBulletCountKind;		//�ӵ�����
	BYTE			byShootCount;			//�����Ŀ
	float			fAngle;					//����Ƕ�
	DWORD			dwBulletID;				//�ӵ���ʶ

	CMD_C_UserShoot()
	{
		memset(this, 0, sizeof(CMD_C_UserShoot));
	}

};
/*-------------------------------------------------------------------------------------*/
//�����ڵ�
struct	CMD_S_UserShoot
{
	BYTE			byStation;				//�������
	bool			bIsRobot;				//�ж��Ƿ��ǻ�����
	BYTE			byShootCount;			//�ӵ���Ŀ	
	float			fAngle;					//����Ƕ�
	int 			iBulletCountKind;		//�ӵ�����
	__int64         i64UserScore;			//���ʣ���ӵ���
	
	CMD_S_UserShoot()
	{
		memset(this, 0, sizeof(CMD_S_UserShoot));
	}
};
/*-------------------------------------------------------------------------------------*/
//�����ڵ�����
struct	CMD_S_UserShootError
{
	BYTE			byStation;				//�������
	bool			bIsRobot;				//�ж��Ƿ��ǻ�����
	int				iFireBase;				//�ڻ���
	__int64         i64UserScore;			//���ʣ���ӵ���

	CMD_S_UserShootError()
	{
		memset(this, 0, sizeof(CMD_S_UserShootError));
	}
};
/*-------------------------------------------------------------------------------------*/
struct	CMD_C_AddFireBase
{
	bool		bAdd;			//��/��
	BYTE		byDeskStation;	//��λ��
	CMD_C_AddFireBase()
	{
		memset(this, 0, sizeof(CMD_C_AddFireBase));
	}
};
/*-------------------------------------------------------------------------------------*/
struct	CMD_S_AddFireBase
{
	BYTE		byDeskStation;	//��λ��
	int			iUserFireBase;	//����ڻ���
	CMD_S_AddFireBase()
	{
		memset(this, 0, sizeof(CMD_S_AddFireBase));
	}
};
/*-------------------------------------------------------------------------------------*/
//������Ⱥ
struct CMD_C_HitFish
{
	int				iFishID;			//��Ⱥ��ʶ
	int				iBulletID;			//�ӵ���ʶ
	int             iBulletUserId;		//�����ӵ���ҵ�ID
	bool            bIsAndroid;			//�Ƿ������

	CMD_C_HitFish()
	{
		memset(this, 0, sizeof(CMD_C_HitFish));
	}
};
/*-------------------------------------------------------------------------------------*/
//������Ⱥ
struct CMD_S_CaptureFish
{
	bool                bCanSuperPao;               //������
	BYTE				byStation;					//�����λ
	int					iFishID;					//��Ⱥ��ʶ
	int					iCatchCount;				//ץס�������
	int					iCatchFishID[150];			//ץס���ID
	int    				iFishKindScore;				//��Ⱥ����
	LONG				lFishScore;					//��Ⱥ�÷�
	int					iCatchScore[150];			//�����Ӧ��ĵ÷�
	__int64             i64UserScore;			//����ܷ�

	CMD_S_CaptureFish()
	{
		memset(this,0,sizeof(CMD_S_CaptureFish));
	}
};
/*-------------------------------------------------------------------------------------*/
//������Ⱥ�㲥
struct CMD_S_CaptureFishBroadcast
{
	char				szNickName[61];				///<�ǳ�  
	char				szRoomName[61];				///<��������  
	int					iDeskNo;					///<����ID��
	BYTE				byStation;					//�����λ
	int					iFishType;					//�����ʶ
	__int64				i64FishScore;				//��Ⱥ�÷�


	CMD_S_CaptureFishBroadcast()
	{
		memset(this,0,sizeof(CMD_S_CaptureFishBroadcast));
	}
};
/*-------------------------------------------------------------------------------------*/
//ȡ�������ڵ�
struct CMD_S_CanCelSuperPao
{
	bool                bCanSuperPao;               //������
	BYTE				byStation;					//�����λ

	CMD_S_CanCelSuperPao()
	{
		memset(this,0,sizeof(CMD_S_CanCelSuperPao));
	}
};
/*-------------------------------------------------------------------------------------*/


















/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/
#define ASS_VREB_CHECK					179				//�û�����
/**************************����ˡ���>�ͻ�����Ϣ**************************/
#define ASS_S_USER_COME					51				//��ҽ�����Ϣ
#define ASS_BUILD_TRACE					52				//��������Ϣ
#define	ASS_CHANGE_SCENCE				53				//�л�����
#define	ASS_FISH_REG					54				//����

#define	ASS_S_BUY_BULLET				60				//�Ϸ��·ֽ����Ϣ
#define	ASS_S_USER_SHOT					61				//��ҷ����ӵ����
#define	ASS_S_USER_SHOT_ERROR			62				//��ҷ����ӵ�����
#define	ASS_S_ADD_FIRE					63				//��Ҽ���
#define	ASS_S_HIT_FISH					64				//�������
#define	ASS_S_CANCEL_SUPER				65				//ȡ�������ڵ�
#define ASS_S_CATCH_FISH_BROAD			66				//������Ĺ㲥


/**************************�ͻ��ˡ���>�������Ϣ**************************/
#define	ASS_C_BUY_BULLET				80				//�Ϸ��·���Ϣ	
#define	ASS_C_USER_SHOT					81				//��ҷ���
#define	ASS_C_ADD_FIRE					82				//��Ҽ���
#define	ASS_C_HIT_FISH					83				//�������
/********************************************************************************/
#endif
