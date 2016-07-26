#include "StdAfx.h"
#include "ServerManage.h"
#include "../../../../sdk/include/Common/writelog.h"
#include <math.h>
#include<vector>
/*
* ��ӡ��־�ļ�
*/
void DebugPrintf(int iRoomID, const char *p, ...)
{
	return;
	CTime time = CTime::GetCurrentTime();
	char szFilename[256];
	sprintf(szFilename, "%d_%d-%d%d%d-log.txt", NAME_ID, iRoomID, time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen(szFilename, "a");
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start(arg, p);
	vfprintf(fp, p, arg);
	fprintf(fp,"\n");
	fclose(fp);
}
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_iBeginTime = 20;			//��ʼ�ȴ�ʱ��
	m_iSendCardTime = 20;		//��������ʱ��
	m_iThinkTime = 20;			//˼��ʱ��
	m_bySeparateCardTime = 30;	//����ʱ��
	m_iAllCardCount=32;			//��Ϸ�����˿�����
	m_iFirstOutPeople=-1;		//��һ����ע��
	m_iOutCardPeople=-1;
	m_iLastOutPeople=-1;
	m_iNtPeople=-1;
	m_iUpGradePeople=0;						
	m_bGameStation=GS_WAIT_AGREE;//��Ϸ״̬
	memset(m_iTotalCard,0,sizeof(m_iTotalCard));
	memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));
	memset(m_iUserCard,0,sizeof(m_iUserCard));
	memset(m_iUserStation,0,sizeof(m_iUserStation));
	memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));
	memset(m_iNowNote,0,sizeof(m_iNowNote));
	memset(m_bUserReady,0,sizeof(m_bUserReady));
	memset(m_bAgree,0,sizeof(m_bAgree));
	memset(m_bAlreadSetBobo,0,sizeof(m_bAlreadSetBobo));
	memset(m_bSeparatCardFale,0x00,sizeof(m_bSeparatCardFale));
	memset(m_byNoteTime,0x00,sizeof(m_byNoteTime));
	memset(m_byOperationState,0xff,sizeof(m_byOperationState));
	m_iTotalNote=0;
	m_iFrontNote=0;
	m_iTimerCount = 0;
	m_byGameStation = 0xff;
	m_bSetAllFlag = false;
	m_bFollowFlag = false;
	InitAIData();  //��ʼ���й�����
	InitializeCriticalSection(&m_csForIPCMessage);
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_bSuperCardList[i],0,sizeof(m_bSuperCardList[i]));
	}
	m_bSuperStation = -1;
#endif
}
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();//����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	CINIFile f(temp);
	CString key = TEXT("game");
	m_iBeginTime = f.GetKeyVal(key,"begintime",20);//�ȴ���ʼʱ��
	if(m_iBeginTime<10)
	{
		m_iBeginTime=10;
	}
	m_iSendCardTime = f.GetKeyVal(key,"setbobotime",20);//��������ʱ��
	if(m_iSendCardTime < 10)
	{
		m_iSendCardTime = 10;
	}
	m_iThinkTime = f.GetKeyVal(key,"thinktime",20);//˼��ʱ��
	if(m_iThinkTime<10)
	{
		m_iThinkTime=20;
	}
	m_bySeparateCardTime = f.GetKeyVal(key,"separatecardtime",20);//����ʱ��
	if(m_bySeparateCardTime<10)
	{
		m_iThinkTime=10;
	}
	
//--------------------------------------------------------------
	m_i64BoBase = f.GetKeyVal(key,"BoboBase",10); //��Ƥ
	if(m_i64BoBase < 10)
	{
		m_i64BoBase = 10;
	}
	m_i64SmallBobo = f.GetKeyVal(key,"SmallBobo",100); //��С����
	if(m_i64SmallBobo < 10)
	{
		m_i64SmallBobo = 10;
	}	
	return true;
}

bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadIni();
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
//������ע
BOOL CServerGameDesk::LoadExtIni()
{
	return TRUE;
}
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();//����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	CINIFile f(temp);
	TCHAR szKey[20];
	wsprintf(szKey,"%d_%d",NAME_ID,iRoomID);
	CString key = szKey;
	m_iBeginTime = f.GetKeyVal(key,"begintime",m_iBeginTime);//�ȴ���ʼʱ��
	if(m_iBeginTime<10)
	{
		m_iBeginTime=20;
	}
	m_iThinkTime = f.GetKeyVal(key,"thinktime",m_iThinkTime);//˼��ʱ��
	if(m_iThinkTime<10)
	{
		m_iThinkTime=20;
	}
	m_iSendCardTime = f.GetKeyVal(key,"setbobotime",m_iSendCardTime);//��������ʱ��
	if(m_iSendCardTime < 10)
	{
		m_iSendCardTime = 10;
	}
	m_bySeparateCardTime = f.GetKeyVal(key,"separatecardtime",m_bySeparateCardTime);//����ʱ��
	if(m_bySeparateCardTime<10)
	{
		m_iThinkTime=10;
	}
//--------------------------------------------------------------
	m_i64BoBase = f.GetKeyVal(key,"BoboBase",m_i64BoBase); //��Ƥ
	if(m_i64BoBase < 10)
	{
		m_i64BoBase = 10;
	}

	m_i64SmallBobo = f.GetKeyVal(key,"SmallBobo",m_i64SmallBobo); //��С����
	if(m_i64SmallBobo < 10)
	{
		m_i64SmallBobo = 10;
	}	
//--------------------------------------------------------------
	return TRUE;
}
//��������
CServerGameDesk::~CServerGameDesk(void)
{
	DeleteCriticalSection(&m_csForIPCMessage);
}

bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	if (uTimerID != TIME_MY_TIMER)
	{
		return __super::OnTimer(uTimerID);
	}
	for (int i=0; i<TIMER_COUNT; ++i)
	{
		if (m_arTimer[i].nId == 0)
		{
			continue;
		}
		++m_arTimer[i].nTickCount;
		if (m_arTimer[i].nTickCount >= m_arTimer[i].nTriggerCount)
		{
			m_arTimer[i].nTickCount = 0;
			OnMyTimer(m_arTimer[i].nId);
		}
	}
	return true;
}
//��ʱ����Ϣ
bool CServerGameDesk::OnMyTimer(UINT uTimerID)
{
	if(uTimerID==TIME_BOBO_RESET_TIME)
	{
		if(-- m_iSendCardRes)
			return true;
		KillMyTimer(TIME_BOBO_RESET_TIME);
	}
	if (uTimerID==TIME_SEND_CARD)				//��������ʱ��
	{
		if (m_bGameStation==GS_SEND_CARD) 
			SendCard();
		else KillMyTimer(TIME_SEND_CARD);
		return TRUE;
	}
	else if(uTimerID == TIME_SEND_ONE_CARD)
	{
		if (m_bGameStation==GS_PLAY_GAME) 
			SendOneCardFinsh();
		else KillMyTimer(TIME_SEND_ONE_CARD);
	}
	else if (uTimerID==TIME_GAME_FINISH)	//������ʱ��
	{
		KillMyTimer(TIME_GAME_FINISH);
		if (m_bGameStation==GS_PLAY_GAME) 
			GameFinish(0,GF_NORMAL);
	}
	else if (TIMER_SEND_END == uTimerID)
	{
		KillMyTimer(TIMER_SEND_END);
		if (m_bGameStation==GS_SEND_CARD) 
			SendCardFinish();

	}
	else if (uTimerID == TIME_CUT_AI)
	{
		KillMyTimer(TIME_CUT_AI);
		ExcutNetCutAI();
		return true;
	}
	else if (uTimerID == TIMER_WAIT_NOTE)
	{
		if (--m_iTimerCount>0)
		{
			return true;
		}
		KillMyTimer(TIMER_WAIT_NOTE);
		// ���ʱ�䵽�˻������ƣ�����Ϊ����
		UserActiveResult(m_iOutCardPeople,TYPE_GIVE_UP,0);
		//UserCallScore(m_bCurrentCallScore,0);
	}
	else if(uTimerID == TIME_SEPARATE)
	{
		KillMyTimer(TIME_SEPARATE);
		if (m_bGameStation==GS_PLAY_GAME) 
		{
			SeparateCardFinsh();
		}
	}
	return __super::OnTimer(uTimerID);
}

