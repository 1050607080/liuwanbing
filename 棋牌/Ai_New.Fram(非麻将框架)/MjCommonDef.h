// ***************************************************************
//  SoundsDef   version:  1.0   ��  date: 7/06/2011
//  -------------------------------------------------------------

//	�齫��Ϸ�������ݶ���,�ʺϴ󲿷��齫��Ϸ����ʹ��\

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once

#ifndef MJCommon_DEF_HEAD_FILE

#define TAG_INIT(tagName) \
	tagName(){Clear();} \
	void Clear()

//��Ϸ���ݳ������

#define MJ_MAX_CARDS				136		//�齫����
#define MJ_MAX_WALLCARD				34		//��ǽ�����ɵ��齫�ؼ���
#define MJ_MAX_WALLDUN				17		//��ǽ�����ɵ��齫����(��ֵ��MJ_MAX_WALLCARD��һ��)
#define MJ_MAX_HANDCARD				17		//���������ɵ��齫�ؼ���
#define MJ_MAX_HUA					24		//���Ƶ�����
#define MJ_MAX_OUTED				68		//���ĳ�������
#define MJ_MAX_CGP					7		//����������
#define MJ_MAX_HUTYPE				8		//��������

namespace MjBase
{
	//���ڷ�λ�Ķ���
	namespace MjDirect
	{
		const BYTE East	 = 0;	//��
		const BYTE South = 1;	//��
		const BYTE West	 = 2;	//��
		const BYTE North = 3;	//��
		const BYTE Unknown = 255;	//δ֪����
	}
	
	//�����齫��ֵ�Ķ���
	namespace MjKind
	{
		const BYTE Wan	= 0;	//��
		const BYTE Tiao	= 10;	//��
		const BYTE Bing	= 20;	//��
		const BYTE Feng	= 30;	//��
		const BYTE Hua	= 40;	//�� 
		const BYTE Unknown = 255;	//δ֪����
	}

	namespace MjFeng
	{
		const BYTE Dong	 = 1;	//��
		const BYTE Nan	 = 2;	//��
		const BYTE Xi	 = 3;	//��
		const BYTE Bei	 = 4;	//��
		const BYTE Zhong = 5;	//��
		const BYTE Fa	 = 6;	//��
		const BYTE Bai	 = 7;	//��
	}

	namespace MjHua
	{
		const BYTE Chun	= 1;	//��
		const BYTE Xia	= 2;	//��
		const BYTE Qiu	= 3;	//��
		const BYTE Dong	= 4;	//��
		const BYTE Mei	= 5;	//÷
		const BYTE Lan	= 6;	//��
		const BYTE Zhu	= 7;	//��
		const BYTE Ju	= 8;	//��
	}

	//ͨ����ֵ�����Ƶ�����
	static BYTE GetKind(BYTE byPs)
	{
		if (byPs >= 1 && byPs <= 9)
		{
			return MjKind::Wan;
		}
		else if (byPs >= 11 && byPs <= 19)
		{
			return MjKind::Tiao;
		}
		else if (byPs >= 21 && byPs <= 29)
		{
			return MjKind::Bing;
		}
		else if (byPs >= 31 && byPs <= 37)
		{
			return MjKind::Feng;
		}
		else if (byPs >= 41 && byPs <= 48)
		{
			return MjKind::Hua;
		}

		return MjKind::Unknown;
	}

	//�ж�һ�����Ƿ���Ч
	static bool Verify(BYTE byPs)
	{
		return (MjKind::Unknown != GetKind(byPs));
	}

	//ͨ����ֵ�����Ƶĵ���
	static BYTE GetDian(BYTE byPs)
	{
		return byPs % 10;
	}

	//ȡ�Ƶ���Ϣ������ǽȡ�ƺ󣬽��鷵������ṹ��
	struct TMjFetch
	{
		BYTE val;	//��ֵ
		BYTE from;	//���Ǹ������ǰ���Ƴ�ȡ
		BYTE x;
		BYTE y;
		BYTE left; //��ǽʣ�������

		TMjFetch()
		{
			::memset(this,0,sizeof(TMjFetch));
		}
	};

	//��Ϸ�ж����ı�ʶ��һ����˵�������ȼ���Խ�ߣ�����λԽ��
	namespace MjActFlag
	{
		const USHORT usGuo  = 0x0002L;  //��
		const USHORT usChi  = 0x0004L;  //��
		const USHORT usPeng = 0x0008L;  //��
		const USHORT usGang = 0x0010L;  //��
		const USHORT usTing = 0x0020L;  //��
		const USHORT usHu   = 0x0040L;  //��

		const USHORT usWaitting = 0x0001L;  //�ȴ������Ƚ�λ�����������ǰΪ0��\
									          ��������Ϊ1
	};

	namespace MjChiFlag						//���ݶ����齫����
	{
		const int usT		= 1;			//��ͷ
		const int usZ		= 2;			//����
		const int usW		= 3;			//��β
		const int usTZ		= 4;			//��ͷ ����
		const int usTW		= 5;			//��ͷ ��β
		const int usZW		= 6;			//���� ��β
		const int usTZW		= 7;			//��ͷ ���� ��β
	};

	//��ҵĶ�����ϸ��Ϣ�ṹ////////////////////////////////////////////////////////////////////
	struct TMjActInfo
	{
		USHORT usFlags;	//������ʶ

		/*-�йس���-----------------------------------------------------*/
		BYTE   byChiSels[3];  //���Գ��Ƶ�ѡ��
		int    iChiNums;	  //���Գ���ѡ������
		int	   fgChi;		  //�Ե�״̬(��ͷ ���� ��β  ���ݶ����齫����)

		/*-�йظ���-----------------------------------------------------*/
		BYTE   byGangSels[4]; //���Ըܵ�ѡ��
		int    iGangNums;	  //���Ը�ѡ������
		int    nBuGangInPeng; //�����ڽ�������һ��������

		enum emGangType
		{
			gt_Unknown	= 0,	//δ֪����
			gt_MingGang	= 1,	//����
			gt_BuGang	= 2,	//����
			gt_AnGang	= 3,	//����
			gt_HuaGang	= 4,	//����(�˻��齫����)

		} nGType[4]; //������

		/*-�й�����-----------------------------------------------------*/
		BYTE   byTingSels[MJ_MAX_HANDCARD]; //�����ɴ��������ѡ��
		int    iTingNums; //�����ɴ��������ѡ������

		/*-�йغ���-----------------------------------------------------*/

		int  iHuTypeNums;	//������������

		enum emHuType
		{
			ht_Unknown	= 0,	//δ֪���
			ht_Long		= 1,	//һ����
			ht_HunYiSe	= 2,	//��һɫ
			ht_PengPeng = 3,	//�Զ���
			ht_Dui7		= 4,	//�߶�
			ht_Dui7Bomb = 5,	//ը���߶�
			ht_QingYiSe = 6,	//��һɫ

		} nHuType[8]; //���ͱ�

		enum emHuSpecialType
		{
			hst_Unknown		= 0,  //δ֪���
			hst_QiangGang	= 1,  //����
			hst_GangKai		= 2,  //���Ͽ���
			hst_GangChong	= 3,  //�ܳ�

		} nHuSpecType; //���������

		int  nHuGangIdx;	//���ܿؼ���λ��(��������)

		/*-����-----------------------------------------------------*/

		int nIndexSel; //���������ѡ��

		TAG_INIT(TMjActInfo)
		{
			::memset(this,0,sizeof(TMjActInfo));
		}

		//һЩ����////////////////////////////////////////////////////////////
		public:
		
			//���Ҹ���ѡ�������
			int FindGangIndex(BYTE byCard) const
			{
				for (int i = 0; i < iGangNums; ++i)
				{
					if (byCard == byGangSels[i])
					{
						return i;
					}
				}
				return -1;
			}

			//���Ҹ���ѡ�������
			int FindTingIndex(BYTE byCard) const
			{
				for (int i = 0; i < iTingNums; ++i)
				{
					if (byCard == byTingSels[i])
					{
						return i;
					}
				}
				return -1;
			}

