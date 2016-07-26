#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"

//��Ϸ��չ����
BYTE CServerGameDesk:: m_iAllCardCount;
int CServerGameDesk:: m_iSendCardTime;
BYTE CServerGameDesk:: m_iThinkTime;
BYTE CServerGameDesk:: m_iBeginTime;
BYTE CServerGameDesk:: m_iCallScoreTime;
BYTE CServerGameDesk:: m_iXiaZhuTime;
BOOL CServerGameDesk:: m_bTurnRule;		//˳ʱ�뻹����ʱ��
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bGameStation=GS_WAIT_ARGEE;//��Ϸ״̬
	m_iLeaveArgee     = 0;								//ͬ���뿪
	m_iFirstOutPeople = -1;							//��һ����ע��
	m_iLastOutPeople  = -1;
	m_iUpGradePeople  = -1;						
						//�Ѿ�����ľ���
	m_iPrepareNT      = 255;
	//���׼��״̬Ϊ0��ʾ��û��׼��
	::memset(m_bUserReady , 0 , sizeof(m_bUserReady)) ; 
	//���̯��״̬Ϊ0��ʾ��û��̯��
	::memset(m_bReadyOpenCard , 0 , sizeof(m_bReadyOpenCard)) ; 
	memset(&m_SuperSetData,255,sizeof(m_SuperSetData));		//������������
	m_SuperSetData.bSetSuccese = false;
	m_bIsFirstLoadIni = true;						//��һ��ȥ�������ļ�
	memset(m_bIsSuper,0,sizeof(m_bIsSuper));
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	memset(m_iTotalCard,0,sizeof(m_iTotalCard));
	for(int i=0; i<PLAY_COUNT; i++)
		memset(m_bBullCard[i],0,sizeof(m_bBullCard[i]));
	memset(m_bAdd,0,sizeof(m_bAdd));
	memset(m_bIsCut,0,sizeof(m_bIsCut));
	memset(m_iOpenShape,0,sizeof(m_iOpenShape));
	memset(m_byOpenUnderCount,0,sizeof(m_byOpenUnderCount));
	memset(m_byOpenUnderCard,0,sizeof(m_byOpenUnderCard));
	memset(m_byOpenUpCard,0,sizeof(m_byOpenUpCard));
	memset(m_i64AcculateGrade,0,sizeof(m_i64AcculateGrade));
	m_bControlWin = 255;
	m_bControlLost = 255;
	m_iFirstJiao  = 255;
	m_iSuperCount=0;//��������
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
	m_iAllCardCount = f.GetKeyVal(key,"cardcount",54);
	m_iSendCardTime = f.GetKeyVal(key,"Sendcardtime",120);
	m_bTurnRule= f.GetKeyVal(key,"TurnRule",0);
	m_iCardShape &=0x00000000;
	m_iCardShape |= (f.GetKeyVal(key,"Yn",0)&0xFFFFFFFF);//��ţ
	m_iCardShape |= ((f.GetKeyVal(key,"Jn",0)<<1)&0xFFFFFFFF);//��ţ
	m_iCardShape |= ((f.GetKeyVal(key,"Bomb",0)<<2)&0xFFFFFFFF);//ը��
	m_iCardShape |= ((f.GetKeyVal(key,"Five",0)<<3)&0xFFFFFFFF);//��С
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
	m_iSuperCount = f.GetKeyVal(key, "SuperUserCount", 0);
	long  lUserID = 0;
	CString strText;
	m_vlSuperID.clear();
	for (int j=0; j<m_iSuperCount; j++)
	{
		strText.Format("SuperUserID[%d]", j);
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

	m_iLimit= f.GetKeyVal(key,"LimitNote",m_iLimit);	//���ע����ΪС�ڻ�=0�Ǿ��Զ����������Ǯ��ע
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
//-------------------------------------------------------------------------------------------------------------------------------------------


//��������
CServerGameDesk::~CServerGameDesk(void)
{
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
						tagUserProcess userBet;
						userBet.iNoteType = 3;
						userBet.iVrebType = TYPE_NOTE;				//��ע����
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
	case TIME_SEND_CARD_FINISH:	//���ƶ�ʱ��
		{
			KillTimer(TIME_SEND_CARD_FINISH);
			if (m_bGameStation == GS_SEND_CARD)
			{
				SendCardFinish();
			}
			return TRUE;
		}
	case TIME_AUTO_BULL:	//�Զ���ţ��ʱ��
		{
			KillTimer(TIME_AUTO_BULL);

			// ��ţǰ���ߣ���ׯ�����������û��עǰ���ߣ���ϵͳ�Զ������ţ��
			for (BYTE i=0; i<PLAY_COUNT; i++)
			{
				if (NULL!=m_pUserInfo[i] && m_iUserStation[i] != -1 && m_iUserStation[i] != TYPE_OPEN)
				{
					BYTE bResult[3];
					memset(bResult, 0, sizeof(bResult));
					tagUserProcess Verb;

					if (m_Logic.GetBull(m_iUserCard[i], 5, bResult))
					{
						::CopyMemory(m_bBullCard[i], bResult, sizeof(bResult));
					}
					else
					{
						memset(m_bBullCard[i], 0, sizeof(m_bBullCard[i]));
						m_bBullCard[i][0] = m_iUserCard[i][0];
						m_bBullCard[i][1] = m_iUserCard[i][1];
						m_bBullCard[i][2] = m_iUserCard[i][2];
					}
					UserOpenCard(i,&Verb);
				}
			}
			return TRUE;
		}
	case TIME_GAME_FINISH:	//������ʱ��
		{
			KillTimer(TIME_GAME_FINISH);
			if (m_bGameStation==GS_OPEN_CARD ) 
				GameFinish(0,GF_NORMAL);
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
				if(m_bGameStation < GS_ROB_NT || m_bGameStation >GS_OPEN_CARD)
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
					memset(&m_SuperSetData,255,sizeof(m_SuperSetData));		
					m_SuperSetData.bSetSuccese = false;

					memcpy(&m_SuperSetData,pSuperSet,sizeof(m_SuperSetData));

					m_SuperSetData.bSetSuccese = true;

					//��ȫ�ж�
					if (m_SuperSetData.byMaxDesk >=0 && m_SuperSetData.byMaxDesk < PLAY_COUNT )
					{
						if (-1 == m_iUserStation[m_SuperSetData.byMaxDesk])
						{
							m_SuperSetData.bSetSuccese = false;
						}
					}
					if (m_SuperSetData.byMinDesk >=0 && m_SuperSetData.byMinDesk < PLAY_COUNT )
					{
						if (-1 == m_iUserStation[m_SuperSetData.byMinDesk])
						{
							m_SuperSetData.bSetSuccese = false;
						}
					}
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
				return UserNoteResult(bDeskStation,pVerb->iVrebType,pVerb->iNoteType);

				return TRUE;
			}

		case ASS_USER_OPEN:		//�û���ţ
			{
				OutputDebugString("err::ASS_USER_OPEN(0)");
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

				UserOpenCard(bDeskStation,pData);
				OutputDebugString("err::ASS_USER_OPEN(1)");
				return true;
			}
		case ASS_USER_SCORE:	//��ҽз�
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
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	// ��֤�����û�Ȩ��
	SuperUserExamine(bDeskStation);
	//֪ͨ�����û����뱾��
	NotifyComeMes tComeDesk;
	tComeDesk.byDeskStation=bDeskStation;
	tComeDesk.i64AcculateGrade=m_i64AcculateGrade[bDeskStation];
	for (int i=0; i < PLAY_COUNT; i++) 
	{
		if (m_pUserInfo[i]!=NULL) 
			SendGameData(i,&tComeDesk,sizeof(tComeDesk),MDM_GM_GAME_NOTIFY,ASS_COME_DESK,0);
	}
	SendWatchData(m_bMaxPeople,&tComeDesk,sizeof(tComeDesk),MDM_GM_GAME_NOTIFY,ASS_COME_DESK,0);
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_WaiteAgree TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
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
			TGameStation.iAllCardCount	= m_iAllCardCount;		//�˿���Ŀ
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
	
			TGameStation.iCardShape		= m_iCardShape;			//��������

			//����Ƿ���׼��
			memcpy(TGameStation.bUserReady ,m_bUserReady , sizeof(TGameStation.bUserReady )) ;
			memcpy(TGameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(TGameStation.i64AcculateGrade )) ;
			
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
			TGameStation.iThinkTime		= m_iThinkTime;			//̯��ʱ��
			TGameStation.iBeginTime		= m_iBeginTime;			//׼��ʱ��
			TGameStation.iSendCardTime	= m_iSendCardTime;		//����ʱ��
			TGameStation.iCallScoreTime = m_iCallScoreTime;		//��ׯʱ��
			TGameStation.iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.iAllCardCount	= m_iAllCardCount;		//�˿���Ŀ
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��

			TGameStation.iCardShape		= m_iCardShape;			//��������

			TGameStation.byCurrentCallScore	= m_bCurrentCallScore;	//��ǰ��ׯ�� 
			for (int i=0; i<PLAY_COUNT; i++)
			{
				//�������ׯ���	255-��ʾ��û���� 0-��ʾ���� 1-��ʾ�Ѿ�����
				TGameStation.byUserCallStation[i] = m_iCallScore[i];
				//����ҵ�״̬ ����Ƿ���;�����
				TGameStation.iUserStation[i] = m_iUserStation[i];
			}
			memcpy(TGameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(TGameStation.i64AcculateGrade )) ;
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
			TGameStation.iAllCardCount	= m_iAllCardCount;		//�˿���Ŀ
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��

			TGameStation.iCardShape		= m_iCardShape;			//��������
			
			TGameStation.byNtStation	= m_iUpGradePeople;		//ׯ��λ��

	
			memcpy(TGameStation.i64UserNoteLimite,m_i64UserNoteLimite,sizeof(TGameStation.i64UserNoteLimite));
			for(int i=0;i<PLAY_COUNT;i++)
			{
				TGameStation.iUserStation[i]		= m_iUserStation[i];			//�������ע״̬ -1-��ʾ��û���� 0-��ʾ���� 1-��ʾ�Ѿ�����
				TGameStation.i64PerJuTotalNote[i]	= m_iPerJuTotalNote[i];			//�û�ÿ��ע���
				TGameStation.iLimitNote[i]			= m_iLimitNote[i];				//���ע��
			}
			memcpy(TGameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(TGameStation.i64AcculateGrade)) ;
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
			TGameStation.iAllCardCount	= m_iAllCardCount;		//�˿���Ŀ
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			TGameStation.iCardShape		= m_iCardShape;			//��������

			TGameStation.byNtStation	= m_iUpGradePeople;		//ׯ��λ��

			//�û����ϵ��˿�
			memcpy(TGameStation.iUserCard,m_iUserCard,sizeof(TGameStation.iUserCard));

			for(int i=0; i<PLAY_COUNT; i++)
			{
				TGameStation.iUserCardCount[i]		= m_iUserCardCount[i];			//�û������˿���Ŀ
				TGameStation.i64PerJuTotalNote[i]	= m_iPerJuTotalNote[i];			//�û�ÿ��ע���
				//����ҵ�״̬ ����Ƿ���;�����
				TGameStation.iUserStation[i] = m_iUserStation[i];
			}
			memcpy(TGameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(TGameStation.i64AcculateGrade )) ;
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return TRUE;
		}
	case GS_OPEN_CARD:	//��Ϸ��״̬
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
			TGameStation.iAllCardCount	= m_iAllCardCount;		//�˿���Ŀ
			TGameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			TGameStation.iCardShape		= m_iCardShape;			//��������

			TGameStation.byNtStation	= m_iUpGradePeople;		//ׯ��λ��
			
			//���״̬
			memcpy(TGameStation.iUserStation, m_iUserStation, sizeof(TGameStation.iUserStation));
			//�û�������
			::CopyMemory(TGameStation.iUserCardCount,m_iUserCardCount,sizeof(TGameStation.iUserCardCount));
			::CopyMemory(TGameStation.iUserCard,m_iUserCard,sizeof(TGameStation.iUserCard));
			//��ǰ���û���ע����
			::CopyMemory(TGameStation.i64PerJuTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));

			//��Ұ�ţ����
			for(int i=0; i<PLAY_COUNT; i++)
			{
				//���� ��ţ�� 
				TGameStation.iOpenShape[i] = m_iOpenShape[bDeskStation];
				TGameStation.byOpenUnderCount[i] = m_byOpenUnderCount[bDeskStation];
			}
			memcpy(TGameStation.byOpenUnderCard,m_byOpenUnderCard,sizeof(TGameStation.byOpenUnderCard));
			memcpy(TGameStation.byOpenUpCard,m_byOpenUpCard,sizeof(TGameStation.byOpenUpCard));
			memcpy(TGameStation.i64AcculateGrade ,m_i64AcculateGrade , sizeof(TGameStation.i64AcculateGrade )) ;
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
	for(int i=0; i<PLAY_COUNT; i++)
		memset(m_bBullCard[i],0,sizeof(m_bBullCard[i]));
	KillTimer(TIME_NOTE_FINISH);
	//	KillTimer(TIME_WAIT_NT);
	KillTimer(TIME_GAME_FINISH);



	KillTimer(TIME_AUTO_BULL);

	if ((bLastStation==GFF_FORCE_FINISH)||(bLastStation==GF_SALE))
	{
		
		//m_iUpGradePeople=-1;	

	}
	//���׼��״̬Ϊ0��ʾ��û��׼��
	memset(m_bUserReady,0,sizeof(m_bUserReady));
	//���̯��״̬Ϊ0��ʾ��û��̯��
	memset(m_bReadyOpenCard , 0 , sizeof(m_bReadyOpenCard)) ; 

	memset(m_iOpenShape,0,sizeof(m_iOpenShape));
	m_bControlWin = 255;
	m_bControlLost = 255;
	//	m_iUpGradePeople=-1;
	m_iLeaveArgee=0;
	m_iFirstOutPeople=-1;

	m_iLastOutPeople=-1;

	//m_iUpGradePeople=-1;
	m_iPrepareNT = m_iUpGradePeople;

	memset(&m_SuperSetData,255,sizeof(m_SuperSetData));		//������������
	m_SuperSetData.bSetSuccese = false;

	memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	//m_Logic.SetNTHuaKind(UG_ERROR_HUA);
	//memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_iUserCardCount, 0, sizeof(m_iUserCardCount));
	memset(m_bAdd, 0, sizeof(m_bAdd));
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

	//�����ۻ��ɼ�
	for(int i=0; i < PLAY_COUNT; i++) 
	{
		if (!m_pUserInfo[i]) 
		{
			m_i64AcculateGrade[i]=0;
		}
	}
	m_iIngPlayNote = -1;
	m_iLastShuttle = -1;

	//-ADD--BY---DWJ-20121122
	//����ÿ�̸��ݷ���ID��ȡ�������ļ��е�ReSetAIHaveWinMoney ���� �趨�»�����Ӯ�˶���Ǯ
	GetAIContrlSetFromIni(m_pDataManage->m_InitData.uRoomID);

	m_bGameStation = GS_ROB_NT;				//��Ϸ״̬
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
			else
			{
				OutputDebugString("zhtlog:ûϴ������");
			}
			m_Logic.RandCard(m_iTotalCard,m_iAllCardCount,m_bHaveKing);
		}
	}
	
	srand(GetTickCount());

	if (m_iUpGradePeople == -1 || m_pUserInfo[m_iUpGradePeople] == NULL)
	{
		m_bGameStation = GS_ROB_NT;

		if (m_iFirstJiao != 255)
		{
			m_iFirstJiao = GetNextDeskStation(m_iFirstJiao);
		}
		else
		{
			m_iFirstJiao = GetNextDeskStation(rand()%PLAY_COUNT);
		}

		SendCallScore(m_iFirstJiao);
	}
	else
	{
		CallScoreFinish();
	}
	
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

	BeginUpgradeStruct BeginMessage;
	memset(&BeginMessage,0,sizeof(BeginMessage));
	BeginMessage.AllCardCount=m_iAllCardCount;
	::CopyMemory(BeginMessage.iLimitNote,m_iLimitNote,sizeof(m_iLimitNote));
	memcpy(BeginMessage.i64UserNoteLimite,m_i64UserNoteLimite,sizeof(BeginMessage.i64UserNoteLimite));
	m_MaxPlayer = CountNotePeople() - 1;//�мҵ�����Ϊ����δ������ҵ�������ȥһ��ׯ��
	BeginMessage.iNt = m_iUpGradePeople;	 

	for (int i=0;i<m_bMaxPeople;i++) 
	{
		if(m_pUserInfo[i]&&m_iUserStation[i]!=TYPE_GIVE_UP)
			SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_CALL_NOTE,0);
		else
			continue;
	}
	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_CALL_NOTE,0);

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
	
	m_iPerJuTotalNote[bDeskStation] = m_i64UserNoteLimite[bDeskStation][iNoteType];
	m_iUserStation[bDeskStation] = TYPE_NOTE;

	m_MaxPlayer--;
	if (CountNotePeople() == 1)
	{
		GameFinish(0,GF_SALE);
		OutputDebugString("err::UserNoteResult(1)");
		return TRUE;
	}
	NoteResult noteresult;					
	noteresult.bAddStyle=iVerbType;										//��ע����
	noteresult.iOutPeople = bDeskStation;								//������ע��
	noteresult.iCurNote =  m_i64UserNoteLimite[bDeskStation][iNoteType];;
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
	if (m_SuperSetData.bSetSuccese)
	{
		//����
		if (m_SuperSetData.byMaxDesk != 255)
		{
			if (-1 != m_iUserStation[m_SuperSetData.byMaxDesk] && m_iUserCardCount[m_SuperSetData.byMaxDesk] == SH_USER_CARD)
			{
				for(int i=0; i<PLAY_COUNT; i++)
				{
					if (i != m_SuperSetData.byMaxDesk && -1 != m_iUserStation[i] && m_iUserCardCount[i] == SH_USER_CARD)
					{
						//�жϴ�С
						if( m_Logic.CompareCard(m_iUserCard[i], SH_USER_CARD, m_iUserCard[m_SuperSetData.byMaxDesk],SH_USER_CARD) == 1)
						{
							ChangeTwoUserCard(i,m_SuperSetData.byMaxDesk);
							//ChangeCard(i,m_SuperSetData.byMaxDesk);
						}
					}
				}
			}
		}

		//��С��
		if (m_SuperSetData.byMinDesk != 255)
		{
			if (-1 != m_iUserStation[m_SuperSetData.byMinDesk] && m_iUserCardCount[m_SuperSetData.byMinDesk] == SH_USER_CARD)
			{
				for(int i=0; i<PLAY_COUNT; i++)
				{
					if (i != m_SuperSetData.byMinDesk && -1 != m_iUserStation[i] && m_iUserCardCount[i] == SH_USER_CARD)
					{
						//�жϴ�С
						if( m_Logic.CompareCard(m_iUserCard[i], SH_USER_CARD, m_iUserCard[m_SuperSetData.byMinDesk],SH_USER_CARD) != 1)
						{
							ChangeCard(i,m_SuperSetData.byMaxDesk);
							//ChangeCard(i,m_SuperSetData.byMaxDesk);
						}
					}
				}
			}
		}
	}




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

	///����ʱ��
	SetTimer(TIME_SEND_CARD_FINISH,(CountNotePeople()*(m_iSendCardTime*SH_USER_CARD))+1000);

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
	m_bGameStation=GS_OPEN_CARD;

	//������Ϸ��ʼ��Ϣ
	BeginPlayStruct TBegin;
	for (int i=0;i<PLAY_COUNT;i++)
	{
		SendGameData(i,&TBegin,sizeof(TBegin),MDM_GM_GAME_NOTIFY,ASS_CALL_OPEN,0);
	}
	SendWatchData(m_bMaxPeople,&TBegin,sizeof(TBegin),MDM_GM_GAME_NOTIFY,ASS_CALL_OPEN,0);

	// ����ʱ��ţ
	SetTimer(TIME_AUTO_BULL, (m_iThinkTime+2)*1000);
	OutputDebugString("err::BeginPlayUpGrade(1)");
	return TRUE;
}
/*---------------------------------------------------------------------------------*/
//��Ұ�ţ
BOOL	CServerGameDesk::UserOpenCard(BYTE byDeskStation,void * pData)
{
	OutputDebugString("err::UserOpenCard(0)");
	UserTanPai *pUserTanPai = (UserTanPai *) pData;
	if (NULL == pUserTanPai)
	{
		return TRUE;
	}

	m_iUserStation[byDeskStation] = TYPE_OPEN;

	//���ŵ���ţ��
	memcpy(m_bBullCard[byDeskStation],pUserTanPai->byUnderCard,sizeof(pUserTanPai->byUnderCard));

	//������������������
	BYTE byUpCard[2];
	memset(byUpCard,255,sizeof(byUpCard));
	if (m_bBullCard[byDeskStation][0] != 0 && m_bBullCard[byDeskStation][1] != 0 && m_bBullCard[byDeskStation][2] != 0 && 3 == pUserTanPai->byUnderCount)
	{
		BYTE byTmpCard[SH_USER_CARD];
		memcpy(byTmpCard,m_iUserCard[byDeskStation],sizeof(byTmpCard));


		int iIndex = 0;
		for (int i=0; i<SH_USER_CARD; i++)
		{
			for(int j=0;j<3;j++)
			{
				if (m_bBullCard[byDeskStation][j] == byTmpCard[i])
				{
					byTmpCard[i] = 255;
					break;
				}
			}
		}

		for (int i=0; i<SH_USER_CARD; i++)
		{
			if (255 != byTmpCard[i])
			{
				byUpCard[iIndex] = byTmpCard[i];
				iIndex++;
				if (iIndex >=2 )
				{
					break;
				}
			}
		}
	}

	//�жϰ�ţ����
	int iShape = m_Logic.GetShape(m_iUserCard[byDeskStation],m_iUserCardCount[byDeskStation],m_bBullCard[byDeskStation]);

	UserTanPai TUserTanPaiResult;
	m_bReadyOpenCard[byDeskStation]=true;
	TUserTanPaiResult.iShape = iShape;
	TUserTanPaiResult.byDeskStation = byDeskStation;

	//�����������
	memcpy(TUserTanPaiResult.byUpCard,byUpCard,sizeof(TUserTanPaiResult.byUpCard));

	//����ţ������������
	TUserTanPaiResult.byUnderCount = pUserTanPai->byUnderCount;
	memcpy(TUserTanPaiResult.byUnderCard,pUserTanPai->byUnderCard,sizeof(TUserTanPaiResult.byUnderCard));
	//��ʶ������Ѿ���ţ��
	memcpy(TUserTanPaiResult.bReadyOpenCard,m_bReadyOpenCard,sizeof(m_bReadyOpenCard));


	//�����¼��Ұ�ţ��Ϣ(����������ʱ����Ҫ�õ���Щ����)
	//��ţ����
	m_iOpenShape[byDeskStation] = iShape;
	//��������
	m_byOpenUnderCount[byDeskStation] = TUserTanPaiResult.byUnderCount;		
	//���Ƶ�������
	memcpy(m_byOpenUnderCard[byDeskStation],TUserTanPaiResult.byUnderCard,sizeof(m_byOpenUnderCard[byDeskStation]));
	//��������2����
	memcpy(m_byOpenUpCard[byDeskStation],TUserTanPaiResult.byUpCard,sizeof(m_byOpenUpCard[byDeskStation]));

	for(int i = 0;i < PLAY_COUNT; ++i)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&TUserTanPaiResult,sizeof(TUserTanPaiResult),MDM_GM_GAME_NOTIFY,ASS_CALL_OPEN_RESULT,0);
		}
	}
	SendWatchData(m_bMaxPeople,&TUserTanPaiResult,sizeof(TUserTanPaiResult),MDM_GM_GAME_NOTIFY,ASS_CALL_OPEN_RESULT,0);
	OutputDebugString("err::UserOpenCard(1)");
	//����Ƿ񶼰�ţ��
	if (CheckFinish())
		return TRUE;
}
/*---------------------------------------------------------------------------------*/
//����Ƿ����
BOOL CServerGameDesk::CheckFinish()
{
	OutputDebugString("err::CheckFinish(0)");
	int iOpenCount = 0;
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i])
		{
			continue;
		}
		if (m_iUserStation[i] == TYPE_OPEN)
		{
			iOpenCount++;
		}
	}

	if(iOpenCount == CountPlayer())			
	{
		//����
		SetTimer(TIME_GAME_FINISH,1000);
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
	OutputDebugString("dwj4rnn::GameFinish(0)");
	//�õ���С���Ľ��
	long m_dwMaxPoint=0;
	bool bSetMaxMoney=false;
	KillTimer(TIME_AUTO_BULL);
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
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.iUpGradeStation=m_iUpGradePeople;
			::CopyMemory(GameEnd.iUpBullCard,&m_bBullCard,sizeof(m_bBullCard));
			::CopyMemory(GameEnd.iCardList,&m_iUserCard,sizeof(m_iUserCard));
			//�����ͻ��˵�ǰ����
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL) 
					continue;
				//���ʹ���
				if(m_iUserStation[i]!=TYPE_GIVE_UP)
				{
					GameEnd.iUserState[i] = m_iUserStation[i];

					GameEnd.iCardShape[i] = m_Logic.GetShape(m_iUserCard[i],m_iUserCardCount[i],m_bBullCard[i]);
				}
			}
			__int64 iNtPoint = 0; //ׯ�����Ǯ������Ӯ�����мң�
			double  iNtLoseMoney = 0;//ׯ�����Ǯ��������Ӯ�����мҵģ�
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL) 
					continue;
				if(i != m_iUpGradePeople)//�õ����мҵĵ÷ֵ�
				{
					
					GameEnd.iTurePoint[i] = ComputePoint(i)*m_iPerJuTotalNote[i];
					
					//20100802 zht ͬϲ�ֿͻ��޸�����Ӯ�����мҲ���Ǯ
					if (GameEnd.iTurePoint[i] + m_pUserInfo[i]->m_UserData.i64Money < 0)
					{
                         GameEnd.iTurePoint[i] = -m_pUserInfo[i]->m_UserData.i64Money;    
					}
					if (GameEnd.iTurePoint[i] > 0)
					{
						iNtLoseMoney+=GameEnd.iTurePoint[i];
					}
					iNtPoint += GameEnd.iTurePoint[i];
				}
			}
			//20100802 zht ͬϲ�ֿͻ��޸�����Ӯ����
			if (m_pUserInfo[m_iUpGradePeople] != NULL)
			{

				__int64 dwNtMoney = m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money;
				if (iNtPoint > dwNtMoney)//����Ǯ��
				{

					for (int i = 0;i < PLAY_COUNT;i++)
					{
						if (m_pUserInfo[i]==NULL) 
							continue;
						if(i != m_iUpGradePeople && GameEnd.iTurePoint[i] > 0)//�õ����мҵĵ÷ֵ�
						{
							GameEnd.iTurePoint[i] = (dwNtMoney+(iNtLoseMoney-iNtPoint)) *(GameEnd.iTurePoint[i]/iNtLoseMoney);
						}
					}
					GameEnd.iTurePoint[m_iUpGradePeople] = -m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money; 

				}
				else
				{
					GameEnd.iTurePoint[m_iUpGradePeople] = -iNtPoint;
				}
			}

			//д�����ݿ�				
			int temp_point[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			for (int i=0;i<PLAY_COUNT;i++)
			{
				temp_point[i]=GameEnd.iTurePoint[i];//+m_iAwardPoint[i];
				GameEnd.iChangeMoney[i]=GameEnd.iTurePoint[i];
				CString str;
			
			}
			//20121203dwj ��¼��������Ӯֵ �ڿ�˰֮ǰ��¼ �������ֻ�������Ӯ��ƽ������;
			RecordAiHaveWinMoney(&GameEnd);

			ChangeUserPointint64(/*temp_point*/GameEnd.iTurePoint, temp_cut);
			
			__super::RecoderGameInfo(GameEnd.iChangeMoney);
			//ͳ���ۻ��ɼ�
			for(int i=0; i < PLAY_COUNT; i++) 
			{
				if (0!=GameEnd.iChangeMoney[i] && m_pUserInfo[i]) 
				{
					m_i64AcculateGrade[i]+=GameEnd.iChangeMoney[i];
				}
			}
			memcpy(GameEnd.i64AcculateGrade,m_i64AcculateGrade,sizeof(GameEnd.i64AcculateGrade));
			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));
			}
			//��������

			for (int i=0; i < PLAY_COUNT; i++) 
			{
				if (m_pUserInfo[i]!=NULL) 
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			}
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			

			ClearQuitPlayer();

			//����ץ��ţţ �ͻ�װ
			int iNiuNiuCount = 0;
			int iDeskStation=-1;
			for(int i=0; i<PLAY_COUNT; i++)
			{
				if (m_iOpenShape[i] >= UG_BULL_BULL)
				{
					iNiuNiuCount++;
					if (1 == iNiuNiuCount)
					{
						iDeskStation = i;
					}
				}
			}
			//ֻ��һ����ץ��ţţ ��ô��һ�̾�������ׯ
			if (1 == iNiuNiuCount)
			{
				m_iUpGradePeople = iDeskStation;
			}
			if(iNiuNiuCount>1)
			{
				int iMaxUser = m_iUpGradePeople;
				//����ץ��ţţ ��ô��Ҫ������ţţ
				for(int i=0; i<PLAY_COUNT; i++)
				{
					if (m_iUpGradePeople == i || m_pUserInfo[i] == NULL)
					{
						continue;
					}
					if( m_Logic.CompareCard(m_iUserCard[i], m_iUserCardCount[i], m_bBullCard[i],
						m_iUserCard[iMaxUser],m_iUserCardCount[iMaxUser],m_bBullCard[iMaxUser]) == 1)
					{
						iMaxUser = i;
					}
				}
				
				m_iUpGradePeople = iMaxUser;
			}
			
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			OutputDebugString("err::GameFinish(1)");
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
	case GFF_SAFE_FINISH:
		{
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;
			/*for (int i = 0; i < m_bMaxPeople; i++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}*/
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			for (int i = 0; i < m_bMaxPeople; i++)
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
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
				CutEnd.iTurePoint[bDeskStation]= -total*5;;
			}
			else
			{
				CutEnd.iTurePoint[bDeskStation]= -m_iPerJuTotalNote[bDeskStation]*5;;//m_iTotalNote;			//�۳�ǿ�����ȫ����ע
			}

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			int i=0;
			for (i=0; i<PLAY_COUNT; ++i) 
			{ 
				temp_cut[i] = (bDeskStation == i)?true:false; 
			} 


			ChangeUserPointint64(CutEnd.iTurePoint, temp_cut);
			//__super::RecoderGameInfo(CutEnd.iChangeMoney);

			__super::RecoderGameInfo(CutEnd.iChangeMoney);
			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(CutEnd.iTurePoint,0,sizeof(CutEnd.iTurePoint));
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
	OutputDebugString("err::GameFinish(1)");
	return true;
}

