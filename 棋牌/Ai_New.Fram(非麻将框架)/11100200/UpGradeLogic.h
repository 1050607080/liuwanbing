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


// �Ƶĸ���
const BYTE  POKER_SET_NUM  = 8;         // �˿��Ƶĸ���
const BYTE  POKER_CARD_NUM = 52;        // �˿��Ƶ�����
const BYTE  POKER_COUNT_SHAPES = 5;     // 0(��С��) 1(��) 2(��) 3(÷) 4(��) ���ֻ�ɫ
const BYTE  POKER_COUNT_CARDNUM=16;     // 0 A 2 3 ... J Q K С�� ����������16����(0��ʾ�Ʊ�)
const WORD  POKER_CARD_NONE = 0x00;     // ����
const int   SINGLE_CARD_COUNT = 1;      // ����

const HRESULT RET_SUCCESS = 0;          // �ɹ�
const HRESULT RET_FAILED = -1;          // ʧ��
#define SUCCESSED(hRet) (RET_SUCCESS == (HRESULT)hRet)

// ����һ����
#define MAKE_A_CARD(byPkSet,byCardShape,byCardNumber)  (((byPkSet<<8)&0xFF00)|((byCardShape<<4)&0x00F0)|(byCardNumber&0x000F))

// �Ƶĸ���
typedef enum ePokerSetNumber
{
	EPSN_SET_NUM_NONE = 0,		// ���� 
	EPSN_SET_NUM_1,			    // ��һ��
	EPSN_SET_NUM_2              // �ڶ���
}E_POKER_SET_NUMBER;

// �ƵĻ�ɫ
typedef enum eCardShape
{
	ECS_SHAPE_NONE = 0,      // ��С�����޻�ɫ
	ECS_SHAPE_DIAMONDS=1,    // ����
	ECS_SHAPE_CLUBS=2,       // ÷��
	ECS_SHAPE_HEARTS=3,      // ����
	ECS_SHAPE_SPADE=4        // ����
}E_CARD_SHAPE;

// �Ƶĵ���
typedef enum eCardNumber
{
	ECN_NUM_NONE = 0,		// �Ʊ� 
	ECN_NUM_A,			    // A	
	ECN_NUM_2,
	ECN_NUM_3,
	ECN_NUM_4,
	ECN_NUM_5,
	ECN_NUM_6,
	ECN_NUM_7,
	ECN_NUM_8,
	ECN_NUM_9,
	ECN_NUM_10,
	ECN_NUM_J,
	ECN_NUM_Q,
	ECN_NUM_K,
	ECN_NUM_Joker,	       // С��
	ECN_NUM_JOKER	       // ����
}E_CARD_NUMBER;

// ��������
typedef enum eBaseCardsType
{ 
	EBCT_TYPE_NONE = 0,				    // ������
	EBCT_BASETYPE_SINGLE = 1001,		// ���� 
	EBCT_BASETYPE_PAIR,				    // ����:		KK 
	EBCT_BASETYPE_SISTER,				// ˳��:		34567...
	EBCT_BASETYPE_PAIRS,				// ����:		QQKKAA...
	EBCT_BASETYPE_3KIND,				// ����:		KKK
	EBCT_BASETYPE_3KINDS,				// ��˳:		JJJQQQ...	
	EBCT_BASETYPE_3AND1,				// ����һ:	JJJ+K 
	EBCT_BASETYPE_3AND2,				// ������:	JJJ+KK
	EBCT_BASETYPE_3KINDSAND1,			// ��˳��һ:	JJJQQQ...+KA...
	EBCT_BASETYPE_3KINDSAND2,			// ��˳����:	JJJQQQ...+KKAA...
	EBCT_BASETYPE_BOMB					// ը��:		KKKK...   Ĭ��ը�����������ʹ� 
	// ����������չ��������
}E_BASE_CARDS_TYPE;

// �ȽϽ��
typedef enum eCompareResult
{
	ECR_OTHER = -2,         // �޷��Ƚ�
	ECR_LESS  = -1,         // С��
	ECR_EQUAL = 0,          // ����
	ECR_MORE = 1            // ����
}E_COMPARE_RESULT;

