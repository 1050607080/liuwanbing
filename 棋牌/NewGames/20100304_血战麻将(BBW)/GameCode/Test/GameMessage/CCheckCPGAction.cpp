#include "stdafx.h"
#include "CCheckCPGAction.h"
#include "CSrvLogic.h"

CCheckCPGAction::CCheckCPGAction(void)
{
	pUserData = NULL;
}

CCheckCPGAction::~CCheckCPGAction(void)
{

}

///�ܷ����
bool CCheckCPGAction::CanChi(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData)
{
	if(station != CLogicBase::GetNextStation(pDesk->m_UserData.m_byNowOutStation))//�����¼Ҳ��ܳ���
		return false;
	bool canchi = false;
	int index = 0;
	//���ƻ��й�״̬������
	if(pDesk->m_UserData.IsUserState( station,USER_STATE_TING) || pDesk->m_UserData.IsUserState( station,USER_STATE_AUTO))
		return false;
	if(!pDesk->m_GameData.m_mjAction.bChi || pai == 255)
	{
		return false;
	}
	if(!pDesk->m_GameData.m_mjAction.bChiJing && pDesk->m_UserData.m_StrJing.CheckIsJing(pai))
	{
		return false;
	}

	if(pai <= CMjEnum::MJ_TYPE_B9)
	{//�������
		BYTE huase = pai/10;
		if(pDesk->m_UserData.IsHaveAHandPai(station,pai+1) && pDesk->m_UserData.IsHaveAHandPai(station,pai+2) && (pai+1)/10 == huase && (pai+2)/10 == huase)
		{
			if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(pai+1) && !pDesk->m_UserData.m_StrJing.CheckIsJing(pai+2)))
			{
				ResultData.m_iChi[index][0] = pai;
				ResultData.m_iChi[index][1] = pai+1;
				ResultData.m_iChi[index][2] = pai+2;
				index++;
				canchi = true;
			}
		}
		if(pDesk->m_UserData.IsHaveAHandPai(station,pai+1) && pDesk->m_UserData.IsHaveAHandPai(station,pai-1) && (pai+1)/10 == huase && (pai-1)/10 == huase)
		{
			if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(pai+1) && !pDesk->m_UserData.m_StrJing.CheckIsJing(pai-1)))
			{
				ResultData.m_iChi[index][0] = pai-1;
				ResultData.m_iChi[index][1] = pai;
				ResultData.m_iChi[index][2] = pai+1;
				index++;
				canchi = true;
			}
		}
		if(pDesk->m_UserData.IsHaveAHandPai(station,pai-1) && pDesk->m_UserData.IsHaveAHandPai(station,pai-2) && (pai-1)/10 == huase && (pai-2)/10 == huase)
		{
			if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(pai-1) && !pDesk->m_UserData.m_StrJing.CheckIsJing(pai-2)))
			{
				ResultData.m_iChi[index][0] = pai-2;
				ResultData.m_iChi[index][1] = pai-1;
				ResultData.m_iChi[index][2] = pai;
				index++;
				canchi = true;
			}
		}

	}
	else if( pai <= CMjEnum::MJ_TYPE_FB &&  pDesk->m_GameData.m_mjAction.bChiFeng )
	{//�Զ�������
		switch(pai)
		{
		case CMjEnum::MJ_TYPE_FD://��
			{
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FN) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FX))//������
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FN) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FX)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FN;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FX;
						index++;
						canchi = true;
					}
				}
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FN) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FB))//���ϱ�
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FN) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FB)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FN;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FB;
						index++;
						canchi = true;
					}
				}
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FX) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FB))//������
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FX) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FB)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FX;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FB;
						index++;
						canchi = true;
					}
				}
			}
			break;
		case CMjEnum::MJ_TYPE_FN://��
			{
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FD) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FX))//�϶���
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FD) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FX)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FN;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FX;
						index++;
						canchi = true;
					}
				}
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FD) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FB))//�϶���
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FD) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FB)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FN;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FB;
						index++;
						canchi = true;
					}
				}
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FX) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FB))//������
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FX) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FB)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FN;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FX;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FB;
						index++;
						canchi = true;
					}
				}
			}
			break;
		case CMjEnum::MJ_TYPE_FX://��
			{
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FD) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FN))//������
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FD) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FN)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FX;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FN;
						index++;
						canchi = true;
					}
				}
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FN) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FB))//���ϱ�
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FN) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FB)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FX;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FN;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FB;
						index++;
						canchi = true;
					}
				}
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FD) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FB))//������
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FD) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FB)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FX;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FB;
						index++;
						canchi = true;
					}
				}
			}
			break;
		case CMjEnum::MJ_TYPE_FB://��
			{
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FD) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FN))//������
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FD) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FN)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FB;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FN;
						index++;
						canchi = true;
					}
				}
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FD) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FX))//������
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FD) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FX)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FB;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FD;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FX;
						index++;
						canchi = true;
					}
				}
				if(pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FN) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FX))//������
				{
					if(pDesk->m_GameData.m_mjAction.bChiJing || (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FN) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FX)))
					{
						ResultData.m_iChi[index][0] = CMjEnum::MJ_TYPE_FB;
						ResultData.m_iChi[index][1] = CMjEnum::MJ_TYPE_FN;
						ResultData.m_iChi[index][2] = CMjEnum::MJ_TYPE_FX;
						index++;
						canchi = true;
					}
				}
			}
			break;
		}
	}
	else if(pDesk->m_GameData.m_mjAction.bChiJian &&  pai <= CMjEnum::MJ_TYPE_BAI && pai >= CMjEnum::MJ_TYPE_ZHONG)
	{//���з���
		if(pai==CMjEnum::MJ_TYPE_ZHONG && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FA) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_BAI))
		{
			canchi = true;
		}
		else if(pai==CMjEnum::MJ_TYPE_FA && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_ZHONG) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_BAI))
		{
			canchi = true;
		}
		else if(pai==CMjEnum::MJ_TYPE_BAI && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_ZHONG) && pDesk->m_UserData.IsHaveAHandPai(station,CMjEnum::MJ_TYPE_FA))
		{
			canchi = true;
		}
		if(canchi)
		{
			if(pDesk->m_GameData.m_mjAction.bChiJing 
				|| (!pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_ZHONG) && !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_FA) 
				&& !pDesk->m_UserData.m_StrJing.CheckIsJing(CMjEnum::MJ_TYPE_BAI))
				)
			{
				ResultData.m_iChi[0][0] = CMjEnum::MJ_TYPE_ZHONG;
				ResultData.m_iChi[0][1] = CMjEnum::MJ_TYPE_FA;
				ResultData.m_iChi[0][2] = CMjEnum::MJ_TYPE_BAI;
			}
			else
			{
				canchi=false;
			}
		}
	}
	if(canchi)//�ܳ�
	{
		ResultData.bChi = true;
		ResultData.bCanAction = true;
		if(ResultData.m_byMaxThing < ACTION_CHI)
		{
			ResultData.m_byMaxThing = ACTION_CHI;
		}
	}
	return canchi;
}

