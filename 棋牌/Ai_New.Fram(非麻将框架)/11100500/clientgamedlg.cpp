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

//---------------------------------------------------------------------------------------
///��ʼ����������
void	CClientGameDlg::ResetGameData()
{
	KillAllTimer();
}
//��ʼ����Ϸ����
void	CClientGameDlg::InitGameData()
{
	KillAllTimer();
	LoadGameIni();
	m_iMyBetSum = 0;
}
//���������ļ�
void CClientGameDlg::LoadGameIni()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();///����·��
	CINIFile f( s +nid +"BBNNRobotSet.ini");
	CString key = TEXT("robot");
	m_iRobotBetSumLimit = f.GetKeyVal(key,"RobotBetSumLimit",100000);		//��������ע����
	m_iRobotMaxNtLimit = f.GetKeyVal(key,"RobotMaxNtLimit",8);				//�����������ׯ��������

	CString sKey;
	for (int i=0; i<6; i++)
	{
		sKey.Format("BetChouMaPro_%d",i);
		m_iBetChouMaPro[i] = f.GetKeyVal(key,sKey,100);
	}
	
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
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:		//�ȴ�ͬ��״̬
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
	case GS_FREE:
		{
			if (sizeof(TagGameStation_WaiteAgee) != uDataSize)
			{
				return false;
			}
			TagGameStation_WaiteAgee *pGameStation = (TagGameStation_WaiteAgee *)pStationData;
			if (NULL == pGameStation)
			{
				return false;
			}
			SetStationParameter(GS_WAIT_AGREE);
			m_byGameStation	= pGameStation->byGameStaion;	//��Ϸ״̬

			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;	///��עʱ��
			m_iKaiPaiTime	= pGameStation->iKaiPaiTime;	///����ʱ��
			m_iFreeTime		= pGameStation->iFreeTime;		///����ʱ��	
	
			m_byNtStation = pGameStation->iNowNtStation;	///��ǰׯ�ҵ�λ��
		
			m_i64MaxNote	= pGameStation->i64MaxNote;		///������Ϸ�����ע��
			m_i64ApplyNtLimite	= pGameStation->i64ShangZhuangLimit;///��ׯ��Ҫ�����ٽ��
			// ���������ֵ
			memcpy(m_i64ChoumaValue,pGameStation->i64ChoumaValue,sizeof(m_i64ChoumaValue));

			if (-1 == m_byNtStation)
			{
				//������ׯ��ʱ��
				srand(GetTickCount()+GetMeUserInfo()->bDeskStation);
				int iTmpTime = ((rand()%5)+2);
				SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_APPLY);
			}
			else
			{
				int iRand = rand()%10;
				if (iRand<5)
				{
					//������ׯ��ʱ��
					srand(GetTickCount()+GetMeUserInfo()->bDeskStation);
					int iTmpTime = ((rand()%5)+2);
					SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_APPLY);
				}
			}
			break;
		}
	case GS_BET://��ע״̬
		{
			if (sizeof(TagGameStation_Bet) != uDataSize)
			{
				return false;
			}
			
			TagGameStation_Bet * pGameStation=(TagGameStation_Bet *)pStationData;
			if (NULL == pGameStation)
			{
				return false;
			}

			m_byGameStation	= pGameStation->byGameStaion;	//��Ϸ״̬

			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;	///��עʱ��
			m_iKaiPaiTime	= pGameStation->iKaiPaiTime;	///����ʱ��
			m_iFreeTime		= pGameStation->iFreeTime;		///����ʱ��	

			m_byNtStation = pGameStation->iNowNtStation;	///��ǰׯ�ҵ�λ��

			m_i64MaxNote	= pGameStation->i64MaxNote;		///������Ϸ�����ע��
			m_i64ApplyNtLimite	= pGameStation->i64ShangZhuangLimit;///��ׯ��Ҫ�����ٽ��
			// ���������ֵ
			memcpy(m_i64ChoumaValue,pGameStation->i64ChoumaValue,sizeof(m_i64ChoumaValue));

			//������ע��ʱ��
			if (GetMeUserInfo()->bDeskStation != m_byNtStation)
			{
				//������ע��ʱ��
				int iTime = rand()%3+1;
				SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, TIME_BET);
			}
			
			break;
		}
	case GS_SEND_CARD:	//��Ϸ��ע�׶�
		{
			if (sizeof(TagGameStation_OpenCard) != uDataSize)
			{
				return false;
			}
			TagGameStation_OpenCard * pGameStation=(TagGameStation_OpenCard *)pStationData;
			if (NULL == pGameStation)
			{
				return false;
			}
			m_byGameStation	= pGameStation->byGameStaion;	//��Ϸ״̬

			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;	///��עʱ��
			m_iKaiPaiTime	= pGameStation->iKaiPaiTime;	///����ʱ��
			m_iFreeTime		= pGameStation->iFreeTime;		///����ʱ��	

			m_byNtStation = pGameStation->iNowNtStation;	///��ǰׯ�ҵ�λ��

			m_i64MaxNote	= pGameStation->i64MaxNote;		///������Ϸ�����ע��
			m_i64ApplyNtLimite	= pGameStation->i64ShangZhuangLimit;///��ׯ��Ҫ�����ٽ��
			// ���������ֵ
			memcpy(m_i64ChoumaValue,pGameStation->i64ChoumaValue,sizeof(m_i64ChoumaValue));

			break;
		}
	case GS_PLAY_GAME://��Ϸ����״̬
		{
			if(sizeof(TagGameStation_ShowResult) == uDataSize)
			{
				return false;
			}
			TagGameStation_ShowResult * pGameStation=(TagGameStation_ShowResult *)pStationData;
			if (NULL == pGameStation)
			{
				return false;
			}
			
			m_byGameStation	= pGameStation->byGameStaion;	//��Ϸ״̬

			m_iXiaZhuTime	= pGameStation->iXiaZhuTime;	///��עʱ��
			m_iKaiPaiTime	= pGameStation->iKaiPaiTime;	///����ʱ��
			m_iFreeTime		= pGameStation->iFreeTime;		///����ʱ��	

			m_byNtStation = pGameStation->iNowNtStation;	///��ǰׯ�ҵ�λ��

			m_i64MaxNote	= pGameStation->i64MaxNote;		///������Ϸ�����ע��
			m_i64ApplyNtLimite	= pGameStation->i64ShangZhuangLimit;///��ׯ��Ҫ�����ٽ��
			// ���������ֵ
			memcpy(m_i64ChoumaValue,pGameStation->i64ChoumaValue,sizeof(m_i64ChoumaValue));

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
		case ASS_GAME_BEGIN:      //��Ϸ��ʼ������ע
			{
				OnHandleBet(pNetData,uDataSize);
				break;
			}
		case ASS_KAI_PAI:					//������Ϣ
		case ASS_RESULT:					//��Ϸ����
			{
				KillGameTimer(TIME_BET);
				m_byGameStation = GS_PLAY_GAME;
				break;
			}
		case ASS_FREE_TIME:				//�ȴ���һ����Ϸ��ʼ
			{
				OnHandleFree(pNetData,uDataSize);
				break;
			}
		case ASS_WU_ZHUANG://�յ���ׯ�ȴ���Ϣ
			{
				if (sizeof(DENG_DAI) > uDataSize)
				{
					return false;
				}
				DENG_DAI *pWait = (DENG_DAI *)pNetData;
				if (NULL == pWait)
				{
					return false;
				}
				
				SetStationParameter(GS_WAIT_AGREE);//��Ϸ��Ϊ��ʼ״̬
				m_byGameStation = GS_WAIT_AGREE;
				for(int i=0;i<=PLAY_COUNT;i++)
				{
					m_byWaitNtList[i] = pWait->dzhuangstation[i][0];
				}
				m_byNtStation		= -1;
				//������ׯ��ʱ��
				srand(GetTickCount()+GetMeUserInfo()->bDeskStation);
				int iTmpTime = ((rand()%5)+2);
				SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_APPLY);
	
				return true;
			}
		case ASS_XIA_ZHU:					//��ע�����Ϣ
			{
				if(sizeof(TagXiaZhu) != uDataSize)
				{
					return false;
				}							

				TagXiaZhu *pXiaZhuResult = (TagXiaZhu *)pNetData;
				if (NULL == pXiaZhuResult)
				{
					return false;
				}

				if(pXiaZhuResult->byStation == GetMeUserInfo()->bDeskStation)
				{
					//�Լ�����ע
					m_iMyBetSum = pXiaZhuResult->iUserXiaZhu[MAX_NOTE_COUNT];
				}
			

				break;
			}
		case ASS_SHANG_ZHUANG:      // ��ׯ��Ϣ
			{
				if (sizeof(SHANG_ZHUANG)!= uDataSize)
				{
					return false;
				}
				SHANG_ZHUANG *pSZ = (SHANG_ZHUANG *)pNetData;
				if (NULL == pSZ)
				{
					return  false;
				}
				for(int i=0;i<=PLAY_COUNT;i++)
				{
					m_byWaitNtList[i] = pSZ->dzhuangstation[i][0];
				}

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
/*-----------------------------------------------------------------------------*/
//��ʼ��Ϸ
void		CClientGameDlg::OnHandleBet(void * buffer,int nLen)
{
	if (sizeof(BEGIN_DATA) != nLen)
	{
		return;
	}
	SetStationParameter(GS_BET);				//��Ϸ��Ϊ��ע״̬
	m_byGameStation = GS_BET;
	BEGIN_DATA *pBegin = (BEGIN_DATA *)buffer;
	if (NULL == pBegin)
	{
		return;
	}
	//�Լ�������ע���
	m_iMyBetSum = 0;

	m_iXiaZhuTime		= pBegin->iXiaZhuTime;		//��עʱ��	
	m_iKaiPaiTime		= pBegin->iKaiPaiTime;		//����ʱ��
	m_iFreeTime			= pBegin->iFreeTime;		//����ʱ��
	m_byNtStation		= pBegin->iNowNtStation;	///ȡ��ׯ�ҵ�λ��
	

	//��ׯ�б�
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_byWaitNtList[i] = pBegin->iNTlist[i][0];
	}
	if (GetMeUserInfo()->bDeskStation != m_byNtStation)
	{
		//������ע��ʱ��
		int iTime = rand()%3+1;
		SetGameTimer(GetMeUserInfo()->bDeskStation, iTime, TIME_BET);
	}
}
/*---------------------------------------------------------------------------*/
void	CClientGameDlg::OnHandleFree(void * buffer,int nLen)
{
	if(sizeof(DENG_DAI)!=nLen)
	{
		return ;
	}
	DENG_DAI *pWait = (DENG_DAI *)buffer;
	if (NULL == pWait)
	{
		return;
	}
	m_byGameStation = GS_WAIT_NEXT;
	SetStationParameter(GS_WAIT_NEXT);				//����Ϊ�ȴ�״̬

	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_byWaitNtList[i] = pWait->dzhuangstation[i][0];
	}
	m_byNtStation		= pWait->iZhuang;	///ȡ��ׯ�ҵ�λ��

	int iRand = rand()%10;
	if (iRand<8)
	{
		//�ж��ܷ�������ׯ
		JudgeApplyNt(true);
	}
	else
	{
		//�ж��ܷ�������ׯ
		JudgeApplyNt(false);
	}
	

}


