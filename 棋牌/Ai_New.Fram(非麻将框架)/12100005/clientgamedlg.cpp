#include "StdAfx.h"
#include "ClientGameDlg.h"
#include ".\clientgamedlg.h"


BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(IDM_CALL_SCORE,OnCallScore)				//�з�
	//ON_MESSAGE(IDM_RIGHT_HITCARD,OnRightHitCard)
	//ON_MESSAGE(IDM_STOP,OnStop)
	//ON_MESSAGE(IDM_STOP_THING,OnStopThing)			//������ǰ�����¼�
	//ON_MESSAGE(IDM_ARGEE_STOP,OnAgreeStop)			//ͬ����ǰ�����¼�
	ON_MESSAGE(IDM_AUTO,OnHitAuto)				//�й�
	//ON_MESSAGE(IDM_PASS,OnHitPass)
	//	ON_MESSAGE(IDM_USER_NT,OnHitNt)
	//ON_MESSAGE(IDM_ACTIVE_RESULT,OnHitActiveResult)//�û�������
	//ON_MESSAGE(IDM_SHOW_NOTE,OnShowNote)			//�û���ע��ע������
//	ON_MESSAGE(IDM_SELECT_NOTE_NUM,OnSelectNote)		//�û�ѡ����ע
//	ON_MESSAGE(WM_SENDCARD_SHOWCARD,SendCardShowCard)
	//	ON_MESSAGE(WM_SETMESSAGE,OnSetMessage)
	ON_WM_TIMER()
	//	ON_WM_CLOSE()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	m_iThinkTime=0;
	m_iCardCount=0;
	m_iNowOutPeople=0;
	m_iFirstOutPeople = 0;
	//m_PlayView.SetNTStation();
	m_bTimeOutCount=0;
	m_bCurrentOperationStation = 255;//��ǰ�з���
	//ϵͳ������
	//m_iSysCardCount=28;
	m_iNowBigNote=0;				//��ǰ���Ѻע��
	m_iThisTurnLimit=0;				//������ע
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	
	m_GameType = 0;
	m_hMciWnd1=NULL;
	m_hMciWnd2=NULL;

	m_iGameTimes = 0;

	CINIFile iniFile(CINIFile::GetAppPath()+"NNRobotSet.ini");

	m_bLeftOrGoOn = iniFile.GetKeyVal("RobotSet", "LeftOrGoOn",1);

	m_byGameTimes = iniFile.GetKeyVal("RobotSet", "GameTimes",1);

	m_byLeaveThinkTime = iniFile.GetKeyVal("RobotSet", "LeaveThinkTime",0);

	// �����˿��ٲ�������
	m_bQuick = iniFile.GetKeyVal("RobortSet", "Quick", 1);
	m_bMiddForQuick = m_bQuick;
	CString strTemp;
	for(int i = 0; i < 4; ++i)
	{
		strTemp.Format("ProNote[%d]",i);
		m_iProNote[i] = iniFile.GetKeyVal("RobotSet",strTemp,25);
	}


	m_bUseMilliSecond = iniFile.GetKeyVal("RobortSet" , "UseMilliSecond" , 1)>0?true:false; ///ʹ�ú���Ϊ��λ

	m_iMilliSecond =  iniFile.GetKeyVal("RobortSet" , "MilliSecond" , 200) ; ///ʹ�ú���Ϊ��λ

	if(m_iMilliSecond<30 ||m_iMilliSecond>300000)
	{
		m_iMilliSecond = 300 ;
	}

	CString lhx;
	lhx.Format("lihexing::_AI %d %d",m_bUseMilliSecond,m_iMilliSecond);
	OutputDebugString(lhx);

}

//��������
CClientGameDlg::~CClientGameDlg()
{
	if(m_hMciWnd1)
	{
//		MCIWndStop(m_hMciWnd1);
//		MCIWndDestroy(m_hMciWnd1);
		m_hMciWnd1=NULL;
	}

	if(m_hMciWnd2)
	{
//		MCIWndStop(m_hMciWnd2);
//		MCIWndDestroy(m_hMciWnd2);
		m_hMciWnd2=NULL;
	}	

}

