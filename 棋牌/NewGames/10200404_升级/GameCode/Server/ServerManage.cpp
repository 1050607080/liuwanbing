#include "StdAfx.h"
#include "ServerManage.h"
#include "writelog.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <windows.h> 
#include <DbgHelp.h>  
#include <stdlib.h>  
#pragma comment(lib, "dbghelp.lib")  
#pragma comment(lib, "dbghelp.lib")  

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
	//DisableSetUnhandledExceptionFilter();  
} 


//��Ϸ����
void DebugPrintf(int iRoomID,const char *p, ...)
{
	char szFilename[256];
	sprintf( szFilename, "%s\\Log\\%d_%dServer.txt", CINIFile::GetAppPath (),NAME_ID,iRoomID);

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
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{
	//c++ test
	InitMinDump();
	memset(m_i64Score,0,sizeof(m_i64Score));
}

//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//�������ǿ����Ϣ
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}
	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:
		{
			bool  bForceQuitAsAuto = true ; 
			///�����ϲ�·��
			CString szAppPath = CINIFile::GetAppPath();

			CString strBCFFile  ; 
			CString strKeyName  ; 
			strKeyName.Format("%d" ,NAME_ID) ; 
			strBCFFile.Format("%s\\SpecialRule.bcf",szAppPath) ; 

			CBcfFile File(strBCFFile) ;

			if (File.IsFileExist(strBCFFile))
			{
				bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strKeyName ,0) ; 
			}
			if(bForceQuitAsAuto == true || (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT))
			{
				return true ; 
			}
		}
		break;
	default:
		break;
	}
	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}
