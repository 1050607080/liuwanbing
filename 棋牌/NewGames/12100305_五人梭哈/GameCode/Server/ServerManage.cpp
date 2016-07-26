#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"
//#include "..\\..\\��Ϸ����������\include\���������\Common\\writelog.h"


//��Ϸ��չ����
BYTE CServerGameDesk:: m_iAllCardCount;

BYTE CServerGameDesk:: m_iSendCardTime;
BYTE CServerGameDesk:: m_iThinkTime;
BYTE CServerGameDesk:: m_iBeginTime;
DWORD CServerGameDesk:: m_dwCardShape;


void DebugPrintf(const char *p, ...)
{
	return;
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

//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bAddTime = 0;					//��ע���
	m_bNoteTimes = 0;				//��ע����					
	::memset(m_UserThisTurnAct,0,sizeof(m_UserThisTurnAct));	//�����ļ���ע���ͳ��
	m_iFirstOutPeople = -1;							
	m_iOutCardPeople = -1;
	m_iLastOutCardPeople = -1;
	m_iUpGradePeople = 0;						
	m_iBeenPlayGame = 0;								

	m_bGameStation = GS_WAIT_SETGAME;
	memset(m_iTotalCard,0,sizeof(m_iTotalCard));
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));		//�û���������
	::memset(m_iUserNote,0,sizeof(m_iUserNote));				//������ע��
	::memset(m_iNowNote,0,sizeof(m_iNowNote));					//���ҵ�ǰ��ע��
	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));	//���ָ�����ע
	memset((void*)&m_iUserCard,0,sizeof(m_iUserCard));
	memset((void*)&m_iTotalCard, 0, sizeof(m_iTotalCard));
	memset((void*)&m_iUserStation,0, sizeof(m_iUserStation));
	memset((void*)&m_UserThisTurnAct,0,sizeof(m_UserThisTurnAct));
	memset(m_bNetCutPlayer, 0, sizeof(m_bNetCutPlayer));
	memset(m_iGameAddBasePoint,0,sizeof(m_iGameAddBasePoint));
	memset(m_bUserState,false,sizeof(m_bUserState));	//���״̬
	memset(m_i64JudgeScore,0,sizeof(m_i64JudgeScore));  /**<�жϵȼ��ķ�����*/ //add xyh 2012-1-10
			
	memset(m_byUserAddNum,0,sizeof(m_byUserAddNum));//���ָ�����Ҽ�ע����
	memset(m_i64AcculateGrade,0,sizeof(m_i64AcculateGrade));
	memset(m_i64UpGrade,0,sizeof(m_i64UpGrade));
	m_byAddNum= 0;///**< ÿ�ּ�ע���� 0ʱ��������*/   //lym 2011-12-13;		
	m_iThisNoteLimit = 0;
	m_iLimitNote = -1;
	m_iBaseNote = 1;
	m_iBaseNoteEx=0;
	m_iTotalNote = 0;
	m_byUserFollow = 0x00;
	//����
	m_dwDeskOwer = 0;
	::memset(m_bChangeRobotCard,false,sizeof(m_bChangeRobotCard));
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_bSuperCardList[i],0,sizeof(m_bSuperCardList[i]));
	}
	m_bSuperStation = -1;
#endif
}
//��ʼ��Ϸ
bool CServerGameDesk::InitDeskGameStation()
{
	LoadIni();		//���������ļ�
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	CINIFile f( temp);
	CString key = TEXT("game");
	m_iBeginTime = f.GetKeyVal(key,"begintime",30);
	if(m_iBeginTime<10)
	{
		m_iBeginTime=10;
	}
	m_iThinkTime = f.GetKeyVal(key,"thinktime",20);
	if(m_iThinkTime<10)
	{
		m_iThinkTime=10;
	}
	m_iSendCardTime = f.GetKeyVal(key,"sendcardtime",300);
	m_iAllCardCount = f.GetKeyVal(key,"cardcount",48);
	m_iLimitNote = f.GetKeyVal(key,"limitnote",__int64(100000));
	if(m_iLimitNote<1000)
	{
		m_iLimitNote=1000;
	}
	m_iBaseNote = f.GetKeyVal(key,"basenote",__int64(10));
	if(m_iBaseNote<1)
	{
		m_iBaseNote=1;
	}
	m_iFirstSendCardCount = f.GetKeyVal(key,"FirstSendCardCount",2);
	if(m_iFirstSendCardCount<1)
	{
		m_iFirstSendCardCount=1;
	}
	m_bShowHa = f.GetKeyVal(key, "ShowHa", 2);//�ڼ��ֿ�����
	if(m_bShowHa<1)
	{
		m_bShowHa=1;
	}
	m_iBaseScale = f.GetKeyVal(key, "BaseScale", 5);
	m_byAddNum= f.GetKeyVal(key, "AddNum", m_byAddNum);///**< ÿ�ּ�ע���� 0ʱ��������*/   //lym 2011-12-13;										
	m_bPK        = f.GetKeyVal(key,"CanPK",0);
	m_iOutCardOrder=f.GetKeyVal(key,"OutCardOrder",0);//����˳��
	if(0!=m_iOutCardOrder && 1!=m_iOutCardOrder)
	{
		m_iOutCardOrder=0;//Ĭ����ʱ��
	}
	int iFollow = f.GetKeyVal(key, "userfollow", 0);
	if (iFollow < 0)
	{
		m_byUserFollow = 0;
	}
	else
	{
		if (iFollow >  0xFF)
		{
			iFollow = 0xFF;
		}
		m_byUserFollow = (BYTE)iFollow;
	}
	CString strDJ;
	for (int i = 0;i<13;i++)
	{
		strDJ.Format("GameAddBasePoint[%d]",i);
		m_iGameAddBasePoint[i] = f.GetKeyVal(key,strDJ,(__int64)pow(double(10.0),i));

		if (m_iGameAddBasePoint[i]<=0)
		{
			m_iGameAddBasePoint[i] = 1;
		}
	}
	m_iAIWantWinMoneyA1	= f.GetKeyVal(key,"AIWantWinMoneyA1 ",__int64(500000));		//������ӮǮ����1 
	m_iAIWantWinMoneyA2	= f.GetKeyVal(key,"AIWantWinMoneyA2 ",__int64(5000000));	//������ӮǮ����2  
	m_iAIWantWinMoneyA3	= f.GetKeyVal(key,"AIWantWinMoneyA3 ",__int64(50000000));	//������ӮǮ����3  
	m_iAIWinLuckyAtA1	= f.GetKeyVal(key,"AIWinLuckyAtA1 ",90);				    //������������1ӮǮ�ĸ���  
	m_iAIWinLuckyAtA2	= f.GetKeyVal(key,"AIWinLuckyAtA2 ",70);				    //��������Ӯ���ƣ�������������2ӮǮ�ĸ���  
	m_iAIWinLuckyAtA3	= f.GetKeyVal(key,"AIWinLuckyAtA3 ",50);				    //��������Ӯ���ƣ�������������3ӮǮ�ĸ���  
	m_iAIWinLuckyAtA4	= f.GetKeyVal(key,"AIWinLuckyAtA4 ",30);				    //��������Ӯ���ƣ�������������4ӮǮ�ĸ���  
	m_iAddMoneyEveryDay	= f.GetKeyVal(key,"AddMoneyEveryDay ",__int64(0));		    //��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(key,"AIWinAndLostAutoCtrl",0);		//��������Ӯ����
	m_iAIHaveWinMoney	= f.GetKeyVal(key,"AIHaveWinMoney ",__int64(0));		//��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  
	//�жϵȼ��ķ�����
	m_i64JudgeScore[0] = 100;
	m_i64JudgeScore[1] = 1000;
	m_i64JudgeScore[2] = 10000;
	m_i64JudgeScore[3] = 100000;
	m_i64JudgeScore[4] = 1000000;
	m_i64JudgeScore[5] = 5000000;
	m_i64JudgeScore[6] = 10000000;
	m_i64JudgeScore[7] = 26000000;
	m_i64JudgeScore[8] = 1000000000;
	m_i64JudgeScore[9] = 10000000000;
	m_i64JudgeScore[10] = 100000000000;
	m_i64JudgeScore[11] = 1000000000000;

	key = TEXT("game");
	CString StrTemp = "";
	for(int i = 0; i < 12; ++i)
	{
		StrTemp.Format("i64JudgeScore[%d]",i);
		m_i64JudgeScore[i] = f.GetKeyVal(key,StrTemp,m_i64JudgeScore[i]);
	}
	return true;
}
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	CINIFile f( temp);
	TCHAR szKey[20];
	wsprintf(szKey,"%s_%d",GET_SKIN_FOLDER(skin),iRoomID);
	CString key = szKey;
	m_iBeginTime = f.GetKeyVal(key,"begintime",m_iBeginTime);
	if(m_iBeginTime<10)
	{
		m_iBeginTime=10;
	}
	m_iThinkTime = f.GetKeyVal(key,"thinktime",m_iThinkTime);
	if(m_iThinkTime<10)
	{
		m_iThinkTime=10;
	}
	m_iLimitNote = f.GetKeyVal(key,"limitnote",m_iLimitNote);
	if(m_iLimitNote<1000)
	{
		m_iLimitNote=1000;
	}
	m_iBaseNote = f.GetKeyVal(key,"basenote",m_iBaseNote);
	if(m_iBaseNote<1)
	{
		m_iBaseNote=1;
	}
	m_iFirstSendCardCount = f.GetKeyVal(key,"FirstSendCardCount",m_iFirstSendCardCount);
	if(m_iFirstSendCardCount<1)
	{
		m_iFirstSendCardCount=1;
	}
	m_bShowHa = f.GetKeyVal(key, "ShowHa", m_bShowHa);//�ڼ��ֿ�����
	if(m_bShowHa<1)
	{
		m_bShowHa=1;
	}
	m_iOutCardOrder=f.GetKeyVal(key,"OutCardOrder",m_iOutCardOrder);//����˳��
	if(0!=m_iOutCardOrder && 1!=m_iOutCardOrder)
	{
		m_iOutCardOrder=0;//Ĭ����ʱ��
	}
	int iFollow = f.GetKeyVal(key, "userfollow", (int)m_byUserFollow);	
	m_iBaseScale = f.GetKeyVal(key, "BaseScale", (int)m_iBaseScale);
	m_bPK        = f.GetKeyVal(key,"CanPK",m_bPK);
	CString strDJ;
	for (int i = 0;i<13;i++)
	{
		strDJ.Format("GameAddBasePoint[%d]",i);
		m_iGameAddBasePoint[i] = f.GetKeyVal(key,strDJ,m_iGameAddBasePoint[i]);
		if (m_iGameAddBasePoint[i]<=0)
		{
			m_iGameAddBasePoint[i] = 1;
		}
	}

	if (iFollow < 0)
	{
		m_byUserFollow = 0;
	}
	else
	{
		if (iFollow >  0xFF)
		{
			iFollow = 0xFF;
		}
		m_byUserFollow = (BYTE)iFollow;
	}
	return TRUE;
}
///��ȡ����Ļ�������
///@return ���ط���Ļ�������
int CServerGameDesk::GetRoomLimitBasePoint()
{
	__int64 iBasePoint = 0;
	for (int i = 0;i<PLAY_COUNT;i++)
	{
		if (NULL == m_pUserInfo[i]  || false == m_bUserState[i])
		{
			continue;
		}
		if (m_pUserInfo[i]->m_UserData.i64Money<iBasePoint||0 == iBasePoint)
		{
			iBasePoint = m_pUserInfo[i]->m_UserData.i64Money;
		}
	}
	int iDengji = 0;
	if (iBasePoint<m_i64JudgeScore[0])
	{
		iDengji = 0;
	}
	else if(m_i64JudgeScore[0]<=iBasePoint&&iBasePoint<m_i64JudgeScore[1]) 
	{
		iDengji = 1;
	}
	else if (m_i64JudgeScore[1]<=iBasePoint&&iBasePoint<m_i64JudgeScore[2]) 
	{		
		iDengji = 2;
	}
	else if(m_i64JudgeScore[2]<=iBasePoint&&iBasePoint<m_i64JudgeScore[3])
	{
		iDengji = 3;
	}
	else if(m_i64JudgeScore[3]<=iBasePoint&&iBasePoint<m_i64JudgeScore[4])
	{
		iDengji = 4;
	}
	else if(m_i64JudgeScore[4]<=iBasePoint&&iBasePoint<m_i64JudgeScore[5])
	{
		iDengji = 5;
	}
	else if (m_i64JudgeScore[5]<=iBasePoint&&iBasePoint<m_i64JudgeScore[6])
	{
		iDengji = 6;
	}
	else if (m_i64JudgeScore[6]<=iBasePoint&&iBasePoint<m_i64JudgeScore[7])
	{
		iDengji = 7;
	}
	else if (m_i64JudgeScore[7]<=iBasePoint&&iBasePoint<m_i64JudgeScore[8])
	{
		iDengji = 8;
	}
	else if (m_i64JudgeScore[8]<=iBasePoint&&iBasePoint<m_i64JudgeScore[9])
	{
		iDengji = 9;
	}
	else if (m_i64JudgeScore[9]<=iBasePoint&&iBasePoint<m_i64JudgeScore[10])
	{
		iDengji = 10;
	}
	else if (m_i64JudgeScore[10]<=iBasePoint&&iBasePoint<m_i64JudgeScore[11])
	{
		iDengji = 11;
	}
	else if (iBasePoint >=m_i64JudgeScore[11])
	{
		iDengji = 12;
	}
	else 
	{
		iDengji = 0;
	}

	//CString CST;
	//CST.Format("wlog::iDengji===%d,iBasePoint===%I64d,m_iGameAddBasePoint[iDengji]===%d",iDengji,iBasePoint,m_iGameAddBasePoint[iDengji]);
	//OutputDebugString(CST);

	return m_iGameAddBasePoint[iDengji];
}

