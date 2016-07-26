#include "StdAfx.h"
#include "ClientGameDlg.h"
#include ".\clientgamedlg.h"


BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(IDM_CALL_SCORE,OnCallScore)				//�з�

	ON_MESSAGE(IDM_AUTO,OnHitAuto)				//�й�
	
	ON_MESSAGE(IDM_ACTIVE_RESULT,OnHitActiveResult)//�û�������
	ON_MESSAGE(IDM_SHOW_NOTE,OnShowNote)			//�û���ע��ע������
//	ON_MESSAGE(IDM_SELECT_NOTE_NUM,OnSelectNote)		//�û�ѡ����ע
//	ON_MESSAGE(WM_SENDCARD_SHOWCARD,SendCardShowCard)
	//	ON_MESSAGE(WM_SETMESSAGE,OnSetMessage)
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
	m_bCurrentOperationStation = 255;//��ǰ�з���
	//ϵͳ������
	//m_iSysCardCount=28;
	m_iNowBigNote=0;				//��ǰ���Ѻע��
	m_iThisTurnLimit=0;				//������ע
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iUpBullCard,1,sizeof(m_iUpBullCard));
	m_bBgSound=true;
	m_GameType = 0;
	m_hMciWnd1=NULL;
	m_hMciWnd2=NULL;
	m_bCancel = false;
	//	WriteProfileInt(TEXT("ShowPicture"),GameSetDlg.m_bRightPower?1:0);

	// ��¼ÿ����Ҵ�����
	memset(m_iMoneys, 0, sizeof(m_iMoneys));

	m_iCurrentMoney = 0;	// ��ǰҪ����Ľ��
	m_iMinMoney = 0;		// ��С������
	m_iMaxMoney = 0;		// ��������
	m_iSelfMoney = 0;		// �Լ�����ӵ�еĽ��

	//ׯ��λ��
	m_byNTUser = 255;

	//Сäעλ��
	m_bySmallBlind = 255;

	//��äעλ��
	m_byBigBlind = 255;

	//�õ����Ƶ����
	m_byTokenUser = 255;

	m_nTypeCard = UNKNOWN;	// δ֪����

	// �������ܹ���������
	memset(m_byRobotInAllCard, 255,sizeof(m_byRobotInAllCard));

	// �������ܹ�������
	m_nRobotInAllCardNum = 0;	
}

