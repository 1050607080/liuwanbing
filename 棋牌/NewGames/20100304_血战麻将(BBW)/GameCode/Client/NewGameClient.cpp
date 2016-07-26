#include "stdafx.h"
#include "NewGameClient.h"
#include "IGameFrame.h"
#include "Define.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "..\GameMessage\UpgradeMessage.h"

#include "SoundDefines.h"

#include "LoveSendClassInExe.h"

static BYTE ucChangePos[28] ={ 27,26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
static BYTE ucChangePos2[28] ={ 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	(( PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID)) )!=NULL)

//���ֽڱ���ת���ɿ��ֽڱ���
void char2Wchar(char* pchar,wchar_t* wchar)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0);
	MultiByteToWideChar(CP_ACP,0,pchar,-1,wchar,nwLen);
}

//���ֽڱ���ת���ɶ��ֽڱ���
void wchar2char(wchar_t* wchar,char* pchar)
{
	int len = WideCharToMultiByte(CP_ACP,0,wchar,(int)wcslen(wchar),NULL,0,NULL,NULL);   
	WideCharToMultiByte(CP_ACP,0,wchar,(int)wcslen(wchar),pchar,len,NULL,NULL);
}

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

CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	//��Ϸ���
    m_pGameFrame = pGameFrame;
	//������������
	m_pUIBkMusic = createIrrKlangDevice();
	//��Ч����
	m_pUIGameSound = createIrrKlangDevice();
	//��Ϸ����������Ч����
	m_bBgMusic = true;
	//��Ϸ��Ч����
	m_bGameSound = true;
	//��ϷUI
    m_pUI = pUI;
	//��Ϸ״̬
 	m_bGameStation = GS_WAIT_SETGAME;
	m_byShowFaPai = 255;//��ǰ����״̬
	GetSoundMapFromCfg();//���������ļ�
}

CNewGameClient::~CNewGameClient()
{
	//�رձ�����������
	if(m_pUIBkMusic != NULL)
	{
		m_pUIBkMusic->drop();
	}
	//�ر���Ϸ��Ч����
	if(m_pUIGameSound != NULL)
	{
		m_pUIGameSound->drop();
	}
}

/*-----------------------------------------------------------------------------------------------------------
	һϵ��ƽ̨�ṩ����Ҫ������������Ϸ�ĺ���ʵ�֣������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
  ----------------------------------------------------------------------------------------------------------*/
//��ʼ��
int CNewGameClient::Initial()
{
	//����UI
	wchar_t wszUIFile[MAX_PATH];
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
	m_pUI->LoadData(wszUIFile);
	LoadGameConfig();//������Ϸ����
    return 0;
}

//��ʼ��UI
int CNewGameClient::InitUI()
{		
	//InitGameUI();

	for(int i=0;i<PLAY_COUNT;++i)
	{
		SAFE_CTRL_OPERATE(IText, CTN_1_TEXT_70 + i, SetControlVisible(false));  //�����ǳ�
		SAFE_CTRL_OPERATE(IText, CTN_1_TEXT_102 + i, SetControlVisible(false));  //���ؽ��
	}

	m_nDeskStation=m_pGameFrame->GetMyDeskStation();
    // ��ʾ��ʼ��Ť �����У��Ϲܣ��Ȱ�Ť
    SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10001, SetControlVisible(false));  

	//Ϊ�˱�����ʾ���öԻ���ĸ����㣬����ʾ���������öԻ���
	SAFE_CTRL_OPERATE(IContain, 401, SetControlVisible(true));	// 
	SAFE_CTRL_OPERATE(IContain, 401, SetControlVisible(false));	// 
    // ��������
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_101018, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_101019, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_101021, SetControlVisible(false));    
    // �����Ƴ�
    SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_1_MjWallCtrl_101026, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_1_MjWallCtrl_101027, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_1_MjWallCtrl_101028, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_1_MjWallCtrl_101029, SetControlVisible(false));    
    // ���س���
    SAFE_CTRL_OPERATE(IMjGiveCtrl, CTN_1_MjGiveCtrl_101022, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjGiveCtrl, CTN_1_MjGiveCtrl_101023, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjGiveCtrl, CTN_1_MjGiveCtrl_101024, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjGiveCtrl, CTN_1_MjGiveCtrl_101025, SetControlVisible(false));  

	SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjGangCtrl_600011, SetIsUseMouse(false));
	SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjGangCtrl_600012, SetIsUseMouse(false));
    // ����С��
    IImage *pHandImg = NULL;
    GETCTRL(IImage, pHandImg, m_pUI, IMG_FANJIN_HAND);
    if (NULL!=pHandImg)
    {
        pHandImg->SetControlVisible(false);
    }
    // ����ʱ��
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600031, SetControlVisible(false));    
    // �������ư�ť
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));    
    // ����ׯ�ұ�־
    UIShowZhuang(0xFF, false);
    // ���س�����ʾ
    UIShowLastTileTip(4, false);
    // ����׼��
    UIShowReady(0xFF, false);
    // �����й�
    UIShowTrustee( false);
    // �ȴ�����
    UIShowWaiting(false);

    // �齫�ؼ�
    IContain *pCtnAllMj = NULL;
	GETCTRL(IContain, pCtnAllMj, m_pUI, CTN_ALL_MJCTRL);
    if (pCtnAllMj != NULL)
    {
        pCtnAllMj->SetControlVisible(true);
    }

	//���ظ��ƿؼ�
    UIShowGangCtrl(false);
	//���سԸܿؼ�
	UIShowChiGangCtrl(false,false);

//	//����Ƿ��Ʒ����汾����ʾ���ư�ť
#ifdef MJ_CAN_SETPAI
    SAFE_CTRL_OPERATE(IButton, CTN_BTN_FAPAI, SetControlVisible(true));//��ʾ��������ť
	SAFE_CTRL_OPERATE(IButton, CTN_BTN_FAPAI, SetEnable(true));	// 
#endif
//	////////////////////////
	ShowGFXY(false);
    return 0;
}


//��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{

	if(m_nDeskStation == 0)
	{
		char buff[500];
		sprintf(buff,"�ͻ��ˣ��յ���Ϣ��%d",nSubId);
		OutputDebugString(buff);
	}

    switch(nSubId)
    {
	case ASS_GM_GAME_STATION:           ///<����״̬/// ����������Ϣ
		{			    
			SetGameStation(buffer,nLen);
		}break;
	case ASS_AUTOCARD:				//����й�֪ͨ/// �й���Ϣ
		{
			OnUserTuoguan( buffer,  nLen);
		}break;
	case ASS_GM_AGREE_GAME:
		{
			OnUserAgree( buffer,  nLen);           /// ͬ����Ϣ    
		}break;
	case THING_BEGIN:
		{
			OnGameStart( buffer,  nLen);                 /// ��Ϸ��ʼ��Ϣ  ---
		}break;
	case THING_DONG_NT:
		{
			OnDongNt( buffer,  nLen);					/// ��0λ���Ϊׯ����Ϣ
		}break;
	case THING_2SEZI_DIR_GP:
		{
			OnSeziSureOpenPoint( buffer,  nLen);  /// �����Ŷ�����
		}break;
	case THING_SEND_PAI:
		{
			OnSendAllCard( buffer,  nLen);            /// ����
		}break;
	case THING_BEGIN_OUT:
		{
			OnGamePlay( buffer,  nLen);              /// ��ʼ���ƣ����ƣ�
		}break;
	case THING_OUT_PAI:
		{
			OnNotifyOutCard( buffer,  nLen);          /// ����
		}break;
	case THING_ZHUA_PAI:
		{
			OnNotifyZhuaPai( buffer,  nLen);          /// ץ��
		}break;
	case THING_CPG_NOTIFY:
		{
			OnNotifyBlock( buffer,  nLen);         /// ����
		}break;
	case THING_CHI_PAI:
		{
			OnNotifyChiPai( buffer,  nLen);            /// ����
		}break;
	case THING_PENG_PAI:
		{
			OnNotifyPengPai( buffer,  nLen);          /// ����
		}break;
	case THING_GANG_PAI:
		{
			OnNotifyGangPai( buffer,  nLen);          /// ����
		}break;
	case THING_HU_PAI:
		{
			OnNotifyHuPai( buffer,  nLen);             /// ��
		}break;
	case THING_DING_QUE:
		{
			OnNotifyDingQue( buffer,  nLen);         /// ��ȱ�¼�
		}break;
    case ASS_SUPER:
		{
			OnNotifySuperSetPai( buffer,  nLen);		/// ����������
		}break;
    case THING_ENG_HANDLE:
		{
			OnNotifyFinish( buffer,  nLen);         /// ��֣���Ϸ����
		}break;
	case THING_ENG_UN_NORMAL:
		{
			OnForceFinish( buffer,  nLen);         /// ǿ�˽�����Ϸ
		}break;
    default:
        break;
    }
    return 0;
}



//������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{

	if(pMessage->_uMessageType==UI_MJCTRL_MOVEONMJ) // �齫�ؼ���Ϣ���ڽ�ֹʹ���������£��Ƶ����Ͼͷ���Ϣ
    {
        OnMoveOnMjCtrl(pMessage);
    }
    else if (pMessage->_uMessageType==UI_MJCTRL_LBUTTON)
    {
        OnLButtonOnMjCtrl(pMessage);
    }
	else if(pMessage->_uMessageType==UI_KEYUP)
	{
		if(pMessage->_uControlID == CTN_EDIT_FAPAI_NEW || pMessage->_uControlID == CTN_EDIT_FAPAI_OLD)//��ʾҪ���õ���
		{
			ShowSuperSetPai(true);
		}
	}
	else if(pMessage->_uMessageType==UI_LBUTTONDOWN) // ����
	{
		switch (pMessage->_uControlID)
		{
		case CTN_1_BTN_10002: //�йܰ�ť
		case CTN_1_BTN_10003://ȡ���й�
			{
				 //   //// �������ƶ���
					//PlayAni(Ani_Peng_0 + 2);
							//PlayTileSound(0,1);
				RequestTuoGuan((pMessage->_uControlID-CTN_1_BTN_10002+1)%2);
			}break;
        case CTN_1_MjHandCtrl_600010:       // �Լ�����
            {
				if(!m_UserData.m_bTuoGuan[m_nDeskStation])//�ֶ����ƣ����ּ��㳬ʱ����
				{
					m_nTimeOutCnt = 0;
				}
                // �������                
                OnUIRequestOutCard(pMessage->_uControlID);              
            }
            break;
        case CTN_1_CTN_600000_BTN_600001://��
            {
                // ��������
                OnRequestBlock(pMessage->_uControlID);
            }
            break;
        case CTN_1_CTN_600000_BTN_600002://��
            {
                // ��������
                OnRequestBlock(pMessage->_uControlID);
            }
            break;
        case CTN_1_CTN_600000_BTN_600003://��
            {
                // ��������
                OnRequestBlock(pMessage->_uControlID);
            }
            break;
        case CTN_1_CTN_600000_BTN_600004:///<��
            {
                // ��������
                OnRequestBlock(pMessage->_uControlID);
            }
            break;
        case CTN_1_CTN_600000_BTN_600005:///<����
            {
                // ��������
                OnRequestBlock(pMessage->_uControlID);
            }   
            break;
        case CTN_1_CTN_600000_BTN_600006:///<����
            {
                // ��������
                OnRequestBlock(pMessage->_uControlID);
            }   
            break;
		case CTN_1_BTN_10001://��ʼ��Ϸ
		case CTN_1_CTN_20500_BTN_20502://����������ť
			{
				InitGameDate();
				ReStartGameInitUI();
				//��ʼ��Ϸ
				OnKillAllTimer();
				m_pGameFrame->ClickContinue();
			}break;
		case CTN_1_CTN_20500_BTN_20501://������뿪��ť
			{
				OnKillAllTimer();
				m_pGameFrame->CloseClient();
			}break;
		case HUPAI_GUAN_BI://�رպ��ƴ���
			{
				SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));	
			}
			break;
		case GFXY_GUAN_BI://�رչη����괰��
			{
				SAFE_CTRL_OPERATE(IContain, CTN_GFXY_DLG, SetControlVisible(false));
			}
			break;
		case DING_WAN:	//<ȱ��
		case DING_SUO:	//<ȱ��
		case DING_BING: //<ȱͬ
			{
				OnKillAllTimer();
				SAFE_CTRL_OPERATE(IContain, CTN_DING_QUE, SetControlVisible(false));	
				tagDingQueEx que;
				que.Clear();
				que.byQue = pMessage->_uControlID - DING_WAN;
				que.byUser = m_nDeskStation;     
				SendGameDataEx(THING_DING_QUE, &que, sizeof(tagDingQueEx));
			}
			break;
		case CTN_BTN_FAPAI_QUEDING://ȷ������������
			{
				SendSuperData();//��ȡ�ؼ�����,��������������
			}
			break;
		case CTN_BTN_FAPAI_QUXIAO://ȡ��,���ط������Ի���
			{
                SAFE_CTRL_OPERATE(IContain, CTN_FAPAI_CTN, SetControlVisible(false));
			}
			break;
		case BN_VOLUM_UP://����+
			{
			}
			break;
		case BN_VOLUM_DOWN://����-
			{
			}
			break;
		case CTN_1000000_BTN_1003500: //������Ϸ
			{
				//UI��ʾ
				OnClickedConfig();
			}
			break;
		case BN_CLOSE_FRAME://�رտͻ��˰�ť
			{
				//���ڰ汾ƽ̨�Ĺرհ�ťû�е���OnGameQuiting������ڴ��ֹ�����һ��
				OnGameQuiting();
			}
			break;
		case CTN_1000000_CTN_1003510_BTN_1003540: //������Ϸȷ��
			{
				OnClickedConfigOK();//UI��ʾ
				SaveGameConfig();//��������
			}
			break;
		case CTN_1000000_CTN_1003510_BTN_1003550: //������Ϸȡ��
			{
				OnClickedConfigCancel();//UI��ʾ
			}
			break;
		case CTN_1000000_CTN_1000600_BTN_1000603: //ȷ��ǿ����Ϸ
			{
				m_pGameFrame->CloseClient();//�رտͻ���
			}
			break;
		case CTN_1000000_CTN_1000600_BTN_1000604: //ȡ��ǿ����Ϸ
			{
				SAFE_CTRL_OPERATE(IContain, CTN_1000000_CTN_1000600, SetControlVisible(false));
			}
			break;
        default:
            break;
        }
    }
    else if (pMessage->_uMessageType==UI_CATOONFINISH)//��������
    {
        OnCartoonFinish(pMessage);
        if (ZHUAPAI_ACT_MVACT==pMessage->_uControlID && m_GameData.m_byThingDoing < THING_HU_PAI)
        {
            // �������ץ�ƶ���
            UISetHandCard(m_nZhuaViewStaiton, m_UserDataCard[m_nZhuaViewStaiton].m_nUserHandInfo, m_UserDataCard[m_nZhuaViewStaiton].m_nHandCardCount, true);        
            IMjWallCtrl *pWall = NULL;
			GETCTRL(IMjWallCtrl, pWall, m_pUI, ZHUAPAI_ACT_MJWALL);
            if ( pWall != NULL )
            {             
                pWall->SetControlVisible(false);
            }
        }
    }
    return 0;
}
int CNewGameClient::ResetGameFrame(void)
{
	//InitGameUI();
    return 0;
}

//��ҽ���
int CNewGameClient::GameUserCome(void)
{
    return 0;
}
//����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	BYTE nView = m_pGameFrame->DeskStation2View(nIndex);
    return 0;
}

//��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
    return 0;
}
//�Ƿ�������Ϸ������Ϸʵ��
//�Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
 	if(m_bGameStation >= 20 && m_bGameStation < 23)
 		return true;
	return false;
}

//��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
    case TIME_ZHUAPAI://ץ��
        {
            m_pGameFrame->KillTimer(TIME_ZHUAPAI);
            // ����������ʾ    
            for (int i=0; i<PLAY_COUNT; ++i)
            {
				if(m_UserData.m_bIsHu[i])//������Ҳ�����
					continue;
                int desk = m_pGameFrame->DeskStation2View(i);
				m_UserDataCard[desk].GetFromByte( m_UserData.m_byArHandPai[i] );   
				if(i != m_nDeskStation)
				{
					m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
				}
                // �������ץ�ƶ�������������Ϊ��һ��
                UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, i==m_ucCurrentPlayer);
            }
			//ץ�ƶ���
			POINT pt,ptDst;   
            UIShowZhuaMoveAction(pt, ptDst, 0xFF, false);
        }
        break;
    case TIME_FAPAI:
        {
            m_pGameFrame->KillTimer(nTimerID);
        }
        break;
    case TIME_BEGIN:
        {
			OnKillAllTimer();
            if (m_MJSetting.bAutoBegin)
            {
                m_pGameFrame->ClickContinue();
            }
            else
            {
                m_pGameFrame->CloseClient();
            }
        }
        break;
	case TIME_OUT_CARD: // ����
		{
			OnKillAllTimer();
			// �������ƿ�
			SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
			if(m_nDeskStation != m_UserData.m_byNowOutStation)
				break;
            OnUIRequestOutCard(CTN_1_MjHandCtrl_600010,true);//����ʱ�䵽��
			if (!m_bIsTuoGuan[m_nDeskStation])
            {
                // ��¼��ʱ����
                m_nTimeOutCnt ++;
                if (m_nTimeOutCnt>=3)
                {
                    RequestTuoGuan(true);
                }
                else
                {
                    //std::wostringstream oss;
                    //oss<<L"ע�⣬���Ѿ���ʱ"<<m_nTimeOutCnt<<L"�Σ���ʱ3�ν��Զ��й�"<<std::flush;
                    //m_pGameFrame->InsertSystemMessage((wchar_t *)oss.str().c_str());
                }
            }
			break;
		}
    case TIME_BLOCK_CARD: // ����
        {
			OnKillAllTimer();
			UIShowGangCtrl(false);
			//�ָ����е�����ɫ
			if(m_UserData.m_byNowOutStation == m_nDeskStation)//�Լ�����
			{			
				if((m_UserData.m_bTuoGuan[m_nDeskStation] || m_UserData.m_bTing[m_nDeskStation]) && m_GameData.T_CPGNotify[m_nDeskStation].bHu)//�����й�״̬��b�����ܺ��ͺ���
				{
					RequestBlockHu();//���ͺ�������
				}
				else if(m_UserData.m_bTing[m_nDeskStation] && (m_GameData.T_CPGNotify[m_nDeskStation].bGang || m_GameData.T_CPGNotify[m_nDeskStation].bBigGang))//�������ܸ�
				{
					RequestBlockGang();//���͸�����
				}
				else 
				{	
					RequestCancelBlock();//ȡ��
					OnSetTimer(m_UserData.m_byNowOutStation,TIME_OUT_CARD);
				}
			}
			else//�����˵���
			{           
				if((m_UserData.m_bTuoGuan[m_nDeskStation] || m_UserData.m_bTing[m_nDeskStation]) && m_GameData.T_CPGNotify[m_nDeskStation].bHu)//�����й�״̬��b�����ܺ��ͺ���
				{
					RequestBlockHu();//���ͺ�������
				}
				else if(m_UserData.m_bTing[m_nDeskStation] && (m_GameData.T_CPGNotify[m_nDeskStation].bGang || m_GameData.T_CPGNotify[m_nDeskStation].bBigGang))//�������ܸ�
				{
					RequestBlockGang();//���͸�����
				}
				else
				{
					RequestCancelBlock();
				}
			}
            break;
        }
    case TIME_HIDE_SEZI_ANI:    // ��������
        {           
			UIShowsaizi(0xFF,0,0);
            m_pGameFrame->KillTimer(TIME_HIDE_SEZI_ANI);
        }
		break;
	case TIME_FENG_YU://���عη�����
		{
            m_pGameFrame->KillTimer(TIME_FENG_YU);
			ShowGFXY(false);
		}
        break;
	case TIME_SHOW_HU:// ��ʾ���ƶԻ���ʱ��
		{
            m_pGameFrame->KillTimer(TIME_SHOW_HU);
			SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));//���غ��ƶԻ���
		}
		break;
	case TIME_DING_QUE://��ȱ�ż�ʱ��
		{
			srand(m_nDeskStation);
			OnKillAllTimer();
            KillGameTimer(TIME_DING_QUE);
			//����һ�������ȱ��Ϣ
			SAFE_CTRL_OPERATE(IContain, CTN_DING_QUE, SetControlVisible(false));	
			tagDingQueEx que;
			que.Clear();
			que.byQue = 255/*rand()%3*/;
			que.byUser = m_nDeskStation;     
			SendGameDataEx(THING_DING_QUE, &que, sizeof(tagDingQueEx)); 
		}
		break;
	case TIME_TALKING_0://�����ʱ
	case TIME_TALKING_1:
	case TIME_TALKING_2:
	case TIME_TALKING_3:
		{
            m_pGameFrame->KillTimer(nTimerID);
			this->ShowTalkFinish(nTimerID - TIME_TALKING_0);
		}
		break;
	}
}

//��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_bGameStation = bGameStation;
}

//������Ϸ״̬����Ҫ�Ƕ����������������Ϣ
void CNewGameClient::SetGameStation(void* pBuffer,int nLen)
{
	UIShowWaiting(false);
	ReStartGameInitUI();
                
	int iMyDesk = m_pGameFrame->GetMyDeskStation();
	GameStationData *pGameStation = (GameStationData *) pBuffer;
	SetStationParameter(pGameStation->Stationpara);//������Ϸ״̬

    GameStationDataEx *pHead = (GameStationDataEx *)pBuffer;
	m_nDeskStation = m_pGameFrame->GetMyDeskStation();

    m_MJSetting.ucDingQueTime = pHead->ucDingQueTime;
    m_MJSetting.ucBlockTime		= pHead->byThinkTime;
    m_MJSetting.ucOutcardTime	= pHead->byOutTime;
    m_MJSetting.ucBeginTime		= pHead->byBeginTime;
    m_MJSetting.ucNextTime		= pHead->byBeginTime; 

    m_MJSetting.bAutoBegin		= pHead->bAutoBegin;
    m_MJSetting.nAutoTime		= pHead->byAutoOutTime;  

	m_GameData.m_byFengDing		= pHead->m_byFengDing;	//�ⶥ����
	m_GameData.m_bShowTax		= pHead->m_bShowTax;	//�Ƿ���ʾ̨��
	m_GameData.m_iBaseFen		= pHead->m_iBaseFen;	//��Ϸ������
	m_GameData.m_bChengBao		= pHead->m_bChengBao;	//�ֳа��ƺͼҼ���

	///��ʾ�ⶥ
	UIShowFengDing();
    SAFE_CTRL_OPERATE(IButton, 10001, SetControlVisible(true));

	PlayBkMusic(true);//���ű�������

	char buff[500];
	sprintf(buff,"CNewGameClient��OnGetGameStation ״̬%d",pHead->Stationpara);
	OutputDebugString(buff);

 //   tagCountFenEx data;   
	//for(int i=0;i<PLAY_COUNT;++i)
	//{
	//	for(int j=0;j<13;++j)
	//		data.m_byArHandPai[i][j] = i%8+1;
	//}
	//OnNotifyFinish(&data,sizeof(tagCountFenEx));
	//return;

	switch(pHead->Stationpara)
    {
        case GS_SEND_CARD:		//����״̬(��δ����,�����Ѿ����ƣ�����û�г���)
            {
                GameStationDataEx *pGameStation = (GameStationDataEx *)pBuffer;
                ReStartGameInitUI();
                SAFE_CTRL_OPERATE(IButton, 10001, SetControlVisible(false));
                //// ��ʾ��ǽ
                UIShowWall(0xFF, true);
				//��ʾ����
                for(int i=0;i<PLAY_COUNT;i++)
                {
					m_UserData.m_byDingQue[i] = pGameStation->m_byDingQue[i];
                    int desk =m_pGameFrame->DeskStation2View(i);
                    // ����
                    m_UserData.SetHandPaiData(i,pGameStation->m_byArHandPai[i]);
                    m_UserData.SortHandPai(i,false);
                    m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
					if(i != m_nDeskStation)
					{
						m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
					}
                    UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, i==m_ucCurrentPlayer);
                    // ����
                    m_UserData.SetMenPaiData(i,pGameStation->m_iArMenPai[i]);
                    UIShowWall(i, m_UserData.m_iArMenPai[i]);
                }
                /// ��ʾׯ��
                UIShowZhuang(m_pGameFrame->DeskStation2View(pGameStation->byNtStation), true); 
				// �ȴ�����
				UIShowWaiting(false);
				////��ʾȱ��
				UIShowQueMen(true);
				if(pHead->m_byThingDoing == THING_DING_QUE)//��ȱ�¼�
				{
					if(pHead->m_byDingQue[m_nDeskStation] == 255)//û�ж�ȱ����ʾ��ȱ�Ի���
					{
						OnSetTimer(m_nDeskStation,TIME_DING_QUE);//������ȱ��ʱ��
						SAFE_CTRL_OPERATE(IContain, CTN_DING_QUE, SetControlVisible(true));//��ʾ��ȱ�Ի���
					}
				}
            }
            break;        
        case GS_PLAY_GAME:	//��Ϸ��״̬
            {
                GameStationDataEx	*pGameStation = (GameStationDataEx *)pBuffer;
                ReStartGameInitUI();
                //// ��ʾ��ǽ
                UIShowWall(0xFF, true);
                SAFE_CTRL_OPERATE(IButton, 10001, SetControlVisible(false));

                m_byNtLogicPos = pGameStation->byNtStation;
                // ��ʾׯ��
                UIShowZhuang(m_pGameFrame->DeskStation2View(m_byNtLogicPos), true); 
				// 
                m_ucCurrentPlayer = pGameStation->m_byNowOutStation;    
                for(int i=0;i<PLAY_COUNT;i++)
                {
					m_UserData.m_byDingQue[i] = pGameStation->m_byDingQue[i];//

                    int desk =m_pGameFrame->DeskStation2View(i);
                    // ��������
                    for (int j=0; j<5; ++j)
                    {
                        if (255==pGameStation->m_UserGCPData[i][j].iStation)
                        {
                            continue;
                        }
                        m_UserData.AddToGCP(i,&pGameStation->m_UserGCPData[i][j]);
                    }      
                    m_UserDataCard[desk].GetFromCPGData(m_UserData.m_UserGCPData[i],i==m_nDeskStation);
                    // ����
                    m_UserData.SetHandPaiData(i,pGameStation->m_byArHandPai[i]);
                    m_UserData.SortHandPai(i,false);

					if(i == m_nDeskStation)//�Լ�
					{
						m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
					}
					else
					{
						m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
					}

                    UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, i==m_ucCurrentPlayer);
                    // ����
                    m_UserData.SetMenPaiData(i,pGameStation->m_iArMenPai[i]);
                    UIShowWall(i, m_UserData.m_iArMenPai[i]);
                    // ����
                    m_UserData.SetOutPaiData(i,pGameStation->m_byArOutPai[i]);
                    m_UserDataCard[desk].GetOutFromByte( pGameStation->m_byArOutPai[i] );
                    UISetOutCard(desk, m_UserDataCard[desk].m_nUserOutInfo, m_UserDataCard[desk].m_nOutCardCount);
                }
				OnSetTimer(pGameStation->m_byNowOutStation,TIME_OUT_CARD);

                SAFE_CTRL_OPERATE(IButton, 10001, SetControlVisible(false));
				// �ȴ�����
				UIShowWaiting(false);
				//��ʾȱ��
				UIShowQueMen(true);
                for(int i=0;i<PLAY_COUNT;i++)
                {
					if(!pGameStation->m_bIsHu[i])
						continue;
					// �ؼ�ID
					static int nCtrl_1[PLAY_COUNT] = {CTN_1_MjHandCtrl_101018 , CTN_1_MjHandCtrl_101019 , CTN_1_MjHandCtrl_600010 ,CTN_1_MjHandCtrl_101021 };
					int desk =m_pGameFrame->DeskStation2View(i);
					if(i == m_nDeskStation)
					{
						m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
					}
					else
					{
						m_UserDataCard[desk].FillWithBack(m_UserData.GetGCPCount(i));
						m_UserDataCard[desk].m_nUserHandInfo[m_UserData.GetGCPCount(i)-1] = m_UserData.GetLastHandPai(i);
					}
					UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount);
					SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl_1[desk], SetIsLaydown(true));
					SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl_1[desk], SetGiveTileMode(i==m_UserData.m_byNowOutStation));
				}
            }
            break;
        case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
            {
				OnSetTimer(m_nDeskStation,TIME_BEGIN);
				////����ȱ��
				UIShowQueMen(false);
                GameStationDataEx *pGameStation = (GameStationDataEx *)pBuffer;
                for (int i=0; i<PLAY_COUNT; ++i)
                {
                    if ((( (pGameStation->ucAgree>>i) )&0x01)==0x01 )
                    {
                        UIShowReady(m_pGameFrame->DeskStation2View(i), true);
                    }
                    else
                    {
                        UIShowReady(m_pGameFrame->DeskStation2View(i), false);
                    }
                }
            }
            break;
        case GS_WAIT_ARGEE:		
            {
				OnSetTimer(m_nDeskStation,TIME_BEGIN);
                GameStationDataEx *pGameStation = (GameStationDataEx *)pBuffer;
                for (int i=0; i<PLAY_COUNT; ++i)
                {
                    if ( (( (pGameStation->ucAgree>>i) )&0x01)==0x01 )
                    {
                        UIShowReady(m_pGameFrame->DeskStation2View(i), true);
                    }
                    else
                    {
                        UIShowReady(m_pGameFrame->DeskStation2View(i), false);
                    }
                }
				////����ȱ��
				UIShowQueMen(false);
            }
            break;
        case GS_WAIT_SETGAME:
            {
				OnSetTimer(m_nDeskStation,TIME_BEGIN);
                GameStationDataEx *pGameStation = (GameStationDataEx *)pBuffer;
                for (int i=0; i<PLAY_COUNT; ++i)
                {
                    if ( (( (pGameStation->ucAgree>>i) )&0x01)==0x01 )
                    {
                        UIShowReady(m_pGameFrame->DeskStation2View(i), true);
                    }
                    else
                    {
                        UIShowReady(m_pGameFrame->DeskStation2View(i), false);
                    }
                }
				////����ȱ��
				UIShowQueMen(false);
            }
            break;
        default:
            break;
    }

    //tagCountFenEx data;   
    //UIShowResultDlg(&data,NULL,1);


}

///����Ƴ�
bool CNewGameClient::OnGameQuiting()
{
	BYTE byCurrStation = GetStationParameter();

	if(m_bGameStation >= 20 && m_bGameStation <= 22)
	{
		if (!m_pGameFrame->GetIsWatching())
		{
			//��ʾ��ʾ��
			IText* pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(CTN_1000000_CTN_1000600_TEXT_1000602));
			pText->SetText(L"�����˳������йܴ����Ƿ��˳���");
			// ��ʾ�����
			SAFE_CTRL_OPERATE(IContain, CTN_1000000_CTN_1000600, SetControlVisible(true));
			return false;
		}
	}	
	return true;
}
BYTE CNewGameClient::GetStationParameter()
{
    return m_bGameStation;
}
///���¿���ʱ�ĳ�ʼ��
void CNewGameClient::ReStartGameInitUI()
{
//��ʼ���ؼ�
	SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10002, SetControlVisible(true));//��ʾ�й�
    SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10003, SetControlVisible(false));//����ȡ���й�
    // ���ؿ�ʼ��ť
    SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10001, SetControlVisible(false));
	//��ȡ����������ǳ�
	GetUserName();
	InitGameDate();
    // ��ʾ��ʼ��Ť �����У��Ϲܣ��Ȱ�Ť
    // ��������
    for (int i=0; i<PLAY_COUNT; ++i)
    {

        UISetHandCard(i, 0, 0, false);
        UISetOutCard(i, 0, 0, false);        
    }

    // �����Ƴ�
    SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_1_MjWallCtrl_101026, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_1_MjWallCtrl_101027, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_1_MjWallCtrl_101028, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_1_MjWallCtrl_101029, SetControlVisible(false));   
    // ��������
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_101018, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_101019, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetControlVisible(false));    
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_101021, SetControlVisible(false));   
    // ����ʱ��
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600031, SetControlVisible(false));  
    // �������ư�ť
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));  
    // ����ׯ�ұ�־
    UIShowZhuang(0xFF, false);
    // ��ʾ�����
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_20500, SetControlVisible(false));
	SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));///���غ��ƶԻ���
    // ��������
    UIShowsaizi(0xFF,0,0);
    // ���س��Ʊ�ʶ
    UIShowLastTileTip(4,false);
    // �����й�
    UIShowTrustee(false);
	//���ظ��ƿؼ�
    UIShowGangCtrl(false);
	////����ȱ��
	UIShowQueMen(false);

	ShowGFXY(false);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �û��й�
void CNewGameClient::OnUserTuoguan(void *buffer, int nLen)
{
    Usertouguan *pCmd = (Usertouguan *)buffer;
    m_bIsTuoGuan[pCmd->desk] = pCmd->touguang;

	for(int i=0;i<4;++i)
	{
		this->m_UserData.m_bTuoGuan[i] = pCmd->bTuoGuan[i];//��¼�й�״̬
	}

    if (pCmd->desk==m_nDeskStation)
    {
        if (pCmd->touguang)
        {
            SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10002, SetControlVisible(false));
            SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10003, SetControlVisible(true));
            SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));

			if(m_nDeskStation == m_UserData.m_byNowOutStation)//������Լ����ƾ��Զ�����
			{
				OnUIRequestOutCard(CTN_1_MjHandCtrl_600010,true);//�Զ�����
			}
			else
			{
				RequestCancelBlock();//��������
			}
        }    
        else
        {
			m_nTimeOutCnt = 0;
            SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10002, SetControlVisible(true));
            SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10003, SetControlVisible(false));
        }
    }
    else
    {
    }
    UIShowTrustee(true); 
}
// �û�ͬ��
void CNewGameClient::OnUserAgree(void *buffer, int len)
{
	///��Ϸ�в�����ͬ����Ϣ
	if(GetStationParameter() == GS_PLAY_GAME || GetStationParameter() == GS_SEND_CARD)
	{
		return; 
	}
    if(len!=sizeof(MSG_GR_R_UserAgree))
    {
        return ;
    }
    MSG_GR_R_UserAgree * pUser=(MSG_GR_R_UserAgree *)buffer;
    if(pUser->bDeskStation==m_nDeskStation)         // ����
    {
        PlayNormalSound(SOUND_READY);
		OnKillAllTimer();
        ReStartGameInitUI();
        ///��ʼ��ť
        IButton * pButton=NULL;
        GETCTRL(IButton,pButton,m_pUI,CTN_1_BTN_10001);
        if(pButton!=NULL)
        {
            pButton->SetControlVisible(false);
        }
        IImage * pIMage=NULL;
        //		GETCTRL(IImage,pIMage,m_pUI,CTN_1_IMG_20001);	//�ȴ���ͼƬ
        if(pIMage!=NULL)
        {
            pIMage->SetControlVisible(true);
        }                
        // �ȴ�����
        UIShowWaiting(true);
    }
    else
    {
        PlayNormalSound(SOUND_READY);
    }

    UIShowReady(m_pGameFrame->DeskStation2View(pUser->bDeskStation), true);
}
// ��Ϸ��ʼ
void CNewGameClient::OnGameStart(void *buffer, int len)
{
	SetStationParameter(GS_SEND_CARD);
	// �����
	SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL,0));
	SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
    m_ucHandState = 0xFF;//����״̬
	InitGameDate();
	m_GameData.InitData();
	m_UserData.InitData();
	ReStartGameInitUI();//���ý���

    ///��ʼ��ť
    IButton * pButton=NULL;
    GETCTRL(IButton,pButton,m_pUI,CTN_1_BTN_10001);
    if(pButton!=NULL)
    {
        pButton->SetControlVisible(false);
    }
    // �ȴ�����
    UIShowWaiting(false);
    tagBeginEx *pCmd = (tagBeginEx*)buffer;
    //// ��ʾ��ǽ
    UIShowWall(0xFF, true);  
    /// ׼����־
    UIShowReady(0xFF, false);

	m_UserData.m_byNtStation = pCmd->byNt;//ׯ��λ��
	for(int i=0;i<4;++i)
	{
		m_UserData.SetMenPaiData(i,pCmd->byMenPai[i]);//����
	}
	m_GameData.m_byThingDoing = THING_BEGIN;//���ڷ������¼�
	m_GameData.T_Begin.Clear();
	m_GameData.T_Begin.byNt = pCmd->byNt;//ׯ��λ��

	this->OnKillAllTimer();
    KillGameTimer(TIME_OUT_CARD);
    m_pGameFrame->KillTimer(TIME_BLOCK_CARD);
    m_pGameFrame->KillTimer(TIME_HIDE_SEZI_ANI);
    m_pGameFrame->KillTimer(TIME_BEGIN);
    m_pGameFrame->KillTimer(TIME_FAPAI);

	m_bStopSendPai = false;//�����Ƿ����
    // �ؼ�ID
    static int nCtrl[PLAY_COUNT] = {CTN_1_MjHandCtrl_101018 , CTN_1_MjHandCtrl_101019 , CTN_1_MjHandCtrl_600010 ,CTN_1_MjHandCtrl_101021  };
	for(int i=0;i<4;++i)//��Ϸ��ʼ��������ܱ�������
	{
		int desk =m_pGameFrame->DeskStation2View(i);
		m_UserDataCard[desk].ClearHand();
		m_UserDataCard[desk].ClearCPG();
		m_UserDataCard[desk].ClearOut();

		UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, 0);
		SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[desk], SetIsLaydown(false));//��Ϸ��ʼ�����������ֱ����ʾ

		m_GameData.T_CPGNotify[i].Clear();
	}
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetControlVisible(false));   
	//���عη�����Ի���
	ShowGFXY(false);
	//��ʾׯ��
    UIShowZhuang(m_pGameFrame->DeskStation2View(pCmd->byNt), true);
	///��ʾ�ⶥ
	UIShowFengDing();
	
	//���سԸܿؼ�
	UIShowChiGangCtrl(false,false);
