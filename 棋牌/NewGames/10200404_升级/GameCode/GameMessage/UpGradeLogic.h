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
	BYTE            m_byMajorFlower;//����ɫ
	BYTE            m_byLevel;//����
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
	//�����Ƹ���
	int GetCardNums(BYTE byCardList[], int iCardCount);
	//�ж����Ƿ�Ϸ�
	bool IsLegalCard(BYTE byCard);
	//��ɫ�Ƿ�Ϸ�
	bool IsLegalFlower(BYTE byFlower);
	//���ü���ͻ�ɫ
	void SetLevelAndFlower(BYTE byLevel,BYTE byMajorFlower);
	//[����]
public:
	//���Ŵ�С���ɴﵽС����
	void SortCard(BYTE byCardArr[],int iCount);
	//��չ����
	BOOL SortCardByEx(BYTE iCardList[], BYTE bUp[], BYTE iCardCount,BOOL bSysSort = FALSE);
	//��ת��˳(�ӵ�->��)
	BOOL ReverseCard(BYTE iCardList[], BYTE bUp[], BYTE iCardCount);
	//����������
	BOOL SortCardByStyle(BYTE iCardList[],BYTE iCardCount);
	//����ɫ����
	BOOL SortCardByKind(BYTE iCardList[],BYTE iCardCount);
	//1.�������ڣ��죬÷��������
	void SortByColor(BYTE byCardArr[],int iCount,vector<BYTE> &vJor,vector<BYTE> &vSpade,vector<BYTE> &vHeart,vector<BYTE> &vClub,vector<BYTE> &vDiamond);
	//2.��ü���
	void GetGameLevelCard(BYTE byLedCardColor,BYTE byCardLevel,vector<BYTE> &vSpade,vector<BYTE> &vHeart,vector<BYTE> &vClub,vector<BYTE> &vDiamond,vector<BYTE>& vOuputVector,BYTE byDeskStation);
public:
	//ɾ���˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);
public:
	//��Ȧ����
	BYTE SinglePK(TFirstData ptcsOutCard[],BYTE byRoundFirstOutCard,int& iRoundScore);	//αʵ��
	//������ɾ��������
	void ReMoveOutCards(BYTE byCardList[],int iCardCount,vector<BYTE> &vCardsInHand);
	//������ͣ� iNums ������;  tcsOutCard-�������
	int GetCardShape(BYTE byCardList[],int iCardCount);
	//�ж��Ƿ���Ƴɹ�
	bool IsOutCardSuccess(TFirstData &tFirstPlayerCard,BYTE byCardList[],int iCardCount,int iShape,vector<BYTE> &vHandleCard);
	//������ʾ��ȡĳ��ɫ����
	bool GetTipDoubleCardForSameFlower(BYTE byCardList[],int iCardCount,BYTE byColor,BYTE byNewList[], int &iNewCount);
	//�������
	void GetMainCard(BYTE byLevelColor,BYTE byLevelCard,vector<BYTE>& vCardsInHand,vector<BYTE>&vMainCard);
	//�Ƿ�Ϊĳָ����˳��
	BOOL IsSequence(BYTE iCardList[], int iCardCount, int iCount);
	//�Ƿ�Ϊͬһ��ɫ
	bool IsSameHuaKind(BYTE iCardList[], int iCardCount);
	//������С��
	BYTE FindMinCard(BYTE byCardList[], int iCardCount);
	//�йܳ���
	void AutoOutCard(TFirstData &tFirstPlayerCard,BYTE byCardList[],int iCardCount,int iRoundOutCardCount,T_CS_OutCard &tOutCard);
	//���ƶ�������
	int GetMajorDoubleNums(BYTE byCardList[],int iCardCount,vector<BYTE> &vMajorDoubleCard);
	//ͬ��ɫ������
	int GetSameFlowerNums(BYTE byCardList[],int iCardCount,BYTE byFlower,vector<BYTE> &vSameFlowerCard);
	//�������
	void GetMajorCard(BYTE byCardList[],int iCardCount,vector<BYTE> &vMajorCard);
	//��ø���
	void GetOtherCard(BYTE byCardList[],int iCardCount,BYTE byResultCard[],int &iResultCount);
	//��������������
	void GetSerialNums(BYTE byCardList[],int iCardCount,int iLen,vector<BYTE> &vSerialCard);
	//�Ƿ�����
	bool IsMajorCard(BYTE byCard);
	//˦��
	bool ThrowCard(vector<BYTE> vHandleCard[],BYTE byCardList[],int iCardCount,BYTE byDeskStation,int &iThrowSthle);
	//�õ���һ������λ��
	BYTE GetNextDeskStation(BYTE byDeskStation);
	//��÷���
	int GetPoint(BYTE byCardList[],int iCardCount);
	//������ͬ�Ƹ���
	int GetSameCardNums(BYTE byCardList[],int iCardCount,BYTE byCard);
	//�����ƱȽϴ�С
	int CompareCard(BYTE byFirstCard,BYTE bySecondCard);
	//����Ƶȼ�(����>С��>������>������>����ɫ��>�������η������ȼ�5,4,3,2,1,0)
	int GetCardLevel(BYTE byCard);
	//��־��¼
	void Debuglog(const char *p, ...);
};