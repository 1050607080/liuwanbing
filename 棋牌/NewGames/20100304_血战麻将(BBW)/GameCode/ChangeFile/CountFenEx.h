#pragma once

#include "CountFen.h"
///�¼������ࣨ���أ�
class CountUserFenEx:public CountUserFen
{
public:
	CountUserFenEx(void);
	~CountUserFenEx(void);
	
public:	
	
	int  m_iHuaZhu[PLAY_COUNT];//�����
	int  m_iChaJiao[PLAY_COUNT];//��з�
	int  m_iZhuanYi[PLAY_COUNT];//����ת�Ʒ�

public:
	virtual void CountFen(tagHuPaiEx& verb,tagCountFenEx& CountFen);
	///����ܷ�
	virtual void CountGangFen(int GangFen[]);
};

//ȫ�ֶ���
//extern CountUserFenEx g_CountFen;