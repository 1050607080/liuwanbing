#include "stdafx.h"
#include "CCheckHuPai.h"
#include "CSrvLogic.h"

CCheckHuPai::CCheckHuPai(void)
{
	m_byStation = 0;///��ǰ�����ҵ�λ��
	hupaistruct.Init();
	HuTempData.Init();	//���Ƽ����ʱ����
	NoJingHuTempData.Init();///���Ƽ����ʱ����(�޲���)	
	m_byJingNum = 0;  //��������
	m_byLastPai = 255;//�����������ڵ���
	m_bZimo = false;//�Ƿ����������ж�
	m_byOutStation = 255;///��ǰ���Ƶ�λ��
}

CCheckHuPai::~CCheckHuPai(void)
{
}

//��ȡ�����ָ��
void CCheckHuPai::SetDeskPoint(CSrvLogic *desk)
{
	pDesk = desk;
}

///���ƣ�ChangeHandPaiData
///������ת���������ݵĴ洢��ʽ��Ϊ�˷�����Ƽ��
///@param handpai[] ��������, pai ���˴������
void CCheckHuPai::ChangeHandPaiData(BYTE handpai[],BYTE pai)
{			
	HuTempData.Init();
	NoJingHuTempData.Init();///���Ƽ����ʱ����(�޲���)	
	m_byJingNum = 0;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(handpai[i] != 255 )
		{
			if(!pDesk->m_UserData.m_StrJing.CheckIsJing(handpai[i]))//������
			{
				NoJingHuTempData.Add(handpai[i]);
			}
			else
			{
				m_byJingNum++;
			}
			HuTempData.Add(handpai[i]);
		}
	}
	if(pai!= 255)
	{
		if(!pDesk->m_UserData.m_StrJing.CheckIsJing(pai))//������
		{
			NoJingHuTempData.Add(pai);
		}
		else
		{
			m_byJingNum++;
		}
		HuTempData.Add(pai);
	}
}

///���ƣ�CanTing
///����������ܷ�����
///@param  handpai[] , CanOut[] ����������Ƶ��ƣ������ж��ţ�
///@return true ������false ������
bool CCheckHuPai::CanTing(BYTE station,tagCPGNotifyEx &ResultData)
{
	//�Ѿ����ƻ�ȡ�й�
	if(pDesk->m_UserData.IsUserState( station,USER_STATE_TING) || pDesk->m_UserData.IsUserState( station,USER_STATE_AUTO))
		return false;
	if(!pDesk->m_GameData.m_mjAction.bTing)//��������
		return false;
	
	m_byStation = station;
	memset(m_byArHandPai,255,sizeof(m_byArHandPai));
	memset(ResultData.m_byTingCanOut,255,sizeof(ResultData.m_byTingCanOut));
	int num=0;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		m_byArHandPai[i] = pDesk->m_UserData.m_byArHandPai[station][i];
	}
	MaoPaoSort(m_byArHandPai,HAND_CARD_NUM,false);
	ChangeHandPaiData(m_byArHandPai,255);
	m_byJingNum += 1;
	BYTE pai =255;
	for(int i=0;i<NoJingHuTempData.conut;i++)
	{
		if(NoJingHuTempData.data[i][1] < 1 || NoJingHuTempData.data[i][1] == 255 || NoJingHuTempData.data[i][0] == 255)
			continue;
		pai = NoJingHuTempData.data[i][0];
		NoJingHuTempData.data[i][1] -= 1;

		if(CheckPingHu(255,true,true)|| CheckQiDui()>0 || CheckShiSanYao()|| CheckLvYiSe() )//ƽ�����߶ԣ�ʮ���ۣ���һɫ
		{
			ResultData.m_byTingCanOut[num] = pai;
			num++;
		}
		NoJingHuTempData.data[i][1] += 1;
	}
	if(num>0)
	{
		ResultData.bTing = true;
		ResultData.bCanAction = true;
		return true;
	}
	return false;
}

///���ƣ�ChiTing
///����ܷ�����ƣ��Ժ�����
///@param  station ����λ��, CanOut[] ����������Ƶ��ƣ�ÿ�ų���ֻ����һ�ţ�
///@return true ������false ������
bool CCheckHuPai::ChiTing(BYTE station,tagCPGNotifyEx &cpg,BYTE CanOut[])
{
	m_byStation = station;

	return false;
}

///���ƣ�PengTing
///����ܷ������ƣ���������
///@param  station ����λ��, CanOut[] ����������Ƶ��ƣ�ÿ�ų���ֻ����һ�ţ�
///@return true ������false ������
bool CCheckHuPai::PengTing(BYTE station,tagCPGNotifyEx &cpg,BYTE CanOut[])
{
	m_byStation = station;

	return false;
}

///���ƣ�GangTing
///���ܺ��ܷ񱣴�����״̬
///@param  station ����λ��, CanTing[] ��Ӧ����ϸܺ��ܷ񱣴�����paip[][4] ���Ƶ����
///@return true ������false ������
bool CCheckHuPai::GangTing(BYTE station,tagCPGNotifyEx &cpg ,BYTE ps,bool zimo)
{
	m_byStation = station;
	m_bZimo = zimo;
	BYTE pai = 255;
	BYTE OldHandPai[HAND_CARD_NUM];//ԭʼ����
	memset(OldHandPai,255,sizeof(OldHandPai));
	int index = 0;
	bool bgang = false;
	for(int i=0;i<HAND_CARD_NUM;i++)//��¼ԭ������
	{
		OldHandPai[i] = pDesk->m_UserData.m_byArHandPai[station][i];
	}

	if(zimo)//�Լ�ץ��
	{	
		//���Ÿ��Ƽ��
		for(int i=0;i<HAND_CARD_NUM;i++)
		{ 
			if(pDesk->m_UserData.m_byArHandPai[station][i] == 255 || pai == pDesk->m_UserData.m_byArHandPai[station][i])//���ظ����һ������
				continue;
			pai = pDesk->m_UserData.m_byArHandPai[station][i];
			if(pDesk->m_UserData.GetAHandPaiCount(station,pai)<4)
				continue;
			//ɾ��Ҫ�ܵ���
			for(int j=0;j<4;j++)
			{
				pDesk->m_UserData.DelleteAHandPai(station,pai);
			}
			//�鹹һ�����ƣ�����ɾ��
			GCPStructEx gang;
			gang.Init();
			gang.byType = ACTION_AN_GANG;
			gang.iStation = gang.iBeStation = station;
			gang.byData[0] = gang.byData[1] = gang.byData[2] = gang.byData[3] = pai;
			pDesk->m_UserData.AddToGCP(station,&gang);
			//���������
			//if(CanTing(station,Out))
			//{
			//	cpg.bGang = bgang = true;
			//	memset(cpg.m_iGangData,255,sizeof(cpg.m_iGangData));
			//	cpg.m_iGangData[index][0] = ACTION_AN_GANG;//����
			//	cpg.m_iGangData[index][1] = pai;
			//	index++;
			//}
			pDesk->m_UserData.DelleteAGCPData(station,ACTION_AN_GANG,pai);//�ָ���������
			ResetHandPai(station,OldHandPai);//�ָ�����
		}
		//���ܼ�⣺��ͨ����,ֻ������һ����
		if(!pDesk->m_UserData.IsUserHavePengPai(station,pai))//����������
				return bgang;
		pai = pDesk->m_UserData.GetLastHandPai(station);//�����һ����
		pDesk->m_UserData.DelleteAHandPai(station,pai);//��ʱɾ��
		//�鹹һ�����ƣ�����ɾ��
		GCPStructEx gang;
		gang.Init();
		gang.byType = ACTION_BU_GANG;
		gang.iStation = gang.iBeStation = station;
		gang.byData[0] = gang.byData[1] = gang.byData[2] = gang.byData[3] = pai;
		pDesk->m_UserData.AddToGCP(station,&gang);
		//���������
		//if(CanTing(station,Out))
		//{
		//	//��������
		//	cpg.bGang = bgang = true;
		//	memset(cpg.m_iGangData,255,sizeof(cpg.m_iGangData));
		//	cpg.m_iGangData[index][0] = ACTION_BU_GANG;//����
		//	cpg.m_iGangData[index][1] = pai;
		//	index++;
		//}
		pDesk->m_UserData.DelleteAGCPData(station,ACTION_BU_GANG,pai);//�ָ���������
		ResetHandPai(station,OldHandPai);//�ָ�����
	}
	else //���˳���
	{	
		if(ps == 255 || pDesk->m_UserData.GetAHandPaiCount(station,ps) <3)//û�����Ų��ܸ���
			return false;
		//ɾ��Ҫ�ܵ���
		for(int j=0;j<3;j++)
		{
			pDesk->m_UserData.DelleteAHandPai(station,ps);
		}
		//�鹹һ�����ƣ�����ɾ��
		GCPStructEx gang;
		gang.Init();
		gang.byType = ACTION_MING_GANG;
		gang.iBeStation = cpg.byOutStation;
		gang.iStation = station;
		gang.byData[0] = gang.byData[1] = gang.byData[2] = gang.byData[3] = ps;
		pDesk->m_UserData.AddToGCP(station,&gang);
		//if(CanTing(station,Out))
		//{
		//	cpg.bGang =bgang= true;
		//	memset(cpg.m_iGangData,255,sizeof(cpg.m_iGangData));
		//	cpg.m_iGangData[0][0] = ACTION_MING_GANG;
		//	cpg.m_iGangData[0][1] = ps;
		//}
		pDesk->m_UserData.DelleteAGCPData(station,ACTION_MING_GANG,ps);
		ResetHandPai(station,OldHandPai);//�ָ�����
	}
	return bgang;
}
///�������Ƽ��
bool CCheckHuPai::CheckTing(BYTE pai[])
{

	return false;
}

///������ܺ�ָ�����
void CCheckHuPai::ResetHandPai(BYTE station,BYTE pai[])
{
	memset(pDesk->m_UserData.m_byArHandPai[station],255,sizeof(pDesk->m_UserData.m_byArHandPai[station]));
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		pDesk->m_UserData.m_byArHandPai[station][i] = pai[i];
	}
}

///���ƣ�CanHu
///��������ϸ������
///@param station ���Ƽ��λ�ã�hutype[] �ܺ��Ƶ����ͣ�zimo �Ƿ�����
///@return true  �ܺ��� false ����
bool CCheckHuPai::CanHu(BYTE station,BYTE lastpai,tagCPGNotifyEx &ResultData,bool zimo,BYTE dianpao)
{
	//�����������ڵ���
	m_byLastPai = lastpai;
	m_byStation = station;
	m_bZimo = zimo;//�Ƿ����������ж�
	m_byOutStation = ResultData.byOutStation;///��ǰ���Ƶ�λ��

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
	///������ת��
	ChangeHandPaiData(m_byArHandPai,255);	

	if(pDesk->m_GameData.m_mjHuType.byQiXing!=255 && CheckQiXingBuKao())//����
	{
		SetAHupaiType(HUPAI_TYPE_QiXingBuKao,ResultData.byHuType);
		bb = true;
	}
	else if(pDesk->m_GameData.m_mjHuType.byQiXingBuKao!=255 && CheckQuanBuKao())//ʮ���� 
	{
		SetAHupaiType(HUPAI_TYPE_QuanBuKao,ResultData.byHuType);
		bb = true;
	}
	else if(pDesk->m_GameData.m_mjHuType.byLianQiDui!=255 && CheckLianQiDui())//���߶�
	{
		SetAHupaiType(HUPAI_TYPE_LianQiDui,ResultData.byHuType);
		bb = true;
	}
	else if(pDesk->m_GameData.m_mjHuType.byQiDui!=255 && CheckQiDui()>0)//�߶�
	{
		SetAHupaiType(HUPAI_TYPE_QiDui,ResultData.byHuType);
		bb = true;
	}
	else if(pDesk->m_GameData.m_mjHuType.byShiSanYao!=255 && CheckShiSanYao())//ʮ����
	{
		SetAHupaiType(HUPAI_TYPE_ShiSanYao,ResultData.byHuType);
		bb = true;
	}
	
	if(!bb && CheckPingHu(lastpai,zimo,true))
	{
		bb = true;
		for(int i=0;i<MAX_HUPAI_TYPE;i++)
		{
			ResultData.byHuType[i] = m_byHuType[i];
		}
	}
	if(bb)
	{
		ResultData.bHu = true;
		ResultData.m_iHuPs = lastpai;//��¼������
		ResultData.m_byDianPao = dianpao;
		ResultData.bCanAction = true;
		ResultData.m_byMaxThing = ACTION_HU;
		
	}
	return bb;
}