//����׼����Ϣ
bool CServerGameDesk::OnHandleAgree(BYTE byDeskStation,void *pBuffer,int iSize)
{
	if(GS_WAIT_ARGEE!=m_bGameStation)
	{
		return false;
	}
	if(iSize!=sizeof(TGetReady))
	{
		return false;
	}
	TGetReady *ptGetReady = (TGetReady*)pBuffer;
	if(NULL==ptGetReady)
	{
		return false;
	}
	//�������׼��
	m_bUserReady[byDeskStation] = true;
	SendPlayerReadyMsg(byDeskStation);
	int iAgreeNums=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_bUserReady[i])
		{
			iAgreeNums++;
		}
	}
	if(PLAY_COUNT==iAgreeNums)
	{
		m_bStartFlag=true;
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
			{
				m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
			}
		}
		GameBegin(ALL_ARGEE);
	}
	return true;
}
//�����������Ϣ
bool CServerGameDesk::OnHandleBackCard(BYTE byDeskStation,void *pBuffer,int iSize)
{
	if(iSize!=sizeof(TSendCards))
	{
		return false;
	}
	TSendCards *ptSendCard  = (TSendCards*)pBuffer;
	if(ptSendCard == nullptr)
	{
		return false;
	}
	if(m_tCurrentCall.byDeskStation!=byDeskStation)
	{
		return true;
	}
	KillTimer(TIMER_BACK);
	if(ptSendCard->byCardCount>0)//ѡ����
	{
		if(8!=ptSendCard->byCardCount)
		{
			return true;
		}
		for(int i=0;i<ptSendCard->byCardCount;i++)
		{
			if(!m_Logic.IsLegalCard(ptSendCard->byCards[i]))
			{
				return true;
			}
		}
		//��ׯ����ĵ��ƴ�ׯ������ɾ��
		m_Logic.ReMoveOutCards(ptSendCard->byCards,ptSendCard->byCardCount,m_vPlayerCards[byDeskStation]);
		/*for(int i=0;i<ptSendCard->byCardCount;i++)
		{
			m_vPlayerCards[byDeskStation].push_back(m_byBackCard[i]);
		}*/
		memcpy(m_byBackCard,ptSendCard->byCards,sizeof(BYTE)*(ptSendCard->byCardCount));		//�������
	}
	else
	{
		//��ׯ����ĵ��ƴ�ׯ������ɾ��
		m_Logic.ReMoveOutCards(m_byBackCard,8,m_vPlayerCards[byDeskStation]);
	}
	//������ƣ����ʼ�� ÿȦ�ѳ���������  ÿ����ҳ��ƺ� �ó�Ա������1������PLAY_COUNT�󣬵�Ȧ���б��ƣ������ƺ�ó�Ա�����ã����ƴ�����һȦ���ȳ��ơ�
	TSendCards tBackCard;
	tBackCard.byDeskStation=byDeskStation;
	tBackCard.byCardCount=m_vPlayerCards[byDeskStation].size();	//������
	tBackCard.byBackCount=m_Logic.GetCardNums(m_byBackCard,8);
	memcpy(tBackCard.byBackCard,m_byBackCard,sizeof(tBackCard.byBackCard));
	for(int i=0;i<m_vPlayerCards[byDeskStation].size();i++)
	{
		tBackCard.byCards[i]=m_vPlayerCards[byDeskStation].at(i);
	}
	//�������Ƴɹ���Ϣ
	for (int i = 0;i<PLAY_COUNT;++i)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(byDeskStation,&tBackCard,sizeof(tBackCard),MDM_GM_GAME_NOTIFY,ASS_BACKCARDBURIED,0);
		}
	}
	SendGameData(PLAY_COUNT,&tBackCard,sizeof(tBackCard),MDM_GM_GAME_NOTIFY,ASS_BACKCARDBURIED,0);
	m_byRoundOutCardCount = 0;
	m_byNextOutCardPlayer=m_tCurrentCall.byDeskStation;
	//������ʱ��1.5��󣬸���Ȧ��������ҷ��ͳ�����Ϣ
	SetTimer(TIMER_ROUNDBOSSOUTCARD,500);
	//�����Ƹ�����
	SendAllCardsToSuper();
	return true;
}
//���������Ϣ
bool CServerGameDesk::OnHandleOutCard(BYTE byDeskStation,void *pBuffer,int iSize)
{
	if(iSize!=sizeof(T_CS_OutCard))
	{
		return false;
	}
	T_CS_OutCard *pcsOutCard = (T_CS_OutCard*)pBuffer;
	if(pcsOutCard == nullptr)
	{
		return false;
	}
	if(0==pcsOutCard->byCardNums)
	{
		return true;
	}
	if(m_byNextOutCardPlayer!=byDeskStation)
	{
		return true;
	}
	int iCardShape=m_Logic.GetCardShape(pcsOutCard->byCards,pcsOutCard->byCardNums);
	CString cs;
	cs.Format("lwlog::m_byRoundOutCardCount=%d iCardShape=%d",m_byRoundOutCardCount,iCardShape);
	OutputDebugString(cs);
	if(0==m_byRoundOutCardCount)//����ǵ�Ȧ�е�һ����ҳ�����
	{
		//�����׼ҵ���λ��
		m_byFirstOutCardDeskStation = byDeskStation;
		if(iCardShape == E_CS_MIXED)	//˦�Ƶ����
		{
			int iThrowSthle=-1;//˦������
			BYTE byOutCard[25];
			int iOutCount=0;
			memset(byOutCard,0,sizeof(byOutCard));
			T_SC_OutCardPackage tSCOutCard;
			if(m_Logic.ThrowCard(m_vPlayerCards,pcsOutCard->byCards,pcsOutCard->byCardNums,byDeskStation,iThrowSthle))
			{
				OutputDebugString("lwlog::˦�Ƴɹ�");
				iOutCount=pcsOutCard->byCardNums;
				memcpy(byOutCard,pcsOutCard->byCards,sizeof(BYTE)*pcsOutCard->byCardNums);
				//�����������
				m_tRoundCard[byDeskStation].byFlower=255;
				m_tRoundCard[byDeskStation].cardShape=E_CS_MIXED;
				m_tRoundCard[byDeskStation].iThrowStyle=iThrowSthle;
				tSCOutCard.tPreoutCards.cardShape=E_CS_MIXED;
			}
			else
			{
				//����С��
				BYTE byMinCard=m_Logic.FindMinCard(pcsOutCard->byCards,pcsOutCard->byCardNums);
				if(!m_Logic.IsLegalCard(byMinCard))
				{
					return true;
				}
				iOutCount=1;
				byOutCard[0]=byMinCard;
				//�����������
				m_tRoundCard[byDeskStation].byFlower=m_Logic.GetCardHuaKind(byMinCard);
				m_tRoundCard[byDeskStation].cardShape=E_CS_SINGLE;
				tSCOutCard.tPreoutCards.cardShape=E_CS_SINGLE;
			}
			memcpy(m_tRoundCard[byDeskStation].byCards,byOutCard,sizeof(BYTE)*iOutCount);
			m_tRoundCard[byDeskStation].iCardNums=iOutCount;
			//���ӵ�Ȧ���ѳ�������
			++m_byRoundOutCardCount;
			//������ɾ��������
			m_Logic.ReMoveOutCards(byOutCard,iOutCount,m_vPlayerCards[byDeskStation]);
			RecordTurnCardData(byDeskStation,byOutCard,iOutCount);
			//���������ݸ�����
			SendAllCardsToSuper();
			m_byNextOutCardPlayer=m_Logic.GetNextDeskStation(byDeskStation);
			tSCOutCard.byOutCardPlayerDeskStation = m_byNextOutCardPlayer;
			tSCOutCard.tPreoutCards.isFirst=true;
			tSCOutCard.tPreoutCards.byCardNums=iOutCount;
			memcpy(tSCOutCard.tPreoutCards.byCards,byOutCard,sizeof(BYTE)*iOutCount);
			tSCOutCard.tPreoutCards.byDeskStation=byDeskStation;
			tSCOutCard.tPreoutCards.byFlower=m_tRoundCard[byDeskStation].byFlower;
			tSCOutCard.tPreoutCards.iThrowStyle=iThrowSthle;
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&tSCOutCard,sizeof(T_SC_OutCardPackage),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
				}
			}
			SendWatchData(PLAY_COUNT,&tSCOutCard,sizeof(tSCOutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
			KillTimer(TIMER_THINK);
			SetTimer(TIMER_THINK,(m_iTimeToThink+2)*1000);
			return true;
		}
		else
		{
			if(E_CS_INVALIED==iCardShape || E_CS_MIXED==iCardShape)
			{
				return true;
			}
			//���ӵ�Ȧ���ѳ�������
			++m_byRoundOutCardCount;
			//�����������
			m_tRoundCard[byDeskStation].byFlower=m_Logic.GetCardHuaKind(pcsOutCard->byCards[0]);
			m_tRoundCard[byDeskStation].cardShape=iCardShape;
			m_tRoundCard[byDeskStation].iCardNums=pcsOutCard->byCardNums;
			memcpy(m_tRoundCard[byDeskStation].byCards,pcsOutCard->byCards,sizeof(BYTE)*pcsOutCard->byCardNums);

			//�������ƴ�����ɾ��
			m_Logic.ReMoveOutCards(pcsOutCard->byCards,pcsOutCard->byCardNums,m_vPlayerCards[byDeskStation]);
			RecordTurnCardData(byDeskStation,pcsOutCard->byCards,pcsOutCard->byCardNums);
			//���������ݸ�����
			SendAllCardsToSuper();
			m_byNextOutCardPlayer=m_Logic.GetNextDeskStation(byDeskStation);
			T_SC_OutCardPackage tSCOutCard;
			tSCOutCard.byOutCardPlayerDeskStation = m_byNextOutCardPlayer;
			tSCOutCard.tPreoutCards.isFirst=true;
			tSCOutCard.tPreoutCards.byCardNums=pcsOutCard->byCardNums;
			memcpy(tSCOutCard.tPreoutCards.byCards,pcsOutCard->byCards,sizeof(BYTE)*pcsOutCard->byCardNums);
			tSCOutCard.tPreoutCards.byDeskStation=byDeskStation;
			tSCOutCard.tPreoutCards.cardShape=iCardShape;
			tSCOutCard.tPreoutCards.byFlower=m_tRoundCard[byDeskStation].byFlower;
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&tSCOutCard,sizeof(T_SC_OutCardPackage),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
				}
			}
			SendWatchData(PLAY_COUNT,&tSCOutCard,sizeof(tSCOutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
			KillTimer(TIMER_THINK);
			SetTimer(TIMER_THINK,(m_iTimeToThink+2)*1000);
			return true;
		}
	}
	else	//���ǵ�Ȧ��һ�����Ƶģ� �������ܹ�����
	{
		if(E_CS_INVALIED==iCardShape)
		{
			return true;
		}
		//TODO:�ж��Ƿ��ܹ����ƣ������򱣴�������ݣ��ȴ���Ȧ����
		WCHAR buffer[100] = {0};
		if(m_Logic.IsOutCardSuccess(m_tRoundCard[m_byFirstOutCardDeskStation],pcsOutCard->byCards,pcsOutCard->byCardNums,iCardShape,m_vPlayerCards[byDeskStation]))
		{
			//���ӵ�Ȧ���ѳ�������
			++m_byRoundOutCardCount;
			//TODO�������������
			m_tRoundCard[byDeskStation].byFlower=m_Logic.GetCardHuaKind(pcsOutCard->byCards[0]);
			m_tRoundCard[byDeskStation].cardShape=iCardShape;
			m_tRoundCard[byDeskStation].iCardNums=pcsOutCard->byCardNums;
			memcpy(m_tRoundCard[byDeskStation].byCards,pcsOutCard->byCards,sizeof(BYTE)*pcsOutCard->byCardNums);
			//ɾ��������
			m_Logic.ReMoveOutCards(pcsOutCard->byCards,pcsOutCard->byCardNums,m_vPlayerCards[byDeskStation]);
			RecordTurnCardData(byDeskStation,pcsOutCard->byCards,pcsOutCard->byCardNums);
			//���������ݸ�����
			SendAllCardsToSuper();

			//TODO:�ж��Ƿ���е�Ȧ����
			if(m_byRoundOutCardCount == PLAY_COUNT)	//����Ȧ�������� == PLAY_COUNTʱ�����е�Ȧ����
			{
				m_iTurnNums++;
				//���õ�Ȧ��������
				m_byRoundOutCardCount = 0;
				//����
				m_iRoundScore = 0;
				m_tCurrentCall.byCallCard[0];
				m_byNextOutCardPlayer = m_Logic.SinglePK(m_tRoundCard,m_byFirstOutCardDeskStation,m_iRoundScore);
				//���ׯ��Ӯ�����޷�
				if(m_byNextOutCardPlayer== m_tCurrentCall.byDeskStation)
				{
					m_iRoundScore = 0;
				}
				if(m_iRoundScore>0)
				{
					BYTE byWinDeskStation = m_tCurrentCall.byDeskStation %2 +1;
					//�ܷ�
					m_iTotalScore += m_iRoundScore;
					//���͵÷���Ϣ
					T_SC_Score tScore;
					tScore.byDeskStation[0]=byWinDeskStation;
					tScore.byDeskStation[1]=sByFaceDeskStation[byWinDeskStation];
					tScore.iScore = m_iTotalScore;
					for (int i=0;i<PLAY_COUNT;++i)
					{
						if(m_pUserInfo[i])
						{
							SendGameData(i,&tScore,sizeof(T_SC_Score),MDM_GM_GAME_NOTIFY,ASS_SCORE,0);
						}
					}
					SendWatchData(PLAY_COUNT,&tScore,sizeof(tScore),MDM_GM_GAME_NOTIFY,ASS_SCORE,0);
				}
				if(m_vPlayerCards[byDeskStation].size() == 0)
				{
					T_SC_OutCardPackage tSCOutCard;
					tSCOutCard.byOutCardPlayerDeskStation = 255;
					tSCOutCard.tPreoutCards.isFirst=false;
					tSCOutCard.tPreoutCards.byCardNums=pcsOutCard->byCardNums;
					memcpy(tSCOutCard.tPreoutCards.byCards,pcsOutCard->byCards,sizeof(BYTE)*pcsOutCard->byCardNums);
					tSCOutCard.tPreoutCards.byDeskStation=byDeskStation;
					tSCOutCard.tPreoutCards.cardShape=iCardShape;
					for (int i = 0;i<PLAY_COUNT;++i)
					{
						SendGameData(i,&tSCOutCard,sizeof(T_SC_OutCardPackage),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
					}
					SendWatchData(PLAY_COUNT,&tSCOutCard,sizeof(tSCOutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
					//��õ��Ʒ�
					m_iBackCardScore= 0;
					m_iBackCardScore=m_Logic.GetPoint(m_byBackCard,8);
					int iTemBackScore=m_iBackCardScore;
					//���Ƶ÷�
					if(m_byNextOutCardPlayer == m_tCurrentCall.byDeskStation)
					{
						iTemBackScore = 0;
					}
					if(iTemBackScore>0)
					{
						BYTE byWinDeskStation = m_tCurrentCall.byDeskStation %2 +1;
						//�ܷ�
						m_iTotalScore += iTemBackScore;
						//���͵÷���Ϣ
						T_SC_Score tScore;
						tScore.byDeskStation[0]=byWinDeskStation;
						tScore.byDeskStation[1]=sByFaceDeskStation[byWinDeskStation];
						tScore.iScore = m_iTotalScore;
						for (int i=0;i<PLAY_COUNT;++i)
						{
							if(m_pUserInfo[i])
							{
								SendGameData(i,&tScore,sizeof(T_SC_Score),MDM_GM_GAME_NOTIFY,ASS_SCORE,0);
							}
						}
						SendWatchData(PLAY_COUNT,&tScore,sizeof(tScore),MDM_GM_GAME_NOTIFY,ASS_SCORE,0);
					}
					//����
					//m_iTotalScore=85;//��ׯ����
					//TODO: ���ִ��꣬����
					if(m_iTotalScore == 0)
					{
						m_byGroupOneevel += 3;	//ׯ�ҷ�����3��
					}
					else if( m_iTotalScore <40 && m_iTotalScore >0)
					{
						m_byGroupOneevel += 2;	//ׯ�ҷ�����2��
					}
					else if(m_iTotalScore <80 && m_iTotalScore >=40)
					{
						m_byGroupOneevel += 1;	//ׯ�ҷ�����1��
					}
					else if(m_iTotalScore <100 && m_iTotalScore >=80)//��ׯ
					{
						//��������
					}
					else if(m_iTotalScore <120 && m_iTotalScore >=100)
					{
						m_byGoupTwolevel += 1;	//ץ����1��
						
					}
					else if(m_iTotalScore <160 && m_iTotalScore >=120)
					{
						m_byGoupTwolevel += 2;	//ץ������2��
						
					}
					else if(m_iTotalScore >= 160)
					{
						m_byGoupTwolevel += 3;	//ץ������3��
						
					}
					else if(200==m_iTotalScore)
					{
						m_byGoupTwolevel += 4;	//ץ������4��
						
					}
					// ��ȡׯ����λ�� m_tCurrentCall.byDeskStation
					BYTE byWinDeskStation=255;
					if(m_iTotalScore >=0 && m_iTotalScore <80)//ׯ��һ��Ӯ
					{ 
						byWinDeskStation = m_tCurrentCall.byDeskStation;
					}
					else//�м�һ��Ӯ
					{
						byWinDeskStation = m_tCurrentCall.byDeskStation %2 +1;
					}
					memset(m_i64Score,0,sizeof(m_i64Score));
					if(byWinDeskStation>=0 && byWinDeskStation<PLAY_COUNT)
					{
						//Ӯ��һ�����
						m_i64Score[byWinDeskStation]+=1000;
						m_i64Score[sByFaceDeskStation[byWinDeskStation]]+=1000;
						//���һ�����
						BYTE byTempDestStation=byWinDeskStation+1;
						if(byTempDestStation>=PLAY_COUNT)
						{
							byTempDestStation=0;
						}
						m_i64Score[byTempDestStation]-=1000;
						byTempDestStation=sByFaceDeskStation[byWinDeskStation]+1;
						if(byTempDestStation>=PLAY_COUNT)
						{
							byTempDestStation=0;
						}
						m_i64Score[byTempDestStation]-=1000;
					}
					if(m_iTotalScore>=80)//��Ҫ��ׯ
					{
						m_tCurrentCall.byDeskStation= m_tCurrentCall.byDeskStation %2 +1;;
						BYTE byTemp=m_byGroupOneevel;
						m_byGroupOneevel=m_byGoupTwolevel;
						m_byGoupTwolevel=byTemp;
					}
					//��Ϸ����
					GameFinish(byDeskStation,GF_NORMAL);
					KillTimer(TIMER_THINK);
					//������ʱ����3�������Ϸ������Ϣ
					//SetTimer(TIMER_GAMEOVER,3000);
				}
				else
				{
					T_SC_OutCardPackage tSCOutCard;
					tSCOutCard.tPreoutCards.isFirst=false;
					tSCOutCard.tPreoutCards.byCardNums=pcsOutCard->byCardNums;
					memcpy(tSCOutCard.tPreoutCards.byCards,pcsOutCard->byCards,sizeof(BYTE)*pcsOutCard->byCardNums);
					tSCOutCard.tPreoutCards.byDeskStation=byDeskStation;
					tSCOutCard.tPreoutCards.cardShape=iCardShape;
					for (int i = 0;i<PLAY_COUNT;++i)
					{
						SendGameData(i,&tSCOutCard,sizeof(T_SC_OutCardPackage),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
					}
					SendWatchData(PLAY_COUNT,&tSCOutCard,sizeof(tSCOutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
					//��� m_tRoundCard �е�����
					memset(m_tRoundCard,0,sizeof(m_tRoundCard));
					KillTimer(TIMER_THINK);
					//������ʱ��1.5��󣬸���Ȧ��������ҷ��ͳ�����Ϣ
					SetTimer(TIMER_ROUNDBOSSOUTCARD,1.5*1000);
				}
			}
			else
			{
				//TODO:֪ͨ��һ����ҳ���
				m_byNextOutCardPlayer=m_Logic.GetNextDeskStation(byDeskStation);
				T_SC_OutCardPackage tSCOutCard;
				tSCOutCard.byOutCardPlayerDeskStation = m_byNextOutCardPlayer;
				tSCOutCard.tPreoutCards.isFirst=false;
				tSCOutCard.tPreoutCards.byCardNums=pcsOutCard->byCardNums;
				memcpy(tSCOutCard.tPreoutCards.byCards,pcsOutCard->byCards,sizeof(BYTE)*pcsOutCard->byCardNums);
				tSCOutCard.tPreoutCards.byDeskStation=byDeskStation;
				tSCOutCard.tPreoutCards.cardShape=iCardShape;
				for (int i = 0;i<PLAY_COUNT;++i)
				{
					if(m_pUserInfo[i])
					{
						SendGameData(i,&tSCOutCard,sizeof(T_SC_OutCardPackage),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
					}
				}
				SendWatchData(PLAY_COUNT,&tSCOutCard,sizeof(tSCOutCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
				KillTimer(TIMER_THINK);
				SetTimer(TIMER_THINK,(m_iTimeToThink+2)*1000);
			}
		}
		else
		{
			CString cs;
			cs.Format("lwlog::����ʧ����?????????????????????");
			OutputDebugString(cs);
		}
	}
	return true;
}

//�������
void CServerGameDesk::SendCardFinish()
{
	if (!m_bIsCalled)	//���˽���
	{
		/*srand(time(0));
		int ira = rand() %PLAY_COUNT;
		m_tCurrentCall.byDeskStation = ira;*/
		m_tCurrentCall.byLevelColor = 255;
	}
	if(255==m_tCurrentCall.byDeskStation)//���ѡ��һ����ׯ
	{
		srand(time(0));
		int ira = rand() %PLAY_COUNT;
		m_tCurrentCall.byDeskStation = ira;
	}
	m_tCurrentCall.byLevel=m_byGroupOneevel;
	m_Logic.SetLevelAndFlower(m_byGroupOneevel,m_tCurrentCall.byLevelColor);//���ü��������ɫ
	//֪ͨ˭��ׯ��
	for (int i =0;i<PLAY_COUNT;++i)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&m_tCurrentCall,sizeof(TCallLevelCard),MDM_GM_GAME_NOTIFY,ASS_SENDFINISH,0);
		}
	}
	SendWatchData(PLAY_COUNT,&m_tCurrentCall,sizeof(TCallLevelCard),MDM_GM_GAME_NOTIFY,ASS_SENDFINISH,0);
	SetTimer(TIMER_SENDBACKCARD,1200);
}
//�������
bool CServerGameDesk::OnHandleLookCard(BYTE byDeskStation,void *pBuffer,int iSize)
{
	if(iSize!=sizeof(TLookTurnData))
	{
		return false;
	}
	TLookTurnData *pLookTurn = (TLookTurnData*)pBuffer;
	if(NULL== pLookTurn)
	{
		return false;
	}
	if(m_iTurnNums>=1)
	{
		TLookTurnDataRes sendLookData;
		memcpy(sendLookData.iCardNums,m_tTurnCardData[m_iTurnNums-1].iCardNums,sizeof(m_tTurnCardData[m_iTurnNums-1].iCardNums));
		memcpy(sendLookData.byCards,m_tTurnCardData[m_iTurnNums-1].byCards,sizeof(m_tTurnCardData[m_iTurnNums-1].byCards));
		SendGameData(byDeskStation,&sendLookData,sizeof(sendLookData),MDM_GM_GAME_NOTIFY,ASS_LOOK_TURNRES,0);	
	}
	return true;
}
//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	//MessageBox(NULL,"2","3",0);
	CString cs;
	cs.Format("lwlog::pNetHead->bAssistantID=%d",pNetHead->bAssistantID);
	//OutputDebugString(cs);
	switch (pNetHead->bAssistantID)
	{
	case ASS_LOOK_TURN://�鿴��һ������
		{
			return OnHandleLookCard(bDeskStation,pData,uSize);
		}
	case ASS_GETREADY:
		{
			return OnHandleAgree(bDeskStation,pData,uSize);
		}
	case ASS_CALL:	//����
		{
			TCallLevelCard *ptCallLevelCard = (TCallLevelCard*)pData;
			if(ptCallLevelCard == nullptr)
			{
				return false;
			}
			CallGameLevelCard(ptCallLevelCard,bDeskStation);
			return true;
		}
	case ASS_BURYCARD:		//�����
		{
			return OnHandleBackCard(bDeskStation,pData,uSize);
		}
	case ASS_OUT_CARD:		//����
		{
			return OnHandleOutCard(bDeskStation,pData,uSize);
		}
	case ASS_SUPER_USER:
		{
			m_bySuperDeskStation[bDeskStation] = true;
			return true;
		}
		break;
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIMER_BACK://����
		{
			KillTimer(TIMER_BACK);
			TSendCards tSendCards;
			OnHandleBackCard(m_tCurrentCall.byDeskStation,&tSendCards,sizeof(tSendCards));
			break;
		}
	case TIMER_THINK://˼��
		{
			KillTimer(TIMER_THINK);
			BYTE byCardList[33];
			memset(byCardList,0,sizeof(byCardList));
			for(int i=0;i<m_vPlayerCards[m_byNextOutCardPlayer].size();i++)
			{
				byCardList[i]=m_vPlayerCards[m_byNextOutCardPlayer].at(i);
			}
			m_Logic.SortCard(byCardList,33);
			int iCardNums=m_Logic.GetCardNums(byCardList,33);
			T_CS_OutCard tOutCard;
			m_Logic.AutoOutCard(m_tRoundCard[m_byFirstOutCardDeskStation],byCardList,iCardNums,m_byRoundOutCardCount,tOutCard);
			
			CString cs="�������Զ�����::";
			CString cs1="";
			for(int i=0;i<tOutCard.byCardNums;i++)
			{
				cs1.Format("byCards[%d]=%d ",i,tOutCard.byCards[i]);
				cs.Append(cs1);
			}
			Debuglog(cs);
			OnHandleOutCard(m_byNextOutCardPlayer,&tOutCard,sizeof(tOutCard));
			break;
		}
	case TIMER_SENDCARD://����
		{
			KillTimer(TIMER_SENDCARD);
			SendCardFinish();
			break;
		}
	case TIMER_SENDBACKCARD:
		{
			KillTimer(TIMER_SENDBACKCARD);
			//�����Ʒ��͸�ׯ��
			TSendCards tBackCard;
			tBackCard.byDeskStation = m_tCurrentCall.byDeskStation;
			int index=0;
			for(int i=100;i<m_vbyAllCards.size();i++)
			{
				tBackCard.byCards[index]=m_vbyAllCards.at(i);
				m_byBackCard[index]=m_vbyAllCards.at(i);
				index++;
				if(8==index)
				{
					break;
				}
			}
			for(int i=0;i<8;i++)
			{
				m_vPlayerCards[m_tCurrentCall.byDeskStation].push_back(m_byBackCard[i]);
			}
			tBackCard.byCardCount = 8;
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&tBackCard,sizeof(tBackCard),MDM_GM_GAME_NOTIFY,ASS_SENDBACKCARD,0);
				}
			}
			SendWatchData(PLAY_COUNT,&tBackCard,sizeof(tBackCard),MDM_GM_GAME_NOTIFY,ASS_SENDBACKCARD,0);
			SendAllCardsToSuper();
			SetTimer(TIMER_BACK,(m_iTimeToThink+2)*1000);
		}
		break;
	case TIMER_ROUNDBOSSOUTCARD://�µ�һ�ֿ�ʼ
		{
			KillTimer(TIMER_ROUNDBOSSOUTCARD);
			T_SC_NewTurn tNewTurn;
			tNewTurn.byDeskStation = m_byNextOutCardPlayer;
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&tNewTurn,sizeof(tNewTurn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);
				}
			}
			SendWatchData(PLAY_COUNT,&tNewTurn,sizeof(tNewTurn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);
			SetTimer(TIMER_THINK,(m_iTimeToThink+2)*1000);
		}
		break;
	case TIMER_SENDOTHERCARD:
		{
			KillTimer(TIMER_SENDOTHERCARD);
			for (int i = 0;i<PLAY_COUNT;++i)
			{
				TOtherCards tOthercards;
				int iCounter = 0;
				if(m_bySuperDeskStation[i])
				{
					for (int j = 0;j<PLAY_COUNT;++j)
					{
						if(j == i)
							continue;
						else
						{
							tOthercards.tsendCards[iCounter].byCardCount = m_vPlayerCards[j].size();
							tOthercards.tsendCards[iCounter].byDeskStation = j;
							memcpy(tOthercards.tsendCards[iCounter].byCards,&m_vbyAllCards[j*25],sizeof(BYTE)*25);
							++iCounter;
						}
					}
					SendGameData(i,&tOthercards,sizeof(TOtherCards),MDM_GM_GAME_NOTIFY,ASS_SENDOTHERCARD,0);
				}
			}
		}
		break;
	}
	return __super::OnTimer(uTimerID);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	/// �����������ʾ�����й�״̬���������������Ϊ���й�
	if (!bWatchUser)
	{
	}

	//������Ϸ������Ϣ��Ϣ
	TGameConfigs tGameConfigs;
	tGameConfigs.iTimeToCallNT = m_iTimeToCallNT;
	tGameConfigs.iTimeToGetReady = m_iTimeToReady;
	tGameConfigs.iTimeToThink = m_iTimeToThink;
	tGameConfigs.iSendCardSpeed = m_iSendCardSpeed;
	for (int i= 0;i<PLAY_COUNT;++i)
	{
		if (m_pUserInfo[i])
		{
			SendGameData(i,&tGameConfigs,sizeof(TGameConfigs),MDM_GM_GAME_NOTIFY,ASS_CONFIGINFO,0);
		}
	}

	for (vector<long>::iterator it = m_vSuperUsers.begin();it != m_vSuperUsers.end();++it)
	{
		if( (m_pUserInfo[bDeskStation]->m_UserData.dwUserID) == *it)
		{
			SendGameData(bDeskStation,NULL,0,MDM_GM_GAME_NOTIFY,ASS_SUPER_USER,0);
		}
	}
	

	//������Ϸ���Ӻ� ����д��־
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			TGameStation tGameStation;	//��Ϸ״̬��
			tGameStation.byGameStation = GS_WAIT_ARGEE;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&tGameStation,sizeof(tGameStation));
			//��������Щ����Ѿ�׼��
			SendPlayerReadyMsg();
			return TRUE;
		}
		break;
	case GS_WASHING_CARD:
	case GS_SEND_CARD:		//����״̬
	case GS_PLAY_GAME:		//��Ϸ��״̬
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			TGameStation tGameStation;	//��Ϸ״̬��
			tGameStation.byGameStation = m_bGameStation;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&tGameStation,sizeof(tGameStation));
			//����ȡ���������й�
			return TRUE;
		}
		break;
	}
	return false;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillTimer(TIMER_SENDBACKCARD);
	KillTimer(TIMER_ROUNDBOSSOUTCARD);
	KillTimer(TIMER_GAMEOVER);
	KillTimer(TIMER_SENDOTHERCARD);
	KillTimer(TIMER_SENDCARD);
	KillTimer(TIMER_THINK);
	KillTimer(TIMER_BACK);
	memset(m_bUserReady,false,sizeof(bool)*PLAY_COUNT);
	m_bIsCalled = false;
	m_byRoundOutCardCount = 0;
	m_byFirstOutCardDeskStation = 255;

	memset(&m_tRoundCard,0,sizeof(T_CS_OutCard)*PLAY_COUNT);
	for (int i =0;i<PLAY_COUNT;++i)
	{
		m_vPlayerCards[i].clear();
	}
	m_vbyAllCards.clear();

	m_iRoundScore = 0;
	m_iTotalScore = 0;
	m_byNextOutCardPlayer = 255;

	m_tCurrentCall.bySize = 0;
	m_tCurrentCall.byDeskStation = 255;
	m_tCurrentCall.byLevelColor = 255;
	memset(m_tCurrentCall.byCallCard,0,sizeof(BYTE)*2);
	memset(m_i64Score,0,sizeof(m_i64Score));
	memset(m_byBackCard,0,sizeof(m_byBackCard));
	m_bGameStation = GS_WAIT_ARGEE;
	m_bStartFlag=false;
	m_bQiangNt=false;
	m_iBackCardScore= 0;
	return TRUE;
}

