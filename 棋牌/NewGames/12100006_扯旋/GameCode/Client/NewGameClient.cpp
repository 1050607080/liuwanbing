#include "stdafx.h"
#include "NewGameClient.h"
//#include "IGameFrame.h"
//#include "UI8_ControlsDefine.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "LoveSendClassInExe.h"
#include "define.h"
IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
	return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
	delete *ppImp;
	*ppImp = NULL;
}
/**
* ����ǰ��ұ��ʵõ��ַ���
* @param [int] int nNum Ҫ��ʾ������
* @param [out] CString &Str ���ص��ַ���
* @param [int] int nPower ��ұ���
* @param [in]  BOOL bIsSpace TRUE: �ÿո�ָ� FALSE: ���ָ�
* @return void
*/
void GlbGetFloatString( char  Str[256], __int64 nNum, int nPower=0, BOOL bIsSpace=TRUE)
{
	if (0 == nNum)
	{
		wsprintf(Str,"0");
		return;
	}
	// �ݴ���
	TCHAR strTmp[32];
	TCHAR str[50];

	// Ŀǰֻ�������0��������������ַ�������Ҫ������һ��0
	wsprintf(strTmp, TEXT("%I64d"), nNum);

	for (int i = 0; i < nPower; ++i)
	{
		_tcscat(strTmp, TEXT("0"));
	}
	// ��λ��ÿ4λ�ö��Ÿ���ʾ
	int nLen = _tcslen(strTmp);	// �ַ�������
	int nTimes = nLen % 3;	    // ����ַ������ԷֳɵĽ�����������1��
	int nSrc = 0;
	int nDes = 0;
	for (int i = 0; i < nLen; ++i)
	{
		if ((i > 0) && (i % 3 == nTimes) && bIsSpace && (str[i-1] != '-'))
		{
			str[nDes++] = TEXT(',');
		}
		str[nDes++] = strTmp[nSrc++];
	}
	str[nDes] = TEXT('\0');
	wsprintf(Str,"%s",str);
}

CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;
	m_bGameStation = GS_WAIT_SETGAME;           //��Ϸ״̬
	m_iBeginTime = 0;			//��Ϸ��ʼʱ��
	m_iThinkTime = 0;			//��Ϸ˼��ʱ��
	m_iEndWaitTime = 0;			//��Ϸ������ȴ�ʱ��
	m_iCurrentTimeId = 0;       //��ǰ��ʱ��ID
	m_SoundEngine = NULL;
	m_bInLisureRoom = FALSE;
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));			//��������ϵ��˿���
	::memset(m_iUserCard,0,sizeof(m_iUserCard));			//��������ϵ��˿�
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	::memset(m_iDeskCard,0,sizeof(m_iDeskCard));			//������˿�
	::memset(m_bUserReady,0,sizeof(m_bUserReady));
	::memset(m_bPlayer,0x00,sizeof(m_bPlayer));
	::memset(m_byUserOperatorType,0xff,sizeof(m_byUserOperatorType));
	::memset(m_i64UseAllNote,0x00,sizeof(m_i64UseAllNote));				//ÿ���û��µ���ע

	m_bSoundPlay = true;				//�Ƿ���Բ�������
	m_bSoundPlayBG = true;				//�Ƿ�ű�������

	m_SoundEngine = createIrrKlangDevice();
	m_SoundEnginePlay = createIrrKlangDevice();
	m_SoundEnginBg = createIrrKlangDevice();
}
CNewGameClient::~CNewGameClient()
{
	if(m_SoundEngine)
	{
		m_SoundEngine->drop();				//�ͷ���Ч�ļ�
		m_SoundEngine = NULL;
	}
	if(m_SoundEnginePlay)
	{
		m_SoundEnginePlay->drop();				//�ͷ���Ч�ļ�
		m_SoundEnginePlay = NULL;
	}

	if(m_SoundEnginBg)
	{
		m_SoundEnginBg->drop();				//�ͷ���Ч�ļ�
		m_SoundEnginBg = NULL;
	}
}
//��Ϸ�˳�
bool CNewGameClient::OnGameQuiting()
{
	//��ѯ״̬
	if (GetStationParameter() >= GS_SEND_CARD&&GetStationParameter() < GS_WAIT_NEXT && m_bPlayer[m_pGameFrame->GetMyDeskStation()]
	&& !m_pGameFrame->GetIsWatching())
	{
		CString strInfo = "";
		strInfo += "��������Ϸ�У��Ƿ�Ҫ�˳���Ϸ?\n";

		char cKey[10];
		CString sPath=CBcfFile::GetAppPath();
		int iResult = 1;
		if (CBcfFile::IsFileExist(sPath + "..\\SpecialRule.bcff"))
		{
			CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
			sprintf(cKey, "%d", NAME_ID);
			iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
		}
		if (!iResult)
		{
			strInfo.Format("�����˳����ᱻ��������ע�������Ҫ�˳���?");
		}
		else
		{
			strInfo.Format("��ע���˳����ᱻ�й�,�����Ҫ�˳���");
		}
		if(IDOK != AFCMessageBox(strInfo, "��ʾ",AFC_YESNO))
		{
			return false;
		}	
	}
	m_pGameFrame->SendGameData(NULL,0,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	memset(m_bPlayer,0,sizeof(m_bPlayer));
	return true;
}
//���������ļ�
void CNewGameClient::LoadData(void)
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f(s + GET_SKIN_FOLDER(szTempStr) +".bcf");
	CString key = TEXT("Operate");
}
//��ʼ��
int CNewGameClient::Initial()
{
	// ����UI
	LoadData();
	m_pUI->LoadData(L"12100006_UI.dat"); 
	const GameInfoStructInExe* gameinfo =  m_pGameFrame->GetGameInfo();
	m_bInLisureRoom = FALSE; //�����ã���ʽ�ƽ�ɾ��
	memset(m_bPlayer,0,sizeof(m_bPlayer));
	HideNoteAndPic();
	return 0;
}
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	InitGameData();
	InitGameUI();
	ShowControl(IDD_START_BT,false);
	ShowControl(IDD_SET_THING_DLG,false);
	return 0;
}
///������Ϸ��ͬ״̬ʱ���������ݣ���Ҫ���ڶ�������
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	InitGameUI();
	InitGameData();
	const GameInfoStructInExe* gameinfo =  m_pGameFrame->GetGameInfo();
	m_bInLisureRoom = FALSE; //�����ã���ʽ�ƽ�ɾ��
	int iMyDesk = m_pGameFrame->GetMyDeskStation();
	GameStationBase *pGameBase = (GameStationBase *) pBuffer;
	SetStationParameter(pGameBase->bGameStation);
	PlaySound(true);
	UpDateMyUserInfo(); //�����Լ����û���Ϣ	
	UpdateRoomInfo();
	//�������ʣ��Ľ��
	UpdateMyLeftMoney();
	SetUserInfo();
	ShowControl(IDD_START_BT,false);
	
	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_AGREE:
		{
			GameStation_2 * pGameStation=(GameStation_2 *)pBuffer;
			m_iBeginTime=pGameStation->iBeginTime;						//��ʼʱ��
			m_byBoBoTime=pGameStation->byBoBoTime;						//��������ʱ��;
			m_iThinkTime=pGameStation->iThinkTime;						//˼��ʱ��
			m_bySeparateCardTime = pGameStation->bySeparateCardTime;	//����ʱ��
			m_i64BoBase = pGameStation->i64BoBase;						//����
			m_i64SmallBobo = pGameStation->i64SmallBobo;				//��С����
			m_i64TempBobo = m_i64SmallBobo;								//��С����(��ʱ����)
			m_i64FrontNote = 0;
			if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
			{
				ShowControl(IDD_START_BT,false);
			}
			else
			{
				ShowControl(IDD_START_BT,true);
				SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_iBeginTime,ID_BEGIN_TIME);
			}
			memcpy(m_bUserReady , pGameStation->bUserReady ,sizeof(m_bUserReady)) ;
			for(int i = 0x00;i < PLAY_COUNT;i ++)									//��ʾ׼��
			{
				SOrHReadyMark(i,m_bUserReady[i]);
			}
			UpdateRoomInfo();														//������ʾ������Ϣ
			break;
		}
	case GS_SEND_CARD:													//���������������ƶ���
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pBuffer;
			//��������
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
			SetOperateNote(false,false ,false,false,false);								//������ע�Ĳ�����Ч
			if(pGameStation->byGameStation == GS_SET_BOBO_TIME)							//��ǰ��������������ʱ��
			{
				if(byTimeRest != 0x00 && byTimeRest < m_byBoBoTime)
					SetGameTimer(m_pGameFrame->GetMyDeskStation(),byTimeRest,ID_BOBO_TIME);
				for(int i = 0x00;i < PLAY_COUNT;i ++)
				{
					if(pGameStation->bAlreadSetBobo[i])
					{
						BYTE bView = ViewStation(i);
						ShowControl(IMG_OK_00 + bView,true);
					}
				}
				if(pGameStation->bAlreadSetBobo[m_iMyLogDesk] == false)						//�����û������������
				{
					SetShowOrHideBoboAndResetnoteBNT(true);
					UserInfoForGame UserInfo;
					if (m_pGameFrame->GetUserInfo(m_iMyLogDesk,UserInfo))
						SetNoteEnable(UserInfo.i64Money);
				}
			}
			else if(pGameStation->byGameStation == GS_SEND_CARD_TIME)						//����ʱ��
			{
				::CopyMemory(m_iDeskCardCount,pGameStation->byAlreadySendCardCount,sizeof(m_iDeskCardCount));	//�Ѿ�����������
				::CopyMemory(m_iUserCardCount,pGameStation->iUserCardCount,sizeof(m_iUserCardCount));			//��������
				::CopyMemory(m_bSendCardCount,pGameStation->byAlreadySendCardCount,sizeof(m_bSendCardCount));	//�Ѿ�����������
				for(int i = 0x00;i < PLAY_COUNT;i ++)
				{
					memset(m_iDeskCard[i],0x00,sizeof(m_iDeskCard[i]));
					if (m_pGameFrame->GetIsWatching())
					{
						::memset(m_iUserCard,0,sizeof(m_iUserCard));
					}
						::CopyMemory(m_iDeskCard[i],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
						SetHandCard(i,m_iDeskCard[i],NULL,m_iDeskCardCount[i],0);
				}
				SetNT(m_bNtPeople);
			}
			UpdateRoomInfo();
			break;
		}
	case GS_PLAY_GAME:
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pBuffer;
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
			PlayGameSetShowAllNote();										//������ʾ������Ϣ	
			for(int i = 0x00;i< PLAY_COUNT;i ++)							//����������
			{
				for(int j = 0x02;j < m_bSendCardCount[i];j++)
					m_iDeskCard[i][j] = m_iUserCard[i][j];
			}

			for(int i = 0x00;i < PLAY_COUNT;i ++)
			{
				if(m_bPlayer[i])									//��λ������(���Ҳ�����;�����)
				{
					if(m_byUserOperatorType[i] != TYPE_GIVE_UP)
					{
						if(i == m_iMyLogDesk)						//��������ѵ��ƾ���ʾ
						{
							SetHandCard(i,m_iUserCard[i],NULL,m_bSendCardCount[i],0);
						}
						else
						{
							SetHandCard(i,m_iDeskCard[i],NULL,m_bSendCardCount[i],0);
						}
					}
					else
					{
						BYTE bView = ViewStation(i);
						ShowGiveUpPic(m_bSendCardCount[i],IMP_GIVE_UP_BACK_CARD + bView,true);						//��ʾ��������Ƭ
					}
				}
			}
			if(pGameStation->byGameStation == GS_SPARE_CARD_TIME)															//����ʱ��
			{
				if(m_bPlayer[m_iMyLogDesk] && !m_pGameFrame->GetIsWatching())			//����,ͬʱ�����Թ���
				{
					SetShowSeparetCardBntAndImageBottomCard(true);

					if(m_byUserOperatorType[m_iMyLogDesk] == TYPE_GIVE_UP)
					{
						ShowControl(IMG_BOTTOM_OPERATOR_CARD,false);						//������ʾ��ͼ
						return ;
					}

					IOperateCard *pOperateCard = NULL;
					pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(OPERATOR_CARD_ONE))  ; 
					if(NULL != pOperateCard)
					{
						pOperateCard->SetControlVisible(true);
						pOperateCard->SetCardList(m_iUserCard[m_pGameFrame->GetMyDeskStation()] , 0x04) ; 
					}

					for(int i = 0x00;i < 0x04;i ++)
					{
						m_byOneOparateHand[i] = m_iUserCard[m_pGameFrame->GetMyDeskStation()][i];
					}

					m_iOneOparateHandCount = 0x04;
					memset(m_byTwoOparateHand,0x00,sizeof(m_byTwoOparateHand));
					m_iTwoOparateHandCount = 0x00;

					BYTE byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_iUserCard[m_pGameFrame->GetMyDeskStation()],0x04);
					if(byCardSpecialShap != 0x00)
					{
						SetShowCardSpaceCard(IMG_CARD_SHAP_LEFT_3,byCardSpecialShap,true,0x02);
						EnableControl(BNT_GIVE_UP,true);
					}
				}

			}
			if(pGameStation->byGameStation == GS_PLAY_GAME_TIME)										//��עʱ��
			{
				if(byTimeRest != 0x00 &&byTimeRest < m_iThinkTime)
				{
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD_TIME);
					if(m_iNowOutPeople == m_iMyLogDesk && !m_pGameFrame->GetIsWatching())								//��ǰ�ֵ����ѳ���
					{
						if(m_bPlayer[m_iMyLogDesk])
							SetCtrlBtn(true);   //��һ����ע
					}
				}
			}
			else if(pGameStation->byGameStation == GS_SPARE_CARD_TIME)										//���÷���ʱ��
			{
				SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_bySeparateCardTime,ID_SEPARET_CARD_TIME);
			}
			// �����û���ʾ��Ϣ
			SetNT(m_bNtPeople);
			UpdateRoomInfo();
			SetUserInfo();
			break;
		}
	case GS_WAIT_NEXT:	
		{
			GameStation_5 * pGameStation=(GameStation_5 *)pBuffer;
			//��������
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_bySeparateCardTime = pGameStation->bySeparateCardTime;
			m_i64BoBase = pGameStation->i64BoBase;					//����
			m_i64SmallBobo = pGameStation->i64SmallBobo;			//��С����
			m_i64TempBobo = m_i64SmallBobo;
			// �����û���ʾ��Ϣ
			SetUserInfo();

			UpdateRoomInfo();
			if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
			{

				ShowControl(IDD_START_BT,false);
			}
			else
			{
				ShowControl(IDD_START_BT,true);
				SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_iBeginTime,ID_BEGIN_TIME);
			}
			break;
		}
	}
}

//===========================================================================================
// ���� : ��������ʱ,������ʾ����ͼƬ
//===========================================================================================
void CNewGameClient::PlayGameSetShowAllNote()
{
	for(int i = 0x00;i < PLAY_COUNT;i ++)		
	{
		if(m_bPlayer[i])						//���˲����г���
		{
			for(int j = 0x00;j < 0x03;j++)
			{
				if(m_i64NoteMoney[i][j] == 0x00)
				{
					break;
				}
					int	iImageID;
					int iMoveID = ViewStation(i);
					if(j == 0x00)
					{
						iImageID = IMG_NOTE_PLAY_ONE_00 + iMoveID * 3;
					}
					else if(j == 0x01)
					{
						iImageID = IMG_NOTE_PLAY_TWO_00 + iMoveID * 3;
					}
					else if(j == 0x02)
					{
						iImageID = IMG_NOTE_PLAY_THR_00 + iMoveID * 3;
					}
					else
					{
						continue;
					}			
					SetShowNotePic(iImageID,m_i64NoteMoney[i][j]);
			}
		}
	}
}

//��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	switch(nSubId)
	{
	case ASS_GM_AGREE_GAME:
		{
			if(nLen!=sizeof(MSG_GR_R_UserAgree))
			{
				return 0;
			}
			MSG_GR_R_UserAgree * pUser=(MSG_GR_R_UserAgree *)buffer;
			SOrHReadyMark(pUser->bDeskStation,pUser->bAgreeGame);
			m_bUserReady[pUser->bDeskStation] = true ;  ///��Ϸ���׼��״̬
			m_bPlayer[pUser->bDeskStation] = true;
			break;
		}	
	case ASS_GM_GAME_STATION:           ///<����״̬
		{			    
			SetGameStation(buffer,nLen);
			break;
		}

	case ASS_BEGIN_UPGRADE:			
		{
			if (nLen!=sizeof(BeginUpgradeStruct)) 
			{
				return FALSE;
			}
			m_byGameStation = 0x00;
			memset(m_i64UseAllNote,0x00,sizeof(m_i64UseAllNote));
			OnGameUpgrade(buffer);
			return true;
		}
	case ASS_SEND_CARD:							//����������
		{
			if (nLen!=sizeof(SendCardStruct)) 
			{
				return FALSE;
			}
			OnHandleSendCardBegin(buffer);
			return true;
		}
	case ASS_GAME_PLAY:							//��ʼ��Ϸ
		{
			if (nLen!=sizeof(BeginPlayStruct)) 
			{
				return false;
			}
			OnHandleGameBegin(buffer);
			return true;
		}
	case ASS_NOTE:								//Ѻע
		{
			if(nLen == sizeof(NoteResult))
			{
				OnHandleBet(buffer,1);
			}
			return true;
		}
	case ASS_SALE_END:	//��Ϸ��ȫ����
		{
			if (nLen!=sizeof(GameCutStruct)) 
			{
				return FALSE;
			}
			GameCutStruct * pGameEnd=(GameCutStruct *)buffer;
			KillGameTimer(0);
			return true;
		}
	case ASS_CONTINUE_END://��Ϸ����
		{
			if (nLen!=sizeof(T_SC_GameEndStruct)) 
			{
				return FALSE;
			}
			OnHandleNormalFinish(buffer);
			return true;
		}
	case ASS_CUT_END://�û�ǿ���뿪
		{
			if (nLen!=sizeof(GameCutStruct))
			{
				return FALSE;
			}
			GameCutStruct * pGameEnd=(GameCutStruct *)buffer;
			UserInfoForGame UserInfo;
			if (m_pGameFrame->GetUserInfo(pGameEnd->bDeskStation,UserInfo))
			{
				CString str;
				str.Format(TEXT("%s ǿ���ˣ���Ϸ��ǰ������"),UserInfo.szNickName);
				m_pGameFrame->InsertNormalMessage(str.GetBuffer());
			}
			KillGameTimer(0);

			ShowControl(IDD_START_BT,true);
			SetStationParameter(GS_WAIT_AGREE);	
			InitGameUI();
			InitGameData();
			return true;
		}
	case ASS_SEND_FINISH:								//�������
		{	
			return true;
		}
	case ASS_SET_BOBO_DATA:								//�����������BoBo
		{
			Ass_SetBobo_Data(buffer,nLen);
			return true;
		}
	case ASS_SC_NOTICE_SEND_CARD:							//
		{
			Ass_Sc_Notice_Send_Card(buffer,nLen);
			return true;
		}
	case ASS_SC_SEND_ONE_CARD_FINSH:						//��һ���ƽ���
		{
			Ass_Sc_Send_One_Card_Finsh(buffer,nLen);
			return true;
		}
	case ASS_SC_NOTIC_SEPARET_CARD:							//֪ͨ���Է�����
		{
			Ass_Sc_Notic_Separe_Card(buffer,nLen);
			return true;
		}
	case ASS_SC_SEPARAT_CARD:								//���Ƴɹ���Ϣ
		{
			Ass_Sc_Separat_Card(buffer,nLen);
			return true;
		}
	case ASS_SC_SHOW_ALL_SEPARAT_CARD:						//������ҷ��Ƴɹ���
		{
			Ass_Sc_Show_All_Separat_Card(buffer,nLen);
			return true;
		}
	case ASS_EARLY_END:
		{
			return true;
		}
	case ASS_SC_USER_GIVE_UP:
		{
			Ass_Sc_UserGiveUp(buffer,nLen);
			return true;
		}
	}
	return TRUE;
}
//=============================================================================
// ���� : ����ַ����еķ���
// ���� : BYTE * buffer		[in]		���紫�������
//		  int nLen			[in]		���ݳ���	
//=============================================================================
void CNewGameClient::Ass_Sc_Show_All_Separat_Card(BYTE * buffer,int nLen)
{
	if(sizeof(T_SC_SeparatFinish) != nLen)
	{
		return ;
	}
	T_SC_SeparatFinish *SeparatFinish = (T_SC_SeparatFinish *)buffer;
	memcpy(m_iUserCard,SeparatFinish->bySeparatCard,sizeof(SeparatFinish->bySeparatCard));
	memcpy(m_iUserCardCount,SeparatFinish->byCardCount,sizeof(m_iUserCardCount));
	SetShowLeftAndRighCardAndShap(true);
	HideOrShowOperatCard(false);
	KillGameTimer(0);
	SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_iBeginTime,ID_BEGIN_TIME);
	SetPlaySound(SOUND_OPEN_CARD, false);
}

//=============================================================================
// ���� : �Ѿ���������֮��,�û������������
// ���� : BYTE * buffer		[in]		���紫�������
//		  int nLen			[in]		���ݳ���	
//=============================================================================
void CNewGameClient::Ass_Sc_UserGiveUp(BYTE * buffer,int nLen)
{
	if(sizeof(T_SC_SpecialGiveUp) != nLen)
	{
		return ;
	}
	T_SC_SpecialGiveUp *pSpecialGiveUp = (T_SC_SpecialGiveUp *)buffer;
	if(NULL == pSpecialGiveUp)
	{
		return ;
	}
	if(pSpecialGiveUp->byOperatType == TYPE_GIVE_UP)
	{
		BYTE bView = ViewStation(pSpecialGiveUp->byDesk);
		ShowGiveUpPic(m_bSendCardCount[pSpecialGiveUp->byDesk],IMP_GIVE_UP_BACK_CARD + bView,true);
		HideINoperateCard(CARD_HAND_0 + bView);
	}
	if(pSpecialGiveUp->byDesk == m_iMyLogDesk)
	{
		EnableControl(BNT_GIVE_UP,false);									//������ŤʧЧ
		ShowControl(IMG_BOTTOM_OPERATOR_CARD,false);						//������ʾ��ͼ
		HideOrShowOperatCard(false);
	}
	m_byUserOperatorType[pSpecialGiveUp->byDesk] = TYPE_GIVE_UP;
}
//=============================================================================
// ���� : ������ʾ��ҵ����ͼ����ƺ�����
//=============================================================================
void CNewGameClient::SetShowLeftAndRighCardAndShap(bool bShow)
{
	BYTE byOneOparateHand[2];
	BYTE byTwoOparateHand[2];
	for(int i = 0x00;i < PLAY_COUNT;i ++)
	{
		if(m_byUserOperatorType[i] != 0x04)
		{
			BYTE bView = ViewStation(i);
			byOneOparateHand[0] = m_iUserCard[i][0];
			byOneOparateHand[1] = m_iUserCard[i][1];

			byTwoOparateHand[0] = m_iUserCard[i][2];
			byTwoOparateHand[1] = m_iUserCard[i][3];
			if(m_iUserCardCount[i] != 0x00)
			{
				SetShowCardShap(byOneOparateHand,0x02,IMG_CARD_SHAP_LEFT_0 + bView,true);
				SetShowCardShap(byTwoOparateHand,0x02,IMG_CARD_SHAP_RIGHT_0 + bView,true);
				SetShowOneCard(byOneOparateHand,0x02,SEPARAT_CARD_LEFT_00 + bView,true);
				SetShowOneCard(byTwoOparateHand,0x02,SEPARAT_CARD_RIGHT_00 + bView,true);
			}
			SetShowOneCard(NULL,0x00,CARD_HAND_0 + bView,false);
		}
		ShowControl(IMG_OK_00 + i,false);							//���OK��ʾ
	}
}


//=============================================================================
// ���� : ������ʾһ���Ƽ�����
// ���� : BYTE byCardList[]			[in]		������
//		  int CardCount				[in]		������
//		  int iINopID				[in]		�ؼ�ID
//		  bool bShow				[in]		�Ƿ���ʾ
//=============================================================================
void CNewGameClient::SetShowOneCard(BYTE byCardList[],int CardCount,int iINopID,bool bShow)
{
	ICardBase *pCard1;
	pCard1 = dynamic_cast<ICardBase *>(m_pUI->GetIControlbyID(iINopID));
	if(NULL != pCard1)
	{			
		if(bShow)
		{
			pCard1->SetControlVisible(bShow);
			pCard1->SetCardList(byCardList,CardCount);
			pCard1->SetCardHSpace(15);
		}
		else
		{
			pCard1->SetControlVisible(bShow);
			pCard1->SetCardList(NULL,0);
		}
	}
}

//=============================================================================
//������ҷ���
//=============================================================================
void CNewGameClient::Ass_Sc_Separat_Card(BYTE * buffer,int nLen)
{
	if(sizeof(T_SC_SeparatSuccess) != nLen)
	{
		return ;
	}
	T_SC_SeparatSuccess *tSeparatSuccess = (T_SC_SeparatSuccess *)buffer;
	if(tSeparatSuccess->byDeskStation == m_iMyLogDesk)
	{
		HideOrShowOperatCard(false);
		INoMoveCard * pCard1=NULL;
		INoMoveCard * pCard2=NULL;
		pCard1 = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(SEPARAT_CARD_LEFT_03));
		pCard2 = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(SEPARAT_CARD_RIGHT_03));
		if (pCard1 != NULL && pCard2 != NULL)
		{
			pCard1->SetCardList(m_byOneOparateHand,m_iOneOparateHandCount);
			pCard1->SetControlVisible(true);
			pCard2->SetCardList(m_byTwoOparateHand,m_iTwoOparateHandCount);
			pCard2->SetControlVisible(true);
		}
		SetShowCardShap(m_byOneOparateHand,m_iOneOparateHandCount,IMG_CARD_SHAP_LEFT_3,true);
		SetShowCardShap(m_byTwoOparateHand,m_iTwoOparateHandCount,IMG_CARD_SHAP_RIGHT_3,true);
		EnableControl(BNT_GIVE_UP,false);
	}
	else
	{
		BYTE bView = ViewStation(tSeparatSuccess->byDeskStation);
		ShowControl(IMG_OK_00 + bView,true);
	}
}

//=============================================================================
// ���� : ���ػ���ʾ�ɲ����ƿؼ�
// ���� : bool bShow	true : ��ʾ false : ����
//=============================================================================
void CNewGameClient::HideOrShowOperatCard(bool bShow)
{
	ShowControl(BNT_SEPARATE_CARD, bShow);							//����ʾ���ư�Ť
	IOperateCard *pOperateCardFirst = NULL; 
	IOperateCard *pOperateCardSecond = NULL;
	pOperateCardFirst = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(OPERATOR_CARD_ONE));
	pOperateCardSecond = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(OPERATOR_CARD_TWO));
	if((NULL != pOperateCardFirst)&&(NULL != pOperateCardSecond))
	{
		pOperateCardFirst->SetControlVisible(bShow);
		pOperateCardSecond->SetControlVisible(bShow);
	}
	ShowControl(IMG_BOTTOM_OPERATOR_CARD,bShow);
	SetShowCardShap(m_byTwoOparateHand,m_iTwoOparateHandCount,IMG_CARD_SHAP_RIGHT_30,false);
}
//=============================================================================
// ���� : ����֪ͨ��ҿ��Է�����
//=============================================================================
void CNewGameClient::Ass_Sc_Notic_Separe_Card(BYTE * buffer,int nLen)
{
	if(sizeof(tNoticeSeparetCard) != nLen)
	{
		return ;
	}

	SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_bySeparateCardTime,ID_SEPARET_CARD_TIME);
	SetShowSeparetCardBntAndImageBottomCard(true);

	if(m_byUserOperatorType[m_iMyLogDesk] == TYPE_GIVE_UP)
	{
		ShowControl(IMG_BOTTOM_OPERATOR_CARD,false);						//������ʾ��ͼ
		return ;
	}
	m_bSepartorFlag = true;
	IOperateCard *pOperateCard = NULL;
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(OPERATOR_CARD_ONE))  ; 
	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(true);
		pOperateCard->SetCardList(m_iUserCard[m_pGameFrame->GetMyDeskStation()] , 0x04) ; 
	}

	for(int i = 0x00;i < 0x04;i ++)
	{
		m_byOneOparateHand[i] = m_iUserCard[m_pGameFrame->GetMyDeskStation()][i];
	}

	m_iOneOparateHandCount = 0x04;
	memset(m_byTwoOparateHand,0x00,sizeof(m_byTwoOparateHand));
	m_iTwoOparateHandCount = 0x00;

	BYTE byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_iUserCard[m_iMyLogDesk],m_bSendCardCount[m_iMyLogDesk]);
	if(byCardSpecialShap != 0x00)
	{
		EnableControl(BNT_GIVE_UP,true);
		SetShowCardSpaceCard(IMG_CARD_SHAP_LEFT_3,byCardSpecialShap,true,0x02);
	}
}

