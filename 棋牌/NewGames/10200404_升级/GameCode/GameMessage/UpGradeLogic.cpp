#include "StdAfx.h"
#include "UpGradeLogic.h"
#include<algorithm>
#include<vector>
extern T_CS_OutCard;

//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	m_bSortCardStyle = 0; //0 �ư���С����;1 ����������
	m_iCondition=0;			//�޳�����������
	m_bKingCanReplace=false;
	m_byMajorFlower=255;//����ɫ
	m_byLevel=255;//����
}
//���캯��
CUpGradeGameLogic::~CUpGradeGameLogic()
{
}
//���ü���ͻ�ɫ
void CUpGradeGameLogic::SetLevelAndFlower(BYTE byLevel,BYTE byMajorFlower)
{
	m_byLevel=byLevel;//����
	m_byMajorFlower=byMajorFlower;//����ɫ
}
//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard, BOOL bTrueHua)
{ 
	int iHuaKind=(iCard&UG_HUA_MASK);
	if (!bTrueHua)
	{
		return iHuaKind=UG_NT_CARD;
	}
	return iHuaKind; 
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard, bool bExtVal)
{
	if ((iCard == 0x4E) || (iCard == 0x4F))
	{
		return bExtVal ? (iCard-14) : (iCard-62); //��С��64+14-62=16	ֻ���ش�Сè��ֵ
	}
	int iCardNum = GetCardNum(iCard);
	int iHuaKind = GetCardHuaKind(iCard, TRUE);
	if (iCardNum == 2) //2��
	{
		if(bExtVal) //�й�
		{
			return ((iHuaKind>>4)+(15*4));
		}
		else //û�й�����2��
		{
			return 15;
		}
	}
	return ((bExtVal) ? ((iHuaKind>>4)+(iCardNum*4)) : (iCardNum));
}

//��ֵ�õ���
BYTE CUpGradeGameLogic::GetCardByValue(int Value)
{
	BYTE CardArray[55]={
		0x00,
		0x01,0x11, 0x21,0x31,
		0x02, 0x12 ,0x22 , 0x32 ,
		0x03,0x13, 0x23,0x33,
		0x04,0x14, 0x24, 0x34,
		0x05, 0x15,0x25, 0x35,
		0x06, 0x16, 0x26,0x36,
		0x07, 0x17,0x27, 0x37,
		0x08, 0x18, 0x28,  0x38,
		0x09, 0x19, 0x29,0x39,
		0x0A,  0x1A, 0x2A, 0x3A,
		0x0B,0x1B,  0x2B,0x3B,
		0x0C, 0x1C,0x2C, 0x3C,
		0x0D,  0x1D, 0x2D, 0x3D,
		0x4E, 0x4F};

		return CardArray[Value];
}


