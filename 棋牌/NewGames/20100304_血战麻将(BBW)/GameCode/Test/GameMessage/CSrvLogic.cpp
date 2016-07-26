
#include "stdafx.h"
#include "CSrvLogic.h"
#include "../TestDlg.h"

CSrvLogic::CSrvLogic(void)
{	

	m_CPGAction.SetDeskPoint(this);//�����ܼ�ⷽ��
	m_CheckHuPai.SetDeskPoint(this);//���Ƽ�ⷽ��
	m_CountFen.SetDeskPoint(this);//���
}
CSrvLogic::~CSrvLogic(void)
{
}
//��ȡ�����ָ��
void CSrvLogic::SetDeskPoint(CTestDlg *desk)
{
	pDesk = desk;
}

//���������¼�������������Ӿͻ�ͨ���ýӿڻ�ȡ��Ϸ������
bool CSrvLogic::GetGameStaton(BYTE bDeskStation, bool bWatchUser,BYTE GameStation)
{

	GameStationDataEx GameStationData;
	GameStationData.Init();
	GameStationData.iVersion	=  MAIN_VERSION;
	GameStationData.iVersion2	=  ASS_VERSION;
	//��ȡ���䱶�����ܷ���
	GameStationData.Stationpara = pDesk->GetGameStation();		 //��Ϸ״̬

	m_UserData.CopyAllGCPData(GameStationData.m_UserGCPData);
	m_UserData.CopyAllOutPai(GameStationData.m_byArOutPai);
	m_UserData.CopyAllMenPai(GameStationData.m_iArMenPai);
	m_UserData.CopyAllHandPai(GameStationData.m_byArHandPai,bDeskStation,false);
	if(!bWatchUser)
	{
		m_UserData.SetUserState(bDeskStation,USER_STATE_CUT,false);//ȡ������״̬
		m_UserData.SetUserState(bDeskStation,USER_STATE_AUTO,false);//ȡ���й�״̬
	}

	GameStationData.m_bitUserState = m_UserData.m_bitUserState;//��ȡ���״̬
	GameStationData.m_byOtherOutPai = m_UserData.m_byOtherOutPai;
	GameStationData.m_byThingDoing = m_GameData.m_byThingDoing;
	GameStationData.byNtStation = m_UserData.m_byNtStation;
	GameStationData.m_byNowOutStation = m_UserData.m_byNowOutStation;
	GameStationData.m_StrJing = m_UserData.m_StrJing;

	pDesk->GetUserAgree(GameStationData.ucAgree);//��ȡ������ҵ�ͬ��״̬

	char path[500];
	sprintf(path,"%s%s_s.ini",CINIFile::GetAppPath (),SKIN_FOLDER);
	CINIFile pz(path);
	char key[500];
	sprintf(key,"game");
	m_GameData.m_MjSet.bAutoBegin	= pz.GetKeyVal(key,TEXT("bAutoBegin"),0);		//�Ƿ��Զ���ʼ
	m_GameData.m_MjSet.byAutoTime = pz.GetKeyVal(key,TEXT("byAutoTime"),3);			//�йܳ���ʱ��
	m_GameData.m_MjSet.byBeginTime = pz.GetKeyVal(key,TEXT("byBeginTime"),30);		//��ʼ�ȴ�ʱ��
	m_GameData.m_MjSet.byBlockTime = pz.GetKeyVal(key,TEXT("byBlockTime"),15);		//���Ƶȴ�ʱ��
	m_GameData.m_MjSet.byOutcardTime = pz.GetKeyVal(key,TEXT("byOutcardTime"),20);	//���Ƶȴ�ʱ��

	memcpy(&GameStationData.m_MjSet,&m_GameData.m_MjSet,sizeof(GameStationData.m_MjSet));//�齫���ã���Ҫ��ʱ�����ã�
	//char strdug[500];
	//sprintf(strdug,"�¼��� bAutoBegin %d byAutoTime %d byBeginTime %d byBlockTime %d byOutcardTime %d",GameStationData.m_MjSet.bAutoBegin,GameStationData.m_MjSet.byAutoTime,GameStationData.m_MjSet.byBeginTime,GameStationData.m_MjSet.byBlockTime,GameStationData.m_MjSet.byOutcardTime);
	//OutputDebugString(strdug);

	switch (GameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
		{
			//��������
			SendGameStation(bDeskStation, bWatchUser, &GameStationData, sizeof(GameStationData));
			MessageStruct Mess;
			ZeroMemory(Mess.Message, 200);
			lstrcpy(Mess.Message, TEXT("�gӭ�����錢�Α�,ף������_�ģ�"));
			SendGameData(bDeskStation, &Mess, sizeof(Mess), ASS_GM_GAME_NOTIFY, ASS_MESSAGE, 0);
		}
		break;
	case GS_WAIT_ARGEE:	//�ȴ���ҿ�ʼ״̬
	case GS_SEND_CARD:	//����״̬(��δ����)
	case GS_PLAY_GAME:	//��Ϸ��״̬
	case GS_WAIT_NEXT:	//�ȴ���һ����Ϸ��ʼ
		{
			//��������
			SendGameStation(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
		}
		break;
	}
	return true;
}

///���ղ�������Ϣ
int CSrvLogic::HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser)
{
	pDesk->KillTimer(TIME_CHECK_GAME_MSG);
	pDesk->SetTimer(TIME_CHECK_GAME_MSG,TIME_DEFAULT_HANDLE);//�������ó�ʱ��ʱ��
	switch (code)
	{
	case ASS_SEZIFINISH:	//ɫ�Ӷ�������
		{
			if(m_GameData.m_byGameState != STATE_BEGIN_OUT)///�������Ϸ����Ϣ���״̬
			{
				OutputDebugString("״̬��⣺ �¼� ReceiveSeziFinish ���ڳ���״̬");
				return 0;
			}
			return ReceiveSeziFinish(bDeskStation, pData, uSize, bWatchUser);
		}
	case ASS_AUTOCARD://�й���Ϣ
		{	
			return ReceiveUserTuoGuan(bDeskStation, pData, uSize, bWatchUser);
		}
		break;
	case EVENT_OUT_PAI:// �û�������Ϣ
		{
			if(m_GameData.m_byGameState != STATE_OUT_CARD)
			{
				OutputDebugString("״̬��⣺�¼� ReceiveUserOutCard ���ڳ���״̬");
				return 0;
			}
			return ReceiveUserOutCard(bDeskStation, pData, uSize, bWatchUser);
		}
		break;
	case ASS_PASS_CHECK://��ҷ���������Ϣ	
		{
			if(m_GameData.m_byGameState != STATE_OUT_CARD && m_GameData.m_byGameState != STATE_BLOCK_CARD && m_GameData.m_byGameState != STATE_QIANG_GANG)
			{
				OutputDebugString("״̬��⣺�¼� ReceiveUserPassCheck ����״̬");
				return 0;
			}
			return ReceiveUserPassCheck(bDeskStation, pData, uSize,   bWatchUser);
		}
		break;
	case ASS_GM_AGREE_GAME:	//�յ��û�ͬ����Ϸ��Ϣ	
		{
			if(m_GameData.m_byGameState != STATE_BEGIN_OUT && m_GameData.m_byGameState != STATE_FINISH)
			{
				OutputDebugString("״̬��⣺�¼� ReceiveUserAgreeGame ����״̬");
				return 0;
			}
			return ReceiveUserAgreeGame(bDeskStation, pData, uSize,   bWatchUser);
		}
		break;
	case EVENT_CHI_PAI:	// �û����� ��Ϣ
		{
			if(m_GameData.m_byGameState != STATE_BLOCK_CARD)
			{
				OutputDebugString("״̬��⣺�¼� ReceiveUserChiCard ��������״̬");
				return 0;
			}
			return ReceiveUserChiCard(bDeskStation, pData, uSize, bWatchUser);
		}
		break;
	case EVENT_PENG_PAI:// �û����� ��Ϣ
		{
			if(m_GameData.m_byGameState != STATE_BLOCK_CARD)
			{
				OutputDebugString("״̬��⣺�¼� ReceiveUserPengCard ��������״̬");
				return 0;
			}
			return ReceiveUserPengCard(bDeskStation, pData, uSize, bWatchUser);
		}
		break;;
	case EVENT_GANG_PAI:// �û����� ��Ϣ
		{
			if(m_GameData.m_byGameState != STATE_BLOCK_CARD && m_GameData.m_byGameState != STATE_OUT_CARD)
			{
				OutputDebugString("״̬��⣺�¼� ReceiveUserGangCard ����״̬");
				return 0;
			}
			return ReceiveUserGangCard(bDeskStation, pData, uSize, bWatchUser);
		}
		break;
	case EVENT_TING_PAI:// �û����� ��Ϣ
		{
			if(m_GameData.m_byGameState != STATE_OUT_CARD)
			{
				OutputDebugString("״̬��⣺�¼� ReceiveUserTingCard ����״̬");
				return 0;
			}
			return ReceiveUserTingCard(bDeskStation, pData, uSize, bWatchUser);
		}
		break;
	case EVENT_HU_PAI:	// �û�����	 ��Ϣ
		{
			if(m_GameData.m_byGameState != STATE_BLOCK_CARD && m_GameData.m_byGameState != STATE_OUT_CARD && m_GameData.m_byGameState != STATE_QIANG_GANG)
			{
				OutputDebugString("״̬��⣺�¼� ReceiveUserHuCard ����״̬");
				return 0;
			}
			return ReceiveUserHuCard(bDeskStation, pData, uSize, bWatchUser);
		}
		break;
	case ASS_HAVE_THING://���������뿪	 ��Ϣ
		{
			return ReceiveUserHaveThing(bDeskStation, pData, uSize,   bWatchUser);
		}
		break;
	case ASS_LEFT_RESULT: //ͬ���û��뿪 ��Ϣ
		{
			return ReceiveArgeeUserLeft(bDeskStation, pData, uSize,   bWatchUser);
		}
		break;
	case ASS_EXIT://�ȴ���ʼһ�¾�ʱ�˳�
		{
			//if(pDesk->m_bGameStation==23)
			//	return pDesk->GameFinish(bDeskStation,GF_SALE);
			//else
			//	return 0;
		}
		break;
#ifdef MJ_CAN_SETPAI//�����ͻ���ץ�ƣ�����
	case ASS_SUPER://�����ͻ���,����������
		{
			ReceiveSuperChangePai(bDeskStation,pData, uSize, bWatchUser);
		}
		break;
#endif // MJ_CAN_SETPAI
	}
	return 0;
}

////////////////////////////////////////���������Ϣ//////////////////////////////////////////////////////
// �յ��û�ɫ�Ӷ���������Ϣ
int CSrvLogic::ReceiveSeziFinish(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(!m_UserData.m_bWaitSezi || bWatchUser)//�Ƿ���ȴ�ɫ�Ӷ�������״̬
		return 0;
	m_UserData.m_bSiziFinish[bDeskStation] = true;
	if(!m_UserData.IsAllSeziFinish())//�����û�з���ɫ�ӽ�����Ϣ
		return 0;
	m_UserData.SetWaiteSezi(false);	//�����ٵȴ�ɫ����Ϣ��	
	ApplyAction(255,500);		//�¼�id�Ѿ����ú��ˣ�����Ϳ�����
	return 0;
}

