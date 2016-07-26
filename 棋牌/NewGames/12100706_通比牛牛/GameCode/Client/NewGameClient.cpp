#include "stdafx.h"
#include "NewGameClient.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "defines.h"
#include "afxmt.h"

static void Msg(const char *lpszFormat, ...)
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

CEvent g_PlayEvent; 

IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
	return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
	if (*ppImp!=NULL)
	{
		delete *ppImp;
		*ppImp = NULL;
	}
}

//���캯��
CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;					///��Ϸ���

	m_pUI = pUI;								///��ϷUI

	::memset(m_UserInfo,NULL,sizeof(m_UserInfo));	/**<�����Ϣ*/

	m_SoundEngine = createIrrKlangDevice();		///��Чָ��

	m_tSond = AfxBeginThread(PlaySond,this);

	m_bGameStation = GS_WAIT_SETGAME;           ///��Ϸ״̬

	m_byLgStation = 255;						///�Լ����߼�λ��

	m_bWatchMode = false;						///�Ƿ�Ϊ�Թ���

	m_bSuperShow = false;
	ZeroMemory(m_bSuperTag,sizeof(m_bSuperTag));
	m_bSuperSet = false;
	m_bCanSuper = false;

	InitGameData();
}

//��������
CNewGameClient::~CNewGameClient()
{
	if(m_SoundEngine)
	{
		m_SoundEngine->drop();				//�ͷ���Ч�ļ�
		m_SoundEngine = NULL;
	}
	//�˳��߳�
	m_pSoundNews.bExitProess = true;	
	g_PlayEvent.SetEvent();

}

/// ��ʼ��
int CNewGameClient::Initial()
{
	// ����UI
	wchar_t wszUIFile[MAX_PATH];
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
	m_pUI->LoadData(wszUIFile);
	return 0;
}

/**
* ���ط����������ļ�
*
* return   void
**/
void CNewGameClient::LoadBcf()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +GET_SKIN_FOLDER(szTempStr) +"_c.ini");
	CString key = TEXT("config");

	m_bSound = f.GetKeyVal(key,"bSound",true);		/**<�Ƿ񲥷���Ч*/
	m_bWatch = f.GetKeyVal(key,"bWatch",true);		/**<�Ƿ�����Թ�*/

	CBcfFile fbcf(s + "Order.bcf"); 
	key = TEXT("VirtualMoney");

	m_bySpaceBits  = fbcf.GetKeyVal(key,"SpaceBits",3);
	m_bShowComma  = fbcf.GetKeyVal(key,"UseSpace",false);

	SendIsWatchData();
}

///����UI����Ԫ�ص�״̬
int CNewGameClient::InitUI()
{	
	LoadBcf();					//���������ļ�

	ShowOrEnableControl(BN_GAME_START,false,false);			//��ʼ��ť

	ReSetGameUIStation();

	return 0;	
}

/**
*��ʼ����Ϸ����
*
*return bool ��ʼ����Ϸ�����Ƿ�ɹ�
*/
bool CNewGameClient::InitGameData()
{
	m_iSCCount = 0;										///��������

	::memset(m_iUCCount,0,sizeof(m_iUCCount));			///�����������

	::memset(m_byUCard,0,sizeof(m_byUCard));			///����˿�

	::memset(m_byDownBCard,0,sizeof(m_byDownBCard));		///�����ţ��

	::memset(m_sBData.bIsInGame,false,sizeof(m_sBData.bIsInGame));	///����Ƿ�����Ϸ��

	return true;
}

/**
* ��������UI��״̬
*
* @param BYTE bDeskStation [in] ���λ��
*
* @param bool bSetCommon [in] �Ƿ����ù�ͬ����
*
*return bool true:�������óɹ���false:��������ʧ��
*/
bool CNewGameClient::ReSetGameUIStation(BYTE bDeskStation,bool bSetCommon,bool bAgreeStation) 
{
	if(255 != bDeskStation)
	{
		ShowOrHideTime(bDeskStation,false,0);		//���ؼ�ʱ��

		SOrHReadyMark(bDeskStation,false);			//����׼��ͼ��

		SorHTanPaiMark(bDeskStation,false);			//����̯��ͼƬ

		if(bAgreeStation)
		{
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				SOrHCard(i,0,false);			//�����ƿؼ�

				SorHCardShape(i,255,false);	//��������ͼƬ
			}
		}
	}
	else
	{
		for(int i = 0; i < PLAY_COUNT; ++i)
		{
			ShowOrHideTime(i,false,0);		//���ؼ�ʱ��

			SOrHReadyMark(i,false);			//����׼��ͼ��

			SOrHCard(i,0,false);			//�����ƿؼ�

			SorHTanPaiMark(i,false);		//����̯��ͼƬ

			SorHCardShape(i,255,false);		//��������ͼƬ
		}
	}

	if(bSetCommon)		//���ع����ؼ�
	{
		SOrHGResultData(NULL,false);		//���ؽ�������

		SOrHBasePoint(0,false);				//���ص׷�
		
		for(int i = 0; i <= 3; ++i)			//����̯�ơ���ʾ������
		{
			ShowOrEnableControl(BN_CUE+i,false,false);			
		}
	}

	return true;
}

//������Ϸ״̬
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:	//�ȴ�ͬ��״̬
		{	
			GameStation_2 * pGameStation=(GameStation_2 *)pBuffer;

			m_sIData = pGameStation->s_Data;

			m_Logic.SetCardShape(m_sIData.iCardShape);

			ShowOrHideTime(m_byLgStation,true,m_sIData.byBTime,0,ID_BEGIN_TIME);

			//�Ŷӻ�����ʾ��ʼ��ť
			if(!(m_pGameFrame->GetGameInfo()->dwRoomRule&GRR_QUEUE_GAME))
			{
				ShowOrHideTime(m_byLgStation,true,m_sIData.byBTime,0,ID_BEGIN_TIME);	//��ʾ��ʱ��
				ShowOrEnableControl(BN_GAME_START,true,true);
			}
			
			m_bCanSuper = pGameStation->sBData.bIsSuper;
			InitialSuper(pGameStation->sBData.bIsSuper);
			
			return;
		}
	case GS_SEND_CARD:	//����״̬
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pBuffer;

			m_sIData = pGameStation->s_Data;

			::CopyMemory(m_iUCCount,pGameStation->iUCCount,sizeof(pGameStation->iUCCount));

			::CopyMemory(m_byUCard[m_byLgStation],pGameStation->byUCard,sizeof(pGameStation->byUCard));

			m_Logic.SetCardShape(m_sIData.iCardShape);

			SOrHCard(m_byLgStation,1,true);

			m_bCanSuper = pGameStation->sBData.bIsSuper;
			InitialSuper(pGameStation->sBData.bIsSuper);
			return;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pBuffer;

			m_sIData = pGameStation->s_Data;

			::CopyMemory(m_iUCCount,pGameStation->iUCCount,sizeof(pGameStation->iUCCount));

			::CopyMemory(m_byUCard[m_byLgStation],pGameStation->byUCard,sizeof(pGameStation->byUCard));

			::CopyMemory(m_byDownBCard[m_byLgStation],pGameStation->byDownCard,sizeof(pGameStation->byDownCard));

			m_Logic.SetCardShape(m_sIData.iCardShape);

			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(!m_sBData.bIsInGame[i])
					continue;

				if(i == m_byLgStation)			//�Լ��߼�λ��
				{
					if(m_sBData.bHaveBaiPai[i])
					{
						SOrHCard(i,2,true);

						ShowOrEnableControl(BN_TAN_PAI,false,false);			//̯�ư�ť
						ShowOrEnableControl(BN_CUE,false,false);				//��ʾ��ť
						ShowOrEnableControl(BN_CHONG_BAI,false,false);			//�ذڰ�ť

						ShowOrHideTime(i,true,pGameStation->iSurplusTime,0,0);	//��ʾ��ʱ��
					}
					else
					{
						SOrHCard(i,1,true);

						if(m_sBData.bIsAuto[i])		//�й�
						{
							ShowOrHideTime(i,true,pGameStation->iSurplusTime,0,0);	//��ʾ��ʱ��
							m_pGameFrame->SetTimer(ID_THINK_TIME,SEC);
						}
						else						//û���й�
						{
							ShowOrHideTime(i,true,pGameStation->iSurplusTime,0,ID_THINK_TIME);	//��ʾ��ʱ��
						}

						ShowOrEnableControl(BN_TAN_PAI,true,true);				//̯�ư�ť
						ShowOrEnableControl(BN_CUE,true,true);					//��ʾ��ť
						ShowOrEnableControl(BN_CHONG_BAI,false,false);			//�ذڰ�ť
					}	
				}
				else							//��������߼�λ��
				{
					if(m_sBData.bHaveBaiPai[i])
					{
						SorHTanPaiMark(i,true);
					}

					SOrHCard(i,1,true);		
				}
			}

			m_bCanSuper = pGameStation->sBData.bIsSuper;
			InitialSuper(pGameStation->sBData.bIsSuper);
			return;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			GameStation_5 * pGameStation=(GameStation_5 *)pBuffer;

			m_sIData = pGameStation->s_Data;

			m_Logic.SetCardShape(m_sIData.iCardShape);

			ShowOrHideTime(m_byLgStation,true,m_sIData.byBTime,0,ID_BEGIN_TIME);

			//�Ŷӻ�����ʾ��ʼ��ť
			if(!(m_pGameFrame->GetGameInfo()->dwRoomRule&GRR_QUEUE_GAME))
			{
				ShowOrHideTime(m_byLgStation,true,m_sIData.byBTime,0,ID_BEGIN_TIME);

				ShowOrEnableControl(BN_GAME_START,true,true);
			}

			m_bCanSuper = pGameStation->sBData.bIsSuper;
			InitialSuper(pGameStation->sBData.bIsSuper);
			return;
		}
	}

	return;
}

