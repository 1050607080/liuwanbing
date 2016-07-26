#include "stdafx.h"
#include "NewGameClient.h"
//#include "IGameFrame.h"
#include "../common/IGameFrame.h"
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
	if(m_SoundEngineGame)
	{
		m_SoundEngineGame->drop();
	}

	if(m_SoundEnginBg)
	{
		m_SoundEnginBg->drop() ; 
	} 

}
//-----------------------------------------------------------------------------------
/// ��ʼ��
int CNewGameClient::Initial()
{
	
	return 0;
}
//-----------------------------------------------------------------------------------
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	//����UI
	wchar_t wszUIFile[MAX_PATH];
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
	m_pUI->LoadData(wszUIFile);
	ResetGameDialog();
	return 0;
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
/*
@brief:���ð�ť�ؼ������ƶ�ͼƬ
*/
void CNewGameClient::SetBtLoadSrc(const int iButtonID,CString sImagSrc,bool bFlag)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{	
		pButton->LoadPic(CA2W(sImagSrc));
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
	/*m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	*/
	m_iMyDeskStation = 255;									///����Լ�����Ϸλ��
	m_byGameStation=0 ;										//��ǰ����Ϸ״̬
	m_bGameBeginWaitTime=0;									//��Ϸ��ʼ�ȴ�ʱ��
	m_bGameBetTime=0;										//��Ϸ��עʱ��
	m_bCallCardTime=0;										//��Ϸ����ʱ��
	m_bIsSuperUser	 = false;								//�Ƿ񳬼��û�
	memset(m_iGameSet,1,sizeof(m_iGameSet));
	SetStationParameter(GS_WAIT_ARGEE);
	//������Ϸ����
	ResetGameData();
}


//����UI���� 
void	CNewGameClient::ResetGameDialog()
{
	OutputDebugString("lw::ResetGameDialog");
	//���ؿ�ʼ��ť
	SetBtVisible(CTN_1014089_BTN_1004006,false);

	for(int i=0;i<PLAY_COUNT;i++)
	{
		//׼��ͼƬ����
		SetImageVisible(CTN_1014089_IMG_2000167+i,false);
		//����ʱ��
		SetContainVisible(CTN_1014089_CTN_2000110+i,false);
		//������ע����ͼƬ
		SetImageVisible(CTN_1014089_IMG_2000199+i,false);
		//����ׯ��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004060+i,false);
	}
	//������������
	SetContainVisible(CTN_1014089_CTN_2000171,false);
	//���ؽ��������
	SetContainVisible(CTN_1014089_CTN_1004100,false);
	//������������
	SetContainVisible(CTN_1014089_CTN_2000221,true);

	
	SetContainVisible(CTN_1014089_CTN_2000246,false);

	SetTextinfo(CTN_1014089_CTN_2000221_TEXT_2000224,0,true);
	SetTextinfo(CTN_1014089_CTN_2000221_TEXT_2000225,0,true);

	for(int i=0;i<5;i++)
	{
		SetImageVisible(CTN_1014089_IMG_2000231+i,false);
	}
	SetImageVisible(CTN_1014089_IMG_1005003,false);
	for(int i=0;i<4;i++)
	{
		INoMoveCard *noMove;
		noMove=dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(CTN_1014089_NoMoveCard_1004080+i));
		if(noMove)
		{
			noMove->SetControlVisible(false);
		}
	}
	
	IOperateCard *moveOperate;
	moveOperate=dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(CTN_1014089_OperateCard_2000230));
	if(moveOperate)
	{
		moveOperate->SetControlVisible(false);
	}
	for(int i=0;i<4;i++)
	{
		SetImageVisible(CTN_1014089_IMG_2000240+i,false);
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetInumInfo(CTN_1014089_Num_2000236+i,0,false);
	}
	SetContainVisible(CTN_900000_CTN_1003510,false);
}

//������Ϸ����
void CNewGameClient::ResetGameData()
{
		m_byGameStation=GS_WAIT_ARGEE;
		m_IsJoin=false;
		m_bNtPeople=255;
		m_iClockLabel=0;											//��ǰʱ�ӱ�ǩ
		m_iCurrentID=0;											//��ǰʱ��ID;
		m_iNoteLabel=0;
		m_i64CurrentNote=0;
		m_IsNote=false;
		m_iSendCardLabel=0;
		m_bScorePeople=255;
		memset(m_i64UserBet,0,sizeof(m_i64UserBet));			//�û�����ע
		memset(m_iUserCard,0,sizeof(m_iUserCard));
		m_vSendSqueue.clear();
		m_vCardSqueue.clear();
		memset(m_iUserAlreadySend,0,sizeof(m_iUserAlreadySend));
		memset(m_iUserAlreadySendCount,0,sizeof(m_iUserAlreadySendCount));
		memset(m_i64UserCanBet,0,sizeof(m_i64UserCanBet));
		mi64UserMaxBet=0;											
}



