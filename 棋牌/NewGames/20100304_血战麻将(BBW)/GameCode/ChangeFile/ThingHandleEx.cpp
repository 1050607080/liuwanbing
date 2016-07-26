#include "stdafx.h"
#include "ThingHandleEx.h"
#include "../server/ServerManage.h"
#include "../server/DebugPrintf.h"

//ȫ�ֶ���
//ThingHandleEx g_ThingHandle;
	
ThingHandleEx::ThingHandleEx(void)
{
}
ThingHandleEx::~ThingHandleEx(void)
{
}
//���������¼�
bool ThingHandleEx::GetGameStaton(BYTE bDeskStation, UINT uSocketID, bool bWatchUser,BYTE GameStation)
{
	GameStationDataEx GameStationData;
	::memset(&GameStationData, 0, sizeof(GameStationData));

	char key[200];
    CString s = CINIFile::GetAppPath ();/////����·��    
	CINIFile f(s +_T("\\")+SKIN_FOLDER  + _T("_s.ini"));
	sprintf(key,"game");
	pDesk->sGameData.m_byFengDing = f.GetKeyVal(key,"fengding",pDesk->sGameData.m_byFengDing);
	pDesk->sGameData.m_iBaseFen =  f.GetKeyVal(key,"basefen",1);//��Ϸ�����֣�Ĭ��Ϊ1
	pDesk->sGameData.m_bForceFinish =  f.GetKeyVal(key,"bforce",0);//�Ƿ�ǿ�˽����汾
	pDesk->sGameData.m_bChengBao =  f.GetKeyVal(key,"bchengbao",pDesk->sGameData.m_bChengBao);//�ֳа��ƺͼҼ���

	sprintf(key,"%d_%d",NAME_ID,pDesk->m_RoomId);
	pDesk->sGameData.m_byFengDing = f.GetKeyVal(key,"fengding",pDesk->sGameData.m_byFengDing);
	pDesk->sGameData.m_iBaseFen =  f.GetKeyVal(key,"basefen",pDesk->sGameData.m_iBaseFen);//��Ϸ�����֣�Ĭ��Ϊ1
	pDesk->sGameData.m_bForceFinish =  f.GetKeyVal(key,"bforce",pDesk->sGameData.m_bForceFinish);//�Ƿ�ǿ�˽����汾
	pDesk->sGameData.m_bChengBao =  f.GetKeyVal(key,"bchengbao",pDesk->sGameData.m_bChengBao);//�ֳа��ƺͼҼ���

    GameStationData.ucDingQueTime	= pDesk->sGameData.GetMjSetting().ucDingQueTime;
    GameStationData.ucDingQueTime   = pDesk->sGameData.GetMjSetting().ucDingQueTime;

    //GameStationData.byThinkTime     = pDesk->sGameData.GetMjSetting().ucBlockTime;//˼��ʱ��
    //GameStationData.byOutTime		= pDesk->sGameData.GetMjSetting().ucOutcardTime;//����ʱ��
    //GameStationData.byBeginTime     = pDesk->sGameData.GetMjSetting().ucBeginTime;//��ʼ�ȴ�ʱ��

    //GameStationData.ucNextTime      = pDesk->sGameData.GetMjSetting().ucNextTime;
    //GameStationData.bAutoBegin      = pDesk->sGameData.GetMjSetting().bAutoBegin;//ʱ�䵽���Ƿ��Զ���ʼ
    GameStationData.byAutoOutTime		= pDesk->sGameData.GetMjSetting().nAutoTime;//�Զ�����ʱ��

	GameStationData.byAutoOutTime = pDesk->sGameData.m_mjRule.byAutoOutTime;		//�Զ�����ʱ��
	GameStationData.byOutTime = pDesk->sGameData.m_mjRule.byOutTime;				//����ʱ��
	GameStationData.byThinkTime = pDesk->sGameData.m_mjRule.byThinkTime;			//˼��ʱ��
	GameStationData.byBeginTime = pDesk->sGameData.m_mjRule.byBeginTime;			//��ʼ�ȴ�ʱ��
	GameStationData.bAutoBegin =  pDesk->sGameData.m_mjRule.bAutoBegin;			//ʱ�䵽���Ƿ��Զ���ʼ


	GameStationData.m_byFengDing = pDesk->sGameData.m_byFengDing;//�ⶥ����
	GameStationData.m_bShowTax = pDesk->sGameData.m_bShowTax;//�Ƿ���ʾ�����
	GameStationData.m_iBaseFen = pDesk->sGameData.m_iBaseFen;//�ⶥ����
	GameStationData.m_bForceFinish = pDesk->sGameData.m_bForceFinish;//�Ƿ�ǿ�˽����汾
	GameStationData.m_bChengBao = pDesk->sGameData.m_bChengBao;//�ֳа��ƺͼҼ���

	sprintf(key,"�����ֲ��ԣ� ��ǰ������%d",pDesk->sGameData.m_iBaseFen);
	OutputDebugString(key);


	GameStationData.iVersion	=  MAIN_VERSION;
	GameStationData.iVersion2	= ASS_VERSION;
	
	//��ȡ���䱶�����ܷ���
	GameStationData.bShowUserId	= pDesk->sGameData.m_bShowUserId;//�Ƿ���ʾ������ҵ�id
	GameStationData.Stationpara = pDesk->m_bGameStation;		 //��Ϸ״̬
	pDesk->GetUserAgree(GameStationData.ucAgree);
	if(!bWatchUser)
	{
		pDesk->sUserData.m_bFoceLeave[bDeskStation] = false;
		//��������ȡ���й�
		pDesk->sUserData.m_bTuoGuan[bDeskStation] = false;
		//pDesk->sUserData.Isu
	}

	for(int i=0;i<4;++i)
	{
		memset(GameStationData.m_byArHandPai,255,sizeof(GameStationData.m_byArHandPai));
		pDesk->sUserData.CopyGCPData(i,GameStationData.m_UserGCPData[i]);
		pDesk->sUserData.CopyOutPai(i,GameStationData.m_byArOutPai[i]);
		pDesk->sUserData.CopyMenPai(i,GameStationData.m_iArMenPai[i]);
		pDesk->sUserData.CopyHuaPai(i,GameStationData.m_byArHuaPai[i]);
		GameStationData.m_bTing[i] = pDesk->sUserData.m_bTing[i];
		GameStationData.m_bistuoguan[i] = pDesk->sUserData.m_bTuoGuan[i];
		//��ȡȱ��״̬
		GameStationData.m_byDingQue[i] = pDesk->sUserData.m_byDingQue[i];
	}
	pDesk->sUserData.CopyHandPai(GameStationData.m_byArHandPai,bDeskStation,false);
	GameStationData.m_byOtherOutPai = pDesk->sUserData.m_byOtherOutPai;
	GameStationData.m_byThingDoing = pDesk->sGameData.m_byThingDoing;
	GameStationData.byQuanFeng = pDesk->sUserData.m_byQuanFeng;
	GameStationData.byNtStation = pDesk->sUserData.m_byNtStation;
	GameStationData.m_byNowOutStation = pDesk->sUserData.m_byNowOutStation;

	OutputDebugString(TEXT("�齫-������������ OnGetGameStation  "));
	switch (pDesk->m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
		{
			//��������
			pDesk->SendGameStationEx(bDeskStation, bWatchUser, &GameStationData, sizeof(GameStationData));
			MessageStruct Mess;
			ZeroMemory(Mess.Message, 200);
			lstrcpy(Mess.Message, TEXT("�gӭ�����錢�Α�,ף������_�ģ�"));
			pDesk->SendGameDataEx(bDeskStation, &Mess, sizeof(Mess), MDM_GM_GAME_NOTIFY, ASS_MESSAGE, 0);
			return true;
		}
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_SEND_CARD:		//����״̬(��δ����)
		{
			//��������
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
			return true;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			if(pDesk->sUserData.m_bIsHu[bDeskStation])//����Ѿ�������
			{
				memcpy(&GameStationData.m_HuData,&pDesk->sGameData.T_HuPaiXZMJ[bDeskStation],sizeof(tagHuPaiEx));//��ҵĺ�������
			}
			for(int i=0;i<4;++i)//����Ƿ��Ѿ�����
			{
				if(pDesk->sUserData.m_bIsHu[i])//����Ѿ�������
				{
					GameStationData.m_bIsHu[i] = true;
					for(int j=0;j<17;j++)
					{
						GameStationData.m_byArHandPai[i][j] = pDesk->sUserData.m_byArHandPai[i][j];
					}
				}
			}
			//��������
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
			//if(!pDesk->sUserData.m_bIsHu[bDeskStation] && pDesk->sGameData.T_CPGNotify[bDeskStation].m_byMaxThing > ACTION_NO && pDesk->sGameData.T_CPGNotify[bDeskStation].m_byMaxThing <= ACTION_HU)//����г����ܾͽ��д���
			//{
			//	pDesk->SendGameDataEx(bDeskStation, &pDesk->sGameData.T_CPGNotify[bDeskStation], sizeof(pDesk->sGameData.T_CPGNotify[bDeskStation]), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
			//}
			return true;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{//��������
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
			return true;
		}
	}
    return true;
}

///���ղ�������Ϣ
int ThingHandleEx::HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser)
{
	pDesk->KillTimer(TIME_CHECK_GAME_MSG);
	pDesk->SetTimer(TIME_CHECK_GAME_MSG,TIME_DEFAULT_HANDLE);//�������ó�ʱ��ʱ��

    switch (code)
    {
    case ASS_GM_SHORT_STOP:
        {
            return 0;
        }
        break;
	case ASS_SEZIFINISH:	//ɫ�Ӷ�������
		{
			return ReceiveSeziFinish(bDeskStation, pData, uSize, bWatchUser);
		}
    case ASS_AUTOCARD://�й�
        {	
            return ReceiveUserTuoGuan(bDeskStation, pData, uSize, bWatchUser);
        }
        break;
    case THING_OUT_PAI:
        {
            // �û�����
            return ReceiveUserOutCard(bDeskStation, pData, uSize, bWatchUser);
        }
        break;
    case ASS_PASS_CHECK:		
        {
            return ReceiveUserPassCheck(bDeskStation, pData, uSize,   bWatchUser);
        }
        break;
    case ASS_GM_AGREE_GAME:		
        {
            //�յ��û�ͬ����Ϸ��Ϣ
            return ReceiveUserAgreeGame(bDeskStation, pData, uSize,   bWatchUser);
        }
        break;
    case THING_PENG_PAI:
        {// �û�����
            return ReceiveUserPengCard(bDeskStation, pData, uSize, bWatchUser);
        }
        break;;
    case THING_GANG_PAI:
        {// �û�����
            return ReceiveUserGangCard(bDeskStation, pData, uSize, bWatchUser);
        }
        break;
    case THING_TING_PAI:
        {// �û�����
            return ReceiveUserTingCard(bDeskStation, pData, uSize, bWatchUser);
        }
        break;
    case THING_HU_PAI:		
        {// �û�����
            return ReceiveUserHuCard(bDeskStation, pData, uSize, bWatchUser);
        }
        break;
    case ASS_HAVE_THING:	
        {//���������뿪
            return ReceiveUserHaveThing(bDeskStation, pData, uSize,   bWatchUser);
        }
        break;
    case ASS_LEFT_RESULT: 
        {//ͬ���û��뿪
            return ReceiveArgeeUserLeft(bDeskStation, pData, uSize,   bWatchUser);
        }
        break;
    case ASS_EXIT://�ȴ���ʼһ�¾�ʱ�˳�
        {
            if(pDesk->m_bGameStation==23)
                return pDesk->GameFinish(bDeskStation,GF_SALE);
            else
                return 0;
        }
        break;
    case THING_DING_QUE:        // ��ȱ
        {
             return ReceiveUserDingQue(bDeskStation, pData, uSize,   bWatchUser);
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
int ThingHandleEx::ReceiveSeziFinish(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return ThingHandle::ReceiveSeziFinish(bDeskStation, pData, uSize, bWatchUser);
}

// �յ��й���Ϣ
int ThingHandleEx::ReceiveUserTuoGuan(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return ThingHandle::ReceiveUserTuoGuan(bDeskStation, pData, uSize, bWatchUser);
}

// �û�����
int ThingHandleEx::ReceiveUserOutCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
    tagOutPaiEx * pOutCard = (tagOutPaiEx *)pData;

    if(pOutCard == NULL || bWatchUser || pDesk->m_bGameStation != GS_PLAY_GAME)
    {
        DebugPrintf("�û� %d ����ʧ�ܣ�����Ϸ״̬", bDeskStation);
		OutputDebugString("�������� ReceiveUserOutCard   ����Ϸ״̬");
        return 0;
    }

    //if(m_curState.thingDoing!=THING_OUT_PAI || m_curState.ucCurChair!=bDeskStation)
    //{
    //    DebugPrintf("�û� %d ����ʧ�ܣ��ǳ���״̬", bDeskStation);
    //    return 0;
    //}
	if(pDesk->sUserData.m_bIsHu[bDeskStation])//�Ѿ����Ƶ���ң����ܳ�����
		return 0;

    if(pOutCard->byPs == 255 || pDesk->sUserData.GetAHandPaiCount(pOutCard->byUser,pOutCard->byPs)==0 || !pDesk->sUserData.IsOutPaiPeople(pOutCard->byUser))
    {
        DebugPrintf("�û� %d ����ʧ�ܣ�û�д���", bDeskStation);

		OutputDebugString("�������� ReceiveUserOutCard   û�д���");
        return 0;
    }

    // ����ȱ
    if(pDesk->sUserData.GetAKindPaiCount(pOutCard->byUser, pDesk->sUserData.m_byDingQue[pOutCard->byUser]) >0 ) 
    {
        // ������Ʋ���ȱ
        if (pOutCard->byPs/10 != pDesk->sUserData.m_byDingQue[pOutCard->byUser])
        {
            DebugPrintf("�û� %d ����ʧ�ܣ�����ȱ", bDeskStation);

			OutputDebugString("�������� ReceiveUserOutCard   ����ȱ");
            return 0;
        }
    }
	pDesk->sGameData.T_HuPai.Clear();//������Ƽ�¼ 2012-08-15
	SetDoingThing(bDeskStation,THING_OUT_PAI);//����ִ���¼�

    for(int i=0;i<PLAY_COUNT;++i)//��¼����״̬��ӵ�ڸܿ��͸ܺ���
    {
		if(i != bDeskStation)
			pDesk->sGameData.m_bGangState[i] = false;
	}
	pDesk->sUserData.m_bQiHu[bDeskStation] = false;//�����������״̬


	//��¼�Ѿ���������
	pDesk->sUserData.m_bHaveOut[bDeskStation] = true;

    pDesk->sGameData.T_OutPai = * pOutCard;
    pDesk->sGameData.m_byThingDoing = THING_OUT_PAI;//��¼����ִ�е��¼�
    pDesk->sGameData.m_byNextAction = ACTION_NO;
    pDesk->sUserData.m_byOtherOutPai = pOutCard->byPs;
    //ɾ��һ������
    pDesk->sUserData.DelleteAHandPai(pOutCard->byUser,pOutCard->byPs);
    //���һ�ų���
    pDesk->sUserData.AddToOutPai(pOutCard->byUser,pOutCard->byPs);
    //������ʱ����
    ClearTempData();
	pDesk->sUserData.m_byBuGangPai = 255;//���ܵ÷ֵ��ƣ����ڲ��ܣ�ץ���ܲ��ܵ��ƺ����ϲ��ܲ��з֣������޷֣�
	//��¼�¼�
	LogThing(THING_OUT_PAI,pOutCard->byPs,bDeskStation);


    //��ҵ��������
    if(pDesk->sGameData.T_TingPai.bHitTing[pOutCard->byUser] && !pDesk->sUserData.m_bTing[pOutCard->byUser])//��ҵ��������,
    {
        pDesk->sGameData.T_TingPai.bHitTing[pOutCard->byUser] = false;
        if(CLogicBase::IsHaveInArr(pDesk->sGameData.T_CPGNotify[pOutCard->byUser].m_byTingCanOut,pOutCard->byPs,HAND_CARD_NUM))
        {
            pDesk->sUserData.m_bTing[pOutCard->byUser] = true;
            pDesk->sGameData.T_OutPai.bTing = true;
        }
        else
        {
            pDesk->sUserData.m_bTing[pOutCard->byUser] = false;
            pDesk->sGameData.T_TingPai.bHitTing[pOutCard->byUser] = false;
        }
    }
    //��������
    for(int i=0;i<4;++i)
    {
        pDesk->sUserData.CopyOutPai(i,pDesk->sGameData.T_OutPai.m_byArOutPai[i]);
    }
    ///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
    pDesk->sUserData.m_bZhuaPaiDir = true;
    for(int i=0;i<4;i++)//��ȡ����״̬
    {
        pDesk->sGameData.T_OutPai.bUserTing[i] = pDesk->sUserData.m_bTing[i];
    }
    //���ͳ�������
    for (int i=0;i<PLAY_COUNT;i++)
    {	
        memset(pDesk->sGameData.T_OutPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_OutPai.m_byArHandPai));
        pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_OutPai.m_byArHandPai,i,false);

        pDesk->SendGameDataEx(i, &pDesk->sGameData.T_OutPai, sizeof(pDesk->sGameData.T_OutPai), MDM_GM_GAME_NOTIFY, THING_OUT_PAI, 0);
        pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_OutPai, sizeof(pDesk->sGameData.T_OutPai), MDM_GM_GAME_NOTIFY, THING_OUT_PAI, 0);
    }


    //������⣺û��������Ӧ��ֱ��ץ��
    if(!CheckAction(pOutCard->byUser,pOutCard->byPs,2))
    {//û�ж�����ֱ����һ�����ץ��
        pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
        ApplyAction(THING_ZHUA_PAI,100);	//������һ���¼�

        // ������״̬
        //SetGameCurState(THING_MAX, 0xFF, 0, 0, false);  
    }
    else
    {
        // ��������״̬
        //SetGameCurState(THING_CPG_NOTIFY, 0xFF, pDesk->sGameData.GetMjSetting().ucBlockTime*1000+TIME_NETWORK, 0, false);

        for (int i=0;i<PLAY_COUNT;i++)
        {	
            if(i==bDeskStation || pDesk->sUserData.m_bFoceLeave[i] || pDesk->sGameData.T_CPGNotify[i].m_byMaxThing == ACTION_NO
                || pDesk->sUserData.m_bIsHu[i])
                continue;
            pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify[i], sizeof(pDesk->sGameData.T_CPGNotify[i]), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
            pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify[i], sizeof(pDesk->sGameData.T_CPGNotify[i]), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);


			if(pDesk->sGameData.T_CPGNotify[i].bHu && pDesk->sUserData.m_MenPai.GetMenPaiNum()<4)//����������ܺ��������
			{
				tagHuPaiEx hu;
				hu.Clear();
				hu.byUser = pDesk->sGameData.T_ZhuaPai.byUser;
				this->ReceiveUserHuCard(i,&hu,sizeof(hu),false);
			}
        }
        
    }
    pDesk->sUserData.m_byApplyMaxAction = ACTION_NO;//��ҵ�ǰ���������¼�������������󣬻ָ�����

    for(int i=0;i<4;i++)//
    {
		if(i == bDeskStation)
			continue;
		pDesk->sUserData.m_bIsGangPao[i] = false;//���ڸܺ��ڵ�״̬
	}

    return 0;
}