//��Ϸ��Ϣ������
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	switch (nSubId)
	{
	case ASS_SUPER:
		{
			if(nLen!=sizeof(SuperStruct))
			{
				return 0;
			}

			SuperStruct * pCmd=(SuperStruct *)buffer;

			if (pCmd->bSuper != m_byLgStation)
			{
				return 0;
			}

			int itag = -1;
			for (int i = 0; i < PLAY_COUNT; ++i)
			{
				if (m_bSuperTag[i] != 0)
				{
					itag = i;
					break;			
				}
			}

			if (pCmd->bDeskStation != itag || pCmd->bSuperTag != m_bSuperTag[itag])
			{
				OutputDebugString("sdp_���� ʧ�ܣ�");
				m_pGameFrame->KillTimer(ID_SUPER_TIME);
				ShowControl(CHAO_DUAN_IMG1, false);
				ShowControl(CHAO_DUAN_IMG2, true);
				m_pGameFrame->SetTimer(ID_SUPER_TIME, 2000);
			}
			else
			{
				OutputDebugString("sdp_���� �ɹ���");
				m_pGameFrame->KillTimer(ID_SUPER_TIME);
				ShowControl(CHAO_DUAN_IMG1, true);
				ShowControl(CHAO_DUAN_IMG2, false);
				m_pGameFrame->SetTimer(ID_SUPER_TIME, 2000);
			}

			break;
		}
	case ASS_GM_GAME_STATION:						///<����״̬
		{	
			m_byLgStation = m_pGameFrame->GetMyDeskStation();
			m_bWatchMode = m_pGameFrame->GetIsWatching();	//��ȡ�Ƿ�Ϊ�Թ�״̬

			//������Ϸ״̬
			GameStationBase *pGameStationBase = (GameStationBase *)buffer;

			m_sBData = pGameStationBase->sBData;

			SetStationParameter(m_sBData.bGameStation);

			SetGameStation(buffer,nLen);

			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				ShowOrHideAutoImage(i,m_sBData.bIsAuto[i]);

				if(i == m_byLgStation)	//�Լ�λ�� ��ʾ�йܰ�ť
				{
					SOrHAutoBtn();
				}
			}

			break;
		}
	case ASS_GM_AGREE_GAME:
		{
			if(nLen!=sizeof(MSG_GR_R_UserAgree))
			{
				return 0;
			}

			MSG_GR_R_UserAgree * pUser=(MSG_GR_R_UserAgree *)buffer;

			if(m_byLgStation == pUser->bDeskStation)
			{
				ReSetGameUIStation(pUser->bDeskStation,true,true);		//���ý���
			}

			SOrHReadyMark(pUser->bDeskStation,pUser->bAgreeGame);

			break;
		}	
	case ASS_BEGIN_UPGRADE:		//��Ϸ��ʼ
		{
			if(nLen!=sizeof(BeginUpgradeStruct))
			{
				return 0;
			}

			InitGameData();	//��ʼ������

			ReSetGameUIStation(255);					//���ý���
			if (m_bSuperShow)
			{
				OnHitSuperBut();
			}
			


			BeginUpgradeStruct *pBegin = (BeginUpgradeStruct *)buffer;
			
			::CopyMemory(m_sBData.bIsInGame,pBegin->bIsInGame,sizeof(m_sBData.bIsInGame));

			SOrHBasePoint(m_sIData.iBaseNote,true);		//��ʾ�׷�

			SetStationParameter(GS_SEND_CARD);

			m_pSoundNews.byCureentAction = SOUND_START;
			g_PlayEvent.SetEvent();

			break;
		}
	case ASS_SEND_CARD:			//����������
		{
			if(nLen!=sizeof(SendCardStruct))
			{
				return 0;
			}
			
			SendCardStruct * pCardData=(SendCardStruct *)buffer;

			::CopyMemory(m_iUCCount,pCardData->iUCCount,sizeof(pCardData->iUCCount));

			::CopyMemory(m_byUCard,pCardData->byUCard,sizeof(pCardData->byUCard));

			m_pGameFrame->SetTimer(ID_SCARD_TIME,250);	//���÷��ƶ�ʱ��
	
			break;
		}
	case ASS_TAN_PAI:
		{
			if(nLen!=sizeof(TanPaiStruct))
			{
				return 0;
			}

			TanPaiStruct *pTanPai = (TanPaiStruct *)buffer;

			SorHTanPaiMark(pTanPai->byStaiton,true);

			if(m_byLgStation == pTanPai->byStaiton)
			{
				m_pGameFrame->KillTimer(ID_THINK_TIME);					//ɱ�����Ƽ�ʱ��

				ShowOrEnableControl(BN_TAN_PAI,false,false);			//̯�ư�ť
				ShowOrEnableControl(BN_CUE,false,false);				//��ʾ��ť
				ShowOrEnableControl(BN_CHONG_BAI,false,false);			//�ذڰ�ť

				::CopyMemory(m_byDownBCard[m_byLgStation],pTanPai->byDownCard,sizeof(pTanPai->byDownCard));
				if(0 != m_byDownBCard[m_byLgStation][0])
				{
					SOrHCard(m_byLgStation,2,true);
				}

				m_pSoundNews.byCureentAction = SOUND_OUT_CARD;
				m_pSoundNews.byCardShape = pTanPai->byShape;
				m_pSoundNews.byDeskStation = pTanPai->byStaiton;
				g_PlayEvent.SetEvent();	
			}

			break;
		}
	case ASS_CONTINUE_END:				//��Ϸ����
		{
			if(nLen!=sizeof(GameEndStruct))
			{
				return 0;
			}

			m_pGameFrame->KillTimer(ID_THINK_TIME);
			m_pGameFrame->KillTimer(ID_SCARD_TIME);

			GameEndStruct* pGameEnd = (GameEndStruct*)buffer;

			SOrHGResultData(pGameEnd,true);

			//�Ŷӻ�����ʾ��ʼ��ť
			if(!(m_pGameFrame->GetGameInfo()->dwRoomRule&GRR_QUEUE_GAME))
			{
				ShowOrEnableControl(BN_GAME_START,true,true);

				if(m_sBData.bIsAuto[m_byLgStation])
				{
					ShowOrHideTime(m_byLgStation,true,m_sIData.byRTime,0,0);
					m_pGameFrame->SetTimer(ID_RESULT_TIME,SEC);
				}
				else
				{
					ShowOrHideTime(m_byLgStation,true,m_sIData.byRTime,0,ID_RESULT_TIME);
				}
			}

			SetStationParameter(GS_WAIT_ARGEE);

			break;
		}
	case ASS_AUTO:
		{
			if(nLen!=sizeof(AutoCardStruct))
			{
				return 0;
			}

			AutoCardStruct *pAutoCard = (AutoCardStruct *)buffer;

			//�Լ� ��ı�״̬
			if(m_byLgStation == pAutoCard->bDeskStation && !m_bWatchMode)
			{
				m_sBData.bIsAuto[m_byLgStation] = pAutoCard->bAutoCard;	/**<�Զ�����*/

				SOrHAutoBtn();		//�����йܰ�ť״̬

				//��ȡ���ӵ�ǰ��ʾʱ��
				int iCurrentTime = 0;
				ITime *pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(TIME_0+3));
				if(NULL != pTime)
				{
					iCurrentTime = pTime->GetCurrentTimeNum();
				}

				if(GS_PLAY_GAME == GetStationParameter() && !m_sBData.bHaveBaiPai[m_byLgStation])	//�й�
				{	
					m_pGameFrame->KillTimer(ID_THINK_TIME);	//��ɱ
					if(m_sBData.bIsAuto[m_byLgStation])
					{
						m_pGameFrame->SetTimer(ID_THINK_TIME,SEC);
					}
					else
					{
						m_pGameFrame->SetTimer(ID_THINK_TIME,iCurrentTime*1000);
					}
					
				}
				else if(GS_WAIT_ARGEE == GetStationParameter() && GetControlEnable(BN_GAME_START))
				{
					m_pGameFrame->KillTimer(ID_RESULT_TIME);
					if(m_sBData.bIsAuto[m_byLgStation])
					{
						m_pGameFrame->SetTimer(ID_RESULT_TIME,SEC);
					}
					else
					{
						m_pGameFrame->SetTimer(ID_RESULT_TIME,iCurrentTime*1000);
					}
				}
			}

			ShowOrHideAutoImage(pAutoCard->bDeskStation,pAutoCard->bAutoCard);	//��ʾ�й�ͼƬ

			break;
		}
	case ASS_USER_QUIT_GAME:
		{
			if(nLen!=sizeof(UserQuitStruct))
			{
				return 0;
			}

			UserQuitStruct *pUserQuit = (UserQuitStruct *)buffer;

			InsertNormalMessage("%s ����,��Ϸ����",m_UserInfo[pUserQuit->bDeskStation].szNickName);

			m_pSoundNews.byCureentAction = SOUND_ESCAPLE;
			g_PlayEvent.SetEvent();

			break;
		}
	default:break;
	}

	return 0;
}

