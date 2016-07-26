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
	
	m_byGameStation = GS_WAIT_AGREE;


	m_iXiaZhuTime	= 15;			///��עʱ��	
	m_iKaiPaiTime	= 15;			///����ʱ��  
	m_iShowResult	= 5;			///��ʾ�����ʱ��
	m_iFreeTime		= 15;			///����ʱ��	

	m_iLimitMoney	= 0;			///<��ׯ��Ҫ�����ٽ��
	m_iMaxNote		= 0;			///��ǰ����������µ���ע

	m_iNTju			= 0;			///<��ǰׯ�ҽ��еľ���
	m_iNowNTstation	= 0;			///<��ǰׯ�ҵ�λ��

	m_i64MyWinMoney = 0;			//ׯ�ҵĳɼ�
	m_i64MyBetCount = 0;			//�Լ���ע����
	m_i64MyWinMoney	= 0;			//�Լ���Ӯ���
	


	ZeroMemory(m_i64MaxZhu,sizeof(m_i64MaxZhu));	//ÿ���������µ����ע	
	memset(m_iZhuangList,-1,sizeof(m_iZhuangList));	///ׯ���б�
	memset(m_byNowCard,-1,sizeof(m_byNowCard));		///<��ǰ����//ׯ�е��ƣ�0Ϊ�У�1Ϊׯ


	
}

//��������
CClientGameDlg::~CClientGameDlg()
{

}

