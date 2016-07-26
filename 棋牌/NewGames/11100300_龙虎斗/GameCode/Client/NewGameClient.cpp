#include "stdafx.h"
#include "NewGameClient.h"
#include "../common/IGameFrame.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "GameControlsIDDefine.h"
#include "Common/UIMessageDefines.h"
#include "LoveSendClassInExe.h"

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
	
	//��ע��ť������
	SetBetBtEnable(false);
	//��ʾ����ʱ��
	SetImageVisible(BTN_FREE_TIME,false);

	m_dbyLuzi.clear();
	//��ׯ��ť������
	SetBtEnable(BTN_APPLY_XIAZHUANG,false);
	//��ׯ��ť����
	SetBtEnable(BTN_APPLY_MING,true);
	//��ׯ�б�����������ť
	SetBtEnable(BTN_BUTTON_LEFT,true);
	SetBtEnable(BTN_BUTTON_RIGHT,true);
	ResetGameData();
	
	return 0;
}
//---------------------------------------------------------------------------------------
//����UI���� 
void	CNewGameClient::ReSetUiDlag()
{
	//��ע��ť������
	SetBetBtEnable(false);
	
}
/// ��ҽ���
int CNewGameClient::GameUserCome(void)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation(); 
	ShowMyInfo(true);
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
	
	m_iMyDeskStation = 255;		///����Լ�����Ϸλ��
	m_byNtStation	 = 255;		//ׯ��λ��
	m_i64NtHaveWin	 = 0;		//ׯ����Ӯ���
	m_iNtPlayCount	 = 0;		//ׯ����ׯ����
	m_i64MeHaveWin	 = 0;		//�ҵ���Ӯ���
	m_bIsSuperUser	 = false;	//�Ƿ񳬼��û�
	
	m_bIsReady		 = false;	//�Ƿ�׼����
	m_bIsPlaying	 = false;	//��Ϸ�Ƿ��Ѿ���ʼ
	m_byGameStation	 = GS_WAIT_ARGEE;         ///��ǰ����Ϸ״̬
	
	m_iPlayBaShu=0;//�Լ��İ���
	m_iPlayJuShu=0;//�Լ��ľ���
	m_iPlayGrade=0;//�Լ��ĳɼ�
	m_byCardSort=0;
	m_byNtWaitListCounter=0;//��ׯ�б��ǩ
	SetImageVisible(CTN_1000000_IMG_10001330,false);
	SetImageVisible(CTN_1000000_IMG_10001331,false);
	m_bySelChouMaType = 255;		//��ǰѡ��ĳ�������
	
	memset(m_byLuziList,-1,sizeof(m_byLuziList));			//·��
	memset(m_i64AreaAllSum,0x00,sizeof(m_i64AreaAllSum));					//���������ֵ
	memset(m_i64RealityAreaAllSum,0x00,sizeof(m_i64RealityAreaAllSum));		//���������ע��ֵ

	//����������Ӧ���
	//SetContainEnable(BTN_APPLY_MING,true);
	for (int i=0; i<PLAY_COUNT; i++)
	{
		m_byWaitNtList[i] = 255;	//��ׯ�б�
	}
	m_bSoundIsClose=TRUE;										//�Ƿ�ر�����
	m_bUserExitIsClose=FALSE;										//�Ƿ�ر��û�������ʾ��Ϣ
	
}

//------------------------------------------------------------------------------
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	///��ť�������Ϣ 
	if(pMessage->_uMessageType == UI_LBUTTONUP)
	{
		switch(pMessage->_uControlID)
		{
		case  BTN_MONEY_YIBAI:	//100����
		case  BTN_MONEY_YIQIAN:	//1000����
		case  BTN_MONEY_YIWAN:	//10000����
		case  BTN_MONEY_SHIWAN:	//100000����
		case  BTN_XIAZHU_YIBAIWAN:	//1000000����
		case  BTN_MONEY_YIQIANWAN:	//10000000����
			{
				if (m_byGameStation != GS_BET_STATUS)
				{
					return 0;
				}
				
				//��ǰѡ��ĳ�������ֵ
				m_bySelChouMaType = pMessage->_uControlID - BTN_MONEY_YIBAI;
				if (m_bySelChouMaType >= 0 && m_bySelChouMaType<=5 )
				{
					CString sImageSrc;
					if(4==m_bySelChouMaType)
					{
						sImageSrc.Format(".\\resources\\image\\chouma_%d.png",m_bySelChouMaType);
					}
					else
					{
						sImageSrc.Format(".\\resources\\image\\chouma_%d.bmp",m_bySelChouMaType);
					}
					m_pUI->GetICursor()->SetCursorImage(CA2W(sImageSrc));
					m_pUI->GetICursor()->SetSrcW(m_pUI->GetICursor()->GetCursorImageWidth()/4);
					m_pUI->GetICursor()->SetPointMode(1);
					m_pUI->GetICursor()->SetControlVisible(true);
				}
				break;
			}
			//����ؼ�ID
		case   BTN_10001321://��ע��
			{
				if (m_byGameStation != GS_BET_STATUS)
				{
					return 0;
				}
				if(255==m_bySelChouMaType)
				{
					return 0;
				}
				if(m_iMyDeskStation==m_byNtStation)
				{
					return 0;
				}
				OutputDebugString("liuwen-��ע��");
				UserBetMsg betMsg;
				betMsg.byDeskStation=m_iMyDeskStation;
				betMsg.byBetArea=0;
				betMsg.byChouMaType=m_bySelChouMaType;
				m_pGameFrame->SendGameData(&betMsg, sizeof(betMsg) ,MDM_GM_GAME_NOTIFY,ASS_USER_BET,0);
				break;
			}
		case   BTN_10001322://��ע��
			{
				if (m_byGameStation != GS_BET_STATUS)
				{
					return 0;
				}
				if(255==m_bySelChouMaType)
				{
					return 0;
				}
				if(m_iMyDeskStation==m_byNtStation)
				{
					return 0;
				}
				
				UserBetMsg betMsg;
				betMsg.byDeskStation=m_iMyDeskStation;
				betMsg.byBetArea=1;
				betMsg.byChouMaType=m_bySelChouMaType;
				m_pGameFrame->SendGameData(&betMsg, sizeof(betMsg) ,MDM_GM_GAME_NOTIFY,ASS_USER_BET,0);
				break;
			}
		case   BTN_10001325://��ע��
			{
				if (m_byGameStation != GS_BET_STATUS)
				{
					return 0;
				}
				if(255==m_bySelChouMaType)
				{
					return 0;
				}
				if(m_iMyDeskStation==m_byNtStation)
				{
					return 0;
				}
				
				UserBetMsg betMsg;
				betMsg.byDeskStation=m_iMyDeskStation;
				betMsg.byBetArea=2;
				betMsg.byChouMaType=m_bySelChouMaType;
				m_pGameFrame->SendGameData(&betMsg, sizeof(betMsg) ,MDM_GM_GAME_NOTIFY,ASS_USER_BET,0);
				break;
			}
		case BTN_APPLY_MING:	//������ׯ
			{
				
				//������ׯ
				UserApplyNtStation();
				break;
			}
		case BTN_APPLY_XIAZHUANG:	//������ׯ
			{
				//������ׯ
				UserApplyLiveNt();
				break;
			}
		case BTN_BUTTON_RIGHT://����
		case BTN_BUTTON_LEFT://����
			{
				BYTE NtListBt=-1;
				NtListBt = pMessage->_uControlID - BTN_BUTTON_LEFT;
				NtListMove(NtListBt);//��ׯ�б��ƶ�
				break;
			}
		case BTN_BANK:
			{
				m_pGameFrame->OnVisitBank(); 
				break;
			}
		case CTN_2_BTN_13://�����Ӽ�
		case CTN_2_BTN_14:
			{
				BYTE bButton=0;
				bButton = pMessage->_uControlID - CTN_2_BTN_13;
				GameSet(bButton);
				break;
			}
		case CTN_2_BTN_1003500://��Ϸ����
			{
				SetContainVisible(CTN_1000000_CTN_10001420,TRUE);
				for(int i=0;i<2;i++)
				{
					if(0==i)
					{
						SetRButtonSelected(CTN_1000000_CTN_10001420_RButton_10001421+i,m_bSoundIsClose);
					}
					else if(1==i)
					{
						SetRButtonSelected(CTN_1000000_CTN_10001420_RButton_10001421+i,m_bUserExitIsClose);
					}
				}
				break;
			}
		case CTN_1000000_CTN_10001420_BTN_10001423:
		case CTN_1000000_CTN_10001420_BTN_10001424://��Ϸ����ȷ��/ȡ��
			{
				BYTE bButton=0;
				bButton = pMessage->_uControlID - CTN_1000000_CTN_10001420_BTN_10001423;
				GameSet(bButton);
				break;
			}
		case CTN_1000000_CTN_10001411_BTN_10001418://�����û�ȷ��
			{
				BYTE byTempSuperResult=255;
				for(int i=0;i<3;i++)
				{
					bool bSelected=FALSE;
					bSelected=GetRButtonSelected(CTN_1000000_CTN_10001411_RButton_10001415+i,&bSelected);
					if(bSelected)
					{
						byTempSuperResult=i;
						break;
					}
				}
				if((byTempSuperResult>=0) && (byTempSuperResult<=2))
				{
					SuperUserControlReslut SuperResult;
					SuperResult.byDeskStation=m_iMyDeskStation;
					SuperResult.bySuperReslut=byTempSuperResult;
					//����������ׯ��Ϣ
					m_pGameFrame->SendGameData(&SuperResult, sizeof(SuperResult) ,MDM_GM_GAME_NOTIFY,ASS_SUPER_RESULT,0);
				}
				ClearShowSuperResult();
				break;
			}
		case CTN_1000000_CTN_10001411_BTN_10001419://�����û�ȡ��
			{
				ClearShowSuperResult();
				break;
			}
		case CTN_1000000_CTN_10001411_BTN_10001426:
		case CTN_1000000_CTN_10001411_BTN_10001427:
			{
				m_bShowOrHideRobot =! m_bShowOrHideRobot;
				SetShowOrHideRobotBnt();
				SetShowRobotNote();
				break;
			}
		default:
			{

				break;
			}
		}
	}
	//�һ�
	if(pMessage->_uMessageType == UI_RBUTTONDOWN)
	{
		if (m_pUI->GetICursor()->GetControlVisible())
		{
			m_pUI->GetICursor()->SetControlVisible(false);
		}
		m_bySelChouMaType = 255;
		return 0;
	}
	//����Ƴ�����
	if(pMessage->_uMessageType == UI_MOUSEENTER)
	{
		switch(pMessage->_uControlID)
		{
		case BTN_APPLY_MING:	//������ׯ
		case BTN_APPLY_XIAZHUANG:	//������ׯ
		case BTN_MONEY_YIBAI:	//����
		case BTN_MONEY_YIQIAN:	//����
		case BTN_MONEY_YIWAN:	//����
		case BTN_MONEY_SHIWAN:	//����
		case BTN_XIAZHU_YIBAIWAN:	//����
		case BTN_MONEY_YIQIANWAN:	//����
			{
				
				break;
			}
		
		default:
			{
				break;
			}
		}

		return 0;
	}
	//���̰���
	if (pMessage->_uMessageType == UI_KEYDOWN)
	{
		//����F12 ��������
		if (pMessage->_ch == VK_F12 && m_bIsSuperUser && m_byGameStation == GS_BET_STATUS)
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

//��Ϸ����ȷ��/ȡ��
void CNewGameClient::GameSet(BYTE bButton)
{
	if(0==bButton)//ȷ��
	{
		for(int i=0;i<2;i++)
		{
			bool bSelected=FALSE;
			bSelected=GetRButtonSelected(CTN_1000000_CTN_10001420_RButton_10001421+i,&bSelected);
			{
				if(0==i)//�ر�����
				{
					if(bSelected)
					{
						m_bSoundIsClose=TRUE;
						
					}
					else
					{
						m_SoundEngineGame->stopAllSounds(); 
						m_bSoundIsClose=FALSE;
					}
				}
				else if(1==i)//�ر��û�������ʾ��Ϣ
				{
					if(bSelected)
					{
						
						m_bUserExitIsClose=TRUE;
					}
					else
					{
						
						m_bUserExitIsClose=FALSE;
					}
					
				}
				
			}
		}
	}
	else if(1==bButton)//ȡ��
	{
		;
	}
	SetContainVisible(CTN_1000000_CTN_10001420,FALSE);
	for(int i=0;i<2;i++)
	{
		if(0==i)
		{
			SetRButtonSelected(CTN_1000000_CTN_10001420_RButton_10001421+i,m_bSoundIsClose);
		}
		else if(1==i)
		{
			SetRButtonSelected(CTN_1000000_CTN_10001420_RButton_10001421+i,m_bUserExitIsClose);
		}
		
	}
}

//-------------------------------------------------------------------------------------------------------
//������ʾ��������ע��
void	CNewGameClient::SetShowRobotNote()
{
	//��������ע
	if(m_i64AreaAllSum[0]>0 && m_bShowOrHideRobot)
	{
		SetInumPro(CTN_1000000_CTN_10001369_Num_10001392,m_i64AreaAllSum[0],2,true);
	}
	else if(m_i64RealityAreaAllSum[0] > 0x00)
	{
		SetInumPro(CTN_1000000_CTN_10001369_Num_10001392,m_i64RealityAreaAllSum[0],2,true);
	}
	else
	{
		SetInumPro(CTN_1000000_CTN_10001369_Num_10001392,0,2,FALSE);
	}
	//��������ע
	if(m_i64AreaAllSum[1]>0 && m_bShowOrHideRobot)
	{
		SetInumPro(CTN_1000000_CTN_10001370_Num_10001394,m_i64AreaAllSum[1],2,true);
	}
	else if(m_i64RealityAreaAllSum[1] > 0x00)
	{
		SetInumPro(CTN_1000000_CTN_10001370_Num_10001394,m_i64RealityAreaAllSum[1],2,true);
	}
	else
	{
		SetInumPro(CTN_1000000_CTN_10001370_Num_10001394,0,2,FALSE);
	}
	//��������ע
	if(m_i64AreaAllSum[2]>0 && m_bShowOrHideRobot)
	{
		SetInumPro(CTN_1000000_CTN_10001371_Num_10001396,m_i64AreaAllSum[2],2,true);
	}
	else if(m_i64RealityAreaAllSum[2] > 0x00)
	{
		SetInumPro(CTN_1000000_CTN_10001371_Num_10001396,m_i64RealityAreaAllSum[2],2,true);
	}
	else
	{
		SetInumPro(CTN_1000000_CTN_10001371_Num_10001396,0,2,FALSE);
	}

}
//-------------------------------------------------------------------------------------------------------
//��ʾ���˽������
void	CNewGameClient::ShowSuperResult(bool bFlag)
{
	if (m_bIsSuperUser)
	{
		SetContainVisible(CTN_1000000_CTN_10001411,bFlag);		///<���˽���  ����
		//SetBtVisible(,bFlag);//<�ر� 
		//SetBtVisible(,bFlag);///<ȷ��
	}
}
//��ճ����û�����
void	CNewGameClient::ClearShowSuperResult()
{
	SetContainVisible(CTN_1000000_CTN_10001411,FALSE);		///<���˽���  ����
	for(int i=0;i<3;i++)
	{
		SetRButtonSelected(CTN_1000000_CTN_10001411_RButton_10001415+i,FALSE);
	}
	SetShowOrHideRobotBnt();
}
//--------------------------------------------------------------------------------------------------------
//������ʾ�����ػ�������ʾ��ť
void CNewGameClient::SetShowOrHideRobotBnt()
{
	IButton *pButtonShow = NULL ;
	IButton *pButtonNoShow = NULL ;
	pButtonShow = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(CTN_1000000_CTN_10001411_BTN_10001426));
	pButtonNoShow = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(CTN_1000000_CTN_10001411_BTN_10001427));
	CString lhx;
	if(pButtonShow!=NULL && pButtonNoShow !=NULL)
	{
		pButtonShow->SetControlVisible(m_bShowOrHideRobot);
		pButtonNoShow->SetControlVisible(!m_bShowOrHideRobot);

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
}
/*
���ܣ���ׯ�б��ƶ�
������0��������1������
*/
void CNewGameClient::NtListMove(BYTE button)
{
	BYTE byNtListCount=NtListCount();
	if(byNtListCount<=5)
	{
		return;
	}
	if(0==button)
	{
		if(m_byNtWaitListCounter>=5)
		{
			m_byNtWaitListCounter=m_byNtWaitListCounter-5;
			UserInfoForGame UserInfo ;
			if(m_pGameFrame->GetUserInfo(m_byWaitNtList[m_byNtWaitListCounter], UserInfo))
			{
				ShowNtWaitList(TRUE);
			}
			else
			{
				m_byNtWaitListCounter=m_byNtWaitListCounter+5;
			}
		}

	}
	if(1==button)
	{
		if(m_byNtWaitListCounter>=0)
		{
			m_byNtWaitListCounter=m_byNtWaitListCounter+5;
			UserInfoForGame UserInfo ;
			if(m_pGameFrame->GetUserInfo(m_byWaitNtList[m_byNtWaitListCounter], UserInfo))
			{
				ShowNtWaitList(TRUE);
			}
			else
			{
				m_byNtWaitListCounter=m_byNtWaitListCounter-5;
			}
		}
	}
}
//ͳ����ׯ�б�����
BYTE CNewGameClient::NtListCount()
{
	BYTE byNtListCount=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255!=m_byWaitNtList[i])
		{
			byNtListCount++;
		}
	}
	return byNtListCount;
}