//------------------------------------------------------------------------------
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	
	//��ť�������Ϣ
	if(pMessage->_uMessageType == UI_LBUTTONUP)
		{
			switch(pMessage->_uControlID)
			{
			case CTN_1014089_BTN_1004006://��ʼ��ť
				{
					//��;���벻�ɲ�����Ϸ
					if(m_IsJoin)
					{
						return 0;
					}
					OnButton(CLICK_START,0);
					break;
				}
			case CTN_1014089_CTN_2000171_BTN_2000178:		//�������水ť
			case CTN_1014089_CTN_2000171_BTN_2000179:
			case CTN_1014089_CTN_2000171_BTN_2000180:
			case CTN_1014089_CTN_2000171_BTN_2000181:
			case CTN_1014089_CTN_2000171_BTN_2000182:
			case CTN_1014089_CTN_2000171_BTN_2000183:
				{
					//��;���벻�ɲ�����Ϸ
					if(m_IsJoin)
					{
						return 0;
					}
					int iButton=pMessage->_uControlID-CTN_1014089_CTN_2000171_BTN_2000178;
					if(GS_BET==GetStationParameter())
					{
						
						OnButton(CLICK_OPERATE,iButton);
					}
					else if(GS_PLAY_GAME==GetStationParameter())
					{
						
						OnButton(CLICK_PLAYOPERATE,iButton);
					}
					break;
				}
			case CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000189:	//ѡ�����
			case CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000190:
			case CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000191:
				{
					//��;���벻�ɲ�����Ϸ
					if(m_IsJoin)
					{
						return 0;
					}
					if(GS_BET!=GetStationParameter())
					{
						return 0;
					}
					if(m_IsNote)
					{
						return 0;
					}
					int iButton=pMessage->_uControlID-CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000189;
					OnButton(CLICK_BET,iButton);
					break;
				}
			case CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000192:		//��ע����
			case CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000193:
			case CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000194:
				{
					//��;���벻�ɲ�����Ϸ
					if(m_IsJoin)
					{
						return 0;
					}
					if(GS_BET!=GetStationParameter())
					{
						return 0;
					}
					int iButton=pMessage->_uControlID-CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000192;
					OnButton(CLICK_BETOPERATE,iButton);
					break;
				}
			case CTN_1014089_CTN_1004100_BTN_2000244:
			case CTN_1014089_CTN_1004100_BTN_2000245:
				{

					int iButton=pMessage->_uControlID-CTN_1014089_CTN_1004100_BTN_2000244;
					if(0==iButton)	//����
					{
						ResetGameData();
						ResetGameDialog();
						KillAllGameTime();
						SetContainVisible(CTN_1014089_CTN_2000112,false);
						GameStationButton startMessage;
						startMessage.bDeskStation=m_iMyDeskStation;
						startMessage.iBtType=CLICK_START;

						m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
						
						PlayGameSound(SOUND_HIT_BEGIN,false);
					}
					else		//�뿪
					{
						KillAllGameTime();
						ResetGameData();
						ResetGameDialog();
						m_pGameFrame->CloseClient();
						PlayGameSound(SOUND_HIT_LEAVE,false);
					}
					break;
				}
			case CTN_1014089_CTN_2000246_BTN_2000249://�����û�ȷ��
				{
					//��;���벻�ɲ�����Ϸ
					if(m_IsJoin)
					{
						return 0;
					}
					BYTE byTempSuperResult=255;
					bool bSelected=FALSE;
					bSelected=GetRButtonSelected(CTN_1014089_CTN_2000246_RButton_2000247,&bSelected);
					if(bSelected && m_bIsSuperUser)
					{
						SuperUserControlReslut SuperResult;
						SuperResult.byDeskStation=m_iMyDeskStation;
						SuperResult.bySuperReslut=true;
						//����������ׯ��Ϣ
						m_pGameFrame->SendGameData(&SuperResult, sizeof(SuperResult) ,MDM_GM_GAME_NOTIFY,ASS_SUPER_RESULT,0);
					}
					ClearShowSuperResult();
					break;
				}
			case CTN_1014089_CTN_2000246_BTN_2000250://�����û�ȡ��
				{
					//��;���벻�ɲ�����Ϸ
					if(m_IsJoin)
					{
						return 0;
					}
					ClearShowSuperResult();
					break;
				}
			case CTN_1014089_BTN_1003500:					//��Ϸ����1003500
			case CTN_900000_CTN_1003510_BTN_1003540:
			case CTN_900000_CTN_1003510_BTN_1003550:
				{
					
					
					int iButton=pMessage->_uControlID-CTN_1014089_BTN_1003500;
					OnButton(CLICK_GAMESET,iButton);
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
		case ACTION_SEND_1:
		case ACTION_SEND_2:
		case ACTION_SEND_3:
		case ACTION_SEND_4:
			{
				int iAction=pMessage->_uControlID-CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000192;
				OnAction(iAction);
				break;
			}
		default:
			{
				break;
			}
		}
	}
	if (pMessage->_uMessageType == UI_KEYDOWN)
	{
		//��;���벻�ɲ�����Ϸ
		if(m_IsJoin)
		{
			return 0;
		}
		//����F12 ��������
		if (pMessage->_ch == VK_F12 && m_bIsSuperUser && GS_BET==m_byGameStation)
		{
			//��������û�����
			ClearShowSuperResult();
			//��ʾ���˽������
			ShowSuperResult(true);
			return 0;
		}
	}
	return 0;
	
}
//��ճ����û�����
void	CNewGameClient::ClearShowSuperResult()
{
	SetContainVisible(CTN_1014089_CTN_2000246,FALSE);		///<���˽���  ����
	SetRButtonSelected(CTN_1014089_CTN_2000246_RButton_2000247,FALSE);
	
}
//��ʾ���˽������
void	CNewGameClient::ShowSuperResult(bool bFlag)
{
	if (m_bIsSuperUser)
	{
		SetContainVisible(CTN_1014089_CTN_2000246,bFlag);		///<���˽���  ����
	}
}
/*
@brief:����������
*/
void CNewGameClient::OnAction(int iParam)
{
	
	m_iUserAlreadySendCount[m_vSendSqueue.at(m_iSendCardLabel)]++;
	m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)][m_iUserAlreadySendCount[m_vSendSqueue.at(m_iSendCardLabel)]-1]=m_vCardSqueue.at(m_iSendCardLabel);
	switch(m_vSendSqueue.at(m_iSendCardLabel))
	{
	case 0:
		{
			
			//���ɲ����Ʒ���
			NoOperateSendCardList(CTN_1014089_NoMoveCard_1004080,m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)]);

			int iCount=0;
			for(int i=0;i<13;i++)
			{
				if(m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)][i]>0)
				{
					iCount++;
				}
			}
			if(iCount<=0)
			{
				return;
			}
			break;
		}
	case 1:
		{
			
			NoOperateSendCardList(CTN_1014089_NoMoveCard_1004081,m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)]);

			int iCount=0;
			for(int i=0;i<13;i++)
			{
				if(m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)][i]>0)
				{
					iCount++;
				}
			}
			if(iCount<=0)
			{
				return;
			}
			break;
		}
	case 2:
		{
			
			//�ɲ����Ʒ���
			OperateSendCardList(CTN_1014089_OperateCard_2000230,m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)]);

			int iCount=0;
			for(int i=0;i<13;i++)
			{
				if(m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)][i]>0)
				{
					iCount++;
				}
			}
			if(iCount<=0)
			{
				return;
			}
			int iPoints=GetPoints(m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)],iCount);

			if(iPoints<21)
			{
				SetInumInfo(CTN_1014089_Num_2000236+m_vSendSqueue.at(m_iSendCardLabel),(__int64)iPoints,true);
			}
			else if(21==iPoints)
			{
				SetInumInfo(CTN_1014089_Num_2000236+m_vSendSqueue.at(m_iSendCardLabel),0,false);
				CString imageSrc;
				imageSrc.Format("./resources/image/maxfen.png");
				SetImageLoadSrc(CTN_1014089_IMG_2000240+m_vSendSqueue.at(m_iSendCardLabel),imageSrc,true);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,false);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,false);
			}
			else if(iPoints>21)
			{
				SetInumInfo(CTN_1014089_Num_2000236+m_vSendSqueue.at(m_iSendCardLabel),0,false);
				CString imageSrc;
				imageSrc.Format("./resources/image/busrt.png");
				SetImageLoadSrc(CTN_1014089_IMG_2000240+m_vSendSqueue.at(m_iSendCardLabel),imageSrc,true);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,false);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,false);
			}
			PlayGameSound(SOUND_SENDCARD,false);
			break;
		}
	case 3:
		{
			
			NoOperateSendCardList(CTN_1014089_NoMoveCard_1004083,m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)]);

			int iCount=0;
			for(int i=0;i<13;i++)
			{
				if(m_iUserAlreadySend[m_vSendSqueue.at(m_iSendCardLabel)][i]>0)
				{
					iCount++;
				}
			}
			if(iCount<=0)
			{
				return;
			}
			break;
		}
	default:
		{
			break;
		}
	}
	if(m_iSendCardLabel>=(m_vCardSqueue.size()-1))
	{
		SetImageVisible(CTN_1014089_IMG_1005003,false);
		return;
	}
	else
	{
		m_iSendCardLabel++;
		ShowAction(ACTION_SEND_1+m_vSendSqueue.at(m_iSendCardLabel),CTN_1014089_IMG_1005003,CTN_1014089_IMG_2000235,CTN_1014089_IMG_2000231+m_vSendSqueue.at(m_iSendCardLabel),250);
		
	}
}
/*
@brief:���������
*/
void CNewGameClient::OnButton(const int iBtType,int iParam)
{
	switch(iBtType)
	{
	case CLICK_START:
		{
			KillAllGameTime();
			SetContainVisible(CTN_1014089_CTN_2000112,false);
			GameStationButton startMessage;
			startMessage.bDeskStation=m_iMyDeskStation;
			startMessage.iBtType=CLICK_START;
			
			m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
			SetBtVisible(CTN_1014089_BTN_1004006,false);
			PlayGameSound(SOUND_HIT_BEGIN,false);
			break;
		}
	case CLICK_OPERATE:
		{
			switch(iParam)
			{
			case 0:					//��ע
				{
					//��ʾ��������
					SetContainVisible(CTN_1014089_CTN_2000171_CTN_2000188,true);
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000189,true);
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000190,true);
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000191,true);
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000192,true);
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000193,true);
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000194,true);
					//��ʾ����
					CString imageSrc;
					imageSrc.Format("./resources/image/jeton.png");
					int x=GetBetIndex(m_i64UserCanBet[2])*31;
					SetPicSize(CTN_1014089_CTN_2000171_CTN_2000188_IMG_2000195,imageSrc,true,x,0,31,30);
					x=GetBetIndex(m_i64UserCanBet[1])*31;
					SetPicSize(CTN_1014089_CTN_2000171_CTN_2000188_IMG_2000196,imageSrc,true,x,0,31,30);
					x=GetBetIndex(m_i64UserCanBet[0])*31;
					SetPicSize(CTN_1014089_CTN_2000171_CTN_2000188_IMG_2000197,imageSrc,true,x,0,31,30);
					PlayGameSound(SOUND_XIAZHU,false);
					break;
				}
			case 2:				//����
				{
					GameStationButton operateMessage;
					operateMessage.bDeskStation=m_iMyDeskStation;
					operateMessage.iBtType=CLICK_OPERATE;
					operateMessage.iBtParam=iParam;
					m_pGameFrame->SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
					SetContainVisible(CTN_1014089_CTN_2000171_CTN_2000188,false);
					for(int i=0;i<6;i++)
					{
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false);
						SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000189+i,false);
					}
					
					PlayGameSound(SOUND_HIT_BEGIN,false);
					break;
				}
			case 1:				//�ӱ�
			case 3:				//Ҫ��
			case 4:				//����
			case 5:				//ͣ��
				{
					break;
				}
			default:
				{
					break;
				}
			}
			break;
		}
	case CLICK_BET:		//������밴ť
		{
			switch(iParam)
			{
			case 0:
				{
					m_i64CurrentNote=m_i64UserCanBet[2];
					SetInumShow(CTN_1014089_CTN_2000171_CTN_2000188_Num_2000198,m_i64CurrentNote,true);
					break;
				}
			case 1:
				{
					m_i64CurrentNote=m_i64UserCanBet[1];
					SetInumShow(CTN_1014089_CTN_2000171_CTN_2000188_Num_2000198,m_i64CurrentNote,true);
					break;
				}
			case 2:
				{
					m_i64CurrentNote=m_i64UserCanBet[0];
					SetInumShow(CTN_1014089_CTN_2000171_CTN_2000188_Num_2000198,m_i64CurrentNote,true);
					break;
				}
			default:
				{
					break;
				}
			}
			break;
		}
	case CLICK_BETOPERATE:
		{
			switch(iParam)
			{
			case 0:							//ȷ��
				{
					if(m_i64CurrentNote<=0)
					{
						return;
					}
					GameStationButton betMessage;
					betMessage.bDeskStation=m_iMyDeskStation;
					betMessage.iBtType=CLICK_BETOPERATE;
					betMessage.i64Note=m_i64CurrentNote;
					m_pGameFrame->SendGameData(&betMessage, sizeof(betMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
					m_IsNote=true;
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000192,false);
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000193,false);
					SetBtEnable(CTN_1014089_CTN_2000171_CTN_2000188_BTN_2000194,false);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,false);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);

					SetContainVisible(CTN_1014089_CTN_2000112,false);
					m_pGameFrame->KillTimer(ID_NOTE) ;
					PlayGameSound(SOUND_HIT_BEGIN,false);
					break;
				}
			case 1:							//���
				{
					m_i64CurrentNote=mi64UserMaxBet;
					SetInumShow(CTN_1014089_CTN_2000171_CTN_2000188_Num_2000198,m_i64CurrentNote,true);
					PlayGameSound(SOUND_HIT_BEGIN,false);
					break;
				}
			case 2:							//ȡ��
				{
					m_i64CurrentNote=0;
					SetInumShow(CTN_1014089_CTN_2000171_CTN_2000188_Num_2000198,m_i64CurrentNote,true);
					PlayGameSound(SOUND_HIT_BEGIN,false);
					break;
				}
			default:
				{
					break;
				}
			}
			break;
		}
	case CLICK_PLAYOPERATE:			//��Ϸ�в���
		{
			switch(iParam)
			{
			case 0:					//��ע
				{
					return;
					break;
				}
			case 1:				//�ӱ�
				{
					if(m_bScorePeople==m_iMyDeskStation)
					{
						UserInfoForGame userInfo;
						if(m_pGameFrame->GetUserInfo(m_iMyDeskStation,userInfo))
						{
							if(userInfo.i64Money<(m_i64UserBet[m_iMyDeskStation]*2))
							{
								return;
							}	
						}
						GameStationButton operateMessage;
						operateMessage.bDeskStation=m_iMyDeskStation;
						operateMessage.iBtType=CLICK_PLAYOPERATE;
						operateMessage.iBtParam=iParam;
						operateMessage.i64Note=m_i64UserBet[m_iMyDeskStation];
						m_pGameFrame->SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);

						for(int i=0;i<6;i++)
						{
							SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false); 
						}
						PlayGameSound(SOUND_ADDDOUBLE,false);
					}
					
					break;
				}
			case 2:				//����
				{
					if(m_bScorePeople==m_iMyDeskStation)
					{
						GameStationButton operateMessage;
						operateMessage.bDeskStation=m_iMyDeskStation;
						operateMessage.iBtType=CLICK_PLAYOPERATE;
						operateMessage.iBtParam=iParam;
						m_pGameFrame->SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);

						for(int i=0;i<6;i++)
						{
							SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false); 
						}
					}
					PlayGameSound(SOUND_HIT_BEGIN,false);
					break;
				}
			case 3:				//Ҫ��
				{
					if(m_bScorePeople==m_iMyDeskStation)
					{
						GameStationButton operateMessage;
						operateMessage.bDeskStation=m_iMyDeskStation;
						operateMessage.iBtType=CLICK_PLAYOPERATE;
						operateMessage.iBtParam=iParam;
						m_pGameFrame->SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
						PlayGameSound(SOUND_HIT_BEGIN,false);
					}
					break;
				}
			case 4:				//����
				{
					return;
					GameStationButton operateMessage;
					operateMessage.bDeskStation=m_iMyDeskStation;
					operateMessage.iBtType=CLICK_PLAYOPERATE;
					operateMessage.iBtParam=iParam;
					m_pGameFrame->SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
					PlayGameSound(SOUND_HIT_BEGIN,false);
					break;
				}
			case 5:				//ͣ��
				{
					if(m_bScorePeople==m_iMyDeskStation)
					{
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,false);
						GameStationButton btOperate;
						btOperate.bDeskStation=m_iMyDeskStation;
						btOperate.iBtType=CLICK_PLAYOPERATE;
						btOperate.iBtParam=iParam;	//��ʾ��Ҫ��
						m_pGameFrame->SendGameData(&btOperate, sizeof(btOperate) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
						for(int i=0;i<6;i++)
						{
							SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false); 
						}
						
					}
					PlayGameSound(SOUND_HIT_BEGIN,false);
					break;
				}
			default:
				{
					break;
				}
			}
			break;
		}
		case CLICK_GAMESET:				//��Ϸ����
			{
				switch(iParam)
				{
				case 0:
					{
						SetContainVisible(CTN_900000_CTN_1003510,true);
						SetRButtonSelected(CTN_900000_CTN_1003510_RButton_1003520,m_iGameSet[0]);
						SetRButtonSelected(CTN_900000_CTN_1003510_RButton_1003530,m_iGameSet[1]);
						SetRButtonSelected(CTN_900000_CTN_1003510_RButton_2000254,m_iGameSet[2]);
						break;
					}
				case 40:					//ȷ��
					{
						GetRButtonSelected(CTN_900000_CTN_1003510_RButton_1003520,&m_iGameSet[0]);
						GetRButtonSelected(CTN_900000_CTN_1003510_RButton_1003530,&m_iGameSet[1]);
						GetRButtonSelected(CTN_900000_CTN_1003510_RButton_2000254,&m_iGameSet[2]);
						if(!m_iGameSet[0])
						{
							m_SoundEngineGame->stopAllSounds();
						}
						SetContainVisible(CTN_900000_CTN_1003510,false);
						break;
					}
				case 50:					//ȡ��
					{
						SetContainVisible(CTN_900000_CTN_1003510,false);
						break;
					}
				default:
					{
						break;
					}
				}
			break;
			}
	default:
		{
			break;
		}
	}
}