//��ʼ����
    PlayNormalSound(SOUND_BEGIN);

//	//����Ƿ��Ʒ����汾����ʾ���ư�ť
#ifdef MJ_CAN_SETPAI
    SAFE_CTRL_OPERATE(IButton, CTN_BTN_FAPAI, SetControlVisible(true));//��ʾ��������ť
	SAFE_CTRL_OPERATE(IButton, CTN_BTN_FAPAI, SetEnable(true));	// 
#endif
	
	//GetUserName();//��ȡ����������ǳ�
}
/// ��0λ���Ϊׯ����Ϣ:��һ��
void CNewGameClient::OnDongNt(void *buffer, int nLen)
{	
	tagDongNtEx *pCmd = (tagDongNtEx*)buffer;
	m_UserData.m_byNtStation = pCmd->byNt;//ׯ��λ��
	m_GameData.m_byThingDoing = THING_DONG_NT;//����ִ�е��¼�
    memcpy(&m_GameData.T_DongNt, pCmd, sizeof(m_GameData.T_DongNt));
	//��ʾׯ��
    UIShowZhuang(m_pGameFrame->DeskStation2View(pCmd->byNt), true);
	m_bStopSendPai = false;//�����Ƿ����
	SetStationParameter(GS_SEND_CARD);

}
// �����Ƶ���
void CNewGameClient::OnSeziSureOpenPoint(void *buffer, int nLen)
{
    tagTwoSeziDirAndGetPaiEx *pCmd=(tagTwoSeziDirAndGetPaiEx *)buffer;
    memcpy(&m_GameData.T_TwoSeziGetPai, pCmd, sizeof(m_GameData.T_TwoSeziGetPai));//�����¼�����
	m_GameData.m_byThingDoing = THING_2SEZI_DIR_GP;//����ִ�е��¼�
    /// ��ʾ����
    UIShowsaizi(m_pGameFrame->DeskStation2View(pCmd->byUser ), pCmd->bySezi0, pCmd->bySezi1);
    m_ucOpenDoor = (pCmd->bySezi0+pCmd->bySezi1)%PLAY_COUNT;
    m_ucOpenDun = min(pCmd->bySezi0,pCmd->bySezi1);
	m_bStopSendPai = false;//�����Ƿ����
	SetStationParameter(GS_SEND_CARD);

}

// ������������
// ע���ʱû�������ݣ����ƺ����
void CNewGameClient::OnSendAllCard(void *buffer, int nLen)
{
	UIShowsaizi(0xFF,0,0);//����ɫ��
    tagSendPaiEx * pSendPai=(tagSendPaiEx *)buffer;
    memcpy(&m_GameData.T_SendPai, pSendPai, sizeof(m_GameData.T_SendPai));
	m_GameData.m_byThingDoing = THING_SEND_PAI;//����ִ�е��¼�

    BYTE ucView = 255; 
	for(int i=0;i<PLAY_COUNT;++i)
	{
		BYTE ucView = m_pGameFrame->DeskStation2View(i);
        m_UserData.SetHandPaiData(i,pSendPai->m_byArHandPai[i]);
        m_UserDataCard[ucView].GetFromByte( pSendPai->m_byArHandPai[i] );  
		m_UserData.SetMenPaiData(i,pSendPai->byMenPai[i]);
		if(i != m_nDeskStation)
		{
			m_UserDataCard[ucView].FillWithBack(m_UserData.GetHandPaiCount(i));
		}
	}
    // ��ʾ��ǽ�ͳ���
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        int desk = m_pGameFrame->DeskStation2View(i);
        // ��ǽ
        m_UserData.SetMenPaiData(i,pSendPai->byMenPai[i]);	//��ȡ��������
        UIShowWall(i, m_UserData.m_iArMenPai[i]);		//��ʾ��ǽ�����ƣ�
    } 
	SetStationParameter(GS_SEND_CARD);
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        int desk = m_pGameFrame->DeskStation2View(i);
        UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, i==m_UserData.m_byNowOutStation);

		if(m_nDeskStation ==0)
		{
			CString str="";
			str.Format("�ͻ��ˣ�ץ�ƶ�����OnSendAllCard ��ͼλ��%d  �����������%d ",desk,m_UserDataCard[desk].m_nHandCardCount);
			OutputDebugString(str);
		}
    }
	////���ŷ��ƶ���
	//m_pGameFrame->SetTimer(TIME_FAPAI, 200);
	//m_byShowFaPai = 0;
 //   PlayNormalSound(SOUND_FAPAI);//���ŷ�������
}
// ��Ϸ��ʼ������
void CNewGameClient::OnGamePlay(void *buffer, int nLen)
{
	SetStationParameter(GS_PLAY_GAME);
    tagBeginOutPaiEx *pCmd = (tagBeginOutPaiEx *)buffer;
	OutputDebugString(TEXT("Ѫս�齫���ԣ�OnGamePlay  �յ���Ϣ"));

    memcpy(&m_GameData.T_BeginOutPai, pCmd, sizeof(m_GameData.T_BeginOutPai));
	m_GameData.m_byThingDoing = THING_BEGIN_OUT;//����ִ�е��¼�
	m_UserData.m_byNtStation = pCmd->byNt;
	m_UserData.m_byNowOutStation = pCmd->byNt; //��ǰ����λ��
    ////////////////////////////////////////////////////////////////////////////////

	UIShowQueMen(true);
    // ��ʾ��ǽ�ͳ���
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        int desk = m_pGameFrame->DeskStation2View(i);
        // ����                        
        m_UserDataCard[desk].ClearOut();                        
        UISetOutCard(desk, m_UserDataCard[desk].m_nUserOutInfo, m_UserDataCard[desk].m_nOutCardCount);
        UIShowLastTileTip(4, false);
        // ��ǽ
        m_UserData.SetMenPaiData(i,pCmd->byMenPai[i]);//��ȡ��������
        UIShowWall(i, m_UserData.m_iArMenPai[i]);//��ʾ��ǽ�����ƣ�
    } 

    m_ucCurrentPlayer = pCmd->byUser;//��ǰ���
    m_nZhuaViewStaiton = m_pGameFrame->DeskStation2View(pCmd->byUser);
	//��ʾׯ��
    UIShowZhuang(m_pGameFrame->DeskStation2View(pCmd->byNt), true);
//��ʼ����
    PlayNormalSound(SOUND_BEGIN);
	//���ö�ʱ��
	OnSetTimer(pCmd->byNt,TIME_OUT_CARD);

    for (int i=0; i<PLAY_COUNT; ++i)
    {
        int desk = m_pGameFrame->DeskStation2View(i);
        m_UserDataCard[desk].GetFromByte(  m_UserData.m_byArHandPai[i] );  
		if(i != m_nDeskStation)
		{
			m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
		}
        UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, i==m_UserData.m_byNowOutStation);

		if(m_nDeskStation ==0)
		{
			CString str="";
			str.Format("�ͻ��ˣ�ץ�ƶ�������ͼλ��%d  �����������%d ",desk,m_UserDataCard[desk].m_nHandCardCount);
			OutputDebugString(str);
		}
    }
	//��ʾ���ҵ�ȱ�����
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		m_UserData.m_byDingQue[i] = pCmd->byQuePai[i];
	}
	UIShowQueMen(true);

	if(m_nDeskStation ==0)
	{
		CString str="";
		str.Format("�ͻ��ˣ�ץ�ƶ����� �����������%d %d %d %d",m_UserData.GetHandPaiCount(0),m_UserData.GetHandPaiCount(1),m_UserData.GetHandPaiCount(2),m_UserData.GetHandPaiCount(3));
		OutputDebugString(str);
	}
}

// ����
void CNewGameClient::OnNotifyOutCard(void *buffer, int nLen)
{
	SetStationParameter(GS_PLAY_GAME);
    tagOutPaiEx *pCmd = (tagOutPaiEx *)buffer;
    memcpy(&m_GameData.T_OutPai, pCmd, sizeof(m_GameData.T_OutPai));
	m_GameData.m_byThingDoing = THING_OUT_PAI;//����ִ�е��¼�

    PlayNormalSound(SOUND_OUTCARD);

	for(int i=0;i<4;++i)
	{
		m_GameData.T_CPGNotify[i].Clear();//���������֪ͨ����
	}

    BYTE ucView = m_pGameFrame->DeskStation2View(pCmd->byUser);//������ͼλ��

    m_UserData.SetOutPaiData(pCmd->byUser,pCmd->m_byArOutPai[pCmd->byUser]);//��ȡ������
    m_UserDataCard[ucView].GetOutFromByte( pCmd->m_byArOutPai[pCmd->byUser] );//��ȡ�����ݣ�ui��ʾ��
    UISetOutCard(ucView, m_UserDataCard[ucView].m_nUserOutInfo, m_UserDataCard[ucView].m_nOutCardCount);//������ҵĳ��ƣ���ʾ��

	//���Ƹ�ֵ
	for(int i=0;i<4;i++)
	{
		m_UserData.m_bTing[i] = pCmd->bUserTing[i];
	}
    // �����Լ���������Ϣ
    if (pCmd->byUser==m_nDeskStation)//����������Լ�
    {
        KillGameTimer(TIME_OUT_CARD);

        m_UserData.SetHandPaiData(pCmd->byUser,pCmd->m_byArHandPai[pCmd->byUser]);
        m_UserDataCard[ucView].GetFromByte( pCmd->m_byArHandPai[pCmd->byUser] );//�����������       
        UISetHandCard(ucView, m_UserDataCard[ucView].m_nUserHandInfo, m_UserDataCard[ucView].m_nHandCardCount, true);  //��ʾ����
    }
    else    // ���ñ�ҵ�������Ϣ����������Ǳ���
    {
		//m_UserData.SetHandPaiBack(pCmd->byUser,m_UserData.GetHandPaiCount(pCmd->byUser));
		m_UserData.SetHandPaiData(pCmd->byUser, pCmd->m_byArHandPai[pCmd->byUser]);
		if (m_UserData.m_bIsHu[pCmd->byUser]) {
			
			int j = 0;
			for (j = 0; j < m_UserDataCard[ucView].m_nHandCardCount; j ++) {

				m_UserDataCard[ucView].m_nUserHandInfo[j] = 0;
			}
			m_UserDataCard[ucView].m_nUserHandInfo[j - 1] = m_UserData.GetLastHandPai(0);
		
		} else {
			
			m_UserDataCard[ucView].FillWithBack(m_UserData.GetHandPaiCount(pCmd->byUser));//�����������
		}
        UISetHandCard(ucView, m_UserDataCard[ucView].m_nUserHandInfo,m_UserDataCard[ucView].m_nHandCardCount, true);  //��ʾ����
    }
    // ֹͣץ�ƶ���          
    UIShowZhuaMoveAction(m_ptXiaJin, m_ptXiaJin, 0xFF, false);
    // ����������ʾ    
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		if(m_UserData.m_bIsHu[i])//������Ҳ�����
			continue;
        int desk = m_pGameFrame->DeskStation2View(i);
        UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, false);
    }
    m_ucLastTile = pCmd->byPs;//����
    // ������ʾ
    UIShowLastTileTip(ucView, true,m_ucLastTile);
    // ���ض�ʱ��
    UIShowTimer(ucView, m_MJSetting.ucOutcardTime, false);
	//��ʾ�й�ͼ��
    UIShowTrustee(true);   
    PlayTileSound(pCmd->byUser, pCmd->byPs);//��������
	m_bStopSendPai = true;//�����Ƿ����
	//���عη�����Ի���
	ShowGFXY(false);

}
// ץ��
void CNewGameClient::OnNotifyZhuaPai(void *buffer, int nLen)
{
	if(this->m_nDeskStation == 0)
	{
		OutputDebugString(TEXT("�¼���Ѫս�齫�� Client OnNotifyZhuaPai  �յ�ץ����Ϣ"));
	}

	if(m_GameData.m_byThingDoing >= THING_COUNT_FEN)//������Ϸ��
		return;

	if(this->m_nDeskStation == 0)
	{
		OutputDebugString(TEXT("�¼���Ѫս�齫�� Client OnNotifyZhuaPai  �յ�ץ����Ϣ ---"));
	}

	SetStationParameter(GS_PLAY_GAME);

	m_bStopSendPai = true;//�����Ƿ����
	UIShowQueMen(true);
    tagZhuaPaiEx *pCmd = (tagZhuaPaiEx *)buffer;
    memcpy(&m_GameData.T_ZhuaPai, pCmd, sizeof(m_GameData.T_ZhuaPai));//��������
	m_GameData.m_byThingDoing = THING_ZHUA_PAI;//����ִ�е��¼�

	m_UserData.m_byNowOutStation = pCmd->byUser; //��ǰ����λ��
    m_ucCurrentPlayer = pCmd->byUser;
    BYTE ucView = m_pGameFrame->DeskStation2View(pCmd->byUser);

    //// ͳ����������
    if (pCmd->byUser==m_nDeskStation)
    {// �����
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL,0));
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
		m_ucHandState = 0xFF;//����״̬
        m_UserDataCard[ucView].GetFromByte( m_UserData.m_byArHandPai[pCmd->byUser] );
        // �������ץ�ƶ�������������Ϊ��һ��
        UISetHandCard(ucView, m_UserDataCard[ucView].m_nUserHandInfo, m_UserDataCard[ucView].m_nHandCardCount, false);
    }   
    else// ���ñ�ҵ�������Ϣ
    {
		m_UserData.SetHandPaiBack(pCmd->byUser,m_UserData.GetHandPaiCount(pCmd->byUser));
        m_UserDataCard[ucView].FillWithBack(m_UserData.GetHandPaiCount(pCmd->byUser));
	}

	m_UserData.SetHandPaiData(pCmd->byUser,pCmd->m_byArHandPai[pCmd->byUser]);
	m_UserData.m_byLastZhuaPai[pCmd->byUser] = pCmd->byPs;//��¼ץ������
    // ��������    
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		m_UserData.SetMenPaiData(i,pCmd->byMenPai[i]);
	}
    // �ؼ�ID
    static int nCtrl_1[PLAY_COUNT] = {CTN_1_MjHandCtrl_101018 , CTN_1_MjHandCtrl_101019 , CTN_1_MjHandCtrl_600010 ,CTN_1_MjHandCtrl_101021  };
    // ����������ʾ    
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        int desk = m_pGameFrame->DeskStation2View(i);
		if(m_UserData.m_bIsHu[i])//������Ҳ�����
		{
			m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
		//	if(i == m_nDeskStation)
			//{
				m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
			//}
			//else
			//{
			//	m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
			//	m_UserDataCard[desk].m_nUserHandInfo[m_UserData.GetHandPaiCount(i)-1] = m_UserData.GetLastHandPai(i);
			//}
			int j = 0;
			for (j = 0; j < m_UserData.GetHandPaiCount(i); j ++) m_UserDataCard[desk].m_nUserHandInfo[j] = 0;
			m_UserDataCard[desk].m_nUserHandInfo[j-1] = m_UserData.GetLastHandPai(i);
			UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount);
			SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl_1[desk], SetIsLaydown(true));
			SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl_1[desk], SetGiveTileMode(i==pCmd->byUser));   
			continue;
		}
        // �������ץ�ƶ�������������Ϊ��һ��
        UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, false);    
		UIShowWall(i, m_UserData.m_iArMenPai[i]);
    }
    m_nZhuaViewStaiton = m_pGameFrame->DeskStation2View(m_ucCurrentPlayer);

	////����ץ�ƶ���
	BYTE index = pCmd->byGetPaiIndex;//��ǽ����
	BYTE dir = 0;//��ץ�����Ʒ���
	m_UserData.ZhuaPaiIndex(index,dir);
	index = ucChangePos[index];//��ǽ������
          
    IMjWallCtrl *pWallSrc = NULL;
    IMjHandCtrl *pHandSrc = NULL;
    RECT rt,rtDst;
    POINT pt,ptDst;         
	int iwall = m_pGameFrame->DeskStation2View(dir);//��ץ�����Ʒ���
	int iuser = m_pGameFrame->DeskStation2View(pCmd->byUser);//ץ����ҵ�λ��

    if ( GETCTRL(IMjWallCtrl, pWallSrc, m_pUI, CTN_1_MjWallCtrl_101026 + iwall) )//��ǽ
    {
        rt = pWallSrc->GetTileRectByIndex(index);
    }
	GETCTRL(IMjHandCtrl, pHandSrc, m_pUI, CTN_1_MjHandCtrl_101018 + iuser);
	if(m_nDeskStation == pCmd->byUser)//�Լ�ץ��
	{
		GETCTRL(IMjHandCtrl, pHandSrc, m_pUI, CTN_1_MjHandCtrl_600010);
	}
    if ( pHandSrc != NULL)
    {                        
		rtDst = pHandSrc->GetTileRectByIndex( m_UserData.GetHandPaiCount(pCmd->byUser)-2);
		if(m_nDeskStation ==0 && pCmd->byUser == 0)
		{
			CString str="";
			str.Format("�ͻ��ˣ�ץ�ƶ����� �յ�%d %d   iuser=%d", ptDst.x ,ptDst.y,iuser);
			OutputDebugString(str);
		}

    }
    // �ƶ�
    pt.x = rt.left;
    pt.y = rt.top;
    ptDst.x = rtDst.left;
    ptDst.y = rtDst.top;

    UIShowZhuaMoveAction(pt, ptDst, iwall, true);
    m_pGameFrame->SetTimer(TIME_ZHUAPAI, 500);

	if(m_nDeskStation ==0 && pCmd->byUser == 0)
	{
		//UserInfoForGame user,user2;
		//m_pGameFrame->GetUserInfo(pCmd->byUser,user);
		//CString str="";
		//str.Format("�ͻ��ˣ�ץ�ƶ��������%d %d  �յ�%d %d  ץ�����%d ���Ʒ���%d   ԭʼλ��%d  ץ�����id=%s ", pt.x,pt.y,ptDst.x ,ptDst.y,iuser,iwall,pCmd->byUser,user.szNickName);
		//OutputDebugString(str);

		//m_pGameFrame->GetUserInfo(dir,user);
		//str.Format("�ͻ��ˣ�ץ�ƶ�����ץ������%d dir=%d index=%d ��ץ���� ���id=%s",pCmd->byGetPaiIndex,dir ,index,user.szNickName);
		//OutputDebugString(str);

		//m_pGameFrame->GetUserInfo(0,user);
		//str.Format("�ͻ��ˣ�ץ�ƶ������۲����id=%s ���ƣ�%d %d %d %d",user.szNickName,m_UserData.GetMenPaiCount(0),m_UserData.GetMenPaiCount(1),m_UserData.GetMenPaiCount(2),m_UserData.GetMenPaiCount(3));
		//OutputDebugString(str);
	}


    PlayNormalSound(SOUND_ZHUAPAI);
	//��ʾ��ʱ��
	OnSetTimer(pCmd->byUser,TIME_OUT_CARD);
	if(pCmd->byUser == this->m_nDeskStation)//��˸����
	{
		FlashWindow();
	}
}

