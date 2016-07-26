#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"
///

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


void DebugPrintf(const char *p, ...)
{
	return ;
	char szFilename[256];
	sprintf( szFilename, "%dServer.txt", NAME_ID);
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

__int64 my_atoi(const char *str)
{
	 __int64 result = 0;
	 int signal = 1;
	 /* Ĭ��Ϊ���� */
	 if((*str>='0'&&*str<='9')||*str=='-'||*str=='+')
	 {
		 if(*str=='-'||*str=='+')
		 {
			 if(*str=='-')
			 signal = -1; /* ���븺�� */
		 str++;
		 }
	 }
	 else return 0;/* ��ʼת�� */
	 while(*str>='0'&&*str<='9')
		 result = result*10+(*str++ -'0');
	 return signal*result;
 }
///���ƣ�CServerGameDesk
///���������������캯��
///@param ALL_ARGEE ��Ϸ��ʼģʽ���������ͬ�⼴�ɿ�ʼ��
///@return 
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bGameStation=GS_WAIT_AGREE;  // ��ʼΪ�ȴ���ʼ״̬
	m_iCurrentMessage = ASS_WU_ZHUANG;//��ʼ��Ϊ��ׯ�ȴ�
	m_iIsMatch=0;										//�Ƿ�����	
	InitUserSeatID();
	m_iZhuangBaShu=0;
	m_byBaseOutCount = 0;					                    // ���Ƶ���Ŀ
	memset(m_wBaseOutCard, 0, sizeof(m_wBaseOutCard));		    // ����
	memset(m_byDeskCardCount, 0, sizeof(m_byDeskCardCount));	// �����˿˵���Ŀ
	memset(m_wDeskCard, 0, sizeof(m_wDeskCard));		        // ������˿�
	memset(m_byUserCardCount, 0, sizeof(m_byUserCardCount));	// �û������˿���Ŀ
	memset(m_wUserCard, 0, sizeof(m_wUserCard));		        // �û����ϵ��˿�

	m_bySendStartPos = 0x00;             // ��һ�����Ƶ����
	m_byFirstOutPos = 0x00;              // ��һ�������
	m_wMingCard = 0x00;                  // һ������
	m_bShuangKou = true;                 // �Ͼ��Ƿ�˫�۳ɹ� Ĭ���ǳɹ���
	m_byLeaveAgree = 0x00;				 // �뿪ͬ���־
	m_byUserAgreed = 0x00;
	m_byHaveThingPeople=0xFF;		     // ����Ҫ�����λ��

	m_byLastGameTimer=0x00;                // ���һ����Ϸ��ʱ��ID  (0x10 ~ 0x20)
	m_dwLastGameStartTime=0x00;            // ���һ����Ϸ��ʱ������ʱ��
	m_iLastTimerTime=0x00;                 // ���һ����Ϸ��ʱ��������ʱ��(��ʱ��)

	////////////////////////�ټ��ֳ�ʼ������lym//////////////////////////////////////////////////////////////////

	memset(m_iZhuangStation, -1, sizeof(m_iZhuangStation)); //ׯ���б��ܵ�ׯ��λ��
	memset(m_iZPaiXing, 0, sizeof(m_iZPaiXing));	 //0�ŵ�����1�����͡����ͣ�0������1������2�ͣ�3ͬ���
	memset(m_iXPaiXing, 0, sizeof(m_iXPaiXing));	//0�ŵ�����1�����͡����ͣ�0������1������2�ͣ�3ͬ���

	memset(m_iAIMoney, 0, sizeof(m_iAIMoney));	/**< �����˵���ע*/
	memset(m_iQuYuZhu, 0, sizeof(m_iQuYuZhu));	//����ÿ�������µ�ע��
	memset(m_iMaxZhu, 0, sizeof(m_iMaxZhu));	//����ÿ������������µ�ע��
	memset(m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���

	memset(m_iMax, 0, sizeof(m_iMax));			//����ÿ��������Ƶ������ע��
	memset(m_iPeiLv, 0, sizeof(m_iPeiLv));		//ÿ�����������

	memset(m_iQuYuJinBi, 0, sizeof(m_iQuYuJinBi));		//ÿ�����¸��ࣨ��6�ࣩ��ҵĸ���
	memset(m_iResultInfo, -1, sizeof(m_iResultInfo));	//���30�ѵĿ�����Ϣ
	memset(m_iXiaZhuType, 0, sizeof(m_iXiaZhuType));	//�����ҵ���ע����
	memset(m_UserCard, 0, sizeof(m_UserCard));		    // �û����ϵ��˿�
	memset(m_CardCount, 255, sizeof(m_CardCount));		//ׯ��˫��������
				
	m_iGameCount = 0;           //�Ѿ����еľ�����ֻ����30�֡�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		for(int j=0;j<11;j++)
		{
			m_iUserXiaZhuData[i][j]=0;
		}
	}
	m_iMaxZhuang = 30;			//ׯ��һ����������پ֣�Ĭ��30��

	m_iWinner = 0;				//Ӯ��1 ׯ��2�У�3��
	m_iKaiPai =0;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	m_bThreeCard=false;			//ׯ��������3����

	m_iCardCount=0;			//�Ѿ����Ƶ�������
	m_iZhongZhu=0;			//���ѵ�ǰ��ע��
	m_iZhuangFen=0;			//ׯ�ҵĵ÷�
	m_iXianFen=0;			//�мҵĵ÷�

	m_iBaSHu=0;				//���֣�30��һ�֣����еİ���
	m_iZhuangBaShu=0;		//ׯ�ҽ����˼���
	m_iZongTime=0;				//��ʱ��
    m_iSYTime=0;				//ʣ��ʱ��
	m_iXiaZhuTime = 0;			//��עʱ��			
	m_iKaiPaiTime = 0;			//����ʱ��
	m_iFreeTime = 0;			//����ʱ��

	m_iNowNtStation=-1;			//��ǰׯ��λ��
	m_iLastNtStation=-1;		//�ϸ�ׯ�ҵ�λ��
	m_bXiaZhuang=false;			//��ǰׯ��������ׯ

	m_iShangZhuangLimit=0;		//��ׯ��Ҫ�����ٽ��
	m_iNtWin =0;				//��ǰׯ��Ӯ�Ľ��
	m_bIsFirstLoadIni = true;	//��һ��ȥ�������ļ�

	////////////////////////////////////////////////////////////////////////////////////////////
	int i=0;
	for (i=0; i<PLAY_COUNT; ++i)
	{
		m_iUserFen[i] =0;          //������ұ��ֵĵ÷�
		m_iUserMoney[i] =0;          //������ұ���Ӧ�Ľ��
		m_bIsTrustee[i] = false;           // �й�
	}
	
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_wSuperCardList[i],0,sizeof(m_wSuperCardList[i]));
	}
	m_bySuperStation = -1;
#endif

	//���������ļ�
	LoadIni();

	m_bSuperSetting = FALSE;
	m_bSuperState = 0;

	m_iNowJuShu = 1;				//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
	m_bGamePlaying = false;			///lym2010-03-19��¼��Ϸ�Ƿ����ڽ�����
	m_bPrintEnding = false;                      /**< �Ƿ��ӡ��� */

	/// ��¼��ǰʱ��
	m_RecordTime = CTime::GetCurrentTime();

	m_iNoteMoney[0] = 100;					/**< �������Ͷ�Ӧ�Ľ����*/
	m_iNoteMoney[1] = 1000;					/**< �������Ͷ�Ӧ�Ľ����*/
	m_iNoteMoney[2] = 10000;					/**< �������Ͷ�Ӧ�Ľ����*/
	m_iNoteMoney[3] = 100000;					/**< �������Ͷ�Ӧ�Ľ����*/
	m_iNoteMoney[4] = 1000000;					/**< �������Ͷ�Ӧ�Ľ����*/

#ifdef SET72
	m_iNoteMoney[5] = 10000000;					/**< �������Ͷ�Ӧ�Ľ����*/
#else
	m_iNoteMoney[5] = 5000000;					/**< �������Ͷ�Ӧ�Ľ����*/
#endif

	m_nPowerMoney = 1;					    /**<�Ŵ���*/        	
	memset(m_bIsSuperStation, 0, sizeof(m_bIsSuperStation));/**< ��¼������ҵ�λ�á�*/
	m_iCtrTime = 0;			//������ǰ���벻����ע��ʱ��
	m_bCanNote = false;			//�Ƿ�����ע	
	m_bIsJieSuan = false;						/**<�Ƿ��Ѿ�����2010-05-21*/
	memset(m_byQuYuJinBi, 0, sizeof(m_byQuYuJinBi));/**< ���������Ҹ���*/
	memset(m_IsAIStation,0,sizeof(m_IsAIStation));///��¼�����˵�λ��
	///ׯ�ҳ�ˮ����
	m_byNtTax = 0;	
	///�����ע�ⶥ
	m_iMaxNote = 0;//Ĭ��Ϊ������

	m_bAIWinAndLostAutoCtrl = false;///�Ƿ�����������Ӯ�Զ�����
	m_iAIWantWinMoney = 0;		///������ҪӮ��Ǯ
	m_iAIHaveWinMoney = 0;		///�������Ѿ�Ӯ�˶���Ǯ
	m_iAIMaxWinInOne = 0;		///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�		
	m_iAICtrlCount = 0;			///��ǰ�������Ѿ��������Զ����Ƶİ���

	m_iApplyNtAINum = 0;			///��Ҫ��ׯ�Ļ��������������ƻ�������ׯ��
	m_iAINtZhuangCount =0;		///���������ڵİ��������ƻ�������ׯ��

	m_iRobotSZCount = 0;
	m_bIsVipDouble = FALSE;
	m_bDoubleWin = FALSE;

	m_iHaveGameCount = -1;

	//�������Ҹ�����ע������ע����Ӯ���, 0:Ϊû����ע, 1Ϊ��עͬʱӮ��, 2Ϊ��ע��������
	memset(m_iUserXiaZhuInfo, 0, sizeof(m_iUserXiaZhuInfo));
	memset(m_iResultInfoArea, 0, sizeof(m_iResultInfoArea)); 

	memset(m_iUserOldMoney, 0, sizeof(m_iUserOldMoney)); 	/* ��ҵĴ����� */
		
}
///���ƣ�~CServerGameDesk
///���������������캯��
///@param 
///@return 
CServerGameDesk::~CServerGameDesk(void)
{	
}

/// ԭ�����������λ��
void CServerGameDesk::InitUserSeatID(void)
{
	int i =0;
	for (i=0; i<PLAY_COUNT; ++i)
	{
		m_abySeatID[i] = i;
		m_bySequence[i] = PLAY_COUNT;  // Ĭ�϶������һ��
	}
}

///���ƣ�LoadIni
///���������ط�������ini�����ļ�
///@param 
///@return 
BOOL CServerGameDesk::LoadIni()
{	
	return TRUE;
}
///���ƣ�InitDeskGameStation
///��������ʼ�����Ӻ�����һ����������������ļ�
///@param 
///@return 
bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadExtIni();
	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
///���ƣ�LoadExtIni
///���������ط�������ini�����ļ�������ʱ�䣬����֡��
///@param 
///@return 
BOOL CServerGameDesk::LoadExtIni()
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("game");
	///��ȡ��עʱ��
	m_iXiaZhuTime = f.GetKeyVal(szSec,"xiazhutime",15);	
	if(m_iXiaZhuTime<10)
	{
		m_iXiaZhuTime=10;
	}
	///��ȡ����ʱ��
	m_iKaiPaiTime = f.GetKeyVal(szSec,"kaipaitime",15);	
	if(m_iKaiPaiTime<15)
	{
		m_iKaiPaiTime=15;
	}
	///��ȡ����ʱ��
	m_iFreeTime = f.GetKeyVal(szSec,"freetime",10);		
	if(m_iFreeTime<10)
	{
		m_iFreeTime=10;
	}
	// ����ʱ����(ms)
	m_GameBaseInfo.iSendCardInterval = f.GetKeyVal(szSec, _T("SendCardInterval"),(int)500);
	if (m_GameBaseInfo.iSendCardInterval < 50)
	{
		m_GameBaseInfo.iSendCardInterval = 50;
	}

	// ����˼��ʱ��(s)
	m_GameBaseInfo.iThinkTime = f.GetKeyVal(szSec, _T("ThinkTime"),(int)30);
	if (m_GameBaseInfo.iThinkTime < 5)
	{
		m_GameBaseInfo.iThinkTime = 5;
	}

	// �澯ʱ��(s)
	m_GameBaseInfo.iAlarmTime = f.GetKeyVal(szSec, _T("AlarmTime"), (int)5);
	if (m_GameBaseInfo.iAlarmTime < 5)
	{
		m_GameBaseInfo.iAlarmTime = 5;
	}

	// ��ʼʱ��(s)
	m_GameBaseInfo.iBeginTime = f.GetKeyVal(szSec, _T("BeginTime"), (int)30);
	if (m_GameBaseInfo.iBeginTime < 5)
	{
		m_GameBaseInfo.iBeginTime = 5;
	}
	
	// ����ʱ��(ms)
	m_GameBaseInfo.iSendCardTime = f.GetKeyVal(szSec, _T("SendCardTime"), (int)400);
	if (m_GameBaseInfo.iSendCardTime < 5)
	{
		m_GameBaseInfo.iSendCardTime = 5;
	}
	
	// ����֡��
	m_GameBaseInfo.iSendCardZhen = f.GetKeyVal(szSec, _T("SendCardZhen"), (int)10);
	if (m_GameBaseInfo.iSendCardZhen < 1)
	{
		m_GameBaseInfo.iSendCardZhen = 1;
	}

	// ����˳��
	m_GameBaseInfo.bySendCardOrder = (0==f.GetKeyVal(szSec, _T("SendCardOrder"),(int)0))? 0: 1;

///��ȡ��ׯ��Ҫ�����ٽ��
	m_iShangZhuangLimit = f.GetKeyVal(szSec,"ShangZhuangMoney",1000);
///ׯ��һ����������پ֣�Ĭ��30��
	m_iMaxZhuang = f.GetKeyVal(szSec,"maxzhuang",30);			
///����ׯ�������ע��
	m_iMax[GF_ZHUANG] = f.GetKeyVal(szSec,"max0",0);		
///����ׯ�����������ע��
	m_iMax[GF_Z_TIANWANG] = f.GetKeyVal(szSec,"max1",0);	
///����ׯ�����������ע��
	m_iMax[GF_Z_DUIZI] = f.GetKeyVal(szSec,"max2",0);		
///�������������ע��
	m_iMax[GF_XIAN] = f.GetKeyVal(szSec,"max3",0);		
///�����������������ע��
	m_iMax[GF_X_TIANWANG] = f.GetKeyVal(szSec,"max4",0);	
///�����ж����������ע��
	m_iMax[GF_X_DUIZI] = f.GetKeyVal(szSec,"max5",0);		
///���ƺ��������ע��
	m_iMax[GF_HE] = f.GetKeyVal(szSec,"max6",0);		
///����ͬ����������ע��
	m_iMax[GF_T_HE] = f.GetKeyVal(szSec,"max7",0);			

	///�Ƿ��ӡ���
	m_bPrintEnding = f.GetKeyVal(szSec, "PrintEnding", 0);
///������ǰ���벻����ע��ʱ��
	m_iCtrTime = f.GetKeyVal(szSec,"ctrtime",0);	
	///ׯ�ҳ�ˮ����
	m_byNtTax = f.GetKeyVal(szSec,"NtTax",0);	
	///��ǰ����������µ���ע
	m_iMaxNote = f.GetKeyVal(szSec,"MaxNote",-1);//Ĭ��Ϊ500��	

	///��������Ӯ�Զ�����
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",0);///�Ƿ�����������Ӯ�Զ�����
	m_iAIWantWinMoney = f.GetKeyVal(szSec,"AIWantWinMoney",0);			///������ҪӮ��Ǯ
	m_iAIMaxWinInOne = f.GetKeyVal(szSec,"AIMaxWinInOne",0);				///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	

	// ��ȡ��������ׯ�������
	m_iRobotSZCount = f.GetKeyVal(szSec, "RobotSZCount", 15);

	if (m_iRobotSZCount < 1)
	{
		m_iRobotSZCount = 15;
	}
	m_bIsVipDouble = f.GetKeyVal(szSec,"IsVipDouble",0);
	m_bDoubleWin = f.GetKeyVal(szSec,"DoubleWin",0);

	m_bStartGameWithOutNt = f.GetKeyVal(szSec,"StartGameWithOutNt",true);	/* ��ׯ�Ƿ�ʼ��Ϸ;true:��ʼ��Ϸ;false:����ʼ��Ϸ */

	m_bShowAllResultInfo = f.GetKeyVal(szSec , "ShowAllResultInfo" , 0) ; 

	m_bNoteFullOpenCard = f.GetKeyVal(szSec,"NoteFullOpenCard",true);

	m_iLevelBase = f.GetKeyVal(szSec , "LevelBase" , 12) ; //ƽ�ı���
	return TRUE;
}

///���ƣ�LoadExtIni
///���������ݷ���ID���������ļ�
///@param iRoomID �����
///@return 
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.INI"));
	TCHAR szSec[_MAX_PATH] = TEXT("game");
	_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,iRoomID);

	// ����ʱ����(ms)
	m_GameBaseInfo.iSendCardInterval = f.GetKeyVal(szSec, _T("SendCardInterval"),(int)m_GameBaseInfo.iSendCardInterval);
	if (m_GameBaseInfo.iSendCardInterval < 50)
	{
		m_GameBaseInfo.iSendCardInterval = 50;
	}

	// ����˼��ʱ��(s)
	m_GameBaseInfo.iThinkTime = f.GetKeyVal(szSec, _T("ThinkTime"),(int)m_GameBaseInfo.iThinkTime);
	if (m_GameBaseInfo.iThinkTime < 5)
	{
		m_GameBaseInfo.iThinkTime = 5;
	}

	// �澯ʱ��(s)
	m_GameBaseInfo.iAlarmTime = f.GetKeyVal(szSec, _T("AlarmTime"), (int)m_GameBaseInfo.iAlarmTime);
	if (m_GameBaseInfo.iAlarmTime < 5)
	{
		m_GameBaseInfo.iAlarmTime = 5;
	}

	// ��ʼʱ��(s)
	m_GameBaseInfo.iBeginTime = f.GetKeyVal(szSec, _T("BeginTime"), (int)m_GameBaseInfo.iBeginTime);
	if (m_GameBaseInfo.iBeginTime < 5)
	{
		m_GameBaseInfo.iBeginTime = 5;
	}
	
	// ����ʱ��(ms)
	m_GameBaseInfo.iSendCardTime = f.GetKeyVal(szSec, _T("SendCardTime"), (int)m_GameBaseInfo.iSendCardTime);
	if (m_GameBaseInfo.iSendCardTime < 5)
	{
		m_GameBaseInfo.iSendCardTime = 5;
	}
	
	// ����֡��
	m_GameBaseInfo.iSendCardZhen = f.GetKeyVal(szSec, _T("SendCardZhen"), (int)m_GameBaseInfo.iSendCardZhen);
	if (m_GameBaseInfo.iSendCardZhen < 1)
	{
		m_GameBaseInfo.iSendCardZhen = 1;
	}

	// ����˳��
	m_GameBaseInfo.bySendCardOrder = (0==f.GetKeyVal(szSec, _T("SendCardOrder"),m_GameBaseInfo.bySendCardOrder))? 0: 1;


///��ȡ��ׯ��Ҫ�����ٽ��
	m_iShangZhuangLimit = f.GetKeyVal(szSec,"ShangZhuangMoney",m_iShangZhuangLimit);

	//CString str=f.GetKeyVal(szSec,"ShangZhuangMoney","111111");
	//__int64 pp=my_atoi(str);
	//MsgPrint("zhangjing::pp=%I64d",pp);
///ׯ��һ����������پ֣�Ĭ��30��
	m_iMaxZhuang = f.GetKeyVal(szSec,"maxzhuang",m_iMaxZhuang);			
///����ׯ�������ע��
	m_iMax[GF_ZHUANG] = f.GetKeyVal(szSec,"max0",m_iMax[GF_ZHUANG]);		
///����ׯ�����������ע��
	m_iMax[GF_Z_TIANWANG] = f.GetKeyVal(szSec,"max1",m_iMax[GF_Z_TIANWANG]);	
///����ׯ�����������ע��
	m_iMax[GF_Z_DUIZI] = f.GetKeyVal(szSec,"max2",m_iMax[GF_Z_DUIZI]);		
///�������������ע��
	m_iMax[GF_XIAN] = f.GetKeyVal(szSec,"max3",m_iMax[GF_XIAN]);		
///�����������������ע��
	m_iMax[GF_X_TIANWANG] = f.GetKeyVal(szSec,"max4",m_iMax[GF_X_TIANWANG]);	
///�����ж����������ע��
	m_iMax[GF_X_DUIZI] = f.GetKeyVal(szSec,"max5",m_iMax[GF_X_DUIZI]);		
///���ƺ��������ע��
	m_iMax[GF_HE] = f.GetKeyVal(szSec,"max6",m_iMax[GF_HE]);		
///����ͬ����������ע��
	m_iMax[GF_T_HE] = f.GetKeyVal(szSec,"max7",m_iMax[GF_T_HE]);		
///��ȡ��עʱ��
	m_iXiaZhuTime = f.GetKeyVal(szSec,"xiazhutime",m_iXiaZhuTime);	
	if(m_iXiaZhuTime<10)
	{
		m_iXiaZhuTime=10;
	}
///��ȡ����ʱ��
	m_iKaiPaiTime = f.GetKeyVal(szSec,"kaipaitime",m_iKaiPaiTime);	
	if(m_iKaiPaiTime<15)
	{
		m_iKaiPaiTime=15;
	}
