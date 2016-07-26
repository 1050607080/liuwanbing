#include "StdAfx.h"
#include "GameRoomEx.h"
#include "auto_aidlg.h"
#include "gameroommessage.h"


static bool g_bUserAI = true;

///rqffffffffffffffffffffff
static double randf()
{
	return rand()*1.0/RAND_MAX;
}

int CGameRoomEx::m_nOnLinePeople = 0;

BEGIN_MESSAGE_MAP(CGameRoomEx, CBaseRoom)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_SET_GAME_TIME,SetGameTime)
	ON_MESSAGE(IDM_KILL_GAME_TIME,KillGameTime)
	ON_MESSAGE(IDM_CLOSE,OnCloseGame)//20090211
END_MESSAGE_MAP()

//���캯��
CGameRoomEx::CGameRoomEx(TCHAR * szGameName, RoomInfoStruct * pRoomInfo, MSG_GP_R_LogonResult * pLogonInfo,BYTE bEnterStyle
						 , bool bMachineAndPlay, int iMachineUserId, BYTE bMachineDeskCount, bool bAutoDesk) 
: CBaseRoom(IDD_GAME_ROOM)
{
	m_IGameFrame = NULL; 	//��Ϸ�ӿ�

	m_bEnterStyle = bEnterStyle;
   m_iContestState = 0;
	//20081126�¼ӻ�����������
	m_bMachineAndPlay = bMachineAndPlay;//������������Ƿ�ͬ��
	m_iMachineCheckTime  = iMachineUserId;//������UserID
	m_bMachineDeskCount = bMachineDeskCount;//ÿ�������˸���
	m_bAutoDesk       = bAutoDesk;
	m_bNowDesk        = 255;                         //��ǰҪ��������
	m_bErrorDeskCount = 0;                  //���������Ÿ���������20��ʱ������
	memset(&m_bErrorDesk,255,sizeof(m_bErrorDesk)); //����������

	memset(&m_bErrorDeskFlag,0,sizeof(m_bErrorDeskFlag)); //����������
	
	memset(&m_pLogonInfo,0,sizeof(m_pLogonInfo));
	::memset(&m_ComRoomInfo,0,sizeof(m_ComRoomInfo));

	m_pRoomInfo = pRoomInfo;
	////��ʼ����������
	memset(&m_GameInfo,0,sizeof(m_GameInfo));
	lstrcpy(m_GameInfo.szGameName,szGameName);
	m_GameInfo.pISocketSend=this;
	m_GameInfo.pIFindUser=&m_UserManage;
	m_GameInfo.bShowUserInfo=true;
	m_GameInfo.bEnableSound=true;
	m_GameInfo.bEnableWatch=false;
	m_GameInfo.uNameID=pRoomInfo->stComRoomInfo.uNameID;
	m_GameInfo.uRoomID=pRoomInfo->stComRoomInfo.uRoomID;
	//m_GameInfo.dwGameLSVer=pRoomInfo->dwGameLSVer;
	m_GameInfo.uComType=pRoomInfo->stComRoomInfo.uComType;
	m_GameInfo.uDeskPeople=pRoomInfo->stComRoomInfo.uDeskPeople;
	m_GameInfo.dwRoomRule = pRoomInfo->stComRoomInfo.dwRoomRule ; 


	m_i64MaxAiMoney = 10000000;                    //AI��������Ǯ�����ﵽ������ʹ�Ǯ
	m_i64CheckMoneyMax = 100000;                 //ȡǮ�����
	m_i64CheckMoneyMix = 10000;                 //ȡǮ��С��
	m_i64SaveAndOutMoneyBase = 100;				//��ȡǮ������������ȡ������
	m_bAiBank = FALSE;
	m_bOpenBankBase = FALSE;					//�Ƿ���������������ȡ
	m_UseBank = FALSE;
	m_CheckCount = 0;

	CopyMemory(&m_ComRoomInfo,&(pRoomInfo->stComRoomInfo),sizeof(ComRoomInfo));
	CopyMemory(&m_pLogonInfo,(MSG_GP_R_LogonResult*)pLogonInfo,sizeof(m_pLogonInfo));

	

	return;
}

//��������
CGameRoomEx::~CGameRoomEx()
{
	if (m_IGameFrame != NULL)
	{
		delete m_IGameFrame;
		m_IGameFrame = NULL;
	}
	//OutputDebugString("~CGameRoomEx()");
}

//���ݰ󶨺���
void CGameRoomEx::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

}

//��ʼ������
BOOL CGameRoomEx::OnInitDialog()
{
	__super::OnInitDialog();

	ConnectToGameServer();

	CString ad=CINIFile::GetAppPath ();
	CINIFile f( ad + "bzgame.ini");
	SetTimer(IDT_GAMEEXIT, TIME_CHECK_GAMEEXIT, NULL);
	CString m_key = "BZW";
	m_CheckCountMax = f.GetKeyVal(m_key,"CheckCountMax",10);//
	//CString ad=CINIFile::GetAppPath ();
	//CINIFile f( ad + "bzgame.ini");
	//CString key = "BZW";
	//m_i64MaxAiMoney = f.GetKeyVal(key,"MaxAiMoney",m_i64MaxAiMoney);                    //AI��������Ǯ�����ﵽ������ʹ�Ǯ
	//m_i64CheckMoneyMax = f.GetKeyVal(key,"CheckMoneyMax",m_i64CheckMoneyMax);                 //ȡǮ�����
	//m_i64CheckMoneyMix = f.GetKeyVal(key,"CheckMoneyMix",m_i64CheckMoneyMix);                   //ȡǮ��С��
	//m_bAiBank = f.GetKeyVal(key,"AiBank",m_bAiBank);
	/*CString str;
	str.Format("zhtlog:m_i64MaxAiMoney=%I64d",m_i64MaxAiMoney);
	OutputDebugString(str);*/

	return TRUE;
}
//��½��Ϸ����
void  CGameRoomEx::ConnectToGameServer()
{
	int static iConnect = 0;

	if(m_TCPSocket->Connect(m_ComRoomInfo.szServiceIP,m_ComRoomInfo.uServicePort)==false && iConnect ++ < 5)
	{
		
		this->ConnectToGameServer();
	}
	

	return ;
}
//����������Ϣ
bool CGameRoomEx::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	//WriteStr("OnSocketConnectEvent");
	if (uErrorCode!=0)	//���ӳ��ִ���
	{
		//WriteStr("OnSocketConnectEvent Error");
		return true;
	}
	return true;
}