/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	//������������Ϣ
	if(pMessage->_uMessageType == UI_LBUTTONUP && !m_bWatchMode)
	{
		if(pMessage->_uControlID > BN_VOLUM_ON)
		{
			IButton* pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(pMessage->_uControlID));
			if(NULL != pButton && pButton->GetEnable())
			{
				m_pSoundNews.byCureentAction = SOUND_HIT_CARD;
				g_PlayEvent.SetEvent();		
			}
		}
	
		//CString str;
		//str.Format("xyh_postion:_uControlID=%d",pMessage->_uControlID);
		//OutputDebugString(str);

		switch(pMessage->_uControlID)
		{
		case CHAO_DUAN_BU1:
		case CHAO_DUAN_BU2:
		case CHAO_DUAN_BU3:
		case CHAO_DUAN_BU4:
		case CHAO_DUAN_BU5:
		case CHAO_DUAN_BU6:
		case CHAO_DUAN_BU7:
		case CHAO_DUAN_BU8:
		case CHAO_DUAN_BU9:
		case CHAO_DUAN_BU10:
		case CHAO_DUAN_BU11:
		case CHAO_DUAN_BU12:
			{
				OnHitSuperWinBut(pMessage->_uControlID);
				break;
			}
		case CHAO_DUAN_BU13:
			{
				OnHitResetSuperBut();
				break;
			}
		case CHAO_DUAN_BU14:
			{
				OnHitSuperOKBut();
				break;
			}
		case CHAO_DUAN_BU15:
			{
				OnHitSuperBut();
				break;
			}
		case BN_GAME_START:				//�����ʼ��ť
			{
				OnHitBegin();
				break;
			}
		case HAND_CARD_0 + 3:			//����
			{
				m_pSoundNews.byCureentAction = SOUND_HIT_CARD;
				g_PlayEvent.SetEvent();	

				OnLButtonUpOnCard();
				break;
			}
		case BN_CUE:					//��ʾ��ť
			{
				OnHitCue();
				break;
			}
		case BN_CHONG_BAI:				//�ذڰ�ť
			{
				OnHitChongBai();
				break;
			}
		case BN_TAN_PAI:				//̯�ư�ť
			{
				OnHitTanPai();
				break;
			}
		case BN_TUO_GUAN:	//�йܰ�ť
		case BN_CTUO_GUAN:	//ȡ���йܰ�ť
			{
				OnHitOkOrCancleAutoBtn(pMessage->_uControlID == BN_TUO_GUAN);
				break;
			}
		case BN_GAME_SET:		//���ð�ť
			{
				ShowControl(SET_CONTAIN,true);	//��ʾ��������

				IRadioButton *pIRadioButton = NULL;
				pIRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(SOUND_EFFECT));	//��Ч
				if(NULL != pIRadioButton)
				{
					pIRadioButton->SetIsSelect(m_bSound);
				}

				pIRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(PANG_GUAN));	//�Թ�
				if(NULL != pIRadioButton)
				{
					pIRadioButton->SetIsSelect(m_bWatch);
				}

				break;
			}
		case BN_SET_OK:			//����ȷ����ť
			{
				IRadioButton *pIRadioButton = NULL;
				pIRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(SOUND_EFFECT));	//��Ч
				if(NULL != pIRadioButton)
				{
					if(pIRadioButton->GetIsSelect())
					{
						m_bSound = true;
						StopPlaySound(false);
					}
					else
					{
						m_bSound = false;
						StopPlaySound(true);
					}	
				}

				pIRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(PANG_GUAN));	//��Ч
				if(NULL != pIRadioButton)
				{
					if(pIRadioButton->GetIsSelect())
					{
						m_bWatch = true;
					}
					else
					{
						m_bWatch = false;
					}	
				}

				//�Լ������Թ��� �����Ƿ�����Թ�����
				if(false == m_bWatchMode)
				{
					SendIsWatchData();
				}

				ShowControl(SET_CONTAIN,false);	//������������

				CString s = CINIFile::GetAppPath ();/////����·��
				CINIFile f( s +GET_SKIN_FOLDER(szTempStr) +"_c.ini");
				CString key = TEXT("config");

				f.SetKeyValString(key,"bSound",true==m_bSound?"1":"0");
				f.SetKeyValString(key,"bWatch",true==m_bWatch?"1":"0");

				break;
			}
		case BN_SET_CANCEL:		//����ȡ����ť
			{
				ShowControl(SET_CONTAIN,false);	//������������
				break;
			}
		default:
			{
				break;
			}
		}

		return 0;
	}

	//��� �Ҽ�������Ϣ
	if(pMessage->_uMessageType == UI_RBUTTONDOWN && !m_bWatchMode)
	{
	
	}

	if(pMessage->_uMessageType == UI_TIME_RUN)
	{
		if(pMessage->_uControlID == TIME_0 + 3)
		{
			m_pSoundNews.byCureentAction = SOUND_CLOCK;
			g_PlayEvent.SetEvent();	
		}

		return 0;
	}

	if (pMessage->_uMessageType == UI_MOUSEENTER)// ����ƶ���Ϣ
	{
		if(pMessage->_uControlID == CHAO_DUAN_BU15)
		{
			m_pGameFrame->KillTimer(ID_SUPER2_TIME);
			ShowControl(CHAO_DUAN_IMG3, true);
			m_pGameFrame->SetTimer(ID_SUPER2_TIME, 1000);	
		}

		return 0;
	}

	return 0;
}