//��������
CClientGameDlg::~CClientGameDlg()
{
	if(m_hMciWnd1)
	{
//		MCIWndStop(m_hMciWnd1);
//		MCIWndDestroy(m_hMciWnd1);
		m_hMciWnd1=NULL;
	}

	if(m_hMciWnd2)
	{
//		MCIWndStop(m_hMciWnd2);
//		MCIWndDestroy(m_hMciWnd2);
		m_hMciWnd2=NULL;
	}	

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
void CClientGameDlg::InitData()
{
	CINIFile iniFile(CINIFile::GetAppPath()+ "Setup.ini");
	m_bCancel =  iniFile.GetKeyVal("Cancel", "Cancel" , 0) ; ///�Ƿ��˳�

	// ȫ�����
	memset(m_bAllBet, 0, sizeof(m_bAllBet));

	// �������
	memset(m_bGiveUp, 0, sizeof(m_bGiveUp));

	// ÿ�������ע���
	memset(m_nBetMoney, 0, sizeof(m_nBetMoney));

	//��������
	memset(m_iUserDataCard, 0, sizeof(m_iUserDataCard));

	//������
	m_iCardCounts = 0;

	// ���������ʾ
	memset(m_iCardData, 0, sizeof(m_iCardData));

	//������3
	m_iCardDataCounts = 0;

	//��ʼ�Լ��˿˱�
	::memset(m_byMyCards,0,sizeof(m_byMyCards));

	//�û������˿���Ŀ
	m_iCardsNum = 0;

	// ��һ�β�������
	memset(&m_emLastOperation, ET_UNKNOWN,sizeof(m_emLastOperation));

	// ��ע���
	m_nCallMoney = 0;

	// ��ʼ���ۼӽ��
	m_nCurrentMoney = 0;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	const TGSBase* pGSBase = (TGSBase*)pStationData;

	m_byMeStation = GetMeUserInfo()->bDeskStation;

	//��ԭ������������
	::memcpy(&m_tagDeskCfg,&pGSBase->tagDeskCfg,sizeof(TDeskCfg));
	
	// ��ǰ����ʱ��
	m_byCurveOperationTimer = pGSBase->tagDeskCfg.Time.byOperate;

	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
	case GS_WAIT_NEXT_ROUND:
		{
			TGSWaitAgree* pGSWaitAgree = (TGSWaitAgree*)pStationData;
			
			// �����˵�ǰ׼������Ľ��
			m_iCurrentMoney = pGSWaitAgree->tagDeskCfg.dz.iSubPlanMoney;
			m_iMinMoney = pGSWaitAgree->tagDeskCfg.dz.iSubMinMoney;
			m_iMaxMoney	= pGSWaitAgree->tagDeskCfg.dz.iSubMaxMoney;
			
			// ������ݲ���Ⱦ͸���
			if (m_iCurrentMoney < m_iMinMoney)
			{
				m_iCurrentMoney = m_iMinMoney;
			}
			else if (m_iCurrentMoney > m_iMaxMoney)
			{
				m_iCurrentMoney = m_iMaxMoney;
			}
			
			m_iSelfMoney = GetMeUserInfo()->i64Money;
			
			// ��ֹ�����ҳ���������
			if (m_iCurrentMoney > m_iSelfMoney)
			{
				m_iCurrentMoney = m_iSelfMoney;
			}

			srand((unsigned) time (NULL) + m_byMeStation) ; 

			int iRandTime = rand() % 10 + 1; 

			// �������Զ������ʱ��
			SetGameTimer(m_byMeStation,iRandTime,IDT_IN_MONEY_TIMER);
		}
		break;
	case GS_PLAY_GAME:
		{
			TGSPlaying * pGSPlaying = (TGSPlaying *)pStationData;

			// ��õõ����Ƶ����
			m_byTokenUser = pGSPlaying->byTokenUser;

			// ׯ�Ҵ�С��
			m_byNTUser = pGSPlaying->byNTUser;
			m_bySmallBlind = pGSPlaying->bySmallBlind;
			m_byBigBlind = pGSPlaying->byBigBlind;

			// ��������
			m_iCardCounts = pGSPlaying->nHandCardNums;
			memcpy(m_iUserDataCard, pGSPlaying->byHandCard, sizeof(BYTE) * m_iCardCounts);

			// ȫ�����
			memcpy(m_bAllBet, pGSPlaying->bNoBet, sizeof(pGSPlaying->bNoBet));

			// �������
			memcpy(m_bGiveUp, pGSPlaying->bGiveUp, sizeof(pGSPlaying->bGiveUp));

			// ��ȡ������
			memcpy(m_iMoneys, pGSPlaying->nSubMoney, sizeof(pGSPlaying->nSubMoney));

			for (int i = 0; i < PLAY_COUNT; i++)
			{
				// ��ʾ��ע���
				m_nBetMoney[i] = 0;

				for (int j = 0; j < PLAY_COUNT; j++)
				{
					m_nBetMoney[i] += pGSPlaying->nBetMoney[i][j];
				}
			}
			
			// ��ǰ�������
			TToken toKen;
			toKen.bNewTurn	 = pGSPlaying->bNewTurn;
			toKen.byUser	 = pGSPlaying->byTokenUser;
			toKen.byVerbFlag = pGSPlaying->byVerbFlag;
			toKen.nCallMoney = pGSPlaying->nCallMoney;
		}
		break;
	default:{break;}
	}
	return true;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID == MDM_GM_GAME_NOTIFY)
	{
		switch(pNetHead->bAssistantID)

		{
		case ASS_GAME_BEGIN:
			{
				//��Ϸ��ʼ
				SetStationParameter(GS_PLAY_GAME);
				// ��ʼ������
				InitData();
				return true;
			}
		case ASS_SYS_DESK_CFG:
			{
				//�յ�ϵͳ�������ӵ���Ϣ����������
				TDeskCfg* pDeskCfg = (TDeskCfg*)pNetData;

				//��������
				::memcpy(&m_tagDeskCfg,pDeskCfg,sizeof(TDeskCfg));
				return true;
			}
		case ASS_CFG_DESK: 
			{
				//������Ϸ��״̬
				SetStationParameter(GS_CONFIG_NOTE);
				return true;
			}
		case ASS_CFG_DESK_TIMEOUT:
			{
				return true;
			}
		case ASS_DESK_CFG: 
			{
				//������Ϸ��״̬
				SetStationParameter(GS_CONFIG_NOTE);

				TDeskCfg* pDeskCfg = (TDeskCfg*)pNetData;

				//��������
				::memcpy(&m_tagDeskCfg,pDeskCfg,sizeof(TDeskCfg));
				return true;
			}
		case ASS_SHOW_IN_MONEY: 
			{
				//�յ���������Ϣ����������
				TSubMoney* pSubMoney = (TSubMoney*)pNetData;

				memcpy(m_iMoneys, pSubMoney->nMoney, sizeof(m_iMoneys));
				return true;
			}
		case ASS_SEND_A_CARD:
			{
				//�յ���������һ���ư���������
				TCards* pCards = (TCards*)pNetData;

				if (m_byNTUser == 255)
				{
					return false;
				}

				m_nTypeCard = pCards->nTypeCard;	

				// �Լ���������
				::memcpy(m_iUserDataCard, pCards->byCards, MAX_HANDCARDS_NUM);
				m_iCardCounts = pCards->iCardsNum;

				memset(m_byRobotInAllCard,255,sizeof(m_byRobotInAllCard));
				m_nRobotInAllCardNum = 0;
				// ���Լ��������ݿ�������������������
				::memcpy(m_byRobotInAllCard,pCards->byCards,sizeof(BYTE)*pCards->iCardsNum);
				m_nRobotInAllCardNum = pCards->iCardsNum;

				SendGameData(MDM_GM_GAME_NOTIFY, ASS_SEND_CARD_FINISH, NULL);

				return true;
			}
		case ASS_BETPOOL_UP:
			{
				//�յ��������߳ظ��°���������
				TBetPool* pBetPool = (TBetPool *)pNetData;

				memcpy(&m_tagBetPool, pBetPool, sizeof(m_tagBetPool));

				return true;
			}
		case ASS_SEND_3_CARD:
			{
				//�յ���������һ���ư���������
				TCards * pCards = (TCards *)pNetData;
				
				// �������Ź�����
				memcpy(m_iCardData, pCards->byCards, sizeof(BYTE) * (pCards->iCardsNum));
				m_iCardDataCounts += pCards->iCardsNum;

				// ���Լ��������ݿ�������������������
				::memcpy(&m_byRobotInAllCard[m_nRobotInAllCardNum],pCards->byCards,sizeof(BYTE)*pCards->iCardsNum);
				m_nRobotInAllCardNum += pCards->iCardsNum;

				return true;
			}			
		case ASS_SEND_4_5_CARD:
			{
				//�յ���������һ���ư���������
				TCards * pCards = (TCards *)pNetData;

				// ��������������Ź�����
				memcpy(m_iCardData, pCards->byCards, sizeof(BYTE) * (pCards->iCardsNum));
				m_iCardDataCounts = pCards->iCardsNum;

				// ���Լ��������ݿ�������������������
				::memcpy(&m_byRobotInAllCard[m_nRobotInAllCardNum],pCards->byCards,sizeof(m_byRobotInAllCard[m_nRobotInAllCardNum]));
				m_nRobotInAllCardNum += 1;

				return true;
			}
		case ASS_SEND_CARD_FINISH:
			{
				//�յ�����������������ҷ�����ɵ���Ϣ��������
				
				return true;
			}
		case ASS_BET_INFO:
			{
				//�յ������ע����Ϣ
				TBet* pBet = (TBet*)pNetData;

				if (pBet->nType == ET_AUTO || pBet->nType == ET_BET || pBet->nType == ET_ADDNOTE || pBet->nType == ET_ALLIN || pBet->nType == ET_CALL)
				{
					m_nBetMoney[pBet->byUser] += pBet->nMoney; 
					m_iMoneys[pBet->byUser] -= pBet->nMoney;
				}

				// ��¼ȫ����������
				if (pBet->nType == ET_ALLIN)
				{
					m_bAllBet[pBet->byUser] = true;
				}
				else if (pBet->nType == ET_FOLD)
				{
					m_bGiveUp[pBet->byUser] = true;		
				}

				// ��¼�����Ҳ�������
				m_emLastOperation[pBet->byUser] = pBet->nType;

				return true;
			}
		case ASS_TOKEN:
			{
				//�յ�������Ϣ�������û�
				TToken* pToken = (TToken*)pNetData;

				//��������
				m_byTokenUser = pToken->byUser;

				// ��������־
				BYTE byVerbFlag = pToken->byVerbFlag;

				int iWeight = 0;
				int nRandByWeight = 0;

				// ģ����ע��Ϣ
				if (m_byTokenUser == m_byMeStation)
				{
					if (m_nRobotInAllCardNum >= 2)
					{
						iWeight = m_Logic.RobotHandCardAnalysis(m_byRobotInAllCard,m_nRobotInAllCardNum);

						if ((byVerbFlag & UD_VF_BET) && (byVerbFlag & UD_VF_CHECK) && (byVerbFlag & UD_VF_FOLD))
						{
							nRandByWeight = GetRandBetByWeight(iWeight);
						}
						else if ((byVerbFlag & UD_VF_ADD) && (byVerbFlag & UD_VF_CALL) && (byVerbFlag & UD_VF_FOLD))
						{
							nRandByWeight = GetRandAddByWeight(iWeight);
						}
						else if ((byVerbFlag & UD_VF_ALLIN) && (byVerbFlag & UD_VF_FOLD))
						{
							nRandByWeight = GetRandAllinByWeight(iWeight);
						}
					}

					switch (byVerbFlag & nRandByWeight)
					{
					case UD_VF_BET:		// ������ע
						{
							srand((unsigned) time (NULL) + m_byMeStation);
							int iRandTime = rand() % 5 + 1;
							SetGameTimer(m_byMeStation,iRandTime,IDT_BET_TIMER);
							break;
						}
					case UD_VF_CALL:	// �����ע
						{
							m_nCallMoney = pToken->nCallMoney;
							srand((unsigned) time (NULL) + m_byMeStation);
							int iRandTime = rand() % 5 + 1;
							SetGameTimer(m_byMeStation,iRandTime,IDT_CALL_TIMER);
							break;
						}
					case UD_VF_ADD:		// �����ע
						{
							m_nCallMoney = pToken->nCallMoney;
							srand((unsigned) time (NULL) + m_byMeStation);
							int iRandTime = rand() % 5 + 1;
							SetGameTimer(m_byMeStation,iRandTime,IDT_RAISE_TIMER);
							break;
						}
					case UD_VF_CHECK:  // �������
						{
							srand((unsigned) time (NULL) + m_byMeStation);
							int iRandTime = rand() % 5 + 1;
							SetGameTimer(m_byMeStation,iRandTime,IDT_CHECK_TIMER);
							break;
						}
					case UD_VF_FOLD:   // ��������
						{
							srand((unsigned) time (NULL) + m_byMeStation);
							int iRandTime = rand() % 7 + 1;
							SetGameTimer(m_byMeStation,iRandTime,IDT_FOLO_TIMER);
							break;
						}
					case UD_VF_ALLIN:  // ����ȫ��
						{
							__int64 nMoney = m_nBetMoney[GetLastUserStation(m_byMeStation)] - m_nBetMoney[m_byMeStation];

							if (m_iMoneys[m_byMeStation] <= nMoney)
							{
								srand((unsigned) time (NULL) + m_byMeStation);
								int iRandTime = rand() % 5 + 1;

								SetGameTimer(m_byMeStation,iRandTime,IDT_ALLIN_TIMER);
							}
							break;
						}
					default:
						{
							OutputDebugString("jan: δ֪����\n");
							break;
						}
					}	
				}

				return true;
			}
		case ASS_COMPARE_CARD:
			{
				//������Ϣ
				TCompareCard *pCmd = (TCompareCard*)pNetData;

				return true;
			}
		case ASS_RESULT:
			{
				//������Ϸ��״̬
				SetStationParameter(GS_WAIT_NEXT_ROUND);
				//�յ�������Ϣ����������
				TResult* pResult = (TResult*)pNetData;

				//��¼��Ϸ����
				::memcpy(&m_tagResult,pResult,sizeof(TResult));

				if (m_iMoneys[m_byMeStation] <= m_tagDeskCfg.dz.iUntenMoney)
				{
					// �����˵�ǰ׼������Ľ��
					m_iCurrentMoney = m_tagDeskCfg.dz.iSubPlanMoney;

					// ������ݲ���Ⱦ͸���
					if (m_iCurrentMoney < m_iMinMoney)
					{
						m_iCurrentMoney = m_iMinMoney;
					}
					else if (m_iCurrentMoney > m_iMaxMoney)
					{
						m_iCurrentMoney = m_iMaxMoney;
					}

					m_iSelfMoney = GetMeUserInfo()->i64Money;

					// ��ֹ�����ҳ���������
					if (m_iCurrentMoney > m_iSelfMoney)
					{
						m_iCurrentMoney = m_iSelfMoney;
					}


					if (m_iSelfMoney < m_iMinMoney)
					{
						// ������������ʹ��뽫����߳�
						if (m_pUserInfo[m_byMeStation] != NULL)
						{
							if (m_tagResult.nSelfMoney[m_byMeStation] < m_tagDeskCfg.dz.iSubMinMoney)
							{
								SetGameTimer(m_byMeStation, 500, IDT_MONEY_LACK);
							}
						}
						return false;
					}

					srand((unsigned) time (NULL) + m_byMeStation) ; 
					int iRandTime = rand() % 10 + 1; 

					// �������Զ������ʱ��
					SetGameTimer(m_byMeStation,iRandTime,IDT_IN_MONEY_TIMER);
				}
				else
				{
					srand((unsigned) time (NULL) + m_byMeStation) ; 
					int iRandTime = rand() % 10 + 1; 

					// �������Զ������ʱ��
					SetGameTimer(m_byMeStation,iRandTime,IDT_PODO_MONEY_TIMER);
				}

				return true;
			}
		case ASS_NEW_ROUND_BEGIN:
			{
				//�յ��µ�һ�غϿ�ʼ��Ϣ����������
				TNextRoundBeginInfo* pNextRoundBeginInfo = (TNextRoundBeginInfo*)pNetData;

				//��¼ׯ�ҵ�λ��
				m_byNTUser = pNextRoundBeginInfo->byNTUser;

				//��¼��Сäע
				m_bySmallBlind = pNextRoundBeginInfo->bSmallBlind;
				m_byBigBlind = pNextRoundBeginInfo->bBigBlind;


				//������գ���������
				::memset(m_byBackCard,0,MAX_BACKCARDS_NUM);
				m_iBackCardNums = 0;


				//��ʼһЩ��������/////////////////////////////

				TDeskCfg::TRule* pRule = &m_tagDeskCfg.Rule;
				return true;
			}
		case ASS_NO_PLAYER:
			{
				//û����ҽ�����Ϸ���˳�����
				//OnHandleNoPlayer(buffer,nLen);
				return true;
			}
		case ASS_AGREE:
			{
				//����Ƿ�ͬ�����Ϸ�ģ�����
				//return m_GameMachine.OnHandleAgree(pNetData,uDataSize);
				return true;
			}
		case ASS_NOT_ENOUGH_MONEY:
			{
				//������ϵ�Ǯ���㹻ʱ������
				//OnHandleNotEnoughMoney(buffer,nLen);
				return true;
			}
		case ASS_USER_LEFT_DESK:
			{
				return true;
			}
		case ASS_SORT_OUT:
			{
				//ϵͳ�Զ�����Ƭ
				//OnUserSortOut();
				return true;
			}
		default:
			break;

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
	KillTimer(ID_OUT_CARD);
	KillTimer(ID_LOOK_BACK);
	KillTimer(ID_NOTE);
	KillTimer(ID_CALL_SCORE_TIME);
	return;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case IDT_CANCEL_TIME:
		{
			KillAllTimer();
			AFCCloseFrame();
			__super::OnCancel();
			SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
			break;
		}
	case IDT_MONEY_LACK:
		{
			SendGameData(ASS_MONEY_LACK, NULL,0);
			break;
		}
	case IDT_IN_MONEY_TIMER:	// �����ʱ��
		{
			KillTimer(IDT_IN_MONEY_TIMER);
			// ȷ�ϴ���	
			TSubMoney cmd;
			cmd.nMoney[m_byMeStation] = m_iCurrentMoney;
			cmd.bDeskStation = m_byMeStation;
			SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);
			break;
		}
	case IDT_IN_MONEY_TIMERS:	// �����ʱ��
		{
			KillTimer(IDT_IN_MONEY_TIMERS);
			OutputDebugString("DKSS:�����ʱ��");

			if (m_bCancel)
			{
				OnCancel();
			}
			else
			{
				// ȷ�ϴ���	
				TSubMoney cmd;
				cmd.nMoney[m_byMeStation] = m_iCurrentMoney;
				cmd.bDeskStation = m_byMeStation;
				SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);
			}
			break;			
		}
	case IDT_PODO_MONEY_TIMER: // �����˻��н�������´���	
		{
			if (m_bCancel)
			{
				OnCancel();
			}
			else
			{
				// �Զ�׼��(����)
				TSubMoney cmd;
				cmd.nMoney[m_byMeStation] = m_iMoneys[m_byMeStation];
				cmd.bDeskStation = m_byMeStation;
				SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);
			}			
			break;
		}
	case IDT_BET_TIMER:			// ��ע��ʱ��
		{
			KillTimer(IDT_BET_TIMER);

			TBet tagBet;
			tagBet.nType = ET_BET;
			tagBet.nMoney = m_tagDeskCfg.dz.iMinMoney;
			SendGameData(&tagBet, sizeof(tagBet), MDM_GM_GAME_NOTIFY, ASS_BET, 0);	
			break;
		}
	case IDT_CALL_TIMER:			// ��ע��ʱ��
		{
			KillTimer(IDT_CALL_TIMER);
			TBet tagBet;
			tagBet.nType = ET_CALL;
			tagBet.nMoney = m_nCallMoney;

			SendGameData(&tagBet, sizeof(tagBet), MDM_GM_GAME_NOTIFY, ASS_BET, 0);	
			break;
		}
	case IDT_RAISE_TIMER:			// ��ע��ʱ��
		{
			KillTimer(IDT_RAISE_TIMER);
			TBet tagBet;
			tagBet.nType = ET_ADDNOTE;
			tagBet.nMoney = m_nCallMoney + 500;

			SendGameData(&tagBet, sizeof(tagBet), MDM_GM_GAME_NOTIFY, ASS_BET, 0);
			break;
		}
	case IDT_CHECK_TIMER:			// ���ƶ�ʱ��
		{
			KillTimer(IDT_CHECK_TIMER);
			TBet tagBet;
			tagBet.nType = ET_CHECK;
			SendGameData(&tagBet, sizeof(tagBet), MDM_GM_GAME_NOTIFY, ASS_BET, 0);
			break;
		}
	case IDT_FOLO_TIMER:			// ���ƶ�ʱ��
		{
			KillTimer(IDT_FOLO_TIMER);
			TBet tagBet;
			tagBet.nType = ET_FOLD;
			SendGameData(&tagBet, sizeof(tagBet), MDM_GM_GAME_NOTIFY, ASS_BET, 0);
			break;
		}
	case IDT_ALLIN_TIMER:			// ȫ�¶�ʱ��
		{
			KillTimer(IDT_ALLIN_TIMER);
			TBet tagBet;
			tagBet.nType = ET_ALLIN;
			tagBet.nMoney = m_iMoneys[m_byMeStation];
			SendGameData(&tagBet, sizeof(tagBet), MDM_GM_GAME_NOTIFY, ASS_BET, 0);	
			break;
		}
	default:{break;}
	}
	return true;
}

