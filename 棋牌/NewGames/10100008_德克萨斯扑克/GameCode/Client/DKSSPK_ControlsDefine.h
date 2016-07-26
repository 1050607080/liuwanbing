#ifndef _CONTROLID_FOR_LOGIC_2uigamecomm_
#define _CONTROLID_FOR_LOGIC_2uigamecomm_
///���Ǹ��߼��õĿؼ���Ϣ������Ա���԰��Լ���ϰ�߸�����ĺ�
///�ļ�����ʽΪ�����ɵ���Ӧ�Ŀؼ���Ϣ�ļ���+ ��_��+ControlDefines 
///��    +    ID    +  �ؼ�����  + �ؼ�����  + ��Դ·��

/**************************************************************************************************************
*											 �����ǵ¿���˹�˿˰�ť											  *
***************************************************************************************************************/
const int DKSSPK_CONTAINER									= 1000000;					///<�¿���˹�˿�����
const int DZPK_GAME_BEGIN_BUT								= 1000001;					///<��Ϸ��ʼ��ť
const int DZPK_DAI_RU_MONEY_BUT								= 1000002;					///<�����ť
const int DZPK_BET_BUT										= 1000003;					///<��ע��ť
const int DZPK_CALL_BUT										= 1000004;					///<��ע��ť
const int DZPK_RAISE_BUT									= 1000005;					///<��ע��ť
const int DZPK_CHECK_BUT									= 1000006;					///<���ư�ť
const int DZPK_FOLD_BUT										= 1000007;					///<���ư�ť
const int DZPK_ALL_IN_BUT									= 1000008;					///<ȫ�°�ť

const int DZPK_MONEY_BUT									= 1000010;					///<�����ť * 5
const int DZPK_SHOW_CARD_BUT								= 1000015;					///<չʾ�ư�ť	

const int DZPK_JSLIDER_BUT									= 1000020;					///<��������ť
const int DZPK_ADD_MONEY									= 1000021;					///<��ע(+)��ť
const int DZPK_SUB_MONEY									= 1000022;					///<��ע(-)��ť
const int DZPK_JSLIDER_MAP									= 1000023;					///<��������ͼ

const int DZPK_CALL_TUO_GUAN_BUT							= 1000030;					///<��ע�йܰ�ť
const int DZPK_CHECK_TUO_GUAN_BUT							= 1000031;					///<�����йܰ�ť
const int DZPK_CALL_ANY_TUO_GUAN_BUT						= 1000032;					///<���κ�ע�йܰ�ť
const int DZPK_CHECK_FOLD_TUO_GUAN_BUT						= 1000033;					///<���ơ������йܰ�ť

const int DZPK_GOU_QU_CALL_TUO_GUAN_BUT						= 1000040;					///<�򹳸�ע�йܰ�ť
const int DZPK_GOU_QU_CHECK_TUO_GUAN_BUT					= 1000041;					///<�򹳹����йܰ�ť
const int DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT					= 1000042;					///<�򹳸��κ�ע�йܰ�ť
const int DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT				= 1000043;					///<�򹳹��ơ������йܰ�ť

/**************************************************************************************************************
*											�����ǵ¿���˹�˿�LOGO											  *
***************************************************************************************************************/
const int DZPK_USER_LOGO_CONTAINER							= 1000050;					///<�û�ͷ������
const int DZPK_USER_LOGO									= 1000060;					///<�û�ͷ�� * 8

const int DZPK_USER_NAME_CONTAINER							= 1000070;					///<�û��ǳ�����
const int DZPK_USER_MAP										= 1000080;					///<�û���Ϣ��ͼ * 8
const int DZPK_USER_NAME									= 1000090;					///<�û��ǳ� * 8
const int DZPK_USER_MONEY									= 1000100;					///<�û���� * 8

const int DZPK_CARD_CONTAINER								= 1000110;					///<��ʾ��������
const int DZPK_SHOW_CARD									= 1000120;					///<��ʾ���� * 8

const int DZPK_SIDE_POT_CONTAINER							= 1000130;					///<��ʾ���½Ǳ߳�����
const int DZPK_SHOW_POT_DINAR								= 1000140;					///<��ʾ���½Ǳ߳ؽ�� * 8
const int DZPK_SHOW_SIDE_POT								= 1000150;					///<��ʾ���½Ǳ߳� * 8

const int DZPK_DESKTOP_MONEY_CONTAINER						= 1000160;					///<����������
const int DZPK_SHOW_DESKTOP_MONEY_LOGO						= 1000170;					///<������LOGO * 8
const int DZPK_SHOW_DESKTOP_MONEY							= 1000180;					///<������ * 8


