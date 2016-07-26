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
#define SH_SPECIAL					1				//����235
#define SH_OTHER					2				//����
#define UG_ERROR_KIND				0				//����

//===========================================================================================
//�˿˳�������
#define ZX_DING_HUANG				38				//����		��3+����	18
#define ZX_TIAN_PAI					37				//����		�Ժ�Q		17
#define ZX_DI_PAI					36				//����		�Ժ�2		16
#define ZX_REN_PAI					35				//����		�Ժ�8		15
#define ZX_HE_PAI					34				//����		�Ժ�4		14

#define ZX_MEI_SHI					33				//÷ʮ		�Ժ�10	13	//���
#define ZX_CHANG_SANG				32				//����		�Ժ�6	12
#define ZX_BAN_DENG					31				//���		�Ժ�4	11

#define ZX_HU_TOU					30				//��ͷ		�Ժ�J	10	//���
#define ZX_TIAO_SHI					29				//��ʮ		�Ժ�10	9
#define ZX_MAO_MAO					28				//èè		�Ժ�6	8
#define ZX_KAO_YAO					27				//��ҩ		�Ժ�7	7

#define ZX_DUI_HEI_JIU				26				//�Ժھ�	6			//���
#define ZX_DUI_HEI_BA				25				//�Ժڰ�	5
#define ZX_DUI_HEI_QI				24				//�Ժ���	4
#define ZX_DUI_HEI_WU				23				//�Ժ���	3

#define ZX_NAI_GOU					22				//�̹�		2	
#define ZX_TIAN_GANG				21				//���		1
#define ZX_DI_GANG					20				//�ظ�		0
//---------------------------------------------------------------------------
#define ZX_TIAN_GUAN_JIU			19				//��ؾ�
#define ZX_DI_GUAN_JIU				18				//�عؾ�
#define	ZX_DENG_LONG_JIU			17				//������
#define	ZX_HE_WU_JIU				16				//�����
#define ZX_BAN_WU_JIU				15				//�����	���������ʹ�С��ͬ
#define ZX_DING_CHANG_JIU			14				//������
#define ZX_MEI_SHI_JIU				13				//÷ʮ��	
#define ZX_DING_MAO_JIU				12				//��è��    10	���������ʹ�С��ͬ
#define ZX_WU_LONG_JIU				11				//������	10
#define ZX_TIAO_SHI_JIU				10				//��ʮ��	10
#define ZX_NINE_POINT				9				//�ŵ�
#define ZX_EIGHT_POINT				8				//�˵�
#define ZX_SEVEN_POINT				7				//�ߵ�
#define ZX_SIX_POINT				6				//����
#define ZX_FIVE_POINT				5				//���
#define ZX_FOUR_POINT				4				//�ĵ�
#define ZX_THREE_POINT				3				//����
#define ZX_TWO_POINT				2				//����
#define ZX_ONE_POINT				1				//һ��
#define ZX_ZERO_POINT				0				//���

//------------------------------------------------------------------------
#define RESULT_BIG					1				//�ȽϽ����
#define RESULT_EQU					0				//�ȽϽ�����
#define RESULT_SMA					-1				//�ȽϽ�����
		
//===========================================================================================

//�����߼��� 
class CUpGradeGameLogic
{
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
	//���ܺ���������������
public:
	inline int GetCardNum(BYTE iCard){ return (iCard&UG_VALUE_MASK)+1; }				   //��ȡ�˿�����
	BYTE GetCardHuaKind(BYTE iCard);													   //��ȡ�˿˻�ɫ
	BOOL SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount);						   //�����˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);//ɾ���˿�
	BOOL ReSortCard(BYTE iCardList[], int iCardCount);										//��������
public:
	int GetCardBulk(BYTE iCard, BOOL bExtVol);												//��ȡ�˿���Դ�С
public:
	//�Զ����ƺ���
	BOOL AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut);
	//�Ƚ������ƴ�С
	int CompareCard(BYTE iFirstCardList[], int iFirstCardCount, BYTE iSecondCardList[], int iSecondCardCount);
		int RemoveNummCard(BYTE iCardList[], int iCardCount);						//��� 0 λ�˿�
		BYTE RandCard(BYTE iCard[], int iCardCount,int iDeskIndex = 0);				//�����˿�(ϴ��)
