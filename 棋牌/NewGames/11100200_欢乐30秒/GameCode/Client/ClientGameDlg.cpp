#include "StdAfx.h"
#include "resource.h"
#include "ClientGameDlg.h"
#include "playcardsound.h"
#include "GameSet.h"
#include "SuperSettingDlg.h"
#include "afxmt.h"
#ifdef SUPER_VERSION
//#include "../../�ͻ������/�����.2003/CardSet.h"
#include "platform/CardSet.h"
#endif

CEvent g_Event,g_EventPlay;

void MsgPrint(const char *lpszFormat, ...)
{
	va_list pArg;
	va_start(pArg, lpszFormat);
	char szMessage[1024] = { 0 };
#ifdef WIN32
	_vsnprintf(szMessage, 1023, lpszFormat, pArg);
#else
	vsnprintf(szMessage, 1023, lpszFormat, pArg);
#endif
	va_end(pArg);
	OutputDebugString(szMessage);

}

/// ��Ϣӳ��
BEGIN_MESSAGE_MAP(CClientGameDlg,CLoveSendClassInExe/* CLoveSendClass*/)
	ON_MESSAGE(WM_USER_SHANG_ZHUANG, OnShangZhuang)       // �������ׯ����
	ON_MESSAGE(WM_USER_XIA_ZHU, OnXiaZhu)           // �����ע
	ON_MESSAGE(WM_USER_TEST, OnTest)           // ������Ϣ
	ON_MESSAGE(WM_USER_BANK, OnBank)       // ��Ҵ�������Ϣ
	ON_MESSAGE(WM_SUPER_CTR, OnSuperCtr)       // ������ҿ�����Ϣ
	ON_MESSAGE(IDM_SEND_AI_CTRL,OnSendAICtrl)///�����˿��Ƹı���Ϣ
	ON_WM_TIMER()
	//ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

///���ƣ�WriteInfo
///������д��־ add by wlr 20090806
///@param pch �ַ���Ϣ
///@return  
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

///���ƣ�WorkThread
///������������ע�����߳�
///@param 
///@return  
DWORD WINAPI WorkThread(LPVOID pParam)
{
	CClientGameDlg * pDlg = (CClientGameDlg *)pParam;
	if (pDlg == NULL)
	{
		return 0;
	}
	
	XIA_ZHU xiazhu;
	while (true)
	{
		while (pDlg->m_ArrNote.GetCount() > 0)
		{
			// ��ȡ����
			xiazhu = pDlg->m_ArrNote.GetAt(0);

			pDlg->OnHandleXiazhu(&xiazhu, sizeof(xiazhu));

			// ɾ������
			pDlg->m_ArrNote.RemoveAt(0);
		}

		// �������ݺ󣬽���ȴ�״̬
	    WaitForSingleObject(g_Event, INFINITE);

		if (pDlg->m_bThreadExit)
		{
			pDlg->m_ArrNote.RemoveAll();

			// �˳�
			break;
		}
	}
	
	return 0;
}


///���ƣ�WorkThreadPlaySound
///���������������̣߳����𲥷Ŷ����е�����
///@param 
///@return  
DWORD WINAPI WorkThreadPlaySound(LPVOID pParam)
{
	CClientGameDlg * pDlg = (CClientGameDlg *)pParam;
	if (pDlg == NULL)
	{
		return 0;
	}

	XIA_ZHU xiazhu;
	while (true)
	{
		while (pDlg->m_ArrPlay.GetCount() > 0)
		{
			// ��ȡ����
			xiazhu = pDlg->m_ArrPlay.GetAt(0);

			pDlg->OnHandleXiazhuPlaySound(&xiazhu, sizeof(xiazhu));

			// ɾ������
			pDlg->m_ArrPlay.RemoveAt(0);
		}

		// �������ݺ󣬽���ȴ�״̬
		WaitForSingleObject(g_EventPlay, INFINITE);

		if (pDlg->m_bThreadExit)
		{
			pDlg->m_ArrPlay.RemoveAll();

			// �˳�
			break;
		}
	}

	return 0;
}

CClientGameDlg * g_pCClientGameDlg=NULL;
///���ƣ�CClientGameDlg
///��������Ϸ�ͻ��˿�ܹ��캯��
///@param 
///@return  
CClientGameDlg::CClientGameDlg(void):CLoveSendClassInExe(&m_PlayView) //CLoveSendClass(&m_PlayView)
{

	m_pSEBkMusic = createIrrKlangDevice();
	g_pCClientGameDlg=this;

	m_bPlayListPos = 0;
	memset(m_bPlayListHwnd, -1, sizeof(m_bPlayListHwnd));
	m_byHighVersion= DEV_HIGH_VERSION;  // ��Ϸ�汾
	m_byLowVersion = DEV_LOW_VERSION;   // ��Ϸ�汾
	m_iSendCardTime =0;					// ���ƶ�������ʱ�䣬���ƶ�����
	int i = 0;
	//m_bCanXiaZhu =false ;							//�ܷ���ע��
	m_byViewOrder = ID_VIEW_ANTICLOCKWISE;
	for (i=0; i<PLAY_COUNT; ++i)
	{
		m_abySeatID[i] = (BYTE)i;
		m_byHandCardCount[i]=0;         // ��������е�������
		m_bySentCardCount[i]=0;         // ������Ѿ����˵�������
		m_bIsTrustee[i]=false;          // �й�
		m_iAwardPoint[i]=0;				// ���μӷ�
		m_bySequence[i]=PLAY_COUNT;		// ���ҳ������Ⱥ�˳��
		m_bGiveUp[i]=false;             // �Ƿ񲻳���
	}
	SoundbirdID = 0;							//��ǰ����������id

	m_iXiaZhuTime = 0;			//��עʱ��			
	m_iKaiPaiTime = 0;         //����ʱ��
	m_iFreeTime = 0;			//����ʱ��
	m_iSendInterval = 400;		//����ʱ����
	m_iTimeSendCard = 100;		//ÿ���Ʒ���ʱ��

	m_iNtStation = -1;                  // ׯ��λ��
	m_byCurOrientation=255;				// ��ǰ�����߷���

	memset(m_wHandCard, 0, sizeof(m_wHandCard)); // ��������е���

	m_wCurSendPos = 0;     // ��ǰ����λ��
	m_wSendCount   = 0;    // �ܹ���������
	m_byFirstSendPos=0;    // ��һ���������

	memset(m_iGameInfo, 0, sizeof(m_iGameInfo)); // ��ʱ��Ϸ��Ϣ

	m_bThreadExit = false;
	
	m_hMciWnd1 = NULL;
	m_hMciWnd2 = NULL;
	m_hMciWnd3 = NULL;
	m_hMciWnd4 = NULL;
	m_hMciWnd5 = NULL;

	m_iCountTimer = 0;

	m_bSuperState = FALSE;
	m_bInitFinish = false;			//�Ƿ��ʼ�����
	m_MoneyType = "";
	m_isWinRed =false;
	m_SpeCount=4;

}

CClientGameDlg::~CClientGameDlg(void)
{

	if(m_pSEBkMusic)
	{
		m_pSEBkMusic->drop();
	}

	m_bThreadExit = true;
	g_Event.SetEvent();
	g_EventPlay.SetEvent();

}
///���ƣ�OnInitDialog
///��������ʼ����Ϸ�ͻ��˿�ܹ�����
///@param 
///@return  
BOOL CClientGameDlg::OnInitDialog()
{
	//OutputDebugString("CClientGameDlg::OnInitDialog");
	// ��Ϸ��������
	m_PlayView.m_showcoredlg.m_ucomtype=GetComType();
	//CLoveSendClass::OnInitDialog();
	//���±���
	SetDialogIcon(LoadIcon(GetModuleHandle(CLIENT_DLL_NAME),MAKEINTRESOURCE(IDI_CLIENT_ROOM)),TRUE);

	// ������ע�����߳�
	CreateThread(NULL, 0, WorkThread, (LPVOID)this, 0, NULL);
	// ������ע�����߳�
	CreateThread(NULL, 0, WorkThreadPlaySound, (LPVOID)this, 0, NULL);
	// ��ʼ����
	BZSoundInitial();

	CLoveSendClassInExe::OnInitDialog();

	m_PlayView.m_iChooseNote = 0;
	m_PlayView.EnableXiaZhu(false);

	m_PlayView.m_nPowerOfGold=Glb().m_nPowerOfGold;  //��������

	m_PlayView.m_LuziDlg.ShowWindow(false);
	CBcfFile f("GameOption.bcf");
	m_MoneyType = f.GetKeyVal("config", "MoneyType", "���");
	m_isWinRed =f.GetKeyVal("game","isred",1);


	m_SpeCount =f.GetKeyVal("game","isSpe",4);//�ָ�������
	m_PlayView.m_Rcolor =f.GetKeyVal("game","rcolor",255);
	m_PlayView.m_Gcolor =f.GetKeyVal("game","gcolor",255);
	m_PlayView.m_Bcolor =f.GetKeyVal("game","bcolor",255);
	m_PlayView.m_isWinRed =m_isWinRed;
	m_PlayView.m_SpeCount=m_SpeCount;

	m_PlayView.m_SuperData.Init();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CClientGameDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���
	CLoveSendClassInExe::DoDataExchange(pDX);
	//CLoveSendClass::DoDataExchange(pDX);
	DoDataExchangeWebVirFace(pDX);
}

/// ��λ���㵽��ͼλ��
BYTE CClientGameDlg::GetViewSeat(BYTE byDeskStation)
{
	// ת��
	BYTE bySeatID = m_abySeatID[byDeskStation%PLAY_COUNT];

	// ����Լ���λ��
	BYTE bySelfSeatID = m_abySeatID[GetMeUserInfo()->bDeskStation%PLAY_COUNT];
	BYTE bySelfViewStation = PLAY_COUNT/2;
	if (bySelfSeatID == bySeatID)
	{
		return bySelfViewStation;
	}

	// �������
	if (ID_VIEW_ANTICLOCKWISE == m_byViewOrder)
	{
		return ((bySelfViewStation + ((bySeatID - bySelfSeatID + PLAY_COUNT)%PLAY_COUNT))%PLAY_COUNT);
	}
	else
	{
		return ((bySelfViewStation - ((bySeatID - bySelfSeatID + PLAY_COUNT)%PLAY_COUNT) + PLAY_COUNT)%PLAY_COUNT);;
	}
}