///�ܷ�����
bool CCheckCPGAction::CanPeng(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData)
{

	//���ƻ��й�״̬�������
	if(!pDesk->m_GameData.m_mjAction.bPeng)
		return false;
	if(pDesk->m_UserData.IsUserState( station,USER_STATE_TING) || pDesk->m_UserData.IsUserState( station,USER_STATE_AUTO))
		return false;
	if(pai != 255 && pDesk->m_UserData.GetAHandPaiCount(station,pai) >= 2)
	{
		ResultData.bPeng = true;
		ResultData.bCanAction = true;
		if(ResultData.m_byMaxThing < ACTION_PENG)
		{
			ResultData.m_byMaxThing = ACTION_PENG;
		}
		return true;
	}
	return false;
}

///�ܷ����
bool CCheckCPGAction::CanGang(BYTE station,BYTE pai,tagCPGNotifyEx &ResultData,bool me)
{
	//���ƻ��й�״̬������
	if(pDesk->m_UserData.IsUserState( station,USER_STATE_TING) || pDesk->m_UserData.IsUserState( station,USER_STATE_AUTO))
		return false;
	if(!pDesk->m_GameData.m_mjAction.bGang)
		return false;
	bool bb = false;
	if(me)//���Լ����ƣ����ܺͲ���
	{	
		int temp = 0,index=0;
		for(int i=0;i<HAND_CARD_NUM;i++)
		{
			if(pDesk->m_UserData.m_byArHandPai[station][i] == 255)
				continue;
			if(pDesk->m_UserData.GetAHandPaiCount(station,pDesk->m_UserData.m_byArHandPai[station][i])>=4 && temp != pDesk->m_UserData.m_byArHandPai[station][i])
			{
				ResultData.m_iGangData[index][1] = pDesk->m_UserData.m_byArHandPai[station][i];
				ResultData.m_iGangData[index][0] = ACTION_AN_GANG;
				temp = pDesk->m_UserData.m_byArHandPai[station][i];
				bb = true;
				index++;
				continue;
			}
			if(pDesk->m_GameData.m_mjAction.bBuGang)
			{
				if(pDesk->m_UserData.IsUserHavePengPai(station,pDesk->m_UserData.m_byArHandPai[station][i]))
				{
					ResultData.m_iGangData[index][1] = pDesk->m_UserData.m_byArHandPai[station][i];
					ResultData.m_iGangData[index][0] = ACTION_BU_GANG;
					temp = pDesk->m_UserData.m_byArHandPai[station][i];
					bb = true;
					index++;
				}
			}

		}
	}
	else//����
	{
		if(pai == 255 || pai == 0)
			return false;
		if(pDesk->m_UserData.GetAHandPaiCount(station,pai)>=3 )
		{
			bb = true;
			ResultData.m_iGangData[0][1] = pai;
			ResultData.m_iGangData[0][0] = ACTION_MING_GANG;
		}
	}
	if(bb)
	{
		ResultData.bGang = true;
		ResultData.bCanAction = true;
		if(ResultData.m_byMaxThing < ResultData.m_iGangData[0][0])
		{
			ResultData.m_byMaxThing = ResultData.m_iGangData[0][0];
		}
	}
	return bb;
}
////���๹��ʱ�����ݶ����ַ�����Ӷ����ָ��
//void CCheckCPGAction::SetDataPoint(UserDataEx *userdata)
//{
//	pUserData = userdata;
//}

//��ȡ�����ָ��
void CCheckCPGAction::SetDeskPoint(CSrvLogic *desk)
{
	pDesk = desk;
}

