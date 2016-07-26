#include "StdAfx.h"
#include "ServerManage.h"
#include "writelog.h"
#include <math.h>
#include <algorithm>

//��Ϸ����

bool CServerGameDesk:: m_bHaveKing;
bool CServerGameDesk:: m_bRobnt;
bool CServerGameDesk:: m_bAdddouble;
bool CServerGameDesk:: m_bShowcard;
UINT CServerGameDesk:: m_iPlayCard;
UINT CServerGameDesk:: m_iPlayCount;
UINT CServerGameDesk:: m_iSendCount;
UINT CServerGameDesk:: m_iBackCount;
UINT CServerGameDesk:: m_iUserCount;
BOOL CServerGameDesk::m_bKingCanReplace;				//���ɷ������(DEF=1,0)

void DebugPrintf(const char *p, ...)
{
	char szFilename[256];
	sprintf( szFilename, "E:\\bzw\\DebugInfo\\%dServer.txt", NAME_ID);
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
	ResetGameData();
	InitGameData();
}

//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();									//����·��
	CINIFile f( s +nid +"_s.ini");
	CString key = TEXT("game");
	m_byFreeTime=f.GetKeyVal(key,"freetime",10);							//����ʱ��
	if(m_byFreeTime<10)
	{
		m_byFreeTime=10;
	}
	m_byBetTime=f.GetKeyVal(key,"xiazhutime",20);							//��עʱ��
	if(m_byBetTime<10)
	{
		m_byBetTime=10;
	}
	m_byOpenCardTime=f.GetKeyVal(key,"opencardtime",20);					//����ʱ��
	if(m_byOpenCardTime<15)
	{
		m_byOpenCardTime=15;
	}
	m_iAreaBetLimite=f.GetKeyVal(key,"AreaBetLimite",1000000);				//������ע����
	if(m_iAreaBetLimite<100000)
	{
		m_iAreaBetLimite=100000;
	}
	m_iNtPlayLimite = f.GetKeyVal(key,"NtMaxPlay",15);						//ׯ����ׯ��������
	if(m_iNtPlayLimite<=0)
	{
		m_iNtPlayLimite=1;
	}
	m_iApplyNtLimite=f.GetKeyVal(key,"shangzhuang",5000);					//��ׯ����
	if(m_iApplyNtLimite<1000)
	{
		m_iApplyNtLimite=1000;
	}
	m_iGameEven=f.GetKeyVal(key,"GameEven",72);								//���ٰ�Ϊһ��

	m_byMaxNtPeople=f.GetKeyVal(key,"MaxNtPeople",15);						//��ׯ��������
	if(m_byMaxNtPeople<5)
	{
		m_byMaxNtPeople=5;
	}

	key = "RobotControl";
	//m_iRobotNtCountLimit = f.GetKeyVal(key, "RobotNtCount", 8);								//�����������ׯ����
	m_bIsRobotWinAndLostAutoCtrl = f.GetKeyVal(key,"IsRobotWinAndLostAutoCtrl",0);				//�Ƿ�����������Ӯ�Զ�����
	m_iRobotMaxWinInOne = f.GetKeyVal(key,"RobotMaxWinInOne",0);								//������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�

	//�����˽�������
	m_iRobotWinMoneyA1	= f.GetKeyVal(key,"RobotWinMoneyA1 ",500000);						/*<������ӮǮ����1*/
	m_iRobotWinMoneyA2	= f.GetKeyVal(key,"RobotWinMoneyA2 ",5000000);						/*<������ӮǮ����2*/
	m_iRobotWinMoneyA3	= f.GetKeyVal(key,"RobotWinMoneyA3 ",50000000);						/*<������ӮǮ����3*/

	//�������ڸ�������ӮǮ���ʿ���
	m_iRobotWinLuckyAtA1	= f.GetKeyVal(key,"RobotWinLuckyAtA1 ",90);						/*<������������1ӮǮ�ĸ���*/
	m_iRobotWinLuckyAtA2	= f.GetKeyVal(key,"RobotWinLuckyAtA2 ",60);						/*<��������Ӯ���ƣ�������������2ӮǮ�ĸ���*/
	m_iRobotWinLuckyAtA3	= f.GetKeyVal(key,"RobotWinLuckyAtA3 ",40);						/*<��������Ӯ���ƣ�������������3ӮǮ�ĸ���*/
	m_iRobotWinLuckyAtA4	= f.GetKeyVal(key,"RobotWinLuckyAtA4 ",20);						/*<��������Ӯ���ƣ�������������4ӮǮ�ĸ���*/

	//��¼������ÿ�յ�ӮǮ��Ŀ�͵�ǰ������ӮǮ��Ŀ
	m_iRobotAddMoneyEveryDay	= f.GetKeyVal(key,"RobotAddMoneyEveryDay ",0);				/*<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ*/
	m_iResetRobotHaveWinMoney	= f.GetKeyVal(key,"ReSetRobotHaveWinMoney ",0);				/*<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ*/

	key = "SuperUser";
	// �жϳ����û��ڷ��������Ƿ�����
	int iCount = f.GetKeyVal(key, "SuperUserCount", 0);
	long  lUserID = 0;
	CString strText;
	m_vlSuperID.clear();
	for (int j=0; j<iCount; j++)
	{
		strText.Format("SuperUserID%d", j);
		lUserID = f.GetKeyVal(key, strText, 0);
		//�����г���ID ���浽����������
		m_vlSuperID.push_back(lUserID);
		CString cs;
		cs.Format("lwlog lUserID=%d",lUserID);
		OutputDebugString(cs);
	}
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
//��ʼ����������
bool	CServerGameDesk::InitDeskGameStation()
{
	LoadIni();
	return true;
}
//��ʼ������
void CServerGameDesk::InitGameData()
{
	m_bGameStation	= GS_WAIT_ARGEE;	//�ȴ�ͬ��״̬
	m_bySendCardTime=15;					//����ʱ��
	m_byShowResultTime=5;					//����ʱ��
	m_byShowResult=5;						//������ʾʱ��
	m_bySendSpaceTime=3;					//���Ƽ��ʱ��
	m_bySpeedTime=30;						//�����ٶ�
	m_byShowWinTime=3;						//�ж���Ӯ
	m_bHaveKing = 0;						//�Ƿ�����
	m_iPlayCard = 1;						//ʹ�ü����˿�
	m_iPlayCount = 52;						//�˿�����
	m_iClock=0;//ʱ������
	m_dbyLuzi.clear();//·��˫�˶���	
	m_iNtPlayCount=0;//ׯ����ׯ����
	m_byNtStation = 255;//ׯ��λ��
	m_bGameIsRun = false;//��Ϸ�Ƿ��Ѿ�����
	i64NtGrade=0;//ׯ��ÿһ�����óɼ�
	m_byCurrResult = 0;	//���ֿ������
	m_iPlayerCount=0;//��Ϸ����
	m_byEvent=0;//��Ϸ����
	//�мҳɼ�
	memset(i64PlayerGrade,0,sizeof(i64PlayerGrade));
	i64NtGrade=0;//ׯ�ҳɼ�
	m_bRobotNtCount=0;								//Ŀǰ��������ׯ����
	// ��¼��ǰʱ��
	m_RecordTime = CTime::GetCurrentTime();
}
//------------------------------------------------------------------------------
//�����û���֤
void	CServerGameDesk::SuperUserExamine(BYTE bDeskStation)
{

	SUPERSTATE TSuperUser;
	if (NULL == m_pUserInfo[bDeskStation])
	{
		return ;
	}
	if (IsSuperUser(bDeskStation))
	{
		// ���û�Ϊ�����û�, ֪ͨ�ͻ���
		TSuperUser.byDeskStation = bDeskStation;
		// �����ͻ���
		TSuperUser.bEnable = true;
		SendGameData(bDeskStation, &TSuperUser, sizeof(TSuperUser), MDM_GM_GAME_NOTIFY, ASS_SUPER_USER, 0);
	}
	
}
//---------------------------------------------------------------------------------------------------
//�Ƿ񳬶��û� �����û�Ҫ���ͽ����������
bool	CServerGameDesk::IsSuperUser(BYTE byDeskStation)
{
	if (NULL == m_pUserInfo[byDeskStation])
	{
		return false;
	}
	for(int i=0; i<m_vlSuperID.size(); i++)
	{
		if (NULL != m_pUserInfo[byDeskStation])
		{
			//�Ҵ��˳������
			if (m_pUserInfo[byDeskStation]->m_UserData.dwUserID == m_vlSuperID.at(i))
			{
				return true;
			}
		}
	}
	return false;
}
//�������ǿ����Ϣ
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	
	
	CString debg;
	debg.Format("debug-HandleFrameMessage(server)\n");
	OutputDebugString(debg);
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}

	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:
		{
			OutputDebugString("dwjlog::���ǿ��");
			return true ; 
		}
	}

	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}
