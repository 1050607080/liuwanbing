#include "stdafx.h"
#include "CCountFen.h"
#include "CSrvLogic.h"

CCountFen::CCountFen(void)
{

	memset(m_iMoney,0,sizeof(m_iMoney));// ���	
	memset(m_iZongFen,0,sizeof(m_iZongFen));// �ܷ�	
	memset(m_iHuFen,0,sizeof(m_iHuFen));// ���Ʒ�

}

CCountFen::~CCountFen(void)
{

}

///���������ҵķ���
void CCountFen::CountFen(tagHuPaiEx& verb,tagCountFenEx& CountFen)
{
	memset(m_iMoney,0,sizeof(m_iMoney));// ���	
	memset(m_iZongFen,0,sizeof(m_iZongFen));// �ܷ�	
	memset(m_iHuFen,0,sizeof(m_iHuFen));// ���Ʒ�

}
///����ܷ�
void CCountFen::CountGangFen(int GangFen[])
{

}


//��ȡ�����ָ��
void CCountFen::SetDeskPoint(CSrvLogic *desk)
{
	pDesk = desk;
}