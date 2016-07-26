#include "stdafx.h"
#include "CountFenEx.h"
#include "../server/ServerManage.h"
#include "../Server/DebugPrintf.h"
//ȫ�ֶ���
//CountUserFenEx g_CountFen;

CountUserFenEx::CountUserFenEx(void)
{
	memset(m_iHuaZhu,0,sizeof(m_iHuaZhu));//�����
	memset(m_iChaJiao,0,sizeof(m_iChaJiao));//��з�
	memset(m_iZhuanYi,0,sizeof(m_iZhuanYi));//����ת�Ʒ�
}

CountUserFenEx::~CountUserFenEx(void)
{

}

///�����ܷ֣�3������8���׷֣��ⶥ
void CountUserFenEx::CountFen(tagHuPaiEx& verb,tagCountFenEx& CountFen)
{
	//CountUserFen::CountFen(verb,CountFen);	
	//�������
	int iDingFen = 1;
	for(int i=0;i<pDesk->sGameData.m_byFengDing;++i)
	{
		iDingFen *= 2;
	}
	//iDingFen += 1;

	int fan = 0;//����    
	int fen = 1;//����
	int GangFen[4]={0},HuFen[4]={0},ZongFen[4]={0};//�ܷ� //���Ʒ� //�ܷ�
     
	memset(m_iHuaZhu,0,sizeof(m_iHuaZhu));      //�����
	memset(m_iChaJiao,0,sizeof(m_iChaJiao));	//��з�
	memset(m_iZhuanYi,0,sizeof(m_iZhuanYi));	//����ת�Ʒ�

	if(CountFen.byUser == 255)                  //����
	{
		for(int i=0;i<4;i++)
		{
			ZongFen[i] = HuFen[i];
		}
		//�黨���黨���������������Ƶ����Ϊ������������ǻ������3������
		int izhu[PLAY_COUNT];
		memset(izhu,0,sizeof(izhu));
		for (int i=0; i<PLAY_COUNT; ++i)
		{
			if(pDesk->sUserData.m_bIsHu[i])//����Ѿ�����
				continue;
			if(pDesk->sUserData.GetPaiSeCount(i) >= 3)//����
			{
				izhu[i] = 1;
			}
			else
			{
				izhu[i] = 2;
			}
		}
		for (int i=0; i<PLAY_COUNT; ++i)
		{
			if(0 == izhu[i] || izhu[i] == 2)//���ǻ���
				continue;
			for (int j=0; j<PLAY_COUNT; ++j)
			{
				if(izhu[j] == 2)//�ǻ�����һ�û����3��
				{
					CountFen.m_iHuaZhu[i] -= iDingFen;
					CountFen.m_iHuaZhu[j] += iDingFen;
					CountFen.m_bHuaJiao = true;//�Ƿ��л���
				}
			}
		}
		//��У�û�е���ң������ã�������Ƶ�������Ŀ��ܷ�(�ⶥ3��)�����˻����йη��������á�
		BYTE card[17];
		int ijiao[PLAY_COUNT]={0},iFan[PLAY_COUNT]={0};
		for (int i=0; i<PLAY_COUNT; ++i)
		{
			if(pDesk->sUserData.m_bIsHu[i])//����Ѿ�����
				continue;
			if(pDesk->sCheckHuPai.CanTing(i,card))//����
			{
				ijiao[i] = 1;
				if(pDesk->sCheckHuPai.m_byTingMaxFan > iDingFen)
					pDesk->sCheckHuPai.m_byTingMaxFan = iDingFen;
				iFan[i] = pDesk->sCheckHuPai.m_byTingMaxFan;
			}
			else
			{
				ijiao[i] = 2;//û�к�Ҳû�не����
			}
		}
		for (int i=0; i<PLAY_COUNT; ++i)
		{
			if(ijiao[i] != 1 || izhu[i] == 1 )//����û���������,��������
				continue;
			for(int j=0;j<PLAY_COUNT;++j)
			{
				if(j != i && ijiao[j] == 2)//�не���ҵ÷�
				{
					CountFen.m_iChaJiao[i] += iFan[i];
					CountFen.m_iChaJiao[j] -= iFan[i];
					CountFen.m_bHuaJiao = true;//�Ƿ��л���
				}
			}
		}
		for(int i=0;i<4;i++)
		{
			CountFen.m_iHuaJiao[i] = CountFen.m_iHuaZhu[i] + CountFen.m_iChaJiao[i]; 
		}
		//////�˻ظܷ�
		for(int i=0;i<4;i++)
		{
			if(ijiao[i] != 2)//û�к�Ҳû�не����Ҫ�˻ظܷ�
				continue;
			pDesk->sUserData.m_stGangFen[i].Init();//�ܷ�����
		}
		//////

		//CountGangFen(GangFen);
		for(int i=0;i<4;i++)
		{
			CountFen.iZongFen[i] += GangFen[i];//�ܷ�
			CountFen.iZongFen[i] += CountFen.m_iHuaJiao[i];//�黨����з�
			CountFen.iHuFen[i]	 = HuFen[i];//���Ʒ�
			//CountFen.iGangFen[i] = GangFen[i];//�ܷ�
		}

		char log[300];
		for(int i=0;i<4;i++)
		{
			sprintf(log,"��ֲ��ԣ����� ���%d �ܷ� %d ���Ʒ� %d �ܷ�%d ��%d ����%d ���з�%d �ⶥ��%d",i,GangFen[i],HuFen[i],ZongFen[i],CountFen.m_iChaJiao[i],CountFen.m_iHuaZhu[i],CountFen.m_iHuaJiao[i],iDingFen);
			DebugPrintf(log);
			OutputDebugString(log);
		}

		return ;///���ֲ�����������
	}
            
	fan = 0;
	fen = 1;//����
	bool bqidui = false;//�Ƿ��߶Ժ��� �߶Բ���� ȥ��δ�� 2011-11-29
    if(CMjRef::CheckHupaiType(HUPAI_HU_PAI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//�ط�1��
	{
		fan += 0;
	}

    if(CMjRef::CheckHupaiType(HUPAI_AN_QI_DUI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//���߶�
	{
		fan += 2;
		bqidui = true;//�Ƿ��߶Ժ���
	}
	else if(CMjRef::CheckHupaiType(HUPAI_LONG_QI_DUI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//���߶� 1��
	{
		fan += 3;
		bqidui = true;//�Ƿ��߶Ժ���
	}
	else if(CMjRef::CheckHupaiType(HUPAI_QING_QI_DUI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//���߶�
	{
		fan += 3;
		bqidui = true;//�Ƿ��߶Ժ���
	}
	//else if(CMjRef::CheckHupaiType(HUPAI_QING_LONG_QI_DUI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//�����߶� 1��
	//{
	//	fan += 6;
	//	bqidui = true;//�Ƿ��߶Ժ���
	//}
	//else if(CMjRef::CheckHupaiType(HUPAI_QING_DAI_YAO, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//�۾��߶� 8�� 128��
	//{
	//	fan += 8;
	//	bqidui = true;//�Ƿ��߶Ժ���
	//}

	char log[500];
	sprintf(log,"��ֲ��ԣ��������------- �ܷ���%d �ⶥ%d �ⶥ��%d",CountFen.m_iZongFan[CountFen.byUser],pDesk->sGameData.m_byFengDing,iDingFen);
	OutputDebugString(log);

	if(CMjRef::CheckHupaiType(HUPAI_QING_YI_SE, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//��һɫ
	{
		fan += 2;
	}
	if(CMjRef::CheckHupaiType(HUPAI_PENG_PENG_HU, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//������
	{
		fan += 1;
	}
	if(CMjRef::CheckHupaiType(HUPAI_QUAN_DAI_YAO, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//ȫ����
	{
		fan += 2;
	}
	//if(CMjRef::CheckHupaiType(HUPAI_JIANG_DA_DUI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//���ԣ�258��������8
	//{
	//	fan += 3;
	//}
	if(CMjRef::CheckHupaiType(HUPAI_QING_DA_DUI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//��ԣ���һɫ��������8
	{
		fan += 3;
	}
	//if(CMjRef::CheckHupaiType(HUPAI_QING_DAI_YAO, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE))//����� 5�� 16��
	//{
	//	fan += 5;
	//}

	//�ⶥ����Ĭ��10��
	if(fan >= pDesk->sGameData.m_byFengDing)//�ⶥ3��
		fan = pDesk->sGameData.m_byFengDing;
    int nAddFan = 0;

    // �� ���� ������ ���ϻ� ��� �غ�
    if (CMjRef::CheckHupaiType(HUPAI_TYPE_QIANG_GANG, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE) )   // ����
    {
		OutputDebugString("Ѫս�齫���ԣ�����");
        nAddFan += 1;
    }
    if (CMjRef::CheckHupaiType(HUPAI_TYPE_GANG_PAO, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE) )   // ������
    {   
		OutputDebugString("Ѫս�齫���ԣ��¼� ������");
        nAddFan += 1;
    }
    if (CMjRef::CheckHupaiType(HUPAI_TYPE_GANG_KAI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE) )   // ���ϻ�
    {
		OutputDebugString("Ѫս�齫���ԣ����ϻ�");
        nAddFan += 1;//lym 2012-1-3
    }
    if (CMjRef::CheckHupaiType(HUPAI_TYPE_TIAN_HU, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE) )   // ���
    {
		OutputDebugString("Ѫս�齫���ԣ����");
        nAddFan += 4;
    }
    if (CMjRef::CheckHupaiType(HUPAI_TYPE_DI_HU, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE) )   // �غ�
    {
		OutputDebugString("Ѫս�齫���ԣ��غ�");
        nAddFan += 4;
    }

	CountFen.byGengCount = 0;
	if(!bqidui)//�߶Բ���� 2011-11-29
	{
		nAddFan += pDesk->sUserData.m_byGengCount[CountFen.byUser];
		CountFen.byGengCount = pDesk->sUserData.m_byGengCount[CountFen.byUser]; // ��
	}
    fan += nAddFan;//���ϸ��ķ���
	//if(CMjRef::CheckHupaiType(HUPAI_ZI_MO,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//����
	//{
	//	fan+=1;
	//}

	if(fan >= pDesk->sGameData.m_byFengDing)//�ⶥ ��
		fan = pDesk->sGameData.m_byFengDing;

	//�������
	for(int i=0;i<fan;++i)
	{
		fen *= 2;
	}

	if(CMjRef::CheckHupaiType(HUPAI_ZI_MO,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))
	{//���� + 1��
		fen += 1;
	}

	CountFen.m_iZongFan[CountFen.byUser] = fan;
	verb.m_iZongFan[CountFen.byUser] = fan;

	sprintf(log,"��ֲ��ԣ�������� �ܷ���%d �ⶥ%d",CountFen.m_iZongFan[CountFen.byUser],pDesk->sGameData.m_byFengDing);
	OutputDebugString(log);

	bool diankai = false;//�Ƿ��ܸ��Ͽ���
	BYTE dian = 0;
	if (CMjRef::CheckHupaiType(HUPAI_TYPE_GANG_KAI, CountFen.byHuType[ CountFen.byUser ], MAX_HUPAI_TYPE) )   // ���ϻ�
    {//�ҵ������
		for(int i=0;i<5;++i)
		{
			if(pDesk->sUserData.m_UserGCPData[CountFen.byUser][i].byType == 255)
				continue;
			if(pDesk->sUserData.m_UserGCPData[CountFen.byUser][i].byType == ACTION_MING_GANG)
			{
				diankai = true;
				dian = pDesk->sUserData.m_UserGCPData[CountFen.byUser][i].iBeStation;//������
			}
			break;
		}
	}
    // ������ҵ�ʧ
    if (!CountFen.bZiMo && CountFen.byDian!=255)   // ����
    {
        HuFen[ CountFen.byUser ] += fen;
        HuFen[ CountFen.byDian ] -= fen;
    }
	else if(diankai && pDesk->sGameData.m_bChengBao)//��ܸܿ���ֻ�е����Ҹ�
	{//�ǳа��ƣ��Ҽ��� ���ܣ�lym 2012-1-3
		HuFen[ CountFen.byUser ] += fen;
		HuFen[ dian ] -= fen;
	}
    else                                            // ����
    {
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            if (pDesk->sUserData.m_bIsHu[i] || i==CountFen.byUser)
            {
                continue;
            }
            HuFen[CountFen.byUser] += fen;	
            HuFen[i] -= fen;				
        }
    }


//����ת�ƣ��ܺ����ʱ������ת�ƣ������߽���������ת�Ƹ��������ߣ����ܺ����Ϊ��������죬�������Ҫ����ǰ�������÷ָ������ߡ������ڶ��죬����Ҫ�ٸ�һ���ܷ�������ڵķ�ֵ�����������������ơ�
	if(!CountFen.bZiMo && pDesk->sUserData.m_bIsGangPao[CountFen.byDian])//�Ǹܺ���lym 2012-1-3
	{
		int zhuanyi = 0;
		int ihu = 0;

		int count = pDesk->sUserData.m_stGangFen[CountFen.byDian].count;//���i ӵ�еĸ�����
		if(count>0)
		{
			zhuanyi = pDesk->sUserData.m_stGangFen[CountFen.byDian].fen[count-1][CountFen.byDian];//ת�Ƶķ���������������һ�θ������÷����� 
		}
		for(int i=0;i<4;i++)
		{
			if(pDesk->sGameData.T_CountFen.bHu[i])
			{
				CountFen.m_iZhuanYi[i] += zhuanyi;
				CountFen.m_iZhuanYi[CountFen.byDian] -= zhuanyi;//
				CountFen.m_bZhuanYi = true;						// �Ƿ��к���ת��
			}

			sprintf(log,"��ֲ��ԣ����%d ����ת��%d ����%d ת�ƻ�����%d",i,i==CountFen.byDian,CountFen.m_iZhuanYi[i],zhuanyi);
			OutputDebugString(log);
		}

	}
	
	for(int i=0;i<4;i++)
	{
		ZongFen[i] = HuFen[i] + CountFen.m_iZhuanYi[i];
	}

	for(int i=0;i<4;i++)
	{
		CountFen.iZongFen[i] = ZongFen[i];
		CountFen.iHuFen[i] = HuFen[i];
	}

	for(int i=0;i<4;i++)
	{
		sprintf(log,"��ֲ��ԣ����%d ������%d �ܷ� %d ���Ʒ� %d  ����%d �ӷ� %d  ���� %d �ܷ�%d  ��%d �Ѿ���%d",i,pDesk->sGameData.m_iBaseFen,GangFen[i],HuFen[i],CountFen.m_iZongFan[i],nAddFan,CountFen.byGengCount,ZongFen[i],pDesk->sGameData.T_CountFen.bHu[i],pDesk->sUserData.m_bIsHu[i]);
		DebugPrintf(log);
		OutputDebugString(log);
	}
}

///����ܷ�
void CountUserFenEx::CountGangFen(int GangFen[])
{
	char log[800];
	for(int i=0;i<4;i++)
	{
		int count = pDesk->sUserData.m_stGangFen[i].count;//���i ӵ�еĸ�����
		for(int j=0;j<count;j++)
		{
			GangFen[0] += pDesk->sUserData.m_stGangFen[i].fen[j][0];
			GangFen[1] += pDesk->sUserData.m_stGangFen[i].fen[j][1];
			GangFen[2] += pDesk->sUserData.m_stGangFen[i].fen[j][2];
			GangFen[3] += pDesk->sUserData.m_stGangFen[i].fen[j][3];

			sprintf(log,"��ֲ��ԣ�ץ�� �ּܷ��� ���%d ���θܷ�%d %d %d %d %d ����%d",i, pDesk->sUserData.m_stGangFen[i].fen[j][0],pDesk->sUserData.m_stGangFen[i].fen[j][1],pDesk->sUserData.m_stGangFen[i].fen[j][2],pDesk->sUserData.m_stGangFen[i].fen[j][3],pDesk->sUserData.m_stGangFen[i].fen[j][4]);
			OutputDebugString(log);

		}
	}

}