// �յ��й���Ϣ
int CSrvLogic::ReceiveUserTuoGuan(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(bWatchUser || uSize != sizeof(Usertouguan))
		return 0;
	if(bWatchUser || pDesk->GetGameStation() > GS_PLAY_GAME || pDesk->GetGameStation() < GS_SEND_CARD)//������Ϸ�в����й�
		return 0;
    Usertouguan *pCmd = (Usertouguan *)pData;
	Usertouguan touguan;
	touguan.desk = bDeskStation;
	touguan.touguang = pCmd->touguang;
	m_UserData.SetUserState(bDeskStation,USER_STATE_AUTO,touguan.touguang);//�޸��й�״̬
	touguan.m_bitUserState = m_UserData.m_bitUserState;
	SendAllGameData(&touguan,sizeof(Usertouguan),ASS_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
	return 0;
}

// �û�����
int CSrvLogic::ReceiveUserOutCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	tagOutPaiEx * pOutCard = (tagOutPaiEx *)pData;
	if(pOutCard == NULL || bWatchUser || pDesk->GetGameStation() != GS_PLAY_GAME || uSize!=sizeof(tagOutPaiEx)
		||pOutCard->byPs == 255 || m_UserData.GetAHandPaiCount(bDeskStation,pOutCard->byPs)==0 || !m_UserData.IsOutPaiPeople(bDeskStation))
	{
		char strdug[500];
		sprintf(strdug,"�¼��� ���Ϸ������� %d %d %d %d %d %d %d",bWatchUser,pDesk->GetGameStation() != GS_PLAY_GAME,uSize!=sizeof(tagOutPaiEx),pOutCard->byPs, m_UserData.GetAHandPaiCount(bDeskStation,pOutCard->byPs)==0 , !m_UserData.IsOutPaiPeople(bDeskStation),m_UserData.GetHandPaiCount(bDeskStation));
		OutputDebugString(strdug);

		return 0;
	}
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(i == bDeskStation)
			continue;
		m_UserData.SetUserState(i,USER_STATE_GANG,false);//��������״̬
	}
	m_GameData.T_OutPai = * pOutCard;
	m_UserData.m_byOtherOutPai = pOutCard->byPs;
	
	OutputDebugString("�¼���ReceiveUserOutCard  ���Ƴɹ�");

	m_UserData.DelleteAHandPai(bDeskStation,pOutCard->byPs);//ɾ��һ������
	m_UserData.AddToOutPai(bDeskStation,pOutCard->byPs);//���һ�ų���
	m_UserData.SetUserState(bDeskStation,USER_STATE_HAVE_OUT,true);//����Ϊ������״̬
	m_GameData.ClearTempData();//������ʱ���ݣ�֮ǰ���ܵĳ���������
	//��ҵ��������
	m_GameData.T_OutPai.bTing = false;
	if(m_GameData.T_TingPai.bHitTing[bDeskStation] && !m_UserData.IsUserState(bDeskStation,USER_STATE_TING))//��ҵ��������
	{
		m_GameData.T_TingPai.Clear();
		if(CLogicBase::IsHaveInArr(m_GameData.T_CPGNotify[bDeskStation].m_byTingCanOut,pOutCard->byPs,HAND_CARD_NUM))
		{
			m_UserData.SetUserState(bDeskStation,USER_STATE_TING,true);
			m_GameData.T_OutPai.bTing = true;
		}
	}
	//��������
	m_UserData.CopyAllOutPai(m_GameData.T_OutPai.m_byArOutPai);
	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	m_UserData.m_bZhuaPaiDir = true;
	m_GameData.T_OutPai.m_bitUserState = m_UserData.m_bitUserState;//��ȡ���״̬��Ϣ
	//���ͳ�������
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		m_UserData.SortHandPai(i,false);
		m_UserData.CopyAllHandPai(m_GameData.T_OutPai.m_byArHandPai,i,false);
		SendGameData(i, &m_GameData.T_OutPai, sizeof(m_GameData.T_OutPai), ASS_GM_GAME_NOTIFY, EVENT_OUT_PAI, 0);
	}
	//������⣺û��������Ӧ��ֱ��ץ��
	if(!CheckAction(bDeskStation,pOutCard->byPs,2))
	{//û�ж�����ֱ����һ�����ץ��
		ApplyAction(EVENT_ZHUA_PAI,500);	//������һ���¼�

		OutputDebugString("�¼���ReceiveUserOutCard û�г�����");
	}
	else
	{
		OutputDebugString("�¼���ReceiveUserOutCard ��⵽�г�����");

		SetGameState(STATE_BLOCK_CARD);//������Ϸ״̬
		for (int i=0;i<PLAY_COUNT;i++)
		{	
			m_GameData.T_CPGNotify[i].byUser = i;
			if(i==bDeskStation || m_UserData.IsUserState(i,USER_STATE_CUT) || m_GameData.T_CPGNotify[i].m_byMaxThing == ACTION_NO)
				continue;
			SendGameData(i, &m_GameData.T_CPGNotify[i], sizeof(m_GameData.T_CPGNotify[i]), ASS_GM_GAME_NOTIFY, EVENT_CPG_NOTIFY, 0);
		}
	}
	m_UserData.m_byNowOutStation = CLogicBase::GetNextStation(m_UserData.m_byNowOutStation);//ץ��λ��,�ı���λ��
	SetDoingThing(EVENT_OUT_PAI,bDeskStation);
	return 0;
}
// �յ��û�pass ��Ϣ
int CSrvLogic::ReceiveUserPassCheck(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(m_GameData.ApplyThingID != 255 || bWatchUser)//�������¼��Ͳ���Ӧpass��Ϣ
		return 0;
	m_GameData.T_TingPai.Clear();//ȡ�����Ƶ��״̬
	m_GameData.AcceptAction(bDeskStation,ACTION_NO);//���ö�������״̬
	CheckNextAction();	
	return 0;
}
//�յ��û�ͬ����Ϸ��Ϣ
int CSrvLogic::ReceiveUserAgreeGame(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	//�û�ͬ����Ϸ
	if (bWatchUser || (pDesk->GetGameStation() != GS_WAIT_ARGEE) && (pDesk->GetGameStation() != GS_WAIT_NEXT)) 
		return 0;
	SendAllGameData( ASS_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME, 0);
	return 0;
}

//���������뿪
int CSrvLogic::ReceiveUserHaveThing(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return 0;
}
//ͬ���û��뿪
int CSrvLogic::ReceiveArgeeUserLeft(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return 0;
}
// �û�������Ϣ
int CSrvLogic::ReceiveUserChiCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	tagChiPaiEx * pChiCard = (tagChiPaiEx *)pData;
	if(pChiCard == NULL || bWatchUser || pDesk->GetGameStation() != GS_PLAY_GAME || uSize!=sizeof(tagChiPaiEx)
	|| !m_GameData.T_CPGNotify[bDeskStation].bChi || pChiCard->byPs == 255)
	{//���Ϸ�����
		char strdug[500];
		sprintf(strdug,"�¼��� ���Ϸ��Զ��� %d %d %d %d %d",bWatchUser,pDesk->GetGameStation() != GS_PLAY_GAME,uSize!=sizeof(tagChiPaiEx),!m_GameData.T_CPGNotify[bDeskStation].bChi,pChiCard->byPs == 255);
		OutputDebugString(strdug);
		return 0;
	}
	memcpy(&m_GameData.temp_ChiPai,pChiCard,sizeof(m_GameData.temp_ChiPai));//����������Ϣ����
	m_GameData.temp_ChiPai.byBeChi = m_GameData.T_CPGNotify[bDeskStation].byOutStation;//��¼����λ��
	m_GameData.AcceptAction(pChiCard->byUser,ACTION_CHI);
	//�����һ������
	CheckNextAction();
	OutputDebugString(TEXT("�¼���ReceiveUserChiCard �ɹ�����"));
	return 0;
}
// �û�������Ϣ
int CSrvLogic::ReceiveUserPengCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	tagPengPaiEx * pPengCard = (tagPengPaiEx *)pData;
	if(pPengCard == NULL || bWatchUser ||  pDesk->GetGameStation() != GS_PLAY_GAME || uSize!=sizeof(tagPengPaiEx)
	|| !m_GameData.T_CPGNotify[bDeskStation].bPeng || pPengCard->byPs == 255)
	{
		return 0;
	}
	memcpy(&m_GameData.temp_PengPai,pPengCard,sizeof(m_GameData.temp_PengPai));//����������Ϣ����
	m_GameData.temp_PengPai.byBePeng = m_GameData.T_CPGNotify[bDeskStation].byOutStation;//��¼����λ��
	m_GameData.AcceptAction(pPengCard->byUser,ACTION_PENG);//���ý��յĶ������ж���һ��������ʲôʱ��Ҫʹ�����õ�����
	//�����һ������
	CheckNextAction();
	return 0;
}
// �û�������Ϣ
int CSrvLogic::ReceiveUserGangCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	tagGangPaiEx * pGangCard = (tagGangPaiEx *)pData;
	if(pGangCard == NULL || bWatchUser || pDesk->GetGameStation() != GS_PLAY_GAME || uSize!=sizeof(tagGangPaiEx)
		|| !m_GameData.T_CPGNotify[bDeskStation].bGang || pGangCard->byPs == 255)
	{//���Ϸ�����
		OutputDebugString("�¼������Ʋ��Ϸ�");
		return 0;
	}
	bool have = false;
	for(int i=0;i<4;++i)
	{
		if(pGangCard->byType == m_GameData.T_CPGNotify[bDeskStation].m_iGangData[i][0])//���ڸ�����
		{
			have = true;
		}
	}
	if(!have)
	{
		OutputDebugString("�¼��������ڸ����͸���");
		return 0;
	}
	bool action = false;
	memcpy(&m_GameData.temp_GangPai,pGangCard,sizeof(m_GameData.temp_GangPai));//����������Ϣ����
	m_GameData.temp_GangPai.byBeGang = m_GameData.T_CPGNotify[bDeskStation].byOutStation;//��¼����λ��
	if(pGangCard->byType == ACTION_BU_GANG)
	{
		m_GameData.temp_GangPai.bChenkQ = true;
		m_GameData.T_CPGNotify[bDeskStation].bQiangGang = true;
		action = CheckAction(pGangCard->byUser,pGangCard->byPs,3);//����Ƿ��н�ȡ���ܵĶ��������ܺ��Ƶ�
	}

	switch(pGangCard->byType)
	{
	case ACTION_AN_GANG://���ܿ�����������ִ��
		{
			ApplyAction(EVENT_GANG_PAI,2*100);//����ִ�и����¼�
		}
		break;
	case ACTION_BU_GANG:
		{
			m_GameData.AcceptAction(pGangCard->byUser,ACTION_BU_GANG);//���ý��յĶ������ж���һ��������ʲôʱ��Ҫʹ�����õ�����
			if(!action)//�������ܣ�ֱ�Ӵ���
			{
				ApplyAction(EVENT_GANG_PAI,2*100);//����ִ�и����¼�
			}
			else///�������ܣ�����������Ϣ�����˼��
			{
				SetGameState(STATE_QIANG_GANG);//������Ϸ״̬
				for (int i=0;i<PLAY_COUNT;i++)
				{	
					m_GameData.T_CPGNotify[i].byUser = i;
					if(!m_GameData.T_CPGNotify[i].bCanAction || m_UserData.IsUserState(i,USER_STATE_CUT))
						continue;
					SendGameData(i,&m_GameData.T_CPGNotify[i],sizeof(m_GameData.T_CPGNotify[i]),ASS_GM_GAME_NOTIFY, EVENT_CPG_NOTIFY, 0);
				}
			}
		}
		break;
	case ACTION_MING_GANG://����
		{
			m_GameData.AcceptAction(pGangCard->byUser,ACTION_MING_GANG);//���ý��յĶ������ж���һ��������ʲôʱ��Ҫʹ�����õ�����
			CheckNextAction();//�����һ������
		}
		break;
	}
	return 0;
}
// �û�������Ϣ
int CSrvLogic::ReceiveUserTingCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	tagTingPaiEx * pTingCard = (tagTingPaiEx *)pData;
	if(pTingCard == NULL || bWatchUser || pDesk->GetGameStation() != GS_PLAY_GAME || uSize!=sizeof(tagTingPaiEx)
		|| !m_GameData.T_CPGNotify[bDeskStation].bTing  )
	{//���Ϸ�����
		return 0;
	}
	m_GameData.AcceptAction(pTingCard->byUser,ACTION_TING);//���ý��յĶ������ж���һ��������ʲôʱ��Ҫʹ�����õ�����
	m_GameData.T_TingPai.bHitTing[pTingCard->byUser] = true;//��¼��ҵ��������
	//֪ͨ�������
	SendAllGameData( &m_GameData.T_TingPai, sizeof(m_GameData.T_TingPai), ASS_GM_GAME_NOTIFY, EVENT_TING_PAI, 0);
	return 0;
}
// �û�������Ϣ
int CSrvLogic::ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	tagHuPaiEx * pHuCard = (tagHuPaiEx *)pData;
	if(pHuCard == NULL || bWatchUser || pDesk->GetGameStation() != GS_PLAY_GAME || uSize!=sizeof(tagHuPaiEx) || !m_GameData.T_CPGNotify[bDeskStation].bHu)
	{//���Ϸ�����
		return 0;
	}
	m_GameData.AcceptAction(pHuCard->byUser,ACTION_HU);//���ý��յĶ������ж���һ��������ʲôʱ��Ҫʹ�����õ�����

	m_GameData.T_HuPai.bZimo = m_GameData.T_CPGNotify[bDeskStation].bZimo;
	m_GameData.T_HuPai.byDianPao = m_GameData.T_CPGNotify[bDeskStation].m_byDianPao;
	m_GameData.T_HuPai.bCanH[pHuCard->byUser] = true;
	m_GameData.T_HuPai.byUserNum++;
	m_GameData.T_HuPai.byNt = m_UserData.m_byNtStation;		//ׯ��λ��
	m_GameData.T_HuPai.byPs = m_GameData.T_CPGNotify[bDeskStation].m_iHuPs;	//���ڵ���ֵ
	

	if(m_GameData.T_HuPai.byUser == 255)
	{
		m_GameData.T_HuPai.byUser = pHuCard->byUser;
	}
	else if(!m_GameData.T_HuPai.bZimo)//ѡ�񿿺�������������Һ���
	{//���ں��ƣ������ж����ҿ��Ժ���
		m_GameData.T_HuPai.bDuoXiang = true;//��־����
		m_GameData.T_HuPai.byUser = CLogicBase::GetCanHuUser(m_GameData.T_CPGNotify[bDeskStation].m_byDianPao,m_GameData.T_HuPai.bCanH,m_GameData.T_HuPai.byUser);//��ȡ�������ҽ������
	}

	memcpy(m_GameData.T_HuPai.byHuType[pHuCard->byUser],m_GameData.T_CPGNotify[pHuCard->byUser].byHuType,sizeof(m_GameData.T_CPGNotify[pHuCard->byUser].byHuType));//������������
	m_UserData.CopyAllGCPData(m_GameData.T_HuPai.m_UserGCPData);//��������������
	if(m_GameData.T_HuPai.bZimo)//���������������
	{
		ApplyAction(EVENT_HU_PAI,2*100);//������һ���¼�
	}
	else
	{
		//�����һ������
		CheckNextAction();
	}
	return 0;
}