//=================================================================================================
// ���� : ����ʱ�����
//=================================================================================================
void CServerGameDesk::SeparateCardFinsh()
{
	T_SC_SeparatFinish SeparatFinish;
	int iCmpResult = 0x00;
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{
		if(!m_bSeparatCardFale[i])						//��û�з���
		{
			if(m_pUserInfo[i])							//ͬʱ����
			{
				iCmpResult = m_Logic.CompareTwoCardHand(&m_iUserCard[i][0],0x02,&m_iUserCard[i][2],0x02);
				{
					if(iCmpResult < 0x00)
					{
						BYTE byTemp = 0x00; 
						for(int j = 0x00;j < 0x02;j ++)
						{
							byTemp = m_iUserCard[i][j];
							m_iUserCard[i][j] = m_iUserCard[i][j + 2];
							m_iUserCard[i][j + 2] = byTemp;
						}
					}
				}
			}
		}
	}
	
	memcpy(SeparatFinish.bySeparatCard,m_iUserCard,sizeof(m_iUserCard));
	memcpy(SeparatFinish.byCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));
	for(int i = 0x00;i < PLAY_COUNT;i ++ )
	{
		if(m_pUserInfo[i])
		{
				SendGameData(i,&SeparatFinish,sizeof(SeparatFinish),MDM_GM_GAME_NOTIFY,ASS_SC_SHOW_ALL_SEPARAT_CARD,0);
		}
	}
	SendWatchData(m_bMaxPeople,&SeparatFinish,sizeof(SeparatFinish),MDM_GM_GAME_NOTIFY,ASS_SC_SHOW_ALL_SEPARAT_CARD,0);

	SetMyTimer(TIME_GAME_FINISH, 5000);
}
//=================================================================================================
//���� : ֪ͨ��һ�������
//=================================================================================================
void CServerGameDesk::SendOneCardFinsh()
{
	
	KillMyTimer(TIME_SEND_ONE_CARD);
	if(m_iAlreadySendCard != 0x04)
	{
		tSendOneCardFinsh tSendOneCard;

		for(int i = 0x00;i < PLAY_COUNT;i ++)
		{
			if(m_byOperationState[i] != 0x04)			//���û�з���,��������0x00
			{
				m_byOperationState[i] = 0xff;
			}
		}
		memcpy(tSendOneCard.byUserOperatorType,m_byOperationState,sizeof(m_byOperationState));
		tSendOneCard.iOutDeskStation = m_iFirstOutPeople;
		for(int i = 0x00;i < PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
				SendGameData(i,&tSendOneCard,sizeof(tSendOneCard),MDM_GM_GAME_NOTIFY,ASS_SC_SEND_ONE_CARD_FINSH,0);
		}
		SendWatchData(m_bMaxPeople,&tSendOneCard,sizeof(tSendOneCard),MDM_GM_GAME_NOTIFY,ASS_SC_SEND_ONE_CARD_FINSH,0);
	}
	else
	{
		Sleep(1000);
		SetMyTimer(TIME_SEPARATE,m_bySeparateCardTime *1000);			//����ʱ��
		tNoticeSeparetCard NoticeSeparetCard;
		for(int i = 0x00;i < PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
				SendGameData(i,&NoticeSeparetCard,sizeof(NoticeSeparetCard),MDM_GM_GAME_NOTIFY,ASS_SC_NOTIC_SEPARET_CARD,0);
		}
		SendWatchData(m_bMaxPeople,&NoticeSeparetCard,sizeof(NoticeSeparetCard),MDM_GM_GAME_NOTIFY,ASS_SC_NOTIC_SEPARET_CARD,0);
		m_byGameStation = GS_SPARE_CARD_TIME;
	}
}
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
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

			if (iResult == 0)
			{
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

	switch (pNetHead->bAssistantID)
	{
		//�Q����
	case ASS_GM_SUPERCLIENT:
		{
			if (bWatchUser)
				return FALSE;
			if(uSize!=sizeof(ChangeBackStruct))
				return FALSE;
			ChangeBackStruct * pChangeCard=(ChangeBackStruct *)pData;
			return ChangeBackCard(pChangeCard->bDeskStation,pChangeCard->bSourceCard,pChangeCard->bResultCard);
		}
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if (bWatchUser)
				return FALSE;
	
			if (m_bGameStation>=GS_SEND_CARD&&m_bGameStation<GS_WAIT_NEXT)
			{
				return TRUE;
			}
			if(m_bUserReady[bDeskStation])
			{
				return true; 
			}
			m_bUserReady[bDeskStation] = true ;  
			m_bAgree[bDeskStation] = true ;				
			if(m_bGameStation!=GS_WAIT_NEXT)
				m_bGameStation=GS_WAIT_AGREE;
			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
	case ASS_VREB_CHECK:		//�û�����
		{
			if(bWatchUser)
			{
				return false;
			}

			if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_AGREE || m_bGameStation==GS_WAIT_SETGAME)
			{
				return true;
			}
			tagUserProcess *pVerb = (tagUserProcess *) pData;
			UserActiveResult(bDeskStation,pVerb->iVrebType,pVerb->iNote);
			return true;

		}
	case ASS_CS_SEPARAT_CARD:						//���մ��������Ϣ
		{
			Ass_Cs_Separat_Card(pData,uSize);
			return true;
		}
	case ASS_SET_BOBO_DATA:							//����������
		{
			if(m_bGameStation == GS_SEND_CARD)
				Ass_Set_Bobo_Data(pData,uSize);
			return true;
		}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//====================================================================================================
//���� : ���������Ϣ
//���� : void * pData	[in]	��������
//		 UINT uSize		[in]	���ݳ���
//====================================================================================================
bool CServerGameDesk::Ass_Cs_Separat_Card(void * pData, UINT uSize)
{
	T_CS_SeparatCard *tSeparatCard = (T_CS_SeparatCard *)pData;
	if(m_bSeparatCardFale[tSeparatCard->byDeskStation])					//�Ѿ��ֹ�����
	{
		return true;
	}

	memcpy(m_iUserCard[tSeparatCard->byDeskStation],tSeparatCard->bySeparatCard,sizeof(BYTE)*4);
	m_byCardShap[tSeparatCard->byDeskStation][0] = tSeparatCard->byLeftCardShap;
	m_byCardShap[tSeparatCard->byDeskStation][1] = tSeparatCard->byRightCardShap;
	m_bSeparatCardFale[tSeparatCard->byDeskStation] = true;
	T_SC_SeparatSuccess tSeparatSuccess;
	tSeparatSuccess.byDeskStation = tSeparatCard->byDeskStation;
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{
		if(m_pUserInfo[i] == NULL)
			continue;
		if(m_pUserInfo[i])
			SendGameData(i,&tSeparatSuccess,sizeof(tSeparatSuccess),MDM_GM_GAME_NOTIFY,ASS_SC_SEPARAT_CARD,0);
	}
	SendWatchData(m_bMaxPeople,&tSeparatSuccess,sizeof(tSeparatSuccess),MDM_GM_GAME_NOTIFY,ASS_SC_SEPARAT_CARD,0);

	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{

		if(m_byOperationState[i] != TYPE_GIVE_UP)
		{
			if(m_bSeparatCardFale[i] != m_bAgree[i])
			{
				return true;
			}
		}
	}
	KillMyTimer(TIME_SEPARATE);
	SetMyTimer(TIME_SEPARATE,1000);
	return true;
}
//====================================================================================================
// ���� : ������յ�����������
//====================================================================================================
bool CServerGameDesk::Ass_Set_Bobo_Data(void * pData, UINT uSize)
{
	if(sizeof(tSetBoboStruct) != uSize)
	{
		return false;
	}

	tSetBoboStruct *tBobodata = (tSetBoboStruct *)pData;
	tSetBoboStruct tBobo;

	if(m_bAlreadSetBobo[tBobodata->byMyDesk])						//��ֹ�ظ���������
		return true;

	tBobo.byMyDesk = tBobodata->byMyDesk;
	tBobo.i64BoboMoney = tBobodata->i64BoboMoney;
	m_i64Bobo[tBobodata->byMyDesk] = tBobodata->i64BoboMoney;
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{
		if(m_pUserInfo[i] == NULL)
			continue;
			if(m_pUserInfo[i])
				SendGameData(i,&tBobo,sizeof(tBobo),MDM_GM_GAME_NOTIFY,ASS_SET_BOBO_DATA,0);
	}
	SendWatchData(m_bMaxPeople,&tBobo,sizeof(tBobo),MDM_GM_GAME_NOTIFY,ASS_SET_BOBO_DATA,0);

	m_bAlreadSetBobo[tBobodata->byMyDesk] = true;			//�������������

	for(int i = 0x00;i < PLAY_COUNT;i ++)					//����������Ƿ���������
	{
		if(m_bAlreadSetBobo[i] != m_bAgree[i])
		{
			return true;
		}
	}
	
	KillMyTimer(TIME_SEND_CARD);
	SetMyTimer(TIME_SEND_CARD,1000);		
	return true;
}
//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	if(!bWatchUser)
	{
		::memset(m_i64Bobo , 0 , sizeof(m_i64Bobo));
	}
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			//��Ϸ������Ϣ����
			GameStation.iBeginTime=m_iBeginTime;					//��ʼʱ��
			GameStation.byBoBoTime = m_iSendCardTime;				//��������ʱ��
			GameStation.iThinkTime=m_iThinkTime;					//˼��ʱ��
			GameStation.bySeparateCardTime = m_bySeparateCardTime;	//����ʱ��
			GameStation.i64BoBase = m_i64BoBase;					//����
			GameStation.i64SmallBobo = m_i64SmallBobo;				//��С����
			GameStation.bGameStation   = m_bGameStation;	
			memcpy(GameStation.bUserReady,m_bUserReady,sizeof(m_bUserReady));		//׼����ť��Ϣ
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
		{
			GameStation_3 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			//��Ϸ������������
			GameStation.iBeginTime=m_iBeginTime;							//��ʼʱ��
			GameStation.byBoBoTime = m_iSendCardTime;						//��������ʱ��
			GameStation.iThinkTime=m_iThinkTime;							//˼��ʱ��
			GameStation.bySeparateCardTime = m_bySeparateCardTime;			//����ʱ��
			GameStation.i64BoBase = m_i64BoBase;							//����
			GameStation.i64SmallBobo = m_i64SmallBobo;						//��С����
			GameStation.iUpGradePeople=m_iUpGradePeople;					//ׯ��λ��	
			GameStation.bGameStation   = m_bGameStation;					//����
			GameStation.byGameStation = m_byGameStation;					//����С�ֽ�
			GameStation.iTimeRest = m_iSendCardRes - 3;						//�����������е�ʱ��
			::CopyMemory(GameStation.i64Bobo,m_i64Bobo,sizeof(m_i64Bobo));		//������õ�������
			::CopyMemory(GameStation.bPlayer,m_bAgree,sizeof(m_bAgree));		//��ǰ�Ƿ�����;�����
			::CopyMemory(GameStation.bAlreadSetBobo,m_bAlreadSetBobo,sizeof(m_bAlreadSetBobo));				//��ǰ�Ƿ��Ѿ�������������
			::CopyMemory(GameStation.byAlreadySendCardCount,m_bySendCardCount,sizeof(m_bySendCardCount));	//�Ѿ����Ƶ�����
			//�û�������
			::CopyMemory(GameStation.iUserCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));
			//��������
			int iSendSize=sizeof(GameStation);
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			GameStation_4 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;					//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iBeginTime=m_iBeginTime;						//��ʼʱ��
			GameStation.byBoBoTime = m_iSendCardTime;					//��������ʱ��
			GameStation.iThinkTime=m_iThinkTime;						//˼��ʱ��
			GameStation.bySeparateCardTime = m_bySeparateCardTime;		//����ʱ��
			GameStation.i64BoBase = m_i64BoBase;						//����
			GameStation.i64SmallBobo = m_i64SmallBobo;					//��С����
			GameStation.bNtPeople = m_iNtPeople;						//ׯ��λ��
			GameStation.bGameStation   = m_bGameStation;				//�����ֽ�
			GameStation.byGameStation   = m_byGameStation;				//���趯��С�ֽ�
			GameStation.iOutCardPeople=m_iOutCardPeople;				//��ǰ������
			GameStation.iUpGradePeople=m_iUpGradePeople;		
			GameStation.iCurNote = m_iFrontNote;						//��ǰ�µ�ע
			GameStation.iTimeRest = m_iTimerCount - 3;
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				GameStation.bPlayer[i] = m_iUserStation[i] == -1 ? FALSE:TRUE;
			}	
			//Ѻע��ز���
			GameStation.tag.iOutPeople=m_iOutCardPeople;					//��ǰѺע��
			GameStation.tag.iTotalNote=m_iTotalNote;						//��ǰ��ע
			//�û�������
			::CopyMemory(GameStation.iTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));				//�û����µ���ע
			::CopyMemory(GameStation.i64Bobo,m_i64Bobo,sizeof(m_i64Bobo));									//������õ�������
			::CopyMemory(GameStation.bPlayer,m_bAgree,sizeof(m_bAgree));									//��ǰ�Ƿ�����;�����
			::CopyMemory(GameStation.i64NoteMoney,m_i64NoteMoney,sizeof(m_i64NoteMoney));					//�û�����ע������Ϣ
			::CopyMemory(GameStation.iTotalGameNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));			//��������µ�ע
			::CopyMemory(GameStation.byAlreadySendCardCount,m_bySendCardCount,sizeof(m_bySendCardCount));	//�Ѿ����Ƶ�����
			::CopyMemory(GameStation.iUserCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));				//�û������Ƶ�����
			::CopyMemory(GameStation.iUserCard,m_iUserCard,sizeof(m_iUserCard));							//�û�����������
			::CopyMemory(GameStation.byOperationState,m_byOperationState,sizeof(m_byOperationState));		//��ҵĲ���
			::CopyMemory(GameStation.byNoteTime,m_byNoteTime,sizeof(m_byNoteTime));							//�Ѿ���ע�Ĵ���
			//��ǰ���û���ע����
			

			int iSendSize=sizeof(GameStation);

			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_5 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iBeginTime=m_iBeginTime;
			GameStation.iThinkTime=m_iThinkTime;
			GameStation.bySeparateCardTime = m_bySeparateCardTime;
			GameStation.bGameStation   = m_bGameStation;
			GameStation.i64BoBase = m_i64BoBase;						//����
			GameStation.i64SmallBobo = m_i64SmallBobo;					//��С����
			GameStation.iUpGradePeople=m_iUpGradePeople;		
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	}
	return false;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillMyTimer(TIME_SEND_CARD);
	KillMyTimer(TIME_GAME_FINISH);
	KillMyTimer(TIMER_WAIT_NOTE);
	m_iTimerCount = 0;
	for (int i = 0;i < TIMER_WAIT_NOTE;i++) 
	{
		m_iUserStation[i] = -1;
	}
	m_iUpGradePeople=-1;
	m_iFirstOutPeople=-1;
	m_iOutCardPeople=-1;
	m_iLastOutPeople=-1;
	m_iSendCardPos=0;
	m_iFrontNote=0;
	memset(m_bAgree,0,sizeof(m_bAgree));

	memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));

	InitAIData();
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

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
		return false;
	KillTimer(TIME_MY_TIMER);
	KillMyTimer(TIME_GAME_FINISH);
	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	::memset(m_bUserReady , 0 , sizeof(m_bUserReady));
