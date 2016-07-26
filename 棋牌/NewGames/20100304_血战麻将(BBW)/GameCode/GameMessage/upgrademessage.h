#pragma once

#include "mj.h"
///������汾
#define DEV_LIB_VER				1									///������汾
//֧������
///֧�����Ͷ���
#define SUP_NORMAL_GAME			0x01								///��ͨ��Ϸ
#define SUP_MATCH_GAME			0x02								///������Ϸ
#define SUP_MONEY_GAME			0x04								///�����Ϸ
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

////////////////////////////////////////////////////////////////////////////////
#define GFF_FORCE_FINISH			0										///< ǿ�н��
#define GF_NORMAL               10                                  /// ��Ϸ��������
#define GF_SAFE                 11                                  /// ��Ϸ��ȫ����

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE		// ��֪ʲô��˼
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

#define AI_FAST							// �����˿��ٳ���
#ifndef AI_MACHINE						// ȡ�����ٻ�����
#undef AI_FAST
#endif

// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					TEXT("MatchTable_BSMJ")

// �汾����
#define GAME_MAX_VER					2									// ������߰汾
#define GAME_LESS_VER					2									// ������Ͱ汾
#define GAME_CHANGE_VER					0									// �޸İ汾		
#define PLAY_COUNT						4									// ��Ϸ����	

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
	void DeleteData(BYTE type,BYTE pai[],bool jing[])//ɾ��Ԫ��
	{
		for(int i=0;i<6;i++)
		{
			if(byType[i] == type && data[i][0] == pai[0 ]&& data[i][1] == pai[1] && isjing[i][0] == jing[0] && isjing[i][1] == jing[1])
			{
				byType[i] = 255;
				memset(data[i],255,sizeof(data[i]));
				memset(isjing[i],0,sizeof(isjing[i]));
				count--;
				break;
			}
		}
	};
	void SortData()
	{
		for(int i=0;i<5;i++)
		{
			if(byType[i] != 255)
				continue;
			for(int j=i;j<5;j++)
			{
				byType[j] = byType[j+1];
				memcpy(data[j],data[j+1],sizeof(data[j]));//����
				memcpy(isjing[j],isjing[j+1],sizeof(isjing[j]));//�Ƿ�������
			}
		}
		count = 0;
		for(int i=0;i<5;i++)
		{
			if(byType[i] != 255)
				count++;
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
	bool CheckIsJing(BYTE pai)
	{
		for(int i=0;i<8;i++)
		{
			if(byPai[i] == pai && pai!=255)
				return true;
		}
		return false;
	};
	bool AddJing(BYTE pai)
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
	void sort()
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
//
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
	bool				bShowUserId;					// �Ƿ���ʾ������ҵ�id
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

    int					Stationpara;
    BYTE                ucAgree;                        //����λ���Ƿ�ͬ��
    int                 m_nWhichTask;
	//������
	BYTE				m_byArHandPai[4][HAND_CARD_NUM];//���Ʋ�����20�ţ�0Ϊ������־
	BYTE				m_byArOutPai[4][OUT_CARD_NUM];	//���Ʋ�����160�ţ�0Ϊ������־
	BYTE				m_byArHuaPai[4][HUA_CARD_NUM];	//���Ʋ�����10�ţ�0Ϊ������־
	BYTE				m_iArMenPai[4][MEN_CARD_NUM];	//���Ʋ�����40��
	//��Ϸʱ��
	BYTE				byAutoOutTime;		//�Զ�����ʱ��
	BYTE				byOutTime;			//����ʱ��
	BYTE				byThinkTime;		//˼��ʱ��
	BYTE				byBeginTime;		//��ʼ�ȴ�ʱ��
	bool				bAutoBegin;			//ʱ�䵽���Ƿ��Զ���ʼ

	GameStationData()
	{
		Init();
	};
	void Init()
	{
		memset(this, 0, sizeof(GameStationData));	
		memset(m_byArHandPai,255,sizeof(m_byArHandPai));
		memset(m_byArOutPai,255,sizeof(m_byArOutPai));
		memset(m_byArHuaPai,255,sizeof(m_byArHuaPai));
		memset(m_iArMenPai,255,sizeof(m_iArMenPai));

		byAutoOutTime = 1;		//�Զ�����ʱ��
		byOutTime = 20;			//����ʱ��
		byThinkTime = 15;		//˼��ʱ��
		byBeginTime = 30;		//��ʼ�ȴ�ʱ��
		bAutoBegin = 0;			//ʱ�䵽���Ƿ��Զ���ʼ
	}
};

//�û��й����ݰ�
struct Usertouguan
{						
	BYTE			desk;				// ˭�й�
	bool			touguang;			// �й�״̬
	bool			bTuoGuan[4];		// ��������й�״̬
};
/////�û�ͬ��ṹ
//struct MSG_GR_R_UserAgree
//{
//    BYTE			bDeskNO;							///��Ϸ����
//    BYTE			bDeskStation;						///λ�ú���
//    BYTE			bAgreeGame;							///ͬ���־
//};
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
	BYTE	byRemainDun;				//ʣ�����
	BYTE	byGetPai;				    //���ƶ���
	BYTE    byMenPaiDun[4];             //ÿ�������ǰ�Ķ���
	BYTE    byStartIndex;				//������ʼ����		
	BYTE    byEndIndex;				    //���ƽ�������		
	BYTE    byCaiShenIndex;				//����λ������		
	BYTE    byAllPaiNum;				//�����Ƶ�����		

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
		byRemainDun = 255;				//ʣ�����
		byGetPai = 255;				    //���ƶ���
		byStartIndex = 255;				//������ʼ����
		byEndIndex = 255;				    //���ƽ�������
		byAllPaiNum = 255;				    //�����Ƶ�����
		byCaiShenIndex = 255;				//����λ������
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
			if(num >= 254)
				break;
		}
		return num;
	};
	BYTE GetOnePaiNum(BYTE pai)
	{
		BYTE num = 0;
		for(int i=0;i<300;i++)
		{
			if(byMenPai[i] == pai)
			{
				num++;
			}
		}
		return num;
	};
	bool DeleteAnMenPai(BYTE pai)
	{
		if(pai == 255)
			return false;
		for(int i=0;i<300;++i)
		{
			if(byMenPai[i] != 255)
			{
				byMenPai[i] = 255;
				return true;
			}
		}
		return false;
	};
	bool ChangAnMenPai(BYTE oldpai,BYTE newpai)// ������Ҫ�������� oldpai,���������� newpai
	{
		if(oldpai == 255 || newpai == 255)
			return false;
		for(int i=0;i<300;++i)
		{
			if(byMenPai[i] == oldpai)
			{
				byMenPai[i] = newpai;//���Ƴɹ�
				return true;
			}
		}
		return false;
	};
};

