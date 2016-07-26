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
#define UG_PERSON_SMALL_FIVE        0x05			//��С��
#define UG_HEAVEN_KING				0x04			//������
#define UG_FIVE_SMALL				0x03			//��С��
#define UG_HALF_PAST_TEN			0x02			//ʮ���
#define UG_NORMAL_CARD				0x01			//��ͨ��
#define UG_BUST_CARD				0x00			 //����

//�����߼��� 
class CUpGradeGameLogic
{
	//��������
private:
	//BYTE					m_iNTNum;							//��������
	//BYTE					m_iNTHuaKind;						//���ƻ�ɫ
	int						m_iStation[5];						//���λ�ã����飬÷�������ң����ң����ƣ�

	CPtrList				m_ptrPSList;       //���б�

	DWORD				    m_iCardShape;	  //֧������

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

	//���ܺ���������������
public:

	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK)+1; }
	//��ȡ�˿˻�ɫ
	BYTE GetCardHuaKind(BYTE iCard);
	//�����˿�
	BOOL SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount);
	//ɾ���˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);
	//���Ƶ�
	//int GetCardPoint(BYTE Card[], int CardCount, bool split);

	//��������
	BOOL ReSortCard(BYTE iCardList[], int iCardCount);
	//����==iCard�ĵ�������iCardList�е����(��ʼλ��,���K�cλ��)
	int GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard);

	//��������
public:

	//�õ����л�ɫ
	BYTE GetCardListHua(BYTE iCardList[], int iCardCount);

	//��ȡ�˿���Դ�С
	int GetCardBulk(BYTE iCard, BOOL bExtVol = false);
	//�Աȵ���
	BOOL CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard);

	//��������
	BOOL IsSpecial(BYTE iCardList[],int iCardCount);
	//�Ƿ�ڽܿ�
	BOOL IsBlackJack(BYTE iCardList[], int iCardCount,bool Split);

public:
	//��ȡ�˿�
	BYTE GetCardFromHua(int iHuaKind, int iNum);
	//���ҷ���
	//int FindPoint(BYTE iCardList[], int iCardCount);

	//������ר�ú���
public:
	//�����˿�
	BYTE RandCard(BYTE iCard[], int iCardCount,bool bhaveKing=true);

	//�ͻ��˺���
public:
	//�Զ����ƺ���
	BOOL AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut);
	//�Ƿ���Գ���
	BOOL CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], int iBaseCount, BYTE iHandCard[], int iHandCount);

	//�Ƚ������ƴ�С
	int CompareCard(BYTE iFirstCardList[], int iFirstCardCount,BYTE iFirstUpCard[], BYTE iSecondCardList[], int iSecondCardCount,BYTE iSecondUpCard[]);
	//�Ƚ������ƴ�С
	int CompareCard(BYTE iFirstCardList[], int iFirstCardCount, BYTE iSecondCardList[], int iSecondCardCount);

	//�õ���ǰ����
	BYTE GetShape(BYTE iCardList[],BYTE iCardCount);

	///������Ϸ����
	void SetCardShape(DWORD iCardShape){m_iCardShape=iCardShape;}

	//�ڲ�����
public:
	///��� 0 λ�˿�
	int RemoveNummCard(BYTE iCardList[], int iCardCount);

	///��С��
	bool IsPersonSmallFive(BYTE  iCardList[],int iCardCount);
	///����
	bool IsHeavenKing(BYTE  iCardList[],int iCardCount);
	///��С
	bool IsFiveSmall(BYTE  iCardList[],int iCardCount);
	/// ʮ���
	bool IsHalfPastTen(BYTE  iCardList[],int iCardCount);
	/// ��ͨ��
	bool IsNormalCard(BYTE  iCardList[],int iCardCount);
	//����
	bool IsBustCard(BYTE  iCardList[],int iCardCount);

	/// �õ�������������
	int GetMaxCard(BYTE  iCardList[],int iCardCount);
	//��ȡ�ƵĴ�С
	int GetCardValue(BYTE byCardValue);
	//��ȡ�Ƶĵ���
	int GetPoint(BYTE byCardList[],int iCardNum);

};
