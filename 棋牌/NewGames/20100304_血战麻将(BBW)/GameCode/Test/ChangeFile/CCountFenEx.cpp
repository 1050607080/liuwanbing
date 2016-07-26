#include "stdafx.h"
#include "CCountFenEx.h"
#include "../GameMessage/CSrvLogic.h"


CCountFenEx::CCountFenEx(void)
{
	m_byWiner = 255;//Ӯ��λ��
}

CCountFenEx::~CCountFenEx(void)
{

}

///�����ܷ�
void CCountFenEx::CountFen(tagHuPaiEx& verb,tagCountFenEx& CountFen)
{
	//CCountFen::CountFen(verb,CountFen);

	int base = 1;

	for(int i=0;i<PLAY_COUNT;++i)
	{
		CountFen.m_bYaZhu[i] = pDesk->m_UserData.m_bYaZhu[i];//��ȡѺע��Ӧ���
	}

	if(CountFen.byUser == 255)//û���˺���
	{
		return;
	}
	m_byWiner = CountFen.byUser;//Ӯ��λ��


	char strdug[500];
	for(int i=0;i<PLAY_COUNT;++i)
	{
		sprintf(strdug,"��ֲ��ԣ�-----λ��%d �ܷ�%d ����%d Ѻע��%d �ܷ�%d",i,CountFen.iGangFen[i],CountFen.iHuFen[i],CountFen.iYaZhu[i],CountFen.iZongFen[i]);
		OutputDebugString(strdug);
	}


	int gengfen[4];
	memset(gengfen,0,sizeof(gengfen));
	CountGangFen(gengfen);

	for(int i=0;i<PLAY_COUNT;++i)
	{
		gengfen[i] *= base;
		CountFen.iGangFen[i] = gengfen[i];
	}

	int fan = 0,fen = base;
	bool baopai = false;//���ܰ���
	if(CMjRef::CheckHupaiType(HUPAI_QING_YI_SE,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//��һɫ
	{
		fan++;
	}
	if(CMjRef::CheckHupaiType(HUPAI_PENG_PENG_HU,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//������
	{
		fan++;
	}
	if(CMjRef::CheckHupaiType(HUPAI_QI_DUI,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//�߶�
	{
		fan++;
	}
	if(CMjRef::CheckHupaiType(HUPAI_HAO_HUA_QI_DUI,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//�����߶�
	{
		fan++;
	}
	if(CMjRef::CheckHupaiType(HUPAI_TIAN_HU,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//���
	{
		fan++;
	}
	if(CMjRef::CheckHupaiType(HUPAI_DI_HU,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//�غ�
	{
		fan++;
	}

	if(CMjRef::CheckHupaiType(HUPAI_GANG_KAI,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//�ܿ� 2��
	{
		fan++;
	}
	if(CMjRef::CheckHupaiType(HUPAI_GANG_PAO,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//�ܺ��� 2��
	{
		fan++;
	}
	if(CMjRef::CheckHupaiType(HUPAI_QIANG_GANG,CountFen.byHuType[CountFen.byUser],MAX_HUPAI_TYPE))//���� 2��
	{
		 baopai = true;//���ܰ���
	}

	if(!CountFen.bZimo)//����
	{
	}
	else//����
	{
		fan++;
	}
	//�������ͷ���
	for(int i=0;i<fan;++i)
	{
		fen*=2;
	}

	if(CountFen.bZimo)//������3�Ҹ�
	{
		for(int i=0;i<PLAY_COUNT;++i)
		{
			if(i == CountFen.byUser)
				continue;
			CountFen.iHuFen[CountFen.byUser] += fen;
			CountFen.iHuFen[i] = -fen;
		}
	}
	else//���ڣ������߸�
	{
		if(baopai)//���ܳ�3�ݷ�
		{
			CountFen.iHuFen[CountFen.byUser] = 3*fen;
			CountFen.iHuFen[CountFen.byDian] = -3*fen;
		}
		else
		{
			CountFen.iHuFen[CountFen.byUser] = fen;
			CountFen.iHuFen[CountFen.byDian] = -fen;
		}
	}

	for(int i=0;i<PLAY_COUNT;++i)
	{
		sprintf(strdug,"��ֲ��ԣ�λ��%d �ܷ�%d ����%d Ѻע��%d �ܷ�%d",i,CountFen.iGangFen[i],CountFen.iHuFen[i],CountFen.iYaZhu[i],CountFen.iZongFen[i]);
		OutputDebugString(strdug);
	}

	//����Ѻע
	if(CountFen.byUser == pDesk->m_UserData.m_byNtStation && pDesk->m_UserData.m_byYaZhu>0 && pDesk->m_UserData.m_byYaZhu!=255)
	{//ׯ�Һ��ƣ�����ׯ����Ѻע
		for(int i=0;i<PLAY_COUNT;++i)
		{
			if(i == CountFen.byUser || !pDesk->m_UserData.m_bYaZhu[i])
				continue;
			if(pDesk->m_UserData.m_byYaZhu == 1 )//ׯ��ѹһ��ע
			{
				CountFen.iYaZhu[CountFen.byUser] += -CountFen.iHuFen[i]/2;
				CountFen.iYaZhu[i] = CountFen.iHuFen[i]/2;
			}
			else if(pDesk->m_UserData.m_byYaZhu == 2)//ׯ��ѹȫ��ע
			{
				CountFen.iYaZhu[CountFen.byUser] += -CountFen.iHuFen[i];
				CountFen.iYaZhu[i] = CountFen.iHuFen[i];
			}
		}
	}
	else if(pDesk->m_UserData.m_bYaZhu[CountFen.byUser]  )
	{//�мҺ��Ʋ���ׯ����Ѻע
		if(pDesk->m_UserData.m_byYaZhu == 1 )//ׯ��ѹһ��ע
		{
			CountFen.iYaZhu[CountFen.byUser] = -CountFen.iHuFen[CountFen.byNt]/2;
			CountFen.iYaZhu[CountFen.byNt] = CountFen.iHuFen[CountFen.byNt]/2;
		}
		else if(pDesk->m_UserData.m_byYaZhu == 2)//ׯ��ѹȫ��ע
		{
			CountFen.iYaZhu[CountFen.byUser] = -CountFen.iHuFen[CountFen.byNt];
			CountFen.iYaZhu[CountFen.byNt] = CountFen.iHuFen[CountFen.byNt];
		}
	}
	
	
	//�����ܷ�
	for(int i=0;i<PLAY_COUNT;++i)
	{
		CountFen.iZongFen[i] += CountFen.iGangFen[i];
		CountFen.iZongFen[i] += CountFen.iHuFen[i];
		CountFen.iZongFen[i] += CountFen.iYaZhu[i];
	}
	

	for(int i=0;i<PLAY_COUNT;++i)
	{
		sprintf(strdug,"��ֲ��ԣ�λ��%d �ܷ�%d ����%d Ѻע��%d �ܷ�%d Ѻע��%d",i,CountFen.iGangFen[i],CountFen.iHuFen[i],CountFen.iYaZhu[i],CountFen.iZongFen[i],pDesk->m_UserData.m_byYaZhu);
		OutputDebugString(strdug);
	}
}

///����ܷ�
void CCountFenEx::CountGangFen(int GangFen[])
{
	//CCountFen::CountGangFen(GangFen);

	if(255 == m_byWiner)//�ܷ�ֻ��Ӯ�ҵ�
		return;
	for(int j=0;j<5;++j)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byWiner][j].byType == ACTION_AN_GANG)//���ܣ�3�Ҹ���ÿ��2��
		{
			for(int k=0;k<PLAY_COUNT;++k)
			{
				if(k == m_byWiner)
				{
					GangFen[k] += 2;
				}
				else
				{
					GangFen[k] -= 2;
				}
			}
		}
		else if(pDesk->m_UserData.m_UserGCPData[m_byWiner][j].byType == ACTION_BU_GANG)//��ܣ����ܣ���3�Ҹ���ÿ��1��
		{
			for(int k=0;k<PLAY_COUNT;++k)
			{
				if(k == m_byWiner)
				{
					GangFen[k] += 1;
				}
				else
				{
					GangFen[k] -= 1;
				}
			}
		}
		else if(pDesk->m_UserData.m_UserGCPData[m_byWiner][j].byType == ACTION_MING_GANG)//��ܣ����ܣ�������߸�3�֣������˲���
		{
			GangFen[m_byWiner] += 2;
			GangFen[pDesk->m_UserData.m_UserGCPData[m_byWiner][j].iBeStation] -= 2;
		}
	}


}

///���㾫������
void CCountFenEx::CountJingNum()
{
	//CCountFen::CountJingNum();
}

///���㾫��
void CCountFenEx::CountJingFen(int JingFen[])
{
	//CCountFen::CountJingFen(JingFen);
}


//�������ͻ�ȡ���ƻ�����
void CCountFenEx::GetHuPaiBaseFen(BYTE station,tagCountFenEx& CountFen,int fen[])
{
}






