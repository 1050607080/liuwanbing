#include "stdafx.h"
#include "NewGameClient.h"
#include "UI_ControlsDefine.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "LoveSendClassInExe.h"


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
	int nTimes = nLen % 4;	    // ����ַ������ԷֳɵĽ�����������1��

	int nSrc = 0;
	int nDes = 0;

	for (int i = 0; i < nLen; ++i)
	{
		if ((i > 0) && (i % 4 == nTimes) && bIsSpace && (str[i-1] != '-'))
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
	//����UI
	m_MyUI.SetpIUI(pUI,NAME_ID);
	m_bIsWatching = false;
	m_pGameFrame = pGameFrame;
	m_bGameStation = GS_WAIT_SETGAME;           //��Ϸ״̬
	m_iBeginTime = 0;			//��Ϸ��ʼʱ��
	m_iThinkTime = 0;			//��Ϸ˼��ʱ��
	m_bTimeOutCount = 0;
	m_bSound = TRUE;
	m_SoundEngine = NULL;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	m_SoundEngine = createIrrKlangDevice();

	InitGameData();

}

CNewGameClient::~CNewGameClient()
{

}

//���������ļ�
void CNewGameClient::LoadData(void)
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f(s + GET_SKIN_FOLDER(szTempStr) + TEXT(".bcf"));
	CString key = TEXT("Operate");
	m_bSound = f.GetKeyVal(key,TEXT("Sound"),TRUE);
	m_MyUI.SetIsSelect(CTN_1000000_CTN_1003510_RButton_1003520, m_bSound);
}
/// ��ʼ��
int CNewGameClient::Initial()
{
	// ����UI
	LoadData();
	m_MyUI.Initial();

	return 0;
}
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	m_MyUI.ShowControl(CTN_1000000_BTN_10001, false);//׼����ť����
	InitGameUI();

	return 0;
}

//��Ϸ�˳�
bool CNewGameClient::OnGameQuiting()
{

	//��ѯ״̬
	if (GetStationParameter() >= GS_SEND_CARD&&GetStationParameter() < GS_WAIT_NEXT && m_bPlayer[m_pGameFrame->GetMyDeskStation()]
	&& !m_pGameFrame->GetIsWatching() && m_bUserState[m_bMyDesk])
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


		if (iResult)
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
		m_pGameFrame->SendGameData(NULL,0,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
		memset(m_bPlayer,0,sizeof(m_bPlayer));
	}

	
	return true;
}

//�û�ǿ���뿪
void CNewGameClient::OnHandleASS_CUT_END(BYTE * buffer,int nLen)
{
	if (nLen!=sizeof(GameCutStruct)) 
		return;

	BYTE bStationPa=GetStationParameter();
	if (GS_WAIT_NEXT==bStationPa || GS_WAIT_SETGAME==bStationPa || GS_WAIT_ARGEE==bStationPa)
		return;

	GameCutStruct * pCmd=(GameCutStruct *)buffer;
	KillAllTimer();

	TCHAR skin[MAX_NAME_INFO];
	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + GET_SKIN_FOLDER(skin) + ".bcf");
	CString key = TEXT("config");
	CString sec = _T("");
	//����ǿ�
	GameFinishNotify CutNotify;

	if(pCmd->bDeskStation != m_bMyDesk && IsPlayer(pCmd->bDeskStation))
	{
		//ϵͳ��Ϣ
		CutNotify.iBasePoint = m_iBaseNote;
		CutNotify.iStyle = 0;
		//��ʾ��Ϣ
		TCHAR szMessage[100] = {0};
		UserInfoForGame UserInfo;
		if (m_pGameFrame->GetUserInfo(pCmd->bDeskStation,UserInfo))
		{
			// ���ڡ� %s ��ǿ���˳�,�������۷�(��Ϸ��)����Ϸ������"
			sec.Format("ResultTipCutEnd%d", m_pGameFrame->GetGameInfo()->uComType);
			wsprintf(szMessage,f.GetKeyVal(key, sec, ""),UserInfo.szNickName);
		}
		m_pGameFrame->InsertNormalMessage(szMessage);
		CutNotify.iBasePoint = m_iBaseNote;
		CutNotify.iStyle = 0;
		//����
		
	}

	//�����ռ�
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		ShowPlayerNote(i,false);
	}

	SetStationParameter(GS_WAIT_ARGEE);
	ShowResult(CutNotify,true);
	
	return;
}

//���׼����Ϣ
void CNewGameClient::OnHandleGM_AGREE_GAME(BYTE * buffer,int nLen)
{
	MSG_GR_R_UserAgree * pCmd = (MSG_GR_R_UserAgree *)buffer;
	if (nLen != sizeof(MSG_GR_R_UserAgree))
	{
		return;
	}

	if (1 == pCmd->bAgreeGame)
	{
		ShowAgree(pCmd->bDeskStation);
	}

	return;
}

