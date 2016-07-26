#include "StdAfx.h"
#include "UpGradeLogic.h"

//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	m_bTwoIsNT=false;
	m_iNTNum=0;
	m_iNTHuaKind=UG_ERROR_HUA;
	m_iStation[4]=500;
	for (int i=0;i<4;i++) m_iStation[i]=100*i;
	
}

//�������ƻ�ɫ
void CUpGradeGameLogic::SetNTHuaKind(BYTE iNTHuaKind)
{
	//��������
	m_iNTHuaKind=iNTHuaKind & UG_HUA_MASK;
	
	//�������λ��
	for (int i=0;i<4;i++) m_iStation[i]=100*i;
	if (m_iNTHuaKind!=UG_ERROR_HUA) m_iStation[m_iNTHuaKind>>4]=400;
	if (m_iNTHuaKind==UG_MEI_HUA)
	{
		m_iStation[3]=200;
		m_iStation[2]=300;
	}
	else if (m_iNTHuaKind==UG_HONG_TAO)
	{
		m_iStation[0]=100;
		m_iStation[1]=0;
	}

	return;
}

//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard, BOOL bTrueHua)
{ 
	int iHuaKind=(iCard&UG_HUA_MASK);
	if (!bTrueHua)
	{
		if ((iHuaKind==m_iNTHuaKind)||(GetCardNum(iCard)==m_iNTNum)||(GetCardNum(iCard)==2)&&GetTwoIsNT()) 
			return iHuaKind=UG_NT_CARD;
	}
	return iHuaKind; 
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard, BOOL bExtVol)
{
	int setp=0;
	if(GetTwoIsNT()&&!bExtVol)					//2Ϊ������2����λ��
			setp=2;
	if ((iCard==0x4E)||(iCard==0x4F))
	{
		return bExtVol?(iCard-53):(iCard-61+setp);	//�Ƚϴ�С(��èΪ79-59=20,Сè78-59=19)�������С��è26,Сè25
	}

	int iCardNum=GetCardNum(iCard);

	if (iCardNum==m_iNTNum)			//����
	{
		int iHuaKind=GetCardHuaKind(iCard,TRUE);
		if (bExtVol)				//��������
		{
			if(iHuaKind==m_iNTHuaKind)			//��7Ϊ24
				return 24;							
			else
				return (iHuaKind>>4)+20;			//��7(�ں�÷��23,22,21,20)
		}	
		return (iHuaKind==m_iNTHuaKind)?(16+setp):(15+setp);
	}	
	if(iCardNum==2&&GetTwoIsNT())
	{
		int iHuaKind=GetCardHuaKind(iCard,TRUE);
		if (bExtVol)
		{
			if(iHuaKind==m_iNTHuaKind)	//��2Ϊ19
				return 19;
			else
				return (iHuaKind>>4)+15;//��2(�ں�÷��18,17,16,15)
		}
		return (iHuaKind==m_iNTHuaKind)?16:15;
	}
	return iCardNum;
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
		if (iStationVol[i]>=15) iStationVol[i]+=m_iStation[4];
		else iStationVol[i]+=m_iStation[GetCardHuaKind(iCardList[i],FALSE)>>4];
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

//��ȡ�˿�
BYTE CUpGradeGameLogic::GetCardFromHua(int iHuaKind, int iNum)
{
	if (iHuaKind!=UG_ERROR_HUA) return (iHuaKind+iNum-1);
	return 0;
}

//�Ƿ����
BOOL CUpGradeGameLogic::IsDouble(BYTE iCardList[], int iCardCount)
{
	if (iCardCount!=2) 
		return FALSE;
	return (iCardList[0]==iCardList[1]);
}

//�Ƿ�����
BOOL CUpGradeGameLogic::IsThree(BYTE iCardList[], int iCardCount)
{
	if (iCardCount!=3) return FALSE;
	return ((iCardList[0]==iCardList[1])&&(iCardList[0]==iCardList[2]));
}

//�Ƿ��������߼�
BOOL CUpGradeGameLogic::IsTrackorRule(BYTE iFirstCard, BYTE iNextCard)
{
	int iCardBulk=GetCardBulk(iFirstCard,FALSE);
	//�����ռ�˵��е�λ��
	if(GetNTCardNum()!=0 && (iCardBulk==GetNTCardNum()-1 || iCardBulk==GetNTCardNum()+1) && 
		(GetCardBulk(iNextCard,FALSE)==GetNTCardNum()-1 || GetCardBulk(iNextCard,FALSE)==GetNTCardNum()+1))
	{
		return true;
	}
	if (GetCardBulk(iNextCard,FALSE)!=(iCardBulk-1)) 
		return FALSE;
	return TRUE;
}

//�Ƿ���������������
BOOL CUpGradeGameLogic::IsDoubleTrackor(BYTE iCardList[], int iCardCount)
{
	if ((iCardCount<4)||((iCardCount%2)!=0)) return FALSE;

	BYTE iCard=iCardList[0];
	if (!IsDouble(iCardList,2)) return FALSE;
	for (int i=2;i<iCardCount;i+=2)
	{
		if (!IsDouble(&iCardList[i],2)||!IsTrackorRule(iCard,iCardList[i])) return FALSE;
		iCard=iCardList[i];
	}
	return TRUE;
}

//�Ƿ���������������
BOOL CUpGradeGameLogic::IsThreeTrackor(BYTE iCardList[], int iCardCount)
{
	if ((iCardCount<6)||((iCardCount%3)!=0)) return FALSE;

	BYTE iCard=iCardList[0];
	if (!IsThree(iCardList,3)) return FALSE;
	for (int i=3;i<iCardCount;i+=3)
	{
		if (!IsThree(&iCardList[i],3)||!IsTrackorRule(iCard,iCardList[i])) return FALSE;
		iCard=iCardList[i];
	}
	return TRUE;
}

//�Ƿ�˦��
BOOL CUpGradeGameLogic::IsShowCard(BYTE iCardList[], int iCardCount)
{
	return (GetCardListHua(iCardList,iCardCount)!=UG_ERROR_HUA);
}

//�õ����л�ɫ
BYTE CUpGradeGameLogic::GetCardListHua(BYTE iCardList[], int iCardCount)
{
	int iHuaKind=GetCardHuaKind(iCardList[0],FALSE);
	if (GetCardHuaKind(iCardList[iCardCount-1],FALSE)!=iHuaKind) 
		return UG_ERROR_HUA;
	return iHuaKind;
}

//��ȡ����
BYTE CUpGradeGameLogic::GetCardShape(BYTE iCardList[], int iCardCount)
{
	if (GetCardListHua(iCardList,iCardCount)!=UG_ERROR_HUA)
	{
		if (IsOnlyOne(iCardList,iCardCount)) return UG_ONLY_ONE;
		if (IsDouble(iCardList,iCardCount)) return UG_DOUBLE;
		if (IsThree(iCardList,iCardCount)) return UG_THREE;
		if (IsDoubleTrackor(iCardList,iCardCount)) return UG_DOUBLE_TRACKOR;
		if (IsThreeTrackor(iCardList,iCardCount)) return UG_THREE_TARCKOR;
		//if (IsShowCard(iCardList,iCardCount)) return UG_SHOW_CARD;//��Դ��˵���ǲ�Ҫ˦�Ƶ�
	}
	return UG_ERROR_KIND;
}

//�Աȵ���
BOOL CUpGradeGameLogic::CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard)
{
	int iFristHua=GetCardHuaKind(iFirstCard,FALSE);
	int iNextHua=GetCardHuaKind(iNextCard,FALSE);
	if (iFristHua!=iNextHua)
	{
		//��ͬ��ɫ�Ա�
		if (iFristHua==UG_NT_CARD)
			return TRUE;
		return (iNextHua!=UG_NT_CARD);
	}
	
	//ͬ��ɫ�Ա�
	return GetCardBulk(iFirstCard,FALSE) >= GetCardBulk(iNextCard,FALSE);
}