///�齫��������
struct MjRuleSet 
{
	BYTE    byGameUser;			//��Ϸ����
	BYTE    bySeziNum;			//ɸ�ӵĸ���
	BYTE    byAllMjCount;		//�齫������ 
	BYTE	byHandCardNum;		//��������
	BYTE	byGamePassType;		//����ʣ������
	BYTE	byGamePassNum;		//��������
	BYTE	byNextNtType;		//�¾�ׯ�ҵ�ȷ������
	BYTE    byMenPaiNum[4];		//����������
	BYTE	byOtherJing;		//��������
	BYTE    byAutoOutTime;		//�Զ�����ʱ��
	BYTE    byOutTime;			//����ʱ��
	BYTE    byThinkTime;		//˼��ʱ��
	BYTE    byBeginTime;		//��ʼ�ȴ�ʱ��
	BYTE    byGoodPaiChance;	//�����Ƶļ���
	bool    bHaveFlower;		//�Ƿ��л���
	bool    bGangZhong;			//�Ƿ�ܺ���
	bool    bGangFa;			//�Ƿ�ܷ���
	bool    bGangBai;			//�Ƿ�ܰװ�
	bool    bHaveHongZhong;		//�Ƿ��к���
	bool    bHaveFaCai;			//�Ƿ��з���
	bool    bHaveBaiBan;		//�Ƿ��аװ�
	bool    bHaveFengPai;		//�Ƿ��ж�������
	bool	bHaveJing;			//���޲���
	bool    bRandJing;          //���������
	bool	bMainJing;			//����
	bool	bLessJing;			//����
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
	bool bKan;			//��
	bool bSaoHu;		//ɨ�����黢�齫��
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
///�������
struct CountFenRuleSet 
{
	CountFenRuleSet()
	{
		memset(this, 0, sizeof(CountFenRuleSet));
	}
};
///��������
struct HuPaiRuleSet 
{
	bool bDuoXiang;		//һ�ڶ���
	bool bJieHu;		//�غ�,��һ�ڶ����У����˿��Ժ��ƣ����������͵���ҿ��Խغ�
	bool bZimoHu;		//����
	bool bDianPaoHu;	//����
	bool bNoLimite;		//û��������ֻҪ���ͷ��ϼ�¼��4+1 ���� 7�ԣ�ʮ���ã�ʮ���ۣ�
	bool bSanMQ;		//������
	bool bHaveYJ;		//���۾�
	bool bHaveZi;		//������
	bool bHaveZorYJ;	//�����ƻ����۾�
	bool b258Jiang;		//258����
	bool bKaiMen;		//�ѿ���
	bool bShouBaYi;		//�����ְ�һ
	bool bPiaoCan;		//Ʈ����������
	bool bQiangGHu;		//���ܺ�
	bool bGangKai;		//���Ͽ���
	bool bGangPao;		//�ܺ���
	bool bHaiDiLaoYue;  //��������
	bool bTianHu;		//���
	bool bDiHu;			//�غ�
	BYTE byPingHuType;	//ƽ����������,0û�����ƣ�1��2��3
	HuPaiRuleSet()
	{
		memset(this, 0, sizeof(HuPaiRuleSet));
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
struct MjThing
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

	MjThing()
	{
		memset(this, 255, sizeof(MjThing));
	}
};

//�齫�������ݰ���֪ͨ�ͻ��˸��齫������
struct MjRuleStruct 
{	
	MjRuleSet       m_mjRule;					// �齫��������
	ActionRuleSet   m_mjAction;                 // �齫��������
	CountFenRuleSet m_mjFenRule;				// �齫�������
	HuPaiRuleSet    m_mjHuRule;					// �齫��������
	HuPaiTypeSet	m_mjHuType;					// �齫��������
};

//���������������ݰ�
struct SuperStruct
{
	BYTE  bySetType;	//�������ͣ�0 �������ƣ�1��ѡ���ƣ�2�����Լ�����һ���ƣ�3������ǿ����һ����
	BYTE  mjcount;		//�����Ƶĸ���
	BYTE  mjdata[HAND_CARD_NUM];	//����������
	BYTE  handdata[HAND_CARD_NUM];	//����������

	SuperStruct()
	{
		Init();
	}
	void Init()
	{	 
		bySetType = 255;
		mjcount = 0;
		for( int i = 0; i < 20; i++)
		{  mjdata[i] = 255; }
	}
};
/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/
#define  MDM_GM_GAME_NOTIFY				180				///��Ϣ���ͣ���Ϸ֪ͨ��Ϣ
#define ASS_GM_GAME_STATION				2				///��Ϸ״̬
#define ASS_GM_AGREE_GAME               1               ///�û�ͬ��

#define ASS_UG_USER_SET					70				//�û�������Ϸ
#define ASS_SEZIFINISH					72				//ɫ�ӽ���
#define ASS_AUTOCARD					74				//�й�
#define ASS_PASS_CHECK					75				//"��"
#define ASS_HAVE_THING					76				//�����뿪
#define ASS_LEFT_RESULT					77				//�뿪���

#define ASS_SUPER						78				//�����ͻ��ˣ�������
#define ASS_HUPAI_TEST					79				//�����ͻ��ˣ����Ʋ���

#define ASS_MJ_RULE						89				//�齫��������
#define ASS_EXIT						90				//�˳�
#define ASS_MESSAGE						91				//��Ϣ
#define	ASS_SET_STATE					92				//���ڳ����˵��������󷢸�4���ͻ��˵ļ�ʱ���ź�
#define ASS_GM_SHORT_STOP               93              //��ͣ��Ϣ
#define ASS_NET_CAUSE_END               94              //����ԭ������Ϸ����

//#define ASS_HUPAI_TEST					159			//�����ͻ��ˣ����Ʋ���
#define ASS_CHANGE_PAI					160				//�û����ƣ�������
#define ASS_SET_MOPAI					161				//�û�����׼��Ҫ������
#define ASS_HUANGZHUANG					162				//ǿ�ƻ�ׯ

/********************************************************************************/

//////////////////////�¼���////////////////////////////////////////////////////////////////////
//����ǰ�¼�
enum MJTHINGS
{
    THING_BEGIN				=10,			//��Ϸ��ʼ�¼�					        
    THING_DONG_NT			,			//�׾��Զ�Ϊׯ�¼�					         
    THING_2SEZI_NT			,			//�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
    THING_2SEZI_NT_GP		,			//�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
    THING_TURN_2SEZI_NT		,			//�׾�������2��ɫ�Ӷ�ׯ���¼�		
    THING_2SEZI_DIR			,			//��2��ɫ���¼������Ʒ����¼�		
    THING_2SEZI_DIR_GP		,			//��2��ɫ���¼������Ʒ�������Ƶ��¼�
    THING_2SEZI_GP			,			//��2��ɫ���¼������Ƶ��¼�		
    THING_2SEZI_NT_DIR_GP	,			//��2��ɫ���¼������Ƶ��¼�	
    //����ǰ�¼�            
    THING_SEND_PAI			,			//�����¼�							
    THING_TIAO_PAI			,			//�����¼�							
    THING_2SEZI_JING		,			//��2��ɫ�Ӷ������¼�			
    THING_1SEZI_JIN			,			//��1��ɫ�Ӷ������¼�
    THING_ALL_BUHUA		    ,			//Ϊ������Ҳ����¼�				
    THING_ONE_BUHUA		    ,			//������Ҳ����¼�					
    //���������¼�          
    THING_BEGIN_OUT			,			//ׯ�ҿ�ʼ����֪ͨ�¼�
    THING_DING_QUE          ,           //��ȱ�¼�
    THING_OUT_PAI			,			//�����¼�
    THING_ZHUA_PAI			,			//ץ���¼�
    THING_CPG_NOTIFY		,			//�ܳ���֪ͨ�¼��¼�
    THING_CHI_PAI			,			//�����¼�
    THING_PENG_PAI			,			//�����¼�
    THING_KAN_PAI			,			//�����¼�
    THING_SAO_HU			,			//ɨ���¼�
    THING_GANG_PAI			,			//�����¼�
    THING_TING_PAI			,			//�����¼�
    //���������¼�          

