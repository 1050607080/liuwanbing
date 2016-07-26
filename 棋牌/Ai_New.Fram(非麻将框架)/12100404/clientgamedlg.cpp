#include "StdAfx.h"
#include "ClientGamedlg.h"

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	ResetGameData();
	LoadGameIni();
} 

//��������
CClientGameDlg::~CClientGameDlg()
{

}

//---------------------------------------------------------------------------------------
///��ʼ����������
void	CClientGameDlg::ResetGameData()
{

	m_byGameStation=GS_WAIT_ARGEE;
	m_bNtPeople=255;
	m_i64CurrentNote=0;
	m_bScorePeople=255;
	memset(m_i64UserBet,0,sizeof(m_i64UserBet));			//�û�����ע
	memset(m_iUserAlreadySend,0,sizeof(m_iUserAlreadySend));
	memset(m_i64UserCanBet,0,sizeof(m_i64UserCanBet));
	mi64UserMaxBet=0;	
	KillAllTimer();
}
//���������ļ�
void CClientGameDlg::LoadGameIni()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();///����·��
	CINIFile f( s +nid +"_robot.ini");
	CString key = TEXT("robot");
}
//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClass::OnInitDialog();
	return TRUE;
}
//------------------------------------------------------------------------------------------------------------------
//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	//��ȡ�Լ���λ��
	m_iMyDeskStation = GetMeUserInfo()->bDeskStation;
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			if(uDataSize!=sizeof(GameStation_1))
			{
				return false;
			}

			GameStation_1 *gameStation_1=(GameStation_1 *)pStationData;
			if(NULL==gameStation_1)
			{
				return false;
			}

			m_bGameBeginWaitTime=gameStation_1->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_1->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_1->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_1->bGameStation;											//��Ϸ״̬
			

			//��ʼ�ȴ���ʱ��
			srand((unsigned)time(0));
			int iTime=(rand()%5)+3;
			SetGameTimer(m_iMyDeskStation, iTime, ID_BEGIN_TIME);

			break; 
		}
	case GS_BET:	//��ע״̬
		{
			if(uDataSize!=sizeof(GameStation_2))
			{
				return false;
			}

			GameStation_2 *gameStation_2=(GameStation_2 *)pStationData;
			if(NULL==gameStation_2)
			{
				return false;
			}

			m_bGameBeginWaitTime=gameStation_2->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_2->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_2->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_2->bGameStation;											//��Ϸ״̬
			break;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			if(uDataSize!=sizeof(GameStation_3))
			{
				return false;
			}

			GameStation_3 *gameStation_3=(GameStation_3 *)pStationData;
			if(NULL==gameStation_3)
			{
				return false;
			}
			m_bGameBeginWaitTime=gameStation_3->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_3->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_3->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_3->bGameStation;											//��Ϸ״̬
			break;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			if(uDataSize!=sizeof(GameStation_4))
			{
				return false;
			}

			GameStation_4 *gameStation_4=(GameStation_4 *)pStationData;
			if(NULL==gameStation_4)
			{
				return false;
			}
			m_bGameBeginWaitTime=gameStation_4->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_4->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_4->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_4->bGameStation;											//��Ϸ״̬

			//��ʼ�ȴ���ʱ��
			srand((unsigned)time(0));
			int iTime=(rand()%5)+3;
			SetGameTimer(m_iMyDeskStation, iTime, ID_WAIT_NEXT);
			break;
		}
	default:
		{
			break;
		}
	}
	return true;
}
//------------------------------------------------------------------------------------------------------------------
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
				break;        
			}
		case ASS_AGREE:				//ͬ����Ϸ��Ϣ
			{
				if(uDataSize!=sizeof(AgreeGame))
				{
					return 0;
				}
				OnHandleAgreeGame(pNetData);
				break;
			}
		case ASS_NT:				//��ׯ��
			{
				if(uDataSize!=sizeof(GameMessageNt))
				{
					return 0;
				}
				OnHandleNt(pNetData);
				break;
			}

		case ASS_BET:				//������ע��Ϣ
			{
				if(uDataSize!=sizeof(GameMessageBet))
				{
					return 0;
				}
				OnHandleBet(pNetData);

				break;
			}
		case ASS_SEND:					//��ʼ����
			{
				if(uDataSize!=sizeof(GameMessageSendCard))
				{
					return 0;
				}
				OnHandleSendCard(pNetData);
				break;
			}
		case ASS_PLAY:						//��ʼ��Ϸ
			{
				if(uDataSize!=sizeof(GameMessageStart))
				{
					return 0;
				}

				OnHandlePlay(pNetData);
				break;
			}
		case ASS_SAFEEND:					//��Ϸ��ȫ����
			{
				if(uDataSize!=sizeof(GameMessageEnd))
				{
					return 0;
				}
				OnHandleSafeFinish(pNetData);
				break;
			}
		case ASS_BETRESULT:				//��ע�����Ϣ
			{

				if(uDataSize!=sizeof(GameMessageBetResult))
				{

					return 0;
				}
				OnHandleBetResult(pNetData);
				break;
			}
		case ASS_CALL:				//�з���Ϣ
			{
				if(uDataSize!=sizeof(GameMessageNeedCard))
				{

					return 0;
				}
				//����������������Ľ�����Ϣ
				OnHandleCall(pNetData);
				break;
			}
		case ASS_RESTART:		//��Ϸ���¿�ʼ
			{
				ResetGameData();
				KillAllTimer();
				//��ʼ�ȴ���ʱ��
				srand((unsigned)time(0));
				int iTime=(rand()%5)+3;
				SetGameTimer(m_iMyDeskStation, iTime, ID_BEGIN_TIME);
				break;
			}
		case ASS_NORMALFINISH:			//��Ϸ��������
			{
				if(uDataSize!=sizeof(GameMessageNormalFinish))
				{

					return 0;
				}

				OnHandleNormalFinish(pNetData);
				break;
			}
		case ASS_SUPER_USER:	//���˿ͻ���
			{  
				if(uDataSize!=sizeof(SUPERSTATE))
				{
					return 0;
				}	
				HandleSuperMessage(pNetData);			
				break;
			}
		default:
			{
				break;
			}
		}
	}

	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}


