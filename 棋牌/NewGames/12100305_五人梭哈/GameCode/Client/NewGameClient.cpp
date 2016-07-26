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
	m_pIUI = pUI;
	m_MyUI.SetpIUI(pUI,NAME_ID);
	m_bIsWatching = false;
	m_pGameFrame = pGameFrame;
	m_bGameStation = GS_WAIT_SETGAME;           //��Ϸ״̬
	m_iBeginTime = 0;			//��Ϸ��ʼʱ��
	m_iThinkTime = 0;			//��Ϸ˼��ʱ��
	m_bTimeOutCount = 0;
	m_bSound = TRUE;
	m_SoundEngine = NULL;
	m_bCanMove = false;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	m_SoundEngine = createIrrKlangDevice();
	m_bAutoStart=false;
	m_i64HalfLessMoney=0;
	InitGameData();
	m_bNewTurn=false;
	memset(m_iTempBt,0,sizeof(m_iTempBt));
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
	// ����UIsdp
	LoadData();

	TCHAR szDataFileName[MAX_PATH]={0};
	
	sprintf(szDataFileName, "UI_%d.dat",NAME_ID);
	m_MyUI.Initial(szDataFileName);

	m_crMoveRect.CreateRectRgn(685,640,760,640+27);

	return 0;
}
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	m_MyUI.ShowControl(CTN_1000000_BTN_10001, false);//׼����ť����
	//���������Ӧ454
	//SetButtonOnMoseMove(10121002,true);
	SetImageOnMoseMove(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121001,true);
	SetImageOnMoseMove(CTN_1000000_IMG_120120088,true);
	SetImageOnMoseMove(CTN_1000000_CTN_1012000_IMG_10150027,true);
	SetImageOnMoseMove(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,true);
	SetImageOnMoseMove(CTN_1000000_IMG_454,true);
	SetImageMoseThough(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,true);
	for(int i=0;i<6;i++)
	{
		SetImageOnMoseMove(CTN_1000000_CTN_1012000_BTN_10120041+i,true);
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetImageOnMoseMove(CTN_1000000_CTN_1012000_BTN_120120040+i,true);
	}
	//SetImageOnMoseMove(10121003,true);
	SetImageOnMoseMove(454,true);
	//��ʼ���ɼ�����
	SetControlVisible(CTN_1000000_BTN_2004043,false);
	SetControlVisible(CTN_1000000_CTN_2004042,false);
	SetControlVisible(CTN_1000000_BTN_2004044,true);
	for(int i=0;i<15;i++)
	{
		SetControlVisible(CTN_1000000_CTN_2004042_TEXT_10150028+i,false);
	}
	SetRButtonSelected(CTN_1000000_RButton_10150047,false);
	SetRButtonSelected(CTN_1000000_RButton_10150048,false);
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetControlVisible(CTN_1000000_CTN_120120073+i,false);
	}
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
	{
		return;
	}

	InitGameData();
	InitGameUI();
	ShowBegin();
	m_bNewTurn=false;
	memset(m_iTempBt,0,sizeof(m_iTempBt));
	KillAllTimer();
	
	memset((void *)&m_szName, 0,sizeof(m_szName));

	//������Ϸ״̬
	SetStationParameter(GS_SEND_CARD);
	m_bTimeOutCount = 0;												// ��ʱ����
	GameBeginStruct * pCmd=(GameBeginStruct *)buffer;
	m_bIsSuperUser = pCmd->bIsSuperUser;//�Լ��Ƿ񳬼��ͻ���//lym 2011-11-28

	m_iLimitNote = pCmd->iLimitNote;									// ���ע(һ��)
	m_iBaseNote = pCmd->iBaseNote;									// ��ע
	m_iSelAddMoney = m_iBaseNote;
	m_i64HalfLessMoney=pCmd->i64HalfLessMoney;
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
	OutputDebugString("dwjsh::OnHandleGM_ASS_SEND_CARD��0��");
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

	m_MyUI.ShowControl(CTN_1000000_CTN_1015000_NoMoveCard_10150011, false);
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
		SetImageOnMoseMove(CTN_1000000_IMG_120120088,true);
		m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,true);
	}
	else
	{
		SetImageOnMoseMove(CTN_1000000_IMG_120120088,false);
		m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,false);
	}
	bool bSelected=false;
	GetRButtonSelected(CTN_1000000_RButton_10150048,&bSelected);
	if(bSelected)
	{
		m_pGameFrame->KillTimer(ID_TIME_SEE_CARD);
		BYTE icard[5] = {0};
		CopyMemory(icard,&m_UserCard[m_bMyDesk],sizeof(BYTE)*5);
		ShowHandCard(m_bMyDesk, icard, m_iDeskCardCount[m_bMyDesk], true);
		m_pGameFrame->SetTimer(ID_TIME_SEE_CARD, 2000);
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
			if(m_bNewTurn)
			{
				add = FALSE;
			}
		}
		//����Ѻע������ lym 2011-12-13
		if(m_byAddNum>0 && m_byAddNum!=255)
		{
			if (pCmd->m_byUserAddNum[m_bMyDesk] >= m_byAddNum)
			{
				if(m_bNewTurn)
				{
					add = FALSE;
				}
			}
			//��һ�����ܼ�
			if (m_iFirstOutPeople == m_bMyDesk && !pCmd->bNewTurn)
			{
				if(m_bNewTurn)
				{
					add = FALSE;
				}
				
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
	int iViewStation=m_pGameFrame->DeskStation2View(pCmd->iOutPeople);
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
		__int64 i64AllBet=0;
		//������ע
		for(int i=0;i<PLAY_COUNT;i++)
		{
			i64AllBet+=m_iPerJuTotalNote[i];
		}
		SetControlVisible(CTN_1000000_IMG_120120045,true);
		SetTextinfo(CTN_1000000_TEXT_120120046,i64AllBet, true);
		SetTextinfo(CTN_1000000_CTN_1011000_TEXT_10110002,i64AllBet, true);
		//��������ע
		SetControlVisible(CTN_1000000_IMG_120120047+iViewStation,true);
		SetTextinfo(CTN_1000000_TEXT_120120052+iViewStation,m_iThisTurnNote[pCmd->iOutPeople], true);		
	}
	//������ʾ
	if (TYPE_GIVE_UP==pCmd->bNoteStyle)//����
	{
		//��ʾ����
		for(int i=0;i<5;i++)
		{
			SetControlVisible(CTN_1000000_CTN_120120062+i,false);
		}
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		SetControlVisible(CTN_1000000_CTN_120120062+iViewStation,true);
		SetPicSize(CTN_1000000_CTN_120120062_IMG_120120067+iViewStation,"./resources/05.png",true,188,0,47,24);
		m_pGameFrame->SetTimer(ID_TIME_TIP,2*1000);
	}
	else if (TYPE_NOTE==pCmd->bNoteStyle)//��ע
	{
		//��ʾ����
		for(int i=0;i<5;i++)
		{
			SetControlVisible(CTN_1000000_CTN_120120062+i,false);
		}
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		SetControlVisible(CTN_1000000_CTN_120120062+iViewStation,true);
		SetPicSize(CTN_1000000_CTN_120120062_IMG_120120067+iViewStation,"./resources/05.png",true,47,0,47,24);
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		m_pGameFrame->SetTimer(ID_TIME_TIP,2*1000);
	}
	else if (TYPE_ADD==pCmd->bNoteStyle)//��ע
	{
		//��ʾ����
		for(int i=0;i<5;i++)
		{
			SetControlVisible(CTN_1000000_CTN_120120062+i,false);
		}
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		SetControlVisible(CTN_1000000_CTN_120120062+iViewStation,true);
		SetPicSize(CTN_1000000_CTN_120120062_IMG_120120067+iViewStation,"./resources/05.png",true,0,0,47,24);
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		m_pGameFrame->SetTimer(ID_TIME_TIP,2*1000);
	}
	else if (TYPE_FOLLOW==pCmd->bNoteStyle)//��ע
	{
		//��ʾ����
		for(int i=0;i<5;i++)
		{
			SetControlVisible(CTN_1000000_CTN_120120062+i,false);
		}
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		SetControlVisible(CTN_1000000_CTN_120120062+iViewStation,true);
		SetPicSize(CTN_1000000_CTN_120120062_IMG_120120067+iViewStation,"./resources/05.png",true,141,0,47,24);
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		m_pGameFrame->SetTimer(ID_TIME_TIP,2*1000);
	}
	else if (TYPE_SHUTTLE==pCmd->bNoteStyle)//���
	{
		//��ʾ����
		for(int i=0;i<5;i++)
		{
			SetControlVisible(CTN_1000000_CTN_120120062+i,false);
		}
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		SetControlVisible(CTN_1000000_CTN_120120062+iViewStation,true);
		SetPicSize(CTN_1000000_CTN_120120062_IMG_120120067+iViewStation,"./resources/05.png",true,235,0,47,24);
		m_pGameFrame->KillTimer(ID_TIME_TIP);
		m_pGameFrame->SetTimer(ID_TIME_TIP,2*1000);
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
			//��ҽ�����Ϣ
			m_pGameFrame->InsertNormalMessage(str.GetBuffer(MAX_PATH));
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
	//ShowResult(finishnotify,true);
	for(int i=0; i<PLAY_COUNT; i++)
	{
		SetControlVisible(CTN_1000000_CTN_120120073+i,false);
	}
	//ˢ���ۻ��ɼ��ͱ��ֳɼ�
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (!IsPlayer(i))
			continue;
		//����״̬�����
		if (m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			SetTextShowText(CTN_1000000_CTN_2004042_TEXT_10150028+i,UserInfo.szNickName, true);
			SetTextinfo(CTN_1000000_CTN_2004042_TEXT_10150033+i,pCmd->i64UpGrade[i], true);
			SetTextinfo(CTN_1000000_CTN_2004042_TEXT_10150038+i,pCmd->i64AcculateGrade[i], true);
			//ˢ�±��ֳɼ�
			int iViewStation=m_pGameFrame->DeskStation2View(i);
			SetControlVisible(CTN_1000000_CTN_120120073+iViewStation,true);
			if(pGameEnd->iChangeMoney[i]>0)
			{
				wchar_t szImagePath[256]=L"./resources/10.png";
				SetInumInfo(CTN_1000000_CTN_120120073_Num_120120078+iViewStation,szImagePath,pGameEnd->iChangeMoney[i],true);
				//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
				SetPicSize(CTN_1000000_CTN_120120073_IMG_120120083+iViewStation,"./resources/11.png", true,0,0,35,39);
			}
			else if(0==pGameEnd->iChangeMoney[i])
			{
				wchar_t szImagePath[256]=L"./resources/09.png";
				SetInumInfo(CTN_1000000_CTN_120120073_Num_120120078+iViewStation,szImagePath,pGameEnd->iChangeMoney[i],true);
			}
			else
			{
				wchar_t szImagePath[256]=L"./resources/09.png";
				SetInumInfo(CTN_1000000_CTN_120120073_Num_120120078+iViewStation,szImagePath,pGameEnd->iChangeMoney[i],true);
				//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
				SetPicSize(CTN_1000000_CTN_120120073_IMG_120120083+iViewStation,"./resources/11.png", true,35,0,35,39);
			}
		}
		else
		{
			SetControlVisible(CTN_1000000_CTN_2004042_TEXT_10150028+i,false);
			SetControlVisible(CTN_1000000_CTN_2004042_TEXT_10150033+i,false);
			SetControlVisible(CTN_1000000_CTN_2004042_TEXT_10150038+i,false);
		}
	}
	//��ʾ������ť
	SetControlVisible(CTN_1000000_CTN_1001000_BTN_1001100,true);
	bool bSelected=false;
	GetRButtonSelected(CTN_1000000_RButton_10150047,&bSelected);
	if(bSelected)
	{
		m_pGameFrame->SetTimer(ID_TIME_AUTOSTART,5*1000);
	}
	m_bAutoStart=true;
	//����������ʱ��
	m_pGameFrame->SetTimer(ID_END_TIME,m_iBeginTime*1000);
	m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + ViewStation(m_bMyDesk), true);
	m_MyUI.SetClock(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + ViewStation(m_bMyDesk), m_iBeginTime, true);
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
	m_iSelAddMoney	 = m_iBaseNote;
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
		m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + ViewStation(m_bMyDesk), true);
		m_MyUI.SetClock(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + ViewStation(m_bMyDesk), m_iBeginTime, true);
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
		m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + ViewStation(m_bMyDesk), true);
		m_MyUI.SetClock(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + ViewStation(m_bMyDesk), m_iBeginTime, true);
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
	GameStation_Base *pGameStation = (GameStation_Base *)pBuffer;
	if (NULL == pGameStation)
	{
		return;
	}
	//ˢ���ۻ��ɼ�
	UserInfoForGame tUserInfo;
	memset(&tUserInfo,0,sizeof(tUserInfo));
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pGameFrame->GetUserInfo(i,tUserInfo))
		{
			SetTextShowText(CTN_1000000_CTN_2004042_TEXT_10150028+i,tUserInfo.szNickName, true);
			SetTextinfo(CTN_1000000_CTN_2004042_TEXT_10150033+i,pGameStation->i64UpGrade[i], true);
			SetTextinfo(CTN_1000000_CTN_2004042_TEXT_10150038+i,pGameStation->i64AcculateGrade[i], true);
		}
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
	case ASS_COME_DESK:
		{
			if(nLen!=sizeof(NotifyComeMes))
			{
				return 0;
			}	
			NotifyComeMes * pComeDesk = (NotifyComeMes *)buffer;
			if (NULL == pComeDesk)
			{
				return 0;
			}
			if(255!=pComeDesk->byDeskStation)
			{
				UserInfoForGame tUserInfo;
				memset(&tUserInfo,0,sizeof(tUserInfo));
				if(m_pGameFrame->GetUserInfo(pComeDesk->byDeskStation,tUserInfo))
				{
					SetTextShowText(CTN_1000000_CTN_2004042_TEXT_10150028+pComeDesk->byDeskStation,tUserInfo.szNickName, true);
					SetTextinfo(CTN_1000000_CTN_2004042_TEXT_10150033+pComeDesk->byDeskStation,pComeDesk->i64UpGrade, true);
					SetTextinfo(CTN_1000000_CTN_2004042_TEXT_10150038+pComeDesk->byDeskStation,pComeDesk->i64AcculateGrade, true);
				}
			}
			break;
		}
	case ASS_LEAVE_DESK://�û��뿪��Ϸ�������
		{
			
			if(nLen!=sizeof(LeaveDeskMes))
			{
				return 0;
			}	
			LeaveDeskMes * pLeaveDesk = (LeaveDeskMes *)buffer;
			if (NULL == pLeaveDesk)
			{
				return 0;
			}
			
			if(255!=pLeaveDesk->byDeskStation)
			{
				SetControlVisible(CTN_1000000_CTN_2004042_TEXT_10150028+pLeaveDesk->byDeskStation,false);
				SetControlVisible(CTN_1000000_CTN_2004042_TEXT_10150033+pLeaveDesk->byDeskStation,false);
				SetControlVisible(CTN_1000000_CTN_2004042_TEXT_10150038+pLeaveDesk->byDeskStation,false);
			}
			break;
		}
	case ASS_NEW_TURN://��һ����Ϸ��ʼ
		{
			if(nLen!=sizeof(NewTurn))
			{
				return 0;
			}	
			NewTurn * pNewTurn = (NewTurn *)buffer;
			if (NULL == pNewTurn)
			{
				return 0;
			}
			m_bNewTurn=true;
			break;
		}
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
	CString cs;
	cs.Format("lwlog::pMessage->_uMessageType=%d",pMessage->_uMessageType);
	//OutputDebugString(cs);
	if(pMessage->_uMessageType == UI_LBUTTONUP)
	{
		switch(pMessage->_uControlID)
		{
		case CTN_1000000_CTN_1015000_NoMoveCard_10150023://����
			{
				if (GetStationParameter() == GS_WAIT_BACK || GetStationParameter() == GS_PLAY_GAME)
				{
					int x=0,y=0,iMinx=0,iMax=0;
					m_MyUI.GetControlGameXY(CTN_1000000_CTN_1015000_NoMoveCard_10150023,x,y);//������ƿؼ�λ��
					iMinx=x+((5-m_iDeskCardCount[m_pGameFrame->GetMyDeskStation()])*8);
					iMax=iMinx+15;
					
					if(pMessage->_mouseX>=iMinx && pMessage->_mouseX<=iMax)
					{
						
						m_pGameFrame->KillTimer(ID_TIME_SEE_CARD);
						BYTE icard[5] = {0};
						CopyMemory(icard,&m_UserCard[m_pGameFrame->GetMyDeskStation()],sizeof(BYTE)*5);
						ShowHandCard(m_pGameFrame->GetMyDeskStation(), icard, m_iDeskCardCount[m_pGameFrame->GetMyDeskStation()], true);
						m_pGameFrame->SetTimer(ID_TIME_SEE_CARD, 2*1000);
					}
				}
				break;
			}
		case CTN_1000000_RButton_10150047:///<�Զ���ʼ  ��ѡ�� 
			{
				if(m_bAutoStart)
				{
					bool bSelected=false;
					GetRButtonSelected(CTN_1000000_RButton_10150047,&bSelected);
					if(bSelected)
					{
						m_pGameFrame->SetTimer(ID_TIME_AUTOSTART,5*1000);
					}
				}
				break;
			}
		case CTN_1000000_RButton_10150048:///<�Զ�����  ��ѡ�� 
			{
				if (GetStationParameter() == GS_WAIT_BACK || GetStationParameter() == GS_PLAY_GAME)
				{
					
					bool bSelected=false;
					GetRButtonSelected(CTN_1000000_RButton_10150048,&bSelected);
					if(bSelected)
					{
						m_pGameFrame->KillTimer(ID_TIME_SEE_CARD);
						BYTE icard[5] = {0};
						CopyMemory(icard,&m_UserCard[m_bMyDesk],sizeof(BYTE)*5);
						ShowHandCard(m_bMyDesk, icard, m_iDeskCardCount[m_bMyDesk], true);
						m_pGameFrame->SetTimer(ID_TIME_SEE_CARD, 2000);
					}
					
				}
				break;
			}
		case CTN_1000000_BTN_2004043://�رճɼ�
			{
				SetControlVisible(CTN_1000000_CTN_2004042,false);
				SetControlVisible(CTN_1000000_BTN_2004043,false);
				SetControlVisible(CTN_1000000_BTN_2004044,true);
				break;
			}
		case CTN_1000000_BTN_2004044://�򿪳ɼ�
			{
				SetControlVisible(CTN_1000000_CTN_2004042,true);
				SetControlVisible(CTN_1000000_BTN_2004043,true);
				SetControlVisible(CTN_1000000_BTN_2004044,false);
				break;
			}
		}
	}
	if (pMessage->_uControlID == CTN_1000000_CTN_100210735_CTN_1002202 && pMessage->_uMessageType == UI_LBUTTONUP)
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
	case UI_LBUTTONDOWN:	//��갴��
		{
			
			POINT pPT;
			pPT.x = pMessage->_mouseX;
			pPT.y = pMessage->_mouseY;
			
			if (m_crMoveRect.PtInRegion(pPT))
			{
				m_ptMouse.x=pMessage->_mouseX;
				m_ptMouse.y=pMessage->_mouseY;
				m_bCanMove = true;
				SetImageSrcX(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,17);
			}
			else
			{
				if (pMessage->_uControlID == CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121001)
				{
					OutputDebugString("dwjlogsh::�������");
				}
			}
			break;
		}
	case UI_LBUTTONUP://����������
		{
			if (m_bIsWatching)
			{
				return 0;
			}
			
			//if(m_MyUI.GetControlShow(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002))
			//{
				m_bCanMove = false;
			//}
			//SetImageSrcX(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,0);
			OnUIMess_LBUTTONUP(pMessage->_uControlID);
			break;
		}
	case UI_MOUSEENTER://�������/�Ƴ��ؼ�
		{
			OnUIMess_MOUSEENTER(pMessage->_uControlID);
			
			break;
		}
	case UI_MOUSEMOVE:	//����ƶ�
		{
			//if (pMessage->_uControlID == 10121001 || pMessage->_uControlID == CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003)
			if(CTN_1000000_IMG_120120088==pMessage->_uControlID)
			{
				
				if (m_bCanMove)
				{
					int iMoveX = pMessage->_mouseX-m_ptMouse.x;
					int iMoveY = pMessage->_mouseY-m_ptMouse.y;

					POINT ptImage = GetImagePosition(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003);
					if (ptImage.x+iMoveX <= 685)
					{
						SetImagePosition(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,685,640);
						m_crMoveRect.SetRectRgn(685,640,760,640+27);
						m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,true);
						if(m_bNewTurn)
						{
							m_iSelAddMoney = m_iBaseNote;
						}
						else
						{
							m_iSelAddMoney=0;
						}
						
						m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002, m_iSelAddMoney);

					}
					else if (ptImage.x+iMoveX >= 760)
					{
						SetImagePosition(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,760,640);
						m_crMoveRect.SetRectRgn(685,640,773,640+27);
						m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,true);
						if(m_bNewTurn)
						{
							m_iSelAddMoney = m_iLimitNote;
						}
						else
						{
							m_iSelAddMoney=m_iTempBt[3];
						}
						
						m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002, m_iSelAddMoney);
					}
					else
					{
						SetImagePosition(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,ptImage.x+iMoveX,640);
						m_crMoveRect.SetRectRgn(ptImage.x+iMoveX,640,ptImage.x+iMoveX+26,640+27);
						m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,true);

						//��ȡ�ƶ���λ��
						int iX = (ptImage.x+iMoveX)-685;
						if (iX == 0)
						{
							if(m_bNewTurn)
							{
								m_iSelAddMoney = m_iBaseNote;
							}
							else
							{
								m_iSelAddMoney = 0;
							}
							
						}
						else if (iX == 200)
						{
							if(m_bNewTurn)
							{
								m_iSelAddMoney = m_iLimitNote;
							}
							else
							{
								m_iSelAddMoney = m_iTempBt[3];
							}
							
						}
						else
						{
							if(m_bNewTurn)
							{
								m_iSelAddMoney = m_iBaseNote + (((m_iLimitNote-m_iBaseNote)/200)*iX);
							}
							else
							{
								m_iSelAddMoney = 0 + (((m_iTempBt[3]-0)/200)*iX);
							}
							
						}
						
						m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002, m_iSelAddMoney);
					}

					//���¼�¼����λ��
					m_ptMouse.x=pMessage->_mouseX;
					m_ptMouse.y=pMessage->_mouseY;
				}
			}
			else
			{
				
				m_bCanMove = false;
				//SetImageSrcX(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,0);
			}
			
			break;
		}
	default:
		break;
	}

	return 0;
}
//�������������Ϣ
void CNewGameClient::OnUIMess_CATOONFINISH(UINT32 uControlID)
{
	switch(uControlID)
	{
	case CTN_1000000_CTN_1015000_MOVE_10150001:
		{
			m_MyUI.ShowControl(CTN_1000000_CTN_1015000_NoMoveCard_10150011, false);
			int ix = 0;
			int iy = 0;
			m_MyUI.GetControlGameXY(CTN_1000000_CTN_1015000_NoMoveCard_10150011, ix, iy);

			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				int _ix = 0;
				int _iy = 0;

				m_MyUI.GetControlGameXY(CTN_1000000_CTN_1015000_NoMoveCard_10150021 + i, _ix, _iy);
				int iw = 0;
				int ih = 0;
				m_MyUI.GetControlGameWH(CTN_1000000_CTN_1015000_NoMoveCard_10150021 + i, iw, ih);

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
	case CTN_1000000_CTN_1012000_MOVE_10120001://�����ƶ�ͼƬ
		{
			m_MyUI.ShowControl(CTN_1000000_CTN_1012000_IMG_10120012, false);
			int ix = 0;
			int iy = 0;
			m_MyUI.GetControlGameXY(CTN_1000000_CTN_1012000_IMG_10120012, ix, iy);

			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				int _ix = 0;
				int _iy = 0;

				m_MyUI.GetControlGameXY(CTN_1000000_CTN_1012000_IMG_10120021 + i, _ix, _iy);

				if (_ix == ix && _iy == iy)
				{
					//��ʾ����µ�ע
					ShowPlayerNote(m_pGameFrame->ViewStation2DeskStation(i),true);
					break;
				}
			}
		}
		break;
	case CTN_1000000_CTN_1012000_MOVE_10120002://�����ƶ�����
		{
			m_MyUI.ShowControl(CTN_1000000_CTN_1012000_TEXT_10120011, false);
			int ix = 0;
			int iy = 0;
			m_MyUI.GetControlGameXY(CTN_1000000_CTN_1012000_TEXT_10120011, ix, iy);

			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				int _ix = 0;
				int _iy = 0;

				m_MyUI.GetControlGameXY(CTN_1000000_CTN_1012000_TEXT_10120031 + i, _ix, _iy);

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
			m_pGameFrame->KillTimer(ID_TIME_AUTOSTART);
			m_bAutoStart=false;
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
	case CTN_1000000_CTN_1012000_BTN_10120041:///<��Ť1��
		{
			OnHitActiveResult(TYPE_FOLLOW, 0);
		}
		break;
	case CTN_1000000_CTN_1012000_BTN_10120042:///<��Ť2ȫѺ
		{
			OnHitActiveResult(TYPE_SHUTTLE, 0);
		}
		break;
	case CTN_1000000_CTN_1012000_BTN_120120040:///<��Ť3��ע
	case CTN_1000000_CTN_1012000_BTN_120120041:///<��Ť4��ע
	case CTN_1000000_CTN_1012000_BTN_120120042:///<��Ť5��ע
	case CTN_1000000_CTN_1012000_BTN_120120043:
		{
			int itag = uControlID - CTN_1000000_CTN_1012000_BTN_120120040;
			if(m_bNewTurn)
			{
				OnSelectNote(m_iAddButtonNum[itag]);
			}
			else
			{
				OnSelectNote(m_iTempBt[itag]);
			}
			
		}
		break;
	case CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002:	//�϶���ע
		{
			if (m_iSelAddMoney == m_iLimitNote)
			{
				OnHitActiveResult(TYPE_SHUTTLE, 0);
			}
			else
			{
				if(m_iSelAddMoney>0)
				{
					OnSelectNote(m_iSelAddMoney);
				}
				
			}
			break;
		}
	case CTN_1000000_CTN_1012000_BTN_10120046:///<��Ť6����
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
		{
			break;
		}
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
	int iView = ViewStation(nIndex);
	m_MyUI.ShowControl(CTN_1000000_CTN_1014000_IMG_10140001 + iView, false);//׼����־
	SetControlVisible(CTN_1000000_CTN_120120073+iView,false);
	SetControlVisible(CTN_1000000_IMG_120120047+iView,false);
	SetControlVisible(CTN_1000000_TEXT_120120052+iView,false);
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
	case ID_TIME_AUTOSTART://�Զ���ʼ��Ϣ
		{
			m_pGameFrame->KillTimer(ID_TIME_AUTOSTART);
			KillAllTimer();
			m_pGameFrame->ClickContinue();
			InitGameData();
			InitGameUI();
			break;
		}
	case ID_TIME_TIP://��ʾ
		{
			m_pGameFrame->KillTimer(ID_TIME_TIP);
			//��ʾ����
			for(int i=0;i<5;i++)
			{
				SetControlVisible(CTN_1000000_CTN_120120062+i,false);
			}
			break;
		}
	case ID_TIME_SEE_CARD:
		{
			m_pGameFrame->KillTimer(ID_TIME_SEE_CARD);
			BYTE icard[5] = {0};
			CopyMemory(icard,&m_DeskCard[m_bMyDesk],sizeof(BYTE)*5);
			ShowHandCard(m_bMyDesk, icard, m_iDeskCardCount[m_bMyDesk], true);	
		}
		break;
	case ID_BEGIN_TIME:
		{
			m_pGameFrame->KillTimer(ID_BEGIN_TIME);
			if (!m_bIsWatching)
			{
				m_pGameFrame->ClickContinue();
			}
			InitGameData();
			InitGameUI();
			break;
		}
	case ID_END_TIME:
		{
			m_pGameFrame->KillTimer(ID_END_TIME);
			InitGameData();
			InitGameUI();
			m_pGameFrame->CloseClient();
			break;
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
			OutputDebugString("dwjsh::ID_TIME_SEND_CARD��0��");
			if (m_iSendCardTotle >= m_iNeedSendCardTotle)//������
			{
				m_bSendCard = false;
				break;
			}
			OutputDebugString("dwjsh::ID_TIME_SEND_CARD��1��");
			if (!IsPlayer(m_iSendAnimaCardPos) || m_iUserStation[m_iSendAnimaCardPos] == TYPE_GIVE_UP)//��λ������Ҳ����� �� �Ѿ����ƣ���Ѹ�������¼ҷ���
			{
				OutputDebugString("dwjsh::ID_TIME_SEND_CARD��2��");
				m_iSendAnimaCardPos = (m_iSendAnimaCardPos - 1 + PLAY_COUNT) % PLAY_COUNT;//��ʱ�뷢��
				m_pGameFrame->SetTimer(ID_TIME_SEND_CARD, 20);
				break;
			}
			OutputDebugString("dwjsh::ID_TIME_SEND_CARD��3��");
			PlaySound(SOUND_SEND_CARD);
			int iView = ViewStation(m_iSendAnimaCardPos);
			BYTE icard[1] = {0};
			icard[0] = m_DeskCard[m_iSendAnimaCardPos][m_iSendCardNum[m_iSendAnimaCardPos]];
			m_iSendCardNum[m_iSendAnimaCardPos]++;
			m_MyUI.PlayCardInAnima(CTN_1000000_CTN_1015000_MOVE_10150001, 
				CTN_1000000_CTN_1015000_MOVE_10150001, 
				CTN_1000000_CTN_1015000_NoMoveCard_10150021, 
				CTN_1000000_CTN_1015000_NoMoveCard_10150011, 
				iView, 
				icard, 
				1, 
				200, 
				true);
			OutputDebugString("dwjsh::ID_TIME_SEND_CARD��4��");
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
	m_iSelAddMoney = 0;
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
		m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + i, false);//��ʱ��

		m_MyUI.ShowControl(CTN_1000000_CTN_1014000_IMG_10140001 + i, false);//׼����־
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


	for (int i = 0; i< 6; ++i)
	{
		m_MyUI.ShowControl(CTN_1000000_CTN_1012000_BTN_10120041 + i, true);
	}


	m_ptMouse.x=685;
	m_ptMouse.y=640;
	m_bCanMove = false;
	SetImagePosition(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,685,640);
	m_crMoveRect.SetRectRgn(685,640,760,640+27);
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,false);

	for(int i=0;i<10;i++)
	{
		SetControlVisible(CTN_1000000_IMG_120120047+i,false);
	}
	SetControlVisible(CTN_1000000_IMG_120120045,false);
	SetControlVisible(CTN_1000000_TEXT_120120046,false);
	SetControlVisible(CTN_1000000_CTN_1001000_BTN_1001100,false);
	//��ʾ����
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetControlVisible(CTN_1000000_CTN_120120062+i,false);
	}
	for(int i=0; i<PLAY_COUNT; i++)
	{
		SetControlVisible(CTN_1000000_CTN_120120073+i,false);
	}
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,false);
	SetImageOnMoseMove(CTN_1000000_IMG_120120088,false);
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
	m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + iView, true);
	m_MyUI.SetClock(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + iView, m_iThinkTime, true);

	return;

}