//�Զ����ƺ���
BOOL CUpGradeGameLogic::AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount,
									BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut)
{
	if (bFirstOut==FALSE)	//���������
	{
		int iSameHuaCount=0,iTempCount=0;
		BYTE iSameHuaCard[39],iTempCard[54];
		BYTE iBaseHuaKind=GetCardHuaKind(iBaseCard[0],FALSE);
		BYTE iBaseCardKind=GetCardShape(iBaseCard,iBaseCardCount);

		//��ȡ�˿�
		iSameHuaCount=TackOutCardByHua(iHandCard,iHandCardCount,iBaseHuaKind,iSameHuaCard,FALSE);

		if (iSameHuaCount<=iBaseCardCount)	//û����ͬɫ�˿�
		{
			int iCardPos=iHandCardCount-1;
			iResultCardCount=iSameHuaCount;
			::CopyMemory(iResultCard,iSameHuaCard,sizeof(BYTE)*iSameHuaCount);
			while (iResultCardCount<iBaseCardCount)
			{
				if (GetCardHuaKind(iHandCard[iCardPos],FALSE)!=iBaseHuaKind)
					iResultCard[iResultCardCount++]=iHandCard[iCardPos];
				iCardPos--;
			}
			return TRUE;
		}
		else	//�����˿�
		{
			iResultCardCount=0;
			int iStationPos=0,iTakeCount=0;
			CardAnalyseStruct AnalyseBase;
			AnalyseCard(iBaseCard,iBaseCardCount,AnalyseBase);

			//��������������
			if (AnalyseBase.iThreeTractorCount>0)
			{
				//�������������
				iTakeCount=0,iStationPos=0;
				iTempCount=TackOutThreeTrackor(iSameHuaCard,iSameHuaCount,iTempCard,TRUE);
				iSameHuaCount-=iTempCount;
				while ((iTakeCount<iTempCount)&&(iTakeCount<iBaseCardCount))
				{
					::CopyMemory(&iResultCard[iResultCardCount],&iTempCard[iStationPos+2],sizeof(BYTE)*iTempCard[iStationPos+1]);
					iResultCardCount+=iTempCard[iStationPos+1];
					iTakeCount+=iTempCard[iStationPos+1];
					iStationPos+=iTempCard[iStationPos+1]+2;
				}
				iResultCardCount=__min(iBaseCardCount,iResultCardCount+iTakeCount);
				if (iResultCardCount==iBaseCardCount) return TRUE;
				if (AnalyseBase.iThreeTractorCount>iTempCount)
				{	
					AnalyseBase.iThreeCount+=(AnalyseBase.iThreeTractorCount-iTempCount);
					AnalyseBase.iThreeTractorCount=iTempCount;
				}
			}

			//��������
			if (AnalyseBase.iThreeCount>0)
			{
				//�������
				iTakeCount=0,iStationPos=0;
				iTempCount=TackOutThree(iSameHuaCard,iSameHuaCount,iTempCard,TRUE);
				iSameHuaCount-=iTempCount;
				::CopyMemory(&iResultCard[iResultCardCount],iTempCard,sizeof(BYTE)*iTempCount);
				iResultCardCount=__min(iBaseCardCount,iResultCardCount+iTempCount);
				if (iResultCardCount==iBaseCardCount) return TRUE;
				if (AnalyseBase.iThreeCount>iTempCount)
				{	
					AnalyseBase.iDoubleCount+=(AnalyseBase.iThreeCount-iTempCount)/3*2;
					AnalyseBase.iSignedCount+=(AnalyseBase.iThreeCount-iTempCount)/3;
					AnalyseBase.iThreeTractorCount=iTempCount;
				}
			}

			//��������������
			if (AnalyseBase.iDoubleTractorCount>0)
			{
				//�������������
				iTakeCount=0,iStationPos=0;
				iTempCount=TackOutDoubleTrackor(iSameHuaCard,iSameHuaCount,iTempCard,TRUE);
				iSameHuaCount-=iTempCount;
				while ((iTakeCount<iTempCount)&&(iTakeCount<iBaseCardCount))
				{
					::CopyMemory(&iResultCard[iResultCardCount],&iTempCard[iStationPos+2],sizeof(BYTE)*iTempCard[iStationPos+1]);
					iResultCardCount+=iTempCard[iStationPos+1];
					iTakeCount+=iTempCard[iStationPos+1];
					iStationPos+=iTempCard[iStationPos+1]+2;
				}
				iResultCardCount=__min(iBaseCardCount,iResultCardCount+iTakeCount);
				if (iResultCardCount==iBaseCardCount) return TRUE;
				if (AnalyseBase.iDoubleTractorCount>iTempCount)
				{	
					AnalyseBase.iDoubleCount+=(AnalyseBase.iDoubleTractorCount-iTempCount);
					AnalyseBase.iDoubleTractorCount=iTempCount;
				}
			}

			//���Ʒ���
			if (AnalyseBase.iDoubleCount>0)
			{
				//�������
				iTakeCount=0,iStationPos=0;
				iTempCount=TackOutDouble(iSameHuaCard,iSameHuaCount,iTempCard,TRUE);
				iSameHuaCount-=iTempCount;
				::CopyMemory(&iResultCard[iResultCardCount],iTempCard,sizeof(BYTE)*iTempCount);
				iResultCardCount=__min(iBaseCardCount,iResultCardCount+iTempCount);
				if (iResultCardCount==iBaseCardCount) return TRUE;
				if (AnalyseBase.iDoubleCount>iTempCount)
				{	
					AnalyseBase.iSignedCount+=AnalyseBase.iDoubleCount-iTempCount;
					AnalyseBase.iDoubleCount=iTempCount;
				}
			}

			//�������
			::CopyMemory(&iResultCard[iResultCardCount],&iSameHuaCard[iSameHuaCount+iResultCardCount-iBaseCardCount],
						 sizeof(BYTE)*(iBaseCardCount-iResultCardCount));
			iResultCardCount=iBaseCardCount;

			return TRUE;
		}
	}
	else //��һ��������
	{
		TackOutBySpecifyCard(iHandCard,iHandCardCount,iResultCard,iResultCardCount,iHandCard[iHandCardCount-1]);
		/*
		if(iHandCard[iHandCardCount-1] == iHandCard[iHandCardCount-2])
		{
			iResultCardCount=2;
			iResultCard[0]=iResultCard[1]=iHandCard[iHandCardCount-1];
		}
		else
		{
			iResultCardCount=1;
			iResultCard[0]=iHandCard[iHandCardCount-1];
		}*/
	}

	return TRUE;
}