//---------------------------------------------------------------------------------------
///ɾ�����ж�ʱ��,����ע����ʤ������ʾ��ʱ����
void CNewGameClient:: KillAllGameTime() 
{
	m_pGameFrame->KillTimer(ID_BEGIN_TIME) ; 

	m_pGameFrame->KillTimer(ID_WAIT_NEXT) ; 

	m_pGameFrame->KillTimer(ID_NOTE) ; 

	m_pGameFrame->KillTimer(ID_OUT_CARD );
}
//---------------------------------------------------------------------------------------
//���ű�����Ч
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	//if(!m_SoundEnginBg)
	//{
	//	return ;
	//}
	///*if(!m_bSoundBgPlay)
	//{
	//return ;
	//}*/

	//char szFileName[MAX_PATH];
	//memset(szFileName,0,sizeof(szFileName)) ; 
	//wsprintf(szFileName,"music\\sound\\BeiJing.ogg");
	//m_SoundEnginBg->setSoundVolume(0.5);
	//m_SoundEnginBg->play2D(szFileName,bISLoop);
}



//-----------------------------------------------------------------------------------------------
/// ������Ϸ�ĵ�ǰ״̬
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	//m_bWatchMode = m_pGameFrame->GetIsWatching() ;
	
	CString aa;
	aa.Format("lw:: ��ǰ��Ϸ״̬=%d", GetStationParameter());
	OutputDebugString(aa);

	CString imageSrc;
	//PlayGameSound(SOUND_BEGIN_GAME,false);
	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			if(nLen!=sizeof(GameStation_1))
			{
				return;
			}

			GameStation_1 *gameStation_1=(GameStation_1 *)pBuffer;
			if(NULL==gameStation_1)
			{
				return;
			}

			m_bGameBeginWaitTime=gameStation_1->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_1->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_1->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_1->bGameStation;											//��Ϸ״̬
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(gameStation_1->bUserReady[i])
				{
					int iView=m_pGameFrame->DeskStation2View(i);
					imageSrc.Format("./resources/image/agree.png");
					IImage *pImage = NULL;
					pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1014089_IMG_1004070+iView));
					if (NULL != pImage)
					{
						pImage->SetGameW(79);
						pImage->SetGameH(44);
						pImage->LoadPic(CA2W(imageSrc));
						pImage->SetControlVisible(true);
					}
				}
			}
			//��ʾ��ʼ��ť
			imageSrc.Format("./resources/image/start_bt.png");
			SetBtLoadSrc(CTN_1014089_BTN_1004006,imageSrc,true);

			SetContainVisible(CTN_1014089_CTN_2000112,true);
			m_iClockLabel=m_bGameBeginWaitTime;											//��ǰʱ�ӱ�ǩ
			m_iCurrentID=CTN_1014089_CTN_2000112_Num_2000228;							//��ǰʱ��ID
			//�����ȴ�ʱ��
			m_pGameFrame->SetTimer(ID_BEGIN_TIME,1000);
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			break; 
		}
	case GS_BET:	//��ע״̬
		{
			if(nLen!=sizeof(GameStation_2))
			{
				return;
			}

			GameStation_2 *gameStation_2=(GameStation_2 *)pBuffer;
			if(NULL==gameStation_2)
			{
				return;
			}

			m_bGameBeginWaitTime=gameStation_2->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_2->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_2->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_2->bGameStation;											//��Ϸ״̬
			m_bNtPeople=gameStation_2->iNtPeople;
			m_IsJoin=true;
			int iView=m_pGameFrame->DeskStation2View(m_bNtPeople);
			CString imageSrc;
			imageSrc.Format("./resources/image/nt.png");
			SetImageLoadSrc(CTN_1014089_IMG_1004060+iView,imageSrc,true);
			break;
		}
	case GS_SEND:		//����״̬
		{
			if(nLen!=sizeof(GameStation_5))
			{
				return;
			}

			GameStation_5 *gameStation_5=(GameStation_5 *)pBuffer;
			if(NULL==gameStation_5)
			{
				return;
			}

			m_bGameBeginWaitTime=gameStation_5->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_5->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_5->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_5->bGameStation;											//��Ϸ״̬
			m_bNtPeople=gameStation_5->iNtPeople;
			m_IsJoin=true;
			int iView=m_pGameFrame->DeskStation2View(m_bNtPeople);
			CString imageSrc;
			imageSrc.Format("./resources/image/nt.png");
			SetImageLoadSrc(CTN_1014089_IMG_1004060+iView,imageSrc,true);
			break;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			if(nLen!=sizeof(GameStation_3))
			{
				return;
			}

			GameStation_3 *gameStation_3=(GameStation_3 *)pBuffer;
			if(NULL==gameStation_3)
			{
				return;
			}
			m_bGameBeginWaitTime=gameStation_3->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_3->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_3->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_3->bGameStation;											//��Ϸ״̬
			m_bNtPeople=gameStation_3->iNtPeople;
			m_IsJoin=true;
			int iView=m_pGameFrame->DeskStation2View(m_bNtPeople);
			CString imageSrc;
			imageSrc.Format("./resources/image/nt.png");
			SetImageLoadSrc(CTN_1014089_IMG_1004060+iView,imageSrc,true);
			break;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			if(nLen!=sizeof(GameStation_4))
			{
				return;
			}

			GameStation_4 *gameStation_4=(GameStation_4 *)pBuffer;
			if(NULL==gameStation_4)
			{
				return;
			}
			m_bGameBeginWaitTime=gameStation_4->bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
			m_bGameBetTime=gameStation_4->bGameBetTime;												//��Ϸ��עʱ��
			m_bCallCardTime=gameStation_4->bCallCardTime;											//��Ϸ����ʱ��
			m_byGameStation=gameStation_4->bGameStation;											//��Ϸ״̬
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(gameStation_4->bUserReady[i])
				{
					int iView=m_pGameFrame->DeskStation2View(i);
					
					imageSrc.Format("./resources/image/agree.png");
					IImage *pImage = NULL;
					pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1014089_IMG_1004070+iView));
					if (NULL != pImage)
					{
						pImage->SetGameW(79);
						pImage->SetGameH(44);
						pImage->LoadPic(CA2W(imageSrc));
						pImage->SetControlVisible(true);
					}
				}
			}

			//��ʾ��ʼ��ť
			imageSrc.Format("./resources/image/start_bt.png");
			SetBtLoadSrc(CTN_1014089_BTN_1004006,imageSrc,true);

			SetContainVisible(CTN_1014089_CTN_2000112,true);
			m_iClockLabel=m_bGameBeginWaitTime;											//��ǰʱ�ӱ�ǩ
			m_iCurrentID=CTN_1014089_CTN_2000112_Num_2000228;							//��ǰʱ��ID
			//�����ȴ�ʱ��
			m_pGameFrame->SetTimer(ID_BEGIN_TIME,1000);
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------
//���õ�ѡ��ؼ��Ƿ�ѡ��
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		pRadioButton->SetIsSelect(bSelected);
		//pRadioButton->SetControlVisible(bFlag);

	}
}
//--------------------------------------------------------------------------------------------------------
//��ȡ��ѡ��ؼ��Ƿ�ѡ��
BOOL	CNewGameClient::GetRButtonSelected(const int iRButtonID,bool *bSelected)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		//pRadioButton->SetIsSelect(bSelected);
		//pRadioButton->SetControlVisible(bFlag);
		*bSelected=pRadioButton->GetIsSelect();
		return (*bSelected);
	}
	return TRUE;
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
	/*CString aa;
	aa.Format("lw::test: client Msg %d", nSubId);
	OutputDebugString(aa);*/

	switch(nSubId)
	{
	case ASS_GM_GAME_STATION:   ///<����״̬(ƽ̨���õ�)
		{
			SetGameStation(buffer, nLen);    
			break;        
		}
	case ASS_AGREE:				//ͬ����Ϸ��Ϣ
		{
			if(nLen!=sizeof(AgreeGame))
			{
				return 0;
			}
			OnHandleAgreeGame(buffer);
			break;
		}
	case ASS_NT:				//��ׯ��
		{
			if(nLen!=sizeof(GameMessageNt))
			{
				return 0;
			}
			OnHandleNt(buffer);
			break;
		}

	case ASS_BET:				//������ע��Ϣ
		{
			if(nLen!=sizeof(GameMessageBet))
			{
				return 0;
			}
			OnHandleBet(buffer);
			
			break;
		}
	case ASS_SEND:					//��ʼ����
		{
			if(nLen!=sizeof(GameMessageSendCard))
			{
				return 0;
			}
			OnHandleSendCard(buffer);
			break;
		}
	case ASS_PLAY:						//��ʼ��Ϸ
		{
			if(nLen!=sizeof(GameMessageStart))
			{
				return 0;
			}
			
			OnHandlePlay(buffer);
			break;
		}
	case ASS_SAFEEND:					//��Ϸ��ȫ����
		{
			if(nLen!=sizeof(GameMessageEnd))
			{
				return 0;
			}
			OnHandleSafeFinish(buffer);
			break;
		}
	case ASS_BETRESULT:				//��ע�����Ϣ
		{
			
			if(nLen!=sizeof(GameMessageBetResult))
			{
				
				return 0;
			}
			OnHandleBetResult(buffer);
			break;
		}
	case ASS_CALL:				//�з���Ϣ
		{
			if(nLen!=sizeof(GameMessageNeedCard))
			{

				return 0;
			}
			//����������������Ľ�����Ϣ
			OnHandleCall(buffer);
			break;
		}
	case ASS_RESTART:
		{
			ResetGameDialog();
			ResetGameData();
			CString imageSrc;
			imageSrc.Format("./resources/image/start_bt.png");
			SetBtLoadSrc(CTN_1014089_BTN_1004006,imageSrc,true);
			//������ʼ�ȴ���ʱ��
			SetContainVisible(CTN_1014089_CTN_2000112,true);
			m_iClockLabel=m_bGameBeginWaitTime;											//��ǰʱ�ӱ�ǩ
			m_iCurrentID=CTN_1014089_CTN_2000112_Num_2000228;							//��ǰʱ��ID
			//�����ȴ�ʱ��
			m_pGameFrame->SetTimer(ID_WAIT_NEXT,1000);
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			break;
		}
	case ASS_NORMALFINISH:			//��Ϸ��������
		{
			if(nLen!=sizeof(GameMessageNormalFinish))
			{

				return 0;
			}
			OnHandleNormalFinish(buffer);
			break;
		}
	case ASS_SUPER_USER:	//���˿ͻ���
		{  
			if(nLen!=sizeof(SUPERSTATE))
			{
				return 0;
			}	
			HandleSuperMessage(buffer);			
			break;
		}
	default:
		{
			break;
		}

	}
	return 0;
}
/*
@brief:���������ͬ����Ϸ��Ϣ
*/
void CNewGameClient::OnHandleAgreeGame(void *buffer)
{
	AgreeGame *agreeGame=(AgreeGame *)buffer;
	if(NULL==agreeGame)
	{
		return;
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(agreeGame->bUserReady[i])
		{
			int iView=m_pGameFrame->DeskStation2View(i);
			
			CString imageSrc;
			imageSrc.Format("./resources/image/agree.png");
			IImage *pImage = NULL;
			pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1014089_IMG_1004070+iView));
			if (NULL != pImage)
			{
				pImage->SetGameW(79);
				pImage->SetGameH(44);
				pImage->LoadPic(CA2W(imageSrc));
				pImage->SetControlVisible(true);
			}
		}
	}
}
/*
@brief:�����������������ȷ��ׯ����Ϣ
*/
void CNewGameClient::OnHandleNt(void *buffer)
{
	GameMessageNt *haveNt=(GameMessageNt *)buffer;
	if(NULL==haveNt)
	{
		return;
	}
	int iView=m_pGameFrame->DeskStation2View(haveNt->bNtStation);
	m_bNtPeople=haveNt->bNtStation;
	CString imageSrc;
	imageSrc.Format("./resources/image/nt.png");
	SetImageLoadSrc(CTN_1014089_IMG_1004060+iView,imageSrc,true);
}
/*
@brief:�������������������ע��Ϣ
*/				
void CNewGameClient::OnHandleBet(void *buffer)
{
	GameMessageBet *haveBet=(GameMessageBet *)buffer;
	if(NULL==haveBet)
	{
		return;
	}
	SetStationParameter(GS_BET);
	if(GS_BET!=GetStationParameter())
	{
		return;
	}

	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetImageVisible(CTN_1014089_IMG_1004070+i,false);
	}
	//��ʾ��ע����
	SetContainVisible(CTN_1014089_CTN_2000221,true);
	SetTextinfo(CTN_1014089_CTN_2000221_TEXT_2000224,(__int64)haveBet->i64BetLimit[m_iMyDeskStation],true);
	SetTextinfo(CTN_1014089_CTN_2000221_TEXT_2000225,(__int64)haveBet->i64Bet[m_iMyDeskStation][2],true);
	if(m_IsJoin)
	{
		return;
	}
	mi64UserMaxBet=haveBet->i64BetLimit[m_iMyDeskStation];
	for(int i=0;i<3;i++)
	{
		m_i64UserCanBet[i]=haveBet->i64Bet[m_iMyDeskStation][i];
	}
	if(m_bNtPeople==m_iMyDeskStation)
	{
		//��ʾ��ע����
		SetContainVisible(CTN_1014089_CTN_2000171,false);
	}
	else
	{
		//��ʾ��ע����
		SetContainVisible(CTN_1014089_CTN_2000171,true);
		SetContainVisible(CTN_1014089_CTN_2000171_CTN_2000188,false);
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,true);
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,true);
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,false);
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,false);
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,false);
	}

	//������ע��ʱ��
	SetContainVisible(CTN_1014089_CTN_2000112,true);
	m_iClockLabel=m_bGameBetTime;											//��ǰʱ�ӱ�ǩ
	m_iCurrentID=CTN_1014089_CTN_2000112_Num_2000228;							//��ǰʱ��ID
	//�����ȴ�ʱ��
	m_pGameFrame->SetTimer(ID_NOTE,1000);
	SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true); 
}
/*
@brief:����������������ķ�����Ϣ
*/
void CNewGameClient::OnHandleSendCard(void *buffer)
{
	
	GameMessageSendCard *sendCard=(GameMessageSendCard *)buffer;
	if(NULL==sendCard)
	{
		return;
	}
	KillAllGameTime();
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetContainVisible(CTN_1014089_CTN_2000110+i,false);
	}
	SetStationParameter(GS_SEND);
	SetContainVisible(CTN_1014089_CTN_2000171,true);
	for(int i=0;i<6;i++)
	{
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false);
	}
	//���س�������
	SetContainVisible(CTN_1014089_CTN_2000171_CTN_2000188,false);

	int iView=0;
	UserInfoForGame userInfo;
	memset(&userInfo,0,sizeof(userInfo));
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pGameFrame->GetUserInfo(i,userInfo))
		{
			iView=m_pGameFrame->DeskStation2View(i);
			memcpy(m_iUserCard[iView],sendCard->bUserCard[i],sizeof(BYTE)*2);
		}
	}

	iView=m_pGameFrame->DeskStation2View(sendCard->bFirstPeople);
	int iNtView=m_pGameFrame->DeskStation2View(m_bNtPeople);

	for(int k=0;k<2;k++)
	{
		for(int i=0,t=iView;i<PLAY_COUNT;i++,t++)
		{
			if(t>=PLAY_COUNT)
			{
				t=0;
			}
			if(iNtView==t)
			{
				continue;
			}
			if(m_iUserCard[t][k]>0)
			{
				m_vCardSqueue.push_back(m_iUserCard[t][k]);
				m_vSendSqueue.push_back(t);
			}
		}
		m_vCardSqueue.push_back(m_iUserCard[iNtView][k]);
		m_vSendSqueue.push_back(iNtView);
	}
	
	//��ʼ����
	m_iSendCardLabel=0;
	SetImageVisible(CTN_1014089_IMG_1005003,true);
	ShowAction(ACTION_SEND_1+m_vSendSqueue.at(m_iSendCardLabel),CTN_1014089_IMG_1005003,CTN_1014089_IMG_2000235,CTN_1014089_IMG_2000231+m_vSendSqueue.at(m_iSendCardLabel),250);
}