//�����ȡ��Ϣ
bool CGameRoomEx::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	/*wsprintf(m_ErrorMsg,"OnSocketReadEvent bMainID = %d,bAssistantID = %d",pNetHead->bMainID,pNetHead->bAssistantID );
	WriteStr(m_ErrorMsg,1);*/
	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:			//������Ϣ,׼�����뷿�� 
		{
			return OnConnectMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_LOGON:			//�����½
		{
			return OnLogonMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_USER_LIST:		//�û��б�
		{
			return OnUserListMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_USER_ACTION:	//�û�����
		{
			return OnUserActionMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_ROOM:			//������Ϣ
		{
			return OnRoomMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_MATCH_INFO:		//������Ϣ
		{
			return true;//return OnMatchMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_MANAGE:			//������Ϣ
		{
			return true;//return OnManageMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_MESSAGE:		//ϵͳ��Ϣ
		{
			return true;//return OnSystemMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	//case MDM_GR_POINT:			//�����
	//	{
	//		break;
	//	}
	case MDM_GR_MONEY:			//Ǯ����,��Ǯ
		{
			//return true;//
			return OnAboutMoney(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	//case MDM_GR_ITEM_DATA_MSG://����
	//	{
	//		return true;
			//return OnItemDataMsg(pNetHead,pNetData,uDataSize,pClientSocket);
	//	}	
		//case MDM_GR_BROADCAST_MESSAGE://�㲥��Ϣ
		//	{
		//		return OnBroadCastManage(pNetHead,pNetData,uDataSize,pClientSocket);
		//	}
#ifdef MY_MDM
	case MDM_GR_FRIEND_MANAGE://������Ϣ
		{
			return true;//OnFriendManage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_QUEUE_MSG://�Ŷӵȴ���Ϣ
		{
			return true;//OnQueueMsg(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_VIP_MSG:	//vip��Ϣ
		{
			return true;//OnVipOperationMsg(pNetHead,pNetData,uDataSize,pClientSocket);
		}
#endif
	}

	////��Ϸ��Ϣ
	if (m_IGameFrame!=NULL)
		return m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,pClientSocket);

	return true;
}
#ifdef MY_MDM
//������ҵ���
bool CGameRoomEx::OnItemDataMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_S_GET_ITEM_DATA:	//��ȡ
		return true;
		break;
	case ASS_S_USE_ITEM:		//ʹ�õ���
		return OnUserUseItemResult(pNetHead,pNetData,uDataSize,pClientSocket);
		break;
	case ASS_S_GET_BUY_ITEM_DATA://����
		return true;
		break;
	case ASS_S_BUY_ITEM_DATA:	//����
		return true;
		break;
	}
	return true;
}

//ʹ�õ���
bool CGameRoomEx::OnUserUseItemResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(sizeof(tagUserUseItem)!= uDataSize)
		return false;
	tagUserUseItem * pUseItem = (tagUserUseItem *)pNetData;
	UserItemStruct *pUserInfo = m_UserManage.FindOnLineUser(pUseItem->dwDestUserID);
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
	int curtime=atoi(stime);                        //ת��Ϊint
	if(pUserInfo == NULL)
		return true;
	switch(pUseItem->iItemID)
	{
	case ITEM_FLOWER://�ʻ�����+1
		{
			pUserInfo->GameUserInfo.dwFascination += 1;
			break;
		}
	case ITEM_BLOCK://שͷ����-1
		{
			pUserInfo->GameUserInfo.dwFascination -= 1;
			break;
		}
	case ITEM_ERASER://��������
		{
			if(pUserInfo->GameUserInfo.dwPoint < 0)
				pUserInfo->GameUserInfo.dwPoint = 0;
			break;
		}
	case ITEM_SMALL_BROADCAST:
	case ITEM_BIG_BROADCAST:
		{
			break;
		}
	case DOUBLE_SCORE_CARD_1_HOUR:
		{
			if(pUseItem->dwDestUserID ==  m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
			{
				if(pUserInfo->GameUserInfo.iDoublePointTime>curtime)
					pUserInfo->GameUserInfo.iDoublePointTime+=60*60;
				else
					pUserInfo->GameUserInfo.iDoublePointTime=curtime+60*60;
				time_t t=(time_t)pUserInfo->GameUserInfo.iDoublePointTime;
				CTime expiresTime(t);

			}
			break;
		}
	case DOUBLE_SCORE_CARD_3_HOUR:
		if(pUseItem->dwDestUserID ==  m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			if(pUserInfo->GameUserInfo.iDoublePointTime>curtime)
				pUserInfo->GameUserInfo.iDoublePointTime+=3*60*60;
			else
				pUserInfo->GameUserInfo.iDoublePointTime=curtime+3*60*60;
			time_t t=(time_t)pUserInfo->GameUserInfo.iDoublePointTime;
			CTime expiresTime(t);

		}
		break;
	case DOUBLE_SCORE_CARD_1_DAY:
		if(pUseItem->dwDestUserID ==  m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			if(pUserInfo->GameUserInfo.iDoublePointTime>curtime)
				pUserInfo->GameUserInfo.iDoublePointTime+=24*60*60;
			else
				pUserInfo->GameUserInfo.iDoublePointTime=curtime+24*60*60;
			time_t t=(time_t)pUserInfo->GameUserInfo.iDoublePointTime;
			CTime expiresTime(t);
		}
		break;
	case DOUBLE_SCORE_CARD_7_DAY:
		{
			if(pUseItem->dwDestUserID ==  m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
			{
				if(pUserInfo->GameUserInfo.iDoublePointTime>curtime)
					pUserInfo->GameUserInfo.iDoublePointTime+=7*24*60*60;
				else
					pUserInfo->GameUserInfo.iDoublePointTime=curtime+7*24*60*60;
				time_t t=(time_t)pUserInfo->GameUserInfo.iDoublePointTime;
				CTime expiresTime(t);
			}
			break;
		}
	}
	//��������б�
//	m_pUserList->UpdateGameUser(pUserInfo);
	//���Լ�ͬ��
	if(pUserInfo->GameUserInfo.bDeskNO == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO
		&& m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
	{
		if(NULL != m_IGameFrame )
			m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,pClientSocket);
	}
	return true;
}
#endif
//����ر���Ϣ
bool CGameRoomEx::OnSocketCloseEvent()
{
	//OutputDebugString("zhtlog:����ر���Ϣ");
	try
	{
		if(m_IGameFrame)
		{
			m_IGameFrame->AFCCloseFrame();
			//����
		}
	}catch(...)
	{
		if (m_IGameFrame != NULL)
		{
			delete m_IGameFrame;
		}
		m_IGameFrame = NULL;
	}
	::PostMessage(GetParent()->m_hWnd,IDM_CLOSE_GAME_ROOM,(WPARAM)m_pRoomInfo, m_pLogonInfo.dwUserID);
	return true;
}
//�رշ���
void CGameRoomEx:: CloseGameRoom()
{
	m_TCPSocket->CloseSocket(false);
	try
	{
		if(m_IGameFrame)
		{
			m_IGameFrame->AFCCloseFrame();
			//����
		}
	}catch(...)
	{
		if (m_IGameFrame != NULL)
		{
			delete m_IGameFrame;
		}
		m_IGameFrame = NULL;
	}
	::PostMessage(GetParent()->m_hWnd,IDM_CLOSE_GAME_ROOM,(WPARAM)m_pRoomInfo, m_pLogonInfo.dwUserID);
	return ;
}
//������Ϣ����
bool CGameRoomEx::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID==ASS_NET_TEST)	//���������Ϣ
	{
		pClientSocket->SendData(MDM_CONNECT,ASS_NET_TEST,0);
		return true;
	}
	else if ((pNetHead->bAssistantID==ASS_CONNECT_SUCCESS)||(pNetHead->bAssistantID==3))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * _p = (MSG_S_ConnectSuccess *)pNetData;
		if (_p != NULL)
		{
			pClientSocket->SetCheckCode(_p->i64CheckCode, SECRET_KEY);
		}

		//���ɹ�
		SendUserInfo();
		return true;
	}
	return true;
}

//001,�����½����,һ���뷿��  
bool CGameRoomEx::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_LOGON_SUCCESS:		//��½�ɹ�
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_LogonResult)) return false;

			MSG_GR_R_LogonResult * pLogonInfo=(MSG_GR_R_LogonResult *)pNetData;

			//CopyMemory(&m_pLogonInfo,(MSG_GR_R_LogonResult * )pLogonInfo,sizeof(MSG_GR_R_LogonResult));
			m_GameInfo.uLessPoint=pLogonInfo->uLessPoint;
			m_GameInfo.pMeUserInfo = m_UserManage.AddGameUser(&pLogonInfo->pUserInfoStruct,1,0);
						
			if(m_GameInfo.pMeUserInfo == NULL)
			{
				WriteStr("ASS_GR_LOGON_SUCCESS����뿪��");
				
				CloseGameRoom();
				return false;
			}
			
			m_iMyMoney = (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money+m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank);
						
			return true;
		}
	case ASS_GR_LOGON_ERROR:		//��½ʧ��
		{
			//wsprintf(m_ErrorMsg,"ASS_GR_LOGON_ERROR//pNetHead->bHandleCode = %d",pNetHead->bHandleCode);
			WriteStr("Login Failed");
			OutputDebugString("ASS_GR_LOGON_ERROR");
			//m_bNowRoomIndex--;
			::PostMessage(GetParent()->m_hWnd,IDM_LOGON_FAIL,0,0);//20081128
			return false;
		}
	case ASS_GR_SEND_FINISH:		//��½���
		{
			if(m_bEnterStyle == 1)
				return true;
			//������״̬����
			SetCheckStationTime();
			return true;
		}
	}
	return true;
}


//����״̬����ʱ��
bool CGameRoomEx::SetCheckStationTime()
{
	//SetTimer(ID_CHECK_STATE,TIME_CHECK_STATE,NULL);
	SetTimer(ID_CHECK_STATE,m_iMachineCheckTime,NULL);
	SetTimer(ID_CLEAR_ERRORDESK,1800*1000,NULL);
	AiControlBank();
	return true;
}

//����
bool CGameRoomEx::StandingDesk()
{
	//���淢�����µ���Ϣ
	if (m_IGameFrame!=NULL)
	{
		CClientGameDlg *dlg = NULL;
		dlg = (CClientGameDlg*)m_IGameFrame;
	}
	m_TCPSocket->SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
	return true;
}

