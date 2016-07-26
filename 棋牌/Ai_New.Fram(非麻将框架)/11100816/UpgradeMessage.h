/**
* ��Ϣ������ UpgradeMessage.h   
*
* @author duanxiaohui
* @version 1.0
* @date 2009.12.15
*/

#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

#include "GameMessage.h"

/// �ļ����ֶ���
#define GAMENAME						TEXT("���ݶ��ţ�16�ˣ�")

#ifdef VIDEO
#define NAME_ID							11901600					    /** ���� ID */
#else
#define NAME_ID							11901600						/** ���� ID */
#endif

#define PLAY_COUNT						16								/** ��Ϸ���� */

/// �汾����
#define GAME_MAX_VER					1								/** ������߰汾 */
#define GAME_LESS_VER					1								/** ������Ͱ汾 */
#define GAME_CHANGE_VER					0								/** �޸İ汾 */

/// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

/// ��Ϸ��Ϣ

/// ��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				13				/** �ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1) */
#define DEV_LOW_VERSION					1				/** �ڲ������Ͱ汾�� */

#define SKIN_CARD						TEXT("cardskin") /** ����ͼƬ�ļ��� */

static TCHAR szTempStr[MAX_PATH] =		{0};			/** �ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ�� */
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_SKIN_FOLDER(A)				(sprintf(A,"%d",NAME_ID),strcat(A,""))

/// ��Ϸ״̬����
#define GS_WAIT_SETGAME					0				/** �ȴ���������״̬ */
#define GS_WAIT_ARGEE					1				/** �ȴ�ͬ������ */

#define GS_XIAZHU                       21              /** ��ע״̬ */
#define GS_PLAY_GAME					22				/** ��Ϸ��״̬ */
#define  GS_SEND_CARD                   23              ///����
#define GS_WAIT_NEXT					25				/** �ȴ���һ�̿�ʼ  */

/// ��ע����
#define TYPE_GIVE_UP					0x00			/** ���� */
#define TYPE_NOTE						0x01			/** ��ע */
#define TYPE_ADD						0x02			/** ��ע */
#define TYPE_FOLLOW						0x03			/** ��ע */
#define TYPE_SHUTTLE					0x04			/** ��� */
													
/// ��������ע�����嶨��
#define GF_SHUNMEN_HNEG                 0          /** �� */
#define GF_DAOMEN_HENG					1			/** �� */
#define GF_SHUNMEN						2			/** ˳�� */
#define GF_DAOMEN			            3			/** ���� */
#define GF_SHUNMEN_JIAO					4		    /** ˳��-�� */
#define GF_DUIMEN						5			/** ���� */

#define GF_DAOMEN_JIAO					6			    /** ����-�� */

// ��Ϸ���Ͱ汾����
#define GAME_VERSION_1                  0               /** ��ͨ�汾 */
#define GAME_VERSION_2                  1               /** �����Ӱ汾 */
#define GAME_VERSION_3                  2               /** ���«2���汾 */

#define  REGION_COUNT                   7              ///��ǰ����ע������

/// ���������Ϸ��Ϣ�ṹ
struct UserSetStruct
{
	bool				bFirstFollow;
	bool				bAllFollow;
	BYTE				bDeskStation;
	BYTE				bReserve;
};
///��ҿ�ʼ��ׯ
struct GameJiaoZhaungStruct
{
	BYTE  bDeskStation ; ///֪ͨ����ҽ�ׯ	
};
/// ��Ϸ��ʼ��Ϣ�ṹ
struct GameBeginStruct
{
	int                 bZhuangStation;                 /**< ׯ�� */
	int 				iBeenPlayGame;					/**< ��Ϸ�غ� */
	__int64             iZhuangResult ; 

	int					iBaseNote;						/**< ��ע */	
	int					iLimitNote;						/**< ���ע�� */

	BOOL                bNTGameMode;                    /**< ��Ϸׯģʽ: TRUE:��ׯģʽ  FALSE:��ׯģʽ */
	int                 iZhuangMaxCount;                /**< ׯ���������پ� */
	char                chGameCount;                    /**< ��Ϸ���� */
	char                chGameBeen;                     /**< ��Ϸ���� */
	int                 iSelfMoney;                     /**< ˢ�±�����, ��ֹ�ͻ��˽�Ҳ�һ�� */		
};

