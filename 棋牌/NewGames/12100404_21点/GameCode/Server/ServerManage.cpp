#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"


	//��Ϸ��չ����
BYTE CServerGameDesk::m_bGameBeginWaitTime;								
BYTE CServerGameDesk::m_bGameBetTime;											
BYTE CServerGameDesk::m_bCallCardTime;										
BYTE CServerGameDesk::m_bSendCardTime;
BYTE CServerGameDesk::m_bFinishTime;
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	//��ʼ������������
	InitData();
}
//��������
CServerGameDesk::~CServerGameDesk(void)
{
}
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");
	m_bGameBeginWaitTime = f.GetKeyVal(key,"GameBeginWaitTime",30);
	if(m_bGameBeginWaitTime<10)
	{
		m_bGameBeginWaitTime=10;
	}
	m_bGameBetTime = f.GetKeyVal(key,"GameBetTime",15);
	if(m_bGameBetTime<10)
	{
		m_bGameBetTime=10;
	}
	m_bCallCardTime = f.GetKeyVal(key,"GameCallCardTime",15);
	if(m_bCallCardTime<10)
	{
		m_bCallCardTime=10;
	}
	//����
	key = "SuperUser";
	// �жϳ����û��ڷ��������Ƿ�����
	int iCount = f.GetKeyVal(key, "SuperUserCount", 0);
	long  lUserID = 0;
	CString strText;
	m_vlSuperID.clear();
	for (int j=0; j<iCount; j++)
	{
		strText.Format("SuperUserID%d", j);
		lUserID = f.GetKeyVal(key, strText, 0);
		//�����г���ID ���浽����������
		m_vlSuperID.push_back(lUserID);
		CString cs;
		cs.Format("lwlog ����ID=%d",lUserID);
		OutputDebugString(cs);
	}
	return true;
}
//��ʼ����������Ϸ����
BOOL CServerGameDesk::InitData()
{
	m_bGameStation=GS_WAIT_ARGEE;
	m_bSendCardTime=2;
	m_bFinishTime=3;
	m_iNtPeople=255;
	m_GameData.InitData();
	ReSetGameState(0);
	return TRUE;
}
//��ʼ��
bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadIni();
	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key;
	key.Format("%s_%d",SKIN_FOLDER,iRoomID);
	m_bGameBeginWaitTime = f.GetKeyVal(key,"GameBeginWaitTime",m_bGameBeginWaitTime);
	if(m_bGameBeginWaitTime<10)
	{
		m_bGameBeginWaitTime=10;
	}
	m_bGameBetTime = f.GetKeyVal(key,"GameBetTime",m_bGameBetTime);
	if(m_bGameBetTime<10)
	{
		m_bGameBetTime=10;
	}
	m_bCallCardTime = f.GetKeyVal(key,"GameCallCardTime",m_bCallCardTime);
	if(m_bCallCardTime<10)
	{
		m_bCallCardTime=10;
	}

	//����
	key = "SuperUser";
	// �жϳ����û��ڷ��������Ƿ�����
	int iCount = f.GetKeyVal(key, "SuperUserCount", 0);
	long  lUserID = 0;
	CString strText;
	for (int j=0; j<iCount; j++)
	{
		strText.Format("SuperUserID[%d]", j);
		lUserID = f.GetKeyVal(key, strText, m_vlSuperID.at(j));
		//�����г���ID ���浽����������
		m_vlSuperID.push_back(lUserID);
		CString cs;
		cs.Format("lwlog ����ID=%d",lUserID);
		OutputDebugString(cs);
	}
	return TRUE;
}
//�����Ϣ������
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{
			
			GameFinish(bDeskStation,GFF_FORCE_FINISH);
			//MakeUserOffLine(bDeskStation);
			char cKey[10];
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
			}
			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	/*CString aa;
	aa.Format("lw::test: Server Msg %d", pNetHead->bAssistantID);
	OutputDebugString(aa);*/
	switch(pNetHead->bAssistantID)
	{
		case ASS_GM_AGREE_GAME:
			{
				if (bWatchUser)
				{
					return TRUE;
				}
				OnButtonMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
				return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
			}
			case GAME_BUTTON:
				{
					if (bWatchUser)
					{
						return TRUE;
					}
					OnButtonMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
					return true;
				}
			case ASS_SUPER_RESULT://�����û���Ϣ
				{
					if(uSize != sizeof(SuperUserControlReslut))
					{
						return true ; 
					}
					DealSuperUserMessage(pData);
					return true;
				}
		default:
			{
				break;
			}
	}
	
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		GameFinish(0,GF_SALE);
		return false;
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] && PLAYER_READY==m_iPlayerStation[i])//��Ҳ����ھ�
		{
			m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
		}
	}	

	//����ȷ��ׯ����Ϣ
	SendHaveNt();
	return true;
}
//�������û����ƽ��
void	CServerGameDesk::DealSuperUserMessage(void *pBuffer)
{
	SuperUserControlReslut *SuperUserMessage = (SuperUserControlReslut *)pBuffer;
	if(NULL == SuperUserMessage)
	{
		return;
	}
	if(PLAYER_JOIN==m_iPlayerStation[SuperUserMessage->byDeskStation])
	{
		return;
	}
	m_bIsSuperControl=SuperUserMessage->bySuperReslut;
	m_bLastSuperStation=SuperUserMessage->byDeskStation;
}
/*
@brief:��ť��Ϣ������
*/
void CServerGameDesk::OnButtonMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if(uSize!=sizeof(GameStationButton))
	{
		return;
	}
	GameStationButton *ButtonMessage=(GameStationButton *)pData;
	if(NULL==ButtonMessage)
	{
		return;
	}
	//��������;������ҵ���Ϣ
	if(PLAYER_JOIN==m_iPlayerStation[ButtonMessage->bDeskStation])
	{
		return;
	}
	switch(ButtonMessage->iBtType)
	{
	case CLICK_START:
		{
			if ((m_bGameStation==GS_WAIT_ARGEE)||(m_bGameStation==GS_WAIT_NEXT)) 
			{
				if (m_pUserInfo[bDeskStation]) 
				{
					m_bUserReady[bDeskStation]=1;
					m_iPlayerStation[bDeskStation]=PLAYER_READY;
					m_GameData.SetAgreePeople(bDeskStation);

					AgreeGame agreeGame;
					agreeGame.flag=true;
					memcpy(agreeGame.bUserReady,m_bUserReady,sizeof(m_bUserReady));
					for(int i=0;i<PLAY_COUNT;i++)
					{
						if(m_pUserInfo[i])
						{
							SendGameData(i,&agreeGame,sizeof(agreeGame),MDM_GM_GAME_NOTIFY,ASS_AGREE,0);		
						}
					}
				}
			}
			break;
		}
	case CLICK_BETOPERATE:
		{
			if(m_bGameStation!=GS_BET)
			{
				return;
			}
			if(m_iNtPeople==ButtonMessage->bDeskStation)
			{
				return;
			}
			if(ButtonMessage->i64Note<=0)
			{
				return;
			}
			if(PLAYER_JOIN==m_iPlayerStation[ButtonMessage->bDeskStation])
			{
				return;
			}
			if(GS_BET!=m_iPlayerStation[ButtonMessage->bDeskStation])
			{
				return;
			}
			if(m_pUserInfo[ButtonMessage->bDeskStation])
			{
				if(m_pUserInfo[ButtonMessage->bDeskStation]->m_UserData.isVirtual)
				{
					m_GameData.SetPlayerBet(ButtonMessage->bDeskStation,ButtonMessage->i64Note,false);
				}
				else
				{
					m_GameData.SetPlayerBet(ButtonMessage->bDeskStation,ButtonMessage->i64Note,true);
				}
			}
			GameMessageBetResult betResult;
			betResult.bDeskStation=ButtonMessage->bDeskStation;
			betResult.i64Note=ButtonMessage->i64Note;
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&betResult,sizeof(betResult),MDM_GM_GAME_NOTIFY,ASS_BETRESULT,0);
				}
			}
			CheckSend();
			break;
		}
	case CLICK_OPERATE:		//��ע�в���
		{
			switch(ButtonMessage->iBtParam)
			{
			case 2:		//����
				{
					if(PLAYER_JOIN==m_iPlayerStation[ButtonMessage->bDeskStation])
					{
						return;
					}
					if(GS_BET!=m_iPlayerStation[ButtonMessage->bDeskStation])
					{
						return;
					}
					m_iPlayerStation[ButtonMessage->bDeskStation]=PLAYER_GIVEUP;
					//����Ƿ��ܷ���
					CheckSend();
					break;
				}
			case 1:		//�ӱ�
			case 0:		//��ע
			case 3:		//Ҫ��
			case 4:		//����
			case 5:		//ͣ��
				{
					break;
				}
			default:
				{
					break;
				}
			}
			break;
		}
	case CLICK_PLAYOPERATE:		//��Ϸ�в���
		{
			switch(ButtonMessage->iBtParam)
			{
			case 0:		//��ע(��Ϸ�в�����)
				{
					return;
				}
			case 1:		//�ӱ�
				{
					OutputDebugString("lw::�յ��ӱ���Ϣ");
					if(PLAYER_JOIN==m_iPlayerStation[ButtonMessage->bDeskStation])
					{
						return;
					}
					if(GS_PLAY_GAME!=m_iPlayerStation[bDeskStation])
					{
						return;
					}

					m_iScoreNum--;
					m_iCallLabel++;

					if(m_pUserInfo[ButtonMessage->bDeskStation])
					{
						if(m_pUserInfo[ButtonMessage->bDeskStation]->m_UserData.isVirtual)
						{
							m_GameData.SetPlayerBet(ButtonMessage->bDeskStation,ButtonMessage->i64Note,false);
						}
						else
						{
							m_GameData.SetPlayerBet(ButtonMessage->bDeskStation,ButtonMessage->i64Note,true);
						}
					}

					GameMessageNeedCard needCard;

					BYTE bCallCard=0;
					for(int i=0;i<13;i++)
					{
						if(m_bUserCard[ButtonMessage->bDeskStation][i]>0)
						{
							bCallCard=m_bUserCard[ButtonMessage->bDeskStation][i];
							//code:�Ѿ������Ʒŵ��ѷ�������
							for(int j=0;j<13;j++)
							{
								if(m_bUserAlreadySendCard[ButtonMessage->bDeskStation][j]<=0)
								{
									m_bUserAlreadySendCard[ButtonMessage->bDeskStation][j]=m_bUserCard[ButtonMessage->bDeskStation][i];
									break;
								}
							}
							m_bUserCard[ButtonMessage->bDeskStation][i]=0;
							break;
						}
					}
					//code:�з����
					if(m_iScoreNum<=0)
					{
						needCard.bNextStation=255;
						needCard.bFlag=false;
					}
					else
					{
						needCard.bFlag=true;
						needCard.bNextStation=m_bScoreSqueue[m_iCallLabel];			//��һ���з���
						m_bCurrentCallPeople=needCard.bNextStation;						//��ǰ�з���
					}
					needCard.bDeskStation=ButtonMessage->bDeskStation;
					needCard.iStyle=SERVER_EXTRA_DOUBLE;							//�ӱ�
					needCard.bCard=bCallCard;										//��
					needCard.i64Note=ButtonMessage->i64Note;
					for(int i=0;i<PLAY_COUNT;i++)
					{
						if(m_pUserInfo[i])
						{
							SendGameData(i,&needCard,sizeof(needCard),MDM_GM_GAME_NOTIFY,ASS_CALL,0);
						}
					}

					if(needCard.bFlag)
					{
						KillAllTimer();
						SetTimer(TIME_CALL,(m_bCallCardTime+2)*1000);
					}
					else
					{
						KillAllTimer();
						//code:����������ʱ��
						SetTimer(TIME_FINISH,m_bFinishTime*1000);
					}
					break;
				}
			case 2:		//����
				{
					OutputDebugString("lw::�յ�������Ϣ");
					if(PLAYER_JOIN==m_iPlayerStation[ButtonMessage->bDeskStation])
					{
						return;
					}
					if(GS_PLAY_GAME==m_iPlayerStation[bDeskStation])
					{
						m_iPlayerStation[bDeskStation]=PLAYER_GIVEUP;
					}
					else
					{
						return;
					}
					m_iScoreNum--;
					m_iCallLabel++;

					GameMessageNeedCard needCard;
					//code:�з����
					if(m_iScoreNum<=0)
					{
						
						needCard.bFlag=false;
						needCard.bNextStation=255;
					}
					else
					{
						needCard.bFlag=true;
						needCard.bNextStation=m_bScoreSqueue[m_iCallLabel];			//��һ���з���
						m_bCurrentCallPeople=needCard.bNextStation;						//��ǰ�з���
					}
					
					needCard.bDeskStation=ButtonMessage->bDeskStation;
					needCard.iStyle=SERVER_EXTRA_CLICK_GIVEUP;	
					
					for(int i=0;i<PLAY_COUNT;i++)
					{
						if(m_pUserInfo[i])
						{
							SendGameData(i,&needCard,sizeof(needCard),MDM_GM_GAME_NOTIFY,ASS_CALL,0);
						}
					}
					if(needCard.bFlag)
					{
						KillAllTimer();
						SetTimer(TIME_CALL,(m_bCallCardTime+2)*1000);
					}
					else
					{
						KillAllTimer();
						//code:����������ʱ��
						SetTimer(TIME_FINISH,m_bFinishTime*1000);
					}
					break;
				}
			case 3:		//Ҫ��
				{
					OutputDebugString("lw::�յ�Ҫ����Ϣ");
					if(PLAYER_JOIN==m_iPlayerStation[ButtonMessage->bDeskStation])
					{
						return;
					}
					if(GS_PLAY_GAME!=m_iPlayerStation[bDeskStation])
					{
						return;
					}

					BYTE bCallCard=0;
					for(int i=0;i<13;i++)
					{
						if(m_bUserCard[ButtonMessage->bDeskStation][i]>0)
						{
							bCallCard=m_bUserCard[ButtonMessage->bDeskStation][i];
							//code:�Ѿ������Ʒŵ��ѷ�������
							for(int j=0;j<13;j++)
							{
								if(m_bUserAlreadySendCard[ButtonMessage->bDeskStation][j]<=0)
								{
									m_bUserAlreadySendCard[ButtonMessage->bDeskStation][j]=m_bUserCard[ButtonMessage->bDeskStation][i];
									break;
								}
							}
							m_bUserCard[ButtonMessage->bDeskStation][i]=0;
							break;
						}
					}

					int iTempCount=0,iTempPoint=0;
					
					for(int j=0;j<13;j++)
					{
						if(m_bUserAlreadySendCard[ButtonMessage->bDeskStation][j]>0)
						{
							iTempCount++;
						}
					}
					iTempPoint=GetPoints(m_bUserAlreadySendCard[ButtonMessage->bDeskStation],iTempCount);

					GameMessageNeedCard needCard;
					needCard.iPoints=iTempPoint;
					needCard.bFlag=true;
					//������Ҫ��
					if(iTempPoint>=21)
					{
						
						m_iScoreNum--;
						m_iCallLabel++;
						//code:�з����
						if(m_iScoreNum<=0)
						{
							needCard.bNextStation=255;
							needCard.bFlag=false;
						}
						else
						{
							needCard.bNextStation=m_bScoreSqueue[m_iCallLabel];			//��һ���з���	
							m_bCurrentCallPeople=needCard.bNextStation;						//��ǰ�з���
						}
						
					}
					else
					{
						needCard.bNextStation=255;
					}

					needCard.bDeskStation=ButtonMessage->bDeskStation;
					needCard.iStyle=SERVER_EXTRA_CLICK_NEED;
					needCard.bCard=bCallCard;										//��

					for(int i=0;i<PLAY_COUNT;i++)
					{
						if(m_pUserInfo[i])
						{
							SendGameData(i,&needCard,sizeof(needCard),MDM_GM_GAME_NOTIFY,ASS_CALL,0);
						}
					}
					if(!needCard.bFlag)
					{
						KillAllTimer();
						//code:����������ʱ��
						SetTimer(TIME_FINISH,m_bFinishTime*1000);
					}
					else
					{
						KillAllTimer();
						//����Ҫ�ƶ�ʱ��
						SetTimer(TIME_CALL,(m_bCallCardTime+2)*1000);
					}
					break;
				}
			case 4:		//����
				{
					if(PLAYER_JOIN==m_iPlayerStation[ButtonMessage->bDeskStation])
					{
						return;
					}
					if(GS_PLAY_GAME!=m_iPlayerStation[bDeskStation])
					{
						return;
					}
					break;
				}
			case 5:		//ͣ��
				{
					OutputDebugString("lw::�յ�ͣ����Ϣ");
					if(PLAYER_JOIN==m_iPlayerStation[ButtonMessage->bDeskStation])
					{
						return;
					}
					if(GS_PLAY_GAME!=m_iPlayerStation[bDeskStation])
					{
						return;
					}
					m_iScoreNum--;
					m_iCallLabel++;

					GameMessageNeedCard needCard;
					//code:�з����
					if(m_iScoreNum<=0)
					{
						needCard.bNextStation=255;
						needCard.bFlag=false;
					}
					else
					{
						needCard.bFlag=true;
						needCard.bNextStation=m_bScoreSqueue[m_iCallLabel];
						m_bCurrentCallPeople=m_bScoreSqueue[m_iCallLabel];
					}

					needCard.bDeskStation=ButtonMessage->bDeskStation;
					needCard.iStyle=SERVER_EXTRA_CLICK_STOP;

					for(int i=0;i<PLAY_COUNT;i++)
					{
						if(m_pUserInfo[i])
						{
							SendGameData(i,&needCard,sizeof(needCard),MDM_GM_GAME_NOTIFY,ASS_CALL,0);
						}
					}

					if(needCard.bFlag)
					{
						KillAllTimer();
						SetTimer(TIME_CALL,(m_bCallCardTime+2)*1000);
					}
					else
					{
						KillAllTimer();
						//code:����������ʱ��
						SetTimer(TIME_FINISH,m_bFinishTime*1000);
					}

					break;
				}
			default:
				{
					break;
				}
			}
			break;
		}
	default:
		{
			break;
		}
	}
	
}
/*
@brief:����Ƿ��ܷ���
*/
void CServerGameDesk::CheckSend()
{
	OutputDebugString("lw::CheckSend");
	int iTemp=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] && PLAYER_GIVEUP==m_iPlayerStation[i])
		{
			iTemp++;
		}
	}
	if(CountPlayer()-1==iTemp)
	{
		GameFinish(0,GF_SALE);
	}
	//��ʼ����
	if(m_GameData.m_AgreePeopleCount-iTemp-1==m_GameData.m_bBetPeopleCount)
	{
		KillAllTimer();
		//��ע����Һ�ׯ�ҿ��Է���
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
			{
				if(m_GameData.i64TotalBet[i]>0 || m_iNtPeople==i)
				{
					m_iPlayerStation[i]=GS_SEND;
				}
			}
		}
		SetTimer(TIME_STARTGAME,1*1000);
	}
}
/*
@breief:ͳ����Ϸ����
*/
BYTE  CServerGameDesk::CountPlayer()
{
	BYTE bTemp=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] && PLAYER_JOIN!=m_iPlayerStation[i])
		{
			bTemp++;
		}
	}
	return bTemp;
}
/*
@breief:ͳ�ƶ�������
*/
BYTE  CServerGameDesk::CountCut()
{
	BYTE bTemp=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_bIsCut[i])
		{
			bTemp++;
		}
	}
	return bTemp;
}
//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	// ��֤�����û�Ȩ��
	SuperUserExamine(bDeskStation);
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			OnSendGameStationAgree(bDeskStation,uSocketID,bWatchUser);
			return TRUE;
		}
	case GS_BET:				//��ע״̬
		{
			OnSendGameStationBet(bDeskStation,uSocketID,bWatchUser);
			m_iPlayerStation[bDeskStation]=PLAYER_JOIN;
			return TRUE;
		}
	case GS_SEND:				//����״̬
		{
			OnSendGameStationSendCard(bDeskStation,uSocketID,bWatchUser);
			m_iPlayerStation[bDeskStation]=PLAYER_JOIN;
			return true;
		}
	case GS_PLAY_GAME:			//��Ϸ��״̬
		{
			OnSendGameStationPlaying(bDeskStation,uSocketID,bWatchUser);
			m_iPlayerStation[bDeskStation]=PLAYER_JOIN;
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			OnSendGameStationNext(bDeskStation,uSocketID,bWatchUser);
			return TRUE;
		}
	}
	return false;
}
/*
@brief:�ȴ�ͬ��״̬
*/
void CServerGameDesk::OnSendGameStationAgree(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	GameStation_1 gameStation_1;
	gameStation_1.iVersion		= DEV_HEIGHT_VERSION;					//��Ϸ�߰汾
	gameStation_1.iVersion2		= DEV_LOW_VERSION;						//�Ͱ汾
	gameStation_1.bGameStation=m_bGameStation;							//��Ϸ״̬
	gameStation_1.bGameBeginWaitTime=m_bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	gameStation_1.bGameBetTime=m_bGameBetTime;							//��Ϸ��עʱ��
	gameStation_1.bCallCardTime=m_bCallCardTime;						//��Ϸ����ʱ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		gameStation_1.bUserReady[i]=m_bUserReady[i];		//���׼��״̬
	}
	SendGameStation(bDeskStation,uSocketID,bWatchUser,&gameStation_1,sizeof(gameStation_1));
}

