/******************************************************************************************
*����˵����������Ϸ�߼�������                                                             *
*����˵������Ϸ�߼�������Ϸ�㷨����ÿ�ֲ�ͬ����Ϸ���в�ͬ����Ϸ�߼��������߿ɽ�       *
*��Ϸ�߼������ڸ�����ʵ�֡�                                                               *
*����ʱ�䣺2009.02.06                                                                     *
*�޸�ʱ�䣺                                                                               *
*���ߣ�   qy                                                                              *
*�޸��ߣ�                                                                                 *
******************************************************************************************/

#pragma once
#include "UI_ForLogic/IUserInterface.h"

// �Ƶĸ���
const BYTE  POKER_SET_NUM  = 1;         /// �˿��Ƶĸ���
const BYTE  POKER_CARD_NUM = 54;        /// �˿��Ƶ�����
const BYTE  POKER_COUNT_SHAPES = 5;     /// 0(��С��) 1(��) 2(��) 3(÷) 4(��) ���ֻ�ɫ
const BYTE  POKER_COUNT_CARDNUM=16;     /// 0 A 2 3 ... J Q K С�� ����������16����(0��ʾ�Ʊ�)
const BYTE  POKER_CARD_NONE = 0x00;     /// ����
const int   SINGLE_CARD_COUNT = 1;      /// ����

const HRESULT RET_SUCCESS = 0;          /// �ɹ�
const HRESULT RET_FAILED = -1;          /// ʧ��
#define SUCCESSED(hRet) (RET_SUCCESS == (HRESULT)hRet)

// ����һ����
#define MAKE_A_CARD(byPkSet,byCardShape,byCardNumber)  ((byPkSet&0x0F)|((byCardShape<<4)&0xF0)|(byCardNumber&0x0F))

/// �Ƶĸ���
typedef enum ePokerSetNumber
{
	EPSN_SET_NUM_NONE = 0,		/// ���� 
	EPSN_SET_NUM_1,			    /// ��һ��
	EPSN_SET_NUM_2              /// �ڶ���
}E_POKER_SET_NUMBER;

// �ƵĻ�ɫ
typedef enum _eCardShape
{
	_ECS_SHAPE_NONE = 0,      /// ��С�����޻�ɫ
	_ECS_SHAPE_DIAMONDS=1,    /// ����
	_ECS_SHAPE_CLUBS=2,       /// ÷��
	_ECS_SHAPE_HEARTS=3,      /// ����
	_ECS_SHAPE_SPADE=4        /// ����
}_E_CARD_SHAPE;

 //�Ƶĵ���
typedef enum _eCardNumber
{
	_ECN_NUM_NONE = 0,		// �Ʊ� 
	_ECN_NUM_A,			    // A	
	_ECN_NUM_2,
	_ECN_NUM_3,
	_ECN_NUM_4,
	_ECN_NUM_5,
	_ECN_NUM_6,
	_ECN_NUM_7,
	_ECN_NUM_8,
	_ECN_NUM_9,
	_ECN_NUM_10,
	_ECN_NUM_J,
	_ECN_NUM_Q,
	_ECN_NUM_K,
	_ECN_NUM_Joker,	       // С��
	_ECN_NUM_JOKER	       // ����
}_E_CARD_NUMBER;

/// ��������
typedef enum eBaseCardsType
{ 
	EBCT_TYPE_NONE = 0,				    /// ������
	//ţţ����
	EBCT_TYPE_NO_POINT	= 1000,							///�޵���
	EBCT_TYPE_BULL_ONE,                            ///ţһ
	EBCT_TYPE_BULL_TWO,                             ///ţ��
	EBCT_TYPE_BULL_THREE,                           ///ţ��
	EBCT_TYPE_BULL_FOUR,                            ///ţ��
	EBCT_TYPE_BULL_FIVE,                            ///ţ��
	EBCT_TYPE_BULL_SIX,                             ///ţ��
	EBCT_TYPE_BULL_SEVEN,                           ///ţ��
	EBCT_TYPE_BULL_EIGHT,                           ///ţ��
	EBCT_TYPE_BULL_NINE,                            ///ţ��
    EBCT_TYPE_BULL_joker,                           ///С��ţţ
	EBCT_TYPE_BULL_JOKER,                           /// ����ţţ
	EBCT_TYPE_BULL_BULL							///ţţ
	// ����������չ��������
}E_BASE_CARDS_TYPE;

