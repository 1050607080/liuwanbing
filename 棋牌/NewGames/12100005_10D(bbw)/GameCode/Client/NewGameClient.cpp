#include "stdafx.h"
#include "NewGameClient.h"
//#include "../Common/IGameFrame.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "GameControlsIDDefine.h"
#include "Common/UIMessageDefines.h"

static  bool  AIChooseCard = false ;   

void MsgPrint(const char *lpszFormat, ...)
{
	va_list pArg;
	va_start(pArg, lpszFormat);
	char szMessage[1024] = { 0 };
#ifdef WIN32
	_vsnprintf(szMessage, 1023, lpszFormat, pArg);
#else
	vsnprintf(szMessage, 1023, lpszFormat, pArg);
#endif
	va_end(pArg);
	OutputDebugString(szMessage);
}

IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
	return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
	delete *ppImp;
	*ppImp = NULL;
}
//���캯��
CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;

	m_SoundEngineGame = createIrrKlangDevice();
	m_SoundEnginBg = createIrrKlangDevice() ; 
	InitialGameData() ; 
}
//��������
CNewGameClient::~CNewGameClient()
{

}
//-----------------------------------------------------------------------------------
/// ��ʼ��
int CNewGameClient::Initial()
{
	//����UI
	wchar_t wszUIFile[MAX_PATH];
#ifdef NOT_FLAT
	::swprintf_s(wszUIFile,L"UI_%d_10D_3D.dat");
#else
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
#endif
	m_pUI->LoadData(wszUIFile);
	return 0;
}
//-----------------------------------------------------------------------------------
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	//���س���
	ShowSuperSet(false);
	ResetGameDialog();

	////���˰�ť
	//SetBtVisible(CTN_1014089_BTN_2004000,true);
	//SetBtEnable(CTN_1014089_BTN_2004000,true);
	
	return 0;
}
//---------------------------------------------------------------------------------------
//����UI���� 
void	CNewGameClient::ResetGameDialog()
{
	//����������;
	for(int i=0;i<5;i++)
	{
		SetTextinfo(CTN_1014089_CTN_1004100_TEXT_2000214+i,0,true);
		SetTextinfo(CTN_1014089_CTN_1004100_TEXT_2000218+i,0,true);
		SetTextShowText(CTN_1014089_CTN_1004100_TEXT_2000210+i,"",true);
		//ʱ��
		SetBtVisible(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+i,false);
		//׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+i,false);
		//��ׯͼƬ
		SetImageVisible(CTN_1014089_IMG_2000132+i,false);
		//����ׯͼƬ
		SetImageVisible(CTN_1014089_IMG_2000137+i,false);	
		//ׯ��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004060+i,false);
		//<���밴Ť1��Ť
		SetBtVisible(CTN_1014089_CTN_2000180_BTN_2000198+i,false);
		//<����1 ���ֿؼ� 
		SetInumInfo(CTN_1014089_CTN_2000180_Num_2000202+i,0,false);
		//<��ע��ʾͼƬ
		SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+i,false);
		//��ע����
		SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,0,false);
	}
	//��ʼ��ť
	SetBtVisible(CTN_1014089_BTN_1004006,false);
	//<��ׯ��Ť 
	SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008002,false);
	//<������Ť
	SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008001,false);
	//<ͣ�ư�Ť
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000207,false);
	//<Ҫ�ư�Ť
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000209,false);
	//��ʾҪ�ƺ�ͣ�ư�ť
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
	//<������Ť
	SetBtVisible(CTN_1014089_CTN_1004100_BTN_2000231,false);
	//<�뿪��Ť
	SetBtVisible(CTN_1014089_CTN_1004100_BTN_2000232,false);

	//������ע����
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//�������ֿռ���ʾ����
		SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,0,false);
	}
	
	//�����Ʊ�
	SetImageVisible(CTN_1014089_IMG_1005003,false);
	
	//�ƿؼ�����
	SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation], false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003020,NULL,0,false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003030,NULL,0,false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003050,NULL,0,false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003060,NULL,0,false);
	

	//ʱ��λ�û�ԭ
	POINT point;
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000268));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,false,point.x,point.y);
	

	SetContainVisible(CTN_2000280_CTN_1003510,false);
	SetContainVisible(CTN_2000280,true);

	//���������Ŀ�ѡ��
	//SetTextShowText(CTN_2000280_CTN_100200_TEXT_100220,"��ǰδ����",true);
	//for(int i=0; i<PLAY_COUNT; i++)
	//{
	//	SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100201+i,false,false);
	//	SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100211+i,false,false);
	//}

	////���س��˰�ť
	SetBtVisible(CTN_1014089_BTN_2004000,false);
	SetBtEnable(CTN_1014089_BTN_2004000,false);
	///���س�����������
	ShowSuperSet(false);
}


/// ��ҽ���
int CNewGameClient::GameUserCome(void)
{
	
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation(); 
	return 0;
}
//--------------------------------------------------------------------------------------------------------
//���ð�ť�Ƿ����
void	CNewGameClient::SetBtEnable(const int iButtonID, bool bFlag)
{

	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetEnable(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//���ð�ť�Ƿ�ɼ�
void	CNewGameClient::SetBtVisible(const int iButtonID, bool bFlag)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetControlVisible(bFlag);
	}
}


//-------------------------------------------------------------------------------
/// ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_byGameStation = bGameStation ; 
}
//-------------------------------------------------------------------------------
///��ȡ��Ϸ״̬
BYTE CNewGameClient::GetStationParameter()
{
	return m_byGameStation;
	
}
//---------------------------------------------------------------------------------------
///��ʼ����Ϸ�е�����
void	CNewGameClient::InitialGameData()
{
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;

	m_iMyDeskStation = 255;		///����Լ�����Ϸλ��
	m_byNtStation	 = 255;		//ׯ��λ��

	m_bIsSuper=false;
	m_bSoundPlay = false;		//�Ƿ���Բ�������
	m_bSoundBgPlay = false;		//������Ч
	memset(m_iGameSet,1,sizeof(m_iGameSet));
	//memset(m_bIsSuper,false,sizeof(m_bIsSuper));
	////������Ϸ����
	ResetGameData();
	LoadGameSet();
}

//---------------------------------------------------------------------------------------
//��ȡ��Ϸ����
void	CNewGameClient::LoadGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	
	//������Ч
	m_bSoundBgPlay	= ("1" == f.GetKeyVal(key,"BgSound","1"));
	//��Ϸ��Ч
	m_bSoundPlay	= ("1" == f.GetKeyVal(key,"GameSound","1"));

}

//---------------------------------------------------------------------------------------
//������Ϸ����
void	CNewGameClient::SaveGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	

	IRadioButton *pRadioButton = NULL;
	//������Ч
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_2000280_CTN_1003510_RButton_1003501));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			//����û�в��� ��ô�����Ҫ��ʼ���ű���������
			if (!m_bSoundBgPlay)
			{
				m_bSoundBgPlay = true;
				PlayBgSound(true);
			}
			f.SetKeyValString(key,"BgSound","1");
		}
		else
		{
			//������ڲ��� ��ô��Ҫֹͣ��
			if (m_bSoundBgPlay)
			{
				m_bSoundBgPlay = false;
				if (NULL != m_SoundEnginBg)
				{
					m_SoundEnginBg->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"BgSound","0");
		}
	}
	//��Ϸ��Ч
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_2000280_CTN_1003510_RButton_1003502));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			m_bSoundPlay = true;
			f.SetKeyValString(key,"GameSound","1");
		}
		else
		{
			//������ڲ�����Ч ��ʱ��Ҫֹͣ
			if (m_bSoundPlay)
			{
				m_bSoundPlay = false;
				if (NULL != m_SoundEngineGame)
				{
					m_SoundEngineGame->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"GameSound","0");
		}
	}
}

//���泬������
void	CNewGameClient::SaveSuperSet()
{
	
}
//------------------------------------------------------------------------------
//��ʾ��Ϸ����
void	CNewGameClient::ShowGameSet(bool bShow)
{
	SetContainVisible(CTN_2000280_CTN_1003510,bShow);
	SetBtVisible(CTN_2000280_CTN_1003510_BTN_1003504,bShow);
	SetBtVisible(CTN_2000280_CTN_1003510_BTN_1003505,bShow);
	SetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003501,m_bSoundBgPlay,bShow);
	SetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003502,m_bSoundPlay,bShow);
}
//------------------------------------------------------------------------------
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	//��ť�������Ϣ
	if(pMessage->_uMessageType == UI_LBUTTONUP)
	{
		if(m_pGameFrame->GetIsWatching())
		{
			return 0;
		}
		switch(pMessage->_uControlID)
		{
		case CTN_1014089_BTN_1003500:	//���ð�ť
			{
				ShowGameSet(true);
				break;
			}
		case CTN_2000280_CTN_1003510_BTN_1003504:	//ȷ������
			{
				SaveGameSet();
				ShowGameSet(false);
				break;
			}
		case CTN_2000280_CTN_1003510_BTN_1003505:  //ȡ������
			{
				ShowGameSet(false);
				break;
			}
		case CTN_1014089_BTN_1004006://��ʼ��ť
			{
				OnControlHitBeginBt();
				break;
			}
		case CTN_1014089_CTN_1008000_BTN_1008002://��ׯ��ť
			{
				OnHandleHitNtBt();
				break;
			}
		case CTN_1014089_CTN_1008000_BTN_1008001://������ׯ��ť
			{
				OnHandleHitGiveUpNtBt();
				break;
			}
		case CTN_1014089_CTN_2000180_BTN_2000198://��ע
		case CTN_1014089_CTN_2000180_BTN_2000199:
		case CTN_1014089_CTN_2000180_BTN_2000200:
		case CTN_1014089_CTN_2000180_BTN_2000201:
			{
				//��ǰ��ť����
				int iHitButton = pMessage->_uControlID - CTN_1014089_CTN_2000180_BTN_2000198;
				OnHandleHitBetBt(iHitButton);
				break;
			}
		case CTN_1014089_CTN_2000206_BTN_2000209://Ҫ��
		case CTN_1014089_CTN_2000206_BTN_2000207://ͣ��
			{
				//��ǰ��ť����
				int iHitButton = pMessage->_uControlID - CTN_1014089_CTN_2000206_BTN_2000209;	
				OnHandleHitTakeCardBt(iHitButton);
				break;
			}
		case CTN_1014089_CTN_1004100_BTN_2000231: //<������Ť 
		case CTN_1014089_CTN_1004100_BTN_2000232: //<�뿪��Ť 
			{
				//��ǰ��ť����
				int iHitButton = pMessage->_uControlID - CTN_1014089_CTN_1004100_BTN_2000231;
				OnClickContinueBt(iHitButton);
				break;
			}
		case CTN_1014089_BTN_2004000:	//���˰�ť
			{
				//��ʾ��������
				ShowSuperSet(true);
				break;
			}
		
		case CTN_2000280_CTN_100200_BTN_100231:  //ȷ�����˰�ť
			{
				//���ͳ�������
				SuperUserSet();
				ShowSuperSet(false);
				break;
			}
		case CTN_2000280_CTN_100200_BTN_100232:   //ȡ�����˰�ť
			{
				m_bIsSuper = false;
				ShowSuperSet(false);
				break;
			}
		default:
			{
				break;
			}
		}
	}
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		switch(pMessage->_uControlID)
		{
		case CTN_2000128_ACTION_2000128:			//���ƶ����ؼ�1
		case CTN_2000129_ACTION_2000129:			//���ƶ����ؼ�2
		case CTN_2000130_ACTION_2000130:			//���ƶ����ؼ�3
		case CTN_2000131_ACTION_2000131:			//���ƶ����ؼ�4
		case CTN_2000132_ACTION_2000132:			//���ƶ����ؼ�5
		{
			OnSendCardFinish();
			break;
		}
		case CTN_1014089_CTN_200400_CTN_2004030_Time_2004031:	//�Լ��ĵ���ʱ
			{
				if (GetStationParameter()==GS_WAIT_SETGAME || GetStationParameter()==GS_WAIT_NEXT || GetStationParameter()==GS_WAIT_ARGEE)
				{
					ResetGameDialog();
					ResetGameData();				
					m_pGameFrame->CloseClient();
				}
				else
				{
					ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,false,0,0);
				}
				
			}
		}
	}
	return 0;
}

