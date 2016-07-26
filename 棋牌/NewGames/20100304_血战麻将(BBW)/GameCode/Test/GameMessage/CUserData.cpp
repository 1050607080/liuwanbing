#include "stdafx.h"
//#include "../Client/stdafx.h"
#include "CUserData.h"


CUserData::CUserData(void)
{
	memset(m_byArHandPai,255,sizeof(m_byArHandPai));	/// ���Ʋ�����20�ţ�255Ϊ������־
	memset(m_byArOutPai,255,sizeof(m_byArOutPai));		/// ���Ʋ�����160�ţ�255Ϊ������־
	memset(m_byArHuaPai,255,sizeof(m_byArHuaPai));		/// ���Ʋ�����10�ţ�255Ϊ������־
	memset(m_iArMenPai,255,sizeof(m_iArMenPai));		/// ���Ʋ�����40�ţ�255Ϊ������־
	memset(m_byGuanXi,0,sizeof(m_byGuanXi));			/// �����ܹ�ϵ����
	memset(m_bEnableCard,1,sizeof(m_bEnableCard));		/// �Ƿ���õ���
	memset(m_bIsBoy,0,sizeof(m_bIsBoy));				/// �ؼ�����	
	memset(m_byCanOutCard,255,sizeof(m_byCanOutCard));	/// ͣ�ƻ������������ܴ������
	memset(m_byMenFeng,255,sizeof(m_byMenFeng));		/// �ŷ�	

	memset(m_byLastZhuaPai,255,sizeof(m_byLastZhuaPai));///	���ץ������
	memset(m_bySetMoPai,255,sizeof(m_bySetMoPai));//ץ��ǿ��ָ��ֵ�������ã�
	memset(m_bSiziFinish,0,sizeof(m_bSiziFinish));			///	����Ƿ񷵻���ɫ�ӽ�����Ϣ

	m_bWaitSezi = false;//�Ƿ���ȴ�ɫ�Ӷ�������״̬

	m_StrJing.Init();
	for(int i=0;i<4;++i)
	{//�������ݽṹ
		m_SetData[i].Init();
	}
	//��ʼ����
	m_MenPai.Init();

	m_byFoceLeavCount=0;		///����뿪������
	m_byNtStation = 255;		///ׯ��λ��	
	m_byStation = 255;			///��ǰ��ҵ��߼�λ�ã��ͻ���ʹ�ã�
	byPlayCount = 0;			///��Ϸ�Ѿ����еľ�����ǿ�ˣ���ȫ�˳�����ɢ�����¿�ʼ���㣩
	m_byQuanFeng = 255;			///Ȧ��


	m_bySelectIndex = 255;		///��ǰѡ��������
	m_byLastOutPai = 255;		///���������
	m_byMeZhuaPai = 255;		///��ǰ������ץ��
	m_byNowOutStation = 255;	///��ǰ����λ��
	m_byLastOutStation = 255;	///�ϴγ���λ��
	m_byOtherOutPai = 255;		///��ǰ������Ҵ������
	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	m_bZhuaPaiDir = true;

	m_byApplyMaxAction = 0;//��ҵ�ǰ���������¼�
	m_byLianZhuang = 0;//��ׯ��
	m_bitUserState = 0;//���״̬

}

CUserData::~CUserData(void)
{

}


///���ƣ�IsHaveAHandPai
///����������Ƿ����ָ��������
///@param pai Ҫ������
///@return true �� false ��
bool CUserData::IsHaveAHandPai(BYTE station,BYTE pai)
{
	if(station <0 || station>=PLAY_COUNT)
		return false;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[station][i] == pai)
			return true;
	}
	return false;
}

///���ƣ�GetAHandPaiCount
///���������������ĳ���Ƶĸ���
///@param pai Ҫ������
///@return ������
int CUserData::GetAHandPaiCount(BYTE station,BYTE pai)
{
	if(station <0 || station>=PLAY_COUNT)
		return 0;
	int count = 0;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[station][i] == pai)
			count++;
	}
	return count;
}

///���ƣ�GetHandPaiCount
///������������Ƹ���
///@param 
///@return ������Ƶ�����
int CUserData::GetHandPaiCount(BYTE station)
{
	if(station <0 || station>=PLAY_COUNT)
		return 0;
	int count = 0;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[station][i] != 255)
			count++;
	}
	return count;
}

///���ƣ�CopyHandPai
///������������ҵ�����
///@param pai[] ������, station λ��, show �Ƿ񿽱�����
///@return �����������
int CUserData::CopyAllHandPai(BYTE pai[][HAND_CARD_NUM] ,BYTE station,bool show)
{
	if(station <0 || station>=PLAY_COUNT)
		return 0;
	for(int i=0;i<4;++i)
	{
		if(i == station)//�Լ�����
		{
			memcpy(pai[i],m_byArHandPai[station],sizeof(m_byArHandPai[station]));
		}
		else 
		{
			if(!show)//��ʾ�Ʊ�
			{
				for(int j=0;j<HAND_CARD_NUM;++j)
				{	
					pai[i][j] = 255;
					if( m_byArHandPai[i][j] != 255)
					{	
						pai[i][j] = 0;
					}
				}
			}
			else
			{
				memcpy(pai[i],m_byArHandPai[i],sizeof(m_byArHandPai[i]));
			}
		}
	}
	return 0;
}