//�Ƿ����˦��
BOOL CUpGradeGameLogic::CanShowCard(BYTE iOutCard[], int iOutCardCount, int iOutStation, 
									BYTE * iCardArray[], int iCardArrayCount, 
									BYTE iResultCard[], int & iResultCardCount)
{
	CardAnalyseStruct AnalyseOut;
	BYTE iTempCard[54],iSameHuaCard[39];
	int iTempCount=0,iOtherSameCount=0;

	AnalyseCard(iOutCard,iOutCardCount,AnalyseOut);
	for (int i=0;i<PLAY_COUNT;i++)
	{
		if (iOutStation==i) continue;

		//��ȡ�˿�
		iOtherSameCount=TackOutCardByHua(iCardArray[i],iCardArrayCount,AnalyseOut.iCardHuaKind,iSameHuaCard,FALSE);

		//��������������
		if (AnalyseOut.iThreeTractorCount!=0)
		{
			int iOutStation=0,iOtherStation=0,iCompOutCount=0,iCompOtherCount=0;
			iTempCount=TackOutThreeTrackor(iSameHuaCard,iOtherSameCount,iTempCard,FALSE);
			if (iTempCount!=0)
			{
				do
				{
					iOtherStation=0;
					iCompOtherCount=0;
					while (iCompOtherCount<iTempCount)
					{
						if ((AnalyseOut.iThreeTractorArray[iOutStation+1]<=iTempCard[iOtherStation+1])
							&&(CompareOnlyOne(AnalyseOut.iThreeTractorArray[iOutStation+2],iTempCard[0])==FALSE))
						{
							//˦��ʧ��
							iResultCardCount=AnalyseOut.iThreeTractorArray[iOutStation+1];
							::CopyMemory(iResultCard,&AnalyseOut.iThreeTractorArray[iOutStation+2],sizeof(BYTE)*iResultCardCount);
							return FALSE;
						}
						iCompOtherCount+=iTempCard[iOtherStation+1];
						iOtherStation+=iTempCard[iOtherStation+1]+2;
					}
					iCompOutCount+=AnalyseOut.iThreeTractorArray[iOutStation+1];
					iOutStation+=AnalyseOut.iThreeTractorArray[iOutStation+1]+2;
				} while (iCompOutCount<AnalyseOut.iThreeTractorCount);
			}
		}

		//��������������
		if (AnalyseOut.iDoubleTractorCount!=0)
		{
			int iOutStation=0,iOtherStation=0,iCompOutCount=0,iCompOtherCount=0;
			iTempCount=TackOutDoubleTrackor(iSameHuaCard,iOtherSameCount,iTempCard,FALSE);
			if (iTempCount!=0)
			{
				do
				{
					iOtherStation=0;
					iCompOtherCount=0;
					while (iCompOtherCount<iTempCount)
					{
						if ((AnalyseOut.iDoubleTractorArray[iOutStation+2]<=iTempCard[iOtherStation+2])
							&&(CompareOnlyOne(AnalyseOut.iDoubleTractorArray[iOutStation+2],iTempCard[2])==FALSE))
						{
							//˦��ʧ��
							iResultCardCount=AnalyseOut.iDoubleTractorArray[iOutStation+1];
							::CopyMemory(iResultCard,&AnalyseOut.iDoubleTractorArray[iOutStation+2],sizeof(BYTE)*iResultCardCount);
							return FALSE;
						}
						iCompOtherCount+=iTempCard[iOtherStation+1];
						iOtherStation+=iTempCard[iOtherStation+1]+2;
					}
					iCompOutCount+=AnalyseOut.iDoubleTractorArray[iOutStation+1];
					iOutStation+=AnalyseOut.iDoubleTractorArray[iOutStation+1]+2;
				} while (iCompOutCount<AnalyseOut.iDoubleTractorCount);
			}
		}
		
		//��������
		if (AnalyseOut.iThreeCount!=0)
		{
			iTempCount=TackOutThree(iSameHuaCard,iOtherSameCount,iTempCard,FALSE);
			if ((iTempCount!=0)&&(CompareOnlyOne(AnalyseOut.iThreeArray[AnalyseOut.iThreeCount-3],iTempCard[0])==FALSE))
			{
				//˦��ʧ��
				iResultCardCount=3;
				::CopyMemory(iResultCard,&AnalyseOut.iThreeArray[AnalyseOut.iThreeCount-3],sizeof(BYTE)*iResultCardCount);
				return FALSE;
			}
		}

		//��������
		if (AnalyseOut.iDoubleCount!=0)
		{
			iTempCount=TackOutDouble(iSameHuaCard,iOtherSameCount,iTempCard,FALSE);
			if ((iTempCount!=0)&&(CompareOnlyOne(AnalyseOut.iDoubleArray[AnalyseOut.iDoubleCount-2],iTempCard[0])==FALSE))
			{
				//˦��ʧ��
				iResultCardCount=2;
				::CopyMemory(iResultCard,&AnalyseOut.iDoubleArray[AnalyseOut.iDoubleCount-2],sizeof(BYTE)*iResultCardCount);
				return FALSE;
			}
		}

		//��������
		if ((AnalyseOut.iSignedCount!=0)&&(iOtherSameCount>0)&&
			(CompareOnlyOne(AnalyseOut.iLessSignedCard,iSameHuaCard[0])==FALSE))
		{
			//˦��ʧ��
			iResultCardCount=1;
			iResultCard[0]=AnalyseOut.iLessSignedCard;
			return FALSE;
		}
	}

	//˦�Ƴɹ�
	iResultCardCount=iOutCardCount;
	::CopyMemory(iResultCard,iOutCard,sizeof(BYTE)*iResultCardCount);

	return TRUE;
}

//�ԱȺ���
int CUpGradeGameLogic::CompareCard(BYTE * iCardListArray[], int iCardCount)
{
	//�����˿�
	for (int i=0;i<PLAY_COUNT;i++)
		SortCard(iCardListArray[i],NULL,iCardCount);
	
	//�Աȴ���
	int iWinner=0;
	int iCardShape=GetCardShape(iCardListArray[0],iCardCount);
	switch (iCardShape)
	{
		case UG_ONLY_ONE:
		case UG_DOUBLE:
		case UG_THREE: 
		case UG_DOUBLE_TRACKOR:
		case UG_THREE_TARCKOR:
			{
//				for (int i=1;i<4;i++)
				for (int i=3;i>=0;i--)
				{//��Ϊ����ʱ����Ƶ�
					if ((GetCardShape(iCardListArray[i],iCardCount)==iCardShape)&&
					   (CompareOnlyOne(iCardListArray[iWinner][0],iCardListArray[i][0])==FALSE))
						iWinner=i;
				}
				return iWinner;
			}
		case UG_SHOW_CARD: return CompareShowCard(iCardListArray,iCardCount);
	}

	return -1;
}

