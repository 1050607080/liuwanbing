#include "StdAfx.h"

#include "clientgamedlg.h"


BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()
	//ON_MESSAGE(IDM_BEGIN,OnHitBegin)
	//ON_MESSAGE(IDM_LEFT_HITCARD,OnHitCard)
	//ON_MESSAGE(IDM_MOUSE_MOVE,OnMouseMove)
	//ON_MESSAGE(IDM_MOUSE_LEFT,OnMouseLeft)
	//ON_MESSAGE(IDM_HAVE_THING,OnHitHaveThing)
	//ON_MESSAGE(IDM_SEND_THING,OnSendHaveThing)
	//ON_MESSAGE(IDM_ARGEE_LEAVE,OnArgeeUserLeft)
	//ON_MESSAGE(IDM_STOP,OnStop)
	//ON_MESSAGE(IDM_STOP_THING,OnStopThing)				//������ǰ�����¼�
	//ON_MESSAGE(IDM_ARGEE_STOP,OnAgreeStop)				//ͬ����ǰ�����¼�
	//ON_MESSAGE(IDM_AUTO,OnHitAuto)						//�й�
	//ON_MESSAGE(IDM_ACTIVE_RESULT,OnHitActiveResult)		//�û�������
	//ON_MESSAGE(IDM_SHOW_NOTE,OnShowNote)				//�û���ע��ע������
	//ON_MESSAGE(IDM_SELECT_NOTE_NUM,OnSelectNote)		//�û�ѡ����ע
	//ON_MESSAGE(WM_SENDCARD_SHOWCARD,SendCardShowCard)

	//ON_MESSAGE(IDM_LOCK,OnHitLock)				        //����
	ON_WM_TIMER()
END_MESSAGE_MAP()

extern void MyMsgBox(const char *lpszFormat, ...);
extern void MyDug(const char *lpszFormat, ...);
CClientGameDlg * m_pCClientGameDlg = NULL;



/**
 * ����ǰ��ұ��ʵõ��ַ���
 * @param [in] int nNum Ҫ��ʾ������
 * @param [out] CString &Str ���ص��ַ���
 * @param [in] int nPower ��ұ���
 * @param [in] bool bIsBlank �����Ƿ�ӿո�
 * @return void
 */



//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	m_pCClientGameDlg = this;
	m_iVersion = DEV_HEIGHT_VERSION;
	m_iVersion2 = DEV_LOW_VERSION;
	
	m_iThinkTime = 0;
	m_iCardCount = 0;
	m_iNowOutPeople = 0;
	m_iFirstOutPeople = 0;
	m_bTimeOutCount = 0;
	m_iNowBigNote = 0;				//��ǰ���Ѻע��
	m_iThisTurnLimit = 0;				//������ע
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_DeskCard,0,sizeof(m_DeskCard));
	memset(m_UserCard,0,sizeof(m_UserCard));
	memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
	memset(m_iThisTurnNote,0,sizeof(m_iThisTurnNote));
	::memset(m_iUserStation,0,sizeof(m_iUserStation));
	::memset(m_bUserState,false,sizeof(m_bUserState));		//���״̬
	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));//��ұ�����ע����
	m_iThisNoteTimes = 0;
	for(int i = 0; i < PLAY_COUNT; i ++)
		m_szName[i][0] = '\0';

	m_iBeenPlayGame = 0;
	m_iRoomBasePoint = 1;
	m_iRunPublish = 0;
	m_iLimitNote = 0;
	m_iBaseNote = 0;
	
	m_byUserFollow = 0x00;
	m_byCurStyle = 0x00;
	m_bSeeCardAnimal = false;
	ZeroMemory(m_TempNote, 0, sizeof(m_TempNote));
	m_desk = -1;

	m_iAfterWaitAction = 2;		//��ʼ��Ϊ�Զ���ʼ 090311a1 JJ
   /* for (int i =0;i< PLAY_COUNT;i++)
    {
      m_PlayView.m_NoteFirst[i].ShowWindow(SW_SHOW);
    }
	*/

	m_bCanHitLock = false;
	m_iSendAnimaCardPos = 255 ;
	m_iSendCardTotle = 0 ;
	m_iNeedSendCardTotle = 0 ;
  	m_byAddNum = 0;												/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
	m_bShowHa       = 2;
	m_bIsSuperUser = false;		//�ж��Լ��Ƿ񳬼��ͻ��ˣ��ɷ������������ݣ�ÿ�ֶ�У��//lym 2011-11-28
	m_bPK = 0;
	m_iOutCardPeople = 255;
	m_bChangeRobotCard = false;
	m_iGameTimes = 0;


	CINIFile iniFile(CINIFile::GetAppPath()+"5RSHRobotSet.ini");



	m_bLeftOrGoOn = iniFile.GetKeyVal("RobotSet", "LeftOrGoOn",1);

	m_byGameTimes = iniFile.GetKeyVal("RobotSet", "GameTimes",1);

	m_byLeaveThinkTime = iniFile.GetKeyVal("RobotSet", "LeaveThinkTime",10);
	m_bNewTurn=false;
	m_i64HalfLessMoney=0;
	memset(m_iTempBt,0,sizeof(m_iTempBt));
}