//�����˿�
LRESULT CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	
	return 0L;
}

//�����˿�
LRESULT CClientGameDlg::OnMouseMove(WPARAM wparam, LPARAM lparam)
{
	
	return TRUE;;
}
//�����˿�
LRESULT CClientGameDlg::OnMouseLeft(WPARAM wparam, LPARAM lparam)
{
	return TRUE;	
}

//�û�������
LRESULT CClientGameDlg::OnHitActiveResult(WPARAM wparam, LPARAM lparam)
{
	return 0L;
}

//�й�
LRESULT CClientGameDlg::OnHitAuto(WPARAM wparam,LPARAM lparam)
{
	return 0;
}
//�������°�ť����
LRESULT	CClientGameDlg::OnHitHaveThing(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

//��ǰ����
LRESULT	CClientGameDlg::OnStop(WPARAM wparam, LPARAM lparam)
{
	
	return 0;
}

//������ǰ����
LRESULT	CClientGameDlg::OnStopThing(WPARAM wparam, LPARAM lparam)
{
	
	return 0;
}

//ͬ����ǰ����
LRESULT	CClientGameDlg::OnAgreeStop(WPARAM wparam, LPARAM lparam)
{
	return 0;
}


//�����뿪������
LRESULT	CClientGameDlg::OnSendHaveThing(WPARAM wparam, LPARAM lparam)
{
	/*	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
	m_PlayView.m_btThing.EnableWindow(FALSE);
	HaveThingStruct HaveThing;
	HaveThing.pos=0;
	::strcpy(HaveThing.szMessage,(char *)wparam);
	SendGameData(&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}*/
	return 0;
}

//�û������뿪
LRESULT	CClientGameDlg::OnArgeeUserLeft(WPARAM wparam, LPARAM lparam)
{
	/*	LeaveResultStruct Leave;
	Leave.bDeskStation=GetMeUserInfo()->bDeskStation;
	Leave.bArgeeLeave=(BYTE)wparam;
	SendGameData(&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	*/	return 0;
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

	return;
}

void CClientGameDlg::OnGameSetting()
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	//CGameSet GameSetDlg(this);
	//GameSetDlg.m_bSound=m_pGameInfo->bEnableSound;
	//GameSetDlg.m_bShowUser=m_pGameInfo->bShowUserInfo;	
	//GameSetDlg.m_bEnableWatch=m_pGameInfo->bEnableWatch;
	//GameSetDlg.m_bRightPower=GetProfileInt(TEXT("RightPower"),FALSE);

	//if (GameSetDlg.DoModal()==IDOK)
	//{
	//	//�������
	//	bool bSendWatch=((m_bWatchMode==false)&&(m_pGameInfo->bEnableWatch!=GameSetDlg.m_bEnableWatch));

	//	//��������
	//	/*m_pGameInfo->bShowUserInfo=GameSetDlg.m_bShowUser;
	//	m_pGameInfo->bEnableWatch=GameSetDlg.m_bEnableWatch;
	//	m_pGameInfo->bEnableSound=GameSetDlg.m_bSound;
	//	WriteProfileInt(TEXT("RightPower"),GameSetDlg.m_bRightPower?1:0);*/
	//	//m_PlayView.m_bRightPower=GameSetDlg.m_bRightPower;

	//	//��������
	//	if (bSendWatch==true)
	//	{
	//		MSG_GM_WatchSet WatchSet;			
	//		memset(&WatchSet,0,sizeof(WatchSet));
	//		WatchSet.dwUserID=0;
	//		SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,GameSetDlg.m_bEnableWatch?TRUE:FALSE);
	//	}
	//}
	AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}

