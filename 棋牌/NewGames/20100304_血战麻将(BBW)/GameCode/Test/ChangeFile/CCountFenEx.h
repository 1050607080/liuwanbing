#pragma once

#include "../GameMessage/CCountFen.h"

///�¼������ࣨ���أ�
class CCountFenEx:public CCountFen
{
public:
	CCountFenEx(void);
	~CCountFenEx(void);
	
public:	
	
	
public:
	virtual void CountFen(tagHuPaiEx& verb,tagCountFenEx& CountFen);
	///����ܷ�
	virtual void CountGangFen(int GangFen[]);
	///���㾫������
	virtual void CountJingNum();
	///���㾫��
	virtual void CountJingFen(int JingFen[]);

	/////����Ƿ����ĳ�ֺ�������
	//virtual bool CheckHupaiType(BYTE type,BYTE hupai[]);

	//�������ͻ�ȡ���ƻ�����
	void GetHuPaiBaseFen(BYTE station,tagCountFenEx& CountFen,int fen[]);

	BYTE m_byWiner;//Ӯ��λ��


};