///��ȡ����ʱ��
	m_iFreeTime = f.GetKeyVal(szSec,"freetime",m_iFreeTime);	
	if(m_iFreeTime<10)
	{
		m_iFreeTime=10;
	}

	///�Ƿ��ӡ���
	m_bPrintEnding = f.GetKeyVal(szSec, "PrintEnding", m_bPrintEnding);
///������ǰ���벻����ע��ʱ��
	m_iCtrTime = f.GetKeyVal(szSec,"ctrtime",m_iCtrTime);	
	///ׯ�ҳ�ˮ����
	m_byNtTax = f.GetKeyVal(szSec,"NtTax",m_byNtTax);	
	///��ǰ����������µ���ע
	m_iMaxNote = f.GetKeyVal(szSec,"MaxNote",m_iMaxNote);//Ĭ��Ϊ500��	

	///��������Ӯ�Զ�����
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",m_bAIWinAndLostAutoCtrl);///�Ƿ�����������Ӯ�Զ�����
	m_iAIWantWinMoney = f.GetKeyVal(szSec,"AIWantWinMoney",m_iAIWantWinMoney);			///������ҪӮ��Ǯ
	m_iAIMaxWinInOne = f.GetKeyVal(szSec,"AIMaxWinInOne",m_iAIMaxWinInOne);				///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	

	// ��ȡ��������ׯ�������
	m_iRobotSZCount = f.GetKeyVal(szSec, "RobotSZCount", m_iRobotSZCount);
	if (m_iRobotSZCount <1)
	{
		m_iRobotSZCount = 15;
	}
	m_bIsVipDouble = f.GetKeyVal(szSec,"IsVipDouble",m_bIsVipDouble);
	m_bDoubleWin = f.GetKeyVal(szSec,"DoubleWin",m_bDoubleWin);

	m_bStartGameWithOutNt = f.GetKeyVal(szSec,"StartGameWithOutNt",m_bStartGameWithOutNt);	/* ��ׯ�Ƿ�ʼ��Ϸ;true:��ʼ��Ϸ;false:����ʼ��Ϸ */

	m_bNoteFullOpenCard = f.GetKeyVal(szSec,"NoteFullOpenCard",m_bNoteFullOpenCard);

	return TRUE;
}
//-------------------------------add-by-DWJ-20121108-----------------------------------------------------------------------------------------
//�������ļ��ж�ȡ�������Ѿ�Ӯ�˶���Ǯ����Ϊ��ÿ�̽����ʱ�򶼻Ὣ������ӮǮ����д�������ļ��С� 
//������ÿ�̿�ʼ��ʱ���ֶ�ȡ�£���Ϊ�������޸�����ô�ͻ�������Ǯ��.
void CServerGameDesk::GetAIContrlSetFromIni(int iRoomID)
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	CString szSec = TEXT("game");
	szSec.Format("%s_%d",SKIN_FOLDER,iRoomID);

	//szSec.Format("%s",m_GRM_Key);
	m_iAIWantWinMoneyA1	= f.GetKeyVal(szSec,"AIWantWinMoneyA1 ",100000);		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(szSec,"AIWantWinMoneyA2 ",1000000);		/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(szSec,"AIWantWinMoneyA3 ",10000000);		/**<������ӮǮ����3  */
	m_iAIWinLuckyAtA1	= f.GetKeyVal(szSec,"AIWinLuckyAtA1 ",90);				/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(szSec,"AIWinLuckyAtA2 ",70);				/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(szSec,"AIWinLuckyAtA3 ",50);				/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(szSec,"AIWinLuckyAtA4 ",30);				/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAddMoneyEveryDay	= f.GetKeyVal(szSec,"AddMoneyEveryDay ",0);				/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	m_iAIHaveWinMoney	= f.GetKeyVal(szSec,"ReSetAIHaveWinMoney ",0);			/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */

	CheckNewDayReSetIni(&f,&szSec);
}
//---------------------------add-by-DWJ-20121108---------------------------------------------------------
//����Ƿ����һ��,����һ���Ҫ�޸������ļ���A1~A3�����ֵ
void CServerGameDesk::CheckNewDayReSetIni(CINIFile *f, CString *key)
{	//ddwwjj
	CTime Cttm;
	CString sDay,sValue;
	Cttm=CTime::GetCurrentTime();
	sDay  = Cttm.Format(("%Y%m%d"));
	if (m_bIsFirstLoadIni)
	{//��һ�ζ�ȡ�����ļ�������ֻ������ȡһ��
		m_iRecFroDayTime  = StrToInt(sDay);
		m_bIsFirstLoadIni = false;
	}
	if (m_iRecFroDayTime < StrToInt(sDay))
	{//�����¼��ʱ��С�ڵ�ǰ�����ڣ���ʾ��ǰʱ�����24�㣬�����Ѿ���һ��,���µ�һ�졣
		m_iRecFroDayTime = StrToInt(sDay);
		if (0 !=m_iAddMoneyEveryDay)
		{//���ÿ�յ���Ǯ���Ĳ�Ϊ0�ſ�����A1��A2��A3����������ܣ�д�������ļ�����
			sValue.Format("%d",(m_iAIWantWinMoneyA1+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA1 ",sValue);		/**<������ӮǮ����1  */

			sValue.Format("%d",(m_iAIWantWinMoneyA2+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA2 ",sValue);		/**<������ӮǮ����2  */

			sValue.Format("%d",(m_iAIWantWinMoneyA3+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA3 ",sValue);		/**<������ӮǮ����3  */
		}
	}	
}
//-------------------------------------------------------------------------------------------------------------------------------------------
/// ������ҵ��߼�λ�û�ȡ��������λ��(������ƶ�λ��ʱ��Ҳ�һ���������߼�λ����)
BYTE CServerGameDesk::GetSeatID(BYTE byDeskStation)
{
	return m_abySeatID[byDeskStation%PLAY_COUNT];
}

/// ������ҵ�������λ�û�ȡ�߼�λ��(������ƶ�λ��ʱ��Ҳ�һ���������߼�λ����)
BYTE CServerGameDesk::GetDeskStation(BYTE bySeatID)
{
	bySeatID = bySeatID%PLAY_COUNT;
	BYTE i=0;
	BYTE byDeskStation = bySeatID;   // ��ʼ������߼�λ��
	for (i=0; i<PLAY_COUNT; ++i)
	{
		if (bySeatID == m_abySeatID[i])
		{
			byDeskStation = i;      // ���ҵ���ҵ��߼�λ��
			break;
		}
	}
	return byDeskStation;
}

/// ��õ�ǰ�������е���Ϸ��ʱ����ID
BYTE CServerGameDesk::GetGameTimerID()
{
	return m_byLastGameTimer;
}

///���ƣ�GetGameRemainTime
///��������õ�ǰ�������е���Ϸ��ʱ����ʣ��ʱ��
///@param 
///@return ��Ϸʣ��ʱ��
int CServerGameDesk::GetGameRemainTime()
{
	long lTime = m_iLastTimerTime - (GetTickCount() - m_dwLastGameStartTime);
	if (lTime < 0)
	{
		return -1;
	}
	return ((int)lTime);
}

///���ƣ�GameBegin
///��������Ϸ��ʼ
///@param 
///@return 
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if(!m_bGamePlaying)
	{
		return false;
	}
	m_bGamePlaying=!m_bGamePlaying;
	// ����ϵͳ��ʼ����Ϸ��ʼ״̬
	__super::GameBegin(bBeginFlag);
#ifdef AUTOLOADINI
	AutoLoadIni(m_pDataManage->m_InitData.uRoomID);
#endif

	CString strLog="";
	InitUserSeatID();
	
	m_bGameStation = GS_SEND_CARD;         // ��Ϸһ��ʼ�ͽ�����ע״̬��
	m_byLeaveAgree = USER_DISAGREE_LEAVE;  // ��ʼ������Ҷ���ͬ��
	m_byUserAgreed = USER_DISAGREE_LEAVE;  // δͬ���
	m_byLastGameTimer=0x00;                // ���һ����Ϸ��ʱ��ID  (0x10 ~ 0x20)
	m_dwLastGameStartTime=0x00;            // ���һ����Ϸ��ʱ������ʱ��
	m_iLastTimerTime=0x00;                 // ���һ����Ϸ��ʱ��������ʱ��(��ʱ��)
	m_iApplyNtAINum = 0;						///��Ҫ��ׯ�Ļ��������������ƻ�������ׯ��

////////////////////////�ټ��ֳ�ʼ������lym//////////////////////////////////////////////////////////////////
	ServerInit();//lym0514��ʼ������������
	//---------------------------ADD--BY---DWJDWJ------------------------------------
	//����ÿ�̸��ݷ���ID��ȡ�������ļ��е�ReSetAIHaveWinMoney ���� �趨�»�����Ӯ�˶���Ǯ
	GetAIContrlSetFromIni(m_pDataManage->m_InitData.uRoomID);


	m_iZongTime = m_iXiaZhuTime + m_iKaiPaiTime + m_iFreeTime + 4;
	m_iSYTime = m_iXiaZhuTime;
	m_bCanNote = true;			//�Ƿ�����ע

	m_iHaveGameCount++;	
    //���֣�30��һ�֣����еİ���
	if(m_iGameCount<MAXCOUNT)//�Ѿ����еľ�����ֻ����30�֡�
	{
		m_iGameCount++;
	}	


	
	if(m_iBaSHu==MAXCOUNT)
	{
		m_iBaSHu=0;			//ÿ72�Ѵ�ͷ��ʼ����
		m_iNowJuShu++;
	}

	m_iBaSHu++;  
	m_iZhuangBaShu++;		//ׯ�ҽ����˼���
	//��ȡ�������������ע
	GetQuYuCanNote();

	// ֪ͨ������Ҹ�����ͼ
	BEGIN_DATA BeginData;
	BeginData.m_iBaSHu = m_iBaSHu;  //��ǰ��Ϸ�İ���
	BeginData.m_iNowNtStation = m_iNowNtStation;

	if (m_iNowNtStation>=0 && m_iNowNtStation <PLAY_COUNT && NULL != m_pUserInfo[m_iNowNtStation])
	{
		BeginData.m_iNTdata[0] = m_iNowNtStation;
		BeginData.m_iNTdata[1] = m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;
		BeginData.m_iNTdata[2] = m_iNtWin;//ׯ�ҵĳɼ�
		BeginData.m_iNTdata[3] = m_iZhuangBaShu;
	}

	for(int i=0;i<8;i++)
	{
		BeginData.m_iMaxZhu[i] = m_iMaxZhu[i];
	}

	for(int i=0;i<=PLAY_COUNT;i++)//ׯ���б�
	{
		BeginData.m_iNTlist[i] = m_iZhuangStation[i];
	}

	for(int i=0;i<MAXCOUNT;i++)//���30�ֵ���Ϣ
	{
		for(int j=0;j<4;j++)
		{
			BeginData.m_iResultInfo[i][j] = m_iResultInfo[i][j];
		}
	}
	BeginData.m_iShangZhuangLimit = m_iShangZhuangLimit;//��ׯ����
	BeginData.m_iZhuangBaShu = m_iZhuangBaShu;          //��ǰׯ�ҽ��еİ���
	BeginData.m_iGameCount  = m_iGameCount;				//��ȡ����

	BeginData.m_iXiaZhuTime = m_iXiaZhuTime;		//��עʱ��
	BeginData.m_iKaiPaiTime = m_iKaiPaiTime;		//����ʱ��
	BeginData.m_iFreeTime = m_iFreeTime;			//����ʱ��
	BeginData.m_iNowJuShu = m_iNowJuShu;			//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�	
	BeginData.bHaveUserInList = IsHaveUserInList(); //�Ƿ��зǻ�������ׯ���б���
	// ��ʼ�����������
	m_byBaseOutCount = 0;					                    // ���Ƶ���Ŀ

	// add by wys ����ǰׯ�ҵ�ID����ƽ̨�����ׯ�Ҳ�������ׯ�ڼ��Ǯ������
	long iNTUserID = 0;
	if (NULL != m_pUserInfo[m_iNowNtStation])
	{
		iNTUserID = m_pUserInfo[m_iNowNtStation]->m_UserData.dwUserID;		
	}
	SetCurrentZhuangjia(iNTUserID);
	///����
#ifdef SET72 //72��һ��
	if(m_iBaSHu==1)
	{
		CTime time = CTime::GetCurrentTime();
		// �ж������Ƿ��Ѿ�����һ��(�жϵ�ǰʱ����֮ǰ��¼��ʱ�䲻ͬ���������һ��)
		if (time.GetDay() != m_RecordTime.GetDay()
			|| time.GetMonth() != m_RecordTime.GetMonth()
			|| time.GetYear() != m_RecordTime.GetYear())
		{
			///	���µ�ǰʱ��
			m_RecordTime = time;
			/// �µ�һ�죬������0
			m_iNowJuShu = 0;
		}
		m_ResultData.Init();
		for(int i=0;i<MAXCOUNT;i++)
		{
			SendCard();
			///��¼����
			SetResultData(i);			
			if(i%10==0)
			{
				m_iCardCount=0;      //�Ѿ����Ƶ�������
				Sleep(50);
			}
		}
		// ��ӡ���
		if (m_bPrintEnding)
		{
			PrintEnding();
		}
	}
	m_ResultData.m_iNowBaShu = m_iBaSHu;
#endif
	m_iCurrentMessage = ASS_XIA_ZHU;	
	//������ҵ���ʷ��ע������Ϣ
	int iUserXiaZhuInfo[MAXCOUNT];
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			m_iUserOldMoney[i] = 0;
			continue;	
		}				
		if(m_pUserInfo[i]->m_UserData.isVirtual != 0)//��������Ӯ���ƣ���ȡ���������λ��
		{
			m_IsAIStation[i] = true;
		}		
		BeginData.iMaxNote = CanDouble(i)?2*m_iMaxNote:m_iMaxNote;				//������ע�ⶥ
		
		SendGameData(i, &BeginData, sizeof(BEGIN_DATA), MDM_GM_GAME_NOTIFY, ASS_BJL_BEGIN, 0);//������ע��Ϣ
	}

	CString cs;
	cs.Format("bjlServer:sizeof(BEGIN_DATA)=%d",sizeof(BEGIN_DATA));
	OutputDebugString(cs);
	// ��֤�����û�
	AuthPermissions();		

	//��ȡ��ǰׯ���б���������������ׯ���ƣ�
	if(m_iZhuangStation[PLAY_COUNT]<=m_iRobotSZCount+1)
	{			
		srand((unsigned)time(NULL));
		bool addnt = false;
		if(m_iZhuangStation[PLAY_COUNT]<=1)
		{
			addnt = true;
		}
		else if(rand()%m_iZhuangStation[PLAY_COUNT]==0)
		{
			addnt = true;
		}
		if(addnt)
		{
			m_iApplyNtAINum = rand()%m_iRobotSZCount;///��Ҫ��ׯ�Ļ��������������ƻ�������ׯ��
			if(m_iApplyNtAINum>0)
			{
				//SetTimer(ID_TIMER_AI_APPLY_NT,(rand()%5+5)*1000);
			}			
		}
	}
	m_iTimeLabel=m_iXiaZhuTime;
	m_bGameStation=GS_SEND_CARD;//��ע״̬
	//������ע��ʱ��
	SetTimer(ID_TIMER_GAME_TIME, 1000); 
	return true;
}

///���ƣ�GameFinish
///��������Ϸ����
///@param byDeskStation ����λ�ã�bCloseFlag ������ʽ
///@return 
bool CServerGameDesk::GameFinish(BYTE byDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:///lym0512����������Ϸ�����
		{
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			for (int i=0; i<PLAY_COUNT; i++)
			{
				temp_cut[i] = false;
			}		
			break;
		}
	case GF_SAFE:
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			if(m_bGameStation>=GS_SEND_CARD && m_bGameStation<=GS_PLAY_GAME)
			{
				//������ǿ������ʾǿ����Ϣ��add by wlr 20090807
				if (bCloseFlag == GFF_FORCE_FINISH)
				{
					int i = 0;
					// ���ǿ��
					GAME_FORCE_QUIT ForceQuit;
					ForceQuit.byDeskStation = byDeskStation;
					for (i=0; i<PLAY_COUNT; ++i)
					{
						if (NULL == m_pUserInfo[i])
						{
							continue;
						}
						SendGameData(i,&ForceQuit,sizeof(ForceQuit),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
					}

					bool bIsPlayerBetted = false;
					if (m_iUserXiaZhuData[byDeskStation][8] != 0)
					{
						bIsPlayerBetted = true;
					}
					if ((byDeskStation != m_iNowNtStation)&& (!bIsPlayerBetted) && byDeskStation!=255)
					{						
						MakeUserOffLine(byDeskStation);
						memset(m_iUserXiaZhuInfo[byDeskStation],0,sizeof(m_iUserXiaZhuInfo[byDeskStation]));
					}
				}
				return true;
			}
			m_bGameStation = GS_WAIT_AGREE;
			int i = 0;
			// ���ǿ��
			GAME_FORCE_QUIT CutEnd;
			CutEnd.byDeskStation = byDeskStation;
			if (GF_SAFE == bCloseFlag)
			{
				for (i=0; i<PLAY_COUNT; ++i)
				{
					if (NULL == m_pUserInfo[i])
					{
						continue;
					}
					SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SAFE_END,0);
				}
			}
			else
			{				
				bCloseFlag = GFF_FORCE_FINISH;
				bool bIsPlayerBetted = false;
				if (m_iUserXiaZhuData[byDeskStation][8] != 0)
				{
					bIsPlayerBetted = true;
				}
				if ((byDeskStation != m_iNowNtStation)&& (!bIsPlayerBetted) && byDeskStation!=255)
				{
					MakeUserOffLine(byDeskStation);
				}
			}
			return true;
			break;
		}
	default:
		break;
	}

	ASSERT(CHECK_DEBUG);
	return __super::GameFinish(byDeskStation,bCloseFlag);
}


/**
* �����Ϸ���ݰ�������
*
* @param [in] bDeskStation ���λ��
*
* @param [in] pNetHead ��Ϣͷ
*
* @param [in] pData ��Ϣ��
*
* @param [in] uSize ��Ϣ���Ĵ�С
*
* @param [in] uSoketID Socket ID
*
* @param [in] bWatchUser �Թ�
*
* @return ���ز�������
*/
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:	 //ǿ���˳�
		{
			//UserFourceQuit(bDeskStation);
			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

///���ƣ�HandleNotifyMessage
///��������Ϸ���ݰ������� 
///@param byDeskStation λ�� pNetHead ��Ϣͷ pData ��Ϣ�� uSize ��Ϣ���Ĵ�С uSocketID Socket ID  bWatchUser �Թ�
///@return 
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_SUPER_SETTING:  // ��Ӯ����
		{
			if (uSize != sizeof(SUPERSETTING))
			{
				return true;
			}

			SUPERSETTING * pSuperStting = (SUPERSETTING *)pData;
			OnSuperSetting(bDeskStation, pSuperStting->bState);

			return true;
		}
		case ASS_SUPER_AI_INFO://////��������Ӯ���� �������ͻ��˿������û����˿��Ʋ���
		{
			if (sizeof(AICTRLINFO) != uSize)
			{
				return false;
			}			
			SetAICtrl(bDeskStation,pData,uSize);

			return true;
		}
	case ASS_SUPER_SET_CARD:
		{
			if (bWatchUser) 
			{
				return false;
			}
#ifdef SUPER_VERSION

#endif
			return true;
		}
	case ASS_GM_AGREE_GAME:		// �û�ͬ����Ϸ
		{
			if (bWatchUser) 
			{
				return false;
			}
			break;
		}
	case ASS_TRUSTEE:			// ����й�
		{
			if (bWatchUser || (sizeof(TRUSTEE_CARD) != uSize))
			{
				return false;
			}
			TRUSTEE_CARD *psTrusteeCard = (TRUSTEE_CARD *)pData;
			psTrusteeCard->byDeskStation = bDeskStation;           // �Է���������Ϊ׼
			OnChangeTrustee(psTrusteeCard->byDeskStation);
			return true;
		}
	case ASS_HAVE_THING:		// ����Ҫ����Ϣ
		{
			if (bWatchUser || (sizeof(S_HAVE_THING) != uSize))
			{
				return false;
			}
			S_HAVE_THING *psHaveThing = (S_HAVE_THING *)pData;
			psHaveThing->byDeskStation = bDeskStation;
			return OnUserHaveThing(*psHaveThing);
		}
	case ASS_USER_AGREE_LEAVE:  // �Ƿ�ͬ������뿪
		{
			if (bWatchUser || (sizeof(S_AGREE_USER_LEAVE) != uSize))
			{
				return false;
			}
			S_AGREE_USER_LEAVE *psAgreeUserLeave = (S_AGREE_USER_LEAVE *)pData;
			psAgreeUserLeave->byDeskStation = bDeskStation;
			return OnAgreeUserLeave(*psAgreeUserLeave);
		}
	case ASS_XIA_ZHU:           // ��ע��Ϣ
		{
			if (sizeof(XIA_ZHU) != uSize)
			{
				return false;
			}
			if (GS_SEND_CARD!=m_bGameStation || NULL==m_pUserInfo[m_iNowNtStation] || !m_bCanNote)
			{
				return true;
			}
			HanleXiaZhu(bDeskStation,pData, uSize);
			return true;
		}
	case ASS_KAI_PAI:			// ������Ϣ
		{
			return true;
		}
	case ASS_DENG_DAI:          // �ȴ���Ϣ
		{
			return true;
		}
	case ASS_SHANG_ZHUANG:      // ��ׯ��Ϣ
		{
			if (sizeof(SHANG_ZHUANG) != uSize)
			{
				return false;
			}			
			SHANG_ZHUANG *pdata = (SHANG_ZHUANG *)pData;			
			if(pdata->shang && m_pUserInfo[bDeskStation] != NULL)//��ׯ����
			{	
				if (m_pUserInfo[bDeskStation]->m_UserData.isVirtual>0)
				{					
					if (CountRobortNum()>m_iRobotSZCount)
					{
						return true;
					}
				}
				
				if(m_pUserInfo[bDeskStation]->m_UserData.i64Money>= m_iShangZhuangLimit && m_pUserInfo[bDeskStation]->m_UserData.i64Money<1500000000000000000)
				{
					HanleSzhuang(bDeskStation,pData, uSize);
				}
				else 
				{
					SHANG_ZHUANG lost;
					lost.shang = pdata->shang;
					lost.success= 1;
					lost.m_iNowNtStation = m_iNowNtStation;
					if(	m_bGameStation == GS_SEND_CARD)
					{
						lost.bXiaZhu = true;   //�Ƿ���ע״̬
					}
					for(int i=0;i<=PLAY_COUNT;i++)
					{
						lost.zhuangstation[i] = m_iZhuangStation[i];
					}
					if (NULL !=m_pUserInfo[bDeskStation])
					{						
						if (bDeskStation == pdata->station)
						{
							SendGameData(bDeskStation,&lost,sizeof(lost),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//������ׯʧ����Ϣ
						}	
					}					
				}
			}
			else
			{
				HanleXzhuang(bDeskStation,pData, uSize);
			}
			return true;
		}
	case ASS_APPLY_GAME_DATA:///lym2010-03-18�յ��������������Ϣ
		{
			OnGetGameData(bDeskStation,pData, uSize);
			return true;
		}
	case ASS_BJL_TEST:          // �յ�������Ϣ
		{
			if (sizeof(BJL_TEST) != uSize)
			{
				return false;
			}
			return true;
		}
	default:
		break;
	}
	ASSERT(CHECK_DEBUG);
	return __super::HandleNotifyMessage(bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser);
}
///���ƣ�IsPlayGame
///�������ж��Ƿ�������Ϸ
///@param byDeskStation λ��
///@return true ������Ϸ��false ������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if((m_bGameStation >= GS_SEND_CARD)&& (m_bGameStation < GS_WAIT_NEXT))
	{
		return true;
	}
	else
	{
		return false;
	}
}
///���ƣ�UserLeftDesk
///�������û��뿪��Ϸ�� 
///@param byDeskStation λ�� pUserInfo ����
///@return 
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{

	if(m_iNowNtStation==bDeskStation)//ׯ���뿪��
	{
		m_iNowNtStation=-1;//���õ�ǰׯ��Ϊ-1	
		m_iNtWin=0;	//��ǰׯ��Ӯ�Ľ��
		m_iZhuangBaShu=0;//ׯ�ҽ����˼���
		m_bXiaZhuang=false;
		if(NULL==m_pUserInfo[m_iNowNtStation])//��ǰׯ�Ҳ�����
		{
			//������ׯ�б�
			MoveData(0);
			GetNextNT();
			if(m_pUserInfo[m_iNowNtStation])
			{
				//���ͻ�ׯ��Ϣ
				for(int i = 0; i < PLAY_COUNT; ++i)
				{
					if(NULL != m_pUserInfo[i])
					{
						SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_CHANGE_NT,0);
					}
				}
			}
		}
		//������ׯ�ȴ���Ϣ			
		DENG_DAI Wait;
		for(int i=0;i<=PLAY_COUNT;i++)
		{
			Wait.zhuangstation[i] = m_iZhuangStation[i];
		}
		Wait.m_iNtWin = 0;					//��ǰׯ��Ӯ�Ľ��
		Wait.m_iZhuangBaShu = 0;			//ׯ�ҽ����˼���
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL == m_pUserInfo[i])
				continue;
			SendGameData(i,&Wait,sizeof(Wait),MDM_GM_GAME_NOTIFY, ASS_WU_ZHUANG,0);////������ׯ�ȴ���Ϣ
		}
	}
	else//�˳�����������ׯ�б����������ׯ�б�λ��
	{
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_iZhuangStation[i] == bDeskStation)
			{
				m_iZhuangStation[i]=-1;
				MoveData(0);
				break;
			}
		}
	}
	return __super::UserLeftDesk(bDeskStation, pUserInfo);
}
///�û������뿪
bool  CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	return __super::UserNetCut(bDeskStation, pLostUserInfo);
}
///���ƣ�UserSitDesk
///������������� 
///@param pUserSit λ������ pUserInfo �������
///@return 
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	return __super::UserSitDesk(pUserSit, pUserInfo);
}

