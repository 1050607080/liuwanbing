#pragma once

#include "mj.h"
#include <bitset>
using namespace std;

///������汾
#define DEV_LIB_VER				1									///������汾
//֧������
///֧�����Ͷ���
#define SUP_NORMAL_GAME			0x01								///��ͨ��Ϸ
#define SUP_MATCH_GAME			0x02								///������Ϸ
#define SUP_MONEY_GAME			0x04								///�����Ϸ
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

////////////////////////////////////////////////////////////////////////////////
#define GF_NORMAL               10                                  /// ��Ϸ��������
#define GF_SAFE                 11                                  /// ��Ϸ��ȫ����

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE		// ��֪ʲô��˼
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AI_FAST								// �����˿��ٳ���
#ifndef AI_MACHINE						// ȡ�����ٻ�����
#undef AI_FAST
#endif
// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					TEXT("MatchTable_BSMJ")
// ��Ϸ״̬����
#define GS_WAIT_SETGAME					0				// �ȴ���������״̬
#define GS_WAIT_ARGEE					1				// �ȴ�ͬ������
#define GS_SEND_CARD					20				// ����״̬
#define GS_PLAY_GAME					22				// ��Ϸ��״̬
#define GS_WAIT_NEXT					23				// �ȴ���һ�̿�ʼ 
// ץ��״̬
#define GAME_STATUS_FAPAI				0				// ��Ϸ״̬Ϊ0ʱ,��ʾ�ǲ���
#define GAME_STATUS_CHUPAI				1				// Ϊ1,��ʾ����������

//////�������ͼ���������////////////////////
/********************************************************************************/
#define ACTION_NO				0				//û�ж���
#define ACTION_CHI				1				//���ƶ���
#define ACTION_PENG				2				//���ƶ���
#define ACTION_KAN				3				//�����ƶ���
#define ACTION_AN_GANG			4				//���ܶ���
#define ACTION_BU_GANG			5				//���ܶ���
#define ACTION_MING_GANG		6				//���ܶ���
#define ACTION_CHI_TNG			7				//���ԣ��Ժ����ƣ�
#define ACTION_TING				8				//������
#define ACTION_HU				10				//������

#define TYPE_JIANG_PAI			15				//��������
#define TYPE_JINGDIAO_JIANG		16				//������������
#define TYPE_SHUN_ZI			17				//˳������
#define TYPE_AN_KE				18				//��������
#define TYPE_JING_KE			19				//�����������

///�����Ƶ�����
#define HAND_CARD_NUM			17				//�����������
#define OUT_CARD_NUM			60				//��������
#define HUA_CARD_NUM			20				//��������
#define MEN_CARD_NUM			40				//��������
#define MAX_HUPAI_TYPE			20				//��������
#define MAX_HAND_PAI			14				//������

///���Ƽ����ʱ����
struct CheckHuStruct
{
	BYTE  data[HAND_CARD_NUM][2];   //�����ݣ�0��ֵ��1����
	BYTE  conut;
	void Add(BYTE pai)
	{
		if(GetPaiCount(pai)>0)
		{
			for(int i=0;i<HAND_CARD_NUM;i++)
			{
				if(data[i][0] == pai && pai != 255)
					data[i][1]++;
			}
		}
		else
		{
			data[conut][0] = pai;
			data[conut][1] = 1;
			conut++;
		}
	};
	BYTE GetPaiCount(BYTE pai)
	{
		for(int i=0;i<HAND_CARD_NUM;i++)
		{
			if(data[i][0] == pai && pai != 255 && data[i][1] != 255)
				return data[i][1];
		}
		return 0;
	};
	BYTE GetDataCount()
	{
		conut = 0;
		for(int i=0;i<HAND_CARD_NUM;i++)
		{
			if(data[i][0] != 255)
				conut++;
		}
		return conut;
	};
	BYTE GetAllPaiCount()
	{
		BYTE num = 0;
		for(int i=0;i<HAND_CARD_NUM;i++)
		{
			if(data[i][0] != 255)
				num += data[i][1];
		}
		return num;
	};
	void SetPaiCount(BYTE pai,BYTE num)
	{
		for(int i=0;i<HAND_CARD_NUM;i++)
		{
			if(data[i][0] == pai && pai != 255)
			{
				data[i][1] = num;
			}
		}
	}
	void Init()
	{
		memset(data,255,sizeof(data));
		for(int i=0;i<HAND_CARD_NUM;i++)
		{
			data[i][1] = 0;
		}
		conut=0;
	};
	CheckHuStruct()
	{
		Init();
	}
};
///ƽ���������ݽṹ
struct PingHuStruct
{
	BYTE  byType[6];    //��������
	BYTE  data[6][4];   //����
	bool  isjing[6][4];//�Ƿ�������
	BYTE  count;        //�ڵ���
	PingHuStruct()
	{
		Init();
	};
	void AddData(BYTE type,BYTE pai[],bool jing[])
	{
		for(int i=0;i<6;i++)
		{
			if(byType[i] == 255)
			{
				byType[i] = type;
				data[i][0] = pai[0];data[i][1] = pai[1];data[i][2] = pai[2];data[i][3] = pai[3];
				isjing[i][0] = jing[0];isjing[i][1] = jing[1];isjing[i][2] = jing[2];isjing[i][3] = jing[3];
				count++;
				break;
			}
		}
	};
	void Init()
	{
		memset(this,255,sizeof(PingHuStruct));
		memset(isjing,0,sizeof(isjing));
		count = 0;
	};
};

