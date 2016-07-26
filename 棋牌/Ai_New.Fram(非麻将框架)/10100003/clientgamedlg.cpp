#include "StdAfx.h"

#include ".\clientgamedlg.h"
#include "gameframedlg.h"
#include "advant.h"
#include "MultyTree.h"

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() :CLoveSendClass(IDD_GAME_FRAME)
{
	m_bAutoCard=false;
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	m_iThinkTime=0;	
	m_iBeenPlayGame=0;
	m_iMeCardCount=0;
	m_bShowIngLast=FALSE;
	m_bShowLast=FALSE;
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));

	//	m_bisexit=false;
	m_iNowOutPeople=-1;		//��ǰ������
	m_bCurrentOperationStation = 255;//��ǰ�з���
	m_iFirstOutPeople=-1;
	m_iBigOutPeople=-1;
	m_iNtPeople=-1;
	m_ResultCallScore = -1;			//��ʼ��
	m_btAutoTime=0;
	m_bSortCard=0;
	m_bTimeOutCount=0;
	::memset(m_iBackCard,0,sizeof(m_iBackCard));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iMeCardList,0,sizeof(m_iMeCardList));
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_iAIStation[i] = 0;
	}

}

//��������
CClientGameDlg::~CClientGameDlg()
{

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
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:	//�ȴ�ͬ��״̬
		{
		   GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			//��������
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;

			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_dwCardShape = pGameStation->iCardShape;
			m_Logic.SetCardShape(pGameStation->iCardShape) ; 

			if(!(m_pGameInfo->dwRoomRule &GRR_QUEUE_GAME) && !(m_pGameInfo->dwRoomRule &GRR_CONTEST))
			{
				SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
			}

			return TRUE;
		}
	case GS_SEND_CARD:	//����״̬
	case GS_WAIT_BACK:	//�ȴ������״̬
		{
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pStationData;
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			//����ȡ���������й�
			//SetAIMachine(bDeskStation,false);
			GameStation_5 * pGameStation=(GameStation_5 *)pStationData;
			//������
			m_dwCardShape = pGameStation->iCardShape;
			m_Logic.SetCardShape(pGameStation->iCardShape) ; 


			if(!(m_pGameInfo->dwRoomRule &GRR_CONTEST))
			{
				SetGameTimer(GetMeUserInfo()->bDeskStation,(rand() % GAME_RAND_TIME)+1,ID_BEGIN_TIME);
			}
			
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
		case ASS_GAME_BEGIN:		//��Ϸ��ʼ
			{				
				if (uDataSize != sizeof(GameBeginStruct))
					return FALSE;
				GameBeginStruct * pBegin=(GameBeginStruct *)pNetData;

				m_dwCardShape = pBegin->iCardShape;
				m_bTimeOutCount = 0;
				m_btAutoTime = 0;
				m_iBeenPlayGame ++;
				SetStationParameter(GS_SEND_CARD);
				m_Logic.SetCardShape(m_dwCardShape) ; 
				return TRUE;
			}
		case ASS_SEND_CARD:			//����������
			{
				if (uDataSize!=sizeof(SendCardStruct)) 
					return FALSE;
				SendCardStruct * pSendCard=(SendCardStruct *)pNetData;

				//if(pSendCard->bDeskStation != GetMeUserInfo()->bDeskStation)
				{
					int desk = ViewStation(pSendCard->bDeskStation);
					m_Card[desk][m_CardCount[desk]++] = pSendCard->bCard;
					m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					return true;
				}
				m_iMeCardList[m_iMeCardCount++] = pSendCard->bCard;
				m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
				return TRUE;
			}
		case ASS_SEND_ALL_CARD:			//һ�ν�һ���˵���ȫ������
			{
				if (uDataSize!=sizeof(SendAllStruct)) 
					return FALSE;
				SendAllStruct * pSendAll=(SendAllStruct *)pNetData;
				int iPos = 0;
				for(int i = 0;i < PLAY_COUNT; i ++)
				{
					int desk = ViewStation(i);
					m_CardCount[desk] = pSendAll->iUserCardCount[i];
					::CopyMemory(m_Card[desk],&pSendAll->iUserCardList[iPos],sizeof(BYTE)*m_CardCount[desk]);
					iPos += m_CardCount[desk];
					//	m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);

					if(i == GetMeUserInfo()->bDeskStation)
					{
						::CopyMemory(m_iMeCardList,m_Card[desk],m_CardCount[desk]);
						m_iMeCardCount = m_CardCount[desk];
						m_Logic.SortCard(m_Card[GetMeUserInfo()->bDeskStation],NULL,m_CardCount[GetMeUserInfo()->bDeskStation]);
					}
				}
				return TRUE;
			}
		case ASS_SEND_CARD_MSG://������Ϣ
			{
				return TRUE;
			}
		case ASS_SEND_FINISH:		//�������
			{
				m_Logic.SortCard(m_Card[GetMeUserInfo()->bDeskStation],NULL,m_CardCount[GetMeUserInfo()->bDeskStation]);
				initGuessCard();
				deleteGuessCard(m_Card[GetMeUserInfo()->bDeskStation],m_CardCount[GetMeUserInfo()->bDeskStation]);
				return TRUE;
			}
		case ASS_CALL_SCORE:			//�з�
			{	
				SetStationParameter(GS_WAIT_BACK);
				CallScoreStruct *score=(CallScoreStruct *)pNetData;	
				m_bCurrentOperationStation = score->bDeskStation;
				m_ResultCallScore = (score->iValue == 255) ? -1: score->iValue;

				if( GetMeUserInfo()->bDeskStation == score->bDeskStation )
				{
					SetGameTimer(score->bDeskStation,rand() % GAME_RAND_TIME + 1,ID_CALL_SCORE_TIME);
				}
				return true;
			}
		case ASS_CALL_SCORE_RESULT:			//�зֽ��
			{
				return true;
			}
		case ASS_CALL_SCORE_FINISH:
			{
				return true;
			}
		case ASS_ROB_NT:	//������
			{
				if (uDataSize != sizeof(RobNTStruct)) 
					return FALSE;
				RobNTStruct * pRobNT = (RobNTStruct *)pNetData;
				m_bCurrentOperationStation = pRobNT->bDeskStation ;

				//����������
				if(GetMeUserInfo()->bDeskStation == pRobNT->bDeskStation )
				{
					SetGameTimer(m_bCurrentOperationStation,(rand() % GAME_RAND_TIME)+1,ID_ROB_NT_TIME);
				}

				return TRUE;
			}
		case ASS_ROB_NT_RESULT://���������
			{
				return TRUE;
			}
		case ASS_GAME_MULTIPLE:
			{
				return TRUE;
			}
		case ASS_ROB_NT_FINISH:		//����������
			{
				return true;
			}
		case ASS_BACK_CARD_EX:		//��չ��������
			{
				if (uDataSize!=sizeof(BackCardExStruct)) 
					return FALSE;
				BackCardExStruct * pBackCard=(BackCardExStruct *)pNetData;

				m_iNtPeople = pBackCard->iGiveBackPeople ; 

				diPai(pBackCard);
				//��������
				if(pBackCard->iGiveBackPeople != GetMeUserInfo()->bDeskStation)
				{
					int desk = ViewStation(pBackCard->iGiveBackPeople);
					::CopyMemory(&m_Card[desk][m_CardCount[desk]],pBackCard->iBackCard,sizeof(BYTE)*(pBackCard->iBackCardCount));
					m_CardCount[desk] += pBackCard->iBackCardCount;
					//	m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					return true;
				}	
				if (GetMeUserInfo()->bDeskStation == pBackCard->iGiveBackPeople)
				{
					::CopyMemory(&m_iMeCardList[m_iMeCardCount],pBackCard->iBackCard,sizeof(BYTE)*pBackCard->iBackCardCount);
					m_iMeCardCount += pBackCard->iBackCardCount;
					int desk = ViewStation(pBackCard->iGiveBackPeople);
					::CopyMemory(&m_Card[desk][m_CardCount[desk]],pBackCard->iBackCard,sizeof(BYTE)*(pBackCard->iBackCardCount));
					m_CardCount[desk] += pBackCard->iBackCardCount;
					m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
				}
				return TRUE;
			}
		case ASS_ADD_DOUBLE://�Ӱ�
			{
				if (uDataSize!=sizeof(AddDoubleStruct)) 
					return FALSE;
				AddDoubleStruct * pAddDouble=(AddDoubleStruct *)pNetData;

				//�Ӱ�
				if(GetMeUserInfo()->bDeskStation != pAddDouble->bDeskStation )
				{
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME + 1,ID_ADD_DOUBLE_TIME);
				}				
				return TRUE;
			}
		case ASS_ADD_DOUBLE_RESULT://�Ӱ����
			{
				return true;
			}
		case ASS_ADD_DOUBLE_FINISH:
			{
				KillGameTimer(ID_ADD_DOUBLE_TIME);
				return true;
			}
		case ASS_SHOW_CARD://����
			{
				/*if (uDataSize!=sizeof(ShowCardStruct)) 
				return FALSE;*/
				ShowCardStruct  * pShow = (ShowCardStruct*)pNetData;

				if( GetMeUserInfo()->bDeskStation == pShow->bDeskStation )
				{
					SetGameTimer(pShow->bDeskStation,rand() % GAME_RAND_TIME + 1,ID_SHOW_CARD);
				}
				return TRUE;
			}
		case ASS_SHOW_CARD_RESULT:
			{
				return TRUE;
			}
		case ASS_SHOW_CARD_FINISH:
			{
				return true;
			}
		case ASS_GAME_PLAY:		//��ʼ������Ϸ
			{
				//����ۿ�,�����й�
				if (uDataSize!=sizeof(BeginPlayStruct)) 
				{
					return FALSE;
				}
				BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)pNetData;
				m_bTimeOutCount=0;
				SetStationParameter(GS_PLAY_GAME);
				m_iBigOutPeople = m_iNowOutPeople = m_iFirstOutPeople = pBeginInfo->iOutDeskStation;

				if (GetMeUserInfo()->bDeskStation == m_iNowOutPeople)
				{
					SetGameTimer(m_iNowOutPeople,(rand()%GAME_RAND_TIME )+ 1,ID_OUT_CARD);
				}
				return TRUE;
			}
		case ASS_OUT_CARD_RESULT:
			{
				OutCardMsg * pOutCardInfo=(OutCardMsg *)pNetData;
				KillGameTimer(0);

				m_iDeskCardCount[pOutCardInfo->bDeskStation]=pOutCardInfo->iCardCount;
				::CopyMemory(m_DeskCard[pOutCardInfo->bDeskStation],pOutCardInfo->iCardList,sizeof(BYTE)*pOutCardInfo->iCardCount);

				//��¼������Ϣ
				if (pOutCardInfo->iCardCount > 0)
				{
					m_iBigOutPeople = pOutCardInfo->bDeskStation;

					m_iBaseCount = pOutCardInfo->iCardCount;

					::CopyMemory(m_iBaseCard,pOutCardInfo->iCardList,sizeof(BYTE)*m_iBaseCount);
					if(pOutCardInfo->bDeskStation != GetMeUserInfo()->bDeskStation)
					{
						BYTE desk = pOutCardInfo->bDeskStation;
						m_Logic.RemoveCard(pOutCardInfo->iCardList,pOutCardInfo->iCardCount,m_Card[desk],m_CardCount[desk]);
						m_CardCount[desk] -= pOutCardInfo->iCardCount;
						m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					}
					else			//ɾ���Լ����˿�
					{
						BYTE desk = pOutCardInfo->bDeskStation;
						m_Logic.RemoveCard(pOutCardInfo->iCardList,pOutCardInfo->iCardCount,m_Card[desk],m_CardCount[desk]/*m_iMeCardList,m_iMeCardCount*/);
						m_iMeCardCount -= pOutCardInfo->iCardCount;
						m_CardCount[desk] -= pOutCardInfo->iCardCount;
						m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					}
				}
				outCardResult(pOutCardInfo);

				//��ʾ�˿�
				int iViewStation = ViewStation(pOutCardInfo->bDeskStation);
				m_Logic.SortCard(m_DeskCard[pOutCardInfo->bDeskStation],NULL,m_iDeskCardCount[pOutCardInfo->bDeskStation]);
				return TRUE;
			}
		case ASS_AWARD_POINT://����
			{
				return true;
			}
		case ASS_OUT_CARD:		//�û�����
			{
				OutCardMsg * pOutCardInfo=(OutCardMsg *)pNetData;
				KillGameTimer(0);

				//�ж���һ������
				m_iNowOutPeople = pOutCardInfo->iNextDeskStation;

				if (m_iNowOutPeople == GetMeUserInfo()->bDeskStation)
				{
					SetGameTimer(m_iNowOutPeople,(rand()%GAME_RAND_TIME) + 1,ID_OUT_CARD);
				}
				return TRUE;
			}
		case ASS_AI_STATION:
			{
				return true;
			}
		case ASS_ONE_TURN_OVER:
			{
				return true;
			}
		case ASS_NEW_TURN:		//��һ�ֳ���
			{
				if(uDataSize!=sizeof(NewTurnStruct))
					return FALSE;
				NewTurnStruct *turn = (NewTurnStruct *)pNetData; 
				m_iBaseCount = 0;
				m_iBigOutPeople = m_iNowOutPeople = m_iFirstOutPeople = turn->bDeskStation ;
				//ɾ���˿���Ϣ
				for (int i = 0;i < PLAY_COUNT;i ++)
					m_iDeskCardCount[i] = 0;

				if (GetMeUserInfo()->bDeskStation == m_iNowOutPeople)
				{
					SetGameTimer(m_iNowOutPeople,rand()%GAME_RAND_TIME + 1,ID_OUT_CARD);
				}

				return TRUE;
			}
		case ASS_NO_CALL_SCORE_END://���˽з�ֱ�ӽ�����һ��
			{
				KillGameTimer(0);
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(0);
				return true;
			}
		case ASS_SAFE_END:	//��Ϸ��ȫ����
		case ASS_CUT_END:	//�û�ǿ���뿪
		case ASS_TERMINATE_END://�������
		case ASS_NO_CONTINUE_END://��Ϸ����
		case ASS_CONTINUE_END:	//��Ϸ����
		case ASS_AHEAD_END:	

			{
				KillGameTimer(0);
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(0);
	
				if(!(m_pGameInfo->dwRoomRule &GRR_CONTEST))
				{
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
				}
				
			
				return true;
			}
		case ASS_HAVE_THING:	//�û������뿪
			{
				return TRUE;
			}
		case ASS_LEFT_RESULT:	//�����뿪��� 
			{
				return TRUE;
			}
		case ASS_AUTO:				//�й�
			{
				return true;
			}
		case ASS_MESSAGE:
			{
				return true;
			}
		}
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	m_bTimeOutCount = 0;
	m_iMeCardCount = 0;
	m_iNtPeople = -1;
	m_iNowOutPeople = -1;
	m_iBaseCount = 0;
	m_iFirstOutPeople = -1;
	//	m_iLastCardCount=0;
	m_iBeenPlayGame = 0;
	m_bShowLast = FALSE;
	::memset(m_iBackCard,0,sizeof(m_iBackCard));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));

	KillGameTimer(0);
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
		)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	KillGameTimer(0);
	OutputDebugString("dwjrobot::OnControlHitBegin-�������������");
	ResetGameStation(0);
	return true;
}


