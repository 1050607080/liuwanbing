#include "StdAfx.h"
#include "ClientGameDlg.h"

#define ASSERT_(A) if (A) { __asm{int 3}  }

// ��Ϣӳ��
BEGIN_MESSAGE_MAP(CClientGameDlg,CLoveSendClass)
	ON_MESSAGE(WM_USER_XIA_ZHU, OnXiaZhu)           // �����ע
	ON_MESSAGE(WM_USER_TEST, OnTest)           // ������Ϣ
	ON_WM_TIMER()
	//ON_WM_TIMER()
END_MESSAGE_MAP()

//д��־ add by wlr 20090806
void CClientGameDlg::WriteInfo(TCHAR * pch)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString cStr;
	cStr.Format(_T("%d-%d-%d %02d:%02d:%02d ��Ϸ���id=%d,%s"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,st.wSecond,
		GetMeUserInfo()->dwUserID, pch);
	WriteStr(cStr.GetBuffer());
	return;
}
//end of д��־ add by wlr 20090806
void  CClientGameDlg::SrvTestlog(TCHAR *pMessage)
{
	try
	{
		//CString  tempstr = CINIFile::GetAppPath();
		//SYSTEMTIME st;
		//GetLocalTime(&st);
		//TCHAR FileName[256];
		//wsprintf(FileName, TEXT("%s�ټ��ֻ�����-test.txt"),tempstr,st.wYear,st.wMonth,st.wDay,st.wHour);
		//CFile syslog;
		//syslog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite);
		//TCHAR szDate[256];
		//wsprintf(szDate, TEXT("%d-%d-%d %d-%d-%d.%d\t"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		//syslog.SeekToEnd();
		//syslog.Write(szDate,lstrlen(szDate));
		//syslog.SeekToEnd();
		//syslog.Write(pMessage,lstrlen(pMessage));
		//TCHAR nextLine[] = TEXT("\r\n");
		//syslog.SeekToEnd();
		//syslog.Write(nextLine,lstrlen(nextLine));
		//syslog.Close();
	}
	catch(CException *e)
	{
		e->Delete();
		return ; 
	}
}


CClientGameDlg::CClientGameDlg(void):CLoveSendClass(IDD_GAME_FRAME)
{
	 //��עȨ��
	memset(m_probability,0,sizeof(m_probability));

	m_byHighVersion		= DEV_HIGH_VERSION; // ��Ϸ�汾
	m_byLowVersion		= DEV_LOW_VERSION;  // ��Ϸ�汾
	m_bCanXiaZhu		= false ;			// �ܷ���ע��
	m_iNowNtStation		= -1;				// ��ǰׯ�ҵ�λ��
	m_iMeZhu			= 0;				// ��ǰ�����˱����µ�ע
	m_iZnum				= 0;				// ׯ�Ҿ���
	m_iXnum				= 0;				// ���о���
	m_iZhuangBaShu		= 0;				// ��ׯ���پ�
	m_iShangZhuangLimit	= 0;				// ��ׯ��Ҫ�Ľ����
	memset(m_iAIMoney,0,sizeof(m_iAIMoney));//��������ÿ�������µ�ע
	m_iMaxZhuang		= 1;				// ׯ�ҽ��м��Ѿ���ׯ
	m_iBaseNum			= 1;				// ���������ֻ��������Я����ҵļ���֮һ
	m_nPowerOfGold      = 0;//�Ŵ���
	///��ǰ����������µ���ע
	m_iMaxNote          = 0;	
	///��ǰ��ע
	m_iMeZongZhu		= 0;	

	m_byZTWangPct	= 0;				//ׯ������ע����(��ȷ��ǧ��֮һ) 
	m_byZDZiPct		= 0;				//ׯ������ע����(��ȷ��ǧ��֮һ) 
	m_byXTWangPct	= 0;				//��������ע����(��ȷ��ǧ��֮һ) 
	m_byXDZiPct		= 0;				//�ж�����ע����(��ȷ��ǧ��֮һ) 
	m_byHePct			= 0;				//����ע����	(��ȷ��ǧ��֮һ) 
	m_byTDHePct			= 0;				//ͬ�����ע����(��ȷ��ǧ��֮һ) 

	m_iRobortSitCount = 0;
	m_bPlayerInList = FALSE;

	m_iRobortSitZMax = 0;
	m_iRobortSitZMin = 0;	
	//m_iRobotNotePercent = 0;
	m_bUse500WJeton = false ; 

	m_bUseMilliSecond = true ; 

	m_iMilliSecond = 200 ; 

	m_bSZSwitch = false;
	m_bXZSwitch = false;

	m_iMaxApplyNtMoney = 0;
	m_iMinApplyNtMoney = 0;

	m_iWinDownMoney	 = 0;
	m_iLostDownMoney = 0;

	m_iCanNoteValue = 0;
	m_iNowNtStation=-1;//ׯ��λ��
	//���������ļ�
	LoadIniConfig();

	
}

CClientGameDlg::~CClientGameDlg(void)
{
}

BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClass::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	switch (GetStationParameter())
	{          
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:	//�ȴ�ͬ��״̬
		{
			CHONG_LIAN *pData = (CHONG_LIAN *)pStationData;
			if(NULL==pData)
			{
				return false;
			}
			///��ǰ����������µ���ע
			m_iMaxNote = pData->iMaxNote;
			m_iNowNtStation=pData->m_iNowNtStation;// ��ǰׯ�ҵ�λ��
			//�ȴ���ʱ��
			SetGameTimer(GetMeUserInfo()->bDeskStation,(rand()%5)+1, ID_WAIT_TIME);
			SetStationParameter(GS_WAIT_AGREE);
			return true;
		}
	case GS_SEND_CARD:	//����״̬
		{
			CHONG_LIAN *pData = (CHONG_LIAN *)pStationData;
			if(NULL==pData)
			{
				return false;
			}
			///��ǰ����������µ���ע
			m_iMaxNote= pData->iMaxNote;
			m_iNowNtStation=pData->m_iNowNtStation;// ��ǰׯ�ҵ�λ��
			SetStationParameter(GS_SEND_CARD);
			return true;
		}
	case GS_PLAY_GAME:	//��Ϸ����״̬
		{
			CHONG_LIAN *pData = (CHONG_LIAN *)pStationData;
			if(NULL==pData)
			{
				return false;
			}
			///��ǰ����������µ���ע
			m_iMaxNote= pData->iMaxNote;
			m_iNowNtStation=pData->m_iNowNtStation;// ��ǰׯ�ҵ�λ��
			SetStationParameter(GS_PLAY_GAME);
			return true;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			CHONG_LIAN *pData = (CHONG_LIAN *)pStationData;
			if(NULL==pData)
			{
				return false;
			}
			///��ǰ����������µ���ע
			m_iMaxNote= pData->iMaxNote;
			m_iNowNtStation=pData->m_iNowNtStation;// ��ǰׯ�ҵ�λ��
			//�ȴ���ʱ��
			SetGameTimer(GetMeUserInfo()->bDeskStation,(rand()%5)+1, ID_WAIT_TIME);
			SetStationParameter(GS_WAIT_NEXT);
			return true;
		}
	default:
		break;
	}
	ASSERT(CHECK_DEBUG);
	return true;
}
//��ȡ�����ע����
int CClientGameDlg::GetRandType()
{
	int type=0;
	srand((unsigned)GetTickCount() + GetMeUserInfo()->bDeskStation);
	type = rand()%100;
	if(m_iAIMoney[0]<m_iAIMoney[3])//ׯ,����ׯ��������עƽ��
	{
		type = 0;
	}
	else //��
	{
		type = 3;
	}
	return type;
}