/*
@brief:��ע״̬
*/
void CServerGameDesk::OnSendGameStationBet(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	GameStation_2 gameStation_2;
	gameStation_2.iVersion		= DEV_HEIGHT_VERSION;					//��Ϸ�߰汾
	gameStation_2.iVersion2		= DEV_LOW_VERSION;						//�Ͱ汾
	gameStation_2.bGameStation=m_bGameStation;							//��Ϸ״̬
	gameStation_2.bGameBeginWaitTime=m_bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	gameStation_2.bGameBetTime=m_bGameBetTime;							//��Ϸ��עʱ��
	gameStation_2.bCallCardTime=m_bCallCardTime;						//��Ϸ����ʱ��
	gameStation_2.iNtPeople=m_iNtPeople;								//ׯ��λ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		gameStation_2.bUserReady[i]=m_bUserReady[i];		//���׼��״̬
	}
	SendGameStation(bDeskStation,uSocketID,bWatchUser,&gameStation_2,sizeof(gameStation_2));
}
/*
@brief:����״̬
*/
void CServerGameDesk::OnSendGameStationSendCard(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	GameStation_5 gameStation_5;
	gameStation_5.iVersion		= DEV_HEIGHT_VERSION;					//��Ϸ�߰汾
	gameStation_5.iVersion2		= DEV_LOW_VERSION;						//�Ͱ汾
	gameStation_5.bGameStation=m_bGameStation;							//��Ϸ״̬
	gameStation_5.bGameBeginWaitTime=m_bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	gameStation_5.bGameBetTime=m_bGameBetTime;							//��Ϸ��עʱ��
	gameStation_5.bCallCardTime=m_bCallCardTime;						//��Ϸ����ʱ��
	gameStation_5.iNtPeople=m_iNtPeople;								//ׯ��λ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		gameStation_5.bUserReady[i]=m_bUserReady[i];		//���׼��״̬
	}
	SendGameStation(bDeskStation,uSocketID,bWatchUser,&gameStation_5,sizeof(gameStation_5));
}
/*
@brief:��Ϸ��״̬
*/
void CServerGameDesk::OnSendGameStationPlaying(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	GameStation_3 gameStation_3;
	gameStation_3.iVersion		= DEV_HEIGHT_VERSION;					//��Ϸ�߰汾
	gameStation_3.iVersion2		= DEV_LOW_VERSION;						//�Ͱ汾
	gameStation_3.bGameStation=m_bGameStation;							//��Ϸ״̬
	gameStation_3.bGameBeginWaitTime=m_bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	gameStation_3.bGameBetTime=m_bGameBetTime;							//��Ϸ��עʱ��
	gameStation_3.bCallCardTime=m_bCallCardTime;						//��Ϸ����ʱ��
	gameStation_3.iNtPeople=m_iNtPeople;								//ׯ��λ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		gameStation_3.bUserReady[i]=m_bUserReady[i];		//���׼��״̬
	}
	SendGameStation(bDeskStation,uSocketID,bWatchUser,&gameStation_3,sizeof(gameStation_3));
}