// �յ��û�pass ��Ϣ
int ThingHandleEx::ReceiveUserPassCheck(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
    if(pDesk->sGameData.ApplyThingID != 255 || bWatchUser)//�������¼��Ͳ���Ӧpass��Ϣ
    {
       return 0;
    }
    if (pDesk->sGameData.T_CPGNotify[bDeskStation].bHaveReturn || !pDesk->sGameData.T_CPGNotify[bDeskStation].bCanAction)
    {
        return 0;
    }

	
	if(bDeskStation != pDesk->sUserData.m_byNowOutStation && pDesk->sGameData.T_CPGNotify[bDeskStation].bHu)//����ܺ�û�к���,��¼����
	{
		pDesk->sUserData.m_bQiHu[bDeskStation] = true;//����Ƿ�����״̬
	}

    pDesk->sGameData.T_CPGNotify[bDeskStation].bHaveReturn = true;//��¼��һ�Ӧ
    pDesk->sGameData.T_CPGNotify[bDeskStation].m_byMaxThing = ACTION_NO;//�ı���������������

    CheckNextAction();	
	pDesk->sGameData.T_CPGNotify[bDeskStation].Clear();

    return 0;
}

//�յ��û�ͬ����Ϸ��Ϣ
int ThingHandleEx::ReceiveUserAgreeGame(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return ThingHandle::ReceiveUserAgreeGame(bDeskStation, pData, uSize, bWatchUser);
}

//���������뿪
int ThingHandleEx::ReceiveUserHaveThing(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return ThingHandle::ReceiveUserHaveThing(bDeskStation, pData, uSize, bWatchUser);
}

//ͬ���û��뿪
int ThingHandleEx::ReceiveArgeeUserLeft(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return ThingHandle::ReceiveArgeeUserLeft(bDeskStation, pData, uSize, bWatchUser);
}

// �û�������Ϣ
int ThingHandleEx::ReceiveUserPengCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	char buff[500];

    tagPengPaiEx * pPengCard = (tagPengPaiEx *)pData;
    if(pPengCard == NULL || bWatchUser ||  pDesk->m_bGameStation != GS_PLAY_GAME)
    {
        return 0;
    }

    if(bDeskStation == pDesk->sUserData.m_byNowOutStation || !pDesk->sGameData.T_CPGNotify[bDeskStation].bPeng || pPengCard->byPs == 255 || !pDesk->sGameData.m_mjAction.bPeng)
    {
        //���Ϸ�����
        return 0;
    }


    if(pDesk->sUserData.m_byApplyMaxAction < ACTION_PENG)
    {
        pDesk->sUserData.m_byApplyMaxAction = ACTION_PENG;//��ҵ�ǰ���������¼�
    }

    pDesk->sGameData.temp_PengPai.Clear();
    pDesk->sGameData.temp_PengPai = * pPengCard;//��¼��ʱ��������
    pDesk->sGameData.T_CPGNotify[pPengCard->byUser].bHaveReturn = true;//��¼��һ�Ӧ
    pDesk->sGameData.T_CPGNotify[pPengCard->byUser].m_byMaxThing = ACTION_PENG;//�ı���������������
    if(pDesk->sGameData.m_byNextAction < ACTION_PENG)
    {
        pDesk->sGameData.m_byNextAction = ACTION_PENG;
    }
    //�����һ������
    CheckNextAction();
    return 0;
}

// �û�������Ϣ
int ThingHandleEx::ReceiveUserGangCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
    tagGangPaiEx * pGangCard = (tagGangPaiEx *)pData;

    if(pGangCard == NULL || bWatchUser || pDesk->m_bGameStation != GS_PLAY_GAME)
    {
        DebugPrintf("��� %d ������ƣ����ܾ�����ǰ������Ϸ״̬");
        return 0;
    }
    if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bGang || pGangCard->byPs == 255 || !pDesk->sGameData.m_mjAction.bGang)
    {
        DebugPrintf("��� %d ������ƣ����ܾ��������ܸ�");
        //���Ϸ�����
        return 0;
    }


	OutputDebugString("���̲��ԣ� ������ ReceiveUserGangCard  �յ����ƺϷ���Ϣ");

    bool action = false;
    pDesk->sGameData.temp_GangPai.Clear();
    pDesk->sGameData.temp_GangPai = * pGangCard;//��¼��ʱ��������
    if(pGangCard->byType == ACTION_BU_GANG)
    {
        action = CheckAction(pGangCard->byUser,pGangCard->byPs,3);//����Ƿ��н�ȡ���ܵĶ��������Ƶ�
    }
    if(pGangCard->byType == ACTION_MING_GANG)//���ܣ���¼����
    {
        pDesk->sGameData.T_CPGNotify[pGangCard->byUser].bHaveReturn = true;//��¼��һ�Ӧ
        pDesk->sGameData.T_CPGNotify[pGangCard->byUser].m_byMaxThing = ACTION_MING_GANG;//�ı���������������
        if(pDesk->sGameData.m_byNextAction < ACTION_MING_GANG)
        {
            pDesk->sGameData.m_byNextAction = ACTION_MING_GANG;
        }
    }

    switch(pGangCard->byType)
    {
    case ACTION_AN_GANG:
        {
            if(pDesk->sUserData.m_byApplyMaxAction < ACTION_AN_GANG)
            {
                pDesk->sUserData.m_byApplyMaxAction = ACTION_AN_GANG;//��ҵ�ǰ���������¼�
            }
            //���ܿ��Լ�ʱִ�ж���
            BYTE NextID=255,StarTime=255;//�¸��¼���id,255ʱΪ�����¼���Ҫ�ͻ����أ�StarTime�೤ʱ���ʼִ�У�100����Ϊ��λ��
            HandleGangPai(NextID,StarTime);//�����¼�����
            pDesk->sGameData.ApplyThingID = NextID;
            ApplyAction(NextID,StarTime*100);//������һ���¼�
            //���͸�����Ϣ
            for (int i=0;i<PLAY_COUNT;i++)
            {	
                pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_GangPai.m_byArHandPai,i,false);
                pDesk->SendGameDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY , THING_GANG_PAI, 0);
                pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY , THING_GANG_PAI, 0);
            }
        }
        break;
    case ACTION_BU_GANG:
        {

            if(pDesk->sUserData.m_byApplyMaxAction < ACTION_BU_GANG)
            {
                pDesk->sUserData.m_byApplyMaxAction = ACTION_BU_GANG;//��ҵ�ǰ���������¼�
            }
            if(!action)//�������ܣ�ֱ�Ӵ���
            {
                BYTE NextID=255,StarTime=255;//�¸��¼���id,255ʱΪ�����¼���Ҫ�ͻ����أ�StarTime�೤ʱ���ʼִ�У�100����Ϊ��λ��
                HandleGangPai(NextID,StarTime);//�����¼�����
                pDesk->sGameData.ApplyThingID = NextID;
                ApplyAction(NextID,StarTime*100);//������һ���¼�
                for (int i=0;i<PLAY_COUNT;i++)
                {	
                    pDesk->sUserData.CopyGCPData(i,pDesk->sGameData.T_GangPai.m_UserGCPData[i]);
                }
                //���͸�����Ϣ
                for (int i=0;i<PLAY_COUNT;i++)
                {						
                    pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_GangPai.m_byArHandPai,i,false);
                    pDesk->SendGameDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY , THING_GANG_PAI, 0);
                    pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY , THING_GANG_PAI, 0);
                }
            }
            else///�������ܣ�����������Ϣ�����˼��
            {
                for (int i=0;i<PLAY_COUNT;i++)
                {	
                    if(!pDesk->sGameData.T_CPGNotify[i].bCanAction || pDesk->sUserData.m_bFoceLeave[i])
                        continue;
                    pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify[i], sizeof(pDesk->sGameData.T_CPGNotify[i]),  MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
                    pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify[i], sizeof(pDesk->sGameData.T_CPGNotify[i]),  MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
                }
                if(pDesk->sGameData.m_mjRule.bNetCutTuoGuan)
                {
                    NetCutAutoHandle();
                }
                return 0;
            }
        }
        break;
    case ACTION_MING_GANG://����
        {
            if(pDesk->sUserData.m_byApplyMaxAction < ACTION_MING_GANG)
            {
                pDesk->sUserData.m_byApplyMaxAction = ACTION_MING_GANG;//��ҵ�ǰ���������¼�
            }
            //�����һ������
            CheckNextAction();
            return 0;
        }
        break;
    }
    return 0;
}

// �û�������Ϣ
int ThingHandleEx::ReceiveUserTingCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return ThingHandle::ReceiveUserTingCard(bDeskStation, pData, uSize, bWatchUser);
}

// �û�������Ϣ
int ThingHandleEx::ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	OutputDebugString("���ܼ�⣺ReceiveUserHuCard 000");
    // ����ȱ
    if(pDesk->sUserData.GetAKindPaiCount(bDeskStation, pDesk->sUserData.m_byDingQue[bDeskStation]) >0 ) 
    {
        return 0;
    }

	OutputDebugString("���ܼ�⣺ReceiveUserHuCard 111");
    tagHuPaiEx * pHuCard = (tagHuPaiEx *)pData;
    if(pHuCard == NULL || bWatchUser || pDesk->m_bGameStation != GS_PLAY_GAME)
    {
        return 0;
    }

	OutputDebugString("���ܼ�⣺ReceiveUserHuCard 222");
    if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bHu)
    {
        //���Ϸ�����
        return 0;
    }
	OutputDebugString("���ܼ�⣺ReceiveUserHuCard 333");

    if(pDesk->sUserData.m_byApplyMaxAction < ACTION_HU)
    {
        pDesk->sUserData.m_byApplyMaxAction = ACTION_HU;//��ҵ�ǰ���������¼�
    }

    if(bDeskStation == pDesk->sUserData.m_byNowOutStation)//��ǰ������Һ���(����)
    {
        pDesk->sGameData.T_HuPai.bZimo = true;
        pDesk->sGameData.T_HuPai.byDianPao = 255;
        pDesk->sGameData.T_HuPai.byPs = 255;
    }
    else
    {
        pDesk->sGameData.T_HuPai.byDianPao = pDesk->sUserData.m_byNowOutStation;
        pDesk->sGameData.T_HuPai.byPs = pDesk->sUserData.m_byOtherOutPai;// pHuCard->byPs;				//���ڵ���ֵ
    }

    pDesk->sGameData.T_HuPai.bCanH[bDeskStation] = true;
    pDesk->sGameData.T_HuPai.byUserNum++;
    pDesk->sGameData.T_HuPai.byNt = pDesk->sUserData.m_byNtStation;		//ׯ��λ��

    pDesk->sGameData.T_CPGNotify[bDeskStation].bHaveReturn = true;//��¼��һ�Ӧ
    pDesk->sGameData.T_CPGNotify[bDeskStation].m_byMaxThing = ACTION_HU;//�ı���������������

    if(pDesk->sGameData.T_HuPai.byUser == 255)
    {
        pDesk->sGameData.T_HuPai.byUser = bDeskStation;//pHuCard->byUser;
    }
    else if(!pDesk->sGameData.T_HuPai.bZimo)//ѡ�񿿺�������������Һ���
    {
        //���ں��ƣ������ж����ҿ��Ժ��ƣ�һ�ڶ��죩
        pDesk->sGameData.T_HuPai.byUser = bDeskStation;//GetCanHuUser(pDesk->sUserData.m_byNowOutStation,pDesk->sGameData.T_HuPai.bCanH,pDesk->//sGameData.T_HuPai.byUser);//��ȡ�������ҽ������
		pDesk->sGameData.T_HuPai.bDuoXiang = true;//һ�ڶ���

    }


	char buff[800];
	sprintf(buff,"��������ץ�� �յ�������Ϣ %d  λ��%d",pDesk->sGameData.T_HuPai.byUser,bDeskStation);
	OutputDebugString(buff); 

    if(pDesk->sGameData.m_byNextAction < ACTION_HU)
    {
        pDesk->sGameData.m_byNextAction = ACTION_HU;
    }
    for(int i=0;i<MAX_HUPAI_TYPE;++i)
    {
        pDesk->sGameData.T_HuPai.byHuType[bDeskStation][i] = pDesk->sGameData.T_CPGNotify[bDeskStation].byHuType[i];//��ȡ��������
    }
    for(int i=0;i<HAND_CARD_NUM;++i)
    {
        pDesk->sGameData.T_HuPai.m_byArHandPai[bDeskStation][i] = pDesk->sUserData.m_byArHandPai[bDeskStation][i];
    }
    for(int i=0;i<4;++i)
    {
        pDesk->sUserData.CopyGCPData(i,pDesk->sGameData.T_HuPai.m_UserGCPData[i]);
    }

    if(pDesk->sGameData.T_HuPai.bZimo)//���������������
    {
        BYTE NextID=255,StarTime=255;//�¸��¼���id,255ʱΪ�����¼���Ҫ�ͻ����أ�StarTime�೤ʱ���ʼִ�У�100����Ϊ��λ��

        HandleHuPai(NextID,StarTime);//�����¼�����
        pDesk->sGameData.ApplyThingID = NextID;
        ApplyAction(NextID,StarTime*100);//������һ���¼�

		char strdug[300];
		//��ȡ������ҵķ���
		for (int i=0; i<PLAY_COUNT; ++i)
		{
			sprintf(strdug,"Ѫս�齫���ԣ�ReceiveUserHuCard  λ�� %d �ܷ� %d",i,pDesk->sGameData.T_HuPaiXZMJ[bDeskStation].iZongFen[i]);
			OutputDebugString(strdug);
		}

		pDesk->sUserData.m_bIsHu[bDeskStation] = true;

		for(int i=0;i<PLAY_COUNT;i++)
		{
			pDesk->sGameData.T_HuPaiXZMJ[bDeskStation].m_bIsHu[i] = pDesk->sUserData.m_bIsHu[i];
			if(!pDesk->sUserData.m_bIsHu[i])
				continue;
			//��ȡ��������
			for(int j=0;j<MAX_HUPAI_TYPE;j++)
			{
				if(i==bDeskStation)
				{
					pDesk->sGameData.T_HuPaiXZMJ[bDeskStation].byHuType[i][j] = pDesk->sGameData.T_HuPai.byHuType[i][j];
				}
				else
				{
					pDesk->sGameData.T_HuPaiXZMJ[bDeskStation].byHuType[i][j] = pDesk->sGameData.T_HuPaiXZMJ[i].byHuType[i][j];
				}
			}
		}

        //֪ͨ��Һ�����
        memset(pDesk->sGameData.T_HuPaiXZMJ[bDeskStation].m_byArHandPai,255,sizeof(pDesk->sGameData.T_HuPaiXZMJ[bDeskStation].m_byArHandPai));
        pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_HuPaiXZMJ[bDeskStation].m_byArHandPai,0,true);
        for (int i=0;i<PLAY_COUNT;i++)
        {	
            pDesk->sUserData.CopyGCPData(i,pDesk->sGameData.T_HuPaiXZMJ[bDeskStation].m_UserGCPData[i]);
            pDesk->SendGameDataEx(i, &pDesk->sGameData.T_HuPaiXZMJ[bDeskStation], sizeof(pDesk->sGameData.T_HuPaiXZMJ[bDeskStation]), MDM_GM_GAME_NOTIFY, THING_HU_PAI, 0);
            pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_HuPaiXZMJ[bDeskStation], sizeof(pDesk->sGameData.T_HuPaiXZMJ[bDeskStation]), MDM_GM_GAME_NOTIFY, THING_HU_PAI, 0);
        }
        //pDesk->sGameData.T_HuPai.Clear();

    }
    else
    {
        //�����һ������
        CheckNextAction();
    }
    return 0;
}
// �û���ȱ��Ϣ
int ThingHandleEx::ReceiveUserDingQue(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
    tagDingQueEx *pCmd = (tagDingQueEx *)pData;
    if (uSize!=sizeof (tagDingQueEx))
    {
        return -1;
    }
	if (pDesk->sGameData.m_byThingDoing != THING_DING_QUE || bWatchUser)    // ���ڶ�ȱ��
    {
        return 0;
    }

    if (255==pDesk->sUserData.m_byDingQue[bDeskStation] ||( pCmd->byQue>=0 && pCmd->byQue<3))   
    {
        pDesk->sUserData.m_byDingQue[bDeskStation] = pCmd->byQue%3;
		pDesk->sGameData.T_DingQue.byQuePai[bDeskStation] = pCmd->byQue%3;
    }
	pDesk->sGameData.T_DingQue.bFinish[bDeskStation] = true;

	OutputDebugString("Ѫս�齫���ԣ� ������ ReceiveUserDingQue 0000");

	pDesk->sGameData.T_DingQue.byUser = bDeskStation;
	pDesk->sGameData.T_DingQue.bNotify = false;
    for (int i=0;i<PLAY_COUNT;i++)
    {	
        pDesk->SendGameDataEx(i, &pDesk->sGameData.T_DingQue, sizeof(pDesk->sGameData.T_DingQue), MDM_GM_GAME_NOTIFY, THING_DING_QUE, 0);
        pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_DingQue, sizeof(pDesk->sGameData.T_DingQue), MDM_GM_GAME_NOTIFY, THING_DING_QUE, 0);
    }
    bool bDingQueOver = true;
    //�ж��Ƿ�ȱ��� 
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        if (!pDesk->sGameData.T_DingQue.bFinish[i])
        {
            bDingQueOver = false;
            break;
        }
    }
    if (bDingQueOver)
    { // ��ʼ����
        pDesk->sGameData.ApplyThingID = THING_BEGIN_OUT;
        ApplyAction(THING_BEGIN_OUT, 100);
    }
    return 0;
}