/*---------------------------------------------------------------------------------*/
//��Ϸ��ʼ
bool	CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false || !m_bStartFlag)
	{
		GameFinish(0,GF_SAFE);
		return false;
	}
	//static int lw=0;//����
	//if(0==lw)
	//{
	//	lw=1;
	//	m_byGroupOneevel=2;//����
	//	m_byGoupTwolevel=3;//����
	//}
	if(255==m_tCurrentCall.byDeskStation)
	{
		m_bQiangNt=true;
	}
	else
	{
		m_bQiangNt=false;
	}
	//������Ϸ������Ϣ
	SendGameLevelMsg();
	//ϴ��
	//WashCards(m_vbyAllCards);
	BYTE byCards[108] = {0};
	m_Logic.RandCard(byCards,108,true);
	m_vbyAllCards.clear();
	for (int i= 0;i<108;++i)
	{
		m_vbyAllCards.push_back(byCards[i]);
	}
	//������Ϸ״̬
	m_bGameStation = GS_PLAY_GAME;
	SendCards();
	//m_bGameStation = GS_WASHING_CARD;	//������Ϸϴ��״̬
	//����ϴ����Ϣ(��ʱ��ϴ�ƶ���)
	//SendWashCardMsg();
	//�������ƶ�ʱ��
	//SetTimer(TIME_SEND_CARD,1000*3);
	return TRUE;
}
//����
void CServerGameDesk::SendCards()
{
	for (int i = 0;i<PLAY_COUNT;++i)	//����������
	{
		if(m_pUserInfo[i])
		{
			TSendCards tSendCards;
			tSendCards.byDeskStation = i;
			tSendCards.byCardCount = 25;
			memcpy(&tSendCards.byCards,&m_vbyAllCards[i*25],sizeof(BYTE)*25);
			SendGameData(i,&tSendCards,sizeof(tSendCards),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);		//
		}
	}
	//����ÿ����ҵ�������;
	for (int i = 0;i<PLAY_COUNT;++i)
	{
		for (int j = 0;j<25;++j)
		{
			m_vPlayerCards[i].push_back(m_vbyAllCards[i*25 + j]);
		}
	}
	SetTimer(TIMER_SENDOTHERCARD,500);
	SetTimer(TIMER_SENDCARD,15*1000);
}
//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	memset(m_tTurnCardData,0,sizeof(m_tTurnCardData));
	m_iTurnNums=0;
	//��д����
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			TGameResult tResult;
			tResult.iBackCardScore=m_iBackCardScore;
			tResult.iTotalScore=m_iTotalScore;
			//�������ݿ�
			__int64 i64Money[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			memset(i64Money,0,sizeof(i64Money));
			ChangeUserPointint64(m_i64Score, temp_cut);
			__super::RecoderGameInfo(i64Money);//��˰�Ժ�Ľ��
			memcpy(tResult.i64Money,i64Money,sizeof(tResult.i64Money));
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&tResult,sizeof(tResult),MDM_GM_GAME_NOTIFY,ASS_GAMEOVER,0);
				}
			}
			SendWatchData(PLAY_COUNT,&tResult,sizeof(tResult),MDM_GM_GAME_NOTIFY,ASS_GAMEOVER,0);
			break;
		}
	case GFF_SAFE_FINISH:
		{
			memset(m_i64Score,0,sizeof(m_i64Score));
			m_i64Score[bDeskStation]=-300;
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(bDeskStation!=i)
				{
					m_i64Score[i]=100;
				}
			}
			TGameResult tResult;
			//�������ݿ�
			__int64 i64Money[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			memset(i64Money,0,sizeof(i64Money));
			ChangeUserPointint64(m_i64Score, temp_cut);
			__super::RecoderGameInfo(i64Money);//��˰�Ժ�Ľ��
			memcpy(tResult.i64Money,i64Money,sizeof(tResult.i64Money));
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&tResult,sizeof(tResult),MDM_GM_GAME_NOTIFY,ASS_GAMEOVER,0);
				}
			}
			SendWatchData(PLAY_COUNT,&tResult,sizeof(tResult),MDM_GM_GAME_NOTIFY,ASS_GAMEOVER,0);
			break;
		}
	case GF_SAFE:			//��Ϸ��ȫ����
		{
			break;
		}
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			break;
		}
	}
	//��������
	ReSetGameState(bCloseFlag);
	__super::GameFinish(bDeskStation,bCloseFlag);
	return true;
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	ResetGameLevel();
	m_tCurrentCall.init();
	if(GS_WAIT_ARGEE==m_bGameStation)
	{
		return false;
	}
	else
	{
		GameFinish(bDeskStation,GFF_SAFE_FINISH);
		return false;
	}
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGameEx(BYTE bDeskStation)
{
	return true;
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	OutputDebugString("lwlog::�û��뿪��Ϸ��");
	m_bUserReady[bDeskStation] = false;
	// 	int iCounter = 0;
	// 	for (int i = 0;i<PLAY_COUNT;++i)
	// 	{
	// 		if(!m_bUserReady[i])
	// 		{
	// 			++iCounter;
	// 		}
	// 	}
	// 	if (iCounter == PLAY_COUNT)
	// 	{
	// 		GameFinish(bDeskStation,GFF_SAFE_FINISH);
	// 		InitDeskGameStation();
	// 	}
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}
///�û������뿪
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	return true;
}