/*
@brief:���������ͬ����Ϸ��Ϣ
*/
void CClientGameDlg::OnHandleAgreeGame(void *buffer)
{
	AgreeGame *agreeGame=(AgreeGame *)buffer;
	if(NULL==agreeGame)
	{
		return;
	}
}
/*
@brief:�����������������ȷ��ׯ����Ϣ
*/
void CClientGameDlg::OnHandleNt(void *buffer)
{
	GameMessageNt *haveNt=(GameMessageNt *)buffer;
	if(NULL==haveNt)
	{
		return;
	}
	m_bNtPeople=haveNt->bNtStation;
}
/*
@brief:�������������������ע��Ϣ
*/
void CClientGameDlg::OnHandleBet(void *buffer)
{
	GameMessageBet *haveBet=(GameMessageBet *)buffer;
	if(NULL==haveBet)
	{
		return;
	}
	SetStationParameter(GS_BET);
	if(GS_BET!=GetStationParameter())
	{
		return;
	}

	mi64UserMaxBet=haveBet->i64BetLimit[m_iMyDeskStation];
	for(int i=0;i<3;i++)
	{
		m_i64UserCanBet[i]=haveBet->i64Bet[m_iMyDeskStation][i];
	}

	//������ע��ʱ��
	srand((unsigned)time(0));
	int iTime=(rand()%4)+3;
	SetGameTimer(m_iMyDeskStation, iTime, ID_NOTE);

}
/*
@brief:����������������ķ�����Ϣ
*/
void CClientGameDlg::OnHandleSendCard(void *buffer)
{
	
	GameMessageSendCard *sendCard=(GameMessageSendCard *)buffer;
	if(NULL==sendCard)
	{
		return;
	}
	KillAllTimer();
	SetStationParameter(GS_SEND);
	
	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		
		if(m_pUserInfo[i])
		{
			memcpy(m_iUserAlreadySend[i],sendCard->bUserCard[i],sizeof(BYTE)*2);
		}
	}

}