//52��Ϸ��ʼ
void CNewGameClient::OnHandleGM_ASS_GAME_BEGIN(BYTE * buffer,int nLen)
{
	if (nLen != sizeof(GameBeginStruct))
		return;

	InitGameData();
	InitGameUI();
	ShowBegin();

	KillAllTimer();
	
	memset((void *)&m_szName, 0,sizeof(m_szName));

	//������Ϸ״̬
	SetStationParameter(GS_SEND_CARD);
	m_bTimeOutCount = 0;												// ��ʱ����
	GameBeginStruct * pCmd=(GameBeginStruct *)buffer;
	m_bIsSuperUser = pCmd->bIsSuperUser;//�Լ��Ƿ񳬼��ͻ���//lym 2011-11-28

	m_iLimitNote = pCmd->iLimitNote;									// ���ע(һ��)
	m_iBaseNote = pCmd->iBaseNote;									// ��ע

	m_byCurStyle = 0x00;
	::CopyMemory(m_bUserState,pCmd->bUserState,sizeof(pCmd->bUserState));	//�û�״̬

	UserInfoForGame UserInfo;
	//�µ�ע
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(!IsPlayer(i) || false == m_bUserState[i])
			continue;
		//modified by yjp.��Ϸ��ʼ�ĵ�ע���ݵ�ע������������
		AddTotalNote(m_iBaseNote);
		m_TempNote[i] = m_iBaseNote;
		m_iPerJuTotalNote[i] += m_iBaseNote;//(DOUBLE)m_iLimitNote*0.015;
		m_iUserStation[i] = -1;
		if (m_pGameFrame->GetUserInfo(i,UserInfo))
			lstrcpy(m_szName[i],UserInfo.szNickName);
	}

	//���������ע����
	m_Time_BASENOTE_Count = 0;
	m_pGameFrame->SetTimer(ID_TIME_BASENOTE,10);

	ShowRoomInfo();
	PlaySound(SOUND_START_DEF);

	return;
}
//53������Ϣ
void CNewGameClient::OnHandleGM_ASS_SEND_CARD(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(SendOneTurnCardStruct))
	{
		return;
	}

	SendOneTurnCardStruct  *pSendData = (SendOneTurnCardStruct *)buffer;

	for(int i = 0 ; i<PLAY_COUNT ; i++)
	{
		if(!IsPlayer(i) || false == m_bUserState[i])
		{
			m_iDeskCardCount[i] = 0 ;
		}
		else
		{

			if(m_bIsSuperUser)//�����ͻ�����ʾ���е���
			{
				if (m_iUserStation[i] != TYPE_GIVE_UP)
				{
					m_iDeskCardCount[i] = pSendData->iCardCount[i]; 
					memcpy(&m_UserCard[i],&pSendData->iCardList[i],sizeof(BYTE)*5);
					memcpy(&m_DeskCard[i],&pSendData->iCardList[i],sizeof(BYTE)*5);
				}
			}
			else
			{
				m_iDeskCardCount[i] = pSendData->iCardCount[i];
				memcpy(&m_UserCard[i],&pSendData->iCardList[i],sizeof(BYTE)*5);
				memcpy(&m_DeskCard[i],&pSendData->iCardList[i],sizeof(BYTE)*5);
			}

		}
		if(!m_bIsSuperUser)//lym 2011-11-28
		{
			m_DeskCard[i][0] = 0x00;	
		}
	}

	CString sdp;
	for(int i = 0 ;i<PLAY_COUNT; i++)
	{
		if(!IsPlayer(i) || false == m_bUserState[i])
		{
			continue;
		}
		else
		{
			m_iNeedSendCardTotle += pSendData->iCardCount[i] ;
		}
	}			

	m_iFirstSendCardCount = pSendData->iFirstSendCardCount;
	m_iSendCardTotle = 0;
	m_iSendAnimaCardPos = pSendData->iStartPos;
	ZeroMemory(m_iSendCardNum,sizeof(m_iSendCardNum));
	m_bSendCard = true;
	m_pGameFrame->SetTimer(ID_TIME_SEND_CARD,20);

	return;
}
//54�������
void CNewGameClient::OnHandleGM_ASS_SEND_FINISH(BYTE * buffer,int nLen)
{
	//ֹͣ���ƣ���ʾ����
	m_pGameFrame->KillTimer(ID_TIME_SEND_CARD);
	m_bSendCard = false;
	m_iSendAnimaCardPos = 0;//�����ò����ˣ���ʼ���Ϳ�����
	m_iSendCardTotle = m_iNeedSendCardTotle;

	m_MyUI.ShowControl(CTN_1000000_CTN_100210709_NoMoveCard_100210767, false);
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (IsPlayer(i))
		{
			m_iSendCardNum[i] = m_iFirstSendCardCount;//����ֻ��������
			BYTE icard[5] = {0};
			CopyMemory(icard,&m_DeskCard[i],sizeof(BYTE)*5);
			ShowHandCard(i, icard, m_iSendCardNum[i], true);
		}
	}

	return;
}
//55��ʼ��Ϸ
void CNewGameClient::OnHandleGM_ASS_GAME_PLAY(BYTE * buffer,int nLen)
{
	if (nLen != sizeof(BeginPlayStruct)) 
		return;
	//������Ϸ״̬
	SetStationParameter(GS_PLAY_GAME);

	BeginPlayStruct * pCmd=(BeginPlayStruct *)buffer;

	m_bTimeOutCount = 0;					//��ʱ��������
	m_iNowOutPeople = m_iFirstOutPeople = pCmd->bDeskStaton ;
	m_iNowBigNote = 0;							//��ǰ���Ѻע��
	m_iThisTurnLimit = m_iLimitNote/4; //NOTE_LIMIT+m_iBaseNote; ��һ����ע �ڶ���Ϊ �ǵ���ȫ˦��1/4֮��
	//��һ����ע
	m_iThisNoteTimes = 1;
	//����ʱ��
	if(m_iNowOutPeople != -1)
	{
		SetGameTimer(m_iNowOutPeople);
	}
	//��ʾ��������λ��
	ShowCardMaxMark(true);

	if (true == m_bUserState[m_bMyDesk])
	{
		SetNoteButText(m_iBaseNote);
	}

	bool bShow = true;
	// С��2�������
	if (m_iThisNoteTimes < m_bShowHa)
	{
		bShow = false;
	}

	// �Լ��ǵ�һ����ע����
	if (m_bMyDesk == m_iNowOutPeople)
	{
		if (m_bPK)
		{
			ShowNoteBtnEnable(0,bShow,0,1);
		}
		else
		{
			ShowNoteBtnEnable(1,bShow,1,1);
		}
	}

	return;
}
//60��ע
void CNewGameClient::OnHandleGM_ASS_NOTE(BYTE * buffer,int nLen)
{
	if (nLen != sizeof(tagUserResult)) 
		return;
	tagUserResult *pCmd = (tagUserResult *) buffer;
	//�û���ע������ʾ��ע����,�ݲ���ֵ

	KillGameTimer(m_iNowOutPeople);

	m_iNowOutPeople = pCmd->iOutPeople;		//��ǰ��ע��

	CString sdp;
	sdp.Format("sdp_ ��һ���� ��ʼ��ע��Ϣ ���ID[%d]",m_iNowOutPeople);
	OutputDebugString(sdp);

	if (pCmd->bNewTurn)
	{
		m_iFirstOutPeople = pCmd->iOutPeople;
		ShowCardMaxMark(true);//����������־
	}

	//����ʱ��
	if (m_iNowOutPeople != -1) 
	{
		SetGameTimer(m_iNowOutPeople);
	}
		//��ʱ��
//1��	��1��ϵͳΪ�����˹�ͬ�µĵף������ò�ͨ���䲻ͬ�������µ׶
//2��	��2����ע��ǵ���ȫ˦��1/4֮�䣬
//3��	��3����ע��ǵ׵�ȫ˦��1/2֮�䣬
//4��	��4����ע���ǵ׺����֮��
//5�� ��5 ����ע���ǵ׺����֮��

	// qy //����Ѻע����
	if (1 == pCmd->bNoteTimes)  // ��һ��
	{
		m_iThisTurnLimit = m_iLimitNote/4;
	}
	else if (2 == pCmd->bNoteTimes)
	{
		m_iThisTurnLimit = m_iLimitNote/2;
	}
	else
	{
		m_iThisTurnLimit = (m_iLimitNote - m_iLimitNote/2 - m_iLimitNote/4-m_iBaseNote);
	}

	//ϵͳ������ע����
	m_iThisNoteTimes = pCmd->bNoteTimes;
	//��ǰ���Ѻע
	m_iNowBigNote = pCmd->iNowBigNote;

	if (pCmd->iOutPeople == m_bMyDesk)
	{					
		BYTE style = pCmd->iNowBigNoteStyle;			//��һ��������ע����
		m_byCurStyle = style;                           // ������ע����
		BOOL show = m_iThisNoteTimes >= m_bShowHa;					//�����ֲſ�����
					
		BOOL add;

		add = (style != TYPE_SHUTTLE && m_iNowBigNote >= 0);

		BOOL note = (m_iNowBigNote == 0);					//��ע,ֻ���ڵ�һ����ע��,���߱���û�����¹�ע
		BOOL follow = (style != TYPE_SHUTTLE);	//û����������ע���0
		BOOL giveup = 1;
		if (m_bPK)
		{
			giveup= ((m_iThisNoteTimes > m_byUserFollow) && (m_byCurStyle != TYPE_SHUTTLE));
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
		if(m_byAddNum>0 && m_byAddNum!=255)
		{
			if (pCmd->m_byUserAddNum[m_bMyDesk] >= m_byAddNum)
			{
				add = FALSE;
							

			}
			//��һ�����ܼ�
			if (m_iFirstOutPeople == m_bMyDesk && !pCmd->bNewTurn)
			{
				add = FALSE;
				if (pCmd->m_byUserAddNum[m_bMyDesk]  == 0)
					show = FALSE;
			}
		}

		// С��2�������
		if (m_iThisNoteTimes < m_bShowHa)
		{
			show = FALSE;
		}

		if (m_bPK)
		{
			ShowNoteBtnEnable(0,1,0,1);
		}

		// ���������ʱ���ǿ��������
		if (TYPE_SHUTTLE == style)
		{
			show = FALSE;
			follow = TRUE;
		}
					
		ShowNoteBtnEnable(follow,show,add,giveup);
	}
	return;
}
//63��ע���
void CNewGameClient::OnHandleGM_ASS_NOTE_RESULT(BYTE * buffer,int nLen)
{
	if (nLen != sizeof(NoteResult)) 
		return;
	KillGameTimer(m_iNowOutPeople);
	UserInfoForGame UserInfo;
	NoteResult *pCmd = (NoteResult*)buffer;
	::CopyMemory(m_iPerJuTotalNote,pCmd->iPlayerTotalNote,sizeof(pCmd->iPlayerTotalNote));
	
	//����
	//����Ƿ���������ʾ����
	if(pCmd->bNoteStyle == TYPE_GIVE_UP)
	{
		PlaySound(SOUND_GIVE_UP);
		//������@ʾ
		TCHAR szMessage[100];
		if(IsPlayer(pCmd->iOutPeople))
		{
			if (m_pGameFrame->GetUserInfo(pCmd->iOutPeople,UserInfo))
			{
				wsprintf(szMessage,TEXT("[��ʾ] %s ����������Ϸ"),UserInfo.szNickName);
			}
		}

		m_pGameFrame->InsertNormalMessage(szMessage);
		m_iUserStation[pCmd->iOutPeople] = TYPE_GIVE_UP;
		ShowGiveUp(pCmd->iOutPeople);

		memset(m_DeskCard[pCmd->iOutPeople],0,m_iDeskCardCount[pCmd->iOutPeople]);
		memset(m_UserCard[pCmd->iOutPeople],0,sizeof(m_UserCard[pCmd->iOutPeople]));
		//��ʾ����������
		BYTE icard[5] = {0};
		CopyMemory(icard,&m_DeskCard[pCmd->iOutPeople],sizeof(BYTE)*5);
		ShowHandCard(pCmd->iOutPeople, icard, 0, false);
									
		if(m_bMyDesk==pCmd->iOutPeople)
		{
			ShowNoteBtnEnable();
		}
	}
	else			//������ע
	{
		if (pCmd->bNoteStyle != TYPE_SHUTTLE)
		{
			PlaySound(SOUND_NOTE);
		}
		else
		{
			PlaySound(SOUND_SHUTTLE);
		}
			
		//��Ϊ�����û���ע��,�����Ǽ�ע��,Ҳ�����Ǹ�ע,��ע
		AddTotalNote(pCmd->iCurNote);
				
		//colin090420��¼
		m_TempNote[pCmd->iOutPeople] = pCmd->iCurNote;

		//����ĳһ������ע�ܶ�
		if(m_iThisNoteTimes < m_bShowHa)
		m_iThisGameNote[pCmd->iOutPeople] += pCmd->iCurNote;
		//������ע
		m_iThisTurnNote[pCmd->iOutPeople] += pCmd->iCurNote;
		//��ע����
		if (pCmd->iCurNote > 0)
		{
			ShowNoteAnima(pCmd->iOutPeople, pCmd->iCurNote, 500);
		}
	}

	return;
}
//70�ռ�����
void CNewGameClient::OnHandleGM_ASS_COLLECT_JETON(BYTE * buffer,int nLen)
{
	if (nLen != sizeof(CollectJetonStruct)) 
		return;	

	ZeroMemory(m_iTempThisTurnNote, 0, sizeof(m_iTempThisTurnNote));
	//colin090418�������һ�ַ���ʱ����
	for (int i = 0; i< PLAY_COUNT;i++)
	{
		m_iTempThisTurnNote[i] = m_iThisTurnNote[i];
	}

	//�����ռ����붯��
	return;
}
//71�������й�
void CNewGameClient::OnHandleGM_ASS_AI_STATION(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(UseAIStation))
		return;
	UseAIStation *pCmd = (UseAIStation*)buffer;
	//������@ʾ
	TCHAR szMessage[100];
	UserInfoForGame UserInfo;

	if (!IsPlayer(pCmd->bDeskStation))
	{
		return;
	}

	if(IsPlayer(pCmd->bDeskStation) && true == m_bUserState[pCmd->bDeskStation])
	{
		if (m_pGameFrame->GetUserInfo(pCmd->bDeskStation,UserInfo))
		{
			m_iUserStation[pCmd->bDeskStation] = TYPE_GIVE_UP;
			wsprintf(szMessage,TEXT("[��ʾ] %s ��������"),UserInfo.szNickName);
			m_pGameFrame->InsertNormalMessage(szMessage);
		}
	}

	memset(m_DeskCard[pCmd->bDeskStation],0,m_iDeskCardCount[pCmd->bDeskStation]);
	m_bNetCutPlayer[pCmd->bDeskStation] = TRUE;

	if( m_iNowOutPeople == pCmd->bDeskStation)
		ReplaceUserNote(pCmd->bDeskStation);

	//�������ƺͳ���
	BYTE icard[5] = {0};
	CopyMemory(icard,&m_DeskCard[pCmd->bDeskStation],sizeof(BYTE)*5);
	ShowHandCard(pCmd->bDeskStation, icard, 0, false);

	return;
}
//81�·�һ����
void CNewGameClient::OnHandleGM_ASS_SEND_A_CARD(BYTE * buffer,int nLen)
{
	OutputDebugString("dwjlog::C_�յ���һ������Ϣ");
	if(nLen != sizeof(SendOneTurnCard))
	{
		return;
	}
	ShowCardMaxMark(false);
	::memset(m_iThisTurnNote,0,sizeof(m_iThisTurnNote));

	///���ܻ�����������������¼��㷢�Ƶ�������Ҫ��������
	m_iSendCardTotle = 0;
	m_iNeedSendCardTotle = 0;
	SendOneTurnCard *pCmd = (SendOneTurnCard *)buffer ;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if(0 !=pCmd->iTurnCard[i] && IsPlayer(i) &&	true == m_bUserState[i] && TYPE_GIVE_UP!=m_iUserStation[i])
		{
			m_iSendCardNum[i] = m_iDeskCardCount[i];
			m_iSendCardTotle += m_iDeskCardCount[i];
			m_DeskCard[i][m_iDeskCardCount[i]] = pCmd->iTurnCard[i];			//�û������ϵ���
			m_UserCard[i][m_iDeskCardCount[i]++] = pCmd->iTurnCard[i];		//�û�������
			m_iNeedSendCardTotle += m_iDeskCardCount[i];
		}
	}
	//������ʼλ��
	m_iSendAnimaCardPos = pCmd->bStartPos;
	m_bSendCard = true;
	m_pGameFrame->SetTimer(ID_TIME_SEND_CARD,20);

	CString sdp;
	sdp.Format("sdp_ ��һ���� ���ID[%d]������=%d,%d",pCmd->bStartPos,m_iDeskCardCount[pCmd->bStartPos],m_iSendCardNum[pCmd->bStartPos]);
	OutputDebugString(sdp);

	return;
}
//90��Ϸ����
void CNewGameClient::OnHandleGM_ASS_CONTINUE_END(BYTE * buffer,int nLen)
{
	GameEndStruct * pCmd = (GameEndStruct *)buffer;
	if (nLen != sizeof(GameEndStruct))
	{
		return;
	}

	BYTE bStationPa=GetStationParameter();
	if (GS_WAIT_NEXT==bStationPa || GS_WAIT_SETGAME==bStationPa || GS_WAIT_ARGEE==bStationPa)
		return;

	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());
	int curtime=atoi(stime);

	__int64 nMoneyTemp = 1;
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nMoneyTemp*=10;
	}

	TCHAR skin[MAX_NAME_INFO];
	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + GET_SKIN_FOLDER(skin) + ".bcf");
	CString key = TEXT("config");
	CString sec = _T("");

	GameFinishNotify finishnotify;
	GameEndStruct * pGameEnd = (GameEndStruct *)buffer;
	KillAllTimer();
	m_pGameFrame->InsertNormalMessage(TEXT("������Ϸ�����"));
	m_pGameFrame->InsertNormalMessage(TEXT("������������������������"));
				
	CString sdp;


	CString str;
	int count = 0;
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if (pGameEnd->bShowCardShape[i])
		{
			count++;
		}
	}
	UserInfoForGame UserInfo;
	//���÷��� 
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		if(IsPlayer(i) && true == m_bUserState[i])
		{
			__int64 nChangeMoneyTmp = pGameEnd->iChangeMoney[i]*nMoneyTemp;

			if (count <= 1 && m_bMyDesk != i)
			{
				m_UserCard[i][0] = 0;
			}

			//ˢ��
			BYTE icard[5] = {0};
			CopyMemory(icard,&m_UserCard[i],sizeof(BYTE)*5);
			ShowHandCard(i, icard, m_iDeskCardCount[i], true);
			sdp.Format("sdp_ ��һ���� ��Ϸ���� ���ID[%d]������=%d,%d",i, m_iDeskCardCount[i]);
			OutputDebugString(sdp);

			// %s : �÷� %d ��Ϸ�� %d
			sec.Format("ResultMsg%d", m_pGameFrame->GetGameInfo()->uComType);
			if (m_pGameFrame->GetUserInfo(i,UserInfo))
			{
				if (m_pGameFrame->GetGameInfo()->uComType ==1)
				{
					str.Format(f.GetKeyVal(key, sec, ""),UserInfo.szNickName,
						pGameEnd->iTurePoint[ i ]);

				}
				else
				{
					str.Format(f.GetKeyVal(key, sec, ""),UserInfo.szNickName,
						nChangeMoneyTmp);
				}
			}
		}
		else
		{
			__int64 nChangeMoneyTmp = pGameEnd->iChangeMoney[i]*nMoneyTemp;
			sec.Format("ResultMsg%d", m_pGameFrame->GetGameInfo()->uComType);
			str.Format(f.GetKeyVal(key, sec, ""), m_szName[i], nChangeMoneyTmp);
		}
		if (i == m_bMyDesk)
		{
			//����
			if (pGameEnd->iChangeMoney[i] > 0)
			{
				PlaySound(SOUND_WIN);
			}
			else
			{
				PlaySound(SOUND_LOSE);
			}
		}
		//��ҽ�����Ϣ
		m_pGameFrame->InsertNormalMessage(str.GetBuffer(MAX_PATH));

		finishnotify.iWardPoint[i] = pGameEnd->iTurePoint[i];
		finishnotify.iMoney[i] = pGameEnd->iChangeMoney[i];
		if (count > 1 || i ==  m_bMyDesk)
			finishnotify.iCardShape[i] = pGameEnd->iCardShape[i];
		else
			finishnotify.iCardShape[i] = 0;
	}

	// ��������Ƿ���ʾ���ͣ������ļ����ã�
	CBcfFile fCardShape( s + GET_SKIN_FOLDER(skin) + ".bcf");
	int iShowCardType = fCardShape.GetKeyVal(_T("config"), _T("GiveUpShowCardType"), 1);
	if (0==iShowCardType && !pGameEnd->bShowCardShape[m_bMyDesk])
		memset(finishnotify.iCardShape, 0, sizeof(finishnotify.iCardShape));

	m_pGameFrame->InsertNormalMessage(TEXT("������������������������"));
	SetStationParameter(GS_WAIT_NEXT);
	finishnotify.iBasePoint = m_iBaseNote;
	finishnotify.iStyle = 1;
        
	////////////////////////////////////colin�ճ���������� 
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		ShowPlayerNote(i,false);
	}
	
	//�����
	ShowResult(finishnotify,true);

	return;
}
//93��Ϸ��ȫ����
void CNewGameClient::OnHandleGM_ASS_SALE_END(BYTE * buffer,int nLen)
{
	if (nLen!=sizeof(GameCutStruct)) 
		return;

	BYTE bStationPa=GetStationParameter();
	if (GS_WAIT_NEXT==bStationPa || GS_WAIT_SETGAME==bStationPa || GS_WAIT_ARGEE==bStationPa)
		return;

	GameCutStruct * pCmd=(GameCutStruct *)buffer;
	//KillGameTimer(0);

	//����
	SetStationParameter(GS_WAIT_ARGEE);	
	KillAllTimer();

	return;
}
//94��Ϸ������ʤ����
void CNewGameClient::OnHandleGM_ASS_NO_WINER(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(GameAheadStruct))
		return;

	BYTE bStationPa=GetStationParameter();
	if (GS_WAIT_NEXT==bStationPa || GS_WAIT_SETGAME==bStationPa || GS_WAIT_ARGEE==bStationPa)
		return;

	KillAllTimer();
	GameAheadStruct * pCmd = (GameAheadStruct*)buffer;

	GameFinishNotify finishnotify;
	::memset(&finishnotify,0,sizeof(finishnotify));
	m_pGameFrame->InsertNormalMessage(TEXT("������Ϸ�����"));
	m_pGameFrame->InsertNormalMessage(TEXT("--------------------"));

	__int64 nMoneyTemp = 1;
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nMoneyTemp*=10;
	}

	TCHAR skin[MAX_NAME_INFO];
	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + GET_SKIN_FOLDER(skin) + ".bcf");
	CString key = TEXT("config");
	CString sec = _T("");

	CString str;
	UserInfoForGame UserInfo;
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		if(!IsPlayer(i) || false == m_bUserState[i])
			continue;

		__int64 nChangeMoneyTmp = pCmd->iChangeMoney[i]*nMoneyTemp;
		
		finishnotify.iWardPoint[i] = pCmd->iTurePoint[i];
		finishnotify.iMoney[i] = pCmd->iChangeMoney[i];
		if (m_pGameFrame->GetUserInfo(pCmd->bDeskStation,UserInfo))
		{
			// %s �÷� %d ��� %d
			sec.Format("ResultTip%d", m_pGameFrame->GetGameInfo()->uComType);
			if (m_pGameFrame->GetGameInfo()->uComType ==1)
			{
				str.Format(f.GetKeyVal(key, sec, ""),UserInfo.szNickName,
					pCmd->iTurePoint[ i ]);

			}
			else
			{
				str.Format(f.GetKeyVal(key, sec, ""),UserInfo.szNickName,
					nChangeMoneyTmp);
			}	
			m_pGameFrame->InsertNormalMessage(str.GetBuffer(MAX_PATH));
		}
	}

	////////////////////////////////////colin�ճ���������� 
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		ShowPlayerNote(i,false);
	}

	//����
	//////////////////////////////// 

	m_pGameFrame->InsertNormalMessage(TEXT("������������������������"));
	SetStationParameter(GS_WAIT_ARGEE);
	ShowResult(finishnotify,true);

	return;
}

