#include "StdAfx.h"
#include "clientgamedlg.h"
//#include "playcardsound.h"
#include <math.h>

//�Զ�����Ϣ��// To Be Continued..
enum MB_AFC_STYLE          //KT��Ϣ������
{
	AFC_CONFIRM = 500,       //ȷ�϶Ի���
	AFC_YESNO,               //��ʾ���ǡ������񡱶Ի���
	AFC_WARN                 //����Ի���
};
void DebugPrintf(const char *p, ...)
{
	char szFilename[256];
	sprintf( szFilename, "E:\\bzw\\DebugInfo\\AI_%d.txt", NAME_ID);
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
#define  SOUND_TYPE_CHIP  0
#define  SOUND_TYPE_WIN   1
#define  SOUND_TYPE_LOSE  2
#define  SOUND_TYPE_GROUP_OVER 3
BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGED()	// ���ڵ���Flash��������ڸ����ڵ�λ��
END_MESSAGE_MAP()
//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	m_bGameStation = GS_WAIT_SETGAME;           //��Ϸ״̬
	m_iBeginTime = 0;			//��Ϸ��ʼʱ��
	m_iThinkTime = 0;			//��Ϸ˼��ʱ��
	m_iEndWaitTime = 0;			//��Ϸ������ȴ�ʱ��
	m_iCurrentTimeId = 0;       //��ǰ��ʱ��ID
	m_bSound = TRUE;
	m_bInLisureRoom = FALSE;
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));			//��������ϵ��˿���
	::memset(m_iUserCard,0,sizeof(m_iUserCard));			//��������ϵ��˿�
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	::memset(m_iDeskCard,0,sizeof(m_iDeskCard));			//������˿�
	::memset(m_bUserReady,0,sizeof(m_bUserReady));
	::memset(m_bPlayer,0x00,sizeof(m_bPlayer));
	::memset(m_byUserOperatorType,0xff,sizeof(m_byUserOperatorType));
	::memset(m_i64UseAllNote,0x00,sizeof(m_i64UseAllNote));				//ÿ���û��µ���ע
	m_iAfterWaitAction = 1;		//��ʼ��Ϊ�Զ���ʼ 090311a1 JJ
	//���������ļ�
	LoadIniConfig();
}
//=========================================================================================
// ���� : ���������ļ�
//=========================================================================================
void CClientGameDlg::LoadIniConfig()
{
	CINIFile iniFile(CINIFile::GetAppPath()+"CXRobotSet.ini");

	CString strkey;
	strkey = "RobortSet";
	// �����˿��ٲ�������
	m_iBoboPercentage = iniFile.GetKeyVal("RobortSet", "BoboPercentage", 80);
}
//��������
CClientGameDlg::~CClientGameDlg()
{

}

//�����Ϸ�汾
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{
	return TRUE;
}

//��ʱ��
void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}
void CClientGameDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);
}

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_SET_BOBOTIME);
	KillTimer(ID_FOLLOW_TIME);
	KillTimer(ID_SET_ALL_TIME);
	KillTimer(ID_BIG_TIME);
	KillTimer(ID_STOP_TIME);
	KillTimer(ID_GIVE_UP_TIME);
	KillTimer(ID_SEPARATE_TIME);
	KillTimer(ID_BEGIN_TIME);
	return;
}

void CClientGameDlg::ShowStartButtons()
{
}

//�����˿˰�ť����  //թ�𻨴˹������� yjj 090505
LRESULT	CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	return 0;
}
// �ؼ���ʼ��ť�����
bool CClientGameDlg::OnControlHitBegin()
{
	if ((GetStationParameter()==GS_WAIT_SETGAME||GetStationParameter()==GS_WAIT_NEXT||GetStationParameter()==GS_WAIT_AGREE))
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}
	ResetGameStation(RS_GAME_BEGIN);
	KillAllTimer();
	return true;
}