//������ׯ
void CNewGameClient::UserApplyLiveNt()
{
	ApplyLiveNtMsg liveNtMsg;
	liveNtMsg.byDeskStation=m_iMyDeskStation;
	//����������ׯ��Ϣ
	m_pGameFrame->SendGameData(&liveNtMsg, sizeof(liveNtMsg) ,MDM_GM_GAME_NOTIFY,ASS_APPLY_LEAVE,0);

}
//������ׯ
void CNewGameClient::UserApplyNtStation()
{
	ApplyNtMsg applyMsg;
	applyMsg.byDeskStation = m_iMyDeskStation;
	//����������ׯ��Ϣ
	m_pGameFrame->SendGameData(&applyMsg, sizeof(applyMsg) ,MDM_GM_GAME_NOTIFY,ASS_APPLY_NT,0);
	
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
//---------------------------------------------------------------------------------------
//���ű�����Ч
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	if(!m_SoundEnginBg)
	{
		return ;
	}
	/*if(!m_bSoundBgPlay)
	{
	return ;
	}*/

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	wsprintf(szFileName,"music\\sound\\BeiJing.ogg");
	m_SoundEnginBg->setSoundVolume(0.5);
	m_SoundEnginBg->play2D(szFileName,bISLoop);
}

//-------------------------------------------------------------------------
//������Щ��ע��ť����
void	CNewGameClient::SetBetBtEnable(bool bFalg)
{
	UserInfoForGame UserInfo ; 
	if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
	{
		__int64 i64Money = UserInfo.i64Money;
		for(int i=0;i<6;i++)
		{

			if (i64Money > G_iChouMaValue[i])
			{
				SetBtEnable(BTN_MONEY_YIBAI+i,bFalg);
			}
			else
			{
				SetBtEnable(BTN_MONEY_YIBAI+i,false);
			}
		}
		
	}
}
//-------------------------------------------------------------------------
//������ʾ·��
void CNewGameClient::ShowLuziList(bool bFlag)
{
		CString sFilePath;
		for (int i=0; i<13; i++)
		{
			if (0 != m_byLuziList[i])
			{
				sFilePath.Format(".\\resources\\image\\LuZi\\LuZi_%d.jpg",m_byLuziList[i]);
				SetImageLoadSrc(BTN_BUTTON_LEFT+i,sFilePath,true);
			}
			else
			{
				//SetImageVisible(m_byLuziList+i,false);
			}
		}
	}

//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ�������ʾָ�����ļ�
void	CNewGameClient::SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag)
{
	//virtual int LoadPic(wchar_t *szFileName)=0;
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->LoadPic(CA2W(sImagSrc));
		pImage->SetControlVisible(bFlag);
	}
}
//---------------------------------------------------------------------------------
//��ȡָ����ҵ�ʣ����
__int64		CNewGameClient::GetUserRemaindMoney(BYTE byDestStation)
{
	__int64 i64Money = 0;
	UserInfoForGame UserInfo ; 
	if (m_pGameFrame->GetUserInfo(byDestStation , UserInfo))
	{
		i64Money = UserInfo.i64Money - GetUserBetSum(byDestStation);
	}
	return i64Money;
}
//---------------------------------------------------------------------------------
//��ȡĳλ��ҵ�����ע���
__int64		CNewGameClient::GetUserBetSum(BYTE byDestStation)
{
	if (byDestStation > PLAY_COUNT)
	{
		return 0;
	}
	__int64 i64Money = 0;
	for(int i=0; i< BET_ARES; i++)
	{
		//i64Money += m_i64UserBetCount[byDestStation][i];
	}
	return i64Money;
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

/// ���ǿ�˽�����Ϸ
bool CNewGameClient::UserExit(WORD nSubId,BYTE * buffer)
{
	tagGameForceQuit *quit=(tagGameForceQuit *)buffer;
	if (NULL==quit)
	{
		return true;
	}
	switch(nSubId)
	{
	case ASS_CUT_END:
		{
			TCHAR szMessage[100]={0};
			UserInfoForGame userInfo;
			CString cs;
			cs.Format("�˳����=%d",quit->byDeskStation);
			OutputDebugString(cs);
			m_pGameFrame->GetUserInfo(quit->byDeskStation,userInfo);
			_stprintf_s(szMessage, sizeof(szMessage), TEXT("�� %s ��ǿ����"),userInfo.szNickName);
			m_pGameFrame->InsertNormalMessage(szMessage);
			break;
		}
	case ASS_SAFE_END:
		{
			TCHAR szMessage[100]={0};
			UserInfoForGame userInfo;
			m_pGameFrame->GetUserInfo(quit->byDeskStation,userInfo);
			_stprintf_s(szMessage, sizeof(szMessage), TEXT("�� %s ���˳���"),userInfo.szNickName);
			m_pGameFrame->InsertNormalMessage(szMessage);
			break;
		}
	}
	return TRUE;
}
//�û�׼���׶��˳���Ϣ
void	CNewGameClient::HandleGamePrepareQuit(void * pBuffer)
{
	GamePrepareStageQuit *prepareQuit = (GamePrepareStageQuit *)pBuffer;
	if(NULL == prepareQuit)
	{
		return ;
	}
	m_byNtStation = prepareQuit->byNtStation;
	m_iNtPlayCount= prepareQuit->byNtcount;
	m_i64NtScore=prepareQuit->i64NtScore;

	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));

	//������ׯ�б�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==prepareQuit->byNtWaitList[i])
		{
			continue;
		}

		m_byWaitNtList[i]=prepareQuit->byNtWaitList[i];
	}
	//ˢ��ׯ����Ϣ
	ShowNtInfo(TRUE);
	//ˢ��ׯ���б�
	ShowNtWaitList(TRUE);
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
	case ASS_CUT_END:	///�û�ǿ���뿪
	case ASS_SAFE_END:
		{
			UserExit(nSubId, buffer);
			break;
		}
	case ASS_PREPARE_END://�û�׼���׶��˳�
		{
			if(nLen!=sizeof(GamePrepareStageQuit))
			{
				return 0;
			}
			HandleGamePrepareQuit(buffer);
			break;
		}
	case ASS_GM_AGREE_GAME:
		{		
			if(nLen!=sizeof(MSG_GR_R_UserAgree) || m_bIsPlaying)
			{
				return 0;
			}				

			MSG_GR_R_UserAgree * pUserAgree=(MSG_GR_R_UserAgree *)buffer;
			if (NULL == pUserAgree)
			{
				return 0;
			}
			if (pUserAgree->bDeskStation == m_iMyDeskStation)
			{
				m_bIsReady = true;	//�Ѿ�׼����
				//������Ϸ����
				ReSetUiDlag();
				//���ص���ʱ
				//SetTimePlay(CTN_1000000_CTN_1000100_Time_1000199,false,0);
			}
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
	case ASS_GAME_COME:
		{
			if(nLen!=sizeof(UserComePlayerMsg))
			{
				return 0;
			}
			PlayCome(buffer);
			break;
		}

	case ASS_GAME_FREE:	//����
		{
			if(nLen!=sizeof(BeginFreeMsg))
			{
				return 0;
			}
			
			HandleGameFree(buffer);
			break;
		}
	case ASS_BEGIN_BET:	//��ʼ��ע(��ע�׶�)
		{
			if(nLen!=sizeof(BeginBetMsg))
			{
				return 0;
			}	
			HandleBeginBet(buffer);
			break;
		}
	
	case ASS_GAME_SEND://��ʼ���ƣ����ƽ׶Σ�
		{
			if(nLen!=sizeof(BeginSendCardMsg))
			{
				return 0;
			}	
			//����
			HandleKaiPai(buffer);
			break;
		}
	case ASS_GAME_DATA://���������
		{
			if(nLen!=sizeof(BeginShowResultData))
			{
				return 0;
			}
			HandleReceiveData(buffer);
			break;
		}
	case ASS_GAME_ACCOUNT://��ʾ��Ϸ�����
		{
			if(nLen!=sizeof(BeginShowResult))
			{
				return 0;
			}	
			ShowGameResult(buffer) ; 
			break;
		}
	case ASS_APPLY_RESULT://������ׯ�����Ϣ
		{
			if(nLen!=sizeof(ApplyNtResultMsg))
			{
				return 0;
			}
			
			HandleApplyResult(buffer);
			break;
		}
	case ASS_LEAVE_RESULT:	//������ׯ���
		{
			
			if(nLen!=sizeof(ApplyLiveNtResultMsg))
			{
				return 0;
			}
			
			HandleLeaveNtResult(buffer);
			break;
		}
	case ASS_BET_RESULT:	//��ע���
		{
			
			if(nLen!=sizeof(UserBetResultMsg))
			{
				return 0;
			}
			HandleUserBetResult(buffer);
			break;
		}
	
	default:
		{
			break;
		}

	}
	return 0;
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
	/*CString cs;
	cs.Format("test  pSuperUser->byDeskStation=%d",pSuperUser->byDeskStation);
	OutputDebugString(cs);
	cs.Format("test  m_iMyDeskStation=%d",m_iMyDeskStation);
	OutputDebugString(cs);*/
	if (pSuperUser->byDeskStation == m_iMyDeskStation)
	{
				
		m_bIsSuperUser	 = TRUE;	//�Ƿ񳬼��û�
		CString cs;
		cs.Format("test  ���˿ͻ���=%d",m_iMyDeskStation);
		OutputDebugString(cs);
	}
}
//���ܽ�������
void CNewGameClient::HandleReceiveData(void *buffer)
{
	BeginShowResultData *receiveData = (BeginShowResultData *)buffer;
	if(NULL == receiveData)
	{
		return ;
	}
	i64PlayerGrade[receiveData->bDeskStation]=receiveData->i64PlayerGrade;//��ҳɼ�
	i64ChangeMoney[receiveData->bDeskStation]=receiveData->i64ChangeMoney;//��ҵ÷�
	i64ReturnMoney[receiveData->bDeskStation]=receiveData->i64ReturnMoney;//�����Ľ���� 
	m_i64NtScore=i64PlayerGrade[m_byNtStation];

}
//��ҽ�����Ϸ
void CNewGameClient::PlayCome(void *buffer)
{
	UserComePlayerMsg *playerComeMsg = (UserComePlayerMsg *)buffer;
	if(NULL == playerComeMsg)
	{
		return ;
	}
	UserInfoForGame UserInfo ; 
	if(m_pGameFrame->GetUserInfo(playerComeMsg->byPlayStation, UserInfo))
	{
		TCHAR cs[100];
		_stprintf_s(cs, sizeof(cs), TEXT("%s��ҽ�������Ϸ"), UserInfo.szNickName);
		m_pGameFrame->InsertNormalMessage(cs);
	}
	
}
void CNewGameClient::ChaoGuo(void *buffer)
{
	ChaoguoMsg *startMsg = (ChaoguoMsg *)buffer;
	if(NULL == startMsg)
	{
		return ;
	}
	for(int i=0;i<6;i++)
	{
		SetBtEnable(BTN_MONEY_YIBAI+i,false);
	}
}