//���ݰ󶨺���
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClass::DoDataExchange(pDX);
//	DoDataExchangeWebVirFace(pDX);
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClass::OnInitDialog();

	return TRUE;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	m_iMyDeskStation = GetMeUserInfo()->bDeskStation ; 

	GameStation_Base *pGameStation = (GameStation_Base *)pStationData;
	if (NULL == pGameStation)
	{
		return false;
	}
	SetStationParameter(pGameStation->byGameStation);

	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:			//�ȴ���һ�̿�ʼ
		{
			if (uDataSize != sizeof(GameStation_WaiteAgree))
			{
				return false;
			}
			SetStationParameter(GS_WAIT_ARGEE);
			HandleStartStation(pGameStation);
			SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 4), ID_BEGIN_TIME);
			break; 
		}
	case GS_ROB_NT:		//��ׯ״̬
		{
			if (uDataSize != sizeof(GameStation_RobNt))
			{
				return false;
			}
			SetStationParameter(GS_ROB_NT);	
			HandleRobNtStation(pGameStation);
			SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 4), ID_CALL_SCORE_TIME);
			break;
		}
	case GS_NOTE:		//��ע״̬
		{
			if (uDataSize != sizeof(GameStation_Note))
			{
				return false;
			}
			SetStationParameter(GS_NOTE);	
			HandleNoteStation(pGameStation);
			SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 4), ID_NOTE_TIME);
			break;;
		}
	case GS_SEND_CARD:	//����״̬
		{
			if (uDataSize != sizeof(GameStation_SendCard))
			{
				return false;
			}
			SetStationParameter(GS_SEND_CARD);			
			//����״̬
			HandleSendCardStation(pGameStation);
			//SetBtEnable(CTN_2000280_CTN_100200_BTN_100231,false);
			break;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			if (uDataSize != sizeof(GameStation_OpenCard))
			{
				return false;
			}
			SetStationParameter(GS_PLAY_GAME);		
			//��Ϸ������
			HandleTakeCardStation(pGameStation);
			SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 4), ID_RESULT_TIME);
			break;
		}
	}
	
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_UG_USER_SET:		//�û������ƾ�
			{
				return TRUE;
			}
		case ASS_CALL_SCORE:			//��ׯ
			{				
				SetStationParameter(GS_ROB_NT);
				CallScoreStruct *score=(CallScoreStruct *)pNetData;	
				m_bCurrentOperationStation = score->bDeskStation;

				if(GetMeUserInfo()->bDeskStation == score->bDeskStation )
				{
					KillAllTimer();
					if ((rand()%2)+1 == 1)
					{
						OnCallScore(0,0);
					}
					else
					{
						OnCallScore(1,0);
					}
				}
				return true;
			}
		case ASS_CALL_SCORE_RESULT:			//��ׯ���
			{
				OutputDebugString("----xqm ������ �зֽ��");
				//OnHandleBetResult(buffer);
				return true;
			}
		case ASS_CALL_NOTE:				//�����ע
			{
				OutputDebugString("---xqm ������ �յ���ע��Ϣ");

				SetStationParameter(GS_NOTE);
				UserNoteStruct *score=(UserNoteStruct *)pNetData;	
				//m_bCurrentOperationStation = score->bDeskStation;
				
				if(GetMeUserInfo()->bDeskStation == m_iMyDeskStation && GetMeUserInfo()->bDeskStation != score->iNt)
				{
					KillAllTimer();
					//��ǰ��ť����
					int iHitButton = rand()%4;
					OnHandleHitBet(iHitButton);							
				}
				return true;
			}		
		case ASS_CALL_NOTE_RESULT:		//Ѻע���
			{				
				return true;
			}
		case ASS_USER_TAKE_CARD:		
			{
				return true;
			}
		case ASS_GAME_PLAY:				//��Ϸ��ʼ
			{
				SetStationParameter(GS_PLAY_GAME);
				OnHandleHitTakeCard();
				return true;
			}
		case ASS_TAKE_CARD_RESULT://Ҫ�ƽ��
			{				
				return true;
			}
		case ASS_USER_STOP:     //ͣ��
			{
				SetStationParameter(GS_PLAY_GAME);
				OnHandleHitStopCard();
				return true;
			}
		case ASS_USER_STOP_RESULT:
			{
				return true;
			}
		
		case ASS_SALE_END:	//��Ϸ��ȫ����		
		case ASS_CUT_END:	//�û�ǿ���뿪		
		case ASS_CONTINUE_END:	//��Ϸ����
		case ASS_NO_CONTINUE_END:
			{
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(0);

				srand(GetTickCount()+GetMeUserInfo()->bDeskStation);

				if(m_byLeaveThinkTime <= 0)	//û�����û������뿪ʱ�� �Ͱ�ԭ���ķ�ʽ�趨��ʱ��
				{
					int  iRandNo = rand() % 4;
					switch(iRandNo )
					{
					case 0:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 2)+1, ID_BEGIN_TIME);
						break;
					case 1:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 2)+2, ID_BEGIN_TIME);
						break;
					case 2:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 5)+3, ID_BEGIN_TIME);
						break;
					case 3:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 5)+4, ID_BEGIN_TIME);
						break;
					default:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 5)+5, ID_BEGIN_TIME);
						break;
					}
				}
				else
				{
					if (m_bQuick)
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
					else
					{
						SetGameTimer(GetMeUserInfo()->bDeskStation,2, ID_RESULT_TIME);
					}

				}				

				/// End Add by xqm 2011-5-10 �����˿���ʱ�� ���һ��ʱ�俪ʼ��Ϸ������һ��ʼ��Ϸ��̯��

				//SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
				return true;
			}
