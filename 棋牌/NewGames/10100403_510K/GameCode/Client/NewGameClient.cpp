#include "stdafx.h"
#include "NewGameClient.h"
#include "../common/IGameFrame.h"
//#include "IGameFrame.h"
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
	//����UI
	wchar_t wszUIFile[MAX_PATH];
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
	m_pUI->LoadData(wszUIFile);
	return 0;
}
//-----------------------------------------------------------------------------------
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	//��������
	SetContainVisible(CTN_2000280_CTN_1003510,false);
	//׼��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetImageVisible(CTN_1014089_IMG_2000288+i,false);
		SetContainVisible(CTN_1014089_CTN_2000111+i,false);
		SetImageVisible(CTN_1014089_IMG_2000401+i,false);
		//չʾ��
		SetNoOperateVisible(CTN_1014089_NoMoveCard_2000304+i,false);
	}
	//��
	SetOperateVisible(CTN_1014089_OperateCard_1004082,false);
	SetNoOperateVisible(CTN_1014089_NoMoveCard_1004081,false);
	SetNoOperateVisible(CTN_1014089_NoMoveCard_1004083,false);
	SetNoOperateVisible(CTN_1014089_NoMoveCard_1004080,false);
	
	//��������
	SetContainVisible(CTN_1014089_CTN_2000293,false);
	//��ʾ
	SetImageVisible(CTN_1014089_IMG_2000297,false);
	//�����
	SetContainVisible(CTN_1014089_CTN_2000287,false);

	//��ǰ�÷�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetTextShowText(CTN_1014089_TEXT_2000301+i,NULL, false);
	}
	//ʣ�༸����
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetTextShowText(CTN_1014089_TEXT_2000298+i,NULL, false);
		SetImageVisible(CTN_1014089_IMG_2000307+i,false);
	}
	//�÷�����
	SetContainVisible(CTN_1014089_CTN_2000314,false);
	SetContainVisible(CTN_1014089_CTN_2000315,false);
	SetContainVisible(CTN_1014089_CTN_2000316,false);
	for(int i=0;i<12;i++)
	{
		SetImageVisible(CTN_1014089_CTN_2000316_IMG_2000341+i,false);
		SetImageVisible(CTN_1014089_CTN_2000316_IMG_2000377+i,false);
		SetImageVisible(CTN_1014089_CTN_2000315_IMG_2000329+i,false);
		SetImageVisible(CTN_1014089_CTN_2000315_IMG_2000365+i,false);
		SetImageVisible(CTN_1014089_CTN_2000314_IMG_2000317+i,false);
		SetImageVisible(CTN_1014089_CTN_2000314_IMG_2000353+i,false);
	}
	SetBtVisible(CTN_1014089_BTN_2000398,false);
	SetBtVisible(CTN_1014089_BTN_2000399,false);
	SetContainVisible(CTN_1014089_CTN_2000405,false);
	SetContainVisible(CTN_1014089_CTN_2000411,false);
	for(int i=0;i<12;i++)
	{
		SetImageVisible(CTN_1014089_CTN_2000411_IMG_2000413+i,false);
	}
	//��ʼ��ť
	SetBtVisible(CTN_1014089_BTN_2000286,false);
	return 0;
}
//---------------------------------------------------------------------------------------
//����UI���� 
void	CNewGameClient::ResetGameDialog()
{
	//��������
	SetContainVisible(CTN_1014089_CTN_2000293,false);
	//��ʾ
	SetImageVisible(CTN_1014089_IMG_2000297,false);
	//�����
	SetContainVisible(CTN_1014089_CTN_2000287,false);
	//��ǰ�÷�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetTextShowText(CTN_1014089_TEXT_2000301+i,NULL, false);
		//չʾ��
		SetNoOperateVisible(CTN_1014089_NoMoveCard_2000304+i,false);
		SetImageVisible(CTN_1014089_IMG_2000401+i,false);
	}
	//ʣ�༸����
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetTextShowText(CTN_1014089_TEXT_2000298+i,NULL, false);
		SetImageVisible(CTN_1014089_IMG_2000307+i,false);
	}
	//�÷�����
	SetContainVisible(CTN_1014089_CTN_2000314,false);
	SetContainVisible(CTN_1014089_CTN_2000315,false);
	SetContainVisible(CTN_1014089_CTN_2000316,false);
	for(int i=0;i<12;i++)
	{
		SetImageVisible(CTN_1014089_CTN_2000316_IMG_2000341+i,false);
		SetImageVisible(CTN_1014089_CTN_2000316_IMG_2000377+i,false);
		SetImageVisible(CTN_1014089_CTN_2000315_IMG_2000329+i,false);
		SetImageVisible(CTN_1014089_CTN_2000315_IMG_2000365+i,false);
		SetImageVisible(CTN_1014089_CTN_2000314_IMG_2000317+i,false);
		SetImageVisible(CTN_1014089_CTN_2000314_IMG_2000353+i,false);
	}
	SetBtVisible(CTN_1014089_BTN_2000398,false);
	SetBtVisible(CTN_1014089_BTN_2000399,false);
	SetContainVisible(CTN_1014089_CTN_2000405,false);
	SetContainVisible(CTN_1014089_CTN_2000411,false);
	for(int i=0;i<12;i++)
	{
		SetImageVisible(CTN_1014089_CTN_2000411_IMG_2000413+i,false);
	}
	//���ÿɲ������Ƿ�ɼ�
	SetOperateVisible(CTN_1014089_OperateCard_1004082,false);
	//���ò��ɲ������Ƿ�ɼ�
	SetNoOperateVisible(CTN_1014089_NoMoveCard_1004081,false);
	//���ò��ɲ������Ƿ�ɼ�
	SetNoOperateVisible(CTN_1014089_NoMoveCard_1004083,false);
}
/// ��ҽ���
int CNewGameClient::GameUserCome(void)
{
	
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation(); 
	return 0;
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
	m_byGameStation=GS_WAIT_ARGEE;
	m_iThinkTime=0;									//��Ϸ˼��ʱ��
	m_iBeginTime=0;									//��Ϸ��ʼʱ��
	m_timeXY[1].x=480;
	m_timeXY[1].y=450;
	m_bIsAuto=false;
	m_bIsHaveBig=false;									//�Ƿ��б��ϼҴ����					
	memset(m_bCompareCard,0,sizeof(m_bCompareCard));    //Ҫ�ȵ���
	m_bIsPlayAction=true;								//�Ƿ񲥷Ŷ���
	m_bIsPlaySound=true;								//�Ƿ񲥷���Ϸ��Ч
	m_bIsWatching=true;									//�Ƿ������Թ�
	////������Ϸ����
	ResetGameData();
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
			case CTN_1014089_BTN_2000286://��ʼ��ť
				{
					//�Թ��߲��ܲ�������
					if(m_pGameFrame->GetIsWatching())
					{
						return 0;
					}
					OnButton(CLICK_START,0);
					break;
				}
			case CTN_1014089_BTN_2000399://����
				{
					//�Թ��߲��ܲ�������
					if(m_pGameFrame->GetIsWatching())
					{
						return 0;
					}
					ResetGameData();
					ResetGameDialog();
					OnButton(CLICK_START,0);
					break;
				}
			case CTN_1014089_BTN_2000398://�뿪��ť
				{
					//�Թ��߲��ܲ�������
					if(m_pGameFrame->GetIsWatching())
					{
						return 0;
					}
					ResetGameData();
					ResetGameDialog();
					m_pGameFrame->CloseClient();
					break;
				}
			case CTN_1014089_CTN_2000293_BTN_2000294://<������Ť
			case CTN_1014089_CTN_2000293_BTN_2000295://<��ʾ��Ť 
			case CTN_1014089_CTN_2000293_BTN_2000296://<���ư�Ť 
				{
					//�Թ��߲��ܲ�������
					if(m_pGameFrame->GetIsWatching())
					{
						return 0;
					}
					if(GetStationParameter()!=GS_PLAY_GAME)
					{
						return 0;
					}
					int iButton=pMessage->_uControlID-CTN_1014089_CTN_2000293_BTN_2000294;
					OnButton(CLICK_OPERATE,iButton);
					break;
				}
			case CTN_1014089_CTN_2000405_BTN_2000406://�йܺ�ȡ���й�
			case CTN_1014089_CTN_2000405_BTN_2000407://�鿴�Ʒ�
			case CTN_1014089_CTN_2000405_BTN_2000408://����
				{
					//�Թ��߲��ܲ�������
					if(m_pGameFrame->GetIsWatching())
					{
						return 0;
					}
					int iButton=pMessage->_uControlID-CTN_1014089_CTN_2000405_BTN_2000406;
					OnButton(CLICK_AUTO,iButton);
					break;
				}
			case CTN_1014089_OperateCard_1004082://�����
				{
					//�Թ��߲��ܲ�������
					if(m_pGameFrame->GetIsWatching())
					{
						return 0;
					}
					PlayGameSound(SOUND_CLICKCARD, false);
					break;
				}
			case CTN_2_BTN_1003500://��Ϸ����
			case CTN_2000280_CTN_1003510_BTN_1003504:
			case CTN_2000280_CTN_1003510_BTN_1003505:
				{
					int iButton=pMessage->_uControlID-CTN_2_BTN_1003500;
					OnButton(CLICK_SET,iButton);
					break;
				}
			default:
				{
					break;
				}
			}
		}
	if(pMessage->_uMessageType == UI_RBUTTONDOWN)//�һ�
	{
		//�Թ��߲��ܲ�������
		if(m_pGameFrame->GetIsWatching())
		{
			return 0;
		}
		if(GetStationParameter()!=GS_PLAY_GAME || m_iMyDeskStation!=m_bCurrentCall)
		{
			return 0;
		}
		OnButton(CLICK_OPERATE,2);
	}
	if(UI_LBUTTONDBDOWN==pMessage->_uMessageType)//˫��
	{
		if(CTN_1014089_OperateCard_1004082!=pMessage->_uControlID)
		{
			//�����˿�
			int iTemp=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);
			ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,iTemp);
		}
		return 0;
	}
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		switch(pMessage->_uControlID)
		{

		}
	}
	if(UI_KEYUP==pMessage->_uMessageType)//����
	{
		if(VK_F12==pMessage->_ch)
		{
		}
	}
	return 0;
	
}
/*
@brief:���������
*/
void CNewGameClient::OnButton(const int iBtType,int iParam,bool bFlag)
{
	if(CLICK_START==iBtType)
	{
		GameClientToServer_Button startMessage;
		startMessage.bDeskStation=m_iMyDeskStation;
		startMessage.iBtType=CLICK_START;
		m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_AGREE,0);
		PlayGameSound(SOUND_HIT_BEGIN,false);
		KillAllGameTime();
		SetContainVisible(CTN_1014089_CTN_2000112,false);
		//��ʼ��ť
		SetBtVisible(CTN_1014089_BTN_2000286,false);
	}
	else if(CLICK_OPERATE==iBtType)
	{
		switch(iParam)
		{
		case 0://����
			{
				GameClientToServer_Button startMessage;
				startMessage.bFlag=bFlag;
				startMessage.bDeskStation=m_iMyDeskStation;
				startMessage.iBtType=CLICK_NOOUT;
				m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);
				PlayGameSound(SOUND_HIT_BUYAO,false);
				int iTemp=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);
				ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,iTemp);
				m_pGameFrame->KillTimer(ID_SCORE_TIME);
				//��ʱ��
				SetContainVisible(CTN_1014089_CTN_2000112,false);
				//��������
				SetContainVisible(CTN_1014089_CTN_2000293,false);
				break;
			}
		case 1://��ʾ
			{
				PlayGameSound(SOUND_TIP,false);
				BYTE iResultCard[CARD_NUMS]={0};
				int iResultCardCount=0;
				int iLeftTemp=m_Logic.CountUserCard(m_bCompareCard,CARD_NUMS);//Ҫѹ���Ƶ�����
				int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
				if(iLeftTemp>0)//��ҪѺ����
				{
					//��ȡ���ϼҴ����
					m_Logic.TackOutCardMoreThanLast(m_bOwnCard,iHandCount,m_bCompareCard,iLeftTemp,iResultCard,iResultCardCount);
					if(iResultCardCount>0)//�бȵ�ǰ�����
					{
						ShowUpCard(CTN_1014089_OperateCard_1004082,iResultCard,iResultCardCount);
						m_bIsHaveBig=true;									//�Ƿ��б��ϼҴ����	
						memset(m_bCompareCard,0,sizeof(m_bCompareCard));
						memcpy(m_bCompareCard,iResultCard,sizeof(BYTE)*iResultCardCount);//Ҫ�ȵ���
						return ;
					}
					if(!m_bIsHaveBig)
					{
						//û�бȵ�ǰ�����
						OnButton(CLICK_OPERATE,0);
					}
					return ;
				}
				//���һ����
				iResultCardCount=1;
				iResultCard[0]=m_bOwnCard[iHandCount-1];

				ShowUpCard(CTN_1014089_OperateCard_1004082,iResultCard,iResultCardCount);
				break;
			}
		case 2://����
			{
				//ɨ���������
				BYTE bCardList[CARD_NUMS]={0};
				GetUpCard(CTN_1014089_OperateCard_1004082,bCardList);
				int iOutCount=m_Logic.CountUserCard(bCardList,CARD_NUMS);//Ҫ������
				int iLeftTemp=m_Logic.CountUserCard(m_bLeftCard,CARD_NUMS);//Ҫѹ���Ƶ�����
				int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
				if(iOutCount<=0 || iHandCount<=0)
				{
					//��ʾ�Ʋ����Ϲ���
					SetImageVisible(CTN_1014089_IMG_2000297,true);
					m_pGameFrame->SetTimer(ID_TIP_NORULE,2*1000);
					return;
				}
				//�Ƿ��������
				int iCardStyle=m_Logic.GetCardShape(bCardList,iOutCount);
				if(!m_Logic.Is510kCardStyle(iCardStyle))
				{
					//��ʾ�Ʋ����Ϲ���
					SetImageVisible(CTN_1014089_IMG_2000297,true);
					m_pGameFrame->SetTimer(ID_TIP_NORULE,2*1000);
					return;
				}
				if(iLeftTemp>0)//��ҪѺ����
				{
					//�ж��Ƿ���Գ���
					if(!m_Logic.CanOutCard(bCardList,iOutCount,m_bLeftCard,iLeftTemp,m_bOwnCard,iHandCount))
					{
						//��ʾ��û�д���ϼ�
						PlayAnimate(CTN_ANI_NOCARD,360,660,true);
						return;

					}
				}
				GameClientToServer_Button startMessage;
				startMessage.bFlag=bFlag;
				startMessage.bDeskStation=m_iMyDeskStation;
				startMessage.iBtType=CLICK_OUTCARD;

				memcpy(startMessage.bCardList,bCardList,sizeof(BYTE)*iOutCount);
				startMessage.iCardCount=iOutCount;
				m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);
				PlayGameSound(SOUND_TANPAI,false);
				//��Ҫ�����ƴ���������ɾ����
				int iDeleteCount=m_Logic.RemoveCard(bCardList,iOutCount,m_bOwnCard,iHandCount);
				//��������ʣ�����
				int iRemainCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);
				//����չ����
				ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,iRemainCount);
				m_pGameFrame->KillTimer(ID_SCORE_TIME);
				//��ʱ��
				SetContainVisible(CTN_1014089_CTN_2000112,false);
				//��������
				SetContainVisible(CTN_1014089_CTN_2000293,false);
				break;
			}
		case 3://ʱ�䵽����
			{
				//���һ����
				BYTE iResultCard[CARD_NUMS]={0};
				int iResultCardCount=0;
				int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
				m_Logic.AutoOutCard(m_bOwnCard,iHandCount,NULL,0,iResultCard,iResultCardCount,true);
				if(iResultCardCount>0)
				{
					//��Ҫ�����ƴ���������ɾ����
					int iDeleteCount=m_Logic.RemoveCard(iResultCard,iResultCardCount,m_bOwnCard,iHandCount);
					GameClientToServer_Button startMessage;
					startMessage.bDeskStation=m_iMyDeskStation;
					startMessage.iBtType=CLICK_OUTCARD;
					startMessage.bFlag=bFlag;
					CopyMemory(startMessage.bCardList,iResultCard,sizeof(BYTE)*iResultCardCount);
					startMessage.iCardCount=iResultCardCount;
					m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);
					//��������ʣ�����
					int iRemainCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);
					//����չ����
					ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,iRemainCount);
					m_pGameFrame->KillTimer(ID_SCORE_TIME);
					//��ʱ��
					SetContainVisible(CTN_1014089_CTN_2000112,false);
					//��������
					SetContainVisible(CTN_1014089_CTN_2000293,false);
				}
				break;
			}
		case 4://�йܳ���
			{
				BYTE iResultCard[CARD_NUMS]={0};
				int iResultCardCount=0;
				int iLeftTemp=m_Logic.CountUserCard(m_bLeftCard,CARD_NUMS);//Ҫѹ���Ƶ�����
				int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
				if(iLeftTemp>0)//��ҪѺ����
				{
					m_Logic.TackOutCardMoreThanLast(m_bOwnCard,iHandCount,m_bLeftCard,iLeftTemp,iResultCard,iResultCardCount);
					if(iResultCardCount>0)//�бȵ�ǰ�����
					{
						//��Ҫ�����ƴ���������ɾ����
						int iDeleteCount=m_Logic.RemoveCard(iResultCard,iResultCardCount,m_bOwnCard,iHandCount);
						
						GameClientToServer_Button startMessage;
						startMessage.bDeskStation=m_iMyDeskStation;
						startMessage.iBtType=CLICK_OUTCARD;
						startMessage.bFlag=bFlag;
						memcpy(startMessage.bCardList,iResultCard,sizeof(BYTE)*iResultCardCount);
						startMessage.iCardCount=iResultCardCount;
						m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);
						//��������ʣ�����
						int iRemainCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);
						//����չ����
						ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,iRemainCount);
						m_pGameFrame->KillTimer(ID_SCORE_TIME);
						//��ʱ��
						SetContainVisible(CTN_1014089_CTN_2000112,false);
						//��������
						SetContainVisible(CTN_1014089_CTN_2000293,false);
					}
					else//û�бȵ�ǰ�����
					{
						//����
						GameClientToServer_Button startMessage;
						startMessage.bDeskStation=m_iMyDeskStation;
						startMessage.iBtType=CLICK_NOOUT;
						startMessage.bFlag=bFlag;
						m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);
						m_pGameFrame->KillTimer(ID_SCORE_TIME);
						//��ʱ��
						SetContainVisible(CTN_1014089_CTN_2000112,false);
						//��������
						SetContainVisible(CTN_1014089_CTN_2000293,false);
					}
				}
				else
				{
					//���һ����
					BYTE iResultCard[CARD_NUMS]={0};
					int iResultCardCount=0;
					m_Logic.AutoOutCard(m_bOwnCard,iHandCount,NULL,0,iResultCard,iResultCardCount,true);
					if(iResultCardCount>0)
					{
						//��Ҫ�����ƴ���������ɾ����
						int iDeleteCount=m_Logic.RemoveCard(iResultCard,iResultCardCount,m_bOwnCard,iHandCount);
						GameClientToServer_Button startMessage;
						startMessage.bDeskStation=m_iMyDeskStation;
						startMessage.iBtType=CLICK_OUTCARD;
						startMessage.bFlag=bFlag;
						CopyMemory(startMessage.bCardList,iResultCard,sizeof(BYTE)*iResultCardCount);
						startMessage.iCardCount=iResultCardCount;
						m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);
						//��������ʣ�����
						int iRemainCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);
						//����չ����
						ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,iRemainCount);
						m_pGameFrame->KillTimer(ID_SCORE_TIME);
						//��ʱ��
						SetContainVisible(CTN_1014089_CTN_2000112,false);
						//��������
						SetContainVisible(CTN_1014089_CTN_2000293,false);
					}
				}
				break;
			}
		}
	}
	else if (CLICK_AUTO==iBtType)
	{
		switch(iParam)
		{
		case 0://�йܡ�ȡ���й�
			{
				GameClientToServer_Button startMessage;
				startMessage.bDeskStation=m_iMyDeskStation;
				startMessage.iBtType=CLICK_AUTO;
				startMessage.iBtParam=0;
				if(m_bIsAuto)//ȡ���й�
				{
					startMessage.bFlag=false;
					m_bIsAuto=false;
				}
				else//�й�
				{
					m_pGameFrame->KillTimer(ID_SCORE_TIME);
					//��ʱ��
					SetContainVisible(CTN_1014089_CTN_2000112,false);
					//��������
					SetContainVisible(CTN_1014089_CTN_2000293,false);
					startMessage.bFlag=true;
					m_bIsAuto=true;
					if(m_iMyDeskStation==m_bCurrentCall)
					{
						OnButton(CLICK_OPERATE,4,false);
					}
				}
				m_pGameFrame->SendGameData(&startMessage, sizeof(startMessage) ,MDM_GM_GAME_NOTIFY,CLIENTTOSERVER_BUTTON,0);
				break;
			}
		case 1://�鿴�Ʒ�
			{
				if(!GetContainVisible(CTN_1014089_CTN_2000411))
				{
					__int64 i64Temp=0;
					int iTemp=0;
					i64Temp=m_Logic.Get510kPoint(m_bHaveCard,12);
					SetTextinfo(CTN_1014089_CTN_2000411_TEXT_2000425,i64Temp,true);
					for(int i=0;i<12;i++)
					{
						SetImageVisible(CTN_1014089_CTN_2000411_IMG_2000413+i,false);
					}
					for(int i=0;i<12;i++)
					{
						if(m_Logic.TestData(m_bHaveCard[i],1))
						{
							UpdateSeeStyle(m_bHaveCard[i],iTemp);
							iTemp++;
						}
					}
					SetContainVisible(CTN_1014089_CTN_2000411,true);
				}
				else
				{
					SetContainVisible(CTN_1014089_CTN_2000411,false);
				}
				break;
			}
		case 2://����
			{
				int iCardCount=0;
				iCardCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);
				m_Logic.ChangeSort(m_bOwnCard,iCardCount);
				ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,iCardCount);
				break;
			}
		}
	}
	else if(CLICK_SET==iBtType)
	{
		switch(iParam)
		{
		case 0://����
			{
				SetContainVisible(CTN_2000280,true);
				SetContainVisible(CTN_2000280_CTN_1003510,true);
				SetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003502,m_bIsPlaySound);
				SetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003503,m_bIsWatching);
				SetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003501,m_bIsPlayAction);
				break;
			}
		case 4://ȷ��
			{
				GetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003502,&m_bIsPlaySound);
				GetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003503,&m_bIsWatching);
				GetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003501,&m_bIsPlayAction);
				if(m_bIsPlaySound)
				{
					m_SoundEngineGame->stopAllSounds();
				}
				if(m_bIsWatching)
				{
					//��Ҫʱ����չ
				}
				SetContainVisible(CTN_2000280_CTN_1003510,false);
				break;
			}
		case 5://ȡ��
			{
				SetContainVisible(CTN_2000280_CTN_1003510,false);
				break;
			}
		}
	}
	
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
//--------------------------------------------------------------------------------------------------------
//��������ؼ��ɼ�
bool	CNewGameClient::GetContainVisible(const int iContainID)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		return pContain->GetVisible();
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
//���ͼƬ�ؼ��Ƿ���ʾ
BOOL CNewGameClient::GetImageIsVisible(const int iImageID)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		return pImage->GetVisible();
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
//���õ�ѡ��ؼ��Ƿ�ѡ��
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
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
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		*bSelected=pRadioButton->GetIsSelect();
		return (*bSelected);
	}
	return TRUE;
}
//����ָ��������ʾָ����֡��
void CNewGameClient::PlayAnimate(const int iID,int x,int y, bool bFlag)
{
	IExAnimate *pControl = NULL;
	pControl = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(iID));
	if (NULL != pControl)
	{
		pControl->SetGameXY(x,y);
		pControl->SetIsLoop(false);
		pControl->SetCurveInstance(CT_NONE);
		pControl->SetPlayState(bFlag);
		//�ڼ�֡
		//pControl->SetShowFrame(iFramNum);
		pControl->SetControlVisible(bFlag);
	}

	return;
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
���ܣ�����˷���
�βΣ�bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowCurrentNoMoveCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
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
���ܣ�������ʾ������
�βΣ�iControlID,�ɲ�����ID,bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowUpCard(const int iControlID,BYTE bUpCardList[],BYTE bUpCardCount)
{
	IOperateCard  *pOperateCard1 = NULL ; 
	pOperateCard1 = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID))  ; 
	if(NULL != pOperateCard1)
	{
		pOperateCard1->SetControlVisible(true) ; 
		pOperateCard1->SetUpCardList(bUpCardList,bUpCardCount);
	}
}
/*
���ܣ���ȡ�������
�βΣ�iControlID,�ɲ�����ID,bCardList�������ֵ
����ֵ��void
*/
void CNewGameClient::GetUpCard(const int iControlID,BYTE bCardList[])
{
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID)); 
	memset(bCardList,255,sizeof(bCardList));
	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(true) ; 
		pOperateCard->GetUpCardList(bCardList) ; 
	}
}
/*
���ܣ���ȡ������Ƶ�����
�βΣ�iControlID,�ɲ�����ID
����ֵ��������Ƶ�����
*/
int CNewGameClient::GetUpCardNum(const int iControlID)
{
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID))  ; 

	BYTE bCardList[5];
	int iUpCount=0;
	memset(bCardList,255,sizeof(bCardList));
	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(true) ; 
		pOperateCard->GetUpCardList(bCardList) ; 
	}
	for(int i=0;i<5;i++)
	{
		if(255==bCardList[i])
		{
			continue;
		}
		iUpCount++;
	}
	return iUpCount;
}
//���ÿɲ������Ƿ�ɼ�
void CNewGameClient::SetOperateVisible(const int iControlID,bool bFlag)
{
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID)); 
	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(bFlag) ; 
	}
}
//���ò��ɲ������Ƿ�ɼ�
void CNewGameClient::SetNoOperateVisible(const int iControlID,bool bFlag)
{
	INoMoveCard  *pNoOperateCard1 = NULL ; 
	pNoOperateCard1 = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 
	if(NULL != pNoOperateCard1)
	{
		pNoOperateCard1->SetControlVisible(bFlag) ; 
	}
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


//-----------------------------------------------------------------------------------------
/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	switch(nSubId)
	{
	case ASS_GM_GAME_STATION:   ///<����״̬(ƽ̨���õ�)
		{
			SetGameStation(buffer, nLen);    
			break;        
		}
	case SERVERTOCLIENT_AGREE:			//ͬ����Ϸ
		{
			if(nLen!=sizeof(MSG_GR_R_UserAgree))
			{
				return 0;
			}	
			OnAgreeGame(buffer);
			break;
		}
	case SERVERTOCLIENT_SEND://��ʼ����
		{
			if(nLen != sizeof(GameServerToClient_SendCard))
			{
				return FALSE;
			}
			OnHandleSendCardBegin(buffer);
			break;
		}
	case SERVERTOCLIENT_START:	//��ʼ��Ϸ
		{
			if(nLen != sizeof(GameServerToClient_StartGame))
			{
				return FALSE;
			}
			OnHandleGameBegin(buffer);
			break;
		}
	case SERVERTOCLIENT_NEXT://��һ������
		{
			if(nLen != sizeof(GameServerToClient_NextPeople))
			{
				return FALSE;
			}
			OnHandleCall(buffer);
			break;
		}
	case SERVERTOCLIENT_SHOW://��ʾ�÷���
		{
			if(nLen != sizeof(GameServerToClient_StyleCard))
			{
				return FALSE;
			}
			
			OnHandleShowStyle(buffer);
			break;
		}
	case SERVERTOCLIENT_HANDUP://��Ϸ�������һ��̯��
		{
			if(nLen != sizeof(GameServerToClient_ShowCard))
			{
				return FALSE;
			}
			OnHandleLastShowCard(buffer);
			break;
		}
	case SERVERTOCLIENT_NORMALFINISH://��Ϸ����
		{
			if(nLen != sizeof(GameServerToClient_Finish))
			{
				return FALSE;
			}
			OnHandleNormalFinish(buffer);
			break;
		}
	case SERVERTOCLIENT_AUTO://�й�
		{
			if(nLen != sizeof(GameServerToClient_Auto))
			{
				return FALSE;
			}
			OnHandleAuto(buffer);
			break;
		}
	case SERVERTOCLIENT_CUTEND://��Ϸ��Ϊ������Ҷ��߽���������Թ����
		{
			if(m_pGameFrame->GetIsWatching())
			{
				ResetGameData();
				ResetGameDialog();
				KillAllGameTime();
				m_pGameFrame->CloseClient();
			}
		}
	default:
		{
			break;
		}

	}
	return 0;
}