/*
@brief:�ȴ���һ����Ϸ��ʼ
*/
void CServerGameDesk::OnSendGameStationNext(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	GameStation_4 gameStation_4;
	gameStation_4.iVersion		= DEV_HEIGHT_VERSION;					//��Ϸ�߰汾
	gameStation_4.iVersion2		= DEV_LOW_VERSION;						//�Ͱ汾
	gameStation_4.bGameStation=m_bGameStation;							//��Ϸ״̬
	gameStation_4.bGameBeginWaitTime=m_bGameBeginWaitTime;				//��Ϸ��ʼ�ȴ�ʱ��
	gameStation_4.bGameBetTime=m_bGameBetTime;							//��Ϸ��עʱ��
	gameStation_4.bCallCardTime=m_bCallCardTime;						//��Ϸ����ʱ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		gameStation_4.bUserReady[i]=m_bUserReady[i];		//���׼��״̬
	}
	SendGameStation(bDeskStation,uSocketID,bWatchUser,&gameStation_4,sizeof(gameStation_4));
}
//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	m_bLastSuperStation=255;
	memset(m_bUserReady,0,sizeof(m_bUserReady));
	memset(m_iPlayerStation,-1,sizeof(m_iPlayerStation));
	memset(m_bUserCard,0,sizeof(m_bUserCard));
	memset(m_bScoreSqueue,255,sizeof(m_bScoreSqueue));
	memset(m_bUserAlreadySendCard,0,sizeof(m_bUserAlreadySendCard));
	memset(m_iUserPoints,255,sizeof(m_iUserPoints));
	memset(m_i64UserScore,0,sizeof(m_i64UserScore));
	memset(m_bIsCut,0,sizeof(m_bIsCut));
	m_iCallLabel=0;
	m_iNeedLabel=0;
	m_iScoreNum=0;
	m_bIsSuperControl=false;
	return TRUE;
}
//����ȷ��ׯ����Ϣ
void CServerGameDesk::SendHaveNt()
{
	OutputDebugString("lw::SendHaveNt");
	GameMessageNt NtMessage;
	
	if(255==m_iNtPeople)
	{
		m_iNtPeople=NtMessage.bNtStation=m_GameData.GetRandomNt();
	}
	else
	{
		m_iNtPeople=NtMessage.bNtStation=HaveNt(m_iNtPeople);
	}
	
	NtMessage.flag=true;
	//����ׯ����Ϣ
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&NtMessage,sizeof(NtMessage),MDM_GM_GAME_NOTIFY,ASS_NT,0);		//���з�������������û�
		}

	}
	//��ʼ��ע
	StartNote();
}
//��ʼ��ע
void CServerGameDesk::StartNote()
{
	m_bGameStation=GS_BET;

	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] && PLAYER_READY==m_iPlayerStation[i])
		{
			m_iPlayerStation[i]=m_bGameStation;
		}
	}

	GameMessageBet BetMessage;
	BetMessage.flag=true;
	int iLimitTemp=0;
	if(m_pUserInfo[m_iNtPeople]->m_UserData.i64Money)
	{
		iLimitTemp=m_pUserInfo[m_iNtPeople]->m_UserData.i64Money/(m_GameData.m_AgreePeopleCount-1);
	}
	
	int iLabel=0;
	__int64 i64Temp=AccountLimit();
	for(int i=0;i<12;i++)
	{
		if(iDouble[11]<i64Temp)
		{
			iLabel=11;
			break;
		}
		if(iDouble[i]>i64Temp)
		{
			iLabel=i-1;
			break;
		}
		else if(iDouble[i]==i64Temp)
		{
			iLabel=i;
			break;
		}
	}

	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] && GS_BET==m_iPlayerStation[i])
		{
			if(m_iNtPeople==i)
			{
				continue;
			}
			if(m_pUserInfo[i]->m_UserData.i64Money>=iDouble[iLabel])
			{
				BetMessage.i64BetLimit[i]=iDouble[iLabel];
				BetMessage.i64Bet[i][0]=iDouble[iLabel-1];
				BetMessage.i64Bet[i][1]=iDouble[iLabel-2];
				BetMessage.i64Bet[i][2]=iDouble[iLabel-3];
			}
			else
			{
				for(int j=iLabel;j>=0;j--)
				{
					if(m_pUserInfo[i]->m_UserData.i64Money>=iDouble[j])
					{
						BetMessage.i64BetLimit[i]=iDouble[j];
						BetMessage.i64Bet[i][0]=iDouble[j-1];
						BetMessage.i64Bet[i][1]=iDouble[j-2];
						BetMessage.i64Bet[i][2]=iDouble[j-3];
						break;
					}
				}
			}
		}
	}
	
	//������ע��Ϣ
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&BetMessage,sizeof(BetMessage),MDM_GM_GAME_NOTIFY,ASS_BET,0);		
		}
	}
	SetTimer(TIME_BET,(m_bGameBetTime+2)*1000);
}
//��ʼ����
void CServerGameDesk::SendCard()
{
	m_bGameStation= GS_SEND;
	GameMessageSendCard SendCardMessage;
	SendCardMessage.flag=true;
	SendCardMessage.bFirstPeople=m_GameData.GetRandomFirstSendCard();
	BYTE bCardList[CARDCOUNT];
	memset(bCardList,0,sizeof(bCardList));
	m_Logic.RandCard(bCardList,CARDCOUNT,0);

	int test=0;
	int iStemp=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(GS_SEND==m_iPlayerStation[i] && m_pUserInfo[i])
		{
			memcpy(m_bUserCard[i],&bCardList[iStemp],sizeof(BYTE)*13);
			iStemp+=13;
			test++;
		}
	}

	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(GS_SEND==m_iPlayerStation[i] && m_pUserInfo[i])
		{
			//����ÿ����ҷ������ƣ��ѷ���������0
			for(int j=0;j<2;j++)
			{
				SendCardMessage.bUserCard[i][j]=m_bUserCard[i][j];
				m_bUserAlreadySendCard[i][j]=m_bUserCard[i][j];
				m_bUserCard[i][j]=0;
			}
		}
		
	}

	if(m_bIsSuperControl)			//�����û�����
	{
		
		int iRandom=rand()%4;
		SendCardMessage.bUserCard[m_bLastSuperStation][0]=bCardA[iRandom];
		m_bUserAlreadySendCard[m_bLastSuperStation][0]=bCardA[iRandom];
		iRandom=rand()%16;
		SendCardMessage.bUserCard[m_bLastSuperStation][1]=bCardB[iRandom];
		m_bUserAlreadySendCard[m_bLastSuperStation][1]=bCardB[iRandom];
		
	}
	//���ͷ�����Ϣ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&SendCardMessage,sizeof(SendCardMessage),MDM_GM_GAME_NOTIFY,ASS_SEND,0);		//����
		}
		
	}
}
/*
@brief:������ע���޺�����
*/
__int64 CServerGameDesk::AccountLimit()
{
	//ׯ�ҽ��
	__int64 i64NtTemp=m_pUserInfo[m_iNtPeople]->m_UserData.i64Money;
	double dTempBase=1;
	dTempBase=dTempBase/(m_GameData.m_AgreePeopleCount-1);
	return i64NtTemp*dTempBase;
}
//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIME_BET:
		{
			KillTimer(TIME_BET);

			//ɨ��û����ע�����
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i] && m_iNtPeople!=i)
				{
					if(m_GameData.i64TotalBet[i]<=0)
					{
						m_iPlayerStation[i]=PLAYER_GIVEUP;
					}
				}
			}
			CheckSend();
			break;
		}
	case TIME_SENDCARD:
		{
			KillTimer(TIME_SENDCARD);
			StartGame();
			break;
		}
	case TIME_CALL:
		{

			KillTimer(TIME_CALL);
			m_iScoreNum--;
			m_iCallLabel++;

			GameMessageNeedCard needCard;
			//code:�з����
			if(m_iScoreNum<=0)
			{

				needCard.bFlag=false;
				needCard.bNextStation=255;
			}
			else
			{
				needCard.bFlag=true;
				needCard.bNextStation=m_bScoreSqueue[m_iCallLabel];			//��һ���з���
				m_bCurrentCallPeople=needCard.bNextStation;						//��ǰ�з���
			}

			needCard.iStyle=SERVER_EXTRA_CLICK_TIMEEND;	

			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&needCard,sizeof(needCard),MDM_GM_GAME_NOTIFY,ASS_CALL,0);
				}
			}

			if(needCard.bFlag)
			{
				KillAllTimer();
				SetTimer(TIME_CALL,(m_bCallCardTime+2)*1000);
			}
			else
			{
				KillAllTimer();
				//code:����������ʱ��
				SetTimer(TIME_FINISH,m_bFinishTime*1000);
			}
			break;
		}
	case TIME_FINISH:
		{
			KillTimer(TIME_FINISH);
			KillAllTimer();
			GameFinish(0,GF_NORMAL);
			break;
		}
	case TIME_STARTGAME:
		{
			KillTimer(TIME_STARTGAME);	
			SendCard();
			//�������ƶ�ʱ��
			SetTimer(TIME_SENDCARD,m_bSendCardTime*1000);  
			break;
		}
	}
	return __super::OnTimer(uTimerID);
}
//��ʼ��Ϸ
void CServerGameDesk::StartGame()
{
	KillAllTimer();
	m_bGameStation=GS_PLAY_GAME;
	//���Ƶ���Ҳſ��Խ�����Ϸ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] && GS_SEND==m_iPlayerStation[i])
		{
			m_iPlayerStation[i]=GS_PLAY_GAME;
		}
	}
	
	int k=m_GameData.GetRandomFirstSendCard();
	if(m_pUserInfo[k])
	{
		int iTempCount=0;
		for(int i=0;i<PLAY_COUNT;i++)
		{
				
			for(int j=0;j<13;j++)
			{
				if(m_bUserAlreadySendCard[i][j]>0)
				{
					iTempCount++;
				}
			}
			m_iUserPoints[i]=GetPoints(m_bUserAlreadySendCard[i],iTempCount);
			iTempCount=0;
		}

		GameMessageStart startGame;
		startGame.flag=true;
		int j=0;
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(k>=PLAY_COUNT)
			{
				k=0;
			}
			if(m_pUserInfo[k] && GS_PLAY_GAME==m_iPlayerStation[k] && m_iUserPoints[k]<21)
			{
				m_bScoreSqueue[j]=k;
				m_iScoreNum++;
				j++;
			}
			k++;
		}
		m_iCallLabel=0;
		startGame.bFirstScoreStation=m_bScoreSqueue[m_iCallLabel];
		m_bCurrentCallPeople=startGame.bFirstScoreStation;
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
			{
				SendGameData(i,&startGame,sizeof(startGame),MDM_GM_GAME_NOTIFY,ASS_PLAY,0);		//���Ϳ�ʼ��Ϸ��Ϣ
			}
		}
	}

	//����Ҫ�ƶ�ʱ��
	SetTimer(TIME_CALL,(m_bCallCardTime+2)*1000);
}
//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			m_bGameStation=GS_WAIT_NEXT;
			KillAllTimer();

			CString cs;
			
			int iTempCount=0;
			int iTempPoints=0;
			for(int i=0;i<PLAY_COUNT;i++)
			{
				
				for(int j=0;j<13;j++)
				{
					if(m_bUserAlreadySendCard[i][j]>0)
					{
						iTempCount++;
					}
				}
				iTempPoints=GetPoints(m_bUserAlreadySendCard[i],iTempCount);
				if(iTempPoints>21 ||  PLAYER_GIVEUP==m_iPlayerStation[i])
				{
					iTempPoints=0;
				}
				m_iUserPoints[i]=iTempPoints;
				iTempCount=0;
				iTempPoints=0;
			}
			
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_iNtPeople==i)
				{
					continue;
				}
				if(m_iUserPoints[i]>m_iUserPoints[m_iNtPeople])
				{
					m_i64UserScore[i]=m_GameData.i64TotalBet[i];
				}
				else if(m_iUserPoints[i]==m_iUserPoints[m_iNtPeople])
				{
					m_i64UserScore[i]=0;
				}
				else 
				{
					m_i64UserScore[i]=-m_GameData.i64TotalBet[i];
				}
				m_i64UserScore[m_iNtPeople]-=m_i64UserScore[i];
			}

			__int64 i64ChangeMoney[PLAY_COUNT];
			memset(i64ChangeMoney,0,sizeof(i64ChangeMoney));

			//�������ݿ�
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(m_i64UserScore, temp_cut);

			__super::RecoderGameInfo(i64ChangeMoney);

			GameMessageNormalFinish normalFinish;

			memcpy(normalFinish.iUserPoint,m_iUserPoints,sizeof(m_iUserPoints));
			memcpy(normalFinish.i64UserScore,i64ChangeMoney,sizeof(i64ChangeMoney));
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(NULL==m_pUserInfo[i] || PLAYER_JOIN==m_iPlayerStation[i])
				{
					normalFinish.bIsRuning[i]=-1;
				}
				else
				{
					normalFinish.bIsRuning[i]=1;
				}
			}

			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&normalFinish,sizeof(normalFinish),MDM_GM_GAME_NOTIFY,ASS_NORMALFINISH,0);
				}
			}
			bCloseFlag=GF_NORMAL;
			ReSetGameState(bCloseFlag);
			m_GameData.InitData();
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
		{
			m_bGameStation=GS_WAIT_NEXT;
			KillAllTimer();
			ReSetGameState(bCloseFlag);
			m_GameData.InitData();
			bCloseFlag=GF_SALE;
			GameMessageEnd endMessage;
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&endMessage,sizeof(endMessage),MDM_GM_GAME_NOTIFY,ASS_SAFEEND,0);
				}
			}
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_SAFE_FINISH:
		{
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_FORCE_FINISH:		//�û�ǿ���뿪
		{
			////ǿ�ƶ���
			//MakeUserOffLine(bDeskStation);
			m_bIsCut[bDeskStation]=true;
			if(CountCut()==CountPlayer())
			{
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_bIsCut[i])
					{
						MakeUserOffLine(i);
					}
				}
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i])
					{
						SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_RESTART,0);
					}
				}
				m_bGameStation=GS_WAIT_NEXT;
				KillAllTimer();
				ReSetGameState(0);
				m_GameData.InitData();
			}
			bCloseFlag=GFF_FORCE_FINISH;
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	}

	//��������
	ReSetGameState(bCloseFlag);
	__super::GameFinish(bDeskStation,bCloseFlag);

	return true;
}

bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	OutputDebugString("lw::�û������뿪");
	m_bIsCut[bDeskStation]=true;
	if(CountCut()==CountPlayer())
	{
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_bIsCut[i])
			{
				MakeUserOffLine(i);
			}
		}
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
			{
				SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_RESTART,0);
			}
		}
		m_bGameStation=GS_WAIT_NEXT;
		KillAllTimer();
		ReSetGameState(0);
		m_GameData.InitData();
	}
	//UserAI(bDeskStation, 1, true);
	return __super:: UserNetCut(bDeskStation, pLostUserInfo);
}

//�û���������
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{
	return __super:: UserReCome(bDeskStation, pNewUserInfo);
}
//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	OutputDebugString("lw::�û��뿪��Ϸ��");
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_RESTART,0);
		}
	}
	m_bGameStation=GS_WAIT_NEXT;
	KillAllTimer();
	ReSetGameState(0);
	m_GameData.InitData();
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}
//���AI����
BOOL CServerGameDesk::UserAI(BYTE bDeskStation,BYTE bCutType,bool bNet)
{

	m_bIsCut[bDeskStation] = bNet;
	return TRUE;
}
///�������м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_BET);			
	KillTimer(TIME_SENDCARD);				
	KillTimer(TIME_CALL);			
	KillTimer(TIME_FINISH);		
	KillTimer(TIME_STARTGAME);	
	
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	OutputDebugString("lw::IsPlayGame");
	if(m_bGameStation>GS_WAIT_ARGEE && m_bGameStation<GS_WAIT_NEXT)
	{
		OutputDebugString("lw::IsPlayGame===========true");
		return true;
	}
	else
	{
		OutputDebugString("lw::IsPlayGame===========false");
		return false;
	}
}
//�õ��Ƶ��С
int CServerGameDesk::GetPoints(BYTE bCardList[],int iSize)
{
	BYTE iPoint=0,iTotalPoint=0;
	for(int i=0;i<iSize;i++)
	{
		switch(bCardList[i])
		{
		case 0x01:
		case 0x11:
		case 0x21:
		case 0x31:
			{
				iPoint=2;
				break;
			}
		case 0x02:
		case 0x12:
		case 0x22:
		case 0x32:
			{
				iPoint=3;
				break;
			}
		case 0x03:
		case 0x13:
		case 0x23:
		case 0x33:
			{
				iPoint=4;
				break;
			}
		case 0x04:
		case 0x14:
		case 0x24:
		case 0x34:
			{
				iPoint=5;
				break;
			}
		case 0x05:
		case 0x15:
		case 0x25:
		case 0x35:
			{
				iPoint=6;
				break;
			}
		case 0x06:
		case 0x16:
		case 0x26:
		case 0x36:
			{
				iPoint=7;
				break;
			}
		case 0x07:
		case 0x17:
		case 0x27:
		case 0x37:
			{
				iPoint=8;
				break;
			}
		case 0x08:
		case 0x18:
		case 0x28:
		case 0x38:
			{
				iPoint=9;
				break;
			}
		case 0x09:
		case 0x19:
		case 0x29:
		case 0x39:
			{
				iPoint=10;
				break;
			}
		case 0x0A:
		case 0x1A:
		case 0x2A:
		case 0x3A:
			{
				iPoint=10;
				break;
			}
		case 0x0B:
		case 0x1B:
		case 0x2B:
		case 0x3B:
			{
				iPoint=10;
				break;
			}
		case 0x0C:
		case 0x1C:
		case 0x2C:
		case 0x3C:
			{
				iPoint=10;
				break;
			}
		case 0x0D:
		case 0x1D:
		case 0x2D:
		case 0x3D:
			{
				if(iTotalPoint+11>21)
				{
					iPoint=1;
				}
				else
				{
					iPoint=11;
				}
				break;
			}
		default:
			{
				break;
			}
		}
		iTotalPoint+=iPoint;
		iPoint=0;
	}
	return 	iTotalPoint;
}
//------------------------------------------------------------------------------
//�����û���֤
void	CServerGameDesk::SuperUserExamine(BYTE bDeskStation)
{
	SUPERSTATE TSuperUser;
	if (NULL == m_pUserInfo[bDeskStation])
	{
		return ;
	}
	if (IsSuperUser(bDeskStation))
	{
		// ���û�Ϊ�����û�, ֪ͨ�ͻ���
		TSuperUser.byDeskStation = bDeskStation;
		// �����ͻ���
		TSuperUser.bEnable = true;
		SendGameData(bDeskStation, &TSuperUser, sizeof(TSuperUser), MDM_GM_GAME_NOTIFY, ASS_SUPER_USER, 0);
	}
}
//ѡׯ��
/*
@brief:���ݵ�ǰׯ��˳ʱ��ѡׯ��
@param:��ǰׯ����λ��
@return:���ص�ǰѡ��ׯ��
*/
int CServerGameDesk::HaveNt(BYTE bNtStation)
{
	BYTE k=bNtStation;
	k++;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(k>=PLAY_COUNT)
		{
			k=0;
		}
		if(m_pUserInfo[k] && PLAYER_JOIN!=m_iPlayerStation[k])
		{
			return k;
		}
		k++;
	}
	return -1;
}
//---------------------------------------------------------------------------------------------------
//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
bool	CServerGameDesk::IsSuperUser(BYTE byDeskStation)
{
	if (NULL == m_pUserInfo[byDeskStation])
	{
		return false;
	}
	for(int i=0; i<m_vlSuperID.size(); i++)
	{
		if (NULL != m_pUserInfo[byDeskStation])
		{
			//�Ҵ��˳������
			if (m_pUserInfo[byDeskStation]->m_UserData.dwUserID == m_vlSuperID.at(i))
			{
				return true;
			}
		}
	}
	return false;
}