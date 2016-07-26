#include"stdafx.h"
#include "UpGradeLogic.h"

// ���졢����
CPokerGameLogic::CPokerGameLogic()
{

}

CPokerGameLogic::~CPokerGameLogic()
{

}

/********************************************************************************
* ������: static char Card2Char(char shape,char number)
* ����;   �������ڼ�������ɫ���Ƶ����ת��Ϊ�ַ� ��ʽ��
*         (((byPkSet<<8)&0xFF00)|((byCardShape<<4)&0x00F0)|(byCardNumber&0x000F))
* ����: BYTE byPkSet [in] Ϊ�ڼ�����
*       BYTE byShape [in] �����Ļ�ɫ 
*	     BYTE byNumber[in] �������Ƶ�
* ����: ת����Ľ�� 
********************************************************************************/
WORD CPokerGameLogic::CardToWord(BYTE byPkSet, BYTE byShape, BYTE byNumber)
{
	// �ڼ�����
	if (!IsAvailableCardSet(byPkSet))
	{
		return POKER_CARD_NONE;
	}

	// ��黨ɫ
	if (!IsAvailableCardShape(byShape))
	{
		return POKER_CARD_NONE;
	}

	// ����Ƶ�
	if (!IsAvailableCardNumber(byNumber))
	{
		return POKER_CARD_NONE;
	}

	// ��ϳ���
	WORD wCard = MAKE_A_CARD(byPkSet, byShape, byNumber);
	return wCard;
}

/********************************************************************************
* ������: static bool WordToCard(WORD wInCard, BYTE &byPkSet, BYTE &byShape, BYTE &byNumber)
* ����;   ����������ת��Ϊ�Ƶĵڼ�������ɫ���Ƶ�
* ����: WORD wInCard  [in]  ������Ҫת������
*       BYTE &byPkSet [out] Ϊ�ڼ�����
*       BYTE &byShape [out] �����Ļ�ɫ 
*	     BYTE &byNumber[out] �������Ƶ� 
* ����: bool ת���ɹ�true��ʧ��false 
********************************************************************************/
bool CPokerGameLogic::WordToCard(WORD wInCard, BYTE &byPkSet, BYTE &byShape, BYTE &byNumber)
{
	// ��ȡ�ڼ�����
	byPkSet = GetCardSet(wInCard);

	// ��ȡ��ɫ
	byShape = GetCardShape(wInCard);

	// ��ȡ�Ƶ�
	byNumber = GetCardNumber(wInCard);

	// �Ƹ������Ƶ���
	if ((EPSN_SET_NUM_NONE == byPkSet) || (ECN_NUM_NONE == byNumber))
	{
		return false;
	}
	return true;
}

/********************************************************************************
* ������: static BYTE GetCardSet(WORD wCard)
* ����;   ��ȡ��Ϊ�ڼ������е���
* ����: WORD wCard  [in]  ������
* ����: BYTE �ڼ�����
********************************************************************************/
BYTE CPokerGameLogic::GetCardSet(WORD wCard)
{
	BYTE bySet = (BYTE)(wCard>>8&0x00FF);     // �Ƹ�
	if (IsAvailableCardSet(bySet))
	{
		return bySet;
	}
	return EPSN_SET_NUM_NONE;
}

/********************************************************************************
* ������: static BYTE GetCardShape(WORD wCard)
* ����;   ��ȡ�ƵĻ�ɫ
* ����: WORD wCard  [in]  ������
* ����: BYTE �ƵĻ�ɫ
********************************************************************************/
BYTE CPokerGameLogic::GetCardShape(WORD wCard)
{
	BYTE byCardShape = (BYTE)(wCard>>4&0x000F);  // �ƻ�ɫ
	if (IsAvailableCardShape(byCardShape))
	{
		return byCardShape;
	}
	return ECS_SHAPE_NONE;
}

/********************************************************************************
* ������: static BYTE GetCardNumber(WORD wCard)
* ����;   �����Ƶ�
* ����: WORD wCard  [in]  ������
* ����: BYTE �Ƶ�
********************************************************************************/
BYTE CPokerGameLogic::GetCardNumber(WORD wCard)
{
	BYTE byCardNumber = (BYTE)(wCard&0x000F);  // �Ƶ�
	if (IsAvailableCardNumber(byCardNumber))
	{
		return byCardNumber;
	}
	return ECN_NUM_NONE;
}

