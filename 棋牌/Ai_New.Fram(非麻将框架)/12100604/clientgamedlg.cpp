#include "StdAfx.h"
#include "ClientGameDlg.h"
#include ".\clientgamedlg.h"


BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
		


	ON_WM_TIMER()
	//	ON_WM_CLOSE()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	m_iThinkTime=0;
	m_iCardCount=0;
	m_iNowOutPeople=0;
	m_iFirstOutPeople = 0;
	//m_PlayView.SetNTStation();
	m_bTimeOutCount=0;
	
	//ϵͳ������
	//m_iSysCardCount=28;
	m_iNowBigNote=0;				//��ǰ���Ѻע��
	m_iThisTurnLimit=0;				//������ע
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	//���������
	::memset(m_UserCard,255,sizeof(m_UserCard));
	::memset(m_iUpBullCard,1,sizeof(m_iUpBullCard));
	m_bBgSound=true;




	m_iGameTimes = 0;

	CINIFile iniFile(CINIFile::GetAppPath()+"NNRobotSet.ini");

	m_bLeftOrGoOn = iniFile.GetKeyVal("RobotSet", "LeftOrGoOn",1);

	m_byGameTimes = iniFile.GetKeyVal("RobotSet", "GameTimes",1);

	m_byLeaveThinkTime = iniFile.GetKeyVal("RobotSet", "LeaveThinkTime",10);

	CString strTemp;
	for(int i = 0; i < 4; ++i)
	{
		strTemp.Format("ProNote[%d]",i);
		m_iProNote[i] = iniFile.GetKeyVal("RobotSet",strTemp,25);
	}

}

//��������
CClientGameDlg::~CClientGameDlg()
{

}