// 		case ASS_HAVE_THING:	//�û������뿪
// 			{
// 				return TRUE;
// 			}
// 		case ASS_LEFT_RESULT:	//�����뿪��� 
// 			{
// 				return TRUE;
// 			}
// 		case ASS_MESSAGE:
// 			{
// 				return true;
// 			}
// 		case ASS_STOP_THING://�Լ�������ǰ����
// 			{
// 				return true;
// 			}
// 		case ASS_AGREE_STOP://������ǰ�������
// 			{
// 				return true;
// 			}
// 		case ASS_NOMONEY:
// 			{
// 				ResetGameStation(RS_ALL_VIEW);
// 				OnCancel();
// 				return true;
// 			}

		}
	}

	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	//m_PlayView.m_Result.ShowWindow(SW_HIDE);
	m_bTimeOutCount=0;

	m_iNowOutPeople=-1;

	m_iFirstOutPeople=-1;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

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
	if (((GetStationParameter()==GS_WAIT_SETGAME)
		||(GetStationParameter()==GS_WAIT_NEXT)
		||(GetStationParameter()==GS_WAIT_ARGEE))
		)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	KillGameTimer(0);
	ResetGameStation(0);
	return true;;
}



//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_RESULT_TIME);
	KillTimer(ID_OUT_CARD);
	KillTimer(ID_LOOK_BACK);
	KillTimer(ID_NOTE_TIME);
	KillTimer(ID_CALL_SCORE_TIME);
	return;
}
//�Ƿ�ʹ��΢��Ϊ��λ������ע��ʱ��
bool CClientGameDlg::SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUseMillSecond)
{
	int iTimeType = 1000 ;
	if(m_bUseMilliSecond &&bUseMillSecond)
	{
		iTimeType = m_iMilliSecond ;
	}
	if(m_pGameRoom != NULL)
		m_pGameRoom->SendMessage(IDM_SET_GAME_TIME,(WPARAM)uTimeID,(LPARAM)uTimeCount*iTimeType);
	//	OnGameTimer(bDeskStation,uTimeID,uTimeCount);
	return true;
}
//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			KillAllTimer();
			OnHitBegin(0,0);
			return true;
		}
	
	case ID_RESULT_TIME:  //��Ϸ����
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
	case ID_CALL_SCORE_TIME:    //��ׯ
		{
			KillAllTimer();
			if ((rand()%2)+1 == 1)
			{
				OnCallScore(0,0);
			}
			else
			{
				OnCallScore(1,0);
			}
			return true;
		}
	case ID_NOTE_TIME:      //��ע
		{
			
			if(uTimeCount <= 0)
			{
				KillAllTimer();

				srand(GetTickCount()+GetMeUserInfo()->bDeskStation);

				if(!m_bLeftOrGoOn && m_iGameTimes >= rand()%m_byGameTimes+2)
				{
					OnHandleHitBet(3);
				}
			}

				
		}
	case ID_TAKE_CARD_TIME:	//Ҫ��
		{
			if(uTimeCount <= 0)
			{
				KillAllTimer();

				srand(GetTickCount()+GetMeUserInfo()->bDeskStation);

				if(!m_bLeftOrGoOn && m_iGameTimes >= rand()%m_byGameTimes+2)
				{
					OnHandleHitStopCard();
				}
			}
		}

	}
	return true;
}

//�����˿�
LRESULT CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	
	return 0L;
}

//�����˿�
LRESULT CClientGameDlg::OnMouseMove(WPARAM wparam, LPARAM lparam)
{
	
	return TRUE;;
}
//�����˿�
LRESULT CClientGameDlg::OnMouseLeft(WPARAM wparam, LPARAM lparam)
{
	return TRUE;	
}