void CNewGameClient::SetGameStat_GameStation_Base(void * pBuffer,int nLen)
{
	GameStation_Base *pCmd = (GameStation_Base *) pBuffer;
	m_bIsSuperUser   = pCmd->bIsSuperUser;    //�Լ��Ƿ񳬼��ͻ���//lym 2011-11-28
	m_bIsWatching = m_pGameFrame->GetIsWatching();
	CString sdp;
	sdp.Format("sdp_ �Լ����Թ�=%d",m_bIsWatching);
	OutputDebugString(sdp);
	//��������
	m_iThinkTime     = pCmd->iThinkTime;
	m_iBeginTime     = pCmd->iBeginTime;

	m_iRoomBasePoint = pCmd->iRoomBasePoint ;//���䱶��
	m_iRunPublish    = pCmd->iRunPublish ;	 //���ܿ۷�
	m_iLimitNote     = pCmd->iLimitNote;	 //���ע
	m_iBaseNote      = pCmd->iBaseNote;		 //��ע
	//��עѡ��				
	m_byUserFollow   = pCmd->byUserFollow ;
	m_bShowHa        = pCmd->bShowHa;
	m_byAddNum	     = pCmd->m_byAddNum;	 /**< ÿ�ּ�ע����  0ʱ�������� */   //lym 2011-12-13

	m_iThisTurnLimit = 0;
	m_iThisNoteTimes = 0;
	m_bPK            = pCmd->bPK;
	::CopyMemory(m_bUserState,pCmd->bUserState,sizeof(pCmd->bUserState));	//�û�״̬
	
	
	SetStationParameter(pCmd->bGameStation);

	ShowRoomInfo();

	return;
}
void CNewGameClient::SetGameStat_GameStation_1(void * pBuffer,int nLen)
{
	GameStation_1 *pCmd = (GameStation_1 *) pBuffer;
	return;
}
void CNewGameClient::SetGameStat_GameStation_2(void * pBuffer,int nLen)
{
	GameStation_2 *pCmd = (GameStation_2 *) pBuffer;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (pCmd->bAgree[i])
		{
			ShowAgree(i);
		}
	}

	memcpy(m_bUserState,pCmd->bUserState, sizeof(m_bUserState));



	//��ʾ��ʼ��ť 
	if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
	{
		m_MyUI.ShowControl(CTN_1000000_BTN_10001, false);//׼����ť����
	}
	else
	{
		m_pGameFrame->SetTimer(ID_BEGIN_TIME,m_iBeginTime * 1000);
		m_MyUI.ShowControl(CTN_1000000_BTN_10001, true);//׼����ť��ʾ
		m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720 + ViewStation(m_bMyDesk), true);
		m_MyUI.SetClock(CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724 + ViewStation(m_bMyDesk), m_iBeginTime, true);
	}
	return;
}

