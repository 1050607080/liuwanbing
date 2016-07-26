#include "StdAfx.h"
#include "UpGradeLogic.h"
//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{

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

//=================================================================================
//	���� : ���������еķ�0��������ǰ�ƶ�,�ƶ�֮��0���ԭ����λ��
//	���� : BYTE iCardList[]		[in out]		������
//		   int iCardCount		[in]			�Ƶ�����	
//=================================================================================
int CUpGradeGameLogic::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0;i<iCardCount;i++)
	{
		if (iCardList[i]!=0)
		{
			iCardList[i-iRemoveCount]=iCardList[i];
			if((i-iRemoveCount) != i)
				iCardList[i] = 0x00;
		}
		else 
		{
			iRemoveCount++;
		}
	}
	return iRemoveCount;
}

//==============================================================================================================
// ���� : ϴ�ƺ���
// ���� : BYTE iCard[]		[out]		�������ƽ��
//		  int iCardCount	[in]		���Ƶ�����
//		  int iDeskIndex	[in]		����������
//==============================================================================================================
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,int iDeskIndex)
{
	static const BYTE m_CardArray[32]={
			//2		3     4	    5    6     7     8     9	 10		J	 Q		K  A
			0x01,		0x03,       0x05, 0x06, 0x07,        0x09,		 0x0B, 		//����
						0x13, 0x14, 0x15, 0x16, 0x17, 0x18,  0x19, 0x1A,		 	//÷��
			0x21, 0x22 ,0x23,       0x25, 0x26, 0x27,        0x29,		 0x2B,		//����
				        0x33, 0x34, 0x35, 0x36, 0x37, 0x38,  0x39, 0x3A,		  	//����
			0x4F};																	//���

	BYTE iSend=0,iStation=0,iCardList[32];
	srand(GetTickCount()+iDeskIndex);
	::CopyMemory(iCardList,m_CardArray,sizeof(m_CardArray));

	do
	{
		iStation=rand()%(iCardCount-iSend);
		iCard[iSend]=iCardList[iStation];
		iSend++;
		iCardList[iStation]=iCardList[iCardCount-iSend];
	} while (iSend<iCardCount);

	return iCardCount;
}

//=======================================================================================================
// ���� : ɾ���˿�(��iCardList[] ��ɾ�� iRemoveCard[])
// ���� : BYTE iRemoveCard[]	[in]	Ҫɾ������
//		  int iRemoveCount		[in]	Ҫɾ���Ƶ�����
//		  BYTE iCardList[]		[in]	��������
//		  int iCardCount		[in]	�������ݵ�����
//return  iDeleteCount ����ɾ��������
//=======================================================================================================
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount)
{
	//��������
	if ((iRemoveCount>iCardCount)) return 0;

	//��Ҫɾ����������
	int iDeleteCount=0;
	for (int i=0;i<iRemoveCount;i++)
	{
		for (int j=0;j<iCardCount;j++)
		{
			if (iRemoveCard[i]==iCardList[j])
			{
				iDeleteCount++;
				iCardList[j]=0;
				break;
			}
		}
	}
	RemoveNummCard(iCardList,iCardCount);
	if (iDeleteCount!=iRemoveCount) return 0;

	return iDeleteCount;
}