/// �ȽϽ��
typedef enum eCompareResult
{
	ECR_OTHER = -2,         /// �޷��Ƚ�
	ECR_LESS  = -1,         /// С��
	ECR_EQUAL = 0,          /// ����
	ECR_MORE = 1            /// ����
}E_COMPARE_RESULT;

/// �ȽϽ��
typedef enum eCardSortType
{
	ECST_LEVEL   = 0,       /// Ȩֵ����
	ECST_NUMBER  = 1,       /// �Ƶ�����
	ECST_CARDCOUN= 2,       /// ��ͬ�Ƶ����������
	ECST_SHAPE   = 3        /// ��ɫ����
}E_CARD_SORT_TYPE;

/// ������Ϸ�߼�
class CPokerGameLogic
{
public:
	CPokerGameLogic();
	virtual ~CPokerGameLogic();

private:
	bool  m_bCompareKind ; 

public:																									

	/********************************************************************************
	* ������: static char Card2Char(char shape,char number)
	* ����;   �������ڼ�������ɫ���Ƶ����ת��Ϊ�ַ� ��ʽ��
	*         (((byPkSet<<8)&0xFF00)|((byCardShape<<4)&0x00F0)|(byCardNumber&0x000F))
	* @param: BYTE byPkSet [in] Ϊ�ڼ�����
	* @param: BYTE byShape [in] �����Ļ�ɫ 
	* @param: BYTE byNumber[in] �������Ƶ�
	* @param: ת����Ľ�� 
	********************************************************************************/
	static BYTE CardToWord(BYTE byPkSet, BYTE byShape, BYTE byNumber);

	/********************************************************************************
	* ������: static bool WordToCard(WORD wInCard, BYTE &byPkSet, BYTE &byShape, BYTE &byNumber)
	* ����;   ����������ת��Ϊ�Ƶĵڼ�������ɫ���Ƶ�
	* @param: WORD wInCard  [in]  ������Ҫת������
	* @param: BYTE &byPkSet [out] Ϊ�ڼ�����
	* @param: BYTE &byShape [out] �����Ļ�ɫ 
	* @param: BYTE &byNumber[out] �������Ƶ� 
	* ����: bool ת���ɹ�true��ʧ��false 
	********************************************************************************/
	static bool WordToCard(BYTE wInCard, BYTE &byPkSet, BYTE &byShape, BYTE &byNumber);

	/********************************************************************************
	* ������: static BYTE GetCardSet(WORD wCard)
	* ����;   ��ȡ��Ϊ�ڼ������е���
	* ����: WORD wCard  [in]  ������
	* ����: BYTE �ڼ�����
	********************************************************************************/
	static BYTE GetCardSet(BYTE wCard);

	/********************************************************************************
	* ������: static BYTE GetCardShape(WORD wCard)
	* ����;   ��ȡ�ƵĻ�ɫ
	* ����: WORD wCard  [in]  ������
	* ����: BYTE �ƵĻ�ɫ
	********************************************************************************/
	static BYTE GetCardShape(BYTE wCard);

	/********************************************************************************
	* ������: static BYTE GetCardNumber(WORD wCard)
	* ����;   �����Ƶ�
	* ����: WORD wCard  [in]  ������
	* ����: BYTE �Ƶ�
	********************************************************************************/
	static BYTE GetCardNumber(BYTE wCard);


	/********************************************************************************
	* ������: static BYTE GetCardNumber(WORD wCard)
	* ����;   ������ֵ(��ɫ<<4|�Ƶ�)
	* ����: WORD wCard  [in]  ������
	* ����: BYTE ��ֵ
	********************************************************************************/
	//static BYTE GetCardValue(BYTE wCard);

