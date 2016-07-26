#include"stdafx.h"
#include "UpGradeLogic.h"
#include "rand/GenRand.h"
// ���졢����
CPokerGameLogic::CPokerGameLogic()
{
	m_bCompareKind = true ; 
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
BYTE CPokerGameLogic::CardToWord(BYTE byPkSet, BYTE byShape, BYTE byNumber)
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
	BYTE wCard = MAKE_A_CARD(byPkSet, byShape, byNumber);
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
bool CPokerGameLogic::WordToCard(BYTE wInCard, BYTE &byPkSet, BYTE &byShape, BYTE &byNumber)
{
	// ��ȡ�ڼ�����
	//byPkSet = GetCardSet(wInCard);

	// ��ȡ��ɫ
	byShape = GetCardShape(wInCard);

	// ��ȡ�Ƶ�
	byNumber = GetCardNumber(wInCard);

	// �Ƹ������Ƶ���
	if ( _ECN_NUM_NONE == byNumber)
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
BYTE CPokerGameLogic::GetCardSet(BYTE wCard)
{
	BYTE bySet = (BYTE)(wCard&0xFF);     // �Ƹ�
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
BYTE CPokerGameLogic::GetCardShape(BYTE wCard)
{
	BYTE byCardShape = 0x00;  // �ƻ�ɫ

	if((wCard>>4)&0x0F == 0)
	{
		byCardShape = _ECS_SHAPE_DIAMONDS;
	}
	else if((wCard>>4)&0x0F == 1)
	{
		byCardShape = _ECS_SHAPE_CLUBS;
	}
	else if((wCard>>4)&0x0F == 2)
	{
		byCardShape = _ECS_SHAPE_HEARTS;
	}
	else if((wCard>>4)&0x0F == 3)
	{
		byCardShape = _ECS_SHAPE_SPADE;
	}
	else
	{
		byCardShape = _ECS_SHAPE_NONE;
	}

	if (IsAvailableCardShape(byCardShape))
	{
		return byCardShape;
	}
	return _ECS_SHAPE_NONE;
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CPokerGameLogic::GetCardBulk(BYTE iCard, BOOL bExtVol)
{
	if ((iCard==0x4E)||(iCard==0x4F))
	{
		return bExtVol?(iCard-14):(iCard-64);	//��С��64+14-62=16			ֻ���ش�Сè��ֵ
	}

	int iCardNum=(iCard&0x0F);
	int iHuaKind=(iCard&0xF0);

	if (iCardNum == 0x0D)	// AΪ1��,ţţ������	
	{
		if(bExtVol)		
			return ((iHuaKind>>4)+(1*4));
		else
			return 1;
	}
	return ((bExtVol)?((iHuaKind>>4)+(iCardNum*4)):(iCardNum));
}

/********************************************************************************
* ������: static BYTE GetCardNumber(WORD wCard)
* ����;   �����Ƶ�
* ����: WORD wCard  [in]  ������
* ����: BYTE �Ƶ�
********************************************************************************/
BYTE CPokerGameLogic::GetCardNumber(BYTE wCard)
{
	BYTE byCardNumber = 0x00;  // �Ƶ�
	if((wCard & 0x0F) == 0x0D)		//A
	{
		byCardNumber = _ECN_NUM_A;
	}
	else if((wCard & 0x0F) == 0x0E)		//С��
	{
		byCardNumber = _ECN_NUM_Joker;
	}
	else if((wCard & 0x0F) == 0x0F)		//����
	{
		byCardNumber = _ECN_NUM_JOKER;
	}
	else if((wCard&0x0F)==0x0A || (wCard&0x0F)==0x0B || (wCard&0x0F)==0x0C)		//����
	{
		byCardNumber = _ECN_NUM_10;
	}
	else
	{
		byCardNumber = (wCard&0x0F) + 1;
	}
	
	if (IsAvailableCardNumber(byCardNumber))
	{
		return byCardNumber;
	}
	return _ECN_NUM_NONE;
}


/********************************************************************************
* ������: static BYTE GetCardValue(WORD wCard)
* ����;   ������ֵ(��ɫ<<4|�Ƶ�)
* ����: WORD wCard  [in]  ������
* ����: BYTE ��ֵ
********************************************************************************/
// BYTE CPokerGameLogic::GetCardValue(BYTE wCard)
// {
// 	// �����ֵ
// 	if (IsAvailableCardValue(GetCardNumber(wCard)))
// 	{
// 		return byCardValue;
// 	}
// 	return 0;
// }

/********************************************************************************
* ������:static bool IsAvailableCard(WORD wCard)
* ����;   �������Ƿ�Ϊ��Ч
* ����: WORD wCard [in]  ��(�ڼ�������ɫ���Ƶ����)
* ����: bool �Ϸ����ƣ�����true�����򷵻�false
********************************************************************************/
bool CPokerGameLogic::IsAvailableCard(BYTE wCard)
{
	if (IsAvailableCardValue(wCard&0xFF))
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
	if (!IsAvailableCardShape((byCardValue>>4)&0x0F))
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
int CPokerGameLogic::RemoveCard(BYTE wRemoveCard[], int iRemoveCount, BYTE wCardList[], int iCardCount)
{
	//��������
	if ((iRemoveCount>iCardCount)) return 0;

	//��Ҫɾ����������
	int iDeleteCount=0;
	for (int i=0; i<iRemoveCount; i++)
	{
		for (int j=0; j<iCardCount; j++)
		{
			if (wRemoveCard[i]==wCardList[j])
			{
				iDeleteCount++;
				wCardList[j] = 0;
				break;
			}
		}
	}
	RemoveNummCard(wCardList,iCardCount);
	if (iDeleteCount!=iRemoveCount) return 0;

	return iDeleteCount;
	//return 0;
}

//��� 0 λ�˿�
int CPokerGameLogic::RemoveNummCard(BYTE wCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0; i<iCardCount; i++)
	{
		if (wCardList[i]!=0) wCardList[i-iRemoveCount]=wCardList[i];
		else iRemoveCount++;
	}
	return iRemoveCount;
}

/********************************************************************************
* ������: static int CardCompare(WORD wCardFirst, WORD wCardSecond)
* ����;   �����ƽ��бȽ�
* ����: WORD wCardFirst  [in]   ��һ����
*       WORD wCardSecond [in]   �ڶ����� 
* ����: int ����E_COMPARE_RESULT ��һ����<�ڶ��� ECR_LESS
********************************************************************************/
int CPokerGameLogic::CardCompare(BYTE wCardFirst, BYTE wCardSecond)
{
	// ������Ƿ�Ϸ�
	if (!IsAvailableCard(wCardFirst) || !IsAvailableCard(wCardSecond))
	{
		return ECR_OTHER;   // �޷��Ƚ�
	}

	// ȥ���Ƹ������бȽ�
	BYTE byValueFirst = GetCardNumber(wCardFirst);
	BYTE byValueSecond = GetCardNumber(wCardSecond);
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
*       bool bHaveKing    [in]      �Ƿ�Ҫ��
* ����: HRESULT 0 �ɹ�������ʧ��
********************************************************************************/
BYTE CPokerGameLogic::RandCard(BYTE iCard[], int iCardCount,bool bhaveKing)
{
	static const BYTE m_CardArray[54]={
		//2     3     4     5     6     7     8     9     10   J      Q    K     A
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
		0x4E, 0x4F};
		//С�����

		BYTE iSend=0,iStation=0,iCardList[162];
		static int temp = 0;
		if (temp > 9999999999)
			temp = 0;
		srand((unsigned)GetCurrentTime()+(temp++));

		if ( bhaveKing)
		{
			for (int i=0;i<iCardCount;i+=54)
				::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));
		}
		else
		{
			for (int i=0;i<iCardCount;i+=52)
				::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));
		}

		do
		{
			iStation=rand()%(iCardCount-iSend);
			iCard[iSend]=iCardList[iStation];
			iSend++;
			iCardList[iStation]=iCardList[iCardCount-iSend];
		} while (iSend<iCardCount);

		return iCardCount;
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
HRESULT CPokerGameLogic::FindCardIndex(int &iIndex, BYTE wFindCard, BYTE wCardList[], int iCardCount)
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
BYTE CPokerGameLogic::GetCardLevel(BYTE wCard)
{
	// ÿ��Ϸ���Ƶ�Ȩֵ�ܲ�һ�������������޸Ĵ˺���
	if (!IsAvailableCard(wCard))
	{
		return POKER_CARD_NONE;
	}

	// K(13) A(14) 2(16) С����18�� ����(20)
	BYTE byCardNumber = GetCardNumber(wCard);

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
bool CPokerGameLogic::SortByLevel(BYTE wCardList[], int iCardCount,bool bAscending)
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
					if (_ECS_SHAPE_NONE == byFirst)  // ���Ļ�ɫ��һ��
					{
						byFirst = POKER_COUNT_SHAPES;
					}
					bySecond = GetCardShape(wCardList[j]);
					if (_ECS_SHAPE_NONE == bySecond)  // ���Ļ�ɫ��һ��
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
				BYTE wTemp = wCardList[j-1];
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
bool CPokerGameLogic::SortByNumber(BYTE wCardList[], int iCardCount,bool bAscending)
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
					if (_ECS_SHAPE_NONE == byFirst)  // ���Ļ�ɫ��һ��
					{
						byFirst = POKER_COUNT_SHAPES;
					}
					bySecond = GetCardShape(wCardList[j]);
					if (_ECS_SHAPE_NONE == bySecond)  // ���Ļ�ɫ��һ��
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
				BYTE wTemp = wCardList[j-1];
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
bool CPokerGameLogic::SortByShape(BYTE wCardList[], int iCardCount,bool bAscending)
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
			if (_ECS_SHAPE_NONE == byFirst)  // ���Ļ�ɫ��һ��
			{
				byFirst = POKER_COUNT_SHAPES;
			}
			BYTE bySecond = GetCardShape(wCardList[j]);
			if (_ECS_SHAPE_NONE == bySecond)  // ���Ļ�ɫ��һ��
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
				BYTE wTemp = wCardList[j-1];
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
bool CPokerGameLogic::SortByCardCount(BYTE wCardList[], int iCardCount,bool bAscending)
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
						if (_ECS_SHAPE_NONE == byFirst)  // ���Ļ�ɫ��һ��
						{
							byFirst = POKER_COUNT_SHAPES;
						}
						bySecond = GetCardShape(wCardList[j]);
						if (_ECS_SHAPE_NONE == bySecond)  // ���Ļ�ɫ��һ��
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
				BYTE wTemp = wCardList[j-1];
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
bool CPokerGameLogic::SortCards(BYTE wCardList[], int iCardCount, int iMod, bool bAscending)
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
int CPokerGameLogic::GetPoint(BYTE wCard)
{
	if (wCard == 0x00)
		return 0;
	switch (GetCardNumber(wCard))
	{
	case _ECN_NUM_10:
		return 10;
	case _ECN_NUM_J:
		return 10;
	case _ECN_NUM_Q:
		return 10;
	case _ECN_NUM_K:
		return 10;
	case _ECN_NUM_A:
		return 1;
	case _ECN_NUM_Joker:
		return 0x0E;
	case _ECN_NUM_JOKER:
		return 0x0F;
	default:
		return GetCardNumber(wCard);
	}
}
// int CPokerGameLogic::GetCardPoint(BYTE wCardList[],int iCardCount)
// {
// 
// 	return 0;
// }

///�õ���ǰ����
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@return: int [out]  ��������
int CPokerGameLogic::GetShape(BYTE wCardList[],int iCardCount)
{
	
// 	if (CanSumIn(wCardList, iCardCount, 3)>0 && GetCardNumber(GetMaxCard(wCardList,iCardCount)) == ECN_NUM_Joker)
// 		return EBCT_TYPE_BULL_joker;
// 	if (CanSumIn(wCardList, iCardCount, 3)>0 && GetCardNumber(GetMaxCard(wCardList,iCardCount)) == ECN_NUM_JOKER)
// 		return EBCT_TYPE_BULL_JOKER;
	if (2 == CountKing(wCardList,iCardCount)) //����������
		return EBCT_TYPE_BULL_JOKER;
	if (1 == CountKing(wCardList,iCardCount))//����ţ������һ������
	{
		BYTE Temp[5] = {0};
		switch(GetMaxPoint(wCardList,iCardCount,Temp))//����һ�����������ҳ����������������
		{
		case 0:
			if (GetCardNumber(GetMaxCard(wCardList,iCardCount)) == _ECN_NUM_Joker)
				return EBCT_TYPE_BULL_joker;
			if (GetCardNumber(GetMaxCard(wCardList,iCardCount)) == _ECN_NUM_JOKER)
				return EBCT_TYPE_BULL_JOKER;
		case 1:
			return EBCT_TYPE_BULL_ONE;
		case 2:
			return EBCT_TYPE_BULL_TWO;
		case 3:
			return EBCT_TYPE_BULL_THREE;
		case 4:
			return EBCT_TYPE_BULL_FOUR;
		case 5:
			return EBCT_TYPE_BULL_FIVE;
		case 6:
			return EBCT_TYPE_BULL_SIX;
		case 7:
			return EBCT_TYPE_BULL_SEVEN;
		case 8:
			return EBCT_TYPE_BULL_EIGHT;
		case 9:
			return EBCT_TYPE_BULL_NINE;
		}
	}
	if(IsBullBull(wCardList,iCardCount)) 
		return EBCT_TYPE_BULL_BULL;
	switch(IsHaveNote(wCardList,iCardCount))
	{
	case 0:
		return EBCT_TYPE_NO_POINT;
	case 1:
		return EBCT_TYPE_BULL_ONE;
	case 2:
		return EBCT_TYPE_BULL_TWO;
	case 3:
		return EBCT_TYPE_BULL_THREE;
	case 4:
		return EBCT_TYPE_BULL_FOUR;
	case 5:
		return EBCT_TYPE_BULL_FIVE;
	case 6:
		return EBCT_TYPE_BULL_SIX;
	case 7:
		return EBCT_TYPE_BULL_SEVEN;
	case 8:
		return EBCT_TYPE_BULL_EIGHT;
	case 9:
		return EBCT_TYPE_BULL_NINE;
	case 10:
		return EBCT_TYPE_BULL_BULL;
	}
	
	return EBCT_TYPE_NO_POINT;
}

///�Ƚ��������ƵĴ�С
///@param: WORD wFirstCard[] [in] ��һ���Ƽ�
///@param: int  iFirstCount  [in] ��һ��������
///@param: WORD wSecondCard[] [in] �ڶ����Ƽ�
///@param: int  iSecondCount  [in] �ڶ���������
///@return: int [out]  �Ƿ���
int CPokerGameLogic::CompareCard(BYTE wFirstCard[], int iFirstCount, BYTE wSecondCard[], int iSecondCount)
{
	BYTE wFirCard[5],wSecCard[5];

	::CopyMemory(wFirCard,wFirstCard,sizeof(BYTE)*iFirstCount);
	::CopyMemory(wSecCard,wSecondCard,sizeof(BYTE)*iSecondCount);


	SortByNumber(wFirCard,iFirstCount);
	SortByNumber(wSecCard,iSecondCount);	

 	BYTE iFirstCardShape = GetShape(wFirCard,iFirstCount);
 	BYTE iSecondCardShape = GetShape(wSecCard,iSecondCount);

	BYTE MaxFir = GetMaxCard(wFirCard, iFirstCount);
	BYTE MaxSec = GetMaxCard(wSecCard, iSecondCount);

 	if(iFirstCardShape != iSecondCardShape)  //�Ƚ�����
 	{		
 		return (iFirstCardShape - iSecondCardShape > 0) ? 1 : -1;
 	}
 	else if(MaxFir != MaxSec)						//�Ƚ���ֵ��С
 	{	
		if (GetCardBulk(MaxFir) >=_ECN_NUM_Joker && GetCardBulk(MaxSec)>=_ECN_NUM_Joker)
		{
			return (GetCardBulk(MaxFir) - GetCardBulk(MaxSec) > 0) ? 1 : -1;
		}
		if (GetCardBulk(MaxFir) >=_ECN_NUM_Joker || GetCardBulk(MaxSec)>=_ECN_NUM_Joker)
		{
			return GetCardBulk(MaxFir) >= _ECN_NUM_Joker ? -1 : 1;
		}	
		if (GetCardBulk(MaxFir) != GetCardBulk(MaxSec)) //����ţţ�ͱȽ������
		{
			return (GetCardBulk(MaxFir) - GetCardBulk(MaxSec) > 0) ? 1 : -1;
		}
		else
		{
			if(m_bCompareKind)
			{
				BYTE byFirsCardColor = (MaxFir&0xF0);
				BYTE bySecCardColor = (MaxSec&0xF0);
				return (byFirsCardColor - bySecCardColor > 0) ? 1 : -1 ; 
			}
			else
			{
				return -1 ; 
			}
		}
	}	
	return 0;
}
///�������е���
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@return: int [out]  �����Ƶ���
int CPokerGameLogic::CountPoint(BYTE  wCardList[],int iCardCount)
{
	int point = 0;
	for (int i=0;i<iCardCount;i++)
	{
		int temp = GetCardNumber(wCardList[i]);
		point += temp;
	}
	return point;
}
///ͳ��ѡ��ָ���������Ƿ�������20,10,0�������Ϊ��0ֵ,��ʾ���µ�����,����0��ʾ������
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@return: int [out]  �����Ƶ���

int CPokerGameLogic::CanSumIn(BYTE wCardList[],int iCardCount,int iSelectNum)
{
	int total = CountPoint(wCardList,iCardCount);
	for (int i=0; i<3; i++)
	{
		if (GetCardNumber(wCardList[i]) == _ECN_NUM_Joker
			|| GetCardNumber(wCardList[i]) == _ECN_NUM_JOKER)
		{
			continue;
		}
		for (int j=i+1; j<4; j++)
		{
			if (GetCardNumber(wCardList[j]) == _ECN_NUM_Joker
				|| GetCardNumber(wCardList[j]) == _ECN_NUM_JOKER)
			{
				continue;
			}
			for (int k = j+1; k<iCardCount; k++)
			{
				if (GetCardNumber(wCardList[k]) == _ECN_NUM_Joker
					|| GetCardNumber(wCardList[k]) == _ECN_NUM_JOKER)
				{
					continue;
				}

				int temp = GetCardNumber(wCardList[i])+GetCardNumber(wCardList[j])+GetCardNumber(wCardList[k]);
				if (temp==30||temp==10||temp==20)
				{
					return total-temp;
				}
			}
		}
	}
	return -1;
}

///�Զ���ţ
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@param: WORD wBullCard[] [out] �ҵ���ţ��
///@return: int [out]  �����Ƶ���
bool CPokerGameLogic::GetBull(BYTE wCardList[],int iCardCount,BYTE wBullCard[])
{
	int total = CountPoint(wCardList,iCardCount);
	for (int i=0; i<3; i++)
	{
		if (GetCardNumber(wCardList[i]) == _ECN_NUM_Joker
			|| GetCardNumber(wCardList[i]) == _ECN_NUM_JOKER)
		{
			continue;
		}
		for (int j=i+1; j<4; j++)
		{
			if (GetCardNumber(wCardList[j]) == _ECN_NUM_Joker
				|| GetCardNumber(wCardList[j]) == _ECN_NUM_JOKER)
			{
				continue;
			}
			for (int k = j+1; k<iCardCount; k++)
			{
				if (GetCardNumber(wCardList[k]) == _ECN_NUM_Joker
					|| GetCardNumber(wCardList[k]) == _ECN_NUM_JOKER)
				{
					continue;
				}
				int temp = GetCardNumber(wCardList[i])+GetCardNumber(wCardList[j])+GetCardNumber(wCardList[k]);
				if (temp==30||temp==10||temp==20)
				{
					wBullCard[0]=wCardList[i];
					wBullCard[1]=wCardList[j];
					wBullCard[2]=wCardList[k];
					return true;
				}
			}
		}
	}
	if (2 == CountKing(wCardList,iCardCount)) //˫��һ����ţţ
	{
		int index = 0;
		int kingCount = 0;
		for (int k=0; k<5; k++)
		{
			if (GetCardNumber(wCardList[k]) == _ECN_NUM_Joker
				&& kingCount == 0)
			{
				kingCount++;
				wBullCard[index++] = wCardList[k];
			}
			if (2 == index && 0 == kingCount)
			{
				continue;
			}
			else
			{
				if (GetCardNumber(wCardList[k]) != _ECN_NUM_Joker
					&& GetCardNumber(wCardList[k]) != _ECN_NUM_JOKER)
				{
					wBullCard[index++] = wCardList[k];
				}
			}
			if (index == 3 && kingCount == 1)
			{
				return true;
			}
		}
	}
	if (1 == CountKing(wCardList,iCardCount))//����ţ
	{
		int iMaxPoint = 0;
		int iTempPoint= 0;
		int index = 0;
		for (int i=0; i<4; i++)
		{
			if (GetCardNumber(wCardList[i]) == _ECN_NUM_Joker
				|| GetCardNumber(wCardList[i]) == _ECN_NUM_JOKER)
			{
				continue;
			}
			for (int j=i+1; j<5; j++)
			{
				if (GetCardNumber(wCardList[j]) == _ECN_NUM_Joker
					|| GetCardNumber(wCardList[j]) == _ECN_NUM_JOKER)
				{
					continue;
				}
				iTempPoint = GetCardNumber(wCardList[j]) + GetCardNumber(wCardList[i]);
				if (iTempPoint%10 == 0)
				{
					iMaxPoint = iTempPoint%10;
					index = 0;
					for (int k=0; k<5; k++)
					{
						if (k!=i && k!=j)
						{
							wBullCard[index++] = wCardList[k];
						}
						if (index == 3)
						{
							return true;
						}						
					}
				}
				else
					if (iTempPoint%10 > iMaxPoint)
					{
						iMaxPoint = iTempPoint%10;
						index = 0;
						for (int k=0; k<5; k++)
						{
							if (k!=i && k!=j)
							{
								wBullCard[index++] = wCardList[k];
							}
							if (index == 3)
							{
								break;
							}
						}
					}
			}
		}
		if (index == 3)
		{
			return true;
		}
	}
	return false;
}


///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@return: bool [out]  �����Ƿ�ţ
bool CPokerGameLogic::IsBullBull(BYTE  wCardList[],int iCardCount)
{
	int total=CountPoint(wCardList,iCardCount);
	if(CanSumIn(wCardList,iCardCount,3)==-1)
		return false;
	if((total > 0) && (total%10 == 0) && (CanSumIn(wCardList,iCardCount,3)!=-1))
		return true;
	return false;
}

///�õ��Ƽ������������������ϣ��Ա��������ţ
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@param: WORD wBullCard[] [out] �����Ƽ�
///@return: int [out]  ������
int CPokerGameLogic::GetMaxPoint(BYTE  wCardList[],int iCardCount,BYTE wBullCard[])
{
	int iMaxPoint = 0;
	int iTempPoint= 0;

	bool bFlag = false;
	int iOne = 0xff;
	int iTwo = 0xff;
	int iThree = 0xff;
	for(int i= 0x00;i < 0x03;i ++)
	{
		if (GetCardNumber(wCardList[i]) == _ECN_NUM_Joker|| GetCardNumber(wCardList[i]) == _ECN_NUM_JOKER)
		{
			continue;
		}
		for(int j = i + 0x01;j < 0x04;j ++)
		{
			if (GetCardNumber(wCardList[j]) == _ECN_NUM_Joker|| GetCardNumber(wCardList[j]) == _ECN_NUM_JOKER)
			{
				continue;
			}
			for(int k = j + 1;k < 0x05; k ++)
			{
				if (GetCardNumber(wCardList[k]) == _ECN_NUM_Joker|| GetCardNumber(wCardList[k]) == _ECN_NUM_JOKER)
				{
					continue;
				}
				int sum = wCardList[i] + wCardList[j] + wCardList[k];
				if(sum % 10 == 0x00)
				{
					iOne = i;
					iTwo = j;
					iThree = k;
					bFlag = true;
					break;
				}
			}
			if(bFlag)
			{
				break;
			}
		}
		if(bFlag)
		{
			break;
		}
	}
	if(bFlag)
	{
		int j = 0x00;
		for(int i = 0x00;i < 0x05;i ++)
		{
			if(i != iOne || i != iTwo || i != iThree)
			{
				wBullCard[j ++] = wCardList[i];
			}
		}
		return 0x00;
	}

	for (int i=0; i<4; i++)
	{
		if (GetCardNumber(wCardList[i]) == _ECN_NUM_Joker
			|| GetCardNumber(wCardList[i]) == _ECN_NUM_JOKER)
		{
			continue;
		}
		for (int j=i+1; j<5; j++)
		{
			if (GetCardNumber(wCardList[j]) == _ECN_NUM_Joker
				|| GetCardNumber(wCardList[j]) == _ECN_NUM_JOKER)
			{
				continue;
			}
			iTempPoint = GetCardNumber(wCardList[j]) + GetCardNumber(wCardList[i]);
			if (iTempPoint%10 == 0)
			{
				iMaxPoint = iTempPoint%10;
				wBullCard[0] = wCardList[i];
                wBullCard[1] = wCardList[j];
				return 0x00;
			}
			else if (iTempPoint%10 > iMaxPoint)
			{
				iMaxPoint = iTempPoint%10;
				wBullCard[0] = wCardList[i];
                wBullCard[1] = wCardList[j];
			}
		}
	}
	return iMaxPoint;
}
///�Ƿ�Ϊ�е�
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@return: int [out]  ���ص���
int CPokerGameLogic::IsHaveNote(BYTE  wCardList[],int iCardCount)
{
	int Note = CanSumIn(wCardList, iCardCount, 3);
	if((Note!=-1) && (Note%10!=0)) //��ţ
	{
		return (Note % 10);
	}
	else if((Note!=-1) && (Note%10==0)) //ţţ
	{
		return 10;
	}
	else if(Note == -1)  //��ţ
	{
		return 0;
	}
	return 0;	
}
///�õ�������������(����ɫ)
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@return: int [out]  �����
BYTE CPokerGameLogic::GetMaxCard(BYTE  wCardList[],int iCardCount)
{
	int temp = 0;
	BYTE card = 0;
	for (int i = 0; i < iCardCount; i++)
	{
		if (temp == 0)
		{
			temp = GetCardBulk(wCardList[i]);
			card = wCardList[i];
		}
		else
		{
			if (temp < GetCardBulk(wCardList[i])) 
			{
				temp = GetCardBulk(wCardList[i]);
				card = wCardList[i];
			}
			else if (temp == GetCardBulk(wCardList[i]))
			{
				if(GetCardShape(card) < GetCardShape(wCardList[i]))
				{
					temp = GetCardBulk(wCardList[i]);
					card = wCardList[i];
				}
			}
		}
	}
	return card;
}
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@return: bool [out]  �Ƿ�ţ
bool CPokerGameLogic::IsBull(BYTE  wCardList[],int iCardCount)
{
	int temp=0;
	for (int i=0; i<iCardCount; i++)
	{
		temp += GetCardNumber(wCardList[i]);
	}
	return temp % 10 == 0;
}
///@param: WORD wCardList[] [in] �Ƽ�
///@param: int  iCardCount  [in] ������
///@return: int [out]  ������
int  CPokerGameLogic::CountKing(BYTE  wCardList[],int iCardCount)
{
	int temp=0;
	for (int i=0; i<iCardCount; i++)
	{
		if (GetCardNumber(wCardList[i]) == _ECN_NUM_Joker
			|| GetCardNumber(wCardList[i]) == _ECN_NUM_JOKER)
		{
			temp++;
		}
	}
	return temp;
}
//===================================================================================
//��ȡ�Ƶ�
//===================================================================================
BYTE CPokerGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	BYTE cbPipCount=0;

	//��ȡ�Ƶ�
	BYTE cbCardValue=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		cbCardValue=GetCardNumber(cbCardData[i]);
		if(cbCardValue==0x0D)					//�����A��1��
		{
			cbCardValue=0x01;
			cbPipCount+=cbCardValue;
		}
		else
		{
			cbPipCount+=(++cbCardValue);
		}
	}
	return (cbPipCount%10);
}