//-----------------------------------------------------------------------------------------------
/// ������Ϸ�ĵ�ǰ״̬
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 
	
	PlayGameSound(SOUND_BEGIN_GAME,false);
	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:
		{
			if (nLen != sizeof(GameStation))
			{
				return;
			}
			HandleStartStation(pBuffer);

			break; 
		}
	case GS_SEND:		//��Ϸ��״̬
	case GS_PLAY_GAME:
		{
			if (nLen != sizeof(GameStation_0))
			{
				return;
			}
			HandleGameRuningStation(pBuffer);
			break; 
		}
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


/// ��������״̬
int	CNewGameClient::ResetGameFrame(void)
{
	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	if(m_bUserPrepare[nIndex])
	{
		//��λ��ת��ͼλ��
		int viewStation=m_pGameFrame->DeskStation2View(nIndex);
		//��ʾ׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_2000288+viewStation,false);
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
		if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪��Ϸ�����йܴ��� ���Ƿ�Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
		{
			return false ; 
		}	
		m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}
	return true;
}
//---------------------------------------------------------------------------------------
///ɾ�����ж�ʱ��,����ע����ʤ������ʾ��ʱ����
void CNewGameClient:: KillAllGameTime() 
{
	m_pGameFrame->KillTimer(ID_BEGIN_TIME);
	m_pGameFrame->KillTimer(ID_SCORE_TIME);
	m_pGameFrame->KillTimer(ID_TIP_NORULE);
	m_pGameFrame->KillTimer(ID_SEND_CARD);
}
/// ��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
	case ID_BEGIN_TIME:
		{
			m_bCurrentClockLabel--;
			if(m_bCurrentClockLabel<=5)//���ŵȴ�ʱ�侯ʾ��
			{
				TimeWarning(m_bCurrentClockLabel);
			}
			SetInumInfo(m_iCurrentClockControlID,(__int64)m_bCurrentClockLabel ,true);
			if(m_bCurrentClockLabel<=0)
			{
				m_pGameFrame->KillTimer(ID_BEGIN_TIME);
				SetContainVisible(CTN_1014089_CTN_2000113,false);
				ResetGameDialog();
				ResetGameData();
				m_pGameFrame->CloseClient();
			}
			return ;
		}
	case ID_SCORE_TIME:
		{
			m_bCurrentClockLabel--;
			
			if(m_bCurrentClockLabel<=5)//���ŵȴ�ʱ�侯ʾ��
			{
				TimeWarning(m_bCurrentClockLabel);
			}
			SetInumInfo(m_iCurrentClockControlID,(__int64)m_bCurrentClockLabel ,true);
			if(m_bCurrentClockLabel<=0)
			{
				m_pGameFrame->KillTimer(ID_SCORE_TIME);
				SetContainVisible(m_iCurrentContainControlID,false);
				if(m_bCurrentCall==m_iMyDeskStation)
				{
					if(m_bIsOutCard)//����
					{
						OnButton(CLICK_OPERATE,3,false);
					}
					else//����
					{
						OnButton(CLICK_OPERATE,0,false);
					}
				}
			}
			return ;
		}
	case ID_TIP_NORULE:
		{
			m_pGameFrame->KillTimer(ID_TIP_NORULE);
			SetImageVisible(CTN_1014089_IMG_2000297,false);
			return ;
		}
	case ID_SEND_CARD://����
		{
			PlayGameSound(SOUND_SENDCARD,false);
			m_bCurrentClockLabel++;
			BYTE bCardList[CARD_NUMS]={0};
			if(CARD_NUMS==m_bCurrentClockLabel)
			{
				m_pGameFrame->KillTimer(ID_SEND_CARD);
				if(0==m_bNtViewStation)
				{
					ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_1004081,bCardList,m_bCurrentClockLabel);
				}
				else if(1==m_bNtViewStation)
				{
					if(m_pGameFrame->GetIsWatching())
					{
						//�Թ���Ҳ��ܿ����˵���
						BYTE bTemoList[CARD_NUMS]={48};
						memset(bTemoList,48,sizeof(bTemoList));
						//����չ����
						ShowCurrentCard(CTN_1014089_OperateCard_1004082,bTemoList,m_bCurrentClockLabel);
					}  
					else
					{
						ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,m_bCurrentClockLabel);
					}
				}
				else if(2==m_bNtViewStation)
				{
					ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_1004083,bCardList,m_bCurrentClockLabel);
				}
			}
			else
			{
				ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_1004081,bCardList,m_bCurrentClockLabel);
				ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_1004083,bCardList,m_bCurrentClockLabel);
				if(m_pGameFrame->GetIsWatching())
				{
					//�Թ���Ҳ��ܿ����˵���
					BYTE bTemoList[CARD_NUMS]={48};
					memset(bTemoList,48,sizeof(bTemoList));
					//����չ����
					ShowCurrentCard(CTN_1014089_OperateCard_1004082,bTemoList,m_bCurrentClockLabel);
				}  
				else
				{
					ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,m_bCurrentClockLabel);
				}
				
			}
			
			return;
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
///��ʼ����������
void CNewGameClient::ResetGameData()
{
	//�������
	m_sendQueue.clear();			//����˳������
	memset(m_bOwnCard,0,sizeof(m_bOwnCard));
	memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_DeskCard,0,sizeof(m_DeskCard));
	memset(m_bLeftCard,0,sizeof(m_bLeftCard));
	m_bCurrentClockLabel=0;							//��ǰʱ�ӱ�ǩ
	m_iCurrentClockControlID=0;						//��ǰ���Ƶ�ʱ��ID
	m_iCurrentContainControlID=0;					//��ǰ���Ƶ�ʱ������ID
	m_bCurrentCall=255;
	m_bIsOutCard=false;
	m_bNtPeople=255;									//ׯ��
	m_bNtViewStation=255;								//ׯ�ҵ���ͼλ��
	memset(m_bUserPrepare,0,sizeof(m_bUserPrepare));//���׼��
	m_bIsAuto=false;										//�Ƿ��й�
	memset(m_bHaveCard,0,sizeof(m_bHaveCard));							//�����з���
}
//---------------------------------------------------------------------------------------
//������Ϸ��Ч