/////////////////�������ͽṹ//////////////////////////////////////
///��������
enum HUPAI_TYPE//LU��������ö�ٽṹ
{		
	HUPAI_TYPE_None						=0,		////û��	
	HUPAI_TYPE_HuPai					=1,		////����
	HUPAI_TYPE_ZiMo						=2,		////����
	HUPAI_TYPE_PingHu					=3,		////ƽ��
	HUPAI_TYPE_TianHu					=4,		////���
	HUPAI_TYPE_DiHu						=5,		////�غ�
	HUPAI_TYPE_GangKai					=6,		//���ϻ�
	HUPAI_TYPE_GangHaoPao				=7,		//�ܺ���
	HUPAI_TYPE_YiBanGao					=8,		//һ���
	HUPAI_TYPE_XiXiangFeng				=9,		//ϲ���
	HUPAI_TYPE_LianLiu					=10,	//����
	HUPAI_TYPE_LaoShaoFu				=11,	//���ٸ�
	HUPAI_TYPE_YaoJiuKe					=12,	//�۾ſ�
	HUPAI_TYPE_MingGang					=13,	//����
	HUPAI_TYPE_QueYiMen					=14,	//ȱһ��
	HUPAI_TYPE_WuZi						=15,	//����
	HUPAI_TYPE_BianZhang				=16,	//����
	HUPAI_TYPE_KanZhang					=17,	//����
	HUPAI_TYPE_DanDiaoJiang				=18,	//������
	HUPAI_TYPE_JianKe					=19,	//����
	HUPAI_TYPE_QuanFengKe				=20,	//Ȧ���
	HUPAI_TYPE_MenFengKe				=21,	//�ŷ��
	HUPAI_TYPE_MenQianQing				=22,	//��ǰ��
	HUPAI_TYPE_SiGuiYi					=23,	//�Ĺ�һ
	HUPAI_TYPE_ShuangTongKe				=24,	//˫ͬ��
	HUPAI_TYPE_ShuangAnKe				=25,	//˫����
	HUPAI_TYPE_AnGang					=26,	//����
	HUPAI_TYPE_DuanYao					=27,	//����
	HUPAI_TYPE_QuanDaiYao				=28,	//ȫ����
	HUPAI_TYPE_BuQiuRen					=29,	//������
	HUPAI_TYPE_ShuangMingGang			=30,	//˫����
	HUPAI_TYPE_HuJueZhang				=31,	//�;���
	HUPAI_TYPE_PengPengHu				=32,	//������
	HUPAI_TYPE_HunYiSe					=33,	//��һɫ
	HUPAI_TYPE_SanSeSanBuGao			=34,	//��ɫ������
	HUPAI_TYPE_WuMenQi					=35,	//������
	HUPAI_TYPE_QuanQiuRen				=36,	//ȫ����
	HUPAI_TYPE_ShuangAnGang				=37,	//˫����
	HUPAI_TYPE_ShuangJianKe				=38,	//˫����
	HUPAI_TYPE_HuaLong					=39,	//����
	HUPAI_TYPE_TuiBuDao					=40,	//�Ʋ���
	HUPAI_TYPE_SanSeSanTongShun			=41,	//��ɫ��ͬ˳
	HUPAI_TYPE_SanSeSanJieGao			=42,	//��ɫ���ڸ�
	HUPAI_TYPE_WuFanHu					=43,	//�޷���
	HUPAI_TYPE_MiaoShouHuiChun			=44,	//���ֻش�
	HUPAI_TYPE_HaiDiLaoYue				=45,	//��������
	HUPAI_TYPE_QiangGang				=46,	//���ܺ�
	HUPAI_TYPE_QuanBuKao				=47,	//ȫ����(ʮ����)
	HUPAI_TYPE_ZuHeLong					=48,	//�����
	HUPAI_TYPE_DaYuWu					=49,	//������
	HUPAI_TYPE_XiaoYuWu					=50,	//С����
	HUPAI_TYPE_SanFengKe				=51,	//�����
	HUPAI_TYPE_QingLong					=52,	//����
	HUPAI_TYPE_SanSeShuangLongHui		=53,	//��ɫ˫����
	HUPAI_TYPE_YiSeSanBuGao				=54,	//һɫ������
	HUPAI_TYPE_QuanDaiWu				=55,	//ȫ����
	HUPAI_TYPE_SanTongKe				=56,	//��ͬ��
	HUPAI_TYPE_SanAnKe					=57,	//������
	HUPAI_TYPE_QiDui					=58,	//�߶�
	HUPAI_TYPE_QiXingBuKao				=59,	//���ǲ���
	HUPAI_TYPE_QuanShuangKe				=60,	//ȫ˫��
	HUPAI_TYPE_QingYiSe					=61,	//��һɫ
	HUPAI_TYPE_YiSeSanTongShun			=62,	//һɫ��ͬ˳
	HUPAI_TYPE_YiSeSanJieGao			=63,	//һɫ���ڸ�
	HUPAI_TYPE_QuanDa					=64,	//ȫ��
	HUPAI_TYPE_QuanZhong				=65,	//ȫ��
	HUPAI_TYPE_QuanXiao					=66,	//ȫС
	HUPAI_TYPE_YiSeSiBuGao				=67,	//һɫ�Ĳ���
	HUPAI_TYPE_SanGang					=68,	//����
	HUPAI_TYPE_HunYaoJiu				=69,	//���۾�
	HUPAI_TYPE_YiSeSiTongShun			=70,	//һɫ��ͬ˳
	HUPAI_TYPE_YiSeSiJieGao				=71,	//һɫ�Ľڸ�
	HUPAI_TYPE_QingYaoJiu				=72,	//���۾�
	HUPAI_TYPE_XiaoSiXi					=73,	//С��ϲ
	HUPAI_TYPE_XiaoSanYuan				=74,	//С��Ԫ
	HUPAI_TYPE_ZiYiSe					=75,	//��һɫ
	HUPAI_TYPE_SiAnKe					=76,	//�İ���
	HUPAI_TYPE_YiSeShuangLongHui		=77,	//һɫ˫����
	HUPAI_TYPE_DaSiXi					=78,	//����ϲ
	HUPAI_TYPE_DaSanYuan				=79,	//����Ԫ
	HUPAI_TYPE_LvYiSe					=80,	//��һɫ
	HUPAI_TYPE_JiuLianBaoDeng			=81,	//��������
	HUPAI_TYPE_SiGang					=82,	//�ĸܣ�ʮ���޺���
	HUPAI_TYPE_LianQiDui				=83,	//���߶�
	HUPAI_TYPE_ShiSanYao				=84		//ʮ����
};
///////////////////////////////////////////////////////////////

///�������ݽṹ
struct JingStruct
{
	BYTE  byPai[8];    //ÿ�ž��Ƶ���ֵ
	BYTE  byJingNum;   //������������
	JingStruct()
	{
		Init();
	}
	void Init()
	{
		memset(this,255,sizeof(JingStruct));
		byJingNum = 0;
	};
	bool CheckIsJing(BYTE pai)//���ĳ�����Ƿ����
	{
		for(int i=0;i<8;i++)
		{
			if(byPai[i] == pai && pai!=255)
				return true;
		}
		return false;
	};
	bool AddJing(BYTE pai)//��Ӳ�����
	{
		for(int i=0;i<8;i++)
		{
			if(byPai[i] == 255 && pai !=255)
			{

				byPai[i] = pai;
				byJingNum++;
				return true;
			}
		}
		return false;
	};
	void sort()//���������
	{
		BYTE pai = 255;
		for(int i=0;i<7;i++)
		{
			
			for(int j=i+1;j<8;++j)
			{
				if(byPai[j] < byPai[i])
				{
					pai = byPai[j];
					byPai[j] = byPai[i];
					byPai[i] = pai;
				}
			}
		}
	}
};

///�����������ݽṹ
struct GCPStruct
{
	BYTE  byType;    //����������
	BYTE  iOutpai;   //���˳�����
	BYTE  byData[4]; //������������
	BYTE  iStation;  //���������λ��
	BYTE  iBeStation;//�����������λ��
	GCPStruct()
	{
		Init();
	}
	void Init()
	{
		memset(byData,255,sizeof(byData));
		byType = 255;    //����������
		iOutpai = 255;   //���˳�����
		iStation = 255;  //���������λ��
		iBeStation = 255;//�����������λ��
	};
};
///�����ݽṹ��������ֵ��������
struct PaiStruct
{
	BYTE  pai;   //������
	BYTE  count; //������
	PaiStruct()
	{
		Init();
	};
	void Init()
	{
		memset(this,255,sizeof(PaiStruct));
		count = 0;
	};
};