/// ������ʱ��
void CNewGameClient::KillGameTimer(BYTE bDesk)
{
	m_pGameFrame->KillTimer(ID_TIME_THINK);
	int iView = ViewStation(bDesk);
	m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + iView, false);
	m_MyUI.SetClock(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + iView, 0, false);

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
		m_MyUI.ShowControl(CTN_1000000_CTN_1001000_IMG_1001021 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_1001000_TEXT_1001031 + i, false);
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
			m_MyUI.SetTextInfo(CTN_1000000_CTN_1001000_TEXT_1001001 + iDesk, UserInfo.szNickName, true);

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
			sprintf(TcNickName, "%s/resources/result/win.png", CBcfFile::GetAppPath());
		}
		else if(szResultInfo.iMoney[i] < 0)
		{
			sprintf(TcNickName, "%s/resources/result/lost.png", CBcfFile::GetAppPath());
		}
#endif

		m_MyUI.SetImagesLoadPic(CTN_1000000_CTN_1001000_IMG_1001021 + iDesk, TcNickName, true);

		//����
		m_MyUI.SetTextInfo(CTN_1000000_CTN_1001000_TEXT_1001031 + iDesk, szResultInfo.iWardPoint[i], true,true);
		//���
		m_MyUI.SetTextInfo(CTN_1000000_CTN_1001000_TEXT_1001041 + iDesk, szResultInfo.iMoney[i], true,true);

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

	m_MyUI.ShowControl(CTN_1000000_CTN_1001000, false);

	m_pGameFrame->SetTimer(ID_END_TIME,m_iBeginTime*1000);
	m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + ViewStation(m_bMyDesk), true);
	m_MyUI.SetClock(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + ViewStation(m_bMyDesk), m_iBeginTime, true);
  
}

