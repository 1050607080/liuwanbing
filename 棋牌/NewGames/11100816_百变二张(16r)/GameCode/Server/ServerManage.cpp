#include "StdAfx.h"
#include "ServerManage.h"
#include <shlobj.h>

/** ��Ϸ��չ���� */
BYTE CServerGameDesk:: m_iAllCardCount; 

/*
* ��ӡ��־�ļ�
*/
void CServerGameDesk::DebugPrintf2(int iRoomID, const char *p, ...)
{
	if(!m_bWriteLog)
	{
		return ; 
	}
	char szFilename[1024];
	CString strPath = CINIFile::GetAppPath() +"\\GameLog\\";

	SHCreateDirectoryEx(NULL, strPath, NULL);

	CTime time = CTime::GetCurrentTime();

	sprintf(szFilename, "%s%d_%d-%d%d%d%d-log.txt",
		strPath , NAME_ID, iRoomID, time.GetYear(), time.GetMonth(), time.GetDay(),time.GetHour());
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

/**
* ��ӡ���
*/
void CServerGameDesk::PrintEnding()
{
	try
	{
		CTime time = CTime::GetCurrentTime();
		char szPath[MAX_PATH], szFilename[MAX_PATH];

		// -------------- �洢·�� --------------
		wsprintf(szPath, "D:\\GamePrint\\%d", NAME_ID);

		// �����ļ��� ��Ҫ����#include <shlobj.h>
		SHCreateDirectoryEx(NULL, szPath, NULL);
		// --------------------------------------

		// �ļ�����
		wsprintf(szFilename, "%s\\%d(%d�ŷ���%d��) %d-%d-%d %d��%d��%d��.txt", 
			szPath, NAME_ID, m_pDataManage->m_InitData.uRoomID, m_chGameBeen, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		CFile file;
		file.Open(szFilename, CFile::modeCreate | CFile::modeReadWrite);

		TCHAR chKind[5][8] = {"����", "÷��", "����", "����", ""};
		TCHAR chNumber[15][8] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "С��","����"};

		BYTE bCard1, bCard2;         // ��
		BYTE bKind1, bKind2;         // ��ɫ
		BYTE bNumber1, bNumber2;     // �Ƶ�

		TCHAR msg[MAX_PATH];

		char i = 0, j = 0;
		for (i = 0; i < GAME_XIPAI_COUNT; i++)
		{
			wsprintf(msg, "---------------- ��%d�� ----------------\r\n", i + 1);
			file.Write(msg, strlen(msg));

			for (j = 0; j < 4; j++)
			{
				// ��һ����
				bCard1 = m_gameLogic.GetValue(m_gameLogic.m_GameCard[i][j].paiAry[0]); // ת����
				bKind1 = m_gameLogic.GetShape(bCard1);   // ת���ƻ�ɫ
				bNumber1 = m_gameLogic.GetNumber(bCard1);  // ת���Ƶ�

				// �ڶ�����
				bCard2 = m_gameLogic.GetValue(m_gameLogic.m_GameCard[i][j].paiAry[1]); // ת����
				bKind2 = m_gameLogic.GetShape(bCard2);   // ת���ƻ�ɫ
				bNumber2 = m_gameLogic.GetNumber(bCard2);  // ת���Ƶ�

				// ��ӡÿ��λ�õ�������
				wsprintf(msg, "λ��%d: %s%s, %s%s\r\n", j, chKind[bKind1], chNumber[bNumber1], chKind[bKind2], chNumber[bNumber2]);

				// д���ļ���
				file.Write(msg, strlen(msg));
			}

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
/**
* ���캯��
*/
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{			
	m_bGameStation = GS_WAIT_SETGAME;

	m_iTime = 0;

	m_iNtTax = 0;

	m_bNTGameMode = FALSE;	

	m_bUseCard = TRUE;

	m_bKingAndThreeBig = TRUE;

	m_bGameVersion = GAME_VERSION_1;

	m_chGameCount = 0;

	m_chGameBeen = 0;

	memset(m_bInNoteStage , 0 , sizeof(m_bInNoteStage)) ; 
	// ��¼��ǰʱ��
	m_RecordTime = CTime::GetCurrentTime();

	// ������ׯ������б�
	m_ArrUserList.RemoveAll();

	// ���Ƽ�¼
	m_GameKPInfo.RemoveAll();

	// ��Ϸ·�Ӽ�¼
	m_GameLizuInfo.RemoveAll();

	// ÿ�������ע��¼
	for (BYTE i = 0; i < PLAY_COUNT; i++)
	{
		m_GameXZInfo[i].RemoveAll();
	}	

	// ��ǰׯ����ׯ����
	m_iZhuangCount = 0;

	// ׯ�������ׯ����    
	m_iZhuangMaxNum = 0;

	// ��ǰׯ��
	m_iZhuangStation = -1;

	// ������ׯ����
	m_iApplyMoneyLimit = 0;

	// ׯ����Ӯ�ɼ�  
	m_iZhuangResult = 0;

	// �Ƿ�����������Ӯ�Զ�����
	m_bAIWinAndLostAutoCtrl = false; 

	//������ӮǮ��
	G_i64AIHaveWinMoney = 0;		 
	//��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ
	G_i64ReSetAIHaveWinMoney = 0;	
	m_i64AIWantWinMoneyA1 = 100000000;		/**<��������Ӯ���ƣ�������ӮǮ����1  */
	m_i64AIWantWinMoneyA2 = 1000000000;		/**<��������Ӯ���ƣ�������ӮǮ����2  */
	m_i64AIWantWinMoneyA3 = 1000000000;		/**<��������Ӯ���ƣ�������ӮǮ����3  */
	m_iAIWinLuckyAtA1 = 100;			/**<��������Ӯ���ƣ�������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2 = 100;			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3 = 100;			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4 = 100;			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */

	// ������ҪӮ��Ǯ
	m_iAIWantWinMoney = 0;		     

	// �������Ѿ�Ӯ�˶���Ǯ
	m_iAIHaveWinMoney = 0;		     

	// ������ƽ��ÿ�����ӮǮ����С�ڵ���0ʱΪ���ޣ�
	m_iAIMaxWinInOne = 0;		     

	// ��ǰ�������Ѿ��������Զ����Ƶİ���
	m_iAICtrlCount = 0;			     

	//VIP ����Ƿ����˫����ע
	m_bIsVipDouble = false;

	m_bWriteLog = false ;  

	m_bIsRunning = false;


	// ÿ����ҵ���Ӯ�ɼ�
	memset(m_iUserResult, 0, sizeof(m_iUserResult));
	//�û�����״̬
	memset(m_bUserNetStation,0,sizeof(m_bUserNetStation)); 

	memset(m_iUserNoteZeroCount,0,sizeof(m_iUserNoteZeroCount)) ; 

	memset(m_iRectNoteMax , 0 , sizeof(m_iRectNoteMax)) ; 

	// ���ݳ�ʼ��
	InitData();

	// ���������ļ�
	LoadIni();

	// ������Ϸģʽ
	m_gameLogic.SetGameMode(m_bGameVersion, m_bUseCard, m_bKingAndThreeBig);
}

/**
* ���ݳ�ʼ��
*/
void CServerGameDesk::InitData()
{
	// ��ʼ����Ϸ�߼�
	m_gameLogic.InitGame();

	m_UserXiaZhu.Init();

	m_bSendCardPos = 0;

	m_iAllMoney = 0;

	m_bUserXZ = FALSE;	

	// ���û�����״̬
	memset(m_bIsRobot, 0, sizeof(m_bIsRobot));
	//�û�����״̬
	memset(m_bUserNetStation,0,sizeof(m_bUserNetStation)); 

	// --------- �����û����� ---------
	m_bSuperSetting = FALSE;

	memset(m_bSuperState, 0, sizeof(m_bSuperState));
	// --------- �����û����� ---------
}


/**
* ���������ļ�
*
* @return ���ز�������
*/
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath (); //����·��
	CINIFile f (s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString key = TEXT("game");

	m_bStartGameWithOutNt = f.GetKeyVal(key,"StartGameWithOutNt",1);	/* ��ׯ�Ƿ�ʼ��Ϸ;true:��ʼ��Ϸ;false:����ʼ��Ϸ */

	//VIP ���˫����ע���
	m_bIsVipDouble = f.GetKeyVal(key, "IsVipDouble", 0);

	//��Ϸ��ע
	//m_iLimitNote = f.GetKeyVal(key, "limitnote", 1000);
	m_iBaseNote = f.GetKeyVal(key, "basenote", 10);
	if (m_iBaseNote < 1)
	{
		m_iBaseNote = 1;
	}
	//������ׯ����
	m_iApplyMoneyLimit = f.GetKeyVal(key, "ApplyMoneyLimit", 10000);
	if(m_iApplyMoneyLimit<1000)
	{
		m_iApplyMoneyLimit=1000;
	}
	m_iLimitNote = m_iApplyMoneyLimit;                                             

	//��עʱ��
	m_iXiaZhuTime = f.GetKeyVal(key, "time_xiazhu", 15);
	if(m_iXiaZhuTime<10)
	{
		m_iXiaZhuTime=10;
	}
	//����ʱ��
	m_iKaipaiTime = f.GetKeyVal(key, "time_kaipai", 15);

	//if(m_iKaipaiTime<25)
	//{
	//	m_iKaipaiTime=25;
	//}
	//����ʱ��
	m_iEmptyTime = f.GetKeyVal(key, "time_empty", 5);
	//if(m_iEmptyTime<10)
	//{
	//	m_iEmptyTime=10;
	//}
	//�����ׯ����
	m_iMaxZhuangCount = f.GetKeyVal(key, "MaxZhuangCount", 30);
	if(m_iMaxZhuangCount<1)
	{
		m_iMaxZhuangCount=5;
	}
	m_iZhuangMaxNum = m_iMaxZhuangCount;

	//��С��ׯ����
	m_iMinZhuangCount = f.GetKeyVal(key, "MinZhuangCount", 15);
	if(m_iMinZhuangCount<1)
	{
		m_iMinZhuangCount=1;
	}
	// ��ȡʹ�ô������Ǻ�A
	m_bUseCard = f.GetKeyVal(key, "UseCard", 1);

	// ��ȡ��Ϸ���Ǵ������3��󣬻���˫��Q���
	m_bKingAndThreeBig = f.GetKeyVal(key, "KingAndThreeBig", 1);

	// ��ȡ��Ϸ���Ͱ汾 0: ��ͨ 1: ������ 2: ���«2��
	m_bGameVersion = f.GetKeyVal(key, "GameVersion", 0);

	/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
	m_bRedJOrBlackJ = f.GetKeyVal(key, "RedJOrBlackJ", 0);   

	// ��ȡ��˰��ʽ
	m_iNtTax = f.GetKeyVal(key, "NtTax", 0);

	// ��������ע��
	m_iMaxNote = f.GetKeyVal(key, "MaxNote", -1);

	// ��ȡ��������ׯ�������
	m_iRobotSZCount = f.GetKeyVal(key, "RobotSZCount", 15);

	if (m_iRobotSZCount < 1)
	{
		m_iRobotSZCount = 5;
	}

	CString strValueName ; 
	for(int i = 0 ; i<REGION_COUNT ; i++)
	{
		strValueName.Format("RectNoteMax[%d]",i) ; 
		m_iRectNoteMax[i] = f.GetKeyVal(key , strValueName , 0) ; 
	}

	m_bWriteLog = f.GetKeyVal(key , "WriteLog" , 0);


	// -------------��������Ӯ�Զ�����--------------------
	m_bAIWinAndLostAutoCtrl		= f.GetKeyVal(key,"AIWinAndLostAutoCtrl", 0);				/**<�Ƿ�����������Ӯ�Զ�����*/
	G_i64AIHaveWinMoney			= f.GetKeyVal(key,"AIHaveWinMoney",__int64(0));				/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */
	G_i64ReSetAIHaveWinMoney	= f.GetKeyVal(key,"ReSetAIHaveWinMoney ",__int64(0));		/**<��¼���û�����ӮǮ���������Ϸ�����иı��˾�Ҫ�ı������ӮǮ��*/

	m_i64AIWantWinMoneyA1	= f.GetKeyVal(key,"AIWantWinMoneyA1 ",__int64(500000));			/**<������ӮǮ����1  */
	m_i64AIWantWinMoneyA2	= f.GetKeyVal(key,"AIWantWinMoneyA2 ",__int64(5000000));		/**<������ӮǮ����2  */
	m_i64AIWantWinMoneyA3	= f.GetKeyVal(key,"AIWantWinMoneyA3 ",__int64(50000000));		/**<������ӮǮ����3  */

	m_iAIWinLuckyAtA1	= f.GetKeyVal(key,"AIWinLuckyAtA1 ",90);				/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(key,"AIWinLuckyAtA2 ",70);				/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(key,"AIWinLuckyAtA3 ",50);				/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(key,"AIWinLuckyAtA4 ",30);				/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */


	m_iAIWantWinMoney = f.GetKeyVal(key, "AIWantWinMoney", 0);		       // ������ҪӮ��Ǯ
	m_iAIMaxWinInOne = f.GetKeyVal(key, "AIMaxWinInOne", 0);		       // ������ƽ��ÿ�����ӮǮ����С�ڵ���0ʱΪ���ޣ�


	key = TEXT("other");
	m_iSendCardTime = f.GetKeyVal(key, "sendcardtime", 500);
	m_iBeginTime = f.GetKeyVal(key, "begintime", 5);
	m_iThinkTime = f.GetKeyVal(key, "thinktime", 20);

	return true;
}

/**
* ���ݷ���ID���������ļ�
*
* @param [in] iRoomID ����ID
*
* @return ���ز�������
*/
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	TCHAR szKey[20];
	wsprintf(szKey,"%s_%d",GET_SKIN_FOLDER(szTempStr), iRoomID);

	CString s = CINIFile::GetAppPath (); //����·��
	CINIFile f (s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString key = szKey;

	m_bWriteLog = f.GetKeyVal(key , "WriteLog" , m_bWriteLog);
	//VIP ���˫����ע���
	m_bIsVipDouble = f.GetKeyVal(key, "IsVipDouble", m_bIsVipDouble);
	//m_iLimitNote = f.GetKeyVal(key, "limitnote", m_iLimitNote);
	m_bStartGameWithOutNt = f.GetKeyVal(key,"StartGameWithOutNt",m_bStartGameWithOutNt);	/* ��ׯ�Ƿ�ʼ��Ϸ;true:��ʼ��Ϸ;false:����ʼ��Ϸ */
	m_iBaseNote = f.GetKeyVal(key,"basenote", m_iBaseNote);
	if (m_iBaseNote < 1)
	{
		m_iBaseNote = 1;
	}
	//������ׯ����
	m_iApplyMoneyLimit = f.GetKeyVal(key, "ApplyMoneyLimit", m_iApplyMoneyLimit);
	if (m_iApplyMoneyLimit < 1000)
	{
		m_iApplyMoneyLimit = 1000;
	}
	m_iLimitNote = m_iApplyMoneyLimit;

	//��עʱ��
	m_iXiaZhuTime = f.GetKeyVal(key, "time_xiazhu", m_iXiaZhuTime);
	if (m_iXiaZhuTime < 10)
	{
		m_iXiaZhuTime = 10;
	}
	//����ʱ��
	m_iKaipaiTime = f.GetKeyVal(key, "time_kaipai", m_iKaipaiTime);
	//if (m_iKaipaiTime < 25)
	//{
	//	m_iKaipaiTime = 25;
	//}
	//����ʱ��
	m_iEmptyTime = f.GetKeyVal(key, "time_empty", m_iEmptyTime);
	//if (m_iEmptyTime < 10)
	//{
	//	m_iEmptyTime = 10;
	//}
	//�����ׯ����
	m_iMaxZhuangCount = f.GetKeyVal(key, "MaxZhuangCount", m_iMaxZhuangCount);
	if (m_iMaxZhuangCount < 1)
	{
		m_iMaxZhuangCount = 5;
	}
	m_iZhuangMaxNum = m_iMaxZhuangCount;

	//��С��ׯ����
	m_iMinZhuangCount = f.GetKeyVal(key, "MinZhuangCount", m_iMinZhuangCount);
	if (m_iMinZhuangCount < 1)
	{
		m_iMinZhuangCount = 1;
	}
	// ��ȡ��˰��ʽ
	m_iNtTax = f.GetKeyVal(key, "NtTax", m_iNtTax);

	// ��������ע��
	m_iMaxNote = f.GetKeyVal(key, "MaxNote", m_iMaxNote);

	// ��ȡ��������ׯ�������
	m_iRobotSZCount = f.GetKeyVal(key, "RobotSZCount", m_iRobotSZCount);

	if (m_iRobotSZCount < 1)
	{
		m_iRobotSZCount = 5;
	}

	CString strValueName ; 
	for(int i = 0 ; i<REGION_COUNT ; i++)
	{
		strValueName.Format("RectNoteMax[%d]",i) ; 
		m_iRectNoteMax[i] = f.GetKeyVal(key,strValueName ,m_iRectNoteMax[i]) ; 
	}
	m_bStartGameWithOutNt = f.GetKeyVal(key,"StartGameWithOutNt",m_bStartGameWithOutNt);	/* ��ׯ�Ƿ�ʼ��Ϸ;true:��ʼ��Ϸ;false:����ʼ��Ϸ */

	// -------------��������Ӯ�Զ�����--------------------
	m_bAIWinAndLostAutoCtrl		= f.GetKeyVal(key,"AIWinAndLostAutoCtrl",m_bAIWinAndLostAutoCtrl);		// �Ƿ�����������Ӯ�Զ�����
	G_i64AIHaveWinMoney			= f.GetKeyVal(key,"AIHaveWinMoney",G_i64AIHaveWinMoney);				/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */
	G_i64ReSetAIHaveWinMoney	= f.GetKeyVal(key,"ReSetAIHaveWinMoney ",G_i64ReSetAIHaveWinMoney);		/**<��¼���û�����ӮǮ���������Ϸ�����иı��˾�Ҫ�ı������ӮǮ��*/

	m_i64AIWantWinMoneyA1	= f.GetKeyVal(key,"AIWantWinMoneyA1 ",m_i64AIWantWinMoneyA1);			/**<������ӮǮ����1  */
	m_i64AIWantWinMoneyA2	= f.GetKeyVal(key,"AIWantWinMoneyA2 ",m_i64AIWantWinMoneyA2);			/**<������ӮǮ����2  */
	m_i64AIWantWinMoneyA3	= f.GetKeyVal(key,"AIWantWinMoneyA3 ",m_i64AIWantWinMoneyA3);			/**<������ӮǮ����3  */

	m_iAIWinLuckyAtA1	= f.GetKeyVal(key,"AIWinLuckyAtA1 ",m_iAIWinLuckyAtA1);				/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(key,"AIWinLuckyAtA2 ",m_iAIWinLuckyAtA2);				/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(key,"AIWinLuckyAtA3 ",m_iAIWinLuckyAtA3);				/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(key,"AIWinLuckyAtA4 ",m_iAIWinLuckyAtA4);				/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */

	return TRUE;
}

/**
* ��������
*/
CServerGameDesk::~CServerGameDesk(void)
{
	m_ArrUserList.RemoveAll();
}

/**
* ��ʱ����Ϣ
*
* @param [in] uTimerID ��ʱ��ID
*
* @return ���ز�������
*/
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	
	switch(uTimerID)
	{
	case  ID_WIAT_GAME_BEGIN:
		{
			DebugPrintf("��ׯģʽ�еȴ���ʱ��-ID_WIAT_GAME_BEGIN-��Ϸ״̬:%d", m_bGameStation);
			KillTimer(ID_WIAT_GAME_BEGIN) ; 
			SetTimer(ID_WIAT_GAME_BEGIN , 60*1000); 
			///�����������Ĵ���
			break;
		}
	case TIME_JIAO_ZHUANG:
		{
			m_iTime--;
			if(m_iTime<=0)
			{
				DebugPrintf("��ׯ��ʱ�� -TIME_JIAO_ZHUANG- ��Ϸ״̬��%d", m_bGameStation);
				KillTimer(TIME_JIAO_ZHUANG) ; 
				NotifyClientNote();
			}
			
			return true;
		}
	case  TIME_RAND_CARD:
		{
			DebugPrintf("֪ͨ�ͻ���ϴ�� -TIME_RAND_CARD- ��Ϸ״̬��%d, ��עʱ��:%d", m_bGameStation, m_iXiaZhuTime);
			SendGameData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_SEND_NOTE_MSG,0) ; 
			// ��ע��ʱ��
			SetGameTimer(TIME_XIAZHU, m_iXiaZhuTime);
			KillTimer(TIME_RAND_CARD); 
			return true ;  
		}
	case TIME_XIAZHU:      // ��ע��ʱ��
		{
			m_iTime--;
			if (m_iTime <= 0)
			{
				KillTimer(TIME_XIAZHU);
				DebugPrintf("��ע��ʱ�� -TIME_XIAZHU- ��Ϸ״̬��%d", m_bGameStation);

				if (m_bGameStation == GS_XIAZHU) 
				{
					DebugPrintf("��ע��ʱ������ ����OnUserXiaZhuFinish()��ע��� ---TIME_XIAZHU---��Ϸ״̬��%d", m_bGameStation);
					// ��ע���
					OnUserXiaZhuFinish();
				}
			}
			return true;
		}
	case TIME_SEND_CARD:   // ���ƶ�ʱ��
		{
			KillTimer(TIME_SEND_CARD);
			DebugPrintf("���ƶ�ʱ�� -TIME_SEND_CARD- ��Ϸ״̬��%d", m_bGameStation);
			if (m_bGameStation == GS_XIAZHU)
			{
				DebugPrintf("���ƶ�ʱ�� -TIME_SEND_CARD- ����FaPaiToAllUser() - ��Ϸ״̬��%d", m_bGameStation);
				// ����
				FaPaiToAllUser();
			}
			return true;
		}
	case TIME_GAME_FINISH: // ��Ϸ����
		{
			m_iTime--;

			if (m_iTime <= 0)
			{
				DebugPrintf("��Ϸ����ʱ�� -TIME_GAME_FINISH -- ��Ϸ״̬��%d", m_bGameStation);
				KillTimer(TIME_GAME_FINISH);
				KillTimer(TIME_SEND_CARD);
				
				if (m_bGameStation == GS_PLAY_GAME)
				{
					DebugPrintf("��Ϸ����ʱ�� -TIME_GAME_FINISH -����GameFinish()- ��Ϸ״̬��%d", m_bGameStation);

					GameFinish(0, GF_NORMAL);
				}
			}

			return true;
		}
	case TIME_EMPTY:       // ����ʱ��
		{
			m_iTime--;


			//CTime time = CTime::GetCurrentTime();
			if (m_iTime <= 0)
			{
				KillTimer(TIME_EMPTY);

				if (m_bGameStation == GS_WAIT_NEXT || m_bGameStation == GS_WAIT_ARGEE || m_bGameStation == GS_WAIT_SETGAME) 
				{
					DebugPrintf("����ʱ�� -TIME_EMPTY -����StartGame()��ʼ��Ϸ- ��Ϸ״̬��%d", m_bGameStation);
					// ��ʼ��Ϸ
					StartGame();
				}
			}
			return true;
		}
	case  TIMT_APPLY_NT:
		{
			m_iTime--;
			if (m_iTime <= 0)
			{
				KillTimer(TIMT_APPLY_NT); 

				IsZhuangRule();

				if(m_iZhuangStation >=0)
				{
					DebugPrintf("��ׯʱ�� -TIMT_APPLY_NT -����GameXiPai()����- ��Ϸ״̬��%d", m_bGameStation);
					//����
					m_gameLogic.GameXiPai(FALSE);

					NotifyClientNote();
				}
				else
				{
					DebugPrintf("��ׯʱ�� -TIMT_APPLY_NT -��ǰ���� �� ����GameFinish()- ��Ϸ״̬��%d", m_bGameStation);
					GameFinish(0, GF_AHEAD); 
				}
			}

			return true; 
		}
	default:
		{
			break;
		}
	}
	return __super::OnTimer(uTimerID);
}

/**
* ��ȡ��Ϸ״̬��Ϣ
*
* @param [in] bDeskStation ���λ��
*
* @param [in] uSoketID Socket ID
*
* @param [in] bWatchUser �Թ�
*
* @return ���ز�������
*/
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	//DebugPrintf2(m_pDataManage->m_InitData.uRoomID, "------ ���Ӻ�%d ,��ҽ�����Ϸ����OnGetGameStation() ------" ,m_bDeskIndex);
	// ��֤�����û�Ȩ��
	AuthPermissions();
	//��������Ϸ��ʼ���������״̬û�����õ����������Ϸ�д�Ǯ�����ڴ���������
	if (IsPlayGame(bDeskStation))
	{
		m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_PLAY_GAME;//���������Ϊͬ��״̬
	}
	else
	{
		m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;	//��ҽ����ͱ�ʾͬ��
	}

	m_bUserNetStation[bDeskStation] = false ; 

	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		// ��Ϸû�п�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));

			// ��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						// ��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;

			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			// ��Ϸ������Ϣ����
			GameStation.iLimitNote = m_iLimitNote;							// ���ע(Ŀǰ����������ׯ����)
			GameStation.iBaseNote = m_iBaseNote;						    // ��ע		
			GameStation.iMaxNote = m_iMaxNote;                              // �м������ע��

			GameStation.iThinkTime = m_iThinkTime;
			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iAllCardCount = m_iAllCardCount;

			GameStation.iXiaZhuTime = m_iXiaZhuTime;                        // ��עʱ��
			GameStation.iKaipaiTime = m_iKaipaiTime;                        // ����ʱ��
			GameStation.iEmptyTime = m_iEmptyTime;                          // ����ʱ��

			GameStation.bIsApplyXZ = m_bUserXZ ;

			// ��ׯ�б�����
			memset(GameStation.userList, -1, sizeof(GameStation.userList));
			for (int i = 0; i < m_ArrUserList.GetSize(); i++)
			{
				GameStation.userList[i] = m_ArrUserList.GetAt(i);
			}
			
			// ���Ƽ�¼����
			GameStation.iKaiPaiCount = m_GameKPInfo.GetSize();   
			int iMaxCount = /*GAME_VERSION_3 == m_bGameVersion ? GAME_XIPAI_COUNT : */30;
			// �������ݼ�¼
			for (BYTE i = 0; i < GameStation.iKaiPaiCount && i < iMaxCount; i++)
			{
				// ���ֿ��Ƽ�¼ (0x01:˳�š�0x02:���š�0x04:����)
				GameStation.bIsKaiPai[i] = m_GameKPInfo.GetAt(i);  

				if (i < m_GameXZInfo[bDeskStation].GetSize())
				{
					// ������ע��¼(0x01:˳�š�0x02:���š�0x04:����)
					GameStation.bIsXiaZhu[i] = m_GameXZInfo[bDeskStation].GetAt(i);  
				}
			}

			GameStation.bUseCard = m_bUseCard;                              // ѡ��ʹ���� TRUE: ʹ�ú�A FALSE: ʹ�ô���
			GameStation.bGameMode = m_bGameVersion;                         // ��Ϸ�汾 ��Ϸ�汾 0: ��ͨ 1: ������ 2: ���«2��
			GameStation.bKingAndThreeBig = m_bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
			GameStation.bRedJOrBlackJ = m_bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
			if (CanDouble(bDeskStation))
			{
				GameStation.iMaxNote = m_iMaxNote*2;
			}
			GameStation.bIsVipDouble = m_bIsVipDouble;
			GameStation.iMaxBaseNote = m_iMaxNote;							//��ͨ��ҵ������ע��

//			GameStation.bNTStation = m_iZhuangStation;
			// ��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));

			return true;
		}
	case GS_WAIT_ARGEE:			// �ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));

			// ��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						// ��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			// ��Ϸ������Ϣ����
			GameStation.iLimitNote = m_iLimitNote;							// ���ע(Ŀǰ����������ׯ����)
			GameStation.iBaseNote = m_iBaseNote;							// ��ע
			GameStation.iMaxNote = m_iMaxNote;                              // �м������ע��

			GameStation.iThinkTime = m_iThinkTime;
			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iAllCardCount = m_iAllCardCount;

			GameStation.iXiaZhuTime = m_iXiaZhuTime;                        // ��עʱ��
			GameStation.iKaipaiTime = m_iKaipaiTime;                        // ����ʱ��
			GameStation.iEmptyTime = m_iEmptyTime;                          // ����ʱ��

			GameStation.bIsApplyXZ = m_bUserXZ ;

			// ��ׯ�б�����
			memset(GameStation.userList, -1, sizeof(GameStation.userList));
			for (int i = 0; i < m_ArrUserList.GetSize(); i++)
			{
				GameStation.userList[i] = m_ArrUserList.GetAt(i);
			}

			// ���Ƽ�¼����
			GameStation.iKaiPaiCount = m_GameKPInfo.GetSize();   

			// �ж���Ϸģʽ��������Ϸ��¼�������
			int iMaxCount = /*GAME_VERSION_3 == m_bGameVersion ? GAME_XIPAI_COUNT : */30;

			// �������ݼ�¼
			for (BYTE i = 0; i < GameStation.iKaiPaiCount && i < iMaxCount; i++)
			{
				// ���ֿ��Ƽ�¼ (0x01:˳�š�0x02:���š�0x04:����)
				GameStation.bIsKaiPai[i] = m_GameKPInfo.GetAt(i);  

				if (i < m_GameXZInfo[bDeskStation].GetSize())
				{
					// ������ע��¼(0x01:˳�š�0x02:���š�0x04:����)
					GameStation.bIsXiaZhu[i] = m_GameXZInfo[bDeskStation].GetAt(i);  
				}
			}


			GameStation.bGameMode = m_bGameVersion; 
			GameStation.bUseCard = m_bUseCard;								/**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */
			GameStation.bKingAndThreeBig = m_bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
			GameStation.bRedJOrBlackJ = m_bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
			if (CanDouble(bDeskStation))
			{
				GameStation.iMaxNote = m_iMaxNote*2;
			}
			GameStation.bIsVipDouble = m_bIsVipDouble;
			GameStation.iMaxBaseNote = m_iMaxNote;							//��ͨ��ҵ������ע��

			GameStation.bNTStation = m_iZhuangStation;
			// ��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));

			return true;
		}
	case GS_XIAZHU:     // ��ע״̬
		{
			GameStation_3 GameStation;
			::memset(&GameStation, 0, sizeof(GameStation));

			// ��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						// ��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			// ��Ϸ������Ϣ����
			GameStation.iLimitNote = m_iLimitNote;							// ���ע(Ŀǰ����������ׯ����)
			GameStation.iBaseNote = m_iBaseNote;							// ��ע
			GameStation.iMaxNote = m_iMaxNote;                              // �м������ע��

			GameStation.iThinkTime = m_iThinkTime;
			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iAllCardCount = m_iAllCardCount;

			GameStation.iXiaZhuTime = m_iXiaZhuTime;                        // ��עʱ��
			GameStation.iKaipaiTime = m_iKaipaiTime;                        // ����ʱ��
			GameStation.iEmptyTime = m_iEmptyTime;                          // ����ʱ��

			GameStation.iTime = m_iTime;                                    // ʣ��ʱ��

			GameStation.bUseCard = m_bUseCard;                              // ѡ��ʹ���� TRUE: ʹ�ú�A FALSE: ʹ�ô���
			GameStation.bGameMode = m_bGameVersion;                         // ��Ϸ�汾 ��Ϸ�汾 0: ��ͨ 1: ������ 2: ���«2��
			GameStation.bKingAndThreeBig = m_bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
			GameStation.bRedJOrBlackJ = m_bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
			GameStation.chGameCount = m_chGameCount;                        // ��Ϸ����
			GameStation.chGameBeen = m_chGameBeen;                          // ��Ϸ����

			GameStation.iUpGradePeople = m_iZhuangStation;
			GameStation.bIsApplyXZ = m_bUserXZ;
			GameStation.bIsInNoteStage = m_bInNoteStage[bDeskStation] ; 
			GameStation.iBeenPlayGame = m_iZhuangCount;
			GameStation.iZhuangMoenyResult = m_iZhuangResult;
			GameStation.iSelfMoenyResult = m_iUserResult[bDeskStation];

			// ������ע����
			::CopyMemory(GameStation.iMyZhu, m_UserXiaZhu.iXiaZhu[bDeskStation], sizeof(m_UserXiaZhu.iXiaZhu[bDeskStation]));

			// ��������ע����
			::CopyMemory(GameStation.iRobotZhu, m_UserXiaZhu.iRobotZhu, sizeof(m_UserXiaZhu.iRobotZhu));

			// ��ׯ�б�����
			memset(GameStation.userList, -1, sizeof(GameStation.userList));
			for (int i = 0; i < m_ArrUserList.GetSize(); i++)
			{
				GameStation.userList[i] = m_ArrUserList.GetAt(i);
			}

			// �����������ע����
			::CopyMemory(&GameStation.iXiaZhuInfo, &m_UserXiaZhu.iDeskInfo, sizeof(m_UserXiaZhu.iDeskInfo));

			// ���Ƽ�¼����
			GameStation.iKaiPaiCount = m_GameKPInfo.GetSize();   

			// �ж���Ϸģʽ��������Ϸ��¼�������
			int iMaxCount = /*GAME_VERSION_3 == m_bGameVersion ? GAME_XIPAI_COUNT : */30;

			// �������ݼ�¼
			for (BYTE i = 0; i < GameStation.iKaiPaiCount && i < iMaxCount; i++)
			{
				// ���ֿ��Ƽ�¼ (0x01:˳�š�0x02:���š�0x04:����)
				GameStation.bIsKaiPai[i] = m_GameKPInfo.GetAt(i);  

				if (i < m_GameXZInfo[bDeskStation].GetSize())
				{
					// ������ע��¼(0x01:˳�š�0x02:���š�0x04:����)
					GameStation.bIsXiaZhu[i] = m_GameXZInfo[bDeskStation].GetAt(i);  
				}
			}

			// ·������
			GameStation.iLuziCount = m_GameLizuInfo.GetSize();      
			for (BYTE i = 0; i < GameStation.iLuziCount && i < GAME_XIPAI_COUNT; i++)
			{
				// ·������
				GameStation.ArrLuzi[i] = m_GameLizuInfo.GetAt(i);   
			}

			if (CanDouble(bDeskStation))
			{
				GameStation.iMaxNote = m_iMaxNote*2;
			}
			GameStation.bIsVipDouble = m_bIsVipDouble;
			GameStation.iMaxBaseNote = m_iMaxNote;							//��ͨ��ҵ������ע��

			// ��������
			SendGameStation(bDeskStation, uSocketID, bWatchUser, &GameStation, sizeof(GameStation));
			return true;
		}
	case GS_PLAY_GAME:	// ��Ϸ��״̬
		{
			GameStation_4 GameStation;
			::memset(&GameStation, 0, sizeof(GameStation));

			// ��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						// ��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			GameStation.iLimitNote = m_iLimitNote;						    // ���ע(Ŀǰ����������ׯ����)
			GameStation.iBaseNote = m_iBaseNote;
			GameStation.iMaxNote = m_iMaxNote;                              // �м������ע��

			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iThinkTime = m_iThinkTime;

			GameStation.iXiaZhuTime = m_iXiaZhuTime;                        // ��עʱ��
			GameStation.iKaipaiTime = m_iKaipaiTime;                        // ����ʱ��
			GameStation.iEmptyTime = m_iEmptyTime;                          // ����ʱ��

			GameStation.iTime = m_iTime;                                    // ʣ��ʱ��

			GameStation.bUseCard = m_bUseCard;                              // ѡ��ʹ���� TRUE: ʹ�ú�A FALSE: ʹ�ô���
			GameStation.bGameMode = m_bGameVersion;                         // ��Ϸ�汾 ��Ϸ�汾 0: ��ͨ 1: ������ 2: ���«2��
			GameStation.bKingAndThreeBig = m_bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
			GameStation.bRedJOrBlackJ = m_bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
			GameStation.chGameCount = m_chGameCount;                        // ��Ϸ����
			GameStation.chGameBeen = m_chGameBeen;                          // ��Ϸ����

			GameStation.iUpGradePeople = m_iZhuangStation;
			GameStation.bIsApplyXZ = m_bUserXZ;
			GameStation.iBeenPlayGame = m_iZhuangCount;
			GameStation.iZhuangMoenyResult = m_iZhuangResult;
			GameStation.iSelfMoenyResult = m_iUserResult[bDeskStation];

			// ������ע����
			::CopyMemory(GameStation.iMyZhu, m_UserXiaZhu.iXiaZhu[bDeskStation], sizeof(m_UserXiaZhu.iXiaZhu[bDeskStation]));

			// ��������ע����
			::CopyMemory(GameStation.iRobotZhu, m_UserXiaZhu.iRobotZhu, sizeof(m_UserXiaZhu.iRobotZhu));

			// ��ׯ�б�����
			memset(GameStation.userList, -1, sizeof(GameStation.userList));
			for (int i = 0; i < m_ArrUserList.GetSize(); i++)
			{
				GameStation.userList[i] = m_ArrUserList.GetAt(i);
			}

			// �����������ע����
			::CopyMemory(&GameStation.iXiaZhuInfo, &m_UserXiaZhu.iDeskInfo, sizeof(m_UserXiaZhu.iDeskInfo));

			// ����������
			::CopyMemory(GameStation.cardData, m_gameLogic.m_userData, sizeof(m_gameLogic.m_userData));

			// ���Ƽ�¼����
			GameStation.iKaiPaiCount = m_GameKPInfo.GetSize();   

			// �ж���Ϸģʽ��������Ϸ��¼�������
			int iMaxCount = /*GAME_VERSION_3 == m_bGameVersion ? GAME_XIPAI_COUNT : */30;

			// �������ݼ�¼
			for (BYTE i = 0; i < GameStation.iKaiPaiCount && i < iMaxCount; i++)
			{
				// ���ֿ��Ƽ�¼ (0x01:˳�š�0x02:���š�0x04:����)
				GameStation.bIsKaiPai[i] = m_GameKPInfo.GetAt(i);  

				if (i < m_GameXZInfo[bDeskStation].GetSize())
				{
					// ������ע��¼(0x01:˳�š�0x02:���š�0x04:����)
					GameStation.bIsXiaZhu[i] = m_GameXZInfo[bDeskStation].GetAt(i);  
				}
			}

			// ·������
			GameStation.iLuziCount = m_GameLizuInfo.GetSize();      
			for (BYTE i = 0; i < GameStation.iLuziCount && i < GAME_XIPAI_COUNT; i++)
			{
				// ·������
				GameStation.ArrLuzi[i] = m_GameLizuInfo.GetAt(i);   
			}

			if (CanDouble(bDeskStation))
			{
				GameStation.iMaxNote = m_iMaxNote*2;
			}
			GameStation.bIsVipDouble = m_bIsVipDouble;
			GameStation.iMaxBaseNote = m_iMaxNote;							//��ͨ��ҵ������ע��

			// ��������
			SendGameStation(bDeskStation, uSocketID, bWatchUser, &GameStation, sizeof(GameStation));
			return true;
		}
	case GS_WAIT_NEXT:		// �ȴ���һ����Ϸ��ʼ
		{
			GameStation_5 GameStation;
			::memset(&GameStation, 0, sizeof(GameStation));

			// ��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						// ��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			GameStation.iLimitNote = m_iLimitNote;							// ���ע(Ŀǰ����������ׯ����)
			GameStation.iBaseNote = m_iBaseNote;
			GameStation.iMaxNote = m_iMaxNote;                              // �м������ע��

			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iThinkTime = m_iThinkTime;

			GameStation.iXiaZhuTime = m_iXiaZhuTime;                        // ��עʱ��
			GameStation.iKaipaiTime = m_iKaipaiTime;                        // ����ʱ��
			GameStation.iEmptyTime = m_iEmptyTime;                          // ����ʱ��

			GameStation.iTime = m_iTime;                                    // ʣ��ʱ��

			GameStation.bUseCard = m_bUseCard;                              // ѡ��ʹ���� TRUE: ʹ�ú�A FALSE: ʹ�ô���
			GameStation.bGameMode = m_bGameVersion;                         // ��Ϸ�汾 ��Ϸ�汾 0: ��ͨ 1: ������ 2: ���«2��
			GameStation.bKingAndThreeBig = m_bKingAndThreeBig;				/**< TRUE: �������3��� FALSE: ˫��Q���*/
			GameStation.bRedJOrBlackJ = m_bRedJOrBlackJ;					/**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
			GameStation.chGameCount = m_chGameCount;                        // ��Ϸ����
			GameStation.chGameBeen = m_chGameBeen;                          // ��Ϸ����

			GameStation.iUpGradePeople = m_iZhuangStation;
			GameStation.bIsApplyXZ = m_bUserXZ;
			GameStation.iBeenPlayGame = m_iZhuangCount;
			GameStation.iZhuangMoenyResult = m_iZhuangResult;
			GameStation.iSelfMoenyResult = m_iUserResult[bDeskStation];

			// ��ׯ�б�����
			memset(GameStation.userList, -1, sizeof(GameStation.userList));
			for (int i = 0; i < m_ArrUserList.GetSize(); i++)
			{
				GameStation.userList[i] = m_ArrUserList.GetAt(i);
			}

			// ���Ƽ�¼����
			GameStation.iKaiPaiCount = m_GameKPInfo.GetSize();   

			// �ж���Ϸģʽ��������Ϸ��¼�������
			int iMaxCount = /*GAME_VERSION_3 == m_bGameVersion ? GAME_XIPAI_COUNT : */30;

			// �������ݼ�¼
			for (BYTE i = 0; i < GameStation.iKaiPaiCount && i < iMaxCount; i++)
			{
				// ���ֿ��Ƽ�¼ (0x01:˳�š�0x02:���š�0x04:����)
				GameStation.bIsKaiPai[i] = m_GameKPInfo.GetAt(i);  

				if (i < m_GameXZInfo[bDeskStation].GetSize())
				{
					// ������ע��¼(0x01:˳�š�0x02:���š�0x04:����)
					GameStation.bIsXiaZhu[i] = m_GameXZInfo[bDeskStation].GetAt(i);  
				}
			}

			// ·������
			GameStation.iLuziCount = m_GameLizuInfo.GetSize();      
			for (BYTE i = 0; i < GameStation.iLuziCount && i < GAME_XIPAI_COUNT; i++)
			{
				// ·������
				GameStation.ArrLuzi[i] = m_GameLizuInfo.GetAt(i);   
			}

			if (CanDouble(bDeskStation))
			{
				GameStation.iMaxNote = m_iMaxNote*2;
			}
			GameStation.bIsVipDouble = m_bIsVipDouble;
			GameStation.iMaxBaseNote = m_iMaxNote;							//��ͨ��ҵ������ע��

			// ��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return true;
		}
	}

	return false;
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
	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:
		{
			///�����ϲ�·��
			CString szAppPath = CINIFile::GetAppPath();

			CString strBCFFile; 
			strBCFFile.Format("%s\\SpecialRule.bcf",szAppPath); 

			CString strName;
			strName.Format("%d",NAME_ID);

			CBcfFile File(strBCFFile);

			bool  bForceQuitAsAuto = true; 

			if (File.IsFileExist(strBCFFile))
			{
				bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto", strName,0); 
			}

			if (bForceQuitAsAuto == true)
			{
				return true;
			}

			break;
		}
	default:
		break;
	}

	DebugPrintf("�����Ϸ���ݰ������� - ǿ���˳� - ��Ϸ״̬��%d", m_bGameStation);
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}
/**
* ����
*/
void CServerGameDesk::FaPaiToAllUser()
{
	m_bGameStation = GS_PLAY_GAME;

	KillTimer(TIME_SEND_CARD);
	DebugPrintf("����FaPaiToAllUser(1)- ��Ϸ״̬��%d", m_bGameStation);

	static int  iAllWinOrLost = 0;
	int iLost = 0;
	int iWin = 0;

	for (int i=0;i<3;i++)
	{
		if (BIPAI_FIRST_WIN == m_gameLogic.BiPaiFromStation(0, 3 - i))
		{
			iWin++;
		}
		if (BIPAI_SECONED_WIN == m_gameLogic.BiPaiFromStation(0, 3 - i))
		{
			iLost++;
		}
	}
	if (3 == iWin||3 ==iLost)
	{
		iAllWinOrLost++;
	}
	if (iAllWinOrLost>0)
	{	
		srand((unsigned)time(NULL));
		int  iRand = rand()%2;

		if (iRand>0)
		{
			PaiData PaiTemp(m_gameLogic.m_userData[0]);
			if (3==iWin)
			{
				if (BIPAI_FIRST_WIN == m_gameLogic.BiPaiFromStation(1,2))
				{
					memcpy(&m_gameLogic.m_userData[0], &m_gameLogic.m_userData[1], sizeof(PaiData));
					memcpy(&m_gameLogic.m_userData[1], &PaiTemp, sizeof(PaiTemp));
				}
				else
				{
					memcpy(&m_gameLogic.m_userData[0], &m_gameLogic.m_userData[2], sizeof(PaiData));
					memcpy(&m_gameLogic.m_userData[2], &PaiTemp, sizeof(PaiTemp));
				}
			}
			if (3 == iLost)
			{
				if (BIPAI_FIRST_WIN == m_gameLogic.BiPaiFromStation(1,2))
				{
					memcpy(&m_gameLogic.m_userData[0], &m_gameLogic.m_userData[2], sizeof(PaiData));
					memcpy(&m_gameLogic.m_userData[2], &PaiTemp, sizeof(PaiTemp));
				}
				else
				{
					memcpy(&m_gameLogic.m_userData[0], &m_gameLogic.m_userData[1], sizeof(PaiData));
					memcpy(&m_gameLogic.m_userData[1], &PaiTemp, sizeof(PaiTemp));
				}
			}
			iAllWinOrLost = 0;
		}
			 
	}
	// ִ�г����ͻ�������
	if (!RunSuperSetting())
	{
		// �����ͻ���δ���ƣ�����л�����ʤ�ʿ���
		IAWinAutoCtrl();
	}
#ifdef SEND_CARD_VERSION
	// ����
	InitSuperConfig();
#endif
	//�ͻ�����ͨ�Ժ�ͨ��̫�࣬���л���	

	// ��ʼ����
	GameFaPaiData  gameFaPai;

	// ���һ����
	//gameFaPai.bRandCard[0] = m_gameLogic.RandACard();

	// ���㿪ʼ����λ��
	//BYTE bCardValue = (gameFaPai.bRandCard[0] & 0x0F) + 1;
	//m_bSendCardPos = (bCardValue == 14 ? 1 : bCardValue % 4);
	//m_bSendCardPos = m_bSendCardPos % 2 == 0 ? (m_bSendCardPos + 1) % 4 : (m_bSendCardPos + 3) % 4;

	srand(unsigned(GetTickCount()));
	//int iRandNum = rand()%6+1;
	//int iRandNm
	gameFaPai.bDiceNum[0]= rand()%6 + 1 ;
	gameFaPai.bDiceNum[1] = rand()%6 +1 ; 


	DebugPrintf("����FaPaiToAllUser(2)- ��Ϸ״̬��%d", m_bGameStation);
	SetCardFun();

	DebugPrintf("����FaPaiToAllUser(3)- ��Ϸ״̬��%d", m_bGameStation);
	// ѡ��ʹ����
	gameFaPai.bUseCard = m_bUseCard;

	// ��ʼ����λ��
	gameFaPai.bStartSendPos = m_bSendCardPos;

	// ÿ����ҵ���
	::CopyMemory(gameFaPai.userCard, m_gameLogic.m_userData, sizeof(gameFaPai.userCard));

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if (NULL != m_pUserInfo[i])
		{
			SendGameData(i, &gameFaPai, sizeof(gameFaPai), MDM_GM_GAME_NOTIFY, EZP_MSG_SENDCARD, 0);
		}
	}

	SendWatchData(m_bMaxPeople, &gameFaPai, sizeof(gameFaPai), MDM_GM_GAME_NOTIFY, EZP_MSG_SENDCARD, 0);


	DebugPrintf("����FaPaiToAllUser(4)- ��Ϸ״̬��%d, ����ʱ��%d", m_bGameStation, m_iKaipaiTime);
	// ���ÿ���ʱ��
	SetGameTimer(TIME_GAME_FINISH, m_iKaipaiTime);

	DebugPrintf("����FaPaiToAllUser(5)- ��Ϸ״̬��%d", m_bGameStation);
}