/*
@brief:�������������������Ϸ��ȫ������Ϣ
*/
void CNewGameClient::OnHandleSafeFinish(void *buffer)
{
	GameMessageEnd *safeFinishGame=(GameMessageEnd *)buffer;
	if(NULL==safeFinishGame)
	{
		return;
	}
	KillAllGameTime();
	ResetGameData();
	ResetGameDialog();
	SetStationParameter(GS_WAIT_NEXT);
	//��ʾ��ʼ��ť
	SetBtVisible(CTN_1014089_BTN_1004006,true);
	//������ʼ�ȴ���ʱ��
	SetContainVisible(CTN_1014089_CTN_2000112,true);
	m_iClockLabel=m_bGameBeginWaitTime;											//��ǰʱ�ӱ�ǩ
	m_iCurrentID=CTN_1014089_CTN_2000112_Num_2000228;							//��ǰʱ��ID
	//�����ȴ�ʱ��
	m_pGameFrame->SetTimer(ID_WAIT_NEXT,1000);
	SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
}
/*
@brief:�������������������ע��Ϣ
*/
void CNewGameClient::OnHandleBetResult(void *buffer)
{
	GameMessageBetResult *betResult=(GameMessageBetResult *)buffer;
	if(NULL==betResult)
	{
		
		return;
	}
	
	m_i64UserBet[betResult->bDeskStation]+=betResult->i64Note;
	int iView=m_pGameFrame->DeskStation2View(betResult->bDeskStation);
	SetNote(betResult->i64Note,iView);
}
/*
@brief:����������������Ŀ�ʼ��Ϸ��Ϣ
*/
void CNewGameClient::OnHandlePlay(void *buffer)
{
	GameMessageStart *startGame=(GameMessageStart *)buffer;
	if(NULL==startGame)
	{
		return;
	}

	SetStationParameter(GS_PLAY_GAME);
	
	//code:����У��
	if(startGame->bFirstScoreStation>=0 && startGame->bFirstScoreStation<PLAY_COUNT)
	{
		m_bScorePeople=startGame->bFirstScoreStation;
		
		if(m_iMyDeskStation==startGame->bFirstScoreStation && m_bNtPeople==m_iMyDeskStation)
		{
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,true);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,true);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,true);
		}
		else if(m_iMyDeskStation==startGame->bFirstScoreStation && m_bNtPeople!=m_iMyDeskStation)
		{
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,true);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,true);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,true);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,true);
		}
		else
		{
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
			SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,false);
		}
		UserInfoForGame userInfo;
		if(m_pGameFrame->GetUserInfo(m_iMyDeskStation,userInfo))
		{
			if(userInfo.i64Money<(m_i64UserBet[m_iMyDeskStation]*2))
			{
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,false);
			}	
		}

		//������м�ʱ��
		for(int i=0;i<PLAY_COUNT;i++)
		{
			SetContainVisible(CTN_1014089_CTN_2000110+i,false);
		}
		KillAllGameTime();
		//��ʼ�з�
		int View=m_pGameFrame->DeskStation2View(startGame->bFirstScoreStation);
		SetContainVisible(CTN_1014089_CTN_2000110+View,true);

		m_iClockLabel=m_bCallCardTime;											//��ǰʱ�ӱ�ǩ
		m_iCurrentID=CTN_1014089_CTN_2000110_Num_2000226+View;					//��ǰʱ��ID
		//�����ȴ�ʱ��
		m_pGameFrame->SetTimer(ID_OUT_CARD,1000);
		SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
	}
}
/*
@brief:����������������Ľ�����Ϣ
*/
void CNewGameClient::OnHandleCall(void *buffer)
{
	GameMessageNeedCard *callCard=(GameMessageNeedCard *)buffer;
	if(NULL==callCard)
	{
		return;
	}
	
	//code:�����ж�
	if(SERVER_EXTRA_DOUBLE==callCard->iStyle)			//�ӱ�
	{
		int iMyView=m_pGameFrame->DeskStation2View(callCard->bDeskStation);
		//code:����У��
		if(callCard->bCard>=0)
		{
			//code:������м�ʱ��
			for(int i=0;i<PLAY_COUNT;i++)
			{
				SetContainVisible(CTN_1014089_CTN_2000110+i,false);
			}
			//ɱ�����м�ʱ��
			KillAllGameTime();

			if(m_iMyDeskStation==callCard->bDeskStation)
			{
				for(int i=0;i<6;i++)
				{
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false);
				}

			}
			m_bScorePeople=callCard->bNextStation;

			for(int i=0;i<13;i++)
			{
				if(m_iUserAlreadySend[iMyView][i]<=0)
				{
					m_iUserAlreadySend[iMyView][i]=callCard->bCard;
					break;
				}
			}

			switch(iMyView)
			{
			case 0:
				{

					//���ɲ����Ʒ���
					NoOperateSendCardList(CTN_1014089_NoMoveCard_1004080,m_iUserAlreadySend[iMyView]);

					break;
				}
			case 1:
				{

					NoOperateSendCardList(CTN_1014089_NoMoveCard_1004081,m_iUserAlreadySend[iMyView]);

					break;
				}
			case 2:
				{

					//�ɲ����Ʒ���
					OperateSendCardList(CTN_1014089_OperateCard_2000230,m_iUserAlreadySend[iMyView]);

					int iCount=0;
					for(int i=0;i<13;i++)
					{
						if(m_iUserAlreadySend[iMyView][i]>0)
						{
							iCount++;
						}
					}
					if(iCount<=0)
					{
						return;
					}
					int iPoints=GetPoints(m_iUserAlreadySend[iMyView],iCount);

					if(iPoints<21)
					{
						SetInumInfo(CTN_1014089_Num_2000236+iMyView,(__int64)iPoints,true);
					}
					else if(21==iPoints)
					{
						SetInumInfo(CTN_1014089_Num_2000236+iMyView,0,false);
						CString imageSrc;
						imageSrc.Format("./resources/image/maxfen.png");
						SetImageLoadSrc(CTN_1014089_IMG_2000240+iMyView,imageSrc,true);
					}
					else if(iPoints>21)
					{
						SetInumInfo(CTN_1014089_Num_2000236+iMyView,0,false);
						CString imageSrc;
						imageSrc.Format("./resources/image/busrt.png");
						SetImageLoadSrc(CTN_1014089_IMG_2000240+iMyView,imageSrc,true);
					}
					PlayGameSound(SOUND_SENDCARD,false);
					break;
				}
			case 3:
				{
					NoOperateSendCardList(CTN_1014089_NoMoveCard_1004083,m_iUserAlreadySend[iMyView]);
					break;
				}
			default:
				{
					break;
				}
			}
		}
		if(callCard->i64Note>0)
		{
			__int64 i64Temp=m_i64UserBet[callCard->bDeskStation]+callCard->i64Note;
			SetNote(i64Temp,iMyView);
		}
		if(!callCard->bFlag)
		{
			GameEnd();
		}
		else
		{
			if(255!=callCard->bNextStation)
			{
				if(m_iMyDeskStation==callCard->bNextStation)
				{
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,true);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,true);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,true);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,true);
				}
				//code:������һ�������߶�ʱ��
				int iNextView=m_pGameFrame->DeskStation2View(callCard->bNextStation);
				SetContainVisible(CTN_1014089_CTN_2000110+iNextView,true);
				m_iClockLabel=m_bCallCardTime;											//��ǰʱ�ӱ�ǩ
				m_iCurrentID=CTN_1014089_CTN_2000110_Num_2000226+iNextView;				//��ǰʱ��ID
				//�����ȴ�ʱ��
				m_pGameFrame->SetTimer(ID_OUT_CARD,1000);
				SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			}
		}
	}
	else if(SERVER_EXTRA_CLICK_GIVEUP==callCard->iStyle)			//����
	{
		if(callCard->bNextStation>=0 && callCard->bNextStation<=PLAY_COUNT)
		{
			//code:������м�ʱ��
			for(int i=0;i<PLAY_COUNT;i++)
			{
				SetContainVisible(CTN_1014089_CTN_2000110+i,false);
			}
			//ɱ�����м�ʱ��
			KillAllGameTime();

			if(!callCard->bFlag)
			{
				GameEnd();
				return;
			}

			if(255!=callCard->bNextStation && callCard->bFlag)
			{
				if(m_iMyDeskStation==callCard->bDeskStation)
				{
					for(int i=0;i<6;i++)
					{
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false);
					}

				}
				if(m_iMyDeskStation==callCard->bNextStation)
				{
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,true);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,true);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,true);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,true);
				}
				m_bScorePeople=callCard->bNextStation;

				//��ʼ�з�
				int iNextView=m_pGameFrame->DeskStation2View(callCard->bNextStation);
				SetContainVisible(CTN_1014089_CTN_2000110+iNextView,true);

				m_iClockLabel=m_bCallCardTime;											//��ǰʱ�ӱ�ǩ
				m_iCurrentID=CTN_1014089_CTN_2000110_Num_2000226+iNextView;					//��ǰʱ��ID
				//�����ȴ�ʱ��
				m_pGameFrame->SetTimer(ID_OUT_CARD,1000);
				SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			}
		}
	}
	else if(SERVER_EXTRA_CLICK_NEED==callCard->iStyle)				//Ҫ��
	{
		int iMyView=m_pGameFrame->DeskStation2View(callCard->bDeskStation);
		if(callCard->bCard>=0)
		{
			//code:������м�ʱ��
			for(int i=0;i<PLAY_COUNT;i++)
			{
				SetContainVisible(CTN_1014089_CTN_2000110+i,false);
			}
			//ɱ�����м�ʱ��
			KillAllGameTime();
			
			for(int i=0;i<13;i++)
			{
				if(m_iUserAlreadySend[iMyView][i]<=0)
				{
					m_iUserAlreadySend[iMyView][i]=callCard->bCard;
					break;
				}
			}
			switch(iMyView)
			{
			case 0:
				{

					//���ɲ����Ʒ���
					NoOperateSendCardList(CTN_1014089_NoMoveCard_1004080,m_iUserAlreadySend[iMyView]);

					int iCount=0;
					for(int i=0;i<13;i++)
					{
						if(m_iUserAlreadySend[iMyView][i]>0)
						{
							iCount++;
						}
					}
					if(iCount<=0)
					{
						return;
					}
					int iPoints=GetPoints(m_iUserAlreadySend[iMyView],iCount);

					
					break;
				}
			case 1:
				{

					NoOperateSendCardList(CTN_1014089_NoMoveCard_1004081,m_iUserAlreadySend[iMyView]);

					int iCount=0;
					for(int i=0;i<13;i++)
					{
						if(m_iUserAlreadySend[iMyView][i]>0)
						{
							iCount++;
						}
					}
					if(iCount<=0)
					{
						return;
					}
					
					break;
				}
			case 2:
				{

					//�ɲ����Ʒ���
					OperateSendCardList(CTN_1014089_OperateCard_2000230,m_iUserAlreadySend[iMyView]);

					int iCount=0;
					for(int i=0;i<13;i++)
					{
						if(m_iUserAlreadySend[iMyView][i]>0)
						{
							iCount++;
						}
					}
					if(iCount<=0)
					{
						return;
					}
					int iPoints=GetPoints(m_iUserAlreadySend[iMyView],iCount);



					if(iPoints<21)
					{
						SetInumInfo(CTN_1014089_Num_2000236+iMyView,(__int64)iPoints,true);
					}
					else if(21==iPoints)
					{
						SetInumInfo(CTN_1014089_Num_2000236+iMyView,0,false);
						CString imageSrc;
						imageSrc.Format("./resources/image/maxfen.png");
						SetImageLoadSrc(CTN_1014089_IMG_2000240+iMyView,imageSrc,true);
					}
					else if(iPoints>21)
					{
						SetInumInfo(CTN_1014089_Num_2000236+iMyView,0,false);
						CString imageSrc;
						imageSrc.Format("./resources/image/busrt.png");
						SetImageLoadSrc(CTN_1014089_IMG_2000240+iMyView,imageSrc,true);
					}
					PlayGameSound(SOUND_SENDCARD,false);
					break;
				}
			case 3:
				{

					NoOperateSendCardList(CTN_1014089_NoMoveCard_1004083,m_iUserAlreadySend[iMyView]);

					int iCount=0;
					for(int i=0;i<13;i++)
					{
						if(m_iUserAlreadySend[iMyView][i]>0)
						{
							iCount++;
						}
					}
					if(iCount<=0)
					{
						return;
					}
					
					break;
				}
			default:
				{
					break;
				}
			}
		}
			if(!callCard->bFlag)
			{
				GameEnd();
			}
			else
			{
				if(255!=callCard->bNextStation)
				{
					if(m_iMyDeskStation==callCard->bDeskStation)
					{
						for(int i=0;i<6;i++)
						{
							SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false);
						}

					}
					if(m_iMyDeskStation==callCard->bNextStation)
					{
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,true);
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,true);
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,true);
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
						SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,true);
						SetStationParameter(GS_PLAY_GAME);
					}
					int iNextView=m_pGameFrame->DeskStation2View(callCard->bNextStation);
					SetContainVisible(CTN_1014089_CTN_2000110+iNextView,true);
					m_iClockLabel=m_bCallCardTime;											//��ǰʱ�ӱ�ǩ
					m_iCurrentID=CTN_1014089_CTN_2000110_Num_2000226+iNextView;				//��ǰʱ��ID
					//�����ȴ�ʱ��
					m_pGameFrame->SetTimer(ID_OUT_CARD,1000);
					SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
					m_bScorePeople=callCard->bNextStation;
				}
				else
				{
					SetContainVisible(CTN_1014089_CTN_2000110+iMyView,true);
					m_iClockLabel=m_bCallCardTime;											//��ǰʱ�ӱ�ǩ
					m_iCurrentID=CTN_1014089_CTN_2000110_Num_2000226+iMyView;				//��ǰʱ��ID
					//�����ȴ�ʱ��
					m_pGameFrame->SetTimer(ID_OUT_CARD,1000);
					SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
					m_bScorePeople=callCard->bDeskStation;
				}
			}
	}
	else if(SERVER_EXTRA_CLICK_STOP==callCard->iStyle)				//ͣ��
	{
		
		//code:������м�ʱ��
		for(int i=0;i<PLAY_COUNT;i++)
		{
			SetContainVisible(CTN_1014089_CTN_2000110+i,false);
		}
		//ɱ�����м�ʱ��
		KillAllGameTime();

		if(!callCard->bFlag)
		{
			GameEnd();
		}
		else
		{
			if(m_iMyDeskStation==callCard->bDeskStation)
			{
				for(int i=0;i<6;i++)
				{
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false);
				}

			}
			m_bScorePeople=callCard->bNextStation;
			
			if(m_iMyDeskStation==callCard->bNextStation)
			{
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,true);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,true);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,true);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,true);
			}
			//��ʼ�з�
			int iNextView=m_pGameFrame->DeskStation2View(m_bScorePeople);
			SetContainVisible(CTN_1014089_CTN_2000110+iNextView,true);

			m_iClockLabel=m_bCallCardTime;											//��ǰʱ�ӱ�ǩ
			m_iCurrentID=CTN_1014089_CTN_2000110_Num_2000226+iNextView;					//��ǰʱ��ID
			//�����ȴ�ʱ��
			m_pGameFrame->SetTimer(ID_OUT_CARD,1000);
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
		}
	}
	else if(SERVER_EXTRA_CLICK_TIMEEND==callCard->iStyle)
	{
		//code:������м�ʱ��
		for(int i=0;i<PLAY_COUNT;i++)
		{
			SetContainVisible(CTN_1014089_CTN_2000110+i,false);
		}
		//ɱ�����м�ʱ��
		KillAllGameTime();

		if(!callCard->bFlag)
		{
			GameEnd();
			return;
		}

		if(255!=callCard->bNextStation && callCard->bFlag)
		{
			
			if(m_iMyDeskStation==callCard->bNextStation)
			{
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,true);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,true);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,true);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
				SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,true);
			}
			m_bScorePeople=callCard->bNextStation;

			//��ʼ�з�
			int iNextView=m_pGameFrame->DeskStation2View(callCard->bNextStation);
			SetContainVisible(CTN_1014089_CTN_2000110+iNextView,true);

			m_iClockLabel=m_bCallCardTime;											//��ǰʱ�ӱ�ǩ
			m_iCurrentID=CTN_1014089_CTN_2000110_Num_2000226+iNextView;					//��ǰʱ��ID
			//�����ȴ�ʱ��
			m_pGameFrame->SetTimer(ID_OUT_CARD,1000);
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
		}
	}
	if(m_bNtPeople==m_iMyDeskStation)
	{
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,false);
	}
}
//�������������������Ϸ����������Ϣ
void CNewGameClient::OnHandleNormalFinish(void *buffer)
{
	GameMessageNormalFinish *normalFinish=(GameMessageNormalFinish *)buffer;
	if(NULL==normalFinish)
	{
		return;
	}
	SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178,false);
	SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000179,false);
	SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000180,false);
	SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000181,false);
	SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000182,false);
	SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,false);
	SetContainVisible(CTN_1014089_CTN_1004100,true);
	
	//��ʼ�������
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetControlVisible(CTN_1014089_CTN_1004100_TEXT_2000255+i,false);
		SetControlVisible(CTN_1014089_CTN_1004100_TEXT_1004120+i,false);
		SetControlVisible(CTN_1014089_CTN_1004100_TEXT_2000143+i,false);
		SetControlVisible(CTN_1014089_CTN_1004100_Num_2000208+i,false);
		SetControlVisible(CTN_1014089_CTN_1004100_IMG_2000212+i,false);
		SetControlVisible(CTN_1014089_CTN_1004100_Num_2000216+i,false);
	}
	UserInfoForGame userInfo;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pGameFrame->GetUserInfo(i,userInfo) && 1==normalFinish->bIsRuning[i])
		{
			if(m_bNtPeople==i)
			{
				SetControlVisible(CTN_1014089_CTN_1004100_TEXT_2000255+i,true);
			}
			SetTextShowText(CTN_1014089_CTN_1004100_TEXT_1004120+i,userInfo.szNickName,true);
			SetTextinfo(CTN_1014089_CTN_1004100_TEXT_2000143+i,normalFinish->i64UserScore[i],true);
			SetInumInfo(CTN_1014089_CTN_1004100_Num_2000208+i,normalFinish->iUserPoint[i],true);
			SetInumShow(CTN_1014089_CTN_1004100_Num_2000216+i,0,true);
			if(normalFinish->i64UserScore[i]>0)
			{
				CString imageSrc;
				imageSrc.Format("./resources/image/win.png");
				SetImageLoadSrc(CTN_1014089_CTN_1004100_IMG_2000212+i,imageSrc,true);
				PlayGameSound(SOUND_WIN,false);
			}
			else if(0==normalFinish->i64UserScore[i])
			{
				CString imageSrc;
				imageSrc.Format("./resources/image/equal.png");
				SetImageLoadSrc(CTN_1014089_CTN_1004100_IMG_2000212+i,imageSrc,true);
				PlayGameSound(SOUND_WIN,false);
			}
			else 
			{
				CString imageSrc;
				imageSrc.Format("./resources/image/lost.png");
				SetImageLoadSrc(CTN_1014089_CTN_1004100_IMG_2000212+i,imageSrc,true);
				PlayGameSound(SOUND_LOSE,false);
			}
		}
	}
	KillAllGameTime();
	m_IsJoin=false;
	SetContainVisible(CTN_1014089_CTN_2000112,true);
	m_iClockLabel=m_bGameBeginWaitTime;						//��ǰʱ�ӱ�ǩ
	m_iCurrentID=CTN_1014089_CTN_2000112_Num_2000228;					//��ǰʱ��ID
	//�����ȴ�ʱ��
	m_pGameFrame->SetTimer(ID_WAIT_NEXT,1000);
	SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
	SetStationParameter(GS_WAIT_NEXT);
}