///���ƣ�CopyOneUserHandPai
///����������ĳ����ҵ�����
///@param pai[] ������, station λ��
///@return �����������
int CUserData::CopyOneUserHandPai(BYTE pai[] ,BYTE station)
{
	if(station <0 || station>=PLAY_COUNT)
		return 0;

	memcpy(pai,m_byArHandPai[station],sizeof(m_byArHandPai[station]));
	return GetHandPaiCount(station);
}

///����������ĳ����ҵĲ�������
int CUserData::CopyOneUserHandPai(BYTE pai[] ,BYTE station,BYTE size)
{
	if(station <0 || station>=PLAY_COUNT)
		return 0;


	//memcpy(pai,m_byArHandPai[station],size);
	if(size > HAND_CARD_NUM)
		size = HAND_CARD_NUM;//��ֹԽ��
	for(int i=0;i<size;++i)
	{
		pai[i] = m_byArHandPai[station][i];
	}
	return size;
}

///���ƣ�SortHandPai
///��������������small�Ƿ��С������
///@param big �Ƿ�ӵ���С����Ĭ�ϴ�С��������
void CUserData::SortHandPai(BYTE station,bool big)
{
	if(station <0 || station>=PLAY_COUNT)
		return ;
	///����
	MaoPaoSort(m_byArHandPai[station],HAND_CARD_NUM,big);
	BYTE HandPai[HAND_CARD_NUM];
	memcpy(HandPai,m_byArHandPai[station],sizeof(m_byArHandPai[station]));
	int index=0;
	for(int j=0;j<HAND_CARD_NUM;++j)
	{
		if(m_StrJing.CheckIsJing(HandPai[j]))//�����������ǰ��
		{
			m_byArHandPai[station][index] = HandPai[j];
			index++;
		}
	}
	for(int j=0;j<HAND_CARD_NUM;++j)
	{
		if(!m_StrJing.CheckIsJing(HandPai[j]) && index<HAND_CARD_NUM)
		{
			m_byArHandPai[station][index] = HandPai[j];
			index++;
		}
	}
}

///���ƣ�AddToHandPai
///���������һ���Ƶ�������
///@param pai ��ӵ�����ֵ
void CUserData::AddToHandPai(BYTE station,BYTE pai)
{
	if(station <0 || station>=PLAY_COUNT)
		return ;
	if((GetHandPaiCount(station)+3*GetGCPCount(station))>=MAX_HAND_PAI || pai==255)
	{
		return;
	}
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[station][i] == 255)
		{
			m_byArHandPai[station][i] = pai;
			break;
		}
	}
}

///���ƣ�SetHandPaiData
///������������������
///@param  pai[] �������������
///@return 
void CUserData::SetHandPaiData(BYTE station,BYTE pai[])
{
	if(station <0 || station>=PLAY_COUNT)
		return ;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		m_byArHandPai[station][i] = pai[i];
	}
}

///���ƣ�DelleteAHandPai
///������ɾ��һ��ָ��������
///@param pai Ҫɾ������
void CUserData::DelleteAHandPai(BYTE station,BYTE pai)
{
	if(station <0 || station>=PLAY_COUNT)
		return ;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[station][i] == pai)
		{
			m_byArHandPai[station][i] = 255;
			break;
		}
	}
	SortHandPai(station,false);
}

///���ƣ�DelleteSomeHandPai
///������ɾ��һ��ָ��������
///@param pai Ҫɾ������,len ����
void CUserData::DelleteSomeHandPai(BYTE station,BYTE pai[],BYTE len)
{	
	if(station <0 || station>=PLAY_COUNT)
		return ;

	for(int i=0;i<len;++i)
	{
		DelleteAHandPai(station,pai[i]);
	}
	SortHandPai(station,false);
}

///���ƣ�SetHandPaiBack
///���������������Ʊ�
///@param num���õ�����
///@return 
void CUserData::SetHandPaiBack(BYTE station,BYTE num)
{
	if(station <0 || station>=PLAY_COUNT)
		return ;
	memset(m_byArHandPai[station],255,sizeof(m_byArHandPai[station]));
	for(int i=0;i<num;i++)
	{
		m_byArHandPai[station][i] = 0;
	}
}

///���ƣ�GetLastHandPai
///������������һ������
///@param 
///@return 
BYTE CUserData::GetLastHandPai(BYTE station)
{
	if(station <0 || station>=PLAY_COUNT)
		return 255;
	for(int i=HAND_CARD_NUM-1;i>=0;i--)
	{
		if(m_byArHandPai[station][i] != 255 && m_bEnableCard[station][i])
		{
			return m_byArHandPai[station][i];
		}
	}
	return 255;
}

///���ƣ�GetSelectHandPai
///���������ѡ�е�һ������
///@param 
///@return 
BYTE CUserData::GetSelectHandPai(BYTE station)
{
	if(station <0 || station>=PLAY_COUNT)
		return 255;
	if(m_bySelectIndex<HAND_CARD_NUM && m_bySelectIndex!=255)
		return m_byArHandPai[station][m_bySelectIndex];
	return 255;
}