// ��Ϸ���ݰ�
/********************************************************************************/
//ֻ��һ�����ݰ�����������ݿ��Բ�ʹ��,��������ʹ��
struct GameStationData
{
	//������Ϣ
	int					iVersion;						// �����汾
	int					iVersion2;
	int					uRunPublish;					//����Ҫ�۵ķ���
	int					basepoint;						//����ı���
	int					iWinPoint;						// ��Ϸ����
	int					iPunishPoint;					// �û�����
	//��Ϸ��Ϣ
	BYTE				byPlayCount;					// ��Ϸ����
	BYTE				byNtStation;					// ׯ��λ��
	BYTE				byBeenPlayGame;					// �Ѿ���Ϸ�ľ���
	BYTE				byQuanFeng;						// Ȧ������
	bool				m_bistuoguan[4];				// �Ƿ��й�
	BYTE				m_byNowOutStation;				// ���ڳ����û�
	BYTE				m_byOtherOutPai;				// ���ڳ�����
	bool				m_bTing[4];						// �Ƿ�����
	BYTE				m_byThingDoing;					// ���ڷ������¼��� 
    BYTE				Stationpara;					// ��Ϸ״̬���ȴ�ͬ��״̬������״̬����Ϸ��״̬���ȴ���һ��״̬
    BYTE                ucAgree;                        //����λ���Ƿ�ͬ��
	//������
	BYTE				m_byArHandPai[4][HAND_CARD_NUM];//���Ʋ�����20�ţ�0Ϊ������־
	BYTE				m_byArOutPai[4][OUT_CARD_NUM];	//���Ʋ�����160�ţ�0Ϊ������־
	//�ĸ���ҵ�״̬��ÿ�����32λ�����Դ���32��״̬��ǰ10 ���ʹ�ã�����ĺ�ʹ��
	bitset< 128 >m_bitUserState;
	GameStationData()
	{
		Init();
	};
	void Init()
	{
		memset(this, 0, sizeof(GameStationData));	
		memset(m_byArHandPai,255,sizeof(m_byArHandPai));
		memset(m_byArOutPai,255,sizeof(m_byArOutPai));
	}
};
//�û��й����ݰ�
struct Usertouguan
{						
	BYTE			desk;				// ˭�й�
	bool			touguang;			// �й�״̬
	//�ĸ���ҵ�״̬��ÿ�����32λ�����Դ���32��״̬��ǰ10 ���ʹ�ã�����ĺ�ʹ��
	bitset< 128 > m_bitUserState;
};
///�û�ͬ��ṹ
struct MSG_UserAgree
{
    BYTE			bDeskNO;							///��Ϸ����
    BYTE			bDeskStation;						///λ�ú���
    BYTE			bAgreeGame;							///ͬ���־
};
//�û�ͬ����Ϸ
struct UserArgeeGame
{
	BYTE			byPlayCount;				// ��Ϸ����
	BYTE			byCardCount;				// ����Ŀ
};

// ��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	BYTE				bDeskStation;					// �˳�λ��
	int					iTurePoint[4];					// ׯ�ҵ÷�
	int					iChangeMoney[8];				// �������
};
struct HaveThingStruct		// �����뿪
{
	BYTE	pos;
	TCHAR	szMessage[61];
};
struct LeaveResultStruct	// �����뿪���
{
	BYTE	bLeaveStation; //��Ҫ�뿪��ҵ�λ��
	BYTE	bDeskStation;
	int		bArgeeLeave;
};
struct MessageStruct		// �ͻ��˵�������,��Ϣ
{
	TCHAR Message[200];
};

///////////////////////////////////////////////////////////////////////////

#define MAX_HEPAI_TYPES     256 ///����������

///�齫�ƣ��������ƣ������㣬��ǰ���Ʒ���
struct MjMenPaiData 
{
	BYTE    byMenPai[300];				//�齫����������Ҹ�����������������ÿ������2���� 
	BYTE    byNt;						//ׯ��λ��
	BYTE	byStation;					//��ǰץ����ҵ�λ��
	BYTE	byDir;						//��ǰץ�Ʒ���(ץ�Ǹ������ǰ����)
	BYTE	byGetPaiDir;				//�������Ʒ���
	BYTE	byGetPai;				    //���ƶ���
	BYTE    byMenPaiDun[4];             //ÿ�������ǰ�Ķ���
	BYTE    byStartIndex;				//������ʼ����		
	BYTE    byEndIndex;				    //���ƽ�������		
	BYTE    byCaiShenIndex;				//����λ������		
	BYTE    byAllPaiNum;				//�����Ƶ�����	
	BYTE    byStart;				//����ԭʼ��ʼ����		
	BYTE    byEnd;				    //����ԭʼ��������	

	MjMenPaiData()
	{
		Init();
	};
	void Init()
	{
		memset(byMenPai,255,sizeof(byMenPai));
		memset(byMenPaiDun,255,sizeof(byMenPaiDun));//�����ǰ�Ķ���
		byNt = 255;						//ׯ��λ��
		byStation = 255;					//��ǰץ����ҵ�λ��
		byDir = 255;						//��ǰץ�Ʒ���(ץ�Ǹ������ǰ����)
		byGetPaiDir = 255;				//�������Ʒ���
		byGetPai = 255;				    //���ƶ���
		byStartIndex = 255;				//������ʼ����
		byEndIndex = 255;				    //���ƽ�������
		byAllPaiNum = 255;				    //�����Ƶ�����
		byCaiShenIndex = 255;				//����λ������
		byStart = 255;				//����ԭʼ��ʼ����		
		byEnd = 255;				    //����ԭʼ��������	
	};
	BYTE GetMenPaiNum()
	{
		BYTE num = 0;
		for(int i=0;i<300;i++)
		{
			if(byMenPai[i] != 255)
			{
				num++;
			}
		}
		return num;
	};
};

///�齫��������
struct MjRuleSet 
{
	BYTE    byGameUser;			//��Ϸ����
	BYTE    byAllMjCount;		//�齫������ 
	BYTE	byHandCardNum;		//��������
	BYTE	byGamePassNum;		//��������
	BYTE	byNextNtType;		//�¾�ׯ�ҵ�ȷ������
	BYTE    byMenPaiNum[4];		//����������
	BYTE	byOtherJing;		//��������
	bool    bHaveFlower;		//�Ƿ��л���
	bool    bGangZhong;			//�Ƿ�ܺ���
	bool    bGangFa;			//�Ƿ�ܷ���
	bool    bGangBai;			//�Ƿ�ܰװ�
	bool    bHaveHongZhong;		//�Ƿ��к���
	bool    bHaveFaCai;			//�Ƿ��з���
	bool    bHaveBaiBan;		//�Ƿ��аװ�
	bool    bHaveFengPai;		//�Ƿ��ж�������
	bool	bHaveJing;			//���޲���

	bool    bHaveQFeng;			//�Ƿ���Ȧ��
	bool    bNetCutTuoGuan;		//�Ƿ�����й�
	bool	bForceTuoGuan;		//�Ƿ�ǿ���й�
	bool	bAutoBegin;			//ʱ�䵽���Ƿ��Զ���ʼ
	bool	bGetGoodPai;		//�Ƿ����������
	MjRuleSet()
	{
		memset(this, 0, sizeof(MjRuleSet));		
	}
};

///��������
struct ActionRuleSet 
{
	bool bChi;			//��
	bool bChiFeng;		//�Է���
	bool bChiJian;		//���з���
	bool bChiJing;		//�Բ���
	bool bPeng;			//��
	bool bGang;			//��
	bool bBuGang;		//����
	bool bTing;			//��
	bool bQiangChi;		//����(�Ժ�����)
	bool bGangAfterTing;//���ƺ��ܸ���
	bool bSeziAfterGang;//�ܺ��ɫ��ץ��
	bool bQxHh;			//�������Ⱥ���һȦ�ڣ�
	bool bQxHhBut;		//�������Ⱥ���һȦ�ڣ�������ͬ���ƿ��ԣ����ø������
	bool bOutLianZhang;	//���ƺ��ܳ����ţ������Ʋ�����������Լ������������˳�ӣ�
	ActionRuleSet()
	{
		memset(this, 0, sizeof(ActionRuleSet));
	}
};