//���ݰ󶨺���
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClass::DoDataExchange(pDX);
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClass::OnInitDialog();

	CINIFile iniFile(CINIFile::GetAppPath()+"BJLRobotSet.ini");
	m_iRobotMaxBet = iniFile.GetKeyVal("BJL", "RobotMaxBet", 0);

	return TRUE;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{

	GameStationBase *pGameStation = (GameStationBase *)pStationData;
	if (NULL == pGameStation)
	{
		return TRUE;
	}
	srand(GetTickCount()+GetMeUserInfo()->bDeskStation);
	SetStationParameter(pGameStation->m_bGameStation);
	OutputDebugString("bjlrobot::������Ϸ״̬");
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_AGREE:
	case GS_WAIT_NEXT:			//�ȴ����̿�ʼ
		{	
			if (uDataSize != sizeof(GameStationBase))
			{
				return TRUE;
			}
			GameStationBase * pGameStation=(GameStationBase *)pStationData;
			if (NULL == pGameStation)
			{
				return TRUE;
			}
			//��Ϸ״̬

			m_byGameStation	= GS_WAIT_AGREE;

			m_iXiaZhuTime	= pGameStation->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime	= pGameStation->m_iKaiPaiTime;      //����ʱ��
			m_iShowResult	= pGameStation->m_iShowResult;		
			m_iFreeTime		= pGameStation->m_iFreeTime;		//����ʱ��

			m_iNTju			= pGameStation->m_iZhuangBaShu;
			m_iNowNTstation = pGameStation->m_iNowNtStation;
			m_i64NtWinMoney	= pGameStation->i64NtWin;
			m_i64MyWinMoney = pGameStation->i64MeWin;

			m_iLimitMoney	= pGameStation->m_iShangZhuangLimit;//��ׯ�޶�
			m_iMaxNote		= pGameStation->iMaxNote;	

			OutputDebugString("bjlrobot::������Ϸ״̬-0");
			///ׯ���б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_iZhuangList[i] = pGameStation->iZhuangWaitList[i];
			}

			if (m_iNowNTstation == -1 || m_iNowNTstation == 255)
			{
				//����������ׯ��ʱ��
				int iTime = (rand()%5+1);
				//������ע��ʱ��
				CString dwjlog;
				dwjlog.Format("bjlrobot::iTime-2=%d",iTime);
				OutputDebugString(dwjlog);
				SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, ID_APPLYNT_TIMER);
			}

			return TRUE;
		}
	case GS_NOTE_STATE:	//��ע״̬
		{
			if (uDataSize != sizeof(GameStation_Bet))
			{
				return TRUE;
			}
			GameStation_Bet * pGameStation=(GameStation_Bet *)pStationData;
			if (NULL == pGameStation)
			{
				return TRUE;
			}
			//��Ϸ״̬
			m_byGameStation	= GS_NOTE_STATE;

			m_iXiaZhuTime	= pGameStation->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime	= pGameStation->m_iKaiPaiTime;      //����ʱ��
			m_iShowResult	= pGameStation->m_iShowResult;		
			m_iFreeTime		= pGameStation->m_iFreeTime;		//����ʱ��

			m_iNTju			= pGameStation->m_iZhuangBaShu;
			m_iNowNTstation = pGameStation->m_iNowNtStation;

			m_i64NtWinMoney	= pGameStation->i64NtWin;
			m_i64MyWinMoney = pGameStation->i64MeWin;

			m_iLimitMoney	= pGameStation->m_iShangZhuangLimit;//��ׯ�޶�
			m_iMaxNote		= pGameStation->iMaxNote;	

			OutputDebugString("bjlrobot::������Ϸ״̬-1");
			///ׯ���б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_iZhuangList[i] = pGameStation->iZhuangWaitList[i];
			}
			
			//����ע���
			for(int i=0;i<BET_ARES;i++)
			{
				m_i64MaxZhu[i] = pGameStation->i64AresMaxZhu[i];
			}
				

			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
		{
			if (uDataSize != sizeof(GameStation_SendCard))
			{
				return TRUE;
			}
			GameStation_SendCard * pGameStation=(GameStation_SendCard *)pStationData;
			if (NULL == pGameStation)
			{
				return TRUE;
			}
			//��Ϸ״̬
			m_byGameStation	= GS_SEND_CARD;

			m_iXiaZhuTime	= pGameStation->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime	= pGameStation->m_iKaiPaiTime;      //����ʱ��
			m_iShowResult	= pGameStation->m_iShowResult;		
			m_iFreeTime		= pGameStation->m_iFreeTime;		//����ʱ��

			m_iNTju			= pGameStation->m_iZhuangBaShu;
			m_iNowNTstation = pGameStation->m_iNowNtStation;

			m_i64NtWinMoney	= pGameStation->i64NtWin;
			m_i64MyWinMoney = pGameStation->i64MeWin;

			m_iLimitMoney	= pGameStation->m_iShangZhuangLimit;//��ׯ�޶�
			m_iMaxNote		= pGameStation->iMaxNote;	
			OutputDebugString("bjlrobot::������Ϸ״̬-2");
			///ׯ���б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_iZhuangList[i] = pGameStation->iZhuangWaitList[i];
			}
		
			//����ע���
			for(int i=0;i<BET_ARES;i++)
			{
				m_i64MaxZhu[i] = pGameStation->i64AresMaxZhu[i];
			}
			
			m_i64MyBetCount = pGameStation->i64UserXiaZhuSum;
		
			//������
			for(int i=0; i<2; i++)
			{
				for(int j=0; j<3; j++)
				{
					m_byNowCard[i][j] = pGameStation->byUserCard[i][j];
				}
			}

			return TRUE;
		}
	case GS_OPEN_RESULT:	//����״̬
		{
			if (uDataSize != sizeof(GameStation_Result))
			{
				return TRUE;
			}
			GameStation_Result * pGameStation=(GameStation_Result *)pStationData;
			if (NULL == pGameStation)
			{
				return TRUE;
			}
			//��Ϸ״̬
			m_byGameStation	= GS_OPEN_RESULT;

			m_iXiaZhuTime	= pGameStation->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime	= pGameStation->m_iKaiPaiTime;      //����ʱ��
			m_iShowResult	= pGameStation->m_iShowResult;		
			m_iFreeTime		= pGameStation->m_iFreeTime;		//����ʱ��

			m_iNTju			= pGameStation->m_iZhuangBaShu;
			m_iNowNTstation = pGameStation->m_iNowNtStation;

			m_i64NtWinMoney	= pGameStation->i64NtWin;
			m_i64MyWinMoney = pGameStation->i64MeWin;

			m_iLimitMoney	= pGameStation->m_iShangZhuangLimit;//��ׯ�޶�
			m_iMaxNote		= pGameStation->iMaxNote;	

			OutputDebugString("bjlrobot::������Ϸ״̬-3");
			///ׯ���б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_iZhuangList[i] = pGameStation->iZhuangWaitList[i];
			}
			
			//����ע���
			for(int i=0;i<BET_ARES;i++)
			{
				m_i64MaxZhu[i] = pGameStation->i64AresMaxZhu[i];
			}
			
			m_i64MyBetCount = pGameStation->i64UserXiaZhuSum;
			
			
			return TRUE;
		}
	}
	return false;
}
/*--------------------------------------------------------------------------*/
//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_BJL_BEGIN:	//�ͻ����յ��ټ��ֿ�ʼ��Ϣ
			{
				if (sizeof(BEGIN_DATA) != uDataSize)
				{
					return false;
				}

				BEGIN_DATA *pGameBegin = (BEGIN_DATA *)pNetData;
				if (NULL == pGameBegin)
				{
					return false;
				}
			
				ResetGameStation(0);

				m_byGameStation	= GS_NOTE_STATE;
				SetStationParameter(GS_NOTE_STATE);

				m_iXiaZhuTime	= pGameBegin->m_iXiaZhuTime;		// ��עʱ��			
				m_iKaiPaiTime	= pGameBegin->m_iKaiPaiTime;       // ����ʱ��
				m_iShowResult	= pGameBegin->m_iShowResult;
				m_iFreeTime		= pGameBegin->m_iFreeTime;			// ����ʱ��
				m_iMaxNote		= pGameBegin->iMaxNote;				// ��ǰ����������µ���ע

				m_iNowNTstation = pGameBegin->m_iNowNtStation;		//��ȡׯ�ҵ�λ��
				m_i64NtWinMoney = pGameBegin->m_i64NtWin;			//ׯ�ҳɼ�
				m_iNTju			= pGameBegin->m_iZhuangBaShu;		//ׯ�ҵľ���
				m_iLimitMoney	= pGameBegin->m_iShangZhuangLimit;	//��ׯ�޶�

				for(int i=0;i<=PLAY_COUNT;i++)
				{
					m_iZhuangList[i] = pGameBegin->m_iNTlist[i];//��ȡׯ���б�����
				}

				if (GetMeUserInfo()->bDeskStation != pGameBegin->m_iNowNtStation)
				{

					int iTime = (rand()%5+1);
					//������ע��ʱ��
					SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, ID_BET_TIMER);
				}
				return true;
			}
		case ASS_KAI_PAI:			// ������Ϣ
			{
				KillTimer(ID_BET_TIMER);
				SetStationParameter(GS_SEND_CARD);
				return true;
			}
		case ASS_XIA_ZHU:		//�յ���ע��Ϣ
			{
				if (sizeof(XIA_ZHU) != uDataSize)
				{
					return false;
				}
				XIA_ZHU *pXiaZhu = (XIA_ZHU *)pNetData;
				if (NULL == pXiaZhu)
				{
					return false;
				}
				m_i64MyBetCount = pXiaZhu->iMeZhongZhu;
				return true;
			}
		case ASS_SHOW_JS:		//��ʾ�����
			{
				return true;
			}
		case ASS_FREE_STATE:	//��Ϸ���� ���н׶�
			{
				if (sizeof(DENG_DAI) != uDataSize)
				{
					return false;
				}
				DENG_DAI *pDengDai = (DENG_DAI *)pNetData;
				if (NULL == pDengDai)
				{
					return false;
				}
				SetStationParameter(GS_WAIT_NEXT);

				for(int i=0;i<=PLAY_COUNT;i++)
				{
					m_iZhuangList[i] = pDengDai->zhuangstation[i];//��ȡׯ���б�����
				}

				//����������ׯ��ʱ��
				int iRand = rand()%10;
				if (0 == iRand)
				{
					int iTime = ((rand()%(m_iXiaZhuTime/2+m_iKaiPaiTime/2+m_iShowResult/2+m_iFreeTime/2))+1);
					//������ע��ʱ��
					CString dwjlog;
					dwjlog.Format("bjlrobot::iTime-1=%d",iTime);
					OutputDebugString(dwjlog);
					SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, ID_APPLYNT_TIMER);
				}

				return true;
			}
		case ASS_WU_ZHUANG:	//��ׯ��Ϣ֪ͨ
			{
				if (sizeof(tagNoZhuang) != uDataSize)
				{
					return false;
				}
				tagNoZhuang *pNoZhuang = (tagNoZhuang *)pNetData;
				if (NULL == pNoZhuang)
				{
					return false;
				}

				//����������ׯ��ʱ��
				int iRand = rand()%10;
				if (iRand >=5)
				{
					int iTime = ((rand()%10)+1);
					CString dwjlog;
					dwjlog.Format("bjlrobot::iTime-3=%d",iTime);
					OutputDebugString(dwjlog);
					SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, ID_APPLYNT_TIMER);
				}

				break;
			}
		case ASS_SHANG_ZHUANG:	//��ׯ�ɹ�
			{
				if (sizeof(SHANG_ZHUANG) != uDataSize)
				{
					return false;
				}
				SHANG_ZHUANG *pShangZhuang = (SHANG_ZHUANG *)pNetData;
				if (NULL == pShangZhuang)
				{
					return false;
				}
				for(int i=0; i<PLAY_COUNT; i++)
				{
					m_iZhuangList[i] = pShangZhuang->zhuangstation[i];
				}

				return true;
			}
		case ASS_SHANG_ZHUANG_ERR:		//��ׯʧ��
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
	m_i64MyBetCount = 0;			//�Լ���ע����
	ZeroMemory(m_i64MaxZhu,sizeof(m_i64MaxZhu));	//ÿ���������µ����ע	
	memset(m_byNowCard,-1,sizeof(m_byNowCard));		///<��ǰ����//ׯ�е��ƣ�0Ϊ�У�1ׯ

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
		||(GetStationParameter()==GS_WAIT_NEXT))
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
	KillTimer(ID_BET_TIMER);
	KillTimer(ID_APPLYNT_TIMER);
	return;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			return true;
		}
	case ID_BET_TIMER:	//��ע
		{
			KillTimer(ID_BET_TIMER);
			if(GetStationParameter() == GS_NOTE_STATE)
			{
				//���ͽ�ׯ��Ϣ
				XIA_ZHU  xiazhu;
				xiazhu.moneytype = GetSelChouMa();
				xiazhu.type		 = GetSelArea();

				if ((m_iRobotMaxBet <=0) ||((m_iRobotMaxBet > 0 && m_i64MyBetCount+G_iChouMaValues[xiazhu.moneytype]<=m_iRobotMaxBet))) 
				{
					SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
					//����������ע
					int iTime = (rand()%5+1);
					//������ע��ʱ��
					SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, ID_BET_TIMER);
				}	
			}
			return true;
		}
	case ID_APPLYNT_TIMER:
		{
			KillTimer(ID_APPLYNT_TIMER);

			//����Ƿ��Ѿ���ׯ���б���
			bool bCanApply = true;
			if (m_iNowNTstation == GetMeUserInfo()->bDeskStation)
			{
				bCanApply = false;
			}

			for(int i=0; i<PLAY_COUNT;i++)
			{
				if (m_iZhuangList[i] == GetMeUserInfo()->bDeskStation)
				{
					bCanApply = false;
					break;;
				}
			}

			if (bCanApply)
			{
				SHANG_ZHUANG zhuang;
				zhuang.shang=true;
				// ��������ׯ��Ϣ
				SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
			}
			
			return true;
		}	
	}
	return true;
}