/// ��Ϸ��ǰ��������Ϣ�ṹ
struct GameAheadStruct
{
	BYTE				bDeskStation;					/**< ʤ�� */
	int					iTurePoint[PLAY_COUNT];			/**< ׯ�ҵ÷� */
	int					iChangeMoney[PLAY_COUNT];		/**< ѩ�� */
};

/// �û�������Ϣ��Ϣ�ṹ
struct tagUserProcess
{
	BYTE				iVrebType;					    /**< ������İ�ť */
	int					iNote;						    /**< ��ע�� */
};

/// �������й���Ϣ�ṹ
struct UseAIStation
{
	BYTE				bDeskStation;				    /**< ���� */
	BOOL				bState;						    /**< ״̬ */
};

/// ������Ϣ�ṹ
struct GameFinishNotify
{
	__int64             iSelfMoeny;                     /**< �����Լ��÷� */
	__int64             iReturnMoney;                   /**< ���ַ��ط� */
	__int64             iZhuangMoeny;                   /**< ����ׯ�ҵ÷� */
};

/// ��Ϸ״̬���ݰ���Ϣ�ṹ
struct GameStation_1
{
	/// ��Ϸ�汾
	BYTE				iVersion;						/**< ��Ϸ�汾�� */
	BYTE				iVersion2;						/**< ��Ϸ�汾�� */

	int					iRoomBasePoint;					/**< ���䱶�� */
	int					iRunPublish;					/**< ���ܿ۷� */

	/// ��Ϸ��Ϣ
	BYTE				iThinkTime;						/**< ��Ϸ���� */
	BYTE				iBeginTime;						/**< ��Ϸ���� */
	BYTE				iAllCardCount;					/**< �˿���Ŀ */
	int					iLimitNote;						/**< ���ע */
	int					iBaseNote;						/**< ��ע */
	__int64             iMaxNote;                       /**< �м������ע�� */
	__int64     		iMaxBaseNote;					//��ͨ��ҵ������ע

	int                 iXiaZhuTime;                    /**< ��עʱ�� */
	int                 iKaipaiTime;                    /**< ����ʱ�� */
	int                 iEmptyTime;                     /**< ����ʱ�� */

	BOOL                bUseCard;                       /**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */
	BYTE                bGameMode;                      /**< ��Ϸģʽ �汾 0: ��ͨ 1: ������ 2: ���«2��*/
	BOOL				bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
	BOOL                bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
	BOOL                bIsVipDouble;                   /**< �Ƿ�Ϊ˫����ע����*/	
};

/// ��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ����Ϣ�ṹ
struct GameStation_2
{
	/// ��Ϸ�汾
	BYTE				iVersion;						/**< ��Ϸ�汾�� */
	BYTE				iVersion2;						/**< ��Ϸ�汾�� */
	int					iRoomBasePoint;					/**< ���䱶�� */
	int					iRunPublish;					/**< ���ܿ۷� */

	/// ��Ϸ��Ϣ
	BYTE				iThinkTime;						/**< ��Ϸ���� */
	BYTE				iBeginTime;						/**< ��Ϸ���� */
	BYTE				iAllCardCount;					/**< �˿���Ŀ */
	int					iLimitNote;						/**< ���ע */
	int					iBaseNote;						/**< ��ע */
	__int64             iMaxNote;                       /**< �м������ע�� */
	__int64     		iMaxBaseNote;					//��ͨ��ҵ������ע
	
	int                 iXiaZhuTime;                    /**< ��עʱ�� */
	int                 iKaipaiTime;                    /**< ����ʱ�� */
	int                 iEmptyTime;                     /**< ����ʱ�� */

	BOOL                bIsApplyXZ;                     /**< ׯ���Ƿ���������ׯ */

    int                 iKaiPaiCount;                   /**< ���Ƽ�¼���� */
	BYTE                bIsKaiPai[GAME_XIPAI_COUNT];    /**< �����Ƿ�(0x01:˳�š�0x02:���š�0x04:����) */
	BYTE                bIsXiaZhu[GAME_XIPAI_COUNT];    /**< �����Ƿ���ע(0x01:˳�š�0x02:���š�0x04:����) */

