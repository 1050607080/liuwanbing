#include "Shlwapi.h"
#include "StdAfx.h"
#include "ServerManage.h"
#include "writelog.h"
#include <math.h>
//��Ϸ��չ����
int  CServerGameDesk:: m_iSendCardTime;
BYTE CServerGameDesk:: m_iThinkTime;
BYTE CServerGameDesk:: m_iBeginTime;
BYTE CServerGameDesk:: m_iCallScoreTime;
BYTE CServerGameDesk:: m_iXiaZhuTime;
BOOL CServerGameDesk:: m_bTurnRule;		//˳ʱ�뻹����ʱ��
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bGameStation=GS_WAIT_ARGEE;//��Ϸ״̬

	m_iNtPeople       = -1;
	m_iUpGradePeople  = -1;						
	m_iBeenPlayGame   = 0;								//�Ѿ�����ľ���
	m_iPrepareNT      = 255;
	m_iStopCardCount  = 0;								//ͣ����ҵ�����
	//���׼��״̬Ϊ0��ʾ��û��׼��
	::memset(m_bUserReady , 0 , sizeof(m_bUserReady)) ; 
	//���̯��״̬Ϊ0��ʾ��û��̯��
	::memset(m_bReadyOpenCard , 0 , sizeof(m_bReadyOpenCard)) ; 
	memset(&m_SuperSetData,255,sizeof(m_SuperSetData));		//������������
	m_SuperSetData.bSetSuccese = false;
	m_bIsFirstLoadIni = true;						//��һ��ȥ�������ļ�
	//memset(m_bIsSuper,0,sizeof(m_bIsSuper));
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	memset(m_iTotalCard,0,sizeof(m_iTotalCard));

	memset(m_byGetCardCount,0x01,sizeof(m_byGetCardCount));

	memset(m_bIsCut,0,sizeof(m_bIsCut));


	m_iFirstJiao  = 255;
	m_iSuperCount = 0;//��������
	m_byLostTime  = 0;
}
//��������
CServerGameDesk::~CServerGameDesk(void)
{
}
//��ʼ��
bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadIni();
	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
//����ini�����ļ�
BOOL	CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");
	m_iBeginTime	= f.GetKeyVal(key,"begintime",30);
	if(m_iBeginTime<10)
	{
		m_iBeginTime=10;
	}
	m_iThinkTime	= f.GetKeyVal(key,"thinktime",20);
	if(m_iThinkTime<10)
	{
		m_iThinkTime=10;
	}
	m_iCallScoreTime= f.GetKeyVal(key,"CallScoretime",15);
	if(m_iCallScoreTime<10)
	{
		m_iCallScoreTime=10;
	}
	m_iXiaZhuTime	= f.GetKeyVal(key,"XiaZhutime",15);
	if(m_iXiaZhuTime<10)
	{
		m_iXiaZhuTime=10;
	}
	m_iGoodCard= f.GetKeyVal(key,"iGoodCard",80);
	m_bHaveKing= f.GetKeyVal(key,"HaveKing",1);		//�Ƿ�����
	m_iLimit= f.GetKeyVal(key,"LimitNote",0);	//���ע����ΪС�ڻ�=0�Ǿ��Զ����������Ǯ��ע
	m_iSendCardTime = f.GetKeyVal(key,"Sendcardtime",120);
	m_bTurnRule= f.GetKeyVal(key,"TurnRule",0);
	m_iCardShape &=0x00000000;
	m_iCardShape |= (f.GetKeyVal(key,"PersonSmallFive",0)&0xFFFFFFFF);//��С��
	m_iCardShape |= ((f.GetKeyVal(key,"HeavenKing",0)<<1)&0xFFFFFFFF);//����
	m_iCardShape |= ((f.GetKeyVal(key,"FiveSmall",0)<<2)&0xFFFFFFFF);//С��
	m_iCardShape |= ((f.GetKeyVal(key,"HalfPastTen",0)<<3)&0xFFFFFFFF);//ʮ���
	m_iCardShape |= ((f.GetKeyVal(key,"NormalCard",0)<<4)&0xFFFFFFFF);//��ͨ
	m_iCardShape |= ((f.GetKeyVal(key,"BustCard",0)<<5)&0xFFFFFFFF);//����
	m_Logic.SetCardShape(m_iCardShape);
	//������������
	CString keyName;
	int base = 0;
	for (int i=0; i<MAX_SHAPE_COUNT; i++)
	{
		keyName.Format("CardShape%d",i);
		if (i < 6)
		{
			base = 1;
		}
		else
		{
			base = i - 5;
		}
		m_bCardShapeBase[i] = f.GetKeyVal(key,keyName,base);
		if(m_bCardShapeBase[i]<1)
		{
			m_bCardShapeBase[i]=1;
		}
	}

	//��������	
	m_iSuperCount = f.GetKeyVal(key, "SuperUserCount", 2);
	
	long  lUserID = 0;
	CString strText;
	m_vlSuperID.clear();
	for (int j=0; j<m_iSuperCount; j++)
	{
		strText.Format("SuperUserID%d", j);
		lUserID = f.GetKeyVal(key, strText, 0);

		//�����г���ID ���浽����������
		m_vlSuperID.push_back(lUserID);
	}
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
	m_iBeginTime	= f.GetKeyVal(key,"begintime",m_iBeginTime);
	if(m_iBeginTime<10)
	{
		m_iBeginTime=10;
	}
	m_iThinkTime	= f.GetKeyVal(key,"thinktime",m_iThinkTime);
	if(m_iThinkTime<10)
	{
		m_iThinkTime=10;
	}
	m_iCallScoreTime= f.GetKeyVal(key,"CallScoretime",m_iCallScoreTime);
	if(m_iCallScoreTime<10)
	{
		m_iCallScoreTime=10;
	}
	m_iXiaZhuTime	= f.GetKeyVal(key,"XiaZhutime",m_iXiaZhuTime);
	if(m_iXiaZhuTime<10)
	{
		m_iXiaZhuTime=10;
	}
	m_iGoodCard        = f.GetKeyVal(key,"iGoodCard",m_iGoodCard);
	m_bHaveKing        = f.GetKeyVal(key,"HaveKing",m_bHaveKing);//�Ƿ�����
	m_iLimit           = f.GetKeyVal(key,"LimitNote",m_iLimit);//���ע����ΪС�ڻ�=0�Ǿ��Զ����������Ǯ��ע
	//������������
	CString keyName;
	int base = 0;
	for (int i=0; i<MAX_SHAPE_COUNT; i++)
	{
		keyName.Format("CardShape%d",i);
		if (i < 6)
		{
			base = 1;
		}
		else
		{
			base = i - 5;
		}
		m_bCardShapeBase[i] = f.GetKeyVal(key,keyName,m_bCardShapeBase[i]);
		if(m_bCardShapeBase[i]<1)
		{
			m_bCardShapeBase[i]=1;
		}
	}
	//��������
	m_iAIWantWinMoneyA1	= f.GetKeyVal(key,"AIWantWinMoneyA1 ",__int64(500000));		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(key,"AIWantWinMoneyA2 ",__int64(5000000));	/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(key,"AIWantWinMoneyA3 ",__int64(50000000));	/**<������ӮǮ����3  */
	m_iAIWinLuckyAtA1	= f.GetKeyVal(key,"AIWinLuckyAtA1 ",90);				/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(key,"AIWinLuckyAtA2 ",70);				/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(key,"AIWinLuckyAtA3 ",50);				/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(key,"AIWinLuckyAtA4 ",30);				/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAddMoneyEveryDay	= f.GetKeyVal(key,"AddMoneyEveryDay ",__int64(0));	/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	m_iAIHaveWinMoney	= f.GetKeyVal(key,"AIHaveWinMoney ",__int64(0));	/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(key,"AIWinAndLostAutoCtrl",0);		//��������Ӯ����20121122dwj
	m_iReSetAIHaveWinMoney	= f.GetKeyVal(key,"ReSetAIHaveWinMoney ",__int64(0)); //��¼���û�����ӮǮ���������Ϸ�����иı��˾�Ҫ�ı������ӮǮ��
	//��������
	m_iSuperCount = f.GetKeyVal(key, "SuperUserCount", 0);
	long  lUserID = 0;
	CString strText;
	m_vlSuperID.clear();
	for (int j=0; j<m_iSuperCount; j++)
	{
		strText.Format("SuperUserID%d", j);
		lUserID = f.GetKeyVal(key, strText,0);
		m_vlSuperID.push_back(lUserID);
	}
	return TRUE;
}