// ����
void CNewGameClient::OnNotifyBlock(void *buffer, int nLen)
{
	SetStationParameter(GS_PLAY_GAME);
    tagCPGNotifyEx *pCmd = (tagCPGNotifyEx *)buffer;

	if(m_GameData.m_byThingDoing == THING_BEGIN_OUT)//����ִ�е��¼�
	{//�Ȳ��ŷ��ƶ�������ʾ�����ܰ�ť
		if(pCmd->bChi || pCmd->bPeng || pCmd->bGang || pCmd->bTing || pCmd->bHu || pCmd->bBigGang)
		{
			memcpy(&m_GameData.T_CPGNotify[m_nDeskStation], pCmd, sizeof(tagCPGNotifyEx));//��������
		}
		OutputDebugString(TEXT("Ѫս�齫���ԣ�OnNotifyBlock  �յ���Ϣ"));
	}

	OutputDebugString(TEXT("�¼���Ѫս�齫���ԣ�OnNotifyBlock  �յ���Ϣ"));

	if(pCmd->bChi || pCmd->bPeng || pCmd->bGang || pCmd->bTing || pCmd->bHu || pCmd->bBigGang)
	{
		memcpy(&m_GameData.T_CPGNotify[m_nDeskStation], pCmd, sizeof(tagCPGNotifyEx));//��������
		m_GameData.m_byThingDoing = THING_CPG_NOTIFY;//����ִ�е��¼�

		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(true));

		SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600001, SetEnable(pCmd->bChi));		// ��
		SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600002, SetEnable(pCmd->bPeng));	// ��
		SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600003, SetEnable(pCmd->bGang));	// ��
		SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600004, SetEnable(pCmd->bHu));		// ��
		SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600006, SetEnable(pCmd->bTing));	// ��

		PlayNormalSound(SOUND_BLOCKTIP);
		OnKillAllTimer();
		OnSetTimer(m_nDeskStation,TIME_BLOCK_CARD);//�������Ƽ�ʱ��
		FlashWindow();
	}
	else
	{
		if(m_UserData.m_byNowOutStation == m_nDeskStation)//�Լ�����
		{
			OnKillAllTimer();
			OnSetTimer(m_nDeskStation,TIME_OUT_CARD);//�������Ƽ�ʱ��
		}
	}
}
// ����
void CNewGameClient::OnNotifyChiPai(void *buffer, int nLen)
{
}

// ����
void CNewGameClient::OnNotifyPengPai(void *buffer, int nLen)
{

    tagPengPaiEx *pCmd = (tagPengPaiEx *)buffer;
    memcpy(&m_GameData.T_PengPai, pCmd, sizeof(m_GameData.T_PengPai));//��������
	m_GameData.m_byThingDoing = THING_PENG_PAI;//����ִ�е��¼�
	m_UserData.m_byNowOutStation = pCmd->byUser; //��ǰ����λ��

	m_UserData.SetGCPData(pCmd->byUser,pCmd->m_UserGCPData[pCmd->byUser]);//��������������

    BYTE nView = m_pGameFrame->DeskStation2View(pCmd->byUser);
    BYTE nSrcChair = m_pGameFrame->DeskStation2View(pCmd->byBePeng);
    BYTE ucTile[3] = {0,0,0};

    m_UserDataCard[nView].GetFromCPGData(m_UserData.m_UserGCPData[pCmd->byUser],pCmd->byUser==m_nDeskStation);

    DebugPrintf(("��Ϸ�����%d������Ϣ���������� %d %d %d��"),m_nDeskStation, ucTile[0], ucTile[1], ucTile[2]);

    ////////////////////////////////////////////////////////////////////////////////
    // ���ó�����Ϣ
    m_UserData.SetOutPaiData(pCmd->byBePeng ,pCmd->m_byArOutPai[pCmd->byBePeng]);//���ñ�������ҵĳ�������
    m_UserDataCard[nSrcChair].GetOutFromByte( m_UserData.m_byArOutPai[pCmd->byBePeng] );
    UISetOutCard(nSrcChair, m_UserDataCard[nSrcChair].m_nUserOutInfo, m_UserDataCard[nSrcChair].m_nOutCardCount);//��ʾ������ҳ���

    // ͳ����������
    int nHandCount = 0;
    for (int i=0; i<HAND_CARD_NUM; ++i)
    {
        if (pCmd->m_byArHandPai[pCmd->byUser][i]!=255)
        {
            nHandCount++;
        }
    }
    // ����������ʾ    
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		nView = m_pGameFrame->DeskStation2View(i);
        m_UserData.SetHandPaiData(i ,pCmd->m_byArHandPai[i]);//��������
		if(i != m_nDeskStation)
		{
			m_UserDataCard[nView].FillWithBack(m_UserData.GetHandPaiCount(i));
		}
        m_UserDataCard[nView].GetFromByte( pCmd->m_byArHandPai[i] ); 
	}
	m_GameData.T_CPGNotify[m_nDeskStation].Clear();//��������ܱ�������

    // ����������ʾ    
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		if(m_UserData.m_bIsHu[i])//������Ҳ�����
			continue;
        int desk = m_pGameFrame->DeskStation2View(i);
        UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, desk==m_pGameFrame->DeskStation2View(pCmd->byUser));//��ʾ����
    }
    // �������ƿ�
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));

    m_ucCurrentPlayer = pCmd->byUser;//���õ�ǰ�������
    m_ucLastTile = 255;
	m_ucHandState = 0xFF;//����״̬
    // ���س�����ʾ
    UIShowLastTileTip(4, false);

	PlayTileSound(pCmd->byUser,SOUND_PENG);//����������

    //// �������ƶ���
	PlayAni(Ani_Peng_0 + m_pGameFrame->DeskStation2View(pCmd->byUser));
	
	//���عη�����Ի���
	ShowGFXY(false);
	OnSetTimer(pCmd->byUser,TIME_OUT_CARD);//�������Ƽ�ʱ��

	if(m_nDeskStation == pCmd->byUser)
	{
		UserInfoForGame UserInfo;
		m_pGameFrame->GetUserInfo(pCmd->byUser,UserInfo);
		char buff[500];
		sprintf(buff,"�ͻ��ˣ��¼� ���Ʋ��� OnNotifyPengPai   ���λ��%d ��ͼ%d ���id=%d ",pCmd->byUser,m_pGameFrame->DeskStation2View(pCmd->byUser),UserInfo.dwUserID);
		OutputDebugString(buff);
	}
	
}

// ����
void CNewGameClient::OnNotifyGangPai(void *buffer, int nLen)
{
    PlayNormalSound(SOUND_BLOCK);

    tagGangPaiEx *pCmd = (tagGangPaiEx *)buffer;
    memcpy(&m_GameData.T_GangPai, pCmd, sizeof(m_GameData.T_GangPai));//��������
	m_GameData.m_byThingDoing = THING_GANG_PAI;//����ִ�е��¼�
	m_UserData.m_byNowOutStation = pCmd->byUser; //��ǰ����λ��
	m_UserData.SetGCPData(pCmd->byUser,pCmd->m_UserGCPData[pCmd->byUser]);//��������������    
    BYTE nView = m_pGameFrame->DeskStation2View(pCmd->byUser);
    BYTE nSrcChair = m_pGameFrame->DeskStation2View(pCmd->byBeGang);
    BYTE ucTile[3] = {0,0,0};
    m_UserDataCard[nView].GetFromCPGData(m_UserData.m_UserGCPData[pCmd->byUser],pCmd->byUser==m_nDeskStation);

    DebugPrintf(("��Ϸ�����%d������Ϣ���������� %d %d %d��"),m_nDeskStation, ucTile[0], ucTile[1], ucTile[2]);

    ////////////////////////////////////////////////////////////////////////////////
    // ���ó�����Ϣ
    m_UserData.SetOutPaiData(pCmd->byBeGang,pCmd->m_byArOutPai[pCmd->byBeGang]);//���ñ�����ҵĳ���
    m_UserDataCard[nSrcChair].GetOutFromByte( m_UserData.m_byArOutPai[pCmd->byBeGang] );
    UISetOutCard(nSrcChair, m_UserDataCard[nSrcChair].m_nUserOutInfo, m_UserDataCard[nSrcChair].m_nOutCardCount);//��ʾ������ҵĳ���
    // ͳ����������
    int nHandCount = 0;
    for (int i=0; i<HAND_CARD_NUM; ++i)
    {
        if (pCmd->m_byArHandPai[pCmd->byUser][i]!=255)
        {
            nHandCount++;
        }
    }
    // �����Լ���������Ϣ
    if (pCmd->byUser==m_nDeskStation)
    {
        m_UserData.SetHandPaiData(pCmd->byUser,pCmd->m_byArHandPai[pCmd->byUser]);
        m_UserDataCard[nView].GetFromByte( pCmd->m_byArHandPai[pCmd->byUser] );   
		m_GameData.T_CPGNotify[m_nDeskStation].Clear();//��������ܱ�������
    }
    else    // ���ñ�ҵ�������Ϣ
    {
        m_UserData.SetHandPaiBack(pCmd->byUser ,nHandCount);
        m_UserDataCard[nView].FillWithBack(nHandCount);
    }

    // ����������ʾ    
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		if(m_UserData.m_bIsHu[i])//������Ҳ�����
			continue;
        int desk = m_pGameFrame->DeskStation2View(i);
        UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, desk==nView);
    }
    // �������ƿ�
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));

    m_ucCurrentPlayer = pCmd->byUser;
    m_ucLastTile = 255;
    if (ACTION_MING_GANG == pCmd->byType) // ����
    {
        // ���س�����ʾ
        UIShowLastTileTip(4, false);
    }    

	//��ʾ�η�����Ի���
	ShowGFXY(true,pCmd->byUser,pCmd->byBeGang,pCmd->byType);

	m_pGameFrame->SetTimer(TIME_FENG_YU, 3000);

    // ���Ÿ��ƶ���
	PlayAni(Ani_Gang_0 + m_pGameFrame->DeskStation2View(pCmd->byUser));

	if(pCmd->byType == ACTION_BU_GANG)//���ܣ��η磩
	{
		PlayTileSound(pCmd->byUser, SOUND_GUA_FENG);//�η�����
	}
	else
	{    
		PlayTileSound(pCmd->byUser, SOUND_XIA_YU);//��������
	}
    //// ���ö�ʱ��
	OnSetTimer(pCmd->byUser,TIME_OUT_CARD);//�������Ƽ�ʱ��
}

// ����
void CNewGameClient::OnNotifyHuPai(void *buffer, int nLen)  
{
	CString tempStr;

    tagHuPaiEx *pCmd = (tagHuPaiEx *)buffer;   
    memcpy(&m_GameData.T_HuPai, pCmd, sizeof(m_GameData.T_HuPai));//��������

	tempStr.Format("zth:  ��;���Ƶ����Ϊ   pCmd->byUser  = %d", pCmd->byUser);
	OutputDebugString(tempStr);

	//PlayNormalSound(SOUND_HU);
    KillGameTimer(TIME_BLOCK_CARD);
    // ���ź��ƶ���
    IExAnimate *pHupaiAni = NULL;
    GETCTRL(IExAnimate, pHupaiAni, m_pUI, CTN_1_ExAnimate_600034);
    if (NULL!=pHupaiAni)
    {
        pHupaiAni->SetPlayNum(1);                    
        pHupaiAni->SetIsLoop(false);
        pHupaiAni->SetPlayState(true);                    
    }
	
    if (pCmd->bZimo)
    {
		PlayTileSound(pCmd->byUser,SOUND_ZIMO);//��������
    }
    else 
    {		
		bool gangpao = false;
		//�����Ƿ�ܺ���
		for (int i=0; i<20; ++i)
		{
			if (pCmd->byHuType[pCmd->byUser][i] == HUPAI_TYPE_GANG_PAO)//�Ǹܺ���
			{
				gangpao = true;
				break;
			}
		}  
		if(gangpao)
		{
			PlayTileSound(pCmd->byUser,SOUND_GANG_PAO);//��������
		}
		else
		{
			PlayTileSound(pCmd->byUser,SOUND_DIAN_PAO);//���ڸܺ�������
		}
    }

	OnKillAllTimer();
    // �����������
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));

	m_UserData.m_bIsHu[pCmd->byUser] = true;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(pCmd->bCanH[i])
			m_UserData.m_bIsHu[i] = true;
	}
    // �ؼ�ID
    static int nCtrl_1[PLAY_COUNT] = {CTN_1_MjHandCtrl_101018 , CTN_1_MjHandCtrl_101019 , CTN_1_MjHandCtrl_600010 ,CTN_1_MjHandCtrl_101021  };
    for(int i=0;i<PLAY_COUNT;i++)
    {
        int desk =m_pGameFrame->DeskStation2View(i);
        m_UserData.SetHandPaiData(i,pCmd->m_byArHandPai[i]);
		if (!pCmd->m_bIsHu[i] /*&& i!=pCmd->byUser*/)
        {
			m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
			if(i != m_nDeskStation)
			{
				m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
			}
			UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount);
            continue;
        }

		m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
// 		if(i == m_nDeskStation)
// 		{
// 			m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
// 		}
// 		else
// 		{
// 			m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
// 			m_UserDataCard[desk].m_nUserHandInfo[m_UserData.GetHandPaiCount(i)-1] = m_UserData.GetLastHandPai(i);
// 		}

		int j = 0;
		for (j = 0; j < m_UserDataCard[desk].m_nHandCardCount; j ++) {

			m_UserDataCard[desk].m_nUserHandInfo[j] = 0;
		}
		m_UserDataCard[desk].m_nUserHandInfo[j-1] = m_UserData.GetLastHandPai(i);


		UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount);
        SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl_1[desk], SetIsLaydown(true));
        SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl_1[desk], SetGiveTileMode(i==pCmd->byUser));
    }
	if(!pCmd->m_bIsHu[m_nDeskStation] && pCmd->byUser != m_nDeskStation)//ֻ��ʾ������ҵĽ����
	{
		m_pGameFrame->SetTimer(TIME_SHOW_HU, 3000);//3������غ��ƶԻ���
	}
	m_GameData.m_byThingDoing = THING_HU_PAI;//����ִ�е��¼�
    // ��ʾ�������µ���
    // ���س�����ʾ
    UIShowLastTileTip(4, false);
	UIShowResultDlg(NULL,pCmd,0);//��ʾ�����
	//���عη�����Ի���
	ShowGFXY(false); 

}
// ����
void CNewGameClient::OnNotifyCancelBlock(void *buffer, int nLen)
{
	    KillGameTimer(TIME_BLOCK_CARD);
}

/// ��ȱ�¼�
void CNewGameClient::OnNotifyDingQue(void *buffer, int nLen)
{
    tagDingQueEx *pCmd = (tagDingQueEx *)buffer;   

	if(pCmd->bNotify && pCmd->byUser == m_nDeskStation)//�Լ��Ķ�ȱ֪ͨ
	{
		OnSetTimer(m_nDeskStation,TIME_DING_QUE);//������ȱ��ʱ��
		SAFE_CTRL_OPERATE(IContain, CTN_DING_QUE, SetControlVisible(true));//��ʾ��ȱ�Ի���
	}
	else if(pCmd->byUser == m_nDeskStation)//�Լ�����ɶ�ȱ֪ͨ
	{
		SAFE_CTRL_OPERATE(IContain, CTN_DING_QUE, SetControlVisible(false));//��ʾ��ȱ�Ի���
		UIShowQueMen(true,m_nDeskStation);
	}

	if(pCmd->byUser>=0 && pCmd->byUser<PLAY_COUNT)
	{
		m_UserData.m_byDingQue[pCmd->byUser] = pCmd->byQuePai[pCmd->byUser];
		PlayNormalSound(SOUND_DING_QUE);//��ȱ����
	}
}
/// �����ͻ��˻���
void CNewGameClient::OnNotifySuperSetPai(void *buffer, int nLen)
{
	SuperStruct *pCmd = (SuperStruct *)buffer;   
    int desk = m_pGameFrame->DeskStation2View(m_nDeskStation);
	m_UserData.SetHandPaiData(m_nDeskStation,pCmd->mjdata);//������������
    m_UserDataCard[desk].GetFromByte( pCmd->mjdata );       
    // �������ץ�ƶ�������������Ϊ��һ��
    UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount-(m_ucCurrentPlayer==desk), false);  
}
// ����
void CNewGameClient::OnNotifyFinish(void *buffer, int nLen)
{
   	SetStationParameter(GS_WAIT_NEXT);
	m_GameData.m_byThingDoing = THING_ENG_HANDLE;//����ִ�е��¼�
    m_pGameFrame->KillTimer(TIME_ZHUAPAI);
    memcpy((BYTE*)&m_tagCountFen, buffer, sizeof(m_tagCountFen));
    memcpy((BYTE*)&m_GameData.T_CountFen, buffer, sizeof(m_GameData.T_CountFen));
    tagCountFenEx *pCmd = (tagCountFenEx *)buffer;   
	if(m_nDeskStation ==0)
	{
		OutputDebugString(TEXT("�ͻ��� ���齫-���ƣ�Client OnNotifyFinish �յ���Ϸ������Ϣ"));
	}
    // ���¾�
    bool bGetted = false;
    int nWallIndex = -1;
    int nDoor = -1;
    // ������ǽ��ʾ
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        int desk = m_pGameFrame->DeskStation2View(i);
        m_UserData.SetMenPaiData(i,m_UserData.m_iArMenPai[i]);
        UIShowWall(i, m_UserData.m_iArMenPai[i]);
        m_UserData.SetHandPaiData(i,m_tagCountFen.m_byArHandPai[i]);
    } 
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        UIShowLastTileTip(i, false);
    }
    int nCtrl[PLAY_COUNT] = {CTN_1_MjHandCtrl_101018 , CTN_1_MjHandCtrl_101019 ,CTN_1_MjHandCtrl_600010 , CTN_1_MjHandCtrl_101021  };

    for(int i=0;i<PLAY_COUNT;i++)
    {
        int desk =m_pGameFrame->DeskStation2View(i);
        m_UserDataCard[desk].GetFromByte(m_UserData.m_byArHandPai[i]);
		m_UserDataCard[desk].GetFromCPGData(m_UserData.m_UserGCPData[i],true);

        SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[desk], SetIsLaydown(true));
        SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[desk], SetGiveTileMode(i==m_tagCountFen.byUser));
        UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount);

		if(m_nDeskStation ==0)
		{
			CString str="";
			str.Format("�ͻ��ˣ����㣺���%d ���� %d %d %d",desk,m_UserDataCard[desk].m_nUserHandInfo[0],m_UserDataCard[desk].m_nUserHandInfo[1],m_UserDataCard[desk].m_nUserHandInfo[2]);
			OutputDebugString(str);
		}
    }

    UIShowResultDlg(pCmd,NULL,1);
	//���عη�����Ի���
	ShowGFXY(false);
}