//���÷�����Ϣ
void CNewGameClient::ShowRoomInfo()
{
	m_MyUI.ShowControl(CTN_1000000_CTN_1011000, true);

	m_MyUI.SetTextInfo(CTN_1000000_CTN_1011000_TEXT_10110001, m_iBaseNote, true);
	//m_MyUI.SetTextInfo(CTN_1000000_CTN_1011000_TEXT_10110002, m_iLimitNote, true);
	return;
}

//���ð�ť����״̬
void CNewGameClient::ShowNoteBtnEnable(bool bFollow,bool bAllNote,bool bNote,bool bGiveUp)
{
	for (int i = 0; i < 6; ++i)
	{
		m_MyUI.ShowControl(CTN_1000000_CTN_1012000_BTN_10120041 + i, true);
	}
	
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_10120041,bFollow);
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_10120042,bAllNote);

	ShowNoteButText(bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_120120040,bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_120120041,bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_120120042,bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_120120043,bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,bNote);
	//SetImageOnMoseMove(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121001,bNote);
	//SetImageMoseThough(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,bNote);
	//m_bCanMove=bNote;
	
	if(bNote)
	{
		SetImagePosition(CTN_1000000_CTN_1012000_CTN_1012100_IMG_10121003,685,640);
	}
	SetImageOnMoseMove(CTN_1000000_IMG_120120088,bNote);
	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,bNote);

	m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_10120046,bGiveUp);
	
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
void CNewGameClient::OnSelectNote(__int64 i64Num)
{
	if(m_bNewTurn)
	{
		if (i64Num<m_iBaseNote || i64Num > m_iLimitNote)
		{
			return;
		}
	}
	
	tagUserProcess process;
	memset(&process,NULL,sizeof(process));

	//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ) ��Ϊ��Ϊ__int64��lparam�޷����ɶ��޸�  xyh:20110729
	/*switch(itag)
	{
	case 2:     //��������ע��ť
		{
			process.iNote = m_iAddButtonNum[2];//4*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
			break;
		}
	case 1:     //�ڶ�����ע��ť
		{
			process.iNote =  m_iAddButtonNum[1];//2*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
			break;
		}
	case 0:     //��һ����ע��ť
		{	
			process.iNote =  m_iAddButtonNum[0];//1*m_iBaseNote + GetMaxNote(m_iThisTurnNote);
			break;
		}
	default:
		break;
	}*/
	process.iNote = i64Num;

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
#ifdef UI2_5D
	sprintf(pchar, "%s/resources/ready.png", CBcfFile::GetAppPath());
#else
	sprintf(pchar, "%s/resources/ready.png", CBcfFile::GetAppPath());
#endif
	
	m_MyUI.SetImagesLoadPic(CTN_1000000_CTN_1014000_IMG_10140001 + iView, pchar, true);

	if (bDeskStation == m_bMyDesk)
	{	
		SetControlVisible(CTN_1000000_CTN_1001000_BTN_1001100,false);
		m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + ViewStation(m_bMyDesk), false);
		m_MyUI.SetClock(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + ViewStation(m_bMyDesk), 0, false);
		m_MyUI.ShowControl(CTN_1000000_BTN_10001, false);
		m_pGameFrame->KillTimer(ID_BEGIN_TIME);
		m_pGameFrame->KillTimer(ID_END_TIME);
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
#ifdef UI2_5D
	sprintf(pchar, "%s/resources/pass.png", CBcfFile::GetAppPath());
#else
	sprintf(pchar, "%s/resources/pass.png", CBcfFile::GetAppPath());
#endif
	
	m_MyUI.SetImagesLoadPic(CTN_1000000_CTN_1014000_IMG_10140001 + iView, pchar, true);

	return;
}