///���ƣ�SetGameStation
///������������Ϸ״̬������������ȡ��Ϸ����
///@param pStationData ���ݡ� uDataSize ���ݰ�����
///@return  
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	CString fdsa;
	m_bInitFinish = true;			//�Ƿ��ʼ�����
	// ��ȡ��ͼ˳��
	m_byViewOrder = (ID_VIEW_ANTICLOCKWISE == GetProfileInt(TEXT("ViewOrder"),ID_VIEW_ANTICLOCKWISE))?ID_VIEW_ANTICLOCKWISE: ID_VIEW_CLOCKWISE;
	m_PlayView.SetWaitingType(NO_WAITING);
	m_PlayView.m_btnshangz.EnableWindow(true);
	m_PlayView.EnableXiaZhu(false);

	switch (GetStationParameter())
	{         
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:		//�ȴ�ͬ��״̬
		{
			if (sizeof(CHONG_LIAN) > uDataSize)
			{
				return false;
			}

			CHONG_LIAN *sWaiting = (CHONG_LIAN *)pStationData;
			m_PlayView.m_byMyStation =GetMeUserInfo()->bDeskStation;//��ȡ�Լ���λ���Լ���λ��

			SetStationParameter(GS_WAIT_AGREE);
			m_PlayView.m_bNowGameState = GS_WAIT_AGREE;//��ǰ��Ϸ״̬

			SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);

			m_iXiaZhuTime = sWaiting->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime = sWaiting->m_iKaiPaiTime;        //����ʱ��
			m_iFreeTime = sWaiting->m_iFreeTime;			//����ʱ��
			m_iSendInterval = sWaiting->m_iSendInterval;	//����ʱ����
			m_iTimeSendCard = sWaiting->m_iSendCardTime;	//����ʱ��
			m_PlayView.m_iNowJuShu = sWaiting->m_iNowJuShu;	//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
			///��ǰ����������µ���ע
			m_PlayView.m_iMaxNote = sWaiting->iMaxNote;	

			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_PlayView.m_iZhuangList[i] = sWaiting->zhuangstation[i];
			}
			for(int i=0;i<MAXCOUNT;i++)
			{
				for(int j=0;j<4;j++)
				{
					m_PlayView.m_iGameInfo[i][j] = sWaiting->m_iResultInfo[i][j];
				}
			}

			m_PlayView.m_iPlayCount = sWaiting->m_iBaSHu;
			m_PlayView.m_iGameCount = sWaiting->m_iGameCount;
			m_PlayView.m_iNTju =  sWaiting->m_iZhuangBaShu;
			m_PlayView.m_iNowNTstation =  sWaiting->m_iNowNtStation;
			m_PlayView.m_iLimitMoney = sWaiting->m_iShangZhuangLimit;//��ׯ�޶�
			m_PlayView.m_iSendCardZhen = sWaiting->m_iSendCardZhen;		//����֡��

			char szMessage[MAX_PATH] = {0};
			CString strMixMoney, strMaxMoney, strMaxNoteMoney;

			//m_PlayView.NumToChar(m_PlayView.m_iLimitMoney,&strMixMoney);
			m_PlayView.ChangeNumToCStringByPower(m_PlayView.m_iLimitMoney ,true ,"," ,strMixMoney) ;
		
			_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ׯ������%s%s"), 
				strMixMoney,m_MoneyType);
			m_MessageHandle.InsertNormalMessage(szMessage);

			for (int i=0; i<MAXCOUNT; i++)			//������72����ע��Ϣ����Ӯ���
			{		
				m_PlayView.m_iMeXiaZhuInfo[i] = sWaiting->m_iUserXiaZhuInfo[i];
				m_PlayView.m_iResultInfoArea[i] = sWaiting->m_iResultInfoArea[i];	
				m_PlayView.m_iHistoryPlayData[i] = sWaiting->m_iResultInfoArea[i];


				m_PlayView.m_iHistoryDuiZi[i] =sWaiting->m_iResultInfo[i][3];
			}
			m_PlayView.UpdateLuziData();
			
			if (m_PlayView.m_iMaxNote > 0)
			{
				CString strBaseMaxNote;
				MsgPrint("zhangjing::m_iBaseMaxNote=%I64d",sWaiting->m_iBaseMaxNote);
				//m_PlayView.NumToChar(sWaiting->m_iBaseMaxNote, &strBaseMaxNote);
				m_PlayView.ChangeNumToCStringByPower(sWaiting->m_iBaseMaxNote ,true ,"," ,strBaseMaxNote) ;

				//m_PlayView.NumToChar(sWaiting->m_iBaseMaxNote*2, &strMaxNoteMoney);
				m_PlayView.ChangeNumToCStringByPower(sWaiting->m_iBaseMaxNote*2 ,true ,"," ,strMaxNoteMoney) ;

				if (sWaiting->bIsVipDouble)
					_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ͨ��������ע��%s%s�� VIP��������ע��%s%s"), strBaseMaxNote, m_MoneyType, strMaxNoteMoney, m_MoneyType); 
				else
					_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ͨ��������ע��%s%s"), strBaseMaxNote, m_MoneyType); 

				m_MessageHandle.InsertNormalMessage(szMessage);
			}
			return true;
		}
	case GS_SEND_CARD:	//����״̬
		{
			if (sizeof(CHONG_LIAN) > uDataSize)
			{
				return false;
			}
			SendGameData(MDM_GM_GAME_NOTIFY,ASS_APPLY_GAME_DATA,0);///��������lym2010-03-18
			return true;
		}
	case GS_PLAY_GAME:	//��Ϸ����״̬
		{
			if (sizeof(CHONG_LIAN) > uDataSize)
			{
				return false;
			}
			SendGameData(MDM_GM_GAME_NOTIFY,ASS_APPLY_GAME_DATA,0);///��������lym2010-03-18
			return true;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			CHONG_LIAN *psGameStationSendCard = (CHONG_LIAN *)pStationData;
			m_PlayView.m_byMyStation =GetMeUserInfo()->bDeskStation;	//��ȡ�Լ���λ���Լ���λ��			
			SetStationParameter(GS_WAIT_NEXT);

			m_iXiaZhuTime = psGameStationSendCard->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime = psGameStationSendCard->m_iKaiPaiTime;       //����ʱ��
			m_iFreeTime = psGameStationSendCard->m_iFreeTime;			//����ʱ��
			m_iSendInterval = psGameStationSendCard->m_iSendInterval;	//����ʱ����
			m_iTimeSendCard = psGameStationSendCard->m_iSendCardTime;	//����ʱ��
			///��ǰ����������µ���ע
			m_PlayView.m_iMaxNote = psGameStationSendCard->iMaxNote;	

			m_PlayView.m_iSendCardZhen = psGameStationSendCard->m_iSendCardZhen;		//����֡��
			m_PlayView.m_bNowGameState = GS_WAIT_NEXT;//��ǰ��Ϸ״̬

			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_PlayView.m_iZhuangList[i] = psGameStationSendCard->zhuangstation[i];
			}
			for(int i=0;i<MAXCOUNT;i++)
			{			
				for(int j=0;j<4;j++)
				{
					m_PlayView.m_iGameInfo[i][j] = psGameStationSendCard->m_iResultInfo[i][j];
				}
			}

			m_PlayView.m_iPlayCount = psGameStationSendCard->m_iBaSHu;
			m_PlayView.m_iGameCount = psGameStationSendCard->m_iGameCount;
			m_PlayView.m_iNTju =  psGameStationSendCard->m_iZhuangBaShu;
			m_PlayView.m_iNowNTstation =  psGameStationSendCard->m_iNowNtStation;
			m_PlayView.m_iLimitMoney = psGameStationSendCard->m_iShangZhuangLimit;//��ׯ�޶�

			char szMessage[MAX_PATH] = {0};
			CString strMixMoney, strMaxMoney, strMaxNoteMoney;
			//m_PlayView.NumToChar(m_PlayView.m_iLimitMoney,&strMixMoney);
			m_PlayView.ChangeNumToCStringByPower(m_PlayView.m_iLimitMoney ,true ,"," ,strMixMoney) ;

			_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ׯ������%s%s"), 
				strMixMoney,m_MoneyType);
			m_MessageHandle.InsertNormalMessage(szMessage);

			for (int i=0; i<MAXCOUNT; i++)			//������72����ע��Ϣ����Ӯ���
			{		
				m_PlayView.m_iMeXiaZhuInfo[i] = psGameStationSendCard->m_iUserXiaZhuInfo[i];
				m_PlayView.m_iResultInfoArea[i] = psGameStationSendCard->m_iResultInfoArea[i];		
				m_PlayView.m_iHistoryPlayData[i] = psGameStationSendCard->m_iResultInfoArea[i];		

				m_PlayView.m_iHistoryDuiZi[i]=psGameStationSendCard->m_iResultInfo[i][3];
			}
			m_PlayView.UpdateLuziData();

			if (m_PlayView.m_iMaxNote > 0)
			{
				CString strBaseMaxNote;
				//m_PlayView.NumToChar(psGameStationSendCard->m_iBaseMaxNote, &strBaseMaxNote);
				m_PlayView.ChangeNumToCStringByPower(psGameStationSendCard->m_iBaseMaxNote ,true ,"," ,strBaseMaxNote) ;

				//m_PlayView.NumToChar(psGameStationSendCard->m_iBaseMaxNote*2, &strMaxNoteMoney);
				m_PlayView.ChangeNumToCStringByPower(psGameStationSendCard->m_iBaseMaxNote*2 ,true ,"," ,strMaxNoteMoney) ;

				if (psGameStationSendCard->bIsVipDouble)
					_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ͨ��������ע��%s%s�� VIP��������ע��%s%s"), strBaseMaxNote, m_MoneyType, strMaxNoteMoney, m_MoneyType); 
				else
					_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ͨ��������ע��%s%s"), strBaseMaxNote, m_MoneyType); 
				m_MessageHandle.InsertNormalMessage(szMessage);					
			}
			return true;
		}
	default:
		break;
	}
	ASSERT(CHECK_DEBUG);
	return true;
}

///���ƣ�OnGameTimer
///��������ʱ����Ϣ
///@param byDeskStation ��ʱ��λ�á� uTimeID ��ʱ��id ��uTimeCount ��ʱ��ʱ��
///@return  
bool CClientGameDlg::OnGameTimer(BYTE byDeskStation, UINT uTimeID, UINT uTimeCount)
{
	ASSERT(CHECK_DEBUG);
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{			
			if (uTimeCount <= 0)
			{
				//ʱ�䵽��
				return true;
			}

			if (uTimeCount <= (UINT)m_GameBaseInfo.iAlarmTime) 
			{
				if(m_pGameInfo->bEnableSound)
				{
					SetPlaySound(SOUND_WARNING);
				}
			}
			return true;
		}
	case ID_KAIPAI_TIME: //���ƶ�ʱ�� lym0514
		{
			m_PlayView.EnableXiaZhu(false);
			if (uTimeCount <= 0)
			{
				KillTimer(ID_RECT_TIMER);//ɾ����˸��ʱ��
				m_PlayView.m_bShowRect = false ;//����Ӯ�������˸
				//KillGameTimer(ID_KAIPAI_TIME);
				return true;
			}
			if (uTimeCount <= (UINT)m_GameBaseInfo.iAlarmTime) 
			{
				if(m_pGameInfo->bEnableSound)
				{
					SetPlaySound(SOUND_WARNING);
				}
			}
			return true;
		}
	case ID_WAIT_TIME:  //�ȴ���ʱ��
		{
			m_PlayView.EnableXiaZhu(false);
			if (uTimeCount <= 0)
			{
				//KillGameTimer(ID_WAIT_TIME);
				return true;
			}
			m_PlayView.m_bShowResult = false; //�Ƿ���ʾ������Ϣ
			if (uTimeCount <= (UINT)m_GameBaseInfo.iAlarmTime) 
			{
				if(m_pGameInfo->bEnableSound)
				{
					SetPlaySound(SOUND_WARNING);
				}
			}
			return true;
		}
	case ID_XIAZHU_TIME:
		{
			if (uTimeCount <= 0)
			{
				if(m_pGameInfo->bEnableSound)
				{
					TCHAR filename[MAX_PATH+1];
					wsprintf(filename,TEXT("music\\xiazhujieshu.wav"));// ������ע������Ч

					BZSoundStop(SoundbirdID);
					BZSoundPlayGame(this,filename,NULL,1);//lu0820��������
				}
				//m_bCanXiaZhu =false ;		//�ܷ���ע��
				KillGameTimer(ID_XIAZHU_TIME);
				m_PlayView.m_iChooseNote = 0;
				m_PlayView.EnableXiaZhu(false);
				return true;
			}
			if (uTimeCount<=5) 
			{
				if(m_pGameInfo->bEnableSound)
				{
					TCHAR filename[MAX_PATH+1];
					wsprintf(filename,TEXT("music\\Warning.wav"));// ������Ч
					PlayOtherSound(filename);
				}
			}
			if(uTimeCount==16)//���ű�������
			{
				// ��Ч
				if(m_pGameInfo->bEnableSound)
				{
					TCHAR filename[MAX_PATH+1];
					wsprintf(filename,TEXT("music\\xiazhubeijing.wav"));// ������Ч
					SoundbirdID = BZSoundPlay(this,filename,WM_USER,1);	//lu0820��������
				}	
			}

			return true;
		}	
	}
	return true;
}


///���ƣ�KillAllTimer
///������������ж�ʱ��
///@param 
///@return  
void CClientGameDlg::KillAllTimer(void)
{
	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_SEND_CARD_TIMER);
	KillTimer(ID_SENDCARD_INTERVAL);
	KillTimer(PROMPT_CHANGE_NT_TIME);
	memset(m_PlayView.m_uTime,0,sizeof(m_PlayView.m_uTime));
}