    //���Ƽ����ƺ��¼�
    THING_HU_PAI			,			//���¼�	
    THING_COUNT_FEN			,			//����¼�	
    THING_ENG_HANDLE		,			//���������¼�
    THING_ENG_UN_NORMAL		,			//���������������¼�
                               
    THING_MAX				                //����¼�id
};                              
                            
//////////////////////�¼����ݽṹ/////////////

///��Ϸ��ʼ�¼�
struct tagBegin
{
	BYTE	byDo;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byNt;		//ׯ��λ��
	BYTE	byPlayCount;// ��Ϸ�غ�
	BYTE	byQuanFeng;	// Ȧ������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	byNo;		//�¼��Ĵ���
	BYTE	byNext;	//��һ���¼��Ĵ���
	BYTE	byNext2;	//��һ���¼��Ĵ��ţ��Ѿ���ׯʱ��
	void Clear()
	{
		byDo	=255;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byNt	=255;		//ׯ��λ��
		byPlayCount	=255;// ��Ϸ�غ�
		byQuanFeng	=255;// Ȧ������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		byNo = THING_DONG_NT;//�¼��Ĵ���
	}
};

///�Զ�Ϊׯ�¼�
struct tagDongNt
{
	BYTE   byDo;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE   byNt;		//ׯ��λ��
	BYTE   byNo;		//�¼��Ĵ���
	BYTE   byNext;	//��һ���¼��Ĵ���
	void Clear()
	{	
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byNt	=255;//ׯ��λ��
		byNo = THING_BEGIN;//�¼��Ĵ���
	}
};

///��2��ɫ�ӵĵ�����Ϊׯ�¼�
struct tagTwoSeziNt
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//��ɫ�����
	BYTE	byNt;//ׯ��λ��
	BYTE	bySezi0;//ɫ��0����
	BYTE	bySezi1;//ɫ��1����
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE	byNo;//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//��ɫ�����
		byNt	=255;//ׯ��λ��
		bySezi0	=255;//ɫ��0����
		bySezi1	=255;//ɫ��1����	
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_NT;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ��ׯ�Һ����Ƶ�λ���¼�
struct tagTwoSeziNtAndGetPai
{
	BYTE   byDo;	//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//��ɫ�����
		byNt	=255;//ׯ��λ��
		bySezi0	=255;//ɫ��0����
		bySezi1	=255;//ɫ��1����
		byGetPai=255;//����λ��
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_NT_GP;//�¼��Ĵ���
	}
};

///������2��ɫ��ȷ��ׯ��
struct tagTurnSeziNt
{

	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byNo = THING_TURN_2SEZI_NT;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ������λ���¼�
struct tagTwoSeziDir
{
	BYTE	byDo;			//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byDo	=255;			//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;		//��ɫ�����
		bySezi0	=255;		//ɫ��0����
		bySezi1	=255;		//ɫ��1����
		byNt = 255;			//ׯ��
		byDirection	=255;	//���Ʒ�λ
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_DIR;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
struct tagTwoSeziDirAndGetPai
{
	BYTE	byDo;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byDo	=255;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;	//��ɫ�����
		bySezi0	=255;	//ɫ��0����
		bySezi1	=255;	//ɫ��1����
		byDirection	=255;//���Ʒ�λ
		byGetPai	=255;	//���Ƶ�
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_DIR_GP;//�¼��Ĵ���
	}
};

///��2��ɫ��ȷ�����ƶ����¼�
struct tagTwoSeziGetPai
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//��ɫ�����
	BYTE	bySezi0;//ɫ��0����
	BYTE	bySezi1;//ɫ��1����
	BYTE	byGetPai;//���Ƶ�
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;	//��ɫ�����
		bySezi0	=255;	//ɫ��0����
		bySezi1	=255;	//ɫ��1����
		byGetPai	=255;	//���Ƶ�
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_GP;//�¼��Ĵ���
	}
};

