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
	m_iMyDeskStation = 255;		///����Լ�����Ϸλ��

	m_bIsPlaying	 = false;	//��Ϸ�Ƿ��Ѿ���ʼ
	m_byGameStation	 = GS_WAIT_ARGEE;         ///��ǰ����Ϸ״̬
	LoadRobotSet();
	m_iTimeContrl = 0;
} 

//��������
CClientGameDlg::~CClientGameDlg()
{

}

//---------------------------------------------------------------------------------------
///��ʼ����������
void	CClientGameDlg::ResetGameData()
{
	m_bIsPlaying	 = false;	//��Ϸ�Ƿ��Ѿ���ʼ
	
	KillAllTimer();
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClass::OnInitDialog();

	return TRUE;
}
/*-------------------------------------------------------------------*/
void	CClientGameDlg::LoadRobotSet()
{

	CINIFile iniFile(CINIFile::GetAppPath()+"DSNHRobotSet.ini");

	m_i64MinAddScore = iniFile.GetKeyVal("RobotSet", "MinAddScore",(__int64)1000000);
	m_i64MaxAddScore = iniFile.GetKeyVal("RobotSet", "MaxAddScore",(__int64)1000000000);
	m_iAutoAddScoreSet = iniFile.GetKeyVal("RobotSet", "AutoAddScore",8888888);
	m_iMinFire	= iniFile.GetKeyVal("RobotSet", "MinFire",1000);				//��С�ڻ���
	m_iMaxFire	= iniFile.GetKeyVal("RobotSet", "MaxFire",100000);				//����ڻ���


	m_iMinShotTime = iniFile.GetKeyVal("RobotSet", "MinShotTime",100);;			//��췢����
	m_iMaxShotTime = iniFile.GetKeyVal("RobotSet", "MaxShotTime",1000);			//����������

}
//------------------------------------------------------------------------------------------------------------------
//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	//��ȡ�Լ���λ��
	m_iMyDeskStation = GetMeUserInfo()->bDeskStation;
	switch (GetStationParameter())
	{

	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
	case GS_WAIT_NEXT:			//�ȴ����̿�ʼ
		{

			if(uDataSize != sizeof(GameStation_Base))
			{
				return false;
			}
		
			GameStation_Base *pGameStation = (GameStation_Base *) pStationData;
			if(NULL == pGameStation)
			{
				return false;
			}
		
			//������Ϸ����
			memcpy(&m_GameData, pGameStation ,sizeof(m_GameData));

			//�ڻ�λ
			m_GameScreen.SetCellScore(m_GameData.iFireBase);
			//��ҽ���
			m_GameScreen.UserComeInorLeave(SwitchChairID(m_iMyDeskStation),true);
			//������Ϣ
			CString sNickName = "";
			if(m_pUserInfo[m_iMyDeskStation])
			{
				sNickName.Format("%s",m_pUserInfo[m_iMyDeskStation]->GameUserInfo.nickName);
				m_GameScreen.SetMeInformation(SwitchChairID(m_iMyDeskStation),m_iMyDeskStation,sNickName,m_pUserInfo[m_iMyDeskStation]->GameUserInfo.i64Money);
				//�Լ����·�
				m_GameScreen.SetUserScore(SwitchChairID(m_iMyDeskStation),m_GameData.iUserScore[m_iMyDeskStation]);
				//�Լ��ڻ���
				m_GameScreen.SetBeiLv(SwitchChairID(m_iMyDeskStation),m_GameData.iUserFireBase[m_iMyDeskStation]);
			}
	

			SetStationParameter(GS_WAIT_ARGEE);

			
			m_i64RandAddScore = (rand()%(int)(m_i64MaxAddScore-m_i64MinAddScore)) + m_i64MinAddScore;
			
			m_iRandFire = (rand()%(m_iMaxFire-m_iMinFire)) + m_iMinFire;

			//���ڼ�ʱ��
			SetGameTimer(GetMeUserInfo()->bDeskStation, 300, TIME_ADD_FIRE);
			//SetGameTimer(GetMeUserInfo()->bDeskStation, 500, TIME_SHOT);

			//�ж��Ƿ�������
			bool	bHaveUser = false;
			for(int i=0; i<PLAY_COUNT; i++)
			{
				if(!m_pUserInfo[i])
				{
					continue;
				}

				if(0 == m_pUserInfo[i]->GameUserInfo.isVirtual)
				{
					bHaveUser = true;
					break;
				}
			}

			if(bHaveUser)
			{
				OutputDebugString("dwjrobot::������");
				m_iTimeContrl = 0;
			}
			else
			{
				OutputDebugString("dwjrobot::ȫ�ǻ�����");
				m_iTimeContrl = 0;
			}

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
		case ASS_S_USER_COME:		//��ҽ���/�뿪��Ϣ
			{
				OutputDebugString("dwjdsnh::ASS_S_USER_COME");
				if(uDataSize != sizeof(GameUserComeData))
				{
					return TRUE;
				}

				GameUserComeData *pGameUserCome = (GameUserComeData *)pNetData;
				if(NULL == pGameUserCome)
				{
					return TRUE;
				}
				//��ҽ���
				m_GameScreen.UserComeInorLeave(SwitchChairID(pGameUserCome->byDeskStation),pGameUserCome->bComeOrLeave);
				//��ҵ��Ϸ���
				m_GameScreen.SetUserScore(SwitchChairID(pGameUserCome->byDeskStation),pGameUserCome->iScore);
				//��ҵ��ڻ���
				m_GameScreen.SetBeiLv(SwitchChairID(pGameUserCome->byDeskStation),pGameUserCome->iFireBase);

				//�����˽���
				if(pGameUserCome->bComeOrLeave && !pGameUserCome->bIsRobot)
				{
					m_iTimeContrl = 0;
				}
				else if(!pGameUserCome->bComeOrLeave && !pGameUserCome->bIsRobot)
				{
					//�ж��Ƿ�������
					bool	bHaveUser = false;
					for(int i=0; i<PLAY_COUNT; i++)
					{
						if(pGameUserCome->byDeskStation)
						{
							continue;
						}
						if(!m_pUserInfo[i])
						{
							continue;
						}

						if(0 == m_pUserInfo[i]->GameUserInfo.isVirtual)
						{
							bHaveUser = true;
							break;
						}
					}

					if(bHaveUser)
					{
						m_iTimeContrl = 0;
					}
					else
					{
						m_iTimeContrl = 0;
					}

				}

				

				break;
			}
		case ASS_CHANGE_SCENCE:		//�л�����
			{
				if(uDataSize != sizeof(CMD_S_ChangeScene))
				{
					return TRUE;
				}
				CMD_S_ChangeScene *pChangeScene=(CMD_S_ChangeScene *)pNetData;
				if(NULL == pChangeScene)
				{
					return TRUE;
				}
				KillGameTimer(TIME_SHOT);
				int iTime = rand()%10 +1;
				SetGameTimer(GetMeUserInfo()->bDeskStation, iTime*1000, TIME_SHOT);
				break;
			}
		case ASS_S_BUY_BULLET:		//������·ֽ��
			{
				if(uDataSize != sizeof(CMD_S_BulletCount))
				{
					return TRUE;
				}

				CMD_S_BulletCount *pBulletCount = (CMD_S_BulletCount *)pNetData;
				if(NULL == pBulletCount)
				{
					return TRUE;
				}
				//����������·�
				m_GameScreen.SetUserScore(SwitchChairID(pBulletCount->byDeskStation),pBulletCount->i64UserScore);

				//������Լ����·� ��Ҫˢ���Լ��Ľ����
				if(pBulletCount->byDeskStation == m_iMyDeskStation)
				{
					m_GameScreen.SetBaseScore(pBulletCount->i64UserMoney);
				}
				break;
			}
		case ASS_S_USER_SHOT:		//��ҷ����ӵ���Ϣ
			{
				if(uDataSize != sizeof(CMD_S_UserShoot))
				{
					return TRUE;
				}

				CMD_S_UserShoot *pUserShoot = (CMD_S_UserShoot *)pNetData;
				if(NULL == pUserShoot)
				{
					return TRUE;
				}

				//���÷�����ҵķ���
				m_GameScreen.SetUserScore(SwitchChairID(pUserShoot->byStation),pUserShoot->i64UserScore);
				break;
			}
		case ASS_S_USER_SHOT_ERROR:	//�ӵ��������
			{
				if(uDataSize != sizeof(CMD_S_UserShootError))
				{
					return TRUE;
				}

				CMD_S_UserShootError *pUserShootError = (CMD_S_UserShootError *)pNetData;
				if(NULL == pUserShootError)
				{
					return TRUE;
				}

				//�����ڵ�
				if(pUserShootError->byStation == m_iMyDeskStation)
				{
					//���÷�����ҵķ���
					m_GameScreen.SetUserScore(SwitchChairID(pUserShootError->byStation),pUserShootError->i64UserScore);
					m_GameScreen.SetBeiLv(SwitchChairID(pUserShootError->byStation),pUserShootError->iFireBase);
				}
				break;
			}
		case ASS_S_ADD_FIRE:		//��ҼӼ��ڻ���
			{
				if(uDataSize != sizeof(CMD_S_AddFireBase))
				{
					return TRUE;
				}

				CMD_S_AddFireBase *pAddFire = (CMD_S_AddFireBase *)pNetData;
				if(NULL == pAddFire)
				{
					return TRUE;
				}

				m_GameScreen.SetBeiLv(SwitchChairID(pAddFire->byDeskStation),pAddFire->iUserFireBase);
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
//------------------------------------------------------------------------------------------------------------------

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
void	CClientGameDlg::KillAllTimer()
{

	KillGameTimer(TIME_ADD_FIRE);
	KillGameTimer(TIME_ADD_SCORE);
	KillGameTimer(TIME_SHOT);

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

/*--------------------------------------------------------------------------------------------*/
///ԭ�ȵ���Ϊ��λ�����Գ�����ע���ٶȣ�����д��Ϊ��Ϊ��λ
bool CClientGameDlg::SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUserMilliSecond)
{
	int iTimeType = 1000 ; 

	if(m_pGameRoom != NULL)
	{
		m_pGameRoom->SendMessage(IDM_SET_GAME_TIME,(WPARAM)uTimeID,(LPARAM)uTimeCount/**iTimeType*/);
	}
	return true;
}
/*--------------------------------------------------------------------------------------------*/
//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case TIME_ADD_FIRE:	//����
		{
			CMD_C_AddFireBase TAddFire;
			TAddFire.bAdd = true;
			TAddFire.byDeskStation = m_iMyDeskStation;
			SendGameData(&TAddFire,sizeof(TAddFire),MDM_GM_GAME_NOTIFY,ASS_C_ADD_FIRE,0);

			if(m_GameScreen.m_BeiLv[SwitchChairID(m_iMyDeskStation)] >= m_iRandFire)
			{
				KillGameTimer(TIME_ADD_FIRE);
				SetGameTimer(GetMeUserInfo()->bDeskStation, 500, TIME_ADD_SCORE);
			}
			return true;
		}
	case TIME_ADD_SCORE:	//�Ϸ�
		{
			KillGameTimer(TIME_ADD_SCORE);
			//SetGameTimer(GetMeUserInfo()->bDeskStation, 30000, TIME_ADD_SCORE);
			CMD_C_BuyBullet  TBuyBullet;
			TBuyBullet.bAddOrMove = true;
			TBuyBullet.iScore = m_GameScreen.m_BeiLv[SwitchChairID(m_iMyDeskStation)];
			SendGameData(&TBuyBullet,sizeof(TBuyBullet),MDM_GM_GAME_NOTIFY,ASS_C_BUY_BULLET,0);

			//���������ʱ��
			int iTime = rand()%(m_iMaxShotTime-m_iMinShotTime) + m_iMinShotTime;
			SetGameTimer(GetMeUserInfo()->bDeskStation, iTime+m_iTimeContrl, TIME_SHOT);
			return true;

		}
	case TIME_SHOT:
		{
			KillGameTimer(TIME_SHOT);
		
			int iTime = rand()%(m_iMaxShotTime-m_iMinShotTime) + m_iMinShotTime;
			SetGameTimer(GetMeUserInfo()->bDeskStation, iTime+m_iTimeContrl, TIME_SHOT);

			if(m_GameScreen.m_UserScore[SwitchChairID(m_iMyDeskStation)] >= m_GameScreen.m_BeiLv[SwitchChairID(m_iMyDeskStation)])
			{
				CMD_C_UserShoot TUserShoot;
				TUserShoot.fAngle=(float)(rand()%360);
				SendGameData(&TUserShoot,sizeof(TUserShoot),MDM_GM_GAME_NOTIFY,ASS_C_USER_SHOT,0);
			}
			
			//���ڶ��ٷ־��Զ��Ϸ�
			if(m_GameScreen.m_UserScore[SwitchChairID(m_iMyDeskStation)] < m_iAutoAddScoreSet && m_GameScreen.m_i64MyMoney > (__int64)m_GameScreen.m_BeiLv[SwitchChairID(m_iMyDeskStation)])
			{
				CMD_C_BuyBullet  TBuyBullet;
				TBuyBullet.bAddOrMove = true;
				TBuyBullet.iScore = m_GameScreen.m_BeiLv[SwitchChairID(m_iMyDeskStation)];
				SendGameData(&TBuyBullet,sizeof(TBuyBullet),MDM_GM_GAME_NOTIFY,ASS_C_BUY_BULLET,0);

			}
			
			return true;
		}
	
	}

	return true;
}
/*--------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
//ת����λ
int		CClientGameDlg::SwitchChairID(int m_Currchairid)
{
	int changChairID = m_Currchairid;
	switch(GetMeUserInfo()->bDeskStation)
	{
	case 0:
	case 1:
	case 2:
		{
			switch(m_Currchairid)
			{
			case 0:return 0;
			case 1:return 1;
			case 2:return 2;
			case 3:return 3;
			case 4:return 4;
			case 5:return 5;
			}

		}
	case 3:
	case 4:
	case 5:
		{
			switch(m_Currchairid)
			{
			case 0:return 3;
			case 1:return 4;
			case 2:return 5;
			case 3:return 0;
			case 4:return 1;
			case 5:return 2;
			}

		}
	}
	return changChairID;
}
/*-------------------------------------------------------------------*/