///ͬ�⿪ʼ��Ϸ 
bool CClientGameDlg::OnControlHitBegin()
{
	m_PlayView.SetCurOrientation(255);
	m_PlayView.SetWaitingType(WAITING_USER_BEGIN);

	if (((GetStationParameter()==GS_WAIT_SETGAME)
		||(GetStationParameter()==GS_WAIT_NEXT)
		||(GetStationParameter()==GS_WAIT_AGREE))
		&&!m_bWatchMode)
	{
		//SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	// �������
	KillAllTimer();
	ResetGameStation(RS_GAME_BEGIN);
	return true;
}

/// ��ȫ������Ϸ
bool CClientGameDlg::OnControlSafeFinish()
{
	return false;
}

///�ı��Թ�����
void CClientGameDlg::OnWatchSetChange(void)
{
	ASSERT(CHECK_DEBUG);
}

/// �ı��Թ����� ��Ϸ�з��������ݣ����ڸ��½���
void CClientGameDlg::OnWatchSetChange(S_WATCH_SET_CHANGE &sWatchSetChange)
{
	// ���ݱ���
	m_byHandCardCount[sWatchSetChange.byDeskStation] = sWatchSetChange.byCardCount;
	memcpy(m_wHandCard[sWatchSetChange.byDeskStation], sWatchSetChange.wCardList, sWatchSetChange.byCardCount*sizeof(WORD));
	BYTE byCardCount = sWatchSetChange.byCardCount;
	if (GS_SEND_CARD == GetStationParameter())
	{
		byCardCount = m_bySentCardCount[sWatchSetChange.byDeskStation];
	}

	// ������ͼ
	BYTE iViewSeat = GetViewSeat(sWatchSetChange.byDeskStation);
	m_PlayView.m_HandCard[iViewSeat].SetCard(m_wHandCard[sWatchSetChange.byDeskStation], NULL, byCardCount);
}

///�汾�˶�
bool CClientGameDlg::CheckVersion(BYTE byHighVersion,BYTE byLowVersion,TCHAR szUpDateHttp[100])
{
	if (NULL == m_pGameInfo)
	{
		PostMessage(WM_CLOSE,0,0);
		return false;
	}
	TCHAR szGameName[100]={0};
	CString strBuffer;
	_stprintf_s(szGameName,sizeof(szGameName), _T("���%s��Ϸ�汾���ɣ���Ҫ�������������°汾��"),m_pGameInfo->szGameName);
	if((byHighVersion != m_byHighVersion) || (byLowVersion!=m_byLowVersion))
	{
		TCHAR sz[200];
		wsprintf(sz,_T("�汾��ͻ,��ǰ�������汾��%d-%d,��İ汾��%d-%d"),byHighVersion,byLowVersion,m_byHighVersion,m_byLowVersion);
		WriteStr(sz);
	}	
	if(byHighVersion != m_byHighVersion)
	{
		if (IDOK == AFCMessageBox(szGameName,m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON1|MB_ICONINFORMATION)) 
		{
			strBuffer.Format(szUpDateHttp,1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	if(byLowVersion!=m_byLowVersion)
	{
		if (IDOK == AFCMessageBox(szGameName,m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON1|MB_ICONINFORMATION)) 
		{
			strBuffer.Format(szUpDateHttp,1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	return TRUE;
}

/// ��Ϸ����
void CClientGameDlg::OnGameSetting()
{
#ifdef SUPER_VERSION
	//CCardSet	*dlg = new CCardSet();
	//dlg->Create(IDD_CARD_SET,this);
	//dlg->CenterWindow();
	//dlg->ShowWindow(SW_SHOW);
	CCardSet CardSetDlg(this);
	if (IDOK == CardSetDlg.DoModal())
	{
		return ;
	}
#endif

		m_PlayView.OnShowCursor(TRUE);

		// ��Ϸ����
		CGameSet GameSetDlg(this);
		GameSetDlg.m_bSound = m_pGameInfo->bEnableSound;
		GameSetDlg.m_bShowUser = m_pGameInfo->bShowUserInfo;	
		GameSetDlg.m_bEnableWatch = m_pGameInfo->bEnableWatch;
		//m_byViewOrder = (ID_VIEW_ANTICLOCKWISE == GetProfileInt(TEXT("ViewOrder"),ID_VIEW_ANTICLOCKWISE))?ID_VIEW_ANTICLOCKWISE: ID_VIEW_CLOCKWISE;
		//GameSetDlg.m_byClockwise = m_byViewOrder;
		if (GameSetDlg.DoModal()==IDOK)
		{
			//�������
			bool bSendWatch=((!m_bWatchMode)&&(m_pGameInfo->bEnableWatch != GameSetDlg.m_bEnableWatch));

			// ��������
			m_pGameInfo->bShowUserInfo=GameSetDlg.m_bShowUser;
			m_pGameInfo->bEnableWatch=GameSetDlg.m_bEnableWatch;
			m_pGameInfo->bEnableSound=GameSetDlg.m_bSound;
			WriteProfileInt(TEXT("ViewOrder"),(int)m_byViewOrder);

			//��������
			if (bSendWatch)
			{
				MSG_GM_WatchSet WatchSet;			
				memset(&WatchSet,0,sizeof(WatchSet));
				WatchSet.dwUserID=0;
				SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,GameSetDlg.m_bEnableWatch?TRUE:FALSE);
			}
			if(!m_pGameInfo->bEnableSound)
			{
				if(SoundbirdID>0)
				{
					BZSoundStop(SoundbirdID);
					SoundbirdID = 0;
				}
			}
			else 
			{
				if(GetStationParameter() == GS_SEND_CARD)
				{
					//����ע������
					TCHAR filename[MAX_PATH+1];
					wsprintf(filename,TEXT("music\\xiazhubeijing.wav"));// ������Ч
					SoundbirdID = BZSoundPlay(this,filename,WM_USER,1);//lu0820��������
				}
			}
		}
		if (GetStationParameter() == GS_SEND_CARD)
		{
			if (m_PlayView.m_iChooseNote > 0)
			{
				m_PlayView.OnShowCursor(FALSE);
			}
		}
}

//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)//��Ϸ��ʼ
{

}

/// ������һ�λ��������ʱ�롢˳ʱ�����
void CClientGameDlg::OnBeforeUpdateViewSeat()
{
	// �����ݸ���һ��
	int i=0;
	int iViewSeat = 255;
	for(i=0; i<PLAY_COUNT; ++i)
	{
		iViewSeat = GetViewSeat(i);
		m_PlayView.m_HandCard[iViewSeat].SetCard(m_wHandCard[i], NULL, m_byHandCardCount[i]);
		m_PlayView.SetTrustee(iViewSeat, m_bIsTrustee[i]);
	}
	iViewSeat = 255;
	if (m_byCurOrientation<PLAY_COUNT)
	{
		iViewSeat = GetViewSeat(m_byCurOrientation);
	}
	m_PlayView.SetCurOrientation(iViewSeat);
}

           
///���ƣ�OnXiaZhu
///�����������ע��Ϣ
///@param  wparam ��Ϣ������ lparam ��Ϣ����
///@return  
LRESULT	CClientGameDlg::OnXiaZhu(WPARAM wparam, LPARAM lparam)
{
	if(m_PlayView.m_iMyZhu[8]>=m_PlayView.m_iMaxNote && m_PlayView.m_iMaxNote>0)
	{
		char szMessage[MAX_PATH] = {0};
		//CString strMaxNoteMoney;
		//if (m_PlayView.m_iMaxNote > 0)
		{
			//m_PlayView.NumToChar(m_PlayView.m_iMaxNote,&strMaxNoteMoney);
			//_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ�������ע�Ѵﵽ���ޣ�Ŀǰ������ע")); 
			//m_MessageHandle.InsertNormalMessage(szMessage);
		}
		m_PlayView.OnRestoreCursor();
		m_PlayView.m_iChooseNote = 0;
		m_PlayView.UpdateViewFace(NULL);
		return 0;
	}

	int type=0;
	int moneytype=0;
	moneytype = int(wparam);
	type = int(lparam);
	__int64 money=0;
	if(moneytype==1)
	{
		money = 100;
	}
	else if(moneytype==2)
	{
		money = 1000;
	}
	else if(moneytype==3)
	{
		money = 10000;
	}
	else if(moneytype==4)
	{
		money = 100000;
	}
	else if(moneytype==5)
	{
		money = 1000000;
	}
	else if(moneytype==6)
	{
///72��һ��
#ifdef SET72 
		/*if(m_PlayView.m_bUse500WJeton)
		{
		money=5000000;
		}
		else
		{      */ 		
		money = 10000000;
		//}
		
#else
		money = 5000000;
#endif
	}
	///20091215wys �޸İ���
	__int64 nTempMoney=1; 
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nTempMoney*=10;
	}
	money = money/nTempMoney;

	CString str="";
	str.Format("���ڲ��� %d  %d  %d",Glb().m_nPowerOfGold,nTempMoney,money);
	OutputDebugString(str);
	///�޸İ���
	// ������ע��Ϣ
	XIA_ZHU  xiazhu;
	//S_TrusteeCard.byDeskStation = GetMeUserInfo()->bDeskStation;
	xiazhu.money = money;
	xiazhu.moneytype = moneytype;
	xiazhu.type = type;
	xiazhu.m_nPowerOfGold = nTempMoney;//�Ŵ���

	SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
	
	return 0;
}

///���ƣ�OnShangZhuang
///�������������ׯ������Ϣ
///@param  wparam ��Ϣ������ lparam ��Ϣ����
///@return  
LRESULT	CClientGameDlg::OnShangZhuang(WPARAM wparam, LPARAM lparam)
{
	int type=0;
	type = int(lparam);
	SHANG_ZHUANG zhuang;
	if(type==0)//��ׯ
	{
		zhuang.shang=true;
		// ��������ׯ��Ϣ
		SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
	}
	else if(type==1)//��ׯ
	{
		zhuang.shang=false;
		// ��������ׯ��Ϣ
		SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
	}
	else if(type==10)
	{

		TCHAR szMessage[100]={0};
		if (1 == m_PlayView.m_iMessageCount)
		{
			_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ���뱣��һ����ʱ�����������ׯ��ť!"));
		}
		else
		{
			_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ���벻ҪƵ���������ׯ��ť!"));
		}
		
		m_MessageHandle.InsertNormalMessage(szMessage);

	}
	else 
	{
		TCHAR szMessage[100]={0};

		CString str="";
		char szChar[MAX_PATH] = { 0 } ; 

   		//m_PlayView.NumToChar(m_PlayView.m_iLimitMoney,&str);
		m_PlayView.ChangeNumToCStringByPower(m_PlayView.m_iLimitMoney ,true ,"," , str) ;

		_stprintf_s(szMessage, sizeof(szMessage), TEXT("��ׯʧ�ܣ���ׯ������Ҫ %s ��ҡ�"),str);
		m_MessageHandle.InsertNormalMessage(szMessage);
		m_PlayView.SetApplyBanker(true);
		OutputDebugString("��ׯʧ��2");
	}


	//AfxMessageBox("shang");
	return 0;
}

/// ������Ϣ����
LRESULT	CClientGameDlg::OnTest(WPARAM wparam, LPARAM lparam)
{
	BJL_TEST test;
	test.type = (int)lparam;
	//AfxMessageBox("test1");
	SendGameData(&test, sizeof(test), MDM_GM_GAME_NOTIFY, ASS_BJL_TEST,0);

	return 0;
}

///���ƣ�OnSuperCtr
///������������ҿ�����Ϣ,���Կ��ƿ�ׯ�к�
///@param  wparam ��Ϣ������ lparam ��Ϣ����
///@return  
LRESULT	CClientGameDlg::OnSuperCtr(WPARAM wparam, LPARAM lparam)
{
	SUPERSETTING super;
	super.bState = int(wparam);
	SendGameData(&super, sizeof(super), MDM_GM_GAME_NOTIFY, ASS_SUPER_SETTING,0);

	return 0;
}

///���ƣ�OnBank
///��������������Ϣ
///@param  wparam ��Ϣ������ lparam ��Ϣ����
///@return  
LRESULT	CClientGameDlg::OnBank(WPARAM wparam, LPARAM lparam)
{
	OnVisitBank();
	return 0;
}
///���ƣ�OnArgeeUserLeft
///�������û������뿪
///@param 
///@return 
LRESULT	CClientGameDlg::OnSendAICtrl(WPARAM wparam, LPARAM lparam)
{
	CString str="";
	str.Format("������Ϣ��������Ϣ,Ŀ��%d",m_PlayView.m_ShowAIInfo.m_GoalMoney);
	OutputDebugString(str);	
	tagAICtrlInfo	m_SendAIInfo;	//��������Ϣ
	m_SendAIInfo.bAIWinAndLostAutoCtrl = m_PlayView.m_ShowAIInfo.bAIWinAndLostAutoCtrl;//��������
	m_SendAIInfo.bIsClearAIWinMoney = m_PlayView.m_ShowAIInfo.bIsClearAIWinMoney;//��ӮǮ����
	m_SendAIInfo.bIsSetAICtrl = m_PlayView.m_ShowAIInfo.bIsSetAICtrl;//�޸�����
	m_SendAIInfo.iAIWantWinMoney = m_PlayView.m_ShowAIInfo.m_GoalMoney;
	m_SendAIInfo.iAIMaxWinInOne = m_PlayView.m_ShowAIInfo.m_iAveMoney;

	//�Ϳ�����Ϣ
	SendGameData(&m_SendAIInfo, sizeof(m_SendAIInfo), MDM_GM_GAME_NOTIFY, ASS_SUPER_AI_INFO,0);
	return 0;
}
///���ƣ�HandleGameMessage
///��������Ϸ��Ϣ���մ�����
///@param  pNetHead ��Ϣͷ�� pNetData ��Ϣ���� ��uDataSize ������
///@return  
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	CString fdsa;
	fdsa.Format("MSG_%d",pNetHead->bAssistantID);
	OutputDebugString(fdsa);



	if (MDM_GM_GAME_NOTIFY == pNetHead->bMainID && m_bInitFinish)//�Ƿ��ʼ�����
	{
		switch (pNetHead->bAssistantID )
		{
		case ASS_SUPER_STATE: // �ж��Ƿ�Ϊ�����ͻ���
			{
				if (sizeof(SUPERSTATE) > uDataSize)
				{
					return false;
				}
				SUPERSTATE *pSuper = (SUPERSTATE *)pNetData;
				if (GetMeUserInfo()->bDeskStation == pSuper->bDeskStation)
				{
					m_bSuperState = pSuper->bEnable;
					if(m_bSuperState==TRUE)///�����ͻ�����Ϣ���ذ�ť
					{
						
						///��ʾ�����Ի���İ�ť
						m_PlayView.m_btnCtrSuper.ShowWindow(SW_SHOW);
						m_PlayView.m_btnCtrSuper.EnableWindow(true);

						//����Ƿ��ǻ�����
						for(int i = 0; i < PLAY_COUNT; ++i)
						{
							if(NULL != m_pUserInfo[i])
							{
								m_PlayView.m_SuperData.sUserInfo[i].bIsRobot = m_pUserInfo[i]->GameUserInfo.isVirtual;
							}
						}

						m_PlayView.GetAllUserNicheng();
						for(int i=0;i<PLAY_COUNT;++i)
						{
							m_PlayView.m_SuperData.sUserInfo[i].iOldMoney = pSuper->m_iOldMomey[i];
						}
						if(m_bSuperState==TRUE)///�����ͻ�����Ϣ���ذ�ť
						{
							m_PlayView.m_SuperDlg.UpdateListData(&m_PlayView.m_SuperData);
						}
						m_PlayView.m_bShowSuper = true;	///��ʾ�����ͻ��˿�����Ϣ
						m_PlayView.m_sSuperInfo = "";	///�����ͻ��˿�����Ϣ
					}
				}
				return true;
			}
		case ASS_SUPER_AI_INFO:///��������Ӯ���ƣ��յ�������Ϣ
			{
				if (sizeof(AICTRLINFO) > uDataSize)
				{
					return false;
				}
				AICTRLINFO *pAIInfo = (AICTRLINFO *)pNetData;

				m_PlayView.m_ShowAIInfo.m_AIInfo.bDeskStation = pAIInfo->bDeskStation;			//����λ��
				m_PlayView.m_ShowAIInfo.m_AIInfo.bAIWinAndLostAutoCtrl = pAIInfo->bAIWinAndLostAutoCtrl;//�Ƿ�����������Ӯ���Զ�����
				m_PlayView.m_ShowAIInfo.m_AIInfo.iAICtrlCount = pAIInfo->iAICtrlCount;		//�Ѿ����п��Ƶİ���
				m_PlayView.m_ShowAIInfo.m_AIInfo.iAIWantWinMoney = pAIInfo->iAIWantWinMoney;//�����˵�ӮǮĿ��
				m_PlayView.m_ShowAIInfo.m_AIInfo.iAIHaveWinMoney = pAIInfo->iAIHaveWinMoney;//�������Ѿ�Ӯ��Ǯ
				m_PlayView.m_ShowAIInfo.m_AIInfo.iAIMaxWinInOne = pAIInfo->iAIMaxWinInOne;	//������ƽ��ÿ��ӮǮ�����ֵ
				m_PlayView.m_ShowAIInfo.InvalidateRect(NULL,true);

				CString str="";
				str.Format("������Ϣ:����%d  Ŀ��%d  ����%d  Ӯ%d ƽ��%d",m_PlayView.m_ShowAIInfo.m_AIInfo.bAIWinAndLostAutoCtrl,m_PlayView.m_ShowAIInfo.m_AIInfo.iAIWantWinMoney,m_PlayView.m_ShowAIInfo.m_AIInfo.iAICtrlCount,m_PlayView.m_ShowAIInfo.m_AIInfo.iAIHaveWinMoney,m_PlayView.m_ShowAIInfo.m_AIInfo.iAIMaxWinInOne);
				OutputDebugString(str);
				OutputDebugString("������Ϣ���ͻ����յ�������Ϣ");
				return true;
			}
	case ASS_SUPER_SETTING:  /// ��Ӯ������Ϣ
		{
			if (uDataSize != sizeof(SUPERSETTING))
			{
				return true;
			}
			OnHandleSuperCtr(pNetData, uDataSize);
	
			return true;
		}
		case ASS_UPDATE_CLIENT_VIEW:
			{
				return OnNotifyUpdateClientView(pNetData, uDataSize);
			}
		//case ASS_SEND_CARD:			///����������
		//	{
		//		return OnNotifySendCard(pNetData, uDataSize);
		//	}
		case ASS_GAME_BEGIN:		// ����������Ϸ��ʼ
			{
				if (sizeof(GAME_BEGIN) > uDataSize)
				{
					WriteInfo(_T("ASS_GAME_BEGIN"));
					return false;
				}
				SetStationParameter(GS_PLAY_GAME);
				m_PlayView.m_bNowGameState = GS_PLAY_GAME;/// ��ǰ��Ϸ״̬

				GAME_BEGIN *psGameBegin = (GAME_BEGIN *)pNetData;
				UPDATE_CLIENT_VIEW sUpdateClientView;
				memcpy(&sUpdateClientView.abySeatID, &psGameBegin->abySeatID, sizeof(sUpdateClientView.abySeatID));
				OnNotifyUpdateClientView((void*)&sUpdateClientView, sizeof(sUpdateClientView));
								
				return TRUE;
			}
		case ASS_WATCH_SET_CHANGE:
			{
				if (sizeof(S_WATCH_SET_CHANGE) > uDataSize)
				{
					WriteInfo(_T("ASS_WATCH_SET_CHANGE"));
					return true;
				}
				S_WATCH_SET_CHANGE *psWatchSetChange = (S_WATCH_SET_CHANGE *)pNetData;
				if (psWatchSetChange->byDeskStation >= PLAY_COUNT)
				{
					return true;
				}
				OnWatchSetChange(*psWatchSetChange);
				return true;
			}
		case ASS_CUT_END:	///�û�ǿ���뿪
		case ASS_SAFE_END:
			{
				return OnNotifyCutEnd(pNetHead, pNetData, uDataSize);
			}
		case ASS_XIA_ZHU:			// ��ע��Ϣ
			{
				if (sizeof(XIA_ZHU) > uDataSize)
				{
					WriteInfo(_T("ASS_XIA_ZHU"));
					return false;
				}

				XIA_ZHU xiazhu;
				CopyMemory(&xiazhu, (XIA_ZHU *)pNetData, sizeof(XIA_ZHU));
				m_ArrNote.Add(xiazhu);
				
				/// �����¼�
				g_Event.SetEvent();
				///20100207zht�޸İ���
				int nTempMoney=1; 
				for(int i=0; i<Glb().m_nPowerOfGold; i++)
				{
					nTempMoney*=10;
				}
				__int64 money = 100000/nTempMoney;  
				if (xiazhu.money < money)
				{
					if (m_PlaySound)
					{
						m_PlaySound = false;
						SetTimer(ID_PLAY_SOUND, 60, NULL);
						m_ArrPlay.Add(xiazhu);
						g_EventPlay.SetEvent();
					}
				}
				else
				{					
					OnHandleXiazhuPlaySound(pNetData, uDataSize);
				}

				memcpy(m_PlayView.m_SuperData.sUserInfo[xiazhu.station].iUserNote,xiazhu.m_iUserXiaZhu,sizeof(xiazhu.m_iUserXiaZhu));//������ڸ����������ע
				//m_PlayView.m_SuperData.sUserInfo[xiazhu.station].iUserNote[xiazhu.type] += xiazhu.money;//�����������ע��Ǯ
				m_PlayView.m_SuperData.sUserInfo[xiazhu.station].iAllNote = 0;
				for(int i=0;i<8;++i)
				{
					m_PlayView.m_SuperData.sUserInfo[xiazhu.station].iAllNote += m_PlayView.m_SuperData.sUserInfo[xiazhu.station].iUserNote[i];
				}
				if(m_bSuperState==TRUE)///�����ͻ�����Ϣ���ذ�ť
				{
					m_PlayView.m_SuperDlg.UpdateListData(&m_PlayView.m_SuperData);
				}

				return true;
			}
		case ASS_KAI_PAI:			// ������Ϣ
			{
				if (sizeof(KAI_PAI) > uDataSize)
				{
					WriteInfo(_T("ASS_KAI_PAI"));
					return false;
				}
				m_PlayView.SetPromptOpenCard(false);
				OnHandleKaiPai(pNetData, uDataSize);
				//MsgPrint("zhangjing::������");
				
				
				return true;
			}
		case ASS_USER_SCORE:
			{
				if(sizeof(USER_SCORE_STRUCT) >uDataSize)
				{
					return false ; 
				}

				USER_SCORE_STRUCT *UserScoreData = (USER_SCORE_STRUCT *)pNetData ; 
	
				CString strRetultInfo ; 
				strRetultInfo.Format("\n���ֽ�����������������������������") ;
				m_MessageHandle.InsertNormalMessage(strRetultInfo.GetBuffer()) ;

				for(int i = 0 ; i<PLAY_COUNT ; i++)
				{
					if(NULL != m_pUserInfo[i]&&true == UserScoreData->bShowInfo[i])
					{
						strRetultInfo.Format("���%s���÷�%I64d" , m_pUserInfo[i]->GameUserInfo.nickName ,UserScoreData->llUserFen[i]) ;
						m_MessageHandle.InsertNormalMessage(strRetultInfo.GetBuffer()) ; 
					}
				}

				m_MessageHandle.InsertNormalMessage("���ֽ�����������������������������") ;

				return true ; 
			}
		case ASS_SHOW_JS:
			{
				//
				//if (sizeof(JIE_SUAN) > uDataSize)
				//{
				//	WriteInfo(_T("ASS_JIE_SUAN"));
				//	MsgPrint("zhangjing::���������");
				//	return false;
				//}
				//MsgPrint("zhangjing::������");
				OnHandleJieSuan(pNetData, uDataSize);
				
				return true;
			}
		case ASS_DENG_DAI:			// �ȴ���ϢGameOver.wav
			{
				if (sizeof(DENG_DAI) > uDataSize)
				{
					WriteInfo(_T("ASS_DENG_DAI"));
					return false;
				}
				DENG_DAI *pWait = (DENG_DAI *)pNetData;
				KillTimer(ID_RECT_TIMER);		// ɾ����˸��ʱ��
				KillGameTimer(ID_KAIPAI_TIME);	// ɾ�����Ƽ�ʱ��
				KillTimer(ID_SEND_CARD_TIMER);
				KillTimer(ID_SENDCARD_INTERVAL);
				SetGameTimer( GetMeUserInfo()->bDeskStation, m_iFreeTime, ID_WAIT_TIME);//�������еȴ���ʱ��

				SetStationParameter(GS_WAIT_NEXT);				// ��Ϸ��Ϊ��ʼ״̬
				m_PlayView.Init();
				m_PlayView.m_bNowGameState = GS_WAIT_NEXT;		// ��ǰ��Ϸ״̬
				m_PlayView.EnableXiaZhu(false);					// ������ע
				m_PlayView.m_iShowCard = 0;						// ��ʾ�Ƶ�����
				m_PlayView.m_Result.ShowWindow(false);
				m_PlayView.m_bShowResult = false;				// �Ƿ���ʾ������Ϣ

				for(int i=0;i<=PLAY_COUNT;i++)
				{
					m_PlayView.m_iZhuangList[i] = pWait->zhuangstation[i];
				}
				if(m_PlayView.m_iNTju>=0)
				{
					m_PlayView.m_iNTju = pWait->m_iZhuangBaShu;		// ׯ�ҵľ���
					m_PlayView.m_iNTwin = pWait->m_iNtWin;			// ׯ�ҵĳɼ�
				}

				if(m_pGameInfo->bEnableSound)
				{
					TCHAR filename[MAX_PATH+1];
					wsprintf(filename,TEXT("music\\GameOver.wav"));	// ������ע������Ч
					BZSoundPlayGame(this,filename,NULL,1);			// lu0820��������
				}
				m_PlayView.UpdateViewFace(NULL);
				//�������Ϣ��������������
				return true;
			}
		case ASS_SHANG_ZHUANG:      // ��ׯ��Ϣ
			{
				if (sizeof(SHANG_ZHUANG) > uDataSize)
				{
					WriteInfo(_T("ASS_SHANG_ZHUANG"));
					return false;
				}
				
				SHANG_ZHUANG *pSZ = (SHANG_ZHUANG *)pNetData;
				for(int i=0;i<=PLAY_COUNT;i++)
				{
					m_PlayView.m_iZhuangList[i] = pSZ->zhuangstation[i];
				}
						OutputDebugString("������ׯ����2222");
				int nt = m_PlayView.m_iNowNTstation;
				if(pSZ->m_iNowNtStation>=0)
				{
					m_PlayView.m_iNowNTstation = pSZ->m_iNowNtStation;
					if(nt<0)
					{
						m_PlayView.m_iNTju = -1;
						m_PlayView.m_iNTwin = 0;
					}
				}
				if(m_PlayView.m_iNowNTstation>=0 && m_PlayView.m_bNowGameState == GS_SEND_CARD && pSZ->success == 0)//��ǰ��Ϸ״̬:��ע״̬
				{
					OutputDebugString("������ׯ����1111");
					if(nt<0 &&  pSZ->bXiaZhu)//��ע״̬
					{
						for(int i=0;i<8;i++)
						{
							m_PlayView.m_iMaxZhu[i] = pSZ->m_iMaxZhu[i];
						}
						if(m_pUserInfo[m_PlayView.m_iNowNTstation]!=NULL)
						{
							m_PlayView.m_iNTmoney =m_pUserInfo[m_PlayView.m_iNowNTstation]->GameUserInfo.i64Money;//ׯ�ҵĽ��
						}
						if(m_PlayView.m_iNowNTstation != m_PlayView.m_byMyStation)
						{//����ׯ�Ҳ�����ע
							m_PlayView.EnableXiaZhu(true);	//������ע
						}
						else
						{
							m_PlayView.EnableXiaZhu(false);	//
						}

						if(!m_PlayView.CheckMeZhuang())
						{
							m_PlayView.SetApplyBanker(true);
						}
						else
						{
							m_PlayView.SetApplyBanker(false);
						}
						m_PlayView.m_sSuperInfo = "";			///�����ͻ��˿�����Ϣ
						m_PlayView.m_bShowSuper = false;		//��ʾ�����ͻ��˿�����Ϣ

						m_PlayView.SetXiaZhuBtn();
						m_PlayView.UpdateViewFace(NULL);

						OutputDebugString("������ׯ����0000");
					}
				}
				CString cst;
				cst.Format("wyslog::pSZ->success===%d,pSZ->shang==%d",pSZ->success,pSZ->shang);
				OutputDebugString(cst);

				if(pSZ->success !=0 && m_PlayView.m_iLimitMoney>0 && pSZ->shang)
				{
					TCHAR szMessage[100]={0};
					CString str="",str2="";
					char szChar[MAX_PATH] = {0} ; 
					//m_PlayView.NumToChar(m_PlayView.m_iLimitMoney,&str);
					m_PlayView.ChangeNumToCStringByPower(m_PlayView.m_iLimitMoney ,true ,"," ,str) ;
					//m_PlayView.NumToChar(1500000000000000000,&str2);
					m_PlayView.ChangeNumToCStringByPower(1500000000000000000 ,true ,"," ,str2) ;

					if(pSZ->success!=2)
					{	
						CString CST;
						CST.Format("wysout 1111111111�ͻ��˽�����Ϣ %s",GetMeUserInfo()->nickName);
						OutputDebugString(CST);
						if (GetMeUserInfo()->bDeskStation == pSZ->station)
						{

							if(GetMeUserInfo()->i64Money < m_PlayView.m_iLimitMoney)
							{
								_stprintf_s(szMessage, sizeof(szMessage),TEXT("����ʾ����ׯʧ��,��ׯ����%s%s"), str, m_MoneyType);
								m_MessageHandle.InsertNormalMessage(szMessage);
							}
							//if( GetMeUserInfo()->i64Money >= 1500000000000000000)
							//{
							//	_stprintf_s(szMessage, sizeof(szMessage),TEXT("����ʾ����ׯʧ��,��ׯ��Χ��%s����"), str2);
							//	m_MessageHandle.InsertNormalMessage(szMessage);
							//}
						}

						m_PlayView.SetApplyBanker(true);
					}
					else 
					{//���ڶ�����
						if (GetMeUserInfo()->bDeskStation == pSZ->station)
						{
							_stprintf_s(szMessage, sizeof(szMessage),TEXT("����ʾ��������ׯ�Ҷ����У������ĵȴ�"));
							m_MessageHandle.InsertNormalMessage(szMessage);
						}						
						m_PlayView.SetApplyBanker(false);
					}
					
					OutputDebugString("��ׯʧ��1");
				}
				else if(pSZ->shang && m_PlayView.m_byMyStation == pSZ->station)
				{//������ׯ�ɹ�
					TCHAR szMessage[100]={0};
					_stprintf_s(szMessage, sizeof(szMessage),TEXT("����ʾ����������ׯ�ɹ�"));
					m_MessageHandle.InsertNormalMessage(szMessage);
					m_PlayView.SetApplyBanker(false);
				}
				m_PlayView.UpdateViewFace(NULL);
				return true;
			}
		case ASS_CHANGE_NT:		//��ׯ��Ϣ
			{
				m_PlayView.SetPromptChangeNt(true);	///���û�ׯ��ʾͼƬ
				SetTimer(PROMPT_CHANGE_NT_TIME,1*1000,NULL);	///������ʾ ��ׯͼƬ��ʱ�䶨ʱ��
				return true;
			}
		case ASS_BJL_BEGIN://�ͻ����յ��ټ��ֿ�ʼ��Ϣ
			{
				OnHandleBegin(pNetData, uDataSize);
				for(int i=0;i<PLAY_COUNT;++i)
				{
					m_PlayView.m_SuperData.sUserInfo[i].iAllNote = 0;
					memset(m_PlayView.m_SuperData.sUserInfo[i].iUserNote,0,sizeof(m_PlayView.m_SuperData.sUserInfo[i].iUserNote));
				}
				m_PlayView.GetAllUserNicheng();//��ȡ������ҵ��ǳ�
				if(m_bSuperState==TRUE)///�����ͻ�����Ϣ���ذ�ť
				{
					m_PlayView.m_SuperDlg.UpdateListData(&m_PlayView.m_SuperData);

					m_PlayView.m_SuperDlg.UpDataNoteRateInfo(0, 0, 0) ; 
				}

				return true;
			}
		case ASS_WU_ZHUANG://�յ���ׯ�ȴ���Ϣ
			{
				if (sizeof(DENG_DAI) > uDataSize)
				{
					WriteInfo(_T("ASS_WU_ZHUANG"));
					return false;
				}
				DENG_DAI *pWait = (DENG_DAI *)pNetData;

				SetStationParameter(GS_WAIT_AGREE);//��Ϸ��Ϊ��ʼ״̬
				m_PlayView.m_bNowGameState = GS_WAIT_AGREE;//��ǰ��Ϸ״̬
				m_PlayView.m_iNowNTstation=-1;
				m_PlayView.Init();//��ʼ����ͼ����
				m_PlayView.m_Result.ShowWindow(false);
				
				for(int i=0;i<=PLAY_COUNT;i++)
				{
					m_PlayView.m_iZhuangList[i] = pWait->zhuangstation[i];
				}

				m_PlayView.m_iNTju = pWait->m_iZhuangBaShu;//ׯ�ҵľ���
				m_PlayView.m_iNTwin = pWait->m_iNtWin;//ׯ�ҵĳɼ�
				m_PlayView.UpdateViewFace(NULL);

				return true;
			}
		case ASS_APPLY_GAME_DATA: // ��������ʱ������Ϸ������Ϣ��lym2010-03-18
			{
				if (sizeof(CHONG_LIAN) > uDataSize)
				{
					WriteInfo(_T("ASS_WU_CHONGLIAN"));
					return false;
				}
				//������Ϸ����
				OnSetGameData(pNetData, uDataSize);

				return true;
			}
		case ASS_XIA_ZHU_FULL:	//��ע������Ϣ add xyh 2011-12-6
			{
				m_PlayView.SetPromptOpenCard(true);
				return true;
			}
		default:
			break;
		}
	}
	//return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize);
}

bool CClientGameDlg::OnNotifyUpdateClientView(void * pNetData, UINT uDataSize)
{
	if (sizeof(UPDATE_CLIENT_VIEW) > uDataSize)
	{
		WriteInfo(_T("CClientGameDlg::OnNotifyUpdateClientView"));
		return false;
	}
	// �����λ��
	UPDATE_CLIENT_VIEW *psUpdateClientView = (UPDATE_CLIENT_VIEW *)pNetData;
	memcpy(&m_abySeatID, &psUpdateClientView->abySeatID, sizeof(m_abySeatID));

	// ��������
	OnBeforeUpdateViewSeat();

	// �˺��������и��½���
	m_PlayView.SetWaitingType(NO_WAITING);
	return true;
}
//
///// ����������
//bool CClientGameDlg::OnNotifySendCard(void * pNetData, UINT uDataSize)
//{
//	if (sizeof(BEGIN_SEND_CARD) > uDataSize)
//	{
//		WriteInfo(_T("CClientGameDlg::OnNotifySendCard"));
//		return false;
//	}
//	SetStationParameter(GS_SEND_CARD);
//
//
//	// ��ʾ���ƶ���������ֱ�ӷ��͸�ÿ�����	
//	BEGIN_SEND_CARD *psSendCard = (BEGIN_SEND_CARD *)pNetData;
//	m_PlayView.m_HandCard[GetViewSeat(GetMeUserInfo()->bDeskStation)].SetUseMouse(false);
//	
//	int i=0;
//	m_wSendCount = 0;
//	for (i=0; i<PLAY_COUNT; ++i)
//	{
//		memset(m_wHandCard, 0, sizeof(m_wHandCard));            // ��������е���
//		m_byHandCardCount[i] = psSendCard->byUserCardCount[i];  // ��������е�������
//		m_wSendCount += m_byHandCardCount[i];
//	}
//
//	// �����Լ�����		
//	memcpy(&m_wHandCard[GetMeUserInfo()->bDeskStation], psSendCard->wUserCard, (m_byHandCardCount[GetMeUserInfo()->bDeskStation] * sizeof(WORD)));
//
//	// �Ѿ����͵�����
//	m_wCurSendPos = psSendCard->wSentCount;
//	if (m_wCurSendPos >= m_wSendCount)
//	{
//		OnNotifySendCardFinish();
//		return true;
//	}
//
//	// ��ʾһ������
//	m_PlayView.m_MingCard.SetCard(&psSendCard->wMingCard, NULL, 1);
//
//	// ��ʾ�м�һ����
//	WORD wCardList[POKER_CARD_NUM]={0};
//	WORD wPkCardCount = m_wSendCount-m_wCurSendPos;
//	WORD wPKCardCountView = 0x00;   // ��ʾ��ֵ
//	if (wPkCardCount > (POKER_CARD_NUM/2))
//	{
//		wPKCardCountView = wPkCardCount;   // ��ʾ��ֵ
//		wPkCardCount = POKER_CARD_NUM/2;
//	}	
//	m_PlayView.m_SendBaseCard.SetCard(wCardList, NULL, (BYTE)wPkCardCount, (BYTE)wPKCardCountView);
//
//	// �������ƶ���
//	memset(&m_bySentCardCount, 0, sizeof(m_bySentCardCount)); // ������Ѿ����˵�������
//	m_byFirstSendPos = psSendCard->byFirstSendPos;            // ��һ���������
//
//	// ���������λ��
//	// ���Ѿ����˵�����ʾ�ڽ�����		
//	BYTE byTemp = (BYTE)(m_wCurSendPos/PLAY_COUNT);               // ÿ�������ٷ��͵���
//	BYTE byTemp1 = (BYTE)(m_wCurSendPos%PLAY_COUNT);              // ��������ٴν��з���
//	BYTE byNextSendPos = m_byFirstSendPos;
//
//	for (i=0; i<PLAY_COUNT; ++i)
//	{
//		m_bySentCardCount[i] += byTemp;
//		if (byTemp1 > 0)
//		{
//			// �Ե�һ���������Ϊ�������з���
//			int iSendPos = m_byFirstSendPos;
//			if (ID_VIEW_ANTICLOCKWISE == m_GameBaseInfo.bySendCardOrder)
//			{
//				iSendPos = (iSendPos + PLAY_COUNT - i)%PLAY_COUNT;
//				byNextSendPos = (iSendPos + PLAY_COUNT - 1)%PLAY_COUNT;
//			}
//			else
//			{
//				iSendPos = (iSendPos + i)%PLAY_COUNT;
//				byNextSendPos = (iSendPos + 1)%PLAY_COUNT;
//			}
//			m_bySentCardCount[iSendPos]++;
//			byTemp1 --;
//		}		
//	}
//	
//	for (i=0; i<PLAY_COUNT; ++i)
//	{
//		m_PlayView.m_HandCard[GetViewSeat(i)].SetCard(m_wHandCard[i], NULL, m_bySentCardCount[i]);
//	}
//
//	// ��ʾ��һλ��ҵĶ���
//	//int iViewSeatNextSend = GetViewSeat(byNextSendPos);
//	//m_PlayView.SendCard(iViewSeatNextSend, m_GameBaseInfo.iSendCardInterval);
//	//SetTimer(ID_SEND_CARD_TIMER, m_GameBaseInfo.iSendCardInterval, NULL);
//	return true;
//}
//
///// ����һ����
//void CClientGameDlg::OnNotifySendACard(void) 
//{
//	if (m_wCurSendPos > m_wSendCount)
//	{
//		return ;
//	}
//
//	// �Ե�һ���������Ϊ�������з���
//	BYTE bySendPos = m_byFirstSendPos;
//	BYTE byNextSendPos = bySendPos;
//	BYTE byTemp = (BYTE)(m_wCurSendPos%PLAY_COUNT);
//	if (ID_VIEW_ANTICLOCKWISE == m_GameBaseInfo.bySendCardOrder)
//	{
//		bySendPos = (bySendPos + PLAY_COUNT - byTemp+1)%PLAY_COUNT;
//		byNextSendPos = (bySendPos + PLAY_COUNT - 1)%PLAY_COUNT;
//	}
//	else
//	{
//		bySendPos = (bySendPos + byTemp-1)%PLAY_COUNT;
//		byNextSendPos = (bySendPos + 1)%PLAY_COUNT;
//	}
//	m_bySentCardCount[bySendPos]++;
//
//	// �����͵�����ʾ����
//	if (m_bySentCardCount[bySendPos] > m_byHandCardCount[bySendPos])
//	{
//		m_bySentCardCount[bySendPos] = m_byHandCardCount[bySendPos];
//	}
//
//	WORD wCardList[POKER_CARD_NUM]={0};
//	int iViewSeat = GetViewSeat(bySendPos);
//
//	// ������ʧ
//	m_PlayView.m_SendCardAnimal[iViewSeat].StopMove();
//	m_PlayView.m_SendCardAnimal[iViewSeat].SetCard(wCardList, NULL, 0);
//
//	// ��ȡÿ��λ�õ���ͼ	
//	m_PlayView.m_HandCard[iViewSeat].SetCard(m_wHandCard[bySendPos], NULL, m_bySentCardCount[bySendPos]);
//
//	// ��ʾ�м�һ����		
//	WORD wPkCardCount = m_wSendCount-m_wCurSendPos;
//	WORD wPKCardCountView = 0x00;   // ��ʾ��ֵ
//	if (wPkCardCount > (POKER_CARD_NUM/2))
//	{
//		wPKCardCountView = wPkCardCount;   // ��ʾ��ֵ
//		wPkCardCount = POKER_CARD_NUM/2;
//	}
//	m_PlayView.m_SendBaseCard.SetCard(wCardList, NULL, (BYTE)wPkCardCount, (BYTE)wPKCardCountView);
//
//	//if (m_wCurSendPos < m_wSendCount)
//	//{
//	//	// ��ʾ��һλ��ҵĶ���
//	//	iViewSeat = GetViewSeat(byNextSendPos);
//	//	m_PlayView.SendCard(iViewSeat, m_GameBaseInfo.iSendCardInterval*2); 
//	//	return ;
//	//}
//}
//
///// ���ƽ���
//void CClientGameDlg::OnNotifySendCardFinish(void)
//{
//	WORD wCardList[POKER_CARD_NUM]={0};
//
//	// ��������
//	m_PlayView.m_MingCard.SetCard(wCardList, NULL, 0);
//
//	// �����м�һ����	
//	m_PlayView.m_SendBaseCard.SetCard(wCardList, NULL, 0);
//
//	int i=0;
//	for (i=0; i<PLAY_COUNT; ++i)
//	{
//		m_PlayView.m_SendCardAnimal[i].SetCard(wCardList, NULL, 0);
//	}
//
//	KillTimer(ID_SEND_CARD_TIMER);
//	KillTimer(ID_SENDCARD_INTERVAL);
//
//	for (i=0; i<PLAY_COUNT; ++i)
//	{
//		// ��ȡÿ��λ�õ���ͼ
//		int iViewSeat = GetViewSeat(i);
//
//		// ������Լ��������������
//		if (GetMeUserInfo()->bDeskStation == i) 
//		{
//			CPokerGameLogic m_Logic;
//			m_Logic.SortCards(m_wHandCard[i], m_byHandCardCount[i], i, false);
//			m_PlayView.m_HandCard[iViewSeat].SetUseMouse(true);
//		}
//
//		m_PlayView.m_HandCard[iViewSeat].SetCard(m_wHandCard[i], NULL, m_byHandCardCount[i]);
//	}
//}

/// ���ǿ�˽�����Ϸ
bool CClientGameDlg::OnNotifyCutEnd(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if (sizeof(GAME_FORCE_QUIT) > uDataSize)
	{
		WriteInfo(_T("ASS_CUT_END or ASS_SAFE_END"));
		return false;
	}

	GAME_FORCE_QUIT * pGameEnd=(GAME_FORCE_QUIT *)pNetData;
	if(NULL != m_pUserInfo[pGameEnd->byDeskStation]&& 0 == m_pUserInfo[pGameEnd->byDeskStation]->GameUserInfo.isVirtual)
	{
		if(pGameEnd->byDeskStation != GetMeUserInfo()->bDeskStation)
		{

			//��ʾ��Ϣ
			TCHAR szMessage[100]={0};
			if (pNetHead->bAssistantID==ASS_CUT_END)//ǿ����
			{
				//OutputDebugString("OnNotifyCutEnd ASS_CUT_END");
				if(NULL != m_pUserInfo[pGameEnd->byDeskStation])
				{
					//���ǿ��ʱ��ʾǿ���� modify by wlr 20090807
					//_stprintf_s(szMessage, sizeof(szMessage), TEXT("���ڡ� %s ��ǿ���˳�����Ϸ������"),m_pUserInfo[pGameEnd->byDeskStation]->GameUserInfo.nickName);
					_stprintf_s(szMessage, sizeof(szMessage), TEXT("�� %s ��ǿ����"),m_pUserInfo[pGameEnd->byDeskStation]->GameUserInfo.nickName);
				}
				else 
				{
					_stprintf_s(szMessage, sizeof(szMessage), TEXT("�� %s ��ǿ����"), _T("���"));
				}
				//end of ���ǿ��ʱ��ʾǿ���� modify by wlr 20090807
			}
			m_MessageHandle.InsertNormalMessage(szMessage);
			return true;//���ǿ��ʱ��ʾǿ���� add by wlr 20090807
		}
	}

	//������Ϸ�����ڽ���
	//if(m_pGameInfo->bEnableSound)
	//{
	//	SetPlaySound(SOUND_GAME_OVER);
	//}

	SetStationParameter(GS_WAIT_AGREE);	
	KillAllTimer();

	return TRUE;
}

void CClientGameDlg::OnChangeTrustee(BYTE byDeskStation, bool bIsTrustee)
{
	int i = 0;
	for (i=0; i<PLAY_COUNT; ++i)
	{
		if (byDeskStation == i)
		{
			// �����й���ͼ
			m_PlayView.SetTrustee(GetViewSeat(i), bIsTrustee);
			break;
		}
	}
}

///���ƣ�OnCancel
///�������ر���Ϸ����
///@param 
///@return  
void CClientGameDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	TCHAR szMsg[200]={_T("��������Ϸ�У������˳������йܣ�ȷʵҪ�˳���")};

	//��ѯ״̬
	if (!m_bWatchMode && GetStationParameter() >= GS_SEND_CARD && GetStationParameter() < GS_WAIT_NEXT )//
	{
		if(m_PlayView.m_iMyZhu[8]<=0 && GetMeUserInfo()->bDeskStation != m_PlayView.m_iNowNTstation)//���û����ע
		{
			wsprintf(szMsg,"��ǰ��Ϸ����û��ע���Ƿ��뿪������?"); ///lym100120
		}

		if (IDOK!=AFCMessageBox(szMsg,m_pGameInfo->szGameName,AFC_YESNO)) 
			return;

		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);//add by wlr 20090807 ���ǿ������ʾ���ǿ����

		//AFCCloseFrame();

		//m_pGameRoom->PostMessage(WM_COMMAND, MAKELONG(2021, BN_CLICKED), 0);
		////д��־add by wlr 20090806
		//WriteInfo(_T("ǿ����"));
		////end of д��־add by wlr 20090806
		//return;
	}
	m_PlayView.Init();
	AFCCloseFrame();
	__super::OnCancel();
	//CLoveSendClass::OnCancel();
}

void CClientGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case ID_RECT_TIMER://��˸Ӯ����
		{
			m_PlayView.m_bShowRect = !m_PlayView.m_bShowRect;
			m_PlayView.UpdateViewFace(NULL);
		}
		break;
	case ID_TEST_TIMER://��ͣ������ע��Ϣ
		{
			m_PlayView.m_iCardZhen--;
			if(m_PlayView.m_iCardZhen<=0)
			{
				KillTimer(ID_TEST_TIMER);
				m_PlayView.m_iCardZhen = -1;
				m_PlayView.m_bCardMove = false;
				m_PlayView.m_bZhuang = false;
				SetTimer(ID_SEND_CARD_TIMER,1,NULL);//��ʾ���ƶ������
			}

			m_PlayView.UpdateViewFace(NULL);
		}
		break;
	case ID_RESULT_TIMER://��ʾ������Ϣ
		{
			KillTimer(ID_RESULT_TIMER);

			if(m_pGameInfo->bEnableSound)
			{
				TCHAR filename[MAX_PATH+1];
				if(m_PlayView.m_iMeFen>0)
				{
					wsprintf(filename,TEXT("music\\win.wav"));// ������ע������Ч
				}
				else if(m_PlayView.m_iMeFen<0)
				{
					wsprintf(filename,TEXT("music\\lost.wav"));// ������ע������Ч
				}
				else
				{
					wsprintf(filename,TEXT("music\\he.wav"));// ������ע������Ч
				}
				BZSoundPlayGame(this,filename,NULL,1);//lu0820��������
			}
			m_PlayView.m_bSendCard = false;

			m_PlayView.UpdateNowData();//��������		

			SetTimer(ID_RECT_TIMER,300,NULL);//������˸��ʱ��

			m_PlayView.UpdateViewFace(NULL);
		}
		break;
	case ID_SEND_CARD_TIMER:
		{
			m_iSendCardTime++;					// ���ƶ�������ʱ�䣬���ƶ�����
			KillTimer(ID_SEND_CARD_TIMER);
			//m_PlayView.m_iCardZhen--;
			if(m_iSendCardTime==1)
			{
				KillTimer(ID_TEST_TIMER);
				m_PlayView.m_iShowCard=1;
			}
			else if(m_iSendCardTime==2)
			{
				KillTimer(ID_TEST_TIMER);
				m_PlayView.m_iShowCard=2;
			}
			else if(m_iSendCardTime==3)
			{
				m_PlayView.m_iShowCard=3;
			}
			else if(m_iSendCardTime==4)
			{
				m_PlayView.m_iShowCard=4;
				if(m_PlayView.m_CardCount[1]==2 && m_PlayView.m_CardCount[0]==2)//��ֻ��������
				{
					m_iSendCardTime=0;
					KillTimer(ID_SEND_CARD_TIMER);
					//SetTimer(ID_RESULT_TIMER,2000,NULL);
					m_PlayView.UpdateViewFace(NULL);
					return;
				}
			}
			else if(m_iSendCardTime==5)
			{
				if(m_PlayView.m_CardCount[1]==3)//����м�������
				{
					m_PlayView.m_iShowCard=5;
					if(m_PlayView.m_CardCount[0]==2)
					{
						m_iSendCardTime=0;
						KillTimer(ID_SEND_CARD_TIMER);
						//SetTimer(ID_RESULT_TIMER,2000,NULL);
						m_PlayView.UpdateViewFace(NULL);
						return;
					}
				}
				else 
				{
					m_iSendCardTime=0;
					m_PlayView.m_iShowCard=6;
					KillTimer(ID_SEND_CARD_TIMER);
					//SetTimer(ID_RESULT_TIMER,2000,NULL);
					m_PlayView.UpdateViewFace(NULL);
					return;
				}
			}
			else if(m_iSendCardTime==6) 
			{
				m_iSendCardTime=0;
				m_PlayView.m_iShowCard=7;
				KillTimer(ID_SEND_CARD_TIMER);
				//SetTimer(ID_RESULT_TIMER,2000,NULL);
				m_PlayView.UpdateViewFace(NULL);
				return;
			}

			m_PlayView.UpdateViewFace(NULL);
			SetTimer(ID_SENDCARD_INTERVAL, m_iSendInterval, NULL);
		}
		break;
	case ID_SENDCARD_INTERVAL:
		{
			KillTimer(ID_SENDCARD_INTERVAL);			
			if(m_iSendCardTime==1)//
			{
				StartMoveCard(true,1,m_PlayView.m_iSendCardZhen);//����ׯ�ҵ�һ��
			}
			else if(m_iSendCardTime==2)
			{
				StartMoveCard(false,2,m_PlayView.m_iSendCardZhen);//�����мҵ�2��
			}
			else if(m_iSendCardTime==3)//
			{
				StartMoveCard(true,2,m_PlayView.m_iSendCardZhen);//����ׯ�Ҽҵ�2��
			}
			//////
			else if(m_iSendCardTime==4)//
			{
				if(m_PlayView.m_CardCount[1] == 3)//����м��е�������
				{
					StartMoveCard(false,3,m_PlayView.m_iSendCardZhen);//����ׯ�Ҽҵ�2��
				}
				else if(m_PlayView.m_CardCount[0] == 3)//ׯ���е�������
				{
					StartMoveCard(true,3,m_PlayView.m_iSendCardZhen);//����ׯ�Ҽҵ�2��
				}
			}
			//////
			else if(m_iSendCardTime==5)//
			{
				if(m_PlayView.m_CardCount[0]==3 && m_PlayView.m_CardCount[1]==3)
				{
					StartMoveCard(true,3,m_PlayView.m_iSendCardZhen);
				}
			}
		}
		break;
	case ID_XIANPAI:	//���Ʒ��мҵ���
		{

		}
		break;
	case ID_ZHUANGPAI://��ׯ�ҵ���
		{

		}
		break;
	case ID_PLAY_SOUND:	//��������
		{
			m_PlaySound = true;
			KillTimer(nIDEvent);
		}
		break;
	case ID_FULLNOTE_TIME:
		{
			KillTimer(nIDEvent);
			m_PlayView.m_bFullNoteDisplay = false;
			m_PlayView.UpdateViewFace(NULL);
		}
		break;
	case PROMPT_CHANGE_NT_TIME:		//��ʾ��ׯ��Ϣ
		{
			KillTimer(PROMPT_CHANGE_NT_TIME);
			m_PlayView.SetPromptChangeNt(false);	///���û�ׯ��ʾͼƬ
			break;
		}
	default:
		break;
	}
	//CLoveSendClass::OnTimer((UINT)nIDEvent);
	CLoveSendClassInExe::OnTimer(nIDEvent);
}