/**
* ����
*/
void CServerGameDesk::InitSuperConfig()
{
	try
	{
		CString s;
		s = CINIFile::GetAppPath();
		CINIFile f(s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
		//�Ƿ�������ÿ���˵���
		int  bPaiConfig = 0;
		CString str = "paiConfig";
		bPaiConfig = f.GetKeyVal(str,"peipai",0);

		if (bPaiConfig >= 1)
		{
			CString strTmp;

			//������
			for(int i = 0; i < 4; i++)
			{
				strTmp.Format("stationfirst%d", i);
				m_gameLogic.m_userData[i].paiAry[0] = f.GetKeyVal(str, strTmp, 0);
				strTmp.Format("stationsecond%d", i);
				m_gameLogic.m_userData[i].paiAry[1] = f.GetKeyVal(str, strTmp, 0);
			}
		}
	}
	catch(...)
	{
		return;
	}
}

/**
* ִ�г����ͻ�������
*/
BOOL CServerGameDesk::RunSuperSetting()
{	
	if (!m_bSuperSetting)
	{
		// δ����
		return FALSE;
	}

	char inDexStation[4] ={0};

	for (int i = 1 ;i<4 ; i++)
	{
		inDexStation[i] = m_bSuperState[4-i-1] ;
	}

	CString str;
	str.Format("wysoutsuperset::inDexStation[0]=%d,inDexStation[1]==%d,inDexStation[2]==%d,inDexStation[3]==%d",inDexStation[0],inDexStation[1],inDexStation[2],inDexStation[3]) ; 
	OutputDebugString(str) ; 
	// ����1
	for (char i = 1; i < 4; i++)
	{
		if (0 == inDexStation[i])
		{
			continue ;
		}	
		for (int j = 0 ; j<4 ;/* j++*/)
		{
			if(i == j)
			{
				continue;
			}
			if(inDexStation[i]<0)
			{
				if (BIPAI_SECONED_WIN == m_gameLogic.BiPaiFromStation(j, i))
				{
					// ׯ�ҽ�����
					PaiData PaiTemp(m_gameLogic.m_userData[i]);

					memcpy(&m_gameLogic.m_userData[i], &m_gameLogic.m_userData[j], sizeof(PaiData));
					memcpy(&m_gameLogic.m_userData[j], &PaiTemp, sizeof(PaiTemp));
				}
			}
			else if(inDexStation[i]>0)
			{
				if (BIPAI_FIRST_WIN == m_gameLogic.BiPaiFromStation(j, i))
				{
					// ׯ�ҽ�����
					PaiData PaiTemp(m_gameLogic.m_userData[i]);

					memcpy(&m_gameLogic.m_userData[i], &m_gameLogic.m_userData[j], sizeof(PaiData));
					memcpy(&m_gameLogic.m_userData[j], &PaiTemp, sizeof(PaiTemp));
				}

			}
			if (j == 0)
				j=i+1;
			else
				j++;
		}
		
	}

	return TRUE;
}

/**
* �����������ׯ
* 
* @param [in] pApplySZ �����������ׯ��Ϣ
*/
void  CServerGameDesk::OnApplyZhuang(BYTE bDeskStation, EZP_APPLY_ZHUANG * pApplySZ)
{

	// ������ׯ���ǻ����ˣ����жϵ�ǰׯ���Ƿ����
	if (pApplySZ->bIsRobot && pApplySZ->bFlag)
	{
		if (m_ArrUserList.GetSize() >= rand()%(m_iRobotSZCount + 1))
		{		
			return ;
		}
	}

	// ������ׯ
	if (pApplySZ->bFlag)  
	{
		if (NULL == m_pUserInfo[bDeskStation])
		{
			return ;
		}

		// �жϸ�����Ƿ����������ׯ���ʸ�
		if (m_pUserInfo[bDeskStation]->m_UserData.i64Money >= m_iApplyMoneyLimit && m_pUserInfo[bDeskStation]->m_UserData.i64Money < 15000000000000000)
		{
			// �ж�������ׯ�����Ƿ�Ϊׯ��
			if (bDeskStation == m_iZhuangStation)
			{
				// ����������ׯʧ��
				EZP_APPLY_ZHUANG_ERR msg;
				msg.bErrorCode = CUR_ZHUANG_ERR;
				SendGameData(bDeskStation, &msg, sizeof(msg), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG_ERR, 0);

				return ;
			}
	
			if (m_ArrUserList.GetSize() >= 0)
			{
				for (int i = 0; i < m_ArrUserList.GetSize(); i++)
				{
					if (m_ArrUserList.GetAt(i) == bDeskStation)
					{
						// ����������ׯʧ��(�ظ�������ׯ)
						EZP_APPLY_ZHUANG_ERR msg;
						msg.bErrorCode = REPEAT_APPLAYZHUANG_ERR;
						SendGameData(bDeskStation, &msg, sizeof(msg), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG_ERR, 0);

						return ;
					}
				}

				// ������ׯ�����������ׯ����Ҳ��뵽�б���.
				m_ArrUserList.Add(bDeskStation);

				//��ǰ��ׯ ���� ��Ϸû������ 
				if(/*!m_bStartGameWithOutNt &&*/  m_iZhuangStation < 0 && !m_bIsRunning)	
				{
					m_bIsRunning = true;
					StartGame();	
					return ;
				}
			}
		}
		else
		{
			// ������ׯʧ��(��ׯ��Ҳ������߳�����ׯ����)
			EZP_APPLY_ZHUANG_ERR msg;
			msg.bErrorCode = LESS_APPLYZHUANG_MONEY_ERR;
			SendGameData(bDeskStation, &msg, sizeof(msg), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG_ERR, 0);
			return ;
		}

	}
	else  
	{
		// ������ׯ
		if (bDeskStation == m_iZhuangStation)
		{			
			m_bUserXZ = TRUE;

			if (!IsPlayGame(bDeskStation))  // ����ʱ��
			{
				// ------------ ֪ͨ�ͻ��ˣ����µ�ǰ����Ҫ��ׯ��ׯ�ҿͻ��˰�ť״̬ -----------------
				EZP_APPLY_ZHUANG ApplyZhuang;
				ApplyZhuang.bDeskStation = m_iZhuangStation;
				ApplyZhuang.bIsRobot = TRUE;
				ApplyZhuang.bFlag = FALSE;   //֪ͨ�ͻ��˸ı�������ׯ��ťΪ��ׯ��ť
				ApplyZhuang.bFirst = FALSE;

				if(NULL != m_pUserInfo[m_iZhuangStation])
				{
					SendGameData(m_iZhuangStation, &ApplyZhuang, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
				}

				// ------------ ֪ͨ�ͻ��ˣ����µ�ǰ����Ҫ��ׯ��ׯ�ҿͻ��˰�ť״̬ -----------------

				m_iZhuangStation = -1;

				return ;
			}
		}
		else
		{
			BOOL bFlag = FALSE;
			for (int i = 0; i < m_ArrUserList.GetSize(); i++)
			{
				if (m_ArrUserList.GetAt(i) == bDeskStation)
				{
					m_ArrUserList.RemoveAt(i);
					bFlag = TRUE;
					break;
				}
			}

			if (!bFlag)
			{
				// ��ׯ�б���û�д���ң�����
				return ;
			}
		}
	}

	for (int i = 0; i < PLAY_COUNT; i++) 
	{
		if(NULL != m_pUserInfo[i])
		{
			SendGameData(i, pApplySZ, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
		}
	}
	SendWatchData(m_bMaxPeople, pApplySZ, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
}

// ���VIP ˫����ע����(��������Ϸ)
BOOL  CServerGameDesk::CanDouble(BYTE bDeskStation)
{
	//�Ƿ�Ƿ���������
	if (bDeskStation > PLAY_COUNT)
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
	CString sstime;
	sstime.Format("zhtout:curtime=%d VIP=%d",curtime,m_pUserInfo[bDeskStation]->m_UserData.iVipTime);
	OutputDebugString(sstime);
	return curtime < m_pUserInfo[bDeskStation]->m_UserData.iVipTime?TRUE:FALSE;
	
}


/**
* �����ע
* 
* @param [in] pXiazhu �����ע��Ϣ
*/
void  CServerGameDesk::OnUserXiaZhu(BYTE bDeskStation, EZP_XIAZHU_INFO * pXiazhu)
{
	if (GS_XIAZHU != m_bGameStation)
	{
		// ������ע״̬����ׯģʽ
		return;
	}

	// BYTE bDeskStation = pXiazhu->bDeskStation;
	if (bDeskStation == m_iZhuangStation
		|| NULL == m_pUserInfo[bDeskStation] 
		|| NULL == m_pUserInfo[m_iZhuangStation])
	{
		return;
	}

	if (bDeskStation < 0 || bDeskStation >= PLAY_COUNT
		|| m_iZhuangStation < 0 || m_iZhuangStation >= PLAY_COUNT)
	{
		// ��ע��Ҵ����ׯ�Ҵ���
		return ;
	}

	if(m_bInNoteStage[bDeskStation] == FALSE)
	{
		return ; 
	}

	if (pXiazhu->bType < 0 || pXiazhu->bType >= 7)
	{
		// ��ע�������
		return ;
	}

	if (pXiazhu->bMoneyType < 0 || pXiazhu->bMoneyType >= CHOUMA_KIND_COUNT)
	{
		// ��ע������ʹ���
		return ;
	}
	if(pXiazhu->iMoney<= 0) ///�˴����Ʒ�ֹ�¸����
	{
		return ;
	}
	// �������û����˱���
	pXiazhu->bIsRobot = m_bIsRobot[bDeskStation];

	// �жϸ������ע����Ƿ�����Լ�����Ľ��
	if ((m_UserXiaZhu.iAllMoney[bDeskStation] + pXiazhu->iMoney) > m_pUserInfo[bDeskStation]->m_UserData.i64Money)
	{
		return ;
	}

	// �ж����������ע��Ҳ��ܳ���ׯ�ҽ��
	if ((m_iAllMoney + pXiazhu->iMoney) > m_pUserInfo[m_iZhuangStation]->m_UserData.i64Money)
	{
		return ;
	}
	// �ж�����������ע�����Ƿ񳬹������ע��, -1�ǲ������Ƶ�
	if (m_iMaxNote != -1)
	{
		// add xqm 2010-12-1
		if (CanDouble(bDeskStation))
		{
			if (m_UserXiaZhu.iAllMoney[bDeskStation] + pXiazhu->iMoney > m_iMaxNote * 2)
			{
				return ;
			}
		}
		else
		{
			if (m_UserXiaZhu.iAllMoney[bDeskStation] + pXiazhu->iMoney > m_iMaxNote)
			{
				return ;
			}
		}
	}

	if(m_iRectNoteMax[pXiazhu->bType] >0 )///�˴���Ӷ�������ע������
	{
		if((m_UserXiaZhu.iDeskInfo[pXiazhu->bType] + pXiazhu->iMoney) >m_iRectNoteMax[pXiazhu->bType])
		{
			return ; 
		}
	}

	// �ۼƽ��
	m_iAllMoney += pXiazhu->iMoney;

	// ÿ������Ľ���ܺ�
	m_UserXiaZhu.iDeskInfo[pXiazhu->bType] += pXiazhu->iMoney;

	// ��¼�����ע���ܶ�
	m_UserXiaZhu.iAllMoney[bDeskStation] += pXiazhu->iMoney;

	// ���������ע��
	m_UserXiaZhu.iXiaZhu[bDeskStation][pXiazhu->bType] += pXiazhu->iMoney;

	// �ж���ע����Ƿ�Ϊ������
	if (pXiazhu->bIsRobot)
	{
		// �ۼӻ�������ע���ܶ�
		m_UserXiaZhu.iRobotZhu[pXiazhu->bType] += pXiazhu->iMoney;
	}

	// ��־���������ע
	m_UserXiaZhu.bChipFlag[bDeskStation] = TRUE;

	// ���͸���ע��Ϣ���������
	EZP_XIAZHU_INFO userXiaZhuTurn;
	userXiaZhuTurn.bDeskStation = bDeskStation;
	userXiaZhuTurn.bType = pXiazhu->bType;
	userXiaZhuTurn.bMoneyType = pXiazhu->bMoneyType;
	userXiaZhuTurn.iMoney = pXiazhu->iMoney;
	userXiaZhuTurn.bIsRobot = pXiazhu->bIsRobot;

	if (CanDouble(bDeskStation))
	{
		//userXiaZhuTurn.bIsVip = true;
		// �ж���ע�Ƿ�����
		if (m_UserXiaZhu.iAllMoney[bDeskStation] == m_iMaxNote*2)
		{
			// ֪ͨ�ͻ���
			EZP_XIAZHU_HINT cmd;
			cmd.bState = 0;
			SendGameData(bDeskStation, &cmd, sizeof(EZP_XIAZHU_HINT), MDM_GM_GAME_NOTIFY, EZP_MSG_XIAZHU_HINT, 0);
		}
	}
	else
	{
		//userXiaZhuTurn.bIsVip = false;
		// �ж���ע�Ƿ�����
		if (m_UserXiaZhu.iAllMoney[bDeskStation] == m_iMaxNote)
		{
			// ֪ͨ�ͻ���
			EZP_XIAZHU_HINT cmd;
			cmd.bState = 0;
			SendGameData(bDeskStation, &cmd, sizeof(EZP_XIAZHU_HINT), MDM_GM_GAME_NOTIFY, EZP_MSG_XIAZHU_HINT, 0);
		}
	}

	for (int i = 0; i < PLAY_COUNT; i++) 
	{
		if(NULL != m_pUserInfo[i])
		{
			SendGameData(i, &userXiaZhuTurn, sizeof(EZP_XIAZHU_INFO), MDM_GM_GAME_NOTIFY, EZP_MSG_XIAZHU, 0);
		}
	}

	SendWatchData(m_bMaxPeople, &userXiaZhuTurn, sizeof(EZP_XIAZHU_INFO), MDM_GM_GAME_NOTIFY, EZP_MSG_XIAZHU, 0);
}

/**
* ��ע���
*/
void CServerGameDesk::OnUserXiaZhuFinish()
{
	memset(m_bInNoteStage, 0, sizeof(m_bInNoteStage));

	DebugPrintf("��ע���OnUserXiaZhuFinish(1)- ��Ϸ״̬��%d", m_bGameStation);

	KillTimer(TIME_XIAZHU);
	// 1.5���ʼ����
	//SetGameTimer(TIME_SEND_CARD, 1);
	SetTimer(TIME_SEND_CARD,300);

	int i, j, iMoney = 0;
	for (i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			iMoney = 0;

			for (j = 0; j < 6; j++)
			{
				iMoney += m_UserXiaZhu.iXiaZhu[i][j];
			}

			//DebugPrintf(m_pDataManage->m_InitData.uRoomID, "------------------ %s => ���:%d - ��ע���:%d ʣ����:%d ------------------",  
				//m_pUserInfo[i]->m_UserData.nickName, m_pUserInfo[i]->m_UserData.i64Money, iMoney, m_pUserInfo[i]->m_UserData.i64Money - iMoney);
		}
	}

	DebugPrintf("��ע���OnUserXiaZhuFinish(2)- ��Ϸ״̬��%d", m_bGameStation);
}

/**
* ��Ϸ����
*
* @param [in] bResult ���ֿ��е���(0x01:˳�� 0x02:���� 0x03:����)
*
* @param [out] iIsZiaZhu ����(0x01:˳�� 0x02:���� 0x03:����) �Ƿ�����ע
*
* @param [out] iMoney[] ������ҽ�������Ӯ���
*
* @param [out] iReturnMoney[] ������ұ��ַ��صĽ��
*
*/
void CServerGameDesk::DoGameResult(BYTE bResult, __int64 iMoney[], __int64  iReturnMoney[])
{
	BYTE bStation[3] = {GF_SHUNMEN, GF_DUIMEN, GF_DAOMEN};
	int i = 0, k = 0, index = 0;

	// 0:˳�� 1:���� 2:����
	for (i = 0; i < PLAY_COUNT; i++)
	{
		BYTE bSaveXiaZhu = 0;      // ���汾��˳�š����š������Ƿ���ע

		if (NULL != m_pUserInfo[i] && m_iZhuangStation != i)
		{
			for (k = 0; k < 3; k++)
			{
				index = bStation[k];  // �õ���Ӧ��ע���������λ��(������)

				BYTE offset = 0x01;   // λ�Ʊ���

				if (bResult & (offset<<k))
				{
					iMoney[i] += m_UserXiaZhu.iXiaZhu[i][index];

					// �м�Ӯ��Ǯ���뵽����������.
					iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][index];   

					iMoney[m_iZhuangStation] -= m_UserXiaZhu.iXiaZhu[i][index];
				}
				else
				{
					iMoney[i] -= m_UserXiaZhu.iXiaZhu[i][index];
					iMoney[m_iZhuangStation] += m_UserXiaZhu.iXiaZhu[i][index];
				}

				if (m_UserXiaZhu.iXiaZhu[i][index] > 0)
				{
					// ��־����ڴ�����ע��
					// ����λ����� offset = 0x01˳�� 0x02���� 0x04����
					bSaveXiaZhu |= offset<<k;
				}
			}
		}

		m_GameXZInfo[i].Add(bSaveXiaZhu);

		if (GAME_VERSION_3 != m_bGameVersion && m_GameXZInfo[i].GetSize() > 30)
		{
			// ������Ϸֻ��������30����¼,�뿪�Ƽ�¼��Ӧ
			m_GameXZInfo[i].RemoveAt(0);
		}
	}

	if ((bResult & 0x02) && (bResult & 0x04))
	{
		// �е���-��
		for (i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_JIAO];
				iMoney[m_iZhuangStation] -= m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_JIAO];

				iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_JIAO];
			}
		}
	}
	else if (!(bResult & 0x02) && !(bResult & 0x04))
	{
		// δ�е���-��
		for (i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iMoney[i] -= m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_JIAO];
				iMoney[m_iZhuangStation] += m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_JIAO];
			}
		}
	}
	else
	{
		// ���ص���-�ǵĽ��
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_JIAO];
			}
		}
	}

	if ((bResult & 0x01) && (bResult & 0x02))
	{
		// ��˳��-��
		for (i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_JIAO];
				iMoney[m_iZhuangStation] -= m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_JIAO];

				iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_JIAO];
			}
		}
	}
	else if (!(bResult & 0x01) && !(bResult & 0x02))
	{
		// δ��˳��-��
		for (i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iMoney[i] -= m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_JIAO];
				iMoney[m_iZhuangStation] += m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_JIAO];
			}
		}
	}
	else
	{
		// ����˳��-�ǵĽ��
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_JIAO];
			}
		}
	}

	if ((bResult & 0x01) && (bResult & 0x04))
	{
		// ����
		for (i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_HNEG];
				iMoney[m_iZhuangStation] -= m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_HNEG];

				iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_HNEG];

				iMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_HENG];
				iMoney[m_iZhuangStation] -= m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_HENG];

				iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_HENG];

			}
		}
	}
	else if (!(bResult & 0x01) && !(bResult & 0x04))
	{
		// δ����
		for (i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iMoney[i] -= m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_HNEG];
				iMoney[m_iZhuangStation] += m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_HNEG];

				iMoney[i] -= m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_HENG];
				iMoney[m_iZhuangStation] += m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_HENG];
			}
		}
	}
	else
	{
		// �����ŵĽ��
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (i != m_iZhuangStation)
			{
				iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_SHUNMEN_HNEG];

				iReturnMoney[i] += m_UserXiaZhu.iXiaZhu[i][GF_DAOMEN_HENG];
			}
		}
	}
}