//======================================================================================
// ���� : ������ʾ���ư�Ť,��ͼ������һЩ��Ť��
// ���� : bool bFlage		[in]			��ʾ���ư�ť
//======================================================================================
void CNewGameClient::SetShowSeparetCardBntAndImageBottomCard(bool bFlage)
{
	ShowControl(BNT_SEPARATE_CARD, bFlage);							//��ʾ���ư�Ť
	ShowControl(BNT_RESET_NOTE, false);								//�������ó��밴Ť
	for(int i = 0x00;i < 0x07;i++)						
	{
		ShowControl(BNT_NOTE_10 + i, false);						//����ʾ����	
		EnableControl(BNT_NOTE_10 + i,false);
	}

	ShowControl(IMG_BOTTOM_OPERATOR_CARD,true);						//������ʾ��ͼ

	ICardBase *pCard1 = NULL;
	pCard1 = dynamic_cast<ICardBase *>(m_pUI->GetIControlbyID(CARD_HAND_3));
	if(NULL != pCard1)
	{			
		pCard1->SetControlVisible(false);
		pCard1->SetCardList(NULL,0);
		pCard1->SetCardHSpace(15);
	}

	EnableControl(BNT_FOLLOW, false);						//��
	EnableControl(BNT_SET_ALL, false);						//��
	EnableControl(BNT_BIG,false);							//��
	EnableControl(BNT_STOP,false);							//��
	EnableControl(BNT_GIVE_UP,false);						//��
}
//=============================================================================
//֪ͨ��һ���ƽ���
//=============================================================================
void CNewGameClient::Ass_Sc_Send_One_Card_Finsh(BYTE * buffer,int nLen)
{
	if(m_bSepartorFlag)
		return ;
	tSendOneCardFinsh *SendOneCard = (tSendOneCardFinsh *)buffer;
	for(int i = 0x00;i <  PLAY_COUNT;i++)
	{
		SetShowNoticPic(IMG_OPER_TYPE_00 + i, 0xff);
	}
	m_iNowOutPeople=SendOneCard->iOutDeskStation;
	memcpy(m_byUserOperatorType,SendOneCard->byUserOperatorType,sizeof(m_byUserOperatorType));
	//����ʱ�� 
	SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD_TIME);
	if ((m_pGameFrame->GetMyDeskStation()==m_iNowOutPeople) && !m_pGameFrame->GetIsWatching())
	{
		FlashWindow();
		SetCtrlBtn(true);   //��һ����ע	
	}
	SetNT(m_bNtPeople);
	m_i64MyAddNote = 0;									//�����ע����ֵ				
	SetInumPro(INUM_NOTE_VALUE, 0x00,true,false);
}
//=============================================================================
//����:����֪ͨ����
//=============================================================================
void CNewGameClient::Ass_Sc_Notice_Send_Card(BYTE * buffer,int nLen)
{
	T_Notice_SendCard *tNoticSendCard = (T_Notice_SendCard *)buffer ;
	m_iCardShowCount = tNoticSendCard->bySendCardCount;
	m_bSendCardPos = tNoticSendCard->bySendCardFirstDesk;
	PlaySendCard(m_bSendCardPos);
	m_i64FrontNote = 0x00;
	KillGameTimer(0);
	for(int i = 0x00;i < 0x07;i++)						
	{
		ShowControl(BNT_NOTE_10 + i, false);			//����ʾ����	
		EnableControl(BNT_NOTE_10 + i,false);
	}
	for(int i = 0x00;i < 0x05; i ++)
	{
		EnableControl(BNT_FOLLOW + i,false);
	}
}
//����
void CNewGameClient::OnGameUpgrade(void *buffer)
{		
	BeginUpgradeStruct * pBegin=(BeginUpgradeStruct *)buffer;
	if(NULL==pBegin)
	{
		return;
	}
	SetStationParameter(GS_SEND_CARD);
	// �޸��Թ���������Թ۽������bug
	if (m_pGameFrame->GetIsWatching())
	{
		InitGameData();
		InitGameUI();
	}	
	HideAllReadImag();
	ShowControl(IDD_ZX_SMALL_BOBO_VALUE, SW_SHOW);
	ShowControl(IDD_ZX_DI_BI_VALUE, SW_SHOW);
	SetStationParameter(GS_SEND_CARD);
	m_bNtPeople = pBegin->bNtStation;
	m_i64BoBase = pBegin->i64BoBase;				//����
	m_i64SmallBobo = pBegin->i64SmallBobo;			//��С����
	m_i64TempBobo = m_i64SmallBobo;
	::CopyMemory(m_i64Bobo,pBegin->i64BoboData,sizeof(m_i64Bobo));
	::CopyMemory(m_bPlayer,pBegin->bPlay,sizeof(m_bPlayer));
	::memset(m_byUserOperatorType,0xff,sizeof(m_byUserOperatorType));
	m_i64FrontNote = 0;
	m_i64MyAddNote = 0;
	// �����û���ʾ��Ϣ
	SetUserInfo();
	for(int i=0;i<PLAY_COUNT;i++)
	{
		UserInfoForGame UserInfo;
		if (m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			m_i64UseAllNote[i]+=m_i64BoBase;
		}
	}
	UpDateMyUserInfo(); //�����Լ����û���Ϣ	
	UpdateRoomInfo();
	//�������ʣ��Ľ��
	UpdateMyLeftMoney();
	KillGameTimer(0);
	if(m_i64Bobo[m_pGameFrame->GetMyDeskStation()] < m_i64SmallBobo)
	{
		SetShowOrHideBoboAndResetnoteBNT(true);
		UserInfoForGame UserInfo;
		if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo))
			SetNoteEnable(UserInfo.i64Money);	
	}
	else
	{
		SetShowOrHideBoboAndResetnoteBNT(false);
	}
	SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_byBoBoTime,ID_BOBO_TIME);				
}

//=============================================================================================
// ���� : ������յ�����������
// ���� : BYTE * buffer		  [in]		��������
//		  int nLen			  [in]		�������ݳ���
//=============================================================================================
void CNewGameClient::Ass_SetBobo_Data(BYTE * buffer,int nLen)
{
	if(sizeof(tSetBoboStruct) != nLen)
	{
		return ;
	}
	tSetBoboStruct *tBobodata = (tSetBoboStruct *)buffer;
	m_i64Bobo[tBobodata->byMyDesk] = tBobodata->i64BoboMoney;
	SetShowBoboValue(tBobodata->byMyDesk,tBobodata->i64BoboMoney);
	if(tBobodata->byMyDesk == m_iMyLogDesk)
	{
		SetNoteEnable(0);												//���а�ťʧЧ

		for(int i = 0x00;i < 0x07;i++)						
		{
			ShowControl(BNT_NOTE_10 + i, false);						//����ʾ����	
			EnableControl(BNT_NOTE_10 + i,false);
		}
		ShowControl(BNT_BOBO, false);									//����ʾ������Ť
		ShowControl(BNT_SEPARATE_CARD, false);							//����ʾ���ư�Ť
		ShowControl(BNT_RESET_NOTE, false);								//������ע��ť
	}
	BYTE bView = ViewStation(tBobodata->byMyDesk);
	ShowControl(IMG_OK_00 + bView,true);
}

//=====================================================================================================
// ���� : ��ʾ������׼����Ť
// ���� : BYTE bDeskStation		[in]	��λ��
//		  bool bFlag			[in]	�Ƿ���ʾ��־λ
//=====================================================================================================
void CNewGameClient::SOrHReadyMark(BYTE bDeskStation,bool bFlag)
{
	//��Ϸ���в���ʾ
	if(GetStationParameter() >= GS_SEND_CARD && GetStationParameter() <= GS_PLAY_GAME)
	{
		bFlag = false;
	}
	BYTE iViewStation = ViewStation(bDeskStation);
	ShowControl(IMG_READY_MARK_0 + iViewStation,bFlag);
}

//====================================================================================
//����
//====================================================================================
void CNewGameClient::OnHandleSendCardBegin(void *buffer)
{
	SendCardStruct * pSendCard=(SendCardStruct *)buffer;
	if(NULL==pSendCard)
	{
		return;
	}
	KillGameTimer(0);
	memcpy(m_i64NoteMoney,pSendCard->i64dNoteData,sizeof(m_i64NoteMoney));
//-----------------------------------------------------------------------------------
	for (int i=0; i<PLAY_COUNT; i++)
	{
		UserInfoForGame UserInfo;
		m_byNoteTime[i] = pSendCard->byNoteTime[i];
		if (m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			UpdateNoteInfo(i);
			PlayXianShiChouMa(i,m_i64BoBase,true);   //�����³��붯��
		}
	}
//-----------------------------------------------------------------------------------
	SetNoteEnable(0);												//���а�ťʧЧ
	for(int i = 0x00;i < 0x07;i++)						
	{
		ShowControl(BNT_NOTE_10 + i, false);						//����ʾ����	
		EnableControl(BNT_NOTE_10 + i,false);
	}
	ShowControl(BNT_BOBO, false);									//����ʾ������Ť
	ShowControl(BNT_SEPARATE_CARD, false);							//����ʾ���ư�Ť
	ShowControl(BNT_RESET_NOTE, false);								//������ע��ť
//-----------------------------------------------------------------------------------
	//�û�������
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if(pSendCard->bCardCount[i]>0)
		{
			for (int j=0; j<MAX_CARD_COUNT; j++)
			{
				m_iUserCard[i][m_iDeskCardCount[i]]=pSendCard->bCard[i][j];
				m_iDeskCard[i][m_iDeskCardCount[i]++]=pSendCard->bCard[i][j];			//�û������ϵ���
			}
		}
		//��ⷢ��״��
		memset(m_iDeskCard[i],0x00,sizeof(m_iDeskCard[i]));	
		m_i64Bobo[i] = pSendCard->i64BoboMoney[i];
		SetShowBoboValue(i,pSendCard->i64BoboMoney[i]);
		ShowControl(IMG_OK_00 + i,false);
	}
	
	m_iCardShowCount = 0x02;
	//���ŷ��ƶ���
	StartPlaySendCard();
}
//��ʼ��Ϸ
void CNewGameClient::OnHandleGameBegin(void *buffer)
{
	BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)buffer;
	if(NULL==pBeginInfo)
	{
		return;
	}	
	m_bSepartorFlag = false;
	m_iThisNoteTimes=0;//��һ����ע
	m_byGameStation = 0x02;
	//������Ϸ״̬
	SetStationParameter(GS_PLAY_GAME);
	m_iNowOutPeople=pBeginInfo->iOutDeskStation;
	m_bNtPeople = pBeginInfo->bNtPeople;   //ׯ��λ��
	memcpy(m_i64Bobo,pBeginInfo->i64dBobo,sizeof(m_i64Bobo));
	memcpy(m_byUserOperatorType,pBeginInfo->byUserOperatorType,sizeof(m_byUserOperatorType));
	//����ʱ�� 
	SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD_TIME);
	if ((m_pGameFrame->GetMyDeskStation()==m_iNowOutPeople) && !m_pGameFrame->GetIsWatching())
	{
		FlashWindow();
		SetCtrlBtn(true);   //��һ����ע	
	}
	SetNT(m_bNtPeople);

	CString lhx;
	lhx.Format("Bobo::���ƽ���ʱBoBo %I64d %I64d %I64d %I64d %I64d %I64d",m_i64Bobo[0],m_i64Bobo[1],m_i64Bobo[2],m_i64Bobo[3],m_i64Bobo[4],m_i64Bobo[5]);
	OutputDebugString(lhx);
}
//Ѻע
void CNewGameClient::OnHandleBet(void *buffer,int iStyle)
{
	if(m_bSepartorFlag)			//����Ѿ������˾Ͳ�������
		return ;
	NoteResult *pResult = (NoteResult *) buffer;
	if(NULL==pResult)
	{
		return;
	}
	
	//�û���ע������ʾ��ע����
	m_iNowOutPeople = pResult->bNextDeskStation;											//��ǰ��ע��	
	m_byNoteTime[pResult->iOutPeople] = pResult->byNoteTime;								//�ڼ��μ�ע
	if(pResult->byOperatType != TYPE_GIVE_UP && pResult->byOperatType != TYPE_PAUSE)
	{
		PlayXianShiChouMa(pResult->iOutPeople,pResult->i64NoteMoney,true);						//�����³��붯��
	}
	
	m_i64NoteMoney[pResult->iOutPeople][pResult->byNoteTime - 1] += pResult->i64NoteMoney;
	m_i64Bobo[pResult->iOutPeople] -= pResult->i64NoteMoney;								//����������
	m_i64UseAllNote[pResult->iOutPeople] += pResult->i64NoteMoney;							//������ע
	m_i64FrontNote = pResult->i64FrontNote;
	m_i64MyAddNote = pResult->i64NoteMoney;													//ǰ����ע��ֵ
	m_byUserOperatorType[pResult->iOutPeople] =  pResult->byOperatType;						//�����������
	SetUserInfo();
	OperatorPlaySound(pResult->iOutPeople, pResult->byOperatType,0x00 , false);				//������ע����
	BYTE bView = ViewStation(pResult->iOutPeople);
	SetShowNoticPic(IMG_OPER_TYPE_00 + bView, pResult->byOperatType);
	if(pResult->byOperatType == TYPE_GIVE_UP)
	{
		ShowGiveUpPic(m_bSendCardCount[m_bSendCardPos],IMP_GIVE_UP_BACK_CARD + bView,true);
		HideINoperateCard(CARD_HAND_0 + bView);
	}
	//����ʱ��
	if (m_iNowOutPeople!=-1 || m_iNowOutPeople >= PLAY_COUNT) 
	{
		SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD_TIME);
	}
	SetCtrlBtn(true);   //��һ����ע	

}
//��������
void CNewGameClient::OnHandleNormalFinish(void *buffer)
{
	T_SC_GameEndStruct * pGameEnd=(T_SC_GameEndStruct *)buffer;
	if(NULL==pGameEnd)
	{
		return;
	}
	if(m_SoundEnginePlay != NULL)
	{
		m_SoundEnginePlay->stopAllSounds();
	}
	if(m_SoundEngine != NULL)
	{
		m_SoundEngine->stopAllSounds();
	}
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());
	int curtime=atoi(stime);
	CString str;
	GameFinishNotify finishnotify;
	::memset(&finishnotify,0,sizeof(finishnotify));
	for (int i = 0; i < PLAY_COUNT; i ++) 
	{ 
		memset(finishnotify.name[i],0,sizeof(finishnotify.name[i]));//�����������ʼ��lc090122
	}
	if(pGameEnd->byFinishType == GF_NORMAL)
	{
		SetShowLeftAndRighCardAndShap(true);
	}
	else
	{
		SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_iBeginTime,ID_BEGIN_TIME);
	}
	SetInumPro(INUM_NOTE_VALUE, 0x00,false,false);
	m_pGameFrame->InsertNormalMessage(TEXT("������Ϸ�����"));
	m_pGameFrame->InsertNormalMessage(TEXT("������������"));
	
	for (int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iDeskCardCount[i] <= 0)
		{
			continue;
		}
		UserInfoForGame UserInfo;
		if (m_pGameFrame->GetUserInfo(i,UserInfo) == FALSE)
		{
			continue;
		}
		TCHAR szMessage[256];
		CString strUserID;
		GlbGetFloatString(szMessage, pGameEnd->i64NetIncome[i]);
		str.Format(TEXT("%s  �ý�� %s"), UserInfo.szNickName, szMessage);
		wsprintf(finishnotify.name[i], "%s", UserInfo.szNickName);


		m_pGameFrame->InsertNormalMessage(str.GetBuffer(MAX_PATH));

		finishnotify.i64TotalNote[i] = pGameEnd->i64TotalNote[i];				//�û����µĳ�������
		finishnotify.i64WinMoney[i] = pGameEnd->i64WinMoney[i];					//�����Ӯ�õĽ����
		finishnotify.i64NetIncome[i] = pGameEnd->i64NetIncome[i];				//������
	}
	int iMyDesk = m_pGameFrame->GetMyDeskStation();
	if (finishnotify.i64NetIncome[iMyDesk] > 0x00)								
	{
		SetPlaySound(SOUND_WIN);
	}
	else
	{
		SetPlaySound(SOUND_LOSE);
	}
	ShowResult(finishnotify);
	SetStationParameter(GS_WAIT_AGREE);
}
//������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	//������ϢFF
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		switch(pMessage->_uControlID)
		{
		case IDD_MOV_CONTROL_0: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_1: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_2: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_3: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_4: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_5: //�ƶ��Ƴ���ؼ�
			{
				FinishMoveNote(pMessage->_uControlID - IDD_MOV_CONTROL_0);		//�ƶ��������֮��Ĵ���			
			}
			break;
		case IDD_MOV_SEND_CARD:
			{
			
				ShowControl(IDD_SEND_CARD,false);
				//m_Logic.Msg("zhtlog:m_bSendCardCount[%d]=%d",m_bSendCardPos,m_bSendCardCount[m_bSendCardPos]);
				if (m_bSendCardCount[m_bSendCardPos] < 4)
				{
					m_bSendCardCount[m_bSendCardPos]++;

					if(m_bSendCardCount[m_bSendCardPos] > 0x02)
					{
						m_iDeskCard[m_bSendCardPos][m_bSendCardCount[m_bSendCardPos]-1] = m_iUserCard[m_bSendCardPos][m_bSendCardCount[m_bSendCardPos]-1];
					}
					if(m_bSendCardPos == m_iMyLogDesk)						//��������ѵ��ƾ���ʾ
					{
							SetHandCard(m_bSendCardPos,m_iUserCard[m_bSendCardPos],NULL,m_bSendCardCount[m_bSendCardPos],0);

							BYTE byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_iUserCard[m_bSendCardPos],m_bSendCardCount[m_bSendCardPos]);
							if(byCardSpecialShap != 0x00)
							{
								SetShowCardSpaceCard(IMG_CARD_SHAP_LEFT_3,byCardSpecialShap,true,0x01);
							}
					}
					else
					{
						SetHandCard(m_bSendCardPos,m_iDeskCard[m_bSendCardPos],NULL,m_bSendCardCount[m_bSendCardPos],0);
					}
					while(1)
					{
						m_bSendCardPos = GetNextNoteStation(m_bSendCardPos);
						int nDeskStation = m_pGameFrame->ViewStation2DeskStation(m_bSendCardPos);
						if(m_byUserOperatorType[m_bSendCardPos] != TYPE_GIVE_UP)
						{
							break;
						}
					};
					
					if (m_bSendCardCount[m_bSendCardPos] < m_iCardShowCount)
					{
						PlaySendCard(m_bSendCardPos);
					}
				}							
			}
			break;
		case IDD_CTR_PLAY_CLOCK_0:
		case IDD_CTR_PLAY_CLOCK_1:
		case IDD_CTR_PLAY_CLOCK_2:
		case IDD_CTR_PLAY_CLOCK_3:
		case IDD_CTR_PLAY_CLOCK_4:
		case IDD_CTR_PLAY_CLOCK_5:
			{
				switch(m_iCurrentTimeId)
				{
				case ID_BEGIN_TIME://��ʼ��ʱ��
					{
						if (GetStationParameter() == GS_WAIT_NEXT)
						{
							OnControlHitBegin();
							KillGameTimer(0);
							InitGameData();
							InitGameUI();
						}
						else if (GetStationParameter() <= GS_WAIT_AGREE)
						{

							KillGameTimer(0);
							InitGameData();
							InitGameUI();
							m_pGameFrame->CloseClient();
						}
						break;
					}
				case ID_OUT_CARD_TIME://���Ƶȴ�ʱ��
					{

						if (m_iNowOutPeople==m_pGameFrame->GetMyDeskStation())		
						{
							CString str;
							str.Format("��ע�⣬���Ѿ���ʱ��ϵͳ�Զ�Ϊ�������");
							m_pGameFrame->InsertNormalMessage(str.GetBuffer());
						}
						break;
					}
				case ID_BOBO_TIME:
					KillGameTimer(0);
					break;
				case ID_SEPARET_CARD_TIME:
					{
						KillGameTimer(0);
						break;
					}
				}
				KillGameTimer(0);
			}
		}
	}


	//����������
	if(pMessage->_uMessageType==UI_LBUTTONUP)
	{

		switch(pMessage->_uControlID)
		{	
		
		case IDD_START_BT://��ʼ��ť
			{
				ShowControl(IDD_START_BT,false);
				OnControlHitBegin();
				SetPlaySound(SOUND_READY, false);
				break;
			}
		case CARD_HAND_2:
			{
				///�����϶�����
				IOperateCard * pCard=NULL;
				GETCTRL(IOperateCard,pCard,m_pUI,CARD_HAND_2);
				if (NULL != pCard)
				{
					int iMyDesk = m_pGameFrame->GetMyDeskStation();
					m_iUserCardCount[iMyDesk] = pCard->GetCardList(m_iUserCard[iMyDesk]);
				}
				break;
			}
		case RESULT_CONTINUE://����
			{
				OnControlHitBegin();
				InitGameData();
				InitGameUI();
				SetPlaySound(SOUND_READY, false);
				break;
			}
		case RESULT_LEAVE://�뿪
			{
				InitGameData();
				InitGameUI();
				m_pGameFrame->CloseClient();
				break;
			}
		case IDD_SET_THING_SOUND:
			{
				break;
			}
		case IDD_SET_THING_OK_BTN:
			{
				IRadioButton * pSoundRadioButton = NULL;
				IRadioButton * pSoundRadioButtonBG = NULL;
				GETCTRL(IRadioButton,pSoundRadioButton,m_pUI,IDD_SET_THING_SOUND);
				GETCTRL(IRadioButton,pSoundRadioButtonBG,m_pUI,IDD_SET_BG_SOUND);
				
				if (pSoundRadioButton != NULL)
				{
					m_bSoundPlay = pSoundRadioButton->GetIsSelect();
				}
				if(pSoundRadioButtonBG != NULL)
				{
					m_bSoundPlayBG = pSoundRadioButtonBG->GetIsSelect();
				}
				ShowControl(IDD_SET_THING_DLG,false);

				if(m_bSoundPlayBG)		//��������
				{
					m_SoundEnginBg->setAllSoundsPaused(false);
				}
				else
				{
					m_SoundEnginBg->setAllSoundsPaused(true) ;
				}
				break;
			}
		case IDD_SET_THING_CANCEL_BTN:
			{
				IRadioButton * pSoundRadioButton = NULL;
				IRadioButton * pSoundRadioButtonBG = NULL;
				GETCTRL(IRadioButton,pSoundRadioButton,m_pUI,IDD_SET_THING_SOUND);
				GETCTRL(IRadioButton,pSoundRadioButtonBG,m_pUI,IDD_SET_BG_SOUND);
				if (pSoundRadioButton != NULL)
				{
					pSoundRadioButton->SetIsSelect(m_bSoundPlay);
				}
				if (pSoundRadioButtonBG != NULL)
				{
					pSoundRadioButtonBG->SetIsSelect(m_bSoundPlay);
				}
				ShowControl(IDD_SET_THING_DLG,false);
				break;
			}
		case BNT_NOTE_10:					//����10
		case BNT_NOTE_100:					//����100
		case BNT_NOTE_1000:					//����1000
		case BNT_NOTE_1W:					//����1W
		case BNT_NOTE_10W:					//����10W
		case BNT_NOTE_100W:					//����100W
		case BNT_NOTE_1000W:				//����1000W
			{
				if(m_byGameStation == 0x00)								//��ǰ����������ʱ��
				{
					SetBoBoValue(pMessage->_uControlID - BNT_NOTE_10);
				}	
				else if(m_byGameStation == 0x02)
				{
					SetNoteValue(pMessage->_uControlID - BNT_NOTE_10);
				}
				SetPlaySound(SOUND_CLICK_BUTTON);
				break;
			}
		case BNT_BOBO:
			{
				SetBoboData();					//������������
				SetPlaySound(SOUND_CLICK_BUTTON);
				break;
			}
		case BNT_RESET_NOTE:					//��������������
			{
				if(m_byGameStation == 0x00)		//���õ���������
				{
					Bnt_Reset_Bobo();
				}
				else if (m_byGameStation == 0x02)		//���õ�����ע��
				{
					Bnt_Reset_Note();					//������ע��
				}
				SetPlaySound(SOUND_CLICK_BUTTON);
				break;
			}
	 case  BNT_FOLLOW:									//��
		 { 
			Bnt_Follow();
			break;
		 }
	 case  BNT_SET_ALL:							//��
		 {
			 Bnt_Set_All();
			 break;
		 }
	 case  BNT_BIG:								//��
		 {
			 Bnt_Big();
			 break;
		 }
	 case  BNT_STOP:							//��
		 {
			 Bnt_Stop();
			 break;
		 }
	 case  BNT_GIVE_UP:							//��
		 {
			 Bnt_Give_Up();
			 break;
		 }
	 case OPERATOR_CARD_ONE:
		 {
			Operator_Card_One();
			SetPlaySound(SOUND_SEND_CARD, false);
			break;
		 }
	 case OPERATOR_CARD_TWO:
		 {
			Operator_Card_Two();
			SetPlaySound(SOUND_SEND_CARD, false);
			break;
		 }
	 case BNT_SEPARATE_CARD:
		 {
			 Separate_Card();
			 SetPlaySound(SOUND_SEPARE_CARD, false);
			 break;
		 }
	case IDD_SET_THING_BTN:
		{
				IRadioButton * pSoundRadioButton = NULL;
				IRadioButton * pSoundRadioButtonBG = NULL;
				GETCTRL(IRadioButton,pSoundRadioButton,m_pUI,IDD_SET_THING_SOUND);
				GETCTRL(IRadioButton,pSoundRadioButtonBG,m_pUI,IDD_SET_BG_SOUND);
				if (pSoundRadioButton != NULL)
				{
					pSoundRadioButton->SetIsSelect(m_bSoundPlay);
				}
				if (pSoundRadioButtonBG != NULL)
				{
					pSoundRadioButtonBG->SetIsSelect(m_bSoundPlayBG);
				}
			ShowControl(IDD_SET_THING_DLG,true);
			break;
		}
		default:
			break;
		}
	}
	else 
	{
		if(UI_MOUSEENTER == pMessage->_uMessageType)
		{
			IButton *pbut = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(pMessage->_uControlID));
			if (NULL !=  pbut)
			{
				SetPlaySound(SOUND_MOVEBUTTON);
			}
		}
	}
	return 0;
}