//����
bool CGameRoomEx::OnSitDesk()
{
	if(m_GameInfo.dwRoomRule &GRR_QUEUE_GAME)
	{
		m_TCPSocket->SendData(MDM_GR_USER_ACTION,ASS_GR_JOIN_QUEUE,0);
		return  true ; 
	}
	
	BYTE bDeskIndex = 255;
	BYTE bDeskStation = 255;
	//���ҿ�����
	//OnFindNullDesk(bDeskIndex,bDeskStation);
	//�������һ��������
#ifdef MY_GGW
	OnFindRandDesk(bDeskIndex,bDeskStation);
#else
	//���������Ҫ������˲�����������Ϸ
	if (!m_bMachineAndPlay)
	{
		OnFindMachineDesk(bDeskIndex,bDeskStation);
	}
	else
	{
		//����������������(���ܻ�����ͬ��������,���������2��������)

		//rqffffffffffffffffffffffffffffffff
		//OnFindTureDesk(bDeskIndex,bDeskStation);
		OnFindNullDesk(bDeskIndex,bDeskStation);
	}
#endif

	if(bDeskIndex == 255||bDeskStation == 255)
	{
		return true;
	}
	MSG_GR_S_UserSit UserSit;
	UserSit.bDeskIndex = bDeskIndex;
	UserSit.bDeskStation = bDeskStation;
	m_bNowDesk        = bDeskIndex;
	//���淢�����µ���Ϣ
	m_TCPSocket->SendData(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_USER_SIT,0);
	return true;
}
//�����һ��������
bool CGameRoomEx::OnFindRandDesk(BYTE &bDeskIndex,BYTE &bDeskStation)
{
	srand(GetTickCount());
	int time = 0;
	UINT bDesk = rand() % m_ComRoomInfo.uDeskCount;

	while(time++ < 5)
	{
		//��λ
		for(UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
		{
			if (IsErrorDesk(station))//20090227�ж��Ƿ�Ϊ����������
			{
				continue;
			}
			bool havepeople = false;
			for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
			{
				UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
				if(pFindUserItem == NULL)
					continue;
				if(pFindUserItem->GameUserInfo.bDeskNO == bDesk && pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//������Ϸ�Ѿ���ʼ
				{
					havepeople = true;
					break;
				}
				if(pFindUserItem->GameUserInfo.bDeskNO == bDesk &&pFindUserItem->GameUserInfo.bDeskStation == station)//����λ�Ѿ�����
				{
					havepeople = true;
					break;
				}
			}
			if(havepeople != true)
			{
				bDeskIndex = bDesk;
				bDeskStation = station;
				return true;
			}
		}
	}
	return true;
}

//���ҿ�����
bool CGameRoomEx::OnFindNullDesk(BYTE &bDeskIndex,BYTE &bDeskStation)
{
	//��ÿһ�����ӿ�ʼ����
	for(UINT desk = 0; desk < m_ComRoomInfo.uDeskCount; desk ++)
	{
		if (IsErrorDesk(desk))//20090227�ж��Ƿ�Ϊ����������
		{
			continue;
		}
		//��λ
		for(UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
		{
			bool havepeople = false;
			int iPCount = m_UserManage.GetOnLineUserCount();
			for(int i = 0 ; i < iPCount; i ++)
			{
				UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
				if(pFindUserItem == NULL)
					continue;
				if(pFindUserItem->GameUserInfo.bDeskNO == desk&&pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//������Ϸ�Ѿ���ʼ
				{
					//���ǲ������������Ϸ�����н�����sdp2014-05-07
					CString ctext;
					ctext.Format("%d",NAME_ID);
					if (ctext.Left(3) == "330" && ctext.GetAt(4) == '0')
					{
						havepeople = false;
					}
					else
					{
						havepeople = true;
						break;
					}
					//////////////////////////////////
				}
				if(pFindUserItem->GameUserInfo.bDeskNO == desk &&pFindUserItem->GameUserInfo.bDeskStation == station)//��������
				{
					havepeople = true;
					break;
				}
			}
			if(havepeople != true)
			{

				if(CountMachine(desk) >= m_bMachineDeskCount)//�����˲��ܹ������趨������
				{
					continue;
				}
	
				bDeskIndex = desk;
				bDeskStation = station;
				return true;
			}
		}
	}
	return true;
}

//����ָ�����ӿ�λ��(���������ʱ����)
BYTE CGameRoomEx::GetNullStation(BYTE bDeskIndex)
{
	BYTE bDeskStation = 255;
	//��λ
	for(UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
	{
		if (IsErrorDesk(station))//20090227�ж��Ƿ�Ϊ����������
		{
			continue;
		}
		bool havepeople = false;
		for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
		{
			UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
			if(pFindUserItem == NULL)
				continue;
			if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex && pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//������Ϸ�Ѿ���ʼ
			{
				havepeople = true;
				break;
			}
			if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex &&pFindUserItem->GameUserInfo.bDeskStation == station)//��������
			{
				havepeople = true;
				break;
			}
		}
		if(havepeople != true)
		{
			bDeskStation = station;
			break;;
		}
	}
	return bDeskStation;
}
//����������������
bool CGameRoomEx::OnFindTureDesk(BYTE &bDeskIndex,BYTE &bDeskStation)
{
	bDeskIndex = 255;
	bDeskStation = 255;
	//int nPeopleCount = 0;
	//��ÿһ�����ӿ�ʼ����
	for(UINT desk = 0; desk < m_ComRoomInfo.uDeskCount; desk ++)
	{
		if (IsErrorDesk(desk))//20090227�ж��Ƿ�Ϊ����������
		{
			continue;
		}
		//��λ
		for(UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
		{
			bool havepeople = false;
			bool bVirtual = false;
			for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
			{
				UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
				if(pFindUserItem == NULL)
					continue;
				if (pFindUserItem->GameUserInfo.dwUserID < 0 && pFindUserItem->GameUserInfo.bDeskNO == desk)
				{
					bVirtual = true;
					break;
				}
				if(pFindUserItem->GameUserInfo.bDeskNO == desk&&pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//������Ϸ�Ѿ���ʼ
				{
					havepeople = true;
					break;
				}
				if(pFindUserItem->GameUserInfo.bDeskNO == desk &&pFindUserItem->GameUserInfo.bDeskStation == station)//��������
				{
					havepeople = true;
					break;
				}
			}
			if (bVirtual)  //�������λ��������һ��
			{
				continue;
			}
			if(havepeople != true)
			{

				if(CountMachine(desk) >= m_bMachineDeskCount)//�����˲��ܹ�����2
				{

					continue;

				}
				if (CountTruePlayer(desk) > 0)
				{
					//nPeopleCount = CountTruePlayer(desk);
					bDeskIndex = desk;
					bDeskStation = station;
					return true;
				}
				if (bDeskIndex == 255)
				{
					bDeskIndex = desk;
					bDeskStation = station;
				}
				
				//return true;
			}
		}
	}
	return true;
}

//���һ�������������
bool CGameRoomEx:: OnFindMachineDesk(BYTE &bDeskIndex,BYTE &bDeskStation)
{
	//��ÿһ�����ӿ�ʼ����
	for(UINT desk = 0; desk < m_ComRoomInfo.uDeskCount; desk ++)
	{
		if (IsErrorDesk(desk))//20090227�ж��Ƿ�Ϊ����������
		{
			continue;
		}
		//��λ
		for(UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
		{
			bool havepeople = false;
			bool bVirtual = false;
			for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
			{
				UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
				if(pFindUserItem == NULL)
					continue;
				if (pFindUserItem->GameUserInfo.dwUserID < 0 && pFindUserItem->GameUserInfo.bDeskNO == desk)
				{
					bVirtual = true;
					break;
				}
				if(pFindUserItem->GameUserInfo.bDeskNO == desk&&pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//������Ϸ�Ѿ���ʼ
				{
					havepeople = true;
					break;
				}
				if(pFindUserItem->GameUserInfo.bDeskNO == desk &&pFindUserItem->GameUserInfo.bDeskStation == station)//�������Ѿ�����
				{
					havepeople = true;
					break;
				}
			}
			if (bVirtual)  //�������λ��������һ��
			{
				continue;
			}
			if(havepeople != true)//�ǿ�
			{
				if(CountMachine(desk) == m_bMachineDeskCount)//�����˲��ܹ�����2
				{
					continue;
				}
				if(CountTruePlayer(desk) >= 1 
					&& m_GameInfo.pMeUserInfo->GameUserInfo.isVirtual/*IsMachineID(m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)*/)//�����˲��ܹ�������
					continue;
				bDeskIndex = desk;
				bDeskStation = station;
				return true;
			}
		}
	}
	return true;
}



//ͳ��ĳ������������
BYTE CGameRoomEx::CountMachine(BYTE bDeskIndex)
{
	int iCount = 0;
	for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
	{
		UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
		if(pFindUserItem == NULL)
			continue;
		if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex && pFindUserItem->GameUserInfo.isVirtual/*IsMachineID(pFindUserItem->GameUserInfo.dwUserID)*/)
		{
			iCount ++ ;
		}
	}
	return iCount;
}
//ͳ��ĳ����������
BYTE CGameRoomEx::CountTruePlayer(BYTE bDeskIndex)
{
	int iCount = 0;
	for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
	{
		UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
		if(pFindUserItem == NULL)
			continue;
		if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex && !pFindUserItem->GameUserInfo.isVirtual/*IsPlayerID(pFindUserItem->GameUserInfo.dwUserID)*/)
		{
			iCount ++ ;
		}
	}
	return iCount;
}

//�Ƿ���Ҫ������
bool CGameRoomEx::NeedChange(BYTE bDeskIndex)
{
	int count = 0;	
	for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
	{
		UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
		if(pFindUserItem == NULL)
			continue;
		if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)//������Ϸ�Ѿ���ʼ
		{
			count ++;
		}
	}
	if(count >= 2 || m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_PLAY_GAME)
		return false;

	return true;
}

//��������һ������
bool CGameRoomEx::FindOtherDesk()
{
	OnSitDesk();
	return true;
}

//�����û���Ϣ
bool CGameRoomEx::SendUserInfo()
{
	//�����û���¼��Ϣ
	MSG_GR_S_RoomLogon RoomLogon;
	RoomLogon.uGameVer = 0;//m_GameInfo.dwGameLSVer;
	RoomLogon.uRoomVer = GAME_PLACE_MAX_VER;
	RoomLogon.uNameID = m_ComRoomInfo.uNameID;
	RoomLogon.dwUserID = m_pLogonInfo.dwUserID;
	lstrcpy(RoomLogon.szMD5Pass,m_pLogonInfo.szMD5Pass);

	//���͵�½
	m_TCPSocket->SendData(&RoomLogon,sizeof(RoomLogon),MDM_GR_LOGON,ASS_GR_LOGON_BY_ID,0);

	return true;
}

//�û��б���
bool CGameRoomEx::OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_ONLINE_USER:
	case ASS_GR_NETCUT_USER:	//�û��б�
		{
			UINT uUserCount = uDataSize/sizeof(UserInfoStruct);

			if (uUserCount>0L)
			{
				//�������
				UserItemStruct * pUserItem = NULL;
				UserInfoStruct * pUserInfo = (UserInfoStruct *)pNetData;

				for (UINT i = 0; i < uUserCount; i ++)
				{
					pUserItem = m_UserManage.AddGameUser(pUserInfo+i,1,0);
				}
			}
			CGameRoomEx::m_nOnLinePeople = uUserCount;
			return true;
		}
	case ASS_GR_DESK_STATION:	//����״̬ 
		{
			if (uDataSize!=sizeof(MSG_GR_DeskStation)) return false;
			MSG_GR_DeskStation * pDeskStation=(MSG_GR_DeskStation *)pNetData;
			for (UINT i=0;i<pNetHead->bHandleCode;i++)
			{
				if ((pDeskStation->bVirtualDesk[i/8]&(1<<(i%8)))!=0)    //change by yjj 090325
				{
					UserInfoStruct pUserInfo;      //�����Ϣ
					int c=0;

					for (int k=0; k<m_GameInfo.uDeskPeople;k++)
					{
						
						pUserInfo.bUserState=USER_PLAY_GAME;
						pUserInfo.bDeskNO=i;
						pUserInfo.bDeskStation=k;
						pUserInfo.dwUserID = -1;//�����û�

						m_UserManage.AddGameUser(&pUserInfo,1,0);
					}
				}
			}
			return true;
		}
	}
	return true;
}


//������Ϣ����
bool CGameRoomEx::OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID==MDM_GR_ROOM);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_NORMAL_TALK:	//��ͨ����
		{
			
			return true;
		}
	case ASS_GR_SHORT_MSG:		//����Ϣ����
		{
			return true;
		}
	case ASS_GR_USER_AGREE:		//�û�ͬ��
		{
			return true;
		}
	case ASS_GR_CONTEST_APPLY:
		{
			if (uDataSize != sizeof(MSG_GR_ContestApply))	return false;

			MSG_GR_ContestApply *pMsgContestApply = (MSG_GR_ContestApply*)pNetData;
			CString str;
			str.Format("robot-----------switch(%d)", pMsgContestApply->iApplyResult);
			OutputDebugString(str);

			//���ݷ���˷����ı���״��������
			switch (pMsgContestApply->iApplyResult)
			{
			case 1://�����ɹ�
			case 3://�Ѿ�����
			{
				m_iContestState = 1;
				if (pMsgContestApply->iContestBegin == 1)
				{
					m_iContestState = 2;
					OutputDebugString("robot---------������ʼ������������Ϣ");
					//���û�������ﵽ��������Ҫ������������Ϣ
					m_TCPSocket->SendData(MDM_GR_USER_ACTION,ASS_GR_JOIN_QUEUE,0);
				}
				break;
			}
			case 7://���䱨�������ﵽ���ޣ����л�����
				{
					OutputDebugString("robot---------���䱨�������ﵽ���ޣ����л�����");
					CloseGameRoom();
					break;
				}
			case 8://�û����Ͻ�Ҳ��������ѣ���Ҫ��ȡǮ
				{
					OutputDebugString("robot-----------�û����Ͻ�Ҳ��������ѣ���Ҫ��ȡǮ");
					AiControlBank();
					CloseGameRoom();
					break;
				}
			default:
				{
					CloseGameRoom();
					break;
				}
			}
			return true;
		}	
	case ASS_GR_INIT_CONTEST:	
	case ASS_GR_CONTEST_WAIT_GAMEOVER:
		{
			return true;
		}
	case ASS_GR_CAHNGE_ROOM:
		{
			if (0 == m_iContestState)
			{
				CloseGameRoom();//�رշ���				
			}
			return true;
		}
	case ASS_GR_CONTEST_GAMEOVER:
	case ASS_GR_CONTEST_KICK:
		{
			CloseGameRoom();//�رշ���
			return true;
		}
	case ASS_GR_USER_CONTEST:
		{
			//��������
			if (uDataSize != sizeof(MSG_GR_ContestChange)) return false;
			MSG_GR_ContestChange* _p = (MSG_GR_ContestChange*)pNetData;

			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(_p->dwUserID);
			if (pUserItem != NULL)
			{
				//����̭��Ҵ�ʱ������������������ֵҪ�󣬱����û�����̭��ʱ����Ϊ7������������Ϊ6���˴��޸�һ����ʾ
				_p->iRemainPeople = _p->iRemainPeople>_p->iRankNum ? _p->iRemainPeople : _p->iRankNum;
				pUserItem->GameUserInfo.iContestCount = _p->iContestCount;
				pUserItem->GameUserInfo.i64ContestScore = _p->i64ContestScore;
				pUserItem->GameUserInfo.iRankNum = _p->iRankNum;
				pUserItem->GameUserInfo.iRemainPeople = _p->iRemainPeople;

				if( pUserItem->GameUserInfo.dwUserID  == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					CString str;
					str.Format("robot----��������------------id = %d rank = %d RemainPeople = %d DeskNo = %d DeskStation = %d",
						m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID,
						m_GameInfo.pMeUserInfo->GameUserInfo.iRankNum,
						m_GameInfo.pMeUserInfo->GameUserInfo.iRemainPeople,
						m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO,
						m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation);
					OutputDebugString(str);
				}
			}
			return true;
		}
	case ASS_GR_GAME_BEGIN:		//��Ϸ��ʼ
		{
			//Ч������
			if (uDataSize!=0) return false;
			OutputDebugString("robot--------------����ASS_GR_GAME_BEGIN");
			//��������
			BYTE bDeskIndex = pNetHead->bHandleCode;
			for(UINT i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
			{
				UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
				if(pFindUserItem == NULL)
					continue;
				if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)
				{
					pFindUserItem->GameUserInfo.bUserState = USER_PLAY_GAME;					
				}
			}
			////////////�����Զ�����zht2008-7-18
			if(m_GameInfo.pMeUserInfo!=NULL&&bDeskIndex == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
			{
				//����������Ϣ
				CString strBuffer;
				CString s = CINIFile::GetAppPath ();/////����·��
				CINIFile f( s +"automsg.ini");
				CString key = TEXT("AUTOMSG");
				int AutoMsg=f.GetKeyVal(key,"IsAuto",1);
				if (AutoMsg == 0)
					return true;
				TCHAR szName[21]={0};
				srand((unsigned)GetTickCount()+m_pLogonInfo.dwUserID);
				int i = rand()%500;
				if (i > 100)
					return true;
				wsprintf(szName,"MSG%d",i);

				strBuffer = f.GetKeyVal(key,szName,"");
				if (strBuffer.IsEmpty())
				{
					return true;
				}
				int r = f.GetKeyVal(key,"r",0);
				int g = f.GetKeyVal(key,"g",255);
				int b = f.GetKeyVal(key,"b",255);
				MSG_GR_RS_NormalTalk Talk;
				Talk.iLength=strBuffer.GetLength();
				Talk.dwSendID=m_pLogonInfo.dwUserID;
				Talk.dwTargetID=0L;
				lstrcpy(Talk.szMessage,strBuffer);
				Talk.crColor=RGB(r,g,b);
				if (i > 50)
				{
					m_TCPSocket->SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GM_GAME_FRAME,ASS_GM_NORMAL_TALK,FALSE);
				}
				else
				{
					m_TCPSocket->SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GR_ROOM,ASS_GR_NORMAL_TALK,0);
				}
			}
           /////////////////�Զ�����END
			return true;
		}
	case ASS_GR_GAME_END:		//��Ϸ����
		{
			//Ч������
			if (uDataSize!=0) 
			{
				return false;
			}
			if (m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				//������ʱ������һ�̺󷿼䲻���ȡ�κδ�ʩ��ֱ�ӵȴ����������
				return true;
			}
		    bool bHaveplayer=false; //�Ƿ�����ʵ���
			//��������
			BYTE bDeskIndex = pNetHead->bHandleCode;
			for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
			{
				UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
				if(pFindUserItem == NULL)
				{
					continue;
				}
				if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)
				{
					pFindUserItem->GameUserInfo.bUserState = USER_DESK_AGREE;
					if (pFindUserItem->GameUserInfo.isVirtual == false)
					{
						bHaveplayer = true;
					}
				}
			}
			//������Ϸ������1/2�����뿪
			if(m_GameInfo.pMeUserInfo!=NULL&&bDeskIndex == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
			{
				CTime t = CTime::GetCurrentTime();
				TTime_day t_day(t.GetHour(), t.GetMinute());
				if (CmpTime_dat(m_ExitTime, t_day) <= -1)
				{
					OnSocketCloseEvent();
					return true;
				}
			}


			if (m_nOnLinePeople > CAuto_AIDlg::m_nRoomPeopleSize)
			{
				m_nOnLinePeople--;
                StandingDesk();
				CloseGameRoom();
			}

			if(m_GameInfo.pMeUserInfo!=NULL&&bDeskIndex == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
			{
				if(rand()%5 == 1)
				{
					if (m_bAutoDesk)
					{
						StandingDesk();
					}
				}

				if (bHaveplayer && m_bMachineAndPlay == false)//��������ͬ��
				{
					StandingDesk();
				}

				if(m_GameInfo.pMeUserInfo!=NULL)
				{
					__int64 Temp = (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money+m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank);
					
					m_iMyMoney = Temp;
					
					AiControlBank();
				}

				if (g_bUserAI)
				{
					CString ad=CINIFile::GetAppPath ();
					CINIFile f( ad + "AIConfig.ini");
					int static iConnect=1;
					CString m_key = "Action";
					CString sp1 = f.GetKeyVal(m_key,"GameFinishStand", "0.3");
					double fp1 = atof(sp1);
					if (randf() < fp1)
					{
						StandingDesk();		
					}

					CString sp2 = f.GetKeyVal(m_key,"GameFinishLeave", "0.1");
					double fp2 = atof(sp2);
					if (randf() < fp2)
					{
						OnSocketCloseEvent();
					}
				}
			}				
			return true;
		}
	case ASS_GR_INSTANT_UPDATE:		//��ʱ���·������
		{
			return TRUE;
		}
	case ASS_GR_USER_POINT:		//�û���һ���
		{
			//TCHAR sz[200];
			//wsprintf(sz,"pNetHead->bHandleCode = %d,uDataSize  = %d,MSG_GR_S_RefalshMoney = %d",
			//	pNetHead->bHandleCode,uDataSize,sizeof(MSG_GR_S_RefalshMoney));
			//WriteStr(sz);
			if(pNetHead->bHandleCode==10)//ͬ�����
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_S_RefalshMoney)) 
				{
					return false;
				}
				MSG_GR_S_RefalshMoney * pReflashMoney=(MSG_GR_S_RefalshMoney*)pNetData;

				//��������
				UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pReflashMoney->dwUserID);
				if (pUserItem==NULL) 
				{
					return false;
				}

				pUserItem->GameUserInfo.i64Money=pReflashMoney->i64Money;

				if(m_GameInfo.pMeUserInfo!=NULL && pReflashMoney->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					__int64 Temp = (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money+m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank);

	
					m_iMyMoney = Temp;

	
					//AiControlBank();
				}

				//�����û�
				//m_UserManage.UpdateGameUser(pUserItem);

				return true;
			}else if(pNetHead->bHandleCode==11)//ͬ������ֵ
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_S_RefalshMoney)) 
				{
					return false;
				}
				MSG_GR_S_RefalshMoney * pReflashMoney=(MSG_GR_S_RefalshMoney*)pNetData;

				//��������
				UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pReflashMoney->dwUserID);
				if (pUserItem==NULL) 
				{
					return false;
				}
				pUserItem->GameUserInfo.dwPoint=pReflashMoney->i64Money;

				if(m_GameInfo.pMeUserInfo!=NULL && pReflashMoney->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					__int64 Temp = (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money+m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank);

//					m_i64NowMoney+=(Temp-m_iMyMoney);

					m_iMyMoney = Temp;

	//				::PostMessage(GetParent()->m_hWnd,IDM_UPDATE_USER,0,0);

					//AiControlBank();
				}
				//�����û�
				//m_UserManage.UpdateGameUser(pUserItem);
				return true;
			}

			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserPoint)) return false;
			MSG_GR_R_UserPoint * pUserPoint=(MSG_GR_R_UserPoint *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserPoint->dwUserID);
			if (pUserItem!=NULL)
			{
				//��������

				pUserItem->GameUserInfo.dwPoint+=pUserPoint->dwPoint;
				pUserItem->GameUserInfo.i64Money+=pUserPoint->dwMoney;
				pUserItem->GameUserInfo.uWinCount+=pUserPoint->bWinCount;
				pUserItem->GameUserInfo.uLostCount+=pUserPoint->bLostCount;
				pUserItem->GameUserInfo.uMidCount+=pUserPoint->bMidCount;
				pUserItem->GameUserInfo.uCutCount+=pUserPoint->bCutCount;

				if(m_GameInfo.pMeUserInfo!=NULL && pUserPoint->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					__int64 Temp = (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money+m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank);

//					m_i64NowMoney+=(Temp-m_iMyMoney);

					m_iMyMoney = Temp;

//					::PostMessage(GetParent()->m_hWnd,IDM_UPDATE_USER,0,0);

					//AiControlBank();
				}
			}
			return true;
		}
	case ASS_GR_INVITEUSER:		//�����û�
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_SR_InviteUser)) return false;
			MSG_GR_SR_InviteUser * pInviteUser=(MSG_GR_SR_InviteUser *)pNetData;
			if(m_bEnterStyle < 7)
			{
				return true;//����������
			}
			if (!m_bMachineAndPlay)
			{
				return true;
			}

			BYTE bDeskIndex = pNetHead->bHandleCode;
			BYTE bDeskStation = GetNullStation(bDeskIndex);

			if(bDeskIndex == 255||bDeskStation == 255 ||m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_PLAY_GAME)
			{
				return true;
			}
			MSG_GR_S_UserSit UserSit;
			UserSit.bDeskIndex = bDeskIndex;
			UserSit.bDeskStation = bDeskStation;

			//���淢�����µ���Ϣ
			m_TCPSocket->SendData(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_USER_SIT,0);
			return true;
		}
	default:
		return true;
	}
	return true;
}