void CClientGameDlg::OnWatchSetChange(void)
{
	/*if (m_bWatchMode==true)
	{
		if (m_bWatchOther)	m_MessageHandle.InsertNormalMessage(TEXT("����������Թ�����Ϸ"));
		else m_MessageHandle.InsertNormalMessage(TEXT("��Ҳ��������Թ�����Ϸ"));
	}*/
}

//�û��뿪
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	/*	if ((bWatchUser==false)&&(pUserItem!=NULL)&&(bDeskStation==0))
	{
	if (GetStationParameter()==GS_WAIT_ARGEE)
	{
	for (int i=0;i<4;i++)
	{
	if (m_pUserInfo[i]!=NULL) 
	{
	m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
	m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
	}
	}
	if (m_PlayView.m_GameInfo.GetSafeHwnd()) m_PlayView.m_GameInfo.DestroyWindow();
	m_PlayView.m_GameNoteView.SetMessage(TEXT("���ڵȴ����������ƾ�"));
	m_PlayView.UpdateViewFace(NULL);
	}
	}*/
	//	if ((bWatchUser==false)&&(pUserItem!=NULL)&&(bDeskStation==0))		//����Ƕ����뿪
	//	{
	if (GetStationParameter()>GS_WAIT_ARGEE)
	{
		for (int i=0;i<PLAY_COUNT;i++)
		{
			if (m_pUserInfo[i]!=NULL) 
			{
				m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
//				m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
			}
		}
	//	m_PlayView.UpdateViewFace(NULL);
	}