///���ƣ�CheckNextAction
///�����������һ������
void ThingHandleEx::CheckNextAction()
{
    if(!IsCanHandle())
    {
        return ;
    }	
    BYTE NextID=255,StarTime=255;//�¸��¼���id,255ʱΪ�����¼���Ҫ�ͻ����أ�StarTime�೤ʱ���ʼִ�У�100����Ϊ��λ��

    MJTHINGS nextState = THING_MAX;
    BYTE ucNextChair = 0xFF;
    unsigned int unLasting = 0;
    unsigned int unWaiting = 0;
    bool bNotify = false;
    switch(pDesk->sGameData.m_byNextAction)//ȷ������һ������Ӧ�¼����ԣ������ܣ���
    {
    case ACTION_PENG:				//���ƶ���
        {            
            HandlePengPai(NextID,StarTime);
            //֪ͨ�������
            for (int i=0;i<PLAY_COUNT;i++)
            {	
                memset(pDesk->sGameData.T_PengPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_PengPai.m_byArHandPai));
                pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_PengPai.m_byArHandPai,i,false);
                pDesk->SendGameDataEx(i, &pDesk->sGameData.T_PengPai, sizeof(pDesk->sGameData.T_PengPai), MDM_GM_GAME_NOTIFY, THING_PENG_PAI, 0);
                pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_PengPai, sizeof(pDesk->sGameData.T_PengPai), MDM_GM_GAME_NOTIFY, THING_PENG_PAI, 0);
            }
            nextState = THING_OUT_PAI;
            ucNextChair = pDesk->sGameData.T_PengPai.byUser;
            unLasting = pDesk->sGameData.GetMjSetting().ucOutcardTime*1000-TIME_NETWORK;
            unWaiting = 0;
            bNotify = true;
        }
        break;
    case ACTION_BU_GANG://���ܶ���
    case ACTION_MING_GANG:			//���ܶ���
        {

            HandleGangPai(NextID,StarTime);
            //֪ͨ�������
            for (int i=0;i<PLAY_COUNT;i++)
            {	
                memset(pDesk->sGameData.T_GangPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_GangPai.m_byArHandPai));
                pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_GangPai.m_byArHandPai,i,false);
                pDesk->SendGameDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY, THING_GANG_PAI, 0);
                pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY, THING_GANG_PAI, 0);
            }
        }
        break;
    case ACTION_HU:					//������
        {
            DebugPrintf("Ϊ %d ִ�к��Ʋ���", pDesk->sGameData.T_HuPai.byUser);

			char pchar[500];
			//���������Ҫ��������е���ɾ��
			if(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].bQiangGang) //���ܺ�
			{
				pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].m_byDianPao,pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].m_iHuPs);
				sprintf(pchar,"���ܼ�⣺ ���ܺ�ɾ�� �������%d������ %d",pDesk->sGameData.T_HuPai.byUser,pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].m_iHuPs);
				OutputDebugString(pchar);
			}
            if(!pDesk->sGameData.T_HuPai.bZimo)
            {
				for(int i=0;i<4;++i)
				{
					if(pDesk->sGameData.T_HuPai.bCanH[i])
					{
						pDesk->sUserData.AddToHandPai(i,pDesk->sGameData.T_CPGNotify[i].m_iHuPs);//
						OutputDebugString("���ܼ�⣺��ӵ�����");

						sprintf(pchar,"���ܼ�⣺ ���%d ��ӵ�����  %d",i,pDesk->sGameData.T_CPGNotify[i].m_iHuPs);
						OutputDebugString(pchar);
					}
				}
            }

            HandleHuPai(NextID,StarTime);
			///��ȡ������ҵķ�������
			BYTE st = pDesk->sGameData.T_HuPai.byUser;
            ///֪ͨ��Һ�����
            memset(pDesk->sGameData.T_HuPaiXZMJ[st].m_byArHandPai,255,sizeof(pDesk->sGameData.T_HuPaiXZMJ[st].m_byArHandPai));


            pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_HuPaiXZMJ[st].m_byArHandPai,0,true);
            for (int i=0;i<PLAY_COUNT;i++)
            {	
                pDesk->sUserData.CopyOutPai(i,pDesk->sGameData.T_HuPaiXZMJ[st].m_byArOutPai[i]);
            }

			for(int i=0;i<PLAY_COUNT;i++)
			{
				pDesk->sGameData.T_HuPaiXZMJ[st].m_bIsHu[i] = pDesk->sUserData.m_bIsHu[i];
				if(!pDesk->sUserData.m_bIsHu[i])
					continue;
				//��ȡ��������
				for(int j=0;j<MAX_HUPAI_TYPE;j++)
				{
					if(i==st)
					{
						pDesk->sGameData.T_HuPaiXZMJ[st].byHuType[i][j] = pDesk->sGameData.T_HuPai.byHuType[i][j];
					}
					else
					{
						pDesk->sGameData.T_HuPaiXZMJ[st].byHuType[i][j] = pDesk->sGameData.T_HuPaiXZMJ[i].byHuType[i][j];
					}
				}
			}

            for (int i=0;i<PLAY_COUNT;i++)
            {	
                pDesk->sUserData.CopyGCPData(i,pDesk->sGameData.T_HuPaiXZMJ[st].m_UserGCPData[i]);
                pDesk->SendGameDataEx(i, &pDesk->sGameData.T_HuPaiXZMJ[st], sizeof(pDesk->sGameData.T_HuPaiXZMJ[st]), MDM_GM_GAME_NOTIFY, THING_HU_PAI, 0);
                pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_HuPaiXZMJ[st], sizeof(pDesk->sGameData.T_HuPaiXZMJ[st]), MDM_GM_GAME_NOTIFY, THING_HU_PAI, 0);
            }
            // ȷ����һ������
            ////pDesk->sGameData.T_HuPai.Clear();
        }
        break;
    case ACTION_NO:
        {
            if(pDesk->sGameData.m_byThingDoing == THING_OUT_PAI)//�����¼�
            {
                DebugPrintf("����CheckNextAction����Action_no�����ٵȴ����ƣ�����ץ�ƶ���");

				OutputDebugString("���̲��ԣ� CheckNextAction ACTION_NO");

                NextID = THING_ZHUA_PAI;//ִ��ץ���¼�
                StarTime = 1;
            }
            else
            {
                DebugPrintf("����CheckNextAction����Action_no�������ȴ�����");
            }
        }
        break;
    }
    pDesk->sGameData.ApplyThingID = NextID;
    ApplyAction(NextID,StarTime*100);//������һ���¼�
  
    //SetGameCurState(nextState, ucNextChair, unLasting, unWaiting, bNotify);
}

BYTE ThingHandleEx::GetCanHuUser(BYTE outst,bool hu[],BYTE hust)//��ȡ�������ҽ������
{

	return ThingHandle::GetCanHuUser(outst,hu, hust);
}

///��ȡ�¼�λ��
BYTE ThingHandleEx::GetNextStation(BYTE station,bool shun)
{
    BYTE re = 255;
    if(shun)//˳ʱ��
    {
        int nLoop = 8;
        re = (station + 1)%PLAY_COUNT;
        while (nLoop-->0)
        {            
            if (!pDesk->sUserData.m_bIsHu[ re ])
            {
                break;
            }
            re = (re + 1)%PLAY_COUNT;
        }
    }
    else
    {
        int nLoop = 8;
        re = (station + 3)%PLAY_COUNT;
        while (nLoop-->0)
        {            
            if (!pDesk->sUserData.m_bIsHu[ re ])
            {
                break;
            }
            re = (re+3)%PLAY_COUNT;
        }
    }
    return re;
}

///������ʱ����
void ThingHandleEx::ClearTempData()
{
	ThingHandle::ClearTempData();
}

///����Ƿ���Խ�����һ������
bool ThingHandleEx::IsCanHandle()
{
    int num = 0, re = 0;
    BYTE max = ACTION_NO;
    BYTE station = pDesk->sGameData.T_OutPai.byUser;
	bool qianggang = false;

    for(int i=0;i<4;++i)//���ҳ�����¼�
    {
        if (pDesk->sUserData.m_bIsHu[i])        // �Ѻ������
        {
            continue;
        }
        if(pDesk->sGameData.T_CPGNotify[i].bQiangGang && pDesk->sGameData.T_CPGNotify[i].bHu)
			qianggang = true;
    }

    if(qianggang)//�����¼�
    {//�������ܺ��Ȳ���


        station = pDesk->sGameData.T_GangPai.byUser;
        for(int i=0;i<4;++i)//���ҳ�����¼�
        {
            if (pDesk->sUserData.m_bIsHu[i])        // �Ѻ������
            {
                continue;
            }
            if(i==station || pDesk->sGameData.T_CPGNotify[i].m_byMaxThing <= max || pDesk->sGameData.T_CPGNotify[i].m_byMaxThing==255)
                continue;
            max = pDesk->sGameData.T_CPGNotify[i].m_byMaxThing;
        }
        if(max == ACTION_NO)
        {
            pDesk->sGameData.m_byNextAction = ACTION_BU_GANG;//����ִ���겹��

            return true;
        }
        //�����Ƿ��������Ӧ
        for(int i=0;i<4;++i)//����¼�������������¼���Ӧ�ĸ���
        {
            if (pDesk->sUserData.m_bIsHu[i])        // �Ѻ������
            {
                continue;
            }
            if(i==station)
                continue;
            if(max == pDesk->sGameData.T_CPGNotify[i].m_byMaxThing && pDesk->sGameData.T_CPGNotify[i].m_byMaxThing!=255)
            {
                num++;
                if(pDesk->sGameData.T_CPGNotify[i].bHaveReturn)
                    re++;
            }
        }
        if(num == re)//����Ӧ��
        {
            pDesk->sGameData.m_byNextAction = max;
            return true;
        }
    }
    else//һ����Ƽ��
    {
        for(int i=0;i<4;++i)//���ҳ�����¼�
        {
            if (pDesk->sUserData.m_bIsHu[i])        // �Ѻ������
            {
                continue;
            }
            if(i==station || pDesk->sGameData.T_CPGNotify[i].m_byMaxThing <= max || pDesk->sGameData.T_CPGNotify[i].m_byMaxThing==255)
                continue;
            max = pDesk->sGameData.T_CPGNotify[i].m_byMaxThing;
        }
        if(max == ACTION_NO)
        {
            pDesk->sGameData.m_byNextAction = ACTION_NO;
            return true;
        }
        for(int i=0;i<4;++i)//����¼�������������¼���Ӧ�ĸ���
        {
            if (pDesk->sUserData.m_bIsHu[i])        // �Ѻ������
            {
                continue;
            }
            if(i==station)
                continue;
            if(max == pDesk->sGameData.T_CPGNotify[i].m_byMaxThing && pDesk->sGameData.T_CPGNotify[i].m_byMaxThing!=255)
            {
                num++;
                if(pDesk->sGameData.T_CPGNotify[i].bHaveReturn)
                    re++;
            }
        }
        if(num == re)//����Ӧ��
        {
            pDesk->sGameData.m_byNextAction = max;
            return true;
        }
    }
    return false;
}

///////////�����¼�����///////////////////////////////////////////////////////////////////////////////////////

//�����¼�,typeҪ������¼�
void ThingHandleEx::ApplyAction(int type,int time)
{
	if(type >= THING_BEGIN && type <= THING_ENG_UN_NORMAL)
	{
		pDesk->sGameData.ApplyThingID= type;
	}

	char buff[500];
	sprintf(buff,"�¼���ApplyAction �����¼�%d ",pDesk->sGameData.ApplyThingID);
	OutputDebugString(buff);

    if(pDesk->sGameData.ApplyThingID != 255)
    {
        pDesk->SetTimer(10,time);
    }
}