///���ƣ�CheckPingHu
///������ƽ�����
///@param  lastpai ���õ����� , zimo �Ƿ�����, jing �Ƿ�������, CheckTing�Ƿ����Ƶļ��
///@return true �����ƽ���� false �������ƽ��
bool CCheckHuPai::CheckPingHu(BYTE lastpai,bool zimo,bool jing,bool CheckTing)
{
	hupaistruct.Init();
	PingHuStruct	TempHupaistruct;///�������ṹ����
	BYTE			m_byTempHuType[MAX_HUPAI_TYPE];///��������
	TempHupaistruct.Init();
	memset(m_byTempHuType,255,sizeof(m_byTempHuType));
	bool canhu = false;
	BYTE pai[4];
	bool caishen[4];
	memset(pai,255,sizeof(pai));
	memset(caishen,0,sizeof(caishen));
	m_iMaxFan = 0;

	////���ڵ���2������ļ��,˫��������
	if(m_byJingNum>=2 && m_byJingNum!=255)
	{
		if(MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum-2))
		{		
			memset(caishen,0,sizeof(caishen));
			memset(pai,255,sizeof(pai));
			pai[0] = pDesk->m_UserData.m_StrJing.byPai[0];
			pai[1] = pDesk->m_UserData.m_StrJing.byPai[0];
			caishen[0] = true;
			caishen[1] = true;
			hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
			canhu = true;
			if(GetMaxCombination(zimo,jing))//�ҵ�����﷬�����
			{
				TempHupaistruct = hupaistruct;
				for(int j=0;j<MAX_HUPAI_TYPE;j++)
				{
					m_byTempHuType[j] = m_byHuType[j];
				}
			}
		}
	}
	//1������ļ��,һ�������һ��������
	if(m_byJingNum>=1 && m_byJingNum!=255)
	{
		for(int i=0;i<NoJingHuTempData.conut;i++)
		{
			if(NoJingHuTempData.data[i][1]!=1 || NoJingHuTempData.data[i][0]==255)
				continue;
			hupaistruct.Init();//��ʼ�����ƽṹ
			NoJingHuTempData.data[i][1]-=1;
			if(MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum-1))
			{		
				memset(caishen,0,sizeof(caishen));
				memset(pai,255,sizeof(pai));
				pai[0] = NoJingHuTempData.data[i][0];
				pai[1] = NoJingHuTempData.data[i][0];
				caishen[0] = true;
				hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
				canhu = true;
				if(GetMaxCombination(zimo,jing))//�ҵ�����﷬�����
				{
					TempHupaistruct = hupaistruct;
					for(int j=0;j<MAX_HUPAI_TYPE;j++)
					{
						m_byTempHuType[j] = m_byHuType[j];
					}
				}
				NoJingHuTempData.data[i][1]+=1;
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
		if(MakePingHu(NoJingHuTempData,hupaistruct,0))
		{		
			memset(caishen,0,sizeof(caishen));
			memset(pai,255,sizeof(pai));
			pai[0] = NoJingHuTempData.data[i][0];
			pai[1] = NoJingHuTempData.data[i][0];
			hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
			canhu = true;
			if(GetMaxCombination(zimo,jing))//�ҵ�����﷬�����
			{
				TempHupaistruct = hupaistruct;
				for(int j=0;j<MAX_HUPAI_TYPE;j++)
				{
					m_byTempHuType[j] = m_byHuType[j];
				}
			}
			NoJingHuTempData.data[i][1]+=2;
		}
		else
		{
			NoJingHuTempData.data[i][1]+=2;
		}
	}
	if(jing && m_byJingNum>0 && m_byJingNum!=255)
	{//�޾����,����ԭ���
		//���������ļ��
		for(int i=0;i<HuTempData.conut;i++)
		{
			if(HuTempData.data[i][1]<2 || HuTempData.data[i][0]==255)
				continue;
			hupaistruct.Init();//��ʼ�����ƽṹ
			HuTempData.data[i][1]-=2;
			if(MakePingHu(HuTempData,hupaistruct,0))
			{		
				memset(caishen,0,sizeof(caishen));
				memset(pai,255,sizeof(pai));
				pai[0] = HuTempData.data[i][0];
				pai[1] = HuTempData.data[i][0];
				hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
				canhu = true;
				if(GetMaxCombination(zimo,jing))//�ҵ�����﷬�����
				{
					TempHupaistruct = hupaistruct;
					for(int j=0;j<MAX_HUPAI_TYPE;j++)
					{
						m_byTempHuType[j] = m_byHuType[j];
					}
				}
				HuTempData.data[i][1]+=2;
			}
			else
			{
				HuTempData.data[i][1]+=2;
			}
		}
	}
	//�ܺ�
	if(canhu)
	{
		hupaistruct = TempHupaistruct;
		for(int j=0;j<MAX_HUPAI_TYPE;j++)
		{
			m_byHuType[j] = m_byTempHuType[j];
		}
	}
	return canhu;
}
///��ȡ�����������
bool CCheckHuPai:: GetMaxCombination(bool zimo,bool jing,BYTE lastpai)
{
	CheckPingHuData();//�Ȼ�ȡ�����

	this->ClearHupaiType(m_byHuType);//�����������
	int fan = 0;
	///���������ͣ��������̨��������
	SetAHupaiType(HUPAI_TYPE_HuPai,m_byHuType);//����

	if(pDesk->m_GameData.m_mjHuType.byPingHu !=255 && CheckIsPingHu())////ƽ��
	{
		fan += pDesk->m_GameData.m_mjHuType.byPingHu;
		SetAHupaiType(HUPAI_TYPE_PingHu,m_byHuType);//
	}
///////
	if(pDesk->m_GameData.m_mjHuType.byYiBanGao !=255 && CheckYiBanGao())//һ���
	{
		fan += pDesk->m_GameData.m_mjHuType.byYiBanGao;
		SetAHupaiType(HUPAI_TYPE_YiBanGao,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byXiXiangFeng !=255 && CheckXiXiangFeng())//ϲ���
	{
		fan += pDesk->m_GameData.m_mjHuType.byXiXiangFeng;
		SetAHupaiType(HUPAI_TYPE_XiXiangFeng,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byLianLiu !=255 && CheckLianLiu())//����
	{
		fan += pDesk->m_GameData.m_mjHuType.byLianLiu;
		SetAHupaiType(HUPAI_TYPE_LianLiu,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byLaoShaoFu !=255 && CheckLaoShaoFu())//���ٸ�
	{
		fan += pDesk->m_GameData.m_mjHuType.byLaoShaoFu;
		SetAHupaiType(HUPAI_TYPE_LaoShaoFu,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byYaoJiuKe !=255 && CheckYaoJiuKe())//�۾ſ�
	{
		fan += pDesk->m_GameData.m_mjHuType.byYaoJiuKe;
		SetAHupaiType(HUPAI_TYPE_YaoJiuKe,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byMingGang !=255 && CheckMingGang())//����
	{
		fan += pDesk->m_GameData.m_mjHuType.byMingGang;
		SetAHupaiType(HUPAI_TYPE_MingGang,m_byHuType);//
	}		
	if(pDesk->m_GameData.m_mjHuType.byBianZhang !=255 && CheckBianZhang())//����
	{
		fan += pDesk->m_GameData.m_mjHuType.byBianZhang;
		SetAHupaiType(HUPAI_TYPE_BianZhang,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byKanZhang !=255 && CheckKanZhang())//����
	{
		fan += pDesk->m_GameData.m_mjHuType.byKanZhang;
		SetAHupaiType(HUPAI_TYPE_KanZhang,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byDanDiao !=255 && CheckDanDiaoJiang())//������
	{
		fan += pDesk->m_GameData.m_mjHuType.byDanDiao;
		SetAHupaiType(HUPAI_TYPE_DanDiaoJiang,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byJianKe !=255 && CheckJianKe())//����
	{
		fan += pDesk->m_GameData.m_mjHuType.byJianKe;
		SetAHupaiType(HUPAI_TYPE_JianKe,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byQuanFengKe !=255 && CheckQuanFengKe())//Ȧ���
	{
		fan += pDesk->m_GameData.m_mjHuType.byQuanFengKe;
		SetAHupaiType(HUPAI_TYPE_QuanFengKe,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byMenFengKe !=255 && CheckMenFengKe())//�ŷ��
	{
		fan += pDesk->m_GameData.m_mjHuType.byMenFengKe;
		SetAHupaiType(HUPAI_TYPE_MenFengKe,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byMenQianQing !=255 && CheckMenQianQing())//��ǰ��
	{
		fan += pDesk->m_GameData.m_mjHuType.byMenQianQing;
		SetAHupaiType(HUPAI_TYPE_MenQianQing,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.bySiGuiYi !=255 && CheckSiGuiYi())//�Ĺ�һ
	{
		fan += pDesk->m_GameData.m_mjHuType.bySiGuiYi;
		SetAHupaiType(HUPAI_TYPE_SiGuiYi,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byShuangTongKe !=255 && CheckShuangTongKe())//˫ͬ��
	{
		fan += pDesk->m_GameData.m_mjHuType.byShuangTongKe;
		SetAHupaiType(HUPAI_TYPE_ShuangTongKe,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byShuangAnKe !=255 && CheckShuangAnGang())//˫����
	{
		fan += pDesk->m_GameData.m_mjHuType.byShuangAnKe;
		SetAHupaiType(HUPAI_TYPE_ShuangAnKe,m_byHuType);//
	}
	
	if(pDesk->m_GameData.m_mjHuType.byQuanDaiYao !=255 && CheckQuanDaiYao())//ȫ����
	{
		fan += pDesk->m_GameData.m_mjHuType.byQuanDaiYao;
		SetAHupaiType(HUPAI_TYPE_QuanDaiYao,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byBuQiuRen !=255 && CheckBuQiuRen())//������
	{
		fan += pDesk->m_GameData.m_mjHuType.byBuQiuRen;
		SetAHupaiType(HUPAI_TYPE_BuQiuRen,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byShuangMingGang !=255 && CheckShuangMingGang())//˫����
	{
		fan += pDesk->m_GameData.m_mjHuType.byShuangMingGang;
		SetAHupaiType(HUPAI_TYPE_ShuangMingGang,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byHuJueZhang !=255 && CheckHuJueZhang())//�;���
	{
		fan += pDesk->m_GameData.m_mjHuType.byHuJueZhang;
		SetAHupaiType(HUPAI_TYPE_HuJueZhang,m_byHuType);//
	}

	if(pDesk->m_GameData.m_mjHuType.byPengPengHu !=255 && CheckPengPengHu())//������
	{
		fan += pDesk->m_GameData.m_mjHuType.byPengPengHu;
		SetAHupaiType(HUPAI_TYPE_PengPengHu,m_byHuType);//
	}

	if(pDesk->m_GameData.m_mjHuType.bySanSeSanBuGao !=255 && CheckSanSeSanBuGao())//��ɫ������
	{
		fan += pDesk->m_GameData.m_mjHuType.bySanSeSanBuGao;
		SetAHupaiType(HUPAI_TYPE_SanSeSanBuGao,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byWuMenQi !=255 && CheckWuMenQi())//������
	{
		fan += pDesk->m_GameData.m_mjHuType.byWuMenQi;
		SetAHupaiType(HUPAI_TYPE_WuMenQi,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byQuanQiuRen !=255 && CheckQuanQiuRen())//ȫ����
	{
		fan += pDesk->m_GameData.m_mjHuType.byQuanQiuRen;
		SetAHupaiType(HUPAI_TYPE_QuanQiuRen,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byShuangAnGang !=255 && CheckShuangAnKe())//˫����
	{
		fan += pDesk->m_GameData.m_mjHuType.byShuangAnGang;
		SetAHupaiType(HUPAI_TYPE_ShuangAnGang,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byShuangJianKe !=255 && CheckShuangJianKe())//˫����
	{
		fan += pDesk->m_GameData.m_mjHuType.byShuangJianKe;
		SetAHupaiType(HUPAI_TYPE_ShuangJianKe,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byHuaLong !=255 && CheckHuaLong())//����
	{
		fan += pDesk->m_GameData.m_mjHuType.byHuaLong;
		SetAHupaiType(HUPAI_TYPE_HuaLong,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byTuiBuDao !=255 && CheckTuiBuDao())//�Ʋ���
	{
		fan += pDesk->m_GameData.m_mjHuType.byTuiBuDao;
		SetAHupaiType(HUPAI_TYPE_TuiBuDao,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.bySSSTongShun !=255 && CheckSanSeSanTongShun())//��ɫ��ͬ˳
	{
		fan += pDesk->m_GameData.m_mjHuType.bySSSTongShun;
		SetAHupaiType(HUPAI_TYPE_SanSeSanTongShun,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.bySSSJieGao !=255 && CheckSanSeSanJieGao())//��ɫ���ڸ�
	{
		fan += pDesk->m_GameData.m_mjHuType.bySSSJieGao;
		SetAHupaiType(HUPAI_TYPE_SanSeSanJieGao,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byDaYuWu !=255 && CheckDaYuWu())//������
	{
		fan += pDesk->m_GameData.m_mjHuType.byDaYuWu;
		SetAHupaiType(HUPAI_TYPE_DaYuWu,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byXiaoYuWu !=255 && CheckXiaoYuWu())//С����
	{
		fan += pDesk->m_GameData.m_mjHuType.byXiaoYuWu;
		SetAHupaiType(HUPAI_TYPE_XiaoYuWu,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.bySanFengKe !=255 && CheckSanFengKe())//�����
	{
		fan += pDesk->m_GameData.m_mjHuType.bySanFengKe;
		SetAHupaiType(HUPAI_TYPE_SanFengKe,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byQiangLong !=255 && CheckQingLong())//����
	{
		fan += pDesk->m_GameData.m_mjHuType.byQiangLong;
		SetAHupaiType(HUPAI_TYPE_QingLong,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.bySSShuangLongHui !=255 && CheckSanSeShuangLongHui())//��ɫ˫����
	{
		fan += pDesk->m_GameData.m_mjHuType.bySSShuangLongHui;
		SetAHupaiType(HUPAI_TYPE_SanSeShuangLongHui,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byYiSeSanBuGao !=255 && CheckYiSeSanBuGao())//һɫ������
	{
		fan += pDesk->m_GameData.m_mjHuType.byYiSeSanBuGao;
		SetAHupaiType(HUPAI_TYPE_YiSeSanBuGao,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byQuanDaiWu !=255 && CheckQuanDaiWu())//ȫ����
	{
		fan += pDesk->m_GameData.m_mjHuType.byQuanDaiWu;
		SetAHupaiType(HUPAI_TYPE_QuanDaiWu,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.bySanTongKe !=255 && CheckSanTongKe())//��ͬ��
	{
		fan += pDesk->m_GameData.m_mjHuType.bySanTongKe;
		SetAHupaiType(HUPAI_TYPE_SanTongKe,m_byHuType);//
	}
/////////////////
	if(pDesk->m_GameData.m_mjHuType.bySanAnKe !=255 && CheckSanAnKe())//������
	{
		fan += pDesk->m_GameData.m_mjHuType.bySanAnKe;
		SetAHupaiType(HUPAI_TYPE_SanAnKe,m_byHuType);//
	}
	//CheckPingHuData();//�Ȼ�ȡ�����

	if(pDesk->m_GameData.m_mjHuType.byQuanShuangKe !=255 && CheckQuanShuangKe())//ȫ˫��
	{
		fan += pDesk->m_GameData.m_mjHuType.byQuanShuangKe;
		SetAHupaiType(HUPAI_TYPE_QuanShuangKe,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byYiSSTongShun !=255 && CheckYiSeSanTongShun())//һɫ��ͬ˳
	{
		fan += pDesk->m_GameData.m_mjHuType.byYiSSTongShun;
		SetAHupaiType(HUPAI_TYPE_YiSeSanTongShun,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byYiSSJieGao !=255 && CheckYiSeSanJieGao())//һɫ���ڸ�
	{
		fan += pDesk->m_GameData.m_mjHuType.byYiSSJieGao;
		SetAHupaiType(HUPAI_TYPE_YiSeSanJieGao,m_byHuType);//
	}	
//
	if(pDesk->m_GameData.m_mjHuType.byYiSeSiBuGao !=255 && CheckYiSeSiBuGao())//һɫ�Ĳ���
	{
		fan += pDesk->m_GameData.m_mjHuType.byYiSeSiBuGao;
		SetAHupaiType(HUPAI_TYPE_YiSeSiBuGao,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byHunYaoJiu !=255 && CheckHunYaoJiu())//���۾�
	{
		fan += pDesk->m_GameData.m_mjHuType.byHunYaoJiu;
		SetAHupaiType(HUPAI_TYPE_HunYaoJiu,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byYSSiTongShun !=255 && CheckYiSeSiTongShun())//һɫ��ͬ˳
	{
		fan += pDesk->m_GameData.m_mjHuType.byYSSiTongShun;
		SetAHupaiType(HUPAI_TYPE_YiSeSiTongShun,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byYSSiJieGao !=255 && CheckYiSeSiJieGao())//һɫ�Ľڸ�
	{
		fan += pDesk->m_GameData.m_mjHuType.byYSSiJieGao;
		SetAHupaiType(HUPAI_TYPE_YiSeSiJieGao,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byQingYaoJiu !=255 && CheckQingYaoJiu())//���۾�
	{
		fan += pDesk->m_GameData.m_mjHuType.byQingYaoJiu;
		SetAHupaiType(HUPAI_TYPE_QingYaoJiu,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byXiaoSiXi !=255 && CheckXiaoSiXi())//С��ϲ
	{
		fan += pDesk->m_GameData.m_mjHuType.byXiaoSiXi;
		SetAHupaiType(HUPAI_TYPE_XiaoSiXi,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byXiaoSanYuan !=255 && CheckXiaoSanYuan())//С��Ԫ
	{
		fan += pDesk->m_GameData.m_mjHuType.byXiaoSanYuan;
		SetAHupaiType(HUPAI_TYPE_XiaoSanYuan,m_byHuType);//
	}

	if(pDesk->m_GameData.m_mjHuType.bySiAnKe !=255 && CheckSiAnKe())//�İ���
	{
		fan += pDesk->m_GameData.m_mjHuType.bySiAnKe;
		SetAHupaiType(HUPAI_TYPE_SiAnKe,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byYSShuangLongHui !=255 && CheckYiSeShuangLongHui())//һɫ˫����
	{
		fan += pDesk->m_GameData.m_mjHuType.byYSShuangLongHui;
		SetAHupaiType(HUPAI_TYPE_YiSeShuangLongHui,m_byHuType);//
	}	
	if(pDesk->m_GameData.m_mjHuType.byDaSiXi !=255 && CheckDaSiXi())//����ϲ
	{
		fan += pDesk->m_GameData.m_mjHuType.byDaSiXi;
		SetAHupaiType(HUPAI_TYPE_DaSiXi,m_byHuType);//
	}
	if(pDesk->m_GameData.m_mjHuType.byDaSanYuan !=255 && CheckDaSanYuan())//����Ԫ
	{
		fan += pDesk->m_GameData.m_mjHuType.byDaSanYuan;
		SetAHupaiType(HUPAI_TYPE_DaSanYuan,m_byHuType);//
	}	
	if(fan>m_iMaxFan|| (m_iMaxFan ==0 && fan==0))
	{
		m_iMaxFan = fan;
		return true;
	}
	return false;
}
///���ƣ�MakePingHu
///������ƽ������
///@param csnum ������, hupaistruct ���ƽṹ, FengShunZi �����ܷ���˳��, JianShunZi �����Ƿ�����˳��
///@return true �����ƽ���� false �������ƽ��
bool CCheckHuPai::MakePingHu(CheckHuStruct &PaiData,PingHuStruct &hupaistruct,int csnum)
{
	hupaistruct.Init();

	if(PaiData.GetAllPaiCount()==0)//�Ѿ�����
	{
		return true;
	}
	if(csnum >HAND_CARD_NUM)
	{
		csnum=0;
	}
	char strbug[500];

	///��¼ÿ���Ƶ�����
	BYTE painum[HAND_CARD_NUM];
	memset(painum,0,sizeof(painum));
	for(int i=0;i<PaiData.conut;i++)
	{
		painum[i] = PaiData.data[i][1];
	}
	//���������(���Ʋ������˳��)
	BYTE pai[4],jnum=0;
	bool jing[4];
	memset(pai,255,sizeof(pai));
	memset(jing,0,sizeof(jing));

	///����
	if(!pDesk->m_GameData.m_mjAction.bChiFeng)
	{
		for(int i=0;i<PaiData.conut;i++)
		{
			if(PaiData.data[i][0]< CMjEnum::MJ_TYPE_FD  || PaiData.data[i][0]> CMjEnum::MJ_TYPE_FB)//����
				continue;
			if(PaiData.data[i][1]==255)
				continue;
			if(PaiData.data[i][1]>=3)
			{
				PaiData.data[i][1]-=3;
				//���������
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = PaiData.data[i][0];
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			if(PaiData.data[i][1]>0)
			{
				if(csnum < (3-PaiData.data[i][1]))
				{	
					ReSetPaiNum(painum,PaiData);
					OutputDebugString("new:ƽ����⣺���������--�����޷����");
					return false;
				}
				csnum -= (3-PaiData.data[i][1]);
				//���������
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = PaiData.data[i][0];
				jnum = 3-PaiData.data[i][1];
				for(int j=0;j<jnum;++j)
				{
					jing[j] = true;
				}
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);		
				PaiData.data[i][1]=0;
			}
		}
	}
	else
	{
		PaiStruct feng[4];

		for(int i=0;i<PaiData.conut;i++)
		{
			if(PaiData.data[i][0]< CMjEnum::MJ_TYPE_FB  || PaiData.data[i][0]> CMjEnum::MJ_TYPE_FD || PaiData.data[i][1]==255)//����
				continue;
			if(PaiData.data[i][1] >= 3)
			{
				PaiData.data[i][1] -= 3;
				//���������
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = PaiData.data[i][0];

				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
		}
		for(int i=0;i<PaiData.conut;i++)
		{
			if(PaiData.data[i][0]< CMjEnum::MJ_TYPE_FD  || PaiData.data[i][0]> CMjEnum::MJ_TYPE_FB || PaiData.data[i][1]==255 || PaiData.data[i][1] == 0) //����
				continue;
			feng[PaiData.data[i][0]-CMjEnum::MJ_TYPE_FD].count = PaiData.data[i][1];
			feng[PaiData.data[i][0]-CMjEnum::MJ_TYPE_FD].pai = PaiData.data[i][0];
		}

		MaoPaoSort(feng,4);
		sprintf(strbug,"�������ƣ�%d  %d  %d  %d ",feng[0].count,feng[1].count,feng[2].count,feng[3].count);
		OutputDebugString(strbug);

		while(feng[0].count>0 && feng[1].count>0 && feng[2].count>0)
		{
			pai[0] = pai[1] = pai[2]=feng[0].pai;
			hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);

			feng[0].count-=1;
			feng[1].count-=1;
			feng[2].count-=1;

			MaoPaoSort(feng,4);
		}
		///���з��ƣ���Ҫ����������������ˣ�10,11,20,21,22
		if(feng[0].count > 0)
		{
			if(feng[0].count==1 && feng[0].count==0 && csnum>=2)
			{
				csnum -= 2;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2]=feng[0].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			else if(feng[0].count==1 && feng[1].count==1 && csnum>=1)
			{
				BYTE fpai=CMjEnum::MJ_TYPE_FD;
				for(int i=CMjEnum::MJ_TYPE_FD;i<=CMjEnum::MJ_TYPE_FB;i++)
				{
					if(i!=feng[0].pai && i!=feng[1].pai)
						fpai = i;
				}
				csnum -= 1;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = feng[0].pai;
				pai[2] = fpai;
				hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);
			}
			else if(feng[0].count==2 && feng[1].count==0 && csnum>=1)
			{
				csnum -= 1;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2]=feng[0].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			else if(feng[0].count==2 && feng[1].count==1 && csnum>=3)
			{
				csnum -= 3;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2]=feng[0].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);

				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2]=feng[1].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			else if(feng[0].count==2 && feng[1].count==2 && csnum>=2)
			{
				csnum -= 2;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2]=feng[0].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);

				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2]=feng[1].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			else 
			{
				ReSetPaiNum(painum,PaiData);

				sprintf(strbug,"new:ƽ����� �������� �����������--��������ʧ��  ");
				OutputDebugString(strbug);

				return  false;
			}
		}

	}
	///���ƣ��з��ף�
	if(!pDesk->m_GameData.m_mjAction.bChiJian)
	{
		for(int i=0;i<PaiData.conut;i++)
		{
			if(PaiData.data[i][0]< CMjEnum::MJ_TYPE_ZHONG  || PaiData.data[i][0]> CMjEnum::MJ_TYPE_BAI)//����
				continue;
			if(PaiData.data[i][1]==255)
				continue;
			if(PaiData.data[i][1]>=3)
			{
				PaiData.data[i][1]-=3;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = PaiData.data[i][0];
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			if(PaiData.data[i][1]>0)
			{
				if(csnum < (3-PaiData.data[i][1]))
				{			
					ReSetPaiNum(painum,PaiData);
					OutputDebugString("new:ƽ����⣺���������--�����޷����");
					return false;
				}
				csnum -= (3-PaiData.data[i][1]);
				//���������
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = PaiData.data[i][0];
				jnum = 3-PaiData.data[i][1];
				for(int j=0;j<jnum;++j)
				{
					jing[j] = true;
				}
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
				PaiData.data[i][1]=0;
			}
		}
	}
	else
	{
		for(int i=0;i<PaiData.conut;i++)
		{
			if(PaiData.data[i][0]< CMjEnum::MJ_TYPE_ZHONG  || PaiData.data[i][0]> CMjEnum::MJ_TYPE_BAI)//����
				continue;
			if(PaiData.data[i][1]==255)
				continue;
			if(PaiData.data[i][1]>=3)
			{
				PaiData.data[i][1]-=3;
				//���������
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = PaiData.data[i][0];
				pai[1] = PaiData.data[i][0];
				pai[2] = PaiData.data[i][0];
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
		}
		
		PaiStruct jian[3];
		for(int i=0;i<PaiData.conut;i++)
		{
			if(PaiData.data[i][0]< CMjEnum::MJ_TYPE_ZHONG  || PaiData.data[i][0]> CMjEnum::MJ_TYPE_BAI|| PaiData.data[i][1] == 0) //����
				continue;
			jian[PaiData.data[i][0]-CMjEnum::MJ_TYPE_ZHONG].count = PaiData.data[i][1];
			jian[PaiData.data[i][0]-CMjEnum::MJ_TYPE_ZHONG].pai = PaiData.data[i][0];
		}
		MaoPaoSort(jian,3);
		while(jian[0].count>0 && jian[1].count>0 && jian[2].count>0)
		{
			memset(pai,255,sizeof(pai));
			memset(jing,0,sizeof(jing));
			pai[0] = pai[1] = pai[2] = jian[0].pai;
			hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);

			jian[0].count-=1;
			jian[1].count-=1;
			jian[2].count-=1;

			MaoPaoSort(jian,3);
		}
		///���м��ƣ���Ҫ����������������ˣ�10,11,20,21,22
		if(jian[0].count > 0)
		{
			if(jian[0].count==1 && jian[0].count==0 && csnum>=2)
			{
				csnum -= 2;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = jian[0].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			else if(jian[0].count==1 && jian[1].count==1 && csnum>=1)
			{
				BYTE jpai=CMjEnum::MJ_TYPE_ZHONG;
				for(int i=CMjEnum::MJ_TYPE_ZHONG;i<=CMjEnum::MJ_TYPE_BAI;i++)
				{
					if(i!=jian[0].pai && i!=jian[1].pai)
						jpai = i;
				}
				csnum -= 1;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = jian[0].pai;
				pai[2] = jpai;
				hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);
			}
			else if(jian[0].count==2 && jian[1].count==0 && csnum>=1)
			{
				csnum -= 1;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = jian[0].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			else if(jian[0].count==2 && jian[1].count==1 && csnum>=3)
			{
				csnum -= 3;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = jian[0].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);

				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = jian[1].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			else if(jian[0].count==2 && jian[1].count==2 && csnum>=2)
			{
				csnum -= 2;
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = jian[0].pai;
				pai[3] = 255;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);

				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = pai[1] = pai[2] = jian[1].pai;
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
			}
			else 
			{
				ReSetPaiNum(painum,PaiData);
				sprintf(strbug,"new:ƽ����⣺���������--��������ʧ��  ");
				OutputDebugString(strbug);

				return  false;
			}
		}
		
	}
	//�����ͬ��
	for(int i=0;i<PaiData.conut;i++)
	{
		if(PaiData.data[i][1] == 0 || PaiData.data[i][0] == 255)
			continue;
		if(PaiData.data[i][0]>30)
		{//���ϻ������ƣ����ܺ���
			ReSetPaiNum(painum,PaiData);
			sprintf(strbug,"new:ƽ����⣺���������--ʣ�����ƣ�����ʧ��  ");
			OutputDebugString(strbug);
			return false;
		}

		if(PaiData.data[i][1]>=3)
		{
			PaiData.data[i][1] -= 3;
			//���������
			memset(pai,255,sizeof(pai));
			memset(jing,0,sizeof(jing));
			pai[0] = pai[1] = pai[2] = PaiData.data[i][0];
			hupaistruct.AddData(TYPE_AN_KE,pai,jing);

			while(PaiData.data[i][1]>=3)
			{
				pai[0] = pai[1] = pai[2] = PaiData.data[i][0];
				hupaistruct.AddData(TYPE_AN_KE,pai,jing);
				PaiData.data[i][1] -= 3;
			}
		}
		BYTE num=PaiData.data[i][1];	// ��ǰ�Ƶ�����

		if(num == 0)
			continue;
		BYTE num1=num,num2=num;			// ��������
		if(CheckTongSe(PaiData.data[i][0],PaiData.data[i][0]+1))//��һ��������
		{
			if(PaiData.GetPaiCount(PaiData.data[i][0]+1) >= num)
			{
				num1 = 0;
			}
			else
			{
				num1 = num - PaiData.GetPaiCount(PaiData.data[i][0]+1);
			}
		}
		if(CheckTongSe(PaiData.data[i][0],PaiData.data[i][0]+2))//�����������
		{
			if(PaiData.GetPaiCount(PaiData.data[i][0]+2) >= num)
			{
				num2 = 0;
			}
			else
			{
				num2 = num - PaiData.GetPaiCount(PaiData.data[i][0]+2);
			}
		}
		//����������Ҫ�Ĳ����ٻ�����˳����Ҫ�Ĳ�����
		bool ke = false;
		if((3 -num ) < (num1+num2))
		{
			ke = true;
		}
		// �������������γ�����˳��,�����Ƶ����9, ���������ͬ����
		if(csnum<(num1+num2) || PaiData.data[i][0]%10 == CMjEnum::MJ_TYPE_PAI_DIAN_9 || ke)
		{
			csnum -= (3- num);
			if(csnum < 0) 
			{// �ղ�������
				ReSetPaiNum(painum,PaiData);
				sprintf(strbug,"new:ƽ����⣺���������--��ͬ���޷�������  pai=%d csnum=%d num=%d num1=%d num2=%d",PaiData.data[i][0],csnum,num,num1,num2);
				OutputDebugString(strbug);
				return  false;
			}	
			//��ɿ���
			PaiData.data[i][1] -= num;
			//���������
			memset(pai,255,sizeof(pai));
			memset(jing,0,sizeof(jing));
			pai[0] = pai[1] = pai[2] = PaiData.data[i][0];
			jnum = 3- num;//��Ҫ���Ƶ�����
			for(int j=0;j<jnum;++j)
			{
				jing[j] = true;
			}
			hupaistruct.AddData(TYPE_AN_KE,pai,jing);

		}
		else//��˳��
		{
			BYTE tempJing = csnum;//��ǰʣ���¼����
			csnum  -= (num1 + num2);
			if(csnum < 0) 
			{// �ղ���˳��
				ReSetPaiNum(painum,PaiData);
				sprintf(strbug,"new:ƽ����⣺���������--��ͬ���޷���˳��  pai=%d csnum=%d num=%d num1=%d num2=%d",PaiData.data[i][0],csnum,num,num1,num2);
				OutputDebugString(strbug);
				return  false;
			}

			if( PaiData.data[i][0]%10 == CMjEnum::MJ_TYPE_PAI_DIAN_8)
			{//û�д��������ֻ�����߸�ȡһ��
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = PaiData.data[i][0]-1;
				pai[1] = PaiData.data[i][0];
				pai[2] = PaiData.data[i][0]+1;
				if(tempJing>0 && PaiData.GetPaiCount(pai[0])==0)
				{
					tempJing -= 1;
					jing[0] = true;
				}
				if(tempJing>0 && PaiData.GetPaiCount(pai[2])==0)
				{
					tempJing -= 1;
					jing[2] = true;
				}
				hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);

				if(num>1)//��Ҫ��һ��˳��
				{
					memset(jing,0,sizeof(jing));
					if(tempJing>0 && PaiData.GetPaiCount(pai[0])==0)
					{
						tempJing -= 1;
						jing[0] = true;
					}
					if(tempJing>0 && PaiData.GetPaiCount(pai[2])==0)
					{
						tempJing -= 1;
						jing[2] = true;
					}
					hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);
				}
			}
			else if( PaiData.data[i][0]%10 == CMjEnum::MJ_TYPE_PAI_DIAN_9)
			{//û�д�һ�ʹ��������ֻ��ȥǰ��������
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = PaiData.data[i][0]-2;
				pai[1] = PaiData.data[i][0]-1;
				pai[2] = PaiData.data[i][0];
				if(tempJing>0 && PaiData.GetPaiCount(pai[0])==0)
				{
					tempJing -= 1;
					jing[0] = true;
				}
				if(tempJing>0 && PaiData.GetPaiCount(pai[1])==0)
				{
					tempJing -= 1;
					jing[1] = true;
				}
				hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);
				if(num>1)//��Ҫ��һ��˳��
				{
					memset(jing,0,sizeof(jing));
					if(tempJing>0 && PaiData.GetPaiCount(pai[0])==0)
					{
						tempJing -= 1;
						jing[0] = true;
					}
					if(tempJing>0 && PaiData.GetPaiCount(pai[1])==0)
					{
						tempJing -= 1;
						jing[1] = true;
					}
					hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);
				}
			}
			else
			{
				memset(pai,255,sizeof(pai));
				memset(jing,0,sizeof(jing));
				pai[0] = PaiData.data[i][0];
				pai[1] = PaiData.data[i][0]+1;
				pai[2] = PaiData.data[i][0]+2;
				if(tempJing>0 && PaiData.GetPaiCount(pai[1])==0)
				{
					tempJing -= 1;
					jing[1] = true;
				}
				if(tempJing>0 && PaiData.GetPaiCount(pai[2])==0)
				{
					tempJing -= 1;
					jing[2] = true;
				}
				hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);
				if(num>1)//��Ҫ��һ��˳��
				{
					memset(jing,0,sizeof(jing));
					if(tempJing>0 && PaiData.GetPaiCount(pai[1])==0)
					{
						tempJing -= 1;
						jing[1] = true;
					}
					if(tempJing>0 && PaiData.GetPaiCount(pai[2])==0)
					{
						tempJing -= 1;
						jing[2] = true;
					}
					hupaistruct.AddData(TYPE_SHUN_ZI,pai,jing);
				}
			}

			BYTE temp = 0;
			if(CheckTongSe(PaiData.data[i][0],PaiData.data[i][0]+1))//��һ��������,���ô�һ����������
			{
				temp = PaiData.GetPaiCount(PaiData.data[i][0]+1);
				if(temp>=num)
				{
					PaiData.SetPaiCount(PaiData.data[i][0]+1,temp-num);
				}
				else
				{
					PaiData.SetPaiCount(PaiData.data[i][0]+1,0);
				}
			}
			if(PaiData.data[i][0],PaiData.data[i][0]+2)//����������ڣ����ô������������
			{
				temp = PaiData.GetPaiCount(PaiData.data[i][0]+2);
				if(temp>=num)
				{
					PaiData.SetPaiCount(PaiData.data[i][0]+2,temp-num);
				}
				else
				{
					PaiData.SetPaiCount(PaiData.data[i][0]+2,0);
				}
			}
			PaiData.SetPaiCount(PaiData.data[i][0],0);
		}
	}

	///��������
	if(csnum>=3)
	{
		csnum -= 3;
		memset(pai,255,sizeof(pai));
		memset(jing,0,sizeof(jing));
		pai[0] = pai[1] = pai[2] = pDesk->m_UserData.m_StrJing.byPai[0];
		jing[0] = jing[1] = jing[2] = true;
		hupaistruct.AddData(TYPE_JING_KE,pai,jing);

		if(csnum>=3)
		{
			csnum -= 3;
			memset(pai,255,sizeof(pai));
			memset(jing,0,sizeof(jing));
			pai[0] = pai[1] = pai[2] = pDesk->m_UserData.m_StrJing.byPai[0];
			jing[0] = jing[1] = jing[2] = true;
			hupaistruct.AddData(TYPE_JING_KE,pai,jing);
		}
	}
	ReSetPaiNum(painum,PaiData);

	return true;
}

///���ƣ�ReSetPaiNum
///�������ָ�����,�Ѽ������ƻ�ԭ
///@param num[] ��ʱ�����ԭ������
///@return 
void CCheckHuPai::ReSetPaiNum(BYTE num[],CheckHuStruct &TempPai)
{
	for(int i=0;i<TempPai.conut;i++)
	{
		 TempPai.data[i][1] = num[i];
	}
}

///���ƣ�CheckTongSe
///��������������Ƿ�ͬһ��ɫ
///@param pai1, pai2 Ҫ����������
///@return true �ǣ� false ����
bool CCheckHuPai::CheckTongSe(BYTE pai1,BYTE pai2)
{
	int count=0;
	if(pai2%10==0 || pai1/10 != pai2/10)
		return false;
	return true;
}

///���ƣ�CheckLianPai
///��������������Ƿ�����,num �����ţ�1���������ţ�2������������
///@param pai1, pai2 Ҫ�����ƣ� num ���������
///@return true �����ţ�flse ��������
bool CCheckHuPai::CheckLianPai(BYTE pai1,BYTE pai2,BYTE num)
{
	if(!CheckTongSe(pai1,pai2))
		return false;
	if(num==1)
	{
		if(pai1 == pai2+1 || pai1 == pai2-1)
		{
			return true;
		}
	}
	else if(num == 2)
	{
		if(pai1 == pai2+1 || pai1 == pai2-1 || pai1 == pai2+2 || pai1 == pai2-2)
		{
			if(!pDesk->m_GameData.m_mjAction.bChiFeng && (pai1>=CMjEnum::MJ_TYPE_FD && pai1<=CMjEnum::MJ_TYPE_FB || pai2>=CMjEnum::MJ_TYPE_FD && pai2<=CMjEnum::MJ_TYPE_FB))
				return false;
			if(!pDesk->m_GameData.m_mjAction.bChiJian && (pai1>=CMjEnum::MJ_TYPE_ZHONG && pai1<=CMjEnum::MJ_TYPE_BAI || pai2>=CMjEnum::MJ_TYPE_ZHONG && pai2<=CMjEnum::MJ_TYPE_BAI))
				return false;
			if(pai2>=CMjEnum::MJ_TYPE_FD && pai2<=CMjEnum::MJ_TYPE_FB)///��������
			{
				if(pai1>CMjEnum::MJ_TYPE_FB || pai1<CMjEnum::MJ_TYPE_FD)
					return false;
			}
			else if(pai2>=CMjEnum::MJ_TYPE_ZHONG && pai2<=CMjEnum::MJ_TYPE_BAI)///�з���
			{
				if(pai1>CMjEnum::MJ_TYPE_BAI || pai1<CMjEnum::MJ_TYPE_ZHONG)
					return false;
			}
			return true;
		}
	}
	return false;
}

///���ƣ�SetAHupaiType
///���������һ�ֺ�������
///@param   type Ҫ��ӵ����� , hupai[] ������������
///@return 
void CCheckHuPai::SetAHupaiType(BYTE type,BYTE hupai[])
{
	if(CheckHupaiType(type,hupai))//���˾Ͳ������
		return;
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		if(hupai[i] == 255)
		{
			hupai[i] = type;
			break;
		}
	}
}

///���ƣ�ReSetAHupaiType
///����������һ�ֺ�������
///@param type Ҫɾ�������� , hupai[] ������������
///@return 
void CCheckHuPai::ReSetAHupaiType(BYTE type,BYTE hupai[])
{
	int index = 0;
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		if(hupai[i] == type)
		{
			hupai[i] = 255;
			break;
		}
	}
	//ȥ���м���ЧԪ��
	BYTE temp[MAX_HUPAI_TYPE];
	memset(temp,255,sizeof(temp));
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		if(hupai[i] != 255)
		{
			temp[index] = hupai[i];
			index++;
		}
	}
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		hupai[i] = temp[i];
	}
}

///���ƣ�ClearHupaiType
///������������к�������
///@param type Ҫɾ�������� , hupai[] ������������
///@return 
void CCheckHuPai::ClearHupaiType(BYTE hupai[])
{
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		hupai[i] = 255;
	}
}

///���ƣ�CheckHupaiType
///����������Ƿ����ĳ�ֺ�������
///@param type Ҫ�������� , hupai[] ������������
///@return true ���ڣ�flse ������
bool CCheckHuPai::CheckHupaiType(BYTE type,BYTE hupai[])
{
	int index = 0;
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		if(hupai[i] == type && type!=255)
		{
			return true;
		}
	}
	return false;
}

///���ƣ�InitData
///��������ʼ������
///@param 
///@return 
void CCheckHuPai::InitData()
{
	m_byStation = 0;///��ǰ�����ҵ�λ��
	hupaistruct.Init();
	HuTempData.Init();	//���Ƽ����ʱ����
	NoJingHuTempData.Init();///���Ƽ����ʱ����(�޲���)	
	m_byJingNum = 0;  //��������
	//�����������ڵ���
	m_byLastPai = 255;
	ClearPingHuData();
}

///���ƣ�ClearPingHuData
///�����������������
///@param 
///@return 
void CCheckHuPai::ClearPingHuData()
{		
	m_byJiang = 255;			//��������
	m_byShunNum = 0;			//˳�ӵĸ���
	memset(m_byShunData,255,sizeof(m_byShunData));//˳�ӵ�����	
	m_byKeziNum = 0;			//���ӵĸ���	
	memset(m_byKeziData,255,sizeof(m_byKeziData));//���ӵ�����
	m_byGangNum = 0;			//���Ƶĸ���
	memset(m_byGangData,255,sizeof(m_byGangData));	//���Ƶ�����
	m_byChiNum = 0;				//���Ƶĸ���
	m_byPengNum = 0;			//���Ƶĸ���
}

///���ƣ�CheckPingHuData
///���������ƽ��������ϣ����ƣ��ԣ����ܣ�˳�ӣ����̵�
///@param 
///@return 
void CCheckHuPai::CheckPingHuData()
{
	char strbuf[500];
	ClearPingHuData();

	sprintf(strbuf,"���Ʋ��ԣ�CheckPingHuData ��ʼ m_byGangNum=%d  m_byShunNum=%d m_byKeziNum=%d",m_byGangNum,m_byShunNum,m_byKeziNum);
	OutputDebugString(strbuf);

	BYTE num = 0;
	for(int i=0;i<hupaistruct.count;i++)
	{
		switch(hupaistruct.byType[i])
		{
		case TYPE_JINGDIAO_JIANG:
		case TYPE_JIANG_PAI:
			{
				m_byJiang = hupaistruct.data[i][0];
				sprintf(strbuf,"���Ʋ��ԣ�  ���� m_byGangNum=%d  m_byShunNum=%d m_byKeziNum=%d ��%d",m_byGangNum,m_byShunNum,m_byKeziNum,m_byJiang);
				OutputDebugString(strbuf);
			}
			break;
		case TYPE_SHUN_ZI:
			{
				m_byShunData[m_byShunNum] = hupaistruct.data[i][0];
				m_byShunNum++;
				sprintf(strbuf,"���Ʋ��ԣ� ˳�� m_byGangNum=%d  m_byShunNum=%d m_byKeziNum=%d ��%d",m_byGangNum,m_byShunNum,m_byKeziNum,hupaistruct.data[i][0]);
				OutputDebugString(strbuf);
			}
			break;
		case TYPE_AN_KE:
			{
				m_byKeziData[m_byKeziNum] = hupaistruct.data[i][0];
				m_byKeziNum++;
				sprintf(strbuf,"���Ʋ��ԣ�  ���� m_byGangNum=%d  m_byShunNum=%d m_byKeziNum=%d ��%d",m_byGangNum,m_byShunNum,m_byKeziNum,hupaistruct.data[i][0]);
				OutputDebugString(strbuf);

			}
			break;
		default:
			break;
		}
	}

	sprintf(strbuf,"���Ʋ��ԣ� m_byGangNum=%d  m_byShunNum=%d m_byKeziNum=%d",m_byGangNum,m_byShunNum,m_byKeziNum);
	OutputDebugString(strbuf);

	BYTE pai[4];
	memset(pai,255,sizeof(pai));
	for(int i=0;i<5;i++)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		for(int j=0;j<4;++j)
		{
			pai[j] = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[j];
		}
		switch(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType)
		{
		case ACTION_CHI://���ƶ���
			{
				m_byShunData[m_byShunNum] = pai[0];
				m_byShunNum++;
				m_byChiNum++;
				sprintf(strbuf,"���Ʋ��ԣ�   ���ƣ�%d %d %d",pai[0],pai[1],pai[2]);
				OutputDebugString(strbuf);
			}
			break;
		case ACTION_PENG://���ƶ���
			{
				m_byKeziData[m_byKeziNum] = pai[0];
				m_byKeziNum++;
				m_byPengNum++;
				sprintf(strbuf,"���Ʋ��ԣ�   ���ƣ�%d %d %d   ������ = %d  ����%d ",pai[0],pai[1],pai[2],m_byKeziNum,m_byKeziData[m_byKeziNum]);
				OutputDebugString(strbuf);
			}
			break;
		case ACTION_AN_GANG://���ܶ���
			{
				m_byGangData[m_byGangNum][1] = pai[0];
				m_byGangData[m_byGangNum][0] = ACTION_AN_GANG;
				m_byGangNum++;
				sprintf(strbuf,"���Ʋ��ԣ�   ���ܣ�%d  %d  %d %d",pai[0],pai[1],pai[2],pai[3]);
				OutputDebugString(strbuf);
			}
			break;
		case ACTION_BU_GANG://���ܶ���
			{
				m_byGangData[m_byGangNum][1] = pai[0];
				m_byGangData[m_byGangNum][0] = ACTION_BU_GANG;
				m_byGangNum++;
				sprintf(strbuf,"���Ʋ��ԣ�   ���ܣ�%d  %d  %d %d",pai[0],pai[1],pai[2],pai[3]);
				OutputDebugString(strbuf);
			}
			break;
		case ACTION_MING_GANG://���ܶ���
			{
				m_byGangData[m_byGangNum][1] = pai[0];
				m_byGangData[m_byGangNum][0] = ACTION_MING_GANG;
				m_byGangNum++;
				sprintf(strbuf,"���Ʋ��ԣ�  ���ܣ�%d  %d  %d %d",pai[0],pai[1],pai[2],pai[3]);
				OutputDebugString(strbuf);
			}
			break;
		}
	}
	sprintf(strbuf,"���Ʋ��ԣ�CheckPingHuData ���� m_byGangNum=%d  m_byShunNum=%d m_byKeziNum=%d",m_byGangNum,m_byShunNum,m_byKeziNum);
	OutputDebugString(strbuf);
}


/////////////////��ϸ�������ͼ��///////////////////////////////////////////////////////////////////////////////

/////////////////������ƽ��������ͼ��/////////////////////////////

//ƽ������4��˳�Ӽ�������������ɵĺ��ƣ��ߡ���������Ӱ��ƽ�� 
bool CCheckHuPai::CheckIsPingHu()
{
	if(m_byKeziNum>0 || m_byGangNum>0)//�и��ƻ����
		return false;
	return true;
}
//�����
bool CCheckHuPai::CheckTianHu()
{

	if(m_byStation != pDesk->m_UserData.m_byNtStation )
		return false;
	if(pDesk->m_UserData.IsUserState( m_byStation,USER_STATE_HAVE_OUT) || pDesk->m_UserData.GetGCPCount(m_byStation)>0)
		return false;
	return true;
}
//�غ���
bool CCheckHuPai::CheckDiHu()
{
	if(m_byOutStation != pDesk->m_UserData.m_byNtStation || m_byStation == pDesk->m_UserData.m_byNtStation)
		return false;	
	char strdug[500];
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(pDesk->m_UserData.IsUserState( i,USER_STATE_HAVE_OUT) && i!= pDesk->m_UserData.m_byNtStation)//��ׯ�ҳ�����
			return false;
		if(pDesk->m_UserData.GetGCPCount(i)>0)
			return false;
		if(pDesk->m_UserData.GetOutPaiCount(i)>1)
			return false;
	}
	return true;
}

//���ϻ�������ץ�����Ƴɺ���(����������)�������� 
bool CCheckHuPai::CheckGangKai()
{
	if(m_byStation == m_byOutStation && pDesk->m_UserData.IsUserState(m_byStation,USER_STATE_GANG))
	{
		return true;
	}
	return false;
}
//�ܺ��ڣ��ܺ����
bool CCheckHuPai::CheckGangHouPao()
{
	if(m_byStation != m_byOutStation && pDesk->m_UserData.IsUserState(m_byOutStation,USER_STATE_GANG))
	{
		return true;
	}
	return false;
}

//һ��ߣ���һ�ֻ�ɫ2����ͬ��˳����ɵ��� ,���������ƣ�
bool CCheckHuPai::CheckYiBanGao()
{
	if(m_byShunNum>=2)
	{
		for(int i=0;i<m_byShunNum-1;++i)
		{
			for(int j=i+1;j<m_byShunNum;j++)
			{
				if(m_byShunData[i] == m_byShunData[j] && m_byShunData[i] !=255)
					return true;
			}
		}
	}
	return false;
}

//ϲ��꣺2�ֻ�ɫ2��������ͬ��˳��  ,���������ƣ�
bool CCheckHuPai::CheckXiXiangFeng()
{
	BYTE shunzi[4],num=0,count=0;
	memset(shunzi,255,sizeof(shunzi));
	for(int i=0;i<4;++i)
	{
		shunzi[i] = m_byShunData[i];
	}
	if(m_byShunNum==4)
	{
		for(int i=0;i<m_byShunNum-1;++i)
		{
			for(int j=i+1;j<m_byShunNum;j++)
			{
				if(shunzi[i] == shunzi[j] && shunzi[i] !=0)
				{
					shunzi[i] = 0;
					shunzi[j] = 0;
					count++;
				}
			}
		}
		if(count>=2)
		{
			return true;
		}	
	}
	return false;
}

//������һ�ֻ�ɫ6�������ӵ������� 
bool CCheckHuPai::CheckLianLiu()
{
	if(m_byShunNum>=2)
	{
		for(int i=0;i<m_byShunNum-1;++i)
		{
			for(int j=i+1;j<m_byShunNum;j++)
			{
				if(m_byShunData[i] == m_byShunData[j]+3 && m_byShunData[i] !=255 && CMjRef::CheckIsTongSe(m_byShunData[i],m_byShunData[j]))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//���ٸ���һ�ֻ�ɫ�Ƶ�123��789����˳��  ,���������ƣ�
bool CCheckHuPai::CheckLaoShaoFu()
{
	if(m_byShunNum>=2)
	{
		for(int i=0;i<m_byShunNum-1;++i)
		{
			for(int j=i+1;j<m_byShunNum;j++)
			{
				if(m_byShunData[i]%10 == 1 && m_byShunData[j]%10 == 9  && CMjRef::CheckIsTongSe(m_byShunData[i],m_byShunData[j]))
				{
					return true;
				}
				if(m_byShunData[i]%10 == 9 && m_byShunData[j]%10 == 1  && CMjRef::CheckIsTongSe(m_byShunData[i],m_byShunData[j]))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//�۾ſ̣�3����ͬ��һ���������Ƽ�������ɵĿ���(���) 
bool CCheckHuPai::CheckYaoJiuKe()
{
	for(int i=0;i<m_byKeziNum;++i)
	{
		if((m_byKeziData[i]%10 == 1 || m_byKeziData[i]%10 == 9 ) && m_byKeziData[i] < CMjEnum::MJ_TYPE_FD)
			return true;
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if((m_byGangData[i][1]%10 == 1 || m_byGangData[i][1]%10 == 9 ) && m_byGangData[i][1]<CMjEnum::MJ_TYPE_FD)
			return true;
	}
	return false;
}

//���ܣ��Լ��а��̣������˴����һ����ͬ���ƿ��ܣ����Լ�ץ��һ��������������ͬ���ƿ��� 
bool CCheckHuPai::CheckMingGang()
{
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] == ACTION_MING_GANG || m_byGangData[i][1] == ACTION_BU_GANG)
			return true;
	}
	return false;
}

//ȱһ�ţ�������ȱ��һ�ֻ�ɫ������ 
bool CCheckHuPai::CheckQueYiMen()
{
	bool hua[4];
	BYTE num = 0;
	memset(hua,0,sizeof(hua));
	if(m_byJiang!=255)
	{
		hua[CMjRef::GetHuaSe(m_byJiang)] = true;
	}
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i] != 255 && m_byShunData[i]<CMjEnum::MJ_TYPE_FD)
		{
			hua[CMjRef::GetHuaSe(m_byShunData[i])] = true;
		}
	}
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i] != 255 && m_byKeziData[i]<CMjEnum::MJ_TYPE_FD)
		{
			hua[CMjRef::GetHuaSe(m_byKeziData[i])] = true;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] != 255 && m_byGangData[i][1]<CMjEnum::MJ_TYPE_FD)
		{
			hua[CMjRef::GetHuaSe(m_byGangData[i][1])] = true;
		}
	}
	for(int i=0;i<3;++i)
	{
		if(hua[i])
		{
			num++;
		}
	}
	if(num==2)
	{
		return true;
	}
	return false;
}

//���֣�������û�з硢���� 
bool CCheckHuPai::CheckWuZi()
{
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i] != 255 && m_byKeziData[i]>=CMjEnum::MJ_TYPE_FD )
			return false;
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] != 255 && m_byGangData[i][1]>=CMjEnum::MJ_TYPE_FD )
			return false;
	}
	return true;
}

//���ţ�����123��3��789��7��1233��3��77879��7��Ϊ�š�(δ����������12345��3��56789��6������� )
bool CCheckHuPai::CheckBianZhang()
{
	if(pDesk->m_UserData.GetOutPaiCount(pDesk->m_UserData.m_byNtStation)==0 && m_byChiNum == 0 && m_byPengNum == 0 && m_byGangNum == 0 )
	{//�������
		return false;
	}
	if(m_byLastPai >=CMjEnum::MJ_TYPE_FD || (m_byLastPai%10 != 3 && m_byLastPai%10 != 7))
	{//���ץ�����������ƣ����3,7���ƣ����ǽ���
		return false;
	}
	for(int i=0;i<hupaistruct.count;++i)
	{
		if(hupaistruct.byType[i] != TYPE_SHUN_ZI)
		{
			continue;
		}
		if(hupaistruct.data[i][2]%10 == 3 && hupaistruct.data[i][2] == m_byLastPai)
		{
			return true;
		}
		if(hupaistruct.data[i][0]%10 == 7 && hupaistruct.data[i][0] == m_byLastPai)
		{
			return true;
		}
	}
	return false;
}

//���ţ���2����֮����ơ�4556��5ҲΪ���š�δ����������45567��6���㿲�� 
bool CCheckHuPai::CheckKanZhang()
{
	if(pDesk->m_UserData.GetOutPaiCount(pDesk->m_UserData.m_byNtStation)==0 && m_byChiNum == 0 && m_byPengNum == 0 && m_byGangNum == 0 )
	{//�������
		return false;
	}
	if(m_byLastPai >=CMjEnum::MJ_TYPE_FD )
	{//���ץ������������
		return false;
	}
	for(int i=0;i<hupaistruct.count;++i)
	{
		if(hupaistruct.byType[i] != TYPE_SHUN_ZI)
		{
			continue;
		}
		if(hupaistruct.data[i][1] == m_byLastPai)
		{
			return true;
		}
	}
	return false;
}

//���������������������ɺ� 
bool CCheckHuPai::CheckDanDiaoJiang()
{
	if(pDesk->m_UserData.GetOutPaiCount(pDesk->m_UserData.m_byNtStation)==0 && m_byChiNum == 0 && m_byPengNum == 0 && m_byGangNum == 0 )
	{//�������
		return false;
	}
	for(int i=0;i<hupaistruct.count;++i)
	{
		if(hupaistruct.byType[i] == TYPE_JIANG_PAI || hupaistruct.byType[i] == TYPE_JINGDIAO_JIANG)
		{
			if(hupaistruct.data[i][0] == m_byLastPai)
			{
				return true;
			}
		}
	}
	return false;
}

//���̣����С�������3����ͬ������ɵĿ��� ,���ܲ��㣩
bool CCheckHuPai::CheckJianKe()
{
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i] == CMjEnum::MJ_TYPE_ZHONG || m_byKeziData[i] == CMjEnum::MJ_TYPE_FA  || m_byKeziData[i] == CMjEnum::MJ_TYPE_BAI)
		{
			return true;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] == CMjEnum::MJ_TYPE_ZHONG || m_byGangData[i][1] == CMjEnum::MJ_TYPE_FA  || m_byGangData[i][1] == CMjEnum::MJ_TYPE_BAI)
		{
			return true;
		}
	}
	return false;
}

//Ȧ��̣���Ȧ����ͬ�ķ�� �����ܲ��㣩
bool CCheckHuPai::CheckQuanFengKe()
{
	if(pDesk->m_UserData.m_byQuanFeng == 255)
		return false;
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i] == pDesk->m_UserData.m_byQuanFeng)
		{
			return true;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] == pDesk->m_UserData.m_byQuanFeng)
		{
			return true;
		}
	}
	return false;
}

//�ŷ�̣��뱾�ŷ���ͬ�ķ�� ,���ܲ��㣩
bool CCheckHuPai::CheckMenFengKe()
{


	return false;
}

//��ǰ�壺û�гԡ��������ܣ��ͱ��˴������ 
bool CCheckHuPai::CheckMenQianQing()
{
	if(m_byStation == m_byOutStation)
	{
		return false;
	}
	if(m_byChiNum>0 || m_byPengNum>0)
	{
		return false;
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][0] != ACTION_AN_GANG)
		{
			return false;
		}
	}
	return true;
}

//�Ĺ�һ�� �����У���4����ͬ���ƹ���һ�ҵ�˳�����ӡ��ԡ�������(����������) 
bool CCheckHuPai::CheckSiGuiYi()
{
	if(m_bZimo)//����
	{
		for(int i=0;i<hupaistruct.count;++i)
		{
			if(hupaistruct.byType[i] == TYPE_JIANG_PAI || hupaistruct.byType[i] == TYPE_JINGDIAO_JIANG || hupaistruct.byType[i] == TYPE_AN_KE)
			{
				if(pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][0]) == 4)
					return true;
			}
			else if(hupaistruct.byType[i] == TYPE_SHUN_ZI )
			{
				if(pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][0]) == 4)
					return true;
				if(pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][1]) == 4)
					return true;
				if(pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][2]) == 4)
					return true;
			}
		}
	}
	else
	{
		for(int i=0;i<hupaistruct.count;++i)
		{
			if(hupaistruct.byType[i] == TYPE_JIANG_PAI || hupaistruct.byType[i] == TYPE_JINGDIAO_JIANG || hupaistruct.byType[i] == TYPE_AN_KE)
			{
				if(pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][0]) == 4 
					|| (pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][0]) == 3 && m_byLastPai == hupaistruct.data[i][0]))
					return true;
			}
			else if(hupaistruct.byType[i] == TYPE_SHUN_ZI )
			{
				if(pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][0]) == 4
					|| (pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][0]) == 3 && m_byLastPai == hupaistruct.data[i][0]))
					return true;
				if(pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][1]) == 4
					|| (pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][1]) == 3 && m_byLastPai == hupaistruct.data[i][1]))
					return true;
				if(pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][2]) == 4
					|| (pDesk->m_UserData.GetAHandPaiCount(m_byStation,hupaistruct.data[i][2]) == 3 && m_byLastPai == hupaistruct.data[i][2]))
					return true;
			}
		}
	}
	return false;
}