//������Ϣ
void  CServerGameDesk::PrintLog()
{
	CString cs;
	/*cs.Format("test m_iRobotNtCountLimit=%d",m_iRobotNtCountLimit);
	OutputDebugString(cs);*/
	cs.Format("test m_bIsRobotWinAndLostAutoCtrl=%d",m_bIsRobotWinAndLostAutoCtrl);
	OutputDebugString(cs);
	cs.Format("test m_iRobotMaxWinInOne=%d",m_iRobotMaxWinInOne);
	OutputDebugString(cs);
	

	cs.Format("test m_iRobotWinMoneyA1=%I64d",m_iRobotWinMoneyA1);
	OutputDebugString(cs);
	cs.Format("test m_iRobotWinMoneyA2=%I64d",m_iRobotWinMoneyA2);
	OutputDebugString(cs);
	cs.Format("test m_iRobotWinMoneyA3=%I64d",m_iRobotWinMoneyA3);
	OutputDebugString(cs);

	cs.Format("test m_iRobotWinLuckyAtA1=%d",m_iRobotWinLuckyAtA1);
	OutputDebugString(cs);
	cs.Format("test m_iRobotWinLuckyAtA2=%d",m_iRobotWinLuckyAtA2);
	OutputDebugString(cs);
	cs.Format("test m_iRobotWinLuckyAtA3=%d",m_iRobotWinLuckyAtA3);
	OutputDebugString(cs);
	cs.Format("test m_iRobotWinLuckyAtA4=%d",m_iRobotWinLuckyAtA4);
	OutputDebugString(cs);
	
	cs.Format("test m_iRobotAddMoneyEveryDay=%I64d",m_iRobotAddMoneyEveryDay);
	OutputDebugString(cs);
	cs.Format("test m_iResetRobotHaveWinMoney=%I64d",m_iResetRobotHaveWinMoney);
	OutputDebugString(cs);
}





//�����м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_FREE_TIME);
	KillTimer(TIME_XIAZHU_TIME);
	KillTimer(TIME_SEND_TIME);
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	return TRUE;
}

/*--------------------------------------------------------------*/
//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	// ����ϵͳ��ʼ����Ϸ��ʼ״̬
	if (__super::GameBegin(bBeginFlag)==false)
	{
		GameFinish(0,GF_SAFE);
		return false;
	}
	CTime time = CTime::GetCurrentTime();
	char szPath[MAX_PATH], szFilename[MAX_PATH];
	// �ж������Ƿ��Ѿ�����һ��(�жϵ�ǰʱ����֮ǰ��¼��ʱ�䲻ͬ���������һ��)
	if (time.GetDay() != m_RecordTime.GetDay()
		|| time.GetMonth() != m_RecordTime.GetMonth()
		|| time.GetYear() != m_RecordTime.GetYear())
	{
		// �µ�һ��
		m_iRobotAddMoneyEveryDay=0;
	}

	//������Ϸ״̬
	m_bGameStation	= GS_BET_STATUS;
	BeginBetMsg betmsg;
	if(255!=m_byNtStation)
	{
		m_byEvent++;
		m_iNtPlayCount++;
	}   
	if(m_byEvent>m_iGameEven)
	{
		//���·��
		m_dbyLuzi.clear();
		//�������¿�ʼ����
		m_byEvent=0;
		betmsg.bLuziClearFlag=TRUE;
	}  

	betmsg.byBetTime=m_byBetTime;		//��עʱ��
	betmsg.byNtStation=m_byNtStation;
	betmsg.byGameStation= m_bGameStation;         ///��ǰ����Ϸ״̬
	betmsg.byNtStation=m_byNtStation;

	betmsg.byNtcount=m_iNtPlayCount;
	betmsg.byEvent=m_byEvent;
	betmsg.i64NtScore=i64PlayerGrade[m_byNtStation];
	list<BYTE>::iterator ite=m_blNtWaitList.begin();
	for(int i=0;ite!=m_blNtWaitList.end();ite++,i++)
	{
		if(i>PLAY_COUNT)
		{
			break;
		}
		betmsg.byNtWaitList[i]=*ite;
	}
	//������ע��Ϣ		
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&betmsg,sizeof(betmsg),MDM_GM_GAME_NOTIFY,ASS_BEGIN_BET,0);
	}
	m_iClock=m_byBetTime;
	//������ע��ʱ��
	SetTimer(TIME_XIAZHU_TIME ,1*1000) ;
	return TRUE;
}
/*--------------------------------------------------------------*/

//����ʱ�� ��Ϸ׼��
void	CServerGameDesk::GamePrepare()
{
	;
}

//��������
void	CServerGameDesk::ResetGameData()
{

	m_iClock=0;//ʱ������
	//�����ע���
	memset(m_i64UserBetCount,0,sizeof(m_i64UserBetCount));
	memset(m_i64AreaBetAllSum,0,sizeof(m_i64AreaBetAllSum));
	memset(m_i64UserBetSum,0,sizeof(m_i64UserBetSum));
	//����ע��
	m_i64SumAllBet = 0;

	//�����������ע����
	//��������ע��Ϣ
	memset(m_RobotAreaBetSum,0,sizeof(m_RobotAreaBetSum));	//��������ÿ���������ע��
	memset(m_RobotAreaAllSum,0,sizeof(m_RobotAreaAllSum));	//ÿ�������������ע����
	memset(m_RobotBetSum,0,sizeof(m_RobotBetSum));			//ÿ�������˵���ע����
	m_RobotAllSum=0;										//��������ע����

	//������ֵ÷����
	memset(i64ChangeMoney,0,sizeof(i64ChangeMoney));
	memset(i64ReturnMoney,0,sizeof(i64ReturnMoney));
	memset(i64PlayCoin,0,sizeof(i64PlayCoin)); 

	m_bIsSuperControl=FALSE;		//�Ƿ��ǳ����û����ƽ��
	m_bySuperReslut=255;

}

//��ȡ���ʣ�����Ǯ
__int64		CServerGameDesk::GetUserRemaindMoney(BYTE byDeskStation)
{
	if (NULL != m_pUserInfo[byDeskStation])
	{
		return m_pUserInfo[byDeskStation]->m_UserData.i64Money - GetUserBetCount(byDeskStation);
	}
	return 0;
}