///���ƣ�OnWatchSetChange
///�������Թ۲����仯 
///@param byDeskStation λ�� dwTargetUserID ��ɫid bEnableWatch �Ƿ������Թ�
///@return 
bool CServerGameDesk::OnWatchSetChange(BYTE byDeskStation, long int dwTargetUserID, bool bEnableWatch)
{
	S_WATCH_SET_CHANGE sWatchSetChange;
	sWatchSetChange.byDeskStation = byDeskStation;
	sWatchSetChange.byCardCount = m_byUserCardCount[byDeskStation];
	// ���������֪ͨ��ʾ��, ����������Թۣ�֪ͨ�����Թ���ҽ������
	if (IsEnableWatch(byDeskStation))
	{
		memcpy(sWatchSetChange.wCardList, m_wUserCard[byDeskStation], m_byUserCardCount[byDeskStation]*sizeof(WORD));
	}	
	return __super::OnWatchSetChange(byDeskStation, dwTargetUserID,bEnableWatch);
}
///���ƣ�OnGetGameStation
///��������ȡ��Ϸ״̬ 
///@param byDeskStation λ�� uSocketID socket id  bWatchUser �Ƿ������Թ�
///@return 
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	if(m_pUserInfo[bDeskStation] != NULL)
	{
		m_iUserOldMoney[bDeskStation] = m_pUserInfo[bDeskStation]->m_UserData.i64Money;/* ��ҵĴ����� */
	}
	
	// ��֤�����û�Ȩ��
	AuthPermissions();
	if(m_pUserInfo[bDeskStation] != NULL && m_pUserInfo[bDeskStation]->m_UserData.isVirtual != 0)//��������Ӯ���ƣ���ȡ���������λ��
	{
		m_IsAIStation[bDeskStation] = true;
	}
	//��������Ϸ��ʼ���������״̬û�����õ����������Ϸ�д�Ǯ�����ڴ���������
	if (m_bGameStation ==GS_PLAY_GAME ||m_bGameStation == GS_SEND_CARD)
	{
		m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_PLAY_GAME;//���������Ϊͬ��״̬
	}
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:			//�ȴ���һ����Ϸ��ʼ
		{
			//��������
			// ���͵��ͻ���
			CHONG_LIAN sWaiting;
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				sWaiting.zhuangstation[i] = m_iZhuangStation[i];
			}
			for(int i=0;i<MAXCOUNT;i++)
			{
				for(int j=0;j<4;j++)
				{
					sWaiting.m_iResultInfo[i][j] = m_iResultInfo[i][j];
				}
				sWaiting.m_iUserXiaZhuInfo[i] = m_iUserXiaZhuInfo[bDeskStation][i];
				sWaiting.m_iResultInfoArea[i] = m_iResultInfoArea[i];
			}
			sWaiting.m_iBaSHu = m_iBaSHu;
			sWaiting.m_iGameCount = m_iGameCount;
			sWaiting.m_iZhuangBaShu = m_iZhuangBaShu;
			sWaiting.m_iNowNtStation = m_iNowNtStation;
			sWaiting.m_iShangZhuangLimit = m_iShangZhuangLimit;

			sWaiting.m_iXiaZhuTime = m_iXiaZhuTime;		//��עʱ��
			sWaiting.m_iKaiPaiTime = m_iKaiPaiTime;		//����ʱ��
			sWaiting.m_iFreeTime = m_iFreeTime;			//����ʱ��
			sWaiting.m_iSendInterval = m_GameBaseInfo.iSendCardInterval; // ���Ƽ��ʱ��
			sWaiting.m_iSendCardTime = m_GameBaseInfo.iSendCardTime; //����ʱ��(ms)
			sWaiting.m_iSendCardZhen = m_GameBaseInfo.iSendCardZhen; //����֡��
			sWaiting.m_iNowJuShu = m_iNowJuShu;			//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
			sWaiting.iMaxNote = CanDouble(bDeskStation)?2*m_iMaxNote:m_iMaxNote;
			sWaiting.m_iBaseMaxNote = m_iMaxNote;		//��ͨ��������ע
			sWaiting.bIsVipDouble = m_bIsVipDouble;			
		
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&sWaiting,sizeof(sWaiting));
			//�����Щ������ׯ�ȴ�ʱ�䣬��ǿ��һ����������ׯ			
			for (int i=0;i<PLAY_COUNT; i++)
			{
				if (NULL == m_pUserInfo[i])			
					continue;				
				if(m_pUserInfo[i]->m_UserData.isVirtual != 0)//��������Ӯ���ƣ���ȡ���������λ��
				{
					m_IsAIStation[i] = true;					
				}
			}
			return true;
		}
	case GS_SEND_CARD:		//��ע״̬
		{
			CHONG_LIAN sSendCard;
			sSendCard.iMaxNote = CanDouble(bDeskStation)?2*m_iMaxNote:m_iMaxNote;
			sSendCard.m_iBaseMaxNote = m_iMaxNote;		//��ͨ��������ע
			sSendCard.bIsVipDouble = m_bIsVipDouble;
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&sSendCard,sizeof(sSendCard));
			return true;
		}
	case GS_PLAY_GAME:	//����״̬
		{
			CHONG_LIAN sPlaying;			
			sPlaying.iMaxNote = CanDouble(bDeskStation)?2*m_iMaxNote:m_iMaxNote;
			sPlaying.m_iBaseMaxNote = m_iMaxNote;		//��ͨ��������ע
			sPlaying.bIsVipDouble = m_bIsVipDouble;
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&sPlaying,sizeof(sPlaying));
			return true;
		}
	default:
		break;
	}

	ASSERT(CHECK_DEBUG);
	return true;
}

///���ƣ�ReSetGameState
///������������Ϸ״̬ 
///@param bLastStation ״̬
///@return 
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_wSuperCardList[i],0,sizeof(m_wSuperCardList[i]));
	}
	m_bySuperStation = -1;
#endif
	ASSERT(CHECK_DEBUG);
	return true;
}

///���ƣ�OnChangeTrustee
///�������й�״̬�ı���Ϣ 
///@param byDeskStation λ��
///@return 
void CServerGameDesk::OnChangeTrustee(BYTE byDeskStation)
{
	m_bIsTrustee[byDeskStation] = !m_bIsTrustee[byDeskStation];
	// �����й���Ϣ
	TRUSTEE_CARD S_TrusteeCard;
	S_TrusteeCard.byDeskStation = byDeskStation;
	S_TrusteeCard.bIsTrustee = m_bIsTrustee[byDeskStation];

	for(int i=0; i<PLAY_COUNT; ++i)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&S_TrusteeCard,sizeof(S_TrusteeCard),MDM_GM_GAME_NOTIFY,ASS_TRUSTEE,0);
	}	
	return ;
}


///���ƣ�OnUserHaveThing
///�������û������뿪 
///@param sHaveThing ���ݰ�
///@return 
bool CServerGameDesk::OnUserHaveThing(S_HAVE_THING &sHaveThing)
{
	// ������Ϸ��ֱ���˳�
	if (m_bGameStation<GS_SEND_CARD)
	{
		return true;
	}

	// �����һ����ҵ������뿪��ϢΪ�����꣬����Ӧ��һ��
	if (USER_DISAGREE_LEAVE != m_byUserAgreed)
	{
		return true;
	}

	// ���������뿪���
	m_byHaveThingPeople=sHaveThing.byDeskStation;		     // ����Ҫ�����λ��
	m_byLeaveAgree = ~USER_DISAGREE_LEAVE;
	m_byLeaveAgree = m_byLeaveAgree<<PLAY_COUNT;             // ������Ҷ���ͬ��

	// �����뿪���λ������Ϊͬ��
	m_byLeaveAgree |= (USER_AGREE_LEAVE<<sHaveThing.byDeskStation);	  // �뿪ͬ���־
	m_byUserAgreed = m_byLeaveAgree;                                  // ͬ��������

	// ת����Ϣ
	for(BYTE i=0; i<PLAY_COUNT; ++i)
	{
		// ����λ��Ϊͬ��
		if (NULL == m_pUserInfo[i])
		{
			m_byLeaveAgree |= (USER_AGREE_LEAVE<<i);		// �뿪ͬ���־
			m_byUserAgreed = m_byLeaveAgree;			
			continue;
		}

		if (i != sHaveThing.byDeskStation)
		{
			SendGameData(i,&sHaveThing,sizeof(S_HAVE_THING), MDM_GM_GAME_NOTIFY, ASS_HAVE_THING, 0);
		}		
	}

	return true;
}

///���ƣ�OnAgreeUserLeave
///����������Ƿ�ͬ���뿪���� 
///@param sAgreeUserLeave ���ݰ�
///@return 
bool CServerGameDesk::OnAgreeUserLeave(S_AGREE_USER_LEAVE &sAgreeUserLeave)
{
	BYTE byLeaveTemp = 0x00;
	if (USER_AGREE_LEAVE == sAgreeUserLeave.byAgreeLeave)
	{
		byLeaveTemp = USER_AGREE_LEAVE;
		m_byLeaveAgree|= (byLeaveTemp<<sAgreeUserLeave.byDeskStation);
	}
	else
	{
		byLeaveTemp = ~USER_AGREE_LEAVE;
		m_byLeaveAgree &=(byLeaveTemp<<sAgreeUserLeave.byDeskStation);
	}

	// �����ͬ����ˣ��Ͳ���ִ��
	byLeaveTemp = USER_AGREE_LEAVE;
	if (byLeaveTemp == (byLeaveTemp & (m_byUserAgreed>>sAgreeUserLeave.byDeskStation)))
	{
		return true;
	}

	// ��ʾ������Ѿ��ظ�������Ϣ
	m_byUserAgreed |= (byLeaveTemp<<sAgreeUserLeave.byDeskStation);

	// �ж��Ƿ����е��˶�ͬ����
	if (0xFF != m_byLeaveAgree)
	{
		// ת�������Ϣ
		BYTE i=0;
		for(i=0; i<PLAY_COUNT; ++i)
		{
			if (NULL == m_pUserInfo[i])
			{
				continue;
			}
			if (i == sAgreeUserLeave.byDeskStation)
			{
				continue;
			}
			SendGameData(i,&sAgreeUserLeave,sizeof(S_AGREE_USER_LEAVE),MDM_GM_GAME_NOTIFY,ASS_USER_AGREE_LEAVE,0);
		}

		// ���������Ҷ�ͬ�����������Ϣ֪ͨ�����ٴ������뿪
		if (0xFF == m_byUserAgreed)
		{
			m_byUserAgreed = USER_DISAGREE_LEAVE;
			for(i=0; i<PLAY_COUNT; ++i)
			{
				if (NULL == m_pUserInfo[i])
				{
					continue;
				}
				// ֪ͨ
				SendGameData(i,&sAgreeUserLeave,sizeof(S_AGREE_USER_LEAVE),MDM_GM_GAME_NOTIFY,ASS_ALL_USER_AGREEED,0);
			}			
		}
	}
	else
	{
		// ������ͬ��������Ϸ
		GameFinish(m_byHaveThingPeople,GF_SAFE);
		sAgreeUserLeave.byDeskStation=m_byHaveThingPeople;
		sAgreeUserLeave.byAgreeLeave=USER_AGREE_LEAVE;
		SendGameData(m_byHaveThingPeople,&sAgreeUserLeave,sizeof(S_AGREE_USER_LEAVE),MDM_GM_GAME_NOTIFY,ASS_USER_AGREE_LEAVE,0);
	}	
	return true;
}

/// ���ö�ʱ��
bool CServerGameDesk::SetGameTimer(BYTE byTimerID, int uElapse)
{
	// ��Ч����Ϸ��ʱ��
	if ((byTimerID < ID_GAME_TIMER_FALG_START) || (byTimerID > ID_GAME_TIMER_FALG_END))
	{
		return false;
	}

	if (0x00 != m_byLastGameTimer)
	{
		KillGameTimer(m_byLastGameTimer);		
	}
	m_byLastGameTimer = byTimerID;                   // ���һ����Ϸ��ʱ��ID
	m_dwLastGameStartTime=GetTickCount();            // ���һ����Ϸ��ʱ������ʱ��
	m_iLastTimerTime=uElapse;                        // ���һ����Ϸ��ʱ��������ʱ��(��ʱ��)
	return SetTimer(byTimerID, uElapse);
}

/// ɾ����ʱ��
bool CServerGameDesk::KillGameTimer(BYTE byTimerID)
{
	if ((byTimerID >= ID_GAME_TIMER_FALG_START) && (byTimerID <= ID_GAME_TIMER_FALG_END))
	{
		m_byLastGameTimer = 0x00;              // ���һ����Ϸ��ʱ��ID
		m_dwLastGameStartTime=0x00;            // ���һ����Ϸ��ʱ������ʱ��
		m_iLastTimerTime=0x00;                 // ���һ����Ϸ��ʱ��������ʱ��(��ʱ��)
	}	
	return KillTimer(byTimerID);
}

/// ������ж�ʱ��
void  CServerGameDesk::KillAllTimer(void)
{
	;
}