//�û���������
bool CGameRoomEx::OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_USER_HIT_BEGIN:
		{
			//Ч������
			if(uDataSize!=sizeof(MSG_GM_WatchSet))
				return false;
			MSG_GM_WatchSet* pUserID = (MSG_GM_WatchSet*) pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserID->dwUserID);
			pUserItem->GameUserInfo.bUserState = USER_DESK_AGREE;
			return true;
		}
	case ASS_GR_USER_UP:		//�û�����
		//case ASS_GR_WATCH_UP:		//�Թ�����
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserSit)) return false;
			MSG_GR_R_UserSit * pUserUp=(MSG_GR_R_UserSit *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserUp->dwUserID);

			if (pUserItem!=NULL)
			{
				//����
				BYTE bTemp = pUserItem->GameUserInfo.bDeskNO;
				//��������
				pUserItem->GameUserInfo.bDeskNO = 255;
				pUserItem->GameUserInfo.bDeskStation = 255;
				pUserItem->GameUserInfo.bUserState = pUserUp->bUserState;

				if(pUserUp->dwUserID == m_pLogonInfo.dwUserID)//�Լ�����
				{
					if(m_IGameFrame != NULL)
					{
						m_IGameFrame->AFCCloseFrame();
						delete m_IGameFrame;
					}
					m_IGameFrame= NULL;
					return true;
				}
				//����
				if(bTemp == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
				{
					//if(NeedChange(bTemp))
					//{
					//	//��������
					//	//FindOtherDesk();
					//}
				}

			}
			return true;
		}
	case ASS_GR_USER_SIT:		//�û�����
		//case ASS_GR_WATCH_SIT:		//�Թ�����
		{
	//Ч������

			if (uDataSize!=sizeof(MSG_GR_R_UserSit)) 
			{
				return false;
			}
			MSG_GR_R_UserSit * pUserSit=(MSG_GR_R_UserSit *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserSit->dwUserID);
			if (pUserItem!=NULL)
			{
				if(pUserItem->GameUserInfo.dwUserID == m_pLogonInfo.dwUserID )//��������
				memset(m_bErrorDeskFlag, 0, sizeof(m_bErrorDeskFlag));
			}
			if (pUserItem!=NULL)
			{
				/////////////////////////////////////////
				//��������
				pUserItem->GameUserInfo.bDeskNO=pUserSit->bDeskIndex;
				pUserItem->GameUserInfo.bDeskStation=pUserSit->bDeskStation;

				pUserItem->GameUserInfo.bUserState=pUserSit->bUserState;
				if(pUserItem->GameUserInfo.dwUserID == m_pLogonInfo.dwUserID )//��������
				{OutputDebugString("cqro::----��������");
					//3���¼��𲻴�������
					if( m_bEnterStyle < 3)
					{
						return true;
					}
					StartGameClient();
					//���ƿͻ���
					CM_UserState_Change ControlMessage;
					ControlMessage.bActionCode=ACT_USER_SIT;
					ControlMessage.bDeskStation = m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
					ControlMessage.pUserItem = m_GameInfo.pMeUserInfo;
					ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
				}else
				{
					if (m_bMachineAndPlay == false)//���������ͬ��
					{
					
						if(pUserItem->GameUserInfo.bDeskNO == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
						{
							//���������,����������
							if(/*IsPlayerID(pUserItem->GameUserInfo.dwUserID)*/!pUserItem->GameUserInfo.isVirtual
								&&m_GameInfo.pMeUserInfo->GameUserInfo.isVirtual/*IsMachineID(m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)*/)
								StandingDesk();

						}
					}
			
				}
			}
			return true;
		}
	case ASS_GR_QUEUE_USER_SIT:
		{
			int* pData = (int*)pNetData;
			if (pNetHead->uMessageSize - sizeof(NetMessageHead) != sizeof(int) * (m_GameInfo.uDeskPeople*3 + 1)) return false;
			int _DeskIndex = *pData;
			pData++;

			bool bIsSelf = false;
			for (int i = 1; i < m_GameInfo.uDeskPeople + 1; i++)
			{
				UserItemStruct * pUserItem = m_UserManage.FindOnLineUser((long)*pData++);
				if (pUserItem != NULL)
				{
					// PengJiLin, 2010-5-19���Ŷӻ�������������ʾ�����Ϣ
					if(pUserItem->GameUserInfo.dwUserID !=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
						strcpy(pUserItem->GameUserInfo.nickName, "���");

					pUserItem->GameUserInfo.bDeskNO= _DeskIndex;
					pUserItem->GameUserInfo.bDeskStation = i - 1;
					pUserItem->GameUserInfo.bUserState = USER_SITTING;
					pUserItem->GameUserInfo.iRankNum = (long)(*(pData++));
					pUserItem->GameUserInfo.iRemainPeople = (long)(*(pData++));
					if ((pUserItem->GameUserInfo.dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID))
					{				
						bIsSelf = true;
						m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO = _DeskIndex;
						m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation = i - 1;
					}
				}
			}

			if (bIsSelf)
			{
				pData = (int*)pNetData;
				pData++;

				//�ر���Ϸ��
				if(m_IGameFrame!=NULL && !(m_GameInfo.dwRoomRule & GRR_CONTEST))
				{
					m_IGameFrame->AFCCloseFrame();
				}
				StartGameClient();

				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode= ACT_USER_SIT;
				ControlMessage.bDeskStation= m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
				ControlMessage.pUserItem=m_GameInfo.pMeUserInfo;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
			}
			return true ;
		}
	case ASS_GR_USER_COME:		//�û�����
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserCome)) return false;
			MSG_GR_R_UserCome * pUserCome=(MSG_GR_R_UserCome *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.AddGameUser(&pUserCome->pUserInfoStruct,1,0);
			CGameRoomEx::m_nOnLinePeople = m_UserManage.GetOnLineUserCount();
			return true;
		}
	case ASS_GR_USER_LEFT:		//�û��뿪
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserLeft)) return false;
			MSG_GR_R_UserLeft * pUserLeft=(MSG_GR_R_UserLeft *)pNetData;


			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserLeft->dwUserID);
			if (pUserItem!=NULL)
			{
				m_UserManage.DeleteGameUser(pUserItem);
			}
			CGameRoomEx::m_nOnLinePeople = m_UserManage.GetOnLineUserCount();

			if (m_nOnLinePeople < CAuto_AIDlg::m_nRoomPeopleSize)
			{
				::PostMessage(GetParent()->m_hWnd,IDM_UPDATE_USER,0,0);
			}
			return true;
		}
	case ASS_GR_USER_CUT:		//�û�����
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserCut)) return false;
			MSG_GR_R_UserCut * pUserCut=(MSG_GR_R_UserCut *)pNetData;
			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserCut->dwUserID);
			if (pUserItem!=NULL)
			{
				//��������
				pUserItem->GameUserInfo.bUserState=USER_CUT_GAME;
			}

			return true;
		}
	case ASS_GR_SET_TABLE_BASEPOINT:	//�����û����ӱ���
		{	
			return true;
		}
	case ASS_GR_SIT_ERROR:		//���´���
		{
			//20090227���������´�����
			if (m_bErrorDeskCount > 20)
			{
				m_bErrorDeskCount = 0;
				memset(m_bErrorDesk, 255, sizeof(m_bErrorDesk));
			}
			m_bErrorDesk[m_bErrorDeskCount] = m_bNowDesk;
			m_bErrorDeskCount++;

			m_bErrorDeskFlag[m_bNowDesk] = true;

			OnSitDesk();
			return true;
		}
	}
	return true;
}