//�û�������
// LRESULT CClientGameDlg::OnHitActiveResult(WPARAM wparam, LPARAM lparam)
// {
// 	int ret = (int) wparam;
// 	switch(wparam)
// 	{
// 
// 	case TYPE_TAKE_CARD:
// 		KillAllTimer();
// 		tagUserProcess process;
// 		process.iVrebType = TYPE_TAKE_CARD;				//����
// 		//::CopyMemory(process.bUpCard,m_iUpBullCard[ViewStation(GetMeUserInfo()->bDeskStation)],sizeof(m_iUpBullCard[ViewStation(GetMeUserInfo()->bDeskStation)]));
// 		//	SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
// 		break;
// 	}
// 	return 0L;
// }

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
	/*	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
	m_PlayView.m_btThing.EnableWindow(FALSE);
	HaveThingStruct HaveThing;
	HaveThing.pos=0;
	::strcpy(HaveThing.szMessage,(char *)wparam);
	SendGameData(&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}*/
	return 0;
}

//�û������뿪
LRESULT	CClientGameDlg::OnArgeeUserLeft(WPARAM wparam, LPARAM lparam)
{
	/*	LeaveResultStruct Leave;
	Leave.bDeskStation=GetMeUserInfo()->bDeskStation;
	Leave.bArgeeLeave=(BYTE)wparam;
	SendGameData(&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	*/	return 0;
}



//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	//��������
	m_iNowOutPeople=-1;
	m_iFirstOutPeople=-1;
	//::memset(m_iUpBullCard,1,sizeof(m_iUpBullCard));
	::memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));
	::memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
	//���������
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

	return;
}

void CClientGameDlg::OnGameSetting()
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	//CGameSet GameSetDlg(this);
	//GameSetDlg.m_bSound=m_pGameInfo->bEnableSound;
	//GameSetDlg.m_bShowUser=m_pGameInfo->bShowUserInfo;	
	//GameSetDlg.m_bEnableWatch=m_pGameInfo->bEnableWatch;
	//GameSetDlg.m_bRightPower=GetProfileInt(TEXT("RightPower"),FALSE);

	//if (GameSetDlg.DoModal()==IDOK)
	//{
	//	//�������
	//	bool bSendWatch=((m_bWatchMode==false)&&(m_pGameInfo->bEnableWatch!=GameSetDlg.m_bEnableWatch));

	//	//��������
	//	/*m_pGameInfo->bShowUserInfo=GameSetDlg.m_bShowUser;
	//	m_pGameInfo->bEnableWatch=GameSetDlg.m_bEnableWatch;
	//	m_pGameInfo->bEnableSound=GameSetDlg.m_bSound;
	//	WriteProfileInt(TEXT("RightPower"),GameSetDlg.m_bRightPower?1:0);*/
	//	//m_PlayView.m_bRightPower=GameSetDlg.m_bRightPower;

	//	//��������
	//	if (bSendWatch==true)
	//	{
	//		MSG_GM_WatchSet WatchSet;			
	//		memset(&WatchSet,0,sizeof(WatchSet));
	//		WatchSet.dwUserID=0;
	//		SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,GameSetDlg.m_bEnableWatch?TRUE:FALSE);
	//	}
	//}
	AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}

void CClientGameDlg::OnWatchSetChange(void)
{
	/*if (m_bWatchMode==true)
	{
		if (m_bWatchOther)	m_MessageHandle.InsertNormalMessage(TEXT("����������Թ�����Ϸ"));
		else m_MessageHandle.InsertNormalMessage(TEXT("��Ҳ��������Թ�����Ϸ"));
	}*/
}

//�û��뿪
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	/*	if ((bWatchUser==false)&&(pUserItem!=NULL)&&(bDeskStation==0))
	{
	if (GetStationParameter()==GS_WAIT_ARGEE)
	{
	for (int i=0;i<4;i++)
	{
	if (m_pUserInfo[i]!=NULL) 
	{
	m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
	m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
	}
	}
	if (m_PlayView.m_GameInfo.GetSafeHwnd()) m_PlayView.m_GameInfo.DestroyWindow();
	m_PlayView.m_GameNoteView.SetMessage(TEXT("���ڵȴ����������ƾ�"));
	m_PlayView.UpdateViewFace(NULL);
	}
	}*/
	//	if ((bWatchUser==false)&&(pUserItem!=NULL)&&(bDeskStation==0))		//����Ƕ����뿪
	//	{
	if (GetStationParameter()>GS_WAIT_ARGEE)
	{
		for (int i=0;i<PLAY_COUNT;i++)
		{
			if (m_pUserInfo[i]!=NULL) 
			{
				m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
//				m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
			}
		}
	//	m_PlayView.UpdateViewFace(NULL);
	}
//	m_PlayView.SetNTStation();
	//	}
	//��ǰ����뿪��������Ƶ����Ҳ��֮�뿪����
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&pUserItem!=NULL&&!bWatchUser==TRUE)
	{
		//	MessageBox("��ۿ������,�Ѿ��˳�.","ϵͳ��ʾ",MB_ICONQUESTION);
		OnCancel();
		//PostMessage(WM_CLOSE,0,0);
	}
	return true;
