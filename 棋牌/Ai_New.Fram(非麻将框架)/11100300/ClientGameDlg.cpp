#include "StdAfx.h"
#include "ClientGamedlg.h"

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	InitGameData();
} 

//��������
CClientGameDlg::~CClientGameDlg()
{

}
//��ʼ����Ϸ����
void CClientGameDlg::InitGameData()
{
	m_RobotCanBet=TRUE;		//�������ܷ���ע
	KillAllTimer();
	LoadGameIni();
}
//---------------------------------------------------------------------------------------
///��ʼ����������
void	CClientGameDlg::ResetGameData()
{
	m_RobotCanBet=TRUE;		//�������ܷ���ע 
	//KillAllTimer();
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	OutputDebugString("Robot::OnInitDialog");
	CLoveSendClass::OnInitDialog();
	return TRUE;
}
//���������ļ�
void CClientGameDlg::LoadGameIni()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();///����·��
	CINIFile f( s +nid +"_robot.ini");
	CString key = TEXT("robot");
	m_iRobotBetSumLimit = f.GetKeyVal(key,"RobotBetSumLimit",100000);		//��������ע����
	m_iRobotMaxNtLimit = f.GetKeyVal(key,"RobotMaxNtLimit",8);			//�����������ׯ��������
}
//------------------------------------------------------------------------------------------------------------------
//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	
	//��ȡ�Լ���λ��
	m_iMyDeskStation = GetMeUserInfo()->bDeskStation;
	/*CString cs;
	cs.Format("Robot::��Ϸ״̬=%d",m_pGameInfo->bGameStation);
	OutputDebugString(cs);*/
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
	case GS_WAIT_NEXT:			//�ȴ����̿�ʼ
		{
			
			GameStation_Base * pGameStation=(GameStation_Base *)pStationData;
			if (NULL == pGameStation)
			{
				
				return false;
			}

			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬

			m_i64ApplyNtLimite=pGameStation->iApplyNtLimite ; //ׯ����ׯ����
			m_byNtStation=pGameStation->byNtStation;		//ׯ��λ��
			m_byMaxNtPeople=pGameStation->byMaxNtPeople;		//�����ׯ����

			ApplyNtMsg applyMsg;
			applyMsg.byDeskStation = m_iMyDeskStation;
			//������ׯ
			SendGameData(&applyMsg, sizeof(applyMsg) ,MDM_GM_GAME_NOTIFY,ASS_APPLY_NT,0);

			break;
		}
	case GS_FREE_STATUS://��Ϸ����״̬
		{
			
			GameStation_Base * pGameStation=(GameStation_Base *)pStationData;
			if (NULL == pGameStation)
			{
				
				return false;
			}

			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬
			m_i64ApplyNtLimite=pGameStation->iApplyNtLimite ; //ׯ����ׯ����
			m_byNtStation=pGameStation->byNtStation;		//ׯ��λ��
			m_byMaxNtPeople=pGameStation->byMaxNtPeople;		//�����ׯ����

			int iNtCount=0;
			BOOL bCanNt=TRUE;//�ܷ���ׯ��־
			memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
			//������ׯ�б���Ϣ
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(255==pGameStation->byWaitNtList[i])
				{
					continue;
				}
				if(m_iMyDeskStation==pGameStation->byWaitNtList[i])
				{
					bCanNt=FALSE;
				}
				m_byWaitNtList[i]=pGameStation->byWaitNtList[i];
				iNtCount++;
			}

			JudgeApplyNt(iNtCount,bCanNt);

			break;
		}
	case GS_BET_STATUS:	//��Ϸ��ע�׶�
		{
			
			GameStation_Base * pGameStation=(GameStation_Base *)pStationData;
			if (NULL == pGameStation)
			{
				
				return false;
			}
			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬
			m_i64ApplyNtLimite=pGameStation->iApplyNtLimite ; //ׯ����ׯ����
			m_byNtStation=pGameStation->byNtStation;		//ׯ��λ��
			m_byMaxNtPeople=pGameStation->byMaxNtPeople;		//�����ׯ����

			int iNtCount=0;
			BOOL bCanNt=TRUE;//�ܷ���ׯ��־
			memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
			//������ׯ�б���Ϣ
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(255==pGameStation->byWaitNtList[i])
				{
					continue;
				}
				if(m_iMyDeskStation==pGameStation->byWaitNtList[i])
				{
					bCanNt=FALSE;
				}
				m_byWaitNtList[i]=pGameStation->byWaitNtList[i];
				iNtCount++;
			}

			 JudgeApplyNt(iNtCount,bCanNt);

			//������ע��ʱ��
			srand(GetTickCount()+m_iMyDeskStation);
			int iTmpTime = ((rand()%3)+1);
			SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_BET);
			break;
		}
	case GS_SEND_STATUS://��Ϸ����״̬
		{
			
			GameStation_Base * pGameStation=(GameStation_Base *)pStationData;
			if (NULL == pGameStation)
			{
				
				return false;
			}

			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬
			m_i64ApplyNtLimite=pGameStation->iApplyNtLimite ; //ׯ����ׯ����
			m_byNtStation=pGameStation->byNtStation;		//ׯ��λ��
			m_byMaxNtPeople=pGameStation->byMaxNtPeople;		//�����ׯ����

			int iNtCount=0;
			BOOL bCanNt=TRUE;//�ܷ���ׯ��־
			memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
			//������ׯ�б���Ϣ
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(255==pGameStation->byWaitNtList[i])
				{
					continue;
				}
				if(m_iMyDeskStation==pGameStation->byWaitNtList[i])
				{
					bCanNt=FALSE;
				}
				m_byWaitNtList[i]=pGameStation->byWaitNtList[i];
				iNtCount++;
			}
			JudgeApplyNt(iNtCount,bCanNt);

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
				if (uDataSize != sizeof(GameStation_Base))
				{

					return false;
				}
				SetGameStation(pNetData, uDataSize);
				break;      
			}
		case ASS_GAME_FREE://����״̬
			{
				if(uDataSize!=sizeof(BeginFreeMsg))
				{
					
					return false;
				}
				HandleGameFree(pNetData);
				break;
			}
		case ASS_BEGIN_BET:	//��ע״̬
			{
				
				if(uDataSize!=sizeof(BeginBetMsg))
				{
					
					return false;
				}
				
				HandleBeginBet(pNetData);
				break;
			}
		case ASS_GAME_SEND://����״̬
			{
				if(uDataSize!=sizeof(BeginSendCardMsg))
				{
					
					return false;
				}
				//����
				HandleKaiPai(pNetData);
				break;
			}
		case ASS_APPLY_RESULT://������ׯ���
			{
				if(uDataSize!=sizeof(ApplyNtResultMsg))
				{
					
					return false;
				}
				HandleApplyResult(pNetData);
				break;
			}
		case ASS_LEAVE_RESULT://������ׯ���
			{
				if(uDataSize!=sizeof(ApplyLiveNtResultMsg))
				{
					
					return false;
				}
				HandleLeaveNtResult(pNetData);
				break;
			}
		case ASS_BET_RESULT:	//��ע���
			{
				if(uDataSize!=sizeof(UserBetResultMsg))
				{
					return false;
				}
				HandleUserBetResult(pNetData);
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

//������Ϣ
void	CClientGameDlg::HandleGameFree(void * pBuffer)
{
	BeginFreeMsg *pGameFree = (BeginFreeMsg *)pBuffer;
	if(NULL == pGameFree)
	{
		return ;
	}
	ResetGameData();
	m_byGameStation =pGameFree->byGameStation;         ///��ǰ����Ϸ״̬
	m_byNtStation = pGameFree->byNtStation;

	int iNtCount=0;
	BOOL bCanNt=TRUE;//�ܷ���ׯ��־
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==pGameFree->byWaitNtList[i])
		{
			continue;
		}
		if(m_iMyDeskStation==pGameFree->byWaitNtList[i])
		{
			bCanNt=FALSE;
		}
		m_byWaitNtList[i]=pGameFree->byWaitNtList[i];
		iNtCount++;
	}

	JudgeApplyNt(iNtCount,bCanNt);
}
//-------------------------------------------------------------------------
//�յ���ʼ��ע��Ϣ
void	CClientGameDlg::HandleBeginBet(void * pBuffer)
{

	BeginBetMsg *pBeginBet = (BeginBetMsg *)pBuffer;
	if(NULL == pBeginBet)
	{
		return ;
	}
	
	m_byGameStation=pBeginBet->byGameStation;//��Ϸ״̬

	//������ע��ʱ��
	srand(GetTickCount()+m_iMyDeskStation);
	int iTmpTime = ((rand()%3)+1);
	SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_BET);

	int iNtCount=0;
	BOOL bCanNt=TRUE;//�ܷ���ׯ��־
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==pBeginBet->byNtWaitList[i])
		{
			continue;
		}
		if(m_iMyDeskStation==pBeginBet->byNtWaitList[i])
		{
			bCanNt=FALSE;
		}
		m_byWaitNtList[i]=pBeginBet->byNtWaitList[i];
		iNtCount++;
	}

	JudgeApplyNt(iNtCount,bCanNt);

}
//����
void CClientGameDlg::HandleKaiPai(void *buffer)
{
	BeginSendCardMsg *pBeginKaiPai = (BeginSendCardMsg *)buffer;
	if(NULL == pBeginKaiPai)
	{
		return ;
	}
	//ɾ����ע��ʱ��
	KillGameTimer(TIME_BET);
	m_byGameStation =pBeginKaiPai->byGameStation;         ///��ǰ����Ϸ״̬

	BYTE iNtCount=0;
	BOOL bCanNt=TRUE;//�ܷ���ׯ��־
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==pBeginKaiPai->byNtWaitList[i])
		{
			continue;
		}
		if(m_iMyDeskStation==pBeginKaiPai->byNtWaitList[i])
		{
			bCanNt=FALSE;
		}
		m_byWaitNtList[i]=pBeginKaiPai->byNtWaitList[i];
		iNtCount++;
	}

	JudgeApplyNt(iNtCount,bCanNt);
}