void CNewGameClient::SetGameStat_GameStation_3(void * pBuffer,int nLen)
{
	GameStation_3 *pCmd = (GameStation_3 *) pBuffer;
	if (NULL == pCmd)
	{
		return;
	}


	memcpy(m_bUserState,pCmd->bUserState, sizeof(m_bUserState));

	for (int i = 0; i < PLAY_COUNT;i++)
	{	
		//���ø���������
		//��������
		m_iDeskCardCount[i] = pCmd->iUserCardCount[i];
		if(m_bIsSuperUser && m_iDeskCardCount[i]>0)//���ǳ����ͻ��˲��ܿ���//lym 2011-11-28
		{
			m_iDeskCardCount[i] = 5;
		}
		::CopyMemory(m_UserCard[i],pCmd->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
		::CopyMemory(m_DeskCard[i],pCmd->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
		if(!m_bIsSuperUser)//���ǳ����ͻ��˲��ܿ���//lym 2011-11-28
		{
			m_DeskCard[i][0] = 0;
		}
		m_iUserStation[i] = pCmd->iUserStation[i];
		//m_PlayView.m_UserCard[desk].SetCard(m_DeskCard[desk],NULL,m_iDeskCardCount[desk]);
		//���ø�������Ѻע���
		//�µ�ע
		UserInfoForGame UserInfo;
		for(int i = 0; i < PLAY_COUNT; i ++)
		{
			if(!IsPlayer(i) || false == m_bUserState[i])
				continue;
			//modified by yjp.��Ϸ��ʼ�ĵ�ע���ݵ�ע������������
			AddTotalNote(m_iBaseNote);
			m_TempNote[i] = m_iBaseNote;
			m_iPerJuTotalNote[i] += m_iBaseNote;//(DOUBLE)m_iLimitNote*0.015;
			m_iUserStation[i] = -1;
			if (m_pGameFrame->GetUserInfo(i,UserInfo))
				lstrcpy(m_szName[i],UserInfo.szNickName);
			//��ע
			ShowPlayerNote(i,true);
			//����
			BYTE icard[5] = {0};
			CopyMemory(icard,&m_DeskCard[i],sizeof(BYTE)*5);
			ShowHandCard(i, icard, m_iDeskCardCount[i], true);
		}
	}

	return;
}

void CNewGameClient::SetGameStat_GameStation_4(void * pBuffer,int nLen)
{
	GameStation_4 *pCmd = (GameStation_4 *) pBuffer;

	//��������
	m_iNowOutPeople = pCmd->tag.iOutPeople;
	m_iFirstOutPeople = pCmd->iFirstOutPeople;
	//���浱ǰ�û���ע�ܶ�
	::CopyMemory(m_iThisGameNote,pCmd->iTotalNote,sizeof(pCmd->iTotalNote));
	//memcpy(m_bUserState,pCmd->bUserState, sizeof(m_bUserState));
	::CopyMemory(m_bUserState,pCmd->bUserState,sizeof(m_bUserState));	//�û�״̬

	for (int i = 0;i < PLAY_COUNT; i ++)
	{	
		if (!IsPlayer(i))
		{
			continue;
		}

		//���ø���������
		//��������
		m_iDeskCardCount[i] = pCmd->iUserCardCount[i];
		if(m_bIsSuperUser && m_iDeskCardCount[i]>0)//���ǳ����ͻ��˲��ܿ���//lym 2011-11-28
		{
			m_iDeskCardCount[i] = 5;
		}
		::CopyMemory(m_UserCard[i],pCmd->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
		::CopyMemory(m_DeskCard[i],pCmd->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
		if(!m_bIsSuperUser)//���ǳ����ͻ��˲��ܿ���//lym 2011-11-28
		{
			m_DeskCard[i][0] = 0;
		}
		if (m_iUserStation[i] == TYPE_GIVE_UP)
		{
			memset(m_DeskCard[i],0,sizeof(m_DeskCard[i]));
			ShowGiveUp(i);
		}

		//���ø�������Ѻע���
		m_iThisTurnNote[i] = pCmd->iThisTurnNote[i];
		m_iPerJuTotalNote[i] = pCmd->iTotalNote[i];
		AddTotalNote(m_iPerJuTotalNote[i]);
		if (pCmd->iTotalNote[i] > 0)
		{
			ShowPlayerNote(i,true);
		}

		m_bNetCutPlayer[i] = pCmd->ibNetCutPlayer[i];
		if (m_bNetCutPlayer[i] == TRUE)
		{
			m_iDeskCardCount[i]=0;
			memset(m_DeskCard[i],0,sizeof(BYTE)*5);
		}

		BYTE icard[5] = {0};
		CopyMemory(icard,&m_DeskCard[i],sizeof(BYTE)*5);
		ShowHandCard(i, icard, m_iDeskCardCount[i], true);

		m_iUserStation[i] = pCmd->iUserStation[i];
		m_iSendCardTotle+=m_iDeskCardCount[i];
	}
          

	m_iNeedSendCardTotle=m_iSendCardTotle ;

	m_iThisNoteTimes = pCmd->tag.bNoteTimes;

	m_iNowBigNote = pCmd->tag.iNowBigNote;

	//��ʾ��������λ��
	ShowCardMaxMark(true);

	SetNoteButText(m_iBaseNote);
	//������Ѻע���
	ShowTotalNote(true);

	//����ʱ��
	if (m_iNowOutPeople != -1) 
		SetGameTimer(m_iNowOutPeople);

//////////////////////

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

	if (m_iNowOutPeople == m_bMyDesk)
	{					
		BYTE style = pCmd->tag.iNowBigNoteStyle;			//��һ��������ע����
		m_byCurStyle = style;                           // ������ע����
		BOOL show = m_iThisNoteTimes >= m_bShowHa;					//�����ֲſ�����

		BOOL add;

		add = (style != TYPE_SHUTTLE && m_iNowBigNote >= 0);

		BOOL note = (m_iNowBigNote == 0);					//��ע,ֻ���ڵ�һ����ע��,���߱���û�����¹�ע
		BOOL follow = (style != TYPE_SHUTTLE);	//û����������ע���0
		BOOL giveup = 1;
		if (m_bPK)
		{
			giveup= ((m_iThisNoteTimes > m_byUserFollow) && (m_byCurStyle != TYPE_SHUTTLE));
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
		if(m_byAddNum>0 && m_byAddNum!=255)
		{
			if (pCmd->m_byUserAddNum[m_bMyDesk] >= m_byAddNum)
			{
				add = FALSE;


			}
			//��һ�����ܼ�
			if (m_iFirstOutPeople == m_bMyDesk && !pCmd->tag.bNewTurn)
			{
				add = FALSE;
				if (pCmd->m_byUserAddNum[m_bMyDesk]  == 0)
					show = FALSE;
			}
		}

		// С��2�������
		if (m_iThisNoteTimes < m_bShowHa)
		{
			show = FALSE;
		}

		if (m_bPK)
		{
			ShowNoteBtnEnable(0,1,0,1);
		}

		// ���������ʱ���ǿ��������
		if (TYPE_SHUTTLE == style)
		{
			show = FALSE;
			follow = TRUE;
		}

		ShowNoteBtnEnable(follow,show,add,giveup);
	}


	////////////////////////////////
	return;
}

void CNewGameClient::SetGameStat_GameStation_5(void * pBuffer,int nLen)
{
	GameStation_5 *pCmd = (GameStation_5 *) pBuffer;


	if (NULL == pCmd)
	{
		return;
	}

	memcpy(m_bUserState,pCmd->bUserState, sizeof(m_bUserState));
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (pCmd->bAgree[i])
		{
			ShowAgree(i);
		}
	}

	//��ʾ��ʼ��ť 
	if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
	{
		m_MyUI.ShowControl(CTN_1000000_BTN_10001, false);//׼����ť����
	}
	else
	{
		m_pGameFrame->SetTimer(ID_BEGIN_TIME,m_iBeginTime * 1000);
		m_MyUI.ShowControl(CTN_1000000_BTN_10001, true);//׼����ť��ʾ
		m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720 + ViewStation(m_bMyDesk), true);
		m_MyUI.SetClock(CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724 + ViewStation(m_bMyDesk), m_iBeginTime, true);
	}

	return;
}



///������Ϸ��ͬ״̬ʱ���������ݣ���Ҫ���ڶ�������
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	InitGameUI();
	InitGameData();
	
	m_bMyDesk = m_pGameFrame->GetMyDeskStation();

	if(m_SoundEngine)
	{
		m_SoundEngine->setAllSoundsPaused(true);
		m_SoundEngine->stopAllSounds();
	}
	if (m_bSound)
	{
		PlaySceneSound();
	}

	SetGameStat_GameStation_Base(pBuffer, nLen);
	CString sdp;
	sdp.Format("sdp_ �ͻ��� SetGameStation %d", GetStationParameter());
	OutputDebugString(sdp);

	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
		{
			SetGameStat_GameStation_2(pBuffer, nLen);

			break;
		}
	case GS_SEND_CARD:
		{
			SetGameStat_GameStation_3(pBuffer, nLen);
			break;
		}
	case GS_PLAY_GAME:
		{
			SetGameStat_GameStation_4(pBuffer, nLen);
			break;
		}
	case GS_WAIT_NEXT:
		{
			SetGameStat_GameStation_5(pBuffer, nLen);
			break;
		}
	}
}

/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	CString sdp;
	sdp.Format("sdp_ ��Ϸ��Ϣ �ͻ��� ��Ϣ[ID=%d]", nSubId);
	OutputDebugString(sdp);

	/*
	���ڿͻ���������������˵���ϢID
	case ASS_UG_USER_SET://50�û�������Ϸ
	case ASS_VREB_CHECK://61�û�����
	case ASS_REPLACE_VREB_CHECK://62����(����)�û�����
	case ASS_HAVE_THING://100
	case ASS_LEFT_RESULT://101
	case ASS_STOP_THING://103
	case ASS_AGREE_STOP://104
	case ASS_BEGIN_FLASH://106 
	*/
	switch(nSubId)
	{
	case ASS_CUT_END://�û�ǿ���뿪
		{
			OnHandleASS_CUT_END(buffer, nLen);
			break;
		}
	case ASS_GM_AGREE_GAME:
		{
			OnHandleGM_AGREE_GAME(buffer, nLen);
			break;
		}
	case ASS_GM_GAME_STATION:           ///<����״̬
		{			    
			SetGameStation(buffer,nLen);
			break;
		}
	case ASS_GAME_BEGIN://52��Ϸ��ʼ
		{
			OnHandleGM_ASS_GAME_BEGIN(buffer, nLen);
		}
		break;
	case ASS_SEND_CARD://53������Ϣ
		{
			OnHandleGM_ASS_SEND_CARD(buffer, nLen);
		}
		break;
	case ASS_SEND_FINISH://54�������
		{
			OnHandleGM_ASS_SEND_FINISH(buffer, nLen);
		}
		break;
	case ASS_GAME_PLAY://55��ʼ��Ϸ
		{
			OnHandleGM_ASS_GAME_PLAY(buffer, nLen);
		}
		break;
	case ASS_NOTE://60��ע
		{
			OnHandleGM_ASS_NOTE(buffer, nLen);
		}
		break;
	case ASS_NOTE_RESULT://63��ע���
		{
			OnHandleGM_ASS_NOTE_RESULT(buffer, nLen);
		}
		break;
	case ASS_COLLECT_JETON://70�ռ�����
		{
			OnHandleGM_ASS_COLLECT_JETON(buffer, nLen);
		}
		break;
	case ASS_AI_STATION://71����
		{
			OnHandleGM_ASS_AI_STATION(buffer, nLen);
		}
		break;
	case ASS_SEND_A_CARD://81�·�һ����
		{
			OnHandleGM_ASS_SEND_A_CARD(buffer, nLen);
		}
		break;
	case ASS_CONTINUE_END://90��Ϸ����
	case ASS_NO_CONTINUE_END:
		{
			OnHandleGM_ASS_CONTINUE_END(buffer, nLen);
		}
		break;
	case ASS_SALE_END://93��Ϸ��ȫ����
		{
			OnHandleGM_ASS_SALE_END(buffer, nLen);
		}
		break;
	case ASS_NO_WINER://94��Ϸ������ʤ����
	case ASS_AHEAD_END://95��ǰ����
		{
			OnHandleGM_ASS_NO_WINER(buffer, nLen);
		}
		break;
	default:
		{
			break;
		}
	}

	return TRUE;
}
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	CString sdp;
	sdp.Format("sdp_ ����� %d,%d",pMessage->_uControlID,CTN_1000000_CTN_100210709_NoMoveCard_100210712);
	OutputDebugString(sdp);
	if (pMessage->_uControlID == CTN_1000000_CTN_100210735_CTN_1002201 && pMessage->_uMessageType == UI_LBUTTONUP)
	{
		if (m_bSendCard)
		{
			return 0;
		}
		if (GetStationParameter() == GS_WAIT_BACK || GetStationParameter() == GS_PLAY_GAME)
		{
			m_pGameFrame->KillTimer(ID_TIME_SEE_CARD);
			BYTE icard[5] = {0};
			CopyMemory(icard,&m_UserCard[m_bMyDesk],sizeof(BYTE)*5);
			ShowHandCard(m_bMyDesk, icard, m_iDeskCardCount[m_bMyDesk], true);
			m_pGameFrame->SetTimer(ID_TIME_SEE_CARD, 2000);
		}
	}

	switch(pMessage->_uMessageType)
	{
	case UI_CATOONFINISH://<�����������
		{
			OnUIMess_CATOONFINISH(pMessage->_uControlID);
		}
		break;
	case UI_LBUTTONUP://����������
		{
			if (m_bIsWatching)
			{
				return 0;
			}
			OnUIMess_LBUTTONUP(pMessage->_uControlID);
		}
		break;
	case UI_MOUSEENTER://�������/�Ƴ��ؼ�
		{
			OnUIMess_MOUSEENTER(pMessage->_uControlID);
		}
		break;
	default:
		break;
	}

	return 0;
}
//�������������Ϣ
void CNewGameClient::OnUIMess_CATOONFINISH(UINT32 uControlID)
{
	if (uControlID == CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724
		|| uControlID == CTN_1000000_CTN_100210718_CTN_100210721_Time_100210725
		|| uControlID == CTN_1000000_CTN_100210718_CTN_100210722_Time_100210726
		|| uControlID == CTN_1000000_CTN_100210718_CTN_100210723_Time_100210727)
	{
		CString sdp;
		sdp.Format("sdp_ ��ʱ������ %d", uControlID);
		OutputDebugString(sdp);
	}

	switch(uControlID)
	{
	case CTN_1000000_CTN_100210709_MOVE_100210766:
		{
			m_MyUI.ShowControl(CTN_1000000_CTN_100210709_NoMoveCard_100210767, false);
			int ix = 0;
			int iy = 0;
			m_MyUI.GetControlGameXY(CTN_1000000_CTN_100210709_NoMoveCard_100210767, ix, iy);

			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				int _ix = 0;
				int _iy = 0;

				m_MyUI.GetControlGameXY(CTN_1000000_CTN_100210709_NoMoveCard_100210710 + i, _ix, _iy);
				int iw = 0;
				int ih = 0;
				m_MyUI.GetControlGameWH(CTN_1000000_CTN_100210709_NoMoveCard_100210710 + i, iw, ih);

				CString sdp;
				sdp.Format("sdp_ ��һ���� ���� [%d,%d] [%d,%d] [iw=%d]",ix,iy,_ix,_iy,iw / 2 - 20);
				OutputDebugString(sdp);

				if ((_ix + iw / 2 - 20) == ix && _iy == iy)
				{
					BYTE icard[5] = {0};
					int	iDesk = m_pGameFrame->ViewStation2DeskStation(i);
					CopyMemory(icard,&m_DeskCard[iDesk],sizeof(BYTE)*5);
					ShowHandCard(iDesk, icard, m_iSendCardNum[iDesk], true);

					sdp.Format("sdp_ ��һ���� ���� ���ID[%d]������=%d,%d",i,m_iDeskCardCount[i],m_iSendCardNum[i]);
					OutputDebugString(sdp);


					break;
				}
			}
		}
		break;
	case CTN_1000000_CTN_100210728_MOVE_100210756://�����ƶ�ͼƬ
		{
			m_MyUI.ShowControl(CTN_1000000_CTN_100210728_IMG_100210780, false);
			int ix = 0;
			int iy = 0;
			m_MyUI.GetControlGameXY(CTN_1000000_CTN_100210728_IMG_100210780, ix, iy);

			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				int _ix = 0;
				int _iy = 0;

				m_MyUI.GetControlGameXY(CTN_1000000_CTN_100210728_IMG_100210776 + i, _ix, _iy);

				if (_ix == ix && _iy == iy)
				{
					//��ʾ����µ�ע
					ShowPlayerNote(m_pGameFrame->ViewStation2DeskStation(i),true);
					break;
				}
			}
		}
		break;
	case CTN_1000000_CTN_100210728_MOVE_100210757://�����ƶ�����
		{
			m_MyUI.ShowControl(CTN_1000000_CTN_100210728_TEXT_100210785, false);
			int ix = 0;
			int iy = 0;
			m_MyUI.GetControlGameXY(CTN_1000000_CTN_100210728_TEXT_100210785, ix, iy);

			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				int _ix = 0;
				int _iy = 0;

				m_MyUI.GetControlGameXY(CTN_1000000_CTN_100210728_TEXT_100210781 + i, _ix, _iy);

				if (_ix == ix && _iy == iy)
				{
					//��ʾ����µ�ע
					ShowPlayerNote(m_pGameFrame->ViewStation2DeskStation(i),true);
					break;
				}
			}
		}
		break;
	default:
		break;
	}
	return;
}
//������������Ϣ
void CNewGameClient::OnUIMess_LBUTTONUP(UINT32 uControlID)
{
	switch(uControlID)
	{
	case CTN_1000000_CTN_1001000_BTN_1001100:
		{
			KillAllTimer();
			m_pGameFrame->ClickContinue();
			InitGameData();
			InitGameUI();
		}
		break;
	case CTN_1000000_CTN_1001000_BTN_1001200:
		{
			m_pGameFrame->CloseClient();
		}
		break;
	case CTN_1000000_CTN_100210728_BTN_100210729:///<��Ť1��
		{
			OnHitActiveResult(TYPE_FOLLOW, 0);
		}
		break;
	case CTN_1000000_CTN_100210728_BTN_100210730:///<��Ť2ȫѺ
		{
			OnHitActiveResult(TYPE_SHUTTLE, 0);
		}
		break;
	case CTN_1000000_CTN_100210728_BTN_100210731:///<��Ť3��ע
	case CTN_1000000_CTN_100210728_BTN_100210732:///<��Ť4��ע
	case CTN_1000000_CTN_100210728_BTN_100210733:///<��Ť5��ע
		{
			int itag = uControlID - CTN_1000000_CTN_100210728_BTN_100210731;
			OnSelectNote(itag);
		}
		break;
	case CTN_1000000_CTN_100210728_BTN_100210734:///<��Ť6����
		{
			OnHitActiveResult(TYPE_GIVE_UP, 0);
		}
		break;
	case CTN_2_BTN_1003500:
		{
			OutputDebugString("sdp_ ������� ������ð�ť ��ʾ�������");
			ShowInitSet(true);
			break;
		}
	case CTN_1000000_CTN_1003510_BTN_1003540://ok
		{
			bool bOldBSound = m_bSound;
			m_bSound = m_MyUI.GetIsSelect(CTN_1000000_CTN_1003510_RButton_1003520);
			OutputDebugString("sdp_ ������� ���OK �����������");
			ShowInitSet(false);

			CString s = CBcfFile::GetAppPath ();/////����·��

			DWORD dwCfgFile = ::cfgOpenFile(s + GET_SKIN_FOLDER(szTempStr) +".bcf");

			if(dwCfgFile >=0x10) //�ļ��򿪳ɹ�
			{
				//������������
				::cfgSetValue(dwCfgFile,"Operate","Sound",m_bSound);

				::cfgClose(dwCfgFile);						
			}

			if (m_bSound)//��������
			{
				if (bOldBSound != m_bSound)//���ô��ڸı�
				{
					if(m_SoundEngine)
						m_SoundEngine->setAllSoundsPaused(false);
					PlaySceneSound();
				}
			} 
			else//�ر�����
			{
				if (bOldBSound != m_bSound)//���ô��ڸı�
				{
					if(m_SoundEngine)
					{
						m_SoundEngine->setAllSoundsPaused(true);
						m_SoundEngine->stopAllSounds();
					}
				}
			}


			break;
		}
	case CTN_1000000_CTN_1003510_BTN_1003550://ȡ��
		{
			m_MyUI.SetIsSelect(CTN_1000000_CTN_1003510_RButton_1003520, m_bSound);
			OutputDebugString("sdp_ ������� ���ȡ�� �����������");
			ShowInitSet(false);
		}
		break;
	default:
		break;
	}
	return;
}

