#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"

void DebugPrintf2(int iRoomID, const char *p, ...)
{
	OutputDebugString("wx DebugPrintf2");
	/*if(!m_bWriteLog)
	{
	return ; 
	}*/
	return;
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
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bGameStation = GS_WAIT_ARGEE;

	::memset(m_bWatchOther,false,sizeof(m_bWatchOther));	/**<�Ƿ�����Թ�*/

	::memset(m_bIsCut,false,sizeof(m_bIsCut));					//�Ƿ��������ҪΪ����������

	m_bSuperSet       = false;
	m_iSuperSet[0]    = 255;
	m_iSuperSet[1]    = 0;

	m_bIsFirstLoadIni = true;				//��һ��ȥ�������ļ�

	memset(m_bIsAuto,false,sizeof(m_bIsAuto));

	memset(m_bIsSuper,false,sizeof(m_bIsSuper));

#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_bSuperCardList[i],0,sizeof(m_bSuperCardList[i]));
	}
	m_bSuperStation = -1;
#endif

	m_bControlWin = 255;
	m_bControlLost = 255;

	m_bWriteLog = false;		/**< �Ƿ��ӡ��־ */

	InitGameData();

}

///��ʼ������
void CServerGameDesk::InitGameData()
{
	m_iSurplusTime = 0;								//ʣ���ʱ��
	::memset(m_bIsPrepare,0,sizeof(m_bIsPrepare));//�Ƿ�׼��
	::memset(m_iUCCount,0,sizeof(m_iUCCount));		//�û������˿���Ŀ

	::memset(m_byUCard,0,sizeof(m_byUCard));		//�û����ϵ��˿�

	::memset(m_iTotalCard,0,sizeof(m_iTotalCard));	//�ܵ���

	::memset(m_bIsInGame,false,sizeof(m_bIsInGame));		//����Ƿ�����Ϸ��

	::memset(m_bBullCard,0,sizeof(m_bBullCard));			//��Ұڵ�ţ��

	::memset(m_byCardShape,-1,sizeof(m_byCardShape));		//�������

	::memset(m_bHaveBaiPai,false,sizeof(m_bHaveBaiPai));	//�Ƿ��Ѿ����� false:û�� true:�Ѿ�����
	//ţţ�������
	m_byCowUserNum = 0;
	//�Ƿ�ţţ���
	memset(m_bIsCowUser, false, sizeof(m_bIsCowUser));
}

//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString key = TEXT("game");

	m_iBaseNote = f.GetKeyVal(key,"BaseNote",1);				//��ע		
	if(m_iBaseNote<1)
	{
		m_iBaseNote=1;
	}
	m_byBTime = f.GetKeyVal(key,"BeginTime",30);				//��ʼʱ��
	if(m_byBTime<10)
	{
		m_byBTime=10;
	}
	m_bySCTime = f.GetKeyVal(key,"SCtime",120);					//����ʱ��
	m_byTTime = f.GetKeyVal(key,"ThinkTime",30);				//��Ϸ˼��ʱ��
	if(m_byTTime<10)
	{
		m_byTTime=10;
	}
	m_byRTime = f.GetKeyVal(key,"ResultTime",15);				//�����ȴ�ʱ��
	if(m_byRTime<10)
	{
		m_byRTime=10;
	}
	m_iAllCardCount = f.GetKeyVal(key,"cardcount",54);

	m_iGoodCard  =  f.GetKeyVal(key,"iGoodCard",0);

	m_bHaveKing  = f.GetKeyVal(key,"HaveKing",1);				//�Ƿ�����

	m_bWriteLog = f.GetKeyVal(key,"WriteLog",false);

	//--add-by-dwj-20121203------
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(key,"AIWinAndLostAutoCtrl",0);		//��������Ӯ����20121122dwj
	m_iAIWantWinMoneyA1	= f.GetKeyVal(key,"AIWantWinMoneyA1 ",__int64(500000));		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(key,"AIWantWinMoneyA2 ",__int64(5000000));		/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(key,"AIWantWinMoneyA3 ",__int64(50000000));		/**<������ӮǮ����3  */
	m_iAIWinLuckyAtA1	= f.GetKeyVal(key,"AIWinLuckyAtA1 ",90);				/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(key,"AIWinLuckyAtA2 ",70);				/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(key,"AIWinLuckyAtA3 ",50);				/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(key,"AIWinLuckyAtA4 ",30);				/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAddMoneyEveryDay	= f.GetKeyVal(key,"AddMoneyEveryDay ",__int64(0));	/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	m_iAIHaveWinMoney	= f.GetKeyVal(key,"AIHaveWinMoney ",__int64(0));	/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */
	m_iReSetAIHaveWinMoney	= f.GetKeyVal(key,"ReSetAIHaveWinMoney ",__int64(0)); //��¼���û�����ӮǮ���������Ϸ�����иı��˾�Ҫ�ı������ӮǮ��;

	//��������
	CString keyName;
	for (int i=0; i<MAX_SHAPE_COUNT; i++)
	{
		keyName.Format("CardShape%d",i);
		m_bCardShapeBase[i]=f.GetKeyVal(key,keyName,1);
		if(m_bCardShapeBase[i]<1)
		{
			m_bCardShapeBase[i] = 1;
		}
		
	}
	m_iCardShape &=0x00000000;
	m_iCardShape |= (f.GetKeyVal(key,"Yn",0)&0xFFFFFFFF);//��ţ
	m_iCardShape |= ((f.GetKeyVal(key,"Jn",0)<<1)&0xFFFFFFFF);//��ţ
	m_iCardShape |= ((f.GetKeyVal(key,"Bomb",0)<<2)&0xFFFFFFFF);//ը��
	m_iCardShape |= ((f.GetKeyVal(key,"Five",0)<<3)&0xFFFFFFFF);//��С

	m_Logic.SetCardShape(m_iCardShape);
	
	return true;
}
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	TCHAR szKey[20];
	wsprintf(szKey,"%s_%d",SKIN_FOLDER,iRoomID);

	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = szKey;

	m_iBaseNote = f.GetKeyVal(key,"BaseNote",m_iBaseNote);		//��ע		
	if(m_iBaseNote<1)
	{
		m_iBaseNote=1;
	}
	m_byBTime = f.GetKeyVal(key,"BeginTime",m_byBTime);			//��ʼʱ��
	if(m_byBTime<10)
	{
		m_byBTime=10;
	}
	m_bySCTime = f.GetKeyVal(key,"SCtime",m_bySCTime);			//����ʱ��

	m_byTTime = f.GetKeyVal(key,"ThinkTime",m_byTTime);			//��Ϸ˼��ʱ��
	if(m_byTTime<10)
	{
		m_byTTime=10;
	}
	m_byRTime = f.GetKeyVal(key,"ResultTime",m_byRTime);		//�����ȴ�ʱ��
	if(m_byRTime<10)
	{
		m_byRTime=10;
	}
	m_iAllCardCount = f.GetKeyVal(key,"CardCount",m_iAllCardCount);

	m_iGoodCard  =  f.GetKeyVal(key,"iGoodCard",m_iGoodCard);

	m_bHaveKing  = f.GetKeyVal(key,"HaveKing",m_bHaveKing);		//�Ƿ�����
	//��������
	CString keyName;
	for (int i=0; i<MAX_SHAPE_COUNT; i++)
	{
		keyName.Format("CardShape%d",i);
		m_bCardShapeBase[i]=f.GetKeyVal(key,keyName,m_bCardShapeBase[i]);
		if(m_bCardShapeBase[i]<1)
		{
			m_bCardShapeBase[i] = 1;
		}
	}
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(key,"AIWinAndLostAutoCtrl",m_bAIWinAndLostAutoCtrl);		//��������Ӯ����20121122dwj
	m_iAIWantWinMoneyA1	= f.GetKeyVal(key,"AIWantWinMoneyA1 ",m_iAIWantWinMoneyA1);		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(key,"AIWantWinMoneyA2 ",m_iAIWantWinMoneyA2);		/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(key,"AIWantWinMoneyA3 ",m_iAIWantWinMoneyA3);		/**<������ӮǮ����3  */
	m_iAIWinLuckyAtA1	= f.GetKeyVal(key,"AIWinLuckyAtA1 ",m_iAIWinLuckyAtA1);				/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(key,"AIWinLuckyAtA2 ",m_iAIWinLuckyAtA2);				/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(key,"AIWinLuckyAtA3 ",m_iAIWinLuckyAtA3);				/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(key,"AIWinLuckyAtA4 ",m_iAIWinLuckyAtA4);				/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAddMoneyEveryDay	= f.GetKeyVal(key,"AddMoneyEveryDay ",m_iAddMoneyEveryDay);	/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	m_iReSetAIHaveWinMoney	= f.GetKeyVal(key,"ReSetAIHaveWinMoney ",m_iReSetAIHaveWinMoney); //��¼���û�����ӮǮ���������Ϸ�����иı��˾�Ҫ�ı������ӮǮ��;
	m_iAIHaveWinMoney		= f.GetKeyVal(key,"AIHaveWinMoney",m_iAIHaveWinMoney); //������ӮǮ��20121122dwj
	return TRUE;
}

