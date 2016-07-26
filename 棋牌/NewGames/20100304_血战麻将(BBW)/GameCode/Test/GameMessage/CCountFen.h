#pragma once

#include "..\ChangeFile\CGameDataEx.h"
#include "..\ChangeFile\CUserDataEx.h"
#include "..\ChangeFile\CCheckHuPaiEx.h"

class CSrvLogic;


#define MAX_SHADOW_COUNT  7  //JAdd-20090115
/////////////////�������ͽṹ//////////////////////////////////////

///�����
class CCountFen
{
public:
	CCountFen(void);
	~CCountFen(void);

public:	

	CSrvLogic *pDesk;//���ࣨ���ӣ�

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

	void SetDeskPoint(CSrvLogic *desk);
};