//˫ͬ�̣� 2��������ͬ�Ŀ��� ,�����������ƣ�
bool CCheckHuPai::CheckShuangTongKe()
{
	if(m_byKeziNum<2)
	{
		return false;
	}
	//CString str="";
	BYTE num[9];
	memset(num,0,sizeof(num));
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		num[m_byKeziData[i]%10-1]++;
		//str.Format("���Ʋ��ԣ����� %d pai=%d",m_byKeziData[i]%10,m_byKeziData[i]);
		//OutputDebugString(str);
		if(num[m_byKeziData[i]%10-1]>=2)
		{
			return true;
		}
	}
	return false;
}

//˫���̣�2������ ,����˵��ڵ���,�������������ƣ�
bool CCheckHuPai::CheckShuangAnGang()
{
	if((m_byKeziNum - m_byPengNum)==2)
		return true;
	return false;
}

//���ܣ���ץ4����ͬ���ƿ��� 
bool CCheckHuPai::CheckAnGang()
{
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][0] == ACTION_AN_GANG)
		{
			return true;
		}
	}
	return false;
}

//���ۣ�������û��һ���ż����� 
bool CCheckHuPai::CheckDuanYao()
{
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[i] == 255)
			continue;
		if(m_byArHandPai[i]>=CMjEnum::MJ_TYPE_FD || m_byArHandPai[i]%10==1 || m_byArHandPai[i]%10==9)
			return false;
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255 || pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0] == 255 )
			continue;
		for(int j=0;j<3;++j)
		{
			if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[j]>=CMjEnum::MJ_TYPE_FD || pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[j]%10==1 || pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[j]%10==9)
				return false;
		}
	}
	return true;
}