//�����ע
void CClientGameDlg::OnSuiJiXiaZhu()
{	
	__int64 money=0;//��ͷ�ϻ�����ע��Ǯ
	int nete=0;
	int moneytype=0;	//��ע��������
	int type=0;			//��ע����

	int nTempMoney=1; //����zht
	srand((unsigned)GetTickCount() + GetMeUserInfo()->bDeskStation);
	if(m_iMaxNote >0)
	{
		money = min((GetMeUserInfo()->i64Money - m_iMeZhu) , m_iMaxNote) ;
	}
	else
	{
		money = GetMeUserInfo()->i64Money - m_iMeZhu ;
	}

	int  pct = rand()%1000;
	if(pct<(m_byZTWangPct + m_byZDZiPct + m_byXTWangPct + m_byXDZiPct + m_byHePct + m_byTDHePct))
	{
		if(pct<m_byZTWangPct)//ׯ����
		{
			type = 1;
		}
		else if(pct<(m_byZTWangPct + m_byZDZiPct))//ׯ����
		{
			type = 2;
		}
		else if(pct<(m_byZTWangPct + m_byZDZiPct + m_byXTWangPct))//������
		{
			type = 4;
		}
		else if(pct<(m_byZTWangPct + m_byZDZiPct + m_byXTWangPct + m_byXDZiPct))//�ж���
		{
			type = 5;
		}
		else if(pct<(m_byZTWangPct + m_byZDZiPct + m_byXTWangPct + m_byXDZiPct + m_byHePct))//��
		{
			type = 6;
		}
		else//ͬ���
		{
			type = 7;
		}
	}
	else
	{
		type = GetRandType();
	}
	
	if(money >= 10000000)//����1000��
	{
		moneytype = Getchip(0) + 1;//��ע��������
	}
	else if(money >= 1000000 && money < 10000000)//����100��
	{
		moneytype = Getchip(1) + 1;//��ע��������
	}
	else if(money >= 100000 && money < 1000000)//����10��
	{
		moneytype = Getchip(2) + 1;//��ע��������
	}
	else if(money >= 10000 && money < 100000)//����1��
	{
		moneytype = Getchip(3) + 1;//��ע��������
	}
	else if(money >= 1000 && money < 10000)
	{
		moneytype = Getchip(4) + 1;//��ע��������
	}
	else
	{
		moneytype = Getchip(5) + 1;//��ע��������
	}
	
	XIA_ZHU  xiazhu;
	xiazhu.type = type;
	xiazhu.m_bIsAI = true;

		bool bEndWhile = false;
		int TotalTemp = 0;
		xiazhu.moneytype = moneytype;

		int  iTempValue = (int)pow(2.0 , moneytype - 1) ;

		// �����˿��������Զ���ע����ע���ҲҪ�����ù���Ա����һ����Χ����עҲ�ǰ������Χ����ע��������ע������ֵ�ĳ���
		BOOL bEnableNote = (m_iCanNoteValue & iTempValue);

		if(FALSE == bEnableNote)
		{
			return ; 
		}
		switch(moneytype)
		{
		case 6://1000��ĳ���ֻ��һ��
			{
				int temp=0;
				temp = (GetMeUserInfo()->bDeskStation+1)%20;
				temp*=20;
				Sleep(temp);
			

#ifdef SET72 
			
			xiazhu.money = 10000000;		
			
#else
           	xiazhu.money = 5000000;
#endif


				xiazhu.money=xiazhu.money/nTempMoney;
				
				if ((xiazhu.money+m_iMeZhu>m_iMaxDownMoney) || (xiazhu.money+m_iMeZhu>money) || ((m_iMeZongZhu+xiazhu.money)>m_iMaxNote && m_iMaxNote>0))
				{
					moneytype--;
					break;
				}
				bEndWhile = true;
				for(int i=0;i<3;i++)
				{
					
					SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
					type = GetRandType();
					xiazhu.type = type;
					int temp=0;
					temp = (GetMeUserInfo()->bDeskStation+1)%10;
					temp*=10;
					Sleep(temp);
				}
			}
			break;
		case 5://100��ĳ���ֻ��һ��
			{
				int temp=0;
				temp = (GetMeUserInfo()->bDeskStation+1)%20;
				temp*=30;
				Sleep(temp);
				xiazhu.money = 1000000;
				xiazhu.money=xiazhu.money/nTempMoney;
				if ((xiazhu.money+m_iMeZhu>m_iMaxDownMoney) || (xiazhu.money+m_iMeZhu>money)|| ((m_iMeZongZhu+xiazhu.money)>m_iMaxNote && m_iMaxNote>0))
				{
					moneytype--;
					break;
				}


				bEndWhile = true;
				for(int i=0;i<3;i++)
				{				
					SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
					type = GetRandType();
					xiazhu.type = type;
					int temp=0;
					temp = (GetMeUserInfo()->bDeskStation+1)%10;
					temp*=10;
					Sleep(temp);
				}
			}
			break;
		case 4://10��ĳ���ֻ��һ��
			{
				int temp=0;
				temp = (GetMeUserInfo()->bDeskStation+1)%20;
				temp*=40;
				Sleep(temp);
				xiazhu.money = 100000;
				xiazhu.money=xiazhu.money/nTempMoney;
				if ((xiazhu.money+m_iMeZhu>m_iMaxDownMoney) || (xiazhu.money+m_iMeZhu>money)|| ((m_iMeZongZhu+xiazhu.money)>m_iMaxNote && m_iMaxNote>0))
				{
					moneytype--;
					break;
				}


				bEndWhile = true;
				for(int i=0;i<3;i++)
				{
					SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
					type = GetRandType();
					xiazhu.type = type;
					int temp=0;
					temp = (GetMeUserInfo()->bDeskStation+1)%10;
					temp*=10;
					Sleep(temp);
				}
			}
			break;
		case 3://1��ĳ�������5��
			{
				int iCount = 5;
				xiazhu.money = 10000;
				xiazhu.money=xiazhu.money/nTempMoney;
				if (xiazhu.money*2+m_iMeZhu>m_iMaxDownMoney)
				{
					if ((xiazhu.money+m_iMeZhu>m_iMaxDownMoney) || (xiazhu.money+m_iMeZhu>money)|| ((m_iMeZongZhu+xiazhu.money)>m_iMaxNote && m_iMaxNote>0))
					{
						moneytype--;
						break;
					}
					else
					{
						iCount = 2;
					}
				}

				for(int i=0;i<iCount;i++)
				{
					SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
					type = GetRandType();
					xiazhu.type = type;
					int temp=0;
					temp = (GetMeUserInfo()->bDeskStation+1)%20;
					temp*=60;
					Sleep(temp);
				}

				bEndWhile = true;
			}
			break;
		case 2://1ǧ�ĳ�������3��
			{
				int iCount = 5;
				xiazhu.money = 1000;
				xiazhu.money=xiazhu.money/nTempMoney;
				if (xiazhu.money*3+m_iMeZhu>m_iMaxDownMoney)
				{
					if ((xiazhu.money+m_iMeZhu>m_iMaxDownMoney) || (xiazhu.money+m_iMeZhu>money)|| ((m_iMeZongZhu+xiazhu.money)>m_iMaxNote && m_iMaxNote>0))
					{
						moneytype--;
						break;
					}
					else
					{
						iCount = 1;
					}
				}

				for(int i=0;i<iCount;i++)
				{
					type = GetRandType();
					xiazhu.type = type;
					
					SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
					int temp=0;
					temp = (GetMeUserInfo()->bDeskStation+1)%10;
					temp*=10;
					Sleep(temp);
				}

				bEndWhile = true;
			}
			break;
		case 1://100�ĳ�������4��
			{
				int iCount = 4;
				xiazhu.money = 100;
				xiazhu.money=xiazhu.money/nTempMoney;
				if (xiazhu.money*4+m_iMeZhu>m_iMaxDownMoney)
				{
					if ((xiazhu.money+m_iMeZhu>m_iMaxDownMoney) || (xiazhu.money+m_iMeZhu>money)|| ((m_iMeZongZhu+xiazhu.money)>m_iMaxNote && m_iMaxNote>0))
					{
						moneytype--;
						break;
					}
					else
					{
						iCount = 1;
					}
				}

				for(int i=0;i<iCount;i++)
				{
					type = GetRandType();
					xiazhu.type = type;
					SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
					int temp=0;
					temp = (GetMeUserInfo()->bDeskStation+1)%10;
					temp*=10;
					Sleep(temp);
				}

				bEndWhile = true;
			}
		}
}
///ԭ�ȵ���Ϊ��λ�����Գ�����ע���ٶȣ�����д��Ϊ��Ϊ��λ
bool CClientGameDlg::SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUserMilliSecond)
{
	int iTimeType = 1000 ; 
	if(m_bUseMilliSecond&&bUserMilliSecond)
	{
		iTimeType = m_iMilliSecond ; 
	}
	else
	{
		iTimeType = 1000 ; 
	}

	if(m_pGameRoom != NULL)
		m_pGameRoom->SendMessage(IDM_SET_GAME_TIME,(WPARAM)uTimeID,(LPARAM)uTimeCount*iTimeType);
	return true;
}
//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE byDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_WAIT_TIME:  //�ȴ���ʱ��
		{			
			if (uTimeCount <= 0)
			{			
				KillGameTimer(ID_WAIT_TIME);
				SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);//����ͬ����Ϸ��Ϣ
				if (!CheckApply(GetMeUserInfo()->i64Money))
				{
					return true;
				}
				SHANG_ZHUANG zhuang;
				zhuang.shang=true;
				zhuang.isAI = true;
				// ��������ׯ��Ϣ
				SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
			}
			return true;
		}
	case ID_XIAZHU_TIME:
		{
			if(m_iMeZhu >= GetMeUserInfo()->i64Money/10 || m_iMeZhu>=m_iMaxDownMoney || (0<m_iMeZhu && 80<GetMeUserInfo()->bDeskStation))
			{
				KillGameTimer(ID_XIAZHU_TIME);//ֹͣ��ע��ʱ��
				KillGameTimer(ID_XIAZHU_TOTAL_TIME);
				return true;
			}
			if(uTimeCount<=0)
			{
				KillGameTimer(ID_XIAZHU_TIME);
				//�����ע
				OnSuiJiXiaZhu();
				int time = rand()%2+1;
				// ��������Ҫ�ظ���ע
				SetGameTimer(GetMeUserInfo()->bDeskStation, time, ID_XIAZHU_TIME,true);
			}
			return true;
		}
	case ID_XIAZHU_TOTAL_TIME:	// ��ע��ʱ��
		{
			KillGameTimer(ID_XIAZHU_TIME);
			KillGameTimer(ID_XIAZHU_TOTAL_TIME);
			return true;
		}
	default: break;
	}
	return true;
}
// ������ж�ʱ��
void CClientGameDlg::KillAllTimer(void)
{
	if (NULL != m_hWnd)
	{
		KillTimer(ID_BEGIN_TIME);
	}
	return;
}

