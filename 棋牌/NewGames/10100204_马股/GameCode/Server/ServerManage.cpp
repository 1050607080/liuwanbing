#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"
//-------------------------------------------dump��־����
#include <math.h>
#include <windows.h> 
#include <DbgHelp.h>  
#include <stdlib.h>  
#pragma comment(lib, "dbghelp.lib")  
#pragma comment(lib, "dbghelp.lib")  

#ifndef _M_IX86  
#error "The following code only works for x86!"  
#endif  

inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName)  
{  
	if(pModuleName == 0)  
	{  
		return FALSE;  
	}  

	WCHAR szFileName[_MAX_FNAME] = L"";  
	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);  

	if(wcsicmp(szFileName, L"ntdll") == 0)  
		return TRUE;  

	return FALSE;  
}  

inline BOOL CALLBACK MiniDumpCallback(PVOID                            pParam,  
	const PMINIDUMP_CALLBACK_INPUT   pInput,  
	PMINIDUMP_CALLBACK_OUTPUT        pOutput)  
{  
	if(pInput == 0 || pOutput == 0)  
		return FALSE;  

	switch(pInput->CallbackType)  
	{  
	case ModuleCallback:  
		if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg)  
			if(!IsDataSectionNeeded(pInput->Module.FullPath))  
				pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);  
	case IncludeModuleCallback:  
	case IncludeThreadCallback:  
	case ThreadCallback:  
	case ThreadExCallback:  
		return TRUE;  
	default:;  
	}  

	return FALSE;  
}  

inline void CreateMiniDump(PEXCEPTION_POINTERS pep, LPCTSTR strFileName)  
{  
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE,  
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  

	if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))  
	{  
		MINIDUMP_EXCEPTION_INFORMATION mdei;  
		mdei.ThreadId           = GetCurrentThreadId();  
		mdei.ExceptionPointers  = pep;  
		mdei.ClientPointers     = NULL;  

		MINIDUMP_CALLBACK_INFORMATION mci;  
		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;  
		mci.CallbackParam       = 0;  

		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, (pep != 0) ? &mdei : 0, NULL, &mci);  

		CloseHandle(hFile);  
	}  
}  

LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)  
{  
	CString strPath;
	char szPath[MAX_PATH];
	memset(szPath, 0, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szPath);
	strPath.ReleaseBuffer();
	strPath.Format("%s\\D-Error\\", szPath);
	CreateDirectory(strPath, NULL);
	CString strFile;
	CTime tm = CTime::GetCurrentTime();
	strFile.Format(_T("%sError-game-%d-%d-%d&%d-%d-%d.dmp"), strPath,tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
	CreateMiniDump(pExceptionInfo, strFile);  
	AfxMessageBox(strPath);
	return EXCEPTION_EXECUTE_HANDLER;  
}  

// �˺���һ���ɹ����ã�֮��� SetUnhandledExceptionFilter �ĵ��ý���Ч  
void DisableSetUnhandledExceptionFilter()  
{  
	void* addr = (void*)GetProcAddress(LoadLibrary("kernel32.dll"),  
		"SetUnhandledExceptionFilter");  

	if (addr)  
	{  
		unsigned char code[16];  
		int size = 0;  

		code[size++] = 0x33;  
		code[size++] = 0xC0;  
		code[size++] = 0xC2;  
		code[size++] = 0x04;  
		code[size++] = 0x00;  

		DWORD dwOldFlag, dwTempFlag;  
		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);  
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);  
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);  
	}  
}  

void InitMinDump()  
{  
	//ע���쳣������  
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);  

	//ʹSetUnhandledExceptionFilter  
	DisableSetUnhandledExceptionFilter();  
} 

//-----------------------------------------------------------------------------------
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bGameStation = GS_WAIT_AGREE;
	m_iGamesCounter = 0;
}
//��������
CServerGameDesk::~CServerGameDesk(void)
{
}
void DebugPrintf(const char *p, ...)
{
	return;
	char szFilename[256];
	memset(szFilename,0,sizeof(szFilename));
	CString filePath;
	CString s = CINIFile::GetAppPath ();//����·��
	filePath.Format("%s%d_log.txt",s,NAME_ID);
	memcpy(szFilename,filePath,filePath.GetLength());
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start( arg, p );
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}
//д��־�ļ�
void  Testlog(TCHAR *pMessage) 
{ 
	try 
	{ 
		TCHAR FileName[50]; 
		wsprintf(FileName, TEXT("e:\\syslog%d.txt\0"), GetCurrentProcessId()); 
		CFile syslog; 
		syslog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite); 
		syslog.SeekToEnd(); 
		syslog.Write(pMessage, lstrlen(pMessage)); 
		syslog.SeekToEnd(); 
		TCHAR nextLine[] = TEXT("\r\n"); 
		syslog.Write(nextLine, lstrlen(nextLine)); 
		syslog.Close(); 
	} 
	catch(CException *e) 
	{  
		e->Delete(); 
		return ;  
	} 
} 
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	InitMinDump();
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	//��Ϸ������Ϣ
	CString key = TEXT("game");
	m_iUnit=f.GetKeyVal(key,"Unit",100);//�����һ��ɽ�ҵĵ�λ
	if(m_iUnit<=0)
	{
		m_iUnit=1;
	}
	m_iBeginTime = f.GetKeyVal(key,"BeginTime",30);
	if(m_iBeginTime<=0)
	{
		m_iBeginTime=30;
	}
	m_iThinkTime = f.GetKeyVal(key,"ThinkTime",30);
	if(m_iThinkTime<=0)
	{
		m_iThinkTime=30;
	}		


	//�����û� 
	CString mykey = TEXT("superid");
	int count = f.GetKeyVal(mykey,"count",0);
	TCHAR szName[21];
	if(count <= 0 )					//��ʤ��
	{
		return FALSE;
	}

	for(int i = 0; i < count; i++)
	{
		wsprintf(szName,"superid_%d",i);
		m_vSuperClient.push_back(f.GetKeyVal(mykey,szName,0));
	}

	return true;
}
//��ʼ����������Ϸ����
BOOL CServerGameDesk::InitData()
{
	//��ʼ������
	m_EGameStation = GS_NOMODE;				// ��ʼ��ʱ����Ϸ״̬Ϊ �����״̬
	m_GameMode=GAMEMODE_INVALIDE;			// ��ʼ��ʱ����ϷģʽΪ ��Чģʽ����δѡģʽ
	m_PlayersCountInGame = 0;
	m_iPKPlayerCount = 0;					//��������
	m_sendBetBankerPlayer = 0;				//������ׯ��Ϣ���������
	m_iBetPlayerCount = 0;
 	for (int i = 0;i<PLAY_COUNT;++i)
 		m_bIsPrepare[i] = false;			//��ʼ����ҵ�׼��״̬
	for(int i=1;i<4;i++)
	{
		m_iBasePoint[i-1]=i;
	}
	/*ReSetGameState(m_bGameStation);*/
	return TRUE;
}
//��ʼ��
bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadIni();
	//��ʼ������������
	InitData();
	m_bGameStation = GS_NOMODE;	//��ʼ����Ϸ״̬
	return true;
}