//��ȡ�����ע�ܶ�
__int64		CServerGameDesk::GetUserBetCount(BYTE byDeskStation)
{
	__int64 iUserBetCount = 0;
	for (int i=0; i<BET_ARES;i++)
	{
		iUserBetCount += m_i64UserBetCount[byDeskStation][i];
	}
	return iUserBetCount;
}
//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIME_FREE_TIME://����ʱ��
		{
			m_iClock--;
			if(0==m_iClock)
			{
				//ɾ�����ж�ʱ��
				KillTimer(TIME_FREE_TIME);
				sendBetMsg();
			}
			return true;
		}
	case TIME_XIAZHU_TIME://��עʱ��
		{
			m_iClock--;
			if(0==m_iClock)
			{
				//ɾ����ע��ʱ��
				KillTimer(TIME_XIAZHU_TIME);
				SendAllCard();
			}
			return true;
		}
	case TIME_SEND_TIME://����ʱ��
		{
			m_iClock--;
			if(5==m_iClock)
			{
				sendResult();//���ͽ�����Ϣ
			}
			if(0==m_iClock)
			{
				//ɾ�����ƶ�ʱ��
				KillTimer(TIME_SEND_TIME);
				GameFinish(0,GF_NORMAL);//��������
				//������߻���ǿ����ҵĳɼ�
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(NULL==m_pUserInfo[i])
					{
						i64PlayerGrade[i]=0;
					}
				}

				//����ׯ��
				if(NULL==m_pUserInfo[m_byNtStation]|| (m_pUserInfo[m_byNtStation]->m_UserData.i64Money < m_iApplyNtLimite ))
				{
					m_byNtStation=255;
				}

				//����ׯ���б�
				for(list<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();)
				{
					if((NULL==m_pUserInfo[*ite]) || (m_pUserInfo[*ite]->m_UserData.i64Money < m_iApplyNtLimite ))
					{
						m_blNtWaitList.erase(ite++);
					}
					else
					{
						ite++;
					}
				}
				StartGame();
			}
			return true;
		}
	}
	return __super::OnTimer(uTimerID);
}
/*
���ܣ���ͻ��˷�����ע��Ϣ
��������
return����
*/
void CServerGameDesk::sendBetMsg()
{
	//��Ϸ��ʼ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL == m_pUserInfo[i])//��Ҳ����ھ�
		{
			continue;
		}
		m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
	}	
	GameBegin(ALL_ARGEE);//��Ϸ��ʼ
}
/*
���ܣ��������Ʒ��͸������
��������һ���������������飻�ڶ����������Ƿ��Ʊ�־
����ֵ��true:���Ƴɹ���false������ʧ�ܣ�
*/
BOOL CServerGameDesk::SendAllCard()
{
	//������Ϸ״̬
	m_bGameStation	= GS_SEND_STATUS;
	BeginSendCardMsg SendCard;
	SendCard.byGameStation= m_bGameStation;//��ǰ����Ϸ״̬
	SendCard.byCardCount=2;//���Ƹ���
	
	list<BYTE>::iterator ite=m_blNtWaitList.begin();
	for(int i=0;ite!=m_blNtWaitList.end();ite++,i++)
	{
		if(i>PLAY_COUNT)
		{
			break;
		}
		SendCard.byNtWaitList[i]=*ite;
	}
	if(m_bIsRobotWinAndLostAutoCtrl && FALSE==m_bIsSuperControl)//����ǻ������Զ����ƣ����ǳ��˿���
	{
		SendCard.byWinArea=RobotAutoControl();
	}
	else if(m_bIsSuperControl)//�����û�����
	{
		SendCard.byWinArea=m_bySuperReslut;
	}
	else//������
	{
		srand(GetTickCount());
		SendCard.byWinArea=rand()%BET_ARES;
	}
	if(SendCard.byWinArea<0 || SendCard.byWinArea>2)
	{
		srand(GetTickCount());
		SendCard.byWinArea=rand()%BET_ARES;
	}
	ExchangePoker(iCardArray,SendCard.byWinArea);
	m_byWinArea=SendCard.byWinArea;
	//���Ʒ�������
	for(int i=0;i<m_iPlayCount;i++)
	{
		SendCard.byCardStyle[i]=iCardArray[i];
	}
	//����÷�
	accountPlayCoin(SendCard.byWinArea,i64PlayCoin);
	
	//·�Ӽ�¼���
	//ֻ��¼52��·��
	if (m_dbyLuzi.size() >=52)
	{
		m_dbyLuzi.pop_front();
		m_dbyLuzi.push_back(SendCard.byWinArea);
	}
	else
	{
		m_dbyLuzi.push_back(SendCard.byWinArea);
	}
	//����·��
	for(int i=0;i<m_dbyLuzi.size();i++)
	{
		SendCard.byLuZi[i]=m_dbyLuzi.at(i);
	}
	//���ͷ�����Ϣ��
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_GAME_SEND,0);
	}
	m_iClock=m_byOpenCardTime;
	//�������ƶ�ʱ��
	SetTimer(TIME_SEND_TIME,1*1000);
	return TRUE;
}
//�������Զ�����
BYTE CServerGameDesk::RobotAutoControl()
{
	BYTE byWinArea=255;
	BOOL RobotIsWin=FALSE;
	srand((unsigned)GetTickCount());
	int iResult = rand()%100;
	if(m_iResetRobotHaveWinMoney<=0)//������ӮǮ��Ϊ�������㣬�����˱�Ӯ
	{
		RobotIsWin=TRUE;
	}
	else if(m_iResetRobotHaveWinMoney>0 && m_iResetRobotHaveWinMoney<=m_iRobotWinMoneyA1)
	{
		if(iResult<=m_iRobotWinLuckyAtA1)
		{
			RobotIsWin=TRUE;
		}
	}
	else if(m_iResetRobotHaveWinMoney>m_iRobotWinMoneyA1 && m_iResetRobotHaveWinMoney<=m_iRobotWinMoneyA2)
	{
		if(iResult<=m_iRobotWinLuckyAtA2)
		{
			RobotIsWin=TRUE;
		}
	}
	else if(m_iResetRobotHaveWinMoney>m_iRobotWinMoneyA2 && m_iResetRobotHaveWinMoney<=m_iRobotWinMoneyA3)
	{
		if(iResult<=m_iRobotWinLuckyAtA3)
		{
			RobotIsWin=TRUE;
		}
	}
	else if(m_iResetRobotHaveWinMoney>m_iRobotWinMoneyA3)
	{
		if(iResult<=m_iRobotWinLuckyAtA4)
		{
			RobotIsWin=TRUE;
		}
	}
	if(RobotIsWin)//������ӮǮ
	{
		byWinArea=RobotAutoSelectWinArea();
	}
	else//��������Ǯ
	{
		srand(GetTickCount());
		byWinArea=rand()%BET_ARES;
	}
	return byWinArea;
}
//�������Զ�����ѡ����ʵ�ӮǮ����
BYTE CServerGameDesk::RobotAutoSelectWinArea()
{
	BYTE byWinArea=255;
	__int64 i64RobotGrade[3];//������������˵ĵ÷�
	vector<__int64> vWinArea;//ӮǮ����
	vWinArea.clear();
	memset(i64RobotGrade,0,sizeof(i64RobotGrade));
	if(m_pUserInfo[m_byNtStation])
	{
		if(m_pUserInfo[m_byNtStation]->m_UserData.isVirtual)//�����������ׯ���������ҵĵ÷�
		{
			CString cs;
			for(int i=0;i<BET_ARES;i++)
			{
				i64RobotGrade[i]=TempAccountRobotAndRealPlayer(i,1);
				cs.Format("lwlog ��������ׯ�� ����i=%d �÷�=%I64d ",i,i64RobotGrade[i]);
				OutputDebugString(cs);
			}
			//��Ϊ������ҪӮǮ����ѡ����ڵ����������
			for(int i=0;i<BET_ARES;i++)
			{
				if(i64RobotGrade[i]>=0)
				{
					vWinArea.push_back(i);
				}
			}
			if(!vWinArea.empty())
			{
				byWinArea=vWinArea.at(rand()%vWinArea.size());
			}
		}
		else//�����˲���ׯ�Ҽ��������ÿ������ĵ÷�
		{
			CString cs;
			for(int i=0;i<BET_ARES;i++)
			{
				i64RobotGrade[i]=TempAccountRobotAndRealPlayer(i,0);
				cs.Format("lwlog �����˲���ׯ�� ����i=%d �÷�=%I64d ",i,i64RobotGrade[i]);
				OutputDebugString(cs);
			}
			//��Ϊ������ҪӮǮ����ѡ����ڵ����������
			for(int i=0;i<BET_ARES;i++)
			{
				if(i64RobotGrade[i]>=0)
				{
					vWinArea.push_back(i);
				}
			}
			if(!vWinArea.empty())
			{
				byWinArea=vWinArea.at(rand()%vWinArea.size());
			}
		}
	}
	if(byWinArea>=0 && byWinArea<=2)//���ؼ�¼
	{
		CString nid;
		nid.Format("%d",NAME_ID);
		CString s = CINIFile::GetAppPath ();									//����·��
		CINIFile f( s +nid +"_s.ini");
		CString key = TEXT("RobotControl");
		__int64 i64RobotHaveWinTempMoney=i64RobotGrade[byWinArea];
		m_iResetRobotHaveWinMoney= f.GetKeyVal(key,"ReSetRobotHaveWinMoney ",0);
		m_iResetRobotHaveWinMoney+=i64RobotHaveWinTempMoney;
		s.Format("%I64d",m_iResetRobotHaveWinMoney);
		f.SetKeyValString(key,"ReSetRobotHaveWinMoney ",s);//�������������Ľ��д�������ļ���
	}
	return byWinArea;
}