//	::memset(m_i64Bobo , 0 , sizeof(m_i64Bobo));
	::memset(m_bSeparatCardFale,0x00,sizeof(m_bSeparatCardFale));
	::memset(m_byNoteTime,0x01,sizeof(m_byNoteTime));
	::memset(m_byNoteFlag,0x00,sizeof(m_byNoteFlag));
	::memset(m_byOperationState,0xff,sizeof(m_byOperationState));
	::memset(m_i64NoteMoney,0x00,sizeof(m_i64NoteMoney));
	::memset(m_bSpecialCardGiveUp,0x00,sizeof(m_bSpecialCardGiveUp));
	//����״̬
	m_iSendCardPos=0;							//����λ��
	m_iTotalNote = 0;							//��ע	
	m_bSetAllFlag = false;
	m_bFollowFlag = false;
	m_bGameStation=GS_SEND_CARD;								//��Ϸ״̬(��������״̬)
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));		//�û���������
	::memset(m_iNowNote,0,sizeof(m_iNowNote));					//���ҵ�ǰ��ע��
	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));	//���ָ�����ע
	::memset(m_bAlreadSetBobo,0,sizeof(m_bAlreadSetBobo));			//�����������
	for (int i = 0;i < m_bMaxPeople;i++) 
	{
		m_iUserStation[i] = -1;;
		if (m_pUserInfo[i])
		{
			m_iUserStation[i] = TYPE_NORMAL;
		}
	}

	
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{
		if(m_bAgree[i] == true)
		{
			if(m_i64Bobo[i] > m_i64SmallBobo)
			{
				m_bAlreadSetBobo[i] = true;
			}
		}
	}
	//�ַ��˿�
	m_Logic.RandCard(m_iTotalCard,m_iAllCardCount,m_bDeskIndex);

	for(int i=0;i<PLAY_COUNT;i++)									//����ĿǰѺע���
	{
		if(m_pUserInfo[i] != NULL)
		{
			m_iPerJuTotalNote[i] = m_i64BoBase;						
			m_iTotalNote += m_i64BoBase;
			m_i64NoteMoney[i][0] = m_i64BoBase;						//��ע
		}
	}
	//�������ׯ��(��ׯ�����������һ��ׯ��)
	if (m_iNtPeople == 255)
	{
		m_iNtPeople = rand() % PLAY_COUNT;
	}

	if(	m_pUserInfo[m_iNtPeople] == NULL)
	{
		for(int i=0; i<PLAY_COUNT; ++i)
		{
			if(m_pUserInfo[i] == NULL)
				continue;
			m_iNtPeople = i;
			break;
		}
	}
	m_iUpGradePeople = m_iNtPeople;//GetNextNoteStation(m_iNtPeople);
	BeginUpgradeStruct BeginMessage;
	memset(&BeginMessage,0,sizeof(BeginMessage));
	BeginMessage.bNtStation      = m_iUpGradePeople;
	BeginMessage.i64BoBase	=	m_i64BoBase;									//����
	BeginMessage.i64SmallBobo = m_i64SmallBobo;									//��С����
	CopyMemory(BeginMessage.i64BoboData,m_i64Bobo,sizeof(m_i64Bobo));
	CopyMemory(BeginMessage.bPlay,m_bAgree,sizeof(m_bAgree));					//��ǰͬ�����Ϣ
	for (int i=0;i<m_bMaxPeople;i++) 
	{
		if(m_pUserInfo[i])
			SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);
		else
			continue;
	}
	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);
	m_iSendPos=0;
	SetTimer(TIME_MY_TIMER, MY_ELAPSE);
	for(int i = 0x00;i < PLAY_COUNT;i ++)					//����������Ƿ���������
	{
		if(m_bAlreadSetBobo[i] != m_bAgree[i])
		{
			m_iSendCardRes = m_iSendCardTime;
			SetMyTimer(TIME_SEND_CARD,m_iSendCardTime*1000);				//��������ʱ��
			SetMyTimer(TIME_BOBO_RESET_TIME,1000);							//���ڼ���
			m_byGameStation = GS_SET_BOBO_TIME;								//����С�ֽ�

			for(int i = 0x00;i < PLAY_COUNT;i ++)
			{
				if(!m_bAlreadSetBobo[i])
				{
					m_i64Bobo[i] = m_i64SmallBobo;					//Ĭ��Ϊ��С����
				}
			}
			return TRUE;
		}
	}

	KillMyTimer(TIME_SEND_CARD);
	SetMyTimer(TIME_SEND_CARD,1000);	
	return TRUE;
}

