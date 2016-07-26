#include "stdafx.h"
#include "CCheckHuPaiEx.h"

#include "../GameMessage/CSrvLogic.h"

CCheckHuPaiEx::CCheckHuPaiEx(void)
{
}
CCheckHuPaiEx::~CCheckHuPaiEx(void)
{
}
///����ܷ�����
bool CCheckHuPaiEx::CanTing(BYTE station,tagCPGNotifyEx &ResultData)
{
	return CCheckHuPai::CanTing( station, ResultData);
}

//��ϸ����������
bool CCheckHuPaiEx::CanHu(BYTE station,BYTE lastpai,tagCPGNotifyEx &ResultData,bool zimo,BYTE dianpao)
{
	//return CCheckHuPai::CanHu(station, lastpai, NotifyData, zimo,dianpao);

	//�������������
	this->ClearHupaiType(ResultData.byHuType);

	//�����������ڵ���
	m_byLastPai = lastpai;
	m_byStation = station;
	this->m_bZimo = zimo;

	memset(m_byArHandPai,255,sizeof(m_byArHandPai));
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		m_byArHandPai[i] = pDesk->m_UserData.m_byArHandPai[station][i];
		if(pDesk->m_UserData.m_byArHandPai[station][i] == 255 && lastpai != 255 && !zimo)//����ʱ������ڵ���
		{
			m_byArHandPai[i] = lastpai;
			break;
		}
	}
	///��С��������
	MaoPaoSort(m_byArHandPai,HAND_CARD_NUM,false);
	bool bb = false;
	bool big = false;//������
	///������ת��
	ChangeHandPaiData(m_byArHandPai,255);	

	int fan = 0;
	//����߶�

	int num = CheckQiDui();
	if(num == 1)//�߶�
	{
		SetAHupaiType(HUPAI_QI_DUI,ResultData.byHuType);
		bb = true;
		big = true;//������
		fan++;
	}
	else if(num > 1)//�����߶�
	{
		SetAHupaiType(HUPAI_HAO_HUA_QI_DUI,ResultData.byHuType);
		bb = true;
		big = true;//������
		fan++;
	}

	//û�к��Ƽ��ƽ������
	if(!bb && CheckPingHu(lastpai,zimo,true))
	{
		this->CheckPingHuData();
		bb = true;
		//������
		if(this->CheckPengPengHu())
		{
			SetAHupaiType(HUPAI_PENG_PENG_HU,ResultData.byHuType);
			big = true;//������
			fan++;
		}
	}

	if(bb)
	{		
		//��һɫ����һɫ

		if(this->CheckYiSe() == 2)//��һɫ
		{
			SetAHupaiType(HUPAI_QING_YI_SE,ResultData.byHuType);
			big = true;//������
			fan++;
		}

		//��⣺������غ����ܿ������ܣ��ܺ���
		if(this->CheckTianHu())//���
		{
			SetAHupaiType(HUPAI_TIAN_HU,ResultData.byHuType);
			big = true;//������
			fan++;
		}
		else if(this->CheckDiHu())//�غ�
		{
			SetAHupaiType(HUPAI_DI_HU,ResultData.byHuType);
			big = true;//������
			fan++;
		}
		else if(this->CheckGangKai())//�ܿ�
		{
			SetAHupaiType(HUPAI_GANG_KAI,ResultData.byHuType);
			fan++;
		}
		else if(this->CheckGangHouPao())//�ܺ���
		{
			SetAHupaiType(HUPAI_GANG_PAO,ResultData.byHuType);
		}
		else if(this->CheckQiangGangHe())//���ܺ�
		{
			SetAHupaiType(HUPAI_QIANG_GANG,ResultData.byHuType);
		}
		
		if(m_bZimo)//����
		{
			SetAHupaiType(HUPAI_ZI_MO,ResultData.byHuType);
		}
		
		if(!big)
		{
			SetAHupaiType(HUPAI_HU_PAI,ResultData.byHuType);//��ͨ���ƣ��Ƶ�����
		}

		ResultData.m_byFan = fan;//��¼�ܺ��ķ���

		ResultData.bHu = true;
		ResultData.m_iHuPs = lastpai;//��¼������
		ResultData.m_byDianPao = dianpao;
		ResultData.bCanAction = true;
		ResultData.m_byMaxThing = ACTION_HU;
	}
	return bb;
}


///ת���������ݵĴ洢��ʽ��Ϊ�˷�����Ƽ��
void CCheckHuPaiEx::ChangeHandPaiData(BYTE handpai[],BYTE pai)
{
	CCheckHuPai::ChangeHandPaiData(handpai,pai);
}