//	m_PlayView.SetNTStation();
	//	}
	//��ǰ����뿪��������Ƶ����Ҳ��֮�뿪����
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&pUserItem!=NULL&&!bWatchUser==TRUE)
	{
		//	MessageBox("��ۿ������,�Ѿ��˳�.","ϵͳ��ʾ",MB_ICONQUESTION);
		OnCancel();
		//PostMessage(WM_CLOSE,0,0);
	}
	return true;
//	return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
}

void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDT_IN_MONEY_TIMER:
		{
			KillTimer(IDT_IN_MONEY_TIMER);
			// ȷ�ϴ���	
			TSubMoney cmd;
			cmd.nMoney[m_byMeStation] = m_iCurrentMoney;
			cmd.bDeskStation = m_byMeStation;
			SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);
			break;
		}
	}
	CLoveSendClass::OnTimer(nIDEvent);
}


void CClientGameDlg::OnCancel()
{
	/*TCHAR sz[200];
	if(GetComType() == TY_MONEY_GAME)
		wsprintf(sz,"�����˳������ %d ��(��ҳ������ %d ���).�����Ҫ�˳���?",m_iRunPublish,m_iRunPublish * m_iRoomBasePoint);
	else
		wsprintf(sz,"�����˳������ %d ��.�����Ҫ�˳���?",m_iRunPublish);*/
	//��ѯ״̬
	if (/*!m_bWatchMode &&*/ GetStationParameter()>=GS_SEND_CARD&&GetStationParameter() < GS_WAIT_NEXT)
	{
//		if (IDYES!=AFCMessage(sz,m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON2|MB_ICONINFORMATION,this)) 
			return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}

	srand((unsigned) time (NULL) + m_byMeStation); 
	int iRandTime = rand() % 10; 

	SetGameTimer(m_byMeStation, iRandTime, IDT_CANCEL_TIME);
}

//�޸��û�Ѻע���
BOOL CClientGameDlg::ModifyNote(BYTE iViewStation,int iCurNote,BOOL bExtVal)
{
	//׷�ӱ���ĳ��Ѻע���
//	m_PlayView.AddNotePoint(iViewStation,iCurNote);
	//��ע�ۼ�
//	m_PlayView.AddTotalNote(0,iCurNote);
	return true;
}