//ȫ���ۣ�����ʱ��ÿ���ơ����ƶ������� 
bool CCheckHuPai::CheckQuanDaiYao()
{
	for(int i=0;i<hupaistruct.count;++i)
	{
		if(hupaistruct.byType[i] == TYPE_JIANG_PAI || hupaistruct.byType[i] == TYPE_JINGDIAO_JIANG || hupaistruct.byType[i] == TYPE_AN_KE)
		{
			if(hupaistruct.data[i][0]<CMjEnum::MJ_TYPE_FD && hupaistruct.data[i][0]%10!=1 && hupaistruct.data[i][0]%10!=9)
				return false;
		}
		else if(hupaistruct.byType[i] == TYPE_SHUN_ZI )
		{
			if(hupaistruct.data[i][0]<CMjEnum::MJ_TYPE_FD && hupaistruct.data[i][0]%10!=1 && hupaistruct.data[i][0]%10!=9)
				return false;
			if(hupaistruct.data[i][1]<CMjEnum::MJ_TYPE_FD && hupaistruct.data[i][1]%10!=1 && hupaistruct.data[i][1]%10!=9)
				return false;
			if(hupaistruct.data[i][2]<CMjEnum::MJ_TYPE_FD && hupaistruct.data[i][2]%10!=1 && hupaistruct.data[i][2]%10!=9)
				return false;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0]<CMjEnum::MJ_TYPE_FD && pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0]%10!=1 && pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0]%10!=9)
			return false;
	}
	return true;
}

