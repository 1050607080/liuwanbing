#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"

BYTE CServerGameDesk::m_iThinkTime;
BYTE CServerGameDesk::m_iBeginTime;
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{							
	m_bGameStation=GS_WAIT_ARGEE;
	bLastFistOutPeople=255;							//��һ�ֵ�һ��������
	ReSetGameState(GS_WAIT_ARGEE);
}
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");
	return true;
}
//������ע
BOOL CServerGameDesk::LoadExtIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");
	m_iThinkTime = f.GetKeyVal(key,"ThinkTime",15);
	if(m_iThinkTime<10)
	{
		m_iThinkTime=10;
	}
	m_iBeginTime = f.GetKeyVal(key,"BeginTime",30);
	if(m_iBeginTime<10)
	{
		m_iBeginTime=10;
	}
	return TRUE;
}
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	TCHAR szKey[20];
	wsprintf(szKey,"%s_%d",SKIN_FOLDER,iRoomID);

	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = szKey;
	return TRUE;
}


//-------------------------------------------------------------------------------------------------------------------------------------------


//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//��ʼ��
bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadIni();
	//����ͨ����ע
	LoadExtIni();
	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
///�������м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_SEND_CARD);			//���ƶ�ʱ��
	KillTimer(TIME_FINISH);
	KillTimer(TIMER_SCORE);
}
//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIME_SEND_CARD:
		{
			KillTimer(TIME_SEND_CARD);
			//��Ϸ��ʼ
			StartGame();
			return true;
		}
	case TIME_FINISH:
		{
			KillAllTimer();
			GameFinish(0,GF_NORMAL);
			return true;
		}
	case TIMER_SCORE://�з�
		{
			m_iClockLabel--;
			if(m_iClockLabel<=0)
			{
				//OutputDebugString("next::������ʱ�ӵ���");
				KillTimer(TIMER_SCORE);
				AutoOutCard(m_bNextStation,false);
			}
		}
	}
	return __super::OnTimer(uTimerID);
}