//������Ϸ�ͻ���
bool CGameRoomEx::StartGameClient()
{
	try
	{
		//��ȡ���� 
		if (m_IGameFrame != NULL)
		{
			delete m_IGameFrame;
			m_IGameFrame = NULL;
		}
		//if (m_IGameFrame==NULL)
		{
			m_IGameFrame = new CClientGameDlg();

			m_IGameFrame->InitFrame(&m_GameInfo,this);
		}
		return true;
	}
	catch (...) {}

	m_IGameFrame->AFCCreateFrame();

	return false;
}

//���Ϳ�����Ϣ 
UINT CGameRoomEx::ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize)
{
	try
	{
		if (m_IGameFrame!=NULL)
			m_IGameFrame->OnControlMessage(uControlCode,pControlData,uDataSize);
	}
	catch (...) {}
	return 0;
}

//��ʱ����Ϣ
LRESULT CGameRoomEx::SetGameTime(WPARAM wparam,LPARAM lparam)
{
	SetTimer(wparam,lparam,NULL);
	return TRUE;
}

//�ر���Ϸ,����Ϸ�еĻ����˷�ǿ����Ϣ��
//add 20090211
//zht
LRESULT CGameRoomEx::OnCloseGame(WPARAM wparam,LPARAM lparam)
{
	if (NULL != m_IGameFrame)
	{
		CClientGameDlg* FrameGame = (CClientGameDlg*)m_IGameFrame;
		//sdp20140624 д���ж����������ú꣬��ֹÿ�α�����ܲ�ͨ��
		if (FrameGame->GetStationParameter() >= 2 &&FrameGame->GetStationParameter() <= GS_WAIT_NEXT)
		{
			SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
		}

		m_IGameFrame->AFCCloseFrame();
	}
	return TRUE;
}