//��ʱ���㿪�����������˺���Ҹ������ܵ÷�(para=0 �����˵÷�,para=1 ��ҵ÷�)
__int64 CServerGameDesk::TempAccountRobotAndRealPlayer(BYTE byWinArea,BYTE para)
{
	//OutputDebugString("test ��ʱ���㿪�����������˺���Ҹ������ܵ÷�");
	__int64 money=0;
	__int64 i64TempCoin[PLAY_COUNT];//��ʱ�÷�
	memset(i64TempCoin,0,sizeof(i64TempCoin));
	//OutputDebugString("test _____________________1");
	/*CString cs;
	cs.Format("test byWinArea=%d para=%d",byWinArea,para);
	OutputDebugString(cs);*/
	if(0==para)//�����˵÷�
	{
		
		switch(byWinArea)
		{
		case 0:
			{
				//Ӯ�ļ�ȥ���
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i])
					{
						if(i==m_byNtStation || (!m_pUserInfo[i]->m_UserData.isVirtual))
						{
							
							continue;
						}
						
						i64TempCoin[i]=m_i64UserBetCount[i][0]-(m_i64UserBetCount[i][1]+m_i64UserBetCount[i][2]);
					}
				}
				
				break;
			}
		case 1:
			{
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i])
					{
						//Ӯ�ļ�ȥ���
						if(i==m_byNtStation || !m_pUserInfo[i]->m_UserData.isVirtual)
						{
							continue;
						}

						i64TempCoin[i]=m_i64UserBetCount[i][1]-(m_i64UserBetCount[i][0]+m_i64UserBetCount[i][2]);
					}	
				}
				break;
			}
		case 2:
			{
				for(int i=0;i<PLAY_COUNT;i++)
				{
					//Ӯ�ļ�ȥ���
					if(m_pUserInfo[i])
					{
						if(i==m_byNtStation || !m_pUserInfo[i]->m_UserData.isVirtual)
						{
							continue;
						}

						//��������������һ����ע���
						i64TempCoin[i]=(m_i64UserBetCount[i][2]*8)-((m_i64UserBetCount[i][0]+m_i64UserBetCount[i][1])/2);
					}
				}
				break;
			}
		default:
			{
				break;
			}
		}
		
		//�����ܵ÷�
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
			{
				if(i==m_byNtStation || !m_pUserInfo[i]->m_UserData.isVirtual)
				{
					continue;
				}
				money+=i64TempCoin[i];
			}
			
		}
		
	}
	if(1==para)//��ҵ÷�
	{
		
		switch(byWinArea)
		{
		case 0:
			{
				//Ӯ�ļ�ȥ���
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i])
					{
						if(i==m_byNtStation || m_pUserInfo[i]->m_UserData.isVirtual)
						{
							continue;
						}

						i64TempCoin[i]=m_i64UserBetCount[i][0]-(m_i64UserBetCount[i][1]+m_i64UserBetCount[i][2]);
					}
					
				}
				break;
			}
		case 1:
			{
				//Ӯ�ļ�ȥ���
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i])
					{
						if(i==m_byNtStation || m_pUserInfo[i]->m_UserData.isVirtual)
						{
							continue;
						}
						i64TempCoin[i]=m_i64UserBetCount[i][1]-(m_i64UserBetCount[i][0]+m_i64UserBetCount[i][2]);
					}
					
				}
				break;
			}
		case 2:
			{
				//Ӯ�ļ�ȥ���
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i])
					{
						if(i==m_byNtStation || m_pUserInfo[i]->m_UserData.isVirtual)
						{
							continue;
						}

						//��������������һ����ע���
						i64TempCoin[i]=(m_i64UserBetCount[i][2]*8)-((m_i64UserBetCount[i][0]+m_i64UserBetCount[i][1])/2);
					}
					
				}
				break;
			}
		default:
			{
				break;
			}
		}
		//�����ܵ÷�
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])
			{
				if(i==m_byNtStation || m_pUserInfo[i]->m_UserData.isVirtual)
				{
					continue;
				}
				money+=i64TempCoin[i];
			}
			
		}
		money=-money;
	} 

	/*cs.Format("test money=%I64d",money);
	OutputDebugString(cs);*/
	return money;
}
//�����˿�(para=2,��������ȣ�1����һ����С�ڵڶ����ƣ�0����һ���ƴ��ڵڶ�����)
void CServerGameDesk::ExchangePoker(BYTE card[],int para)
{
	//BYTE m_CardArray[52] =
	//{
	//	0x0D,0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,  //���� A-k
	//	0x1D,0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,  //÷�� A-k
	//	0x2D,0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,  //���� A-k 
	//	0x3D,0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C,   //���� A-k
	//	//0x4E, 0x4F //С�����
	//};
	BYTE b_CardArray[13][4]=
	{
		{0x0D,0x1D,0x2D,0x3D},{0x01,0x11,0x21,0x31},{0x02,0x12,0x22,0x32},{0x03,0x13,0x23,0x33},
		{0x04,0x14,0x24,0x34},{0x05,0x15,0x25,0x35},{0x06,0x16,0x26,0x36},{0x07,0x17,0x27,0x37},
		{0x08,0x18,0x28,0x38},{0x09,0x19,0x29,0x39},{0x0A,0x1A,0x2A,0x3A},{0x0B,0x1B,0x2B,0x3B},
		{0x0C,0x1C,0x2C,0x3C}
	};
	
	//srand������������������ĳ�ʼ������
	
	BYTE bIndexStand_x=0,bIndexStand_y=0,bIndex_x=0,bIndex_y=0;
	CString cs;
	cs.Format("test::para=%d",para);
	OutputDebugString(cs);

	srand(GetTickCount());
	
	if(1==para)
	{
		//������һ����
		bIndexStand_x=rand()%9;
		bIndexStand_y=rand()%4;
		card[0]=b_CardArray[bIndexStand_x][bIndexStand_y];

		//�����ڶ�����
		bIndex_x=(rand()%(4+(8-bIndexStand_x)))+(bIndexStand_x+1);
		bIndex_y=rand()%4;
	}
	else if(2==para)
	{
		//������һ����
		bIndexStand_x=rand()%13;
		bIndexStand_y=rand()%4;
		card[0]=b_CardArray[bIndexStand_x][bIndexStand_y];

		//�����ڶ�����
		bIndex_x=bIndexStand_x;
		bIndex_y=rand()%4;
	}
	else if(0==para)
	{
		//������һ����
		bIndexStand_x=((rand()%12)+1);
		bIndexStand_y=rand()%4;
		card[0]=b_CardArray[bIndexStand_x][bIndexStand_y];

		//�����ڶ�����
		if(1==bIndexStand_x)
		{
			bIndex_x=0;
		}
		else
		{
			bIndex_x=rand()%bIndexStand_x;
		}
		bIndex_y=rand()%4;
	}

	card[1]=b_CardArray[bIndex_x][bIndex_y];
	
	cs.Format("test::bIndexStand_x=%d::bIndex_x=%d",bIndexStand_x,bIndex_x);
	OutputDebugString(cs);
}
//��������˫�������С
void CServerGameDesk::trandformNum(BYTE card[])
{
	switch(iCardArray[0])
	{
	case 0x0D:
	case 0x1D:
	case 0x2D:
	case 0x3D:
		{
			card[0]=1;break;
		}
	case 0x01:
	case 0x11:
	case 0x21:
	case 0x31:
		{
			card[0]=2;break;
		}
	case 0x02:
	case 0x12:
	case 0x22:
	case 0x32:
		{
			card[0]=3;break;
		}
	case 0x03:
	case 0x13:
	case 0x23:
	case 0x33:
		{
			card[0]=4;break;
		}
	case 0x04:
	case 0x14:
	case 0x24:
	case 0x34:
		{
			card[0]=5;break;
		}
	case 0x05:
	case 0x15:
	case 0x25:
	case 0x35:
		{
			card[0]=6;break;
		}
	case 0x06:
	case 0x16:
	case 0x26:
	case 0x36:
		{
			card[0]=7;break;
		}
	case 0x07:
	case 0x17:
	case 0x27:
	case 0x37:
		{
			card[0]=8;break;
		}
	case 0x08:
	case 0x18:
	case 0x28:
	case 0x38:
		{
			card[0]=9;break;
		}
	case 0x09:
	case 0x19:
	case 0x29:
	case 0x39:
		{
			card[0]=10;break;
		}
	case 0x0A:
	case 0x1A:
	case 0x2A:
	case 0x3A:
		{
			card[0]=10;break;
		}
	case 0x0B:
	case 0x1B:
	case 0x2B:
	case 0x3B:
		{
			card[0]=10;break;
		}
	case 0x0C:
	case 0x1C:
	case 0x2C:
	case 0x3C:
		{
			card[0]=10;break;
		}

	}
	switch(iCardArray[1])
	{
	case 0x0D:
	case 0x1D:
	case 0x2D:
	case 0x3D:
		{
			card[1]=1;break;
		}
	case 0x01:
	case 0x11:
	case 0x21:
	case 0x31:
		{
			card[1]=2;break;
		}
	case 0x02:
	case 0x12:
	case 0x22:
	case 0x32:
		{
			card[1]=3;break;
		}
	case 0x03:
	case 0x13:
	case 0x23:
	case 0x33:
		{
			card[1]=4;break;
		}
	case 0x04:
	case 0x14:
	case 0x24:
	case 0x34:
		{
			card[1]=5;break;
		}
	case 0x05:
	case 0x15:
	case 0x25:
	case 0x35:
		{
			card[1]=6;break;
		}
	case 0x06:
	case 0x16:
	case 0x26:
	case 0x36:
		{
			card[1]=7;break;
		}
	case 0x07:
	case 0x17:
	case 0x27:
	case 0x37:
		{
			card[1]=8;break;
		}
	case 0x08:
	case 0x18:
	case 0x28:
	case 0x38:
		{
			card[1]=9;break;
		}
	case 0x09:
	case 0x19:
	case 0x29:
	case 0x39:
		{
			card[1]=10;break;
		}
	case 0x0A:
	case 0x1A:
	case 0x2A:
	case 0x3A:
		{
			card[1]=10;break;
		}
	case 0x0B:
	case 0x1B:
	case 0x2B:
	case 0x3B:
		{
			card[1]=10;break;
		}
	case 0x0C:
	case 0x1C:
	case 0x2C:
	case 0x3C:
		{
			card[1]=10;break;
		}
	}
}
/*
����:ͳ����ҽ��
������whoWin:�н���������i64PlayCoin����ׯ���������������Ӯ��ң�i64NtCoin��ׯ����Ӯ���
����ֵ��true,ͳ�Ƴɹ�.
*/