///���ƣ�IsOutPaiPeople
///�������Ƿ�������
///@param 
///@return  true �� ,false ����
bool CUserData::IsOutPaiPeople(BYTE station)
{
	if(station <0 || station>=PLAY_COUNT)
		return false;
	if(GetHandPaiCount(station)%3==2)
	{
		return true;
	}
	return false;
}

///���ƣ�IsHuaPaiInHand
///����������������Ƿ���ڻ���
///@param 
///@return  true �� ,false ����
bool CUserData::IsHuaPaiInHand(BYTE station)
{
	if(station <0 || station>=PLAY_COUNT)
		return false;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(CheckIsHuaPai(m_byArHandPai[station][i]))
		{
			return true;
		}
	}
	return false;
}

///���ƣ�MoveHuaPaiFormHand
///�������������еĻ����Ƶ�����������
///@param 
///@return  ���Ƶ�����
int CUserData::MoveHuaPaiFormHand(BYTE station)
{
	if(station <0 || station>=PLAY_COUNT)
		return 0;

	BYTE index = 255;
	int hua = 0;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[station][i] == 255)
			break;
		if(CheckIsHuaPai(m_byArHandPai[station][i]))
		{
			hua++;
			AddToHuaPai(station,m_byArHandPai[station][i]);
			DelleteAHandPai(station,m_byArHandPai[station][i]);
		}
	}
	for(int j=0;j<hua;j++)//�ƶ��˶����Ż��ƾͲ���������
	{
		GetPai(index,station,false,1);//��ʼǰ����ץ��
	}
	return hua;
}

///ͨ��������ȡһ������
BYTE CUserData::GetHandPaiFromIndex(BYTE station,int index)
{
	if(station <0 || station>=PLAY_COUNT)
		return 255;
	return m_byArHandPai[station][index];
}

///ͨ����������һ������
bool CUserData::SetHandPaiFromIndex(BYTE station,int index,BYTE pai)
{
	if(station <0 || station>3 || !CMjRef::IsValidateMj(pai))
		return false;
	if(m_byArHandPai[station][index] == 255)
		return false;
	m_byArHandPai[station][index] = pai;
	return true;
}

///�����Ƿ�����
bool CUserData::IsHaveHandPaiFromIndex(BYTE station,int index)
{
	if(station <0 || station>3 || index < 0 || index >= HAND_CARD_NUM)
	{
		return false;
	}
	if(m_byArHandPai[station][index] == 255)
	{
		return false;
	}
	return true;
}

///�������Ƿ����ĳ�ֻ�ɫ����
bool CUserData::IsHaveHuaSe(BYTE station,BYTE huase)
{
	if(station <0 || station>=4)
		return false;

	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[station][i] == 255)
			break;
		if(m_byArHandPai[station][i]/10 == huase)
			return true;
	}
	return false;
}

///��Ϸ��ʼʱ����
void CUserData::SendCard()
{
	BYTE index = 255;
	//һ���Է���
	for(int i=0;i<PLAY_COUNT;i++)
	{
		for(int j=0;j<MAX_HAND_PAI-1;++j)
		{
			GetPai(index,i,true,1);
		}
	}
	//ׯ�Ҷ�ץһ����
	GetPai(index,m_byNtStation,true,1);
}

///���ƣ�GetPai
///������ץһ����
///@param station ץ����ҵ�λ�� , head  �Ƿ��ǰ��ץ,type ץ������ 0 ����ץ�ƣ�1��ʼǰ���ƣ�2��ǰ����ץ�ƣ�3����
///@return ץ������ֵ
BYTE CUserData::GetPai(BYTE &index,int station,bool head,BYTE type)
{
	if(station <0 || station>=PLAY_COUNT)
		return 255;

	BYTE card=GetPaiFromMenPai(index, head);
#ifdef MJ_CAN_SETPAI
	if (m_bySetMoPai[station] != 255)
	{
		card = m_bySetMoPai[station];
		m_bySetMoPai[station] = 255;
	}
#endif //MJ_CAN_SETPAI
	AddToHandPai(station,card);
	return card;
}

///��������ȡ��ǽ�е�һ����
BYTE CUserData::GetPaiFromMenPai(BYTE &index,bool head)
{
	BYTE card=255;
	while(card==255)
	{
		if(head)//��ǰץ
		{
			if(m_MenPai.byStartIndex>=m_MenPai.byAllPaiNum)//������������
			{
				m_MenPai.byStartIndex = 0;
			}
			card = m_MenPai.byMenPai[m_MenPai.byStartIndex];
			m_MenPai.byMenPai[m_MenPai.byStartIndex] = 255;
			index = m_MenPai.byStartIndex;
			m_MenPai.byStartIndex++;
		}
		else//�Ӻ���ץ
		{
			if(m_MenPai.byEndIndex == m_MenPai.byCaiShenIndex)
			{
				if(m_MenPai.byEndIndex == 0)
				{
					m_MenPai.byEndIndex = m_MenPai.byAllPaiNum-1;
				}
				else
				{
					m_MenPai.byEndIndex--;
				}
			}
			else
			{
				card = m_MenPai.byMenPai[m_MenPai.byEndIndex];
				m_MenPai.byMenPai[m_MenPai.byEndIndex] = 255;
				index = m_MenPai.byEndIndex;
				if(m_MenPai.byEndIndex == 0)
				{
					m_MenPai.byEndIndex = m_MenPai.byAllPaiNum-1;
				}
				else
				{
					m_MenPai.byEndIndex--;
				}
			}
		}
	}
	return card;
}