///�յ������ͻ�����������Ϣ
void CSrvLogic::ReceiveSuperChangePai(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(bWatchUser || bDeskStation<0 || bDeskStation>3)
		return;
	SuperStruct *Super =(SuperStruct* )pData;
	bool cpg = false;
	m_UserData.m_SetData[bDeskStation].Init();
	m_UserData.m_SetData[bDeskStation].bySetType = Super->bySetType;
	m_UserData.m_SetData[bDeskStation].mjcount = Super->mjcount;
	for(int i=0;i<Super->mjcount;++i)
	{
		m_UserData.m_SetData[bDeskStation].mjdata[i] = Super->mjdata[i];
		m_UserData.m_SetData[bDeskStation].changedata[i] = Super->changedata[i];
	}
	OutputDebugString("������ �¼����յ���������Ϣ");
	//�������ͣ�0 �������ƣ�1��ѡ���ƣ�2�����Լ�����һ���ƣ�3������ǿ����һ����
	switch(Super->bySetType)
	{
	case 0://����������
		{
			m_UserData.ChangeAllHandPai(bDeskStation,Super->mjdata,Super->mjcount);
		}
		break;
	case 1://����������
		{
			m_UserData.ChangeSomeHandPai(bDeskStation,Super->mjdata,Super->changedata,Super->mjcount);
		}	
		break;
	case 2://�����Լ�����һ����
		{	
			m_UserData.m_bySetMoPai[bDeskStation] = Super->mjdata[0];
		}
		break;
	case 3://������ǽ����һ����
		{
			m_UserData.SetGameNextPai(Super->mjdata[0]);
		}
		break;
	}
	if(Super->bySetType == 0 || Super->bySetType == 1)
	{
		if(bDeskStation == m_UserData.m_byNowOutStation)/////����ǵ�ǰ������ң���Ⲣ���ؼ����
		{
			if(CheckAction(bDeskStation,m_UserData.GetLastHandPai(bDeskStation),0))
			{
				cpg = true;
			}
		}
		OutputDebugString("������ �¼�����������������");
		SuperStruct superdata = *Super;
		memcpy(superdata.mjdata,m_UserData.m_byArHandPai[bDeskStation],sizeof(superdata.mjdata));
		SendGameData(bDeskStation, &superdata, sizeof(SuperStruct), ASS_GM_GAME_NOTIFY, ASS_SUPER, 0);
	}
	if(cpg)//���ƺ��ܽ��г����ܲ���
	{
		if(!m_UserData.IsUserState(bDeskStation,USER_STATE_CUT))
		{
			m_GameData.T_CPGNotify[bDeskStation].byUser = bDeskStation;
			SendGameData(bDeskStation, &m_GameData.T_CPGNotify[bDeskStation], sizeof(tagCPGNotifyEx), ASS_GM_GAME_NOTIFY, EVENT_CPG_NOTIFY, 0);
		}
	}
}

///���ƣ�CheckNextAction
///�����������һ������,��ҷ���������Ϣ���߷�����Ϣ���ñ������ж���Ҫ�����ȴ���Ϣ���п��Խ�����һ������
void CSrvLogic::CheckNextAction()
{
	BYTE max = 0;
	if(!IsCanHandle(max))//��Ҫ�����ȴ���Ϣ��˵���и������ȼ��Ķ���Ҫ�ȴ�
	{
		char strdug[500];
		sprintf(strdug,"�¼���IsCanHandle ���ڸ����¼� %d",max);
		OutputDebugString(strdug);
		return ;
	}	
	switch(m_GameData.m_byNextAction)//ȷ������һ������Ӧ�¼����ԣ������ܣ���
	{
	case ACTION_CHI://���ƶ���
		{
			ApplyAction(EVENT_CHI_PAI,100);//����ִ�г����¼�
		}
		break;
	case ACTION_PENG:			//���ƶ���
		{
			ApplyAction(EVENT_PENG_PAI,100);//����ִ�������¼�
		}
		break;
	case ACTION_BU_GANG://���ܶ���
	case ACTION_MING_GANG:			//���ܶ���
		{
			ApplyAction(EVENT_GANG_PAI,100);//����ִ�и����¼�
		}
		break;
	case ACTION_HU:					//������
		{
			ApplyAction(EVENT_HU_PAI,100);//����ִ�к����¼�
		}
		break;
	case ACTION_NO:
		{
			if(m_GameData.m_byThingDoing == EVENT_OUT_PAI)//�����¼�
			{
				ApplyAction(EVENT_ZHUA_PAI,2*100);//����ִ��ץ���¼�
			}
		}
		break;
	}
}

///����Ƿ���Խ�����һ������
bool CSrvLogic::IsCanHandle(BYTE &mx)
{
	BYTE max = ACTION_NO;
	BYTE station = m_GameData.T_OutPai.byUser;
	if(m_GameData.m_byGameState == STATE_QIANG_GANG)//����״̬
	{//���ܺ��Ȳ���
		station = m_GameData.T_GangPai.byUser;
		max = mx = m_GameData.GetMaxRequet(station);
		if(max == ACTION_NO)
		{
			m_GameData.m_byNextAction = ACTION_BU_GANG;//����ִ���겹��
			return true;
		}
	}
	else//һ����Ƽ��
	{
		max = mx = m_GameData.GetMaxRequet(station);
		if(max == ACTION_NO)//�����������ˣ������������¼�
		{
			m_GameData.m_byNextAction = ACTION_NO;
			return true;
		}
	}
	//���������Ӧ�ˣ�����ִ����
	if(m_GameData.IsNotifyAllReply(station,max))
	{			
		m_GameData.m_byNextAction = max;
		return true;
	}
	return false;
}
///////////�����¼�����///////////////////////////////////////////////////////////////////////////////////////

//�����¼�,typeҪ������¼�
void CSrvLogic::ApplyAction(int type,int time)
{
	char strdug[500];
	sprintf(strdug,"�¼���ApplyAction type =%d time = %d",type, time);
	OutputDebugString(strdug);

	if(type != 255)
	{
		m_GameData.ApplyThingID = type;
	}
	if(m_GameData.ApplyThingID != 255)
	{
		pDesk->SetTimer(10,time);
	}
}

///���ƣ�GameCPU
///��������Ϸ�¼��������ģ�
///@param typeҪ������¼�����
void CSrvLogic::GameCPU(int type)
{
	char strdug[500];
	sprintf(strdug,"�¼���GameCPU type =%d ����λ�� %d",type,m_UserData.m_byNowOutStation);
	OutputDebugString(strdug);

	////���õȴ�ɫ�Ӷ�������״̬
	BYTE nextID = 255,time = 255;
	switch(type)
	{
	case EVENT_BEGIN://��Ϸ��ʼ�¼�
		{
			ExecuteGameBegin(nextID,time);
		}
		break;
	case EVENT_DONG_NT://�׾��Զ�Ϊׯ�¼�
		{
			ExecuteDongNt(nextID,time);
		}
		break;
	case EVENT_2SEZI_NT	://�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�
		{
			ExecuteTwoSeziNt(nextID,time);
		}
		break;
	case EVENT_2SEZI_NT_GP://�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�
		{
			ExecuteTwoSeziNtAndGp(nextID,time);
		}
		break;
	case EVENT_2SEZI_DIR://��2��ɫ���¼������Ʒ����¼�
		{
			ExecuteTwoSeziDir(nextID,time);
		}
		break;
	case EVENT_2SEZI_DIR_GP	://��2��ɫ���¼������Ʒ�������Ƶ��¼�
		{
			ExecuteTwoSeziDirGp(nextID,time);
		}
		break;
	case EVENT_2SEZI_GP	://��2��ɫ���¼������Ƶ��¼�
		{
			ExecuteTwoSeziGp(nextID,time);
		}
		break;
	case EVENT_2SEZI_NT_DIR_GP://��ɫ��ȷ��ׯ�ҷ������
		{
			ExecuteTwoSeziNtDirGp(nextID,time);
		}
		break;
	case EVENT_SEND_PAI://�����¼�
		{
			pDesk->SetGameStation(GS_SEND_CARD) ;
			ExecuteSendPai(nextID,time);
		}
		break;
	case EVENT_2SEZI_JING://��2��ɫ�Ӷ������¼�
		{
			ExecuteTwoSeziMakeJing(nextID,time);
		}
		break;
	case EVENT_ALL_BUHUA://Ϊ������Ҳ����¼�
		{
			ExecuteAllBuHua(nextID,time);
		}
		break;
	case EVENT_ONE_BUHUA://�������ץ�����ƺ󲹻��¼�
		{
			ExecuteOneBuHua(nextID,time);
		}
		break;
	case EVENT_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�
		{
			pDesk->SetGameStation(GS_PLAY_GAME) ;
			ExecuteBeginOut(nextID,time);
		}
		break;
	case EVENT_ZHUA_PAI	://ץ���¼�
		{	
			ExecuteZhuaPai(nextID,time);
		}
		break;
	case EVENT_CHI_PAI	://�����¼�
		{	
			ExecuteChiPai(nextID,time);
		}
		break;
	case EVENT_PENG_PAI	://�����¼�
		{	
			ExecutePengPai(nextID,time);
		}
		break;
	case EVENT_GANG_PAI	://�����¼�
		{	
			ExecuteGangPai(nextID,time);
		}
		break;
	case EVENT_HU_PAI://�����¼�
		{
			ExecuteHuPai(nextID,time);
		}
		break;
	case EVENT_COUNT_FEN://����¼�
		{
			ExecuteCountFen(nextID,time);
		}
		break;
	case EVENT_END_HANDLE://���������¼�
		{
			ExecuteNormalEnd(nextID,time);
		}
		break;
	case EVENT_ENG_UN_NORMAL://�쳣���������¼�
		{
			ExecuteUnNormalEnd(nextID,time);
		}
		break;
	}
	m_GameData.ApplyThingID = nextID;
	ApplyAction(nextID,time*100);//������һ���¼�
}

