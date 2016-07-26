#include "StdAfx.h"
#include "UpGradeLogic.h"
//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	m_iStation[4]=500;
	for (int i=0;i<4;i++) m_iStation[i]=100*i;
	
}


//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard)
{ 
	int iHuaKind=(iCard&UG_HUA_MASK);
	return iHuaKind; 
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard, BOOL bExtVol)
{
	if ((iCard==0x4E)||(iCard==0x4F))
	{
		return bExtVol?(iCard-14):(iCard-62);	//��С��64+14-62=16			ֻ���ش�Сè��ֵ
	}

	int iCardNum=GetCardNum(iCard);
	int iHuaKind=GetCardHuaKind(iCard);

	return ((bExtVol)?((iHuaKind>>4)+(iCardNum*4)):(iCardNum));
}

//�����˿�
BOOL CUpGradeGameLogic::SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount)
{
	BOOL bSorted=TRUE,bTempUp;
	int iTemp,iLast=iCardCount-1,iStationVol[45];

	//��ȡλ����ֵ
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i],TRUE);
		///if (iStationVol[i]>=15) iStationVol[i]+=m_iStation[4];
		///else iStationVol[i]+=m_iStation[GetCardHuaKind(iCardList[i],FALSE)>>4];
	}

	//�������
	do
	{
		bSorted=TRUE;
		for (int i=0;i<iLast;i++)
		{
			if (iStationVol[i]<iStationVol[i+1])
			{	
				//����λ��
				iTemp=iCardList[i];
				iCardList[i]=iCardList[i+1];
				iCardList[i+1]=iTemp;
				iTemp=iStationVol[i];
				iStationVol[i]=iStationVol[i+1];
				iStationVol[i+1]=iTemp;
				if (bUp!=NULL)
				{
					bTempUp=bUp[i];
					bUp[i]=bUp[i+1];
					bUp[i+1]=bTempUp;
				}
				bSorted=FALSE;
			}	
		}
		iLast--;
	} while(!bSorted);
		
	return TRUE;
}



//��������
BOOL CUpGradeGameLogic::ReSortCard(BYTE iCardList[], int iCardCount)
{
	SortCard(iCardList,NULL,iCardCount);
	//====�������Ŵ�С
	int iStationVol[45];
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i],false);
	}

	int Start=0;
	int j,step;
	BYTE CardTemp[8];					//��������Ҫ��λ������
	int CardTempVal[8];					//����������λ������ֵ
	for(int i=8;i>1;i--)				//����������һ������i����ͬ��ֵ
	{
		for(j=Start;j<iCardCount;j++)
		{
			CardTemp[0]=iCardList[j];			//���浱ǰi��������ͬ��ֵ
			CardTempVal[0]=iStationVol[j];
				for(step=1;step<i&&j+step<iCardCount;)			//��һ������i��ֵ��ȵ�����(����������ʱ������)
				{
					if(iStationVol[j]==iStationVol[j+step])
					{
						CardTemp[step]=iCardList[j+step];			//������������
						CardTempVal[step]=iStationVol[j+step];		//��ֵ
						step++;
					}
					else
						break;
				}

			if(step>=i)			//�ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj,����λ��Ϊj+setp-1
			{					//����Start��ʼ��j���������setp��
				if(j!=Start)				//�ų���ʼ��������
				{
					for(;j>=Start;j--)					//��Start����j�ź��ƶ�i��
						{
							iCardList[j+i-1]=iCardList[j-1];
							iStationVol[j+i-1]=iStationVol[j-1];
						}
					for(int k=0;k<i;k++)				
					{
						iCardList[Start+k]=CardTemp[k];	//��Start��ʼ���ó�CardSave
						iStationVol[Start+k]=CardTempVal[k];
					}
				}
				Start=Start+i;
			}
			j=j+step-1;
		}
	}
	return true;
}


//��ȡ�˿�
BYTE CUpGradeGameLogic::GetCardFromHua(int iHuaKind, int iNum)
{
	if (iHuaKind!=UG_ERROR_HUA) return (iHuaKind+iNum-1);
	return 0;
}


BOOL CUpGradeGameLogic::IsSpecial(BYTE iCardList[],int iCardCount)
{
	if(iCardCount!=3)
		return false;
	SortCard(iCardList,NULL,iCardCount);
	if(GetCardNum(iCardList[0])==5
		&&GetCardNum(iCardList[1])==3
		&&GetCardNum(iCardList[2])==2)
		return true;
	return false;
}