/*
@brief:�������������������Ϸ��ȫ������Ϣ
*/		
void CClientGameDlg::OnHandleSafeFinish(void *buffer)
{
	GameMessageEnd *safeFinishGame=(GameMessageEnd *)buffer;
	if(NULL==safeFinishGame)
	{
		return;
	}
	KillAllTimer();
	ResetGameData();
	SetStationParameter(GS_WAIT_NEXT);
	
	//��ʼ�ȴ���ʱ��
	srand((unsigned)time(0));
	int iTime=(rand()%5)+3;
	SetGameTimer(m_iMyDeskStation, iTime, ID_WAIT_NEXT);
	
}
/*
@brief:�������������������ע��Ϣ
*/
void CClientGameDlg::OnHandleBetResult(void *buffer)
{
	GameMessageBetResult *betResult=(GameMessageBetResult *)buffer;
	if(NULL==betResult)
	{
		
		return;
	}
	m_i64UserBet[betResult->bDeskStation]+=betResult->i64Note;
}
/*
@brief:����������������Ŀ�ʼ��Ϸ��Ϣ
*/
void CClientGameDlg::OnHandlePlay(void *buffer)
{
	GameMessageStart *startGame=(GameMessageStart *)buffer;
	if(NULL==startGame)
	{
		return;
	}

	KillAllTimer();
	SetStationParameter(GS_PLAY_GAME);
	
	//code:����У��
	if(startGame->bFirstScoreStation>=0 && startGame->bFirstScoreStation<PLAY_COUNT)
	{
		m_bScorePeople=startGame->bFirstScoreStation;

		if(m_iMyDeskStation==startGame->bFirstScoreStation && m_bNtPeople!=m_iMyDeskStation)
		{
			//�����зֶ�ʱ��
			srand((unsigned)time(0));
			int iTime=(rand()%3)+2;
			SetGameTimer(m_iMyDeskStation, iTime, ID_OUT_CARD);
		}
	}
}
/*
@brief:����������������Ľ�����Ϣ
*/
void CClientGameDlg::OnHandleCall(void *buffer)
{
	GameMessageNeedCard *callCard=(GameMessageNeedCard *)buffer;
	if(NULL==callCard)
	{
		return;
	}
	//code:�����ж�
	if(SERVER_EXTRA_DOUBLE==callCard->iStyle)			//�ӱ�
	{
		//code:����У��
		if(callCard->bCard>=0)
		{
			//ɱ�����м�ʱ��
			KillAllTimer();
			m_bScorePeople=callCard->bNextStation;

			for(int i=0;i<13;i++)
			{
				if(m_iUserAlreadySend[callCard->bDeskStation][i]<=0)
				{
					m_iUserAlreadySend[callCard->bDeskStation][i]=callCard->bCard;
					break;
				}
			}
			if(callCard->i64Note>0)
			{
				m_i64UserBet[callCard->bDeskStation]+=callCard->i64Note;

			}
			if(255!=callCard->bNextStation)
			{
				if(m_iMyDeskStation==callCard->bNextStation)
				{
					//�����зֶ�ʱ��
					srand((unsigned)time(0));
					int iTime=(rand()%3)+2;
					SetGameTimer(m_iMyDeskStation, iTime, ID_OUT_CARD);
				}
			}
			if(m_iMyDeskStation==callCard->bDeskStation)
			{
				;
			}
		}
		
	}
	else if(SERVER_EXTRA_CLICK_GIVEUP==callCard->iStyle)			//����
	{
		if(callCard->bNextStation>=0 && callCard->bNextStation<=PLAY_COUNT)
		{
			//ɱ�����м�ʱ��
			KillAllTimer();
			m_bScorePeople=callCard->bNextStation;

			if(m_iMyDeskStation==callCard->bNextStation)
			{
				//�����зֶ�ʱ��
				srand((unsigned)time(0));
				int iTime=(rand()%3)+2;
				SetGameTimer(m_iMyDeskStation, iTime, ID_OUT_CARD);
			}
		}
	}
	else if(SERVER_EXTRA_CLICK_NEED==callCard->iStyle)				//Ҫ��
	{
		if(callCard->bCard>=0)
		{
			//ɱ�����м�ʱ��
			KillAllTimer();
			for(int i=0;i<13;i++)
			{
				if(m_iUserAlreadySend[callCard->bDeskStation][i]<=0)
				{
					m_iUserAlreadySend[callCard->bDeskStation][i]=callCard->bCard;
					break;
				}
			}
			if(m_iMyDeskStation==callCard->bDeskStation)
			{
				int iCount=0;
				for(int i=0;i<13;i++)
				{
					if(m_iUserAlreadySend[m_iMyDeskStation][i]>0)
					{
						iCount++;
					}
				}
				if(iCount>=21 || iCount<=0)
				{
					//���ܽз�
				}
				else
				{
					//�����зֶ�ʱ��
					srand((unsigned)time(0));
					int iTime=(rand()%3)+2;
					SetGameTimer(m_iMyDeskStation, iTime, ID_OUT_CARD);
					m_bScorePeople=m_iMyDeskStation;
				}
			}
			if(255!=callCard->bNextStation)
			{
				if(m_iMyDeskStation==callCard->bNextStation)
				{
					//�����зֶ�ʱ��
					srand((unsigned)time(0));
					int iTime=(rand()%3)+2;
					SetGameTimer(m_iMyDeskStation, iTime, ID_OUT_CARD);
				}
			}
		}
	}
	else if(SERVER_EXTRA_CLICK_STOP==callCard->iStyle)				//ͣ��
	{
		//ɱ�����м�ʱ��
		KillAllTimer();
		m_bScorePeople=callCard->bNextStation;

		if(m_iMyDeskStation==callCard->bNextStation)
		{
			//�����зֶ�ʱ��
			srand((unsigned)time(0));
			int iTime=(rand()%3)+2;
			SetGameTimer(m_iMyDeskStation, iTime, ID_OUT_CARD);
		}
		if(m_iMyDeskStation==callCard->bDeskStation)
		{
			;
		}
	}
	else if(SERVER_EXTRA_CLICK_TIMEEND==callCard->iStyle)	//ʱ�䵽��һ���˽з�
	{
		
		//ɱ�����м�ʱ��
		KillAllTimer();

		if(255!=callCard->bNextStation && callCard->bFlag)
		{
			
			m_bScorePeople=callCard->bNextStation;
			if(m_iMyDeskStation==callCard->bNextStation)
			{
				//�����зֶ�ʱ��
				srand((unsigned)time(0));
				int iTime=(rand()%3)+2;
				SetGameTimer(m_iMyDeskStation, iTime, ID_OUT_CARD);
			}
		}
	}
}
//�������������������Ϸ����������Ϣ
void CClientGameDlg::OnHandleNormalFinish(void *buffer)
{
	GameMessageNormalFinish *normalFinish=(GameMessageNormalFinish *)buffer;
	if(NULL==normalFinish)
	{
		return;
	}
	
	KillAllTimer();
	ResetGameData();
	SetStationParameter(GS_WAIT_NEXT);

	//��ʼ�ȴ���ʱ��
	srand((unsigned)time(0));
	int iTime=(rand()%5)+3;
	SetGameTimer(m_iMyDeskStation, iTime, ID_WAIT_NEXT);
	
}
//����������������ĳ����û���Ϣ
void CClientGameDlg::HandleSuperMessage(void *buffer)
{
	;
}
//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	return __super::ResetGameFrame();
}