///���ƣ�GameCPU	dwj1-2
///��������Ϸ�¼��������ģ�
void ThingHandleEx::GameCPU(int type)
{

    //���õȴ�ɫ�Ӷ�������״̬
    memset(pDesk->sUserData.m_bSiziFinish,0,sizeof(pDesk->sUserData.m_bSiziFinish));
    pDesk->sUserData.m_bWaitSezi = true;

    BYTE nextID = 255,time = 255;    
    switch(type)
    {
    case THING_BEGIN://��Ϸ��ʼ�¼�
        {
            ExecuteGameBegin(nextID,time);
        }
        break;
    case THING_DONG_NT://�׾��Զ�Ϊׯ�¼�
        {
            ExecuteDongNt(nextID,time);
        }
        break;
    case THING_2SEZI_NT	://�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�
        {
            ExecuteTwoSeziNt(nextID,time);
        }
        break;
    case THING_2SEZI_NT_GP://�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�
        {
            ExecuteTwoSeziNtAndDir(nextID,time);
        }
        break;
    case THING_TURN_2SEZI_NT://�׾�������2��ɫ�Ӷ�ׯ���¼�
        {
            ExecuteTurnSeziNt(nextID,time);
        }
        break;
    case THING_2SEZI_DIR://��2��ɫ���¼������Ʒ����¼�
        {
            ExecuteTwoSeziDir(nextID,time);
        }
        break;
    case THING_2SEZI_DIR_GP	://��2��ɫ���¼������Ʒ�������Ƶ��¼�
        {
            ExecuteTwoSeziDirGp(nextID,time);
        }
        break;
    case THING_2SEZI_GP	://��2��ɫ���¼������Ƶ��¼�
        {
            ExecuteTwoSeziGp(nextID,time);
        }
        break;
    case THING_2SEZI_NT_DIR_GP://��ɫ��ȷ��ׯ�ҷ������
        {
            ExecuteTwoSeziNtDirGp(nextID,time);
        }
        break;
    case THING_SEND_PAI://�����¼�
        {
            ExecuteSendPai(nextID,time);
        }
        break;
    case THING_2SEZI_JING://��2��ɫ�Ӷ������¼�
        {
            ExecuteTwoSeziMakeJing(nextID,time);
        }
        break;
    case THING_ALL_BUHUA://Ϊ������Ҳ����¼�
        {
            ExecuteAllBuHua(nextID,time);
        }
        break;
    case THING_ONE_BUHUA://�������ץ�����ƺ󲹻��¼�
        {
            ExecuteOneBuHua(nextID,time);
        }
        break;
    case THING_DING_QUE://��ȱ�¼�
        {
            ExecuteDingQue(nextID,time);
        }
        break;
    case THING_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�
        {
            pDesk->m_bGameStation = GS_PLAY_GAME;
            ExecuteBeginOut(nextID,time);
        }
        break;
    case THING_ZHUA_PAI	://ץ���¼�
        {	
            ExecuteZhuaPai(nextID,time);
        }
        break;
    case THING_COUNT_FEN://����¼�
        {
            ExecuteCountFen(nextID,time);
        }
        break;
    case THING_ENG_HANDLE://���������¼�
        {
            ExecuteNormalEnd(nextID,time);
        }
        break;
    case THING_ENG_UN_NORMAL://�쳣���������¼�
        {
            ExecuteUnNormalEnd(nextID,time);
        }
        break;
    }

	

    pDesk->sGameData.ApplyThingID = nextID;
    ApplyAction(nextID,time*100);//������һ���¼�

}

///��ʼ�¼�����
void ThingHandleEx::ExecuteGameBegin(BYTE &nextID,BYTE &time)
{   
    pDesk->sGameData.T_Begin.byDo = 1;
    pDesk->sGameData.InitData();//��ʼ������
    pDesk->sUserData.InitData();       

	//memset(m_bGangState,0,sizeof(m_bGangState));//��¼����״̬��ӵ�ڸܿ��͸ܺ���


	pDesk->KillTimer(TIME_WAIT_MSG);
	pDesk->SetTimer(TIME_WAIT_MSG,1000);//�������ó�ʱ��ʱ��

	pDesk->sGameData.m_byThingDoing = THING_BEGIN;
	SetDoingThing(0,THING_BEGIN);//����ִ���¼�

    if(pDesk->sUserData.m_byNtStation!=255)
    {
        //�Ѿ�����ׯ��
        nextID = THING_2SEZI_DIR_GP;//��ɫ�ӷ���
    }
    else 
    {
        srand(GetTickCount());
        // �����ׯ
        pDesk->sUserData.m_byNtStation = rand()%PLAY_COUNT;
        nextID = THING_2SEZI_DIR_GP;//��ɫ�ӷ���
    }

    pDesk->sGameData.T_Begin.byNt = pDesk->sUserData.m_byNtStation;			// ׯ��λ��
    pDesk->sGameData.T_Begin.byQuanFeng =  pDesk->sUserData.m_byQuanFeng;	// Ȧ������
    memset(pDesk->sGameData.T_Begin.byMenPai,255,sizeof(pDesk->sGameData.T_Begin.byMenPai));

	pDesk->sUserData.m_byTempNt = 255;//��¼�¾ֻ�ׯ���

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<pDesk->sGameData.m_mjRule.byAllMjCount/4;j++)
        {
            pDesk->sGameData.T_Begin.byMenPai[i][j] = 0; //����
        }
    }

    pDesk->sGameData.T_Begin.byDo = 10;
    pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_byNtStation;
    ///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
    pDesk->sUserData.m_bZhuaPaiDir = true;

    pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_byNtStation;//���Ʒ���(ÿ�ֳ�ʼ��Ϊׯ����ǰ����)


    //���Ϳ�ʼ��Ϣ
    for (int i=0;i<PLAY_COUNT;i++)
    {
        pDesk->SendGameDataEx(i,&pDesk->sGameData.T_Begin,sizeof(pDesk->sGameData.T_Begin),MDM_GM_GAME_NOTIFY,THING_BEGIN,0);
        pDesk->SendWatchDataEx(i,&pDesk->sGameData.T_Begin,sizeof(pDesk->sGameData.T_Begin),MDM_GM_GAME_NOTIFY,THING_BEGIN,0);
    }
    time = 1;

	//��¼�¼�
	LogThing( pDesk->sGameData.m_byThingDoing);
}

///�Ե�һ�����Ϊׯ�ң��Զ�Ϊׯ�¼���
void ThingHandleEx::ExecuteDongNt(BYTE &nextID,BYTE &time)
{
	ThingHandle::ExecuteDongNt(nextID,time);	
	//��¼�¼�
	LogThing( pDesk->sGameData.m_byThingDoing);
	SetDoingThing(0,THING_DONG_NT);//����ִ���¼�
}

///��2��ɫ��ȷ��ׯ�ң������ͣ�
void ThingHandleEx::ExecuteTwoSeziNt(BYTE &nextID,BYTE &time)	
{
	ThingHandle::ExecuteTwoSeziNt(nextID,time);	
}

///��2��ɫ��ȷ��ׯ�Һ����Ʒ��򣨵�����ׯ�ң���㷽��,δʹ�ã�
void ThingHandleEx::ExecuteTwoSeziNtAndDir(BYTE &nextID,BYTE &time)
{
	ThingHandle::ExecuteTwoSeziNtAndDir(nextID,time);
}

///������ɫ��ȷ��ׯ��
void ThingHandleEx::ExecuteTurnSeziNt(BYTE &nextID,BYTE &time)	
{
	ThingHandle::ExecuteTurnSeziNt(nextID,time);	
}

///��2��ɫ��ȷ��ׯ��+���Ʒ���+���ƶ�����������ׯ�ң���㷽��С�������
void ThingHandleEx::ExecuteTwoSeziNtDirGp(BYTE &nextID,BYTE &time)	
{
	ThingHandle::ExecuteTwoSeziNtDirGp(nextID,time);	
	pDesk->sUserData.m_bWaitSezi = true;
}

///��2��ɫ��ȷ�����Ʒ��򣨵�����ȷ������
void ThingHandleEx::ExecuteTwoSeziDir(BYTE &nextID,BYTE &time)	
{
	ThingHandle::ExecuteTwoSeziDir(nextID,time);
	pDesk->sUserData.m_bWaitSezi = true;
}

///��2��ɫ��ȷ�����Ʒ�������ƶ����������ͷ���С�������
void ThingHandleEx::ExecuteTwoSeziDirGp(BYTE &nextID,BYTE &time)	
{    
	SetDoingThing(0,THING_2SEZI_DIR_GP);//����ִ���¼�

    pDesk->sGameData.m_byThingDoing = THING_2SEZI_DIR_GP;//��¼���ڷ������¼�
    ///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�	
    pDesk->sGameData.T_TwoSeziDirAndGetPai.byUser = pDesk->sUserData.m_byNtStation;
    //ҡ����
    CLogicBase::GetSeziData(&pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0,&pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1);
    pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0 = pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0%6+1;
    pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1 = pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1%6+1;
    //���Ʒ���
    pDesk->sUserData.m_MenPai.byGetPaiDir = (pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1 + pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0)%4-1;
    //���Ƶ���
    pDesk->sUserData.m_MenPai.byGetPai = min(pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0, pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1);
    if(pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0 > pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1)
    {
        pDesk->sUserData.m_MenPai.byGetPai = pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1-1;
    }
    pDesk->sGameData.T_TwoSeziDirAndGetPai.byGetPai = pDesk->sUserData.m_MenPai.byGetPai;

    //֪ͨ�ͻ���
    for (int i=0;i<PLAY_COUNT;i++)
    {
        pDesk->SendGameDataEx(i, &pDesk->sGameData.T_TwoSeziDirAndGetPai, sizeof(pDesk->sGameData.T_TwoSeziDirAndGetPai), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR_GP, 0);
    }
    pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_TwoSeziDirAndGetPai, sizeof(pDesk->sGameData.T_TwoSeziDirAndGetPai), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR_GP, 0);
    
    //���õȴ�ɫ�Ӷ�������״̬
    //memset(pDesk->sUserData.m_bSiziFinish,0,sizeof(pDesk->sUserData.m_bSiziFinish));
    //pDesk->sUserData.m_bWaitSezi = true;
	pDesk->sUserData.m_bWaitSezi = true;
    nextID = THING_SEND_PAI;
    time = 30;

	//��¼�¼�
	LogThing( pDesk->sGameData.m_byThingDoing);
}

///��2��ɫ��ȷ�����ƶ����������Ͷ�����
void ThingHandleEx::ExecuteTwoSeziGp(BYTE &nextID,BYTE &time)	
{
	ThingHandle::ExecuteTwoSeziGp(nextID,time);	
}

///��ע�¼���ȷ��ׯ�Һ󣬿���ͨ��ѡ���ע������Ӯ��
void ThingHandleEx::ExecuteAddNote(BYTE &nextID,BYTE &time)	
{
	ThingHandle::ExecuteAddNote(nextID,time);
}

///����
void ThingHandleEx::ExecuteSendPai(BYTE &nextID,BYTE &time)	
{
    pDesk->sGameData.m_byThingDoing = THING_SEND_PAI;//��¼���ڷ������¼�

	SetDoingThing(0,THING_SEND_PAI);//����ִ���¼�

    ///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
    pDesk->sUserData.m_bZhuaPaiDir = true;    
    ///�����¼�
    pDesk->sGameData.T_SendPai.byDo = 1;	//    
    //������
    DisPatchCard();

    for(int i=0;i<PLAY_COUNT;i++)
    {
        for(int j=0;j<MAX_HAND_PAI-1;++j)
        {
            GetPai(i,true,1);
        }
    }
    //ׯ�Ҷ�ץһ����
    BYTE card = 255;
    card=GetPai(pDesk->sUserData.m_byNtStation,true,1);
    //��������
    ChangeMenPai();
    for(int i=0;i<PLAY_COUNT;++i)
    {
        pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_SendPai.byMenPai[i]);//��������
        pDesk->sUserData.SortHandPai(i,false);
    }



    for(int i=0;i<PLAY_COUNT;i++)
    {	
        memset(pDesk->sGameData.T_SendPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_SendPai.m_byArHandPai));
        pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_SendPai.m_byArHandPai,i,false);
        pDesk->SendGameDataEx(i, &pDesk->sGameData.T_SendPai, sizeof(pDesk->sGameData.T_SendPai), MDM_GM_GAME_NOTIFY, THING_SEND_PAI, 0);
        pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_SendPai, sizeof(pDesk->sGameData.T_SendPai), MDM_GM_GAME_NOTIFY, THING_SEND_PAI, 0);
    }

    nextID = THING_DING_QUE;//��һ���¼��Ĵ��ţ���ʼ��ȱ
    time = 10;

	pDesk->sUserData.m_bWaitSezi = false;

	//��¼�¼�
	LogThing( pDesk->sGameData.m_byThingDoing );
}

///��2��ɫ�Ӷ����¼�
void ThingHandleEx::ExecuteTwoSeziMakeJing(BYTE &nextID,BYTE &time)	
{
	ThingHandle::ExecuteTwoSeziMakeJing(nextID,time);	
}

///������Ҳ����¼�
void ThingHandleEx::ExecuteAllBuHua(BYTE &nextID,BYTE &time)	
{
	ThingHandle::ExecuteAllBuHua(nextID,time);	
}

///������Ҳ����¼�
void ThingHandleEx::ExecuteOneBuHua(BYTE &nextID,BYTE &time)
{
	ThingHandle::ExecuteOneBuHua(nextID,time);	
}

///��ȱ�¼�
void ThingHandleEx::ExecuteDingQue(BYTE &nextID,BYTE &time)
{
    pDesk->sGameData.m_byThingDoing = THING_DING_QUE;//��¼���ڷ������¼�
	SetDoingThing(0,THING_DING_QUE);//����ִ���¼�

	OutputDebugString("Ѫս�齫�������� ExecuteDingQue ");
    // �ȴ���Ҷ�ȱ
    nextID = 255;       // ���¸��¼����ȴ���ȱ���
	time = 255;

	//�ȸ�һ�����ȱ��
	srand(GetTickCount());
	for(int i=0;i<PLAY_COUNT;++i)
	{
		pDesk->sUserData.m_byDingQue[i] = rand()%3;
	}


	//����֪ͨ
	pDesk->sGameData.T_DingQue.bNotify = true;
    for (int i=0;i<PLAY_COUNT;i++)
    {	

		OutputDebugString("Ѫս�齫�������� ExecuteDingQue  ���Ͷ�ȱ֪ͨ");

		pDesk->sGameData.T_DingQue.byUser = i;
        pDesk->SendGameDataEx(i, &pDesk->sGameData.T_DingQue, sizeof(pDesk->sGameData.T_DingQue), MDM_GM_GAME_NOTIFY, THING_DING_QUE, 0);
    }
    pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_DingQue, sizeof(pDesk->sGameData.T_DingQue), MDM_GM_GAME_NOTIFY, THING_DING_QUE, 0);
	
    //SetGameCurState(THING_DING_QUE, 0xFF, pDesk->sGameData.GetMjSetting().ucDingQueTime*1000+TIME_NETWORK);
	
	
	//��¼�¼�
	LogThing( pDesk->sGameData.m_byThingDoing );
	NetCutAutoHandle();
}

///��ʼ����֪ͨ�¼�
void ThingHandleEx::ExecuteBeginOut(BYTE &nextID,BYTE &time)
{    

	char strdug[300];
	OutputDebugString("Ѫս�齫���ԣ�ExecuteBeginOut");
    pDesk->sGameData.m_byThingDoing = THING_BEGIN_OUT;//��¼���ڷ������¼�

	SetDoingThing(pDesk->sUserData.m_byNtStation,THING_BEGIN_OUT);//����ִ���¼�

    ///��ʼ����֪ͨ�¼� 
    pDesk->sGameData.T_BeginOutPai.byNt = pDesk->sUserData.m_byNtStation;//ׯ��λ��
    pDesk->sGameData.T_BeginOutPai.byUser = pDesk->sUserData.m_byNtStation;//ׯ��λ��
    pDesk->sUserData.m_byNowOutStation = pDesk->sUserData.m_byNtStation;//����λ��
    for(int i=0;i<4;i++)
    {
		pDesk->sGameData.T_BeginOutPai.byQuePai[i] = pDesk->sUserData.m_byDingQue[i];//��ȡȱ����
        pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_BeginOutPai.byMenPai[i]);//��������
    }
    ChangeMenPai();//ת������


	sprintf(strdug,"�¼���ExecuteGameBegin ��״̬ %d %d %d %d",pDesk->sGameData.m_bGangState[0] ,pDesk->sGameData.m_bGangState[1] ,pDesk->sGameData.m_bGangState[2] ,pDesk->sGameData.m_bGangState[3] );
	OutputDebugString(strdug);

    //��⶯��
    pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNtStation].Clear();
    CheckAction(pDesk->sUserData.m_byNtStation,pDesk->sUserData.GetLastHandPai(pDesk->sUserData.m_byNowOutStation),0);    
    //����֪ͨ
    for (int i=0;i<PLAY_COUNT;i++)
    {	
        pDesk->SendGameDataEx(i, &pDesk->sGameData.T_BeginOutPai, sizeof(pDesk->sGameData.T_BeginOutPai), MDM_GM_GAME_NOTIFY, THING_BEGIN_OUT, 0);
    }
    pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_BeginOutPai, sizeof(pDesk->sGameData.T_BeginOutPai), MDM_GM_GAME_NOTIFY, THING_BEGIN_OUT, 0);


    DebugPrintf("��Ϸ��ʼ���������״̬��%dΪׯ", pDesk->sUserData.m_byNtStation);
    // �������״̬
    //SetGameCurState(THING_OUT_PAI, pDesk->sUserData.m_byNtStation, pDesk->sGameData.GetMjSetting().ucOutcardTime*1000+TIME_NETWORK,0,false);
    
    pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNtStation].bZhuaPai = true;
    if (pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNtStation].bCanAction)
    {
		sprintf(strdug,"Ѫս�齫���ԣ�ExecuteBeginOut ����֪ͨ ׯ��λ��%d ",pDesk->sUserData.m_byNtStation);
		OutputDebugString(strdug);
        pDesk->SendGameDataEx(pDesk->sUserData.m_byNtStation, &pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNtStation] ,sizeof(tagCPGNotifyEx), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
        pDesk->SendWatchDataEx(pDesk->sUserData.m_byNtStation, &pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNtStation] ,sizeof(tagCPGNotifyEx), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
    }

    nextID = 255;    

	//��¼�¼�
	LogThing( pDesk->sGameData.m_byThingDoing );
}