//�ж��Ƿ�������ׯ
void	CClientGameDlg::JudgeApplyNt(bool bFlag)
{
	
	//������ׯ
	if (bFlag)
	{
		//����Ƿ�����ׯ�б���
		bool bCanApply = true;
		if (GetMeUserInfo()->bDeskStation == m_byNtStation)
		{
			bCanApply = false;
		}
		for(int i=0; i<m_byWaitNtList[PLAY_COUNT]; i++)
		{
			if (GetMeUserInfo()->bDeskStation == m_byWaitNtList[i])
			{
				bCanApply = false;
				break;
			}
		}
		//�鿴�ж��ٸ���������ׯ
		if((m_byWaitNtList[PLAY_COUNT]>=m_iRobotMaxNtLimit))//������ׯ
		{
			bCanApply = false;
		}
		if (bCanApply)
		{
			//������ׯ��ʱ��
			srand(GetTickCount()+GetMeUserInfo()->bDeskStation);
			int iTmpTime = ((rand()%5)+2);
			SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_APPLY);
		}
	}
	else
	{
		
		bool bCanApply = false;
		//����Ƿ�����ׯ�б���,����ׯ�б��в���������ׯ
		for(int i=0; i<m_byWaitNtList[PLAY_COUNT]; i++)
		{
			if (GetMeUserInfo()->bDeskStation == m_byWaitNtList[i])
			{
				bCanApply = true;
				break;
			}
		}
		if (bCanApply)
		{
			srand(GetTickCount()+GetMeUserInfo()->bDeskStation);
			//������ׯ��ʱ��
			int iTmpTime = ((rand()%5)+5);
			SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_LEAVE);  
		}
	}
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
	case TIME_BET:	//��ע��ʱ��
		{
			KillGameTimer(TIME_BET);
			RobortBet();
			//�ظ���ע
			if (m_byGameStation	== GS_BET)
			{
				if (m_iMyBetSum < m_iRobotBetSumLimit)
				{
					int iTmpTime = (rand()%3+1);
					SetGameTimer(GetMeUserInfo()->bDeskStation, iTmpTime, TIME_BET);
				}
			}
			return true;
		}
	case TIME_APPLY:	//������ׯ
		{
			KillGameTimer(TIME_APPLY);
			//������������ׯ
			RobortApplyNt();
			return true;
		}
	case TIME_LEAVE://������ׯ
		{
			KillGameTimer(TIME_LEAVE);
			SHANG_ZHUANG TShangZhuang;
			TShangZhuang.shang=false;
			TShangZhuang.station = GetMeUserInfo()->bDeskStation;
			SendGameData(&TShangZhuang, sizeof(TShangZhuang) ,MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
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
	srand(GetTickCount()+GetMeUserInfo()->bDeskStation);
	int iTmpRand = rand()%100;
	//50%�ĸ���������ׯ
	if (iTmpRand < 50)
	{
		SHANG_ZHUANG TShangZhuang;
		TShangZhuang.shang=true;
		TShangZhuang.station = GetMeUserInfo()->bDeskStation;
		SendGameData(&TShangZhuang, sizeof(TShangZhuang) ,MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
	}
	return;
}

//------------------------------------------------------------------------------------------------------------------
//��������ע
void	CClientGameDlg::RobortBet()
{
	
	if (m_byGameStation != GS_BET)
	{
		return;
	}
	//���һ����ע����
	BYTE byBetArea=255;
	byBetArea = (rand()+GetMeUserInfo()->bDeskStation)%4;
	
	
	//m_pGameInfo->pMeUserInfo->GameUserInfo.i64Money;
	//���һ����������
	
	//��ע��������ܺ�
	int iTotalSum = 0;
	int iSum[6] = {0};
	for(int i=0; i<6;i++)
	{
		iTotalSum += m_iBetChouMaPro[i];
		iSum[i] = iTotalSum;
	}

	int iRand = rand()%iTotalSum;

	BYTE byChouMaType = 0;
	if (iRand >= 0 && iRand <iSum[0])
	{
		byChouMaType = 0;
	}
	else if (iRand >= iSum[0] && iRand <iSum[1])	
	{
		byChouMaType = 1;
	}
	else if (iRand >= iSum[1] && iRand <iSum[2])	
	{
		byChouMaType = 2;
	}
	else if (iRand >= iSum[2] && iRand <iSum[3])	
	{
		byChouMaType = 3;
	}
	else if (iRand >= iSum[3] && iRand < iSum[4])	
	{
		byChouMaType = 4;
	}
	else if (iRand >= iSum[4] && iRand <= iSum[5]) 
	{
		byChouMaType = 5;
	}

	if (m_iMyBetSum + m_i64ChoumaValue[byChouMaType] > m_iRobotBetSumLimit)
	{
		return;
	}

	TagXiaZhu  TXiaZhu;
	TXiaZhu.byStation	= GetMeUserInfo()->bDeskStation;
	TXiaZhu.iChouMaType = byChouMaType;	//�õ�����ֵ
	TXiaZhu.iBetArea	= byBetArea;			//��������
	TXiaZhu.bIsAi		= true;
	
	SendGameData(&TXiaZhu, sizeof(TXiaZhu) ,MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
	
}