/**
* ��Ϸ���ݰ�������
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
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case 79: //�����ͻ���
		{
			return TRUE;
		}
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if (bWatchUser)
			{
				return FALSE;
			}

			//��Ϸ�Ѿ���ʼ,������Ҵ����俪ʼ
			if (m_bGameStation >= GS_XIAZHU && m_bGameStation < GS_WAIT_NEXT)
			{
				return true;
			}

			//�������ͬ��״̬����������
			if (USER_ARGEE == m_pUserInfo[bDeskStation]->m_UserData.bUserState)
			{
				return true;  
			}

			if (NULL != m_pUserInfo[bDeskStation])
			{
				m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;  //�����Ϊͬ��״̬
			}

			__super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);

			//if(!IsPlayGame(m_iZhuangStation) && CountPlayer() >= 2) //add xyh 2012-1-9
			//{
			//	StartGame();
			//}

			return true;
		}
	case ASS_SUPER_SETTING:  // ��Ӯ����
		{
			if (uSize != sizeof(SUPERSETTING))
			{
				DebugPrintf("��Ϸ���ݰ������� - ASS_SUPER_SETTING - ��Ӯ���ư����� - ��Ϸ״̬��%d", m_bGameStation);
				return false;
			}

			OnSuperSetting(bDeskStation, (SUPERSETTING *)pData);

			return true;
		}
	case EZP_MSG_APPLYZHUANG:  //������� ��/��ׯ
		{
			if (bWatchUser)
			{
				return false;
			}

			if (uSize != sizeof(EZP_APPLY_ZHUANG))
			{
				DebugPrintf("��Ϸ���ݰ������� - EZP_MSG_APPLYZHUANG - ������� ��/��ׯ������ - ��Ϸ״̬��%d", m_bGameStation);
				return false;
			}

			EZP_APPLY_ZHUANG *pApplyZhuang = (EZP_APPLY_ZHUANG *)pData;

			OnApplyZhuang(bDeskStation, pApplyZhuang);

			return true;
		}
	case EZP_MSG_XIAZHU:       //�����ע
		{
			if (bWatchUser)
			{
				return false;
			}
			if (uSize != sizeof(EZP_XIAZHU_INFO))
			{
				DebugPrintf("��Ϸ���ݰ������� - EZP_MSG_XIAZHU - �����ע������ - ��Ϸ״̬��%d", m_bGameStation);
				return false;
			}

			OnUserXiaZhu(bDeskStation, (EZP_XIAZHU_INFO *)pData);

			return true;
		}
	case EZP_MSG_FINISH_NOTE:
		{
			if(bWatchUser)
			{
				return false;
			}
			if(uSize != sizeof(EZP_FINISH_NOTE))
			{
				DebugPrintf("��Ϸ���ݰ������� - EZP_MSG_FINISH_NOTE - �����ע��ɰ����� - ��Ϸ״̬��%d", m_bGameStation);
				return false;
			}
			SendGameData(bDeskStation, MDM_GM_GAME_NOTIFY ,EZP_MSG_FINISH_NOTE,0);

			m_bInNoteStage[bDeskStation] = FALSE ; 

			return true;
		}
	}

	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

/**
* ֹͣ���м�ʱ��
*/
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_EMPTY);
	KillTimer(TIME_XIAZHU);
	//KillTimer(TIME_GAME_FINISH);
}

