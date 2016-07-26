#pragma once
#include "../GameMessage/UpgradeMessage.h"
#pragma pack(push,1)


#define MAIN_VERSION					710		// ���汾��,?��?��
#define ASS_VERSION						15		// �����汾��,?��
         
#ifdef HAOHUA //������
//�ļ����ֶ���
#define GAMENAME						TEXT("�Ƶ���2���齫") 
#define NAME_ID						    20607200  		// ���� ID
#else
//�ļ����ֶ���
#define GAMENAME						TEXT("�Ƶ���2���齫") 
#define NAME_ID						    20607200  		// ���� ID
#endif

static TCHAR szTempStr[MAX_PATH] =		{0};			/** �ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ�� */
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_SKIN_FOLDER(A)				(sprintf(A,"%d",NAME_ID),strcat(A,""))
///////////////////////////////////////////////////////////////////////////////////////////////////


#define ROOM_ID                         1

#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")  // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))          // �ͻ���Ŀ¼
#define SKIN_MJ							TEXT("MjSkin")					   // �齫����ͼƬ�ļ���

// �汾����
#define GAME_MAX_VER					1				// ������߰汾
#define GAME_LESS_VER					1				// ������Ͱ汾
#define GAME_CHANGE_VER					0				// �޸İ汾

#define PLAY_COUNT						2				// ��Ϸ����

#define MAX_HAND_PAI					14				//������


#define EVENT_MAKE_DIFEN				70				//ȷ���׷��¼�
#define EVENT_YA_ZHU					71				//Ѻע�¼�



enum	BUTTON_TYPE			// ��ť����
{
	BUTTON_HE = 0,		//���ư�ť
	BUTTON_GANG,		//����
	BUTTON_PENG,		//����
	BUTTON_CHI,			//����
	BUTTON_GUO,			//����
	BUTTON_TING			//����
};

enum	USER_STATE_TYPE			// ��������
{
	USER_STATE_TING = 0,	//����״̬
	USER_STATE_AUTO,		//�й�״̬
	USER_STATE_GANG,		//��״̬
	USER_STATE_CUT,			//����״̬
	USER_STATE_HAVE_OUT		//�Ѿ�����״̬
};

enum HUPAI_TYPE_EX//LU��������ö�ٽṹ
{	
	HUPAI_HU_PAI			=150,//����
	HUPAI_QING_YI_SE,			//��һɫ
	HUPAI_PENG_PENG_HU,			//������
	HUPAI_QI_DUI,				//�߶�
	HUPAI_HAO_HUA_QI_DUI,		//�����߶�
	HUPAI_TIAN_HU,				//���
	HUPAI_DI_HU,				//�غ�
	HUPAI_QIANG_GANG,			//����
	HUPAI_GANG_KAI,				//�ܿ�
	HUPAI_GANG_PAO,				//�ܺ���
	HUPAI_ZI_MO					//����
};

enum	GAME_STATE			// ��Ϸ״̬
{
	STATE_BEGIN_OUT,	//����ǰ״̬�����������ƣ�������Ϣ
	STATE_OUT_CARD,		//���������Ϣ��ץ����ҵĸܺ���Ϣ
	STATE_BLOCK_CARD,	//����������Ϣ���ͷ�����Ϣ
	STATE_QIANG_GANG,	//�������ƣ�������Ϣ
	STATE_FINISH		//��Ϸ����ׯ���������κ���Ϣ
};

///ð������(�Ӵ�С����)
static void MaoPaoSort(int a[], int count,bool big)
{
	if(a == NULL)
        return ;
	for (int i =0; i < count - 1; i++ )
	{
	    for(int j = 0; j < count - i -1; j++)
		{
			if(big)
			{
				if(a[j] < a[j+1])
				{
					int iTempBig = a[j];
					a[j] = a[j+1];
					a[j+1] = iTempBig;		
				}
			}
			else
			{
				if(a[j] > a[j+1])
				{
					int iTempBig = a[j];
					a[j] = a[j+1];
					a[j+1] = iTempBig;		
				}
			}
		}	
	}
}
///ð������(�Ӵ�С����)
static void MaoPaoSort(BYTE a[], int count,bool big)
{
	if(a == NULL)
        return ;
	for (int i =0; i < count - 1; ++i )
	{
	    for(int j = i+1; j < count; ++j)
		{
			if(big)
			{
				if(a[j] > a[i])
				{
					BYTE iTempBig = a[j];
					a[j] = a[i];
					a[i] = iTempBig;		
				}
			}
			else
			{
				if(a[j] < a[i])
				{
					BYTE iTempBig = a[j];
					a[j] = a[i];
					a[i] = iTempBig;		
				}
			}
		}	
	}
}
//ð������(�Ӵ�С)
static void MaoPaoSort(PaiStruct a[], int count)
{
	if(a == NULL)
        return ;
	PaiStruct iTempBig ;
	for (int i =0; i < count - 1; i++ )
	{
	    for(int j = i; j < count; j++)
		{
			if(a[i].count < a[j].count)
			{
				iTempBig.count= a[i].count;
				iTempBig.pai= a[i].pai;

				a[i].count = a[j].count;
				a[i].pai = a[j].pai;

				a[j].count = iTempBig.count;	
				a[j].pai = iTempBig.pai;	
			}
		}	
	}
}

