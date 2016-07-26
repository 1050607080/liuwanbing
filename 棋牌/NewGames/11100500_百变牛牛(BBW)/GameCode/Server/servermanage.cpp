#include "StdAfx.h"
#include "ServerManage.h"
#include <algorithm>
#include <tchar.h>
#include "Shlwapi.h"

void DebugPrintf(const char *p, ...)
{
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

bool Com(BYTE wFirstCard[],BYTE wSecondCard[])
{
	CPokerGameLogic Logic;
	if(Logic.CompareCard(wFirstCard,5,wSecondCard,5) > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}


CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)//lym�޸�Ϊ�������ͬ����ܿ�ʼģʽ
{
	m_bGameStation=GS_WAIT_AGREE;  // ��ʼΪ�ȴ���ʼ״̬
	
	m_bIsStart=false;//�Ƿ�ʼ��Ϸ
	m_dqResultInfo.clear();
	m_iRemaindTime = 0;

	m_i64Max      = 0;
	m_iMaxNote  = -1;
	m_bNtTax    = 0;
	m_bCompareKind = true ;
	m_bHaveKing = true;
	
////////////////////////��ʼ������lym//////////////////////////////////////////////////////////////////

	//memset(&m_iZhuangStation, -1, sizeof(m_iZhuangStation)); //ׯ���б��ܵ�ׯ��λ��
	memset(&m_idzhuangstation, -1, sizeof(m_idzhuangstation)); //ׯ���б��ܵ�ׯ��λ��202121120
	memset(&m_iQuYuZhu, 0, sizeof(m_iQuYuZhu));	//����ÿ�������µ�ע��
	memset(&m_iQuYuZhuTrue, 0, sizeof(m_iQuYuZhuTrue));	//����ÿ�������µ�ע��
	//memset(&m_iCardShape,0, sizeof(m_iCardShape));
	memset(&m_iComputePoint,0, sizeof(m_iComputePoint));
	memset(&m_iResultInfo, 0, sizeof(m_iResultInfo));	//������Ϣ
	memset(&m_UserCard, 0, sizeof(m_UserCard));		    // �û����ϵ��˿�
	memset(&m_UserCardTotal, 0, sizeof(m_UserCardTotal));		    // �û����ϵ��˿�
	memset(&m_CardCount, 0, sizeof(m_CardCount));//ׯ��˫��������

	memset(&m_iRectNoteMax, 0, sizeof(m_iRectNoteMax));
	m_iGameCount = 0;           //�Ѿ����еİ���
	m_iGameBeen  = 0;          // �Ѿ����еľ���
	
	

	for(int i=0;i<PLAY_COUNT;i++)
	{
		for(int j=0;j<(MAX_NOTE_COUNT+1);j++)
		{
			m_iUserXiaZhuData[i][j]=0;
		}
	}
	m_iMaxZhuang = 30;			//ׯ��һ����������پ֣�Ĭ��30��
	m_iZhongZhu=0;				//���ѵ�ǰ��ע��
	m_iZhuangFen=0;     //ׯ�ҵĵ÷�
	m_iXianFen=0;	   //�мҵĵ÷�

	m_iZhuangBaShu=0;      //ׯ�ҽ����˼���

	m_iXiaZhuTime = 0;			//��עʱ��			
	m_iKaiPaiTime = 0;         //����ʱ��
	m_iFreeTime = 0;			//����ʱ��

	m_iNowNtStation=-1;     //��ǰׯ��λ��

	m_bXiaZhuang=false;     //��ǰׯ��������ׯ

	m_iShangZhuangLimit=0; //��ׯ��Ҫ�����ٽ��

	m_iNtWin =0;              //��ǰׯ��Ӯ�Ľ��


	int i=0;
	for (i=0; i<PLAY_COUNT; ++i)
	{
		m_iUserFen[i] =0;          //������ұ��ֵĵ÷�
		m_iUserMoney[i] =0;          //������ұ���Ӧ�Ľ��
	}
	

	int base = 0;
	for (int i=0; i<11; i++)
	{
		base = i>0?i:1;
		m_bCardShapeBase[i] = base;
	}

	m_bAIWinAndLostAutoCtrl = false;///�Ƿ�����������Ӯ�Զ�����
	m_iAIHaveWinMoney = 0;		///�������Ѿ�Ӯ�˶���Ǯ

	m_vlSuperUserId.clear();
}

///��ʼ�����Ӻ�����һ����������������ļ�
bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadIni();
	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
CServerGameDesk::~CServerGameDesk(void)
{
}
/*-----------------------------------------------------------------------------------------*/
///����ini�����ļ�
BOOL	CServerGameDesk::LoadIni()
{	
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("game");

	m_iFreeTime = f.GetKeyVal(szSec,"freetime",10);			//��ȡ����ʱ��
	if (m_iFreeTime<10)
	{
		m_iFreeTime = 10;
	}
	m_iXiaZhuTime = f.GetKeyVal(szSec,"xiazhutime",15);		//��ȡ��עʱ��
	if (m_iXiaZhuTime<10)
	{
		m_iXiaZhuTime = 10;
	}
	m_iKaiPaiTime = f.GetKeyVal(szSec,"kaipaitime",20);		//��ȡ����ʱ��
	if (m_iKaiPaiTime<20)
	{
		m_iKaiPaiTime = 20;
	}
	
	m_iShangZhuangLimit = f.GetKeyVal(szSec, _T("ShangZhuangLimit"), (__int64)10000);
	if (m_iShangZhuangLimit<1000)
	{
		m_iShangZhuangLimit = 1000;
	}
	m_iMaxZhuang = f.GetKeyVal(szSec,"maxzhuang",30);			//��ȡ���ׯ��
	if (m_iMaxZhuang<5)
	{
		m_iMaxZhuang = 5;
	}
	///�Ƿ�Ƚϻ�ɫ�������INI�������ˣ�
	m_bCompareKind = f.GetKeyVal(szSec, "CompareKind" , 1) ; 
	//�Ƿ�Ҫ��С��
	m_bHaveKing = f.GetKeyVal(szSec,"Haveking",0);
	//�Ƿ�ֻ��ׯ�ҵ�˰��
	m_bNtTax = f.GetKeyVal(szSec, "NtTax", 0);
	//�����ׯ����
	m_iRobotSZCount = f.GetKeyVal(szSec, "SZCount", 0);
	//���������ע��
	m_iMaxNote = f.GetKeyVal(szSec, "MaxNote", 1000000000);
	//��ע��������
	CString strName ;
	for(int i = 0 ; i< MAX_NOTE_COUNT ; i++)
	{
		strName.Format("RectNoteMax[%d]",i) ; 
		m_iRectNoteMax[i] = f.GetKeyVal(szSec , strName ,0) ; 
		if(m_iRectNoteMax[i]<0)
		{
			m_iRectNoteMax[i]=0;
		}
	}

	//������������
	CString keyName;
	int base = 0;
	for (int i=0; i<11; i++)
	{
		keyName.Format("CardShape%d",i);
		base = i>0?i:1;
		m_bCardShapeBase[i] = f.GetKeyVal(szSec,keyName,base);
		if(m_bCardShapeBase[i]<1)
		{
			m_bCardShapeBase[i]=1;
		}
	}
	
	//��ȡ��������
	TCHAR szSet[_MAX_PATH] = TEXT("SuperSet");
	int iCount = f.GetKeyVal(szSet, "SuperUserCount" , 0) ; 
	long lUserId = 0;
	CString sKey;
	for(int i=0; i<iCount; i++)
	{
		sKey.Format("SuperUserID%d",i);
		lUserId = f.GetKeyVal(szSet,sKey,0) ; 
		m_vlSuperUserId.push_back(lUserId);
	}

	return TRUE;
}
/*-----------------------------------------------------------------------------------------*/
///���ݷ���ID���������ļ�
///
///@param int iRoomID��ǰ�����ID��
///
BOOL	CServerGameDesk::LoadExtIni(int iRoomID)
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("game");
	_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,iRoomID);

	
	m_iFreeTime = f.GetKeyVal(szSec,"freetime",m_iFreeTime);			//��ȡ����ʱ��
	if(m_iFreeTime<10)
	{
		m_iFreeTime=10;
	}
	m_iXiaZhuTime = f.GetKeyVal(szSec,"xiazhutime",m_iXiaZhuTime);		//��ȡ��עʱ��
	if(m_iXiaZhuTime<10)
	{
		m_iXiaZhuTime=10;
	}
	m_iKaiPaiTime = f.GetKeyVal(szSec,"kaipaitime",m_iKaiPaiTime);		//��ȡ����ʱ��
	if(m_iKaiPaiTime < 20)
	{
		m_iKaiPaiTime = 20;
	}
	
	m_iShangZhuangLimit = f.GetKeyVal(szSec, _T("ShangZhuangLimit"), m_iShangZhuangLimit);
	if (m_iShangZhuangLimit<1000)
	{
		m_iShangZhuangLimit = 1000;
	}
	m_iMaxZhuang = f.GetKeyVal(szSec,"maxzhuang",m_iMaxZhuang);			//��ȡ���ׯ��
	if (m_iMaxZhuang<5)
	{
		m_iMaxZhuang = 5;
	}
	///�Ƿ�Ƚϻ�ɫ�������INI�������ˣ�
	m_bCompareKind = f.GetKeyVal(szSec, "CompareKind" , m_bCompareKind) ; 
	//�Ƿ�Ҫ��С��
	m_bHaveKing = f.GetKeyVal(szSec,"Haveking",m_bHaveKing);
	//�Ƿ�ֻ��ׯ�ҵ�˰��
	m_bNtTax = f.GetKeyVal(szSec, "NtTax", m_bNtTax);
	//�����ׯ����
	m_iRobotSZCount = f.GetKeyVal(szSec, "SZCount", m_iRobotSZCount);
	if(m_iRobotSZCount<0)
	{
		m_iRobotSZCount=0;
	}
	//���������ע��
	m_iMaxNote = f.GetKeyVal(szSec, "MaxNote", m_iMaxNote);
	//��ע��������
	CString strName ;
	for(int i = 0 ; i< MAX_NOTE_COUNT ; i++)
	{
		strName.Format("RectNoteMax[%d]",i) ; 
		m_iRectNoteMax[i] = f.GetKeyVal(szSec , strName ,m_iRectNoteMax[i]) ; 
		if(m_iRectNoteMax[i]<0)
		{
			m_iRectNoteMax[i]=0;
		}
	}
	//������������
	CString keyName;
	for (int i=0; i<11; i++)
	{
		keyName.Format("CardShape%d",i);
		m_bCardShapeBase[i] = f.GetKeyVal(szSec,keyName,m_bCardShapeBase[i]);
		if(m_bCardShapeBase[i]<1)
		{
			m_bCardShapeBase[i]=1;
		}
	}


	/*--------------------------- �����˽��ؿ��Ʋ���-----------------------------*/
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",0);///�Ƿ�����������Ӯ�Զ�����

	m_iAIWantWinMoneyA1	= f.GetKeyVal(szSec,"AIWantWinMoneyA1 ",500000);		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(szSec,"AIWantWinMoneyA2 ",5000000);		/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(szSec,"AIWantWinMoneyA3 ",50000000);		/**<������ӮǮ����3  */

	m_iAIWinLuckyAtA1	= f.GetKeyVal(szSec,"AIWinLuckyAtA1 ",90);				/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(szSec,"AIWinLuckyAtA2 ",70);				/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(szSec,"AIWinLuckyAtA3 ",50);				/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(szSec,"AIWinLuckyAtA4 ",30);				/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAIHaveWinMoney	= f.GetKeyVal(szSec,"ReSetAIHaveWinMoney ",0);			/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */
	return TRUE;
}