//�����ˣ�4���Ƽ�����û�г��ơ�����(��������)���������� 
bool CCheckHuPai::CheckBuQiuRen()
{
	if(!m_bZimo)//����
	{
		return false;
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][0] != ACTION_AN_GANG)
		{
			return false;
		}
	}

	return true;
}

//˫���ܣ�2������ 
bool CCheckHuPai::CheckShuangMingGang()
{
	BYTE num = 0;
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][0] != ACTION_AN_GANG)
		{
			num++;
		}
	}
	if(num == 2)
	{
		return true;
	}
	return false;
}

//�;��ţ����Ƴء�������������3������ʣ�ĵ�4����(���ܺͲ��ƺ;���) 
bool CCheckHuPai::CheckHuJueZhang()
{

	return false;
}

//�����ͣ� ��4������(���)��������ɵĺ��� 
bool CCheckHuPai::CheckPengPengHu()
{

	if(m_byShunNum == 0)//˳�Ӹ���Ϊ0
	{
		OutputDebugString("���Ʋ��ԣ������� �ɹ�");
		return true;
	}
	OutputDebugString("���Ʋ��ԣ������� ʧ��");
	return false;
}

//һɫ�Ƽ�⣨������ƺͳ������ƣ����ﶼ���ã���0����һɫ�ƣ�1��һɫ��2��һɫ
int CCheckHuPai::CheckYiSe()
{
	bool zi = false;
	BYTE type = 255,pai = 255;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[i] == 255)
			continue;
		pai = m_byArHandPai[i];
		if(CMjRef::GetHuaSe(pai) == 3)
		{
			zi = true;
			continue;
		}
		if(type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if(type != CMjRef::GetHuaSe(pai))
		{
			return 0;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(CMjRef::GetHuaSe(pai) == 3)
		{
			zi = true;
			continue;
		}
		if(type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if(type != CMjRef::GetHuaSe(pai))
		{
			return 0;
		}
	}

	if(type == 255)
		return 0;

	if(zi)
	{
		return 1;
	}
	return 2;
}

//��һɫ����һ�ֻ�ɫ�����Ƽ�������ɵĺ��� 
bool CCheckHuPai::CheckHunYiSe()
{	
	return CheckYiSe() == 1;
}

//��ɫ�����ߣ� 3�ֻ�ɫ3�����ε���һλ������˳�ӣ����������ƣ�
bool CCheckHuPai::CheckSanSeSanBuGao()
{
	if(m_byShunNum<3)
	{
		return false;
	}
	bool shun[3][9];
	memset(shun,0,sizeof(shun));
	MaoPaoSort(m_byShunData,4,false);
	for(int i=0;i<4;++i)
	{
		if(m_byShunData[i]==255)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1] = true;
	}
	for(int i=0;i<7;++i)
	{
		if(shun[0][i] && shun[1][i+1] && shun[2][i+2]
		|| shun[1][i] && shun[2][i+1] && shun[0][i+2]
		|| shun[2][i] && shun[1][i+1] && shun[0][i+2]
		)
		{
			return true;
		}
	}

	return false;
}