void CNewGameClient::HandleStart(void *buffer)
{
	StartMsg *startMsg = (StartMsg *)buffer;
	if(NULL == startMsg)
	{
		return ;
	}
	    
	//����ʱ�ӵ���ʱ
	SetTimePlay(BTN_CLOCK,true,startMsg->startTime);  

}
//����������ׯ�����Ϣ
void CNewGameClient::HandleLeaveNtResult(void *buffer)
{
	
	ApplyLiveNtResultMsg *ShowResultMsg = (ApplyLiveNtResultMsg *)buffer;
	if(NULL == ShowResultMsg)
	{
		return ;
	}
	m_byNtStation=ShowResultMsg->byNtStation;
	m_iNtPlayCount=ShowResultMsg->byNtcount;//ׯ����ׯ����
	//ׯ�ҵĳɼ�
	m_i64NtScore=ShowResultMsg->i64NtScore;

	ShowNtInfo(TRUE);
	//������ׯ�б�
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==ShowResultMsg->byWaitNtList[i])
		{
			continue;
		}

		m_byWaitNtList[i]=ShowResultMsg->byWaitNtList[i];
	}

	BOOL flag=false;//�Ƿ�����ׯ�б��еı�־λ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iMyDeskStation==m_byWaitNtList[i])//�Լ���ׯ���б���
		{
			flag=true;

			break;
		}
	}
	if(flag)//��ׯ���б�������ע
	{
		//������ׯ��ׯ��ť��ʹ�ú���ʾ
		BtNtDisp(1);
	}
	else
	{
		if((m_byNtStation!=255)&&(m_byNtStation==m_iMyDeskStation))//�Լ���ׯ���ڿ��f�A�β������f
		{
			if(GS_FREE_STATUS==m_byGameStation)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(1);
			}
			else
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}

		}
		else
		{
			//����Ƿ�ﵽ��ׯ���
			UserInfoForGame UserInfo ; 
			memset(&UserInfo,0,sizeof(UserInfo));
			//�Լ����
			if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
			{
				if(UserInfo.i64Money>=m_i64ApplyNtLimite)
				{
					//������ׯ��ׯ��ť��ʹ�ú���ʾ
					BtNtDisp(0);
				}
				else
				{
					//������ׯ��ׯ��ť��ʹ�ú���ʾ
					BtNtDisp(2);
				}
			}
			m_byNtWaitListCounter=0;
		}
	}
	//��ʾׯ���б�
	ShowNtWaitList(true);
	
}
//����״̬
void CNewGameClient::HandleFreeStation(void *buffer)
{
	GameStation_Base *TGameStation = (GameStation_Base *)buffer;
	if(NULL == TGameStation)
	{
		return ;
	}
	
	
	//��Ұ���
	SetTextinfo(BTN_PLAY_BASHU,m_iPlayBaShu,true);

	m_byGameStation = TGameStation->bGameStation;	//��Ϸ״̬
	m_byFreeTime=TGameStation->byFreeTime;			//����ʱ��
	m_byBetTime=TGameStation->byBetTime;			//��עʱ��
	m_byOpenCardTime=TGameStation->bySendTime;	    //����ʱ��
	byAccountTime=TGameStation->byAccountTime;//����ʱ��
	m_byShowResult=TGameStation->byShowResult;//չʾ���ʱ��
	m_bySendSpaceTime=TGameStation->bySendCardSpaceTime;	//���Ƽ��ʱ��
	m_bySpeedTime=TGameStation->bySpeedTime;               //�ڵ����ƶ��ٶ�
	m_byShowWinTime=TGameStation->byShowWinTime;
	m_iClock=TGameStation->byRemaindTime;                     //ʣ��ʱ��

	m_i64ApplyNtLimite=TGameStation->iApplyNtLimite ; //ׯ����ׯ����

	//ׯ����Ϣ�������ͳɼ���
	m_byNtStation=TGameStation->byNtStation;		//ׯ��λ��

	m_i64NtScore=TGameStation->i64NtGrade;	//ׯ�ҳɼ�
	m_iNtPlayCount=TGameStation->iNtPlayCount;		//ׯ����ׯ����
	m_byMaxNtPeople=TGameStation->byMaxNtPeople;		//�����ׯ����
	m_iPlayGrade=TGameStation->i64PlayerGrade;//��ҳɿ�
	m_iPlayJuShu=TGameStation->byEvent;//��Ϸ����
	if(255!=m_byNtStation)
	{
		m_iPlayBaShu++;
	}

	//��ʾׯ����Ϣ
	ShowNtInfo(true);

	//��ʾ�Լ�����Ϣ
	ShowMyInfo(true);
	//��ʾ�׶�ͼƬ
	ShowSection(0);
	//����ע������
	//SetInumPro(CTN_1000000_CTN_10001421_Num_10001429,0,2,true);
	m_dbyLuzi.clear();
	//����·��
	for(int i=0;i<72;i++)
	{
		if(255==TGameStation->byLuziList[i])
		{
			continue;
		}
		m_dbyLuzi.push_back(TGameStation->byLuziList[i]);
	}

	ShowLuzi();
	int iNtCount=0;
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б���Ϣ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==TGameStation->byWaitNtList[i])
		{
			continue;
		}
		m_byWaitNtList[i]=TGameStation->byWaitNtList[i];
		iNtCount++;
	}
	
	//��ʾׯ���б�
	ShowNtWaitList(true);

	BOOL flag=false;//�Ƿ�����ׯ�б��еı�־λ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iMyDeskStation==m_byWaitNtList[i])//�Լ���ׯ���б���
		{
			flag=true;

			break;
		}
	}

	if(flag)//��ׯ���б�������ע
	{
		//������ׯ��ׯ��ť��ʹ�ú���ʾ
		BtNtDisp(1);
	}
	else
	{
		if((m_byNtStation!=255)&&(m_byNtStation==m_iMyDeskStation))//�Լ���ׯ�Ҳ�����ע
		{
			if(GS_FREE_STATUS==m_byGameStation)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(1);
			}
			else
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}
		}
		else
		{
			if(iNtCount>=m_byMaxNtPeople)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}
			else
			{
				//����Ƿ�ﵽ��ׯ���
				UserInfoForGame UserInfo ; 
				memset(&UserInfo,0,sizeof(UserInfo));
				//�Լ����
				if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
				{
					if(UserInfo.i64Money>=m_i64ApplyNtLimite)
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(0);
					}
					else
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(2);
					}
				}

			}

		}

	}
	//�����ܷ���ע
	BtEnable(false);
	//��ʾ��ע��ť
	SetBetBtEnable(false,1);

	//��������ʱ
	//����ʱ�����ʾ����ʱ
	//SetTimePlay(BTN_CLOCK,true, m_iClock);

	m_iClockLable=m_iClock;                     //ʱ�ӱ�ǩ
	SetInumPro(CTN_1000000_CTN_10001408_Num_10001409,m_iClock,2,true); 
	//����ʱ�ӵ���ʱ
	m_pGameFrame->SetTimer(TIME_ClOCK,1000);

	TCHAR cs[100];
	_stprintf_s(cs, sizeof(cs), TEXT("��ׯ���:%I64d"), m_i64ApplyNtLimite);
	m_pGameFrame->InsertNormalMessage(cs);
	PlayGameSound(SOUND_GAME_E,FALSE);
}
//��ע״̬
void CNewGameClient::HandleXiaZhuStation(void *buffer)
{
	GameStation_Base *TGameStation = (GameStation_Base *)buffer;
	if(NULL == TGameStation)
	{
		return ;
	}
	
	//��Ұ���
	SetTextinfo(BTN_PLAY_BASHU,m_iPlayBaShu,true); 

	m_byGameStation = TGameStation->bGameStation;	//��Ϸ״̬
	m_byFreeTime=TGameStation->byFreeTime;			//����ʱ��
	m_byBetTime=TGameStation->byBetTime;			//��עʱ��
	m_byOpenCardTime=TGameStation->bySendTime;	    //����ʱ��
	byAccountTime=TGameStation->byAccountTime;//����ʱ��
	m_byShowResult=TGameStation->byShowResult;//չʾ���ʱ��
	m_bySendSpaceTime=TGameStation->bySendCardSpaceTime;	//���Ƽ��ʱ��
	m_bySpeedTime=TGameStation->bySpeedTime;               //�ڵ����ƶ��ٶ�
	m_byShowWinTime=TGameStation->byShowWinTime;
	m_iClock=TGameStation->byRemaindTime;                     //ʣ��ʱ��

	m_i64ApplyNtLimite=TGameStation->iApplyNtLimite ; //ׯ����ׯ����

	//ׯ����Ϣ�������ͳɼ���
	m_byNtStation=TGameStation->byNtStation;		//ׯ��λ��

	m_i64NtScore=TGameStation->i64NtGrade;	//ׯ�ҳɼ�
	m_iNtPlayCount=TGameStation->iNtPlayCount;		//ׯ����ׯ����
	m_byMaxNtPeople=TGameStation->byMaxNtPeople;		//�����ׯ����
	m_iPlayGrade=TGameStation->i64PlayerGrade;//��ҳɿ�
	m_iPlayJuShu=TGameStation->byEvent;//��Ϸ����
	if(255!=m_byNtStation)
	{
		m_iPlayBaShu++;
	}
	//��ʾׯ����Ϣ
	ShowNtInfo(true);

	//��ʾ�Լ�����Ϣ
	ShowMyInfo(true);

	//��ʾ�׶�ͼƬ
	ShowSection(1);
	m_dbyLuzi.clear();
	//����·��
	for(int i=0;i<72;i++)
	{
		if(255==TGameStation->byLuziList[i])
		{
			continue;
		}
		m_dbyLuzi.push_back(TGameStation->byLuziList[i]);
	}         
	ShowLuzi();
	int iNtCount=0;
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б���Ϣ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==TGameStation->byWaitNtList[i])
		{
			continue;
		}

		m_byWaitNtList[i]=TGameStation->byWaitNtList[i];
		iNtCount++;
	}

	//��ʾׯ���б�
	ShowNtWaitList(true);

	BOOL flag=false;//�Ƿ�����ׯ�б��еı�־λ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iMyDeskStation==m_byWaitNtList[i])//�Լ���ׯ���б���
		{
			flag=true;

			break;
		}
	}

	if(flag)//��ׯ���б�������ע
	{
		//������ׯ��ׯ��ť��ʹ�ú���ʾ
		BtNtDisp(1);
		//�����ܷ���ע
		BtEnable(true);
		//��ʾ��ע��ť
		SetBetBtEnable(true,0);
	}
	else
	{
		if((m_byNtStation!=255)&&(m_byNtStation==m_iMyDeskStation))//�Լ���ׯ�Ҳ�����ע
		{
			if(GS_FREE_STATUS==m_byGameStation)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(1);
			}
			else
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}
			//�����ܷ���ע
			BtEnable(false);
			//��ʾ��ע��ť
			SetBetBtEnable(false,1);
		}
		else
		{
			if(iNtCount>=m_byMaxNtPeople)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}
			else
			{
				//����Ƿ�ﵽ��ׯ���
				UserInfoForGame UserInfo ; 
				memset(&UserInfo,0,sizeof(UserInfo));
				//�Լ����
				if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
				{
					if(UserInfo.i64Money>=m_i64ApplyNtLimite)
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(0);
					}
					else
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(2);
					}
				}
			}
			//�����ܷ���ע
			BtEnable(true);
			//��ʾ��ע��ť
			SetBetBtEnable(true,0);
		}

	}
	if(255==m_byNtStation)
	{
		//�����ܷ���ע
		BtEnable(false);
		//��ʾ��ע��ť
		SetBetBtEnable(false,1);
	}

	//����ʱ�ӵ���ʱ
	//SetTimePlay(BTN_CLOCK,true,m_iClock);

	m_iClockLable=m_iClock;                     //ʱ�ӱ�ǩ
	SetInumPro(CTN_1000000_CTN_10001408_Num_10001409,m_iClock,2,true); 
	//����ʱ�ӵ���ʱ
	m_pGameFrame->SetTimer(TIME_ClOCK,1000);
	TCHAR cs[100];
	_stprintf_s(cs, sizeof(cs), TEXT("��ׯ���:%I64d"), m_i64ApplyNtLimite);
	m_pGameFrame->InsertNormalMessage(cs);
	PlayGameSound(SOUND_GAME_E, FALSE); 
}
//����״̬
void CNewGameClient::HandleKaiPaiStation(void *buffer)
{
	GameStation_Base *TGameStation = (GameStation_Base *)buffer;
	if(NULL == TGameStation)
	{
		return ;
	}
	  
	//��Ұ���
	SetTextinfo(BTN_PLAY_BASHU,m_iPlayBaShu,true); 

	m_byGameStation = TGameStation->bGameStation;	//��Ϸ״̬
	m_byFreeTime=TGameStation->byFreeTime;			//����ʱ��
	m_byBetTime=TGameStation->byBetTime;			//��עʱ��
	m_byOpenCardTime=TGameStation->bySendTime;	    //����ʱ��
	byAccountTime=TGameStation->byAccountTime;//����ʱ��
	m_byShowResult=TGameStation->byShowResult;//չʾ���ʱ��
	m_bySendSpaceTime=TGameStation->bySendCardSpaceTime;	//���Ƽ��ʱ��
	m_bySpeedTime=TGameStation->bySpeedTime;               //�ڵ����ƶ��ٶ�
	m_byShowWinTime=TGameStation->byShowWinTime;
	m_iClock=TGameStation->byRemaindTime;                     //ʣ��ʱ��

	m_i64ApplyNtLimite=TGameStation->iApplyNtLimite ; //ׯ����ׯ����

	//ׯ����Ϣ�������ͳɼ���
	m_byNtStation=TGameStation->byNtStation;		//ׯ��λ��

	m_i64NtScore=TGameStation->i64NtGrade;	//ׯ�ҳɼ�
	m_iNtPlayCount=TGameStation->iNtPlayCount;		//ׯ����ׯ����
	m_byMaxNtPeople=TGameStation->byMaxNtPeople;		//�����ׯ����
	m_iPlayGrade=TGameStation->i64PlayerGrade;//��ҳɿ�
	m_iPlayJuShu=TGameStation->byEvent;//��Ϸ����
	
	if(255!=m_byNtStation)
	{
		m_iPlayBaShu++;
	}
	//��ʾׯ����Ϣ
	ShowNtInfo(true);

	//��ʾ�Լ�����Ϣ
	ShowMyInfo(true);

	//��ʾ�׶�ͼƬ
	ShowSection(2);
	//��ʾ����ע

	//��ʾ����������ע
	//SetInumPro(CTN_1000000_CTN_10001421_Num_10001429,0,2,true);
	m_dbyLuzi.clear();
	//����·��
	for(int i=0;i<72;i++)
	{
		if(255==TGameStation->byLuziList[i])
		{
			continue;
		}
		m_dbyLuzi.push_back(TGameStation->byLuziList[i]);
	}         
	ShowLuzi();
	int iNtCount=0;
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б���Ϣ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==TGameStation->byWaitNtList[i])
		{
			continue;
		}
		m_byWaitNtList[i]=TGameStation->byWaitNtList[i];
		iNtCount++;
	}

	//��ʾׯ���б�
	ShowNtWaitList(true);

	BOOL flag=false;//�Ƿ�����ׯ�б��еı�־λ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iMyDeskStation==m_byWaitNtList[i])//�Լ���ׯ���б���
		{
			flag=true;

			break;
		}
	}

	if(flag)//��ׯ���б�������ע
	{
		//������ׯ��ׯ��ť��ʹ�ú���ʾ
		BtNtDisp(1);

	}
	else
	{
		if((m_byNtStation!=255)&&(m_byNtStation==m_iMyDeskStation))//�Լ���ׯ�Ҳ�����ע
		{
			//������ׯ��ׯ��ť��ʹ�ú���ʾ
			BtNtDisp(2);

		}
		else
		{
			if(iNtCount>=m_byMaxNtPeople)
			{
				if(GS_FREE_STATUS==m_byGameStation)
				{
					//������ׯ��ׯ��ť��ʹ�ú���ʾ
					BtNtDisp(1);
				}
				else
				{
					//������ׯ��ׯ��ť��ʹ�ú���ʾ
					BtNtDisp(2);
				}
			}
			else
			{
				//����Ƿ�ﵽ��ׯ���
				UserInfoForGame UserInfo ; 
				memset(&UserInfo,0,sizeof(UserInfo));
				//�Լ����
				if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
				{
					if(UserInfo.i64Money>=m_i64ApplyNtLimite)
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(0);
					}
					else
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(2);
					}
				}
			}
		}

	}
	//�����ܷ���ע
	BtEnable(false);
	//��ʾ��ע��ť
	SetBetBtEnable(false,1);
	//����ʱ�ӵ���ʱ
	//SetTimePlay(BTN_CLOCK,true,m_iClock);
	m_iClockLable=m_iClock;                     //ʱ�ӱ�ǩ
	SetInumPro(CTN_1000000_CTN_10001408_Num_10001409,m_iClock,2,true); 
	//����ʱ�ӵ���ʱ
	m_pGameFrame->SetTimer(TIME_ClOCK,1000);
	TCHAR cs[100];
	_stprintf_s(cs, sizeof(cs), TEXT("��ׯ���:%I64d"), m_i64ApplyNtLimite);
	m_pGameFrame->InsertNormalMessage(cs);
	PlayGameSound(SOUND_GAME_E,FALSE);
}
//��ʾ·��
void CNewGameClient::ShowLuzi()
{
	//��ʾ·��
	for(int i=0;i<m_dbyLuzi.size();i++)
	{
		if(0==m_dbyLuzi.at(i))
		{
			CString src;
			src.Format(".\\resources\\image\\daqi.bmp");
			SetPicSize(BTN_LUZI_1+i,src, true,0,0,27,28);
		}
		else if(1==m_dbyLuzi.at(i))
		{
			CString src;
			src.Format(".\\resources\\image\\daqi.bmp");
			SetPicSize(BTN_LUZI_1+i,src,true,27,0,27,28);
		}
		else if(2==m_dbyLuzi.at(i))
		{
			CString src;
			src.Format(".\\resources\\image\\daqi.bmp");
			SetPicSize(BTN_LUZI_1+i,src, true,54,0,27,28);
		}
	}
}
//����·��
void CNewGameClient::HideLuzi()
{
	//��ʾ·��
	for(int i=0;i<72;i++)
	{
		SetImageVisible(BTN_LUZI_1+i,FALSE);
	}
}
//��ʾ�����
void CNewGameClient::HandleShowResult()
{
	;
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
//���������ע���
void CNewGameClient::HandleUserBetResult(void *buffer)
{
	
	UserBetResultMsg *BetResultMsg = (UserBetResultMsg *)buffer;
	if(NULL == BetResultMsg)
	{
		return ;
	}
	PlayGameSound(SOUND_GAME_A, FALSE);
	//��ʾ����ע
	//SetInumPro(CTN_1000000_CTN_10001421_Num_10001429,BetResultMsg->i64SumAllBet,2,true);
	//

	if(0x01==BetResultMsg->byBetNumFlag)
	{
		PlayGameSound(SOUND_GAME_K, FALSE);
	}
	else if(0x02==BetResultMsg->byBetNumFlag)
	{
		PlayGameSound(SOUND_GAME_L, FALSE);
	}
	else if(0x03==BetResultMsg->byBetNumFlag)
	{
		PlayGameSound(SOUND_GAME_D, FALSE);
	}
	//��Ӧ������ʾ����ֵ
	SetShCtrlInfo(BTN_BET_LEFT+BetResultMsg->byBetArea,G_iChouMaValue[BetResultMsg->byChouMaType], true);

	for(int i = 0x00;i < BET_ARES;i++)
	{
		m_i64AreaAllSum[i] = BetResultMsg->i64AreaBetAllSum[i];
		m_i64RealityAreaAllSum[i] = BetResultMsg->i64AreaBetAllSum[i] - BetResultMsg->i64AreaRobot[i];
	}
	//��ʾ�����Լ���ע������ע
	//�����Լ���ע
	if(BetResultMsg->i64UserBetCount[0]>0)
	{
		SetInumPro(CTN_1000000_CTN_10001369_Num_10001393,BetResultMsg->i64UserBetCount[0],2,true);
	}
	if(BetResultMsg->i64UserBetCount[1]>0)
	{
		SetInumPro(CTN_1000000_CTN_10001370_Num_10001395,BetResultMsg->i64UserBetCount[1],2,true);
	}
	if(BetResultMsg->i64UserBetCount[2]>0)
	{
		SetInumPro(CTN_1000000_CTN_10001371_Num_10001397,BetResultMsg->i64UserBetCount[2],2,true);
	}
	//��������ע
	SetShowRobotNote();
	UserInfoForGame UserInfo ;
	memset(&UserInfo,0,sizeof(UserInfo));
	if(m_pGameFrame->GetUserInfo(m_iMyDeskStation, UserInfo))
	{
		m_i64PlayerCurrentMoney=UserInfo.i64Money;
	}
	__int64 betMoney=0;
	for(int i=0;i<BET_ARES;i++)
	{
		betMoney+=BetResultMsg->i64UserBetCount[i]; //�м���ÿ���������ע���
	}
	m_i64PlayerCurrentMoney=m_i64PlayerCurrentMoney-betMoney;
	//��ҽ��
	SetTextinfo(BTN_PLAY_MONEY,m_i64PlayerCurrentMoney,true);
	//����ʣ���Ǯ��ʾ���밴ť
	if(m_iMyDeskStation!=m_byNtStation)
	{
		for(int i=0;i<6;i++)
		{
			if (m_i64PlayerCurrentMoney > G_iChouMaValue[i])
			{
				SetBtEnable(CTN_1000000_CTN_10001332_BTN_10001182+i,true);
			}
			else
			{
				SetBtEnable(CTN_1000000_CTN_10001332_BTN_10001182+i,FALSE);
			}
		}
	}
	if(255 != m_bySelChouMaType)
	{
		if(G_iChouMaValue[m_bySelChouMaType]>m_i64PlayerCurrentMoney)
		{
			if (m_pUI->GetICursor()->GetControlVisible())
			{
				m_pUI->GetICursor()->SetControlVisible(false);
			}
			m_bySelChouMaType = 255;
		}
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

//����
void CNewGameClient::HandleKaiPai(void *buffer)
{
	BeginSendCardMsg *pBeginKaiPai = (BeginSendCardMsg *)buffer;
	
	if(NULL == pBeginKaiPai)
	{
		return ;
	}
	m_byGameStation =pBeginKaiPai->byGameStation;         ///��ǰ����Ϸ״̬
	m_byArea=pBeginKaiPai->byWinArea;
	//�����ƶ�����
	if (m_pUI->GetICursor()->GetControlVisible())
	{
		m_pUI->GetICursor()->SetControlVisible(false);
	}
	m_bySelChouMaType = 255;
	m_SoundEngineGame->stopAllSounds();
	PlayGameSound(SOUND_GAME_C,FALSE); 
	//��ʾ�׶�ͼƬ
	ShowSection(2);
	//ɾ����ʱ��
	m_pGameFrame->KillTimer(TIME_ClOCK);
	//�����׶β�����ע
	//�����ܷ���ע
	BtEnable(false);
	//��ʾ��ע��ť
	SetBetBtEnable(false,1);

	//�����һ������
	memset(m_byCardStyle,-1,sizeof(m_byCardStyle));
	for(int i=0;i<pBeginKaiPai->byCardCount;i++)
	{
		m_byCardStyle[i]=pBeginKaiPai->byCardStyle[i];
	}
	BYTE iNtCount=0;
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==pBeginKaiPai->byNtWaitList[i])
		{
			continue;
		}

		m_byWaitNtList[i]=pBeginKaiPai->byNtWaitList[i];
		iNtCount++;
	}

	//��ʾׯ���б�
	ShowNtWaitList(true);

	BOOL flag=false;//�Ƿ�����ׯ�б��еı�־λ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iMyDeskStation==m_byWaitNtList[i])//�Լ���ׯ���б���
		{
			flag=true;

			break;
		}
	}


	if(flag)//��ׯ���б�������ע
	{
		//������ׯ��ׯ��ť��ʹ�ú���ʾ
		BtNtDisp(1);
	}
	else
	{
		if((m_byNtStation!=255)&&(m_byNtStation==m_iMyDeskStation))//�Լ���ׯ�Ҳ�����ׯ
		{
			if(GS_FREE_STATUS==m_byGameStation)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(1);
			}
			else
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}

		}
		else
		{
			//��ׯҪ�����ׯ��������ׯ���
			if(iNtCount>=m_byMaxNtPeople)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}
			else
			{
				//����Ƿ�ﵽ��ׯ���
				UserInfoForGame UserInfo ; 
				memset(&UserInfo,0,sizeof(UserInfo));
				//�Լ����
				if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
				{
					if(UserInfo.i64Money>=m_i64ApplyNtLimite)
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(0);
					}
					else
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(2);
					}
				}
			}

		}

	}
	//���·��
	m_dbyLuzi.clear();
	//����·��
	for(int i=0;i<72;i++)
	{
		if(255==pBeginKaiPai->byLuZi[i])
		{
			continue;
		}
		m_dbyLuzi.push_back(pBeginKaiPai->byLuZi[i]);
	}

	//���÷���ʱ�ӵ���ʱ
	//SetTimePlay(BTN_CLOCK,true,m_byOpenCardTime);
	m_iClockLable=m_byOpenCardTime;                     //ʱ�ӱ�ǩ
	SetInumPro(CTN_1000000_CTN_10001408_Num_10001409,m_byOpenCardTime,2,true); 
	//����ʱ�ӵ���ʱ
	m_pGameFrame->SetTimer(TIME_ClOCK,1000);


	//���������ƶ�ʱ��
	m_pGameFrame->SetTimer(TIME_SENDLONG_CARD,m_bySendSpaceTime*1000);
}
///��ʾ��ҵķ��ƶ���
void CNewGameClient::ShowUserSendCard(const int ID,BYTE cardStyle,BYTE cardCount,bool bShow) 
{
	
	INoMoveCard  *pNoMoveCard = NULL ; 
		pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(ID))  ; 
		
		if(NULL != pNoMoveCard)
		{
			BYTE iCardList[45] ; 
			memset(iCardList , 0x40, sizeof(iCardList)) ; 
			

			switch(cardStyle)
			{
				//����2-A
			case 0x01:iCardList[0]=49;break;
			case 0x02:iCardList[0]=50;break;
			case 0x03:iCardList[0]=51;break;
			case 0x04:iCardList[0]=52;break;
			case 0x05:iCardList[0]=53;break;
			case 0x06:iCardList[0]=54;break;
			case 0x07:iCardList[0]=55;break;
			case 0x08:iCardList[0]=56;break;
			case 0x09:iCardList[0]=57;break;
			case 0x0A:iCardList[0]=58;break;
			case 0x0B:iCardList[0]=59;break;
			case 0x0C:iCardList[0]=60;break;
			case 0x0D:iCardList[0]=61;break;
				////÷�� 2 - A
			case 0x11:iCardList[0]=33;break;
			case 0x12:iCardList[0]=34;break;
			case 0x13:iCardList[0]=35;break;
			case 0x14:iCardList[0]=36;break;
			case 0x15:iCardList[0]=37;break;
			case 0x16:iCardList[0]=38;break;
			case 0x17:iCardList[0]=39;break;
			case 0x18:iCardList[0]=40;break;
			case 0x19:iCardList[0]=41;break;
			case 0x1A:iCardList[0]=42;break;
			case 0x1B:iCardList[0]=43;break;
			case 0x1C:iCardList[0]=44;break;
			case 0x1D:iCardList[0]=45;break;
				////���� 2 - A
			case 0x21:iCardList[0]=17;break;
			case 0x22:iCardList[0]=18;break;
			case 0x23:iCardList[0]=19;break;
			case 0x24:iCardList[0]=20;break;
			case 0x25:iCardList[0]=21;break;
			case 0x26:iCardList[0]=22;break;
			case 0x27:iCardList[0]=23;break;
			case 0x28:iCardList[0]=24;break;
			case 0x29:iCardList[0]=25;break;
			case 0x2A:iCardList[0]=26;break;
			case 0x2B:iCardList[0]=27;break;
			case 0x2C:iCardList[0]=28;break;
			case 0x2D:iCardList[0]=29;break;
				////���� 2 - A
			case 0x31:iCardList[0]=1;break;
			case 0x32:iCardList[0]=2;break;
			case 0x33:iCardList[0]=3;break;
			case 0x34:iCardList[0]=4;break;
			case 0x35:iCardList[0]=5;break;
			case 0x36:iCardList[0]=6;break;
			case 0x37:iCardList[0]=7;break;
			case 0x38:iCardList[0]=8;break;
			case 0x39:iCardList[0]=9;break;
			case 0x3A:iCardList[0]=10;break;
			case 0x3B:iCardList[0]=11;break;
			case 0x3C:iCardList[0]=12;break;
			case 0x3D:iCardList[0]=13;break;
			default:break;
			}
			pNoMoveCard->SetControlVisible(bShow) ; 
			pNoMoveCard->SetCardList(iCardList , cardCount) ; 
		}
		else
		{
			;
		}
		
	
	return ; 
}
//������ׯ�������
void CNewGameClient::HandleApplyResult(void *buffer)
{
	ApplyNtResultMsg *pApply = (ApplyNtResultMsg *)buffer;
	if(pApply->bApplay)
	{
		//��ʾ��ׯ��Ҳ��������ͼƬ

		//m_pGameFrame->SetTimer(TIME_SHOW_MONEY,1.5*1000);
		return;
	}
	m_byGameStation=pApply->bGameStation;
	m_byNtStation=pApply->byNtStation;		//ׯ��λ��
	m_iNtPlayCount=pApply->byNtcount;//ׯ����ׯ����
	m_iPlayJuShu=pApply->bEvent;
	//ׯ�ҵĳɼ�
	m_i64NtScore=pApply->i64NtScore;
	ShowNtInfo(true);

	int iNtCount=0;//ͳ����ׯ�б������
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==pApply->byWaitNtList[i])
		{
			continue;
		}

		m_byWaitNtList[i]=pApply->byWaitNtList[i];
		iNtCount++;
	}



	BOOL flag=false;//�Ƿ�����ׯ�б��еı�־λ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iMyDeskStation==m_byWaitNtList[i])//�Լ���ׯ���б���
		{
			flag=true;

			break;
		}
	}
	if(flag)//��ׯ���б�������ע
	{
		//������ׯ��ׯ��ť��ʹ�ú���ʾ
		BtNtDisp(1);
		//�����ܷ���ע
		BtEnable(true);
		//��Щ��ע��ť����
		SetBetBtEnable(true,0);
		m_byNtWaitListCounter=0;
	}
	else
	{
		if((m_byNtStation!=255)&&(m_byNtStation==m_iMyDeskStation))//�Լ���ׯ�Ҳ�����ע
		{
			if(GS_FREE_STATUS==m_byGameStation)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(1);
			}
			else
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}

			//�����ܷ���ע
			BtEnable(false);
			//��Щ��ע��ť����
			SetBetBtEnable(false,1);
			m_byNtWaitListCounter=0;
		}
		else
		{
			if(iNtCount>=m_byMaxNtPeople)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}
			else
			{
				//����Ƿ�ﵽ��ׯ���
				UserInfoForGame UserInfo ; 
				memset(&UserInfo,0,sizeof(UserInfo));
				//�Լ����
				if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
				{
					if(UserInfo.i64Money>=m_i64ApplyNtLimite)
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(0);
					}
					else
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(2);
					}
				}
			}

			//�����ܷ���ע
			BtEnable(true);
			//��Щ��ע��ť����
			SetBetBtEnable(true,0);
		}

	}

	if((GS_FREE_STATUS==m_byGameStation) || (GS_SEND_STATUS==m_byGameStation))//������ע
	{
		//�����ܷ���ע
		BtEnable(false);
		//��Щ��ע��ť����
		SetBetBtEnable(false,1);
	}
	if((GS_BET_STATUS==m_byGameStation) || (GS_SEND_STATUS==m_byGameStation))
	{
		if(pApply->bNtFlag)//ɱ�����ж�ʱ��
		{
			KillAllGameTime();
			//m_pGameFrame->KillTimer(TIME_AWARDSHOW);//ɾ��ʤ������ʾ��ʱ��
			////ʤ������ʾ
			//for(int i=0;i<32;i++)
			//{
			//	SetImageVisible(CTN_1000000_CTN_10001478_IMG_10001479+i,false);
			//}
			////ʤ������ʾ
			//SetImageLoadSrc(CTN_1000000_CTN_10001432_IMG_10001465,"",FALSE,1);
		}

	}
	//��ʾׯ���б�
	ShowNtWaitList(true);
	//�Լ��ľ���
	SetTextinfo(BTN_PLAY_ROUND,m_iPlayJuShu,TRUE);
}
//��ʾ��ׯ�б�
void CNewGameClient::ShowNtWaitList(bool bFalg)
{

	BYTE byTemp=0;
	for(int i=m_byNtWaitListCounter;i<m_byNtWaitListCounter+5;i++)
	{
		if(255!=m_byWaitNtList[i])
		{
			UserInfoForGame UserInfo ;
			if (m_pGameFrame->GetUserInfo(m_byWaitNtList[i], UserInfo))
			{
				///�ǳ�
				SetChinfo(BTN_SHANGZHUANG_LIST_1+byTemp,UserInfo.szNickName,bFalg);

			}
		}
		else
		{
			//�ǳ�
			SetChinfo(BTN_SHANGZHUANG_LIST_1+byTemp,"",false);
			
		}
		byTemp++;

	}


}
//-------------------------------------------------------------------------
//������Щ��ע�������
void	CNewGameClient::SetBetBtEnable(bool bFalg,BYTE jud)
{
	if(0==jud)
	{
		UserInfoForGame UserInfo ; 
		if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
		{
			__int64 i64Money = UserInfo.i64Money;
			for(int i=0;i<6;i++)
			{

				if (i64Money > G_iChouMaValue[i])
				{
					SetBtEnable(CTN_1000000_CTN_10001332_BTN_10001182+i,bFalg);
				}
				else
				{
					SetBtEnable(CTN_1000000_CTN_10001332_BTN_10001182+i,!bFalg);
				}
			}

		}
	}
	if(1==jud)
	{
		for(int i=0;i<6;i++)
		{
			SetBtEnable(CTN_1000000_CTN_10001332_BTN_10001182+i,bFalg);
		}
	}
}
//������ע�������Ƿ���Ч
void CNewGameClient::BtEnable(BOOL bflag)
{
	SetBtEnable(CTN_1000000_CTN_10001369_BTN_10001321, bflag);
	SetBtEnable(CTN_1000000_CTN_10001370_BTN_10001322, bflag);
	SetBtEnable(CTN_1000000_CTN_10001371_BTN_10001325, bflag);
	
}
//������ׯ��ׯ��ť��ʹ�ú���ʾ
void CNewGameClient::BtNtDisp(BYTE flag)
{
	if(0==flag)//��ׯ��ť����
	{
		SetBtEnable(CTN_1000000_BTN_10001365,true);
		SetBtVisible(CTN_1000000_BTN_10001365, true);
		SetBtEnable(CTN_1000000_BTN_10001171,false);
		SetBtVisible(CTN_1000000_BTN_10001171, false);
	}
	else if(1==flag)//��ׯ��ť����
	{
		SetBtEnable(CTN_1000000_BTN_10001365,false);
		SetBtVisible(CTN_1000000_BTN_10001365, false);
		SetBtEnable(CTN_1000000_BTN_10001171,true);
		SetBtVisible(CTN_1000000_BTN_10001171, true);
	}
	else if(2==flag)//��������
	{
		SetBtEnable(CTN_1000000_BTN_10001365,false);
		SetBtVisible(CTN_1000000_BTN_10001365,TRUE);
		SetBtEnable(CTN_1000000_BTN_10001171,false);
		SetBtVisible(CTN_1000000_BTN_10001171, false);
	}
}
//-------------------------------------------------------------------------
//������Ϣ
void	CNewGameClient::HandleGameFree(void * pBuffer)
{
	
	BeginFreeMsg *pGameFree = (BeginFreeMsg *)pBuffer;
	if(NULL == pGameFree)
	{
		return ;
	}
	//�����Ϸ��ʱ����
	ResetGameData();
	
	//��ʾ�׶�ͼƬ
	ShowSection(0);
	//��Ϸ���¿�ʼ��ɾ�����ж�ʱ��
	KillAllGameTime();
	//�����ܷ���ע
	BtEnable(false);
	//����״̬��ע��ť������
	SetBetBtEnable(FALSE,1);
	
	if(m_byNtStation!=pGameFree->byNtStation)//��ׯ��
	{
		m_byNtWaitListCounter=0;
		//ׯ�ҵĳɼ�����
		m_i64NtScore=0;
		//m_pGameFrame->SetTimer(TIME_SHOW_HUANZHUANG,3*1000);
	}
	 m_byGameStation =pGameFree->byGameStation;         ///��ǰ����Ϸ״̬
	 m_byNtStation = pGameFree->byNtStation;
	 m_iNtPlayCount= pGameFree->byNtcount;
	 m_i64NtScore= pGameFree->i64NtScore;

	 
	 /*CString cs;
	 cs.Format("test  ��ׯ����=%d",m_iNtPlayCount);
	 OutputDebugString(cs);*/
	//��ʾׯ����Ϣ
	ShowNtInfo(true);

	int iNtCount=0;
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	 //������ׯ�б�
	 for(int i=0;i<PLAY_COUNT;i++)
	 {
		 if(255==pGameFree->byWaitNtList[i])
		 {
			 continue;
		 }
		
		 m_byWaitNtList[i]=pGameFree->byWaitNtList[i];
		 iNtCount++;
	 }
	 //��ʾׯ���б�
	 ShowNtWaitList(true);
	 
	 BOOL flag=false;//�Ƿ�����ׯ�б��еı�־λ
	 for(int i=0;i<PLAY_COUNT;i++)
	 {
		 if(m_iMyDeskStation==m_byWaitNtList[i])//�Լ���ׯ���б���
		 {
			 flag=true;

			 break;
		 }
	 }
	 if(flag)
	 {
		BtNtDisp(1);
	 }
	 else
	 {
		 if(m_byNtStation==m_iMyDeskStation)//�Լ���ׯ��
		 {
			 
			 if(GS_FREE_STATUS==m_byGameStation)
			 {
				 
				 //������ׯ��ׯ��ť��ʹ�ú���ʾ
				 BtNtDisp(1);
			 }
			 else
			 {
				 //������ׯ��ׯ��ť��ʹ�ú���ʾ
				 BtNtDisp(2);
			 }
		 }
		 else
		 {
			 if(iNtCount>=m_byMaxNtPeople)
			 {
				 //������ׯ��ׯ��ť��ʹ�ú���ʾ
				 BtNtDisp(2);
			 }
			 else
			 {
				 //����Ƿ�ﵽ��ׯ���
				 UserInfoForGame UserInfo ; 
				 memset(&UserInfo,0,sizeof(UserInfo));
				 //�Լ����
				 if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
				 {
					 if(UserInfo.i64Money>=m_i64ApplyNtLimite)
					 {
						 //������ׯ��ׯ��ť��ʹ�ú���ʾ
						 BtNtDisp(0);
					 }
					 else
					 {
						 //������ׯ��ׯ��ť��ʹ�ú���ʾ
						 BtNtDisp(2);
					 }
				 }
			 }
			 
		 }
		 
	 }
	m_iClockLable=m_byFreeTime;                     //ʱ�ӱ�ǩ
	//����ʱ�ӵ���ʱ
	//SetTimePlay(CTN_1000000_Time_10001180,true,m_byFreeTime);
	SetInumPro(CTN_1000000_CTN_10001408_Num_10001409,m_byFreeTime,2,true);
	//����ʱ�ӵ���ʱ
	m_pGameFrame->SetTimer(TIME_ClOCK,1000);
}
//��ʾ�׶�ͼƬ
void CNewGameClient::ShowSection(BYTE bSection)
{
	switch(bSection)
	{
	case 0:
		{
			SetImageVisible(CTN_1000000_IMG_10001174,true);
			SetImageVisible(CTN_1000000_IMG_10001175,false);
			SetImageVisible(CTN_1000000_IMG_10001176,false);
			break;
		}
	case 1:
		{
			SetImageVisible(CTN_1000000_IMG_10001175,true);
			SetImageVisible(CTN_1000000_IMG_10001174,false);
			SetImageVisible(CTN_1000000_IMG_10001176,false);
			break;
		}
	case 2:
		{
			SetImageVisible(CTN_1000000_IMG_10001176,true);
			SetImageVisible(CTN_1000000_IMG_10001175,false);
			SetImageVisible(CTN_1000000_IMG_10001174,false);
			break;
		}
	}
}
//---------------------------------------------------------------------------------------
///ɾ�����ж�ʱ��
void CNewGameClient:: KillAllGameTime() 
{
	m_pGameFrame->KillTimer(TIME_SENDLONG_CARD) ; 

	m_pGameFrame->KillTimer(TIME_SENDHU_CARD) ; 

	m_pGameFrame->KillTimer(TIME_SHOW_LONG) ; 

	m_pGameFrame->KillTimer(TIME_SHOW_HU);

	m_pGameFrame->KillTimer(TIME_SHOW_HUANZHUANG);

	m_pGameFrame->KillTimer(TIME_SHOW_WIN_TIME);
	m_pGameFrame->KillTimer(TIME_SHOW_MONEY);
	
	m_pGameFrame->KillTimer(TIME_SHOW_JIEPAI);
	//�ص�ʱ��
	//SetTimePlay(BTN_CLOCK,true,0);
	
	m_pGameFrame->KillTimer(TIME_ClOCK);
}