// ��ʾ�����
void CNewGameClient::UIShowResultDlg(tagCountFenEx * fen , tagHuPaiEx * hu ,BYTE type)
{
	if(type == 0)//���ƣ���Ϸ��û�н���
	{
		if(hu == NULL)
			return;
		// ��ʾ���ƶԻ���
		SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(true));
		//��ȫ������
		for (int i=HUPAI_USER1_0; i<TEXT_JS_HuFen_Title+1; ++i)
		{
			SAFE_CTRL_OPERATE(IText, i, SetControlVisible(false));
		}
		int index=0;//
		for (int i=0; i<PLAY_COUNT; ++i)//ѭ�������������
		{					
			if(!hu->m_bIsHu[i])//û������
				continue;
			wchar_t wszWho[300]=L"";
			if (hu->m_bIsZiMo[i])
			{
				::swprintf_s(wszWho,L" %s",m_wszNiCheng[i]);
				SAFE_CTRL_OPERATE(IText, HUPAI_USER1_0 + index*4, SetText(wszWho));//�������
				SAFE_CTRL_OPERATE(IText, HUPAI_USER1_0 + index*4, SetControlVisible(true));
				SAFE_CTRL_OPERATE(IText, HUPAI_FangShi1 + index*4, SetText(L"����"));//���Ʒ�ʽ
				SAFE_CTRL_OPERATE(IText, HUPAI_FangShi1 + index*4, SetControlVisible(true));
			}
			else if (255!=hu->m_byDianPao[i])
			{
				::swprintf_s(wszWho,L"%s ",m_wszNiCheng[hu->m_byDianPao[i]]);
				SAFE_CTRL_OPERATE(IText, HUPAI_USER1_0 + index*4, SetText(wszWho));//�������
				SAFE_CTRL_OPERATE(IText, HUPAI_USER1_0 + index*4, SetControlVisible(true));
				SAFE_CTRL_OPERATE(IText, HUPAI_FangShi1 + index*4, SetText(L"���ڸ�"));//���Ʒ�ʽ
				SAFE_CTRL_OPERATE(IText, HUPAI_FangShi1 + index*4, SetControlVisible(true));
				::swprintf_s(wszWho,L"%s ",m_wszNiCheng[i]);
				SAFE_CTRL_OPERATE(IText, HUPAI_USER1_1 + index*4, SetText(wszWho));//�������
				SAFE_CTRL_OPERATE(IText, HUPAI_USER1_1 + index*4, SetControlVisible(true));
			}    
			// ����
			IText *pFan = NULL;
			GETCTRL(IText, pFan, m_pUI, HUPAI_TYPE1 + index*4);
			if (pFan)
			{
				wchar_t wszFan[500]=L"";
				wchar_t wszFanName[17][200] = 
				{
				{L"�ط�0��"},{L"�����1��"},{L"��һɫ2��"},{L"���߶�2��"},{L"ȫ����2��"},{L""},{L"���3��"},{L"���߶�3��"},{L"���߶�3��"},
				{L""},{L"���Ͽ���1��"},{L"�ܺ���1��"},{L"���4��"},{L"�غ�4��"},{L"����1��"},{L""},{L""}
				};
				for(int ii=0; ii<17; ++ii)
				{
					if (CMjRef::CheckHupaiType(ii + HUPAI_HU_PAI, hu->byHuType[i], MAX_HUPAI_TYPE)) //���ں���
					{
						::swprintf_s(wszFan, L"%s %s", wszFan, wszFanName[ii]);
					}
				}  
				if(CMjRef::CheckHupaiType(HUPAI_ZI_MO,hu->byHuType[i],MAX_HUPAI_TYPE))
				{//����
					::swprintf_s(wszFan, L"%s ����%d��", wszFan,1);
				}
				if(hu->m_iGengCount[i] > 0 && hu->m_iGengCount[i] != 255)
				{
					::swprintf_s(wszFan, L"%s ��%d��", wszFan,hu->m_iGengCount[i]);
				}
				::swprintf_s(wszFan, L"%s �ϼ�%d��", wszFan,hu->m_iZongFan[i]);
				
				pFan->SetText(wszFan);
				pFan->SetControlVisible(true);
			}
			index++;
		}
		return;
	}
////////////����-��Ϸ�Ѿ�����-------------------

	OnSetTimer(2,TIME_BEGIN);
    // �й�״̬
    SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10002, SetControlVisible(true));
    SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10003, SetControlVisible(false));
    // ��ʾ��ʼ��ť
    SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10001, SetControlVisible(false));
    // �����������
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
    UIShowTimer(0xFF, 0, false);
	SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));///���غ��ƶԻ���

	if(fen == NULL)
		return;
	//������
	for(int i=IMG_Liu_Ju;i<TEXT_JS_HuFen_Title+1;++i)
	{
		SAFE_CTRL_OPERATE(IText,  i, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage,  i, SetControlVisible(false));
	}
	int index=0;//
	wchar_t wszFen[500]=L"";
	//�����ֽ��㣺����Һ���
	SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_20500, SetControlVisible(true));//��ʾ�����GetGameXY 
	//����
	if(fen->byUser == 255)//���ֽ���
	{
		
		SAFE_CTRL_OPERATE(IImage,  IMG_Liu_Ju, SetControlVisible(true));//��ʾ����ͼ��
		PlayNormalSound(SOUND_LIUJU);// ��ʾ�����
		//�黨����
		if(fen->m_bHuaJiao)
		{
			SAFE_CTRL_OPERATE(IText,  TEXT_JS_HuaZhu_Title, SetControlVisible(true));//��� �黨��  ����
			SAFE_CTRL_OPERATE(IText,  TEXT_JS_ChaJiao_Title, SetControlVisible(true));//��� ���  ����

			for (int i=0; i<PLAY_COUNT; ++i)
			{	
				//�黨��
				SAFE_CTRL_OPERATE(IText, TEXT_JS_HuaZhu_0 + i, SetControlVisible(true));

				//GlbGetFloatString(szText,fen->m_iHuaZhu[i]);
				//MultiByteToWideChar(CP_ACP,0,szText,-1,wszFen,256);
				::swprintf_s(wszFen, L"%d", fen->m_iHuaZhu[i]);
				SAFE_CTRL_OPERATE(IText, TEXT_JS_HuaZhu_0 + i, SetText(wszFen));

				//���
				SAFE_CTRL_OPERATE(IText, TEXT_JS_ChaJiao_0 + i, SetControlVisible(true));
				::swprintf_s(wszFen, L"%d", fen->m_iChaJiao[i]);
				//GlbGetFloatString(szText,fen->m_iChaJiao[i]);
				//MultiByteToWideChar(CP_ACP,0,szText,-1,wszFen,256);
				SAFE_CTRL_OPERATE(IText, TEXT_JS_ChaJiao_0 + i, SetText(wszFen));
			}
		}
	}	
	bool bZhuanYi=false;//�Ƿ��к���ת��
	for (int i=0; i<PLAY_COUNT; ++i)
	{	
		if(fen->m_iZhuanYi[i] != 0)
			bZhuanYi = true;//�к���ת��
	}

	SAFE_CTRL_OPERATE(IText,  TEXT_JS_GFXY_Title, SetControlVisible(true));//��� �η�����  ����
	SAFE_CTRL_OPERATE(IText,  TEXT_JS_HuFen_Title, SetControlVisible(true));//��� ����  ����
	for (int i=0; i<PLAY_COUNT; ++i)
	{					
	//�η�����
		//�ǳ�
		SAFE_CTRL_OPERATE(IText, TEXT_JS_Gang_User_0 + i, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IText, TEXT_JS_Gang_User_0 + i, SetText(m_wszNiCheng[i]));
		//��
		SAFE_CTRL_OPERATE(IText, TEXT_JS_Gang_Fen_0 + i, SetControlVisible(true));
		::swprintf_s(wszFen, L"%d", fen->iGangFen[i]);
		SAFE_CTRL_OPERATE(IText, TEXT_JS_Gang_Fen_0 + i, SetText(wszFen));
		////���
		//SAFE_CTRL_OPERATE(IText, TEXT_JS_Gang_JinBi_0 + i, SetControlVisible(true));
		//::swprintf_s(wszFen, L"%d", fen->iGangFen[i]*fen->iBasePoint);
		//SAFE_CTRL_OPERATE(IText, TEXT_JS_Gang_JinBi_0 + i, SetText(wszFen));
		SAFE_CTRL_OPERATE(IText, TEXT_JS_HJZY_Title, SetControlVisible(false));
		if(bZhuanYi)
		{
			SAFE_CTRL_OPERATE(IText, TEXT_JS_HJZY_Title, SetControlVisible(true));
			SAFE_CTRL_OPERATE(IText, TEXT_JS_ZhuanYi_0 + i, SetControlVisible(true));
			::swprintf_s(wszFen, L"%d", fen->m_iZhuanYi[i]);
			SAFE_CTRL_OPERATE(IText, TEXT_JS_ZhuanYi_0 + i, SetText(wszFen));
			OutputDebugString("���߲��ԣ���ʾ����ת��");
		}
		
	//�ܽ���
		//�ǳ�
		SAFE_CTRL_OPERATE(IText, TEXT_JS_User_0 + i, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IText, TEXT_JS_User_0 + i, SetText(m_wszNiCheng[i]));

		//���Ʒ�
		SAFE_CTRL_OPERATE(IText, TEXT_JS_Hu_Fen_0 + i, SetControlVisible(true));
		::swprintf_s(wszFen, L"%d", fen->iHuFen[i]);
		SAFE_CTRL_OPERATE(IText, TEXT_JS_Hu_Fen_0 + i, SetText(wszFen));

		//��
		SAFE_CTRL_OPERATE(IText, TEXT_JS_ZongFen_0 + i, SetControlVisible(true));
		::swprintf_s(wszFen, L"%I64d", fen->iZongFen[i]);
		SAFE_CTRL_OPERATE(IText, TEXT_JS_ZongFen_0 + i, SetText(wszFen));
		//���
		SAFE_CTRL_OPERATE(IText, TEXT_JS_JinBi_0 + i, SetControlVisible(true));
		::swprintf_s(wszFen, L"%I64d", fen->iMoney[i]);
		SAFE_CTRL_OPERATE(IText, TEXT_JS_JinBi_0 + i, SetText(wszFen));

		if(!fen->bHu[i])//û������
		{
			continue;
		}
		//˭��
		IText *pTextWho = NULL;
		SAFE_CTRL_OPERATE(IText, TEXT_JS_Hu_User_1 + index, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IText, TEXT_JS_Hu_User_1 + index, SetText(m_wszNiCheng[i]));

		if(fen->m_bIsZiMo[i])
		{
			//��ʾ����ͼƬ	��������	
			SAFE_CTRL_OPERATE(IImage, IMG_JS_ZiMo_1 + index, SetSrcX(39*2));//��ʾ����ͼƬ�����Ʒ�ʽ��
			SAFE_CTRL_OPERATE(IImage, IMG_JS_ZiMo_1 + index, SetControlVisible(true));
		}
		else if (255!=fen->m_byDianPao[i])
		{
			SAFE_CTRL_OPERATE(IText, TEXT_JS_Dian_User_1 + index, SetControlVisible(true));
			SAFE_CTRL_OPERATE(IText, TEXT_JS_Dian_User_1 + index, SetText(m_wszNiCheng[fen->m_byDianPao[i]]));
			//��ʾ����ͼƬ
			SAFE_CTRL_OPERATE(IImage, IMG_JS_Dian_1 + index, SetControlVisible(true));//����ͼƬ
			////��ʾ����ͼƬ(���ں���)		
			//SAFE_CTRL_OPERATE(IImage, IMG_JS_PaoHu_1 + index, SetControlVisible(true));//��ʾ�������������Ʒ�ʽ��

			SAFE_CTRL_OPERATE(IImage, IMG_JS_ZiMo_1 + index, SetSrcX(39));//��ʾ�������������Ʒ�ʽ��
			SAFE_CTRL_OPERATE(IImage, IMG_JS_ZiMo_1 + index, SetControlVisible(true));//��ʾ�������������Ʒ�ʽ��
		}
		// ����
		IText *pFan = NULL;
		GETCTRL(IText, pFan, m_pUI, TEXT_JS_Hu_Type_1 + index);
		if (pFan)
		{
			wchar_t wszFan[500]=L"";
			wchar_t wszFanName[17][200] = 
			{
				{L"�ط�0��"},{L"�����1��"},{L"��һɫ2��"},{L"���߶�2��"},{L"ȫ����2��"},{L""},{L"���3��"},{L"���߶�3��"},{L"���߶�3��"},
				{L""},{L"���Ͽ���1��"},{L"�ܺ���1��"},{L"���4��"},{L"�غ�4��"},{L"����1��"},{L""},{L""}
			} ;
			for(int ii=0; ii<17; ++ii)
			{
				if (CMjRef::CheckHupaiType(ii + HUPAI_HU_PAI, fen->byHuType[i], MAX_HUPAI_TYPE)) //���ں���
				{
					::swprintf_s(wszFan, L"%s %s", wszFan, wszFanName[ii]);
				}
			}  
			if(CMjRef::CheckHupaiType(HUPAI_ZI_MO,fen->byHuType[i],MAX_HUPAI_TYPE))
			{//����
				::swprintf_s(wszFan, L"%s ����%d��", wszFan,1);
			}
			if(fen->m_iGengCount[i] > 0 && fen->m_iGengCount[i] != 255)
			{
				::swprintf_s(wszFan, L"%s ��%d��", wszFan,fen->m_iGengCount[i]);
			}

			::swprintf_s(wszFan, L"%s �ϼ�%d��", wszFan,fen->m_iZongFan[i]);
			pFan->SetText(wszFan);
			pFan->SetControlVisible(true);
		}
		index++;
	}
	//����ת��
	if(fen->m_bZhuanYi)
	{
		wchar_t wszZhuanYi[512];
		::swprintf_s(wszZhuanYi, L"����ת�ƣ�");
		for (int i=0; i<PLAY_COUNT; ++i)
		{
			if(fen->m_iZhuanYi[i] == 0)
				continue;
			::swprintf_s(wszZhuanYi, L"  %s %d��", m_wszNiCheng[i],fen->m_iZhuanYi[i]);
		}
		SAFE_CTRL_OPERATE(IText, TEXT_JS_ZHUAN_YI, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IText, TEXT_JS_ZHUAN_YI, SetText(wszZhuanYi));
	}

	if(fen->iMoney[m_nDeskStation] > 0)//����Ӯ
	{
		PlayNormalSound(SOUND_WIN);//Ӯ����
	}
	else//������
	{    
		PlayNormalSound(SOUND_LOST);//������
	}
	//���ÿ�ʼ��ʱ��
	OnKillAllTimer();
	OnSetTimer(m_nDeskStation,TIME_BEGIN);//�������Ƽ�ʱ��

}

 /// ǿ�˽�����Ϸ
void CNewGameClient::OnForceFinish(void *buffer, int nLen)     
{
	tagCountFenEx *pCmd = (tagCountFenEx *)buffer;   
	if(pCmd->byCloseFlag == GFF_FORCE_FINISH)
	{//��ʾǿ�˽��������
		OnSetTimer(2,TIME_BEGIN);
		// �й�״̬
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10002, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10003, SetControlVisible(false));
		// ��ʾ��ʼ��ť
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10001, SetControlVisible(false));
		// �����������
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
		UIShowTimer(0xFF, 0, false);
		SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));///���غ��ƶԻ���
		//������
		for(int i=IMG_Liu_Ju;i<TEXT_JS_ZhuanYi_3+1;++i)
		{
			SAFE_CTRL_OPERATE(IText,  i, SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,  i, SetControlVisible(false));
		}
		//�����ֽ��㣺����Һ���
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_20500, SetControlVisible(true));//��ʾ�����GetGameXY 
		wchar_t wszFen[500]=L"";
		for (int i=0; i<PLAY_COUNT; ++i)
		{					
			//�ܽ���
			//�ǳ�
			SAFE_CTRL_OPERATE(IText, TEXT_JS_User_0 + i, SetControlVisible(true));
			SAFE_CTRL_OPERATE(IText, TEXT_JS_User_0 + i, SetText(m_wszNiCheng[i]));
			//��
			SAFE_CTRL_OPERATE(IText, TEXT_JS_ZongFen_0 + i, SetControlVisible(true));
			::swprintf_s(wszFen, L"%I64d", pCmd->iZongFen[i]);
			SAFE_CTRL_OPERATE(IText, TEXT_JS_ZongFen_0 + i, SetText(wszFen));
			//���
			SAFE_CTRL_OPERATE(IText, TEXT_JS_JinBi_0 + i, SetControlVisible(true));
			::swprintf_s(wszFen, L"%I64d", pCmd->iMoney[i]);
			SAFE_CTRL_OPERATE(IText, TEXT_JS_JinBi_0 + i, SetText(wszFen));
			// ����
			IText *pFan = NULL;
			GETCTRL(IText, pFan, m_pUI, TEXT_JS_Hu_Type_1);
			if (pFan)
			{
				wchar_t wszFan[500]=L"ǿ�˽�����Ϸ";
				pFan->SetText(wszFan);
				pFan->SetControlVisible(true);
			}
		}
	}
}