///��������ץ������ת����ץ����ҵ���������
bool CUserData::ZhuaPaiIndex(BYTE &index,BYTE &dir)
{
	if(index == 255)
		return false;
	int num = 0;
	num = m_MenPai.byAllPaiNum/4;
	if(num <= 0)
		return false;
	dir = index/num;
	index = index%num;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///���ƣ�GetUserGangNum
///��������ȡ��Ҹ��Ƶĸ���(���ܣ����ܣ�����)
///@param &an ��������, &ming �������� ,  &bu ��������
///@return ���Ƶ��ܸ���
int CUserData::GetUserGangNum(BYTE station,BYTE &an,BYTE &ming , BYTE &bu)
{
	if(station <0 || station>=4)
		return 0;
	int all = 0;
	an = 0;
	ming = 0;
	bu = 0;
	for(int i=0;i<5;i++)
	{
		switch(m_UserGCPData[station][i].byType)
		{
		case ACTION_AN_GANG://����
			an++;
			break;
		case ACTION_BU_GANG://����
			bu++;
			break;
		case ACTION_MING_GANG://����
			ming++;
			break;
		}
	}
	all = ming + an + bu;
	return all;
}

///���ƣ�GetUserChiNum
///��������ҳ��ƴ���
///@param station ���λ��
///@return ���Ƶ��ܸ���
int CUserData::GetUserChiNum(BYTE station)
{
	if(station <0 || station>=4)
		return 0;
	int num = 0;
	for(int i=0;i<5;i++)
	{
		switch(m_UserGCPData[station][i].byType)
		{
		case ACTION_CHI://����
			num++;
			break;
		}
	}
	return num;
}

///���ƣ�GetUserPengNum
///��������Ҹ��ƴ���
///@param station ���λ��
///@return ���Ƶ��ܸ���
int CUserData::GetUserPengNum(BYTE station)
{
	if(station <0 || station>=4)
		return 0;
	int num = 0;
	for(int i=0;i<5;i++)
	{
		switch(m_UserGCPData[station][i].byType)
		{
		case ACTION_PENG://����
			num++;
			break;
		}
	}
	return num;
}

///���ƣ�IsUserHaveGangPai
///��������Ҹ���ĳ����
///@param pai ��, &type ����
///@return  true �� ,false ����
bool CUserData::IsUserHaveGangPai(BYTE station,BYTE pai,BYTE &type)
{	
	if(station <0 || station>=4)
		return false;
	for(int i=0;i<5;i++)
	{
		if(m_UserGCPData[station][i].byType>=ACTION_AN_GANG && m_UserGCPData[station][i].byType<=ACTION_MING_GANG && m_UserGCPData[station][i].byData[0] == pai)
		{
			type = m_UserGCPData[station][i].byType;
			return true;
		}
	}
	return false;
}
///
///���ƣ�IsUserHavePengPai
///����������Ƿ�����ĳ����
///@param pai ��
///@return  true �� ,false ����
bool CUserData::IsUserHavePengPai(BYTE station,BYTE pai)
{
	if(station <0 || station>=4)
		return false;
	for(int i=0;i<5;i++)
	{
		if(m_UserGCPData[station][i].byType == 255)
			continue;
		if(m_UserGCPData[station][i].byType == ACTION_PENG &&  m_UserGCPData[station][i].byData[0] == pai)
		{
			return true;
		}
	}
	return false;
}

///���ƣ�IsUserHaveChiPai
///����������Ƿ�Թ�ĳ����
///@param pai ��
///@return  true �� ,false ����
bool CUserData::IsUserHaveChiPai(BYTE station,BYTE pai)
{
	if(station <0 || station>=4)
		return false;
	for(int i=0;i<5;i++)
	{
		if(m_UserGCPData[station][i].byType==ACTION_CHI &&  m_UserGCPData[station][i].iOutpai == pai)
		{
			return true;
		}
	}
	return false;
}

///���ƣ�AddToGCP
///���������һ�����ݵ��ܳ���������
///@param gcpData Ҫ��ӵĳ���������
///@return 
void CUserData::AddToGCP(BYTE station,GCPStructEx *gcpData)
{
	if(station <0 || station>=4)
		return ;
	int count=this->GetGCPCount(station);
	if(MAX_HAND_PAI<(count*3+this->GetHandPaiCount(station)))//��ֹ����
	{
		return;
	}
	for(int i=0;i<5;i++)
	{
		if(m_UserGCPData[station][i].byType==255)
		{
			m_UserGCPData[station][i].byType = gcpData->byType;
			m_UserGCPData[station][i].iBeStation = gcpData->iBeStation;
			m_UserGCPData[station][i].iOutpai = gcpData->iOutpai;
			m_UserGCPData[station][i].iStation = gcpData->iStation;
			for(int j=0;j<4;j++)
			{
				m_UserGCPData[station][i].byData[j] = gcpData->byData[j];
			}
			break;
		}
	}
}

///���ƣ�DelleteAGCPData
///������ɾ���ܳ���������ָ����������
///@param type ����, pai ��
///@return 
void CUserData::DelleteAGCPData(BYTE station,BYTE type ,BYTE pai)
{			
	if(station <0 || station>=4)
		return ;
	for(int i=0;i<5;i++)
	{
		if(m_UserGCPData[station][i].byType == type && (m_UserGCPData[station][i].byData[0]==pai ||m_UserGCPData[station][i].byData[1]==pai || m_UserGCPData[station][i].byData[2]==pai ))
		{
			m_UserGCPData[station][i].Init();
			break;
		}
	}
	ShortGCP(station);//����ܳ�������
}	

///���ƣ�CopyGCPData
///�����������ܳ��������е�������
///@param gcpData ����������
///@return �����ܸ���
int CUserData::CopyGCPData(BYTE station,GCPStructEx gcpData[])
{
	if(station <0 || station>=4)
		return 0;

	memcpy(gcpData,m_UserGCPData[station],sizeof(m_UserGCPData[station]));
	return GetGCPCount(station);
}

///����������Ҹܳ��������е�������
void CUserData::CopyAllGCPData(GCPStructEx gcpData[][5])
{
	memcpy(gcpData,m_UserGCPData,sizeof(m_UserGCPData));
}

///���ƣ�SetGCPData
///���������øܳ��������е�������
///@param gcpData ����������
///@return 
void CUserData::SetGCPData(BYTE station,GCPStructEx gcpData[])
{
	if(station <0 || station>=4)
		return ;
	for(int i=0;i<5;i++)
	{
		m_UserGCPData[station][i] = gcpData[i];
	}
}

///���ƣ�GetGCPCount
///��������ȡ�ܳ����Ľڵ����
///@param 
///@return ��ó������ܸ���
int CUserData::GetGCPCount(BYTE station)
{
	if(station <0 || station>=4)
		return 0;
	int count =0;
	for(int i=0;i<5;i++)
	{
		if(m_UserGCPData[station][i].byType!=255)
		{
			count++;
		}
	}
	return count;
}
///���ƣ�GetOnePaiGCPCount
///��������ȡ�ܳ�������ĳ���Ƶ�����
///@param 
///@return ĳ���Ƶ�����
int CUserData::GetOnePaiGCPCount(BYTE station,BYTE pai)
{
	if(station <0 || station>=4)
		return 0;
	int count =0;
	for(int i=0;i<5;i++)
	{
		if(m_UserGCPData[station][i].byType==255)
			continue;
		for(int j=0;j<4;j++)
		{
			if(m_UserGCPData[station][i].byData[j] == pai && pai!=255)
			{
				count++;
			}
		}
	}
	return count;
}
///���ƣ�ShortGCP
///����������ܳ�������
///@param 
///@return 
void CUserData::ShortGCP(BYTE station)
{
	if(station <0 || station>=4)
		return ;
	int index=0;
	GCPStructEx m_TempGCPData[5];
	for(int i=0;i<5;i++)
	{
		m_TempGCPData[i] = m_UserGCPData[station][i];
	}
	memset(m_UserGCPData[station],255,sizeof(m_UserGCPData[station]));
	for(int i=0;i<5;i++)
	{
		if(m_TempGCPData[i].byType != 255)
		{
			m_UserGCPData[station][index] = m_TempGCPData[i];
			index++;
		}
	}
}
///���ƣ�InitGCP
///��������ʼ���ܳ�������
///@param 
///@return 
void CUserData::InitGCP(BYTE station)
{
	if(station <0 || station>=4)
		return ;
	for(int i=0;i<5;i++)
	{
		m_UserGCPData[station][i].Init();
	}
}
//////////////////���Ʋ���///////////////////////////////////////////////////////////////////////////////

///���ƣ�AddToOutPai
///���������һ���Ƶ�����������
///@param pai ��
///@return 
void CUserData::AddToOutPai(BYTE station,BYTE pai)
{
	if(station <0 || station>=4)
		return ;
	for(int i=0;i<OUT_CARD_NUM;i++)
	{
		if(m_byArOutPai[station][i] == 255)
		{
			m_byArOutPai[station][i] = pai;
			break;
		}
	}
}

///���ƣ�DelleteLastOutPai
///������ɾ�����һ�ų���
///@param 
///@return 
void CUserData::DelleteLastOutPai(BYTE station)
{
	if(station <0 || station>=4)
		return ;
	for(int i=0;i<OUT_CARD_NUM;i++)
	{
		if(m_byArOutPai[station][i] == 255 && i>0)
		{
			m_byArOutPai[station][i-1] = 255;
			break;
		}
	}
}

///���ƣ�SetOutPaiData
///���������ó�������
///@param pai[] ��������
///@return 
void CUserData::SetOutPaiData(BYTE station,BYTE pai[])
{
	if(station <0 || station>=4)
		return ;
	//memcpy(m_byArOutPai[station],pai,sizeof(m_byArOutPai[station]));
	for(int i=0;i<OUT_CARD_NUM;i++)
	{
		m_byArOutPai[station][i] = pai[i] ;
	}
}

///���ƣ�CopyOutPai
///������������ҵĳ���
///@param pai[] ��������
///@return ���Ƶ�����
int CUserData::CopyOutPai(BYTE station,BYTE pai[])
{
	if(station <0 || station>=4)
		return 0;
	//memcpy(pai,m_byArOutPai,sizeof(m_byArOutPai));
	for(int i=0;i<OUT_CARD_NUM;i++)
	{
		pai[i] = m_byArOutPai[station][i];
	}
	return GetPaiNum(station,m_byArOutPai[station],OUT_CARD_NUM);
}

///����������ҵĳ���
void CUserData::CopyAllOutPai(BYTE pai[][OUT_CARD_NUM])
{
	memcpy(pai,m_byArOutPai,sizeof(m_byArOutPai));
}

///���ƣ�GetOutPaiCount
///��������ȡ��ҵĳ�������
///@param 
///@return ���Ƶ�����
int CUserData::GetOutPaiCount(BYTE station)
{
	if(station <0 || station>=4)
		return 0;
	int num = 0;
	for(int i=0;i<OUT_CARD_NUM;i++)
	{
		if(m_byArOutPai[station][i] != 255)
		{
			num++;
		}
	}
	return num;
}
///��ȡ��ҵ����һ�ų���
BYTE CUserData::GetLastOutPai(BYTE station)
{
	if(station <0 || station>=4)
		return 255;
	if(m_byArOutPai[station][0] == 255)
		return 255;
	for(int i=0;i<OUT_CARD_NUM;i++)
	{
		if(m_byArOutPai[station][i] == 255 && i>0)
		{
			return m_byArOutPai[station][i-1];
		}
	}
	return 255;
}
///���ƣ�GetOutPaiCount
///��������ȡ���ĳ���Ƶĳ�������
///@param 
///@return ĳ���Ƴ��Ƶ�����
int CUserData::GetOneOutPaiCount(BYTE station,BYTE pai)
{
	if(station <0 || station>=4)
		return 0;
	int num = 0;
	for(int i=0;i<OUT_CARD_NUM;i++)
	{
		if(m_byArOutPai[station][i] != 255 && m_byArOutPai[station][i] == pai)
		{
			num++;
		}
	}
	return num;
}
/////////////////////���Ʋ���////////////////////////////////////////////////////////

///���ƣ�AddToHuaPai
///���������һ�Ż��Ƶ�����������
///@param pai ����
///@return 
void CUserData::AddToHuaPai(BYTE station,BYTE pai)
{
	if(station <0 || station>=4)
		return ;
	for(int i=0;i<HUA_CARD_NUM;i++)
	{
		if(m_byArHuaPai[station][i] == 255)
		{
			m_byArHuaPai[station][i] = pai;
			break;
		}
	}
}

///���ƣ�SetHuaPaiData
///���������û�������
///@param pai[] ��������
///@return 
void CUserData:: SetHuaPaiData(BYTE station,BYTE pai[])
{
	if(station <0 || station>=4)
		return ;
	memcpy(m_byArHuaPai[station],pai,sizeof(m_byArHuaPai[station]));
}

///���ƣ�CopyHuaPai
///������������ҵĻ���
///@param pai[] ��������
///@return 
int CUserData::CopyHuaPai(BYTE station,BYTE pai[])
{
	if(station <0 || station>=4)
		return 0;
	int count =0;
	for(int i=0;i<HUA_CARD_NUM;i++)
	{
		pai[i] = m_byArHuaPai[station][i];
		if(m_byArHuaPai[station][i]!=255)
		{
			count++;
		}
	}
	return count;
}

///����������ҵĻ���
void CUserData::CopyAllHuaPai(BYTE pai[][HUA_CARD_NUM])
{
	memcpy(pai,m_byArHuaPai,sizeof(m_byArHuaPai));
}

///����Ƿ���
bool CUserData::CheckIsHuaPai(BYTE pai)
{
	if(pai>=CMjEnum::MJ_TYPE_FCHUN && pai != 255)
		return true;
	return false;
}
///////////////////////���Ʋ���//////////////////////////////////////////////////////////////////

///���ƣ�SetMenPaiData
///������������������
///@param  pai[] ��������
///@return 
void CUserData::SetMenPaiData(BYTE station,BYTE pai[])
{
	if(station <0 || station>=4)
		return ;
	memcpy(m_iArMenPai[station],pai,sizeof(m_iArMenPai[station]));
}

///���ƣ�CopyMenPai
///������������ҵ�����
///@param  pai[] ��������
///@return �����������
int CUserData::CopyMenPai(BYTE station,BYTE pai[])
{
	if(station <0 || station>=4)
		return 0;
	for(int i=0;i<MEN_CARD_NUM;i++)
	{
		pai[i] = m_iArMenPai[station][i];
	}
	return GetPaiNum(station,m_iArMenPai[station],MEN_CARD_NUM);
}

///����������ҵ�����
void CUserData::CopyAllMenPai(BYTE pai[][MEN_CARD_NUM])
{
	memcpy(pai,m_iArMenPai,sizeof(m_iArMenPai));
}

///���ƣ�ChangeMenPai
///����������ת��
///@param  pai[] ��������
///@return �����������
void CUserData::ChangeMenPai()
{
	int num = 0;
	int index=0;
	num = m_MenPai.byAllPaiNum/4;
	for(int i=0;i<4;i++)
	{
		memset(m_iArMenPai[i],255,sizeof(m_iArMenPai[i]));
		for(int j=i*num;j<(i+1)*num ;j++)
		{
			if(m_MenPai.byMenPai[j] != 255)
			{
				m_iArMenPai[i][j-i*num] = 0;
				if(m_MenPai.byCaiShenIndex == j && m_MenPai.byCaiShenIndex != 255)
				{
					m_iArMenPai[i][j-i*num] = m_MenPai.byMenPai[j];
				}
			}
			else
			{
				m_iArMenPai[i][j-i*num] = 255;
			}
		}
	}
}

///���������ݣ�ÿ�ַ���ǰ����
void CUserData::RandCard()
{
	///������
	BYTE index = m_MenPai.byAllPaiNum;
	srand(GetTickCount());
	BYTE temp=255,data=0;
	for(int i=0;i<index;i++)
	{
		temp = rand()%(index-i)+i;
		data = m_MenPai.byMenPai[temp];
		m_MenPai.byMenPai[temp] = m_MenPai.byMenPai[i];
		m_MenPai.byMenPai[i] = data;
	}
	///////////////
	m_MenPai.byGetPai = m_MenPai.byGetPai%13;
	if(m_MenPai.byGetPaiDir == 255)
	{
		m_MenPai.byGetPaiDir = m_byNtStation;
	}
	m_MenPai.byGetPaiDir = m_MenPai.byGetPaiDir%4;
	int dun = 0,num=0;
	dun = index/8;
	num = (dun*m_MenPai.byGetPaiDir + m_MenPai.byGetPai)*2;
	if(num == 0)
		num=2;
	m_MenPai.byStartIndex = num;	//������ʼ����
	m_MenPai.byEndIndex = num-1;	//���ƽ�������

	m_MenPai.byStart = num;	//������ʼ����
	m_MenPai.byEnd = num-1;	//���ƽ�������
}

///���÷����е�����,send �Ѿ����Ĵ���
void CUserData::SetMenPaiFaPai(BYTE num)
{
	int send = PLAY_COUNT*2*num;
	if(send > PLAY_COUNT*12)
		send = PLAY_COUNT*12;
	int index = m_MenPai.byStart;
	for(int i=0;i<send;++i)
	{
		if(index>=m_MenPai.byAllPaiNum)//������������
		{
			index = 0;
		}
		m_MenPai.byMenPai[index] = 255;
		index++;
	}
	this->ChangeMenPai();
}


///////////////////////////////ɫ�Ӳ���//////////////////////////////////////////////////////////////

//����Ƿ��Ѿ���������ɫ�ӣ�station ���λ�ã�����ĳ������Ƿ����
bool CUserData::IsOneSeziFinish(BYTE station)
{
	return m_bSiziFinish[station];
}

//����Ƿ��Ѿ���������ɫ�ӣ������Ƿ�������ҽ���ɫ��
bool CUserData::IsAllSeziFinish()
{
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(!m_bSiziFinish[i])//�����û�з���ɫ�ӽ�����Ϣ
			return false;
	}
	return true;
}
	