//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	//���ط���˶���ʱ�䶨ʱ��
	//if ((uTimerID >= IDT_USER_CUT)&&(uTimerID < (UINT)(IDT_USER_CUT + m_bMaxPeople)))
	//{
	//	//�ȴ�����ʱ�䵽��
	//	KillTimer(uTimerID);
	//	int bDeskStation = uTimerID - IDT_USER_CUT;
	//	if ((m_pUserInfo[bDeskStation] != NULL) && true == m_bUserState[bDeskStation])
	//	{
	//		UserFourceQuit(bDeskStation);
	//	}
	//	return true;
	//}

	switch(uTimerID)
	{
	case TIME_WAIT:
		{
			KillTimer(TIME_WAIT);

			if (CheckFinish())															//�Ƿ����
				return TRUE;
			//��һѺע��
			for (int i = 0; i < PLAY_COUNT - 1; i ++)
			{
				if(m_iOutCardOrder)//˳ʱ��
				{
					m_iOutCardPeople = (m_iOutCardPeople +1 + PLAY_COUNT) % PLAY_COUNT;
				}
				else//��ʱ��
				{
					m_iOutCardPeople = (m_iOutCardPeople - 1 + PLAY_COUNT) % PLAY_COUNT;
				}
				//��ǰ������Ѿ��뿪
				if(m_iOutCardPeople == m_iNowBigPeople && (m_pUserInfo[m_iOutCardPeople] == NULL || false == m_bUserState[m_iOutCardPeople]
				|| m_iUserStation[m_iOutCardPeople] == TYPE_GIVE_UP))
				{
					if(m_iOutCardOrder)//˳ʱ��
					{
						m_iNowBigPeople = (m_iNowBigPeople + 1 + PLAY_COUNT) % PLAY_COUNT;  
					}
					else//��ʱ��
					{
						m_iNowBigPeople = (m_iNowBigPeople - 1 + PLAY_COUNT) % PLAY_COUNT;
					}
					
				}

				if(!m_pUserInfo[m_iOutCardPeople] || !m_bUserState[m_iOutCardPeople])
					continue;	

				if (m_iUserStation[m_iOutCardPeople] != TYPE_GIVE_UP)
					break;
			}
			//��һѺע��Ϊ�������Ѻע��,һ���Ѿ�������Ҫ�·�һ����
			if (m_iOutCardPeople == m_iNowBigPeople)												//�Ƿ���һ�ֿ�ʼ
			{
				SetTimer(TIME_COLLECT_JETON,300);
			}
			else
			{
				tagUserResult result;
				result.bNoteTimes = m_bNoteTimes;							//��ע����
				result.iOutPeople = m_iOutCardPeople;						//��ע��
				result.iNowBigNote = m_iUserNote[m_iNowBigPeople];			//���������ע��
				result.bNewTurn=false;
				memcpy(result.m_byUserAddNum,m_byUserAddNum,sizeof(m_byUserAddNum));//���ָ�����Ҽ�ע���� lym

				result.iNowBigNoteStyle = m_iUserStation[m_iNowBigPeople];	//��ǰ�����ע����
				result.iTotalNote = m_iTotalNote;							//��ע
				result.bAddTimes = m_UserThisTurnAct[m_iOutCardPeople];//m_bAddTime;								//��ע����
				for (int i = 0; i < PLAY_COUNT; i ++)
				{
					if(m_pUserInfo[i] != NULL && m_bUserState[i])		
						SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
				}	
				SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
				SetTimer(TIME_NOTE,(m_iThinkTime+3)*1000);
			}
			return TRUE;
		}
	case TIME_BEGIN_ANIMAL://��ʼ����
		{
			if (m_bGameStation == GS_SEND_CARD) 
			{	
				KillTimer(TIME_BEGIN_ANIMAL);

				SetTimer(TIME_SEND_CARD,m_iSendCardTime);
			}
			else
			{
				KillTimer(TIME_BEGIN_ANIMAL);
			}

			return TRUE;
		}
	case TIME_SEND_CARD://����
		{
			if (m_bGameStation == GS_SEND_CARD) 
			{
				SendCard();
			}
			else
			{
				KillTimer(TIME_SEND_CARD);
			}

			return TRUE;
		}
	case TIME_COLLECT_JETON://�ռ�����
		{
			if (m_bGameStation == GS_PLAY_GAME)
				CollectJeton();
			else 
				KillTimer(TIME_COLLECT_JETON);
			return TRUE;
		}
	case TIME_SEND_A_CARD://����
		{
			if (m_bGameStation == GS_PLAY_GAME)
				SendACard();
			else KillTimer(TIME_SEND_A_CARD);
			return TRUE;
		}
	case TIME_GAME_FINISH://��Ϸ����
		{
			KillTimer(TIME_GAME_FINISH);
			if (m_bGameStation == GS_PLAY_GAME) 
				GameFinish(0,GF_NORMAL);
			return TRUE;
		}
	case TIME_NOTE:
		{
			KillTimer(TIME_NOTE);
			if (m_bGameStation == GS_PLAY_GAME) 
				UserActiveResult(m_iOutCardPeople,TYPE_GIVE_UP,0);
			break;
		}
	}
	return __super::OnTimer(uTimerID);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	//֪ͨ�����û����뱾��
	NotifyComeMes tComeDesk;
	tComeDesk.byDeskStation=bDeskStation;
	tComeDesk.i64AcculateGrade=m_i64AcculateGrade[bDeskStation];
	tComeDesk.i64UpGrade=m_i64UpGrade[bDeskStation];
	for (int i=0; i < PLAY_COUNT; i++) 
	{
		if (m_pUserInfo[i]!=NULL) 
			SendGameData(i,&tComeDesk,sizeof(tComeDesk),MDM_GM_GAME_NOTIFY,ASS_COME_DESK,0);
	}
	SendWatchData(m_bMaxPeople,&tComeDesk,sizeof(tComeDesk),MDM_GM_GAME_NOTIFY,ASS_COME_DESK,0);
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
		{
			GameStation_1 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;

			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			GameStation.bGameStation = m_bGameStation;
			//��Ϸ������Ϣ����
			GameStation.iLimitNote = m_iLimitNote;							//���ע
			
			GameStation.iBaseNote = m_iBaseNoteEx;							//��ע
			//��������
			GameStation.byUserFollow = m_byUserFollow;					
	
			GameStation.iThinkTime = m_iThinkTime;
			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iAllCardCount = m_iAllCardCount;
			GameStation.bShowHa    = m_bShowHa;
			::CopyMemory(GameStation.bUserState,m_bUserState,sizeof(m_bUserState));
			memcpy(GameStation.i64AcculateGrade,m_i64AcculateGrade,sizeof(GameStation.i64AcculateGrade)) ;
			memcpy(GameStation.i64UpGrade,m_i64UpGrade,sizeof(GameStation.i64UpGrade)) ;
			GameStation.bIsSuperUser = !bWatchUser && IsSuperUser(bDeskStation);//�Ƿ񳬼��ͻ���//lym 2011-11-28
			GameStation.m_byAddNum = m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
			GameStation.bPK        = m_bPK;
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return true;
		}
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			GameStation.bGameStation = m_bGameStation;
			//��Ϸ������Ϣ����
			GameStation.iLimitNote = m_iLimitNote;								//���ע
			GameStation.iBaseNote = m_iBaseNoteEx;							//��ע
			//��������
			GameStation.byUserFollow = m_byUserFollow;
			GameStation.iThinkTime = m_iThinkTime;
			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iAllCardCount = m_iAllCardCount;
			GameStation.bShowHa    = m_bShowHa;
			::CopyMemory(GameStation.bUserState,m_bUserState,sizeof(m_bUserState));
			memcpy(GameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(GameStation.i64AcculateGrade )) ;
			memcpy(GameStation.i64UpGrade,m_i64UpGrade,sizeof(GameStation.i64UpGrade)) ;
			GameStation.bIsSuperUser = !bWatchUser && IsSuperUser(bDeskStation);//�Ƿ񳬼��ͻ���//lym 2011-11-28
			GameStation.m_byAddNum = m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
			GameStation.bPK        = m_bPK;
			
			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				if (NULL == m_pUserInfo[i])
					continue;

				if (m_pUserInfo[i]->m_UserData.bUserState == USER_ARGEE)
				{
					GameStation.bAgree[i] = true;
				}
			}

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
		{
			GameStation_3 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			GameStation.bIsSuperUser = !bWatchUser && IsSuperUser(bDeskStation);//�Ƿ񳬼��ͻ���//lym 2011-11-28
			
			GameStation.bGameStation = m_bGameStation;
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;						//
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			//��Ϸ������������
			GameStation.iLimitNote = m_iLimitNote;							//���ע

			GameStation.iBaseNote = m_iBaseNoteEx;
			//��������
			GameStation.byUserFollow = m_byUserFollow;	

			GameStation.iThinkTime = m_iThinkTime;
			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iAllCardCount = m_iAllCardCount;
			GameStation.iUpGradePeople = m_iUpGradePeople;	
			GameStation.bShowHa    = m_bShowHa;

			GameStation.iBeenPlayGame = m_iBeenPlayGame;
			::CopyMemory(GameStation.bUserState,m_bUserState,sizeof(m_bUserState));
			//�û�������
			::CopyMemory(GameStation.iUserCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));
			::CopyMemory(GameStation.iUserCard,m_iUserCard,sizeof(m_iUserCard));
			::CopyMemory(GameStation.iTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));
			memcpy(GameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(GameStation.i64AcculateGrade )) ;
			memcpy(GameStation.i64UpGrade,m_i64UpGrade,sizeof(GameStation.i64UpGrade)) ;
			GameStation.m_byAddNum = m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
			GameStation.bPK        = m_bPK;
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				GameStation.iUserStation[i] = m_iUserStation[i];
			}
			//��������
			int iSendSize = sizeof(GameStation);
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			GameStation_4 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			GameStation.bIsSuperUser = !bWatchUser && IsSuperUser(bDeskStation);//�Ƿ񳬼��ͻ���//lym 2011-11-28
			
			GameStation.bGameStation = m_bGameStation;
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			//��������
			GameStation.byUserFollow = m_byUserFollow;

			GameStation.iLimitNote = m_iLimitNote;								//���ע
			GameStation.iBaseNote = m_iBaseNoteEx;
			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iThinkTime = m_iThinkTime;

			GameStation.iUpGradePeople = m_iUpGradePeople;		

			GameStation.iBeenPlayGame = m_iBeenPlayGame;		

			GameStation.iOutCardPeople = m_iOutCardPeople;	
			GameStation.iFirstOutPeople = m_iFirstOutPeople;
			GameStation.bShowHa    = m_bShowHa;
			::CopyMemory(GameStation.bUserState,m_bUserState,sizeof(m_bUserState));
			//Ѻע��ز���
			GameStation.tag.bNoteTimes = m_bNoteTimes;		//Ѻע����
			if(m_iNowBigPeople == 255)
				GameStation.tag.iNowBigNote = 0;
			else
				GameStation.tag.iNowBigNote = m_iUserNote[m_iNowBigPeople];//��ǰ���Ѻע��
			GameStation.tag.iNowBigNoteStyle = m_iUserStation[m_iNowBigPeople];//Ѻע����
			GameStation.tag.iOutPeople = m_iOutCardPeople;					//��ǰѺע��
			GameStation.tag.iTotalNote = m_iTotalNote;						//��ǰ��ע
			GameStation.tag.bAddTimes = m_UserThisTurnAct[m_iOutCardPeople];//m_bAddTime;						//��ǰ��ע
			GameStation.m_byAddNum = m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
			GameStation.bPK        = m_bPK;

			//�û�������
			::CopyMemory(GameStation.iUserCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));
			::CopyMemory(GameStation.iUserCard,m_iUserCard,sizeof(m_iUserCard));
			//��ǰ���û���ע����
			::CopyMemory(GameStation.iTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));
			::CopyMemory(GameStation.iThisTurnNote,m_iUserNote,sizeof(m_iUserNote));
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (m_bNetCutPlayer[i])
				{
					GameStation.ibNetCutPlayer[i] = TRUE;
				}
				GameStation.iUserStation[i] = m_iUserStation[i];
			}
			memcpy(GameStation.m_byUserAddNum,m_byUserAddNum,sizeof(m_byUserAddNum));//���ָ�����Ҽ�ע���� 2011-12-13
			memcpy(GameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(GameStation.i64AcculateGrade )) ;
			memcpy(GameStation.i64UpGrade,m_i64UpGrade,sizeof(GameStation.i64UpGrade)) ;
			//��������
			int iSendSize = sizeof(GameStation);
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_5 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			GameStation.bIsSuperUser = !bWatchUser && IsSuperUser(bDeskStation);//�Ƿ񳬼��ͻ���//lym 2011-11-28
			
			GameStation.bGameStation = m_bGameStation;
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2 = DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			//��������
			GameStation.byUserFollow = m_byUserFollow;

			GameStation.iLimitNote = m_iLimitNote;								//���ע
			GameStation.iBaseNote = m_iBaseNoteEx;

			GameStation.iBeginTime = m_iBeginTime;
			GameStation.iThinkTime = m_iThinkTime;
			GameStation.iUpGradePeople = m_iUpGradePeople;		

			GameStation.bShowHa    = m_bShowHa;

			GameStation.iBeenPlayGame = m_iBeenPlayGame;
			::CopyMemory(GameStation.bUserState,m_bUserState,sizeof(m_bUserState));
			GameStation.m_byAddNum = m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
			GameStation.bPK        = m_bPK;
			memcpy(GameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(GameStation.i64AcculateGrade )) ;
			memcpy(GameStation.i64UpGrade,m_i64UpGrade,sizeof(GameStation.i64UpGrade)) ;
			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				if (NULL == m_pUserInfo[i])
					continue;

				if (m_pUserInfo[i]->m_UserData.bUserState == USER_ARGEE)
				{
					GameStation.bAgree[i] = true;
				}
			}

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	}
	return false;
}