//�����Ϣ������
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{
			CString szAppPath = CINIFile::GetAppPath();
			CString strBCFFile  ; 
			CString strName;
			strName.Format("%d",NAME_ID);
			strBCFFile.Format("%s\\SpecialRule.bcf",szAppPath) ; 

			CBcfFile File(strBCFFile) ;

			bool  bForceQuitAsAuto = 0 ; 

			if (File.IsFileExist(strBCFFile))
			{
				bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strName,1) ; 
			}

			//if(true == bForceQuitAsAuto)
			//{
			//	//GameFinish(bDeskStation,GF_NORMAL)
			//	m_bAuto[bDeskStation] = true;
			//	return true;
			//}
			//else
			//{
			//	GameFinish(bDeskStation,GF_NORMAL);
			//}
		}
	}

	return  __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case GM_GAMESTATIONCHANGED:
		{
			OnGameStationChanged(pData);
			return true;
		}
		break;
	case ASS_WINORLOSE:	//��Ӯ
		{
			TSuper *ptsuper = (TSuper*)pData;
			if(ptsuper == NULL)
			{
				return false;
			}
			m_tSuper.IsSetSuper = ptsuper->IsSetSuper;
			m_tSuper.isWinOrLose = ptsuper->isWinOrLose;
			CString cs;
			cs.Format("hs isWinorLose:%d",m_tSuper.isWinOrLose);
			OutputDebugString(cs);

			return true;
		}
		break;
	case GM_GETREADY:
		{
			m_bIsPrepare[bDeskStation] = true;
			int iPrepareCount(0);
			for(int i = 0; i < PLAY_COUNT; i++)
			{
				if(m_bIsPrepare[i])
					iPrepareCount++;
			}
			//������ׯģʽ����֪ͨ��ׯ�� ����ģʽ��֪ͨ��ע
			if(iPrepareCount == GetPlayersCountInDesk() && GetPlayersCountInDesk() > 1)
				OnGMGetReady(bDeskStation,pData);	
			return true;
		}
		break;
	case GM_BETBANKER:
		{
			++m_sendBetBankerPlayer;
			TBetBanker *ptBetBanker = (TBetBanker*)pData;
			if(ptBetBanker == NULL)
				return false;
			if(ptBetBanker->isBet)
			{
				m_isBetBanker[ptBetBanker->byDeskStation] = true;
			}
			else
			{
				m_isBetBanker[ptBetBanker->byDeskStation] = false;
			}

			if(m_sendBetBankerPlayer < GetInGamePlayerCount())//��������ׯ���������������ʱ������ҷ�����ע��Ϣ
				return true;
			//if(m_sendBetBankerPlayer == GetPlayersCountInDesk())	
			else
			{
				BetBanker();

				while(!m_pUserInfo[m_byBankerDeskStation])
				{
					m_byBankerDeskStation++;
					m_byBankerDeskStation = m_byBankerDeskStation % PLAY_COUNT;
				}

				for (int i =0;i<PLAY_COUNT;++i)
				{
					if(m_pUserInfo[i] && m_byUserStation[i] == 1)
					{
						T_SC_WhoIsBanker tWhoIsBanker;
						tWhoIsBanker.byDeskStation = m_byBankerDeskStation;
						SendGameData(i,&tWhoIsBanker,sizeof(tWhoIsBanker),MDM_GM_GAME_NOTIFY,GM_BET,0);
					}
				/*	OutputDebugString("hs ��������ע��Ϣ");*/
				}
			}

			if(!m_pUserInfo[m_byBankerDeskStation])
			{
				CString cs;
				cs.Format("MG:----------m_sendBetBankerPlayer = %d", m_sendBetBankerPlayer);
				OutputDebugString(cs);
			}
			return true;
		}
		break;
	case GM_BET:
		{
			//�ı���Ϸ״̬

			if(!m_byUserStation[bDeskStation])
				return false;
			m_EGameStation = GS_BETTING;	//��ע״̬
			++m_iBetPlayerCount;			//������ע�������
			T_CS_Bet *ptBet = (T_CS_Bet*)pData;
			if(ptBet == NULL)
				return false;
			m_iBetIn[ptBet->byDeskStation] = ptBet->iBetMony;	//���������ע���
			//if(m_GameMode == GAMEMODE_BANKER_BET)
			//{
			//	if (m_iBetPlayerCount == (GetPlayersCountInDesk()-1))
			//	{
			//		for (int i =0;i<PLAY_COUNT;++i)
			//		{
			//			if (m_pUserInfo[i])
			//			{
			//				SendGameData(i,&m_byBankerDeskStation,sizeof(m_byBankerDeskStation),MDM_GM_GAME_NOTIFY,ASS_HANDSUP,0);
			//			}
			//		}
			//	}
			//}
			//else
			//{
			//SendGameData(ptBet->byDeskStation,&m_byBankerDeskStation,sizeof(m_byBankerDeskStation),MDM_GM_GAME_NOTIFY,ASS_HANDSUP,0);
			//}
			if(m_iBetPlayerCount == GetInGamePlayerCount() - 1)
			{
				memset(m_byUserCard,0,sizeof(m_byUserCard));	
				m_Logic.RandCard();		// ϴ��
				for (int i = 0 ;i<PLAY_COUNT;++i)	//��������ҷ���
					m_Logic.DistributeCards(i,m_byUserCard[i],5);
				
				for (int i = 0; i< PLAY_COUNT;++i)
				{
					SendGameData(i,MDM_GM_GAME_NOTIFY,GM_WASHCARD,0);
				}
				SendWatchData(PLAY_COUNT,MDM_GM_GAME_NOTIFY,GM_WASHCARD,0);
				//����ϴ�ƶ�ʱ������ʱ�䵽��ʱ����ͻ��˷���������
				SetTimer(S_TIMER_WASHCARD,3000);
			}
			return true;
		}
		break;
	case GM_PK:
		{

			if(!m_byUserStation[bDeskStation])
				return false;
			m_EGameStation = GS_PK;	//����״̬
			//-------------------------------------------------
			//-------------------------------------------------
			// ��������
			ReceiveClientCards(bDeskStation,pData);
			//��������ҷ����ѷ�����ҵ�������
			ShowCardsToAll(bDeskStation, pData);
			// ����
			if(m_iPKPlayerCount == GetInGamePlayerCount())
			{
				SetTimer(TIMER_SETTLE,3000);
			}
			return true;
		}
		break;
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

void CServerGameDesk::GameOverRest()
{
	m_iPKPlayerCount = 0;	//��������
	memset(m_bAuto,0,sizeof(m_bAuto));
	memset(m_iBetIn,0,sizeof(m_iBetIn));	//ÿ�������ע��
	memset(m_CardResult,0,sizeof(m_CardResult));	//��ҵ�����
}