//�������/�Ƴ��ؼ���Ϣ
void CNewGameClient::OnUIMess_MOUSEENTER(UINT32 uControlID)
{
	if (m_MyUI.IsBut(uControlID))
	{
		PlaySound(SOUND_MOVEBUTTON);
	}
	return;
}

//��ʾ���ÿ�
void CNewGameClient::ShowInitSet(bool bFlag)
{
	m_MyUI.ShowControl(CTN_1000000_CTN_1003510, bFlag);
	m_MyUI.ShowControl(CTN_1000000_CTN_1003510_BTN_1003540, bFlag);
	m_MyUI.ShowControl(CTN_1000000_CTN_1003510_BTN_1003550, bFlag);

	m_MyUI.SetIsSelect(CTN_1000000_CTN_1003510_RButton_1003520, m_bSound);
	m_MyUI.ShowControl(CTN_1000000_CTN_1003510_RButton_1003520, bFlag);

	m_MyUI.ShowControl(CTN_1000000_CTN_1003510_TEXT_1003521, bFlag);

	OutputDebugString("sdp_ ������� �������������Ч");
	return;
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
	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	OutputDebugString("zhtlog:����뿪");

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
	CString sdp;

	switch(nTimerID)
	{
	case ID_TIME_SEE_CARD:
		{
			m_pGameFrame->KillTimer(ID_TIME_SEE_CARD);

			BYTE icard[5] = {0};
			CopyMemory(icard,&m_DeskCard[m_bMyDesk],sizeof(BYTE)*5);
			ShowHandCard(m_bMyDesk, icard, m_iDeskCardCount[m_bMyDesk], true);	
		}
		break;
	case ID_BEGIN_TIME:
	case ID_END_TIME:
		{
			if (nTimerID == ID_BEGIN_TIME)
			{
				m_pGameFrame->KillTimer(ID_BEGIN_TIME);
			} 
			else
			{
				m_pGameFrame->KillTimer(ID_END_TIME);
			}
			
			if (!m_bIsWatching)
			{
				m_pGameFrame->CloseClient();
			}
			
			InitGameData();
			InitGameUI();
		}
		break;
	case ID_TIME_THINK:
		{
			KillGameTimer(m_iNowOutPeople);
		}
		break;
	case ID_TIME_BASENOTE:
		{
			m_pGameFrame->KillTimer(ID_TIME_BASENOTE);
			if (IsPlayer(m_Time_BASENOTE_Count))
			{
				//�����ע����
				ShowNoteAnima(m_Time_BASENOTE_Count, m_iBaseNote, 100);
			}
			if (m_Time_BASENOTE_Count >= PLAY_COUNT)//�Ѿ��������������
			{
				return;
			}

			m_Time_BASENOTE_Count++;
			if (IsPlayer(m_Time_BASENOTE_Count))
			{
				m_pGameFrame->SetTimer(ID_TIME_BASENOTE,800);
			}
			else
			{
				m_pGameFrame->SetTimer(ID_TIME_BASENOTE,10);
			}

			break;
		}
	case ID_TIME_SEND_CARD://���ƶ���
		{
			m_pGameFrame->KillTimer(ID_TIME_SEND_CARD);

			if (m_iSendCardTotle >= m_iNeedSendCardTotle)//������
			{
				m_bSendCard = false;
				break;
			}

			if (!IsPlayer(m_iSendAnimaCardPos) || m_iUserStation[m_iSendAnimaCardPos] == TYPE_GIVE_UP)//��λ������Ҳ����� �� �Ѿ����ƣ���Ѹ�������¼ҷ���
			{
				m_iSendAnimaCardPos = (m_iSendAnimaCardPos - 1 + PLAY_COUNT) % PLAY_COUNT;//��ʱ�뷢��
				m_pGameFrame->SetTimer(ID_TIME_SEND_CARD, 20);
				break;
			}
			PlaySound(SOUND_SEND_CARD);
			int iView = ViewStation(m_iSendAnimaCardPos);
			BYTE icard[1] = {0};
			icard[0] = m_DeskCard[m_iSendAnimaCardPos][m_iSendCardNum[m_iSendAnimaCardPos]];
			m_iSendCardNum[m_iSendAnimaCardPos]++;
			m_MyUI.PlayCardInAnima(CTN_1000000_CTN_100210709_MOVE_100210766, 
				CTN_1000000_CTN_100210709_MOVE_100210766, 
				CTN_1000000_CTN_100210709_NoMoveCard_100210710, 
				CTN_1000000_CTN_100210709_NoMoveCard_100210767, 
				iView, 
				icard, 
				1, 
				200, 
				true);

			m_iSendCardTotle++;
			m_iSendAnimaCardPos = (m_iSendAnimaCardPos - 1 + PLAY_COUNT) % PLAY_COUNT;//��ʱ�뷢��

			m_pGameFrame->SetTimer(ID_TIME_SEND_CARD, 400);
		}
	default:
		{
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

int CNewGameClient::ViewStation(int desk)
{
	if (NULL != m_pGameFrame)
	{
		return m_pGameFrame->DeskStation2View(desk);
	}

	return 0;


}

//�Ƿ���ڸ����
bool CNewGameClient::IsPlayer(BYTE bDeskStation)
{
	UserInfoForGame UserInfo;
	if (NULL != m_pGameFrame)
	{
		if (m_pGameFrame->GetUserInfo(bDeskStation, UserInfo) && m_bUserState[bDeskStation])
		{
			return true;
		}
	}

	return false;
}


///ͬ����Ϸ
bool CNewGameClient::OnControlHitBegin()
{
	//if (m_bInLisureRoom)//����Ϸ�Ƿ������з�
	//{
	//	MSG_GM_S_ClientInfo ClientInfo;
	//	ClientInfo.bEnableWatch=false;
	//	m_pGameFrame->SendGameData(&ClientInfo,sizeof(ClientInfo),MDM_GM_GAME_FRAME,ASS_GM_GAME_INFO,0);
	//}
	//else
	{
		m_pGameFrame->SendGameData(ASS_GM_AGREE_GAME,NULL,0);
	}
				
	return true;
}

//��Ϸ�߼���������
void CNewGameClient::InitGameData()
{
	m_bTimeOutCount = 0;
	m_iNowBigNote = 0;
	m_bSendCard = false;
	m_iThisNoteTimes = 0;			//Ѻע����
	//������Ϣ
	m_iLimitNote     = 0;				//����޶�ע
	 m_iBaseNote     = 0;				//��ע
	m_iThisTurnLimit = 0;			//������ע
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	memset(m_DeskCard,0,sizeof(m_DeskCard));
	memset(m_UserCard,0,sizeof(m_UserCard));
	memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
	m_iNoteTotalPoint = 0;
	m_iSendAnimaCardPos = 0;     ///���ƶ���λ��
	m_iSendCardTotle = 0;         ///���Ƶ�����
	m_iNeedSendCardTotle = 0;	 ///��ǰ��Ҫ��������Ŀ
	ZeroMemory(m_iSendCardNum,sizeof(m_iSendCardNum));
	SetNoteButText(m_iBaseNote);
	::memset(m_iThisTurnNote,0,sizeof(m_iThisTurnNote));
	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));
	m_iNowOutPeople = -1;
	m_iFirstOutPeople=-1;
	memset(m_iThisTurnNote,0,sizeof(m_iThisTurnNote));
	m_bTimeOutCount=0;
	m_bShowAnimalStation=0;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		m_iUserStation[i] = -1;
	}
	memset(m_szName,0,sizeof(m_szName));
	memset(m_iDoublePointTime,0,sizeof(m_iDoublePointTime));
	memset(m_iiProtectTime,0,sizeof(m_iiProtectTime));
	memset(m_TempNote,0,sizeof(m_TempNote));
	memset(m_iTempThisTurnNote,0,sizeof(m_iTempThisTurnNote));
	memset(m_bUserState,0,sizeof(m_bUserState));
	memset(m_iAddButtonNum,0,sizeof(m_iAddButtonNum));
	m_Time_BASENOTE_Count=0;
	m_byCurStyle=0;
	m_iFirstSendCardCount = 1;
	memset(m_bNetCutPlayer,0,sizeof(m_bNetCutPlayer));
}
//��ϷUI����
void CNewGameClient::InitGameUI()
{
	for (int i = 0; i< PLAY_COUNT; ++i)
	{
		m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724 + i, false);//��ʱ��
	}

	ShowNoteBtnEnable();
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		ShowPlayerNote(i,false);
		BYTE iCard[5] = {0};
		ShowHandCard(i, iCard, 0, false);
	}

	ShowCardMaxMark(false);
	ShowTotalNote(false);

	GameFinishNotify szResultInfo;
	ShowResult(szResultInfo, false);
}