//�û���������
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{
	UserAI(bDeskStation, 1, false);
	return __super:: UserReCome(bDeskStation, pNewUserInfo);
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
//ͳ������Ϸ���
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
		return false;
	}
	if(GS_ROB_NT==m_bGameStation || GS_NOTE==m_bGameStation || GS_SEND_CARD==m_bGameStation || GS_OPEN_CARD==m_bGameStation)
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
	if (m_iUpGradePeople == bDeskStation)
	{
		m_iUpGradePeople = -1;
	}
	m_bUserReady[bDeskStation] = false ; 
	m_i64AcculateGrade[bDeskStation]=0;
	__super::UserLeftDesk(bDeskStation,pUserInfo);
	LeaveDeskMes tLeaveDesk;
	tLeaveDesk.byDeskStation=bDeskStation;
	for (int i=0; i < PLAY_COUNT; i++) 
	{
		if (m_pUserInfo[i]!=NULL) 
			SendGameData(i,&tLeaveDesk,sizeof(tLeaveDesk),MDM_GM_GAME_NOTIFY,ASS_LEAVE_DESK,0);
	}
	SendWatchData(m_bMaxPeople,&tLeaveDesk,sizeof(tLeaveDesk),MDM_GM_GAME_NOTIFY,ASS_LEAVE_DESK,0);
	return __super::UserLeftDesk(bDeskStation, pUserInfo);
}
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	UserAI(bDeskStation, 1, true);
	return __super:: UserNetCut(bDeskStation, pLostUserInfo);
}