///�����¼�
struct tagSendPai
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byPaiNum;//ÿ����ҷ�������
	BYTE	byRemainPai;//ʣ������
	BYTE	bySendPai;//��������
	BYTE	byGetPai;//���Ƶ�
	BYTE	byAllPaiNum;//������
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	byStartIndex;///��ʼ����
	BYTE	byEndIndex;  ///��������

	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo		=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byPaiNum	=255;//ÿ����ҷ�������
		byRemainPai	=255;//ʣ������
		bySendPai	=255;//��������
		byGetPai	=255;//���Ƶ�
		byAllPaiNum	=255;//������
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		byStartIndex	=255;///��ʼ����
		byEndIndex	=255;///��������
		byNo = THING_SEND_PAI;//�¼��Ĵ���
	}
};
///�����¼�
struct tagTiaoPai
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//�������λ��
	BYTE	byNt;//ׯ��λ��
	BYTE	byIndex0;//����λ��0
	BYTE	byIndex1;//����λ��1
	BYTE	byRemainPai;//ʣ������
	BYTE	bySendPai;//��������
	BYTE	byGetPai;//���Ƶ�
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo		=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser		=255;//�������λ��
		byNt		=255;//ׯ��λ��
		byIndex0	=255;//����λ��0
		byIndex1	=255;//����λ��1
		byRemainPai	=255;//ʣ������
		bySendPai	=255;//��������
		byGetPai	=255;//���Ƶ�
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		byNo = THING_TIAO_PAI;//�¼��Ĵ���
	}
};
///��ɫ��2��ɫ�Ӷ������¼�
struct tagTwoSeziJing
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//��ɫ�����λ��
	BYTE	byNt;//ׯ��λ��
	BYTE	byFanPai;//���Ƶ�
	BYTE	bySezi0;//ɫ��0����
	BYTE	bySezi1;//ɫ��1����
	BYTE	byMainJing;//������ֵ
	BYTE	byLessJing;//��������ֵ
	BYTE	byStartIndex;///��ʼ����
	BYTE	byEndIndex;  ///��������
	BYTE	byCaiShenIndex; ///��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo		=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser		=255;//��ɫ�����λ��
		byNt		=255;//ׯ��λ��
		byFanPai	=255;//���Ƶ�
		bySezi0		=255;//ɫ��0����
		bySezi1		=255;//ɫ��1����
		byMainJing	=255;//������ֵ
		byLessJing	=255;//��������ֵ
		byStartIndex	=255;///��ʼ����
		byEndIndex	=255;  ///��������
		byCaiShenIndex=255; ///��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_2SEZI_JING;//�¼��Ĵ���
	}
};
///��ɫ��1��ɫ�Ӷ������¼�
struct tagOneSeziJin
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//��ɫ�����λ��
	BYTE	byNt;//ׯ��λ��
	BYTE	byFanPai;//���Ƶ�
	BYTE	bySezi0;//ɫ��0����
	BYTE	byMainJing;//������ֵ
	BYTE	byLessJing;//��������ֵ
	BYTE	byStartIndex;///��ʼ����
	BYTE	byEndIndex;  ///��������
	BYTE	byCaiShenIndex; ///��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	bool	bSeziFinish[4];//��¼�Ѿ�������ɫ�Ӷ��������
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo		=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser		=255;//��ɫ�����λ��
		byNt		=255;//ׯ��λ��
		byFanPai	=255;//���Ƶ�
		bySezi0		=255;//ɫ��0����
		byMainJing	=255;//������ֵ
		byLessJing	=255;//��������ֵ
		byStartIndex	=255;///��ʼ����
		byEndIndex	=255;  ///��������
		byCaiShenIndex=255; ///��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(bSeziFinish,0,sizeof(bSeziFinish));
		byNo = THING_1SEZI_JIN;//�¼��Ĵ���
	}
};

///������Ҳ����¼�
struct tagAllBuHua
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser[4];//��Ҫ�������λ��
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	m_byArHuaPai[4][HUA_CARD_NUM];//��������
	bool	bBuHuaFinish[4];//��¼�Ѿ��Ѿ�������ɵ����
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo = 255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		memset(byUser, 255, sizeof(byUser));	//��Ҫ�������λ��
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//��������	
		memset(bBuHuaFinish,0,sizeof(bBuHuaFinish));
		byNo = THING_ALL_BUHUA;//�¼��Ĵ���
	}
};