//���ݰ󶨺���
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClass::DoDataExchange(pDX);
//	DoDataExchangeWebVirFace(pDX);
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

	GameStation_Base *pGameStation = (GameStation_Base *)pStationData;
	if (NULL == pGameStation)
	{
		return TRUE;
	}
	SetStationParameter(pGameStation->byGameStation);

	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:			//�ȴ���һ�̿�ʼ
		{	
			GameStation_WaiteAgree *pGameStation = (GameStation_WaiteAgree *)pStationData;
			if(NULL == pGameStation)
			{
				return TRUE;
			}
			//��������
			m_Logic.SetCardShape(pGameStation->iCardShape);
			//��������
			m_iThinkTime	= pGameStation->iThinkTime;
			m_iBeginTime	= pGameStation->iBeginTime;
			m_iCallScoreTime= pGameStation->iCallScoreTime;
			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
			m_iPalyCardCount= pGameStation->iAllCardCount;
			m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��

			int iTime=1;
			if ((m_iBeginTime ) > 10)
			{
				iTime = ((rand()+GetMeUserInfo()->bDeskStation)%(10))+1;
			}
			else if (m_iBeginTime > 0)
			{
				iTime = ((rand()+GetMeUserInfo()->bDeskStation)%(m_iBeginTime))+1;
			}
			//������Ŷӳ��Ͳ���ȥ���׼����
			if (!(m_pGameInfo->dwRoomRule & GRR_QUEUE_GAME))
			{
				SetGameTimer(GetMeUserInfo()->bDeskStation,(iTime+1),ID_BEGIN_TIME);
			}

			return TRUE;
		}
	case GS_ROB_NT:		//��ׯ״̬
		{
			GameStation_RobNt *pGameStation = (GameStation_RobNt *)pStationData;
			if(NULL == pGameStation)
			{
				return TRUE;
			}
			//��������
			m_Logic.SetCardShape(pGameStation->iCardShape);
			//��������
			m_iThinkTime	= pGameStation->iThinkTime;
			m_iBeginTime	= pGameStation->iBeginTime;
			m_iCallScoreTime= pGameStation->iCallScoreTime;
			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
			m_iPalyCardCount= pGameStation->iAllCardCount;
			m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��

			return TRUE;
		}
	case GS_NOTE:		//��ע״̬
		{
			GameStation_Note *pGameStation = (GameStation_Note *)pStationData;
			if(NULL == pGameStation)
			{
				return TRUE;
			}
			//��������
			m_Logic.SetCardShape(pGameStation->iCardShape);
			//��������
			m_iThinkTime	= pGameStation->iThinkTime;
			m_iBeginTime	= pGameStation->iBeginTime;
			m_iCallScoreTime= pGameStation->iCallScoreTime;
			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
			m_iPalyCardCount= pGameStation->iAllCardCount;
			m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��

			//ׯ��λ��
			m_iUpGradePeople	 = pGameStation->byNtStation;
			//��ע����
			m_iThisTurnLimit = pGameStation->iLimitNote[GetMeUserInfo()->bDeskStation];
			return TRUE;
		}
	case GS_SEND_CARD:	//����״̬
		{
			GameStation_SendCard *pGameStation = (GameStation_SendCard *)pStationData;
			if(NULL == pGameStation)
			{
				return TRUE;
			}
			m_Logic.SetCardShape(pGameStation->iCardShape);
			//��������
			m_iThinkTime	= pGameStation->iThinkTime;
			m_iBeginTime	= pGameStation->iBeginTime;
			m_iCallScoreTime= pGameStation->iCallScoreTime;
			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
			m_iPalyCardCount= pGameStation->iAllCardCount;
			m_iRoomBasePoint= pGameStation->iRoomBasePoint ;	//���䱶��
			m_iUpGradePeople= pGameStation->byNtStation;		//ׯ��λ��

			//�˿�������
			memcpy(m_UserCard,pGameStation->iUserCard,sizeof(m_UserCard));
			memcpy(m_iDeskCardCount,pGameStation->iUserCardCount,sizeof(m_iDeskCardCount));
			return TRUE;
		}
	case GS_OPEN_CARD:	//��Ϸ������
		{
			if (uDataSize != sizeof(GameStation_OpenCard))
			{
				return TRUE;
			}
			GameStation_OpenCard *pGameStation = (GameStation_OpenCard *)pStationData;
			if(NULL == pGameStation)
			{
				return TRUE;
			}

			m_Logic.SetCardShape(pGameStation->iCardShape);
			//��������
			m_iThinkTime	= pGameStation->iThinkTime;
			m_iBeginTime	= pGameStation->iBeginTime;
			m_iCallScoreTime= pGameStation->iCallScoreTime;
			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
			m_iPalyCardCount= pGameStation->iAllCardCount;
			m_iRoomBasePoint= pGameStation->iRoomBasePoint ;	//���䱶��
			m_iUpGradePeople= pGameStation->byNtStation;		//ׯ��λ��

			//�˿�������
			memcpy(m_UserCard,pGameStation->iUserCard,sizeof(m_UserCard));
			memcpy(m_iDeskCardCount,pGameStation->iUserCardCount,sizeof(m_iDeskCardCount));

			return TRUE;
		}
	}
	return false;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_UG_USER_SET:		//�û������ƾ�
			{
				return TRUE;
			}
		case ASS_CALL_SCORE:				//���ܽ�ׯ��Ϣ
			{
				if(uDataSize!=sizeof(CallScoreStruct))
				{
					return 0;
				}
				CallScoreStruct *pCallScore=(CallScoreStruct *)pNetData;	
				if(NULL==pCallScore)
				{
					return TRUE;
				}
				SetStationParameter(GS_ROB_NT);

				if (pCallScore->bDeskStation == GetMeUserInfo()->bDeskStation)
				{
					int iTime = 1;
					if (m_iCallScoreTime > 10)
					{
						iTime = (rand()%(m_iCallScoreTime -10))+1;
					}
					else if (m_iCallScoreTime > 0)
					{
						iTime = (rand()%(m_iCallScoreTime))+1;
					}
				
					SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, ID_ROB_NT_TIMER);
				}
				
				return TRUE;
			}
		case ASS_CALL_SCORE_RESULT:			//���ܽ�ׯ�����Ϣ
			{
				return TRUE;
			}
		case ASS_CALL_SCORE_FINISH:			//���ܽ�ׯ������Ϣ
			{
				return TRUE;
			}
		case ASS_CALL_NOTE:				//��ʼ��ע
			{
				if (uDataSize !=sizeof(BeginUpgradeStruct))
				{
					return TRUE;
				}
				BeginUpgradeStruct * pBeginGame = (BeginUpgradeStruct *)pNetData;
				if (NULL == pBeginGame)
				{
					return TRUE;
				}
				//������Ϸ״̬
				SetStationParameter(GS_NOTE);

				if (GetMeUserInfo()->bDeskStation != pBeginGame->iNt)
				{
					int iTime = 1;
					if ((m_iXiaZhuTime - 10) > 0)
					{
						iTime = ((rand()+GetMeUserInfo()->bDeskStation)%(10))+1;
					}
					else if (m_iXiaZhuTime > 0)
					{
						iTime = ((rand()+GetMeUserInfo()->bDeskStation)%(m_iXiaZhuTime))+1;
					}
				
					SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, ID_NOTE_TIMER);
				}
				
				return TRUE;
			}
		case ASS_CALL_NOTE_RESULT:			//��ע���
			{
				return TRUE;
			}
		case ASS_CALL_SEND_CARD:				//��ʼ����
			{
				if(uDataSize != sizeof(SendAllCardStruct))
				{
					return FALSE;
				}
				SendAllCardStruct * pSendAllCard = (SendAllCardStruct *)pNetData;
				if (NULL == pSendAllCard)
				{
					return TRUE;
				}

				//����������
				for(int i=0;i<PLAY_COUNT;i++)
				{
					//ÿ�������˿˵���Ŀ
					m_iDeskCardCount[i]=pSendAllCard->iUserCardCount[i];	
				}
				memcpy(m_UserCard,pSendAllCard->iUserCard,sizeof(m_UserCard));
				
				return TRUE;
			}
		case ASS_CALL_SEND_FINISH:		//���ƽ���
			{
				return TRUE;
			}
		case ASS_CALL_OPEN:			//֪ͨ���ư�ţ
			{
				if(uDataSize != sizeof(BeginPlayStruct))
				{
					return FALSE;
				}
				BeginPlayStruct *gameBegin = (BeginPlayStruct *)pNetData ;
				if(NULL==gameBegin)
				{
					return TRUE; 
				}
				SetStationParameter(GS_OPEN_CARD);
				
				int iTime = 1;
				if ((m_iThinkTime - 10) > 0)
				{
					iTime = ((rand()+GetMeUserInfo()->bDeskStation)%(10))+1;
				}
				else if (m_iThinkTime > 0)
				{
					iTime = ((rand()+GetMeUserInfo()->bDeskStation)%(m_iThinkTime))+1;
				}
				SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, ID_OPEN_CARD_TIMER);
				return TRUE;
			}
		case ASS_CALL_OPEN_RESULT:			//̯��
			{
				return TRUE;
			}
		case ASS_CONTINUE_END:		//��Ϸ����
		case ASS_SALE_END:			//��Ϸ��ȫ����
		case ASS_CUT_END:			//�û�ǿ���뿪
		case ASS_NO_CONTINUE_END:
			{

				if(uDataSize != sizeof(GameEndStruct))
				{
					return TRUE;
				}
				SetStationParameter(GS_WAIT_NEXT);

				//��������
				int iTime=1;
				if ((m_iBeginTime ) > 10)
				{
					iTime = ((rand()+GetMeUserInfo()->bDeskStation)%(10))+1;
				}
				else if (m_iBeginTime > 0)
				{
					iTime = ((rand()+GetMeUserInfo()->bDeskStation)%(m_iThinkTime))+1;
				}
				
				//������Ŷӳ��Ͳ���ȥ���׼����
				if (!(m_pGameInfo->dwRoomRule & GRR_QUEUE_GAME))
				{
		
					SetGameTimer(GetMeUserInfo()->bDeskStation,iTime,ID_BEGIN_TIME);
				}
			
				break;
			}
		}
	}

	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	//m_PlayView.m_Result.ShowWindow(SW_HIDE);
	m_bTimeOutCount=0;

	m_iNowOutPeople=-1;

	m_iFirstOutPeople=-1;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

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
	if (((GetStationParameter()==GS_WAIT_SETGAME)
		||(GetStationParameter()==GS_WAIT_NEXT)
		||(GetStationParameter()==GS_WAIT_ARGEE))
		&& (!(m_pGameInfo->dwRoomRule & GRR_QUEUE_GAME))
		)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	KillGameTimer(0);
	ResetGameStation(0);
	return true;;
}

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{

	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_ROB_NT_TIMER);
	KillTimer(ID_NOTE_TIMER);
	KillTimer(ID_OPEN_CARD_TIMER);

	return;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			KillAllTimer();
			if(GetStationParameter() == GS_WAIT_SETGAME || GetStationParameter() == GS_WAIT_ARGEE || GetStationParameter() == GS_WAIT_NEXT)
			{
				OnHitBegin(0,0);
			}			
			return true;
		}
	case ID_ROB_NT_TIMER:	//��ׯ��ʱ��
		{
			KillAllTimer();
			if(GetStationParameter() == GS_ROB_NT)
			{
				//1/2�ĸ�����ׯ
				int iRand = rand()%2;
				CallScoreStruct callScore;
				callScore.iValue=iRand;
				callScore.bDeskStation = GetMeUserInfo()->bDeskStation;			
				callScore.bCallScoreflag = false;
				//���ͽ�ׯ��Ϣ
				SendGameData(&callScore,sizeof(callScore),MDM_GM_GAME_NOTIFY,ASS_USER_SCORE,0);
			}
			
			return true;
		}
	case ID_NOTE_TIMER:	//��ע
		{
			KillAllTimer();
			if(GetStationParameter() == GS_NOTE)
			{
				int iRand = (rand()+GetMeUserInfo()->bDeskStation)%4;
				tagUserProcess userBet;
				userBet.iNoteType= iRand;
				userBet.iVrebType = TYPE_NOTE;				//��ע����
				SendGameData(&userBet,sizeof(userBet),MDM_GM_GAME_NOTIFY,ASS_USER_NOTE,0);
			}

			return true;
		}
	case ID_OPEN_CARD_TIMER:
		{
			KillAllTimer();
			if (GetStationParameter() == GS_OPEN_CARD)
			{
				memset(m_bUpCard,255, sizeof(m_bUpCard));
				memset(m_bUnderCard,255,sizeof(m_bUnderCard));

				//�ж��Ƿ���ţ
				BYTE bHaveCard[3];
				memset(bHaveCard,255,sizeof(bHaveCard));
				if(m_Logic.GetBull(m_UserCard[GetMeUserInfo()->bDeskStation], 5, bHaveCard))
				{
					int j=0;
					int k=0;
					bool bHave=false;
					for(int i=0;i<5;i++)
					{
						bHave=false;
						for(k=0;k<3;k++)
						{
							if(m_UserCard[GetMeUserInfo()->bDeskStation][i]==bHaveCard[k])
							{
								bHave=true;
								break;
							}
						}
						if(bHave)
						{
							continue;
						}
						m_bUpCard[j]=m_UserCard[GetMeUserInfo()->bDeskStation][i];
						j++;
					}

					memcpy(m_bUnderCard,bHaveCard,sizeof(m_bUnderCard));
				}

				UserTanPai TUserTanPai;
				for(int i=0; i<3; i++)
				{
					if (m_bUnderCard[i] != 255)
					{
						TUserTanPai.byUnderCount++;
					}
				}
				memcpy(TUserTanPai.byUnderCard,m_bUnderCard,sizeof(TUserTanPai.byUnderCard));
				SendGameData(&TUserTanPai,sizeof(TUserTanPai),MDM_GM_GAME_NOTIFY,ASS_USER_OPEN,0);
			}
			

			return true;
		}
	//case ID_RESULT_TIME:
	//	{
	//		if(uTimeCount <= 0)
	//		{
	//			KillAllTimer();

	//			srand(GetTickCount()+GetMeUserInfo()->bDeskStation);

	//			if(!m_bLeftOrGoOn && m_iGameTimes >= rand()%m_byGameTimes+2)
	//			{
	//				OnCancel();
	//			}
	//			else
	//			{
	//				OnHitBegin(0,0);	//ʱ�䵽��
	//			}
	//		}

	//		return true;
	//	}
	
	}
	return true;
}