///��ʼ�¼�����
void CSrvLogic::ExecuteGameBegin(BYTE &nextID,BYTE &time)
{
	pDesk->KillTimer(TIME_WAIT_MSG);
	pDesk->SetTimer(TIME_WAIT_MSG,1000);//�������ó�ʱ��ʱ��

	time = 10;
	m_GameData.InitData();//��ʼ������
	m_UserData.InitData();

	if(m_UserData.m_byNtStation!=255)//��ׯ����
	{//�Ѿ�����ׯ��
		nextID = EVENT_2SEZI_DIR_GP;//��ɫ�ӷ���
	}
	else 
	{
		nextID = EVENT_DONG_NT;//ִ���Զ�Ϊׯ�¼�
	}
	m_GameData.T_Begin.byNt = m_UserData.m_byNtStation;			// ׯ��λ��			
	m_GameData.T_Begin.byPlayCount = m_UserData.byPlayCount;	// ��Ϸ���еľ���	
	m_GameData.T_Begin.byQuanFeng =  m_UserData.m_byQuanFeng;	// Ȧ������			
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<m_GameData.m_mjRule.byAllMjCount/4;j++)
		{
			m_GameData.T_Begin.byMenPai[i][j] = 0; //����
		}
	}
	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	m_UserData.m_bZhuaPaiDir = true;
	m_UserData.m_MenPai.byGetPaiDir = m_UserData.m_byNtStation;//���Ʒ���(ÿ�ֳ�ʼ��Ϊׯ����ǰ����)
	//���Ϳ�ʼ��Ϣ
	SendAllGameData(&m_GameData.T_Begin,sizeof(m_GameData.T_Begin),ASS_GM_GAME_NOTIFY,EVENT_BEGIN,0);
	//��ʱ���ã�
	SetDoingThing(EVENT_BEGIN,0);//�������ڷ������¼�
	SetGameState(STATE_BEGIN_OUT);//������Ϸ״̬
}

///�Ե�һ�����Ϊׯ�ң��Զ�Ϊׯ�¼���
void CSrvLogic::ExecuteDongNt(BYTE &nextID,BYTE &time)
{
	nextID = EVENT_2SEZI_DIR_GP;
	time = 2;	//200������
	m_GameData.T_DongNt.byNt = m_UserData.m_byNtStation = 0;			// ׯ��λ��	
	m_UserData.m_MenPai.byGetPaiDir = m_UserData.m_byNtStation;//���Ʒ���(ÿ�ֳ�ʼ��Ϊׯ����ǰ����)
	//������Ϸ��Ϣ
	SendAllGameData(&m_GameData.T_DongNt,sizeof(m_GameData.T_DongNt),ASS_GM_GAME_NOTIFY,EVENT_DONG_NT,0);
	//��ʱ����
	SetDoingThing(EVENT_DONG_NT,0);//�������ڷ������¼�
}

///��2��ɫ��ȷ��ׯ�ң������ͣ�
void CSrvLogic::ExecuteTwoSeziNt(BYTE &nextID,BYTE &time)	
{
	nextID = EVENT_2SEZI_DIR_GP;//�¸��¼���id
	time = 2;	//200������
	///��2��ɫ�ӵĵ�����Ϊׯ�¼�
	m_GameData.T_TwoSeziNt.byUser = 0;//��ɫ��λ��
	if(m_UserData.m_byNtStation!=255)
	{//�Ѿ���ׯ���˾Ͳ���ɫ����
		return;
	}
	time = 50;	//5����
	CLogicBase::GetSeziData(&m_GameData.T_TwoSeziNt.bySezi0,&m_GameData.T_TwoSeziNt.bySezi1);
	m_UserData.m_byNtStation = (m_GameData.T_TwoSeziNt.bySezi0 + m_GameData.T_TwoSeziNt.bySezi1)%4;//ׯ��λ�ã�2ɫ��֮��
	m_GameData.T_TwoSeziNt.byNt = m_UserData.m_byNtStation;
	m_UserData.m_MenPai.byGetPaiDir = m_UserData.m_byNtStation;//���Ʒ���(ÿ�ֳ�ʼ��Ϊׯ����ǰ����)
	//������Ϸ��Ϣ
	SendAllGameData( &m_GameData.T_TwoSeziNt, sizeof(m_GameData.T_TwoSeziNt), ASS_GM_GAME_NOTIFY, EVENT_2SEZI_NT, 0);
	//���õȴ�ɫ�Ӷ�������״̬
	m_UserData.SetWaiteSezi(true);
	//��ʱ����
	SetDoingThing(EVENT_2SEZI_NT,0);//�������ڷ������¼�
}
///��2��ɫ��ȷ��ׯ�Һ����ƶ�����������ׯ�ң���������
void CSrvLogic::ExecuteTwoSeziNtAndGp(BYTE &nextID,BYTE &time)
{
	nextID = EVENT_SEND_PAI;
	time=50;
	CLogicBase::GetSeziData(&m_GameData.T_TwoSeziNtAndGetPai.bySezi0,&m_GameData.T_TwoSeziNtAndGetPai.bySezi1);
	m_UserData.m_byNtStation = (m_GameData.T_TwoSeziNtAndGetPai.bySezi0 + m_GameData.T_TwoSeziNtAndGetPai.bySezi1)%4;//ׯ��λ�ã�2ɫ��֮��
	m_GameData.T_TwoSeziNtAndGetPai.byNt = m_UserData.m_byNtStation;
	m_UserData.m_MenPai.byGetPaiDir = m_UserData.m_byNtStation;//���Ʒ���(ÿ�ֳ�ʼ��Ϊׯ����ǰ����)
	m_UserData.m_MenPai.byGetPai = m_GameData.T_TwoSeziNtAndGetPai.bySezi0*2%13;
	if(m_GameData.T_TwoSeziNtAndGetPai.bySezi1>m_GameData.T_TwoSeziNtAndGetPai.bySezi0)
	{
		m_UserData.m_MenPai.byGetPai = m_GameData.T_TwoSeziNtAndGetPai.bySezi1*2%13;
	}
	//������Ϸ��Ϣ
	SendAllGameData( &m_GameData.T_TwoSeziNtAndGetPai, sizeof(m_GameData.T_TwoSeziNtAndGetPai), ASS_GM_GAME_NOTIFY, EVENT_2SEZI_NT_GP, 0);
	//���õȴ�ɫ�Ӷ�������״̬
	m_UserData.SetWaiteSezi(true);
	//��ʱ����
	SetDoingThing(EVENT_2SEZI_NT_GP,0);//�������ڷ������¼�
}

///��2��ɫ��ȷ��ׯ��+���Ʒ���+���ƶ�����������ׯ�ң���㷽��С�������
void CSrvLogic::ExecuteTwoSeziNtDirGp(BYTE &nextID,BYTE &time)	
{
	nextID = EVENT_SEND_PAI;
	time=50;
	m_GameData.T_TwoSeziNtAndGetPai.byUser = 0;
	CLogicBase::GetSeziData(&m_GameData.T_TwoSeziNtAndGetPai.bySezi0,&m_GameData.T_TwoSeziNtAndGetPai.bySezi1);
	if(m_UserData.m_byNtStation!=255)
	{//�Ѿ�����ׯ��
		m_GameData.T_TwoSeziNtAndGetPai.byUser = m_UserData.m_byNtStation;
	}
	else
	{
		m_UserData.m_byNtStation = (m_GameData.T_TwoSeziNtAndGetPai.bySezi0 + m_GameData.T_TwoSeziNtAndGetPai.bySezi1)%4;
	}
	m_GameData.T_TwoSeziNtAndGetPai.byNt = m_UserData.m_byNtStation;
	m_GameData.T_TwoSeziNtAndGetPai.byGetPai = m_GameData.T_TwoSeziNtAndGetPai.bySezi0;
	m_UserData.m_MenPai.byGetPaiDir = m_GameData.T_TwoSeziNtAndGetPai.bySezi1%4;
	if(m_GameData.T_TwoSeziNtAndGetPai.bySezi0>m_GameData.T_TwoSeziNtAndGetPai.bySezi1)
	{
		m_GameData.T_TwoSeziNtAndGetPai.byGetPai = m_GameData.T_TwoSeziNtAndGetPai.bySezi1;
		m_UserData.m_MenPai.byGetPaiDir = m_GameData.T_TwoSeziNtAndGetPai.bySezi0%4;
	}
	//������Ϸ��Ϣ
	SendAllGameData( &m_GameData.T_TwoSeziNtAndGetPai, sizeof(m_GameData.T_TwoSeziNtAndGetPai), ASS_GM_GAME_NOTIFY, EVENT_2SEZI_NT, 0);
	//���õȴ�ɫ�Ӷ�������״̬
	m_UserData.SetWaiteSezi(true);
	//��ʱ����
	SetDoingThing(EVENT_2SEZI_NT_GP,0);//�������ڷ������¼�
}

///��2��ɫ��ȷ�����Ʒ��򣨵�����ȷ������
void CSrvLogic::ExecuteTwoSeziDir(BYTE &nextID,BYTE &time)	
{
	nextID = EVENT_2SEZI_GP;//׼��ִ����ɫ��ȷ�����ƶ����¼�
	time=50;
	m_GameData.T_TwoSeziDir.byUser = m_UserData.m_byNtStation;
	m_GameData.T_TwoSeziDir.byNt = m_UserData.m_byNtStation;
	CLogicBase::GetSeziData(&m_GameData.T_TwoSeziDir.bySezi0,&m_GameData.T_TwoSeziDir.bySezi1);
	m_UserData.m_MenPai.byGetPaiDir = (m_GameData.T_TwoSeziDir.bySezi1 + m_GameData.T_TwoSeziDir.bySezi0)%4;
	m_GameData.T_TwoSeziDir.byDirection = m_UserData.m_MenPai.byGetPaiDir;
	//������Ϸ��Ϣ
	SendAllGameData(&m_GameData.T_TwoSeziDir, sizeof(m_GameData.T_TwoSeziDir), ASS_GM_GAME_NOTIFY, EVENT_2SEZI_DIR, 0);
	//���õȴ�ɫ�Ӷ�������״̬
	m_UserData.SetWaiteSezi(true);
	//��ʱ����
	SetDoingThing(EVENT_2SEZI_DIR,0);//�������ڷ������¼�
}