//-------------------������λ�õ��ؼ�λ��----------
BYTE CClientGameDlg::GetViewStaionToCtr(BYTE bDeskStation)
{
	if (bDeskStation >= PLAY_COUNT)
	{
		return 255;
	}
	return (ViewStation(bDeskStation) + PLAY_COUNT - 1) % (PLAY_COUNT);
}
//---------------------------------------------------------------------------------
 bool CClientGameDlg::OnCloseGame()           //�ر���Ϸ
 {
	 TCHAR sz[200];	
	 KillAllTimer();
	  __super::OnCancel();
	 return true;
 }
 void CClientGameDlg::OnCancel()//{OnCloseGame(); __super::OnCancel();}
 {
	 if (!OnCloseGame())
	 {
		 return;
	 }
	 __super::OnCancel();
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
 int CClientGameDlg::MyRand()
 {
	 static int innerSeed = GetTickCount();

	 return (((innerSeed = innerSeed * 214013L + 2531011L) >> 16) & 0x7fff );
 }

//----------------------------------------------------------------------------------
//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	m_iMyLogDesk = GetMeUserInfo()->bDeskStation;
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:	//�ȴ�ͬ��״̬
	case GS_WAIT_NEXT:
		{	
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;

			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);	

			m_iBeginTime=pGameStation->iBeginTime;						//��ʼʱ��
			m_byBoBoTime=pGameStation->byBoBoTime;						//��������ʱ��;
			m_iThinkTime=pGameStation->iThinkTime;						//˼��ʱ��
			m_bySeparateCardTime = pGameStation->bySeparateCardTime;	//����ʱ��
			m_i64BoBase = pGameStation->i64BoBase;						//����
			m_i64SmallBobo = pGameStation->i64SmallBobo;				//��С����
			m_i64TempBobo = m_i64SmallBobo;								//��С����(��ʱ����)
			m_i64FrontNote = 0;
			memcpy(m_bUserReady , pGameStation->bUserReady ,sizeof(m_bUserReady)) ;
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_BEGIN_TIME);
			return TRUE;
		}
	case GS_SEND_CARD:	//����״̬
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pStationData;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			BYTE	byTimeRest;
			m_iBeginTime=pGameStation->iBeginTime;						//��ʼʱ��
			m_byBoBoTime=pGameStation->byBoBoTime;						//��������ʱ��;
			m_iThinkTime=pGameStation->iThinkTime;						//˼��ʱ��
			m_bySeparateCardTime = pGameStation->bySeparateCardTime;	//����ʱ��
			m_i64BoBase = pGameStation->i64BoBase;						//����
			m_i64SmallBobo = pGameStation->i64SmallBobo;				//��С����
			m_i64TempBobo = m_i64SmallBobo;								//��С����(��ʱ����)
			m_bNtPeople = pGameStation->iUpGradePeople;					//ׯ��λ��
			m_bGameStation = pGameStation->bGameStation;				//����
			m_byGameStation = pGameStation->byGameStation;				//����С�ֽ�
			byTimeRest = pGameStation->iTimeRest;
			m_i64FrontNote = 0;
			::CopyMemory(m_i64Bobo,pGameStation->i64Bobo,sizeof(m_i64Bobo));			//��������ֵ
			::CopyMemory(m_bPlayer,pGameStation->bPlayer,sizeof(m_bPlayer));			//����Ƿ��ǿ�ʼʱ���������
			if(pGameStation->byGameStation == GS_SEND_CARD_TIME)						//����ʱ��
			{
				::CopyMemory(m_iDeskCardCount,pGameStation->byAlreadySendCardCount,sizeof(m_iDeskCardCount));	//�Ѿ�����������
				::CopyMemory(m_iUserCardCount,pGameStation->iUserCardCount,sizeof(m_iUserCardCount));			//��������
				::CopyMemory(m_bSendCardCount,pGameStation->byAlreadySendCardCount,sizeof(m_bSendCardCount));	//�Ѿ�����������
				for(int i = 0x00;i < PLAY_COUNT;i ++)
				{
					memset(m_iDeskCard[i],0x00,sizeof(m_iDeskCard[i]));
					::memcpy(m_iDeskCard[i],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
				}
			}
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pStationData;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			//��������
			BYTE	byTimeRest;
			m_iBeginTime=pGameStation->iBeginTime;							//��ʼʱ��
			m_byBoBoTime=pGameStation->byBoBoTime;							//��������ʱ��
			m_iThinkTime=pGameStation->iThinkTime;							//˼��ʱ��
			m_bySeparateCardTime = pGameStation->bySeparateCardTime;		//����ʱ��
			m_i64BoBase = pGameStation->i64BoBase;							//����
			m_i64SmallBobo = pGameStation->i64SmallBobo;					//��С����
			m_i64TempBobo = m_i64SmallBobo;									//��С����(��ʱ����)
			m_bNtPeople = pGameStation->bNtPeople;							//ׯ��
			m_byGameStation = pGameStation->byGameStation;					//����С�ֽ�
			m_iNowOutPeople=pGameStation->iOutCardPeople;					//���ڳ��Ƶ��û�
			m_i64FrontNote = pGameStation->iCurNote;						//ǰ�����µ�ע
			m_iFirstOutPeople=pGameStation->iFirstOutPeople;				//��һ�ҳ�����
			int iRestTime = pGameStation->iTimeRest;
			m_bSendCardPos = pGameStation->iFirstOutPeople;
			m_iFirstOutPeople=pGameStation->iFirstOutPeople;
			byTimeRest = pGameStation->iTimeRest;							//ʵ�ʻ��е�ʱ��
			//���浱ǰ�û���ע�ܶ�
			::CopyMemory(m_i64UseAllNote,pGameStation->iTotalGameNote,sizeof(m_i64UseAllNote));		//�û����µ���ע
			::CopyMemory(m_i64Bobo,pGameStation->i64Bobo,sizeof(m_i64Bobo));						//���õ�������
			::CopyMemory(m_bPlayer,pGameStation->bPlayer,sizeof(m_bPlayer));						//����Ƿ��ǿ�ʼʱ���������
			::CopyMemory(m_i64NoteMoney,pGameStation->i64NoteMoney,sizeof(m_i64NoteMoney));			//�û�����ע������Ϣ
			::CopyMemory(m_i64UseAllNote,pGameStation->iTotalGameNote,sizeof(m_i64UseAllNote));		//��������µ�ע
			::CopyMemory(m_bSendCardCount,pGameStation->byAlreadySendCardCount,sizeof(m_bSendCardCount));	//�Ѿ����Ƶ�����
			::CopyMemory(m_iDeskCardCount,pGameStation->byAlreadySendCardCount,sizeof(m_iDeskCardCount));	//�����Ƶ�����
			::CopyMemory(m_iUserCard,pGameStation->iUserCard,sizeof(m_iUserCard));							//�û����е���
			::CopyMemory(m_iUserCardCount,pGameStation->iUserCardCount,sizeof(m_iUserCardCount));			//�û������Ƶ�����
			::CopyMemory(m_byUserOperatorType,pGameStation->byOperationState,sizeof(m_byUserOperatorType));	//��ҵĲ���
			::CopyMemory(m_byNoteTime,pGameStation->byNoteTime,sizeof(m_byNoteTime));						//�Ѿ���ע�Ĵ���
			for(int i = 0x00;i< PLAY_COUNT;i ++)							//����������
			{
				for(int j = 0x02;j < m_bSendCardCount[i];j++)
					m_iDeskCard[i][j] = m_iUserCard[i][j];
			}
			return TRUE;
		}
	}
	return false;
}