//���õȴ�ɫ�ӽ���
void CUserData::SetWaiteSezi(bool wait)
{
	memset(m_bSiziFinish,0,sizeof(m_bSiziFinish));
	m_bWaitSezi = wait;
}

///////////////////���״̬����/////////////////////////////////////////////

//����/ȡ����ҵ�״̬
void CUserData::SetUserState(BYTE station,BYTE type,bool state)
{
	if(station <0 || station>=PLAY_COUNT)
		return;
	type += station*32;
	if(state)
	{
		m_bitUserState.set(type);
	}
	else
	{
		m_bitUserState.reset(type);////��������
	}
}
//ȡ��������ҵ�ĳ��״̬
void CUserData::ReSetUserState(BYTE type)
{
	BYTE state = 0;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		state = type + i*32;
		m_bitUserState.reset(state);////��������
	}
}
//��ȡ��ҵ�״̬
bool CUserData::IsUserState(BYTE station,BYTE type)
{
	if(station <0 || station>=PLAY_COUNT)
		return false;
	if(m_bitUserState.test(type+station*32))//״̬����
	{
		return true;
	}
	return false;
}

//�Ƿ�������Ҷ�������ĳ��״̬
bool CUserData::IsAllUserState(BYTE type)
{
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(!IsUserState(i,type))
		{
			return false;
		}
	}
	return true;
}