// ��Ϸ״̬�仯
void CServerGameDesk::OnGameStationChanged(void *pData)
{
	//����״̬����֪ͨ�����û���Ϸ״̬�仯
	TGameStation *pGameStation = (TGameStation*)pData;
	m_EGameStation = pGameStation->enGameStation; // update station
	
	// inform all other client the station have changed
	switch (m_EGameStation)
	{
	case GS_MODESETTING:	//����������Ϸģʽ	
		{
			m_EGameStation = pGameStation->enGameStation;	//���ķ������Ϸ״̬
			break;
		}
	case GS_WITHMODE:	//ģʽ��ѡ��
		{
			m_IsModeSet = true;			//��Ϸģʽ��ѡ��
			m_EGameStation = pGameStation->enGameStation;	//���ķ������Ϸ״̬
			m_GameMode = pGameStation->enGameMode;			//�趨��Ϸģʽ
			
			if(m_GameMode == GAMEMODE_BANKER_CONST || m_GameMode == GAMEMODE_BANKER_INTURN)
				m_byBankerDeskStation = pGameStation->byGameDeskNO;	//����ׯ�ҵ���λ��
			
			for (int i = 0;i <PLAY_COUNT;++i)	//�㲥��Ϸ״̬�仯
			{	
				if(m_pUserInfo[i])
				{
					pGameStation->enGameStation = GS_WITHMODE;
					pGameStation->byGameDeskNO = i;
					SendGameData(i,pGameStation,sizeof(TGameStation),MDM_GM_GAME_NOTIFY,GS_WITHMODE,0);					
				}
			}
		}
		break;
	case GS_NOMODE:	//��Ϸģʽδ����
		{
			m_iGamesCounter = 0;
			int icounter = 0;
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				if (m_pUserInfo[i])
				{
					TGameStation tGameStation;
					tGameStation.enGameMode = GAMEMODE_INVALIDE;
					tGameStation.enGameStation = GS_NOMODE;
					SendGameData(BYTE(i),&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GS_NOMODE,0);
					++icounter;
					break;
				}
			}
			if(icounter == PLAY_COUNT)
			{
				m_GameMode = GAMEMODE_INVALIDE;
				m_EGameStation = GS_NOMODE;
			}
		}
		break;
	case GS_READY:			//��Ϸ��ʼ
		break;
	case GS_FINDBANKER:			//����
		break;
	case GS_GAMEBEGIN:			//��Ϸ���
		break;
	case GS_WASHCARDS:			//�ȴ��¾ֿ�ʼ
		break;
	default:
		break;
	}
}

//��ȡ��ǰ�����������
int CServerGameDesk::GetPlayersCountInDesk()
{
	int iCount = 0;
	for (int i = 0;i<PLAY_COUNT;++i)
		if (m_pUserInfo[i])
			++iCount;

	return iCount;
}

int CServerGameDesk::GetInGamePlayerCount()
{
	int iCount = 0;
	for (int i=0 ;i <PLAY_COUNT; ++i)
		if(m_bIsPrepare[i])
			++iCount;

	return iCount;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	/*if(m_EGameStation > GS_WITHMODE && m_EGameStation < GS_GAMEOVER)
	return true;*/
	if (__super::GameBegin(bBeginFlag) == false) 
	{
		GameFinish(0,GF_SALE);
		return false;
	}

	//�ı���Ϸ״̬
	//if(m_GameMode == GAMEMODE_BANKER_CONST && !m_pUserInfo[m_byBankerDeskStation])
	//{
	//	for(int i = 0; i < PLAY_COUNT; i++)
	//	{
	//		if(m_pUserInfo[i])
	//		{
	//			m_byBankerDeskStation = i;
	//			break;
	//		}
	//	}
	//}
	//else if(m_GameMode == GAMEMODE_BANKER_INTURN && !m_pUserInfo[++m_byBankerDeskStation])
	//{
	//	while(true)
	//	{
	//		if(m_pUserInfo[++m_byBankerDeskStation % PLAY_COUNT])
	//		{
	//			m_byBankerDeskStation = m_byBankerDeskStation % PLAY_COUNT;
	//			break;;
	//		}
	//	}
	//}

	for(int i= 0; i < PLAY_COUNT; i++)
	{
		m_byUserStation[i] = 0;
		m_bIsPrepare[i] = false;
		if(m_pUserInfo[i])
		{
			m_byUserStation[i] = 1;
			m_bIsPrepare[i] = true;
		}
	}

	m_bGameStation = GS_PLAY_GAME;
	m_EGameStation = GS_READY;	//��Ϸ��ʼ
	m_iPKPlayerCount = 0;

	for(int i = 0; i < PLAY_COUNT; i++)
		if(m_pUserInfo[i])
			SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,GM_BETBANKER,0);

	//����ׯ��
	//if(!m_bIsPrepare[m_byBankerDeskStation])
	//	m_bIsPrepare[m_byBankerDeskStation] = true;

	//switch (m_GameMode)
	//{
	//case GAMEMODE_BANKER_BET:	//��ׯ
	//case GAMEMODE_INVALIDE:
	//	{
	//	}
	//	break;
	//case GAMEMODE_BANKER_INTURN:	//��ׯ
	//	{
	//		/*		if(!m_pUserInfo[m_byBankerDeskStation])
	//		{
	//		while(true)
	//		{
	//		int banker = ((++m_byBankerDeskStation)%PLAY_COUNT);
	//		if(m_pUserInfo[m_byBankerDeskStation])
	//		{
	//		m_byBankerDeskStation = banker;
	//		m_iBetIn[m_byBankerDeskStation] = 0;
	//		break;
	//		}
	//		}
	//		}*/
	//		++m_iGamesCounter;	
	//	}
	//	break;
	//}
	//memset(m_byUserCard,0,sizeof(m_byUserCard));	
	//m_Logic.RandCard();		// ϴ��
	//for (int i = 0 ;i<PLAY_COUNT;++i)	//��������ҷ���
	//	m_Logic.DistributeCards(i,m_byUserCard[i],5);
	//
	//for (int i = 0; i< PLAY_COUNT;++i)
	//{
	//	SendGameData(i,MDM_GM_GAME_NOTIFY,GM_WASHCARD,0);
	//}
	//SendWatchData(PLAY_COUNT,MDM_GM_GAME_NOTIFY,GM_WASHCARD,0);
	////����ϴ�ƶ�ʱ������ʱ�䵽��ʱ����ͻ��˷���������
	//
	//SetTimer(S_TIMER_WASHCARD,3000);

	return true;
}
//��Ϸģʽѡ��
void CServerGameDesk::OnGameMode(void * pData,BYTE bDeskStation)
{
}