//-------------------------------add-by-DWJ-20121204-----------------------------------------------------------------------------------------
//�������ļ��ж�ȡ�������Ѿ�Ӯ�˶���Ǯ����Ϊ��ÿ�̽����ʱ�򶼻Ὣ������ӮǮ����д�������ļ��С� ;
//������ÿ�̿�ʼ��ʱ���ֶ�ȡ�£���Ϊ�������޸�����ô�ͻ�������Ǯ��.;
void CServerGameDesk::GetAIContrlSetFromIni(int iRoomID)
{
	CString s = CINIFile::GetAppPath ();/////����·��;
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	CString szSec = TEXT("game");
	szSec.Format("%s_%d",SKIN_FOLDER,iRoomID);

	m_iAIWantWinMoneyA1	= f.GetKeyVal(szSec,"AIWantWinMoneyA1 ",m_iAIWantWinMoneyA1);		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(szSec,"AIWantWinMoneyA2 ",m_iAIWantWinMoneyA2);		/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(szSec,"AIWantWinMoneyA3 ",m_iAIWantWinMoneyA3);		/**<������ӮǮ����3  */
	m_iAIWinLuckyAtA1	= f.GetKeyVal(szSec,"AIWinLuckyAtA1 ",m_iAIWinLuckyAtA1);			/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(szSec,"AIWinLuckyAtA2 ",m_iAIWinLuckyAtA2);			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(szSec,"AIWinLuckyAtA3 ",m_iAIWinLuckyAtA3);			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(szSec,"AIWinLuckyAtA4 ",m_iAIWinLuckyAtA4);			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAddMoneyEveryDay	= f.GetKeyVal(szSec,"AddMoneyEveryDay ",m_iAddMoneyEveryDay);		/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	//m_iAIHaveWinMoney	= f.GetKeyVal(szSec,"AIHaveWinMoney ",m_iAIHaveWinMoney);		/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */

	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",m_bAIWinAndLostAutoCtrl);//��������Ӯ����20121122dwj
	CheckNewDayReSetIni(&f,&szSec);
}
//---------------------------add-by-DWJ-20121113---------------------------------------------------------
//����Ƿ����һ��,����һ���Ҫ�޸������ļ���A1~A3�����ֵ;
void CServerGameDesk::CheckNewDayReSetIni(CINIFile *f, CString *key)
{	
	CTime Cttm;
	CString sDay,sValue;
	Cttm=CTime::GetCurrentTime();
	sDay  = Cttm.Format(("%Y%m%d"));
	if (m_bIsFirstLoadIni)
	{//��һ�ζ�ȡ�����ļ�������ֻ������ȡһ��;
		m_iRecFroDayTime  = StrToInt(sDay);
		m_bIsFirstLoadIni = false;
	}
	if (m_iRecFroDayTime < StrToInt(sDay))
	{//�����¼��ʱ��С�ڵ�ǰ�����ڣ���ʾ��ǰʱ�����24�㣬�����Ѿ���һ��,���µ�һ�졣;
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



//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//��ʼ��
bool CServerGameDesk::InitDeskGameStation()
{
	LoadIni();		//���������ļ�
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);	//���¼��������ļ������
	return true;
}
//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	DebugPrintf("��ʱ��  ID=��%d��",uTimerID);
	switch(uTimerID)
	{
	case TIME_WAIT_THINK:			//�ȴ�������Ϸ״̬
		{
			KillTimer(TIME_WAIT_THINK);

			m_bGameStation = GS_PLAY_GAME;		//��Ϸ��״̬

			SetGameTimer(TIME_THINK,m_byTTime,true);

			break;
		}
	case TIME_THINK:				//����˼��ʱ��
		{
			m_iSurplusTime--;

			if(m_iSurplusTime<= 0)
			{
				KillTimer(TIME_THINK);

				for(int i = 0; i < PLAY_COUNT; ++i)
				{
					AutoTanPai(i);			//ʱ�䵽���Զ�����	
				}
			}

			break;
		}
	default:break;
	}

	return __super::OnTimer(uTimerID);
}