	BYTE                userList[PLAY_COUNT];           /**< ��ׯ�б����� */


	BOOL                bUseCard;                       /**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */
	BYTE                bGameMode;                      /**< ��Ϸģʽ �汾 0: ��ͨ 1: ������ 2: ���«2��*/	
	BOOL				bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/	
	BOOL                bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
	BOOL                bIsVipDouble;                   /**< �Ƿ�Ϊ˫����ע����*/

	BYTE                bNTStation;                      ///��ǰ��ׯ��λ��
};

//��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ����Ϣ�ṹ
struct GameStation_3
{
	/// ��Ϸ�汾
	BYTE				iVersion;						/**< ��Ϸ�汾�� */
	BYTE				iVersion2;						/**< ��Ϸ�汾�� */
	int					iRoomBasePoint;					/**< ���䱶�� */
	int					iRunPublish;					/**< ���ܿ۷� */

	/// ��Ϸ��Ϣ
	BYTE				iThinkTime;						/**< ��Ϸ���� */
	BYTE				iBeginTime;						/**< ��Ϸ���� */
	BYTE				iAllCardCount;					/**< �˿���Ŀ */
	int					iLimitNote;						/**< ���ע */
	int					iBaseNote;						/**< ��ע */
	__int64             iMaxNote;                       /**< �м������ע�� */
	__int64     		iMaxBaseNote;					//��ͨ��ҵ������ע

	int                 iXiaZhuTime;                    /**< ��עʱ�� */
	int                 iKaipaiTime;                    /**< ����ʱ�� */
	int                 iEmptyTime;                     /**< ����ʱ�� */

	int                 iTime;                          /**< ��ǰʣ��ʱ�� */

	/// ����״̬����
	int					iUpGradePeople;					/**< ׯ��λ�� */
	BOOL                bIsApplyXZ;                     /**< ׯ���Ƿ���������ׯ */
	BOOL                bIsInNoteStage ;                ///�Ƿ��ǳ�����ע�׶�
	BYTE				iBeenPlayGame;					/**< �Ѿ���Ϸ�ľ��� */
	__int64             iZhuangMoenyResult;             /**< ׯ����Ӯ��� */
	__int64             iSelfMoenyResult;               /**< �Լ���Ӯ��� */
	__int64             iXiaZhuInfo[7];                 /**< �����ϵ���ע����  0˳�ţ�1˳��-�ǣ�2���ţ�3�ţ�4���ţ�5����-�� */
	__int64             iMyZhu[7];                      /**< ������ע */
	__int64             iRobotZhu[7];                   /**< ������ÿ�������µ�ע�� */

	BYTE                userList[PLAY_COUNT];           /**< ��ׯ�б����� */

	int                 iKaiPaiCount;                   /**< ���Ƽ�¼���� */
	BYTE                bIsKaiPai[GAME_XIPAI_COUNT];    /**< �����Ƿ�(0x01:˳�š�0x02:���š�0x04:����) */
	BYTE                bIsXiaZhu[GAME_XIPAI_COUNT];    /**< �����Ƿ���ע(0x01:˳�š�0x02:���š�0x04:����) */

	BOOL                bUseCard;                       /**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */
	BYTE                bGameMode;                      /**< ��Ϸģʽ �汾 0: ��ͨ 1: ������ 2: ���«2��*/
	BOOL				bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
	BOOL                bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
	BOOL                bIsVipDouble;                   /**< �Ƿ�Ϊ˫����ע����*/	
	char                chGameCount;                    /**< ��Ϸ���� */
	char                chGameBeen;                     /**< ��Ϸ���� */

	int                 iLuziCount;                     /**< ·������ */
	GameLuziInfo        ArrLuzi[GAME_XIPAI_COUNT];      /**< ·������ */	
};

/// ��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ����Ϣ�ṹ
struct GameStation_4	
{
	/// ��Ϸ�汾
	BYTE				iVersion;						/**< ��Ϸ�汾�� */
	BYTE				iVersion2;						/**< ��Ϸ�汾�� */

	/// ��Ϸ��Ϣ
	BYTE				iThinkTime;						/**< ��Ϸ���� */
	BYTE				iBeginTime;						/**< ��Ϸ���� */
	BYTE				iAllCardCount;				    /**< �˿���Ŀ */
	