//�������û���Ϣ
void CNewGameClient::HandleSuperMessage(void *buffer)
{
	SUPERSTATE *pSuperUser = (SUPERSTATE *) buffer;
	if (NULL == pSuperUser)
	{
		return ;
	}
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation(); 
	if(m_iMyDeskStation==pSuperUser->byDeskStation)
	{
		m_bIsSuperUser	 = TRUE;	//�Ƿ񳬼��û�
	}
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
//�������ֿؼ���ʾ����
void CNewGameClient::SetInumShow(const int iNumID, __int64 i64Num,bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		NEW_NUM_STRUCT NumStruct;

		NumStruct.bShowPlusSignOrMinusSign = true;
		NumStruct.bShowComma = true;
		NumStruct.iShowCommaIndex = 3;
		NumStruct.iShowZeroType = 0;
		pNum->SetNewNumStruct(NumStruct);
		pNum->SetNewNum(i64Num);
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
//���ÿؼ��Ƿ���ʾ
void CNewGameClient::SetControlVisible(const int iID,bool bFlag)
{
	IBCInterFace *pControl = NULL;
	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iID));
	if (NULL != pControl)
	{
		pControl->SetControlVisible(bFlag);
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


////��������
BOOL CNewGameClient::CheckVersion(BYTE iVersion,BYTE iVersion2)
{	
	//TCHAR GameName[200];
	//CString strBuffer;
	//wsprintf(GameName,"���� %s ��Ϸ�汾����,��Ҫ�������������°汾��?",m_pGameInfo[]->szGameName);
	//if(iVersion!=m_iVersion || iVersion2!=m_iVersion2)
	//{
	//	TCHAR sz[200];
	//	wsprintf(sz,"�汾��ͻ:��ǰ�汾%d-%d,����˰汾%d-%d",m_iVersion,m_iVersion2,iVersion,iVersion2);
	//}
	//if(iVersion!=m_iVersion)
	//{

	//	if (IDOK==AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
	//	{
	//		strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
	//		ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
	//	}
	//	PostMessage(WM_CLOSE,0,0);
	//	return TRUE;
	//}

	//if(iVersion2!=m_iVersion2)
	//{
	//	if (IDOK==AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
	//	{
	//		strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
	//		ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
	//	}
	//	PostMessage(WM_CLOSE,0,0);
	//	return TRUE;
	//}
	return TRUE;
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
	CString debg;
	debg.Format("debug-ResetGameFrame(client)\n");
	OutputDebugString(debg);
	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	OutputDebugString("lw::����뿪GameUserLeft");
	return 0;
}
/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	OutputDebugString("lw::C_��Ҷ��� GameUserOffLine");
	return 0;
}
// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	/*if (m_byGameStation > GS_FREE_STATUS && m_byGameStation < GS_WAIT_NEXT)
	{
	OutputDebugString("dwjlog::C_������Ϸ��...");
	return true;
	}*/
	
	return false;
}

