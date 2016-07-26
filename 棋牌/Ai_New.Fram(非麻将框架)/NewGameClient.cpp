#include "stdafx.h"
#include "NewGameClient.h"
#include "IGameFrame.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "GameControlsIDDefine.h"
#include "Common/UIMessageDefines.h"
#include "LoveSendClassInExe.h"

static  bool  AIChooseCard = false ;   

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

IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
	return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
	delete *ppImp;
	*ppImp = NULL;
}

CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;
	m_SoundEngineGame = createIrrKlangDevice();
	m_SoundEnginBg = createIrrKlangDevice() ; 
	InitialGameData() ; 

}

CNewGameClient::~CNewGameClient()
{
	if(m_SoundEngineGame)
	{
		m_SoundEngineGame->drop();
	}

	if(m_SoundEnginBg)
	{
		m_SoundEnginBg->drop() ; 
	} 

}
//-----------------------------------------------------------------------------------
/// ��ʼ��
int CNewGameClient::Initial()
{
	//����UI
	wchar_t wszUIFile[MAX_PATH];
	::swprintf_s(wszUIFile,L"UI_%d_HH.dat",NAME_ID);
	m_pUI->LoadData(wszUIFile);

	return 0;
}
//-----------------------------------------------------------------------------------
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	//��ע��ť������
	SetBetBtEnable(false);
	//����ׯ����Ϣ
	ShowNtInfo(0,0,false);
	//������ׯ�б�
	ShowWaitNtInfo(false);
	//����·��
	ShowLuziList(false);
	//����������װ��ť
	SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000402,false);
	//�������
	for(int i=0; i<BET_ARES;i++)
	{
		SetShCtrlInfo(CTN_1000000_CTN_1000200_ShCtrl_1000271+i,0,false);
	}
	//�ȴ������ע��ʾ
	SetImageVisible(CTN_1000000_IMG_10000211,false);
	//������ʾ��ׯ
	ShowPromptDlg(false);
	//��ׯ�б�����������ť
	SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000403,false);
	SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000404,false);
	//���صȴ�ׯ�Ҳ�ǩ��ʾ
	SetImageVisible(CTN_1000000_IMG_10000202,false);
	//��������ׯ�Ҳ�ǩ����
	SetContainVisible(CTN_1000000_CTN_10000200,false);
	//��ǩ��ʾ�Ի���
	ShowBallotDlg(0,false);
	//���ؿ�ʼ��ť
	SetBtVisible(CTN_1000000_BTN_10001,false);
	//�رճ���
	ShowSuperResult(false);
	//��Ϸ���öԻ���
	ShowGameSetDlg(false);
	return 0;
}
//---------------------------------------------------------------------------------------
//����UI���� 
void	CNewGameClient::ReSetUiDlag()
{
	//��ע��ť������
	SetBetBtEnable(false);
	//���ؽ����
	ShowGameResult(0,0,0,false);
	//���������ע���
	ShowBetInfo(false);
	//�������
	for(int i=0; i<BET_ARES;i++)
	{
		SetShCtrlInfo(CTN_1000000_CTN_1000200_ShCtrl_1000271+i,0,false);
	}
	//��������ׯ�Ҳ�ǩ����
	SetContainVisible(CTN_1000000_CTN_10000200,false);
	//���صȴ�ׯ�Ҳ�ǩ��ʾ
	SetImageVisible(CTN_1000000_IMG_10000202,false);
	//��ǩ��ʾ�Ի���
	ShowBallotDlg(0,false);
	//���ؿ�ʼ��ť
	SetBtVisible(CTN_1000000_BTN_10001,false);
	//�رճ���
	ShowSuperResult(false);
}
//---------------------------------------------------------------------------------------
///��ʼ����Ϸ�е�����
void	CNewGameClient::InitialGameData()
{
	m_iMyDeskStation = 255;		///����Լ�����Ϸλ��
	m_byNtStation	 = 255;		//ׯ��λ��
	m_bIsSuperUser	 = false;	//�Ƿ񳬼��û�
	m_bIsReady		 = true;	//�Ƿ�׼����
	m_byGameStation	 = GS_WAIT_ARGEE;         ///��ǰ����Ϸ״̬
    
	m_byFreeTime	= 10;		//����ʱ��
	m_byBetTime		= 20;		//��עʱ��
	m_byNoBetTime	= 3;		//�����벻����ע

	m_byBallotTime	= 30;		//��ǩʱ��
	m_byOpenTime	= 10;		//����ʱ��
	m_byShowResultTime = 10;	//������ʾʱ��
	m_byRemaindTime	= 0;			//ʣ��ʱ��

	m_iApplyNtLimite= 0;		//������ׯ����
	m_i64MyBetCount = 0;		//�Լ�������ע
	m_i64SumAllBet	= 0;		//��������ע

	m_iCurrWaitNtTag = 0;		//��ǰ��ׯ�б��ǩ

	//�����������ע
	memset(m_i64AreaBetAllSum,0,sizeof(m_i64AreaBetAllSum));
	//�������ÿ���������ע���
	memset(m_i64UserBetCount,0,sizeof(m_i64UserBetCount));


	m_bySelChouMaType = 0;		//��ǰѡ��ĳ�������
	m_iBetSumLimite	  = 0;		//����ע����
	//��ע����������
	memset(m_iBetAreaLimite,0,sizeof(m_iBetAreaLimite));

	m_byLastResult	= RESULT_TaiPing;	//���ֿ������
	m_byCurrResult	= RESULT_TaiPing;	//���ֿ������
	m_byNtSelArea	= RESULT_ERR;		//ׯ�Ҳ�ǩ���
	m_bShowResult	= false;			//�Ƿ���ʾ�н�����
	memset(m_byLuziList,0,sizeof(m_byLuziList));		//·��
	for (int i=0; i<PLAY_COUNT; i++)
	{
		m_byWaitNtList[i] = 255;	//��ׯ�б�
	}
	//��Ϸ����
	LoadGameSet();
}
//---------------------------------------------------------------------------------------
///��ʼ����������
void	CNewGameClient::ResetGameData()
{
	
	m_bIsReady		= false;	//�Ƿ�׼����
	m_i64MyBetCount	= 0;		//�Լ�������ע
	m_i64SumAllBet	= 0;		//��������ע
	//�����������ע
	memset(m_i64AreaBetAllSum,0,sizeof(m_i64AreaBetAllSum));
	//�������ÿ���������ע���
	memset(m_i64UserBetCount,0,sizeof(m_i64UserBetCount));

	m_bySelChouMaType = 0;		//��ǰѡ��ĳ�������

	m_byRemaindTime	= 0;			//ʣ��ʱ��
	m_bShowResult	= false;			//�Ƿ���ʾ�н�����
}
//---------------------------------------------------------------------------------------
//��ȡ��Ϸ����
void	CNewGameClient::LoadGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	
	//������Ч
	m_bSoundBgPlay	= ("1" == f.GetKeyVal(key,"BgSound","1"));
	//��Ϸ��Ч
	m_bSoundPlay	= ("1" == f.GetKeyVal(key,"GameSound","1"));

}
//---------------------------------------------------------------------------------------
//������Ϸ����
void	CNewGameClient::SaveGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	

	IRadioButton *pRadioButton = NULL;
	//������Ч
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_1000000_CTN_10001100_RButton_10001101));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			//����û�в��� ��ô�����Ҫ��ʼ���ű���������
			if (!m_bSoundBgPlay)
			{
				m_bSoundBgPlay = true;
				PlayBgSound(true);
			}
			
			f.SetKeyValString(key,"BgSound","1");
		}
		else
		{
			//������ڲ��� ��ô��Ҫֹͣ��
			if (m_bSoundBgPlay)
			{
				m_bSoundBgPlay = false;
				if (NULL != m_SoundEnginBg)
				{
					m_SoundEnginBg->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"BgSound","0");
		}
	}
	//��Ϸ��Ч
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_1000000_CTN_10001100_RButton_10001103));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			m_bSoundPlay = true;
			f.SetKeyValString(key,"GameSound","1");
		}
		else
		{
			//������ڲ�����Ч ��ʱ��Ҫֹͣ
			if (m_bSoundPlay)
			{
				m_bSoundPlay = false;
				if (NULL != m_SoundEngineGame)
				{
					m_SoundEngineGame->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"GameSound","0");
		}
	}
}
//---------------------------------------------------------------------------------------
///ɾ�����ж�ʱ��
void CNewGameClient:: KillAllGameTime() 
{
	m_pGameFrame->KillTimer(TIME_GAME_BEGIN) ; 
	m_pGameFrame->KillTimer(TIME_NO_BET) ; 
	m_pGameFrame->KillTimer(TIME_RESULT_SHOW) ; 
	m_pGameFrame->KillTimer(TIME_ONE_TIME) ; 
}
//-----------------------------------------------------------------------------------------------
/// ������Ϸ�ĵ�ǰ״̬
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 
	m_bWatchMode = m_pGameFrame->GetIsWatching() ;
	CString dwjlog;
	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
	case GS_WAIT_NEXT:			//�ȴ����̿�ʼ
		{
			if (nLen != sizeof(GameStation_Base))
			{
				return;
			}
			GameStation_Base * pGameStation=(GameStation_Base *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬
			m_byFreeTime	= pGameStation->byFreeTime;		//����ʱ��
			m_byBetTime		= pGameStation->byBetTime;		//��עʱ��
			m_byNoBetTime	= pGameStation->byNoBetTime;	//�����벻����ע
			m_byNtStation	= pGameStation->byNtStation;	//ׯ��λ��
			m_byCurrResult	= pGameStation->byLastResult;	//���ֿ������
			m_byLastResult	= pGameStation->byLastResult;	//���ֿ������
			m_iApplyNtLimite= pGameStation->iApplyNtLimite;	//��ׯ����
			//��ׯ�б�
			for(int i=0; i<PLAY_COUNT; i++)
			{
				m_byWaitNtList[i] = pGameStation->byWaitNtList[i];
			}

			//·��
			for(int i=0; i<10; i++)
			{
				m_byLuziList[i] = pGameStation->byLuziList[i];
			}

			//��ʾׯ����Ϣ
			ShowNtInfo(pGameStation->i64NtHaveWin, pGameStation->iNtPlayCount,true);
			//��ʾ�Լ�����Ϣ
			ShowMyInfo(pGameStation->i64MyWinMoney,true);

			break;
		}
	case GS_BALLOT_STATUS:		//��ǩ״̬
		{
			if (nLen != sizeof(GameStation_Base))
			{
				return;
			}
			GameStation_Base * pGameStation=(GameStation_Base *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬

			m_byFreeTime	= pGameStation->byFreeTime;		//����ʱ��
			m_byBetTime		= pGameStation->byBetTime;		//��עʱ��
			m_byNoBetTime	= pGameStation->byNoBetTime;	//�����벻����ע
			m_byNtStation	= pGameStation->byNtStation;	//ׯ��λ��
			m_byCurrResult	= pGameStation->byLastResult;	//���ֿ������
			m_byLastResult	= pGameStation->byLastResult;	//���ֿ������
			m_iApplyNtLimite= pGameStation->iApplyNtLimite;	//��ׯ����
			//��ׯ�б�
			for(int i=0; i<PLAY_COUNT; i++)
			{
				m_byWaitNtList[i] = pGameStation->byWaitNtList[i];
			}

			//·��
			for(int i=0; i<10; i++)
			{
				m_byLuziList[i] = pGameStation->byLuziList[i];
			}

			//��ʾׯ����Ϣ
			ShowNtInfo(pGameStation->i64NtHaveWin, pGameStation->iNtPlayCount,true);
			//��ʾ�Լ�����Ϣ
			ShowMyInfo(pGameStation->i64MyWinMoney,true);
			//�����ׯ�� ��ȥ��ʾ��ǩ
			if (m_iMyDeskStation == m_byNtStation)
			{
				//��ʾ����ׯ�Ҳ�ǩ��ʾ
				SetContainVisible(CTN_1000000_CTN_10000200,true);
				SetBtVisible(CTN_1000000_CTN_10000200_BTN_10000210,true);
				//��ע��ť������
				SetBetBtEnable(false);
			}
			else
			{
				//��ʾ�ȴ�ׯ�Ҳ�ǩ��ʾ
				SetImageVisible(CTN_1000000_IMG_10000202,true);
				//��ע��ť����
				SetBetBtEnable(true);
			}
			break;
		}

	case GS_BET_STATUS:	//��Ϸ��ע�׶�
		{
			if (nLen != sizeof(GameStation_Bet))
			{
				return;
			}
			GameStation_Bet * pGameStation=(GameStation_Bet *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬
			m_byFreeTime	= pGameStation->byFreeTime;		//����ʱ��
			m_byBetTime		= pGameStation->byBetTime;		//��עʱ��
			m_byNoBetTime	= pGameStation->byNoBetTime;	//�����벻����ע
			m_byNtStation	= pGameStation->byNtStation;	//ׯ��λ��
			m_byCurrResult	= pGameStation->byLastResult;	//���ֿ������
			m_byLastResult	= pGameStation->byLastResult;	//���ֿ������
			m_iApplyNtLimite= pGameStation->iApplyNtLimite;	//��ׯ����
			//��ׯ�б�
			for(int i=0; i<PLAY_COUNT; i++)
			{
				m_byWaitNtList[i] = pGameStation->byWaitNtList[i];
			}

			//·��
			for(int i=0; i<10; i++)
			{
				m_byLuziList[i] = pGameStation->byLuziList[i];
			}

			//�������ÿ���������ע���
			memcpy(m_i64UserBetCount,pGameStation->i64UserBetCount,sizeof(m_i64UserBetCount));
			//�����������ע
			for(int i=0; i<BET_ARES; i++)
			{
				for (int j =0; j<PLAY_COUNT; j++)
				{
					m_i64AreaBetAllSum[i] = m_i64UserBetCount[j][i];
					//����ע
					m_i64SumAllBet += m_i64UserBetCount[j][i];
				}
				//��ʾ������ע����
				SetShCtrlInfo((CTN_1000000_CTN_1000200_ShCtrl_1000271+i),m_i64AreaBetAllSum[i],true);
			}
			
			
			//��ʾׯ����Ϣ
			ShowNtInfo(pGameStation->i64NtHaveWin, pGameStation->iNtPlayCount,true);
			//��ʾ�Լ�����Ϣ
			ShowMyInfo(pGameStation->i64MyWinMoney,true);
			//�����ׯ��
			if (m_iMyDeskStation == m_byNtStation)
			{
				//��ʾ�ȴ������ע
				SetImageVisible(CTN_1000000_IMG_10000211,true);
				//��ע��ť������
				SetBetBtEnable(false);
			}
			else
			{
				//��ע��ť����
				SetBetBtEnable(true);
			}
			//��ʾ��ע���
			ShowBetInfo(true);
			//dwjtest ��ʾʣ�൹��ʱ
			break;;
		}
	case GS_OPEN_RESULT:		//��ǩ�׶�
		{
			if (nLen != sizeof(GameStation_OpenResult))
			{
				return;
			}
			GameStation_OpenResult * pGameStation=(GameStation_OpenResult *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬
			m_byFreeTime	= pGameStation->byFreeTime;		//����ʱ��
			m_byBetTime		= pGameStation->byBetTime;		//��עʱ��
			m_byNoBetTime	= pGameStation->byNoBetTime;	//�����벻����ע
			m_byNtStation	= pGameStation->byNtStation;	//ׯ��λ��
			//m_byCurrResult	= pGameStation->byLastResult;	//���ֿ������
			m_byLastResult	= pGameStation->byLastResult;	//���ֿ������
			m_iApplyNtLimite= pGameStation->iApplyNtLimite;	//��ׯ����
			m_byCurrResult	= pGameStation->byCurrResult;	//��ǰ�������
			//��ׯ�б�
			for(int i=0; i<PLAY_COUNT; i++)
			{
				m_byWaitNtList[i] = pGameStation->byWaitNtList[i];
			}

			//·��
			for(int i=0; i<10; i++)
			{
				m_byLuziList[i] = pGameStation->byLuziList[i];
			}

			//�������ÿ���������ע���
			memcpy(m_i64UserBetCount,pGameStation->i64UserBetCount,sizeof(m_i64UserBetCount));
			//�����������ע
			for(int i=0; i<BET_ARES; i++)
			{
				for (int j =0; j<PLAY_COUNT; j++)
				{
					m_i64AreaBetAllSum[i] = m_i64UserBetCount[j][i];
					//����ע
					m_i64SumAllBet += m_i64UserBetCount[j][i];
				}
				//��ʾ������ע����
				SetShCtrlInfo((CTN_1000000_CTN_1000200_ShCtrl_1000271+i),m_i64AreaBetAllSum[i],true);
			}

			//��ʾׯ����Ϣ
			ShowNtInfo(pGameStation->i64NtHaveWin, pGameStation->iNtPlayCount,true);
			//��ʾ�Լ�����Ϣ
			ShowMyInfo(pGameStation->i64MyWinMoney,true);
			//��ע��ť������
			SetBetBtEnable(false);
			//�����н�������˸����
			m_bShowResult = true;
			m_pGameFrame->SetTimer(TIME_RESULT_SHOW,1000);

			break;
		}
	case GS_SHOW_RESULT:		//��ʾ�����׶�
		{
			if (nLen != sizeof(GameStation_ShowResult))
			{
				return;
			}
			GameStation_ShowResult * pGameStation=(GameStation_ShowResult *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			m_byGameStation = pGameStation->bGameStation;	//��Ϸ״̬
			m_byFreeTime	= pGameStation->byFreeTime;		//����ʱ��
			m_byBetTime		= pGameStation->byBetTime;		//��עʱ��
			m_byNoBetTime	= pGameStation->byNoBetTime;	//�����벻����ע
			m_byNtStation	= pGameStation->byNtStation;	//ׯ��λ��
			m_byCurrResult	= pGameStation->byLastResult;	//���ֿ������
			m_byLastResult	= pGameStation->byLastResult;	//���ֿ������
			m_iApplyNtLimite= pGameStation->iApplyNtLimite;	//��ׯ����

			//��ׯ�б�
			for(int i=0; i<PLAY_COUNT; i++)
			{
				m_byWaitNtList[i] = pGameStation->byWaitNtList[i];
			}

			//·��
			for(int i=0; i<10; i++)
			{
				m_byLuziList[i] = pGameStation->byLuziList[i];
			}

			//��ʾׯ����Ϣ
			ShowNtInfo(pGameStation->i64NtHaveWin, pGameStation->iNtPlayCount,true);
			//��ʾ�Լ�����Ϣ
			ShowMyInfo(pGameStation->i64MyWinMoney,true);
			//��ע��ť������
			SetBetBtEnable(false);
			//��ʾ����ʱdwjtest
			
			break;
		}
	default:
		{
			break;
		}
	}
	//��ʾ�����н�����
	SetResultPosition(true);
	//��ʾ·��
	ShowLuziList(true);
	//��ʾ��ׯ�б�
	ShowWaitNtInfo(true);
	
	dwjlog.Format("dwjlog::SETGAMESTATION_m_byNtStation=%d",m_byNtStation);
	OutputDebugString(dwjlog);
	//��ǰ��ׯ 
	if (m_byNtStation == 255)
	{
		ShowPromptDlg(true);
	}
	else
	{
		ShowPromptDlg(false);
	}
	//���ű�����Ч
	PlayBgSound(true);
	return  ; 
}
//-----------------------------------------------------------------------------------------
/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 
	switch(nSubId)
	{
	case ASS_GM_GAME_STATION:   ///<����״̬
		{
			SetGameStation(buffer, nLen);    
			break;        
		}
	case ASS_GM_AGREE_GAME:
		{		
			if(nLen!=sizeof(MSG_GR_R_UserAgree) || m_byGameStation != GS_SHOW_RESULT)
			{
				return 0;
			}				

			MSG_GR_R_UserAgree * pUserAgree=(MSG_GR_R_UserAgree *)buffer;
			if (NULL == pUserAgree)
			{
				return 0;
			}
			if (pUserAgree->bDeskStation == m_iMyDeskStation)
			{
				m_bIsReady = true;	//�Ѿ�׼����
				//������Ϸ����
				ReSetUiDlag();
			}
			break;
		}
	case ASS_SUPER_USER:	//���˿ͻ���
		{
			if(nLen!=sizeof(SUPERSTATE))
			{
				return 0;
			}	
			SUPERSTATE *pSuperUser = (SUPERSTATE *) buffer;
			if (NULL == pSuperUser)
			{
				return 0;
			}
			if (pSuperUser->byDeskStation == m_iMyDeskStation)
			{
				m_bIsSuperUser	 = true;	//�Ƿ񳬼��û�
			}
			break;
		}
	case ASS_GAME_BEGIN:	//��Ϸ��ʼ
		{		
			if(nLen!=sizeof(GameBeginMsg))
			{
				return 0;
			}	

			GameBeginMsg * pGameBegin=(GameBeginMsg *)buffer;
			if (NULL == pGameBegin)
			{
				return 0;
			}
			if (pGameBegin->bGameBegin)
			{
				m_byBallotTime = pGameBegin->byBallotTime;
				m_byNtStation = pGameBegin->byNtStation;
				if (pGameBegin->byNtStation != 255)
				{
					ShowNtInfo(pGameBegin->i64NtHaveWin,pGameBegin->iNtPlayCount,true);
				}
				//��Ϸ��ʼׯ�Ҳ�ǩ
				HandleGameBegin();		
			}
			break;
		}
	case ASS_BEGIN_BET:	//��ʼ��ע
		{
			if(nLen!=sizeof(BeginBetMsg))
			{
				return 0;
			}	
			
			HandleBeginBet(buffer);
			//���ſ�ʼ��ע��Ч
			PlayGameSound(SOUND_BEGIN_BET);
			break;
		}
	case ASS_OPEN_RESULT:	//��ǩ
		{
			if(nLen!=sizeof(OpenResultMsg))
			{
				return 0;
			}	
			HandleOpenResult(buffer);
			break;
		}
	case ASS_GAME_FINISH:	//��Ϸ���� ��ʾ�����
		{
			if(nLen!=sizeof(GameEndMsg))
			{
				return 0;
			}	
			HandleGameResult(buffer);
			break;
		}
	case ASS_GAME_FREE:	//����
		{
			if(nLen!=sizeof(GameFreeMsg))
			{
				return 0;
			}
			HandleGameFree(buffer);
			break;
		}
	case ASS_APPLY_RESULT:	//������ׯ�����Ϣ
		{
			if(nLen!=sizeof(ApplyNtResultMsg))
			{
				return 0;
			}
			
			HandleApplyResult(buffer);
			break;
		}
	case ASS_LEAVE_RESULT:	//������ׯ���
		{
			if(nLen!=sizeof(ApplyNtResultMsg))
			{
				return 0;
			}
			HandleLeaveNtResult(buffer);
			break;
		}
	case ASS_BET_RESULT:	//��ע���
		{
			if(nLen!=sizeof(UserBetResultMsg))
			{
				return 0;
			}
			HandleUserBetResult(buffer);
			break;
		}
	default:
		{
			break;
		}
		
	}
	return 0;
}
//------------------------------------------------------------------------------
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	///��ť�������Ϣ 
	if(pMessage->_uMessageType == UI_LBUTTONUP)
	{
		switch(pMessage->_uControlID)
		{
		case  CTN_1000000_CTN_1000100_BTN_1000101:	//100����
		case  CTN_1000000_CTN_1000100_BTN_1000102:	//1000����
		case  CTN_1000000_CTN_1000100_BTN_1000103:	//10000����
		case  CTN_1000000_CTN_1000100_BTN_1000104:	//100000����
		case  CTN_1000000_CTN_1000100_BTN_1000105:	//1000000����
		case  CTN_1000000_CTN_1000100_BTN_1000106:	//10000000����
			{
				CString sImageSrc;
				sImageSrc.Format(".\\resources\\image\\chouma_%d.png",(pMessage->_uControlID-CTN_1000000_CTN_1000100_BTN_1000101));
				m_pUI->GetICursor()->SetCursorImage(CA2W(sImageSrc));
				m_pUI->GetICursor()->SetSrcW(m_pUI->GetICursor()->GetCursorImageWidth()/4);
				m_pUI->GetICursor()->SetPointMode(1);
				m_pUI->GetICursor()->SetControlVisible(true);
				//��ǰѡ��ĳ�������ֵ
				m_bySelChouMaType = pMessage->_uControlID - CTN_1000000_CTN_1000100_BTN_1000101 +1;
				break;
			}
		case CTN_1000000_CTN_1000400_BTN_1000401:	//������ׯ
			{
				OutputDebugString("dwjlog::CTN_1000000_CTN_1000400_BTN_1000401");
				//������ׯ
				UserApplyNtStation();
				break;
			}
		case CTN_1000000_CTN_1000400_BTN_1000402:	//������ׯ
			{
				//������ׯ
				UserApplyLiveNt();
				break;
			}
		case CTN_1000000_CTN_1000200_BTN_1000201:	///<���
		case CTN_1000000_CTN_1000200_BTN_1000202:	///<����  
		case CTN_1000000_CTN_1000200_BTN_1000203:	///<̫ƽ  
		case CTN_1000000_CTN_1000200_BTN_1000204:	///<��Ʒ  
		case CTN_1000000_CTN_1000200_BTN_1000205:	///<����  
		case CTN_1000000_CTN_1000200_BTN_1000206:	///<��˳  
		case CTN_1000000_CTN_1000200_BTN_1000207:	///<��ͬ  
		case CTN_1000000_CTN_1000200_BTN_1000208:	///<��Ԫ  
		case CTN_1000000_CTN_1000200_BTN_1000209:	///<Ԫ��  
		case CTN_1000000_CTN_1000200_BTN_1000210:	///<����  
		case CTN_1000000_CTN_1000200_BTN_1000211:	///<ռ��  
		case CTN_1000000_CTN_1000200_BTN_1000212:	///<־��  
			{
				//��ע״̬
				if (m_byGameStation == GS_BET_STATUS)
				{
					//�����ע
					UserBet((pMessage->_uControlID - CTN_1000000_CTN_1000200_BTN_1000201 +1));
				}
				//��ǩ״̬
				if (m_byGameStation == GS_BALLOT_STATUS)
				{
					//ׯ�ҲŲ�ǩ
					if (m_byNtStation == m_iMyDeskStation)
					{
						//ׯ�Ҳ�ǩ
						NtSelBallot((pMessage->_uControlID - CTN_1000000_CTN_1000200_BTN_1000201 +1));
					}
				}
				
				break;
			}
		case CTN_1000000_BTN_1001300:    //��Ǯ
		case CTN_1000000_BTN_1001400:    //ȡǮ
			{
				m_pGameFrame->OnVisitBank();
				break;
			}
		case CTN_1000000_CTN_1000700_BTN_1000702:	//ͬ����ׯ
			{
				//������ׯ
				UserApplyNtStation();
				ShowPromptDlg(false);
				break;
			}
		case CTN_1000000_CTN_1000700_BTN_1000703:	//��ͬ����ׯ
			{
				ShowPromptDlg(false);
				break;
			}
		case CTN_1000000_CTN_1000400_BTN_1000403:	//��һ��
			{
				if (m_iCurrWaitNtTag > 0)
				{
					m_iCurrWaitNtTag--;
					ShowWaitNtInfo(true);
				}	
				break;
			}
		case CTN_1000000_CTN_1000400_BTN_1000404:	//��һ��
			{
				m_iCurrWaitNtTag++;
				break;
			}
		case CTN_1000000_CTN_10000200_BTN_10000210:	//ׯ��ȷ��ȥ��ǩ
			{
				SetContainVisible(CTN_1000000_CTN_10000200,false);
				break;
			}
		case CTN_1000000_CTN_1000800_BTN_1000803:	//��ǩȷ��
			{
				//����ׯ�Ҳ�ǩȷ�� m_byNtSelArea
				NtSureBallot();
				//��ǩ��ʾ�Ի���
				ShowBallotDlg(0,false);
				break;
			}
		case CTN_1000000_CTN_1000800_BTN_1000804:	//��ǩȷ��
			{
				//��ǩ��ʾ�Ի���
				ShowBallotDlg(0,false);
				break;
			}
		case CTN_1000000_CTN_1001000_BTN_1001001:	//�رճ���
		case CTN_1000000_CTN_1001000_BTN_1001004:	//ȷ��
			{
				//�رճ���
				ShowSuperResult(false);
				break;
			}
		case CTN_2_BTN_1003500:	//��Ϸ����
			{
				//��ʾ��Ϸ���öԻ���
				ShowGameSetDlg(true);
				break;
			}
		case CTN_1000000_CTN_10001100_BTN_10001105:	//ȷ������
			{
				//������Ϸ����
				SaveGameSet();
				//�ر���Ϸ���öԻ���
				ShowGameSetDlg(false);
				break;
			}
		case CTN_1000000_CTN_10001100_BTN_10001106:	//ȡ������
			{
				//�ر���Ϸ���öԻ���
				ShowGameSetDlg(false);
				break;
			}
		}
		return 0;
	}
	//�һ�
	if(pMessage->_uMessageType == UI_RBUTTONDOWN)
	{
		OutputDebugString("dwjlog::UI_RBUTTONDOWN");
		if (m_pUI->GetICursor()->GetControlVisible())
		{
			m_pUI->GetICursor()->SetControlVisible(false);
		}
		m_bySelChouMaType = 0;
	}
	//���̰���
	if (pMessage->_uMessageType == UI_KEYDOWN)
	{
		//����F12 ��������
		if (pMessage->_ch == VK_F12 && m_bIsSuperUser && m_byGameStation == GS_BET_STATUS)
		{
			//��ʾ���˽������
			ShowSuperResult(true);
		}
	}
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		switch(pMessage->_uControlID)
		{
		case  CTN_1000000_ExAnimate_10000215:	//��ǩ�����������
			{
				//��ʾ�ȴ�ׯ�Ҳ�ǩ��ʾ
				SetImageVisible(CTN_1000000_IMG_10000202,true);
//				SetAnimateShowFrame(CTN_1000000_ExAnimate_10000215,true,-1);
				break;
			}
		case CTN_1000000_ExAnimate_10000216:	//��ǩ����
			{
				SetAnimateShowFrame(CTN_1000000_ExAnimate_10000216,true,-1);
				//�����н�������˸����
				m_bShowResult = true;
				m_pGameFrame->SetTimer(TIME_RESULT_SHOW,2000);
				break;
			}
		case CTN_1000000_CTN_1000100_Time_1000199:
			{
				if (m_byGameStation == GS_SHOW_RESULT && !m_bIsReady)
				{
					OutputDebugString("dwjlog::CTN_1000000_CTN_1000100_Time_1000199-GS_SHOW_RESULT");
					m_pGameFrame->CloseClient();
				}
				break;
			}
		default:
			{
				break;
			}
		}
		return 0 ; 
	}
	if(pMessage->_uMessageType == UI_MOUSEENTER)
	{
		switch(pMessage->_uControlID)
		{
		case CTN_1000000_CTN_1000400_BTN_1000401:	//������ׯ
		case CTN_1000000_CTN_1000400_BTN_1000402:	//������ׯ
		case CTN_1000000_CTN_1000100_BTN_1000101:	//����
		case CTN_1000000_CTN_1000100_BTN_1000102:	//����
		case CTN_1000000_CTN_1000100_BTN_1000103:	//����
		case CTN_1000000_CTN_1000100_BTN_1000104:	//����
		case CTN_1000000_CTN_1000100_BTN_1000105:	//����
		case CTN_1000000_CTN_1000100_BTN_1000106:	//����
		case CTN_1000000_BTN_1001400:	//ȡǮ
		case CTN_1000000_BTN_1001300:	//��Ǯ
			{
				//������Ч
				PlayGameSound(SOUND_MOUSE_ENTER);
				break;
			}
		default:
			{
				break;
			}
		}
		
		return 0;
	}

	return 0;
}
//---------------------------------------------------------------------------------
//������ׯ
void	CNewGameClient::UserApplyNtStation()
{
	//��Ҳ���
	if (GetMyRemaindMoney() < m_iApplyNtLimite)
	{
		//������Ϣ	dwjtest
		return;
	}

	if (m_iMyDeskStation == m_byNtStation )
	{
		return;
	}
	
	if (IsInWaitNtList())
	{
		return;
	}
	ApplyNtMsg	TApplyNt;

	TApplyNt.byDeskStation = m_iMyDeskStation;
	TApplyNt.bApplay	= true;
	OutputDebugString("dwjlog::UserApplyNtStation()");
	m_pGameFrame->SendGameData(&TApplyNt, sizeof(TApplyNt) ,MDM_GM_GAME_NOTIFY,ASS_APPLY_NT,0);

}
//--------------------------------------------------------------------------------
//������ׯ
void	CNewGameClient::UserApplyLiveNt()
{
	if (!IsInWaitNtList() && m_iMyDeskStation != m_byNtStation)
	{
		return;
	}
	ApplyNtMsg	TApplyNt;

	TApplyNt.byDeskStation = m_iMyDeskStation;
	TApplyNt.bApplay	= false;

	m_pGameFrame->SendGameData(&TApplyNt, sizeof(TApplyNt) ,MDM_GM_GAME_NOTIFY,ASS_APPLY_LEAVE,0);
}
//--------------------------------------------------------------------------------
//�����ע
void	CNewGameClient::UserBet(int byBetArea)
{
	if (byBetArea < 1 || byBetArea > 12)
	{
		return;
	}
	//û��ѡ�г���
	if (0 == m_bySelChouMaType)
	{
		return;
	}
	
	//��ע�������޶� �Ͳ�����ע
	if (m_iBetSumLimite > 0 && m_i64MyBetCount + G_iChouMaValue[m_bySelChouMaType] > m_iBetSumLimite )
	{
		//������Ϣdwjtest
		m_pUI->GetICursor()->SetControlVisible(false);
		return;
	}
	//��ĳ��������ע�����޶� 
	if (m_iBetAreaLimite[byBetArea] > 0 && (m_i64UserBetCount[m_iMyDeskStation][byBetArea]) + G_iChouMaValue[m_bySelChouMaType] > m_iBetAreaLimite[byBetArea])
	{
		//������Ϣdwjtest
		return;
	}
	//ׯ�ҹ�������
	if ((m_i64AreaBetAllSum[byBetArea]+G_iChouMaValue[m_bySelChouMaType] )*G_iArePeiLv[byBetArea] > GetUserRemaindMoney(m_byNtStation))
	{
		//������Ϣdwjtest
		return;
	}

	UserBetMsg TUserBet;
	TUserBet.byDeskStation	= m_iMyDeskStation;
	TUserBet.byBetArea		= byBetArea;
	TUserBet.byChouMaType	= m_bySelChouMaType;

	m_pGameFrame->SendGameData(&TUserBet, sizeof(TUserBet) ,MDM_GM_GAME_NOTIFY,ASS_USER_BET,0);

}
//--------------------------------------------------------------------------------
///ׯ�Ҳ�ǩ
void	CNewGameClient::NtSelBallot(int iBetArea)
{
	//���ܲ�������һ����ǩ
	if (iBetArea == m_byLastResult)
	{
		return;
	}
	//ׯ�Ҳ�ǩ���
	m_byNtSelArea	= iBetArea;		
	//��ʾ��ǩȷ����ʾ�Ի���
	ShowBallotDlg(iBetArea,true);

}
//--------------------------------------------------------------------------------
//��ʾ��ǩȷ����ʾ�Ի���
void	CNewGameClient::ShowBallotDlg(int iBetArea, bool bFlag)
{
	//����
	SetContainVisible(CTN_1000000_CTN_1000800,bFlag);
	SetBtVisible(CTN_1000000_CTN_1000800_BTN_1000803,bFlag);
	SetBtVisible(CTN_1000000_CTN_1000800_BTN_1000804,bFlag);
	CString sFilePath;
	sFilePath.Format(".\\resources\\image\\LuZi\\LuZi_%d.jpg",iBetArea);
	SetImageLoadSrc(CTN_1000000_CTN_1000800_IMG_1000802,sFilePath,bFlag);
	switch(iBetArea)
	{
	case 	RESULT_WanJin:		//���
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[���]",bFlag);
			break;
		}
	case	RESULT_YinYu:		//����
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[����]",bFlag);
			break;
		}
	case	RESULT_TaiPing:		//̫ƽ
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[̫ƽ]",bFlag);
			break;
		}
	case	RESULT_JiPin:		//��Ʒ
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[��Ʒ]",bFlag);
			break;
		}
	case	RESULT_JingLin:		//����
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[����]",bFlag);
			break;
		}
	case	RESULT_ZhengShun:	//��˳
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[��˳]",bFlag);
			break;
		}
	case	RESULT_HeTong:		//��ͬ
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[��ͬ]",bFlag);
			break;
		}
	case	RESULT_QingYuan:	//��Ԫ
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[��Ԫ]",bFlag);
			break;
		}
	case	RESULT_YuanGui:		//Ԫ��
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[Ԫ��]",bFlag);
			break;
		}
	case	RESULT_ShangZhao:	//����
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[����]",bFlag);
			break;
		}
	case	RESULT_ZhanKui:		//ռ��
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[ռ��]",bFlag);
			break;
		}
	case	RESULT_ZhiGao:		//־��
		{
			SetTextinfo(CTN_1000000_CTN_1000800_TEXT_1000801,"��ѡ���ǩ��[־��]",bFlag);
			break;
		}
	}
	
}
//-------------------------------------------------------------------------------------------------------
//��ʾ���˽������
void	CNewGameClient::ShowSuperResult(bool bFlag)
{
	if (m_bIsSuperUser)
	{
		//����
		SetContainVisible(CTN_1000000_CTN_1001000,bFlag);		///<���˽���  ����
		SetBtVisible(CTN_1000000_CTN_1001000_BTN_1001001,bFlag);//<�ر� 
		SetBtVisible(CTN_1000000_CTN_1001000_BTN_1001004,bFlag);///<ȷ��
		//�������ͼƬ
		CString sFilePath;
		sFilePath.Format(".\\resources\\image\\LuZi\\LuZi_%d.jpg",m_byCurrResult);
		SetImageLoadSrc(CTN_1000000_CTN_1001000_IMG_1001003,sFilePath,bFlag);

		//������ʾ
		switch(m_byCurrResult)
		{
		case 	RESULT_WanJin:		//���
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[���]",bFlag);
				break;
			}
		case	RESULT_YinYu:		//����
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[����]",bFlag);
				break;
			}
		case	RESULT_TaiPing:		//̫ƽ
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[̫ƽ]",bFlag);
				break;
			}
		case	RESULT_JiPin:		//��Ʒ
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[��Ʒ]",bFlag);
				break;
			}
		case	RESULT_JingLin:		//����
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[����]",bFlag);
				break;
			}
		case	RESULT_ZhengShun:	//��˳
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[��˳]",bFlag);
				break;
			}
		case	RESULT_HeTong:		//��ͬ
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[��ͬ]",bFlag);
				break;
			}
		case	RESULT_QingYuan:	//��Ԫ
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[��Ԫ]",bFlag);
				break;
			}
		case	RESULT_YuanGui:		//Ԫ��
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[Ԫ��]",bFlag);
				break;
			}
		case	RESULT_ShangZhao:	//����
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[����]",bFlag);
				break;
			}
		case	RESULT_ZhanKui:		//ռ��
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[ռ��]",bFlag);
				break;
			}
		case	RESULT_ZhiGao:		//־��
			{
				SetTextinfo(CTN_1000000_CTN_1001000_TEXT_1001002,"����ׯ�ҵĲ�ǩ��[־��]",bFlag);
				break;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------
//��ʾ��Ϸ���öԻ���
void	CNewGameClient::ShowGameSetDlg(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_10001100,bFlag);
	//������Ч
	SetRButtonSelected(CTN_1000000_CTN_10001100_RButton_10001101,m_bSoundBgPlay,bFlag);
	//��Ϸ��Ч
	SetRButtonSelected(CTN_1000000_CTN_10001100_RButton_10001103,m_bSoundPlay,bFlag);
}
//-------------------------------------------------------------------------------------------------------
//����ׯ�Ҳ�ǩȷ�� 
void 	CNewGameClient::NtSureBallot()
{
	if (m_byNtSelArea < RESULT_WanJin || m_byNtSelArea > RESULT_ZhiGao )
	{
		return;
	}

	NtSelAreaMsg	TNtSelArea;
	TNtSelArea.byDeskStation = m_iMyDeskStation;
	TNtSelArea.bySelArea = m_byNtSelArea;
	m_pGameFrame->SendGameData(&TNtSelArea, sizeof(TNtSelArea) ,MDM_GM_GAME_NOTIFY,ASS_SEL_AREA,0);
}
//--------------------------------------------------------------------------------
//�Ƿ�����ׯ�б���
bool	CNewGameClient::IsInWaitNtList()
{
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (m_byWaitNtList[i] == m_iMyDeskStation)
		{
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------------
//��ȡ���ʣ��Ľ��
__int64		CNewGameClient::GetMyRemaindMoney()
{
	__int64 i64Money = 0;
	UserInfoForGame UserInfo ; 
	if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
	{
		i64Money = UserInfo.i64Money - m_i64MyBetCount;
	}
	return i64Money;
}
//---------------------------------------------------------------------------------
//��ȡָ����ҵ�ʣ����
__int64		CNewGameClient::GetUserRemaindMoney(BYTE byDestStation)
{
	__int64 i64Money = 0;
	UserInfoForGame UserInfo ; 
	if (m_pGameFrame->GetUserInfo(byDestStation , UserInfo))
	{
		i64Money = UserInfo.i64Money - GetUserBetSum(byDestStation);
	}
	return i64Money;
}
//---------------------------------------------------------------------------------
//��ȡĳλ��ҵ�����ע���
__int64		CNewGameClient::GetUserBetSum(BYTE byDestStation)
{
	if (byDestStation > PLAY_COUNT)
	{
		return 0;
	}
	__int64 i64Money = 0;
	for(int i=0; i< BET_ARES; i++)
	{
		i64Money += m_i64UserBetCount[byDestStation][i];
	}
	return i64Money;
}
//---------------------------------------------------------------------------------
/// ������Ϸ״̬
int CNewGameClient::ResetGameFrame(void)
{
	return 0;
}
/// ��ҽ���
int CNewGameClient::GameUserCome(void)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 
	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	return 0;
}
/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	return 0;
}
// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	return true;
}
//--------------------------------------------------------------------
/// ��ʱ����Ϣ����Ϸ�еĶ�ʱ������
void CNewGameClient::OnTimer(int nTimerID)
{
 	switch(nTimerID)
 	{
 	case  TIME_GAME_BEGIN :
 		{
 			m_pGameFrame->KillTimer(TIME_GAME_BEGIN);
 			if(m_byGameStation == GS_BALLOT_STATUS)
 			{
				//���Ų�ǩ����
				SetAnimatePlay(CTN_1000000_ExAnimate_10000215,true);
 			} 			
 			break ; 
 		}
	case TIME_NO_BET:	//�����벻����ע
		{
			m_pGameFrame->KillTimer(TIME_NO_BET);
			m_pUI->GetICursor()->SetControlVisible(false);
			SetBetBtEnable(false);
			break;
		}
	case TIME_ONE_TIME:	//1�뵹��ʱ
		{
			m_byRemaindTime--;
			if (m_byGameStation == GS_BET_STATUS)
			{
				if (m_byRemaindTime <= 5 )
				{
					//������Ч
					PlayGameSound(SOUND_COUNTDWON);
				}
			}
			if (m_byRemaindTime <= 0)
			{
				m_pGameFrame->KillTimer(TIME_ONE_TIME);
			}
			break;
		}
	case TIME_RESULT_SHOW:
		{
			m_pGameFrame->KillTimer(TIME_RESULT_SHOW);
			//���س�ǩ����
			SetAnimatePlay(CTN_1000000_ExAnimate_10000216,false);
			if (m_byGameStation == GS_OPEN_RESULT)
			{
				m_pGameFrame->SetTimer(TIME_RESULT_SHOW,500);
				if (m_bShowResult)
				{
					m_bShowResult = false;
				}
				else
				{
					m_bShowResult = true;
				}
				//��ʾ�н�����
				SetResultPosition(m_bShowResult);
			}
			break;
		}
	default:
		{
			break;
		}
	}
	return ; 

}
//-------------------------------------------------------------------------------
/// ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_byGameStation = bGameStation ; 
}
//-------------------------------------------------------------------------------
///��ȡ��Ϸ״̬
BYTE CNewGameClient::GetStationParameter()
{
	return m_byGameStation;
}
//-------------------------------------------------------------------------------
///����˳���Ϸ
bool CNewGameClient::OnGameQuiting()
{
// 	if(m_pGameFrame->GetIsWatching())
// 	{
// 		return true ; 
// 	}
// 
// 	CString szAppPath = CINIFile::GetAppPath();
// 	szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
// 	int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
// 	CString szAppPathParent = szAppPath.Left(nPosOfSlash);
// 
// 
// 	CString strKeyName  ; 
// 	strKeyName.Format("%d" ,NAME_ID) ; 
// 	CBcfFile File(szAppPathParent + "\\SpecialRule.bcf") ; 
// 
// 	bool   bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strKeyName ,1) ; 
// 
// 	if(GetStationParameter() >=GS_SEND_CARD && GetStationParameter() <=GS_PLAY_GAME)
// 	{
// 		if(bForceQuitAsAuto)
// 		{
// 			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪��Ϸ�����йܴ��� ���Ƿ�Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
// 			{
// 				return false ; 
// 			}
// 
// 			//if(IDYES != DUIMessageBox(m_pUI->GetWindowsHwnd() , MB_YESNO|MB_ICONQUESTION,"��ʾ" , false , "��������Ϸ��, �����뿪��Ϸ�����йܴ��� ���Ƿ�Ҫ�뿪��"))
// 			//{
// 				//return false ; 
// 			//}		
// 		}
// 		else
// 		{
// 			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪���ᱻ�۷����Ƿ����Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
// 			{
// 				return false ; 
// 			}
// 			//if(IDYES != DUIMessageBox(m_pUI->GetWindowsHwnd() , MB_YESNO|MB_ICONQUESTION,"��ʾ" , false , "��������Ϸ��, �����뿪���ᱻ�۷����Ƿ����Ҫ�뿪��"))
// 			//{
// 			//	return false ; 
// 			//}	
// 
// 			m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
// 		}
// 	}

	return true;
}
//---------------------------------------------------------------------------------------
//������Ϸ��Ч
void	CNewGameClient::PlayGameSound(int SoundID, bool bISLoop )
{
	if(!m_SoundEngineGame)
	{
		return ;
	}
	if(!m_bSoundPlay)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	
	switch(SoundID)
	{
	case SOUND_GAME_BEGIN://��Ϸ��ʼ
		{
			wsprintf(szFileName,"music\\sound\\BeGin.ogg");
			break;
		}
	case SOUND_BEGIN_BET://��ע��ʼ����
		{
			wsprintf(szFileName,"music\\sound\\BetStart.ogg");
			break;
		}
	case SOUND_COUNTDWON://��ע����ʱ
		{
			wsprintf(szFileName,"music\\sound\\CountDwon.ogg");
			break;
		}
	case SOUND_MOUSE_ENTER://����ƶ�
		{
			wsprintf(szFileName,"music\\sound\\MouseMove.ogg");
			break;
		}
	case SOUND_WIN://Ӯ����Ч
		{
			wsprintf(szFileName,"music\\sound\\win.ogg");
			break;
		}
	case SOUN_LOST://������Ч
		{
			wsprintf(szFileName,"music\\sound\\lost.ogg");
			break;
		}
	case SOUND_BET://��ע��Ч
		{
			wsprintf(szFileName,"music\\sound\\Note.ogg");
			break;
		}
	default:
		{
			break;
		}
	}
	m_SoundEngineGame->setSoundVolume(1.0);
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}
//---------------------------------------------------------------------------------------
//���ű�����Ч
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	if(!m_SoundEnginBg)
	{
		return ;
	}
	if(!m_bSoundBgPlay)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	wsprintf(szFileName,"music\\sound\\BeiJing.ogg");
	m_SoundEnginBg->setSoundVolume(0.5);
	m_SoundEnginBg->play2D(szFileName,bISLoop);
}
//-------------------------------------------------------------------------
//��ʾ��ʾ������ׯ��Ϣ
void	CNewGameClient::ShowPromptDlg(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_1000700,bFlag);
	SetTextinfo(CTN_1000000_CTN_1000700_TEXT_1000701,"��ǰ��ׯ���Ƿ�ѡ��������ׯ��",bFlag);
	SetTextinfo(CTN_1000000_CTN_1000700_TEXT_1000704,"��ʾ",bFlag);
	SetBtVisible(CTN_1000000_CTN_1000700_BTN_1000702,bFlag);
	SetBtVisible(CTN_1000000_CTN_1000700_BTN_1000703,bFlag);
}
//-------------------------------------------------------------------------
//��ʾ�Լ�����Ϣ
void	CNewGameClient::ShowMyInfo(__int64 i64MyWinMoney,bool bFalg)
{
	OutputDebugString("dwjlog::ShowMyInfo");
	UserInfoForGame UserInfo ; 
	if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
	{
		//�ǳ�
		SetTextinfo(CTN_1000000_CTN_1000100_TEXT_1002310,UserInfo.szNickName,bFalg);
		//�Լ��Ľ��
		SetTextinfo(CTN_1000000_CTN_1000100_TEXT_1002311,UserInfo.i64Money,bFalg);
		//ս��
		SetTextinfo(CTN_1000000_CTN_1000100_TEXT_1002312,i64MyWinMoney,bFalg);
	}
	else
	{
		OutputDebugString("dwjlog::ShowMyInfo_FALSE");
	}
}
//-------------------------------------------------------------------------
//��ʾׯ����Ϣ
void	CNewGameClient::ShowNtInfo(__int64 i64NtHaveWin, int iNtPlayCount,bool bFalg)
{
	UserInfoForGame UserInfo ; 
	if (m_pGameFrame->GetUserInfo(m_byNtStation , UserInfo) && m_byNtStation != 255)
	{
		//ׯ���ǳ�
		SetTextinfo(CTN_1000000_CTN_1000300_TEXT_1000311,UserInfo.szNickName,bFalg);
		//ׯ�ҵĽ��
		SetTextinfo(CTN_1000000_CTN_1000300_TEXT_1000312,UserInfo.i64Money,bFalg);
		//ׯ�ҵľ���
		SetTextinfo(CTN_1000000_CTN_1000300_TEXT_1000313,iNtPlayCount,bFalg);
		//ׯ�ҵĳɼ�
		SetTextinfo(CTN_1000000_CTN_1000300_TEXT_1000314,i64NtHaveWin,bFalg);
	}
	else
	{
		//ׯ���ǳ�
		SetTextinfo(CTN_1000000_CTN_1000300_TEXT_1000311,UserInfo.szNickName,false);
		//ׯ�ҵĽ��
		SetTextinfo(CTN_1000000_CTN_1000300_TEXT_1000312,UserInfo.i64Money,false);
		//ׯ�ҵľ���
		SetTextinfo(CTN_1000000_CTN_1000300_TEXT_1000313,(__int64)iNtPlayCount,false);
		//ׯ�ҵĳɼ�
		SetTextinfo(CTN_1000000_CTN_1000300_TEXT_1000314,i64NtHaveWin,false);
	}
}
//-------------------------------------------------------------------------
//��Ϸ��ʼ
void	CNewGameClient::HandleGameBegin()
{
	//��Ϸ״̬
	m_byGameStation = GS_BALLOT_STATUS;
	m_byNtSelArea	= RESULT_ERR;		//ׯ�Ҳ�ǩ���
	//����ʱ
	SetTimePlay(CTN_1000000_CTN_1000100_Time_1000199,true,m_byBallotTime);
	//�����ׯ�� ��ȥ��ʾ��ǩ
	if (m_iMyDeskStation == m_byNtStation)
	{
		//��ʾ����ׯ�Ҳ�ǩ��ʾ
		SetContainVisible(CTN_1000000_CTN_10000200,true);
		SetBtVisible(CTN_1000000_CTN_10000200_BTN_10000210,true);
	}
	else
	{
		//��ʾ��ǩ��һ֡
		SetAnimateShowFrame(CTN_1000000_ExAnimate_10000215,true,0);
		//�ӳ�һ�벥����ǩ����
		m_pGameFrame->SetTimer(TIME_GAME_BEGIN , 1000 ) ;
	}
}
//-------------------------------------------------------------------------
//�յ���ʼ��ע��Ϣ
void	CNewGameClient::HandleBeginBet(void * pBuffer)
{
	BeginBetMsg *pBeginBet = (BeginBetMsg *)pBuffer;
	if(NULL == pBeginBet)
	{
		return ;
	}
	//������ǩ����
	SetAnimatePlay(CTN_1000000_ExAnimate_10000215,false);
	//���صȴ�ׯ�Ҳ�ǩ��ʾ
	SetImageVisible(CTN_1000000_IMG_10000202,false);
	m_pGameFrame->KillTimer(TIME_GAME_BEGIN);
	//��������ׯ�Ҳ�ǩ��ʾ
	SetContainVisible(CTN_1000000_CTN_10000200,false);
	//��Ϸ״̬
	m_byGameStation = GS_BET_STATUS;
	m_byBetTime		= pBeginBet->byBetTime;			//��עʱ��
	m_byNoBetTime	= pBeginBet->byNoBetTime;		//�����벻����ע
	m_byNtStation	= pBeginBet->byNtStation;		//ׯ��λ��
	m_byCurrResult	= pBeginBet->byCurrResult;		//��ǰ�������

	//û��ׯ�� || �Լ���ׯ�� ��ע��ť������
	if (m_byNtStation == 255  || m_iMyDeskStation == m_byNtStation)
	{
		SetBetBtEnable(false);
	}
	else
	{
		SetBetBtEnable(true);
	}
	//�������½ǵ���ʱ
	SetTimePlay(CTN_1000000_CTN_1000100_Time_1000199,true,m_byBetTime);
	//���ſ�ʼ��ע��Ч	dwjtest
	
	//�ȴ������ע��ʾ
	if (m_iMyDeskStation == m_byNtStation)
	{
		SetImageVisible(CTN_1000000_IMG_10000211,true);
	}
	else
	{
		SetImageVisible(CTN_1000000_IMG_10000211,false);
	}
	//��ʾ��ע���
	ShowBetInfo(true);

	//����趨�˶����벻����ע ��ô��ȥ���������ʱ��
	if (m_byNoBetTime > 0 && (m_byBetTime-m_byNoBetTime) > 0)
	{
		m_pGameFrame->SetTimer(TIME_NO_BET , (m_byBetTime-m_byNoBetTime)*1000 ) ;
	}
	//��������ʱ 5���ʱ�򲥷���Ч����
	m_byRemaindTime	= m_byBetTime;			//ʣ��ʱ��
	m_pGameFrame->SetTimer(TIME_ONE_TIME , 1000 ) ;
}//-------------------------------------------------------------------------
//�յ�������ǩ��Ϣ
void	CNewGameClient::HandleOpenResult(void * pBuffer)
{
	OpenResultMsg *pOpenResult = (OpenResultMsg *)pBuffer;
	if(NULL == pOpenResult)
	{
		return ;
	}

	//��Ϸ״̬
	m_byGameStation = GS_OPEN_RESULT;
	m_byCurrResult = pOpenResult->byOpenResult;
	m_byLastResult = pOpenResult->byOpenResult;	//���ֿ������
	//����������
	m_pUI->GetICursor()->SetControlVisible(false);
	//�رճ���
	ShowSuperResult(false);
	//���صȴ������ע��ʾ
	SetImageVisible(CTN_1000000_IMG_10000211,false);
	//���ص���ʱ
	SetTimePlay(CTN_1000000_CTN_1000100_Time_1000199,false,0);
	//���ų�ǩ����
	SetAnimatePlay(CTN_1000000_ExAnimate_10000216,true);

}
//-------------------------------------------------------------------------
//��Ϸ����
void	CNewGameClient::HandleGameResult(void * pBuffer)
{
	GameEndMsg *pGameEnd = (GameEndMsg *)pBuffer;
	if(NULL == pGameEnd)
	{
		return ;
	}

	//��Ϸ״̬
	m_byGameStation = GS_SHOW_RESULT;
	m_byShowResultTime = pGameEnd->byShowResultTime;
	m_bIsReady		 = false;	//�Ƿ�׼���� ���ڹرտͻ���

	//·������
	memset(m_byLuziList,0,sizeof(m_byLuziList));
	for(int i=0; i<10; i++)
	{
		m_byLuziList[i] = pGameEnd->byLuzi[i];
	}
	//��ʾ�н�����
	SetResultPosition(true);
	//������ʾ·��
	ShowLuziList(true);
	//��ʾ�����
	ShowGameResult(pGameEnd->i64ChangeMoney[m_iMyDeskStation],pGameEnd->i64ChangeMoney[m_byNtStation],pGameEnd->i64ReturnMoney[m_iMyDeskStation],true);
	//��ʾ��ʼ��ť
	SetBtVisible(CTN_1000000_BTN_10001,true);
	//��ʾ����ʱ
	SetTimePlay(CTN_1000000_CTN_1000100_Time_1000199,true,m_byShowResultTime-1);
	//������Ч
	if (pGameEnd->i64ChangeMoney[m_iMyDeskStation] > 0)
	{
		//����Ӯ����Ч
		PlayGameSound(SOUND_WIN);
	}
	else if (pGameEnd->i64ChangeMoney[m_iMyDeskStation] < 0)
	{
		//����������Ч
		PlayGameSound(SOUN_LOST);
	}
}
//-------------------------------------------------------------------------
//������Ϣ
void	CNewGameClient::HandleGameFree(void * pBuffer)
{
	GameFreeMsg *pGameFree = (GameFreeMsg *)pBuffer;
	if(NULL == pGameFree)
	{
		return ;
	}
	//ɱ�����м�ʱ��
	KillAllGameTime();
	//������Ч
	PlayGameSound(SOUND_GAME_BEGIN);
	//��Ϸ��ʼ���ò�������
	ResetGameData();

	m_byFreeTime = pGameFree->byFreeTime;
	//ׯ��λ��
	m_byNtStation = pGameFree->byNtStation;
	//��ׯ�б�
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_byWaitNtList[i] = pGameFree->byWaitNtList[i];
	}
	//������Ϸ����
	ReSetUiDlag();
	//��ʾׯ����Ϣ
	ShowNtInfo(pGameFree->i64NtHaveWin,pGameFree->iNtPlayCount,true);
	//��ʾ�Լ�����Ϣ
	ShowMyInfo(pGameFree->i64MeHaveWin,true);
	//ˢ����ׯ�б�
	ShowWaitNtInfo(true);
	//��ʾ������������ׯ��ť
	if (IsInWaitNtList() || m_iMyDeskStation == m_byNtStation)
	{
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000401,false);
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000402,true);
	}
	else
	{
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000401,true);
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000402,false);
	}
	//������ׯ��ť����
	SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000402,true);
	//���ؽ����
	ShowGameResult(0,0,0,false);
	//��ǰ��ׯ 
	if (m_byNtStation == 255)
	{
		ShowPromptDlg(true);
	}
	else
	{
		ShowPromptDlg(false);
		//��ʾ���е���ʱ
		SetTimePlay(CTN_1000000_CTN_1000100_Time_1000199,true,m_byFreeTime);
	}
}
//-------------------------------------------------------------------------
//������ׯ�����Ϣ
void	CNewGameClient::HandleApplyResult(void * pBuffer)
{
	ApplyNtResultMsg *pApplyNtResult = (ApplyNtResultMsg *)pBuffer;
	if(NULL == pApplyNtResult)
	{
		return ;
	}
	//��ׯ�б�
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_byWaitNtList[i] = pApplyNtResult->byWaitNtList[i];
	}
	//�����������ׯ��λ�úͿͻ��˵�λ�ò�һ�� ˵����ׯ��
	if (pApplyNtResult->byNtStation != m_byNtStation)
	{
		m_byNtStation = pApplyNtResult->byNtStation;
		ShowNtInfo(0,0,true);
	}
	//ˢ����ׯ�б�
	ShowWaitNtInfo(true);
	//��ʾ������������ׯ��ť
	if (IsInWaitNtList() || m_iMyDeskStation == m_byNtStation)
	{
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000401,false);
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000402,true);
		SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000402,true);
	}
	else
	{
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000401,true);
		SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000402,true);
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000402,false);
	}
}
//-------------------------------------------------------------------------
//������ׯ�����Ϣ
void	CNewGameClient::HandleLeaveNtResult(void * pBuffer)
{
	ApplyNtResultMsg *pApplyNtResult = (ApplyNtResultMsg *)pBuffer;
	if(NULL == pApplyNtResult)
	{
		return ;
	}
	//��ׯ�б�
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_byWaitNtList[i] = pApplyNtResult->byWaitNtList[i];
	}
	//ׯ�Ҳ�����
	if (pApplyNtResult->byNtStation == 255)
	{
		m_byNtStation = pApplyNtResult->byNtStation;
		ShowNtInfo(0,0,false);
	}

	//ˢ����ׯ�б�
	ShowWaitNtInfo(true);
	//��ʾ������������ׯ��ť
	if (IsInWaitNtList() || m_iMyDeskStation == m_byNtStation)
	{
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000401,false);
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000402,true);
		if (m_iMyDeskStation == m_byNtStation)
		{
			//������Ϣ dwjtest
			SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000402,false);
		}
		else
		{
			SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000401,true);
		}
	}
	else
	{
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000401,true);
		//������ׯ��ť����
		SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000402,true);
		SetBtVisible(CTN_1000000_CTN_1000400_BTN_1000402,false);
	}
}
//-------------------------------------------------------------------------
//�����ע�����Ϣ
void	CNewGameClient::HandleUserBetResult(void * pBuffer)
{
	UserBetResultMsg *pUserBetResult = (UserBetResultMsg *)pBuffer;
	if(NULL == pUserBetResult)
	{
		return ;
	}
	//�������ÿ���������ע���
	memcpy(m_i64UserBetCount,pUserBetResult->i64UserBetCount,sizeof(m_i64UserBetCount));
	//�����������ע
	memcpy(m_i64AreaBetAllSum,pUserBetResult->i64AreaBetAllSum,sizeof(m_i64AreaBetAllSum));
	//����ע
	m_i64SumAllBet = pUserBetResult->i64SumAllBet;
	
	//��ʾ��ע����
	SetShCtrlInfo((CTN_1000000_CTN_1000200_ShCtrl_1000271+ (pUserBetResult->byBetArea - 1)),G_iChouMaValue[pUserBetResult->byChouMaType],true);
	//��ʾ�Լ�����ע���
	ShowBetInfo(true);
	//������ע��Ч
	PlayGameSound(SOUND_BET);
}
//-------------------------------------------------------------------------
//���ÿ������ͼƬλ��
void	CNewGameClient::SetResultPosition(bool bFlag)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(CTN_1000000_CTN_1000200_BTN_1000201+m_byCurrResult-1));
	//��ȡλ��
	int iPositionX = 0;
	int iPositionY = 0;
	if (NULL != pButton)
	{
		iPositionX = pButton->GetGameX();
		iPositionY = pButton->GetGameY();

		SetImagePosition(CTN_1000000_CTN_1000200_IMG_1000241,iPositionX,iPositionY,bFlag);
	}
}
//-------------------------------------------------------------------------
//������ע��ť����
void	CNewGameClient::SetBetBtEnable(bool bFalg)
{
	SetBtEnable(CTN_1000000_CTN_1000100_BTN_1000101,bFalg);
	SetBtEnable(CTN_1000000_CTN_1000100_BTN_1000102,bFalg);
	SetBtEnable(CTN_1000000_CTN_1000100_BTN_1000103,bFalg);
	SetBtEnable(CTN_1000000_CTN_1000100_BTN_1000104,bFalg);
	SetBtEnable(CTN_1000000_CTN_1000100_BTN_1000105,bFalg);
	SetBtEnable(CTN_1000000_CTN_1000100_BTN_1000106,bFalg);
}
//-------------------------------------------------------------------------
//������ʾ·��
void	CNewGameClient::ShowLuziList(bool bFlag)
{
	CString sFilePath;
	for (int i=0; i<10; i++)
	{
		if (0 != m_byLuziList[i])
		{
			sFilePath.Format(".\\resources\\image\\LuZi\\LuZi_%d.jpg",m_byLuziList[i]);
			SetImageLoadSrc(CTN_1000000_CTN_1000500_IMG_1000501+i,sFilePath,true);
		}
		else
		{
			SetImageVisible(CTN_1000000_CTN_1000500_IMG_1000501+i,false);
		}
	}
}
//-------------------------------------------------------------------------
//��ʾ��ׯ�б�
void	CNewGameClient::ShowWaitNtInfo(bool bFlag)
{
	//��������ׯ�б�Ŀռ�
	for(int i=0; i<4; i++)
	{
		SetTextinfo(CTN_1000000_CTN_1000400_TEXT_1000411+i,"",false);
		SetTextinfo(CTN_1000000_CTN_1000400_TEXT_1000421+i,"",false);
	}
	//��ׯ�б��ж��ٸ�
	int iWaitCount = 0;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (255 == m_byWaitNtList[i])
		{
			break;
		}
		iWaitCount++;
	}
	//����4�� ������ť����
	if (iWaitCount > 4)
	{
		//��� ��ǰ��ǩ+4������ ��ô�ʹӵ� iWaitCount-4��ʼ 
		if (m_iCurrWaitNtTag + 4 >= iWaitCount)
		{
			m_iCurrWaitNtTag = iWaitCount - 4;
			//˵�������� ��������
			SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000404,false);
		}
		else
		{
			SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000404,bFlag);
		}
	}
	else
	{
		SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000403,false);
		SetBtEnable(CTN_1000000_CTN_1000400_BTN_1000404,false);
		//��������ĸ� ��ô�ʹӵ�һ����ʼ��ʾ
		m_iCurrWaitNtTag = 0;
	}

	UserInfoForGame UserInfo ; 
	for(int i = m_iCurrWaitNtTag; i<iWaitCount; i++)
	{
		if (255 == m_byWaitNtList[i])
		{
			continue;
		}
		
		memset(&UserInfo , 0 , sizeof(UserInfo)) ;
		if (m_pGameFrame->GetUserInfo(m_byWaitNtList[i] , UserInfo))
		{
			//�ǳ�
			SetTextinfo(CTN_1000000_CTN_1000400_TEXT_1000411+i,UserInfo.szNickName,bFlag);
			//���
			SetTextinfo(CTN_1000000_CTN_1000400_TEXT_1000421+i,UserInfo.i64Money,bFlag);
		}
	}
	

	//�������ׯ�� ��Ҫ��ʾׯ��ʶ
	if (m_iMyDeskStation == m_byNtStation)
	{
		SetImageVisible(CTN_1000000_IMG_10000203,bFlag);
	}
	else
	{
		SetImageVisible(CTN_1000000_IMG_10000203,false);
	}
}
//-------------------------------------------------------------------------
///��ʾ�����
void	CNewGameClient::ShowGameResult(__int64 iMyPoint, __int64 iNtPoint, __int64 iMyReturn,bool bFlag) 
{
	SetContainVisible(CTN_1000000_CTN_1000600,bFlag);
	//��ʾ���ҵ÷�
	SetTextinfo(CTN_1000000_CTN_1000600_TEXT_1000602,iMyPoint,bFlag);
	
	//ׯ�ҵ÷�
	if (m_byNtStation != 255)
	{
		SetTextinfo(CTN_1000000_CTN_1000600_TEXT_1000603,iNtPoint,bFlag);
	}
	else
	{
		SetTextinfo(CTN_1000000_CTN_1000600_TEXT_1000603,(__int64)0,false);
	}
	
	//�������������
	SetTextinfo(CTN_1000000_CTN_1000600_TEXT_1000604,iMyReturn,bFlag);

	return  ; 
}
//-------------------------------------------------------------------------
//��ʾ��ע��Ϣ
void	CNewGameClient::ShowBetInfo(bool bFlag)
{
	//��ʾ����ע
	SetInumInfo(CTN_1000000_CTN_10000212_Num_1002406,m_i64SumAllBet,bFlag);
	//��ʾ�Լ�������ע
	m_i64MyBetCount = 0;
	for(int i=0; i<BET_ARES; i++)
	{
		m_i64MyBetCount += m_i64UserBetCount[m_iMyDeskStation][i];
	}
	SetInumInfo(CTN_1000000_CTN_1000100_Num_1000120,m_i64MyBetCount,bFlag);
	//�Լ��ڸ����������ע����Լ������������ע���
	for(int i=0; i<BET_ARES; i++)
	{
		if (m_i64AreaBetAllSum[i] > 0)
		{
			//����������ע���
			SetInumInfo(CTN_1000000_CTN_1000200_Num_1000221+i,m_i64AreaBetAllSum[i],bFlag);
		}
		else
		{
			//����������ע���
			SetInumInfo(CTN_1000000_CTN_1000200_Num_1000221+i,m_i64AreaBetAllSum[i],false);
		}
		if (m_i64UserBetCount[m_iMyDeskStation][i] > 0)
		{
			//�Լ��ڸ����������ע��� 
			SetInumInfo(CTN_1000000_CTN_1000200_Num_1000251+i,m_i64UserBetCount[m_iMyDeskStation][i],bFlag);
		}
		else
		{
			//�Լ��ڸ����������ע��� 
			SetInumInfo(CTN_1000000_CTN_1000200_Num_1000251+i,m_i64UserBetCount[m_iMyDeskStation][i],false);
		}
	}
}
//-------------------------------------------------------------------------