//��ȡ��Ϸ״̬��Ϣ	--����ҽ������Ӻ�ר���������ͻ��˷�����Ϸ״̬��Ϣ�ġ�
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	TGameStation tGameStation;	//��Ϸ״̬��
	tGameStation.byGameDeskNO = bDeskStation;	//��ֵ��λ��
	
	//��֤����
	for (vector<long>::iterator it= m_vSuperClient.begin(); it != m_vSuperClient.end();++it)
	{
		if(*it == (m_pUserInfo[bDeskStation]->m_UserData.dwUserID) )
		{
			SendGameData(bDeskStation,NULL,0,MDM_GM_GAME_NOTIFY,ASS_SUPER,0);
			break;
		}
	}
	switch (m_bGameStation)
	{
	case GS_NOMODE:	//ģʽδ����  
		{
			m_GameMode == GAMEMODE_BANKER_BET;
			m_EGameStation = GS_WITHMODE;
			tGameStation.enGameStation = m_EGameStation;
			tGameStation.enGameMode = m_GameMode;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&tGameStation,sizeof(tGameStation));	
			//m_bGameStation = GS_MODESETTING;
			m_byUserStation[bDeskStation] = 1;
			return true;
		}
		break;
	case GS_READY:       //ȫ��׼��
	case GS_BETTING:        //��ע
	case GS_GAMEBEGIN:		//��Ϸ��ʼ
	case GS_FINDBANKER:		//ȷ��ׯ��
	case GS_WASHCARDS:		//ϴ��
	case GS_SENDCARDS:		//����
	case GS_PK:				//����
	case GS_SETTLE:			//����
		{
			m_byUserStation[bDeskStation] = 0;
			tGameStation.enGameStation = m_EGameStation;
			tGameStation.enGameMode = m_GameMode;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&tGameStation,sizeof(tGameStation));	
			return true;
		}
	default:	//�������ʱ������Ϸ״̬���͸��ͻ��ˣ��ÿͻ�������Ӧ����
		{
			m_byUserStation[bDeskStation] = 1;
			tGameStation.enGameStation = m_EGameStation;
			tGameStation.enGameMode = m_GameMode;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&tGameStation,sizeof(tGameStation));	
			return true;
		}
		break;
	}
	return false;
}
/*
@brief:�ȴ�ͬ��״̬
*/
void CServerGameDesk::OnSendGameStationAgree(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
// 	T_SC_GameStationAgree gameStation;
// 	gameStation.iGameStation=m_bGameStation;
// 	gameStation.iGameMode=m_GameMode;
// 	SendGameStation(bDeskStation,uSocketID,bWatchUser,&gameStation,sizeof(gameStation));
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{									
	m_iPKPlayerCount = 0;	//��������
	m_iBetPlayerCount = 0;
	m_sendBetBankerPlayer = 0;
	memset(m_cardShape, 0, sizeof(m_cardShape));
	memset(m_bIsPrepare,0,sizeof(m_bIsPrepare));	//����Ƿ�׼��
	memset(m_bAuto,0,sizeof(m_bAuto));
	m_byBankerDeskStation = 255;
	memset(m_isBetBanker, 0, sizeof(m_isBetBanker));
	//if(!m_pUserInfo[m_byBankerDeskStation])
	//	m_GameMode == GAMEMODE_BANKER_INTURN;

	if(m_GameMode == GAMEMODE_BANKER_INTURN)
	{
		////
		while(1)
		{
			int bankerDeskStation = ((++m_byBankerDeskStation)%PLAY_COUNT);

			if(m_pUserInfo[bankerDeskStation])
			{
				m_byBankerDeskStation = bankerDeskStation;
				break;
			}
		}
	}
	return TRUE;
}

//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case S_TIMER_WASHCARD:
		{
			KillTimer(S_TIMER_WASHCARD);

			memset(m_byPlayerCards,0,sizeof(BYTE)*PLAY_COUNT*5);
			memset(m_cardShape,0,sizeof(EnCardShape)*PLAY_COUNT);

			
			for(int i = 0;i< PLAY_COUNT;++i)
			{
				memcpy(m_byPlayerCards[i],m_byUserCard[i],sizeof(BYTE)*5);
				m_cardShape[i] = GetCardShape(m_byPlayerCards[i],sizeof(BYTE)*5);
			}

			
			int iBanker;
			memset(m_iArrMaxOrder,0,sizeof(int)*PLAY_COUNT);
			vector<int> vCounter;
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				int iCounter = 0;
				for (int j = 0;j< PLAY_COUNT;++j)
				{
					if(m_cardShape[i] >= m_cardShape[j])
						iCounter++;
				}
 
 				if(iCounter ==4)
 				{
					m_iArrMaxOrder[0] = i;
 				}
 				else if( iCounter <= 2)
 				{
					m_iArrMaxOrder[3] = i;
 				}

				for(int i = 0;i<PLAY_COUNT;++i)
				{
					if( i != m_iArrMaxOrder[0] && i != m_iArrMaxOrder[3])
						m_iArrMaxOrder[1] = i;
				}
				for(int i = 0;i<PLAY_COUNT;++i)
				{
					if( i != m_iArrMaxOrder[0] && i != m_iArrMaxOrder[3] && i != m_iArrMaxOrder[1])
						m_iArrMaxOrder[2] = i;
				}

				if(i == m_byBankerDeskStation)
					iBanker = i;
			}

			for(int i = 0;i<PLAY_COUNT;++i)
			{
				CString cs;
				cs.Format("hs m_iArrMaxOrder:%d - %d",i,m_iArrMaxOrder[i]);
				OutputDebugString(cs);
			}

			if(m_tSuper.IsSetSuper == false)
			{
				for (int i = 0;i<PLAY_COUNT;++i)
					if(m_pUserInfo[i] && m_byUserStation[i] == 1)
						SendCard(i);	//����ҷ���

				break;
			}
			else if(m_tSuper.IsSetSuper && (m_tSuper.isWinOrLose == TRUE))
			{
				OutputDebugString("hs ׯ��Ӯ");
				int icounter = 1;
				for (int i = 0;i<PLAY_COUNT;++i)
				{
					T_SC_Cards tCards;
					
					if(i != m_byBankerDeskStation)
					{
						memcpy(tCards.m_cards,m_byPlayerCards[m_iArrMaxOrder[icounter++]],sizeof(BYTE)*5);
						SendGameData(i,&tCards,sizeof(T_SC_Cards),MDM_GM_GAME_NOTIFY,GM_SENDCARD,0);
					}
				}
				T_SC_Cards tCards;
				memcpy(tCards.m_cards,m_byPlayerCards[m_iArrMaxOrder[0]],sizeof(BYTE)*5);
				SendGameData(m_byBankerDeskStation,&tCards,sizeof(T_SC_Cards),MDM_GM_GAME_NOTIFY,GM_SENDCARD,0);
			}
			else if(m_tSuper.IsSetSuper && (m_tSuper.isWinOrLose == FALSE))
			{
				OutputDebugString("hs ׯ����");
				int icounter = 0;
				for (int i = 0;i<PLAY_COUNT;++i)
				{
					T_SC_Cards tCards;

					if(i != m_byBankerDeskStation)
					{
						memcpy(tCards.m_cards,m_byPlayerCards[m_iArrMaxOrder[icounter++]],sizeof(BYTE)*5);
						SendGameData(i,&tCards,sizeof(T_SC_Cards),MDM_GM_GAME_NOTIFY,GM_SENDCARD,0);
					}
				}
				T_SC_Cards tCards;
				memcpy(tCards.m_cards,m_byPlayerCards[m_iArrMaxOrder[3]],sizeof(BYTE)*5);
				SendGameData(m_byBankerDeskStation,&tCards,sizeof(T_SC_Cards),MDM_GM_GAME_NOTIFY,GM_SENDCARD,0);
			}
		}
		break;
	case TIMER_SETTLE:
		{
			KillTimer(TIMER_SETTLE);
			GameFinish(0,GF_NORMAL);
		}
		break;
	}
	return __super::OnTimer(uTimerID);
}
//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{

			m_sendBetBankerPlayer = 0;

			//CString cs;
			//cs.Format("MG:m_pUserInfo[%d] = %d",m_byBankerDeskStation,m_pUserInfo[m_byBankerDeskStation]);
			//OutputDebugString(cs);

			Settle();
			//��Ϸ����״̬
			ReSetGameState(bCloseFlag);

			m_bGameStation = GS_WAIT_NEXT;
			m_EGameStation = GS_GAMEOVER; 

 			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
		{
			/*m_bGameStation=GS_WAIT_NEXT;*/
			ReSetGameState(bCloseFlag);
			m_bGameStation = GS_WAIT_NEXT;
			m_EGameStation = GS_GAMEOVER; 
			bCloseFlag=GF_SALE;
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_SAFE_FINISH:
		{
			/*m_bGameStation=GS_WAIT_NEXT;*/
			bCloseFlag=GFF_SAFE_FINISH;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_FORCE_FINISH:		//�û�ǿ���뿪
		{
			bCloseFlag=GFF_FORCE_FINISH;

			m_EGameStation = GS_WITHMODE;
			m_bGameStation = GS_GAMEOVER;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,GFF_FORCE_FINISH);
			
			return true;
		}
	}
	
	return true;
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	/*if(!m_byUserStation[bDeskStation])
		return false;

	if(m_EGameStation > GS_WITHMODE && m_EGameStation < GS_GAMEOVER)
		return true;*/

	return false;
}
//û�е���GameBegin()��������������˳����˺���������
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	m_iBetIn[bDeskStation] = 0;
	m_bIsPrepare[bDeskStation]=false;
	//--m_iBetPlayerCount;	//Ͷע�������

	//GameFinish(bDeskStation,GF_SALE);
	if( GetPlayersCountInDesk()== 1)	//���һ�����뿪����
	{
		m_EGameStation = GS_NOMODE;
		m_GameMode = GAMEMODE_BANKER_BET;
		return __super::UserLeftDesk(bDeskStation,pUserInfo);
	}

	//if(m_byUserStation[bDeskStation] != 1)
	//{
	//	return __super::UserLeftDesk(bDeskStation,pUserInfo);
	//}

	//TGameStation gameStation;
	//memset(&gameStation, 0, sizeof(TGameStation));

	//if(bDeskStation == m_byBankerDeskStation && m_GameMode == GAMEMODE_BANKER_CONST)	//�����ׯ���˳�,ѡһ����ҵ�ׯ��
	//{
	//	m_GameMode = GAMEMODE_BANKER_INTURN;
	//	ReSetGameState(GF_SALE);
	//	gameStation.byGameDeskNO = m_byBankerDeskStation;
	//	gameStation.enGameMode = m_GameMode;
	//	gameStation.enGameStation = m_EGameStation;
	//	m_iBetIn[m_byBankerDeskStation] = 0;
	//	
	//	for(int i = 0; i < PLAY_COUNT; i++)
	//	{
	//		if(m_pUserInfo[i])
	//		{
	//			m_iBetIn[i] = 0;
	//			m_bIsPrepare[i] = false;
	//			SendGameData(i,&gameStation,sizeof(TGameStation),MDM_GM_GAME_NOTIFY,GM_RESET_STATION,0);
	//		}
	//	}
	//}
	
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}
//��ʼ��Ϸ����������˳����˺�������
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	if(m_byUserStation[bDeskStation])
	{
		m_bAuto[bDeskStation] = true;
		AutoPlay(bDeskStation);
	}
	return __super::UserNetCut(bDeskStation,pLostUserInfo);
	//if(m_byUserStation[bDeskStation] == 0)
	//{
	//	return false;
	//}
	//else//(m_byUserCard[bDeskStation][0] > 0 || bDeskStation == m_byBankerDeskStation)
	//{
	//	m_bAuto[bDeskStation] = true;
	//	AutoPlay(bDeskStation);
	//}
	//return __super::UserNetCut(bDeskStation, pLostUserInfo);
}