///����˳���Ϸ
bool CNewGameClient::OnGameQuiting()
{
	if(m_pGameFrame->GetIsWatching())
	{
		return true ; 
	}

	CString szAppPath = CINIFile::GetAppPath();
	szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
	int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
	CString szAppPathParent = szAppPath.Left(nPosOfSlash);


	CString strKeyName  ; 
	strKeyName.Format("%d" ,NAME_ID) ; 
	CBcfFile File(szAppPathParent + "\\SpecialRule.bcf") ; 

	bool   bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strKeyName ,1) ; 

	if(m_byGameStation > GS_WAIT_ARGEE && m_byGameStation < GS_WAIT_NEXT)
	{
		OutputDebugString("exit-OnGameQuiting========1(Client)");
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
	case ID_BEGIN_TIME:
		{
			m_iClockLabel--;
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			TimeWarning(m_iClockLabel);
			if(m_iClockLabel<=0)
			{
				ResetGameData();
				ResetGameDialog();
				KillAllGameTime();
				m_pGameFrame->CloseClient();
			}
			return ;
		}
	case ID_WAIT_NEXT:
		{
			m_iClockLabel--;
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			TimeWarning(m_iClockLabel);
			/*if(m_iClockLabel==15)
			{
			SetContainVisible(CTN_1014089_CTN_1004100,false);
			}*/
			if(m_iClockLabel<=0)
			{
				ResetGameData();
				ResetGameDialog();
				KillAllGameTime();
				m_pGameFrame->CloseClient();
			}
			return ;
		}
	case ID_NOTE:
		{
			m_iClockLabel--;
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			TimeWarning(m_iClockLabel);
			if(m_iClockLabel<=0)
			{
				for(int i=0;i<PLAY_COUNT;i++)
				{
					SetContainVisible(CTN_1014089_CTN_2000110+i,false);
				}
				m_pGameFrame->KillTimer(ID_NOTE) ;
				GameStationButton operateMessage;
				operateMessage.bDeskStation=m_iMyDeskStation;
				operateMessage.iBtType=CLICK_OPERATE;
				operateMessage.iBtParam=2;
				m_pGameFrame->SendGameData(&operateMessage, sizeof(operateMessage) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
			}
			return ;
		}
case ID_OUT_CARD:
		{
			
			m_iClockLabel--;
			SetInumInfo(m_iCurrentID,(__int64)m_iClockLabel ,true);
			TimeWarning(m_iClockLabel);
			if(m_iClockLabel<=0)
			{
				m_pGameFrame->KillTimer(ID_OUT_CARD) ;
				for(int i=0;i<PLAY_COUNT;i++)
				{
					SetContainVisible(CTN_1014089_CTN_2000110+i,false);
				}
				if(m_bScorePeople==m_iMyDeskStation)
				{
					GameStationButton btOperate;
					btOperate.bDeskStation=m_iMyDeskStation;
					btOperate.iBtType=CLICK_PLAYOPERATE;
					btOperate.iBtParam=5;	//��ʾ��Ҫ��
					m_pGameFrame->SendGameData(&btOperate, sizeof(btOperate) ,MDM_GM_GAME_NOTIFY,GAME_BUTTON,0);
					SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000183,false);
				}
			}
			break;
		}
	default:
		{
			return;
		}	
	}
}
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