//���������Ϣ
void CNewGameClient::OnSendCardFinish()
{
	PlayGameSound(SOUND_XIAZHU,false);
	SetImageVisible(CTN_1014089_IMG_1005003,false);
	m_bySendCount[m_bySendCardPos]++;
	//���ƴ����ۼ�
	m_bySendCardCount++;	
	
	//��ʾ�˿�
	int iView = m_pGameFrame->DeskStation2View(m_bySendCardPos);
	if (2 == iView)
	{
			BYTE bTempList[5]={80,80,80,80,80};
			bTempList[0] = m_DeskReadyCard[m_bySendCardPos][0];
			SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, bTempList,m_bySendCount[m_bySendCardPos], true);
	}
	else
	{
			ShowCurrentNoMoveCard(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView,m_DeskReadyCard[m_bySendCardPos],m_bySendCount[m_bySendCardPos]);
	}
	
	//���ѷ����ƴ�˳��������ɾ��
	m_bySendCardPos = (m_bySendCardPos+1)%PLAY_COUNT;
	
	//�ж��Ƿ���Խ�������
	while (m_bySendCardCount < m_sendQueue.size())
	{
		if (m_sendQueue.at(m_bySendCardCount) != 0)
		{
			SendCard(m_sendQueue.at(0),m_bySendCardPos);
			break;
		}
		else
		{
			m_bySendCardCount++;
			m_bySendCardPos = (m_bySendCardPos+1)%PLAY_COUNT;
		}
	}
}
/*
���ܣ�����������뿪��ť��Ӧ����
�βΣ�iHitButton,��ť����
����ֵ��void
*/
void CNewGameClient::OnClickContinueBt(int iHitButton)
{
	if(0==iHitButton)//����
	{
		PlayGameSound(SOUND_HIT_BEGIN,false);
		ResetGameDialog();
		//����ʾ��ʼ��ť
		SetBtVisible(CTN_1014089_BTN_1004006,false);
		OnControlHitBeginBt();
	}
	else			//�뿪
	{
		m_pGameFrame->CloseClient();
	}
}

//���ܣ����ͣ�ơ�Ҫ�ư�ť������
void CNewGameClient::OnHandleHitTakeCardBt(int iHitButton)
{
	if(GS_PLAY_GAME!=m_byGameStation)
	{
		return;
	}
	if(0==iHitButton)//Ҫ��
	{
		PlayGameSound(SOUND_CHONGPAI,false);
		TakeCardStruct TUserTakeCard;
		TUserTakeCard.byReadyYaoCard[m_iMyDeskStation]=true;
		m_pGameFrame->SendGameData(&TUserTakeCard,sizeof(TUserTakeCard),MDM_GM_GAME_NOTIFY,ASS_USER_TAKE_CARD,0);
		
	}
	else			//ͣ��
	{
		StopCardStruct TUserStopCard;
		TUserStopCard.bReadyStopCard[m_iMyDeskStation]=true;
		m_pGameFrame->SendGameData(&TUserStopCard,sizeof(TUserStopCard),MDM_GM_GAME_NOTIFY,ASS_USER_STOP,0);

		m_pGameFrame->KillTimer(ID_TAKE_CARD) ;
		PlayGameSound(SOUND_CHONGPAI,false);
	}
}

/*
���ܣ������˷�����ע��Ϣ
�βΣ�iHitButton ,��ť����
����ֵ��void
*/
void CNewGameClient::OnHandleHitBetBt(int iHitButton)
{
	if(m_byGameStation!=GS_NOTE)
	{
		return;
	}
	if(iHitButton>3)//��עʱ�䵽�Զ�Ϊ�û���ע��Сֵ
	{
		tagUserProcess userBet;
		userBet.iNoteType= 3;
		userBet.iVrebType = TYPE_NOTE;				//��ע����
		m_pGameFrame->SendGameData(&userBet,sizeof(userBet),MDM_GM_GAME_NOTIFY,ASS_USER_NOTE,0);
		
	}
	else		//�û�������ע
	{
		tagUserProcess userBet;
		userBet.iNoteType= iHitButton;
		userBet.iVrebType = TYPE_NOTE;				//��ע����		
		m_pGameFrame->SendGameData(&userBet,sizeof(userBet),MDM_GM_GAME_NOTIFY,ASS_USER_NOTE,0);
	}	
}
/*
���ܣ������˷��ͷ�����ׯ��Ϣ
*/
void CNewGameClient::OnHandleHitGiveUpNtBt()
{
	CallScoreStruct TCallScore;
	TCallScore.iValue	      = 0;
	TCallScore.bDeskStation   = m_iMyDeskStation;			
	TCallScore.bCallScoreflag = false;

	//���ؽ�ׯ��ť
	SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008002,FALSE);
	//���ز���ׯ��ť
	SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008001,FALSE);
	//���ͽ�ׯ��Ϣ
	m_pGameFrame->SendGameData(&TCallScore,sizeof(TCallScore),MDM_GM_GAME_NOTIFY,ASS_USER_SCORE,0);
}