void	CNewGameClient::PlayGameSound(int SoundID,bool IsBoy,bool bISLoop)
{
	if(!m_SoundEngineGame)
	{
		return ;
	}
	if(!m_bIsPlaySound)
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
			folder.Format("./resources/music/cardwav/wznn/normal/nn_start.wav");
			break;
		}
	case SOUND_TIME_0:
		{
			folder.Format("./resources/music/cardwav/wznn/normal/time0.wav");
			break;
		}
	case SOUND_TIME_1:
		{
			folder.Format("./resources/music/cardwav/wznn/normal/time1.wav");
			break;
		}
	case SOUND_TIME_2:
		{
			folder.Format("./resources/music/cardwav/wznn/normal/time2.wav");
			break;
		}
	case SOUND_TIME_3:	
		{
			folder.Format("./resources/music/cardwav/wznn/normal/time3.wav");
			break;
		}
	case SOUND_TIME_4:	
		{
			folder.Format("./resources/music/cardwav/wznn/normal/time4.wav");
			break;
		}
	case SOUND_TIME_5:	
		{
			folder.Format("./resources/music/cardwav/wznn/normal/time5.wav");
			break;
		}
	case SOUND_WIN:		
		{
			folder.Format("./resources/music/cardwav/wznn/normal/win.wav");
			break;
		}
	case SOUND_LOSE:	
		{
			folder.Format("./resources/music/cardwav/wznn/normal/lose.wav");
			break;
		}
	case SOUND_CHONGPAI:
		{
			folder.Format("./resources/music/cardwav/wznn/normal/sendcard.ogg");
			break;
		}
	case SOUND_SENDCARD:		
		{
			folder.Format("./resources/music/cardwav/wznn/normal/chip.wav");
			break;
		}
	case SOUND_TIP:		
		{
			folder.Format("./resources/music/cardwav/wznn/normal/addchip.wav");
			break;
		}
	case SOUND_TANPAI:		
		{
			folder.Format("./resources/music/cardwav/wznn/normal/tanpai.wav");
			break;
		}
	case SOUND_CLICKCARD:	
		{
			folder.Format("./resources/music/cardwav/wznn/normal/hitcard.wav");
			break;
		}
	case SOUND_HIT_BEGIN:
		{
			folder.Format("./resources/music/cardwav/wznn/normal/hitbegin.wav");
			break;
		}
	case SOUND_GAME_RUN:
		{
			folder.Format("./resources/music/cardwav/wznn/normal/begingame.wav");
			break;
		}
	case SOUND_HIT_LEAVE:
		{
			folder.Format("./resources/music/cardwav/wznn/normal/leave.wav");
			break;
		}
	default:
		{
			break;
		}
	}
	if(IsBoy)
	{
		switch(SoundID)
		{
		case SOUND_HIT_SANDAIYI://����һ	
			{
				folder.Format("./resources/music/cardwav/wznn/Man/sandaiyi.ogg");
				break;
			}
		case SOUND_HIT_SANDAIDUI://������
			{
				folder.Format("./resources/music/cardwav/wznn/Man/sandaiyidui.ogg");
				break;
			}
		case SOUND_HIT_SANGE://����
			{
				folder.Format("./resources/music/cardwav/wznn/Man/sange.ogg");
				break;
			}
		case SOUND_HIT_BOMB://ը��
			{
				folder.Format("./resources/music/cardwav/wznn/Man/zhadan.ogg");
				break;
			}
		case SOUND_HIT_DANI://����	
			{
				folder.Format("./resources/music/cardwav/wznn/Man/dani1.ogg");
				break;
			}
		case SOUND_HIT_BUYAO://��Ҫ
			{
				folder.Format("./resources/music/cardwav/wznn/Man/buyao1.ogg");
				break;
			}
		case SOUND_HIT_FEIJI://�ɻ�	
			{
				folder.Format("./resources/music/cardwav/wznn/Man/feiji.ogg");
				break;
			}
		case SOUND_HIT_LIANDUI://����	
			{
				folder.Format("./resources/music/cardwav/wznn/Man/liandui.ogg");
				break;
			}
		case SOUND_HIT_SHUNZI://˳��
			{
				folder.Format("./resources/music/cardwav/wznn/Man/shunzi.ogg");
				break;
			}
		case SOUND_HIT_SIDAIER://�Ĵ���	
			{
				folder.Format("./resources/music/cardwav/wznn/Man/sidaier.ogg");
				break;
			}
		case SOUND_HIT_SIDAILIANGDUI://�Ĵ�����
			{
				folder.Format("./resources/music/cardwav/wznn/Man/sidailiangdui.ogg");
				break;
			}
		case SOUND_HIT_SIGNAL_2://����
		case SOUND_HIT_SIGNAL_3:
		case SOUND_HIT_SIGNAL_4:
		case SOUND_HIT_SIGNAL_5:
		case SOUND_HIT_SIGNAL_6:
		case SOUND_HIT_SIGNAL_7:
		case SOUND_HIT_SIGNAL_8:
		case SOUND_HIT_SIGNAL_9:
		case SOUND_HIT_SIGNAL_10:
		case SOUND_HIT_SIGNAL_11:
		case SOUND_HIT_SIGNAL_12:
		case SOUND_HIT_SIGNAL_13:
		case SOUND_HIT_SIGNAL_14:
		case SOUND_HIT_SIGNAL_15:
		case SOUND_HIT_SIGNAL_16:
			{
				int index=(SoundID-SOUND_HIT_SIGNAL_2)+2;
				folder.Format("./resources/music/cardwav/wznn/Man/%d.ogg",index);
				break;
			}
		case SOUND_HIT_DOUBLE_2://����		
		case SOUND_HIT_DOUBLE_3:
		case SOUND_HIT_DOUBLE_4:
		case SOUND_HIT_DOUBLE_5:
		case SOUND_HIT_DOUBLE_6:
		case SOUND_HIT_DOUBLE_7:
		case SOUND_HIT_DOUBLE_8:
		case SOUND_HIT_DOUBLE_9:
		case SOUND_HIT_DOUBLE_10:
		case SOUND_HIT_DOUBLE_11:
		case SOUND_HIT_DOUBLE_12:
		case SOUND_HIT_DOUBLE_13:
		case SOUND_HIT_DOUBLE_14:
			{
				int index=(SoundID-SOUND_HIT_DOUBLE_2)+2;
				folder.Format("./resources/music/cardwav/wznn/Man/dui%d.ogg",index);
				break;
			}
		}
	}
	else
	{
		switch(SoundID)
		{
		case SOUND_HIT_SANDAIYI://����һ	
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/sandaiyi.ogg");
				break;
			}
		case SOUND_HIT_SANDAIDUI://������
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/sandaiyidui.ogg");
				break;
			}
		case SOUND_HIT_SANGE://����
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/sange.ogg");
				break;
			}
		case SOUND_HIT_BOMB://ը��
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/zhadan.ogg");
				break;
			}
		case SOUND_HIT_DANI://����	
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/dani1.ogg");
				break;
			}
		case SOUND_HIT_BUYAO://��Ҫ
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/buyao1.ogg");
				break;
			}
		case SOUND_HIT_FEIJI://�ɻ�	
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/feiji.ogg");
				break;
			}
		case SOUND_HIT_LIANDUI://����
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/liandui.ogg");
				break;
			}
		case SOUND_HIT_SHUNZI://˳��
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/shunzi.ogg");
				break;
			}
		case SOUND_HIT_SIDAIER://�Ĵ���	
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/sidaier.ogg");
				break;
			}
		case SOUND_HIT_SIDAILIANGDUI://�Ĵ�����
			{
				folder.Format("./resources/music/cardwav/wznn/Woman/sidailiangdui.ogg");
				break;
			}
		case SOUND_HIT_SIGNAL_2://����
		case SOUND_HIT_SIGNAL_3:
		case SOUND_HIT_SIGNAL_4:
		case SOUND_HIT_SIGNAL_5:
		case SOUND_HIT_SIGNAL_6:
		case SOUND_HIT_SIGNAL_7:
		case SOUND_HIT_SIGNAL_8:
		case SOUND_HIT_SIGNAL_9:
		case SOUND_HIT_SIGNAL_10:
		case SOUND_HIT_SIGNAL_11:
		case SOUND_HIT_SIGNAL_12:
		case SOUND_HIT_SIGNAL_13:
		case SOUND_HIT_SIGNAL_14:
		case SOUND_HIT_SIGNAL_15:
		case SOUND_HIT_SIGNAL_16:
			{
				int index=(SoundID-SOUND_HIT_SIGNAL_2)+2;
				folder.Format("./resources/music/cardwav/wznn/Woman/%d.ogg",index);
				break;
			}
		case SOUND_HIT_DOUBLE_2://����		
		case SOUND_HIT_DOUBLE_3:
		case SOUND_HIT_DOUBLE_4:
		case SOUND_HIT_DOUBLE_5:
		case SOUND_HIT_DOUBLE_6:
		case SOUND_HIT_DOUBLE_7:
		case SOUND_HIT_DOUBLE_8:
		case SOUND_HIT_DOUBLE_9:
		case SOUND_HIT_DOUBLE_10:
		case SOUND_HIT_DOUBLE_11:
		case SOUND_HIT_DOUBLE_12:
		case SOUND_HIT_DOUBLE_13:
		case SOUND_HIT_DOUBLE_14:
			{
				int index=(SoundID-SOUND_HIT_DOUBLE_2)+2;
				folder.Format("./resources/music/cardwav/wznn/Woman/dui%d.ogg",index);
				break;
			}
		}
	}
	wsprintf(szFileName,"%s%s",strPath ,folder);
	m_SoundEngineGame->setSoundVolume(1.0);
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}