/********************************************************************************
* ������: static BYTE GetCardNumber(WORD wCard)
* ����;   ������ֵ(��ɫ<<4|�Ƶ�)
* ����: WORD wCard  [in]  ������
* ����: BYTE ��ֵ
********************************************************************************/
BYTE CPokerGameLogic::GetCardValue(WORD wCard)
{
	BYTE byCardValue = (BYTE)(wCard&0x00FF);
	// �����ֵ
	if (IsAvailableCardValue(byCardValue))
	{
		return byCardValue;
	}
	return 0;
}

/********************************************************************************
* ������:static bool IsAvailableCard(WORD wCard)
* ����;   �������Ƿ�Ϊ��Ч
* ����: WORD wCard [in]  ��(�ڼ�������ɫ���Ƶ����)
* ����: bool �Ϸ����ƣ�����true�����򷵻�false
********************************************************************************/
bool CPokerGameLogic::IsAvailableCard(WORD wCard)
{
	// �����Ϊ�ڼ�����
	BYTE bySet = (BYTE)(wCard>>8&0xFF);
	if (!IsAvailableCardSet(bySet))
	{
		return false;
	}

	// �����ֵ
	BYTE byCardValue = (BYTE)(wCard&0x00FF);
	if (IsAvailableCardValue(byCardValue))
	{
		return true;
	}
	return false;
}

/********************************************************************************
* ������:static bool IsAvailableCardValue(BYTE byCardValue)
* ����;   ������ֵ�Ƿ�Ϊ��Ч
* ����: BYTE byCardValue [in]  ��ֵ(��ɫ���Ƶ����)
* ����: bool �Ϸ�����ֵ������true�����򷵻�false
********************************************************************************/
bool CPokerGameLogic::IsAvailableCardValue(BYTE byCardValue)
{
	// ����ƻ�ɫ
	BYTE byCardShape = (byCardValue>>4)&0x0F;
	if (!IsAvailableCardShape(byCardShape))
	{
		return false;
	}

	// �����ֵ
	if (!IsAvailableCardNumber(byCardValue&0x0F))
	{
		return false;
	}
	return true;
}

/********************************************************************************
* ������:static bool IsAvailableCardSet(BYTE byCardSet)
* ����;   �����Ƹ��Ƿ�Ϊ��Ч
* ����: BYTE byCardSet [in]  �ڼ���
* ����: bool �Ϸ����ƣ�����true�����򷵻�false
********************************************************************************/
bool CPokerGameLogic::IsAvailableCardSet(BYTE byCardSet)
{
	// ����Ƹ��Ƿ�Ϸ�
	if ((EPSN_SET_NUM_NONE == byCardSet) || (byCardSet>POKER_SET_NUM))
	{
		return false;
	}
	return true;
}

/********************************************************************************
* ������: static bool IsAvailableCardShape(BYTE byCardShape)
* ����;   �����ƻ�ɫ�Ƿ�Ϊ��Ч
* ����: BYTE byCardShape [in]  �ƻ�ɫ
* ����: bool �Ϸ����ƻ�ɫ������true�����򷵻�false
********************************************************************************/
bool CPokerGameLogic::IsAvailableCardShape(BYTE byCardShape)
{
	// ����ƵĻ�ɫ
	if (byCardShape < POKER_COUNT_SHAPES)
	{
		return true;
	}
	return false;
}

/********************************************************************************
* ������:static bool IsAvailableCardNumber(BYTE byCardNumber)
* ����;   �����Ƶ��Ƿ�Ϊ��Ч
* ����: BYTE byCardNumber [in]  �Ƶ�
* ����: bool �Ϸ����Ƶ㣬����true�����򷵻�false
********************************************************************************/
bool CPokerGameLogic::IsAvailableCardNumber(BYTE byCardNumber)
{
	if ((byCardNumber < POKER_COUNT_CARDNUM) && (0x00 != byCardNumber))
	{
		return true;
	}
	return false;
}


/********************************************************************************
* ������: static int RemoveCard(WORD wRemoveCard[], int iRemoveCount, WORD wCardList[], int iCardCount)
* ����;   ɾ���˿�
* ����: WORD wRemoveCard[]  [in]    ��Ҫɾ�����˿�
*       int iRemoveCount  [in]      ɾ���������С
*       WORD wCardList[]  [in/out]  �Ӹ���ֵ��ɾ���˿�
*       int iCardCount    [in]      ɾ����ԭ����Ĵ�С  
* ����: int ɾ���Ƶ�����
********************************************************************************/
int CPokerGameLogic::RemoveCard(WORD wRemoveCard[], int iRemoveCount, WORD wCardList[], int iCardCount)
{
	return 0;
}