//	return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
}

void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	CLoveSendClass::OnTimer(nIDEvent);
}


void CClientGameDlg::OnCancel()
{
	if(GetStationParameter()>=GS_SEND_CARD&&GetStationParameter() < GS_WAIT_NEXT)		//��ѯ״̬
	{
			return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}

	KillAllTimer();

	AFCCloseFrame();

	SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);		//����������Ϣ

	__super::OnCancel();
}

//�޸��û�Ѻע���
BOOL CClientGameDlg::ModifyNote(BYTE iViewStation,int iCurNote,BOOL bExtVal)
{
	//׷�ӱ���ĳ��Ѻע���
//	m_PlayView.AddNotePoint(iViewStation,iCurNote);
	//��ע�ۼ�
//	m_PlayView.AddTotalNote(0,iCurNote);
	return true;
}


////��������
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{	
	TCHAR GameName[200];
	CString strBuffer;
	wsprintf(GameName,"��� %s ��Ϸ�汾����,��Ҫ�������������°汾��?",m_pGameInfo->szGameName);
	if(iVersion!=m_iVersion || iVersion2!=m_iVersion2)
	{
		TCHAR sz[200];
		wsprintf(sz,"�汾��ͻ:��ǰ�汾%d-%d,����˰汾%d-%d",m_iVersion,m_iVersion2,iVersion,iVersion2);
	}
	if(iVersion!=m_iVersion)
	{

//		if (IDYES==AFCMessage(GameName,m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
//			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	if(iVersion2!=m_iVersion2)
	{
///		if (IDYES==AFCMessage(GameName,m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
//			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	return TRUE;
}
//��ׯ
LRESULT	CClientGameDlg::OnCallScore(WPARAM wparam, LPARAM lparam)
{
	KillTimer(ID_CALL_SCORE_TIME);
	HideButton();
	CallScoreStruct score;
	memset(&score,0,sizeof(score));
	score.iValue = (int)wparam;								
	score.bDeskStation = GetMeUserInfo()->bDeskStation;			
	score.bCallScoreflag = false;
	SendGameData(&score,sizeof(score),MDM_GM_GAME_NOTIFY,ASS_USER_SCORE,0);

	return 0;
}
//�û�ѡ����ע��
LRESULT CClientGameDlg::OnHandleHitBet(int iHitButton)
{
	KillTimer(ID_NOTE_TIME);
	HideButton();
	
	tagUserProcess userBet;
	userBet.iNoteType= iHitButton;
	userBet.iVrebType = TYPE_NOTE;				//��ע����		
	SendGameData(&userBet,sizeof(userBet),MDM_GM_GAME_NOTIFY,ASS_USER_NOTE,0);	
	return 0;
}

//���ܣ�������ע���
LRESULT CClientGameDlg::OnHandleBetResult(void *buffer)
{
	NoteResult *betResult=(NoteResult *)buffer;	
	if(NULL==betResult)
	{
		return 0 ;
	}
	if(TYPE_TAKE_CARD==betResult->bAddStyle)//ͣ��������ܽ������ͷ���Ҳ����յ���ע��Ϣ
	{
		return 0;
	}
	//��¼������ע��
	m_iTotalGameNote[m_iMyDeskStation]=betResult->iCurNote;

	return 0;
}
//Ҫ����Ϣ
LRESULT CClientGameDlg::OnHandleHitTakeCard()
{
	KillTimer(ID_TAKE_CARD_TIME);
	HideButton();
	TakeCardStruct TUserTakeCard;
	TUserTakeCard.byYaoPaiCount=0;
	for(int i=0;i<5;i++)
	{
		//���Ƶ���С��7��Ҫ������С��4ʱ ����Ҫ��
		if(m_Logic.GetPoint(m_DeskReadyCard[TUserTakeCard.byDeskStation],TUserTakeCard.byYaoPaiCount)<=70 && TUserTakeCard.byYaoPaiCount<5)
		{
			//m_bGameStation = GS_PLAY_FINISH;
			m_iUserStation[TUserTakeCard.byDeskStation] = TYPE_TAKE_CARD;
			TUserTakeCard.byReadyYaoCard[TUserTakeCard.byDeskStation] = true;
			SendGameData(&TUserTakeCard,sizeof(TUserTakeCard),MDM_GM_GAME_NOTIFY,ASS_USER_TAKE_CARD,0);
			TUserTakeCard.byYaoPaiCount++;
			CString lhx;
			lhx.Format("lihexing::AIҪ��");
			OutputDebugString(lhx);
		}
		else if(m_Logic.GetPoint(m_DeskReadyCard[TUserTakeCard.byDeskStation],TUserTakeCard.byYaoPaiCount)>=70 && TUserTakeCard.byYaoPaiCount>=4)
		{
			m_iUserStation[TUserTakeCard.byDeskStation] = TYPE_STOP_CARD;
			TUserTakeCard.byReadyYaoCard[TUserTakeCard.byDeskStation] = false;
			SendGameData(&TUserTakeCard,sizeof(TUserTakeCard),MDM_GM_GAME_NOTIFY,ASS_USER_STOP,0);
			CString lhx;
			lhx.Format("lihexing::AIͣ��");
			OutputDebugString(lhx);
			break;
		}
	}
	return 0;	
}
//ͣ����Ϣ
LRESULT CClientGameDlg::OnHandleHitStopCard()
{
	KillTimer(ID_TAKE_CARD_TIME);
	HideButton();
	StopCardStruct TUserStopCard;
	TUserStopCard.bReadyStopCard[m_iMyDeskStation]=true;
	SendGameData(&TUserStopCard,sizeof(TUserStopCard),MDM_GM_GAME_NOTIFY,ASS_USER_STOP,0);
	return 0;
}



//���ƶ�����ʾ��Ϻ�,��ʾ������
LRESULT CClientGameDlg::SendCardShowCard(WPARAM wparam,LPARAM lparam)
{
	//BYTE iViewStation=(BYTE)wparam;
	//m_PlayView.m_UserCard[iViewStation].SetCard(m_DeskCard[iViewStation],NULL,m_iDeskCardCount[iViewStation]);
	////	m_PlayView.m_UserCard[m_PlayView.m_flag].SetCard(m_DeskCard[m_PlayView.m_flag],NULL,m_iDeskCardCount[m_PlayView.m_flag]);

	////ϵͳ����Ա��ʾ���ҵ���
	//if(m_pGameInfo->pMeUserInfo->GameUserInfo.bGameMaster==MAST_PEOPLE)
	//	m_PlayView.m_UserCard[iViewStation].SetCard(m_UserCard[iViewStation],NULL,m_iDeskCardCount[iViewStation]);
	return 0;
}



//���Ż��㵽��ͼλ��
//BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
//{
//	BYTE bViewStation=m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
//	int temp=((PLAY_COUNT/2)-bViewStation+bDeskStation);
//	if(temp>=0)
//		return temp%PLAY_COUNT;
//	else
//		return PLAY_COUNT-1;
//switch(bViewStation)
//	{
//	case 0: { 
//		if(bDeskStation==0)	
//			return 3;
//		if(bDeskStation==1)
//			return 4;
//		if(bDeskStation==2)
//			return 5;
//		if(bDeskStation==3)
//			return 0;
//		if(bDeskStation==4)
//			return 1;
//		if(bDeskStation==5)
//			return 2;
//		break;
//			}
//	case 1: {
//		if(bDeskStation==0)	
//			return 2;
//		if(bDeskStation==1)
//			return 3;
//		if(bDeskStation==2)
//			return 4;
//		if(bDeskStation==3)
//			return 5;
//		if(bDeskStation==4)
//			return 0;
//        if(bDeskStation==5)
//			return 1;
//		break;
//			}
//	case 2: { 
//		if(bDeskStation==0)	
//			return 1;
//		if(bDeskStation==1)
//			return 2;
//		if(bDeskStation==2)
//			return 3;
//		if(bDeskStation==3)
//			return 4;
//		if(bDeskStation==4)
//			return 5;
//		if(bDeskStation==5)
//			return 0;
//		break;
//			}
//	case 3: { 
//		if(bDeskStation==0)	
//			return 0;
//		if(bDeskStation==1)
//			return 1;
//		if(bDeskStation==2)
//			return 2;
//		if(bDeskStation==3)
//			return 3;
//		if(bDeskStation==4)
//			return 4;
//		if(bDeskStation==5)
//			return 5;
//		break;
//			}	
//	case 4: { 
//		if(bDeskStation==0)	
//			return 5;
//		if(bDeskStation==1)
//			return 0;
//		if(bDeskStation==2)
//			return 1;
//		if(bDeskStation==3)
//			return 2;
//		if(bDeskStation==4)
//			return 3;
//		if(bDeskStation==5)
//			return 4;
//		break;
//			}
//	case 5:
//		{
//		if(bDeskStation==0)	
//			return 4;
//		if(bDeskStation==1)
//			return 5;
//		if(bDeskStation==2)
//			return 0;
//		if(bDeskStation==3)
//			return 1;
//		if(bDeskStation==4)
//			return 2;
//		if(bDeskStation==5)
//			return 3;
//		break;
//		}
//	default:
//		break;
//    }
//	return 0;
//}
//�Q���ƽY��
BOOL CClientGameDlg::ChangeBackCardResult(BYTE bDeskStation,BYTE bSourceBackCard,BYTE bResultCard,BOOL bExtVal)
{
	if(!bExtVal)
		return true;
	m_DeskReadyCard[bDeskStation][0]=bResultCard;
	/*m_PlayView.m_UserCard[bDeskStation].SetCard(m_UserCard[bDeskStation],NULL,m_iDeskCardCount[bDeskStation]);
	ReplaceCard(m_SysCard,0,m_iSysCardCount,bSourceBackCard,bResultCard);

	m_PlayView.m_SysBackCard.SetCard(m_SysCard,NULL,m_iSysCardCount);*/
	return true;
}
//ϵ�y�������Ƿ���ĳ����
BOOL CClientGameDlg::ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard)
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

// ������������ע
// BOOL CClientGameDlg::UserSupersedeNote(BYTE bDeskStation)
// {
// 	KillAllTimer();
// 	if (GetStationParameter() == GS_PLAY_GAME || GetStationParameter() == GS_SEND_CARD)
// 	{
// 		if (bDeskStation==GetMeUserInfo()->bDeskStation)
// 			return 0L;
// 		SupersedeUserNote supNote;
// 		supNote.bDeskStation = bDeskStation;	//������עλ��
// 		supNote.iVrebType = 0;					//��ע����
// 		supNote.iNote = 0;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
// 		if (GetStationParameter() == GS_PLAY_GAME)
// 			supNote.iVrebType = TYPE_TAKE_CARD;
// 		SendGameData(&supNote,sizeof(supNote),MDM_GM_GAME_NOTIFY,ASS_SUPERSEDE_VREB_CHECK,0);
// 	}
// 	return TRUE;
// }
int CClientGameDlg::GetFrontPlay(int NowPaly)
{
	int frontplay;//��һλ��ע��
	for(int i=1;i<PLAY_COUNT;i++)
	{
		frontplay=(GetMeUserInfo()->bDeskStation-i<0)?PLAY_COUNT+(GetMeUserInfo()->bDeskStation-i)
			:GetMeUserInfo()->bDeskStation-i;

		if(!m_pUserInfo[frontplay])		
			continue;

		if(m_DeskReadyCard[frontplay]!=0)//��ǰ�������δPASS

			return frontplay;
	}
	return 0;
}
void CClientGameDlg::DelShowThis()
{
	/*for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_PlayView.m_UserCard[i].SetCardShowNum(0);
	}*/
}
//Ϊ��Ƶģ�����ش˺���
#ifdef VIDEO
bool CClientGameDlg::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return FALSE;
	}

	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && bWatchUser )
	{
		m_PlayView.m_bWatch=bWatchUser;
	}
	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && (!bWatchUser) )
	{    
		CPlayVideo *pPlayVideo=CPlayVideo::CreatePlayVideo();


		if(NULL!=pPlayVideo)
		{ 

			CString NickName=TEXT("");
			int    byteDeskNO=0;
			UserInfoStruct &UInfo=	pUserItem->GameUserInfo;
			NickName+=UInfo.nickName;
			byteDeskNO=UInfo.bDeskNO;
			UINT	uiRoomID = m_pGameInfo->uRoomID;
			UINT	uiBRRoomID =   uiRoomID * 1000 + byteDeskNO; 

			ST_INITVIDEO stInitVideo;
			stInitVideo.iRoomPassType = 0;
			stInitVideo.nRoomID		  = uiBRRoomID;
			stInitVideo.pParentWnd    = &m_PlayView;
			stInitVideo.strRoomPass   = _T("");
			stInitVideo.strUserName	  = NickName;
			stInitVideo.strUserPass   = _T("");
			if( pPlayVideo->Video_Initialize(stInitVideo) )
			{
				//			
				//sprintf(sz,"��Ƶ��ʼ���ɹ�!");
				//Writelog(sz,1);
				////������Ƶ������

				pPlayVideo->Connect();

			}
			else
			{
				pPlayVideo->Video_Release();
				/*	sprintf(sz,"��Ƶ��ʼ��ʧ��!");
				Writelog(sz,1);*/
				return 0;
			}


		} 
	}
	return TRUE;
}
bool CClientGameDlg::AFCCloseFrame()
{
	///�ͷ���Ƶ��Դ 
	if( NULL != CPlayVideo::GetPlayVideo())
		CPlayVideo::GetPlayVideo()->Video_Release(); 
	return __super::AFCCloseFrame();
}
#endif
//���ؽ�ׯ��ť
BOOL CClientGameDlg::HideButton()
{
	/*m_PlayView.m_btCallScore1.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_HIDE);*/
	return true;
}
//��ʾ��ׯ��ť
void CClientGameDlg::ShowCallScoreBt(int CallScore,BOOL bExtVol)
{
	/*m_PlayView.m_btCallScore1.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScorePass.EnableWindow(1);
	m_PlayView.m_btCallScore1.EnableWindow(TRUE);*/
	return;
}