void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
	case ID_SUPER2_TIME:
		{
			m_pGameFrame->KillTimer(ID_SUPER2_TIME);
			ShowControl(CHAO_DUAN_IMG3, false);
			break;
		}
	case ID_SUPER_TIME://����������ʾ��ʱ��
		{
			m_pGameFrame->KillTimer(ID_BEGIN_TIME);
			ShowControl(CHAO_DUAN_IMG1, false);
			ShowControl(CHAO_DUAN_IMG2, false);
			break;
		}
	case ID_BEGIN_TIME:			//��ʼ��ʱ��
		{
			m_pGameFrame->KillTimer(ID_BEGIN_TIME);

			m_pGameFrame->CloseClient();

			break;
		}
	case ID_THINK_TIME:			//˼����ʱ��
		{
			m_pGameFrame->KillTimer(ID_THINK_TIME);

			OnHitTanPai();

			break;
		}
	case ID_SCARD_TIME:			//���ƶ�ʱ��
		{
			m_iSCCount++;

			//��ʾ����
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(!m_sBData.bIsInGame[i])
					continue;

				ShowOrHideHandCard(i,m_byUCard[i],m_iSCCount,true);
			}

			m_pSoundNews.byCureentAction = SOUND_DEAL;
			g_PlayEvent.SetEvent();	

			if(m_iSCCount == USER_HAND_CARD)
			{
				m_pGameFrame->KillTimer(ID_SCARD_TIME);

				if(m_sBData.bIsAuto[m_byLgStation])
				{
					ShowOrHideTime(m_byLgStation,true,m_sIData.byTTime,0,0);	//��ʾ��ʱ��
					m_pGameFrame->SetTimer(ID_THINK_TIME,SEC);
				}
				else
				{
					ShowOrHideTime(m_byLgStation,true,m_sIData.byTTime,0,ID_THINK_TIME);
				}

				ShowOrEnableControl(BN_CUE,true,true);			//��ʾ��ť

				ShowOrEnableControl(BN_TAN_PAI,true,true);		//̯�ư�ť

				SetStationParameter(GS_PLAY_GAME);
			}

			break;
		}
	case ID_RESULT_TIME:
		{
			m_pGameFrame->KillTimer(ID_RESULT_TIME);

			if(!(m_pGameFrame->GetGameInfo()->dwRoomRule&GRR_QUEUE_GAME))
			{
				if(m_sBData.bIsAuto[m_byLgStation])
				{
					OnHitBegin();
				}
				else
				{
					m_pGameFrame->CloseClient();
				}
			}
			else
			{
				OnHitBegin();
			}

			break;
		}
	default:break;
	}
}

// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	return false;
}

/// ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_sBData.bGameStation = bGameStation;

	//if(m_pGameFrame)
	//{
	//	m_pGameFrame->SetStationParameter(bGameStation);
	//}

}

/**
* ��ȡ��Ϸ״̬ 
*
*return BYTE ��Ϸ״̬
*/
BYTE CNewGameClient::GetStationParameter()
{
	return m_sBData.bGameStation;
}	

///ֹͣ��Ϸ
bool CNewGameClient::OnGameQuiting()
{
	if(true == m_pGameFrame->GetIsWatching() || !m_sBData.bIsInGame[m_byLgStation])  ///�Թ��߿������˳�
	{
		return true ; 
	}

	if(GetStationParameter()>=GS_SEND_CARD && GetStationParameter()<GS_WAIT_NEXT)
	{
		char cKey[10];
		CString sPath=CBcfFile::GetAppPath();
		sPath.Delete(sPath.GetLength()-9,9);
		int iResult = 1;
		if(CBcfFile::IsFileExist(sPath + "SpecialRule.bcf"))
		{
			CBcfFile fsr(sPath + "SpecialRule.bcf");
			sprintf(cKey, "%d", NAME_ID);
			iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
		}

		CString strInfoMsg ;
		if(1 == iResult)
		{
			strInfoMsg.Format("�����˳���Ϸ�Զ��й�,�����Ҫ�˳���?");	
		}
		else
		{	
			strInfoMsg.Format("��������Ϸ�У�ȷ��Ҫ�˳���");
		}

		if(IDOK != AFCMessageBox(strInfoMsg, "��ʾ",AFC_YESNO)) 
		{
			return false ; 
		}
		m_pGameFrame->SendGameData(NULL,0,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}	
	return true;
}

//�û��뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	ReSetGameUIStation(nIndex,false);

	if(nIndex == m_byLgStation)
	{
		m_pGameFrame->CloseClient();
	}

	ShowOrHideAutoImage(nIndex,false);		//�����й�ͼ��
	
	return 0;
}

int CNewGameClient::GameUserCome()
{
	m_byLgStation = m_pGameFrame->GetMyDeskStation();
	m_bWatchMode = m_pGameFrame->GetIsWatching();	//��ȡ�Ƿ�Ϊ�Թ�״̬

	//��ȡ�����Ϣ ����¼����Ƿ�����Ϸ��
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		m_pGameFrame->GetUserInfo(i,m_UserInfo[i]);
	}

	return 0;
}

/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	return 0;
}

//�������ý���
int CNewGameClient::ResetGameFrame()
{
	return 0;
}

//��ʼ��ť����
void CNewGameClient::OnHitBegin(void)
{
	ShowControl(BN_GAME_START,false);

	m_pGameFrame->KillTimer(ID_BEGIN_TIME);			//�رտ�ʼ��ʱ��

	m_pGameFrame->KillTimer(ID_RESULT_TIME);			//�رռ�ʱ��

	ShowOrHideTime(m_byLgStation,false,0,0);		//���ؼ�ʱ��

	m_pGameFrame->ClickContinue();

	return;
}