//���Ż��㵽��ͼλ��
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{	
	BYTE bViewStation=GetMeUserInfo()->bDeskStation;
	//�Լ�Ϊ  3
	switch(bViewStation)
	{
	case 0: { 
		if(bDeskStation==0)	
			return 3;
		if(bDeskStation==1)
			return 4;
		if(bDeskStation==2)
			return 5;
		if(bDeskStation==3)
			return 0;
		if(bDeskStation==4)
			return 1;
		if(bDeskStation==5)
			return 2;
		break;
			}
	case 1: {
		if(bDeskStation==0)	
			return 2;
		if(bDeskStation==1)
			return 3;
		if(bDeskStation==2)
			return 4;
		if(bDeskStation==3)
			return 5;
		if(bDeskStation==4)
			return 0;
		if(bDeskStation==5)
			return 1;
		break;
			}
	case 2: { 
		if(bDeskStation==0)	
			return 1;
		if(bDeskStation==1)
			return 2;
		if(bDeskStation==2)
			return 3;
		if(bDeskStation==3)
			return 4;
		if(bDeskStation==4)
			return 5;
		if(bDeskStation==5)
			return 0;
		break;
			}
	case 3: { 
		if(bDeskStation==0)	
			return 0;
		if(bDeskStation==1)
			return 1;
		if(bDeskStation==2)
			return 2;
		if(bDeskStation==3)
			return 3;
		if(bDeskStation==4)
			return 4;
		if(bDeskStation==5)
			return 5;
		break;
			}	
	case 4: { 
		if(bDeskStation==0)	
			return 5;
		if(bDeskStation==1)
			return 0;
		if(bDeskStation==2)
			return 1;
		if(bDeskStation==3)
			return 2;
		if(bDeskStation==4)
			return 3;
		if(bDeskStation==5)
			return 4;
		break;
			}
	case 5:
		{
			if(bDeskStation==0)	
				return 4;
			if(bDeskStation==1)
				return 5;
			if(bDeskStation==2)
				return 0;
			if(bDeskStation==3)
				return 1;
			if(bDeskStation==4)
				return 2;
			if(bDeskStation==5)
				return 3;
			break;
		}
	default:
		break;
	}
	return 0;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_GM_AGREE_GAME:
			{
				if(uDataSize!=sizeof(MSG_GR_R_UserAgree))
				{
					return 0;
				}

				MSG_GR_R_UserAgree * pUser=(MSG_GR_R_UserAgree *)pNetData;

				m_bUserReady[pUser->bDeskStation] = true ;  ///��Ϸ���׼��״̬
				m_bPlayer[pUser->bDeskStation] = true;
				m_byGameStation = 0x00;
				memset(m_i64UseAllNote,0x00,sizeof(m_i64UseAllNote));
				return true;
			}	
		case ASS_BEGIN_UPGRADE:		//��Ϸ��ʼ(��������ʱ��)
			{
				if (uDataSize!=sizeof(BeginUpgradeStruct))
					return FALSE;
				SetStationParameter(GS_SEND_CARD);
				KillAllTimer();
				SetBoboTime(pNetData,uDataSize);
				return true;
			}
		case ASS_SEND_CARD:			//����������
			{
				if (uDataSize!=sizeof(SendCardStruct)) 
					return FALSE;
				Ass_Send_Card(pNetData,uDataSize);
				return true;
			}
		case ASS_SC_NOTICE_SEND_CARD:						//֪ͨ����
			{
				if(uDataSize!=sizeof(T_Notice_SendCard)) 
					return FALSE;
				Ass_Sc_Notice_Send_Card(pNetData, uDataSize);
				return true;
			}
		case ASS_SC_SEND_ONE_CARD_FINSH:							//�ڶ��η��ƽ���
			{
				Ass_Sc_Send_One_Card_Finsh( pNetData, uDataSize);
				return true;
			}
		case ASS_SEND_FINISH:		//�������
			{	
				return true;
			}
		case ASS_GAME_PLAY:		//��ʼ��Ϸ
			{
				//������ɣ���ʼ��ע
				if (uDataSize!=sizeof(BeginPlayStruct)) 
					return FALSE;
				OnHandleGameBegin( pNetData, uDataSize);
				return true;
			}
		case ASS_NOTE:								//���Ѻע���
			{
				if(sizeof(NoteResult) != uDataSize)
				{
					return FALSE;
				}
				OnHandleBet(pNetData,uDataSize);
				return true;
			}
		case ASS_SC_NOTIC_SEPARET_CARD:
			{
				Ass_Sc_Notic_Separe_Card( pNetData, uDataSize);
				return true;
			}
		case ASS_SALE_END:	//��Ϸ��ȫ����
		case ASS_CUT_END:	//�û�ǿ���뿪
		case ASS_CONTINUE_END:	//��Ϸ����
		case ASS_NO_CONTINUE_END:
			{
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(RS_GAME_BEGIN);
				m_iBeginTime = 1;
				SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 5 +2,ID_BEGIN_TIME);					
				return TRUE;
			}
		case ASS_SET_BOBO_DATA:								//�����������BoBo
			{
				Ass_SetBobo_Data(pNetData,uDataSize);
				return true;
			}
		}
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize, pClientSocket);
}

//============================================================================================
// ���� : ���յ��û���ע����Ϣ
// ���� : void * pNetData		[in]	�������ݰ�
//		  UINT uDataSize		[in]	���ݰ�����
//============================================================================================
void CClientGameDlg::OnHandleBet(void * pNetData, UINT uDataSize)
{
	NoteResult *pResult = (NoteResult *) pNetData;
	if(NULL==pResult)
	{
		return;
	}
	CString lhx;
	lhx.Format("lihexing::��עǰ�������� %I64d %I64d %I64d %I64d %I64d %I64d",m_i64Bobo[0],m_i64Bobo[1],m_i64Bobo[2],m_i64Bobo[3],m_i64Bobo[4],m_i64Bobo[5]);
	OutputDebugString(lhx);
	//�û���ע������ʾ��ע����
	m_iNowOutPeople = pResult->bNextDeskStation;											//��ǰ��ע��	
	m_byNoteTime[pResult->iOutPeople] = pResult->byNoteTime;								//�ڼ��μ�ע

	m_i64NoteMoney[pResult->iOutPeople][pResult->byNoteTime - 1] = pResult->i64NoteMoney;	//ÿ����ע����Ϣ
	m_i64Bobo[pResult->iOutPeople] -= pResult->i64NoteMoney;								//����������
	m_i64UseAllNote[pResult->iOutPeople] += pResult->i64NoteMoney;							//������ע
	m_i64FrontNote = pResult->i64FrontNote;													//�û�����ע
	m_byUserOperatorType[pResult->iOutPeople] =  pResult->byOperatType;						//�����������

	
	lhx.Format("lihexing::��ע��������� %I64d %I64d %I64d %I64d %I64d %I64d",m_i64Bobo[0],m_i64Bobo[1],m_i64Bobo[2],m_i64Bobo[3],m_i64Bobo[4],m_i64Bobo[5]);
	OutputDebugString(lhx);
	//����ʱ��
	if((m_iNowOutPeople == m_iMyLogDesk) && (m_bPlayer[m_iMyLogDesk])&&(pResult->byFirstOutPeople != m_iNowOutPeople))							//��ǰ���Ƶ�������
	{
		int iCardShap = GetMaxCardShap(m_iCardShowCount,m_iNowOutPeople);			//�õ�����
		int OperatorType = CheckFrontUserOperator();								//ǰ��Ĳ���
		SelectOperator(OperatorType,iCardShap);										//����Ҫ����������
	}
}