//�����룺����ʱ3�������ơ��硢������ȫ 
bool CCheckHuPai::CheckWuMenQi()
{
	bool wbt[3],feng=false,jian=false;
	memset(wbt,0,sizeof(wbt));
	BYTE pai=255;

	for(int i=0;i<hupaistruct.count;++i)
	{
		if(hupaistruct.data[i][0]==255)
			continue;
		pai = hupaistruct.data[i][0];
		if(CMjRef::GetHuaSe(pai)<3)
		{
			wbt[CMjRef::GetHuaSe(pai)] = true;
		}
		else
		{
			if(pai >= CMjEnum::MJ_TYPE_ZHONG)
			{
				jian = true;
			}
			else if(pai>=CMjEnum::MJ_TYPE_FD)
			{
				feng = true;
			}
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(CMjRef::GetHuaSe(pai)<3)
		{
			wbt[CMjRef::GetHuaSe(pai)] = true;
		}
		else
		{
			if(pai>=CMjEnum::MJ_TYPE_ZHONG)
			{
				jian = true;
			}
			else if(pai>=CMjEnum::MJ_TYPE_FD)
			{
				feng = true;
			}
		}
	}

	if(wbt[0] && wbt[1] && wbt[2] && feng && jian)
	{
		return true;
	}
	return false;
}

//ȫ���ˣ�ȫ�����ơ����ơ����������������ƺ��ơ����Ƶ��� 
bool CCheckHuPai::CheckQuanQiuRen()
{
	if(m_bZimo)//����
	{
		return false;
	}
	for(int i=0;i<hupaistruct.count;++i)
	{
		if(hupaistruct.byType[i] == TYPE_SHUN_ZI || hupaistruct.byType[i] == TYPE_AN_KE)
		{
			return false;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][0] == ACTION_AN_GANG)
		{
			return false;
		}
	}
	return true;
}

//˫���ܣ� 2������ 
bool CCheckHuPai::CheckShuangAnKe()
{
	BYTE num = 0;
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][0] == ACTION_AN_GANG)
		{
			num++;
		}
	}
	if(num == 2)
	{
		return true;
	}
	return false;
}

//˫���̣�2������(���) 
bool CCheckHuPai::CheckShuangJianKe()
{
	BYTE num = 0;
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] >= CMjEnum::MJ_TYPE_ZHONG && m_byGangData[i][1] <= CMjEnum::MJ_TYPE_BAI)
		{
			num++;
		}
	}
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i]>= CMjEnum::MJ_TYPE_ZHONG && m_byKeziData[i] <= CMjEnum::MJ_TYPE_BAI)
		{
			num++;
		}
	}
	return false;
}

//������ 3�ֻ�ɫ��3��˳�����ӳ�1-9�������� ,�����������ƣ�
bool CCheckHuPai::CheckHuaLong()
{
	bool shun[3][9];
	memset(shun,0,sizeof(shun));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1] = true;
	}
	if(shun[0][1] && shun[1][4] && shun[2][7] 
	||shun[1][1] && shun[2][4] && shun[0][7] 
	||shun[2][1] && shun[0][4] && shun[1][7] )
	{
		return true;
	}
	return false;
}

//�Ʋ����� ������ͼ��û���������������ɵĺ��ƣ�����1234589����245689�����װ塣����ȱһ�� 
bool CCheckHuPai::CheckTuiBuDao()
{
	BYTE pai = 255;
	for(int i=0;i<hupaistruct.count;++i)
	{			
		pai = hupaistruct.data[i][0];
		if(pai<=CMjEnum::MJ_TYPE_W9 || pai>=CMjEnum::MJ_TYPE_FD && pai!=CMjEnum::MJ_TYPE_BAI)
		{
			return false;
		}	
		if(hupaistruct.byType[i] == TYPE_SHUN_ZI)
		{
			if(pai>CMjEnum::MJ_TYPE_B3 && pai!=CMjEnum::MJ_TYPE_T4)
			{
				return false;
			}
		}
		else
		{
			if(pai==CMjEnum::MJ_TYPE_B6 || pai==CMjEnum::MJ_TYPE_B7 || pai==CMjEnum::MJ_TYPE_T1 || pai==CMjEnum::MJ_TYPE_T3 || pai==CMjEnum::MJ_TYPE_T7)
			{
				return false;
			}
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(pai<=CMjEnum::MJ_TYPE_W9 || pai>=CMjEnum::MJ_TYPE_FD && pai!=CMjEnum::MJ_TYPE_BAI)
		{
			return false;
		}
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if(pai>CMjEnum::MJ_TYPE_B3 && pai!=CMjEnum::MJ_TYPE_T4)
			{
				return false;
			}
		}
		else
		{
			if(pai==CMjEnum::MJ_TYPE_B6 || pai==CMjEnum::MJ_TYPE_B7 || pai==CMjEnum::MJ_TYPE_T1 || pai==CMjEnum::MJ_TYPE_T3 || pai==CMjEnum::MJ_TYPE_T7)
			{
				return false;
			}
		}
	}
	return true;
}

//��ɫ��ͬ˳������ʱ����3�ֻ�ɫ3��������ͬ��˳�� �����������ƣ�
bool CCheckHuPai::CheckSanSeSanTongShun()
{
	bool shun[3][9];
	memset(shun,0,sizeof(shun));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1]=true;
	}
	for(int i=0;i<9;++i)
	{
		if(shun[0][i] && shun[1][i] && shun[2][i])
		{
			return true;
		}
	}
	return false;
}

//��ɫ���ڸߣ�����ʱ����3�ֻ�ɫ3�����ε���һλ���Ŀ��ӣ����������ƣ�
bool CCheckHuPai::CheckSanSeSanJieGao()
{
	bool shun[3][9];
	memset(shun,0,sizeof(shun));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1]=true;
	}
	for(int i=0;i<7;++i)
	{
		if(shun[0][i] && shun[1][i+1] && shun[2][i+2]
		||shun[1][i] && shun[2][i+1] && shun[0][i+2]
		||shun[2][i] && shun[1][i+1] && shun[0][i+2])
		{
			return true;
		}
	}
	return false;
}

//�޷��ͣ����ƺ��������κη��ַ�(���Ʋ���������) 
bool CCheckHuPai::CheckWuFanHe()
{
	
	return false;
}

//���ֻش���������ǽ�����һ���ƺ��ơ��������� 
bool CCheckHuPai::CheckMiaoShouHuiChun()
{
	if(m_bZimo && pDesk->m_UserData.m_MenPai.GetMenPaiNum()==0)//����
	{
		return true;
	}
	return false;
}

//�������£��ʹ�������һ���� 
bool CCheckHuPai::CheckHaiDiLaoYue()
{
	if(!m_bZimo && pDesk->m_UserData.m_MenPai.GetMenPaiNum()==0)//����
	{
		return true;
	}
	return false;
}

//���ܺͣ��ͱ�����ץ�����ܵ��ơ����ƺ;���  
bool CCheckHuPai::CheckQiangGangHe()
{
	if(!m_bZimo && pDesk->m_GameData.temp_GangPai.bChenkQ)
	{
		return true;
	}
	return false;
}