///�����������ݽṹ
struct GCPStructEx:GCPStruct
{
	GCPStructEx()
	{
		Init();
	}
	void Init()
	{
		GCPStruct::Init();
	};
};

///�¼����ݰ�,���ص�������������Լ�����������
///��Ϸ��ʼ�¼�
struct tagBeginEx:tagBegin
{
	BYTE	m_byMenFeng[PLAY_COUNT];
	void Clear()
	{
		tagBegin::Clear();
		memset(m_byMenFeng,255,sizeof(m_byMenFeng));
	}
};

///�Զ�Ϊׯ�¼�
struct tagDongNtEx:tagDongNt
{
	void Clear()
	{
		tagDongNt::Clear();
	}
};

///��2��ɫ�ӵĵ�����Ϊׯ�¼�
struct tagTwoSeziNtEx:tagTwoSeziNt
{
	void Clear()
	{
		tagTwoSeziNt::Clear();
	}
};

///��2��ɫ��ȷ��ׯ�Һ����Ƶ�λ���¼�
struct tagTwoSeziNtAndGetPaiEx:tagTwoSeziNtAndGetPai
{
	void Clear()
	{
		tagTwoSeziNtAndGetPai::Clear();
	}
};

///������2��ɫ��ȷ��ׯ��
struct tagTurnSeziNtEx:tagTurnSeziNt
{
	void Clear()
	{
		tagTurnSeziNt::Clear();
	}
};

///��2��ɫ��ȷ������λ���¼�
struct tagTwoSeziDirEx:tagTwoSeziDir
{
	void Clear()
	{
		tagTwoSeziDir::Clear();
	}
};

///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
struct tagTwoSeziDirAndGetPaiEx:tagTwoSeziDirAndGetPai
{
	void Clear()
	{
		tagTwoSeziDirAndGetPai::Clear();
	}
};

///��2��ɫ��ȷ�����ƶ����¼�
struct tagTwoSeziGetPaiEx:tagTwoSeziGetPai
{
	void Clear()
	{
		tagTwoSeziGetPai::Clear();
	}
};

///�����¼�
struct tagSendPaiEx:tagSendPai
{
	void Clear()
	{
		tagSendPai::Clear();
	}
};

///��ɫ��2��ɫ�Ӷ������¼�
struct tagTwoSeziJingEx:tagTwoSeziJing
{
	void Clear()
	{
		tagTwoSeziJing::Clear();
	}
};

///������Ҳ����¼�
struct tagAllBuHuaEx:tagAllBuHua
{
	void Clear()
	{
		tagAllBuHua::Clear();
	}
};

///������Ҳ����¼�
struct tagOneBuHuaEx:tagOneBuHua
{
	void Clear()
	{
		tagOneBuHua::Clear();
	}
};
///��ʼ����֪ͨ�¼�
struct tagBeginOutPaiEx:tagBeginOutPai
{

	JingStruct  m_StrJing;//����
	void Clear()
	{
		tagBeginOutPai::Clear();
		m_StrJing.Init();
	}
};
///�����¼�
struct tagOutPaiEx:tagOutPai
{
	void Clear()
	{
		tagOutPai::Clear();
	}
};
///ץ�����¼�
struct tagZhuaPaiEx:tagZhuaPai
{
	JingStruct  m_StrJing;//����
	void Clear()
	{
		tagZhuaPai::Clear();
		m_StrJing.Init();
	}
};
///�����ܺ�֪ͨ�¼��¼�
struct tagCPGNotifyEx:tagCPGNotify
{
	BYTE	m_byFan;//��⵽�ĺ��Ʒ���
	void Clear()
	{
		tagCPGNotify::Clear();
		m_byFan = 0;//��⵽�ĺ��Ʒ���
	}
};
///�������¼�
struct tagChiPaiEx:tagChiPai
{
	GCPStructEx m_UserGCPData[4][5];     //�����ܸ���
	void Clear()
	{
		tagChiPai::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
	}
};
///�������¼�
struct tagPengPaiEx:tagPengPai
{
	GCPStructEx m_UserGCPData[4][5];     //�����ܸ���
	void Clear()
	{
		tagPengPai::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
	}
};