//��������
CClientGameDlg::~CClientGameDlg()
{

}
int CClientGameDlg::GetGameStatus()
{
	 if(this)
        return GetStationParameter();
     else
        return 0;

}
//���ݰ󶨺���
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClass::DoDataExchange(pDX);
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClass::OnInitDialog();
	//////////////////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{

	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:		//�ȴ�ͬ��״̬
		{	
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			m_bIsSuperUser = pGameStation->bIsSuperUser;//�Լ��Ƿ񳬼��ͻ���//lym 2011-11-28
			//��������
			m_iThinkTime = pGameStation->iThinkTime;
			m_iBeginTime = pGameStation->iBeginTime;
		
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			m_iLimitNote = pGameStation->iLimitNote;			//���ע
			m_iBaseNote = pGameStation->iBaseNote;			//��ע
			//��עѡ��				
			m_byUserFollow = pGameStation->byUserFollow ;
			m_bShowHa      = pGameStation->bShowHa;
			m_byAddNum	   = pGameStation->m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13

			m_iThisTurnLimit = 0;
			m_iThisNoteTimes = 0;
			m_bPK            = pGameStation->bPK;
			::CopyMemory(m_bUserState,pGameStation->bUserState,sizeof(pGameStation->bUserState));	//�û�״̬
			ResetGameStation(RS_ALL_VIEW);

		
			SetGameTimer(GetMeUserInfo()->bDeskStation,rand()%3+1,ID_BEGIN_TIME);
		
			return TRUE;
		}
	case GS_SEND_CARD:	//����״̬
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pStationData;
			m_bIsSuperUser = pGameStation->bIsSuperUser;//�Լ��Ƿ񳬼��ͻ���//lym 2011-11-28
			//��������
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;		//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;				//���ܿ۷�
			//��������
			m_iThinkTime = pGameStation->iThinkTime;
			m_iBeginTime = pGameStation->iBeginTime;
			m_iLimitNote = pGameStation->iLimitNote;					
			m_iBaseNote = pGameStation->iBaseNote;	
			m_bShowHa      = pGameStation->bShowHa;
			m_byAddNum	   = pGameStation->m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
			//��עѡ��					
			m_byUserFollow = pGameStation->byUserFollow ;
			m_bPK            = pGameStation->bPK;
			m_bTimeOutCount = 0;					//��ʱ��������
			m_iNowBigNote = 0;						//��ǰ���Ѻע��
			m_iThisTurnLimit = m_iLimitNote/4; //NOTE_LIMIT+m_iBaseNote; ��һ����ע �ڶ���Ϊ �ǵ���ȫ˦��1/4֮��
			//��һ����ע
			m_iThisNoteTimes = 1;
			::CopyMemory(m_bUserState,pGameStation->bUserState,sizeof(pGameStation->bUserState));	//�û�״̬

			for (int i = 0; i < PLAY_COUNT;i++)
			{	
				//���ø���������
				int desk = ViewStation(i);
				//��������
				m_iDeskCardCount[desk] = pGameStation->iUserCardCount[i];
				if(m_bIsSuperUser && m_iDeskCardCount[desk]>0)//���ǳ����ͻ��˲��ܿ���//lym 2011-11-28
				{
					m_iDeskCardCount[desk] = 5;
				}
				::CopyMemory(m_UserCard[desk],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[desk]);
				::CopyMemory(m_DeskCard[desk],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[desk]);
				//if(!m_bIsSuperUser)//���ǳ����ͻ��˲��ܿ���//lym 2011-11-28
				//{
				//	m_DeskCard[desk][0] = 0;
				//}
			}

			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pStationData;
			m_bIsSuperUser = pGameStation->bIsSuperUser;//�Լ��Ƿ񳬼��ͻ���//lym 2011-11-28
			//��������
			//��������
			m_iThinkTime = pGameStation->iThinkTime;
			m_iBeginTime = pGameStation->iBeginTime;
			m_iLimitNote = pGameStation->iLimitNote;			//���ע
			m_iBaseNote = pGameStation->iBaseNote;				//��ע
			
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;	//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;			//���ܿ۷�
			//��עѡ��					
			m_byUserFollow = pGameStation->byUserFollow ;					
			m_bShowHa      = pGameStation->bShowHa;
			m_byAddNum	   = pGameStation->m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
			m_bPK            = pGameStation->bPK;

			m_iNowOutPeople = pGameStation->tag.iOutPeople;
			m_iFirstOutPeople = pGameStation->iFirstOutPeople;
			//���浱ǰ�û���ע�ܶ�
			::CopyMemory(m_iThisGameNote,pGameStation->iTotalNote,sizeof(pGameStation->iTotalNote));
			::CopyMemory(m_bUserState,pGameStation->bUserState,sizeof(pGameStation->bUserState));	//�û�״̬


			for (int i = 0;i < PLAY_COUNT; i ++)
			{	
				//���ø���������
				int desk = ViewStation(i);
				//��������
				m_iDeskCardCount[desk] = pGameStation->iUserCardCount[i];
				if(m_bIsSuperUser && m_iDeskCardCount[desk]>0)//���ǳ����ͻ��˲��ܿ���//lym 2011-11-28
				{
					m_iDeskCardCount[desk] = 5;
				}
				::CopyMemory(m_UserCard[desk],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[desk]);
				::CopyMemory(m_DeskCard[desk],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[desk]);
				//if(!m_bIsSuperUser)//���ǳ����ͻ��˲��ܿ���//lym 2011-11-28
				//{
				//	m_DeskCard[desk][0] = 0;
				//}
				m_iThisTurnNote[i] = pGameStation->iThisTurnNote[i];


				m_bNetCutPlayer[i] = pGameStation->ibNetCutPlayer[i];
				if (m_bNetCutPlayer[i] == TRUE)
				{
					BYTE iViewStation = ViewStation(i);
					memset(m_DeskCard[iViewStation],0,m_iDeskCardCount[iViewStation]);
				}
				m_iUserStation[i] = pGameStation->bUserState[i];
				m_iSendCardTotle+=m_iDeskCardCount[desk];
	
			}
          

			m_iNeedSendCardTotle=m_iSendCardTotle ;

	
			m_iThisNoteTimes = pGameStation->tag.bNoteTimes;

			// qy //����Ѻע����
			if (1 == m_iThisNoteTimes)  // ��һ��
			{
				m_iThisTurnLimit = m_iLimitNote/4;
			}
			else if (2 == m_iThisNoteTimes)
			{
				m_iThisTurnLimit = m_iLimitNote/2;
			}
			else
			{
				m_iThisTurnLimit = (m_iLimitNote - m_iLimitNote/2 - m_iLimitNote/4-m_iBaseNote);
			}
			if (true == m_bUserState[GetMeUserInfo()->bDeskStation])
			{
				//modified by yjp.��ע��ť���ݵ�ע������������
				__int64 ibt1 = (__int64)(m_iBaseNote*1.0);//(DOUBLE)m_iLimitNote*1.5/100;
				__int64 ibt2 = (__int64)(m_iBaseNote*2.0);//(DOUBLE)m_iLimitNote*3/100;
				__int64 ibt3 = (__int64)(m_iBaseNote*4.0);//(DOUBLE)m_iLimitNote*6/100;
				if (ibt1<= 0)
				{
					ibt1 = 1;
					ibt2 = 2;
					ibt3 = 4;
				}
			}
			//����Ѻע����
			/*if(pGameStation->tag.bNoteTimes >= SHOWHA_TIME)
				m_iThisTurnLimit = m_iLimitNote;
			else
				m_iThisTurnLimit = NOTE_LIMIT+m_iBaseNote;*/

			//����Ѻע����
			m_iNowBigNote = pGameStation->tag.iNowBigNote;

			//����ʱ��
			if (m_iNowOutPeople != -1) 
				SetGameTimer(m_iNowOutPeople,rand()%3+2,ID_OUT_CARD);
			//�Ƿ�Ϊ��ǰѺע��
			if (m_iNowOutPeople == GetMeUserInfo()->bDeskStation)
			{
				m_byCurStyle = pGameStation->tag.iNowBigNoteStyle;
				BOOL show = (pGameStation->tag.bNoteTimes >= m_bShowHa);					
				BOOL add = (m_byCurStyle  != TYPE_SHUTTLE) &&(/*!pGameStation->tag.bAddTimes&&*/m_iNowBigNote>=0);
				BOOL note = (m_iNowBigNote == 0);					
				BOOL follow = (m_byCurStyle  != TYPE_SHUTTLE && m_iNowBigNote != 0);	
				BOOL giveup = ((m_iThisNoteTimes > m_byUserFollow) || (m_byCurStyle == TYPE_SHUTTLE));
				Msg("Test:Button> %d,%d,%d,%d,%d", show,add,note,follow,giveup);

				// ���ڱ�����ע�����ܼ�ע
				if (m_iNowBigNote >= m_iThisTurnLimit)
				{
					add = FALSE;
				}
				//����Ѻע������ lym 2011-12-13
				if(m_byAddNum>0 && m_byAddNum!=255 && pGameStation->m_byUserAddNum[GetMeUserInfo()->bDeskStation] >= m_byAddNum)
				{
					add = FALSE;
				}


				// С��3�������
				if (m_iThisNoteTimes < m_bShowHa)
				{
					show = FALSE;
				}

				// ���������ʱ���ǿ��������
				if (TYPE_SHUTTLE == m_byCurStyle)
				{
					show = TRUE;
				}

				//m_PlayView.m_ShowNote.UpdateShowNoteBtn(show,note,add,follow,giveup/*,ibt1,ibt2,ibt3*/);				
			}
			char buff[500];
			sprintf(buff,"Ѻע���ԣ�m_byAddNum=%d ",m_byAddNum);
			OutputDebugString(buff);
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			GameStation_5 * pGameStation = (GameStation_5 *)pStationData;
			m_bIsSuperUser = pGameStation->bIsSuperUser;//�Լ��Ƿ񳬼��ͻ���//lym 2011-11-28
			//��������
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			//��������
			m_iThinkTime = pGameStation->iThinkTime;
			m_bShowHa      = pGameStation->bShowHa;
			m_byAddNum	   = pGameStation->m_byAddNum;		/**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13
			m_bPK            = pGameStation->bPK;
			//��עѡ��					
			m_byUserFollow = pGameStation->byUserFollow ;
			::CopyMemory(m_bUserState,pGameStation->bUserState,sizeof(pGameStation->bUserState));	//�û�״̬
			ResetGameStation(RS_ALL_VIEW);

			//��ʾ��ʼ��ť 
			if (GetMeUserInfo()->bUserState != USER_ARGEE)
			{
				SetGameTimer(GetMeUserInfo()->bDeskStation,rand()%3+2,ID_BEGIN_TIME);
			}
	
			return TRUE;
		}
	}
	return false;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	
	if (pNetHead->bMainID == MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_UG_USER_SET:		//�û������ƾ�
			{
				return TRUE;
			}
		case ASS_UG_USER_SET_CHANGE://���ø���
			{				
				return true;
			}
		case ASS_GAME_BEGIN:		//��Ϸ��ʼ
			{
			
				////////////////
				//m_uKanID = ::BZPlayFlash(this,CPoint(100,100),"MiPaiAnima\\miPai.xml",1,"Mipai|res\\animaCfg.xml|45,1,-30,60|17,0|50, 53,33");A
				////////////////
				if (uDataSize != sizeof(GameBeginStruct))
					return FALSE;

				memset((void *)&m_szName, 0,sizeof(m_szName));

				//������Ϸ״̬
				SetStationParameter(GS_SEND_CARD);
				m_bTimeOutCount = 0;												// ��ʱ����
				GameBeginStruct * pBegin=(GameBeginStruct *)pNetData;
				m_bNewTurn=false;
				memset(m_iTempBt,0,sizeof(m_iTempBt));
				m_bIsSuperUser = pBegin->bIsSuperUser;//�Լ��Ƿ񳬼��ͻ���//lym 2011-11-28

				m_iLimitNote = pBegin->iLimitNote;									// ���ע(һ��)
				m_iBaseNote = pBegin->iBaseNote;									// ��ע
				m_i64HalfLessMoney=pBegin->i64HalfLessMoney;
				for(int i=0;i<PLAY_COUNT;i++)
				{
					m_iPerJuTotalNote[i] += m_iBaseNote;
				}
				__int64 i64Temp=m_i64HalfLessMoney-m_iPerJuTotalNote[GetMeUserInfo()->bDeskStation];
				m_iTempBt[3] = (__int64)(i64Temp/1.0);
				m_iTempBt[2] = (__int64)(i64Temp/2.0);
				m_iTempBt[1] = (__int64)(i64Temp/3.0);
				m_iTempBt[0] = (__int64)(i64Temp/4.0);
				
				
				m_byCurStyle = 0x00;
				::CopyMemory(m_bUserState,pBegin->bUserState,sizeof(pBegin->bUserState));	//�û�״̬
				m_bChangeRobotCard = pBegin->bChangeRobotCard;

				Msg("AAAA ��ע--%I64d,,,%s",pBegin->iBaseNote,GetMeUserInfo()->szName);

				//�µ�ע
				for(int i = 0; i < PLAY_COUNT; i ++)
				{
					/*if(m_pUserInfo[i] == NULL || false == m_bUserState[i])
						continue;*/
					m_iThisGameNote[i] += m_iBaseNote;//(DOUBLE)m_iLimitNote*0.015;
					m_iUserStation[i] = -1;
				
					lstrcpy(m_szName[i],m_pUserInfo[i]->GameUserInfo.nickName);
				}

				if(m_byGameTimes > 0)
				{
					m_iGameTimes++;
				}

				return TRUE;
			}
		case ASS_NOTE_LIMIE://������ע
			{
				return true;
			}
	
		case ASS_SEND_CARD:			//����������
			{
				if(uDataSize != sizeof(SendOneTurnCardStruct))
				{
					return TRUE;
				}

				SendOneTurnCardStruct  *pSendData = (SendOneTurnCardStruct *)pNetData;

				for(int i = 0 ; i<PLAY_COUNT ; i++)
				{			
					BYTE   bDesk  = ViewStation(i) ; 

					m_iDeskCardCount[ViewStation(i)] = pSendData->iCardCount[i]; 
					memcpy(&m_UserCard[bDesk],&pSendData->iCardList[i],sizeof(BYTE)*pSendData->iCardCount[i]);
					memcpy(&m_DeskCard[bDesk],&pSendData->iCardList[i],sizeof(BYTE)*pSendData->iCardCount[i]);

					//if(!m_bIsSuperUser)//lym 2011-11-28
					//{
					//	m_DeskCard[bDesk][0] = 0x00;	
					//}

				}

				if(m_bIsSuperUser)//�����ͻ�����ʾ���е���
				{
					OutputDebugString("�����ͻ��ˣ�ASS_SEND_CARD");
				}

				CString str;

				for(int i = 0 ; i<PLAY_COUNT;i++)
				{
					BYTE  bdesk = ViewStation(i);
					for(int j = 0 ;j<5 ;j++)
					{
						CString str;
						str.Format("wys423client::pAllCard->iCardList[%d][%d]=%d,m_DeskCard[%d][%d]==%d  %d",i,j,pSendData->iCardList[i][j],bdesk,j,m_DeskCard[bdesk][j],m_iDeskCardCount[bdesk]);
						OutputDebugString(str);
					}
				}
				return TRUE ;

			}
		case ASS_COLLECT_JETON://�ռ�����
			{
				
				
				return TRUE;
			}
		case ASS_SEND_A_CARD:
			{
				if(uDataSize != sizeof(SendOneTurnCard))
				{
					return false;
				}
				
				m_iOutCardPeople = 255;
				::memset(m_iThisTurnNote,0,sizeof(m_iThisTurnNote));

				///���ܻ�����������������¼��㷢�Ƶ�������Ҫ��������

		
				SendOneTurnCard *pSendOneTurnCard = (SendOneTurnCard *)pNetData ; 

				for(int i = 0;i<PLAY_COUNT;i++)
				{
					BYTE iPos = (pSendOneTurnCard->bStartPos+i)%PLAY_COUNT;

					int desk = ViewStation(iPos);

					CString str;
					Msg("zhtlog:pSendOneTurnCard->iTurnCard[%d]=%d m_bUserState[iPos]=%d m_iUserStation[iPos]=%d",iPos,pSendOneTurnCard->iTurnCard[iPos],m_iUserStation[iPos]);

					if(0 !=pSendOneTurnCard->iTurnCard[iPos] /*&& NULL!=m_pUserInfo[iPos] &&\
						true == m_bUserState[iPos] && TYPE_GIVE_UP!=m_iUserStation[iPos]*/)
					{
						m_DeskCard[desk][m_iDeskCardCount[desk]] = pSendOneTurnCard->iTurnCard[iPos];		//�û�������

						m_UserCard[desk][m_iDeskCardCount[desk] ++] = pSendOneTurnCard->iTurnCard[iPos];		//�û�������
						//�޷��ƶ�����
						CString strr;
						strr.Format("zhtlog::pSendCard->iTurnCard[%d]==%d",iPos,pSendOneTurnCard->iTurnCard[iPos]);
						OutputDebugString(strr);

					}
				}

			   return true;
			}
		case ASS_SEND_FINISH:		//�������
			{
					return TRUE;
			}

		case ASS_GAME_PLAY:		//��ʼ��Ϸ
			{
				if (uDataSize != sizeof(BeginPlayStruct)) 
				{
					return FALSE;
				}

				SetStationParameter(GS_PLAY_GAME);	//������Ϸ״̬

				BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)pNetData;

				m_bTimeOutCount = 0;					//��ʱ��������
				m_iNowOutPeople = m_iFirstOutPeople = pBeginInfo->bDeskStaton ;
				m_iNowBigNote = 0;							//��ǰ���Ѻע��
				m_iThisTurnLimit = m_iLimitNote/4; //NOTE_LIMIT+m_iBaseNote; ��һ����ע �ڶ���Ϊ �ǵ���ȫ˦��1/4֮��
				//��һ����ע
				m_iThisNoteTimes = 1;
				//����ʱ��
				if(m_iNowOutPeople != -1)
				{
					SetGameTimer(m_iNowOutPeople,rand()%3+1,ID_OUT_CARD);
				}

				
				if (true == m_bUserState[GetMeUserInfo()->bDeskStation])
				{
					//modified by yjp.��ע��ť���ݵ�ע������������
					__int64 ibt1 = (__int64)(m_iBaseNote*1.0);//(DOUBLE)m_iLimitNote*1.5/100;
					__int64 ibt2 = (__int64)(m_iBaseNote*2.0);//(DOUBLE)m_iLimitNote*3/100;
					__int64 ibt3 = (__int64)(m_iBaseNote*4.0);//(DOUBLE)m_iLimitNote*6/100;
					if (ibt1<= 0)
					{
						ibt1 = 1;
						ibt2 = 2;
						ibt3 = 4;
					}
				}

				// �Լ��ǵ�һ����ע����
				if (GetMeUserInfo()->bDeskStation == m_iNowOutPeople)
				{	
					SetGameTimer(GetMeUserInfo()->bDeskStation ,rand() % GAME_RAND_TIME + 1,ID_OUT_CARD);
				}
			
				return TRUE;
			}
		case ASS_NOTE:								//���Ѻע���
			{
				if (uDataSize != sizeof(tagUserResult)) 
				{
					return TRUE;
				}

				tagUserResult *pResult = (tagUserResult *) pNetData;

				//�û���ע������ʾ��ע����,�ݲ���ֵ
				m_iNowOutPeople = pResult->iOutPeople;		//��ǰ��ע��

				//����ʱ��
				if (m_iNowOutPeople != -1) 
				{
					SetGameTimer(pResult->iOutPeople,rand()%3+2,ID_OUT_CARD);
				}
//1��	��1��ϵͳΪ�����˹�ͬ�µĵף������ò�ͨ���䲻ͬ�������µ׶
//2��	��2����ע��ǵ���ȫ˦��1/4֮�䣬
//3��	��3����ע��ǵ׵�ȫ˦��1/2֮�䣬
//4��	��4����ע���ǵ׺����֮��
//5�� ��5 ����ע���ǵ׺����֮��

				// qy //����Ѻע����
				if (1 == pResult->bNoteTimes)  // ��һ��
				{
					m_iThisTurnLimit = m_iLimitNote/4;
				}
				else if (2 == pResult->bNoteTimes)
				{
					m_iThisTurnLimit = m_iLimitNote/2;
				}
				else
				{
					m_iThisTurnLimit = (m_iLimitNote - m_iLimitNote/2 - m_iLimitNote/4-m_iBaseNote);
				}
				//if(pResult->bNoteTimes >= SHOWHA_TIME)
				//	m_iThisTurnLimit = m_iLimitNote;
				//else
				//	m_iThisTurnLimit = NOTE_LIMIT + m_iBaseNote;

				m_iThisNoteTimes = pResult->bNoteTimes;	//ϵͳ������ע����
				
				m_iNowBigNote = pResult->iNowBigNote;	//��ǰ���Ѻע

				if (pResult->iOutPeople == GetMeUserInfo()->bDeskStation )
				{
				
					BYTE style = pResult->iNowBigNoteStyle;			//��һ��������ע����
					m_byCurStyle = style;                           // ������ע����
					BOOL show = m_iThisNoteTimes >= m_bShowHa;					//�����ֲſ�����
					
					BOOL add;

					add = (/*!pResult->bAddTimes&&*/style != TYPE_SHUTTLE && m_iNowBigNote >= 0);
				                    
					//MyMsgBox("A:%d,%d,%d,%d",!pResult->bAddTimes,style,m_iNowBigNote,m_iThisTurnLimit);

					BOOL note = (m_iNowBigNote == 0);					//��ע,ֻ���ڵ�һ����ע��,���߱���û�����¹�ע
					BOOL follow = (style != TYPE_SHUTTLE /*&& m_iNowBigNote != 0*/);	//û����������ע���0
					BOOL giveup = 1;
					if (m_bPK)
					{
						giveup= ((m_iThisNoteTimes > m_byUserFollow) && (m_byCurStyle != TYPE_SHUTTLE));
						CString str;
						str.Format("zhtlog:giveup=%d m_byCurStyle=%d",giveup,m_byCurStyle);
						OutputDebugString(str);
					}
					else
					{
						giveup= ((m_iThisNoteTimes > m_byUserFollow) || (m_byCurStyle == TYPE_SHUTTLE));
					}
					
					
					// ���ڱ�����ע�����ܼ�ע
					if (m_iNowBigNote >= m_iThisTurnLimit)
					{
						add = FALSE;
					}
					//����Ѻע������ lym 2011-12-13
					if(m_byAddNum>0 && m_byAddNum!=255 && pResult->m_byUserAddNum[GetMeUserInfo()->bDeskStation] >= m_byAddNum)
					{
						add = FALSE;
					}
					char buff[500];
					sprintf(buff,"Ѻע���ԣ�m_byAddNum=%d ��ǰ����%d  ���%s",m_byAddNum,pResult->m_byUserAddNum[GetMeUserInfo()->bDeskStation],GetMeUserInfo()->nickName);
					OutputDebugString(buff);

					// С��2�������
					if (m_iThisNoteTimes < m_bShowHa)
					{
						show = FALSE;
					}

					// ���������ʱ���ǿ��������
					if (TYPE_SHUTTLE == style)
					{
						show = TRUE;
					}
					
					//m_PlayView.m_ShowNote.UpdateShowNoteBtn(show,note,add,follow,giveup);
					
					SetGameTimer(GetMeUserInfo()->bDeskStation ,rand() % GAME_RAND_TIME + 1,ID_OUT_CARD);
	}
				return true;
			}
		case ASS_NOTE_RESULT:											//Ѻע���
			{
				if (uDataSize != sizeof(NoteResult)) 
				{
					return FALSE;
				}

				KillAllTimer();
				NoteResult *noteresult = (NoteResult*)pNetData;
				m_iOutCardPeople = noteresult->iOutPeople;
				int iViewStation = ViewStation(noteresult->iOutPeople);
				::CopyMemory(m_iPerJuTotalNote,noteresult->iPlayerTotalNote,sizeof(noteresult->iPlayerTotalNote));
				
				//����Ƿ���������ʾ����
				if(noteresult->bNoteStyle == TYPE_GIVE_UP)
				{
				
					m_iUserStation[noteresult->iOutPeople] = TYPE_GIVE_UP;
					Msg("zhtlog: ASS_NOTE_RESULTiViewStation=%d",iViewStation);

					memset(m_DeskCard[iViewStation],0,m_iDeskCardCount[iViewStation]);
					memset(m_UserCard[iViewStation],0,sizeof(m_UserCard[iViewStation]));
									
				}
				else			//������ע
				{
					//colin090420��¼
				    m_TempNote[noteresult->iOutPeople] = noteresult->iCurNote;
					
					//����ĳһ������ע�ܶ�
					if(m_iThisNoteTimes < m_bShowHa)
						m_iThisGameNote[noteresult->iOutPeople] += noteresult->iCurNote;
					//������ע
					   m_iThisTurnNote[noteresult->iOutPeople] += noteresult->iCurNote;
					
				}

				return true;
			}
		case ASS_AI_STATION://����
			{
				if(uDataSize != sizeof(UseAIStation))
					return TRUE;
				UseAIStation *ai = (UseAIStation*)pNetData;
							
				if( m_iNowOutPeople == ai->bDeskStation)
					ReplaceUserNote(ai->bDeskStation);
				BYTE iViewStation = ViewStation(ai->bDeskStation);
				memset(m_DeskCard[iViewStation],0,m_iDeskCardCount[iViewStation]);
				m_bNetCutPlayer[ai->bDeskStation] = TRUE;
				Msg("zhtlog: ASS_NOTE_RESULTiViewStation=%d",iViewStation);
				return true;
			}
		case ASS_NEW_TURN://��һ����Ϸ��ʼ
			{
				if(uDataSize!=sizeof(NewTurn))
				{
					return 0;
				}	
				NewTurn * pNewTurn = (NewTurn *)pNetData;
				if (NULL == pNewTurn)
				{
					return 0;
				}
				m_bNewTurn=true;
				return true;
			}
		case ASS_NO_WINER://��ʤ�߲���
		case ASS_AHEAD_END://��ǰ����
		case ASS_SALE_END:	//��Ϸ��ȫ����
		case ASS_CUT_END:	//�û�ǿ���뿪	
		case ASS_CONTINUE_END:	//��Ϸ����
		case ASS_NO_CONTINUE_END:
			{
				SetStationParameter((pNetHead->bAssistantID == ASS_CONTINUE_END) ? GS_WAIT_NEXT : GS_WAIT_ARGEE);

				ResetGameStation(RS_GAME_END);

				KillAllTimer();

				srand(GetMeUserInfo()->bDeskStation);
				BYTE byTemp = 0; 
				if(m_byLeaveThinkTime <= 0)	//û�����û������뿪ʱ�� �Ͱ�ԭ���ķ�ʽ�趨��ʱ��
				{
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()%3+3,ID_BEGIN_TIME);
				}
				else
				{
					byTemp = rand()%m_byLeaveThinkTime; 
					byTemp = byTemp < 2?2:byTemp;
					SetGameTimer(GetMeUserInfo()->bDeskStation,byTemp,ID_RESULT_TIME);
				}

				return TRUE;
			}
	

		case ASS_PLAY_FLASH:
			{
							
				return TRUE;
			}

		case ASS_HAVE_THING:	//�û������뿪
			{
				return TRUE;
			}
		case ASS_LEFT_RESULT:	//�����뿪��� 
			{
				return TRUE;
			}
		case ASS_MESSAGE:
			{

				return true;
			}
		case ASS_STOP_THING://�Լ�������ǰ����
			{
				return true;
			}
		case ASS_AGREE_STOP://������ǰ�������
			{
				return true;
			}

		}
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	m_bTimeOutCount=0;

	m_iNowOutPeople=-1;

	m_iFirstOutPeople=-1;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_bUserState,false,sizeof(m_bUserState));		//���״̬
	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));//��ұ�����ע����

	return __super::ResetGameFrame();
}