//������֤
void CServerGameDesk::SuperUserIDJudge()
{
}

//�û�������Ϸ
BOOL CServerGameDesk::UserSetGame(UserArgeeGame * pGameSet)
{
	return TRUE;
}

//�����˿˸��û�
BOOL CServerGameDesk::SendCard()
{
	memset(m_bAlreadSetBobo,0,sizeof(m_bAlreadSetBobo));			//�����������
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i]&&CanSendCard(i)&&m_iUserStation[i] != -1)//����û��ǿ�,��������������
		{
			m_iSendPos=i;
			break;
		}
	}
	SendCardStruct SendCard;

	memset(&SendCard,0,sizeof(SendCard));
	memcpy(SendCard.i64dNoteData,m_i64NoteMoney,sizeof(m_i64NoteMoney));	//�û����µĳ�����Ϣ
	memcpy(SendCard.byNoteTime,m_byNoteTime,sizeof(m_byNoteTime));
	::memset(m_byNoteTime,0x01,sizeof(m_byNoteTime));
	for (int i=0; i<PLAY_COUNT; i++)
	{
		//if (m_pUserInfo[i] == NULL)
		if(m_bAgree[i] == false)
		{
			continue;
		}
		m_iSendPos = i;
		for (int j=0; j<MAX_CARD_COUNT;j++)
		{
			BYTE bCard = m_iTotalCard[m_iSendPos * 4 + m_iUserCardCount[ m_iSendPos ]];
			m_iUserCard[ m_iSendPos ][m_iUserCardCount[m_iSendPos] ++ ] = bCard;
		}
		SendCard.bCardCount[i] = m_iUserCardCount[ m_iSendPos ];				//Ҫ��ȥ����
		m_i64Bobo[i] = m_i64Bobo[i] - m_i64BoBase;
		SendCard.i64BoboMoney[i] = m_i64Bobo[i];								
	}
	for(int j=0;j<PLAY_COUNT;j++)
	{
		if(m_pUserInfo[j])			
		{
			for (int cd=0; cd<PLAY_COUNT; ++cd)
			{
				memcpy(SendCard.bCard[cd], m_iUserCard[cd], sizeof(SendCard.bCard[0]));
			}
			SendGameData(j,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);
		}
	}
	SendWatchData(m_bMaxPeople,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);

	KillMyTimer(TIME_SEND_CARD);
	SetMyTimer(TIMER_SEND_END,CountPlayer() * 1000);
	m_byGameStation = GS_SEND_CARD_TIME;						//����ʱ��
	m_iAlreadySendCard = 0x02;									//���Ƶ�����
	memset(m_bySendCardCount,0x02,sizeof(m_bySendCardCount));	//�Ѿ����Ƶ�����
	return TRUE;
}