//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			if((m_pGameInfo->dwRoomRule &GRR_CONTEST) || m_pGameInfo->dwRoomRule &GRR_QUEUE_GAME)
			{
				OutputDebugString("dwjrobot::�����������Ŷӻ� ����׼����ʱ��-ֱ�ӷ���");
				KillGameTimer(ID_BEGIN_TIME);
				return true;
			}
			OnHitBegin(0,0);
			return true;
		}
	case ID_LEAVE_TIME:
		{
			return true;
		}
	case ID_CALL_SCORE_TIME:	//�зּ�ʱ��
		{
			//int result = rand()%4;
			//if(result > m_ResultCallScore && result > 0 && result <= 3)
			//	;
			//else 
			//	result = 0;
			//OnCallScore(result,result);
			AI_QiangDiZhu();
			return true;
		}
	case ID_ROB_NT_TIME://������
		{
			if(canQiangDiZhu())
			{
				OnRobNT(1,0);
			}
			else
			{				
				OnRobNT(0,0);
			}
			
			return true;
		}
	case ID_ADD_DOUBLE_TIME://�Ӱ�
		{
			//OnAddDouble(rand()%2,rand()%2);
			AI_JiaBei();
			return true;
		}
	case ID_SHOW_CARD://����
		{
			OnShowCard(0,0);   //��Զ������
			return true;
		}
	case ID_OUT_CARD:	//����
		{
	
			if(m_iBigOutPeople == GetMeUserInfo()->bDeskStation)
	        {
				AI_zhanQuanOutCard();
	        }
			else
			{
				AI_buZhanQuanQutCard();
			}
	        UserOutCard();
		    return true;
		}
	default:
		break;
	}
	return TRUE;
}