//-------------------------------add-by-DWJ-20121113-----------------------------------------------------------------------------------------
//�������ļ��ж�ȡ�������Ѿ�Ӯ�˶���Ǯ����Ϊ��ÿ�̽����ʱ�򶼻Ὣ������ӮǮ����д�������ļ��С� 
//������ÿ�̿�ʼ��ʱ���ֶ�ȡ�£���Ϊ�������޸�����ô�ͻ�������Ǯ��.
void	CServerGameDesk::GetAIContrlSetFromIni(int iRoomID)
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	CString szSec = TEXT("game");
	szSec.Format("%s_%d",SKIN_FOLDER,iRoomID);
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",0);///�Ƿ�����������Ӯ�Զ�����
	m_iAIWantWinMoneyA1	= f.GetKeyVal(szSec,"AIWantWinMoneyA1 ",m_iAIWantWinMoneyA1);		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(szSec,"AIWantWinMoneyA2 ",m_iAIWantWinMoneyA2);		/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(szSec,"AIWantWinMoneyA3 ",m_iAIWantWinMoneyA3);		/**<������ӮǮ����3  */
	m_iAIWinLuckyAtA1	= f.GetKeyVal(szSec,"AIWinLuckyAtA1 ",m_iAIWinLuckyAtA1);			/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(szSec,"AIWinLuckyAtA2 ",m_iAIWinLuckyAtA2);			/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(szSec,"AIWinLuckyAtA3 ",m_iAIWinLuckyAtA3);			/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(szSec,"AIWinLuckyAtA4 ",m_iAIWinLuckyAtA4);			/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAIHaveWinMoney	= f.GetKeyVal(szSec,"ReSetAIHaveWinMoney ",m_iAIHaveWinMoney);		/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */
}
//-------------------------------------------------------------------------------------------------------------------------------------------








/*------------------------------------------------------------------------------------------*/
///��Ϸ��ʼ
///@param BYTE bBeginFlag ��ʼģʽ
bool	CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
    if(!m_bIsStart)
	{
		return false;
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL != m_pUserInfo[i])//��Ҳ����ھ�
		{
			m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
		}
	}

	if (!__super::GameBegin(bBeginFlag) ) 
	{
		StartGame();
		return false;
	}
	KillAllTimer();
	//ÿ����Ϸ��ʼ��ʱ���ȡ�»����˽��ؿ���
	GetAIContrlSetFromIni(m_pDataManage->m_InitData.uRoomID);

	// ��Ϸһ��ʼ�ͽ�����ע״̬��
	m_bGameStation = GS_BET;
	//�Ƿ�ʼ��Ϸ
	m_bIsStart = !m_bIsStart;
	//����һ�뵹��ʱ��ʱ�� ���ڼ�����׶λ�ʣ����ʱ��
	SetTimeRemaindCount(m_iXiaZhuTime);
	//��ʼ������������
	ServerInit();
	//ׯ�ҽ����˼���
	m_iZhuangBaShu++;		
	// ��Ϸ�����ۼ�
	m_iGameCount += 1;
	// �жϵ�һ�ο�ʼ��Ϸ �� ��Ϸ�����Ѵ���������
	if (1 == m_iGameCount || m_iGameCount > GAME_SAVE_COUNT)
	{
		m_iGameCount = 1;		//��Ϸ�İ���		
		m_iGameBeen += 1;		//��Ϸ�ľ���
	}	
	
	if(m_iNowNtStation != -1 && m_pUserInfo[m_iNowNtStation] != NULL)
	{
		m_i64Max = m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/m_bCardShapeBase[10]; //���������ע
	}
	else
	{
		m_i64Max = 0;
	}

	// ֪ͨ������Ҹ�����ͼ
	BEGIN_DATA TBeginData;
	TBeginData.iGameBeen		= m_iGameBeen;		//��ǰ��Ϸ�ľ���
	TBeginData.iNowNtStation	= m_iNowNtStation;	//��ǰׯ��λ��
	TBeginData.iNTdata[0]		= m_iNowNtStation;
	if (-1 != m_iNowNtStation && m_pUserInfo[m_iNowNtStation])
	{
		TBeginData.iNTdata[1] = m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;
	}
	else
	{
		TBeginData.iNTdata[1] = 0;
	}
	TBeginData.iNTdata[2]	= m_iNtWin;					//ׯ�ҵĳɼ�
	TBeginData.iNTdata[3]	= m_iZhuangBaShu;			//ׯ�ҽ����˼���
	TBeginData.iZhuangBaShu	= m_iZhuangBaShu;			//��ǰׯ�ҽ��еİ���
	TBeginData.iMaxZhu		= m_i64Max;					//�����м����µ������ע��
	TBeginData.iMaxBase		=  m_bCardShapeBase[10];	//��Ϸ�����
	for(int i=0;i<=PLAY_COUNT;i++)//ׯ���б�
	{
		TBeginData.iNTlist[i][0] = m_idzhuangstation[i][0];
		TBeginData.iNTlist[i][1] = m_idzhuangstation[i][1];
	}

	TBeginData.iShangZhuangLimit	= m_iShangZhuangLimit;	//��ׯ����
	TBeginData.iGameCount  = m_iGameCount;				//��Ϸ����
	TBeginData.iXiaZhuTime = m_iXiaZhuTime;				//��עʱ��
	TBeginData.iKaiPaiTime = m_iKaiPaiTime;				//����ʱ��
	TBeginData.iFreeTime   = m_iFreeTime;				//����ʱ��
	// ���������ֵ
	memcpy(TBeginData.i64ChoumaValue,G_i64ChoumaValue,sizeof(TBeginData.i64ChoumaValue)); 

	for (int i=0;i<PLAY_COUNT; ++i)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}	
		SendGameData(i,&TBeginData,sizeof(TBeginData),MDM_GM_GAME_NOTIFY, ASS_GAME_BEGIN,0);//������ע��Ϣ
	}

	//������ע��ʱ��
	SetTimer(ID_TIMER_XIA_ZHU,(m_iXiaZhuTime)*1000);

	return true;
}
/*---------------------------------------------------------------------*/
//����һ�뵹��ʱ��ʱ�� ���ڼ�����׶λ�ʣ����ʱ��
void	CServerGameDesk::SetTimeRemaindCount(int  iTime)
{
	KillTimer(TIMER_REMAIND_COUNT);
	m_iRemaindTime = iTime;
	SetTimer(TIMER_REMAIND_COUNT,1*1000);
}
/*---------------------------------------------------------------------*/
//����ׯ��
void CServerGameDesk::ClearNt()
{
	if(m_pUserInfo[m_iNowNtStation]==NULL)
	{
		m_iNowNtStation = -1;
	}
	else
	{
		if(	m_bXiaZhuang
			|| m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money < m_iShangZhuangLimit
			|| m_iZhuangBaShu >= m_iMaxZhuang
			|| m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money>150000000000000000)//��ǰׯ������������ׯ��
		{
			m_iNowNtStation = -1;//���õ�ǰׯ��Ϊ-1
			m_iZhuangBaShu = 0;   //��ʼ��ׯ�ҵľ���
			m_iNtWin = 0;        //��ʼ��ׯ��Ӯ�Ľ��
			m_bXiaZhuang = false;		//��ǰׯ��������ׯ
		}
	}
}

///��Ϸ����
///
///@param BYTE byDeskStation ������λ��
///@param BYTE bCloseFlag    ��������
///
bool CServerGameDesk::GameFinish(BYTE byDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:
		{
			m_bGameStation = GS_WAIT_NEXT;
			bCloseFlag=GF_NORMAL;
			return __super::GameFinish(byDeskStation,bCloseFlag);
		}
	case GF_SAFE:
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			/*if(m_bGameStation>=GS_BET && m_bGameStation<=GS_PLAY_GAME)*/
			{
				//������ǿ������ʾǿ����Ϣ��add by wlr 20090807
				if (bCloseFlag == GFF_FORCE_FINISH)
				{
					if ((byDeskStation != m_iNowNtStation)
						&& (m_iUserXiaZhuData[byDeskStation][4] == 0)
						)
					{
						MakeUserOffLine(byDeskStation);
						return true;
					}

				}
				//end of add by wlr 20090807
				return true;
			}
			m_bGameStation = GS_WAIT_AGREE;

			int i = 0;

			//CutEnd.iTurePoint[byDeskStation] = - GetRunPublish();
			if (GF_SAFE == bCloseFlag)
			{
	
			}
			else
			{
				//bool temp_cut[PLAY_COUNT];
				//memset(temp_cut,0,sizeof(temp_cut));
				//for (i=0; i<PLAY_COUNT; ++i)
				//{
				//	temp_cut[i] = (byDeskStation == i && CutEnd.iTurePoint[i] < 0);
				//}
				//ChangeUserPoint(CutEnd.iTurePoint, temp_cut);
				//__super::RecoderGameInfo(CutEnd.iChangeMoney);
			
				bCloseFlag = GFF_FORCE_FINISH;
			}
			return true;
			break;
		}
	default:
		break;
	}

	
	return __super::GameFinish(byDeskStation,bCloseFlag);
}

///
///��ǰ�Ƿ�����Ϸ��
///@param BYTE bDeskStation ���ţ���ʵ�޾�������
///@return �����Ƿ�����Ϸ��
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	__int64 llUserNoteAll  = 0l; 

	for (int i = 0 ; i < 5; i++)
	{
		llUserNoteAll += m_iUserXiaZhuData[bDeskStation][i]; 
	}

	if (0 == llUserNoteAll && bDeskStation != m_iNowNtStation) //�����;�����
	{
		OutputDebugString("dwjnn::������Ϸ��");
		return false; 
	}
	else
	{
		OutputDebugString("dwjnn::������Ϸ��1");
		return 	m_bGameStation >= GS_BET && m_bGameStation < GS_WAIT_NEXT;
	}	
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

			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//�յ����˿�����Ϣ
void	CServerGameDesk::OnHandleSuperUserSet(BYTE bDeskStation,void * pData, UINT uSize)
{
	TagSuperSet *pSuperSet = (TagSuperSet *)pData;
	if (NULL == pSuperSet)
	{
		return;
	}

	m_TSuperSet.bSuccesse = true;
	for(int i=0; i<4; i++)
	{
		m_TSuperSet.iAreaContrl[i] = pSuperSet->iAreaContrl[i];
	}

	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(bDeskStation,&m_TSuperSet,sizeof(m_TSuperSet),MDM_GM_GAME_NOTIFY, ASS_SUPER_SET,0);//���ͳɹ���Ϣ	
	}

}