//��ʼ��ť����
LRESULT	CClientGameDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{
	OnControlHitBegin();
	return 0;
}

bool CClientGameDlg::OnControlHitBegin()
{
	return true;
}
//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
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
	OutputDebugString("test �ܷ��뿪����");
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

///ԭ�ȵ���Ϊ��λ�����Գ�����ע���ٶȣ�����д��Ϊ��Ϊ��λ
bool CClientGameDlg::SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUserMilliSecond)
{
	int iTimeType = 1000 ; 

	if(m_pGameRoom != NULL)
	{
		m_pGameRoom->SendMessage(IDM_SET_GAME_TIME,(WPARAM)uTimeID,(LPARAM)uTimeCount*iTimeType);
	}
	return true;
}
//------------------------------------------------------------------------------------------------------------------
//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	
		{
			KillGameTimer(ID_BEGIN_TIME);
			KillAllTimer();
			//���Ϳ�ʼ��Ϸ��Ϣ
			GameStationButton startMessage;
			startMessage.bDeskStation=m_iMyDeskStation;
			startMessage.iBtType=CLICK_START;

			SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);

			return true;
		}
	case ID_WAIT_NEXT:	
		{
			KillGameTimer(ID_WAIT_NEXT);
			KillAllTimer();
			//���Ϳ�ʼ��Ϸ��Ϣ
			GameStationButton startMessage;
			startMessage.bDeskStation=m_iMyDeskStation;
			startMessage.iBtType=CLICK_START;

			SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
			return true;
		}
	case ID_NOTE:
		{
			KillGameTimer(ID_NOTE);

			srand((unsigned)time(0));
			int iRandom=rand()%3;


			m_i64CurrentNote=m_i64UserCanBet[iRandom];
			//������ע��Ϣ
			if(m_i64CurrentNote<=0)
			{
				return true;
			}
			GameStationButton betMessage;
			betMessage.bDeskStation=m_iMyDeskStation;
			betMessage.iBtType=CLICK_BETOPERATE;
			betMessage.i64Note=m_i64CurrentNote;
			SendGameData(&betMessage, sizeof(betMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
			return true;
		}
	case ID_OUT_CARD:
		{
			KillGameTimer(ID_OUT_CARD);
			int iRandom=rand()%100;
			if(iRandom>=0 && iRandom<40)//�ӱ�
			{
				if(m_bScorePeople==m_iMyDeskStation)
				{
					if(m_bNtPeople==m_iMyDeskStation)
					{
						return true;
					}
					if(m_pUserInfo[m_iMyDeskStation])
					{
						if(m_pUserInfo[m_iMyDeskStation]->GameUserInfo.i64Money<(m_i64UserBet[m_iMyDeskStation]*2))
						{
							return true;
						}
					}
					GameStationButton operateMessage;
					operateMessage.bDeskStation=m_iMyDeskStation;
					operateMessage.iBtType=CLICK_PLAYOPERATE;
					operateMessage.iBtParam=1;
					operateMessage.i64Note=m_i64UserBet[m_iMyDeskStation];
					SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
				}

			}
			else if(iRandom>=40 && iRandom<80)	//Ҫ��
			{
				if(m_bScorePeople==m_iMyDeskStation)
				{
					GameStationButton operateMessage;
					operateMessage.bDeskStation=m_iMyDeskStation;
					operateMessage.iBtType=CLICK_PLAYOPERATE;
					operateMessage.iBtParam=3;
					SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
				}
			}
			else if(iRandom>=80 && iRandom<90)		//����
			{
				if(m_bScorePeople==m_iMyDeskStation)
				{
					GameStationButton operateMessage;
					operateMessage.bDeskStation=m_iMyDeskStation;
					operateMessage.iBtType=CLICK_PLAYOPERATE;
					operateMessage.iBtParam=2;
					SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
				}
			}
			else if(iRandom>=90 && iRandom<100)		//ͣ��
			{
				if(m_bScorePeople==m_iMyDeskStation)
				{
					GameStationButton btOperate;
					btOperate.bDeskStation=m_iMyDeskStation;
					btOperate.iBtType=CLICK_PLAYOPERATE;
					btOperate.iBtParam=5;	//��ʾ��Ҫ��
					SendGameData(&btOperate, sizeof(btOperate) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
				}
			}
			return true;
		}
	}

	return true;
}
//������ж�ʱ��
void	CClientGameDlg::KillAllTimer()
{
	KillGameTimer(ID_BEGIN_TIME);
	KillGameTimer(ID_WAIT_NEXT);
	KillGameTimer(ID_NOTE);
	KillGameTimer(ID_OUT_CARD);
	return;
}