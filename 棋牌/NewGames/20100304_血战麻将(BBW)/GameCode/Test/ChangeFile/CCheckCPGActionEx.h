#pragma once

#include "../GameMessage/CCheckCPGAction.h"
///�����ܼ����(����)
class CCheckCPGActionEx:public CCheckCPGAction
{
public:
	CCheckCPGActionEx(void);
	~CCheckCPGActionEx(void);
	
public:	
	
	
public:
	///�ܷ����
	virtual bool CanChi(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData);
	///�ܷ�����
	virtual bool CanPeng(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData);
	///�ܷ����
	virtual bool CanGang(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData,bool me=false);


};