//��ʼ��ť����
LRESULT	CClientGameDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{
	OnControlHitBegin();
	return 0;
}

bool CClientGameDlg::OnControlHitBegin()
{
	if ((GetStationParameter() == GS_WAIT_SETGAME
		||GetStationParameter() == GS_WAIT_NEXT
		||GetStationParameter() == GS_WAIT_ARGEE)
		)

	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	KillAllTimer();

	ResetGameStation(RS_GAME_BEGIN);

	return true;
}

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_RESULT_TIME);
	KillTimer(ID_OUT_CARD);

	return;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			if (uTimeCount <= 0)
			{

				OnHitBegin(0,0);	//ʱ�䵽��
			}

			return true;
		}

	case ID_RESULT_TIME:
		{
			if(uTimeCount <= 0)
			{
				KillAllTimer();

				srand(GetTickCount()+GetMeUserInfo()->bDeskStation);

				if(!m_bLeftOrGoOn && m_iGameTimes >= rand()%m_byGameTimes+2)
				{
					OnCancel();
				}
				else
				{
					OnHitBegin(0,0);	//ʱ�䵽��
				}
			}

			return true;
		}

	case ID_OUT_CARD:	//����
		{
			int iViewStation = ViewStation(bDeskStation);

			if (uTimeCount <= 0)	//ʱ�䵽��
			{
				if(m_iNowOutPeople == GetMeUserInfo()->bDeskStation)
				{
					UserNote();	
				}
				return true;
			}
			
			return true;
		}
	}
	return true;
}