/// FlashWindowEx��գ�
void CNewGameClient::FlashWindow()
{
	HWND hwnd = (m_MyUI.GetpIUI()->GetWindowsHwnd());
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
void CNewGameClient::SetGameTimer(BYTE bDesk)
{
	m_pGameFrame->SetTimer(ID_TIME_THINK, m_iThinkTime * 1000);
	int iView = ViewStation(bDesk);
	m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720 + iView, true);
	m_MyUI.SetClock(CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724 + iView, m_iThinkTime, true);

	return;

}

/// ������ʱ��
void CNewGameClient::KillGameTimer(BYTE bDesk)
{
	m_pGameFrame->KillTimer(ID_TIME_THINK);
	int iView = ViewStation(bDesk);
	m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720 + iView, false);
	m_MyUI.SetClock(CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724 + iView, 0, false);

	if (m_iNowOutPeople == m_bMyDesk)
	{
		ShowNoteBtnEnable();
	}
	return;
}

/// ��ʾ �����
void CNewGameClient::ShowResult(GameFinishNotify szResultInfo, bool bFlag)
{
	//TCHAR				name[PLAY_COUNT][61];//�ǳ�
	//BYTE				iCardShape[PLAY_COUNT];		//����	
	//__int64				iWardPoint[PLAY_COUNT];//�÷�
	//__int64				iMoney[PLAY_COUNT];//���
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		m_MyUI.ShowControl(CTN_1000000_CTN_1001000_TEXT_1001001 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_1001000_TEXT_1001011 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_1001000_IMG_100210701 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_1001000_TEXT_100210705 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_1001000_TEXT_1001041 + i, false);
	}
	m_MyUI.ShowControl(CTN_1000000_CTN_1001000_BTN_1001100, bFlag);
	m_MyUI.ShowControl(CTN_1000000_CTN_1001000_BTN_1001200, bFlag);

	if (!bFlag)
	{
		m_MyUI.ShowControl(CTN_1000000_CTN_1001000, false);
		return;
	}

	UserInfoForGame UserInfo;
	int iDesk = 0;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (!IsPlayer(i))
			continue;

		//�ǳ�
		if (m_pGameFrame->GetUserInfo(i,UserInfo))
			m_MyUI.SetTextInfo(CTN_1000000_CTN_1001000_TEXT_1001001 + iDesk, UserInfo.dwUserID, true);

		//����
		TCHAR _TcNickName[MAX_PATH] = {0};
		switch(szResultInfo.iCardShape[i])
		{
		case UG_ERROR_KIND:/**< ���� */
			{
				sprintf(_TcNickName, "��");
			}
			break;
		case SH_OTHER:/**< ɢ�� */
			{
				sprintf(_TcNickName, "ɢ��");
			}
			break;
		case SH_DOUBLE:/**< ���� */
			{
				sprintf(_TcNickName, "����");
			}
			break;
		case SH_TWO_DOUBLE:/**< ���� */
			{
				sprintf(_TcNickName, "����");
			}
			break;
		case SH_THREE:/**< ���� */
			{
				sprintf(_TcNickName, "����");
			}
			break;
		case SH_SMALL_CONTINUE:/**< ��С˳�� */
			{
				sprintf(_TcNickName, "��С˳��");
			}
			break;
		case SH_CONTINUE:/**< ˳�� */
			{
				sprintf(_TcNickName, "˳��");
			}
			break;
		case SH_SAME_HUA:/**< ͬ�� */
			{
				sprintf(_TcNickName, "ͬ��");
			}
			break;
		case SH_HU_LU:/**< ��« */
			{
				sprintf(_TcNickName, "��«");
			}
			break;
		case SH_TIE_ZHI:/**< ��֧ */
			{
				sprintf(_TcNickName, "��֧");
			}
			break;
		case SH_SMALL_SAME_HUA_CONTINUE:/**< ͬ����С˳�� */
			{
				sprintf(_TcNickName, "ͬ����С˳��");
			}
			break;
		case SH_SAME_HUA_CONTINUE:/**< ͬ��˳ */
			{
				sprintf(_TcNickName, "ͬ��˳");
			}
			break;
		default:
			{
				sprintf(_TcNickName, "��");
				break;
			}
		}

		m_MyUI.SetTextInfo(CTN_1000000_CTN_1001000_TEXT_1001011 + iDesk, _TcNickName, true);
		//ʤ��
		TCHAR TcNickName[MAX_PATH] = {0};