//============================================================================================
// ���� : ���յ��û���ע����Ϣ
// ���� : void * pNetData		[in]	�������ݰ�
//		  UINT uDataSize		[in]	���ݰ�����
//============================================================================================
void CClientGameDlg::Ass_Sc_Notice_Send_Card(void * pNetData, UINT uDataSize)
{
	T_Notice_SendCard *tNoticSendCard = (T_Notice_SendCard *)pNetData ;
	m_iCardShowCount = tNoticSendCard->bySendCardCount;
	m_bSendCardPos = tNoticSendCard->bySendCardFirstDesk;
	m_i64FrontNote = 0x00;
}
//==============================================================================================
// ���� : ����һ���ƽ�����Ϣ
// ���� : void * pNetData			[in]		���紫���ݰ�
//		  UINT uDataSize			[in]		���ݰ���С
//==============================================================================================
void CClientGameDlg::Ass_Sc_Send_One_Card_Finsh(void * pNetData, UINT uDataSize)
{
	if(m_bSepartorFlag)
		return ;
	tSendOneCardFinsh *SendOneCard = (tSendOneCardFinsh *)pNetData;

	m_iNowOutPeople=SendOneCard->iOutDeskStation;
	memcpy(m_byUserOperatorType,SendOneCard->byUserOperatorType,sizeof(m_byUserOperatorType));
	CString lhx;
	lhx.Format("lihexing::������� %d %d %d %d %d %d",m_byUserOperatorType[0],m_byUserOperatorType[1],m_byUserOperatorType[2],m_byUserOperatorType[2],m_byUserOperatorType[4],m_byUserOperatorType[5]);
	OutputDebugString(lhx);
	//����ʱ�� 
	if((m_iNowOutPeople == m_iMyLogDesk) && m_bPlayer[m_iMyLogDesk])
	{
		int iCardShap = GetMaxCardShap(m_iCardShowCount,m_iNowOutPeople);			//�õ�����
		int OperatorType = CheckFrontUserOperator();								//ǰ��Ĳ���
		CString lhx;
		lhx.Format("lihexing::���֮ǰ�Ĳ��� %x",OperatorType);
		SelectOperator(OperatorType,iCardShap);										//����Ҫ����������
	}
}