	/********************************************************************************
	* ������:static bool IsAvailableCard(WORD wCard)
	* ����;   �������Ƿ�Ϊ��Ч
	* ����: WORD wCard [in]  ��(�ڼ�������ɫ���Ƶ����)
	* ����: bool �Ϸ����ƣ�����true�����򷵻�false
	********************************************************************************/
	static bool IsAvailableCard(BYTE wCard);

	/********************************************************************************
	* ������:static bool IsAvailableCardValue(BYTE byCardValue)
	* ����;   ������ֵ�Ƿ�Ϊ��Ч
	* ����: BYTE byCardValue [in]  ��ֵ(��ɫ���Ƶ����)
	* ����: bool �Ϸ�����ֵ������true�����򷵻�false
	********************************************************************************/
	static bool IsAvailableCardValue(BYTE byCardValue);

	/********************************************************************************
	* ������:static bool IsAvailableCardSet(BYTE byCardSet)
	* ����;   �����Ƹ��Ƿ�Ϊ��Ч
	* ����: BYTE byCardSet [in]  �ڼ���
	* ����: bool �Ϸ����ƣ�����true�����򷵻�false
	********************************************************************************/
	static bool IsAvailableCardSet(BYTE byCardSet);

	/********************************************************************************
	* ������: static bool IsAvailableCardShape(BYTE byCardShape)
	* ����;   �����ƻ�ɫ�Ƿ�Ϊ��Ч
	* ����: BYTE byCardShape [in]  �ƻ�ɫ
	* ����: bool �Ϸ����ƻ�ɫ������true�����򷵻�false
	********************************************************************************/
	static bool IsAvailableCardShape(BYTE byCardShape);

	/********************************************************************************
	* ������: static bool IsAvailableCardNumber(BYTE byCardNumber)
	* ����;   �����Ƶ��Ƿ�Ϊ��Ч
	* ����: BYTE byCardNumber [in]  �Ƶ�
	* ����: bool �Ϸ����Ƶ㣬����true�����򷵻�false
	********************************************************************************/
	static bool IsAvailableCardNumber(BYTE byCardNumber);	
	
	/********************************************************************************
	* ������: int RemoveCard(WORD wRemoveCard[], int iRemoveCount, WORD wCardList[], int iCardCount)
	* ����;   ɾ���˿�
	* ����: WORD wRemoveCard[]  [in]    ��Ҫɾ�����˿�
	*       int iRemoveCount  [in]      ɾ���������С
	*       BYTE wCardList[]  [in/out]  �Ӹ���ֵ��ɾ���˿�
	*       int iCardCount    [in]      ɾ����ԭ����Ĵ�С  
	* ����: int ɾ���Ƶ�����
	********************************************************************************/
	int RemoveCard(BYTE wRemoveCard[], int iRemoveCount, BYTE wCardList[], int iCardCount);

	/********************************************************************************
	* ������: static int CardCompare(WORD wCardFirst, WORD wCardSecond)
	* ����;   �����ƽ��бȽ�
	* ����: WORD wCardFirst  [in]   ��һ����
	*       WORD wCardSecond [in]   �ڶ����� 
	* ����: int ����E_COMPARE_RESULT ��һ����<�ڶ��� ECR_LESS
	********************************************************************************/
	static int CardCompare(BYTE wCardFirst, BYTE wCardSecond);

	/********************************************************************************
	* ������: static HRESULT RandCard(WORD wCardList[], int iCardCount)
	* ����;   ϴ��
	* ����: WORD wCardList[]  [out]     ϴ�õ���
	*       int iCardCount    [in]      �ܹ�����
	* ����: HRESULT 0 �ɹ�������ʧ��
	********************************************************************************/
	static BYTE	RandCard(BYTE iCard[], int iCardCount,bool bhaveKing=true);

