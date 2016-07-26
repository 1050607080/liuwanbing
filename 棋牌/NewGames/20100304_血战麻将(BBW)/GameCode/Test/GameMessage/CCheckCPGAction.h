#pragma once

#include "..\ChangeFile\CGameDataEx.h"
#include "..\ChangeFile\CUserDataEx.h"
class CSrvLogic;

///�����ܼ����
class CCheckCPGAction
{
public:
	CCheckCPGAction(void);
	~CCheckCPGAction(void);
	
public:	
		CUserDataEx	*pUserData;//�û�����

		CSrvLogic *pDesk;//���ࣨ���ӣ�
	
public:


	///�ܷ����
	virtual bool CanChi(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData);
	///�ܷ�����
	virtual bool CanPeng(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData);
	///�ܷ����
	virtual bool CanGang(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData,bool me=false);

	//void SetDataPoint(UserDataEx *userdata);

	void SetDeskPoint(CSrvLogic *desk);
};