//==============================================================================================
// ���� : ���տ��Է�����Ϣ
// ���� : void * pNetData			[in]		���紫���ݰ�
//		  UINT uDataSize			[in]		���ݰ���С
//==============================================================================================
void CClientGameDlg::Ass_Sc_Notic_Separe_Card( void * pNetData, UINT uDataSize)
{
	if(sizeof(tNoticeSeparetCard) != uDataSize)
	{
		return ;
	}
	m_bSepartorFlag = true;
	GetSeparatorCardShap(m_iMyLogDesk);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	m_iNowOutPeople=-1;
	m_iFirstOutPeople=-1;
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

	return __super::ResetGameFrame();
}
//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{

	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			if (uTimeCount<=0)
			{
				//ʱ�䵽��
				KillAllTimer();
				if (m_iAfterWaitAction == 1)
				{
					OnControlHitBegin();
				}
				else
				{
					SendMessage(WM_CLOSE,0,0);
				}
				return true;
			}
			return true;
		}
	case ID_SET_BOBOTIME:							//��ʱ������������
		{
			KillTimer(ID_SET_BOBOTIME);
			SendBoboData();
			return true;
		}	
	case ID_FOLLOW_TIME:									//��ʱ���͸���Ϣ
		{
			KillTimer(ID_FOLLOW_TIME);
			Bnt_Follow();
			return true;
		}
	case ID_SET_ALL_TIME:									//��ʱ��������Ϣ
		{
			KillTimer(ID_SET_ALL_TIME);
			Bnt_Set_All();	
			return true;
		}
	case ID_BIG_TIME:										//��ʱ���ʹ���Ϣ
		{
			KillTimer(ID_BIG_TIME);
			Bnt_Big();	
			return true;
		}
	case ID_STOP_TIME:										//��ʱ��������Ϣ
		{
			KillTimer(ID_STOP_TIME);
			Bnt_Stop();
			return true;
		}
	case ID_GIVE_UP_TIME:									//��ʱ���Ͷ���Ϣ
		{
			KillTimer(ID_GIVE_UP_TIME);
			Bnt_Give_Up();
			return true;
		}
	case ID_SEPARATE_TIME:
		{
			KillTimer(ID_SEPARATE_TIME);
			SendSeparatorCardData();
			return true;
		}
	}
	return true;
}
//========================================================================================
//������
//========================================================================================
void CClientGameDlg::Bnt_Follow()												
{
	tagUserProcess tUserProcess;
	
	if(m_i64FrontNote < m_i64Bobo[m_iMyLogDesk])
	{
		tUserProcess.iVrebType = TYPE_FOLLOW;
		tUserProcess.iNote = m_i64FrontNote;
	}
	else
	{
		tUserProcess.iVrebType = TYPE_ALL_NOTE;
		tUserProcess.iNote = m_i64Bobo[m_iMyLogDesk];
	}
	SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//========================================================================================
//�ò���(ȫ������ע����)
//========================================================================================
void CClientGameDlg::Bnt_Set_All()												
{
	tagUserProcess tUserProcess;
	tUserProcess.iVrebType = TYPE_ALL_NOTE;
	tUserProcess.iNote = m_i64Bobo[m_iMyLogDesk];

	SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//========================================================================================
//�����(��ע����)
//========================================================================================
void CClientGameDlg::Bnt_Big()													
{

	m_i64MyAddNote = m_i64FrontNote;
	if(m_i64FrontNote > m_i64Bobo[m_iMyLogDesk])
	{
		m_i64MyAddNote = m_i64Bobo[m_iMyLogDesk];
	}
	else
	{
		__int64	iTempMoney = 0x00;
		iTempMoney = m_i64Bobo[m_iMyLogDesk] - m_i64MyAddNote;
		int itemp =rand()%3;
		m_i64MyAddNote +=((iTempMoney * (itemp + 1))/4);

	}
	tagUserProcess tUserProcess;
	tUserProcess.iVrebType = TYPE_ADD;
	tUserProcess.iNote = m_i64MyAddNote;
	SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//========================================================================================
//�ݲ���
//========================================================================================
void CClientGameDlg::Bnt_Stop()												
{
	tagUserProcess tUserProcess;
	tUserProcess.iVrebType = TYPE_PAUSE;
	SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//========================================================================================
//������(��������)
//========================================================================================
void CClientGameDlg::Bnt_Give_Up()												
{
	tagUserProcess tUserProcess;
	tUserProcess.iVrebType = TYPE_GIVE_UP;
	SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//========================================================================================
// ���� : ���ͷ�����Ϣ
//========================================================================================
void CClientGameDlg::SendSeparatorCardData()
{
	if(m_byUserOperatorType[m_iMyLogDesk] == TYPE_GIVE_UP)
		return ;
	T_CS_SeparatCard tSeparatCard;
	tSeparatCard.byDeskStation = m_iMyLogDesk;
	tSeparatCard.byLeftCardShap  = m_Logic.GetCardShape(&m_bySeparatorCard[0],0x02);
	tSeparatCard.byRightCardShap = m_Logic.GetCardShape(&m_bySeparatorCard[2],0x02);
	tSeparatCard.bySeparatCard[0] = m_bySeparatorCard[0];
	tSeparatCard.bySeparatCard[1] = m_bySeparatorCard[1];
	tSeparatCard.bySeparatCard[2] = m_bySeparatorCard[2];
	tSeparatCard.bySeparatCard[3] = m_bySeparatorCard[3];
	SendGameData(&tSeparatCard,sizeof(tSeparatCard),MDM_GM_GAME_NOTIFY,ASS_CS_SEPARAT_CARD,0);
}
bool CClientGameDlg::AFCCloseFrame()
{

	return __super::AFCCloseFrame();
}

// �������Ǿ�̬������������Flash����ʹ�õĻص�����
void CClientGameDlg::FlashEndMessage(CWnd * pWnd,UINT uID,CString msgString)
{
}

//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	//��������
	m_iNowOutPeople=-1;
	m_iFirstOutPeople=-1;
	KillAllTimer();
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));			//��������ϵ��˿���
	::memset(m_iUserCard,0,sizeof(m_iUserCard));			//��������ϵ��˿�
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	::memset(m_iDeskCard,0,sizeof(m_iDeskCard));			//������˿�
	::memset(m_bUserReady,0,sizeof(m_bUserReady));
	::memset(m_bPlayer,0x00,sizeof(m_bPlayer));
	::memset(m_byUserOperatorType,0xff,sizeof(m_byUserOperatorType));
	::memset(m_i64UseAllNote,0x00,sizeof(m_i64UseAllNote));				//ÿ���û��µ���ע

	switch (iGameStation)
	{
	case RS_ALL_VIEW:
		{
			break;
		}
	case RS_GAME_CUT:
		{
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			break;
		}
	case RS_GAME_END:
		{
			break;
		}
	case RS_GAME_BEGIN:
		{

			break;
		}
	}

	return __super::ResetGameStation(iGameStation);
}

/*

�����ƶ���
static const BYTE m_CardArray[54]={
0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
0x4E, 0x4F};																		//С�����

//flash �ƶ��� 0 - 51  ���� flash �е�  ��Ƭ A ��  ����  13
*/
int CClientGameDlg::GetFlashCard(int iCard)
{
	if (iCard == 0x4F)
	{
		return 54;
	}
	else if (iCard == 0x4E)
	{
		return 53;
	}
	int iHs = iCard / 16;
	int iPd = iCard % 16;

	if (iHs < 4 && iPd > 13)
	{
		return 52;   //Ϊ�Ʊ���
	}
	if (iPd < 13)
	{
		return iCard - iHs * 3;
	}
	else if (iPd == 13)
	{
		return iCard - 13 - iHs *3;
	}
	return 52;
}
int CClientGameDlg::GetTotalPlayNum()
{
	int iCount = 0;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			iCount++;
		}
	}
	return iCount;
}

BOOL CClientGameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	return __super::PreTranslateMessage(pMsg);
}
BOOL CClientGameDlg::OnInitDialog()
{
	BOOL bRet = __super::OnInitDialog();
	return bRet;
}

//=======================================================================================
// ���� : ��������ʱ��
//

