/*	UpGradeLogic.h
*	�����Ϸ�Ĺ��� 
*	1.�ƣ� �������� ��ɫ ��
*	2.�������
*	3.ׯ�Һ��мҵ�ȷ������
*	4.��Ϸģʽ
*	5.����˼��ʱ��
*	6.���ʣ���ע��������ע
*	7.����
*/

#pragma once

#include <vector>
using namespace std;

extern struct T_CS_OutCard;

//��������
#define	UG_HUA_MASK		0xF0	//1111 0000
#define	UG_VALUE_MASK	0x0F	//0000 1111

//�˿˻�ɫ
#define CT_SPADE		0x00	//����
#define CT_HREART		0x10	//����
#define	CT_CLUB			0x20	//÷��
#define CT_DIAMONS		0x30	//����


//��
// const BYTE Cards[36] =	
// {
// 	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
// 	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39
// };


//��С�����
const int MinMoney = 200;

//ģʽѡ��˼��ʱ��
const short SelectGameModeTime = 15;
//��֪�����Ϸģʽʱ��
const short TellGameModeTime = 15;
//ǿׯ˼��ʱ��
const short BetBankerTime = 15;
//Ͷעʱ��
const short BetTime = 15;

//��ע
const short MinBet = 100;
const short DoubleBet = 2;	//2��
const short TripleBet = 3;	//3��
const short FiveTimesBet = 5;	//5��
const short TenTimesBet = 10;	//10��


//һ������������
const short MaxPlayerInTable = 4;

//����
const short Pay_NOTEN = 1;			//�޴�  no ten
const short Pay_ONETEN = 1;		//�д�  one ten
const short Pay_DOUBLETEN = 2;		//���	double ten
const short Pay_DTENANDDF = 4;	//ͭ�� double ten AND double five
const short Pay_DTSMALLASC = 5;		//С˳��small double ten
const short Pay_DTBIGASC = 6;		//��˳��, big double ten
const short Pay_ALLSUMTEN = 8;		//ʮ�� all sum ten
const short Pay_BOOM = 10;			//������ boom

//����
const enum EnCardShape {
	CS_INVALID = 1,			//��Ч
	CS_NOTEN ,			//�޴�
	CS_ONETEN,			//�д�
	CS_OT_ONE,			//�д�һ��
	CS_OT_TWO,			//�дն���
	CS_OT_THREE,		//�д�����
	CS_OT_FOUR,			//�д��ĵ�
	CS_OT_FIVE,			//�д����
	CS_OT_SIX,			//�д�����
	CS_OT_SEVEN,		//�д��ߵ�
	CS_OT_EIGHT,		//�дհ˵�
	CS_OT_NIGHT,		//�дվŵ�
	CS_DOUBLETEN,		//���
	CS_DTENANDDF,		//ͭ��
	CS_SMALLHORSE,		//С˳��
	CS_BIGHORSE,		//��˳��
	CS_TENHORSE,			//ʮ��
	CS_BOOM			//����

};

//��ɹ���
class CUpGradeGameLogic
{
public:
	CUpGradeGameLogic();
	~CUpGradeGameLogic();	


public:

	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK)+1; }
	//��ȡ�˿˻�ɫ(Ĭ��Ϊ��ʵ��ɫ)
	BYTE GetCardHuaKind(BYTE iCard, BOOL bTrueHua=TRUE);

	//ϴ��
	void RandCard();

	//����
	void DistributeCards(int iDeskStation,BYTE* pCards,int iCardCount);

	//��֤��Ϸ�������Ƿ���Ч
	bool TestData(int iData,int iStyle);


	//////////////////////////////////////////////////////////////////////////
	//�����ж�
	
	//�޴�
	bool NoTen(T_CS_OutCard *pCards);

	//�д�
	int IsOneTen(T_CS_OutCard *pCards);
	//���
	bool IsDoubleTen(const T_CS_OutCard *pCards);
	//ͭ��
	bool IsDoubleTenDoubleFive(const T_CS_OutCard *pCards);
	//С˳��
	bool IsSmallHourse(const T_CS_OutCard *pCards);
	//��˳��
	bool IsBigHourse(const T_CS_OutCard *pCards);
	//ʮ��
	bool isAllSumTen(const T_CS_OutCard *pCards);
	//����
	bool isFourSame(const T_CS_OutCard *pCards);
private:
	vector<BYTE> m_Cards;

};