/********************************************************************************
* ������: static int CardCompare(WORD wCardFirst, WORD wCardSecond)
* ����;   �����ƽ��бȽ�
* ����: WORD wCardFirst  [in]   ��һ����
*       WORD wCardSecond [in]   �ڶ����� 
* ����: int ����E_COMPARE_RESULT ��һ����<�ڶ��� ECR_LESS
********************************************************************************/
int CPokerGameLogic::CardCompare(WORD wCardFirst, WORD wCardSecond)
{
	// ������Ƿ�Ϸ�
	if (!IsAvailableCard(wCardFirst) || !IsAvailableCard(wCardSecond))
	{
		return ECR_OTHER;   // �޷��Ƚ�
	}

	// ȥ���Ƹ������бȽ�
	BYTE byValueFirst = GetCardValue(wCardFirst);
	BYTE byValueSecond = GetCardValue(wCardSecond);
	if (byValueFirst == byValueSecond)
	{
		return ECR_EQUAL;
	}
	else if (byValueFirst < byValueSecond)
	{
		return ECR_LESS;
	}
	else
	{
		return ECR_MORE;
	}
}

/********************************************************************************
* ������: static HRESULT RandCard(WORD wCardList[], int iCardCount)
* ����;   ϴ��
* ����: WORD wCardList[]  [out]     ϴ�õ���
*       int iCardCount    [in]      �ܹ�����
* ����: HRESULT 0 �ɹ�������ʧ��
********************************************************************************/
HRESULT CPokerGameLogic::RandCard(WORD wCardList[], int iCardCount)
{
	// �������
	if ((iCardCount<SINGLE_CARD_COUNT) || (iCardCount > (POKER_CARD_NUM*POKER_SET_NUM)))
	{
		return RET_FAILED;
	}

	// ���
	memset(wCardList, 0,iCardCount*sizeof(WORD));
	BYTE i = 0;
	BYTE j = 0;
	BYTE h = 0;
	int iUseCount = 0;

	//CINIFile  f("E:/T.ini");
	//BYTE byShape = (BYTE)f.GetKeyVal("game", "shape", 0);
	//BYTE byNumber = (BYTE)f.GetKeyVal("game", "number", 0);

	// ��װ������
	for (i=0; i<POKER_SET_NUM; ++i)
	{
		BYTE byCardSet = i+1;
		for (j=ECS_SHAPE_SPADE; j>ECS_SHAPE_NONE; --j)
		{
			for (h=ECN_NUM_K; h>ECN_NUM_NONE; --h)
			{
				//wCardList[iUseCount] = CardToWord(byCardSet, byShape, byNumber); 
				wCardList[iUseCount] = CardToWord(byCardSet, j, h);
				iUseCount ++;
				if (iUseCount == iCardCount) // ����װ���
				{
					break;
				}
			}

			if (iUseCount == iCardCount) // ����װ���
			{
				break;
			}
		}

		if (iUseCount == iCardCount) // ����װ���
		{
			break;
		}

		// ���Ӵ�С��
		//wCardList[iUseCount] = CardToWord(byCardSet, byShape, byNumber); 
		//wCardList[iUseCount] = CardToWord(byCardSet, ECS_SHAPE_NONE, ECN_NUM_Joker);   // С��
		//iUseCount ++;
		if (iUseCount == iCardCount) // ����װ���
		{
			break;
		}

		//wCardList[iUseCount] = CardToWord(byCardSet, byShape, byNumber); 
		//wCardList[iUseCount] = CardToWord(byCardSet, ECS_SHAPE_NONE, ECN_NUM_JOKER);   // ����
		//iUseCount ++;
		if (iUseCount == iCardCount) // ����װ���
		{
			break;
		}
	}

	// ���ϴ��
	int iRandStart = 0;
	srand((unsigned)GetTickCount());
	do
	{
		int iPos = rand()%(iUseCount-iRandStart) + iRandStart;
		if (iPos != iRandStart)
		{
			WORD wCardTemp = wCardList[iRandStart];
			wCardList[iRandStart]= wCardList[iPos];
			wCardList[iPos] = wCardTemp;
		}		
		iRandStart++;
	}while (iRandStart<iUseCount);

	return RET_SUCCESS;	
}