//��һ����������������ִ�д˺���
void CClientGameDlg::HandleStartStation(void *buffer)
{
	GameStation_WaiteAgree *pGameStation = (GameStation_WaiteAgree *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}
	//��������
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��

}
/*----------------------------------------------------------------------------------*/
//��ׯ״̬
void	CClientGameDlg::HandleRobNtStation(void *buffer)
{
	GameStation_RobNt *pGameStation = (GameStation_RobNt *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}
	//��������
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��

	m_byLostTime	= pGameStation->byLostTime;		//��¼��ʱ��ʣ��ʱ��

	//������ҵ�״̬
	memcpy(m_iUserStation,pGameStation->iUserStation,sizeof(m_iUserStation));
}
/*----------------------------------------------------------------------------------*/
//��ע״̬
void	CClientGameDlg::HandleNoteStation(void *buffer)
{
	GameStation_Note *pGameStation = (GameStation_Note *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}

	//��������
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��
	m_byLostTime	= pGameStation->byLostTime;		//��¼��ʱ��ʣ��ʱ��

	//������ҵ�״̬
	memcpy(m_iUserStation,pGameStation->iUserStation,sizeof(m_iUserStation));
	//ׯ��λ��
	m_byNtStation	 = pGameStation->byNtStation;
	//��ע����
	m_iThisTurnLimit = pGameStation->iLimitNote[m_iMyDeskStation];
	memcpy(m_i64UserNoteLimite,pGameStation->i64UserNoteLimite,sizeof(m_i64UserNoteLimite));
	
	//��¼��ʾ����ÿ���˵���ע��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		m_iTotalGameNote[i] = pGameStation->i64PerJuTotalNote[i];
	}

	//�Լ������ && ����ׯ�� && ������;�����
// 	if (pGameStation->iUserStation[m_iMyDeskStation] == TYPE_PLAY_GAME && m_iMyDeskStation!=m_byNtStation && m_iUserStation[m_iMyDeskStation] != -1)	
// 	{
// 		
// 	}
}
/*----------------------------------------------------------------------------------*/
//����״̬
void	CClientGameDlg::HandleSendCardStation(void *buffer)
{
	GameStation_SendCard *pGameStation = (GameStation_SendCard *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}//��������
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;	//���䱶��
	m_byNtStation	= pGameStation->byNtStation;		//ׯ��λ��
	m_byLostTime	= pGameStation->byLostTime;		//��¼��ʱ��ʣ��ʱ��

	//������ҵ�״̬
	memcpy(m_iUserStation,pGameStation->iUserStation,sizeof(m_iUserStation));

	//�˿�������
	memcpy(m_DeskReadyCard,pGameStation->iUserCard,sizeof(m_DeskReadyCard));
	memcpy(m_iDeskCardCount,pGameStation->iUserCardCount,sizeof(m_iDeskCardCount));
	//�û���ע���
	memcpy(m_iTotalGameNote,pGameStation->i64PerJuTotalNote,sizeof(m_iTotalGameNote));
		
}
/*----------------------------------------------------------------------------------*/
//��Ϸ������
void	CClientGameDlg::HandleTakeCardStation(void *buffer)
{
	GameStation_OpenCard *pGameStation = (GameStation_OpenCard *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;	//���䱶��
	m_byNtStation	= pGameStation->byNtStation;		//ׯ��λ��
	m_byLostTime	= pGameStation->byLostTime;		//��¼��ʱ��ʣ��ʱ��

	//������ҵ�״̬
	memcpy(m_iUserStation,pGameStation->iUserStation,sizeof(m_iUserStation));
	//�˿�������
	memcpy(m_DeskReadyCard,pGameStation->iUserCard,sizeof(m_DeskReadyCard));
	memcpy(m_iDeskCardCount,pGameStation->iUserCardCount,sizeof(m_iDeskCardCount));
	memcpy(m_byOpenCardCount,pGameStation->byOpenCardCount,sizeof(m_byOpenCardCount));		//�Ѿ���������

	//�û���ע���
	memcpy(m_iTotalGameNote,pGameStation->i64PerJuTotalNote,sizeof(m_iTotalGameNote));

}