//�����Ϣ������
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	DebugPrintf("���յ������Ϣ λ��=��%d�� ID=��%d��",bDeskStation,pNetHead->bAssistantID);

	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{
			char cKey[10];
			CString sPath=CBcfFile::GetAppPath();
			int iResult = 1;
			if (CBcfFile::IsFileExist(sPath + "SpecialRule.bcf"))
			{
				CBcfFile fsr( sPath + "SpecialRule.bcf");
				sprintf(cKey, "%d", NAME_ID);
				iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
			}

			if(iResult == 0)
			{
				UserQuitStruct s_UserQuit;
				s_UserQuit.bDeskStation = bDeskStation;
				for(int i = 0; i < PLAY_COUNT; ++i)
				{
					if(i != bDeskStation);
					{
						SendGameData(i,&s_UserQuit,sizeof(UserQuitStruct),MDM_GM_GAME_NOTIFY,ASS_USER_QUIT_GAME,0);
					}
				}
				SendWatchData(m_bMaxPeople,&s_UserQuit,sizeof(UserQuitStruct),MDM_GM_GAME_NOTIFY,ASS_USER_QUIT_GAME,0);

				return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser); 
			}

			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	DebugPrintf("���յ��ͻ�����Ϣ λ��=��%d�� ID=��%d��",bDeskStation,pNetHead->bAssistantID);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if (bWatchUser)
				return TRUE;

			//�������
			if ((m_bGameStation!=GS_WAIT_ARGEE)&&(m_bGameStation!=GS_WAIT_NEXT)) 
				return TRUE;

			if(m_bGameStation!=GS_WAIT_NEXT)
			{
				m_bGameStation=GS_WAIT_ARGEE;
			}
			m_bIsPrepare[bDeskStation]=true;//�Ƿ�׼��
			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
	case ASS_TAN_PAI:		//̯����Ϣ
		{
			if(bWatchUser || uSize != sizeof(TanPaiStruct))
			{
				return true;
			}

			TanPaiStruct *pTanPai = (TanPaiStruct *)pData;

			HandTanPaiAction(pTanPai->byStaiton,pTanPai->byDownCard);

			return true;
		}
	case ASS_WATCH_OTHER:		//�Թ���Ϣ
		{
			if(bWatchUser || uSize!=sizeof(WatchOtherStruct))
			{
				return true;
			}

			WatchOtherStruct *pWatchOther = (WatchOtherStruct *)pData;

			m_bWatchOther[bDeskStation] = pWatchOther->bWatch;

			return true;
		}
	case ASS_AUTO:	//����й�
		{
			if(bWatchUser || uSize!=sizeof(AutoCardStruct))
			{
				return true;
			}

			UserAuto(bDeskStation,(AutoCardStruct*)pData);

			return true;
		}
	case ASS_SUPER://��������
		{
			if(bWatchUser || uSize != sizeof(SuperStruct))
			{
				return true;
			}

			SuperStruct *pCmd = (SuperStruct *)pData;

			HandleSuperInfo(bDeskStation,(SuperStruct*)pData);

			return true;
		}
	default:break;
	}

	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	m_bIsCut[bDeskStation] = false;

	SuperLoad();

	DebugPrintf("�û�����  λ��=��%d��״̬=��%d��",bDeskStation,m_bGameStation);

	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;

			GetIniData(GameStation.s_Data);		//��ȡ��ȡINI�ļ�����

			GetBaseData(GameStation.sBData, bDeskStation);	//��ȡ��Ϸ��������

			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			
			break;
		}
	case GS_SEND_CARD:		//����״̬
		{
			GameStation_3 GameStation;

			GetIniData(GameStation.s_Data);		//��ȡ��ȡINI�ļ�����

			GetBaseData(GameStation.sBData, bDeskStation);	//��ȡ��Ϸ��������

			::CopyMemory(GameStation.iUCCount,m_iUCCount,sizeof(m_iUCCount));

			::CopyMemory(GameStation.byUCard,m_byUCard[bDeskStation],sizeof(m_byUCard[bDeskStation]));

			//������ �Թ�
			if(bWatchUser && !m_bWatchOther[bDeskStation])
			{
				::memset(GameStation.byUCard,0x4D,sizeof(GameStation.byUCard));	
			}
	
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			
			break;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			GameStation_4 GameStation;

			GetIniData(GameStation.s_Data);		//��ȡ��ȡINI�ļ�����

			GetBaseData(GameStation.sBData, bDeskStation);	//��ȡ��Ϸ��������

			::CopyMemory(GameStation.iUCCount,m_iUCCount,sizeof(m_iUCCount));

			::CopyMemory(GameStation.byUCard,m_byUCard[bDeskStation],sizeof(m_byUCard[bDeskStation]));

			::CopyMemory(GameStation.byDownCard,m_bBullCard[bDeskStation],sizeof(m_bBullCard[bDeskStation]));

			GameStation.iSurplusTime = m_iSurplusTime;		//ʣ���ʱ��

			//������ �Թ�
			if(bWatchUser && !m_bWatchOther[bDeskStation])
			{
				::memset(GameStation.byUCard,0x4D,sizeof(GameStation.byUCard));	
			}
	
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			
			break;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_5 GameStation;

			GetIniData(GameStation.s_Data);		//��ȡ��ȡINI�ļ�����

			GetBaseData(GameStation.sBData, bDeskStation);	//��ȡ��Ϸ��������
	
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			
			break;
		}
	}

	return true;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	InitGameData();

	return true;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		GameFinish(0,GF_SALE);
		return false;
	}

	m_bGameStation = GS_SEND_CARD;				//��Ϸ״̬

	//-ADD--BY---DWJ-20121203
	//����ÿ�̸��ݷ���ID��ȡ�������ļ��е�ReSetAIHaveWinMoney ���� �趨�»�����Ӯ�˶���Ǯ;
	GetAIContrlSetFromIni(m_pDataManage->m_InitData.uRoomID);

	//ţţ�������
	m_byCowUserNum = 0;
	//�Ƿ�ţţ���
	memset(m_bIsCowUser, false, sizeof(m_bIsCowUser));

	for (int i = 0; i < PLAY_COUNT; ++i) 
	{
		if(m_pUserInfo[i])
		{
			m_bIsInGame[i] = true;
		}
	}

	//�ַ��˿�
	m_Logic.RandCard(m_iTotalCard,m_iAllCardCount,m_bHaveKing);
	int temp = rand()%100;

	//ϴ������
	if (temp < m_iGoodCard)
	{
		for (int i=0; i<20; i++)
		{
			if (GetMaxCardShape(m_iTotalCard,m_iAllCardCount) > UG_BULL_SEVEN)
			{
				break;
			}

			m_Logic.RandCard(m_iTotalCard,m_iAllCardCount,m_bHaveKing);
		}
	}

	srand(GetTickCount());

	m_bGameStation = GS_SEND_CARD;

	BeginUpgradeStruct begindata;

	::CopyMemory(begindata.bIsInGame,m_bIsInGame,sizeof(m_bIsInGame));

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(NULL == m_pUserInfo[i])
			continue;
	
		SendGameData(i,&begindata,sizeof(BeginUpgradeStruct),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);
	}
	SendWatchData(m_bMaxPeople,&begindata,sizeof(BeginUpgradeStruct),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);

	SendCard();

	DebugPrintf("��Ϸ��ʼ__ happy landowner gamebing end!");

	return true;
}