//-------------------------------------------------------------------------
//�յ���ʼ��ע��Ϣ
void	CNewGameClient::HandleBeginBet(void * pBuffer)
{
	
	BeginBetMsg *pBeginBet = (BeginBetMsg *)pBuffer;
	if(NULL == pBeginBet)
	{
		return ;
	}
	//��ʾ�׶�ͼƬ
	ShowSection(1);
	//ɾ����ʱ��
	m_pGameFrame->KillTimer(TIME_ClOCK);
	//m_pGameFrame->KillTimer(TIME_AWARDSHOW);//ɾ��ʤ������ʾ��ʱ��
	//PlayGameSound(SOUND_GAME_J, FALSE);
	m_SoundEngineGame->stopAllSounds();
	PlayGameSound(SOUND_GAME_M,FALSE); 
	PlayGameSound(SOUND_GAME_J,FALSE); 
	//ʤ������ʾ
	//SetImageLoadSrc(CTN_1000000_CTN_10001432_IMG_10001465,"",FALSE,1);
	//ʤ������ʾ
	
		//SetImageVisible(CTN_1000000_CTN_10001478_IMG_10001479+i,false);
	
	//m_byWinPrize=-1;//���������ע�׶����
	m_byGameStation=pBeginBet->byGameStation;//��Ϸ״̬
	m_byNtStation	= pBeginBet->byNtStation;		//ׯ��λ��
	m_iPlayJuShu    =pBeginBet->byEvent;//��Ϸ����
	m_iNtPlayCount= pBeginBet->byNtcount;
	m_i64NtScore= pBeginBet->i64NtScore;
	//��ʾׯ����Ϣ
	ShowNtInfo(true);
	if(pBeginBet->bLuziClearFlag)
	{
		HideLuzi(); 
		ClearAccountResult();
	}
	//��ׯ�Ҳ���һ�ѣ����°����;�����
	if(255!=m_byNtStation)
	{
		m_iPlayBaShu++;
		//�Լ��İ���
		SetTextinfo(BTN_PLAY_BASHU,m_iPlayBaShu,TRUE);
		//����
		SetTextinfo(BTN_PLAY_ROUND,m_iPlayJuShu,TRUE);
	}
	//��ס������ʼλ��
	//m_byWinPrizeStart= pBeginBet->byWinPrizeStart;
	////����������
	//for(int i=0;i<SEND_COUNT;i++)
	//{
	//	m_byCardStyle[i]=pBeginBet->byCardStyle[i];
	//}
	int iNtCount=0;
	memset(m_byWaitNtList,255,sizeof(m_byWaitNtList));
	//������ׯ�б�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(255==pBeginBet->byNtWaitList[i])
		{
			continue;
		}

		m_byWaitNtList[i]=pBeginBet->byNtWaitList[i];
		iNtCount++;
	}
	//��ʾׯ���б�
	ShowNtWaitList(true);

	BOOL flag=false;//�Ƿ�����ׯ�б��еı�־λ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iMyDeskStation==m_byWaitNtList[i])//�Լ���ׯ���б���
		{
			flag=true;

			break;
		}
	}

	if(flag)//��ׯ���б�������ע
	{
		//������ׯ��ׯ��ť��ʹ�ú���ʾ
		BtNtDisp(1);
		//�����ܷ���ע
		BtEnable(true);
		//��ʾ��ע��ť
		SetBetBtEnable(true,0);
	}
	else
	{
		if((m_byNtStation!=255)&&(m_byNtStation==m_iMyDeskStation))//�Լ���ׯ�Ҳ�����ע
		{
			if(GS_FREE_STATUS==m_byGameStation)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(1);
			}
			else
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}
			//�����ܷ���ע
			BtEnable(false);
			//��ʾ��ע��ť
			SetBetBtEnable(false,1);
		}
		else
		{
			if(iNtCount>=m_byMaxNtPeople)
			{
				//������ׯ��ׯ��ť��ʹ�ú���ʾ
				BtNtDisp(2);
			}
			else
			{
				//����Ƿ�ﵽ��ׯ���
				UserInfoForGame UserInfo ; 
				memset(&UserInfo,0,sizeof(UserInfo));
				//�Լ����
				if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
				{
					if(UserInfo.i64Money>=m_i64ApplyNtLimite)
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(0);
					}
					else
					{
						//������ׯ��ׯ��ť��ʹ�ú���ʾ
						BtNtDisp(2);
					}
				}
			}

			//�����ܷ���ע
			BtEnable(true);
			//��ʾ��ע��ť
			SetBetBtEnable(true,0);
		}

	}
	if(255==m_byNtStation)//û��ׯ�Ҳ�����ע
	{
		//�����ܷ���ע
		BtEnable(false);
		//��ʾ��ע��ť
		SetBetBtEnable(false,1);
	}

	//����ʱ�ӵ���ʱ
	//SetTimePlay(CTN_1000000_Time_10001180,true,m_byBetTime);
	m_iClockLable=m_byBetTime;                     //ʱ�ӱ�ǩ
	SetInumPro(CTN_1000000_CTN_10001408_Num_10001409,m_byBetTime,2,true); 
	//����ʱ�ӵ���ʱ
	m_pGameFrame->SetTimer(TIME_ClOCK,1000);


}