///������������
struct HuPaiTypeSet 
{
	BYTE byZiMo;			//����
	BYTE byTianHu;			//���
	BYTE byDiHu;			//�غ�
	BYTE byGSH;				//���ϻ�
	BYTE byGHP;				//�ܺ���
	BYTE byWuHu;			//�ں�����
	BYTE byQiXing;			//����
	BYTE byLongQiDui;		//���߶�
	BYTE byYiBanGao;		//һ���
	BYTE byXiXiangFeng;		//ϲ���
	BYTE byLianLiu;			//����
	BYTE byLaoShaoFu;		//���ٸ�
	BYTE byYaoJiuKe;		//�۾ſ�
	BYTE byMingGang;		//����
	BYTE byQueYiMen;		//ȱһ��
	BYTE byWuZi;			//����
	BYTE byBianZhang;		//����
	BYTE byKanZhang;		//����
	BYTE byDanDiao;			//������
	BYTE byJianKe;			//����
	BYTE byQuanFengKe;		//Ȧ���
	BYTE byMenFengKe;		//�ŷ��
	BYTE byMenQianQing;		//��ǰ��
	BYTE byPingHu;			//ƽ��
	BYTE bySiGuiYi;			//�Ĺ�һ
	BYTE byShuangTongKe;	//˫ͬ��
	BYTE byShuangAnKe;		//˫����
	BYTE byAnGang;			//����
	BYTE byDuanYao;			//����
	BYTE byQuanDaiYao;		//ȫ����
	BYTE byBuQiuRen;		//������
	BYTE byShuangMingGang;	//˫����
	BYTE byHuJueZhang;		//�;���
	BYTE byPengPengHu;		//������
	BYTE byHunYiSe;			//��һɫ
	BYTE bySanSeSanBuGao;	//��ɫ������
	BYTE byWuMenQi;			//������
	BYTE byQuanQiuRen;		//ȫ����
	BYTE byShuangAnGang;	//˫����
	BYTE byShuangJianKe;	//˫����
	BYTE byHuaLong;			//����
	BYTE byTuiBuDao;		//�Ʋ���
	BYTE bySSSTongShun;		//��ɫ��ͬ˳
	BYTE bySSSJieGao;		//��ɫ���ڸ�
	BYTE byWuFanHu;			//�޷���
	BYTE byMiaoShouHuiChun;	//���ֻش�
	BYTE byHaiDiLaoYue;		//��������
	BYTE byQiangGangHu;		//���ܺ�
	BYTE byQuanBuKao;		//ȫ����
	BYTE byZuHeLong;		//�����
	BYTE byDaYuWu;			//������
	BYTE byXiaoYuWu;		//С����
	BYTE bySanFengKe;		//�����
	BYTE byQiangLong;		//����
	BYTE bySSShuangLongHui;	//��ɫ˫����
	BYTE byYiSeSanBuGao;	//һɫ������
	BYTE byQuanDaiWu;		//ȫ����
	BYTE bySanTongKe;		//��ͬ��
	BYTE bySanAnKe;			//������
	BYTE byQiDui;			//�߶�
	BYTE byQiXingBuKao;		//���ǲ���
	BYTE byQuanShuangKe;	//ȫ˫��
	BYTE byQingYiSe;		//��һɫ
	BYTE byYiSSTongShun;	//һɫ��ͬ˳
	BYTE byYiSSJieGao;		//һɫ���ڸ�
	BYTE byQuanDa;			//ȫ��
	BYTE byQuanZhong;		//ȫ��
	BYTE byQuanXiao;		//ȫС
	BYTE byYiSeSiBuGao;		//һɫ�Ĳ���
	BYTE bySanGang;			//����
	BYTE byHunYaoJiu;		//���۾�
	BYTE byYSSiTongShun;	//һɫ��ͬ˳
	BYTE byYSSiJieGao;		//һɫ�Ľڸ�
	BYTE byQingYaoJiu;		//���۾�
	BYTE byXiaoSiXi;		//С��ϲ
	BYTE byXiaoSanYuan;		//С��Ԫ
	BYTE byZiYeSe;			//��һɫ
	BYTE bySiAnKe;			//�İ���
	BYTE byYSShuangLongHui;	//һɫ˫����
	BYTE byDaSiXi;			//����ϲ
	BYTE byDaSanYuan;		//����Ԫ
	BYTE byLvYiSe;			//��һɫ
	BYTE byJiuLianBaoDeng;	//��������
	BYTE bySiGang;			//�ĸܣ�ʮ���޺���
	BYTE byLianQiDui;		//���߶�
	BYTE byShiSanYao;		//ʮ����

	HuPaiTypeSet()
	{
		memset(this, 255, sizeof(HuPaiTypeSet));
	}
};

///�齫�¼����ݽṹ
struct MjEvent
{
	BYTE byBeginNext1;			//	��һ��ǰ����Ϸ��ʼ�¼�����¼���
	BYTE byBeginNext2;			//	��һ�ֺ���Ϸ��ʼ�¼�����¼���
	BYTE byDongNtNext;			//	�Զ�Ϊׯ�¼��ĺ���¼���
	BYTE by2SeziNtNext;			//	��2��ɫ�Ӷ�ׯ�¼��ĺ���¼���
	BYTE by2SeziNtAndGpNext;	//	��2��ɫ�Ӷ�ׯ�����Ƶ��¼��ĺ���¼���
	BYTE byTurnSeziNtNext;		//	������2��ɫ�Ӷ�ׯ�¼��ĺ���¼���
	BYTE by2SeziDirNext;		//	��2��ɫ�Ӷ������¼��ĺ���¼���
	BYTE by2SeziDirAndGpNext;	//	��2��ɫ�Ӷ���������Ƶ��¼��ĺ���¼���
	BYTE by2SeziGpNext;			//	��2��ɫ�Ӷ����Ƶ��¼��ĺ���¼���
	BYTE bySendPaiNext;			//	�����¼��ĺ���¼���
	BYTE byTiaoPaiNext;			//	�����¼��ĺ���¼���
	BYTE by2SeziJingNext;		//	��2��ɫ�Ӷ������¼��ĺ���¼���
	BYTE by1SeziJinNext;		//	��1��ɫ�Ӷ������¼��ĺ���¼���
	BYTE byAllBuHuaNext;		//	����ǰ������Ҳ����¼��ĺ���¼���
	BYTE byOneBuHuaNext;		//	��Ϸ�е��˲����¼��ĺ���¼���
	BYTE byBeginOutNext;		//	��ʼ�����¼��ĺ���¼���
	BYTE byOutPaiNext;			//	�����¼��ĺ���¼���
	BYTE byZhuaPaiNext;			//	ץ���¼��ĺ���¼���
	BYTE byChiPaiNext;			//	�����¼��ĺ���¼���
	BYTE byPengPaiNext;			//	�����¼��ĺ���¼���
	BYTE byKanPaiNext;			//	�����¼��ĺ���¼���
	BYTE bySaoHuNext;			//	ɨ���¼��ĺ���¼���
	BYTE byGangPaiNext;			//	�����¼��ĺ���¼���
	BYTE byTingPaiNext;			//	�����¼��ĺ���¼���
	BYTE byHuPaiNext;			//	�����¼��ĺ���¼���
	BYTE byCountFenNext;		//	����¼��ĺ���¼���
	BYTE byEndHandleNext;		//	���������¼��ĺ���¼���
	BYTE byUnnormalEngNext;		//	�����������¼��ĺ���¼���

	MjEvent()
	{
		memset(this, 255, sizeof(MjEvent));
	}
};

//���������������ݰ�
struct SuperStruct
{
	BYTE  bySetType;	//�������ͣ�0 �������ƣ�1��ѡ���ƣ�2�����Լ�����һ���ƣ�3������ǿ����һ����
	BYTE  mjcount;		//�����Ƶĸ���
	BYTE  mjdata[HAND_CARD_NUM];	//��Ҫ��������
	BYTE  changedata[HAND_CARD_NUM];	//��Ҫ������������
	SuperStruct()
	{
		Init();
	}
	void Init()
	{	 
		bySetType = 255;
		mjcount = 0;
		memset(mjdata,255,sizeof(mjdata));
		memset(changedata,255,sizeof(changedata));
	}
};
/********************************************************************************/
// ���ݰ���������ʶ

/********************************************************************************/
#define ASS_GM_GAME_NOTIFY				1				///��Ϣ���ͣ���Ϸ֪ͨ��Ϣ
#define ASS_GM_GAME_STATION				2				///��Ϸ״̬
#define ASS_GM_AGREE_GAME               1               ///�û�ͬ��

//////////////////////�¼���////////////////////////////////////////////////////////////////////
//����ǰ�¼�
#define EVENT_NO				0			//����Ϸ�¼�	
#define EVENT_BEGIN				10			//��Ϸ��ʼ�¼�					        
#define EVENT_DONG_NT			11			//�׾��Զ�Ϊׯ�¼�					         
#define EVENT_2SEZI_NT			12			//�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
#define EVENT_2SEZI_NT_GP		13			//�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
#define EVENT_TURN_2SEZI_NT		14			//�׾�������2��ɫ�Ӷ�ׯ���¼�		
#define EVENT_2SEZI_DIR			15			//��2��ɫ���¼������Ʒ����¼�		
#define EVENT_2SEZI_DIR_GP		16			//��2��ɫ���¼������Ʒ�������Ƶ��¼�
#define EVENT_2SEZI_GP			17			//��2��ɫ���¼������Ƶ��¼�		
#define EVENT_2SEZI_NT_DIR_GP	18			//��2��ɫ���¼������Ƶ��¼�	
//����ǰ�¼�
#define EVENT_SEND_PAI			30			//�����¼�							
#define EVENT_TIAO_PAI			31			//�����¼�							
#define EVENT_2SEZI_JING		32			//��2��ɫ�Ӷ������¼�			
#define EVENT_1SEZI_JIN			33			//��1��ɫ�Ӷ������¼�
#define EVENT_ALL_BUHUA		    34			//Ϊ������Ҳ����¼�				
#define EVENT_ONE_BUHUA		    35			//������Ҳ����¼�					
//���������¼�
#define EVENT_BEGIN_OUT			50			//ׯ�ҿ�ʼ����֪ͨ�¼�				
#define EVENT_OUT_PAI			51			//�����¼�
#define EVENT_ZHUA_PAI			52			//ץ���¼�
#define EVENT_CPG_NOTIFY		53			//�ܳ���֪ͨ�¼��¼�
#define EVENT_CHI_PAI			54			//�����¼�
#define EVENT_PENG_PAI			55			//�����¼�
#define EVENT_GANG_PAI			56			//�����¼�
#define EVENT_TING_PAI			57			//�����¼�
//���������¼�

//���Ƽ����ƺ��¼�
#define EVENT_HU_PAI			60			//���¼�	
#define EVENT_COUNT_FEN			61			//����¼�	
#define EVENT_END_HANDLE		62			//���������¼�
#define EVENT_ENG_UN_NORMAL		63			//���������������¼�

//�������¼���Ϣ
#define ASS_SEZIFINISH			91				//ɫ�ӽ���
#define ASS_AUTOCARD			92				//�й�
#define ASS_PASS_CHECK			93				//"��"
#define ASS_SUPER				94				//�����ͻ��ˣ�������

#define ASS_HAVE_THING			95				//�����뿪
#define ASS_LEFT_RESULT			97				//�뿪���
#define ASS_EXIT				98				//�˳�
#define ASS_MESSAGE				99				//��Ϣ

#define EVENT_GAME_MAX				100			//����¼�id
//////////////////////�¼����ݽṹ/////////////
struct tagBaseData//�������ݣ�ÿ���¼��ذ���������
{
	BYTE byUser;//ִ�����
	BYTE byNo; //�¼���
	BYTE byNext;//��һ���¼���
	//�ĸ���ҵ�״̬��ÿ�����32λ�����Դ���32��״̬��ǰ10 ���ʹ�ã�����ĺ�ʹ��
	bitset< 128 >m_bitUserState;
	void Clear()
	{
		byUser = 255;	//ִ�����
		byNo = 255;		//�¼���
		byNext = 255;	//��һ���¼���
		m_bitUserState = 0;
	}
};

///��Ϸ��ʼ�¼�
struct tagBegin
{
	BYTE	byNt;		//ׯ��λ��
	BYTE	byPlayCount;// ��Ϸ�غ�
	BYTE	byQuanFeng;	// Ȧ������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	m_byMenPaiCount[4];//�ĸ��������Ƶ�����
	BYTE	byNo;		//�¼��Ĵ���
	BYTE	byNext;	//��һ���¼��Ĵ���
	BYTE	byNext2;	//��һ���¼��Ĵ��ţ��Ѿ���ׯʱ��
	void Clear()
	{
		byNt	=255;		//ׯ��λ��
		byPlayCount	=255;// ��Ϸ�غ�
		byQuanFeng	=255;// Ȧ������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byMenPaiCount, 0, sizeof(m_byMenPaiCount));	//�ĸ��������Ƶ�����
		byNo = EVENT_DONG_NT;//�¼��Ĵ���
	}
};

///�Զ�Ϊׯ�¼�
struct tagDongNt
{
	BYTE   byNt;		//ׯ��λ��
	BYTE   byNo;		//�¼��Ĵ���
	BYTE   byNext;	//��һ���¼��Ĵ���
	void Clear()
	{	
		byNt	=255;//ׯ��λ��
		byNo = EVENT_BEGIN;//�¼��Ĵ���
	}
};