///
///��Ϣ������
///@param BYTE bDeskStation �ͻ��˷�����Ϣ������
///@param NetMessageHead * pNetHead ��Ϣͷ
///@param void * pData ���ݰ�
///@param UINT uSize ���ݰ���С
///@param UINT uSocketID socketid
///@param bool bWatchUser �Ƿ��Թ�
///@return ����false ��������ͻ��˷���Ϣ���˶���
///
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_SUPER_SET:
		{
			if (bWatchUser) 
			{
				return false;
			}
			if (sizeof(TagSuperSet) != uSize)
			{
				return false;
			}
			if (IsSuperUser(bDeskStation))
			{
				OnHandleSuperUserSet(bDeskStation,pData, uSize);
			}
		
			return true;
		}
	case ASS_XIA_ZHU:               // ��ע��Ϣ
		{		
			if (sizeof(TagXiaZhu) != uSize)
			{
				return false;
			}
			if(	m_bGameStation == GS_BET)//����ע״̬�Ž��ո���Ϣ
			{
				//����ͻ��˷��ص���ע��Ϣ
				HandleXiaZhu(bDeskStation,pData, uSize);
			}
			return true;
		}
	case ASS_KAI_PAI:				  // ������Ϣ
		{
			return true;
		}
	case ASS_SHANG_ZHUANG:          // ��ׯ��Ϣ
		{
			if (sizeof(SHANG_ZHUANG) != uSize)
			{
				return false;
			}
			SHANG_ZHUANG *pShangZhuang = (SHANG_ZHUANG *)pData;
			if (NULL == pShangZhuang)
			{
				return false;
			}

			if(pShangZhuang->shang && m_pUserInfo[bDeskStation] != NULL)//��ׯ����
			{					
				if (pShangZhuang->bIsAi && (m_idzhuangstation[PLAY_COUNT][0]>=m_iRobotSZCount))
				{
					return true;//�����˲�������ׯ
				}
			
				if(m_pUserInfo[bDeskStation]->m_UserData.i64Money>= m_iShangZhuangLimit && m_pUserInfo[bDeskStation]->m_UserData.i64Money<150000000000000000)
				{
					HanleSXzhuang(bDeskStation,pData, uSize);
				}
				else 
				{			
					SHANG_ZHUANG Tlost;
					Tlost.success= false;
					for(int i=0;i<=PLAY_COUNT;i++)
					{			
						Tlost.dzhuangstation[i][0] = m_idzhuangstation[i][0];
						Tlost.dzhuangstation[i][1] = m_idzhuangstation[i][1];
					}
					SendGameData(bDeskStation,&Tlost,sizeof(Tlost),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//������ׯʧ����Ϣ
				}
			}
			else
			{
				HanleSXzhuang(bDeskStation,pData, uSize);
			}
			return true;
		}
	default:
		break;
	}
	
	return __super::HandleNotifyMessage(bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser);
}
///�û������뿪
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	if (bDeskStation == m_iNowNtStation)
	{
		m_bXiaZhuang = true;
	}
	return __super::UserNetCut(bDeskStation, pLostUserInfo); 
}
///
///�û��뿪��Ϸ��
///@param BYTE bDeskStation �뿪�˵�����
///@param CGameUserInfo * pUserInfo �뿪������
///@return ����
///
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	if (m_bGameStation>=GS_BET && m_bGameStation <= GS_PLAY_GAME)  //����Ϸ�У�
	{
		if (m_iNowNtStation != bDeskStation)
			return __super::UserLeftDesk(bDeskStation, pUserInfo);
		else
			return true;
	}
	else
	{
		if (m_iNowNtStation == bDeskStation)
		{
			m_iNowNtStation=-1;//���õ�ǰׯ��Ϊ-1
			m_iZhuangBaShu = 0;   //��ʼ��ׯ�ҵľ���
			m_iNtWin =0;        //��ʼ��ׯ��Ӯ�Ľ��
		}
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_idzhuangstation[i][0]==bDeskStation)//
			{
				m_idzhuangstation[i][0]=-1;
				m_idzhuangstation[i][1] = -1;
				m_idzhuangstation[PLAY_COUNT][0]--;
				break;
			}
		}
		MoveData(0);//��������

		return __super::UserLeftDesk(bDeskStation, pUserInfo);
	}
}

//lym0526����������º���
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	return __super::UserSitDesk(pUserSit, pUserInfo);
}

///
///�ս�����Ϸ���˻�ȡ��ǰ��Ϸ״̬��Ϣ�������͵��ͻ����ÿͻ������ó���
///@param BYTE bDeskStation ��������
///@param UINT uSocketID  socket
///@param bool bWatchUser �Ƿ��Թ����
///@return ����ֵbool  �޾�������
///
bool	CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	///����״̬����ֹ��������������Ҷ��������Ĺ���������ڵ�һ���ܹ������д�Ǯ
	if (IsPlayGame(bDeskStation))
	{
		m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_PLAY_GAME;//���������Ϊͬ��״̬
	}	
	//������֤
	if (IsSuperUser(bDeskStation))
	{
		TagSuperUser TSuperUser;
		TSuperUser.byStation = bDeskStation;
		TSuperUser.bIsSuper	 = true;
		OutputDebugString("dwjnn::�ǳ������");
		SendGameData(bDeskStation, &TSuperUser,sizeof(TSuperUser),MDM_GM_GAME_NOTIFY, ASS_IS_SUPER_USER,0);
	}

	CString dwjlog;
	dwjlog.Format("dwjnn::m_bGameStation=%d",m_bGameStation);
	OutputDebugString(dwjlog);
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
	case GS_FREE:
		{
			// ���͵��ͻ���
			TagGameStation_WaiteAgee TGameStation;	
			TGameStation.byGameStaion	= m_bGameStation;		//��Ϸ״̬

			TGameStation.bMaxBase		= m_bCardShapeBase[10];	/// ����ʼ�ţţ�ı���

			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;	///��עʱ��
			TGameStation.iKaiPaiTime	= m_iKaiPaiTime;	///����ʱ��		
			TGameStation.iFreeTime		= m_iFreeTime;		///����ʱ��	
			TGameStation.iRemaindTime	= m_iRemaindTime;	///ʣ��ʱ��

			TGameStation.iNowNtStation	= m_iNowNtStation;	///��ǰׯ�ҵ�λ��
			TGameStation.iGameBeen		= m_iGameBeen;		///��ǰ����		
			TGameStation.iZhuangBaShu	= m_iZhuangBaShu;	///ׯ�ҽ����˼���	
			TGameStation.iGameCount		= m_iGameCount;		///��ǰ�Ѿ������˼���
			TGameStation.i64NtWin		= m_iNtWin;			///ׯ����Ӯ�ɼ�
			TGameStation.i64MaxNote		= m_iMaxNote;		///������Ϸ�����ע��

			TGameStation.i64ShangZhuangLimit	= m_iShangZhuangLimit;			///��ׯ��Ҫ�����ٽ��

			///������ע����
			memcpy(TGameStation.i64RectNoteMax,m_iRectNoteMax,sizeof(m_iRectNoteMax));
			// ���������ֵ
			memcpy(TGameStation.i64ChoumaValue,G_i64ChoumaValue,sizeof(G_i64ChoumaValue));
					
			//��ׯ�б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				TGameStation.idzhuangstation[i][0] = m_idzhuangstation[i][0];
				TGameStation.idzhuangstation[i][1] = m_idzhuangstation[i][1];
			}
			//·����Ϣ
			for(int i=0;i<GAME_SAVE_COUNT;i++)
			{
				for(int j=0;j<MAX_NOTE_COUNT;j++)
				{
					TGameStation.iResultInfo[i][j] = m_iResultInfo[i][j];
				}
			}
				
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return true;
		}
	case GS_BET:		//��ע״̬
		{	
			TagGameStation_Bet TGameStation;
			TGameStation.byGameStaion	= m_bGameStation;		//��Ϸ״̬

			TGameStation.bMaxBase		= m_bCardShapeBase[10];	/// ����ʼ�ţţ�ı���

			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;	///��עʱ��
			TGameStation.iKaiPaiTime	= m_iKaiPaiTime;	///����ʱ��		
			TGameStation.iFreeTime		= m_iFreeTime;		///����ʱ��	
			TGameStation.iRemaindTime	= m_iRemaindTime;	///ʣ��ʱ��

			TGameStation.iNowNtStation	= m_iNowNtStation;	///��ǰׯ�ҵ�λ��
			TGameStation.iGameBeen		= m_iGameBeen;		///��ǰ����		
			TGameStation.iZhuangBaShu	= m_iZhuangBaShu;	///ׯ�ҽ����˼���	
			TGameStation.iGameCount		= m_iGameCount;		///��ǰ�Ѿ������˼���
			TGameStation.i64NtWin		= m_iNtWin;			///ׯ����Ӯ�ɼ�
			TGameStation.i64MaxNote		= m_iMaxNote;		///������Ϸ�����ע��

			TGameStation.i64ShangZhuangLimit	= m_iShangZhuangLimit;			///��ׯ��Ҫ�����ٽ��

			///������ע����
			memcpy(TGameStation.i64RectNoteMax,m_iRectNoteMax,sizeof(m_iRectNoteMax));
			// ���������ֵ
			memcpy(TGameStation.i64ChoumaValue,G_i64ChoumaValue,sizeof(G_i64ChoumaValue));

			//��ׯ�б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				TGameStation.idzhuangstation[i][0] = m_idzhuangstation[i][0];
				TGameStation.idzhuangstation[i][1] = m_idzhuangstation[i][1];
			}
			//·����Ϣ
			for(int i=0;i<GAME_SAVE_COUNT;i++)
			{
				for(int j=0;j<MAX_NOTE_COUNT;j++)
				{
					TGameStation.iResultInfo[i][j] = m_iResultInfo[i][j];
				}
			}
		
			TGameStation.iZhongZhu = m_iZhongZhu;			//���ѵ�ǰ��ע��
			for(int i=0;i<MAX_NOTE_COUNT;i++)
			{
				TGameStation.iMyZhu[i]		= m_iUserXiaZhuData[bDeskStation][i];	//����������ע
				TGameStation.iQuYuZhu[i]	= m_iQuYuZhu[i];						///����ÿ�������µ�ע��
				TGameStation.iQuYuZhuTrue[i]= m_iQuYuZhuTrue[i];					///������ʵÿ�������µ�ע��
			}
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return true;
		}
	case GS_SEND_CARD:	//����״̬
		{
			TagGameStation_OpenCard TGameStation;
			TGameStation.byGameStaion	= m_bGameStation;		//��Ϸ״̬

			TGameStation.bMaxBase		= m_bCardShapeBase[10];	/// ����ʼ�ţţ�ı���

			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;	///��עʱ��
			TGameStation.iKaiPaiTime	= m_iKaiPaiTime;	///����ʱ��		
			TGameStation.iFreeTime		= m_iFreeTime;		///����ʱ��	
			TGameStation.iRemaindTime	= m_iRemaindTime;	///ʣ��ʱ��

			TGameStation.iNowNtStation	= m_iNowNtStation;	///��ǰׯ�ҵ�λ��
			TGameStation.iGameBeen		= m_iGameBeen;		///��ǰ����		
			TGameStation.iZhuangBaShu	= m_iZhuangBaShu;	///ׯ�ҽ����˼���	
			TGameStation.iGameCount		= m_iGameCount;		///��ǰ�Ѿ������˼���
			TGameStation.i64NtWin		= m_iNtWin;			///ׯ����Ӯ�ɼ�
			TGameStation.i64MaxNote		= m_iMaxNote;		///������Ϸ�����ע��

			TGameStation.i64ShangZhuangLimit	= m_iShangZhuangLimit;			///��ׯ��Ҫ�����ٽ��

			///������ע����
			memcpy(TGameStation.i64RectNoteMax,m_iRectNoteMax,sizeof(m_iRectNoteMax));
			// ���������ֵ
			memcpy(TGameStation.i64ChoumaValue,G_i64ChoumaValue,sizeof(G_i64ChoumaValue));

			//��ׯ�б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				TGameStation.idzhuangstation[i][0] = m_idzhuangstation[i][0];
				TGameStation.idzhuangstation[i][1] = m_idzhuangstation[i][1];
			}
			//·����Ϣ
			for(int i=0;i<GAME_SAVE_COUNT;i++)
			{
				for(int j=0;j<MAX_NOTE_COUNT;j++)
				{
					TGameStation.iResultInfo[i][j] = m_iResultInfo[i][j];
				}
			}

			TGameStation.iZhongZhu = m_iZhongZhu;			//���ѵ�ǰ��ע��
			for(int i=0;i<MAX_NOTE_COUNT;i++)
			{
				TGameStation.iMyZhu[i]		= m_iUserXiaZhuData[bDeskStation][i];	//����������ע
				TGameStation.iQuYuZhu[i]	= m_iQuYuZhu[i];						///����ÿ�������µ�ע��
				TGameStation.iQuYuZhuTrue[i]= m_iQuYuZhuTrue[i];					///������ʵÿ�������µ�ע��

				TGameStation.iComputePoint[i] = m_iComputePoint[i];
			}
			//������
			for(int i=0; i<5; i++)
			{
				for(int j=0; j<5; j++)
				{
					TGameStation.byCard[i][j] = m_UserCard[i][j];
				}
				TGameStation.byCardCount[i] = m_CardCount[i];
			}
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			return true;
		}
	case GS_PLAY_GAME:	//����״̬
		{
			TagGameStation_ShowResult TGameStation;
			TGameStation.byGameStaion	= m_bGameStation;		//��Ϸ״̬

			TGameStation.bMaxBase		= m_bCardShapeBase[10];	/// ����ʼ�ţţ�ı���

			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;	///��עʱ��
			TGameStation.iKaiPaiTime	= m_iKaiPaiTime;	///����ʱ��		
			TGameStation.iFreeTime		= m_iFreeTime;		///����ʱ��	
			TGameStation.iRemaindTime	= m_iRemaindTime;	///ʣ��ʱ��

			TGameStation.iNowNtStation	= m_iNowNtStation;	///��ǰׯ�ҵ�λ��
			TGameStation.iGameBeen		= m_iGameBeen;		///��ǰ����		
			TGameStation.iZhuangBaShu	= m_iZhuangBaShu;	///ׯ�ҽ����˼���	
			TGameStation.iGameCount		= m_iGameCount;		///��ǰ�Ѿ������˼���
			TGameStation.i64NtWin		= m_iNtWin;			///ׯ����Ӯ�ɼ�
			TGameStation.i64MaxNote		= m_iMaxNote;		///������Ϸ�����ע��

			TGameStation.i64ShangZhuangLimit	= m_iShangZhuangLimit;			///��ׯ��Ҫ�����ٽ��

			///������ע����
			memcpy(TGameStation.i64RectNoteMax,m_iRectNoteMax,sizeof(m_iRectNoteMax));
			// ���������ֵ
			memcpy(TGameStation.i64ChoumaValue,G_i64ChoumaValue,sizeof(G_i64ChoumaValue));

			//��ׯ�б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				TGameStation.idzhuangstation[i][0] = m_idzhuangstation[i][0];
				TGameStation.idzhuangstation[i][1] = m_idzhuangstation[i][1];
			}
			//·����Ϣ
			for(int i=0;i<GAME_SAVE_COUNT;i++)
			{
				for(int j=0;j<MAX_NOTE_COUNT;j++)
				{
					TGameStation.iResultInfo[i][j] = m_iResultInfo[i][j];
				}
			}

			TGameStation.iZhongZhu = m_iZhongZhu;			//���ѵ�ǰ��ע��
			for(int i=0;i<MAX_NOTE_COUNT;i++)
			{
				TGameStation.iMyZhu[i]		= m_iUserXiaZhuData[bDeskStation][i];	//����������ע
				TGameStation.iQuYuZhu[i]	= m_iQuYuZhu[i];						///����ÿ�������µ�ע��
				TGameStation.iQuYuZhuTrue[i]= m_iQuYuZhuTrue[i];					///������ʵÿ�������µ�ע��
			}
			//������
			for(int i=0; i<5; i++)
			{
				for(int j=0; j<5; j++)
				{
					TGameStation.byCard[i][j] = m_UserCard[i][j];
				}
				TGameStation.byCardCount[i] = m_CardCount[i];
			}

			TGameStation.iZhuangFen	= m_iZhuangFen;		///ׯ�ҵĵ÷�		
			TGameStation.iXianFen	= m_iXianFen;			///�мҵĵ÷�	
			TGameStation.iUserFen	= m_iUserFen[bDeskStation];			///��ǰ��ҵĵ÷�
			////��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return true;
		}
	default:
		{
			break;
		}
	}
	return true;
}