//�����ı��û����������ĺ���
bool CServerGameDesk::CanNetCut(BYTE bDeskStation)
{
	return true;
}
//��ʼ��
bool CServerGameDesk::InitDeskGameStation()
{
	/*BYTE byCardList[4]={0x03,0x13,0x15,0x25};
	CString cs;
	cs.Format("lwlog::m_Logic=%d",m_Logic.IsContinueDouble(byCardList,4));
	OutputDebugString(cs);*/
	//���������ļ�
	LoadIni();
	//��ʼ������������
	ResetGameLevel();								//��ʼ����Ϸ����
	memset(m_bUserReady,0,sizeof(m_bUserReady));	//��ʼ������Ƿ�׼��
	memset(m_bUserNetCut,0,sizeof(m_bUserNetCut));	//����Ƿ����
	m_bGameStation = GS_WAIT_ARGEE;					//��Ϸ״̬
	m_iRoundScore = 0;								//��Ȧ�÷�
	m_iTotalScore = 0;								//�ܵ÷�
	memset(m_bySuperDeskStation,0,sizeof(bool)*4);
	memset(m_tTurnCardData,0,sizeof(m_tTurnCardData));
	memset(m_byBackCard,0,sizeof(m_byBackCard));
	m_bStartFlag=false;
	m_iTurnNums=0;
	m_tCurrentCall.init();
	/*vector<BYTE> vSerialCard;
	vSerialCard.clear();
	BYTE byCardList[30]={0x03, 0x04, 0x03, 0x04,0x05,0x05, 0x06, 0x06, 0x0A, 0x17, 0x18, 0x19, 0x17, 0x18, 0x19,
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x27, 0x28, 0x2B, 0x2C, 0x2D,0x2C,0x2D};
	CString cs;
	cs.Format("lwlog::������Ŀ=%d",m_Logic.GetSerialNums(byCardList,30,0,0,4,vSerialCard));
	OutputDebugString(cs);
	for(int i=0;i<vSerialCard.size();i++)
	{
		cs.Format("lwlog::vSerialCard[%d]=%d",i,vSerialCard.at(i));
		OutputDebugString(cs);
	}*/

	/*BYTE byList[9]={0x01,0x31,0x05,0x01};
	m_Logic.SortCard(byList,4,0x00,0x01);
	for(int i=0;i<9;i++)
	{
		CString cs;
		cs.Format("lwlog::byList[%d]=%d",i,byList[i]);
		OutputDebugString(cs);
	}
	OutputDebugString("lwlog::===============");*/
	//m_Logic.SetLevelAndFlower(10,0x30);
	
	/*CString cs;
	cs.Format("lwlog::CompareCard=%d",m_Logic.CompareCard(0x3A,0x1A));
	OutputDebugString(cs);*/
	return true;
}
//����ϴ����Ϣ
void CServerGameDesk::SendWashCardMsg()
{
	for (int i = 0;i<PLAY_COUNT;++i)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_WASHINGCARD,0);
		}
	}
	SendWatchData(PLAY_COUNT,NULL,0,MDM_GM_GAME_NOTIFY,ASS_WASHINGCARD,0);
}