BOOL CServerGameDesk::accountPlayCoin(BYTE winArea,__int64 i64PlayCoin[])
{
	//�ȼ����м�
	switch(winArea)
	{
		case 0:
			{
				//Ӯ�ļ�ȥ���
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(i==m_byNtStation)
					{
						continue;
					}
					
					i64PlayCoin[i]=m_i64UserBetCount[i][0]-(m_i64UserBetCount[i][1]+m_i64UserBetCount[i][2]);
					i64ReturnMoney[i]=m_i64UserBetCount[i][0];
				}
				break;
			}
		case 1:
			{
				//Ӯ�ļ�ȥ���
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(i==m_byNtStation)
					{
						continue;
					}
					
					i64PlayCoin[i]=m_i64UserBetCount[i][1]-(m_i64UserBetCount[i][0]+m_i64UserBetCount[i][2]);
					i64ReturnMoney[i]=m_i64UserBetCount[i][1];
				}
				break;
			}
		case 2:
			{
				//Ӯ�ļ�ȥ���
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(i==m_byNtStation)
					{
						continue;
					}
					
					//��������������һ����ע���
					i64PlayCoin[i]=(m_i64UserBetCount[i][2]*8)-((m_i64UserBetCount[i][0]+m_i64UserBetCount[i][1])/2);
					i64ReturnMoney[i]=(m_i64UserBetCount[i][2]*8);
				}
				break;
			}
	}
	//����ׯ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(i==m_byNtStation)
		{
			continue;
		}
		i64PlayCoin[m_byNtStation]-=i64PlayCoin[i];
	}
	
	//������ҳɼ�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		i64PlayerGrade[i]+=i64PlayCoin[i];
	}

	return TRUE;
}
//���ͽ�����Ϣ
void CServerGameDesk::sendResult()
{
	BeginShowResult beginShowResult;
	bool temp_cut[PLAY_COUNT];
	memset(temp_cut,0,sizeof(temp_cut));
	//�������ݿ�
	ChangeUserPointint64(i64PlayCoin, temp_cut);
	//�����ݿ�ȡ����˰�Ժ�ĵ÷�
	__super::RecoderGameInfo(i64ChangeMoney);
	BeginShowResultData ScoreData;
	//�Ȱ����ݷ���ȥ
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		for(int j=0;j<PLAY_COUNT;j++)
		{
			if (NULL==m_pUserInfo[j])
			{
				continue;
			}
			ScoreData.bDeskStation=j;
			ScoreData.i64ChangeMoney=i64ChangeMoney[j];
			ScoreData.i64ReturnMoney=i64ReturnMoney[j];
			ScoreData.i64PlayerGrade=i64PlayerGrade[j];
			//������Ϸ��Ϣ
			SendGameData(i,&ScoreData,sizeof(ScoreData),MDM_GM_GAME_NOTIFY,ASS_GAME_DATA,0);
		}
		
	}

	//���ͽ�����Ϣ
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		if(m_pUserInfo[i])
		{
			CString cs;
			cs.Format("test ���%d���=%I64d",i,m_pUserInfo[i]->m_UserData.i64Money);
			OutputDebugString(cs);
		}
		//������Ϸ��Ϣ
		SendGameData(i,&beginShowResult,sizeof(beginShowResult),MDM_GM_GAME_NOTIFY,ASS_GAME_ACCOUNT,0);
	}
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{

			if (bWatchUser)
			{
				return FALSE;
			}

			if(NULL != m_pUserInfo[bDeskStation])
			{
				m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;  //�����Ϊͬ��״̬
			}

			__super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);

			return true;

		}
	case ASS_GM_STATUS_GAME://��ȡ��Ϸ״̬
		{
			break;
		}
	case ASS_USER_BET:	//�����ע
		{
			if(uSize != sizeof(UserBetMsg))
			{
				return true ; 
			}
			DealUserBet(pData);
			return true;
		}
	case ASS_APPLY_NT:	//���������ׯ
		{
			if(uSize != sizeof(ApplyNtMsg))
			{
				return true ; 
			}
			//�������������ׯ��Ϣ
			DealUserApplyNt(pData);
			return true;
		}
	case ASS_APPLY_LEAVE:	//������ׯ
		{
			if(uSize != sizeof(ApplyLiveNtMsg))
			{
				return true ; 
			}
			DealUserApplyLeaveNt(pData);
			return true;
		}
	case ASS_SUPER_RESULT://�����û���Ϣ
		{
			if(uSize != sizeof(SuperUserControlReslut))
			{
				return true ; 
			}
			DealSuperUserMessage(pData);
			return true;
		}
	
	default:
		{
			break;
		}
	}

	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}
//�������û����ƽ��
void	CServerGameDesk::DealSuperUserMessage(void *pBuffer)
{
	SuperUserControlReslut *SuperUserMessage = (SuperUserControlReslut *)pBuffer;
	if(NULL == SuperUserMessage)
	{
		return;
	}
	OutputDebugString("test �յ������û����ƽ��");
	m_bIsSuperControl=TRUE;
	m_bySuperReslut=SuperUserMessage->bySuperReslut;
}