////�û�Ѻע����
//BOOL CClientGameDlg::SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy)
//{
//	TCHAR str[MAX_PATH+1]="";
//	TCHAR filename[MAX_PATH];//path[MAX_PATH];
//	CString s=CINIFile::GetAppPath ();/////����·��
//	CINIFile f( s + "cardsound.ini");
//	CString key = TEXT("wznnnote");
//	//	TCHAR path[MAX_PATH];
//	CString folder,extendname,name;
//	folder=f.GetKeyVal(key,"folder","music\\cardwav\\wznn\\");
//	extendname=f.GetKeyVal(key,"extendname",".wav");
//	TCHAR Val[50];
//	srand(GetCurrentTime());
//	int count =f.GetKeyVal(key,"count",1);
//	switch(bNoteStyle)
//	{
//		/*case TYPE_OPENCARD:
//		{	
//		wsprintf(Val,"%c(open%d)",bIsBoy?'b':'g',rand()%count);
//		name = f.GetKeyVal(key,Val,Val);
//		break;
//		}
//		case TYPE_FOLLOW:
//		{
//		wsprintf(Val,"%c(follow%d)",bIsBoy?'b':'g',rand()%count);
//		name = f.GetKeyVal(key,Val,Val);
//		break;
//		}
//		case TYPE_BULL:
//		{
//		wsprintf(Val,"%c(add%d)",bIsBoy?'b':'g',rand()%count);
//		name = f.GetKeyVal(key,Val,Val);
//		break;
//		}*/
//	case TYPE_NOTE:
//		{
//			wsprintf(Val,"%c(note%d)",bIsBoy?'b':'g',rand()%count);
//			name = f.GetKeyVal(key,Val,Val);
//			break;
//		}
//	case TYPE_GIVE_UP:
//		{
//			wsprintf(Val,"%c(pass%d)",bIsBoy?'b':'g',rand()%count);
//			name = f.GetKeyVal(key,Val,Val);
//			break;
//		}	
//	default:
//		{
//			wsprintf(str,"%c(def)",bIsBoy?'b':'g');
//			name = f.GetKeyVal(key,Val,Val);
//			break;
//		}
//
//	}
//	wsprintf(filename,"%s%s%s",folder,name,extendname);
//	PlayOutCardType(filename,m_hMciWnd1);
//	return true;
//}
//
//BOOL CClientGameDlg::SetPlaySound(BYTE bSoundStyle,BOOL bExtVal)
//{
//	CString s=CINIFile::GetAppPath ();/////����·��
//	CINIFile f( s + "cardsound.ini");
//	CString key = TEXT("system");
//	CString folder,filename;
//	folder=f.GetKeyVal(key,"folder","music\\cardwav\\");
//	TCHAR fi[MAX_PATH+1];
//	switch(bSoundStyle)
//	{
//	case SOUND_WARNING:
//		{
//			filename = f.GetKeyVal(key,"warning","warning");
//			break;
//		}
//	case SOUND_INVALIDATE:
//		{
//			filename = f.GetKeyVal(key,"invalidate","invalidate");
//			break;
//		}
//	case SOUND_SEND_CARD:
//		{
//			filename = f.GetKeyVal(key,"sendcard","sendcard");
//			break;
//		}
//	}
//	wsprintf(fi,"%s%s%s",folder,filename,".wav");
//	::PlaySound(fi,NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP|SND_NOWAIT|SND_NODEFAULT);
//	return true;
//}
//
////���ó�����������
//BOOL CClientGameDlg::SetOutCardTypeSound(BYTE iCardList[],int iCardCount,BOOL IsBoy)
//{
//	int cardshape = m_Logic.GetCardShape(iCardList,iCardCount);
//	//	int cardstart,cardend;
//	//	TCHAR filename[MAX_PATH],
//	TCHAR str[100]="";
//	TCHAR filename[MAX_PATH];//path[MAX_PATH];
//	CString s=CINIFile::GetAppPath ();/////����·��
//	CINIFile f( s + "cardsound.ini");
//	CString key = TEXT("type0");
//	//	TCHAR path[MAX_PATH];
//	CString folder,extendname,name;
//	folder=f.GetKeyVal(key,"folder","music\\cardwav\\type0\\");
//	extendname=f.GetKeyVal(key,"extendname",".mp3");
//
//
//	if(IsBoy)
//	{
//		wsprintf(str,"b(def)");
//	}
//	else
//		wsprintf(str,"g(def)");
//
//	name=f.GetKeyVal(key,str,str);
//	wsprintf(filename,"%s%s%s",folder,name,extendname);
//
//	PlayOutCardType(filename,m_hMciWnd1);
//
//	//::PlaySound(filename,NULL,SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT);
//
//	return TRUE;
//}
////���ų�������
//BOOL CClientGameDlg::SetSenceSound(BYTE iSendSound,BOOL IsBoy,int iVal)
//{
//	TCHAR str[100]="";
//	TCHAR filename[MAX_PATH];//path[MAX_PATH];
//	CString s=CINIFile::GetAppPath ();/////����·��
//	CINIFile f( s + "cardsound.ini");
//	CString key = TEXT("cardsence");
//	//	TCHAR path[MAX_PATH];
//	CString folder,extendname,name;
//	folder=f.GetKeyVal(key,"folder","music\\cardwav\\sence\\");
//	extendname=f.GetKeyVal(key,"extendname",".mp3");
//
//	//	TCHAR filename[MAX_PATH],str[100]="";
//	srand(GetCurrentTime());
//	int count;
//	switch(iSendSound)
//	{
//	case SOUND_WIN://ʤ��
//		if(IsBoy)
//		{	
//			count = f.GetKeyVal(key,"bwincount",10);
//			wsprintf(str,"b(win%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"gwincount",10);
//			wsprintf(str,"g(win%d)",rand()%count);
//		}
//		break;
//	case SOUND_LOSE://ʧ��
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"blosecount",10);
//			wsprintf(str,"b(lose%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"glosecount",10);
//			wsprintf(str,"g(lose%d)",rand()%count);
//		}
//		break;
//	case SOUND_WAITING://�ȴ�
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"bwaitcount",10);
//			wsprintf(str,"b(wait%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"gwaitcount",10);
//			wsprintf(str,"g(wait%d)",rand()%count);
//		}
//		break;
//	case SOUND_ERROR://���Ƴ�����
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"berrorcount",10);
//			wsprintf(str,"b(error%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"gerrorcount",10);
//			wsprintf(str,"g(error%d)",rand()%count);
//		}
//		break;
//	case SOUND_READY://׼��
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"breadycount",10);
//			wsprintf(str,"b(ready%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"greadycount",10);
//			wsprintf(str,"g(ready%d)",rand()%count);
//		}
//		break;
//	case SOUND_START://��ʼ��Ϸ(���)
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"bstartcount",10);
//			wsprintf(str,"b(start%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"gstartcount",10);
//			wsprintf(str,"g(start%d)",rand()%count);
//		}
//		break;
//	case SOUND_JIAO_FEN://�з�
//		{
//			if(IsBoy)
//				wsprintf(str,"b(jiaofen%d)",iVal);
//			else
//				wsprintf(str,"g(jiaofen%d)",iVal);
//		}
//	case SOUND_OUT_CARD_FINISH://������
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"boutcardfinishcount",10);
//				wsprintf(str,"b(outcardfinish%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"goutcardfinishcount",10);
//				wsprintf(str,"g(outcardfinish%d)",rand()%count);
//			}
//			break;
//		}
//	case SOUND_FRIEND_APPEAR://���ѳ��F
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"bfriendcount",10);
//				wsprintf(str,"b(friend%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"gfriendcount",10);
//				wsprintf(str,"g(friend%d)",rand()%count);
//			}
//			break;
//		}
//	case SOUND_USER_RUNAWARY:	//����
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"brunawarycount",10);
//				wsprintf(str,"b(runawary%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"grunawarycount",10);
//				wsprintf(str,"g(runawary%d)",rand()%count);
//			}
//			break;
//		}
//	case SOUND_USER_LEFT:	//�����뿪
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"bleftcount",10);
//				wsprintf(str,"b(left%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"gleftcount",10);
//				wsprintf(str,"g(left%d)",rand()%count);
//			}
//			break;
//		}
//	case SOUND_GIVE_BACK://���
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"bgivebackcount",10);
//				wsprintf(str,"b(giveback%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"ggivebackcount",10);
//				wsprintf(str,"g(giveback%d)",rand()%count);
//			}
//			break;
//		}
//	default:
//		if(IsBoy)
//			wsprintf(str,"b(sence_0)");
//		else
//			wsprintf(str,"g(sence_0)");
//		break;
//	}
//	name=f.GetKeyVal(key,str,str);
//	wsprintf(filename,"%s%s%s",folder,name,extendname);
//	PlayOutCardType(filename,m_hMciWnd2);
//	return TRUE;
//}
////����Mp3����
//BOOL CClientGameDlg::PlayOutCardType(TCHAR FileName[],HWND hmciwnd)
//{
//	if(hmciwnd)
//	{
//		MCIWndStop(hmciwnd);
//		MCIWndDestroy(hmciwnd);
//		hmciwnd=NULL;
//	}
//
//	//hmciwnd = MCIWndCreate(this->m_hWnd,AfxGetInstanceHandle(),WS_CHILD,FileName);
//
//	hmciwnd=MCIWndCreate(this->m_hWnd,AfxGetInstanceHandle(),WS_CHILD|MCIWNDF_NOMENU|MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR|MCIWNDF_NOERRORDLG,FileName);
//	if(hmciwnd)
//		MCIWndPlay(hmciwnd);	
//	return true;
//}
////��������
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{	
	TCHAR GameName[200];
	CString strBuffer;
	wsprintf(GameName,"��� %s ��Ϸ�汾����,��Ҫ�������������°汾��?",m_pGameInfo->szGameName);
	if(iVersion!=m_iVersion || iVersion2!=m_iVersion2)
	{
		TCHAR sz[200];
		wsprintf(sz,"�汾��ͻ:��ǰ�汾%d-%d,����˰汾%d-%d",m_iVersion,m_iVersion2,iVersion,iVersion2);
	}
	if(iVersion!=m_iVersion)
	{

//		if (IDYES==AFCMessage(GameName,m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
//			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	if(iVersion2!=m_iVersion2)
	{
///		if (IDYES==AFCMessage(GameName,m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
//			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	return TRUE;
}
//�û���ע
LRESULT CClientGameDlg::OnShowNote(WPARAM wparam,LPARAM lparam)
{
//	m_PlayView.m_NtList.ShowWindow(TRUE);
//	m_PlayView.m_NtList.SetNumList(m_iThisTurnLimit, m_iBaseNote);				//������ע��Ŀ
	return TRUE;
}
//�û�ѡ����ע��
LRESULT CClientGameDlg::OnSelectNote(__int64 wparam,int lparam)
{
	return TRUE;
}


//���ƶ�����ʾ��Ϻ�,��ʾ������
LRESULT CClientGameDlg::SendCardShowCard(WPARAM wparam,LPARAM lparam)
{
	//BYTE iViewStation=(BYTE)wparam;
	//m_PlayView.m_UserCard[iViewStation].SetCard(m_DeskCard[iViewStation],NULL,m_iDeskCardCount[iViewStation]);
	////	m_PlayView.m_UserCard[m_PlayView.m_flag].SetCard(m_DeskCard[m_PlayView.m_flag],NULL,m_iDeskCardCount[m_PlayView.m_flag]);

	////ϵͳ����Ա��ʾ���ҵ���
	//if(m_pGameInfo->pMeUserInfo->GameUserInfo.bGameMaster==MAST_PEOPLE)
	//	m_PlayView.m_UserCard[iViewStation].SetCard(m_UserCard[iViewStation],NULL,m_iDeskCardCount[iViewStation]);
	return 0;
}

//�Ñ��Q����
LRESULT CClientGameDlg::ChangeBackCard()
{
	return 0;
}

//���Ż��㵽��ͼλ��
//BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
//{
//	BYTE bViewStation=m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
//	int temp=((PLAY_COUNT/2)-bViewStation+bDeskStation);
//	if(temp>=0)
//		return temp%PLAY_COUNT;
//	else
//		return PLAY_COUNT-1;
//switch(bViewStation)
//	{
//	case 0: { 
//		if(bDeskStation==0)	
//			return 3;
//		if(bDeskStation==1)
//			return 4;
//		if(bDeskStation==2)
//			return 5;
//		if(bDeskStation==3)
//			return 0;
//		if(bDeskStation==4)
//			return 1;
//		if(bDeskStation==5)
//			return 2;
//		break;
//			}
//	case 1: {
//		if(bDeskStation==0)	
//			return 2;
//		if(bDeskStation==1)
//			return 3;
//		if(bDeskStation==2)
//			return 4;
//		if(bDeskStation==3)
//			return 5;
//		if(bDeskStation==4)
//			return 0;
//        if(bDeskStation==5)
//			return 1;
//		break;
//			}
//	case 2: { 
//		if(bDeskStation==0)	
//			return 1;
//		if(bDeskStation==1)
//			return 2;
//		if(bDeskStation==2)
//			return 3;
//		if(bDeskStation==3)
//			return 4;
//		if(bDeskStation==4)
//			return 5;
//		if(bDeskStation==5)
//			return 0;
//		break;
//			}
//	case 3: { 
//		if(bDeskStation==0)	
//			return 0;
//		if(bDeskStation==1)
//			return 1;
//		if(bDeskStation==2)
//			return 2;
//		if(bDeskStation==3)
//			return 3;
//		if(bDeskStation==4)
//			return 4;
//		if(bDeskStation==5)
//			return 5;
//		break;
//			}	
//	case 4: { 
//		if(bDeskStation==0)	
//			return 5;
//		if(bDeskStation==1)
//			return 0;
//		if(bDeskStation==2)
//			return 1;
//		if(bDeskStation==3)
//			return 2;
//		if(bDeskStation==4)
//			return 3;
//		if(bDeskStation==5)
//			return 4;
//		break;
//			}
//	case 5:
//		{
//		if(bDeskStation==0)	
//			return 4;
//		if(bDeskStation==1)
//			return 5;
//		if(bDeskStation==2)
//			return 0;
//		if(bDeskStation==3)
//			return 1;
//		if(bDeskStation==4)
//			return 2;
//		if(bDeskStation==5)
//			return 3;
//		break;
//		}
//	default:
//		break;
//    }
//	return 0;
//}
//�Q���ƽY��
BOOL CClientGameDlg::ChangeBackCardResult(BYTE bDeskStation,BYTE bSourceBackCard,BYTE bResultCard,BOOL bExtVal)
{
	if(!bExtVal)
		return true;
	m_UserCard[bDeskStation][0]=bResultCard;
	/*m_PlayView.m_UserCard[bDeskStation].SetCard(m_UserCard[bDeskStation],NULL,m_iDeskCardCount[bDeskStation]);
	ReplaceCard(m_SysCard,0,m_iSysCardCount,bSourceBackCard,bResultCard);

	m_PlayView.m_SysBackCard.SetCard(m_SysCard,NULL,m_iSysCardCount);*/
	return true;
}
//ϵ�y�������Ƿ���ĳ����
BOOL CClientGameDlg::ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard)
{
	//�Q����
	for(int i=iSendCardPos;i<iEndPos;i++)
	{
		if(iTotalCard[i]==bResultCard)
		{
			iTotalCard[i]=bSourceCard;
			return true;
		}
	}
	return false;
}

//������������ע
BOOL CClientGameDlg::UserSupersedeNote(BYTE bDeskStation)
{
	KillAllTimer();
	return TRUE;
}
int CClientGameDlg::GetFrontPlay(int NowPaly)
{
	int frontplay;//��һλ��ע��
	for(int i=1;i<PLAY_COUNT;i++)
	{
		frontplay=(GetMeUserInfo()->bDeskStation-i<0)?PLAY_COUNT+(GetMeUserInfo()->bDeskStation-i)
			:GetMeUserInfo()->bDeskStation-i;

		if(!m_pUserInfo[frontplay])		
			continue;

		if(m_UserCard[frontplay]!=0)//��ǰ�������δPASS

			return frontplay;
	}
	return 0;
}
void CClientGameDlg::DelShowThis()
{
	/*for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_PlayView.m_UserCard[i].SetCardShowNum(0);
	}*/
}
//Ϊ��Ƶģ�����ش˺���
#ifdef VIDEO
bool CClientGameDlg::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return FALSE;
	}

	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && bWatchUser )
	{
		m_PlayView.m_bWatch=bWatchUser;
	}
	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && (!bWatchUser) )
	{    
		CPlayVideo *pPlayVideo=CPlayVideo::CreatePlayVideo();


		if(NULL!=pPlayVideo)
		{ 

			CString NickName=TEXT("");
			int    byteDeskNO=0;
			UserInfoStruct &UInfo=	pUserItem->GameUserInfo;
			NickName+=UInfo.nickName;
			byteDeskNO=UInfo.bDeskNO;
			UINT	uiRoomID = m_pGameInfo->uRoomID;
			UINT	uiBRRoomID =   uiRoomID * 1000 + byteDeskNO; 

			ST_INITVIDEO stInitVideo;
			stInitVideo.iRoomPassType = 0;
			stInitVideo.nRoomID		  = uiBRRoomID;
			stInitVideo.pParentWnd    = &m_PlayView;
			stInitVideo.strRoomPass   = _T("");
			stInitVideo.strUserName	  = NickName;
			stInitVideo.strUserPass   = _T("");
			if( pPlayVideo->Video_Initialize(stInitVideo) )
			{
				//			
				//sprintf(sz,"��Ƶ��ʼ���ɹ�!");
				//Writelog(sz,1);
				////������Ƶ������

				pPlayVideo->Connect();

			}
			else
			{
				pPlayVideo->Video_Release();
				/*	sprintf(sz,"��Ƶ��ʼ��ʧ��!");
				Writelog(sz,1);*/
				return 0;
			}


		} 
	}
	return TRUE;
}
bool CClientGameDlg::AFCCloseFrame()
{
	///�ͷ���Ƶ��Դ 
	if( NULL != CPlayVideo::GetPlayVideo())
		CPlayVideo::GetPlayVideo()->Video_Release(); 
	return __super::AFCCloseFrame();
}
#endif
//���ؽ�ׯ��ť
BOOL CClientGameDlg::HideButton()
{
	/*m_PlayView.m_btCallScore1.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_HIDE);*/
	return true;
}
//�з�
void CClientGameDlg::ShowCallScoreBt(int CallScore,BOOL bExtVol)
{
	/*m_PlayView.m_btCallScore1.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScorePass.EnableWindow(1);
	m_PlayView.m_btCallScore1.EnableWindow(TRUE);*/
	return;
}
//�з�
LRESULT	CClientGameDlg::OnCallScore(WPARAM wparam, LPARAM lparam)
{
	return 0;
}
BOOL CClientGameDlg::GetBull(BYTE iCardList[],int iCardCount,BYTE iBullCard[])
{
	return false;
}