//-------------------------------add-by-DWJ-20121113-----------------------------------------------------------------------------------------
//�������ļ��ж�ȡ�������Ѿ�Ӯ�˶���Ǯ����Ϊ��ÿ�̽����ʱ�򶼻Ὣ������ӮǮ����д�������ļ��С� 
//������ÿ�̿�ʼ��ʱ���ֶ�ȡ�£���Ϊ�������޸�����ô�ͻ�������Ǯ��.
void CServerGameDesk::GetAIContrlSetFromIni(int iRoomID)
{
	CString s = CINIFile::GetAppPath ();/////����·��
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
//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIME_CALL_NT:	//��ׯ
		{
			KillTimer(TIME_CALL_NT);
			if (m_bGameStation == GS_ROB_NT)
			{
				UserCallScore(m_bCurrentCallScore, 1);
			}
			return TRUE;
		}
	case TIME_NOTE:	//��ע��ʱ��
		{
			KillTimer(TIME_NOTE);
			if (m_bGameStation == GS_NOTE)
			{
				for (int i=0; i<PLAY_COUNT; i++)
				{
					if (m_pUserInfo[i] != NULL && m_iUserStation[i] == TYPE_PLAY_GAME && i!= m_iUpGradePeople)
					{
						tagUserProcess userBet;  //�û�������Ϣ
						userBet.iNoteType = 3;   //��ע����  ��������
						userBet.iVrebType = TYPE_NOTE;	//������İ�ť ??????
						UserNoteResult(i,userBet.iVrebType,userBet.iNoteType);
					}
				}
				if (CountNotePeople() == 1)
				{
					GameFinish(0,GF_SALE);
					return TRUE;
				}

				SetTimer(TIME_NOTE_FINISH,100);
			}
			return TRUE;
		}
	case TIME_NOTE_FINISH:	//��ע��ɶ�ʱ��
		{
			KillTimer(TIME_NOTE_FINISH);
			if (m_bGameStation==GS_NOTE) 
			{
				SendCard();
			}
			return TRUE;
		}
	case TIME_SEND_CARD:	//������ɶ�ʱ��
		{
			KillTimer(TIME_SEND_CARD);
			if (m_bGameStation == GS_SEND_CARD)
			{
				SendCardFinish();
			}
			return TRUE;
		}
	case TIME_TAKE_CARD:	//Ҫ�Ƽ�ʱ��
		{
			KillTimer(TIME_TAKE_CARD);
			// ͣ��ǰ���ߣ���ׯ�����������û��עǰ���ߣ���ϵͳ�Զ�����ͣ�ƣ�
			for (BYTE i=0; i<PLAY_COUNT; i++)
			{
				StopCardStruct TAllStopCard;
				TAllStopCard.bReadyStopCard[i]=true;
				m_iUserStation[i] = TYPE_STOP_CARD;
				if (NULL!=m_pUserInfo[i] && m_iUserStation[i] != -1 /*&& m_iUserStation[i] != TYPE_TAKE_CARD*/)
				{
					UserStopCard(i,&TAllStopCard);
				}
			}
			return TRUE;
		}
	case TIME_GAME_FINISH:	//������ʱ��
		{
			KillTimer(TIME_GAME_FINISH);
			KillTimer(TIME_TAKE_CARD);
			if (m_bGameStation==GS_PLAY_FINISH ) 
				GameFinish(0,GF_NORMAL);
			return TRUE;
		}
	case TIME_LOST_TIME:
		{
			m_byLostTime--;
			if(m_byLostTime <= 0x00)
			{
				KillTimer(TIME_LOST_TIME);
			}
			return TRUE;
		}
	}
	
	return __super::OnTimer(uTimerID);
}