//�������������ׯ��Ϣ
void	CServerGameDesk::DealUserApplyNt(void *pBuffer)
{

	ApplyNtMsg *pApplyNt = (ApplyNtMsg *)pBuffer;
	if(NULL == pApplyNt)
	{
		return;
	}
	ApplyNtResultMsg TApplyNtResult;
	TApplyNtResult.byDeskStation=pApplyNt->byDeskStation;
	

	if(m_blNtWaitList.size()>=m_byMaxNtPeople)//�����ׯ������20
	{
		return;
	}
	//�Ƿ�ׯ��||�Ѿ�����ׯ�б���
	if(pApplyNt->byDeskStation == m_byNtStation || IsInWaitNtList(pApplyNt->byDeskStation))
	{
		return;
	}
	//�Ƿ����㹻��Ǯ��ׯ
	if (GetUserRemaindMoney(pApplyNt->byDeskStation) < m_iApplyNtLimite )
	{
		TApplyNtResult.bApplay=true;
		SendGameData(TApplyNtResult.byDeskStation,&TApplyNtResult,sizeof(TApplyNtResult),MDM_GM_GAME_NOTIFY,ASS_APPLY_RESULT,0);
		return;
	}
	//if(m_pUserInfo[pApplyNt->byDeskStation])
	//{
	//	OutputDebugString("test ������������ׯ");
	//	if(m_pUserInfo[pApplyNt->byDeskStation]->m_UserData.isVirtual)//��������ׯ�������ô��ڻ�������ׯ��������
	//	{
	//		if(m_bRobotNtCount>=m_iRobotNtCountLimit)
	//		{
	//			return;
	//		}
	//	}
	//}
	
	//ׯ�Ҳ�����
	if(255==m_byNtStation )
	{
		m_byNtStation=pApplyNt->byDeskStation;//��סׯ��λ��
		
		i64PlayerGrade[m_byNtStation]=0;

		/*if((GS_FREE_STATUS==m_bGameStation) && m_bGameIsRun)
		{
			m_iNtPlayCount=1;
			m_byEvent++;
		}*/
		
		TApplyNtResult.bNtFlag=TRUE;
	}
	else
	{
		if(m_pUserInfo[pApplyNt->byDeskStation])
		{
			if(m_pUserInfo[pApplyNt->byDeskStation]->m_UserData.isVirtual)//�����˷�����ׯ�б���
			{
				m_bRobotNtCount++;
			}
		}
		m_blNtWaitList.push_back(pApplyNt->byDeskStation);
	}
	int i=0;
	for(list<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
	{
		TApplyNtResult.byWaitNtList[i]=*ite;
		i++;
	}
	TApplyNtResult.byNtStation = m_byNtStation;

	TApplyNtResult.byNtcount=m_iNtPlayCount;//ׯ����ׯ����
	TApplyNtResult.i64NtScore=i64PlayerGrade[m_byNtStation];//ׯ�ҳɼ�
	TApplyNtResult.bGameStation=m_bGameStation;
	TApplyNtResult.bEvent=m_byEvent;//��Ϸ����
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&TApplyNtResult,sizeof(TApplyNtResult),MDM_GM_GAME_NOTIFY,ASS_APPLY_RESULT,0);
	}
	
	if (!m_bGameIsRun)
	{
		StartGame();
		m_bGameIsRun = true;		//��Ϸ�Ƿ��Ѿ�����
	}
	//��ׯ�������´ӿ���״̬��ʼ
	if((m_bGameStation>GS_FREE_STATUS)&&(m_bGameStation<GS_WAIT_NEXT))
	{
		
		if(TApplyNtResult.bNtFlag)
		{
			launchTimer();
		}
	}
}
//-----------------------------------------------------------------------------------------------
//�������������ׯ��Ϣ
void	CServerGameDesk::DealUserApplyLeaveNt(void *pBuffer)
{
	ApplyLiveNtMsg *pApplyLeaveNt = (ApplyLiveNtMsg *)pBuffer;
	if (pApplyLeaveNt == NULL)
	{
		return;
	}
	ApplyLiveNtResultMsg TApplyNtResult;
	TApplyNtResult.byDeskStation=pApplyLeaveNt->byDeskStation;
	if((pApplyLeaveNt->byDeskStation == m_byNtStation) && (GS_FREE_STATUS==m_bGameStation))//����״̬ׯ������ׯ
	{
		m_byNtStation=255;//ׯ��λ�����
		m_iNtPlayCount=0;		//ׯ����ׯ��������
		i64PlayerGrade[m_byNtStation]=0;//ׯ�ҳɼ�����
		//ѡׯ��
		list<BYTE>::iterator ite=m_blNtWaitList.begin();
		for(;ite!=m_blNtWaitList.end();ite++)
		{
			m_byNtStation=*ite;
			m_blNtWaitList.erase(ite);
			break;
		}
		if(255!=m_byNtStation)
		{
			m_iNtPlayCount++;
		}
	}
	else
	{
		//����ׯ���б��� ������ׯ��Ч
		if (!IsInWaitNtList(pApplyLeaveNt->byDeskStation))
		{
			return;
		}
		list<BYTE>::iterator ite=m_blNtWaitList.begin();
		for(;ite!=m_blNtWaitList.end();ite++)
		{
			if(pApplyLeaveNt->byDeskStation==*ite)
			{
				if(m_pUserInfo[pApplyLeaveNt->byDeskStation])
				{
					if(m_pUserInfo[pApplyLeaveNt->byDeskStation]->m_UserData.isVirtual)//�����˷�����ׯ�б���
					{
						m_bRobotNtCount--;
					}
				}
				
				m_blNtWaitList.erase(ite);
				break;
			}
		}

	}
	//����ׯ��
	TApplyNtResult.byNtStation=m_byNtStation;
	TApplyNtResult.byNtcount=m_iNtPlayCount;//ׯ����ׯ����
	//ׯ�ҵĳɼ�
	TApplyNtResult.i64NtScore=i64PlayerGrade[m_byNtStation];

	int i=0;
	////��ׯ�б�
	for(list<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
	{
		TApplyNtResult.byWaitNtList[i]=*ite;
		i++;
	}
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&TApplyNtResult,sizeof(TApplyNtResult),MDM_GM_GAME_NOTIFY,ASS_LEAVE_RESULT,0);
	}
	

}
//-----------------------------------------------------------------------------------------------
//���������ע
void	CServerGameDesk::DealUserBet(void *pBuffer)
{
	
	UserBetMsg *pUserBet = (UserBetMsg *)pBuffer;
	if(NULL == pUserBet)
	{
		return;
	}
	/*CString cs;
	cs.Format("test ��������ע����=%d  ����ֵ=%d",pUserBet->byBetArea,pUserBet->byChouMaType);
	OutputDebugString(cs);*/
	//������ע����
	__int64 i64RemainBetNum=RemainingBettingNumber(pUserBet->byBetArea);
	if(G_iChouMaValue[pUserBet->byChouMaType]>i64RemainBetNum)
	{
		return;
	}
	//���õ�������ע����
	if((m_i64AreaBetAllSum[pUserBet->byBetArea]+G_iChouMaValue[pUserBet->byChouMaType])>m_iAreaBetLimite)
	{

		return;
	}
	//�Լ���Ǯ��������ע
	if(G_iChouMaValue[pUserBet->byChouMaType]>GetUserRemaindMoney(pUserBet->byDeskStation))
	{
		return;
	}
	////��ע��ׯ�� ����
	if (pUserBet->byDeskStation == m_byNtStation)
	{
		return;
	}
	//��¼��������ע����
	if(m_pUserInfo[pUserBet->byDeskStation])
	{
		if(m_pUserInfo[pUserBet->byDeskStation]->m_UserData.isVirtual)//�����˷�����ׯ�б���
		{
			//��������ÿ���������ע��
			m_RobotAreaBetSum[pUserBet->byDeskStation][pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];
			//ÿ�������������ע����
			m_RobotAreaAllSum[pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];
			//ÿ�������˵���ע����
			m_RobotBetSum[pUserBet->byDeskStation] += G_iChouMaValue[pUserBet->byChouMaType];
			//��������ע����
			m_RobotAllSum+= G_iChouMaValue[pUserBet->byChouMaType];
		}
	}
	
	
	//��¼��ע���
	//�������ÿ���������ע���
	m_i64UserBetCount[pUserBet->byDeskStation][pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];
	//�����������ע
	m_i64AreaBetAllSum[pUserBet->byBetArea] += G_iChouMaValue[pUserBet->byChouMaType];
	//�����ע����
	m_i64UserBetSum[pUserBet->byDeskStation] += G_iChouMaValue[pUserBet->byChouMaType];
	//����ע��
	m_i64SumAllBet+= G_iChouMaValue[pUserBet->byChouMaType];

	UserBetResultMsg TUserBetResult;
	if(G_iChouMaValue[pUserBet->byChouMaType]>=100000)
	{
		TUserBetResult.byBetNumFlag=0x01;
	}
	else if (G_iChouMaValue[pUserBet->byChouMaType]>=1000000)
	{
		TUserBetResult.byBetNumFlag=0x02;
	}
	else if (G_iChouMaValue[pUserBet->byChouMaType]>=10000000)
	{
		TUserBetResult.byBetNumFlag=0x03;
	}
	//����ע
	TUserBetResult.i64SumAllBet=m_i64SumAllBet;
	//��ע��λ��
	TUserBetResult.byDeskStation=pUserBet->byDeskStation;
	//��ע����
	TUserBetResult.byBetArea = pUserBet->byBetArea;
	//��ע����ֵ
	TUserBetResult.byChouMaType = pUserBet->byChouMaType;
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		for(int j=0;j<BET_ARES;j++)
		{
			TUserBetResult.i64UserBetCount[j]=m_i64UserBetCount[i][j];
			TUserBetResult.i64AreaBetAllSum[j]=m_i64AreaBetAllSum[j];
			TUserBetResult.i64AreaRobot[j] = m_RobotAreaAllSum[j];
		}
		//OutputDebugString("test ������ע��");
		SendGameData(i,&TUserBetResult,sizeof(TUserBetResult),MDM_GM_GAME_NOTIFY,ASS_BET_RESULT,0);
	}
	
}
//�õ�����ʣ����ע��
__int64 CServerGameDesk::RemainingBettingNumber(BYTE byArea)
{
	__int64 betNum=0;
	for(int i=0;i<8;i++)
	{
		if(byArea==i)
		{
			continue;
		}
		betNum+=m_i64AreaBetAllSum[i];
	}
	__int64 MaxBetNum=0;
	if(m_pUserInfo[m_byNtStation])
	{
		MaxBetNum=(m_pUserInfo[m_byNtStation]->m_UserData.i64Money+betNum)/G_iArePeiLv[byArea];
	}
	return (MaxBetNum-m_i64AreaBetAllSum[byArea]);
}


//�Ƿ�����ׯ�б���
bool	CServerGameDesk::IsInWaitNtList(BYTE byDeskStation)
{
	/*for(int i=0; i<PLAY_COUNT; i++)
	{
		if (m_byWaitNtList[i] == byDeskStation )
		{
			return true;
		}
	}*/
	list<BYTE>::iterator ite=m_blNtWaitList.begin();
	for(;ite!=m_blNtWaitList.end();ite++)
	{
		if(byDeskStation==*ite)
		{
			return true;
		}
	}
	return false;
}


void CServerGameDesk::SendCard()
{
	

	SendAllStruct SendAll;
	::memset(&SendAll,0,sizeof(SendAll));
	//���Ʒ�������4	
	SendAll.iUserCardCount =SendCardCount;
	//����4���Ƶ����ݵ�iUserCardList������
	for(int i=0;i<SendCardCount;i++)
	{
		::CopyMemory(SendAll.iUserCardList,iCardArray,sizeof(BYTE)*SendCardCount);
	}
	//��������
	SendGameData(0,&SendAll,sizeof(SendAll),MDM_GM_GAME_NOTIFY,ASS_SEND_ALL_CARD,0);
		
	

}