		////////////////////////////////////////////////////////////
	};

	//һ�����ƽ��
	struct TMjCGPNode
	{
		enum emType
		{
			UNKNOWN		= 0,	//δ֪����
			Chi			= 1,	//��
			Peng		= 2,	//��
			MingGang	= 3,	//����
			BuGang		= 4,	//����
			AnGang		= 5,	//����

		} nType; //�������

		BYTE val[4]; //������ֵ
		BYTE from;	//���Ƶ���Դ
		int  nIdx;  //���Ƶ�λ��

		TAG_INIT(TMjCGPNode)
		{
			::memset(this,0,sizeof(TMjCGPNode));
		}
	};

	//�齫��Ϸ�����Ƕ���
	namespace MjResult
	{
		const BYTE Unknown	= 0;  //δ֪���
		const BYTE Huang	= 1;  //��ׯ
		const BYTE FangPao	= 2;  //����
		const BYTE JiePao	= 3;  //����
		const BYTE ZiMo		= 4;  //����
	}

	///���Ƽ����ʱ����
	struct CheckHuStruct
	{
		BYTE  data[MJ_MAX_HANDCARD][2];   //�����ݣ�0��ֵ��1����
		BYTE  count;
		void Add(BYTE pai)
		{
			if(GetPaiCount(pai)>0)
			{
				for(int i=0;i<MJ_MAX_HANDCARD;i++)
				{
					if(data[i][0] == pai && pai != 255)
						data[i][1]++;
				}
			}
			else
			{
				data[count][0] = pai;
				data[count][1] = 1;
				count++;
			}
		};
		BYTE GetPaiCount(BYTE pai)
		{
			for(int i=0;i<MJ_MAX_HANDCARD;i++)
			{
				if(data[i][0] == pai && pai != 255)
					return data[i][1];
			}
			return 0;
		};
		BYTE GetDataCount()
		{
			count = 0;
			for(int i=0;i<MJ_MAX_HANDCARD;i++)
			{
				if(data[i][0] != 255)
					count++;
			}
			return count;
		};
		BYTE GetAllPaiCount()
		{
			BYTE num = 0;
			for(int i=0;i<MJ_MAX_HANDCARD;i++)
			{
				if(data[i][0] != 255)
					num += data[i][1];
			}
			return num;
		};
		void SetPaiCount(BYTE pai,BYTE num)
		{
			for(int i=0;i<MJ_MAX_HANDCARD;i++)
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
			//memset(data[1],0,sizeof(data[1]));
			for(int i=0;i<MJ_MAX_HANDCARD;i++)
			{
				data[i][1] = 0;
			}
			count=0;
		};
		CheckHuStruct()
		{
			Init();
		}
	};
	///ƽ���������ݽṹ
	struct PingHuStruct
	{
		BYTE  byType[6];		//��������
		BYTE  data[6][4];		//����
		bool  bUsedJing[6][4];	//���Ƴ䵱��data�е�λ�ã�trueΪ�о��Ƴ䵱��falseΪû���Ƴ䵱
		BYTE  count;			//�ڵ���
		PingHuStruct()
		{
			Init();
		};
		void AddData(BYTE type,BYTE pai[],bool bJing[4] = NULL)
		{
			for(int i=0;i<6;i++)
			{
				if(byType[i] == 255)
				{
					byType[i] = type;
					data[i][0] = pai[0];
					data[i][1] = pai[1];
					data[i][2] = pai[2];
					data[i][3] = pai[3];
					if (bJing)
					{
						bUsedJing[i][0] = bJing[0];
						bUsedJing[i][1] = bJing[1];
						bUsedJing[i][2] = bJing[2];
						bUsedJing[i][3] = bJing[3];
					}
					count++;
					break;
				}
			}
		};
		void Init()
		{
			memset(this,255,sizeof(PingHuStruct));
			memset(bUsedJing,false,sizeof(bUsedJing));
			count = 0;
		};
	};
}


#define MJCommon_DEF_HEAD_FILE
#endif