	int					iRoomBasePoint;					/**< ���䱶�� */
	int					iRunPublish;					/**< ���ܿ۷� */
	int					iLimitNote;						/**< ���ע */
	int					iBaseNote;						/**< ��ע */
	__int64             iMaxNote;                       /**< �м������ע�� */
	__int64     		iMaxBaseNote;					//��ͨ��ҵ������ע

	int                 iXiaZhuTime;                    /**< ��עʱ�� */
	int                 iKaipaiTime;                    /**< ����ʱ�� */
	int                 iEmptyTime;                     /**< ����ʱ�� */

	int                 iTime;                          /**< ��ǰʣ��ʱ�� */

	/// ����״̬����
	int					iUpGradePeople;					/**< ׯ��λ�� */
	BOOL                bIsApplyXZ;                     /**< ׯ���Ƿ���������ׯ */
	BYTE				iBeenPlayGame;					/**< �Ѿ���Ϸ�ľ��� */
	__int64             iZhuangMoenyResult;             /**< ׯ����Ӯ��� */
	__int64             iSelfMoenyResult;               /**< �Լ���Ӯ��� */
	__int64             iXiaZhuInfo[7];                 /**< �����ϵ���ע����  0˳�ţ�1˳��-�ǣ�2���ţ�3�ţ�4���ţ�5����-�� */
	__int64             iMyZhu[7];                      /**< ������ע */
	__int64             iRobotZhu[7];                   /**< ������ÿ�������µ�ע�� */

	
	PaiData             cardData[4];                    /**< ������ */
	BYTE                userList[PLAY_COUNT];           /**< ��ׯ�б����� */

	int                 iKaiPaiCount;                   /**< ���Ƽ�¼���� */
	BYTE                bIsKaiPai[GAME_XIPAI_COUNT];    /**< �����Ƿ�(0x01:˳�š�0x02:���š�0x04:����) */
	BYTE                bIsXiaZhu[GAME_XIPAI_COUNT];    /**< �����Ƿ���ע(0x01:˳�š�0x02:���š�0x04:����) */

	BOOL                bUseCard;                       /**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */
	BYTE                bGameMode;                      /**< ��Ϸģʽ �汾 0: ��ͨ 1: ������ 2: ���«2��*/
	BOOL				bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
	BOOL                bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
	BOOL                bIsVipDouble;                   /**< �Ƿ�Ϊ˫����ע����*/	
	char                chGameCount;                    /**< ��Ϸ���� */
	char                chGameBeen;                     /**< ��Ϸ���� */

	int                 iLuziCount;                     /**< ·������ */
	GameLuziInfo        ArrLuzi[GAME_XIPAI_COUNT];      /**< ·������ */	
};

/// ��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ����Ϣ�ṹ
struct GameStation_5
{
	/// ��Ϸ�汾
	BYTE				iVersion;						/**< ��Ϸ�汾�� */
	BYTE				iVersion2;						/**< ��Ϸ�汾�� */
	int					iRoomBasePoint;					/**< ���䱶�� */
	int					iRunPublish;					/**< ���ܿ۷� */

	BYTE				iThinkTime;						/**< ��Ϸ���� */
	BYTE				iBeginTime;						/**< ��Ϸ���� */

	int					iLimitNote;						/**< ���ע */
	int					iBaseNote;						/**< ��ע */
	__int64             iMaxNote;                       /**< �м������ע�� */
	__int64     		iMaxBaseNote;					//��ͨ��ҵ������ע

	int                 iXiaZhuTime;                    /**< ��עʱ�� */
	int                 iKaipaiTime;                    /**< ����ʱ�� */
	int                 iEmptyTime;                     /**< ����ʱ�� */

	int                 iTime;                          /**< ��ǰʣ��ʱ�� */

	/// ����״̬����
	int					iUpGradePeople;					/**< ׯ��λ�� */
	BOOL                bIsApplyXZ;                     /**< ׯ���Ƿ���������ׯ */
	BYTE				iBeenPlayGame;					/**< �Ѿ���Ϸ�ľ��� */
	__int64             iZhuangMoenyResult;             /**< ׯ����Ӯ��� */
	__int64             iSelfMoenyResult;               /**< �Լ���Ӯ��� */