///ץ���¼�
void ThingHandleEx::ExecuteZhuaPai(BYTE &nextID,BYTE &time)	
{
    ///ץ�����¼�
    nextID = 255;
    BYTE card = 255;
    if(pDesk->sUserData.m_MenPai.GetMenPaiNum()<=pDesk->sGameData.m_mjRule.byGamePassNum)
    {
		pDesk->sGameData.T_HuPai.Clear();
        nextID = THING_ENG_HANDLE;
        time = 1;
		
			OutputDebugString("��������ExecuteZhuaPai");
    }
    else
    {
		char buff[800];
		sprintf(buff,"��������ץ�� ExecuteZhuaPai  ----- MenPaiNum=%d PassNum=%d  donging = %d",pDesk->sUserData.m_MenPai.GetMenPaiNum(),pDesk->sGameData.m_mjRule.byGamePassNum,pDesk->sGameData.m_byThingDoing);
		OutputDebugString(buff); 
		//if()

        if( pDesk->sGameData.m_byThingDoing == THING_OUT_PAI)//�����¼�
        {
            pDesk->sUserData.m_byNowOutStation = GetNextStation(pDesk->sUserData.m_byNowOutStation);//ץ��λ��
        }

        if( THING_HU_PAI == pDesk->sGameData.m_byThingDoing)//�����¼�
        {
			char nick[100];
			pDesk->GetUserNickName(pDesk->sGameData.T_HuPai.byUser,nick);
			sprintf(buff,"��������ץ�� ������ң�%s λ��%d",nick,pDesk->sGameData.T_HuPai.byUser);
			OutputDebugString(buff); 
            //pDesk->sUserData.m_byNowOutStation = GetNextStation(pDesk->sUserData.m_byNowOutStation);//ץ��λ��
            //pDesk->sUserData.m_byNowOutStation = GetNextStation(pDesk->sGameData.T_HuPai.byUser);//ץ��λ��
			BYTE st = pDesk->sGameData.T_HuPai.byUser;
			for(int i=0;i<3;++i)
			{
				st = GetNextStation(st);
				if(!pDesk->sUserData.m_bIsHu[st])//
					break;
			}

			pDesk->sUserData.m_byNowOutStation = st;//ץ��λ��
			pDesk->GetUserNickName(st,nick);
			sprintf(buff,"��������ץ�� ������ң�%s",nick);
			OutputDebugString(buff); 

        }
		pDesk->sGameData.T_HuPai.Clear();//������Ƽ�¼


        pDesk->sGameData.m_byThingDoing = THING_ZHUA_PAI;//��¼���ڷ������¼�

		SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_ZHUA_PAI);//����ִ���¼�

		pDesk->sUserData.m_bQiHu[pDesk->sUserData.m_byNowOutStation] = false;//�����������״̬

        pDesk->sGameData.T_ZhuaPai.byUser = pDesk->sUserData.m_byNowOutStation;//ׯ��λ��
        card = GetPai(pDesk->sUserData.m_byNowOutStation,pDesk->sUserData.m_bZhuaPaiDir,0,&pDesk->sGameData.T_ZhuaPai.byGetPaiIndex);//����ץ��
        pDesk->sGameData.T_ZhuaPai.byPs = card;		//ץ�Ƶ�ֵ

		//ץ������ǿ�ƽ���
		if(card==255 && (pDesk->sUserData.m_MenPai.GetMenPaiNum()-5)<=pDesk->sGameData.m_mjRule.byGamePassNum)
		{
			nextID = THING_ENG_HANDLE;
			time = 1;
		}

		//��¼�¼�
		LogThing( THING_ZHUA_PAI,card,pDesk->sUserData.m_byNowOutStation);

        ///���������Ҫץ����(�������汾)
        if(pDesk->sUserData.m_SetData[pDesk->sUserData.m_byNowOutStation].bySetType==2)
        {		
            DebugPrintf("�����˷�����ץ��");
            pDesk->sUserData.DelleteAHandPai(pDesk->sUserData.m_byNowOutStation,card);
            card = pDesk->sUserData.m_SetData[pDesk->sUserData.m_byNowOutStation].mjdata[0];
            pDesk->sUserData.AddToHandPai(pDesk->sUserData.m_byNowOutStation,card);

			pDesk->sUserData.m_SetData[pDesk->sUserData.m_byNowOutStation].Init();
        }

        //�������
        pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].Clear();
        CheckAction(pDesk->sUserData.m_byNowOutStation,card,0);
        if(pDesk->sUserData.m_bTuoGuan[pDesk->sUserData.m_byNowOutStation])//����й�
        {
            pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bChi = false;
            memset(pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].m_iChi,255,sizeof(pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].m_iChi));
            pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bPeng = false;
        }
        if(pDesk->sUserData.m_bTing[pDesk->sUserData.m_byNowOutStation] && !pDesk->sGameData.m_mjAction.bGangAfterTing)
        {
            pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bGang = false;
            memset(pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].m_iGangData,255,sizeof(pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].m_iGangData));
        }

        ChangeMenPai();//ת������

        for(int i=0;i<4;i++)
        {
            pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_ZhuaPai.byMenPai[i]);//��������
            pDesk->sUserData.CopyHuaPai(i,pDesk->sGameData.T_ZhuaPai.m_byArHuaPai[i]);//��������
            pDesk->sUserData.CopyOutPai(i,pDesk->sGameData.T_ZhuaPai.m_byArOutPai[i]);//��������
        }

		pDesk->sUserData.m_byBuGangPai = card;//���ܵ÷ֵ��ƣ����ڲ��ܣ�ץ���ܲ��ܵ��ƺ����ϲ��ܲ��з֣������޷֣�


		//sprintf(buff,"������ץ�� ���%d ��%d  ",pDesk->sUserData.m_byNowOutStation,pDesk->sUserData.m_byBuGangPai);
		//OutputDebugString(buff);
    }

    for(int i=0;i<PLAY_COUNT;++i)//��¼����״̬��ӵ�ڸܿ��͸ܺ���
    {
		if(i != pDesk->sUserData.m_byNowOutStation)
			 pDesk->sGameData.m_bGangState[i] = false;
	}


    BYTE pai = pDesk->sGameData.T_ZhuaPai.byPs ;
    ///����������
    for (int i=0;i<PLAY_COUNT;i++)
    {			
        pDesk->sGameData.T_ZhuaPai.byPs = 255;
        memset(pDesk->sGameData.T_ZhuaPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_ZhuaPai.m_byArHandPai));
        pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_ZhuaPai.m_byArHandPai,i,false);
        if(pDesk->sGameData.T_ZhuaPai.byUser == i)
        {
            pDesk->sGameData.T_ZhuaPai.byPs = pai;
        }
        pDesk->SendGameDataEx(i, &pDesk->sGameData.T_ZhuaPai, sizeof(pDesk->sGameData.T_ZhuaPai), MDM_GM_GAME_NOTIFY, THING_ZHUA_PAI, 0);
        pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_ZhuaPai, sizeof(pDesk->sGameData.T_ZhuaPai), MDM_GM_GAME_NOTIFY, THING_ZHUA_PAI, 0);
    }
    pDesk->sGameData.T_ZhuaPai.byPs = pai;
    if(nextID == 255)
    {
        // �������״̬
        //SetGameCurState(THING_OUT_PAI, pDesk->sGameData.T_ZhuaPai.byUser,  pDesk->sGameData.GetMjSetting().ucOutcardTime*1000*2+TIME_NETWORK,0,false);

        //id=255˵����û������Ҳ���ǲ��������Է��ͳ�����֪ͨ��������߾�ֱ�ӳ���
        pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_ZhuaPai.byUser].byUser = pDesk->sGameData.T_ZhuaPai.byUser;
        pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_ZhuaPai.byUser].bZhuaPai = true;

        if (pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_ZhuaPai.byUser].bCanAction)
        {
			if(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_ZhuaPai.byUser].bHu && pDesk->sUserData.m_MenPai.GetMenPaiNum()<4)//����������ܺ��������
			{
				//tagHuPaiEx hu;
				//hu.Clear();
				//hu.byUser = pDesk->sGameData.T_ZhuaPai.byUser;
				//this->ReceiveUserHuCard(pDesk->sGameData.T_ZhuaPai.byUser,&hu,sizeof(hu),false);

			}

			OutputDebugString("�¼���ExecuteZhuaPai ����������Ϣ 111 ");

            pDesk->SendGameDataEx(pDesk->sGameData.T_ZhuaPai.byUser, &pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_ZhuaPai.byUser]
            , sizeof(tagCPGNotifyEx), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
            pDesk->SendWatchDataEx(pDesk->sGameData.T_ZhuaPai.byUser, &pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_ZhuaPai.byUser]
            , sizeof(tagCPGNotifyEx), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
        } 
		////�����Զ�����
		//if(pDesk->sUserData.m_bFoceLeave[pDesk->sUserData.m_byNowOutStation])
		//{
		//	this->NetCutAutoHandle();
		//}
    }
}

///����¼�
void ThingHandleEx::ExecuteCountFen(BYTE &nextID,BYTE &time)
{
	ThingHandle::ExecuteCountFen(nextID,time);	
}

///������Ϸ�����¼�
void ThingHandleEx::ExecuteNormalEnd(BYTE &nextID,BYTE &time)
{    
	pDesk->sGameData.m_byThingDoing = THING_ENG_HANDLE;//��ǰִ���¼�

    pDesk->sGameData.T_CountFen.Clear();  
	pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_CountFen.m_byArHandPai,0,true);
	pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_HuPai.m_byArHandPai,0,true);

    int nHuCount = 0;
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        if (pDesk->sUserData.m_bIsHu[i])
        {
            nHuCount++;    
			pDesk->sGameData.T_CountFen.bHu[i] = true;
			pDesk->sGameData.T_CountFen.byUser = i;
			pDesk->sGameData.T_CountFen.m_bIsZiMo[i] = pDesk->sGameData.T_HuPaiXZMJ[i].bZimo;  //�Ƿ�����
			pDesk->sGameData.T_CountFen.m_byDianPao[i] = pDesk->sGameData.T_HuPaiXZMJ[i].byDianPao; //����λ��
			pDesk->sGameData.T_CountFen.m_iZongFan[i] = pDesk->sGameData.T_HuPaiXZMJ[i].m_iZongFan[i];//�ܷ���
			pDesk->sGameData.T_CountFen.m_iGengCount[i] = pDesk->sUserData.m_byGengCount[i];//��Ҹ���

			for(int j=0;j<20;++j)
			{
				pDesk->sGameData.T_CountFen.byHuType[i][j] =  pDesk->sGameData.T_HuPaiXZMJ[i].byHuType[i][j];
			}
        }
    }

	if(nHuCount <= 2)
	{//���֣��黨�����
		pDesk->sGameData.T_CountFen.byUser = 255;
		tagHuPaiEx hu;
		hu.Clear();
		pDesk->sCountFen.CountFen(hu,pDesk->sGameData.T_CountFen);
		
	}
	else
	{
		pDesk->sGameData.T_CountFen.m_bHuaJiao = false;
	}

    // �����н������һ��
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		//�ܷ�
        pDesk->sGameData.T_CountFen.iZongFen[i] += (pDesk->sGameData.T_CountFenXZMJ[0].iZongFen[i]+ pDesk->sGameData.T_CountFenXZMJ[1].iZongFen[i]+pDesk->sGameData.T_CountFenXZMJ[2].iZongFen[i]+ pDesk->sGameData.T_CountFenXZMJ[3].iZongFen[i]);
		//���Ʒ�
		pDesk->sGameData.T_CountFen.iHuFen[i] += (pDesk->sGameData.T_CountFenXZMJ[0].iHuFen[i]+ pDesk->sGameData.T_CountFenXZMJ[1].iHuFen[i]+pDesk->sGameData.T_CountFenXZMJ[2].iHuFen[i]+ pDesk->sGameData.T_CountFenXZMJ[3].iHuFen[i]);
		//�η�����
		//pDesk->sGameData.T_CountFen.iGangFen[i] += (pDesk->sGameData.T_CountFenXZMJ[0].iGangFen[i]+ pDesk->sGameData.T_CountFenXZMJ[1].iGangFen[i]+pDesk->sGameData.T_CountFenXZMJ[2].iGangFen[i]+ pDesk->sGameData.T_CountFenXZMJ[3].iGangFen[i]);
		//����ת��
        pDesk->sGameData.T_CountFen.m_iZhuanYi[i] += (pDesk->sGameData.T_CountFenXZMJ[0].m_iZhuanYi[i]+ pDesk->sGameData.T_CountFenXZMJ[1].m_iZhuanYi[i]+pDesk->sGameData.T_CountFenXZMJ[2].m_iZhuanYi[i]+ pDesk->sGameData.T_CountFenXZMJ[3].m_iZhuanYi[i]);

		if(pDesk->sGameData.T_CountFenXZMJ[0].m_bZhuanYi || pDesk->sGameData.T_CountFenXZMJ[1].m_bZhuanYi || pDesk->sGameData.T_CountFenXZMJ[2].m_bZhuanYi || pDesk->sGameData.T_CountFenXZMJ[3].m_bZhuanYi)
		{
			pDesk->sGameData.T_CountFen.m_bZhuanYi = true;
		}
    }

	//�η����������
	int igang[4];
	memset(igang,0,sizeof(igang));
	pDesk->sCountFen.CountGangFen(igang);

    for (int i=0; i<PLAY_COUNT; ++i)
    {
		pDesk->sGameData.T_CountFen.iZongFen[i] += igang[i];
		pDesk->sGameData.T_CountFen.iGangFen[i] = igang[i];
	}


    ///��Ϸ���������¼�
	if(pDesk->sUserData.m_byTempNt>0 && pDesk->sUserData.m_byTempNt<PLAY_COUNT)
	{
		pDesk->sUserData.m_byNtStation = pDesk->sUserData.m_byTempNt;//��¼�¾ֻ�ׯ���
	}

    pDesk->sGameData.T_CountFen.byNextNt = pDesk->sUserData.m_byNtStation;
	//��¼�¼�
	LogThing( pDesk->sGameData.m_byThingDoing);

	pDesk->KillTimer(TIME_CHECK_GAME_MSG);
    //����ƽ̨������Ϸ�ӿ�
    pDesk->GameFinish(pDesk->sGameData.T_HuPai.byUser,GF_NORMAL);

	pDesk->KillTimer(TIME_WAIT_MSG);
	this->ReSetOutTime();
}

///�쳣��Ϸ�����¼��¼�
void ThingHandleEx::ExecuteUnNormalEnd(BYTE &nextID,BYTE &time)
{
	ThingHandle::ExecuteUnNormalEnd(nextID,time);
}

///��ȡ�����ݣ�
void ThingHandleEx::DisPatchCard()
{
    //����
    BYTE index=0;
    memset(pDesk->sUserData.m_MenPai.byMenPai,255,sizeof(pDesk->sUserData.m_MenPai.byMenPai));//
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<27;j++)
        {
            pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_WTT[j];
            index++;
        }
    }
    ///������
    srand(GetTickCount());
    BYTE temp=255,data=0;
    for(int i=0;i<index;i++)
    {
        temp = rand()%(index-i)+i;
        data = pDesk->sUserData.m_MenPai.byMenPai[temp];
        pDesk->sUserData.m_MenPai.byMenPai[temp] = pDesk->sUserData.m_MenPai.byMenPai[i];
        pDesk->sUserData.m_MenPai.byMenPai[i] = data;
    }
    ///////////////
    pDesk->sUserData.m_MenPai.byGetPai = pDesk->sUserData.m_MenPai.byGetPai%13;
    //pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_MenPai.byGetPaiDir%4;
    if(pDesk->sUserData.m_MenPai.byGetPaiDir == 255)
    {
        pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_byNtStation;
    }
    pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_MenPai.byGetPaiDir%4;

    int dun = 0,num=0;
    dun = index/8;
    num = (dun*pDesk->sUserData.m_MenPai.byGetPaiDir + pDesk->sUserData.m_MenPai.byGetPai)*2;
    if(num == 0)
        num=2;
    pDesk->sUserData.m_MenPai.byStartIndex = num;//������ʼ����
    pDesk->sUserData.m_MenPai.byEndIndex = num-1;	//���ƽ�������
    pDesk->sUserData.m_MenPai.byAllPaiNum = index;//�����Ƶ�����
}