//for debug
void CServerGameDesk::ForDebugging(TSendCards& tSendCards)
{
	for(int i = 0;i<25;++i)
	{
		CString cs;
		cs.Format("chs card %d: %d",i,tSendCards.byCards[i]);
		OutputDebugString(cs);
	}
	OutputDebugString("chs \n");
}

//������Ϸ����
void CServerGameDesk::ResetGameLevel()
{
	m_byGroupOneevel = 2;	//��һ�����ڴ����
	m_byGoupTwolevel = 2;	//�ڶ������ڴ����

	//��ʼ����ǰ��������
	m_tCurrentCall.byDeskStation = 255;
	memset(m_tCurrentCall.byCallCard,0,sizeof(BYTE)*2);
	m_bIsCalled = false;		//��ʼ���Ƿ��Ѿ����˽���
}

//������Ϸ������Ϣ
void CServerGameDesk::SendGameLevelMsg()
{
	TGameLevel tGameLevel;
	for(int i = 0;i<PLAY_COUNT;++i)
	{
		if(m_pUserInfo[i])
		{
			tGameLevel.byDeskStation = i;
			tGameLevel.byGoup1Level = m_byGroupOneevel;
			tGameLevel.byGoup2Level = m_byGoupTwolevel;
			SendGameData(i,&tGameLevel,sizeof(tGameLevel),MDM_GM_GAME_NOTIFY,ASS_GAMELEVEL,0);
		}
	}
	SendWatchData(PLAY_COUNT,&tGameLevel,sizeof(tGameLevel),MDM_GM_GAME_NOTIFY,ASS_GAMELEVEL,0);
}