//���ܣ������˷��ͽ�ׯ��Ϣ
void CNewGameClient::OnHandleHitNtBt()
{
	CallScoreStruct callScore;
	callScore.iValue=1;
	callScore.bDeskStation = m_iMyDeskStation;			
	callScore.bCallScoreflag = false;
	//���ؽ�ׯ��ť
	SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008002,FALSE);
	//���ز���ׯ��ť
	SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008001,FALSE);
	//���ͽ�ׯ��Ϣ
	m_pGameFrame->SendGameData(&callScore,sizeof(callScore),MDM_GM_GAME_NOTIFY,ASS_USER_SCORE,0);
	//PlayGameSound(SOUND_JIAOZHUANG,false);
}
/*
���ܣ���ҵ����ʼ��ť�����Ϳ�ʼ��Ϣ
*/
void CNewGameClient::OnControlHitBeginBt()
{
	if (GetStationParameter()==GS_WAIT_SETGAME || GetStationParameter()==GS_WAIT_NEXT || GetStationParameter()==GS_WAIT_ARGEE)
	{
		//�Ŷӳ� �����Ŷ�
		/*if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
		{
			m_pGameFrame->JoinQueue();
		}
		else*/
		{
			m_pGameFrame->ClickContinue();
		}
		//�����˷��Ϳ�ʼ��Ϣ
		//m_pGameFrame->SendGameData(NULL,0,MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}
	
	PlayGameSound(SOUND_HIT_BEGIN,false);
	KillAllGameTime();
	//���ؿ�ʼ��ť
	SetBtVisible(CTN_1014089_BTN_1004006,FALSE);
	//��ʼ��ť������
	SetBtEnable(CTN_1014089_BTN_1004006,FALSE);
	m_pGameFrame->KillTimer(ID_BEGIN_TIME) ;//��ʼ��Ϸ�ȴ�ʱ�䵽
	m_pGameFrame->KillTimer(ID_WAIT_NEXT) ;//��ʼ��Ϸ�ȴ�ʱ�䵽
}

/*------------------------------------------------------------------------------*/
//����
/*
���ܣ�������ţ����
�βΣ�iHaveCattle����ţֵ
����ֵ��void
*/
void CNewGameClient::ShowHaveCattleAction(const int iImageID,int iCardShape,bool bShow,BYTE byDeskStation)
{
	CString imageSrc;
	switch(iCardShape)
	{
	case UG_BUST_CARD:									//ţ��0x05 
		{
			imageSrc.Format("./image/5Num.png");
			break;
		}
	case UG_NORMAL_CARD:									//ţ��0x06
		{
			imageSrc.Format("./image/6Num.png");
			break;
		}
	case UG_HALF_PAST_TEN:								//ţ��0x07
		{
			imageSrc.Format("./image/7Num.png");
			break;
		}
	case UG_FIVE_SMALL:								//ţ��0x08 
		{
			imageSrc.Format("./image/8Num.png");
			break;
		}
	case UG_HEAVEN_KING:								//ţ�� 0x09
		{
			imageSrc.Format("./image/9Num.png");
			break;
		}
	case UG_PERSON_SMALL_FIVE :								//ţţ0x0A
		{
			imageSrc.Format("./image/10Num.png");
			break;
		}
	default:
		{
			break;
		}
	}
	SetImageLoadSrc(iImageID,imageSrc,bShow);
}
//-------------------------------------------------------------------------------------------------------
//������������
void	CNewGameClient::PlayShapeSound(int iCardShape,bool bShow,BYTE byDeskStation)
{
	switch(iCardShape)
	{
	case UG_BUST_CARD:									//��ţ0x00
		{
			PlayGameSound(SOUND_NIUNIU_0,false,byDeskStation);
			break;
		}
	case UG_NORMAL_CARD:									//ţһ0x01
		{
			PlayGameSound(SOUND_NIUNIU_1,false,byDeskStation);
			break;
		}
	case UG_HALF_PAST_TEN:									//ţ��0x02
		{
			PlayGameSound(SOUND_NIUNIU_2,false,byDeskStation);
			break;
		}
	case UG_FIVE_SMALL:									//ţ�� 0x03
		{
			PlayGameSound(SOUND_NIUNIU_3,false,byDeskStation);
			break;
		}
	case UG_HEAVEN_KING:									//ţ��0x04
		{
			PlayGameSound(SOUND_NIUNIU_4,false,byDeskStation);
			break;
		}
	case UG_PERSON_SMALL_FIVE:									//ţ��0x05 
		{
			PlayGameSound(SOUND_NIUNIU_5,false,byDeskStation);
			break;
		}
	default:
		{
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------------
//��ʾ���˽������
void	CNewGameClient::ShowSuperSet(bool bFlag)
{
	//��ʾ��������
	SetContainVisible(CTN_2000280_CTN_100200,bFlag);

}
//���ͳ�����������
void	CNewGameClient::SuperUserSet()
{
// 	if(GetStationParameter() < GS_ROB_NT || GetStationParameter() >=GS_PLAY_GAME)
// 	{
// 		return;
// 	}

	m_bIsSuper = true;

	m_SuperSetData.byDeskStation = m_iMyDeskStation;
	m_SuperSetData.bSetSuccese   = true;

	m_pGameFrame->SendGameData(&m_SuperSetData,sizeof(m_SuperSetData),MDM_GM_GAME_NOTIFY,ASS_SUPER_USER_SET,0);

}

//--------------------------------------------------------------------------------------------------------
//���õ�ѡ��ؼ��Ƿ�ѡ��
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected,bool bShow )
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		pRadioButton->SetIsSelect(bSelected);
		pRadioButton->SetControlVisible(bShow);

	}
}
//--------------------------------------------------------------------------------------------------------
//��ȡ��ѡ��ؼ��Ƿ�ѡ��
bool	CNewGameClient::GetRButtonSelected(const int iRButtonID)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		return pRadioButton->GetIsSelect();
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------
//���ó���ؼ���ʾֵ
void	CNewGameClient::SetShCtrlInfo(const int iShCtrlID, int iNum, bool bFlag)
{
	IShCtrl *pShCtrl = NULL;
	pShCtrl = dynamic_cast<IShCtrl *>(m_pUI->GetIControlbyID(iShCtrlID));
	if (NULL != pShCtrl)
	{
	pShCtrl->SetShNote(iNum);
	pShCtrl->SetControlVisible(bFlag);
	}

}
//-----------------------------------------------------------------------------------------
//���ÿؼ�λ��
void   CNewGameClient::SetControlPos(const int iControlID,bool bFlag,int x,int y)
{
	IBCInterFace *pContain = NULL;
	pContain = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID));
	if (NULL != pContain)
	{
		pContain->SetGameXY(x,y); 
		pContain->SetControlVisible(bFlag);
	}
}
//-----------------------------------------------------------------------------------------
//���ÿɲ����ƿؼ�����
void	CNewGameClient::SetOperateCardInfo(const int iOperateCardID, BYTE byCardList[],BYTE byCardCount, bool bShow)
{
	IOperateCard *pOperateCard = NULL;
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iOperateCardID)); 
	if(NULL != pOperateCard)
	{
		pOperateCard->SetCardList(byCardList,byCardCount);
		pOperateCard->SetControlVisible(bShow);

	} 
}
//-----------------------------------------------------------------------------------------
//���ò��ɲ����ƿؼ�����
void	CNewGameClient::SetNoMoveCardInfo(const int iNoMoveCardID,BYTE byCardList[],BYTE byCardCount, bool bShow)
{
	INoMoveCard *pNoMoveCard = NULL;
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iNoMoveCardID)); 
	if(NULL != pNoMoveCard)
	{
		pNoMoveCard->SetCardList(byCardList,byCardCount);
		pNoMoveCard->SetControlVisible(bShow);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ�������ʾָ�����ļ�
void	CNewGameClient::SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag)
{
		
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->LoadPic(CA2W(sImagSrc));
		pImage->SetControlVisible(bFlag);
	}
}

//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ��Ƿ�ɼ�
void	CNewGameClient::SetImageVisible(const int iImageID, bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetControlVisible(bFlag);
	}
}
//-----------------------------------------------------------------------------------------
/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{	

	switch(nSubId)
	{
	case ASS_SUPER_USER:	//���˿ͻ���
		{  
			if(nLen!=sizeof(SUPERSTATE))
			{
				return 0;
			}	
			HandleSuperMessage(buffer);			
			break;
		}
	case ASS_SUPER_USER_SET_RESULT:	//�������ý��
		{
			if(nLen!=sizeof(SuperUserSetData))
			{
				return 0;
			}		
			SuperUserSetData * pSuperSet = (SuperUserSetData *)buffer;
			if (NULL == pSuperSet)
			{
				return 0;
			}
			memcpy(&m_SuperSetData, pSuperSet, sizeof(m_SuperSetData));
			ShowSuperSet(true);
			break;
		}
	case ASS_GM_GAME_STATION:   ///<����״̬(ƽ̨���õ�)
		{
			SetGameStation(buffer, nLen);    
			break;        
		}
	case ASS_GM_AGREE_GAME:			//ͬ����Ϸ
		{
			if(nLen!=sizeof(MSG_GR_R_UserAgree))
			{
				return 0;
			}	
			OnAgreeGame(buffer);
			break;
		}
	case ASS_CALL_SCORE:				//���ܽ�ׯ��Ϣ
		{
			if(nLen!=sizeof(CallScoreStruct))
			{
				return 0;
			}
			OnHandleCallScoreMessage(buffer);
			break;
		}
	case ASS_CALL_SCORE_RESULT:			//���ܽ�ׯ������Ϣ
		{
			if (nLen != sizeof(CallScoreStruct)) 
			{
				return 0;
			}
			OnHandleCallScoreResultMessage(buffer);
			break;
		}
	case ASS_CALL_SCORE_FINISH:			//���ܽ�ׯ������Ϣ
		{
			if (nLen != sizeof(CallScoreStruct)) 
			{
				return 0;
			}
			OnHandleCallScoreFinishMessage(buffer);
			break;
		}
	case ASS_CALL_NOTE:				//��ʼ��ע
		{
			if (nLen !=sizeof(UserNoteStruct))
			{
				return 0;
			}
			OnHandleBetBegin(buffer);
			break;
		}
	case ASS_CALL_NOTE_RESULT:			//��ע���
		{
			if (nLen != sizeof(NoteResult)) 
			{
				return 0;
			}
			OnHandleBetResult(buffer);
			break;
		}
	case ASS_CALL_SEND_CARD:				//��ʼ����
		{

			if(nLen != sizeof(SendAllCardStruct))
			{
				return FALSE;
			}
			
			OnHandleSendCardBegin(buffer);
			break;
		}
	case ASS_CALL_SEND_FINISH:		//���ƽ���
		{
			OnHandleSendFinish();
			break;
		}
	case ASS_GAME_PLAY:			//֪ͨ��ʼ��Ϸ��Ҫ�ơ�ͣ�ƣ�
		{
			if(nLen != sizeof(BeginPlayStruct))
			{
				return FALSE;
			}
			OnHandleGameBegin(buffer);
			break;
		}
	case ASS_TAKE_CARD_RESULT:			//Ҫ�ƽ������
		{
			if(nLen != sizeof(TakeCardStruct))
			{
				return FALSE;
			}
			OnHandleTakeCardResult(buffer);
			break;
		}
	case ASS_USER_STOP_RESULT:      //ͣ�ƽ������
		{
			OnHandleStopPaiResult(buffer);
			break;
		}
	case ASS_CONTINUE_END:		//��Ϸ����
		{
			if(nLen != sizeof(GameEndStruct))
			{
				return FALSE;
			}
			OnGameFinish(buffer);
			break;
		}
	
	case ASS_CUT_END:		//��ҵ���
		{
			if(nLen != sizeof(GameCutStruct))
			{

				return FALSE;
			}
			OnHandlePlayerCut(buffer);
			break;
		}
	case ASS_SALE_END:		//����˳�
		{
			if(nLen != sizeof(GameCutStruct))
			{
				return FALSE;
			}
			OnHandlePlayerSafeEnd(buffer);
			break;
		}
	
	default:
		{
			break;
		}

	}
	return 0;
}
//����ͬ����Ϸ��Ϣ
void CNewGameClient::OnAgreeGame(void *buffer)
{
	MSG_GR_R_UserAgree * agreeGame=(MSG_GR_R_UserAgree *)buffer;
	if(NULL==agreeGame)
	{
		return ;
	}
	
	//���Լ�
	if (m_iMyDeskStation == agreeGame->bDeskStation)
	{
		ResetGameDialog();
		KillAllGameTime();

		//���ؿ�ʼ��ť
		SetBtVisible(CTN_1014089_BTN_1004006,FALSE);
		//��ʼ��ť������
		SetBtEnable(CTN_1014089_BTN_1004006,FALSE);
		//ʱ�ӵ���ʱ
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,false,0);
	}
	//��λ��ת��ͼλ��
	int viewStation=m_pGameFrame->DeskStation2View(agreeGame->bDeskStation);
	//��ʾ׼��ͼƬ
	SetImageVisible(CTN_1014089_IMG_1004070+viewStation,TRUE);
}
/*
@brief��������������˳���Ϣ
*/
void CNewGameClient::OnHandlePlayerSafeEnd(void *buffer)
{
	GameCutStruct *playerSafeEnd = (GameCutStruct *)buffer ;
	if(NULL==playerSafeEnd)
	{

		return;
	}
}
/*
@brief���������˷���������Ϸ����˳���Ϣ
@param����ָ��
@return��void
*/
void CNewGameClient::OnHandlePlayerCut(void *buffer)
{
	GameCutStruct *playerCut = (GameCutStruct *)buffer ;
	if(NULL==playerCut)
	{

		return;
	}
	SetStationParameter(GS_WAIT_NEXT);
	//��ʾ��ʼ��ť���ص��ȴ���ʼ״̬
	KillAllGameTime();
	ResetGameData();
	ResetGameDialog();
}
/*
@brief���������û���Ϣ
@param����ָ��
@return��void
*/
void	CNewGameClient::HandleSuperMessage(void *buffer)
{

	SUPERSTATE *pSuperUser = (SUPERSTATE *) buffer;
	if (NULL == pSuperUser)
	{
		return ;
	}
	
	if(m_pGameFrame->GetIsWatching())
	{
		return;
	}

	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation(); 


	if(m_iMyDeskStation==pSuperUser->byDeskStation && pSuperUser->bEnable)
	{	
		//��ʾ���˰�ť
		SetBtVisible(CTN_1014089_BTN_2004000,true);
		SetBtEnable(CTN_1014089_BTN_2004000,true);
	}
}