	BYTE                userList[PLAY_COUNT];           /**< ��ׯ�б����� */

	int                 iKaiPaiCount;                   /**< ���Ƽ�¼���� */
	BYTE                bIsKaiPai[GAME_XIPAI_COUNT];    /**< �����Ƿ�(0x01:˳�š�0x02:���š�0x04:����) */
	BYTE                bIsXiaZhu[GAME_XIPAI_COUNT];    /**< �����Ƿ���ע(0x01:˳�š�0x02:���š�0x04:����) */

	BOOL                bUseCard;                       /**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */
	BYTE                bGameMode;                      /**< ��Ϸģʽ �汾 0: ��ͨ 1: ������ 2: ���«2��*/
	BOOL				bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
	BOOL                bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
	BOOL                bIsVipDouble;                   /**< �Ƿ�Ϊ˫����ע����*/	
	char                chGameCount;                    /**< ��Ϸ���� */
	char                chGameBeen;                     /**< ��Ϸ���� */

	int                 iLuziCount;                     /**< ·������ */
	GameLuziInfo        ArrLuzi[GAME_XIPAI_COUNT];      /**< ·������ */	
};

/// ����ׯ����Ӯ���ݰ���Ϣ�ṹ
typedef struct tagSuperSetting
{
	char bState[3];         /**< 0: ����˳�� 1: ������� 2: ������ (TRUE: ѡ�� FALSE: δѡ��) */

}SUPERSETTING;

/// ֪ͨ�ͻ����Ƿ�Ϊ�����ͻ���״̬��Ϣ�ṹ
typedef struct tagSuperState
{
	BYTE bDeskStation;      /**< ���λ�� */
	BOOL bEnable;           /**< �Ƿ�ͨ */
}SUPERSTATE;

/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					49				/** �û�������Ϸ */
#define ASS_UG_USER_SET_CHANGE			50				/** ������ø��� */

                
#define ASS_USER_JIAO_ZHUANG            51              ///��ҽ�ׯ
#define ASS_GAME_BEGIN					52				/** ������Ϸ��ʼ */

#define ASS_SEND_CARD					53				/** ������Ϣ */
#define ASS_SEND_FINISH					54				/** ������� */
#define ASS_GAME_PLAY					55				/** ��ʼ��Ϸ */
#define ASS_NOTE_LIMIE					56				/** ������ע */

#define  ASS_SEND_NOTE_MSG              57               ///֪ͨϴ��
#define ASS_APPLY_NT                    58             /** ���������ׯ*/

#define ASS_NOTE						60				/** ��ע */
#define ASS_VREB_CHECK					61				/** �û����� */
#define ASS_REPLACE_VREB_CHECK			62				/** ����(����)�û����� */
#define ASS_NOTE_RESULT					63				/** ��ע��� */

#define ASS_COLLECT_JETON				70				/** �ռ����� */
#define ASS_AI_STATION					71				/** �������й� */

#define ASS_NEW_TURN					80				/** ��һ�ֿ�ʼ */
#define ASS_SEND_A_CARD					81				/** �·�һ���� */

#define ASS_CONTINUE_END				90				/** ��Ϸ���� */
#define ASS_NO_CONTINUE_END				91				/** ��Ϸ���� */
#define ASS_CUT_END						92				/** �û�ǿ���뿪 */
#define ASS_SALE_END					93				/** ��Ϸ��ȫ���� */
#define ASS_NO_WINER					94				/** ��Ϸ������ʤ���� */
#define ASS_AHEAD_END					95				/** ��ǰ���� */

#define ASS_SUPER_SETTING               150             /** �ɿͻ��˷�����Ϣ���Ʊ�����Ӯ */
#define ASS_SUPER_SETTING_SUCCEED       151             /** �ɷ���˷��Ϳͻ��˳������óɹ���Ϣ */
#define ASS_SUPER_STATE                 152            /** ֪ͨ�ͻ����Ƿ�Ϊ����״̬ */
#define ASS_NOT_START_GAME              153            /**	��ׯ����ʼ��Ϸ */

/********************************************************************************/
#endif