///������Ҳ����¼�
struct tagOneBuHua
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byHua	=255;	  //ץ���û���
		byPs	=255;	  //��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//��������
		bFinish=false;
		byNo = THING_ONE_BUHUA;//�¼��Ĵ���
	}
};
///��ʼ����֪ͨ�¼�
struct tagBeginOutPai
{
	BYTE	byDo;	//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;	//�״γ������λ��
	BYTE	byNt;	//ׯ��λ��
	BYTE	byMainJing;//������ֵ
	BYTE	byLessJing;//��������ֵ
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�״γ������λ��
		byNt	=255;//ׯ��λ��
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		byMainJing	=255;//������ֵ
		byLessJing	=255;//��������ֵ
		byNo = THING_BEGIN_OUT;//�¼��Ĵ���
	}
};
///�����¼�
struct tagOutPai
{
	BYTE	byDo;				//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
	bool    bUserTing[4];		//�������״̬
	BYTE	byNo;				//�¼��Ĵ���
	BYTE	byNext;				//��һ���¼��Ĵ���(0������ѭ���¼�����һ���¼����û�����)
	void Clear()
	{
		byDo	=255;				//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;				//�������λ��
		byPs		=255;				//���Ƶ�ֵ
		byMaxAction	=255;		//������������Ķ���
		byMaxStation = 255;		//���������Ķ����������λ��
		memset(byCanAction, 255, sizeof(byCanAction));//������������Ķ���	
		memset(byWantAction, 255, sizeof(byWantAction));//��������Ķ���
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������	
		memset(bUserTing, 0, sizeof(bUserTing));//�������״̬	
		bOtherUse = false;    //�ƴ����������Ҫ��������
		bTing = false;//�Ƿ����Ƴ���
		byNo = THING_OUT_PAI;//�¼��Ĵ���
	}
};
///ץ�����¼�
struct tagZhuaPai
{
	BYTE	byDo;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;		//ץ�����λ��
	BYTE	byPs;		//ץ�Ƶ�ֵ
	BYTE	byStartIndex;///��ʼ����
	BYTE	byEndIndex;  ///��������
	BYTE	byCaiShenIndex; ///��������
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	m_byArHuaPai[4][HUA_CARD_NUM];//��һ���
	BYTE	byGetPaiIndex;//ץ�Ƶ�����
	bool	bHead;		//ץ�Ƶķ���
	bool	bCanOutPai;	//ץ�ƺ��Ƿ���Գ���
	bool    m_bGangKai;	//�Ƿ�ܿ�	
	bool    m_bBuHua;	//�Ƿ�Ҫ����	
	bool    bUserTing[4];		//�������״̬
	BYTE	byNo;		//�¼��Ĵ���
	BYTE	byNext;		//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;		//ץ�����λ��
		byPs		=255;		//ץ�Ƶ�ֵ
		byStartIndex=255;///��ʼ����
		byEndIndex	=255;  ///��������
		byCaiShenIndex=255; ///��������
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));	//����
		byGetPaiIndex = 255;//ץ�Ƶ�����
		bHead = true;
		bCanOutPai = false;//ץ�ƺ��Ƿ���Գ���
		m_bGangKai = false;//�Ƿ�ܿ�	
		m_bBuHua = false;	//�Ƿ�Ҫ����	
		memset(bUserTing, 0, sizeof(bUserTing));//�������״̬	
		byNo = THING_ZHUA_PAI;//�¼��Ĵ���
	}
};
///�����ܺ�֪ͨ�¼��¼�
struct tagCPGNotify
{
	BYTE	byDo;				//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;				//�������λ��
	BYTE	byPs;				//���Ƶ�ֵ
	bool	bZhuaPai;			//�Ƿ��Լ�ץ��
	bool	bChi;				//�Ƿ��ܳ�
	bool	bPeng;				//�Ƿ�����
	bool	bGang;				//�Ƿ��ܸ�
	bool	bKan;				//�Ƿ��ܿ���
	bool	bSaoHu;				//��ǰ�ܷ�ɨ��
	bool	bTing;				//��ǰ�ܷ�����
	bool	bHu;				//�Ƿ��ܺ�
	bool	bQiangGang;			//�Ƿ����ܺ�
	bool    bHaveReturn;		//�ͻ����Ƿ񷵻ص��û�ѡ��
	bool    bCanAction;			//����Ƿ��ж�������
	BYTE	m_iChi[3][3];		//�ܳԵ���
	BYTE	m_iGangData[4][2];	//�ܸܵ���	
	BYTE	m_byTingCanOut[HAND_CARD_NUM];//�����ܴ����
	BYTE    m_byMaxThing;		//��������������
	BYTE	byHuType[MAX_HUPAI_TYPE];	//��������
	BYTE	m_byKan[5];			//��ǰ�ܿ�����
	BYTE    m_iHuPs;			//��Һ�����
	BYTE	m_byDianPao;		//�������λ��
	BYTE	byNo;				//�¼��Ĵ���
	BYTE	byNext;				//��һ���¼��Ĵ���(0������ѭ���¼�����һ���¼����û�����)
	void Clear()
	{
		byDo	=255;			//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;			//�������λ��
		byPs	=255;			//���Ƶ�ֵ
		bZhuaPai=0;				//�Ƿ��Լ�ץ��
		bChi	=0;				//�Ƿ��ܳ�
		bPeng	=0;				//�Ƿ�����
		bGang	=0;				//�Ƿ��ܸ�
		bKan	=0;				//�Ƿ��ܿ���
		bSaoHu  =0;				//��ǰ�ܷ�ɨ��
		bTing	=0;				//��ǰ�ܷ�����
		bHu		=0;				//�Ƿ��ܺ�
		bQiangGang = 0;			//�Ƿ����ܺ�
		bCanAction = false;			//����Ƿ��ж�������
		m_byMaxThing = 0;		//��������������
		bHaveReturn = 0;		//�ͻ����Ƿ񷵻ص��û�ѡ��
		memset(m_iChi,255,sizeof(m_iChi));//�ܳԵ���
		memset(m_iGangData,255,sizeof(m_iGangData));//�ܸܵ���	
		memset(m_byTingCanOut,255,sizeof(m_byTingCanOut));///�����ܴ����
		memset(byHuType,255,sizeof(byHuType));///��������
		memset(m_byKan,255,sizeof(m_byKan));///��ǰ�ܿ�����
		m_iHuPs = 255;///��Һ�����
		m_byDianPao = 255;		//�������λ��
		byNo = THING_OUT_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagChiPai
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byBeChi	=255;//���������λ��
		byType	=255;//���Ƶ����ͣ�0�Դ�1���м䣬2��С�������ƣ�
		byPs		=255;	//�Ե���ֵ
		memset(byChiPs, 255, sizeof(byChiPs));//���Ƶ�����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		byNo = THING_CHI_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagPengPai
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//�������λ��
	BYTE	byBePeng;//���������λ��
	BYTE	byPs;	//������ֵ
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byBePeng	=255;//���������λ��
		byPs		=255;//������ֵ
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		byNo = THING_PENG_PAI;//�¼��Ĵ���
	}
};
///�����¼�
struct tagKanPai
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//�������λ��
	BYTE	byPs;	//������ֵ
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byPs		=255;//������ֵ
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		byNo = THING_KAN_PAI;//�¼��Ĵ���
	}
};
///ɨ���¼����黢�齫ר�ã�
struct tagSaoHu
{
	BYTE	byDo;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;		//���λ��
	BYTE	byPs;		//������ֵ
	bool	bSaohu[4];	//ɨ�������
	bool	bHitSaohu[4];	//�յ����ɨ��
	BYTE    byOutData[4];//����ɨ����ҳ�����
	BYTE    byHandData[4];//����ɨ������������е���
	BYTE    byBaoPai;	//�������λ��
	BYTE	byNo;		//�¼��Ĵ���
	BYTE	byNext;		//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byPs	=255;//������ֵ
		byBaoPai=255;	//�������λ��
		memset(bSaohu, 0, sizeof(bSaohu));//	
		memset(bHitSaohu, 0, sizeof(bHitSaohu));//	
		memset(byOutData, 255, sizeof(byOutData));//	
		memset(byHandData, 255, sizeof(byHandData));//	
		byNo = THING_SAO_HU;//�¼��Ĵ���
	}
};
///�������¼�
struct tagGangPai
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byBeGang	=255;//���������λ��
		byType	=255;//���Ƶ�����
		byPs		=255;	//�ܵ���ֵ
		memset(byGangPai, 255, sizeof(byGangPai));//Ҫ�ܵ�������
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(bHe, 0, sizeof(bHe));
		bChenkQ = false;
		byNo = THING_GANG_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagTingPai
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//�������λ��
	BYTE	byTing;//�������λ��
	BYTE	byPs;	//���Ƴ�����ֵ
	BYTE	byNo;	//�¼��Ĵ���
	bool	bHitTing[4];//��ҵ��������
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byTing	=255;//�������λ��
		byPs		=255;	//���Ƴ�����ֵ	
		memset(bHitTing,0,sizeof(bHitTing));//��ҵ��������
		byNo = THING_TING_PAI;//�¼��Ĵ���
	}
};
///�������¼�
struct tagHuPai
{
	BYTE	byDo;				//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;				//�������λ��
	BYTE	byDianPao;			//�������λ��
	BYTE	byNt;				//ׯ��λ��
	BYTE	byPs;				//���ڵ���ֵ
	BYTE	byMainJing;//������ֵ
	BYTE	byLessJing;//��������ֵ
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE    byHuType[4][MAX_HUPAI_TYPE];//������ҵĺ�������
	BYTE	byUserNum;			//������ҵĸ���
	BYTE	byBaoPaiUser;		//�������
	bool	bCanH[4];			//λ��0-3��Щ��ҵ���˺�
	bool	bDuoXiang;			//�Ƿ�һ�ڶ���
	bool    bZimo;				//�Ƿ�����
	bool    bQiangGang;			//�Ƿ�����
	BYTE	byNo;				//�¼��Ĵ���
	BYTE	byNext;				//��һ���¼��Ĵ���
	void Clear()
	{
		byDo		=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser		=255;//�������λ��
		byDianPao	=255;//�������λ��
		byNt		=255;//ׯ��λ��
		byPs		=255;	//���ڵ���ֵ
		byMainJing	=255;//������ֵ
		byLessJing	=255;//��������ֵ
		byBaoPaiUser=255;		//�������
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(byHuType, 255, sizeof(byHuType));//������ҵĺ�������
		byUserNum = 0;//������ҵĸ���
		memset(bCanH,0,sizeof(bCanH));
		bDuoXiang = false;//�Ƿ�һ�ڶ���
		bZimo = false;   //�Ƿ�����
		bQiangGang = false;   //�Ƿ�����
		byNo = THING_HU_PAI;//�¼��Ĵ���
	}
};
///����¼�
struct tagCountFen
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//�������λ��
	BYTE	byDian;//�������λ��
	BYTE	byNt;//ׯ��λ��
	BYTE	byNextNt;//�¾�ׯ��λ��
	BYTE	byLianZhuang;//��ׯ��
	bool	bHu[4];//��Щ��Һ���
	bool	bCanHu[4];//�������ܺ������
	bool	bZiMo;//�Ƿ�����
	bool    bTest;//�Ƿ�������ݰ�
	__int64 		iZongFen[8];//������ҵ��ܷ�
	__int64 		iMoney[8];//������ҵ��ܷ�
	int		iHuShu[4];//������ҵĺ���
	int 	iHuFen[4];//������ҵĺ��Ʒ�
	BYTE	m_byArHandPai[4][HAND_CARD_NUM];//��������
	BYTE	m_byArOutPai[4][OUT_CARD_NUM];//���Ʋ�����160�ţ�0Ϊ������־
	BYTE    byHuType[4][MAX_HUPAI_TYPE];//������ҵĺ�������	
	bool	bDH;//һ�ڶ���
	BYTE	byPs;	//���ڵ���ֵ
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	void Clear()
	{
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byDian	=255;//�������λ��
		byNt	=255;//ׯ��λ��
		byNextNt=255;//�¾�ׯ��λ��
		byLianZhuang = 0;//��ׯ��
		bZiMo   = false;
		bTest   = false;//�Ƿ�������ݰ�
		memset(bHu,0,sizeof(bHu));
		memset(bCanHu,0,sizeof(bCanHu));
		memset(iZongFen,0,sizeof(iZongFen));
		memset(iHuShu,0,sizeof(iHuShu));
		memset(iMoney,0,sizeof(iMoney));
		memset(iHuFen,0,sizeof(iHuFen));
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(byHuType, 255, sizeof(byHuType));//������ҵĺ�������
		bDH = false;	
		byPs = 255;	//���ڵ���ֵ
		byNo = THING_COUNT_FEN;//�¼��Ĵ���
	}
};
///��Ϸ���������¼�
struct tagEndHandle
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;//�������λ��
	BYTE	byHu;//�������λ��
	BYTE	byNextNt;//�¾�ׯ��λ��
	BYTE	byPs;	//���ڵ���ֵ
	bool	bDH;//һ�ڶ���
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;//��һ���¼��Ĵ���
	tagEndHandle()
	{
		Clear();
	}
	void Clear()
	{		
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		byHu	=255;//�������λ��
		byNextNt=255;//�¾�ׯ��λ��
		byPs		=255;	//���ڵ���ֵ
		bDH = 0;	
		byNo = THING_ENG_HANDLE;//�¼��Ĵ���
	}
};
///���������������¼�
struct tagUnnormalEndHandle
{
	BYTE	byDo;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
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
		byDo	=255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser	=255;//�������λ��
		memset(iFen, 0, sizeof(iFen));
		memset(iMoney, 0, sizeof(iMoney));
		bForce = false;//�Ƿ�ȫ����
		byNo = THING_ENG_UN_NORMAL;//�¼��Ĵ���
	}
};
///////////////////////////////////////////////////


#endif