///��2��ɫ�ӵĵ�����Ϊׯ�¼�
struct tagTwoSeziNt
{
	BYTE	byUser;//��ɫ�����
	BYTE	byNt;//ׯ��λ��
	BYTE	bySezi0;//ɫ��0����
	BYTE	bySezi1;//ɫ��1����
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE	byNo;//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//��ɫ�����
		byNt	=255;//ׯ��λ��
		bySezi0	=255;//ɫ��0����
		bySezi1	=255;//ɫ��1����	
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = EVENT_2SEZI_NT;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ��ׯ�Һ����Ƶ�λ���¼�
struct tagTwoSeziNtAndGetPai
{
	BYTE   byUser;	//��ɫ�����
	BYTE   byNt;	//ׯ��λ��
	BYTE   bySezi0;	//ɫ��0����
	BYTE   bySezi1;	//ɫ��1����
	BYTE   byGetPai;//����λ��
	bool   bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE   byNo;//�¼��Ĵ���
	BYTE   byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//��ɫ�����
		byNt	=255;//ׯ��λ��
		bySezi0	=255;//ɫ��0����
		bySezi1	=255;//ɫ��1����
		byGetPai=255;//����λ��
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = EVENT_2SEZI_NT_GP;//�¼��Ĵ���
	}
};

///������2��ɫ��ȷ��ׯ��
struct tagTurnSeziNt
{
	BYTE	byNt;//ׯ��λ��
	BYTE	byUser;//��ǰ��ɫ�ӵ����
	BYTE	bySezi0;//ɫ��0����
	BYTE	bySezi1;//ɫ��1����
	BYTE	bySezi[4];//���0ɫ������
	BYTE	byMaxUser;//����������Ҹ���
	BYTE	byMaxDian;//������ֵ
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	bool	bDoFinish[4];//��¼�Ѿ�����ɫ�����
	bool	bDoSezi[4];//����Ҫ��ɫ�ӵ����
	bool	bFirst;//�Ƿ��һ����ɫ��
	BYTE	byNo;//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byNt	=255;//ׯ��λ��
		byUser	=255;//��ǰ��ɫ�ӵ����
		bySezi0	=255;//ɫ��0����
		bySezi1	=255;//ɫ��1����
		memset(bySezi,255,sizeof(bySezi));//���0ɫ������
		byMaxUser	=255;//����������Ҹ���
		byMaxDian	=255;//������ֵ
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		memset(bDoFinish,0,sizeof(bDoFinish));
		memset(bDoSezi,0,sizeof(bDoSezi));
		bFirst = true;//�Ƿ��һ����ɫ��
		byNo = EVENT_TURN_2SEZI_NT;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ������λ���¼�
struct tagTwoSeziDir
{
	BYTE	byUser;			//��ɫ�����
	BYTE	byNt;			//ׯ��
	BYTE	bySezi0;		//ɫ��0����
	BYTE	bySezi1;		//ɫ��1����
	BYTE	byDirection;	//���Ʒ�λ
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE	byNo;			//�¼��Ĵ���
	BYTE	byNext;		//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;		//��ɫ�����
		bySezi0	=255;		//ɫ��0����
		bySezi1	=255;		//ɫ��1����
		byNt = 255;			//ׯ��
		byDirection	=255;	//���Ʒ�λ
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = EVENT_2SEZI_DIR;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
struct tagTwoSeziDirAndGetPai
{
	BYTE	byUser;	//��ɫ�����
	BYTE	bySezi0;	//ɫ��0����
	BYTE	bySezi1;	//ɫ��1����
	BYTE	byDirection;//���Ʒ�λ
	BYTE	byGetPai;	//���Ƶ�
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE	byNo;		//�¼��Ĵ���
	BYTE	byNext;	//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;	//��ɫ�����
		bySezi0	=255;	//ɫ��0����
		bySezi1	=255;	//ɫ��1����
		byDirection	=255;//���Ʒ�λ
		byGetPai	=255;	//���Ƶ�
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = EVENT_2SEZI_DIR_GP;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ�����ƶ����¼�
struct tagTwoSeziGetPai
{
	BYTE	byUser;//��ɫ�����
	BYTE	bySezi0;//ɫ��0����
	BYTE	bySezi1;//ɫ��1����
	BYTE	byGetPai;//���Ƶ�
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;	//��ɫ�����
		bySezi0	=255;	//ɫ��0����
		bySezi1	=255;	//ɫ��1����
		byGetPai	=255;	//���Ƶ�
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = EVENT_2SEZI_GP;//�¼��Ĵ���
	}
};

///�����¼�
struct tagSendPai
{
	BYTE	byPaiNum;//ÿ����ҷ�������
	BYTE	byRemainPai;//ʣ������
	BYTE	bySendPai;//��������
	BYTE	byGetPai;//���Ƶ�
	BYTE	byAllPaiNum;//������
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE    byStart;				//����ԭʼ��ʼ����		
	BYTE    byEnd;				    //����ԭʼ��������	
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byPaiNum	=255;//ÿ����ҷ�������
		byRemainPai	=255;//ʣ������
		bySendPai	=255;//��������
		byGetPai	=255;//���Ƶ�
		byAllPaiNum	=0;//������
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		byStart	=255;///��ʼ����
		byEnd	=255;///��������
		byNo = EVENT_SEND_PAI;//�¼��Ĵ���
	}
};

///��ɫ��2��ɫ�Ӷ������¼�
struct tagTwoSeziJing
{
	BYTE	byUser;//��ɫ�����λ��
	BYTE	byNt;//ׯ��λ��
	BYTE	byFanPai;//���Ƶ�
	BYTE	bySezi0;//ɫ��0����
	BYTE	bySezi1;//ɫ��1����
	BYTE	byCaiShenIndex; ///��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	JingStruct  m_StrJing;
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser		=255;//��ɫ�����λ��
		byNt		=255;//ׯ��λ��
		byFanPai	=255;//���Ƶ�
		bySezi0		=255;//ɫ��0����
		bySezi1		=255;//ɫ��1����
		byCaiShenIndex=255; ///��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		m_StrJing.Init();
		byNo = EVENT_2SEZI_JING;//�¼��Ĵ���
	}
};

///������Ҳ����¼�
struct tagAllBuHua
{
	BYTE	byUser[4];//��Ҫ�������λ��
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	m_byArHuaPai[4][HUA_CARD_NUM];//��������
	bool	bBuHuaFinish[4];//��¼�Ѿ��Ѿ�������ɵ����
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		memset(byUser, 255, sizeof(byUser));	//��Ҫ�������λ��
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//��������	
		memset(bBuHuaFinish,0,sizeof(bBuHuaFinish));
		byNo = EVENT_ALL_BUHUA;//�¼��Ĵ���
	}
};