//��Ϸ��ʼʱ��ʼ������
void CNewGameClient::ShowBegin()
{
	for (int i = 0; i< PLAY_COUNT; ++i)
	{
		m_MyUI.ShowControl(CTN_1000000_CTN_1014000_IMG_10140001 + i, false);//׼����־
		m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001 + i, false);
		m_MyUI.ShowControl(CTN_1000000_CTN_1010000_CTN_1010001_Time_10100001 + i, false);//��ʱ��
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
	//m_MyUI.SetTextInfo(CTN_1000000_CTN_1012000_TEXT_10120050, m_iNoteTotalPoint, bFlag,false, AlignmentCenter);
	return;
}

//�����ע����
void CNewGameClient::ShowNoteAnima(BYTE bDeskStation, __int64 iNote, int iTime)
{
	int bView = ViewStation(bDeskStation);
	int iMoveActionID[2] = {CTN_1000000_CTN_1012000_MOVE_10120001,CTN_1000000_CTN_1012000_MOVE_10120002};
	int iEndPointID[2] = {CTN_1000000_CTN_1012000_IMG_10120021,CTN_1000000_CTN_1012000_TEXT_10120031};
	int iBeMoveID[2] = {CTN_1000000_CTN_1012000_IMG_10120012,CTN_1000000_CTN_1012000_TEXT_10120011};

	m_MyUI.SetTextInfo(CTN_1000000_CTN_1012000_TEXT_10120011, iNote, false, false,AlignmentCenter);

	m_MyUI.PlayChipInAnima_Image(iMoveActionID, 
		CTN_1000000_CTN_1015000_NoMoveCard_10150021, 
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
	m_MyUI.SetTextInfo(CTN_1000000_CTN_1012000_TEXT_10120031 + iView, m_iPerJuTotalNote[bDeskStation], bFlag, false,AlignmentCenter);
	m_MyUI.ShowControl(CTN_1000000_CTN_1012000_IMG_10120021 + iView, bFlag);
	
	return;
}

//��ʾ�������
void CNewGameClient::ShowHandCard(BYTE bDeskStation, BYTE iCard[], int iCount, bool bFlag)
{
	int iView = ViewStation(bDeskStation);

	m_MyUI.SetCardInfo(CTN_1000000_CTN_1015000_NoMoveCard_10150021 + iView, iCard, iCount, bFlag);
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
				m_MyUI.ShowControl(CTN_1000000_CTN_1014000_IMG_10140011 + iView, true);
			}
			else
			{
				m_MyUI.ShowControl(CTN_1000000_CTN_1014000_IMG_10140011 + iView, false);
			}
		}
	} 
	else
	{
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			m_MyUI.ShowControl(CTN_1000000_CTN_1014000_IMG_10140011 + i, false);
		}
	}

	return;
}