//�Ա�˦��
int CUpGradeGameLogic::CompareShowCard(BYTE * iCardListArray[], int iCardCount)
{
	CardAnalyseStruct AnalyseResul[4];

	//�����˿�
	AnalyseCard(iCardListArray[0],iCardCount,AnalyseResul[0]);
	if (AnalyseResul[0].iCardHuaKind==UG_NT_CARD) return 0;

	///////////
	//���������ɱ������׷ɱ���Ͳ��ǰ������ķ�ʽ�Ƚ��ˣ����Ǳ�����һ����
	///////////

	//��ʼ�Ա�
	int iWiner=0;
//	for (int i=1;i<4;i++)
	////��Ϊ����ʱ����Ƶ�
	for (int i=3;i>=0;i--)
	{
		if (GetCardListHua(iCardListArray[i],iCardCount)!=UG_NT_CARD) 
			continue;
		if (FixCardType(iCardListArray[i],iCardCount,AnalyseResul[0],AnalyseResul[i])==FALSE) continue;

		//��������������
		if (AnalyseResul[0].iThreeTractorCount!=0)
		{
			if (AnalyseResul[i].iThreeTractorCount!=AnalyseResul[iWiner].iThreeTractorCount) continue;
			if (CompareOnlyOne(AnalyseResul[iWiner].iThreeTractorArray[2],AnalyseResul[i].iThreeTractorArray[2])==TRUE)
				continue;
		}

		//��������������
		if (AnalyseResul[0].iDoubleTractorCount!=0)
		{
			if (AnalyseResul[i].iDoubleTractorCount!=AnalyseResul[iWiner].iDoubleTractorCount) continue;
			if (CompareOnlyOne(AnalyseResul[iWiner].iDoubleArray[2],AnalyseResul[i].iDoubleArray[2])==TRUE)
	            continue;
		}

		//��������
		if (AnalyseResul[0].iThreeCount!=0)
		{
			if (AnalyseResul[i].iThreeCount!=AnalyseResul[iWiner].iThreeCount) continue;
			if (CompareOnlyOne(AnalyseResul[iWiner].iThreeArray[0],AnalyseResul[i].iThreeArray[0])==TRUE)
				continue;
		}

		//��������
		if (AnalyseResul[0].iDoubleCount!=0)
		{
			if (AnalyseResul[i].iDoubleCount!=AnalyseResul[iWiner].iDoubleCount) continue;
			if (CompareOnlyOne(AnalyseResul[iWiner].iDoubleArray[0],AnalyseResul[i].iDoubleArray[0])==TRUE)
				continue;
		}

		//��������
		if (AnalyseResul[i].iSignedCount!=0)
		{
			if (AnalyseResul[i].iSignedCount!=AnalyseResul[iWiner].iSignedCount) continue;
			if (CompareOnlyOne(AnalyseResul[iWiner].iBigSignedCard,AnalyseResul[i].iBigSignedCard)==TRUE)
				continue;
		}

		//�Աȳɹ�
		iWiner=i;
	}

	//����׷ɱ�����
	if(iWiner!=0 && iCardCount>=3)
	{
		for(int i=1;i<4;i++)
		{
			if (GetCardListHua(iCardListArray[i],iCardCount)!=UG_NT_CARD) continue;
			if (FixCardType(iCardListArray[i],iCardCount,AnalyseResul[0],AnalyseResul[i])==FALSE) continue;

			//��������������
			if (AnalyseResul[0].iThreeTractorCount!=0)
			{
				if (AnalyseResul[i].iThreeTractorCount!=AnalyseResul[iWiner].iThreeTractorCount) continue;
				if (CompareOnlyOne(AnalyseResul[iWiner].iThreeTractorArray[2],AnalyseResul[i].iThreeTractorArray[2])==FALSE)
				{
					iWiner=i;
					break;
				}
			}

			//��������������
			if (AnalyseResul[0].iDoubleTractorCount!=0)
			{
				if (AnalyseResul[i].iDoubleTractorCount!=AnalyseResul[iWiner].iDoubleTractorCount) continue;
				if (CompareOnlyOne(AnalyseResul[iWiner].iDoubleArray[2],AnalyseResul[i].iDoubleArray[2])==FALSE)
				{
					iWiner=i;
					break;
				}
			}

			//��������
			if (AnalyseResul[0].iThreeCount!=0)
			{
				if (AnalyseResul[i].iThreeCount!=AnalyseResul[iWiner].iThreeCount) continue;
				if (CompareOnlyOne(AnalyseResul[iWiner].iThreeArray[0],AnalyseResul[i].iThreeArray[0])==FALSE)
				{
					iWiner=i;
					break;
				}
			}

			//��������
			if (AnalyseResul[0].iDoubleCount!=0)
			{
				if (AnalyseResul[i].iDoubleCount!=AnalyseResul[iWiner].iDoubleCount) continue;
				if (CompareOnlyOne(AnalyseResul[iWiner].iDoubleArray[0],AnalyseResul[i].iDoubleArray[0])==FALSE)
				{
					iWiner=i;
					break;
				}
			}

			//��������
			if (AnalyseResul[i].iSignedCount!=0)
			{
				if (AnalyseResul[i].iSignedCount!=AnalyseResul[iWiner].iSignedCount) continue;
				if (CompareOnlyOne(AnalyseResul[iWiner].iBigSignedCard,AnalyseResul[i].iBigSignedCard)==FALSE)
				{
					iWiner=i;
					break;
				}
			}
		}
	}
	return iWiner;
}

//��������
BOOL CUpGradeGameLogic::AnalyseCard(BYTE iCardList[], int iCardCount, CardAnalyseStruct & Analyse)
{
	//��ʼ������
	::memset(&Analyse,0,sizeof(Analyse));

	//�ж��Ƿ�ͬ��ɫ
	Analyse.iCardHuaKind=GetCardListHua(iCardList,iCardCount);
	if (Analyse.iCardHuaKind==UG_ERROR_HUA) return FALSE;

	//������ʱ����
	BYTE iTempCard[39];
	::CopyMemory(iTempCard,iCardList,sizeof(BYTE)*iCardCount);

	//ȥ��һ���˿˵ķ�������
	if (iCardCount==1) 
	{
		Analyse.iSignedCount=1;
		Analyse.iLessSignedCard=iCardList[0];
		return TRUE;
	}

	//��ȡ����
	Analyse.iThreeCount=TackOutThree(iTempCard,iCardCount,Analyse.iThreeArray,TRUE);
	iCardCount-=Analyse.iThreeCount;
	
	//��ȡ����������
	Analyse.iThreeTractorCount=TackOutThreeTrackor(Analyse.iThreeArray,Analyse.iThreeCount,
							   Analyse.iThreeTractorArray,TRUE);
	Analyse.iThreeCount-=Analyse.iThreeTractorCount;

	//��ȡ����
	Analyse.iDoubleCount=TackOutDouble(iTempCard,iCardCount,Analyse.iDoubleArray,TRUE);
	iCardCount-=Analyse.iDoubleCount;

	//��ȡ����������
	Analyse.iDoubleTractorCount=TackOutDoubleTrackor(Analyse.iDoubleArray,Analyse.iDoubleCount,
							    Analyse.iDoubleTractorArray,TRUE);
	Analyse.iDoubleCount-=Analyse.iDoubleTractorCount;

	//��ȡ����
	if (iCardCount>0)
	{
		Analyse.iSignedCount=iCardCount;
		Analyse.iBigSignedCard=iTempCard[0];
		Analyse.iLessSignedCard=iTempCard[iCardCount-1];
	}

	return TRUE;
}