///
///������Ϸ״̬
///@param BYTE bLastStation ������
///@return bool �޾������� 
///
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
	
	return true;
}




///
///ɾ����ʱ��
///@param BYTE byTimerID ��ʱ��ID 
///@return ����bool �޾������� 
///
bool CServerGameDesk::KillGameTimer(BYTE byTimerID)
{
	//DebugPrintf("KillGameTimer__%d",byTimerID);
	return KillTimer(byTimerID);
}

///
///ɾ����Ϸ�����ж�ʱ��
///@param void
///@return void 
///
void  CServerGameDesk::KillAllTimer(void)
{
	KillTimer(TIMER_REMAIND_COUNT);
	KillTimer(ID_TIMER_XIA_ZHU);
	KillTimer(ID_TIMER_KAI_PAI);
	KillTimer(ID_TIMER_SHOW_RESULT);
	KillTimer(ID_TIMER_FREE_TIME);
}

///
///��Ӧ��ʱ����Ϣ
///@param UINT uTimerID
///@return bool �� ������ 
///
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch (uTimerID)
	{
	case TIMER_REMAIND_COUNT:	//ʣ��ʱ���ʱ��
		{
			m_iRemaindTime--;
			if (m_iRemaindTime <= 0)
			{
				KillTimer(TIMER_REMAIND_COUNT);
			}
			return true;
		}
	case ID_TIMER_XIA_ZHU://��עʱ�䵽
		{
			KillTimer(ID_TIMER_XIA_ZHU);
			OnSendKaiPai();//���Ϳ�ʼ���ơ�������Ϣ
			return true;
		}
	case ID_TIMER_KAI_PAI:		// ����ʱ��
		{
			KillTimer(ID_TIMER_KAI_PAI);
			//�������
			CountFen();
			return true;
		}
	case ID_TIMER_SHOW_RESULT:
		{
			KillTimer(ID_TIMER_SHOW_RESULT);
			//���Ϳ�����Ϣ
			OnSendFree();
			return true;
		}
	case ID_TIMER_FREE_TIME:		// ����ʱ�䵽�����Ϳ�����Ϣ
		{			
			KillTimer(ID_TIMER_FREE_TIME);
			StartGame();//��ʼ��Ϸ			
			return true;
		}
	}
	return __super::OnTimer(uTimerID);
}

///
///��Ӧ��Ϸ�Զ���ʱ����Ϣ
///@param UINT uTimerID
///@return bool �� ������ 
///
bool CServerGameDesk::OnGameTimer(BYTE byTimerID)
{
	//switch(byTimerID)
	//{
	//case ID_TIMER_SEND_CARD:		// ��������ʱ�䵽
	//	{
	//		// ���ͷ�������Ϣ,��ʱ��Ҫ������λ��֪ͨ��ҳ���
	//		//m_bGameStation = GS_PLAY_GAME;  // �ı���Ϸ״̬
	//		KillGameTimer(byTimerID);
	//		GAME_BEGIN sGameBegin;
	//		sGameBegin.byOutCardSeatID = GetDeskStation(m_byFirstOutPos);
	//		memcpy(&sGameBegin.abySeatID, &m_abySeatID, sizeof(m_abySeatID));
	//		int i=0;
	//		for (i=0;i<PLAY_COUNT; ++i)
	//		{
	//			if (NULL == m_pUserInfo[i])
	//			{
	//				continue;
	//			}
	//			SendGameData(i,&sGameBegin,sizeof(sGameBegin),MDM_GM_GAME_NOTIFY, ASS_GAME_BEGIN,0);
	//		}			
	//		return true;
	//	}
	//case ID_TIMER_OUT_CARD:
	//	{
	//		// ���û�г������Զ����� 
	//		return true;
	//	}
	//case ID_TIMER_GAME:
	//	{
	//		m_iCurrentMessage = ASS_WU_ZHUANG;
	//		KillGameTimer(ID_TIMER_GAME);
	//	
	//		return true;
	//	}
	//default:
	//	break;
	//}
	return true;
}
//--------------------------------------------------------------------------
void CServerGameDesk::ResetZhuangStation()
{
	short int iTmpQiang[PLAY_COUNT+1][2];
	short int iTmpZhuang[PLAY_COUNT+1][2];
	memset(&iTmpQiang, -1, sizeof(iTmpQiang));
	memset(&iTmpZhuang, -1, sizeof(iTmpZhuang));	
	int iZhuang = 0;	
	int iQiang = 0;
	for (int i= 2;i<PLAY_COUNT; i++)
	{
		iTmpQiang[i][0] = m_idzhuangstation[i][0];
		iTmpQiang[i][1] = m_idzhuangstation[i][1];
		iTmpZhuang[i][0] = m_idzhuangstation[i][0];
		iTmpZhuang[i][1] = m_idzhuangstation[i][1];
	}

	for (int i= 0;i<PLAY_COUNT; i++)
	{
		if (NULL != m_pUserInfo[iTmpQiang[i][0]] && iTmpQiang[i][1] == 1 && iTmpQiang[i][0] >= 0)
		{
			m_idzhuangstation[iQiang+2][0] = iTmpQiang[i][0];
			m_idzhuangstation[iQiang+2][1] = iTmpQiang[i][1];
			iQiang++;
		}
	}
	for (int j= 0;j<PLAY_COUNT; j++)
	{
		if (NULL != m_pUserInfo[iTmpZhuang[j][0]] && iTmpZhuang[j][0] >= 0 && iTmpZhuang[j][1] == -1)
		{
			m_idzhuangstation[iZhuang+iQiang+2][0] = iTmpZhuang[j][0];
			m_idzhuangstation[iZhuang+iQiang+2][1] = iTmpZhuang[j][1];
			iZhuang++;
		}
	}
	for ( int k = iZhuang+iQiang+2 ;k<PLAY_COUNT; k++)
	{
		m_idzhuangstation[k][0] = -1;
		m_idzhuangstation[k][1] = -1;
	}
	m_idzhuangstation[PLAY_COUNT][0] = iZhuang+iQiang+2;
	m_idzhuangstation[PLAY_COUNT][1] = -1;

}
//--------------------------------------------------------------------------------
//���������е����ݣ������õ�����Ԫ�����
//0Ϊ��ׯ���б������1Ϊ�Խ������ݲ���
void CServerGameDesk::MoveData(int type)
{
	if(type==0)//ׯ���б������ƶ�
	{  
		int num=0;
		for(int i=0;i<PLAY_COUNT;i++)
		{	
			if (NULL != m_pUserInfo[m_idzhuangstation[i][0]] && m_idzhuangstation[i][0]>=0)//����һ�����Ϸ��
			{
				if(m_pUserInfo[m_idzhuangstation[i][0]]->m_UserData.i64Money >= m_iShangZhuangLimit)//Ҫ���㹻��Ǯ
				{
					m_idzhuangstation[num][0] = m_idzhuangstation[i][0];
					m_idzhuangstation[num][1] = m_idzhuangstation[i][1];
					num++;
				}
			}
		}
		for(int i=num;i<PLAY_COUNT;i++)
		{
			m_idzhuangstation[i][0] = -1;
			m_idzhuangstation[i][1] = -1;
			m_idzhuangstation[PLAY_COUNT][0] = num;
		}
		//20121120dwj ����������ׯ�б�
		ResetZhuangStation();
	}
}

