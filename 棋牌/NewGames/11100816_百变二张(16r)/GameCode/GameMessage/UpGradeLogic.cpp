#include "StdAfx.h"
#include "UpGradeLogic.h"

/// ��������ͼ�е�����������,�����ͼ
static const BYTE cardData[PAI_COUNT]=
{
	55,                 /** �������A */
	2,13 * 2 + 2,       /** 2 �ź�2 */
	3 * 13 + 3,         /** ����3 */
	12,13 * 2 + 12,     /** ����Q */
	24,50,              /** ����J */
	10,23,36,49,        /** ����10 */
	9,13 * 2 + 9,       /** ����9 */
	8,21,34,47,         /** ����8 */
	7,20,33,46,         /** ����7 */
	6,19,32,45,         /** ����6 */
	5,13 * 2 + 5,       /** 2��5 */
	4,17,30,43          /** ����4 */
};

/**
* ���캯��
*/
GameLogic::GameLogic(void)
{
	m_bGameVersion = GAME_VERSION_1;
	m_bKingAndThreeBig = TRUE;
	m_bUseCard = FALSE;
	m_bRedJOrBlackJ = FALSE;

	InitGame();
}

/** 
* ��������
*/
GameLogic::~GameLogic(void)
{
}

/**
* ��ʼ������
*/
void  GameLogic::InitGame()
{
	// ��ʼ���ƾ�
	memset(m_cardAry,0,sizeof(m_cardAry));	
}


/** 
* ���һ�����˿�
*/
BYTE GameLogic::RandACard()
{
	BYTE m_CardArray[PAI_COUNT-1] =
	{
		/*0x4F, */0x01, 0x21, 0x32,
		0x0B, 0x2B, 0x1A, 0x3A,
		0x09, 0x19, 0x29, 0x39,
		0x08, 0x28,
		0x07, 0x17, 0x27, 0x37,
		0x06, 0x16, 0x26, 0x36,
		0x05, 0x15, 0x25, 0x35,
		0x04, 0x24,
		0x03, 0x13, 0x23, 0x33,
	};

	srand((unsigned)GetCurrentTime());
	
	return m_CardArray[rand()%(PAI_COUNT-1)];
}

/** 
* ��ʮ����������ת����16��������,����ʮ������ֵ
* @param bValue ��ֵ
* @return ����ת�������ֵ
*/
BYTE  GameLogic::GetValue(BYTE bValue)
{
	//static const BYTE m_CardArray[54] =
	//{
	//	0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
	//	0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
	//	0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
	//	0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
	//	0x4E, 0x4F																			//С�����
	//};

	BYTE m_CardArray[PAI_COUNT] =
	{
		0x4F, 0x01, 0x21, 0x32,
		0x0B, 0x2B, 0x1A, 0x3A,
		0x09, 0x19, 0x29, 0x39,
		0x08, 0x28,
		0x07, 0x17, 0x27, 0x37,
		0x06, 0x16, 0x26, 0x36,
		0x05, 0x15, 0x25, 0x35,
		0x04, 0x24,
		0x03, 0x13, 0x23, 0x33,
	};

	// �ж��Ƿ�Ϊ�����汾
	if (m_bUseCard)
	{
		// ����ת���ɺ�A
		m_CardArray[0] = 0x3D;
	}

	if(m_bRedJOrBlackJ)
	{
		m_CardArray[6] = 0x0A;
		m_CardArray[7] = 0x2A;
	}

	for (int i = 0; i < PAI_COUNT; i++)
	{
		if (cardData[i] == bValue)
		{
			return m_CardArray[i];
		}
	}

	return 0;
}