/**
* ������������¼�
*
* @return void
*/
void CNewGameClient::OnLButtonUpOnCard()
{
	if (GetStationParameter()!=GS_PLAY_GAME && m_bWatchMode)
	{
		return;
	}

	int iUpCCount = 0;		
	BYTE byUpCard[USER_HAND_CARD] = {0};

	IOperateCard *pOperateCard = NULL;
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(HAND_CARD_0+3));
	if(NULL != pOperateCard)
	{
		iUpCCount = pOperateCard->GetUpCardList(byUpCard);
	}

	bool bIsBull = false;	//�ڵ����Ƿ���ţ
	BYTE byTCard[USER_HAND_CARD] = {0};	
	if(2 == iUpCCount)
	{
		::CopyMemory(byTCard,m_byUCard[m_byLgStation],sizeof(m_byUCard[m_byLgStation]));

		m_Logic.RemoveCard(byUpCard,iUpCCount,byTCard,USER_HAND_CARD);

		if (m_Logic.IsBull(byTCard,3))
		{	
			for (int i=0; i<3; ++i)
			{
				m_byDownBCard[m_byLgStation][i]=byTCard[i];
			}

			bIsBull = true;
		}
	}

	if(iUpCCount == 3)
	{
		if (m_Logic.IsBull(byUpCard,iUpCCount))
		{
			for (int i=0; i<3; ++i)
			{
				m_byDownBCard[m_byLgStation][i]=byUpCard[i];
			}

			bIsBull = true;
		}
	}

	if(bIsBull)		//��ţ
	{
		SOrHCard(m_byLgStation,2,true);
		ShowOrEnableControl(BN_CUE,false,false);		//��ʾ��ť
		ShowOrEnableControl(BN_CHONG_BAI,true,true);	//�ذڰ�ť
	}
	else 			//��ţ
	{
		SOrHUAndDCard(m_byLgStation,m_byDownBCard[m_byLgStation],3,false);
		ShowOrEnableControl(BN_CUE,true,true);			//��ʾ��ť
		ShowOrEnableControl(BN_CHONG_BAI,false,false);	//�ذڰ�ť
	}

	return;
}


/**
* ��ʾ
*/
void CNewGameClient::OnHitCue(bool bShowChongPai)
{
	ShowOrEnableControl(BN_CUE,false,false);				//��ʾ��ť

	m_Logic.GetBull(m_byUCard[m_byLgStation],m_iUCCount[m_byLgStation], m_byDownBCard[m_byLgStation]);

	SOrHCard(m_byLgStation,2,true);

	if(bShowChongPai)
	{
		//��ʾ�� û��ţ ���Ű�ť���ɵ��
		bool bTemp = UG_NO_POINT != m_Logic.GetShape(m_byUCard[m_byLgStation],m_iUCCount[m_byLgStation],m_byDownBCard[m_byLgStation]);
		
		ShowOrEnableControl(BN_CHONG_BAI,true,bTemp);			//�ذڰ�ť
	}

	
}

/**
* �ذ�
*/
void CNewGameClient::OnHitChongBai()
{
	ShowOrEnableControl(BN_CHONG_BAI,false,false);			//�ذڰ�ť

	SOrHCard(m_byLgStation,1,true);

	::memset(m_byDownBCard,0,sizeof(m_byDownBCard));		///�����ţ��

	ShowOrEnableControl(BN_CUE,true,true);					//��ʾ��ť
}

/**
* ̯��
*/
void CNewGameClient::OnHitTanPai()
{
	if(GetControlShow(HAND_CARD_0 + 3))
	{
		OnHitCue(false);
	}

	TanPaiStruct tanpai;

	tanpai.byStaiton = m_byLgStation;
	::CopyMemory(tanpai.byDownCard,m_byDownBCard[m_byLgStation],sizeof(m_byDownBCard[m_byLgStation]));

	m_pGameFrame->SendGameData(ASS_TAN_PAI,&tanpai,sizeof(TanPaiStruct));
}

/**
* ����й� �� ȡ���й� ��ť
*
* @param [in] bool bAutoCard; true: �й�;false:ȡ���й�
*
* @return void
*/
void CNewGameClient::OnHitOkOrCancleAutoBtn(bool bAutoCard)
{
	AutoCardStruct s_autocard;
	s_autocard.bDeskStation = m_byLgStation;
	s_autocard.bAutoCard = bAutoCard;

	m_pGameFrame->SendGameData(ASS_AUTO,&s_autocard,sizeof(AutoCardStruct));
}



//���Ż��㵽��ͼλ��
BYTE CNewGameClient::ViewStation(BYTE bDeskStation)
{
	return m_pGameFrame->DeskStation2View(bDeskStation);
}

/**
* �����Ƿ�����Թ�����
*
* @return void
*/
void CNewGameClient::SendIsWatchData()
{
	WatchOtherStruct s_watchother;

	s_watchother.bWatch = m_bWatch;

	m_pGameFrame->SendGameData(ASS_WATCH_OTHER,&s_watchother,sizeof(WatchOtherStruct));
}

/**
* ������ͨ��Ϣ
*
* @param CString strTemp [in] ��Ϣ����
*
* @param BYTE bDeskStation [in] ˵�����λ��
*
* @return void
*/
void CNewGameClient::InsertNormalMessage(const char *p, ...)
{
	va_list pArg;
	va_start(pArg, p);

	char szMessage[1024] = { 0 };

#ifdef WIN32
	_vsnprintf(szMessage, 1023, p, pArg);
#else
	vsnprintf(szMessage, 1023, p, pArg);
#endif
	va_end(pArg);

	m_pGameFrame->InsertNormalMessage(szMessage);
}