///���ƣ�OnTimer
///��������ʱ����Ϣ 
///@param uTimerID ��ʱ��id
///@return 
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch (uTimerID)
	{
	case ID_TIMER_GAME_TIME://��עʱ��
		{
			m_iTimeLabel--;
			m_iCurrentMessage = ASS_XIA_ZHU;
			if(m_bNoteFullOpenCard && JudgeXiaZhuIsFull())
			{
				SendXiaZhuFullNews();
			}
			if(0==m_iTimeLabel)
			{
				m_bCanNote = false;//�Ƿ�����ע	
				KillTimer(ID_TIMER_GAME_TIME);
#ifdef SET72 //72��һ��
				//�ӷ��õ����л�ȡ����
				GetResultData(m_iBaSHu-1);
#else
				//����lym0512
				SendCard();	
#endif
				//ׯ�п���
				ConTrolZX();
				m_iTimeLabel=m_iKaiPaiTime;
				m_bGameStation=GS_PLAY_GAME;//����״̬
				// ���ÿ���ʱ��
				SetTimer(ID_TIMER_GAME_SENDCARD, 1000);
				//���Ϳ�����Ϣ
				OnSendOpenCard();
			}
			return true;
		}
	case ID_TIMER_GAME_SENDCARD:		// ����ʱ��
		{
			m_iTimeLabel--;
			if(5==m_iTimeLabel)//��ǰ���뷢�ͽ�����Ϣ
			{
				//���ͽ�����Ϣ
				OnSendEnd();
			}
			if(0==m_iTimeLabel)//������н׶�
			{
				KillTimer(ID_TIMER_GAME_SENDCARD);
				m_iTimeLabel=m_iFreeTime;
				m_bGameStation = GS_WAIT_NEXT;//���н׶�
				// ���ÿ���ʱ��
				SetTimer(ID_TIMER_GAME_FREE, 1000);
				//���Ϳ�����Ϣ
				OnSenFree();
			}
			return true;
		}
	
	case ID_TIMER_GAME_FREE:		// ����ʱ��
		{
			m_iTimeLabel--;
			if(0==m_iTimeLabel)//��Ϸ���¿�ʼ
			{
				KillTimer(ID_TIMER_GAME_FREE);
				//��Ϸ���¿�ʼ
				RestartGame();
			}
			return true;
		}
	}
	return OnTimer(uTimerID);
}
//���Ϳ�����Ϣ
void CServerGameDesk::OnSendOpenCard()
{
	m_iCurrentMessage = ASS_KAI_PAI;
	srand((unsigned)time(NULL));
	//���Ϳ�����Ϣ			
	m_bGameStation = GS_PLAY_GAME;         // ��Ϸһ����״̬��
	//�������
	if(m_iNowNtStation>=0)///û��ׯ�ҾͲ����
	{
		CountFen();
	}	
	if (m_iHaveGameCount >= MAXCOUNT)
	{
		m_iHaveGameCount = MAXCOUNT-1;
		MoveData(1);
	}

	RecordData();	//��¼����

	KAI_PAI KaiPaiData;
	KaiPaiData.m_iXianFen = m_iXianFen;				//�мҵ÷�
	KaiPaiData.m_iZhuangFen = m_iZhuangFen;         //ׯ�ҵ÷�

	KaiPaiData.m_CardCount[0] = m_CardCount[0]; //����
	KaiPaiData.m_CardCount[1] = m_CardCount[1];
	KaiPaiData.m_iKaiPai = m_iKaiPai;   //��������
	KaiPaiData.m_iWinner = m_iWinner;   //Ӯ��

	for(int i=0;i<5;i++)//ׯ������
	{
		KaiPaiData.m_iZPaiXing[i] =  m_iZPaiXing[i];
		KaiPaiData.m_iXPaiXing[i] =  m_iXPaiXing[i];
	}

	//����ׯ�պ���Ӯ��ʷ��¼ 1ׯ�� 2�գ� 3��
	if (m_iZPaiXing[1] == m_iXPaiXing[1])
	{
		m_iResultInfoArea[m_iHaveGameCount] = 3;				
	}	
	else if (m_iZPaiXing[1] > m_iXPaiXing[1])
	{
		m_iResultInfoArea[m_iHaveGameCount] = 1;
	}
	else
	{
		m_iResultInfoArea[m_iHaveGameCount] = 2;
	}		

	//��ȡӮ������
	for(int i=0;i<8;i++)
	{
		KaiPaiData.m_iWinQuYu[i] = m_iWinQuYu[i];			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	}

	KaiPaiData.pai[0][0] = m_UserCard[0][0];
	KaiPaiData.pai[0][1] = m_UserCard[0][1];
	KaiPaiData.pai[0][2] = m_UserCard[0][2];

	KaiPaiData.pai[1][0] = m_UserCard[1][0];
	KaiPaiData.pai[1][1] = m_UserCard[1][1];
	KaiPaiData.pai[1][2] = m_UserCard[1][2];
	//����
	MatchCard(KaiPaiData.pai);
	for(int i=0;i<MAXCOUNT;i++)
	{
		for(int j=0;j<4;j++)
		{
			KaiPaiData.m_iResultInfo[i][j] = m_iResultInfo[i][j];
		}

		//��ȡ���72�ѵ���ʷ��ׯ�к͵���Ϣ
		KaiPaiData.m_iHistoryPlayData[i] = m_iResultInfoArea[i];
	}

	for(int i=0;i<=PLAY_COUNT;i++)
	{
		KaiPaiData.zhuangstation[i] = m_iZhuangStation[i];
	}

	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
			continue;
		if (m_iCurrentMessage!=ASS_KAI_PAI) 
		{
			break;
		}
		KaiPaiData.m_iUserFen = m_iUserFen[i];//����Լ��ĵ÷�
		SendGameData(i, &KaiPaiData, sizeof(KaiPaiData), MDM_GM_GAME_NOTIFY, ASS_KAI_PAI, 0);//���Ϳ�����Ϣ
	}
	m_bIsJieSuan = false;						/**<�Ƿ��Ѿ�����2010-05-21*/
}
//���ͽ�����Ϣ
void CServerGameDesk::OnSendEnd()
{
	m_bIsJieSuan = true;						/**<�Ƿ��Ѿ�����2010-05-21*/
	KillTimer(ID_TIMER_GAME_SHOW_JS);
	//����
	bool temp_cut[PLAY_COUNT];

	memset(&temp_cut, 0, sizeof(temp_cut));		//ׯ���б��ܵ�ׯ��λ��
	memset(&m_iUserMoney, 0, sizeof(m_iUserMoney)); //��ҽ��
	MsgPrint("zhangjing2::������㿪ʼ") ; 
	////д�����ݿ�
	if(m_iNowNtStation >= 0)	///û��ׯ�ҾͲ����
	{
		///2010-9-27 zht �޸�̨�ѿ۷����⣬û��ע���˵�ׯʱ�����˶�����ע����̨��
		CGameUserInfo	* pUserInfoTemp[MAX_PEOPLE];
		memcpy(pUserInfoTemp,m_pUserInfo, sizeof(m_pUserInfo));
		BOOL bHaveNote = FALSE; //������ע��
		for (int i=0; i<PLAY_COUNT && i<MAX_PEOPLE; i++)
		{
			if (m_iUserXiaZhuData[i][8] == 0 && m_iNowNtStation != i)
			{
				m_pUserInfo[i] = NULL;
			}
			else if(!bHaveNote && m_iNowNtStation != i)
			{
				bHaveNote = TRUE;  
			}
		}
		if (!bHaveNote)
		{
			m_pUserInfo[m_iNowNtStation] = NULL;
		}

		if(m_byNtTax==0)//ֻ��ׯӮ��ˮ
		{
			ChangeUserPointint64(m_iUserFen, temp_cut,m_iNowNtStation);
		}
		else//������Ӯ�ҵ�ˮ
		{
			ChangeUserPointint64(m_iUserFen, temp_cut);
		}
		__super::RecoderGameInfo(m_iUserMoney);
		memcpy(m_pUserInfo,pUserInfoTemp,sizeof(m_pUserInfo));
	}
	MsgPrint("zhangjing2::����������") ; 

	int iBasePoint = m_pDataManage->m_InitData.uBasePoint;
	if (iBasePoint<1)
	{
		iBasePoint = 1;
	}
			
			
	if (m_bAIWinAndLostAutoCtrl)
	{//�����˿����˾�Ҫ���������Ӯ��Ǯ
		//----DWJ-20121108---
		CString sTemp = CINIFile::GetAppPath ();/////����·��
		CINIFile f(sTemp +SKIN_FOLDER  + _T("_s.ini"));
		TCHAR szSec[_MAX_PATH] = TEXT("game");
		_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);
		m_iReSetAIHaveWinMoney	= f.GetKeyVal(szSec,"ReSetAIHaveWinMoney ",0);
			
		if (m_iReSetAIHaveWinMoney != m_iAIHaveWinMoney)
		{//�������ȣ�˵���ֶ��޸��������ļ��е�ֵ���û�������Ǯ�ˣ�����Ⱦ�ȥ�ۼӻ�����Ӯ��Ǯ
			m_iAIHaveWinMoney = m_iReSetAIHaveWinMoney;
		}
		else
		{
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(NULL == m_pUserInfo[i])
				{
					continue;
				}
				if (m_pUserInfo[i]->m_UserData.isVirtual>0)
				{
					m_iAIHaveWinMoney += m_iUserFen[i]*iBasePoint;		///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ	
				}
			}
		}
		//����ǰ������Ӯ��Ǯд�������ļ�����
		sTemp.Format("%d",m_iAIHaveWinMoney);
		f.SetKeyValString(szSec,"ReSetAIHaveWinMoney ",sTemp);
	}
			
	for(int i=0;i<PLAY_COUNT;i++)
	{				
		if(NULL == m_pUserInfo[i])
		{
			continue;
		}
		/*if(m_bAIWinAndLostAutoCtrl && m_IsAIStation[i]&&m_pUserInfo[i]->m_UserData.isVirtual>0)	///��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
		{
			//ֻ���������Ӯ						
			m_iAIHaveWinMoney += m_iUserFen[i]*iBasePoint;		///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ
		}*/

		m_iUserFen[i] = m_iUserMoney[i];
	}

	m_iZhuangFen = m_iUserFen[m_iNowNtStation];			//ׯ�ҵĵ÷֣�����
	m_iNtWin += m_iZhuangFen;							///����ׯ�ҵ��ܳɼ�

	m_iXianFen=0;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(m_pUserInfo[m_iNowNtStation]==NULL || i==m_iNowNtStation)
			continue;
		m_iXianFen += m_iUserFen[i];
	}
	if(m_iNowNtStation<0)
	{
		m_iZhuangFen = 0;
		m_iXianFen = 0;
	}
	JIE_SUAN JieSuanData;		//�������ݰ�
	JieSuanData.m_iNtWin = m_iNtWin;
	JieSuanData.m_iZhuangFen = m_iZhuangFen;
	JieSuanData.m_iXianFen = m_iXianFen;
			
	for(int i=0;i<MAXCOUNT;i++)
	{
		for(int j=0;j<4;j++)
		{
			JieSuanData.m_iResultInfo[i][j] = m_iResultInfo[i][j];
		}
	}
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		JieSuanData.zhuangstation[i] = m_iZhuangStation[i];
	}

	for(int i=0;i<5;i++)		//ׯ������
	{
		JieSuanData.m_iZPaiXing[i] =  m_iZPaiXing[i];
		JieSuanData.m_iXPaiXing[i] =  m_iXPaiXing[i];
	}

	//��ȡӮ������
	for(int i=0;i<8;i++)
	{
		JieSuanData.m_iWinQuYu[i] = m_iWinQuYu[i];			// ��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	}

	for (int i=0;i<PLAY_COUNT; i++)
	{	
		m_iUserXiaZhuInfo[i][m_iHaveGameCount] = 0;
		if (NULL == m_pUserInfo[i])
		{						
			continue;					
		}

		if (m_iUserFen[i] < 0)			//�������ע��������
		{
			m_iUserXiaZhuInfo[i][m_iHaveGameCount] = 2;
		}
		else if (m_iUserFen[i] > 0)		//�������ע����Ӯ��
		{
			m_iUserXiaZhuInfo[i][m_iHaveGameCount] = 1;						
		}					

		//��ȡ�ҵķ�����
		JieSuanData.m_iMeFanFen = GetMeFanFen(i);		//������ҷ����ķ֣�����ʱ����				
		JieSuanData.m_iUserFen = m_iUserFen[i];			//����Լ��ĵ÷�					
		for (int j=0; j<MAXCOUNT; j++)
		{
			JieSuanData.m_iUserXiaZhuInfo[j] = m_iUserXiaZhuInfo[i][j];	//��ȡ��ҵ���ע��Ϣ				
			JieSuanData.m_iResultInfoArea[j] = m_iResultInfoArea[j];	//��ȡ��ʷ��ׯ�պ͵ļ�¼��Ϣ
		}			

		SendGameData(i, &JieSuanData, sizeof(JieSuanData), MDM_GM_GAME_NOTIFY, ASS_SHOW_JS, 0);//���ͽ�����Ϣ	

		MsgPrint("zhangjing2::���ͽ�����Ϣ") ;  
	}

	if(m_bShowAllResultInfo && m_iNowNtStation>=0)
	{
		USER_SCORE_STRUCT  UserScoreData ; 

		for(int i = 0 ; i<PLAY_COUNT ; i++)
		{
			if(NULL != m_pUserInfo[i])
			{
				UserScoreData.llUserFen[i] = m_iUserFen[i] ; 
				UserScoreData.bShowInfo[i] = (m_iUserXiaZhuData[i][8]>0||i == m_iNowNtStation) ; 
			}
			else
			{
				UserScoreData.llUserFen[i] = 0 ; 
				UserScoreData.bShowInfo[i] = false ; 
			}
		}

		for(int i = 0 ; i<PLAY_COUNT ; i++)
		{
			if(NULL != m_pUserInfo[i])
			{
				SendGameData(i, &UserScoreData, sizeof(UserScoreData), MDM_GM_GAME_NOTIFY, ASS_USER_SCORE, 0);//���ͽ�����Ϣ	
			}
		}
	}
}
//���Ϳ�����Ϣ
void CServerGameDesk::OnSenFree()
{
	m_iCurrentMessage = ASS_DENG_DAI;
	GameFinish(0,GF_NORMAL);				//����������Ϸ�����ͽ�����Ϣ
	//����ׯ��
	ClearNt();
	//������ׯ�б�
	MoveData(0);
	if(-1==m_iNowNtStation || NULL==m_pUserInfo[m_iNowNtStation])
	{
		GetNextNT();
		if(m_pUserInfo[m_iNowNtStation])
		{
			//���ͻ�ׯ��Ϣ
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL != m_pUserInfo[i])
				{
					SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_CHANGE_NT,0);
				}
			}
		}
	}
	DENG_DAI Wait;
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		Wait.zhuangstation[i] = m_iZhuangStation[i];
	}
	Wait.m_iNtWin = m_iNtWin;				//��ǰׯ��Ӯ�Ľ��
	Wait.m_iZhuangBaShu = m_iZhuangBaShu;	//ׯ�ҽ����˼���
	//����׼����ʼ��Ϣ
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			memset(m_iUserXiaZhuInfo[i], 0 , sizeof(m_iUserXiaZhuInfo[i]));					
			continue;
		}	
		SendGameData(i, &Wait, sizeof(Wait), MDM_GM_GAME_NOTIFY, ASS_DENG_DAI,0);//���Ϳ���ʱ����Ϣ
		if(i%20==0)//ÿ��20����Ϣ�͵ȴ�10����
		{
			Sleep(10);
		}
	}
}
//��Ϸ���¿�ʼ
void CServerGameDesk::RestartGame()
{
	//����ׯ��
	ClearNt();
	//������ׯ�б�
	MoveData(0);
	if(NULL==m_pUserInfo[m_iNowNtStation])//��ǰׯ�Ҳ�����
	{
		GetNextNT();
		if(m_pUserInfo[m_iNowNtStation])
		{
			//���ͻ�ׯ��Ϣ
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL != m_pUserInfo[i])
				{
					SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_CHANGE_NT,0);
				}
			}
		}
	}
	if(m_pUserInfo[m_iNowNtStation])
	{
		StartGame();//��ʼ��Ϸ
	}
	else
	{
		m_iCurrentMessage = ASS_WU_ZHUANG;
		//������ׯ�ȴ���Ϣ			
		DENG_DAI Wait;
		for(int i=0;i<=PLAY_COUNT;i++)
		{
			Wait.zhuangstation[i] = m_iZhuangStation[i];
		}
		Wait.m_iNtWin = 0;						//��ǰׯ��Ӯ�Ľ��
		Wait.m_iZhuangBaShu = 0;			//ׯ�ҽ����˼���
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL == m_pUserInfo[i])
				continue;
			SendGameData(i,&Wait,sizeof(Wait),MDM_GM_GAME_NOTIFY, ASS_WU_ZHUANG,0);////������ׯ�ȴ���Ϣ
		}

	}
}
///���ƣ�OnGameTimer
///��������ʱ����Ϣ 
///@param byTimerID ��ʱ��id
///@return 
bool CServerGameDesk::OnGameTimer(BYTE byTimerID)
{
	return true;
}


///���ƣ�MoveData
///���������������е����ݣ������õ�����Ԫ����� 
///@param type ���� 0Ϊ��ׯ���б������1Ϊ�Խ������ݲ���
///@return 
void CServerGameDesk::MoveData(int type)
{
	if(type==0)//ׯ���б������ƶ�
	{  
		int num=0;
		int iNtList[PLAY_COUNT+1]; 
		memset(iNtList,-1,sizeof(iNtList));
		for(int i=0;i<PLAY_COUNT;i++)
		{	

			if (NULL != m_pUserInfo[m_iZhuangStation[i]] && m_pUserInfo[m_iZhuangStation[i]]->m_UserData.i64Money >= m_iShangZhuangLimit)//����һ�����Ϸ��
			{
				iNtList[num]=m_iZhuangStation[i];
				num++;
			}
		}
		iNtList[PLAY_COUNT]=num;
		memset(m_iZhuangStation,-1,sizeof(m_iZhuangStation));
		memcpy(m_iZhuangStation,iNtList,sizeof(m_iZhuangStation));
		
	}
	else if (type==1)
	{
		for(int i=1;i<MAXCOUNT;i++)
		{
			m_iResultInfo[i-1][0]=m_iResultInfo[i][0];
			m_iResultInfo[i-1][1]=m_iResultInfo[i][1];
			m_iResultInfo[i-1][2]=m_iResultInfo[i][2];
			m_iResultInfo[i-1][3]=m_iResultInfo[i][3];
			m_iResultInfoArea[i-1] = m_iResultInfoArea[i];
			for (int j=0; j<PLAY_COUNT; j++)
			{
				m_iUserXiaZhuInfo[j][i-1] = m_iUserXiaZhuInfo[j][i] ;
			}
			
		}
	}
}
///���ƣ�GetNextNT
///������ׯ����ׯ���ȡ��һ��ׯ���б��е�ׯ��
///@param 
///@return true �õ���һ��ׯ�� false û��ׯ����
bool CServerGameDesk::GetNextNT()
{
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iZhuangStation[i]>=0 && NULL!=m_pUserInfo[m_iZhuangStation[i]])
		{
			m_iNowNtStation = m_iZhuangStation[i];
			m_iZhuangStation[i]=-1;
			//������ׯ�б�
			MoveData(0);
			return true;
		}
	}
	return false;
}
///���ƣ�HanleXiaZhu
///������������ҵ���ע
///@param Station λ�� pData ���ݰ� uSize ������
///@return 
void CServerGameDesk::HanleXiaZhu(BYTE Station,void * pData, UINT uSize)
{
	XIA_ZHU *pdata = (XIA_ZHU *)pData;
	TCHAR  mess[100];
	wsprintf (mess, "���ڲ��� ���յ�%s��ע��Ϣ %d",m_pUserInfo[Station]->m_UserData.nickName,pdata->money);	
	OutputDebugString(mess);

	m_IsAIStation[Station] = false;/// ��¼�����˵�λ�� 
	if(pdata->m_bIsAI)
	{	
		m_IsAIStation[Station] = true;/// ��¼�����˵�λ�� 
	}

	if (m_iNowNtStation<0 || m_iNowNtStation >=PLAY_COUNT || NULL == m_pUserInfo[m_iNowNtStation])
		return;

	bool sucess=false;//�Ƿ��ܳɹ���ע
	__int64 money=0;
	__int64 temp=0;//�м����
	BYTE ran = 1;//���λ��ģ
	CPoint point(0,0);//���λ��
	money = m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;//lym0512��ȡׯ�ҵĽ���� 
	CString st="";

	char buff[800];
	if((m_iUserXiaZhuData[Station][8] + pdata->money) > m_pUserInfo[Station]->m_UserData.i64Money) //��ҵ���ע�ܶ������ҵĽ�����Ͳ�����ע
	{
		sprintf(buff,"�����ˣ����������ܾ���ע  ����=%d ���=%I64d  ��ע%I64d money=%I64d",pdata->moneytype,pdata->money,m_iUserXiaZhuData[Station][8],m_pUserInfo[Station]->m_UserData.i64Money);
		OutputDebugString(buff);
		return;
	}
	if (CanDouble(Station))
	{
		if( (m_iUserXiaZhuData[Station][8]+ pdata->money)>2*m_iMaxNote && m_iMaxNote>0) //������ע�Ͳ�����
		{	
			OutputDebugString("�����ˣ����������ܾ���ע 111 ");
			return;
		}
	}
	else
	{
		if( (m_iUserXiaZhuData[Station][8]+ pdata->money)>m_iMaxNote && m_iMaxNote>0) //������ע�Ͳ�����
		{
			OutputDebugString("�����ˣ����������ܾ���ע 222 ");
			return;
		}
	}	

	long tempMoney = abs((long)(m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]));//ׯ����λ֮��
	switch(pdata->type)//��ע����
	{
	case GF_ZHUANG:		// ׯ
		{	
			if(m_iMax[GF_ZHUANG] > 0 && (m_iQuYuZhu[GF_ZHUANG] + pdata->money) > m_iMax[GF_ZHUANG] )//��ע���ܳ������ƵĶ��
			{
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 000 ");

				sprintf(buff,"�����ˣ����������ܾ���ע ���� 000 ����=%d ���=%I64d  �����ע%I64d ��ǰ����ע=%I64d",pdata->moneytype,pdata->money,m_iMax[GF_ZHUANG],m_iQuYuZhu[GF_ZHUANG]);
				OutputDebugString(buff);
				return;
			}
			temp = money - m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_Z_TIANWANG]*2 - m_iQuYuZhu[GF_Z_DUIZI]*11 - pdata->money;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 - m_iQuYuZhu[GF_XIAN]);
			if(temp >= 0 && m_iQuYuZhu[GF_ZHUANG] < (100000000000/2))
			{
				m_iMaxZhu[GF_ZHUANG] = temp;   
				sucess = true;
				ran = 2 * GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.x = (rand()%ran - ran/2)*4;
				point.y = (rand()%ran - ran/2)*4;
			}
		}
		break;
	case GF_Z_TIANWANG:	// ׯ����
		{
			if(m_iMax[GF_Z_TIANWANG] > 0 && (m_iQuYuZhu[GF_Z_TIANWANG] + pdata->money) > m_iMax[GF_Z_TIANWANG] )//��ע���ܳ������ƵĶ��
			{
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 111 ");
				return;
			}
			temp = money - tempMoney/*m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]���Գ�ׯ���У���������ֻҪ������֮��Ϳ���*/ - m_iQuYuZhu[GF_Z_TIANWANG]*2 - m_iQuYuZhu[GF_Z_DUIZI]*11 - pdata->money*2;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 + m_iQuYuZhu[GF_HE]*8 + m_iQuYuZhu[GF_T_HE]*32);			
			if(temp >= 0 && m_iQuYuZhu[GF_Z_TIANWANG] < (100000000000/3))//Ϊ�˷�ֹ�������ע*���ʲ��ܴ���20��
			{
				m_iMaxZhu[GF_Z_TIANWANG] = temp;   
				m_iMaxZhu[GF_ZHUANG] = temp;   
				sucess = true;
				ran = 2*GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.x = (rand()%ran - ran/2)*4;
				ran = GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.y = (rand()%ran - ran/2)*3;
			}
		}
		break;
	case GF_Z_DUIZI:	// ׯ����
		{	
			if(m_iMax[GF_Z_DUIZI] > 0 && (m_iQuYuZhu[GF_Z_DUIZI] + pdata->money) > m_iMax[GF_Z_DUIZI])//��ע���ܳ������ƵĶ��
			{
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 222 ");
				return;
			}

			temp = money - tempMoney/*m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]���Գ�ׯ���У���������ֻҪ������֮��Ϳ���*/ - m_iQuYuZhu[GF_Z_TIANWANG]*2 - m_iQuYuZhu[GF_Z_DUIZI]*11 - pdata->money*11;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 + m_iQuYuZhu[GF_HE]*8 + m_iQuYuZhu[GF_T_HE]*32);			
			if(temp >= 0 && m_iQuYuZhu[GF_Z_DUIZI] < (100000000000/12))
			{
				m_iMaxZhu[GF_Z_DUIZI] = temp;   
				m_iMaxZhu[GF_ZHUANG] = temp;   
				sucess = true;
				ran = 2*GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.x = (rand()%ran - ran/2)*4;
				ran = GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.y = (rand()%ran - ran/2)*3;
			}
		}
		break;
	case GF_XIAN:		// ��
		{
			if(m_iMax[GF_XIAN] > 0 && (m_iQuYuZhu[GF_XIAN] + pdata->money) > m_iMax[GF_XIAN])//��ע���ܳ������ƵĶ��
			{	
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 333 ");
				return;
			}
			temp  = money - m_iQuYuZhu[GF_XIAN] - m_iQuYuZhu[GF_X_TIANWANG]*2 - m_iQuYuZhu[GF_X_DUIZI]*11 - pdata->money;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 - m_iQuYuZhu[GF_ZHUANG]);//���������������ע			
			if(temp >= 0 && m_iQuYuZhu[GF_XIAN]<(100000000000/2))
			{
				m_iMaxZhu[GF_XIAN] = temp;   
				sucess = true;
				ran = 2*GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.x = (rand()%ran - ran/2)*4;
				point.y = (rand()%ran - ran/2)*4;
			}
		}
		break;
	case GF_X_TIANWANG:	// ������
		{
			if(m_iMax[GF_X_TIANWANG] > 0 && (m_iQuYuZhu[GF_X_TIANWANG] + pdata->money) > m_iMax[GF_X_TIANWANG])//��ע���ܳ������ƵĶ��
			{
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 444 ");
				return;
			}
			temp  = money - tempMoney/*m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]���Գ�ׯ���У���������ֻҪ������֮��Ϳ���*/ - m_iQuYuZhu[GF_X_TIANWANG]*2 - m_iQuYuZhu[GF_X_DUIZI]*11 - pdata->money*2;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_HE]*8 + m_iQuYuZhu[GF_T_HE]*32);
			if(temp >= 0 && m_iQuYuZhu[GF_X_TIANWANG] < (100000000000/3))
			{
				m_iMaxZhu[GF_X_TIANWANG] = temp;   
				m_iMaxZhu[GF_XIAN] = temp;   
				sucess = true;
				ran = 2*GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.x = (rand()%ran - ran/2)*4;
				ran = GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.y = (rand()%ran - ran/2)*3;
			}
		}
		break;
	case GF_X_DUIZI:	// �ж���
		{
			if(m_iMax[GF_X_DUIZI] > 0 && (m_iQuYuZhu[GF_X_DUIZI] + pdata->money) > m_iMax[GF_X_DUIZI])//��ע���ܳ������ƵĶ��
			{
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 555 ");
				return;
			}
			temp = money - tempMoney/*m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]���Գ�ׯ���У���������ֻҪ������֮��Ϳ���*/ - m_iQuYuZhu[GF_X_TIANWANG]*2 - m_iQuYuZhu[GF_X_DUIZI]*11 - pdata->money*11;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_HE]*8 + m_iQuYuZhu[GF_T_HE]*32);			
			if(temp >= 0 && m_iQuYuZhu[GF_X_DUIZI] < (100000000000/(__int64)12))
			{
				m_iMaxZhu[GF_X_DUIZI] = temp;   
				m_iMaxZhu[GF_XIAN] = temp;   
				sucess = true;
				ran = 2*GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.x = (rand()%ran - ran/2)*4;
				ran = GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.y = (rand()%ran - ran/2)*3;
			}
		}
		break;
	case GF_HE:			// ��
		{
			if(m_iMax[GF_HE] > 0 && (m_iQuYuZhu[GF_HE] + pdata->money) > m_iMax[GF_HE])//��ע���ܳ������ƵĶ��
			{
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 666 ");
				return;
			}
			temp = money - m_iQuYuZhu[GF_HE]*8 - m_iQuYuZhu[GF_T_HE]*32 - pdata->money*8;     //����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11);//���������������ע
			if(temp >= 0 && m_iQuYuZhu[GF_HE] < (100000000000/(__int64)9))
			{
				m_iMaxZhu[GF_HE] = temp/8;   
				sucess = true;
				ran = 2*GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.x = (rand()%ran - ran/2)*5;
				ran = 2*GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.y = (rand()%ran - ran/2)*2;
			}
		}
		break;
	case GF_T_HE:		// ͬ���
		{
			if(m_iMax[GF_T_HE] > 0 && (m_iQuYuZhu[GF_T_HE] + pdata->money) > m_iMax[GF_T_HE] )//��ע���ܳ������ƵĶ��
			{
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 7777 ");
				return;
			}
			temp  = money - m_iQuYuZhu[GF_HE]*8 - m_iQuYuZhu[GF_T_HE]*32 - pdata->money*32;     //����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11);//���������������ע

			if(temp >= 0 && m_iQuYuZhu[GF_T_HE] < (100000000000/(__int64)33))
			{
				m_iMaxZhu[GF_T_HE] = temp/8;
				m_iMaxZhu[GF_HE] = temp/8;   
				sucess = true;
				ran = 2*GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.x = (rand()%ran - ran/2)*5;
				ran = GetWeiZhiRand(m_byQuYuJinBi[pdata->type]);
				point.y = (rand()%ran - ran/2)*3;
			}
		}
		break;
	default:
		break;
	}

	if(sucess)//��ע�ɹ�
	{
		CString strLog="";
		m_iQuYuZhu[pdata->type] += pdata->money;							//��������ע������
		m_iUserXiaZhuData[Station][pdata->type] += pdata->money;			//����ڱ�������ע������
		m_iUserXiaZhuData[Station][8] += pdata->money;						//�����ע�ܶ�����
		if(m_byQuYuJinBi[pdata->type] < 120)
		{
			m_byQuYuJinBi[pdata->type]++;									//��¼�������Ҹ���
		}		

		//����ǻ�������ע
		if(pdata->m_bIsAI)
		{
			m_iAIMoney[pdata->type] += pdata->money;		//��������ע������
		}

		//�����ע�ɹ��󣬸��¿ͻ������Ͻǵ�ׯ���С��Ϳ���ע��
		int temp = 0;
		temp = money - m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_Z_TIANWANG]*2 - m_iQuYuZhu[GF_Z_DUIZI]*11; //����ׯ����������µ�ע��
		temp += m_iQuYuZhu[GF_XIAN];		//���������������ע
		temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11); 
		if(temp >= 0 && m_iQuYuZhu[GF_ZHUANG] < (100000000000/(__int64)2))				//ׯ��ע�������ע�����
		{
			m_iMaxZhu[GF_ZHUANG] = temp;   			
		}	

		temp  = money - m_iQuYuZhu[GF_XIAN] - m_iQuYuZhu[GF_X_TIANWANG]*2 - m_iQuYuZhu[GF_X_DUIZI]*11;	//����������������µ�ע��
		temp += m_iQuYuZhu[GF_ZHUANG];		//���������������ע
		temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11);
		if(temp >= 0 && m_iQuYuZhu[GF_XIAN] < (100000000000/(__int64)2))				//����ע�������ע�����
		{
			m_iMaxZhu[GF_XIAN] = temp;   			
		}

		temp = money - m_iQuYuZhu[GF_HE]*8 - m_iQuYuZhu[GF_T_HE]*32;		//����ÿ������������µ�ע��	
		temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11);
		temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11);
		if(temp >= 0 && m_iQuYuZhu[GF_HE] < (100000000000/(__int64)9))					//����ע�������ע�����
		{
			m_iMaxZhu[GF_HE] = temp/8; 
		}

		m_iZhongZhu += pdata->money;		//��ǰ����ע������
		//���ͳɹ���Ϣ
		XIA_ZHU xiazhu;
		xiazhu.money = pdata->money;
		xiazhu.station = Station;
		xiazhu.type = pdata->type;
		xiazhu.point.SetPoint(point.x,point.y-15);
		xiazhu.moneytype = pdata->moneytype;

		wsprintf (mess, "%s��ע�ɹ�",m_pUserInfo[Station]->m_UserData.nickName);		
		CString fdsa;
		fdsa.Format("temp_%i64d,%i64d",m_iMaxZhu[6],m_iMaxZhu[7]);
		OutputDebugString(fdsa);

		for(int i=0; i<8; i++)
		{
			xiazhu.m_iMaxZhu[i] = m_iMaxZhu[i];
			xiazhu.m_iQuYuZhu[i] = m_iQuYuZhu[i];
			xiazhu.m_iAIMoney[i] = m_iAIMoney[i];			/**< �����˵���ע*/
			xiazhu.m_iUserXiaZhu[i] = m_iUserXiaZhuData[Station][i];   //��ҵ�ǰ��ÿ�������µ�ע��
		}
		for (int i=0; i<PLAY_COUNT; i++)
		{
			if (NULL == m_pUserInfo[i])
			{
				continue;
			}
			if (m_iCurrentMessage != ASS_XIA_ZHU)
			{
				OutputDebugString("MSG_��ǰ������ע��Ϣ�������ж�������ע");
				break;
			}
			if (m_iZongTime == (m_iKaiPaiTime + m_iFreeTime + 2))
			{
				OutputDebugString("MSG_��עʱ���ѹ�");
				break;
			}
			xiazhu.m_iMeZhongZhu = m_iUserXiaZhuData[i][8];
			SendGameData(i,&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY, ASS_XIA_ZHU,0);//���ͳɹ���Ϣ
		}
	}
}