//�û���������
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{
	//����Ϸû�ж�������
	return __super:: UserReCome(bDeskStation, pNewUserInfo);
}

//����
void CServerGameDesk::SendCard(int bDeskStation)
{
	T_SC_Cards tCards;
	if(bDeskStation >PLAY_COUNT)	//�����λ���Ƿ���Ч
		return;
	
	if(!m_tSuper.IsSetSuper)
	{
		memcpy(tCards.m_cards,m_byUserCard[bDeskStation],5);
		SendGameData(bDeskStation,&tCards,sizeof(tCards),MDM_GM_GAME_NOTIFY,GM_SENDCARD,0);	//����ҷ���
		return ;
	}

	if(m_bAuto[bDeskStation])
	{
		AutoPlay(bDeskStation);
	}
}

//������ҷ��ɵ�����
void CServerGameDesk::ReceiveClientCards(BYTE byDeskStation,void *buffer)
{
	T_CS_OutCard *ptOutCard = (T_CS_OutCard*)buffer;
	if(ptOutCard == NULL)
		return;
	//if(m_byBankerDeskStation != m_byWinSuperControl)
	++m_iPKPlayerCount;	//�����Ѿ�������ҵ������� �������뵱ǰ����������ʱ�����������ͱ��ƽ�����ͻ���

	if(ptOutCard->cardCount != 5)
	{
		return;
	}
	
	//��������
	analyseCardShape(byDeskStation,ptOutCard);
	for (int i=0;i<PLAY_COUNT;++i)
	{
		if(m_pUserInfo[i])
		{
			T_SC_CardResult tCardResult;
			tCardResult.enCardShape = m_CardResult[byDeskStation];
			tCardResult.byDeskStation = byDeskStation;
			SendGameData(i,&tCardResult,sizeof(tCardResult),MDM_GM_GAME_NOTIFY,GM_RESULT,0);
		}
	}
}

//��������
void CServerGameDesk::analyseCardShape(BYTE byDeskStation,T_CS_OutCard* ptCoutCards)
{
	/*OutputDebugString("hs isFourSame");*/
	//����
	if(m_Logic.isFourSame(ptCoutCards))
	{
		m_CardResult[byDeskStation] = CS_BOOM;
		return;
	}
	/*OutputDebugString("hs isAllSumTen");*/
	//ʮ��
	if (m_Logic.isAllSumTen(ptCoutCards))
	{
		m_CardResult[byDeskStation] = CS_TENHORSE;
		return;
	}
	/*OutputDebugString("hs IsBigHourse");*/
	//��˳��
	if(m_Logic.IsBigHourse(ptCoutCards))
	{
		m_CardResult[byDeskStation] = CS_BIGHORSE;
		return;
	}
	/*OutputDebugString("hs IsSmallHourse");*/
	//С˳��
	if (m_Logic.IsSmallHourse(ptCoutCards))
	{
		m_CardResult[byDeskStation] = CS_SMALLHORSE;
		return;
	}
	/*OutputDebugString("hs IsDoubleTenDoubleFive");*/
	//ͭ��
	if(m_Logic.IsDoubleTenDoubleFive(ptCoutCards))
	{
		m_CardResult[byDeskStation] = CS_DTENANDDF;
		return;
	}
	OutputDebugString("hs IsDoubleTen");
	//���
	if(m_Logic.IsDoubleTen(ptCoutCards))
	{
		m_CardResult[byDeskStation] = CS_DOUBLETEN;
		return;
	}
	//�д�
	int iret = m_Logic.IsOneTen(ptCoutCards);
	if (iret != -1)
	{
		m_CardResult[byDeskStation] = (EnCardShape)(CS_ONETEN + iret);
		return;
	}
	
	//�޴�
	if(m_Logic.NoTen(ptCoutCards))
	{
		m_CardResult[byDeskStation] = CS_NOTEN;
		return;
	}
}