//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)//��Ϸ��ʼ
{
	//��������
	m_bTimeOutCount=0;//��ʱ����

	m_iNtPeople=-1;
	m_iNowOutPeople=-1;
	m_iBaseCount=0;
	m_iFirstOutPeople=-1;

	m_bShowLast=FALSE;
	m_bCurrentOperationStation = 255;//��ǰ�з���
	//��������
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));

	m_ResultCallScore=-1;			//��ʼ��
	m_bAutoCard=false;
	::memset(m_CardCount,0,sizeof(m_CardCount));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	m_iMeCardCount = 0;
	::memset(m_iMeCardList,0,sizeof(m_iMeCardList));
	::memset(m_iBackCard,0,sizeof(m_iBackCard));
	::memset(m_iAIStation,0,sizeof(m_iAIStation));





	m_bAutoCard=false;


	m_iBeenPlayGame=0;

	m_bShowIngLast=FALSE;
	m_bShowLast=FALSE;
	m_iBigOutPeople=-1;
	m_bSortCard=0;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_iAIStation[i] = 0;
	}


	return;
}



//�з�
LRESULT	CClientGameDlg::OnCallScore(WPARAM wparam, LPARAM lparam)
{
	KillGameTimer(ID_CALL_SCORE_TIME);
	CallScoreStruct score;
	memset(&score,0,sizeof(score));
	score.iValue = (int)wparam;								
	score.bDeskStation = GetMeUserInfo()->bDeskStation;			
	score.bCallScoreflag = false;
	SendGameData(&score,sizeof(score),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	return 0;
}
//������
LRESULT	CClientGameDlg::OnRobNT(WPARAM wparam, LPARAM lparam)
{
	RobNTStruct robnt;
	memset(&robnt,0,sizeof(robnt));
	robnt.bDeskStation = GetMeUserInfo()->bDeskStation;		
	robnt.iValue  = (int)wparam;	
	SendGameData(&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT,0);
	return 0;
}
//�Ӱ�
LRESULT	CClientGameDlg::OnAddDouble(WPARAM wparam, LPARAM lparam)
{
	AddDoubleStruct AddDouble;
	memset(&AddDouble,0,sizeof(AddDouble));
	AddDouble.bDeskStation = GetMeUserInfo()->bDeskStation;		
	AddDouble.iValue  = (int)wparam;	
	SendGameData(&AddDouble,sizeof(AddDouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE,0);
	return 0;
}
//����
LRESULT	CClientGameDlg::OnShowCard(WPARAM wparam, LPARAM lparam)
{
	KillGameTimer(ID_SHOW_CARD);

	ShowCardStruct show;
	memset(&show,0,sizeof(show));
	show.bDeskStation = GetMeUserInfo()->bDeskStation;		
	show.iValue  = (int)wparam;	
	SendGameData(&show,sizeof(show),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD,0);
	return 0;
}
/*---------------------------------------------------------------------------------------*/
//��ö��ѵ�λ��
BYTE	CClientGameDlg::GetFriendStation(BYTE bDeskStation)
{
	BYTE byFriendStation = 0;
	//�ǵ����ͷ��ص���
	if(bDeskStation == m_iNtPeople)
	{
		return bDeskStation;
	}
	else
	{
		for(int i=0; i<PLAY_COUNT; i++)
		{
			if(bDeskStation != i && i != m_iNtPeople)
			{
				byFriendStation = i;
				break;
			}
		}
	}
	return byFriendStation;
}
/*---------------------------------------------------------------------------------------*/
//��ȡ��һ�����λ��
BYTE	CClientGameDlg::GetNextDeskStation(BYTE bDeskStation)
{
	if(false)//˳ʱ��
		return (bDeskStation + 1 ) % PLAY_COUNT;
	//��ʱ��
	return 	(bDeskStation + (PLAY_COUNT - 1)) % PLAY_COUNT;
}
/*---------------------------------------------------------------------------------------*/
//���������ܳ���
bool	CClientGameDlg::DwjAiOutCard()
{
	CString dwjlog;
	
	//�Լ���λ��
	BYTE bMyStation = GetMeUserInfo()->bDeskStation;
	//��һ��λ��
	BYTE bNextStation = GetNextDeskStation(bMyStation);
	//���ѵ�λ��
	BYTE byFriendStation = GetFriendStation(bMyStation);
	//���ϵ�����
	BYTE bLastCardShape = m_Logic.GetCardShape(m_Card[bMyStation],m_CardCount[bMyStation]);
	int	 iTmpResultCardCount = 0;	//�˿���Ŀ
	BYTE byTmpResultCardList[45];	//�˿���Ϣ
	memset(byTmpResultCardList,255,sizeof(byTmpResultCardList));


	if(m_iBigOutPeople == bMyStation)
	{
		//����Լ����ϵ�����һ�³��� ��һ�³���
		if(bLastCardShape != UG_ERROR_KIND)	
		{
			//�����Ĵ��� 
			if((bLastCardShape <  UG_FOUR_ONE || bLastCardShape > UG_FOUR_TWO_DOUBLE) )
			{
				memcpy(m_myOutCard.iCardList,m_Card[bMyStation],sizeof(m_myOutCard.iCardList));
				m_myOutCard.iCardCount = m_CardCount[bMyStation];
			}
			else if(bMyStation != m_iNtPeople && m_CardCount[m_iNtPeople]==1)	// �Լ����ǵ��� && ����ֻ��һ���� ��ô��һ�׳���
			{
				memcpy(m_myOutCard.iCardList,m_Card[bMyStation],sizeof(m_myOutCard.iCardList));
				m_myOutCard.iCardCount = m_CardCount[bMyStation];
			}
		}
		else if(m_myOutCard.iCardCount == 1) 
		{
			if(m_CardCount[m_iNtPeople] == 1)	//����ֻ��һ�� �ͳ���������
			{
				//����������һ����
				m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],255,true);
				m_myOutCard.iCardCount = 1;
			}
			else if(m_CardCount[m_iNtPeople] !=2)	// �������ϲ��������� �����˲��ܲ��˶��Ӵ�(�� ����ʣ�����ŵ������ ��ʱ���Բ��˶��Ӵ�һ��)
			{
				//������ȡ����
				if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
				{
					memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
					m_myOutCard.iCardCount = iTmpResultCardCount;
				}
			}
		}
		else if(m_CardCount[m_iNtPeople]==1)	//����ֻ��һ����		//����¼��Ƕ��� ֻʣһ������
		{
			if(bMyStation != m_iNtPeople)		//�Լ����ǵ���
			{
				if(m_CardCount[byFriendStation]==1)	//����Ҳֻ��һ����
				{
					if(byFriendStation == GetNextDeskStation(bMyStation))	//��һ������Ƕ���
					{
						//��������С��һ����
						m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],1,true);
						m_myOutCard.iCardCount = 1;
					}
					else
					{
						if(m_CardCount[bMyStation]==2)	//�Լ�ֻ����������
						{
							//���ȳ����� ����û�ҵ� �ͳ�����һ����
							if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
							{
								memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
								m_myOutCard.iCardCount = iTmpResultCardCount;
							}
							else	//û�ж��� ��ô�ͳ�����һ��
							{
								//����������һ����
								m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],255,true);
								m_myOutCard.iCardCount = 1;
							}
						}
						else
						{
							//����������һ����
							m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],255,true);
							m_myOutCard.iCardCount = 1;
						}
					}
				}
				else if(m_CardCount[byFriendStation]==2)		//���ѻ������������ȳ�����	
				{
					if(m_CardCount[bMyStation]==2)	//�Լ�ֻ����������
					{
						//���ȳ����� 
						if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
						{
							memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
							m_myOutCard.iCardCount = iTmpResultCardCount;
						}
						else	//û�ж��� ��ô�ͳ�����һ��
						{
							//����������һ����
							m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],255,true);
							m_myOutCard.iCardCount = 1;
						}
					}
					else
					{
						//������ȡ����
						if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
						{
							memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
							m_myOutCard.iCardCount = iTmpResultCardCount;
						}
						else
						{
							//����������һ����
							m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],255,true);
							m_myOutCard.iCardCount = 1;
						}
					}
				}
				else	//���ѻ��ܶ��ƣ����ʱ���Լ���ȡ������
				{
					//���֮ǰ�ǵ��� ��ô�Ͳ��ܳ�����
					if(m_myOutCard.iCardCount == 1 || m_myOutCard.iCardCount == 0)
					{
						//�ȳ�����
						if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
						{
							memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
							m_myOutCard.iCardCount = iTmpResultCardCount;
						}
						else	//û���� �Ǿͳ����ĵ���
						{
							m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],255,true);
							m_myOutCard.iCardCount = 1;
						}
					}
					else
					{
						
					}
				}
			}
		}
		else if(m_CardCount[m_iNtPeople]==2)	//��������������
		{
			//������������Ƶ�����
			BYTE byNtCardShpe = m_Logic.GetCardShape(m_Card[m_iNtPeople],m_CardCount[m_iNtPeople]);

			if(bMyStation != m_iNtPeople)		//�Լ����ǵ���
			{
				if(m_CardCount[byFriendStation]==1)	//����ֻ��һ����
				{
					//����¼��Ƕ��� ��ô�ͳ���С����(�����Ƿ�ɶ�)
					if(byFriendStation == GetNextDeskStation(bMyStation))
					{
						m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],1,true);
						m_myOutCard.iCardCount = 1;
					}
					else	
					{
						//�¼Ҳ��Ƕ��� ��ô�ͳ���Сû�гɶԵ���
						if(m_Logic.TackOutMinOrMaxOneNoDouble(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
						{
							memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
							m_myOutCard.iCardCount = iTmpResultCardCount;
						}
						else	//û�ҵ����ɶԵ��� ��ô�Ͳ�����С�Ķ���
						{
							m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],1,true);
							m_myOutCard.iCardCount = 1;
						}
					}					
				}
				else if(m_CardCount[byFriendStation]==2 && byFriendStation == GetNextDeskStation(bMyStation))		//���ѻ������������ȳ�����	
				{
					//if(byFriendStation == GetNextDeskStation(bMyStation))	//��һ������Ƕ���
					{
						//���ѵ����Ƕ���(�������ͻ�ȡ���� ����û�з����� )
						//if(m_Logic.GetCardShape(m_Card[byFriendStation],m_CardCount[byFriendStation]) == UG_DOUBLE)
						{
							//������ȡ��С����
							if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
							{
								//dwjtest����ֻ��С���� ����ӾͲ����� (��Jһ�¾���С����)
								if(m_Logic.GetCardNum(byTmpResultCardList[0]) <= 11)
								{
									memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
									m_myOutCard.iCardCount = iTmpResultCardCount;
								}
							}
						}
					}
				}
				else	//���ѻ��ܶ��ƣ����ʱ���Լ���ȡ������
				{
					if(m_CardCount[bMyStation]==2)	//�Լ�ֻ����������
					{
						//���ȳ����� 
						if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
						{
							memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
							m_myOutCard.iCardCount = iTmpResultCardCount;
						}
						else	//û�ж��� ��ô�ͳ�����һ��
						{
							//����������һ����
							m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],255,true);
							m_myOutCard.iCardCount = 1;
						}
					}
					else
					{
						//�������ʣ�µ��Ƕ��� ��ô�Ϳ��Գ�һ��
						if(byNtCardShpe == UG_DOUBLE)
						{
							//���������Ҫ��һ��  ��ô�ͳ�����
							if(m_myOutCard.iCardCount == 2 || m_myOutCard.iCardCount == 0)
							{
								//������Ҫ����Сû�гɶԵ���
								if(m_Logic.TackOutMinOrMaxOneNoDouble(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
								{
									memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
									m_myOutCard.iCardCount = iTmpResultCardCount;
								}
								else	//û�ҵ����ɶԵ��� ��ô�Ͳ�����С�Ķ���
								{
									m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],1,true);
									m_myOutCard.iCardCount = 1;
								}
								/*m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],1,true);
								m_myOutCard.iCardCount = 1;*/
							}
						}
						else	//����������Ų��Ƕ��� ��ô�Ϳ��Գ�����
						{
							//���ʱ�����ȳ�����
							if(m_myOutCard.iCardCount == 1 || m_myOutCard.iCardCount == 0)
							{
								//������ȡ����
								if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
								{
									memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
									m_myOutCard.iCardCount = iTmpResultCardCount;
								}
							}
						}
					}
				}
			}
		}
		else	//�������кܶ���
		{
			if(bMyStation != m_iNtPeople)		//�Լ����ǵ���
			{
				if(m_CardCount[byFriendStation]==1)	//����ֻ��һ���� 
				{
					//��������С��һ����
					m_myOutCard.iCardList[0] = m_Logic.GetBulkBySepcifyMinOrMax(m_Card[bMyStation],m_CardCount[bMyStation],1,true);
					m_myOutCard.iCardCount = 1;
				}
				else if(m_CardCount[byFriendStation]==2)		//���ѻ������������ȳ�����	
				{
					//if(byFriendStation == GetNextDeskStation(bMyStation))	//��һ������Ƕ���
					{
						//���ѵ����Ƕ���(�������ͻ�ȡ���� ����û�з����� )
						//if(m_Logic.GetCardShape(m_Card[byFriendStation],m_CardCount[byFriendStation]) == UG_DOUBLE)
						{
							//������ȡ��С����
							if(m_Logic.TackOutDouBle(m_Card[bMyStation],m_CardCount[bMyStation],byTmpResultCardList,iTmpResultCardCount,0))
							{
								memcpy(m_myOutCard.iCardList , byTmpResultCardList, sizeof(m_myOutCard.iCardList));
								m_myOutCard.iCardCount = iTmpResultCardCount;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		BYTE byOutCardShape = m_Logic.GetCardShape(m_myOutCard.iCardList,m_myOutCard.iCardCount);

		if(bMyStation == m_iNtPeople)	//�Լ��ǵ���
		{
			if(byOutCardShape == UG_BOMB || UG_KING_BOMB == byOutCardShape)
			{
				//����з�������6���� ��ը
				if(m_CardCount[m_iBigOutPeople] <=6 )
				{
					//����ը ����
				}
				else
				{
					memset(m_myOutCard.iCardList,255,sizeof(m_myOutCard.iCardList));
					m_myOutCard.iCardCount = 0;
				}
			}
		}
		else	//�Լ����ǵ���
		{
			if(byOutCardShape == UG_BOMB || UG_KING_BOMB == byOutCardShape)
			{
				//����з�������6���� ��ը
				if(m_CardCount[m_iNtPeople] <=6 )
				{
					//����ը ����
				}
				else
				{
					memset(m_myOutCard.iCardList,255,sizeof(m_myOutCard.iCardList));
					m_myOutCard.iCardCount = 0;
				}
			}
		}
	}


	//�������ֻ��һ������
	//if(m_CardCount[m_iNtPeople] == 1 && bMyStation != m_iNtPeople)

	if(m_myOutCard.iCardCount !=0)   
	{
		SendGameData(&m_myOutCard,sizeof(m_myOutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		m_iNowOutPeople=-1;
		return true;
	}
	return false;
}
/*---------------------------------------------------------------------------------------*/
//����
BOOL CClientGameDlg::UserOutCard()
{
	
	//���������ܳ���
	if(DwjAiOutCard())
	{
		return TRUE;
	}

	if(m_myOutCard.iCardCount !=0)   //����˵���ǳ��ĵ��ƻ��߶��ӻ���ը��
	{
		SendGameData(&m_myOutCard,sizeof(m_myOutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		m_iNowOutPeople=-1;
		return true;
	}
	
	BYTE bMyStation = GetMeUserInfo()->bDeskStation;
	OutCardStruct OutCard;
	//��������
	
	if(m_iBaseCount == 0 || m_bAutoCard)//�йܳ���
	{

		m_Logic.AutoOutCard(m_Card[bMyStation], m_CardCount[bMyStation], m_iBaseCard,m_iBaseCount,OutCard.iCardList,
			OutCard.iCardCount,m_iBigOutPeople==GetMeUserInfo()->bDeskStation);
	}
	else
	{
		OutCard.iCardCount=0;
	}

//#ifdef AI_MACHINE
	if(m_iBigOutPeople!=m_iNtPeople&&m_iBigOutPeople!=GetMeUserInfo()->bDeskStation&&m_iNtPeople!=GetMeUserInfo()->bDeskStation)
	{
		/// ͬ���ƵĴ���,�Ҳ��ܱ�Ӯ��ʱ��
		AI_DaTongHuo();
		return true;
	}

	
	m_Logic.AutoOutCard(m_Card[bMyStation], m_CardCount[bMyStation], m_iBaseCard,m_iBaseCount,OutCard.iCardList,OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
   

	if(m_iBigOutPeople == GetMeUserInfo()->bDeskStation)
	{
		/// �������ը�Ļ���˵��ֻʣ����ը�ˣ����Կ���ֱ�ӳ������ֻ������ͨը����
		BYTE					m_tempCard[CARD_COUNT_PER_PERSON];
		int                    m_tempCount = m_CardCount[bMyStation];
		memcpy(m_tempCard,m_Card[bMyStation],sizeof(m_tempCard));
		m_tempCount = m_CardCount[bMyStation];
		while(m_Logic.GetCardShape(OutCard.iCardList,OutCard.iCardCount) == UG_BOMB)
		{
			del(m_tempCard,m_tempCount,OutCard.iCardList,OutCard.iCardCount);

			if(m_tempCount == 0)
			{
				/// ˵��ֻ��ը��û��Ķ���
				m_Logic.AutoOutCard(m_Card[bMyStation], m_CardCount[bMyStation], m_iBaseCard,m_iBaseCount,OutCard.iCardList,
					OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
				goto OUTCARD;
			}
			m_Logic.AutoOutCard(m_tempCard,m_tempCount, m_iBaseCard,m_iBaseCount,OutCard.iCardList,
				OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);

		}

	}
//#endif

OUTCARD:	m_iNowOutPeople=-1;

	BYTE byOutCardShape = m_Logic.GetCardShape(OutCard.iCardList,OutCard.iCardCount);
	if(bMyStation == m_iNtPeople)	//�Լ��ǵ���
	{
		if(byOutCardShape == UG_BOMB || UG_KING_BOMB == byOutCardShape)
		{
			//����з�������6���� ��ը
			if(m_CardCount[m_iBigOutPeople] <=6 )
			{
				//����ը ����
			}
			else
			{
				memset(OutCard.iCardList,255,sizeof(OutCard.iCardList));
				OutCard.iCardCount = 0;
			}
		}
	}
	else	//�Լ����ǵ���
	{
		if(byOutCardShape == UG_BOMB || UG_KING_BOMB == byOutCardShape)
		{
			//����з�������6���� ��ը
			if(m_CardCount[m_iNtPeople] <=6 )
			{
				//����ը ����
			}
			else
			{
				memset(OutCard.iCardList,255,sizeof(OutCard.iCardList));
				OutCard.iCardCount = 0;
			}
		}
	}

	SendGameData(&OutCard,sizeof(OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);



	////��������
	//OutCardStruct OutCard;

	//m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,OutCard.iCardList,
	//	OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);

	//m_iNowOutPeople = -1;

	//SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList) + sizeof(BYTE)*OutCard.iCardCount,
	//	MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

	return TRUE;
}

bool CClientGameDlg::OnControlHitPass(void)
{
	//���ÿؼ�
	//if (m_PlayView.m_btPass.IsWindowVisible())
	{
		//��������
		KillAllTimer();
		m_iNowOutPeople=-1;
		//ShowControl(DDZ_CHUPAI, SW_HIDE);
		//ShowControl(DDZ_BUCHU_PASS, SW_HIDE);
		//ShowControl(DDZ_TISHI, SW_HIDE);
		//��������
		OutCardStruct OutCard;
		memset(OutCard.iCardList, 0, sizeof(OutCard.iCardList));
		OutCard.iCardCount=0;
		SendGameData(&OutCard,sizeof(OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	}

	return true;
}

void CClientGameDlg::AI_DaTongHuo()
{
	//if(m_index==2)	
	//{
	//	/// ���������Ҫ����Ҳ��Ҫ
	//	OnControlHitPass();
	//}
	//else
	{
		BYTE shapeType = m_Logic.GetCardShape(m_iBaseCard,m_iBaseCount); 
		OutCardStruct OutCard;
		if(shapeType==UG_ONLY_ONE)
		{
			if(m_Logic.GetCardBulk(m_iBaseCard[0])<10)
			{
	        	BYTE					m_tempCard[CARD_COUNT_PER_PERSON];
				int                    m_tempCount = m_CardCount[GetMeUserInfo()->bDeskStation];
				memcpy(m_tempCard,m_Card[GetMeUserInfo()->bDeskStation],sizeof(m_tempCard));
				m_tempCount = m_CardCount[GetMeUserInfo()->bDeskStation];
				do{
        			m_Logic.AutoOutCard(m_tempCard,m_tempCount, m_iBaseCard,m_iBaseCount,OutCard.iCardList,
						OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
					if(OutCard.iCardCount>0)
					{
				    	delBulk(m_tempCard,m_tempCount,m_Logic.GetCardBulk(OutCard.iCardList[0]));
						if(OutCard.iCardCount == 1&&m_Logic.GetCardBulk(OutCard.iCardList[0])<15&&m_Logic.GetCardBulk(OutCard.iCardList[0])>=10)
						{
							/// ˵���ҵ����ʵ�����
							m_iNowOutPeople=-1;
							SendGameData(&OutCard,sizeof(OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
							return;
						}
						if(m_tempCount == 0)
						{
							/// ˵��û���ҵ����ʵ��ƣ�ֻ��ѡ����Ժ��ʵ���
							m_Logic.AutoOutCard(m_Card[GetMeUserInfo()->bDeskStation], m_CardCount[GetMeUserInfo()->bDeskStation], m_iBaseCard,m_iBaseCount,OutCard.iCardList,
								OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
							if(OutCard.iCardCount!=1||m_Logic.GetCardBulk(OutCard.iCardList[0])>=15)
							{
								/// ˵��3~10�����Լ�Ҳû���ˣ�������2���������Լ���ͬ��
								OnControlHitPass();
								return;
							}
							m_iNowOutPeople=-1;
							SendGameData(&OutCard,sizeof(OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
							return;
						}
					}
					else if(OutCard.iCardCount == 0)
					{
						/// ˵���Ѿ��Ҳ���
						m_Logic.AutoOutCard(m_Card[GetMeUserInfo()->bDeskStation], m_CardCount[GetMeUserInfo()->bDeskStation], m_iBaseCard,m_iBaseCount,OutCard.iCardList,
							OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
						if(OutCard.iCardCount!=1||m_Logic.GetCardBulk(OutCard.iCardList[0])>=15)
						{
							/// ˵��3~10�����Լ�Ҳû���ˣ�������2���������Լ���ͬ��
							OnControlHitPass();
							return;
						}
						m_iNowOutPeople=-1;
						SendGameData(&OutCard,sizeof(OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
						return;
					}
				}while(true);

			}

		}
		else if(shapeType==UG_DOUBLE)
		{
			if(m_Logic.GetCardBulk(m_iBaseCard[0])<7)
			{
	        	BYTE					m_tempCard[CARD_COUNT_PER_PERSON];
				int                    m_tempCount = m_CardCount[GetMeUserInfo()->bDeskStation];
				memcpy(m_tempCard,m_Card[GetMeUserInfo()->bDeskStation],sizeof(m_tempCard));
				m_tempCount = m_CardCount[GetMeUserInfo()->bDeskStation];
				do{
        			m_Logic.AutoOutCard(m_tempCard,m_tempCount, m_iBaseCard,m_iBaseCount,OutCard.iCardList,
						OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
					if(OutCard.iCardCount>0)
					{
				    	delBulk(m_tempCard,m_tempCount,m_Logic.GetCardBulk(OutCard.iCardList[0]));
						if(OutCard.iCardCount == 2&&m_Logic.GetCardBulk(OutCard.iCardList[0])<15&&m_Logic.GetCardBulk(OutCard.iCardList[0])>=7)
						{
							/// ˵���ҵ����ʵ�����
							m_iNowOutPeople=-1;
							SendGameData(&OutCard,sizeof(OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
							return;
						}
						if(m_tempCount == 0)
						{
							/// ˵��û���ҵ����ʵ��ƣ�ֻ��ѡ����Ժ��ʵ���
							m_Logic.AutoOutCard(m_Card[GetMeUserInfo()->bDeskStation], m_CardCount[GetMeUserInfo()->bDeskStation], m_iBaseCard,m_iBaseCount,OutCard.iCardList,
								OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
							if(OutCard.iCardCount!=2||m_Logic.GetCardBulk(OutCard.iCardList[0])>=15)
							{
								/// ˵��3~10�����Լ�Ҳû���ˣ�������2���������Լ���ͬ��
								OnControlHitPass();
								return;
							}
							m_iNowOutPeople=-1;
							SendGameData(&OutCard,sizeof(OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
							return;
						}
					}
					else if(OutCard.iCardCount == 0)
					{
						/// ˵���Ѿ��Ҳ���
						m_Logic.AutoOutCard(m_Card[GetMeUserInfo()->bDeskStation], m_CardCount[GetMeUserInfo()->bDeskStation], m_iBaseCard,m_iBaseCount,OutCard.iCardList,
							OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
						if(OutCard.iCardCount!=2||m_Logic.GetCardBulk(OutCard.iCardList[0])>=15)
						{
							/// ˵��3~10�����Լ�Ҳû���ˣ�������2���������Լ���ͬ��
							OnControlHitPass();
							return;
						}
						m_iNowOutPeople=-1;
						SendGameData(&OutCard,sizeof(OutCard), MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
						return;
					}
				}while(true);

			}
		}

		OnControlHitPass();
		return;
		
	}
    
}

//ɱ��ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillGameTimer(ID_BEGIN_TIME);
	KillGameTimer(ID_LEAVE_TIME);
	KillGameTimer(ID_CALL_SCORE_TIME);
	KillGameTimer(ID_ROB_NT_TIME);
	KillGameTimer(ID_ADD_DOUBLE_TIME);
	KillGameTimer(ID_SHOW_CARD);
	KillGameTimer(ID_OUT_CARD);
}


///��Ϊû��ͬ���Ԯ�����ڵ�����˵��ȡ��ʤ���ǳ���
///������true���������е��Ʒǳ��ã�������ȡʤ�Ŀ����Ժܴ�falseΪ����
bool CClientGameDlg::canQiangDiZhu()
{

   	std::vector<peiOutCardStruct> pei_card;

    BYTE myCardTemp[CARD_COUNT_PER_PERSON];
	memcpy(myCardTemp,m_Card[GetMeUserInfo()->bDeskStation],sizeof(myCardTemp));


	peiCard(myCardTemp,m_CardCount[GetMeUserInfo()->bDeskStation],pei_card,peiCardAnalQiangDiZhu,this);

	//������ ����������������	ADD BY DWJ
	if((m_pGameInfo->dwRoomRule &GRR_CONTEST))
	{
		OutputDebugString("dwjrobot::�Ǳ�����");
		//���������2��2���ϵĴ��� ��ȥ�е���
		int iNum = 0;
		for(int i=0; i<m_CardCount[GetMeUserInfo()->bDeskStation]; i++)
		{
			if(m_Logic.GetCardNum(myCardTemp[i]) == 2 || myCardTemp[i] == 0x4E || myCardTemp[i] == 0x4F)
			{
				iNum++;
			}
		}
		if(iNum>=2)
		{
			OutputDebugString("dwjrobot::2��2����ȥ�е���");
			m_qiangDiZhu = true;
		}
		else
		{
			OutputDebugString("dwjrobot::2��2���²�ȥ�е���");
		}
	}
	else
	{
		OutputDebugString("dwjrobot::���Ǳ�����");
	}
	return m_qiangDiZhu;   //����������²��߱���������ʵ������ʱ��д���ģ�����֮Ϊ����false
}

///������ʱ�ڵ�AI����
void CClientGameDlg::AI_QiangDiZhu()
{
	if(canQiangDiZhu())
	{

	//	OnControlHitCallScore(3);
		OnCallScore(3,3);
	}
	else
	{
		OutputDebugString("log:���˲���������");
	//	OnControlHitCallScore(0);
		OnCallScore(0,0);
	}
}

/// �����ж��Ƿ񽫷����ӱ�������Լ�ʵ����ǿ��ֻ����Ϊ���᲻��û���ϵ��������ӱ�����
/// �Եù��ڼ٣������Ҫ�ж��Ƿ��ʸ�ӱ����������
/// ������false���ӱ�������ӱ�����
bool CClientGameDlg::canJiaBei()
{


   	std::vector<peiOutCardStruct> pei_card;

    BYTE myCardTemp[CARD_COUNT_PER_PERSON];
	memcpy(myCardTemp,m_Card[GetMeUserInfo()->bDeskStation],sizeof(myCardTemp));


	peiCard(myCardTemp,m_CardCount[GetMeUserInfo()->bDeskStation],pei_card,peiCardAnalJiaBei,this);
	return m_jiaBei;   //����������²��߱���������ʵ������ʱ��д���ģ�����֮Ϊ����false
}

///�ӱ����ʱ�ڵ�AI����
void CClientGameDlg::AI_JiaBei()
{
	if(canJiaBei())
	{
        OutputDebugString("log:����Ҫ�ӱ���");
		//OnControlHitDouble(1);
		OnAddDouble(1,1);
	}
	else
	{
		OutputDebugString("log:���˲��ӱ���");
		//OnControlHitDouble(0);
        OnAddDouble(1,1);
	}
}

/// �Լ�ռȦ֮��ĳ���
void CClientGameDlg::AI_zhanQuanOutCard()
{
	realData data;
	initRealData(data);
    std::vector<peiOutCardStruct> myCard;
	
    multyTree mtree(data);
    mtree.depth(data);    /// ���ɼ�����
	data.disposeDaiPai();
	if(data.m_isSureWin)
	{
	    m_myOutCard = data.m_BigOut;    /// ���ƽ�������Ƿ���Ա�Ӯ����Ӯ�������ַ�������
	    
		CString str;
		str.Format("log:�ҳ�:");
		printCard(m_myOutCard.iCardList,m_myOutCard.iCardCount,str);
		OutputDebugString(str);
		
	}
	else
	{
		m_myOutCard.iCardCount =0;

   		std::vector<peiOutCardStruct> pei_card;

		BYTE myCardTemp[CARD_COUNT_PER_PERSON];
		memcpy(myCardTemp,m_Card[GetMeUserInfo()->bDeskStation],sizeof(myCardTemp));
	    
		m_diaoZhang = -20;           //��ʼһ�����ŵ�������Ŀǰ���Ʒ���ѡ���Ե�������Ϊ����
		peiCard(myCardTemp,m_CardCount[GetMeUserInfo()->bDeskStation],pei_card,peiCardFirstOutCard,this);
	}

	int time = AI_getThinkTime();
//	SetGameTimer(GetMeUserInfo()->bDeskStation,time,ID_AI_OUT_CARD);
}

/// �Լ�û��ռȦ֮��ĳ���
void CClientGameDlg::AI_buZhanQuanQutCard()
{
	CString str;

	str.Format("dwjrobot:��������:");
	printCard(m_Card[GetMeUserInfo()->bDeskStation],(int)m_CardCount[GetMeUserInfo()->bDeskStation],str);
	OutputDebugString(str);
	realData data;
	initRealData(data);
    std::vector<peiOutCardStruct> myCard;

    multyTree mtree(data);
    mtree.depth(data);    /// ���ɼ�����
	data.disposeDaiPai();


	if(data.m_isSureWin)
	{
	    m_myOutCard = data.m_BigOut;    /// ���ƽ�������Ƿ���Ա�Ӯ����Ӯ�������ַ�������
	}
	else
	{
		m_myOutCard.iCardCount = 0;
	}

	if(m_myOutCard.iCardCount!=0)
	{
		str.Format("dwjrobot:�ҳ�:");
		printCard(m_myOutCard.iCardList,m_myOutCard.iCardCount,str);
		OutputDebugString(str);
	}
    
	
	int time = AI_getThinkTime();
	//SetGameTimer(GetMeUserInfo()->bDeskStation,time,ID_AI_OUT_CARD);
}

int CClientGameDlg::AI_getThinkTime()
{
	srand((unsigned)time( NULL ));
    int temp = rand()%100;

	return 1+rand()%4;
}

/// ��ʼ���µ���
void CClientGameDlg::initGuessCard()
{
	static const BYTE m_CardArray[54] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
		0x4E, 0x4F //С�����
	};
	vLeftCard.resize(54);
	for(int i=0;i<54;i++)
	{
		guessCard temp;
		temp.data = m_CardArray[i];
		vLeftCard[i] = temp;
	}
}

/// ȥ���Ѿ��������ߴ������,delCardҪȥ�����ƣ�numΪ��Ӧ������
void CClientGameDlg::deleteGuessCard(BYTE *delCard,int num)
{
	for(int i=0;i<num;i++)
	{
		for(int j=0;j<vLeftCard.size();j++)
		{
			//ɾ����Ӧ����
			if(vLeftCard[j].data == delCard[i])
			{
				vLeftCard.erase(vLeftCard.begin()+j);
				break;
			}
		}
	}

	int n = 0;
	for(int i=0;i<vLeftCard.size();i++)
	{
		if(vLeftCard[i].desk==255)
			n++;
	}
	CString str;
	str.Format("log:������ҹ���%d���ƣ�����%d���������˭����",vLeftCard.size(),n);
	//OutputDebugString(str);

}

/// ���ƵĴ�����Ҫ���ڼ�¼���Ƶ���Ϣ,�͸������Ƶ���Ϣ
/// pOutCardInfoΪ��������
void CClientGameDlg::outCardResult(OutCardMsg * pOutCardInfo)
{
	OutCardStruct temp;
	temp.iCardCount = pOutCardInfo->iCardCount;
	memcpy(temp.iCardList,pOutCardInfo->iCardList,sizeof(temp.iCardList));
	vChuCard[pOutCardInfo->bDeskStation].push_back(temp);
	if(pOutCardInfo->bDeskStation != GetMeUserInfo()->bDeskStation)
	{
       deleteGuessCard(temp.iCardList,temp.iCardCount);
	}

}

/// ������������ŵ��ƣ�����������Լ���ֱ��ȥ���������ƣ������ڲ����н���������ȷ����������˭��
/// ����pBackCard�ǵ��Ƶ���Ϣ
void CClientGameDlg::diPai(BackCardExStruct * pBackCard)
{
	if(pBackCard->iGiveBackPeople == GetMeUserInfo()->bDeskStation)
	{
		deleteGuessCard(pBackCard->iBackCard,pBackCard->iBackCardCount);
	}
	else
	{
		for(int i = 0;i<pBackCard->iBackCardCount;i++)
		{
			for(int j=0;j<vLeftCard.size();j++)
			{
				if(vLeftCard[j].data == pBackCard->iBackCard[i])
				{
					vLeftCard[j].desk = pBackCard->iGiveBackPeople;
					break;
				}
			}
		}
	}
	m_iNtPeople = pBackCard->iGiveBackPeople;

	if(pBackCard->iGiveBackPeople == GetMeUserInfo()->bDeskStation)
	{
		m_index =0;
		OutputDebugString("log:������λ���Ѿ�ȷ������,�������Լ�");
	}
	else if((GetMeUserInfo()->bDeskStation+1)%3 == pBackCard->iGiveBackPeople)
	{

		m_index = 1;
		OutputDebugString("log:������λ���Ѿ�ȷ������,�������¼�");
	}
	else
	{
		m_index = 2;
		OutputDebugString("log:������λ���Ѿ�ȷ������,�������ϼ�");


	}
	int n = 0;
	for(int i=0;i<vLeftCard.size();i++)
	{
		if(vLeftCard[i].desk==255)
			n++;
	}
	CString str;
	str.Format("log:������ҹ���%d���ƣ�����%d���������˭����",vLeftCard.size(),n);
	//OutputDebugString(str);

	str = "log:ץ�����е���Ϊ��";
	n=m_CardCount[GetMeUserInfo()->bDeskStation];
	printCard(m_Card[GetMeUserInfo()->bDeskStation],n,str);
	OutputDebugString(str);
    

}

/// ���ڳ�ʼ���ƾ�������״̬�ڵ��״̬��Ϣ
void CClientGameDlg::initRealData(realData &data)
{
	memcpy(data.m_CardCount,m_CardCount,sizeof(m_CardCount));
	data.m_myCardCount = m_CardCount[GetMeUserInfo()->bDeskStation];
	memcpy(data.m_DeskCard,m_Card[GetMeUserInfo()->bDeskStation],sizeof(BYTE)*CARD_COUNT_PER_PERSON);
	data.m_index = m_index;
	data.m_iNtPeople = m_iNtPeople;
	data.m_myDeskStation = GetMeUserInfo()->bDeskStation;
	data.m_iBigOutPeople = m_iBigOutPeople;
	data.m_iNowOutPeople = m_iNowOutPeople;
	data.m_BigOut.iCardCount = m_iBaseCount;
	memcpy(data.m_BigOut.iCardList,m_iBaseCard,sizeof(m_iBaseCard));
	for(int i=0;i<3;i++)
	{
		
		data.m_DaiDanCard[i].clear();
		data.m_DaiDuiCard[i].clear();
		data.m_DaiPaiData[i].clear();
	}
	data.vLeftCard = vLeftCard;
	if(vLeftCard.size())
	{
		OutputDebugString("log:��������");
	}
}

/// ��õ��Ƶ���ʽ���ж� ı
/// handCard,num��������ʣ�µĵ��ơ�2������myCard�������˳�����Ŷ��ӵ�
/// @return ����õ��Ʒ���Ҫ��ʱ����true�����򷵻�true
bool peiCardAnalQiangDiZhu(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg)
{
	static int index = 1;
	CString str;
	str.Format("log:��%d����ɵ��ƵĽ��",index);
	//OutputDebugString(str);
	str = "log:ʣ�����е���Ϊ��";
	printCard(handCard,num,str);
//	OutputDebugString(str);
	for(int i=0;i<myCard.size();i++)
	{
		str.Format("log:��õĵ�%d��Ϊ��",i+1);
		printCard(myCard[i].out.iCardList,myCard[i].out.iCardCount,str);
	//	OutputDebugString(str);
	}
    index++;
    int diaoNum = pClientGameDlg->computeDiaoZhang(handCard,num,myCard);
	str.Format("log:���ŵ�����Ϊ%d��",diaoNum);
	//OutputDebugString(str);
    
    if((pClientGameDlg->computeDingzhang(myCard)-diaoNum)>0)
	{
		pClientGameDlg->m_qiangDiZhu = true;
		return true;
	}
	pClientGameDlg->m_qiangDiZhu = false;
	return false;
}
/// ��õ��Ƶ���ʽ���ж�
/// handCard,num��������ʣ�µĵ��ơ�2������myCard�������˳�����Ŷ��ӵ�
/// @return ����õ��Ʒ���Ҫ��ʱ����true�����򷵻�true
bool peiCardAnalJiaBei(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg)
{
	static int index = 1;
	CString str;
	str.Format("log:��%d����ɵ��ƵĽ��",index);
	//OutputDebugString(str);
	str = "log:ʣ�����е���Ϊ��";
	printCard(handCard,num,str);
	//OutputDebugString(str);
	for(int i=0;i<myCard.size();i++)
	{
		str.Format("log:��õĵ�%d��Ϊ��",i+1);
		printCard(myCard[i].out.iCardList,myCard[i].out.iCardCount,str);
	//	OutputDebugString(str);
	}
    index++;
    int diaoNum = pClientGameDlg->computeDiaoZhang(handCard,num,myCard);
	str.Format("log:���ŵ�����Ϊ%d��",diaoNum);
	//OutputDebugString(str);
    
    if((pClientGameDlg->computeDingzhang(myCard)-diaoNum)>-1)
	{
		pClientGameDlg->m_jiaBei = true;
		return true;
	}
	pClientGameDlg->m_jiaBei = false;
	return false;
}

/// �ж���õ����Ƿ�����Ҫ��
/// handCard,num��������ʣ�µĵ��ơ�2������myCard�������˳�����Ŷ��ӵ�
/// @return ����õ��Ʒ���Ҫ��ʱ����true�����򷵻�true
bool peiCardFirstOutCard(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg)
{
	static int index = 1;
	CString str;
	str.Format("log:��%d����ɵ��ƵĽ��",index);
	//OutputDebugString(str);
	str = "log:ʣ�����е���Ϊ��";
	printCard(handCard,num,str);
//	OutputDebugString(str);
	for(int i=0;i<myCard.size();i++)
	{
		str.Format("log:��õĵ�%d��Ϊ��",i+1);
		printCard(myCard[i].out.iCardList,myCard[i].out.iCardCount,str);
	//	OutputDebugString(str);
	}
    index++;
    int diaoNum = pClientGameDlg->computeDiaoZhang(handCard,num,myCard);
	str.Format("log:���ŵ�����Ϊ%d��",diaoNum);
//	OutputDebugString(str);
    int diaozhang =pClientGameDlg->computeDingzhang(myCard)-diaoNum;
    if(diaozhang>pClientGameDlg->m_diaoZhang)   ///˵���µ����Ʒ�������һЩ
	{

		pClientGameDlg->m_diaoZhang = diaozhang;
		/// ĿǰΪ����Ӧ��ʱ���������һ�����ԱȽϺ���ĳ��ƵĻ�����
		for(int i=0;i<myCard.size();i++)
		{
			if(myCard[i].type == 5)   //˫˳
			{
				pClientGameDlg->m_myOutCard = myCard[i].out;
                return false;
			}
		}
		for(int i=0;i<myCard.size();i++)
		{
			if(myCard[i].type == 6)   //��˳
			{
				pClientGameDlg->m_myOutCard = myCard[i].out;
                return false;
			}
		}
		for(int i=0;i<myCard.size();i++)
		{
			if(myCard[i].type == 4)   //��˳
			{
				pClientGameDlg->m_myOutCard = myCard[i].out;
				/// ������˳����
				if(num>=(myCard[i].out.iCardCount/3))
				{
					int cardNum = 2;   ///�����Ƶ�����
					while(pClientGameDlg->m_myOutCard.iCardCount<myCard[i].out.iCardCount/3*4)
					{
						int smallCardNum = 15;   //���ҷ�Χ��3~A���������14
						int smallIndex =-1;
						for(int j=0;j<num;j++)
						{
							if(logic.GetCardBulk(handCard[j])<smallCardNum&&logic.GetCardBulk(handCard[j])>cardNum)
							{
                                smallCardNum = logic.GetCardBulk(handCard[j]);
								smallIndex = j;
							}
						}
						cardNum = smallCardNum;
						pClientGameDlg->m_myOutCard.iCardList[pClientGameDlg->m_myOutCard.iCardCount++] = handCard[smallIndex];
					}

				}
				else
				{   
					///��Ϊ���Ӷ��Ǵ�С�������еģ�����ʡ�˺ܶ������
					for(int j=0;j<myCard.size();j++)
					{
						if(myCard[j].type == 8)
						{
							pClientGameDlg->m_myOutCard.iCardList[pClientGameDlg->m_myOutCard.iCardCount++] = myCard[j].out.iCardList[0];
							pClientGameDlg->m_myOutCard.iCardList[pClientGameDlg->m_myOutCard.iCardCount++] = myCard[j].out.iCardList[1];
							if(pClientGameDlg->m_myOutCard.iCardCount>=myCard[i].out.iCardCount/3*5)
								return false;
						}
					}
                    pClientGameDlg->m_myOutCard.iCardCount = myCard[i].out.iCardCount;
				}
                return false;
			}
		}
		for(int i=0;i<myCard.size();i++)
		{
			
			if(myCard[i].type == 7)   //����
			{
				OutputDebugString("log:������������");
				pClientGameDlg->m_myOutCard = myCard[i].out;
				/// �������Ŵ���
				if(num>0)
				{
					int smallCardNum = 15;   //���ҷ�Χ��3~A���������14
					pClientGameDlg->m_myOutCard.iCardCount = 4;
					for(int j=0;j<num;j++)
					{
						if(logic.GetCardBulk(handCard[j])<smallCardNum)
						{
                            pClientGameDlg->m_myOutCard.iCardList[3] = handCard[j];
                            smallCardNum = logic.GetCardBulk(handCard[j]);
						}
					}

					return false;
				}
				else
				{   
					///��Ϊ���Ӷ��Ǵ�С�������еģ�����ʡ�˺ܶ������
					for(int j=0;j<myCard.size();j++)
					{
						if(myCard[j].type == 8)
						{
							pClientGameDlg->m_myOutCard.iCardCount = 5; 
							pClientGameDlg->m_myOutCard.iCardList[3] = myCard[j].out.iCardList[0];
							pClientGameDlg->m_myOutCard.iCardList[4] = myCard[j].out.iCardList[1];
							return false;
						}
					}
					for(int j=0;j<myCard.size();j++)
					{
						if(myCard[j].type == 2)
						{
							if(myCard[j].out.iCardCount == 1)
							{
								pClientGameDlg->m_myOutCard.iCardCount = 4; 
								pClientGameDlg->m_myOutCard.iCardList[3] = myCard[j].out.iCardList[0];
							}
							else if(myCard[j].out.iCardCount == 2)
							{
								pClientGameDlg->m_myOutCard.iCardCount = 4; 
								pClientGameDlg->m_myOutCard.iCardList[3] = myCard[j].out.iCardList[0];
								pClientGameDlg->m_myOutCard.iCardList[4] = myCard[j].out.iCardList[1];
							}
							return false;

						}
					}
					for(int j=0;j<myCard.size();j++)
					{
						if(myCard[j].type == 1)
						{
							if(myCard[j].out.iCardCount == 1)
							{
								pClientGameDlg->m_myOutCard.iCardCount = 4; 
								pClientGameDlg->m_myOutCard.iCardList[3] = myCard[j].out.iCardList[0];
							}
							return false;

						}
					}
                    
				}
				CString str;
				str.Format("log:�ҳ���������:");
				printCard(pClientGameDlg->m_myOutCard.iCardList,pClientGameDlg->m_myOutCard.iCardCount,str);
				OutputDebugString(str);
			}
		}
        pClientGameDlg->m_myOutCard.iCardCount = 0;
		return false;
	}
	
	return false;
}

/// ���㵥�ŵ��ŵ����� 10���µ� 
int CClientGameDlg::computeDanDiaoZhang(BYTE* handCard,int num)
{
	int result = 0;
    for(int i=0;i<num;i++)
	{
		///����С��10
		if(logic.GetCardNum(handCard[i])<=10)
			result++;
	}
    return result;
}
/// ������ӵ��ŵ����� 7����
int CClientGameDlg::computeduiDiaoZhang(std::vector<peiOutCardStruct>& myCard)
{
	int result = 0;
	for(int i=0;i<myCard.size();i++)
	{
		if(myCard[i].type == 8)
		{
			if(logic.GetCardNum(myCard[i].out.iCardList[0])<=7)
				result++;
		}
	}
	return result;
}

/// ���㽫��˳������������ը��������µ��ŵ�����
int CClientGameDlg::computeDiaoZhang(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard)
{
	int danDiao = computeDanDiaoZhang(handCard,num);  //���ŵ��Ƶ�����
	int duiDiao = computeduiDiaoZhang(myCard);
	int zhaDan = 0;
	int shun3Num = 0;
	int sanNum = 0;
	for(int i=0;i<myCard.size();i++)
	{
        if(myCard[i].type == 4)
		{
			int canDai = myCard[i].out.iCardCount/3;  ///�ó����Դ����Ƶ�����
			if(danDiao>=canDai)
			{
				danDiao -= canDai;
			}
			else if(duiDiao>=canDai)
			{
				duiDiao -= canDai;
			}
			else
			{
				shun3Num += canDai;	
			}
		}
		else if(myCard[i].type == 7)
		{
			sanNum++;
		}
	}

	///  ��Ϊ���������Ϊһ���ο�����������˳���ֶ����Ų������ʱ������Ƿ����������ж�

	return (duiDiao+danDiao-shun3Num-sanNum-zhaDan);

}

/// ���㶨�ŵ�������2����ը��
int CClientGameDlg::computeDingzhang(std::vector<peiOutCardStruct>& myCard)
{
	int result = 0;
	for(int i=0;i<myCard.size();i++)
	{
		if(myCard[i].type == 1||myCard[i].type == 2||myCard[i].type ==3)
		{
			result++;
		}
	}
	return result;
}