///���ƣ�HanleSzhuang
///������������ҵ���ׯ��Ϣ
///@param Station λ�� pData ���ݰ� uSize ������
///@return 
void CServerGameDesk::HanleSzhuang(BYTE Station,void * pData, UINT uSize)
{
	SHANG_ZHUANG *pdata = (SHANG_ZHUANG *)pData;
	int sucess=2;//�Ƿ��ܳɹ�
	CString st="";

	if(m_pUserInfo[Station] == NULL)
		return;
	m_IsAIStation[Station] = false;/// ��¼�����˵�λ�� 
	if(pdata->isAI)
	{
		m_IsAIStation[Station] = true;/// ��¼�����˵�λ�� 		
	}
	MoveData(0);//��������
	//ׯ�Ҳ�����������ׯ
	if(m_iNowNtStation == Station)
	{
		return;
	}
	if(m_iNowNtStation<0)//û��ׯ��
	{
		m_iNowNtStation = Station;
		long iNTUserID = 0;
		if (m_iNowNtStation>=0)
		{
			iNTUserID = m_pUserInfo[m_iNowNtStation]->m_UserData.dwUserID;		
		}
		SetCurrentZhuangjia(iNTUserID);
		if(m_pUserInfo[m_iNowNtStation])
		{
			StartGame();//��ʼ��Ϸ
		}
		sucess = 0;
	}
	else
	{
		bool bIsList=false;
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_iZhuangStation[i] == Station)//���б��е���Ҳ�������
			{
				sucess = 2;
				bIsList=true;
				break;
			}
		}
		if(!bIsList)
		{
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_iZhuangStation[i]<0)
				{
					m_iZhuangStation[i]=Station;
					sucess = 0;
					break;
				}
			}
		}
	}
	//������ׯ��Ϣ
	SHANG_ZHUANG szhuang;
	szhuang.shang = true;
	szhuang.station = Station;
	szhuang.success = sucess;
	szhuang.m_iNowNtStation = m_iNowNtStation;//ׯ��
	if(	m_bGameStation == GS_SEND_CARD)
	{
		//��ȡ�������������ע
		GetQuYuCanNote();
		for(int i=0;i<8;i++)
		{
			szhuang.m_iMaxZhu[i] = m_iMaxZhu[i];
		}
		szhuang.bXiaZhu = true;   //�Ƿ���ע״̬
	}
	if(sucess==0)//�ɹ��������
	{
		MoveData(0);//��������
		
		szhuang.success = 0;	
		for(int i=0;i<=PLAY_COUNT;i++)
		{
			szhuang.zhuangstation[i] = m_iZhuangStation[i];
		}
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL == m_pUserInfo[i])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
			{
				continue;
			}
			SendGameData(i,&szhuang,sizeof(szhuang),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//���ͳɹ���Ϣ
		}
	}
	else 
	{//����ʧ����Ϣ
		if (NULL != m_pUserInfo[Station])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
		{
			SendGameData(Station,&szhuang,sizeof(szhuang),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//���ͳɹ���Ϣ
		}
	}
}	

///���ƣ�HanleXzhuang
///������������ҵ���ׯ��Ϣ
///@param Station λ�� pData ���ݰ� uSize ������
///@return 
void CServerGameDesk::HanleXzhuang(BYTE Station,void * pData, UINT uSize)
{
	SHANG_ZHUANG *pdata = (SHANG_ZHUANG *)pData;

	if( m_pUserInfo[Station] == NULL)
		return;
	
	bool success = false;
	if(Station == m_iNowNtStation)//��ǰׯ�Ҳ���������ׯ
	{
		m_bXiaZhuang = true;
		success = true;
	}
	else
	{
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_iZhuangStation[i]==Station)//
			{
				m_iZhuangStation[i]=-1;
				success = true;
				break;
			}
		}
	}
	MoveData(0);//��������

	if(!success)
		return;

	m_IsAIStation[Station] = false;/// ��¼�����˵�λ�� 
	if(pdata->isAI)
	{
		m_IsAIStation[Station] = true;/// ��¼�����˵�λ�� 
	}
	//������ׯ��Ϣ
	SHANG_ZHUANG szhuang;
	szhuang.shang = false;
	szhuang.station = Station;
	szhuang.success = 0;
	szhuang.m_iNowNtStation = m_iNowNtStation;
	if(	m_bGameStation == GS_SEND_CARD)
	{
		szhuang.bXiaZhu = true;   //�Ƿ���ע״̬
	}
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		szhuang.zhuangstation[i] = m_iZhuangStation[i];
	}
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
		{
			continue;
		}
		SendGameData(i,&szhuang,sizeof(szhuang),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//���ͳɹ���Ϣ
	}
}	


///���ƣ�CountPaiZhi
///������������ֵ,ǰconut�ŵ�����ֵ
///@param count ǰ���ŵ���ֵ 2 ǰ���ţ�3 ��������ֵ
///@return 
void CServerGameDesk::CountPaiZhi(int count)
{	CString strLog="";
	int zhuang=0;//ׯ�Ƶ�
	int xian=0;  //���Ƶ�
	
	if(count==2)//ǰ�����Ƶ�ֵ
	{
		int value=0,value1=0;
		//��һ����
		value = CPokerGameLogic::GetCardNumber(m_UserCard[0][0]);//ׯ
		value1= CPokerGameLogic::GetCardNumber(m_UserCard[1][0]);//��
		if(value>=10)
		{
			value=0;
		}
		zhuang = value;
		if(value1>=10)
		{
			value1=0;
		}
		xian = value1;

		//�ڶ�����
		value = CPokerGameLogic::GetCardNumber(m_UserCard[0][1]);//ׯ
		value1= CPokerGameLogic::GetCardNumber(m_UserCard[1][1]);//��
		if(value>=10)
		{
			value=0;
		}
		zhuang += value;
		if(value1>=10)
		{
			value1=0;
		}
		xian += value1;

		if(zhuang>=10)
		{
			zhuang%=10;
		}
		if(xian>=10)
		{
			xian%=10;
		}
		m_iZPaiXing[0]=zhuang; //ǰ�����Ƶ��Ƶ�
		m_iXPaiXing[0]=xian;   //

	}
	else if(count==3)//�����Ƶ����Ƶ�
	{
		int value=0,value1=0;
		value = CPokerGameLogic::GetCardNumber(m_UserCard[0][2]);//ׯ
		value1= CPokerGameLogic::GetCardNumber(m_UserCard[1][2]);//��

		if(value>=10)
		{
			value=0;
		}
		if(value1>=10)
		{
			value1=0;
		}

		zhuang = value + m_iZPaiXing[0];	//ׯ�Ƶ�
		xian = value1 + m_iXPaiXing[0];		//���Ƶ�

		if(zhuang>=10)
		{
			zhuang%=10;
		}
		if(xian>=10)
		{
			xian%=10;
		}
		m_iZPaiXing[1]=zhuang;
		m_iXPaiXing[1]=xian;
	}

	if(zhuang>=8)//ׯ����
	{
		m_iZPaiXing[2]=zhuang;
		if(zhuang==xian)//����ͬ��
		{
			m_iZPaiXing[4]=zhuang;
		}
	}

	if(xian>=8)//������
	{
		m_iXPaiXing[2]=xian;
		if(zhuang==xian)//����ͬ��
		{
			m_iXPaiXing[4]=xian;
		}
	}

	if(CPokerGameLogic::GetCardNumber(m_UserCard[0][0])==CPokerGameLogic::GetCardNumber(m_UserCard[0][1]))//ׯ����
	{
		m_iZPaiXing[3]=zhuang;
		if (zhuang == 0)		//��Ҫ�����ڿ��Ƶ�����ֵΪ0ʱ��ǰ��������ͬ��Ȼ�Ƕ���
		{
			m_iZPaiXing[3] = 1;
		}
	}
	if(CPokerGameLogic::GetCardNumber(m_UserCard[1][0])==CPokerGameLogic::GetCardNumber(m_UserCard[1][1]))//�ж���
	{
		m_iXPaiXing[3]=xian;
		if (xian == 0)			//��Ҫ�����ڿ��Ƶ�����ֵΪ0ʱ��ǰ��������ͬ��Ȼ�Ƕ���
		{
			m_iXPaiXing[3] = 1;
		}
	}
}

///���ƣ�ChangeCard
///����������ׯ�е�ǰ������
///@param 
///@return 
void CServerGameDesk::ChangeCard()
{
	WORD temp=0;
	//��һ����
	temp=m_UserCard[0][0];
	m_UserCard[0][0]=m_UserCard[1][0];
	m_UserCard[1][0]=temp;
	//�ڶ�����
	temp=m_UserCard[0][1];
	m_UserCard[0][1]=m_UserCard[1][1];
	m_UserCard[1][1]=temp;
	//��յ�������
	m_UserCard[0][2]=0;
	m_UserCard[1][2]=0;
}

///���ƣ�ChangeWinner
///�������ı�Ӯ��
///@param zhuang:trueΪ����ׯӮ��falseΪ������Ӯ
///@return 
void CServerGameDesk::ChangeWinner(bool zhuang)
{
	if(zhuang)//����ׯӮ
	{
		ChangeCard();
		ClearWinData(false);//����֮ǰ����Ϣ

		CountPaiZhi(2);		//����ǰ������ֵ
		if(GetTheThird())	//��ȡ��������
		{//�е������ƾ����¼�����ֵ
			m_bThreeCard=true;
			CountPaiZhi(3);	//��������ֵ
		}
		GetWin();			//���Ʋ���ȡ�����ź����Ӯ��
		if(m_iWinner==2)	//���������Ӯ���ͽ���3�η���
		{
			for(int i=0;i<10;i++)
			{
				SendCard();
				if(m_iWinner!=2 && m_iWinner>0)
					return;
			}
		}
	}
	else	//������Ӯ
	{
		ChangeCard();
		ClearWinData(false);//����֮ǰ����Ϣ

		CountPaiZhi(2);		//����ǰ������ֵ
		if(GetTheThird())	//��ȡ��������
		{//�е������ƾ����¼�����ֵ
			m_bThreeCard=true;
			CountPaiZhi(3);	//��������ֵ
		}
		GetWin();			//���Ʋ���ȡ�����ź����Ӯ��
		if(m_iWinner==1)	//�������ׯӮ���ͽ���3�η���
		{
			for(int i=0;i<10;i++)
			{
				SendCard();
				if(m_iWinner!=1 && m_iWinner>0)
					return;
			}
		}
	}
}

///���ƣ�ConTrolZX
///���������ƿ�ׯ����
///@param 
///@return 
void CServerGameDesk::ConTrolZX()
{
	int probability=0;
	switch (m_bSuperState)
	{
	case 0:  //������Ϸ
		{
			probability = 0;  
			break;
		}
	case 1:  //��ׯ
		{
			probability = 1;  
			break;
		}
	case 2:  //����
		{
			probability = 2;  
			break;
		}
	case 3:  //����
		{
			probability = 3;  
			break;
		}
	}
	m_bSuperState = 0;

	if(probability == 0) //50%ʱ��������
	{///���û�г����ͻ��˿��Ʋ��ҿ������Զ����ƣ�����л�������Ӯ�Զ����ƣ�lym2010-06-29
		IAWinAutoCtrl();
	}
	else
	{
		if(probability == 1)//���ƿ�ׯ
		{
			if(m_iWinner!=1||m_iZPaiXing[2]>0||m_iXPaiXing[3]>0||m_iZPaiXing[3]>0)//���ֲ��ǿ�ׯ
			{
				for(int i=0;i<50;i++)
				{
					SendCard();
					if(m_iWinner==1 && m_iWinner>0 &&m_iZPaiXing[2]<=0 &&m_iXPaiXing[3]<=0 &&m_iZPaiXing[3]<=0)
						break;
				}
			}
		}
		else if(probability == 2)//���ƿ���
		{
			if(m_iWinner!=2||m_iXPaiXing[2]>0||m_iXPaiXing[3]>0||m_iZPaiXing[3]>0)//���ֲ��ǿ���
			{		
				for(int i=0;i<50;i++)
				{
					SendCard();
					if(m_iWinner==2 && m_iWinner>0 &&m_iXPaiXing[2]<=0 &&m_iXPaiXing[3]<=0 &&m_iZPaiXing[3]<=0)
						break;
				}
			}
		}
		else if(probability == 3)//���ƿ���
		{
			if(m_iWinner!=3||m_iKaiPai == 8||m_iXPaiXing[2]>0||m_iZPaiXing[2]>0||m_iXPaiXing[3]>0||m_iZPaiXing[3]>0 )//���ֲ��ǿ���
			{		
				for(int i=0;i<50;i++)
				{
					SendCard();
					if(m_iWinner==3 && m_iWinner>0 
						&&m_iKaiPai != 8 
						&&m_iXPaiXing[2]<=0 
						&&m_iZPaiXing[2]<=0
						&&m_iXPaiXing[3]<=0 
						&&m_iZPaiXing[3]<=0 )
						break;
				}
			}
		}
	}
}


///���ƣ�ClearWinData
///�����������Ѽ�����Ƶ�����
///@param all:true���������� ��������������
///@return 
void CServerGameDesk::ClearWinData(bool all)
{
	for(int i=0; i<8; i++)
	{
		m_iWinQuYu[0] = 0;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
		if(i<5)
		{
			m_iZPaiXing[i] = 0;
			m_iXPaiXing[i] = 0;
		}
	}
	m_iWinner = 0;
	m_iKaiPai = 0;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	if(all)
	{
		memset(&m_UserCard, 0, sizeof(m_UserCard));	// ׯ�����ϵ��˿�����
		memset(&m_CardCount, 0, sizeof(m_CardCount));	// ׯ�����ϵ��˿�����
	}
}