const int DZPK_BET_CALL_RAISE_CHECK_FOLD_ALLIN_CONTAINER	= 1000190;					///<�¸��ӹ���ȫ������					
const int DZPK_SHOW_BET_LOGO								= 1000200;					///<��עLogo��ʾ * 8
const int DZPK_SHOW_CALL_LOGO								= 1000210;					///<��עLogo��ʾ * 8
const int DZPK_SHOW_RAISE_LOGO								= 1000220;					///<��עLogo��ʾ * 8
const int DZPK_SHOW_CHECK_LOGO								= 1000230;					///<����Logo��ʾ * 8
const int DZPK_SHOW_FOLD_LOGO								= 1000240;					///<����Logo��ʾ * 8
const int DZPK_SHOW_ALLIN_LOGO								= 1000250;					///<ȫ��Logo��ʾ * 8

const int DZPK_POT_CONTAINER								= 1000260;					///<����߳�����
const int DZPK_SHOW_BALANCE_POT_LOGO						= 1000270;					///<����߳ؽ�����ʾ * 8
const int DZPK_SHOW_BALANCE_POT_MONEY						= 1000280;					///<����߳ؽ�����ʾ��� * 8

const int DZPK_CHAT_CONTAINER								= 1000290;					///<��������
const int DZPK_SHOW_CHAT_BUBBLE								= 1000300;					///<����������ʾ * 8
const int DZPK_SHOW_CHAT_TYPE								= 1000310;					///<��������������ʾ * 8

const int DZPK_NT_DA_XIAO_CONTAINER							= 1000320;					///<ׯ��Сäע����
const int DZPK_SHOW_NTUSER_IMAGE							= 1000330;					///<��ʾׯ�� * 8
const int DZPK_SHOW_XIAO_IMAGE								= 1000340;					///<Сäע��ʾ * 8

const int DZPK_SHOW_CHAIR_CONTAINER							= 1000350;					///<�����������
const int DZPK_SHOW_USER_CHAIR								= 1000360;					///<������� * 8

const int DZPK_ZHUN_BEI_CONTAINER							= 1000370;					///<���׼������
const int DZPK_READY_MAP									= 1000380;					///<���׼��ͼƬ

const int DZPK_SHOW_GAME_HELP_CONTAINER						= 1000400;					///<���ֽ̳�����
const int DZPK_SHOW_GAME_HELP								= 1000410;					///<���ֽ̳� * 9

const int DZPK_SHOW_TOKEN_CONTAINER							= 1000500;					///<�����������
const int DZPK_SHOW_TOKEN									= 1000510;					///<������� * 8

const int DZPK_SHOW_USER_TIMER_CONTAINER					= 1000520;					///<���ʱ������
const int DZPK_SHOW_USER_TIMER								= 1000530;					///<���ʱ�� * 8

const int DZPK_SHOW_DA_IMAGE								= 1000540;					///<��äע��ʾ * 8	

const int DZPK_SHOW_SMALL_CARD_CONTAINER					= 1000550;					///<С����ʾ����
const int DZPK_SHOW_SMALL_CARD								= 1000560;					///<С����ʾ * 8

const int DZPK_V_PAI_CONTAINER								= 1000570;					///<V������
const int DZPK_V1_PAI										= 1000580;					///<V1�� * 8
const int DZPK_V2_PAI										= 1000590;					///<V2�� * 8

const int DZPK_PAI_TYPE_ANIMATION_CONTAINER					= 1000600;					///<���Ͷ�������
const int DZPK_PAI_TYPE_ANIMATION							= 1000610;					///<���Ͷ��� * 8

const int DZPK_PAI_TYPE_MAP_CONTAINER						= 1000620;					///<����ͼƬ����
const int DZPK_PAI_TYPE_MAP									= 1000630;					///<����ͼƬ * 8

const int DZPK_SHOW_REAL_MONEY								= 1001000;					///<��ʵ�����ʾ
const int DZPK_SHOW_REAL_DINAR								= 1001001;					///<��ʵ�����ʾ
const int DZPK_SHOW_USER_THIS_ID							= 1001002;					///<�������ID
const int DZPK_SHOW_ADDRESS									= 1001003;					///<��ַIP��ʾ
const int DZPK_SHOW_BASE_CARD								= 1001004;					///<���湫����
const int DZPK_SHOW_ZHUANG_LOGO								= 1001005;					///<ׯ
const int DZPK_SHOW_SMALL_BLIND								= 1001006;					///<Сäע
const int DZPK_SHOW_BIG_BLIND								= 1001007;					///<��äע
const int DZPK_SHOW_VICE_CARD								= 1001008;					///<һ����

const int DZPK_SHOW_KE_YONG_MONEY							= 1001009;					///<��ǰ���ý��
const int DZPK_SHOW_READY_GO								= 1001010;					///<��׼��Logo

const int DZPK_SHOW_PROMPT									= 1001011;					///<��ʾ�������
const int DZPK_SHOW_CLCKTIME								= 1001012;					///<����ʱʱ��
const int DZPK_SHOW_MONEYLACK								= 1001013;					///<��ұ������
const int DZPK_SHOW_IN_TIME									= 1001014;					///<���붨ʱ��

const int DZPK_SHOW_TIMER_MAP								= 1001015;					///<�����ע��ʱ��ͼ

const int DZPK_NT_MOVE										= 1001020;					///<ׯ�ƶ��ؼ�
const int DZPK_SMALL_BLIND_MOVE								= 1001030;					///<Сäע�ƶ��ؼ�
const int DZPK_BIG_BLIND_MOVE								= 1001040;					///<��äע�ƶ��ؼ�

const int DZPK_HIDE_NT_BEGIN								= 1001050;					///<ׯ���ؿ�ʼ�ؼ�
const int DZPK_HIDE_SMALL_BLIND_BEGIN						= 1001051;					///<Сäע���ؿ�ʼ�ؼ�
const int DZPK_HIDE_BIG_BLIND_BEGIN							= 1001052;					///<��äע���ؿ�ʼ�ؼ�

const int DZPK_FA_PAI_ANIMATION								= 1001060;					///<���ƶ����ؼ�
const int DZPK_VICE_CARD_BEGIN								= 1001061;					///<һ���ƿ�ʼ�ؼ�


const int DZPK_SHOW_BET_MONEY_CONTAINER						= 1001069;					///<��ע�������
const int DZPK_SHOW_BET_MONEY								= 1001070;					///<��ע���
const int DZPK_SHOW_CALL_MONEY								= 1001071;					///<��ע���
const int DZPK_SHOW_RAISE_MONEY								= 1001072;					///<��ע���
const int DZPK_SHOW_FIVE_MONEY								= 1001080;					///<������ * 5

const int DZPK_CHIP_TOTAL_MAP								= 1001090;					///<������ͼƬ
const int DZPK_MONEY_COLLECT								= 1001091;					///<����ռ���

const int DZPK_IN_MONEY_BU_ZU								= 1001092;					///<�������ͼƬ
const int DZPK_SELF_MONEY_BU_ZU								= 1001093;					///<�������ͼƬ


/**************************************************************************************************************
*										�����ǵ¿���˹�˿˴���������										  *
***************************************************************************************************************/
const int DZPK_DAI_RU_MONEY_CONTAINER						= 1001100;					///<����������
const int DZPK_OK_LOAD_BUT									= 1001110;					///<ȷ�����밴ť
const int DZPK_EXIT_GAME_BUT								= 1001111;					///<�˳���Ϸ��ť
const int DZPK_ADD_BUT										= 1001112;					///<����Ӱ�ť
const int DZPK_SUB_BUT										= 1001113;					///<�������ť
const int DZPK_OFF_BUT										= 1001114;					///<�رմ���������
const int DZPK_TOTAL_MONEY_TEXT								= 1001115;					///<�����ܽ����
const int DZPK_MAX_MONEY_TEXT								= 1001116;					///<�����������
const int DZPK_MIN_MONEY_TEXT								= 1001117;					///<������С�����
const int DZPK_PLAN_MONEY_INPUT_TEXT						= 1001118;					///<������ٽ����

/**************************************************************************************************************
*											�����ǵ¿���˹�˿˽�������										  *
***************************************************************************************************************/
const int DZPK_BALANCE_CONTAINER							= 1001500;					///<��������
const int DZPK_BALANCE_TOP									= 1001501;					///<����򶥲�
const int DZPK_BALANCE_ZHONG								= 1001510;					///<������в�
const int DZPK_BALANCE_BASE									= 1001520;					///<�����ײ�
const int DZPK_WIN_USER_ID									= 1001530;					///<Ӯ��ID
const int DZPK_GONG_WIN_MONEY								= 1001540;					///<��Ӯ�ö��ٽ��
const int DZPK_LOSE_WIN_INFO								= 1001550;					///<��Ӯ����
const int DZPK_OFF_BALANCE_CONTAINER						= 1001560;					///<�رս�������
/**************************************************************************************************************
*											�����ǵ¿���˹�˿���������										  *
***************************************************************************************************************/
const int DZPK_SET_CONTAINER								= 1001600;					///<��������
const int DZPK_MUSIC_SELECT 								= 1001800;					///<������ѡ��
const int DZPK_KE_WATCH										= 1001801;					///<���Թ۹�ѡ��
const int DZPK_MUSIC_TEXT									= 1001603;					///<������Ϣ
const int DZPK_WATCH_TEXT									= 1001604;					///<���Թ���Ϣ
const int DZPK_SET_OK_BUT									= 1001605;					///<����ȷ����ť
const int DZPK_SET_CANCEL_BUT								= 1001606;					///<����ȡ����ť

/**************************************************************************************************************
*											�����ǵ¿���˹�˿˶�������										  *
***************************************************************************************************************/

const int DZPK_CHIP_RECYCLE_MOVE_CONTAINER					= 1002000;					///<������հ䷢�ƶ�����
const int DZPK_CHIP_RECYCLE_MOVE							= 1002010;					///<������հ䷢�ƶ��ؼ�

const int DZPK_MONEY_ISSUE_MOVE_CONTAINER					= 1002020;					///<�����հ䷢�ƶ�����
const int DZPK_MONEY_ISSUE_MOVE								= 1002030;					///<�����հ䷢�ƶ��ؼ�

const int DZPK_CHIP_MONEY_MAP								= 1002040;					///<�������շ�����ͼƬ

const int DZPK_CHIP_MONEY_CONTAINER							= 1002050;					///<������䷢����
const int DZPK_CHIP_MAP										= 1002060;					///<����䷢ͼƬ
const int DZPK_MONEY_NUM									= 1002070;					///<���䷢����

const int DZPK_FOLD_LOGO_CONTAINER							= 1002080;					///<��ҷ���ͷ��ͼƬ����
const int DZPK_FOLD_LOGO									= 1002090;					///<��ҷ���ͷ��ͼƬ * 8

const int DZPK_WIN_DONG_HUA_CONTAINER						= 1002100;					///<ʤ����������
const int DZPK_WIN_DONG_HUA									= 1002110;					///<ʤ������ * 8

const int DZPK_SET_BUT										= 1003000;					///<���ð�ť

const int DZPK_MOVE_COLLECT_CONTAINER						= 1003010;					///<ׯ��Сäע�ƶ��ռ�����
const int DZPK_NT_MOVE_COLLECT								= 1003020;					///<ׯ�ƶ��ռ�
const int DZPK_XIAO_MOVE_COLLECT							= 1003030;					///<Сäע�ƶ��ռ�
const int DZPK_DA_MOVE_COLLECT								= 1003040;					///<��äע�ƶ��ռ�

const int DZPK_NT_DA_XIAO_MOVE_CONTAINER					= 1003050;					///<ׯ��Сä�ƶ�ͼƬ����
const int DZPK_NT_MOVE_MAP									= 1003060;					///<ׯä�ƶ�ͼƬ
const int DZPK_DA_XIAO_MOVE_MAP								= 1003070;					///<��Сä�ƶ�ͼƬ

const int DZPK_TIME_NUM										= 1003115;					///<��ʼ������ʾʱ��

/**************************************************************************************************************
*											�����ǵ¿���˹�˿��ƻ�����										  *
***************************************************************************************************************/
const int DZPK_LOOK_CARD_ANIMA_0							= 1004000;					///<0��ͼλ�ÿ��ƶ���
const int DZPK_LOOK_CARD_ANIMA_1							= 1004001;					///<1��ͼλ�ÿ��ƶ���
const int DZPK_LOOK_CARD_ANIMA_2							= 1004002;					///<2��ͼλ�ÿ��ƶ���
const int DZPK_LOOK_CARD_ANIMA_3							= 1004003;					///<3��ͼλ�ÿ��ƶ���
const int DZPK_LOOK_CARD_ANIMA_5							= 1004005;					///<5��ͼλ�ÿ��ƶ���
const int DZPK_LOOK_CARD_ANIMA_6							= 1004006;					///<6��ͼλ�ÿ��ƶ���
const int DZPK_LOOK_CARD_ANIMA_7							= 1004007;					///<7��ͼλ�ÿ��ƶ���

#endif 
