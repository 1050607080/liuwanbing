#include "stdafx.h"
//#include "../Client/stdafx.h"
#include "CUserDataEx.h"


CUserDataEx::CUserDataEx(void)
{
	CUserData::CUserData();
	m_byYaZhu = 255;//ׯ��Ѻע�����255δ������Ѻע��0��Ѻע��1ѹһ�룬2ѹȫ��
	memset(m_bYaZhu,0,sizeof(m_bYaZhu));//�м�ͬ��Ѻע���
	memset(m_byQiHuFan,255,sizeof(m_byQiHuFan));//��������ķ���
	memset(m_byMenPaiCount,0,sizeof(m_byMenPaiCount));//�ĸ��������Ƶ�����
}

CUserDataEx::~CUserDataEx(void)
{
	CUserData::~CUserData();
}


///���ƣ�IsHaveAHandPai
///����������Ƿ����ָ��������
///@param pai Ҫ������
///@return true �� false ��
bool CUserDataEx::IsHaveAHandPai(BYTE station,BYTE pai)
{
	return CUserData::IsHaveAHandPai(station,pai);
}

///���ƣ�GetAHandPaiCount
///���������������ĳ���Ƶĸ���
///@param pai Ҫ������
///@return ������
int CUserDataEx::GetAHandPaiCount(BYTE station,BYTE pai)
{
	return CUserData::GetAHandPaiCount(station,pai);
}

///���ƣ�GetHandPaiCount
///������������Ƹ���
///@param 
///@return ������Ƶ�����
int CUserDataEx::GetHandPaiCount(BYTE station)
{
	return CUserData::GetHandPaiCount(station);
}

///���ƣ�CopyHandPai
///������������ҵ�����
///@param pai[] ������, station λ��, show �Ƿ񿽱�����
///@return �����������
int CUserDataEx::CopyAllHandPai(BYTE pai[][HAND_CARD_NUM] ,BYTE station,bool show)
{
	return CUserData::CopyAllHandPai( pai,station ,show);
}

///���ƣ�CopyOneUserHandPai
///����������ĳ����ҵ�����
///@param pai[] ������, station λ��
///@return �����������
int CUserDataEx::CopyOneUserHandPai(BYTE pai[] ,BYTE station)
{
	return CUserData::CopyOneUserHandPai( pai,station);
}

///���ƣ�CopyOneUserHandPai
///����������ĳ����ҵ�����
///@param pai[] ������, station λ��
///@return �����������
int CUserDataEx::CopyOneUserHandPai(BYTE pai[] ,BYTE station,BYTE size)
{
	return CUserData::CopyOneUserHandPai( pai,station,size);
}

///���ƣ�SortHandPai
///��������������small�Ƿ��С������
///@param big �Ƿ�ӵ���С����Ĭ�ϴ�С��������
void CUserDataEx::SortHandPai(BYTE station,bool big)
{
	CUserData::SortHandPai( station, big);
}

///���ƣ�AddToHandPai
///���������һ���Ƶ�������
///@param pai ��ӵ�����ֵ
void CUserDataEx::AddToHandPai(BYTE station,BYTE pai)
{
	CUserData::AddToHandPai( station, pai);
}

///���ƣ�SetHandPaiData
///������������������
///@param  pai[] �������������
///@return 
void CUserDataEx::SetHandPaiData(BYTE station,BYTE pai[])
{
	CUserData::SetHandPaiData( station, pai);
}

///���ƣ�DelleteAHandPai
///������ɾ��һ��ָ��������
///@param pai Ҫɾ������
void CUserDataEx::DelleteAHandPai(BYTE station,BYTE pai)
{
	CUserData::DelleteAHandPai(station,pai);
}

///���ƣ�SetHandPaiBack
///���������������Ʊ�
///@param num���õ�����
void CUserDataEx::SetHandPaiBack(BYTE station,BYTE num)
{
	CUserData::SetHandPaiBack(station,num);
}

///���ƣ�GetLastHandPai
///������������һ������
///@param 
///@return 
BYTE CUserDataEx::GetLastHandPai(BYTE station)
{
	return CUserData::GetLastHandPai(station);
}

///���ƣ�GetSelectHandPai
///���������ѡ�е�һ������
///@param 
///@return 
BYTE CUserDataEx::GetSelectHandPai(BYTE station)
{
	return CUserData::GetSelectHandPai(station);
}

///���ƣ�IsOutPaiPeople
///�������Ƿ�������
///@param 
///@return  true �� ,false ����
bool CUserDataEx::IsOutPaiPeople(BYTE station)
{
	return CUserData::IsOutPaiPeople(station);
}

///���ƣ�IsHuaPaiInHand
///����������������Ƿ���ڻ���
///@param 
///@return  true �� ,false ����
bool CUserDataEx::IsHuaPaiInHand(BYTE station)
{
	return CUserData::IsHuaPaiInHand(station);
}