//�ػ�ǿ����Ϣ
//��Ϸ���ݰ�������
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:	 //ǿ���˳�
		{
			///�����ϲ�·��
			CString szAppPath = CINIFile::GetAppPath();

			CString strBCFFile  ; 
			CString strName;
			strName.Format("%d",NAME_ID);
			strBCFFile.Format("%s\\SpecialRule.bcf",szAppPath) ; 

			CBcfFile File(strBCFFile) ;

			bool  bForceQuitAsAuto = 0 ; 

			if (File.IsFileExist(strBCFFile))
			{
				bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strName,0) ; 
			}

			if(bForceQuitAsAuto == true)
			{
				return true ; 
			}
			else
			{
				UserFourceQuit(bDeskStation);
			}
		}
	}


	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//���ǿ��
BOOL CServerGameDesk::UserFourceQuit(BYTE bDeskStation)
{
	/*if(m_pUserInfo[bDeskStation] != NULL)
		UpdateUserDB(m_pUserInfo[bDeskStation]->m_UserData.dwUserID, \
		(m_pDataManage->m_InitData.uComType==TY_MONEY_GAME) ? (- m_iPerJuTotalNote[bDeskStation]) : 0, \
		- m_iPerJuTotalNote[bDeskStation]);*/

	//��ǰǿ����Ϊ��ע��
	if(bDeskStation == m_iOutCardPeople)
		UserActiveResult(bDeskStation,0,0);

	//CleanCutGameInfo(bDeskStation);

	//��Ϸ����
	if(IsPlayingByGameStation())
	{	
		if(!UserAI(bDeskStation,2))
			return TRUE;
	}
	else
	{
		return TRUE;
	}

	return TRUE;
}

//���AI����
BOOL CServerGameDesk::UserAI(BYTE bDeskStation,BYTE bCutType)
{
	m_bNetCutPlayer[bDeskStation] = true;
	m_iUserStation[bDeskStation] = TYPE_GIVE_UP;
	UseAIStation ai;
	ai.bDeskStation = bDeskStation;
	ai.bState = 0;
	for(int i = 0; i < PLAY_COUNT ;i++)
	{
		if(i != bDeskStation && m_pUserInfo[i] != NULL)		
			SendGameData(i,&ai,sizeof(ai),MDM_GM_GAME_NOTIFY,ASS_AI_STATION,0);	
	}

	SendWatchData(m_bMaxPeople,&ai,sizeof(ai),MDM_GM_GAME_NOTIFY,ASS_AI_STATION,0);

	if(CountActivePlayer() <= 1)	//��1��
	{
		BYTE b = 255;

		for(int i = 0; i < PLAY_COUNT; i ++)
		{
			if(m_pUserInfo[i] == NULL)
				continue;
			if(i == bDeskStation)
				continue;
			if(m_iUserStation[i] == TYPE_GIVE_UP)
				continue;
			b = i;
			break;
		}

		if(b == 255)//������һ��������
			GameFinish(b,GF_NO_WINER);
		else
			GameFinish(b,GF_NORMAL);
		return FALSE;
	}
	return TRUE;
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case 79://�����ͻ���
		{
#ifdef SUPER_VERSION
			if(bWatchUser)
				return FALSE;
			if(m_bGameStation == GS_PLAY_GAME)
				return true;
			SetCardStruct * pCard = (SetCardStruct*)pData;
			//Ĭ�Ϸ�ʽ(��ǰ�������)
			m_bSuperStation = bDeskStation ;
			if(pCard->paicount[0] > 0)//�������Լ�
			{
				m_iSuperCardCount[m_bSuperStation] = (pCard->paicount[0] > 5) ? 5 : pCard->paicount[0];
				::CopyMemory(m_bSuperCardList[m_bSuperStation],pCard->pai[0],m_iSuperCardCount[m_bSuperStation]);

			}else//���ö��0λ�ø�Ĭ��
			{
				for(int i = 1; i < 8; i ++)
				{
					m_iSuperCardCount[i-1] = (pCard->paicount[i] > 5) ? 5 : pCard->paicount[i];
					::CopyMemory(m_bSuperCardList[i-1],pCard->pai[i],sizeof(BYTE)*m_iSuperCardCount[i-1]);
				}
			}
#endif
			return TRUE;
		}
	case ASS_UG_USER_SET:
		{						
			return true;
		}
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if(bWatchUser)
				return FALSE;

			if (m_bGameStation > GS_WAIT_ARGEE && m_bGameStation < GS_WAIT_NEXT)
			{
				return TRUE;
			}

			if(m_bGameStation != GS_WAIT_NEXT)
				m_bGameStation = ASS_GM_AGREE_GAME;

			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
	case ASS_HAVE_THING:	//���������뿪
		{
			if(bWatchUser)
				return false;
			HaveThingStruct * pThing=(HaveThingStruct *)pData;
			return UserHaveThing(bDeskStation,pThing->szMessage);
		}
	case ASS_LEFT_RESULT: //ͬ���û��뿪
		{
			if(bWatchUser)
				return false;
			LeaveResultStruct * pLeave=(LeaveResultStruct *)pData;
			return ArgeeUserLeft(bDeskStation,pLeave->bArgeeLeave);
		}
	case ASS_AGREE_STOP:
		{
			return true;
		}
		//colin090511����FLASH����
	case ASS_BEGIN_FLASH:
		{
			if (m_bGameStation != GS_PLAY_GAME)
			{
               return false;
			}
			CardInfo pcardinfo = {0};
			pcardinfo.bDeskStation = bDeskStation;

			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
					SendGameData(i,&pcardinfo,sizeof(pcardinfo),MDM_GM_GAME_NOTIFY,ASS_PLAY_FLASH,0);
			}
			SendWatchData(m_bMaxPeople,&pcardinfo,sizeof(pcardinfo),MDM_GM_GAME_NOTIFY,ASS_PLAY_FLASH,0);	
			//pcardinfo->iUserCard =m_iUserCard
			return true;
		}
	case ASS_VREB_CHECK:		//�û�����
		{
			if(bWatchUser)
				return false;
			if(m_bGameStation == GS_WAIT_NEXT || m_bGameStation == GS_WAIT_ARGEE || m_bGameStation == GS_WAIT_SETGAME)
				return true;
			tagUserProcess *pVerb = (tagUserProcess *) pData;
			return UserActiveResult(bDeskStation,pVerb->iVrebType,pVerb->iNote);
		}
	case ASS_REPLACE_VREB_CHECK://�����û�����
		{
			if(bWatchUser)
				return false;
			if(m_bGameStation == GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
				return true;
			ReplaceUserNoteStruct *pUserNote = (ReplaceUserNoteStruct *) pData;

			return UserActiveResult(pUserNote->bDeskStation ,pUserNote->iVrebType,pUserNote->iNote);
		}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//ɱ���м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_BEGIN_ANIMAL);
	KillTimer(TIME_SEND_CARD);
	KillTimer(TIME_COLLECT_JETON);
	KillTimer(TIME_SEND_A_CARD);
	KillTimer(TIME_GAME_FINISH);
	return ;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillAllTimer();
	if ((bLastStation == GFF_FORCE_FINISH)||(bLastStation == GF_SALE))
	{
		m_iBeenPlayGame = 0;
		m_iUpGradePeople = -1;
	}

	m_iFirstOutPeople = -1;
	m_iOutCardPeople = -1;
	m_iLastOutCardPeople = -1;

	m_iSendCardPos = 0;
	m_bAddTime = 0;					//��ע���
	::memset(m_UserThisTurnAct,0,sizeof(m_UserThisTurnAct));	//�����ļ���ע���ͳ��

	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));

	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));	//���ָ�����ע
	memset(m_bNetCutPlayer, 0, sizeof(m_bNetCutPlayer));
	memset(m_bUserState,false,sizeof(m_bUserState));			//���״̬
	::memset(m_bChangeRobotCard,false,sizeof(m_bChangeRobotCard));
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_bSuperCardList[i],0,sizeof(m_bSuperCardList[i]));
	}
	m_bSuperStation = -1;
#endif
	return TRUE;
}
//�������޺͵�ע
BOOL CServerGameDesk::SetLimitNote()
{
	//InitDeskGameStation();	// ���������ļ�

	//if(m_iLimitNote < 1)
	//{
	//	m_iLimitNote = GetHumanBasePoint();

	//	if(m_iLimitNote <= 0 || m_pDataManage->m_InitData.uComType != TY_MONEY_GAME)
	//	{
	//		m_iLimitNote  = 1000;	
	//	}
	//}

	//if (m_iLimitNote > GetHumanBasePoint())
	{
		m_iLimitNote = GetHumanBasePoint();

		if(m_iLimitNote <= 0 || m_pDataManage->m_InitData.uComType != TY_MONEY_GAME)
		{
			m_iLimitNote  = 1000;
		}
	}

	if (m_iLimitNote >= 999999999999999999)		// �����зⶥ
	{
		m_iLimitNote =999999999999999999;
	}
	m_iBaseNoteEx=m_iBaseNote;
	if(m_iBaseNoteEx>GetRoomLimitBasePoint())
	{
		m_iBaseNoteEx= GetRoomLimitBasePoint();
	}

	if (m_iBaseNoteEx <=0)
	{
		m_iBaseNoteEx = 10;
	}

	/*DOUBLE dScale = (DOUBLE)m_iBaseScale/100;

	if (m_iBaseNote > (DOUBLE)m_iLimitNote*dScale)
	{
		m_iBaseNote = (DOUBLE)m_iLimitNote*dScale;
	}*/
	
	return TRUE;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		GameFinish(0, GF_SALE);
		return false;
	}
	//�����ۻ��ɼ�
	for(int i=0; i < PLAY_COUNT; i++) 
	{
		if (!m_pUserInfo[i]) 
		{
			m_i64AcculateGrade[i]=0;
			m_i64UpGrade[i]=0;
		}
	}
	for(int i = 0;i < PLAY_COUNT;i++)
	{
		if(NULL != m_pUserInfo[i])
		{
			m_bUserState[i] = true;
		}
	}

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		m_iUserStation[i] = -1;
	}

	SetLimitNote();

	m_iSendCardPos = 0;							//����λ��
	m_iBeenPlayGame ++;							//�Ѿ���ľ���
	m_iTotalNote = 0;							//������ע��

	m_iLastShuttle = -1;
	m_bNoteTimes = 0;							//��ע����
	m_bGameStation = GS_SEND_CARD;				//��Ϸ״̬

	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));		// �û���������
	::memset(m_iUserNote,0,sizeof(m_iUserNote));				// ������ע��
	::memset(m_iNowNote,0,sizeof(m_iNowNote));					// ���ҵ�ǰ��ע��
	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));	// ���ָ�����ע
	::memset(m_bNetCutPlayer, 0, sizeof(m_bNetCutPlayer));

	m_Logic.RandCard(m_iTotalCard,28);				//�ַ��˿�
	
	m_Logic.SetCardShapeRestrict(m_dwCardShape);	//��������

#ifdef SUPER_VERSION
	for(int j = 0; j < PLAY_COUNT; j ++)
	{
		for(int i = 0; i < m_iSuperCardCount[j]; i ++)
		{
			int temp=m_Logic.GetSerialBySpecifyCard(m_iTotalCard,0,m_iAllCardCount,m_bSuperCardList[j][i]);

			if(temp != -1)
			{	
				BYTE t = m_iTotalCard[i + j * 5];
				m_iTotalCard[i + j * 5] = m_iTotalCard[temp];
				m_iTotalCard[temp] = t;
			}
		}
	}
#endif

	//-ADD--BY---DWJ-20121122
	//����ÿ�̸��ݷ���ID��ȡ�������ļ��е�ReSetAIHaveWinMoney ���� �趨�»�����Ӯ�˶���Ǯ
	GetAIContrlSetFromIni(m_pDataManage->m_InitData.uRoomID);


	//20121122 dwj �����˿�����Ӯ���� û�г��˿���;
	if(m_bAIWinAndLostAutoCtrl/*&& !m_bSuperSet*/)
	{
		IAWinAutoCtrl();
	}

#ifdef SET_JUDGE
	Judge();
#endif
	
	//���Ϳ�ʼ��Ϣ
	GameBeginStruct BeginMessage;
	memset(&BeginMessage,0,sizeof(BeginMessage));
	BeginMessage.iBeenPlayGame = m_iBeenPlayGame;				// �Ѿ�����ľ���
	BeginMessage.iLimitNote = m_iLimitNote;						// ��ע
	BeginMessage.iBaseNote = m_iBaseNoteEx;						// ��ע
	BeginMessage.bIsSuperUser = false;	
	BeginMessage.i64HalfLessMoney=GetHalfforLessUserMoney();
	::CopyMemory(BeginMessage.bUserState,m_bUserState,sizeof(m_bUserState));

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if(m_pUserInfo[i] == NULL)
			continue;
		BeginMessage.bChangeRobotCard = m_bChangeRobotCard[i];		
		BeginMessage.bIsSuperUser = IsSuperUser(i);//�Ƿ񳬼��ͻ���//lym 2011-11-28
		SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);
	}
	BeginMessage.bIsSuperUser = false;
	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);


	for(int i = 0; i < PLAY_COUNT; i ++)									// ����ĿǰѺע���
	{
		if(NULL != m_pUserInfo[i] && m_bUserState[i])
		{
			m_iPerJuTotalNote[i] = m_iBaseNoteEx;
		}
		else
		{
			m_iPerJuTotalNote[i] = 0 ;
		}
	}

	// ����ͳ��ϵͳ�Զ���ע�Ľ�ң���������ԭ���ǵ�ֵ��Сʱ��100,��ʱ�ͻ����Է���עΪ200���������עΪ300���������죩
	m_iTotalNote = m_iBaseNoteEx;
	m_iTotalNote *= CountPlayer();

	m_iSendPos = 0;

	SendLimit();	// ������ע

	//������Ϣ
	m_iSendCardPos = 0;
	SetTimer(TIME_SEND_CARD,CountPlayer()*1000);

	//DebugPrintf("GameBegin() m_iLimitNote %I64d, m_iBaseNote %I64d, m_byUserFollow %I64d",m_iLimitNote,m_iBaseNote,m_byUserFollow);
	return TRUE;
}


//�����˿˸��û�
BOOL CServerGameDesk::SendCard()
{
	///add by wys һ���Խ��÷����ƶ������ͻ���
	KillTimer(TIME_SEND_CARD);

	if(m_iSendCardPos>0)
	{
		KillTimer(TIME_SEND_CARD);

		SendCardFinish();

		return TRUE;
	}

	SendOneTurnCardStruct pSendCardBag;

	pSendCardBag.iFirstSendCardCount = m_iFirstSendCardCount;
	BYTE iCardCount = m_iFirstSendCardCount;		///ÿ�˷�������


	for(int iSendCardPos = 0 ; iSendCardPos<PLAY_COUNT *5 ; iSendCardPos++)
	{
		m_iUserCard[iSendCardPos%PLAY_COUNT][iSendCardPos/SH_USER_CARD] =  m_iTotalCard[iSendCardPos];
		m_iTotalCard[iSendCardPos] = 255;
	}

	CString dwjlog;
	for(int i=0;i<PLAY_COUNT; i++)
	{
		for(int j=0; j<SH_USER_CARD; j++)
		{
			dwjlog.Format("dwjsh::m_iUserCard[%d][%d]=%d",i,j,m_iUserCard[i][j]);
			OutputDebugString(dwjlog);
		}
	}

	BYTE iStartPos = 255 ;

	int iUserCount = 0 ;

	for(int i = 0; i<PLAY_COUNT ; i++)
	{
		if(NULL == m_pUserInfo[i] || false == m_bUserState[i])
		{
			m_iUserCardCount[i] = 0 ;
		}
		else
		{	
			iUserCount++;

			if(255 == iStartPos)
			{
				iStartPos = i ;
			}
			m_iUserCardCount[i] = iCardCount;

			pSendCardBag.iCardCount[i] = m_iUserCardCount[i] ;

			memcpy(&pSendCardBag.iCardList[i],&m_iUserCard[i],sizeof(BYTE)*(m_iUserCardCount[i]));
		}
	}

	pSendCardBag.iStartPos = iStartPos;

	for(int i = 0 ;i<PLAY_COUNT ; i++)
	{
		if(NULL != m_pUserInfo[i])
		{
			if(IsSuperUser(i))//i�ǳ����ͻ��ˣ���ȫ������
			{
				memcpy(pSendCardBag.iCardList,m_iUserCard,sizeof(m_iUserCard));
			}
			SendGameData(i,&pSendCardBag,sizeof(pSendCardBag),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);
		}
		
	}
	for(int i = 0 ;i<PLAY_COUNT ; i++)//�Թ���Ҳ������Ʒ���
	{
		memcpy(&pSendCardBag.iCardList[i],&m_iUserCard[i],sizeof(BYTE)*(m_iUserCardCount[i]));
	}
	SendWatchData(m_bMaxPeople,&pSendCardBag,sizeof(pSendCardBag),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);	

	m_iSendCardPos++;

	SetTimer(TIME_SEND_CARD,iUserCount*400+2000) ; 	

	return TRUE;
}

//���ƽ���
BOOL CServerGameDesk::SendCardFinish()
{
	SendCardFinishStruct SendCardFinishMsg;	
	::memset(&SendCardFinishMsg,0,sizeof(SendCardFinishMsg));

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if(NULL != m_pUserInfo[i])
		{
			SendGameData(i,&SendCardFinishMsg,sizeof(SendCardFinishMsg),MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);
		}
	}
	SendWatchData(m_bMaxPeople,&SendCardFinishMsg,sizeof(SendCardFinishMsg),MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);

	BeginPlay();

	return true;
}

//�܉�l��
BOOL CServerGameDesk::CanSendCard(BYTE bDeskStation)
{
	int min = 99;
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_iUserCardCount[i] < min && m_pUserInfo[i] && true == m_bUserState[i])
			min = m_iUserCardCount[i];
	}
	if(m_iUserCardCount[bDeskStation] == min)
		return true;
	return false;
}

//ͳ������Ϸ���
BYTE CServerGameDesk::CountPlayer()
{
	BYTE count = 0;
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(NULL != m_pUserInfo[i] && m_bUserState[i])
		{	
			count ++;
		}
	}
	return count;
}

//ͳ������Ϸ���
BYTE CServerGameDesk::CountActivePlayer()
{
	BYTE count = 0;
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_pUserInfo[i] == NULL || false == m_bUserState[i])
			continue;

		if(m_iUserStation[i] != TYPE_GIVE_UP)
		{	
			count ++;
		}
	}
	return count;
}

/*************************************************
*Function: ���������
*explain:�ú�������һ���̶���ֵ����ȡ���ֵ����Ч��������������ʱ���ֹ���������
*writer:
*		˧����
*Parameters:
*		void
*Return:
*		���������
*************************************************/
int CServerGameDesk::MyRand()
{
	static int innerSeed = GetTickCount();

	return (((innerSeed = innerSeed * 214013L + 2531011L) >> 16) & 0x7fff );
}

/*************************************************
*Function: ��ȡȨ�����ֵ
*explain:�ú���ͨ��һ��Ȩ��ֵ������ĳһ��Χ�����ֵ�����ɸ��ʣ�
*����ֵ���ܳ����꡾MAX_PATH����Ȩ��ֵ������ڵ���0
*writer:
*		˧����
*Parameters:
*		void
*Return:
*		���������
*************************************************/
int CServerGameDesk::GetWeight(unsigned int iPoint[], int iCount)
{
	unsigned int idata[MAX_PATH + 1] = {0};//���öϵ� ��Ŀ��ָ��������Чֵ��һ��
	unsigned int imidd = 0;
	if (iCount > MAX_PATH)
	{
		return 0;
	}

	for (int i = 0; i < iCount; ++i)//����������Чֵ�����ֶϵ�
	{
		idata[i] = imidd;
		imidd += iPoint[i];
	}

	idata[iCount] = imidd;//�����һ���ϵ�ӽ���

	unsigned int tag = MyRand() % imidd;//��ָ����Χ���������ֵ

	for (int i = 0; i < iCount; ++i)
	{
		if (tag >= idata[i] && tag <= idata[i+1])//�ڶϵ�֮�䣬���ظöϵ�ֵ
		{
			return i;//��Ȩ���ҵ������ֵ
		}
	}

	return 0;//�쳣��������ʼֵ
}

//��Ϸ��ʼ
BOOL CServerGameDesk::BeginPlay()
{
	// ��������
	m_bGameStation = GS_PLAY_GAME;

	unsigned int iPoint[PLAY_COUNT] = {0};
	int iCount = PLAY_COUNT;

	for(int i = 0; i<PLAY_COUNT ; i++)
	{
		if(NULL == m_pUserInfo[i] || false == m_bUserState[i])
			iPoint[i] = 0;
		else
			iPoint[i] = 100;
	}

	m_iUpGradePeople = getFirstGreate();//GetWeight(iPoint, iCount);	 // ��Ϊ���޷������ƣ����������ĳһ���������ע
	m_iOutCardPeople = m_iUpGradePeople;
	m_iFirstOutPeople = m_iUpGradePeople;

	memset(m_byUserAddNum,0,sizeof(m_byUserAddNum));//���ָ�����Ҽ�ע����
	m_bNoteTimes ++;						    // ��ע����
	m_iNowBigPeople = 255;						// ��ǰ�����

	//������Ϸ��ʼ��Ϣ
	BeginPlayStruct Begin;
	Begin.bDeskStaton  = m_iOutCardPeople;
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
 		if(NULL != m_pUserInfo[i])
		{
			SendGameData(i,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
		}
	}
	SendWatchData(m_bMaxPeople,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

	SetTimer(TIME_NOTE,(m_iThinkTime+3)*1000);

	return TRUE;
}

//��һ�������
BOOL CServerGameDesk::FistShutle()
{
	int shutle = 0;
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(m_iUserStation[i] == TYPE_SHUTTLE)
		{
			shutle ++;
		}	
	}
	return (shutle >= 2);
}