///���ƣ�StartMoveCard
///��������ʼһ���ƶ����ĺ���
///@param  zhuang �Ƿ�ׯ���ơ� num �ڼ����� ��zhen ����֡
///@return  
void CClientGameDlg::StartMoveCard(bool zhuang,int num, int zhen)
{
	if(zhen<=0)
		zhen=8;
	m_PlayView.m_iCardZhen = zhen;
	m_PlayView.m_bCardMove = true;
	m_PlayView.m_iCardNum =num;
	m_PlayView.m_bZhuang = zhuang;

	if (m_PlayView.m_bZhuang)
	{
		m_PlayView.m_ContinueShowCardCount[0]++;
		if (3<m_PlayView.m_ContinueShowCardCount[0]) 
			m_PlayView.m_ContinueShowCardCount[0]=3;
	}
	else
	{
		m_PlayView.m_ContinueShowCardCount[1]++;
		if (3<m_PlayView.m_ContinueShowCardCount[1]) 
			m_PlayView.m_ContinueShowCardCount[1]=3;
	}

	//if(zhuang)
	//{
		SetTimer(ID_TEST_TIMER,m_iTimeSendCard/zhen,NULL);
	//}
	//else
	//{
	//	SetTimer(ID_TEST_TIMER,20,NULL);
	//}
	if(m_pGameInfo->bEnableSound)
	{
		TCHAR filename[MAX_PATH+1];
		wsprintf(filename,TEXT("music\\sendcard.wav"));// ���ŷ�����Ч
		PlayOtherSound(filename);
	}
}