//�������׼��״̬��Ϣ
void CServerGameDesk::SendPlayerReadyMsg(BYTE byDeskStation)
{
	TGetReady tGetReady;
	tGetReady.byDeskStation=byDeskStation;
	memcpy(tGetReady.bUserGetReady,m_bUserReady,sizeof(BYTE)*PLAY_COUNT);
	for(int i = 0;i<PLAY_COUNT;++i)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&tGetReady,sizeof(tGetReady),MDM_GM_GAME_NOTIFY,ASS_GETREADY,0);
		}
	}
	SendWatchData(PLAY_COUNT,&tGetReady,sizeof(TGetReady),MDM_GM_GAME_NOTIFY,ASS_GETREADY,0);	//�Թ���Ϣ
}

// ��������
void CServerGameDesk::CallGameLevelCard(TCallLevelCard *ptCallLevelCard,BYTE byDeskStation)
{
	CString lwlog="";
	lwlog.Format("��������::bySize=%d byCallCard[0]=%d byCallCard[1]=%d",ptCallLevelCard->bySize,ptCallLevelCard->byCallCard[0],ptCallLevelCard->byCallCard[1]);
	Debuglog(lwlog);
	lwlog.Format("m_bIsCalled=%d m_tCurrentCall.bySize=%d",m_bIsCalled,m_tCurrentCall.bySize);
	Debuglog(lwlog);
	if(m_bIsCalled == false)	//�����û�˽�������ֱ�ӱ���������ݣ���֪ͨ������ҵ�ǰ��������
	{
		m_bIsCalled = true;	
		if(m_bQiangNt)
		{
			m_tCurrentCall.byDeskStation = byDeskStation;
		}
		m_tCurrentCall.byLevelColor = ptCallLevelCard->byLevelColor;
		m_tCurrentCall.bySize=ptCallLevelCard->bySize;
		//���漶��
		memcpy(m_tCurrentCall.byCallCard,ptCallLevelCard->byCallCard,sizeof(BYTE)*(ptCallLevelCard->bySize));
		SendCallGameLevelCard(ptCallLevelCard);
 	}
 	else	//�������˽����������Ƿ��ܹ�����
 	{
		switch(m_tCurrentCall.bySize)
		{
		case 1:
			{
				if(2==ptCallLevelCard->bySize && ptCallLevelCard->byCallCard[1]==ptCallLevelCard->byCallCard[0])//Ϊ���ӣ���ֵ��ǰһ������һ��
				{
					if(0x4E==ptCallLevelCard->byCallCard[0] || 0x4F==ptCallLevelCard->byCallCard[0])//��С��������
					{
						lwlog.Format("��С��������");
						Debuglog(lwlog);
						if(m_bQiangNt)
						{
							m_tCurrentCall.byDeskStation = byDeskStation;
						}
						m_tCurrentCall.bySize = ptCallLevelCard->bySize;
						m_tCurrentCall.byLevelColor = 255;
						memcpy(m_tCurrentCall.byCallCard,ptCallLevelCard->byCallCard,sizeof(BYTE)*2);
					}
					else
					{
						lwlog.Format("���ӷ���");
						Debuglog(lwlog);
						if(m_bQiangNt)
						{
							m_tCurrentCall.byDeskStation = byDeskStation;
						}
						m_tCurrentCall.bySize = ptCallLevelCard->bySize;
						m_tCurrentCall.byLevelColor = m_Logic.GetCardHuaKind(ptCallLevelCard->byCallCard[0]);
						lwlog.Format("���ӷ���::Ŀǰ��ɫΪ=%d",m_tCurrentCall.byLevelColor);
						Debuglog(lwlog);
						memcpy(m_tCurrentCall.byCallCard,ptCallLevelCard->byCallCard,sizeof(BYTE)*2);
					}
					//����ҷ��������ļ�����Ϣ
					SendCallGameLevelCard(ptCallLevelCard);
				}
			}
			break;
		case 2:
			{
				if( ptCallLevelCard->byCallCard[1]!=ptCallLevelCard->byCallCard[0])
				{
					return;
				}
				//��ǰ�Ƿ�ΪС��
				bool bCurrentIsSmallJor = ( ((m_tCurrentCall.byCallCard[0] == m_tCurrentCall.byCallCard[1]) && m_tCurrentCall.byCallCard[0] == 0x4E) ? true:false );
				//��ǰ�Ƿ�Ϊ����
				bool bCurrentIsBigJor = ( ((m_tCurrentCall.byCallCard[0] == m_tCurrentCall.byCallCard[1]) && m_tCurrentCall.byCallCard[0] == 0x4F) ? true:false );

				if( !bCurrentIsSmallJor && !bCurrentIsBigJor )	//����Ȳ��ǶԴ�����Ҳ���Ƕ�С��
				{
					if (ptCallLevelCard->byCallCard[0] == 0x4E || ptCallLevelCard->byCallCard[0] == 0x4F)//������
					{
						lwlog.Format("����Ȳ��ǶԴ�����Ҳ���Ƕ�С�� ��С��������");
						Debuglog(lwlog);
						if(m_bQiangNt)
						{
							m_tCurrentCall.byDeskStation = byDeskStation;
						}
						m_tCurrentCall.bySize = ptCallLevelCard->bySize;
						m_tCurrentCall.byLevelColor = 255;
						memcpy(m_tCurrentCall.byCallCard,ptCallLevelCard->byCallCard,sizeof(BYTE)*2);

						//����ҷ��������ļ�����Ϣ
						SendCallGameLevelCard(ptCallLevelCard);
					}
					if(m_Logic.GetCardHuaKind(ptCallLevelCard->byCallCard[0])>m_Logic.GetCardHuaKind(m_tCurrentCall.byCallCard[0]))
					{
						if(m_bQiangNt)
						{
							m_tCurrentCall.byDeskStation = byDeskStation;
						}
						m_tCurrentCall.bySize = ptCallLevelCard->bySize;
						m_tCurrentCall.byLevelColor = m_Logic.GetCardHuaKind(ptCallLevelCard->byCallCard[0]);
						lwlog.Format("����Ȳ��ǶԴ�����Ҳ���Ƕ�С��::�����ɹ�::Ŀǰ��ɫΪ=%d",m_tCurrentCall.byLevelColor);
						Debuglog(lwlog);
						memcpy(m_tCurrentCall.byCallCard,ptCallLevelCard->byCallCard,sizeof(BYTE)*2);

						//����ҷ��������ļ�����Ϣ
						SendCallGameLevelCard(ptCallLevelCard);
					}
				}

				if( bCurrentIsSmallJor)	//�����ǰ�Ƕ�С���� �����Դ������ܷ���
				{
					if( ptCallLevelCard->byCallCard[0] == 0x4F)
					{
						lwlog.Format("�����ǰ�Ƕ�С���� �����Դ������ܷ���::�������ɹ�");
						Debuglog(lwlog);
						if(m_bQiangNt)
						{
							m_tCurrentCall.byDeskStation = byDeskStation;
						}
						m_tCurrentCall.bySize = ptCallLevelCard->bySize;
						m_tCurrentCall.byLevelColor = 255;
						memcpy(m_tCurrentCall.byCallCard,ptCallLevelCard->byCallCard,sizeof(BYTE)*2);
						
						//����ҷ��������ļ�����Ϣ
						SendCallGameLevelCard(ptCallLevelCard);
					}
				}
				//����������ܷ���
			}
			break;
		}
 	}
}