//���ܣ��������˷���������ϷҪ�ƽ����Ϣ
void CNewGameClient::OnHandleTakeCardResult(void *buffer)
{
	TakeCardStruct *pUserTakeCardResult = (TakeCardStruct*)buffer ;
	if(NULL==pUserTakeCardResult)
	{
		return;
	}
	if(pUserTakeCardResult->byDeskStation != m_iMyDeskStation)
	{
		return ;
	}
	m_byOpenCardCount[m_iMyDeskStation] = pUserTakeCardResult->byYaoPaiCount+1;
	//���Ҫ��ʱ ��ʾ�˿�
	BYTE bTempList[5]={80,80,80,80,80};
	for(int i=0;i<=pUserTakeCardResult->byYaoPaiCount;i++)
	{		
		bTempList[i] = m_DeskReadyCard[pUserTakeCardResult->byDeskStation][i];		
	}
	SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, bTempList,m_iDeskCardCount[pUserTakeCardResult->byDeskStation], true);


	//Ҫ������
	//SetInumInfo(CTN_11112000_Num_12112201,m_byOpenCardCount[m_iMyDeskStation],true);


	if(pUserTakeCardResult->byReadyYaoCard[m_iMyDeskStation] == false)
	{
		m_pGameFrame->KillTimer(ID_TAKE_CARD);
		SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000209,false);
		SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000207,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false); 
	}
	
	//��ʾ������ʾ
//	ShowHaveCattleAction(CTN_1014089_CTN_200300_IMG_2003023+iView,pUserTakeCardResult->iShape,true,pUserTakeCardResult->byDeskStation);	
	//PlayShapeSound(pUserTakeCardResult->iShape,true,pUserTakeCardResult->byDeskStation);

}
/*
���ܣ��������˷���������Ϸͣ�ƽ����Ϣ
*/
void CNewGameClient::OnHandleStopPaiResult(void *buffer)
{

	StopCardStruct *pUserStopCardResult = (StopCardStruct *)buffer ;
	if(NULL==pUserStopCardResult)
	{
		return;
	}
	if(pUserStopCardResult->byDeskStation != m_iMyDeskStation)
	{
		return;
	}
	//������Ϸ����״̬
	SetStationParameter(GS_PLAY_FINISH);
	//����Ҫ�ƺ�ͣ�ư�ť������	
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000209,false);
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000207,false); 
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false); 	

	PlayShapeSound(pUserStopCardResult->iShape,true,pUserStopCardResult->byDeskStation);
	KillAllGameTime();
	//m_pGameFrame->KillTimer(ID_TAKE_CARD);
	
}
/*
���ܣ��������˷���������Ϸ������Ϣ
*/
void CNewGameClient::OnGameFinish(void *buffer)
{
	CString lhx;
	lhx.Format("lihexing::����");
	OutputDebugString(lhx);
	GameEndStruct *pGameFinish = (GameEndStruct *)buffer ;
	if(NULL==pGameFinish)
	{
		return;
	}
	
	SetStationParameter(GS_WAIT_NEXT);

	//��ʾ�˿���
	for(int i = 0x00;i < PLAY_COUNT;i++)
	{
		int iView = m_pGameFrame->DeskStation2View(i);
		if(2 == iView)
		{
			BYTE bTempList[5] = {80,80,80,80,80};
			for(int j = 0x00;j < pGameFinish->byOpenCardCount[i];j++)
			{
				bTempList[j] = m_DeskReadyCard[i][j];
			}

			SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, bTempList,m_iDeskCardCount[i], true);
		}
		else
		{
			BYTE bTempList[5] = {0,0,0,0,0};
			for(int j = 0x00;j < pGameFinish->byOpenCardCount[i];j++)
			{
				bTempList[j] = m_DeskReadyCard[i][j];
			}
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView, bTempList,m_iDeskCardCount[i], true);
		}
	}			

	//��ʾ��������
	ShowResultDlg(pGameFinish,true);

	if(pGameFinish->iChangeMoney[m_iMyDeskStation]>=0)
	{
		PlayGameSound(SOUND_WIN,false);
	}
	else
	{
		PlayGameSound(SOUND_LOSE,false);
	}

	//ʱ��λ�û�ԭ
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000268));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,point.x,point.y);

	//��ʾ��ʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iBeginTime);

	//�����ȴ���ʼ��ʱ��
	m_bCurrentClockLabel=m_iBeginTime;
	//�������ƶ�ʱ��
	m_pGameFrame->SetTimer(ID_WAIT_NEXT,1000);
	//��ʾ������Ϸ���뿪��ť
	SetBtVisible(CTN_1014089_CTN_1004100_BTN_2000231,true);
	SetBtEnable(CTN_1014089_CTN_1004100_BTN_2000231,true);
	SetBtVisible(CTN_1014089_CTN_1004100_BTN_2000232,true);
	SetBtEnable(CTN_1014089_CTN_1004100_BTN_2000232,true);
}
/*--------------------------------------------------------------------------------------*/
//��ʾ�����
void	CNewGameClient::ShowResultDlg(GameEndStruct *pGameFinish,bool bShow)
{
	if (NULL == pGameFinish)
	{
		return;
	}

	UserInfoForGame UserInfo;
	memset(&UserInfo,0,sizeof(UserInfo));
	for(int i=0; i<PLAY_COUNT; i++)
	{	
		//����״̬�����
		if (pGameFinish->iUserState[i] != -1 && m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			//��λ��ת��ͼλ��
			int iView=m_pGameFrame->DeskStation2View(i);
			//if(m_iTotalGameNote[i]>0)
			{
				SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,true);
				SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,pGameFinish->i64TurePoint[i],true);
				SetINumSingle(CTN_1014089_CTN_2000246_Num_2000251+iView,pGameFinish->i64TurePoint[i],true);	
			}
		}
	}
}
/*--------------------------------------------------------------------------------------*/