/********************************************************************************
* ������: static HRESULT FindCardIndex(int &iIndex, WORD wFindCard, WORD wCardList[], int iCardCount);
* ����;   ����ĳ�����ڶ����е�����ֵ
* ����: int  &iIndex      [out]  ���ҵ�������ֵ (δ���ҵ�Ϊ-1)
*       WORD wFindCard    [in]   ��Ҫ���ҵ���
*       WORD wCardList[]  [in]   �ƶ���
*       int iCardCount    [in]   �ܹ�����
* ����: HRESULT 0 �ɹ�������ʧ��
********************************************************************************/
HRESULT CPokerGameLogic::FindCardIndex(int &iIndex, WORD wFindCard, WORD wCardList[], int iCardCount)
{
	iIndex = -1;
	if (!IsAvailableCard(wFindCard))
	{
		return RET_SUCCESS;	
	}

	int i=0;
	for (i=0; i<iCardCount; ++i)
	{
		if (wFindCard == wCardList[i])
		{
			iIndex = i;
			break;  // �ҵ����˳���
		}
	}
	return RET_SUCCESS;	
}

/********************************************************************************
* ������: BYTE GetCardLevel(WORD wCard)
* ����;   �����Ƶ�Ȩֵ
* ����: WORD wCard  [in]  ������
* ����: BYTE �Ƶ�Ȩֵ
********************************************************************************/
BYTE CPokerGameLogic::GetCardLevel(WORD wCard)
{
	// ÿ��Ϸ���Ƶ�Ȩֵ�ܲ�һ�������������޸Ĵ˺���
	if (!IsAvailableCard(wCard))
	{
		return POKER_CARD_NONE;
	}

	// K(13) A(14) 2(16) С����18�� ����(20)
	BYTE byCardNumber = GetCardNumber(wCard);
	if (ECN_NUM_A == byCardNumber)
	{
		return 0x0E;
	}
	else if (ECN_NUM_2 == byCardNumber)
	{
		return 0x10;
	}
	else if (ECN_NUM_Joker == byCardNumber)
	{
		return 0x12;
	}
	else if (ECN_NUM_JOKER == byCardNumber)
	{
		return 0x14;
	}
	else
	{
		return byCardNumber;
	}
}