// ���������ļ�����Ϣ
void CServerGameDesk::SendCallGameLevelCard(TCallLevelCard *ptCallLevelCard)
{
	for(int i =0;i<PLAY_COUNT;++i)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,ptCallLevelCard,sizeof(TCallLevelCard),MDM_GM_GAME_NOTIFY,ASS_SHOWCALLLEVELCARD,0);
		}
	}
	SendWatchData(PLAY_COUNT,ptCallLevelCard,sizeof(TCallLevelCard),MDM_GM_GAME_NOTIFY,ASS_SHOWCALLLEVELCARD,0);
}


//��ȡ��̬����
BOOL CServerGameDesk::LoadIni()
{
	CString strPath = CINIFile::GetAppPath();	//����·��
	CINIFile f(strPath + SKIN_FOLDER + "_s.ini");
	
	CString key = TEXT("timeclock");
	m_iTimeToThink = f.GetKeyVal(key,"TimeToThink",30);
	if(m_iTimeToThink <=0)
		m_iTimeToThink = 30;
	m_iTimeToReady = f.GetKeyVal(key,"TimeToReady",30);
	if(m_iTimeToReady <=0)
		m_iTimeToReady = 15;
	m_iTimeToCallNT = f.GetKeyVal(key,"TimeToCallNT",30);
	if(m_iTimeToCallNT <=0)
		m_iTimeToCallNT = 15;
	m_iSendCardSpeed = f.GetKeyVal(key,"SendCardSpeed",500);
	m_iSendCardSpeed = 500;
	CString mykey = TEXT("superid");
	int count = f.GetKeyVal(mykey,"count",0);
	TCHAR szName[21] = {0};
	if(count <= 0 )					//��ʤ��
	{
		return FALSE;
	}
	for(int i = 0; i < count; i++)
	{
		wsprintf(szName,"superid_%d",i);
		m_vSuperUsers.push_back(f.GetKeyVal(mykey,szName,7758520));
	}
	return TRUE;
}