//���ܣ��������˷���������Ϸ��ʼ��Ϣ
void CNewGameClient::OnHandleGameBegin(void *buffer)
{
	BeginPlayStruct *gameBegin = (BeginPlayStruct *)buffer ;
	if(NULL==gameBegin)
	{
		return;
	}
	SetStationParameter(GS_PLAY_GAME);

	//��ʾҪ�ƺ�ͣ�ư�ť     
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000209,true);
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000207,true);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,true);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,true);       

	//ʱ��λ�øı�
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000267));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,point.x,point.y);
	
	//������ʱ��
	m_bCurrentClockLabel=m_iThinkTime;
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iThinkTime);
	//�������ƶ�ʱ��
	m_pGameFrame->SetTimer(ID_TAKE_CARD,1000);
}
/*
���ܣ��������˷������ķ��ƽ�����Ϣ
*/
void CNewGameClient::OnHandleSendFinish()
{
	//��ʾҪ�ƺ�ͣ�ư�ť     
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000209,true);
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000207,true);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,true);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,true);

	//��ʾ�˿�
	if(m_bIsSuper)   //����ǳ���
	{
		int iView = 0;
		for(int i=0; i<PLAY_COUNT; i++)
		{
			iView = m_pGameFrame->DeskStation2View(i);
			if (2 == iView)
			{									
				BYTE bTempList[5]={80,80,80,80,80};
				for(int j=0; j<5; j++)
				{
					bTempList[j] = m_DeskReadyCard[i][j];
				}				
				SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, bTempList,m_iDeskCardCount[i], true);
			}
			else
			{
				BYTE bTempList[5]={0,0,0,0,0};
				for(int j=0; j<5; j++)
				{
					bTempList[j] = m_DeskReadyCard[i][j];
				}			
				SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView, bTempList,m_iDeskCardCount[i], true);
			}
		}
	}
	else       //���ǳ���
	{		
		int iView = 0;
		for(int i=0; i<PLAY_COUNT; i++)
		{
			iView = m_pGameFrame->DeskStation2View(i);
			if (2 == iView)
			{									
				BYTE bTempList[5]={80,80,80,80,80};
				bTempList[0] = m_DeskReadyCard[m_iMyDeskStation][0];
				SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, bTempList,m_iDeskCardCount[i], true);
			}
			else
			{
				ShowCurrentNoMoveCard(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView,m_DeskReadyCard[i],m_bySendCount[i]);
			}
		}
	}	
}
/*
���ܣ��������˷������ķ�����Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleSendCardBegin(void *buffer)
{
	
	SendAllCardStruct *pSendAllCard = (SendAllCardStruct *)buffer ;
	if(NULL==pSendAllCard)
	{
		return;
	}	
	//��ע��� ��ʼ���� �������м�ʱ��
	ClearAllClock();
	//��ǰ���Ƶ�λ��
	m_bySendCardPos = pSendAllCard->iStartPos;
	m_bySendCardCount = 0;
	SetStationParameter(GS_SEND_CARD);
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//ÿ�������˿˵���Ŀ
		m_iDeskCardCount[i]=pSendAllCard->iUserCardCount[i];
	}
	memcpy(m_DeskReadyCard,pSendAllCard->iUserCard,sizeof(m_DeskReadyCard));
	
	memset(m_bySendCount,0,sizeof(m_bySendCount));
	//���㷢��˳��
	SendQueue(pSendAllCard->iStartPos);

	if(!m_sendQueue.empty())
	{
		SendCard(m_sendQueue.at(0),m_bySendCardPos);
	}
}

/*
���ܣ��������˷������Ľ�ׯ�����Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleCallScoreResultMessage(void *buffer)
{
	CallScoreStruct *pScoreResult=(CallScoreStruct *)buffer;
	if(NULL==pScoreResult)
	{
		return ;
	}

	if(m_iMyDeskStation == pScoreResult->bDeskStation && 1 == pScoreResult->iValue)//��ǰ��ׯ���Լ�
	{
		//���ؽ�ׯ��ť
		SetContainVisible(CTN_1014089_CTN_1008000_BTN_1008002,FALSE);
		//���ز���ׯ��ť
		SetContainVisible(CTN_1014089_CTN_1008000_BTN_1008001,FALSE);
		//���ý�ׯ������ׯ��ť������	
		SetBtEnable(CTN_1014089_CTN_1008000_BTN_1008002,FALSE);
		SetBtEnable(CTN_1014089_CTN_1008000_BTN_1008001,FALSE);
		//ɾ����ׯ��ʱ��
		m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ;
	}

	if(0 == pScoreResult->iValue)//������ׯ
	{
		//��λ��ת��ͼλ��
		int viewStation=m_pGameFrame->DeskStation2View(pScoreResult->bDeskStation);
		//������ׯͼƬ
		SetImageVisible(CTN_1014089_IMG_2000132+viewStation,FALSE);
		//��ʾ����ׯͼƬ
		SetImageVisible(CTN_1014089_IMG_2000137+viewStation,TRUE);
	}

	int iView = m_pGameFrame->DeskStation2View(pScoreResult->bDeskStation);
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+iView,false,m_iCallScoreTime);

}
/*
���ܣ��������˷������Ľ�ׯ������Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleCallScoreFinishMessage(void *buffer)
{
	CallScoreStruct *TscoreResult=(CallScoreStruct *)buffer;
	if(NULL==TscoreResult)
	{
		return ;
	}

	m_byNtStation=TscoreResult->bDeskStation;
	
// 	if(m_iMyDeskStation==TscoreResult->bDeskStation)//��ǰ��ׯ���Լ�
// 	{
// 		//���ؽ�ׯ��ť
// 		SetContainVisible(CTN_1014089_CTN_1008000_BTN_1008002,FALSE);
// 		//ɾ����ׯ��ʱ��
// 		m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ;
// 	}
	//���ؽ�ׯ��ť
	SetContainVisible(CTN_1014089_CTN_1008000_BTN_1008002,FALSE);
	//���ز���ׯ��ť
	SetContainVisible(CTN_1014089_CTN_1008000_BTN_1008001,FALSE);
	//ɾ����ׯ��ʱ��
	m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ;
		
	//���������ׯ/����ׯͼƬ
	CLearScratchNt();
	//�������ʱ��
	ClearAllClock();
}

/*
���ܣ��������˷������Ľ�ׯ��Ϣ
*/
void CNewGameClient::OnHandleCallScoreMessage(void *buffer)
{

	CallScoreStruct *TScore=(CallScoreStruct *)buffer;	
	if(NULL==TScore)
	{
		return ;
	}
	
	ResetGameData();
	//������Ϸ��ʼ��Ч
	PlayGameSound(SOUND_BEGIN_GAME,false);	
	//����״̬
	SetStationParameter(GS_ROB_NT);

	//������ҵ�״̬
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_iUserStation[i] = TScore->byUserState[i];
	}

	//����Ҫ������
	UserInfoForGame sUser;
	int iView =0;
	if (m_bIsSuper)
	{
		for(int i=0; i<PLAY_COUNT; i++)
		{
			memset(&sUser,0,sizeof(sUser));
			if (-1 != m_iUserStation[i] && m_pGameFrame->GetUserInfo(i,sUser))
			{
				iView = m_pGameFrame->DeskStation2View(i);
			}
		}

		if(!m_SuperSetData.bSetSuccese)
		{
			SetTextShowText(CTN_2000280_CTN_100200_TEXT_100220,"��ǰδ����",true);
		}
	}

	m_bCurrentOperationStation = TScore->bDeskStation;	
	if(m_bCurrentOperationStation != 255)
	{
		if(m_bCurrentOperationStation==m_iMyDeskStation)
		{
			//��ʾ��ׯ��ť
			SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008002,true);
			SetBtEnable(CTN_1014089_CTN_1008000_BTN_1008002,true);
			//��ʾ����ׯ��ť
			SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008001,true);
			SetBtEnable(CTN_1014089_CTN_1008000_BTN_1008001,true);
		}
		//��λ��ת��ͼλ��
		int viewStation=m_pGameFrame->DeskStation2View(m_bCurrentOperationStation);
		//������Ӧ��ׯ��ʱ��
		OnStartClock(viewStation,m_iCallScoreTime);
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//����׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+i,FALSE);
	}
}
/*
���ܣ�������Ϸ��ʼ��ע��Ϣ
*/
void CNewGameClient::OnHandleBetBegin(void *buffer)
{
	UserNoteStruct *beginGame=(UserNoteStruct *)buffer;	
	if(NULL==beginGame)
	{
		return ;
	}
	
	//���ÿ�ʼ������ʼλ��
	//������Ϸ״̬
	SetStationParameter(GS_NOTE);
	m_bTimeOutCount=0;	
	
	::CopyMemory(m_iLimitNote,&beginGame->iLimitNote,sizeof(beginGame->iLimitNote));	//���ע
	memcpy(m_i64UserNoteLimite,beginGame->i64UserNoteLimite,sizeof(m_i64UserNoteLimite));//�ĸ���ע����

	m_iUpGradePeople = beginGame->iNt;                    //ׯ��
	m_iThisTurnLimit=beginGame->iLimitNote[m_iMyDeskStation];//���������ע
	
	//��ʾׯ��λ��
	int iView = m_pGameFrame->DeskStation2View(m_iUpGradePeople);
	SetImageVisible(CTN_1014089_IMG_1004060+iView,true);
		
	if(255!=m_byNtStation)
	{
		//ʱ��λ�øı�
		POINT point;
		IText *pText = NULL;
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000267));
		point.x=pText->GetGameX();
		point.y=pText->GetGameY();
		SetControlPos(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,point.x,point.y);
		//��ʾʱ��
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iXiaZhuTime);

		if (m_iMyDeskStation!=m_byNtStation && m_iUserStation[m_iMyDeskStation] != -1)
		{
			//������ע��ʱ��
			m_bCurrentClockLabel=m_iXiaZhuTime;
			m_pGameFrame->SetTimer(ID_NOTE,1000);
			//��ʾ��ע
			DisplayBet(m_iThisTurnLimit);
		}
	}
}

/*
���ܣ�������ע���
�βΣ���ָ��
����ֵ����
*/
void CNewGameClient::OnHandleBetResult(void *buffer)
{
	NoteResult *betResult=(NoteResult *)buffer;	
	if(NULL==betResult)
	{
		return ;
	}
	if(TYPE_TAKE_CARD==betResult->bAddStyle)//ͣ��������ܽ������ͷ���Ҳ����յ���ע��Ϣ
	{
		return;
	}
	
	if(m_iMyDeskStation==betResult->iOutPeople)//��ǰ����Ѿ���ע
	{
		//��ֹ��ע
		ClearAllBet();
	}
	
	//��¼����ÿ���˵���ע��
	m_iTotalGameNote[betResult->iOutPeople]=betResult->iCurNote;
	//��λ��ת��ͼλ��
	int iView = m_pGameFrame->DeskStation2View(betResult->iOutPeople);
	//������ע��ʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+iView,false,0);
	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//��λ��ת��ͼλ��
		iView=m_pGameFrame->DeskStation2View(i);
		if(m_iTotalGameNote[i]>0)
		{
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,true);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,m_iTotalGameNote[i],true);
			SetINumSingle(CTN_1014089_CTN_2000246_Num_2000251+iView,m_iTotalGameNote[i],true);	
		}
		else
		{
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,false);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,0,false);
		}
	}
}
/*
���ܣ���ʾ�ĸ���עѡ��
�βΣ�����������ע��
*/
void CNewGameClient::DisplayBet(__int64 i64MaxBetCount)
{

	__int64 i64UserCurrentMoney=0;//�û���ǰ�����

	UserInfoForGame UserInfo ;
	memset(&UserInfo,0,sizeof(UserInfo));

	//����û��Ľ����
	if(m_pGameFrame->GetUserInfo(m_iMyDeskStation, UserInfo))
	{
		i64UserCurrentMoney=UserInfo.i64Money;
	}

	if(i64MaxBetCount>0)
	{
		for(int i=3;i>=0;i--)
		{
			if(m_i64UserNoteLimite[m_iMyDeskStation][i]<=i64UserCurrentMoney)
			{
				//������ע
				BetOptionSet(m_i64UserNoteLimite[m_iMyDeskStation][i],i,true);
			}
			else
			{
				//��������ע
				BetOptionSet(m_i64UserNoteLimite[m_iMyDeskStation][i],i,false);
			}
		}
	}
}
/*
���ܣ���עѡ������
�βΣ�iOption,Ҫ���õ���עѡ�IsUse���Ƿ����
*/
void CNewGameClient::BetOptionSet(__int64 i64Num,int iOption,bool IsUse)
{
	//���ð�ť
	SetBtVisible(CTN_1014089_CTN_2000180_BTN_2000198+iOption,IsUse);
	SetBtEnable(CTN_1014089_CTN_2000180_BTN_2000198+iOption,IsUse);
	//�������ֿؼ�
	SetInumPro(CTN_1014089_CTN_2000180_Num_2000202+iOption,i64Num,2,IsUse);
}