//==================================================================================================
// ���� : ������עֵ
//==================================================================================================
void CNewGameClient::SetNoteValue(int iNoteId)
{
	if(iNoteId < 0 || iNoteId > 7)
		return ;
	EnableControl(BNT_BIG,true);										//��Ť����ʹ��
	int iNoteValue[7] = {10,100,1000,10000,100000,1000000,10000000};
	m_i64MyAddNote += iNoteValue[iNoteId];
	SetInumPro(INUM_NOTE_VALUE, m_i64MyAddNote,true,false);
	SetNoteEnable(m_i64Bobo[m_iMyLogDesk] - m_i64MyAddNote);
}

//==================================================================================================
// ���� : �����ƶ���ָ��λ��֮��Ĵ���
// ���� : int iMoveID			[in]		�ƶ���ɵĿؼ�
//==================================================================================================
void CNewGameClient::FinishMoveNote(int iMoveID)
{
	IImage* iImage = NULL;
	int	iImageID;
	ShowControl(IMG_MOVE_NOTE_00 + iMoveID,false);						//�ƶ��ĳ���
	int nDeskStation = m_pGameFrame->ViewStation2DeskStation(iMoveID);
	if(m_byNoteTime[nDeskStation] == 0x01)
	{
		iImageID = IMG_NOTE_PLAY_ONE_00 + iMoveID * 3;
	}
	else if(m_byNoteTime[nDeskStation] == 0x02)
	{
		iImageID = IMG_NOTE_PLAY_TWO_00 + iMoveID * 3;
	}
	else if(m_byNoteTime[nDeskStation] == 0x03)
	{
		iImageID = IMG_NOTE_PLAY_THR_00 + iMoveID * 3;
	}
	else
	{
		return ;
	}			
	SetShowNotePic(iImageID,m_i64NoteMoney[nDeskStation][m_byNoteTime[nDeskStation]-1]);
}
//==================================================================================================
//���� : ����ͬʱ���ͷ�����Ϣ
//==================================================================================================
void CNewGameClient::Separate_Card()
{
	if(m_iOneOparateHandCount == 0x02 && m_iTwoOparateHandCount== 0x02)
	{
		int iCmpResult = 0x00;
		iCmpResult = m_Logic.CompareTwoCardHand(m_byOneOparateHand,m_iOneOparateHandCount,m_byTwoOparateHand,m_iTwoOparateHandCount);
		if(iCmpResult < 0x00)
		{
			BYTE byTemp = 0x00; 
			for(int i = 0x00;i < 0x02;i ++)
			{
				byTemp = m_byOneOparateHand[i];
				m_byOneOparateHand[i] = m_byTwoOparateHand[i];
				m_byTwoOparateHand[i] = byTemp;
			}
		}
		T_CS_SeparatCard tSeparatCard;
		tSeparatCard.byDeskStation = m_iMyLogDesk;
		tSeparatCard.byLeftCardShap  = m_Logic.GetCardShape(m_byOneOparateHand,m_iOneOparateHandCount);
		tSeparatCard.byRightCardShap = m_Logic.GetCardShape(m_byTwoOparateHand,m_iTwoOparateHandCount);
		tSeparatCard.bySeparatCard[0] = m_byOneOparateHand[0];
		tSeparatCard.bySeparatCard[1] = m_byOneOparateHand[1];
		tSeparatCard.bySeparatCard[2] = m_byTwoOparateHand[0];
		tSeparatCard.bySeparatCard[3] = m_byTwoOparateHand[1];
		m_pGameFrame->SendGameData(&tSeparatCard,sizeof(tSeparatCard),MDM_GM_GAME_NOTIFY,ASS_CS_SEPARAT_CARD,0);
	}
	else
	{
		CString strInfo ;
		strInfo.Format("����ʾ��������ȷ����!");
		m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
	}
}
//==================================================================================================
//���� : ����1�Ĳ���
//==================================================================================================
void CNewGameClient::Operator_Card_One()
{
	IOperateCard *pOperateCardFirst = NULL;			
	IOperateCard *pOperateCardSecond = NULL;

	BYTE byFirstUpCard[4] = {0};
	BYTE bySecondUpCard[4] = {0};
	int iFirstSelectCardCount = 0x00;
	int iSecondSelectCardCount = 0x00;

	pOperateCardFirst = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(OPERATOR_CARD_ONE));
	pOperateCardSecond = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(OPERATOR_CARD_TWO));
	if(NULL != pOperateCardFirst)
	{
		iFirstSelectCardCount = pOperateCardFirst->GetUpCardList(byFirstUpCard);
	}

	if(0x00 != iFirstSelectCardCount)
	{
		m_Logic.RemoveCard(byFirstUpCard,iFirstSelectCardCount, m_byOneOparateHand,m_iOneOparateHandCount);
		m_iOneOparateHandCount -= iFirstSelectCardCount;
	}
	if(0x00 != iFirstSelectCardCount)
	{
		pOperateCardFirst->SetControlVisible(true);
		pOperateCardFirst->SetCardList(m_byOneOparateHand , m_iOneOparateHandCount) ; 
	}
	if(0x00 != iFirstSelectCardCount)			//�����ƶ�
	{
		for(int i = 0x00;i < 0x04;i ++)				//0x04 : �ܹ���������
		{
			if(m_byTwoOparateHand[i] == 0x00)
			{
				for(int j = 0x00;j < iFirstSelectCardCount;j ++,i++)
				{
					m_byTwoOparateHand[i] = byFirstUpCard[j];
				}
				m_iTwoOparateHandCount += iFirstSelectCardCount;
				break;
			}
		}
		if(NULL != pOperateCardSecond)
		{
			pOperateCardSecond->SetControlVisible(true);
			pOperateCardSecond->SetCardList(m_byTwoOparateHand , m_iTwoOparateHandCount) ; 
		}
	}
	SetShowCardShap(m_byOneOparateHand,m_iOneOparateHandCount,IMG_CARD_SHAP_LEFT_3,true);
	SetShowCardShap(m_byTwoOparateHand,m_iTwoOparateHandCount,IMG_CARD_SHAP_RIGHT_30,true);

	BYTE byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_byOneOparateHand,m_iOneOparateHandCount);
	if(byCardSpecialShap != 0x00)
	{
		SetShowCardSpaceCard(IMG_CARD_SHAP_LEFT_3,byCardSpecialShap,true,0x02);
	}
	byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_byTwoOparateHand,m_iTwoOparateHandCount);
	if(byCardSpecialShap != 0x00)
	{
		SetShowCardSpaceCard(IMG_CARD_SHAP_RIGHT_30,byCardSpecialShap,true,0x03);
	}
}