#ifdef UI2_5D
		if (szResultInfo.iMoney[i] > 0)
		{
			sprintf(TcNickName, "%s/resources/result/win.png", CBcfFile::GetAppPath());
		}
		else if(szResultInfo.iMoney[i] < 0)
		{
			sprintf(TcNickName, "%s/resources/result/lost.png", CBcfFile::GetAppPath());
		}
#else
		if (szResultInfo.iMoney[i] > 0)
		{
			sprintf(TcNickName, "%s/resources/result/win.bmp", CBcfFile::GetAppPath());
		}
		else if(szResultInfo.iMoney[i] < 0)
		{
			sprintf(TcNickName, "%s/resources/result/lost.bmp", CBcfFile::GetAppPath());
		}
#endif
		m_MyUI.SetImagesLoadPic(CTN_1000000_CTN_1001000_IMG_100210701 + iDesk, TcNickName, true);

		//����
		m_MyUI.SetTextInfo(CTN_1000000_CTN_1001000_TEXT_100210705 + iDesk, szResultInfo.iWardPoint[i], true);
		//���
		m_MyUI.SetTextInfo(CTN_1000000_CTN_1001000_TEXT_1001041 + iDesk, szResultInfo.iMoney[i], true);

		iDesk++;
	}

	if (m_pGameFrame->GetIsWatching())
	{
		m_MyUI.EnableControl(CTN_1000000_CTN_1001000_BTN_1001200,false);
	}
	else
	{
		m_MyUI.EnableControl(CTN_1000000_CTN_1001000_BTN_1001200,true);
	}

	m_MyUI.ShowControl(CTN_1000000_CTN_1001000, true);

	m_pGameFrame->SetTimer(ID_END_TIME,m_iBeginTime*1000);
	m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720 + ViewStation(m_bMyDesk), true);
	m_MyUI.SetClock(CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724 + ViewStation(m_bMyDesk), m_iBeginTime, true);
  
}

//���÷�����Ϣ
void CNewGameClient::ShowRoomInfo()
{
	m_MyUI.ShowControl(CTN_1000000_CTN_100210714, true);

	m_MyUI.SetTextInfo(CTN_1000000_CTN_100210714_TEXT_100210715, m_iBaseNote, true);
	m_MyUI.SetTextInfo(CTN_1000000_CTN_100210714_TEXT_100210716, m_iLimitNote, true);
	return;
}

//���ð�ť����״̬
void CNewGameClient::ShowNoteBtnEnable(bool bFollow,bool bAllNote,bool bNote,bool bGiveUp)
{
	for (int i = 0; i < 6; ++i)
	{
		m_MyUI.ShowControl(CTN_1000000_CTN_100210728_BTN_100210729 + i, true);
	}
	
	m_MyUI.EnableControl(CTN_1000000_CTN_100210728_BTN_100210729,bFollow);
	m_MyUI.EnableControl(CTN_1000000_CTN_100210728_BTN_100210730,bAllNote);

	ShowNoteButText(bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_100210728_BTN_100210731,bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_100210728_BTN_100210732,bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_100210728_BTN_100210733,bNote);

	m_MyUI.EnableControl(CTN_1000000_CTN_100210728_BTN_100210734,bGiveUp);

	return ;
}

//---------------��Ϸ����----------------------------------
//�û�������
void CNewGameClient::OnHitActiveResult(BYTE btag, __int64 iNote)
{
	if (GetStationParameter() == GS_PLAY_GAME)
	{
		if (m_iNowOutPeople != m_bMyDesk)
		{
			KillGameTimer(m_bMyDesk);
			ShowNoteBtnEnable();
			return;
		}
		
		tagUserProcess process;
		process.iVrebType = btag;	// ��ע����
		process.iNote =iNote;		// ��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
		m_pGameFrame->SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
	}
	return;
}

//�û�ѡ����ע��
void CNewGameClient::OnSelectNote(int itag)
{
	if (itag < 0 || itag > 3)
	{
		return;
	}

	tagUserProcess process;
	memset(&process,NULL,sizeof(process));

	//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ) ��Ϊ��Ϊ__int64��lparam�޷����ɶ��޸�  xyh:20110729
	switch(itag)
	{
	case 2:     //��������ע��ť
		{
			process.iNote = 4*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
			break;
		}
	case 1:     //�ڶ�����ע��ť
		{
			process.iNote =  2*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
			break;
		}
	case 0:     //��һ����ע��ť
		{	
			process.iNote =  1*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
			break;
		}
	default:
		break;
	}

	process.iVrebType = (m_iNowBigNote == 0)?TYPE_NOTE:TYPE_ADD;//��ע����
	m_pGameFrame->SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
	return;
}

//��ʾ׼�������
void CNewGameClient::ShowAgree(BYTE bDeskStation)
{
	UserInfoForGame UserInfo;
	if (NULL != m_pGameFrame)
	{
		if (!m_pGameFrame->GetUserInfo(bDeskStation, UserInfo))
		{
			return;
		}
	}
	
	int iView = ViewStation(bDeskStation);
	TCHAR pchar[MAX_PATH] = {0};
	sprintf(pchar, "%s/resources/agree.png", CBcfFile::GetAppPath());
	m_MyUI.SetImagesLoadPic(CTN_1000000_CTN_100210736_IMG_100210741 + iView, pchar, true);

	if (bDeskStation == m_bMyDesk)
	{	
		m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720 + ViewStation(m_bMyDesk), false);
		m_MyUI.SetClock(CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724 + ViewStation(m_bMyDesk), 0, false);
		m_MyUI.ShowControl(CTN_1000000_BTN_10001, false);
		m_pGameFrame->KillTimer(ID_BEGIN_TIME);
	}
	
	return;
}