// �ȽϽ��
typedef enum eCardSortType
{
	ECST_LEVEL   = 0,       // Ȩֵ����
	ECST_NUMBER  = 1,       // �Ƶ�����
	ECST_CARDCOUN= 2,       // ��ͬ�Ƶ����������
	ECST_SHAPE   = 3        // ��ɫ����
}E_CARD_SORT_TYPE;

// ������Ϸ�߼�
class CPokerGameLogic
{
public:
	CPokerGameLogic();
	virtual ~CPokerGameLogic();

public:

	/********************************************************************************
	* ������: static char Card2Char(char shape,char number)
	* ����;   �������ڼ�������ɫ���Ƶ����ת��Ϊ�ַ� ��ʽ��
	*         (((byPkSet<<8)&0xFF00)|((byCardShape<<4)&0x00F0)|(byCardNumber&0x000F))
	* ����: BYTE byPkSet [in] Ϊ�ڼ�����
	*       BYTE byShape [in] �����Ļ�ɫ 
	*	     BYTE byNumber[in] �������Ƶ�
	* ����: ת����Ľ�� 
	********************************************************************************/
	static WORD CardToWord(BYTE byPkSet, BYTE byShape, BYTE byNumber);

	/********************************************************************************
	* ������: static bool WordToCard(WORD wInCard, BYTE &byPkSet, BYTE &byShape, BYTE &byNumber)
	* ����;   ����������ת��Ϊ�Ƶĵڼ�������ɫ���Ƶ�
	* ����: WORD wInCard  [in]  ������Ҫת������
	*       BYTE &byPkSet [out] Ϊ�ڼ�����
	*       BYTE &byShape [out] �����Ļ�ɫ 
	*	     BYTE &byNumber[out] �������Ƶ� 
	* ����: bool ת���ɹ�true��ʧ��false 
	********************************************************************************/
	static bool WordToCard(WORD wInCard, BYTE &byPkSet, BYTE &byShape, BYTE &byNumber);

	/********************************************************************************
	* ������: static BYTE GetCardSet(WORD wCard)
	* ����;   ��ȡ��Ϊ�ڼ������е���
	* ����: WORD wCard  [in]  ������
	* ����: BYTE �ڼ�����
	********************************************************************************/
	static BYTE GetCardSet(WORD wCard);

	/********************************************************************************
	* ������: static BYTE GetCardShape(WORD wCard)
	* ����;   ��ȡ�ƵĻ�ɫ
	* ����: WORD wCard  [in]  ������
	* ����: BYTE �ƵĻ�ɫ
	********************************************************************************/
	static BYTE GetCardShape(WORD wCard);

	/********************************************************************************
	* ������: static BYTE GetCardNumber(WORD wCard)
	* ����;   �����Ƶ�
	* ����: WORD wCard  [in]  ������
	* ����: BYTE �Ƶ�
	********************************************************************************/
	static BYTE GetCardNumber(WORD wCard);

	/********************************************************************************
	* ������: static BYTE GetCardNumber(WORD wCard)
	* ����;   ������ֵ(��ɫ<<4|�Ƶ�)
	* ����: WORD wCard  [in]  ������
	* ����: BYTE ��ֵ
	********************************************************************************/
	static BYTE GetCardValue(WORD wCard);

	/********************************************************************************
	* ������:static bool IsAvailableCard(WORD wCard)
	* ����;   �������Ƿ�Ϊ��Ч
	* ����: WORD wCard [in]  ��(�ڼ�������ɫ���Ƶ����)
	* ����: bool �Ϸ����ƣ�����true�����򷵻�false
	********************************************************************************/
	static bool IsAvailableCard(WORD wCard);

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
	* ������: static int RemoveCard(WORD wRemoveCard[], int iRemoveCount, WORD wCardList[], int iCardCount)
	* ����;   ɾ���˿�
	* ����: WORD wRemoveCard[]  [in]    ��Ҫɾ�����˿�
	*       int iRemoveCount  [in]      ɾ���������С
	*       WORD wCardList[]  [in/out]  �Ӹ���ֵ��ɾ���˿�
	*       int iCardCount    [in]      ɾ����ԭ����Ĵ�С  
	* ����: int ɾ���Ƶ�����
	********************************************************************************/
	static int RemoveCard(WORD wRemoveCard[], int iRemoveCount, WORD wCardList[], int iCardCount);