//�����û����
bool CServerGameDesk::UserActiveResult(BYTE bDeskStation, BYTE iVerbType,__int64 iNoteVal)
{
	if (bDeskStation != m_iOutCardPeople) //��ǰ��ע��
	{
		
		if (m_pUserInfo[bDeskStation])
		{
			DebugPrintf("%s���ǵ�ǰ��ע��,m_iOutCardPeople=%d bDeskStation=%d iVerbType=%d",m_pUserInfo[bDeskStation]->m_UserData.nickName,m_iOutCardPeople,bDeskStation,iVerbType);
		}
			return true;
	}

	m_iOutCardPeople = bDeskStation;
	if(m_iOutCardPeople == m_iLastOutCardPeople)
	{
		if (m_pUserInfo[bDeskStation])
		{
			DebugPrintf("%s���ǵ�ǰ��ע��,m_iOutCardPeople=%d m_iLastOutCardPeople=%d iVerbType=%d",m_pUserInfo[bDeskStation]->m_UserData.nickName,m_iOutCardPeople,m_iLastOutCardPeople,iVerbType);
		}
		return true;
	}
	m_iLastOutCardPeople = m_iOutCardPeople;
	m_iUserStation[bDeskStation] = iVerbType;									//Ѻע����

	KillTimer(TIME_NOTE);
	switch (iVerbType)
	{
	case TYPE_SHUTTLE://Ѻע��=���-�Ѿ�Ѻע��
		{	
			//���ݵ�ǰ��ע�˶���ע
			if(!FistShutle())			
			{
				SetLimitNote();
			}

			m_iUserNote[bDeskStation] = m_iLimitNote - m_iPerJuTotalNote[bDeskStation];										//��ǰѺע��
			m_iNowNote[bDeskStation] = m_iLimitNote-m_iPerJuTotalNote[bDeskStation];//m_iUserNote[bDeskStation]-m_iNowNote[bDeskStation];	//��ǰѺע��
			m_iPerJuTotalNote[bDeskStation] = m_iLimitNote;					//�û�һ��Ѻע��

			Msg("AAAA ȫѹ%I64d", m_iLimitNote);
			if(	m_iNowBigPeople == 255)										//��һ���󱣴���Ϊ���ֵ
				m_iNowBigPeople = bDeskStation;

			if(m_iPerJuTotalNote[bDeskStation] > m_iPerJuTotalNote[m_iNowBigPeople])
				m_iNowBigPeople = bDeskStation;
			m_iUserStation[bDeskStation] = TYPE_SHUTTLE;	
			m_UserThisTurnAct[bDeskStation] = true;
			break;
		}
	case TYPE_FOLLOW:// Ѻע��=��ǰ���ע��-�Ѿ���ע��
		{	
			
			if(	m_iNowBigPeople == 255)										//��һ���󱣴���Ϊ���ֵ
				m_iNowBigPeople = bDeskStation;
			if(m_iPerJuTotalNote[m_iNowBigPeople] ==0)
			{
				m_iUserStation[bDeskStation] = iVerbType;						//Ѻע����
				if(	m_iNowBigPeople == 255)										// ����
					m_iNowBigPeople = bDeskStation;									
				m_iNowNote[bDeskStation] = 0;											// ��ǰѺע��Ϊ0			
				m_UserThisTurnAct[bDeskStation] = true;
			}
			m_iUserNote[bDeskStation] = m_iUserNote[m_iNowBigPeople];
			m_iNowNote[bDeskStation] = m_iPerJuTotalNote[m_iNowBigPeople]-m_iPerJuTotalNote[bDeskStation];	//��ǰѺע��
			m_iPerJuTotalNote[bDeskStation] = m_iPerJuTotalNote[m_iNowBigPeople];					//�û�һ��Ѻע��
			m_iUserStation[bDeskStation] = TYPE_FOLLOW;



			// �ͻ����쳣ʱ�Ż����
			if (m_iPerJuTotalNote[bDeskStation] >= m_iLimitNote)
			{
				__int64 lose = m_iPerJuTotalNote[bDeskStation] -  m_iLimitNote;
				m_iPerJuTotalNote[bDeskStation] = m_iLimitNote;
				m_iNowNote[bDeskStation] -= lose;
				m_iUserNote[bDeskStation] -= lose;
				iNoteVal -= lose;

				//�ı䵱ǰ��ע״̬
				iVerbType = TYPE_SHUTTLE;
				m_iUserStation[bDeskStation] = TYPE_SHUTTLE;	
			}
			m_UserThisTurnAct[bDeskStation] = true;
			break;
		}
	case TYPE_ADD://��ע��=��ǰ���ע��-�û��Ѿ���ע��+��ע��
		{
			// ��ע�����쳣 ����Ϊ��ע��ʽ�Ѿ���Ϊ��iNoteVal���Լ���������ע����
			if ((iNoteVal - m_iUserNote[m_iNowBigPeople]) <= 0) 
			{
				iNoteVal = m_iUserNote[m_iNowBigPeople]+iNoteVal;	// �������Ѻע��=���Ѻע��+��ע��
			}
			m_byUserAddNum[bDeskStation]++;//���ָ�����Ҽ�ע����

			//��ǰѺע��=���Ѻע��-ԭѺע��
			m_iNowNote[bDeskStation] = iNoteVal - m_iUserNote[bDeskStation];	//��ǰѺע��
			m_iUserNote[bDeskStation] = iNoteVal;	        // �������Ѻע��=���Ѻע��+��ע��

			m_iPerJuTotalNote[bDeskStation] += m_iNowNote[bDeskStation];
			m_iNowBigPeople = bDeskStation;									
			m_iUserStation[bDeskStation] = TYPE_ADD;

			//������ע
			if (m_iPerJuTotalNote[bDeskStation]  >= m_iLimitNote)
			{
				__int64 lose = m_iPerJuTotalNote[bDeskStation] -  m_iLimitNote; 				
				m_iPerJuTotalNote[bDeskStation] = m_iLimitNote;
				m_iNowNote[bDeskStation] -= lose;
				m_iUserNote[bDeskStation] -= lose;
				iNoteVal -= lose;
				//�ı䵱ǰ��ע״̬
				iVerbType = TYPE_SHUTTLE;
				m_iUserStation[m_iNowBigPeople] = iVerbType;
			}
			m_bAddTime ++;					//��ע���
			m_UserThisTurnAct[bDeskStation] = true;
			break;
		}
	case TYPE_NOTE:
		{
			m_iUserNote[bDeskStation] = iNoteVal;	//�������Ѻע��=���Ѻע��+��ע��
			m_iNowNote[bDeskStation] = iNoteVal;
			m_iPerJuTotalNote[bDeskStation] += m_iNowNote[bDeskStation];		//�û�һ��Ѻע��
			m_iNowBigPeople = bDeskStation;	
			m_iUserStation[bDeskStation] = TYPE_NOTE;
			m_byUserAddNum[bDeskStation]++;//���ָ�����Ҽ�ע����

			//������ע
			if (m_iPerJuTotalNote[bDeskStation]  >= m_iLimitNote)
			{
				__int64 lose = m_iPerJuTotalNote[bDeskStation] -  m_iLimitNote; 				
				m_iPerJuTotalNote[bDeskStation] = m_iLimitNote;
				m_iUserNote[bDeskStation] -= lose;
				m_iNowNote[bDeskStation] -= lose;
				m_iUserNote[bDeskStation] -= lose;
				iNoteVal -= lose;
				//�ı䵱ǰ��ע״̬
				iVerbType = TYPE_SHUTTLE;
				m_iUserStation[m_iNowBigPeople] = iVerbType;
			}
			m_UserThisTurnAct[bDeskStation] = true;
			break;
		}

	case TYPE_GIVE_UP:														//����
		{
			//if (m_bPK && m_iUserStation[m_iNowBigPeople]==TYPE_SHUTTLE)
			//{

			//	if(m_iPerJuTotalNote[m_iNowBigPeople] ==0)
			//	{
			//		m_iUserStation[bDeskStation] = iVerbType;						//Ѻע����
			//		if(	m_iNowBigPeople == 255)										// ����
			//			m_iNowBigPeople = bDeskStation;									
			//		m_iNowNote[bDeskStation] = 0;											// ��ǰѺע��Ϊ0			
			//		m_UserThisTurnAct[bDeskStation] = true;
			//	}
			//	m_iUserNote[bDeskStation] = m_iUserNote[m_iNowBigPeople];
			//	m_iNowNote[bDeskStation] = m_iPerJuTotalNote[m_iNowBigPeople]-m_iPerJuTotalNote[bDeskStation];	//��ǰѺע��
			//	m_iPerJuTotalNote[bDeskStation] = m_iPerJuTotalNote[m_iNowBigPeople];					//�û�һ��Ѻע��
			//	m_iUserStation[bDeskStation] = TYPE_FOLLOW;

			//	// �ͻ����쳣ʱ�Ż����
			//	if (m_iPerJuTotalNote[bDeskStation] >= m_iLimitNote)
			//	{
			//		__int64 lose = m_iPerJuTotalNote[bDeskStation] -  m_iLimitNote;
			//		m_iPerJuTotalNote[bDeskStation] = m_iLimitNote;
			//		m_iNowNote[bDeskStation] -= lose;
			//		m_iUserNote[bDeskStation] -= lose;
			//		iNoteVal -= lose;

			//		//�ı䵱ǰ��ע״̬
			//		iVerbType = TYPE_SHUTTLE;
			//		m_iUserStation[bDeskStation] = TYPE_SHUTTLE;	
			//	}
			//	m_UserThisTurnAct[bDeskStation] = true;

			//}
			//else
			{
				m_iUserStation[bDeskStation] = iVerbType;						//Ѻע����
				if(	m_iNowBigPeople == 255)										//��һ���󱣴���Ϊ���ֵ
					m_iNowBigPeople = bDeskStation;									
				m_iNowNote[bDeskStation] = 0;											// ��ǰѺע��Ϊ0
				memset(m_iUserCard[bDeskStation],0,sizeof(m_iUserCard[bDeskStation]));  // �������
			}
			break;
		}
	}

	//���ۼ���ע����
	m_iTotalNote += m_iNowNote[bDeskStation];								// ������ע��
	//����Ѻע���
	NoteResult noteresult;					
	noteresult.bNoteStyle = iVerbType;										// ��ע����
	noteresult.iOutPeople = m_iOutCardPeople;								// ������ע��
	noteresult.iCurNote=  m_iNowNote[bDeskStation];							// �����û���ע��
	::CopyMemory(noteresult.iPlayerTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));  //�û�ÿ��ѹ��ע xyh 20110818


	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i])
			SendGameData(i,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);
	}
	SendWatchData(m_bMaxPeople,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);	

	if (iVerbType != TYPE_GIVE_UP)
	{
		SetTimer(TIME_WAIT,1500);
	}
	else
	{
		SetTimer(TIME_WAIT,20);
	}
	

	return TRUE;
}

//�Ѿ���������
BOOL CServerGameDesk::AlreadyShow()
{
	bool bFinish = false;
	int iShuttle = 0,iGiveUp = 0;

	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if(!m_pUserInfo[i] || !m_bUserState[i])
			continue;

		if(m_iUserCardCount[i] == 5)//�Ѿ����������Ʋ�������ַ�Χ
			return FALSE;

		if (m_iUserStation[i] == TYPE_SHUTTLE)
		{
			iShuttle ++;
			continue;
		}
		if (m_iUserStation[i] == TYPE_GIVE_UP)
		{
			iGiveUp ++;
			continue;
		}
	}
	return (iShuttle + iGiveUp) == CountPlayer();
}

//����Ƿ����
BOOL CServerGameDesk::CheckFinish()
{
	//����Ƿ����
	/*******************
	����������
	*******************/
	bool bFinish = false;
	int iShuttle = 0,iGiveUp = 0;
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if(!m_pUserInfo[i] || !m_bUserState[i])
			continue;
		if(m_iUserCardCount[i] == 5)
			bFinish = true;	
		if (m_iUserStation[i] == TYPE_SHUTTLE)
		{
			iShuttle ++;
			continue;
		}
		if (m_iUserStation[i] == TYPE_GIVE_UP)
		{
			iGiveUp ++;
			continue;
		}
	}

	if (((iShuttle + iGiveUp) == CountPlayer())&& bFinish || (iGiveUp ==  CountPlayer() - 1))			//�����ҷ���,�������Ѿ������ҷ����ĺ���������Ϊ��Ϸ����
	{	//���ռ����г���
				CollectJeton(true);
		//����
		SetTimer(TIME_GAME_FINISH,600);
		return TRUE;
	}
	return FALSE;
}