//�����˿˸��û�
BOOL CServerGameDesk::SendCard()
{
	int iCardPos = 0;	//��ǰ����λ��;
	//20121204DWJ û�л����˾Ͳ�ȥ������;
	bool bAllUser = true;//ȫ�����������Ͳ�������;
	bool bALLAi	  = true;//ȫ�ǻ�����;
	for (int i =0 ;i< PLAY_COUNT ; i++)
	{
		if(NULL != m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual > 0 )
		{// �л����˴��ڣ���ȥ����;
			bAllUser = false;
			break;
		}
		
	}
	for (int i =0 ;i< PLAY_COUNT ; i++)
	{
		if (NULL != m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual < 1)
		{//ȫ�ǻ����ˣ�����Ҵ��ڣ���ȥ����;
			bALLAi	= false;
			break;
		}
	}

	if (m_bSuperSet)
	{
		SuperSet();
		//ÿ��ֻ��������ʼ��
		m_bSuperSet       = false;
		m_iSuperSet[0]    = 255;
		m_iSuperSet[1]    = 0;
	}
	else if (m_bAIWinAndLostAutoCtrl && !m_bSuperSet && !bAllUser && !bALLAi)
	{//�����˿�����Ӯ dwj 20121203 �����˿�����Ӯ���� û�г��˿��ơ��л�������������;
		IAWinAutoCtrl();
	}

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(!m_bIsInGame[i])
			continue;

		::CopyMemory(m_byUCard[i],&m_iTotalCard[iCardPos],sizeof(BYTE)*USER_HAND_CARD);

		m_iUCCount[i] = USER_HAND_CARD;

		iCardPos += USER_HAND_CARD;
	}
	
	//dddwwwjjj;
	//BYTE byTemp1[PLAY_COUNT][USER_HAND_CARD] = {{0x03,0x3C,0x29,0x02,0x07},{0x0D,0x0C,0x0A,0x15,0x3A},{0x4e,0x03,0x3d,0x08,0x0b},
	//											{0x07,0x01,0x4f,0x17,0x06},{0x32,0x14,0x26,0x18,0x38},{0x16,0x24,0x0B,0x2A,0x09}};
	//for(int i = 0; i < PLAY_COUNT; ++i)
	//{
	//	if(NULL == m_pUserInfo[i])
	//		continue;
	//	
	//	::CopyMemory(m_byUCard[i],byTemp1[i],sizeof(BYTE)*USER_HAND_CARD);
	//}

	SendCardStruct  sCardData;
	::CopyMemory(sCardData.iUCCount,m_iUCCount,sizeof(m_iUCCount));

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(NULL == m_pUserInfo[i])
			continue;

		::CopyMemory(sCardData.byUCard[i],m_byUCard[i],sizeof(m_byUCard[i]));
		
		SendGameData(i,&sCardData,sizeof(sCardData),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);

		//������ �Թ�
		if(!m_bWatchOther[i])
		{
			::memset(sCardData.byUCard[i],0x4D,sizeof(sCardData.byUCard[i]));	
		}
	
		SendWatchData(i,&sCardData,sizeof(sCardData),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);

		::ZeroMemory(sCardData.byUCard[i],sizeof(sCardData.byUCard[i]));	
	}

	SetGameTimer(TIME_WAIT_THINK,1250,false);		//���Ͽͻ��˷���ʱ��

	return TRUE;
}
//------------------------------------------------------------------------------
/*�����˿�����Ӯ 
add by dwj 20121203;
*/
void CServerGameDesk::IAWinAutoCtrl()
{
	if(!m_bAIWinAndLostAutoCtrl)
		return;

	bool bAIWin = false;
	srand((unsigned)GetTickCount());
	int iResult = rand()%100;

	if (0 >=m_iAIHaveWinMoney )
	{//������Ӯ��Ǯ����0 �����˱�Ӯ
		bAIWin = true;		
	}
	else if((0 < m_iAIHaveWinMoney) && (m_iAIHaveWinMoney < m_iAIWantWinMoneyA1))
	{// �����˵�ӮǮ��0-A1���� ���Ҹ��ʷ���m_iAIWinLuckyAtA1 ������ҪӮ;
		if (iResult <m_iAIWinLuckyAtA1)
			bAIWin = true;								
	}
	else if ((m_iAIWantWinMoneyA1 <= m_iAIHaveWinMoney)  && (m_iAIHaveWinMoney <m_iAIWantWinMoneyA2))
	{// �����˵�ӮǮ��A1-A2���� ���Ҹ��ʷ���m_iAIWinLuckyAtA2 ������ҪӮ;
		if (iResult <m_iAIWinLuckyAtA2)
			bAIWin = true;			
	}
	else if ((m_iAIWantWinMoneyA2 <= m_iAIHaveWinMoney)  && (m_iAIHaveWinMoney <m_iAIWantWinMoneyA3))
	{// �����˵�ӮǮ��A2-A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA3 ������ҪӮ;
		if (iResult <m_iAIWinLuckyAtA3)
			bAIWin = true;
	}
	else
	{// �����˵�ӮǮ����A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA4 ������ҪӮ;
		if (iResult <m_iAIWinLuckyAtA4)
			bAIWin = true;
	}
	if (bAIWin)
	{//������ҪӮǮ
		
		if (CountAIWinMoney() < 0)
		{
			for (int i=0;i<PLAY_COUNT;i++)
			{
				for(int j = i+1; j <  PLAY_COUNT; j++)
				{
					ChangeCard(i,j);
					if(CountAIWinMoney()>=0)
					{
						return;
					}
				}
			}	
		}		
	}
	else
	{//������Ҫ��Ǯ
		OutputDebugString("dwjlog::������Ҫ��Ǯ");
		if (CountAIWinMoney() > 0 || (m_iAIHaveWinMoney +CountAIWinMoney())<0)
		{
			for (int i=0;i<PLAY_COUNT;i++)
			{	
				for(int j = i+1; j <  PLAY_COUNT; j++)
				{
					ChangeCard(i,j);
					if (CountAIWinMoney()<= 0)
					{
						if ((m_iAIHaveWinMoney +CountAIWinMoney())<0)
						{//���������������Ǯ�� ���»�������ӮǮ��Ϊ��������ô���»���;
								continue;
						}
						else
						{
							return ;
						}
					}
				}
			}
			for (int i=0;i<PLAY_COUNT;i++)
			{
				for(int j = i+1; j <  PLAY_COUNT; j++)
				{
					ChangeCard(i,j);
					if(CountAIWinMoney()>=0)
					{
						return;
					}
				}
			}	

		}
	}
}
//-----------------------------------------------------------------------------
/*
ADD BY DWJ 20121103
���ݵ�ǰ�������������������������ӮǮ������Ǯ����;
*/
int CServerGameDesk::CountAIWinMoney()
{
	int money = 0;
	BYTE iUserCard[PLAY_COUNT][USER_HAND_CARD];	//��ʱ�������ҵ���;
	BYTE byDownBCard[PLAY_COUNT][USER_HAND_CARD];	///�¶���;
	BYTE iShape[PLAY_COUNT];					//��������;
	memset(byDownBCard,0,sizeof(byDownBCard));		///�����ţ��;
	memset(&iUserCard, 0, sizeof(iUserCard));
	memset(&iShape, 0, sizeof(iShape));

	int iCardPos = 0;
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(!m_bIsInGame[i])
			continue;

		CopyMemory(iUserCard[i],&m_iTotalCard[iCardPos],sizeof(BYTE)*USER_HAND_CARD);
		//m_iUCCount[i] = USER_HAND_CARD;
		iCardPos += USER_HAND_CARD;
		//�������ҵ��������ڱȽϴ�С;
		m_Logic.GetBull(iUserCard[i],SH_USER_CARD, byDownBCard[i]);
		//iShape[i] = m_Logic.GetShape(iUserCard[i],SH_USER_CARD);m_byUCard[PLAY_COUNT][USER_HAND_CARD];	//�û����ϵ��˿�
		iShape[i] = m_Logic.GetShape(iUserCard[i],SH_USER_CARD,byDownBCard[i]);
	}

	BYTE bBeginDesk;
	//��¼�£����ĸ����Ӻſ�ʼ����;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_bIsInGame[i])
		{
			bBeginDesk = i;
			break;
		}
	}
	BYTE bMaxDesk, bMixDesk;
	bMaxDesk = bBeginDesk;
	bMixDesk = bBeginDesk;
	//��ʼ�Ƚ�����ƺ���С�ƣ��������¼����;
	for (int i=bBeginDesk+1; i<PLAY_COUNT; i++)
	{
		if (!m_bIsInGame[i])
		{//�����ڵ���ҾͲ����ƴ�С��;			
			continue;
		}
		if (m_Logic.CompareCard(iUserCard[bMaxDesk],SH_USER_CARD, byDownBCard[bMaxDesk],iUserCard[i],SH_USER_CARD,byDownBCard[i])==-1)
		{
			bMaxDesk = i;
		}
		if (m_Logic.CompareCard(iUserCard[bMixDesk],SH_USER_CARD, byDownBCard[bMixDesk],iUserCard[i],SH_USER_CARD,byDownBCard[i])==1)
		{
			bMixDesk = i;
		}
	}
	
	if (m_pUserInfo[bMaxDesk]->m_UserData.isVirtual)
	{//������ڻ��������� ,��ô�����˾���ӮǮ�� ��ôֻ������ҵ�Ǯ;
		
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL != m_pUserInfo[i] && !m_pUserInfo[i]->m_UserData.isVirtual)
			{//���;
				money += m_iBaseNote;	
			}
		}
	}
	else
	{//�������������� �ͼ����������Ǯ����;
		
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL != m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual)
			{//���;	
				money -= m_iBaseNote;	
			}
		}
	}
	if (iShape[bMaxDesk] == UG_BULL_BULL)
	{//�����ţţ����2��
		money = 2*money;
	}
	return money;
}
//--------------------------------------------------------------------------------
//�������̯�ƶ���;
void CServerGameDesk::HandTanPaiAction(BYTE byDStation,BYTE byDownCard[])
{
	//��ȫ�ж�
	if(!m_bIsInGame[byDStation] || NULL == byDownCard || m_bHaveBaiPai[byDStation] || m_bGameStation != GS_PLAY_GAME)
	{
		return;
	}

	::CopyMemory(m_bBullCard[byDStation],byDownCard,sizeof(BYTE)*USER_HAND_CARD);		//��������

	m_byCardShape[byDStation] = m_Logic.GetShape(m_byUCard[byDStation],m_iUCCount[byDStation],byDownCard);

	TanPaiStruct tanpai;
	
	tanpai.byStaiton = byDStation;
	tanpai.byShape = m_byCardShape[byDStation];

	if(m_bWatchOther[byDStation])
	{
		::CopyMemory(tanpai.byDownCard,m_bBullCard[byDStation],sizeof(m_bBullCard[byDStation]));
	}
		
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&tanpai,sizeof(tanpai),MDM_GM_GAME_NOTIFY,ASS_TAN_PAI,0);
		}
	}
	SendWatchData(m_bMaxPeople,&tanpai,sizeof(tanpai),MDM_GM_GAME_NOTIFY,ASS_TAN_PAI,0);

	m_bHaveBaiPai[byDStation] = true;

	//����Ƿ������˶���̯��
	if(CheckTanPaiFinish())
	{
		GameFinish(byDStation,GF_NORMAL);
	}
}