// �������
void CNewGameClient::RequestOutCard(BYTE ucTile)
{
	char buff[500];
	sprintf(buff,"�ͻ��ˣ��¼� RequestOutCard 000  �������%d",m_UserData.GetHandPaiCount(m_nDeskStation));
	OutputDebugString(buff);
    if (m_UserData.IsOutPaiPeople(m_nDeskStation)/*m_nDeskStation==m_ucCurrentPlayer*/)
    {
        tagOutPaiEx chu;
        chu.Clear();
        chu.byPs = ucTile;
        chu.byUser = m_nDeskStation;
        chu.bTing = false;//����

		OutputDebugString(TEXT("�ͻ��ˣ��¼� �齫 Client ���Ƴɹ�"));
        SendGameDataEx(THING_OUT_PAI, &chu, sizeof(chu));
		
		UIShowGangCtrl(false);
		// ������	
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
		// �����	
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL,0));
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
        m_ucHandState = 0xFF;//����״̬
		//���سԸܿؼ�
		UIShowChiGangCtrl(false,false);
    }
}
// �������
void CNewGameClient::RequestBlockChi()
{
}
// ��������
void CNewGameClient::RequestBlockPeng()
{
    tagPengPaiEx peng;
    peng.Clear();
    peng.byPs = m_ucLastTile;
    peng.byUser = m_nDeskStation;
    peng.byBePeng = m_ucCurrentPlayer;
    SendGameDataEx(THING_PENG_PAI, &peng, sizeof(peng));
    // �����������
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
}
// �������
void CNewGameClient::RequestBlockGang()
{

    bool m_bGang = false;
	bool m_bBigGang=false;//�Ƿ������
    BYTE gang[4];
    BYTE index=0;
    memset(gang,255,sizeof(gang));
    BYTE		ucGangData[4][2];


    if(m_nDeskStation != m_ucCurrentPlayer)//�������
    {
        if(m_ucLastTile== 255 || !m_GameData.T_CPGNotify[m_nDeskStation].bGang)
            return ;   

		tagGangPaiEx gang;
		gang.Clear();
		gang.byPs = m_ucLastTile;
		gang.byType = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[0][0];
		gang.byUser = m_nDeskStation;
		gang.byBeGang = m_ucCurrentPlayer;
        SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));

		//ClearTempData();
		//m_pGameFrame->KillAllTimer();
		// �����������
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));

		return ;
	}
	else//�Լ�ץ�ƣ���ⰵ�ܺͲ��ܣ�
	{
		//���ܵ�����
		BYTE byPs = 255;	//Ҫ�ܵ���
		BYTE byType = 255; //���Ƶ�����
		int count = 0;
		for(int i=0;i<4;i++)
		{
			if(m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1] != 255)
			{
				count++;
				byPs = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1];
				byType = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][0];
			}
		}
		if(count==1)//һ�ָ�
		{
			tagGangPaiEx gang;
			gang.Clear();
			gang.bIsBigGang = m_bBigGang;
			gang.byPs = byPs;
			gang.byType = byType;
			gang.byUser = m_nDeskStation;
			gang.byBeGang = m_ucCurrentPlayer;
			SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
			// �����������
			SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
			return;
		}
    }
	OutputDebugString("�ͻ��ˣ��¼� ����״̬  000 ");
    m_ucHandState = 2;
	//��ʾ�Ըܿؼ�
	//UIShowChiGangCtrl(true,false);
    // ��Ҫѡ��
    int nInvalidTiles[MAX_TILES_INCTRL];
	int nEnableTiles[MAX_TILES_INCTRL];
    int nInvalidCnt = 0;
    int nEnableCnt = 0;
	//�ж��ָ��ƣ��ûҲ��ܸܵ���
	for(int i=0;i<4;i++)
	{
		if(m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1] != 255)
		{
			nEnableTiles[nEnableCnt] = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1];
			nEnableCnt++;
		}
	}
	memcpy(m_nEnableTiles, nEnableTiles, sizeof(m_nEnableTiles));//��������
    // ��Ҫѡ��
	OutputDebugString("�ͻ��ˣ��¼� ����״̬  111 ");
    m_ucHandState = 2;
    // ���
    for (int i=0; i<38; ++i)
    {
        if (i%10==0 )
        {
            continue;
        }
        bool bFind = false;
        for (int j=0; j<nEnableCnt; ++j)
        {
            if (nEnableTiles[j]==i)
            {
                bFind = true;
                break;
            }
        }
        if (!bFind)
        {
            nInvalidTiles[nInvalidCnt++] = i;
        }
    }            
    // ���
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(nInvalidTiles, nInvalidCnt));
    SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(false));
    // �����������
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
    //m_pGameFrame->KillAllTimer();
	//��ʾȡ����ť��������ʱȡ������
	SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(true));	

	SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600001, SetEnable(false));	// ��
	SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600002, SetEnable(false));	// ��
	SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600003, SetEnable(false));	// ��
	SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600004, SetEnable(false));	// ��
	SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600006, SetEnable(false));	// ��
	SAFE_CTRL_OPERATE(IButton, CTN_1_CTN_600000_BTN_600005, SetEnable(true));	// ����

}
// �������
void CNewGameClient::RequestBlockHu()
{
    ///��ҵĺ�������    
    BYTE    byHuType[4][MAX_HUPAI_TYPE];//������ҵĺ�������
    tagHuPaiEx hu;
    hu.Clear();
    hu.byUser = m_nDeskStation;
    hu.byDianPao = m_ucCurrentPlayer;
    hu.byPs = m_ucLastTile;
    if(m_nDeskStation == m_ucCurrentPlayer)
    {
        hu.byPs = 255;
        hu.bZimo = true;
    }
    memcpy(hu.byHuType[m_nDeskStation],byHuType[m_nDeskStation],sizeof(byHuType[m_nDeskStation]));
    SendGameDataEx(THING_HU_PAI, &hu, sizeof(hu));
    // �����������
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
}
// ��������
void CNewGameClient::RequestCancelBlock()
{
    SendGameDataEx(ASS_PASS_CHECK,NULL, 0);
	UIShowGangCtrl(false);
    // ������
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
	// �����
	SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL, 0));
	SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
    m_ucHandState = 0xFF;//����״̬
	//��ʾ�Ըܿؼ�
	UIShowChiGangCtrl(false,false);
	OnKillAllTimer();
	if(m_UserData.m_byNowOutStation  == m_nDeskStation)//�Լ�����
	{
		OnSetTimer(m_nDeskStation,TIME_OUT_CARD);
	}
}

// �����й�
void CNewGameClient::RequestTuoGuan(bool bTuoguan )
{
	if(m_GameData.m_byThingDoing < THING_BEGIN_OUT || m_GameData.m_byThingDoing >= THING_HU_PAI)
		return;//������Ϸ״̬�в����й�
    Usertouguan msg;
    msg.desk = m_nDeskStation;
    msg.touguang = bTuoguan;
    SendGameDataEx(ASS_AUTOCARD, &msg, sizeof(msg));
}
//�Ը�����
void CNewGameClient::RequestBlockChiGang(UINT32 id)
{
	int index = 0;
	index = id - Ctr_ChiGang0;
	if(m_ucHandState == 1)//��
	{
        tagChiPaiEx chi;
        chi.Clear();
        chi.byUser = m_nDeskStation;
        chi.byBeChi = m_ucCurrentPlayer;
        chi.byPs = m_ucLastTile;
        chi.byChiPs[0] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[index][0];
        chi.byChiPs[1] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[index][1];
        chi.byChiPs[2] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[index][2];
        SendGameDataEx(THING_CHI_PAI, &chi, sizeof(chi));
	}
	else if(m_ucHandState == 2)//��
	{
		tagGangPaiEx gang;
		gang.Clear();
		gang.byPs = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[index][1];
		gang.byType = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[index][0];
		gang.byUser = m_nDeskStation;
		gang.byBeGang = m_ucCurrentPlayer;
		SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
	}	
	// �����������
	SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
	//��ʾ�Ըܿؼ�
	UIShowChiGangCtrl(false,false);
	m_ucHandState = 0xFF;//����״̬
}
// ��ʾ��ǽ
void CNewGameClient::UIShowWall(BYTE bViewStation, bool bShow)
{
    // ��ǽID
    static int nCtrl[PLAY_COUNT] = {CTN_1_MjWallCtrl_101026,CTN_1_MjWallCtrl_101027,CTN_1_MjWallCtrl_101028,CTN_1_MjWallCtrl_101029};
	BYTE view = 255;
    if (bViewStation>PLAY_COUNT)
    {
        for (int i=0; i<PLAY_COUNT; ++i)
        {
			view = m_pGameFrame->DeskStation2View(i);
			if(i == 0 || i == 2)
			{
				SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[view], FillWallBack(28));
			}
			else 
			{
				SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[view], FillWallBack(26));
			}
            SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[view], SetControlVisible(bShow));
        }
        return;
    }
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		view = m_pGameFrame->DeskStation2View(i);
		if(i == 0 || i == 2)
		{
			SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[view], FillWallBack(28));
		}
		else 
		{
			SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[view], FillWallBack(26));
		}
    }
    SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[bViewStation], SetControlVisible(bShow));
}
// ���ݸ�ʽ��255��û���ƣ� 0���Ʊ�������������һ��
void CNewGameClient::UIShowWall(BYTE station, BYTE ucArrMenPai1[])
{
    // ��ǽID
    static int nCtrl[PLAY_COUNT] = {CTN_1_MjWallCtrl_101026,CTN_1_MjWallCtrl_101027,CTN_1_MjWallCtrl_101028,CTN_1_MjWallCtrl_101029};
	BYTE bViewStation = m_pGameFrame->DeskStation2View(station);
    BYTE ucArrMenPai[28];
    memcpy(ucArrMenPai, ucArrMenPai1, sizeof(ucArrMenPai));
	int num = 26;
	if(station == 0||station ==2)
	{
		num = 28;
	}
    for (int i=0; i<num; ++i)
    {
        int nCurIndex = i;
        if (nCurIndex%2==1)
        {            
            if (    255==ucArrMenPai[nCurIndex]        // ���������棬����
                &&  255!=ucArrMenPai[nCurIndex-1]      // ���������棬����
                )
            {
                // ���������Ժ����ԣ��������ƣ��������ƣ�����Ӧ�ý���                
                ucArrMenPai[nCurIndex] = ucArrMenPai[nCurIndex-1];
                ucArrMenPai[nCurIndex-1] = 255;
            }        
        }
    }

	if(station == 0||station ==2)
	{
		int count = 0;
		for (int i=0; i<num; ++i)        // ��������
		{   
			if (255==ucArrMenPai[i])    // ����
			{
				SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[bViewStation], SetWallTile(-1, ucChangePos[i]));
			}
			else if (ucArrMenPai[i]>0 && ucArrMenPai[i]<38 && ucArrMenPai[i]%10!=0)
			{
				SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[bViewStation], SetWallTile(ucArrMenPai[i], ucChangePos[i]));
			}
			else if (0==ucArrMenPai[i])//�Ʊ�
			{
				SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[bViewStation], SetWallTile(0, ucChangePos[i]));
				count++;
			}
		}
	}
	else
	{
		int count = 0;
		for (int i=0; i<num; ++i)        // ��������
		{   
			if (255==ucArrMenPai[i])    // ����
			{
				SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[bViewStation], SetWallTile(-1, ucChangePos2[i]));
			}
			else if (ucArrMenPai[i]>0 && ucArrMenPai[i]<38 && ucArrMenPai[i]%10!=0)
			{
				SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[bViewStation], SetWallTile(ucArrMenPai[i], ucChangePos2[i]));
			}
			else if (0==ucArrMenPai[i])//�Ʊ�
			{
				SAFE_CTRL_OPERATE(IMjWallCtrl, nCtrl[bViewStation], SetWallTile(0, ucChangePos2[i]));
				count++;
			}
		}
	}
}

// ��ʾׯ�ұ�־
void CNewGameClient::UIShowZhuang(BYTE bViewStation,bool bShow)
{
    // �ؼ�ID
    static int nCtrl[PLAY_COUNT] = {CTN_1_IMG_101039 ,CTN_1_IMG_101040 ,CTN_1_IMG_101041 , CTN_1_IMG_101038 };
    if (bViewStation>PLAY_COUNT)
    {
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            SAFE_CTRL_OPERATE(IImage, nCtrl[i], SetControlVisible(bShow));
        }
        return;
    }
    SAFE_CTRL_OPERATE(IImage, nCtrl[bViewStation], SetControlVisible(bShow));
	if(bShow)
	{
		SAFE_CTRL_OPERATE(IImage, nCtrl[0], SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, nCtrl[1], SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, nCtrl[2], SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, nCtrl[3], SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, nCtrl[bViewStation], SetControlVisible(true));
	}
}

// ������������(������������)
void CNewGameClient::UISetHandCard(BYTE bViewStation, int nHandInfo[], int nCount, bool bShowGive )
{
    // �ؼ�ID
    static int nCtrl[PLAY_COUNT] = {CTN_1_MjHandCtrl_101018 ,CTN_1_MjHandCtrl_101019 , CTN_1_MjHandCtrl_600010 , CTN_1_MjHandCtrl_101021  };
    if (bViewStation>PLAY_COUNT || nCount<=0)
    {
        return;
    }
    if (NULL==nHandInfo || 0==nCount || nCount>MAX_HAND_TILE || nCount<0)
    {
        SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation], SetControlVisible(false));
        return;
    }

    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation], SetHandInfo(nHandInfo, nCount));
    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation], SetControlVisible(true));
    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation], SetGiveTileMode(bShowGive));
  //���ó�������
    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation], ClearAllBlock());
    for (int i=0; i<m_UserDataCard[bViewStation].m_nCPGCount; ++i)
    {
		if(m_UserDataCard[bViewStation].m_bIsBigGang[i])
		{
			SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation], AddOneSpecQuad(m_UserDataCard[bViewStation].m_ucSpeCPGData[i], m_UserDataCard[bViewStation].m_ucSpeCPGNum[i]));
		}
		else
		{
			SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation], AddOneBlock(m_UserDataCard[bViewStation].m_CPGtype[i], m_UserDataCard[bViewStation].m_ucCPGData[i], m_UserDataCard[bViewStation].m_ucChair[i]));
		}
    }	
}
// ���ó���
void CNewGameClient::UISetOutCard(BYTE bViewStation, int nOutInfo[], int nCount, bool bShowGive)
{
    // �ؼ�ID
    static int nCtrl[PLAY_COUNT] = {CTN_1_MjGiveCtrl_101022, CTN_1_MjGiveCtrl_101023,CTN_1_MjGiveCtrl_101024,CTN_1_MjGiveCtrl_101025 };
    if (bViewStation>PLAY_COUNT)
    {
        return;
    }

    if (NULL==nOutInfo || 0==nCount || nCount>OUT_CARD_NUM || nCount<0)
    {
        SAFE_CTRL_OPERATE(IMjGiveCtrl, nCtrl[bViewStation], SetControlVisible(false));
        return;
    }

    SAFE_CTRL_OPERATE(IMjGiveCtrl, nCtrl[bViewStation], SetControlVisible(true));
    SAFE_CTRL_OPERATE(IMjGiveCtrl, nCtrl[bViewStation], SetGiveInfo(nOutInfo, nCount));
}
// ���Ӷ���
void CNewGameClient::UIShowsaizi(BYTE bViewStation, int nPoint1, int nPoint2)
{
	m_pGameFrame->SetTimer(TIME_HIDE_SEZI_ANI, 3000);
    int nIDSezi1[PLAY_COUNT] = {SHAIZI_UP_1, SHAIZI_RIGHT_1, SHAIZI_DOWN_1, SHAIZI_LEFT_1};
    int nIDSezi2[PLAY_COUNT] = {SHAIZI_UP_2, SHAIZI_RIGHT_2, SHAIZI_DOWN_2, SHAIZI_LEFT_2};
    if (bViewStation>=PLAY_COUNT)
    {   
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            IExAnimate *pAnimate1 = NULL;
            IExAnimate *pAnimate2 = NULL;
            GETCTRL(IExAnimate, pAnimate1, m_pUI, nIDSezi1[i]);
            GETCTRL(IExAnimate, pAnimate2, m_pUI, nIDSezi2[i]);
            if (NULL!=pAnimate1 && NULL!=pAnimate2)
            {
                pAnimate1->SetIsLoop(false);
                pAnimate1->SetPlayNum(1);
                pAnimate2->SetIsLoop(false);
                pAnimate2->SetPlayNum(1);
                pAnimate1->SetPlayState(false);
                pAnimate2->SetPlayState(false);
            }            
        }
        return;
    }
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        IExAnimate *pAnimate1 = NULL;
        IExAnimate *pAnimate2 = NULL;
        GETCTRL(IExAnimate, pAnimate1, m_pUI, nIDSezi1[i]);
        GETCTRL(IExAnimate, pAnimate2, m_pUI, nIDSezi2[i]);
        if (NULL!=pAnimate1 && NULL!=pAnimate2)
        {
			PlayNormalSound(SOUND_SEZI);
            pAnimate1->SetIsLoop(false);
            pAnimate1->SetPlayNum(1);
            pAnimate2->SetIsLoop(false);
            pAnimate2->SetPlayNum(1);
            if (i==bViewStation)
            {
				if(nPoint1>0 && nPoint1<=12)
				{
					pAnimate1->SetPlayState(true);
				}
				if(nPoint2>0 && nPoint2<=12)
				{
					pAnimate2->SetPlayState(true);
				}
            }
            else
            {
                pAnimate1->SetPlayState(false);
                pAnimate2->SetPlayState(false);
            }
        }
    }
}

// ������ʾ
void CNewGameClient::UIShowLastTileTip(BYTE bViewStation, bool bShow,BYTE pai)
{
    static int nCtrl[PLAY_COUNT] = {CTN_Last_Mj_0, CTN_Last_Mj_1, CTN_Last_Mj_2, CTN_Last_Mj_3};
	for(int i=0;i<PLAY_COUNT;++i)
	{
		SAFE_CTRL_OPERATE(IContain, nCtrl[i], SetControlVisible(false));//��ʾ����
	}
    SAFE_CTRL_OPERATE(IContain, nCtrl[bViewStation], SetControlVisible(bShow));//��ʾ����

    static int nMjCtrl[PLAY_COUNT] = {CTN_1_MjHandCtrl_101018 ,CTN_1_MjHandCtrl_101019 , CTN_1_MjHandCtrl_600010 , CTN_1_MjHandCtrl_101021  };
    if (bViewStation>PLAY_COUNT)
    {
        return;
    }
	int Out[1] = {0};
	Out[0] = pai;
    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation]+1, SetHandInfo(Out, 1));
    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation]+1, SetControlVisible(bShow));
    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation]+1, SetGiveTileMode(false));
}
// ��ʾ���ƿؼ�
void CNewGameClient::UIShowGangCtrl(bool bShow,int pai[] ,int pai2[])
{
   // �ؼ�ID
    static int nCtrl[2] = {CTN_1_MjGangCtrl_600011 ,CTN_1_MjGangCtrl_600012 };
    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[0], SetControlVisible(false));
    SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[1], SetControlVisible(false));
	if(!bShow || (pai == NULL && pai2 == NULL))
		return;
	int nCount = 0;
	if(pai[0] != 255 &&  pai != NULL)
	{
		for(int i=0;i<4;i++)
		{
			if(pai[i] != 255 && pai[i]!=0)
				nCount++;

		}
		SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[0], SetHandInfo(pai, nCount));
		SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[0], SetControlVisible(true));
	}

	if(pai2[0] != 255 && pai2 != NULL)
	{
		nCount = 0;
		for(int i=0;i<4;i++)
		{
			if(pai2[i] != 255 && pai2[i]!=0)
				nCount++;
		}
		SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[1], SetHandInfo(pai2, nCount));
		SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[1], SetControlVisible(true));
	}
}
// ��ʾ�Ը��ƿؼ�
void CNewGameClient::UIShowChiGangCtrl(bool bShow,bool chi)
{
	for(int i=0;i<4;++i)
	{
		//SAFE_CTRL_OPERATE(IMjHandCtrl, nCtrl[bViewStation], SetHandInfo(nHandInfo, nCount));
		SAFE_CTRL_OPERATE(IMjHandCtrl, Ctr_ChiGang0 + i, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IMjHandCtrl, Ctr_ChiGang0 + i, SetIsUseMouse(false));
	}
	if(!bShow)
		return;
	int nHandInfo[4];
	memset(nHandInfo,0,sizeof(nHandInfo));
	if(chi && m_GameData.T_CPGNotify[m_nDeskStation].bChi)//��ʾ����
	{
		for(int i=0;i<3;++i)
		{
			if(m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][0] == 255)
				break;
			memset(nHandInfo,0,sizeof(nHandInfo));
			nHandInfo[0] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][0];
			nHandInfo[1] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][1];
			nHandInfo[2] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][2];
			SAFE_CTRL_OPERATE(IMjHandCtrl, Ctr_ChiGang0 + i, SetHandInfo(nHandInfo, 3));
			SAFE_CTRL_OPERATE(IMjHandCtrl, Ctr_ChiGang0 + i, SetControlVisible(true));
		}
	}
	else if(m_GameData.T_CPGNotify[m_nDeskStation].bGang)//��ʾ����
	{
		for(int i=0;i<4;++i)
		{
			if(m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][0] == 255)
				break;
			memset(nHandInfo,0,sizeof(nHandInfo));
			nHandInfo[0] = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1];
			nHandInfo[1] = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1];
			nHandInfo[2] = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1];
			nHandInfo[3] = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1];
			SAFE_CTRL_OPERATE(IMjHandCtrl, Ctr_ChiGang0 + i, SetHandInfo(nHandInfo, 4));
			SAFE_CTRL_OPERATE(IMjHandCtrl, Ctr_ChiGang0 + i, SetControlVisible(true));
		}
	}
}