	/********************************************************************************
	* ������: static HRESULT FindCardIndex(int &iIndex, WORD wFindCard, WORD wCardList[], int iCardCount);
	* ����;   ����ĳ�����ڶ����е�����ֵ
	* ����: int  &iIndex      [out]  ���ҵ�������ֵ (δ���ҵ�Ϊ-1)
	*       WORD wFindCard    [in]   ��Ҫ���ҵ���
	*       WORD wCardList[]  [in]   �ƶ���
	*       int iCardCount    [in]   �ܹ�����
	* ����: HRESULT 0 �ɹ�������ʧ��
	********************************************************************************/
	static HRESULT FindCardIndex(int &iIndex, BYTE wFindCard, BYTE wCardList[], int iCardCount);

	


public:
	/********************************************************************************
	* ������: BYTE GetCardLevel(WORD wCard)
	* ����;   �����Ƶ�Ȩֵ
	* ����: WORD wCard  [in]  ������
	* ����: BYTE �Ƶ�Ȩֵ
	********************************************************************************/
	 BYTE GetCardLevel(BYTE wCard);

	/********************************************************************************
	* ������: bool SortByLevel(WORD wCardList[], int iCardCount,bool bAscending=false)
	* ����;   Ȩֵ����
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortByLevel(BYTE wCardList[], int iCardCount,bool bAscending=false);

	/********************************************************************************
	* ������: bool SortByNumber(WORD wCardList[], int iCardCount,bool bAscending=false)
	* ����;   �Ƶ�����
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortByNumber(BYTE wCardList[], int iCardCount,bool bAscending=false);

	/********************************************************************************
	* ������: bool SortByShape(WORD wCardList[], int iCardCount,bool bAscending=false)
	* ����;   �ƵĻ�ɫ����
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortByShape(BYTE wCardList[], int iCardCount,bool bAscending=false);

	/********************************************************************************
	* ������: bool SortByCardCount(WORD wCardList[], int iCardCount,bool bAscending=false)
	* ����;   �Ƶ����������
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortByCardCount(BYTE wCardList[], int iCardCount,bool bAscending=false);

	/********************************************************************************
	* ������: bool SortCards(WORD wCardList[], int iCardCount, int iMod, bool bAscending)
	* ����;   �����˿�
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       int iMod          [in]      ����ģʽ 0��ֵ��С��1��������2�ƵĻ�ɫ
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortCards(BYTE wCardList[], int iCardCount, int iMod, bool bAscending=false);

	///��� 0 λ�˿�
	int RemoveNummCard(BYTE wCardList[], int iCardCount);
	///�õ��Ƶ�ֵţţ�еĵ�
	int GetPoint(BYTE wCard);
// 	�õ���ֵ
// 	int GetCardPoint(BYTE wCardList[],int iCardCount);

	///�õ������������ƣ�ѡ��ţ��
	//int GetShape(WORD wCardList[],int iCardCount,WORD wUpCard[]);

	///�õ����ͣ�����������
	int GetShape(BYTE wCardList[],int iCardCount);

	///�������е���
	int CountPoint(BYTE  wCardList[],int iCardCount);

	///ͳ��ѡ��ָ���������Ƿ�������20,10,0�������Ϊ��0ֵ,��ʾ���µ�����,����0��ʾ������

	int CanSumIn(BYTE wCardList[],int iCardCount,int iSelectNum);

	///��ţţ����
	bool IsBullBull(BYTE  wCardList[],int iCardCount);

	///�Ƿ�Ϊ�е�
	int IsHaveNote(BYTE  wCardList[],int iCardCount);
	///�õ�������������(����ɫ)
	BYTE GetMaxCard(BYTE  wCardList[],int iCardCount);
    ///��ţ
	bool IsBull(BYTE  wCardList[],int iCardCount);
	///�Զ���ţ
	bool GetBull(BYTE wCardList[],int iCardCount,BYTE wBullCard[]);
	///�Ƚ��������ƵĴ�С
	 int CompareCard(BYTE wFirstCard[], int iFirstCount, BYTE wSecondCard[], int iSecondCount);

	 ///�õ��Ƽ��з�ʮ�����㣬�Ա��������ţ
	int GetMaxPoint(BYTE  wCardList[],int iCardCount,BYTE wBullCard[]);

	///����������
	int  CountKing(BYTE  wCardList[],int iCardCount);

	///��ȡ�Ƿ�Ƚϻ�ɫֵ
	inline void  GetCompareValue(bool  bRet) 
	{
		m_bCompareKind = bRet ; 
	}

	//��ȡ�Ƶĵ���
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	int GetCardBulk(BYTE iCard, BOOL bExtVol = false);
};