//�仯�˿˸�ʽ
BOOL CUpGradeGameLogic::FixCardType(BYTE iCardList[], int iCardCount, CardAnalyseStruct & BaseAnalyse, CardAnalyseStruct & OutAnalyse)
{
	if (GetCardListHua(iCardList,iCardCount)==UG_ERROR_HUA) return FALSE;

	BYTE iSoureCard[39],iTempCard[39];
	int iSoureCardCount=iCardCount,iTempCount=0;
	::CopyMemory(iSoureCard,iCardList,sizeof(BYTE)*iCardCount);
	::memset(&OutAnalyse,0,sizeof(OutAnalyse));

	//��������������
	if (BaseAnalyse.iThreeTractorCount!=0)
	{
		iTempCount=TackOutThreeTrackor(iSoureCard,iSoureCardCount,iTempCard,TRUE);
		iSoureCardCount-=iTempCount;
		if (BaseAnalyse.iThreeTractorCount>iTempCount) return FALSE;
		int iOutPos=0,iTempPos=0,iNeedLength=0,iCopyLength=0;
		do
		{
			iNeedLength=BaseAnalyse.iThreeTractorCount-OutAnalyse.iThreeTractorCount;
			if (iNeedLength<6) return FALSE;
			iCopyLength=__min(iTempCard[iTempPos+1],iNeedLength);
			::CopyMemory(&OutAnalyse.iThreeTractorArray[iOutPos+2],&iTempCard[iTempPos+2],sizeof(BYTE)*iCopyLength);
			OutAnalyse.iThreeTractorArray[iOutPos]=0;
			OutAnalyse.iThreeTractorArray[iOutPos+1]=iCopyLength;
			OutAnalyse.iThreeTractorCount+=iCopyLength;
			if (iCopyLength==iTempCard[iTempPos+1])
			{
				iTempCount-=iTempCard[iTempPos+1];
				iTempPos+=iTempCard[iTempPos+1]+2;
				iOutPos+=OutAnalyse.iThreeTractorArray[iOutPos+1]+2;
			}
			else
			{
				::CopyMemory(&iSoureCard[iSoureCardCount],&iTempCard[iTempPos+2+iCopyLength],
							 sizeof(BYTE)*(iTempCard[iTempPos+1]-iCopyLength));
				iTempCount-=iTempCard[iTempPos+1];
				iSoureCardCount+=(iTempCard[iTempPos+1]-iCopyLength);
				iTempPos+=iTempCard[iTempPos+1]+2;
				while ((iSoureCardCount+OutAnalyse.iThreeTractorCount)<iCardCount)
				{
					::CopyMemory(&iSoureCard[iSoureCardCount],&iTempCard[iTempPos+2],sizeof(BYTE)*iTempCard[iTempPos+1]);
					iTempCount-=iTempCard[iTempPos+1];
					iSoureCardCount+=iTempCard[iTempPos+1];
					iTempPos+=iTempCard[iTempPos+1]+2;
				}
				SortCard(iSoureCard,NULL,iSoureCardCount);
			}
		} while (BaseAnalyse.iThreeTractorCount>=OutAnalyse.iThreeTractorCount);
	}

	//��������
	if (BaseAnalyse.iThreeCount!=0)
	{
		iTempCount=TackOutThree(iSoureCard,iSoureCardCount,iTempCard,FALSE);
		if (BaseAnalyse.iThreeCount>iTempCount) return FALSE;
		OutAnalyse.iThreeCount=BaseAnalyse.iThreeCount;
		::CopyMemory(OutAnalyse.iThreeArray,iTempCard,sizeof(BYTE)*BaseAnalyse.iThreeCount);
		iSoureCardCount-=RemoveCard(iTempCard,BaseAnalyse.iThreeCount,iSoureCard,iSoureCardCount);
	}

	//��������������
	if (BaseAnalyse.iDoubleTractorCount!=0)
	{
		iTempCount=TackOutDoubleTrackor(iSoureCard,iSoureCardCount,iTempCard,TRUE);
		iSoureCardCount-=iTempCount;
		if (BaseAnalyse.iDoubleTractorCount>iTempCount) return FALSE;
		int iOutPos=0,iTempPos=0,iNeedLength=0,iCopyLength=0;
		do
		{
			iNeedLength=BaseAnalyse.iDoubleTractorCount-OutAnalyse.iDoubleTractorCount;
			if (iNeedLength<4) return FALSE;
			iCopyLength=__min(iTempCard[iTempPos+1],iNeedLength);
			::CopyMemory(&OutAnalyse.iDoubleTractorArray[iOutPos+2],&iTempCard[iTempPos+2],sizeof(BYTE)*iCopyLength);
			OutAnalyse.iDoubleTractorArray[iOutPos]=0;
			OutAnalyse.iDoubleTractorArray[iOutPos+1]=iCopyLength;
			OutAnalyse.iDoubleTractorCount+=iCopyLength;
			if (iCopyLength==iTempCard[iTempPos+1])
			{
				iTempCount-=iTempCard[iTempPos+1];
				iTempPos+=iTempCard[iTempPos+1]+2;
				iOutPos+=OutAnalyse.iDoubleTractorArray[iOutPos+1]+2;
			}
			else
			{
				::CopyMemory(&iSoureCard[iSoureCardCount],&iTempCard[iTempPos+2+iCopyLength],
							 sizeof(BYTE)*(iTempCard[iTempPos+1]-iCopyLength));
				iTempCount-=iTempCard[iTempPos+1];
				iSoureCardCount+=(iTempCard[iTempPos+1]-iCopyLength);
				iTempPos+=iTempCard[iTempPos+1]+2;
				while ((iSoureCardCount+OutAnalyse.iDoubleTractorCount)<iCardCount)
				{
					::CopyMemory(&iSoureCard[iSoureCardCount],&iTempCard[iTempPos+2],sizeof(BYTE)*iTempCard[iTempPos+1]);
					iTempCount-=iTempCard[iTempPos+1];
					iSoureCardCount+=iTempCard[iTempPos+1];
					iTempPos+=iTempCard[iTempPos+1]+2;
				}
				SortCard(iSoureCard,NULL,iSoureCardCount);
			}
		} while (BaseAnalyse.iDoubleTractorCount>=OutAnalyse.iDoubleTractorCount);
	}

	//��������
	if (BaseAnalyse.iDoubleCount!=0)
	{
		iTempCount=TackOutDouble(iSoureCard,iSoureCardCount,iTempCard,FALSE);
		if (BaseAnalyse.iDoubleCount>iTempCount) return FALSE;
		OutAnalyse.iDoubleCount=BaseAnalyse.iDoubleCount;
		::CopyMemory(OutAnalyse.iDoubleArray,iTempCard,sizeof(BYTE)*BaseAnalyse.iDoubleCount);
		iSoureCardCount-=RemoveCard(iTempCard,BaseAnalyse.iDoubleCount,iSoureCard,iSoureCardCount);
	}

	//��������
	if (iSoureCardCount>0)
	{
		OutAnalyse.iSignedCount=iSoureCardCount;
		OutAnalyse.iBigSignedCard=iSoureCard[0];
		OutAnalyse.iLessSignedCard=iSoureCard[iSoureCardCount-1];
	}

	return TRUE;
}

