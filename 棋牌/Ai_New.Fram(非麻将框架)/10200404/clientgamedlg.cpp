#include "StdAfx.h"
#include "ClientGamedlg.h"

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	
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
	OutputDebugString("liuwen::������Ϸ״̬");
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			SetGameTimer(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,GenerateRand(3),TIMER_GETREADY);
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
			#pragma region ��Ϸ״̬
			{
				TGameStation *ptGameStation = (TGameStation*)pNetData;
				if(ptGameStation == nullptr)
				{
					return -1;
				}
				if(uDataSize != sizeof(TGameStation))
				{
					return -1;
				}
				GameStationProc(*ptGameStation);    
				return true;       
			}
			#pragma endregion ��Ϸ״̬
		case ASS_GETREADY:		//׼��
			#pragma region ASS_GETREADY
			{
				TGetReady *ptGetReady = (TGetReady*)pNetData;
				if(ptGetReady == nullptr)
				{
					return -1;
				}
				else
				{
				}
			}
			break;
			#pragma endregion ASS_GETREADY
		case ASS_GAMELEVEL:	//��Ϸ������Ϣ
			#pragma region ASS_GAMELEVEL
			{
				if (sizeof(TGameLevel) != uDataSize)
				{
					return -1;
				}
				TGameLevel *ptGameLevel = (TGameLevel*)pNetData;
				if(ptGameLevel == NULL)
				{
					OutputDebugString("chs ASS_GAMELEVEL, ptGameLevel null pointer.");
					return -1;
				}
				ShowCurrentGameLevel(ptGameLevel);
			}
			break;
			#pragma endregion ASS_GAMELEVEL
		case ASS_SEND_CARD:	//����
			#pragma region ASS_SEND_CARD
			{
				OutputDebugString("chs ������Ϣ0");
				TSendCards *ptSendCards = (TSendCards*)pNetData;
				if(ptSendCards == nullptr)
				{
					return -1;
				}
				OutputDebugString("chs ������Ϣ1");
				ReceiveAndShowCards(ptSendCards);
			}
			break;
			#pragma endregion ASS_SEND_CARD
		case ASS_SHOWCALLLEVELCARD:
			#pragma region ASS_SHOWCALLLEVELCARD
			{
				TCallLevelCard *ptCallLevelCard = (TCallLevelCard*)pNetData;
				if(ptCallLevelCard == nullptr)
				{
					return -1;
				}
			}
			break;
			#pragma endregion ASS_SHOWCALLLEVELCARD
		case ASS_WHOISBANKER:	//֪ͨ˭��ׯ�ң��ͼ���
			#pragma region ASS_WHOISBANKER
			{
				//��ׯ��ͼ���ԣ�����ׯ��ͼƬ
				TCallLevelCard *ptCallLevelCard = (TCallLevelCard*)pNetData;
				if (ptCallLevelCard == nullptr)
				{
					return -1;
				}
			
				//���漶�ƵĻ�ɫ
				m_byGameLevelColor = ptCallLevelCard->byLevelColor;
				//���浱ǰ�ļ���
				if(m_byGameLevelColor != 255)
				{
					memcpy(m_byGameLevel,ptCallLevelCard->byCallCard,sizeof(BYTE)*2);
				}
				//��ʾׯ�ҵ�����
				//��ʾ��ǰ�����Ļ�ɫ
				//�����е�������
				//m_cLogic.SortCard(m_byCards,33,m_byGameLevelColor,m_byGameLevel[(m_pGameFrame->GetMyDeskStation())%2]);
				//m_pIOperateCard->SetCardList(m_byCards,33);
				BYTE byLevelcard = ptCallLevelCard->byCallCard[0];
				BYTE byColor = (byLevelcard & 0xF0);
			}
			#pragma endregion ASS_WHOISBANKER
		case ASS_SENDBACKCARD:	//�յ���
			#pragma region ASS_SENDBACKCARD
			{
				TSendCards *ptSendCard = (TSendCards*)pNetData;
				if(ptSendCard == nullptr)
				{
					return -1;
				}
			
				if((m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation) == ptSendCard->byDeskStation)
				{
					//��ʾ����
					BYTE byCards[8] = {0};
					memcpy(byCards,ptSendCard->byCards,sizeof(BYTE)*(ptSendCard->byCardCount));
					//�����Ʋ����������е�����
					for(int i = 0;i<8;++i)
					{
						m_byCards[25+i] = ptSendCard->byCards[i];
						m_vbyCards.push_back(ptSendCard->byCards[i]);
					}
					//�������ƶ�ʱ��,��ʱ�䵽û�����ƣ����������г��8���ƣ�����
					SetGameTimer(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,GenerateRand(10),TIMER_BURYBACKCARD);
				}
			}
			break;
			#pragma endregion ASS_SENDBACKCARD
		case ASS_BACKCARDBURIED:	//����������
			#pragma region ASS_BACKCARDBURIED
			{
				//���������У�ɾ������
				for(int j=0; j<8;j++)
				{
					for(int i=0;i<33; i++)
					{
						if (m_byBackCard[j] == m_byCards[i])
						{
							m_byCards[i] = 255;
							break;
						}
					}
				}
				//�������¸�ֵ�� m_byCards
				int iIndex = 0;
				for(int i=0;i<33; i++)
				{
					if (m_byCards[i] != 255)
					{
						m_byCards[iIndex++] = m_byCards[i];
					}
				}
				//���m_vbyCards�� ����������ѹ�� m_vbyCards
				m_vbyCards.clear();
				for(int i=0; i<iIndex; i++)
				{
					if (m_byCards[i] != 255)
					{
						m_vbyCards.push_back(m_byCards[i]);
					}
				}
			}
			break;
			#pragma endregion ASS_BACKCARDBURIED
		case ASS_OUT_CARD:	//����
			#pragma region ASS_OUT_CARD
			{
				T_SC_OutCardPackage *pscOutCard = (T_SC_OutCardPackage *)pNetData;
				if (pscOutCard == nullptr)
				{
					return -1;
				}
				
				//�����һ����ҳ�����
				if(pscOutCard->tPreoutCards.isFirst)
				{
					memcpy(&m_tFirstPlayerCard,&pscOutCard->tPreoutCards,sizeof(T_CS_OutCard));
				}
				
				//�������
				if(pscOutCard->byOutCardPlayerDeskStation == m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation)	
				#pragma region �������
				{
					if(pscOutCard->IsFirst)
					{
						m_isFirstOutCard = true;	//һȦ�е�һ������
					}
					else	
					{
						m_isFirstOutCard = false;	//һȦ�в��ǵ�һ������
					}
					//������ʱ������
					SetGameTimer(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,GenerateRand(10),TIMER_OUTCARD);
				}
				#pragma endregion �������
				else	//�ǳ������
				#pragma region �ǳ������
				{
					//TODO:���浥Ȧ�е�һ�����Ƶ��˵�������
					if(pscOutCard->tPreoutCards.isFirst)
					{
						memcpy(&m_tFirstPlayerCard,&(pscOutCard->tPreoutCards),sizeof(T_CS_OutCard));
					}
				}
				#pragma endregion �ǳ������
			}
			break;
			#pragma endregion ASS_OUT_CARD
		case ASS_OUTMIXEDCARD_FAILED:	//˦��ʧ��
			#pragma region ˦��ʧ��
			{
				T_SC_MixedCardFailed *ptMixedCard = (T_SC_MixedCardFailed*)pNetData;
				if (ptMixedCard == NULL)
				{
					return -1;
				}
				if( uDataSize!= sizeof(T_SC_MixedCardFailed))
				{
					return -1;
				}
				//�������������С����
				T_CS_OutCard tcsOutCard;
				BYTE byDeskStation =m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
				//�����һȦ�ĵ�һ�����Ƶ��ˣ�ֱ�ӽ��Ʒ��͸����������ɷ������ж��Ƿ��ܹ����ơ� ʱ�ӺͶ�ʱ�����أ����յ����Ƴɹ����ٹرն�ʱ����ʱ��;//������ɾ���Ѿ�������
				//TODO: ������ͣ���������ͳ�����Ϣ
				m_cLogic.GetOutCardShape(ptMixedCard->byMinCard,ptMixedCard->byCardCount,tcsOutCard,byDeskStation,m_byGameLevel[0],true);
				SendGameData(&tcsOutCard,sizeof(T_CS_OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
				//�����������
				memcpy(m_tCurrentOutCard.byCards,ptMixedCard->byMinCard,sizeof(BYTE)*(ptMixedCard->byCardCount));
			}
			break;
			#pragma endregion ˦��ʧ��
		case ASS_OUTCARD_FAILED:	//����ʧ��
			#pragma region ����ʧ��
			{
				//TODO:������Ҫ�����׼ҵ��ƣ��Ա㵱����ʧ�ܣ�����ʱ���ѵ�ʱ�������Զ�ѡ��Ҫ������
				T_SC_OutCardFailed *ptOutCardFailed = (T_SC_OutCardFailed*)pNetData;
				if(ptOutCardFailed == NULL)
				{
					return -1;
				}
				if(uDataSize != sizeof(T_SC_OutCardFailed))
				{
					return -1;
				}
				
			}
			break;	
			#pragma endregion ����ʧ��
		case ASS_OUTCARD_SUCCESS:	//���Ƴɹ�
			#pragma region ���Ƴɹ�
			{
				T_CS_OutCard *ptOutCards = (T_CS_OutCard*)pNetData;
				if(ptOutCards == NULL)
				{
					return -1;
				}
				if(uDataSize != sizeof(T_CS_OutCard))
				{
					return -1;
				}
				//������
				if(ptOutCards->byDeskStation == m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation)
				{
					#pragma region ���Ƴɹ�
					//TODO:���س��ư�ť����ʾ��ť��ʱ�ӡ� ɾ��������
					//��ʾ������
					//ׯ��ɾ��������
					int iOriginCounter = m_vbyCards.size();

					m_cLogic.RemoveOutCards(m_byCards,&iOriginCounter,m_tCurrentOutCard.byCards,m_tCurrentOutCard.byCardNums);
					m_cLogic.RemoveOutCards(m_byCards,&iOriginCounter,m_tCurrentOutCard.byCards,m_tCurrentOutCard.byCardNums);

					for (int i=  0;i< m_tCurrentOutCard.byCardNums;++i)
					{
						for (vector<BYTE>::iterator it = m_vbyCards.begin();it != m_vbyCards.end();++it)
						{
							if(*it == m_tCurrentOutCard.byCards[i])
							{
								m_vbyCards.erase(it);
								break;
							}
						}
					}
					#pragma region ������ʾ���пɲ�����
// 					if(m_byGameLevelColor == 255)
// 					{
// 						m_cLogic.SortCard(m_byCards,m_vbyCards.size(),0x30,m_byGameLevel[(m_pGameFrame->GetMyDeskStation())%2]);
// 						CString cs;
// 						cs.Format("chs ���%d ����2",m_pGameFrame->GetMyDeskStation());
// 						OutputDebugString(cs);
// 					}
// 					else
// 					{
// 						m_cLogic.SortCard(m_byCards,m_vbyCards.size(),m_byGameLevelColor,m_byGameLevel[(m_pGameFrame->GetMyDeskStation())%2]);
// 						CString cs;
// 						cs.Format("chs ���%d ����2",m_pGameFrame->GetMyDeskStation());
// 						OutputDebugString(cs);
// 					}
					#pragma endregion ������ʾ���пɲ�����
				}
				#pragma endregion ���Ƴɹ�
				else	//�ǳ�����
				{
				}
			}
			break;
			#pragma endregion ���Ƴɹ�
		case ASS_SCORE:
			#pragma region �÷�
			{
				T_SC_Score *pScore = (T_SC_Score*)pNetData;
				if(pScore == NULL)
				{
					return -1;
				}
				WCHAR wbuffer[50] = {0};
				swprintf(wbuffer,L"%d",pScore->iScore);
			}
			break;
			#pragma endregion �÷�
		case ASS_GAMEOVER:
			#pragma region ��Ϸ����
			{
				ResetGameStatus();
				SetGameTimer(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,GenerateRand(10),TIMER_GETREADY);
			}
			break;
			#pragma endregion ��Ϸ����
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

	return;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case TIMER_GETREADY:
		{
			KillTimer(TIMER_GETREADY);
			ReadyOrQuit(true);
			SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
		}
		break;
	case TIMER_CALLLEVELCARD:
		{
			OutputDebugString("chs �������");
			KillTimer(TIMER_CALLLEVELCARD);
			SendGameData(NULL,0,MDM_GM_GAME_NOTIFY,ASS_CALL_COMPLETED,0);
		}
		break;
	case TIMER_BURYBACKCARD:
		#pragma region ����
		{
			KillTimer(TIMER_BURYBACKCARD);
			//���������ѡ8���ƣ�����
			TSendCards tSendCards;
			tSendCards.byCardCount = 8;
			tSendCards.byDeskStation = m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
			memcpy(tSendCards.byCards,&m_byCards[25],sizeof(BYTE)*8);
			memset(m_byBackCard,0,sizeof(m_byBackCard));
			memcpy(m_byBackCard,&m_byCards[25],sizeof(BYTE)*8);
			SendGameData(&tSendCards,sizeof(TSendCards),MDM_GM_GAME_NOTIFY,ASS_BURYCARD,0);
		}
		break;
		#pragma endregion ����
	case TIMER_OUTCARD:
		{
			KillTimer(TIMER_OUTCARD);
			OutTimeAutoOutCard();
		}
		break;
	}

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

void CClientGameDlg::GameStationProc(TGameStation& tGameStation)
{
	BYTE byGameStation = tGameStation.byGameStation;
	switch(byGameStation)
	{
	case GS_WAIT_ARGEE:
		{
			SetGameTimer(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,GenerateRand(10),TIMER_GETREADY);
		}
		break;
	default:
		{
		}
		break;
	}
}
//����һ�������
int CClientGameDlg::GenerateRand(int iBase)
{
	srand((unsigned)time(NULL));
	int iTime=(rand()%iBase)+1;

	return iTime;
}

void CClientGameDlg::ReceiveAndShowCards(TSendCards* ptSendCards)
{
	m_vbyCards.clear();
	memcpy(m_byCards,ptSendCards->byCards,(int)ptSendCards->byCardCount);

	//����������ʱ��
	OutputDebugString("chs ����������ʱ��");
	SetGameTimer(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,15,TIMER_CALLLEVELCARD);
	m_CardCounts = ptSendCards->byCardCount;	//����Ҫ��ʾ���Ƶ�����
	// ����������
	for(int i = 0;i<ptSendCards->byCardCount;++i)
	{
		m_vbyCards.push_back(ptSendCards->byCards[i]);
	}
}
//ʱ�䵽���Զ�����
void CClientGameDlg::OutTimeAutoOutCard()
{
	T_CS_OutCard tCSOutCard;
	if(m_isFirstOutCard)
	{
	#pragma region ��Ȧ�׼ҳ���
		//TODO�� ѡ��С�ĵ��Ƴ���
		tCSOutCard.byCardNums = 1;
		tCSOutCard.byDeskStation = m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
		tCSOutCard.isFirst = true;

		tCSOutCard.byCards[0] = m_vbyCards.back();
		tCSOutCard.cardShape = E_CS_SINGLE;
	#pragma endregion ��Ȧ�׼ҳ���
	}
	else
	{
	#pragma region ��Ȧ���׼ҳ���
		//����ׯ�ҳ���

		tCSOutCard.byCardNums = m_tFirstPlayerCard.byCardNums;
		tCSOutCard.byDeskStation =  m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
		tCSOutCard.isFirst = false;

		BYTE* pbyCards = new BYTE[tCSOutCard.byCardNums];
		int icardcount = 0;
		FindCardsCompatibleWithFirst(m_tFirstPlayerCard,pbyCards,&icardcount);

		CString cs;
		cs.Format("chs tCSOutCard.byCardNums:%d , icardcount:%d",tCSOutCard.byCardNums,icardcount);
		OutputDebugString(cs);
		ASSERT( tCSOutCard.byCardNums == icardcount);
		memcpy(tCSOutCard.byCards,pbyCards,sizeof(BYTE)*icardcount);
		delete[] pbyCards;

		m_cLogic.GetOutCardShape(tCSOutCard.byCards,tCSOutCard.byCardNums,tCSOutCard, m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,m_byGameLevel[0],false);

	#pragma endregion ��Ȧ���׼ҳ���
	}
	for (int i = 0;i<tCSOutCard.byCardNums;++i)
	{
		CString cs;
		cs.Format("chs �Զ����ƣ�Card%d : %d",i,tCSOutCard.byCards[i]);
		OutputDebugString(cs);
	}
	SendGameData(&tCSOutCard,sizeof(T_CS_OutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	memcpy(&m_tCurrentOutCard,&tCSOutCard,sizeof(T_CS_OutCard));
}
//��ׯ����
void CClientGameDlg::FindCardsCompatibleWithFirst(T_CS_OutCard &tFirstOutCard,BYTE *pOutCards,int* piCount)
{
	#pragma region prepare
	vector<BYTE> vJor;		//0x40
	vector<BYTE> vSpade;	//0x30
	vector<BYTE> vHeart;	//0x20
	vector<BYTE> vClub;		//0x10
	vector<BYTE> vDiamond;	//0x00
	vector<BYTE> vMainCard;
	vMainCard.clear();
	m_cLogic.SortByColor(m_byCards,m_vbyCards.size(),vJor,vSpade,vHeart,vClub,vDiamond);
	m_cLogic.GetMainCard(m_byGameLevelColor,m_byGameLevel[0],m_vbyCards,vMainCard);
	#pragma endregion prepare

	switch(tFirstOutCard.cardShape)
	{
	case E_CS_SINGLE:
		#pragma region ����
		{
			BYTE byColor = (tFirstOutCard.byCards[0] & 0xF0);
			BYTE byCard =  (tFirstOutCard.byCards[0] & 0x0F);
			if(m_cLogic.IsMainCard(m_byGameLevelColor,(m_byGameLevel[0]&0x0F),tFirstOutCard.byCards[0]))
			{
				OutputDebugString("chs [�Զ�����] �׼ҳ���������");
				#pragma region �׼ҳ�����
				if(vMainCard.size()>0)
				{
					pOutCards[0] = vMainCard[0];
					*piCount = 1;
					return ;
				}
				else if(vDiamond.size()>0)
				{
					pOutCards[0] = vDiamond[0];
					*piCount = 1;
					return ;
				}
				else if(vClub.size()>0)
				{
					pOutCards[0] = vClub[0];
					*piCount = 1;
					return ;
				}
				else if(vHeart.size()>0)
				{
					pOutCards[0] = vHeart[0];
					*piCount = 1;
					return ;
				}
				else if(vSpade.size()>0)
				{
					pOutCards[0] = vSpade[0];
					*piCount = 1;
					return ;
				}
				#pragma endregion �׼ҳ�����
			}
			else
			{
				OutputDebugString("chs [�Զ�����] �׼ҳ��Ĳ�������");
				switch(byColor)
				{
				case 0x30:	//spade
					#pragma region spade
					{
						if(vSpade.size()>0)
						{
							pOutCards[0] = vSpade[0];
							*piCount = 1;
							return ;
						}
						else if(vDiamond.size()>0)
						{
							pOutCards[0] = vDiamond[0];
							*piCount = 1;
							return ;
						}
						else if(vClub.size()>0)
						{
							pOutCards[0] = vClub[0];
							*piCount = 1;
							return ;
						}
						else if(vHeart.size()>0)
						{
							pOutCards[0] = vHeart[0];
							*piCount = 1;
							return ;
						}
						else
						{
							pOutCards[0] = vJor[0];
							*piCount = 1;
							return ;
						}
					}
					break;
					#pragma endregion spade
				case 0x20:	//heart
					#pragma region heart
					{
						if(vHeart.size()>0)
						{
							pOutCards[0] = vHeart[0];
							*piCount = 1;
							return ;
						}
						else if(vDiamond.size()>0)
						{
							pOutCards[0] = vDiamond[0];
							*piCount = 1;
							return ;
						}
						else if(vClub.size()>0)
						{
							pOutCards[0] = vClub[0];
							*piCount = 1;
							return ;
						}
						else if(vSpade.size()>0) 
						{
							pOutCards[0] = vSpade[0];
							*piCount = 1;
							return ;
						}
						else 
						{
							pOutCards[0] = vJor[0];
							*piCount = 1;
							return ;
						}
					}
					break;
					#pragma endregion heart
				case 0x10:	//club
					#pragma region club
					{
						if(vClub.size()>0)
						{
							pOutCards[0] = vClub[0];
							*piCount = 1;
							return ;
						}
						else if(vDiamond.size()>0)
						{
							pOutCards[0] = vDiamond[0];
							*piCount = 1;
							return ;
						}
						else if(vHeart.size()>0)
						{
							pOutCards[0] = vHeart[0];
							*piCount = 1;
							return ;
						}
						else if(vSpade.size()>0)
						{
							pOutCards[0] = vSpade[0];
							*piCount = 1;
							return ;
						}
						else
						{
							pOutCards[0] = vJor[0];
							*piCount = 1;
							return ;
						}
					}
					break;
					#pragma endregion club
				case 0x00:	//diamond
					#pragma region diamond
					{
						if(vDiamond.size()>0)
						{
							pOutCards[0] = vDiamond[0];
							*piCount = 1;
							return ;
						}
						else if(vClub.size()>0)
						{
							pOutCards[0] = vClub[0];
							*piCount = 1;
							return ;
						}
						else if(vHeart.size()>0)
						{
							pOutCards[0] = vHeart[0];
							*piCount = 1;
							return ;
						}
						else if(vSpade.size()>0)
						{
							pOutCards[0] = vSpade[0];
							*piCount = 1;
							return ;
						}
						else
						{
							pOutCards[0] = vJor[0];
							*piCount = 1;
							return ;
						}
					}
					break;
					#pragma endregion diamond
				}
			}		
		}
		break;
		#pragma endregion ����
	case E_CS_DOUBLE:
		#pragma region ����
		{
			BYTE byColor = (tFirstOutCard.byCards[0] & 0xF0);
			BYTE byCard = (tFirstOutCard.byCards[0] & 0xF0);

			if(m_cLogic.IsMainCard(m_byGameLevelColor,(m_byGameLevel[0]&0x0F),tFirstOutCard.byCards[0]))
			#pragma region ����
			{
				OutputDebugString("chs [�Զ�����] �׼ҳ���������");
				BYTE *pCards = new BYTE[sizeof(BYTE)*2];	// allocate
				int ilen = 0;
				vector<BYTE> vMainCard;
				vMainCard.clear();
				m_cLogic.GetMainCard(m_byGameLevelColor,m_byGameLevel[0],m_vbyCards,vMainCard);

				for (int i = 0 ; i<vMainCard.size();++i)
				{
					CString cs;
					cs.Format("chs [�Զ�����] ����%d �ǣ� %d",i,vMainCard[i]);
					OutputDebugString(cs);
				}

				sort(vMainCard.begin(),vMainCard.end());
				if(vMainCard.size() == 0)
				{
					pOutCards[0] = m_vbyCards[m_vbyCards.size()-1];
					pOutCards[1] = m_vbyCards[m_vbyCards.size()-2];
					*piCount = 2;
				}
				if(vMainCard.size() <2 && vMainCard.size()>0)
				{
					pOutCards[0] = m_vbyCards[0];
					for (vector<BYTE>::iterator it = m_vbyCards.begin(); it != m_vbyCards.end();++it)
					{
						if(*it != pOutCards[0])
						{
							pOutCards[0] = *it;
							break;
						}
					}
					*piCount = 2;
					return ;
				}
				if(vMainCard.size() >= 2)
				{
					for (int i = 0;i<vMainCard.size()-1;++i)
					{
						if(vMainCard[i] == vMainCard[i+1])
						{
							pOutCards[0] = vMainCard[i];
							pOutCards[1] = vMainCard[i+1];
							*piCount = 2;
							return ;
						}
					}
					pOutCards[0] = vMainCard[0];
					pOutCards[1] = vMainCard[1];
					*piCount = 2;
					return ;
				}

			}
			#pragma endregion ����
			else
			#pragma region ����
			{
				BYTE byCards[2] = {0};
				int iCount = 0;
				if(m_cLogic.GetDoubleCard(byColor,m_vbyCards,byCards,&iCount) )
				{
					OutputDebugString("chs [�Զ�����] �ж���");
					pOutCards[0] = byCards[0];
					pOutCards[1] = byCards[1];
					*piCount = 2;
					return ;
				}
				else
				{
					OutputDebugString("chs [�Զ�����] û�ж���");
					vector<BYTE> vSameWithFirst;
					vSameWithFirst.clear();
					for (vector<BYTE>::iterator it = m_vbyCards.begin(); it != m_vbyCards.end();++it)
					{
						BYTE temp1 =  (*it)&0xF0;
						if( temp1 == byColor )
						{
							CString cs1;
							vSameWithFirst.push_back(*it);
						}
					}
					if(vSameWithFirst.size()>=2)
					{
						pOutCards[0] = vSameWithFirst[0];
						pOutCards[1] = vSameWithFirst[1];
						*piCount = 2;
						return ;
					}
					else if(vSameWithFirst.size()  == 1)
					{
						pOutCards[0] = vSameWithFirst[0];
						for (vector<BYTE>::iterator it = m_vbyCards.begin(); it != m_vbyCards.end();++it)
						{
							if(*it != pOutCards[0])
							{
								pOutCards[1] = *it;
								*piCount = 2;
								return ;
							}
						}
					}
					else if(vSameWithFirst.size() == 0)
					{
						pOutCards[0] = m_vbyCards[m_vbyCards.size()-1];
						pOutCards[1] = m_vbyCards[m_vbyCards.size()-2];
						*piCount = 2;
						return;
					}
					return ;
				}
			}
			#pragma endregion ����
		}
		break;
		#pragma endregion ����
	case E_CS_SERIALDOUBLE:
		#pragma region ������
		{
		}
		break;
		#pragma endregion ������
	case E_CS_MIXED:
		#pragma region ˦��
		{
		}
		break;
		#pragma endregion ˦��
	}
}

void CClientGameDlg::ShowCurrentGameLevel(TGameLevel *pTGameLevel)
{
	

	switch(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation)
	{
	case 0:
	case 2:
		{
			m_byGameLevel[0] = pTGameLevel->byGoup1Level;
		}
		break;
	case 1:
	case 3:
		{
			m_byGameLevel[1] = pTGameLevel->byGoup2Level;
		}
		break;
	}
}

//����׼�����˳���Ϣ
void CClientGameDlg::ReadyOrQuit(bool bReadyOrQuit)
{
	TGetReady tGetReady;		//׼����Ϣ��
	tGetReady.byDeskStation = m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
	if(bReadyOrQuit)
		SendGameData(&tGetReady,sizeof(tGetReady),MDM_GM_GAME_NOTIFY,ASS_GETREADY,0);
	else
		SendGameData(&tGetReady,sizeof(tGetReady),MDM_GM_GAME_NOTIFY,ASS_QUIT,0);
}


//������Ϸ״̬
void CClientGameDlg::ResetGameStatus()
{
	memset(m_byGameLevel,0,sizeof(BYTE)*2);
	memset(m_byCards,0,sizeof(BYTE)*33);
	memset(m_byBackCard,0,sizeof(BYTE)*8);
	m_vbyCards.clear();
	m_vbyBigJor.clear();
	m_vbySmallJor.clear();
	m_vbyLevelCard.clear();
	memset(&m_tFirstPlayerCard,0,sizeof(T_CS_OutCard));
	memset(&m_tCurrentOutCard,0,sizeof(T_CS_OutCard));	

	m_CardCounts = 25;
}