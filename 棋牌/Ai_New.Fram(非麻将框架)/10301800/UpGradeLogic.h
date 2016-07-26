#pragma once

//#include "StdAfx.h"

//��������
#define	UG_HUA_MASK					0xF0			//1111 0000
#define	UG_VALUE_MASK				0x0F			//0000 1111

//�˿˻�ɫ
#define UG_FANG_KUAI				0x00			//����	0000 0000
#define UG_MEI_HUA					0x10			//÷��	0001 0000
#define UG_HONG_TAO					0x20			//����	0010 0000
#define UG_HEI_TAO					0x30			//����	0011 0000
#define UG_NT_CARD					0x40			//����	0100 0000
#define UG_ERROR_HUA				0xF0			//����  1111 0000

//�˿˳�������
#define SH_THREE					7				//����
#define SH_SAME_HUA_CONTINUE		6				//ͬ��˳
#ifdef  ZJH_CONTINUE_MORE
#define SH_SAME_HUA					4				//ͬ��
#define SH_CONTINUE					5				//˳��
#else
#define SH_SAME_HUA					5				//ͬ��
#define SH_CONTINUE					4				//˳��
#endif
#define SH_DOUBLE					3				//����
#define SH_OTHER					2				//����
#define SH_SPECIAL					1				//����235
#define UG_ERROR_KIND				0				//����

//�����߼��� 
class CUpGradeGameLogic
{
	//��������
private:

	struct PSTypeNode
	{
		int mps,mcount;

		PSTypeNode()
		{
			mps = -1;
			mcount = 0;
		}
	};


	//��������
public:
	//���캯��
	CUpGradeGameLogic(void);

	void Msg(const char *lpszFormat, ...)
	{
		va_list pArg;
		va_start(pArg, lpszFormat);
		char szMessage[1024] = { 0 };
#ifdef WIN32
		_vsnprintf(szMessage, 1023, lpszFormat, pArg);
#else
		vsnprintf(szMessage, 1023, lpszFormat, pArg);
#endif
		va_end(pArg);
		OutputDebugString(szMessage);
	}

public:
	//��ȡ�˿�����
	int GetCardNum(BYTE iCard);
	//�����˿�
	BYTE RandCard(BYTE iCard[], int iCardCount,int iDeskIndex = 0);

};