//��ʾ����
void CNewGameClient::ShowGiveUp(BYTE bDeskStation)
{
	if (!IsPlayer(bDeskStation))
	{
		return;
	}

	int iView = ViewStation(bDeskStation);
	TCHAR pchar[MAX_PATH] = {0};
	sprintf(pchar, "%s/resources/pass.png", CBcfFile::GetAppPath());
	m_MyUI.SetImagesLoadPic(CTN_1000000_CTN_100210736_IMG_100210741 + iView, pchar, true);

	return;
}

//��Ϸ��ʼʱ��ʼ������
void CNewGameClient::ShowBegin()
{
	for (int i = 0; i< PLAY_COUNT; ++i)
	{
		m_MyUI.ShowControl(CTN_1000000_CTN_100210736_IMG_100210741 + i, false);//׼����־
		m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_100210718_CTN_100210720_Time_100210724 + i, false);//��ʱ��
	}
	m_MyUI.ShowControl(CTN_1000000_BTN_10001, false);//׼����ť����
	m_pGameFrame->KillTimer(ID_BEGIN_TIME);

	return;
}

__int64 CNewGameClient::GetMaxNote(__int64 a[4])
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

//������������ע
BOOL CNewGameClient::ReplaceUserNote(BYTE bDeskStation)
{
	KillAllTimer();
	if (GetStationParameter() == GS_PLAY_GAME)
	{
		ReplaceUserNoteStruct supNote;
		supNote.bDeskStation = bDeskStation;	//������עλ��
		supNote.iVrebType = 0;					//��ע����
		supNote.iNote = 0;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
		m_pGameFrame->SendGameData(&supNote,sizeof(supNote),MDM_GM_GAME_NOTIFY,ASS_REPLACE_VREB_CHECK,0);
	}
	return TRUE;
}

//��ѭ��������������
void CNewGameClient::PlaySound(BYTE bDeskStation, BYTE bStyle)
{
	if (!m_bSound || m_SoundEngine==NULL)
	{
		return;
	}

	if (!IsPlayer(bDeskStation))
	{
		return;
	}

	bool bIsBoy = false;
	UserInfoForGame UserInfo;
	if (m_pGameFrame->GetUserInfo(bDeskStation,UserInfo))
	bIsBoy = UserInfo.bBoy;
	char szFileName[MAX_PATH];

	switch(bStyle)
	{
	case SOUND_SHUTTLE:
		wsprintf(szFileName, "Music\\%s\\suoha.wav",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_FOLLOW:
		wsprintf(szFileName, "Music\\%s\\xiazhu.ogg",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_ADD:
		wsprintf(szFileName, "Music\\%s\\xiazhu.ogg",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_NOTE:
		wsprintf(szFileName, "Music\\%s\\xiazhu.ogg",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_GIVE_UP:
		wsprintf(szFileName, "Music\\%s\\pass.wav",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_MOVEBUTTON:
		wsprintf(szFileName, "Music\\%s\\movebutton.ogg",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_WARNING:
		wsprintf(szFileName, "Music\\%s\\Warning.wav",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_SEND_CARD:
		wsprintf(szFileName, "Music\\%s\\fapai.wav",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_START_DEF:
		wsprintf(szFileName, "Music\\%s\\start.wav",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_WIN:
		wsprintf(szFileName, "Music\\%s\\ying.wav",bIsBoy?"MAN":"WOM");
		break;
	case SOUND_LOSE:
		wsprintf(szFileName, "Music\\%s\\shu.wav",bIsBoy?"MAN":"WOM");
		break;
	default:
		return;
		break;
	}

	m_SoundEngine->play2D(szFileName,false);
	return;
}

//��ѭ��������������(�����ж��������Ů)
void CNewGameClient::PlaySound(BYTE bStyle)
{
	if (!m_bSound || m_SoundEngine==NULL)
	{
		return;
	}

	char szFileName[MAX_PATH];

	switch(bStyle)
	{
	case SOUND_SHUTTLE:
		wsprintf(szFileName, "Music\\suoha.wav");
		break;
	case SOUND_FOLLOW:
		wsprintf(szFileName, "Music\\xiazhu.ogg");
		break;
	case SOUND_ADD:
		wsprintf(szFileName, "Music\\xiazhu.ogg");
		break;
	case SOUND_NOTE:
		wsprintf(szFileName, "Music\\xiazhu.ogg");
		break;
	case SOUND_GIVE_UP:
		wsprintf(szFileName, "Music\\pass.wav");
		break;
	case SOUND_MOVEBUTTON:
		wsprintf(szFileName, "Music\\movebutton.ogg");
		break;
	case SOUND_WARNING:
		wsprintf(szFileName, "Music\\Warning.wav");
		break;
	case SOUND_SEND_CARD:
		wsprintf(szFileName, "Music\\fapai.wav");
		break;
	case SOUND_START_DEF:
		wsprintf(szFileName, "Music\\start.wav");
		break;
	case SOUND_WIN:
		wsprintf(szFileName, "Music\\ying.wav");
		break;
	case SOUND_LOSE:
		wsprintf(szFileName, "Music\\shu.wav");
		break;
	default:
		return;
		break;
	}

	m_SoundEngine->play2D(szFileName,false);
	return;
}

//�����������
void CNewGameClient::PlaySceneSound()
{
	char szFileName[MAX_PATH];
	strcpy(szFileName, "Music\\gamemusic\\1.ogg");
	m_SoundEngine->play2D(szFileName,true);

	return;
}

//��ע
void CNewGameClient::AddTotalNote(__int64 iNoteTotal)
{
	m_iNoteTotalPoint += iNoteTotal;
	ShowTotalNote(true);
	return ;
}

//��ʼ����ע
void CNewGameClient::SetTotalNote(__int64 iNoteTotal)
{
	m_iNoteTotalPoint=iNoteTotal;
	ShowTotalNote(true);
	return ;
}

//��ʾ��ע
void CNewGameClient::ShowTotalNote(bool bFlag)
{
	m_MyUI.SetTextInfo(CTN_1000000_CTN_100210728_TEXT_100210745, m_iNoteTotalPoint, bFlag, AlignmentCenter);
	return;
}

//�����ע����
void CNewGameClient::ShowNoteAnima(BYTE bDeskStation, __int64 iNote, int iTime)
{
	int bView = ViewStation(bDeskStation);
	int iMoveActionID[2] = {CTN_1000000_CTN_100210728_MOVE_100210756,CTN_1000000_CTN_100210728_MOVE_100210757};
	int iEndPointID[2] = {CTN_1000000_CTN_100210728_IMG_100210776,CTN_1000000_CTN_100210728_TEXT_100210781};
	int iBeMoveID[2] = {CTN_1000000_CTN_100210728_IMG_100210780,CTN_1000000_CTN_100210728_TEXT_100210785};

	m_MyUI.SetTextInfo(CTN_1000000_CTN_100210728_TEXT_100210785, iNote, false, AlignmentCenter);

	m_MyUI.PlayChipInAnima_Image(iMoveActionID, 
		CTN_1000000_CTN_100210709_NoMoveCard_100210710, 
		iEndPointID, 
		iBeMoveID, 
		bView, 
		(int)iNote, 
		iTime, 
		true);

	return;
}

//ɱ�����м�ʱ��
void CNewGameClient::KillAllTimer()
{
	m_pGameFrame->KillTimer(ID_BEGIN_TIME);
	m_pGameFrame->KillTimer(ID_END_TIME);
	m_pGameFrame->KillTimer(ID_TIME_SEND_CARD);
	m_pGameFrame->KillTimer(ID_TIME_THINK);
	m_pGameFrame->KillTimer(ID_TIME_BASENOTE);
	return;
}

//��ʾ����µ�ע
void CNewGameClient::ShowPlayerNote(BYTE bDeskStation, bool bFlag)
{
	int iView = ViewStation(bDeskStation);
	m_MyUI.SetTextInfo(CTN_1000000_CTN_100210728_TEXT_100210781 + iView, m_iPerJuTotalNote[bDeskStation], bFlag, AlignmentCenter);
	m_MyUI.ShowControl(CTN_1000000_CTN_100210728_IMG_100210776 + iView, bFlag);
	
	return;
}

//��ʾ�������
void CNewGameClient::ShowHandCard(BYTE bDeskStation, BYTE iCard[], int iCount, bool bFlag)
{
	int iView = ViewStation(bDeskStation);

	m_MyUI.SetCardInfo(CTN_1000000_CTN_100210709_NoMoveCard_100210710 + iView, iCard, iCount, bFlag);
	return;
}

//��ʾ�������ұ��
void CNewGameClient::ShowCardMaxMark(bool bFlag)
{
	if (bFlag)
	{
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			int iView = ViewStation(i);
			if (IsPlayer(i) && i == m_iNowOutPeople)
			{
				m_MyUI.ShowControl(CTN_1000000_CTN_100210736_IMG_100210737 + iView, true);
			}
			else
			{
				m_MyUI.ShowControl(CTN_1000000_CTN_100210736_IMG_100210737 + iView, false);
			}
		}
	} 
	else
	{
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			m_MyUI.ShowControl(CTN_1000000_CTN_100210736_IMG_100210737 + i, false);
		}
	}

	return;
}

//��ʾ��ע��ť�ϵ���Ŀ
void CNewGameClient::ShowNoteButText(bool bFlag)
{
	if (bFlag)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_MyUI.SetTextForBut( CTN_1000000_CTN_100210728_BTN_100210731 + i, m_iAddButtonNum[i]);


		}
	} 
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_MyUI.SetTextForBut( CTN_1000000_CTN_100210728_BTN_100210731 + i, 0);
		}
	}

	return;
}

//������ע��ť�ϵ���Ŀ
void CNewGameClient::SetNoteButText(__int64 iBaseNote)
{
	//modified by yjp.��ע��ť���ݵ�ע������������
	__int64 ibt[3] = {0};
	ibt[0] = (__int64)(iBaseNote*1.0);//(DOUBLE)m_iLimitNote*1.5/100;
	ibt[1] = (__int64)(iBaseNote*2.0);//(DOUBLE)m_iLimitNote*3/100;
	ibt[2] = (__int64)(iBaseNote*4.0);//(DOUBLE)m_iLimitNote*6/100;
	if (ibt[0]<= 0)
	{
		ibt[0] = 1;
		ibt[1] = 2;
		ibt[2] = 4;
	}

	CopyMemory(m_iAddButtonNum, ibt, sizeof(ibt));

	return;
}