//�����Ϣ������
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
	OutputDebugString("err::HandleFrameMessage(1)");
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
		case ASS_SUPER_USER_SET:	//�������������Ϣ
			{
				if(bWatchUser)
				{
					return FALSE;
				}
				if(m_bGameStation < GS_ROB_NT || m_bGameStation > GS_PLAY_GAME)
				{
					return true;
				}
				SuperUserSetData * pSuperSet= (SuperUserSetData*)pData;
				if(NULL==pSuperSet)
				{
					return true;
				}

				if (IsSuperUser(bDeskStation))
				{
					
					//������������
					memcpy(&m_SuperSetData,pSuperSet,sizeof(m_SuperSetData));
					m_SuperSetData.bSetSuccese = true;

					//�������ý����Ϣ
					SendGameData(bDeskStation,&m_SuperSetData,sizeof(m_SuperSetData),MDM_GM_GAME_NOTIFY,ASS_SUPER_USER_SET_RESULT,0);
					
				}
				return TRUE;
			}

		case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
			{
				OutputDebugString("err::ASS_GM_AGREE_GAME(0)");
				if (bWatchUser)
					return TRUE;
				if(m_bUserReady[bDeskStation])
				{
					return true; 
				}

				m_bUserReady[bDeskStation] = true ; 
				//�������
				if ((m_bGameStation!=GS_WAIT_ARGEE)&&(m_bGameStation!=GS_WAIT_NEXT)) 
					return TRUE;
					
				if(m_bGameStation!=GS_WAIT_NEXT)
				{
					m_bGameStation=GS_WAIT_ARGEE;
					//GameFinish(0,GF_SALE);
				}
				OutputDebugString("err::ASS_GM_AGREE_GAME(1)");
				return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
			}
		case ASS_USER_SCORE:	//��ҽ�ׯ
			{
				OutputDebugString("err::ASS_USER_SCORE(0)");
				if ((bWatchUser)||(uSize!=sizeof(CallScoreStruct))) 
					return true;
				if (m_iUserStation[bDeskStation] == -1)
				{
					return true;
				}
				CallScoreStruct * pCallScore=(CallScoreStruct *)pData;
				if (NULL == pCallScore)
				{
					return true;
				}
				UserCallScore(bDeskStation,pCallScore->iValue);
				OutputDebugString("err::ASS_USER_SCORE(1)");
				return true;
			}
		case ASS_USER_NOTE:	//�����ע
			{
				OutputDebugString("err::ASS_USER_NOTE(0)");
				if(bWatchUser)
					return true;
				if (m_iPerJuTotalNote[bDeskStation]!=0 || m_iUserStation[bDeskStation] == TYPE_GIVE_UP)
				{
					return true;
				}
				if (m_iUserStation[bDeskStation] == -1)
				{
					return true;
				}

				tagUserProcess *pVerb = (tagUserProcess *) pData;
				if (NULL == pVerb)
				{
					return true;
				}
				OutputDebugString("err::ASS_USER_NOTE(1)");
				UserNoteResult(bDeskStation,pVerb->iVrebType,pVerb->iNoteType);
				return TRUE;
			}
		case ASS_USER_TAKE_CARD:		//�û�Ҫ��
			{
				OutputDebugString("err::ASS_USER_TAKE_CARD(0)");
				if(bWatchUser)
				{
					return true;
				}
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
				{
					return true;
				}
				if (m_iUserStation[bDeskStation] == -1)
				{
					return true;
				}

				TakeCardStruct * pYaoPai=(TakeCardStruct *)pData;
				if (NULL == pYaoPai)
				{
					return true;
				}
				UserYaoPai(bDeskStation,pYaoPai);

				OutputDebugString("err::ASS_USER_TAKE_CARD(1)");
				return true;
			}
		case ASS_USER_STOP:		//�û�ͣ��
			{
				OutputDebugString("err::ASS_USER_STOP(0)");
				if(bWatchUser)
				{
					return true;
				}
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
				{
					return true;
				}
				if (m_iUserStation[bDeskStation] == -1)
				{
					return true;
				}

				UserStopCard(bDeskStation,pData);
				OutputDebugString("lihexing::ASS_USER_STOP(1)");
				return true;
			}
	
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	// ��֤�����û�Ȩ��
	SuperUserExamine(bDeskStation);

	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			// ��֤�����û�Ȩ��
			GameStation_WaiteAgree TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			//��Ϸ�汾�˶�
			TGameStation.iVersion=DEV_HEIGHT_VERSION;			//��Ϸ�汾��
			TGameStation.iVersion2=DEV_LOW_VERSION;
	
			//��Ϸ������Ϣ
			TGameStation.byGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.iThinkTime		= m_iThinkTime;			//Ҫ��ʱ��
			TGameStation.iBeginTime		= m_iBeginTime;			//׼��ʱ��
			TGameStation.iSendCardTime	= m_iSendCardTime;		//����ʱ��
			TGameStation.iCallScoreTime = m_iCallScoreTime;		//��ׯʱ��
			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
	
			TGameStation.iCardShape		= m_iCardShape;			//��������

			//����Ƿ���׼��
			memcpy(TGameStation.bUserReady ,m_bUserReady , sizeof(TGameStation.bUserReady )) ;
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return TRUE;
		}
	case GS_ROB_NT:		//��ׯ״̬
		{
			GameStation_RobNt TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			//��Ϸ�汾�˶�
			TGameStation.iVersion=DEV_HEIGHT_VERSION;			//��Ϸ�汾��
			TGameStation.iVersion2=DEV_LOW_VERSION;

			//��Ϸ������Ϣ
			TGameStation.byGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.iThinkTime		= m_iThinkTime;			//Ҫ��ʱ��
			TGameStation.iBeginTime		= m_iBeginTime;			//׼��ʱ��
			TGameStation.iSendCardTime	= m_iSendCardTime;		//����ʱ��
			TGameStation.iCallScoreTime = m_iCallScoreTime;		//��ׯʱ��
			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			TGameStation.iCardShape		= m_iCardShape;			//��������
			TGameStation.byLostTime		= m_byLostTime;			//��¼��ʱ��ʣ��ʱ��
			TGameStation.byCurrentCallScore	= m_bCurrentCallScore;	//��ǰ��ׯ�� 


			for (int i=0; i<PLAY_COUNT; i++)
			{
				//�������ׯ���	255-��ʾ��û���� 0-��ʾ���� 1-��ʾ�Ѿ�����
				TGameStation.byUserCallStation[i] = m_iCallScore[i];
				//��ҵ�״̬ ����Ƿ���;�����
				TGameStation.iUserStation[i] = m_iUserStation[i];
			}
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return TRUE;
		}
	case GS_NOTE:		//��ע״̬
		{

			GameStation_Note TGameStation;
			//��Ϸ�汾�˶�
			TGameStation.iVersion=DEV_HEIGHT_VERSION;			//��Ϸ�汾��
			TGameStation.iVersion2=DEV_LOW_VERSION;

			//��Ϸ������Ϣ
			TGameStation.byGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.iThinkTime		= m_iThinkTime;			//̯��ʱ��
			TGameStation.iBeginTime		= m_iBeginTime;			//׼��ʱ��
			TGameStation.iSendCardTime	= m_iSendCardTime;		//����ʱ��
			TGameStation.iCallScoreTime = m_iCallScoreTime;		//��ׯʱ��
			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			TGameStation.iCardShape		= m_iCardShape;			//��������		
			TGameStation.byNtStation	= m_iUpGradePeople;		//ׯ��λ��
			TGameStation.byLostTime		= m_byLostTime;			//��¼��ʱ��ʣ��ʱ��


			memcpy(TGameStation.i64UserNoteLimite,m_i64UserNoteLimite,sizeof(TGameStation.i64UserNoteLimite));
			for(int i=0;i<PLAY_COUNT;i++)
			{
				TGameStation.iUserStation[i]		= m_iUserStation[i];			//�������ע״̬ -1-��ʾ��û���� 0-��ʾû��ע 1-��ʾ�Ѿ���ע
				TGameStation.i64PerJuTotalNote[i]	= m_iPerJuTotalNote[i];			//�û�ÿ��ע���
				TGameStation.iLimitNote[i]			= m_iLimitNote[i];				//���ע��
			}

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return TRUE;
		}
	case GS_SEND_CARD:	
		{

			GameStation_SendCard TGameStation;
			//��Ϸ�汾�˶�
			TGameStation.iVersion=DEV_HEIGHT_VERSION;			//��Ϸ�汾��
			TGameStation.iVersion2=DEV_LOW_VERSION;

			//��Ϸ������Ϣ
			TGameStation.byGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.iThinkTime		= m_iThinkTime;			//̯��ʱ��
			TGameStation.iBeginTime		= m_iBeginTime;			//׼��ʱ��
			TGameStation.iSendCardTime	= m_iSendCardTime;		//����ʱ��
			TGameStation.iCallScoreTime = m_iCallScoreTime;		//��ׯʱ��
			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			TGameStation.iCardShape		= m_iCardShape;			//��������
			TGameStation.byNtStation	= m_iUpGradePeople;		//ׯ��λ��
			TGameStation.byLostTime		= m_byLostTime;			//��¼��ʱ��ʣ��ʱ��

			//�û����ϵ��˿�
			memcpy(TGameStation.iUserCard,m_iUserCard,sizeof(TGameStation.iUserCard));

			for(int i=0; i<PLAY_COUNT; i++)
			{
				TGameStation.iUserCardCount[i]		= m_iUserCardCount[i];			//�û������˿���Ŀ
				TGameStation.i64PerJuTotalNote[i]	= m_iPerJuTotalNote[i];			//�û�ÿ��ע���
				//����ҵ�״̬ ����Ƿ���;�����
				TGameStation.iUserStation[i] = m_iUserStation[i];
			}
	
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{

			GameStation_OpenCard TGameStation;
			//��Ϸ�汾�˶�
			TGameStation.iVersion=DEV_HEIGHT_VERSION;			//��Ϸ�汾��
			TGameStation.iVersion2=DEV_LOW_VERSION;

			//��Ϸ������Ϣ
			TGameStation.byGameStation	= m_bGameStation;		//��Ϸ״̬
			TGameStation.iThinkTime		= m_iThinkTime;			//̯��ʱ��
			TGameStation.iBeginTime		= m_iBeginTime;			//׼��ʱ��
			TGameStation.iSendCardTime	= m_iSendCardTime;		//����ʱ��
			TGameStation.iCallScoreTime = m_iCallScoreTime;		//��ׯʱ��
			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			TGameStation.iCardShape		= m_iCardShape;			//��������
			TGameStation.byNtStation	= m_iUpGradePeople;		//ׯ��λ��
			TGameStation.byLostTime		= m_byLostTime;			//��¼��ʱ��ʣ��ʱ��
			

			//���״̬
			memcpy(TGameStation.iUserStation, m_iUserStation, sizeof(TGameStation.iUserStation));
			//�û�������
			::CopyMemory(TGameStation.iUserCardCount,m_iUserCardCount,sizeof(TGameStation.iUserCardCount));
			::CopyMemory(TGameStation.iUserCard,m_iUserCard,sizeof(TGameStation.iUserCard));
			//�Ѿ����Ƶ�����
			::CopyMemory(TGameStation.byOpenCardCount,m_byGetCardCount,sizeof(TGameStation.byOpenCardCount));
			//��ǰ���û���ע����
			::CopyMemory(TGameStation.i64PerJuTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));
			
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			return TRUE;
		}

	}
	return false;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	OutputDebugString("err::ReSetGameState(0)");

	KillTimer(TIME_NOTE_FINISH);
	KillTimer(TIME_GAME_FINISH);

	if ((bLastStation==GFF_FORCE_FINISH)||(bLastStation==GF_SALE))
	{
		m_iBeenPlayGame = 0;
		m_iNtPeople = -1;	
	}
	//���׼��״̬Ϊ0��ʾ��û��׼��
	memset(m_bUserReady,0,sizeof(m_bUserReady));
	//���̯��״̬Ϊ0��ʾ��û��̯��
	memset(m_bReadyOpenCard , 0 , sizeof(m_bReadyOpenCard)) ; 

	//	m_iUpGradePeople=-1;
	m_iPrepareNT = 255;

	m_byLostTime = 0;

	memset(&m_SuperSetData,255,sizeof(m_SuperSetData));		//������������
	m_SuperSetData.bSetSuccese = false;

	memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�

	//memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_iUserCardCount, 0, sizeof(m_iUserCardCount));

	memset(m_bIsCut, 0, sizeof(m_bIsCut));
	OutputDebugString("err::ReSetGameState(1)");
	return TRUE;
}