//===========================================================================================================
//��ʾ����ͼ��
void CNewGameClient::SetShowCardShap(BYTE byCardList[],int iCardCount,int iImagID,bool bShow)				//������ʾ�ұ�����
{
	IImage * pImg = NULL;
	pImg = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImagID));
	if(iCardCount == 0x02)				//������,������ʾ
	{
		CString strTemp;
		GetCardShapePath(byCardList,iCardCount,strTemp);
		pImg->LoadPic(CA2W(strTemp));
		pImg->SetGameW(pImg->GetPicWidth());
		pImg->SetGameH(pImg->GetPicHeight());
		pImg->SetControlVisible(bShow);		
	}
	else
	{
		pImg->SetControlVisible(false);	
	}
}

//======================================================================================
// ���� : ��ʾ����ʮ��������ͼƬ
// ���� : int iImagID			[in]	Ҫ��ʾ��ͼ��ID��
//		  int iCardShap			[in]	����
//		  bool bShow			[in]	��ʾ����
//======================================================================================
void CNewGameClient::SetShowCardSpaceCard(int iImagID,int iCardShap,bool bShow,int iSelectShow)
{
	IImage * pImg = NULL;
	pImg = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImagID));
	int iX = 100;
	if(iSelectShow == 0x01)
	{
		ICardBase *pCard1 = NULL;
		pCard1 = dynamic_cast<ICardBase *>(m_pUI->GetIControlbyID(CARD_HAND_3));
		if(pCard1 != NULL)
		{
			iX = pCard1->GetGameX();
		}
	}
	else if(iSelectShow == 0x02)
	{
		ICardBase *pCard1 = NULL;
		pCard1 = dynamic_cast<ICardBase *>(m_pUI->GetIControlbyID(OPERATOR_CARD_ONE));
		if(pCard1 != NULL)
		{
			iX = pCard1->GetGameX();
		}
	}
	
	if(pImg != NULL)
	{
		if(bShow)				//������,������ʾ
		{
			CString strTemp;
			if(iCardShap == 0x02)
				strTemp = ".\\resources\\image\\CardShap\\shanghualiu.png";
			else
				strTemp = ".\\resources\\image\\CardShap\\shanhuashi.png";
			pImg->LoadPic(CA2W(strTemp));
			pImg->SetGameW(pImg->GetPicWidth());
			pImg->SetGameH(pImg->GetPicHeight());
			pImg->SetControlVisible(bShow);	
			if(iSelectShow != 0x03)
			{
				int iY_0 = pImg->GetGameY();
				pImg->SetGameXY(iX +16,iY_0);
			}
		}
		else
		{
			pImg->SetControlVisible(false);	
		}
	}
}

//==================================================================================================
//���� : ����2�Ĳ���
//==================================================================================================
void CNewGameClient::Operator_Card_Two()
{
	IOperateCard *pOperateCardFirst = NULL;			
	IOperateCard *pOperateCardSecond = NULL;

	BYTE byFirstUpCard[4] = {0};
	BYTE bySecondUpCard[4] = {0};
	int iFirstSelectCardCount = 0x00;
	int iSecondSelectCardCount = 0x00;

	pOperateCardFirst = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(OPERATOR_CARD_ONE));
	pOperateCardSecond = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(OPERATOR_CARD_TWO));
	if(NULL != pOperateCardSecond)
	{
		iSecondSelectCardCount = pOperateCardSecond->GetUpCardList(bySecondUpCard);
	}
	if(0x00 != iSecondSelectCardCount)
	{
		m_Logic.RemoveCard(bySecondUpCard,iSecondSelectCardCount, m_byTwoOparateHand,m_iTwoOparateHandCount);
		m_iTwoOparateHandCount -= iSecondSelectCardCount;
	}
	if(0x00 != iSecondSelectCardCount)
	{
		pOperateCardSecond->SetControlVisible(true);
		pOperateCardSecond->SetCardList(m_byTwoOparateHand , m_iTwoOparateHandCount) ; 
	}
	if(0x00 != iSecondSelectCardCount)			//�����ƶ�
	{
		for(int i = 0x00;i < 0x04;i ++)				//0x04 : �ܹ���������
		{
			if(m_byOneOparateHand[i] == 0x00)
			{
				for(int j = 0x00;j < iSecondSelectCardCount;j ++,i++)
				{
					m_byOneOparateHand[i] = bySecondUpCard[j];
				}
				m_iOneOparateHandCount += iSecondSelectCardCount;
				break;
			}
		}
		if(NULL != pOperateCardFirst)
		{
			pOperateCardFirst->SetControlVisible(true);
			pOperateCardFirst->SetCardList(m_byOneOparateHand , m_iOneOparateHandCount) ; 
		}
	}
	SetShowCardShap(m_byOneOparateHand,m_iOneOparateHandCount,IMG_CARD_SHAP_LEFT_3,true);
	SetShowCardShap(m_byTwoOparateHand,m_iTwoOparateHandCount,IMG_CARD_SHAP_RIGHT_30,true);

	BYTE byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_byOneOparateHand,m_iOneOparateHandCount);
	if(byCardSpecialShap != 0x00)
	{
		SetShowCardSpaceCard(IMG_CARD_SHAP_LEFT_3,byCardSpecialShap,true,0x02);
	}
	byCardSpecialShap = m_Logic.IsThreeTenOrThreThreeSixe(m_byTwoOparateHand,m_iTwoOparateHandCount);
	if(byCardSpecialShap != 0x00)
	{
		SetShowCardSpaceCard(IMG_CARD_SHAP_RIGHT_30,byCardSpecialShap,true,0x03);
	}
}
//==================================================================================================
// ���� : ������
//==================================================================================================
void CNewGameClient::Bnt_Follow()
{
	tagUserProcess tUserProcess;
	if(m_i64FrontNote < m_i64Bobo[m_iMyLogDesk])			//���Ǯ��������Ǹ�(����ֻ������)
	{
		tUserProcess.iVrebType = TYPE_FOLLOW;
		tUserProcess.iNote = m_i64FrontNote;
	}
	else
	{
		tUserProcess.iVrebType = TYPE_ALL_NOTE;
		tUserProcess.iNote = m_i64Bobo[m_iMyLogDesk];
		CString str;
		str.Format("��ʾ:��Ϊ�����������,ϵͳ�Զ�������Ϊ��");
		m_pGameFrame->InsertNormalMessage(str.GetBuffer());
	}																//����һ���һ��ֵ
	m_pGameFrame->SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//==================================================================================================
// ���� : �ò���
//==================================================================================================
void CNewGameClient::Bnt_Set_All()
{
	tagUserProcess tUserProcess;
	tUserProcess.iVrebType = TYPE_ALL_NOTE;
	tUserProcess.iNote = m_i64Bobo[m_iMyLogDesk];
	m_pGameFrame->SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//==================================================================================================
// ���� : �����
//==================================================================================================
void CNewGameClient::Bnt_Big()
{
	tagUserProcess tUserProcess;
	tUserProcess.iVrebType = TYPE_ADD;
	tUserProcess.iNote = m_i64MyAddNote;
	m_pGameFrame->SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//==================================================================================================
// ���� : �ݲ���
//==================================================================================================
void CNewGameClient::Bnt_Stop()
{
	tagUserProcess tUserProcess;
	tUserProcess.iVrebType = TYPE_PAUSE;
	m_pGameFrame->SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}

//==================================================================================================
// ���� : ������
//==================================================================================================
void CNewGameClient::Bnt_Give_Up()
{
	tagUserProcess tUserProcess;
	tUserProcess.iVrebType = TYPE_GIVE_UP;
	m_pGameFrame->SendGameData(&tUserProcess,sizeof(tUserProcess),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
}
//==================================================================================================
//����������
//==================================================================================================
void CNewGameClient::SetBoBoValue(int iNoteId)
{
	if(iNoteId < 0 || iNoteId > 7)
		return ;
	int iNoteValue[7] = {10,100,1000,10000,100000,1000000,10000000};
	m_i64TempBobo += iNoteValue[iNoteId];
	IText* pText = NULL;
	GETCTRL(IText,pText,m_pUI,IDD_PLAY_MONEY_3);

	TCHAR szMessage[MAX_PATH];
	wchar_t wszTemp[64];
	GlbGetFloatString(szMessage, m_i64TempBobo); 
	MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
	pText->SetText(wszTemp);
	pText->SetControlVisible(true);
	UserInfoForGame UserInfo;
	if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo))
	{
		SetNoteEnable(UserInfo.i64Money - m_i64TempBobo);
	}
}

//==================================================================================================
// ���� : ����������ע��
//==================================================================================================
void CNewGameClient::Bnt_Reset_Note()
{
	
	if(m_pGameFrame->GetMyDeskStation() == m_iNowOutPeople)
	{
		m_i64MyAddNote = m_i64FrontNote;
		SetInumPro(INUM_NOTE_VALUE, 0x00,true,false);
		SetNoteEnable(m_i64Bobo[m_iMyLogDesk]);
	}
}
//==================================================================================================
// ���� : ��������������
//==================================================================================================
void CNewGameClient::Bnt_Reset_Bobo()
{
	m_i64TempBobo = m_i64SmallBobo;
	IText* pText = NULL;
	GETCTRL(IText,pText,m_pUI,IDD_PLAY_MONEY_3);
	TCHAR szMessage[MAX_PATH];
	wchar_t wszTemp[64];
	GlbGetFloatString(szMessage, m_i64TempBobo); 
	MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
	pText->SetText(wszTemp);
	pText->SetControlVisible(true);

	UserInfoForGame UserInfo;
	if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo))
		SetNoteEnable(UserInfo.i64Money);
}
//===================================================================================================
//���� : ������ʾ��������
//===================================================================================================
void CNewGameClient::SetShowBoboValue(int iDesk,__int64 i64dMoney)
{
	IText* pText = NULL;
	GETCTRL(IText,pText,m_pUI,IDD_PLAY_MONEY_0 + ViewStation(iDesk));
	TCHAR szMessage[MAX_PATH];
	wchar_t wszTemp[64];
	GlbGetFloatString(szMessage, i64dMoney); 
	MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
	pText->SetText(wszTemp);
	pText->SetControlVisible(true);
}
//=================================================================================================
//���� : ������������
//=================================================================================================
void CNewGameClient::SetBoboData()
{
	UserInfoForGame UserInfo;
	if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo))
	{
		if(UserInfo.i64Money < m_i64TempBobo)
		{
			CString strInfo ;
			strInfo.Format("����ʾ������,������������������!");
			m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
			return ;
		}
	}
		tSetBoboStruct tSetBobo;
		tSetBobo.i64BoboMoney = m_i64TempBobo;
		tSetBobo.byMyDesk = m_iMyLogDesk;
		m_pGameFrame->SendGameData(&tSetBobo,sizeof(tSetBobo),MDM_GM_GAME_NOTIFY,ASS_SET_BOBO_DATA,0);
}
/// 
int CNewGameClient::ResetGameFrame(void)
{
	InitGameUI();
	return 0;
}
/// ��ҽ���
int CNewGameClient::GameUserCome()
{
	/*if (nIndex == m_pGameFrame->GetMyDeskStation())
	{
		InitGameUI();
		InitGameData();
	}*/
	m_iMyLogDesk = m_pGameFrame->GetMyDeskStation();
	SetUserInfo();

	for(int i = 0 ; i<PLAY_COUNT ; i++)
	{
		UpdateNoteInfo( i);
	}
	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	if(nIndex<0 ||nIndex >=PLAY_COUNT)
	{
		return 0 ; 
	}
	m_bUserReady[nIndex] = false ; 
	SOrHReadyMark(nIndex,false);
	SetUserInfo();
	if(nIndex == m_pGameFrame->GetMyDeskStation() && m_pGameFrame->GetIsWatching())
	{
		m_pGameFrame->CloseClient();
	}
	return 0;
}
/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	return 0;
}
// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	return true;
}
/// ��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
	case ID_TIME_CLOCK:
		{
			m_TimeClockCount--;

			if (m_TimeClockCount < 5)
			{
				SetPlaySound(SOUND_COLOR, FALSE);
			}
			if (m_TimeClockCount<=0)
			{
				m_pGameFrame->KillTimer(ID_TIME_CLOCK);
			}
			break;
		}
	}
}

/// ������Ϸ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_bGameStation = bGameStation;
	if (m_pGameFrame)
		m_pGameFrame->SetStationParameter(bGameStation);
}

/// ��ȡ��Ϸ״̬
BYTE CNewGameClient::GetStationParameter(void)
{
	return m_bGameStation;
}

void CNewGameClient::CloseAllLogo(BYTE bDesk)
{
	if (bDesk >= PLAY_COUNT)
	{
		for (int i=0; i<PLAY_COUNT; i++)
		{
			IExAnimate* pAnimal=dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(PLAYER_LOGO0+i));
			if (pAnimal)
			{
				pAnimal->SetIsLoop(false);
				pAnimal->SetControlVisible(false);

			}
		}
	}
	else
	{
		IExAnimate * pAnimal = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(PLAYER_LOGO0+ViewStation(bDesk)));
		if (pAnimal)
		{
			pAnimal->SetIsLoop(false);
			pAnimal->SetControlVisible(false);
		}
	}

}

int CNewGameClient::ViewStation(int desk)
{
	if (NULL != m_pGameFrame)
	{
		return m_pGameFrame->DeskStation2View(desk);
	}

	return 0;


}



///��ʼ��Ϸ,��ʼ����
bool CNewGameClient::OnControlHitBegin()
{
	if (m_bInLisureRoom)
	{
		MSG_GM_S_ClientInfo ClientInfo;
		ClientInfo.bEnableWatch=false;
		m_pGameFrame->SendGameData(&ClientInfo,sizeof(ClientInfo),MDM_GM_GAME_FRAME,ASS_GM_GAME_INFO,0);
	}
	else
	{
		m_pGameFrame->SendGameData(ASS_GM_AGREE_GAME,NULL,0);

	}
	
	KillGameTimer(0);

	ShowControl(RESULT_WND, false);
	
	return true;
}
//�й�ֱ�ӵ��ô˺���
BOOL CNewGameClient::UserOutCard()
{

	return true;
}
//��Ϸ�߼���������
void CNewGameClient::InitGameData()
{
	m_iCurrentTimeId = 0;       //��ǰ��ʱ��ID
	m_bSendCardPos = 255;
	m_bNtPeople = 255;

	m_iThisNoteTimes = 0;			//Ѻע����
	m_i64FrontNote     = 0;                //��ǰ��Ч��ע����ǰλ�����Ч��ע��;
	m_i64MyAddNote     = 0;                //�Ҽ�ע��ֵ 
	//������Ϣ
	::memset(m_bSendCardCount,0,sizeof(m_bSendCardCount));
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));			//��������ϵ��˿���
	::memset(m_iUserCard,0,sizeof(m_iUserCard));			//��������ϵ��˿�
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	::memset(m_iDeskCard,0,sizeof(m_iDeskCard));			//������˿�
	::memset(m_i64Bobo,0x00,sizeof(m_i64Bobo));
}
//��ϷUI����
void CNewGameClient::InitGameUI()
{

	//����������
	ICardBase *pCard1 = NULL;
	for (int i=0; i<PLAY_COUNT; i++)
	{					  
		HideINoperateCard(CARD_HAND_0+i);
		HideINoperateCard(SEPARAT_CARD_LEFT_00+i);
		HideINoperateCard(SEPARAT_CARD_RIGHT_00+i);
		ShowControl(IDD_PLAY_BG_0+i,false);
		ShowControl(IDD_CTR_PLAY_NT_0+i,false);
		ShowControl(IDD_CTR_CLOCK_BG_0+i,false);
		ShowControl(IDD_CTR_PLAY_CLOCK_0+i,false);
		ShowControl(IMG_OK_00 + i,false);
		ShowControl(IMG_ALREADY_NOET + i,false);

		ShowControl(IMG_NOTE_MOVE_START_00 + i,false);		//��ʼ����
		ShowControl(IMG_NOTE_MOVE_END_00 + i,false);		//�ŵ�����
		ShowControl(IMG_MOVE_NOTE_00 + i,false);			//�ƶ��ĳ���

		ShowControl(IMG_NOTE_PLAY_ONE_00 + i*3,false);		//��Ƥ
		ShowControl(IMG_NOTE_PLAY_TWO_00 + i*3,false);		//��һ����ע
		ShowControl(IMG_NOTE_PLAY_THR_00 + i*3,false);		//�ڶ�����ע
		ShowControl(IMP_GIVE_UP_BACK_CARD + i,false);
		HideNoteAndPic();
	}

	ShowControl(IDD_SEND_CARD,false);
	ShowControl(IMG_CARD_SHAP_RIGHT_30,false);
	//���������
	IContain* pResult=dynamic_cast<IContain *>(m_pUI->GetIControlbyID(RESULT_WND));

	if (NULL != pResult)
	{
		pResult->SetControlVisible(false);
	}

	SetUserInfo();
	IShCtrl* pShCtrl = NULL;
	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_MOV_CONTROL_0);
	if (NULL != pShCtrl)
	{
		pShCtrl->SetShNote(0);
		pShCtrl->SetControlVisible(false);
	}

	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_CENTER);
	if (NULL != pShCtrl)
	{
		pShCtrl->SetShNote(0);
		pShCtrl->SetControlVisible(false);
	}
	CloseAllLogo();
}