///������Ҳ����¼�
struct tagOneBuHua
{
	BYTE	byUser;//�������λ��
	BYTE	byHua;	  //ץ���û���
	BYTE	byPs;	  //��������
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	m_byArHuaPai[4][HUA_CARD_NUM];//
	bool	bFinish;//�Ƿ񲹻�����
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//�������λ��
		byHua	=255;	  //ץ���û���
		byPs	=255;	  //��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//��������
		bFinish=false;
		byNo = EVENT_ONE_BUHUA;//�¼��Ĵ���
	}
};
///��ʼ����֪ͨ�¼�
struct tagBeginOutPai
{
	BYTE	byUser;	//�״γ������λ��
	BYTE	byNt;	//ׯ��λ��
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//�״γ������λ��
		byNt	=255;//ׯ��λ��
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		byNo = EVENT_BEGIN_OUT;//�¼��Ĵ���
	}
};
///�����¼�
struct tagOutPai
{
	BYTE	byUser;				//�������λ��
	BYTE	byPs;				//���Ƶ�ֵ
	BYTE	byMaxAction;		//������������Ķ���
	BYTE	byMaxStation;		//���������Ķ����������λ��
	BYTE	byCanAction[4][5];	//��ҿ������Ķ���
	BYTE	byWantAction[4];	//��������Ķ���
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	bool	bOtherUse;			//�ƴ����������Ҫ��������
	bool	bTing;				//�Ƿ����Ƴ���
	//�ĸ���ҵ�״̬��ÿ�����32λ�����Դ���32��״̬��ǰ10 ���ʹ�ã�����ĺ�ʹ��
	bitset< 128 >m_bitUserState;
	BYTE	byNo;				//�¼��Ĵ���
	BYTE	byNext;				//��һ���¼��Ĵ���(0������ѭ���¼�����һ���¼����û�����)
	void Clear()
	{
		byUser	=255;				//�������λ��
		byPs		=255;				//���Ƶ�ֵ
		byMaxAction	=255;		//������������Ķ���
		byMaxStation = 255;		//���������Ķ����������λ��
		memset(byCanAction, 255, sizeof(byCanAction));//������������Ķ���	
		memset(byWantAction, 255, sizeof(byWantAction));//��������Ķ���
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������	
		bOtherUse = false;    //�ƴ����������Ҫ��������
		bTing = false;//�Ƿ����Ƴ���
		byNo = EVENT_OUT_PAI;//�¼��Ĵ���
	}
};
///ץ�����¼�
struct tagZhuaPai
{
	BYTE	byUser;		//ץ�����λ��
	BYTE	byPs;		//ץ�Ƶ�ֵ
	BYTE	byStartIndex;///��ʼ����
	BYTE	byEndIndex;  ///��������
	BYTE	byCaiShenIndex; ///��������
	BYTE	byGetPaiIndex;//ץ�Ƶ�����
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	m_byArHuaPai[4][HUA_CARD_NUM];//��һ���
	bool	bHead;		//ץ�Ƶķ���
	bool	bCanOutPai;	//ץ�ƺ��Ƿ���Գ���
	bool    m_bGangKai;	//�Ƿ�ܿ�	
	bool    m_bBuHua;	//�Ƿ�Ҫ����	
	//�ĸ���ҵ�״̬��ÿ�����32λ�����Դ���32��״̬��ǰ10 ���ʹ�ã�����ĺ�ʹ��
	bitset< 128 >m_bitUserState;
	BYTE	byNo;		//�¼��Ĵ���
	BYTE	byNext;		//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;		//ץ�����λ��
		byPs		=255;		//ץ�Ƶ�ֵ
		byStartIndex=255;///��ʼ����
		byEndIndex	=255;  ///��������
		byCaiShenIndex =255; ///��������
		byGetPaiIndex = 255;//ץ�Ƶ�����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));	//����
		bHead = true;
		bCanOutPai = false;//ץ�ƺ��Ƿ���Գ���
		m_bGangKai = false;//�Ƿ�ܿ�	
		m_bBuHua = false;	//�Ƿ�Ҫ����	
		byNo = EVENT_ZHUA_PAI;//�¼��Ĵ���
	}
};
///�����ܺ�֪ͨ�¼��¼�
struct tagCPGNotify
{
	BYTE	byUser;				//֪ͨ���λ��
	BYTE	byOutStation;		//������ҵ�λ��
	BYTE	byPs;				//���Ƶ�ֵ
	bool	bZhuaPai;			//�Ƿ��Լ�ץ��
	bool	bChi;				//�Ƿ��ܳ�
	bool	bPeng;				//�Ƿ�����
	bool	bGang;				//�Ƿ��ܸ�
	bool	bTing;				//��ǰ�ܷ�����
	bool	bHu;				//�Ƿ��ܺ�
	bool	bQiangGang;			//�Ƿ����ܺ�
	bool    bHaveReturn;		//�ͻ����Ƿ񷵻ص��û�ѡ��
	bool    bCanAction;			//����Ƿ��ж�������
	bool    bZimo;				//�Ƿ���������
	BYTE	m_iChi[3][3];		//�ܳԵ���
	BYTE	m_iGangData[4][2];	//�ܸܵ���	
	BYTE	m_byTingCanOut[HAND_CARD_NUM];//�����ܴ����
	BYTE    m_byMaxThing;		//��������������
	BYTE	byHuType[MAX_HUPAI_TYPE];	//��������
	BYTE    m_iHuPs;			//��Һ�����
	BYTE	m_byDianPao;		//�������λ��
	BYTE	byNo;				//�¼��Ĵ���
	BYTE	byNext;				//��һ���¼��Ĵ���(0������ѭ���¼�����һ���¼����û�����)
	void Clear()
	{
		byUser	=255;			//�������λ��
		byOutStation = 255;		//������ҵ�λ��
		byPs	=255;			//���Ƶ�ֵ
		bZhuaPai=0;				//�Ƿ��Լ�ץ��
		bChi	=0;				//�Ƿ��ܳ�
		bPeng	=0;				//�Ƿ�����
		bGang	=0;				//�Ƿ��ܸ�
		bTing	=0;				//��ǰ�ܷ�����
		bHu		=0;				//�Ƿ��ܺ�
		bQiangGang = 0;			//�Ƿ����ܺ�
		bCanAction = false;			//����Ƿ��ж�������
		bZimo = false;				//�Ƿ���������
		m_byMaxThing = 0;		//��������������
		bHaveReturn = 0;		//�ͻ����Ƿ񷵻ص��û�ѡ��
		memset(m_iChi,255,sizeof(m_iChi));//�ܳԵ���
		memset(m_iGangData,255,sizeof(m_iGangData));//�ܸܵ���	
		memset(m_byTingCanOut,255,sizeof(m_byTingCanOut));///�����ܴ����
		memset(byHuType,255,sizeof(byHuType));///��������
		m_iHuPs = 255;///��Һ�����
		m_byDianPao = 255;		//�������λ��
		byNo = EVENT_OUT_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagChiPai
{
	BYTE	byUser;//�������λ��
	BYTE	byBeChi;//���������λ��
	BYTE	byType;//���Ƶ����ͣ�0�Դ�1���м䣬2��С�������ƣ�
	BYTE	byPs;	//�Ե���ֵ
	BYTE	byChiPs[3];//���Ƶ�����
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//�������λ��
		byBeChi	=255;//���������λ��
		byType	=255;//���Ƶ����ͣ�0�Դ�1���м䣬2��С�������ƣ�
		byPs		=255;	//�Ե���ֵ
		memset(byChiPs, 255, sizeof(byChiPs));//���Ƶ�����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		byNo = EVENT_CHI_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagPengPai
{
	BYTE	byUser;//�������λ��
	BYTE	byBePeng;//���������λ��
	BYTE	byPs;	//������ֵ
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//�������λ��
		byBePeng	=255;//���������λ��
		byPs		=255;//������ֵ
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		byNo = EVENT_PENG_PAI;//�¼��Ĵ���
	}
};

///�������¼�
struct tagGangPai
{
	BYTE	byUser;//�������λ��
	BYTE	byBeGang;//���������λ��
	BYTE	byType;//���Ƶ�����
	BYTE	byPs;	//�ܵ���ֵ
	BYTE	byGangPai[4];//Ҫ�ܵ�������
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	bool    bHe[4];//����ܷ����ܺ�
	bool    bChenkQ;//�������
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//�������λ��
		byBeGang	=255;//���������λ��
		byType	=255;//���Ƶ�����
		byPs		=255;	//�ܵ���ֵ
		memset(byGangPai, 255, sizeof(byGangPai));//Ҫ�ܵ�������
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(bHe, 0, sizeof(bHe));
		bChenkQ = false;
		byNo = EVENT_GANG_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagTingPai
{
	BYTE	byUser;//�������λ��
	BYTE	byTing;//�������λ��
	BYTE	byPs;	//���Ƴ�����ֵ
	BYTE	byNo;	//�¼��Ĵ���
	bool	bHitTing[4];//��ҵ��������
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//�������λ��
		byTing	=255;//�������λ��
		byPs		=255;	//���Ƴ�����ֵ	
		memset(bHitTing,0,sizeof(bHitTing));//��ҵ��������
		byNo = EVENT_TING_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagHuPai
{
	BYTE	byUser;				//�������λ��
	BYTE	byDianPao;			//�������λ��
	BYTE	byNt;				//ׯ��λ��
	BYTE	byPs;				//���ڵ���ֵ
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE    byHuType[4][MAX_HUPAI_TYPE];//������ҵĺ�������
	BYTE	byUserNum;			//������ҵĸ���
	BYTE	byBaoPaiUser;		//�������
	bool	bCanH[4];			//λ��0-3��Щ��ҵ���˺�
	bool	bDuoXiang;			//�Ƿ�һ�ڶ���
	bool    bZimo;				//�Ƿ�����
	bool    bQiangGang;			//�Ƿ�����
	JingStruct  m_StrJing;
	BYTE	byNo;				//�¼��Ĵ���
	BYTE	byNext;				//��һ���¼��Ĵ���
	void Clear()
	{
		byUser		=255;//�������λ��
		byDianPao	=255;//�������λ��
		byNt		=255;//ׯ��λ��
		byPs		=255;	//���ڵ���ֵ
		byBaoPaiUser=255;		//�������
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(byHuType, 255, sizeof(byHuType));//������ҵĺ�������
		byUserNum = 0;//������ҵĸ���
		memset(bCanH,0,sizeof(bCanH));
		bDuoXiang = false;//�Ƿ�һ�ڶ���
		bZimo = false;   //�Ƿ�����
		m_StrJing.Init();
		bQiangGang = false;   //�Ƿ�����
		byNo = EVENT_HU_PAI;//�¼��Ĵ���
	}
};
///����¼�
struct tagCountFen
{
	BYTE	byUser;//�������λ��
	BYTE	byDian;//�������λ��
	BYTE	byNt;//ׯ��λ��
	BYTE	byNextNt;//�¾�ׯ��λ��
	BYTE	byLianZhuang;//��ׯ��
	bool	bHu[4];//��Щ��Һ���
	bool	bZimo;//�Ƿ�����
	bool    bTest;//�Ƿ�������ݰ�
	__int64		iZongFen[4];//������ҵ��ܷ�
	__int64		iMoney[4];//������ҵ��ܷ�
	int 	iHuFen[4];//������ҵĺ��Ʒ�
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE    byHuType[4][MAX_HUPAI_TYPE];//������ҵĺ�������	
	bool	bDuoXiang;//һ�ڶ���
	BYTE	byPs;	//���ڵ���ֵ
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byUser	=255;//�������λ��
		byDian	=255;//�������λ��
		byNt	=255;//ׯ��λ��
		byNextNt=255;//�¾�ׯ��λ��
		byLianZhuang = 0;//��ׯ��
		bZimo   = false;
		bTest   = false;//�Ƿ�������ݰ�
		memset(bHu,0,sizeof(bHu));
		memset(iZongFen,0,sizeof(iZongFen));
		memset(iMoney,0,sizeof(iMoney));
		memset(iHuFen,0,sizeof(iHuFen));
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(byHuType, 255, sizeof(byHuType));//������ҵĺ�������
		bDuoXiang = false;	
		byPs = 255;	//���ڵ���ֵ
		byNo = EVENT_COUNT_FEN;//�¼��Ĵ���
	}
};
///��Ϸ���������¼�
struct tagEndHandle
{
	BYTE	byUser;//�������λ��
	BYTE	byHu;//�������λ��
	BYTE	byNextNt;//�¾�ׯ��λ��
	BYTE	byPs;	//���ڵ���ֵ
	bool	bDuoXiang;//һ�ڶ���
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	tagEndHandle()
	{
		Clear();
	}
	void Clear()
	{		
		byUser	=255;//�������λ��
		byHu	=255;//�������λ��
		byNextNt=255;//�¾�ׯ��λ��
		byPs		=255;	//���ڵ���ֵ
		bDuoXiang = 0;	
		byNo = EVENT_END_HANDLE;//�¼��Ĵ���
	}
};
///���������������¼�
struct tagUnnormalEndHandle
{
	BYTE	byUser;//�������λ��
	int		iFen[8];//�÷�
	int		iMoney[8];//���
	bool    bForce;//�Ƿ�ǿ�˽���
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	tagUnnormalEndHandle()
	{
		Clear();
	}
	void Clear()
	{		
		byUser	=255;//�������λ��
		memset(iFen, 0, sizeof(iFen));
		memset(iMoney, 0, sizeof(iMoney));
		bForce = false;//�Ƿ�ȫ����
		byNo = EVENT_ENG_UN_NORMAL;//�¼��Ĵ���
	}
};

//////////////////�������ݰ�/////////
///�齫��������
struct MJ_Setting
{
	BYTE		byAutoTime;				//�йܵķ�Ӧʱ��
    BYTE        byBlockTime;            ///����ʱ��
    BYTE        byOutcardTime;          ///����ʱ��
    BYTE        byBeginTime;            ///�Զ���ʼ���뿪��ʱ��
    BYTE        byNextTime;             ///��һ���Զ���ʼ���뿪��ʱ��
    BYTE        byWaiteSeziTime;        ///�ȴ�ɫ��ʱ��
    bool        bAutoBegin;             ///�Ƿ��Զ�׼��
    MJ_Setting()
    {
		Clear();
    }
	void Clear()
	{		
		byAutoTime = 1;					// �йܵķ�Ӧʱ��
		byBlockTime = 15;               ///����ʱ��
		byOutcardTime = 20;             ///����ʱ��
		byBeginTime = 30;               ///�Զ���ʼ���뿪��ʱ��
		byNextTime = 30;                ///��һ���Զ���ʼ���뿪��ʱ��
		byWaiteSeziTime = 10;        ///�ȴ�ɫ��ʱ��
		bAutoBegin = 1;                 ///�Ƿ��Զ�׼��
	}
};

///�齫ʱ��ȴ���ʱ�䵽�˷�������Ϊ����
struct MJ_WaitTime
{
	BYTE					byWaitThing;		  // �ȴ������¼�
    BYTE                    byCurChair;           // ��ǰ��˭����        
    BYTE                    byTime;               // �ȴ�ʱ��ʱ��(s)
	bool					bEffect;			  // �ȴ��Ƿ���Ч
    MJ_WaitTime()
    {
		Clear();
    }
	void Clear()
	{	
		byWaitThing = EVENT_NO;		// �ȴ������¼�
		byCurChair = 255;           // ��ǰ��˭����        
		byTime = 255;               // �ȴ�ʱ��ʱ��(s)
		bEffect = false;			// �ȴ��Ƿ���Ч
	}
};
///////////////////////////////////////////////////


#endif