//-------------------------------------------------------------------------
///��ʾ�����
void	CNewGameClient::ShowGameResult(void *buffer) 
{
	BeginShowResult *BeginJieSuan= (BeginShowResult *)buffer;

	if(NULL == BeginJieSuan)
	{
		return ;
	}

	//
	//��ʾ�����
	SetContainVisible(CTN_1000000_CTN_10001280,true);
	if(m_iMyDeskStation==m_byNtStation)//�Լ���ׯ��
	{
		
		//�÷�
		SetInumPro(CTN_1000000_CTN_10001280_Num_10001386,0,0,true);
		SetInumPro(CTN_1000000_CTN_10001280_Num_10001387,0,0,true);
		SetInumPro(CTN_1000000_CTN_10001280_Num_10001388,i64ChangeMoney[m_byNtStation],0,true);

		//�ɼ�
		if(255!=m_byNtStation)
		{
			SetTextinfo(CTN_1000000_CTN_10001360_TEXT_10001212,i64PlayerGrade[m_byNtStation],true);
		}
		SetTextinfo(CTN_1000000_CTN_10001362_TEXT_10001296,i64PlayerGrade[m_iMyDeskStation],true);

		if(i64ChangeMoney[m_iMyDeskStation]>0)
		{
			PlayGameSound(SOUND_GAME_O, FALSE);
		}
		else
		{
			PlayGameSound(SOUND_GAME_G, FALSE);
		}
	}
	else
	{
		
		SetInumPro(CTN_1000000_CTN_10001280_Num_10001386,i64ChangeMoney[m_iMyDeskStation],0,true);
		SetInumPro(CTN_1000000_CTN_10001280_Num_10001387,i64ReturnMoney[m_iMyDeskStation],0,true);
		SetInumPro(CTN_1000000_CTN_10001280_Num_10001388,i64ChangeMoney[m_byNtStation],0,true);

		
		//�ɼ�
		if(255!=m_byNtStation)
		{
			SetTextinfo(CTN_1000000_CTN_10001360_TEXT_10001212,i64PlayerGrade[m_byNtStation],true);
		}
		SetTextinfo(CTN_1000000_CTN_10001362_TEXT_10001296,i64PlayerGrade[m_iMyDeskStation],true);
		if(i64ChangeMoney[m_iMyDeskStation]>0)
		{
			PlayGameSound(SOUND_GAME_O, FALSE);
		}
		else
		{
			PlayGameSound(SOUND_GAME_G, FALSE);
		}
		
	}

	//���½��

	//�мҽ��
	UserInfoForGame UserInfo ; 
	memset(&UserInfo,0,sizeof(UserInfo));
	if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
	{
		/*CString cs;
		cs.Format("test ��ҽ��=%d",UserInfo.i64Money);
		OutputDebugString(cs);*/
		SetTextinfo(CTN_1000000_CTN_10001362_TEXT_10001293,UserInfo.i64Money,true);
	}

	//ׯ�ҽ��
	memset(&UserInfo,0,sizeof(UserInfo));
	if (m_pGameFrame->GetUserInfo(m_byNtStation , UserInfo))
	{
		/*CString cs;
		cs.Format("test ׯ�ҽ��=%d",UserInfo.i64Money);
		OutputDebugString(cs);*/
		SetTextinfo(CTN_1000000_CTN_10001360_TEXT_10001210,UserInfo.i64Money,true);
	}

	if(m_bUserExitIsClose)
	{
		TCHAR cs[100];
		_stprintf_s(cs, sizeof(cs), TEXT("���ֽ��:----------"));
		m_pGameFrame->InsertNormalMessage(cs);
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i==m_byNtStation)
			{
				if(m_pGameFrame->GetUserInfo(m_byNtStation, UserInfo))
				{

					TCHAR cs[100];
					_stprintf_s(cs, sizeof(cs), TEXT("%s(ׯ��)�÷�:%I64d"),UserInfo.szNickName, i64ChangeMoney[m_byNtStation]);
					m_pGameFrame->InsertNormalMessage(cs);

				}

			}
			else
			{
				if(m_pGameFrame->GetUserInfo(i, UserInfo))
				{

					TCHAR cs[100];
					_stprintf_s(cs, sizeof(cs), TEXT("%s(�м�)�÷�:%I64d"),UserInfo.szNickName, i64ChangeMoney[i]);
					m_pGameFrame->InsertNormalMessage(cs);

				}

			}
		}
		_stprintf_s(cs, sizeof(cs), TEXT("------------------"));
		m_pGameFrame->InsertNormalMessage(cs);
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
	/*if(!m_bSoundPlay)
	{
		return ;
	}*/
	if(!m_bSoundIsClose)
	{
		return;
	}
	CString  folder ;
	CString strPath = CINIFile::GetAppPath() ;
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 

	switch(SoundID)
	{
	case SOUND_GAME_A:	//��ע������
		{
			folder.Format("music/addchip.ogg");
			break;
		}
	case SOUND_GAME_B:	//ͼƬ����������
		{
			//folder.Format("Music/Sound/Animal1.wav");
			break;
		}
	case SOUND_GAME_C:	//��ʼ����
		{
			folder.Format("music/ReadyStart.ogg");
			break;
		}
	case SOUND_GAME_D:	//��500w�Ȳ���
		{
			folder.Format("music/chao500w.wav");
			break;
		}
	case SOUND_GAME_E:	//��ҽ����ʱ�򲥷ŵ�����
		{
			folder.Format("music/begingame.wav");
			break;
		}
	case SOUND_GAME_F:	//��Ϸ��ʼ������
		{
			folder.Format("music/gamebegin.wav");
			break;
		}
	case SOUND_GAME_G:	//�������
		{
			folder.Format("music/lost.wav");
			break;
		}
	case SOUND_GAME_H:	//��Ϸ����
		{
			//folder.Format("Music/Sound/Over.ogg");
			break;
		}
	case SOUND_GAME_I:	//����
		{
			folder.Format("music/sendcard.wav");
			break;
		}
	case SOUND_GAME_J:		//��ʼ��ע
		{
			folder.Format("music/ReadyXiaZhu.ogg");
			break;
		}
	case SOUND_GAME_K:	//100w������
		{
			folder.Format("music/sound100w.wav");
			break;
		}
	case SOUND_GAME_L://500w������
		{
			folder.Format("music/sound500w.wav");
			break;
		}
	case SOUND_GAME_M:	//�趯������
		{
			folder.Format("music/Start.ogg");
			break;
		}
	case SOUND_GAME_N:	//������ʱ������
		{
			folder.Format("music/timeout.ogg");
			break;
		}
	case SOUND_GAME_O:	//Ӯ������
		{
			folder.Format("music/win.wav");
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
//�������ֿռ���ʾ����
void	CNewGameClient::SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag)
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
//�����ı��ؼ���ʾ���ݣ���ʾ�ַ���
void	CNewGameClient::SetChinfo(const int iTextID,TCHAR ch[61], bool bFlag)
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
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 

	//m_bWatchMode = m_pGameFrame->GetIsWatching() ;

	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			if (nLen != sizeof(GameStation_Base))
			{
				return;
			}

			GameStation_Base * TGameStation = (GameStation_Base *)pBuffer;
			if(NULL == TGameStation)
			{
				return;
			}
			m_byGameStation = TGameStation->bGameStation;	//��Ϸ״̬
			m_byGameStation=GS_FREE_STATUS;//��Ϊ����״̬������������ׯ
			m_byFreeTime=TGameStation->byFreeTime;			//����ʱ��
			m_byBetTime=TGameStation->byBetTime;			//��עʱ��
			m_byOpenCardTime=TGameStation->bySendTime;	    //����ʱ��
			byAccountTime=TGameStation->byAccountTime;//����ʱ��
			m_byShowResult=TGameStation->byShowResult;//չʾ���ʱ��
			m_bySendSpaceTime=TGameStation->bySendCardSpaceTime;	//���Ƽ��ʱ��
			m_bySpeedTime=TGameStation->bySpeedTime;               //�ڵ����ƶ��ٶ�
			m_byShowWinTime=TGameStation->byShowWinTime;
			m_iClock=TGameStation->byRemaindTime;                     //ʣ��ʱ��

			m_i64ApplyNtLimite=TGameStation->iApplyNtLimite ; //ׯ����ׯ����

			//ׯ����Ϣ�������ͳɼ���
			m_byNtStation=TGameStation->byNtStation;		//ׯ��λ��

			m_i64NtScore=TGameStation->i64NtGrade;	//ׯ�ҳɼ�
			m_iNtPlayCount=TGameStation->iNtPlayCount;		//ׯ����ׯ����
			m_byMaxNtPeople=TGameStation->byMaxNtPeople;		//�����ׯ����
			m_iPlayGrade=TGameStation->i64PlayerGrade;//��ҳɿ�
			m_iPlayJuShu=TGameStation->byEvent;//��Ϸ����
			if(255!=m_byNtStation)
			{
				m_iPlayBaShu++;
			}
			//ׯ����Ϣ����
			ShowNtInfo(true);
			//��ׯ�б�����
			ShowNtWaitList(true);
			//��ʾ�Լ�����Ϣ
			ShowMyInfo(true);
			//�����ܷ���ע
			BtEnable(false);
			//��ע��ť������
			SetBetBtEnable(FALSE,1);

			HideLuzi();
			//������ׯ��ׯ��ť��ʹ�ú���ʾ
			UserInfoForGame UserInfo ; 
			memset(&UserInfo,0,sizeof(UserInfo));
			//�Լ����
			if (m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
			{
				if(UserInfo.i64Money>=m_i64ApplyNtLimite)
				{
					BtNtDisp(0);
				}
				else
				{
					BtNtDisp(2);
				}
			}      

			//��ʾ�׶�ͼƬ
			ShowSection(0);
			//����ע������
			//SetInumPro(CTN_1000000_CTN_10001421_Num_10001429,0,2,true);
			TCHAR cs[100];
			_stprintf_s(cs, sizeof(cs), TEXT("��ׯ���:%I64d"), m_i64ApplyNtLimite);
			m_pGameFrame->InsertNormalMessage(cs);
			PlayGameSound(SOUND_GAME_E,FALSE);
			//ViewLuzi();
			break; 
		}

	case GS_FREE_STATUS://���н׶�
		{
			if (nLen != sizeof(GameStation_Base))
			{
				return;
			}
			HandleFreeStation(pBuffer);
			break;
		}
	case GS_BET_STATUS:	//��Ϸ��ע�׶�
		{
			if (nLen != sizeof(GameStation_Base))
			{
				return;
			}
			HandleXiaZhuStation(pBuffer);
			break ;
		}
	case GS_SEND_STATUS://���ƽ׶�
		{
			if (nLen != sizeof(GameStation_Base))
			{
				return;
			}
			//��Ϸ״̬��Ϣ����
			HandleKaiPaiStation(pBuffer);
			break;
		}
	}
}
////�鿴·��
//void CNewGameClient::ViewLuzi(const int iContainID)
//{
//	IContain *pContain = NULL;
//	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
//	if (NULL != pContain)
//	{
//		pContain->SetControlVisible(TRUE);
//		IImage *image;
//		pContain->IInsertControl(image);
//
//		SetPicSize(const int iButtonID, CString sImagSrc,bool bFlag,int x,int y,int w,int h);
//	}
//}
//��ʾ�Լ�����Ϣ
void CNewGameClient::ShowMyInfo(bool bFalg)
{
	//��ȡ�û���Ϣ
	UserInfoForGame UserInfo ; 
	if (NULL==m_pGameFrame->GetUserInfo(m_iMyDeskStation , UserInfo))
	{
		return ;
	}
	
	//�Լ����ǳ�
	SetChinfo(BTN_PLAY_NAME,UserInfo.szNickName,bFalg);
	//�Լ��Ľ��
	SetTextinfo(BTN_PLAY_MONEY,UserInfo.i64Money,bFalg);
	//�Լ��İ���
	SetTextinfo(BTN_PLAY_BASHU,m_iPlayBaShu,bFalg);
	//�Լ��ľ���
	SetTextinfo(BTN_PLAY_ROUND,m_iPlayJuShu,bFalg);
	//�Լ��ĳɼ�
	SetTextinfo(BTN_PLAY_GRADGE,m_iPlayGrade,bFalg);
}
//-------------------------------------------------------------------------
//��ʾׯ����Ϣ
void	CNewGameClient::ShowNtInfo(bool bFalg)
{
	UserInfoForGame UserInfo ; 
	//�ж�ׯ���Ƿ���ڣ����ھ���ʾ
	if (m_pGameFrame->GetUserInfo(m_byNtStation , UserInfo) && m_byNtStation != 255)
	{
		
		//ׯ���ǳ�
		SetChinfo(BTN_KING_NAME,UserInfo.szNickName,bFalg);
		//ׯ�ҵĽ��
		SetTextinfo(BTN_KING_MONEY,UserInfo.i64Money,bFalg);
		//ׯ�ҵľ���
		SetTextinfo(BTN_KING_ROUND,m_iNtPlayCount,bFalg);
		//ׯ�ҵĳɼ�
		SetTextinfo(BTN_KING_GRADGE,m_i64NtScore,bFalg);
	}
	else
	{
		//ׯ���ǳ�
		SetChinfo(BTN_KING_NAME,UserInfo.szNickName,!bFalg);
		//ׯ�ҵĽ��
		SetTextinfo(BTN_KING_MONEY,UserInfo.i64Money,!bFalg);
		//ׯ�ҵľ���
		SetTextinfo(BTN_KING_ROUND,m_iNtPlayCount,!bFalg);
		//ׯ�ҵĳɼ�
		SetTextinfo(BTN_KING_GRADGE,m_i64NtScore,!bFalg);
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
	CString debg;
	debg.Format("exit-GameUserLeft(client)\n");
	OutputDebugString(debg);
	OutputDebugString("dwjlog::C_����뿪...");
	return 0;
}
/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	CString debg;
	debg.Format("exit-GameUserOffLine(client)\n");
	OutputDebugString(debg);
	OutputDebugString("dwjlog::C_��Ҷ���...");
	return 0;
}
// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	if (m_byGameStation > GS_FREE_STATUS && m_byGameStation < GS_WAIT_NEXT)
	{
		OutputDebugString("dwjlog::C_������Ϸ��...");
		return true;
	}
	
	return false;
}