/**
* ֹͣ���м�ʱ��
*/
void CServerGameDesk::KillGameAllTimer()
{
	KillTimer(IDT_USER_CUT);
	KillTimer(ID_WIAT_GAME_BEGIN);
	KillTimer(TIME_RAND_CARD);
	KillTimer(TIME_JIAO_ZHUANG);
	KillTimer(TIME_XIAZHU);
	KillTimer(TIME_SEND_CARD);
	//KillTimer(TIME_GAME_FINISH);
	KillTimer(TIME_EMPTY);
}

/**
* ������Ϸ״̬
*
* @param [in] bLastStation ��Ϸ������־
*
* @return ���ز�������
*/
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillAllTimer();

	//���ݳ�ʼ��
	InitData();

	return TRUE;
}

/**
* ��ʼ����Ϸ
*
* @return ���ز�������
*/
bool CServerGameDesk::InitDeskGameStation()
{
	// ���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);

	return true;
}

/**
* ��Ϸ��ʼ
*
* @param [in] bBeginFlasg ��ʼģʽ��־
*
* @return ���ز�������
*/
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	DebugPrintf("��Ϸ��ʼGameBegin(1)- ��Ϸ״̬��%d", m_bGameStation);

#ifdef AUTOLOADINI
	AutoLoadIni(m_pDataManage->m_InitData.uRoomID);