void CClientGameDlg::SetBoboTime( void * pNetData, UINT uDataSize)
{
	m_bSepartorFlag = false;
	BeginUpgradeStruct * pBegin=(BeginUpgradeStruct *)pNetData;
	m_bNtPeople = pBegin->bNtStation;										//ׯ��λ��
	m_i64BoBase = pBegin->i64BoBase;										//����
	m_i64SmallBobo = pBegin->i64SmallBobo;									//��С����
	m_i64TempBobo = m_i64SmallBobo;											//������������С��ֵ
	::CopyMemory(m_bPlayer,pBegin->bPlay,sizeof(m_bPlayer));				//��¼��ǰ����ǲ�����;�����(true:������;����)
	::memset(m_byUserOperatorType,0xff,sizeof(m_byUserOperatorType));
	m_i64FrontNote = 0;															//ǰ���µ�ע
	m_i64MyAddNote = 0;															//������Ҫ�µ�ע
	__int64 myMoney = GetMeUserInfo()->i64Money * m_iBoboPercentage/100;		//���������������ֵ

	if(myMoney < m_i64BoBase)			//������������Ƿ�Ϸ�
	{
		 myMoney = GetMeUserInfo()->i64Money - m_i64BoBase;
	}
	else
	{
		myMoney -= m_i64BoBase;
	}
	int iRand = rand() % 7 + 1;

	m_i64TempBobo = (myMoney * iRand)/7 + m_i64BoBase;

	int iTime = m_byBoBoTime /(rand()/10 + 1);
	SetGameTimer(m_iMyLogDesk,iTime,ID_SET_BOBOTIME);
}

//================================================================================
// ���� : ������������
//================================================================================
void CClientGameDlg::SendBoboData()
{
	tSetBoboStruct tSetBobo;
	tSetBobo.i64BoboMoney = m_i64TempBobo;
	tSetBobo.byMyDesk = m_iMyLogDesk;
	SendGameData(&tSetBobo,sizeof(tSetBobo),MDM_GM_GAME_NOTIFY,ASS_SET_BOBO_DATA,0);
}

//=================================================================================
// ���� : ����ӷ���˽��յ�����������
// ���� : void * pNetData			[in]	���յ�����������
//		  UINT uDataSize			[in]	���ݰ���С
//=================================================================================
void CClientGameDlg::Ass_SetBobo_Data(void * pNetData, UINT uDataSize)
{
	if(sizeof(tSetBoboStruct) != uDataSize)
	{
		return ;
	}
	tSetBoboStruct *tBobodata = (tSetBoboStruct *)pNetData;
	m_i64Bobo[tBobodata->byMyDesk] = tBobodata->i64BoboMoney;		
}

//=================================================================================
// ���� : ����ӷ���˽��յ��ķ�������
// ���� : void * pNetData			[in]	���յ�����������
//		  UINT uDataSize			[in]	���ݰ���С
//=================================================================================
void CClientGameDlg::Ass_Send_Card(void * pNetData, UINT uDataSize)
{
	SendCardStruct * pSendCard=(SendCardStruct *)pNetData;
	memcpy(m_i64NoteMoney,pSendCard->i64dNoteData,sizeof(m_i64NoteMoney));				//����ע�ĳ�����Ϣ
	memcpy(m_byNoteTime,pSendCard->byNoteTime,sizeof(m_byNoteTime));					//���³���Ĵ���

	//-----------------------------------------------------------------------------------
	//�û�������
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if(pSendCard->bCardCount[i]>0)
		{
			for (int j=0; j<MAX_CARD_COUNT; j++)
			{
				m_iUserCard[i][m_iDeskCardCount[i]]=pSendCard->bCard[i][j];				//�û����е���
				m_iDeskCard[i][m_iDeskCardCount[i]++]=pSendCard->bCard[i][j];			//�û������ϵ���
			}
		}
		//��ⷢ��״��
		memset(m_iDeskCard[i],0x00,sizeof(m_iDeskCard[i]));	
		m_i64Bobo[i] = pSendCard->i64BoboMoney[i];
	}
	m_iCardShowCount = 0x02;															//�Ѿ�����������
	CString lhx;
	lhx.Format("lihexing::AI BOBO %I64d %I64d %I64d %I64d %I64d %I64d",m_i64Bobo[0],m_i64Bobo[1],m_i64Bobo[2],m_i64Bobo[3],m_i64Bobo[4],m_i64Bobo[5]);
	OutputDebugString(lhx);
}

//=================================================================================
// ���� : ����ӷ���˽��յ��Ŀ�ʼ����Ϸ��Ϣ
// ���� : void * pNetData			[in]	���յ�����������
//		  UINT uDataSize			[in]	���ݰ���С
//=================================================================================
void CClientGameDlg::OnHandleGameBegin(void * pNetData, UINT uDataSize)
{					
	BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)pNetData;
	if(NULL==pBeginInfo)
	{
		return;
	}	
	m_iThisNoteTimes=0;//��һ����ע
	m_byGameStation = 0x02;
	//������Ϸ״̬
	SetStationParameter(GS_PLAY_GAME);
	m_iNowOutPeople=pBeginInfo->iOutDeskStation;
	m_bNtPeople = pBeginInfo->bNtPeople;   //ׯ��λ��
	memcpy(m_i64Bobo,pBeginInfo->i64dBobo,sizeof(m_i64Bobo));
	memcpy(m_byUserOperatorType,pBeginInfo->byUserOperatorType,sizeof(m_byUserOperatorType));

	CString lhx;
	lhx.Format("lihexing::AI BOBO %I64d %I64d %I64d %I64d %I64d %I64d",m_i64Bobo[0],m_i64Bobo[1],m_i64Bobo[2],m_i64Bobo[3],m_i64Bobo[4],m_i64Bobo[5]);
	OutputDebugString(lhx);

	if(m_iNowOutPeople == m_iMyLogDesk && m_bPlayer[m_iMyLogDesk])					//����ǵ�ǰ���
	{
		int iCardShap = GetMaxCardShap(0x02,m_iNowOutPeople);						//�õ�����
		int OperatorType = CheckFrontUserOperator();								//ǰ��Ĳ���
		SelectOperator(OperatorType,iCardShap);										//����Ҫ����������
	}
}
//===================================================================================================
// ���� : ����Ҫ����������
// ���� : int iOperatorType		[in]		ǰһ��ҵĲ���
//	      BYTE byCardType		[in]		��ǰ������е�����
//===================================================================================================
void CClientGameDlg::SelectOperator(int iOperatorType,BYTE byCardType)
{
		switch(iOperatorType)
		{
		case TYPE_FOLLOW:					//��
			HavaFollow(byCardType);
			break;
		case TYPE_ALL_NOTE:					//��
			HaveSetAllNote(byCardType);
			break;
		case TYPE_ADD:						//��
			HaveSetBigNote(byCardType);
			break;
		case TYPE_PAUSE:					//��		
			NoOneOperator(byCardType);					
			break;
		case TYPE_GIVE_UP:					//��
			NoOneOperator(byCardType);					//��û���˲���
			break;
		default:										//֮ǰ��û���˲�����
			NoOneOperator(byCardType);					
			break;
		}
}