//�Զ�̯��
void CServerGameDesk::AutoTanPai(BYTE byDStation)
{
	if(m_bGameStation != GS_PLAY_GAME || !m_bIsInGame[byDStation] || m_bHaveBaiPai[byDStation])
	{
		return;
	}

	BYTE byDownCard[USER_HAND_CARD] = {0};

	m_Logic.GetBull(m_byUCard[byDStation],m_iUCCount[byDStation],byDownCard);

	HandTanPaiAction(byDStation,byDownCard);	
}

//���̯���Ƿ����
BOOL CServerGameDesk::CheckTanPaiFinish()
{
	int iTempCount = 0;

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(m_bHaveBaiPai[i])
		{
			iTempCount++;
		}
	}

	if(iTempCount == CountPlayer())
	{
		return TRUE;
	}

	return FALSE;
}

///���ʤ�����
BYTE CServerGameDesk::GetWinPeo()
{
	//������Ϸ�е���ҷ���һ��������
	int iArray[PLAY_COUNT] = {0};
	int iIndex = 0;

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(m_bIsInGame[i])
		{
			iArray[iIndex] = i;
			iIndex++;
		}
	}

	BYTE byTempPeo = iArray[0];
	for(int i = 1; i < iIndex; ++i)
	{
		if(m_Logic.CompareCard(m_byUCard[byTempPeo],m_iUCCount[byTempPeo],m_bBullCard[byTempPeo],
								m_byUCard[iArray[i]],m_iUCCount[iArray[i]],m_bBullCard[iArray[i]]) != 1)
		{
			byTempPeo = iArray[i];
		}
	}
	
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			if (m_Logic.GetShape(m_byUCard[i], m_iUCCount[i], m_bBullCard[i]) == UG_BULL_BULL)
			{
				m_byCowUserNum ++ ;
				m_bIsCowUser[i] = true;
			}
		}
	}

	return byTempPeo;
}

///����������
void CServerGameDesk::CountResultScore(BYTE bDeskStation,GameEndStruct *pGameEnd,BYTE bCloseFlag)
{
	__int64 iTempPoint[PLAY_COUNT];
	::memset(iTempPoint,0,sizeof(iTempPoint));

	switch(bCloseFlag)
	{
	case GF_NORMAL:		//����������Ϸ
		{
			BYTE byWinPeo = GetWinPeo(); 
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(i == byWinPeo || !m_bIsInGame[i])
					continue;

				iTempPoint[i] = -(m_bCardShapeBase[m_byCardShape[byWinPeo]]*m_iBaseNote);
				iTempPoint[byWinPeo] += -iTempPoint[i];
			}

			pGameEnd->byCowUserNum = m_byCowUserNum;
			memcpy(pGameEnd->bIsCowUser, m_bIsCowUser, sizeof(pGameEnd->bIsCowUser));
			break;
		}
	case GFF_FORCE_FINISH:		//ǿ�˽�����Ϸ
		{
			iTempPoint[bDeskStation] = -GetRunPublish();
			break;
		}
	default:
		{
			break;
		}
	}
	//��¼��������Ӯ�����һ��Ҫ�ڿ�˰֮ǰ�޸ģ������������Ӯ��ƽ��;
	RecordAiHaveWinMoney(iTempPoint);

	ChangeUserPointint64(iTempPoint, m_bIsCut);

	__int64 iTempMoney[PLAY_COUNT] = {0};
	__super::RecoderGameInfo(iTempMoney);	

	if(1 != m_pDataManage->m_InitData.uComType)	//��ҳ�
	{
		::CopyMemory(pGameEnd->iMoney,iTempMoney,sizeof(iTempMoney));
	}
	else										//���ֳ�
	{
		::CopyMemory(pGameEnd->iMoney,iTempPoint,sizeof(iTempPoint));
	}
	::CopyMemory(pGameEnd->iCardShape,m_byCardShape,sizeof(m_byCardShape));
	::CopyMemory(pGameEnd->byDownCard,m_bBullCard,sizeof(m_bBullCard));
	::CopyMemory(pGameEnd->byUCard,m_byUCard,sizeof(m_byUCard));
}
//------------------------------------------------------------------------------------
void CServerGameDesk::RecordAiHaveWinMoney(__int64 iTempPoint[PLAY_COUNT])
{
	//bool bAllAi = true;//�����˺ͻ�������  �Ͳ�ȥ����;
	//for (int i =0 ;i< PLAY_COUNT ; i++)
	//{
	//	if(NULL != m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual < 1)
	//	{// ���������ң���ô��ȥ���㣬�����������ң�˵��ȫ�ǻ����ˣ��Ͳ���ȥ������ ;
	//		bAllAi = false;
	//		break;
	//	}
	//}
	//20121203dwj ���������ӮǮ�����; �����˿����˲���������������棨ȫ�ǻ����˾Ͳ�ȥ�����ˣ�
	if (/*!bAllAi &&*/ m_bAIWinAndLostAutoCtrl)
	{
		try
		{//�����쳣������Ϊͬ��ţţ�漰��������Ӷ�ͬһ�������ļ���д��������������ˣ��Ͳ��������ļ���д����;
			//----DWJ-20121203---��
			__int64 iReSetAIHaveWinMoney; //���û�����ӮǮ��Ŀ����ʱ����
			__int64 jqrMoney = 0,wjMoney = 0;
			CString sTemp = CINIFile::GetAppPath ();/////����·����
			CINIFile f(sTemp +SKIN_FOLDER  + _T("_s.ini"));
			TCHAR szSec[_MAX_PATH] = TEXT("game");
			_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);
			iReSetAIHaveWinMoney = f.GetKeyVal(szSec,"ReSetAIHaveWinMoney ",(__int64)0);

			if (m_iReSetAIHaveWinMoney != iReSetAIHaveWinMoney)
			{//�������ȣ�˵���ֶ��޸��������ļ��е�ֵ���û�������Ǯ�ˣ�����Ⱦ�ȥ�ۼӻ�����Ӯ��Ǯ;
				m_iAIHaveWinMoney = iReSetAIHaveWinMoney;
				m_iReSetAIHaveWinMoney = iReSetAIHaveWinMoney;
			}
			else
			{
				for(int i=0;i<PLAY_COUNT;i++)
				{
					//m_iUserFen[i] = m_iUserMoney[i];
					if(NULL != m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual > 0)///��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
					{//ֻ���������Ӯ;
						//m_iAIHaveWinMoney += GameEnd.iTurePoint[i];	
						m_iAIHaveWinMoney += iTempPoint[i];///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ
						jqrMoney += iTempPoint[i];
					}
					else
					{
						wjMoney += iTempPoint[i];
					}
				}
			}
			CTime time = CTime::GetCurrentTime();
			DebugPrintf2(m_pDataManage->m_InitData.uRoomID,"����ʱ��[%d:%d:%d]::�������Ѿ�Ӯ�Ľ��%I64d ������Ӯ�Ľ��%I64d,���Ӯ�Ľ��%I64d",
				time.GetHour(),time.GetMinute(),time.GetSecond(),m_iAIHaveWinMoney,jqrMoney ,wjMoney) ;
			//����ǰ������Ӯ��Ǯд�������ļ�����;
			sTemp.Format("%I64d",m_iAIHaveWinMoney);
			f.SetKeyValString(szSec,"AIHaveWinMoney ",sTemp);
		}
		catch (...)
		{
			//����ֻ�����������ʩ��ʲôҲ������DWJ
			OutputDebugString("dwjlog::д�����ļ�������!");
		}			
	}
}
//------------------------------------------------------------------------------------
//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{

	DebugPrintf("happy landowner GameFinish ������ҡ�%d��,�������͡�%d��",bDeskStation,bCloseFlag);

	KillAllTimer();
	
	GameEndStruct s_gameend;

	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			CountResultScore(bDeskStation,&s_gameend,GF_NORMAL);	//����÷�

			m_bGameStation = GS_WAIT_ARGEE;

			break;
		}
	case GFF_FORCE_FINISH:	//ǿ���˳�����
		{
			CountResultScore(bDeskStation,&s_gameend,GFF_FORCE_FINISH);	//����÷�

			m_bGameStation = GS_WAIT_ARGEE;	//��Ϸ״̬ �ȴ���һ�̿�ʼ

			bCloseFlag = GF_SALE;

			break;
		}
	case GF_SALE:			//��Ϸ��ȫ����
		{
			m_bGameStation = GS_WAIT_ARGEE;

			break;
		}

	default:break;
	}
	memcpy(s_gameend.bIsPrepare,m_bIsPrepare,sizeof(s_gameend.bIsPrepare));
	//��������
	for (int i=0; i<PLAY_COUNT; ++i) 
	{
		if(NULL == m_pUserInfo[i])
			continue;

		SendGameData(i,&s_gameend,sizeof(GameEndStruct),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
	}
	SendWatchData(m_bMaxPeople,&s_gameend,sizeof(GameEndStruct),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);

	ReSetGameState(bCloseFlag);	//��������

	__super::GameFinish(bDeskStation,bCloseFlag);

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(NULL == m_pUserInfo[i])
		{
			m_bIsAuto[i] = false;
		}
	}
	
	return true;
}