///
///��Ӧ�����ע��Ϣ
///@param BYTE Station ����
///@param void * pData ���ݰ�
///@param UINT uSize ���ݰ���С
///@return void
///
void CServerGameDesk::HandleXiaZhu(BYTE Station,void * pData, UINT uSize)
{

	if (-1 == m_iNowNtStation || NULL == m_pUserInfo[m_iNowNtStation])//��ׯ������ע
	{
		return;
	}
	TagXiaZhu *pXiaZhu = (TagXiaZhu *)pData;	
	if (pXiaZhu->iBetArea < 0 || pXiaZhu->iBetArea >= MAX_NOTE_COUNT || pXiaZhu->iChouMaType < 0 || pXiaZhu->iChouMaType >=6)
	{
		return;
	}
	__int64 i64NtMoney=0;
	i64NtMoney = m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;//��ȡׯ�ҵĽ���� 
	
	__int64 i64StationMoney = 0;	
	if (NULL != m_pUserInfo[Station])
	{
		i64StationMoney = m_pUserInfo[Station]->m_UserData.i64Money;
	}


	//��ע�����޲�������
	if (m_i64Max < 0 || (m_iZhongZhu +  G_i64ChoumaValue[pXiaZhu->iChouMaType] > m_i64Max
		|| (m_iUserXiaZhuData[Station][4] + G_i64ChoumaValue[pXiaZhu->iChouMaType])*m_bCardShapeBase[10] > i64StationMoney
		|| (0 < m_iMaxNote && (m_iUserXiaZhuData[Station][4]+G_i64ChoumaValue[pXiaZhu->iChouMaType]) > m_iMaxNote)
		|| (m_iRectNoteMax[pXiaZhu->iBetArea] > 0 && m_iQuYuZhu[pXiaZhu->iBetArea]+G_i64ChoumaValue[pXiaZhu->iChouMaType]) > m_iRectNoteMax[pXiaZhu->iBetArea]))//����������ע������������
	{
		return;
	}
	//��ǰ����ע������
	m_iZhongZhu += G_i64ChoumaValue[pXiaZhu->iChouMaType]; 
	//��������ע������
	m_iQuYuZhu[pXiaZhu->iBetArea] += G_i64ChoumaValue[pXiaZhu->iChouMaType];    
	m_iUserXiaZhuData[Station][pXiaZhu->iBetArea]	+= G_i64ChoumaValue[pXiaZhu->iChouMaType];    //����ڱ�������ע������
	m_iUserXiaZhuData[Station][MAX_NOTE_COUNT]		+= G_i64ChoumaValue[pXiaZhu->iChouMaType];    //�����ע�ܶ�����
	if (!pXiaZhu->bIsAi)
	{
		m_iQuYuZhuTrue[pXiaZhu->iBetArea] += G_i64ChoumaValue[pXiaZhu->iChouMaType];    //��������ע������
	}
	
	//���ͳɹ���Ϣ
	TagXiaZhu TXiaZhuResult;
	TXiaZhuResult.iBetArea	= pXiaZhu->iBetArea;
	TXiaZhuResult.iMoney	= G_i64ChoumaValue[pXiaZhu->iChouMaType];
	TXiaZhuResult.byStation = Station;
	TXiaZhuResult.iMaxZhu	= m_i64Max - m_iZhongZhu;
	TXiaZhuResult.iZhongZhu = m_iZhongZhu;
	for(int i=0;i<MAX_NOTE_COUNT;i++)
	{
		TXiaZhuResult.iQuYuZhu[i]		= m_iQuYuZhu[i];
		TXiaZhuResult.iQuYuZhuTrue[i]	= m_iQuYuZhuTrue[i];
		TXiaZhuResult.iUserXiaZhu[i]	= m_iUserXiaZhuData[Station][i];
	}
	//������ע
	TXiaZhuResult.iUserXiaZhu[MAX_NOTE_COUNT] = m_iUserXiaZhuData[Station][MAX_NOTE_COUNT];
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL==m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&TXiaZhuResult,sizeof(TXiaZhuResult),MDM_GM_GAME_NOTIFY, ASS_XIA_ZHU,0);//���ͳɹ���Ϣ	
	}
}
//------------------------------------------------------------------------------------------------------------------------

///
///��Ӧ�������ׯ��Ϣ
///@param BYTE Station ����
///@param void * pData ���ݰ�
///@param UINT uSize ���ݰ���С
///@return void
///
void CServerGameDesk::HanleSXzhuang(BYTE Station,void * pData, UINT uSize)
{
	SHANG_ZHUANG *pdata = (SHANG_ZHUANG *)pData;
	bool sucess = false;//�Ƿ��ܳɹ�
	if(!pdata->shang && pdata->bIsAi && Station==m_iNowNtStation)
	{
		if(GS_FREE!=m_bGameStation)//���н׶���ׯ�Ļ����˲ſ���������ׯ
		{
			return;
		}
	}
	if(NULL==m_pUserInfo[Station])
	{
		return;
	}
	//������ׯ��Ϣ
	if(pdata->shang)//��ׯ����
	{	
		MoveData(0);//��������
		
		if( m_iNowNtStation<0 && m_idzhuangstation[0][0]<0)//��ǰû��ׯ��,�б�Ϊ��
		{
			if( m_pUserInfo[Station]->m_UserData.i64Money >= m_iShangZhuangLimit && m_pUserInfo[Station]->m_UserData.i64Money<150000000000000000)//��ҷ�����ׯ����
			{
				m_iNowNtStation = Station;//������������ׯ
				m_iZhuangBaShu = 0;   //��ʼ��ׯ�ҵİ��� 
				m_iNtWin = 0;        //��ʼ��ׯ��Ӯ�Ľ��
				m_bXiaZhuang = false;	//��ǰׯ��������ׯ
		          
				StartGame();//��ʼ��Ϸ	
			}
		}
		else //��ǰ��ׯ�ҾͰ������߷ŵ�������
		{
			if(m_iNowNtStation != Station)//��ǰׯ�Ҳ���������ׯ
			{   
				bool bIsList=false;
				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_idzhuangstation[i][0] == Station)
					{
						//���б��е���Ҳ�������
						bIsList=true;
						break;
					}
				}
				if(!bIsList)
				{
					for(int i=0;i<PLAY_COUNT;i++)
					{
						if(m_idzhuangstation[i][0]<0)
						{
							m_idzhuangstation[i][0] = Station;
							m_idzhuangstation[i][1] = -1;
							m_idzhuangstation[PLAY_COUNT][0]++;//��ׯ������++;
							sucess = true;
							break;
						}
					}
				}
			}
		}
		if(sucess)//�ɹ��������
		{
			SHANG_ZHUANG szhuang;
			szhuang.shang = true;
			szhuang.station = Station;
			szhuang.success = true;
			szhuang.dzhuangstation[PLAY_COUNT][0] = m_idzhuangstation[PLAY_COUNT][0];

			for(int i=0;i<PLAY_COUNT;i++)
			{
				szhuang.dzhuangstation[i][0] = m_idzhuangstation[i][0];
				szhuang.dzhuangstation[i][1] = m_idzhuangstation[i][1];
			}
			for (int i=0;i<PLAY_COUNT; i++)
			{
				if (NULL != m_pUserInfo[i])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
				{
					SendGameData(i,&szhuang,sizeof(szhuang),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//���ͳɹ���Ϣ
				}
			}
		}
	}
	else//��ׯ����
	{
		if(Station == m_iNowNtStation)//��ǰׯ�Ҳ���������ׯ
		{
			m_bXiaZhuang = true;
			sucess = true;
		}
		else
		{
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_idzhuangstation[i][0]==Station)
				{
					m_idzhuangstation[i][0] = -1;
					m_idzhuangstation[i][1] = -1; 
					m_idzhuangstation[PLAY_COUNT][0]--;
					sucess = true;
					break;
				}
			}
		}
		MoveData(0);//��������
		if(sucess)
		{
			//������ׯ��Ϣ
			SHANG_ZHUANG szhuang;
			szhuang.shang = false;
			szhuang.station = Station;
			szhuang.success = true;

			for(int i=0;i<=PLAY_COUNT;i++)
			{
				szhuang.dzhuangstation[i][0] = m_idzhuangstation[i][0];
				szhuang.dzhuangstation[i][1] = m_idzhuangstation[i][1];
			}
			for (int i=0;i<PLAY_COUNT; i++)
			{
				if (NULL != m_pUserInfo[i])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
				{
					SendGameData(i,&szhuang,sizeof(szhuang),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//���ͳɹ���Ϣ
				}
			}
		}
	}
}	


///
///����
///@param [in] BYTE bDestStation ����1
///@param [in] BYTE bWinStation ����2����λ
///@return void
///
void CServerGameDesk::ChangeCard(BYTE bDestStation,BYTE bWinStation)
{
	if (bDestStation < 5 && bWinStation <5)
	{
		BYTE TempCard[5]={0};
		memcpy(TempCard, m_UserCard[bWinStation], sizeof(TempCard));
		memcpy(m_UserCard[bWinStation], m_UserCard[bDestStation], sizeof(TempCard));
		memcpy(m_UserCard[bDestStation], TempCard, sizeof(TempCard));
	}
	
}