// ��ȫ������Ϸ
bool CClientGameDlg::OnControlSafeFinish()
{
	return false;
}

//�汾�˶�
bool CClientGameDlg::CheckVersion(BYTE byHighVersion,BYTE byLowVersion,TCHAR szUpDateHttp[100])
{
	if (NULL == m_pGameInfo)
	{
		PostMessage(WM_CLOSE,0,0);
		return false;
	}
	return TRUE;
}

//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)//��Ϸ��ʼ
{
}  
// �����ע
LRESULT	CClientGameDlg::OnXiaZhu(WPARAM wparam, LPARAM lparam)
{
	return 0;
}
// // ������Ϣ����
LRESULT	CClientGameDlg::OnTest(WPARAM wparam, LPARAM lparam)
{
	BJL_TEST test;
	test.type = (int)lparam;
	//AfxMessageBox("test1");
	SendGameData(&test, sizeof(test), MDM_GM_GAME_NOTIFY, ASS_BJL_TEST,0);
	return 0;
}
//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (MDM_GM_GAME_NOTIFY == pNetHead->bMainID)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_SUPER_STATE: // �ж��Ƿ�Ϊ�����ͻ���
			{
				if (sizeof(SUPERSTATE) != uDataSize)
				{
					return false;
				}
				return true;
			}
		case ASS_SUPER_SETTING:  // ��Ӯ������Ϣ
			{
				if (uDataSize != sizeof(SUPERSETTING))
				{
					return false;
				}
				return true;
			}
		case ASS_SUPER_AI_INFO:  // ///��������Ӯ���ƣ��յ�������Ϣ
			{
				return true;
			}
		case ASS_XIA_ZHU:			// ��ע��Ϣ
			{
				if (sizeof(XIA_ZHU) != uDataSize)
				{
					return false;
				}
				OnHandleXiazhu(pNetData, uDataSize);
				return true;
			}
		case ASS_KAI_PAI:			// ������Ϣ
			{		
				if (sizeof(KAI_PAI) != uDataSize)
				{
					return false;
				}
				OnHandleKaiPai(pNetData, uDataSize);
				return true;
			}
		case ASS_SHOW_JS:
			{
				
				if (sizeof(JIE_SUAN) != uDataSize)
				{
					return false;
				}
				return true;
			}
		case ASS_DENG_DAI:			// �ȴ���Ϣ
			{
				if (sizeof(DENG_DAI) != uDataSize)
				{
					return false;
				}
				DENG_DAI *pWaitData = (DENG_DAI *)pNetData ; 
				if(GetMeUserInfo()->bDeskStation == m_iNowNtStation && m_bXZSwitch)
				{
					if((rand()%100)>=80)
					{
						SHANG_ZHUANG zhuang;
						zhuang.shang=false;
						zhuang.isAI = true;
						zhuang.station = m_iNowNtStation ; 
						SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
					}
				}
				//�ȴ���ʱ��
				SetGameTimer(GetMeUserInfo()->bDeskStation,(rand()%5)+1, ID_WAIT_TIME);
				SetStationParameter(GS_WAIT_NEXT);//��Ϸ��Ϊ��ʼ״̬
				return true;
			}
		case ASS_SHANG_ZHUANG:      // ��ׯ��Ϣ
			{
				if (uDataSize!=sizeof(SHANG_ZHUANG))
				{
					return false;
				}
				OnHandleApplyNt(pNetData, uDataSize);
				return true;
			}
		case ASS_BJL_BEGIN:			// ��ʼ��Ϣ
			{
				if (uDataSize!=sizeof(BEGIN_DATA))
				{
					return false;
				}
				OnHandleBegin(pNetData, uDataSize);
				return true;
			}
		case ASS_WU_ZHUANG://�յ���ׯ�ȴ���Ϣ
			{
				if (sizeof(DENG_DAI) != uDataSize)
				{
					return false;
				}
				SetStationParameter(GS_WAIT_AGREE);//��Ϸ��Ϊ��ʼ״̬
				DENG_DAI *pWait = (DENG_DAI *)pNetData;					
				//������ׯ�������˾���ׯ
				SHANG_ZHUANG zhuang;
				zhuang.shang=true;
				zhuang.isAI = true;
				SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
				return true;
			}
		default:
			break;
		}
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}
// ����ټ�����ׯ��Ϣ
void CClientGameDlg::OnHandleApplyNt(void * pNetData, UINT uDataSize)
{
	SHANG_ZHUANG *pApplyNt = (SHANG_ZHUANG *)pNetData;	
	if(NULL==pApplyNt)
	{
		return;
	}
	m_iNowNtStation=pApplyNt->m_iNowNtStation;// ��ǰׯ�ҵ�λ��
}
void CClientGameDlg::OnCancel()
{
	CLoveSendClass::OnCancel();
}

void CClientGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	default:
		break;
	}
	CLoveSendClass::OnTimer(nIDEvent);
}

//���ش����ƶ�����
void CClientGameDlg::OnMove(int x, int y)
{
}
// ϵͳ������Ϣlym0512
void CClientGameDlg::OnHandleKaiPai(void * pNetData, UINT uDataSize)
{
	SetStationParameter(GS_PLAY_GAME);//��Ϸ��Ϊ��ʼ״̬
	KillGameTimer(ID_XIAZHU_TIME);
	KillTimer(ID_SEND_CARD_TIMER);
	KillTimer(ID_TEST_TIMER);//ɾ�����Զ�ʱ��

	KAI_PAI *paidata = (KAI_PAI *)pNetData;
	if (NULL == paidata)
	{
		return ;
	}
	//��¼�Ͼֿ�����ׯ������
	m_iZhuang = paidata->m_iWinner ;
	
	if(paidata->m_iWinner == 1)
	{
		m_iZnum++;//ׯ�Ҿ���
		m_iXnum = 0;//���о���
	}
	else if(paidata->m_iWinner == 2)
	{
		m_iZnum = 0;//ׯ�Ҿ���
		m_iXnum++;//���о���
	}
	
	//if(m_iZhuangBaShu>=3)//����������3��ׯ���б������˵ȴ���ׯ�������˾���ׯ�ñ�����ׯ

	//if(m_iNowNtStation==GetMeUserInfo()->bDeskStation)
	//{ 
	//	if (m_iRobortSitCount>=m_iRobortSitZMax||(m_bPlayerInList&&m_iRobortSitCount>=m_iRobortSitZMin) )
	//	{
	//		SHANG_ZHUANG zhuang;
	//		zhuang.shang=false;
	//		zhuang.isAI = true;
	//		// ��������ׯ��Ϣ
	//		SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
	//	}	
	//}
	
}
// �����ע��Ϣlym0512
void CClientGameDlg::OnHandleXiazhu(void * pNetData, UINT uDataSize)
{		
	XIA_ZHU *pXiazhu = (XIA_ZHU *)pNetData;
	if(NULL==pXiazhu)
	{
		return;
	}
	if(pXiazhu->station == GetMeUserInfo()->bDeskStation)
	{
		m_iMeZhu += pXiazhu->money;
	}
	memcpy(m_iAIMoney,pXiazhu->m_iAIMoney,sizeof(m_iAIMoney));
	///��ǰ��ע
	m_iMeZongZhu	= pXiazhu->m_iMeZhongZhu;


	if(pXiazhu->moneytype == 6)
	{
		
	}

}
// ����ټ�����Ϸ��ʼ��Ϣlym0512
void CClientGameDlg::OnHandleBegin(void * pNetData, UINT uDataSize)
{	
	
	BEGIN_DATA *pBegin = (BEGIN_DATA *)pNetData;
	if(NULL==pBegin)
	{
		
		return;
	}
	//m_iSendCardTime =0;						// ���ƶ�������ʱ�䣬���ƶ�����	
	m_bCanXiaZhu = true ;						// �ܷ���ע��
	SetStationParameter(GS_SEND_CARD);			// ��Ϸ��Ϊ��ʼ״̬
	CINIFile iniFile(CINIFile::GetAppPath()+"BJLRobotSet.ini");	
	// ÿ������¾�
	m_iMaxDownMoney = iniFile.GetKeyVal("Money", "MaxDownMoney", (__int64)1000000);
	m_iMaxZhuang	= iniFile.GetKeyVal("bjl", "MaxZhuang", 10);// ׯ�ҽ��м��Ѿ���ׯ
	m_iBaseNum	= iniFile.GetKeyVal("bjl", "BaseNum", 1);// ���������ֻ��������Я����ҵļ���֮һ

	///�����ׯ�����������
	m_byZTWangPct	= iniFile.GetKeyVal("bjl", "ZTWangPct", 50);				//ׯ������ע����(��ȷ��ǧ��֮һ) 
	m_byZDZiPct		= iniFile.GetKeyVal("bjl", "ZDZiPct", 100);				//ׯ������ע����(��ȷ��ǧ��֮һ) 
	m_byXTWangPct	= iniFile.GetKeyVal("bjl", "XTWangPct", 110);				//��������ע����(��ȷ��ǧ��֮һ) 
	m_byXDZiPct		= iniFile.GetKeyVal("bjl", "XDZiPct", 120);				//�ж�����ע����(��ȷ��ǧ��֮һ) 
	m_byHePct		= iniFile.GetKeyVal("bjl", "HePct", 130);					//����ע����	(��ȷ��ǧ��֮һ) 
	m_byTDHePct		= iniFile.GetKeyVal("bjl", "TDHePct", 140);				//ͬ�����ע����(��ȷ��ǧ��֮һ) 
	
	// �жϻ����������ׯ����
	CString strkey;
	strkey = "RobortSet";	
	m_iRobortSitZMax = iniFile.GetKeyVal(strkey,"RobortSitZMax",15); // �����������ׯ����
	m_iRobortSitZMin = iniFile.GetKeyVal(strkey,"RobortSitZMin",5);

	m_bUse500WJeton = iniFile.GetKeyVal(strkey,"Use500WJeton", 0) > 0 ? true : false;

	m_bUseMilliSecond = iniFile.GetKeyVal("RobortSet", "UseMilliSecond", 1) > 0 ? true : false; ///ʹ�ú���Ϊ��λ

	m_iMilliSecond = iniFile.GetKeyVal("RobortSet", "MilliSecond", 200) ; ///ʹ�ú���Ϊ��λ
	
	//���������ļ�
	void LoadIniConfig();

	if (m_iRobortSitZMax<=0)
	{
		m_iRobortSitZMax = 1;
	}
	if (m_iRobortSitZMin>m_iRobortSitZMax)
	{
		m_iRobortSitZMin = m_iRobortSitZMax;
	}	
	if (m_iRobortSitZMin <1)
	{
		m_iRobortSitZMin = 0;
	}

	///��ǰ��ע
	m_iMeZongZhu			= 0;	
	///��ǰ����������µ���ע
	m_iMaxNote          = pBegin->iMaxNote;
	int temp=0;
	CINIFile iniOrder(CINIFile::GetAppPath()+"Order.ini");	
 
	m_nPowerOfGold = iniOrder.GetKeyVal("VirtualMoney", "PowerOfGold", 0);// ���������ֻ��������Я����ҵļ���֮һ;//�Ŵ���PowerOfGold    VirtualMoney
	KillGameTimer(ID_WAIT_TIME);
	KillTimer(ID_SEND_CARD_TIMER);

	m_iMeZhu = 0;								//��ǰ�����˱����µ�ע
	memset(m_iAIMoney,0,sizeof(m_iAIMoney));    //��������ÿ�������µ�ע

	m_iNowNtStation = pBegin->m_iNowNtStation;	//��ȡׯ�ҵ�λ��

	m_iZhuangBaShu = pBegin->m_iZhuangBaShu;//ׯ�ҵľ���
	m_iShangZhuangLimit = pBegin->m_iShangZhuangLimit;//��ׯ�޶�
	m_bPlayerInList = pBegin->bHaveUserInList;   //�Ƿ��������ׯ���б���



	if(GetMeUserInfo()->bDeskStation == m_iNowNtStation)
	{
		m_iRobortSitCount++;
		if (m_iRobortSitCount>=m_iRobortSitZMax||(TRUE == m_bPlayerInList&&m_iRobortSitCount>=m_iRobortSitZMin))
		{
			/*SHANG_ZHUANG zhuang;
			zhuang.shang=false;
			zhuang.isAI = true;
			SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);*/			
		}
	}
	else
	{
		m_iRobortSitCount = 0;
	}
	if(-1 ==m_iNowNtStation)
	{
		srand((unsigned)time(NULL)+GetMeUserInfo()->bDeskStation);
		int iRand = (rand()%3+GetMeUserInfo()->bDeskStation)%3;
		if (iRand>1)
		{
			SHANG_ZHUANG zhuang;
			zhuang.shang=true;
			zhuang.isAI = true;
			SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
		}
	}
	
	if(m_iNowNtStation != GetMeUserInfo()->bDeskStation && m_iNowNtStation>=0)//����ׯ�ҾͿ�����ע
	{

		int temp=0,TotalTemp=0;
		temp = (GetMeUserInfo()->bDeskStation+1)%20;
		temp*=20;
		
		if (0<=GetMeUserInfo()->bDeskStation && 10>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%1;
		}
		else if (10<=GetMeUserInfo()->bDeskStation && 20>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%2;
		}
		else if (20<=GetMeUserInfo()->bDeskStation && 30>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%3;
		}
		else if (30<=GetMeUserInfo()->bDeskStation && 40>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%3;
		}
		else if (50<=GetMeUserInfo()->bDeskStation && 60>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%4;
		}
		else if (70<=GetMeUserInfo()->bDeskStation && 80>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%4;
		}
		else if (90<=GetMeUserInfo()->bDeskStation && 100>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%5;
		}
		else if (110<=GetMeUserInfo()->bDeskStation && 120>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%5;
		}
		else if (120<=GetMeUserInfo()->bDeskStation && 130>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%6;
		}
		else if (130<=GetMeUserInfo()->bDeskStation && 140>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%6;
		}
		else if (140<=GetMeUserInfo()->bDeskStation && 150>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%7;
		}
		else if (150<=GetMeUserInfo()->bDeskStation && 160>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%7;
		}
		else if (160<=GetMeUserInfo()->bDeskStation && 170>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%8;
		}
		else if (170<=GetMeUserInfo()->bDeskStation && 180>GetMeUserInfo()->bDeskStation)
		{
			temp = GetMeUserInfo()->bDeskStation%8;
		}
		int iAllTime = pBegin->m_iXiaZhuTime;
		SetGameTimer(GetMeUserInfo()->bDeskStation,iAllTime, ID_XIAZHU_TOTAL_TIME);
		SetGameTimer(GetMeUserInfo()->bDeskStation, temp+1, ID_XIAZHU_TIME,true);
		
	}		
}