#endif

	if (__super::GameBegin(bBeginFlag) == false) 
	{
		m_bNTGameMode = FALSE;

		GameFinish(0,GF_SALE);

		DebugPrintf("��Ϸ��ʼGameBegin(����)- ��Ϸ״̬��%d", m_bGameStation);
		return false;
	}

	m_bGameStation = GS_XIAZHU;

	InitData();

	if(m_iZhuangStation < 0)
	{
		m_iZhuangCount = 0;
		m_iZhuangResult = 0; 
	}

	IsZhuangRule();
	
	///��Ϸ�ȴ�״̬
	KillTimer(ID_WIAT_GAME_BEGIN);

	// �ж���Ϸģʽ
	//DecideGameMode();
	// add by wys ����ǰׯ�ҵ�ID����ƽ̨�����ׯ�Ҳ�������ׯ�ڼ��Ǯ������
	long iNTUserID = 0;

	if (m_iZhuangStation>=0 && m_pUserInfo[m_iZhuangStation]->m_UserData.isVirtual == 0 )
	{
		iNTUserID = m_pUserInfo[m_iZhuangStation]->m_UserData.dwUserID;			
	}

	SetCurrentZhuangjia(iNTUserID);	

	if(m_iZhuangStation < 0 ) ///�����һ������ׯ
	{
		SetGameTimer(TIMT_APPLY_NT , 10); 

		for(int i = 0 ; i<PLAY_COUNT ; i++)
		{
			if(NULL != m_pUserInfo[i])
			{
				SendGameData(i , MDM_GM_GAME_NOTIFY , ASS_APPLY_NT , 0 ) ; 
			}
		}
	}
	else
	{
		//����
		m_gameLogic.GameXiPai(FALSE);

		NotifyClientNote();
	}

	DebugPrintf("��Ϸ��ʼGameBegin(2)- ��Ϸ״̬��%d", m_bGameStation);
	return true;
}

/**
* ��ʼ��Ϸ
*/
void CServerGameDesk::StartGame()
{
	//�����ׯ����������Ϸ�򷵻�
	/*if(!m_bStartGameWithOutNt && m_iZhuangStation < 0)
	{
		return ;
	}*/

	int iRemaindCount = 0;	//ʣ������
	//��������ҵ�״̬����ͬ��״̬
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL == m_pUserInfo[i])//��Ҳ����ھ�
		{
			continue;
		}
		iRemaindCount++;
		m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
	}	

	if(0 == iRemaindCount)
	{
		KillAllTimer();
		m_bIsRunning = false;
		return;
	}

	//��ׯ�ҵ�״̬����ͬ��״̬
	//if(NULL != m_pUserInfo[m_iZhuangStation])	//add xyh 2012-1-9
	//{
	//	m_pUserInfo[m_iZhuangStation]->m_UserData.bUserState = USER_ARGEE;
	//}

	int count = 0;
	int iArgeeCount = 0;
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(NULL == m_pUserInfo[i]) //��Ҳ����ھ�
		{
			continue;
		}

		if(m_pUserInfo[i]->m_UserData.bUserState == USER_ARGEE)
		{
			iArgeeCount++;
		}
		
		count ++;
	}

	if(iArgeeCount == count)
	{
		GameBegin(ALL_ARGEE);
	}
	else
	{
		KillAllTimer();
		m_bIsRunning = false;
	}
}

/// ֪ͨ�ͻ��˿�ʼ��ׯ
///֪ͨ�ͻ��˿�ʼ��ע
void CServerGameDesk::NotifyClientNote()
{	
	DebugPrintf("֪ͨ�ͻ��˿�ʼ��עNotifyClientNote(1)- ��Ϸ״̬��%d", m_bGameStation);
	m_bGameStation = GS_XIAZHU;
	// ��һ����ׯ�����ׯ�ĳɼ�
	if (m_iZhuangCount == 0)
	{
		m_iZhuangResult = 0;
	}
	// ��ׯ�����ۼ�
	m_iZhuangCount += 1;

	memset(m_bInNoteStage , 1, sizeof(m_bInNoteStage)) ; 

	GameBeginStruct BeginMessage;
	memset(&BeginMessage,0,sizeof(BeginMessage));
	BeginMessage.bZhuangStation = m_iZhuangStation;             // ����ׯ��
	BeginMessage.iBeenPlayGame = m_iZhuangCount;				// �Ѿ�����ľ���
	BeginMessage.iLimitNote = m_iLimitNote;						// ��ע
	BeginMessage.iBaseNote = m_iBaseNote;						// ��ע
	BeginMessage.bNTGameMode = m_bNTGameMode;                   // ��Ϸׯģʽ
	BeginMessage.iZhuangMaxCount = m_iZhuangMaxNum;             // �����ׯ���پ�
	BeginMessage.chGameBeen = m_chGameBeen;                     // ��Ϸ����
	BeginMessage.chGameCount = m_chGameCount;                   // ��Ϸ����
	BeginMessage.iZhuangResult = m_iZhuangResult; 

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if(m_pUserInfo[i] != NULL)
		{
			// �ж�����Ƿ�Ϊ������ 0: ��Ϊ������ 1: Ϊ������
			if (1 == m_pUserInfo[i]->m_UserData.isVirtual)
			{
				// ����Ϊ������
				m_bIsRobot[i] = true;
			}

			// ������ҽ��
			BeginMessage.iSelfMoney = m_pUserInfo[i]->m_UserData.i64Money;

			SendGameData(i,&BeginMessage, sizeof(BeginMessage), MDM_GM_GAME_NOTIFY, ASS_GAME_BEGIN, 0);
		}		
	}	
	
	// �������ݸ��Թ���
	SendWatchData(m_bMaxPeople, &BeginMessage, sizeof(BeginMessage), MDM_GM_GAME_NOTIFY, ASS_GAME_BEGIN, 0);

	DebugPrintf("֪ͨ�ͻ��˿�ʼ��עNotifyClientNote(2)- ��Ϸ״̬��%d", m_bGameStation);
	SetTimer(TIME_RAND_CARD,7000); // ϴ��
	DebugPrintf("֪ͨ�ͻ��˿�ʼ��עNotifyClientNote(3)- ��Ϸ״̬��%d", m_bGameStation);
	return ;
}