//===================================================================================
// ���� : ����Ƿ��Ƕ���(��3�������)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ƕ��� false : ���Ƕ���
//===================================================================================
bool CUpGradeGameLogic::IsRedThreeAndBigKing(BYTE byCardList[], int iCardCount)
{						
	if(0x02 != iCardCount)
		return false;
	if((0x4F == byCardList[0]) && (0x22 == byCardList[1]))
		return true;
	if((0x22 == byCardList[0]) && (0x4F == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ�������(һ�Ժ�Q)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ������ false : ��������
//===================================================================================
bool CUpGradeGameLogic::IsDoubleRedQ(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x0B == byCardList[0]) && (0x2B == byCardList[1]))
		return true;
	if((0x2B == byCardList[0]) && (0x0B == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��ǵ���(һ�Ժ�2)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �ǵ��� false : ���ǵ���
//===================================================================================
bool CUpGradeGameLogic::IsDoubleRedTwo(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x01 == byCardList[0]) && (0x21 == byCardList[1]))
		return true;
	if((0x21 == byCardList[0]) && (0x01 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ�������(һ�Ժ�8)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ������ false : ��������
//===================================================================================
bool CUpGradeGameLogic::IsDoubleRedEight(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x07 == byCardList[0]) && (0x27 == byCardList[1]))
		return true;
	if((0x27 == byCardList[0]) && (0x07 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��Ǻ���(һ�Ժ�4)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ǻ��� false : ���Ǻ���
//===================================================================================
bool CUpGradeGameLogic::IsDoubleRedFour(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x03 == byCardList[0]) && (0x23 == byCardList[1]))
		return true;
	if((0x23 == byCardList[0]) && (0x03 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ���÷ʮ(һ�Ժ�10)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ��÷ʮ false : ����÷ʮ
//===================================================================================
bool CUpGradeGameLogic::IsDoubleBlackTen(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x19 == byCardList[0]) && (0x39 == byCardList[1]))
		return true;
	if((0x39 == byCardList[0]) && (0x19 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��ǰ��(һ�Ժ�4)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �ǰ�� false : ���ǰ��
//===================================================================================
bool CUpGradeGameLogic::IsDoubleBlackFour(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x13 == byCardList[0]) && (0x33 == byCardList[1]))
		return true;
	if((0x33 == byCardList[0]) && (0x13 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��ǳ���(һ�Ժ�6)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �ǳ��� false : ���ǳ���
//===================================================================================
bool CUpGradeGameLogic::IsDoubleBlackSix(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x15 == byCardList[0]) && (0x35 == byCardList[1]))
		return true;
	if((0x35 == byCardList[0]) && (0x15 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��ǻ�ͷ(һ�Ժ�J)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �ǻ�ͷ false : ���ǻ�ͷ
//===================================================================================
bool CUpGradeGameLogic::IsDoubleBlackJ(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x1A == byCardList[0]) && (0x3A == byCardList[1]))
		return true;
	if((0x3A == byCardList[0]) && (0x1A == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ�����ʮ(һ�Ժ�10)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ����ʮ false : ������ʮ
//===================================================================================
bool CUpGradeGameLogic::IsDoubleRedTen(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x09 == byCardList[0]) && (0x29 == byCardList[1]))
		return true;
	if((0x29 == byCardList[0]) && (0x09 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ���èè(һ�Ժ���)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ��èè false : ����èè
//===================================================================================
bool CUpGradeGameLogic::IsDoubleRedSix(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x05 == byCardList[0]) && (0x25 == byCardList[1]))
		return true;
	if((0x25 == byCardList[0]) && (0x05 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��Ǹ�ҩ(һ�Ժ���)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ǹ�ҩ false : ���Ǹ�ҩ
//===================================================================================
bool CUpGradeGameLogic::IsDoubleRedSeven(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x06 == byCardList[0]) && (0x26 == byCardList[1]))
		return true;
	if((0x26 == byCardList[0]) && (0x06 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��Ƕ���(һ�Ժھ�)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ƕ��� false : ���Ƕ���
//===================================================================================
bool CUpGradeGameLogic::IsDoubleBlackNine(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x18 == byCardList[0]) && (0x38 == byCardList[1]))
		return true;
	if((0x38 == byCardList[0]) && (0x18 == byCardList[1]))
		return true;
	return false;
}
//===================================================================================
// ���� : ����Ƿ��Ƕ���(һ�Ժڰ�)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ƕ��� false : ���Ƕ���
//===================================================================================
bool CUpGradeGameLogic::IsDoubleBlackEight(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x17 == byCardList[0]) && (0x37 == byCardList[1]))
		return true;
	if((0x37 == byCardList[0]) && (0x17 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��Ƕ���(һ�Ժ���)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ƕ��� false : ���Ƕ���
//===================================================================================
bool CUpGradeGameLogic::IsDoubleBlackSeven(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x16 == byCardList[0]) && (0x36 == byCardList[1]))
		return true;
	if((0x36 == byCardList[0]) && (0x16 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��Ƕ���(һ�Ժ���)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ƕ��� false : ���Ƕ���
//===================================================================================
bool CUpGradeGameLogic::IsDoubleBlackFive(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x14 == byCardList[0]) && (0x34 == byCardList[1]))
		return true;
	if((0x34 == byCardList[0]) && (0x14 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ����̹�(��Q��9 ��Q��9)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ���̹� false : �����̹�
//===================================================================================
bool CUpGradeGameLogic::IsRedQAdnNine(BYTE byCardList[], int iCardCount)						//����Ƿ����̹�
{
	if(0x02 != iCardCount)
		return false;
	if((0x2B == byCardList[0]) && (0x08 == (byCardList[1]&0x0f)))
		return true;
	if((0x08 == (byCardList[0]&0x0f)) && (0x2B == byCardList[1]))
		return true;
	if((0x0B == byCardList[0]) && (0x08 == (byCardList[1]&0x0f)))
		return true;
	if((0x08 == (byCardList[0]&0x0f)) && (0x0B == byCardList[1]))
		return true;

	return false;
}

//===================================================================================
// ���� : ����Ƿ������(��Q��8)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ����� false : �������
//===================================================================================
bool CUpGradeGameLogic::IsRedQAdnEight(BYTE byCardList[], int iCardCount)						//����Ƿ������
{
	if(0x02 != iCardCount)
		return false;
	if((0x2B == byCardList[0]) && (0x07 == (byCardList[1]&0x0f)))
		return true;
	if((0x07 == (byCardList[0]&0x0f)) && (0x2B == byCardList[1]))
		return true;
	if((0x0B == byCardList[0]) && (0x07 == (byCardList[1]&0x0f)))
		return true;
	if((0x07 == (byCardList[0]&0x0f)) && (0x0B == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��ǵظ�(��2��8)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �ǵظ� false : ���ǵظ�
//===================================================================================
bool CUpGradeGameLogic::IsRedTwoAndEight(BYTE byCardList[], int iCardCount)					//����Ƿ��ǵظ�
{
	if(0x02 != iCardCount)
		return false;
	if((0x21 == byCardList[0]) && (0x07 == (byCardList[1]&0x0f)))
		return true;
	if((0x07 == (byCardList[0]&0x0f)) && (0x21 == byCardList[1]))
		return true;
	if((0x01 == byCardList[0]) && (0x07 == (byCardList[1]&0x0f)))
		return true;
	if((0x07 == (byCardList[0]&0x0f)) && (0x01 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ�����ؾ�(��Q��7)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ����ؾ� false : ������ؾ�
//===================================================================================
bool CUpGradeGameLogic::IsRedQAndSeven(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x2B == byCardList[0]) && (0x06 == (byCardList[1]&0x0f)))
		return true;
	if((0x06 == (byCardList[0]&0x0f)) && (0x2B == byCardList[1]))
		return true;
	if((0x0B == byCardList[0]) && (0x06 == (byCardList[1]&0x0f)))
		return true;
	if((0x06 == (byCardList[0]&0x0f)) && (0x0B == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��ǵعؾ�(��2��7)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �ǵعؾ� false : ���ǵعؾ�
//===================================================================================
bool CUpGradeGameLogic::IsRedTwoAndSeven(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x21 == byCardList[0]) && (0x06 == (byCardList[1]&0x0f)))
		return true;
	if((0x06 == (byCardList[0]&0x0f)) && (0x21 == byCardList[1]))
		return true;
	if((0x01 == byCardList[0]) && (0x06 == (byCardList[1]&0x0f)))
		return true;
	if((0x06 == (byCardList[0]&0x0f)) && (0x01 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��ǵ�����(��8��J)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �ǵ����� false : ���ǵ�����
//===================================================================================
bool CUpGradeGameLogic::IsRedEightAndJ(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x27 == byCardList[0]) && (0x0A == (byCardList[1]&0x0f)))
		return true;
	if((0x0A == (byCardList[0]&0x0f)) && (0x27 == byCardList[1]))
		return true;
	if((0x07 == byCardList[0]) && (0x0A == (byCardList[1]&0x0f)))
		return true;
	if((0x0A == (byCardList[0]&0x0f)) && (0x07 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��Ǻ����(��4��5)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ǻ���� false : ���Ǻ����
//===================================================================================
bool CUpGradeGameLogic::IsRedFourAndFive(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x23 == byCardList[0]) && (0x04 == (byCardList[1]&0x0f)))
		return true;
	if((0x04 == (byCardList[0]&0x0f)) && (0x23 == byCardList[1]))
		return true;
	if((0x03 == byCardList[0]) && (0x04 == (byCardList[1]&0x0f)))
		return true;
	if((0x04 == (byCardList[0]&0x0f)) && (0x03 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��ǰ����(��4��5)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �ǰ���� false : ���ǰ����
//===================================================================================
bool CUpGradeGameLogic::IsBlackFourAndFive(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x33 == byCardList[0]) && (0x04 == (byCardList[1]&0x0f)))
		return true;
	if((0x04 == (byCardList[0]&0x0f)) && (0x33 == byCardList[1]))
		return true;

	if((0x13 == byCardList[0]) && (0x04 == (byCardList[1]&0x0f)))
		return true;
	if((0x04 == (byCardList[0]&0x0f)) && (0x13 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��Ƕ�����(��3��6)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ƕ����� false : ���Ƕ�����
//===================================================================================
bool CUpGradeGameLogic::IsRedThreeAndBlackSix(BYTE byCardList[], int iCardCount)				
{
	if(0x02 != iCardCount)
		return false;
	if((0x22 == byCardList[0]) && (0x15 == (byCardList[1])&0x0f))
		return true;
	if((0x15 == (byCardList[0]&0x0f)) && (0x22 == byCardList[1]))
		return true;

	if((0x22 == byCardList[0]) && (0x35 == (byCardList[1])&0x0f))
		return true;
	if((0x35 == (byCardList[0]&0x0f)) && (0x22 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ���÷ʮ��(��10��9)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ��÷ʮ�� false : ����÷ʮ��
//===================================================================================
bool CUpGradeGameLogic::IsBlackTenAndNine(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x39 == byCardList[0]) && (0x08 == (byCardList[1]&0x0f)))
		return true;
	if((0x08 == (byCardList[0]&0x0f)) && (0x39 == byCardList[1]))
		return true;
	if((0x19 == byCardList[0]) && (0x08 == (byCardList[1]&0x0f)))
		return true;
	if((0x08 == (byCardList[0]&0x0f)) && (0x19 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ��Ƕ�è��(��3���6)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �Ƕ�è�� false : ���Ƕ�è��
//===================================================================================
bool CUpGradeGameLogic::IsRedThreeAndRedSix(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x22 == byCardList[0]) && (0x25 == byCardList[1]))
		return true;
	if((0x25 == byCardList[0]) && (0x22 == byCardList[1]))
		return true;
	if((0x22 == byCardList[0]) && (0x05 == byCardList[1]))
		return true;
	if((0x05 == byCardList[0]) && (0x22 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ���������(��8��J)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �������� false : ����������
//===================================================================================
bool CUpGradeGameLogic::IsBlackEightAndJ(BYTE byCardList[], int iCardCount)					
{
	if(0x02 != iCardCount)
		return false;
	if((0x37 == byCardList[0]) && (0x0A == (byCardList[1]&0x0f)))
		return true;
	if((0x0A == (byCardList[0]&0x0f)) && (0x37 == byCardList[1]))
		return true;
	if((0x17 == byCardList[0]) && (0x0A == (byCardList[1]&0x0f)))
		return true;
	if((0x0A == (byCardList[0]&0x0f)) && (0x17 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ�����ʮ��(��10��9)
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ����ʮ�� false : ������ʮ��
//===================================================================================
bool CUpGradeGameLogic::IsRedTenAndNine(BYTE byCardList[], int iCardCount)						
{
	if(0x02 != iCardCount)
		return false;
	if((0x29 == byCardList[0]) && (0x08 == (byCardList[1]&0x0f)))
		return true;
	if((0x08 == (byCardList[0]&0x0f)) && (0x29 == byCardList[1]))
		return true;
	if((0x09 == byCardList[0]) && (0x08 == (byCardList[1]&0x0f)))
		return true;
	if((0x08 == (byCardList[0]&0x0f)) && (0x09 == byCardList[1]))
		return true;
	return false;
}

//===================================================================================
// ���� : ����Ƿ������
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ����� false : �������
//===================================================================================
bool CUpGradeGameLogic::IsThreeAndBigKingAndDoubleRedQ(BYTE byCardList[], int iCardCount)		
{
	if(0x04 != iCardCount)
		return false;
	const BYTE byCardValue[] = {0x22,0x4F,0x0B,0x2B};
	bool bFlage = true;
	for(int i = 0x00;i < 0x04;i ++)
	{
		 bFlage = true;
		for(int j = 0x00;j < 0x04;j ++)
		{
			if(byCardList[i] == byCardValue[j])
			{
				bFlage = false;
				break;
			}
		}
		if(bFlage)
		{
			return false;
		}
	}
	return true;
}

//===================================================================================
// ���� : ����Ƿ���Ѽ��
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ��Ѽ�� false : ����Ѽ��
//===================================================================================
bool CUpGradeGameLogic::IsThreeAndBigKingAndDoubleRedTwo(BYTE byCardList[], int iCardCount)		
{
	if(0x04 != iCardCount)
		return false;
	const BYTE byCardValue[] = {0x22,0x4F,0x01,0x21};
	bool bFlage = true;
	for(int i = 0x00;i < 0x04;i ++)
	{
		bFlage = true;
		for(int j = 0x00;j < 0x04;j ++)
		{
			if(byCardList[i] == byCardValue[j])
			{
				bFlage = false;
				break;
			}
		}
		if(bFlage)
		{
			return false;
		}
	}
	return true;
}

//===================================================================================
// ���� : ����Ƿ�������ʮ	��ʮ + ��ʮ + J
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : ������ʮ false : ��������ʮ
//===================================================================================
bool CUpGradeGameLogic::IsRedTenAndBlackTenAndJ(BYTE byCardList[], int iCardCount)			
{
	if(0x03 > iCardCount || iCardCount > 0x04)
	{
		return false;
	}
		
	BYTE byCardValue = 0x00;
	BYTE byCardValueSum = 0x00;
	for(int i = 0x00;i < iCardCount;i ++)
	{
		byCardValue = (byCardList[i]&0x0F);
		if(byCardValue == 0x09)
		{
			byCardValueSum += byCardList[i];
		}
		else if(byCardValue == 0x0A)
		{
			byCardValueSum += byCardValue;
		}
	}
	if(byCardValueSum == 0x2C)	//��10 + ÷10 + J = 0x09 + 0x19 + 0x0A = 0x2C
	{							
		return true;			
	}					
	if(byCardValueSum == 0x4C)	//��10 + ��10 + J = 0x09 + 0x39 + 0x0A = 0x4C �� ��10 + ÷10 + J = 0x29 + 0x19 + 0x0A = 0x4C
	{
		return true;
	}
	if(byCardValueSum == 0x6C)	//��10 + ��10 + J = 0x29 + 0x39 + 0x0A = 0x6C
	{
		return true;
	}
	return false;
}

//===================================================================================
// ���� : ����Ƿ��������� ��6,��6,����
// ���� : BYTE iCardList[]		[in]		�Ƶ�����
//		  int iCardCount		[in]		�Ƶ�����
// return true : �������� false : ����������
//===================================================================================
bool CUpGradeGameLogic::IsRedSixAndBlackSixAndBigKing(BYTE byCardList[], int iCardCount)			
{
	if(0x03 >iCardCount || iCardCount > 0x04)
	{
		return false;
	}
		
	BYTE byCardValue = 0x00;
	BYTE byCardValueSum = 0x00;
	for(int i = 0x00;i < iCardCount;i++)
	{
		byCardValue = (byCardList[i]&0x0F);
		if(byCardValue == 0x05 || byCardValue == 0x0F)		//��������������OK
		{
			byCardValueSum += byCardList[i];
		}
	}
	if(byCardValueSum == 0x89)		//��6 + ÷6 + ���� = 0x25 + 0x15 + 0x4F = 0x89 �� ��6 + ��6 + ���� = 0x05 + 0x35 + 0x4F = 0x89
	{
		return true;
	}

	if(byCardValueSum == 0xA9)		//��6 + ��6 + ���� = 0x25 + 0x35 + 0x4F = 0xA9
	{
		return true;
	}
	if(byCardValueSum == 0x69)		//��6 + ÷6 + ���� = 0x05 + 0x15 + 0x4F = 0x69
	{
		return true;
	}
	return false;
}

//========================================================================================================
//���� : ��ȡһ���Ƶ�ֵ
//���� : BYTE byCard		[in]		��������ֵ
//return ת�����Ƶĵ�
//========================================================================================================
int  CUpGradeGameLogic::GetCardValue(BYTE byCard)												//��ȡ��ֵ
{
	BYTE byTemp = (byCard&0x0F);
	if(0x0F == byTemp)	return 0x06;				//����
	if(0x0B == byTemp)	return 0x02;				//Q
	if(0x0A == byTemp)	return 0x01;				//J
	return (byTemp + 1);
}

//================================================================================================================
// ���� : ��ȡ����
// ���� : BYTE byCardList[]			[in]		������
//		  int iCardNum				[in]		������
//return  �Ƶ�����
//================================================================================================================
int CUpGradeGameLogic::GetCardShape(BYTE byCardList[],int iCardNum)
{					
	if(IsRedThreeAndBigKing(byCardList,iCardNum))	return ZX_DING_HUANG;		//����ǲ��Ƕ���
	if(IsDoubleRedQ(byCardList,iCardNum))			return ZX_TIAN_PAI;			//����Ƿ�������
	if(IsDoubleRedTwo(byCardList,iCardNum))			return ZX_DI_PAI;			//����Ƿ��ǵ���
	if(IsDoubleRedEight(byCardList,iCardNum))		return ZX_REN_PAI;			//����Ƿ�������
	if(IsDoubleRedFour(byCardList,iCardNum))		return ZX_HE_PAI;			//����Ƿ��Ǻ���
	if(IsDoubleBlackTen(byCardList,iCardNum))		return ZX_MEI_SHI;			//����Ƿ���÷ʮ
	if(IsDoubleBlackFour(byCardList,iCardNum))		return ZX_BAN_DENG;			//����Ƿ��ǰ��
	if(IsDoubleBlackSix(byCardList,iCardNum))		return ZX_CHANG_SANG;		//����Ƿ��ǳ���
	if(IsDoubleBlackJ(byCardList,iCardNum))			return ZX_HU_TOU;			//����Ƿ��ǻ�ͷ
	if(IsDoubleRedTen(byCardList,iCardNum))			return ZX_TIAO_SHI;			//����Ƿ�����ʮ
	if(IsDoubleRedSix(byCardList,iCardNum))			return ZX_MAO_MAO;			//����Ƿ���èè
	if(IsDoubleRedSeven(byCardList,iCardNum))		return ZX_KAO_YAO;			//����Ƿ��Ǹ�ҩ
	if(IsDoubleBlackNine(byCardList,iCardNum))		return ZX_DUI_HEI_JIU;		//����Ƿ��Ƕ���(�Ժڣ�)
	if(IsDoubleBlackEight(byCardList,iCardNum))		return ZX_DUI_HEI_BA;		//����Ƿ��Ƕ���(��8)
	if(IsDoubleBlackSeven(byCardList,iCardNum))		return ZX_DUI_HEI_QI;		//����Ƿ��Ƕ���(��7)
	if(IsDoubleBlackFive(byCardList,iCardNum))		return ZX_DUI_HEI_WU;		//����Ƿ��Ƕ���(��5)
	if(IsRedQAdnNine(byCardList,iCardNum))			return ZX_NAI_GOU;			//����Ƿ����̹�
	if(IsRedQAdnEight(byCardList,iCardNum))			return ZX_TIAN_GANG;		//����Ƿ������
	if(IsRedTwoAndEight(byCardList,iCardNum))		return ZX_DI_GANG;			//����Ƿ��ǵظ�
	if(IsRedQAndSeven(byCardList,iCardNum))			return ZX_TIAN_GUAN_JIU;	//����Ƿ�����ؾ�
	if(IsRedTwoAndSeven(byCardList,iCardNum))		return ZX_DI_GUAN_JIU;		//����Ƿ��ǵعؾ�
	if(IsRedEightAndJ(byCardList,iCardNum))			return ZX_DENG_LONG_JIU;	//����Ƿ��ǵ�����
	if(IsRedFourAndFive(byCardList,iCardNum))		return ZX_HE_WU_JIU;		//����Ƿ��Ǻ����
	if(IsBlackFourAndFive(byCardList,iCardNum))		return ZX_BAN_WU_JIU;		//����Ƿ��ǰ����
	if(IsRedThreeAndBlackSix(byCardList,iCardNum))	return ZX_DING_CHANG_JIU;	//����Ƿ��Ƕ�����
	if(IsBlackTenAndNine(byCardList,iCardNum))		return ZX_MEI_SHI_JIU;		//����Ƿ���÷ʮ��
	if(IsRedThreeAndRedSix(byCardList,iCardNum))	return ZX_DING_MAO_JIU;		//����Ƿ��Ƕ�è��
	if(IsBlackEightAndJ(byCardList,iCardNum))		return ZX_WU_LONG_JIU;		//����Ƿ���������
	if(IsRedTenAndNine(byCardList,iCardNum))		return ZX_TIAO_SHI_JIU;		//����Ƿ�����ʮ��
	return CalculationCardPoint(byCardList,iCardNum);							//���ؼ�����
}

//==================================================================================================
// ���� : ����Ƿ�������ʮ������
// ���� : BYTE byCardList[]			[in]		������
//		  int iCardNum				[in]		������
//==================================================================================================
BYTE CUpGradeGameLogic::IsThreeTenOrThreThreeSixe(BYTE byCardList[], int iCardCount)			
{
	if(IsRedTenAndBlackTenAndJ(byCardList, iCardCount))		return 	ZX_SHAN_HUA_SHI;	//���Ƿ�������ʮ
	if(IsRedSixAndBlackSixAndBigKing(byCardList, iCardCount)) return ZX_SHAN_HUA_LIU;	//���Ƿ���������
	return ZX_ERROR;
}
//==================================================================================================
// ���� : ���������Ƶĵ���
// ���� : BYTE byCardList[]		[in]	������
//		  int iCardNum			[in]	������
//	return �����Ƶĵ���			���� -1 Ϊ����ֵ
//==================================================================================================
int CUpGradeGameLogic::CalculationCardPoint(BYTE byCardList[],int iCardNum)
{
	if(iCardNum != 0x02)
	{
		return -1;
	}
	int iFirstCardValue = GetCardValue(byCardList[0]);
	int iSecondCardValue = GetCardValue(byCardList[1]);
	return (iFirstCardValue + iSecondCardValue)%10;
}

//==================================================================================================
// ���� : �Ƚ������ƵĴ�С
// ���� : BYTE byFirstCardList[]		[in]			��һ����
//		  int iFirstCardCount			[in]			������
//		  BYTE bySecondCardList[]		[in]			��һ����
//		  int iSecondCardCount			[in]			������
// return ����
//#define RESULT_BIG					1				//�ȽϽ����
//#define RESULT_EQU					0				//�ȽϽ�����
//#define RESULT_SMA					-1				//�ȽϽ�����
//==================================================================================================
int CUpGradeGameLogic::CompareTwoCardHand(BYTE byFirstCardList[],int iFirstCardCount,BYTE bySecondCardList[],int iSecondCardCount)
{
	const int iShapValue[] ={0,1,2,3,3,3,3,4,4,4,4,5,5,5,6,7,8,9,10};
	int iFirstCardShap = 0x00;
	int iSecondCardShap = 0x00;
	int iTemp = 0x00;
	iFirstCardShap = GetCardShape(byFirstCardList,iFirstCardCount);
	iSecondCardShap = GetCardShape(bySecondCardList,iSecondCardCount);

	if((iFirstCardShap >= ZX_DI_GANG)&&(iSecondCardShap < ZX_DI_GANG))				//��һ��������,�ڶ���������
	{
		return RESULT_BIG;
	}
	else if((iFirstCardShap < ZX_DI_GANG)&&(iSecondCardShap >= ZX_DI_GANG))			//��һ���Ƿ�����,�ڶ�������
	{
		return RESULT_SMA;
	}
	else if((iFirstCardShap >= ZX_DI_GANG)&&(iSecondCardShap >= ZX_DI_GANG))			//������������
	{
		iTemp = iShapValue[iFirstCardShap - ZX_DI_GANG] - iShapValue[iSecondCardShap];
		if(iTemp > 0x00)
		{
			return RESULT_BIG;
		}
		else if(iTemp < 0x00)
		{
			return RESULT_SMA;
		}
		else
		{
			int iFirstCardValue = GetMaxCardValue(byFirstCardList,iFirstCardCount);
			int iSecondCardValue = GetMaxCardValue(bySecondCardList,iSecondCardCount);
			if(iFirstCardValue > iSecondCardValue)
			{
				return RESULT_BIG;
			}
			else if(iFirstCardValue < iSecondCardValue)
			{
				return RESULT_SMA;
			}
			else
			{
				return RESULT_EQU;
			}
		}
	}									//��������������
	else
	{
		int iFirstCardPoint = CalculationCardPoint(byFirstCardList,iFirstCardCount);
		int iSecondCardPoint = CalculationCardPoint(bySecondCardList,iSecondCardCount);
		if(iFirstCardPoint > iSecondCardPoint)
		{
			return RESULT_BIG; 
		}
		else if(iFirstCardPoint < iSecondCardPoint)
		{
			return RESULT_SMA;
		}
		else
		{
			int iFirstCardValue = GetMaxCardValue(byFirstCardList,iFirstCardCount);
			int iSecondCardValue = GetMaxCardValue(bySecondCardList,iSecondCardCount);
			if(iFirstCardValue > iSecondCardValue)
			{
				return RESULT_BIG;
			}
			else if(iFirstCardValue < iSecondCardValue)
			{
				return RESULT_SMA;
			}
			else
			{
				return RESULT_EQU;
			}
		}
	}
}

//==================================================================================================
// ���� : ����������ȡ�����ֵ��������
// ���� : BYTE byCardList[]			[in]	��ֵ�б�
//        int iCardCount			[in]	�б��Ƶ�����
//return  ����ת�������ֵ
//==================================================================================================
BYTE CUpGradeGameLogic::GetMaxCardValue(BYTE byCardList[],int iCardCount)
{
	if(iCardCount != 0x02)
		return 0x00;
	int iFirstCardValue = 0x00;
	int iSecondCardValue = 0x00;
	iFirstCardValue = TransformCardValue(byCardList[0]);
	iSecondCardValue = TransformCardValue(byCardList[1]);
	if(iFirstCardValue >= iSecondCardValue)
	{
		return iFirstCardValue;
	}
	return iSecondCardValue;
}
//==================================================================================================
// ���� : ת��һ���Ƴ���Ӧ�Ĵ�С,��ֵ���ڱȽϴ�С
// ���� : BYTE byCardList		[in]		Ҫת������ֵ
// return ����Ϊ��Ӧ�ƵĴ�С
//==================================================================================================
BYTE CUpGradeGameLogic::TransformCardValue(BYTE byCardList)
{										  // 2     3    4    5    6    7    8    9   10    J    Q    K    A
	const BYTE byCardTable[4][14] ={{0x00,0x05,0x02,0x03,0x04,0x01,0x01,0x04,0x08,0x01,0x0A,0x06,0x0C,0x0D},			//��
									{0x00,0x05,0x12,0x03,0x14,0x01,0x01,0x04,0x18,0x01,0x1A,0x06,0x1C,0x1D},			//��
									{0x00,0x21,0x00,0x02,0x00,0x02,0x00,0x00,0x00,0x02,0x01,0x2B,0x2C,0x2D},			//÷
									{0x00,0x31,0x32,0x02,0x00,0x02,0x00,0x00,0x00,0x02,0x01,0x3B,0x3C,0x3D}};			//��
	if(byCardList == 0x4F)		//�Ǵ���
	   return 0x00;
	int iColumn = (byCardList&0x0f);			//��ֵ
	int iRow = (byCardList&0xf0)>>4;			//��ֵ
	return byCardTable[iRow][iColumn];
}		

//============================================================================================================
// ���� : �Ƚ��������ƵĴ�С
// ���� : BYTE iFirstCard[]		[in]		��һ��������
//		  int iFirstCount		[in]		��һ��������
//		  BYTE iSecondCard[]	[in]		�ڶ���������
//		  int iSecondCount		[in]		�ڶ���������
//============================================================================================================
int CUpGradeGameLogic::CompareCard(BYTE iFirstCard[], int iFirstCount, BYTE iSecondCard[], int iSecondCount)
{
	return -1;
}


//=================================================================================================
// ���� : ����������ϳ�һ������
// ���� : BYTE iFirstCard[]		[in]		������
//		  int iFirstCount		[in]		������
//return  ������0xaabbcc;		aa:����		bb:�Ƶ��� cc:�����ֵ
//=================================================================================================
int CUpGradeGameLogic::GetCmpCardValue(BYTE byCardList[],int iCardCount)
{
	const int iShapValue[] ={0,1,2,3,3,3,3,4,4,4,4,5,5,5,6,7,8,9,10};
	int iSaveCardGoundInfo = 0x00;
	int iCardShap = 0x00;									//����
	int iCardPoint = 0x00;									//�Ƶĵ�
	int iMaxCard = 0x00;									//������
	iCardShap = GetCardShape(byCardList,iCardCount);
	if(iCardShap >= ZX_DI_GANG)			
	{
		iCardShap = iCardShap - ZX_DI_GANG;
		iCardShap = iShapValue[iCardShap] + 1;
		iSaveCardGoundInfo = (iCardShap << 0x10);			//����ʮ��λ
		return iSaveCardGoundInfo;
	}
	iCardPoint = CalculationCardPoint(byCardList,iCardCount);
	iCardPoint = (iCardPoint << 0x08);
	iMaxCard = GetMaxCardValue(byCardList,iCardCount);
	iSaveCardGoundInfo = (iCardPoint | iMaxCard);
	return iSaveCardGoundInfo;
}