//������ׯ�������
void CClientGameDlg::HandleApplyResult(void *buffer)
{
	ApplyNtResultMsg *pApply = (ApplyNtResultMsg *)buffer;
	if(NULL==pApply)
	{
		return;
	}

	m_byNtStation=pApply->byNtStation;		//ׯ��λ��

	int iNtCount=0;//ͳ����ׯ�б������
	BOOL bCanNt=TRUE;//�ܷ���ׯ��־
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==pApply->byWaitNtList[i])
		{
			continue;
		}
		if(m_iMyDeskStation==pApply->byWaitNtList[i])
		{
			bCanNt=FALSE;
		}
		m_byWaitNtList[i]=pApply->byWaitNtList[i];
		iNtCount++;
	}

	JudgeApplyNt(iNtCount,bCanNt);
}

//����������ׯ�����Ϣ
void CClientGameDlg::HandleLeaveNtResult(void *buffer)
{

	ApplyLiveNtResultMsg *ShowResultMsg = (ApplyLiveNtResultMsg *)buffer;
	if(NULL == ShowResultMsg)
	{
		return ;
	}
	m_byNtStation=ShowResultMsg->byNtStation;
	
	int iNtCount=0;//ͳ����ׯ�б������
	BOOL bCanNt=TRUE;//�ܷ���ׯ��־
	//������ׯ�б�
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==ShowResultMsg->byWaitNtList[i])
		{
			continue;
		}
		if(m_iMyDeskStation==ShowResultMsg->byWaitNtList[i])
		{
			bCanNt=FALSE;
		}
		m_byWaitNtList[i]=ShowResultMsg->byWaitNtList[i];
		iNtCount++;
	}

	JudgeApplyNt(iNtCount,bCanNt);
}
//���������ע���
void CClientGameDlg::HandleUserBetResult(void *buffer)
{

	UserBetResultMsg *BetResultMsg = (UserBetResultMsg *)buffer;
	if(NULL == BetResultMsg)
	{
		return ;
	}
	__int64 i64Temp=0;
	for(int i=0;i<BET_ARES;i++)
	{
		//�����������ע
		i64Temp+=BetResultMsg->i64UserBetCount[i];
	}
	if(i64Temp>m_iRobotBetSumLimit)//��������ע���˲�������ע��
	{
		m_RobotCanBet=FALSE;		//�������ܷ���ע
		KillTimer(TIME_BET);;	//ɱ����ע��ʱ��
	}
}
//�ж��Ƿ�������ׯ
void CClientGameDlg::JudgeApplyNt(BYTE iNtCount,BOOL flag)
{
	if((iNtCount<m_iRobotMaxNtLimit) && flag)//������ׯ
	{
		//������ׯ��ʱ��
		srand(GetTickCount()+m_iMyDeskStation);
		int iTmpTime = ((rand()%5)+2);
		SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_APPLY);
	}
	if(!flag)//������ׯ
	{
		srand(GetTickCount()+m_iMyDeskStation);
		//������ׯ��ʱ��
		int iTmpTime = ((rand()%5)+5);
		SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_LEAVE);  
	}
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	OutputDebugString("Robot::ResetGameFrame");
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
	OutputDebugString("Robot::ResetGameFrame");
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
	OutputDebugString("Robot::OnCancel");
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
	case TIME_BET:	//��ע��ʱ��
		{
			KillGameTimer(TIME_BET);

			if(!m_RobotCanBet)//������ע
			{
				return true;
			}

			RobortBet();
			
			//�ظ���ע
			if (m_byGameStation	== GS_BET_STATUS)
			{
				int iTmpTime = (rand()%3+1);
				SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_BET);
			}

			return true;
		}
	case TIME_APPLY:	//������ׯ
		{
			KillGameTimer(TIME_APPLY);

			//������������ׯ
			RobortApplyNt();

			////�ظ�������ׯ��ʱ��
			//srand(GetTickCount()+m_iMyDeskStation);
			//int iTmpTime = ((rand()%6)+1);
			//SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_APPLY);

			return true;
		}
	case TIME_LEAVE://������ׯ
		{
			KillGameTimer(TIME_LEAVE);

			//������������ׯ
			RobortApplyLeave();

			////�ظ�������ׯ��ʱ��
			//srand(GetTickCount()+m_iMyDeskStation);
			////������ׯ��ʱ��
			//int iTmpTime = ((rand()%8)+1);
			//SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_LEAVE);
			return true;
		}	
	}

	return true;
}
//������ж�ʱ��
void	CClientGameDlg::KillAllTimer()
{
	KillGameTimer(TIME_BET);
	KillGameTimer(TIME_APPLY);
	KillGameTimer(TIME_LEAVE);
	return;
}
//------------------------------------------------------------------------------------------------------------------
//��������ׯ
void	CClientGameDlg::RobortApplyNt()
{
	srand(GetTickCount()+m_iMyDeskStation);
	int iTmpRand = rand()%100;
	//50%�ĸ���������ׯ
	if (iTmpRand < 50)
	{
		ApplyNtMsg applyMsg;
		applyMsg.byDeskStation = m_iMyDeskStation;
		SendGameData(&applyMsg, sizeof(applyMsg) ,MDM_GM_GAME_NOTIFY,ASS_APPLY_NT,0);
	}

	return;
}
//��������ׯ
void CClientGameDlg::RobortApplyLeave()
{
	srand(GetTickCount()+m_iMyDeskStation);
	if(m_byNtStation==m_iMyDeskStation)
	{
		if((GS_BET_STATUS==m_byGameStation) || (GS_SEND_STATUS==m_byGameStation))
		{
			return;
		}
	}
	int iRand = rand()%100;
	//50%�ĸ���������ׯ
	if (iRand < 50)
	{
		ApplyLiveNtMsg liveNtMsg;
		liveNtMsg.byDeskStation=m_iMyDeskStation;
		SendGameData(&liveNtMsg, sizeof(liveNtMsg) ,MDM_GM_GAME_NOTIFY,ASS_APPLY_LEAVE,0);
	}
}
//------------------------------------------------------------------------------------------------------------------
//��������ע
void	CClientGameDlg::RobortBet()
{
	
	if (m_byGameStation != GS_BET_STATUS)
	{
		return;
	}
	//m_pGameInfo->pMeUserInfo->GameUserInfo.i64Money;

	//���һ����ע����
	BYTE byBetArea = 0;
	int iRand = rand()%100;
	if(iRand >= 0 && iRand <=39)
	{
		byBetArea=0;
	}
	else if (iRand >= 40 && iRand <=79)
	{
		byBetArea=1;
	}
	else if(iRand >= 80 && iRand <=99)
	{
		byBetArea=2;
	}
	
	//���һ����������
	BYTE byChouMaType = 0;
	iRand = rand()%100;

	if (iRand >= 0 && iRand <=24)
	{
		byChouMaType = 0;
	}
	else if (iRand >= 25 && iRand <= 44)	
	{
		byChouMaType = 1;
	}
	else if (iRand >= 45 && iRand <=62)	
	{
		byChouMaType = 2;
	}
	else if (iRand >= 63 && iRand <= 77)	
	{
		byChouMaType = 3;
	}
	else if (iRand >= 78 && iRand <= 90)	
	{
		byChouMaType = 4;
	}
	else if (iRand >= 91 && iRand <= 99) 
	{
		byChouMaType = 5;
	}
	CString cs;
	cs.Format("test ��������ע=%d",byBetArea);
	OutputDebugString(cs);
	UserBetMsg betMsg;
	betMsg.byDeskStation=m_iMyDeskStation;
	betMsg.byBetArea=byBetArea;
	betMsg.byChouMaType=byChouMaType;
	SendGameData(&betMsg, sizeof(betMsg) ,MDM_GM_GAME_NOTIFY,ASS_USER_BET,0);
}


