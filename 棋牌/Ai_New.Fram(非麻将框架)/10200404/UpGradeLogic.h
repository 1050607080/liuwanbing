#pragma once

//#include "StdAfx.h"

#include <vector>


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

/****************����*****************/
#define CS_SINGLE					10				//����
#define CS_DOUBLE					11				//����
#define CS_MULTIDOUBLE				12				//������
#define CS_MIXED					13				//˦��

//�˿˳�������
#define KING_COUNT								2				//�������ĸ���

class CUpGradeGameLogic
{
	//��������
private:
//	int				m_iStation[5];	   //���λ�ã����飬÷�������ң����ң����ƣ�
	BYTE			m_bSortCardStyle;  //����ʽ
	int				m_iCondition;			//��������
	BOOL			m_bKingCanReplace;		//���Ƿ�ɵ�

	DWORD           m_iCardShape;	//֧������
	//��������
public:
	//���캯��		
	CUpGradeGameLogic(void);
	//��������
	virtual ~CUpGradeGameLogic();

	//���ܺ���������������
public:
	//[�������]
	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK)+1; }
	//��ȡ�˿˻�ɫ(Ĭ��Ϊ��ʵ��ɫ)
	BYTE GetCardHuaKind(BYTE iCard, BOOL bTrueHua=TRUE);
	//��ȡ�˿���Դ�С(Ĭ��Ϊ�ƴ�С,�������С)
	int GetCardBulk(BYTE iCard, bool bExtVal=false);
	//��ȡ�˿���ͨ����Դ�С
	BYTE GetCardByValue(int iCardValue);
	//�O�������Ԯ���
	void SetKingCanReplace(BOOL bKingCanReplace=false){m_bKingCanReplace=bKingCanReplace;}
	//�Cȡ���Ƿ���Ԯ���
	BOOL GetKingCanReplace(){return m_bKingCanReplace;}
	//��������ʽ
	void SetSortCardStyle(BYTE SortCardStyle){m_bSortCardStyle=SortCardStyle;}
	//��ȡ����ʽ
	BYTE GetSortCardStyle(){return m_bSortCardStyle;}

	//��֤��Ϸ�������Ƿ���Ч
	bool TestData(int iData,int iStyle);

	//ϴ��
	BYTE RandCard(BYTE iCard[], int iCardCount,bool bHaveKing);

	//[����]
public:
	//���Ŵ�С���ɴﵽС����
	void SortCard(BYTE byCardArr[],int iCount,BYTE byLedCardColor,BYTE byCardLevel);
		
		//1.�������ڣ��죬÷��������
	void SortByColor(BYTE byCardArr[],int iCount,vector<BYTE> &vJor,vector<BYTE> &vSpade,vector<BYTE> &vHeart,vector<BYTE> &vClub,vector<BYTE> &vDiamond);
		//2.��ü���
	void GetGameLevelCard(BYTE byLedCardColor,BYTE byCardLevel,vector<BYTE> &vSpade,vector<BYTE> &vHeart,vector<BYTE> &vClub,vector<BYTE> &vDiamond,vector<BYTE>& vOuputVector);


public:
	//ɾ���˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);
private:
	//��� 0 λ�˿�
	int RemoveNummCard(BYTE iCardList[], int iCardCount);

public:
	BOOL IsThereLevelCard(BYTE byLevelCard,BYTE* pbyCards,int iCardNum);
	//���ѡ�������Ƿ�Ϸ� // ���������ûʲô�á�
	BOOL IsLegal(vector<BYTE> &vOutCards);
	//��Ȧ����
	BYTE SinglePK(T_CS_OutCard *ptcsOutCard,BYTE byRoundFirstOutCard,BYTE byGameLevel,BYTE byGameLevelColor,int& iRoundScore);	//αʵ��
	//�ж�˦���ǲ������
	bool CheckMixedCardLegal(T_CS_OutCard *ptcsOutCard);	//αʵ��
	//������ɾ������ȥ����
	void RemoveOutCards(BYTE *OriCards,int* iOriNums,BYTE *DelpCards,int iDelNums);
	//������ɾ��������
	void ReMoveOutCards(T_CS_OutCard* ptOutCards,vector<BYTE> &vCardsInHand);
	//��ó��Ƶ����ͣ� iNums ������;  tcsOutCard-�������
	void GetOutCardShape(BYTE *pCards,int iNums,T_CS_OutCard &tcsOutCard,BYTE byDeskStation,BYTE byGameLevelCard,bool isFirst);
	//�ж��Ƿ���Ƴɹ�
	bool IsOutCardSuccess(T_CS_OutCard &tFirstPlayerCard, T_CS_OutCard &tCurrentOutCard,vector<BYTE> pvPlayerCards[],WCHAR* RetMsg,BYTE byGameLevel,BYTE byGameLevelColor);
	//�ж��Ƿ�˦�Ƴɹ�
	bool IsOutMixedCardsSuccess(T_CS_OutCard &tCurrentOutCard,vector<BYTE> pvPlayerCards[],BYTE byMinCards[],int* iCardCount);
	//��ȡĳ��ɫ����
	bool GetSingleCard(BYTE byColor,vector<BYTE>& vHandCards,BYTE* pbyOutCards,int *piOutCount);
	//��ȡĳ��ɫ����
	bool GetDoubleCard(BYTE byColor,vector<BYTE>& vHandCards,BYTE* pbyOutCards,int *piOutCount);
	//��ȡĳ��ɫ������
	bool GetSerialDouble(BYTE byColor,vector<BYTE>& vHandCards,int iPair,BYTE byGameLevelCard,BYTE* pbyOutCards,int *piOutPair);
	//�Ƿ�������
	bool IsMainCard(BYTE byLevelColor , BYTE byLevelCard,BYTE byCompareCard);
	//�������
	void GetMainCard(BYTE byLevelColor,BYTE byLevelCard,vector<BYTE>& vCardsInHand,vector<BYTE>&vMainCard);
};