//���ƽ���
BOOL CServerGameDesk::SendCardFinish()
{
	BeginPlayUpGrade();
	return true;
}

//�Ƿ����ӷ���
BOOL CServerGameDesk::CanSendCard(BYTE bDeskStation)
{
	int min=99;
	for(int i=0;i<m_bMaxPeople;i++)
	{
		if(m_iUserCardCount[i]<min&&m_pUserInfo[i])
			min=m_iUserCardCount[i];
	}
	if(m_iUserCardCount[bDeskStation]==min)
		return true;
	return false;
}

//ͳ������Ϸ���
BYTE CServerGameDesk::CountPlayer()
{
	BYTE count=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] && m_bAgree[i] == true)
			count++;
	}
	return count;
}

//��Ϸ��ʼ
BOOL CServerGameDesk::BeginPlayUpGrade()
{
	//��������
	m_bGameStation=GS_PLAY_GAME;
	m_byGameStation = GS_PLAY_GAME_TIME;	//��ʼ����Ϸ״̬
	m_iOutCardPeople=m_iUpGradePeople;		//���ڳ���λ��
	m_iFirstOutPeople=m_iUpGradePeople;		//��һ����λ��
	m_iFrontNote=0;					
	BeginPlayStruct Begin;
	Begin.iOutDeskStation=m_iOutCardPeople;	//
	Begin.bNtPeople = m_iNtPeople;			//ׯ��λ��
	memcpy(Begin.i64dBobo,m_i64Bobo,sizeof(m_i64Bobo));
	for (int i=0;i<m_bMaxPeople;i++)
	{
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}
		SendGameData(i,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	}	
	SendWatchData(m_bMaxPeople,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

	m_iTimerCount = m_iThinkTime + 3;
	SetMyTimer(TIMER_WAIT_NOTE, DEFAULT_WAIT_FREQUENCE);

	return TRUE;
}

//�����û����
BOOL CServerGameDesk::UserActiveResult(BYTE bDeskStation, BYTE iVerbType,__int64 iNoteVal,BYTE bComparedStation)
{
	if(m_iAlreadySendCard == 0x04)
	{
		if(iVerbType == TYPE_GIVE_UP)
		{
			BYTE byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_iUserCard[bDeskStation],m_iAlreadySendCard);
			if(byCardSpecialShap != 0x00)
			{
				m_byOperationState[bDeskStation] = iVerbType;
				m_bSpecialCardGiveUp[bDeskStation] = true;
				SpecialCardGive(bDeskStation);
			}
		}
		return true;
	}
		
	BZWLockForCriticalSection cs(&m_csForIPCMessage);
	switch (iVerbType)
	{
	case TYPE_FOLLOW:  // ��ע
		{	
			m_i64NoteMoney[bDeskStation][m_byNoteTime[bDeskStation]] = iNoteVal;		//��ע��ֵ
			if(m_byNoteFlag[bDeskStation] == 0x00)
			{
				m_byNoteTime[bDeskStation] ++;												//��ע������1
				m_byNoteFlag[bDeskStation] = 0xff;
			}
			m_byOperationState[bDeskStation] = iVerbType;								//��ҵĲ���
			m_i64Bobo[bDeskStation] -= iNoteVal;
			m_iPerJuTotalNote[bDeskStation] += iNoteVal;
			break;
		}
	case TYPE_ALL_NOTE://��
		{
			m_i64NoteMoney[bDeskStation][m_byNoteTime[bDeskStation]] = iNoteVal;		//��ע��ֵ
			if(m_byNoteFlag[bDeskStation] == 0x00)
			{
				m_byNoteTime[bDeskStation] ++;												//��ע������1
				m_byNoteFlag[bDeskStation] = 0xff;
			}
			m_byOperationState[bDeskStation] = iVerbType;								//��ҵĲ���
			m_i64Bobo[bDeskStation] -= iNoteVal;
			m_iPerJuTotalNote[bDeskStation] += iNoteVal;
			if(iNoteVal > m_iFrontNote)
			{
				m_iFrontNote = iNoteVal;
			}
			break;
		}
	case TYPE_ADD:		//��
		{
			m_i64NoteMoney[bDeskStation][m_byNoteTime[bDeskStation]] = iNoteVal;		//��ע��ֵ
			m_iFrontNote = iNoteVal;
			m_i64SaveFront = iNoteVal;
			if(m_byNoteFlag[bDeskStation] == 0x00)
			{
				m_byNoteTime[bDeskStation] ++;												//��ע������1
				m_byNoteFlag[bDeskStation] = 0xff;
			}
			m_byOperationState[bDeskStation] = iVerbType;								//��ҵĲ���
			m_i64Bobo[bDeskStation] -= iNoteVal;
			m_iPerJuTotalNote[bDeskStation] += iNoteVal;
			break;
		}
	case TYPE_PAUSE:		//��
		{
			m_byOperationState[bDeskStation] = iVerbType;
			break;
		}
	case TYPE_GIVE_UP:		//��
		{
			m_byOperationState[bDeskStation] = iVerbType;

			BYTE byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_iUserCard[bDeskStation],m_iAlreadySendCard);
			if(byCardSpecialShap != 0x00)
			{
				m_bSpecialCardGiveUp[bDeskStation] = true;
			}
			break;
		}
	}
	if(m_bSetAllFlag == false)
	{
		if(iVerbType == TYPE_ALL_NOTE)						//������������ϼ�Ϊ�׼�
		{
			if(iNoteVal > m_i64SaveFront)
			{
				m_iFirstOutPeople = bDeskStation;
				m_bSetAllFlag = true;
			}
		}
	}

	if(m_bFollowFlag == false)
	{
		if(iVerbType == TYPE_FOLLOW)						//������������ϼ�Ϊ�׼�
		{
			
			m_bFollowFlag = true;
		}
	}
	if(iVerbType == TYPE_ADD)
	{
		m_iFirstOutPeople = bDeskStation;
		m_bFollowFlag = false;
		m_bSetAllFlag == false;
		m_iFirstOutPeople = bDeskStation;
	}

	BYTE bNextStation = GetNextNoteStation(m_iOutCardPeople);
	//����Ѻע���
	NoteResult noteresult;					
	noteresult.iOutPeople = bDeskStation;								//������ע��
	noteresult.bNextDeskStation = bNextStation;							//��һ�����
	noteresult.byOperatType = iVerbType;								//��������
	noteresult.i64NoteMoney = iNoteVal;									//����ע��Ǯ
	noteresult.i64FrontNote = m_iFrontNote;								//ǰ�����µ�ע
	noteresult.byNoteTime = m_byNoteTime[bDeskStation];					//��������ע�Ĵ���
	noteresult.byFirstOutPeople = m_iFirstOutPeople;					//��һ������֮��
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i])
			SendGameData(i,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
	}
	SendWatchData(m_bMaxPeople,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);	

	if(CheckGameFinish())
	{
		return true;
	}

	m_iTimerCount = m_iThinkTime + 3;
	SetMyTimer(TIMER_WAIT_NOTE,DEFAULT_WAIT_FREQUENCE);

	if(m_iFirstOutPeople == bNextStation)
	{
		m_iFrontNote = 0x00;
		m_iAlreadySendCard ++;
		for(int i = 0x00;i < PLAY_COUNT;i ++)
		{
			m_bySendCardCount[i] = m_iAlreadySendCard;
		}
		bool bAllSetNote = true;
		for(int i = 0x00;i < PLAY_COUNT;i ++)
		{
			if(m_byOperationState[i] != TYPE_ALL_NOTE && m_bAgree[i])					//����������˾ͷ����е���
			{
				bAllSetNote = false;
				break;
			}
		}

		if(bAllSetNote)
		{
			m_iAlreadySendCard = 0x04;
		}

		memset(m_byNoteFlag,0x00,sizeof(m_byNoteFlag));
		T_Notice_SendCard tNoticSendCard;
		tNoticSendCard.bySendCardCount = m_iAlreadySendCard;
		tNoticSendCard.bySendCardFirstDesk = m_iFirstOutPeople;
		m_iOutCardPeople = bNextStation;
		m_bSetAllFlag = false;
		m_bFollowFlag = false;
		for(int i = 0x00;i < PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
				SendGameData(i,&tNoticSendCard,sizeof(tNoticSendCard),MDM_GM_GAME_NOTIFY,ASS_SC_NOTICE_SEND_CARD,0);
		}
		SendWatchData(m_bMaxPeople,&tNoticSendCard,sizeof(tNoticSendCard),MDM_GM_GAME_NOTIFY,ASS_SC_NOTICE_SEND_CARD,0);
		KillMyTimer(TIMER_WAIT_NOTE);
		SetMyTimer(TIME_SEND_ONE_CARD, CountPlayer()*1500);
		return true;
	}
	if(m_iFirstOutPeople == bDeskStation)													//��ǰ�����ǵ�һ��
	{
		if(iVerbType == TYPE_GIVE_UP || iVerbType == TYPE_PAUSE)							//��һ�ҷ�����,���һ�����ƺ���һ��,�����һ����,Ҳ����
		{
			m_iFirstOutPeople = bNextStation;
		}
	}
	m_iOutCardPeople = bNextStation;
	return TRUE;
}
//==========================================================================================
// ���� : ��������������֮��,��������ʮ�������������������
// ���� : int iDesk		[in]		��ǰ���Ӻ�
//==========================================================================================
void CServerGameDesk::SpecialCardGive(int iDesk)
{
	T_SC_SpecialGiveUp tSpecialGiveUp;
	tSpecialGiveUp.byDesk = iDesk;
	tSpecialGiveUp.byOperatType = TYPE_GIVE_UP;
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{	
		if(m_pUserInfo[i])
		{
			SendGameData(i,&tSpecialGiveUp,sizeof(tSpecialGiveUp),MDM_GM_GAME_NOTIFY,ASS_SC_USER_GIVE_UP,0);
		}	
	}
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{

		if(m_byOperationState[i] != TYPE_GIVE_UP)
		{
			if(m_bSeparatCardFale[i] != m_bAgree[i])
			{
				return ;
			}
		}
	}
	KillMyTimer(TIME_SEPARATE);
	SetMyTimer(TIME_SEPARATE,1000);
}
//==========================================================================================
// ���� : �����Ϸ�Ƿ������2
//==========================================================================================
bool CServerGameDesk::CheckGameFinish()
{
	BYTE bNextStation = GetNextNoteStation(m_iOutCardPeople);
	int iPlayCount = CountPlayer();									//����Ϸ������
	int iGiveUp = 0x00;
	int iStop = 0x00;
	for(int i = 0x00;i < PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i])											//������
			continue;	
		if(m_byOperationState[i] == TYPE_GIVE_UP)
			iGiveUp ++;
	}
	if(iGiveUp == (iPlayCount - 1))										//���ֻ��һ���˻�û�з���,�����ʤ
	{
		GameFinish(0,GF_EARLY_END);
		return true;
	}
	for(int i = 0x00;i < PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i])											//������
			continue;	
		if(m_byOperationState[i] == TYPE_PAUSE)						//���е��˶���������
			iStop ++;
	}
	if(iStop == iPlayCount)												//���е��˶�����
	{
		GameFinish(0,GF_SALE);
		return true;
	}
	return false;
}
//===========================================================================================
// ���� : ���������
//===========================================================================================
void CServerGameDesk::FinshCountScore()
{
	memset(&m_iUserCardShap,0xff,sizeof(m_iUserCardShap));
	for(int i = 0x00;i < PLAY_COUNT;i ++)												//ȡ������
	{
		if(m_bAgree[i] == true)				//�п�ʼ,ͬʱû��������
		{
			m_iUserCardShap[i][2] = i;
			if(m_byOperationState[i] != TYPE_GIVE_UP)
			{
				m_iUserCardShap[i][0] = m_Logic.GetCmpCardValue(&m_iUserCard[i][0],0x02);
				m_iUserCardShap[i][1] = m_Logic.GetCmpCardValue(&m_iUserCard[i][2],0x02);
			}	
		}
	}
	//===============================================================================
	//����
	bool bSorted = true;
	int  iLast = PLAY_COUNT - 1;
	int	 iTemp = 0x00;
	do
	{
		bSorted = true;
		for(int i = 0x00;i < iLast;i ++)
		{
			if(m_iUserCardShap[i][1] < m_iUserCardShap[i+1][1])					//������β��С��������
			{
				iTemp = m_iUserCardShap[i][0];									//�����������
				m_iUserCardShap[i][0] = m_iUserCardShap[i + 1][0];
				m_iUserCardShap[i + 1][0] = iTemp;

				iTemp = m_iUserCardShap[i][1];									//�����ұ�����
				m_iUserCardShap[i][1] = m_iUserCardShap[i + 1][1];
				m_iUserCardShap[i + 1][1] = iTemp;

				iTemp = m_iUserCardShap[i][2];									//�������
				m_iUserCardShap[i][2] = m_iUserCardShap[i + 1][2];
				m_iUserCardShap[i + 1][2] = iTemp;
				bSorted = false;
			}
			if(m_iUserCardShap[i][1] == m_iUserCardShap[i+1][1])				//���β�����,�����ͷ
			{
				if(m_iUserCardShap[i][0] < m_iUserCardShap[i+1][0])
				{
					iTemp = m_iUserCardShap[i][0];									//�����������
					m_iUserCardShap[i][0] = m_iUserCardShap[i + 1][0];
					m_iUserCardShap[i + 1][0] = iTemp;
	
					iTemp = m_iUserCardShap[i][1];									//�����ұ�����
					m_iUserCardShap[i][1] = m_iUserCardShap[i + 1][1];
					m_iUserCardShap[i + 1][1] = iTemp;
	
					iTemp = m_iUserCardShap[i][2];									//�������
					m_iUserCardShap[i][2] = m_iUserCardShap[i + 1][2];
					m_iUserCardShap[i + 1][2] = iTemp;
					bSorted = false;
				}
			}
		}
		iLast--;
	}while(!bSorted);

	m_iNtPeople = (m_iUserCardShap[0][2]&0x000000FF);								//��������β���������Ϊ��һ�ֵ�ׯ��
	//========================================================================================
	//����ע��
	for(int i = 0x00;i < PLAY_COUNT;i++)
	{
		if(m_bAgree[i])
		{
			if(m_bSpecialCardGiveUp[i])
			{
				m_i64WinMoney[i] = m_iPerJuTotalNote[i];
			}
			else
			{
				m_i64WinMoney[i] = m_iPerJuTotalNote[i] - m_i64BoBase;
			}
		}
	}

	memcpy(m_iPerJuTotalNote,m_i64WinMoney,sizeof(m_i64WinMoney));					//������µ���ƤƤ��
//======================================================================================================
//�ȼ���β���Ƥ
	int iFirst;
	iFirst = m_iUserCardShap[0][2];	
	int iSecond = 0x00;
	for(int i = 0x00;i < PLAY_COUNT;i++)										//�ü�ԭ�����ѵ�Ǯ
	{
		iSecond = m_iUserCardShap[i][2];
		if(m_bSpecialCardGiveUp[iSecond])										//���������ʮ��������,������Ǯ
		{
			continue;
		}
		if(m_bAgree[iSecond] == false)						
		{
			continue ;
		}
			m_i64WinMoney[iFirst] +=  m_i64BoBase;
	}

	//��һ�Ҵ�
	for(int i = 0x00;i < PLAY_COUNT - 1;i ++)
	{
		int iWinFlag = 0x00;
		int iFirst = 0x00;
		int iSecond = 0x00;
		iFirst = m_iUserCardShap[i][2];									//���Ƶı��A
		if(m_bAgree[iFirst] == false || m_byOperationState[iFirst] == TYPE_GIVE_UP)						//����,��;����,���߷�����
		{
			continue ;
		}
		for(int j = i+1;j < PLAY_COUNT;j ++)
		{
			
			iSecond = m_iUserCardShap[j][2];								//���Ƶı��B
			if(m_bSpecialCardGiveUp[iSecond])								//����ʮ�����������������
			{
				continue;
			}
			if(m_bAgree[iSecond] == false)									//����,��;���벻�ü��
			{
				continue ;
			}
			if(m_byOperationState[iSecond] == TYPE_GIVE_UP)							//�������,�Ͳ��ñȴ�С��
			{
				m_i64WinMoney[iFirst] += m_i64WinMoney[iSecond];
				m_i64WinMoney[iSecond] = 0;
			}
			else
			{
				iWinFlag = CmpTwoValue(m_iUserCardShap[i],m_iUserCardShap[j]);
				if(iWinFlag == 1)												//Ӯ
				{
					if(m_iPerJuTotalNote[iFirst] >= m_i64WinMoney[iSecond])		//��һ����ҵĽ�������ڵڶ�����������µĽ��
					{
						m_i64WinMoney[iFirst] += m_i64WinMoney[iSecond];
						m_i64WinMoney[iSecond] = 0;
					}
					else
					{
						m_i64WinMoney[iFirst] += m_iPerJuTotalNote[iFirst];
						m_i64WinMoney[iSecond] -= m_iPerJuTotalNote[iFirst];
					}
				}
				else if(iWinFlag == -1)													//��
				{
					if(m_iPerJuTotalNote[iSecond] >= m_i64WinMoney[iFirst])
					{
						m_i64WinMoney[iSecond] += m_i64WinMoney[iFirst];
						m_i64WinMoney[iFirst] = 0;
					}
					else
					{
						m_i64WinMoney[iSecond] += m_iPerJuTotalNote[iSecond];
						m_i64WinMoney[iFirst] -= m_iPerJuTotalNote[iSecond];
					}
				}
			}
		}
	}

	for(int i = 0x00;i < PLAY_COUNT; i++)
	{
		if(m_bSpecialCardGiveUp[i])
		{
			continue;
		}
		m_iPerJuTotalNote[i] = m_iPerJuTotalNote[i] + m_i64BoBase;
	}
	//���㾻����
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{
		m_i64NetIncome[i] = m_i64WinMoney[i] - m_iPerJuTotalNote[i];		//������ = ������ - ����ע
	}

}
//=======================================================================================
// ���� : �Ƚ������Ƶ��������ʹ�С
// ���� : int iFirstList[]			[in]		��һ�����͵����
//		  int iSecondList[]			[in]		�ڶ������͵����
//return 0: ��� 1: ��	-1:С
//=======================================================================================
int	 CServerGameDesk::CmpTwoValue(int iFirstList[],int iSecondList[])
{
	if((iFirstList[0] > iSecondList[0]) && (iFirstList[1] > iSecondList[1]))			//��
		return 1;
	if((iFirstList[0] > iSecondList[0]) && (iFirstList[1] == iSecondList[1]))			//��
		return 1;
	if((iFirstList[0] == iSecondList[0]) && (iFirstList[1] > iSecondList[1]))			//��
		return 1;

	if((iFirstList[0] < iSecondList[0]) && (iFirstList[1] < iSecondList[1]))			//С
		return -1;
	if((iFirstList[0] < iSecondList[0]) && (iFirstList[1] == iSecondList[1]))			//С
		return -1;
	if((iFirstList[0] == iSecondList[0]) && (iFirstList[1] < iSecondList[1]))			//С
		return -1;
	return 0x00;																		//���
}
//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	KillTimer(TIME_MY_TIMER);
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{	
			FinshCountScore();
			//�������� 
			m_bGameStation=GS_WAIT_AGREE;
			//��Ϸ����
			T_SC_GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			memcpy(GameEnd.i64TotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));	//�û����µ���ע
			memcpy(GameEnd.i64WinMoney,m_i64WinMoney,sizeof(m_i64WinMoney));			//�û�������
			memcpy(GameEnd.i64NetIncome,m_i64NetIncome,sizeof(m_i64NetIncome));			//�û��ľ�����
			GameEnd.byFinishType = GF_NORMAL;											//��������					

			for(int i = 0x00;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
				{
					m_i64Bobo[i] += GameEnd.i64WinMoney[i];
					if(m_pUserInfo[i]->m_UserData.i64Money < m_i64Bobo[i])
					{
						m_i64Bobo[i] = m_pUserInfo[i]->m_UserData.i64Money;
					}
				}
			}

			//д�����ݿ�				
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(GameEnd.i64NetIncome, temp_cut/*,GameEnd.iChangeMoney,true*/);	//
			__super::RecoderGameInfo(GameEnd.i64NetIncome);
			//��������
			for (int i=0;i<PLAY_COUNT;i++) 
			{
				if (m_pUserInfo[i]!=NULL) 
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			}
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);

			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
		{
			m_bGameStation=GS_WAIT_AGREE;//GS_WAIT_SETGAME;

			T_SC_GameEndStruct tSafeFinish;
			::memset(&tSafeFinish,0,sizeof(tSafeFinish));

			
			memcpy(tSafeFinish.i64TotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));			//�û����µ���ע
			memcpy(tSafeFinish.i64WinMoney,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));			//�û�������
			memset(tSafeFinish.i64NetIncome,0x00,sizeof(m_i64NetIncome));							//�û��ľ�����
			tSafeFinish.byFinishType = GF_SALE;														//��ȫ����
			//��������
			for (int i=0;i<PLAY_COUNT;i++) 
			{
				if (m_pUserInfo[i]!=NULL) 
					SendGameData(i,&tSafeFinish,sizeof(tSafeFinish),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			}
			SendWatchData(m_bMaxPeople,&tSafeFinish,sizeof(tSafeFinish),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);

			bCloseFlag=GFF_FORCE_FINISH;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;

		}
	case GF_EARLY_END:															//��ǰ����
		{
			m_bGameStation=GS_WAIT_AGREE;
			int iWiner = 0x00;
			for(int i = 0x00;i < PLAY_COUNT;i ++)
			{
				if(m_bAgree[i] == true && m_byOperationState[i] != TYPE_GIVE_UP)
				{
					iWiner = i;													//δ��������(ʤ��)
					break;
				}	
			}
			__int64 i64WinMoney = 0x00;
			T_SC_GameEndStruct tEarlEnd;
			memcpy(tEarlEnd.i64TotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));			//�û����µ���ע
			memset(tEarlEnd.i64WinMoney,0x00,sizeof(m_i64WinMoney));							//�û�������
			memset(tEarlEnd.i64NetIncome,0x00,sizeof(m_i64NetIncome));							//�û��ľ�����
			tEarlEnd.byFinishType = GF_EARLY_END;														//��ȫ����
			for(int i = 0x00;i < PLAY_COUNT;i ++)
			{
				if(m_bSpecialCardGiveUp[i] == false)					//���������ʮ�������������,�������Ҳ���Ǯ
				{
					i64WinMoney += m_iPerJuTotalNote[i];
				}
				else
				{
					tEarlEnd.i64WinMoney[i] = m_iPerJuTotalNote[i];
				}
			}
			tEarlEnd.i64WinMoney[iWiner] = i64WinMoney;

			for(int i = 0x00;i < PLAY_COUNT;i ++)
			{
				tEarlEnd.i64NetIncome[i] = tEarlEnd.i64WinMoney[i] - m_iPerJuTotalNote[i];		//������ = ������ - ����ע
			}
			for(int i = 0x00;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
				{
					m_i64Bobo[i] += tEarlEnd.i64WinMoney[i];
					if(m_pUserInfo[i]->m_UserData.i64Money < m_i64Bobo[i])
					{
						m_i64Bobo[i] = m_pUserInfo[i]->m_UserData.i64Money;
					}
				}
			}
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(tEarlEnd.i64NetIncome, temp_cut/*,GameEnd.iChangeMoney,true*/);	//
			__super::RecoderGameInfo(tEarlEnd.i64NetIncome);

			//��������
			for (int i=0;i<PLAY_COUNT;i++) 
			{
				if (m_pUserInfo[i]!=NULL) 
					SendGameData(i,&tEarlEnd,sizeof(tEarlEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			}
			SendWatchData(m_bMaxPeople,&tEarlEnd,sizeof(tEarlEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);

			bCloseFlag=GFF_FORCE_FINISH;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;		
		}
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			//��������
			m_bGameStation=GS_WAIT_AGREE;//GS_WAIT_SETGAME;
			GameCutStruct CutEnd;
			memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			CutEnd.iTurePoint[bDeskStation]=-GetRunPublish()-m_iPerJuTotalNote[bDeskStation];//m_iTotalNote;			//�۳�ǿ�����ȫ����ע

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(CutEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(CutEnd.iChangeMoney);
			for (int i=0;i<PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i] != NULL)
				{
					SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
				}
			}
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);

			bCloseFlag=GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
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
	if(m_bGameStation>=GS_SEND_CARD&&m_bGameStation<GS_WAIT_NEXT && m_iUserStation[bDeskStation] != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
	//return __super::IsPlayGame(bDeskStation);
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	m_bUserReady[bDeskStation] = false ; 
	m_bAgree[bDeskStation] = false ;
//	m_i64Bobo[bDeskStation] = 0x00;
	if (this->IsPlayGame(bDeskStation))
	{
		m_arybCutUser[bDeskStation] = true;
	}	
	else
	{
		m_iNtPeople=255;	
	}

	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//�û�����
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	//m_arybCutUser[bDeskStation] = true;

	//BYTE count = 0;
	//for (int i=0; i<PLAY_COUNT; i++)
	//{
	//	if (m_arybCutUser[i])
	//	{
	//		count++;
	//	}
	//}
	//if (count >= PLAY_COUNT)
	//{
	//	__super::UserNetCut(bDeskStation,pUserInfo);
	//	GameFinish(0,GF_SALE);
	//	return true;
	//}
	return __super::UserNetCut(bDeskStation,pUserInfo);
}