//��ʱ����Ϣ
LRESULT CGameRoomEx::KillGameTime(WPARAM wparam,LPARAM lparam)
{
	if((UINT)wparam == 0)
	{
		for(int i = 100;i < 110;i++)
		{
			KillTimer(i);
		}
		return 0;

	}
	KillTimer((UINT)wparam);
	return TRUE;
}

//��ʱ��
void CGameRoomEx::OnTimer(UINT_PTR uIDEvent)
{
	if(uIDEvent >= 100 && uIDEvent <= 200 )//100-110��ʱ��Ϊ��Ϸ�߼�ʹ��
	{
		KillTimer(uIDEvent);
		if (m_IGameFrame!=NULL)
		{
			m_IGameFrame->OnGameTimer(0,uIDEvent,0);
		}
		return ;
	}
	//1-100Ϊ����ʹ��
	switch(uIDEvent)
	{
	case ID_CHECK_STATE://��ǰ״̬����
		{
			//��Ϸ�л���ʹ�������У�����Ϸ
			if(m_GameInfo.pMeUserInfo == NULL || m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_PLAY_GAME || m_UseBank)
			{
				///rqffffffffffffffffffffff
			/*	if (g_bUserAI)
				{
					CString ad=CINIFile::GetAppPath ();
					CINIFile f( ad + "AIConfig.ini");
					int static iConnect=1;
					CString m_key = "Action";


					CString sp2 = f.GetKeyVal(m_key,"GameForceLeave", "0.1");//�Զ�����ʱ��
					double fp2 = atof(sp2);
					if (randf() < fp2)
					{OutputDebugString("cqro::------------------�ر�4");
						OnSocketCloseEvent();
					}
				}*/
				break;
			}
			
			if (m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				KillTimer(ID_CHECK_STATE);
				//�����������˿�ʼ����
				m_TCPSocket->SendData(MDM_GR_USER_ACTION, ASS_GR_CONTEST_APPLY, 0);				
				return;
			}
			//��Ҳ�����(�������ڽ�ҳ�)
			if(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < m_GameInfo.uLessPoint && m_GameInfo.uComType == TY_MONEY_GAME)
			{
				KillTimer(ID_CHECK_STATE);
				KillTimer(ID_CLEAR_ERRORDESK);
				CloseGameRoom();
				//g_ErrorUser.insert(std::pair<std::string,int>(m_GameInfo.pMeUserInfo->GameUserInfo.szName,m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID));
				
				return ;
			}
			if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO == 255)//δ�����������¼�ʱ��
			{
				OnSitDesk();
			}
			else
			{
				///rqffffffffffffffffffffff
			/*	if (g_bUserAI)
				{
					CString ad=CINIFile::GetAppPath ();
					CINIFile f( ad + "AIConfig.ini");
					int static iConnect=1;
					CString m_key = "Action";
					CString sp1 = f.GetKeyVal(m_key,"GameSitStand", "0.33");//�Զ�����ʱ��
					double fp1 = atof(sp1);
					if (randf() < fp1)
						StandingDesk();

					CString sp2 = f.GetKeyVal(m_key,"GameSitLeave", "0.1");//�Զ�����ʱ��
					double fp2 = atof(sp2);
					if (randf() < fp2)
					{
						OutputDebugString("cqro::------------------�ر�5");
						OnSocketCloseEvent();
					}
				}*/

				if(rand()%20 > 5 )//�������
				{
					//���������ֲ���Ҫ������������
					//OnInvitePlay();
					break;
				}
				else
				{
					if (m_bAutoDesk)
					{
						StandingDesk();//����
						break;
					}
				}
				m_CheckCount++;
				if (m_CheckCount > m_CheckCountMax)
				{
					StandingDesk();//����
					break;
				}
			}
			break;
		}
	case ID_CLEAR_ERRORDESK:
		{
			//20090227���������´�����
			m_bErrorDeskCount = 0;
			memset(m_bErrorDesk, 255, sizeof(m_bErrorDesk));

			memset(m_bErrorDeskFlag, 0, sizeof(m_bErrorDeskFlag));
			

			break;
		}
	case ID_CHECK_AIBANK:
		{
			KillTimer(ID_CHECK_AIBANK);
			m_UseBank = FALSE;
			//AiControlBank();
			break;
		}
	case IDT_GAMEEXIT:
		{
			CTime t = CTime::GetCurrentTime();
			TTime_day t_day(t.GetHour(), t.GetMinute());
			if (CmpTime_dat(m_ExitTime, t_day) <= -1)
			{
				if (m_GameInfo.pMeUserInfo != NULL && 
					m_GameInfo.pMeUserInfo->GameUserInfo.bUserState != USER_PLAY_GAME)
				{
					OutputDebugString("cqro::------------------�ر�6");
					OnSocketCloseEvent();
				}
			}
			break;
		}
	}
	return __super::OnTimer(uIDEvent);
}