//�����壺��������6-9��˳�ӡ����ӡ�������ɵĺ��ơ��������� 
bool CCheckHuPai::CheckDaYuWu()
{
	BYTE pai = 255;
	for(int i=0;i<hupaistruct.count;++i)
	{			
		pai = hupaistruct.data[i][0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(pai%10 < 6 )
		{
			return false;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(pai%10 < 6)
		{
			return false;
		}
	}
	return true;
}

//С���壺��������1-4��˳�ӡ����ӡ�������ɵĺ��ơ��������� 
bool CCheckHuPai::CheckXiaoYuWu()	
{
	BYTE pai = 255;
	for(int i=0;i<hupaistruct.count;++i)
	{			
		pai = hupaistruct.data[i][0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(hupaistruct.byType[i] == TYPE_SHUN_ZI)
		{
			if(pai%10 > 2 )
			{
				return false;
			}
		}
		else
		{
			if(pai%10 > 4)
			{
				return false;
			}
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if(pai%10 > 2 )
			{
				return false;
			}
		}
		else
		{
			if(pai%10 > 4)
			{
				return false;
			}
		}
	}
	return true;
}

//����̣�3����� ��(����������)
bool CCheckHuPai::CheckSanFengKe()
{
	BYTE num = 0;
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i]>=CMjEnum::MJ_TYPE_FD && m_byKeziData[i]<=CMjEnum::MJ_TYPE_FB)
		{
			num++;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1]>=CMjEnum::MJ_TYPE_FD && m_byGangData[i][1]<=CMjEnum::MJ_TYPE_FB)
		{
			num++;
		}
	}
	if(num==3)
	{
		return true;
	}
	return false;
}

//����������ʱ����һ�ֻ�ɫ1-9�����ӵ������� �����������ƣ�
bool CCheckHuPai::CheckQingLong()
{ 
	if(m_byShunNum<3)
	{
		return false;
	}
	MaoPaoSort(m_byShunData,4,false);
	if(m_byShunData[0]==m_byShunData[1]-3==m_byShunData[2]-6 && m_byShunData[0]%10==1)
	{
		return true;
	}
	return false;
}

//��ɫ˫���᣺2�ֻ�ɫ2�����ٸ�����һ�ֻ�ɫ5�����ĺ��ơ�����ϲ��ꡢ���ٸ������֡�ƽ�� �����������ƣ�
bool CCheckHuPai::CheckSanSeShuangLongHui()
{
	if(m_byShunNum<4)
	{
		return false;
	}
	bool shun[3][9];
	memset(shun,0,sizeof(shun));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1]=true;
	}
	if(shun[0][0] && shun[0][6] && shun[1][0] && shun[1][6] && m_byJiang == CMjEnum::MJ_TYPE_T5
	|| shun[0][0] && shun[0][6] && shun[2][0] && shun[2][6] && m_byJiang == CMjEnum::MJ_TYPE_B5
	|| shun[2][0] && shun[2][6] && shun[1][0] && shun[1][6] && m_byJiang == CMjEnum::MJ_TYPE_W5)
	{
		return true;
	}
	return false;
}

//һɫ�����ߣ� ����ʱ����һ�ֻ�ɫ3�����ε���һλ�����ε�����λ���ֵ�˳�� �����������ƣ�
bool CCheckHuPai::CheckYiSeSanBuGao()
{
	if(m_byShunNum<3)
	{
		return false;
	}
	bool shun[3][9];
	memset(shun,0,sizeof(shun));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1]=true;
	}
	for(int i=0;i<7;++i)
	{
		if(shun[0][i] && shun[0][i+1] && shun[0][i+2] 
		|| shun[1][i] && shun[1][i+1] && shun[1][i+2]
		|| shun[2][i] && shun[2][i+1] && shun[2][i+2])
		{
			return true;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(shun[0][i] && shun[0][i+2] && shun[0][i+4] 
		|| shun[1][i] && shun[1][i+2] && shun[1][i+4]
		|| shun[2][i] && shun[2][i+2] && shun[2][i+4])
		{
			return true;
		}
	}
	return false;
}

//ȫ���壺ÿ���Ƽ����Ʊ�����5�������ơ����ƶ��� 
bool CCheckHuPai::CheckQuanDaiWu()
{
	BYTE pai = 255,type = 255;
	bool zi = false;
	for(int i=0;i<hupaistruct.count;++i)
	{	
		pai = hupaistruct.data[i][0];
		if(CMjRef::GetHuaSe(pai)==3)
		{
			return false;
		}
		if(hupaistruct.byType[i] == TYPE_SHUN_ZI)
		{
			if(pai%10>7 || pai%10<3)
			{
				return false;
			}
		}
		else 
		{
			if(pai%10 != 5)
			{
				return false;
			}
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(CMjRef::GetHuaSe(pai)==3)
		{
			return false;
		}
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if(pai%10>7 || pai%10<3)
			{
				return false;
			}
		}
		else 
		{
			if(pai%10 != 5)
			{
				return false;
			}
		}
	}
	return true;
}

//��ͬ�̣�3��������ͬ�Ŀ���(��) 
bool CCheckHuPai::CheckSanTongKe()
{
	BYTE num[9];
	memset(num,0,sizeof(num));
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i]>=CMjEnum::MJ_TYPE_FD)
		{
			continue;
		}
		num[m_byKeziData[i]%10-1]++;
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1]>=CMjEnum::MJ_TYPE_FD)
		{
			continue;
		}
		num[m_byGangData[i][1]%10-1]++;
	}
	for(int i=0;i<9;++i)
	{
		if(num[i]==3)
		{
			return true;
		}
	}
	return false;
}

//�����̣� 3������ 
bool CCheckHuPai::CheckSanAnKe()	
{
	BYTE num = 0;
	for(int i=0;i<hupaistruct.count;++i)
	{	
		if(hupaistruct.byType[i] == TYPE_AN_KE)
		{
			num++;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == ACTION_AN_GANG)
			num++;
	}
	if(num == 3)
	{
		return true;
	}
	return false;
}

//ȫ˫�̣� ��2��4��6��8�����ƵĿ��ˡ�������ɵĺ��ơ������������ƣ������������͡����� 
bool CCheckHuPai::CheckQuanShuangKe()
{
	if(m_byShunNum>0)
	{
		return false;
	}
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i]>=CMjEnum::MJ_TYPE_FD || m_byKeziData[i]%10!=0)
		{
			return false;
		}	
	}
	if(m_byJiang>=CMjEnum::MJ_TYPE_FD || m_byJiang%10!=0)
	{
		return false;
	}
	return true;
}

//��һɫ�� ��һ�ֻ�ɫ����������ɺ͸��ơ������� 
bool CCheckHuPai::CheckQingYiSe()
{
	return CheckYiSe()==2;
}

//һɫ��ͬ˳������ʱ��һ�ֻ�ɫ3��������ͬ��˳�ˡ�����һɫ���ڸ� �����������ƣ�
bool CCheckHuPai::CheckYiSeSanTongShun()
{
	if(m_byShunNum<3)
	{
		return false;
	}
	BYTE num[3][9];
	memset(num,0,sizeof(num));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		num[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1]++;
	}
	for(int i=0;i<7;++i)
	{
		if(num[0][i]==3 || num[1][i]==3  || num[2][i]==3)
		{
			return true;
		}
	}
	return false;
}

//һɫ���ڸߣ�����ʱ��һ�ֻ�ɫ3�����ε���һλ���ֵĿ���(��)������һɫ��ͬ˳ ������������,���ƣ�
bool CCheckHuPai::CheckYiSeSanJieGao()
{
	if(m_byKeziNum<3)
	{
		return false;
	}
	bool kezi[3][9];
	memset(kezi,0,sizeof(kezi));
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		kezi[CMjRef::GetHuaSe(m_byKeziData[i])][m_byKeziData[i]%10-1]=true;
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1]>=CMjEnum::MJ_TYPE_FD)
			continue;
		kezi[CMjRef::GetHuaSe(m_byGangData[i][1])][m_byGangData[i][1]%10-1]=true;
	}
	for(int i=0;i<7;++i)
	{
		if(kezi[0][i] && kezi[0][i+1] && kezi[0][i+2] 
		|| kezi[1][i] && kezi[1][i+1] && kezi[1][i+2]
		|| kezi[2][i] && kezi[2][i+1] && kezi[2][i+2])
		{
			return true;
		}
	}
	return false;
}

//ȫ����������789��ɵ�˳�ˡ�����(��)�����Ƶĺ��ơ��������� 
bool CCheckHuPai::CheckQuanDa()
{
	BYTE pai = 255;
	for(int i=0;i<hupaistruct.count;++i)
	{			
		pai = hupaistruct.data[i][0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(pai%10 < 6)
		{
			return false;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(pai%10 < 6)
		{
			return false;
		}
	}
	return true;
}

//ȫ�У� ��������456��ɵ�˳�ӡ�����(��)�����Ƶĺ��ơ����ƶ��� 
bool CCheckHuPai::CheckQuanZhong()	
{
	BYTE pai = 255;
	for(int i=0;i<hupaistruct.count;++i)
	{			
		pai = hupaistruct.data[i][0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(hupaistruct.byType[i] == TYPE_SHUN_ZI)
		{
			if(pai%10 != 4 )
			{
				return false;
			}
		}
		else
		{
			if(pai%10 > 6 ||  pai%10 < 4)
			{
				return false;
			}
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if(pai%10 != 4 )
			{
				return false;
			}
		}
		else
		{
			if(pai%10 > 6 ||  pai%10 < 4)
			{
				return false;
			}
		}
	}
	return true;
}

//ȫС����������123��ɵ�˳�ӡ�����(��)���Ƶĵĺ��ơ��������� 
bool CCheckHuPai::CheckQuanXiao()
{
	BYTE pai = 255;
	for(int i=0;i<hupaistruct.count;++i)
	{			
		pai = hupaistruct.data[i][0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(hupaistruct.byType[i] == TYPE_SHUN_ZI)
		{
			if(pai%10 >1 )
			{
				return false;
			}
		}
		else
		{
			if(pai%10 >3 )
			{
				return false;
			}
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(pai>=CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}	
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if(pai%10 >1 )
			{
				return false;
			}
		}
		else
		{
			if(pai%10 >3 )
			{
				return false;
			}
		}
	}
	return true;
}

//һɫ�Ĳ��ߣ� һ�ֻ�ɫ4�����ε���һλ�������ε�����λ����˳�� �����������ƣ�
bool CCheckHuPai::CheckYiSeSiBuGao()	
{
	if(m_byShunNum<4)
	{
		return false;
	}
	bool shun[3][9];
	memset(shun,0,sizeof(shun));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1]=true;
	}
	for(int i=0;i<6;++i)
	{
		if(shun[0][i] && shun[0][i+1]  && shun[0][i+2] && shun[0][i+3]
		|| shun[1][i] && shun[1][i+1]  && shun[1][i+2] && shun[1][i+3]
		|| shun[2][i] && shun[0][i+1]  && shun[2][i+2] && shun[2][i+3] )
		{
			return true;
		}
	}
	for(int i=0;i<3;++i)
	{
		if(shun[0][i] && shun[0][i+2]  && shun[0][i+4] && shun[0][i+6]
		|| shun[1][i] && shun[1][i+2]  && shun[1][i+4] && shun[1][i+6]
		|| shun[2][i] && shun[0][i+2]  && shun[2][i+4] && shun[2][i+6] )
		{
			return true;
		}
	}
	return false;
}

//���ܣ�3���� 
bool CCheckHuPai::CheckSanGang()
{
	if(m_byGangNum==3)
	{
		return true;
	}
	return false;
}

//���۾ţ������ƺ�������һ���ŵĿ����ý�����ɵĺ��ơ����������� 
bool CCheckHuPai::CheckHunYaoJiu()
{
	if(m_byShunNum>0)
	{
		return false;
	}
	BYTE pai = 255,type = 255;
	bool zi = false;
	for(int i=0;i<hupaistruct.count;++i)
	{	
		pai = hupaistruct.data[i][0];
		if(CMjRef::GetHuaSe(pai)==3)
		{
			zi = true;
			continue;
		}
		if(pai%10 != 1 && pai%10 != 9)
		{
			return false;
		}
		if(type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if(type != CMjRef::GetHuaSe(pai))
		{
			return false;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(CMjRef::GetHuaSe(pai)==3)
		{
			zi = true;
			continue;
		}
		if(pai%10 != 1 && pai%10 != 9)
		{
			return false;
		}
		if(type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if(type != CMjRef::GetHuaSe(pai))
		{
			return false;
		}
	}
	if(zi)
	{
		return true;
	}
	return false;
}

//һɫ��ͬ˳��һ�ֻ�ɫ4��������ͬ��˳�ӣ�����һɫ���ڸߡ�һ��ߡ��Ĺ�һ �����������ƣ�
bool CCheckHuPai::CheckYiSeSiTongShun()
{
	if(m_byShunNum<4)
	{
		return false;
	}
	BYTE num[3][9];
	memset(num,0,sizeof(num));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		num[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1]++;
	}
	for(int i=0;i<7;++i)
	{
		if(num[0][i]==4 || num[1][i]==4  || num[2][i]==4)
		{
			return true;
		}
	}
	return false;
}

//һɫ�Ľڸߣ�һ�ֻ�ɫ4�����ε���һλ���Ŀ��ӣ��ܣ�����һɫ���ڸߡ������� �����������ƣ����ƣ� 
bool CCheckHuPai::CheckYiSeSiJieGao()
{
	if((m_byKeziNum + m_byGangNum)<4)
	{
		return false;
	}
	bool kezi[3][9];
	memset(kezi,0,sizeof(kezi));
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		kezi[CMjRef::GetHuaSe(m_byKeziData[i])][m_byKeziData[i]%10-1]=true;
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1]>=CMjEnum::MJ_TYPE_FD)
			continue;
		kezi[CMjRef::GetHuaSe(m_byGangData[i][1])][m_byGangData[i][1]%10-1]=true;
	}
	for(int i=0;i<6;++i)
	{
		if(kezi[0][i] && kezi[0][i+1] && kezi[0][i+2] && kezi[0][i+3] 
		|| kezi[1][i] && kezi[1][i+1] && kezi[1][i+2] && kezi[1][i+3] 
		|| kezi[2][i] && kezi[2][i+1] && kezi[2][i+2] && kezi[2][i+3] )
		{
			return true;
		}
	}	
	return false;
}

//���۾ţ���������һ���ſ�����ɵĺ��ơ����������͡�ͬ�̡����� 
bool CCheckHuPai::CheckQingYaoJiu()
{
	if(m_byShunNum>0)
	{
		return false;
	}
	BYTE pai = 255,type = 255;
	for(int i=0;i<hupaistruct.count;++i)
	{	
		pai = hupaistruct.data[i][0];
		if(pai%10 != 1 && pai%10 != 9)
		{
			return false;
		}
		if(type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if(type != CMjRef::GetHuaSe(pai))
		{
			return false;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(pai%10 != 1 && pai%10 != 9)
		{
			return false;
		}
		if(type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if(type != CMjRef::GetHuaSe(pai))
		{
			return false;
		}
	}
	return true;
}

//С��ϲ������ʱ�з��Ƶ�3�����Ӽ����ơ���������� �������������ƣ�
bool CCheckHuPai::CheckXiaoSiXi()
{
	if((m_byGangNum+m_byKeziNum)<3)
	{
		return false;
	}
	BYTE num = 0;
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD && m_byKeziData[i] <=CMjEnum::MJ_TYPE_FB)
		{
			num++;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD && m_byGangData[i][1] <=CMjEnum::MJ_TYPE_FB)
		{
			num++;
		}
	}
	if(num==3 && m_byJiang >= CMjEnum::MJ_TYPE_FD && m_byJiang <=CMjEnum::MJ_TYPE_FB)
	{
		return true;
	}
	return false;
}

//С��Ԫ�� ����ʱ�м��Ƶ��������Ӽ����ơ����Ƽ��� �������������ƣ�
bool CCheckHuPai::CheckXiaoSanYuan()
{
	if((m_byGangNum+m_byKeziNum)<2)
		return false;
	BYTE num = 0;
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i] >= CMjEnum::MJ_TYPE_ZHONG && m_byKeziData[i] <=CMjEnum::MJ_TYPE_BAI)
		{
			num++;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] >= CMjEnum::MJ_TYPE_ZHONG && m_byGangData[i][1] <=CMjEnum::MJ_TYPE_BAI)
		{
			num++;
		}
	}
	if(num==2 && m_byJiang >= CMjEnum::MJ_TYPE_ZHONG && m_byJiang <=CMjEnum::MJ_TYPE_BAI)
	{
		return true;
	}
	return false;
}