///////////////////////////////�����ͻ��˲���//////////////////////////////////////////////////////////
///���ƣ�ChangeAllHandPai
///����������
///@param  station λ��, pai[] ��, count������
void CUserData::ChangeAllHandPai(BYTE station,BYTE pai[],BYTE count)
{
	if(station <0 || station>=PLAY_COUNT)
		return ;

	for(int i=0;i<count;i++)
	{
		if(!CMjRef::IsValidateMj(pai[i]))
			continue;
		if(m_byArHandPai[station][i] == 255)
			break;
		m_byArHandPai[station][i] = pai[i];
	}
}

///���ƣ�ChangeAllHandPai
///��ָ������
///@param  station λ��, pai[] ��, handpai[]Ҫ����������, count������
void CUserData::ChangeSomeHandPai(BYTE station,BYTE pai[],BYTE handpai[],BYTE count)
{
	if(station <0 || station>=PLAY_COUNT)
		return ;
	for(int i=0;i<count;i++)
	{
		if(!CMjRef::IsValidateMj(pai[i]) || !CMjRef::IsValidateMj(handpai[i]))
			continue;
		if(IsHaveAHandPai(station,handpai[i]))
		{
			DelleteAHandPai(station,handpai[i]);
			AddToHandPai(station,pai[i]);
		}
	}
}