/********************************************************************************
* ������: bool SortByLevel(WORD wCardList[], int iCardCount,bool bAscending=false)
* ����;   Ȩֵ����
* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
*       int iCardCount    [in]      �������С
*       bool bAscending   [in]      true����false����
* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
********************************************************************************/
bool CPokerGameLogic::SortByLevel(WORD wCardList[], int iCardCount,bool bAscending)
{
	// һ���Ʋ�������
	if (iCardCount <= SINGLE_CARD_COUNT)
	{
		return true;
	}

	// ���Ƿ�Ϸ�
	int i=0;
	for (i=0; i<iCardCount; ++i)
	{
		if (!IsAvailableCard(wCardList[i]))
		{
			return false;
		}
	}

	// ʹ��ð������
	int j=0;
	for (i=0; i<iCardCount; ++i)
	{
		for (j=(iCardCount -1); j>i; --j)
		{
			// �Ƚ�Ȩֵ
			BYTE byFirst = GetCardLevel(wCardList[j-1]);
			BYTE bySecond = GetCardLevel(wCardList[j]);
			if (byFirst == bySecond)
			{
				// �Ƚ��Ƶ�
				byFirst = GetCardNumber(wCardList[j-1]);
				bySecond = GetCardNumber(wCardList[j]);
				if (byFirst == bySecond)
				{
					// �Ƚϻ�ɫ
					byFirst = GetCardShape(wCardList[j-1]);
					if (ECS_SHAPE_NONE == byFirst)  // ���Ļ�ɫ��һ��
					{
						byFirst = POKER_COUNT_SHAPES;
					}
					bySecond = GetCardShape(wCardList[j]);
					if (ECS_SHAPE_NONE == bySecond)  // ���Ļ�ɫ��һ��
					{
						bySecond = POKER_COUNT_SHAPES;
					}
					if (byFirst == bySecond)
					{
						continue;
					}
				}
			}

			// ����λ��
			if (((byFirst > bySecond) && (bAscending))    // ��һ�Ŵ��ڵڶ��� Ҫ������
				||((byFirst < bySecond) && (!bAscending)))// ��һ��С�ڵڶ���  Ҫ����
			{
				WORD wTemp = wCardList[j-1];
				wCardList[j-1] = wCardList[j];
				wCardList[j] = wTemp;
			}
		}
	}
	return true;
}
/********************************************************************************
* ������: bool SortByNumber(WORD wCardList[], int iCardCount,bool bAscending=false)
* ����;   �Ƶ�����
* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
*       int iCardCount    [in]      �������С
*       bool bAscending   [in]      true����false����
* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
********************************************************************************/
bool CPokerGameLogic::SortByNumber(WORD wCardList[], int iCardCount,bool bAscending)
{
	// һ���Ʋ�������
	if (iCardCount <= SINGLE_CARD_COUNT)
	{
		return true;
	}

	// ���Ƿ�Ϸ�
	int i=0;
	for (i=0; i<iCardCount; ++i)
	{
		if (!IsAvailableCard(wCardList[i]))
		{
			return false;
		}
	}

	// ʹ��ð������
	int j=0;
	for (i=0; i<iCardCount; ++i)
	{
		for (j=(iCardCount -1); j>i; --j)
		{
			// �Ƚ��Ƶ�
			BYTE byFirst = GetCardNumber(wCardList[j-1]);
			BYTE bySecond = GetCardNumber(wCardList[j]);
			if (byFirst == bySecond)
			{
				// �Ƚ�Ȩֵ
				byFirst = GetCardLevel(wCardList[j-1]);
				bySecond = GetCardLevel(wCardList[j]);
				if (byFirst == bySecond)
				{
					// �Ƚϻ�ɫ
					byFirst = GetCardShape(wCardList[j-1]);
					if (ECS_SHAPE_NONE == byFirst)  // ���Ļ�ɫ��һ��
					{
						byFirst = POKER_COUNT_SHAPES;
					}
					bySecond = GetCardShape(wCardList[j]);
					if (ECS_SHAPE_NONE == bySecond)  // ���Ļ�ɫ��һ��
					{
						bySecond = POKER_COUNT_SHAPES;
					}
					if (byFirst == bySecond)
					{
						continue;
					}
				}
			}

			// ����λ��
			if (((byFirst > bySecond) && (bAscending))    // ��һ�Ŵ��ڵڶ��� Ҫ������
				||((byFirst < bySecond) && (!bAscending)))// ��һ��С�ڵڶ���  Ҫ����
			{
				WORD wTemp = wCardList[j-1];
				wCardList[j-1] = wCardList[j];
				wCardList[j] = wTemp;
			}
		}
	}
	return true;
}
/********************************************************************************
* ������: bool SortByShape(WORD wCardList[], int iCardCount,bool bAscending=false)
* ����;   �ƵĻ�ɫ����
* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
*       int iCardCount    [in]      �������С
*       bool bAscending   [in]      true����false����
* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
********************************************************************************/
bool CPokerGameLogic::SortByShape(WORD wCardList[], int iCardCount,bool bAscending)
{
	// һ���Ʋ�������
	if (iCardCount <= SINGLE_CARD_COUNT)
	{
		return true;
	}

	// ���Ƿ�Ϸ�
	int i=0;
	for (i=0; i<iCardCount; ++i)
	{
		if (!IsAvailableCard(wCardList[i]))
		{
			return false;
		}
	}

	// ʹ��ð������
	int j=0;
	for (i=0; i<iCardCount; ++i)
	{
		for (j=(iCardCount -1); j>i; --j)
		{
			// �Ƚϻ�ɫ
			BYTE byFirst = GetCardShape(wCardList[j-1]);
			if (ECS_SHAPE_NONE == byFirst)  // ���Ļ�ɫ��һ��
			{
				byFirst = POKER_COUNT_SHAPES;
			}
			BYTE bySecond = GetCardShape(wCardList[j]);
			if (ECS_SHAPE_NONE == bySecond)  // ���Ļ�ɫ��һ��
			{
				bySecond = POKER_COUNT_SHAPES;
			}
			if (byFirst == bySecond)
			{
				// �Ƚ�Ȩֵ
				byFirst = GetCardLevel(wCardList[j-1]);
				bySecond = GetCardLevel(wCardList[j]);
				if (byFirst == bySecond)
				{
					// �Ƚ��Ƶ�
					byFirst = GetCardNumber(wCardList[j-1]);
					bySecond = GetCardNumber(wCardList[j]);
					if (byFirst == bySecond)
					{
						continue;
					}
				}
			}

			// ����λ��
			if (((byFirst > bySecond) && (bAscending))    // ��һ�Ŵ��ڵڶ��� Ҫ������
				||((byFirst < bySecond) && (!bAscending)))// ��һ��С�ڵڶ���  Ҫ����
			{
				WORD wTemp = wCardList[j-1];
				wCardList[j-1] = wCardList[j];
				wCardList[j] = wTemp;
			}
		}
	}

	return true;
}
/********************************************************************************
* ������: bool SortByCardCount(WORD wCardList[], int iCardCount,bool bAscending=false)
* ����;   �Ƶ����������
* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
*       int iCardCount    [in]      �������С
*       bool bAscending   [in]      true����false����
* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
********************************************************************************/
bool CPokerGameLogic::SortByCardCount(WORD wCardList[], int iCardCount,bool bAscending)
{
	// һ���Ʋ�������
	if (iCardCount <= SINGLE_CARD_COUNT)
	{
		return true;
	}

	// �����Ƶ����Ƶ�����
	BYTE abyCount[POKER_COUNT_CARDNUM] = {0};
	int i=0;
	for (i=0; i<iCardCount; ++i)
	{
		if (!IsAvailableCard(wCardList[i]))
		{
			return false;
		}
		abyCount[GetCardNumber(wCardList[i])] ++;
	}

	// ʹ��ð������
	int j=0;
	for (i=0; i<iCardCount; ++i)
	{
		for (j=(iCardCount -1); j>i; --j)
		{
			BYTE byFirst = abyCount[GetCardNumber(wCardList[j-1])];
			BYTE bySecond = abyCount[GetCardNumber(wCardList[j])];
			if (byFirst == bySecond)
			{
				// �Ƚ�Ȩֵ
				byFirst = GetCardLevel(wCardList[j-1]);
				bySecond = GetCardLevel(wCardList[j]);
				if (byFirst == bySecond)
				{
					// �Ƚ��Ƶ�
					byFirst = GetCardNumber(wCardList[j-1]);
					bySecond = GetCardNumber(wCardList[j]);
					if (byFirst == bySecond)
					{
						// �Ƚϻ�ɫ
						byFirst = GetCardShape(wCardList[j-1]);
						if (ECS_SHAPE_NONE == byFirst)  // ���Ļ�ɫ��һ��
						{
							byFirst = POKER_COUNT_SHAPES;
						}
						bySecond = GetCardShape(wCardList[j]);
						if (ECS_SHAPE_NONE == bySecond)  // ���Ļ�ɫ��һ��
						{
							bySecond = POKER_COUNT_SHAPES;
						}
						if (byFirst == bySecond)
						{
							continue;
						}
					}
				}
			}

			// ����λ��
			if (((byFirst > bySecond) && (bAscending))    // ��һ�Ŵ��ڵڶ��� Ҫ������
				||((byFirst < bySecond) && (!bAscending)))// ��һ��С�ڵڶ���  Ҫ����
			{
				WORD wTemp = wCardList[j-1];
				wCardList[j-1] = wCardList[j];
				wCardList[j] = wTemp;
			}
		}
	}

	return true;
}
/********************************************************************************
* ������: bool SortCards(WORD wCardList[], int iCardCount, int iMod, bool bAscending)
* ����;   �����˿�
* ����: WORD wCardList[]  [in/out]  ���������ơ��ź��������
*       int iCardCount    [in]      �������С
*       int iMod          [in]      ����ģʽ   0����Ȩֵ 1�Ƶ��С��2��������3�ƵĻ�ɫ
*       bool bAscending   [in]      true����false����
* ����: bool �����Ƿ�ɹ� true �ɹ�  false ʧ��
********************************************************************************/
bool CPokerGameLogic::SortCards(WORD wCardList[], int iCardCount, int iMod, bool bAscending)
{
	if ((iMod < ECST_LEVEL) || (iMod > ECST_SHAPE))
	{
		return false;
	}
	// һ���Ʋ�������
	if (iCardCount <= SINGLE_CARD_COUNT)
	{
		return true;
	}

	switch (iMod)
	{
	case ECST_LEVEL:            // Ȩֵ����
		{
			return SortByLevel(wCardList, iCardCount, bAscending);
		}
	case ECST_NUMBER:          // �Ƶ�����
		{
			return SortByNumber(wCardList, iCardCount, bAscending);
		}
	case ECST_CARDCOUN:        // �Ƶ���������
		{
			return SortByCardCount(wCardList, iCardCount, bAscending);
		}
	case ECST_SHAPE:           // ��ɫ����
		{
			return SortByShape(wCardList, iCardCount, bAscending);
		}
	default:
		break;
	}
	return false;
}