//��ȡ���ƺ���
int CUpGradeGameLogic::TackOutDouble(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], BOOL bRemoveCard)
{
	int iCount=0;
	for (int i=0;i<iCardCount-1;i++)
	{
		if (iCardList[i]==iCardList[i+1])
		{
			iDoubleBuffer[iCount+1]=iDoubleBuffer[iCount]=iCardList[i];
			iCount+=2;
			if (bRemoveCard) iCardList[i]=iCardList[i+1]=0;
			i++;
		}
	}
	if (iCount && bRemoveCard) RemoveNummCard(iCardList,iCardCount);

	return iCount;
}

//��ȡ��������������
int CUpGradeGameLogic::TackOutDoubleTrackor(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], BOOL bRemoveCard)
{
	BOOL bEndFind=FALSE;
	int iCount=0,iRecordPos=0,iDataPos=0,iTrackorLen=0;

	for (int i=0;i<iCardCount-3;i++)
	{
		if (IsDouble(&iCardList[i],2))
		{
			iDataPos=i;
			iTrackorLen=2;
			bEndFind=FALSE;
			do
			{
				if ((IsDouble(&iCardList[i+2],2))&&(IsTrackorRule(iCardList[i],iCardList[i+2])))
				{
					//��������
					i+=2;
					iTrackorLen+=2;
				}
				else bEndFind=TRUE;
			} while ((bEndFind==FALSE)&&(i<iCardCount-3));

			//��¼����
			if (iTrackorLen>=4)
			{
				iCount+=iTrackorLen;
				iDoubleBuffer[iRecordPos]=0;
				iDoubleBuffer[iRecordPos+1]=iTrackorLen;
				for (int j=0;j<iTrackorLen;j++)
				{
					iDoubleBuffer[iRecordPos+j+2]=iCardList[iDataPos+j];
					if (bRemoveCard) iCardList[iDataPos+j]=0;
				}
				iRecordPos+=(iTrackorLen+2);
			}
		}
	}
	if (iCount && bRemoveCard) RemoveNummCard(iCardList,iCardCount);

	return iCount;
}

//��ȡ��������
int CUpGradeGameLogic::TackOutThree(BYTE iCardList[], int iCardCount, BYTE iThreeBuffer[], BOOL bRemoveCard)
{
	int iCount=0;
	for (int i=0;i<iCardCount-2;i++)
	{
		if ((iCardList[i]==iCardList[i+1])&&(iCardList[i]==iCardList[i+2]))
		{
			iThreeBuffer[iCount]=iThreeBuffer[iCount+1]=iThreeBuffer[iCount+2]=iCardList[i];
			iCount+=3;
			if (bRemoveCard) iCardList[i]=iCardList[i+1]=iCardList[i+2]=0;
			i+=2;
		}
	}
	if (iCount && bRemoveCard) RemoveNummCard(iCardList,iCardCount);

	return iCount;
}

//��ȡ��������������
int CUpGradeGameLogic::TackOutThreeTrackor(BYTE iCardList[], int iCardCount, BYTE iThreeBuffer[], BOOL bRemoveCard)
{
	BOOL bEndFind=FALSE;
	int iCount=0,iRecordPos=0,iDataPos=0,iTrackorLen=0;

	for (int i=0;i<iCardCount-5;i++)
	{
		if (IsThree(&iCardList[i],3))
		{
			iDataPos=i;
			iTrackorLen=3;
			bEndFind=FALSE;
			do
			{
				if ((IsThree(&iCardList[i+3],3))&&(IsTrackorRule(iCardList[i],iCardList[i+3])))
				{
					//��������
					i+=3;
					iTrackorLen+=3;
				}
				else bEndFind=TRUE;
			} while ((bEndFind==FALSE)&&(i<iCardCount-5));

			//��¼����
			if (iTrackorLen>=6)
			{
				iCount+=iTrackorLen;
				iThreeBuffer[iRecordPos]=0;
				iThreeBuffer[iRecordPos+1]=iTrackorLen;
				for (int j=0;j<iTrackorLen;j++)
				{
					iThreeBuffer[iRecordPos+j+2]=iCardList[iDataPos+j];
					if (bRemoveCard) iCardList[iDataPos+j]=0;
				}
				iRecordPos+=(iTrackorLen+2);
			}
		}
	}
	if (iCount && bRemoveCard) RemoveNummCard(iCardList,iCardCount);

	return iCount;
}

//��ȡͬһ��ɫ�˿�
int CUpGradeGameLogic::TackOutCardByHua(BYTE iCardList[], int iCardCount, BYTE iHuaKind, BYTE iCardBuffer[], BOOL bRemoveCard)
{int iBeginPos,iEndPos;//Visual C++ .Net 2005
	for (iBeginPos=0;iBeginPos<iCardCount;iBeginPos++)
	{
		if (GetCardHuaKind(iCardList[iBeginPos],FALSE)==iHuaKind)
			break;
	}
	if (iBeginPos==iCardCount)
		return 0;
	for (iEndPos=iCardCount-1;iEndPos>=0;iEndPos--)
	{
		if (GetCardHuaKind(iCardList[iEndPos],FALSE)==iHuaKind)
			break;
	}
	::CopyMemory(iCardBuffer,&iCardList[iBeginPos],sizeof(BYTE)*(iEndPos-iBeginPos+1));
	if (bRemoveCard) 
		RemoveNummCard(iCardList,iCardCount);
	return (iEndPos-iBeginPos+1);
}

//��� 0 λ�˿�
int CUpGradeGameLogic::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0;i<iCardCount;i++)
	{
		if (iCardList[i]!=0) 
			iCardList[i-iRemoveCount]=iCardList[i];
		else iRemoveCount++;
	}
	return iRemoveCount;
}

//�����˿�
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,BOOL bHaveKing)
{
	static const BYTE m_CardArray[46]={
			0x01, /*0x02 ,0x03,*/ 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
			0x11, /*0x12 ,0x13,*/ 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
			0x21, /*0x22 ,0x23,*/ 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
			0x31, /*0x32 ,0x33,*/ 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
			0x4E, 0x4F};																		//С�����

	BYTE iSend=0,iStation=0,iCardList[162];
	srand((unsigned)time(NULL));
	
	for (int i=0;i<iCardCount;i+=46)
		::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));
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
	if ((iRemoveCount>iCardCount)) 
		return 0;

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
	if (iDeleteCount!=iRemoveCount)
		return 0;

	return iDeleteCount;
}