//ɾ���˿�
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[],   //Ҫɾ��������
								  int iRemoveCount,		//Ҫɾ����������
								  BYTE iCardList[],		//Ҫ���������
								  int iCardCount)		//�������������
{
	//��������
	if(iRemoveCount > iCardCount) return 0;
	vector<BYTE> vTemp;
	vTemp.clear();
	int iDeleteCount = 0; //��Ҫɾ����������
	for (int i = 0; i < iRemoveCount; i++)
	{
		for (int j = 0; j < iCardCount; j++)
		{
			if (iRemoveCard[i] == iCardList[j])
			{
				iDeleteCount++;
				iCardList[j] = 0;
				break;
			}
		}
	}
	for (int i = 0; i < iCardCount; i++)
	{
		if(IsLegalCard(iCardList[i]))
		{
			vTemp.push_back(iCardList[i]);
		}
	}
	memset(iCardList,0,sizeof(BYTE)*iCardCount);
	for(int i = 0; i <vTemp.size(); i++)
	{
		iCardList[i]=vTemp.at(i);
	}
	return iDeleteCount;
}
//��ɫ�Ƿ�Ϸ�
bool CUpGradeGameLogic::IsLegalFlower(BYTE byFlower)
{
	if(0x00==byFlower || 0x10==byFlower || 0x20==byFlower || 0x30==byFlower)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//�ж����Ƿ�Ϸ�
bool CUpGradeGameLogic::IsLegalCard(BYTE byCard)
{
	if(byCard>=0x01 && byCard<=0x4F)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BOOL CUpGradeGameLogic::ReverseCard(BYTE iCardList[], BYTE bUp[], BYTE iCardCount)
{
	BYTE iTemp;
	for(int i=0;i< iCardCount /2 ;i++)
	{
		iTemp = iCardList[i];
		iCardList[i] = iCardList[iCardCount - 1 -i];
		iCardList[iCardCount - 1 -i] = iTemp;
	}
	return TRUE;
}
//��չ����
BOOL CUpGradeGameLogic::SortCardByEx(BYTE iCardList[], BYTE bUp[], BYTE iCardCount,BOOL bSysSort)
{
	BOOL bSorted=TRUE,bTempUp;
	int iTemp,iLast=0,iStationVol[45];
	memset(iStationVol, 0, sizeof(iStationVol));
	if (iCardCount > 45)
	{
		iCardCount = 45;
	}
	iLast=iCardCount-1;
	//��ȡλ����ֵ
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i], true);
		//if (iStationVol[i]>=15) iStationVol[i]+=m_iStation[4];
		//else iStationVol[i]+=m_iStation[GetCardHuaKind(iCardList[i],FALSE)>>4];
	}

	//�������(���Ӵ�С����)
	do
	{
		bSorted=TRUE;
		for (int i=0;i<iLast;i++)
		{
			if (iStationVol[i]<iStationVol[i+1])
			{	
				//����λ��				//==ð������
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

	//ϵͳ���в����ǻ�ɫ��������
	if(bSysSort)
	{
		ReverseCard(iCardList,bUp,iCardCount);
		return TRUE;
	}
	if(GetSortCardStyle() == 1) //����������
		SortCardByStyle(iCardList,iCardCount);

	if(GetSortCardStyle() == 2)
		SortCardByKind(iCardList,iCardCount);


	return TRUE;
}
//����������
BOOL CUpGradeGameLogic::SortCardByStyle(BYTE iCardList[], BYTE iCardCount)
{
	//�������������Ҫ�󰴴�С����
	if(m_bSortCardStyle == 0)
	{
		SortCardByEx(iCardList, NULL, iCardCount);

		return TRUE;
	}

	//����Ĵ���==�������Ŵ�С
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

			if(step>=i)	//�ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj,����λ��Ϊj+setp-1
			{			//����Start��ʼ��j���������setp��
				if(j!=Start) //�ų���ʼ��������
				{
					for(;j>=Start;j--) //��Start����j�ź��ƶ�i��
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
	return TRUE;
}

//����ɫ����
BOOL CUpGradeGameLogic::SortCardByKind(BYTE iCardList[],BYTE iCardCount)
{
	return TRUE;
}

////�����˿�,������ʹ��
//BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,int iRoomId,bool bHaveKing)
//{
//	static const BYTE m_CardArray[54] =
//	{
//		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
//		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
//		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
//		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
//		0x4E, 0x4F //С�����
//	};
//
//	BYTE iSend=0,iStation=0,iCardList[216],step=(bHaveKing?54:52);
//	srand(GetTickCount()+iRoomId);
//
//	for (int i=0;i<iCardCount;i+=step)
//		::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));
//
//	do
//	{
//		iStation=rand()%(iCardCount-iSend);
//		iCard[iSend]=iCardList[iStation];
//		iSend++;
//		iCardList[iStation]=iCardList[iCardCount-iSend];
//	} while (iSend<iCardCount);
//
//	return iCardCount;
//}
////���Ŵ�С���ɴﵽС����,�ڣ��죬÷������@byLedCardColor:  0x00: �� ��0x10:�� �� 0x20:÷�� 0x30:�� �� 0x40:��
void CUpGradeGameLogic::SortCard(BYTE byCardArr[],int iCount)
{
	vector<BYTE> vJor;		//��
	vector<BYTE> vSpade;	//��
	vector<BYTE> vHeart;	//��
	vector<BYTE> vClub;		//÷
	vector<BYTE> vDiamond;	//��
	vector<BYTE> vAllCard;
	vJor.clear();
	vSpade.clear();
	vHeart.clear();
	vClub.clear();
	vDiamond.clear();
	vAllCard.clear();
	
	
	vector<BYTE> vMajorLevel;
	vector<BYTE> vOtherLevel;
	vMajorLevel.clear();
	vOtherLevel.clear();
	for(int i=0;i<iCount;i++)//��ȡ����
	{
		if(!IsLegalCard(byCardArr[i]))
		{
			continue;
		}
		if(m_byLevel==GetCardNum(byCardArr[i]))
		{
			if(IsLegalFlower(m_byMajorFlower) && m_byMajorFlower==GetCardHuaKind(byCardArr[i]))//������
			{
				vMajorLevel.push_back(byCardArr[i]);
			}
			else//������
			{
				vOtherLevel.push_back(byCardArr[i]);
			}
			byCardArr[i]=0;
		}
	}
	//����
	sort(vOtherLevel.begin(),vOtherLevel.end(),greater<BYTE>());
	//�������ڣ��죬÷��������������
	SortByColor(byCardArr,iCount,vJor,vSpade,vHeart,vClub,vDiamond);
		// 1.������
	for (int i =0;i<vJor.size();++i)
	{
		vAllCard.push_back(vJor[i]);
	}
	//����������
	for (int i =0;i<vMajorLevel.size();++i)
	{
		vAllCard.push_back(vMajorLevel.at(i));
	}
	//���븱����
	for (int i =0;i<vOtherLevel.size();++i)
	{
		vAllCard.push_back(vOtherLevel.at(i));
	}
	if(0x30==m_byMajorFlower)//��
	{
		for (int i =0;i<vSpade.size();++i)
		{
			vAllCard.push_back(vSpade[i]);
		}
		//4.���븱��
		for (int i =0;i<vHeart.size();++i)
		{
			vAllCard.push_back(vHeart[i]);
		}
		for (int i =0;i<vClub.size();++i)
		{
			vAllCard.push_back(vClub[i]);
		}
		for (int i =0;i<vDiamond.size();++i)
		{
			vAllCard.push_back(vDiamond[i]);
		}
	}
	else if(0x20==m_byMajorFlower)//��
	{
		for (int i =0;i<vHeart.size();++i)
		{
			vAllCard.push_back(vHeart[i]);
		}
		//4.���븱��
		for (int i =0;i<vSpade.size();++i)
		{
			vAllCard.push_back(vSpade[i]);
		}
		for (int i =0;i<vClub.size();++i)
		{
			vAllCard.push_back(vClub[i]);
		}
		for (int i =0;i<vDiamond.size();++i)
		{
			vAllCard.push_back(vDiamond[i]);
		}
	}
	else if(0x10==m_byMajorFlower)//÷
	{
		for (int i =0;i<vClub.size();++i)
		{
			vAllCard.push_back(vClub[i]);
		}
		//4.���븱��
		for (int i =0;i<vSpade.size();++i)
		{
			vAllCard.push_back(vSpade[i]);
		}
		for (int i =0;i<vHeart.size();++i)
		{
			vAllCard.push_back(vHeart[i]);
		}
		for (int i =0;i<vDiamond.size();++i)
		{
			vAllCard.push_back(vDiamond[i]);
		}
	}
	else if(0x00==m_byMajorFlower)//��
	{
		for (int i =0;i<vDiamond.size();++i)
		{
			vAllCard.push_back(vDiamond[i]);
		}
		//4.���븱��
		for (int i =0;i<vSpade.size();++i)
		{
			vAllCard.push_back(vSpade[i]);
		}
		for (int i =0;i<vHeart.size();++i)
		{
			vAllCard.push_back(vHeart[i]);
		}
		for (int i =0;i<vClub.size();++i)
		{
			vAllCard.push_back(vClub[i]);
		}
	}
	else
	{
		for (int i =0;i<vSpade.size();++i)
		{
			vAllCard.push_back(vSpade[i]);
		}
		//4.���븱��
		for (int i =0;i<vHeart.size();++i)
		{
			vAllCard.push_back(vHeart[i]);
		}
		for (int i =0;i<vClub.size();++i)
		{
			vAllCard.push_back(vClub[i]);
		}
		for (int i =0;i<vDiamond.size();++i)
		{
			vAllCard.push_back(vDiamond[i]);
		}
	}
	memset(byCardArr,0,sizeof(BYTE)*iCount);
	for (int i =0;i<vAllCard.size();++i)
	{
		byCardArr[i] = vAllCard[i];
	}
}

//����ɫ����
void CUpGradeGameLogic::SortByColor(BYTE byCardArr[],int iCount,vector<BYTE> &vJor,vector<BYTE> &vSpade,vector<BYTE> &vHeart,vector<BYTE> &vClub,vector<BYTE> &vDiamond)
{
	vJor.clear();
	vSpade.clear();
	vHeart.clear();
	vClub.clear();
	vDiamond.clear();
	for (int i =0;i<iCount;++i)
	{
		if(!IsLegalCard(byCardArr[i]))
			continue;
		BYTE ret =GetCardHuaKind(byCardArr[i]);
		if(ret == 0x30)	//��
		{
			vSpade.push_back(byCardArr[i]);
		}
		else if(ret == 0x20)	//��
		{
			vHeart.push_back(byCardArr[i]);
		}
		else if(ret == 0x10)	//÷
		{
			vClub.push_back(byCardArr[i]);
		}
		else if(ret == 0x00)	//��
		{
			vDiamond.push_back(byCardArr[i]);
		}
		else if(ret == 0x40)	//��
		{
			vJor.push_back(byCardArr[i]);
		}
	}
	//����
	sort(vJor.begin(),vJor.end(),greater<BYTE>());
	sort(vSpade.begin(),vSpade.end(),greater<BYTE>());
	sort(vHeart.begin(),vHeart.end(),greater<BYTE>());
	sort(vClub.begin(),vClub.end(),greater<BYTE>());
	sort(vDiamond.begin(),vDiamond.end(),greater<BYTE>());
}

//������еļ��ƣ�������С����
void CUpGradeGameLogic::GetGameLevelCard(BYTE byLedCardColor,BYTE byCardLevel,vector<BYTE> &vSpade,vector<BYTE> &vHeart,vector<BYTE> &vClub,vector<BYTE> &vDiamond,vector<BYTE>& vOuputVector,BYTE byDeskStation)
{
	BYTE GameLevelCard = (byLedCardColor | byCardLevel);	//������
	vOuputVector.clear();
	vector<BYTE> vGameLevelCard;		//��������
	vector<BYTE> vNoColorLevelCard;		//��������
	vGameLevelCard.clear();
	vNoColorLevelCard.clear();

	if(byLedCardColor == 0x30)		//��
	{
		#pragma region spade
		//���������
		for (vector<BYTE>::iterator it = vSpade.begin();it != vSpade.end();)
		{
			if(*it == GameLevelCard)
			{
				vGameLevelCard.push_back(*it);
				it = vSpade.erase(it);
			}
			else
			{
				++it;
			}
		}
		//��ø�����
		for (vector<BYTE>::iterator it = vHeart.begin();it != vHeart.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vHeart.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vClub.begin();it != vClub.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vClub.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vDiamond.begin();it != vDiamond.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vDiamond.erase(it);
			}
			else
			{
				++it;
			}
		}
		#pragma endregion spade
	}
	else if(byLedCardColor == 0x20)	//��
	{
		#pragma region heart
		//���������
		for (vector<BYTE>::iterator it = vHeart.begin();it != vHeart.end();)
		{
			if(*it == GameLevelCard)
			{
				vGameLevelCard.push_back(*it);
				it = vHeart.erase(it);
			}
			else
			{
				++it;
			}
		}
		//��ø�����
		for (vector<BYTE>::iterator it = vSpade.begin();it != vSpade.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vSpade.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vClub.begin();it != vClub.end();)
		{
			if((*it&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vClub.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vDiamond.begin();it != vDiamond.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vDiamond.erase(it);
			}
			else
			{
				++it;
			}
		}
		#pragma endregion heart
	}
	else if(byLedCardColor == 0x10)	//÷
	{
		#pragma region club

		//���������
		for (vector<BYTE>::iterator it = vClub.begin();it != vClub.end();)
		{
			if(*it == GameLevelCard)
			{
				vGameLevelCard.push_back(*it);
				it = vClub.erase(it);
			}
			else
			{
				++it;
			}
		}
		//��ø�����
		for (vector<BYTE>::iterator it = vSpade.begin();it != vSpade.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vSpade.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vHeart.begin();it != vHeart.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vHeart.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vDiamond.begin();it != vDiamond.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vDiamond.erase(it);
			}
			else
			{
				++it;
			}
		}
		#pragma endregion club
	}
	else if (byLedCardColor == 0x00 )
	{
		#pragma region diamond
		//���������
		for (vector<BYTE>::iterator it = vDiamond.begin();it != vDiamond.end();)
		{
			if(*it == GameLevelCard)
			{
				vGameLevelCard.push_back(*it);
				it = vDiamond.erase(it);
			}
			else
			{
				++it;
			}
		}
		//��ø�����
		for (vector<BYTE>::iterator it = vSpade.begin();it != vSpade.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vSpade.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vHeart.begin();it != vHeart.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vHeart.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vClub.begin();it != vClub.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vClub.erase(it);
			}
			else
			{
				++it;
			}
		}
#pragma endregion diamond
	}
	else if(byLedCardColor == 255)
	{
	#pragma region ����
		for (vector<BYTE>::iterator it = vSpade.begin();it != vSpade.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vSpade.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vHeart.begin();it != vHeart.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vHeart.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vClub.begin();it != vClub.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vClub.erase(it);
			}
			else
			{
				++it;
			}
		}
		for (vector<BYTE>::iterator it = vDiamond.begin();it != vDiamond.end();)
		{
			if(((*it)&0x0F) == byCardLevel)
			{
				vNoColorLevelCard.push_back(*it);
				it = vDiamond.erase(it);
			}
			else
			{
				++it;
			}
		}
	#pragma endregion ����
	}
	//�������ƣ��͸������ɴ�С����
	for (int i = 0;i< vGameLevelCard.size();++i)
	{
		vOuputVector.push_back(vGameLevelCard[i]);
	}
	for (int i =0;i< vNoColorLevelCard.size();++i)
	{
		vOuputVector.push_back(vNoColorLevelCard[i]);
	}

	CString cs;
	for (int i =0;i<vOuputVector.size();++i)
	{
		cs.Format("chs [GetGameLevelCard]-[��� %d]: %x,byCardLevel=%x",byDeskStation,vOuputVector[i],byCardLevel);
		OutputDebugString(cs);
	}
}
//����������Ƿ���ȷ
bool CUpGradeGameLogic::TestData(int iData,int iStyle)
{
	if(0==iStyle)//��֤�����λ���Ƿ���Ч
	{
		if(iData>=0 && iData<PLAY_COUNT)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(1==iStyle)//��֤�������Ƿ���Ч
	{
		if(iData>0 && iData<80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
//�õ���һ������λ��
BYTE CUpGradeGameLogic::GetNextDeskStation(BYTE byDeskStation)
{
	BYTE byNextDestStation=((++byDeskStation)%PLAY_COUNT);
	return byNextDestStation;
}
//��Ȧ����
BYTE CUpGradeGameLogic::SinglePK(TFirstData ptcsOutCard[],BYTE byRoundFirstOutCard,int& iRoundScore)
{
	for (int i = 0;i<PLAY_COUNT;++i)
	{
		BYTE byScore[25];
		memset(byScore,255,sizeof(byScore));
		int iScoreNums=0;
		for (int j = 0;j<ptcsOutCard[i].iCardNums;++j)
		{
			byScore[iScoreNums++]=ptcsOutCard[i].byCards[j];
		}
		iRoundScore+=GetPoint(byScore,iScoreNums);
	}
	int cards = ptcsOutCard[byRoundFirstOutCard].cardShape;
	if(E_CS_MIXED==cards)
	{
		return byRoundFirstOutCard;
	}
	else if(cards == E_CS_SINGLE)
	{
		bool bMajor=false;
		BYTE byNextStation=0;
		BYTE byList[4];
		memset(byList,0,sizeof(byList));
		BYTE byMajorList[4];//����
		memset(byMajorList,0,sizeof(byMajorList));
		int iTemp=0;
		int iMajorNums=0;
		for (int i = 0;i<PLAY_COUNT;++i)
		{
			byList[iTemp++]=ptcsOutCard[i].byCards[0];
			if(IsMajorCard(ptcsOutCard[i].byCards[0]))
			{
				bMajor=true;
			}
		}
		BYTE byMax = 255;
		if(!bMajor)
		{
			vector<BYTE> vSameFlower;
			vSameFlower.clear();
			for (int i = 0;i<iTemp;i++)
			{
				if(ptcsOutCard[byRoundFirstOutCard].byFlower==GetCardHuaKind(byList[i]))
				{
					vSameFlower.push_back(byList[i]);
				}
			}
			//����
			sort(vSameFlower.begin(),vSameFlower.end(),greater<BYTE>());
			byMax=vSameFlower.at(0);
		}
		else
		{
			byNextStation=byRoundFirstOutCard;
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				if(IsMajorCard(ptcsOutCard[byNextStation].byCards[0]))
				{
					byMajorList[iMajorNums++]=ptcsOutCard[byNextStation].byCards[0];
				}
				byNextStation=GetNextDeskStation(byNextStation);
			}
			byMax=byMajorList[0];
			for (int i = 1;i<iMajorNums;i++)
			{
				if(1==CompareCard(byMajorList[i],byMax))
				{
					byMax=byMajorList[i];
				}
			}
			//���ƱȽϴ�С
			//SortCard(byList,iTemp);
			//byMax=byList[0];
		}
		byNextStation=byRoundFirstOutCard;
		//�ҵ�������Ƶ����λ��
		for (int i = 0;i<PLAY_COUNT;++i)
		{
			if(byMax == ptcsOutCard[byNextStation].byCards[0])
			{
				return byNextStation;
			}
			byNextStation=GetNextDeskStation(byNextStation);
		}
	}
	else if(cards == E_CS_DOUBLE)
	{
		bool bMajor=false;
		BYTE byList[33];
		memset(byList,0,sizeof(byList));
		int iTemp=0;
		for (int i=0;i<PLAY_COUNT;++i)
		{
			if (ptcsOutCard[i].cardShape == E_CS_DOUBLE )
			{
				if(IsMajorCard(ptcsOutCard[i].byCards[0]))
				{
					bMajor=true;
				}
				byList[iTemp++]=ptcsOutCard[i].byCards[0];
				byList[iTemp++]=ptcsOutCard[i].byCards[1];
				//vtemp.push_back(ptcsOutCard[i].byCards[0]);
			}
		}
		BYTE byMax = 255;
		if(!bMajor)//ͬ��ɫ�бȴ�С
		{
			vector<BYTE> vSameFlower;
			vSameFlower.clear();
			for (int i = 0;i<iTemp;i++)
			{
				if(ptcsOutCard[byRoundFirstOutCard].byFlower==GetCardHuaKind(byList[i]))
				{
					vSameFlower.push_back(byList[i]);
				}
			}
			//����
			sort(vSameFlower.begin(),vSameFlower.end(),greater<BYTE>());
			byMax=vSameFlower.at(0);
		}
		else
		{
			SortCard(byList,iTemp);
			byMax=byList[0];
		}
		
		BYTE byNextStation=byRoundFirstOutCard;
		//�ҵ�������Ƶ����λ��
		for (int i = 0;i<PLAY_COUNT;++i)
		{
			if(byMax == ptcsOutCard[byNextStation].byCards[0])
			{
				return byNextStation;
			}
			byNextStation=GetNextDeskStation(byNextStation);
		}
	}
	else if(cards == E_CS_SERIALDOUBLE)//������
	{
		bool bMajor=false;
		BYTE byMainSerial[PLAY_COUNT][25];//����
		int iMainSerialNums[PLAY_COUNT];
		BYTE bySerial[PLAY_COUNT][25];//����
		int iSerialNums[PLAY_COUNT];
		memset(byMainSerial,255,sizeof(byMainSerial));
		memset(bySerial,255,sizeof(bySerial));
		memset(iMainSerialNums,0,sizeof(iMainSerialNums));
		memset(iSerialNums,0,sizeof(iSerialNums));
		for (int i = 0;i<PLAY_COUNT;++i)
		{
			if(ptcsOutCard[i].cardShape ==E_CS_SERIALDOUBLE )
			{
				if(IsMajorCard(ptcsOutCard[i].byCards[0]))
				{
					bMajor=true;
					for (int index =0;index<ptcsOutCard[i].iCardNums;index++)
					{
						byMainSerial[i][iMainSerialNums[i]++]=ptcsOutCard[i].byCards[index];
					}
				}
				else
				{
					for (int index =0;index<ptcsOutCard[i].iCardNums;index++)
					{
						bySerial[i][iSerialNums[i]++]=ptcsOutCard[i].byCards[index];
					}
				}
			}
		}
		BYTE byMax = 255;
		BYTE byNextStation=255;
		BYTE byMaxStation=255;
		byNextStation=byRoundFirstOutCard;
		bool bFlag=false;
		if(bMajor)
		{
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				for (int index =0;index<iMainSerialNums[byNextStation];index++)
				{
					BYTE byMin=FindMinCard(byMainSerial[byNextStation],iMainSerialNums[byNextStation]);
					if(!bFlag)
					{
						bFlag=!bFlag;
						byMax=byMin;
						byMaxStation=byNextStation;
					}
					else
					{
						if(1==CompareCard(byMin,byMax))
						{
							byMaxStation=byNextStation;
						}
					}
				}
				byNextStation=GetNextDeskStation(byNextStation);
			}
			return byMaxStation;
		}
		else
		{
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				for (int index =0;index<iSerialNums[byNextStation];index++)
				{
					BYTE byMin=FindMinCard(bySerial[byNextStation],iSerialNums[byNextStation]);
					if(!bFlag)
					{
						bFlag=!bFlag;
						byMax=byMin;
						byMaxStation=byNextStation;
					}
					else
					{
						if(1==CompareCard(byMin,byMax))
						{
							byMaxStation=byNextStation;
						}
					}
				}
				byNextStation=GetNextDeskStation(byNextStation);
			}
			return byMaxStation;
		}
	}
}

//��ó��Ƶ����ͣ� iNums ������
int CUpGradeGameLogic::GetCardShape(BYTE byCardList[],int iCardCount)
{
	if(iCardCount == 1)
	{
		return E_CS_SINGLE;
	}
	else if(iCardCount == 2)
	{
		if(byCardList[0] == byCardList[1] && IsSameHuaKind(byCardList,iCardCount))
		{	
			return E_CS_DOUBLE;	
		}
		else
		{
			return E_CS_MIXED;
		}
	}
	else if(iCardCount >2)
	{
		if(IsSequence(byCardList,iCardCount,2) && IsSameHuaKind(byCardList,iCardCount))
		{
			return E_CS_SERIALDOUBLE;
		}
		else
		{
			return E_CS_MIXED;
		}
	}
}
//��ø���
void CUpGradeGameLogic::GetOtherCard(BYTE byCardList[],int iCardCount,BYTE byResultCard[],int &iResultCount)
{
	iResultCount=0;
	for(int i=0;i<iCardCount;i++)
	{
		if(!IsMajorCard(byCardList[i]))
		{
			byResultCard[iResultCount++]=byCardList[i];
		}
	}
}
//�������
void CUpGradeGameLogic::GetMajorCard(BYTE byCardList[],int iCardCount,vector<BYTE> &vMajorCard)
{
	for(int i=0;i<iCardCount;i++)
	{
		if(IsMajorCard(byCardList[i]))
		{
			vMajorCard.push_back(byCardList[i]);
		}
	}
}
//����Ƶȼ�(����>С��>������>������>����ɫ��>�������η������ȼ�5,4,3,2,1,0)
int CUpGradeGameLogic::GetCardLevel(BYTE byCard)
{
	if(!IsMajorCard(byCard))//����
	{
		return 0;
	}
	if(0x4F==byCard)//����
	{
		return 5;
	}
	else if(0x4E==byCard)//С��
	{
		return 4;
	}
	else if(m_byLevel==GetCardNum(byCard) && IsLegalFlower(m_byMajorFlower) && m_byMajorFlower==GetCardHuaKind(byCard))//������
	{
		return 3;
	}
	else if(m_byLevel==GetCardNum(byCard) && m_byMajorFlower!=GetCardHuaKind(byCard))//������
	{
		return 2;
	}
	else//����ɫ��
	{
		return 1;
	}
}
//�����ƱȽϴ�С
int CUpGradeGameLogic::CompareCard(BYTE byFirstCard,BYTE bySecondCard)
{
	int iFirstLevel=GetCardLevel(byFirstCard);
	int iSecondLevel=GetCardLevel(bySecondCard);
	int iFirstNum=GetCardNum(byFirstCard);
	int iSecondNum=GetCardNum(bySecondCard);

	if(0!=iFirstLevel && 0==iSecondLevel)
	{
		return 1;
	}
	if(0==iFirstLevel && 0!=iSecondLevel)
	{
		return -1;
	}
	if(0==iFirstLevel && 0==iSecondLevel)//���Ǹ�����ֵ��ľʹ�
	{
		if(iFirstNum>iSecondNum)
		{
			return 1;
		}
		else if(iFirstNum<iSecondNum)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	if(0!=iFirstLevel && 0!=iSecondLevel)//��������
	{
		//����>С��>������>������>����ɫ��
		//������ȼ�4,3,2,1,0
		if(iFirstLevel>iSecondLevel)
		{
			return 1;
		}
		else if(iFirstLevel<iSecondLevel)
		{
			return -1;
		}
		else
		{
			if(iFirstNum>iSecondNum)
			{
				return 1;
			}
			else if(iFirstNum<iSecondNum)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
	}
}
//˦��
bool CUpGradeGameLogic::ThrowCard(vector<BYTE> vHandleCard[],BYTE byCardList[],int iCardCount,BYTE byDeskStation,int &iThrowSthle)
{
	char lwlog[256];//˦��ʧ����־��¼
	bool bThrowMajor=false;//˦��
	bool bThrowSameFlower=false;//˦ͬһ��ɫ
	SortCard(byCardList,iCardCount);//��˦������
	int iMajorNums=0;//��������
	for(int i=0;i<iCardCount;i++)
	{
		if(IsMajorCard(byCardList[i]))
		{
			iMajorNums++;
		}
	}
	if(iCardCount==iMajorNums)//ȫ������
	{
		bThrowMajor=true;
		iThrowSthle=0;
	}
	if(IsSameHuaKind(byCardList,iCardCount))//�Ƿ�ͬһ��ɫ
	{
		bThrowSameFlower=true;
		iThrowSthle=1;
	}
	if(bThrowMajor)//˦��
	{
		//���������ǵ��Ż��߶���
		bool bSingle=false;
		if(byCardList[iCardCount-2]!=byCardList[iCardCount-1])
		{
			bSingle=true;
		}
		if(bSingle)//����(���������ҵ����д������ŵ��Ƶ��ƣ���˦��ʧ��Ĭ�ϳ���С��)
		{
			BYTE byMinSingle=byCardList[iCardCount-1];//��С����
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(byDeskStation==i)
				{
					continue;
				}
				BYTE byTempList[33];
				memset(byTempList,255,sizeof(byTempList));
				int iTempCount=0;
				for(int index=0;index<vHandleCard[i].size();index++)
				{
					byTempList[iTempCount++]=vHandleCard[i].at(index);
				}
				for(int index=0;index<iTempCount;index++)
				{
					if(1==GetSameCardNums(byTempList,iTempCount,byTempList[index]) && IsMajorCard(byTempList[index]) && -1==CompareCard(byMinSingle,byTempList[index]))
					{
						_stprintf(lwlog,"ʧ��::˦��::�������ǵ���::byMinSingle=%d byTempList[index]=%d",byMinSingle,byTempList[index]);
						Debuglog(lwlog);
						return false;
					}
				}
			}
			return true;
		}
		else//����
		{
			BYTE byMinDouble=255;//��С����
			BYTE byMinSingle=255;//��С����
			for(int i=iCardCount-1;i>=0;i--)
			{
				if(2==GetSameCardNums(byCardList,iCardCount,byCardList[i]))
				{
					byMinDouble=byCardList[i];
					break;
				}
			}

			for(int i=iCardCount-1;i>=0;i--)
			{
				if(1==GetSameCardNums(byCardList,iCardCount,byCardList[i]))
				{
					byMinSingle=byCardList[i];
					break;
				}
			}
			if(255!=byMinSingle)
			{
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(byDeskStation==i)
					{
						continue;
					}
					BYTE byTempList[33];
					memset(byTempList,255,sizeof(byTempList));
					int iTempCount=0;
					for(int index=0;index<vHandleCard[i].size();index++)
					{
						byTempList[iTempCount++]=vHandleCard[i].at(index);
					}
					for(int index=0;index<iTempCount;index++)
					{
						if(1==GetSameCardNums(byTempList,iTempCount,byTempList[index]) && IsMajorCard(byTempList[index]) && -1==CompareCard(byMinSingle,byTempList[index]))
						{
							_stprintf(lwlog,"ʧ��::˦��::�������Ƕ���::�����ж�::byMinSingle=%d byTempList[index]=%d",byMinSingle,byTempList[index]);
							Debuglog(lwlog);
							return false;
						}
					}
				}
			}
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(byDeskStation==i)
				{
					continue;
				}
				BYTE byTempList[33];
				memset(byTempList,255,sizeof(byTempList));
				int iTempCount=0;
				for(int index=0;index<vHandleCard[i].size();index++)
				{
					byTempList[iTempCount++]=vHandleCard[i].at(index);
				}
				for(int index=0;index<iTempCount;index++)
				{
					if(2==GetSameCardNums(byTempList,iTempCount,byTempList[index]) && IsMajorCard(byTempList[index]) && -1==CompareCard(byMinDouble,byTempList[index]))
					{
						_stprintf(lwlog,"ʧ��::˦��::�������Ƕ���::�����ж�::byMinDouble=%d byTempList[index]=%d",byMinDouble,byTempList[index]);
						Debuglog(lwlog);
						return false;
					}
				}
			}
			return true;
		}
	}
	
	if(bThrowSameFlower)//˦ͬһ��ɫ
	{
		BYTE byFlower=GetCardHuaKind(byCardList[0]);
		//���������ǵ��Ż��߶���
		bool bSingle=false;
		if(byCardList[iCardCount-2]!=byCardList[iCardCount-1])
		{
			bSingle=true;
		}
		if(bSingle)//����(���������ҵ����д������ŵ��Ƶ��ƣ���˦��ʧ��Ĭ�ϳ���С��)
		{
			OutputDebugString("lwlog::ThrowCard::˦ͬһ��ɫ::˦����");
			BYTE byMinSingle=byCardList[iCardCount-1];//��С����
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(byDeskStation==i)
				{
					continue;
				}
				BYTE byTempList[33];
				memset(byTempList,255,sizeof(byTempList));
				int iTempCount=0;
				for(int index=0;index<vHandleCard[i].size();index++)
				{
					byTempList[iTempCount++]=vHandleCard[i].at(index);
				}
				for(int index=0;index<iTempCount;index++)
				{
					if(1==GetSameCardNums(byTempList,iTempCount,byTempList[index]) && !IsMajorCard(byTempList[index])
						&& byFlower==GetCardHuaKind(byTempList[index]) && -1==CompareCard(byMinSingle,byTempList[index]))
					{
						_stprintf(lwlog,"ʧ��::˦ͬһ��ɫ::�������ǵ���::byMinSingle=%d byTempList[index]=%d",byMinSingle,byTempList[index]);
						Debuglog(lwlog);
						return false;
					}
				}
			}
			return true;
		}
		else//����
		{
			BYTE byMinDouble=255;//��С����
			BYTE byMinSingle=255;//��С����
			for(int i=iCardCount-1;i>=0;i--)
			{
				if(2==GetSameCardNums(byCardList,iCardCount,byCardList[i]))
				{
					byMinDouble=byCardList[i];
					break;
				}
			}

			for(int i=iCardCount-1;i>=0;i--)
			{
				if(1==GetSameCardNums(byCardList,iCardCount,byCardList[i]))
				{
					byMinSingle=byCardList[i];
					break;
				}
			}
			if(255!=byMinSingle)
			{
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(byDeskStation==i)
					{
						continue;
					}
					BYTE byTempList[33];
					memset(byTempList,255,sizeof(byTempList));
					int iTempCount=0;
					for(int index=0;index<vHandleCard[i].size();index++)
					{
						byTempList[iTempCount++]=vHandleCard[i].at(index);
					}
					for(int index=0;index<iTempCount;index++)
					{
						if(1==GetSameCardNums(byTempList,iTempCount,byTempList[index]) && !IsMajorCard(byTempList[index])
							&& byFlower==GetCardHuaKind(byTempList[index]) && -1==CompareCard(byMinSingle,byTempList[index]))
						{
							_stprintf(lwlog,"ʧ��::˦ͬһ��ɫ::�������Ƕ���::�����ж�::byMinSingle=%d byTempList[index]=%d",byMinSingle,byTempList[index]);
							Debuglog(lwlog);
							return false;
						}
					}
				}
			}
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(byDeskStation==i)
				{
					continue;
				}
				BYTE byTempList[33];
				memset(byTempList,255,sizeof(byTempList));
				int iTempCount=0;
				for(int index=0;index<vHandleCard[i].size();index++)
				{
					byTempList[iTempCount++]=vHandleCard[i].at(index);
				}
				for(int index=0;index<iTempCount;index++)
				{
					if(2==GetSameCardNums(byTempList,iTempCount,byTempList[index]) && !IsMajorCard(byTempList[index])
						&& byFlower==GetCardHuaKind(byTempList[index]) && byTempList[index]>byMinDouble)
					{
						_stprintf(lwlog,"ʧ��::˦ͬһ��ɫ::�������Ƕ���::�����ж�::byMinDouble=%d byTempList[index]=%d",byMinDouble,byTempList[index]);
						Debuglog(lwlog);
						return false;
					}
				}
			}
			return true;
		}
	}
	return false;
}
//������ͬ�Ƹ���
int CUpGradeGameLogic::GetSameCardNums(BYTE byCardList[],int iCardCount,BYTE byCard)
{
	int iNums=0;
	for(int i=0;i<iCardCount;i++)
	{
		if(byCard==byCardList[i])
		{
			iNums++;
		}
	}
	return iNums;
}
//�Ƿ�����
bool CUpGradeGameLogic::IsMajorCard(BYTE byCard)
{
	bool bMajor=false;
	if(IsLegalFlower(m_byMajorFlower) && m_byMajorFlower==GetCardHuaKind(byCard))//������ɫ��
	{
		bMajor=true;
	}
	else if(m_byLevel==GetCardNum(byCard) || 0x4E==byCard || 0x4F==byCard)//�Ǽ��ƻ��С��
	{
		bMajor=true;
	}
	return bMajor;
}
//���ƶ�������
int CUpGradeGameLogic::GetMajorDoubleNums(BYTE byCardList[],int iCardCount,vector<BYTE> &vMajorDoubleCard)
{
	int iDoubleNums=0;
	for(int i=0;i<iCardCount;i++)
	{
		bool bMajor=false;
		BYTE byFirstCard=byCardList[i];
		if(IsMajorCard(byFirstCard))
		{
			bMajor=true;
		}
		if(!bMajor)
		{
			continue;
		}
		
		for(int index=i+1;index<iCardCount;index++)
		{
			if(byFirstCard==byCardList[index])
			{
				vMajorDoubleCard.push_back(byFirstCard);
				vMajorDoubleCard.push_back(byCardList[index]);
				iDoubleNums++;
			}
		}
	}
	return iDoubleNums;
}
//��������������
void CUpGradeGameLogic::GetSerialNums(BYTE byCardList[],int iCardCount,int iLen,vector<BYTE> &vSerialCard)
{
	BYTE byDouble[33];//�洢����
	memset(byDouble,0,sizeof(byDouble));
	int iDoubleNums=0;
	for(int i=iCardCount-1;i>=0;i--)
	{
		if(2==GetCardNum(byCardList[i]) || 0x40==GetCardHuaKind(byCardList[i]))//2����С������
		{
			continue;
		}
		BYTE byDest=byCardList[i];
		for(int index=i-1;index>=0;index--)
		{
			if(byDest==byCardList[index])//��һ��
			{
				byDouble[iDoubleNums++]=byDest;
				byDouble[iDoubleNums++]=byCardList[index];
				break;
			}
		}
	}
	
	vector<BYTE> vTemp;
	vTemp.clear();
	for(int i=0;i<iDoubleNums;)
	{
		BYTE byDest=byDouble[i];
		bool bSuccess=true;
		int index=i+1;
		vTemp.clear();
		for(;index<(i+iLen*2);)
		{
			if(byDest!=byDouble[index])
			{
				bSuccess=false;
				break;
			}
			vTemp.push_back(byDouble[index]);
			vTemp.push_back(byDouble[index]);
			index+=2;
			byDest++;
		}
		if(bSuccess)//������һ��
		{
			for(int t=0;t<vTemp.size();t++)
			{
				vSerialCard.push_back(vTemp.at(t));
			}
			i=index-1;
		}
		else
		{
			i+=2;
		}
	}
}
int CUpGradeGameLogic::GetSameFlowerNums(BYTE byCardList[],int iCardCount,BYTE byFlower,vector<BYTE> &vSameFlowerCard)
{
	int iSameFlowerNums=0;
	for(int i=iCardCount-1;i>=0;i--)
	{
		if(m_byLevel!=GetCardNum(byCardList[i]) && byFlower==GetCardHuaKind(byCardList[i]))
		{
			vSameFlowerCard.push_back(byCardList[i]);
			iSameFlowerNums++;
		}
	}
	return iSameFlowerNums;
}
//�ж��Ƿ���Ƴɹ�	;������ͬ�ļ���ƵĻ�ɫ�� ���Ͳ�ͬ�ļ���Ƿ����㹻���׼ҵ��ƵĻ�ɫ
bool CUpGradeGameLogic::IsOutCardSuccess(TFirstData &tFirstPlayerCard,BYTE byCardList[],int iCardCount,int iShape,vector<BYTE> &vHandleCard)
{
	//����������һ��
	if(tFirstPlayerCard.iCardNums !=iCardCount)
	{
		return false;
	}
	BYTE byHandleList[33];//����
	memset(byHandleList,0,sizeof(byHandleList));
	int iHandleNums=0;//������
	for(int i=0;i<vHandleCard.size();i++)
	{
		byHandleList[i]=vHandleCard.at(i);
		iHandleNums++;
	}
	vector<BYTE> vMajorCard;
	vMajorCard.clear();
	GetMajorCard(byHandleList,iHandleNums,vMajorCard);
	char szMes[256];
	_stprintf(szMes,"IsOutCardSuccess::tFirstPlayerCard.cardShape=%d",tFirstPlayerCard.cardShape);
	Debuglog(szMes);
	if(1==tFirstPlayerCard.cardShape)//����
	{
		if(IsMajorCard(tFirstPlayerCard.byCards[0]))//����
		{
			_stprintf(szMes,"����");
			Debuglog(szMes);
			if(0==vMajorCard.size())//û����������
			{
				return true;
			}
			if(IsMajorCard(byCardList[0]))
			{
				return true;
			}
			return false;
		}
		if(tFirstPlayerCard.byFlower!=GetCardHuaKind(byCardList[0]))//��ɫ��һ��
		{
			_stprintf(szMes,"��ɫ��һ��");
			Debuglog(szMes);
			for(int i=0;i<vHandleCard.size();i++)
			{
				if(m_byLevel!=GetCardNum(vHandleCard.at(i)) && tFirstPlayerCard.byFlower==GetCardHuaKind(vHandleCard.at(i)))
				{
					_stprintf(szMes,"��ɫ��һ��::return false::vHandleCard[%d]=%d",i,vHandleCard.at(i));
					Debuglog(szMes);
					return false;
				}
			}
		}
	}
	else if(2==tFirstPlayerCard.cardShape)//����
	{
		if(IsMajorCard(tFirstPlayerCard.byCards[0]))//����
		{
			CString cs;
			cs.Format("lwlog::���ӵ���");
			OutputDebugString(cs);
			if(0==vMajorCard.size())//û����������
			{
				return true;
			}
			bool bMajorDouble=false;
			vector<BYTE> vMajorDoubleCard;
			vMajorDoubleCard.clear();
			if(GetMajorDoubleNums(byHandleList,iHandleNums,vMajorDoubleCard)>0)//�����ƶ��ӱ�������ƶ���
			{
				for(int i=0;i<vMajorDoubleCard.size();i++)
				{
					cs.Format("lwlog::�����ƶ���vMajorDoubleCard[%d]=%d",i,vMajorDoubleCard.at(i));
					OutputDebugString(cs);
				}
				if(byCardList[0] == byCardList[1] && IsSameHuaKind(byCardList,iCardCount))//�Ƕ���
				{	
					if(IsMajorCard(byCardList[0]))
					{
						return true;
					}
					return false;
				}
				else
				{
					return false;
				}
			}
			else
			{
				cs.Format("lwlog::û�����ƶ���vMajorCard.size()=%d",vMajorCard.size());
				OutputDebugString(cs);
				if(vMajorCard.size()==1)//һ�����ƣ��������ȥ
				{
					for(int i=0;i<iCardCount;i++)
					{
						if(IsMajorCard(byCardList[i]))
						{
							cs.Format("lwlog::û�����ƶ���return false;000000");
							OutputDebugString(cs);
							return true;
						}
					}
					return false;
				}
				if(vMajorCard.size()>1)//�����������ƣ������Ʊ���ȫ������
				{
					for(int i=0;i<iCardCount;i++)
					{
						if(!IsMajorCard(byCardList[i]))
						{
							cs.Format("lwlog::û�����ƶ���return false;1111111111");
							OutputDebugString(cs);
							return false;
						}
					}
					return true;
				}
			}
		}
		CString cs;
		bool bSameFlower=false;
		for(int i=0;i<vHandleCard.size();i++)
		{
			if(m_byLevel!=GetCardNum(vHandleCard.at(i)) && tFirstPlayerCard.byFlower==GetCardHuaKind(vHandleCard.at(i)))
			{
				for(int index=i+1;index<vHandleCard.size();index++)
				{
					if(vHandleCard.at(i)==vHandleCard.at(index))
					{
						_stprintf(szMes,"������ͬ��ɫ�Ķ���::=%d::%d::%d",tFirstPlayerCard.byFlower,vHandleCard.at(i),vHandleCard.at(index));
						Debuglog(szMes);
						bSameFlower=true;//������ͬ��ɫ�Ķ���
						break;
					}
				}
			}
		}
		if(bSameFlower)//�����ͬ��ɫ�Ķ���
		{
			cs.Format("lwlog::�����ͬ��ɫ�Ķ���");
			OutputDebugString(cs);
			if(byCardList[1]!=byCardList[0] || tFirstPlayerCard.byFlower!=GetCardHuaKind(byCardList[0]))//��ɫ��һ��
			{
				cs.Format("lwlog::�����ͬ��ɫ�Ķ���return false000000");
				OutputDebugString(cs);
				return false;
			}
		}
		else
		{
			cs.Format("lwlog::û��ͬ��ɫ�Ķ���");
			OutputDebugString(cs);
			vector<BYTE> vHandleSameFlowerCard;
			vHandleSameFlowerCard.clear();
			vector<BYTE> vOutSameFlowerCard;
			vOutSameFlowerCard.clear();
			GetSameFlowerNums(byHandleList,iHandleNums,tFirstPlayerCard.byFlower,vHandleSameFlowerCard);//����ͬ��ɫ������
			GetSameFlowerNums(byCardList,iCardCount,tFirstPlayerCard.byFlower,vOutSameFlowerCard);//����ͬ��ɫ������
			if(0==vHandleSameFlowerCard.size())
			{
				return true;
			}
			else if(vHandleSameFlowerCard.size()>=iCardCount)
			{
				if(iCardCount==vOutSameFlowerCard.size())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if(vHandleSameFlowerCard.size()<iCardCount)
			{
				if(vHandleSameFlowerCard.size()==vOutSameFlowerCard.size())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	else if(3==tFirstPlayerCard.cardShape)//������
	{
		int iLen=tFirstPlayerCard.iCardNums/2;
		//���ǵ����������ƣ��¼������Ʊ�������ƣ�������>ɢ��
		//�Ǹ���,�м���ͬ��ɫ�ı��������ͬ��ɫ�ģ�Ȼ����ɢ�ƴ���
		vector<BYTE> vSerialCard;
		vSerialCard.clear();
		SortCard(byHandleList,iHandleNums);
		GetSerialNums(byHandleList,iHandleNums,tFirstPlayerCard.iCardNums/2,vSerialCard);
		if(vSerialCard.size()>0)//��������
		{
			if(IsMajorCard(tFirstPlayerCard.byCards[0]))//����
			{
				bool bSerial=false;
				for(int i=0;i<vSerialCard.size();i+=tFirstPlayerCard.iCardNums)
				{
					if(IsMajorCard(vSerialCard.at(i)))//����������
					{
						bSerial=true;
						break;
					}
				}
				if(bSerial)//���������������
				{
					if(3==iShape && IsMajorCard(byCardList[0]))//����������
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					if(0==vMajorCard.size())//û����������
					{
						return true;
					}
					else if(vMajorCard.size()>0)
					{
						vector<BYTE> vOutMajorCard;
						vOutMajorCard.clear();
						GetMajorCard(byCardList,iCardCount,vOutMajorCard);
						if(vMajorCard.size()>=iCardCount)
						{
							if(iCardCount==vOutMajorCard.size())//�������ȫ��������
							{
								return true;
							}
							else
							{
								return false;
							}
						}
						else//���������ȫ����ȥ
						{
							if(vMajorCard.size()==vOutMajorCard.size())
							{
								return true;
							}
							else
							{
								return false;
							}
						}
					}
				}
			}
			//��һ�ҳ��ĸ���������
			//ͬ��ɫ�Ƴ���
			bool bSameFlower=false;//����ͬ��ɫ������
			for(int i=0;i<vSerialCard.size();i+=tFirstPlayerCard.iCardNums)
			{
				if(tFirstPlayerCard.byFlower==GetCardHuaKind(vSerialCard.at(i)))
				{
					bSameFlower=true;
					break;
				}
			}
			if(bSameFlower)//�����ͬ��ɫ������
			{
				if(3==iShape && tFirstPlayerCard.byFlower==GetCardHuaKind(byCardList[0]))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else//���뽫ͬ��ɫ�Ƴ���
			{
				vector<BYTE> vHandleSameFlowerCard;
				vHandleSameFlowerCard.clear();
				vector<BYTE> vOutSameFlowerCard;
				vOutSameFlowerCard.clear();
				GetSameFlowerNums(byHandleList,iHandleNums,tFirstPlayerCard.byFlower,vHandleSameFlowerCard);
				GetSameFlowerNums(byCardList,iCardCount,tFirstPlayerCard.byFlower,vOutSameFlowerCard);
				if(vHandleSameFlowerCard.size()<=0)
				{
					return true;
				}
				else if(vHandleSameFlowerCard.size()<iCardCount)
				{
					if(vHandleSameFlowerCard.size()==vOutSameFlowerCard.size())
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if(vHandleSameFlowerCard.size()>=iCardCount)
				{
					if(iCardCount==vOutSameFlowerCard.size())
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}
		else
		{
			vector<BYTE> vHandleSameFlowerCard;
			vHandleSameFlowerCard.clear();
			vector<BYTE> vOutSameFlowerCard;
			vOutSameFlowerCard.clear();
			GetSameFlowerNums(byHandleList,iHandleNums,tFirstPlayerCard.byFlower,vHandleSameFlowerCard);
			GetSameFlowerNums(byCardList,iCardCount,tFirstPlayerCard.byFlower,vOutSameFlowerCard);
			if(vHandleSameFlowerCard.size()<=0)
			{
				return true;
			}
			else if(vHandleSameFlowerCard.size()<iCardCount)
			{
				if(vHandleSameFlowerCard.size()==vOutSameFlowerCard.size())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if(vHandleSameFlowerCard.size()>=iCardCount)
			{
				if(iCardCount==vOutSameFlowerCard.size())//�������ȫ��������
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	if(4==tFirstPlayerCard.cardShape)//˦��
	{
		if(0==tFirstPlayerCard.iThrowStyle)//˦����
		{
			int iOutMajorNums=0;//������������
			for(int i=0;i<iCardCount;i++)
			{
				if(IsMajorCard(byCardList[i]))
				{
					iOutMajorNums++;
				}
			}
			if(tFirstPlayerCard.iCardNums>=vMajorCard.size())//���������ȫ�����
			{
				if(vMajorCard.size()==iOutMajorNums)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if(tFirstPlayerCard.iCardNums==iOutMajorNums)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		if(1==tFirstPlayerCard.iThrowStyle)//˦ͬһ��ɫ��
		{
			vector<BYTE> vHandleSameFlowerCard;
			vHandleSameFlowerCard.clear();
			vector<BYTE> vOutSameFlowerCard;
			vOutSameFlowerCard.clear();
			GetSameFlowerNums(byHandleList,iHandleNums,GetCardHuaKind(tFirstPlayerCard.byCards[0]),vHandleSameFlowerCard);//����ͬ��ɫ������
			GetSameFlowerNums(byCardList,iCardCount,GetCardHuaKind(tFirstPlayerCard.byCards[0]),vOutSameFlowerCard);//����ͬ��ɫ������
			if(tFirstPlayerCard.iCardNums>=vHandleSameFlowerCard.size())//�����ͬ��ɫ��ȫ�����
			{
				if(vHandleSameFlowerCard.size()==vOutSameFlowerCard.size())
				{
					return true;
				}
				else 
				{
					_stprintf(szMes,"�����ͬ��ɫ��ȫ�����::%d :: %d",vHandleSameFlowerCard.size(),vOutSameFlowerCard.size());
					Debuglog(szMes);
					return false;
				}
			}
			else
			{
				if(tFirstPlayerCard.iCardNums==vOutSameFlowerCard.size())
				{
					return true;
				}
				else 
				{
					_stprintf(szMes,"ssss::%d :: %d",tFirstPlayerCard.iCardNums,vOutSameFlowerCard.size());
					Debuglog(szMes);
					return false;
				}
			}
		}
	}
	return true;
}
//ϴ�� 
///�����˿�,������ʹ��
///@param BYTE iCard[] [out] Ҫ�õ��Ļ����˿�����
///@param int iCardCount [out] �˿˵�����
///@return ��������TRUE
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,bool bHaveKing)
{
	static const BYTE m_CardArray[54] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
		0x4E, 0x4F //С�����
	};

	BYTE iSend=0,iStation=0,iCardList[216],step=(bHaveKing?54:52);
	srand((unsigned)time(NULL));

	for (int i=0;i<iCardCount;i+=step)
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
//�����Ƹ���
int CUpGradeGameLogic::GetCardNums(BYTE byCardList[], int iCardCount)
{
	int iNums=0;
	for(int i =0;i<iCardCount;i++)
	{
		if(byCardList[i]>=0x01 && byCardList[i]<=0x4F)
		{
			iNums++;
		}
	}
	return iNums;
}
//������ɾ��������
/*@ptOutCards��Ҫɾ������; @vCardsInHand:���е���*/
void CUpGradeGameLogic::ReMoveOutCards(BYTE byCardList[],int iCardCount,vector<BYTE> &vCardsInHand)
{
	for(int i =0;i<iCardCount;++i)
	{
		for (int j =0;j<vCardsInHand.size();++j)
		{
			if(byCardList[i] == vCardsInHand[j])
			{
				vCardsInHand[j] = 255;
				break;
			}
		}
	}
	vector<BYTE> temp_cards;
	temp_cards.clear();
	for (vector<BYTE>::iterator it = vCardsInHand.begin(); it != vCardsInHand.end();++it)
	{
		if(*it != 255)
		{
			temp_cards.push_back(*it);
		}
	}
	vCardsInHand.clear();
	for (vector<BYTE>::iterator it = temp_cards.begin(); it != temp_cards.end();++it)
	{
		vCardsInHand.push_back(*it);
	}
}
//������ʾ��ȡĳ��ɫ����
bool CUpGradeGameLogic::GetTipDoubleCardForSameFlower(BYTE byCardList[],int iCardCount,BYTE byColor,BYTE byNewList[], int &iNewCount)
{
	iNewCount=0;
	for(int i=iCardCount-1;i>=0;i--)
	{
		int iCardNum=GetCardNum(byCardList[i]);
		int iCardFlower=GetCardHuaKind(byCardList[i]);
		if(byColor!=iCardFlower || m_byLevel==iCardNum)//�Ǽ���
		{
			continue;
		}
		for(int index=i-1;index>=0;index--)
		{
			if(byCardList[i]==byCardList[index])
			{
				byNewList[iNewCount++]=byCardList[i];
				byNewList[iNewCount++]=byCardList[index];
				return true;
			}
		}
	}
	return false;
}
//�������
/*
@byLevelColor:���ƻ�ɫ; @byLevelCard:���Ƶ����� @vCardsInHand:���е��ƣ�@vMainCard�����е����ƣ��������
*/
void CUpGradeGameLogic::GetMainCard(BYTE byLevelColor,BYTE byLevelCard,vector<BYTE>& vCardsInHand,vector<BYTE>&vMainCard)
{
	vMainCard.clear();	//�������

	for (vector<BYTE>::iterator it = vCardsInHand.begin(); it != vCardsInHand.end(); ++it)
	{
		if( ( ((*it)&0xF0) == 0x40) || (((*it)&0xF0) == byLevelColor) || ( ((*it)&0x0F) == byLevelCard) )
		{
			vMainCard.push_back(*it);
		}
	}
}
//�Ƿ�Ϊͬһ��ɫ
bool CUpGradeGameLogic::IsSameHuaKind(BYTE iCardList[], int iCardCount)
{
	if(iCardCount<1)
	{
		return false;
	}
	BYTE byFirstHua=GetCardHuaKind(iCardList[0]);
	for(int i=0;i<iCardCount;i++)
	{
		if(byFirstHua!=GetCardHuaKind(iCardList[i]))
		{
			return false;
		}
	}
	return true;
}
//������С��
BYTE CUpGradeGameLogic::FindMinCard(BYTE byCardList[], int iCardCount)
{
	bool bFlag=true;
	BYTE byMinCard=0;
	for(int i= 0;i < iCardCount;i++)
	{
		if(IsLegalCard(byCardList[i]) && m_byLevel!=GetCardNum(byCardList[i]))
		{
			if(bFlag)
			{
				bFlag=!bFlag;
				byMinCard=byCardList[i];
			}
			else
			{
				byMinCard=(GetCardNum(byMinCard)>GetCardNum(byCardList[i]))?byCardList[i]:byMinCard;
			}
		}
	}
	return byMinCard;
}
//�Ƿ�����
BOOL CUpGradeGameLogic::IsSequence(BYTE iCardList[], int iCardCount, int iCount)
{
	if(iCardCount<4)
	{
		return false;
	}
	BYTE temp[18];
	memset(temp,0,sizeof(temp));
	for(int i= 0;i < iCardCount;i++)
	{
		temp[GetCardNum(iCardList[i])]++;
	}
	for(int i = 0; i < 18 ; i ++)
	{
		if(temp [i]!= 0 &&temp[i] !=iCount)	//��ָ��˳
		{
			return false;
		}
	}
	int len = iCardCount / iCount;
	for(int i=0;i<18;i++)
	{
		if(temp[i] != 0)//��ֵ
		{
			for(int j = i;j < i + len  ;j ++)
			{
				if(temp[j] != iCount || j >=15 )
					return false;
			}
			return true;
		}
	}
	return false;
}
//�йܳ���
void CUpGradeGameLogic::AutoOutCard(TFirstData &tFirstPlayerCard,BYTE byCardList[],int iCardCount,int iRoundOutCardCount,T_CS_OutCard &tOutCard)
{
	char szMes[256];
	_stprintf(szMes,"iRoundOutCardCount=%d",iRoundOutCardCount);
	Debuglog(szMes);
	vector<BYTE> vMajorCard;
	vMajorCard.clear();
	GetMajorCard(byCardList,iCardCount,vMajorCard);
	if(0==iRoundOutCardCount)//��һ������
	{
		if(byCardList[iCardCount-2]==byCardList[iCardCount-1])
		{
			tOutCard.byCardNums=2;
			tOutCard.byCards[0]=byCardList[iCardCount-1];
			tOutCard.byCards[1]=byCardList[iCardCount-2];
			return;
		}
		else
		{
			tOutCard.byCardNums=1;
			tOutCard.byCards[0]=byCardList[iCardCount-1];
			return;
		}
	}
	else
	{
		_stprintf(szMes,"tFirstPlayerCard.cardShape=%d",tFirstPlayerCard.cardShape);
		Debuglog(szMes);
		if(1==tFirstPlayerCard.cardShape)//����
		{
			if(IsMajorCard(tFirstPlayerCard.byCards[0]))//����
			{
				if(0==vMajorCard.size())//û������
				{
					tOutCard.byCardNums=1;
					tOutCard.byCards[0]=byCardList[iCardCount-1];
					return;
				}
				for(int i=iCardCount-1;i>=0;i--)
				{
					//������
					if(IsMajorCard(byCardList[i]))
					{
						tOutCard.byCardNums=1;
						tOutCard.byCards[0]=byCardList[i];
						return;
					}
				}
			}
			vector<BYTE> vSameFlowerCard;
			vSameFlowerCard.clear();
			GetSameFlowerNums(byCardList,iCardCount,tFirstPlayerCard.byFlower,vSameFlowerCard);
			if(vSameFlowerCard.size()>0)//��ͬ��ɫ��
			{
				BYTE byTemp[33];
				memset(byTemp,0,sizeof(byTemp));
				int iNums=0;
				for(int i=0;i<vSameFlowerCard.size();i++)
				{
					byTemp[iNums++]=vSameFlowerCard.at(i);
				}
				SortCard(byTemp,iNums);
				tOutCard.byCardNums=1;
				tOutCard.byCards[0]=byTemp[iNums-1];
				return;
			}
			else//��ͬ��ɫ��
			{
				tOutCard.byCardNums=1;
				tOutCard.byCards[0]=byCardList[iCardCount-1];
				return;
			}
		}
		else if(2==tFirstPlayerCard.cardShape)//����
		{
			if(IsMajorCard(tFirstPlayerCard.byCards[0]))//����
			{
				if(0==vMajorCard.size())//û������
				{
					tOutCard.byCardNums=2;
					tOutCard.byCards[0]=byCardList[iCardCount-1];
					tOutCard.byCards[1]=byCardList[iCardCount-2];
					return;
				}
				vector<BYTE> vMajorDoubleCard;
				vMajorDoubleCard.clear();
				if(GetMajorDoubleNums(byCardList,iCardCount,vMajorDoubleCard)>0)//�����ƶ��ӱ�������ƶ���
				{
					BYTE byTemp[33];
					memset(byTemp,0,sizeof(byTemp));
					int iNums=0;
					for(int i=0;i<vMajorDoubleCard.size();i++)
					{
						byTemp[iNums++]=vMajorDoubleCard.at(i);
					}
					SortCard(byTemp,iNums);
					tOutCard.byCardNums=2;
					tOutCard.byCards[0]=byTemp[iNums-1];
					tOutCard.byCards[1]=byTemp[iNums-2];
					return;
				}
				else
				{
					if(1==vMajorCard.size())//һ�����ƣ��������ȥ
					{
						BYTE byMajorCard=0;
						for(int i=iCardCount-1;i>=0;i--)
						{
							if(vMajorCard.at(0)!=byCardList[i])
							{
								byMajorCard=byCardList[i];
								break;
							}
						}
						tOutCard.byCardNums=2;
						tOutCard.byCards[0]=byMajorCard;
						tOutCard.byCards[1]=vMajorCard.at(0);
						return;
					}
					if(vMajorCard.size()>1)//�����������ƣ������Ʊ���ȫ������
					{
						BYTE byTemp[33];
						memset(byTemp,0,sizeof(byTemp));
						int iNums=0;
						for(int i=0;i<vMajorCard.size();i++)
						{
							byTemp[iNums++]=vMajorCard.at(i);
						}
						SortCard(byTemp,iNums);
						tOutCard.byCardNums=2;
						tOutCard.byCards[0]=byTemp[iNums-1];
						tOutCard.byCards[1]=byTemp[iNums-2];
						return;
					}
				}
			}
			BYTE byNewList[2]={0,0};
			int iNewCount=0;
			if(GetTipDoubleCardForSameFlower(byCardList,iCardCount,tFirstPlayerCard.byFlower,byNewList,iNewCount))//ͬ��ɫ�Ķ���
			{
				_stprintf(szMes,"ͬ��ɫ�Ķ���iNewCount=%d",iNewCount);
				Debuglog(szMes);
				tOutCard.byCardNums=iNewCount;
				memcpy(tOutCard.byCards,byNewList,sizeof(BYTE)*iNewCount);
				return;
			}
			else
			{
				vector<BYTE> vHandleSameFlowerCard;
				vHandleSameFlowerCard.clear();
				GetSameFlowerNums(byCardList,iCardCount,tFirstPlayerCard.byFlower,vHandleSameFlowerCard);//����ͬ��ɫ������
				if(0==vHandleSameFlowerCard.size())//����
				{
					_stprintf(szMes,"����");
					Debuglog(szMes);
					tOutCard.byCardNums=2;
					tOutCard.byCards[0]=byCardList[iCardCount-1];
					tOutCard.byCards[1]=byCardList[iCardCount-2];
					return;
				}
				else if(vHandleSameFlowerCard.size()>=tFirstPlayerCard.iCardNums)
				{
					_stprintf(szMes,"vHandleSameFlowerCard.size()>=tFirstPlayerCard.iCardNums");
					Debuglog(szMes);
					tOutCard.byCardNums=2;
					tOutCard.byCards[0]=vHandleSameFlowerCard.at(0);
					tOutCard.byCards[1]=vHandleSameFlowerCard.at(1);
					return;
				}
				else if(vHandleSameFlowerCard.size()<tFirstPlayerCard.iCardNums)
				{
					_stprintf(szMes,"vHandleSameFlowerCard.size()<tFirstPlayerCard.iCardNums");
					Debuglog(szMes);
					tOutCard.byCardNums=2;
					tOutCard.byCards[0]=vHandleSameFlowerCard.at(0);
					for(int index=iCardCount-1;index>=0;index--)
					{
						if(vHandleSameFlowerCard.at(0)!=byCardList[index])
						{
							tOutCard.byCards[1]=byCardList[index];
							break;
						}
					}
					return;
				}
			}
		}
		else if(3==tFirstPlayerCard.cardShape)
		{
			int iLen=tFirstPlayerCard.iCardNums/2;
			//���ǵ����������ƣ��¼������Ʊ�������ƣ�������>ɢ��
			//�Ǹ���,�м���ͬ��ɫ�ı��������ͬ��ɫ�ģ�Ȼ����ɢ�ƴ���
			vector<BYTE> vSerialCard;
			vSerialCard.clear();
			SortCard(byCardList,iCardCount);
			GetSerialNums(byCardList,iCardCount,iLen,vSerialCard);
			if(vSerialCard.size()>0)//��������
			{
				if(IsMajorCard(tFirstPlayerCard.byCards[0]))//����
				{
					bool bSerial=false;
					int index=0;
					for(int i=0;i<vSerialCard.size();i+=tFirstPlayerCard.iCardNums)
					{
						if(IsMajorCard(vSerialCard.at(i)))//����������
						{
							bSerial=true;
							index=i;
							break;
						}
					}
					if(bSerial)//���������������
					{
						tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
						int iTemp=0;
						for(int i=index;i<tFirstPlayerCard.iCardNums;i++)
						{
							tOutCard.byCards[iTemp++]=vSerialCard.at(i);
						}
						return;
					}
					else
					{
						if(0==vMajorCard.size())//û����������
						{
							tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
							memcpy(tOutCard.byCards,&byCardList[iCardCount-tFirstPlayerCard.iCardNums],sizeof(BYTE)*tFirstPlayerCard.iCardNums);
							return;
						}
						else if(vMajorCard.size()>0)
						{
							int iNums=0;
							for(int i=0;i<vMajorCard.size();i++)
							{
								tOutCard.byCards[iNums++]=vMajorCard.at(i);
							}
							BYTE byResultCard[33];
							memset(byResultCard,0,sizeof(byResultCard));
							int iResultCount=0;
							GetOtherCard(byCardList,iCardCount,byResultCard,iResultCount);
							SortCard(byResultCard,iResultCount);
							int iLenTemp=tFirstPlayerCard.iCardNums-iNums;
							if(iLenTemp>0)
							{
								memcpy(&tOutCard.byCards[iNums],&byResultCard[iResultCount-iLenTemp],sizeof(BYTE)*iLenTemp);
							}
							tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
							return;
						}
					}
				}
				//��һ�ҳ��ĸ���������
				//ͬ��ɫ�Ƴ���
				bool bSameFlower=false;//����ͬ��ɫ������
				int index=0;
				for(int i=0;i<vSerialCard.size();i+=tFirstPlayerCard.iCardNums)
				{
					if(tFirstPlayerCard.byFlower==GetCardHuaKind(vSerialCard.at(i)))
					{
						index=i;
						bSameFlower=true;
						break;
					}
				}
				if(bSameFlower)//�����ͬ��ɫ������
				{
					tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
					int iTemp=0;
					for(int i=index;i<tFirstPlayerCard.iCardNums;i++)
					{
						tOutCard.byCards[iTemp++]=vSerialCard.at(i);
					}
					return;
				}
				else//���뽫ͬ��ɫ�Ƴ���
				{
					vector<BYTE> vHandleSameFlowerCard;
					vHandleSameFlowerCard.clear();
					GetSameFlowerNums(byCardList,iCardCount,tFirstPlayerCard.byFlower,vHandleSameFlowerCard);
					if(vHandleSameFlowerCard.size()<=0)//����
					{
						tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
						memcpy(tOutCard.byCards,&byCardList[iCardCount-tFirstPlayerCard.iCardNums],sizeof(BYTE)*tFirstPlayerCard.iCardNums);
						return;
					}
					else if(vMajorCard.size()>0)
					{
						int iNums=0;
						for(int i=0;i<vHandleSameFlowerCard.size();i++)
						{
							tOutCard.byCards[iNums++]=vHandleSameFlowerCard.at(i);
						}
						for(int i=iNums;i<tFirstPlayerCard.iCardNums;i++)
						{
							for(int index=iCardCount-1;index>=0;index--)
							{
								if(tFirstPlayerCard.byFlower!=GetCardHuaKind(byCardList[index]))
								{
									tOutCard.byCards[i++]=byCardList[index];
								}
							}
						}
						tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
						return;
					}
				}
			}
			else
			{
				vector<BYTE> vHandleSameFlowerCard;
				vHandleSameFlowerCard.clear();
				GetSameFlowerNums(byCardList,iCardCount,tFirstPlayerCard.byFlower,vHandleSameFlowerCard);
				if(vHandleSameFlowerCard.size()<=0)//����
				{
					tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
					memcpy(tOutCard.byCards,&byCardList[iCardCount-tFirstPlayerCard.iCardNums],sizeof(BYTE)*tFirstPlayerCard.iCardNums);
					return;
				}
				else if(vMajorCard.size()>0)
				{
					int iNums=0;
					for(int i=0;i<vHandleSameFlowerCard.size();i++)
					{
						tOutCard.byCards[iNums++]=vHandleSameFlowerCard.at(i);
					}
					for(int i=iNums;i<tFirstPlayerCard.iCardNums;i++)
					{
						for(int index=iCardCount-1;index>=0;index--)
						{
							if(tFirstPlayerCard.byFlower!=GetCardHuaKind(byCardList[index]))
							{
								tOutCard.byCards[i++]=byCardList[index];
							}
						}

					}
					tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
					return;
				}
			}
		}
		if(4==tFirstPlayerCard.cardShape)//˦��
		{
			if(0==tFirstPlayerCard.iThrowStyle)//˦����
			{
				if(tFirstPlayerCard.iCardNums>=vMajorCard.size())//���������ȫ�����
				{
					tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
					int index=0;
					for(int i=0;i<vMajorCard.size();i++)
					{
						tOutCard.byCards[index++]=vMajorCard.at(i);
					}
					int iQue=tFirstPlayerCard.iCardNums-index;
					if(iQue>0)
					{
						BYTE byResultCard[33];
						memset(byResultCard,0,sizeof(byResultCard));
						int iResultCount=0;
						GetOtherCard(byCardList,iCardCount,byResultCard,iResultCount);
						for(int i=iResultCount-1;i>=0;i--)
						{
							tOutCard.byCards[index++]=byResultCard[i];
							if(tFirstPlayerCard.iCardNums==index)
							{
								return;
							}
						}
					}
				}
				else//���ϼ���д
				{
					tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
					int index=0;
					for(int i=vMajorCard.size()-1;i>=0;i--)
					{
						tOutCard.byCards[index++]=vMajorCard.at(i);
						if(tFirstPlayerCard.iCardNums==index)
						{
							return;
						}
					}
				}
			}
			if(1==tFirstPlayerCard.iThrowStyle)//˦ͬһ��ɫ��
			{
				vector<BYTE> vHandleSameFlowerCard;
				vHandleSameFlowerCard.clear();
				GetSameFlowerNums(byCardList,iCardCount,GetCardHuaKind(tFirstPlayerCard.byCards[0]),vHandleSameFlowerCard);//����ͬ��ɫ������
				if(tFirstPlayerCard.iCardNums>=vHandleSameFlowerCard.size())//�����ͬ��ɫ��ȫ�����
				{
					tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
					int index=0;
					for(int i=0;i<vHandleSameFlowerCard.size();i++)
					{
						tOutCard.byCards[index++]=vHandleSameFlowerCard.at(i);
					}
					int iQue=tFirstPlayerCard.iCardNums-index;
					if(iQue>0)
					{
						BYTE byResultCard[33];
						memset(byResultCard,0,sizeof(byResultCard));
						int iResultCount=0;
						for(int i=0;i<iCardCount;i++)
						{
							if(!IsMajorCard(byCardList[i]) && GetCardHuaKind(tFirstPlayerCard.byCards[0])!=GetCardHuaKind(byCardList[i]))
							{
								byResultCard[iResultCount++]=byCardList[i];
							}
						}
						for(int i=iResultCount-1;i>=0;i--)
						{
							tOutCard.byCards[index++]=byResultCard[i];
							if(tFirstPlayerCard.iCardNums==index)
							{
								return;
							}
						}
					}
				}
				else
				{
					tOutCard.byCardNums=tFirstPlayerCard.iCardNums;
					int index=0;
					for(int i=0;i<vHandleSameFlowerCard.size();i++)
					{
						tOutCard.byCards[index++]=vHandleSameFlowerCard.at(i);
						if(tFirstPlayerCard.iCardNums==index)
						{
							return;
						}
					}
				}
			}
		}
	}
}
//��÷���
int CUpGradeGameLogic::GetPoint(BYTE byCardList[],int iCardCount)
{
	int iScore=0;
	for (int i= 0;i<iCardCount;++i)
	{
		switch (GetCardNum(byCardList[i]))
		{
		case 0x05:
			{
				iScore += 5;
			}
			break;
		case 0x0A:
		case 0x0D:
			{
				iScore += 10;
			}
			break;
		}
	}
	return iScore;
}
//��־��¼
void CUpGradeGameLogic::Debuglog(const char *p, ...)
{
	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() +"\\����log\\";
	SHCreateDirectoryEx(NULL, strPath, NULL);
	CTime time = CTime::GetCurrentTime();
	sprintf(szFilename, "%s%d-%d-%d.log",strPath ,time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	//����ʱ��
	char cTime[30];
	::memset(cTime,0,sizeof(cTime));
	sprintf(cTime,"[%d:%d:%d] ",time.GetHour(),time.GetMinute(),time.GetSecond());
	fprintf(fp,cTime);
	va_list arg;
	va_start( arg, p );
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}