//=================================================================================
// ���� : ����һ�����ɲ����ƿ�ţ
// ���� : int iCardID	[in]		�ؼ�ID
//		  bool bShow
//=================================================================================
void CNewGameClient::HideINoperateCard(int iCardID)
{
	ICardBase *pCard = NULL;
	pCard = dynamic_cast<ICardBase *>(m_pUI->GetIControlbyID(iCardID));
	if(NULL != pCard)
	{			
		pCard->SetControlVisible(false);
		pCard->SetCardList(NULL,0);
		pCard->SetCardHSpace(15);
	}
}

//��������
void CNewGameClient::SetHandCard(BYTE bDesk, BYTE iCardList[], BYTE iUpCard[], int iCardCount, int iUpCardCount)
{
	BYTE bView = ViewStation(bDesk);

	INoMoveCard * pCard=NULL;
	pCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(CARD_HAND_0+bView));
	if (pCard != NULL)
	{
		pCard->SetCardList(iCardList,iCardCount);
		pCard->SetControlVisible(true);
	}

}


/// ��ʾ/���� �ؼ�
/// @param int iControlID �ؼ�ID
/// @param bool bShow  �Ƿ���ʾ
/// @return BOOL �Ƿ����óɹ�
BOOL CNewGameClient::ShowControl(int iControlID, bool bShow)
{
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		pControl->SetControlVisible(bShow);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// ���ÿ���/���� �ؼ���ֻ֧�ְ�ť��
/// @param int iControlID �ؼ�ID
/// @param bool bShow  �Ƿ���ʾ
/// @return BOOL �Ƿ����óɹ�
BOOL CNewGameClient::EnableControl(int iControlID, bool bShow)
{
	IButton *pControl = NULL;

	pControl = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		pControl->SetEnable(bShow);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/// ��ѯ��ť����/���� �ؼ���ֻ֧�ְ�ť��
/// @param int iControlID �ؼ�ID
/// @return BOOL 
BOOL CNewGameClient::GetControlEnable(int iControlID)
{
	IButton *pControl = NULL;

	pControl = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		return pControl->GetEnable();
	}
	else
	{
		return FALSE;
	}
}

/// ��ѯ��ť��ʾ/���� �ؼ���ֻ֧�ְ�ť��
/// @param int iControlID �ؼ�ID
/// @return BOOL 
BOOL CNewGameClient::GetControlShow(int iControlID)
{
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		return pControl->GetVisible();
	}
	else
	{
		return FALSE;
	}
}

/// FlashWindowEx��գ�
void CNewGameClient::FlashWindow()
{
	HWND hwnd = (m_pUI->GetWindowsHwnd());
	FLASHWINFO info;
	info.cbSize = sizeof(FLASHWINFO);
	info.dwFlags = FLASHW_TRAY|FLASHW_TIMERNOFG;
	info.hwnd = hwnd;
	info.uCount = 100;
	info.dwTimeout = 500;
	FlashWindowEx(&info);
}
/// �趨��Ϸ��ʱ��
/// @param BYTE bDesk ��ǰ��ʾ��ʱ����λ��
/// @param int TimeLen ʱ�䳤��
/// @param int TimerID ��ʱ��ID
BOOL CNewGameClient::SetGameTimer(BYTE bDesk, int TimeLen, int TimerID)
{
	BYTE bView = ViewStation(bDesk);

	//��������ָʾ
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_CTR_PLAY_CLOCK_0+i,false);
		ShowControl(IDD_CTR_CLOCK_BG_0+i,false);

	}
	
	ShowControl(IDD_CTR_CLOCK_BG_0+ViewStation(bDesk),true);
	ITime *pTime = NULL ;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(IDD_CTR_PLAY_CLOCK_0+ViewStation(bDesk)));
	m_pGameFrame->KillTimer(ID_TIME_CLOCK);
	if(NULL != pTime)
	{
		pTime->SetShowMaxNum(TimeLen);
		pTime->SetShowMinNum(0);
		pTime->SetPLay(true);
		pTime->SetControlVisible(true);
		//��������ѵĻ��������µ�ǰTIMEID���Ա�֤��ʱ����Ӧ
		if (m_pGameFrame->GetMyDeskStation() == bDesk)
		{
			m_iCurrentTimeId = TimerID;
			m_pGameFrame->SetTimer(ID_TIME_CLOCK,1000);
			m_TimeClockCount = TimeLen;
		}
		return TRUE;
	}

	return FALSE;

}

/// ������ʱ��
BOOL CNewGameClient::KillGameTimer(int TimerID)
{

	//��������ָʾ
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_CTR_PLAY_CLOCK_0+i,false);
		ShowControl(IDD_CTR_CLOCK_BG_0+i,false);
	}

	ITime *pTime = NULL ;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(IDD_CTR_PLAY_CLOCK_0));

	m_iCurrentTimeId = 0;
	m_pGameFrame->KillTimer(ID_TIME_CLOCK);
	if(NULL != pTime)
	{
		pTime->SetPLay(false);
		pTime->SetControlVisible(false);

		return TRUE;
	}
	return FALSE;
}

/// ��ʾ �����
void CNewGameClient::ShowResult(GameFinishNotify szResultInfo)
{
	//20130701 ljg ���ؿ���ͼƬ
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(RESULT_NAME+i,false);
		ShowControl(RESULT_NOTE_MONEY+i,false);
		ShowControl(RESULT_WIN_MONEY+i,false);
		ShowControl(RESULT_NET_INCOME_MONEY_US+i,false);
	}
	wchar_t wszTemp[64];
	IText* pText = NULL;
	BYTE nMeStation = m_pGameFrame->GetMyDeskStation();
	UserInfoForGame UserInfo;
	int count = 0,temp;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if(false == m_bPlayer[i])						//�����������;��������
			continue ;
		if (m_pGameFrame->GetUserInfo(i,UserInfo) )
		{
			if( m_pGameFrame->GetMyDeskStation() == i)
			{
				temp = count;
				count = 0;
			}
			else
			{
				count ++ ;
			}
			bool bMyStation = i==m_pGameFrame->GetMyDeskStation()?true:false;
			GETCTRL(IText,pText,m_pUI,RESULT_NAME + count);
			if (pText)
			{
				pText->SetFontShowStyle(AlignmentCenter);
				if (bMyStation)
				{
					pText->SetFontColor(RGB(0,255,255));
				}
				else
				{
					pText->SetFontColor(RGB(0,255,0));
				}

				wchar_t	m_wszNiCheng[256];//��Ϸ�б�����ҵ��ǳ� ,lym 2012-5-11��
				MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,m_wszNiCheng,256);
				::swprintf_s(wszTemp,L"%s ",m_wszNiCheng);
				pText->SetText(wszTemp);
				pText->SetControlVisible(true);
			}
			SetInumPro(IMGNUM_NOTE_MONEY + count, szResultInfo.i64TotalNote[i],true,false);
			SetInumPro(IMGNUM_WIN_MONEY + count, szResultInfo.i64WinMoney[i],true,false);
			SetInumPro(IMGNUM_NET_INCOME_MONEY_A + count, szResultInfo.i64NetIncome[i],true,true);
			if( m_pGameFrame->GetMyDeskStation() == i)
			{
				count = temp;
			}

		}
	}
	if (m_pGameFrame->GetIsWatching())
	{
		EnableControl(RESULT_CONTINUE,false);
	}
	else
	{
		EnableControl(RESULT_CONTINUE,true);
	}
	ShowControl(RESULT_WND,true);
}

//==========================================================================================
// ���� : ������ʾ����ͼƬ�ؼ�
// ���� : const int iNumID		[in]	�ؼ���ID���
//		  bool bFlag			[in]	�Ƿ���ʾ
//		  bool bSingle			[in]	�Ƿ��Ǵ�����
//==========================================================================================
void	CNewGameClient::SetInumPro(const int iNumID, __int64 i64Num,bool bFlag,bool bSingle)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		if(bSingle)
		{
			NEW_NUM_STRUCT NumStruct;
			NumStruct.bShowPlusSignOrMinusSign = true;
			NumStruct.bShowComma = false;
			NumStruct.iShowCommaIndex = 3;
			NumStruct.iShowZeroType = 0;
			pNum->SetNewNumStruct(NumStruct);
			pNum->SetNewNum(i64Num);
			pNum->SetControlVisible(bFlag);
		}
		else
		{
			pNum->SetNum(i64Num);
			pNum->SetControlVisible(bFlag);
		}
	}

}
//����ׯ��
void CNewGameClient::SetNT(int iNTDesk)
{
    int iViewDesk = ViewStation(iNTDesk);

	for (int i=0; i<PLAY_COUNT; i++)
	{
       if (i == iViewDesk)
	   {
		   ShowControl(IDD_CTR_PLAY_NT_0+i ,true);
	   }
	   else
	   {
		   ShowControl(IDD_CTR_PLAY_NT_0+i ,false);
	   }
	}
}



/// ���������Ϣ
void CNewGameClient::SetUserInfo(int iDesk)
{
	UserInfoForGame UserInfo;
	if (iDesk == -1)
	{
		
		for (int i=0; i<PLAY_COUNT; i++)
		{
			if (m_pGameFrame->GetUserInfo(i,UserInfo))
			{

				wchar_t wszTemp[64];
				IText* pText = NULL;
				GETCTRL(IText,pText,m_pUI,IDD_PLAY_USERID_0+ViewStation(i));
				if (pText)
				{

					wchar_t	m_wszNiCheng[256];//��Ϸ�б�����ҵ��ǳ� ,lym 2012-5-11��
					MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,m_wszNiCheng,256);
					::swprintf_s(wszTemp,L"%s ",m_wszNiCheng);
					pText->SetText(wszTemp);
					pText->SetControlVisible(true);
				}
				GETCTRL(IText,pText,m_pUI,IDD_PLAY_MONEY_0+ViewStation(i));
				if (pText)
				{
					TCHAR szMessage[MAX_PATH];
					wchar_t wszTemp[64];
					GlbGetFloatString(szMessage, m_i64Bobo[i]); 
					MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
					pText->SetText(wszTemp);
					pText->SetControlVisible(true);
				}
				ShowControl(IDD_PLAY_BG_0+ViewStation(i),true);
				ShowControl(IMG_ALREADY_NOET+ViewStation(i),true);			//��ע��ͼ
				SetTextShow(TXT_ALL_NOTE+ViewStation(i),m_i64UseAllNote[i],true);
			}
			else
			{
				ShowControl(IDD_PLAY_USERID_0+ViewStation(i),false);
				ShowControl(IDD_PLAY_MONEY_0+ViewStation(i),false);
				ShowControl(IDD_PLAY_BG_0+ViewStation(i),false);
				ShowControl(IMG_ALREADY_NOET+ViewStation(i),false);			//��ע��ͼ
				ShowControl(TXT_ALL_NOTE+ViewStation(i),false);				//����ע��
			}

		}
	}
}
//=====================================================================================
// ���� : ������ת���ַ���ʾ
// ���� : int iTextID		[in]   �ؼ�ID���
//		  __int64 iMoney	[in]   Ҫת������(Ǯ��)
//		  bool bShow		[in]   �Ƿ�Ҫ��ʾ
//=====================================================================================
void CNewGameClient::SetTextShow(int iTextID,__int64 iMoney,bool bShow)
{
	IText* pText = NULL;
	GETCTRL(IText,pText,m_pUI,iTextID);
	if (pText)
	{
			TCHAR szMessage[MAX_PATH];
			wchar_t wszTemp[64];
			GlbGetFloatString(szMessage,  iMoney); 
			MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
			pText->SetText(wszTemp);
			pText->SetControlVisible(true);
	}
}

//���÷�����Ϣ
void CNewGameClient::UpdateRoomInfo()
{
	IText* pStatic = NULL;
	GETCTRL(IText,pStatic,m_pUI,IDD_ZX_SMALL_BOBO_VALUE);					//��С����
	TCHAR szMessage[MAX_PATH];

	if (pStatic != NULL)
	{
		wchar_t wszTemp[64];
		GlbGetFloatString(szMessage, m_i64SmallBobo); 
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}
	GETCTRL(IText,pStatic,m_pUI,IDD_ZX_DI_BI_VALUE);						//��Ƥ
	if (pStatic != NULL)
	{
		wchar_t wszTemp[64];
		GlbGetFloatString(szMessage, m_i64BoBase); 
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}

}

//�����Լ�����Ϣ
void CNewGameClient::UpDateMyUserInfo()
{
	IText* pStatic = NULL;
	
	TCHAR szMessage[MAX_PATH];	

	UserInfoForGame UserInfo;

	if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo) == FALSE)

		return;

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_NAME);
	if (NULL != pStatic)
	{
		//wsprintf(szMessage,"%d",UserInfo.dwUserID);
		wchar_t wszTemp[64];

		//MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}
	
		

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_POINT);
	if (NULL != pStatic)
	{
		GlbGetFloatString(szMessage,UserInfo.i64Money);
		wchar_t wszTemp[64];

		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_LEFT_MONEY);
	if (NULL != pStatic)
	{
		GlbGetFloatString(szMessage,UserInfo.i64Money);
		wchar_t wszTemp[64];

		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}


}
//�����Լ��Ŀ������
void CNewGameClient::UpdateMyLeftMoney()
{
	IText* pStatic = NULL;

	TCHAR szMessage[MAX_PATH];	

	UserInfoForGame UserInfo;

	if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo) == FALSE)
	{
		return;
	}

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_POINT);
	if (NULL != pStatic)
	{
		GlbGetFloatString(szMessage,UserInfo.i64Money);
		wchar_t wszTemp[64];

		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}
}