/**
* �ж�ׯ���Ƿ������Ϸ����
*
* @return ���ز�������
*/
bool CServerGameDesk::IsZhuangRule()
{
	if (m_iZhuangStation != -1 && NULL != m_pUserInfo[m_iZhuangStation])
	{
		if (m_iZhuangCount >= m_iZhuangMaxNum 
			|| m_pUserInfo[m_iZhuangStation]->m_UserData.i64Money <= 0
			|| m_pUserInfo[m_iZhuangStation]->m_UserData.i64Money >= 15000000000000000)
		{
			// ------------ ֪ͨ�ͻ��ˣ����µ�ǰ����Ҫ��ׯ��ׯ�ҿͻ��˰�ť״̬ -----------------
			EZP_APPLY_ZHUANG ApplyZhuang;
			ApplyZhuang.bDeskStation = m_iZhuangStation;
			ApplyZhuang.bIsRobot = TRUE;
			ApplyZhuang.bFlag = FALSE;   //֪ͨ�ͻ��˸ı�������ׯ��ťΪ��ׯ��ť
			ApplyZhuang.bFirst = FALSE;

			if(NULL != m_pUserInfo[m_iZhuangStation])
			{
				SendGameData(m_iZhuangStation, &ApplyZhuang, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
			}
			// ------------ ֪ͨ�ͻ��ˣ����µ�ǰ����Ҫ��ׯ��ׯ�ҿͻ��˰�ť״̬ -----------------

			if (!IsZhuangList())
			{
				return false;
			}
		}		
	}
	else
	{
		if (!IsZhuangList())
		{
			return false;
		}
	}

	return true;
}

/**
* �ж���ׯ�б�
*
* @return ���ز�������
*/
bool CServerGameDesk::IsZhuangList()
{
	BYTE bDeskStation = -1;
	m_iZhuangStation = -1;

	// �ж���ׯ�б����Ƿ���������ׯ��
	if (m_ArrUserList.GetSize() > 0)
	{
		bDeskStation = m_ArrUserList.GetAt(0);

		EZP_APPLY_ZHUANG ApplyZhuang;
		while (true)
		{
			// ���б���ɾ��
			ApplyZhuang.bDeskStation = bDeskStation;
			ApplyZhuang.bFlag = FALSE;   //֪ͨ�ͻ���ɾ����ׯ�û�
			ApplyZhuang.bIsRobot = TRUE;
			ApplyZhuang.bFirst = FALSE;

			if (m_pUserInfo[bDeskStation] != NULL)
			{
				// �ж�ׯ�ҽ�ҷ�Χ
				if (m_pUserInfo[bDeskStation]->m_UserData.i64Money >= m_iApplyMoneyLimit 
					&& m_pUserInfo[bDeskStation]->m_UserData.i64Money < 15000000000000000)
				{
					// ����Ҫ��
					ApplyZhuang.bFirst = TRUE;
					OnApplyZhuang(bDeskStation, &ApplyZhuang);

					// ����ׯ��
					m_iZhuangStation = bDeskStation;

					// ��ׯ30��
					m_iZhuangMaxNum = m_iMaxZhuangCount;
					break;
				}

				OnApplyZhuang(bDeskStation, &ApplyZhuang);
			}
			else
			{
				OnApplyZhuang(bDeskStation, &ApplyZhuang);
			}

			// ���б���ȡ��һ����ׯ���
			if (m_ArrUserList.GetSize() > 0)
			{
				bDeskStation = m_ArrUserList.GetAt(0);
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		// û���򷵻�
		return false;
	}

	if (m_ArrUserList.GetSize() > 0)
	{
		bDeskStation = m_ArrUserList.GetAt(0);
		if (m_pUserInfo[bDeskStation] != NULL && m_pUserInfo[m_iZhuangStation] != NULL)
		{
			// �ж���һ��������ׯ�ߵĽ���Ƿ���ڵ�ǰׯ�ҽ��
			if (m_pUserInfo[bDeskStation]->m_UserData.i64Money > m_pUserInfo[m_iZhuangStation]->m_UserData.i64Money)
			{
				// ����ׯ�ҽ�ң����ֻ����ׯ15��
				m_iZhuangMaxNum = m_iMinZhuangCount;
			}
		}
	}

	// ��ׯ��������
	m_iZhuangCount = 0;

	return true;
}

/**
* �ж���Ϸģʽ
*/
void CServerGameDesk::DecideGameMode()
{
	// �ж��Ƿ�Ϊ���«2���汾, ��ӡδ��72���ƽ��
	if (GAME_VERSION_3 == m_bGameVersion)
	{
		// �жϵ�һ����Ϸ �� ��Ϸ�����Ѵ���������
		if (0 == m_chGameCount || m_chGameCount >= GAME_XIPAI_COUNT)
		{
			CTime time = CTime::GetCurrentTime();

			// �ж������Ƿ��Ѿ�����һ��(�жϵ�ǰʱ����֮ǰ��¼��ʱ�䲻ͬ���������һ��)
			if (time.GetDay() != m_RecordTime.GetDay()
				|| time.GetMonth() != m_RecordTime.GetMonth()
				|| time.GetYear() != m_RecordTime.GetYear())
			{
				// ���µ�ǰʱ��
				m_RecordTime = time;

				// �µ�һ�죬������0
				m_chGameBeen = 0;
			}

			// ϴ�� ϴ72����
			m_gameLogic.GameXiPai(TRUE);

			// ��Ϸ��������Ϊ0
			m_chGameCount = 0;

			// ��Ϸ����
			m_chGameBeen += 1;

			// ��ӡ���
			PrintEnding();

			// �����Ϸ��¼
			m_GameKPInfo.RemoveAll();

			// ����û���ע��¼
			for (BYTE i = 0; i < PLAY_COUNT; i++)
			{
				m_GameXZInfo[i].RemoveAll();
			}
		}

		// ��Ϸ�����ۼ�
		m_chGameCount += 1;

		// ���Ʊ���������
		::CopyMemory(m_gameLogic.m_userData, m_gameLogic.m_GameCard[m_chGameCount - 1], sizeof(m_gameLogic.m_userData));
	}
	else
	{
		// ϴ��
		m_gameLogic.GameXiPai(FALSE);
	}
}

/**
* ͳ������Ϸ���
*
* @return ���ص�ǰ��Ϸ����
*/
BYTE CServerGameDesk::CountPlayer()
{
	BYTE count = 0;
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(m_pUserInfo[i] != NULL)
		{	
			count ++;
		}
	}

	return count;
}

/**
* ��Ϸ����
*
* @param [in] bDeskStation ���λ��
*
* @param [in] bCloseFlag ��Ϸ������־
*
* @return ���ز�������
*/
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	DebugPrintf("��Ϸ���� - GameFinish(1) - ��Ϸ״̬��%d, bCloseFlag == %d", m_bGameStation, bCloseFlag);
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			DebugPrintf("��Ϸ���� - GameFinish(GF_NORMAL - 1) - ��Ϸ״̬��%d,��%d�� ��%d��", m_bGameStation, m_chGameBeen, m_chGameCount);

			m_bGameStation = GS_WAIT_ARGEE;			
    		//����ʤ������
			for (int i = 0;i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
				{
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
				}
			}

			GameEndData gameEndData;
			gameEndData.bAutoZhuangXZ = FALSE;
			gameEndData.bZhuangXZ = FALSE;

			__int64   i_ChangePoint[PLAY_COUNT];     //����
			__int64   i_ChangeMoney[PLAY_COUNT];     //���
			__int64   i_ReturnMoney[PLAY_COUNT];     //���ؽ��			

			memset(i_ChangePoint, 0, sizeof(i_ChangePoint));
			memset(i_ChangeMoney, 0, sizeof(i_ChangeMoney));
			memset(i_ReturnMoney, 0, sizeof(i_ReturnMoney));			

			for (int i = 0; i < 4; i++)
			{
				// �ĸ�λ�õ���
				gameEndData.userData[i].paiAry[0] = m_gameLogic.GetValue(m_gameLogic.m_userData[i].paiAry[0]);
				gameEndData.userData[i].paiAry[1] = m_gameLogic.GetValue(m_gameLogic.m_userData[i].paiAry[1]);
			}

			// ÿ������ܹ����˶���ע
			// �������ʤ������
			for(int i = 1; i < 4; i++)   // ׯ��λ��Ϊ0
			{
				char szPai[50];
				wsprintf(szPai, "%s", m_gameLogic.GetPaiValueName(i));

				gameEndData.paiValue[i].SetValue(szPai);

				BIPAI_RET biPaiRet;
				biPaiRet = m_gameLogic.BiPaiFromStation(0, i);
				if (biPaiRet == BIPAI_SECONED_WIN)
				{
					// �м�Ӯ
					BYTE offset = 0x01;             // λ�Ʊ���

					offset <<= (3-i);               // ���ƶ�����Ӧλ�� ����: ������Ҫ�ƶ�����3λ�� k<<2 ����0100

					gameEndData.bKaiPai |= offset;  // 3-i: ������˳�Ŵ��λ�öԵ�(����λ�� 0x01:˳�� 0x02:���� 0x04:����)
				}
				else
				{
					// �м��䣬�����в���������ԭ��ֵ.
				}
			}	

			// ���ƽ���
			DoGameResult(gameEndData.bKaiPai, i_ChangePoint, i_ReturnMoney);


			// ������д�����ݿ�
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut, 0, sizeof(temp_cut));

			///2010-9-27 �޸�̨�ѿ۷����⣬û��ע���˵�ׯʱ�����˶�����ע����̨��
			CGameUserInfo	* pUserInfoTemp[MAX_PEOPLE];
			memcpy(pUserInfoTemp,m_pUserInfo, sizeof(m_pUserInfo));
			BOOL bHaveNote = FALSE; //������ע��
			for (int i=0; i<PLAY_COUNT&&i<MAX_PEOPLE; i++)
			{
				if (m_UserXiaZhu.iAllMoney[i] == 0 && m_iZhuangStation != i)
				{
					m_pUserInfo[i] = NULL;
				}
				else if(!bHaveNote && m_iZhuangStation != i)
				{
					bHaveNote = TRUE;  
				}
			}
			if (!bHaveNote)
			{
				m_pUserInfo[m_iZhuangStation] = NULL;
			}

			if (0 == m_iNtTax)
			{
				// ֻ��ׯ��˰
				ChangeUserPointint64(i_ChangePoint, temp_cut, m_iZhuangStation);
			}
			else
			{
				// ������Ӯ��˰
				ChangeUserPointint64(i_ChangePoint, temp_cut);
			}

			__super::RecoderGameInfo(i_ChangeMoney);

			//��¼������Ӯ���
			RecordAiHaveWinMoney(i_ChangeMoney);

			memcpy(m_pUserInfo,pUserInfoTemp,sizeof(m_pUserInfo));
			// ��¼ׯ����Ӯ���
			if (!m_bNTGameMode && -1 != m_iZhuangStation)
			{
				if (NULL != m_pUserInfo[m_iZhuangStation])
				{	
					// ׯ�ҽ��
					gameEndData.dwZhuangMoney = m_pUserInfo[m_iZhuangStation]->m_UserData.i64Money;

					// ׯ����Ӯ���
					gameEndData.iZhuangMoney = i_ChangeMoney[m_iZhuangStation];

					// ��¼ׯ����Ӯ���
					m_iZhuangResult += i_ChangeMoney[m_iZhuangStation];

					if (gameEndData.dwZhuangMoney < m_iApplyMoneyLimit 
						|| gameEndData.dwZhuangMoney >= 15000000000000000
						|| m_iZhuangCount >= m_iZhuangMaxNum)
					{	
						if (m_iZhuangCount < m_iZhuangMaxNum)
						{
							// ׯ�ҽ��С�ڻ������ׯ�����Զ���ׯ
							gameEndData.bAutoZhuangXZ = TRUE;
						}

						if (m_ArrUserList.GetSize() <= 0)
						{
							gameEndData.bZhuangXZ = TRUE;
						}

						// ------------ ֪ͨ�ͻ��ˣ����µ�ǰ����Ҫ��ׯ��ׯ�ҿͻ��˰�ť״̬ -----------------
						EZP_APPLY_ZHUANG ApplyZhuang;
						ApplyZhuang.bDeskStation = m_iZhuangStation;
						ApplyZhuang.bIsRobot = TRUE;
						ApplyZhuang.bFlag = FALSE;   //֪ͨ�ͻ��˸ı�������ׯ��ťΪ��ׯ��ť
						ApplyZhuang.bFirst = FALSE;

						if (NULL != m_pUserInfo[m_iZhuangStation])
						{
							SendGameData(m_iZhuangStation, &ApplyZhuang, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
						}
						// ------------ ֪ͨ�ͻ��ˣ����µ�ǰ����Ҫ��ׯ��ׯ�ҿͻ��˰�ť״̬ -----------------

						m_iZhuangStation = -1;
					}	
				}
			}
			else
			{
				// ׯ����Ӯ���
				gameEndData.iZhuangMoney = 0;
				m_iZhuangResult = 0;
			}

			// ������Ϸ������ׯ��������ׯ�˻���ׯģʽ
			if (m_bNTGameMode || m_bUserXZ)
			{
				if (m_ArrUserList.GetSize() <= 0)
				{
					gameEndData.bZhuangXZ = TRUE;
				}

				m_iZhuangStation = -1;
			}	

			int iRetCount = m_ArrUserList.GetSize();
			if (m_iRobotSZCount>iRetCount)
			{
				gameEndData.bRobortCanApply = TRUE;
			}
			else
			{
				gameEndData.bRobortCanApply = FALSE;
			}

			CString CST;

			for(int i = 0 ; i<PLAY_COUNT ; i++)
			{
				if(NULL  == m_pUserInfo[i] || i == m_iZhuangStation)
				{
					m_iUserNoteZeroCount[i] = 0 ;
				}
				else
				{
					if(m_UserXiaZhu.iAllMoney[i]>0)
					{
						m_iUserNoteZeroCount[i] = 0 ;
					}
					else
					{
						m_iUserNoteZeroCount[i]++;
					}
					//CST.Format("wysoutlog::�����������û����ע==%d,�������==%s",m_iUserNoteZeroCount[i],m_pUserInfo[i]->m_UserData.nickName);
					//OutputDebugString(CST);
				}
			}	

		    __int64 jqrMoney = 0,wjMoney = 0;

			// ��������
		   CTime time = CTime::GetCurrentTime();

			for (int i = 0 ; i < PLAY_COUNT; i ++) 
			{
				if (m_pUserInfo[i] != NULL) 
				{
					// ��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
					if (m_bAIWinAndLostAutoCtrl && m_bIsRobot[i])
					{
						// ��¼���л�������Ӯ���
						m_iAIHaveWinMoney += i_ChangeMoney[i];	// ��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ
						jqrMoney += i_ChangeMoney[i];
					}
					else
					{
						wjMoney += i_ChangeMoney[i];
						CString sdp;
						sdp.Format("wx wjMoney = %I64d i_ChangeMoney[%d] = %I64d",wjMoney,i,i_ChangeMoney[i]);
						OutputDebugString(sdp);
					}
				}	
				m_iUserResult[i] += i_ChangeMoney[i];  // ��¼ÿ����Ӯ���
			}

			DebugPrintf2(m_pDataManage->m_InitData.uRoomID,"����ʱ��[%d:%d:%d]::�������Ѿ�Ӯ�Ľ��%d ������Ӯ�Ľ��%I64d,���Ӯ�Ľ��%I64d",
				time.GetHour(),time.GetMinute(),time.GetSecond(),m_iAIHaveWinMoney,jqrMoney ,wjMoney) ;
		

			for(int  i = 0 ;i<PLAY_COUNT ; i++)
			{
				if(NULL != m_pUserInfo[i])
				{
					gameEndData.dwMoney = m_pUserInfo[i]->m_UserData.i64Money;
					gameEndData.bForceLeave = /*m_iUserNoteZeroCount[i]>2?TRUE:*/FALSE;

					gameEndData.iUserWinMoney = i_ChangeMoney[i] ; 
					gameEndData.iUserReturnMoney = i_ReturnMoney[i] ; 

					SendGameData(i, &gameEndData, sizeof(gameEndData), MDM_GM_GAME_NOTIFY, ASS_CONTINUE_END, 0);
				}
			}			

			SendWatchData(m_bMaxPeople, &gameEndData, sizeof(gameEndData), MDM_GM_GAME_NOTIFY, ASS_CONTINUE_END, 0);

			bCloseFlag = GFF_FORCE_FINISH;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);

			// ����ģʽ
			m_bNTGameMode = FALSE;

			// ����ʱ��10��
			//SetGameTimer(TIME_EMPTY, m_iEmptyTime);

			// ���濪����Ϣ
			m_GameKPInfo.Add(gameEndData.bKaiPai);

			if (m_GameKPInfo.GetSize() > 30)
			{
				// ������Ϸֻ��������30����¼
				m_GameKPInfo.RemoveAt(0);
			}

			if (GAME_VERSION_3 == m_bGameVersion)
			{
				// ������Ϣ���浽·��������
				GameLuziInfo gameInfo;
				gameInfo.bKaiPai = gameEndData.bKaiPai;
				gameInfo.chGameBeen = m_chGameBeen;
				gameInfo.chGameCount = m_chGameCount;
				m_GameLizuInfo.Add(gameInfo);

				// ֻ��������72��
				if (m_GameLizuInfo.GetSize() > GAME_XIPAI_COUNT)
				{
					m_GameLizuInfo.RemoveAt(0);
				}
			}		
			// �����ׯ�б�
			CheckAppalyList();

			// ���������ϢΪ�յ�����
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] == NULL)
				{
					ClearUserInfo(i);
					m_GameXZInfo[i].RemoveAll();					
				}
			}

			// ����ʱ��10��
			//SetGameTimer(TIME_EMPTY, m_iBeginTime);
			SetGameTimer(TIME_EMPTY, 10);
			return true;
		}
	case  GF_AHEAD:
		{
			DebugPrintf("��Ϸ���� - GameFinish(GF_AHEAD - 1) - ��Ϸ״̬��%d", m_bGameStation);
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}

			memset(m_iUserNoteZeroCount, 0 , sizeof(m_iUserNoteZeroCount)) ; 
		
			m_ArrUserList.RemoveAll();

			m_iZhuangStation = -1;

			for(int  i = 0 ;i<PLAY_COUNT ; i++)
			{
				if(NULL != m_pUserInfo[i])
				{
					SendGameData(i,  MDM_GM_GAME_NOTIFY, ASS_AHEAD_END, 0);
				}
			}		

			//SendWatchData(m_bMaxPeople, MDM_GM_GAME_NOTIFY, ASS_AHEAD_END, 0);		

			// �����ׯ�б�
			CheckAppalyList();

			// ���������ϢΪ�յ�����
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				m_GameXZInfo[i].RemoveAll();	 ///���ǿ�������ҵ���ע��Ϣ
				if (m_pUserInfo[i] == NULL)
				{
					ClearUserInfo(i);					
				}
			}

			bCloseFlag = GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);

			__super::GameFinish(bDeskStation,bCloseFlag);	

			//��װ���� ��Ϸû��������
			m_bIsRunning = false;

			return true;	
		}
	case GFF_FORCE_FINISH:
		{
			KillGameAllTimer();

			DebugPrintf("��Ϸ���� - GameFinish(GFF_FORCE_FINISH - 1) - ��Ϸ״̬��%d", m_bGameStation);
			// Ϊ��������û��뿪������ӵĴ���
			// �жϸ�����Ƿ�ѹ��ע���Ƿ�ׯ��
			bool bIsPlayerBetted = false;
			for (int i = 0; i < 6; ++i)
			{
				if (m_UserXiaZhu.iXiaZhu[bDeskStation][i] != 0)
				{
					bIsPlayerBetted = true;
					break;
				}
			}

			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL)
				{
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
				}
			}

			///ȫ��ǿ������Ϸ�е�ׯ���б����
			memset(m_iUserNoteZeroCount, 0 , sizeof(m_iUserNoteZeroCount)); 

			m_ArrUserList.RemoveAll();

			m_iZhuangStation = -1;

			// ���������ϢΪ�յ�����
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				m_GameXZInfo[i].RemoveAll();	 ///���ǿ�������ҵ���ע��Ϣ
				if (m_pUserInfo[i] == NULL)
				{
					ClearUserInfo(i);					
				}
			}

			ReSetGameState(bCloseFlag);

			__super::GameFinish(bDeskStation,bCloseFlag);

			// �����ׯ�б�
			CheckAppalyList();
			DebugPrintf("��Ϸ���� - GameFinish(GFF_FORCE_FINISH - 2) - ��Ϸ״̬��%d", m_bGameStation);
			//��װ���� ��Ϸû��������
			m_bIsRunning = false;
			return true;
		}
	default:
		{
			break;
		}
	}

	DebugPrintf("��Ϸ���� - GameFinish(2) - ��Ϸ״̬��%d", m_bGameStation);
	// ��������
	ReSetGameState(bCloseFlag);
	return __super::GameFinish(bDeskStation,bCloseFlag);
}