/*
@brief:����
@param:bCardValue,��ֵ��bSendPeople����ǰ������
@return:void
*/
void CNewGameClient::SendCard(BYTE bCardValue,BYTE bSendPeople)
{
	switch(bSendPeople)
	{
	case 0:
		{
			
			break;
		}
	case 1:
		{
			
			break;
		}
	case 2:
		{
			
			break;
		}
	case 3:
		{
			
			break;
		}
	default:
		{
			break;	
		}
	}
	
}
//ɾ���ƶ�����
void CNewGameClient::RemoveCard(BYTE bCardList[],BYTE bTarCard,int iCount)
{
	for(int i=0;i<iCount;i++)
	{
		if(bTarCard==bCardList[i])
		{
			bCardList[i]=0;
			continue;
		}
	}
}
//����������
void CNewGameClient::RestartSort(BYTE bCardList[],int iCount)
{
	BYTE bTempList[CARD_NUMS]={0};
	for(int i=0,j=0;i<iCount;i++)
	{
		if(m_Logic.TestData(bCardList[i],1))
		{
			bTempList[j]=bCardList[i];
			j++;
		}
	}
	memset(bCardList,0,sizeof(BYTE)*iCount);
	memcpy(bCardList,bTempList,sizeof(BYTE)*iCount);
}
//CStringתTCHAR
TCHAR* CNewGameClient::CStringToCHAR(CString &str) 
{ 
	int iLen = str.GetLength(); 
	TCHAR* szRs = new TCHAR[iLen]; 
	lstrcpy(szRs, str.GetBuffer(iLen)); 
	str.ReleaseBuffer(); 
	return szRs; 
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


//��һ����������������ִ�д˺���
void CNewGameClient::HandleStartStation(void *buffer)
{
	GameStation *TGameStation = (GameStation *)buffer;
	if(NULL == TGameStation)
	{
		return ;
	}
	//��������
	m_iThinkTime=TGameStation->iThinkTime;
	m_iBeginTime=TGameStation->iBeginTime;

	//��ʼ��ť
	SetBtVisible(CTN_1014089_BTN_2000286,true);
	//��������
	CheckVersion(TGameStation->iVersion,TGameStation->iVersion2);	
	SetStationParameter(TGameStation->iGameStation);
	//��ʾ�Ѿ�׼�����мҵ���ʾ׼��ͼƬ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(TGameStation->bUserReady[i])//�Ѿ�׼��
		{
			int iViewStation=m_pGameFrame->DeskStation2View(i);
			SetImageVisible(CTN_1014089_IMG_2000288+iViewStation,true);
		}

	}
	CopyMemory(m_bUserPrepare,TGameStation->bUserReady,sizeof(m_bUserPrepare));
	//ʱ��λ�øı�
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_TEXT_2000292));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_2000112,true,point.x,point.y);
	//�����ȴ���ʼ��ʱ��
	m_bCurrentClockLabel=m_iBeginTime;
	m_iCurrentClockControlID=CTN_1014089_CTN_2000112_Num_2000114;
	SetContainVisible(CTN_1014089_CTN_2000112,true);
	m_pGameFrame->SetTimer(ID_BEGIN_TIME,1000);
	SetInumInfo(m_iCurrentClockControlID,m_bCurrentClockLabel,true);
	//PlayGameSound(SOUND_GAME_E, FALSE); 

}
//��Ϸ������
void CNewGameClient::HandleGameRuningStation(void *buffer)
{
	GameStation_0 *TGameStation = (GameStation_0 *)buffer;
	if(NULL == TGameStation)
	{
		return ;
	}
	//��������
	m_iThinkTime=TGameStation->iThinkTime;
	m_iBeginTime=TGameStation->iBeginTime;
	SetStationParameter(TGameStation->iGameStation);
	memcpy(m_bOwnCard,TGameStation->bCardList,sizeof(m_bOwnCard));

	int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
	m_Logic.SortCard(m_bOwnCard,NULL,iHandCount);
	
	if(m_pGameFrame->GetIsWatching())
	{
		//�Թ���Ҳ��ܿ����˵���
		BYTE bTemoList[CARD_NUMS]={48};
		memset(bTemoList,48,sizeof(bTemoList));
		//����չ����
		ShowCurrentCard(CTN_1014089_OperateCard_1004082,bTemoList,iHandCount);
	}  
	else
	{
		ShowCurrentCard(CTN_1014089_OperateCard_1004082,m_bOwnCard,iHandCount);
	}
	if(m_Logic.TestData(TGameStation->bNextPeople,0))
	{
		m_bCurrentCall=TGameStation->bNextPeople;	//��ǰ�з���			
	}
	
	//�����ϼҳ�������
	memset(m_bLeftCard,0,sizeof(m_bLeftCard));
	int iLeftTemp=m_Logic.CountUserCard(TGameStation->bLeftCard,CARD_NUMS);//Ҫѹ���Ƶ�����
	if(iLeftTemp<=0)//û����Լ��
	{
		memset(m_bLeftCard,0,sizeof(m_bLeftCard));
		m_bIsOutCard=true;//ʱ�䵽�������
	}
	else
	{
		m_bIsOutCard=false;//����ѡ�񲻳���
		memcpy(m_bLeftCard,TGameStation->bLeftCard,sizeof(BYTE)*CARD_NUMS);
	}
	//�йܲ�������
	SetContainVisible(CTN_1014089_CTN_2000405,true);

	int viewStation=0;
	//��ʾ����ҷ���
	UserInfoForGame UserInfo;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (m_pGameFrame->GetUserInfo(i , UserInfo))
		{
			viewStation=m_pGameFrame->DeskStation2View(i);
			CString text;
			TCHAR *ch;
			text.Format("��ǰ�÷�:%I64d��",TGameStation->iPoint[i]);
			ch=CStringToCHAR(text);
			SetTextShowText(CTN_1014089_TEXT_2000301+viewStation,ch, true);
		}
	}
	//��ҵ÷��Ƹ���
	for(int i=0;i<PLAY_COUNT;i++)
	{
		viewStation=m_pGameFrame->DeskStation2View(i);
		for(int j=0;j<12;j++)
		{
			if(m_Logic.TestData(TGameStation->bHavePoint[i][j],1))
			{
				Paste510kPhoto(viewStation,TGameStation->bHavePoint[i][j]);
			}
		}
	}
	//���������ָ���
	for(int i=0;i<PLAY_COUNT;i++)
	{
		viewStation=m_pGameFrame->DeskStation2View(i);
		for(int j=0;j<CARD_NUMS;j++)
		{
			if(!m_Logic.TestData(TGameStation->DeskBehave[i][j],1))
			{
				//����
				CString sImagSrc;
				sImagSrc.Format("./resources/image/pass.png");
				SetImageLoadSrc(CTN_1014089_IMG_2000307+viewStation,sImagSrc,true);
			}
			else
			{
				int iLeftNums=m_Logic.CountUserCard(TGameStation->DeskBehave[i],CARD_NUMS);
				if(iLeftNums>0)
				{
					ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_2000304+viewStation,TGameStation->DeskBehave[i],iLeftNums);
				}
			}
			break;
		}
		
	}
	//�������������
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iMyDeskStation==i)
		{
			continue;
		}
		viewStation=m_pGameFrame->DeskStation2View(i);
		BYTE bTempList[CARD_NUMS]={0};
		ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_1004081+viewStation,bTempList,TGameStation->iPeopleHandCount[i]);
		CString text;
		TCHAR *ch;
		text.Format("ʣ��%d����",TGameStation->iPeopleHandCount[i]);
		ch=CStringToCHAR(text);
		SetTextShowText(CTN_1014089_TEXT_2000298+viewStation,ch, true);
		delete[] ch;
	}

	//���汾���з���
	memcpy(m_bHaveCard,TGameStation->bRoundHaveCard,sizeof(m_bHaveCard));
	//��ǰ��������������
	viewStation=m_pGameFrame->DeskStation2View(m_bCurrentCall);
	SetImageVisible(CTN_1014089_IMG_2000307+viewStation,false);
	SetNoOperateVisible(CTN_1014089_NoMoveCard_2000304+viewStation,false);
	if(TGameStation->iClockLabel>0)
	{
		//�����з�ʱ��
		if(m_Logic.TestData(m_bCurrentCall,0))
		{
			if(m_iMyDeskStation==m_bCurrentCall)
			{
				//��������
				SetContainVisible(CTN_1014089_CTN_2000293,true);
				SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000294,true);
				SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000295,true);
				SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000296,true);
			}
			int viewStation=m_pGameFrame->DeskStation2View(m_bCurrentCall);
			StartTime(viewStation,TGameStation->iClockLabel);
		}
	}
	CString cs;
	cs.Format("clock::TGameStation->iClockLabel=%d",TGameStation->iClockLabel);
	OutputDebugString(cs);
}
//����ͬ����Ϸ��Ϣ
void CNewGameClient::OnAgreeGame(void *buffer)
{
	GameServerToClient_ArgeeGame * agreeGame=(GameServerToClient_ArgeeGame *)buffer;
	if(NULL==agreeGame)
	{
		return ;
	}
	if(m_pGameFrame->GetIsWatching())
	{
		if(m_iMyDeskStation==agreeGame->bDeskStation)
		{
			ResetGameData();
			ResetGameDialog();   
			KillAllGameTime();
			SetContainVisible(CTN_1014089_CTN_2000112,false);
			//��ʼ��ť
			SetBtVisible(CTN_1014089_BTN_2000286,false);
		}
	}
	
	m_bUserPrepare[agreeGame->bDeskStation]=1;
	//��λ��ת��ͼλ��
	int viewStation=m_pGameFrame->DeskStation2View(agreeGame->bDeskStation);
	//��ʾ׼��ͼƬ
	SetImageVisible(CTN_1014089_IMG_2000288+viewStation,TRUE);
	SetContainVisible(CTN_1014089_CTN_2000113+viewStation,false);
	if(m_iMyDeskStation==agreeGame->bDeskStation)
	{
		m_pGameFrame->KillTimer(ID_BEGIN_TIME);
	}
}
/*
���ܣ��������˷������ķ�����Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleSendCardBegin(void *buffer)
{
	GameServerToClient_SendCard *sendAllCard = (GameServerToClient_SendCard *)buffer ;
	if(NULL==sendAllCard)
	{
		return;
	}
	for(int i=0;i<3;i++)
	{
		SetImageVisible(CTN_1014089_IMG_2000288+i,false);
	}
	m_pGameFrame->KillTimer(ID_BEGIN_TIME);
	SetContainVisible(CTN_1014089_CTN_2000113,false);

	SetStationParameter(GS_SEND);
	//����������
	UserInfoForGame UserInfo ;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (m_pGameFrame->GetUserInfo(i , UserInfo))
		{
			int viewStation=m_pGameFrame->DeskStation2View(i);
			//ÿ�������˿˵���Ŀ
			m_iDeskCardCount[viewStation]=sendAllCard->iUserCardCount[i];	
			//ÿ��������˿�1
			memcpy(m_DeskCard[viewStation],sendAllCard->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[viewStation]);
		}
	}
	if(m_Logic.TestData(sendAllCard->bNtPeople,0))
	{
		m_bNtPeople=sendAllCard->bNtPeople;							//ׯ��
		m_bNtViewStation=m_pGameFrame->DeskStation2View(m_bNtPeople);								//ׯ�ҵ���ͼλ��
	}
	memset(m_bOwnCard,0,sizeof(m_bOwnCard));
	memcpy(m_bOwnCard,m_DeskCard[1],sizeof(BYTE)*m_iDeskCardCount[1]);
	m_Logic.SortCard(m_bOwnCard,NULL,m_iDeskCardCount[1]);
	m_bCurrentClockLabel=0;
	//��ʱ������
	m_pGameFrame->SetTimer(ID_SEND_CARD,150);
}
/*
���ܣ��������˷���������Ϸ��ʼ��Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleGameBegin(void *buffer)
{
	GameServerToClient_StartGame *gameBegin = (GameServerToClient_StartGame *)buffer ;
	if(NULL==gameBegin)
	{
		return;
	}
	if(!m_Logic.TestData(gameBegin->iOutDeskStation,0))
	{
		return ;
	}
	SetStationParameter(GS_PLAY_GAME);
	PlayGameSound(SOUND_GAME_RUN,false);
	SetContainVisible(CTN_1014089_CTN_2000405,true);
	CString sImagSrc;
	sImagSrc.Format("./resources/image/�й�.png");
	SetImageLoadSrc(CTN_1014089_CTN_2000405_BTN_2000406,sImagSrc,true);
	

	int viewStation=m_pGameFrame->DeskStation2View(gameBegin->iOutDeskStation);
	
	m_bCurrentCall=gameBegin->iOutDeskStation;
	m_bIsOutCard=true;
	if(m_iMyDeskStation==gameBegin->iOutDeskStation)
	{
		//ʱ��λ�øı�
		SetControlPos(CTN_1014089_CTN_2000112,true,m_timeXY[1].x,m_timeXY[1].y);
		SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000294,false);
		SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000295,false);
		SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000296,true);
		//��������
		SetContainVisible(CTN_1014089_CTN_2000293,true);
	}
	
	UserInfoForGame UserInfo;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (m_pGameFrame->GetUserInfo(i , UserInfo))
		{
			if(m_iMyDeskStation==i)
			{
				continue;
			}
			int viewStation=m_pGameFrame->DeskStation2View(i);
			
			CString text;
			TCHAR *ch;

			text.Format("��ǰ�÷�:0��");
			ch=CStringToCHAR(text);
			SetTextShowText(CTN_1014089_TEXT_2000301+viewStation,ch, true);
			if(m_bNtPeople==i)
			{
				text.Format("ʣ��18����");
			}
			else
			{
				text.Format("ʣ��17����");
			}
			ch=CStringToCHAR(text);
			SetTextShowText(CTN_1014089_TEXT_2000298+viewStation,ch, true);
			delete[] ch;
			
		}
	}
	//������Ϸ˼����ʱ��
	StartTime(viewStation,m_iThinkTime);
}
/*
@brief:����������������Ľ�����Ϣ
*/
void CNewGameClient::OnHandleCall(void *buffer)
{
	GameServerToClient_NextPeople *callCard=(GameServerToClient_NextPeople *)buffer;
	if(NULL==callCard)
	{
		return;
	}
	if(m_pGameFrame->GetIsWatching())
	{
		if(m_iMyDeskStation==callCard->bLeftStation)
		{
			int iLeftNums=m_Logic.CountUserCard(callCard->bCardList,CARD_NUMS);
			int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
			//��Ҫ�����ƴ���������ɾ����
			int iDeleteCount=m_Logic.RemoveCard(callCard->bCardList,iLeftNums,m_bOwnCard,iHandCount);
			//��������ʣ�����
			int iRemainCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);
			//�Թ���Ҳ��ܿ����˵���
			BYTE bTemoList[CARD_NUMS];
			memset(bTemoList,48,sizeof(bTemoList));
			//����չ����
			ShowCurrentCard(CTN_1014089_OperateCard_1004082,bTemoList,iRemainCount);
			m_pGameFrame->KillTimer(ID_SCORE_TIME);
			//��ʱ��
			SetContainVisible(CTN_1014089_CTN_2000112,false);
			//��������
			SetContainVisible(CTN_1014089_CTN_2000293,false);
		}
	}    
	//ɾ����ʱ��
	m_pGameFrame->KillTimer(ID_SCORE_TIME);
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetContainVisible(CTN_1014089_CTN_2000111+i,false);
	}
	//�����з���
	memcpy(m_bHaveCard,callCard->bHaveCard,sizeof(m_bHaveCard));
	//�Ƿ����鿴�Ʒְ�ť
	if(GetContainVisible(CTN_1014089_CTN_2000411))
	{
		int iTemp=0;
		__int64 i64Temp=0;
		i64Temp=m_Logic.Get510kPoint(callCard->bHaveCard,12);
		SetTextinfo(CTN_1014089_CTN_2000411_TEXT_2000425,i64Temp,true);
		for(int i=0;i<12;i++)
		{
			SetImageVisible(CTN_1014089_CTN_2000411_IMG_2000413+i,false);
		}
		for(int i=0;i<12;i++)
		{
			if(m_Logic.TestData(callCard->bHaveCard[i],1))
			{
				UpdateSeeStyle(callCard->bHaveCard[i],iTemp);
				iTemp++;
			}
		}
	}
	
	int viewStation;//��ͼλ��
	if(callCard->bLeftFlag)//�ϸ�����г���
	{
		int iLeftNums=m_Logic.CountUserCard(callCard->bCardList,CARD_NUMS);
		//������ʾ
		PlayCardTip(callCard->bLeftStation,callCard->bCardList,iLeftNums);
		//��ʾչʾ��
		viewStation=m_pGameFrame->DeskStation2View(callCard->bLeftStation);
		
		//�ϼ���������
		if(iLeftNums>0)
		{
			ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_2000304+viewStation,callCard->bCardList,iLeftNums);
		}
		//�����������Ƽ���
		if(m_Logic.TestData(callCard->bNextStation,0))//��Ϸû�н���
		{
			BYTE bCardList[CARD_NUMS]={0};
			ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_1004081+viewStation,bCardList,callCard->iPeopleCount[callCard->bLeftStation]);
		}
	}
	else//�ϸ���ҷ�������
	{
		viewStation=m_pGameFrame->DeskStation2View(callCard->bLeftStation);
		CString sImagSrc;
		sImagSrc.Format("./resources/image/pass.png");
		SetImageLoadSrc(CTN_1014089_IMG_2000307+viewStation,sImagSrc,true);
		PlayGameSound(SOUND_HIT_BUYAO,false);
	}
	UserInfoForGame UserInfo;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (m_pGameFrame->GetUserInfo(i , UserInfo))
		{
			int viewStation=m_pGameFrame->DeskStation2View(i);

			CString text;
			TCHAR *ch;

			text.Format("��ǰ�÷�:%I64d��",callCard->i64PeoplePoint[i]);
			ch=CStringToCHAR(text);
			SetTextShowText(CTN_1014089_TEXT_2000301+viewStation,ch, true);
			delete[] ch;
			if(m_iMyDeskStation==i)
			{
				continue;
			}
			text.Format("ʣ��%d����",callCard->iPeopleCount[i]);
			ch=CStringToCHAR(text);
			SetTextShowText(CTN_1014089_TEXT_2000298+viewStation,ch, true);
			delete[] ch;

		}
	}
	if(!m_Logic.TestData(callCard->bNextStation,0))//��Ϸ�Ѿ�����
	{
		return;
	}
	//����չʾ��
	viewStation=m_pGameFrame->DeskStation2View(callCard->bNextStation);
	SetNoOperateVisible(CTN_1014089_NoMoveCard_2000304+viewStation,false);
	//���ز���ͼƬ
	SetImageVisible(CTN_1014089_IMG_2000307+viewStation,false);
	
	m_bCurrentCall=callCard->bNextStation;
	if(m_iMyDeskStation==callCard->bNextStation)
	{
		//�����ϼҳ�������
		memset(m_bLeftCard,0,sizeof(m_bLeftCard));
		int iLeftTemp=m_Logic.CountUserCard(callCard->bCardList,CARD_NUMS);//Ҫѹ���Ƶ�����
		if(iLeftTemp<=0)//û����Լ��
		{
			memset(m_bLeftCard,0,sizeof(m_bLeftCard));
			m_bIsOutCard=true;//ʱ�䵽�������
		}
		else
		{
			m_bIsOutCard=false;//����ѡ�񲻳���
			memset(m_bCompareCard,0,sizeof(m_bCompareCard));
			memcpy(m_bLeftCard,callCard->bCardList,sizeof(BYTE)*CARD_NUMS);
			memcpy(m_bCompareCard,callCard->bCardList,sizeof(BYTE)*CARD_NUMS);//Ҫ�ȵ���
		}
		if(m_bIsAuto)
		{
			OnButton(CLICK_OPERATE,4,false);
			return;
		}
		//ʱ��λ�øı�
		SetControlPos(CTN_1014089_CTN_2000112,true,m_timeXY[1].x,m_timeXY[1].y);
		
		if(m_bIsOutCard)
		{
			SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000294,false);
			SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000295,false);
			SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000296,true);
		}
		else
		{
			m_bIsHaveBig=false;									//�Ƿ��б��ϼҴ����					
			SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000294,true);
			SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000295,true);
			SetBtVisible(CTN_1014089_CTN_2000293_BTN_2000296,true);
		}
		//��������
		SetContainVisible(CTN_1014089_CTN_2000293,true);
		BYTE iResultCard[CARD_NUMS]={0};
		int iResultCardCount=0;
		int iHandCount=m_Logic.CountUserCard(m_bOwnCard,CARD_NUMS);//���е�������
		if(iLeftTemp>0)//��ҪѺ����
		{
			m_Logic.TackOutCardMoreThanLast(m_bOwnCard,iHandCount,m_bLeftCard,iLeftTemp,iResultCard,iResultCardCount);
			if(iResultCardCount<=0)//û�бȵ�ǰ�����
			{
				//��ʾ��û�д���ϼ�
				PlayAnimate(CTN_ANI_NOCARD,360,660,true);
			}
		}
	}
	viewStation=m_pGameFrame->DeskStation2View(callCard->bNextStation);
	StartTime(viewStation,m_iThinkTime);
}
//��ʾ�÷���
void CNewGameClient::OnHandleShowStyle(void *buffer)
{
	GameServerToClient_StyleCard *showStyle=(GameServerToClient_StyleCard *)buffer;
	if(NULL==showStyle)
	{
		return;
	}
	int viewStation=m_pGameFrame->DeskStation2View(showStyle->bDeskStation);
	for(int i=0;i<12;i++)
	{
		Paste510kPhoto(viewStation,showStyle->bCardList[i]);
	}
	
}
//���һ��̯��
void CNewGameClient::OnHandleLastShowCard(void *buffer)
{
	GameServerToClient_ShowCard *showCard=(GameServerToClient_ShowCard *)buffer;
	if(NULL==showCard)
	{
		return;
	}
	int viewStation=m_pGameFrame->DeskStation2View(showCard->bLastStation);
	
	if(1==viewStation)
	{
		return;
	}

	int iRemainNums=m_Logic.CountUserCard(showCard->bRemainCard,CARD_NUMS);
	
	m_Logic.SortCard(showCard->bRemainCard,NULL,iRemainNums);
	ShowCurrentNoMoveCard(CTN_1014089_NoMoveCard_1004081+viewStation,showCard->bRemainCard,iRemainNums);
}
//�������������������Ϸ����������Ϣ
void CNewGameClient::OnHandleNormalFinish(void *buffer)
{
	GameServerToClient_Finish *normalFinish=(GameServerToClient_Finish *)buffer;
	if(NULL==normalFinish)
	{
		return;
	}
	//ɾ�����ж�ʱ��
	KillAllGameTime();
	for(int i=0;i<PLAY_COUNT;i++)
	{
		SetContainVisible(CTN_1014089_CTN_2000111+i,false);
	}
	SetContainVisible(CTN_1014089_CTN_2000405,false);
	SetStationParameter(GS_WAIT_NEXT);
	//��������
	SetContainVisible(CTN_1014089_CTN_2000293,false);
	SetContainVisible(CTN_1014089_CTN_2000287,true);
	UserInfoForGame userInfo;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pGameFrame->GetUserInfo(normalFinish->iRanking[i],userInfo))
		{
			SetTextShowText(CTN_1014089_CTN_2000287_TEXT_2000389+i,userInfo.szNickName, true);
			SetTextinfo(CTN_1014089_CTN_2000287_TEXT_2000392+i,i+1,true);
			SetTextinfo(CTN_1014089_CTN_2000287_TEXT_2000395+i,normalFinish->i64Point[normalFinish->iRanking[i]],true);
		}
	}
	SetImageVisible(CTN_1014089_IMG_2000308,false);
	SetNoOperateVisible(CTN_1014089_NoMoveCard_2000305,false);
	//��ʾ�����뿪��ť
	SetBtVisible(CTN_1014089_BTN_2000398,true);
	SetBtVisible(CTN_1014089_BTN_2000399,true);
	//ʱ��λ�øı�
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_TEXT_2000292));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_2000112,true,point.x,point.y);
	//�����ȴ���ʼ��ʱ��
	m_bCurrentClockLabel=m_iBeginTime;
	m_iCurrentClockControlID=CTN_1014089_CTN_2000112_Num_2000114;
	SetContainVisible(CTN_1014089_CTN_2000112,true);
	m_pGameFrame->SetTimer(ID_BEGIN_TIME,1000);
	SetInumInfo(m_iCurrentClockControlID,m_bCurrentClockLabel,true);
}
//�������������������Ϸ�й���Ϣ
void CNewGameClient::OnHandleAuto(void *buffer)
{
	GameServerToClient_Auto *userAuto=(GameServerToClient_Auto *)buffer;
	if(NULL==userAuto)
	{
		return;
	}

	int viewStation=m_pGameFrame->DeskStation2View(userAuto->bDeskStation);
	if(userAuto->bIsAuto)
	{
		SetImageVisible(CTN_1014089_IMG_2000401+viewStation,true);
		if(m_iMyDeskStation==userAuto->bDeskStation)
		{
			CString sImagSrc;
			sImagSrc.Format("./resources/image/ȡ���й�.png");
			SetImageLoadSrc(CTN_1014089_CTN_2000405_BTN_2000406,sImagSrc,true);
		}
		if(m_iMyDeskStation==userAuto->bDeskStation)
		{
			m_bIsAuto=true;
		}
		
	}
	else
	{
		SetImageVisible(CTN_1014089_IMG_2000401+viewStation,false);
		if(m_iMyDeskStation==userAuto->bDeskStation)
		{
			CString sImagSrc;
			sImagSrc.Format("./resources/image/�й�.png");
			SetImageLoadSrc(CTN_1014089_CTN_2000405_BTN_2000406,sImagSrc,true);
		}
		if(m_iMyDeskStation==userAuto->bDeskStation)
		{
			m_bIsAuto=false;
		}
		
	}
	
}
//������ͼλ��������Ӧ��ʱ��
void CNewGameClient::StartTime(BYTE bViewStation,int iClockLabel)
{
	if(iClockLabel<=0)
	{
		return;
	}
	m_bCurrentClockLabel=iClockLabel;
	m_iCurrentClockControlID=CTN_1014089_CTN_2000111_Num_2000169+bViewStation;
	m_iCurrentContainControlID=CTN_1014089_CTN_2000111+bViewStation;
	SetContainVisible(m_iCurrentContainControlID,true);
	//������Ϸ˼����ʱ��
	m_pGameFrame->SetTimer(ID_SCORE_TIME,1000);
	SetInumInfo(m_iCurrentClockControlID, __int64(m_bCurrentClockLabel),true);
}