//�õ����л�ɫ
BYTE CUpGradeGameLogic::GetCardListHua(BYTE iCardList[], int iCardCount)
{
	int iHuaKind=GetCardHuaKind(iCardList[0]);
	if (GetCardHuaKind(iCardList[iCardCount-1])!=iHuaKind) return UG_ERROR_HUA;
	return iHuaKind;
}



//�Ƚ��������ƵĴ�С
int CUpGradeGameLogic::CompareCard(BYTE iFirstCard[], int iFirstCount, BYTE iSecondCard[], int iSecondCount)
{
	//***************************************************/
	BYTE iFirCard[5],iSecCard[5];

	::CopyMemory(iFirCard,iFirstCard,sizeof(BYTE)*iFirstCount);
	::CopyMemory(iSecCard,iSecondCard,sizeof(BYTE)*iSecondCount);

	SortCard(iFirCard,NULL,iFirstCount);
	SortCard(iSecCard,NULL,iSecondCount);
	//��ȡ����
	BYTE iFirstCardShape  = GetShape(iFirCard,iFirstCount);
	BYTE iSecondCardShape = GetShape(iSecCard,iSecondCount);
	
	if( (iFirstCardShape==UG_BUST_CARD) && (iSecondCardShape!=UG_BUST_CARD) ) //�мұ��Ƶ�ׯ�Ҳ�����
	{
		return -1;
	}
	else if( (iFirstCardShape!=UG_BUST_CARD) && (iSecondCardShape==UG_BUST_CARD) ) 	//ׯ�ұ��Ƶ��мҲ�����
	{
		return 1;
	}
	else if( (iFirstCardShape==UG_BUST_CARD) && (iSecondCardShape==UG_BUST_CARD) ) 
	{
		return 0;
	}
	//��ȡ�����
	BYTE MaxFir = GetMaxCard(iFirCard, iFirstCount);
	BYTE MaxSec = GetMaxCard(iSecCard, iSecondCount);

	if (iFirstCardShape != iSecondCardShape)    //�Ƚ�����
	{
		return (iFirstCardShape - iSecondCardShape > 0) ? 1 : -1;
	}
	else if(GetPoint(iFirCard,iFirstCount) != GetPoint(iSecCard,iSecondCount))    //�Ƚ��ܵ���
	{
		return (GetPoint(iFirCard,iFirstCount) - GetPoint(iSecCard,iSecondCount)>0) ? 1 : -1;
	}
	else if(GetMaxCard(iFirCard, iFirstCount) != GetMaxCard(iSecCard, iSecondCount))    //�Ƚ������
	{
		return (GetMaxCard(iFirCard, iFirstCount) - GetMaxCard(iSecCard, iSecondCount)>0) ? 1 : -1;
	}
	else
	{
		return (GetCardHuaKind(MaxFir) - GetCardHuaKind(MaxSec) > 0) ? 1 : -1;        //�Ƚ�����ƵĻ�ɫ
	}
	return 0;
}
//�Աȵ���
BOOL CUpGradeGameLogic::CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard)
{
	int iFirstNum = GetCardNum(iFirstCard);//������
	int iNextNum  = GetCardNum(iNextCard);//������

	if (iFirstCard == 0x4F) return FALSE;//����
	if (iNextCard == 0x4F) return TRUE;

	if (iFirstCard == 0x4E) return FALSE;//С��
	if (iNextCard == 0x4E) return TRUE;

	if (iFirstNum == 2) return FALSE;//2
	if (iNextNum == 2) return TRUE;

	return ((iNextNum - iFirstNum) > 0 ? TRUE : FALSE);//����

}

//�Զ����ƺ���
BOOL CUpGradeGameLogic::AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount,
									BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut)
{
	if (bFirstOut == TRUE)
	{
		iResultCard[0] = iHandCard[iHandCardCount-1];
		iResultCardCount = 1;
	}
	return TRUE;
}


//��� 0 λ�˿�
int CUpGradeGameLogic::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0; i<iCardCount; i++)
	{
		if (iCardList[i]!=0) iCardList[i-iRemoveCount]=iCardList[i];
		else iRemoveCount++;
	}
	return iRemoveCount;
}

//�����˿�
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,bool bhaveKing)
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

//ɾ���˿�
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount)
{
	//��������
	if ((iRemoveCount>iCardCount)) return 0;

	//��Ҫɾ����������
	int iDeleteCount=0;
	for (int i=0; i<iRemoveCount; i++)
	{
		for (int j=0; j<iCardCount; j++)
		{
			if (iRemoveCard[i]==iCardList[j])
			{
				iDeleteCount++;
				iCardList[j] = 0;
				break;
			}
		}
	}
	RemoveNummCard(iCardList,iCardCount);
	if (iDeleteCount!=iRemoveCount) return 0;

	return iDeleteCount;
}