///������ת����������,�������������е�����ת����ÿ����ҵ�����������
void ThingHandleEx::ChangeMenPai()
{
	//ThingHandle::ChangeMenPai();
	//int count = 0;
	//int num = 0;
	//for(int i=0;i<4;i++)
	//{
	//	num = 27;
	//	if(i == 0 || i == 2)
	//	{
	//		num += 1;
	//	}
	//	else 
	//	{
	//		num -= 1;
	//	}
	//	memset(pDesk->sUserData.m_iArMenPai[i],255,sizeof(pDesk->sUserData.m_iArMenPai[i]));
	//	for(int j=count;j<(count + num) ;++j)//����
	//	{
	//		if(pDesk->sUserData.m_MenPai.byMenPai[j] != 255)
	//		{
	//			pDesk->sUserData.m_iArMenPai[i][j-count] = 0;
	//		}
	//		else
	//		{
	//			pDesk->sUserData.m_iArMenPai[i][j-count] = 255;
	//		}
	//	}		
	//	count += num;
	//}
	for(int i=0;i<28;++i)//0λ�õ�����
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[0][i] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[0][i] = 255;
		}
	}
	for(int i=28;i<54;++i)//1λ�õ�����
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[1][i-28] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[1][i-28] = 255;
		}
	}
	for(int i=54;i<82;++i)//2λ�õ�����
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[2][i-54] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[2][i-54] = 255;
		}
	}
	for(int i=82;i<108;++i)//3λ�õ�����
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[3][i-82] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[3][i-82] = 255;
		}
	}
}

///������ץһ����
///@param station ץ����ҵ�λ�� , head  �Ƿ��ǰ��ץ,type ץ������ 0 ����ץ�ƣ�1��ʼǰ���ƣ�2��ǰ����ץ�ƣ�3����
BYTE ThingHandleEx::GetPai(int station,bool head,BYTE type,BYTE *pIndex)
{
	return ThingHandle::GetPai(station, head, type,pIndex);
}

///������station��ǰ������Щ����
///������station ���λ��,pai �����Ҵ�����ƣ�type ���� ���� 0 ץ�ƣ�1�ܿ���2 ���ڣ�3����
bool ThingHandleEx::CheckAction(int station,BYTE pai,BYTE type)
{
    for(int i=0;i<4;++i)
    {
        //if (pDesk->sUserData.m_bIsHu[i])        // �Ѻ������
        //{
        //    continue;
        //}
        pDesk->sGameData.T_CPGNotify[i].Clear();//����
    }

    bool action = false;
    switch(type)
    {
    case 0://����
    case 1://�ܿ�
        {
			//��
			if(pDesk->sCheckHuPai.CanHu(station,pai,pDesk->sGameData.T_CPGNotify[station].byHuType,true))
			{
				pDesk->sGameData.T_CPGNotify[station].bHu = true;
				pDesk->sGameData.T_CPGNotify[station].m_iHuPs = pDesk->sUserData.GetLastHandPai(station);
				pDesk->sGameData.T_CPGNotify[station].m_byDianPao = station;
			}
            //��
            if(!pDesk->sUserData.m_bTing[station] && pDesk->sCPGAction.CanGang(station,pai,pDesk->sGameData.T_CPGNotify[station].m_iGangData ,true))
            {
                pDesk->sGameData.T_CPGNotify[station].bGang = true;
            }
            if(pDesk->sGameData.T_CPGNotify[station].bHu || pDesk->sGameData.T_CPGNotify[station].bGang)
            {
                action=true;
                pDesk->sGameData.T_CPGNotify[station].bCanAction = true;
            }
        }
        break;
    case 2://����
        {
            for(int i=0;i<PLAY_COUNT;++i)
            {
                if (pDesk->sUserData.m_bIsHu[i] || pDesk->sUserData.m_bFoceLeave[i])        // �Ѻ������,���߶���״̬
                {
                    continue;
                }

                if(i==station)
                    continue;
                //��
                if(!pDesk->sUserData.m_bTing[i] && pDesk->sCPGAction.CanPeng(i,pai))
                {	
                    pDesk->sGameData.T_CPGNotify[i].bPeng = true;
                    pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_PENG;
                }
                //��
                if(!pDesk->sUserData.m_bTing[i] && pDesk->sCPGAction.CanGang(i,pai,pDesk->sGameData.T_CPGNotify[i].m_iGangData,false))
                {
                    pDesk->sGameData.T_CPGNotify[i].bGang = true;
                    pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_MING_GANG;

                }
				//��
				//�������״̬,�������Ⱥ���
				if(!pDesk->sUserData.m_bQiHu[i] && pDesk->sCheckHuPai.CanHu(i,pai,pDesk->sGameData.T_CPGNotify[i].byHuType,false))
				{
					pDesk->sGameData.T_CPGNotify[i].bHu = true;
					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_HU;
					pDesk->sGameData.T_CPGNotify[i].m_iHuPs = pai;
					pDesk->sGameData.T_CPGNotify[i].m_byDianPao = station;
				}
                if(pDesk->sGameData.T_CPGNotify[i].bHu || pDesk->sGameData.T_CPGNotify[i].bPeng || pDesk->sGameData.T_CPGNotify[i].bGang 
                    || pDesk->sGameData.T_CPGNotify[i].bTing)
                {
                    action=true;
                    pDesk->sGameData.T_CPGNotify[i].bCanAction = true;
                }
            }
        }
        break;
    case 3://����
        {
            for(int i=0;i<PLAY_COUNT;++i)
            {
                if (pDesk->sUserData.m_bIsHu[i]|| pDesk->sUserData.m_bFoceLeave[i])        // �Ѻ������,���߶���״̬
                {
                    continue;
                }
                if(i==station)
                    continue;
				if(pDesk->sUserData.m_bQiHu[i] )
				{
					OutputDebugString("���ܼ�⣺�������������ܺ�");
				}
				//��
				if(!pDesk->sUserData.m_bQiHu[i] && pDesk->sCheckHuPai.CanHu(i,pai,pDesk->sGameData.T_CPGNotify[i].byHuType,false))
				{
					OutputDebugString("���ܼ�⣺��������ܺ�");

					pDesk->sCheckHuPai.SetAHupaiType(HUPAI_TYPE_QIANG_GANG, pDesk->sGameData.T_CPGNotify[i].byHuType);//������������
					pDesk->sCheckHuPai.ReSetAHupaiType(HUPAI_TYPE_GANG_PAO, pDesk->sGameData.T_CPGNotify[i].byHuType);//ȥ���ܺ�������

					pDesk->sGameData.T_CPGNotify[i].bHu = true;
					pDesk->sGameData.T_CPGNotify[i].m_iHuPs = pai;//��¼������
					pDesk->sGameData.T_CPGNotify[i].m_byDianPao = station;		
					pDesk->sGameData.T_CPGNotify[i].bQiangGang = true; //���ܺ�

					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_HU;
                    action=true;
                    pDesk->sGameData.T_CPGNotify[i].bCanAction = true;
				}
            }
        }
        break;
    }
    if(action)
    {
        return true;
    }
    return false;
}


///�����Զ�������
void ThingHandleEx::NetCutAutoHandle()
{
	//ThingHandle::NetCutAutoHandle();
	switch(pDesk->sGameData.m_byThingDoing)
	{			        				         
	case THING_2SEZI_NT://�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
	case THING_2SEZI_NT_GP://�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
	case THING_TURN_2SEZI_NT://�׾�������2��ɫ�Ӷ�ׯ���¼�		
	case THING_2SEZI_DIR://��2��ɫ���¼������Ʒ����¼�		
	case THING_2SEZI_DIR_GP://��2��ɫ���¼������Ʒ�������Ƶ��¼�
	case THING_2SEZI_GP://��2��ɫ���¼������Ƶ��¼�												
	case THING_2SEZI_JING://��2��ɫ�Ӷ������¼�			
	case THING_1SEZI_JIN://��1��ɫ�Ӷ������¼�
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(!pDesk->sUserData.m_bSiziFinish[i] && pDesk->sUserData.m_bFoceLeave[i])//�����û�з�������Ϣ
				{
					ReceiveSeziFinish(i,NULL,0,false);
				}
			}
		}break;
	case THING_ALL_BUHUA://Ϊ������Ҳ����¼�
		{
		}
		break;
	case THING_ONE_BUHUA://������Ҳ����¼�	
	case THING_CHI_PAI://�����¼�
	case THING_PENG_PAI://�����¼�
	case THING_GANG_PAI://�����¼�
		{
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sUserData.m_byNowOutStation])//���������Ҷ���
			{
				AutoOutOrGet();//������ҳ���
			}
		}
		break;
	case THING_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�	
		{
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sUserData.m_byNtStation])//���ׯ�Ҷ���
			{
				AutoOutOrGet();
			}
		}
		break;
	case THING_OUT_PAI://�����¼�
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(pDesk->sUserData.m_bFoceLeave[i] && pDesk->sUserData.m_byNowOutStation != i)//�����Ҷ���
				{
					ReceiveUserPassCheck(i,NULL,0,false);//���ͷ�����Ϣ
				}
			}
		}
		break;
	case THING_ZHUA_PAI://ץ���¼�
		{
			OutputDebugString("��ʱ����NetCutAutoHandle 000");
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sUserData.m_byNowOutStation])//���������Ҷ���
			{
				AutoOutOrGet();
			}
		}
		break;
	case THING_DING_QUE:// ��ȱ
		{
			OutputDebugString("��ʱ����OnStateTimer 4444");

            for (int i=0; i<PLAY_COUNT; ++i)
            {
				if(!pDesk->sUserData.m_bFoceLeave[i])//�����Ҷ���
					continue;
				tagDingQueEx cmdDingQue;
				cmdDingQue.byQue = 255/*rand()%3*/;   // �����
				ReceiveUserDingQue(i, &cmdDingQue, sizeof(cmdDingQue), false);
			}
		}
		break;
	}
}

//�������Զ�����
void ThingHandleEx::AutoHandle()
{
	//ThingHandle::AutoHandle();
	switch(pDesk->sGameData.m_byThingDoing)
	{			        				         
	case THING_2SEZI_NT://�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
	case THING_2SEZI_NT_GP://�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
	case THING_TURN_2SEZI_NT://�׾�������2��ɫ�Ӷ�ׯ���¼�		
	case THING_2SEZI_DIR://��2��ɫ���¼������Ʒ����¼�		
	case THING_2SEZI_DIR_GP://��2��ɫ���¼������Ʒ�������Ƶ��¼�
	case THING_2SEZI_GP://��2��ɫ���¼������Ƶ��¼�												
	case THING_2SEZI_JING://��2��ɫ�Ӷ������¼�			
		{//����ɫ�ӽ�����Ϣ
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(!pDesk->sUserData.m_bSiziFinish[i])//�����û�з�������Ϣ
				{
					ReceiveSeziFinish(i,NULL,0,false);
				}
			}
		}
		break;
	case THING_ALL_BUHUA://Ϊ������Ҳ����¼�
		{//ִ�п�ʼ�����¼�
			pDesk->sGameData.ApplyThingID = THING_BEGIN_OUT;
			ApplyAction(pDesk->sGameData.ApplyThingID,200);//������һ���¼�
		}
		break;
	case THING_ONE_BUHUA://������Ҳ����¼�	
	case THING_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�	
	case THING_OUT_PAI://�����¼�
	case THING_CHI_PAI://�����¼�
	case THING_PENG_PAI://�����¼�
	case THING_ZHUA_PAI://ץ���¼�
	case THING_GANG_PAI://�����¼�
		{
			OutputDebugString("�����齫�� AutoHandle �Զ����ƴ��� 00");
			//������ҳ���
			AutoOutOrGet();
		}
		break;
	case THING_HU_PAI://���¼�	
		{
			int nHuCount = 0;
			for (int i=0; i<PLAY_COUNT; ++i)
			{
				if (pDesk->sUserData.m_bIsHu[i])
				{
					nHuCount++;            
				}
			}
			if (nHuCount>=3)
			{        			
				pDesk->sGameData.ApplyThingID = THING_ENG_HANDLE;//��һ���¼�����Ϸ�����¼�
			}
			else
			{// �ֵ���һ�����ץ��
				pDesk->sGameData.ApplyThingID  = THING_ZHUA_PAI;
			}	
			ApplyAction(pDesk->sGameData.ApplyThingID,200);//������һ���¼�
		}
		break;
	case THING_COUNT_FEN://����¼�	
		{
			pDesk->sGameData.ApplyThingID = THING_ENG_HANDLE;
			ApplyAction(pDesk->sGameData.ApplyThingID,200);//������һ���¼�
		}
		break;
	case THING_ENG_HANDLE://���������¼�
	case THING_ENG_UN_NORMAL://���������������¼�
		{
		}
		break;
	case THING_DING_QUE:// ��ȱ
		{
			OutputDebugString("ǿ���йܣ�THING_DING_QUE 000");
            for (int i=0; i<PLAY_COUNT; ++i)
            {
				tagDingQueEx cmdDingQue;
				cmdDingQue.byQue = 255;   // �����
				ReceiveUserDingQue(i, &cmdDingQue, sizeof(cmdDingQue), false);
			}
		}
		break;
	}
}

///�Զ�������ƻ���ץ��
void ThingHandleEx::AutoOutOrGet()
{
	OutputDebugString("ǿ���йܣ�AutoOutOrGet 000");
	bool haveout = false;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(!pDesk->sUserData.IsOutPaiPeople(i))//�������
			continue;
		haveout = true;
		tagOutPaiEx outpai;
		outpai.Clear();
		outpai.byUser = i;
		outpai.byPs = pDesk->sUserData.GetLastHandPai(i);

		//ȱ�ŵ��Ʊ�����
		for(int j=0;j<pDesk->sUserData.GetHandPaiCount(i); ++j)
		{
			if(pDesk->sUserData.m_byArHandPai[i][j]/10 == pDesk->sUserData.m_byDingQue[i])//�ҵ���ȱ�ŵ���
			{
				outpai.byPs = pDesk->sUserData.m_byArHandPai[i][j];
				break;
			}
		}

		OutputDebugString("��ʱ����AutoOutOrGet 111");

		ReceiveUserOutCard(i,&outpai,sizeof(outpai),false);
		break;

	}
	if(!haveout)//û�г��Ƶ���ң�����ץ���¼�
	{
		pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
		ApplyAction(pDesk->sGameData.ApplyThingID,200);//������һ���¼�
	}
}



//////////////////////////////��������//////////////////////////////////////////////////////

///�����¼�
void ThingHandleEx::HandleChiPai(BYTE &nextID,BYTE &time)
{	
	pDesk->sUserData.m_byBuGangPai = 255;//���ܵ÷ֵ��ƣ����ڲ��ܣ�ץ���ܲ��ܵ��ƺ����ϲ��ܲ��з֣������޷֣�
	memset(pDesk->sGameData.m_bGangState,0,sizeof(pDesk->sGameData.m_bGangState));//��¼����״̬��ӵ�ڸܿ��͸ܺ���
	ThingHandle::HandleChiPai(nextID,time);
}