//====================================================================================================
//���� : ���֮ǰ�������ʲô����
//return : ����֮ǰ��ҵĲ���
//====================================================================================================
int CClientGameDlg::CheckFrontUserOperator()
{
	int iOperatorType = 0xff;
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{
		switch(m_byUserOperatorType[i])
		{
		case TYPE_FOLLOW:							//��
			iOperatorType = TYPE_FOLLOW;
			break;
		case TYPE_ALL_NOTE:							//��
			{
				if(m_i64FrontNote < m_i64Bobo[m_iMyLogDesk])
				{
					iOperatorType = TYPE_FOLLOW;
				}
				else
				{
					iOperatorType = TYPE_ALL_NOTE;
				}
			}
			break;
		case TYPE_ADD:								//��
			iOperatorType = TYPE_ADD;
			break;
		case TYPE_PAUSE:							//��
			if(iOperatorType == 0xff)
			{
				iOperatorType = TYPE_PAUSE;
			}
			break;
		case TYPE_GIVE_UP:							//��
			if(iOperatorType == 0xff)
			{
				iOperatorType = TYPE_GIVE_UP;
			}
			break;
		default:
			break;
		}
		if(TYPE_ALL_NOTE == iOperatorType)			//�����������,�ͽ���ѭ��
		{
			if(m_i64FrontNote < m_i64Bobo[m_iMyLogDesk])
			{
				iOperatorType = TYPE_FOLLOW;
			}
			break;
		}	
	}

	if(m_iMyLogDesk == m_iNowOutPeople)
	{
		if(m_i64FrontNote > m_i64Bobo[m_iMyLogDesk])				//���Ǯ������,ֻ���û�
		{
			iOperatorType = TYPE_ALL_NOTE;
		}
	}
	return iOperatorType;
}
//========================================================================================
// ���� : ��û���˲���ʱ�����˵Ĳ���
// ���� : BYTE	byCardType			[in]		��ǰ����������
//========================================================================================
void CClientGameDlg::NoOneOperator(BYTE	byCardType)	//�ɲ������� ��,��,��,��
{
	int iRand = rand()%100;
	if(byCardType >= ZX_HE_PAI)				//����,��,��,��,��
	{
		if(iRand < 60)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_BIG_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_SET_ALL_TIME);
		}
	}
	else if(byCardType >= ZX_DI_GANG)		//�ظ�
	{
		if(iRand < 60)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_BIG_TIME);
		}
		else if(iRand < 85)					//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_STOP_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_SET_ALL_TIME);
		}
	}
	else									//ɢ��
	{
		if(iRand < 60)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_BIG_TIME);
		}
		else if(iRand < 90)					//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_STOP_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_GIVE_UP_TIME);
		}		
	}
}

//========================================================================================
// ���� : ǰ���Ѿ����˸���
// ���� : BYTE	byCardType			[in]		��ǰ����������
//========================================================================================
void CClientGameDlg::HavaFollow(BYTE byCardType)
{
	int iRand = rand()%100;
	if(byCardType >= ZX_HE_PAI)				//����,��,��,��,��
	{
		if(iRand < 50)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_FOLLOW_TIME);
		}
		if(iRand < 85)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_BIG_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_SET_ALL_TIME);
		}
	}
	else if(byCardType >= ZX_DI_GANG)		//�ظ�
	{
		if(iRand < 50)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_BIG_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_FOLLOW_TIME);
		}
	}
	else									//ɢ��
	{
		if(iRand < 60)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_BIG_TIME);
		}
		else if(iRand < 90)					//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_FOLLOW_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_GIVE_UP_TIME);
		}		
	}
}

//========================================================================================
// ���� : ǰ���Ѿ���������
// ���� : BYTE	byCardType			[in]		��ǰ����������
//========================================================================================
void CClientGameDlg::HaveSetAllNote(BYTE byCardType)
{
	int iRand = rand()%100;
	if(byCardType >= ZX_HE_PAI)				//����,��,��,��,��
	{
		SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_SET_ALL_TIME);
	}
	else if(byCardType >= ZX_DI_GANG)		//�ظ�
	{
		if(iRand < 100)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_SET_ALL_TIME);
		}
		else
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_GIVE_UP_TIME);
		}
	}
	else									//ɢ��
	{
		if(iRand < 90)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_SET_ALL_TIME);
		}
		else
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_GIVE_UP_TIME);
		}
	}
}

//========================================================================================
// ���� : ǰ���Ѿ����˴���
// ���� : BYTE	byCardType			[in]		��ǰ����������
//========================================================================================
void CClientGameDlg::HaveSetBigNote(BYTE byCardType)
{
	int iRand = rand()%100;
	if(byCardType >= ZX_HE_PAI)				//����,��,��,��,��
	{
		if(iRand < 50)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 3 +2,ID_FOLLOW_TIME);
		}
		if(iRand < 85)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 3 +2,ID_BIG_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 3 +2,ID_SET_ALL_TIME);
		}
	}
	else if(byCardType >= ZX_DI_GANG)		//�ظ�
	{
		if(iRand < 50)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 3 +2,ID_BIG_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 3 +2,ID_FOLLOW_TIME);
		}
	}
	else									//ɢ��
	{
		if(iRand < 60)						//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 3 +2,ID_BIG_TIME);
		}
		else if(iRand < 90)				//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 3 +2,ID_FOLLOW_TIME);
		}
		else								//��
		{
			SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 3 +2,ID_GIVE_UP_TIME);
		}		
	}
}
//======================================================================================================
// ���� : ��ȡ�Ƶ��������
// ���� : int iCardNum		[in]		�Ƶ�����
//		  BYTE byOperator	[in]		��ǰ������
// return MaxCardShap					�����������
//=====================================================================================================
int CClientGameDlg::GetMaxCardShap(int iCardNum,BYTE byOperator)
{
	int		iMaxCardShap = 0x00;					//Ĭ����С���� 0��
	BYTE	byCardList[2];
	if(iCardNum == 0x03)					//������
	{
		for(int i = 0x00;i < 0x02;i ++)
		{
			for(int j = i;j < 0x02;j ++)
			{
				byCardList[0] = m_iUserCard[byOperator][i];
				byCardList[1] = m_iUserCard[byOperator][j+1];
				int iCardShap = m_Logic.GetCardShape(byCardList,0x02);		//�õ�Ƭ��
				if(iMaxCardShap < iCardShap)
				{
					iMaxCardShap = iCardShap;
				}
			}
		}
	}
	else									//������		
	{
			iMaxCardShap = m_Logic.GetCardShape(m_iUserCard[m_iMyLogDesk],0x02);
	}
	return iMaxCardShap;
}