//��ʾ�������ע״��
void CNewGameClient::UpdateNoteInfo(BYTE bDeskStation, int nNoteValue)
{
	if (bDeskStation >= PLAY_COUNT)
	{
		return;
	}

	BYTE bViewStation = ViewStation(bDeskStation);//(ViewStation(bDeskStation) + PLAY_COUNT) % (PLAY_COUNT + 1);
	
	IText* pStatic = NULL;

	TCHAR szMessage[MAX_PATH];	

	UserInfoForGame UserInfo;

	CString strInfo ; 
	strInfo.Format("wysoutinfo:: 111���Ӻ� %d" ,bDeskStation); 
	OutputDebugString(strInfo) ; 

	if (!m_pGameFrame->GetUserInfo(bDeskStation,UserInfo))
	{
		return;
	}
}
//���ŷ��ƶ���
void CNewGameClient::PlaySendCard(BYTE Station)
{

	INoMoveCard * pCard1=NULL;
	GETCTRL(INoMoveCard,pCard1,m_pUI,IDD_SEND_CARD);
	IMoveAction* pMoveAction = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,IDD_MOV_SEND_CARD);

	IShCtrl *pShCtrl;
	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_CENTER);
	BYTE bViewStation = ViewStation(Station);
	POINT poStart, poEnd;
	poStart.x = pShCtrl->GetGameX();
	poStart.y = pShCtrl->GetGameY();
	INoMoveCard * pCard2=NULL;
	GETCTRL(INoMoveCard,pCard2,m_pUI,CARD_HAND_0+bViewStation);
	if (pCard2 != NULL)
	{
		poEnd.x = pCard2->GetGameX();
		poEnd.y = pCard2->GetGameY();
	}
	//m_Logic.Msg("zhtlog: poEnd.x=%d poEnd.y=%d",poEnd.x,poEnd.y);
	BYTE TempCard[1] = {0};
	if(pMoveAction!=NULL && pCard1!=NULL)
	{
		pCard1->SetCardList(TempCard,1);
		pCard1->SetControlVisible(true);
		pMoveAction->SetControlingCtrlID(IDD_SEND_CARD);
		pMoveAction->SetStartP(poStart);
		pMoveAction->SetEndP(poEnd);
		pMoveAction->SetTotalMoveTime(200);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
		SetPlaySound(SOUND_SEND_CARD);
	}
}
void CNewGameClient::StartPlaySendCard()
{

	for (int i=0; i<PLAY_COUNT; i++)
	{
		SetHandCard(i,m_iDeskCard[i],NULL,0,0);
	}
	if (m_bSendCardPos == 255)
	{
		m_bSendCardPos = m_bNtPeople;
		memset(m_bSendCardCount,0,sizeof(m_bSendCardCount));
	}

	//m_Logic.Msg("zhtlog:m_bSendCardPos=%d m_bNtPeople=%d",m_bSendCardPos,m_bNtPeople);


   PlaySendCard(m_bSendCardPos);
}
int CNewGameClient::GetTotalPlayNum()
{
	UserInfoForGame UserInfo;

	int iCount = 0;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			iCount++;
		}
	}
	return iCount;

}
//--------------------------------------------------------------------------------

void CNewGameClient::SetCtrlBtn(bool bIsFirstNote)
{
	SetEnableNoteBNT();
}

// ������ĳ��ҵı��ư�ť
bool CNewGameClient::OnCompareCard(BYTE bViewStation)
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}

	// {{Add by zxd 20090905 һ��ע��ɱ����ʱ���������η�����ע��Ϣ 
	KillGameTimer();
	// Add by zxd 20090905 һ��ע��ɱ����ʱ���������η�����ע��Ϣ }}
	tagCompareCard process;
	process.iVrebType = TYPE_COMPARE_CARD;
	process.byComparedStation = m_pGameFrame->ViewStation2DeskStation(bViewStation);	// ��ͼλ��ת��Ϊ����λ��
	process.iNote = 0;
	m_pGameFrame->SendGameData(&process,sizeof(process), MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, 0);
	return true;
}
//�ռ�Ӯ�ĳ��붯��
void CNewGameClient::PlayGetWinChouMa(int nDeskStation)
{

	int iView = ViewStation(nDeskStation);
	IMoveAction* pMoveAction  = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,IDD_MOV_CENTER);
	IShCtrl* pShCtrl = NULL;
	POINT pointEnd;
	POINT pStart;
	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_CENTER);
	if (NULL != pShCtrl)
	{
		pStart.x = pShCtrl->GetGameX();
		pStart.y = pShCtrl->GetGameY();
	}
		
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(PLAYER_LOGO0+iView));

	if (NULL != pControl)
	{
		pointEnd.x = pControl->GetGameX();
		pointEnd.y = pControl->GetGameY();;
	}

	if(pMoveAction!=NULL)
	{

		pMoveAction->SetControlingCtrlID(IDD_CM_SHCTRL_CENTER);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pointEnd);
		pMoveAction->SetTotalMoveTime(500);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
		pShCtrl->SetControlVisible(true);
		/*SetPlaySound(SOUND_CHOUMAMOVE);*/

	}
}

//��ע���붯��
void CNewGameClient::PlayXianShiChouMa(int nDeskStation, __int64 iNoteMoney,bool bFlag)
{
	int iView = ViewStation(nDeskStation);
	IMoveAction* pMoveAction  = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,IDD_MOV_CONTROL_0 + iView);
	IImage* iImage = NULL;
	POINT pointEnd;

	if(m_byNoteTime[nDeskStation] == 0x01)
	{
		GETCTRL(IImage,iImage,m_pUI,IMG_NOTE_PLAY_ONE_00 + iView * 3);
	}
	else if(m_byNoteTime[nDeskStation] == 0x02)
	{
		GETCTRL(IImage,iImage,m_pUI,IMG_NOTE_PLAY_TWO_00 + iView * 3);
	}
	else if(m_byNoteTime[nDeskStation] == 0x03)
	{
		GETCTRL(IImage,iImage,m_pUI,IMG_NOTE_PLAY_THR_00 + iView * 3);
	}
	else
	{
		return ;
	}

	if (NULL != iImage)
	{
		pointEnd.x = iImage->GetGameX();
		pointEnd.y = iImage->GetGameY();
	}
	
	POINT pStart;
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(IMG_NOTE_MOVE_START_00+iView));

	if (NULL != pControl)
	{
		pStart.x = pControl->GetGameX();
		pStart.y = pControl->GetGameY();
	}
	
	GETCTRL(IImage,iImage,m_pUI,IMG_MOVE_NOTE_00+iView);
	if (NULL != iImage)
	{
		iImage->SetGameXY(pStart.x,pStart.y);
		SetShowNotePic(IMG_MOVE_NOTE_00+iView,iNoteMoney);
	}

	if(pMoveAction!=NULL)
	{
		pMoveAction->SetControlingCtrlID(IMG_MOVE_NOTE_00+iView);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pointEnd);
		pMoveAction->SetTotalMoveTime(500);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);

		/*SetPlaySound(SOUND_CHOUMAMOVE);*/
	}
}

//==================================================================================
//ȡ����һ����ң����ƽ�������Ҫ��ȡ�����ĸ���Ҵﵽ��ע����
//==================================================================================
BYTE CNewGameClient::GetNextNoteStation(BYTE bCurDeskStation)
{
	BYTE bNextStation = bCurDeskStation;

	int kkk = PLAY_COUNT - 1;   //��ʱ��
	UserInfoForGame UserInfo;
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		bNextStation = (bNextStation + kkk) % PLAY_COUNT;
		if(m_iDeskCardCount[bNextStation] == 0)
		{
			continue;
		}	
		if(m_bPlayer[bNextStation] == false)			//��λ�����˾Ͳ���
		{
			continue;
		}
		if (m_byUserOperatorType[bNextStation] != 0x04 || m_byUserOperatorType[bNextStation] == 0xff)		//������û�з�����,��ʲô��û�в���
			break;
	}
	if (bNextStation == bCurDeskStation)
	{
		return bNextStation;
	}
	return bNextStation;
}

//======================================================================================
// ���� ��������Ϸ�������ע��Ť����������
//======================================================================================
void CNewGameClient::HideNoteAndPic()
{
	for(int i = 0x00;i < 0x07;i++)						
	{
		ShowControl(BNT_NOTE_10 + i, false);			//����ʾ����	
		EnableControl(BNT_NOTE_10 + i,false);
	}
	for(int i = 0x00;i < 0x06;i ++)
	{
		ShowControl(IMG_CARD_SHAP_LEFT_0 + i, false);				//����ʾ�������ͼƬ
		ShowControl(IMG_CARD_SHAP_RIGHT_0 + i, false);				//����ʾ�ұ�����ͼƬ
		ShowControl(IMG_OPER_TYPE_00 + i, false);					//����ʾ��Ҳ�����ʾ
		ShowControl(IMG_READY_MARK_0 + i, false);					//����ʾ׼����ʾ
	}

	ShowControl(BNT_BOBO, false);									//����ʾ������Ť
	ShowControl(BNT_SEPARATE_CARD, false);							//����ʾ���ư�Ť
	ShowControl(BNT_RESET_NOTE, false);								//������ע��ť
	ShowControl(IMG_BOTTOM_OPERATOR_CARD,false);
	for(int i = 0x00;i < 0x05; i ++)
	{
		EnableControl(BNT_FOLLOW + i,false);
	}
}
//======================================================================================
//��������׼��ͼ����ʾ
//======================================================================================
void CNewGameClient::HideAllReadImag()
{
	for(int i = 0x00;i < 0x06;i ++)
	{
		ShowControl(IMG_READY_MARK_0 + i, false);					//����ʾ׼����
	}
}

//======================================================================================
// ���� : ������ʾ����������������ע��Ť
// ���� : bool bFlag true ��ʾ false ����ʾ	
//======================================================================================
void CNewGameClient::SetShowOrHideBoboAndResetnoteBNT(bool bFlag)
{
	ShowControl(BNT_BOBO, bFlag);									//����ʾ������Ť
	ShowControl(BNT_RESET_NOTE, bFlag);								//������ע��ť
}

//======================================================================================
// ���� : ������ҵ�ȫ��ʾ��ע��Ť
// ���� __int64 i64Money	[in]	����Ǯ��
//======================================================================================
void CNewGameClient::SetNoteEnable(__int64 i64Money)
{
	const bool bArray[] = {false,false,false,false,false,false,false,		//0
						   true,false,false,false,false,false,false,		//1
						   true,true,false,false,false,false,false,			//2
	                       true,true,true,false,false,false,false,			//3
						   true,true,true,true,false,false,false,			//4
						   true,true,true,true,true,false,false,			//5
						   true,true,true,true,true,true,false,				//6
						   true,true,true,true,true,true,true,				//7
							};
	int index = 0x00;
	if(i64Money < 10)
	{ 
		index = 0;
	}
	else if(i64Money < 100)
	{
		index = 7;
	}
	else if(i64Money < 1000)
	{
		index = 14;
	}
	else if(i64Money < 10000)
	{
		index = 21;
	}
	else if(i64Money < 100000)
	{
		index = 28;
	}
	else if(i64Money < 1000000)
	{
		index = 35;
	}
	else if(i64Money < 10000000)
	{
		index = 42;
	}
	else
	{
		index = 49;
	}
	for(int i = 0x00;i < 0x07;i ++)
	{
		ShowControl(BNT_NOTE_10 + i, true);				//����ʾ����	
		EnableControl(BNT_NOTE_10 + i, bArray[index++]);
	}
}

//=============================================================================================
// ���� : ���� ��,��,��,��,����Ť
//=============================================================================================
void CNewGameClient::SetEnableNoteBNT()
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
		if(TYPE_ALL_NOTE == iOperatorType)							//�����������,�ͽ���ѭ��
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
	
	if(m_iMyLogDesk == m_iNowOutPeople)
	{
		switch(iOperatorType)
		{
		case TYPE_FOLLOW:								//��
						  //��    ��   ��    ��    ��
			SetOperateNote(true,true,false,false,true);
			break;
		case TYPE_ALL_NOTE:								//��
			SetOperateNote(false,true,false,false,true);
			break;
		case TYPE_ADD:									//��
			SetOperateNote(true,true,false,false,true);
			break;
		case TYPE_PAUSE:								//��(��⵽��:����ǰ�漸��������)
			SetOperateNote(false,true,false,true,true);
			break;
		case TYPE_GIVE_UP:								//��(��⵽��:����ǰ�漸�����Ƿ�����)
			SetOperateNote(false,true,false,false,true);
			break;
		default:										//��һ�β���
			SetOperateNote(false,true,false,true,true);
			break;
		}
	}
	else
	{
		SetOperateNote(false,false,false,false,false);
	}
	if(m_iMyLogDesk == m_iNowOutPeople)
	{
		if(iOperatorType == TYPE_ALL_NOTE)
		{
			SetNoteEnable(0x00);
			ShowControl(BNT_RESET_NOTE,false);
			for(int i = 0x00;i < 0x07;i ++)
				ShowControl(BNT_NOTE_10 + i, false);				//����ʾ����	
		}
		else
		{
			SetNoteEnable(m_i64Bobo[m_iMyLogDesk]);
			ShowControl(BNT_RESET_NOTE,true);
		}

	}
	else
	{
		for(int i = 0x00;i < 0x07;i ++)							//���ò���ʾ��ע����
			ShowControl(BNT_NOTE_10 + i, false);				//����ʾ����	
		ShowControl(BNT_RESET_NOTE,false);
	}
}
	
//================================================================================================
// ���� : ������ע������ť
// ���� : bool bFollow			[in]	����Ť
//		  bool bSetAll			[in]	�ð�Ť
//		  bool bBig				[in]	��Ť
//		  bool bStop			[in]	�ݰ�Ť
//		  bool bGiveUp			[in]	�ݰ�Ť
//===============================================================================================
void CNewGameClient::SetOperateNote(bool bFollow,bool bSetAll,bool bBig,bool bStop,bool bGiveUp)
{
	EnableControl(BNT_FOLLOW, bFollow);						//��
	EnableControl(BNT_SET_ALL, bSetAll);					//��
	EnableControl(BNT_BIG,bBig);							//��
	EnableControl(BNT_STOP,bStop);							//��
	EnableControl(BNT_GIVE_UP,bGiveUp);						//��
}
//===========================================================================================
//���� : �������ͻ�ȡ������ʾ�ļ�
//���� : BYTE byCardList[]  [in]		�����Ƶ�����
//		 int iCardCount		[in]		�Ƶ�����
//		 CString &str		[out]		���ڷ��ص�ͼ������
//		return 0;			��ʾ�ɹ�
//===========================================================================================
int CNewGameClient::GetCardShapePath(BYTE byCardList[],int iCardCount,CString &str)
{
	str = ".\\resources\\image\\CardShap\\";
	int iCardShap = m_Logic.GetCardShape(byCardList,iCardCount);
	switch(iCardShap)
	{
	case ZX_DING_HUANG:						//����
		str +="tinghuang.png" ;
		break;
	case ZX_TIAN_PAI:						//����
		str +="tianpai.png" ;
		break;
	case ZX_DI_PAI:							//����
		str +="dipai.png" ;
		break;
	case ZX_REN_PAI:						//����
		str +="renpai.png" ;
		break;
	case ZX_HE_PAI:							//����
		str +="hepai.png" ;
		break;
	case ZX_MEI_SHI:						//÷ʮ	���������ʹ�С��ͬ
		str +="meishi.png" ;
		break;
	case ZX_BAN_DENG:						//���
		str +="bandeng.png" ;
		break;
	case ZX_CHANG_SANG:						//����
		str +="changshan.png" ;
		break;
	case ZX_HU_TOU:							//��ͷ	���ĸ����ʹ�С��ͬ
		str +="hutou.png" ;
		break;
	case ZX_TIAO_SHI:						//��ʮ
		str +="tiaoshi.png" ;
		break;
	case ZX_MAO_MAO:						//èè
		str +="maomao.png" ;
		break;
	case ZX_KAO_YAO:						//��ҩ
		str +="gaoyao.png" ;
		break;
	case ZX_DUI_HEI_JIU:					//�Ժھ�
		str +="duici.png" ;
		break;
	case ZX_DUI_HEI_BA:						//�Ժڰ�
		str +="duici.png" ;
		break;
	case ZX_DUI_HEI_QI:						//�Ժ���
		str +="duici.png" ;
		break;
	case ZX_DUI_HEI_WU:						//�Ժ���
		str +="duici.png" ;
		break;
	case ZX_NAI_GOU:						//�̹�----------
		str +="naigou.png" ;
		break;
	case ZX_TIAN_GANG:						//���
		str +="tiangang.png" ;
		break;
	case ZX_DI_GANG:						//�ظ�
		str +="diguang.png" ;
		break;
	case ZX_TIAN_GUAN_JIU:					//��ؾ�
		str +="tianguanjiu.png" ;
		break;
	case ZX_DI_GUAN_JIU:						//�عؾ�
		str +="diguangjiu.png" ;
		break;
	case ZX_DENG_LONG_JIU:						//������
		str +="denglongjiu.png" ;
		break;
	case ZX_HE_WU_JIU:							//�����
		str +="hewujiu.png" ;
		break;
	case ZX_BAN_WU_JIU:							//�����	���������ʹ�С��ͬ
		str +="banwujiu.png" ;
		break;
	case ZX_DING_CHANG_JIU:						//������
		str +="tingchangjiu.png" ;
		break;
	case ZX_MEI_SHI_JIU:						//÷ʮ��
		str +="meishijiu.png" ;
		break;
	case ZX_DING_MAO_JIU:						//��è��	���������ʹ�С��ͬ
		str +="tingmaojiu.png" ;
		break;
	case ZX_WU_LONG_JIU:						//������
		str +="wulongjiu.png" ;
		break;
	case ZX_TIAO_SHI_JIU:						//��ʮ��
		str +="tiaoshijiu.png" ;
		break;
	case ZX_NINE_POINT:							//�ŵ�
		str +="9p.png" ;
		break;
	case ZX_EIGHT_POINT:						//�˵�
		str +="8p.png" ;
		break;
	case ZX_SEVEN_POINT:						//�ߵ�
		str +="7p.png" ;
		break;
	case ZX_SIX_POINT:							//����
		str +="6p.png" ;
		break;
	case ZX_FIVE_POINT:							//���
		str +="5p.png" ;
		break;
	case ZX_FOUR_POINT:							//�ĵ�
		str +="4p.png" ;
		break;
	case ZX_THREE_POINT:						//����
		str +="3p.png" ;
		break;	
	case ZX_TWO_POINT:							//����
		str +="2p.png" ;
		break;	
	case ZX_ONE_POINT:							//һ��
		str +="1p.png" ;
		break;
	case ZX_ZERO_POINT:							//���	
		str +="0p.png" ;
		break;
	}
	return 0;
}