BYTE CUpGradeGameLogic::GetShape(BYTE iCardList[],BYTE iCardCount)
{
	if (IsPersonSmallFive(iCardList,iCardCount)) 
	{
		return UG_PERSON_SMALL_FIVE;
	}

	if (IsHeavenKing(iCardList,iCardCount)) 
	{
		return UG_HEAVEN_KING;
	}

	if (IsFiveSmall(iCardList,iCardCount)) 
	{
		return UG_FIVE_SMALL;
	}

	if (IsHalfPastTen(iCardList,iCardCount)) 
	{
		return UG_HALF_PAST_TEN;
	}
	
	if(IsNormalCard(iCardList,iCardCount))
	{
		return UG_NORMAL_CARD;
	}
	if(IsBustCard(iCardList,iCardCount))
	{
		return UG_BUST_CARD;
	}	

}


// �ж��Ƿ���С������
bool CUpGradeGameLogic::IsPersonSmallFive(BYTE  iCardList[],int iCardCount)
{
	if( (GetPoint(iCardList,iCardCount)==25) && (iCardCount==5))
	{
		return true;
	}
	return false;
}

/// �ж��Ƿ���������
bool CUpGradeGameLogic::IsHeavenKing(BYTE  iCardList[],int iCardCount)
{
	if((GetPoint(iCardList,iCardCount)==105) && (iCardCount==5))
	{
		return true;
	}
	return false;
}
/// �ж��Ƿ���С����
bool CUpGradeGameLogic::IsFiveSmall(BYTE  iCardList[],int iCardCount)
{
	if((GetPoint(iCardList,iCardCount)>25) && (GetPoint(iCardList,iCardCount)<105) && (iCardCount==5))
	{
		return true;
	}
	return false;
}
/// �ж��Ƿ�ʮ�������
bool CUpGradeGameLogic::IsHalfPastTen(BYTE  iCardList[],int iCardCount)
{
	if(iCardCount <= 0x00 || iCardCount > 0x05)
		return false;
	if((GetPoint(iCardList,iCardCount)==105))
	{

		return true;
	}
	return false;
}
/// �ж��Ƿ���ͨ������
bool CUpGradeGameLogic::IsNormalCard(BYTE  iCardList[],int iCardCount)
{
	if((GetPoint(iCardList,iCardCount)>0) && (GetPoint(iCardList,iCardCount)<105) && (iCardCount<5))
	{
		return true;
	}
	return false;
}
/// �ж��Ƿ�������
bool CUpGradeGameLogic::IsBustCard(BYTE  iCardList[],int iCardCount)
{
	if(GetPoint(iCardList,iCardCount)>105)
	{
		return true;
	}
	return false;
}


///�õ�������������(����ɫ)
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
int CUpGradeGameLogic::GetMaxCard(BYTE  iCardList[],int iCardCount)
{
	int temp = 0;
	BYTE card = 0;
	for (int i = 0; i < iCardCount; i++)
	{
		if (temp == 0)
		{
			temp = GetCardBulk(iCardList[i]);
			card = iCardList[i];
		}
		else
		{
			if (temp < GetCardBulk(iCardList[i]))
			{
				temp = GetCardBulk(iCardList[i]);
				card =iCardList[i];
			}
			else if (temp == GetCardBulk(iCardList[i]))
			{
				if(GetCardHuaKind(card) < GetCardHuaKind(iCardList[i]))
				{
					temp = GetCardBulk(iCardList[i]);
					card =iCardList[i];
				}
			}
		}

	}
	return card;
}


//����==iCard�ĵ�������iCardList�е����
int  CUpGradeGameLogic::GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard)
{
	for(int i = iStart;i < iCardCount;i ++)
	{
		if(iCardList[i] == iCard)
			return i;
	}
	return -1;
}



int CUpGradeGameLogic::GetCardValue(BYTE byCardValue)
{
	if(byCardValue == 0x00)
	{
		return 0x00;
	}
	else if((byCardValue & 0x0F) == 0x0D)		//A
	{
		return 10;
	}
	else if((byCardValue & 0x0F) >= 0x0A)
	{
		return 0x05;
	}
	else
	{
		return ((byCardValue & 0x0F) + 1)*10;
	}
	return 0xff;
}
int CUpGradeGameLogic::GetPoint(BYTE byCardList[],int iCardNum)
{
	int iTemp = 0;
	for(int i = 0;i < iCardNum;i++)
	{
		iTemp += GetCardValue(byCardList[i]);
	}
	return iTemp;
}