//---------------------------------------------------------------------------------------
//������Ϸ��Ч
void	CNewGameClient::PlayGameSound(int SoundID, bool bISLoop )
{
	if(!m_SoundEngineGame)
	{
		return ;
	}
	if(!m_iGameSet[0])
	{
		return;
	}
	CString  folder ;
	CString strPath = CINIFile::GetAppPath() ;
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 

	switch(SoundID)
	{
	case SOUND_BEGIN_GAME:
		{
			folder.Format("./music/cardwav/GameBegin.wav");
			break;
		}
	case SOUND_TIME_0:
		{
			folder.Format("./music/cardwav/time0.wav");
			break;
		}
	case SOUND_TIME_1:
		{
			folder.Format("./music/cardwav/time1.wav");
			break;
		}
	case SOUND_TIME_2:
		{
			folder.Format("./music/cardwav/time2.wav");
			break;
		}
	case SOUND_TIME_3:	
		{
			folder.Format("./music/cardwav/time3.wav");
			break;
		}
	case SOUND_TIME_4:	
		{
			folder.Format("./music/cardwav/time4.wav");
			break;
		}
	case SOUND_TIME_5:	
		{
			folder.Format("./music/cardwav/time5.wav");
			break;
		}
	case SOUND_WIN:		
		{
			folder.Format("./music/cardwav/win.wav");
			break;
		}
	case SOUND_LOSE:	
		{
			folder.Format("./music/cardwav/lost.wav");
			break;
		}
	case SOUND_ADDDOUBLE:
		{
			folder.Format("./music/cardwav/adddouble.wav");
			break;
		}
	case SOUND_XIAZHU:		
		{
			folder.Format("./music/cardwav/chip.wav");
			break;
		}
	case SOUND_GIVEUP:		
		{
			folder.Format("./music/cardwav/pass.wav");
			break;
		}
	case SOUND_SENDCARD:		
		{
			folder.Format("./music/cardwav/sendcard.wav");
			break;
		}
	case SOUND_HIT_BEGIN:
		{
			folder.Format("./music/cardwav/hitbegin.wav");
			break;
		}
	
	case SOUND_HIT_LEAVE:
		{
			folder.Format("./music/cardwav/leave.wav");
			break;
		}
	default:
		{
			break;
		}
	}
	wsprintf(szFileName,"%s%s",strPath ,folder);
	m_SoundEngineGame->setSoundVolume(1.0);
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}
/*
@brief:���Ŷ���
@param:ACTION_ID,�����ؼ�ID��CONTROL_ID �����ƵĿؼ�,START_ID ���ID,END_ID �յ�ID
@return:void
*/
void CNewGameClient::ShowAction(const int ACTION_ID,const int CONTROL_ID,const int START_ID,const int END_ID,int second)
{
	
	//��ö����ؼ�ID
	IMoveAction* pMoveAction = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,ACTION_ID);

	//��ñ����Ƶ�ID
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CONTROL_ID));

	POINT pStart,pEnd;

	//��÷������
	IImage *pImage_1 = NULL;
	pImage_1 = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(START_ID));
	if (NULL != pImage_1)
	{
		pStart.x=pImage_1->GetGameX();
		pStart.y=pImage_1->GetGameY();
	}

	//��÷����յ�
	IImage *pImage_2 = NULL;
	pImage_2 = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(END_ID));
	if (NULL != pImage_2)
	{
		pEnd.x=pImage_2->GetGameX();
		pEnd.y=pImage_2->GetGameY();
	}
	
	
	if(NULL!=pMoveAction && NULL!=pImage)
	{
		
		pMoveAction->SetControlingCtrlID(CONTROL_ID);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pEnd);
		pMoveAction->SetTotalMoveTime(second);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
	}
	
}
/*
@brief:���Ŷ���
@param:ACTION_ID,�����ؼ�ID��CONTROL_ID �����ƵĿؼ�,START_ID ���ID,END_ID �յ�ID
@return:void
*/
void CNewGameClient::ShowBetAction(const int ACTION_ID,const int CONTROL_ID,const int START_ID,const int END_ID,int second)
{
	
	//��ö����ؼ�ID
	IMoveAction* pMoveAction = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,ACTION_ID);

	//��ñ����Ƶ�ID
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CONTROL_ID));

	POINT pStart,pEnd;

	//��÷������
	IText *pImage_1 = NULL;
	pImage_1 = dynamic_cast<IText *>(m_pUI->GetIControlbyID(START_ID));
	if (NULL != pImage_1)
	{
		pStart.x=pImage_1->GetGameX();
		pStart.y=pImage_1->GetGameY();
	}

	//��÷����յ�
	IText *pImage_2 = NULL;
	pImage_2 = dynamic_cast<IText *>(m_pUI->GetIControlbyID(END_ID));
	if (NULL != pImage_2)
	{
		pEnd.x=pImage_2->GetGameX();
		pEnd.y=pImage_2->GetGameY();
	}


	if(NULL!=pMoveAction && NULL!=pImage)
	{
		
		pMoveAction->SetControlingCtrlID(CONTROL_ID);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pEnd);
		pMoveAction->SetTotalMoveTime(second);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
	}
	
}
//�ɲ����Ʒ���
void CNewGameClient::OperateSendCardList(const int id,BYTE bCardList[])
{
	int iCount=0;
	BYTE bTemp[13];
	memset(bTemp,0,sizeof(bTemp));
	for(int i=0;i<13;i++)
	{
		if(bCardList[i]>0)
		{
			bTemp[i]=bCardList[i];
			iCount++;
		}
	}
	if(iCount<=0)
	{
		return;
	}
	IOperateCard *moveOperate;
	moveOperate=dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(id));
	if(moveOperate)
	{
		moveOperate->SetControlVisible(true);
		moveOperate->SetCardList(bTemp,iCount);
	}
}