//���ش����ƶ�����
// void CClientGameDlg::OnMove(int x, int y)
// {
// 
// }
///���ƣ�OnHandleKaiPai
///�������յ�ϵͳ������Ϣ����
///@param  pNetData ��Ϣͷ�� uDataSize ������
///@return  
void CClientGameDlg::OnHandleKaiPai(void * pNetData, UINT uDataSize)
{		
	KillGameTimer(ID_XIAZHU_TIME);
	KillTimer(ID_SEND_CARD_TIMER);
	KillTimer(ID_SENDCARD_INTERVAL);
	KillTimer(ID_TEST_TIMER);//ɾ�����Զ�ʱ��
	SetStationParameter(GS_PLAY_GAME);//��Ϸ��Ϊ��ʼ״̬
	m_PlayView.m_bNowGameState = GS_PLAY_GAME;//��ǰ��Ϸ״̬

	m_PlayView.m_byMyStation =GetMeUserInfo()->bDeskStation;//��ȡ�Լ���λ���Լ���λ��
	SetGameTimer(m_PlayView.m_byMyStation, m_iKaiPaiTime, ID_KAIPAI_TIME);//�������Ƽ�ʱ��
	m_PlayView.EnableXiaZhu(false);//������ע
	m_PlayView.m_bFullNoteDisplay = false;

	//m_PlayView.m_Result.ShowWindow(SW_HIDE);//lu20080710��ӣ���Ϸ��ʼʱ���ؽ����Ի���

	KAI_PAI *paidata = (KAI_PAI *)pNetData;
//
	m_PlayView.m_iKaiPai = paidata->m_iKaiPai;//zht20100207���濪�Ƶ�λ��

	for(int i=0;i<MAXCOUNT;i++)
	{
		for(int j=0;j<4;j++)
		{
			m_PlayView.m_iTempGameInfo[i][j] = paidata->m_iResultInfo[i][j];
		}
	}
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		 m_PlayView.m_iZhuangList[i] = paidata->zhuangstation[i];
	}
	
	m_PlayView.OnHandleKaiPai(pNetData,uDataSize);

	for ( int i=0; i<15; i++ )
	{
		KillTimer(ID_PLAY_SOUND+i+1);
	}

	m_PlayView.m_bSendCard = true;
	m_iSendCardTime =0;		// ���ƶ�������ʱ�䣬���ƶ��������Ƶ�״̬������������
	StartMoveCard(false,1,m_PlayView.m_iSendCardZhen);
}