BOOL CServerGameDesk::SuperLoad()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("SuperSet");
	int count = f.GetKeyVal(key,"SuperCount",0);
	if(count <= 0 )					//��ʤ��
		return FALSE;
	TCHAR szName[21];
	int iUserID = 0;
	memset(m_bIsSuper,0,sizeof(m_bIsSuper));
	for (int j = 0; j < PLAY_COUNT; j++)
	{
		if(m_pUserInfo[j] == NULL)
			continue;

		iUserID = 0;
		for(int i = 0; i < count; i ++)
		{
			wsprintf(szName,"SuperUserID%d",i);
			iUserID = f.GetKeyVal(key,szName,0);

			if(iUserID == m_pUserInfo[j]->m_UserData.dwUserID)
			{
				m_bIsSuper[j] = true;
				break;
			}
		}

	}
	return true;
}

//���ݳ���������,ʹ��ָ������仹��Ӯ
void CServerGameDesk::SuperSet()
{
	CString sdp;
	sdp.Format("sdp_���� ����� m_iSuperSet[1] = %d",m_iSuperSet[1]);
	OutputDebugString(sdp);

	bool bWin = false;
	if (m_iSuperSet[1] == 1)
	{
		bWin = true;
	}
	else if (m_iSuperSet[1] == 2)
	{
		bWin = false;
	}
	else
	{
		return;
	}

	if (bWin)
	{//ָ�����ҪӮǮ
		if (CountSuperWinMoney() <= 0)
		{
			for (int i=0;i<PLAY_COUNT;i++)
			{
				for(int j = i+1; j <  PLAY_COUNT; j++)
				{
					ChangeCard(i,j);
					if(CountSuperWinMoney()>0)
					{
						OutputDebugString("sdp_���� ����� [Ӯ]�ҵ�Ӯ���ƣ�");
						return;
					}
				}
			}
			//����0�Ҳ������ҵ���0�����
			for (int i=0;i<PLAY_COUNT;i++)
			{
				for(int j = i+1; j <  PLAY_COUNT; j++)
				{
					ChangeCard(i,j);
					if(CountSuperWinMoney()==0)
					{
						OutputDebugString("sdp_���� ����� [Ӯ]�ҵ��͵��ƣ�");
						return;
					}
				}
			}	
		}
		else
		{
			OutputDebugString("sdp_���� ����� [Ӯ]�ҵ�Ӯ���ƣ� aaa");
		}
	}
	else
	{//ָ�����Ҫ��Ǯ
		if (CountSuperWinMoney() >= 0)
		{
			for (int i=0;i<PLAY_COUNT;i++)
			{	
				for(int j = i+1; j <  PLAY_COUNT; j++)
				{
					ChangeCard(i,j);
					if (CountSuperWinMoney()< 0)
					{
						OutputDebugString("sdp_���� ����� [��]�ҵ�����ƣ�");
						return;
					}
				}
			}
			//С��0�Ҳ������ҵ���0�����
			for (int i=0;i<PLAY_COUNT;i++)
			{
				for(int j = i+1; j <  PLAY_COUNT; j++)
				{
					ChangeCard(i,j);
					if(CountSuperWinMoney()==0)
					{
						OutputDebugString("sdp_���� ����� [��]�ҵ��͵��ƣ�");
						return;
					}
				}
			}
		}
		else
		{
			OutputDebugString("sdp_���� ����� [��]�ҵ�����ƣ� aaa");
		}
	}

	return;
}