//��ʾ׼��
void CNewGameClient::UIShowReady(BYTE bViewStation, bool bShow )
{
    static const int nReadyID[PLAY_COUNT] = {READY_OPPO,READY_RIGH, READY_DOWN, READY_LEFT };
    if (bViewStation>=PLAY_COUNT)
    {        
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            SAFE_CTRL_OPERATE(IImage, nReadyID[i], SetControlVisible(false));
        }    
        return;
    }    
    SAFE_CTRL_OPERATE(IImage, nReadyID[bViewStation], SetControlVisible(bShow));  
}
//��ʾ�й�
void CNewGameClient::UIShowTrustee(bool bShow)
{
    static const int nTrusteeID[PLAY_COUNT] = {TRUSTEE_OPPO, TRUSTEE_RIGH, TRUSTEE_DOWN, TRUSTEE_LEFT };
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        SAFE_CTRL_OPERATE(IImage, nTrusteeID[i], SetControlVisible(false));
    }    
	if(!bShow)
		return;

	BYTE bViewStation = 255;
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		bViewStation = m_pGameFrame->DeskStation2View(i);
		if(m_UserData.m_bTuoGuan[i])//����й�
		{		
			SAFE_CTRL_OPERATE(IImage, nTrusteeID[bViewStation], SetControlVisible(true));
		}
	}
}
//��ʾ�ȴ�����
void CNewGameClient::UIShowWaiting(bool bShow)
{
    // ���صȴ���ͼƬ
    SAFE_CTRL_OPERATE(IImage, CTN_1000000_IMG_2000103, SetControlVisible(bShow));    
    //SAFE_CTRL_OPERATE(IExAnimate, EXANIMATE_WAITING, SetPlayState(bShow));
}
///��ʾ�ⶥ
void CNewGameClient::UIShowFengDing()
{
}
//��ʾ�ƶ������ؼ�
void CNewGameClient::UIShowZhuaMoveAction(POINT ptStart, POINT ptEnd, BYTE ucViewStatin, bool bShow)
{
    // �ƶ��ؼ�
    IContain *pCtn = NULL;
    if ( GETCTRL(IContain, pCtn, m_pUI, CONTAIN_ZHUAPAI_ACT) )
    {
        pCtn->SetControlVisible(bShow);
    }
    if (!bShow)
    {
        m_pGameFrame->KillTimer(TIME_ZHUAPAI);
        return;
    }
    IMjWallCtrl *pWall = NULL;
    if ( GETCTRL(IMjWallCtrl, pWall, m_pUI, ZHUAPAI_ACT_MJWALL) )
    {
        if (ucViewStatin<PLAY_COUNT)
        {
            pWall->SetViewDirection(ucViewStatin);
        }
        pWall->FillWallBack(1);
        pWall->SetControlVisible(true);
    }
    IMoveAction *pMvact = NULL;
    GETCTRL(IMoveAction, pMvact, m_pUI, ZHUAPAI_ACT_MVACT);
    if (pMvact)
    {                                    
        pMvact->SetStartP(ptStart);
        pMvact->SetEndP(ptEnd);
        pMvact->SetPlayState(true);
    }
}

// ��ʾȱ��
void CNewGameClient::UIShowQueMen(bool show,BYTE station )
{
	SAFE_CTRL_OPERATE(IImage, QUE_MEN_0, SetControlVisible(false));///<�Լ�ȱ��  
	SAFE_CTRL_OPERATE(IImage, QUE_MEN_1, SetControlVisible(false));//<�ұ����ȱ��
	SAFE_CTRL_OPERATE(IImage, QUE_MEN_2, SetControlVisible(false)); ///<�Լ���ȱ��  
	SAFE_CTRL_OPERATE(IImage, QUE_MEN_3, SetControlVisible(false));///<������ȱ��  
	if(station == this->m_nDeskStation)
	{
		IImage *pHandImg = NULL;
		if(m_UserData.m_byDingQue[m_nDeskStation] < 0 || m_UserData.m_byDingQue[m_nDeskStation] > 3)
				return;
		BYTE viewstation = m_pGameFrame->DeskStation2View(m_nDeskStation);//�����ͼλ��
		GETCTRL(IImage, pHandImg, m_pUI, QUE_MEN_0 + viewstation);
		if (NULL!=pHandImg)
		{
			pHandImg->SetSrcX(82*m_UserData.m_byDingQue[m_nDeskStation]);
			pHandImg->SetControlVisible(true);//
		}
		return;
	}
	if(show)
	{
		IImage *pHandImg = NULL;
		for(int i=0;i<PLAY_COUNT;++i)
		{
			if(m_UserData.m_byDingQue[i] < 0 || m_UserData.m_byDingQue[i] > 3)
				continue;
			BYTE viewstation = m_pGameFrame->DeskStation2View(i);//�����ͼλ��

			GETCTRL(IImage, pHandImg, m_pUI, QUE_MEN_0 + viewstation);
			if (NULL!=pHandImg)
			{
				pHandImg->SetSrcX(82*m_UserData.m_byDingQue[i]);
				pHandImg->SetControlVisible(true);//
			}
		}
	}
}

//��ʾ�η�����Ի���
void CNewGameClient::ShowGFXY(bool show,BYTE station ,BYTE bestation,BYTE type)
{
	SAFE_CTRL_OPERATE(IContain, CTN_GFXY_DLG, SetControlVisible(false));
	IText *pTextGfxy = NULL;
	wchar_t wszWho[300]=L"";
	if(show && station != 255 && type != 255)
	{	
		int fen[PLAY_COUNT];
		memset(fen,0,sizeof(fen));
		//memcpy(fen,m_GameData.T_GangPai.iFen,sizeof(m_GameData.T_GangPai.iFen));

		SAFE_CTRL_OPERATE(IContain, CTN_GFXY_DLG, SetControlVisible(true));

		SAFE_CTRL_OPERATE(IText, GFXY_Gang, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi1, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IText, GFXY_BeGang, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi2, SetControlVisible(false));

		switch(type)
		{
		case ACTION_AN_GANG://����
			{
				::swprintf_s(wszWho, L" %s", m_wszNiCheng[station]);//�������
				SAFE_CTRL_OPERATE(IText, GFXY_Gang, SetText(wszWho));
				SAFE_CTRL_OPERATE(IText, GFXY_Gang, SetControlVisible(true));
				SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi1, SetText(L"����"));
				SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi1, SetControlVisible(true));
			}
			break;
		case ACTION_BU_GANG://����
			{
				::swprintf_s(wszWho, L" %s", m_wszNiCheng[station]);//�������
				SAFE_CTRL_OPERATE(IText, GFXY_Gang, SetText(wszWho));
				SAFE_CTRL_OPERATE(IText, GFXY_Gang, SetControlVisible(true));
				SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi1, SetText(L"�η�"));
				SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi1, SetControlVisible(true));
			}
			break;
		case ACTION_MING_GANG://����
			{
				::swprintf_s(wszWho, L" %s", m_wszNiCheng[station]);//�������
				SAFE_CTRL_OPERATE(IText, GFXY_Gang, SetText(wszWho));
				SAFE_CTRL_OPERATE(IText, GFXY_Gang, SetControlVisible(true));
				SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi1, SetText(L"����"));
				::swprintf_s(wszWho, L" %s", m_wszNiCheng[bestation]);//���������
				SAFE_CTRL_OPERATE(IText, GFXY_BeGang, SetText(wszWho));
				SAFE_CTRL_OPERATE(IText, GFXY_BeGang, SetControlVisible(true));
				SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi2, SetText(L"����"));
				SAFE_CTRL_OPERATE(IText, GFXY_BiaoTi2, SetControlVisible(true));
			}
			break;
		}
		for(int i=0; i<PLAY_COUNT; ++i)
		{
			::swprintf_s(wszWho, L" %s ��", m_wszNiCheng[i]);
			GETCTRL(IText, pTextGfxy, m_pUI, GFXY_USER1 + i);
			if (pTextGfxy)
			{
				pTextGfxy->SetText(wszWho);
			}
			//GlbGetFloatString(szText,m_GameData.T_GangPai.iGangFen[i]);
			//MultiByteToWideChar(CP_ACP,0,szText,-1,wszFen,256);
			::swprintf_s(wszWho, L" %d",m_GameData.T_GangPai.iGangFen[i]);
			GETCTRL(IText, pTextGfxy, m_pUI, GFXY_FEN1 + i);
			if (pTextGfxy)
			{
				pTextGfxy->SetText(wszWho);
				pTextGfxy->SetControlVisible(true);
			}
		}
	}
}
// �������                
void CNewGameClient::OnUIRequestOutCard(UINT32 uControlID,bool outtime )
{
   // ��ʾ���������
    int nUpTile = 0;
    BYTE ucLogicTile = 0;

    IMjHandCtrl *pHand;
    GETCTRL(IMjHandCtrl, pHand, m_pUI, uControlID);
    if (NULL!=pHand)
    {
        nUpTile = pHand->GetUpTile();
    }    
    ucLogicTile = nUpTile;
	if(m_ucHandState == 2)//����״̬
	{
		BYTE byType = 255; //���Ƶ�����
		int count = 0;
		for(int i=0;i<4;i++)
		{
			if(m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1] == ucLogicTile)
			{
				byType = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][0];
				break;
			}
		}
		tagGangPaiEx gang;
		gang.Clear();
		gang.byPs = ucLogicTile;
		gang.byType = byType;
		gang.byUser = m_nDeskStation;
		gang.byBeGang = m_ucCurrentPlayer;
		SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
		// �����������
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
		m_ucHandState = 0xFF;// �����
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL, 0));
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
		//ȡ����ɫ״̬
		return;
	}

	if(outtime)//��ʱ����
	{
		ucLogicTile = m_UserData.m_byLastZhuaPai[m_nDeskStation];
		if(ucLogicTile == 255)
		{
			ucLogicTile = m_UserData.GetLastHandPai(m_nDeskStation);
		}
		if(m_UserData.IsHaveQueMen(m_nDeskStation,m_UserData.m_byDingQue[m_nDeskStation]) && ucLogicTile/10 != m_UserData.m_byDingQue[m_nDeskStation])//����ȱ�ŵ���
		{
			for(int i=0;i<17;++i)
			{
				if(m_UserData.m_byArHandPai[m_nDeskStation][i]/10 == m_UserData.m_byDingQue[m_nDeskStation])
				{
					ucLogicTile = m_UserData.m_byArHandPai[m_nDeskStation][i];
					break;
				}
			}
		}
		RequestOutCard(ucLogicTile);
		OutputDebugString("�ͻ��ˣ��¼� OnUIRequestOutCard 111 ");
		return;
	}

	if( m_UserData.m_bTuoGuan[m_nDeskStation])
	{
		if(m_UserData.m_byLastZhuaPai[m_nDeskStation] != 255)
		{
			RequestOutCard(m_UserData.m_byLastZhuaPai[m_nDeskStation]);
		}
		else
		{
			RequestOutCard(m_UserData.GetLastHandPai(m_nDeskStation));
		}
		OutputDebugString("�ͻ��ˣ��¼� OnUIRequestOutCard 222 ");
		return;
	}

    if (0!=ucLogicTile)
    {
        RequestOutCard(ucLogicTile);    
    }
    else
    {
		if(m_UserData.m_byLastZhuaPai[m_nDeskStation] != 255)
		{
			RequestOutCard(m_UserData.m_byLastZhuaPai[m_nDeskStation]);
		}
		else
		{
			RequestOutCard(m_UserData.GetLastHandPai(m_nDeskStation));
		}
    }
}
// ��������
void CNewGameClient::OnRequestBlock(UINT32 uControlID)
{
    switch(uControlID)
    {
    case CTN_1_CTN_600000_BTN_600001:
        {            
            static IButton *pBtn = NULL;
            if (NULL==pBtn)
            {
                GETCTRL(IButton, pBtn, m_pUI, CTN_1_CTN_600000_BTN_600001);
                if (!pBtn->GetEnable())
                {
                    return;
                }
            }
            // �������
            RequestBlockChi();
        }
        break;
    case CTN_1_CTN_600000_BTN_600002:
        {
            static IButton *pBtn = NULL;
            if (NULL==pBtn)
            {
                GETCTRL(IButton, pBtn, m_pUI, CTN_1_CTN_600000_BTN_600002);
                if (!pBtn->GetEnable())
                {
                    return;
                }
            }
            // ��������
            RequestBlockPeng();
        }
        break;
    case CTN_1_CTN_600000_BTN_600003:
        {
            static IButton *pBtn = NULL;
            if (NULL==pBtn)
            {
                GETCTRL(IButton, pBtn, m_pUI, CTN_1_CTN_600000_BTN_600003);
                if (!pBtn->GetEnable())
                {
                    return;
                }
            }
            // �������
            RequestBlockGang();
        }
        break;
    case CTN_1_CTN_600000_BTN_600004:        
        {
            static IButton *pBtn = NULL;
            if (NULL==pBtn)
            {
                GETCTRL(IButton, pBtn, m_pUI, CTN_1_CTN_600000_BTN_600004);
                if (!pBtn->GetEnable())
                {
                    return;
                }
            }
            // �������
            RequestBlockHu();
        }
        break;
    case CTN_1_CTN_600000_BTN_600005:
        {
            static IButton *pBtn = NULL;
            if (NULL==pBtn)
            {
                GETCTRL(IButton, pBtn, m_pUI, CTN_1_CTN_600000_BTN_600005);
                if (!pBtn->GetEnable())
                {
                    return;
                }
            }
            // ��������
            RequestCancelBlock();
        }
        break;
    default:
        break;    
    }
}

// ���齫�ؼ�������ʱ������ƶ����ؼ���
int CNewGameClient::OnMoveOnMjCtrl(TUIMessage* pMessage)
{
   if (pMessage->_uControlID == CTN_1_MjHandCtrl_600010)
    {       
		if (2==m_ucHandState)  // �������״̬
        {
			IMjHandCtrl *pHand = NULL;
            GETCTRL(IMjHandCtrl, pHand, m_pUI, CTN_1_MjHandCtrl_600010);
            if (NULL!=pHand)
            {
                MJ_TILE_UIINFO uiInfo[MAX_TILES_INCTRL];
                pHand->GetTilesInfo(uiInfo, MAX_TILES_INCTRL);
                // ��ѡ�еĵ��� 
                for (int i=0; i<pHand->GetTileCount(); ++i)
                {   
                    if (LT_EMPTY==uiInfo[i].ucActFlag && uiInfo[i].ucTiles[0]==pMessage->_ch )
                    {
                        uiInfo[i].bSelect = true;
                    }
                    else
                    {
                        uiInfo[i].bSelect = false;
                    }        
                }
                pHand->SetTilesInfo(uiInfo, pHand->GetTileCount());
            }
        }
    }
	return 0;
}
// ���齫�ؼ�������ʱ����굥���ؼ�
int CNewGameClient::OnLButtonOnMjCtrl(TUIMessage* pMessage)
{
	if(pMessage->_uControlID == CTN_1_MjGangCtrl_600011)//���ƿؼ�1
	{
		if(m_GangType[0] == 255)//�ж��ֺ��ƣ���������
			return 0;
        tagGangPaiEx gang;
        gang.Clear();
		gang.bIsBigGang = m_bIsBigGang[0];
        gang.byPs = m_byCheckPai;
        gang.byType = m_GangType[0];
		memcpy(gang.byGangPai,m_GangPai[0],sizeof(gang.byGangPai));
        gang.byUser = m_nDeskStation;
        gang.byBeGang = m_ucCurrentPlayer;
        SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
        // �����������
        SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
        // �����
        SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL, 0));
        SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
        m_ucHandState = 0xFF;//����״̬
		UIShowGangCtrl(false);
		return 0;
	}
	
    if (pMessage->_uControlID!=CTN_1_MjHandCtrl_600010)
    {
		if (2==m_ucHandState)  // �������״̬
		{
			BYTE type = 255;
			BYTE byPs = pMessage->_ch;
			for(int i=0;i<4;++i)
			{
				if(byPs == m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1])
				{
					type = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][0];
				}
			}
			tagGangPaiEx gang;
			gang.Clear();
			gang.byPs = byPs;
			gang.byType = type;
			memset(gang.byGangPai,byPs,sizeof(gang.byGangPai));
			gang.byUser = m_nDeskStation;
			gang.byBeGang = m_ucCurrentPlayer;
			SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
			// �����������
			SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
			// �����
			SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL, 0));
			SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
			m_ucHandState = 0xFF;//����״̬
		}
        return 0;
    }
	return 0;
}
// �����ؼ���Ϣ
int CNewGameClient::OnCartoonFinish(TUIMessage *pMessage)
{
     IExAnimate *pAnimate = NULL;
    GETCTRL(IExAnimate, pAnimate, m_pUI, pMessage->_uControlID);
    if (NULL==pAnimate)
    {
        return 0;
    }
    static const int nIDSezi1[PLAY_COUNT] = {SHAIZI_UP_1, SHAIZI_RIGHT_1, SHAIZI_DOWN_1, SHAIZI_LEFT_1};
    static const int nIDSezi2[PLAY_COUNT] = {SHAIZI_UP_2, SHAIZI_RIGHT_2, SHAIZI_DOWN_2, SHAIZI_LEFT_2};
    int nWhichsezi = 0; // �ǵڼ���ɫ��
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        if (nIDSezi1[i]==pMessage->_uControlID)
        {
            nWhichsezi = 1;
            break;
        }
        if (nIDSezi2[i]==pMessage->_uControlID)
        {
            nWhichsezi = 2;
            break;
        }
    }
	switch(m_GameData.m_byThingDoing)
	{
	case THING_2SEZI_DIR_GP://��2��ɫ���¼������Ʒ����¼�
		{
            if (1==nWhichsezi)
            {
                pAnimate->SetShowFrame(14+m_GameData.T_TwoSeziGetPai.bySezi0);
                pAnimate->SetGameXY(pAnimate->GetEnd().x, pAnimate->GetEnd().y);
            }
            else if (2==nWhichsezi)
            {
                pAnimate->SetShowFrame(14+m_GameData.T_TwoSeziGetPai.bySezi1);
                pAnimate->SetGameXY(pAnimate->GetEnd().x, pAnimate->GetEnd().y);
            }   
			OutputDebugString(TEXT("�齫-�¼�������ɫ�ӽ�����Ϣ   THING_2SEZI_DIR_GP"));
			//����ɫ�ӽ�����Ϣ
			SendGameDataEx(ASS_SEZIFINISH, NULL,0);  
		}
		break;
	case THING_2SEZI_GP://��2��ɫ���¼������ƶ����¼�
		{
            if (1==nWhichsezi)
            {
                DebugPrintf("����ɫ�� 1 ���� %d �ؼ�id %d",m_GameData.T_TwoSeziGetPai.bySezi0, pMessage->_uControlID);
                pAnimate->SetShowFrame(14+m_GameData.T_TwoSeziGetPai.bySezi0);    
                pAnimate->SetGameXY(pAnimate->GetEnd().x, pAnimate->GetEnd().y);
            }
            else if (2==nWhichsezi)
            {
                DebugPrintf("����ɫ�� 2 ���� %d �ؼ�id %d", m_GameData.T_TwoSeziGetPai.bySezi1, pMessage->_uControlID);
                pAnimate->SetShowFrame(14+m_GameData.T_TwoSeziGetPai.bySezi1);
                pAnimate->SetGameXY(pAnimate->GetEnd().x, pAnimate->GetEnd().y);
            }
			OutputDebugString(TEXT("�齫-���ƣ�����ɫ�ӽ�����Ϣ   THING_2SEZI_GP"));
			//����ɫ�ӽ�����Ϣ
			SendGameDataEx(ASS_SEZIFINISH, NULL,0);  
		}
		break;
	}
	return 0;
}
//���Ŷ���
void CNewGameClient::PlayAni(int Id)
{
    IExAnimate *pAni = NULL;
    GETCTRL(IExAnimate, pAni, m_pUI, Id);
    if (NULL!=pAni)
    {
        pAni->SetPlayNum(1);                    
        pAni->SetIsLoop(false);
        pAni->SetPlayState(true);                    
    }
}
//������ҵĵ��ǳ�
void CNewGameClient::GetUserName()
{
	UserInfoForGame UserInfo;
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		if(m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,m_wszNiCheng[i],256);
		}
	}
}