//���ɲ����Ʒ���
void CNewGameClient::NoOperateSendCardList(const int id,BYTE bCardList[])
{
	int iCount=0;
	BYTE bTemp[13];
	memset(bTemp,0,sizeof(bTemp));
	for(int i=0;i<13;i++)
	{
		if(bCardList[i]>0)
		{
			bTemp[i]=bCardList[i];
			iCount++;
		}
	}
	if(iCount<=0)
	{
		return;
	}
	bTemp[0]=0;
	INoMoveCard *noMove;
	noMove=dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(id));
	if(noMove)
	{
		noMove->SetControlVisible(true);
		noMove->SetCardList(bTemp,iCount);
	}
}
//�õ��Ƶ��С
int CNewGameClient::GetPoints(BYTE bCardList[],int iSize)
{
	BYTE iPoint=0,iTotalPoint=0;
	for(int i=0;i<iSize;i++)
	{
		switch(bCardList[i])
		{
		case 0x01:
		case 0x11:
		case 0x21:
		case 0x31:
			{
				iPoint=2;
				break;
			}
		case 0x02:
		case 0x12:
		case 0x22:
		case 0x32:
			{
				iPoint=3;
				break;
			}
		case 0x03:
		case 0x13:
		case 0x23:
		case 0x33:
			{
				iPoint=4;
				break;
			}
		case 0x04:
		case 0x14:
		case 0x24:
		case 0x34:
			{
				iPoint=5;
				break;
			}
		case 0x05:
		case 0x15:
		case 0x25:
		case 0x35:
			{
				iPoint=6;
				break;
			}
		case 0x06:
		case 0x16:
		case 0x26:
		case 0x36:
			{
				iPoint=7;
				break;
			}
		case 0x07:
		case 0x17:
		case 0x27:
		case 0x37:
			{
				iPoint=8;
				break;
			}
		case 0x08:
		case 0x18:
		case 0x28:
		case 0x38:
			{
				iPoint=9;
				break;
			}
		case 0x09:
		case 0x19:
		case 0x29:
		case 0x39:
			{
				iPoint=10;
				break;
			}
		case 0x0A:
		case 0x1A:
		case 0x2A:
		case 0x3A:
			{
				iPoint=10;
				break;
			}
		case 0x0B:
		case 0x1B:
		case 0x2B:
		case 0x3B:
			{
				iPoint=10;
				break;
			}
		case 0x0C:
		case 0x1C:
		case 0x2C:
		case 0x3C:
			{
				iPoint=10;
				break;
			}
		case 0x0D:
		case 0x1D:
		case 0x2D:
		case 0x3D:
			{
				if(iTotalPoint+11>21)
				{
					iPoint=1;
				}
				else
				{
					iPoint=11;
				}
				break;
			}
		default:
			{
				break;
			}
		}
		iTotalPoint+=iPoint;
		iPoint=0;
	}
	return 	iTotalPoint;
}

/*
@brief:û����һ���з�����Ϸ����
*/
void CNewGameClient::GameEnd()
{
	//code:������м�ʱ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetContainVisible(CTN_1014089_CTN_2000110+i,false);
	}
	//ɱ�����м�ʱ��
	KillAllGameTime();
	for(int i=0;i<6;i++)
	{
		SetBtEnable(CTN_1014089_CTN_2000171_BTN_2000178+i,false);
	}
}
//���ݳ���ֵ�õ�ͼƬ�±�
int  CNewGameClient::GetBetIndex(int iValue)
{
	for(int i=0;i<18;i++)
	{
		if(iNote[i]==iValue)
		{
			return i;
		}
	}
	return -1;
}
//��ע
void CNewGameClient::SetNote(__int64 i64Bet,int iView)
{
	int iLabel=0;
	for(int i=0;i<18;i++)
	{
		if(i64Bet==iNote[i])
		{
			iLabel=i;
			break;
		}
	}
	CString imageSrc;
	imageSrc.Format("./resources/image/jeton.png");
	int x=iLabel*31;

	SetPicSize(CTN_1014089_IMG_2000199+iView,imageSrc,true,x,0,31,30);
	SetImageVisible(CTN_1014089_IMG_2000199+iView,true);
	ShowBetAction(ACTION_1+iView,CTN_1014089_IMG_2000199+iView,CTN_1014089_TEXT_1005020+iView,CTN_1014089_TEXT_2000204+iView,100);
}