#include "StdAfx.h"
#include "ClientGamedlg.h"

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	m_iVersion = DEV_HEIGHT_VERSION;
	m_iVersion2 = DEV_LOW_VERSION;
	m_iThinkTime = 20;
	m_iRoomBasePoint = 0;
	m_iRunPublish = 0;
	m_iLimitNote = 0;
	m_iBaseNote = 0;
	m_zhunagStation = -1;
	m_iMyZhu = 0;
	m_iGameBeen = 0;
	m_iZhuangMaxCount = 0;
	m_iMaxDownMoney = 0;
	m_iTempMoney = 1; 
	//m_bIsFinish=false;
	m_iMyXiaZhuCount = 0;		//������ע����
	m_iMyNtCount = 0;
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
	m_iTempMoney = 1; 
	for(int i = 0; i < m_nPowerOfGold; i++)
	{
		m_iTempMoney *= 10;
	}

	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:		//�ȴ�ͬ��״̬
		{	
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			// ��������
			m_iThinkTime = pGameStation->iThinkTime;

			m_iBeginTime = pGameStation->iBeginTime;
			m_iXiaZhuTime = pGameStation->iXiaZhuTime;        // ��עʱ��
			m_iKaipaiTime = pGameStation->iKaipaiTime;        // ����ʱ��	

			SetStationParameter(GS_WAIT_ARGEE);

			SetGameTimer(GetMeUserInfo()->bDeskStation , OnGetRandNum(GetMeUserInfo()->bDeskStation,m_iBeginTime,1),ID_BEGIN_TIME) ; 

			return true;
		}
	case GS_XIAZHU:			//��ע״̬
		{
			SetStationParameter(GS_XIAZHU);
			return true;
		}

	case GS_PLAY_GAME:		//��Ϸ������
		{
			SetStationParameter(GS_PLAY_GAME);
			return true;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			SetStationParameter(GS_WAIT_NEXT);
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
		case ASS_GM_AGREE_GAME:
			{		
				if(uDataSize!=sizeof(MSG_GR_R_UserAgree) )
				{
					return false;
				}	
				return true ;
			}
		case ASS_GAME_BEGIN:		//��Ϸ��ʼ
			{
				if (sizeof(GameBeginStruct) != uDataSize)
				{
					return false;
				}
				// ��Ϸ��ʼ
				OnGameBegin((GameBeginStruct *)pNetData);

				return true;
			}
		case  ASS_SEND_NOTE_MSG:
			{
				int iCount = m_iNtMaxCount;


				if (GetMeUserInfo()->bDeskStation == m_zhunagStation)
				{
					m_iMyNtCount++;			
				}
				else
				{
					m_iMyNtCount = 0;
				} 	

				if (m_iMyNtCount >= iCount)
				{
					// ������ׯ
					if (GetMeUserInfo()->bDeskStation == m_zhunagStation)
					{
						SetGameTimer(GetMeUserInfo()->bDeskStation, 1, TIME_XZ_TIME);
						m_iMyNtCount = 0;
					}
				}
				if (m_zhunagStation>=0&&GetMeUserInfo()->bDeskStation!=m_zhunagStation)
				{
					srand((unsigned)GetTickCount() + GetMeUserInfo()->bDeskStation);
					SetGameTimer(GetMeUserInfo()->bDeskStation, rand()%3+2, TIME_XIAZHU);
				}	

				return true ; 				

			}
		case EZP_MSG_APPLYZHUANG:  //�����������ׯ
			{
				if (sizeof(EZP_APPLY_ZHUANG) != uDataSize)
				{
					return false;
				}
				OnUserApplyZhuang((EZP_APPLY_ZHUANG *)pNetData);

				return true;
			}
		case EZP_MSG_XIAZHU:       //�����ע
			{
				if (sizeof(EZP_XIAZHU_INFO) != uDataSize)
				{
					return false;
				}

				OnUserXiaZhuInfo((EZP_XIAZHU_INFO *)pNetData);

				return true;
			}
		case EZP_MSG_SENDCARD:     //����
			{
				if (sizeof(GameFaPaiData) != uDataSize)
				{
					return false;
				}
				KillAllTimer();				
				return true;
			}
		case  ASS_AHEAD_END:
			{
				srand((unsigned) time(NULL) + GetMeUserInfo()->bDeskStation) ;

				int iRet = rand()%6+GetMeUserInfo()->bDeskStation%3+2;

				if (GetMeUserInfo()->bDeskStation != m_zhunagStation&&iRet<=6&iRet>=2)
				{
					SetGameTimer(GetMeUserInfo()->bDeskStation,iRet,TIME_SZ_TIME);
				}
	
			    SetGameTimer(GetMeUserInfo()->bDeskStation , OnGetRandNum(GetMeUserInfo()->bDeskStation,m_iBeginTime,1),ID_BEGIN_TIME) ; 

				return true ; 
			}
		case ASS_CONTINUE_END:	//��Ϸ����
		case ASS_NO_CONTINUE_END:
			{
				if (sizeof(GameEndData) != uDataSize)
				{
					return false;
				}

				SetStationParameter(GS_WAIT_NEXT);
				//m_bIsFinish=true;

				GameEndData * pGameEnd = (GameEndData *)pNetData;


				if (pGameEnd->bAutoZhuangXZ || pGameEnd->bZhuangXZ)
				{
					// ׯ����ׯ��, �����˿�ʼ��ʱ������ׯ
					SetGameTimer(GetMeUserInfo()->bDeskStation,OnGetRandNum(GetMeUserInfo()->bDeskStation ,10, 3), TIME_WAIT);
				}
				if (pGameEnd->bRobortCanApply)
				{
					int iRet = rand()%6+GetMeUserInfo()->bDeskStation%3+2;
					CString CSt;
					CSt.Format("wysout::iRet==%d",iRet);
					OutputDebugString(CSt);

					if (GetMeUserInfo()->bDeskStation != m_zhunagStation&&iRet<=6&iRet>=2)
					{
						SetGameTimer(GetMeUserInfo()->bDeskStation,iRet,TIME_SZ_TIME);
					}
				}

				SetGameTimer(GetMeUserInfo()->bDeskStation , OnGetRandNum(GetMeUserInfo()->bDeskStation,m_iBeginTime,1),ID_BEGIN_TIME) ; 


				return true;
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

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillGameTimer(ID_BEGIN_TIME);
	KillGameTimer(TIME_EMPTY);
	KillGameTimer(TIME_XIAZHU);
	KillGameTimer(TIME_KAIPAI);
	KillGameTimer(TIME_WAIT);

	return;
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

// ��Ϸ��ʼ
void CClientGameDlg::OnGameBegin(GameBeginStruct * pGameBegin)
{	
	KillGameTimer(TIME_SZ_TIME);
	KillAllTimer();
	SetStationParameter(GS_XIAZHU);
	//m_bIsFinish=false;
	m_iMyZhu = 0;
	m_zhunagStation = pGameBegin->bZhuangStation;                               // ����ׯ��
	m_iLimitNote = pGameBegin->iLimitNote;										// ��ע����
	m_iBaseNote = pGameBegin->iBaseNote;										// ��ע
	m_iGameBeen = pGameBegin->iBeenPlayGame;

	m_iMyXiaZhuCount = 0;		//������ע����

	CINIFile iniFile(CINIFile::GetAppPath()+ "16REZPRobotSet.ini");

	//�����������ׯ����
	m_iNtMaxCount = iniFile.GetKeyVal("Set", "RobortSitZMax", 10);
	// ������������ׯ����
	m_iNtMinCount = iniFile.GetKeyVal("Set", "RobortSitZMin", 3);
	//ÿ����עΪ���Ͻ�ҵİٷּ�(0~100)

	m_iMaxDownMoney = iniFile.GetKeyVal("Money", "MaxDownMoney", 50000000);

}

// ���������ׯ����ׯ��Ϣ
void CClientGameDlg::OnUserApplyZhuang(EZP_APPLY_ZHUANG * pApplyZhuang)
{
}

// �����ע��Ϣ
void CClientGameDlg::OnUserXiaZhuInfo(EZP_XIAZHU_INFO * userXiazhu)
{
	if(NULL == userXiazhu ||NULL == GetMeUserInfo())
	{
		return ; 
	}

	if (GetMeUserInfo()->bDeskStation != m_zhunagStation)
	{
		if (GetMeUserInfo()->bDeskStation == userXiazhu->bDeskStation)
		{
			m_iMyZhu += userXiazhu->iMoney * m_iTempMoney;
		}
	}
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:
		{
			if(uTimeCount <= 0)
			{
				/*if(m_bIsFinish)
				{
					return true;
				}*/
				SendGameData(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME, 0);		
			}	
			int iRet = rand()%6+GetMeUserInfo()->bDeskStation%3+2;
			if (GetMeUserInfo()->bDeskStation != m_zhunagStation&&iRet<=6&iRet>=2)
			{
				SetGameTimer(GetMeUserInfo()->bDeskStation,iRet,TIME_SZ_TIME);
			}

			return  true ; 
		}		
	case TIME_WAIT:   // �ȴ���ʱ��
		{
			if (uTimeCount <= 0)
			{
				KillGameTimer(TIME_WAIT);

				EZP_APPLY_ZHUANG ApplyZhuang;
				ApplyZhuang.bDeskStation = GetMeUserInfo()->bDeskStation;
				ApplyZhuang.bFirst = FALSE;
				ApplyZhuang.bFlag = TRUE;
				ApplyZhuang.bIsRobot = TRUE;  // ������

				SendGameData(&ApplyZhuang, sizeof(ApplyZhuang), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
			}

			return true;
		}
	case TIME_SZ_TIME:
		{
			if (uTimeCount <= 0)
			{
				KillGameTimer(TIME_SZ_TIME);

				EZP_APPLY_ZHUANG ApplyZhuang;
				ApplyZhuang.bDeskStation = GetMeUserInfo()->bDeskStation;
				ApplyZhuang.bFirst = FALSE;
				ApplyZhuang.bFlag = TRUE;
				ApplyZhuang.bIsRobot = TRUE;  // ������

				SendGameData(&ApplyZhuang, sizeof(ApplyZhuang), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
			}
			return true;
		}
	case TIME_XIAZHU: //��ע��ʱ��
		{
			if(GetMeUserInfo()->bDeskStation == m_zhunagStation
				/*|| m_iMyZhu >= GetMeUserInfo()->dwMoney	*/			
				|| m_iMyZhu >= m_iMaxDownMoney
				|| GetMeUserInfo()->i64Money < 100)
			{
				KillGameTimer(TIME_XIAZHU);           //ֹͣ��ע��ʱ��

				return true;
			}

			if (uTimeCount <= 0)
			{
				KillGameTimer(TIME_XIAZHU);

				int iNoteType ;
				//�����ע
				OnSuiJiXiaZhu(iNoteType);

				srand((unsigned)GetTickCount() + GetMeUserInfo()->bDeskStation);

				int time = rand() % 3 + 1; 

				if(iNoteType>=6)
				{
					time+=3;
				}

				// ��������Ҫ�ظ���ע
				SetGameTimer(GetMeUserInfo()->bDeskStation, time, TIME_XIAZHU);
			}

			return true;
		}
	case TIME_XZ_TIME: //������ׯ��ʱ��
		{
			if (uTimeCount <= 0)
			{
				KillGameTimer(TIME_XZ_TIME);

				// ������Ϊׯ�ҵ�ʱ������������ׯ��
				EZP_APPLY_ZHUANG ApplyZhuang;
				ApplyZhuang.bDeskStation = GetMeUserInfo()->bDeskStation;
				ApplyZhuang.bFirst = FALSE;
				ApplyZhuang.bFlag = FALSE;
				ApplyZhuang.bGoto = FALSE;
				ApplyZhuang.bIsRobot = TRUE;  // ��ׯʱ����Ҫ���ͻ����˱�־

				SendGameData(&ApplyZhuang, sizeof(ApplyZhuang), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
			}

			return true;
		}
	}

	return true;
}

//�����ע
void CClientGameDlg::OnSuiJiXiaZhu(int  &iNotetype)
{
	srand((unsigned)GetTickCount() + GetMeUserInfo()->bDeskStation);

	int iMoney = 0;
	int iType      = rand()%7 ;		// 0-5 Ϊ��ע����


	int  iMoneyType = rand()%6+1;
	iNotetype = iMoneyType ;


	// ��ǰѡ��ĳ��� -1û��, 0��100, 1��1000��2��1W, 3��10W, 3: 100W 4��100W, 5��10000W
	if (iMoneyType >= 0 && iMoneyType < CHOUMA_KIND_COUNT)
	{
		iMoney = g_ChoumaValue[iMoneyType];

		// �ж���ע�Ƿ񳬹������ 
		if ((m_iMyZhu + iMoney) > m_iMaxDownMoney)
		{
			return ;
		}
	}
	else
	{
		return ;
	}

	// ������ע��Ϣ
	EZP_XIAZHU_INFO  xiazhu;
	xiazhu.bDeskStation = GetMeUserInfo()->bDeskStation;
	xiazhu.bType = iType;
	xiazhu.bMoneyType = iMoneyType;
	xiazhu.iMoney = iMoney/m_iTempMoney;
	xiazhu.bIsRobot = TRUE;   // �����˱�־
	SendGameData(&xiazhu, sizeof(xiazhu), MDM_GM_GAME_NOTIFY, EZP_MSG_XIAZHU, 0);
}
///��ȡ���ʱ��
int CClientGameDlg::OnGetRandNum(int iDeskStation,int iMaxNum,int iMinNum )
{
	//srand((unsigned)time(NULL)+GetTickCount()) ;
	//int GetRandNum = rand()%(iMaxNum  - iMinNum) + iMinNum;

	//if(GetRandNum-iDeskStation>iMinNum)
	//{
	//	GetRandNum -= iDeskStation ; 
	//}
	//else if(GetRandNum +iDeskStation<iMaxNum)
	//{
	//	GetRandNum += iDeskStation ; 
	//}
	//else 
	//{
	//	GetRandNum  = (GetRandNum + iDeskStation)%iMaxNum + iMinNum;
	//}
	//return GetRandNum;

	if(iMaxNum >20||iMaxNum <1)
	{
		iMaxNum = 10 ;
	}
	srand((unsigned) time (NULL) + GetMeUserInfo()->bDeskStation ) ;
	
	int iRandTime =  rand()% iMaxNum + 1 ;

	return iRandTime ;
}