///���ƣ�SetMeNextPai
///�����Լ�����һ����
///@param  station λ��,pai ��
void CUserData::SetMeNextPai(BYTE station,BYTE pai)
{
}

///���ƣ�SetGameNextPai
///������ǽ����һ����
///@param  pai ��
void CUserData::SetGameNextPai(BYTE pai)
{
	if(m_MenPai.byStartIndex == 255 )
		return;
	if(m_MenPai.byMenPai[m_MenPai.byStartIndex] == 255)
		return;
	m_MenPai.byMenPai[m_MenPai.byStartIndex]  = pai;
}

///�����ͻ���ץ�ƣ������ץ���ƻ�����Ҫץ���ƣ�
void CUserData::SuperZhuaPai(BYTE &pai)
{
	///���������Ҫץ����(�������汾)
	if(m_SetData[m_byNowOutStation].bySetType==2)//�������汾��������Ҫץ����
	{		
		DelleteAHandPai(m_byNowOutStation,pai);
		pai = m_SetData[m_byNowOutStation].mjdata[0];
		AddToHandPai(m_byNowOutStation,pai);
		m_SetData[m_byNowOutStation].Init();
	}
}

///���ƣ�GetPaiNum
///��������ȡ���������Ч������
///@param  pai[] Ҫ����������, count ��Ч�Ƶ�����
///@return 
int CUserData::GetPaiNum(BYTE station,BYTE pai[],BYTE count)
{
	int num=0;
	for(int i=0;i<count;i++)
	{
		if(pai[i]!=255)
		{
			num++;
		}
	}
	return num;
}