/*
ADD BY DWJ 20121103
���ݵ�ǰ�����������������㵱ǰ��������ָ����ҵ���ӮǮ;
*/
int CServerGameDesk::CountSuperWinMoney()
{
	CString sdp;

	int money = 0;
	BYTE iUserCard[PLAY_COUNT][USER_HAND_CARD];	//��ʱ�������ҵ���;
	BYTE byDownBCard[PLAY_COUNT][USER_HAND_CARD];	///�¶���;
	BYTE iShape[PLAY_COUNT];					//��������;
	memset(byDownBCard,0,sizeof(byDownBCard));		///�����ţ��;
	memset(&iUserCard, 0, sizeof(iUserCard));
	memset(&iShape, 0, sizeof(iShape));

	int iCardPos = 0;
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(!m_bIsInGame[i])
			continue;

		CopyMemory(iUserCard[i],&m_iTotalCard[iCardPos],sizeof(BYTE)*USER_HAND_CARD);
		//m_iUCCount[i] = USER_HAND_CARD;
		iCardPos += USER_HAND_CARD;
		//�������ҵ��������ڱȽϴ�С;
		m_Logic.GetBull(iUserCard[i],SH_USER_CARD, byDownBCard[i]);
		//iShape[i] = m_Logic.GetShape(iUserCard[i],SH_USER_CARD);m_byUCard[PLAY_COUNT][USER_HAND_CARD];	//�û����ϵ��˿�
		iShape[i] = m_Logic.GetShape(iUserCard[i],SH_USER_CARD,byDownBCard[i]);
	}

	BYTE bBeginDesk;
	//��¼�£����ĸ����Ӻſ�ʼ����;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_bIsInGame[i])
		{
			bBeginDesk = i;
			break;
		}
	}
	BYTE bMaxDesk, bMixDesk;
	bMaxDesk = bBeginDesk;
	bMixDesk = bBeginDesk;
	//��ʼ�Ƚ�����ƺ���С�ƣ��������¼����;
	for (int i=bBeginDesk+1; i<PLAY_COUNT; i++)
	{
		if (!m_bIsInGame[i])
		{//�����ڵ���ҾͲ����ƴ�С��;			
			continue;
		}
		if (m_Logic.CompareCard(iUserCard[bMaxDesk],SH_USER_CARD, byDownBCard[bMaxDesk],iUserCard[i],SH_USER_CARD,byDownBCard[i])==-1)
		{
			bMaxDesk = i;
		}
		if (m_Logic.CompareCard(iUserCard[bMixDesk],SH_USER_CARD, byDownBCard[bMixDesk],iUserCard[i],SH_USER_CARD,byDownBCard[i])==1)
		{
			bMixDesk = i;
		}
	}

	sdp.Format("sdp_���� ����� bMaxDesk = %d,m_iSuperSet[0]=%d",bMaxDesk, m_iSuperSet[0]);
	OutputDebugString(sdp);

	if (bMaxDesk == m_iSuperSet[0])
	{//�������ָ��������� ,��ôָ������ӮǮ�� ��ôֻ����������ҵ�Ǯ;

		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL != m_pUserInfo[i] && i != m_iSuperSet[0])
			{//���;
				money += m_iBaseNote;	
			}
		}

		sdp.Format("sdp_���� ����� ��ָ��������� Ӯ�˵�Ǯ = %d",money);
		OutputDebugString(sdp);
	}
	else
	{//������������������ �ͼ���ָ�������Ǯ����;

		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL != m_pUserInfo[i] && i == m_iSuperSet[0])
			{//���;	
				money -= m_iBaseNote;
			}
		}

		sdp.Format("sdp_���� ����� ����ָ��������� ���˵�Ǯ = %d",money);
		OutputDebugString(sdp);
	}
	if (iShape[bMaxDesk] == UG_BULL_BULL)
	{//�����ţţ����2��
		money = 2*money;
	}
	return money;
}

//ͳ������Ϸ���
BYTE CServerGameDesk::CountPlayer()
{
	BYTE count=0;

	for(int i=0; i < PLAY_COUNT; ++i)
	{
		if(m_bIsInGame[i])
			count++;
	}

	return count;
}

///�û������й�
///@param BYTE bDeskStation [in] �����йܵ����
///@param AutoCardStruct *bAuto [in] �й���Ϣ�ṹ
///@return ��������true
BOOL CServerGameDesk::UserAuto(BYTE bDeskStation,AutoCardStruct *bAuto)
{
	m_bIsAuto[bDeskStation] = bAuto->bAutoCard;
	bAuto->bDeskStation=bDeskStation;

	for(int i=0;i<m_bMaxPeople;i++)
	{
		SendGameData(i,bAuto,sizeof(AutoCardStruct),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	}
	SendWatchData(m_bMaxPeople,bAuto,sizeof(AutoCardStruct),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);

	return TRUE;
}

///��������Ϣ
///@param BYTE bDeskStation [in] ���ó��˵����
///@param SuperStruct *pSuper [in] ������Ϣ�ṹ
///@return ��������true
BOOL CServerGameDesk::HandleSuperInfo(BYTE bDeskStation,SuperStruct *pSuper)
{
	
	if (!m_bIsSuper[bDeskStation] || m_bGameStation == GS_SEND_CARD)//�Ѿ����ǳ������ ������Ч || �Ѿ�������
	{
		pSuper->bDeskStation = 255; 
		pSuper->bSuperTag = 0; 
	}
	else
	{
		m_bSuperSet = true;
		m_iSuperSet[0] = pSuper->bDeskStation; 
		m_iSuperSet[1] = pSuper->bSuperTag; 
	}

	SendGameData(bDeskStation,pSuper,sizeof(SuperStruct),MDM_GM_GAME_NOTIFY,ASS_SUPER,0);

	return TRUE;
}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(!m_bIsInGame[bDeskStation])
	{
		return false;
	}

	if(m_bGameStation >= GS_SEND_CARD && m_bGameStation<GS_WAIT_NEXT)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	DebugPrintf("�û�����  λ��=��%d��",bDeskStation);

	 __super:: UserNetCut(bDeskStation, pLostUserInfo);

	m_bIsCut[bDeskStation] = true;

	int iTempCount = 0;
	for(int i = 0;i < PLAY_COUNT; ++i)
	{
		if(m_bIsCut[i])
		{
			iTempCount++;
		}
	}

	//�����˶��� ������Ϸ
	if(iTempCount == CountPlayer())
	{
		GameFinish(bDeskStation,GF_SALE);	//��ȫ������Ϸ
	}

	return true;
}

//�û���������
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{
	__super:: UserReCome(bDeskStation, pNewUserInfo);

	m_bIsCut[bDeskStation] = false;

	return true;
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	DebugPrintf("�û��뿪  λ��=��%d��",bDeskStation);

	m_bIsAuto[bDeskStation] = false;

	m_bIsCut[bDeskStation] = false;

	if(IsPlayGame(bDeskStation))
	{	
		GameFinish(bDeskStation,GF_SALE);	//��ȫ������Ϸ
	}

	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}


//ϵ�y�������Ƿ���ĳ����
BOOL CServerGameDesk::ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard)
{
	//�Q����
	for(int i=iSendCardPos;i<iEndPos;i++)
	{
		if(iTotalCard[i]==bResultCard)
		{
			iTotalCard[i]=bSourceCard;
			return true;
		}
	}
	return false;
}

//���ܿ۷�
int CServerGameDesk::GetRunPublish()
{
	return m_pDataManage->m_InitData.uRunPublish;
}

//�Α���A����
int CServerGameDesk::GetRoomBasePoint()
{
	int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����
	return (iBasePoint>0?iBasePoint:1);
}

//���ӱ���
int CServerGameDesk::GetDeskBasePoint()
{
	return 1;
}