//������ͼλ������Ӧ510kͼƬ
void CNewGameClient::Paste510kPhoto(BYTE bViewStation,BYTE bCardData)
{
	int iControlID1=0,iControlID2=0;
	if(0==bViewStation)
	{
		iControlID1=CTN_1014089_CTN_2000314_IMG_2000317;
		iControlID2=CTN_1014089_CTN_2000314_IMG_2000353;
	}
	else if(1==bViewStation)
	{
		iControlID1=CTN_1014089_CTN_2000315_IMG_2000329;
		iControlID2=CTN_1014089_CTN_2000315_IMG_2000365;
	}
	else if(2==bViewStation)
	{
		iControlID1=CTN_1014089_CTN_2000316_IMG_2000341;
		iControlID2=CTN_1014089_CTN_2000316_IMG_2000377;
	}
	
	if(m_Logic.TestData(bCardData,1))
	{
		if(m_Logic.Is510K(bCardData))
		{
			SetContainVisible(CTN_1014089_CTN_2000314+bViewStation,true);
			
			//�Ƿ����5
			if(m_Logic.IsSpade_5(bCardData))
			{
				
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/5.png");
						SetPicSize(iControlID1+i,sImagSrc,true,0,0,19,16);
						sImagSrc.Format("./resources/image/����.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ����5
			if(m_Logic.IsRed_5(bCardData))
			{
				
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/5.png");
						SetPicSize(iControlID1+i,sImagSrc,true,19,0,19,16);
						sImagSrc.Format("./resources/image/����.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ�÷��5
			if(m_Logic.IsFlower_5(bCardData))
			{
				
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						
						CString sImagSrc;
						sImagSrc.Format("./resources/image/5.png");
						SetPicSize(iControlID1+i,sImagSrc,true,0,0,19,16);
						sImagSrc.Format("./resources/image/÷��.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ񷽿�5
			if(m_Logic.IsBox_5(bCardData))
			{
				
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/5.png");
						SetPicSize(iControlID1+i,sImagSrc,true,19,0,19,16);
						sImagSrc.Format("./resources/image/��Ƭ.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}

			//�Ƿ����10
			if(m_Logic.IsSpade_10(bCardData))
			{
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/10.png");
						SetPicSize(iControlID1+i,sImagSrc,true,0,0,19,16);
						sImagSrc.Format("./resources/image/����.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ����10
			if(m_Logic.IsRed_10(bCardData))
			{
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/10.png");
						SetPicSize(iControlID1+i,sImagSrc,true,19,0,19,16);
						sImagSrc.Format("./resources/image/����.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ�÷��10
			if(m_Logic.IsFlower_10(bCardData))
			{
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/10.png");
						SetPicSize(iControlID1+i,sImagSrc,true,0,0,19,16);
						sImagSrc.Format("./resources/image/÷��.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ񷽿�10
			if(m_Logic.IsBox_10(bCardData))
			{
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/10.png");
						SetPicSize(iControlID1+i,sImagSrc,true,19,0,19,16);
						sImagSrc.Format("./resources/image/��Ƭ.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}

			//�Ƿ����k
			if(m_Logic.IsSpade_k(bCardData))
			{
				OutputDebugString("test::����k");
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/k.png");
						SetPicSize(iControlID1+i,sImagSrc,true,0,0,19,16);
						sImagSrc.Format("./resources/image/����.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ����k
			if(m_Logic.IsRed_k(bCardData))
			{
				OutputDebugString("test::����k");
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/k.png");
						SetPicSize(iControlID1+i,sImagSrc,true,19,0,19,16);
						sImagSrc.Format("./resources/image/����.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ�÷��k
			if(m_Logic.IsFlower_k(bCardData))
			{
				OutputDebugString("test::÷��k");
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/k.png");
						SetPicSize(iControlID1+i,sImagSrc,true,0,0,19,16);
						sImagSrc.Format("./resources/image/÷��.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
			//�Ƿ񷽿�k
			if(m_Logic.IsBox_k(bCardData))
			{
				OutputDebugString("test::����k");
				for(int i=0;i<12;i++)
				{
					if(!GetImageIsVisible(iControlID1+i) && !GetImageIsVisible(iControlID2+i))//��ͼƬ
					{
						CString sImagSrc;
						sImagSrc.Format("./resources/image/k.png");
						SetPicSize(iControlID1+i,sImagSrc,true,19,0,19,16);
						sImagSrc.Format("./resources/image/��Ƭ.png");
						SetImageLoadSrc(iControlID2+i,sImagSrc,true);
						break;
					}
				}
				return;
			}
		}
	}
}
//�鿴�Ʒָ���
void CNewGameClient::UpdateSeeStyle(BYTE bCardData,int iView)
{
	int iFlag=0;
	
	if(m_Logic.IsSpade_5(bCardData))//�Ƿ����5
	{
		iFlag=1;
	}
	else if(m_Logic.IsFlower_5(bCardData))//�Ƿ�÷��5
	{
		iFlag=1;
	}
	else if(m_Logic.IsRed_5(bCardData))//�Ƿ����5
	{
		iFlag=10;
	}
	else if(m_Logic.IsBox_5(bCardData))//�Ƿ񷽿�5
	{
		iFlag=10;
	}
	else if(m_Logic.IsSpade_10(bCardData))//�Ƿ����10
	{
		iFlag=2;
	}
	else if(m_Logic.IsFlower_10(bCardData))//�Ƿ�÷��10
	{
		iFlag=2;
	}
	else if(m_Logic.IsRed_10(bCardData))//�Ƿ����10
	{
		iFlag=20;
	}
	
	else if(m_Logic.IsBox_10(bCardData))//�Ƿ񷽿�10
	{
		iFlag=20;
	}
	else if(m_Logic.IsSpade_k(bCardData))//�Ƿ����k
	{
		iFlag=3;
	}
	else if(m_Logic.IsFlower_k(bCardData))//�Ƿ�÷��k
	{
		iFlag=3;
	}
	else if(m_Logic.IsRed_k(bCardData))//�Ƿ����k
	{
		iFlag=30;
	}
	else if(m_Logic.IsBox_k(bCardData))//�Ƿ񷽿�k
	{
		iFlag=30;
	}
	if(1==iFlag)//��ɫ5
	{
		CString sImagSrc;
		sImagSrc.Format("./resources/image/5.png");
		SetPicSize(CTN_1014089_CTN_2000411_IMG_2000413+iView,sImagSrc,true,0,0,19,16);
	}
	else if(10==iFlag)//��ɫ5
	{
		CString sImagSrc;
		sImagSrc.Format("./resources/image/5.png");
		SetPicSize(CTN_1014089_CTN_2000411_IMG_2000413+iView,sImagSrc,true,19,0,19,16);
	}
	else if(2==iFlag)//��ɫ10
	{
		CString sImagSrc;
		sImagSrc.Format("./resources/image/10.png");
		SetPicSize(CTN_1014089_CTN_2000411_IMG_2000413+iView,sImagSrc,true,0,0,19,16);
	}
	else if(20==iFlag)//��ɫ10
	{
		CString sImagSrc;
		sImagSrc.Format("./resources/image/10.png");
		SetPicSize(CTN_1014089_CTN_2000411_IMG_2000413+iView,sImagSrc,true,19,0,19,16);
	}
	else if(3==iFlag)//��ɫk
	{
		CString sImagSrc;
		sImagSrc.Format("./resources/image/k.png");
		SetPicSize(CTN_1014089_CTN_2000411_IMG_2000413+iView,sImagSrc,true,0,0,19,16);
	}
	else if(30==iFlag)//��ɫk
	{
		CString sImagSrc;
		sImagSrc.Format("./resources/image/k.png");
		SetPicSize(CTN_1014089_CTN_2000411_IMG_2000413+iView,sImagSrc,true,19,0,19,16);
	}
	
}
//����������ʾ��
void CNewGameClient::PlayCardTip(BYTE bDeskStation,BYTE bCardList[],int iCount)
{
	//����
	int iSignalSound[15]={SOUND_HIT_SIGNAL_2,SOUND_HIT_SIGNAL_3,SOUND_HIT_SIGNAL_4,SOUND_HIT_SIGNAL_5,		    
	SOUND_HIT_SIGNAL_6,SOUND_HIT_SIGNAL_7,SOUND_HIT_SIGNAL_8,SOUND_HIT_SIGNAL_9,SOUND_HIT_SIGNAL_10,		      
	SOUND_HIT_SIGNAL_11,SOUND_HIT_SIGNAL_12,SOUND_HIT_SIGNAL_13,SOUND_HIT_SIGNAL_14,SOUND_HIT_SIGNAL_15,SOUND_HIT_SIGNAL_16};		      

	//����
	int iDoubleSound[13]={SOUND_HIT_DOUBLE_2,SOUND_HIT_DOUBLE_3,SOUND_HIT_DOUBLE_4,SOUND_HIT_DOUBLE_5,		      
	SOUND_HIT_DOUBLE_6,SOUND_HIT_DOUBLE_7,SOUND_HIT_DOUBLE_8,SOUND_HIT_DOUBLE_9,SOUND_HIT_DOUBLE_10,SOUND_HIT_DOUBLE_11,	
	SOUND_HIT_DOUBLE_12,SOUND_HIT_DOUBLE_13,SOUND_HIT_DOUBLE_14};		    

	bool bIsBoy=true;
	UserInfoForGame UserInfo ; 
	memset(&UserInfo,0,sizeof(UserInfo));
	if (m_pGameFrame->GetUserInfo(bDeskStation, UserInfo)) 
	{
		bIsBoy=UserInfo.bBoy;
	}
	int iCardStyle=m_Logic.GetCardShape(bCardList,iCount);
	CString cs;
	cs.Format("test::iCardStyle=%d",iCardStyle);
	OutputDebugString(cs);
	switch(iCardStyle)
	{

	case UG_SLAVE_510K://ը��
	case UG_MASTER_510K:
	case UG_BOMB:
		{
			PlayGameSound(SOUND_HIT_BOMB,bIsBoy,false);
			break;
		}
	case UG_STRAIGHT://˳�� 
	case UG_STRAIGHT_FLUSH://ͬ��˳
		{
			PlayGameSound(SOUND_HIT_SHUNZI,bIsBoy,false);
			break;
		}
	case UG_DOUBLE_SEQUENCE://����
		{
			PlayGameSound(SOUND_HIT_LIANDUI,bIsBoy,false);
			break;
		}
	case UG_FOUR_TWO://�Ĵ�����
		{
			PlayGameSound(SOUND_HIT_SIDAIER,bIsBoy,false);
			break;
		}
	case UG_FOUR_TWO_DOUBLE://�Ĵ�����   
		{
			PlayGameSound(SOUND_HIT_SIDAILIANGDUI,bIsBoy,false);
			break;
		}
	case UG_THREE_ONE_SEQUENCE://�ɻ�
	case UG_THREE_DOUBLE_SEQUENCE:
	case UG_THREE_SEQUENCE_DOUBLE_SEQUENCE:
		{
			PlayGameSound(SOUND_HIT_FEIJI,bIsBoy,false);
			break;
		}
	case UG_ONLY_ONE://����
		{
			BYTE bCardNum=m_Logic.GetCardNum(bCardList[0]);
			PlayGameSound(iSignalSound[bCardNum-2],bIsBoy,false);
			break;
		}
	case UG_DOUBLE://����
		{
			BYTE bCardNum=m_Logic.GetCardNum(bCardList[0]);
			PlayGameSound(iDoubleSound[bCardNum-2],bIsBoy,false);
			break;
		}
	case UG_THREE_ONE://����һ
		{
			PlayGameSound(SOUND_HIT_SANDAIYI,bIsBoy,false);
			break;
		}
	case UG_THREE_DOUBLE://����һ��
		{
			PlayGameSound(SOUND_HIT_SANDAIDUI,bIsBoy,false);
			break;
		}
	case UG_THREE://����
		{
			PlayGameSound(SOUND_HIT_SANGE,bIsBoy,false);
			break;
		}
	}
}