//��ʾ��ʼ����״̬����
void CNewGameClient::UIshowBeginOut()
{
	
}


//���ü�ʱ��
void CNewGameClient::OnSetTimer(BYTE station,int type)
{
	OnKillAllTimer();
	BYTE viewstation = m_pGameFrame->DeskStation2View(station);//�����ͼλ��
	int itime = 1;
	switch(type)
	{
	case TIME_OUT_CARD://���ƶ�ʱ��
		{
			itime = m_MJSetting.ucOutcardTime;
			if(station == m_nDeskStation && (m_UserData.m_bTuoGuan[station] || m_UserData.m_bTing[station]))//�Լ����������й�״̬
			{
				itime = m_MJSetting.nAutoTime;
			}
		}
		break;
	case TIME_BLOCK_CARD://���ƶ�ʱ��
		{
			itime = m_MJSetting.ucBlockTime;
			if(station == m_nDeskStation && m_UserData.m_bTuoGuan[station] )//�Լ����������й�״̬
			{
				itime = m_MJSetting.nAutoTime;
			}
		}
		break;
	case TIME_BEGIN:// �Զ���ʼ
		{
			itime = m_MJSetting.ucBeginTime;
		}
		break;
	case TIME_DING_QUE://��ȱ�ż�ʱ��
		{
			itime = m_MJSetting.ucDingQueTime;
		}
		break;
	}
	m_pGameFrame->SetTimer(type, itime*1000);
    UIShowTimer(viewstation, itime, true);//��ʾ��ʱ������
}
//ɾ������ʱ��
void CNewGameClient::OnKillAllTimer()
{
    UIShowTimer(0, 0, false);//���ؼ�ʱ������
	m_pGameFrame->KillTimer(TIME_OUT_CARD);
	m_pGameFrame->KillTimer(TIME_BLOCK_CARD);
	m_pGameFrame->KillTimer(TIME_BEGIN);
}
/// ������ʱ��
BOOL CNewGameClient::KillGameTimer(int TimerID)
{
	m_pGameFrame->KillTimer(TimerID);
	m_pGameFrame->KillTimer(ID_WARMING_TIME);//ɾ�������ʱ��
	UIShowTimer(0,0,false);//���ؼ�ʱ��
	return FALSE;
}

// ���ö�ʱ��
void CNewGameClient::UIShowTimer(BYTE bViewStation, int nTime, bool bShow )
{
    // ����ؼ�ID
    static int nCtrl[PLAY_COUNT] = {CTN_1_CTN_600031_IMG_600020  , CTN_1_CTN_600031_IMG_600020+1  , CTN_1_CTN_600031_IMG_600020+2  , CTN_1_CTN_600031_IMG_600020+3  };
    if (!bShow)
    {
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            SAFE_CTRL_OPERATE(IImage, nCtrl[i], SetControlVisible(false) );
        }
        SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600031, SetControlVisible(bShow));
        return;
    }
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetControlVisible(true));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetShowMaxNum(nTime));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetShowMinNum(1));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetPLay(true));
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        SAFE_CTRL_OPERATE(IImage, nCtrl[i], SetControlVisible(i==bViewStation));
    }
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600031, SetControlVisible(bShow));
}


//������Ϸ����:����֪ͨ��Ϣ������������
void CNewGameClient::SendGameDataEx(BYTE id)
{
	m_pGameFrame->SendGameData(id, NULL, 0);
}

//������Ϸ����
void CNewGameClient::SendGameDataEx(BYTE id,void *pData,UINT uSize)
{
	m_pGameFrame->SendGameData( id,pData, uSize);
}

//��˸����
void CNewGameClient::FlashWindow()
{
	HWND hwnd = m_pUI->GetWindowsHwnd();
	FLASHWINFO info;
	info.cbSize = sizeof(FLASHWINFO);
	info.dwFlags = FLASHW_TRAY|FLASHW_TIMERNOFG;
	info.hwnd = hwnd;
	info.uCount = 100;
	info.dwTimeout = 500;
	FlashWindowEx(&info);
}

//������췢��UI��Ⱦ
void CNewGameClient::ShowTalk(BYTE bDeskstation,wchar_t* pwszText,int iDispMillSeconds/* = 5000*/)
{
	BYTE nView = m_pGameFrame->DeskStation2View(bDeskstation);
    IText *pText;
    GETCTRL(IText, pText, m_pUI, CTN_1000000_TEXT_2000361 + nView);
    if (NULL!=pText)
    {
		pText->SetText(pwszText);
		SAFE_CTRL_OPERATE(IImage, CTN_1000000_IMG_2000351 + nView, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IText, CTN_1000000_TEXT_2000361 + nView, SetControlVisible(true));
    } 
	m_pGameFrame->SetTimer(TIME_TALKING_0 + nView,iDispMillSeconds);
}

//������췢��UI��Ⱦ����
void CNewGameClient::ShowTalkFinish(BYTE bViewstation)
{
	//����������ʾ
	SAFE_CTRL_OPERATE(IImage, CTN_1000000_IMG_2000351 + bViewstation, SetControlVisible(false));
	SAFE_CTRL_OPERATE(IText, CTN_1000000_TEXT_2000361 + bViewstation, SetControlVisible(false));
}

//�������Žӿ�

/** @brief �������ļ��ж�ȡ�����ļ���
*/
void CNewGameClient::GetSoundMapFromCfg()
{
    CString s = CINIFile::GetAppPath ();//����·��   
#ifdef HAOHUA //��񳤴��齫
    s = "/music/sound.ini";
#else
    s = s+"/music/sound.ini";
#endif

    CINIFile file(s);
    CString strSec=TEXT("ccmj");
    CString oss;
    // ��(1~9)
    for (int i=1; i<10; ++i)
    {
        SOUND_INFO info;
        int &nCount = info.nCount;
        nCount = 0;
        for (int j=0; j<MAX_SOUND_RAND; ++j)
        {
			oss.Format("%dwan%d",i,j);
            CString strVal = file.GetKeyVal(strSec, oss, TEXT("NULL"));
            oss="";
            if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL")))==0)
            {
                break;
            }
            _tcscpy(info.szFileName[nCount++], strVal.GetBuffer());
        }        
        m_SoundMap[i] = info;
    }
	// ��(11~19)
    for (int i=1; i<10; ++i)
    {
        SOUND_INFO info;
        int &nCount = info.nCount;
        nCount = 0;
        for (int j=0; j<MAX_SOUND_RAND; ++j)
        {
			oss.Format("%dtiao%d",i,j);
            CString strVal = file.GetKeyVal(strSec, oss, TEXT("NULL"));
            oss="";
            if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL")))==0)
            {
                break;
            }
            _tcscpy(info.szFileName[nCount++], strVal.GetBuffer());       
        }        
        m_SoundMap[10+i] = info;
    }
    // Ͳ(21~29)
    for (int i=1; i<10; ++i)
    {
        SOUND_INFO info;
        int &nCount = info.nCount;
        nCount = 0;
        for (int j=0; j<MAX_SOUND_RAND; ++j)
        {
			oss.Format("%dtong%d",i,j);
            CString strVal = file.GetKeyVal(strSec, oss, TEXT("NULL"));
            oss="";
            if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL")))==0)
            {
                break;
            }
            _tcscpy(info.szFileName[nCount++], strVal.GetBuffer());
        }        
        m_SoundMap[20+i] = info;
    }
    // ���������з���(31~37)
    CString strKey[7] = {TEXT("dong"),TEXT("nan"),TEXT("xi"),TEXT("bei"),TEXT("zhong"),TEXT("fa"),TEXT("bai")};
    for (int i=0; i<7; ++i)
    {
        SOUND_INFO info;
        int &nCount = info.nCount;
        nCount = 0;
        for (int j=0; j<MAX_SOUND_RAND; ++j)
        {
			oss.Format("%s%d",strKey[i],j);
            CString strVal = file.GetKeyVal(strSec, oss, TEXT("NULL"));
            oss="";
            if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL")))==0)
            {
                break;
            }
            _tcscpy(info.szFileName[nCount++], strVal.GetBuffer());
        }        
        m_SoundMap[31+i] = info;
    }
    //�����η磬���꣬���������ڣ��ܺ��� (101~106)
    CString strKeyBlock[6] = {TEXT("peng"),TEXT("guafeng"),TEXT("xiayu"),TEXT("zimo"),TEXT("dianpao"),TEXT("gangpao")};
    for (int i=0; i<6; ++i)
    {
        SOUND_INFO info;
        int &nCount = info.nCount;
        nCount = 0;
        for (int j=0; j<MAX_SOUND_RAND; ++j)
        {
			oss.Format("%s%d",strKeyBlock[i],j);
            CString strVal = file.GetKeyVal(strSec, oss, TEXT("NULL"));
            oss="";
            if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL")))==0)
            {
                break;
            }
            _tcscpy(info.szFileName[nCount++], strVal.GetBuffer()); 
        }        
        m_SoundMap[SOUND_PENG+i] = info;
    }
}

/** @brief ���ų�������
 */
void CNewGameClient::PlayTileSound(int nDeskStation,BYTE ucTile)
{
    int nCount = m_SoundMap[ucTile].nCount;
    if (0==nCount)
    {
        return;
    }
    int nSel = rand()%nCount;
	TCHAR szPath[MAX_PATH];
	//�õ�˵���е��Ա�
	TCHAR szSex[16];
	UserInfoForGame user;
	if (m_pGameFrame->GetUserInfo(nDeskStation,user))
	{
		if (user.bBoy)
		{
			::sprintf(szPath,"music\\putong\\man\\%s",m_SoundMap[ucTile].szFileName[nSel]);
		}
		else
		{
			::sprintf(szPath,"music\\putong\\wom\\%s",m_SoundMap[ucTile].szFileName[nSel]);
		}
	}
	if (m_pUIGameSound != NULL && m_bGameSound)
	{
		m_pUIGameSound->play2D(szPath);
	}
}

/**
* @brief ������ͨ�������������ԣ�
*/
void CNewGameClient::PlayNormalSound(const char *pszFileName)
{
	TCHAR szPath[MAX_PATH];
	::sprintf(szPath,"music\\%s",pszFileName);
	if (m_pUIGameSound != NULL && m_bGameSound)
	{
		m_pUIGameSound->play2D(szPath);
	}
}

//������Ϸ����
void CNewGameClient::PlayBkMusic(bool bOn)
{
	if (!bOn)
	{
		m_pUIBkMusic->stopAllSounds();
		return;
	}
	TCHAR szFileName[MAX_PATH];
	::sprintf(szFileName,"Music//bg.ogg");
	if (m_pUIBkMusic)
	{
		m_pUIBkMusic->play2D(szFileName,true);
	}
	return;
}

///���ƿ���
//���ַ���ת����������
void CNewGameClient::GetPaiFormChar(int &count ,BYTE data[],const wchar_t pszPai[],int type)
{
	memset(data,255,sizeof(data));
	count=0;
	switch(type)
	{
	case 0://��ȫ������
		{
			for(int i=2;i<MAX_HAND_PAI*2 + 2;i+=2)
			{
				if(pszPai[i] == 0)
					break;
				data[i/2-1] = ChangeChar(pszPai[i],pszPai[i+1]);
				count++;
			}
		}
		break;
	case 1://�����ƻ���ĳЩ��
		{
			for(int i=0;i<MAX_HAND_PAI*2;i+=2)
			{
				if(pszPai[i] == 0)
					break;
				data[i/2] = ChangeChar(pszPai[i],pszPai[i+1]);
				count++;
			}
		}
		break;
	case 2://������һ��ץ��
		{
			data[0] = ChangeChar(pszPai[2],pszPai[3]);
			count = 1;
		}
		break;
	}
}
//�������ַ�ת������
BYTE CNewGameClient::ChangeChar(wchar_t data1,wchar_t data2)
{
	BYTE pai = 255;
	BYTE num1 = 0,num2 = 0;

	if(data2 == 'b')//ͬ
	{
		num2 = 20;
	}
	else if(data2 == 't')//��
	{
		num2 = 10;
	}
	if(data1 >= '1' && data1<='9' && (data2 == 'w' || data2 == 'b' || data2 == 't'))
	{
		num1 = data1 - '1' +1;
		pai = num1+num2;
	}
	else if(data1 == 'd' && data2=='f')//����
	{
		pai = 31;
	}
	else if(data1 == 'n' && data2=='f')//�Ϸ�
	{
		pai = 32;
	}
	else if(data1 == 'x' && data2=='f')//����
	{
		pai = 33;
	}
	else if(data1 == 'b' && data2=='f')//����
	{
		pai = 34;
	}
	else if(data1 == 'h' && data2=='z')//����
	{
		pai = 35;
	}
	else if(data1 == 'f' && data2=='c')//����
	{
		pai = 36;
	}
	else if(data1 == 'b' && data2=='b')//�װ�
	{
		pai = 37;
	}
	return pai;
}

//��ȡ�ؼ�����,��������������
void CNewGameClient::SendSuperData()
{	
}

//��ʾ�������س����ͻ��˷���
void CNewGameClient::ShowSuperSetPai(bool show)
{	
}


//������������á������Ӧ
void CNewGameClient::OnClickedConfig()
{
	//SetShowPromptBk(true);

	//���ظ������UI
	IRadioButton* pRadio = NULL;
	//��������
	GETCTRL(IRadioButton,pRadio,m_pUI,CTN_1000000_CTN_1003510_RButton_1003520);//
	if(pRadio != NULL)
	{
		pRadio->SetIsSelect(m_bBkMusic);
	}
	//�����Թۿ���
	if(pRadio != NULL)
	{
		GETCTRL(IRadioButton,pRadio,m_pUI,CTN_1000000_CTN_1003510_RButton_1003530);//
	}
	pRadio->SetIsSelect(m_bGameSound);
	//��ʾ�Ի�������
    SAFE_CTRL_OPERATE(IContain, CTN_1000000_CTN_1003510, SetControlVisible(true));
}

//������������á�ȷ����ť�����Ӧ
void CNewGameClient::OnClickedConfigOK()
{
	//���ضԻ�������
    SAFE_CTRL_OPERATE(IContain, CTN_1000000_CTN_1003510, SetControlVisible(false));
	//���ð�ť����
    SAFE_CTRL_OPERATE(IButton, CTN_1000000_BTN_1003500, SetEnable(true));

	//��ȡ��������Ŀ����
	IRadioButton* pRadio = NULL;
	//�������ֿ���
	GETCTRL(IRadioButton,pRadio,m_pUI,CTN_1000000_CTN_1003510_RButton_1003520);//
	if (pRadio != NULL)
	{
		bool bLast = m_bBkMusic;
		m_bBkMusic = pRadio->GetIsSelect();
		if (!bLast)
		{
			PlayBkMusic(m_bBkMusic);
		}
		else
		{
			if (!m_bBkMusic)
			{
				PlayBkMusic(false);
			}
		}
	}
	//��Ϸ��Ч����
	GETCTRL(IRadioButton,pRadio,m_pUI,CTN_1000000_CTN_1003510_RButton_1003530);//
	if (pRadio != NULL)
	{
		m_bGameSound = pRadio->GetIsSelect();
	}
	//SetShowPromptBk(false);
	return;
}

//������������á�ȡ����ť�����Ӧ
void CNewGameClient::OnClickedConfigCancel()
{
	//���ضԻ�������
    SAFE_CTRL_OPERATE(IContain, CTN_1000000_CTN_1003510, SetControlVisible(false));
	//SetShowPromptBk(false);
	return;
}

//������Ϸ����
void CNewGameClient::LoadGameConfig()
{
	CString strCfgName;
	strCfgName.Format("%s%d.bcf",CINIFile::GetAppPath(),NAME_ID);
	CBcfFile cfgFile(strCfgName);

	//��Ϸ�������ֿ�������
	m_bBkMusic = cfgFile.GetKeyVal("config","BkMusic",true);
	//��Ϸ��Ч��������
	m_bGameSound = cfgFile.GetKeyVal("config","GameSound",true);
	//���ܼ���
	//m_nAutoLevel = cfgFile.GetKeyVal("config","AutoLevel",0);
	cfgFile.CloseFile();

	return;
}

//������Ϸ����
void CNewGameClient::SaveGameConfig()
{
	CString strCfgName;
	strCfgName.Format("%s%d.bcf",CINIFile::GetAppPath(),NAME_ID);
	DWORD dwCfgFile = ::cfgOpenFile(strCfgName);

	if(dwCfgFile < 0x10) //�ļ���ʧ��
	{
		return;
	}
	//��Ϸ�������ֿ�������
	::cfgSetValue(dwCfgFile,"config","BkMusic",m_bBkMusic);
	//��Ϸ��Ч��������
	::cfgSetValue(dwCfgFile,"config","GameSound",m_bGameSound);
	::cfgClose(dwCfgFile);
	return;
}

//��ʼ������
void CNewGameClient::InitGameDate()
{   
    m_byNtLogicPos = 0xFF;                      //ׯ��
    m_ucCurrentPlayer = 0xFF;                   //��ֵ���
    m_ucLastTile = 0xFF;                        //�ϴγ�����
    m_ucHandState = 0xFF;                       //����״̬ 0����״̬ 1�����ѡ��״̬
    m_ucOpenDoor = 0xFF;
    m_ucOpenDun = 0xFF;
    m_nWhichTask = 0;
	///��ʼ��һЩ����	
	for (int i=0; i<PLAY_COUNT; ++i)
    {
        m_UserDataCard[i].ClearOut();
        m_UserDataCard[i].ClearCPG();
        m_UserDataCard[i].ClearHand();        
    }
    m_UserData.InitData();
    m_nTimeOutCnt = 0;
    memset(m_bIsTuoGuan, false, sizeof(m_bIsTuoGuan));
}

