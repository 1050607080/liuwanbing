#include "StdAfx.h"
#include "ClientGamedlg.h"

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	m_iBeginTime	= 0;				/// ��ʼʱ��(s)
	m_iThinkTime	= 0;				/// ����˼��ʱ��(s)
	m_iMyDeskStation=255;
	ResetGameStation(0);
} 
//��������
CClientGameDlg::~CClientGameDlg()
{

}

int CClientGameDlg::GetGameStatus()
{
	if (this)
	{
		return GetStationParameter();
	}
	else
	{
		return 0;
	}    
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClass::OnInitDialog();
	return TRUE;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	//��ȡ�Լ���λ��
	m_iMyDeskStation = GetMeUserInfo()->bDeskStation;
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:
		{	
			GameStation *TGameStation = (GameStation *)pStationData;
			if(NULL == TGameStation)
			{
				return false;
			}
			//��������
			m_iThinkTime=TGameStation->iThinkTime;
			m_iBeginTime=TGameStation->iBeginTime;

			SetStationParameter(TGameStation->iGameStation);
			//�ȴ�ʱ��
			srand((unsigned)time(0));
			int iRandBeginTime=0;
			iRandBeginTime = rand()%5 + 1 ; 
			
			SetGameTimer(GetMeUserInfo()->bDeskStation , iRandBeginTime,ID_BEGIN_TIME) ; 

			return true;
		}
	case GS_SEND:		//��Ϸ��ʼ״̬
	case GS_PLAY_GAME:
		{
			GameStation_0 *TGameStation = (GameStation_0 *)pStationData;
			if(NULL == TGameStation)
			{
				return false;
			}
			//��������
			m_iThinkTime=TGameStation->iThinkTime;
			m_iBeginTime=TGameStation->iBeginTime;
			SetStationParameter(TGameStation->iGameStation);
			memcpy(m_bOwnCard,TGameStation->bCardList,sizeof(m_bOwnCard));

			int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
			m_Logic.SortCard(m_bOwnCard,NULL,iHandCount);
			if(m_Logic.TestData(TGameStation->bNextPeople,0))
			{
				m_bCurrentCall=TGameStation->bNextPeople;	//��ǰ�з���			
			}
			if(TGameStation->iClockLabel>0)
			{
				//�����з�ʱ��
				if(m_Logic.TestData(m_bCurrentCall,0) && m_iMyDeskStation==m_bCurrentCall)
				{
					srand((unsigned) time (0)) ; 
					int iRandBeginTime=0;
					
					iRandBeginTime = rand()%5 + 1 ; 
					
					SetGameTimer(GetMeUserInfo()->bDeskStation , iRandBeginTime,ID_SCORE_TIME) ; 

				}
			}
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{

	if (pNetHead->bMainID == MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_GM_GAME_STATION:   ///<����״̬(ƽ̨���õ�)
			{
				SetGameStation(pNetData, uDataSize);    
				return true;       
			}
		case SERVERTOCLIENT_AGREE:			//ͬ����Ϸ
			{
				if(uDataSize!=sizeof(MSG_GR_R_UserAgree))
				{
					return 0;
				}	
				OnAgreeGame(pNetData);
				break;
			}
		case SERVERTOCLIENT_SEND://��ʼ����
			{
				if(uDataSize != sizeof(GameServerToClient_SendCard))
				{
					return FALSE;
				}
				OnHandleSendCardBegin(pNetData);
				break;
			}
		case SERVERTOCLIENT_START:	//��ʼ��Ϸ
			{
				if(uDataSize != sizeof(GameServerToClient_StartGame))
				{
					return FALSE;
				}
				OnHandleGameBegin(pNetData);
				break;
			}
		case SERVERTOCLIENT_NEXT://��һ������
			{
				
				if(uDataSize != sizeof(GameServerToClient_NextPeople))
				{
					return FALSE;
				}
				
				OnHandleCall(pNetData);
				break;
			}
		case SERVERTOCLIENT_SHOW://��ʾ�÷���
			{
				if(uDataSize != sizeof(GameServerToClient_StyleCard))
				{
					return FALSE;
				}

				OnHandleShowStyle(pNetData);
				break;
			}
		case SERVERTOCLIENT_NORMALFINISH://��Ϸ����
			{
				if(uDataSize != sizeof(GameServerToClient_Finish))
				{
					return FALSE;
				}
				OnHandleNormalFinish(pNetData);
				break;
			}
		case SERVERTOCLIENT_AUTO://�й�
			{
				if(uDataSize != sizeof(GameServerToClient_Auto))
				{
					return FALSE;
				}
				OnHandleAuto(pNetData);
				break;
			}
		}
	}

	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	return __super::ResetGameFrame();
}


bool CClientGameDlg::OnControlHitBegin()
{
	return true;
}

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillGameTimer(ID_BEGIN_TIME);
	KillGameTimer(ID_SCORE_TIME);
	return;
}
//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME://��ʼ�ȴ�
		{
			KillAllTimer();
			ResetGameStation(0);
			GameClientToServer_Button startMessage;
			startMessage.bDeskStation=m_iMyDeskStation;
			startMessage.iBtType=CLICK_START;	
			SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_AGREE,0);	
			return  true ; 
		}		
	case ID_SCORE_TIME://����
		{
			KillGameTimer(ID_SCORE_TIME);
			RobotOutCard();
			return true;
		}
	}
	return true;
}