///��2��ɫ��ȷ�����Ʒ�������ƶ����������ͷ���С�������
void CSrvLogic::ExecuteTwoSeziDirGp(BYTE &nextID,BYTE &time)	
{	
	nextID = EVENT_SEND_PAI;
	time=50;
	m_GameData.T_TwoSeziDirAndGetPai.byUser = m_UserData.m_byNtStation;
	CLogicBase::GetSeziData(&m_GameData.T_TwoSeziDirAndGetPai.bySezi0,&m_GameData.T_TwoSeziDirAndGetPai.bySezi1);
	m_UserData.m_MenPai.byGetPaiDir = (m_GameData.T_TwoSeziDirAndGetPai.bySezi1 + m_GameData.T_TwoSeziDirAndGetPai.bySezi0)%4-1;//���Ʒ���
	m_UserData.m_MenPai.byGetPai = m_GameData.T_TwoSeziDirAndGetPai.bySezi0;	//���ƶ���
	if(m_GameData.T_TwoSeziDirAndGetPai.bySezi0 > m_GameData.T_TwoSeziDirAndGetPai.bySezi1)
	{
		m_UserData.m_MenPai.byGetPai = m_GameData.T_TwoSeziDirAndGetPai.bySezi1;
	}
	m_GameData.T_TwoSeziDirAndGetPai.byGetPai = m_UserData.m_MenPai.byGetPai;

	char str[500];
	sprintf(str,"�¼���ExecuteTwoSeziDirGp λ��%d ������%d %d",m_GameData.T_TwoSeziDirAndGetPai.byUser,m_GameData.T_TwoSeziDirAndGetPai.bySezi0,m_GameData.T_TwoSeziDirAndGetPai.bySezi1);
	OutputDebugString(str);

	//������Ϸ��Ϣ
	SendAllGameData( &m_GameData.T_TwoSeziDirAndGetPai, sizeof(m_GameData.T_TwoSeziDirAndGetPai), ASS_GM_GAME_NOTIFY, EVENT_2SEZI_DIR_GP, 0);
	//���õȴ�ɫ�Ӷ�������״̬
	m_UserData.SetWaiteSezi(true);
	//��ʱ����
	SetDoingThing(EVENT_2SEZI_DIR_GP,0);//�������ڷ������¼�
}

///��2��ɫ��ȷ�����ƶ����������Ͷ�����
void CSrvLogic::ExecuteTwoSeziGp(BYTE &nextID,BYTE &time)	
{
	nextID = EVENT_SEND_PAI	;//׼��ִ�з����¼�	
	time = 50;
	m_GameData.T_TwoSeziGetPai.byUser = m_UserData.m_byNtStation;
	CLogicBase::GetSeziData(&m_GameData.T_TwoSeziGetPai.bySezi0,&m_GameData.T_TwoSeziGetPai.bySezi1);
	m_UserData.m_MenPai.byGetPai = (m_GameData.T_TwoSeziGetPai.bySezi1 + m_GameData.T_TwoSeziGetPai.bySezi0)%12;
	m_GameData.T_TwoSeziGetPai.byGetPai = m_UserData.m_MenPai.byGetPai;
	//������Ϸ��Ϣ
	SendAllGameData( &m_GameData.T_TwoSeziGetPai, sizeof(m_GameData.T_TwoSeziGetPai), ASS_GM_GAME_NOTIFY, EVENT_2SEZI_GP, 0);
	//���õȴ�ɫ�Ӷ�������״̬
	m_UserData.SetWaiteSezi(true);
	//��ʱ����
	SetDoingThing(EVENT_2SEZI_GP,0);//�������ڷ������¼�
}

///����
void CSrvLogic::ExecuteSendPai(BYTE &nextID,BYTE &time)	
{
	nextID = EVENT_BEGIN_OUT;//��һ���¼��Ĵ��ţ���ʼ����
	time = 2;
	//������
	DisPatchCard();
	//����
	m_UserData.SendCard();
	//��������
	ChangeMenPai();
	m_UserData.CopyAllMenPai(m_GameData.T_SendPai.byMenPai);//��������

	m_GameData.T_SendPai.byAllPaiNum = m_UserData.m_MenPai.byAllPaiNum;//������
	m_GameData.T_SendPai.byStart = m_UserData.m_MenPai.byStart;//���ƿ�ʼλ��
	m_GameData.T_SendPai.byEnd = m_UserData.m_MenPai.byEnd;//���ƽ���λ��

	for(int i=0;i<PLAY_COUNT;i++)
	{	
		m_UserData.CopyAllHandPai(m_GameData.T_SendPai.m_byArHandPai,i,false);
		SendGameData(i, &m_GameData.T_SendPai, sizeof(m_GameData.T_SendPai), ASS_GM_GAME_NOTIFY, EVENT_SEND_PAI, 0);
	}
	//��ʱ����
	SetDoingThing(EVENT_SEND_PAI,0);//�������ڷ������¼�
}

///��2��ɫ�Ӷ����¼�
void CSrvLogic::ExecuteTwoSeziMakeJing(BYTE &nextID,BYTE &time)	
{
	nextID = EVENT_SEND_PAI;
	m_GameData.T_TwoSeziJing.byUser = m_UserData.m_byNtStation;//Ĭ��ׯ����ɫ�Ӷ�����
	CLogicBase::GetSeziData(&m_GameData.T_TwoSeziJing.bySezi0,&m_GameData.T_TwoSeziJing.bySezi1);

	int num = m_GameData.T_TwoSeziJing.bySezi0 + m_GameData.T_TwoSeziJing.bySezi1;
	num = num%13;//����ɫ��ֵ���ܳ���12
	num = (2*num +1);// numҪ�Ӷأ�2�ţ�ת�����ţ�����Ҫ����2
	if(m_UserData.m_MenPai.byEndIndex>=num)
	{
		m_UserData.m_MenPai.byCaiShenIndex = m_UserData.m_MenPai.byEndIndex -num;
	}
	else
	{
		m_UserData.m_MenPai.byCaiShenIndex = m_UserData.m_MenPai.byAllPaiNum + m_UserData.m_MenPai.byEndIndex + 1 - num;
	}
	m_UserData.m_StrJing.Init();
	if(!m_UserData.m_StrJing.AddJing(m_UserData.m_MenPai.byMenPai[m_UserData.m_MenPai.byCaiShenIndex]))
	{
		OutputDebugString("ɫ�Ӷ�����ʧ��");
	}
	if(m_UserData.m_MenPai.byMenPai[m_UserData.m_MenPai.byCaiShenIndex] == 255)//û�еõ�����ͽ����һ������Ϊ����
	{
		m_UserData.m_MenPai.byCaiShenIndex = m_UserData.m_MenPai.byEndIndex;
		m_UserData.m_StrJing.Init();
		m_UserData.m_StrJing.AddJing(m_UserData.m_MenPai.byMenPai[m_UserData.m_MenPai.byCaiShenIndex]);
	}

#ifdef MJ_CAN_SETPAI
 //   Tstring s = CINIFile::GetAppPath ();/////����·��    
	//CINIFile f(s +_T("\\")+SKIN_FOLDER  + _T("_s.ini"));
	//m_UserData.m_StrJing.byPai[0] = f.GetKeyVal("test","baopai",m_UserData.m_StrJing.byPai[0]);
#endif //MJ_CAN_SETPAI

	m_GameData.T_TwoSeziJing.m_StrJing = m_UserData.m_StrJing;
	//������Ϸ��Ϣ
	SendAllGameData( &m_GameData.T_TwoSeziJing, sizeof(m_GameData.T_TwoSeziJing), ASS_GM_GAME_NOTIFY, EVENT_2SEZI_JING, 0);
	//���õȴ�ɫ�Ӷ�������״̬�����ڸ��¼�Ҫ����ɫ�Ӷ��������Ա���Ҫ�ȴ���������
	m_UserData.SetWaiteSezi(true);
	//��ʱ����
	SetDoingThing(EVENT_2SEZI_JING,0);//�������ڷ������¼�
}

///������Ҳ����¼�
void CSrvLogic::ExecuteAllBuHua(BYTE &nextID,BYTE &time)	
{
	nextID = EVENT_BEGIN_OUT;
	time = 5;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(!m_UserData.IsHuaPaiInHand(i))
			continue;
		while(m_UserData.IsHuaPaiInHand(i))
		{
			m_UserData.MoveHuaPaiFormHand(i);
		}
	}
	m_UserData.CopyAllHuaPai(m_GameData.T_AllBuHua.m_byArHuaPai);//��������
	m_UserData.CopyAllMenPai(m_GameData.T_AllBuHua.byMenPai);//��������
	///����������
	for (int i=0;i<PLAY_COUNT;i++)
	{		
		m_UserData.CopyAllHandPai(m_GameData.T_AllBuHua.m_byArHandPai,i,false);
		SendGameData(i, &m_GameData.T_AllBuHua, sizeof(m_GameData.T_AllBuHua), ASS_GM_GAME_NOTIFY, EVENT_ALL_BUHUA, 0);
	}
	//��ʱ����
	SetDoingThing(EVENT_ALL_BUHUA,0);//�������ڷ������¼�
}

///������Ҳ����¼�
void CSrvLogic::ExecuteOneBuHua(BYTE &nextID,BYTE &time)
{
	m_GameData.T_OneBuHua.byUser = m_UserData.m_byNowOutStation;
	while(m_UserData.IsHuaPaiInHand(m_UserData.m_byNowOutStation))
	{
		m_UserData.MoveHuaPaiFormHand(m_UserData.m_byNowOutStation);
	}
	m_UserData.CopyAllHuaPai(m_GameData.T_OneBuHua.m_byArHuaPai);//��������
	m_UserData.CopyAllMenPai(m_GameData.T_OneBuHua.byMenPai);//��������
	///����������
	for (int i=0;i<PLAY_COUNT;i++)
	{			
		m_UserData.CopyAllHandPai(m_GameData.T_OneBuHua.m_byArHandPai,i,false);
		SendGameData(i, &m_GameData.T_OneBuHua, sizeof(m_GameData.T_OneBuHua), ASS_GM_GAME_NOTIFY, EVENT_ONE_BUHUA, 0);
	}
	//��ʱ����
	SetDoingThing(EVENT_ONE_BUHUA,m_GameData.T_OneBuHua.byUser);//�������ڷ������¼�
}

///��ʼ����֪ͨ�¼�
void CSrvLogic::ExecuteBeginOut(BYTE &nextID,BYTE &time)
{
	m_GameData.T_BeginOutPai.byNt = m_UserData.m_byNtStation;//ׯ��λ��
	m_GameData.T_BeginOutPai.byUser = m_UserData.m_byNtStation;//ׯ��λ��
	m_GameData.T_BeginOutPai.m_StrJing = m_UserData.m_StrJing;//����
	m_UserData.m_byNowOutStation = m_UserData.m_byNtStation;//����λ��
	ChangeMenPai();//ת������,������ת��
	m_UserData.CopyAllMenPai(m_GameData.T_BeginOutPai.byMenPai);//��������
	//��⶯��
	CheckAction(m_UserData.m_byNtStation,m_UserData.GetLastHandPai(m_UserData.m_byNowOutStation),0);
	//����֪ͨ
	SendAllGameData(&m_GameData.T_BeginOutPai, sizeof(m_GameData.T_BeginOutPai), ASS_GM_GAME_NOTIFY, EVENT_BEGIN_OUT, 0);
	if(!m_UserData.IsUserState( m_GameData.T_BeginOutPai.byUser,USER_STATE_CUT))//û�ж���
	{
		m_GameData.T_CPGNotify[m_UserData.m_byNtStation].byUser = m_UserData.m_byNtStation;
		m_GameData.T_CPGNotify[m_UserData.m_byNtStation].bZhuaPai = true;
		SendGameData(m_UserData.m_byNtStation, &m_GameData.T_CPGNotify[m_UserData.m_byNtStation], sizeof(tagCPGNotifyEx), ASS_GM_GAME_NOTIFY, EVENT_CPG_NOTIFY, 0);
	}
	//��ʱ����
	SetDoingThing(EVENT_BEGIN_OUT,m_UserData.m_byNtStation);//�������ڷ������¼�
	SetGameState(STATE_OUT_CARD);//������Ϸ״̬
}