//���ռ����г���
BOOL CServerGameDesk::CollectJeton(BOOL bOnly)
{
	KillTimer(TIME_COLLECT_JETON);

	CollectJetonStruct jeton;
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if(NULL != m_pUserInfo[i])
		{
			SendGameData(i,&jeton,sizeof(jeton),MDM_GM_GAME_NOTIFY,ASS_COLLECT_JETON,0);
		}	
	}
	SendWatchData(m_bMaxPeople,&jeton,sizeof(jeton),MDM_GM_GAME_NOTIFY,ASS_COLLECT_JETON,0);

	if(!bOnly)
		NewPlayTurn(m_iOutCardPeople);
	return TRUE;
}

//������ע
BOOL CServerGameDesk::SendLimit()
{
	// ���±�������  (�ͻ���û�д������Ϣ)
	LimitStruct limit;
	limit.iNoteTime = m_bNoteTimes;
	limit.iLimit = m_iThisNoteLimit; 
	for (int i = 0; i < PLAY_COUNT;i++) 
		SendGameData(i,&limit,sizeof(limit),MDM_GM_GAME_NOTIFY,ASS_NOTE_LIMIE,0);
	SendWatchData(m_bMaxPeople,&limit,sizeof(limit),MDM_GM_GAME_NOTIFY,ASS_NOTE_LIMIE,0);
	return true;
}

//��һ�ֿ�ʼ
BOOL CServerGameDesk::NewPlayTurn(BYTE bDeskStation)
{
	OutputDebugString("lwlog::��һ�ֿ�ʼ");
	NewTurn messNewTurn;
	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if(m_pUserInfo[i] == NULL)
			continue;
		SendGameData(i,&messNewTurn,sizeof(messNewTurn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);
	}
	SendWatchData(m_bMaxPeople,&messNewTurn,sizeof(messNewTurn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);

	m_bAddTime = 0;					//��ע���
	::memset(m_UserThisTurnAct,0,sizeof(m_UserThisTurnAct));	//�����ļ���ע���ͳ��

	memset(m_byUserAddNum,0,sizeof(m_byUserAddNum));//���ָ�����Ҽ�ע����
	m_bNoteTimes ++;
	m_iNowBigPeople = -1; 
	m_iLastOutCardPeople = -1;
	m_iOutCardPeople = -1;

	//ÿ�˷�һ����
	bool bIs5Card = false;

	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if(NULL == m_pUserInfo[i] || false == m_bUserState[i])
			continue;
		if (m_iUserCardCount[i] != SH_USER_CARD)
			continue;
		bIs5Card = true;
	}

	if (bIs5Card) 
	{
		SetTimer(TIME_GAME_FINISH,600);
		return TRUE;
	}
	m_iFirstOutPeople = GetCardGreater(false);
	//��ʼ����λ��
	m_iSendPos = 0;
	::memset(m_iUserNote,0,sizeof(m_iUserNote));				// ������ע��
	::memset(m_iNowNote,0,sizeof(m_iNowNote));					// ���ҵ�ǰ��ע��
	SetTimer(TIME_SEND_A_CARD,500);
	return FALSE;
}

//�°lһ����
BOOL CServerGameDesk::SendACard()
{
	KillTimer(TIME_SEND_A_CARD);	
	if(m_iSendPos >= PLAY_COUNT)//δ������ÿ�˷���һ����
	{
		KillTimer(TIME_SEND_A_CARD);
		if(!AlreadyShow())
		{
			SendUserNote();
		}
		else
		{
			//��ʼ����λ��
			m_iSendPos = 0;
			SetTimer(TIME_SEND_A_CARD,m_iSendCardTime);
		}
		return true;
	}

	SendOneTurnCard pSendCardBag;

	int pos = (m_iFirstOutPeople + m_iSendPos) % PLAY_COUNT;		//��ʼ����λ��

	if(pos < 0 || pos >= PLAY_COUNT)
	{
		GameFinish(0,GF_SALE);
		return false;
	}


	
	pSendCardBag.bStartPos = pos;
	///������ʼλ��
	for(int i = 0 ;i<PLAY_COUNT;i++)
	{
		if (m_iUserStation[pos] != TYPE_GIVE_UP && m_pUserInfo[pos] && m_bUserState[pos])
		{
			
			pSendCardBag.iTurnCard[pos] = m_iUserCard[pos][ m_iUserCardCount[ pos ]];
			m_iUserCardCount[pos] ++;
			
			pos = (pos+1) % PLAY_COUNT;
		}
		else
		{
				
			pos = (pos+1) % PLAY_COUNT;
		}
		m_iSendPos ++;
	}

	for(int i = 0 ;i<PLAY_COUNT;i++)
	{
		SendGameData(i,&pSendCardBag,sizeof(pSendCardBag),MDM_GM_GAME_NOTIFY,ASS_SEND_A_CARD,0);

	}
	SendWatchData(m_bMaxPeople,&pSendCardBag,sizeof(pSendCardBag),MDM_GM_GAME_NOTIFY,ASS_SEND_A_CARD,0);

	SetTimer(TIME_SEND_A_CARD,500);

	return TRUE;

}

//�l����ע��Ϣ
BOOL CServerGameDesk::SendUserNote()
{
	//����Ƿ���Խ���(����һ�ֶ������������½��з���)����Ϸ�������ʡ
	if(CheckFinish())
	{
		return true;
	}
	m_iOutCardPeople = GetCardGreater(false);
	//��һ����ע��
	tagUserResult result;
	memset((void*)&result, 0, sizeof(tagUserResult));
	result.bNoteTimes = m_bNoteTimes;
	result.iOutPeople = m_iOutCardPeople;
	result.iNowBigNote = 0;
	result.iNowBigNoteStyle = 0;
	result.bNewTurn=true;
	memcpy(result.m_byUserAddNum,m_byUserAddNum,sizeof(m_byUserAddNum));//���ָ�����Ҽ�ע���� lym

	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] /*&& m_bUserState[i]*/)
			SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
	}
	SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);

	SetTimer(TIME_NOTE,(m_iThinkTime+3)*1000);
	return TRUE;
}