void CServerGameDesk::Settle()
{
	__int64 i64Money[PLAY_COUNT];
	memset(i64Money,0,sizeof(i64Money));
	//���ׯ�ҵ�����
	EnCardShape  bankerShape = m_CardResult[m_byBankerDeskStation];
	T_SC_Settle tSettle; //�ǳơ��Ƿ�ׯ�ҡ����͡��÷�
	
	tSettle.bIsBanker[m_byBankerDeskStation] = true;			//�Ƿ�Ϊׯ��
	tSettle.enCardShape[m_byBankerDeskStation] = bankerShape;	//����
	//memcpy(tSettle.cbNickName[m_byBankerDeskStation],m_pUserInfo[m_byBankerDeskStation]->m_UserData.nickName,61);	//�ǳ�
	strcpy(tSettle.cbNickName[m_byBankerDeskStation],m_pUserInfo[m_byBankerDeskStation]->m_UserData.nickName);
	
	//���мҽ��бȽ� ,  �÷� = Ͷע * ����
	for(int i = 0; i<PLAY_COUNT; ++i)
	{
		m_EGameStation = GS_SETTLE;
		m_bGameStation = GS_SETTLE;
		if(i == m_byBankerDeskStation)
			continue;

		if(!m_pUserInfo[i])		//��λ�����
			continue;
		int iOdds = 0;	//����
		tSettle.bIsBanker[i] = false;
		tSettle.enCardShape[i] = m_CardResult[i];
		memcpy(tSettle.cbNickName[i],m_pUserInfo[i]->m_UserData.nickName,61);

		if(bankerShape >= m_CardResult[i]/* || (bankerShape == CS_NOTEN && m_CardResult[i] == CS_NOTEN)*/)
		{
			i64Money[m_byBankerDeskStation] += m_iBetIn[i];	//ׯ�ҵ÷�
			i64Money[i] -= m_iBetIn[i];
		}
		else
		{
			switch(m_CardResult[i])
			{
			case CS_OT_ONE:	//�д�1��
			case CS_OT_TWO:	//�д�2��
			case CS_OT_THREE:	//�д�3��
			case CS_OT_FOUR:	//�д�4��
			case CS_OT_FIVE:	//�д�5��
			case CS_OT_SIX:	//�д�6��
			case CS_OT_SEVEN:	//�д�7��
			case CS_OT_EIGHT:	//�д�8��
			case CS_OT_NIGHT:	//�д�9��
				iOdds = 1;
				break;
			case CS_DOUBLETEN:	//���
				iOdds = 2;
				break;
			case CS_DTENANDDF:		//ͭ��
				iOdds = 4;
				break;
			case CS_SMALLHORSE:		//С˳��
				iOdds = 5;
				break;
			case CS_BIGHORSE:		//��˳��
				iOdds = 6;
				break;
			case CS_TENHORSE:			//ʮ��
				iOdds = 8;
				break;
			case CS_BOOM:			//����
				iOdds = 10;
				break;
			}
			i64Money[m_byBankerDeskStation] -= m_iBetIn[i]*iOdds;	//ׯ�����
			i64Money[i] = m_iBetIn[i] * iOdds;	//�мҵ÷�
		}
		tSettle.iScore[i] = i64Money[i];
	}
	
	
	bool temp_cut[PLAY_COUNT];
	memset(temp_cut,0,sizeof(temp_cut));
	ChangeUserPointint64(i64Money, temp_cut);
	__super::RecoderGameInfo(tSettle.iScore);

	for (int i =0 ;i <PLAY_COUNT;++i)
	{
		if(m_pUserInfo[i])
			SendGameData(i,&tSettle,sizeof(tSettle),MDM_GM_GAME_NOTIFY,GM_SETTLE,0);
	}

	//for(int i = 0; i < PLAY_COUNT; i++)
	//{
	//	if(m_bAuto[i])
	//		UserLeftDesk(i,m_pUserInfo[i]);
	//}
}

// �������������
void CServerGameDesk::ShowCardsToAll(BYTE byDeskStation,void * pData)
{
	T_CS_OutCard *pclientCard = (T_CS_OutCard*)pData;	//�ͻ��˷��͹�����������
	T_SC_OutCard tOutCards;	//������Ҫ�㲥���ͻ��˵����ݰ�
	tOutCards.byDeskStation = byDeskStation;
	tOutCards.enCardShape = m_CardResult[byDeskStation];

	memcpy(&tOutCards.firstHeap,pclientCard->firstHeap,sizeof(BYTE)*3);
	memcpy(&tOutCards.secondHeap,pclientCard->SecondHeap,sizeof(BYTE)*2);
	for (int i= 0; i<PLAY_COUNT; ++i)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&tOutCards,sizeof(tOutCards),MDM_GM_GAME_NOTIFY,GM_SHOWTOALL,0);
		}
	}
}

//�й�
void CServerGameDesk::AutoPlay(BYTE byDeskStation)
{
	//BYTE byUserCards[5] = {0};

	T_CS_OutCard tCards;
	memcpy(tCards.allCards,m_byUserCard[byDeskStation],sizeof(BYTE)*5);	//�����й���ҵ���
	tCards.byDeskStation = byDeskStation;
	tCards.cardCount = 5;
	memcpy(tCards.firstHeap,m_byUserCard[byDeskStation],sizeof(BYTE)*3);
	memcpy(tCards.SecondHeap,m_byUserCard[byDeskStation]+3,sizeof(BYTE)*2);
	
	//AutoChoiseOutCard(byDeskStation,tCards);
	ReceiveClientCards(byDeskStation,&tCards);
	ShowCardsToAll(byDeskStation,&tCards);

	//switch(enCardShape)
	//{
	//default:
	//	break;
	//}
}