/*
���ܣ�������ʾ������
�βΣ�iControlID,�ɲ�����ID,bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowUpCard(const int iControlID,const int iControlID2,BYTE bUnderCardList[],BYTE bUnderCardCount,BYTE bUpCardList[],BYTE bUpCardCount)
{
	INoMoveCard  *pNoOperateCard1 = NULL ; 
	pNoOperateCard1 = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 
	if(NULL != pNoOperateCard1)
	{
		pNoOperateCard1->SetControlVisible(true) ; 
		pNoOperateCard1->SetCardList(bUnderCardList,bUnderCardCount);
	}
	INoMoveCard  *pNoOperateCard2 = NULL ; 
	pNoOperateCard2 = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID2))  ;
	if(NULL != pNoOperateCard2)
	{
		pNoOperateCard2->SetControlVisible(true) ; 
		pNoOperateCard2->SetCardList(bUpCardList,bUpCardCount);
	}
}
/*
���ܣ����Լ�����
�βΣ�bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowCurrentCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
{
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID))  ; 

	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(true) ; 
		pOperateCard->SetCardList(bCardList , cardCount) ; 
	}
}
/*
���ܣ����Լ�����
*/
void CNewGameClient::ShowCurrentNoMoveCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
{

	INoMoveCard  *pOperate = NULL ;
	BYTE bTempList[5]={0};
	pOperate = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 

	if(NULL != pOperate)
	{
		pOperate->SetControlVisible(true) ; 
		pOperate->SetCardList(bTempList , cardCount) ; 
	}
}
/*
���ܣ�չʾ̯����ҵ���
�βΣ�bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowOpenCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
{
	INoMoveCard  *pOperate = NULL ; 
	pOperate = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 

	if(NULL != pOperate)
	{
		pOperate->SetControlVisible(true) ; 
		pOperate->SetCardList(bCardList , cardCount) ; 
	}
}
/*
���ܣ��������������ׯ/����ׯͼƬ
�βΣ�void
����ֵ��void
*/
void CNewGameClient::CLearScratchNt()
{
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetImageVisible(CTN_1014089_IMG_2000132+i,FALSE);
		SetImageVisible(CTN_1014089_IMG_2000137+i,FALSE);
	}
}
/*
���ܣ��������ʱ��
�βΣ�void
����ֵ��void
*/
void CNewGameClient::ClearAllClock()
{
	//�ر����ж�ʱ��
	KillAllGameTime();
	//�����û���ʱ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+i,false,0);
	}
}

/*
���ܣ���ֹ��ע
�βΣ�void
����ֵ��void
*/
void CNewGameClient::ClearAllBet()
{
	//�ر���ע��ʱ��
	m_pGameFrame->KillTimer(ID_NOTE) ;
	//���ð�ť
	for(int i=0;i<4;i++)
	{
		SetBtVisible(CTN_1014089_CTN_2000180_BTN_2000198+i,false);
		SetBtEnable(CTN_1014089_CTN_2000180_BTN_2000198+i,false);
		SetInumPro(CTN_1014089_CTN_2000180_Num_2000202+i,0,2,false);
	}		
	
}

//������Ӧ��ׯ��ʱ��
void CNewGameClient::OnStartClock(int viewStation,BYTE bCallScoreTime)
{
	//��ʾ��ʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+viewStation,true,bCallScoreTime);

	m_bCurrentClockLabel=m_iCallScoreTime;
	//������ʱ��
	m_pGameFrame->SetTimer(ID_CALL_SCORE_TIME,1000);	
}

//--------------------------------------------------------------------------------------------------------
//���������ؼ��ɼ�
void	CNewGameClient::SetContainVisible(const int iContainID,bool bFlag)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		pContain->SetControlVisible(bFlag);
	}
}

//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
void CNewGameClient::SetPicSize(const int iButtonID,CString sImagSrc, bool bFlag,int x,int y,int w,int h)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iButtonID));
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
/*
���ܣ����������ؼ�����Ӧ�κ����ʱ��
������iContainID:�ؼ�ID��bFlag���Ƿ���Ӧ
return:void
*/
void  CNewGameClient::SetContainEnable(const int iContainID,bool bFlag)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		pContain->SetEnable(bFlag);
	}
}

//---------------------------------------------------------------------------------------
///ɾ�����ж�ʱ��,����ע����ʤ������ʾ��ʱ����
void CNewGameClient:: KillAllGameTime() 
{
	m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ; 

	m_pGameFrame->KillTimer(ID_NOTE) ; 

	m_pGameFrame->KillTimer(ID_TAKE_CARD) ; 

	m_pGameFrame->KillTimer(ID_BEGIN_TIME);

	m_pGameFrame->KillTimer(ID_WAIT_NEXT);

}


//�������ֿռ���ʾ����
void	CNewGameClient::SetInumInfo(const int iNumID, __int64 i64Num,bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�������ֿռ���ʾ����
void	CNewGameClient::SetInumPro(const int iNumID, __int64 i64Num,BYTE bPro,bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		switch(bPro)
		{
		case 0:
			{
				NEW_NUM_STRUCT NumStruct;

				NumStruct.bShowPlusSignOrMinusSign = true;
				NumStruct.bShowComma = true;
				NumStruct.iShowCommaIndex = 3;
				NumStruct.iShowZeroType = 0;
				pNum->SetNewNumStruct(NumStruct);
				pNum->SetNewNum(i64Num);
				pNum->SetControlVisible(bFlag);
				break;
			}
		case 1:
			{
				NEW_NUM_STRUCT NumStruct;

				NumStruct.bShowPlusSignOrMinusSign = false;
				NumStruct.bShowComma = true;
				NumStruct.iShowCommaIndex = 3;
				NumStruct.iShowZeroType = 0;
				pNum->SetNewNumStruct(NumStruct);
				pNum->SetNewNum(i64Num);
				pNum->SetControlVisible(bFlag);
				break;
			}
		case 2:
			{
				pNum->SetNum(i64Num);
				pNum->SetControlVisible(bFlag);
				break;
			}
		}
		
	}
}
//--------------------------------------------------------------------------------------------------------
//�������ֿռ���ʾ����(�����Ӻźͼ��ż�����)
void	CNewGameClient::SetInumInfo_1(const int iNumID, __int64 i64Num, bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����(��ʾ����)
void	CNewGameClient::SetTextinfo(const int iTextID,__int64 iNum, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
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
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%s",ch);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ʱ�����ʾ����ʱ
void	CNewGameClient::SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime)
{
	ITime *pTime = NULL;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(iTimeID));
	if (NULL != pTime)
	{
		pTime->SetShowMaxNum(byMaxTime);
		pTime->SetShowMinNum(byMinTime);
		pTime->SetPLay(bFlag);
		pTime->SetControlVisible(bFlag);
	}
}