///���ƣ�MoveHuaPaiFormHand
///�������������еĻ����Ƶ�����������
///@param 
///@return  ���Ƶ�����
int CUserDataEx::MoveHuaPaiFormHand(BYTE station)
{
	return CUserData::MoveHuaPaiFormHand(station);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///���ƣ�GetUserGangNum
///��������ȡ��Ҹ��Ƶĸ���(���ܣ����ܣ�����)
///@param &an ��������, &ming �������� ,  &bu ��������
///@return ���Ƶ��ܸ���
int CUserDataEx::GetUserGangNum(BYTE station,BYTE &an,BYTE &ming , BYTE &bu)
{
	return CUserData::GetUserGangNum(station,an,ming ,bu);
}

///���ƣ�GetUserChiNum
///��������ҳ��ƴ���
///@param station ���λ��
///@return ���Ƶ��ܸ���
int CUserDataEx::GetUserChiNum(BYTE station)
{
	return CUserData::GetUserChiNum(station);
}

///���ƣ�GetUserPengNum
///��������Ҹ��ƴ���
///@param station ���λ��
///@return ���Ƶ��ܸ���
int CUserDataEx::GetUserPengNum(BYTE station)
{
	return CUserData::GetUserPengNum(station);
}
///���ƣ�IsUserHaveGangPai
///��������Ҹ���ĳ����
///@param pai ��, &type ����
///@return  true �� ,false ����
bool CUserDataEx::IsUserHaveGangPai(BYTE station,BYTE pai,BYTE &type)
{	
	return CUserData::IsUserHaveGangPai(station, pai, type);
}
///
///���ƣ�IsUserHavePengPai
///����������Ƿ�����ĳ����
///@param pai ��
///@return  true �� ,false ����
bool CUserDataEx::IsUserHavePengPai(BYTE station,BYTE pai)
{
	return CUserData::IsUserHavePengPai(station, pai);
}

///���ƣ�IsUserHaveChiPai
///����������Ƿ�Թ�ĳ����
///@param pai ��
///@return  true �� ,false ����
bool CUserDataEx::IsUserHaveChiPai(BYTE station,BYTE pai)
{
	return CUserData::IsUserHaveChiPai(station,pai);
}

///���ƣ�AddToGCP
///���������һ�����ݵ��ܳ���������
///@param gcpData Ҫ��ӵĳ���������
void CUserDataEx::AddToGCP(BYTE station,GCPStructEx *gcpData)
{
	CUserData::AddToGCP(station,gcpData);
}

///���ƣ�DelleteAGCPData
///������ɾ���ܳ���������ָ����������
///@param type ����, pai ��
void CUserDataEx::DelleteAGCPData(BYTE station,BYTE type ,BYTE pai)
{
	CUserData::DelleteAGCPData(station, type , pai);
}	

///���ƣ�CopyGCPData
///�����������ܳ��������е�������
///@param gcpData ����������
///@return �����ܸ���
int CUserDataEx::CopyGCPData(BYTE station,GCPStructEx gcpData[])
{
	return CUserData::CopyGCPData(station,gcpData);
}

///���ƣ�SetGCPData
///���������øܳ��������е�������
///@param gcpData ����������
void CUserDataEx::SetGCPData(BYTE station,GCPStructEx gcpData[])
{
	CUserData::SetGCPData(station,gcpData);
}

///���ƣ�GetGCPCount
///��������ȡ�ܳ����Ľڵ����
///@param 
///@return ��ó������ܸ���
int CUserDataEx::GetGCPCount(BYTE station)
{
	return CUserData::GetGCPCount(station);
}
///���ƣ�GetOnePaiGCPCount
///��������ȡ�ܳ�������ĳ���Ƶ�����
///@param 
///@return ĳ���Ƶ�����
int CUserDataEx::GetOnePaiGCPCount(BYTE station,BYTE pai)
{
	return CUserData::GetOnePaiGCPCount(station,pai);
}
///���ƣ�ShortGCP
///����������ܳ�������
///@param 
void CUserDataEx::ShortGCP(BYTE station)
{
	CUserData::ShortGCP(station);
}
///���ƣ�InitGCP
///��������ʼ���ܳ�������
///@param 
void CUserDataEx::InitGCP(BYTE station)
{
	CUserData::InitGCP(station);
}
//////////////////���Ʋ���///////////////////////////////////////////////////////////////////////////////

///���ƣ�AddToOutPai
///���������һ���Ƶ�����������
///@param pai ��
void CUserDataEx::AddToOutPai(BYTE station,BYTE pai)
{
	CUserData::AddToOutPai(station, pai);
}

///���ƣ�DelleteLastOutPai
///������ɾ�����һ�ų���
///@param 
void CUserDataEx::DelleteLastOutPai(BYTE station)
{
	CUserData::DelleteLastOutPai(station);
}

///���ƣ�SetOutPaiData
///���������ó�������
///@param pai[] ��������
void CUserDataEx::SetOutPaiData(BYTE station,BYTE pai[])
{
	CUserData::SetOutPaiData(station, pai);
}

///���ƣ�CopyOutPai
///������������ҵĳ���
///@param pai[] ��������
///@return ���Ƶ�����
int CUserDataEx::CopyOutPai(BYTE station,BYTE pai[])
{
	return CUserData::CopyOutPai(station, pai);
}

///���ƣ�GetOutPaiCount
///��������ȡ��ҵĳ�������
///@param 
///@return ���Ƶ�����
int CUserDataEx::GetOutPaiCount(BYTE station)
{
	return CUserData::GetOutPaiCount(station);
}
///���ƣ�GetOutPaiCount
///��������ȡ���ĳ���Ƶĳ�������
///@param 
///@return ĳ���Ƴ��Ƶ�����
int CUserDataEx::GetOneOutPaiCount(BYTE station,BYTE pai)
{
	return CUserData::GetOneOutPaiCount(station, pai);
}
/////////////////////���Ʋ���////////////////////////////////////////////////////////

///���ƣ�AddToHuaPai
///���������һ�Ż��Ƶ�����������
///@param pai ����
void CUserDataEx::AddToHuaPai(BYTE station,BYTE pai)
{
	CUserData::AddToHuaPai(station, pai);
}

///���ƣ�SetHuaPaiData
///���������û�������
///@param pai[] ��������
void CUserDataEx:: SetHuaPaiData(BYTE station,BYTE pai[])
{
	CUserData::SetHuaPaiData( station, pai);
}

///���ƣ�CopyHuaPai
///������������ҵĻ���
///@param pai[] ��������
///@return 
int CUserDataEx::CopyHuaPai(BYTE station,BYTE pai[])
{
	return CUserData::CopyHuaPai(station, pai);
}

///����Ƿ���
bool CUserDataEx::CheckIsHuaPai(BYTE pai)
{
	return CUserData::CheckIsHuaPai(pai);
}
///////////////////////���Ʋ���//////////////////////////////////////////////////////////////////

///���ƣ�SetMenPaiData
///������������������
///@param  pai[] ��������
void CUserDataEx::SetMenPaiData(BYTE station,BYTE pai[])
{
	CUserData::SetMenPaiData(station, pai);
}

///���ƣ�CopyMenPai
///������������ҵ�����
///@param  pai[] ��������
///@return �����������
int CUserDataEx::CopyMenPai(BYTE station,BYTE pai[])
{
	return CUserData::CopyMenPai(station,pai);
}

///���ƣ�ChangeMenPai
///����������ת��
///@param  pai[] ��������
void CUserDataEx::ChangeMenPai()
{
	int count =0;
	int index=0;
	for(int i=0;i<4;i++)
	{
		memset(m_iArMenPai[i],255,sizeof(m_iArMenPai[i]));
		for(int j=count;j<(count + m_byMenPaiCount[i]) ;j++)
		{
			if(m_MenPai.byMenPai[j] != 255)
			{
				m_iArMenPai[i][j-count] = 0;
			}
			else
			{
				m_iArMenPai[i][j-count] = 255;
			}
		}
		count += this->m_byMenPaiCount[i];
	}
}

///���ƣ�GetPaiNum
///��������ȡ���������Ч������
///@param  pai[] Ҫ����������, count ��Ч�Ƶ�����
///@return 
int CUserDataEx::GetPaiNum(BYTE station,BYTE pai[],BYTE count)
{
	return CUserData::GetPaiNum(station, pai,count);
}

///���ƣ�InitData
///��������ʼ������
void CUserDataEx::InitData()
{
	CUserData::InitData();
	m_byYaZhu = 255;//ׯ��Ѻע�����255δ������Ѻע��0��Ѻע��1ѹһ�룬2ѹȫ��
	memset(m_bYaZhu,0,sizeof(m_bYaZhu));//�м�ͬ��Ѻע���
	memset(m_byQiHuFan,255,sizeof(m_byQiHuFan));//��������ķ���	
}


//��������״̬
void CUserDataEx::SetQiHu(BYTE station,BYTE fan)
{
	if(m_byQiHuFan[station] = 255)
	{
		m_byQiHuFan[station] = fan;
	}
}
//�������״̬
void CUserDataEx::ReSetQiHu(BYTE station)
{
	m_byQiHuFan[station] = 255;
}
//����Ƿ�����״̬(����״̬���ܵ��ں���)
bool CUserDataEx::CheckQiHu(BYTE station,BYTE fan)
{
	if(m_byQiHuFan[station] != 255 && m_byQiHuFan[station]>= fan)
		return true;
	return false;
}

//���ø����������Ƶ�����
void CUserDataEx::SetMenPaiCount(BYTE count[])
{
	memcpy(m_byMenPaiCount,count,sizeof(m_byMenPaiCount));
}