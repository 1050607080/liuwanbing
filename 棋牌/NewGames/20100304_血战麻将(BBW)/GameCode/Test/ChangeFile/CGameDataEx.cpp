#include "stdafx.h"
//#include "../Client/stdafx.h"
#include "CGameDataEx.h"


CGameDataEx::CGameDataEx(void)
{
	CGameData::CGameData();
	m_iBaseFen = 0;//��Ϸ�׷�
	m_byMaster = 255;//����
	memset(m_bReBese,0,sizeof(m_bReBese));//�Ƿ��Ӧ�˻���������
	LoadIni();
}

CGameDataEx::~CGameDataEx(void)
{
	CGameData::~CGameData();
}

///���������¼��ĺ���¼�
void CGameDataEx::SetThingNext()
{
	CGameData::SetThingNext();
}

///����ĳ���¼��ĺ���¼�
void CGameDataEx::SetThingNext(BYTE id)
{
	CGameData::SetThingNext(id);
}

//virtual ��ʼ������
void CGameDataEx::InitData()
{
	CGameData::InitData();
	///�����¼����ݰ�
	T_DiFen.Clear();
	//Ѻע�¼����ݰ�
	T_YaZhu.Clear();
}

///��ʼ������
void CGameDataEx::LoadIni()
{
	CGameData::LoadIni();
	m_mjRule.byGamePassNum	= 0;		//��������,����
	m_mjAction.bChi			= 0;	//���ܳ�
	m_mjRule.byAllMjCount   = 110;		//�齫������ 
	m_mjRule.byNextNtType	= 1;		//�¾�ׯ�ҵ�ȷ������
}

///��ʼ��������������
void CGameDataEx::LoadRoomIni(int id)
{
}

//����������
BYTE CGameDataEx::LoadPaiData(MjMenPaiData & menpai)
{
	BYTE index=0;
	memset(menpai.byMenPai,255,sizeof(menpai.byMenPai));//
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<27;j++)
		{
			menpai.byMenPai[index] = stcArr_A_Mj_WTT[j];
			index++;
		}
	}
	for(int i=0;i<2;i++)
	{//2�Űװ�
		menpai.byMenPai[index] = stcArr_A_Mj_ZI[2];
		index++;
	}
	menpai.byAllPaiNum = index;
	return menpai.byAllPaiNum;
}


void  CGameDataEx::ClearBaseFen()//��ջ�������Ϣ
{
	m_iBaseFen = 0;//��Ϸ�׷�
	m_byMaster = 255;//����
	memset(m_bReBese,0,sizeof(m_bReBese));//�Ƿ��Ӧ�˻���������
}