//�Ƿ���Գ���
BOOL CUpGradeGameLogic::CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], 
								   int iBaseCount, BYTE iHandCard[], int iHandCount)
{
	//�ж���Ŀ�Ƿ���ͬ
	if (iOutCount!=iBaseCount)
		return FALSE;

	//��ȡ�˿˵Ļ�����Ϣ
	int iBaseHua=GetCardListHua(iBaseCard,iBaseCount);
	int iOutHua=GetCardListHua(iOutCard,iOutCount);
	BYTE iBaseKind=GetCardShape(iBaseCard,iBaseCount);
	BYTE iOutKind=GetCardShape(iOutCard,iOutCount);
	BYTE iSameHuaCardBuf[39],iTempCard[54];

	//�ж��Ƿ������ֵĳ��Ʋ�������
	if ((iBaseKind==iOutKind)&&(iBaseHua==iOutHua)&&(iBaseKind!=UG_SHOW_CARD))	
		return TRUE;
	
	int iSameHuaCount=TackOutCardByHua(iHandCard,iHandCount,iBaseHua,iSameHuaCardBuf,FALSE);

	if (iBaseHua!=iOutHua)	//��ɫ����ͬ
	{
		//��ɫ����ͬ,�ж��Ƿ�û�����ֻ�ɫ���˿�
		return (TackOutCardByHua(iOutCard,iOutCount,iBaseHua,iTempCard,FALSE)==iSameHuaCount);
	}
	if ((iOutKind!=iBaseKind)&&(iBaseKind!=UG_SHOW_CARD))	//���Ͳ���ͬ
	{
		//���Ͳ���ͬ���ж��Ƿ���Ϸ��û�д�����
		switch (iBaseKind)
		{
			//�������͵��Ѿ��ڻ�ɫ�ж��й��˵���
		case UG_DOUBLE:
			{
				//�ж��Ƿ�û���˶���
				return TackOutDouble(iSameHuaCardBuf,iSameHuaCount,iTempCard,FALSE)==0;
			}
		case UG_THREE:
			{
				//�ж��Ƿ�û��������
				if (TackOutThree(iSameHuaCardBuf,iSameHuaCount,iTempCard,FALSE)!=0) return FALSE;

				//�ж��Ƿ��Ѿ�����������
				BYTE iMeOutLength=TackOutDouble(iOutCard,iOutCount,iTempCard,FALSE);
				BYTE iMeHandLength=TackOutDouble(iSameHuaCardBuf,iSameHuaCount,iTempCard,FALSE);
				if ((iMeHandLength>iMeOutLength)&&(iMeOutLength<(iBaseCount-1))) return FALSE;

				return TRUE;
			}
		case UG_DOUBLE_TRACKOR:
			{
				//�ж���Ϸ���Ƿ�����������
				BYTE iMeOutLength=MaxDoubleTractorLong(iOutCard,iOutCount,NULL);
				BYTE iMeHandLength=MaxDoubleTractorLong(iSameHuaCardBuf,iSameHuaCount,NULL);
				if (iMeHandLength>iMeOutLength) return FALSE;
				iBaseCount-=iMeOutLength;

				//�ж��Ƿ��Ѿ�����������
				iMeOutLength=TackOutDouble(iOutCard,iOutCount,iTempCard,FALSE);
				iMeHandLength=TackOutDouble(iSameHuaCardBuf,iSameHuaCount,iTempCard,FALSE);
				if ((iMeHandLength>iMeOutLength)&&(iMeOutLength<(iBaseCount-1))) return FALSE;

				return TRUE;
			}
		case UG_THREE_TARCKOR:
			{
				//�ж���Ϸ���Ƿ�����������
				BYTE iMeOutLength=MaxThreeTractorLong(iOutCard,iOutCount,NULL);
				BYTE iMeHandLength=MaxThreeTractorLong(iSameHuaCardBuf,iSameHuaCount,NULL);
				if (iMeHandLength>iMeOutLength) return FALSE;
				iBaseCount-=iMeOutLength;

				//�ж��Ƿ��Ѿ�����������
				iMeOutLength=TackOutThree(iOutCard,iOutCount,iTempCard,FALSE);
				iMeHandLength=TackOutThree(iSameHuaCardBuf,iSameHuaCount,iTempCard,FALSE);
				if ((iMeHandLength>iMeOutLength)&&(iMeOutLength<iBaseCount)) return FALSE;
				iBaseCount-=iMeOutLength;

				//�ж���Ϸ���Ƿ�����������
				iMeOutLength=MaxDoubleTractorLong(iOutCard,iOutCount,NULL);
				iMeHandLength=MaxDoubleTractorLong(iSameHuaCardBuf,iSameHuaCount,NULL);
				if ((iMeHandLength>iMeOutLength)&&(iMeOutLength<(iBaseCount-1))) return FALSE;
				iBaseCount-=iMeOutLength;

				//�ж��Ƿ��Ѿ�����������
				iMeOutLength=TackOutDouble(iOutCard,iOutCount,iTempCard,FALSE);
				iMeHandLength=TackOutDouble(iSameHuaCardBuf,iSameHuaCount,iTempCard,FALSE);
				if ((iMeHandLength>iMeOutLength)&&(iMeOutLength<(iBaseCount-1))) return FALSE;

				return TRUE;
			}
		}
	}
	else if (iBaseKind==UG_SHOW_CARD)	//˦��
	{
		//�����˿�
		CardAnalyseStruct AnalyBase,AnalyOut,AnalyHand;
		AnalyseCard(iBaseCard,iBaseCount,AnalyBase);
		AnalyseCard(iOutCard,iOutCount,AnalyOut);
		AnalyseCard(iSameHuaCardBuf,iSameHuaCount,AnalyHand);

		//��������������
		if (AnalyOut.iThreeTractorCount<AnalyBase.iThreeTractorCount)
		{
			if (AnalyHand.iThreeTractorCount>AnalyOut.iThreeTractorCount) return FALSE;
			AnalyBase.iThreeCount+=AnalyBase.iThreeTractorCount-AnalyOut.iThreeTractorCount;
			AnalyBase.iThreeTractorCount-=AnalyOut.iThreeTractorCount;
		}
		else
		{
			AnalyHand.iThreeCount+=(AnalyHand.iThreeTractorCount-AnalyBase.iThreeTractorCount);
			AnalyHand.iThreeTractorCount-=AnalyBase.iThreeTractorCount;
			AnalyOut.iThreeCount+=(AnalyOut.iThreeTractorCount-AnalyBase.iThreeTractorCount);
			AnalyOut.iThreeTractorCount-=AnalyBase.iThreeTractorCount;
		}

		//��������
		if (AnalyOut.iThreeCount<AnalyBase.iThreeCount)		
		{
			if (AnalyHand.iThreeCount>AnalyOut.iThreeCount) return FALSE;
			AnalyBase.iDoubleCount+=(AnalyBase.iThreeCount-AnalyOut.iThreeCount)*2/3;
			AnalyBase.iSignedCount+=(AnalyBase.iThreeCount-AnalyOut.iThreeCount)/3;
			AnalyBase.iThreeCount-=AnalyOut.iThreeCount;
		}
		else
		{
			AnalyHand.iDoubleCount+=(AnalyHand.iThreeCount-AnalyBase.iThreeCount)/3*2;
			AnalyHand.iSignedCount+=(AnalyHand.iThreeCount-AnalyBase.iThreeCount)/3;
			AnalyHand.iThreeCount-=AnalyBase.iThreeCount;
			AnalyOut.iDoubleCount+=(AnalyOut.iThreeCount-AnalyBase.iThreeCount)/3*2;
			AnalyOut.iSignedCount+=(AnalyOut.iThreeCount-AnalyBase.iThreeCount)/3;
			AnalyOut.iThreeCount-=AnalyBase.iThreeCount;
		}

		//��������������
		if (AnalyOut.iDoubleTractorCount<AnalyBase.iDoubleTractorCount)
		{
			if (AnalyHand.iDoubleTractorCount>AnalyOut.iDoubleTractorCount) return FALSE;
			AnalyBase.iDoubleCount+=AnalyBase.iDoubleTractorCount-AnalyOut.iDoubleTractorCount;
			AnalyBase.iDoubleTractorCount-=AnalyOut.iDoubleTractorCount;
		}
		else
		{
			AnalyHand.iDoubleCount+=(AnalyHand.iDoubleTractorCount-AnalyBase.iDoubleTractorCount);
			AnalyHand.iDoubleTractorCount-=AnalyBase.iDoubleTractorCount;
			AnalyOut.iDoubleCount+=(AnalyOut.iDoubleTractorCount-AnalyBase.iDoubleTractorCount);
			AnalyOut.iDoubleTractorCount-=AnalyBase.iDoubleTractorCount;
		}
		
		//���Ʒ���
		if ((AnalyOut.iDoubleCount<AnalyBase.iDoubleCount)&&(AnalyHand.iDoubleCount>AnalyOut.iDoubleCount))
			return FALSE;
	}
	return TRUE;
}