//�Q����
BOOL CServerGameDesk::ChangeBackCard(BYTE bDeskStation,BYTE bSourceCard,BYTE bResultCard)
{
	ChangeBackStruct back;

	if(ReplaceCard(m_iTotalCard,m_iSendCardPos,28,bSourceCard,bResultCard))
	{
		m_iUserCard[bDeskStation][0]=bResultCard;
		back.bExtVal=true;
	}else
		back.bExtVal=false;

	back.bDeskStation=bDeskStation;
	back.bSourceCard=bSourceCard;
	back.bResultCard=bResultCard;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			SendGameData(i,&back,sizeof(back),MDM_GM_GAME_NOTIFY,ASS_GM_SUPERCLIENT,0);
		}

	}
	return true;
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


BYTE CServerGameDesk::GetNextNoteStation(BYTE bCurDeskStation)
{
	BYTE bNextStation = bCurDeskStation;

	int kkk = PLAY_COUNT - 1;   //��ʱ��
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		bNextStation = (bNextStation + kkk) % PLAY_COUNT;
		if(!m_bAgree[bNextStation])										//������
			continue;	
		if (m_byOperationState[bNextStation] != TYPE_GIVE_UP || m_byOperationState[bNextStation] == 0xff)				//�û�û�з���
		{
			if(m_bAgree[bNextStation])
				break;
		}
	}
	if (bNextStation == bCurDeskStation)		//�����һ��������,�͵���255
	{
		return bNextStation;
	}
	return bNextStation;

}
//---------------------------�й���ع���--------------------