/*
1 �õ���Ӯ��� zht
* @param [in] win �õ������Ӯ��1Ϊ�õ�Ӯ�ң���1Ϊ���
* @return �õ�Ӯ�ƻ�����λ��
*/ 
BYTE CServerGameDesk::GetWiner(int win)
{
	BYTE Max = 0, Min = 0;
	for (int i=1; i<5; i++)
	{
		if (m_PokerGameLogic.CompareCard(m_UserCard[Max],5,m_UserCard[i],5) < 0)
			Max = i;
		if (m_PokerGameLogic.CompareCard(m_UserCard[Min],5,m_UserCard[i],5) > 0)
			Min = i;
    }
	if (win == 1)
		return Max;
	else 
		return Min;
}

/*------------------------------------------------------------------------------*/
//�Ƿ񳬶����
bool	CServerGameDesk::IsSuperUser(BYTE bDeskStation)
{
	if (bDeskStation >= PLAY_COUNT || NULL == m_pUserInfo[bDeskStation])
	{
		return false;
	}

	for(int i=0; i<m_vlSuperUserId.size(); i++)
	{
		if (m_vlSuperUserId.at(i) == m_pUserInfo[bDeskStation]->m_UserData.dwUserID)
		{
			return true;
		}
	}
	return false;
}
/*------------------------------------------------------------------------------*/
/*
1 �õ�����Ƿ���Կ�����Ӯ
* @param [in] BYTE bDeskStation ���Ƶ����
* @return ��λ������Ƿ���Կ���
*/ 
bool CServerGameDesk::CanControlWin(BYTE bDeskStation)
{
	if (bDeskStation >= PLAY_COUNT || NULL == m_pUserInfo[bDeskStation])
	{
		return FALSE;
	}
	

	long iID = 0;
	
	// �������жϸ�����Ƿ��п��Ƶ�Ȩ��
	TCHAR skin[MAX_NAME_INFO];
	
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER + _T("_s.ini"));
	CString key = TEXT("superUserConfig");
	int count = f.GetKeyVal(key,"SuperUserCount",0);
	TCHAR szName[21];
	if(count <= 0 )					//��ʤ��
	{
		return FALSE;
	}

	for(int i = 1; i <= count; i ++)
	{
		wsprintf(szName,"SuperUserID%d",i);
			
		iID = f.GetKeyVal(key,szName,10004);
	
		if (iID != m_pUserInfo[bDeskStation]->m_UserData.dwUserID)
			continue;
		return TRUE;
	}
	return FALSE;
}

/*
1 ������Ӯ zht
* @param [in] BYTE bDeskStation ���Ƶ����
* @param [in] BYTE bGameStation ����ҪӮ��λ��
* @return �õ�Ӯ�ƻ�����λ��
*/ 
void CServerGameDesk::ControlWin(BYTE bDeskStation, BYTE bGameStation, BYTE BGameWin[])
{
	if (!CanControlWin(bDeskStation))
	{
		return;
	}
	for (int i=1; i<=MAX_NOTE_COUNT; i++)
	{
		if (BGameWin[i-1] == 255)
		{
			continue;
		}
		for (int j=i+1; j<=MAX_NOTE_COUNT; j++)
		{
			if (BGameWin[j-1] == 255)
			{
				continue;
			}
			if (BGameWin[i-1] == 0) //�졢�ء������� ��
			{
				if(m_PokerGameLogic.CompareCard(m_UserCard[i], m_CardCount[i],
					m_UserCard[j],m_CardCount[j]) > 0)
				{
					ChangeCard(i,j);
				}
			}
			else if (BGameWin[i-1] > 0 && BGameWin[i-1] != 255) //�졢�ء������� Ӯ
			{
				if(m_PokerGameLogic.CompareCard(m_UserCard[i], m_CardCount[i],
					m_UserCard[j],m_CardCount[j]) < 0)
				{
					ChangeCard(i,j);
				}
			}
		}
	}
	
	///���Ų�Ҫ��С��ׯ�ҵ�
	for (int i=0; i<MAX_NOTE_COUNT; i++)
	{
		if (BGameWin[i] == 0)
		{
			if(m_PokerGameLogic.CompareCard(m_UserCard[i+1], m_CardCount[i+1],
				m_UserCard[0],m_CardCount[0]) > 0)
			{
				ChangeCard(0,i+1);
			}
		}
	}

	///�ٲ����ׯ�ҵ�
	for (int i=0; i<MAX_NOTE_COUNT; i++)
	{
		if (BGameWin[i] == 1)
		{
			if(m_PokerGameLogic.CompareCard(m_UserCard[i+1], m_CardCount[i+1],
				m_UserCard[0],m_CardCount[0]) < 0)
			{
				ChangeCard(0,i+1);
			}
		}
	}


	return;
}