/*---------------------------------------------------------------------------------*/
//��Ϸ��ʼ
bool	CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	OutputDebugString("err::GameBegin(0)");
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		GameFinish(0,GF_SALE);
		return false;
	}
	
	m_bGameStation = GS_ROB_NT;				//��Ϸ״̬	
	m_iBeenPlayGame++;							//�Ѿ���ľ���

	//����ÿ�̸��ݷ���ID��ȡ�������ļ��е�ReSetAIHaveWinMoney ���� �趨�»�����Ӯ�˶���Ǯ
	GetAIContrlSetFromIni(m_pDataManage->m_InitData.uRoomID);

	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));		//�û���������
	for (int i = 0; i < m_bMaxPeople; i++) 
	{
		m_iUserStation[i] = -1;
		m_iPerJuTotalNote[i] = 0;
		if (m_pUserInfo[i])
		{
			m_iUserStation[i] = TYPE_PLAY_GAME;
		}
	}
	//�����������
	srand(GetTickCount());
	//ϴ��
	m_Logic.RandCard(m_iTotalCard,54,m_bHaveKing);
	//ϴ������
	int temp = rand()%100;
	if (temp < m_iGoodCard)
	{
		for (int i=0; i<20; i++)
		{
			if (GetMaxCardShape(m_iTotalCard,54) > UG_HALF_PAST_TEN)
			{
				break;
			}
			else
			{
				OutputDebugString("zhtlog:ûϴ������");
			}
			m_Logic.RandCard(m_iTotalCard,54,m_bHaveKing);
		}
	}
	
	if (m_iFirstJiao != 255)
	{
		m_iFirstJiao = GetNextDeskStation(m_iFirstJiao);
	}
	else
	{
		m_iFirstJiao = GetNextDeskStation(rand()%PLAY_COUNT);
	}
	
	SendCallScore(m_iFirstJiao);
	OutputDebugString("err::GameBegin(1)");
	return true;
}
/*---------------------------------------------------------------------------------*/
//֪ͨ�û���ע
BOOL	CServerGameDesk::NoticeUserNote()
{
	OutputDebugString("err::NoticeUserNote(0)");
	m_bGameStation = GS_NOTE;
	//ȷ��ׯ��
	m_iUpGradePeople = m_iPrepareNT;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (i==m_iUpGradePeople || !m_pUserInfo[i])
			continue;
		m_iLimitNote[i] = GetLimit(i);

		//�ĸ���עֵ
		for (int m=0; m<4; m++)
		{
			m_i64UserNoteLimite[i][m] = m_iLimitNote[i]/(m+1);
		}
	}

	UserNoteStruct TUserNote;
	memset(&TUserNote,0,sizeof(TUserNote));
	::CopyMemory(TUserNote.iLimitNote,m_iLimitNote,sizeof(m_iLimitNote));
	memcpy(TUserNote.i64UserNoteLimite,m_i64UserNoteLimite,sizeof(TUserNote.i64UserNoteLimite));
	m_MaxPlayer = CountNotePeople() - 1;//�мҵ�����Ϊ����δ������ҵ�������ȥһ��ׯ��
	TUserNote.iNt = m_iUpGradePeople;	 

	for (int i=0;i<m_bMaxPeople;i++) 
	{
		if(m_pUserInfo[i]&&m_iUserStation[i]!=TYPE_GIVE_UP)
			SendGameData(i,&TUserNote,sizeof(TUserNote),MDM_GM_GAME_NOTIFY,ASS_CALL_NOTE,0);
	}
	SendWatchData(m_bMaxPeople,&TUserNote,sizeof(TUserNote),MDM_GM_GAME_NOTIFY,ASS_CALL_NOTE,0);

	m_byLostTime = m_iXiaZhuTime + 1;
	KillTimer(TIME_LOST_TIME);
	SetTimer(TIME_LOST_TIME,1000);

	SetTimer(TIME_NOTE,(m_iXiaZhuTime+2)*1000);
	m_iSendPos=0;
	OutputDebugString("err::NoticeUserNote(1)");
	return TRUE;
}
/*---------------------------------------------------------------------------------*/
//�û���ע
BOOL	CServerGameDesk::UserNoteResult(BYTE bDeskStation, BYTE iVerbType,int iNoteType)
{
	OutputDebugString("err::UserNoteResult(0)");
	if (m_iPerJuTotalNote[bDeskStation]!=0 || m_iUserStation[bDeskStation] == TYPE_GIVE_UP)
	{
		return true;
	}
	if (iNoteType <0 || iNoteType >3)
	{
		return true ;
	}
	//��Ҳ�����ע��ʱ
	if(m_i64UserNoteLimite[bDeskStation][iNoteType]>m_pUserInfo[bDeskStation]->m_UserData.i64Money)
	{
		m_iPerJuTotalNote[bDeskStation] = m_i64UserNoteLimite[bDeskStation][3];
	}
	else
	{
		m_iPerJuTotalNote[bDeskStation] = m_i64UserNoteLimite[bDeskStation][iNoteType];
	}

	m_iUserStation[bDeskStation] = TYPE_NOTE;

	m_MaxPlayer--;

	if (CountNotePeople() == 1)
	{
		GameFinish(0,GF_SALE);
		OutputDebugString("err::UserNoteResult(1)");
		return TRUE;
	}
	NoteResult noteresult;					
	noteresult.bAddStyle  = iVerbType;									//��ע����
	noteresult.iOutPeople = bDeskStation;								//������ע��
	noteresult.iCurNote   = m_iPerJuTotalNote[bDeskStation];			//��ǰ��ע��
	for (int i = 0; i < m_bMaxPeople; i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_CALL_NOTE_RESULT,0);
		}		
	}
	SendWatchData(m_bMaxPeople,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_CALL_NOTE_RESULT,0);

	//������Ϣ
	if (m_MaxPlayer <= 0)
	{
		KillTimer(TIME_NOTE);
	
		//20121122 dwj �����˿�����Ӯ���� û�г��˿���;
		if (m_bAIWinAndLostAutoCtrl && !m_SuperSetData.bSetSuccese)
		{
			IAWinAutoCtrl();
		}

		SetTimer(TIME_NOTE_FINISH,500);
		OutputDebugString("err::UserNoteResult(1)");
		return TRUE;

	}
	if (CheckFinish())
	{
		OutputDebugString("err::UserNoteResult(1)");
		return TRUE;
	}
	OutputDebugString("err::UserNoteResult(1)");
	return TRUE;

}
/*---------------------------------------------------------------------------------*/
//�����˿˸��û�
BOOL	CServerGameDesk::SendCard()
{
	OutputDebugString("err::SendCard(0)");
	KillTimer(TIME_NOTE_FINISH);
	m_bGameStation = GS_SEND_CARD;
	memset(m_byGetCardCount,0x01,sizeof(m_byGetCardCount));
	//�ַ��˿���
	for(int j = 0 ; j<SH_USER_CARD ;j++)
	{
		for(int i = 0 ;i<PLAY_COUNT;i++)
		{
			if( NULL == m_pUserInfo[i] || m_iUserStation[i] == -1)
			{
				m_iUserCardCount[i] = 0;
			}
			else
			{
				m_iUserCard[i][j] =  m_iTotalCard[i*SH_USER_CARD+j];	
			}			
		}
	}

	//�����û������˿���Ŀ
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if( NULL == m_pUserInfo[i] || m_iUserStation[i] == -1)
		{
			m_iUserCardCount[i] = 0;
			memset(&m_iUserCard[i],0,sizeof(m_iUserCard[i]));
		}
		else
		{
			m_iUserCardCount[i] = SH_USER_CARD;
		}
	}


	//������������� ��Ҫ����
// 	if (m_SuperSetData.bSetSuccese)
// 	{
// 		//����
// 		if (m_SuperSetData.byMaxDesk[m_pUserInfo->GetUserData()->bDeskStation] != 255)
// 		{
// 			if (-1 != m_iUserStation[m_SuperSetData.byMaxDesk[m_pUserInfo->GetUserData()->bDeskStation]] && m_iUserCardCount[m_SuperSetData.byMaxDesk[m_pUserInfo->GetUserData()->bDeskStation]] == SH_USER_CARD)
// 			{
// 				for(int i=0; i<PLAY_COUNT; i++)
// 				{
// 					if (i != m_SuperSetData.byMaxDesk[m_pUserInfo->GetUserData()->bDeskStation] && -1 != m_iUserStation[i] && m_iUserCardCount[i] == SH_USER_CARD)
// 					{
// 						//�жϴ�С
// 						if( m_Logic.CompareCard(m_iUserCard[i], SH_USER_CARD, m_iUserCard[m_SuperSetData.byMaxDesk[m_pUserInfo->GetUserData()->bDeskStation]],SH_USER_CARD) == 1)
// 						{
// 							ChangeTwoUserCard(i,m_SuperSetData.byMaxDesk[m_iMyDeskStation]);
// 							//ChangeCard(i,m_SuperSetData.byMaxDesk);
// 						}
// 					}
// 				}
// 			}
// 		}
// 		//��С��
// 		if (m_SuperSetData.byMinDesk != 255)
// 		{
// 			if (-1 != m_iUserStation[m_SuperSetData.byMinDesk] && m_iUserCardCount[m_SuperSetData.byMinDesk] == SH_USER_CARD)
// 			{
// 				for(int i=0; i<PLAY_COUNT; i++)
// 				{
// 					if (i != m_SuperSetData.byMinDesk && -1 != m_iUserStation[i] && m_iUserCardCount[i] == SH_USER_CARD)
// 					{
// 						//�жϴ�С
// 						if( m_Logic.CompareCard(m_iUserCard[i], SH_USER_CARD, m_iUserCard[m_SuperSetData.byMinDesk],SH_USER_CARD) != 1)
// 						{
// 							ChangeCard(i,m_SuperSetData.byMaxDesk[m_iMyDeskStation]);
// 							
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}

	SendAllCardStruct TSendAllCard;
	TSendAllCard.iStartPos = m_iUpGradePeople;
	memcpy(TSendAllCard.iUserCardCount,m_iUserCardCount,sizeof(TSendAllCard.iUserCardCount));
	memcpy(TSendAllCard.iUserCard,m_iUserCard,sizeof(TSendAllCard.iUserCard));	

	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&TSendAllCard,sizeof(TSendAllCard),MDM_GM_GAME_NOTIFY,ASS_CALL_SEND_CARD,0);
		}
	}	
	SendWatchData(m_bMaxPeople,&TSendAllCard,sizeof(TSendAllCard),MDM_GM_GAME_NOTIFY,ASS_CALL_SEND_CARD,0);

	m_byLostTime = m_iSendCardTime + 1;
	KillTimer(TIME_LOST_TIME);
	SetTimer(TIME_LOST_TIME,1000);

	///����ʱ��
	SetTimer(TIME_SEND_CARD,(CountNotePeople()*(m_iSendCardTime*SH_USER_CARD))+1000);

	OutputDebugString("err::SendCard(1)");
	return TRUE;
}
/*---------------------------------------------------------------------------------*/
//���ƽ���
BOOL	CServerGameDesk::SendCardFinish()
{
	OutputDebugString("err::SendCard(0)");
	for (int i=0;i<PLAY_COUNT;i++) 
	{
		if(m_pUserInfo[i])
			SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_CALL_SEND_FINISH,0);
	}
	SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_CALL_SEND_FINISH,0);

	BeginPlayUpGrade();
	OutputDebugString("err::SendCard(1)");
	return true;
}
/*---------------------------------------------------------------------------------*/
//��Ϸ��ʼ
BOOL	 CServerGameDesk::BeginPlayUpGrade()
{
	OutputDebugString("err::BeginPlayUpGrade(0)");
	//������Ϸ״̬
	m_bGameStation=GS_PLAY_GAME;

	//������Ϸ��ʼ��Ϣ
	BeginPlayStruct TBegin;
	for (int i=0;i<PLAY_COUNT;i++)
	{
		SendGameData(i,&TBegin,sizeof(TBegin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	}
	SendWatchData(m_bMaxPeople,&TBegin,sizeof(TBegin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

	m_byLostTime = m_iThinkTime + 1;
	KillTimer(TIME_LOST_TIME);
	SetTimer(TIME_LOST_TIME,1000);

	// ����ʱҪ��
	SetTimer(TIME_TAKE_CARD, (m_iThinkTime+1)*1000);
	OutputDebugString("err::BeginPlayUpGrade(1)");
	return TRUE;
}
/*---------------------------------------------------------------------------------*/
//��ҿ�ʼ��Ϸ��Ҫ�ƣ�
BOOL CServerGameDesk::UserYaoPai(BYTE byDeskStation,void * pData)
{
	//�ӵڶ����ƿ�ʼ����ֱ��5����ȫ���������߱���
	TakeCardStruct *pUserYaoPai = (TakeCardStruct *) pData;
	if (NULL == pUserYaoPai)
	{
		return TRUE;
	}

	m_iUserStation[byDeskStation] = TYPE_TAKE_CARD;

	TakeCardStruct TUserTakeCardResult;
	TUserTakeCardResult.byDeskStation = byDeskStation;
	TUserTakeCardResult.byReadyYaoCard[byDeskStation] = pUserYaoPai->byReadyYaoCard[byDeskStation];
	m_byGetCardCount[byDeskStation]++;
	TUserTakeCardResult.byYaoPaiCount = m_byGetCardCount[byDeskStation]-1;
	//����5����ʱ����״̬Ϊͣ��״̬
	if(m_byGetCardCount[byDeskStation] >= m_iUserCardCount[byDeskStation])
	{
		m_bGameStation = GS_PLAY_FINISH;
		m_iUserStation[byDeskStation] = TYPE_STOP_CARD;
		TUserTakeCardResult.byReadyYaoCard[byDeskStation] = false;
	}
	//����10���ʱ����״̬Ϊͣ��״̬
	if(m_Logic.GetPoint(m_iUserCard[byDeskStation],m_byGetCardCount[byDeskStation]) >= 105)
	{
		m_bGameStation = GS_PLAY_FINISH;
		m_iUserStation[byDeskStation] = TYPE_STOP_CARD;
		TUserTakeCardResult.byReadyYaoCard[byDeskStation] = false;
	}
	//˭Ҫ���ưѷ��ƽ�����͸�˭
	if(m_pUserInfo[byDeskStation])
	{
		SendGameData(byDeskStation,&TUserTakeCardResult,sizeof(TUserTakeCardResult),MDM_GM_GAME_NOTIFY,ASS_TAKE_CARD_RESULT,0);
	}	
	SendWatchData(m_bMaxPeople,&TUserTakeCardResult,sizeof(TUserTakeCardResult),MDM_GM_GAME_NOTIFY,ASS_TAKE_CARD_RESULT,0);
	OutputDebugString("err::UserYaoCard(1)");
	if (CheckFinish())
		return TRUE;
	return true;
}
//�û�ͣ��
BOOL CServerGameDesk::UserStopCard(BYTE byDeskStation,void * pData)
{	
	StopCardStruct* TUserStopCard=(StopCardStruct*) pData;
	//������Ϸ״̬
	m_bGameStation=GS_PLAY_FINISH;
	m_iUserStation[byDeskStation] = TYPE_STOP_CARD;
	
	//�ض�ʱ��
	//KillAllTimer();
	//֪ͨ�ͻ���ͣ��
	StopCardStruct TUserStopCardResult;
	TUserStopCardResult.bReadyStopCard[byDeskStation] = TUserStopCard->bReadyStopCard[byDeskStation];
	m_byGetCardCount[byDeskStation] = TUserStopCardResult.bReadyStopCard[byDeskStation];

	for (int i=0;i<PLAY_COUNT;i++) 
	{	
		if(m_pUserInfo[i])
		{
			TUserStopCardResult.byDeskStation = i; 
			SendGameData(byDeskStation,&TUserStopCardResult,sizeof(TUserStopCardResult),MDM_GM_GAME_NOTIFY,ASS_USER_STOP_RESULT,0);
		}		
	}
	SendWatchData(m_bMaxPeople,&TUserStopCardResult,sizeof(TUserStopCardResult),MDM_GM_GAME_NOTIFY,ASS_USER_STOP_RESULT,0);

	//�������Ƿ�ȫ��ͣ��
	if (CheckFinish())
		return TRUE;
	return TRUE;
}

/*---------------------------------------------------------------------------------*/
//����Ƿ����
BOOL CServerGameDesk::CheckFinish()
{
	OutputDebugString("err::CheckFinish(0)");

	CString liyitang;
	liyitang.Format("liyitang::����Ƿ����");
	OutputDebugString(liyitang);

	int iOpenCount = 0;
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i])									//have 
		{
			continue;
		}
		if (m_iUserStation[i] == TYPE_STOP_CARD)			//stop card
		{
			iOpenCount++;
		}
	}

	liyitang.Format("liyitang::���ͣ������:%d %d",iOpenCount,CountPlayer());
	OutputDebugString(liyitang);

	if(iOpenCount == CountPlayer())			
	{
		//����
		SetTimer(TIME_GAME_FINISH,1000);

		liyitang.Format("liyitang::���ͣ�ƽ����Ƿ�ɹ�");
		OutputDebugString(liyitang);

		return true;
	}
	OutputDebugString("err::CheckFinish(1)");
	return FALSE;
}
/*---------------------------------------------------------------------------------*/



//ͳ������Ϸ���
BYTE CServerGameDesk::CountPlayer()
{
	OutputDebugString("err::CountPlayer(0)");
	BYTE count=0;
	for(int i=0;i<m_bMaxPeople;i++)
	{
		if(m_pUserInfo[i] && m_iUserStation[i] != -1)
			count++;
	}
	OutputDebugString("err::CountPlayer(1)");
	return count;
}


BYTE CServerGameDesk::CountNotePeople()
{
	OutputDebugString("err::CountNotePeople(0)");
	int iCount=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i])		
			continue;
		if(m_iUserStation[i]!=TYPE_GIVE_UP && m_iUserStation[i]!=-1)
			iCount++;
	}
	OutputDebugString("err::CountNotePeople(1)");
	return iCount;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	OutputDebugString("lihexing::GameFinish(0)");
	//return true;
	//�õ���С���Ľ��
	long m_dwMaxPoint=0;
	bool bSetMaxMoney=false;
	KillAllTimer();

	//��������쳣��ֱ���㰲ȫ����
	if (CountPlayer() <= 1)
	{
		bCloseFlag = GF_SALE;
	}

	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			//�������� 
			m_bGameStation=GS_WAIT_NEXT;

			//��Ϸ����
			GameEndStruct TGameEnd;
			::memset(&TGameEnd,0,sizeof(TGameEnd));
			TGameEnd.iUpGradeStation=m_iUpGradePeople;
			::CopyMemory(TGameEnd.byOpenCardCount,m_byGetCardCount,sizeof(m_byGetCardCount));
			::CopyMemory(TGameEnd.iCardList,&m_iUserCard,sizeof(m_iUserCard));

			//�����ͻ��˵�ǰ����
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL) 
					continue;
				//���ʹ���
				if(m_iUserStation[i]!=TYPE_GIVE_UP)
				{
					TGameEnd.iUserState[i] = m_iUserStation[i];
					TGameEnd.iCardShape[i] = m_Logic.GetShape(m_iUserCard[i],m_byGetCardCount[i]);
				}
			}
			CString liyitang;
			for (int i=0;i<5;i++)
			{
				liyitang.Format("liyitang::ComputePoint(%d)",ComputePoint(i));
				OutputDebugString(liyitang);
			}

			__int64 iNtPoint = 0; //ׯ�����Ǯ������Ӯ�����мң�
			double  iNtLoseMoney = 0;//ׯ�����Ǯ��������Ӯ�����мҵģ�
			//�����м�Ǯ��
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL) 
					continue;
				if(i != m_iUpGradePeople)//�õ����мҵ�Ǯ��
				{
					//�мұ��� ��ׯ�Ҳ�����
					if( (TGameEnd.iCardShape[i]==UG_BUST_CARD) && (TGameEnd.iCardShape[m_iUpGradePeople]!=UG_BUST_CARD))
					{
						TGameEnd.i64TurePoint[i] = ComputePoint(i)*m_iPerJuTotalNote[i];
					}
					//�мҲ����� ��ׯ�ұ���
					else if( (TGameEnd.iCardShape[i]!=UG_BUST_CARD) && (TGameEnd.iCardShape[m_iUpGradePeople]==UG_BUST_CARD))
					{
						TGameEnd.i64TurePoint[i] = ComputePoint(i)*m_iPerJuTotalNote[i];
					}
					//ׯ�ұ��ƣ��мұ�
					else if((TGameEnd.iCardShape[i]==UG_BUST_CARD) && (TGameEnd.iCardShape[m_iUpGradePeople]==UG_BUST_CARD))
					{
						TGameEnd.i64TurePoint[i] = 0;
					}
					else
					{
						TGameEnd.i64TurePoint[i] = ComputePoint(i)*m_iPerJuTotalNote[i];		//������Ǯ	
					}
					//��Ӯ�����мҲ���Ǯ
					if (TGameEnd.i64TurePoint[i] + m_pUserInfo[i]->m_UserData.i64Money < 0) 
					{
							TGameEnd.i64TurePoint[i] = (-m_pUserInfo[i]->m_UserData.i64Money);    
					}
					//ׯ�����Ǯ
					if (TGameEnd.i64TurePoint[i] > 0)
					{
						iNtLoseMoney += TGameEnd.i64TurePoint[i];
					}
					iNtPoint += TGameEnd.i64TurePoint[i];	
				}
			}
			//����ׯ��Ǯ��
			if (m_pUserInfo[m_iUpGradePeople] != NULL)
			{
				__int64 dwNtMoney = m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money;
				if (iNtPoint>dwNtMoney)//����Ǯ��   2014��11��06�����޸Ĺ�  uslyt
				{

					for (int i = 0;i < PLAY_COUNT;i++)
					{
						if (m_pUserInfo[i]==NULL) 
						{
							continue;
						}
						if(i != m_iUpGradePeople && TGameEnd.i64TurePoint[i] > 0)//�м���������Ǯ��
						{
							TGameEnd.i64TurePoint[i] = (dwNtMoney+(iNtLoseMoney-iNtPoint)) * (TGameEnd.i64TurePoint[i]/iNtLoseMoney);
						}
					}
					TGameEnd.i64TurePoint[m_iUpGradePeople] = -m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money; 
				}
				else  //�㹻��Ǯ
				{
					TGameEnd.i64TurePoint[m_iUpGradePeople] = -iNtPoint;
				}
			}


			//д�����ݿ�				
			int temp_point[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			for (int i=0;i<PLAY_COUNT;i++)
			{
				temp_point[i]=TGameEnd.i64TurePoint[i];//+m_iAwardPoint[i];
				TGameEnd.iChangeMoney[i]=TGameEnd.i64TurePoint[i];	
			}
			//20121203dwj ��¼��������Ӯֵ �ڿ�˰֮ǰ��¼ �������ֻ�������Ӯ��ƽ������;
			RecordAiHaveWinMoney(&TGameEnd);

			ChangeUserPointint64(TGameEnd.i64TurePoint, temp_cut);
			
			__super::RecoderGameInfo(TGameEnd.iChangeMoney);

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(TGameEnd.i64TurePoint,0,sizeof(TGameEnd.i64TurePoint));
			}

			
			
			//��������
			for (int i=0; i < PLAY_COUNT; i++) 
			{
				if (m_pUserInfo[i]!=NULL) 
					SendGameData(i,&TGameEnd,sizeof(TGameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			}
			SendWatchData(m_bMaxPeople,&TGameEnd,sizeof(TGameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			
			//����ǿ�����
			ClearQuitPlayer();
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			OutputDebugString("err::GameFinish(1)");
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
	case GFF_SAFE_FINISH:
		{
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;
			
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			for (int i = 0; i < m_bMaxPeople; i++)
			{
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			}			
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			bCloseFlag = GFF_SAFE_FINISH;

			ClearQuitPlayer();
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			OutputDebugString("err::GameFinish(1)");
			return true;

		}
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			//��������
			m_bGameStation=GS_WAIT_ARGEE;//GS_WAIT_SETGAME;

			GameCutStruct CutEnd;
			memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			int total = 0;
			for (int i=0; i<PLAY_COUNT; i++)
			{
				total+=m_iPerJuTotalNote[i];
			}

			if (bDeskStation == m_iUpGradePeople)
			{
				CutEnd.i64TurePoint[bDeskStation]= -total*5;;
			}
			else
			{
				CutEnd.i64TurePoint[bDeskStation]= -m_iPerJuTotalNote[bDeskStation]*5;;//m_iTotalNote;			//�۳�ǿ�����ȫ����ע
			}

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			int i=0;
			for (i=0; i<PLAY_COUNT; ++i) 
			{ 
				temp_cut[i] = (bDeskStation == i)?true:false; 
			} 


			ChangeUserPointint64(CutEnd.i64TurePoint, temp_cut);
			//__super::RecoderGameInfo(CutEnd.iChangeMoney);

			__super::RecoderGameInfo(CutEnd.iChangeMoney);
			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(CutEnd.i64TurePoint,0,sizeof(CutEnd.i64TurePoint));
			}

			for (int i=0;i<m_bMaxPeople;i++)
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);

			bCloseFlag=GFF_FORCE_FINISH;

			ClearQuitPlayer();
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			OutputDebugString("err::GameFinish(1)");
			return true;
		}
	}

	//��������
	ReSetGameState(bCloseFlag);
	__super::GameFinish(bDeskStation,bCloseFlag);

	return true;
}

bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	//UserAI(bDeskStation, 1, true);
	return __super:: UserNetCut(bDeskStation, pLostUserInfo);
}

//���AI����
BOOL CServerGameDesk::UserAI(BYTE bDeskStation,BYTE bCutType,bool bNet)
{

	m_bIsCut[bDeskStation] = bNet;

	if(CountCutPlayer() == CountPlayer())	
	{
		GameFinish(0,GF_SALE);

		return FALSE;
	}
	if(1 == CountPlayer())	//��1��
	{
		GameFinish(0,GF_SALE);
	
		return FALSE;
	}

	return TRUE;
}
//ͳ�Ƶ������
BYTE CServerGameDesk::CountCutPlayer()
{
	OutputDebugString("err::CountCutPlayer(0)");
	BYTE count = 0;
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_pUserInfo[i] == NULL)
			continue;

		if(m_bIsCut[i])
		{	
			count ++;
		}
	}
	OutputDebugString("err::CountCutPlayer(1)");
	return count;
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{	
	if (m_iUserStation[bDeskStation] == -1)
	{
		CString lhx;
		lhx.Format("TestName::IsPlayGameA");
		OutputDebugString(lhx);
		return false;
	}
	if(m_bGameStation>=GS_ROB_NT&&m_bGameStation<GS_WAIT_NEXT)
	{
		CString lhx;
		lhx.Format("TestName::IsPlayGameB");
		OutputDebugString(lhx);
		return true;
	}
	else
	{
		CString lhx;
		lhx.Format("TestName::IsPlayGameC");
		OutputDebugString(lhx);
		return false;
	}
	//return __super::IsPlayGame(bDeskStation);
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{

	m_bUserReady[bDeskStation] = false ; 
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//��Ϸ��������
int CServerGameDesk::GetRoomBasePoint()
{
	
	int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����

	return (iBasePoint>0?iBasePoint:1);
}

//������ҷ���
int CServerGameDesk::ComputePoint(BYTE DeskStation)
{
	OutputDebugString("err::ComputePoint(0)");
	BYTE Shape = 0;
	int Point = 0;
	int winer = -1;
	if( m_Logic.CompareCard(m_iUserCard[DeskStation], m_byGetCardCount[DeskStation],m_iUserCard[m_iUpGradePeople],m_byGetCardCount[m_iUpGradePeople]) == 1)
	{
		winer = DeskStation;
		Point = 1;
	}
	else if(m_Logic.CompareCard(m_iUserCard[DeskStation], m_byGetCardCount[DeskStation],m_iUserCard[m_iUpGradePeople],m_byGetCardCount[m_iUpGradePeople]) == -1)
	{
		winer = m_iUpGradePeople;
		Point = -1;
	}
	else
	{
		winer = m_iUpGradePeople;
		Point = 0;
	}
	CString liyitang;
	liyitang.Format("liyitang::Point000(%d)",Point);
	OutputDebugString(liyitang);

	Shape = m_Logic.GetShape(m_iUserCard[winer],m_byGetCardCount[winer]);

	liyitang.Format("liyitang::Shape(%d)",Shape);
	OutputDebugString(liyitang);

	//���ϱ��ʵ��ܽ��
	Point = Point * m_bCardShapeBase[Shape];

	liyitang.Format("liyitang::Point111(%d)",Point);
	OutputDebugString(liyitang);
	
	OutputDebugString("err::ComputePoint(1)");
	return Point;
}
BYTE CServerGameDesk::GetIndex(int win)
{
	OutputDebugString("err::GetIndex(0)");
	BYTE Max = 0, Mix = 0;
	for (int i=1; i<PLAY_COUNT; i++)
	{
		if (m_Logic.CompareCard(&m_iTotalCard[Max*SH_USER_CARD],SH_USER_CARD,
			&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD)==-1)
			Max = i;
		if (m_Logic.CompareCard(&m_iTotalCard[Mix*SH_USER_CARD],SH_USER_CARD,
			&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD)==1)
			Mix = i;
	}
	if (win == 1)
	{
		OutputDebugString("err::GetIndex(1)");
		return Max;
	}
	else 
	{
		OutputDebugString("err::GetIndex(1)");
		return Mix;
	}

}

//����
BOOL CServerGameDesk::ChangeCard(BYTE bDestStation,BYTE bWinStation)
{
	OutputDebugString("err::ChangeCard(0)");
	for(int i = 0; i < SH_USER_CARD; i ++)
	{
		BYTE bTemp = m_iTotalCard[bDestStation * SH_USER_CARD + i];
		m_iTotalCard[bDestStation * SH_USER_CARD + i ] = m_iTotalCard[bWinStation * SH_USER_CARD + i];
		m_iTotalCard[bWinStation * SH_USER_CARD + i] = bTemp;
	}
	OutputDebugString("err::ChangeCard(1)");
	return TRUE;
}

//����ָ��������ҵ�����
void	CServerGameDesk::ChangeTwoUserCard(BYTE byFirstDesk,BYTE bySecondDesk)
{
	BYTE byTmpCard[SH_USER_CARD];
	ZeroMemory(byTmpCard,sizeof(byTmpCard));

	memcpy(byTmpCard,m_iUserCard[byFirstDesk],sizeof(byTmpCard));
	memcpy(m_iUserCard[byFirstDesk],m_iUserCard[bySecondDesk],sizeof(m_iUserCard[byFirstDesk]));
	memcpy(m_iUserCard[bySecondDesk],byTmpCard,sizeof(m_iUserCard[bySecondDesk]));
}

//��ȡ��һ�����λ��
BYTE CServerGameDesk::GetNextDeskStation(BYTE bDeskStation)
{
	OutputDebugString("err::GetNextDeskStation(0)");
	if(!m_bTurnRule)//˳ʱ��
	{
		int i=1;
		for (;i<PLAY_COUNT; i++)
		{
			if (m_pUserInfo[(bDeskStation + i ) % PLAY_COUNT] && m_iUserStation[(bDeskStation + i ) % PLAY_COUNT] != -1)
			{
				break;
			}			
		}
		OutputDebugString("err::GetNextDeskStation(1)");
		return (bDeskStation + i ) % PLAY_COUNT;
	}
	//��ʱ��
	int i = 4;
	for (;i>=1; i--)
	{
		if (m_pUserInfo[(bDeskStation + i ) % PLAY_COUNT] &&  m_iUserStation[(bDeskStation + i ) % PLAY_COUNT] != -1)
		{
			break;
		}			
	}
	OutputDebugString("err::GetNextDeskStation(1)");
	return 	(bDeskStation + i) % PLAY_COUNT;
}
/*---------------------------------------------------------------------------------*/
//���͸���һ����ׯ
BOOL	CServerGameDesk::SendCallScore(BYTE bDeskStation)
{
	OutputDebugString("err::SendCallScore(0)");
																
	CallScoreStruct TCallScore;
	TCallScore.iValue         = (m_iPrepareNT == 255) ? -1 : m_iCallScore[m_iPrepareNT];
	TCallScore.bDeskStation   = bDeskStation;						
	TCallScore.bCallScoreflag = true;	
	//��ҵ�״̬
	for(int i=0; i<PLAY_COUNT; i++)
	{
		TCallScore.byUserState[i] = m_iUserStation[i]; //����ҵ�״̬(����Ƿ���;����� ����һֱ����Ϸ���е����)
	}
	//��ǰ��ׯ��
	m_bCurrentCallScore = bDeskStation ;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		TCallScore.bPlayer = m_iUserStation[i] == -1?false:true;
		SendGameData(i,&TCallScore,sizeof(TCallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	}
	SendWatchData(m_bMaxPeople,&TCallScore,sizeof(TCallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);

	m_byLostTime = m_iCallScoreTime + 1;
	KillTimer(TIME_LOST_TIME);
	SetTimer(TIME_LOST_TIME,1000);

	SetTimer(TIME_CALL_NT,(m_iCallScoreTime+1)*1000);
	OutputDebugString("err::SendCallScore(1)");
	return TRUE;
}
/*---------------------------------------------------------------------------------*/
//��ׯ
BOOL	CServerGameDesk::UserCallScore(BYTE bDeskStation, int iVal)
{
	OutputDebugString("err::UserCallScore(0)");
	if(bDeskStation != m_bCurrentCallScore)
		return true;
	KillTimer(TIME_CALL_NT);
	if(0 == iVal)
	{	
		m_iCallScore[bDeskStation] = 0;		//====ĳλ�ò���ׯ
	}
	else
	{
		m_iPrepareNT = bDeskStation;				//����ׯ��
		CallScoreFinish();
		return true;
	}

	CallScoreStruct TCallScore;
	TCallScore.bDeskStation   = bDeskStation;
	TCallScore.bCallScoreflag = FALSE;					
	TCallScore.iValue         = m_iCallScore[bDeskStation];		//��ǰ�з����ͱ���

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&TCallScore,sizeof(TCallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&TCallScore,sizeof(TCallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);

	//��ȡ��һ����ҵ�λ��
	int iNextPeople = GetNextDeskStation(bDeskStation);

	if(m_iCallScore[iNextPeople] == 0)
	{
		m_iPrepareNT=m_iFirstJiao;
		m_iCallScore[m_iFirstJiao]=1;
		CallScoreFinish();
		return true;
	}

	if(m_iCallScore[iNextPeople] == -1)
	{
		SendCallScore(iNextPeople);
		return TRUE;
	}

	return true;
}
/*---------------------------------------------------------------------------------*/
//��ׯ����
BOOL	CServerGameDesk::CallScoreFinish()
{
	if (m_iPrepareNT == 255 )	//û���˽з�.���·���
	{
		GameFinish(0,GF_SALE);
		return true;
	}
	if (m_iCallScore[m_iPrepareNT] == 0 )	//û���˽з�.���·���
	{
		GameFinish(0,GF_SALE);
		return true;
	}
	//����������λ�ý�������
	m_iNtPeople = m_iPrepareNT;

	CallScoreStruct TScoreResult;
	TScoreResult.iValue			= m_iCallScore[m_iPrepareNT];
	TScoreResult.bDeskStation   = m_iPrepareNT;
	TScoreResult.bCallScoreflag = false;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&TScoreResult,sizeof(TScoreResult),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_FINISH,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&TScoreResult,sizeof(TScoreResult),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_FINISH,0);

	NoticeUserNote();
	OutputDebugString("err::CallScoreFinish(1)");
	return TRUE;
}
/*---------------------------------------------------------------------------------*/
//�õ���ҵ���ע����
__int64 CServerGameDesk::GetLimit(int bDeskStation)
{
	OutputDebugString("err::GetLimit(0)");

	int iCardShapeCount = MAX_SHAPE_COUNT;

	OutputDebugString("err::GetLimit(0-1)");
	//��Ϸ�����
	int MaxBase = 1; 	
	for (int i=0; i<iCardShapeCount; i++)
	{
		if (m_bCardShapeBase[i] > MaxBase)
		{
			MaxBase = m_bCardShapeBase[i];
		}
	}
	
	//ׯ�����ϵ�Ǯ
	__int64 i64NtNote = 0;
	if (NULL != m_pUserInfo[m_iUpGradePeople])
	{
		i64NtNote = m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money;
	}
	
	//�Լ����ϵ�Ǯ
	__int64 i64StationNote = 0;
	if (NULL != m_pUserInfo[bDeskStation])
	{
		i64StationNote = m_pUserInfo[bDeskStation]->m_UserData.i64Money;
	}
	
	//��ʵ��ҵ�����
	int iPlayerCount = CountPlayer();

	__int64 i64Limite = 0;
	if (iPlayerCount>1)
	{
		//���ׯ��ƽ�ֺ�ķ���
		i64Limite = i64NtNote/(iPlayerCount-1);

		if (m_iLimit > 0)
		{
			if (i64Limite > i64StationNote)
			{
				i64Limite = i64StationNote;
			}

			if (i64Limite > m_iLimit)
			{
				i64Limite = m_iLimit;
			}
			return i64Limite/MaxBase;
		}
		else if (m_pUserInfo[m_iUpGradePeople]!= NULL && m_pUserInfo[bDeskStation]!=NULL)
		{
			if (i64Limite > i64StationNote)
			{
				i64Limite = i64StationNote;
			}
			return i64Limite/MaxBase;
		}
	}
	OutputDebugString("err::GetLimit(1)");
	return 0;
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
///�������м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_NOTE_FINISH);			//���ƶ�ʱ��
	KillTimer(TIME_CALL_NT);				//��ׯ
	KillTimer(TIME_GAME_FINISH);			//��Ϸ������ʱ��

	KillTimer(TIME_NOTE);					 //��ע��ʱ��
	KillTimer(TIME_SEND_CARD);				//Ҫ����ɶ�ʱ��
	KillTimer(TIME_TAKE_CARD);				//Ҫ�ƶ�ʱ��
}
//��ȡ�������
int CServerGameDesk::GetMaxCardShape(BYTE iCard[], int iCardCount)
{
	OutputDebugString("err::GetMaxCardShape(0)");
	if (iCardCount < SH_USER_CARD*PLAY_COUNT)
	{
		return 0;
	}
	int shape = 0;
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if (m_pUserInfo[i] && m_iUserStation[i]!=-1)
		{
			int temp = m_Logic.GetShape(&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD);
			if (shape < temp)
			{
				shape = temp;
			}
		}
		
	}
	OutputDebugString("err::GetMaxCardShape(1)");
	return shape;
}
//-----------------------------------------------------------
/*�����˿�����Ӯ 
add by dwj 20121122
*/
void CServerGameDesk::IAWinAutoCtrl()
{
	OutputDebugString("err::IAWinAutoCtrl(0)");
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
	//CString strInfo;
	if (bAIWin)
	{//������ҪӮǮ
		if (CountAIWinMoney() < 0)
		{
			for (int i=0;i<PLAY_COUNT;i++)
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
	{//������Ҫ��Ǯ
		if (CountAIWinMoney() > 0 || (m_iAIHaveWinMoney +CountAIWinMoney())<0)
		{
			for (int i=0;i<PLAY_COUNT;i++)
			{			
				ChangeCard(i+1,0);
				if (CountAIWinMoney()<= 0)
				{
					if ((m_iAIHaveWinMoney +CountAIWinMoney())<0)
					{//���������������Ǯ�� ���»�������ӮǮ��Ϊ��������ô���»���;
						if (i != (PLAY_COUNT-1))
						{
							continue;
						}
						// �ƶ������˻��ǲ���������������ôֻ�����»���ȥ�û�����Ӯ��;
						for (int j = 0; j < PLAY_COUNT;j++)
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
	OutputDebugString("err::IAWinAutoCtrl(1)");
}
	
//------------------------------------------------------------------------------------
void CServerGameDesk::RecordAiHaveWinMoney(GameEndStruct *pGameEnd)
{
	OutputDebugString("err::RecordAiHaveWinMoney(0)");
	//bool bAllAi = true;//�����˺ͻ�������  �Ͳ�ȥ����;
	//for (int i =0 ;i< PLAY_COUNT ; i++)
	//{
	//	if(NULL != m_pUserInfo[i] && m_pUserInfo[i]->m_UserData.isVirtual < 1)
	//	{// ���������ң���ô��ȥ���㣬�����������ң�˵��ȫ�ǻ����ˣ��Ͳ���ȥ������ ;
	//		bAllAi = false;
	//		break;
	//	}
	//}
	//20121122dwj ���������ӮǮ�����;// 20121203 ������Ҫ�ڽ����˰֮ǰ���㣬��Ȼ����ֻ�������Ӯ��ƽ������
	if (/*!bAllAi && */m_bAIWinAndLostAutoCtrl)
	{//�����ϲ�ȫ�ǻ����ˣ� ���һ����˿�����
		try
		{//�����쳣������Ϊ���ˡ�����ţţ�漰���õ������ͬһ�������ļ���д��������������ˣ��Ͳ��������ļ���д����;
			//----DWJ-20121113---��
			__int64 iReSetAIHaveWinMoney;
			CString sTemp = CINIFile::GetAppPath ();/////����·����
			CINIFile f(sTemp +SKIN_FOLDER  + _T("_s.ini"));
			TCHAR szSec[_MAX_PATH] = TEXT("game");
			_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,m_pDataManage->m_InitData.uRoomID);
			iReSetAIHaveWinMoney = f.GetKeyVal(szSec,"ReSetAIHaveWinMoney ",(__int64)0);
			//CString strInfo;
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
					{//ֻ���������Ӯ
						//m_iAIHaveWinMoney += GameEnd.i64TurePoint[i];	
						m_iAIHaveWinMoney += pGameEnd->iChangeMoney[i];///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���ǮiChangeMoney
				
					}
				}
			}
			//����ǰ������Ӯ��Ǯд�������ļ�����
			sTemp.Format("%I64d",m_iAIHaveWinMoney);
			f.SetKeyValString(szSec,"AIHaveWinMoney ",sTemp);
			
		}
		catch (...)
		{
			//����ֻ�����������ʩ��ʲôҲ������DWJ
			OutputDebugString("dwjlog::д�����ļ�������!");
		}			
	}
	OutputDebugString("err::RecordAiHaveWinMoney(1)");
}
//-------------------------------------------------------------------------------
int CServerGameDesk::CountAIWinMoney()
{
	OutputDebugString("err::CountAIWinMoney(0)");
	int money = 0;
	BYTE iUserCard[PLAY_COUNT][10];	//��ʱ�������ҵ���;
	memset(&iUserCard, 0, sizeof(iUserCard));
	BYTE iShape[PLAY_COUNT];
	memset(&iShape, 0, sizeof(iShape));
	for (int i =0;i<PLAY_COUNT;i++)
	{
		for (int j =0;j<SH_USER_CARD;j++)
		{
			iUserCard[i][j] =  m_iTotalCard[i * SH_USER_CARD+j];
		}
		//�������ҵ��������ڱȽϴ�С��
		iShape[i] = m_Logic.GetShape(iUserCard[i],5);
	}
	
	if (m_iUpGradePeople!=-1 && m_pUserInfo[m_iUpGradePeople])
	{
		int itmpmoney = 0;
		if (m_pUserInfo[m_iUpGradePeople]->m_UserData.isVirtual)
		{///�����������ׯ��,������ʵ��ҵ�Ǯ;
			for(int i=0;i<PLAY_COUNT; i++)
			{
				if (NULL != m_pUserInfo[i] )
				{
					if (!m_pUserInfo[i]->m_UserData.isVirtual)
					{//��Ҳ��ǻ�����ddwwjj;
						//20121126dwj �Ƚϻ�������ׯ�ҵ��ƴ�С;
						if( m_Logic.CompareCard(iUserCard[i], SH_USER_CARD, iUserCard[m_iUpGradePeople],SH_USER_CARD) == 1)
						{//��ҵ����ʹ���ׯ�һ����ˣ���ô���ǻ�������Ǯ;
							itmpmoney -= m_bCardShapeBase[iShape[i]]*m_iPerJuTotalNote[i];
							//itmpmoney += m_bCardShapeBase[iShape[m_iUpGradePeople]]*m_iPerJuTotalNote[i];
						}
						else
						{
							//itmpmoney -= m_bCardShapeBase[iShape[i]]*m_iPerJuTotalNote[i];
							itmpmoney += m_bCardShapeBase[iShape[m_iUpGradePeople]]*m_iPerJuTotalNote[i];
						}					
					}	
				}
				
			}
			money = itmpmoney;
		}
		else
		{///��������˲���ׯ��,��������˵�Ǯ;
			for(int i=0;i<PLAY_COUNT; i++)
			{
				if (NULL != m_pUserInfo[i] )
				{
					if (m_pUserInfo[i]->m_UserData.isVirtual)
					{
						//20121126dwj �Ƚϻ�����ׯ������ҵ��ƴ�С;
						if(m_Logic.CompareCard(iUserCard[i], SH_USER_CARD, iUserCard[m_iUpGradePeople],SH_USER_CARD) == 1)
						{//�����˵����ʹ���ׯ�ҵ��ƣ�������ӮǮ
							itmpmoney += m_bCardShapeBase[iShape[i]]*m_iPerJuTotalNote[i];
							//itmpmoney -= m_bCardShapeBase[iShape[m_iUpGradePeople]]*m_iPerJuTotalNote[i];
						}
						else
						{
							itmpmoney -= m_bCardShapeBase[iShape[m_iUpGradePeople]]*m_iPerJuTotalNote[i];
							//itmpmoney += m_bCardShapeBase[iShape[i]]*m_iPerJuTotalNote[i];
						}					
					}
				}			
			}
			money = itmpmoney;
		}
	}

	return money;
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
			//�ҵ��˳������
			if (m_pUserInfo[byDeskStation]->m_UserData.dwUserID == m_vlSuperID.at(i))
			{
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------------------------------
//�޸Ľ�������1(ƽ̨->��Ϸ)

bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return true;
}

//�޸Ľ�������2(ƽ̨->��Ϸ)
bool CServerGameDesk::SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return true;
}

//�޸Ľ�������3(ƽ̨->��Ϸ)
bool CServerGameDesk::SetRoomPondEx(__int64	iReSetAIHaveWinMoney)
{
	return true;
}
//����ʤ��
BOOL CServerGameDesk::Judge()
{
	return true;
}
//ʤ
BOOL CServerGameDesk::JudgeWiner()
{
	return true;
}
//��
BOOL CServerGameDesk::JudgeLoser()
{
	return true;
}