// ˳ʱ��˳��õ��¼�λ��
BYTE CClientGameDlg::GetNextUserStation(BYTE byStation, BOOL bIsAll)
{
	int nCount = 0;
	int iNextStation = (byStation + PLAY_COUNT + 1) % PLAY_COUNT;

	// �ƹ���Ч������ѷ��������
	while (m_pUserInfo[iNextStation] == NULL 
	|| m_bGiveUp[iNextStation] 
	|| (!bIsAll && m_bAllBet[iNextStation]))
	{
		iNextStation = (iNextStation + PLAY_COUNT + 1) % PLAY_COUNT;
		nCount++;

		if (nCount > PLAY_COUNT)
		{
			iNextStation = -1;
			break;
		}
	}
	return iNextStation;
}

//��ʱ��˳��õ��ϼ�λ��
BYTE CClientGameDlg::GetLastUserStation(BYTE byStation, BOOL bIsAll)
{
	int nCount = 0;
	int iLastStation = (byStation + PLAY_COUNT - 1) % PLAY_COUNT;

	// �ƹ���Ч��� �� �ѷ�������� �� ��;������Ϸ���
	while (m_pUserInfo[iLastStation] == NULL 
	|| m_bGiveUp[iLastStation] 
	|| (!bIsAll && m_bAllBet[iLastStation]))
	{
		iLastStation = (iLastStation + PLAY_COUNT - 1) % PLAY_COUNT;

		nCount++;

		if (nCount > PLAY_COUNT)
		{
			iLastStation = -1;
			break;
		}
	}

	return iLastStation;
}

