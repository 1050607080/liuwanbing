#include "StdAfx.h"
#include "GameServerData.h"



GameServerData::GameServerData(void)
{
	InitData();
}

GameServerData::~GameServerData()
{
	;
}
//��ʼ������
void GameServerData::InitData()
{
	m_bBetPeopleCount=0;
	m_AgreePeopleCount=0;									
	memset(m_bAgreePeople,255,sizeof(m_bAgreePeople));
	memset(m_bBetPeople,255,sizeof(m_bBetPeople));
	memset(i64TotalBet,0,sizeof(i64TotalBet));
	memset(i64ReadPlayerBet,0,sizeof(i64ReadPlayerBet));
	memset(i64RobotBet,0,sizeof(i64RobotBet));
}
//����׼�������
void GameServerData::SetAgreePeople(int bDeskStation)
{
	if(bDeskStation>=0)
	{
		m_AgreePeopleCount++;
		m_bAgreePeople[m_AgreePeopleCount-1]=bDeskStation;
	}
}
//�õ��������
BYTE GameServerData::GetPlayerCount()
{
	return m_AgreePeopleCount;
}

//���ׯ��
BYTE GameServerData::GetRandomNt()
{
	srand((unsigned int)time(0));
	int iRandom=rand()%m_AgreePeopleCount;
	return m_bAgreePeople[iRandom];
}
//�����һ��������
BYTE GameServerData::GetRandomFirstSendCard()
{
	srand((unsigned int)time(0));
	int iRandom=rand()%m_bBetPeopleCount;
	return m_bBetPeople[iRandom];
}
//ͳ����ע
void GameServerData::SetPlayerBet(BYTE bDeskStation,__int64 i64Note,bool flag)
{
	if(0==i64TotalBet[bDeskStation])
	{
		m_bBetPeopleCount++;
	}
	i64TotalBet[bDeskStation]+=i64Note;

	if(flag)//��ʵ�����ע
	{
		i64ReadPlayerBet[bDeskStation]+=i64Note;
	}
	else//��������ע
	{
		i64RobotBet[bDeskStation]+=i64Note;
	}
	m_bBetPeople[m_bBetPeopleCount-1]=bDeskStation;
}