///ץ���¼�
void CSrvLogic::ExecuteZhuaPai(BYTE &nextID,BYTE &time)	
{
	BYTE card = 255;
	if(m_UserData.m_MenPai.GetMenPaiNum()<=m_GameData.m_mjRule.byGamePassNum)
	{
		nextID = EVENT_END_HANDLE;
		time = 10;
		return;
	}
	else
	{
		for(int i=0;i<PLAY_COUNT;++i)
		{
			if(i == m_UserData.m_byNowOutStation)
				continue;
			m_UserData.SetUserState(i,USER_STATE_GANG,false);//��������״̬�����ܿ��͸ܺ���ʱʹ��
		}
		m_GameData.T_ZhuaPai.byUser = m_UserData.m_byNowOutStation;//��ǰҪ���Ƶ�λ��
		card = GetPai(m_GameData.T_ZhuaPai.byGetPaiIndex,m_UserData.m_byNowOutStation,m_UserData.m_bZhuaPaiDir,0);//����ץ��
		m_GameData.T_ZhuaPai.byPs = card;		//ץ�Ƶ�ֵ
		m_UserData.SuperZhuaPai(card);//�����ͻ�������ץ�ƴ����������汾��
		if(m_GameData.m_mjRule.bHaveFlower && m_UserData.CheckIsHuaPai(card))//�������л���
		{//���Ʋ���
			nextID = EVENT_ONE_BUHUA;
			time = 5;
			m_GameData.T_ZhuaPai.bHead = false;			//ץ�Ƶķ���
			m_GameData.T_ZhuaPai.bCanOutPai = false;	//ץ�ƺ��Ƿ���Գ���
		}
		else
		{//�������
			CheckAction(m_UserData.m_byNowOutStation,card,0);
		}
		ChangeMenPai();//ת������
		m_UserData.CopyAllHuaPai(m_GameData.T_ZhuaPai.m_byArHuaPai);//��������
		m_UserData.CopyAllOutPai(m_GameData.T_ZhuaPai.m_byArOutPai);//��������
		m_UserData.CopyAllMenPai(m_GameData.T_ZhuaPai.byMenPai);//��������
	}
	BYTE pai = m_GameData.T_ZhuaPai.byPs ;
	///����������
	for (int i=0;i<PLAY_COUNT;i++)
	{			
		m_GameData.T_ZhuaPai.byPs = 255;
		m_UserData.CopyAllHandPai(m_GameData.T_ZhuaPai.m_byArHandPai,i,false);
		if(m_GameData.T_ZhuaPai.byUser == i)//ֻ��ץ����Ҳ��ܿ�����ץ����
		{
			m_GameData.T_ZhuaPai.byPs = pai;
		}
		SendGameData(i, &m_GameData.T_ZhuaPai, sizeof(m_GameData.T_ZhuaPai), ASS_GM_GAME_NOTIFY, EVENT_ZHUA_PAI, 0);
	}
	m_GameData.T_ZhuaPai.byPs = pai;
	if(nextID == 255)
	{//id=255˵����û������Ҳ���ǲ��������Է��ͳ�����֪ͨ��������߾�ֱ�ӳ���
		SetGameState(STATE_OUT_CARD);//������Ϸ״̬
		m_GameData.T_CPGNotify[m_GameData.T_ZhuaPai.byUser].bZhuaPai = true;
		m_GameData.T_CPGNotify[m_GameData.T_ZhuaPai.byUser].byUser = m_GameData.T_ZhuaPai.byUser;
		SendGameData(m_GameData.T_ZhuaPai.byUser, &m_GameData.T_CPGNotify[m_GameData.T_ZhuaPai.byUser], sizeof(tagCPGNotifyEx), ASS_GM_GAME_NOTIFY, EVENT_CPG_NOTIFY, 0);
	}
	//��ʱ����
	SetDoingThing(EVENT_ZHUA_PAI,m_UserData.m_byNowOutStation);//�������ڷ������¼�
}

//�����¼�
void CSrvLogic::ExecuteChiPai(BYTE &nextID,BYTE &time)
{
	m_GameData.T_ChiPai = m_GameData.temp_ChiPai;//��ȡ����
	m_UserData.DelleteLastOutPai(m_GameData.temp_ChiPai.byBeChi);//ɾ��������ҵ������ƣ���Ϊ������
	m_UserData.m_byNowOutStation = m_GameData.temp_ChiPai.byUser;//�ı����λ��
	//��ӳ���
	GCPStructEx chi;
	CLogicBase::CopyChiData(chi,m_GameData.temp_ChiPai);//������������
	for(int i=0;i<3;i++)
	{	
		if(m_GameData.temp_ChiPai.byChiPs[i] == m_GameData.temp_ChiPai.byPs)
			continue;
		m_UserData.DelleteAHandPai(chi.iStation,m_GameData.temp_ChiPai.byChiPs[i]);//ɾ������
	}
	////�����Ե��Ʒŵ��м�
	memset(chi.byData,255,sizeof(chi.byData));
	chi.byData[1] = m_UserData.m_byOtherOutPai;
	for(int i=0;i<3;++i)
	{
		if(m_GameData.temp_ChiPai.byChiPs[i] == m_UserData.m_byOtherOutPai)
			continue;
		for(int j=0;j<3;++j)
		{
			if(chi.byData[j] == 255)
			{
				chi.byData[j] = m_GameData.temp_ChiPai.byChiPs[i];
				break;
			}
		}
	}
	m_UserData.AddToGCP(chi.iStation,&chi);
	m_UserData.CopyAllGCPData(m_GameData.T_ChiPai.m_UserGCPData);
	m_UserData.CopyAllOutPai(m_GameData.T_ChiPai.m_byArOutPai);
	//��ϵ��¼
	m_UserData.m_byGuanXi[m_GameData.temp_ChiPai.byUser][m_GameData.temp_ChiPai.byBeChi] += 1;
	//֪ͨ�������
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		m_UserData.CopyAllHandPai(m_GameData.T_ChiPai.m_byArHandPai,i,false);
		SendGameData(i, &m_GameData.T_ChiPai, sizeof(m_GameData.T_ChiPai), ASS_GM_GAME_NOTIFY, EVENT_CHI_PAI, 0);
	}
	//�ж��Ƿ�������
	m_GameData.T_CPGNotify[chi.iStation].Clear();//��������
	m_CheckHuPai.CanTing(chi.iStation,m_GameData.T_CPGNotify[chi.iStation]);//����ܷ�����
	if(m_GameData.T_CPGNotify[m_GameData.T_ChiPai.byUser].bTing)//������
	{//������������
		m_GameData.T_CPGNotify[chi.iStation].byUser = chi.iStation;
		SendGameData(chi.iStation, &m_GameData.T_CPGNotify[chi.iStation], sizeof(tagCPGNotifyEx), ASS_GM_GAME_NOTIFY, EVENT_CPG_NOTIFY, 0);
	}	
	SetGameState(STATE_OUT_CARD);//������Ϸ״̬
	//��ʱ����
	SetDoingThing(EVENT_CHI_PAI,m_UserData.m_byNowOutStation);//�������ڷ������¼�
}

//�����¼�
void CSrvLogic::ExecutePengPai(BYTE &nextID,BYTE &time)
{
	m_GameData.T_PengPai = m_GameData.temp_PengPai;//��ȡ����
	//�������ɾ�����һ�ų���
	m_UserData.DelleteLastOutPai(m_GameData.temp_PengPai.byBePeng);
	//�ı����λ��
	m_UserData.m_byNowOutStation = m_GameData.temp_PengPai.byUser;
	//�������¼�
	GCPStructEx peng;
	CLogicBase::CopyPengData(peng,m_GameData.temp_PengPai);//������������
	//ɾ������
	m_UserData.DelleteSomeHandPai(peng.iStation,peng.byData,2);//ɾ��һЩ����
	m_UserData.AddToGCP(m_GameData.temp_PengPai.byUser,&peng);//�������
	m_UserData.CopyAllGCPData(m_GameData.T_PengPai.m_UserGCPData);//��������������
	m_UserData.CopyAllOutPai(m_GameData.T_PengPai.m_byArOutPai);//������������
	//��ϵ��¼
	m_UserData.m_byGuanXi[m_GameData.temp_PengPai.byUser][m_GameData.temp_PengPai.byBePeng] += 1;
	for (int i=0;i<PLAY_COUNT;i++)//֪ͨ�������
	{	
		m_UserData.CopyAllHandPai(m_GameData.T_PengPai.m_byArHandPai,i,false);//��������
		SendGameData(i, &m_GameData.T_PengPai, sizeof(m_GameData.T_PengPai), ASS_GM_GAME_NOTIFY, EVENT_PENG_PAI, 0);
	}
	//�ж��Ƿ�������
	m_GameData.T_CPGNotify[peng.iStation].Clear();//��������
	m_CheckHuPai.CanTing(peng.iStation,m_GameData.T_CPGNotify[peng.iStation]);//����ܷ�����
	if(m_GameData.T_CPGNotify[peng.iStation].bTing)//������
	{//������������
		m_GameData.T_CPGNotify[peng.iStation].byUser = peng.iStation;
		SendGameData(peng.iStation, &m_GameData.T_CPGNotify[peng.iStation], sizeof(tagCPGNotifyEx), ASS_GM_GAME_NOTIFY, EVENT_CPG_NOTIFY, 0);
	}
	SetGameState(STATE_OUT_CARD);//������Ϸ״̬
	//��ʱ����
	SetDoingThing(EVENT_PENG_PAI,m_UserData.m_byNowOutStation);//�������ڷ������¼�
}