//��ȡ�������
int CServerGameDesk::GetCardGreater(BOOL bExtVal)
{
	int iGreater = -1;

	for (int i = 0;i < PLAY_COUNT; i ++)
	{	
		if (m_iUserStation[i] == TYPE_GIVE_UP||!m_pUserInfo[i] || !m_bUserState[i]) 
			continue;

		if (iGreater < 0)
		{
			iGreater = i;
			continue;
		}

		int ret; 		
		if(!bExtVal)		//�Ƚ�����
		{
			ret = m_Logic.CompareCard(m_iUserCard[iGreater]+1,m_iUserCardCount[iGreater]-1,m_iUserCard[i]+1,m_iUserCardCount[i]-1);
		}
		else				//�Ƚ�������
			ret= m_Logic.CompareCard(m_iUserCard[iGreater],m_iUserCardCount[iGreater],m_iUserCard[i],m_iUserCardCount[i]);

		if (ret > 0)
			continue;
		else iGreater = i;
	}
	return iGreater;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	CString str;
	str.Format("xyh_postion:bCloseFlag=%d",bCloseFlag);
	OutputDebugString(str);

	//��д����
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			m_bGameStation = GS_WAIT_NEXT;

			int winer = GetCardGreater(true);

			int  iGiveUpCount = 0;

			__int64 minval = -1;
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				if(m_pUserInfo[i] == NULL || !m_bUserState[i])
					continue;
				if (-1 == minval)
				{
					minval = m_pUserInfo[i]->m_UserData.i64Money;
				}
				else
				{
					minval = min(minval,m_pUserInfo[i]->m_UserData.i64Money);
				}
			}

			if (m_bPK)//PKģʽ�£�������ô����������������ҵĽ��zht
			{
				m_iTotalNote = 0;
			}
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				if (NULL == m_pUserInfo[i] || !m_bUserState[i])
				{
					continue;
				}
				if (TYPE_GIVE_UP == m_iUserStation[i])
				{
					iGiveUpCount++;
				}
				if (m_bPK)//PKģʽ�£�������ô����������������ҵĽ��zht
				{
					m_iPerJuTotalNote[i] = minval;
					m_iTotalNote += minval;
				}
			}
			bool bNoShowPlayCardShape = iGiveUpCount>=(PLAY_COUNT-1)?true:false;

			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			//�����ͻ��˵�ǰ����
			for (int i = 0; i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL || !m_bUserState[i]) 
					continue;
				//���ʹ���
				if(m_iUserStation[i] != TYPE_GIVE_UP &&m_iUserCardCount[i] == 5&&bNoShowPlayCardShape == false)
				{
					GameEnd.iUserState[i] = m_iUserStation[i];
					GameEnd.iCardShape[i] = m_Logic.GetCardShape(m_iUserCard[i],m_iUserCardCount[i]);
					GameEnd.bShowCardShape[i] = TRUE;
				}
				else
				{
					GameEnd.iUserState[i] = m_iUserStation[i];
					GameEnd.bShowCardShape[i] = FALSE;

					if(i != winer)
					{
						GameEnd.iCardShape[i] = 0;
					}
					else 
						GameEnd.iCardShape[i] = 0;
				}
			}

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] == NULL || !m_bUserState[i]) 
					continue;

				if(i == winer)
				{
					GameEnd.iTurePoint[i] = m_iTotalNote - m_iPerJuTotalNote[i];
				}
				else
				{
					GameEnd.iTurePoint[i] = - m_iPerJuTotalNote[i];				//�������Ѻ��ע
				}
			}
			
			CString str;
			str.Format("xyh_postion:m_bAIWinAndLostAutoCtrl=%d",m_bAIWinAndLostAutoCtrl);
			OutputDebugString(str);

			//20121203dwj ��¼��������Ӯֵ �ڿ�˰֮ǰ��¼ �������ֻ�������Ӯ��ƽ������;
			RecordAiHaveWinMoney(&GameEnd);

			bool temp_cut[8];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(GameEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);
			//ͳ���ۻ��ɼ�
			for(int i=0; i < PLAY_COUNT; i++) 
			{
				if (0!=GameEnd.iChangeMoney[i] && m_pUserInfo[i]) 
				{
					m_i64AcculateGrade[i]+=GameEnd.iChangeMoney[i];
					m_i64UpGrade[i]=GameEnd.iChangeMoney[i];
				}
			}
			memcpy(GameEnd.i64AcculateGrade,m_i64AcculateGrade,sizeof(GameEnd.i64AcculateGrade));
			memcpy(GameEnd.i64UpGrade,m_i64UpGrade,sizeof(GameEnd.i64UpGrade));
			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));
			}

			for(int i = 0;i < PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i] == NULL || !m_bUserState[i]) 
					continue;
			}
			

			///2010-9-9���´���ԭ����Ϊ��;ǿ���߱��棬����ϷΪǿ���йܣ��������򹫹�̨��ע��
			////д�����ݿ��
			//for (int i = 0;i < PLAY_COUNT;i++)
			//{
			//	Msg("AAAAA::Server--1 %d___%d,%d",i,GameEnd.iTurePoint[i], GameEnd.iChangeMoney[i]);

			//	if(i == winer)
			//	{
			//		if(GameEnd.iChangeMoney[i] == 0)
			//		{	
			//			//GameEnd.iChangeMoney[i] = GameEnd.iTurePoint[i];
			//		}
			//	}
			//	else
			//	{
			//		if(m_pDataManage->m_InitData.uComType == TY_MONEY_GAME)
			//		{
			//			GameEnd.iChangeMoney[i] =  - m_iPerJuTotalNote[i]*GetRoomBasePoint();
			//			GameEnd.iTurePoint[i] = - m_iPerJuTotalNote[i];
			//		}
			//		else
			//		{
			//			GameEnd.iTurePoint[i] = - m_iPerJuTotalNote[i];
			//		}
			//	}
			//}
			//��������
			for (int i = 0 ; i < PLAY_COUNT; i ++) 
			{
				Msg("AAAAA::Server--2 %I64d___%I64d,%I64d",i,GameEnd.iTurePoint[i], GameEnd.iChangeMoney[i]);
				if (m_pUserInfo[i] != NULL /*&& m_bUserState[i]*/) 
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			}
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);

			bCloseFlag = GFF_FORCE_FINISH;

			/*for (BYTE i=0; i<PLAY_COUNT; i++)
			{
				if (NULL!=m_pUserInfo[i] && m_bNetCutPlayer[i])
				{
					MakeUserOffLine(i);
				}
			}*/

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GF_AHEAD://��ǰ����
		{
			m_bGameStation = GS_WAIT_ARGEE;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL && m_bUserState[i]) 
					m_pUserInfo[i] -> m_UserData.bUserState = USER_SITTING;
			}
			GameAheadStruct AheadEnd;
			::memset(&AheadEnd,0,sizeof(AheadEnd));
			AheadEnd.bDeskStation = bDeskStation;

			__int64 minval = -1;
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				if(m_pUserInfo[i] == NULL || !m_bUserState[i])
					continue;
				if (-1 == minval)
				{
					minval = m_pUserInfo[i]->m_UserData.i64Money;
				}
				else
				{
					minval = min(minval,m_pUserInfo[i]->m_UserData.i64Money);
				}
			}

			if (m_bPK)//PKģʽ�£�������ô����������������ҵĽ��zht
			{
				m_iTotalNote = 0;
			}
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				if (NULL == m_pUserInfo[i] || !m_bUserState[i])
				{
					continue;
				}
				
				if (m_bPK)//PKģʽ�£�������ô����������������ҵĽ��zht
				{
					m_iPerJuTotalNote[i] = minval;
					m_iTotalNote += minval;
				}
			}

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if(m_pUserInfo[i] == NULL || !m_bUserState[i])
					continue;
				if(i == bDeskStation)						//δ��Ȩ�ߵõ����������н��
				{
					AheadEnd.iTurePoint[i] = m_iTotalNote - m_iPerJuTotalNote[i];
				}
				else
				{
					AheadEnd.iTurePoint[i] = - m_iPerJuTotalNote[i];
				}
			}

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(AheadEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(AheadEnd.iChangeMoney);

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(AheadEnd.iTurePoint,0,sizeof(AheadEnd.iTurePoint));
			}


				///2010-9-9���´���ԭ����Ϊ��;ǿ���߱��棬����ϷΪǿ���йܣ��������򹫹�̨��ע��
			//���ݷ���˼�¼��ʾ
			//for (int i = 0; i < PLAY_COUNT; i ++)
			//{
			//	if(i == bDeskStation)//δ��Ȩ�ߵõ����������н��
			//		;
			//	else
			//	{
			//		AheadEnd.iTurePoint[i] = - m_iPerJuTotalNote[i];
			//		
			//		// ������� UserFourceQuit �п۳� ����ֻ����ʾ��Ҫ �������¸�ֵ
			//		if ( m_pDataManage->m_InitData.uComType==TY_MONEY_GAME )
			//			AheadEnd.iChangeMoney[i] = AheadEnd.iTurePoint[i];
			//	}
			//}

			for (int i = 0; i < m_bMaxPeople; i ++)
				SendGameData(i,&AheadEnd,sizeof(AheadEnd),MDM_GM_GAME_NOTIFY,ASS_AHEAD_END,0);
			SendWatchData(m_bMaxPeople,&AheadEnd,sizeof(AheadEnd),MDM_GM_GAME_NOTIFY,ASS_AHEAD_END,0);

			bCloseFlag = GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}

	case GF_NO_WINER://��ʤ����(ʤ������,δ�뿪�߷���)
		{
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL && m_bUserState[i]) 
					m_pUserInfo[i] -> m_UserData.bUserState = USER_SITTING;
			}
			GameAheadStruct AheadEnd;
			::memset(&AheadEnd,0,sizeof(AheadEnd));
			AheadEnd.bDeskStation = bDeskStation;

			for (int i = 0;i < PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] == NULL || !m_bUserState[i]) 
					continue;
				AheadEnd.iTurePoint[i] = - m_iPerJuTotalNote[i];
			}

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(AheadEnd.iTurePoint, temp_cut);
			RecoderGameInfo(AheadEnd.iChangeMoney);

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(AheadEnd.iTurePoint,0,sizeof(AheadEnd.iTurePoint));
			}

			//���ݷ���˼�¼��ʾ
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if(i == bDeskStation)
					;
				else
					AheadEnd.iChangeMoney[i] = AheadEnd.iTurePoint[i] = - m_iPerJuTotalNote[i];
			}
			for (int i = 0; i < PLAY_COUNT; i ++)
				SendGameData(i,&AheadEnd,sizeof(AheadEnd),MDM_GM_GAME_NOTIFY,ASS_NO_WINER,0);
			SendWatchData(m_bMaxPeople,&AheadEnd,sizeof(AheadEnd),MDM_GM_GAME_NOTIFY,ASS_NO_WINER,0);

			bCloseFlag = GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
		{
			m_bGameStation = GS_WAIT_ARGEE;

			for (int i = 0 ; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL && m_bUserState[i]) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation = bDeskStation;
			for (int i = 0; i < PLAY_COUNT; i ++)
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			bCloseFlag = GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_FORCE_FINISH:		//ǿ��
		{
//			//��������
//			m_bGameStation = GS_WAIT_ARGEE;
//
//			for (int i = 0; i < PLAY_COUNT; i ++)
//			{
//				if (m_pUserInfo[i] != NULL) 
//					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
//			}
//			GameCutStruct CutEnd;
//			::memset(&CutEnd,0,sizeof(CutEnd));
//			CutEnd.bDeskStation = bDeskStation;
//			CutEnd.iTurePoint[bDeskStation] =-GetRunPublish() - (m_iPerJuTotalNote[bDeskStation]);
//			CutEnd.iTurePoint[(bDeskStation + 1) % PLAY_COUNT] = 0;
//#ifdef SRSH
//			CutEnd.iTurePoint[(bDeskStation + 2) % PLAY_COUNT] = 0;
//			CutEnd.iTurePoint[(bDeskStation + 3) % PLAY_COUNT] = 0;
//#endif
//			bool temp_cut[PLAY_COUNT];
//			memset(temp_cut,0,sizeof(temp_cut));
//			ChangeUserPoint(CutEnd.iTurePoint, temp_cut);
//			__super::RecoderGameInfo(CutEnd.iChangeMoney);
//
//			for (int i = 0; i < PLAY_COUNT; i ++)
//				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
//			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
//
//			bCloseFlag = GFF_FORCE_FINISH;
//
//			ReSetGameState(bCloseFlag);
//			__super::GameFinish(bDeskStation,bCloseFlag);

			UserFourceQuit(bDeskStation);
			return true;
		}
	}

	//��������
	ReSetGameState(bCloseFlag);
	return __super::GameFinish(bDeskStation,bCloseFlag);
}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if (!m_bUserState[bDeskStation])
	{
		return false;
	}
	return 	m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT;
}

//������Ϸ�ж��Ƿ�����Ϸ��
BOOL CServerGameDesk:: IsPlayingByGameStation() 
{
	return 	m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT;
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	//if(m_iUserStation[bDeskStation] != TYPE_GIVE_UP && IsPlayingByGameStation()) //��Ϸ�вŴ��������Ϣ
	//	UserAI(bDeskStation,1);

	
/*
	if(IsPlayingByGameStation())
	{
		GameFinish(bDeskStation,GFF_FORCE_FINISH);
	}*/
	/*if(m_dwDeskOwer == pUserInfo->m_UserData.dwUserID)
	{
		ChangeDeskOwer(bDeskStation);
	}*/
	m_i64AcculateGrade[bDeskStation]=0;
	m_i64UpGrade[bDeskStation]=0;
	__super::UserLeftDesk(bDeskStation,pUserInfo);
	LeaveDeskMes tLeaveDesk;
	tLeaveDesk.byDeskStation=bDeskStation;
	for (int i=0; i < PLAY_COUNT; i++) 
	{
		if (m_pUserInfo[i]!=NULL) 
			SendGameData(i,&tLeaveDesk,sizeof(tLeaveDesk),MDM_GM_GAME_NOTIFY,ASS_LEAVE_DESK,0);
	}
	SendWatchData(m_bMaxPeople,&tLeaveDesk,sizeof(tLeaveDesk),MDM_GM_GAME_NOTIFY,ASS_LEAVE_DESK,0);
	return 0;
}

//����̨��
void CServerGameDesk::ChangeDeskOwer(BYTE bDeskStation)
{
	//����̨��
	m_dwDeskOwer = 0L;
	//m_DeskBasePoint=0;
	for(int i = 0; i < MAX_PEOPLE; i ++)
	{
		if(i == bDeskStation)
			continue;
		// ������һ��̨��
		if(m_pUserInfo[i] && m_bUserState[i])
		{
			bDeskStation = m_pUserInfo[i]->m_UserData.dwUserID;
			break;
		}
	}
}

//�û������뿪
bool CServerGameDesk::UserHaveThing(BYTE bDeskStation, char * szMessage)
{
	return TRUE;
}

//ͬ���û��뿪
bool CServerGameDesk::ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee)
{
	return TRUE;
}

//�������������ҵ÷�
int CServerGameDesk::GetRunAwayOtherGetPoint(BYTE bDeskStation,BYTE bRunDeskStation )
{
	return 0;
}

//���ܿ۷�
int CServerGameDesk::GetRunPublish()
{
	return m_pDataManage->m_InitData.uRunPublish;
}

//���ܿ۽��
__int64 CServerGameDesk::GetRunPublishMoney(BYTE bDeskStation)
{
	__int64 iMoney = m_iPerJuTotalNote[bDeskStation];
	return iMoney;
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

//ͳ�Ʊ���
__int64 CServerGameDesk::GetHumanBasePoint()
{
	__int64 minval = -1;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_pUserInfo[i] == NULL || !m_bUserState[i])
			continue;

		if(m_iUserStation[i] == TYPE_GIVE_UP)
			continue;

		if (-1 == minval)
		{
			minval = m_pUserInfo[i]->m_UserData.i64Money;
		}
		else
		{
			minval = min(minval,m_pUserInfo[i]->m_UserData.i64Money);
		}
	}

	return  minval;
}

//�ж�
BOOL CServerGameDesk::Judge()
{
	if(JudgeWiner())		//�ȴ���ʤ��
	{
		return TRUE;
	}

	JudgeLoser();				//��������

	return TRUE;
}

//�ж�ʤ��
BOOL CServerGameDesk::JudgeLoser()
{
	try
	{
		TCHAR skin[MAX_NAME_INFO];
		CString s = CINIFile::GetAppPath ();/////����·��
		CString temp = _T("");
		temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
		CINIFile f( temp );
		CString key = TEXT("loser");
		int count = f.GetKeyVal(key,"count",0);
		if(count <= 0 )					//��ʤ��
			return FALSE;
		TCHAR szName[21];
		BYTE  bFind = 255;				//ʤ������
		int probability = 0;
		CString szNickName;
		int NameId;
		for (int j = 0; j < PLAY_COUNT; j++)
		{
			if(m_pUserInfo[j] == NULL || !m_bUserState[j])
				continue;

			for(int i = 0; i < count; i ++)
			{
				wsprintf(szName,"nickname%d",i);
				szNickName = f.GetKeyVal(key,szName,"wing");

				if(strcmp(m_pUserInfo[j]->m_UserData.nickName,szNickName) != 0)
					continue;
				//����ʤ��(��ֻ����һ��)
				wsprintf(szName,"prob%d",i);
				probability = f.GetKeyVal(key,szName,0);
				bFind = j;
				break;
			}

		}

		//��ǰ�������ʤ��
		if(bFind == 255)
			return FALSE;

		BYTE bDest = GetWiner();
		srand(GetTickCount()); 

		if((rand() % 100) <= probability)	//һ������
		{
			if(bFind == bDest)
				ChangeCard(bFind,(bDest+1)%PLAY_COUNT);
		}
		else								//һ����ʤ
		{
			if(bFind != bDest)
				ChangeCard(bFind,bDest);
		}
		return TRUE;
	}
	catch(...)
	{
		return TRUE;
	}
	return FALSE;
}