//-----------------------------------------------------------------------------------------------
/// ������Ϸ�ĵ�ǰ״̬
void	CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{

	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 

	GameStation_Base *pGameStation = (GameStation_Base *)pBuffer;
	if (NULL == pGameStation)
	{
		return;
	}
	SetStationParameter(pGameStation->byGameStation);

	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:			//�ȴ���һ�̿�ʼ
		{
			if (nLen != sizeof(GameStation_WaiteAgree))
			{
				return;
			}
			SetStationParameter(GS_WAIT_ARGEE);
		    HandleStartStation(pBuffer);
			break; 
		}
	case GS_ROB_NT:		//��ׯ״̬
		{
			if (nLen != sizeof(GameStation_RobNt))
			{
				return;
			}
			SetStationParameter(GS_ROB_NT);	
			HandleRobNtStation(pBuffer);
			break;
		}
	case GS_NOTE:		//��ע״̬
		{
			if (nLen != sizeof(GameStation_Note))
			{
				return;
			}
			SetStationParameter(GS_NOTE);	
			HandleNoteStation(pBuffer);
			break;;
		}
	case GS_SEND_CARD:	//����״̬
		{
			if (nLen != sizeof(GameStation_SendCard))
			{
				return;
			}
			SetStationParameter(GS_SEND_CARD);			
			//����״̬
			HandleSendCardStation(pBuffer);
			break;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			if (nLen != sizeof(GameStation_OpenCard))
			{
				return;
			}
			SetStationParameter(GS_PLAY_GAME);		
			//��Ϸ������
			HandleOpenCardStation(pBuffer);			
			break;
		}
	}

}
//��һ����������������ִ�д˺���
void CNewGameClient::HandleStartStation(void *buffer)
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

	//��ʾ�Ѿ�׼�����мҵ���ʾ׼��ͼƬ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		int iViewStation = m_pGameFrame->DeskStation2View(i);
		if(pGameStation->bUserReady[i])//�Ѿ�׼��
		{
			SetImageVisible(CTN_1014089_IMG_1004070+iViewStation,true);
		}
		else
		{
			SetImageVisible(CTN_1014089_IMG_1004070+iViewStation,false);
		}
	}

	//�����ȴ���ʼ��ʱ��
	m_bCurrentClockLabel = m_iBeginTime;
	//������ŶӾͲ���ʾʱ��
	if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
	{
		//���ؿ�ʼ��ť 
		SetBtVisible(CTN_1014089_BTN_1004006,false);
		//����ʾʱ��
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,false,m_iBeginTime);
	}
	else
	{
		//��ʾ��ʼ��ť 
		SetBtVisible(CTN_1014089_BTN_1004006,true);
		SetBtEnable(CTN_1014089_BTN_1004006,true);
		//��ʾʱ��
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iBeginTime);
		m_pGameFrame->SetTimer(ID_BEGIN_TIME,1000);
	}
}
/*----------------------------------------------------------------------------------*/
//��ׯ״̬
void	CNewGameClient::HandleRobNtStation(void *buffer)
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
	//������ʾ��ׯ������ׯ��ť
	if(m_bCurrentOperationStation != 255)
	{
		if(m_bCurrentOperationStation == m_iMyDeskStation)
		{
			//��ʾ��ׯ��ť
			SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008002,true);
			SetBtEnable(CTN_1014089_CTN_1008000_BTN_1008002,true);
			//��ʾ����ׯ��ť
			SetBtVisible(CTN_1014089_CTN_1008000_BTN_1008001,true);
			SetBtEnable(CTN_1014089_CTN_1008000_BTN_1008001,true);
		}		
	}
	
	//��ʾ�����ׯ״̬
	int iView = 0;
	for (int i=0;i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		if (0 == pGameStation->byUserCallStation[i])
		{
			SetImageVisible(CTN_1014089_IMG_2000137+iView,true);
			SetImageVisible(CTN_1014089_IMG_2000132+iView,false);
		}
		else if (1 == pGameStation->byUserCallStation[i])
		{
			SetImageVisible(CTN_1014089_IMG_2000137+iView,false);
			SetImageVisible(CTN_1014089_IMG_2000132+iView,true);
		}
		else
		{
			SetImageVisible(CTN_1014089_IMG_2000137+iView,false);
			SetImageVisible(CTN_1014089_IMG_2000132+iView,false);
		}
		//����׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+iView,false);
	}
	
	//��ʾ��ǰ��ׯ��ҵļ�ʱ��
	iView = m_pGameFrame->DeskStation2View(pGameStation->byCurrentCallScore);
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+iView,true,m_byLostTime);
}
/*----------------------------------------------------------------------------------*/
//��ע״̬
void	CNewGameClient::HandleNoteStation(void *buffer)
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

	//��λ��ת��ͼλ��
	int viewStation=m_pGameFrame->DeskStation2View(m_byNtStation);
	//��ʾׯ��ͼƬ
	SetImageVisible(CTN_1014089_IMG_1004060+viewStation,TRUE);

	//���س���
	for(int i=0;i<4;i++)
	{
		SetBtVisible(CTN_1014089_CTN_2000180_BTN_2000198+i,false);
		SetBtEnable(CTN_1014089_CTN_2000180_BTN_2000198+i,false);
		SetInumPro(CTN_1014089_CTN_2000180_Num_2000202+i,0,2,false);
	}
	
	//��¼��ʾ����ÿ���˵���ע��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		m_iTotalGameNote[i] = pGameStation->i64PerJuTotalNote[i];
		if (pGameStation->iUserStation[i] == TYPE_NOTE )
		{
			if(m_iTotalGameNote[i]>0)
			{
				SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+i,true);
				SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,m_iTotalGameNote[i],true);
				SetINumSingle(CTN_1014089_CTN_2000246_Num_2000251+i,m_iTotalGameNote[i],true);	
			}
			else
			{
				SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+i,false);
				SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,0,false);
			}
		}
		//����׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+i,false);
	}
	//�Լ������ && ����ׯ�� && ������;�����
	if (pGameStation->iUserStation[m_iMyDeskStation] == TYPE_PLAY_GAME && m_iMyDeskStation!=m_byNtStation && m_iUserStation[m_iMyDeskStation] != -1)	
	{
		//������ע��ʱ��
		m_bCurrentClockLabel=m_iXiaZhuTime;
		m_pGameFrame->SetTimer(ID_NOTE,1000);
		//��ʾ��ע
		DisplayBet(m_iThisTurnLimit);
	}

	//ʱ��λ�øı�
	POINT point;
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000267));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,point.x,point.y);
	//��ʾʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_byLostTime);

}
/*----------------------------------------------------------------------------------*/
//����״̬
void	CNewGameClient::HandleSendCardStation(void *buffer)
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
	//��ʾ�˿���
	int iView = 0;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (0 != m_iDeskCardCount[i])
		{
			if (i == m_iMyDeskStation)
			{
				SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010,m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation],true);
			}
			else
			{
				iView = m_pGameFrame->DeskStation2View(i);
				ShowCurrentNoMoveCard(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView,m_DeskReadyCard[i],m_iDeskCardCount[i]);
			}
		}
		//׼��ͼƬ
		//SetImageVisible(CTN_1014089_IMG_1004070+i,false);
	}

	//�û���ע���
	memcpy(m_iTotalGameNote,pGameStation->i64PerJuTotalNote,sizeof(m_iTotalGameNote));
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(m_iTotalGameNote[i]>0)
		{
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+i,true);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,m_iTotalGameNote[i],true);
			SetINumSingle(CTN_1014089_CTN_2000246_Num_2000251+i,m_iTotalGameNote[i],true);	
		}
		else
		{
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+i,false);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,0,false);
		}
	}	
}
/*----------------------------------------------------------------------------------*/
//��Ϸ������
void	CNewGameClient::HandleOpenCardStation(void *buffer)
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

	//��ʾҪ�ƺ�ͣ�ư�ť
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000207,true);	
	SetBtVisible(CTN_1014089_CTN_2000206_BTN_2000209,true);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,true);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,true);	

	//��ʾ�˿���
	for(int i = 0x00;i < PLAY_COUNT;i++)
	{
		if(i == m_iMyDeskStation)
		{
			BYTE bTempList[5]={80,80,80,80,80};
			for(int j = 0x00;j < m_byOpenCardCount[m_iMyDeskStation];j++)
			{
				bTempList[j] = m_DeskReadyCard[m_iMyDeskStation][j];
			}
			SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, bTempList,m_iDeskCardCount[i], true);
		}
		else
		{
			int iView = m_pGameFrame->DeskStation2View(i);
			ShowCurrentNoMoveCard(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView,m_DeskReadyCard[i],m_iDeskCardCount[i]);
		}
	}

	//�û���ע���
	memcpy(m_iTotalGameNote,pGameStation->i64PerJuTotalNote,sizeof(m_iTotalGameNote));
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(m_iTotalGameNote[i]>0)
		{
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+i,true);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,m_iTotalGameNote[i],true);
			SetINumSingle(CTN_1014089_CTN_2000246_Num_2000251+i,m_iTotalGameNote[i],true);	
		}
		else
		{
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+i,false);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,0,false);
		}

		//׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+i,false);
	}	


	//ʱ��λ�øı�
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000267));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,point.x,point.y);
	//��ʾʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_byLostTime);


}


//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ���λ��
void	CNewGameClient::SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetGameXY(iPositonX,iPositonY);
		pImage->SetControlVisible(bFlag);
	}
}
/// ��������״̬
int	CNewGameClient::ResetGameFrame(void)
{
	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	//��λ��ת��ͼλ��
	int viewStation=m_pGameFrame->DeskStation2View(nIndex);
	//��ʾ׼��ͼƬ
	SetImageVisible(CTN_1014089_IMG_1004070+viewStation,false);
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
	if (-1 == m_iUserStation[m_iMyDeskStation])
	{
		return false;
	}

	if(m_byGameStation >= GS_ROB_NT && m_byGameStation < GS_WAIT_NEXT)
	{
		return true;
	}
	return false;
}