public:
		bool IsRedThreeAndBigKing(BYTE byCardList[], int iCardCount);				//����Ƿ��Ƕ���
		bool IsDoubleRedQ(BYTE byCardList[], int iCardCount);						//����Ƿ�������
		bool IsDoubleRedTwo(BYTE byCardList[], int iCardCount);						//����Ƿ��ǵ���
		bool IsDoubleRedEight(BYTE byCardList[], int iCardCount);					//����Ƿ�������
		bool IsDoubleRedFour(BYTE byCardList[], int iCardCount);					//����Ƿ��Ǻ���
		bool IsDoubleBlackTen(BYTE byCardList[], int iCardCount);					//����Ƿ���÷ʮ
		bool IsDoubleBlackFour(BYTE byCardList[], int iCardCount);					//����Ƿ��ǰ��
		bool IsDoubleBlackSix(BYTE byCardList[], int iCardCount);					//����Ƿ��ǳ���
		bool IsDoubleBlackJ(BYTE byCardList[], int iCardCount);						//����Ƿ��ǻ�ͷ
		bool IsDoubleRedTen(BYTE byCardList[], int iCardCount);						//����Ƿ�����ʮ
		bool IsDoubleRedSix(BYTE byCardList[], int iCardCount);						//����Ƿ���èè
		bool IsDoubleRedSeven(BYTE byCardList[], int iCardCount);					//����Ƿ��Ǹ�ҩ
		bool IsDoubleBlackNine(BYTE byCardList[], int iCardCount);					//����Ƿ��Ƕ���(��9)
		bool IsDoubleBlackEight(BYTE byCardList[], int iCardCount);					//����Ƿ��Ƕ���(��8)
		bool IsDoubleBlackSeven(BYTE byCardList[], int iCardCount);					//����Ƿ��Ƕ���(��7)
		bool IsDoubleBlackFive(BYTE byCardList[], int iCardCount);					//����Ƿ��Ƕ���(��5)
		bool IsRedQAdnNine(BYTE byCardList[], int iCardCount);						//����Ƿ����̹�
		bool IsRedQAdnEight(BYTE byCardList[], int iCardCount);						//����Ƿ������
		bool IsRedTwoAndEight(BYTE byCardList[], int iCardCount);					//����Ƿ��ǵظ�
		bool IsRedQAndSeven(BYTE byCardList[], int iCardCount);						//����Ƿ�����ؾ�
		bool IsRedTwoAndSeven(BYTE byCardList[], int iCardCount);					//����Ƿ��ǵعؾ�
		bool IsRedEightAndJ(BYTE byCardList[], int iCardCount);						//����Ƿ��ǵ�����
		bool IsRedFourAndFive(BYTE byCardList[], int iCardCount);					//����Ƿ��Ǻ����
		bool IsBlackFourAndFive(BYTE byCardList[], int iCardCount);					//����Ƿ��ǰ����
		bool IsRedThreeAndBlackSix(BYTE byCardList[], int iCardCount);				//����Ƿ��Ƕ�����
		bool IsBlackTenAndNine(BYTE byCardList[], int iCardCount);					//����Ƿ���÷ʮ��
		bool IsRedThreeAndRedSix(BYTE byCardList[], int iCardCount);				//����Ƿ��Ƕ�è��
		bool IsBlackEightAndJ(BYTE byCardList[], int iCardCount);					//����Ƿ���������
		bool IsRedTenAndNine(BYTE byCardList[], int iCardCount);					//����Ƿ�����ʮ��

		bool IsThreeAndBigKingAndDoubleRedQ(BYTE byCardList[], int iCardCount);		//���Ƿ������
		bool IsThreeAndBigKingAndDoubleRedTwo(BYTE byCardList[], int iCardCount);	//���Ƿ���Ѽ��
		bool IsRedTenAndBlackTenAndJ(BYTE byCardList[], int iCardCount);			//���Ƿ�������ʮ
		bool IsRedSixAndBlackSixAndBigKing(BYTE byCardList[], int iCardCount);		//���Ƿ���������
//------------------------------------------------------------------------------------------------------
public:
		int  GetCardValue(BYTE byCard);												//��ȡ��ֵ
		int  GetCardShape(BYTE iCardList[], int iCardCount);						//��ȡ�Ƶ�����
		int  CalculationCardPoint(BYTE byCardList[],int iCardNum);					//���������Ƶĵ���
		int  CompareTwoCardHand(BYTE byFirstCardList[],int iFirstCardCount,BYTE bySecondCardList[],int iSecondCardCount);	//�Ƚ������ƵĴ�С
		BYTE TransformCardValue(BYTE byCard);																				//ת���ɿɱȵ��ƴ�С
		BYTE GetMaxCardValue(BYTE byCardList[],int iCardCount);																//���������л�ȡ������ֵ

		int GetCmpCardValue(BYTE byCardList[],int iCardCount);						//��ȡ�������Ϣ
};