//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	memset(m_bOwnCard,0,sizeof(m_bOwnCard));
	memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_DeskCard,0,sizeof(m_DeskCard));
	memset(m_bLeftCard,0,sizeof(m_bLeftCard));
	m_bCurrentCall=255;
	m_bIsOutCard=false;
	m_bNtPeople=255;										//ׯ��
	memset(m_bUserPrepare,0,sizeof(m_bUserPrepare));		//���׼��
	return ;
}

//��ʱ��
void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	CLoveSendClass::OnTimer(nIDEvent);
}

//�ܷ��뿪����,�����,���ڿͻ����յ���Ϣ�������뿪.
BOOL CClientGameDlg::CanLeftDesk()
{
	return TRUE;
}

//�˳�
void CClientGameDlg::OnCancel()
{
	KillAllTimer();
	AFCCloseFrame();
	__super::OnCancel();
}

//��������
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{
	return TRUE;
}

//���Ż��㵽��ͼλ��
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{
	return __super::ViewStation(bDeskStation);
}

BOOL CClientGameDlg::PreTranslateMessage(MSG* pMsg)
{
	return CLoveSendClass::PreTranslateMessage(pMsg);
}

//����ͬ����Ϸ��Ϣ
void CClientGameDlg::OnAgreeGame(void *buffer)
{
	GameServerToClient_ArgeeGame * agreeGame=(GameServerToClient_ArgeeGame *)buffer;
	if(NULL==agreeGame)
	{
		return ;
	}
	m_bUserPrepare[agreeGame->bDeskStation]=1;
	if(m_iMyDeskStation==agreeGame->bDeskStation)
	{
		KillGameTimer(ID_BEGIN_TIME);
	}
}
/*
���ܣ��������˷������ķ�����Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CClientGameDlg::OnHandleSendCardBegin(void *buffer)
{
	GameServerToClient_SendCard *sendAllCard = (GameServerToClient_SendCard *)buffer ;
	if(NULL==sendAllCard)
	{
		return;
	}
	
	SetStationParameter(GS_SEND);
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//����������
		//ÿ�������˿˵���Ŀ
		m_iDeskCardCount[i]=sendAllCard->iUserCardCount[i];	
		//ÿ��������˿�
		memcpy(m_DeskCard[i],sendAllCard->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
	}
	
	if(m_Logic.TestData(sendAllCard->bNtPeople,0))
	{
		m_bNtPeople=sendAllCard->bNtPeople;							//ׯ��
	}
	memcpy(m_bOwnCard,m_DeskCard[m_iMyDeskStation],sizeof(BYTE)*m_iDeskCardCount[m_iMyDeskStation]);
	m_Logic.SortCard(m_bOwnCard,NULL,m_iDeskCardCount[m_iMyDeskStation]);
	
}
/*
���ܣ��������˷���������Ϸ��ʼ��Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CClientGameDlg::OnHandleGameBegin(void *buffer)
{
	GameServerToClient_StartGame *gameBegin = (GameServerToClient_StartGame *)buffer ;
	if(NULL==gameBegin)
	{
		return;
	}

	SetStationParameter(GS_PLAY_GAME);
	if(!m_Logic.TestData(gameBegin->iOutDeskStation,0))
	{
		return ;
	}
	
	m_bCurrentCall=gameBegin->iOutDeskStation;
	if(m_iMyDeskStation==gameBegin->iOutDeskStation)
	{
		//����ʱ��
		srand((unsigned)time(0)) ; 
		int iRandBeginTime=0;
		iRandBeginTime = rand()%5 + 1 ; 
		SetGameTimer(GetMeUserInfo()->bDeskStation , iRandBeginTime,ID_SCORE_TIME) ; 
	}
	
}
/*
@brief:����������������Ľ�����Ϣ
*/
void CClientGameDlg::OnHandleCall(void *buffer)
{
	GameServerToClient_NextPeople *callCard=(GameServerToClient_NextPeople *)buffer;
	if(NULL==callCard)
	{
		return;
	}
	
	if(!m_Logic.TestData(callCard->bNextStation,0))
	{
		return ;
	}
	
	m_bCurrentCall=callCard->bNextStation;
	if(m_iMyDeskStation==callCard->bNextStation)
	{
		
		//�����ϼҳ�������
		memset(m_bLeftCard,0,sizeof(m_bLeftCard));
		int iLeftTemp=m_Logic.CountUserCard(callCard->bCardList,CARD_NUMS);//Ҫѹ���Ƶ�����
		if(iLeftTemp<=0)//û����Լ��
		{
			
			memset(m_bLeftCard,0,sizeof(m_bLeftCard));
			
		}
		else
		{
			
			memcpy(m_bLeftCard,callCard->bCardList,sizeof(BYTE)*CARD_NUMS);
		}
		//����ʱ��
		srand((unsigned)time(0)) ; 
		int iRandBeginTime=0;

		iRandBeginTime = rand()%5 + 1 ; 

		SetGameTimer(GetMeUserInfo()->bDeskStation , iRandBeginTime,ID_SCORE_TIME) ; 
	}

}
//��ʾ�÷���
void CClientGameDlg::OnHandleShowStyle(void *buffer)
{
	GameServerToClient_StyleCard *showStyle=(GameServerToClient_StyleCard *)buffer;
	if(NULL==showStyle)
	{
		return;
	}
}
//�������������������Ϸ����������Ϣ
void CClientGameDlg::OnHandleNormalFinish(void *buffer)
{
	GameServerToClient_Finish *normalFinish=(GameServerToClient_Finish *)buffer;
	if(NULL==normalFinish)
	{
		return;
	}
	//ɾ�����ж�ʱ��
	KillAllTimer();
	SetStationParameter(GS_WAIT_NEXT);
	ResetGameStation(0);
	
	//�ȴ�ʱ��
	srand((unsigned)time(0)) ; 
	int iRandBeginTime=0;
	
	iRandBeginTime = rand()%5 + 1 ; 
	
	SetGameTimer(GetMeUserInfo()->bDeskStation , iRandBeginTime,ID_BEGIN_TIME) ; 
}
//�������������������Ϸ�й���Ϣ
void CClientGameDlg::OnHandleAuto(void *buffer)
{
	GameServerToClient_Auto *userAuto=(GameServerToClient_Auto *)buffer;
	if(NULL==userAuto)
	{
		return;
	}

}

