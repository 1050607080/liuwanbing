#pragma once

#include "../Server/StdAfx.h"

//#include "../ChangeFile/UpgradeMessageEx.h"
//#include "..\GameMessage\ThingStruct.h"
//#include "../ChangeFile/GameLogicCUP.h"
#include "..\ChangeFile\CheckHuPaiEx.h"
//#include "UserData.h"

class CServerGameDesk;

#define MAX_SHADOW_COUNT  7  //JAdd-20090115
/////////////////�������ͽṹ//////////////////////////////////////

///�����
class CountUserFen
{
public:
	CountUserFen(void);
	~CountUserFen(void);

public:	

	CServerGameDesk *pDesk;//���ࣨ���ӣ�

	///�Ƿ�һ�ڶ���
	bool	bDuoXiang;
	///�Ƿ�����
	bool	bZimo;
	///���
	int		m_iMoney[4];
	///�ܷ�
	int		m_iZongFen[4];
	///���Ʒ�
	int		m_iHuFen[4];



public:
	///���������ҵķ���
	virtual void CountFen(tagHuPaiEx& verb,tagCountFenEx& CountFen);
	///����ܷ�
	virtual void CountGangFen(int GangFen[]);
	///����Ƿ����ĳ�ֺ�������
	//virtual bool CheckHupaiType(BYTE type,BYTE hupai[]);

	void SetDeskPoint(CServerGameDesk *desk);
};