///�������¼�
struct tagGangPaiEx:tagGangPai
{
	GCPStructEx m_UserGCPData[4][5];     //�����ܸ���
	void Clear()
	{
		tagGangPai::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
	}
};
///�������¼�
struct tagTingPaiEx:tagTingPai
{
	void Clear()
	{
		tagTingPai::Clear();
	}
};
///�������¼�
struct tagHuPaiEx:tagHuPai
{
	GCPStructEx m_UserGCPData[4][5];     //�����ܸ���
	void Clear()
	{
		tagHuPai::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
	}
};
///����¼�
struct tagCountFenEx:tagCountFen
{
	int 	iGangFen[4];//������ҵĸܷ�
	GCPStructEx m_UserGCPData[4][5];     //�����ܸ���
	int		iYaZhu[4];//Ѻע�÷���
	//�м�ͬ��Ѻע���
	bool	m_bYaZhu[PLAY_COUNT];
	void Clear()
	{
		memset(iGangFen, 0, sizeof(iGangFen));//������ҵĸܷ�
		tagCountFen::Clear();
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
		memset(iYaZhu, 0, sizeof(iYaZhu));//Ѻע�÷���
		memset(m_bYaZhu, 0, sizeof(m_bYaZhu));//�м�ͬ��Ѻע���

	}
};
///��Ϸ���������¼�
struct tagEndHandleEx:tagEndHandle
{
	void Clear()
	{
		tagEndHandle::Clear();
	}
};
///���������������¼�
struct tagUnnormalEndHandleEx:tagUnnormalEndHandle
{
	void Clear()
	{
		tagUnnormalEndHandle::Clear();
	}
};
///////////////////////////////////////////////////

///ȷ���׷��¼����ݰ�
struct tagFanDiFenEx
{
	int     iBaseFen;//�׷�
	BYTE	byMaster;//����λ��
	BYTE	byType;//0���õ׷���Ϣ��1ͬ��׷���Ϣ
	BYTE	byNo;	//�¼��Ĵ���
	BYTE	byNext;	//��һ���¼��Ĵ���
	void Clear()
	{
		byNo = EVENT_MAKE_DIFEN;//�¼��Ĵ���
		byNext = 255;
	}
};
///Ѻע�¼����ݰ�
struct tagYaZhuEx
{
	BYTE	byNote;//��ǰѺע״̬��255û�л�Ӧ��0,��ͬ�⣬1ͬ��
	BYTE	byNotify;//֪ͨ���ͣ�0֪ͨׯ��Ѻע��1֪ͨ�м�Ѻע��2Ѻע���
	BYTE	byNtNote;//ׯ��Ѻע״̬��255û�л�Ӧ��0��ѹ��1һ�룬2ȫ��
	BYTE	byUserNote[4];//�������Ѻע״̬��255û�л�Ӧ��0,��ͬ�⣬1ͬ��
	BYTE	byNo;		//�¼��Ĵ���
	BYTE	byNext;	//��һ���¼��Ĵ���
	void Clear()
	{
		byNote = 255;//��ǰ��ҵ�Ѻע״̬
		byNotify = 255;
		byNtNote = 255;
		memset(byUserNote, 255, sizeof(byUserNote));//�������Ѻע״̬��255û�л�Ӧ��0,��ͬ�⣬1ͬ��
		byNo = EVENT_YA_ZHU;//�¼��Ĵ���
		byNext = 255;
	}
};


//////////////////�������ݰ�/////////
///�齫��������
struct MJ_SettingEx:MJ_Setting
{
	BYTE		byYaZhuTime;				//Ѻע�ȴ�ʱ��
    MJ_SettingEx()
    {
		Clear();
    }
	void Clear()
	{		
		MJ_Setting::Clear();
		byYaZhuTime = 10;//Ѻע�ȴ�ʱ��
	}
};

///�齫ʱ��ȴ���ʱ�䵽�˷�������Ϊ����
struct MJ_WaitTimeEx:MJ_WaitTime
{
    MJ_WaitTimeEx()
    {
		Clear();
    }
	void Clear()
	{	
		MJ_WaitTime::Clear();
	}
};

///�����������ݰ�(�������)
struct GameStationDataEx:GameStationData
{
    int                 m_nWhichTask;
	bool				bShowUserId;					// �Ƿ���ʾ������ҵ�id
	BYTE				m_iArMenPai[4][MEN_CARD_NUM];	// ���Ʋ�����40��
	JingStruct			m_StrJing;						// ������Ϣ
	GCPStructEx			m_UserGCPData[4][5];			// ��ҳ�������
	MJ_SettingEx        m_MjSet;
	BYTE				m_byMenPaiCount[4];		//����������������
	//��Ϸ�׷�
	int					m_iBaseFen;//��Ϸ�׷�
	BYTE				m_byMaster;//����
	bool				m_bReBese[PLAY_COUNT];//�Ƿ��Ӧ�˻���������
	GameStationDataEx()
	{		
		Init();
	};
	void Init()
	{
		memset(this, 0, sizeof(GameStationDataEx));	
		m_StrJing.Init();
		memset(m_byArHandPai,255,sizeof(m_byArHandPai));
		memset(m_byArOutPai,255,sizeof(m_byArOutPai));
		memset(m_iArMenPai,255,sizeof(m_iArMenPai));
		memset(m_UserGCPData,255,sizeof(m_UserGCPData));
		m_MjSet.Clear();
		memset(m_byMenPaiCount,26,sizeof(m_byMenPaiCount));
		m_iBaseFen = 0;//��Ϸ�׷�
		m_byMaster = 255;//����
		memset(m_bReBese,0,sizeof(m_bReBese));//�Ƿ��Ӧ�˻���������
	}
};


#pragma pack(pop)