///���ƣ�CheckTongDian
///������ͬ����
///@param num,���ǰ������
///@return true ͬ�㣬false ����ͬ��
bool CServerGameDesk::CheckTongDian(int num)
{
	int zhuang[3];
	int xian[3];
	int temp=0;
	for(int i=0;i<3;i++)
	{
		zhuang[i]= CPokerGameLogic::GetCardNumber(m_UserCard[0][i]);
		xian[i]  = CPokerGameLogic::GetCardNumber(m_UserCard[1][i]);
	}
	for(int i=0;i<2;i++)//����
	{
		for(int j=i+1;j<3;j++)
		{
			if(zhuang[i]<zhuang[j])
			{
				temp = zhuang[i];
				zhuang[i]=zhuang[j];
				zhuang[j]=temp;
			}
			if(xian[i]<xian[j])
			{
				temp = xian[i];
				xian[i]=xian[j];
				xian[j]=temp;
			}
		}
	}

	for(int i=0;i<num;i++)
	{
		if(zhuang[i] != xian[i])
		{
			return false;
		}
	}
	return true;
}

///���ƣ�GetWin
///��������ȡӮǮ������
///@param 
///@return 
void CServerGameDesk::GetWin()
{
	//������
	memset(&m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	CString strLog="";
	if(!m_bThreeCard)//��ֻ����������,ǰ������ֵ�������Ƶ�ֵ
	{
		m_iZPaiXing[1] = m_iZPaiXing[0];
		m_iXPaiXing[1] = m_iXPaiXing[0];
	}
	if(m_iXPaiXing[2]>0 || m_iZPaiXing[2]>0)//������ʱ
	{
		if(m_iXPaiXing[2] == m_iZPaiXing[2])//�����ͣ���Ӯ��
		{
			//ͬ����
			m_iKaiPai =7;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			if(CheckTongDian(2))//ͬ��� ����Ӯ��ͬ���Ӯ��
			{
				m_iWinQuYu[7] = 33;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iKaiPai =8;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ�7�ͣ�8ͬ���
			}
			m_iWinQuYu[6] = 9;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			//lym0709Ц����Ҫ��ĳɣ����ͣ�ׯ�в��䲻Ӯ
			m_iWinQuYu[0] = 1;     //
			m_iWinQuYu[3] = 1;     //			
			m_iWinner=3;//��Ӯ
		}
		else if(m_iXPaiXing[2] > m_iZPaiXing[2])//����������Ӯ����������
		{
			m_iKaiPai =5;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			if(m_iXPaiXing[3]>0)		//�м����������ӣ���Ӯ��������Ӯ���ж���Ӯ��
			{				
				m_iWinQuYu[5] = 12;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iKaiPai =6;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			}
			m_iWinQuYu[3] = 2;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			m_iWinQuYu[4] = 3;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			m_iWinner=2;//��Ӯ
		}
		else //ׯ������ׯӮ��ׯ����Ӯ��
		{
			m_iKaiPai =2;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			if(m_iZPaiXing[3]>0)//ׯ�����������ӣ�ׯӮ��ׯ����Ӯ��ׯ����Ӯ��
			{
				m_iWinQuYu[2] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���				
				m_iKaiPai =3;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			}
			m_iWinQuYu[0] = 2;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			m_iWinQuYu[1] = 3;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			m_iWinner=1;//ׯӮ
		}
	}
	else//û������
	{
		if(m_bThreeCard)//ׯ��������3����ʹ��m_iXPaiXing[1]������Ƶ�
		{
			if(m_iXPaiXing[1] == m_iZPaiXing[1])//�� ������ͬ ��Ӯ��
			{
				m_iKaiPai =7;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//���ͬ���
				if(CheckTongDian(3))
				{
					m_iWinQuYu[7] = 33;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =8;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[6]=9;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				//lym0709Ц����Ҫ��ĳɣ����ͣ�ׯ�в��䲻Ӯ
				m_iWinQuYu[0] = 1;			//
				m_iWinQuYu[3] = 1;			//				
			    m_iWinner=3;//��Ӯ
			}
			else if(m_iXPaiXing[1] > m_iZPaiXing[1])//�мҴ���Ӯ��
			{
				m_iKaiPai =4;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//����ж���
				if(m_iXPaiXing[3]>0)//�м��Ƕ��ӣ���Ӯ���ж���Ӯ��
				{				
					m_iWinQuYu[5] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =6;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[3] = 2;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iWinner=2;//��Ӯ
			}
			else//ׯ�Ҵ�ׯӮ
			{
				m_iKaiPai =1;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//���ׯ����
				if(m_iZPaiXing[3]>0)//ׯ���Ƕ��ӣ�ׯӮ��ׯ����Ӯ��
				{				
					m_iWinQuYu[2] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =3;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[0] = 2;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iWinner=1;//ׯӮ
			}
		}
		else//û��������ׯ�е��ƶ���2�ţ�˵��ׯ�ж���6��7�� 
		{
			if(m_iXPaiXing[0] == m_iZPaiXing[0])//�� ������ͬ��Ӯ��
			{
				m_iKaiPai =7;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//���ͬ���
				if(CheckTongDian(2))//ͬ��ͣ���Ӯ��ͬ���Ӯ��
				{
					m_iWinQuYu[7] = 33;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =8;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[6] = 9;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				//lym0709Ц����Ҫ��ĳɣ����ͣ�ׯ�в��䲻Ӯ
				m_iWinQuYu[0] = 1;     //
				m_iWinQuYu[3] = 1;     //				
				m_iWinner=3;//��Ӯ
			}
			else if(m_iXPaiXing[0] > m_iZPaiXing[0])//�мҴ���Ӯ��
			{
				m_iKaiPai =4;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//����ж���
				if(m_iXPaiXing[3]>0)//�м��Ƕ��ӣ���Ӯ���ж���Ӯ��
				{	
					m_iWinQuYu[5] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =6;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[3] = 2;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iWinner=2;//��Ӯ
			}
			else//ׯ�Ҵ�ׯӮ
			{
				m_iKaiPai =1;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//���ׯ����
				if(m_iZPaiXing[3]>0)//ׯ���Ƕ��ӣ�ׯӮ��ׯ����Ӯ��
				{			
					m_iWinQuYu[2] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =3;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[0] = 2;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iWinner=1;//ׯӮ
			}
		}
	}

	//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	if (m_iXPaiXing[3] > 0)			//�м����������ӣ���Ӯ��������Ӯ���ж���Ӯ��
	{				
		m_iWinQuYu[5] = m_iLevelBase;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���		
	}	
	if (m_iZPaiXing[3] > 0)			//ׯ�����������ӣ�ׯӮ��ׯ����Ӯ��ׯ����Ӯ��
	{
		m_iWinQuYu[2] = m_iLevelBase;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���				
	}

	if (m_iZPaiXing[1] >= 8)		//�г� �������ߵ��� ʱ ׯ�Ҷ� Ҫ��Ǯ
	{
		m_iWinQuYu[1] = 3;
	}
	if (m_iXPaiXing[1] >= 8)
	{
		m_iWinQuYu[4] = 3; 
	}

	if (TRUE == m_bDoubleWin)		//��ԻԻ�������Ϸ���ĵĿ��Ƶ��ж��ӳ���ʱ��ׯ�ն��Ӷ���Ӯ
	{
		if (m_iZPaiXing[3]>0 || m_iXPaiXing[3]>0)
		{
			m_iWinQuYu[2] = m_iLevelBase;		//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���	
			m_iWinQuYu[5] = m_iLevelBase;		//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���		
		}
	}
}
///���ƣ�GetTwoCard
///��������ȡǰ������
///@param 
///@return 
void CServerGameDesk::GetTwoCard()
{
	m_UserCard[0][0] = 5;
	m_UserCard[0][0] = wCardList[m_iCardCount];
	m_iCardCount += 1;//ׯ�и�ץ������

	m_UserCard[1][0] = wCardList[m_iCardCount];
	m_iCardCount += 1;//ׯ�и�ץ������

	m_UserCard[0][1] = wCardList[m_iCardCount];
	m_iCardCount += 1;//ׯ�и�ץ������

	m_UserCard[1][1] = wCardList[m_iCardCount];
	m_iCardCount += 1;//ׯ�и�ץ������
	m_bThreeCard=false;

	

#ifdef SUPER_VERSION

	CString szKey="game";
	int CardNum=0,CardColour=0;
	int setcard=0;
	CString s = CBcfFile::GetAppPath ();	/////����·��
	CBcfFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = szKey;
	setcard = f.GetKeyVal(key,"SetCard",0);//��ȡ�Ƿ���������õ�ֵ

	if(setcard == 1)
	{
		CardColour = f.GetKeyVal(key,"ZCardColour0",0);//��ȡ���õ��ƻ�ɫ
		CardNum = f.GetKeyVal(key,"ZCardNum0",0);//��ȡ���õ��Ƶ�
		m_UserCard[0][0] = CPokerGameLogic::CardToWord(1, CardColour, CardNum);
		CardColour = f.GetKeyVal(key,"ZCardColour1",0);//��ȡ���õ��ƻ�ɫ
		CardNum = f.GetKeyVal(key,"ZCardNum1",0);//��ȡ���õ��Ƶ�
		m_UserCard[0][1] = CPokerGameLogic::CardToWord(1, CardColour, CardNum);

		CardColour = f.GetKeyVal(key,"XCardColour0",0);//��ȡ���õ��ƻ�ɫ
		CardNum = f.GetKeyVal(key,"XCardNum0",0);//��ȡ���õ��Ƶ�
		m_UserCard[1][0] = CPokerGameLogic::CardToWord(1, CardColour, CardNum);
		CardColour = f.GetKeyVal(key,"XCardColour1",0);//��ȡ���õ��ƻ�ɫ
		CardNum = f.GetKeyVal(key,"XCardNum1",0);//��ȡ���õ��Ƶ�
		m_UserCard[1][1] = CPokerGameLogic::CardToWord(1, CardColour, CardNum);
	}

#endif

}

///���ƣ�GetTheThird
///��������ȡ��������
///@param 
///@return true�е�������  false û�е�������
bool CServerGameDesk::GetTheThird()
{
	if(m_iZPaiXing[2] > 0 || m_iXPaiXing[2] > 0)//ׯ������һ������������ץ��
		return false;

	if(m_iXPaiXing[0] >= 6)//�мҲ���ץ��
	{
		if(m_iZPaiXing[0] <= 5)//ׯ��5������Ҫ����
		{
			m_UserCard[0][2] = wCardList[m_iCardCount];
			m_iCardCount += 1;//ׯ����һ����
			m_CardCount[0] = 3; 			
			return true;
		}
	}
	else//�м�Ҫ��һ����
	{
		m_UserCard[1][2] = wCardList[m_iCardCount];
		//m_UserCard[1][2] = CPokerGameLogic::CardToWord(1, 3, 12);
		m_iCardCount += 1;//�м���һ����
		m_CardCount[1] = 3;

		if(IsGetTheThird())//�����мҵĵ�������ֵȷ��ׯ���Ƿ�Ҫ����
		{//
			m_UserCard[0][2] = wCardList[m_iCardCount];
			//m_UserCard[0][2] = CPokerGameLogic::CardToWord(1, 3, 3);			
			m_iCardCount += 1;//ׯ����һ����
			m_CardCount[0] = 3;
		}
		return true;
	}
	return false;
}

///���ƣ�IsGetTheThird
///�������м����˵�����������£�ׯ����Ҫ����lym0511
///@param 
///@return true��  false ����
bool CServerGameDesk::IsGetTheThird()
{
	int num=0;
	num=CPokerGameLogic::GetCardNumber(m_UserCard[1][2]);//�мҵĵ���������ֵ	
	if(num>=10)
	{
		num=0;
	}
	switch(num)
	{
	case 0:
	case 1:
		{
			if(m_iZPaiXing[0]<=3)//ׯ������3�����£�����
				return true;
		}
		break;
	case 2:
	case 3:
		{
			if(m_iZPaiXing[0]<=4)//ׯ������4�����£�����
				return true;
		}
		break;
	case 4:
	case 5:
		{
			if(m_iZPaiXing[0]<=5)//ׯ������5�����£�����
				return true;
		}
		break;
	case 6:
	case 7:
		{
			if(m_iZPaiXing[0]<=6)//ׯ������6�����£�����
				return true;
		}
		break;
	case 8:
		{
			if(m_iZPaiXing[0]<=2)//ׯ������2�����£�����
				return true;
		}
		break;
	case 9:
		{
			if(m_iZPaiXing[0]<=3)//ׯ������3�����£�����
				return true;
		}
		break;
	default:
		break;
	}
	return false;
}

///���ƣ�SendCard
///����������
///@param 
///@return 
void CServerGameDesk::SendCard()
{
	// ϴ�� ׼���˿�(�������Ƿ�����)
	CPokerGameLogic::RandCard(wCardList, (sizeof(wCardList)/sizeof(wCardList[0])));
	// ���һ������
	m_wMingCard = wCardList[rand()%(POKER_CARD_NUM*POKER_SET_NUM)];//m_iZPaiXing
	//////////////
	ClearWinData(true);
	GetTwoCard();//��ȡǰ�����Ƶ�ֵ	
	
	m_CardCount[0]=2;
	m_CardCount[1]=2;

	CountPaiZhi(2);//����ǰ������ֵ

	if(GetTheThird())//��ȡ��������
	{//�е������ƾ����¼�����ֵ				
		m_bThreeCard=true;
		CountPaiZhi(3);//��������ֵ		
	}
	GetWin();//�����ƺ����Ӯ��
}

///���ƣ�CountFen
///�������������
///@param 
///@return 
void CServerGameDesk::CountFen()
{
	if(m_iNowNtStation<0)
		return;
	memset(&m_iUserFen, 0, sizeof(m_iUserFen)); //��������
	int iUserUserComputer[8];	
	m_iXianFen =0;
	m_iZhuangFen=0;

	m_iUserFen[m_iNowNtStation] = m_iZhongZhu;//ׯ���Ȼ�ȡ��ע

	//ׯ���⸶ÿ�����
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_iNowNtStation)//ׯ�Ҳ������
			continue;
		if(NULL == m_pUserInfo[i])//��Ҳ����ھͲ��ü���
			continue;
		if(m_iUserXiaZhuData[i][8] == 0)//���û����ע�Ͳ��ü���
			continue;
		__int64 win=0;
		__int64 zhu=0;
		
		for(int j=0; j<8; j++)
		{
			
			zhu += m_iUserXiaZhuData[i][j];//����µ���ע��
			win += (m_iUserXiaZhuData[i][j]*m_iWinQuYu[j]);//���Ӯ�Ľ��	
			//win += (m_iUserXiaZhuData[i][j]*iUserUserComputer[j]);//���Ӯ�Ľ��			
		}
		if(zhu <= 0)
			continue;
		
		m_iUserFen[i] = (win-zhu);//Ӯ��-�µ�ע������ұ��ֵľ�Ӯ�����
		m_iUserFen[m_iNowNtStation] -= win;//ׯ��-���Ӯ�ķ֡�
		///lym��ֹ��ҵĽ�ҳ���21�ڣ���ֹ���
		//if( m_iUserFen[i] > (2100000000 - m_pUserInfo[i]->m_UserData.dwMoney))
		//{
		//	m_iUserFen[i] = 2100000000 - m_pUserInfo[i]->m_UserData.dwMoney;
		//}
		m_iXianFen += m_iUserFen[i];//�мҵ��ܷ�
	}
	///lym��ֹׯ�ҵĽ�ҳ���21�ڣ���ֹ���
	//if(m_iUserFen[m_iNowNtStation] > (2100000000 - m_pUserInfo[m_iNowNtStation]->m_UserData.dwMoney))
	//{
	//	m_iUserFen[m_iNowNtStation] = 2100000000 - m_pUserInfo[m_iNowNtStation]->m_UserData.dwMoney;
	//}
}
//һ�ֽ��������˿���ʱ������Ƿ��ܹ�������ʼ��Ϸlym0512
bool CServerGameDesk::CheckCanBegin()
{
	return false;
}

///���ƣ�StartGame
///������ͨ��ȫ�����ֵķ�ʽ��ʼ��Ϸ
///@param 
///@return 
bool CServerGameDesk::StartGame()
{	
	KillTimer(ID_TIMER_GAME_FREE);
	m_bGamePlaying=true;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL == m_pUserInfo[i])//��Ҳ����ھ�
			continue;
		m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
	}	
	GameBegin(ALL_ARGEE);
	return true;
}

///���ƣ�RecordData
///��������¼���ֵĳɼ�����
///@param 
///@return 
void CServerGameDesk::RecordData()
{	
	m_iResultInfo[m_iHaveGameCount][0] = m_iZPaiXing[1];//����ׯ�ܵ���
	m_iResultInfo[m_iHaveGameCount][1] = m_iXPaiXing[1];//�������ܵ���
	m_iResultInfo[m_iHaveGameCount][2] = m_iWinner;     //����Ӯ����1ׯ��2�У�3��
	m_iResultInfo[m_iHaveGameCount][3] = m_iKaiPai;     //����Ӯ����1ׯ��2�У�3��
    
	if(m_iZPaiXing[3]>0)
	{
		m_iResultInfo[m_iHaveGameCount][3] =3;
	}
	if(m_iXPaiXing[3]>0)
	{
		m_iResultInfo[m_iHaveGameCount][3] =6;
	}
	MsgPrint("zhangjing::Server:m_iKaiPai=%d,m_iZPaiXing[3]=%d,m_iZPaiXing[3]=%d,",m_iKaiPai,m_iZPaiXing[3],m_iXPaiXing[3]);
}


///���ƣ�ServerInit
///��������ʼ����������������
///@param 
///@return 
void CServerGameDesk::ServerInit()
{
	InitMinDump();
	memset(&wCardList, 0, sizeof(wCardList));				
	memset(&m_CardCount, 255, sizeof(m_CardCount));//ׯ��˫��������

	for(int i=0;i<PLAY_COUNT;i++)
	{
		m_iUserFen[i] =0;          //������ұ��ֵĵ÷�
		m_iUserMoney[i] =0;          //������ұ���Ӧ�Ľ��
	}
	memset(&m_iAIMoney, 0, sizeof(m_iAIMoney));	/**< �����˵���ע*/
	memset(&m_byQuYuJinBi, 0, sizeof(m_byQuYuJinBi));/**< ���������Ҹ���*/
	for(int i=0;i<8;i++)
	{
		m_iQuYuZhu[i]=0;    //����ÿ�������µ�ע��
		m_iMaxZhu[i]=0;     //����ÿ������������µ�ע��
		m_iPeiLv[i]=0;      //ÿ����������ʣ������ѿ��ƵĽ���㣬Ӯ����Ϊ����������Ϊ����
		m_iWinQuYu[i]=0;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
		for(int j=0;j<6;j++)
		{
			m_iQuYuJinBi[i][j]=0;  //ÿ�����¸��ࣨ��6�ࣩ��ҵĸ���
		}
	}
	for(int i=0;i<6;i++)
	{
		m_iXiaZhuType[i]=0;      //�����ҵ���ע����
	}
	for(int i=0;i<5;i++)
	{	
		m_iZPaiXing[i]=0;         //ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
		m_iXPaiXing[i]=0;         //�м�����
	}		
	memset(&m_UserCard, 0, sizeof(m_UserCard));	//��ҵ���

	for(int i=0;i<PLAY_COUNT;i++)
	{
		for(int j=0;j<11;j++)
		{
			m_iUserXiaZhuData[i][j]=0;
		}
	}
	m_iKaiPai =0;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	m_iWinner=0;				//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	m_iZhongZhu =0;				//���ѵ�ǰ��ע��

	m_iZhuangFen=0;				//ׯ�ҵĵ÷֣�����
	m_iXianFen=0;				//�мҵĵ÷֣�����
    m_iSYTime=0;				//ʣ��ʱ��

	m_iCardCount=0;				//�Ѿ����Ƶ�������
	m_bXiaZhuang=false;			//��ǰׯ��������ׯ
	m_bThreeCard=false;			//ׯ��������3����

	memset(&m_bIsSuperStation, 0, sizeof(m_bIsSuperStation));/**< ��¼������ҵ�λ�á�*/

	m_bIsJieSuan = false;						/**<�Ƿ��Ѿ�����2010-05-21*/

	/////////////////////////////////////////////////////////////
}

///���ƣ�CanBeginGame
///������������Ϸ�ܷ�ʼ����������ֹ��ׯ�ļٿ�ʼ
///@param 
///@return false ���ܿ�ʼ ��������ƽ̨����
bool CServerGameDesk::CanBeginGame()
{
	if(m_iNowNtStation<0)
		return false;
	return __super::CanBeginGame();
}

///���ƣ�SrvTestlog
///��������־��¼�ӿ�
///@param pMessage ��־��Ϣ
///@return 
void  CServerGameDesk::SrvTestlog(TCHAR *pMessage)
{
	try
	{
		CString  tempstr = CINIFile::GetAppPath();
		SYSTEMTIME st;
		GetLocalTime(&st);
		TCHAR FileName[256];
		//wsprintf(FileName, TEXT("e:\\syslog%d.txt\0"),GetCurrentProcessId());
		wsprintf(FileName, TEXT("%s%d-%d-%d %d-bjl-test.txt"),tempstr,st.wYear,st.wMonth,st.wDay,st.wHour);
		CFile syslog;
		syslog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite);
		TCHAR szDate[256];
		wsprintf(szDate, TEXT("%d-%d-%d %d-%d-%d.%d\t"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		syslog.SeekToEnd();
		syslog.Write(szDate,lstrlen(szDate));
		syslog.SeekToEnd();
		syslog.Write(pMessage,lstrlen(pMessage));
		TCHAR nextLine[] = TEXT("\r\n");
		syslog.SeekToEnd();
		syslog.Write(nextLine,lstrlen(nextLine));

		syslog.Close();
	}
	catch(CException *e)
	{
		e->Delete();
		return ; 
	}
}

///���ƣ�OnSuperSetting
///������������Ӯ����
///@param bDeskStation λ�� bState ����״̬
///@return 
void CServerGameDesk::OnSuperSetting(BYTE bDeskStation, BYTE bState)
{
	int iRoomID = m_pDataManage->m_InitData.uRoomID;
	CString szKey= "game";
	CString s = CBcfFile::GetAppPath ();	//����·��
	CINIFile f (s + SKIN_FOLDER +"_s.ini");
	CString key = szKey;

	CString strText;
	if (m_pUserInfo[bDeskStation] != NULL)
	{
		// �жϳ����û��ڷ��������Ƿ�����
		int iCount = f.GetKeyVal(key, "SuperUserCount", 5);
		for (int i = 1; i <= iCount; i++)
		{
			strText.Format("SuperUserID%d", i);
			long  lUserID = f.GetKeyVal(key, strText, 0);

			if (m_pUserInfo[bDeskStation]->m_UserData.dwUserID == lUserID)
			{
				// ���û�Ϊ�����û������Կ�����Ӯ
				m_bSuperSetting = TRUE;
				m_bSuperState = bState;
				SUPERSETTING super;
				super.bDeskStation = bDeskStation;
				super.bState = bState;
				wsprintf (super.name, "%s",m_pUserInfo[bDeskStation]->m_UserData.nickName);
				//�����������ͻ��˷��Ϳ�����Ϣ
				for(int j=0;j<PLAY_COUNT;j++)
				{
					if(m_bIsSuperStation[j])
					{
						SendGameData(j,&super, sizeof(super), MDM_GM_GAME_NOTIFY, ASS_SUPER_SETTING, 0);
					}
				}
				return ;
			}
		}
	}
}

///���ƣ�SetAICtrl
///�����������ͻ������û����˿��Ʋ�������������Ӯ���� ��
///@param Station λ�� 
///@return 
void CServerGameDesk::SetAICtrl(BYTE Station,void * pData, UINT uSize)
{
	if (m_pUserInfo[Station] == NULL)
		return;
	OutputDebugString("������Ϣ���������յ���Ϣ");

	int iRoomID = m_pDataManage->m_InitData.uRoomID;
	CString key= "game";
	CString gameKey ="";
	gameKey.Format("%d_%d",NAME_ID, iRoomID);
	CString s = CBcfFile::GetAppPath ();	//����·��
	CINIFile f (s + SKIN_FOLDER +"_s.ini");
	CString strText;

	// �жϳ����û��ڷ��������Ƿ�����
	int iCount = f.GetKeyVal(key, "SuperUserCount", 5);
	for (int i = 1; i <= iCount; i++)
	{
		strText.Format("SuperUserID%d", i);
		long  lUserID = f.GetKeyVal(key, strText, 0);
		if (m_pUserInfo[Station]->m_UserData.dwUserID != lUserID)//�������ǳ����ͻ���
			continue;
		AICTRLINFO *pAIInfo = (AICTRLINFO *)pData;
		// ���û����˿��Ʋ���
		if(pAIInfo->bIsClearAIWinMoney)///���������Ӯ��Ǯ
		{
			m_iAIHaveWinMoney = 0;
			m_iAICtrlCount = 0;
		}
		if(pAIInfo->bIsSetAICtrl)//���û����˿���
		{
			CString value = "";
			if(pAIInfo->bAIWinAndLostAutoCtrl != m_bAIWinAndLostAutoCtrl)//�Ƿ��������˿���
			{
				value.Format("%d",pAIInfo->bAIWinAndLostAutoCtrl);
				f.SetKeyValString(gameKey,"AIWinAndLostAutoCtrl",value);
				m_bAIWinAndLostAutoCtrl = pAIInfo->bAIWinAndLostAutoCtrl;
			}
			if(pAIInfo->iAIWantWinMoney != m_iAIWantWinMoney)//������ҪӮ��Ǯ
			{
				value.Format("%d",pAIInfo->iAIWantWinMoney);
				f.SetKeyValString(gameKey,"AIWantWinMoney",value);
				m_iAIWantWinMoney = pAIInfo->iAIWantWinMoney;
			}
			if(pAIInfo->iAIMaxWinInOne != m_iAIMaxWinInOne)//ƽ��ÿ��Ӯ�����ֵ
			{
				value.Format("%d",pAIInfo->iAIMaxWinInOne);
				f.SetKeyValString(gameKey,"AIMaxWinInOne",value);
				m_iAIMaxWinInOne = pAIInfo->iAIMaxWinInOne;
			}
		}

		AICTRLINFO AIInfo;
		AIInfo.bDeskStation = Station;//λ��
		AIInfo.bAIWinAndLostAutoCtrl = m_bAIWinAndLostAutoCtrl;//�Ƿ�������
		AIInfo.iAICtrlCount = m_iAICtrlCount;		//�Ѿ����п��Ƶİ���
		AIInfo.iAIWantWinMoney = m_iAIWantWinMoney;	//�����˵�ӮǮĿ��
		AIInfo.iAIMaxWinInOne = m_iAIMaxWinInOne;	//������ƽ��ÿ��ӮǮ���ֵ
		AIInfo.iAIHaveWinMoney = m_iAIHaveWinMoney;	//�������Ѿ�Ӯ�˶���Ǯ
		
		//��������Ϣ�������������ͻ���
		for(int j=0;j<PLAY_COUNT;j++)
		{				///��������Ӯ���� ���򳬼��ͻ��˷��ͻ����˿�����Ϣ
			if(m_bIsSuperStation[j])
			{
				strText.Format("������Ϣ��������Ϣ�������ͻ��� Ŀ��%d",pAIInfo->iAIWantWinMoney);
				OutputDebugString(strText);
				SendGameData(j,&AIInfo, sizeof(AIInfo), MDM_GM_GAME_NOTIFY, ASS_SUPER_AI_INFO, 0);
			}
		}
		return ;
	}
	
}

///���ƣ�AuthPermissions
///��������֤�����û�Ȩ�ޣ�����Ƿ񳬼��ͻ��ˣ��������ͻ�����ҷ�����Ϣ
///@param 
///@return 
void CServerGameDesk::AuthPermissions()
{
	int iRoomID = m_pDataManage->m_InitData.uRoomID;
	CString szKey= "";
	szKey.Format("bjl%d", iRoomID);
	CString s = CBcfFile::GetAppPath ();	//����·��
	CINIFile f (s + SKIN_FOLDER +"_s.ini");
	CString key = "game";

	SUPERSTATE superState;

	CString strText;
	BOOL bFind = FALSE;
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue ;
		}
		bFind = FALSE;

		// �жϳ����û��ڷ��������Ƿ�����
		int iCount = f.GetKeyVal(key, "SuperUserCount", 0);
		for (int j = 1; j <= iCount; j++)
		{
			strText.Format("SuperUserID%d", j);
			int  lUserID = f.GetKeyVal(key, strText, 0);
			if (m_pUserInfo[i]->m_UserData.dwUserID == lUserID)
			{
				bFind = TRUE;
				m_bIsSuperStation[i] = true;/**< ��¼������ҵ�λ�á�*/
				// ���û�Ϊ�����û�, ֪ͨ�ͻ���
				superState.bDeskStation = i;
				superState.bEnable = TRUE;  // �����ͻ���

				memcpy(superState.m_iOldMomey,m_iUserOldMoney,sizeof(m_iUserOldMoney));/* ��ҵĴ����� */
				SendGameData(i, &superState, sizeof(superState), MDM_GM_GAME_NOTIFY, ASS_SUPER_STATE, 0);

				///��������Ӯ���� ���򳬼��ͻ��˷��ͻ����˿�����Ϣ
				AICTRLINFO AIInfo;
				AIInfo.bDeskStation = i;//λ��
				AIInfo.bAIWinAndLostAutoCtrl = m_bAIWinAndLostAutoCtrl;	//�Ƿ�������
				AIInfo.iAICtrlCount = m_iAICtrlCount;					//�Ѿ����п��Ƶİ���
				AIInfo.iAIWantWinMoney = m_iAIWantWinMoney;				//�����˵�ӮǮĿ��
				AIInfo.iAIMaxWinInOne = m_iAIMaxWinInOne;				//������ƽ��ÿ��ӮǮ���ֵ
				AIInfo.iAIHaveWinMoney = m_iAIHaveWinMoney;				//�������Ѿ�Ӯ�˶���Ǯ
				//��������Ϣ���������ͻ���
				SendGameData(i,&AIInfo, sizeof(AIInfo), MDM_GM_GAME_NOTIFY, ASS_SUPER_AI_INFO, 0);
				OutputDebugString("�յ���Ϣ:���Ϳ�����Ϣ");
				break;
			}
		}

		if (!bFind)
		{			
			// ���û�Ϊ�����û�, ֪ͨ�ͻ���
			superState.bDeskStation = i;
			superState.bEnable = FALSE;   // ��ͨ�ͻ���
			SendGameData(i, &superState, sizeof(superState), MDM_GM_GAME_NOTIFY, ASS_SUPER_STATE, 0);
		}
	}
}

///���ƣ�GetMeFanFen
///��������ȡ�����֣�����ʱ��ׯ���е���һ�ȡԭ���ķ���
///@param station ��ȡ���ֵ�λ��
///@return  ���صķ���
int CServerGameDesk::GetMeFanFen(int station)
{
	if(m_iNowNtStation<0)
		return 0;
	int fen = 0;
	if(m_pUserInfo[station] == NULL)
		return fen;
	if(station == m_iNowNtStation)
		return fen;

	if(m_iWinner!=3)//���ǿ���
		return fen;
	fen = m_iUserXiaZhuData[station][0] + m_iUserXiaZhuData[station][3];
	return fen;
}

///���ƣ�SetResultData
///���������������ݣ���¼���õ������ݣ�
///@param num �ڼ���
///@return  
void CServerGameDesk::SetResultData(int num)
{
	for(int i=0;i<8;i++)
	{
		if(i<3)
		{
			m_ResultData.Z_Pai[num][i] = m_UserCard[0][i];			//ׯ��72��������
			m_ResultData.X_Pai[num][i] = m_UserCard[1][i];			//�е�72��������
		}
		if(i<5)
		{
			m_ResultData.m_iZPaiXing[num][i] = m_iZPaiXing[i];			//ׯ��72��������
			m_ResultData.m_iXPaiXing[num][i] = m_iXPaiXing[i];			//�е�72��������
		}
		m_ResultData.m_iWinQuYu[num][i] = m_iWinQuYu[i];		//Ӯ������
	}

	m_ResultData.m_iWinner[num] = m_iWinner;			//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	m_ResultData.m_iKaiPai[num] = m_iKaiPai;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	m_ResultData.m_CardCount[num][0] = m_CardCount[0];	//˫���������� 
	m_ResultData.m_CardCount[num][1] = m_CardCount[1];	//˫���������� 

	CString strT="";
	strT.Format("��ӡ���ԣ�ׯ����%d, �е���%d ", m_iZPaiXing[1], m_iXPaiXing[1]);
	if(num>60)
	{
		OutputDebugString(strT);
	}
}

///���ƣ�GetResultData
///��������ȡ�ڼ��ѵ���Ϣ
///@param num �ڼ���
///@return  
void CServerGameDesk::GetResultData(int num)
{
	for(int i=0;i<8;i++)
	{
		if(i<3)
		{
			m_UserCard[0][i] = m_ResultData.Z_Pai[num][i];			//ׯ��72��������
			m_UserCard[1][i] = m_ResultData.X_Pai[num][i];			//�е�72��������
		}
		if(i<5)
		{
			m_iZPaiXing[i] = m_ResultData.m_iZPaiXing[num][i];			//ׯ��72��������
			m_iXPaiXing[i] = m_ResultData.m_iXPaiXing[num][i];			//�е�72��������
		}
		m_iWinQuYu[i] = m_ResultData.m_iWinQuYu[num][i];		//Ӯ������
	}

	m_iWinner = m_ResultData.m_iWinner[num];			//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	m_iKaiPai = m_ResultData.m_iKaiPai[num];			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	m_CardCount[0] = m_ResultData.m_CardCount[num][0];	//˫���������� 
	m_CardCount[1] = m_ResultData.m_CardCount[num][1];	//˫����������
}

///���ƣ�OnGetGameData
///�����������������ҷ�����Ϸ����
///@param Station λ��  pData ���ݰ�  uSize ���ݰ�����
///@return  
void CServerGameDesk::OnGetGameData(BYTE Station,void * pData, UINT uSize)
{
	CHONG_LIAN sGameData;
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		sGameData.zhuangstation[i] = m_iZhuangStation[i];
	}
	for(int i=0;i<MAXCOUNT;i++)
	{
		for(int j=0;j<4;j++)
		{
			sGameData.m_iResultInfo[i][j] = m_iResultInfo[i][j];
		}
		sGameData.m_iUserXiaZhuInfo[i] = m_iUserXiaZhuInfo[Station][i];
		sGameData.m_iResultInfoArea[i] = m_iResultInfoArea[i];
	}
	sGameData.m_iBaSHu = m_iBaSHu;
	sGameData.m_iGameCount = m_iGameCount;
	sGameData.m_iZhuangBaShu = m_iZhuangBaShu;
	sGameData.m_iNowNtStation = m_iNowNtStation;
	sGameData.m_iSYTime = m_iSYTime;
	sGameData.iZhongZhu = m_iZhongZhu;//���ѵ�ǰ��ע��
	sGameData.m_iShangZhuangLimit = m_iShangZhuangLimit;

	sGameData.m_iXiaZhuTime = m_iXiaZhuTime;		//��עʱ��
	sGameData.m_iKaiPaiTime = m_iKaiPaiTime;		//����ʱ��
	sGameData.m_iFreeTime = m_iFreeTime;			//����ʱ��
	sGameData.m_iSendInterval = m_GameBaseInfo.iSendCardInterval;	// ���Ƽ��ʱ��
	sGameData.m_iSendCardTime = m_GameBaseInfo.iSendCardTime;		//����ʱ��(ms)
	sGameData.m_iSendCardZhen = m_GameBaseInfo.iSendCardZhen;		//����֡��
	sGameData.m_iNowJuShu = m_iNowJuShu;							//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�

	for(int i=0;i<8;i++)
	{
		sGameData.m_iMeZhu[i] = m_iUserXiaZhuData[Station][i];	//���������ÿ�������µ�ע
		sGameData.iMaxZhu[i] = m_iMaxZhu[i];					//ÿ���������¶���ע	
		sGameData.m_iQuYuZhu[i] = m_iQuYuZhu[i];				//����ÿ�������µ�ע��
		sGameData.m_iAIMoney[i] = m_iAIMoney[i];				/**< �����˵���ע*/
		for(int j=0;j<6;j++)
		{
			sGameData.m_iQuYuJinBi[i][j] = m_iQuYuJinBi[i][j];	//ÿ�����¸��ࣨ��6�ࣩ��ҵĸ���
		}
	}
	sGameData.m_iMeZhu[8] = m_iUserXiaZhuData[Station][8];				//���������ÿ�������µ�ע
	sGameData.iMaxNote = CanDouble(Station)?2*m_iMaxNote:m_iMaxNote;	//�����ע�ⶥ
	sGameData.bIsVipDouble = m_bIsVipDouble;
	sGameData.m_iBaseMaxNote = m_iMaxNote;		//��ͨ��������ע

	if(m_bGameStation == GS_SEND_CARD)//��ע״̬
	{
		//��������
		sGameData.m_bGameStation = GS_SEND_CARD;
		SendGameData(Station, &sGameData, sizeof(CHONG_LIAN), MDM_GM_GAME_NOTIFY, ASS_APPLY_GAME_DATA, 0);
	}
	else if(m_bGameStation == GS_PLAY_GAME)//����״̬
	{
		for(int i=0;i<5;i++)
		{
			sGameData.m_iZPaiXing[i] = m_iZPaiXing[i];	//ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
			sGameData.m_iXPaiXing[i] = m_iXPaiXing[i];
		}
		for(int i=0;i<2;i++)
		{	
			sGameData.m_CardCount[i] = m_CardCount[i];	//˫���������� 
			for(int j=0;j<3;j++)
			{
				 sGameData.pai[i][j] = m_UserCard[i][j];//ׯ�е��ƣ�0Ϊׯ��1Ϊ��
			}
		}
		sGameData.m_iZhuangFen = m_iZhuangFen;			//ׯ�ҵĵ÷�
		sGameData.m_iXianFen = m_iXianFen;				//�мҵĵ÷�
		sGameData.m_iUserFen = m_iUserFen[Station];		//��ǰ��ҵĵ÷�
		sGameData.m_iWinner = m_iWinner;				//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
		sGameData.m_iKaiPai = m_iKaiPai;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
		sGameData.m_iShangZhuangLimit = m_iShangZhuangLimit;
		sGameData.m_bGameStation = GS_PLAY_GAME;
		sGameData.m_bIsJieSuan = m_bIsJieSuan;						/**<�Ƿ��Ѿ�����2010-05-21*/
		//��������
		SendGameData(Station, &sGameData, sizeof(CHONG_LIAN), MDM_GM_GAME_NOTIFY, ASS_APPLY_GAME_DATA, 0);
	}
}

///���ƣ�PrintEnding
///��������ӡ��������
///@param 
///@return  
void CServerGameDesk::PrintEnding()
{
	try
	{
		CTime time = CTime::GetCurrentTime();
		char szPath[MAX_PATH],szFilename[MAX_PATH];

		wsprintf(szPath, "D:\\GamePrint\\%d", NAME_ID);
		// �����ļ��� ��Ҫ����#include <shlobj.h>
		SHCreateDirectoryEx(NULL, szPath, NULL);
		// �ļ�����
		wsprintf(szFilename, "%s\\%d(%d�ŷ���%d��) %d-%d-%d %d��%d��%d��.txt", 
			szPath, NAME_ID, m_pDataManage->m_InitData.uRoomID, m_iNowJuShu, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		CFile file;
		file.Open(szFilename, CFile::modeCreate | CFile::modeReadWrite);

		TCHAR chKind[5][8] = {"����", "÷��", "����", "����", ""};
		TCHAR chNumber[15][8] = {"A","2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "С��","����"};

		BYTE bCard1, bCard2, bCard3;         // ��
		BYTE bKind1, bKind2, bKind3;         // ��ɫ
		BYTE bNumber1, bNumber2, bNumber3;     // �Ƶ�

		TCHAR msg[MAX_PATH];

		for (int i = 0; i < MAXCOUNT; i++)
		{
			wsprintf(msg, "---------------- ��%d�� ----------------\r\n", i + 1);
			file.Write(msg, strlen(msg));
			///ׯ��
			// ��һ����
			bNumber1 =  CPokerGameLogic::GetCardNumber(m_ResultData.Z_Pai[i][0]);	// ��ȡ�Ƶ�
			bKind1 = CPokerGameLogic::GetCardShape(m_ResultData.Z_Pai[i][0]);		// ��ȡ��ɫ

			// �ڶ�����
			bNumber2 =  CPokerGameLogic::GetCardNumber(m_ResultData.Z_Pai[i][1]);	// ��ȡ�Ƶ�
			bKind2 = CPokerGameLogic::GetCardShape(m_ResultData.Z_Pai[i][1]);		// ��ȡ��ɫ

			bNumber3 =  CPokerGameLogic::GetCardNumber(m_ResultData.Z_Pai[i][2]);	// ��ȡ�Ƶ�
			bKind3 = CPokerGameLogic::GetCardShape(m_ResultData.Z_Pai[i][2]);		// ��ȡ��ɫ
			
			// ��ӡÿ��λ�õ�������
			if(m_ResultData.m_CardCount[i][0]>2)
			{
				wsprintf(msg, "ׯ: %d��, %s%s, %s%s, %s%s\r\n",m_ResultData.m_iZPaiXing[i][1], chKind[bKind1-1], chNumber[bNumber1-1], chKind[bKind2-1], chNumber[bNumber2-1], chKind[bKind3-1], chNumber[bNumber3-1]);
			}
			else
			{
				wsprintf(msg, "ׯ: %d��, %s%s, %s%s\r\n",m_ResultData.m_iZPaiXing[i][1], chKind[bKind1-1], chNumber[bNumber1-1], chKind[bKind2-1], chNumber[bNumber2-1]);
			}
			// д���ļ���
			file.Write(msg, strlen(msg));
			///�м�
			// ��һ����
			bNumber1 =  CPokerGameLogic::GetCardNumber(m_ResultData.X_Pai[i][0]);	// ��ȡ�Ƶ�
			bKind1 = CPokerGameLogic::GetCardShape(m_ResultData.X_Pai[i][0]);		// ��ȡ��ɫ

			// �ڶ�����
			bNumber2 =  CPokerGameLogic::GetCardNumber(m_ResultData.X_Pai[i][1]);	// ��ȡ�Ƶ�
			bKind2 = CPokerGameLogic::GetCardShape(m_ResultData.X_Pai[i][1]);		// ��ȡ��ɫ

			bNumber3 =  CPokerGameLogic::GetCardNumber(m_ResultData.X_Pai[i][2]);	// ��ȡ�Ƶ�
			bKind3 = CPokerGameLogic::GetCardShape(m_ResultData.X_Pai[i][2]);		// ��ȡ��ɫ

			// ��ӡÿ��λ�õ�������
			if(m_ResultData.m_CardCount[i][1]>2)
			{
				wsprintf(msg, "��: %d��  %s%s, %s%s,  %s%s \r\n",m_ResultData.m_iXPaiXing[i][1], chKind[bKind1-1], chNumber[bNumber1-1], chKind[bKind2-1], chNumber[bNumber2-1], chKind[bKind3-1], chNumber[bNumber3-1]);
			}
			else
			{
				wsprintf(msg, "��: %d��  %s%s, %s%s\r\n",m_ResultData.m_iXPaiXing[i][1], chKind[bKind1-1], chNumber[bNumber1-1], chKind[bKind2-1], chNumber[bNumber2-1]);
			}
			// д���ļ���
			file.Write(msg, strlen(msg));
			wsprintf(msg, "---------------------------------------\r\n");
			file.Write(msg, strlen(msg));

		}
		// �ر��ļ�
		file.Close();
	}
	catch (CException *e) 
	{
		e->Delete();
	}
}
/*
///���ƣ�GetIAWinQuYu
///��������������Ӯ���ƣ���������Ӯ�Զ�����
///@param 
///@return ����ֵ 0 ׯ��1 �У�2 ��
void CServerGameDesk::IAWinAutoCtrl()
{///���û�г����ͻ��˿��Ʋ��ҿ������Զ����ƣ�����л�������Ӯ�Զ�����
	if(!m_bAIWinAndLostAutoCtrl)
		return;
	bool bAIWin = false;
	CString str="";

	m_iAICtrlCount++;
	if(m_iAIHaveWinMoney < m_iAIWantWinMoney)
	{
		//��û��׬��Ǯ
		if(/*m_iAIMaxWinInOne<=0 || *//*( m_iAIMaxWinInOne>0 && m_iAIMaxWinInOne>(m_iAIHaveWinMoney/m_iAICtrlCount)))
		{///����ÿ�����ֵ����û�е����ֵ��������ҪӮ��ׯӮǮ
			bAIWin = true;
			//OutputDebugString("wysoutforƽ��ÿ�ѿ���");
		}	
	}
	//������û�п��Ƶ���������������˵���Ӯ���ϱ��ѵ���ӮС�뵱ǰ����Ӯ��Ǯ������Ҫ���ӮǮ
	if (!bAIWin&&CountAIWinMoney()<0)
	{
		if(m_iAIHaveWinMoney+CountAIWinMoney()<m_iAIWantWinMoney)
		{
			bAIWin = true;
		}
	}

	//CString CST;
	//CST.Format("wysoutfor::����ǰ�������Ѿ�Ӯ��Ǯ===%d,׼��Ӯ��Ǯ==%d,�����ֵ==%d,�������=%d",m_iAIHaveWinMoney,m_iAIWantWinMoney,CountAIWinMoney(),bAIWin);
	//OutputDebugString(CST);
	
	if(bAIWin)
	{//������ҪӮǮ
		/*str.Format("�������Զ����ƣ�������ҪӮ   ��Ӯ%d ƽ������%d ��ǰƽ��%d",m_iAIHaveWinMoney,m_iAIMaxWinInOne,m_iAIHaveWinMoney/m_iAICtrlCount);
		OutputDebugString(str);*//*
		if(CountAIWinMoney()<0)
		{
			for(int i=0; i<50; i++)
			{
				SendCard();
				if(CountAIWinMoney()>=0)
					break;
			}
		}
	}
	//else
	//{///������Ҫ��Ǯ
	//	/*str.Format("�������Զ����ƣ�������Ҫ��   ��Ӯ%d ƽ������%d ��ǰƽ��%d",m_iAIHaveWinMoney,m_iAIMaxWinInOne,m_iAIHaveWinMoney/m_iAICtrlCount);
	//	OutputDebugString(str);*//*
	//	if(CountAIWinMoney()>0)
	//	{
	//		for(int i=0; i<50; i++)
	//		{
	//			SendCard();
	//			if(CountAIWinMoney()<=0)
	//				break;
	//		}
	//	}
	//}
}
*/
//-------------------------------------------------------------------------------------------------------------------
///���ƣ�IAWinAutoCtrl
///��������������Ӯ���ƣ������˸����趨���������һ������Ӯ���ʣ��û�������Ӯ�����ȶ�
void CServerGameDesk::IAWinAutoCtrl()
{///���û�г����ͻ��˿��Ʋ��ҿ������Զ����ƣ�����л�������Ӯ�Զ�����
	if(!m_bAIWinAndLostAutoCtrl)
		return;
	bool bAIWin = false;
	m_iAICtrlCount++;

	srand((unsigned)GetTickCount());
	int iResult = rand()%100;
	
	if (0 >=m_iAIHaveWinMoney )
	{//������Ӯ��Ǯ����0 �����˱�Ӯ
		bAIWin = true;		
	}
	else if((0 < m_iAIHaveWinMoney) && (m_iAIHaveWinMoney < m_iAIWantWinMoneyA1))
	{// �����˵�ӮǮ��0-A1���� ���Ҹ��ʷ���m_iAIWinLuckyAtA1 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA1)
			bAIWin = true;								
	}
	else if ((m_iAIWantWinMoneyA1 <= m_iAIHaveWinMoney)  && (m_iAIHaveWinMoney <m_iAIWantWinMoneyA2))
	{// �����˵�ӮǮ��A1-A2���� ���Ҹ��ʷ���m_iAIWinLuckyAtA2 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA2)
			bAIWin = true;			
	}
	else if ((m_iAIWantWinMoneyA2 <= m_iAIHaveWinMoney)  && (m_iAIHaveWinMoney <m_iAIWantWinMoneyA3))
	{// �����˵�ӮǮ��A2-A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA3 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA3)
			bAIWin = true;
	}
	else
	{// �����˵�ӮǮ����A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA4 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA4)
			bAIWin = true;
	}
		
	if(bAIWin)
	{//������ҪӮǮ
		if(CountAIWinMoney()<0)
		{
			for(int i=0; i<50; i++)
			{
				SendCard();
				if(CountAIWinMoney()>=0)
					break;
			}
		}
	}
	else
	{///������Ҫ��Ǯ//��������ӮǮ����0���ߵ��»�����ӮǮΪ��������Ҫ���·���
		if(CountAIWinMoney()>0 || (m_iAIHaveWinMoney +CountAIWinMoney())<0)
		{
			for(int i=0; i<50; i++)
			{
				SendCard();
				if(CountAIWinMoney()<=0)
				{
					if ((m_iAIHaveWinMoney +CountAIWinMoney())<0)
					{//����������»�������ӮǮΪ��������ô�˴��ؿ���ֱ���û�����Ӯ��
						//DWJ��������Ǯ�����һ�����ӮǮ��ȥ�������Ǯ����Ϊ����,����ط���50�λ����У�ֻ��˵��RP��������ߡ�����
						if (49 != i)
						{
							continue;
						}
						//���·��Ʒ���49�λ���û�з���Ҫ��ģ�ֻ���û�����Ӯ��
						for (int j=0; j<50; j++)
						{
							SendCard();
							if (CountAIWinMoney()>=0)
								break;
						}
					}
					break;
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
///���ƣ�CountAIWinMoney
///��������������Ӯ���ƣ����㵱ǰ�ƻ����˵���ӮǮ
///@param 
///@return ������Ӯ��Ǯ������Ϊ��Ǯ
int CServerGameDesk::CountAIWinMoney()
{
	__int64 money = 0;
	if(m_IsAIStation[m_iNowNtStation])
	{///�����������ׯ��,������ʵ��ҵ�Ǯ
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_IsAIStation[i] || NULL == m_pUserInfo[i] || m_iUserXiaZhuData[i][8] == 0)//�����˲��ü��� || ��Ҳ����ھͲ��ü��� || ���û����ע�Ͳ��ü���
				continue;
			for(int j=0;j<8;j++)
			{			
				money += m_iUserXiaZhuData[i][j];
				money -= (m_iUserXiaZhuData[i][j]*m_iWinQuYu[j]);
			}
		}
	}
	else
	{///��������˲���ׯ��,��������˵�Ǯ
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(!m_IsAIStation[i])//�����˲��ü��� || ��Ҳ����ھͲ��ü��� || ���û����ע�Ͳ��ü���
				continue;
			for(int j=0;j<8;j++)
			{			
				money -= m_iUserXiaZhuData[i][j];
				money += (m_iUserXiaZhuData[i][j]*m_iWinQuYu[j]);
			}
		}
	}
	return money;
}

///���ƣ�GetAnAIApplyStation
///��������ȡһ������������ׯ�����Ļ�����λ�ã���������ׯ���ƣ�
///@param 
///@return -1 û������������ >=0 �Ϸ�λ��ֵ
int CServerGameDesk::GetAnAIApplyStation()
{
	int station = -1;
	bool zhuang = false;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(!m_IsAIStation[i] || NULL == m_pUserInfo[i] || m_iNowNtStation == i||m_pUserInfo[i]->m_UserData.isVirtual<=0)
			continue;
		srand((unsigned)time(NULL)+i);
	    int iRand =(rand()%3+i)%3;
		if (iRand>1)
		{
			continue;
		}
		zhuang = false;
		for(int j=0; j<PLAY_COUNT; j++)
		{
			if(m_iZhuangStation[j] == i)
				zhuang = true;
		}
		if(zhuang)//�Ѿ���ׯ���б�
			continue;
		if(m_pUserInfo[i]->m_UserData.i64Money< m_iShangZhuangLimit|| m_pUserInfo[i]->m_UserData.i64Money>1500000000000000000)
			continue;
		station = i;
		break;
	}
	return station;
}

///���ƣ�SetAIToApplyList
///��������һ�������˷ŵ�ׯ���б��У���������ׯ���ƣ�
///@param 
///@return 
bool CServerGameDesk::SetAIToApplyList(int station)
{
	if (NULL == m_pUserInfo[station]||m_pUserInfo[station]->m_UserData.isVirtual<=0)
	{
		return false;
	}
	bool sucess = false;
	if(station<0)
		return false;
	if(m_iNowNtStation == -1)//��ׯ
	{
		m_iNowNtStation = station;
		sucess = true;
		if(m_iCurrentMessage == ASS_WU_ZHUANG)//��ׯ�ȴ�״̬
		{
			
		}
		OutputDebugString("��������ׯ��������������ׯ-��ׯ��Ϊ��ǰׯ��");
	}
	else
	{
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_iZhuangStation[i]<0)
			{
				m_iZhuangStation[i] = station;
				sucess = true;
				OutputDebugString("��������ׯ��������������ׯ-�ɹ��ŵ��б�");
				break;
			}
		}
	}
	//������ׯ��Ϣ
	SHANG_ZHUANG szhuang;
	szhuang.shang = true;
	szhuang.station = station;
	szhuang.success = sucess;	
	if(	m_bGameStation == GS_SEND_CARD)
	{
		szhuang.bXiaZhu = true;   //�Ƿ���ע״̬
	}
	if(sucess)//�ɹ��������
	{
		MoveData(0);//��������
		for(int i=0;i<=PLAY_COUNT;i++)
		{
			szhuang.zhuangstation[i] = m_iZhuangStation[i];
		}
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL == m_pUserInfo[i])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
			{
				continue;
			}
			SendGameData(i,&szhuang,sizeof(SHANG_ZHUANG),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//���ͳɹ���Ϣ
		}
		OutputDebugString("wysout����˻�������ׯ��������������ׯ-�ɹ�");
		return true;
	}
		OutputDebugString("wysout����˻�������ׯ��������������ׯ-ʧ��");
	return false;
}

///���ƣ�GetQuYuCanNote
///��������ȡÿ�����������ע
///@param 
///@return 
void CServerGameDesk::GetQuYuCanNote()
{
	__int64 money = 0;
	__int64 note = 0;
	for(int i=0;i<8;i++)
	{
		note += m_iQuYuZhu[i]; 		/**< ����ÿ�������µ�ע��		*/
		money += m_iMaxZhu[i];  	/**< ����ÿ������������µ�ע�� */
	}
	if(m_iNowNtStation>=0 && m_iNowNtStation <PLAY_COUNT && NULL != m_pUserInfo[m_iNowNtStation] && note<=0 && money<=0)
	{
		m_iMaxZhu[0]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;			//����ÿ������������µ�ע��
		m_iMaxZhu[1]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/2;		//����ÿ������������µ�ע��
		m_iMaxZhu[2]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/11;		//����ÿ������������µ�ע��
		m_iMaxZhu[3]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;			//����ÿ������������µ�ע��
		m_iMaxZhu[4]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/2;		//����ÿ������������µ�ע��
		m_iMaxZhu[5]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/11;		//����ÿ������������µ�ע��
		m_iMaxZhu[6]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/8;		//����ÿ������������µ�ע��
		m_iMaxZhu[7]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/32;		//����ÿ������������µ�ע��
	}
}

///�ж���ע����ע�Ƿ�����
bool CServerGameDesk::JudgeXiaZhuIsFull(void)
{
	if(!m_bCanNote)
	{
		return false;
	}

	//����С����
	__int64 i64LeastChouma = 0;
	i64LeastChouma = 100;

	///���������Ѵﵽ�����ע
	int iTemp = 0;
	for(int i = 0; i < 8; ++i)
	{
		if(m_iMaxZhu[i] != 0 && m_iQuYuZhu[i]  + i64LeastChouma > m_iMaxZhu[i])
		{
			iTemp++;
		}
	}

	if(8 == iTemp)
	{
		return true;
	}
	
	return false;	
}

///������ע������Ϣ
void CServerGameDesk::SendXiaZhuFullNews(void)
{	
	if(m_iZongTime > m_iKaiPaiTime + m_iFreeTime + 3 + m_iCtrTime)	//����׼�����ƽ׶βŷ�����Ϣ
	{
		//������Ϣ
		for(int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL != m_pUserInfo[i])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
			{
				SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY, ASS_XIA_ZHU_FULL,0);
			}	
		}

		m_iSYTime = m_iCtrTime;
		m_iZongTime =  m_iKaiPaiTime + m_iFreeTime + 3 + m_iCtrTime;	//��Ϸ��ʱ��  ��ǰ5�벻����ע
	}
}