//��һɫ�������ƵĿ���(��)������ɵĺ��ơ����������� 
bool CCheckHuPai::CheckZiYiSe()
{
	//m_byShunNum
	BYTE pai = 255;
	for(int i=0;i<HAND_CARD_NUM;i++)
	{
		if(m_byArHandPai[i] == 255 || m_byArHandPai[i] > CMjEnum::MJ_TYPE_BAI)
			continue;
		if(m_byArHandPai[i]<CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0]<CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
	}
	return true;
}

//�İ��̣�4������(����)���������˴�����ơ�������ǰ�塢������ 
bool CCheckHuPai::CheckSiAnKe()
{

	BYTE num = 0;
	for(int i=0;i<hupaistruct.count;++i)
	{			
		if(hupaistruct.byType[i] == TYPE_AN_KE)
		{
			num++;
		}
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == ACTION_AN_GANG)
			num++;
	}
	if(num==4)
	{
		return true;
	}

	CheckPingHuData();

	return false;
}

//һɫ˫���᣺һ�ֻ�ɫ���������ٸ���5Ϊ���ơ�����ƽ�����߶ԡ���һɫ �����������ƣ�
bool CCheckHuPai::CheckYiSeShuangLongHui()
{
	if(m_byShunNum<4)
	{
		return false;
	}
	BYTE num[3][9];
	memset(num,0,sizeof(num));
	for(int i=0;i<m_byShunNum;++i)
	{
		if(m_byShunData[i]>=CMjEnum::MJ_TYPE_FD)
			continue;
		num[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i]%10-1]++;
	}
	if(	  num[0][1]==2 && num[0][7]==2 && m_byJiang==CMjEnum::MJ_TYPE_W5
		||num[1][1]==2 && num[1][7]==2 && m_byJiang==CMjEnum::MJ_TYPE_B5
		||num[2][1]==2 && num[2][7]==2 && m_byJiang==CMjEnum::MJ_TYPE_T5 )
	{
	}

	return false;
}

//����ϲ����4�����(��)��ɵĺ��ơ�����Ȧ��̡��ŷ�̡�����̡������� �����������ƣ�
bool CCheckHuPai::CheckDaSiXi()
{
	if((m_byGangNum+m_byKeziNum)<4)
	{
		return false;
	}
	bool zfb[4];
	memset(zfb,0,sizeof(zfb));
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD && m_byKeziData[i] <=CMjEnum::MJ_TYPE_FB)
		{
			zfb[m_byKeziData[i]-CMjEnum::MJ_TYPE_FD] = true;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD && m_byGangData[i][1] <=CMjEnum::MJ_TYPE_FB)
		{
			zfb[m_byGangData[i][1]-CMjEnum::MJ_TYPE_FD] = true;
		}
	}
	if(zfb[0] && zfb[1] && zfb[2] && zfb[3])
	{
		return true;
	}
	return false;
}

//����Ԫ�������У����з���3�����ӣ����ơ����Ƽ��� �������������ƣ�
bool CCheckHuPai::CheckDaSanYuan()
{
	if((m_byGangNum+m_byKeziNum)<3)
		return false;
	bool zfb[3];
	memset(zfb,0,sizeof(zfb));
	for(int i=0;i<m_byKeziNum;++i)
	{
		if(m_byKeziData[i] >= CMjEnum::MJ_TYPE_ZHONG && m_byKeziData[i] <=CMjEnum::MJ_TYPE_BAI)
		{
			zfb[m_byKeziData[i]-CMjEnum::MJ_TYPE_ZHONG] = true;
		}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
		if(m_byGangData[i][1] >= CMjEnum::MJ_TYPE_ZHONG && m_byGangData[i][1] <=CMjEnum::MJ_TYPE_BAI)
		{
			zfb[m_byGangData[i][1]-CMjEnum::MJ_TYPE_ZHONG] = true;
		}
	}
	if(zfb[0] && zfb[1] && zfb[2])
	{
		return true;
	}
	return false;
}

//��һɫ����23468���������е��κ�����ɵ�˳�ӡ����ӡ����ĺ��ơ����ƻ�һɫ������"��"����ɵĸ��ƣ��ɼ���һɫ 
bool CCheckHuPai::CheckLvYiSe()
{
	BYTE pai = 255;
	for(int i=0;i<HAND_CARD_NUM;i++)//��������
	{
		if(m_byArHandPai[i] == 255)
			continue;
		pai = m_byArHandPai[i];
		if(pai!= CMjEnum::MJ_TYPE_T2 && pai!=CMjEnum::MJ_TYPE_T3 && pai!=CMjEnum::MJ_TYPE_T4 && pai!=CMjEnum::MJ_TYPE_T6 && pai!=CMjEnum::MJ_TYPE_T8 && pai!= CMjEnum::MJ_TYPE_FA)
			return false;
	}
	for(int i=0;i<5;++i)
	{
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		pai = pDesk->m_UserData.m_UserGCPData[m_byStation][i].byData[0];
		if(pDesk->m_UserData.m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if(pai!=CMjEnum::MJ_TYPE_T2)
				return false;
		}
		else
		{
			if(pai!= CMjEnum::MJ_TYPE_T2 && pai!=CMjEnum::MJ_TYPE_T3 && pai!=CMjEnum::MJ_TYPE_T4 && pai!=CMjEnum::MJ_TYPE_T6 && pai!=CMjEnum::MJ_TYPE_T8 && pai!= CMjEnum::MJ_TYPE_FA)
				return false;
		}
	}
	return true;
}



//�ĸܣ�ʮ���޺�����4���� 
bool CCheckHuPai::CheckSiGang()
{
	if(m_byGangNum == 4)
		return true;
	return false;
}

//�������ƣ���һ�ֻ�ɫ�������Ӱ�1112345678999��ɵ��ض����ͣ���ͬ��ɫ�κ�1�������Ƽ��ɺ��ơ�������һɫ���������������ƣ�
bool CCheckHuPai::CheckJiuLianBaoDeng()
{
	if(pDesk->m_UserData.GetGCPCount(m_byStation)>0)//�����г�����
	{
		return false;
	}
	BYTE type = 255;
	BYTE num[9];
	memset(num,0,sizeof(num));
	BYTE hand[HAND_CARD_NUM];
	memset(hand,255,sizeof(hand));

	for(int i=0;i<HAND_CARD_NUM;++i)
	{
		if(m_byArHandPai[i] == 255)
			continue;
		hand[i] = m_byArHandPai[i];
	}
	for(int i=0;i<HAND_CARD_NUM;++i)
	{
		if(hand[i] == 255)
			continue;
		if(hand[i] == m_byLastPai)//���һ���Ʋ���
		{
			hand[i] = 255;
			break;
		}
	}
	///��С��������
	MaoPaoSort(hand,HAND_CARD_NUM,false);
	
	for(int i=0;i<HAND_CARD_NUM;++i)
	{
		if(hand[i] == 255)
			continue;
		if(hand[i] >= CMjEnum::MJ_TYPE_FD)//����������
			return false;
		num[hand[i]%10-1]++;
		if(type == 255)
		{
			type = hand[i]/10;
		}
		if(type != hand[i]/10)//��������һɫ
		{
			return false;
		}
	}
	for(int i=0;i<9;i++)
	{
		if(i==0 || i==8)
		{
			if(num[i] < 3)
			{
				return false;
			}
		}
		else
		{
			if(num[i] != 1)
			{
				return false;
			}
		}
	}
	return true;
}

/////////////////////////��������///////////////////////////////////////////////////////////////////////////////////////////////


//ȫ�������ɵ���3�ֻ�ɫ147��258��369���ܴ�λ�������Ƽ����������з����е��κ�14������ɵĺ��ƣ�ʮ���ã������������롢�����ˡ�����(��ƽ������)
bool CCheckHuPai::CheckQuanBuKao()
{
	if(pDesk->m_UserData.GetGCPCount(m_byStation)>0)//�����г�����
		return false;
	BYTE pai = 255;
	for(int i=0;i<NoJingHuTempData.conut;++i)
	{
		if(NoJingHuTempData.data[i][1]>1)//��������ͬ����
			return false;
		if(NoJingHuTempData.data[i][0]>=CMjEnum::MJ_TYPE_FD)
		{
			continue;
		}
		if(pai == 255 || pai/10!=NoJingHuTempData.data[i][0]/10)
		{
			pai = NoJingHuTempData.data[i][0];
		}
		else
		{
			if((NoJingHuTempData.data[i][0] - pai)<3)
			{
				return false;
			}
		}
		pai = NoJingHuTempData.data[i][0];//�Ƚ�����¼����
	}
	return true;
}

//�������3�ֻ�ɫ��147��258��369���ܴ�λ�������� ��
bool CCheckHuPai::CheckZuHeLong()	
{
	if(pDesk->m_UserData.GetGCPCount(m_byStation)>0)//�����г�����
	{
		return false;
	}
	return false;
}

//�߶ԣ���7��������ɺ��ơ����Ʋ����ˡ�����  (��ƽ������)
int CCheckHuPai::CheckQiDui()
{
	if(pDesk->m_UserData.GetGCPCount(m_byStation)>0)//�����г�����
	{
		return 0;
	}
	int need = 0;
	for(int i=0;i<NoJingHuTempData.conut;++i)
	{
		if(NoJingHuTempData.data[i][1] == 0)
			continue;
		if(NoJingHuTempData.data[i][1]%2==1)
			return 0;
	}
	return 1;
}

//���ǲ�����������7�����ŵĶ����ϱ��з��ף�����3�ֻ�ɫ����λ��147��258��369�е�7�����������û�н��Ƶĺ��ơ����������롢�����ˡ����� (��ƽ������)
bool CCheckHuPai::CheckQiXingBuKao()
{
	if(pDesk->m_UserData.GetGCPCount(m_byStation)>0)//�����г�����
	{
		return false;
	}
	BYTE num=0;
	BYTE pai = 255;
	for(int i=0;i<NoJingHuTempData.conut;++i)
	{
		if(NoJingHuTempData.data[i][1]>1)
			return false;
		if(NoJingHuTempData.data[i][0]>=CMjEnum::MJ_TYPE_FD)
		{
			num++;
			continue;
		}
		if(pai == 255 || pai/10!=NoJingHuTempData.data[i][0]/10)
		{
			pai = NoJingHuTempData.data[i][0];
		}
		else
		{
			if((NoJingHuTempData.data[i][0]-pai)<3)
			{
				return false;
			}
		}
	}
	if(num==7)
	{
		return true;
	}
	return false;
}

//���߶ԣ� ��һ�ֻ�ɫ�������������������7�����ӵĺ��ơ�������һɫ�������ˡ����� �������������ƣ�
bool CCheckHuPai::CheckLianQiDui()
{
	if(pDesk->m_UserData.GetGCPCount(m_byStation)>0)//�����г�����
	{
		return false;
	}	
	BYTE duizi[9],num=0,pai=255;
	memset(duizi,255,sizeof(duizi));
	for(int i=0;i<NoJingHuTempData.conut;++i)
	{
		if(NoJingHuTempData.data[i][1] == 0)
			continue;
		if(NoJingHuTempData.data[i][1] != 2)
			return false;
		duizi[num] = NoJingHuTempData.data[i][0];
		num++;
	}
	pai = duizi[0];
	if( duizi[1]-1 == pai && duizi[2]-2 ==  pai && duizi[3]-3 == pai && duizi[4]-4 == pai && duizi[5]-5 == pai && duizi[6]-6==pai)
	{
		return true;
	}
	return false;
}

//ʮ���ۣ���3�������Ƶ�һ�����ƣ�7�����Ƽ�����һ��������ɵĺ��ơ����������롢�����ˡ����� �������������ƣ�
bool CCheckHuPai::CheckShiSanYao()
{
	if(pDesk->m_UserData.GetGCPCount(m_byStation)>0)//�����г�����
	{
		return false;
	}
	int need = 0;
	int pai[38],duizi=0;
	memset(pai,0,sizeof(pai));
	for(int i=0;i<NoJingHuTempData.conut;i++)
	{
		if(NoJingHuTempData.data[i][1] < 1 || NoJingHuTempData.data[i][1] == 255 || NoJingHuTempData.data[i][0] == 255)
			continue;
		if(NoJingHuTempData.data[i][0] < CMjEnum::MJ_TYPE_FD && NoJingHuTempData.data[i][0]%10 != 1 && NoJingHuTempData.data[i][0]%10 != 9 )
		{
			return false;
		}
		pai[NoJingHuTempData.data[i][0]] = NoJingHuTempData.data[i][1];
	}

	for(int i=0;i<CMjEnum::MJ_TYPE_BAI+1;++i)
	{
		if(i%10 == 0 || (i<CMjEnum::MJ_TYPE_FD && i%10 != 1 && i%10 != 9))
			continue;
		if(pai[i] <= 0)//ȱ����
		{
			need++;
		}
		if(pai[i] > 1)
		{
			duizi++;
		}
		if(duizi >= 2)
		{
			return false;
		}
	}
	if(m_byJingNum == 255)
		m_byJingNum = 0;
	if(need > m_byJingNum)
	{
		return false;
	}
	return true;
}