//�����¼�
void CSrvLogic::ExecuteGangPai(BYTE &nextID,BYTE &time)
{
	nextID = EVENT_ZHUA_PAI;
	time = 3;
	///�������¼�
	m_GameData.T_GangPai = m_GameData.temp_GangPai;//��ȡ����
	GCPStructEx gang;
	CLogicBase::CopyGangData(gang,m_GameData.temp_GangPai);//������������

	switch(m_GameData.temp_GangPai.byType)//��������
	{
	case ACTION_AN_GANG://����
		{
			m_UserData.DelleteSomeHandPai(m_GameData.temp_GangPai.byUser,gang.byData,4);//ɾ������
		}
		break;
	case ACTION_MING_GANG://����
		{
			m_UserData.DelleteSomeHandPai(m_GameData.temp_GangPai.byUser,gang.byData,3);//ɾ������
			m_UserData.DelleteLastOutPai(m_GameData.temp_GangPai.byBeGang);//�������ɾ�����һ�ų���
			//��ϵ��¼
			m_UserData.m_byGuanXi[m_GameData.temp_GangPai.byUser][m_GameData.temp_GangPai.byBeGang] += 1;
		}
		break;
	case ACTION_BU_GANG://����
		{
			m_UserData.DelleteAHandPai(m_GameData.temp_GangPai.byUser,m_GameData.temp_GangPai.byPs);//ɾ������
			m_UserData.DelleteAGCPData(m_GameData.temp_GangPai.byUser,ACTION_PENG,m_GameData.temp_GangPai.byPs);//ɾ����Ӧ����
		}
		break;
	}			
	gang.byType = m_GameData.temp_GangPai.byType;
	m_UserData.m_byNowOutStation = m_GameData.temp_GangPai.byUser;//�ı����λ�ã�һ��Ҫ��ɾ�������ƶ���֮��
	m_UserData.SetUserState(m_UserData.m_byNowOutStation,USER_STATE_GANG,true);//��Ϊ����״̬�����ܿ��͸ܺ���ʹ��


	m_UserData.AddToGCP(m_GameData.temp_GangPai.byUser,&gang);
	m_UserData.CopyAllGCPData(m_GameData.T_GangPai.m_UserGCPData);
	m_UserData.CopyAllOutPai(m_GameData.T_GangPai.m_byArOutPai);
	m_UserData.m_bZhuaPaiDir = false;///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	//֪ͨ�������
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		m_UserData.CopyAllHandPai(m_GameData.T_GangPai.m_byArHandPai,i,false);
		SendGameData(i, &m_GameData.T_GangPai, sizeof(m_GameData.T_GangPai), ASS_GM_GAME_NOTIFY, EVENT_GANG_PAI, 0);
	}
	//��ʱ����
	SetDoingThing(EVENT_GANG_PAI,m_UserData.m_byNowOutStation);//�������ڷ������¼�
}
//�����¼�
void CSrvLogic::ExecuteHuPai(BYTE &nextID,BYTE &time)
{
	nextID = EVENT_COUNT_FEN;
	time = 2;
	///֪ͨ��Һ�����
	if(!m_GameData.T_HuPai.bZimo)
	{
		m_UserData.AddToHandPai(m_GameData.T_HuPai.byUser,m_GameData.T_HuPai.byPs);//
	}
	m_UserData.CopyAllHandPai(m_GameData.T_HuPai.m_byArHandPai,0,true);//���ƿ������ƣ��������������
	m_UserData.CopyAllOutPai(m_GameData.T_HuPai.m_byArOutPai);//��������
	m_UserData.CopyAllGCPData(m_GameData.T_HuPai.m_UserGCPData);//������������
	SendAllGameData(&m_GameData.T_HuPai, sizeof(m_GameData.T_HuPai), ASS_GM_GAME_NOTIFY, EVENT_HU_PAI, 0);
	//��ʱ����
	SetDoingThing(EVENT_HU_PAI,m_GameData.T_HuPai.byUser);//�������ڷ������¼�
}

///����¼�
void CSrvLogic::ExecuteCountFen(BYTE &nextID,BYTE &time)
{
	///����¼�
	nextID = EVENT_END_HANDLE;//��һ���¼�����Ϸ�����¼�
	time = 2;	
	memcpy(m_GameData.T_CountFen.bHu,m_GameData.T_HuPai.bCanH,sizeof(m_GameData.T_HuPai.bCanH));//�����������
	memcpy(m_GameData.T_CountFen.byHuType,m_GameData.T_HuPai.byHuType,sizeof(m_GameData.T_HuPai.byHuType));//������������

	m_GameData.T_CountFen.bDuoXiang = m_GameData.T_HuPai.bDuoXiang;
	m_GameData.T_CountFen.bZimo = m_GameData.T_HuPai.bZimo;
	m_GameData.T_CountFen.byUser = m_GameData.T_HuPai.byUser;
	m_GameData.T_CountFen.byDian = m_GameData.T_HuPai.byDianPao;
	m_GameData.T_CountFen.byPs = m_GameData.T_HuPai.byPs;
	m_GameData.T_CountFen.byNt = m_UserData.m_byNtStation;
	///���
	m_CountFen.CountFen(m_GameData.T_HuPai,m_GameData.T_CountFen);
	m_UserData.CopyAllHandPai(m_GameData.T_CountFen.m_byArHandPai,0,true);
	m_UserData.CopyAllGCPData(m_GameData.T_CountFen.m_UserGCPData);
	//��ʱ���ã�
	SetDoingThing(EVENT_COUNT_FEN,m_GameData.T_CountFen.byUser);//�������ڷ������¼�
}

///������Ϸ�����¼�
void CSrvLogic::ExecuteNormalEnd(BYTE &nextID,BYTE &time)
{
	///��Ϸ���������¼�
	m_GameData.T_CountFen.byLianZhuang = m_UserData.m_byLianZhuang;
	//ȷ��ׯ��
	CLogicBase::GetNextNt(m_GameData.T_CountFen,m_UserData.m_byNtStation,m_UserData.m_byLianZhuang,m_GameData.m_mjRule.byNextNtType);
	//��ȡׯ��
	m_GameData.T_CountFen.byNextNt = m_UserData.m_byNtStation;
	//����ƽ̨������Ϸ�ӿ�
	pDesk->GameFinish(m_GameData.T_HuPai.byUser,GF_NORMAL);
	//��ʱ���ã�
	SetDoingThing(EVENT_END_HANDLE,m_GameData.T_EndHandle.byUser);//�������ڷ������¼�


	char strdug[500];
	for(int i=0;i<PLAY_COUNT;++i)
	{
		sprintf(strdug,"��ֲ��ԣ��¼� ExecuteNormalEnd λ��%d �ܷ�%d ����%d �ܷ�%d",i,m_GameData.T_CountFen.iGangFen[i],m_GameData.T_CountFen.iHuFen[i],m_GameData.T_CountFen.iZongFen[i]);
		OutputDebugString(strdug);
	}

	//���ͽ�������
	SendAllGameData( &m_GameData.T_CountFen, sizeof(m_GameData.T_CountFen), ASS_GM_GAME_NOTIFY, EVENT_END_HANDLE, 0);   
	KillAllTimer();//ɾ�����м�ʱ��
	SetGameState(STATE_FINISH);//������Ϸ״̬
}

///�쳣��Ϸ�����¼��¼�
void CSrvLogic::ExecuteUnNormalEnd(BYTE &nextID,BYTE &time)
{
	KillAllTimer();//ɾ�����м�ʱ��
	//����ƽ̨������Ϸ�ӿ�
	pDesk->GameFinish(255,GF_SAFE);
}

///��ȡ������
void CSrvLogic::DisPatchCard()
{
	m_GameData.LoadPaiData(m_UserData.m_MenPai);//����������
	m_UserData.RandCard();//������
}
///������ת����������,�������������е�����ת����ÿ����ҵ�����������
void CSrvLogic::ChangeMenPai()
{
	m_UserData.ChangeMenPai();
}
///���ƣ�GetPai ������ץһ����
///@param index ͨ����������ץ�Ƶ�������station ץ����ҵ�λ�� , head  �Ƿ��ǰ��ץ,type ץ������ 0 ����ץ�ƣ�1��ʼǰ���ƣ�2��ǰ����ץ�ƣ�3����
///@return ץ������ֵ
BYTE CSrvLogic::GetPai(BYTE &index,int station,bool head,BYTE type)
{
	index = 255;
	if(m_UserData.GetHandPaiCount(station) %3 == 2)//��ҵ������㹻
	{
		OutputDebugString("�¼���GetPai ��������㹻�����ܷ���");
		return 255;
	}
	return m_UserData.GetPai(index,station, head, type);
}

///���ƣ�CheckAction �����ܶ������
///@param ��station ���λ��,pai �����Ҵ�����ƣ�type ���� ���� 0 ץ�ƣ�1�ܿ���2 ���ڣ�3����
///@return true ����������false ����������
bool CSrvLogic::CheckAction(int station,BYTE pai,BYTE type)
{
	for(int i=0;i<4;++i)
	{
		m_GameData.T_CPGNotify[i].Clear();//����
		m_GameData.T_CPGNotify[i].byOutStation = station;//��¼����λ��
	}
	bool action = false;
	switch(type)
	{
	case 0://����:�Լ�ץ�ƶ������
	case 1://�ܿ�:�������
		{
			m_CheckHuPai.CanHu(station,pai,m_GameData.T_CPGNotify[station],true,station);//�����
			m_CPGAction.CanGang(station,pai,m_GameData.T_CPGNotify[station],true);//�ܼ��
			m_CheckHuPai.CanTing(station,m_GameData.T_CPGNotify[station]);//�����
			if(m_GameData.T_CPGNotify[station].bHu || m_GameData.T_CPGNotify[station].bGang || m_GameData.T_CPGNotify[station].bTing)
			{
				m_GameData.T_CPGNotify[station].bZimo = true;
				action=true;
				m_GameData.T_CPGNotify[station].bCanAction = true;
				m_GameData.T_CPGNotify[station].byPs = pai;
			}
		}
		break;
	case 2://����:���˳��ƺ������
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(i==station || m_UserData.IsUserState(i,USER_STATE_CUT))//������Ҳ����
					continue;
				m_CPGAction.CanChi(i,pai,m_GameData.T_CPGNotify[i]);//�Լ��
				m_CPGAction.CanPeng(i,pai,m_GameData.T_CPGNotify[i]);//�����
				m_CPGAction.CanGang(i,pai,m_GameData.T_CPGNotify[i],false);//�ܼ��
				m_CheckHuPai.CanHu(i,pai,m_GameData.T_CPGNotify[i],false,station);//�����
				if(m_GameData.T_CPGNotify[i].bHu || m_GameData.T_CPGNotify[i].bPeng || m_GameData.T_CPGNotify[i].bChi || m_GameData.T_CPGNotify[i].bGang )
				{
					action=true;
					m_GameData.T_CPGNotify[i].bZimo = false;
					m_GameData.T_CPGNotify[i].byPs = pai;
					m_GameData.T_CPGNotify[i].bCanAction = true;
				}
			}
		}
		break;
	case 3://���ܣ����˲���ʱ�������
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(i==station || m_UserData.IsUserState(i,USER_STATE_CUT))//������Ҳ����
					continue;
				if(m_CheckHuPai.CanHu(i,pai,m_GameData.T_CPGNotify[i],false,station))
				{
					action = true;//��
					m_GameData.T_CPGNotify[i].byPs = pai;
					m_GameData.T_CPGNotify[i].bZimo = false;
		
				}
			}
		}
		break;
	}
	return action;
}