/** 
* ϴ ��
* @param [in] bRecordCard  TRUE: ��ϴ�Ƽ�¼��ӡ���ı��в鿴(72���ƣ�ϴ72��)
*                         FALSE: ֻϴһ����, ����ӡ���ı���
*/
void GameLogic::GameXiPai(BOOL bRecordCard)
{
	// ��ʼ���ƾ�����
	BYTE bCard[PAI_COUNT], bCardRand[PAI_COUNT];

	if (!bRecordCard)
	{
		// ���Ƴ�ʼ����
		CopyMemory(bCard, cardData, sizeof(bCard));

		// ϴ ��
		srand((unsigned)time(NULL));
		BYTE iSend = 0, iStation = 0;
		do
		{
			iStation = rand()%(PAI_COUNT-iSend);
			bCardRand[iSend] = bCard[iStation];
			iSend++;
			bCard[iStation] = bCard[PAI_COUNT-iSend];
		} while (iSend < PAI_COUNT);

		CopyMemory(m_cardAry,bCardRand,sizeof(m_cardAry));

		// ��ÿ����ҷ���
		GameFaPai();
	}
	else
	{
		// ϴ72���ƴ�ŵ�������
		char i = 0, j = 0, k = 0, nPai = 0;

		for (i = 0; i < GAME_XIPAI_COUNT; i++)
		{
			memset(bCard, 0, sizeof(bCard));

			memset(bCardRand, 0, sizeof(bCardRand));

			// ���Ƴ�ʼ����
			CopyMemory(bCard, cardData, sizeof(bCard));

			// ϴ ��
			srand((unsigned)(GetTickCount() + i));
			
			BYTE iSend = 0, iStation = 0;
			do
			{
				iStation = rand()%(PAI_COUNT-iSend);
				bCardRand[iSend] = bCard[iStation];
				iSend++;
				bCard[iStation] = bCard[PAI_COUNT-iSend];
			} while (iSend < PAI_COUNT);

			// ��ϴ�ƽ����ŵ���������
			nPai = 0;
			for (j = 0; j < 4; j++)
			{
				for (k = 0; k < GAMER_CARD; k++)
				{
					m_GameCard[i][j].paiAry[k] = bCardRand[nPai];
					nPai++;
				}
			}
		}
	}
}

/** 
* ����
*/
void  GameLogic::GameFaPai()
{
	int nPai = 0;
	for(int i = 0;i < 4; i++)
	{
		for(int j = 0; j < GAMER_CARD; j++)
		{
			m_userData[i].paiAry[j] = m_cardAry[nPai];
			nPai++;
		}
	}

	return;
}