//�����Ϣ������
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	CString aa;
	aa.Format("lw::test: Server message=%d", pNetHead->bAssistantID);
	OutputDebugString(aa);
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{
			
			/*char cKey[10];
			CString sPath=CBcfFile::GetAppPath();
			int iResult = 1;
			if (CBcfFile::IsFileExist(sPath + "SpecialRule.bcf"))
			{
			CBcfFile fsr( sPath + "SpecialRule.bcf");
			sprintf(cKey, "%d", NAME_ID);
			iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
			}

			if (iResult == 0)
			{
			return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser); 
			}*/
			//��ʼ�й�
			//Auto(bDeskStation);
			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
		
		case CLIENTTOSERVER_AGREE:		//�û�ͬ����Ϸ
			{
				if (bWatchUser)
				{
					return TRUE;
				}
				if(GS_WAIT_ARGEE!=m_bGameStation && GS_WAIT_NEXT!=m_bGameStation)
				{
					return true;
				}
				bUserPrepare[bDeskStation]=1;
				//ͳ��׼�������
				int iTemp=0;
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(bUserPrepare[i])
					{
						iTemp++;
					}
				}
				GameServerToClient_ArgeeGame serverToClient_agree;
				serverToClient_agree.bDeskStation=bDeskStation;
				serverToClient_agree.bAgreeGame=1;
				for (int i = 0; i < PLAY_COUNT; i++)
				{
					if(m_pUserInfo[i])
					{
						SendGameData(i,&serverToClient_agree,sizeof(serverToClient_agree),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AGREE,0);
					}	
				}
				SendWatchData(PLAY_COUNT,&serverToClient_agree,sizeof(serverToClient_agree),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AGREE,0);
				if(3==iTemp)//��Ϸ��ʼ
				{
					for(int i=0;i<PLAY_COUNT;i++)
					{
						if(NULL == m_pUserInfo[i])//��Ҳ����ھ�
						{
							continue;
						}
						m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
					}
					bIsCanStart=true;
					GameBegin(ALL_ARGEE);
				}
			}
		case CLIENTTOSERVER_BUTTON://�����û���Ϣ
			{
				OnButtonMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
				return true;
			}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_NEXT:
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation TGameStation;
			TGameStation.iBeginTime=m_iBeginTime;
			TGameStation.iThinkTime=m_iThinkTime;
			TGameStation.iGameStation=m_bGameStation;					//��Ϸ״̬
			memcpy(TGameStation.bUserReady,bUserPrepare,sizeof(TGameStation.bUserReady));
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return TRUE;
		}
	case GS_SEND:		//����״̬
	case GS_PLAY_GAME:
		{
			GameStation_0 TGameStation;
			TGameStation.iThinkTime=m_iThinkTime;						//˼��ʱ��
			TGameStation.iBeginTime=m_iBeginTime;						//��ʼ�ȴ�ʱ��
			TGameStation.iGameStation=m_bGameStation;					//��Ϸ״̬
			memcpy(TGameStation.bCardList,m_bPeopleHandleCard[bDeskStation],sizeof(BYTE)*CARD_NUMS);		//���е���	
			memcpy(TGameStation.bLeftCard,m_bLeftCard,sizeof(m_bLeftCard));//ҪѺ����
			if(m_iClockLabel>0)
			{
				TGameStation.iClockLabel=m_iClockLabel;					//ʱ�ӱ�ǩ
			}
			else
			{
				TGameStation.iClockLabel=-1;
			}
			if(m_Logic.TestData(m_bNextStation,0))
			{
				TGameStation.bNextPeople=m_bNextStation;					//��ǰ�з���
			}
			else
			{
				TGameStation.bNextPeople=255;
			}
			
			//������з���
			memcpy(TGameStation.bHavePoint,m_bPeoplePoint,sizeof(TGameStation.bHavePoint));
			//��ҷ���
			memcpy(TGameStation.iPoint,m_iPoint,sizeof(TGameStation.iPoint));
			//�����з���
			memcpy(TGameStation.bRoundHaveCard,bHavePointCard,sizeof(TGameStation.bRoundHaveCard));
			//��ұ���
			memcpy(TGameStation.DeskBehave,m_DeskBehave,sizeof(m_DeskBehave));
			//ÿ������˿�����
			memcpy(TGameStation.iPeopleHandCount,m_iPeopleHandCount,sizeof(TGameStation.iPeopleHandCount));
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return TRUE;
		}
	}
	return false;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	memset(bUserPrepare,0,sizeof(bUserPrepare));			//���׼��
	bIsStart=false;											//��һ�ֳ����Ƿ�ʼ				
	
	bIsCanStart=false;
	
	bOutBigPeople=255;
					
	memset(m_iPoint,0,sizeof(m_iPoint));//��ҷ���	
	memset(bPeopleStation,LW_NO,sizeof(bPeopleStation));
	memset(m_iPeopleHandCount,0,sizeof(m_iPeopleHandCount));		//��������˿�	
	m_iIsMatch=0;												//�Ƿ�����
	memset(m_bMatchCard,0,sizeof(m_bMatchCard));				//�������
	m_bMatchPeople=255;
	memset(bHavePointCard,0,sizeof(bHavePointCard));			//�����з���
	memset(bDeskCard,0,sizeof(bDeskCard));						//�����û��˿�
	bNtPeople=255;												//ׯ��
	bEndPress=false;											//���˳����ƣ�������������Ƿ�������ѹס
	memset(m_bPeopleHandleCard,0,sizeof(m_bPeopleHandleCard));				//��������˿�
	memset(m_IsCut,0,sizeof(m_IsCut));									//�Ƿ����					
	memset(m_iAddNoOPerate,0,sizeof(m_iAddNoOPerate));			//�޲����Ĵ������ﵽ�����й�								
	memset(m_IsAuto,0,sizeof(m_IsAuto));						//�Ƿ��й�
	memset(m_bPeoplePoint,0,sizeof(m_bPeoplePoint));
	memset(m_DeskBehave,0,sizeof(m_DeskBehave));
	m_bNextStation=255;
	m_vFinishSort.clear();
	return TRUE;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		GameFinish(0,GF_SALE);
		return false;
	}
	if(!bIsCanStart)
	{
		return false;
	}
	//ϴ��
	m_Logic.RandCard(bDeskCard,52,false);

	//�ַ��˿�
	int index=0;
	int iPeople=0;
	int iCard=0;
	for(int index=0;index<52;index++)
	{
		if(0x12==bDeskCard[index])
		{
			continue;
		}
		if(PLAY_COUNT==iPeople)
		{
			iPeople=0;
			iCard++;
		}
		m_bPeopleHandleCard[iPeople][iCard]=bDeskCard[index];
		iPeople++;
	}
	
	//ȷ����һ��������
	bNtPeople=GetFirstOutCard(); 
	//����3���ȳ���
	m_bPeopleHandleCard[bNtPeople][17]=0x12;
	
	SendCard();
	return true;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	KillAllTimer();
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			GameServerToClient_Finish serverToClient_normalFinish;

			BYTE bLastStation=m_vFinishSort.at(PLAY_COUNT-1);
			BYTE bRemainCard[CARD_NUMS]={0};
			//������һ���Ƿ���ʣ����
			int iTemp=0;
			if(0!=m_iPeopleHandCount[bLastStation])
			{
				for(int i=0;i<CARD_NUMS;i++)
				{
					if(m_Logic.TestData(m_bPeopleHandleCard[bLastStation][i],1))
					{
						bRemainCard[iTemp]=m_bPeopleHandleCard[bLastStation][i];
						iTemp++;
					}
				}
			}
			__int64 i64Temp=0;
			i64Temp=m_Logic.Get510kPoint(bRemainCard,iTemp);
			//���һ��Ҫ��ʣ���Ƶķֺ�20��
			m_iPoint[bLastStation]+=-i64Temp-20;
			//��һ����20��
			m_iPoint[m_vFinishSort.at(0)]+=20;

			//д�����ݿ�				
			__int64 temp_point[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			CopyMemory(temp_point,m_iPoint,sizeof(temp_point));

			ChangeUserPointint64(temp_point, temp_cut);
			
			__super::RecoderGameInfo(serverToClient_normalFinish.i64Point);

			Ranking(serverToClient_normalFinish.iRanking,PLAY_COUNT,m_iPoint,PLAY_COUNT);

			//��������
			for (int i=0; i < PLAY_COUNT; i++) 
			{
				if (m_pUserInfo[i]!=NULL) 
					SendGameData(i,&serverToClient_normalFinish,sizeof(serverToClient_normalFinish),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_NORMALFINISH,0);
			}
			SendWatchData(m_bMaxPeople,&serverToClient_normalFinish,sizeof(serverToClient_normalFinish),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_NORMALFINISH,0);
			//�������� 
			m_bGameStation=GS_WAIT_NEXT;
			ReSetGameState(bCloseFlag);
			CleanUpCut();
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
	case GFF_SAFE_FINISH:
		{
			GameServerToClient_CutEnd CutEnd;
			//��׿��Ϸ�Ѿ�����������Թ����
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_CUTEND,0);
			m_bGameStation=GS_WAIT_NEXT;
			bCloseFlag = GFF_SAFE_FINISH;
			CleanUpCut();
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;

		}
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			//��������
			m_bGameStation=GS_WAIT_ARGEE;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	}

	//��������
	ReSetGameState(bCloseFlag);
	__super::GameFinish(bDeskStation,bCloseFlag);

	return true;
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bGameStation>GS_WAIT_ARGEE && m_bGameStation<GS_WAIT_NEXT)
	{
		//����
		m_IsCut[bDeskStation]=true;
		if(PLAY_COUNT==GetCutNum())
		{
			GameFinish(bDeskStation,GF_SALE);
		}
		return true;
	}
	else
	{
		bUserPrepare[bDeskStation]=0;
		return false;
	}
	return __super::IsPlayGame(bDeskStation);
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}
//�û�����
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	//����
	m_IsCut[bDeskStation]=true;
	if(PLAY_COUNT==GetCutNum())
	{
		GameFinish(bDeskStation,GF_SALE);
	}
	return __super:: UserNetCut(bDeskStation, pLostUserInfo);
}