//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	m_i64MyBetCount = 0;			//�Լ���ע����
	ZeroMemory(m_i64MaxZhu,sizeof(m_i64MaxZhu));	//ÿ���������µ����ע	
	memset(m_byNowCard,-1,sizeof(m_byNowCard));		///<��ǰ����//ׯ�е��ƣ�0Ϊ�У�1ׯ
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
	
	if (GetStationParameter()>GS_WAIT_NEXT)
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


//�����ȡ��ע����
int		CClientGameDlg::GetSelArea()
{
	int iMax = 9*11*4; //��Լ��
	int iNum[BET_ARES] = {iMax/2,iMax/3,iMax/12,iMax/2,iMax/3,iMax/12,iMax/9,iMax/33,iMax/2,iMax/2,iMax/2,iMax/2};
	int iSum[BET_ARES] = {0};
	for(int i=0; i<BET_ARES; i++)
	{
		if (0 == i)
		{
			iSum[0] = iNum[0];
		}
		else
		{
			iSum[i] = iSum[i-1]+iNum[i];
		}
	}

	int iRand = rand()*((rand()%100)+1);
	iRand = iRand%iSum[BET_ARES-1];
	
	int iArea = 0;
	if (iRand < iSum[0])
	{
		iArea = 0;
	}
	else
	{
		for(int i=1; i<BET_ARES; i++)
		{
			if (iRand>=iSum[i-1] && iRand<iSum[i])
			{
				iArea = i;
			}
		}
	}
	return iArea;
}


//�����ע����
int		CClientGameDlg::GetSelChouMa()
{
	int iMax = 10000;
	int iNum[6] = {iMax/5,iMax/5,iMax/10,iMax/20,iMax/100,iMax/200};
	int iSum[6] = {0};
	for(int i=0; i<6; i++)
	{
		if (0 == i)
		{
			iSum[0] = iNum[0];
		}
		else
		{
			iSum[i] = iSum[i-1]+iNum[i];
		}
	}


	srand(GetTickCount());
	int iTmp  = rand()*((rand()%100)+1);
	int iRand = (iTmp)%iSum[5];

	int iChouMa = 0;
	if (iRand < iSum[0])
	{
		iChouMa = 0;
	}
	else
	{
		for(int i=1; i<6; i++)
		{
			if (iRand>=iSum[i-1] && iRand<iSum[i])
			{
				iChouMa = i;
			}
		}
	}

	return iChouMa;
}