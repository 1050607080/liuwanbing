#include "stdafx.h"
#include "CCheckCPGActionEx.h"

#include "../GameMessage/CSrvLogic.h"

CCheckCPGActionEx::CCheckCPGActionEx(void)
{

}

CCheckCPGActionEx::~CCheckCPGActionEx(void)
{

}

///�ܷ����(�����������ˣ��Ƿ��гԣ�λ���Ƿ�Ե�)
bool CCheckCPGActionEx::CanChi(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData)
{
	bool bRet = CCheckCPGAction::CanChi(station, pai,ResultData);
	return bRet;
}

///�ܷ�����
bool CCheckCPGActionEx::CanPeng(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData)
{
	bool bRet = CCheckCPGAction::CanPeng(station, pai,ResultData);
	return bRet;
}

///�ܷ����
bool CCheckCPGActionEx::CanGang(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData,bool me)
{
	bool bRet = CCheckCPGAction::CanGang(station, pai,ResultData,me);
	return bRet;
}