//����������ҵ��Ƹ�����
void CServerGameDesk::SendAllCardsToSuper()
{
	return;
	TOtherCards tOtherCards;
	int iCounter = 0;
	for (int i = 0;i<PLAY_COUNT;++i)
	{	
		if(m_bySuperDeskStation[i])
		{
			continue;
		}
		else
		{
			int isize = m_vPlayerCards[i].size();
			tOtherCards.tsendCards[iCounter].byCardCount =isize ;
			tOtherCards.tsendCards[iCounter].byDeskStation = i;
			BYTE byTemp[33] = {0};
			for (int j = 0;j<m_vPlayerCards[i].size();++j)
			{
				byTemp[j] = m_vPlayerCards[i][j];
			}
			memcpy(tOtherCards.tsendCards[iCounter].byCards,byTemp,(sizeof(BYTE)*(isize)));
			++iCounter;
		}
	}
	//�����˷���������
	for (int i = 0;i<PLAY_COUNT;++i)
	{
		if(m_bySuperDeskStation[i])
		{
			SendGameData(i,&tOtherCards,sizeof(TOtherCards),MDM_GM_GAME_NOTIFY,ASS_SENDOTHERCARD,0);
		}
	}
}
//��¼ÿ����ҵ�������
void CServerGameDesk::RecordTurnCardData(BYTE byDeskStation,BYTE bCardList[],int iCardCount)
{
	for(int i=0;i<30;i++)
	{
		if(m_tTurnCardData[i].iCardNums[byDeskStation]<=0)
		{
			m_tTurnCardData[i].iCardNums[byDeskStation]=iCardCount;
			memset(m_tTurnCardData[i].byCards[byDeskStation],0,sizeof(m_tTurnCardData[i].byCards[byDeskStation]));
			memcpy(m_tTurnCardData[i].byCards[byDeskStation],bCardList,sizeof(BYTE)*iCardCount);
			break;
		}
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
//��ӡ��־�ļ�-������
void CServerGameDesk::Debuglog(const char *p, ...)
{
	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() +"\\����log\\";
	SHCreateDirectoryEx(NULL, strPath, NULL);
	CTime time = CTime::GetCurrentTime();
	sprintf(szFilename, "%s%d-%d-%d.log",strPath ,time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	//����ʱ��
	char cTime[30];
	::memset(cTime,0,sizeof(cTime));
	sprintf(cTime,"[%d:%d:%d] ",time.GetHour(),time.GetMinute(),time.GetSecond());
	fprintf(fp,cTime);
	va_list arg;
	va_start( arg, p );
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}