///���ƣ�OnHandleJieSuan
///�������յ�ϵͳ������Ϣ����
///@param  pNetData ��Ϣͷ�� uDataSize ������
///@return  
void CClientGameDlg::OnHandleJieSuan(void * pNetData, UINT uDataSize)
{	
	JIE_SUAN *data = (JIE_SUAN *)pNetData;

	m_PlayView.m_bShowResult = true; //�Ƿ���ʾ������Ϣ
	m_PlayView.m_bFullNoteDisplay = false;
	CString CSt;
	CSt.Format("wysout::��ǰ�ľ���==%d",m_PlayView.m_iNTju);
	OutputDebugString(CSt);

	//�����ʱ����ʾ��ʷ���
	if (m_PlayView.m_iTempGameInfo[0][0]!= 0)
	{
		memcpy(m_PlayView.m_iGameInfo,m_PlayView.m_iTempGameInfo,sizeof(m_PlayView.m_iTempGameInfo));
	}
	
	if(m_PlayView.m_iNowNTstation>=0)
	{
		m_PlayView.m_iZhuangFen = data->m_iZhuangFen;		//ׯ�ҵĵ÷�
		m_PlayView.m_iXianFen = data->m_iXianFen;			//�мҵĵ÷�
		m_PlayView.m_iMeFen = data->m_iUserFen;				//
		m_PlayView.m_iNTwin = data->m_iNtWin;				//��ǰׯ��Ӯ�Ľ�ң��ɼ���
		m_PlayView.m_iMeFanFen = data->m_iMeFanFen;

		m_PlayView.m_iWinmoney += m_PlayView.m_iMeFen ;
		m_PlayView.m_Result.m_iNtWinMoney = m_PlayView.m_iZhuangFen;//ׯӮ��Ǯ
		m_PlayView.m_Result.m_iXianMoney = m_PlayView.m_iXianFen;	//��Ӯ��Ǯ
		m_PlayView.m_Result.m_iMeWinMoney = m_PlayView.m_iMeFen;	//�Լ�Ӯ��Ǯ
	}

	for (int i=0; i<MAXCOUNT; i++)			//������72����ע��Ϣ����Ӯ���
	{		
		m_PlayView.m_iMeXiaZhuInfo[i] = data->m_iUserXiaZhuInfo[i];
		m_PlayView.m_iResultInfoArea[i] = data->m_iResultInfoArea[i];	
		m_PlayView.m_iHistoryPlayData[i] = data->m_iResultInfoArea[i];


		m_PlayView.m_iHistoryDuiZi[i]  = data->m_iResultInfo[i][3];
	}
	m_PlayView.UpdateLuziData();

	TCHAR szMessage[100];
	__int64 nTempMoney=1; 
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nTempMoney *= 10;
	}
	///����ׯ��
	CString str = "";
	
	//m_PlayView.NumToChar(m_PlayView.m_iMeFen,&str);
	//wsprintf(szMessage,TEXT("���� �÷֣�%s"),str);	
	//m_MessageHandle.InsertNormalMessage(szMessage);

	//m_PlayView.NumToChar(m_PlayView.m_iZhuangFen,&str);
	//wsprintf(szMessage,TEXT("ׯ�� �÷֣�%s"),str);	
	//m_MessageHandle.InsertNormalMessage(szMessage);
	

	SetTimer(ID_RESULT_TIMER,100,NULL);
	SetTimer(ID_RECT_TIMER,300,NULL);		//������˸��ʱ��

	m_PlayView.UpdateViewFace(NULL);
}

///���ƣ�OnHandleXiazhu
///�������յ������ע��Ϣ����
///@param  pNetData ��Ϣͷ�� uDataSize ������
///@return  
void CClientGameDlg::OnHandleXiazhu(void * pNetData, UINT uDataSize)
{
	XIA_ZHU *pXiazhu = (XIA_ZHU *)pNetData;

	m_PlayView.OnHandleXiazhu(pNetData, uDataSize);

	if(m_PlayView.m_iMyZhu[8]>=m_PlayView.m_iMaxNote && m_PlayView.m_iMaxNote>0 && pXiazhu->station == m_PlayView.m_byMyStation)
	{
		char szMessage[MAX_PATH] = {0};
		CString strMaxNoteMoney;
		if (m_PlayView.m_iMaxNote > 0)
		{			
			sprintf(szMessage,"����ʾ��:������ע�Ѵﵽ���ޣ�Ŀǰ������ע��");
			m_MessageHandle.InsertNormalMessage(szMessage);
			m_PlayView.	OnRestoreCursor();
			m_PlayView.m_iChooseNote = 0;
			m_PlayView.UpdateViewFace(NULL);
			m_PlayView.m_bFullNoteDisplay = true;
			SetTimer(ID_FULLNOTE_TIME,3000,NULL);
		}
	}
}


///���ƣ�OnHandleXiazhuPlaySound
///������������ע����
///@param  pNetData ��Ϣͷ�� uDataSize ������
///@return  
void CClientGameDlg::OnHandleXiazhuPlaySound(void * pNetData, UINT uDataSize)
{
	XIA_ZHU *pXiazhu = (XIA_ZHU *)pNetData;

	///20100207zht�޸İ���
	int nTempMoney=1; 
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nTempMoney*=10;
	}
	__int64 money = 100000/nTempMoney; 

	//���ų�������
	TCHAR filename[MAX_PATH+1];
	if(pXiazhu->money==5000000/nTempMoney)//500w����
	{
		if(m_pGameInfo->bEnableSound)
		{
			PlayPublicSound(ID_SOUND_500W);
		}
	}
	else if(pXiazhu->money==1000000/nTempMoney)//1000w����
	{
		if(m_pGameInfo->bEnableSound)
		{
			PlayPublicSound(ID_SOUND_100W);
		}
	}
	else if(pXiazhu->money==100000/nTempMoney)//����10�򲥷Ŵ���������
	{
		if(m_pGameInfo->bEnableSound)
		{
			PlayPublicSound(ID_SOUND_DCM);
		}
	}
	else//���򲥷�С��������� 
	{
		if(m_pGameInfo->bEnableSound)
		{
			PlayPublicSound(ID_SOUND_CM);
		}
	}

	m_PlayView.m_Result.ShowWindow(SW_HIDE);//lu20080710��ӣ���Ϸ��ʼʱ���ؽ����Ի���

	//����Ƿ�����ע����
	__int64 nowmoney = m_PlayView.m_iQuYuZhu[pXiazhu->type]%50000;
	nowmoney += pXiazhu->money;
	if(nowmoney >= 50000)
	{	
		//�������ֳ���500w���Ŷ���������
		TCHAR filename[MAX_PATH+1];//500w����
		if(m_pGameInfo->bEnableSound)
		{
			PlayPublicSound(ID_SOUND_MORE_500W);
		}
		//StartChao500Move(pXiazhu->type);//��������500��Ķ���
	}
}

//����Mp3����
BOOL CClientGameDlg::PlayOutCardType(TCHAR FileName[],HWND hmciwnd)
{
	if(hmciwnd)
	{
		MCIWndStop(hmciwnd);
		MCIWndDestroy(hmciwnd);
		hmciwnd=NULL;
	}

	//hmciwnd = MCIWndCreate(this->m_hWnd,AfxGetInstanceHandle(),WS_CHILD,FileName);

	hmciwnd=MCIWndCreate(this->m_hWnd,AfxGetInstanceHandle(),WS_CHILD|MCIWNDF_NOMENU|MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR|MCIWNDF_NOERRORDLG,FileName);
	OutputDebugString(FileName);
	if(hmciwnd)
		MCIWndPlay(hmciwnd);	
	return true;
}