//���������ļ�
void CClientGameDlg::LoadIniConfig()
{
	CINIFile iniFile(CINIFile::GetAppPath()+"BJLRobotSet.ini");	

	// �жϻ����������ׯ����
	CString strkey;
	strkey = "RobortSet";

	// ����ׯ����
	m_bSZSwitch = iniFile.GetKeyVal("RobortSet", "SZSwitch", 1);
	m_bXZSwitch = iniFile.GetKeyVal("RobortSet", "XZSwitch", 1);

	// ������ׯ������1�ڣ������ƻ�������ׯ�����1�����������������λ�õ���ֵ�������ׯ
	m_iMaxApplyNtMoney = iniFile.GetKeyVal("RobortSet","MaxApplyNtMoney",2000000000); 
	m_iMinApplyNtMoney = iniFile.GetKeyVal("RobortSet","MinApplyNtMoney",1000);  

	// ��ׯ���˶���Ǯ��������ׯ��Ӯ�˶���Ǯ��Ҳ������ׯ
	m_iWinDownMoney = iniFile.GetKeyVal("RobortSet","WinDownMoney",100000000); 
	m_iLostDownMoney = iniFile.GetKeyVal("RobortSet","LostDownMoney",10000000); 

	// �����˿��������Զ���ע����ע���ҲҪ�����ù���Ա����һ����Χ����עҲ�ǰ������Χ����ע��������ע������ֵ�ĳ���
	m_iCanNoteValue = iniFile.GetKeyVal("RobortSet","CanNoteValue",63);

	///////////////////////
	//������������Ǯ��ͬ��Ŀ�ǿ�����ע�ĳ����������£�
	//��������ֱ��������
	///////////////////////

	////����������Ǯ>=1000w

	m_probability[0][0] = iniFile.GetKeyVal("RobortSet","probabilityA1",1)      ;//100��עȨ��
	m_probability[0][1] = iniFile.GetKeyVal("RobortSet","probabilityA2",2)      ;//1ǧ��עȨ��
	m_probability[0][2] = iniFile.GetKeyVal("RobortSet","probabilityA3",3)      ;//1w��עȨ��
	m_probability[0][3] = iniFile.GetKeyVal("RobortSet","probabilityA4",50)     ;//10w��עȨ��
	m_probability[0][4] = iniFile.GetKeyVal("RobortSet","probabilityA5",300)    ;//100w��עȨ��
	m_probability[0][5] = iniFile.GetKeyVal("RobortSet","probabilityA6",500)    ;//1000w��עȨ��

	////����������Ǯ<1000w ,>=100w

	m_probability[1][0] = iniFile.GetKeyVal("RobortSet","probabilityB1",1)      ;//100��עȨ��
	m_probability[1][1] = iniFile.GetKeyVal("RobortSet","probabilityB2",2)      ;//1ǧ��עȨ��
	m_probability[1][2] = iniFile.GetKeyVal("RobortSet","probabilityB3",3)      ;//1w��עȨ��
	m_probability[1][3] = iniFile.GetKeyVal("RobortSet","probabilityB4",50)     ;//10w��עȨ��
	m_probability[1][4] = iniFile.GetKeyVal("RobortSet","probabilityB5",100)    ;//100w��עȨ��
	m_probability[1][5] = 0    ;//1000w��עȨ��

	////����������Ǯ<100w ,>=10w

	m_probability[2][0] = iniFile.GetKeyVal("RobortSet","probabilityC1",1)      ;//100��עȨ��
	m_probability[2][1] = iniFile.GetKeyVal("RobortSet","probabilityC2",5)      ;//1ǧ��עȨ��
	m_probability[2][2] = iniFile.GetKeyVal("RobortSet","probabilityC3",50)      ;//1w��עȨ��
	m_probability[2][3] = iniFile.GetKeyVal("RobortSet","probabilityC4",100)     ;//10w��עȨ��
	m_probability[2][4] = 0    ;//100w��עȨ��
	m_probability[2][5] = 0    ;//1000w��עȨ��

	////����������Ǯ<10w ,>=1w

	m_probability[3][0] = iniFile.GetKeyVal("RobortSet","probabilityD1",1)      ;//100��עȨ��
	m_probability[3][1] = iniFile.GetKeyVal("RobortSet","probabilityD2",50)      ;//1ǧ��עȨ��
	m_probability[3][2] = iniFile.GetKeyVal("RobortSet","probabilityD3",100)      ;//1w��עȨ��
	m_probability[3][3] = 0     ;//10w��עȨ��
	m_probability[3][4] = 0    ;//100w��עȨ��
	m_probability[3][5] = 0    ;//1000w��עȨ��

	////����������Ǯ<1w ,>=1000

	m_probability[4][0] = iniFile.GetKeyVal("RobortSet","probabilityE1",1)      ;//100��עȨ��
	m_probability[4][1] = iniFile.GetKeyVal("RobortSet","probabilityE2",10)      ;//1ǧ��עȨ��
	m_probability[4][2] = 0      ;//1w��עȨ��
	m_probability[4][3] = 0     ;//10w��עȨ��
	m_probability[4][4] = 0    ;//100w��עȨ��
	m_probability[4][5] = 0    ;//1000w��עȨ��

	////����������Ǯ<1000

	m_probability[5][0] = iniFile.GetKeyVal("RobortSet","probabilityF1",1)      ;//100��עȨ��
	m_probability[5][1] = 0      ;//1ǧ��עȨ��
	m_probability[5][2] = 0      ;//1w��עȨ��
	m_probability[5][3] = 0     ;//10w��עȨ��
	m_probability[5][4] = 0    ;//100w��עȨ��
	m_probability[5][5] = 0    ;//1000w��עȨ��

}