//==========================================================================================
// ���� : ���ƺ���
// ���� : int iMyDesk		[in]		Ҫ������λ��
//==========================================================================================
void  CClientGameDlg::GetSeparatorCardShap(int iMyDesk)
{
	int iCardShap[3][2];						//���ڱ�������
	BYTE iCardValue[2];
	BYTE byCardValue[3][4];
	int iTemp;
	memset(iCardShap,0x00,sizeof(iCardShap));	//�������
//-------------------------------------------------------------
	byCardValue[0][0] = m_iUserCard[iMyDesk][0];				//��һ��				
	byCardValue[0][1] = m_iUserCard[iMyDesk][1];

	iCardShap[0][0] = m_Logic.GetCmpCardValue(byCardValue[0],0x02);
	byCardValue[0][2] = m_iUserCard[iMyDesk][2];					
	byCardValue[0][3] = m_iUserCard[iMyDesk][3];

	iCardShap[0][1] = m_Logic.GetCmpCardValue(&byCardValue[0][2],0x02);
	if(iCardShap[0][0] < iCardShap[0][1])
	{
		iTemp = byCardValue[0][0];
		byCardValue[0][0] = byCardValue[0][2];
		byCardValue[0][2] = iTemp;

		iTemp = byCardValue[0][1];
		byCardValue[0][1] = byCardValue[0][3];
		byCardValue[0][3] = iTemp;

		iTemp = iCardShap[0][0];
		iCardShap[0][0] = iCardShap[0][1];
		iCardShap[0][1] = iTemp;
	}
//-------------------------------------------------------------
	byCardValue[1][0] = m_iUserCard[iMyDesk][0];				//�ڶ���				
	byCardValue[1][1] = m_iUserCard[iMyDesk][2];
	iCardShap[1][0] = m_Logic.GetCmpCardValue(byCardValue[1],0x02);
	byCardValue[1][2] = m_iUserCard[iMyDesk][1];					
	byCardValue[1][3] = m_iUserCard[iMyDesk][3];
	iCardShap[1][1] = m_Logic.GetCmpCardValue(&byCardValue[1][2],0x02);
	if(iCardShap[1][0] < iCardShap[1][1])
	{
		iTemp = byCardValue[1][0];
		byCardValue[1][0] = byCardValue[1][2];
		byCardValue[1][2] = iTemp;

		iTemp = byCardValue[1][1];
		byCardValue[1][1] = byCardValue[1][3];
		byCardValue[1][3] = iTemp;
		
		iTemp = iCardShap[1][0];
		iCardShap[1][0] = iCardShap[1][1];
		iCardShap[1][1] = iTemp;
	}

	CString lhx;
	lhx.Format("lihexing::IAԭ�� %d %d %d %d",m_iUserCard[iMyDesk][0],m_iUserCard[iMyDesk][1],m_iUserCard[iMyDesk][2],m_iUserCard[iMyDesk][3]);
	OutputDebugString(lhx);
//-------------------------------------------------------------
	byCardValue[2][0] = m_iUserCard[iMyDesk][0];				//������				
	byCardValue[2][1] = m_iUserCard[iMyDesk][3];
	iCardShap[2][0] = m_Logic.GetCmpCardValue(byCardValue[2],0x02);
	byCardValue[2][2] = m_iUserCard[iMyDesk][1];					
	byCardValue[2][3] = m_iUserCard[iMyDesk][2];
	iCardShap[2][1] = m_Logic.GetCmpCardValue(&byCardValue[2][2],0x02);
	if(iCardShap[2][0] < iCardShap[1][1])
	{
		iTemp = byCardValue[2][0];
		byCardValue[2][0] = byCardValue[2][2];
		byCardValue[2][2] = iTemp;

		iTemp = byCardValue[2][1];
		byCardValue[2][1] = byCardValue[2][3];
		byCardValue[2][3] = iTemp;

		iTemp = iCardShap[2][0];
		iCardShap[2][0] = iCardShap[2][1];
		iCardShap[2][1] = iTemp;
	}
	int iMaxIndex = 0x00;
	for(int i = 0x01;i < 0x03;i ++)
	{
		if(iCardShap[iMaxIndex][0] < iCardShap[i][0] && iCardShap[iMaxIndex][1] < iCardShap[i][1])			//��������϶���
		{
			iMaxIndex = i;
		}
		else if(iCardShap[iMaxIndex][0] < iCardShap[i][0] && iCardShap[iMaxIndex][1] == iCardShap[i][1])	//��һ����,һ����Ҳ��
		{
			iMaxIndex = i;
		}
		else if(iCardShap[iMaxIndex][0] == iCardShap[i][0] && iCardShap[iMaxIndex][1] < iCardShap[i][1])
		{
			iMaxIndex = i;
		}
	}
	for(int i = 0x00;i < MAX_CARD_COUNT;i ++)
	{
		m_bySeparatorCard[i] = byCardValue[iMaxIndex][i];
	}
	SetGameTimer(GetMeUserInfo()->bDeskStation,MyRand()% 6 +2,ID_SEPARATE_TIME);
}