//�Α���A����
int CServerGameDesk::GetRoomBasePoint()
{
	int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����
	return (iBasePoint>0?iBasePoint:1);
}

//������ҷ���
int CServerGameDesk::ComputePoint(BYTE DeskStation)
{
	OutputDebugString("err::ComputePoint(0)");
	int Shape;
	int Point = 0;
	int winer = -1;
	if( m_Logic.CompareCard(m_iUserCard[DeskStation], m_iUserCardCount[DeskStation], m_bBullCard[DeskStation],
		m_iUserCard[m_iUpGradePeople],m_iUserCardCount[m_iUpGradePeople],m_bBullCard[m_iUpGradePeople]) == 1)
	{
		winer = DeskStation;
		Point = 1;
	}
	else
	{
		winer = m_iUpGradePeople;
		Point = -1;
	}
	Shape = m_Logic.GetShape(m_iUserCard[winer],m_iUserCardCount[winer],m_bBullCard[winer]);
	Point = Point * m_bCardShapeBase[Shape];
	
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
				break;
		}
		OutputDebugString("err::GetNextDeskStation(1)");
		return (bDeskStation + i ) % PLAY_COUNT;
	}
	//��ʱ��
	int i=3;
	for (;i>=1; i--)
	{
		if (m_pUserInfo[(bDeskStation + i ) % PLAY_COUNT] &&  m_iUserStation[(bDeskStation + i ) % PLAY_COUNT] != -1)
			break;
	}
	OutputDebugString("err::GetNextDeskStation(1)");
	return 	(bDeskStation + i) % PLAY_COUNT;
}
/*---------------------------------------------------------------------------------*/
//���͸���һ���з�
BOOL	CServerGameDesk::SendCallScore(BYTE bDeskStation)
{
	OutputDebugString("err::SendCallScore(0)");
																
	CallScoreStruct TCallScore;
	TCallScore.iValue = (m_iPrepareNT == 255) ? -1 : m_iCallScore[m_iPrepareNT];
	TCallScore.bDeskStation = bDeskStation;						
	TCallScore.bCallScoreflag = true;	
	//��ҵ�״̬
	for(int i=0; i<PLAY_COUNT; i++)
	{
		TCallScore.byUserState[i] = m_iUserStation[i];
	}

	m_bCurrentCallScore = bDeskStation ;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		TCallScore.bPlayer = m_iUserStation[i] == -1?false:true;
		SendGameData(i,&TCallScore,sizeof(TCallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	}

	SendWatchData(m_bMaxPeople,&TCallScore,sizeof(TCallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	SetTimer(TIME_CALL_NT,(m_iCallScoreTime+2)*1000);
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
	if(iVal == 0)
	{	
		m_iCallScore[bDeskStation] = 0;		//====ĳλ�ò��з�
	}
	else
	{
		m_iPrepareNT = bDeskStation;				//���з���
		goto NT;
	}
	CallScoreStruct callscore;
	callscore.bDeskStation = bDeskStation;
	callscore.bCallScoreflag = FALSE;					
	callscore.iValue = m_iCallScore[bDeskStation];		//��ǰ�з����ͱ���

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&callscore,sizeof(callscore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&callscore,sizeof(callscore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);

	int iNextPeople = GetNextDeskStation(bDeskStation);

	if(m_iCallScore[iNextPeople] == 0)
	{
		m_iPrepareNT=m_iFirstJiao;
		m_iCallScore[m_iFirstJiao]=1;
		goto NT;
	}

	if(m_iCallScore[iNextPeople] == -1)
	{
		SendCallScore(iNextPeople);
		OutputDebugString("err::UserCallScore(1)");
		return TRUE;
	}

NT:		
	CallScoreFinish();
	OutputDebugString("err::UserCallScore(1)");
	return true;
}
/*---------------------------------------------------------------------------------*/
//�зֽ���
BOOL	CServerGameDesk::CallScoreFinish()
{
	OutputDebugString("err::CallScoreFinish(0)");
	//if (m_iPrepareNT == 255 )	//û���˽з�.���·���
	//{
	//	GameFinish(0,GF_SALE);
	//	return true;
	//}
	//if (m_iCallScore[m_iPrepareNT] == 0 )	//û���˽з�.���·���
	//{
	//	GameFinish(0,GF_SALE);
	//	return true;
	//}

	CallScoreStruct scoreresult;
	scoreresult.iValue = m_iCallScore[m_iPrepareNT];
	scoreresult.bDeskStation  = m_iPrepareNT;
	scoreresult.bCallScoreflag = false;
	//��ҵ�״̬
	for(int i=0; i<PLAY_COUNT; i++)
	{
		scoreresult.byUserState[i] = m_iUserStation[i];
	}
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&scoreresult,sizeof(scoreresult),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_FINISH,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&scoreresult,sizeof(scoreresult),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_FINISH,0);

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
	int MaxBase = 1; //��Ϸ�����
	for (int i=0; i<iCardShapeCount; i++)
	{
		if (m_bCardShapeBase[i] > MaxBase)
		{
			MaxBase = m_bCardShapeBase[i];
		}
	}
	OutputDebugString("err::GetLimit(0-2)");
	__int64 i64Limite = 0;

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
	
	OutputDebugString("err::GetLimit(0-4)");
	//������ҵ�����
	int iPlayerCount = CountPlayer();
	OutputDebugString("err::GetLimit(0-5)");
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

	KillTimer(TIME_AUTO_BULL);				//�����Զ���ţ
	KillTimer(TIME_NOTE);             //��ע��ʱ��
}

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
void CServerGameDesk::RecordAiHaveWinMoney(GameEndStruct *GameEnd)
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
						//m_iAIHaveWinMoney += GameEnd.iTurePoint[i];	
						m_iAIHaveWinMoney += GameEnd->iChangeMoney[i];///��������Ӯ���ƣ��������Ѿ�Ӯ�˶���ǮiChangeMoney
				
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
#ifdef	SUPER_VERSION
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
#endif
	return false;
}
//---------------------------------------------------------------------------------------------------