///����˳���Ϸ
bool CNewGameClient::OnGameQuiting()
{

	CString debg ;
	debg.Format("exit-OnGameQuiting(Client)(debug)") ; 
	OutputDebugString(debg);
	if(m_pGameFrame->GetIsWatching())
	{
		OutputDebugString("exit-OnGameQuiting(Client)====0");
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

	if(m_byGameStation > GS_FREE_STATUS && m_byGameStation < GS_WAIT_NEXT)
	{
		OutputDebugString("exit-OnGameQuiting========1(Client)");
		if(bForceQuitAsAuto)
		{
			OutputDebugString("exit-OnGameQuiting========2(Client)");
			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪��Ϸ�����йܴ��� ���Ƿ�Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
			{
				return false ; 
			}	
		}
		else
		{
			OutputDebugString("exit-OnGameQuiting========3(Client)");
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
		case TIME_ClOCK:
		{
			m_iClockLable--;
			SetInumPro(CTN_1000000_CTN_10001408_Num_10001409,m_iClockLable,2,true);
			if(m_iClockLable<=5)
			{
				PlayGameSound(SOUND_GAME_N,FALSE);  
			}
			if(m_iClockLable<=0)
			{
				m_pGameFrame->KillTimer(TIME_ClOCK);
			}
			break;
		}
		case TIME_SENDLONG_CARD://��ʼ������
		{
			int x=0,y=0;
			//ɾ����ʱ��
			m_pGameFrame->KillTimer(TIME_SENDLONG_CARD);
			//��λ�÷���
			ShowUserSendCard(CTN_1000000_NoMoveCard_101317,m_byCardStyle[0],1,true);

			IImage *pImage = NULL;
			pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1000000_IMG_10001379));
			if (NULL != pImage)
			{
						
				x=pImage->GetGameX();
				y=pImage->GetGameY();
				SetImagePosition(CTN_1000000_IMG_10001330,x,y, true);
			}
					
			//��ʾ�����ڵ���
			SetImageVisible(CTN_1000000_IMG_10001330,true);
			//���������ƶ�ʱ��
			m_pGameFrame->SetTimer(TIME_SENDHU_CARD,m_bySendSpaceTime*1000);
			break;
		}
		case TIME_SENDHU_CARD://��ʼ������
			{
				int x=0,y=0;
				//ɾ����ʱ��
				m_pGameFrame->KillTimer(TIME_SENDHU_CARD);
				//��λ�÷���
				ShowUserSendCard(BTN_CHUPAI_HU,m_byCardStyle[1],1,true); 
				IImage *pImage = NULL;
				pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1000000_IMG_10001378));
				if (NULL != pImage)
				{
					x=pImage->GetGameX();
					y=pImage->GetGameY();
					SetImagePosition(CTN_1000000_IMG_10001331,x,y, true);
				}
				SetImageVisible(CTN_1000000_IMG_10001331,true);
				//��ʼ����
				m_pGameFrame->SetTimer(TIME_SHOW_JIEPAI,2*1000);
					
				break;
			}
		case TIME_SHOW_JIEPAI:
			{
				m_pGameFrame->KillTimer(TIME_SHOW_JIEPAI);
				PlayGameSound(SOUND_GAME_F, FALSE);
				//���������ڵ����ƶ���ʱ��
				m_pGameFrame->SetTimer(TIME_SHOW_LONG,m_bySpeedTime);
				break;
			}
		case TIME_SHOW_LONG://�����ƶ���
			{
				int x=0,y=0,end=0;
				IImage *pImage = NULL;
				pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1000000_IMG_10001379));
				if (NULL != pImage)
				{
					end=pImage->GetGameX();
				}
				pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1000000_IMG_10001330));
				if (NULL != pImage)
				{
					x=pImage->GetGameX();
					y=pImage->GetGameY();
					x++;
					if(x>(end+20))
					{
						m_pGameFrame->KillTimer(TIME_SHOW_LONG);
						PlayGameSound(SOUND_GAME_F, FALSE);
						//���������ڵ����ƶ���ʱ��
						m_pGameFrame->SetTimer(TIME_SHOW_HU,m_bySpeedTime);
						return;
					}
					else
					{
						SetImagePosition(CTN_1000000_IMG_10001330,x,y, true);

					}
				}
					
				break;
					
			}
		case  TIME_SHOW_HU://�����ƶ���
			{
				int x=0,y=0,end=0;
				IImage *pImage = NULL;
				pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1000000_IMG_10001378));
				if (NULL != pImage)
				{
					end=pImage->GetGameX();
				}
				pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1000000_IMG_10001331));
				if (NULL != pImage)
				{
					x=pImage->GetGameX();
					y=pImage->GetGameY();
					x++;
						
					if(x>(end+20))
					{
						//�����ж���Ӯ����
						m_pGameFrame->SetTimer(TIME_SHOW_WIN_TIME,m_byShowWinTime*1000);
						m_pGameFrame->KillTimer(TIME_SHOW_HU);
						return;
					}
					else
					{
						SetImagePosition(CTN_1000000_IMG_10001331,x,y, true);

					}
				}
				break;

			}
		case TIME_SHOW_WIN_TIME:
			{
				m_pGameFrame->KillTimer(TIME_SHOW_WIN_TIME);
				//m_pGameFrame->SetTimer(TIME_AWARDSHOW,500);
				if(0==m_byArea)
				{
					SetImageVisible(BTN_DRAGON_WINNER,true);
				}
				else if(1==m_byArea)
				{
					SetImageVisible(BTN_TIGER_WINNER,true);
				}
				else if(2==m_byArea)
				{
					SetImageVisible(BTN_AND_WINNER,true);
				}

				AccountResult();
				for(int i=0;i<m_dbyLuzi.size();i++)
				{

					if(0==m_dbyLuzi.at(i))
					{

						CString src(".\\resources\\image\\game_state.bmp");
						SetPicSize(BTN_LUZI_1+i,src, true,0,0,27,28);
					}
					else if(1==m_dbyLuzi.at(i))
					{
						CString src;
						src.Format(".\\resources\\image\\game_state.bmp");
						SetPicSize(BTN_LUZI_1+i,src,true,27,0,27,28);
					}
					else if(2==m_dbyLuzi.at(i))
					{

						CString src;
						src.Format(".\\resources\\image\\game_state.bmp");
						SetPicSize(BTN_LUZI_1+i,src, true,54,0,27,28);
					}
				}
				break;
			}
		case TIME_SHOW_HUANZHUANG:
			{
				m_pGameFrame->KillTimer(TIME_SHOW_HUANZHUANG);
				SetImageVisible(CTN_1000000_IMG_10001398,false);
				break;
			}
		case TIME_SHOW_MONEY:
			{
				SetImageVisible(CTN_1000000_CTN_10001361_IMG_10001399,false);
				m_pGameFrame->KillTimer(TIME_SHOW_MONEY);
				break;
			}
		case TIME_AWARDSHOW:
			{
				ShowAwardFrame();
				break;
			}
	}
}
//��������͵ĸ���
void CNewGameClient::ClearAccountResult()
{
	SetTextinfo(CTN_1000000_TEXT_10001405,0, true);
	SetTextinfo(CTN_1000000_TEXT_10001406,0, true);
	SetTextinfo(CTN_1000000_TEXT_10001407,0, true);
}
//���������͵ĸ���
void CNewGameClient::AccountResult()
{
	int _long=0,_hu=0,_he=0;
	//����72·�������������͵ĸ���
	for(int i=0;i<m_dbyLuzi.size();i++)
	{
		if(0==m_dbyLuzi.at(i))
		{
			_long++;
		}
		else if(1==m_dbyLuzi.at(i))
		{
			_hu++;
		}
		else if(2==m_dbyLuzi.at(i))
		{
			_he++;
		}
	}
	SetTextinfo(CTN_1000000_TEXT_10001405,_long, true);
	SetTextinfo(CTN_1000000_TEXT_10001406,_hu, true);
	SetTextinfo(CTN_1000000_TEXT_10001407,_he, true);
}
//��ʾ�н���
void CNewGameClient::ShowAwardFrame()
{

	BYTE flag=-1;
	//ѡ����ʾ����
	switch(m_byArea)
	{
	case 0:
		{
			if(m_bAwardDisplayFlag)
			{
				m_bAwardDisplayFlag=false;
				CString src;
				src.Format("./image/ractangleBigEnd.png");
				//SetImageVisible(CTN_1000000_CTN_10001478_IMG_10001479+m_byWinPrize,true);
				//SetImageLoadSrc(CTN_1000000_CTN_10001432_IMG_10001465,src,TRUE,0);
			}
			else
			{

				m_bAwardDisplayFlag=true;
				CString src;
				src.Format("./image/ractangleBig.png");
				//SetImageVisible(CTN_1000000_CTN_10001478_IMG_10001479+m_byWinPrize,FALSE);
				//SetImageLoadSrc(CTN_1000000_CTN_10001432_IMG_10001465,src,TRUE,0);
			}
			break;
		}
	
	case 1:
		{
			if(m_bAwardDisplayFlag)
			{
				m_bAwardDisplayFlag=false;
				CString src;
				src.Format("./image/ractangleBigEnd.png");
				//SetImageVisible(CTN_1000000_CTN_10001478_IMG_10001479+m_byWinPrize,true);
				//SetImageLoadSrc(CTN_1000000_CTN_10001432_IMG_10001469,src,TRUE,0);
			}
			else
			{
				m_bAwardDisplayFlag=true;
				CString src;
				src.Format("./image/ractangleBig.png");
				//SetImageVisible(CTN_1000000_CTN_10001478_IMG_10001479+m_byWinPrize,FALSE);
				//SetImageLoadSrc(CTN_1000000_CTN_10001432_IMG_10001469,src,TRUE,0);
			}
			break;
		}
	
	case 2:
		{
			if(m_bAwardDisplayFlag)
			{
				m_bAwardDisplayFlag=false;
				CString src;
				src.Format("./image/ractangleBigEnd.png");
				//SetImageVisible(CTN_1000000_CTN_10001478_IMG_10001479+m_byWinPrize,true);
				//SetImageLoadSrc(CTN_1000000_CTN_10001432_IMG_10001466,src,TRUE,0);
			}
			else
			{
				m_bAwardDisplayFlag=true;
				CString src;
				src.Format("./image/ractangleBig.png");
				//SetImageVisible(CTN_1000000_CTN_10001478_IMG_10001479+m_byWinPrize,FALSE);
				//SetImageLoadSrc(CTN_1000000_CTN_10001432_IMG_10001466,src,TRUE,0);
			}
			break;
		}
	
	}
}
///��ʼ����������
void CNewGameClient::ResetGameData()
{
	//��ս�������
	memset(i64PlayerGrade,0,sizeof(i64PlayerGrade));
	memset(i64ChangeMoney,0,sizeof(i64ChangeMoney));
	memset(i64ReturnMoney,0,sizeof(i64ReturnMoney));

	memset(m_i64AreaAllSum,0x00,sizeof(m_i64AreaAllSum));					//���������ֵ
	memset(m_i64RealityAreaAllSum,0x00,sizeof(m_i64RealityAreaAllSum));		//���������ע��ֵ

	m_bAwardDisplayFlag=TRUE;//����λ����ʾ��ǩ

	//���ؽ����
	SetContainVisible(CTN_1000000_CTN_10001280,FALSE);

	INoMoveCard  *pNoMoveCard = NULL ; 
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(CTN_1000000_NoMoveCard_101317))  ; 
	if(NULL != pNoMoveCard)
	{
		//����
		pNoMoveCard->SetControlVisible(FALSE) ; 
	}
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(BTN_CHUPAI_HU))  ; 

	if(NULL != pNoMoveCard)
	{
		//����
		pNoMoveCard->SetControlVisible(FALSE) ;
	}
	//�ڵ��Ʋ��ɼ�
	SetImageVisible(CTN_1000000_IMG_10001330,FALSE);
	SetImageVisible(CTN_1000000_IMG_10001331,FALSE);

	//��Ӯ...����
	SetImageVisible(BTN_DRAGON_WINNER,FALSE);
	SetImageVisible(BTN_TIGER_WINNER,FALSE);
	SetImageVisible(BTN_AND_WINNER,FALSE);


	m_i64PlayerCurrentMoney=0;//��ҵ�ǰ���

	//��������ϵĳ������ע������ʾ
	//��Ӧ������ʾ����ֵ
	for(int i=0;i<3;i++)
	{
		SetShCtrlInfo(BTN_BET_LEFT+i,0, FALSE);
	}
	//�����Լ���ע
	SetInumPro(CTN_1000000_CTN_10001369_Num_10001393,0,2,FALSE);
	SetInumPro(CTN_1000000_CTN_10001370_Num_10001395,0,2,FALSE);
	SetInumPro(CTN_1000000_CTN_10001371_Num_10001397,0,2,FALSE);
	//��������ע
	SetInumPro(CTN_1000000_CTN_10001369_Num_10001392,0,2,FALSE);
	SetInumPro(CTN_1000000_CTN_10001370_Num_10001394,0,2,FALSE);
	SetInumPro(CTN_1000000_CTN_10001371_Num_10001396,0,2,FALSE);
	
	m_SoundEngineGame->stopAllSounds();


}