//�û���������
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{
	if(m_IsCut[bDeskStation])
	{
		m_IsCut[bDeskStation]=false;
	}
	return __super:: UserReCome(bDeskStation, pNewUserInfo);
}

/*
@brief:��ť��Ϣ������
*/
void CServerGameDesk::OnButtonMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if(uSize!=sizeof(GameClientToServer_Button))
	{
		return;
	}
	GameClientToServer_Button *ButtonMessage=(GameClientToServer_Button *)pData;
	if(NULL==ButtonMessage)
	{
		return;
	}
	if(CLICK_OUTCARD==ButtonMessage->iBtType)//����
	{
		if(GS_PLAY_GAME!=m_bGameStation)
		{
			return ;
		}
		KillTimer(TIMER_SCORE);
		bPeopleStation[bDeskStation]=LW_PASS;

		int iTemp=0;
		BYTE bTempList[CARD_NUMS]={0};
		memset(m_bLeftCard,0,sizeof(m_bLeftCard));
		//������ұ���
		memset(m_DeskBehave[bDeskStation],0,sizeof(m_DeskBehave[bDeskStation]));
		for(int i=0;i<ButtonMessage->iCardCount;i++)
		{
			if(m_Logic.TestData(ButtonMessage->bCardList[i],1))
			{
				m_DeskBehave[bDeskStation][iTemp]=m_bLeftCard[iTemp]=bTempList[iTemp]=ButtonMessage->bCardList[i];
				iTemp++;
			}
		}
		int iHandleCount=m_Logic.CountUserCard(m_bPeopleHandleCard[bDeskStation],CARD_NUMS);
		//ɾ���˿�
		m_Logic.RemoveCard(bTempList,iTemp,m_bPeopleHandleCard[bDeskStation],iHandleCount);
		
		//���õ����з���
		m_Logic.Set510KPoint(bTempList,iTemp,bHavePointCard,12);	

		//��һ���˳����ƣ������˶��г���Ȩ
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(bDeskStation==i)
			{
				continue;
			}
			bPeopleStation[i]=LW_NO;
		}

		int iEnd=GetOutputEndCard();
		if(1==iEnd)
		{
			bEndPress=true;
		}
		m_iPeopleHandCount[bDeskStation]-=ButtonMessage->iCardCount;//��������˿�
		if(0==m_iPeopleHandCount[bDeskStation])
		{
			m_vFinishSort.push_back(bDeskStation);
		}
		bOutBigPeople=bDeskStation;
		if(!ButtonMessage->bFlag)
		{
			m_iAddNoOPerate[bDeskStation]++;
		}
		else
		{
			m_iAddNoOPerate[bDeskStation]=0;
			if(m_IsAuto[bDeskStation])
			{
				//ȡ���й�
				CancelAuto(bDeskStation);
			}
		}
		if(2==m_iAddNoOPerate[bDeskStation])
		{
			if(!m_IsAuto[bDeskStation])
			{
				//��ʼ�й�
				Auto(bDeskStation);
			}
		}
		if(CheckFinish(m_iPeopleHandCount,PLAY_COUNT))
		{
			//��Ϸ����
			//�����շ�
			CollectPoint(bOutBigPeople);

			SetTimer(TIME_FINISH,2.5*1000);
			//�ͷ�����ʾ�÷ֺ�̯���ʣ�����
			InformShowStyle();
			//���һ��̯��
			InformShowCard();
			m_bNextStation=255;
		}
		else
		{
			//��һ��������
			m_bNextStation=GetNextPeople(bDeskStation);
		}

		GameServerToClient_NextPeople serverToClient_next;
		
		memcpy(serverToClient_next.bCardList,m_bLeftCard,sizeof(serverToClient_next.bCardList));
		memcpy(serverToClient_next.bHaveCard,bHavePointCard,sizeof(serverToClient_next.bHaveCard));
		serverToClient_next.bNextStation=m_bNextStation;
		serverToClient_next.bLeftStation=bDeskStation;
		serverToClient_next.bLeftFlag=true;
		memcpy(serverToClient_next.i64PeoplePoint,m_iPoint,sizeof(serverToClient_next.i64PeoplePoint));
		memcpy(serverToClient_next.iPeopleCount,m_iPeopleHandCount,sizeof(serverToClient_next.iPeopleCount));
		//�������ͷ��˷�������                                          
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if(m_pUserInfo[i])
			{
				SendGameData(i,&serverToClient_next,sizeof(serverToClient_next),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_NEXT,0);
			}
		}
		SendWatchData(PLAY_COUNT,&serverToClient_next,sizeof(serverToClient_next),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_NEXT,0);

		//�ж��Ƿ����
		if(m_IsCut[m_bNextStation])
		{
			GameServerToClient_Auto serverToClient_Auto;
			serverToClient_Auto.bDeskStation=m_bNextStation;
			serverToClient_Auto.bIsAuto=true;
			//�������û������й���Ϣ
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&serverToClient_Auto,sizeof(serverToClient_Auto),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AUTO,0);
				}	
			}
			SendWatchData(PLAY_COUNT,&serverToClient_Auto,sizeof(serverToClient_Auto),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AUTO,0);
			//�Զ�����
			AutoOutCard(m_bNextStation,false);
		}
		m_iClockLabel=m_iThinkTime+3;
		SetTimer(TIMER_SCORE,1*1000);
	}
	else if(CLICK_NOOUT==ButtonMessage->iBtType)//����
	{
		if(GS_PLAY_GAME!=m_bGameStation)
		{
			return ;
		}
		KillTimer(TIMER_SCORE);
		bPeopleStation[bDeskStation]=LW_NOOUT;

		GameServerToClient_NextPeople serverToClient_next;
		bool bFinishFlag=false;
		//������ұ���
		memset(m_DeskBehave[bDeskStation],0,sizeof(m_DeskBehave[bDeskStation]));
		//����Ƿ���Խ���
		if(CheckFinish(m_iPeopleHandCount,PLAY_COUNT))
		{
			
			//��Ϸ����
			//�����շ�
			CollectPoint(bOutBigPeople);

			SetTimer(TIME_FINISH,2.5*1000);
			//�ͷ�����ʾ�÷ֺ�̯���ʣ�����
			InformShowStyle();
			//���һ��̯��
			InformShowCard();
			m_bNextStation=255;
			bFinishFlag=true;
		}
		else
		{
			m_bNextStation=GetNextPeople(bDeskStation);
		}
		if(!bFinishFlag)//û������Ҫ���
		{
			//����Ƿ���Կ�ʼ�µ�һ��
			if(CheckRound())
			{
				//�����շ�
				CollectPoint(bOutBigPeople);
				//�ͷ�����ʾ�÷�
				InformShowStyle();
				InitRound();
			}
			
		}
		
		if(!ButtonMessage->bFlag)
		{
			m_iAddNoOPerate[bDeskStation]++;
		}
		else
		{
			m_iAddNoOPerate[bDeskStation]=0;
			if(m_IsAuto[bDeskStation])
			{
				//ȡ���й�
				CancelAuto(bDeskStation);
			}

		}
		if(2==m_iAddNoOPerate[bDeskStation])
		{
			if(!m_IsAuto[bDeskStation])
			{
				//��ʼ�й�
				Auto(bDeskStation);
			}
		}
		
		//��һ��������
		serverToClient_next.bNextStation=m_bNextStation;
		serverToClient_next.bLeftFlag=false;
		serverToClient_next.bLeftStation=bDeskStation;
		memcpy(serverToClient_next.iPeopleCount,m_iPeopleHandCount,sizeof(serverToClient_next.iPeopleCount));
		memcpy(serverToClient_next.bHaveCard,bHavePointCard,sizeof(serverToClient_next.bHaveCard));
		memcpy(serverToClient_next.i64PeoplePoint,m_iPoint,sizeof(serverToClient_next.i64PeoplePoint));
		memcpy(serverToClient_next.bCardList,m_bLeftCard,sizeof(serverToClient_next.bCardList));
		//�������ͷ��˷�������                                          
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if(m_pUserInfo[i])
			{
				SendGameData(i,&serverToClient_next,sizeof(serverToClient_next),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_NEXT,0);
			}
		}
		SendWatchData(PLAY_COUNT,&serverToClient_next,sizeof(serverToClient_next),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_NEXT,0);

		//�ж��Ƿ����
		if(m_IsCut[m_bNextStation])
		{
			GameServerToClient_Auto serverToClient_Auto;
			serverToClient_Auto.bDeskStation=m_bNextStation;
			serverToClient_Auto.bIsAuto=true;
			//�������û������й���Ϣ
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&serverToClient_Auto,sizeof(serverToClient_Auto),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AUTO,0);
				}	
			}
			SendWatchData(PLAY_COUNT,&serverToClient_Auto,sizeof(serverToClient_Auto),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AUTO,0);
			//�Զ�����
			AutoOutCard(m_bNextStation,false);
		}
		m_iClockLabel=m_iThinkTime+3;
		SetTimer(TIMER_SCORE,1*1000);
	}
	else if(CLICK_AUTO==ButtonMessage->iBtType)
	{
		switch(ButtonMessage->iBtParam)
		{
		case 0://�йܣ�ȡ���й�
			{
				if(ButtonMessage->bFlag)//�й�
				{
					if(m_IsAuto[ButtonMessage->bDeskStation])//�Ѿ��й�
					{
						return;
					}
					else
					{
						//�ɹ��й�
						Auto(ButtonMessage->bDeskStation);
						if(bDeskStation==m_bNextStation)
						{
							//�Զ�����
							AutoOutCard(m_bNextStation,false);
						}
					}
				}
				else//ȡ���й�
				{
					if(!m_IsAuto[ButtonMessage->bDeskStation])//���������й�״̬
					{
						return;
					}
					else
					{
						m_iAddNoOPerate[bDeskStation]=0;
						//�ɹ�ȡ���й�
						 CancelAuto(ButtonMessage->bDeskStation);
					}
				}
				break;
			}
		}
	}
}
//��ʼ����
void CServerGameDesk::SendCard()
{
	m_bGameStation=GS_SEND;
	
	GameServerToClient_SendCard serverToClient_Send;
	serverToClient_Send.bNtPeople=bNtPeople;
	int iStep=0;
	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		memcpy(serverToClient_Send.iUserCard[i],m_bPeopleHandleCard[i],sizeof(serverToClient_Send.iUserCard[i]));
		if(bNtPeople==i)
		{
			serverToClient_Send.iUserCardCount[i]=CARD_NUMS;
			m_iPeopleHandCount[i]=CARD_NUMS;//�û��˿�����
		}
		else
		{
			serverToClient_Send.iUserCardCount[i]=CARD_NUMS-1;
			m_iPeopleHandCount[i]=CARD_NUMS-1;//�û��˿�����
		}
	}
	if(IsMatch())
	{
		memset(serverToClient_Send.iUserCard[m_bMatchPeople],0,sizeof(serverToClient_Send.iUserCard[m_bMatchPeople]));
		memcpy(serverToClient_Send.iUserCard[m_bMatchPeople],m_bMatchCard,sizeof(serverToClient_Send.iUserCard[m_bMatchPeople]));							  //�û��˿�
	}
	//����������                                          
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&serverToClient_Send,sizeof(serverToClient_Send),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_SEND,0);
		}	
	}
	SendWatchData(PLAY_COUNT,&serverToClient_Send,sizeof(serverToClient_Send),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_SEND,0);
	//�����ȴ��ͷ��˷�����ɶ�ʱ��
	SetTimer(TIME_SEND_CARD,5*1000);
}
//��ʼ��Ϸ
void CServerGameDesk::StartGame()
{
	m_bGameStation=GS_PLAY_GAME;
	InitRound();
	GameServerToClient_StartGame serverToClient_startGame;
	
	serverToClient_startGame.iOutDeskStation=bNtPeople;
	//������Ϸ��ʼ��Ϣ
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&serverToClient_startGame,sizeof(serverToClient_startGame),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_START,0);
		}	
	}
	SendWatchData(PLAY_COUNT,&serverToClient_startGame,sizeof(serverToClient_startGame),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_START,0);
}
//�õ���һ��������
BYTE CServerGameDesk::GetFirstOutCard()
{
	srand(time(0));
	int iRand=rand()%PLAY_COUNT;
	return iRand;
}