//���ҷ���
int CUpGradeGameLogic::FindPoint(BYTE iCardList[], int iCardCount)
{
	int iPoint=0,iNum=0;
	for (int i=0;i<iCardCount;i++)
	{
		iNum=GetCardNum(iCardList[i]);
		if (iNum==5) iPoint+=5;
		else if ((iNum==10)||(iNum==13)) 
			iPoint+=10;
	}
	return iPoint;
}

//�������������
int	CUpGradeGameLogic::MaxThreeTractorLong(BYTE iCardList[], int iCardCount, BYTE iTrackorBuffer[])
{
	int iMaxPos=0,iMaxCount=0,i=0;
	BYTE iTempThreeTrackor[48];
	int iCount=TackOutThreeTrackor(iCardList,iCardCount,iTempThreeTrackor,FALSE);
	while (i<iCount)
	{
		if (iTempThreeTrackor[i+1]>iMaxCount)
		{
			iMaxPos=i;
			iMaxCount=iTrackorBuffer[i+1];
		}
		i+=(iTempThreeTrackor[i+1]+2);
	}
	if ((iMaxCount!=0)&&(iTrackorBuffer!=NULL)) 
		::CopyMemory(iTrackorBuffer,&iTempThreeTrackor[iMaxPos],sizeof(BYTE)*(iMaxCount+2));

	return iMaxCount;
}

//�������������
int	CUpGradeGameLogic::MaxDoubleTractorLong(BYTE iCardList[], int iCardCount, BYTE iTrackorBuffer[])
{
	int iMaxPos=0,iMaxCount=0,i=0;
	BYTE iTempDoubleTrackor[54];
	int iCount=TackOutDoubleTrackor(iCardList,iCardCount,iTempDoubleTrackor,FALSE);
	while (i<iCount)
	{
		if (iTempDoubleTrackor[i+1]>iMaxCount)
		{
			iMaxPos=i;
			iMaxCount=iTempDoubleTrackor[i+1];
		}
		i+=(iTempDoubleTrackor[i+1]+2);
	}
	if ((iMaxCount!=0)&&(iTrackorBuffer!=NULL)) 
		::CopyMemory(iTrackorBuffer,&iTempDoubleTrackor[iMaxPos],sizeof(BYTE)*(iMaxCount+2));

	return iMaxCount;
}
//���û�������������Ŀ 
int CUpGradeGameLogic::FindTractorNum(BYTE iCardList[],int iCardCount)
{
	int iCount=0;
	int iSameHuaCount=0,iTempCount=0;
	BYTE iSameHuaCard[39];
	for(int i=0;i<4;i++)
	{	
		::memset(iSameHuaCard,0,sizeof(iSameHuaCard));
		iSameHuaCount=TackOutCardByHua(iCardList, iCardCount,i*16, iSameHuaCard, false);
		CardAnalyseStruct AnalyBase;
		AnalyseCard(iSameHuaCard,iSameHuaCount,AnalyBase);
		if(AnalyBase.iDoubleTractorCount!=0)								//����������
			iCount+=AnalyBase.iDoubleTractorCount/4;
	}
	return iCount;
}


//��ȡָ���Ƹ���
BYTE CUpGradeGameLogic::GetCountBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE bCard)
{
	int count = 0;
	for(int i = 0;i < iCardCount;i ++)
		if(iCardList[i] == bCard) 
			count++;

	return count;
}

//��ȡָ���Ʒ����ҵ��Ƃ���
BYTE  CUpGradeGameLogic::TackOutBySpecifyCard(BYTE iCardList[], int iCardCount,BYTE bCardBuffer[],int &iResultCardCount,BYTE bCard)
{
	iResultCardCount = 0;
	for(int i = 0;i < iCardCount;i++)
	{
		if(iCardList[i] == bCard)
		bCardBuffer[iResultCardCount ++] = iCardList[i];
	}
	return iResultCardCount;
}
/*
paramenter iCardList[] �����ƶ���
paramenter iCardCount,�����ƶ�������
paramenter iResultCard[],����ƶ���
paramenter iResultCardCount,����Ƴ���
paramenter iConditionCard[] ������
paramenter iConditionCount  ������������
return value true found >0 
			false found =0
*/

	//��������ĳ��������
BOOL CUpGradeGameLogic::TackOutByCondition(BYTE iCardList[],int iCardCount, 
										   BYTE iResultCard[], int & iResultCardCount,
										   BYTE iConditionCard[],int iConditionCardCount)
{
	iResultCardCount = 0;
	for(int i = 0 ;i < iConditionCardCount ; i++)
	{
		for(int j = 0 ;j<iCardCount ;j ++)
		{
			if(iConditionCard[i] == iCardList[j])
				iResultCard[iResultCardCount++] = iCardList[j]; 
		}
	}
	return iResultCardCount;
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