///����˳���Ϸ
bool CNewGameClient::OnGameQuiting()
{
	if(m_pGameFrame->GetIsWatching())
	{
		return true ; 
	}


	if (-1 == m_iUserStation[m_iMyDeskStation])
	{
		return true;
	}
	CString szAppPath = CINIFile::GetAppPath();
	szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
	int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
	CString szAppPathParent = szAppPath.Left(nPosOfSlash);


	CString strKeyName  ; 
	strKeyName.Format("%d" ,NAME_ID) ; 
	CBcfFile File(szAppPathParent + "\\SpecialRule.bcf") ; 

	bool   bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strKeyName ,1) ; 

	if(m_byGameStation >= GS_ROB_NT && m_byGameStation < GS_WAIT_NEXT)
	{
		if(bForceQuitAsAuto)
		{
			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪��Ϸ�����йܴ��� ���Ƿ�Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
			{
				return false ; 
			}	
		}
		else
		{
			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪���ᱻ�۷����Ƿ����Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
			{
				return false ; 
			}
			m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
		}
	}
	return true;
}
/// ��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
	case ID_CALL_SCORE_TIME:
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//��ׯʱ�䵽
			{
				m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ;
				if(m_bCurrentOperationStation==m_iMyDeskStation)//��ǰ�з����Լ�
				{
					OnHandleHitGiveUpNtBt();
				}
			}
			return ;
		}
	case ID_NOTE:	
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//��עʱ�䵽
			{
				m_pGameFrame->KillTimer(ID_NOTE) ;
				OnHandleHitBetBt(4);
				ClearAllBet();
			}
			return;
		}
	case ID_TAKE_CARD:	
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//
			{
				m_pGameFrame->KillTimer(ID_TAKE_CARD) ;//Ҫ��ʱ�䵽
			}
			return;
		}
	case ID_BEGIN_TIME:			//��ʼ�ȴ���ʱ��
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//��ʼ��Ϸ�ȴ�ʱ�䵽
			{
				m_pGameFrame->KillTimer(ID_BEGIN_TIME) ;
			}
			return;
		}
	case ID_WAIT_NEXT:
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//
			{
				m_pGameFrame->KillTimer(ID_WAIT_NEXT) ;//��ʼ��Ϸ�ȴ�ʱ�䵽
			}
			return;
		}
	default:
		{
			return;
		}	
	}
}
/*----------------------------------------------------------------------------------------*/
//��ʾ��ʱ��
void	CNewGameClient::ShowTimer(const int iTimeID, bool bShow, BYTE byMax, BYTE byMin )
{

	if (iTimeID == CTN_1014089_CTN_200400_CTN_2004010_Time_2004011)
	{
		SetBtVisible(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011,true);
	}
	else if(iTimeID == CTN_1014089_CTN_200400_CTN_2004020_Time_2004021)
	{
		SetBtVisible(CTN_1014089_CTN_200400_CTN_2004020_Time_2004021,true);
	}
	else if(iTimeID == CTN_1014089_CTN_200400_CTN_2004030_Time_2004031)
	{
		SetBtVisible(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true);
	}
	else if(iTimeID == CTN_1014089_CTN_200400_CTN_2004040_Time_2004041)
	{
		SetBtVisible(CTN_1014089_CTN_200400_CTN_2004040_Time_2004041,true);
	}
	else if(iTimeID == CTN_1014089_CTN_200400_CTN_2004040_Time_2004042)
	{
		SetBtVisible(CTN_1014089_CTN_200400_CTN_2004040_Time_2004042,true);
	}

	SetTimePlay(iTimeID,bShow,byMax,byMin);

}
/*----------------------------------------------------------------------------------------*/
//ʱ�侯ʾ��
void CNewGameClient::TimeWarning(int iTime)
{
	switch(iTime)
	{
	case 0:
		{
			PlayGameSound(SOUND_TIME_0,false);
			break;
		}
	case 1:
		{
			PlayGameSound(SOUND_TIME_1,false);
			break;
		}
	case 2:
		{
			PlayGameSound(SOUND_TIME_2,false);
			break;
		}
	case 3:
		{
			PlayGameSound(SOUND_TIME_3,false);
			break;
		}
	case 4:
		{
			PlayGameSound(SOUND_TIME_4,false);
			break;
		}
	case 5:
		{
			PlayGameSound(SOUND_TIME_5,false);
			break;
		}
	default:
		{
			break;
		}
	}
}
///��ʼ����������
void CNewGameClient::ResetGameData()
{
	m_bCurrentOperationStation=255;		//��ǰ�з���
	m_bCurrentClockLabel=0;			//��ǰʱ�ӱ�ǩ
	m_iCurrentClockControlID=0;		//��ǰ���Ƶ�ʱ��ID
	m_iCurrentCardCount=5;
	m_byNtStation=255;

	memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));	//ÿ�������˿˵���Ŀ	
	memset(m_DeskReadyCard,0,sizeof(m_DeskReadyCard));	    ////ÿ�������˿˵�����

	memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));	//������Ϸ��ע��(ʵ��)
			
	memset(m_bSendCard,255,sizeof(m_bSendCard));			//��������
	memset(m_iUserStation,-1,sizeof(m_iUserStation));		//�����Ϸ״̬
	memset(&m_SuperSetData,255,sizeof(m_SuperSetData));		//������������
	m_SuperSetData.bSetSuccese = false;

	//�������
	m_sendQueue.clear();			//����˳������
	m_bySendCardCount=0;			//��ǰ���ƴ���
}
//---------------------------------------------------------------------------------------
//���ű�����Ч
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	if(!m_SoundEnginBg)
	{
		return ;
	}

	if(!m_bSoundBgPlay)
	{
		return ;
	}
	
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	wsprintf(szFileName,"Sound\\BG.ogg");
	m_SoundEnginBg->play2D(szFileName,bISLoop);

}
//---------------------------------------------------------------------------------------
//������Ϸ��Ч
void	CNewGameClient::PlayGameSound( int SoundID, bool bISLoop,BYTE byDeskStation )
{
	if(!m_SoundEngineGame)
	{
		return ;
	}
	if(!m_iGameSet[0])
	{
		return;
	}
	if(!m_bSoundPlay)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	if (SoundID >= SOUND_BEGIN_GAME && SoundID <= SOUND_GIVEUP)
	{
		switch(SoundID)
		{
		case SOUND_BEGIN_GAME:
			{
				wsprintf(szFileName,"Sound\\nn_start.wav");
				break;
			}
		case SOUND_TIME_0:
			{
				wsprintf(szFileName,"Sound\\time0.wav");
				break;
			}
		case SOUND_TIME_1:
			{
				wsprintf(szFileName,"Sound\\time1.wav");
				break;
			}
		case SOUND_TIME_2:
			{
				wsprintf(szFileName,"Sound\\time2.wav");
				break;
			}
		case SOUND_TIME_3:	
			{
				wsprintf(szFileName,"Sound\\time3.wav");
				break;
			}
		case SOUND_TIME_4:	
			{
				wsprintf(szFileName,"Sound\\time4.wav");
				break;
			}
		case SOUND_TIME_5:	
			{
				wsprintf(szFileName,"Sound\\time5.wav");
				break;
			}
		case SOUND_WIN:		
			{
				wsprintf(szFileName,"Sound\\win.wav");
				break;
			}
		case SOUND_LOSE:	
			{
				wsprintf(szFileName,"Sound\\lose.wav");
				break;
			}
		case SOUND_CHONGPAI:
			{
				wsprintf(szFileName,"Sound\\chongpai.wav");
				break;
			}
		case SOUND_XIAZHU:		
			{
				wsprintf(szFileName,"Sound\\chip.wav");
				break;
			}
		case SOUND_TIP:	
			{
				wsprintf(szFileName,"Sound\\Tip.wav");
				break;
			}
		case SOUND_TANPAI:		
			{
				wsprintf(szFileName,"Sound\\tanpai.wav");
				break;
			}
		case SOUND_CLICKCARD:	
			{
				wsprintf(szFileName,"Sound\\hitcard.wav");
				break;
			}
		case SOUND_HIT_BEGIN:
			{
				wsprintf(szFileName,"Sound\\hitbegin.wav");
				break;
			}
		case SOUND_JIAOZHUANG:
		case SOUND_GIVEUP:
			{
				wsprintf(szFileName,"Sound\\giveupnt.wav");
				break;
			}
		default:
			{
				break;
			}
		}
	}
	else 
	{
		UserInfoForGame UserInfo ; 
		memset(&UserInfo,0,sizeof(UserInfo));
		if (m_pGameFrame->GetUserInfo(byDeskStation, UserInfo))
		{
			switch(SoundID)
			{
			case SOUND_NIUNIU_0:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_0.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_1:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_1.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_2:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_2.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_3:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_3.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_4:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_4.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_5:	
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_5.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_6:		
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_6.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_7:	
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_7.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_8:	
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_8.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_9:	
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_9.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_10:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_10.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}
/*
@brief:���ŷ��ƶ���
@param:ACTION_ID,�����ؼ�ID��END_ID �յ�ID
@return:void
*/
void CNewGameClient::ShowSendCardAction(const int ACTION_ID,const int END_ID)
{
	//��ö����ؼ�ID
	IMoveAction* pMoveAction = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,ACTION_ID);

	//��÷����Ʊ�
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1014089_IMG_1005003));

	POINT pStart,pEnd;

	//��÷������
	IText *pText_1 = NULL;
	pText_1 = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_TEXT_1005019));
	if (NULL != pText_1)
	{
		pStart.x=pText_1->GetGameX();
		pStart.y=pText_1->GetGameY();
	}
	//��÷����յ�
	IText *pText_2 = NULL;
	pText_2 = dynamic_cast<IText *>(m_pUI->GetIControlbyID(END_ID));
	if (NULL != pText_2)
	{
		pEnd.x=pText_2->GetGameX();
		pEnd.y=pText_2->GetGameY();
	}
	
	
	if(NULL!=pMoveAction && NULL!=pImage)
	{
		pMoveAction->SetControlingCtrlID(CTN_1014089_IMG_1005003);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pEnd);
		pMoveAction->SetTotalMoveTime((unsigned int)m_iSendCardTime);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
	}
}
//
/*
@brief:����
@param:ACTION_ID,�����ؼ�ID��END_ID �յ�ID,bView����ͼλ��
@return:void
*/
void CNewGameClient::PlaySendCard(BYTE Station)
{
	
}
/*
@brief:����
@param:bCardValue,��ֵ��bSendPeople����ǰ������
@return:void
*/
void CNewGameClient::SendCard(BYTE bCardValue,BYTE bySendPeople)
{

	SetImageVisible(CTN_1014089_IMG_1005003,true);
	int iView = m_pGameFrame->DeskStation2View(bySendPeople);
	ShowSendCardAction(CTN_2000128_ACTION_2000128+iView,CTN_1014089_TEXT_1005020+iView);
}
/*
@brief:���㷢��˳��
@param:BCardList:�û��ƣ�iCardCount��ÿ����Ҫ�����Ƶ�������iStart��˭�ȷ���
@@return:void
*/
void CNewGameClient::SendQueue(int iStart)
{
	int iPos = iStart;
	m_sendQueue.clear();
	for(int m=0;m<SH_USER_CARD;m++)
	{
		iPos = iStart;
		for(int i=0; i<PLAY_COUNT; i++)
		{
			iPos = (iStart+i)%PLAY_COUNT;
			m_sendQueue.push_back(m_DeskReadyCard[iPos][m]);
		}
	}
}
/*
@brief:���㵱ǰ�Ƶ�ӵ����
@param:bCardList:�û���,bCardValue:Ҫ���ҵ�������,int iCardCount,ÿ�������ӵ�е�������
@@return:void
*/
BYTE CNewGameClient::GetSendStation(BYTE (*bCardList)[5],BYTE bCardValue,int iCardCount)
{
	UserInfoForGame userInfo;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pGameFrame->GetUserInfo(i,userInfo))
		{
			int viewStation=m_pGameFrame->DeskStation2View(i);
			for(int k=0;k<iCardCount;k++)
			{
				if(bCardValue==bCardList[viewStation][k])
				{
					return viewStation;
				}
			}
		}
	}
	return 255;//��Ч����
}
/*
@brief:��ǰ�������Ƿ���Ч

*/
BOOL CNewGameClient::IsUser(BYTE bCardValue)
{
	if(bCardValue>0 && bCardValue<80)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�������ֿؼ��Ƿ�Ҫ��ʾ����λ
//======================================================================================
void CNewGameClient::SetINumSingle(const int iNumID,__int64 iNum,bool bFlag)		
{
	INum *pNum = NULL;
	NEW_NUM_STRUCT numNewStruct;
	if(iNum>0x00)
	{
		numNewStruct.bShowPlusSignOrMinusSign=false;
	}
	else
	{
		numNewStruct.bShowPlusSignOrMinusSign=true;
	}
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));		//ׯ�ҵ÷�
	pNum->SetNewNumStruct(numNewStruct);
	pNum->SetControlVisible(bFlag);
	pNum->SetNewNum(iNum);
}  