//�����˿�
LRESULT CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{

	return 0L;
}
//colin09423����
LRESULT	CClientGameDlg::OnHitLock(WPARAM wparam, LPARAM lparam)
{

	return 0;
}
//�����˿�
LRESULT CClientGameDlg::OnMouseMove(WPARAM wparam, LPARAM lparam)
{

	return TRUE;
}

//�����˿�
LRESULT CClientGameDlg::OnMouseLeft(WPARAM wparam, LPARAM lparam)
{

	return TRUE;	
}

//�û�������
LRESULT CClientGameDlg::OnHitActiveResult(WPARAM wparam, LPARAM lparam)
{
	if (GetStationParameter() == GS_PLAY_GAME)
	{
		if (m_iNowOutPeople != GetMeUserInfo()->bDeskStation)
			return 0L;

		KillAllTimer();
	
		int ret = (int) wparam;					
		tagUserProcess process;
		process.iVrebType = ret;					// ��ע����
		process.iNote =(__int64)lparam;				// ��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
		SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
	}
	return 0L;
}

//�й�
LRESULT CClientGameDlg::OnHitAuto(WPARAM wparam,LPARAM lparam)
{
	return 0;
}

//�������°�ť����
LRESULT	CClientGameDlg::OnHitHaveThing(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

//��ǰ����
LRESULT	CClientGameDlg::OnStop(WPARAM wparam, LPARAM lparam)
{

	return 0;
}

//������ǰ����
LRESULT	CClientGameDlg::OnStopThing(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

//ͬ����ǰ����
LRESULT	CClientGameDlg::OnAgreeStop(WPARAM wparam, LPARAM lparam)
{
	return 0;
}


//�����뿪������
LRESULT	CClientGameDlg::OnSendHaveThing(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

//�û������뿪
LRESULT	CClientGameDlg::OnArgeeUserLeft(WPARAM wparam, LPARAM lparam)
{
	return 0;
}



//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	//��������
	m_iNowOutPeople = -1;
	m_iFirstOutPeople = -1;
	m_iNowBigNote = 0;							//��ǰ���Ѻע��

	m_iNeedSendCardTotle = 0 ; 
	m_iSendCardTotle = 0 ; 
	m_iSendAnimaCardPos = 255 ; 
	memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	m_iThinkTime = 0;
	m_iCardCount = 0;
	m_iNowOutPeople = 0;
	m_iFirstOutPeople = 0;
	m_bTimeOutCount = 0;
	m_iNowBigNote = 0;				//��ǰ���Ѻע��
	m_iThisTurnLimit = 0;				//������ע
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_DeskCard,0,sizeof(m_DeskCard));
	memset(m_UserCard,0,sizeof(m_UserCard));
	memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
	memset(m_iThisTurnNote,0,sizeof(m_iThisTurnNote));
	::memset(m_iUserStation,0,sizeof(m_iUserStation));
	::memset(m_bUserState,false,sizeof(m_bUserState));		//���״̬
	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));//��ұ�����ע����
	m_iOutCardPeople = 255;		/**< ���ڳ����û� */
	
	ZeroMemory(m_TempNote,0,sizeof(m_TempNote));
	switch (iGameStation)
	{
	case RS_ALL_VIEW:
		::memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
		::memset(m_iThisTurnNote,0,sizeof(m_iThisTurnNote));
		m_iThisNoteTimes = 0;

	case RS_GAME_CUT:
		{
			
			//���������
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

			break;
		}
	case RS_GAME_END:
		{

			break;
		}
	case RS_GAME_BEGIN:
		{
			//���������
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			::memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
			::memset(m_iThisTurnNote,0,sizeof(m_iThisTurnNote));
			ZeroMemory(m_TempNote,0,sizeof(m_TempNote));
			m_iThisNoteTimes = 0;
		
			break;
		}
	}
	return;
}