/**
* ��ʾ/���� �ؼ�
*
* @param [in] int iControlID �ؼ�ID
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return BOOL �Ƿ����óɹ�
*/
BOOL CNewGameClient::ShowControl(int iControlID,bool bFlag)
{
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		pControl->SetControlVisible(bFlag);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
* ���ÿ���/���� �ؼ���ֻ֧�ְ�ť��
*
* @param [in] int iControlID �ؼ�ID
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return BOOL �Ƿ����óɹ�
*/
BOOL CNewGameClient::EnableControl(int iControlID,bool bFlag)
{
	IButton *pControl = NULL;

	pControl = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		pControl->SetEnable(bFlag);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
* ��ѯ��ť����/���� �ؼ���ֻ֧�ְ�ť��
*
* @param [in] int iControlID �ؼ�ID
*
* @return BOOL ���ؿؼ�״̬ TRUE:���ã�FALSE:������
*/
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

/**
* ��ѯ��ť��ʾ/���� �ؼ���ֻ֧�ְ�ť��
*
* @param [in] int iControlID �ؼ�ID
*
* @return BOOL ���ؿؼ�״̬ TRUE:��ʾ��FALSE:����ʾ
*/
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


/**
* ��ʾ/���� �ؼ���ֻ֧�ְ�ť��
*
* @param [in] int iControlID �ؼ�ID
*
* @param [in] bool sFlag ��ʾ��־true:��ʾ��false:����
*
* @param [in] bool eFlag ���ñ�־true:���ã�false:����
*
* @return void
*/
void CNewGameClient::ShowOrEnableControl(int iControlID,bool sFlag,bool eFlag)
{
	if(m_bWatchMode)
	{
		sFlag = false;

		eFlag = false;
	}

	ShowControl(iControlID,sFlag);	//��ʾ / ����
	EnableControl(iControlID,eFlag);	//���� / ����
}

/**
* ��ʾ/���� ���ֿؼ�
*
* @param [in] int iControlID �ؼ�ID
*
* @param [in] __int64 i64Num ��ʾ��ֵ
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
void CNewGameClient::ShowOrHideNumControl(int iControlID,__int64 i64Num,bool bFlag,bool bSign)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iControlID));
	if(NULL != pNum)
	{
		if(bSign)
		{
			pNum->SetHaveSymbol(bSign);

			CString strPath;
			if(i64Num >= 0)
			{
				strPath.Format(".\\resources\\image\\defen4.png");
			}
			else
			{
				strPath.Format(".\\resources\\image\\defen3.png");
			}
			pNum->SetImagePath(CA2W(strPath));
		}
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}

/**
* �����ⲿ����
*
* @param [in] int iControlID �����ؼ�ID
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @param [in] CString StrTemp ����·��
*
* @return void
*/
void CNewGameClient::PlayExAnimate(int iControlID, bool bFlag, CString StrTemp)
{
	IExAnimate *pExAnimate = NULL;
	pExAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(iControlID));
	if(NULL != pExAnimate)
	{
		if(StrTemp != "")	//���ö���·��
		{
			pExAnimate->SetExtFilePath(CA2W(StrTemp));
			pExAnimate->LoadFromFile();
		}	
		pExAnimate->SetPlayTotalTime(1500);
		pExAnimate->SetIsLoop(false);		//��ѭ������
		pExAnimate->SetPlayState(bFlag);
		pExAnimate->SetControlVisible(bFlag);
	}
}


/**
* ��ʾ�����ص���ʱʱ��
*	
* @param [in] BYTE bDeskStation ���λ��
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @param [in] int nShowMaxNum ���ֵ
*
* @param [in] int nShowMinNum ��СֵĬ��Ϊ0
*
* @param [in] int iTimeID ��ʱ��ID Ĭ��ֵΪ 0 ��Ч
*
* @return void
*/
void CNewGameClient::ShowOrHideTime(BYTE bDeskStation,bool bFlag,int nShowMaxNum,int nShowMinNum,int iTimeID)
{
	BYTE iViewStation = ViewStation(bDeskStation);

	ShowControl(TIME_CONTAINER_0+iViewStation,bFlag);

	ITime *pTime = NULL ;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(TIME_0 + iViewStation));
	if(NULL != pTime)
	{
		if(bFlag)
		{
			pTime->SetPlaySoundNum(5);
			pTime->SetShowMaxNum(nShowMaxNum);
			pTime->SetShowMinNum(nShowMinNum);
			pTime->SetPLay(true);
		}
		pTime->SetControlVisible(bFlag);
	}

	//��ʱ��ID �� �߼�λ�� �ж�
	if(0 == iTimeID || bDeskStation != m_byLgStation || m_bWatchMode)
	{
		return;
	}

	m_pGameFrame->KillTimer(iTimeID);	//����ʱ��ɱ����ʱ��

	if(bFlag)
	{	
		m_pGameFrame->SetTimer(iTimeID,nShowMaxNum*SEC);		
	}
}

/**
* ��ʾ/���� ׼��ͼ��
*
* @param [in] BYTE bDeskStation ���λ��
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
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

	/**
	* ��ʾ/���ص׷�
	*
	* @param [in] __int64 i64Num ��ʾ��ֵ
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @param [in] bool bHaveSign �Ƿ��з��� true:�У�false:û��
	*
	* @return void
	*/
void CNewGameClient::SOrHBasePoint(__int64 i64Num,bool bFlag,bool bHaveSign)
{
	//���ؿؼ�
	if(!bFlag)
	{
		ShowControl(IMG_BASE_POINT_MARK,bFlag);
		ShowControl(INM_BASE_POINT,bFlag);
		return;
	}

	// ���õ׷����ֿؼ�ͼƬ
	NEW_NUM_STRUCT NumStruct;
	INum *pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(INM_BASE_POINT));
	if (NULL != pNum)
	{
		NumStruct.bShowComma = m_bShowComma;		//��ʾ����
		NumStruct.bShowPlusSignOrMinusSign = bHaveSign;	//��ʾ�ӺŻ����
		NumStruct.iShowCommaIndex=m_bySpaceBits;		//����λ��������һ���ֺ�
		NumStruct.iShowZeroType=0;			//��ʾ����Ϊ0ʱ��ʾ�ӺŻ��Ǽ��ţ�����ֻ��ʾ0��0ʱ�Ӽ�������ʾ��1ʱ��ʾ�Ӻţ���2��ʾ���ţ�

		pNum->SetNewNumStruct(NumStruct);
		pNum->SetRankType(0);
		pNum->SetNewNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}

	IImage *pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(IMG_BASE_POINT_MARK));
	if(NULL != pImage)
	{
		CString strPath = "resources\\image\\deifeng_bg.png";
		if(bHaveSign)
		{
			strPath = "resources\\image\\benfang.png";
		}

		pImage->LoadPic(CA2W(strPath));
		pImage->SetControlVisible(bFlag);
	}
}