//���������Ƿ���˫����ע
BOOL  CServerGameDesk::CanDouble(BYTE bDeskStation)
{
	//�Ƿ�Ƿ���������
	if (bDeskStation >= PLAY_COUNT||bDeskStation<0)
		return FALSE;

	if (NULL == m_pUserInfo[bDeskStation])
	{
		return FALSE;
	}

	//�Ƿ�����������VIP��ע˫��
	if (!m_bIsVipDouble)
	{
		return FALSE;
	}

	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());
	int curtime=atoi(stime);

	return curtime < m_pUserInfo[bDeskStation]->m_UserData.iVipTime?TRUE:FALSE;

}
//ͳ�ƻ�������ׯ����
int CServerGameDesk::CountRobortNum()
{
	int iCount = 0;
	for (int i = 0;i<PLAY_COUNT;i++)
	{		
		if(m_iZhuangStation[i]<0)
		{
			continue;
		}
		if (NULL == m_pUserInfo[m_iZhuangStation[i]])
		{
			continue;
		}
		if (m_pUserInfo[m_iZhuangStation[i]]->m_UserData.isVirtual>0)
		{
			iCount++;
		}
	}
	return iCount;	
}
BOOL CServerGameDesk::IsHaveUserInList()
{
	for (int i = 0;i<PLAY_COUNT;i++)
	{
		if (m_iZhuangStation[i]<0)
		{
			continue;
		}
		//��Ҳ�����
		if (NULL == m_pUserInfo[m_iZhuangStation[i]])
		{
			continue;
		}
		//��ǰ��ׯ�Ҳ�������ׯ���б���
		if (m_iNowNtStation == m_iZhuangStation[i])
		{
			continue;
		}
		//���Ϊ�ǻ�����
		if (0 == m_pUserInfo[m_iZhuangStation[i]]->m_UserData.isVirtual)
		{
			return TRUE;
		}
	}
	return FALSE;
}
//����ׯ��
void CServerGameDesk::ClearNt()
{
	if(m_pUserInfo[m_iNowNtStation]==NULL || m_bXiaZhuang || m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money< m_iShangZhuangLimit
		|| m_iZhuangBaShu>=m_iMaxZhuang
		|| m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money>1500000000000000000)//��ǰׯ������������ׯ��
	{
		m_iNowNtStation=-1;//���õ�ǰׯ��Ϊ-1	
		m_iNtWin=0;	//��ǰׯ��Ӯ�Ľ��
		m_iZhuangBaShu=0;//ׯ�ҽ����˼���
		m_bXiaZhuang=false;
	}
}
//����
void CServerGameDesk::MatchCard(WORD (*wCardList)[3])
{
	try
	{
		//����Ƿ�����
		CString s = CINIFile::GetAppPath ();/////����·��
		CINIFile f( s +SKIN_FOLDER +"_s.ini");
		//��Ϸ������Ϣ
		CString key = TEXT("match");
		m_iIsMatch=f.GetKeyVal(key,"IsMatch",0);									//�Ƿ�����	
		CString pri;
		if(m_iIsMatch)
		{
			for(int i=0;i<2;i++)
			{
				for(int index=0;index<3;index++)
				{
					pri.Empty();
					pri.Format("MatchCard[%d][%d]",i,index);
					wCardList[i][index]=f.GetKeyVal(key,pri,0);	//�������
				}
			}     
		}
	}
	catch (...)
	{
		;
	}
}