//--------------------------------------------------------------------------------------------------------
//���ö����ؼ����Ŷ���
void	CNewGameClient::SetAnimatePlay(const int AnimateID,bool bFlag,BYTE byPlayNum, bool bIsLoop)
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetPlayNum(byPlayNum);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}
	
}
//--------------------------------------------------------------------------------------------------------
//���ö�����ʾĳһ֡
void	CNewGameClient::SetAnimateShowFrame(const int AnimateID,bool bFlag,int iFrame)
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetPlayState(bFlag);
		if (-1 == iFrame)
		{
			pAnimate->SetShowFrame(pAnimate->GetFrameNum()-1);
		}
		else
		{
			pAnimate->SetShowFrame(iFrame);
		}
		pAnimate->SetControlVisible(bFlag);

	}
}
//--------------------------------------------------------------------------------------------------------
//���ð�ť�Ƿ����
void	CNewGameClient::SetBtEnable(const int iButtonID, bool bFlag)
{
	
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetEnable(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//���ð�ť�Ƿ�ɼ�
void	CNewGameClient::SetBtVisible(const int iButtonID, bool bFlag)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ʱ�����ʾ����ʱ
void	CNewGameClient::SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime)
{
	ITime *pTime = NULL;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(iTimeID));
	if (NULL != pTime)
	{
		pTime->SetShowMaxNum(byMaxTime);
		pTime->SetShowMinNum(byMinTime);
		pTime->SetPLay(bFlag);
		pTime->SetControlVisible(bFlag);
	}

}
//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ���λ��
void	CNewGameClient::SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetGameXY(iPositonX,iPositonY);
		pImage->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ��Ƿ�ɼ�
void	CNewGameClient::SetImageVisible(const int iImageID, bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ�������ʾָ�����ļ�
void	CNewGameClient::SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag)
{
	//virtual int LoadPic(wchar_t *szFileName)=0;
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->LoadPic(CA2W(sImagSrc));
		pImage->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//���������ؼ��ɼ�
void	CNewGameClient::SetContainVisible(const int iContainID,bool bFlag)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		pContain->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����
void	CNewGameClient::SetTextinfo(const int iTextID,__int64 iNum, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%I64d",iNum);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����
void	CNewGameClient::SetTextinfo(const int iTextID,TCHAR sChar[], bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%s",sChar);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�������ֿռ���ʾ����
void	CNewGameClient::SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//���ó���ؼ���ʾֵ
void	CNewGameClient::SetShCtrlInfo(const int iShCtrlID, int iNum, bool bFlag)
{
	IShCtrl *pShCtrl = NULL;
	pShCtrl = dynamic_cast<IShCtrl *>(m_pUI->GetIControlbyID(iShCtrlID));
	if (NULL != pShCtrl)
	{
		OutputDebugString("dwjlog::SetShCtrlInfo");
		pShCtrl->SetShNote(iNum);
		pShCtrl->SetControlVisible(bFlag);
	}
	
}
//--------------------------------------------------------------------------------------------------------
//���õ�ѡ��ؼ��Ƿ�ѡ��
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected, bool bFlag)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		pRadioButton->SetIsSelect(bSelected);
		pRadioButton->SetControlVisible(bFlag);
	}
}