//��ʾ��ע��ť�ϵ���Ŀ
void CNewGameClient::ShowNoteButText(bool bFlag)
{
	if (bFlag)
	{
		if(m_bNewTurn)
		{
			for (int i = 0; i < 4; ++i)
			{
				m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_BTN_120120040 + i, m_iAddButtonNum[i]);
				m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_120120040 + i,true);
			}
			m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002, m_iSelAddMoney);
			m_MyUI.EnableControl(CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002,true);
		}
		else
		{
			__int64 i64Temp=m_i64HalfLessMoney-m_iPerJuTotalNote[m_pGameFrame->GetMyDeskStation()];
			m_iTempBt[3] = (__int64)(i64Temp/1.0);
			m_iTempBt[2] = (__int64)(i64Temp/2.0);
			m_iTempBt[1] = (__int64)(i64Temp/3.0);
			m_iTempBt[0] = (__int64)(i64Temp/4.0);
			CString cs;
			cs.Format("lwlog::i64Temp=%d",i64Temp);
			OutputDebugString(cs);
			for (int i = 0; i < 4; ++i)
			{
				m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_BTN_120120040 + i, (m_iTempBt[i]>0)?m_iTempBt[i]:0);
				m_MyUI.EnableControl(CTN_1000000_CTN_1012000_BTN_120120040 + i,(m_iTempBt[i]>0)?true:false);
			}
			m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002, 0);
		}
		
	} 
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_BTN_120120040 + i, 0);
		}
		m_MyUI.SetTextForBut( CTN_1000000_CTN_1012000_CTN_1012100_BTN_10121002, 0);
	}

	return;
}