EnCardShape CServerGameDesk::GetCardShape(BYTE *pbyCards, int iCounter)
{
	vector<int> vCardNums;
	for (int i = 0;i<iCounter;++i)
	{
		int inum = m_Logic.GetCardNum(pbyCards[i]);
		vCardNums.push_back((inum == 14 ? 1 :inum));
	}
	sort(vCardNums.begin(),vCardNums.end());

	#pragma region ����
	int iCou;
	for (int i = 0;i<vCardNums.size()-1;++i)
	{
		if(vCardNums[i] == vCardNums[i+1])
		{
			iCou++;
		}
	}
	if(iCou == 4)
		return CS_BOOM;
	#pragma endregion ����

	#pragma region ʮ��
	int allsum=0;
	for (int i = 0;i<vCardNums.size();++i)
	{
		allsum += vCardNums[i];
	}
	if(allsum == 10)
		return CS_TENHORSE;
	#pragma endregion ʮ��

#pragma region ��˳��
	if(vCardNums[0] == 4 && vCardNums[1] == 5 &&
		vCardNums[2] == 6 && vCardNums[3] == 7 && vCardNums[4] == 8)
		return CS_BIGHORSE;
#pragma endregion ��˳��

#pragma region С˳��
	if(vCardNums[0] == 2 && vCardNums[1] == 3 &&
		vCardNums[2] == 4 && vCardNums[3] == 5 && vCardNums[4] == 6)
		return CS_SMALLHORSE;
#pragma endregion С˳��

#pragma region ͭ��
	
	vector<int> vfive;
	vector<int> vNoFive;
	vfive.clear();
	vNoFive.clear();
	for (int i=0 ; i<vCardNums.size();++i)
	{
		if(vCardNums[i] == 5)
		{
			vfive.push_back(vCardNums[i]);
		}
		else
		{
			vNoFive.push_back(vCardNums[i]);
		}
	}
	if(vfive.size()==2)
	{
		int isumNoFive = 0;
		for (int i = 0; i< vNoFive.size();++i)
		{
			isumNoFive += vNoFive[i];
		}
		if((isumNoFive % 10) == 0)
			return CS_DTENANDDF;
	}
	else if(vfive.size() == 3)
	{
		int isumt=0;
		if( (vNoFive[0] + vNoFive[1] +5) %10 == 0 )
			return CS_DTENANDDF;
	}

#pragma endregion ͭ��

#pragma region ���
	if(allsum % 10 == 0)
	{
		for (int i = 0;i<vCardNums.size();++i)
		{
			for (int j =0;j<vCardNums.size();++j)
			{
				for(int k=0;k<vCardNums.size();++k)
				{
					if( k!= j && k != i && i!=j )
					{
						if( (vCardNums[i] + vCardNums[j] + vCardNums[k])%10 ==0 )
						{
							return CS_DOUBLETEN;
						}
					}
				}
			}
		}
	}
#pragma endregion ���
	
#pragma region �д�
	int ii,ij,ik;
	bool isSuccess = false;
	for (int i = 0;i<vCardNums.size();++i)
	{
		for (int j =0;j<vCardNums.size();++j)
		{
			for(int k=0;k<vCardNums.size();++k)
			{
				if( k!= j && k != i && i!=j )
				{
					if( (vCardNums[i] + vCardNums[j] + vCardNums[k])%10 ==0 )
					{
						ii = i;ij =j; ik = k;
						isSuccess = true;
					}
				}
			}
		}
	}
	if(isSuccess)
	{
		for (int i =0;i<vCardNums.size();++i)
		{
			for (int j=0;j<vCardNums.size();++j)
			{
				if( i != ii && i != ij && i != ik && j != ii && j != ij && j != ik )
				{
					int iret = (vCardNums[i] + vCardNums[j]) %10;

					if(iret != 0)
					{
						return (EnCardShape)(CS_ONETEN + iret);
					}
				}
			}
		}
	}
#pragma endregion �д�

#pragma region �޴�
	return CS_NOTEN;
#pragma endregion �޴�

}

EnCardShape CServerGameDesk::AutoPlayGetCardShape(BYTE *pCard,int iLength,T_CS_OutCard* outCards)
{
	BYTE NoColorCard[5]= {0};
	vector<BYTE> vOrderCard;
	vOrderCard.clear();
	memcpy(NoColorCard,pCard,5);
	
	BYTE CardColor[5] = {0};	//5���ƵĻ�ɫ

	//ȥ����ɫ
	for(int i = 0;i<5;++i)
	{
		CardColor[i] = UG_HUA_MASK & NoColorCard[i];

		NoColorCard[i] << 4 ;
		NoColorCard[i] >> 4 ;	
		vOrderCard.push_back(NoColorCard[i]);
	}
	sort(vOrderCard.begin(),vOrderCard.end());
	int iCounter = 0;
	for(int i =0; i<4;++i)
	{
		if(vOrderCard[i] == vOrderCard[i+1])
			++iCounter;
	}
	if(iCounter >= 3)	//����
	{
		memcpy(outCards->firstHeap,pCard,sizeof(BYTE)*3);
		memcpy(outCards->SecondHeap,&pCard[3],sizeof(BYTE)*2);
		memcpy(outCards->allCards,pCard,sizeof(BYTE)*5);
		return CS_BOOM;
	}
	int iSum =0;
	for(int i=0;i<5;++i)
		iSum += NoColorCard[i];
	if(iSum == 10)	//ʮ��
	{
		memcpy(outCards->firstHeap,pCard,sizeof(BYTE)*3);
		memcpy(outCards->SecondHeap,&pCard[3],sizeof(BYTE)*2);
		memcpy(outCards->allCards,pCard,sizeof(BYTE)*5);
		return CS_TENHORSE;
	}

	//��˳��
	if( vOrderCard[0] == 4 && vOrderCard[1] == 5 && vOrderCard[2] == 6 && vOrderCard[3] == 7 && vOrderCard[4] == 8 )
	{
		for(int i = 0;i<5;++i)
		{
			if(NoColorCard[i] == 4)
				outCards->SecondHeap[0] = (CardColor[i] | 0x04);
			else if(NoColorCard[i] == 6)
				outCards->SecondHeap[0] = (CardColor[i] | 0x06);
			else if(NoColorCard[i] == 5)
				outCards->firstHeap[0] = (CardColor[i] | 0x05);
			else if(NoColorCard[i] == 7)
				outCards->firstHeap[1] = (CardColor[i] | 0x07);
			else if(NoColorCard[i] == 8)
				outCards->firstHeap[2] = (CardColor[i] | 0x08);
		}
		memcpy(outCards->allCards,pCard,sizeof(BYTE)*5);
		return CS_BIGHORSE;
	}

	//С˳��
	if( vOrderCard[0] == 2 && vOrderCard[1] == 3 && vOrderCard[2] == 4 && vOrderCard[3] == 5 && vOrderCard[4] == 6 )
	{
		for(int i = 0;i<5;++i)
		{
			if(NoColorCard[i] == 4)
				outCards->SecondHeap[0] = (CardColor[i] | 0x04);
			else if(NoColorCard[i] == 6)
				outCards->SecondHeap[0] = (CardColor[i] | 0x06);
			else if(NoColorCard[i] == 2)
				outCards->firstHeap[0] = (CardColor[i] | 0x02);
			else if(NoColorCard[i] == 3)
				outCards->firstHeap[1] = (CardColor[i] | 0x03);
			else if(NoColorCard[i] == 5)
				outCards->firstHeap[2] = (CardColor[i] | 0x05);
		}
		memcpy(outCards->allCards,pCard,sizeof(BYTE)*5);
		return CS_SMALLHORSE;
	}

	//ͭ��
	int iCounter2 =0;
	int iSec = 0;
	for(int i =0 ;i <5;++i)
	{
		if(NoColorCard[i] == 5)
			++iCounter2;
		if(iCounter2 == 2)
			break;
	}
	
	if(iCounter2 == 2 && iSum %10 == 0)
	{
		for (int i = 0;i<5;++i)
		{
			if(NoColorCard[i] == 5)
			{	
				outCards->SecondHeap[(iSec %2)] = (CardColor[i] | 0x05);
			}
			else
			{
				outCards->firstHeap[i%3] = (CardColor[i] | NoColorCard[i]);
			}
		}
		memcpy(outCards->allCards,pCard,sizeof(BYTE)*5);
		return CS_DTENANDDF;
	}

	//���
	bool bflag =false;
	BYTE s1 =0;
	BYTE s2 = 0;
	if(iSum %10 == 0)
	{
		for(int i = 0;i<=5;++i)
		{
			for(int j =0;j<5;++j)
			{
				if(i != j && ((NoColorCard[i] + NoColorCard[j]) == 10))
				{
					s1 =i;
					s2 = j;
					outCards->SecondHeap[0] = (NoColorCard[i] | CardColor[i]);
					outCards->SecondHeap[1] = (s1 = NoColorCard[i] | CardColor[j]);
					bflag = true;
				}
			}
			if(bflag)
				break;
		}
		int lp= 0;
		for (int i = 0;i<5;++i)
		{
			if( i != s1 && i != s2)
				outCards->firstHeap[lp++] = (NoColorCard[i] | CardColor[i]);
		}
		memcpy(outCards->allCards,pCard,sizeof(BYTE)*5);
		return CS_DOUBLETEN;
	}
	//�д�
	return CS_NOTEN;
}

