#include "stdafx.h"
#include "CountFen.h"
#include "../server/ServerManage.h"

CountUserFen::CountUserFen(void)
{

	memset(m_iMoney,0,sizeof(m_iMoney));// ���	
	memset(m_iZongFen,0,sizeof(m_iZongFen));// �ܷ�	
	memset(m_iHuFen,0,sizeof(m_iHuFen));// ���Ʒ�

}

CountUserFen::~CountUserFen(void)
{

}

///���������ҵķ���
void CountUserFen::CountFen(tagHuPaiEx& verb,tagCountFenEx& CountFen)
{
	memset(m_iMoney,0,sizeof(m_iMoney));// ���	
	memset(m_iZongFen,0,sizeof(m_iZongFen));// �ܷ�	
	memset(m_iHuFen,0,sizeof(m_iHuFen));// ���Ʒ�

}
///����ܷ�
void CountUserFen::CountGangFen(int GangFen[])
{

}


//���������ָ��
void CountUserFen::SetDeskPoint(CServerGameDesk *desk)
{
	pDesk = desk;
}