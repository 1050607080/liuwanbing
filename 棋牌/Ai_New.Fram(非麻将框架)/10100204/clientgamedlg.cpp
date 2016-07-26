#include "StdAfx.h"
#include "clientgamedlg.h"
#include "gameframedlg.h"
#include <time.h>

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() :CLoveSendClass(IDD_GAME_FRAME)
{

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
	TGameStation *ptGameStation = (TGameStation *)pStationData;
	if (ptGameStation == NULL)
		return false;
	switch (ptGameStation->enGameStation)
	{
	case GS_NOMODE:	
		{
			TGameStation tGameStation;
			tGameStation.byGameDeskNO = m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation ;
			tGameStation.enGameStation = GS_MODESETTING;
			SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GAMESTATIONCHANGED,0);	//�ı���Ϸ״̬
			srand((unsigned)time(NULL));
			int iTime=(rand()%7)+1;
			SetGameTimer(tGameStation.byGameDeskNO,iTime,TIMER_SELECTGAMEMODE);			//������ʱ��
			return TRUE;
		}
		break;
	case GS_MODESETTING:	//ģʽ���� 
	case GS_WITHMODE:		//��Ϸģʽ������
	case GS_READY:			//׼��
	case GS_GAMEOVER:		//��Ϸ����
		{
			srand((unsigned)time(NULL));
			int iTime =  (rand()%6) +1;
			SetGameTimer(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,iTime,TIMER_GETREADY);
			SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
			return true;
		}
		break;
	}
	return TRUE;
}

void CClientGameDlg::OnTimerBetting(int iTimerID)
{
	T_CS_Bet tBet;
	tBet.iBetMony = m_pGameInfo->pMeUserInfo->GameUserInfo.i64Money / 20;
	tBet.byDeskStation = m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
	SendGameData(&tBet,sizeof(tBet),MDM_GM_GAME_NOTIFY,GM_BET,0);
}

void CClientGameDlg::OnMessageBetting(void *pData,int iLen)
{
	if (iLen != sizeof(T_SC_WhoIsBanker))
		return;

	T_SC_WhoIsBanker *ptBanker = (T_SC_WhoIsBanker *)pData;
	if (ptBanker->byDeskStation == m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
		return;
	}
	else
	{
		srand(unsigned(time(NULL)));
		int iTime = (rand()%6) +1;
		SetGameTimer(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation,iTime,TIMER_BETTING);
	}
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case GM_BET://��ע		
			{	
				OnMessageBetting(pNetData,uDataSize);
				return TRUE;
			}
			break;
		case GM_WASHCARD:
			{
				return TRUE;
			}
			break;
		case GM_SENDCARD:
			{
				return TRUE;
			}
			break;
		}
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
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
	ResetGameStation(0);
	return true;
}
//����Ϊ��λ
bool CClientGameDlg::SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUserMilliSecond)
{
	int iTimeType = 1000 ; 

	if(m_pGameRoom != NULL)
	{
		m_pGameRoom->SendMessage(IDM_SET_GAME_TIME,(WPARAM)uTimeID,(LPARAM)uTimeCount*iTimeType);
	}
	return true;
}

void CClientGameDlg::OnTimerSelectGameMode(int iTimerID)
{
	KillGameTimer(iTimerID);
	TGameStation tGameStation;	//ģʽ��״̬����λ��
	tGameStation.byGameDeskNO = m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;//��λ��
	tGameStation.enGameStation = GS_WITHMODE;//GAMESTATION_GAMEMODESELECTED;
	tGameStation.enGameMode = GAMEMODE_BANKER_CONST;
	SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GAMESTATIONCHANGED,0);	//�������֪ͨ��Ϸģʽ���óɹ�����������������Ϸ״̬����Ϊ GAMESTATION_WATINGBEGIN��
	//SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	//׼��

	SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GETREADY,0);
}

void CClientGameDlg::OnTimerGetReady(int iTimerID)
{
	KillGameTimer(iTimerID);
	TGameStation tGameStation;
	tGameStation.byGameDeskNO = m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
	SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GETREADY,0);	//׼��
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case TIMER_SELECTGAMEMODE:
		{
			OnTimerSelectGameMode(TIMER_SELECTGAMEMODE);
			return true;
		}
		break;
	case TIMER_GETREADY:
		{
			OnTimerGetReady(TIMER_GETREADY);
			return true;
		}
		break;

	case TIMER_BETTING:
		{
			OnTimerBetting(TIMER_BETTING);
			return true;
		}
		break;

	default:
		break;
	}
	return TRUE;
}


//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)//��Ϸ��ʼ
{
	return;
}

//ɱ��ʱ��
void CClientGameDlg::KillAllTimer()
{
	
}