void CServerGameDesk::InitAIData()
{
	memset(m_arybCutUser,0,sizeof(m_arybCutUser));

}
bool CServerGameDesk::OnBeginUserNetCutAI(BYTE bDeskStation)  //���ش����йܺ���
{
	if (bDeskStation >= PLAY_COUNT)
	{
		return true;
	}
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return true;
	}
	if (m_arybCutUser[bDeskStation])
	{
		return true;
	}
	m_arybCutUser[bDeskStation] = true;
	if (m_iOutCardPeople == bDeskStation)  //��ǰ���ߵ�Ϊ������
	{
		ExcutNetCutAI();   //ִ�в���
	}
	return true;

}
void CServerGameDesk::ExcutNetCutAI() //ִ���йܲ���
{
	if (m_iOutCardPeople >= PLAY_COUNT)
	{
		return;
	}
	if (m_arybCutUser[m_iOutCardPeople])
	{
		UserActiveResult(m_iOutCardPeople,TYPE_GIVE_UP,0);
	}
}
//�ж�����
BOOL CServerGameDesk::JudgeLoser()
{
	return TRUE;
}
//�ж�ʤ��
BOOL CServerGameDesk::JudgeWiner()
{
	return TRUE;
}

bool CServerGameDesk::SetUserCard()
{
	// ��ȡ�����ļ�
	CString s = CINIFile::GetAppPath ();/////����·��

	TCHAR skin[MAX_NAME_INFO];
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));

	if (!CINIFile::IsFileExist(temp))
	{
		return false;
	}
	CINIFile f(temp);
	char key[MAX_PATH];
	char name[MAX_PATH];
	// Ϊÿ����Ҷ�ȡ�����ļ�����Ƶ�
	for (int i=0; i<PLAY_COUNT; ++i)
	{
		sprintf(key, "Player%d", i);
		for (int j=0; j<3; ++j)
		{
			sprintf(name, "card%d", j);
			int nTmpCard = f.GetKeyVal(key, name, -1);
			if (nTmpCard>0)
			{
				m_iTotalCard[i*3+j] = nTmpCard;
			}
		}
	}
	return true;
}
//
//����
//
BOOL CServerGameDesk::ChangeCard(BYTE bDestStation,BYTE bWinStation)
{
	for(int i = 0; i < 4; i ++)
	{
		BYTE bTemp = m_iTotalCard[bDestStation * 4 + i];
		m_iTotalCard[bDestStation * 4 + i ] = m_iTotalCard[bWinStation * 4 + i];
		m_iTotalCard[bWinStation * 4 + i] = bTemp;
	}
	return TRUE;
}
// ɾ����Ϸ�ڲ���ʱ��
void CServerGameDesk::KillMyTimer(UINT nTimeID)
{
	int nIdx = nTimeID-TIME_SEND_CARD;
	if (nIdx<0 || nIdx>TIMER_COUNT-1)
	{
		return;
	}
	memset(&m_arTimer[nIdx], 0, sizeof(TimerStruct));
}
// ������Ϸ�ڲ���ʱ��
int CServerGameDesk::SetMyTimer(UINT nTimeID, int uElapse)
{
	int nElapse = uElapse/MY_ELAPSE;
	if (nElapse<1)
	{
		nElapse = 1;
	}
	int nIdx = nTimeID-TIME_SEND_CARD;
	if (nIdx<0 || nIdx>TIMER_COUNT-1)
	{
		return -1;
	}
	m_arTimer[nIdx].nId = nTimeID;
	m_arTimer[nIdx].nTriggerCount = nElapse;
	return 0;
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