//�������
void CGameRoomEx::OnInvitePlay()
{
	long dwInvertID = GetInvertID();
	if(dwInvertID == 0)
		return ;
	//��������
	MSG_GR_SR_InviteUser InviteUser;
	InviteUser.dwUserID = m_pLogonInfo.dwUserID;
	InviteUser.dwTargetID = dwInvertID;
	m_TCPSocket->SendData(&InviteUser,sizeof(InviteUser),MDM_GR_ROOM,ASS_GR_INVITEUSER,m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO);
	return;
}

//��ȡ�������
LONG CGameRoomEx::GetInvertID()
{
	//�����������û��в���
	for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
	{
		UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
		if(pFindUserItem == NULL)
		{
			continue;
		}
		if(pFindUserItem->GameUserInfo.bDeskNO == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO //ͬ��
			|| pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)					   //������Ϸ�Ѿ���ʼ
		{
			continue;
		}
		//����Ƿ����㹻
		if(pFindUserItem->GameUserInfo.i64Money < m_GameInfo.uLessPoint)
		{
			continue;
		}
		//�����Ƿ����������
		if(pFindUserItem->GameUserInfo.isVirtual/*IsMachineID(pFindUserItem->GameUserInfo.dwUserID)*/)
		{
			continue;
		}
		return pFindUserItem->GameUserInfo.dwUserID;
	}
	return 0;
}
//ʹ��isVirtual���ж��Ƿ������
//�Ƿ�Ϊ������ID
//bool CGameRoomEx::IsMachineID(LONG dwUserID)
//{
//	if(dwUserID <= m_iMachineCheckTime)
//	{
//		return true;
//	}
//	return false;
//}
////�Ƿ�Ϊ������ID
//bool CGameRoomEx::IsPlayerID(LONG dwUserID)
//{
//	if(dwUserID > m_iMachineCheckTime)
//	{
//		return true;
//	}
//	return false;
//}
//�Ƿ�Ϊ����������
bool CGameRoomEx::IsErrorDesk(BYTE desk)
{
	return m_bErrorDeskFlag[desk];

	for (int i=0; i<m_bErrorDeskCount; i++)
	{
		if (m_bErrorDesk[i] == desk)
		{
			return true;
		}
	}
	return false;
}


///Ǯ���˸�Ǯ
bool CGameRoomEx::OnAboutMoney(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (NULL == m_GameInfo.pMeUserInfo)
	{
		return true;
	}
	/*CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	TCHAR szNum[32];
	/// �����Ϸ�����ˣ����͵���Ϸ�ͻ���
	if (NULL != m_IGameFrame)
	{
	m_IGameFrame->OnSocketReadEvent(pNetHead, pNetData, uDataSize, pClientSocket);
	}*/
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_CHECK_OUT://ȡǮ
	case ASS_GR_CHECK_OUT_INGAME:	// ��Ϸ��ȡǮҲ��Ҫ���£�������ʾ�Ի���
		{

			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_ERROR)
			{
				if (ASS_GR_CHECK_OUT == pNetHead->bAssistantID)
				{
					//CString strBuffer;
					//CString str;
					//str = fMsg.GetKeyVal("GameRoom","NetWorkBusy","ȡ��ʧ�ܣ�����ԭ�������벻��ȷ�����㣡");
					//strBuffer.Format(str);
					//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,this);
					WriteStr("ȡ��ʧ�ܣ�����ԭ�������벻��ȷ�����㣡");
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_ERROR_PLAYING) //��Ϸ�в���ȡǮ add by lxl 2010-12-6
			{
				if (ASS_GR_CHECK_OUT == pNetHead->bAssistantID)
				{
					//CString strBuffer;
					//CString str;
					//str = fMsg.GetKeyVal("GameRoom","NoDrawForPlaying","��������Ϸ�У�����ȡ�");
					//strBuffer.Format(str);
					//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
					WriteStr("������Ϸ�У�����ȡ�");
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_SUC)
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
				MSG_GR_CheckMoney * pCheckOut=(MSG_GR_CheckMoney *)pNetData;
				
				if(pCheckOut->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)//�Լ�
				{

					if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
					MSG_GR_CheckMoney * pCheckIn=(MSG_GR_CheckMoney *)pNetData;

					if(pCheckIn->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)//�Լ�
					{
						UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pCheckIn->dwUserID);
						if(pUserItem==NULL)
							return false;

						//��������
						pUserItem->GameUserInfo.i64Money += pCheckIn->i64CheckOut;
						pUserItem->GameUserInfo.i64Bank -= pCheckIn->i64MoneyInBank;//���н��
						//m_UseBank = FALSE;
					}
				}
				//���½����ϢȡǮ�ɹ�

			}
			
			break;
		}
	case ASS_GR_CHECK_IN://��Ǯ
	case ASS_GR_CHECK_IN_INGAME://��Ϸ�д�Ǯ
		{
			
			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ERROR)
			{
				if (ASS_GR_CHECK_IN == pNetHead->bAssistantID)
				{
					//CString strBuffer;
					//CString str;
					//str = fMsg.GetKeyVal("GameRoom","NetWorkBusySave","����ӵ��,����ʧ��!���Ժ�����!");
					//strBuffer.Format(str);
					//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
					WriteStr("����ӵ��,����ʧ��!���Ժ�����!��");
					
				}
			}
			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ERROR_PLAYING)
			{
				if (ASS_GR_CHECK_IN == pNetHead->bAssistantID)
				{
					//CString strBuffer;
					//CString str;
					//str = fMsg.GetKeyVal("GameRoom","NoSaveForPlaying","��������Ϸ�У����ܴ�");
					//strBuffer.Format(str);
					//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
					WriteStr("��������Ϸ�У����ܴ�");
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ZHUANGJIA)  //�ټ�������Ϸ ��ׯ�в��ܴ�� add by lxl 2010-11-5
			{
				if (ASS_GR_CHECK_IN == pNetHead->bAssistantID)
				{
					//CString strBuffer;
					//CString str;
					//str = fMsg.GetKeyVal("GameRoom","NoSaveForZhuangjia","��������ׯ�У����ܴ�");
					//strBuffer.Format(str);
					//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
					WriteStr("��������ׯ�У����ܴ�");
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_SUC)
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
				MSG_GR_CheckMoney * pCheckIn=(MSG_GR_CheckMoney *)pNetData;

				if(pCheckIn->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)//�Լ�
				{
					UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pCheckIn->dwUserID);
					if(pUserItem==NULL)
						return false;

					//��������
					pUserItem->GameUserInfo.i64Money -= pCheckIn->i64CheckOut;
					pUserItem->GameUserInfo.i64Bank += pCheckIn->i64MoneyInBank;//���н��
					
				}
				//��Ǯ�ɹ�
			}
			//m_UseBank = FALSE;
			break;
		}
		break;
	}
	return true;
}