///���ƣ�PlayNoteSound
///������������ע������������ע��������Ͳ�����Ӧ������
///@param  bSoundStyle �������͡�
///@return  
BOOL CClientGameDlg::PlayNoteSound(BYTE bSoundStyle)
{
	CString filename;
	TCHAR fi[MAX_PATH + 1];
	HWND hMciWnd = NULL;

	switch (bSoundStyle)
	{
	case ID_SOUND_MORE_500W:
		{
			hMciWnd = m_hMciWnd1;
			filename = TEXT("music\\chao500w");
			break;
		}
	case ID_SOUND_500W:
		{
			hMciWnd = m_hMciWnd2;
			filename = TEXT("music\\sound500w");
			break;
		}
	case ID_SOUND_100W:
		{
			hMciWnd = m_hMciWnd3;
			filename = TEXT("music\\sound100w");
			break;
		}
	case ID_SOUND_DCM:
		{
			hMciWnd = m_hMciWnd4;
			filename = TEXT("music\\dachouma");
			
			break;
		}
	case ID_SOUND_CM:
		{
			hMciWnd = m_hMciWnd5;
			filename = TEXT("music\\chouma");
			wsprintf(fi, "%s.wav", filename);
			::PlaySound(fi, NULL, SND_FILENAME|SND_ASYNC|SND_NOSTOP|SND_NOWAIT|SND_NODEFAULT);
			return true;
			break;
		}
	default:
		//hMciWnd = m_hMciWnd5;
		break;
	}

	wsprintf(fi, "%s.wav", filename);
	//PlayOutCardType(fi, hMciWnd);
	//PlayCardMusic(fi, hMciWnd)
	//::PlaySound(fi, NULL, SND_FILENAME|SND_ASYNC|SND_NOSTOP|SND_NOWAIT|SND_NODEFAULT);
	BZSoundPlayGame(this, fi, WM_USER, 1);

	/*if (hMciWnd)
		PlayOutCardType(fi, hMciWnd);*/

	return true;
}


///���ƣ�OnHandleBegin
///����������ټ�����Ϸ��ʼ��Ϣ
///@param  pNetData ��Ϣͷ�� uDataSize ������
///@return  
void CClientGameDlg::OnHandleBegin(void * pNetData, UINT uDataSize)
{			
	BEGIN_DATA *pBegin = (BEGIN_DATA *)pNetData;
	KillTimer(ID_RECT_TIMER);//ɾ����˸��ʱ��
	// ��ʼ��������ͨ��
	//BZSoundStopAll();
	memset(m_bPlayListHwnd, -1, sizeof(m_bPlayListHwnd));
	m_bPlayListPos = 0;
	//BZSoundStopAll();
	
	m_iXiaZhuTime = pBegin->m_iXiaZhuTime;		   // ��עʱ��			
	m_iKaiPaiTime = pBegin->m_iKaiPaiTime;         // ����ʱ��
	m_iFreeTime = pBegin->m_iFreeTime;			   // ����ʱ��

	m_PlayView.Init();//��ʼ����ͼ����
	///��ǰ����������µ���ע
	m_PlayView.m_iMaxNote = pBegin->iMaxNote;	


	m_iSendCardTime =0;							//���ƶ�������ʱ�䣬���ƶ�����
	//m_bCanXiaZhu =true ;						//�ܷ���ע
	SetStationParameter(GS_SEND_CARD);			//��Ϸ��Ϊ��ʼ״̬
	m_PlayView.m_bNowGameState = GS_SEND_CARD;	//��ǰ��Ϸ״̬
	//m_PlayView.UpdateNowData();

	//m_PlayView.EnableXiaZhu(false);
	//m_PlayView.EnableXiaZhu(true);

	//m_PlayView.m_iMePlayCount++;

	m_PlayView.m_Result.ShowWindow(SW_HIDE);	//��ӣ���Ϸ��ʼʱ���ؽ����Ի���
	KillGameTimer(ID_WAIT_TIME);
	KillTimer(ID_SEND_CARD_TIMER);
	KillTimer(ID_SENDCARD_INTERVAL);

	m_PlayView.m_iNowNTstation = pBegin->m_iNowNtStation;//��ȡׯ�ҵ�λ��

	if(m_pUserInfo[m_PlayView.m_iNowNTstation]!=NULL)
	{
		CString strLog="";

		strLog.Format("ׯ�ң��ͻ���%d  %s",m_PlayView.m_iNowNTstation, m_pUserInfo[m_PlayView.m_iNowNTstation]->GameUserInfo.nickName);
		OutputDebugString(strLog);

		m_PlayView.m_iNTmoney =m_pUserInfo[m_PlayView.m_iNowNTstation]->GameUserInfo.i64Money;//ׯ�ҵĽ��
	}


	m_PlayView.m_iNTwin = pBegin->m_iNTdata[2];
	m_PlayView.m_iNTju = pBegin->m_iZhuangBaShu;//ׯ�ҵľ���
	m_PlayView.m_iPlayCount = pBegin->m_iBaSHu;//�Ѿ������˱��ֵĵڼ���
	m_PlayView.m_iGameCount = pBegin->m_iGameCount ;//�ܹ������˼���
	m_PlayView.m_iLimitMoney = pBegin->m_iShangZhuangLimit;//��ׯ�޶�
	m_PlayView.m_bShowResult = false; //�Ƿ���ʾ������Ϣ
	m_PlayView.m_iNowJuShu = pBegin->m_iNowJuShu;	//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�

	if(GetMeUserInfo()!=NULL)
	{
		m_PlayView.m_byMyStation =GetMeUserInfo()->bDeskStation;//��ȡ�Լ���λ���Լ���λ��
	}


	for(int i=0;i<8;i++)
	{
		m_PlayView.m_iMaxZhu[i] = pBegin->m_iMaxZhu[i];
	}

	for(int i=0;i<MAXCOUNT;i++)
	{
		for(int j=0;j<4;j++)
		{
			m_PlayView.m_iGameInfo[i][j] = pBegin->m_iResultInfo[i][j];
		}
	}

	SetGameTimer(m_PlayView.m_byMyStation, m_iXiaZhuTime, ID_XIAZHU_TIME);//������ע��ʱ��

	///////////////////��ע����//////////////////////////////
	//SetTimer(ID_TEST_TIMER,20,NULL);
	/////////////////////////////////////////////////////////

	// ��Ч
	if(m_pGameInfo->bEnableSound)
	{
		TCHAR filename[MAX_PATH+1];
		wsprintf(filename,TEXT("music\\BeginGame.wav"));// ������Ч
		BZSoundPlayGame(this, filename, NULL, 1);
		//PlayOtherSound(filename);
	}	

	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_PlayView.m_iZhuangList[i] = pBegin->m_iNTlist[i];//��ȡׯ���б�����
	}


	if(m_PlayView.m_iNowNTstation != m_PlayView.m_byMyStation)
	{//����ׯ�Ҳ�����ע
		m_PlayView.EnableXiaZhu(true);//������ע
	}
	else
	{
		m_PlayView.EnableXiaZhu(false);//
	}

	if(!m_PlayView.CheckMeZhuang())
	{
		m_PlayView.SetApplyBanker(true);
	}
	else
	{
		m_PlayView.SetApplyBanker(false);
	}
	m_PlayView.m_sSuperInfo = "";			///�����ͻ��˿�����Ϣ
	m_PlayView.m_bShowSuper = false;		//��ʾ�����ͻ��˿�����Ϣ

	m_PlayView.SetXiaZhuBtn();
	m_PlayView.UpdateViewFace(NULL);
}
///���ƣ�OnHandleSuperCtr
///�������յ������������ͻ��˿�����Ϣ��ֻ�г����ͻ��˲����յ�
///@param  pNetData ��Ϣͷ�� uDataSize ������
///@return 
void CClientGameDlg::OnHandleSuperCtr(void * pNetData, UINT uDataSize)
{			
	m_PlayView.m_btnCtrZhuang.EnableWindow(true);
	m_PlayView.m_btnCtrXian.EnableWindow(true);
	m_PlayView.m_btnCtrHe.EnableWindow(true);
	m_PlayView.m_btnCtrNormal.EnableWindow(true);
	///��ʾ�����Ի���İ�ť
	m_PlayView.m_btnCtrSuper.ShowWindow(SW_SHOW);
	m_PlayView.m_btnCtrSuper.EnableWindow(true);

	SUPERSETTING *pSuper = (SUPERSETTING *)pNetData;
	CString str="��";
	if(pSuper->bDeskStation != m_PlayView.m_byMyStation)//���Լ�����
	{
		str.Format("%s",pSuper->name);			///�����ͻ��˿�����Ϣ
	}
	switch(pSuper->bState)
	{
	case 0:		//ȡ�����ƣ�����ģʽ
		{
			m_PlayView.m_sSuperInfo = str+"ȡ������";			///�����ͻ��˿�����Ϣ
			m_PlayView.m_btnCtrNormal.EnableWindow(false);
		}
		break;
	case 1:    //���ƿ�ׯ
		{
			m_PlayView.m_sSuperInfo = str+"���Ʊ��ֿ���ׯ";			///�����ͻ��˿�����Ϣ
			m_PlayView.m_btnCtrZhuang.EnableWindow(false);
		}
		break;
	case 2:    //���ƿ���
		{
			m_PlayView.m_sSuperInfo = str+"���Ʊ��ֿ�����";			///�����ͻ��˿�����Ϣ
			m_PlayView.m_btnCtrXian.EnableWindow(false);
		}
		break;
	case 3:    //���ƿ���
		{
			m_PlayView.m_sSuperInfo = str+"���Ʊ��ֿ�����";			///�����ͻ��˿�����Ϣ
			m_PlayView.m_btnCtrHe.EnableWindow(false);
		}
		break;
	}
	m_PlayView.UpdateViewFace(NULL);
	m_PlayView.m_SuperDlg.SetCtrlResult(pSuper->bState);

}


///��������500��Ķ���
void CClientGameDlg::StartChao500Move(int type)
{
	m_PlayView.StartChao500Move(type);
}

void CClientGameDlg::PlayOtherSound(TCHAR* path)
{
#ifndef NO_VIEW  //û�н���,Ҳ����������
	if (!m_pGameInfo->bEnableSound)
	{
		return;
	}
	if (path == NULL)
	{
		return;
	}
	::PlaySound(path,NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP|SND_NOWAIT|SND_NODEFAULT);//SND_LOOP
#endif

}

///������ͼת��������ʹ����ͼλ�����߼�λ��һ��
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation) 
{
	return bDeskStation; 
}
///��������¼�
void CClientGameDlg::OnBnClickedMax()
{

	//m_bMax=true;
	//m_bShowMax=true;

	//MoveWindow(0,0,1024,738);
	//CenterWindow();
	//ShowWindow(SW_RESTORE);
	//
	//m_btMax.ShowWindow(SW_HIDE);
	//m_btRest.ShowWindow(SW_SHOW);

	//SetForegroundWindow();
	m_bMax=true;
	m_bShowMax=true;
	ShowWindow(SW_RESTORE);
	CRect rect;
	::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
	MoveWindow(0,0,rect.Width(),rect.Height());
	m_btMax.ShowWindow(SW_HIDE);
	m_btRest.ShowWindow(SW_SHOW);

	SetForegroundWindow();
}

void CClientGameDlg::OnBnClickedRest()
{

	m_bMax=false;
	m_bShowMax=false;
	
	MoveWindow(0,0,1024,738);
	CenterWindow();
	ShowWindow(SW_RESTORE);

	m_btMax.ShowWindow(SW_SHOW);
	m_btRest.ShowWindow(SW_HIDE);
}

void CClientGameDlg::GetGameCode(char * szGameCode) 
{ 
	strcpy(szGameCode,"10301800"); 
} 

// ���ز�������
void CClientGameDlg::BZSoundPlayGame(CWnd *pWnd, char *szFilename, DWORD dwPlayEnd, int nLoop)
{
	try
	{
		/**
		*1. Ϊ�˽�ʡ�ڴ��CPU��Դ��ͬ������������20·֮�ڣ�
		*2. ����20·�����Ƕ���ͬһ���̶��ԣ�ͬһ�����е�APP��
		*DLL����20·���ơ�һ������£�����ռ��ͨ������һЩ�������֣�
		*Ŀǰ�ڲ�����Ϸ�У�����������3�֡�����ʵ��ʣ��17·����
		**/
		::PlaySound(szFilename, NULL, SND_FILENAME|SND_ASYNC|SND_NOSTOP|SND_NOWAIT|SND_NODEFAULT);
		BYTE i=0;
		for (;i<15;i++)
		{
			if (-1!=m_bPlayListHwnd[i])
			{
				//m_bPlayListHwnd[i] = BZSoundPlay(pWnd, szFilename, dwPlayEnd, nLoop);				

				CString aa;
				aa.Format("MSG_BZSoundPlayGame_%d",m_bPlayListHwnd[i]);
				OutputDebugString(aa);
				return;
			}
		}

		if (i==15)
		{
			CString aa;
			aa.Format("MSG_BZSoundPlayGame_pos=%d,%d",m_bPlayListPos,m_bPlayListHwnd[m_bPlayListPos]);
			OutputDebugString(aa);

			//BZSoundStop(m_bPlayListHwnd[m_bPlayListPos]);
			//m_bPlayListHwnd[m_bPlayListPos++] = BZSoundPlay(pWnd, szFilename, dwPlayEnd, nLoop);
		}
	}
	catch (...)
	{
		DWORD dwError = GetLastError();
		CString aa;
		aa.Format("MSG_BZSoundPlayGame_Error %d",dwError);
		OutputDebugString(aa);
	}
}

///����Mp3����
HWND PlayCardMusic(TCHAR FileName[],HWND hParent)
{	
	HWND hmciwnd;
	//hmciwnd = MCIWndCreate(this->m_hWnd,AfxGetInstanceHandle(),WS_CHILD,FileName);
	hmciwnd=MCIWndCreate(hParent,AfxGetInstanceHandle(),WS_CHILD|MCIWNDF_NOMENU|MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR|MCIWNDF_NOERRORDLG,FileName);
	if(hmciwnd)
		MCIWndPlay(hmciwnd);	
	return hmciwnd;
}

//Ϊ��Ƶģ�����ش˺���
#ifdef VIDEO
bool CClientGameDlg::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (NULL == m_hWnd)
	{
		return false;
	}

	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return false;
	}
	
	if ((GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID) && bWatchUser )
	{
		m_PlayView.m_bWatch=bWatchUser;
	}

	if( (GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID) && (!bWatchUser) )
	{    
		CPlayVideo *pPlayVideo=CPlayVideo::CreatePlayVideo();
		if (NULL != pPlayVideo)
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
			stInitVideo.strUserName	  = NickName.Left(MAX_VIDEO_NICKNAME_LEN);
			stInitVideo.strUserPass   = _T("");
			if ( pPlayVideo->Video_Initialize(stInitVideo) )
			{
				///������Ƶ������				
				pPlayVideo->Connect();			
			}
			else
			{
				if (NULL != pPlayVideo->m_hWnd)
				{
					pPlayVideo->Video_Release();
				}
				return false;
			}
		} 
	}
 	return true;
}