/**
* ��ʾ�����������ƿؼ�
*
* @param [in] BYTE bDeskStation ���λ��
*
* @param [in] BYTE iCard[] ������
*
* @param [in] BYTE iCardCount ������
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
void CNewGameClient::ShowOrHideHandCard(BYTE bDeskStation,BYTE iCard[],BYTE iCardCount,bool bFlag)
{
	BYTE iViewStation = ViewStation(bDeskStation);

	//ֻ���Լ����пɲ�����
	if(bDeskStation == m_byLgStation)
	{
		if(!m_bWatchMode)
		{
			IOperateCard *pOperateCard = NULL;
			pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(HAND_CARD_0+iViewStation));
			if(NULL != pOperateCard)
			{
				if(NULL != iCard)
				{
					pOperateCard->SetCardList(iCard,iCardCount);
				}
				pOperateCard->SetControlVisible(bFlag);
			}
		}
		else
		{
			INoMoveCard *pNoMoveCard = NULL;
			pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DOWN_CARD_0 + iViewStation));
			if(NULL != pNoMoveCard)
			{
				if(NULL != iCard)
				{
					pNoMoveCard->SetCardList(iCard,iCardCount);
				}
				pNoMoveCard->SetControlVisible(bFlag);
			}
		}
	}
	else
	{
		INoMoveCard *pNoMoveCard = NULL;
		pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(HAND_CARD_0 + iViewStation));
		if(NULL != pNoMoveCard)
		{
			if(NULL != iCard)
			{
				pNoMoveCard->SetCardList(iCard,iCardCount);
			}
			pNoMoveCard->SetControlVisible(bFlag);
		}
	}	
}

/**
* ��ʾ�����ذ��ƺ�����ƺ����ƿؼ�
*
* @param [in] BYTE bDeskStation ���λ��
*
* @param [in] BYTE iDownCard[] ������
*
* @param [in] BYTE iDownCCount ������
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
void CNewGameClient::SOrHUAndDCard(BYTE bDeskStation,BYTE iDownCard[],BYTE iDownCCount,bool bFlag)
{
	BYTE iViewStation = ViewStation(bDeskStation);

	BYTE iUpCard[USER_HAND_CARD] = {0};
	int iUpCCount = 0;

	BYTE byTCard[USER_HAND_CARD] = {0};

	::CopyMemory(byTCard,m_byUCard[bDeskStation],sizeof(m_byUCard[bDeskStation]));
	
	if(m_Logic.RemoveCard(iDownCard,iDownCCount,byTCard,USER_HAND_CARD))
	{
		::CopyMemory(iUpCard,byTCard,sizeof(byTCard));
		iUpCCount = USER_HAND_CARD - iDownCCount; 
	}

	INoMoveCard *pUNoMoveCard = NULL;
	pUNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(UP_CARD_0 + iViewStation));
	if(NULL != pUNoMoveCard)
	{
		pUNoMoveCard->SetCardList(iUpCard,iUpCCount);
		pUNoMoveCard->SetControlVisible(bFlag);
		
	}

	INoMoveCard *pDNoMoveCard = NULL;
	pDNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DOWN_CARD_0 + iViewStation));
	if(NULL != pDNoMoveCard)
	{
		pDNoMoveCard->SetControlVisible(bFlag);

		if(bFlag && 0 == iUpCCount)
		{
			pDNoMoveCard->SetCardList(m_byUCard[bDeskStation],USER_HAND_CARD);
			return;
		}
		pDNoMoveCard->SetCardList(iDownCard,iDownCCount);	
	}
}

/**
* ��ʾ��������������� ��������ǰ�Ͱ��ƺ�
*
* @param [in] BYTE bDeskStation ���λ��
*
* @param [in] BYTE bType 1:����ǰ״̬ 2:���ƺ�״̬ 
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
void CNewGameClient::SOrHCard(BYTE bDeskStation,BYTE bType,bool bFlag)
{
	switch(bType)
	{
	case 1:				//����ǰ״̬		
		{
			SOrHUAndDCard(m_byLgStation,m_byDownBCard[bDeskStation],3,false);
			ShowOrHideHandCard(bDeskStation,m_byUCard[bDeskStation],m_iUCCount[bDeskStation],bFlag);
			break;
		}
	case 2:				//���ƺ�״̬ 
		{
			ShowOrHideHandCard(bDeskStation,m_byUCard[bDeskStation],m_iUCCount[bDeskStation],false);
			SOrHUAndDCard(bDeskStation,m_byDownBCard[bDeskStation],3,bFlag);
			break;
		}
	default:
		{
			SOrHUAndDCard(bDeskStation,m_byDownBCard[bDeskStation],3,false);
			ShowOrHideHandCard(bDeskStation,m_byUCard[bDeskStation],m_iUCCount[bDeskStation],false);
			break;
		}
	}
}

/**
* ��ʾ������ ̯�Ʊ�־
*
* @param [in]  BYTE bDeskStation ���λ��
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
void CNewGameClient::SorHTanPaiMark(BYTE bDeskStation,bool bFlag)
{
	BYTE byView = ViewStation(bDeskStation);

	ShowControl(IMG_TAN_PAI_0 + byView,bFlag);
}

/**
* ��ʾ������ �������
*
* @param [in]  BYTE bDeskStation ���λ��
*
* @param [in]  int iCardShape ����
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
void CNewGameClient::SorHCardShape(BYTE bDeskStation,int iCardShape,bool bFlag)
{
	BYTE byView = ViewStation(bDeskStation);
	
	IImage *pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(IMG_CARD_SHAPE_0 + byView));
	if(NULL != pImage)
	{
		if(bFlag)
		{
			int iTemp = iCardShape - UG_NO_POINT;
			pImage->SetSrcX(iTemp * 112);
		}

		pImage->SetControlVisible(bFlag);
	}
}


/**
* ��ʾ������ ��������
*
* @param [in] GameEndStruct *pGameEnd ��Ϸ����ͳ�����ݰ�
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
void CNewGameClient::SOrHGResultData(GameEndStruct *pGameEnd,bool bFlag)
{
	if(NULL == pGameEnd)
	{
		return;
	}

	::CopyMemory(m_byDownBCard,pGameEnd->byDownCard,sizeof(m_byDownBCard));
	::CopyMemory(m_byUCard,pGameEnd->byUCard,sizeof(m_byUCard));
	
	if (pGameEnd->byCowUserNum >= 2)
	{
		IExAnimate * pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(BN_TWO_CATTLE_ANIME));
		if (pAnimate != NULL)
		{
			pAnimate->SetPlayState(true);
			pAnimate->SetControlVisible(true);
		}
	}
	else
	{
		UserInfoForGame userInfo;
		::memset(&userInfo, 0, sizeof(userInfo));
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			BYTE byView = m_pGameFrame->DeskStation2View(i);
			if (m_pGameFrame->GetUserInfo(i, userInfo))
			{
				if (pGameEnd->bIsCowUser[i])
				{
					IExAnimate * pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(BN_ONE_CATTLE_ANIME_0 + byView));
					if (pAnimate != NULL)
					{
						pAnimate->SetPlayState(true);
						pAnimate->SetControlVisible(true);
						//����ţţ����Ч
						m_pSoundNews.byCureentAction = SOUND_NIUNIU;
						g_PlayEvent.SetEvent();

					}
				}
			}
		}
	}
	
	CString strMessage;
	strMessage.Format("*********������Ϣ*********");
	m_pGameFrame->InsertNormalMessage(strMessage.GetBuffer());
	UserInfoForGame userInfo;
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pGameFrame->GetUserInfo(i, userInfo))
		{			
			strMessage.Format("%s,���:%I64d", userInfo.szNickName, pGameEnd->iMoney[i]);
			m_pGameFrame->InsertNormalMessage(strMessage.GetBuffer());
		}

		if(pGameEnd->bIsPrepare[i])
		{
			SOrHCard(i,2,true);

			SorHCardShape(i,pGameEnd->iCardShape[i],true);
		}
		
		SorHTanPaiMark(i,false);
	}

	strMessage.Format("**************************");
	m_pGameFrame->InsertNormalMessage(strMessage.GetBuffer());

	SOrHBasePoint(pGameEnd->iMoney[m_byLgStation],true,true);		//��ʾ������

	if(pGameEnd->iMoney[m_byLgStation] > 0)	//Ӯ����Ҳ�����Ч
	{
		m_pSoundNews.byCureentAction = SOUND_WIN;
	}
	else
	{
		m_pSoundNews.byCureentAction = SOUND_LOSE;
	}
	g_PlayEvent.SetEvent();
}

/**
* ��ʾ������ �йܰ�ť
*
* @return void
*/
void CNewGameClient::SOrHAutoBtn()
{
	if(255 == m_byLgStation)
	{
		return;
	}

	ShowOrEnableControl(BN_TUO_GUAN,!m_sBData.bIsAuto[m_byLgStation],!m_sBData.bIsAuto[m_byLgStation]);		//�йܰ�ť
	ShowOrEnableControl(BN_CTUO_GUAN,m_sBData.bIsAuto[m_byLgStation],m_sBData.bIsAuto[m_byLgStation]);		//ȡ���йܰ�ť
}

/**
* ��ʾ������ �й�ͼƬ
*
* @param [in] BYTE bDeskStation ���λ��
*
* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
*
* @return void
*/
void CNewGameClient::ShowOrHideAutoImage(BYTE bDeskStation,bool bFlag)
{
	BYTE iViewStation = ViewStation(bDeskStation);	

	ShowControl(IMG_AUTO_0 + iViewStation,bFlag);
}

//ϵ�y�������Ƿ���ĳ����
BOOL CNewGameClient::ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard)
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

UINT CNewGameClient::PlaySond(LPVOID lpParam)
{
	CNewGameClient *pClient = (CNewGameClient*)lpParam;

	while(true)
	{
		WaitForSingleObject(g_PlayEvent,INFINITE);

		if(pClient->m_pSoundNews.byCureentAction == SOUND_OUT_CARD)
		{
			pClient->PlayCardShapSound();
		}
		else
		{
			pClient->PlayMotionSound();
		}

		if(pClient->m_pSoundNews.bExitProess)
		{
			DWORD dwExitCode;
			GetExitCodeThread(pClient->m_tSond->m_hThread,&dwExitCode);
			AfxEndThread(dwExitCode);
			return 0;
		}
	}
	return 0;
}