//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	//��������
	m_iNowOutPeople=-1;
	m_iFirstOutPeople=-1;
	::memset(m_iUpBullCard,1,sizeof(m_iUpBullCard));
	::memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));
	::memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
	//���������
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	//���������
	memset(m_UserCard,255,sizeof(m_UserCard));
	return;
}

void CClientGameDlg::OnGameSetting()
{
	AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}

void CClientGameDlg::OnWatchSetChange(void)
{

}

//�û��뿪
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	
	if (GetStationParameter()>GS_WAIT_ARGEE)
	{
		for (int i=0;i<PLAY_COUNT;i++)
		{
			if (m_pUserInfo[i]!=NULL) 
			{
				m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
			}
		}
	}

	//��ǰ����뿪��������Ƶ����Ҳ��֮�뿪����
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&pUserItem!=NULL&&!bWatchUser==TRUE)
	{
	
		OnCancel();

	}
	return true;

}

void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	CLoveSendClass::OnTimer(nIDEvent);
}


void CClientGameDlg::OnCancel()
{
	if(GetStationParameter()>=GS_SEND_CARD&&GetStationParameter() < GS_WAIT_NEXT)		//��ѯ״̬
	{
		return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}

	KillAllTimer();

	AFCCloseFrame();

	SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);		//����������Ϣ

	__super::OnCancel();
}