/**
* �ж��Ƿ�������Ϸ
*
* @param [in] bDeskStation ���λ��
*
* @return ���ص�ǰ��Ϸ״̬
*/
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	DebugPrintf("�ж��Ƿ�������Ϸ - IsPlayGame - ��Ϸ״̬��%d", m_bGameStation);
	if (bDeskStation < 0 || bDeskStation >= PLAY_COUNT)
	{
		return false ; 
	}

	__int64 llUserNoteAll  = 0l ; 

	for(int i = 0 ; i<REGION_COUNT; i++)
	{
		llUserNoteAll += m_UserXiaZhu.iXiaZhu[bDeskStation][i] ; 
	}

	if(0 == llUserNoteAll && bDeskStation != m_iZhuangStation) //�����;�����
	{
		return false ; 
	}
	else
	{
		return 	m_bGameStation >= GS_XIAZHU && m_bGameStation < GS_WAIT_NEXT;
	}	
}

/**
* �û��뿪��Ϸ��
*
* @param [in] bDeskStation ���λ��
*
* @param [in] pUserInfo �û�������Ϣ
*
* @return �����û��뿪״̬
*/
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	DebugPrintf("�û��뿪��Ϸ�� - UserLeftDesk(1) - ��Ϸ״̬��%d, ���:%d", m_bGameStation, bDeskStation);
	///�������ҵ���ׯ��Ϣ
	for (int i = 0; i < m_ArrUserList.GetSize(); i++)
	{
		if (m_ArrUserList.GetAt(i) == bDeskStation)
		{
			m_ArrUserList.RemoveAt(i);
			break;
		}
	}

	if(bDeskStation == m_iZhuangStation)
	{
		m_iZhuangStation = -1 ; 
	}

	EZP_APPLY_ZHUANG ApplyZhuang;
	ApplyZhuang.bDeskStation = bDeskStation;
	ApplyZhuang.bIsRobot = TRUE;
	ApplyZhuang.bFlag = FALSE;   // ֪ͨ�ͻ���ɾ����ׯ�û�
	ApplyZhuang.bFirst = FALSE;

	// ������ׯ֪ͨ�������
	for (int j = 0; j < PLAY_COUNT; j++) 
	{
		if(NULL != m_pUserInfo[j])
		{
			SendGameData(j, &ApplyZhuang, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
		}
	}

	SendWatchData(m_bMaxPeople, &ApplyZhuang, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);

	m_iUserNoteZeroCount[bDeskStation] = 0 ;

	m_GameXZInfo[bDeskStation].RemoveAll();
	
	DebugPrintf("�û��뿪��Ϸ�� - UserLeftDesk(2) - ��Ϸ״̬��%d, ���:%d", m_bGameStation, bDeskStation);

	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}
//�û������뿪
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	DebugPrintf("�û������뿪 - UserNetCut(1) - ��Ϸ״̬��%d, ���:%d", m_bGameStation, bDeskStation);
	__super::UserNetCut(bDeskStation,  pLostUserInfo);

	OutputDebugString("wysoutlog::usernetcut");

	m_bUserNetStation[bDeskStation] = true ;  

	for(int i  = 0 ; i<PLAY_COUNT ; i++)
	{
		if(NULL == m_pUserInfo[i])
		{
			continue ; 
		}
		else
		{
			if(false == m_bUserNetStation[i])
			{
				DebugPrintf("�û������뿪 - UserNetCut(2) - ��Ϸ״̬��%d, ���:%d", m_bGameStation, bDeskStation);
				return true;				
			}
		}
	}
	
	DebugPrintf("�û������뿪 - UserNetCut(3) - ��Ϸ״̬��%d, ���:%d", m_bGameStation, bDeskStation);
	GameFinish(0,GFF_FORCE_FINISH);

   return true;
}

/**
* ���ܿ۷�
*
* @return �������ܿ۷�
*/
int CServerGameDesk::GetRunPublish()
{
	return m_iBaseNote;/*m_pDataManage->m_InitData.uRunPublish*/;
}

/**
* ��Ϸ��������
*
* @return ���ص�ǰ��Ϸ����
*/
int CServerGameDesk::GetRoomBasePoint()
{
	int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����
	return (iBasePoint>0?iBasePoint:1);
}

/**
* �������
*
* @param [in] pUserSit �û�������Ϣ
*
* @param [in] pUserInfo �û�������Ϣ
*
* @return �����û�����״̬
*/
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	return __super::UserSitDesk(pUserSit, pUserInfo);
}

/**
* ��ʱ��
*
* @param [in] uTimeID ��ʱ��ID
*
* @param [in] uMaxTime ʱ��(����Ϊ��λ)
*/
void CServerGameDesk::SetGameTimer(UINT uTimeID, UINT uMaxTime)
{
	CTime time = CTime::GetCurrentTime();
	switch (uTimeID)
	{
	case TIME_EMPTY:
		{
			DebugPrintf("TIME_EMPTY - ���ÿ��ж�ʱ�� - ��Ϸ״̬��%d", m_bGameStation);
			break;;
		}
	case TIME_XIAZHU:
		{
			DebugPrintf("TIME_XIAZHU - ������ע��ʱ�� - ��Ϸ״̬��%d", m_bGameStation);
			break;
		}
	case TIME_GAME_FINISH:
		{
			DebugPrintf("TIME_GAME_FINISH - ���ÿ��ƶ�ʱ�� - ��Ϸ״̬��%d", m_bGameStation);
			break;
		}
	default:
		break;
	}

	KillAllTimer();

	m_iTime = uMaxTime;
	SetTimer(uTimeID, 1000);
}


/**
* ������Ӯ����
*
* @param [in] bDeskStation ���λ��
*
* @param [in] pMsg ��Ϣ��
*/
void CServerGameDesk::OnSuperSetting(BYTE bDeskStation, SUPERSETTING * pMsg)
{
	CString s = CINIFile::GetAppPath ();	//����·��
	CINIFile f (s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString key = TEXT("superUserConfig");

	CString strText;
	if (NULL != m_pUserInfo[bDeskStation])
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
			
				memcpy(m_bSuperState, pMsg->bState, sizeof(pMsg->bState));
				SendGameData(bDeskStation,MDM_GM_GAME_NOTIFY,ASS_SUPER_SETTING_SUCCEED,0);
				return ;
			}
		}
	}
}