//�����˸��ݽ�Ҳ�������
void CGameRoomEx::AiControlBank()
{
	//��������ʹ�����й���
	if (m_bAiBank && !m_UseBank)
	{
		srand(m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID*(int)GetCurrentTime());//ȡ�������
		if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money > m_i64MaxAiMoney && !(m_GameInfo.dwRoomRule & GRR_CONTEST))
		{
			///ȡmix - max ֮��������
			__int64 i64RandMoney = m_i64CheckMoneyMax - m_i64CheckMoneyMix;
			//��ֹȡ���ֵΪ0
			if(m_i64CheckMoneyMax==m_i64CheckMoneyMix)
			{
				m_i64CheckMoneyMax=_abs64(m_i64CheckMoneyMax*2);
			}
			UINT nrand = 0;
			if (m_bAiBank && m_bOpenBankBase)
			{
				nrand = rand()*m_i64SaveAndOutMoneyBase;
			}
			else
			{
				nrand = rand()*100000;
			}
			
			i64RandMoney = (nrand%i64RandMoney) + m_i64CheckMoneyMix + m_i64SaveAndOutMoneyBase;
			__int64 Temp = m_GameInfo.pMeUserInfo->GameUserInfo.i64Money - i64RandMoney;
						
			m_UseBank = true;
			StandingDesk();
			SetTimer(ID_CHECK_AIBANK,10000,NULL);
			CheckMoney(Temp);			
		}
		else if(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < m_i64CheckMoneyMix || m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < m_i64SaveAndOutMoneyBase)
		{
			///ȡmix - max ֮��������
			__int64 i64RandMoney = m_i64CheckMoneyMax - m_i64CheckMoneyMix;
			if(m_i64CheckMoneyMax==m_i64CheckMoneyMix)
			{
				m_i64CheckMoneyMax=_abs64(m_i64CheckMoneyMax*2);
			}

			UINT nrand = 0;
			if (m_bAiBank && m_bOpenBankBase)
			{
				nrand = rand()*m_i64SaveAndOutMoneyBase;
			}
			else
			{
				nrand = rand()*100000;
			}

			i64RandMoney = (nrand%i64RandMoney) + m_i64CheckMoneyMix + m_i64SaveAndOutMoneyBase;
			
			m_UseBank = true;
		//	if (!(m_GameInfo.dwRoomRule & GRR_CONTEST))
			{
				StandingDesk();
			}
			SetTimer(ID_CHECK_AIBANK,10000,NULL);
			CheckMoneyWithPwd(i64RandMoney);			
		}
	}
}
///��Ǯ
void CGameRoomEx::CheckMoney(__int64 iMoney)
{

	//if (m_GameInfo.uNameID == 10900400
	//	|| m_GameInfo.uNameID == 10900200)//ţţ��Ϸ��ȡǮ���뿪����
	//{
	//Ϊ������쳣��ǮҪ�뿪���ӡ�
	/*	if (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
		{
			StandingDesk();
		}*/
	//}
	OutputDebugString("dwjloglog::CheckMoney");
	if (m_bAiBank && m_bOpenBankBase)
	{
		OutputDebugString("dwjloglog::CheckMoney1");
		iMoney = iMoney/m_i64SaveAndOutMoneyBase;
		iMoney *= m_i64SaveAndOutMoneyBase;
	}
	CString dwj;
	dwj.Format("dwjloglog::��Ǯ=%d",iMoney);
	OutputDebugString(dwj);

	/// ȡ���ַ���������
	MSG_GR_S_CheckMoney checkMoney;
	memset(&checkMoney,0,sizeof(checkMoney));
	checkMoney.i64Count = iMoney;
	checkMoney.UserID = m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;//m_pLogonInfo.dwUserID;
	
	/*CString str;
	str.Format("zhtlog:���룺%I64d checkMoney.UserID=%d",iMoney,checkMoney.UserID);*/
	m_TCPSocket->SendData(&checkMoney,sizeof(checkMoney),MDM_GR_MONEY,ASS_GR_CHECK_IN,0);
}

void CGameRoomEx::CheckEditMoney(__int64 iMoney, int UserID)
{
	//	if (m_GameInfo.uNameID == 10900400
	//	|| m_GameInfo.uNameID == 10900200)//ţţ��Ϸ��ȡǮ���뿪����
	//{
	/*	if (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
		{
			StandingDesk();
		}*/
	//}
	/// ȡ���ַ���������
	MSG_GR_S_CheckMoney checkMoney;
	memset(&checkMoney,0,sizeof(checkMoney));
	checkMoney.i64Count = iMoney;
	checkMoney.UserID = UserID;//m_pLogonInfo.dwUserID;
	
	/*CString str;
	str.Format("zhtlog:���룺%I64d checkMoney.UserID=%d",iMoney,checkMoney.UserID);*/
	m_TCPSocket->SendData(&checkMoney,sizeof(checkMoney),MDM_GR_MONEY,ASS_GR_CHECK_IN,0);
}

///ȡǮ
void CGameRoomEx::CheckMoneyWithPwd(__int64 iMoney)
{

	//if (m_GameInfo.uNameID == 10900400
	//	|| m_GameInfo.uNameID == 10900200)//ţţ��Ϸ��ȡǮ���뿪����
	//{
		if (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
		{
			StandingDesk();
		}
	OutputDebugString("dwjloglog::CheckMoneyWithPwd");
	// �����˻����˲������� ��������ȡ �Ǿͳ��Ի����ٳ��Ի���
	if (m_bAiBank && m_bOpenBankBase)
	{
		OutputDebugString("dwjloglog::CheckMoneyWithPwd1");
		iMoney = iMoney/m_i64SaveAndOutMoneyBase;
		iMoney *= m_i64SaveAndOutMoneyBase;
	}
	CString dwj;
	dwj.Format("dwjloglog::ȡǮ=%d",iMoney);
	OutputDebugString(dwj);
	//dwj.FORMAT_EX_PARAMETERS(
	//}
	//ȡǮ
	MSG_GR_S_CheckMoneyWithPwd_t cm;
	cm.i64Count = iMoney;
	cm.UserID = m_pLogonInfo.dwUserID;
	//GetCryptedPasswd(cm.szMD5Pass, pCheckMoney->szPWD);
	memcpy(cm.szMD5Pass,m_pLogonInfo.szMD5Pass,sizeof(m_pLogonInfo.szMD5Pass));
	//cm.szMD5Pass = m_pLogonInfo->szMD5Pass;
	m_TCPSocket->SendData(&cm,sizeof(cm),MDM_GR_MONEY,ASS_GR_CHECK_OUT,0);
}

void CGameRoomEx::SetBankInfo(BOOL bAiBand,BOOL bOpenBankBase, __int64 i64MaxAiMoney, __int64 i64CheckMoneyMax, __int64 i64CheckMoneyMix, __int64 i64SaveAndOutMoneyBase)
{
	m_i64MaxAiMoney     = i64MaxAiMoney;                    //AI��������Ǯ�����ﵽ������ʹ�Ǯ
	m_i64CheckMoneyMax  = i64CheckMoneyMax;                 //ȡǮ�����
	m_i64CheckMoneyMix  = i64CheckMoneyMix;                 //ȡǮ��С��
	m_bAiBank           = bAiBand;                          //�Ƿ�����˲�������
	m_bOpenBankBase		= bOpenBankBase;						//�Ƿ���
	m_i64SaveAndOutMoneyBase =  i64SaveAndOutMoneyBase;		//������ȡ����
}

void CGameRoomEx::SetExitTime(TTime_day time)
{
	m_ExitTime = time;
}