//BYTE CServerGameDesk::GetIndex(int win)
//{
//	BYTE Max = 0, Mix = 0;
//	for (int i=1; i<PLAY_COUNT; i++)
//	{
//		if (!m_bIsInGame[i])
//		{//�����ڵ���ҾͲ����ƴ�С��;
//			continue;
//		}
//		if (m_Logic.CompareCard(&m_iTotalCard[Max*SH_USER_CARD],SH_USER_CARD,
//			&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD)==-1)
//			Max = i;
//		if (m_Logic.CompareCard(&m_iTotalCard[Mix*SH_USER_CARD],SH_USER_CARD,
//			&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD)==1)
//			Mix = i;
//	}
//	if (win == 1)
//		return Max;
//	else 
//		return Mix;
//
//}
//�жϸ���
//BOOL CServerGameDesk::JudgeLoser()
//{
//	try
//	{
//		//CString s = CINIFile::GetAppPath ();/////����·��
//		//CINIFile f( s +"userlist.ini");
//		CString s = CINIFile::GetAppPath ();/////����·��
//		CINIFile f( s +SKIN_FOLDER +"_s.ini");
//		CString key = TEXT("loser");
//		int count = f.GetKeyVal(key,"count",0);
//		if(count <= 0 )					//��ʤ��
//			return FALSE;
//		TCHAR szName[21];
//		BYTE  bFind = 255;				//ʤ������
//		int probability = 0;
//		CString szNickName;
//		for (int j = 0; j < PLAY_COUNT; j++)
//		{
//			if(m_pUserInfo[j] == NULL)
//				continue;
//
//			for(int i = 0; i < count; i ++)
//			{
//				wsprintf(szName,"nickname%d",i);
//				szNickName = f.GetKeyVal(key,szName,"wing");
//
//				if(strcmp(m_pUserInfo[j]->m_UserData.nickName,szNickName) != 0)
//					continue;
//				//����ʤ��(��ֻ����һ��)
//				wsprintf(szName,"prob%d",i);
//				probability = f.GetKeyVal(key,szName,0);
//				bFind = j;
//				break;
//			}
//
//		}
//		//��ǰ������޸���
//		if(bFind == 255)
//			return FALSE;
//
//		BYTE bDest = GetIndex(0);
//		srand(GetTickCount()); 
//		if((rand() % 100) + 1 <= probability)	//һ������
//		{
//			if(bFind != bDest)
//				ChangeCard(bFind,bDest);
//		}
//		//else								//һ����ʤ ��ʤ������ʤ���ܿ���
//		//{
//		//	if(bFind != bDest)
//		//		ChangeCard(bFind,bDest);
//		//}
//		return TRUE;
//	}
//	catch(...)
//	{
//		return TRUE;
//	}
//	return FALSE;
//}

//�ж�ʤ��
//BOOL CServerGameDesk::JudgeWiner()
//{
//	try
//	{
//		//CString s = CINIFile::GetAppPath ();/////����·��
//		//CINIFile f( s +"userlist.ini");
//		CString s = CINIFile::GetAppPath ();/////����·��
//		CINIFile f( s +SKIN_FOLDER +"_s.ini");
//		CString key = TEXT("winer");
//		int count = f.GetKeyVal(key,"count",0);
//
//		if(count <= 0 )					//��ʤ��
//			return FALSE;
//
//		TCHAR szName[21];
//		BYTE  bFind = 255;				//ʤ������
//		int probability = 0;
//		CString szNickName;
//		for (int j = 0; j < PLAY_COUNT; j++)
//		{
//			if(m_pUserInfo[j] == NULL)
//				continue;
//			for(int i = 0; i < count; i ++)
//			{
//				wsprintf(szName,"nickname%d",i);
//				szNickName = f.GetKeyVal(key,szName,"wing");
//
//				if(strcmp(m_pUserInfo[j]->m_UserData.nickName,szNickName) != 0)
//					continue;
//				//����ʤ��(��ֻ����һ��)
//				wsprintf(szName,"prob%d",i);
//				probability = f.GetKeyVal(key,szName,0);
//				bFind = j;
//				break;
//			}
//			if(bFind != 255)
//				break;
//		}
//
//		//��ǰ�������ʤ��
//		if(bFind == 255)
//			return FALSE;
//		BYTE bDest = GetIndex(1);
//		srand(GetTickCount()); 
//		if((rand() % 100) + 1 <= probability)//����ʤ��
//		{
//			if(bFind != bDest)
//				ChangeCard(bFind,bDest);
//		}
//		//else	//һ������
//		//{
//		//	if(bFind == bDest)
//		//		ChangeCard(bFind,(bDest + 1) % PLAY_COUNT);
//		//}
//		return TRUE;
//	}
//	catch(...)
//	{
//		return FALSE;
//	}
//	return FALSE;
//}

//����
BOOL CServerGameDesk::ChangeCard(BYTE bDestStation,BYTE bWinStation)
{
	for(int i = 0; i < SH_USER_CARD; i ++)
	{
		BYTE bTemp = m_iTotalCard[bDestStation * SH_USER_CARD + i];
		m_iTotalCard[bDestStation * SH_USER_CARD + i ] = m_iTotalCard[bWinStation * SH_USER_CARD + i];
		m_iTotalCard[bWinStation * SH_USER_CARD + i] = bTemp;
	}
	return TRUE;
}

//����ǿ�����
void CServerGameDesk::ClearQuitPlayer()
{
	return;
	//�㲥�߳����ж������
	for (BYTE i=0; i<PLAY_COUNT; i++)
	{
		if (m_bIsCut[i])
		{
			MakeUserOffLine(i);
		}
	}
}

/**
* ������Ϸ ��ʱ�� ����Ӧ���� ����OnTimer
*
* @param [in] UINT uTimerID ��ʱ��ID
*
* @param [in] int uElapse ���ʱ��
* 
@param [in] bool bRecTime �Ƿ��¼ʣ��ʱ��
* 
* @return void 
*/
void CServerGameDesk::SetGameTimer(UINT uTimerID, int uElapse,bool bRecTime)
{
	if(bRecTime)
	{
		m_iSurplusTime = 0;

		SetTimer(uTimerID,1000);

		m_iSurplusTime = uElapse;
	}
	else
	{
		SetTimer(uTimerID,uElapse);
	}
}

///�������м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	m_iSurplusTime = 0;

	KillTimer(TIME_WAIT_THINK);		//�ȴ�������Ϸ״̬
	KillTimer(TIME_THINK);			//˼����ʱ��	
}

int CServerGameDesk::GetMaxCardShape(BYTE iCard[], int iCardCount)
{
	if (iCardCount < SH_USER_CARD*PLAY_COUNT)
	{
		return 0;
	}
	int shape = 0;
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if (m_bIsInGame[i])
		{
			//int temp = m_Logic.GetShape(&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD);
			//if (shape < temp)
			//{
			//	shape = temp;
			//}
		}
		
	}
	return shape;
}

//��ȡ��ȡINI�ļ�����
void CServerGameDesk::GetIniData(IniData &sData)
{
	sData.iBaseNote = m_iBaseNote;
	sData.byBTime = m_byBTime;
	sData.bySCTime = m_bySCTime;
	sData.byTTime = m_byTTime;
	sData.byRTime = m_byRTime;
	sData.iCardShape = m_iCardShape;
	sData.iACCount = m_iAllCardCount;
}

//��ȡ��Ϸ��������
void CServerGameDesk::GetBaseData(BData &sData, BYTE bDeskStation)
{
	sData.bGameStation = m_bGameStation;
	sData.iRBPoint = GetRoomBasePoint();
	sData.iRPublish = GetRunPublish();
	sData.bIsSuper = m_bIsSuper[bDeskStation];
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		sData.bIsInGame[i] = m_bIsInGame[i];
		sData.bHaveBaiPai[i] = m_bHaveBaiPai[i];
		sData.bIsAuto[i] = m_bIsAuto[i];
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
	return;
	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() +"\\GameLog\\";

	SHCreateDirectoryEx(NULL, strPath, NULL);

	CTime time = CTime::GetCurrentTime();

	sprintf(szFilename, "%s%d_%d_%d_%d%d%d_log.txt",strPath ,NAME_ID,m_pDataManage->m_InitData.uRoomID,m_bDeskIndex,time.GetYear(), time.GetMonth(), time.GetDay());
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