	/********************************************************************************
	* ������: static int CardCompare(WORD wCardFirst, WORD wCardSecond)
	* ����;   �����ƽ��бȽ�
	* ����: WORD wCardFirst  [in]   ��һ����
	*       WORD wCardSecond [in]   �ڶ����� 
	* ����: int ����E_COMPARE_RESULT ��һ����<�ڶ��� ECR_LESS
	********************************************************************************/
	static int CardCompare(WORD wCardFirst, WORD wCardSecond);

	/********************************************************************************
	* ������: static HRESULT RandCard(WORD wCardList[], int iCardCount)
	* ����;   ϴ��
	* ����: WORD wCardList[]  [out]     ϴ�õ���
	*       int iCardCount    [in]      �ܹ�����
	* ����: HRESULT 0 �ɹ�������ʧ��
	********************************************************************************/
	static HRESULT RandCard(WORD wCardList[], int iCardCount);

	/********************************************************************************
	* ������: static HRESULT FindCardIndex(int &iIndex, WORD wFindCard, WORD wCardList[], int iCardCount);
	* ����;   ����ĳ�����ڶ����е�����ֵ
	* ����: int  &iIndex      [out]  ���ҵ�������ֵ (δ���ҵ�Ϊ-1)
	*       WORD wFindCard    [in]   ��Ҫ���ҵ���
	*       WORD wCardList[]  [in]   �ƶ���
	*       int iCardCount    [in]   �ܹ�����
	* ����: HRESULT 0 �ɹ�������ʧ��
	********************************************************************************/
	static HRESULT FindCardIndex(int &iIndex, WORD wFindCard, WORD wCardList[], int iCardCount);


public:
	/********************************************************************************
	* ������: BYTE GetCardLevel(WORD wCard)
	* ����;   �����Ƶ�Ȩֵ
	* ����: WORD wCard  [in]  ������
	* ����: BYTE �Ƶ�Ȩֵ
	********************************************************************************/
	 BYTE GetCardLevel(WORD wCard);

	/********************************************************************************
	* ������: bool SortByLevel(WORD wCardList[], int iCardCount,bool bAscending=false)
	* ����;   Ȩֵ����
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortByLevel(WORD wCardList[], int iCardCount,bool bAscending=false);

	/********************************************************************************
	* ������: bool SortByNumber(WORD wCardList[], int iCardCount,bool bAscending=false)
	* ����;   �Ƶ�����
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortByNumber(WORD wCardList[], int iCardCount,bool bAscending=false);

	/********************************************************************************
	* ������: bool SortByShape(WORD wCardList[], int iCardCount,bool bAscending=false)
	* ����;   �ƵĻ�ɫ����
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortByShape(WORD wCardList[], int iCardCount,bool bAscending=false);

	/********************************************************************************
	* ������: bool SortByCardCount(WORD wCardList[], int iCardCount,bool bAscending=false)
	* ����;   �Ƶ����������
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortByCardCount(WORD wCardList[], int iCardCount,bool bAscending=false);

	/********************************************************************************
	* ������: bool SortCards(WORD wCardList[], int iCardCount, int iMod, bool bAscending)
	* ����;   �����˿�
	* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
	*       int iCardCount    [in]      �������С
	*       int iMod          [in]      ����ģʽ 0��ֵ��С��1��������2�ƵĻ�ɫ
	*       bool bAscending   [in]      true����false����
	* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
	********************************************************************************/
	bool SortCards(WORD wCardList[], int iCardCount, int iMod, bool bAscending=false);

};