///���ƣ�InitData
///��������ʼ������
///@param 
///@return 
void CUserData::InitData()
{
	memset(m_byArHandPai,255,sizeof(m_byArHandPai));	//���Ʋ�����20�ţ�255Ϊ������־
	memset(m_byArOutPai,255,sizeof(m_byArOutPai));		//���Ʋ�����160�ţ�255Ϊ������־
	memset(m_byArHuaPai,255,sizeof(m_byArHuaPai));		//���Ʋ�����10�ţ�255Ϊ������־
	memset(m_iArMenPai,255,sizeof(m_iArMenPai));		//���Ʋ�����40�ţ�255Ϊ������־
	memset(m_byMenPaiData,255,sizeof(m_byMenPaiData));	//ת�������������
		
	memset(m_byGuanXi,0,sizeof(m_byGuanXi));			//�����ܹ�ϵ����
	memset(m_bEnableCard,1,sizeof(m_bEnableCard));		//�Ƿ���õ���
	memset(m_bySetMoPai,255,sizeof(m_bySetMoPai));		//ץ��ǿ��ָ��ֵ�������ã�

	m_StrJing.Init();//��ʼ������
	m_MenPai.Init();//��ʼ����
	for(int i=0;i<4;++i)
	{//�������ݽṹ
		m_SetData[i].Init();
	}
	for(int i=0;i<5;i++)
	{
		m_UserGCPData[0][i].Init();
		m_UserGCPData[1][i].Init();
		m_UserGCPData[2][i].Init();
		m_UserGCPData[3][i].Init();
	}	

	memset(m_byCanOutCard,255,sizeof(m_byCanOutCard));///ͣ�ƻ������������ܴ������
	memset(m_byMenFeng,255,sizeof(m_byMenFeng));	///�ŷ�	
	memset(m_byLastZhuaPai,255,sizeof(m_byLastZhuaPai));///	���ץ������


	m_byFoceLeavCount=0;				////����뿪������
	m_bySelectIndex = 255;				///��ǰѡ��������
	m_byLastOutPai = 255;				///���������
	m_byMeZhuaPai = 255;				///��ǰ������ץ��
	m_byNowOutStation = 255;			///��ǰ����λ��
	m_byLastOutStation = 255;			///�ϴγ���λ��
	m_byOtherOutPai = 255;				///��ǰ������Ҵ������
	memset(m_bSiziFinish,0,sizeof(m_bSiziFinish));			///	����Ƿ񷵻���ɫ�ӽ�����Ϣ
	m_bWaitSezi = false;//�Ƿ���ȴ�ɫ�Ӷ�������״̬
	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	m_bZhuaPaiDir = true;
	m_byApplyMaxAction = 0;//��ҵ�ǰ���������¼�
	m_bitUserState = 0;//һ�ֿ�ʼʱȡ������״̬
} 