bool CClientGameDlg::AFCCloseFrame()
{
	 ///�ͷ���Ƶ��Դ 
    CPlayVideo *pPlayVideo = CPlayVideo::GetPlayVideo();
    if ((NULL != pPlayVideo) && (NULL != pPlayVideo->m_hWnd))
	{
		pPlayVideo->Video_Release(); 
	}
	return __super::AFCCloseFrame();
}
#endif

void CClientGameDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CLoveSendClassInExe::OnRButtonUp(nFlags, point);
}

void CClientGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CLoveSendClassInExe::OnLButtonUp(nFlags, point);
}

///���ƣ�OnSetGameData
///��������Ҷ�������ʱ��ͨ��������Ϣ��ȡ��Ϸ����
///@param  pNetData ��Ϣͷ�� uDataSize ������
///@return 
void CClientGameDlg::OnSetGameData(void * pNetData, UINT uDataSize)
{

	CHONG_LIAN *pGameData = (CHONG_LIAN *)pNetData;
	if(pGameData->m_bGameStation == GS_SEND_CARD )///< ��Ϸ״̬lym2010-03-19 ��ע״̬
	{
		m_iXiaZhuTime = pGameData->m_iXiaZhuTime;		//��עʱ��			
		m_iKaiPaiTime = pGameData->m_iKaiPaiTime;       //����ʱ��
		m_iFreeTime = pGameData->m_iFreeTime;			//����ʱ��
		m_iSendInterval = pGameData->m_iSendInterval;	//����ʱ����
		m_iTimeSendCard = pGameData->m_iSendCardTime;	//����ʱ��
		// ���汾
		SetStationParameter(GS_SEND_CARD);
		m_PlayView.Init();
		///��ǰ����������µ���ע
		m_PlayView.m_iMaxNote = pGameData->iMaxNote;	
		m_PlayView.m_bNowGameState = GS_SEND_CARD;//��ǰ��Ϸ״̬
		m_PlayView.m_byMyStation =GetMeUserInfo()->bDeskStation;//��ȡ�Լ���λ���Լ���λ��
		m_PlayView.EnableXiaZhu(m_PlayView.m_byMyStation==pGameData->m_iNowNtStation ? false : true);

		for(int i=0;i<=PLAY_COUNT;i++)
		{
			m_PlayView.m_iZhuangList[i] = pGameData->zhuangstation[i];
		}

		for(int i=0;i<MAXCOUNT;i++)
		{
			for(int j=0;j<4;j++)
			{
				m_PlayView.m_iGameInfo[i][j] = pGameData->m_iResultInfo[i][j];
			}
		}
		m_PlayView.m_iPlayCount = pGameData->m_iBaSHu;
		m_PlayView.m_iGameCount = pGameData->m_iGameCount;
		m_PlayView.m_iNTju =  pGameData->m_iZhuangBaShu;
		m_PlayView.m_iNowNTstation =  pGameData->m_iNowNtStation;
		m_PlayView.m_iLimitMoney = pGameData->m_iShangZhuangLimit;//��ׯ�޶�
		m_PlayView.m_iZhongZhu = pGameData->iZhongZhu;//���ѵ�ǰ��ע��
		m_PlayView.m_iSendCardZhen = pGameData->m_iSendCardZhen;		//����֡��
		// ������ׯ����ׯ��ť
		m_PlayView.SetApplyBanker(m_PlayView.CheckMeZhuang() ? false : true);

		for(int i=0;i<8;i++)
		{
			m_PlayView.m_iMyZhu[i] = pGameData->m_iMeZhu[i];		//�ҵ���ע
			m_PlayView.m_iMaxZhu[i] = pGameData->iMaxZhu[i];		//ÿ���������¶���ע	
			m_PlayView.m_iQuYuZhu[i] = pGameData->m_iQuYuZhu[i];	//����ÿ�������µ�ע��
			m_PlayView.m_iAIMoney[i] = pGameData->m_iAIMoney[i];	///�����˵���ע
			//m_PlayView.m_SuperData.sUserInfo[m_PlayView.m_byMyStationi].iUserNote[i] = ;//�Լ���ÿ���������ע
			for(int j=0;j<6;j++)
			{
				m_PlayView.m_iQuYuJinBi[i][j]  = pGameData->m_iQuYuJinBi[i][j];	//ÿ�����¸��ࣨ��6�ࣩ��ҵĸ���
			}
		}
		m_PlayView.m_iMyZhu[8] = pGameData->m_iMeZhu[8];//�ҵ���ע
		m_PlayView.ShowNowNote();
		if(pGameData->m_iSYTime>1)
		{
			SetGameTimer(m_PlayView.m_byMyStation, pGameData->m_iSYTime, ID_XIAZHU_TIME);//������ע��ʱ��
		}

		for (int i=0; i<MAXCOUNT; i++)			//������72����ע��Ϣ����Ӯ���
		{		
			m_PlayView.m_iMeXiaZhuInfo[i] = pGameData->m_iUserXiaZhuInfo[i];
			m_PlayView.m_iResultInfoArea[i] = pGameData->m_iResultInfoArea[i];	
			m_PlayView.m_iHistoryPlayData[i] = pGameData->m_iResultInfoArea[i];	

			m_PlayView.m_iHistoryDuiZi[i]   = pGameData->m_iResultInfo[i][3];
		}
		m_PlayView.UpdateLuziData();

		if(m_PlayView.m_iNowNTstation != m_PlayView.m_byMyStation)
		{//����ׯ�Ҳ�����ע
			m_PlayView.EnableXiaZhu(true);//������ע			
		}
		else
		{
			m_PlayView.EnableXiaZhu(false);//
		}
		if(!m_PlayView.CheckMeZhuang())
		{
			m_PlayView.SetApplyBanker(true);
		}
		else
		{
			m_PlayView.SetApplyBanker(false);
		}
		m_PlayView.m_sSuperInfo = "";			///�����ͻ��˿�����Ϣ
		m_PlayView.m_bShowSuper = false;		//��ʾ�����ͻ��˿�����Ϣ
	}
	else if(pGameData->m_bGameStation == GS_PLAY_GAME)	//����״̬
	{		
		m_iXiaZhuTime = pGameData->m_iXiaZhuTime;		//��עʱ��			
		m_iKaiPaiTime = pGameData->m_iKaiPaiTime;       //����ʱ��
		m_iFreeTime = pGameData->m_iFreeTime;			//����ʱ��
		m_iSendInterval = pGameData->m_iSendInterval;	//����ʱ����
		m_iTimeSendCard = pGameData->m_iSendCardTime;	//����ʱ��
		m_PlayView.Init();
		///��ǰ����������µ���ע
		m_PlayView.m_iMaxNote = pGameData->iMaxNote;	
		// ���汾
		SetStationParameter(GS_PLAY_GAME);
		m_PlayView.m_bNowGameState = GS_PLAY_GAME;//��ǰ��Ϸ״̬

		m_PlayView.m_byMyStation =GetMeUserInfo()->bDeskStation;//��ȡ�Լ���λ���Լ���λ��
		m_PlayView.m_iLimitMoney = pGameData->m_iShangZhuangLimit;//��ׯ�޶�
		m_PlayView.m_iSendCardZhen = pGameData->m_iSendCardZhen;		//����֡��
		m_PlayView.m_iNowJuShu = pGameData->m_iNowJuShu;	//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�

		for(int i=0;i<=PLAY_COUNT;i++)
		{
			m_PlayView.m_iZhuangList[i] = pGameData->zhuangstation[i];
		}
		for(int i=0;i<MAXCOUNT;i++)
		{
			for(int j=0;j<4;j++)
			{
				m_PlayView.m_iGameInfo[i][j] = pGameData->m_iResultInfo[i][j];
			}
		}
		for(int i=0;i<MAXCOUNT;i++)
		{
			if (pGameData->m_iResultInfo[i][0] == 255)	//��Ч����
				continue;
			if (pGameData->m_iResultInfo[i][0] < 0)		//��Ч����
				continue;
			for(int j=0;j<4;j++)
			{
				m_iGameInfo[i][j] = pGameData->m_iResultInfo[i][j];
			}
		}

		for (int i=0; i<MAXCOUNT; i++)			//������72����ע��Ϣ����Ӯ���
		{		
			m_PlayView.m_iMeXiaZhuInfo[i] = pGameData->m_iUserXiaZhuInfo[i];
			m_PlayView.m_iResultInfoArea[i] = pGameData->m_iResultInfoArea[i];	
			m_PlayView.m_iHistoryPlayData[i] = pGameData->m_iResultInfoArea[i];	

			m_PlayView.m_iHistoryDuiZi[i]   = pGameData->m_iResultInfo[i][3];
		}
		m_PlayView.UpdateLuziData();

		m_PlayView.m_iPlayCount = pGameData->m_iBaSHu;
		m_PlayView.m_iGameCount = pGameData->m_iGameCount;
		m_PlayView.m_iNTju =  pGameData->m_iZhuangBaShu;
		m_PlayView.m_iNowNTstation =  pGameData->m_iNowNtStation;   
		m_PlayView.m_iZhongZhu = pGameData->iZhongZhu;//���ѵ�ǰ��ע�� 
		for(int i=0;i<8;i++)
		{
			m_PlayView.m_iMyZhu[i] = pGameData->m_iMeZhu[i];//�ҵ���ע
			m_PlayView.m_iMaxZhu[i] = pGameData->iMaxZhu[i];			//ÿ���������¶���ע	
			m_PlayView.m_iQuYuZhu[i] = pGameData->m_iQuYuZhu[i];			//����ÿ�������µ�ע��
			m_PlayView.m_iAIMoney[i] = pGameData->m_iAIMoney[i];	///�����˵���ע
			for(int j=0;j<6;j++)
			{
				 m_PlayView.m_iQuYuJinBi[i][j] = pGameData->m_iQuYuJinBi[i][j];	//ÿ�����¸��ࣨ��6�ࣩ��ҵĸ���
			}
		}
		m_PlayView.m_iMyZhu[8] = pGameData->m_iMeZhu[8];//�ҵ���ע

		for(int i=0;i<5;i++)
		{
			m_PlayView.m_iZPaiXing[i] = pGameData->m_iZPaiXing[i];//ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
			m_PlayView.m_iXPaiXing[i] = pGameData->m_iXPaiXing[i];			
		}
		
		for(int i=0;i<2;i++)
		{	
			m_PlayView.m_CardCount[i] = pGameData->m_CardCount[i];		//˫���������� 
			for(int j=0;j<3;j++)
			{
				 m_PlayView.m_iNowCard[i][j] = pGameData->pai[i][j];//ׯ�е��ƣ�0Ϊׯ��1Ϊ��
			}
		}

		m_PlayView.m_iZhuangFen = pGameData->m_iZhuangFen;			//ׯ�ҵĵ÷�
		m_PlayView.m_iXianFen = pGameData->m_iXianFen;				//�мҵĵ÷�
		m_PlayView.m_iMeFen = pGameData->m_iUserFen;				//��ǰ��ҵĵ÷�
		m_PlayView.m_iWinner = pGameData->m_iWinner;				//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
		m_PlayView.m_iKaiPai = pGameData->m_iKaiPai;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���

		m_PlayView.ShowNowNote();		
		//ʣ��ʱ��
		if(pGameData->m_iSYTime>1)
		{
			KillGameTimer(ID_XIAZHU_TIME);
			SetGameTimer(m_PlayView.m_byMyStation, pGameData->m_iSYTime, ID_KAIPAI_TIME);//������ע��ʱ��
		}	
		if(m_PlayView.m_CardCount[0] == 2 && m_PlayView.m_CardCount[1] == 2)
		{
			m_PlayView.m_iShowCard = 4;
		}
		else if(m_PlayView.m_CardCount[0] == 2 && m_PlayView.m_CardCount[1] == 3)
		{
			m_PlayView.m_iShowCard = 5;
		}
		else if(m_PlayView.m_CardCount[0] == 3 && m_PlayView.m_CardCount[1] == 2)
		{
			m_PlayView.m_iShowCard = 5;
		}
		else
		{
			m_PlayView.m_iShowCard = 7;
		}
		if(pGameData->m_bIsJieSuan)//�Ѿ���ʾ�˽�����Ϣ
		{
			m_PlayView.m_bShowResult = true;
			SetTimer(ID_RECT_TIMER,300,NULL);//������˸��ʱ��
		}
		m_PlayView.UpdateViewFace(NULL);
	}

	char szMessage[MAX_PATH] = {0};
	CString strMixMoney, strMaxMoney, strMaxNoteMoney;

	//m_PlayView.NumToChar(m_PlayView.m_iLimitMoney,&strMixMoney);
	m_PlayView.ChangeNumToCStringByPower(m_PlayView.m_iLimitMoney ,true ,"," ,strMixMoney) ;
	
	//m_PlayView.NumToChar(1500000000000000000,&strMaxMoney);
	m_PlayView.ChangeNumToCStringByPower(1500000000000000000 ,true ,"," ,strMaxMoney) ;

	_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ׯ������%s%s"), 
		strMixMoney,m_MoneyType);
	m_MessageHandle.InsertNormalMessage(szMessage);

	CString strBaseMaxNote;
	if (m_PlayView.m_iMaxNote > 0)
	{
		//m_PlayView.NumToChar(pGameData->m_iBaseMaxNote, &strBaseMaxNote);
		m_PlayView.ChangeNumToCStringByPower(pGameData->m_iBaseMaxNote ,true ,"," ,strBaseMaxNote) ;

		//m_PlayView.NumToChar(pGameData->m_iBaseMaxNote*2, &strMaxNoteMoney);
		m_PlayView.ChangeNumToCStringByPower(pGameData->m_iBaseMaxNote*2 ,true ,"," ,strMaxNoteMoney) ;

		if (pGameData->bIsVipDouble)
			_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ͨ��������ע��%s%s�� VIP��������ע��%s%s"), strBaseMaxNote, m_MoneyType, strMaxNoteMoney, m_MoneyType); 
		else
			_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ͨ��������ע��%s%s"), strBaseMaxNote, m_MoneyType); 
		m_MessageHandle.InsertNormalMessage(szMessage);
	}

}

// ������Ϸ����
void CClientGameDlg::PlayPublicSound(int SoundID)
{
	CString folder,filename;
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)); 

	folder.Format("music\\"); 	

	switch (SoundID)
	{
	case ID_SOUND_CM:			// ��������
		{
			filename.Format("chouma"); 
			break;
		}
	case ID_SOUND_DCM:			// ���������
		{
			filename.Format("dachouma");
			break;
		}
	case ID_SOUND_100W:			// һ��������
		{
			filename.Format("sound100w");
			break;
		}
	case ID_SOUND_500W:			// ���������
		{
			filename.Format("sound500w"); 
			break;
		}
	case ID_SOUND_MORE_500W:	// �����������
		{
			filename.Format("chao500w"); 
			break;
		}
	default:
		{
			filename = ""; 
			break;
		}
	}

	wsprintf(szFileName,"%s%s%s",folder,filename,".wav");

	if (m_pSEBkMusic)
		m_pSEBkMusic->play2D(szFileName,false);
}