// ��ȡBet�������
int CClientGameDlg::GetRandBetByWeight(int iWeight)
{
	srand(GetTickCount()); 
	int iRandNum = rand() % 10 + 1;
	switch(iWeight)
	{
	case 0:
	case 1:
	case 2:
		{
			if (iRandNum > 3)
			{
				return UD_VF_BET;
			}
			else
			{
				return UD_VF_FOLD;
			}
		}
	case 3:
	case 4:
		{
			if (iRandNum > 6)
			{
				return UD_VF_BET;
			}
			else
			{
				return UD_VF_FOLD; 
			}
		}
	case 5:
	case 6:
		{
			if (iRandNum > 7)
			{
				return UD_VF_BET;
			}
			else if (iRandNum > 4)
			{
				return UD_VF_CHECK;
			}
			else
			{
				return UD_VF_FOLD;
			}
		}
	case 7:
		{
			if (iRandNum > 8)
			{
				return UD_VF_BET;
			}
			else
			{
				return UD_VF_CHECK;
			}
		}
	case 8:
		{
			if (iRandNum > 8)
			{
				return UD_VF_BET;
			}
			else if (iRandNum > 5)
			{
				return UD_VF_CHECK;
			}
		}
	case 9:
	case  10:
		{
			if (iRandNum > 10)
			{
				return UD_VF_BET;
			}
			else
			{
				return UD_VF_CHECK;
			}
		}

	default:{return UD_VF_FOLD;}
	}

	return UD_VF_FOLD;
}

// ��ȡAdd�������
int CClientGameDlg::GetRandAddByWeight(int iWeight)
{
	srand(GetTickCount()); 
	int iRandNum = rand() % 10 + 1;
	switch(iWeight)
	{
	case 0:
	case 1:
	case 2:
		{
			if (iRandNum > 4)
			{
				return UD_VF_CALL;
			}
			else
			{
				return UD_VF_FOLD ; 
			}
		}
	case 3:
	case 4:
		{
			if (iRandNum > 6)
			{
				return UD_VF_ADD;
			}
			else if (iRandNum > 4)
			{
				return UD_VF_CALL;
			}
			else
			{
				return UD_VF_FOLD; 
			}
		}
	case 5:
	case 6:
		{
			if (iRandNum > 6)
			{
				return UD_VF_ADD;
			}
			else if (iRandNum > 4)
			{
				return UD_VF_CALL;
			}
			else
			{
				return UD_VF_FOLD;
			}
		}
	case 7:
		{
			if (iRandNum > 7)
			{
				return UD_VF_ADD;
			}
			else
			{
				return UD_VF_CALL;
			}
		}
	case 8:
		{
			if (iRandNum > 8)
			{
				return UD_VF_ADD;
			}
			else if (iRandNum > 4)
			{
				return UD_VF_CALL;
			}
		}
	case 9:
	case  10:
		{
			if (iRandNum > 10)
			{
				return UD_VF_ADD;
			}
			else
			{
				return UD_VF_CALL;
			}
		}

	default:{return UD_VF_FOLD;}
	}

	return UD_VF_FOLD;
}

// ��ȡAllin�������
int CClientGameDlg::GetRandAllinByWeight(int iWeight)
{
	srand(GetTickCount()); 
	int iRandNum = rand() % 10 + 1;
	switch(iWeight)
	{
	case 0:
	case 1:
	case 2:
		{
			return UD_VF_FOLD;				 
		}
	case 3:
	case 4:
		{
			if (iRandNum > 3)
			{
				return UD_VF_ALLIN;
			}
			else
			{
				return UD_VF_FOLD; 
			}
		}
	case 5:
	case 6:
		{
			if (iRandNum > 6)
			{
				return UD_VF_ALLIN;
			}
			else
			{
				return UD_VF_FOLD;
			}
		}
	case 7:
		{
			if (iRandNum > 7)
			{
				return UD_VF_ALLIN;
			}
		}
	case 8:
		{
			if (iRandNum > 8)
			{
				return UD_VF_ALLIN;
			}
		}
	case 9:
	case  10:
		{
			if (iRandNum > 10)
			{
				return UD_VF_ALLIN;
			}
		}

	default:{return UD_VF_FOLD;}
	}

	return UD_VF_FOLD;
}