//�Զ���ȡ�����ļ���ز���
#ifdef AUTOLOADINI
//�Զ����������ļ� 
void CServerGameDesk::AutoLoadIni(int iRoomID)
{
	LoadModifiedIni();

	LoadModifiedIni(iRoomID);
}	

//�����޸ĵ������ļ� 
void CServerGameDesk::LoadModifiedIni()
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("game");

	bool bTemp = f.GetKeyVal(szSec,"bHaveModified",0);

	if(!bTemp)
	{
		return;
	}

	f.SetKeyValString(szSec,"bHaveModified","0");	//��������Ϊ0

	__int64 i64Temp = 0;
	int iTemp = 0;
	bTemp = false;

	///��������Ӯ�Զ�����
	bTemp = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",m_bAIWinAndLostAutoCtrl);///�Ƿ�����������Ӯ�Զ�����
	m_bAIWinAndLostAutoCtrl = m_bAIWinAndLostAutoCtrl == bTemp?m_bAIWinAndLostAutoCtrl:bTemp;

	///������ҪӮ��Ǯ
	i64Temp = f.GetKeyVal(szSec,"AIWantWinMoney",m_iAIWantWinMoney);			
	if(m_iAIWantWinMoney != i64Temp)
	{
		m_iAIHaveWinMoney = 0;
		m_iAIWantWinMoney = i64Temp;
	}

	///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	
	i64Temp = f.GetKeyVal(szSec,"AIMaxWinInOne",m_iAIMaxWinInOne);	
	m_iAIMaxWinInOne = m_iAIMaxWinInOne == i64Temp?m_iAIMaxWinInOne:i64Temp;

	// ��ȡ��������ׯ�������
	iTemp = f.GetKeyVal(szSec, "RobotSZCount", m_iRobotSZCount);
	m_iRobotSZCount = m_iRobotSZCount == iTemp?m_iRobotSZCount:iTemp;
	if (m_iRobotSZCount < 1)
	{
		m_iRobotSZCount = 15;
	}
}

//���ݷ�������޸ĵ������ļ�
void CServerGameDesk::LoadModifiedIni(int iRoomID)
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.INI"));
	TCHAR szSec[_MAX_PATH] = TEXT("game");
	_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,iRoomID);

	bool bTemp = f.GetKeyVal(szSec,"bHaveModified",0);

	if(!bTemp)
	{
		return;
	}

	f.SetKeyValString(szSec,"bHaveModified","0");	//��������Ϊ0

	__int64 i64Temp = 0;
	int iTemp = 0;
	bTemp = false;

	///��������Ӯ�Զ�����
	bTemp = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",m_bAIWinAndLostAutoCtrl);///�Ƿ�����������Ӯ�Զ�����
	m_bAIWinAndLostAutoCtrl = m_bAIWinAndLostAutoCtrl == bTemp?m_bAIWinAndLostAutoCtrl:bTemp;

	///������ҪӮ��Ǯ
	i64Temp = f.GetKeyVal(szSec,"AIWantWinMoney",m_iAIWantWinMoney);			
	if(m_iAIWantWinMoney != i64Temp)
	{
		m_iAIHaveWinMoney = 0;
		m_iAIWantWinMoney = i64Temp;
	}

	///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	
	i64Temp = f.GetKeyVal(szSec,"AIMaxWinInOne",m_iAIMaxWinInOne);	
	m_iAIMaxWinInOne = m_iAIMaxWinInOne == i64Temp?m_iAIMaxWinInOne:i64Temp;

	// ��ȡ��������ׯ�������
	iTemp = f.GetKeyVal(szSec, "RobotSZCount", m_iRobotSZCount);
	m_iRobotSZCount = m_iRobotSZCount == iTemp?m_iRobotSZCount:iTemp;
	if (m_iRobotSZCount < 1)
	{
		m_iRobotSZCount = 15;
	}
}
#endif
//---------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//�޸Ľ�������1(ƽ̨->��Ϸ)
/// @param bAIWinAndLostAutoCtrl �����˿�����Ӯ����
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return true;
}

//�޸Ľ�������2(ƽ̨->��Ϸ)
/// @param m_iAIWantWinMoney ��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ�
/// @param m_iAIWinLuckyAt ��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ���
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return true;
}

//�޸Ľ�������3(ƽ̨->��Ϸ)
/// @param iReSetAIHaveWinMoney ��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
/// @return �Ƿ�ɹ�
bool CServerGameDesk::SetRoomPondEx(__int64	iReSetAIHaveWinMoney)
{
	return true;
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
	return TRUE;
}
//================================================
//�ж�ʤ��
//================================================
BOOL CServerGameDesk::JudgeWiner()
{
	return true;
}
//GRM����
//���½�������(��Ϸ->ƽ̨)
/// @return ������ӮǮ��
void CServerGameDesk::UpDataRoomPond(__int64 iAIHaveWinMoney)
{

}