//============================================================================================
// ���� : ȡ�ó���ͼƬ·��
// ���� : __int64 i64Money			[in]		��ע�ĳ���ֵ
//		  CString &str				[in]		����ͼƬ·��������
//		  int *iNum					[in]		���ص�ǰ���ڽ�Ҽ���
//============================================================================================
void CNewGameClient::GetNotePicPath(__int64 i64Money,CString &str,int *iNum)
{
	str = ".\\resources\\image\\";
	if(i64Money >= 10 && i64Money < 100)							//10��1��
	{
		str +="01.png";

		*iNum = i64Money/10 - 1;
	}
	else if(i64Money >= 100 && i64Money < 1000)							//1�ٵ�1ǧ
	{
		str +="02.png";
		*iNum = i64Money/100 - 1;
	}
	else if(i64Money >= 1000 && i64Money < 10000)						//1ǧ��1W
	{
		str +="03.png";
		*iNum = i64Money/1000 - 1;
	}
	else if(i64Money >= 10000 && i64Money < 100000)						//1W��ʮW
	{
		str +="04.png";
		*iNum = i64Money/10000 - 1;
	}
	else if(i64Money >= 100000 && i64Money < 1000000)					//ʮW����W
	{
		str +="05.png";
		*iNum = i64Money/100000 - 1;
	}
	else if(i64Money >= 1000000 && i64Money < 10000000)					//��W��ǧW
	{
		str +="06.png";
		*iNum = i64Money/1000000 - 1;
	}
	else if(i64Money >= 10000000 && i64Money < 100000000)				//ǧW��1��
	{
		str +="07.png";
		*iNum = i64Money/10000000 - 1;
	}
	else if(i64Money >= 100000000 && i64Money < 1000000000)				//1�ڵ�ʮ��
	{
		str +="08.png";
		*iNum = i64Money/100000000 - 1;
	}
	else																//ʮ�ڵ�����
	{
		str +="09.png";
		*iNum = i64Money/1000000000 - 1;
	}
}
//============================================================================================
//���� : ���ݳ���ֵ��ʾ��ׯ��ͼƬ
//���� : const int iImagID			[in]	ͼƬ���
//		 __int64 iNoteMoney			[in]	��ע����ֵ
//============================================================================================
void CNewGameClient::SetShowNotePic(const int iImagID,__int64 iNoteMoney)
{
	IImage * pImg = NULL;
	pImg = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImagID));
	if(iNoteMoney != 0x00)				//������,������ʾ
	{
		CString strTemp;
		int iNum;
		GetNotePicPath(iNoteMoney,strTemp,&iNum);
		pImg->LoadPic(CA2W(strTemp));
		pImg->SetSrcX(iNum*55) ;
		pImg->SetSrcY(0);
		pImg->SetSrcW(55); 
		pImg->SetSrcH(92); 
		pImg->SetControlVisible(true);	
	}
	else
	{
		pImg->SetControlVisible(false);	
	}
};

//=====================================================================================
// ���� : ��ȡ��������ͼ��·��������
// ���� : int iOperatType			[in]		����������
//		  CString &str				[in]		���ص�·����
//=====================================================================================
void CNewGameClient::GetOperatePicPath(int iOperatType,CString &str)
{
	str = ".\\resources\\image\\";
	switch(iOperatType)
	{
	case TYPE_FOLLOW:					//��
		str += "gen.png";
		break;
	case TYPE_ALL_NOTE:					//��
		str += "qiao.png";
		break;
	case TYPE_ADD:						//��
		str += "da.png";
		break;
	case TYPE_PAUSE:					//��
		str += "xiu.png"; 
		break;
	case TYPE_GIVE_UP:					//��
		str += "diu.png"; 
		break;
	default :
		break;
	}
}

//=====================================================================================
// ���� : ��ȡ��������ͼ��·��������
// ���� : int iCardNum				[in]		�Ƶ�����
//		  CString &str				[in]		���ص�·����
//=====================================================================================
void CNewGameClient::GetGiveUpPicPath(int iCardNum,CString &str)
{
	str = ".\\resources\\image\\";
	switch(iCardNum)
	{
	case 0x02:							//������
		str += "GivePic0.png";
		break;
	case 0x03:							//������
		str += "GivePic1.png";
		break;
	case 0x04:							//������
		str += "GivePic2.png";
		break;
	default :
		break;
	}
}
//=====================================================================================
// ���� : ��ʾ��Ҳ�����ʾ����
// ���� : int iImageID			[in]	Ҫ��ʾͼƬ��ID���
//		  int iOperType			[in]	��Ȼ�Ĳ�������	(0xff:Ϊ����ʾ)
//=====================================================================================
void CNewGameClient::SetShowNoticPic(int iImageID,int iOperType)
{
	IImage * pImg = NULL;
	pImg = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if(iOperType != 0xff)				//������,������ʾ
	{
		CString strTemp;
		GetOperatePicPath(iOperType,strTemp);
		pImg->LoadPic(CA2W(strTemp));
		pImg->SetControlVisible(true);	
	}
	else
	{
		pImg->SetControlVisible(false);	
	}
}

//======================================================================================
// ���� : ��ʾ����ͼƬ
// ���� : int iCardNum		[in]	   �Ƶ�����
//		  int iPicID		[in]	   ͼ�Ƶ�ID��
//		  bool bShow		[in]	   �Ƿ�Ҫ��ʾ��־λ
//======================================================================================
void CNewGameClient::ShowGiveUpPic(int iCardNum,int iPicID,bool bShow)
{
	IImage * pImg = NULL;
	pImg = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iPicID));
	if(bShow)			
	{
		CString strTemp;
		GetGiveUpPicPath(iCardNum,strTemp);
		pImg->LoadPic(CA2W(strTemp));
		pImg->SetControlVisible(true);	
	}
	else
	{
		pImg->SetControlVisible(false);	
	}
}


///������������
void  CNewGameClient::OperatorPlaySound(BYTE iDeskStation,int SoundID,BYTE bCardType , bool bISLoop)
{
	if(m_SoundEngine == NULL ||  m_bSoundPlay == false)
	{
		return ;
	}

	UserInfoForGame UserInfo ; 
	bool bExist = m_pGameFrame->GetUserInfo(iDeskStation , UserInfo) ; 

	if(false == bExist)
	{
		return ; 
	}

	CString strPath = CINIFile::GetAppPath() ;  

	CString  folder ;				//�ļ���·��
	CString  operatorfilename;		//�������ļ�����
	CString	 filename;				//�ļ���
	char     szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 

	if(UserInfo.bBoy)
	{
		folder.Format("./Sound/Man/") ; 
	}
	else
	{
		folder.Format("./Sound/Woman/") ; 
	}

	switch(SoundID)
	{
	case  SOUND_FOLLOW:					              //��
		{
			operatorfilename="Follow/";
			int iRand = rand()%12;
				switch(iRand)
				{
				case 0x00:
					filename = "�����";
					break;
				case 0x01:
					filename = "��";
					break;
				case 0x02:
					filename = "����";
					break;
				case 0x03:
					filename = "����";
					break;
				case 0x04:
					filename = "����";
					break;
				case 0x05:	
					filename = "��δ����";
					break;
				case 0x06:
					filename = "Ť��";
					break;
				case 0x07:
					filename = "Ť��һ��";
					break;
				case 0x08:
					filename = "��";
					break;
				case 0x09:
					filename = "�˶�Ǯ��";
					break;
				case 0x0A:
					filename = "ѧϰһ��";
					break;
				default:
					filename = "��";
					break;
				}
			break;
		}
	case  SOUND_SET_ALL:		                      //��
		{
			operatorfilename="SetAll/";
			int iRand = rand()%13;
			switch(iRand)
			{
			case 0x00:
				filename = "�������ҵ�";
				break;
			case 0x01:
				filename = "����";
				break;
			case 0x02:
				filename = "ë��";
				break;
			case 0x03:
				filename = "ƴ��";
				break;
			case 0x04:
				filename = "��";
				break;
			case 0x05:	
				filename = "����";
				break;
			case 0x06:
				filename = "����";
				break;
			case 0x07:
				filename = "���ҵĲ���";
				break;
			case 0x08:
				filename = "˵��";
				break;
			case 0x09:
				filename = "һ������ȥ";
				break;
			case 0x0A:
				filename = "ר����ඡ����";
				break;
			case 0x0B:
				filename = "װ�̶�����";
				break;
			default:
				filename = "��";
				break;
			}
			break;
		}
	case  SOUND_BIG:		                          //��
		{
			operatorfilename="Big/";
			int iRand = rand()%9;
				switch(iRand)
				{
				case 0x00:
					filename = "�����ټӵ�";
					break;
				case 0x01:
					filename = "�ȵ�˵���";
					break;
				case 0x02:
					filename = "��˵һ��";
					break;
				case 0x03:
					filename = "���ӵ��";
					break;
				case 0x04:
					filename = "˵��";
					break;
				case 0x05:	
					filename = "̧һ�»���";
					break;
				case 0x06:
					filename = "�ϵ�";
					break;
				case 0x07:
					filename = "��˵��";
					break;
				default:
					filename = "���";
					break;
				}
			break;
		}
	case  SOUND_STOP:						          //��
		{
			operatorfilename="Stop/";
			int iRand = rand()%9;
			switch(iRand)
			{
			case 0x00:
				filename = "��Ĭ�ǽ�";
				break;
			case 0x01:
				filename = "�����";
				break;
			case 0x02:
				filename = "�Ҳ���ǻ";
				break;
			case 0x03:
				filename = "�Ҳ�˵";
				break;
			case 0x04:
				filename = "ϴ��";
				break;
			case 0x05:	
				filename = "��";
				break;
			case 0x06:
				filename = "�ݹ���";
				break;
			case 0x07:
				filename = "����";
				break;
			default:
				filename = "����";
				break;
			}
			break;
		}
	case  SOUND_GIVE_UP:						      //��
		{
			operatorfilename="GiveUp/";
			int iRand = rand()%15;
			switch(iRand)
			{
			case 0x00:
				filename = "��ˣ��";
				break;
			case 0x01:
				filename = "��Ư";
				break;
			case 0x02:
				filename = "����ж�����";
				break;
			case 0x03:
				filename = "��";
				break;
			case 0x04:
				filename = "����";
				break;
			case 0x05:	
				filename = "����";
				break;
			case 0x06:
				filename = "��Լ";
				break;
			case 0x07:
				filename = "����ȥ��";
				break;
			case 0x08:
				filename = "̫����";
				break;
			case 0x09:
				filename = "��ɡ";
				break;
			case 0x0A:
				filename = "��ˮ";
				break;
			case 0x0B:
				filename = "�Ҳ���";
				break;
			case 0x0C:
				filename = "�ҵ�С";
				break;
			case 0x0D:
				filename = "�³���";
				break;
			default:
				filename = "̫����";
				break;
			}
			break;
		}
	default:
		{
			return ;
			break;
		}
	}
	wsprintf(szFileName,"%s%s%s%s%s",strPath ,folder,operatorfilename,filename,".ogg");
	m_SoundEngine->play2D(szFileName,bISLoop);
}

///  ������Ч
void CNewGameClient::SetPlaySound(int iSoundID, BOOL bISLoop)
{
	if (m_SoundEnginePlay==NULL || m_bSoundPlay == false)
	{
		return;
	}

	CString strPath = CINIFile::GetAppPath() ;  

	CString  folder ;				//�ļ���·��
	CString	 filename;				//�ļ���
	char     szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	folder.Format("./Sound/") ; 

	switch(iSoundID)
	{
	case  SOUND_COLOR:					  		     //ʱ��
		{
			filename = "clock.ogg";
			break;
		}
	case SOUND_SEND_CARD:							//����/����
		{
			filename = "sendcard.ogg";
			break;
		}
	case SOUND_CLICK_BUTTON:
		{
			filename = "clickbutton.ogg";
			break;
		}
	case SOUND_MOVEBUTTON:
		{
			filename = "movebutton.ogg";
			break;
		}
	case  SOUND_WIN:							   //ʤ������
		{
			filename = "win.wav";
			break;
		}
	case  SOUND_LOSE:							   //ʧ������
		{
			filename = "lost.wav";
			break;
		}
	case SOUND_OPEN_CARD:						//����
		{
			filename = "opencard.wav"; 
			break;
		}
	case SOUND_SEPARE_CARD:						//����
		{
			filename = "separatcard.wav";
			break;
		}
	case SOUND_READY:							//׼������
		{
			filename = "start.ogg";
			break;
		}
	}
	wsprintf(szFileName,"%s%s%s",strPath ,folder,filename);
	m_SoundEnginePlay->play2D(szFileName,bISLoop);
}

//�����������
BOOL CNewGameClient::PlaySound(bool isLoop)
{
	if (m_SoundEnginBg==NULL || m_bSoundPlayBG == false)
	{
		return TRUE;
	}

	CString strPath = CINIFile::GetAppPath() ;  
	CString  folder ;				//�ļ���·��
	CString	 filename;				//�ļ���
	char     szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	folder.Format("./Sound/BSound/01.ogg") ; 

	wsprintf(szFileName,"%s%s",strPath,folder);
	m_SoundEnginBg->play2D(szFileName,isLoop);
	return TRUE;
}