//ɾ���ƶ�����
void CClientGameDlg::RemoveCard(BYTE bCardList[],BYTE bTarCard,int iCount)
{
	for(int i=0;i<iCount;i++)
	{
		if(bTarCard==bCardList[i])
		{
			bCardList[i]=0;
			continue;
		}
	}
}
//����������
void CClientGameDlg::RestartSort(BYTE bCardList[],int iCount)
{
	BYTE bTempList[CARD_NUMS]={0};
	for(int i=0,j=0;i<iCount;i++)
	{
		if(m_Logic.TestData(bCardList[i],1))
		{
			bTempList[j]=bCardList[i];
			j++;
		}
	}
	memset(bCardList,0,sizeof(BYTE)*iCount);
	memcpy(bCardList,bTempList,sizeof(BYTE)*iCount);
}
//�������ж��Ƿ����
void CClientGameDlg::RobotOutCard()
{
	BYTE iResultCard[CARD_NUMS]={0};
	int iResultCardCount=0;
	int iLeftTemp=m_Logic.CountUserCard(m_bLeftCard,CARD_NUMS);//Ҫѹ���Ƶ�����
	int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
	
	if(iLeftTemp>0)//��ҪѺ����
	{
		m_Logic.TackOutCardMoreThanLast(m_bOwnCard,iHandCount,m_bLeftCard,iLeftTemp,iResultCard,iResultCardCount);
		if(iResultCardCount>0)//�бȵ�ǰ�����
		{
			
			//����
			//��Ҫ�����ƴ���������ɾ����
			for(int i=0;i<iResultCardCount;i++)
			{
				if(m_Logic.TestData(iResultCard[i],1))
				{
					//����������ɾ���������
					RemoveCard(m_bOwnCard,iResultCard[i],CARD_NUMS);
				}
			}
			//�����˿�����
			RestartSort(m_bOwnCard,CARD_NUMS);
			GameClientToServer_Button startMessage;
			startMessage.bFlag=true;
			startMessage.bDeskStation=m_iMyDeskStation;
			startMessage.iBtType=CLICK_OUTCARD;

			memcpy(startMessage.bCardList,iResultCard,sizeof(BYTE)*iResultCardCount);
			startMessage.iCardCount=iResultCardCount;
			SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);	
		}
		else//û�бȵ�ǰ�����
		{
			
			//����
			GameClientToServer_Button startMessage;
			startMessage.bFlag=true;
			startMessage.bDeskStation=m_iMyDeskStation;
			startMessage.iBtType=CLICK_NOOUT;
			SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);	
		}
	}
	else
	{
		
		//����
		//���һ����
		BYTE iResultCard[CARD_NUMS]={0};
		int iResultCardCount=0;
		m_Logic.AutoOutCard(m_bOwnCard,iHandCount,NULL,0,iResultCard,iResultCardCount,true);
		if(iResultCardCount>0)
		{
			//��Ҫ�����ƴ���������ɾ����
			for(int i=0;i<iResultCardCount;i++)
			{
				if(m_Logic.TestData(iResultCard[i],1))
				{
					//����������ɾ���������
					RemoveCard(m_bOwnCard,iResultCard[i],CARD_NUMS);
				}
			}
			//�����˿�����
			RestartSort(m_bOwnCard,CARD_NUMS);
			GameClientToServer_Button startMessage;
			startMessage.bFlag=true;
			startMessage.bDeskStation=m_iMyDeskStation;
			startMessage.iBtType=CLICK_OUTCARD;

			memcpy(startMessage.bCardList,iResultCard,sizeof(BYTE)*iResultCardCount);
			startMessage.iCardCount=iResultCardCount;
			SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);	
		}
	}
}