//-----------------------------------------------------------------------------------------------
//��ȡ��Ϸ״̬��Ϣ����һ�ε��ã�
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	// ��֤�����û�Ȩ��
	SuperUserExamine(bDeskStation);
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			//��Ϸ�汾�˶�
			TGameStation.iVersion		= DEV_HEIGHT_VERSION;	//��Ϸ�߰汾
			TGameStation.iVersion2		= DEV_LOW_VERSION;		//�Ͱ汾
			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬

			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��
			TGameStation.byAccountTime=m_byShowResultTime;	             //����ʱ��
			TGameStation.byShowResult=m_byShowResult;//չʾ���ʱ��
			TGameStation.bySendCardSpaceTime	= m_bySendSpaceTime;	//���Ƽ��ʱ��
			TGameStation.bySpeedTime	=  m_bySpeedTime;               //�ڵ����ƶ��ٶ�
			TGameStation.byShowWinTime=m_byShowWinTime;//��ʾ��Ӯ
			TGameStation.byRemaindTime	= m_iClock;                     //ʣ��ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����

			//ׯ����Ϣ�������ͳɼ���
			TGameStation.i64NtGrade		= i64PlayerGrade[m_byNtStation];	//ׯ�ҳɼ�
			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//ׯ����ׯ����
			TGameStation.i64PlayerGrade =i64PlayerGrade[bDeskStation];//��ҳɿ�

			TGameStation.byEvent= m_byEvent;		//��Ϸ����
			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			
			return TRUE;
		}
	case GS_FREE_STATUS://���н׶�
		{
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			//��Ϸ�汾�˶�
			TGameStation.iVersion		= DEV_HEIGHT_VERSION;	//��Ϸ�߰汾
			TGameStation.iVersion2		= DEV_LOW_VERSION;		//�Ͱ汾
			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬

			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��

			TGameStation.byAccountTime=m_byShowResultTime;	             //����ʱ��
			TGameStation.byShowResult=m_byShowResult;//չʾ���ʱ��
			TGameStation.bySendCardSpaceTime	= m_bySendSpaceTime;	//���Ƽ��ʱ��
			TGameStation.bySpeedTime	=  m_bySpeedTime;               //�ڵ����ƶ��ٶ�
			TGameStation.byShowWinTime=m_byShowWinTime;//��ʾ��Ӯ
			TGameStation.byRemaindTime	= m_iClock;                     //ʣ��ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����

			//ׯ����Ϣ�������ͳɼ���
			TGameStation.i64NtGrade		= i64PlayerGrade[m_byNtStation];	//ׯ�ҳɼ�
			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//ׯ����ׯ����
			TGameStation.i64PlayerGrade =i64PlayerGrade[bDeskStation];//��ҳɿ�
			
			TGameStation.byEvent= m_byEvent;		//��Ϸ����

			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memset(TGameStation.byLuziList,255,sizeof(TGameStation.byLuziList));
			//����·��
			for(int i=0;i<m_dbyLuzi.size();i++)
			{
				TGameStation.byLuziList[i]=m_dbyLuzi.at(i);
			}

			list<BYTE>::iterator ite=m_blNtWaitList.begin();
			for(int i=0;ite!=m_blNtWaitList.end();ite++,i++)
			{
				if(i>PLAY_COUNT)
				{
					break;
				}
				TGameStation.byWaitNtList[i]=*ite;
			}
			m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			UserComePlayerMsg userComeMsg;
			userComeMsg.byPlayStation=bDeskStation;
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&userComeMsg,sizeof(userComeMsg),MDM_GM_GAME_NOTIFY,ASS_GAME_COME,0);
				}
			}
			return TRUE;

		}
	case GS_BET_STATUS:	//��Ϸ��ע�׶�
		{
			
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			//��Ϸ�汾�˶�
			TGameStation.iVersion		= DEV_HEIGHT_VERSION;	//��Ϸ�߰汾
			TGameStation.iVersion2		= DEV_LOW_VERSION;		//�Ͱ汾
			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬

			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��

			TGameStation.byAccountTime=m_byShowResultTime;	             //����ʱ��
			TGameStation.byShowResult=m_byShowResult;//չʾ���ʱ��
			TGameStation.bySendCardSpaceTime	= m_bySendSpaceTime;	//���Ƽ��ʱ��
			TGameStation.bySpeedTime	=  m_bySpeedTime;               //�ڵ����ƶ��ٶ�
			TGameStation.byShowWinTime=m_byShowWinTime;//��ʾ��Ӯ
			TGameStation.byRemaindTime	= m_iClock;                     //ʣ��ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����

			//ׯ����Ϣ�������ͳɼ���
			TGameStation.i64NtGrade		= i64PlayerGrade[m_byNtStation];	//ׯ�ҳɼ�
			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//ׯ����ׯ����
			TGameStation.i64PlayerGrade =i64PlayerGrade[bDeskStation];//��ҳɿ�

			TGameStation.byEvent= m_byEvent;		//��Ϸ����

			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memset(TGameStation.byLuziList,255,sizeof(TGameStation.byLuziList));
			//����·��
			for(int i=0;i<m_dbyLuzi.size();i++)
			{
				TGameStation.byLuziList[i]=m_dbyLuzi.at(i);
			}

			list<BYTE>::iterator ite=m_blNtWaitList.begin();
			for(int i=0;ite!=m_blNtWaitList.end();ite++,i++)
			{
				if(i>PLAY_COUNT)
				{
					break;
				}
				TGameStation.byWaitNtList[i]=*ite;
			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			UserComePlayerMsg userComeMsg;
			userComeMsg.byPlayStation=bDeskStation;
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&userComeMsg,sizeof(userComeMsg),MDM_GM_GAME_NOTIFY,ASS_GAME_COME,0);
				}
			}
			return TRUE;
		}
	case GS_SEND_STATUS://���ƽ׶�
		{
			
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			//��Ϸ�汾�˶�
			TGameStation.iVersion		= DEV_HEIGHT_VERSION;	//��Ϸ�߰汾
			TGameStation.iVersion2		= DEV_LOW_VERSION;		//�Ͱ汾

			TGameStation.bGameStation	= m_bGameStation;		//��Ϸ״̬

			TGameStation.byFreeTime		= m_byFreeTime;			//����ʱ��
			TGameStation.byBetTime		= m_byBetTime;			//��עʱ��
			TGameStation.bySendTime		= m_byOpenCardTime;	//����ʱ��

			TGameStation.byAccountTime=m_byShowResultTime;	             //����ʱ��
			TGameStation.byShowResult=m_byShowResult;//չʾ���ʱ��
			TGameStation.bySendCardSpaceTime	= m_bySendSpaceTime;	//���Ƽ��ʱ��
			TGameStation.bySpeedTime	=  m_bySpeedTime;               //�ڵ����ƶ��ٶ�
			TGameStation.byShowWinTime=m_byShowWinTime;//��ʾ��Ӯ
			TGameStation.byRemaindTime	= m_iClock;                     //ʣ��ʱ��

			TGameStation.iApplyNtLimite = m_iApplyNtLimite;		//��ׯ����
			TGameStation.byNtStation	= m_byNtStation;		//ׯ��λ��
			TGameStation.byMaxNtPeople	= m_byMaxNtPeople;		//�����ׯ����

			//ׯ����Ϣ�������ͳɼ���
			TGameStation.i64NtGrade		= i64PlayerGrade[m_byNtStation];	//ׯ�ҳɼ�
			TGameStation.iNtPlayCount	= m_iNtPlayCount;		//ׯ����ׯ����
			TGameStation.i64PlayerGrade =i64PlayerGrade[bDeskStation];//��ҳɿ�

			TGameStation.byEvent= m_byEvent;		//��Ϸ����

			memset(TGameStation.byWaitNtList,255,sizeof(TGameStation.byWaitNtList));
			memset(TGameStation.byLuziList,255,sizeof(TGameStation.byLuziList));
			//����·��
			for(int i=0;i<m_dbyLuzi.size();i++)
			{
				TGameStation.byLuziList[i]=m_dbyLuzi.at(i);
			}

			list<BYTE>::iterator ite=m_blNtWaitList.begin();
			for(int i=0;ite!=m_blNtWaitList.end();ite++,i++)
			{
				if(i>PLAY_COUNT)
				{
					break;
				}
				TGameStation.byWaitNtList[i]=*ite;
			}
			
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			UserComePlayerMsg userComeMsg;
			userComeMsg.byPlayStation=bDeskStation;
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_pUserInfo[i])
				{
					SendGameData(i,&userComeMsg,sizeof(userComeMsg),MDM_GM_GAME_NOTIFY,ASS_GAME_COME,0);
				}
			}
			return TRUE;
		}
		}
	return false;
}
/*
���ܣ��ж���Ӯ
����ֵ��1Ϊ��Ӯ��2Ϊ��Ӯ��3Ϊ��Ӯ
*/
BYTE   CServerGameDesk::DealWinner()
{
	
	BYTE part_a=0,part_b=0;
	part_a=iCardArray[0]+iCardArray[2];
	part_a=iCardArray[1]+iCardArray[3];
	if(part_a>part_b)
	{
		return 1;
	}
	if(part_a<part_b)
	{
		return 2;
	}
	if(part_a==part_b)
	{
		return 3;
	}
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{

	switch(bCloseFlag)
	{
	case GF_NORMAL:	//��������
		{
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
				{
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
				}
			}
			__super::GameFinish(bDeskStation,bCloseFlag); 
			return true;
		}
	case GF_AHEAD_END:	//��ׯ��ǰ����
		{             
			__super::GameFinish(bDeskStation,bCloseFlag);		
			return true;
		}
	case GF_SAFE:			//��Ϸ��ȫ����
	case GFF_FORCE_FINISH:	//�û������뿪
		{
			if(m_bGameStation>GS_FREE_STATUS && m_bGameStation<GS_WAIT_NEXT)
			{
				if(m_i64UserBetSum[bDeskStation]<=0 && bDeskStation!=m_byNtStation)
				{
					MakeUserOffLine(bDeskStation);
				}
			}
			return true;
		}
	}
	return __super::GameFinish(bDeskStation,bCloseFlag);;

}