/*
1 �������ҵ���Ӯ����
* @return void
*/ 
void CServerGameDesk::GetWin()
{
	memset(&m_iComputePoint,0, sizeof(m_iComputePoint));
	for (int i=1; i<5; i++)
	{
		m_iComputePoint[i-1] = ComputePoint(i); //�õ���Ӯ����
	}
}
/*
1 ������Ӯ���� zht
* @param [in] BYTE bDeskStation Ҫ��������
* @return �õ�Ӯ�䱶��
*/ 
int CServerGameDesk::ComputePoint(BYTE DeskStation)
{ 
	int Point = 0;
	int winer = -1;

	if(m_PokerGameLogic.CompareCard(m_UserCard[DeskStation], m_CardCount[DeskStation],m_UserCard[0],m_CardCount[0]) > 0)
	{
		winer = DeskStation;
		Point = 1;
	}
	else
	{
		winer = 0;
		Point = -1;
	}
			
	
	int Shape = m_PokerGameLogic.GetShape(m_UserCard[winer],m_CardCount[winer]);
	switch(Shape)
	{
	case EBCT_TYPE_BULL_joker:
	case EBCT_TYPE_BULL_JOKER:
	case EBCT_TYPE_BULL_BULL:
		Point = Point * m_bCardShapeBase[10];
		break;
	case EBCT_TYPE_BULL_NINE:
		Point = Point * m_bCardShapeBase[9];
		break;
	case EBCT_TYPE_BULL_EIGHT:
		Point = Point * m_bCardShapeBase[8];
		break;
	case EBCT_TYPE_BULL_SEVEN:
		Point = Point * m_bCardShapeBase[7];
		break;
	case EBCT_TYPE_BULL_SIX:
		Point = Point * m_bCardShapeBase[6];
		break;
	case EBCT_TYPE_BULL_FIVE:
		Point = Point * m_bCardShapeBase[5];
		break;
	case EBCT_TYPE_BULL_FOUR:
		Point = Point * m_bCardShapeBase[4];
		break;
	case EBCT_TYPE_BULL_THREE:
		Point = Point * m_bCardShapeBase[3];
		break;
	case EBCT_TYPE_BULL_TWO:
		Point = Point * m_bCardShapeBase[2];
		break;
	case EBCT_TYPE_BULL_ONE:
		Point = Point * m_bCardShapeBase[1];
		break;
	case EBCT_TYPE_NO_POINT:
		Point = Point * m_bCardShapeBase[0];
		break;
	default:
		break;
	}
	return Point;
}
/*-----------------------------------------------------------------*/
///����
void	CServerGameDesk::SendCard()
{
	//�ַ��˿���
	for(int j = 0 ; j<5 ;j++)
	{
		for(int i = 0 ;i<5;i++)
		{
			m_UserCard[i][j] =  m_UserCardTotal[i*5+j];				
		}
		m_CardCount[j] = 5;
	}

}
/*-----------------------------------------------------------------*/
//���Ϳ�����Ϣ
void	CServerGameDesk::OnSendFree()
{
	GameFinish(0,GF_NORMAL);//����������Ϸ
	m_bGameStation = GS_FREE;// ����״̬
	MoveData(0);//����ׯ���б�
	ClearNt();//����ׯ��
	
	DENG_DAI Wait;
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		Wait.dzhuangstation[i][0] = m_idzhuangstation[i][0];
		Wait.dzhuangstation[i][1] = m_idzhuangstation[i][1];
	}
	Wait.iZhuang		= m_iNowNtStation;	//ׯ��λ��
	Wait.iNtWin			= m_iNtWin;			//��ǰׯ��Ӯ�Ľ��
	Wait.iZhuangBaShu	= m_iZhuangBaShu;	//ׯ�ҽ����˼���

	//���Ϳ���ʱ����Ϣ
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
			continue;
		SendGameData(i,&Wait,sizeof(Wait),MDM_GM_GAME_NOTIFY, ASS_FREE_TIME,0);
	}
	//ʣ��ʱ���ʱ��
	SetTimeRemaindCount(m_iFreeTime);
	SetTimer(ID_TIMER_FREE_TIME, m_iFreeTime*1000);
}
/*--------------------------------------------------------------------------------*/
//���Ϳ�ʼ���ơ�������Ϣ
void	CServerGameDesk::OnSendKaiPai()
{
	m_bGameStation = GS_SEND_CARD;//��Ϸ����״̬��
	//ϴ��
	m_PokerGameLogic.RandCard(m_UserCardTotal,54,m_bHaveKing);
	m_PokerGameLogic.GetCompareValue(m_bCompareKind) ; //�Ƿ�Ƚϻ�ɫ
	SendCard();

	//���˿�����
	if (m_TSuperSet.bSuccesse)
	{
		SuperUserControl();
	}
	else if (m_bAIWinAndLostAutoCtrl)
	{
		IAWinAutoCtrl();
	}

//===================================================================
	//������
	//m_UserCard[0][0] = 0x0B;
	//m_UserCard[0][1] = 0x2D;
	//m_UserCard[0][2] = 0x3A;
	//m_UserCard[0][3] = 0x37;
	//m_UserCard[0][4] = 0x27;

	//m_UserCard[1][0] = 0x34;
	//m_UserCard[1][1] = 0x24;
	//m_UserCard[1][2] = 0x29;
	//m_UserCard[1][3] = 0x33;
	//m_UserCard[1][4] = 0x07;

	//m_UserCard[2][0] = 0x2C;
	//m_UserCard[2][1] = 0x28;
	//m_UserCard[2][2] = 0x3D;
	//m_UserCard[2][3] = 0x0A;
	//m_UserCard[2][4] = 0x02;

	//m_UserCard[3][0] = 0x15;
	//m_UserCard[3][1] = 0x11;
	//m_UserCard[3][2] = 0x12;
	//m_UserCard[3][3] = 0x22;
	//m_UserCard[3][4] = 0x17;

	//m_UserCard[4][0] = 0x3B;
	//m_UserCard[4][1] = 0x05;
	//m_UserCard[4][2] = 0x04;
	//m_UserCard[4][3] = 0x36;
	//m_UserCard[4][4] = 0x1A;
//===================================================================


	GetWin();  //��ȡ����ҵ���Ӯ����
		
	srand((unsigned)time(NULL));
	//��¼·����Ϣ
	GameLuziInfo	TGameLuziInfo;
	for(int i=0; i<MAX_NOTE_COUNT; i++)
	{
		TGameLuziInfo.iResultInfo[i] = m_iComputePoint[i]; //��������Ӯ
	}
	if (m_dqResultInfo.size()<GAME_SAVE_COUNT)
	{
		m_dqResultInfo.push_back(TGameLuziInfo);
	}
	else
	{
		m_dqResultInfo.pop_front();
		m_dqResultInfo.push_back(TGameLuziInfo);
	}

	for(int i=0; i<m_dqResultInfo.size();i++)
	{
		for(int j=0; j<MAX_NOTE_COUNT;j++)
		{
			m_iResultInfo[i][j] = m_dqResultInfo.at(i).iResultInfo[j];
		}
	}

	

	KAI_PAI KaiPaiData;
	//������
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			KaiPaiData.pai[i][j] = m_UserCard[i][j]; 
		}
		KaiPaiData.iCardCount[i] = m_CardCount[i]; //����
	}
	
	//��ȡӮ��������
	for(int i=0;i<MAX_NOTE_COUNT;i++)
	{
		KaiPaiData.iComputePoint[i] = m_iComputePoint[i];	
	}

		

	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
			continue;
	
		SendGameData(i,&KaiPaiData,sizeof(KaiPaiData),MDM_GM_GAME_NOTIFY, ASS_KAI_PAI,0);//���Ϳ�����Ϣ
	}
	//ʣ��ʱ���ʱ��
	SetTimeRemaindCount(m_iKaiPaiTime);
	//�������Ľ���������
	SetTimer(ID_TIMER_KAI_PAI,(m_iKaiPaiTime)*1000);
}
/*--------------------------------------------------------------------------------*/
///�������
void	CServerGameDesk::CountFen()
{
	//��Ϸ������
	m_bGameStation = GS_PLAY_GAME;
	memset(&m_iUserFen, 0, sizeof(m_iUserFen)); //��������
	m_iXianFen = 0;
	m_iZhuangFen = 0;	

    bool temp_cut[PLAY_COUNT];
	memset(&temp_cut, 0, sizeof(temp_cut)); //ׯ���б��ܵ�ׯ��λ��
    //д�����ݿ�
	///2010-9-27 �޸�̨�ѿ۷����⣬û��ע���˵�ׯʱ�����˶�����ע����̨��
	CGameUserInfo	* pUserInfoTemp[MAX_PEOPLE];
	memcpy(pUserInfoTemp,m_pUserInfo, sizeof(m_pUserInfo));
	BOOL bHaveNote = FALSE; //������ע��
	//ׯ���⸶ÿ�����
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(i==m_iNowNtStation)//ׯ�Ҳ������
			continue;
		if(NULL == m_pUserInfo[i])//��Ҳ����ھͲ��ü���
			continue;
		if(m_iUserXiaZhuData[i][4] == 0)//���û����ע�Ͳ��ü���
			continue;
		__int64 win=0;
		__int64 zhu=0;
		bHaveNote = TRUE; 
		for(int j=0;j<MAX_NOTE_COUNT;j++)
		{
			win += (m_iUserXiaZhuData[i][j]*m_iComputePoint[j]);//���Ӯ�Ľ��
		}		

		m_iUserFen[i] = win;//(win-zhu);//Ӯ��-�µ�ע������ұ��ֵľ�Ӯ�����
		m_iXianFen += m_iUserFen[i];//�мҵ��ܷ�
		if (-1 != m_iNowNtStation)
		{
			m_iUserFen[m_iNowNtStation] -= win;//ׯ��-���Ӯ�ķ֡�
		}
	}

	if (m_bNtTax == 0)
	{
		ChangeUserPointint64(m_iUserFen, temp_cut, m_iNowNtStation);
	}
	else
	{
		ChangeUserPointint64(m_iUserFen, temp_cut);
	}

	//���ڿ�˰֮ǰ��¼ ���� ȫ�ǻ����˵������ ��˰����������ƽ�� ���½���Ϊ����
	if (m_bAIWinAndLostAutoCtrl)
	{
		CString sTemp = CINIFile::GetAppPath ();/////����·��
		CINIFile f(sTemp +SKIN_FOLDER  + _T("_s.ini"));
		TCHAR szSec[_MAX_PATH] = TEXT("game");
		_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);
		m_iAIHaveWinMoney=f.GetKeyVal(szSec,"ReSetAIHaveWinMoney ",0);
		__int64 i64Temp=0;
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(NULL != m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual)///��������Ӯ���ƣ��Ƿ�����������Ӯ�Զ�����
			{
				i64Temp += m_iUserFen[i];	///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���Ǯ
			}
		}
		//����ǰ������Ӯ��Ǯд�������ļ�����
		m_iAIHaveWinMoney += i64Temp;
		sTemp.Format("%I64d",m_iAIHaveWinMoney);
		f.SetKeyValString(szSec,"ReSetAIHaveWinMoney ",sTemp);
	}


	__super::RecoderGameInfo(m_iUserMoney);

	tagResult TResult;
	//·����Ϣ
	for(int i=0;i<GAME_SAVE_COUNT;i++)
	{
		for(int j=0;j<MAX_NOTE_COUNT;j++)
		{
			TResult.iResultInfo[i][j] = m_iResultInfo[i][j];
		}
	}

	if (-1 != m_iNowNtStation)
	{
		m_iZhuangFen = m_iUserMoney[m_iNowNtStation];     //ׯ�ҵĵ÷֣�����
	}
	else
	{
		m_iZhuangFen = 0;
	}

	m_iNtWin += m_iZhuangFen;  ///����ׯ�ҵ��ܳɼ�
	if (m_iNowNtStation != -1)
	{
		TResult.iNtWin = m_iNtWin;              //��ǰׯ��Ӯ�Ľ��
	}
	else
	{
		TResult.iNtWin = 0;              //��ǰׯ��Ӯ�Ľ��
	}
	TResult.iZhuangFen = m_iZhuangFen;
	TResult.iXianFen = m_iXianFen;
	Sleep(500);
	for (int i=0; i<PLAY_COUNT; i++)
	{
		TResult.iUserFen = m_iUserMoney[i];
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}

		SendGameData(i,&TResult,sizeof(TResult),MDM_GM_GAME_NOTIFY, ASS_RESULT,0);//���ͳɹ���Ϣ
	}
	memcpy(m_pUserInfo,pUserInfoTemp,sizeof(m_pUserInfo));	

	//ʣ��ʱ���ʱ��
	SetTimeRemaindCount(5);
	SetTimer(ID_TIMER_SHOW_RESULT, 5*1000);
}
//-----------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
///һ�ֽ��������˿���ʱ������Ƿ��ܹ�������ʼ��Ϸlym0512
bool CServerGameDesk::CheckCanBegin()
{
	return false;
}
///ׯ����ׯ���ȡ��һ��ׯ���б��е�ׯ��
bool	CServerGameDesk::GetNextNT()
{
	if(-1!=m_iNowNtStation && m_pUserInfo[m_iNowNtStation])
	{
		return false;
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_idzhuangstation[i][0]>=0 && NULL!=m_pUserInfo[m_idzhuangstation[i][0]])
		{
			m_iNowNtStation = m_idzhuangstation[i][0];
			m_idzhuangstation[i][0] = -1;
			m_idzhuangstation[i][1] = -1;
			m_idzhuangstation[PLAY_COUNT][0]--;
			m_iZhuangBaShu = 0;   //��ʼ��ׯ�ҵľ���
			m_iNtWin =0;        //��ʼ��ׯ��Ӯ�Ľ��
			m_bXiaZhuang=false;			//��ǰׯ��������ׯ
			MoveData(0);
			return true;
		}
	}
	return false;
}
///ͨ��ȫ�����ֵķ�ʽ��ʼ��Ϸlym0512
bool CServerGameDesk::StartGame()
{	 
	KillAllTimer();
	ClearNt(); //ׯ���������
	MoveData(0);
	if (-1==m_iNowNtStation || NULL==m_pUserInfo[m_iNowNtStation])
	{
		GetNextNT();	
		if(-1!=m_iNowNtStation && m_pUserInfo[m_iNowNtStation])//��ȡׯ�ҳɹ�
		{
			m_bIsStart=true;
			GameBegin(ALL_ARGEE);;//��ʼ��Ϸ	
			return true;
		}	
		else
		{
			//������ׯ�ȴ���Ϣ				
			DENG_DAI Wait;
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_idzhuangstation[i][0]=-1;
				m_idzhuangstation[i][1]=-1;
				if (i == PLAY_COUNT)
				{
					m_idzhuangstation[i][0] = 0;
				}
				Wait.dzhuangstation[i][0] = m_idzhuangstation[i][0];
				Wait.dzhuangstation[i][1] = m_idzhuangstation[i][1];
			}
			Wait.iNtWin = 0;              //��ǰׯ��Ӯ�Ľ��
			Wait.iZhuangBaShu = 0;        //ׯ�ҽ����˼���
			for (int i=0;i<PLAY_COUNT; i++)
			{
				if (NULL != m_pUserInfo[i])
				{
					SendGameData(i,&Wait,sizeof(Wait),MDM_GM_GAME_NOTIFY, ASS_WU_ZHUANG,0);////������ׯ�ȴ���Ϣ
				}			
			}
			return false;
		}
	}
	else
	{
		m_bIsStart=true;
		GameBegin(ALL_ARGEE);;//��ʼ��Ϸ	
		return true;
	}
}
///��¼���ֵĳɼ�����ym0512
void CServerGameDesk::RecordData()
{
	if(m_iResultInfo[GAME_SAVE_COUNT-1][0]!=0)
	{		
		m_iResultInfo[GAME_SAVE_COUNT-1][0] = m_iComputePoint[0]; //��������Ӯ
		m_iResultInfo[GAME_SAVE_COUNT-1][1] = m_iComputePoint[1]; //���ֵ���Ӯ
		m_iResultInfo[GAME_SAVE_COUNT-1][2] = m_iComputePoint[2]; //��������Ӯ
		m_iResultInfo[GAME_SAVE_COUNT-1][3] = m_iComputePoint[3]; //���ֻ���Ӯ
	}
	else
	{
		for(int i=0;i<GAME_SAVE_COUNT;i++)
		{
			if(m_iResultInfo[i][0]==0)
			{
				m_iResultInfo[i][0] = m_iComputePoint[0]; //��������Ӯ
				m_iResultInfo[i][1] = m_iComputePoint[1]; //���ֵ���Ӯ
				m_iResultInfo[i][2] = m_iComputePoint[2]; //��������Ӯ
				m_iResultInfo[i][3] = m_iComputePoint[3]; //���ֻ���Ӯ
				break;
			}
		}
	}
}
/*------------------------------------------------------------------------------------*/
///��ʼ���󲿷����ݣ���Щ���ݲ��ܳ�ʼ���ģ�
void	CServerGameDesk::ServerInit()
{
	m_iZhongZhu		= 0;				//���ѵ�ǰ��ע��
	m_iZhuangFen	= 0;				//ׯ�ҵĵ÷֣�����
	m_iXianFen		= 0;				//�мҵĵ÷֣�����
	m_bXiaZhuang	= false;			//��ǰׯ��������ׯ	
	m_TSuperSet.init();


			
	memset(&m_CardCount, 0, sizeof(m_CardCount));	//ׯ��˫��������
	memset(&m_UserCard, 0, sizeof(m_UserCard));		//��ҵ���
	memset(&m_iUserFen, 0, sizeof(m_iUserFen));		//������ұ��ֵĵ÷�
	memset(&m_iUserMoney, 0, sizeof(m_iUserMoney));	//������ұ��ֵĵ÷�
	memset(&m_iQuYuZhu, 0, sizeof(m_iQuYuZhu));		//����ÿ�������µ�ע��
	memset(&m_iQuYuZhuTrue, 0, sizeof(m_iQuYuZhuTrue));	//����ÿ�������µ�ע��
	memset(&m_iUserXiaZhuData, 0, sizeof(m_iUserXiaZhuData));	//����ÿ�������µ�ע��
	memset(&m_iComputePoint,0, sizeof(m_iComputePoint));

	
}
/*------------------------------------------------------------------------------------*/
//lym������Ϸ�ܷ�ʼ����������ֹ��ׯ�ļٿ�ʼ
bool CServerGameDesk::CanBeginGame()
{
	if(m_iNowNtStation<0)
		return false;
	return __super::CanBeginGame();
}
///��ѯ�Ƿ������ע˫��
BOOL  CServerGameDesk::CanDouble(BYTE bDeskStation)
{
	return FALSE;
}
///��־��¼�ӿ�
void  CServerGameDesk::SrvTestlog(TCHAR *pMessage)
{
	return ;	// ������ȥ����¼

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
//���˿���
void	CServerGameDesk::SuperUserControl()
{
	int iMaxRankList[5]={0};

	//�Ƚ����ʹ�С����
	//��һ�����
	int iMax1 = 0;
	for(int i=1; i<5; i++)
	{
		if(m_PokerGameLogic.CompareCard(m_UserCard[i], m_CardCount[i],m_UserCard[iMax1],m_CardCount[iMax1]) > 0)
		{
			iMax1 = i;
		}
	}
	//��2�����
	int	iMax2 = -1;
	for(int i=0; i<5; i++)
	{
		if(i == iMax1)
		{
			continue;
		}
		if (-1 == iMax2)
		{
			iMax2 = i;
			continue;
		}
		if(m_PokerGameLogic.CompareCard(m_UserCard[i], m_CardCount[i],m_UserCard[iMax2],m_CardCount[iMax2]) > 0)
		{
			iMax2 = i;
		}
	}
	//��3�����
	int	iMax3 = -1;
	for(int i=0; i<5; i++)
	{
		if(i == iMax1 || i == iMax2)
		{
			continue;
		}
		if (-1 == iMax3)
		{
			iMax3 = i;
			continue;
		}
		if(m_PokerGameLogic.CompareCard(m_UserCard[i], m_CardCount[i],m_UserCard[iMax3],m_CardCount[iMax3]) > 0)
		{
			iMax3 = i;
		}
	}
	//��4�����
	int	iMax4 = -1;
	for(int i=0; i<5; i++)
	{
		if(i == iMax1 || i == iMax2 || i == iMax3)
		{
			continue;
		}
		if (-1 == iMax4)
		{
			iMax4 = i;
			continue;
		}
		if(m_PokerGameLogic.CompareCard(m_UserCard[i], m_CardCount[i],m_UserCard[iMax4],m_CardCount[iMax4]) > 0)
		{
			iMax4 = i;
		}
	}

	//��5�����
	int	iMax5 = -1;
	for(int i=0; i<5; i++)
	{
		if(i == iMax1 || i == iMax2 || i == iMax3 || i == iMax4)
		{
			continue;
		}
		if (-1 == iMax5)
		{
			iMax5 = i;
			continue;
		}
	}

	//�Ƿ�ͨ��
	if (-1 == m_TSuperSet.iAreaContrl[0] && -1 == m_TSuperSet.iAreaContrl[1] && -1 == m_TSuperSet.iAreaContrl[2] && -1 == m_TSuperSet.iAreaContrl[3])
	{
		//ׯ�Һ�������һ��ƾͿ�����
		if (iMax1 != 0)
		{
			ChangeCard(iMax1,0);
		}
	}
	else if (-1 == m_TSuperSet.iAreaContrl[0] && -1 == m_TSuperSet.iAreaContrl[1] && -1 == m_TSuperSet.iAreaContrl[2] && -1 == m_TSuperSet.iAreaContrl[3])	//�Ƿ�ͨ��
	{
		if (iMax5 != 0)
		{
			ChangeCard(iMax5,0);
		}
	}
	else
	{
		BYTE byMaxRankCard[5][5];
		memset(byMaxRankCard,0,sizeof(byMaxRankCard));
		for(int j=0; j<5; j++)
		{
			byMaxRankCard[0][j] = m_UserCard[iMax1][j];
			byMaxRankCard[1][j] = m_UserCard[iMax2][j];
			byMaxRankCard[2][j] = m_UserCard[iMax3][j];
			byMaxRankCard[3][j] = m_UserCard[iMax4][j];
			byMaxRankCard[4][j] = m_UserCard[iMax5][j];
		}

		int iMaxIndex = 0;
		int iMinIndex = 4;
		for(int i=0;i<4;i++)
		{
			if (1 == m_TSuperSet.iAreaContrl[i])
			{
				memcpy(m_UserCard[i+1],byMaxRankCard[iMaxIndex],sizeof(m_UserCard[i+1]));
				memset(byMaxRankCard[iMaxIndex],0,sizeof(byMaxRankCard[iMaxIndex]));
				iMaxIndex++;
			}
			else
			{
				memcpy(m_UserCard[i+1],byMaxRankCard[iMinIndex],sizeof(m_UserCard[i+1]));
				memset(byMaxRankCard[iMinIndex],0,sizeof(byMaxRankCard[iMinIndex]));
				iMinIndex--;
			}
		}

		//����ׯ�ҵ���
		for(int i=0;i<5;i++)
		{
			if (0 != byMaxRankCard[i][0])
			{
				memcpy(m_UserCard[0],byMaxRankCard[i],sizeof(m_UserCard[0]));
				memset(byMaxRankCard[i],0,sizeof(byMaxRankCard[i]));
				break;
			}
		}
	}
}

///��������������Ӯ���ƣ������˸����趨���������һ������Ӯ���ʣ��û�������Ӯ�����ȶ�
void	CServerGameDesk::IAWinAutoCtrl()
{
	///���û�г����ͻ��˿��Ʋ��ҿ������Զ����ƣ�����л�������Ӯ�Զ�����
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
	{// �����˵�ӮǮ��0-A1���� ���Ҹ��ʷ���m_iAIWinLuckyAtA1 ������ҪӮ
		if (iResult <m_iAIWinLuckyAtA1)
			bAIWin = true;								
	}
	else if ((m_iAIWantWinMoneyA1 <= m_iAIHaveWinMoney)  && (m_iAIHaveWinMoney <m_iAIWantWinMoneyA2))
	{// �����˵�ӮǮ��A1-A2���� ���Ҹ��ʷ���m_iAIWinLuckyAtA2 ������ҪӮ
		if (iResult <m_iAIWinLuckyAtA2)
			bAIWin = true;			
	}
	else if ((m_iAIWantWinMoneyA2 <= m_iAIHaveWinMoney)  && (m_iAIHaveWinMoney <m_iAIWantWinMoneyA3))
	{// �����˵�ӮǮ��A2-A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA3 ������ҪӮ
		if (iResult <m_iAIWinLuckyAtA3)
			bAIWin = true;
	}
	else
	{// �����˵�ӮǮ����A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA4 ������ҪӮ
		if (iResult <m_iAIWinLuckyAtA4)
			bAIWin = true;
	}

	if(!bAIWin && (m_iAIHaveWinMoney+CountAIWinMoney() < 0))
	{//������Ҫ�䣬���ǻ���������֮��ӮǮΪ��������ô�����˱���Ӯ	
		bAIWin = true;
	}	

	if(bAIWin)
	{//������ҪӮǮ
		if(CountAIWinMoney()<0)
		{
			for(int i=0;i<5;i++)
			{
				ChangeCard(i+1,0);
				if(CountAIWinMoney()>=0)
				{
					break;
				}
			}
		}
	}
	else
	{///������Ҫ��Ǯ
		if (CountAIWinMoney() > 0 || (m_iAIHaveWinMoney +CountAIWinMoney())<0)
		{///������Ҫ��Ǯ//��������ӮǮ����0���ߵ��»�����ӮǮΪ��������Ҫ���·���
			for (int i=0;i<5;i++)
			{			
				ChangeCard(i+1,0);
				if (CountAIWinMoney()<0)
				{
					if ((m_iAIHaveWinMoney +CountAIWinMoney())<0)
					{//���������������Ǯ�� ���»�������ӮǮ��Ϊ��������ô���»���
						if (i != 4)
						{
							continue;
						}
						// �ƶ������˻��ǲ���������������ôֻ�����»���ȥ�û�����Ӯ��
						for (int j = 0; j < 5;j++)
						{
							ChangeCard(j+1,0);
							if (CountAIWinMoney() >=0)
							{
								break;
							}
						}		
					}
					break;
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------
///��������������Ӯ���ƣ����㵱ǰ�ƻ����˵���ӮǮ
///@return��������Ӯ��Ǯ������Ϊ��Ǯ
int CServerGameDesk::CountAIWinMoney()
{
	int money = 0;
	if (m_iNowNtStation!=-1 && m_pUserInfo[m_iNowNtStation])
	{
		if(m_pUserInfo[m_iNowNtStation]->m_UserData.isVirtual)
		{///�����������ׯ��,������ʵ��ҵ�Ǯ
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(NULL == m_pUserInfo[i] || m_pUserInfo[i]->m_UserData.isVirtual || m_iUserXiaZhuData[i][4] == 0 || i == m_iNowNtStation)//�����˲��ü��� || ��Ҳ����ھͲ��ü��� || ���û����ע�Ͳ��ü���
					continue;
				for(int j=0;j<4;j++)
				{			
					//money += m_iUserXiaZhuData[i][j];
					money -= (m_iUserXiaZhuData[i][j]*ComputePoint(j+1));
				}
			}
		}
		else
		{///��������˲���ׯ��,��������˵�Ǯ
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(NULL == m_pUserInfo[i] || !m_pUserInfo[i]->m_UserData.isVirtual  || i == m_iNowNtStation)
					continue;
				for(int j=0;j<4;j++)
				{			
					//money -= m_iUserXiaZhuData[i][j];
					money += (m_iUserXiaZhuData[i][j]*ComputePoint(j+1));
				}
			}
		}
	}
	return money;
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