/**
* ����������Ч
*
* @return void
*/
void CNewGameClient::PlayCardShapSound()
{
	if(false == m_bSound || NULL == m_SoundEngine)
	{
		return;
	}

	CString folder,filename;

	UserInfoForGame UserInfo;
	if(m_pGameFrame->GetUserInfo(m_pSoundNews.byDeskStation,UserInfo))
	{
		if(UserInfo.bBoy)
		{
			folder=".\\music\\Shape\\boy\\";
			filename.Format("boy_nn_%d.ogg",m_pSoundNews.byCardShape-UG_NO_POINT);
		}
		else
		{
			folder=".\\music\\Shape\\girl\\";
			filename.Format("girl_nn_%d.ogg",m_pSoundNews.byCardShape-UG_NO_POINT);
		}
	}

	TCHAR fi[MAX_PATH+1] = {0};
	wsprintf(fi,"%s%s",folder,filename);
	m_SoundEngine->play2D(fi,false);

	return;
}

/**
* ���Ŷ�����Ч
*
* @param [in] int MotionType ���嶯��
*
* @param [in] bool playLooped ����ģʽ;true:ѭ����ģʽ,false:ֻ����һ��
*
* @return void
*/
void CNewGameClient::PlayMotionSound()
{
	if(false == m_bSound || NULL == m_SoundEngine)
	{
		return;
	}

	CString folder;

	switch(m_pSoundNews.byCureentAction)
	{
	case SOUND_START:
		{
			folder = ".\\music\\start.ogg";
			break;
		}
	case SOUND_DEAL:
		{
			folder = ".\\music\\deal.ogg";
			break;
		}
	case SOUND_HIT_CARD:
		{
			folder = ".\\music\\hitcard.ogg";
			break;
		}
	case SOUND_CLOCK:
		{
			folder = ".\\music\\clock.ogg";
			break;
		}
	case SOUND_ESCAPLE:
		{
			folder = ".\\music\\escaple.ogg";
			break;
		}
	case SOUND_WIN:
		{
			folder = ".\\music\\win.ogg";
			break;
		}
	case SOUND_LOSE:
		{
			folder = ".\\music\\lose.ogg";
			break;
		}
	case SOUND_NIUNIU:
		{
			
			folder = ".\\music\\bullsmile.wav";
			break;
		}
	default:break;
	}

	TCHAR fi[MAX_PATH+1] = {0};

	wsprintf(fi,"%s",folder);
	m_SoundEngine->play2D(fi,false);

	return;
}

/**
* ֹͣ������Ч
*
* @param [in] bool bFlag  �Ƿ���ͣ
*
* @return void
*/
void CNewGameClient::StopPlaySound(bool bFlag)
{
	if(NULL == m_SoundEngine)
	{
		return;
	}

	m_SoundEngine->setAllSoundsPaused(bFlag);
}

/**
* �ж��ǲ��Ǵ��ڸ����
*
* @param [in] BYTE bDeskStation ���λ��
*
* @return �������
*/
bool CNewGameClient::IsPlayer(BYTE bDeskStation)
{
	if (bDeskStation >= 0 && bDeskStation < PLAY_COUNT)
	{
		return true;
	}

	return false;
}

/**
* ��ʼ����������
*
* @param void
*
* @return void
*/
void CNewGameClient::InitialSuper(bool bFlag)
{
	ZeroMemory(m_bSuperTag, sizeof(m_bSuperTag));
	m_bSuperSet = false;
	if (bFlag)
	{
		for (int i = 0; i <= (CHAO_DUAN_BU14 - CHAO_DUAN_BU1); ++i)
		{
			//ֻ�г��˰�ť��ʾ
			ShowControl(CHAO_DUAN_BU1 + i, false);
			EnableControl(CHAO_DUAN_BU15, false);
		}
		ShowControl(CHAO_DUAN_BU15, true);
		EnableControl(CHAO_DUAN_BU15, true);
	}
	else
	{
		for (int i = 0; i <= (CHAO_DUAN_BU15 - CHAO_DUAN_BU1); ++i)
		{
			//ֻ�г��˰�ť��ʾ
			ShowControl(CHAO_DUAN_BU1 + i, false);
			EnableControl(CHAO_DUAN_BU1 + i, false);
		}		
	}

	return;
}

/**
* ������ó������ð�ť
*
* @param void
*
* @return void
*/
void CNewGameClient::OnHitResetSuperBut()
{
	ZeroMemory(m_bSuperTag, sizeof(m_bSuperTag));
	m_bSuperSet = false;

	//�û���Ҫ��������
	for (int i = 0; i <= (CHAO_DUAN_BU12 - CHAO_DUAN_BU1); ++i)
	{
		ShowControl(CHAO_DUAN_BU1 + i, true);
		EnableControl(CHAO_DUAN_BU1 + i, true);
	}

	ShowControl(CHAO_DUAN_BU13, true);
	EnableControl(CHAO_DUAN_BU13, true);
	ShowControl(CHAO_DUAN_BU14, true);
	EnableControl(CHAO_DUAN_BU14, false);

	return;
}

/**
* �����������Ӯ��ť
*
* @param void
*
* @return void
*/
void CNewGameClient::OnHitSuperWinBut(int iControlID)
{
	if (iControlID < CHAO_DUAN_BU1 || iControlID > CHAO_DUAN_BU12)
	{
		return;
	}

	int ib = iControlID - CHAO_DUAN_BU1;
	int ia = (ib + PLAY_COUNT) % PLAY_COUNT;//���Ǹ���ͼλ��
	int iaForLogicID = -1;//�߼�λ��

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (ViewStation(i) == ia)
		{
			iaForLogicID = i;
			break;
		}
	}

	if (!IsPlayer(iaForLogicID))
	{
		return;
	}

	ZeroMemory(m_bSuperTag, sizeof(m_bSuperTag));
	//��Ӯ������
	m_bSuperTag[iaForLogicID] = (ia == ib)?1:2;

	m_bSuperSet = true;

	CString sdp;
	sdp.Format("sdp_���� �ͻ��� ��ͼλ�� = %d,�߼�λ��=%d",ia, iaForLogicID);
	OutputDebugString(sdp);


	//������ĸ���� ��������Ұ�ť�ûң�����ʾ����ҵ�ǰ������Ч�İ�ť
	for (int i = 0; i <= (CHAO_DUAN_BU12 - CHAO_DUAN_BU1); ++i)
	{
		ShowControl(CHAO_DUAN_BU1 + i, true);
		if (i == ib)
		{
			EnableControl(CHAO_DUAN_BU1 + i, true);
		}
		else
		{
			EnableControl(CHAO_DUAN_BU1 + i, false);
		}
	}

	ShowControl(CHAO_DUAN_BU14, true);
	EnableControl(CHAO_DUAN_BU14, true);

	return;
}

/**
* ������˰�ť
*
* @param void
*
* @return void
*/
void CNewGameClient::OnHitSuperBut()
{
	if (m_bSuperShow)
	{
		InitialSuper(m_bSuperShow);
	}
	else
	{
		OnHitResetSuperBut();
	}

	m_bSuperShow = !m_bSuperShow;

	return;
}

/**
* �������������Ч��ť
*
* @param void
*
* @return void
*/
void CNewGameClient::OnHitSuperOKBut()
{
	SuperStruct cmd;
	cmd.bSuper = m_byLgStation;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_bSuperTag[i] != 0)
		{
			cmd.bDeskStation = i;
			cmd.bSuperTag = m_bSuperTag[i];
		}
	}

	m_pGameFrame->SendGameData(ASS_SUPER,&cmd,sizeof(SuperStruct));
	
	return;
}