///ƽ�����
bool CCheckHuPaiEx::CheckPingHu(BYTE lastpai,bool zimo,bool jing,bool CheckTing)
{
	//return CCheckHuPai::CheckPingHu(lastpai,zimo,jing,CheckTing);

	hupaistruct.Init();
	PingHuStruct	TempHupaistruct;///�������ṹ����
	TempHupaistruct.Init();

	BYTE pai[4];
	bool caishen[4];
	memset(pai,255,sizeof(pai));
	memset(caishen,0,sizeof(caishen));
	bool canhu = false;
	m_iMaxFan = 0;

	if(m_byJingNum >= 2)
	{//˫������
		hupaistruct.Init();//��ʼ�����ƽṹ
		if(MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum-2))
		{		
			pai[0] = CMjEnum::MJ_TYPE_BAI;
			pai[1] =  CMjEnum::MJ_TYPE_BAI;
			caishen[0] = true;
			caishen[1] = true;
			hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
			canhu = true;
			if(GetMaxCombination(zimo,jing))//�ҵ�����﷬�����
			{
				TempHupaistruct = hupaistruct;
			}
		}
	}

	if(m_byJingNum >= 1)
	{//��������
		for(int i=0;i<NoJingHuTempData.conut;i++)
		{
			if(NoJingHuTempData.data[i][1]<1 || NoJingHuTempData.data[i][0]==255)
				continue;
			hupaistruct.Init();//��ʼ�����ƽṹ
			NoJingHuTempData.data[i][1]-=1;
			if(MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum-1))
			{		
				memset(caishen,0,sizeof(caishen));
				pai[0] = NoJingHuTempData.data[i][0];
				pai[1] = NoJingHuTempData.data[i][0];
				caishen[0] = true;
				hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
				NoJingHuTempData.data[i][1]+=1;
				canhu = true;
				if(GetMaxCombination(zimo,jing))//�ҵ�����﷬�����
				{
					TempHupaistruct = hupaistruct;
				}
			}
			else
			{
				NoJingHuTempData.data[i][1]+=1;
			}
		}
	}

	//���������ļ��,(�޾����齫HuTempData �� NoJingHuTempData��һ��������)
	for(int i=0;i<NoJingHuTempData.conut;i++)
	{
		if(NoJingHuTempData.data[i][1]<2 || NoJingHuTempData.data[i][0]==255)
			continue;
		hupaistruct.Init();//��ʼ�����ƽṹ
		NoJingHuTempData.data[i][1]-=2;
		if(MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum))
		{		
			pai[0] = NoJingHuTempData.data[i][0];
			pai[1] = NoJingHuTempData.data[i][0];
			hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
			NoJingHuTempData.data[i][1]+=2;
			canhu = true;
			if(GetMaxCombination(zimo,jing))//�ҵ�����﷬�����
			{
				TempHupaistruct = hupaistruct;
			}
		}
		else
		{
			NoJingHuTempData.data[i][1]+=2;
		}
	}
	//�ܺ�
	if(canhu)
	{
		hupaistruct = TempHupaistruct;
	}
	return canhu;
}

///��ȡ�����������
bool  CCheckHuPaiEx::GetMaxCombination(bool zimo,bool jing,BYTE lastpai)
{
	//return CCheckHuPai::GetMaxCombination(zimo,jing,lastpai);
	CheckPingHuData();//�Ȼ�ȡ�����
	int fan = 0;

	if(this->CheckPengPengHu())
	{
		fan += 1;
	}
	if(fan>m_iMaxFan|| (m_iMaxFan ==0 && fan==0))
	{
		m_iMaxFan = fan;
		return true;
	}
	return false;

}

///ƽ������
bool CCheckHuPaiEx::MakePingHu(CheckHuStruct &PaiData,PingHuStruct &hupaistruct,BYTE csnum)
{
	return CCheckHuPai::MakePingHu(PaiData,hupaistruct,csnum);
}

///��ʼ������
void CCheckHuPaiEx::InitData()
{
	CCheckHuPai::InitData();
}

///////////////////////////////��ϸ��������///////////////////////////////////////////////////////////////////////////////////////////////////

//�߶�
int CCheckHuPaiEx::CheckQiDui()	
{
	if(pDesk->m_UserData.GetGCPCount(m_byStation)>0)//�����г�����
	{
		return 0;
	}
	int need = 0,num=1;
	for(int i=0;i<NoJingHuTempData.conut;++i)
	{
		if(NoJingHuTempData.data[i][1] == 0 || NoJingHuTempData.data[i][1] == 255)
			continue;
		if(NoJingHuTempData.data[i][1]%2 == 1)
			need++;
		if(NoJingHuTempData.data[i][1] >= 3)
			num++;
	}
	if(m_byJingNum >= need && m_byJingNum!=255)
	{
		return num;
	}
	return 0;
}


//������
bool CCheckHuPaiEx::CheckPengPengHu()
{
	return CCheckHuPai::CheckPengPengHu();
}

//���
bool CCheckHuPaiEx::CheckTianHu()
{
	return CCheckHuPai::CheckTianHu();
}

//�غ�
bool CCheckHuPaiEx::CheckDiHu()
{
	return CCheckHuPai::CheckDiHu();
}

//���ϻ�
bool CCheckHuPaiEx::CheckGangKai()
{
	return CCheckHuPai::CheckGangKai();
}

//�ܺ���
bool CCheckHuPaiEx::CheckGangHouPao()
{
	return CCheckHuPai::CheckGangHouPao();
}

//���ܺ�
bool CCheckHuPaiEx::CheckQiangGangHe()
{
	return CCheckHuPai::CheckQiangGangHe();
}