int  CClientGameDlg::Getchip(int line_num)			///����Ȩ�ظ����Ի�ȡһ��λ��
{
	int weight(0),M(10000),m(0);
	double probability[CHIP_NUM],area[CHIP_NUM + 1];
	CString str1;

	for (int i=0;i<CHIP_NUM;i++)
	{
		weight += m_probability[line_num][i];//Ȩֵ�ܺ�
	}

	for (int i=0;i<CHIP_NUM;i++)
	{
		probability[i] = m_probability[line_num][i] / (double)weight;//ÿ�������Ȩ��
	}

	//Ϊÿ���������ֵ��M�ڲ�,ûһ���ϵ㣩
	area[0]	= 0.0;
	//area[1] = probability[0] * M;
	//area[2] = probability[0] * M + probability[1] * M;
	//area[3] = probability[0] * M + probability[1] * M + probability[2] * M;
	//...

	double n = 0.0;//�м�������ۼ�ֵ
	for (int i = 1; i < (CHIP_NUM + 1) ;i++)
	{
		for (int j = 0; j < i; j++)
		{
			n += probability[j] * M;
			CString str1;
			str1.Format("---:aprobability[j]=%d",probability[j]);
			OutputDebugString(str1);
			area[i] = n;//���i���ϵ�
		}
		n = 0;

	}

	srand((unsigned)GetTickCount() + GetMeUserInfo()->bDeskStation);
	m = rand() % M;
	for (int i = 0;i < CHIP_NUM;i++)
	{	
		if (m >= area[i] && m < area[i + 1])
		{
			return i;
		}
	}
}