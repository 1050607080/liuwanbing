
#pragma once

#include "StdAfx.h"
#include"..\GameMessage\UpgradeMessage.h"


class GameServerData
{
public:
	BYTE     m_AgreePeopleCount;							//׼�����������
	BYTE     m_bBetPeopleCount;									//��ע����
	BYTE     m_bAgreePeople[PLAY_COUNT];					//׼�������
	BYTE     m_bBetPeople[PLAY_COUNT];						//��ע�����
	__int64 i64TotalBet[PLAY_COUNT];						//����ע
	__int64 i64ReadPlayerBet[PLAY_COUNT];					//��ʵ�����ע
	__int64 i64RobotBet[PLAY_COUNT];						//��������ע

public:
	GameServerData(void);

	~GameServerData();
	//��ʼ������
	void InitData();

	//����׼�������
	void SetAgreePeople(int bDeskStation);

	//�õ��������
	BYTE GetPlayerCount();

	//���ׯ��
	BYTE GetRandomNt();

	//�����һ��������
	BYTE GetRandomFirstSendCard();

	//ͳ����ע
	void SetPlayerBet(BYTE bDeskStation,__int64 i64Note,bool flag);
	
};