///�����¼�
void ThingHandleEx::HandlePengPai(BYTE &nextID,BYTE &time)	
{
	pDesk->sUserData.m_byBuGangPai = pDesk->sGameData.temp_PengPai.byPs;//���ܵ÷ֵ��ƣ����ڲ��ܣ�ץ���ܲ��ܵ��ƺ����ϲ��ܲ��з֣������޷֣�
    pDesk->sGameData.m_byThingDoing = THING_PENG_PAI;//��¼����ִ�е��¼�

	memset(pDesk->sGameData.m_bGangState,0,sizeof(pDesk->sGameData.m_bGangState));//��¼����״̬��ӵ�ڸܿ��͸ܺ���

    ///�������¼�
    pDesk->sGameData.T_PengPai = pDesk->sGameData.temp_PengPai;//��ȡ����
    pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.temp_PengPai.byUser;
	pDesk->sGameData.T_PengPai.byUser = pDesk->sGameData.temp_PengPai.byUser;

	SetDoingThing( pDesk->sUserData.m_byNowOutStation,THING_PENG_PAI);//����ִ���¼�

    //�������¼�
    GCPStructEx peng;
    peng.Init();

    peng.byType = ACTION_PENG;
    peng.iBeStation = pDesk->sGameData.temp_PengPai.byBePeng;
    peng.iOutpai = pDesk->sGameData.temp_PengPai.byPs;
    peng.iStation = pDesk->sGameData.temp_PengPai.byUser;
    BYTE station = pDesk->sGameData.temp_PengPai.byUser;

	//��¼�¼�
	LogThing( THING_PENG_PAI,peng.iOutpai, peng.iStation,peng.iOutpai,ACTION_PENG);

    //�������ɾ�����һ�ų���
    pDesk->sUserData.DelleteLastOutPai(pDesk->sGameData.T_OutPai.byUser);
    for(int i=0;i<2;i++)
    {	
        pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.temp_PengPai.byUser,pDesk->sGameData.temp_PengPai.byPs);//ɾ������
    }
	//��������������
    for(int i=0;i<3;i++)
    {
        peng.byData[i] = pDesk->sGameData.temp_PengPai.byPs;
    }

    pDesk->sUserData.AddToGCP(pDesk->sGameData.temp_PengPai.byUser,&peng);//�������

    for(int i=0;i<PLAY_COUNT;++i)
    {
        pDesk->sUserData.CopyGCPData(i,pDesk->sGameData.T_PengPai.m_UserGCPData[i]);
        pDesk->sUserData.CopyOutPai(i,pDesk->sGameData.T_PengPai.m_byArOutPai[i]);
    }

    ////�ж��Ƿ�������
    //if(pDesk->sGameData.m_mjAction.bTing)//������
    //{
    //    if(pDesk->sCheckHuPai.CanTing(station,pDesk->sGameData.T_CPGNotify[station].m_byTingCanOut))//����ܷ�����
    //    {
    //        pDesk->sGameData.T_CPGNotify[station].bTing = true;
    //    }
    //}

    pDesk->sGameData.m_byNextAction = ACTION_NO;
    pDesk->sUserData.m_byApplyMaxAction = ACTION_NO;//��ҵ�ǰ���������¼�������������󣬻ָ�����

	//if(pDesk->sUserData.GetAHandPaiCount(station,pDesk->sGameData.temp_PengPai.byPs)>0)////��¼�޷ָܣ����ܣ�������4������������޷֣�ץ�ƺ��ܲ��ܵ��ǲ��ڵ�ǰȦ���޷֣�
	//{
	//	for(int i=0;i<4;++i)
	//	{
	//		if(pDesk->sUserData.m_NoFenGang[station][i] != 255)
	//			continue;
	//		pDesk->sUserData.m_NoFenGang[station][i] = pDesk->sGameData.temp_PengPai.byPs;
	//		break;
	//	}
	//}



}

///���¼�
void ThingHandleEx::HandleGangPai(BYTE &nextID,BYTE &time)	
{
    pDesk->sGameData.m_byThingDoing = THING_GANG_PAI;//��¼����ִ�е��¼�
	nextID = THING_ZHUA_PAI;
	time = 1;
	///�������¼�
	pDesk->sGameData.T_GangPai = pDesk->sGameData.temp_GangPai;//��ȡ����
	pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.temp_GangPai.byUser;

	pDesk->sGameData.m_bGangState[pDesk->sGameData.temp_GangPai.byUser] = true;//��¼����״̬��ӵ�ڸܿ��͸ܺ���

	SetDoingThing( pDesk->sUserData.m_byNowOutStation,THING_GANG_PAI);//����ִ���¼�

	GCPStructEx gang;
	gang.Init();
	gang.iBeStation = pDesk->sGameData.temp_GangPai.byBeGang;
	gang.iOutpai = pDesk->sGameData.temp_GangPai.byPs;
	gang.iStation = pDesk->sGameData.temp_GangPai.byUser;

	gang.byData[0] = pDesk->sGameData.temp_GangPai.byPs;
	gang.byData[1] = pDesk->sGameData.temp_GangPai.byPs;
	gang.byData[2] = pDesk->sGameData.temp_GangPai.byPs;
	gang.byData[3] = pDesk->sGameData.temp_GangPai.byPs;

	pDesk->sUserData.m_bIsGangPao[pDesk->sGameData.temp_GangPai.byUser] = true;//���ڸܺ��ڵ�״̬

	//��¼�¼�
	LogThing( THING_GANG_PAI,gang.iOutpai, gang.iStation,gang.iOutpai,pDesk->sGameData.temp_GangPai.byType);

	memset(pDesk->sGameData.T_GangPai.iGangFen,0,sizeof(pDesk->sGameData.T_GangPai.iGangFen));//����ּܷ�¼
	switch(pDesk->sGameData.temp_GangPai.byType)
	{
	case ACTION_AN_GANG://����
		{
			for(int i=0;i<4;++i)
			{	
				pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.temp_GangPai.byUser,pDesk->sGameData.temp_GangPai.byPs);//ɾ������
			}
			gang.byType = ACTION_AN_GANG;
			////�������(����Ƿ񻹴��ڰ���)
			//CheckAction(pDesk->sGameData.temp_GangPai.byUser,255,1);
			memset(pDesk->sGameData.T_GangPai.iGangFen,0,sizeof(pDesk->sGameData.T_GangPai.iGangFen));

            // ���꣨���ܣ���δ������ 2
            BYTE byGangUser = pDesk->sGameData.temp_GangPai.byUser;
			int count = pDesk->sUserData.m_stGangFen[byGangUser].count;
            for (int i=0; i<PLAY_COUNT; ++i) 
            {
                if (i==byGangUser || pDesk->sUserData.m_bIsHu[i])
                {
                    continue;
                }
                pDesk->sUserData.m_byGFXY[ byGangUser ][i] += 2;
                pDesk->sUserData.m_byGFXY[ byGangUser ][4] += 2;
                //pDesk->sUserData.m_byGFXY[ i ][byGangUser] -= 2;
                //pDesk->sUserData.m_byGFXY[ i ][4] -= 2;
				pDesk->sGameData.T_GangPai.iGangFen[byGangUser] += 2;
				pDesk->sGameData.T_GangPai.iGangFen[i] = -2;

				pDesk->sUserData.m_stGangFen[byGangUser].fen[count][byGangUser] += 2;
				pDesk->sUserData.m_stGangFen[byGangUser].fen[count][i] -= 2;
            }
			pDesk->sUserData.m_stGangFen[byGangUser].fen[count][PLAY_COUNT] = ACTION_AN_GANG;
			pDesk->sUserData.m_stGangFen[byGangUser].count++;

		}
		break;
	case ACTION_MING_GANG://����
		{
			for(int i=0;i<3;++i)
			{	
				pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.temp_GangPai.byUser,pDesk->sGameData.temp_GangPai.byPs);//ɾ������
			}
			gang.byType = ACTION_MING_GANG;
			pDesk->sUserData.DelleteLastOutPai(pDesk->sGameData.temp_GangPai.byBeGang);//�������ɾ�����һ�ų���

            // ���ֱ꣨�ܣ�����������2
            BYTE byGangUser = pDesk->sGameData.temp_GangPai.byUser;
            BYTE byYingGangUser = pDesk->sGameData.temp_GangPai.byBeGang;
            
			memset(pDesk->sGameData.T_GangPai.iGangFen,0,sizeof(pDesk->sGameData.T_GangPai.iGangFen));
			pDesk->sGameData.T_GangPai.iGangFen[byGangUser] = 2;
			pDesk->sGameData.T_GangPai.iGangFen[byYingGangUser] = -2;

            pDesk->sUserData.m_byGFXY[ byGangUser ][byYingGangUser] += 2;//������ҵ�2�֣��������-2��
            pDesk->sUserData.m_byGFXY[ byGangUser ][4] += 2;

			int count = pDesk->sUserData.m_stGangFen[byGangUser].count;
			pDesk->sUserData.m_stGangFen[byGangUser].fen[count][byGangUser] = 2;
			pDesk->sUserData.m_stGangFen[byGangUser].fen[count][byYingGangUser] = -2;


			if(!pDesk->sGameData.m_bChengBao)//�ǳа��ƣ��Ҽ��� ���ܣ�lym 2012-1-3
			{
				for (int i=0; i<PLAY_COUNT; ++i)//��ȡ������Ҹ�1�� lym 2012-1-3
				{
					if (i==byGangUser || pDesk->sUserData.m_bIsHu[i] || i == byYingGangUser)
					{
						continue;
					}
					pDesk->sUserData.m_byGFXY[ byGangUser ][i] += 1;
					pDesk->sUserData.m_byGFXY[ byGangUser ][4]+=1;

					pDesk->sGameData.T_GangPai.iGangFen[byGangUser] += 1;
					pDesk->sGameData.T_GangPai.iGangFen[i] = -1;


					pDesk->sUserData.m_stGangFen[byGangUser].fen[count][byGangUser] += 1;
					pDesk->sUserData.m_stGangFen[byGangUser].fen[count][i] -= 1;
				}	
			}
			pDesk->sUserData.m_stGangFen[byGangUser].fen[count][PLAY_COUNT] = ACTION_MING_GANG;
			pDesk->sUserData.m_stGangFen[byGangUser].count++;
		}
		break;
	case ACTION_BU_GANG://����
        {
            pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.temp_GangPai.byUser,pDesk->sGameData.temp_GangPai.byPs);//ɾ������
            pDesk->sUserData.DelleteAGCPData(pDesk->sGameData.temp_GangPai.byUser,ACTION_PENG,pDesk->sGameData.temp_GangPai.byPs);//ɾ����Ӧ����
            gang.byType = ACTION_BU_GANG;
            // �η磨���ܣ���δ������1�����ϸܲ��ܵ÷� lym 2012-1-3��
            BYTE byGangUser = pDesk->sGameData.temp_GangPai.byUser;
			int count = pDesk->sUserData.m_stGangFen[byGangUser].count;

			char buff[500];
			sprintf(buff,"���������� ���%d ��%d  Ԥ����%d",byGangUser,pDesk->sGameData.temp_GangPai.byPs,pDesk->sUserData.m_byBuGangPai);
			OutputDebugString(buff);

			if(pDesk->sUserData.m_byBuGangPai == pDesk->sGameData.temp_GangPai.byPs)
			{
				memset(pDesk->sGameData.T_GangPai.iGangFen,0,sizeof(pDesk->sGameData.T_GangPai.iGangFen));
				for (int i=0; i<PLAY_COUNT; ++i)
				{
					if (i==byGangUser || pDesk->sUserData.m_bIsHu[i])
					{
						continue;
					}
					pDesk->sUserData.m_byGFXY[ byGangUser ][i] += 1;
					pDesk->sUserData.m_byGFXY[ byGangUser ][4]+=1;
					pDesk->sGameData.T_GangPai.iGangFen[byGangUser] += 1;
					pDesk->sGameData.T_GangPai.iGangFen[i] = -1;

					pDesk->sUserData.m_stGangFen[byGangUser].fen[count][byGangUser] += 1;
					pDesk->sUserData.m_stGangFen[byGangUser].fen[count][i] -= 1;
				}
			}
			pDesk->sUserData.m_stGangFen[byGangUser].fen[count][PLAY_COUNT] = ACTION_BU_GANG;
			pDesk->sUserData.m_stGangFen[byGangUser].count++;
        }
        break;
    }
    pDesk->sUserData.AddToGCP(pDesk->sGameData.temp_GangPai.byUser,&gang);

    for(int i=0;i<4;++i)
    {
        pDesk->sUserData.CopyGCPData(i,pDesk->sGameData.T_GangPai.m_UserGCPData[i]);
        pDesk->sUserData.CopyOutPai(i,pDesk->sGameData.T_GangPai.m_byArOutPai[i]);
    }

    if(pDesk->sGameData.temp_GangPai.byType == ACTION_AN_GANG)
    {
        //CheckAction(pDesk->sGameData.temp_GangPai.byUser,255,0);
        pDesk->sGameData.T_CPGNotify[pDesk->sGameData.temp_GangPai.byUser].bHu = false;
        pDesk->sGameData.T_CPGNotify[pDesk->sGameData.temp_GangPai.byUser].bTing = false;
        pDesk->sGameData.T_CPGNotify[pDesk->sGameData.temp_GangPai.byUser].bKan = false;
        pDesk->sGameData.T_CPGNotify[pDesk->sGameData.temp_GangPai.byUser].bSaoHu = false;
    }
    pDesk->sGameData.m_byNextAction = ACTION_NO;
    ///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
    pDesk->sUserData.m_bZhuaPaiDir = true;
    pDesk->sUserData.m_byApplyMaxAction = ACTION_NO;//��ҵ�ǰ���������¼�������������󣬻ָ�����
	pDesk->sUserData.m_byBuGangPai = 255;//���ܵ÷ֵ��ƣ����ڲ��ܣ�ץ���ܲ��ܵ��ƺ����ϲ��ܲ��з֣������޷֣�

	char strdug[300];
  

}

///���¼�
void ThingHandleEx::HandleTingPai(BYTE &nextID,BYTE &time)
{
	ThingHandle::HandleTingPai(nextID,time);
}

///���¼�
void ThingHandleEx::HandleHuPai(BYTE &nextID,BYTE &time)	
{
    pDesk->sGameData.m_byThingDoing = THING_HU_PAI;//��¼����ִ�е��¼�

    BYTE byHuUser = pDesk->sGameData.T_HuPai.byUser;    // �������
    pDesk->sUserData.m_bIsHu[ byHuUser ] = true;

	SetDoingThing( byHuUser,THING_HU_PAI);//����ִ���¼�
		

	char buff[800];
	sprintf(buff,"��������ץ�� ���ƴ��� %d  ",pDesk->sGameData.T_HuPai.byUser);
	OutputDebugString(buff); 

	//��¼�¼�
	LogThing( THING_HU_PAI,pDesk->sGameData.T_HuPai.byPs, pDesk->sGameData.T_HuPai.byUser,pDesk->sGameData.T_HuPai.byDianPao,pDesk->sGameData.T_HuPai.bDuoXiang );

	memset(pDesk->sGameData.T_CountFen.bCanHu,0,sizeof(pDesk->sGameData.T_CountFen.bCanHu));
	for(int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->sGameData.T_CountFen.bCanHu[i] = pDesk->sGameData.T_HuPai.bCanH[i];
	}

	////�����ھͣ��������������һ���ܵķ� lym 2011-12-16
	//if(CMjRef::CheckHupaiType(HUPAI_TYPE_GANG_PAO, pDesk->sGameData.T_HuPai.byHuType[byHuUser], MAX_HUPAI_TYPE) )   // ������
	//{  
	//	BYTE dian = pDesk->sGameData.T_HuPai.byDianPao;
	//	//��������ҵĸܷ��˻�
	//	for(int i=0;i>=0;i--)
	//	{
	//		if(dian == 255)
	//			break;
	//		if(pDesk->sUserData.m_UserGCPData[dian][i].byType == ACTION_AN_GANG)//���ܶ���
	//		{
	//			for(int j=0;j<PLAY_COUNT;++j)
	//			{
	//				if(j != dian && (!pDesk->sUserData.m_bIsHu[j] || pDesk->sGameData.T_CountFen.bCanHu[j]))
	//				{
	//					pDesk->sUserData.m_byGFXY[dian][j] -= 2;
	//					pDesk->sUserData.m_byGFXY[dian][4] -= 2;
	//				}
	//			}
	//			break;
	//		}
	//		else if(pDesk->sUserData.m_UserGCPData[dian][i].byType == ACTION_BU_GANG)//���ܶ���
	//		{
	//			for(int j=0;j<PLAY_COUNT;++j)
	//			{
	//				if(j != dian && (!pDesk->sUserData.m_bIsHu[j] || pDesk->sGameData.T_CountFen.bCanHu[j]))
	//				{
	//					pDesk->sUserData.m_byGFXY[dian][j] -= 1;
	//					pDesk->sUserData.m_byGFXY[dian][4] -= 1;
	//				}
	//			}
	//			break;
	//		}
	//		else if(pDesk->sUserData.m_UserGCPData[dian][i].byType == ACTION_MING_GANG)//���ܶ������˻�������ҵķ�
	//		{
	//			pDesk->sUserData.m_byGFXY[dian][pDesk->sUserData.m_UserGCPData[dian][i].iBeStation] -= 2;
	//			pDesk->sUserData.m_byGFXY[dian][4] -= 2;
	//			break;
	//		}
	//	}
	//}

	if(pDesk->sGameData.T_HuPai.bDuoXiang )//һ�ڶ���
	{
		
		OutputDebugString("һ�ڶ��죺����");
		if(pDesk->sUserData.m_byTempNt == 255)//��¼�¾ֻ�ׯ���
		{
			pDesk->sUserData.m_byTempNt = pDesk->sGameData.T_CPGNotify[byHuUser].m_byDianPao;
		OutputDebugString("һ�ڶ��죺��¼ׯ��");
		}
		for (int i=0; i<PLAY_COUNT; ++i)
		{
			if(!pDesk->sGameData.T_HuPai.bCanH[i])
				continue;
			pDesk->sUserData.m_bIsHu[ i ] = true;
			pDesk->sGameData.T_HuPai.byUser = i;    // �������
			// ��ֲ���¼
			CountFenAndRecord();
		}
	}
	else
	{
		if(pDesk->sUserData.m_byTempNt == 255)//��¼�¾ֻ�ׯ���
		{
			pDesk->sUserData.m_byTempNt = byHuUser;
		}
		// ��ֲ���¼
		CountFenAndRecord();
	}
    pDesk->sGameData.T_CPGNotify[byHuUser].Clear();

    int nHuCount = 0;
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        if (pDesk->sUserData.m_bIsHu[i])
        {
            nHuCount++;            
        }
    }
    if (nHuCount>=3)
    {        
        // ����¼�
        nextID = THING_ENG_HANDLE;//��һ���¼�����Ϸ�����¼�
        time = 1;	
    }
    else
    {
        // �ֵ���һ�����ץ��
        nextID = THING_ZHUA_PAI;
        time = 1;
    }	

	char strdug[300];
	//��ȡ������ҵķ���
	for (int i=0; i<PLAY_COUNT; ++i)
    {
		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].iGangFen[i] = pDesk->sUserData.m_byGFXY[i][4];
		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].iMoney[i] = pDesk->sGameData.T_CountFen.iZongFen[i];
		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].iZongFen[i] = pDesk->sGameData.T_CountFen.iZongFen[i];
		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].iHuFen[i] = pDesk->sGameData.T_CountFen.iHuFen[i]; 

		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].byGengCount = pDesk->sGameData.T_CountFen.byGengCount;//����
	}
	sprintf(buff,"��������ץ�� ���ƴ��� ��� %d  ",pDesk->sGameData.T_HuPai.byUser);
	OutputDebugString(buff); 

}