void CClientGameDlg::OnGameSetting()
{

	return;
}

//�Թ۸���
void CClientGameDlg::OnWatchSetChange(void)
{
	
}

//�û��뿪
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	return true;
}


//�ܷ��뿪����,�����,���ڿͻ����յ���Ϣ�������뿪.
BOOL CClientGameDlg::CanLeftDesk()
{

	//��Ϸ��δ����
	if(GetStationParameter() >= GS_SEND_CARD && GetStationParameter() < GS_WAIT_NEXT)
		return FALSE;
	return TRUE;
}

//
void CClientGameDlg::OnCancel()
{
	//��ѯ״̬
	if (! CanLeftDesk())
	{
		return;
	}

	KillAllTimer();

	AFCCloseFrame();

	SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);		//����������Ϣ

	__super::OnCancel();
}

//�޸��û�Ѻע��� ��
BOOL CClientGameDlg::ModifyNote(BYTE iViewStation,__int64 iCurNote,BOOL bExtVal)
{

	return true;
}


//��������
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{	
	
	return TRUE;
}

//�û���ע
LRESULT CClientGameDlg::OnShowNote(WPARAM wparam,LPARAM lparam)
{
	// ֻ��Ӧ��ע����ע
	if ((TYPE_ADD != wparam) && (TYPE_NOTE != wparam))
	{
		return TRUE;
	}
	__int64 MeNote = m_iThisGameNote[GetMeUserInfo()->bDeskStation];

	//
	int i = 0;
	while(pow((double)10,i)< (m_iBaseNote+1))
	{
		i++;
	}
	int iBase = 1;
	if (i > 0)
	{
		iBase = (int)pow((double)10,(i-1));
	}

	__int64 iMin = m_iBaseNote;
	if (TYPE_ADD == wparam)
	{
		iMin = m_iNowBigNote + m_iBaseNote;  // ���ټӵ�ע���
	}


	return TRUE;
}