//������ע��ť�ϵ���Ŀ
void CNewGameClient::SetNoteButText(__int64 iBaseNote)
{
	//modified by yjp.��ע��ť���ݵ�ע������������
	__int64 ibt[4] = {0};
	ibt[0] = (__int64)(iBaseNote*1.0);//(DOUBLE)m_iLimitNote*1.5/100;
	ibt[1] = (__int64)(iBaseNote*2.0);//(DOUBLE)m_iLimitNote*3/100;
	ibt[2] = (__int64)(iBaseNote*3.0);//(DOUBLE)m_iLimitNote*3/100;
	ibt[3] = (__int64)(iBaseNote*4.0);//(DOUBLE)m_iLimitNote*6/100;
	if (ibt[0]<= 0)
	{
		ibt[0] = 1;
		ibt[1] = 2;
		ibt[2] = 3;
		ibt[2] = 4;
	}

	CopyMemory(m_iAddButtonNum, ibt, sizeof(ibt));

	return;
}



//���������Ӧ
void	CNewGameClient::SetButtonOnMoseMove(const int iID, bool bFlag)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(iID));
	if (pButton != NULL)
	{
		pButton->SetMouseMove(bFlag);
	}
}


//���������Ӧ
void	CNewGameClient::SetImageOnMoseMove(const int iID,bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pIUI->GetIControlbyID(iID));
	if (pImage != NULL)
	{
		pImage->SetMouseMove(bFlag);
	}
}