//�ж�ʤ��
BOOL CServerGameDesk::JudgeWiner()
{
	try
	{
		TCHAR skin[MAX_NAME_INFO];
		CString s = CINIFile::GetAppPath ();/////����·��
		CString temp = _T("");
		temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
		CINIFile f( temp );
		CString key = TEXT("winer");
		int count = f.GetKeyVal(key,"count",0);

		if(count <= 0 )					//��ʤ��
			return FALSE;

		TCHAR szName[21];
		BYTE  bFind = 255;				//ʤ������
		int probability = 0;
		CString szNickName;
		for (int j = 0; j < PLAY_COUNT; j++)
		{
			if(m_pUserInfo[j] == NULL || !m_bUserState[j])
				continue;
			for(int i = 0; i < count; i ++)
			{
				wsprintf(szName,"nickname%d",i);
				szNickName = f.GetKeyVal(key,szName,"wing");

				if(strcmp(m_pUserInfo[j]->m_UserData.nickName,szNickName) != 0)
					continue;
				//����ʤ��(��ֻ����һ��)
				wsprintf(szName,"prob%d",i);
				probability = f.GetKeyVal(key,szName,0);
				bFind = j;
				break;
			}
			if(bFind != 255)
				break;
		}

		//��ǰ�������ʤ��
		if(bFind == 255)
			return FALSE;
		BYTE bDest = GetWiner();
		srand(GetTickCount()); 
		if((rand() % 100) <= probability)//����ʤ��
		{
			if(bFind != bDest)
				ChangeCard(bFind,bDest);
		}
		else	//һ������
		{
			if(bFind == bDest)
				ChangeCard(bFind,(bDest + 1) % PLAY_COUNT);
	
		}
	
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
	return FALSE;
}

//˭ʤ˭��
BYTE CServerGameDesk::GetWiner()
{
	BYTE bDeskStation = 0;
	try
	{
		for(int i = 1; i < PLAY_COUNT ;i ++)
		{
			if(m_Logic.CompareCard(&(m_iTotalCard[bDeskStation * SH_USER_CARD]),SH_USER_CARD,&(m_iTotalCard[i * SH_USER_CARD]),SH_USER_CARD) < 0)
				bDeskStation = i;
		}
	}
	catch(...)
	{
		return 0;
	}
	return bDeskStation;
}

//����
BOOL CServerGameDesk::ChangeCard(BYTE bDestStation,BYTE bWinStation)
{
	for(int i = 0; i < SH_USER_CARD; i ++)
	{
		BYTE bTemp = m_iTotalCard[bDestStation * SH_USER_CARD + i];

		m_iTotalCard[bDestStation * SH_USER_CARD + i] = m_iTotalCard[bWinStation * SH_USER_CARD + i];

		m_iTotalCard[bWinStation * SH_USER_CARD + i] = bTemp;
	}

	return TRUE;
}

//�������
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	if(m_dwDeskOwer == 0 && pUserInfo != NULL)
		m_dwDeskOwer = pUserInfo->m_UserData.dwUserID;
	return __super::UserSitDesk(pUserSit,pUserInfo);
}

//�ж��Ƿ񳬼��û�//lym 2011-11-28
bool CServerGameDesk::IsSuperUser(BYTE station)
{
	if(m_pUserInfo[station] == NULL)
		return false;
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	CINIFile f(temp );
	CString key = TEXT("super");
	CString value = "";
	int count = f.GetKeyVal(key,"count",0);
	int userid = 0;
	for(int i=0;i<count;++i)
	{
		value.Format("superid_%d",i);
		userid = f.GetKeyVal(key,value,0);
		if(m_pUserInfo[station]->m_UserData.dwUserID == userid)
		{
			return true;
		}
	}
	return false;
}
//-------------------------------add-by-DWJ-20121113-----------------------------------------------------------------------------------------
//�������ļ��ж�ȡ�������Ѿ�Ӯ�˶���Ǯ����Ϊ��ÿ�̽����ʱ�򶼻Ὣ������ӮǮ����д�������ļ��С� 
//������ÿ�̿�ʼ��ʱ���ֶ�ȡ�£���Ϊ�������޸�����ô�ͻ�������Ǯ��.
void CServerGameDesk::GetAIContrlSetFromIni(int iRoomID)
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	CINIFile f(temp );
	CString szSec = TEXT("game");
	szSec.Format("%d_%d",NAME_ID,iRoomID);

	m_iAIWantWinMoneyA1	= f.GetKeyVal(szSec,"AIWantWinMoneyA1 ",m_iAIWantWinMoneyA1);		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(szSec,"AIWantWinMoneyA2 ",m_iAIWantWinMoneyA2);		/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(szSec,"AIWantWinMoneyA3 ",m_iAIWantWinMoneyA3);		/**<������ӮǮ����3  */
	m_iAIWinLuckyAtA1	= f.GetKeyVal(szSec,"AIWinLuckyAtA1 ",m_iAIWinLuckyAtA1);			/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(szSec,"AIWinLuckyAtA2 ",m_iAIWinLuckyAtA2);			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(szSec,"AIWinLuckyAtA3 ",m_iAIWinLuckyAtA3);			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(szSec,"AIWinLuckyAtA4 ",m_iAIWinLuckyAtA4);			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAddMoneyEveryDay	= f.GetKeyVal(szSec,"AddMoneyEveryDay ",m_iAddMoneyEveryDay);		/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",m_bAIWinAndLostAutoCtrl);//��������Ӯ����20121122dwj

	CheckNewDayReSetIni(&f,&szSec);
}

//---------------------------add-by-DWJ-20121113---------------------------------------------------------
//����Ƿ����һ��,����һ���Ҫ�޸������ļ���A1~A3�����ֵ
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
	{//�����¼��ʱ��С�ڵ�ǰ�����ڣ���ʾ��ǰʱ�����24�㣬�����Ѿ���һ��,���µ�һ�졣
		m_iRecFroDayTime = StrToInt(sDay);
		if (0 !=m_iAddMoneyEveryDay)
		{//���ÿ�յ���Ǯ���Ĳ�Ϊ0�ſ�����A1��A2��A3����������ܣ�д�������ļ�����
			sValue.Format("%I64d",(m_iAIWantWinMoneyA1+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA1 ",sValue);		/**<������ӮǮ����1  */

			sValue.Format("%I64d",(m_iAIWantWinMoneyA2+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA2 ",sValue);		/**<������ӮǮ����2  */

			sValue.Format("%I64d",(m_iAIWantWinMoneyA3+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA3 ",sValue);		/**<������ӮǮ����3  */
		}
	}	
}

//-----------------------------------------------------------
/*�����˿�����Ӯ 
add by dwj 20121122
*/
void CServerGameDesk::IAWinAutoCtrl()
{
	if(!m_bAIWinAndLostAutoCtrl)
		return;

	bool bAIWin = false;
	srand((unsigned)GetTickCount());
	int iResult = rand()%100;

	if (0 >=m_iAIHaveWinMoney )				//������Ӯ��Ǯ����0 �����˱�Ӯ
	{
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

	CString str;
	str.Format("xyh_postion:bAIWin=%d",bAIWin);
	OutputDebugString(str);

	if (bAIWin)		//������ҪӮǮ
	{
		int iWiner = 255;

		if(false == GameWinerIsRobot(iWiner))
		{
			for (int i=0;i<PLAY_COUNT;i++)
			{	
				if(IsPlayGame(i) && m_pUserInfo[i]->m_UserData.isVirtual)
				{
					ChangeCard(i,iWiner);
					
					str.Format("xyh_postion:i=%d,iWiner=%d",i,iWiner);
					OutputDebugString(str);

					m_bChangeRobotCard[i] = true;

					return;
				}
			}	
		}
		else
		{
			OutputDebugString("xyh_postion:GameWinerIsRobot(iWiner)=true");
		}
	}

	return;
}

//------------------------------------------------------------------------------------
void CServerGameDesk::RecordAiHaveWinMoney(GameEndStruct *GameEnd)
{
	OutputDebugString("xyh_postion:RecordAiHaveWinMoney");

	if(!m_bAIWinAndLostAutoCtrl)
	{
		OutputDebugString("xyh_postion:m_bAIWinAndLostAutoCtrl=false");
		return;
	}

	OutputDebugString("xyh_postion:RecordAiHaveWinMoney11111111");

	try
	{//�����쳣������Ϊ���ˡ�����ţţ�漰���õ������ͬһ�������ļ���д��������������ˣ��Ͳ��������ļ���д����;
		//----DWJ-20121113---��
		__int64 iReSetAIHaveWinMoney;

		TCHAR skin[MAX_NAME_INFO];
		CString s = CINIFile::GetAppPath ();/////����·��
		CString temp = _T("");
		temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
		CINIFile f(temp );

		TCHAR szSec[_MAX_PATH] = TEXT("game");
		//_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);

		iReSetAIHaveWinMoney = f.GetKeyVal(szSec,"ReSetAIHaveWinMoney",(__int64)0);

		OutputDebugString("xyh_postion:RecordAiHaveWinMoney2222222");

		//�������ȣ�˵���ֶ��޸��������ļ��е�ֵ���û�������Ǯ�ˣ�����Ⱦ�ȥ�ۼӻ�����Ӯ��Ǯ;
		if (m_iReSetAIHaveWinMoney != iReSetAIHaveWinMoney)
		{
			m_iAIHaveWinMoney = iReSetAIHaveWinMoney;
			m_iReSetAIHaveWinMoney = iReSetAIHaveWinMoney;
		}
		else
		{
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(NULL != m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual > 0)///��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
				{//ֻ���������Ӯ
					m_iAIHaveWinMoney += GameEnd->iTurePoint[i];///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���ǮiChangeMoney
				}
			}
		}

		CString str;
		str.Format("xyh_postion:m_iAIHaveWinMoney=%I64d",m_iAIHaveWinMoney);
		OutputDebugString(str);

		//����ǰ������Ӯ��Ǯд�������ļ�����
		CString sTemp;
		sTemp.Format("%I64d",m_iAIHaveWinMoney);
		f.SetKeyValString(szSec,"AIHaveWinMoney",sTemp);
	}
	catch (...)
	{
		//����ֻ�����������ʩ��ʲôҲ������DWJ
		OutputDebugString("xyh_postion:д�����ļ�������!");
	}	

	OutputDebugString("xyh_postion:RecordAiHaveWinMoney44444444");
}
//-------------------------------------------------------------------------------
int CServerGameDesk::GameWinerIsRobot(int &iWiner)
{
	iWiner = GetWiner();

	if(IsPlayGame(iWiner) && m_pUserInfo[iWiner] != NULL && m_pUserInfo[iWiner]->m_UserData.isVirtual)
	{
		return true;
	}

	return false;
}
__int64 CServerGameDesk::GetHalfforLessUserMoney()//������ٽ��������Ͻ�ҵ�һ����
{
	__int64 i64LessMoney=0;
	bool bFlag=false;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i] || !m_bUserState[i])
		{
			continue;
		}
		if(bFlag)
		{
			i64LessMoney=(i64LessMoney<m_pUserInfo[i]->m_UserData.i64Money)?i64LessMoney:m_pUserInfo[i]->m_UserData.i64Money;
		}
		else
		{
			i64LessMoney=m_pUserInfo[i]->m_UserData.i64Money;
			bFlag=true;
		}
	}
	i64LessMoney=i64LessMoney/2;
	CString cs;
	cs.Format("lwlog::i64LessMoney=%d",i64LessMoney);
	OutputDebugString(cs);
	return i64LessMoney;
}

int CServerGameDesk::getFirstGreate(void)
{
	int iGreater = -1;
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_iUserStation[i] == TYPE_GIVE_UP||!m_pUserInfo[i] || !m_bUserState[i]) 
			continue;

		if (iGreater < 0)
		{
			iGreater = i;
			continue;
		}

		// = m_Logic.CompareCard(m_iUserCard[iGreater]+1,m_iUserCardCount[iGreater]-1,m_iUserCard[i]+1,m_iUserCardCount[i]-1);
		int cardValue1 = m_Logic.GetCardNum(m_iUserCard[iGreater][1]);
		int cardValue2 = m_Logic.GetCardNum(m_iUserCard[i][1]);
		if(cardValue1 != cardValue2)
		{
			iGreater = cardValue1-cardValue2 > 0 ? iGreater:i;
			continue;
		}

		cardValue1 = m_Logic.GetCardHuaKind(m_iUserCard[iGreater][1]);
		cardValue2 = m_Logic.GetCardHuaKind(m_iUserCard[i][1]);

		iGreater = cardValue1-cardValue2 < 0 ? iGreater:i;

	}

	CString cs;
	cs.Format("suoha:iGreater=%d",iGreater);
	OutputDebugString(cs);

	return iGreater;
}