/** 
* ����ĳ������������
* @param [in] userStation ���λ��
* @param [in] firstCard   ��һ����
* @param [in] secondCard  �ڶ�����
* @return ��������
*/
BOOL  GameLogic::SetUserCard(BYTE userStation,BYTE firstCard,BYTE secondCard)
{
	if (PAI_IS_VALUE(firstCard) && PAI_IS_VALUE(secondCard))
	{
		if (userStation >=0 && userStation <4)
		{
			m_userData[userStation].paiAry[0] = firstCard;
			m_userData[userStation].paiAry[1] = secondCard;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		return  FALSE;
	}
}

/** 
* �����ֵ�Ƿ�Ϸ�
* @param [in] userStation ���λ��
* @param [in] paiData ������
* @return ��������
*/
BOOL GameLogic::SetUserCard(BYTE userStation, PaiData & paiData)
{
	return SetUserCard(userStation,paiData.paiAry[0],paiData.paiAry[1]);
}

/** 
* �����ֵ�Ƿ�Ϸ�
* @param [in] paiData ������
* @return ��������
*/
BOOL  GameLogic::IsPaiValue(const PaiData & paiData)
{
	if (PAI_IS_VALUE(paiData.paiAry[0]) && PAI_IS_VALUE(paiData.paiAry[1]))
	{
		return TRUE;
	}

	return FALSE;
}

/** 
* ����û������Ƿ��Ƕ���
* @param [in] paiData �û����е�������
* @return ��������
*/
BOOL   GameLogic:: IsCardDouble(const PaiData & paiData)
{
	if (IsPaiValue(paiData))
	{
		BYTE first = paiData.paiAry[0],second = paiData.paiAry[1];
		if ((first + 13 * 2 == second) || (second + 13 * 2 == first) //����
			|| IsCompagasCard(paiData))
		{
			return TRUE;
		}
	}

	return FALSE;
}

/** 
* �ж������
* @param [in] paiData ������
* @return ��������
*/
BOOL  GameLogic::IsCompagasCard(const PaiData & paiData)
{
	if ((paiData.paiAry[0] == 42 && paiData.paiAry[1] == 55) || (paiData.paiAry[0] == 55 && paiData.paiAry[1] == 42)    //�������A�ͺ�3���

       || ((paiData.paiAry[0] == 12 || paiData.paiAry[0] == 38) && (paiData.paiAry[1] == 9 || paiData.paiAry[1] == 35)) //Q��9���
	   || ((paiData.paiAry[1] == 12 || paiData.paiAry[1] == 38) && (paiData.paiAry[0] == 9 || paiData.paiAry[0] == 35)) //Q��9���

	   || ((paiData.paiAry[0] == 12 || paiData.paiAry[0] == 38)   
	   && (paiData.paiAry[1] == 8 || paiData.paiAry[1] == 21 || paiData.paiAry[1] == 34 || paiData.paiAry[1] == 47))    //Q��8���

	   || ((paiData.paiAry[1] == 12 || paiData.paiAry[1] == 38)   
	   && (paiData.paiAry[0] == 8 || paiData.paiAry[0] == 21 || paiData.paiAry[0] == 34 || paiData.paiAry[0] == 47))    //Q��8���

	   || ((paiData.paiAry[0] == 2 || paiData.paiAry[0] == 28)   
	   && (paiData.paiAry[1] == 8 || paiData.paiAry[1] == 21 || paiData.paiAry[1] == 34 || paiData.paiAry[1] == 47))    //2��8���

	   || ((paiData.paiAry[1] == 2 || paiData.paiAry[1] == 28)   
	   && (paiData.paiAry[0] == 8 || paiData.paiAry[0] == 21 || paiData.paiAry[0] == 34 || paiData.paiAry[0] == 47)))    //2��8���
	{
		return TRUE;
	}

	return FALSE;
}

/** 
* �����û��Ƶ���
* @param [in] paiData ������
* @return �����Ƶ���
*/
int GameLogic::CalcCardValue(const PaiData & paiData)
{
	if (IsPaiValue(paiData))
	{
		BYTE first = paiData.paiAry[0],second = paiData.paiAry[1];
		int paiDianSum = 0;
		paiDianSum = GetCardValue(first) + GetCardValue(second);
		return paiDianSum % 10;
	}	
	return 0;
}

/** 
* ��ȡ�Ƶĵ���
* @param [in] card ������
* @return �����Ƶ���
*/
int GameLogic::GetCardValue(BYTE  card)
{
	if (card >= PAI_VALUE_MIN && card <= PAI_VALUE_MAX)
	{
		if (card == 55) 
		{
			// �������A
			return 6;
		}
		else
		{
			return (card % 13) % 10;
		}
	}
	return 0;
}

/** 
* ��ȡ��������Ƶ�����
* @param [in] paiData ������
* @return ����������������
*/
PAI_XING GameLogic::GetCardPaiXing(const PaiData & paiData)
{
	if (IsPaiValue(paiData))
	{
		BYTE first = paiData.paiAry[0],second = paiData.paiAry[1];
		if (((first == 9 || first == 35) && (second == 12 || second == 38))|| 
			((first == 12 || first == 38) && (second == 9 || second == 35)))
		{
			//�����
			return PAI_TIANJIUWANG;
		}
		if (((first == 12 || first == 38) && second % 13 == 8) || 
			((second == 12 || second == 38) && first % 13 == 8))
		{
			//���
			return PAI_TIANGANG;
		}
		if (((first == 2 || first == 28) && second % 13 == 8) || 
			((second == 2 || second == 28) && first % 13 == 8))
		{
			//�ظ�
			return PAI_DIGANG;
		}
		if (((first == 12 || first == 38) && second % 13 == 7) || 
			((second == 12 || second == 38) && first % 13 == 7))
		{
			//���ƾ�
			return PAI_TIANPAIJIU;
		}
	}

	return PAI_ERROR;
}

/** 
* �õ��û����е��ƶ��ӵĴ�Сֵ
* @param [in] paiData ������
* @param [in] bGameMode ��Ϸ�汾 0: ��ͨ 1: ������ 2: ���«2��
* @return ����������������
*/
PAI_DOUBLE_NAME  GameLogic::GetCardDouble(const PaiData & paiData, BYTE bGameMode)
{
	if (IsCardDouble(paiData))
	{
		BYTE first = paiData.paiAry[0],second = paiData.paiAry[1];
		if ((first == 55 && second == 42) || (second == 55 && first == 42))
		{
			// m_bKingAndThreeBig ���ִ�������3 �� ˫��Q ��С(TRUE: ������3��� FALSE: ˫��Q���)
			return m_bKingAndThreeBig ? PAI_KIND_HUANGDI : PAI_KIND_HUANGDI_EX;
		}
		if ((first == 12 && second == 38) || (second == 12 && first == 38))		//��Q + ��Q
		{
			// ����
			return PAI_KIND_DUITIAN;
		}
		if ((first == 2 && second == 28) || (second == 2 && first == 28))		//��2 + ��2
		{
			// �Ե�
			return PAI_KIND_DUIDI;
		}
		if ((first == 8 && second == 34) || (second  == 8 && first == 34))		//��8 + ��8
		{
			// ����
			return PAI_KIND_DUIREN;
		}
		if ((first == 4 && second == 30) || (second == 4 && first == 30))		//��4 + ��4
		{
			// �Ժ�
			return PAI_KIND_DUIHE;
		}
		if ((first == 17 && second == 43) || (second == 17 && first == 43) //˫���		//÷4 + ��4
			|| (first == 19 && second == 45) || (second == 19 && first == 45)//˫����	//÷6 + ��6
			|| (first == 23 && second == 49) || (second == 23 && first == 49)) //˫÷	//÷10 + ��10
		{
			// �Ժ��ġ��Ժ������Ժ�ʮ
			return PAI_KIND_DUIZI_1;
		}
		if ((first == 6 && second == 32) || (second == 6 && first == 32)  ///˫������		//��6 + ��6
			|| (first == 7 && second == 33) || (second == 7 && first == 33)///�߽���			//��7 + ��7
			|| (first == 10 && second == 36) || (second == 10 && first == 36)///˫��ͷ		//��10 + ��10
			|| (first == 24 && second == 50) || (second == 24 && first == 50)) ///˫��ͷ		//÷J + ��J
		{
			// �Ժ������Ժ��ߡ��Ժ�ʮ���Ժ�J
			return PAI_KIND_DUIZI_2;
		}
		if ((first == 5 && second == 31) || (second == 5 && first == 31)  ///����			//��5 + ��5
			|| (first == 9 && second == 35) || (second == 9 && first == 35)///�Ӿ�			//��9 + ��9
			|| (first == 20 && second == 46) || (second == 20 && first == 46)///����			//÷7 + ��7
			|| (first == 21 && second == 47) || (second == 21 && first == 47)) ///�Ӱ�		//÷8 + ��8
		{
			// �Ժ��塢�Ժ��ߡ��Ժڰˡ��Ժ��
			return PAI_KIND_DUIZI_3;
		}
		if ((first == 12 || first == 38) && (second == 9 || second == 35) 
			|| (second == 12 || second == 38) && (first == 9 || first == 35))
		{
			// Q + 9���
			return PAI_KIND_Q9;
		}
		if ((first == 12 || first == 38) && (second == 8 || second == 21 || second == 34 || second == 47)
			|| (second == 12 || second == 38) && (first == 8 || first == 21 || first == 34 || first == 47))
		{
			// Q + 8���
			return PAI_KIND_Q8;
		}
		if ((first == 2 || first == 28) && (second == 8 || second == 21 || second == 34 || second == 47)
			|| (second == 2 || second == 28) && (first == 8 || first == 21 || first == 34 || first == 47))
		{
			// 2 + 8���
			return PAI_KIND_28;
		}
	}

	return PAI_DUIZI_ERROR;
}

/** 
* ��ö�����������һ�ŵ��ƻ����ĵ���λ��
* @param [in] paiData ������
* @param [in] bSeat   TRUE: ���ض�����������һ�ŵ���λ��, FALSE: ���ض�����������һ�ŵ���
* @return ���ص���ֵ����λ��(bSeatΪTRUE�������)
*/
int  GameLogic::GetSingleCardMax(const PaiData & paiData, BOOL bSeat)
{
	BYTE first = paiData.paiAry[0], second = paiData.paiAry[1];
	if (first == 12 || first == 38)
	{
		//����
		return bSeat?0:PAI_KIND_DUITIAN;  // ���ص�0�������
	}
	if (second == 12 || second == 38)
	{
		//����
		return bSeat?1:PAI_KIND_DUITIAN;  // ���ص�1�������
	}

	if (first == 2 || first == 28)
	{
		//�Ե�
		return bSeat?0:PAI_KIND_DUIDI;
	}
	if (second == 2 || second == 28)
	{
		//�Ե�
		return bSeat?1:PAI_KIND_DUIDI;
	}

	if (first == 8 || first == 34)
	{
		//����
		return bSeat?0:PAI_KIND_DUIREN;
	}
	if (second  == 8 || second == 34)
	{
		//����
		return bSeat?1:PAI_KIND_DUIREN;
	}

	if (first == 4 || first == 30)
	{
		//�Ժ�
		return bSeat?0:PAI_KIND_DUIHE;
	}
	if (second == 4 || second == 30)
	{
		//�Ժ�
		return bSeat?1:PAI_KIND_DUIHE;
	}

	if (first == 23 || first == 49)
	{
		//�Ժ�ʮ
		return bSeat?0:PAI_KIND_DUIZI_1;
	}
	if (second == 23 || second == 49)
	{
		//�Ժ�ʮ
		return bSeat?1:PAI_KIND_DUIZI_1;
	}

	if (first == 19 || first == 45)
	{
		//�Ժ���
		return bSeat?0:PAI_KIND_DUIZI_1;
	}
	if (second == 19 || second == 45)
	{
		//�Ժ���
		return bSeat?1:PAI_KIND_DUIZI_1;
	}

	if (first == 17 || first == 43)
	{
		//�Ժ���
		return bSeat?0:PAI_KIND_DUIZI_1;
	}
	if (second == 17 || second == 43)
	{
		//�Ժ���
		return bSeat?1:PAI_KIND_DUIZI_1;
	}

	if (first == 24 || first == 50)
	{
		//�Ժ�J
		return bSeat?0:PAI_KIND_DUIZI_2;
	}
	if (second == 24 || second == 50)
	{
		//�Ժ�J
		return bSeat?1:PAI_KIND_DUIZI_2;
	}

	if (first == 10 || first == 36)
	{
		//�Ժ�ʮ
		return bSeat?0:PAI_KIND_DUIZI_2;
	}
	if (second == 10 || second == 36)
	{
		//�Ժ�ʮ
		return bSeat?1:PAI_KIND_DUIZI_2;
	}

	if (first == 7 || first == 33)
	{
		//�Ժ���
		return bSeat?0:PAI_KIND_DUIZI_2;
	}
	if (second == 7 || second == 33)
	{
		//�Ժ���
		return bSeat?1:PAI_KIND_DUIZI_2;
	}

	if (first == 6 || first == 32)
	{
		//�Ժ���
		return bSeat?0:PAI_KIND_DUIZI_2;
	}
	if (second == 6 || second == 32)
	{
		//�Ժ���
		return bSeat?1:PAI_KIND_DUIZI_2;
	}

	if (first == 9 || first == 35)
	{
		//�Ժ��
		return bSeat?0:PAI_KIND_DUIZI_3;
	}
	if (second == 9 || second == 35)
	{
		//�Ժ��
		return bSeat?1:PAI_KIND_DUIZI_3;
	}

	if (first == 21 || first == 47)
	{
		//�Ժڰ�
		return bSeat?0:PAI_KIND_DUIZI_3;
	}
	if (second == 21 || second == 47)
	{
		//�Ժڰ�
		return bSeat?1:PAI_KIND_DUIZI_3;
	}

	if (first == 20 || first == 46)
	{
		//�Ժ���
		return bSeat?0:PAI_KIND_DUIZI_3;
	}
	if (second == 20 || second == 46)
	{
		//�Ժ���
		return bSeat?1:PAI_KIND_DUIZI_3;
	}

	if (first == 5 || first == 31)
	{
		//�Ժ���
		return bSeat?0:PAI_KIND_DUIZI_3;
	}
	if (second == 5 || second == 31)
	{
		//�Ժ���
		return bSeat?1:PAI_KIND_DUIZI_3;
	}

	return PAI_DUIZI_ERROR;
}

/** 
* ��ȡ�����Ƶ�����
* @param [in] paiDuiZi �ƶ�������ֵ
* @return ���ӵ�����
*/
char * GameLogic::GetDuiZiName(PAI_DOUBLE_NAME paiDuiZi)
{
	if (paiDuiZi == PAI_DUIZI_ERROR)
	{
		return NULL;
	}

	switch(paiDuiZi)
	{
	case PAI_KIND_HUANGDI:       //�ʵ�
	case PAI_KIND_HUANGDI_EX:
		{
			return TEXT("�ʵ�");
		}
	case PAI_KIND_DUITIAN:       //����
		{
			return  TEXT("����");
		}
	case PAI_KIND_DUIDI:         //�Ե�
		{
			return TEXT("�Ե�");
		}
	case PAI_KIND_DUIREN:	     //����
		{
			return TEXT("����");
		}
	case PAI_KIND_DUIHE:	 	 //�Ժ�
		{
			return TEXT("�Ժ�");
		}
	case PAI_KIND_DUIZI_1:       //�Ժ��ġ��Ժ������Ժ�ʮ
	case PAI_KIND_DUIZI_2:       //�Ժ������Ժ��ߡ��Ժ�ʮ���Ժ�J
	case PAI_KIND_DUIZI_3:       //�Ժ��塢�Ժ��ߡ��Ժڰˡ��Ժ��
		{
			return TEXT("����");
		}
	case PAI_KIND_Q9:            //Q + 9���
	case PAI_KIND_Q8:            //Q + 8���
	case PAI_KIND_28:            //2 + 8���
		{
			return TEXT("�������");
		}
	default:
		return NULL;
	}

	return NULL;
}

/**
* ��ȡ�Ƶ���������
* @param [in] paiXing ����������
* @return ��������
*/
char * GameLogic::GetPaiXingName(const PAI_XING & paiXing)
{
	if (paiXing == PAI_ERROR)
	{
		return NULL;
	}
	return g_PaiXing_Name[paiXing];
}

/**
* ��ȡ�Ƶ���������
* @param [in] paiData ������
* @return ��������
*/
CString  GameLogic::GetPaiValueName(PaiData & paiData)
{
	PAI_DOUBLE_NAME paiDui = GetCardDouble(paiData);
	if (paiDui != PAI_DUIZI_ERROR)
	{
		return GetDuiZiName(paiDui);
	}

	//PAI_XING paiXing = GetCardPaiXing(paiData);
	//if (paiXing != PAI_ERROR)
	//{
	//	return GetPaiXingName(paiXing);
	//}

	int paiDian = CalcCardValue(paiData);
	char szBuffer[10];
	sprintf(szBuffer,"%d ��", paiDian);

	return szBuffer;
	
}

/**
* ��ȡ�Ƶ���������
* @param [in] station ��λ��
* @return ��������
*/
CString GameLogic::GetPaiValueName(BYTE station)
{
	if (station >=0 && station < 4 )
	{
		PaiData  paiData(m_userData[station]);
		return GetPaiValueName(paiData);
	}

	return NULL;
}

/**
* �Ƚ�������ҵ��ƴ�С
* @param [in] paiData1 ��һ��������
* @param [in] paiData2 �ڶ���������
* @return ���ر��ƽ�� BIPAI_RET ֵ
*/
BIPAI_RET  GameLogic::BiPaiFromData(const PaiData & paiData1,const PaiData & paiData2)
{
	if (IsPaiValue(paiData1) && IsPaiValue(paiData2))
	{
		//�����ƶ��ӵ�ֵ
		PAI_DOUBLE_NAME paiDuiValue1 = PAI_DUIZI_ERROR,paiDuiValue2 = PAI_DUIZI_ERROR;
		if (IsCardDouble(paiData1))
		{
			paiDuiValue1 = GetCardDouble(paiData1, m_bGameVersion);
		}
		if (IsCardDouble(paiData2))
		{
			paiDuiValue2 = GetCardDouble(paiData2, m_bGameVersion);
		}

		//�������Ƕ���
		if (paiDuiValue1 != PAI_DUIZI_ERROR && paiDuiValue2 != PAI_DUIZI_ERROR)
		{
			if (paiDuiValue1 > paiDuiValue2)
			{
				return BIPAI_FIRST_WIN;     // ��һ���˴�
			}
			else if (paiDuiValue1 < paiDuiValue2)
			{
				return BIPAI_SECONED_WIN;   // �ڶ����˴�
			}
			else
			{
				return BIPAI_SAME;
			}
		}

		if (paiDuiValue1 != PAI_DUIZI_ERROR &&  paiDuiValue2 == PAI_DUIZI_ERROR)
		{
			return BIPAI_FIRST_WIN;         //��һ�����Ƕ��ӣ��ڶ����˲��ǣ����һ���˴�
		}
		if (paiDuiValue1 == PAI_DUIZI_ERROR &&  paiDuiValue2 != PAI_DUIZI_ERROR)
		{
			return BIPAI_SECONED_WIN;       //�ڶ������Ƕ��ӣ���һ���˲��ǣ���ڶ����˴�
		}

		//�����˶��ǵ���
		return DanPaiBiPai(paiData1, paiData2, m_bGameVersion);
	}
	return BIPAI_STATION_ERROR;
}

/**
* �Ƚ�������ҵ��ƴ�С
* @param [in] station1 station1λ��
* @param [in] station2 station2λ��
* @return ���ر��ƽ�� BIPAI_RET ֵ
*/
BIPAI_RET GameLogic::BiPaiFromStation(BYTE station1,BYTE station2)
{
	if (station1 >=0 && station1 < 4 && station2 >=0 && station2 <4)
	{
		PaiData  paiData1(m_userData[station1]);
		PaiData  paiData2(m_userData[station2]);

		//����
		BIPAI_RET retBiPai = BiPaiFromData(paiData1,paiData2);
		if ( retBiPai == BIPAI_SAME)
		{			
			return BIPAI_FIRST_WIN;
		}

		return retBiPai;
	}

	return BIPAI_STATION_ERROR;
}

/**
* �Ƚ������˵��ƵĴ�С
* @param [in] paiData1 ��һ��������
* @param [in] paiData2 �ڶ���������
* @param [in] bGameMode ��Ϸ�汾 0: ��ͨ 1: ������ 2: ���«2��
* @return ���ر��ƽ�� BIPAI_RET ֵ
*/
BIPAI_RET GameLogic::DanPaiBiPai(const PaiData & paiData1, const PaiData & paiData2, BYTE bGameMode)
{
	// �������ͣ��ȵ��Ƶĵ���
	int paiDian1 = 0,paiDian2 = 0;
	paiDian1 = CalcCardValue(paiData1);
	paiDian2 = CalcCardValue(paiData2);

	if (paiDian1 > paiDian2)
	{
		return  BIPAI_FIRST_WIN;
	}
	else if (paiDian1 < paiDian2)
	{
		return BIPAI_SECONED_WIN;
	}
	else
	{
		if (paiDian1 == 0 || 0 == bGameMode)
		{
			// 0��û�д�С֮��
			return BIPAI_SAME;
		}

		// �Ƶ�һ�������ȱȽ϶�����������һ����(��С�����ӳ��ֵ�˳��, �������A��3����С����)
		int cardKind1 = GetSingleCardMax(paiData1);   // ��ȡ��������
		int cardKind2 = GetSingleCardMax(paiData2);

		// �Ƚ�����������
		if (cardKind1 > cardKind2)
		{
			return BIPAI_FIRST_WIN;
		}
		else if (cardKind1 < cardKind2)
		{
			return BIPAI_SECONED_WIN;
		}
		else
		{
			switch (bGameMode)
			{
			case GAME_VERSION_1:  // ��ͨ�汾
			case GAME_VERSION_3:  // ���«2���汾
				{
					// ����һ����ͬ�����ٱȽϣ�������ȣ�ׯ��Ӯ
					return BIPAI_SAME;
				}
			case GAME_VERSION_2:  // �����Ӱ汾
				{
					// ������������һ������������Ƶ���
					int num1 = GetSingleCardMax(paiData1, TRUE);   //��ȡ������������һ����λ��
					int num2 = GetSingleCardMax(paiData2, TRUE);

					// ���ĵ���һ������Ƚ���һ����
					PaiData  paiData3(paiData1.paiAry[0], paiData1.paiAry[1]);
					PaiData  paiData4(paiData2.paiAry[0], paiData2.paiAry[1]);

					// ������λ��Ϊ0�������һ���ƵĴ�С
					paiData3.paiAry[num1] = 0;
					paiData4.paiAry[num2] = 0;

					int cardKind3 = GetSingleCardMax(paiData3);   //��ȡ��������
					int cardKind4 = GetSingleCardMax(paiData4);

					// �Ƚ���С��������
					if (cardKind3 > cardKind4)
					{
						return  BIPAI_FIRST_WIN;
					}
					else if (cardKind3 < cardKind4)
					{
						return BIPAI_SECONED_WIN;
					}

					return BIPAI_SAME;
				}
			default:
				break;
			}
		}

		return BIPAI_SAME;
	}
	
	return BIPAI_STATION_ERROR;	
}

/************************************************************************
* Function : SetGameMode(BYTE bGameVersion, BOOL bUseCard, BOOL bKingAndThreeBig)
* Author   : duanxiaohui
* Date     : 2010-08-05
* Descript : ������Ϸģʽ
* Parameter: BYTE bGameVersion     [in] ��Ϸ����
*          : BOOL bUseCard         [in] ������Ϸ��ֵ TRUE: ���� FALSE: ��A
*          : BOOL bKingAndThreeBig [in] �������ʹ�С TRUE: �������3��� FALSE: ˫��Q���
*		   : BOOL bKingAndThreeBig [in] ���ú�J���Ǻ�J FALSE: ��J TRUE: ��J
* return   : int ��������ID                                                        
************************************************************************/
void GameLogic::SetGameMode(BYTE bGameVersion, BOOL bUseCard, BOOL bKingAndThreeBig, BOOL bRedJOrBlackJ)
{ 
	m_bGameVersion = bGameVersion; 
	m_bUseCard = bUseCard; 
	m_bKingAndThreeBig = bKingAndThreeBig;
	m_bRedJOrBlackJ = bRedJOrBlackJ;

	CString str;
	str.Format(" wysoutlog::bGameVersion==%d,bUserCard=%d,bKingAndThreeBig==%d",bGameVersion,bUseCard,bKingAndThreeBig);
	OutputDebugString(str);
}