__int64 CClientGameDlg::GetMaxNote(__int64 a[4])
{
	__int64 p = -1;
	for (int i = 0; i < 4; i++)
	{
		if (a[i] > p)
		{
			p = a[i];
		}
	}
	return p;
}

//�û�ѡ����ע��
LRESULT CClientGameDlg::OnSelectNote(WPARAM wparam,LPARAM lparam)
{
	int ret = (int)wparam;
	switch(ret)
	{
	case 1://���
		{			

			tagUserProcess process;
			memset(&process,NULL,sizeof(process));

			//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ) ��Ϊ��Ϊ__int64��lparam�޷����ɶ��޸�  xyh:20110729
			switch((int)lparam)
			{
			case 0:     //��һ����ע��ť
				{
					process.iNote = 4*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
					break;
				}
			case 1:     //�ڶ�����ע��ť
				{
					process.iNote =  2*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
					break;
				}
			case 2:     //��������ע��ť
				{	
					process.iNote =  1*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
					break;
				}
			default:
				break;
			}
			process.iVrebType = (m_iNowBigNote == 0)?TYPE_NOTE:TYPE_ADD;//��ע����
			SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
		}	

	default:
		break;
	}
	return TRUE;
}

//���ƶ�����ʾ��Ϻ�,��ʾ������
LRESULT CClientGameDlg::SendCardShowCard(WPARAM wparam,LPARAM lparam)
{
	return 0;
}