void CServerGameDesk::OnGMGetReady(BYTE byDeskStation,void *pData)
{
	m_EGameStation = GS_READY;
	//if(m_GameMode == GAMEMODE_BANKER_CONST && !m_pUserInfo[m_byBankerDeskStation])
	//{
	//	for(int i = 0; i < PLAY_COUNT; i++)
	//		if(m_pUserInfo[i])
	//		{
	//			m_byBankerDeskStation = i;
	//			break;
	//		}
	//}
	//m_bIsPrepare[byDeskStation] = true;
	//bool tmp = false;
	//for(int i = 0; i < PLAY_COUNT; i++)
	//{
	//	if(m_pUserInfo[i])
	//	{
	//		if(m_bIsPrepare[i])
	//			continue;
	//		else
	//			tmp = true;
	//	}
	//}

	//cs.Format("prepare:%d",tmp);
	//OutputDebugString(cs);
	//if(tmp) return;

	//if(m_GameMode == GAMEMODE_BANKER_BET)			//��ׯ
	//{
	//GameBegin(byDeskStation);
	/*for(int i = 0; i < PLAY_COUNT; i++)
	if(m_pUserInfo[i])
	SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,GM_BETBANKER,0);*/
	//	OutputDebugString("MG:GAMEMODE_BANKER_BET");
	//}
	//else
	//{	
	//	T_SC_WhoIsBanker tBanker;	
	//	tBanker.byDeskStation = m_byBankerDeskStation;
	//	SendGameData(byDeskStation,&tBanker,sizeof(tBanker),MDM_GM_GAME_NOTIFY,GM_BET,0);
	//}
}

void CServerGameDesk::BetBanker()
{
	int betPlayerCount = 0;
	int flag=-1;
	for (int i =0;i<PLAY_COUNT;++i)
	{
		if(m_isBetBanker[i])
		{
			++betPlayerCount;
			flag = i;
		}
	}
	if (betPlayerCount == 1)		//ֻ��һ������ׯʱ������Ϊׯ��
	{
		m_byBankerDeskStation = flag;
	}
	else if(betPlayerCount == 0)	//������ׯʱ�����ѡһ����ׯ
	{
		for (int i =0;i<PLAY_COUNT; ++i)
		{
			if(m_pUserInfo[i] && m_bIsPrepare[i])
			{
				m_byBankerDeskStation = i;
				break;
			}
		}
	}
	else if(betPlayerCount > 1)		//����һ������ׯʱ��ѡǮ��ĵ�ׯ
	{
		__int64 PlayerMoney[PLAY_COUNT] = {0};
		for (int i = 0;i<PLAY_COUNT;++i)
		{
			if(m_pUserInfo[i] && m_bIsPrepare[i])
			{
				UserInfoStruct tuserinfo;
				m_pUserInfo[i]->GetUserData(&tuserinfo);
				PlayerMoney[i] = tuserinfo.i64Money;
			}				
		}
		int fla = 0;
		for (int i=0;i<PLAY_COUNT;++i)
		{		
			int counter=0;
			for (int j =0;j<PLAY_COUNT;++j)
			{
				if(PlayerMoney[i] >= PlayerMoney[j])
				{
					++counter;
					if(counter == 4)
					{
						fla = i;
 						break;
 					}
 				}
 			}
 		}
 		m_byBankerDeskStation = fla;
 	}
}
//---------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//�޸Ľ�������1(ƽ̨->��Ϸ)
/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return false;
}

//�޸Ľ�������2(ƽ̨->��Ϸ)
/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ�
/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ���
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return false;
}

//�޸Ľ�������3(ƽ̨->��Ϸ)
/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPondEx(__int64	iReSetAIHaveWinMoney)
{
	return false;
}

//�ж�
BOOL CServerGameDesk::Judge()
{
	if(JudgeWiner())		//�ȴ���ʤ��
	{
		return TRUE;
	}

	if (JudgeLoser())		//��������
	{
		return TRUE;
	}		

	return FALSE;
}
//================================================
//�ж�ʤ��
//================================================
BOOL CServerGameDesk::JudgeLoser()
{
	return FALSE;
}
//================================================
//�ж�ʤ��
//================================================
BOOL CServerGameDesk::JudgeWiner()
{
	return FALSE;
}
//GRM����
//���½�������(��Ϸ->ƽ̨)
/// @return ������ӮǮ��
void CServerGameDesk::UpDataRoomPond(__int64 iAIHaveWinMoney)
{

}

void CServerGameDesk::OnHandleUserNetCut(BYTE bDeskStation, CGameUserInfo* pLostUserInfo)
{
	__int64 sum = 0;
	if(bDeskStation == m_byBankerDeskStation)
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(!m_pUserInfo[i])
				continue;
			if(bDeskStation == i)
				continue;
			else
			{
				m_pUserInfo[i]->m_UserData.i64Money += m_iBetIn[i];
				sum += m_iBetIn[i];
				SendGameData(i,&m_iBetIn[i],sizeof(m_iBetIn[i]),MDM_GM_GAME_NOTIFY,GM_BUCHANG,0);
			}
		}
		
		pLostUserInfo->m_UserData.i64Money -= sum;
	}
	else
	{
		__int64 iCount = 0;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(bDeskStation == i)
				continue;
			if(!m_pUserInfo[i])
				continue;
			else
			{
				iCount++;
			}
		}

		iCount = m_iBetIn[bDeskStation] / iCount;

		pLostUserInfo->m_UserData.i64Money -= m_iBetIn[bDeskStation];

		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(bDeskStation == i)
				continue;
			if(!m_pUserInfo[i])
				continue;
			else
			{
				m_pUserInfo[i]->m_UserData.i64Money += iCount;
				SendGameData(i,&iCount,sizeof(iCount),MDM_GM_GAME_NOTIFY,GM_BUCHANG,0);
			}
		}
	}
}


void CServerGameDesk::AutoChoiseOutCard(BYTE bDeskStation,T_CS_OutCard &tOutCards)
{
	memcpy(tOutCards.allCards,m_byUserCard[bDeskStation],sizeof(BYTE)*5);
	tOutCards.cardCount = 5;

	if(m_Logic.isFourSame(&tOutCards))											//����
	{
		return;
	}
	else if (m_Logic.isAllSumTen(&tOutCards))									//ʮ��
	{
		return;
	}
	else if(m_Logic.IsBigHourse(&tOutCards))									//��˳��
	{
		return;
	}
	else if(m_Logic.IsSmallHourse(&tOutCards))									//С˳��
	{
		return;
	}
	else if(m_Logic.IsDoubleTenDoubleFive(&tOutCards))							//ͭ��
	{
		return;
	}
	else if(m_Logic.IsDoubleTen(&tOutCards))									//���
	{
		
		return;
	}
	else if(m_Logic.IsOneTen(&tOutCards))	                                     //�д�
	{
		return;
	}
	else			                                                              //�޴�
	{

	}
}