//-----------------------------------------------------------------------------------------------------
//��Ϸ����
void	CServerGameDesk::DoGameResult(__int64 i64TurePoint[])
{
	//ׯ�Ҳ�����
	if (m_byNtStation == 255)
	{
		for (int i=0;i<PLAY_COUNT;i++)
		{
			i64TurePoint[i] = 0;
		}
		return;
	}

	//�ȼ����м�
	for (int i=0;i<PLAY_COUNT;i++)
	{
		if (i == m_byNtStation || NULL == m_pUserInfo[i])
		{
			continue;
		}
		i64TurePoint[i] = 0;
		//�÷�=�н��������ע�� * ���н����ʣ� - ����ע�� 
		i64TurePoint[i] = m_i64UserBetCount[i][m_byCurrResult-1] * (G_iArePeiLv[m_byCurrResult-1]) - m_i64UserBetSum[i];
	}

	//����ׯ��(�м���ľ���ׯ��Ӯ�ģ��м�Ӯ�ľ���ׯ�����)
	i64TurePoint[m_byNtStation] = 0;
	for (int i=0;i<PLAY_COUNT;i++)
	{
		if (i != m_byNtStation)
		{
			i64TurePoint[m_byNtStation] -= i64TurePoint[i];
		}
	}

}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	return true;
}


void CServerGameDesk::launchTimer()
{
	KillAllTimer();
	//�����Ϸ��ʱ����
	ResetGameData();
	//������Ϸ״̬
	m_bGameStation	= GS_FREE_STATUS;
	//ׯ��Ϊ�ոû�ׯ��
	if (m_byNtStation == 255)
	{
		//��ׯ��
		for(list<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
		{
			m_byNtStation=*ite;
			m_blNtWaitList.erase(ite);
			m_iNtPlayCount=0;
			i64PlayerGrade[m_byNtStation]=0;
			break;
		}
	}
	if(m_iNtPlayLimite==m_iNtPlayCount)//ׯ����ׯ�������˸û�ׯ��
	{
		m_byNtStation=255;
		//��ׯ��
		list<BYTE>::iterator ite=m_blNtWaitList.begin();
		for(;ite!=m_blNtWaitList.end();ite++)
		{
			m_byNtStation=*ite;
			m_blNtWaitList.erase(ite);
			m_iNtPlayCount=0;
			i64PlayerGrade[m_byNtStation]=0;
			break;
		}

	}

	if(255!=m_byNtStation && m_pUserInfo[m_byNtStation])//��Ҳ��㻻ׯ��
	{
		if((m_pUserInfo[m_byNtStation]->m_UserData.i64Money)<m_iApplyNtLimite)
		{
			m_byNtStation=255;
			//��ׯ��
			list<BYTE>::iterator ite=m_blNtWaitList.begin();
			for(;ite!=m_blNtWaitList.end();ite++)
			{
				m_byNtStation=*ite;
				m_blNtWaitList.erase(ite);
				m_iNtPlayCount=0;
				i64PlayerGrade[m_byNtStation]=0;
				break;
			}
		}
	}

	BeginFreeMsg freeMsg;
	freeMsg.bFlag=true;
	freeMsg.byFreeTime=m_byFreeTime;
	freeMsg.byNtStation=m_byNtStation;
	freeMsg.byNtcount=m_iNtPlayCount;
	freeMsg.byGameStation= m_bGameStation;         ///��ǰ����Ϸ״̬
	freeMsg.i64NtScore=i64PlayerGrade[m_byNtStation];
	int i=0;
	for(list<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
	{

		freeMsg.byWaitNtList[i]=*ite;
		i++;
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL==m_pUserInfo[i])
		{
			continue;
		}
		//������Ϸ��Ϣ
		SendGameData(i,&freeMsg,sizeof(freeMsg),MDM_GM_GAME_NOTIFY,ASS_GAME_FREE,0);
	}
	m_iClock=m_byFreeTime;
	//�������ж�ʱ��5��
	SetTimer(TIME_FREE_TIME,1*1000);
}

//-----------------------------------------------------------------------------------------------------
//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	__super::UserLeftDesk(bDeskStation,pUserInfo);

	//����ׯ���б��ׯ��
	if(NULL==m_pUserInfo[m_byNtStation])
	{
		m_byNtStation=255;
		m_iNtPlayCount=0;
		i64PlayerGrade[m_byNtStation]=0;//ׯ�ҳɼ�
	}
	for(list<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();)
	{
		if(NULL==m_pUserInfo[*ite])
		{
			m_blNtWaitList.erase(ite++);
		}
		else
		{
			ite++;
		}
	}

	if(255==m_byNtStation)
	{
		//��ׯ��
		for(list<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
		{
			m_byNtStation=*ite;
			m_blNtWaitList.erase(ite);
			break;
		}
		//��ׯ���ɹ�,ׯ����ׯ����Ϊ1,�ɼ�����
		if(255==m_byNtStation)
		{
			m_iNtPlayCount=1;
			i64PlayerGrade[m_byNtStation]=0;//ׯ�ҳɼ�
		}

	}
	//��Ӧ��ҳɼ�����
	if(NULL==m_pUserInfo[bDeskStation])
	{
		i64PlayerGrade[bDeskStation]=0;
	}

	GamePrepareStageQuit prepareQuit;

	//������ׯ�б��ׯ��
	prepareQuit.byNtStation=m_byNtStation;
	prepareQuit.byNtcount=m_iNtPlayCount;//��ׯ����
	prepareQuit.i64NtScore=i64PlayerGrade[m_byNtStation];//ׯ�ҳɼ�

	int i=0;
	for(list<BYTE>::iterator ite=m_blNtWaitList.begin();ite!=m_blNtWaitList.end();ite++)
	{

		prepareQuit.byNtWaitList[i]=*ite;
		i++;
	}
	for (int i=0; i<PLAY_COUNT; ++i)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&prepareQuit,sizeof(prepareQuit),MDM_GM_GAME_NOTIFY,ASS_PREPARE_END,0);
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------------
//�����ı��û����������ĺ���
//bool CServerGameDesk::CanNetCut(BYTE bDeskStation)
//{
//	CString debg;
//	debg.Format("exit-CanNetCut(server)\n");
//	OutputDebugString(debg);
//	OutputDebugString("dwjlog::S_���Զ���...");
//	return true;
//}
//-----------------------------------------------------------------------------------------------------
//�û������뿪
//bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
//{
//	CString debg;
//	debg.Format("exit-UserNetCut(server)\n");
//	OutputDebugString(debg);
//	OutputDebugString("dwjlog::S_��Ҷ���...");
//	/*if(bDeskStation==m_byNtStation)
//	{
//	OutputDebugString("exit-ׯ���˳���");
//	m_byNtStation=255;
//	}
//	for(int i=0;i<PLAY_COUNT;i++)
//	{
//	if(m_byWaitNtList[i]!=255)
//	{
//	if(bDeskStation==m_byWaitNtList[i])
//	{
//	m_byWaitNtList[i]=255;
//	}
//	}
//	}*/
//	//return true;
//	return __super::UserNetCut(bDeskStation,  pLostUserInfo);
//}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGameEx(BYTE bDeskStation)
{
	CString debg;
	debg.Format("exit-IsPlayGameEx(server)\n");
	OutputDebugString(debg);
	OutputDebugString("dwjlog::S_������Ϸ��EX...");
	
	return false;
}
//����ȡ���������й�
BOOL CServerGameDesk::SetAIMachine(BYTE bDeskStation,BOOL bState)
{
	CString debg;
	debg.Format("debug-SetAIMachine(server)\n");
	OutputDebugString(debg);
	return true;
}
//������Զ���ʼ���ɷ���˿���
BOOL CServerGameDesk::StartGame()
{
	launchTimer();
	return true;
}
//��Ϸ��ʼ
BOOL CServerGameDesk::BeginPlay()
{
	return true;
}
//�l�ͽY��
BOOL CServerGameDesk::SendCardFinish()
{
	return true;
}