//������������ע
BOOL CClientGameDlg::ReplaceUserNote(BYTE bDeskStation)
{
	return TRUE;
}

//�����ע
BOOL CClientGameDlg::UserNote()
{
	if((m_iThisNoteTimes<= m_byUserFollow) && (m_byCurStyle != TYPE_SHUTTLE))
	{
		if(m_iNowBigNote == 0)	// ��һ����ע��
		{
			int OutNote[3]={1,2,4};	// ��עΪһ�ֵ������ע
			if(m_bNewTurn)
			{
				OnHitActiveResult(TYPE_NOTE,m_iBaseNote*OutNote[rand()%3]);
			}
			else
			{
				OnHitActiveResult(TYPE_NOTE,m_iTempBt[rand()%4]);
			}
		}
		else	// �ǵ�һ����ע��
		{
			OnHitActiveResult(TYPE_FOLLOW,0);	// ��ע
		}
	}
	else
	{
		
		int nCardCount = m_iDeskCardCount[GetMeUserInfo()->bDeskStation];

		//3-4���Ƶ�ʱ��
		if(nCardCount>=3 )
		{
	
			
			if (nCardCount == 5) //���һ�ֵ�ʱ��  /*ADD BY DWJ 2013-3-26*/
			{
				bool bTmpFollow = true; //����Ƿ��ע
				BYTE iTmpMyCardType = m_Logic.GetCardShape(m_UserCard[GetMeUserInfo()->bDeskStation], nCardCount);
				
				for (BYTE i=0; i<PLAY_COUNT; ++i)
				{
					// ������Լ�������
					if(GetMeUserInfo()->bDeskStation == i || m_iUserStation[i] == TYPE_GIVE_UP || m_iDeskCardCount[i] < nCardCount)
					{
						continue;
					}
					//�ж��Ƿ��������ͱ��Լ����ʹ����Ҵ���,
					if (iTmpMyCardType < m_Logic.GetCardShape(&m_DeskCard[i][1], nCardCount-1))
					{
						//������������������;ͱ��Լ��������ͻ���, ��ô��Ҫ����
						bTmpFollow = false;
						break;
					}
				}	
				if (!bTmpFollow)
				{
					OutputDebugString("dwjlog::����С����");
					OnHitActiveResult(TYPE_GIVE_UP,0);	// ����
					return FALSE;
				}
			}/*ADD BY DWJ 2013-3-26*/

			for (BYTE i=0; i<PLAY_COUNT; ++i)
			{
				// ������Լ�������
				if(GetMeUserInfo()->bDeskStation == i || m_iUserStation[i] == TYPE_GIVE_UP || m_iDeskCardCount[i] < nCardCount)
				{
					continue;
				}

				if (m_Logic.CompareCard(&m_DeskCard[i][1], nCardCount-1, m_UserCard[GetMeUserInfo()->bDeskStation], nCardCount)>=0
					&&(m_Logic.GetCardShape(&m_DeskCard[i][1], nCardCount-1) >= SH_DOUBLE||nCardCount==5))
				{
					//���������עΪ 0 ��� ���� �����û�л��Լ�����
					if(m_iOutCardPeople !=255 && m_iThisTurnNote[m_iOutCardPeople]<=0 || m_bChangeRobotCard)	
					{
						OnHitActiveResult(TYPE_FOLLOW,0);
						return TRUE;
					}
					OnHitActiveResult(TYPE_GIVE_UP,0);	// ����

					return FALSE;
				}
				else
				{
					if (m_Logic.GetCardShape(m_UserCard[GetMeUserInfo()->bDeskStation], nCardCount) >= SH_TWO_DOUBLE)
					{
						if(m_iNowBigNote == 0)	// ��һ����ע��
						{
							if(m_bNewTurn)
							{
								OnHitActiveResult(TYPE_SHUTTLE,m_iThisTurnLimit);	// ��עΪһ�ֵ������ע
							}
							else
							{
								OnHitActiveResult(TYPE_NOTE,m_iTempBt[3]);
							}
							

							return TRUE;
						}
						else				// �ǵ�һ����ע��
						{
							OutputDebugString("dwjlog::m_bChangeRobotCard��ע");
							OnHitActiveResult(TYPE_FOLLOW,0);	// ��ע

							return TRUE;
						}
					}
					
				}
			}
		}
		

		if(m_iNowBigNote == 0)	// ��һ����ע��
		{
			// ��עΪһ��
			int OutNote[3]={1,2,4};
			if(m_bNewTurn)
			{
				OnHitActiveResult(TYPE_NOTE,m_iBaseNote*OutNote[rand()%3]);
			}
			else
			{
				OnHitActiveResult(TYPE_NOTE,m_iTempBt[rand()%4]);
			}

			
		}
		else// �ǵ�һ����ע��
		{
			int CardShape = m_Logic.GetCardShape(m_UserCard[GetMeUserInfo()->bDeskStation], nCardCount);
			if (m_byCurStyle == TYPE_SHUTTLE )
			{
              if (CardShape < SH_DOUBLE&&!m_bChangeRobotCard) //�������ˣ�����С�ڶ��� ���ҷ����û�л��Լ�����
			  {
				  // ����
				  OnHitActiveResult(TYPE_GIVE_UP,0);
				  return FALSE;
			  }
			}
			if (CardShape<SH_DOUBLE&&!m_bChangeRobotCard)
			{
				int iProp = rand() % 10 - nCardCount;

				if (iProp < 0)
				{
					OnHitActiveResult(TYPE_GIVE_UP,0);	// ����
					return FALSE;
				}
			}
			
			OnHitActiveResult(TYPE_FOLLOW,0);			// ��ע
		}
	}
	return true;
}

//���Ż��㵽��ͼλ��
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{
	return bDeskStation;
}

BYTE CClientGameDlg::LogicStation(BYTE deskStation)
{
	if (deskStation < 0 || deskStation > PLAY_COUNT)
	{
		
		return 0;
	}
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (ViewStation(i) == deskStation)
		{
			return i;
		}
	}
}