/**
* ��֤�����û�Ȩ��
*/
void CServerGameDesk::AuthPermissions()
{
	CString s = CINIFile::GetAppPath ();	//����·��
	CINIFile f (s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString key = TEXT("superUserConfig");

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
		int iCount = f.GetKeyVal(key, "SuperUserCount", 5);
		for (int j = 1; j <= iCount; j++)
		{
			strText.Format("SuperUserID%d", j);
			long  lUserID = f.GetKeyVal(key, strText, 0);

			if (m_pUserInfo[i]->m_UserData.dwUserID == lUserID)
			{
				bFind = TRUE;

				// ���û�Ϊ�����û�, ֪ͨ�ͻ���
				superState.bDeskStation = i;
				superState.bEnable = TRUE;  // �����ͻ���
				
				SendGameData(i, &superState, sizeof(superState), MDM_GM_GAME_NOTIFY, ASS_SUPER_STATE, 0);
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

/**
* �����������
*/
void CServerGameDesk::ClearUserInfo(BYTE bDeskStation)
{
	if (bDeskStation == m_iZhuangStation/* || (CountPlayer() - 1) <= 0*/)
	{
		// ����ׯ�ұ���
		m_iZhuangStation = -1;
		m_iZhuangResult = 0;
	}
	else
	{
		// �˳�ʱ������Ƿ���������ׯ
		EZP_APPLY_ZHUANG ApplyZhuang;
		ApplyZhuang.bDeskStation = bDeskStation;
		ApplyZhuang.bIsRobot = TRUE;
		ApplyZhuang.bFlag = FALSE;   //֪ͨ�ͻ���ɾ����ׯ�û�
		OnApplyZhuang(bDeskStation, &ApplyZhuang);
	}

	// ��������Ӯ�ɼ�
	m_iUserResult[bDeskStation] = 0;

	// ��������ע��¼
	m_GameXZInfo[bDeskStation].RemoveAll();
}

/**
* ÿ����Ϸ�����󣬼����ׯ�б�����ҽ���Ƿ񻹹���ׯ����
*/
void CServerGameDesk::CheckAppalyList()
{
	BYTE bDeskStation;
	BOOL bLegal = TRUE; // ��־������б����Ƿ�Ϸ�

	int i = 0, j = 0;
	while (i < m_ArrUserList.GetSize())
	{
		bDeskStation = m_ArrUserList.GetAt(i);

		bLegal = TRUE;

		if (m_pUserInfo[bDeskStation] != NULL)
		{
			if (m_pUserInfo[bDeskStation]->m_UserData.i64Money < m_iApplyMoneyLimit ||m_bUserNetStation[bDeskStation])
			{
				// ���Ϸ�
				bLegal = FALSE;
			}
		}
		else
		{
			// ����Ѿ������ڣ����Ϸ�
			bLegal = FALSE;
		}
		

		if (!bLegal)
		{
			// ���ϸ����ׯ����Զ�������ׯ
			EZP_APPLY_ZHUANG ApplyZhuang;
			ApplyZhuang.bDeskStation = bDeskStation;
			ApplyZhuang.bIsRobot = TRUE;
			ApplyZhuang.bFlag = FALSE;   // ֪ͨ�ͻ���ɾ����ׯ�û�
			ApplyZhuang.bFirst = FALSE;

			// OnApplyZhuang(bDeskStation, &ApplyZhuang);

			// ��ׯ���б����Ƴ������
			m_ArrUserList.RemoveAt(i);

			// ������ׯ֪ͨ�������
			for (j = 0; j < PLAY_COUNT; j++) 
			{
				if(NULL != m_pUserInfo[j])
				{
					SendGameData(j, &ApplyZhuang, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);
				}
			}

			SendWatchData(m_bMaxPeople, &ApplyZhuang, sizeof(EZP_APPLY_ZHUANG), MDM_GM_GAME_NOTIFY, EZP_MSG_APPLYZHUANG, 0);

			// ����б���ɾ��һ����ҵ�����ׯ�б�����ȫ����ǰ��λ, �����ڴ�contiune ����ִ�� i++
			continue ;
		}

		i++;
	}
}

/** 
* ��ȡӮǮ������
*/
void CServerGameDesk::GetWin()
{
	memset(m_iWinQuYu, 0, sizeof(m_iWinQuYu));

	BYTE bStation[4] = {0, GF_DAOMEN, GF_DUIMEN, GF_SHUNMEN};

	// 0λ��Ϊׯ�ң���1λ�ÿ�ʼ����, 1:���� 2������ 3:˳��
	for (BYTE i = 1; i < 4; i++)
	{
		if (BIPAI_SECONED_WIN == m_gameLogic.BiPaiFromStation(0, i))
		{
			// �м���Ӯ
			m_iWinQuYu[bStation[i]] = 1;  // bStation[i]�õ���Ӧ��ע���������λ��(������)
		}
	}

	if (m_iWinQuYu[GF_DAOMEN] && m_iWinQuYu[GF_DUIMEN])
	{
		// �е���-��
		m_iWinQuYu[GF_DAOMEN_JIAO] = 1;
	}
	else if (m_iWinQuYu[GF_DAOMEN] || m_iWinQuYu[GF_DUIMEN])
	{
		// ����Ҳ��Ӯ
		m_iWinQuYu[GF_DAOMEN_JIAO] = 2;
	}

	if (m_iWinQuYu[GF_SHUNMEN] && m_iWinQuYu[GF_DUIMEN])
	{
		// ��˳��-��
		m_iWinQuYu[GF_SHUNMEN_JIAO] = 1;
	}
	else if (m_iWinQuYu[GF_SHUNMEN] || m_iWinQuYu[GF_DUIMEN])
	{
		// ����Ҳ��Ӯ
		m_iWinQuYu[GF_SHUNMEN_JIAO] = 2;
	}

	if (m_iWinQuYu[GF_SHUNMEN] && m_iWinQuYu[GF_DAOMEN])
	{
		// ����
		m_iWinQuYu[GF_DAOMEN_HENG] = 1;
		m_iWinQuYu[GF_SHUNMEN_HNEG] = 1;
	}
	else if (m_iWinQuYu[GF_SHUNMEN] || m_iWinQuYu[GF_DAOMEN])
	{
		// ����Ҳ��Ӯ
		m_iWinQuYu[GF_DAOMEN_HENG] = 2;
		m_iWinQuYu[GF_SHUNMEN_HNEG] = 2;
	}
}

/**
* ��������Ӯ���ƣ���������Ӯ�Զ�����
* @return ����ֵ������ֵ 0 ׯ��1 �У�2 ��
*/
void CServerGameDesk::IAWinAutoCtrl()
{
	// ���û�г����ͻ��˿��Ʋ��ҿ������Զ����ƣ�����л�������Ӯ�Զ�����
	if (!m_bAIWinAndLostAutoCtrl)
	{
		return;
	}

	bool bAiWin = false;
	//�жϸ�Ӯ���Ǹ���
	bAiWin = JudgeShouldWinOrLost();

	if (bAiWin)
	{
		//������ҪӮǮ
		if (CountAIWinMoney() < 0)
		{
			for (int i=1;i<4;i++)
			{			

				// 0λ��Ϊׯ�ң���1λ�ÿ�ʼ����, 1:���� 2������ 3:˳��
				ExChangeCard(0,i);
				// ������ûӮǮ������Ҫ���ƣ����ƺ����ж��Ƿ���ӮǮ������ӮǮ�����������
				if (CountAIWinMoney() >= 0)
				{
					break;
				}
			}	
		}		
	}
	else
	{
		//������Ҫ��Ǯ
		if (CountAIWinMoney() > 0 || (G_i64AIHaveWinMoney +CountAIWinMoney())<0)
		{
			for (int i=1;i<4;i++)
			{			
				//��ʼ����
				ExChangeCard(0,i);
				//������ƺ� ��������Ǯ &&��Ǯ�����½���Ϊ����
				if (CountAIWinMoney()<= 0 && (G_i64AIHaveWinMoney +CountAIWinMoney()) >= 0)
				{
					break;
				}
			}

			//�����ж� �Ƿ��½���Ϊ���� ��ô��ҪȥӮǮ��
			if (CountAIWinMoney()<= 0 && (G_i64AIHaveWinMoney +CountAIWinMoney()) < 0)
			{
				for (int i=1;i<4;i++)
				{			
					// 0λ��Ϊׯ�ң���1λ�ÿ�ʼ����, 1:���� 2������ 3:˳��
					ExChangeCard(0,i);
					// ������ûӮǮ������Ҫ���ƣ����ƺ����ж��Ƿ���ӮǮ������ӮǮ�����������
					if (CountAIWinMoney() >= 0)
					{
						break;
					}
				}
			}
		}
	}
}

/*-------------------------------------------------------------------------------------*/
//�жϸ�Ӯ���Ǹ���
bool	CServerGameDesk::JudgeShouldWinOrLost()
{
	bool bAIWin = false;
	srand((unsigned)GetTickCount());
	int iResult = rand()%100;

	if (0 >=G_i64AIHaveWinMoney )
	{
		//������Ӯ��Ǯ����0 �����˱�Ӯ
		bAIWin = true;		
	}
	else if((0 < G_i64AIHaveWinMoney) && (G_i64AIHaveWinMoney < m_i64AIWantWinMoneyA1))
	{
		// �����˵�ӮǮ��0-A1���� ���Ҹ��ʷ���m_iAIWinLuckyAtA1 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA1)
		{
			bAIWin = true;			
		}
	}
	else if ((m_i64AIWantWinMoneyA1 <= G_i64AIHaveWinMoney)  && (G_i64AIHaveWinMoney <m_i64AIWantWinMoneyA2))
	{
		// �����˵�ӮǮ��A1-A2���� ���Ҹ��ʷ���m_iAIWinLuckyAtA2 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA2)
		{
			bAIWin = true;	
		}
	}
	else if ((m_i64AIWantWinMoneyA2 <= G_i64AIHaveWinMoney)  && (G_i64AIHaveWinMoney <m_i64AIWantWinMoneyA3))
	{
		// �����˵�ӮǮ��A2-A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA3 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA3)
		{
			bAIWin = true;
		}
	}
	else
	{
		// �����˵�ӮǮ����A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA4 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA4)
		{
			bAIWin = true;
		}
	}

	//������ӮǮ+�������Ǯ<0 ��ô������ҪӮǮ
	if( ( G_i64AIHaveWinMoney + CountAIWinMoney() )< 0)
	{
		bAIWin = true;
	}

	return bAIWin;
}
/*-------------------------------------------------------------------------------------*/
//����ָ�����ҵ���
void	CServerGameDesk::ExChangeCard(BYTE byFirstStation, BYTE bySecondStation)
{
	PaiData PaiTmp(m_gameLogic.m_userData[byFirstStation]);
	memcpy(&m_gameLogic.m_userData[byFirstStation], &m_gameLogic.m_userData[bySecondStation], sizeof(PaiData));
	memcpy(&m_gameLogic.m_userData[bySecondStation], &PaiTmp, sizeof(PaiTmp));
}
/*-------------------------------------------------------------------------------------*/
/**
* ��������Ӯ���ƣ����㵱ǰ�ƻ����˵���ӮǮ
* @return ������Ӯ��Ǯ������Ϊ��Ǯ
*/
__int64 CServerGameDesk::CountAIWinMoney()
{
	// ������Ӯ����
	GetWin();

	int i, j;
	__int64 i64Money = 0;

	if (m_iZhuangStation != -1 && m_pUserInfo[m_iZhuangStation] != NULL)
	{
		if (m_bIsRobot[m_iZhuangStation])
		{
			// �����������ׯ��, ������ʵ��ҵ�Ǯ
			for (i = 0; i < PLAY_COUNT; i++)
			{
				if (m_iZhuangStation == i || NULL == m_pUserInfo[i])
				{
					// ��Ҳ����ڲ��ü���
					continue ;
				}

				if (m_bIsRobot[i])
				{
					// �����˲��ü���
					continue ;
				}

				if (FALSE == m_UserXiaZhu.bChipFlag[i])
				{
					// ���û����ע�Ͳ��ü���
					continue ;
				}

				for(j = 0; j < 6; j++)
				{
					// �ж�Ӯ������
					if (0 == m_iWinQuYu[j])
					{
						// ׯ��ӮǮ
						i64Money += m_UserXiaZhu.iXiaZhu[i][j];
					}
					else if (1 == m_iWinQuYu[j])
					{
						// ׯ����Ǯ
						i64Money -= m_UserXiaZhu.iXiaZhu[i][j];
					}
					else if (2 == m_iWinQuYu[j])
					{
						// ����2����Ҳ��Ӯ��ֻ��˳�Žǣ����Žǣ��Ųſ���
					}
				}
			}
		}
		else
		{
			// ��������˲���ׯ��, ��������˵�Ǯ
			for (i = 0; i < PLAY_COUNT; i++)
			{
				if (NULL == m_pUserInfo[i])
				{
					// ��Ҳ����ڲ��ü���
					continue ;
				}

				if (!m_bIsRobot[i])
				{
					// �ǻ����˲�����
					continue;
				}

				for(j = 0; j < 6; j++)
				{
					// �ж�Ӯ������
					if (0 == m_iWinQuYu[j])
					{
						// ��������Ǯ
						i64Money -= m_UserXiaZhu.iXiaZhu[i][j];
					}
					else if (1 == m_iWinQuYu[j])
					{
						// ������ӮǮ
						i64Money += m_UserXiaZhu.iXiaZhu[i][j];
					}
					else if (2 == m_iWinQuYu[j])
					{
						// ����2����Ҳ��Ӯ��ֻ��˳�Žǣ����Žǣ��Ųſ���
					}
				}
			}
		}
	}

	return i64Money;
}
/*-------------------------------------------------------------------------------------*/
//��¼��������Ӯֵ
void	CServerGameDesk::RecordAiHaveWinMoney(__int64 *i64ChangeMoney)
{
	if(!m_bAIWinAndLostAutoCtrl)
	{
		return;
	}

	__int64 i64UserWinMoney = 0;	//��ʵ��ҵ�ӮǮ��
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(NULL == m_pUserInfo[i])
		{
			continue;
		}

		if(m_pUserInfo[i]->m_UserData.isVirtual == 0)
		{
			i64UserWinMoney += i64ChangeMoney[i];
		}
	}

	CString dwjlog;
	dwjlog.Format("dwjbbez::���������ӮǮ��--=%I64d",i64UserWinMoney);
	OutputDebugString(dwjlog);

	//��ǰ���� = ֮ǰ������ӮǮ�� - �������ӮǮ��
	G_i64AIHaveWinMoney -= i64UserWinMoney;

	//������д֮ǰ���¶�ȡ�� �ж��Ƿ��ֶ��޸������˽���
	__int64 iTmpReSetAIHaveWinMoney;
	CString sTemp = CINIFile::GetAppPath (); //����·��
	CINIFile f (sTemp + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString key = TEXT("game");

	key.Format("%s_%d",GET_SKIN_FOLDER(szTempStr),m_pDataManage->m_InitData.uRoomID);
	iTmpReSetAIHaveWinMoney = f.GetKeyVal(key,"ReSetAIHaveWinMoney ",(__int64)0);

	if (G_i64ReSetAIHaveWinMoney != iTmpReSetAIHaveWinMoney)
	{
		//�������ȣ�˵���ֶ��޸��������ļ��е�ֵ���û�������Ǯ�ˣ�����Ⱦ�ȥ�ۼӻ�����Ӯ��Ǯ;
		G_i64AIHaveWinMoney = iTmpReSetAIHaveWinMoney;
		G_i64ReSetAIHaveWinMoney = iTmpReSetAIHaveWinMoney;
	}

	//����ǰ������Ӯ��Ǯд�������ļ�����
	sTemp.Format("%I64d",G_i64AIHaveWinMoney);
	f.SetKeyValString(key,"AIHaveWinMoney ",sTemp);
}
/*-------------------------------------------------------------------------------------*/
///�������ƹ���
void CServerGameDesk::SetCardFun()
{
	CString s = CINIFile::GetAppPath (); //����·��
	CINIFile f (s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString szSec = TEXT("SetCard");

	if(!f.GetKeyVal(szSec,"bSet",0))
	{
		return;
	}

	CString strTemp;
	for(int i = 0; i < 4; ++i)
	{
		for(int b = 0; b < 2; ++b)
		{
			strTemp.Format("CardVal[%d][%d]",i,b);
			m_gameLogic.m_userData[i].paiAry[b] = f.GetKeyVal(szSec,strTemp,0);
		}
	}
}

/*
* ��ӡ��־�ļ�
*/
void CServerGameDesk::DebugPrintf(const char *p, ...)
{
	if(!m_bWriteLog || NULL == m_pDataManage)
	{
		return;
	}

	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() +"\\GameServerLog\\";

	SHCreateDirectoryEx(NULL, strPath, NULL);

	CTime time = CTime::GetCurrentTime();

	sprintf(szFilename, "%s%d_%d_%d_%d%d%d_log.txt", strPath , NAME_ID, m_pDataManage->m_InitData.uRoomID, m_bDeskIndex + 1, time.GetYear(), time.GetMonth(), time.GetDay());
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
	CString s = CINIFile::GetAppPath (); //����·��
	CINIFile f (s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString szSec = TEXT("game");

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
	iTemp = f.GetKeyVal(szSec,"AIWantWinMoney",m_iAIWantWinMoney);			
	if(m_iAIWantWinMoney != iTemp)
	{
		m_iAIHaveWinMoney = 0;
		m_iAIWantWinMoney = iTemp;
	}

	///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	
	iTemp = f.GetKeyVal(szSec,"AIMaxWinInOne",m_iAIMaxWinInOne);	
	m_iAIMaxWinInOne = m_iAIMaxWinInOne == iTemp?m_iAIMaxWinInOne:iTemp;

	// ��ȡ��������ׯ�������
	iTemp = f.GetKeyVal(szSec, "RobotSZCount", m_iRobotSZCount);
	m_iRobotSZCount = m_iRobotSZCount == iTemp?m_iRobotSZCount:iTemp;
	if (m_iRobotSZCount < 2)
	{
		m_iRobotSZCount = 2;
	}
}

//���ݷ�������޸ĵ������ļ�
void CServerGameDesk::LoadModifiedIni(int iRoomID)
{
	TCHAR szKey[20];
	wsprintf(szKey,"%s_%d",GET_SKIN_FOLDER(szTempStr), iRoomID);

	CString s = CINIFile::GetAppPath (); //����·��
	CINIFile f (s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");

	CString szSec = szKey;

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
	iTemp = f.GetKeyVal(szSec,"AIWantWinMoney",m_iAIWantWinMoney);			
	if(m_iAIWantWinMoney != iTemp)
	{
		m_iAIHaveWinMoney = 0;
		m_iAIWantWinMoney = iTemp;
	}

	///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	
	iTemp = f.GetKeyVal(szSec,"AIMaxWinInOne",m_iAIMaxWinInOne);	
	m_iAIMaxWinInOne = m_iAIMaxWinInOne == iTemp?m_iAIMaxWinInOne:iTemp;

	// ��ȡ��������ׯ�������
	iTemp = f.GetKeyVal(szSec, "RobotSZCount", m_iRobotSZCount);
	m_iRobotSZCount = m_iRobotSZCount == iTemp?m_iRobotSZCount:iTemp;
	if (m_iRobotSZCount < 2)
	{
		m_iRobotSZCount = 2;
	}
}
#endif