void ThingHandleEx::CountFenAndRecord()
{
    BYTE byHuUser = pDesk->sGameData.T_HuPai.byUser;
	pDesk->sGameData.T_CountFen.byUser=byHuUser;

    pDesk->sUserData.m_bIsHu[byHuUser] = true;
    for(int i=0;i<PLAY_COUNT;i++)
    {
        pDesk->sGameData.T_CountFen.bHu[i] = pDesk->sUserData.m_bIsHu[i];
		pDesk->sGameData.T_CountFen.bCanHu[i] = pDesk->sGameData.T_HuPai.bCanH[i];
        //��ȡ��������
        for(int j=0;j<MAX_HUPAI_TYPE;j++)
        {
            pDesk->sGameData.T_CountFen.byHuType[i][j] = pDesk->sGameData.T_HuPai.byHuType[i][j];
        }
    }
    pDesk->sGameData.T_CountFen.bDH = pDesk->sGameData.T_HuPai.bDuoXiang;
    pDesk->sGameData.T_CountFen.bZiMo = pDesk->sGameData.T_HuPai.bZimo;
    pDesk->sGameData.T_CountFen.byUser = pDesk->sGameData.T_HuPai.byUser;	
    pDesk->sGameData.T_CountFen.byDian = pDesk->sGameData.T_HuPai.byDianPao;		
    pDesk->sGameData.T_CountFen.byPs = pDesk->sGameData.T_HuPai.byPs;	
    pDesk->sGameData.T_CountFen.byNt = pDesk->sUserData.m_byNtStation;
    if(pDesk->sGameData.T_CountFen.bZiMo )
    {
        pDesk->sGameData.T_CountFen.byDian = pDesk->sGameData.T_CountFen.byUser;
    }
    ///���
    pDesk->sCountFen.CountFen(pDesk->sGameData.T_HuPai,pDesk->sGameData.T_CountFen);

    memset(pDesk->sGameData.T_CountFen.m_byArHandPai,255,sizeof(pDesk->sGameData.T_CountFen.m_byArHandPai));
    memset(pDesk->sGameData.T_HuPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_HuPai.m_byArHandPai));

    pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_CountFen.m_byArHandPai,pDesk->sGameData.T_HuPai.byUser,true);
    pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_HuPai.m_byArHandPai,pDesk->sGameData.T_HuPai.byUser,true);

    for(int i=0;i<PLAY_COUNT;i++)
    {
        pDesk->sUserData.CopyGCPData(i,pDesk->sGameData.T_CountFen.m_UserGCPData[i]);
        pDesk->sGameData.T_CountFen.iHuShu[i] = pDesk->sGameData.T_CountFen.iZongFen[i];
    }

    ///��¼���
    memcpy(&pDesk->sGameData.T_HuPaiXZMJ[ byHuUser ], &pDesk->sGameData.T_HuPai, sizeof(pDesk->sGameData.T_HuPai));
    memcpy(&pDesk->sGameData.T_CountFenXZMJ[ byHuUser ], &pDesk->sGameData.T_CountFen, sizeof(pDesk->sGameData.T_CountFen));

    pDesk->sUserData.m_byNowOutStation = byHuUser;

	for(int i=0;i<PLAY_COUNT;i++)
    {
		if(!pDesk->sUserData.m_bIsHu[i])//û�к��Ƶ����
			continue;
		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].m_bIsHu[i] = true;
		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].m_bIsZiMo[i] = pDesk->sGameData.T_HuPaiXZMJ[i].bZimo;//�Ƿ�����
		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].m_byDianPao[i] = pDesk->sGameData.T_HuPaiXZMJ[i].byDianPao;//����λ��
		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].m_iZongFan[i] = pDesk->sGameData.T_HuPaiXZMJ[i].m_iZongFan[i];

		pDesk->sGameData.T_HuPaiXZMJ[byHuUser].m_iGengCount[i] = pDesk->sUserData.m_byGengCount[i];//��Ҹ���
	}
}

///���
void ThingHandleEx::HandleCountFen(BYTE &nextID,BYTE &time)
{
	ThingHandle::HandleCountFen(nextID,time);	
}

/////���������¼�
//void ThingHandleEx::HandleEnd(BYTE &nextID,BYTE &time)
//{
//	ThingHandle::HandleEnd(nextID,time);	
//}

/////ǿ�˽����¼�
//void ThingHandleEx::UnnormalEnd(BYTE &nextID,BYTE &time)
//{
//	ThingHandle::UnnormalEnd(nextID,time);	
//}



/// �����ʱ����Ϣ
void ThingHandleEx::OnGameTimer(UINT uTimerID)
{
    switch(uTimerID)
    {
    case 10://�¼���ʱ��id
        {
            pDesk->KillTimer(uTimerID);
            GameCPU(pDesk->sGameData.ApplyThingID);
        }
        break;        
    case TIME_CHECK_GAME_MSG://��Ϣ��ʱ���Զ�����
        {
            //pDesk->KillTimer(uTimerID);
            //AutoHandle();
			ThingHandle::OnGameTimer( uTimerID);
        }
        break;
    case TIME_STEP:
        {
            //OnStateTimer();
        }
        break;
	case TIME_WAIT_MSG: // �����¼�����Ӧ���ǳ�ʱ�Զ�����ϵͳ�Զ�����
		{
			OutTimeHandle();
		}
		break;
    }
}



/**
* @brief ���õ�ǰ״̬
* @param[in]       curState            Ҫ���ó�ʲô״̬
* @param[in]       unLastingTime       ״̬����ʱ��
* @param[in]       unWaitingTime       ״̬��ʼǰ�ĵȴ�
* @param[in]       bNotify             �Ƿ���Ϣ֪ͨ�ͻ���
*/
void ThingHandleEx::SetGameCurState(MJTHINGS curState, BYTE ucCurChair, unsigned int unLastingTime, unsigned int unWaitingTime,
                     bool bNotify)
{
    m_curState.thingDoing = curState;
    m_curState.ucCurChair = ucCurChair;
    m_curState.unStartTime = m_unCurStateTime;
    m_curState.unEndTime = m_unCurStateTime+unLastingTime;

    m_curState.unWaitingTime = unWaitingTime;
    m_curState.bNotify = bNotify;

    if (bNotify && 0==unWaitingTime && THING_MAX!=curState)
    {
        // ֪ͨ�ͻ���
        MJ_State mjstate;        
        mjstate.thingDoing = curState;
        mjstate.ucCurChair = ucCurChair;
        mjstate.ucTime = unLastingTime/1000;
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            pDesk->SendGameDataEx(i, &mjstate, sizeof(mjstate), 0, ASS_SET_STATE, 0);
        }
        m_curState.bNotify = false;
    }
}

void ThingHandleEx::StartStateTimer()
{
    pDesk->SetTimer(TIME_STEP, TIME_STEP_TIME);
    m_unCurStateTime = 0;
    m_curState.thingDoing = THING_MAX;
}
void ThingHandleEx::EndStateTimer()
{
    pDesk->KillTimer(TIME_STEP);
    m_unCurStateTime = 0;
    m_curState.thingDoing = THING_MAX;
}


void ThingHandleEx::OnStateTimer()
{
	OutputDebugString("��ʱ����OnStateTimer");
    m_unCurStateTime += TIME_STEP_TIME;
    if (m_curState.unWaitingTime<TIME_STEP_TIME)
    {
        m_curState.unWaitingTime = 0;
    }
    else
    {
        m_curState.unWaitingTime -= TIME_STEP_TIME;
    }

    if (THING_MAX==m_curState.thingDoing)
    {
        return;
    }
	OutputDebugString("��ʱ����OnStateTimer 111");
    if (0==m_curState.unWaitingTime && m_curState.bNotify)
    {    
        // ֪ͨ�ͻ���
        MJ_State mjstate;        
        mjstate.thingDoing = m_curState.thingDoing;
        mjstate.ucCurChair = m_curState.ucCurChair;
        mjstate.ucTime = (m_curState.unEndTime-m_curState.unStartTime-TIME_NETWORK)/1000;
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            pDesk->SendGameDataEx(i, &mjstate, sizeof(mjstate), 0, ASS_SET_STATE, 0);
        }
        m_curState.bNotify = false;

		OutputDebugString("��ʱ����OnStateTimer 222");
    }

	if(pDesk->sUserData.m_bFoceLeave[m_curState.ucCurChair] && m_unCurStateTime>3000)//��Ҵ��ڶ���״̬
	{
		OutputDebugString("��ʱ����OnStateTimer 4444");
		NetCutAutoHandle();
	}

    if (m_unCurStateTime >= m_curState.unEndTime && m_curState.thingDoing==m_curState.thingDoing)
    {
		OutputDebugString("��ʱ����OnStateTimer 333");
        AutoHandle();
    }
}

//�¼���¼
void ThingHandleEx::LogThing(BYTE thingId,BYTE pai,BYTE st1,BYTE st2,BYTE data)
{//�¼���¼ֻ��¼��10���¼���0�¼��ţ�1�����ֵ��2���λ��1,3���λ��2��4��������
	if(pDesk->sGameData.m_byThingLog[9][0] != 255)//����
	{
		for(int i=0;i<9;++i)//����һ��
		{
			pDesk->sGameData.m_byThingLog[i][0] = pDesk->sGameData.m_byThingLog[i+1][0];
			pDesk->sGameData.m_byThingLog[i][1] = pDesk->sGameData.m_byThingLog[i+1][1];
			pDesk->sGameData.m_byThingLog[i][2] = pDesk->sGameData.m_byThingLog[i+1][2];
			pDesk->sGameData.m_byThingLog[i][3] = pDesk->sGameData.m_byThingLog[i+1][3];
			pDesk->sGameData.m_byThingLog[i][4] = pDesk->sGameData.m_byThingLog[i+1][4];
		}
		pDesk->sGameData.m_byThingLog[9][0] = thingId;
		pDesk->sGameData.m_byThingLog[9][1] = pai;
		pDesk->sGameData.m_byThingLog[9][2] = st1;
		pDesk->sGameData.m_byThingLog[9][3] = st2;
		pDesk->sGameData.m_byThingLog[9][4] = data;
	}
	else
	{
		for(int i=0;i<10;++i)
		{
			if(pDesk->sGameData.m_byThingLog[i][0] == 255)
			{
				pDesk->sGameData.m_byThingLog[i][0] = thingId;
				pDesk->sGameData.m_byThingLog[i][1] = pai;
				pDesk->sGameData.m_byThingLog[i][2] = st1;
				pDesk->sGameData.m_byThingLog[i][3] = st2;
				pDesk->sGameData.m_byThingLog[i][4] = data;
				break;
			}
		}
	}
	
}


/// ���õ�ǰ���ڷ������¼�
void ThingHandleEx::SetDoingThing(BYTE station,BYTE thing)
{

	pDesk->sGameData.m_byThingDoing = thing;
	/// �ָ���ʱ����
	ReSetOutTime();
	switch(thing)
	{		
	case THING_DING_QUE://��ȱ�¼�		
		{
			SetOutTime(station,pDesk->sGameData.m_MjSet.ucDingQueTime,thing,true);
		}
		break;
	case THING_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�		
		{
			SetOutTime(station,pDesk->sGameData.m_MjSet.ucOutcardTime,thing,true);
		}
		break;
	case THING_OUT_PAI://�����¼�
		{
			//if(pDesk->sUserData.m_bTuoGuan[station] || pDesk->sUserData.m_bTing[station])
			//{
			//	SetOutTime(station,pDesk->sGameData.m_MjSet.byAutoTime,thing,true);
			//	break;
			//}
			SetOutTime(station,pDesk->sGameData.m_MjSet.ucBlockTime,thing,true);
		}
		break;
	case THING_ZHUA_PAI://ץ���¼�
		{
			if(pDesk->sUserData.m_bTuoGuan[station] || pDesk->sUserData.m_bTing[station] || pDesk->sUserData.m_bFoceLeave[station])
			{
				SetOutTime(station,pDesk->sGameData.m_MjSet.nAutoTime,thing,true);
				break;
			}
			SetOutTime(station,pDesk->sGameData.m_MjSet.ucOutcardTime + pDesk->sGameData.m_MjSet.ucBlockTime,thing,true);
		}
		break;
	case THING_CHI_PAI://�����¼�
	case THING_PENG_PAI://�����¼�
		{
			if(pDesk->sUserData.m_bTuoGuan[station] || pDesk->sUserData.m_bTing[station])
			{
				SetOutTime(station,pDesk->sGameData.m_MjSet.nAutoTime,thing,true);
				break;
			}
			SetOutTime(station,pDesk->sGameData.m_MjSet.ucOutcardTime,thing,true);
		}
		break;
	case THING_HU_PAI:	//���¼�	
		{
			SetOutTime(station,pDesk->sGameData.m_MjSet.nAutoTime,thing,true);
		}
		break;
	case THING_BEGIN:	//��Ϸ��ʼ�¼�	
	case THING_DONG_NT:	//�׾��Զ�Ϊׯ�¼�					         
	case THING_2SEZI_NT:	//�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
	case THING_2SEZI_NT_GP:	//�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
	case THING_TURN_2SEZI_NT://�׾�������2��ɫ�Ӷ�ׯ���¼�		
	case THING_2SEZI_DIR://��2��ɫ���¼������Ʒ����¼�		
	case THING_2SEZI_DIR_GP://��2��ɫ���¼������Ʒ�������Ƶ��¼�
	case THING_2SEZI_GP://��2��ɫ���¼������Ƶ��¼�		
	case THING_2SEZI_NT_DIR_GP:	//��2��ɫ���¼������Ƶ��¼�	
	case THING_SEND_PAI://�����¼�							
	case THING_TIAO_PAI://�����¼�							
	case THING_2SEZI_JING://��2��ɫ�Ӷ������¼�			
	case THING_1SEZI_JIN://��1��ɫ�Ӷ������¼�
	case THING_ALL_BUHUA://Ϊ������Ҳ����¼�				
	case THING_ONE_BUHUA://������Ҳ����¼�	
	case THING_GANG_PAI://�����¼�
	case THING_COUNT_FEN://����¼�	
		{

		}
		break;
	}
	
}