//�й�
void CServerGameDesk::Auto(BYTE bDestStation)
{
	m_IsAuto[bDestStation]=true;
	GameServerToClient_Auto serverToClient_Auto;
	serverToClient_Auto.bDeskStation=bDestStation;
	serverToClient_Auto.bIsAuto=true;
	//�������û������й���Ϣ
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&serverToClient_Auto,sizeof(serverToClient_Auto),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AUTO,0);
		}	
	}
	SendWatchData(PLAY_COUNT,&serverToClient_Auto,sizeof(serverToClient_Auto),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AUTO,0);
}
//ȡ���й�
void CServerGameDesk::CancelAuto(BYTE bDestStation)
{
	m_IsAuto[bDestStation]=false;
	GameServerToClient_Auto serverToClient_Auto;
	serverToClient_Auto.bDeskStation=bDestStation;
	serverToClient_Auto.bIsAuto=false;
	//�������û������й���Ϣ
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&serverToClient_Auto,sizeof(serverToClient_Auto),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AUTO,0);
		}	
	}
	SendWatchData(PLAY_COUNT,&serverToClient_Auto,sizeof(serverToClient_Auto),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_AUTO,0);
}
//ͳ�ƶ�������
int CServerGameDesk::GetCutNum()
{
	int iTemp=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_IsCut[i])
		{
			iTemp++;
		}
	}
	return iTemp;
}
//ͳ���й�����
int CServerGameDesk::GetAutoNum()
{
	int iTemp=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_IsAuto[i])
		{
			iTemp++;
		}
	}
	return iTemp;
}
//��ʼ����һ�ֳ�������
void CServerGameDesk::InitRound()
{
	//��ʼ����ʼ����
	bIsStart=true;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		bPeopleStation[i]=LW_NO;
	}
	memset(m_bLeftCard,0,sizeof(m_bLeftCard));
	memset(bHavePointCard,0,sizeof(bHavePointCard));			//�����з���
	bOutBigPeople=255;
	memset(m_bLeftCard,0,sizeof(m_bLeftCard));
}
//����Ƿ���Կ�ʼ�µ�һ��
bool CServerGameDesk::CheckRound()
{
	int iTemp=0;
	int k=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(0==m_iPeopleHandCount[i])
		{
			k++;
		}
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(LW_NOOUT==bPeopleStation[i])
		{
			iTemp++;
		}
	}
	if(bEndPress && 1==iTemp)
	{
		return true;
	}
	if(2==iTemp)//��Ӵ�������̵����ڿ����ڸ�һ����
	{
		return true;
	}
	else
	{
		return false;
	}
}
//�õ���һ���������
BYTE CServerGameDesk::GetNextPeople(BYTE bCurrentPeople)
{
	int k=bCurrentPeople;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		k--;
		if(k<0)
		{
			k=2;
		}
		if(m_iPeopleHandCount[k]<=0)
		{
			continue;
		}
		else
		{
			return k;
		}
	}
	
}
//����Ƿ���Խ���
bool CServerGameDesk::CheckFinish(int bPeopleCount[],int iCount)
{
	
	//���������������˻��������˳����ơ�
	int iTemp1=0;
	for(int i=0;i<iCount;i++)
	{
		if(0==m_iPeopleHandCount[i])
		{
			iTemp1++;
		}
	}
	if(3==iTemp1)
	{
		return true;
	}
	if(2==iTemp1)
	{
		for(int i=0;i<iCount;i++)
		{
			if(0!=m_iPeopleHandCount[i] && LW_NO!=bPeopleStation[i])
			{
				m_vFinishSort.push_back(i);
				return true;
			}
		}
		
	}
	return false;
}
//�Ƿ�����
bool CServerGameDesk::IsMatch()
{
	//����Ƿ�����
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	//��Ϸ������Ϣ
	CString key = TEXT("match");
	m_iIsMatch=f.GetKeyVal(key,"IsMatch",0);									//�Ƿ�����	
	
	CString pri;
	BYTE bTemp=0;
	if(m_iIsMatch)
	{
		m_bMatchPeople=f.GetKeyVal(key,"MatchPeople",255);							//������
		if(!m_Logic.TestData(m_bMatchPeople,0))
		{
			memset(m_bMatchCard,255,sizeof(m_bMatchCard));
			m_iIsMatch=0;
			m_bMatchPeople=255;
			return false;
		}
		for(int i=0;i<CARD_NUMS;i++)
		{
			bTemp=0;
			pri.Format("MatchCard%d",i+1);
			bTemp=f.GetKeyVal(key,pri,0);	//�������
			if(m_Logic.TestData(bTemp,1))
			{
				m_bMatchCard[i]=bTemp;
			}
			else
			{
				memset(m_bMatchCard,255,sizeof(m_bMatchCard));
				m_iIsMatch=0;
				m_bMatchPeople=255;
				return false;
			}
		}     
	}
	else
	{
		memset(m_bMatchCard,255,sizeof(m_bMatchCard));
		m_iIsMatch=0;
		m_bMatchPeople=255;
		return false;
	}
	return true;
}
//֪ͨ�ͷ�����ʾ���͵÷�
void CServerGameDesk::InformShowStyle()
{
	GameServerToClient_StyleCard serverToClient_style;
	int index=0;
	for(int i=0;i<12;i++)
	{
		if(m_Logic.TestData(bHavePointCard[i],1))
		{
			
			serverToClient_style.bCardList[index]=bHavePointCard[i];
			index++;
		}
		
	}
	
	serverToClient_style.bDeskStation=bOutBigPeople;
	//�������ͷ��˷�������                                          
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&serverToClient_style,sizeof(serverToClient_style),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_SHOW,0);
		}
	}
	SendWatchData(PLAY_COUNT,&serverToClient_style,sizeof(serverToClient_style),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_SHOW,0);
}
//���һ��̯��
void CServerGameDesk::InformShowCard()
{
	GameServerToClient_ShowCard serverToClient_lastShow;
	BYTE bLastStation=m_vFinishSort.at(PLAY_COUNT-1);
	//������һ���Ƿ���ʣ����
	int iTemp=0;
	if(0!=m_iPeopleHandCount[bLastStation])
	{
		for(int i=0;i<CARD_NUMS;i++)
		{
			if(m_Logic.TestData(m_bPeopleHandleCard[bLastStation][i],1))
			{
				serverToClient_lastShow.bRemainCard[iTemp]=m_bPeopleHandleCard[bLastStation][i];
				iTemp++;
			}
		}
		serverToClient_lastShow.bLastStation=bLastStation;
		//�������ͷ��˷�������                                          
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if(m_pUserInfo[i])
			{
				SendGameData(i,&serverToClient_lastShow,sizeof(serverToClient_lastShow),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_HANDUP,0);
			}
		}
		SendWatchData(PLAY_COUNT,&serverToClient_lastShow,sizeof(serverToClient_lastShow),MDM_GM_GAME_NOTIFY,SERVERTOCLIENT_HANDUP,0);
	}
	
}
//����Ұ��÷ֶ��ٽ�������
void CServerGameDesk::Ranking(BYTE rank[],int iRandCount,__int64 i64Point[],int iPointCount)
{
	__int64 i64Temp[PLAY_COUNT]={0};
	CopyMemory(i64Temp,i64Point,sizeof(i64Temp));
	int i=0,j=0,t=0;
	for(i=0;i<iRandCount;i++)
	{
		for(j=i;j<iPointCount;j++)
		{
			if(i64Temp[i]<i64Temp[j])
			{
				t=i64Temp[i];
				i64Temp[i]=i64Temp[j];
				i64Temp[j]=t;
			}
		}
	}
	//��������λ�Ŵ洢��rank������
	int k=0;
	__int64 i64Flush[PLAY_COUNT];
	memset(i64Flush,255,sizeof(i64Flush));
	CopyMemory(i64Flush,i64Point,sizeof(i64Flush));
	for(int i=0;i<iRandCount;i++)
	{
		for(int j=0;j<iPointCount;j++)
		{
			if(255==i64Flush[j])
			{
				continue;
			}
			if(i64Temp[k]==i64Flush[j])
			{
				i64Flush[j]=255;
				rank[k]=j;
				k++;
				break;
			}
		}
	}
}
//�õ������Ƶ����
int CServerGameDesk::GetOutputEndCard()
{
	int iTemp=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(0==m_iPeopleHandCount[i])
		{
			iTemp++;
		}
	}
	return iTemp;
}
//�Զ�����
BOOL CServerGameDesk::AutoOutCard(BYTE bDeskStation,bool bFlag)
{
	
	BYTE iResultCard[CARD_NUMS]={0};
	int iResultCardCount=0;
	int iLeftTemp=m_Logic.CountUserCard(m_bLeftCard,CARD_NUMS);//Ҫѹ���Ƶ�����
	int iHandCount=m_Logic.CountUserCard(m_bPeopleHandleCard[bDeskStation],CARD_NUMS);//���е�������
	if(iLeftTemp>0)//��ҪѺ����
	{
		m_Logic.TackOutCardMoreThanLast(m_bPeopleHandleCard[bDeskStation],iHandCount,m_bLeftCard,iLeftTemp,iResultCard,iResultCardCount);
		if(iResultCardCount>0)//�бȵ�ǰ�����
		{
			//��Ҫ�����ƴ���������ɾ����
			int iDeleteCount=m_Logic.RemoveCard(iResultCard,iResultCardCount,m_bPeopleHandleCard[bDeskStation],iHandCount);
			GameClientToServer_Button startMessage;
			startMessage.bDeskStation=bDeskStation;
			startMessage.iBtType=CLICK_OUTCARD;
			startMessage.bFlag=bFlag;
			memcpy(startMessage.bCardList,iResultCard,sizeof(BYTE)*iResultCardCount);
			startMessage.iCardCount=iResultCardCount;
			OnButtonMessage(bDeskStation,NULL,&startMessage,sizeof(GameClientToServer_Button),0,0);
			return true;
		}
		else//û�бȵ�ǰ�����
		{
			//����
			GameClientToServer_Button startMessage;
			startMessage.bDeskStation=bDeskStation;
			startMessage.iBtType=CLICK_NOOUT;
			startMessage.bFlag=bFlag;
			OnButtonMessage(bDeskStation,NULL,&startMessage,sizeof(GameClientToServer_Button),0,0);
			return true;
		}
	}
	else
	{
		//���һ����
		m_Logic.SortCard(m_bPeopleHandleCard[bDeskStation],NULL,iHandCount);
		BYTE iResultCard[CARD_NUMS]={0};
		int iResultCardCount=0;
		m_Logic.AutoOutCard(m_bPeopleHandleCard[bDeskStation],iHandCount,NULL,0,iResultCard,iResultCardCount,true);
		if(iResultCardCount>0)
		{
			//��Ҫ�����ƴ���������ɾ����
			int iDeleteCount=m_Logic.RemoveCard(iResultCard,iResultCardCount,m_bPeopleHandleCard[bDeskStation],iHandCount);
			GameClientToServer_Button startMessage;
			startMessage.bDeskStation=bDeskStation;
			startMessage.iBtType=CLICK_OUTCARD;
			startMessage.bFlag=bFlag;
			CopyMemory(startMessage.bCardList,iResultCard,sizeof(BYTE)*iResultCardCount);
			startMessage.iCardCount=iResultCardCount;
			OnButtonMessage(bDeskStation,NULL,&startMessage,sizeof(GameClientToServer_Button),0,0);
			return true;
		}
	}
}
//�����շ�
void CServerGameDesk::CollectPoint(BYTE bDeskStation)
{
	for(int i=0;i<12;i++)
	{
		if(!m_Logic.TestData(bHavePointCard[i],1))
		{
			continue;
		}
		for(int j=0;j<12;j++)
		{
			if(!m_Logic.TestData(m_bPeoplePoint[bDeskStation][j],1))
			{
				m_bPeoplePoint[bDeskStation][j]=bHavePointCard[i];
				break;
			}
		}
	}
	CopyMemory(m_bPeoplePoint[bDeskStation],bHavePointCard,sizeof(m_bPeoplePoint[bDeskStation]));
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(bDeskStation==i)//�����Ʒ�ӵ����
		{
			m_iPoint[i]+=m_Logic.Get510kPoint(bHavePointCard,12);
		}
	}
}

//��Ϸ����������ж������
void CServerGameDesk::CleanUpCut()
{
	//�㲥�߳����ж������
	for(int i=0; i<PLAY_COUNT; i++)
	{
		
		if (m_IsCut[i])
		{
			CleanCutGameInfo(i);
		}
	}
}