//��ȡͼƬ��λ��
POINT	CNewGameClient::GetImagePosition(const int iID)
{
	POINT ptImage;
	memset(&ptImage,0, sizeof(ptImage));
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pIUI->GetIControlbyID(iID));
	if (pImage != NULL)
	{
		ptImage.x = pImage->GetGameX();
		ptImage.y = pImage->GetGameY();
	}
	return ptImage;
}

//����ͼƬλ��
void	CNewGameClient::SetImagePosition(const int iID, int iX, int iY)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pIUI->GetIControlbyID(iID));
	if (pImage != NULL)
	{
		pImage->SetGameXY(iX, iY);
	}
}

//������괩͸
void	CNewGameClient::SetImageMoseThough(const int iID,bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pIUI->GetIControlbyID(iID));
	if (pImage != NULL)
	{
		pImage->SetMouseThough(bFlag);
	}
}

//����ͼƬ���ؿ��
void	CNewGameClient::SetImageSrcX(const int iID,int isrcX)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pIUI->GetIControlbyID(iID));
	if (pImage != NULL)
	{
		pImage->SetSrcX(isrcX);
	}
}
//���ÿؼ��Ƿ�ɼ�
void CNewGameClient::SetControlVisible(const int iControlID,bool bFlag)
{
	IBCInterFace  *pControl = NULL ; 
	pControl = dynamic_cast<IBCInterFace *>(m_pIUI->GetIControlbyID(iControlID)); 
	if(NULL != pControl)
	{
		pControl->SetControlVisible(bFlag) ; 
	}
}
//�����ı��ؼ���ʾ����(��ʾ����)
void	CNewGameClient::SetTextinfo(const int iTextID,__int64 iNum, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pIUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%I64d",iNum);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����(��ʾ�ַ�)
void	CNewGameClient::SetTextShowText(const int iTextID,TCHAR ch[], bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pIUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%s",ch);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
void CNewGameClient::SetPicSize(const int iButtonID,CString sImagSrc, bool bFlag,int x,int y,int w,int h)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pIUI->GetIControlbyID(iButtonID));
	if (NULL != pImage)
	{
		pImage->LoadPic(CA2W(sImagSrc));
		pImage->SetSrcX(x); 
		pImage->SetSrcY(y); 
		pImage->SetSrcW(w); 
		pImage->SetSrcH(h); 
		pImage->SetControlVisible(bFlag);
	}
}
//�������ֿռ���ʾ����
void	CNewGameClient::SetInumInfo(const int iNumID, wchar_t * pchar,__int64 i64Num,bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pIUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		pNum->SetImagePath(pchar);
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//���õ�ѡ��ؼ��Ƿ�ѡ��
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pIUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		pRadioButton->SetIsSelect(bSelected);
	}
}

//--------------------------------------------------------------------------------------------------------
//��ȡ��ѡ��ؼ��Ƿ�ѡ��
BOOL	CNewGameClient::GetRButtonSelected(const int iRButtonID,bool *bSelected)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pIUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		*bSelected=pRadioButton->GetIsSelect();
		return (*bSelected);
	}
	return TRUE;
}