//�������Զ�����2������û���յ��κ���Ϸ��Ϣ����Ϊ����Ϸ���������ñ����������ݵ�ǰִ���¼�����ҵ�״̬�����Զ������ָ���Ϸ����
void CSrvLogic::AutoHandle(bool bNetCut)
{
	switch(m_GameData.m_byThingDoing)
	{			        				         
	case EVENT_2SEZI_NT://�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
	case EVENT_2SEZI_NT_GP://�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�		
	case EVENT_2SEZI_DIR://��2��ɫ���¼������Ʒ����¼�		
	case EVENT_2SEZI_DIR_GP://��2��ɫ���¼������Ʒ�������Ƶ��¼�
	case EVENT_2SEZI_GP://��2��ɫ���¼������Ƶ��¼�												
	case EVENT_2SEZI_JING://��2��ɫ�Ӷ������¼�			
		{//����ɫ�ӽ�����Ϣ
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(bNetCut)//���ߴ���������������Ӧ
				{
					if(!m_UserData.m_bSiziFinish[i] && m_UserData.IsUserState(i,USER_STATE_CUT))//�����û�з�������Ϣ
					{
						ReceiveSeziFinish(i,NULL,0,false);
					}
				}
				else
				{
					if(!m_UserData.m_bSiziFinish[i])//�����û�з�������Ϣ
					{
						ReceiveSeziFinish(i,NULL,0,false);
					}
				}
			}
		}
		break;
	case EVENT_ALL_BUHUA://Ϊ������Ҳ����¼�
		{//ִ�п�ʼ�����¼�
			ApplyAction(EVENT_BEGIN_OUT,200);//������һ���¼�
		}
		break;
	case EVENT_ONE_BUHUA://������Ҳ����¼�	
	case EVENT_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�	
	case EVENT_CHI_PAI://�����¼�
	case EVENT_PENG_PAI://�����¼�
	case EVENT_ZHUA_PAI://ץ���¼�
	case EVENT_GANG_PAI://�����¼�
		{
			if(bNetCut)//���ߴ���������������Ӧ
			{
				if(m_UserData.IsUserState( m_UserData.m_byNowOutStation,USER_STATE_CUT))//���ׯ�Ҷ���
				{
					AutoOutOrGet();//������ҳ���
				}
			}
			else
			{
				AutoOutOrGet();//������ҳ���
			}
		}
		break;
	case EVENT_OUT_PAI://�����¼�
		{
			if(bNetCut)//���ߴ���������������Ӧ
			{
				for(int i=0;i<PLAY_COUNT;++i)
				{
					if(m_UserData.IsUserState( i,USER_STATE_CUT) )//�����Ҷ���
					{
						ReceiveUserPassCheck(i,NULL,0,false);//���ͷ�����Ϣ
					}
				}
			}
			else
			{
				if(m_GameData.m_byNextAction != 255 && m_GameData.m_byNextAction != ACTION_NO)//���������˶���
				{
					for(int i=0;i<PLAY_COUNT;++i)
					{
						if(m_GameData.T_CPGNotify[i].bCanAction && !m_GameData.T_CPGNotify[i].bHaveReturn)//�ж�������û�л�Ӧ
							ReceiveUserPassCheck(i,NULL,0,false);//���ͷ�����Ϣ
					}
				}
				else
				{
					AutoOutOrGet();//������ҳ���
				}
			}
		}
		break;
	case EVENT_HU_PAI://���¼�	
		{
			if(!bNetCut)//���ߴ���������������Ӧ
			{
				ApplyAction(EVENT_COUNT_FEN,200);//������һ���¼�
			}
		}
		break;
	case EVENT_COUNT_FEN://����¼�	
		{
			if(!bNetCut)//���ߴ���������������Ӧ
			{
				ApplyAction(EVENT_END_HANDLE,200);//������һ���¼�
			}
		}
		break;
	}
}
///�Զ�������ƻ���ץ��
void CSrvLogic::AutoOutOrGet()
{
	bool haveout = false;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(!m_UserData.IsOutPaiPeople(i))//�������
			continue;
		haveout = true;
		tagOutPaiEx outpai;
		outpai.Clear();
		outpai.byUser = i;
		outpai.byPs = m_UserData.GetLastHandPai(i);
		ReceiveUserOutCard(i,&outpai,sizeof(outpai),false);
		break;
	}
	if(!haveout)//û�г��Ƶ���ң�����ץ���¼�
	{
		ApplyAction(EVENT_ZHUA_PAI,200);//������һ���¼�
	}
}
/////////////////////////��������////////////////////////////////////////////////////////////
/// ��Ҷ���
void CSrvLogic::UserNetCut(BYTE bDeskStation)
{
	m_UserData.SetUserState(bDeskStation,USER_STATE_CUT,true);//���ö���״̬
	AutoHandle(true);	///���ߴ���
}
/// �������
void CSrvLogic::UserLeftDesk(BYTE bDeskStation)
{

	if (pDesk->GetGameStation() > GS_PLAY_GAME || pDesk->GetGameStation() < GS_SEND_CARD)  //����Ϸ�У�
	{
		m_UserData.m_byNtStation = 255;
	}
}

/// �������
void CSrvLogic::UserSitDesk(BYTE bDeskStation)
{
}

/// �������
void CSrvLogic::UserReCome(BYTE bDeskStation)
{
	m_UserData.SetUserState(bDeskStation,USER_STATE_CUT,false);//����Ϊ�Ƕ���״̬
}
/// �����ʱ����Ϣ
void CSrvLogic::OnGameTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case 10://�¼���ʱ��id
		{
			pDesk->KillTimer(uTimerID);
			GameCPU(m_GameData.ApplyThingID);
		}
		break;
	case TIME_CHECK_GAME_MSG://��Ϣ��ʱ���Զ�����
		{
			pDesk->KillTimer(uTimerID);
			AutoHandle();
		}
		break;
	case TIME_WAIT_MSG: // �����¼�����Ӧ���ǳ�ʱ�Զ�����ϵͳ�Զ�����
		{
			OutTimeHandle();
		}
		break;
	}
}
/// ��Ϸ����Ҫɾ�����м�ʱ��
void CSrvLogic::KillAllTimer()
{
	pDesk->KillTimer(10);
	pDesk->KillTimer(TIME_CHECK_GAME_MSG);
	pDesk->KillTimer(TIME_WAIT_MSG);
}

/// ���õ�ǰ���ڷ������¼���ͬʱ����һ���ȴ�ʱ�䣬����ʱ��ͻ���û����Ӧ���Զ�����
void CSrvLogic::SetDoingThing(BYTE thingId,BYTE st1,BYTE st2,BYTE pai,BYTE data)
{
	RecordThing( thingId, st1, st2, pai, data);//��¼�¼�
	m_GameData.m_byThingDoing = thingId;//���ڷ������¼�
	m_GameData.ReSetOutTime();/// �ָ���ʱ����
	m_GameData.m_byNextAction = ACTION_NO;//��һ����������Ϊ�޶���
	m_UserData.m_byApplyMaxAction = ACTION_NO;//��ҵ�ǰ���������¼�������������󣬻ָ�����
	switch(thingId)
	{	     	
	case EVENT_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�		
		{
			m_GameData.SetOutTime(st1,m_GameData.m_MjSet.byOutcardTime,thingId,true);
		}
		break;
	case EVENT_OUT_PAI://�����¼�
		{
			AutoHandle(true);
			m_GameData.SetOutTime(st1,m_GameData.m_MjSet.byBlockTime,thingId,true);
		}
		break;
	case EVENT_ZHUA_PAI://ץ���¼�
		{
			if(m_UserData.IsUserState(st1,USER_STATE_AUTO) || m_UserData.IsUserState(st1,USER_STATE_TING) || m_UserData.IsUserState(st1,USER_STATE_CUT))//���ƻ����йܣ��Ƕ�ʱ���Ӧ
			{
				m_GameData.SetOutTime(st1,m_GameData.m_MjSet.byAutoTime,thingId,true);
				break;
			}
			m_GameData.SetOutTime(st1,m_GameData.m_MjSet.byOutcardTime + m_GameData.m_MjSet.byBlockTime,thingId,true);
		}
		break;
	case EVENT_CHI_PAI://�����¼�
	case EVENT_PENG_PAI://�����¼�	
	case EVENT_GANG_PAI://�����¼�
		{
			if(m_UserData.IsUserState(st1,USER_STATE_AUTO) || m_UserData.IsUserState(st1,USER_STATE_TING)|| m_UserData.IsUserState(st1,USER_STATE_CUT))//���ƻ����йܣ��Ƕ�ʱ���Ӧ
			{
				m_GameData.SetOutTime(st1,m_GameData.m_MjSet.byAutoTime,thingId,true);
				break;
			}
			m_GameData.SetOutTime(st1,m_GameData.m_MjSet.byOutcardTime,thingId,true);
		}
		break;
	case EVENT_2SEZI_NT:	//�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
	case EVENT_2SEZI_NT_GP:	//�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
	case EVENT_2SEZI_DIR://��2��ɫ���¼������Ʒ����¼�		
	case EVENT_2SEZI_DIR_GP://��2��ɫ���¼������Ʒ�������Ƶ��¼�
	case EVENT_2SEZI_GP://��2��ɫ���¼������Ƶ��¼�		
	case EVENT_2SEZI_NT_DIR_GP:	//��2��ɫ���¼������Ƶ��¼�	
	case EVENT_2SEZI_JING://��2��ɫ�Ӷ������¼�			
		{
			m_GameData.SetOutTime(st1,m_GameData.m_MjSet.byWaiteSeziTime,thingId,true);
		}
		break;
	default://�����¼���Ϊ˳���¼�����,5���Ӵ���
		{
			m_GameData.SetOutTime(st1,5,thingId,true);
		}
		break;
	}
}
/// ��ʱ����
void CSrvLogic::OutTimeHandle()
{


	if(!m_GameData.m_MjWait.bEffect || m_GameData.m_MjWait.byCurChair == 255 || m_GameData.m_MjWait.byTime == 255 
		|| m_GameData.m_MjWait.byWaitThing != m_GameData.m_byThingDoing )
	{
		return;
	}
	m_GameData.m_MjWait.byTime -= 1;
	if(m_GameData.m_MjWait.byTime == 0)
	{
		this->AutoHandle();
	}
}
/// ������Ϸ״̬
void CSrvLogic::SetGameState(BYTE state)
{
	if(state>= STATE_BEGIN_OUT && state <= STATE_FINISH)
	{
		m_GameData.m_byGameState = state;
	}
}
//�¼���¼
void CSrvLogic::RecordThing(BYTE thingId,BYTE st1,BYTE st2,BYTE pai,BYTE data)
{//�¼���¼ֻ��¼��10���¼���0�¼��ţ�1���λ��1,2���λ��2��3�����ֵ��4��������
	BYTE index = 9;
	if(m_GameData.m_byThingLog[9][0] != 255)//����
	{
		for(int i=0;i<9;++i)//����һ��
		{
			memcpy(m_GameData.m_byThingLog[i],m_GameData.m_byThingLog[i+1],sizeof(m_GameData.m_byThingLog[i]));
		}
	}
	else
	{
		for(int i=0;i<10;++i)
		{
			if(m_GameData.m_byThingLog[i][0] != 255)
				continue;
			index = i;
			break;
		}
	}
	m_GameData.m_byThingLog[index][0] = thingId;
	m_GameData.m_byThingLog[index][1] = pai;
	m_GameData.m_byThingLog[index][2] = st1;
	m_GameData.m_byThingLog[index][3] = st2;
	m_GameData.m_byThingLog[index][4] = data;
}
//������Ϸ״̬
void CSrvLogic::SendGameStation(BYTE bDeskStation, bool bWatchUser, void * pData, UINT uSize)
{
	pDesk->SendGameStationEx(bDeskStation,bWatchUser,pData, uSize);
}

//��������ҷ�����Ϸ��Ϣ
void CSrvLogic::SendGameData(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	pDesk->SendGameDataEx(bDeskStation,bMainID,bAssID,bHandleCode);
	pDesk->SendWatchDataEx(bDeskStation,bMainID,bAssID,bHandleCode);
}
//��������ҷ�����Ϸ��Ϣ
void CSrvLogic::SendGameData(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	pDesk->SendGameDataEx(bDeskStation,pData,uSize,bMainID,bAssID,bHandleCode);
	pDesk->SendWatchDataEx(bDeskStation,pData,uSize,bMainID,bAssID,bHandleCode);
}
//��������ҷ�����Ϸ��Ϣ
void CSrvLogic::SendAllGameData(BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	for(int i=0;i<PLAY_COUNT;++i)
	{
		pDesk->SendGameDataEx(i,bMainID,bAssID,bHandleCode);
		pDesk->SendWatchDataEx(i,bMainID,bAssID,bHandleCode);
	}
}
//��������ҷ�����Ϸ��Ϣ
void CSrvLogic::SendAllGameData( void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	for(int i=0;i<PLAY_COUNT;++i)
	{
		pDesk->SendGameDataEx(i,pData,uSize,bMainID,bAssID,bHandleCode);
		pDesk->SendWatchDataEx(i,pData,uSize,bMainID,bAssID,bHandleCode);
	}
}