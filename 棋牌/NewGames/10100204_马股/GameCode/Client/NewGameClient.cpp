#include "stdafx.h"
#include "NewGameClient.h"
//#include "IGameFrame.h"
#include "../common/IGameFrame.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "UI_ForLogic/IUserInterface.h"
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

	m_isSuperDialogShow = false;
	m_isWinOrLose = FALSE;
	return 0;
}
//-----------------------------------------------------------------------------------
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	SetControlVisible(CTN_1014089_CTN_3000459,false);
	SetControlVisible(CTN_1014089_CTN_3000469,false);
	SetControlVisible(CTN_1014089_CTN_3000478,false);
	SetControlVisible(CTN_1014089_BTN_10001,false);
	SetControlVisible(CTN_1014089_OperateCard_3000489,false);
	SetControlVisible(CTN_1014089_BTN_3000493,false);
	SetControlVisible(CTN_1014089_CTN_3000509,false);

	SetControlVisible(CTN_1014089_BTN_3000492,false);

	CString appPath = CINIFile::GetAppPath();
	char szfileName[MAX_PATH] = {0};
	CString strFile;
	strFile.Format("./resources/sounds/bg.wav");
	wsprintf(szfileName,"%s%s",appPath,strFile);
	m_SoundEnginBg->setSoundVolume(0.4);
	m_SoundEnginBg->play2D(szfileName,true);
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
//�ƶ��ؼ��Ƿ���ʾ
bool CNewGameClient::GetControlShow(const int iID)
{
	IBCInterFace *pControl = NULL;
	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iID));
	if (NULL != iID)
	{
		return pControl->GetVisible();
	}
}

//�趨�ؼ���x,y,w,h
void CNewGameClient::SetControlPosition(const int iID,int x,int y,int w,int h)
{
	IBCInterFace *pControl = NULL;
	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iID));
	if (NULL != iID)
	{
		pControl->SetGameXY(x,y);
		pControl->SetGameW(w);
		pControl->SetGameH(h);
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
//��������ʽ���ֿؼ���ʾ���ݣ����Ӻš����š����ţ�
void CNewGameClient::SetNewInumInfo(const int iNumID, __int64 i64Num,bool bFlag)
{
	/*CString cs;
	cs.Format("lwlog::SetNewInumInfo=%d",iNumID);
	OutputDebugString(cs);*/
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
void CNewGameClient::SetControlPos(const int iControlID,int x,int y)
{
	IBCInterFace  *pControl = NULL ; 
	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID)); 
	if(NULL != pControl)
	{
		pControl->SetGameXY(x,y) ; 
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
void CNewGameClient::PlayIExAnimate(const int iID,int x,int y, bool bFlag)
{
	IExAnimate *pControl = NULL;
	pControl = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(iID));
	if (NULL != pControl)
	{
		pControl->SetIsSymmetrical(false);
		pControl->SetGameXY(x,y);
		pControl->SetIsLoop(false);
		pControl->SetCurveInstance(CT_NONE);
		pControl->SetPlayState(bFlag);
		//�ڼ�֡
		//pControl->SetShowFrame(iFramNum);
		pControl->SetControlVisible(bFlag);
	}
}
//����ָ��������ʾָ����֡��
void CNewGameClient::CloseAnimate(const int iID)
{
	IExAnimate *pControl = NULL;
	pControl = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(iID));
	if (NULL != pControl)
	{
		pControl->SetPlayState(false);
		pControl->SetControlVisible(false);
	}
}
//CStringתTCHAR
TCHAR* CNewGameClient::CStringToTCHAR(CString &str) 
{ 
	int iLen = str.GetLength(); 
	TCHAR* szRs = new TCHAR[iLen]; 
	lstrcpy(szRs, str.GetBuffer(iLen)); 
	str.ReleaseBuffer(); 
	return szRs; 
} 
/*
���ܣ��ɲ����Ʒ���
�βΣ�bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowOperateCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
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
���ܣ����ɲ����Ʒ���
�βΣ�bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowNoMoveCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
{

	INoMoveCard  *pOperate = NULL ;
	pOperate = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 
	if(NULL != pOperate)
	{
		pOperate->SetControlVisible(true) ; 
		pOperate->SetCardList(bCardList , cardCount) ; 
	}
}
//���ͼƬ�ؼ��Ƿ���ʾ
BOOL CNewGameClient::GetNoMoveIsVisible(const int iControlID)
{
	INoMoveCard  *pOperate = NULL ;
	pOperate = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 
	if(NULL != pOperate)
	{
		return pOperate->GetVisible();
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
//���ÿؼ��Ƿ�ɼ�
void CNewGameClient::SetControlVisible(const int iControlID,bool bFlag)
{
	IBCInterFace  *pControl = NULL ; 
	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID)); 
	if(NULL != pControl)
	{
		pControl->SetControlVisible(bFlag) ; 
	}
}
//-------------------------------------------------------------------------------
/// ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_byGameStation = (ENUM_GameStation)bGameStation; 
}
//-------------------------------------------------------------------------------
///��ȡ��Ϸ״̬
BYTE CNewGameClient::GetStationParameter()
{
	//OutputDebugString("hs [Client] 2: CNewGameClient::GetStationParameter");
	return m_byGameStation;
}

//---------------------------------------------------------------------------------------
///��ʼ����Ϸ�е�����
void	CNewGameClient::InitialGameData()
{
	m_iThinkTime=0;//��Ϸ˼��ʱ��
	m_iBeginTime=0;//��Ϸ��ʼʱ��		
	m_iMyDeskStation = 255;	///����Լ�����Ϸλ��

	m_ClockTickCount = TellGameModeTime;	//ʱ�Ӽ�ʱ��

	//׼����ť�ɼ�
	//SetControlVisible(CTN_1014089_BTN_3000492,true);

	SetStationParameter(/*GAMESTATION_NOPLAYER*/GS_NOMODE);
	//������Ϸ����
	ResetGameData();
}

//�ָ���Ϸ״̬
void CNewGameClient::GameOverReset()
{
	//TODO:��ʼ��������
	memset(m_byFirstHeap,0,sizeof(m_byFirstHeap));
	memset(m_bySecondHeap,0,sizeof(m_bySecondHeap));
	memset(m_byOwnCards,0,sizeof(m_byOwnCards));

	//׼����ť�ɼ�
	SetImageLoadSrc(CTN_1014089_BTN_3000492,CString("./resources/image/׼��.png"),true);
	SetControlVisible(CTN_1014089_BTN_3000492,true);
	SetBtEnable(CTN_1014089_BTN_3000492,true);

	int ibaseControlID = 3000700;
	IText *pText = NULL;
	for (int i = 0; i<PLAY_COUNT;++i)
	{
		int ibaseControlIDnew = ibaseControlID + 4*i;

		pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(ibaseControlIDnew));
		if (pText == NULL)
			return;
		pText->SetText(L"");	
		// 			CString cs;
		// 			cs.Format(_T("hs %s"),wbuffer);
		// 			OutputDebugString(cs);

		//�ǳ�
		pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(ibaseControlIDnew+1));
		if (pText == NULL)
			return;
		pText->SetText(L"");	

		//�÷�
		pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(ibaseControlIDnew +2));
		if (pText == NULL)
			return;
		pText->SetText(L"");

		//����
		pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(ibaseControlIDnew +3));
		if (pText == NULL)
			return;
		pText->SetText(L"");
	}
	//////���� �ơ�����ؼ����Լ������ؼ�
	//��
	SetControlVisible(CTN_1014089_NoMoveCard_3000487,false);		// 0-all
	SetControlVisible(CTN_1014089_NoMoveCard_3000488,false);		// 1-all
	SetControlVisible(CTN_1014089_OperateCard_3000489,false);		// 2-all
	SetControlVisible(CTN_1014089_NoMoveCard_3000490,false);		// 3-all

	SetControlVisible(CTN_1014089_NoMoveCard_3000499,false);		// 0-1st
	SetControlVisible(CTN_1014089_NoMoveCard_3000505,false);		// 1-1st
	SetControlVisible(CTN_1014089_NoMoveCard_3000502,false);		// 2-1st
	SetControlVisible(CTN_1014089_NoMoveCard_3000503,false);		// 3-1st

	SetControlVisible(CTN_1014089_NoMoveCard_3000504,false);		// 0-2th
	SetControlVisible(CTN_1014089_NoMoveCard_3000501,false);		// 1-2th
	SetControlVisible(CTN_1014089_NoMoveCard_3000506,false);		// 2-2th
	SetControlVisible(CTN_1014089_NoMoveCard_3000507,false);		// 3-2th
	
	//����
	SetControlVisible(CTN_1014089_IMG_3000518,false);				//0
	SetControlVisible(CTN_1014089_IMG_3000519,false);				//1
	SetControlVisible(CTN_1014089_IMG_3000508,false);				//2
	SetControlVisible(CTN_1014089_IMG_3000520,false);				//3

	//��������
	SetControlVisible(CTN_1014089_CTN_3000522,false);

	//���ư�ť
	SetControlVisible(CTN_1014089_BTN_3000493,false);

}

//������Ϸ����
void CNewGameClient::ResetGameData()
{																			
	memset(m_bUserPrepare,0,sizeof(m_bUserPrepare));
	
	SetControlVisible(CTN_1014089_BTN_3000493,false);
	
}
//����UI���� 
void	CNewGameClient::ResetGameDialog()
{

}
//------------------------------------------------------------------------------
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	CString appPath = CINIFile::GetAppPath();
	CString soundPath = CString("/resources/sounds/button.wav");
	char szFileName[MAX_PATH] = {0};

	m_SoundEngineGame->setSoundVolume(1.0);
	m_SoundEngineGame->play2D(szFileName,false);
	
	//��ť�������Ϣ
	bool RBselect;
	if(pMessage->_uMessageType == UI_LBUTTONUP)
		{
			if(m_pGameFrame->GetIsWatching())
			{
				return 0;
			}
			switch(pMessage->_uControlID)
			{
			case CTN_1014089_CTN_3000459_RButton_3000465:	 ///<��ѡ��-��ׯ
				{
					m_SoundEngineGame->play2D(szFileName,false);
					if (GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000466,&RBselect))
						SetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000466,false);
					if (GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000467,&RBselect))
						SetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000467,false);
				}
				break;
			case CTN_1014089_CTN_3000459_RButton_3000466:	 ///<��ѡ��-��ׯ
				{
					m_SoundEngineGame->play2D(szFileName,false);
					if (GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000465,&RBselect))
						SetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000465,false);
					if (GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000467,&RBselect))
						SetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000467,false);
				}
				
				break;
			case CTN_1014089_CTN_3000459_RButton_3000467:	 ///<��ѡ��-��ׯ
				{
					m_SoundEngineGame->play2D(szFileName,false);
					if (GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000465,&RBselect))
						SetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000465,false);
					if (GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000466,&RBselect))
						SetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000466,false);
				}
				
				break;

			case CTN_1014089_CTN_3000459_BTN_3000463:	//ȷ��ѡ����Ϸģʽ
				{
					m_SoundEngineGame->play2D(szFileName,false);
					OnBtnSelectGameModeOK();
				}
				
				break;
			case CTN_1014089_CTN_3000459_BTN_3000464:	//�˳���Ϸģʽѡ�� ���˳��������Ϸ��
				{
					m_SoundEngineGame->play2D(szFileName,false);
					// TODO: ���߷���������˳�
					ExitWithoutSelectMode();
				}
				
				break;
			case CTN_1014089_CTN_3000478_BTN_3000479:	//ģʽ֪ͨ������ȷ����ť
				{
					m_SoundEngineGame->play2D(szFileName,false);
					m_pGameFrame->KillTimer(TIMER_SHOWGAMEMODEINFO);

					SetTimePlay(CTN_1014089_CTN_3000478_BTN_3000479,false,m_ClockTickCount);//ȷ����ť
					SetControlVisible(CTN_1014089_CTN_3000478,false);		//������Ϸģʽ֪ͨ��ť

					//SetControlVisible(CTN_1014089_BTN_3000492,true);	//����׼����ť�ɼ�
				}
				break;
			case CTN_1014089_BTN_3000492:		//׼����ť
				{
					m_SoundEngineGame->play2D(szFileName,false);
					m_pGameFrame->KillTimer(TIME_BEGIN);

					SetImageLoadSrc(CTN_1014089_BTN_3000492,CString("./resources/image/׼������.png"),true);
					SetBtEnable(CTN_1014089_BTN_3000492,false);	//׼����ť������״̬
					SetControlVisible(CTN_1014089_Time_3000719,false);
					
					TGameStation tGameStation ;
					tGameStation.byGameDeskNO = m_pGameFrame->GetMyDeskStation();					
					//m_pGameFrame->SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GETREADY,0);
					m_pGameFrame->ClickContinue();
				}
				break;
			case CTN_1014089_OperateCard_3000489:	//�ɲ�����
				{
					m_SoundEngineGame->play2D(szFileName,false);
					//���÷��ư�ť�Ƿ����
					if(GetUpNums(CTN_1014089_OperateCard_3000489) == 3)
						SetBtEnable(CTN_1014089_BTN_3000493,true);
					else
						SetBtEnable(CTN_1014089_BTN_3000493,false);						
				}
				break;
			case CTN_1014089_BTN_3000493:		//<����  ��Ť
				{
					m_SoundEngineGame->play2D(szFileName,false);
					//���ֳ����Ʒ��͸�������
				
					m_pGameFrame->KillTimer(TIMER_THINK);
					m_ClockTickCount = 15;
					SetControlVisible(CTN_1014089_Time_3000719,false);

					SendCardToServer();
					//������ɺ󣬽����ư�ť����Ϊ���ɼ�
					SetControlVisible(CTN_1014089_BTN_3000493,false);
					//���ɲ���������Ϊ���ɼ�
					SetControlVisible(CTN_1014089_OperateCard_3000489,false);
				}
				break;
			case CTN_1014089_CTN_3000509_BTN_3000510:	//ע��1 ��ť, 
				{
					m_SoundEngineGame->play2D(szFileName,false);
					m_pGameFrame->KillTimer(TIMER_BET);
					
					SetControlVisible(CTN_1014089_Time_3000719,false);	//���ض�ʱ��

					Betting(CTN_1014089_CTN_3000509_BTN_3000510);	//��ע
					SetControlVisible(CTN_1014089_CTN_3000509,false);	//����Ͷע����

				}
				break;
			case CTN_1014089_CTN_3000509_BTN_3000511:	//ע��2 ��ť
				{
					m_SoundEngineGame->play2D(szFileName,false);
					m_pGameFrame->KillTimer(TIMER_BET);
					
					SetControlVisible(CTN_1014089_Time_3000719,false);	//���ض�ʱ��
					Betting(CTN_1014089_CTN_3000509_BTN_3000511);	//��ע
					SetControlVisible(CTN_1014089_CTN_3000509,false);	//����Ͷע����

				}
				break;
			case CTN_1014089_CTN_3000509_BTN_3000512:	//ע��3 ��ť
				{
					m_SoundEngineGame->play2D(szFileName,false);
					m_pGameFrame->KillTimer(TIMER_BET);
					
					SetControlVisible(CTN_1014089_Time_3000719,false);	//���ض�ʱ��
					Betting(CTN_1014089_CTN_3000509_BTN_3000512);	//��ע
					SetControlVisible(CTN_1014089_CTN_3000509,false);	//����Ͷע����
					
				}
				break;
			case CTN_1014089_CTN_3000509_BTN_3000513:	//ע��4 ��ť
				{
					m_SoundEngineGame->play2D(szFileName,false);
					m_pGameFrame->KillTimer(TIMER_BET);
					m_ClockTickCount = 15;
					SetControlVisible(CTN_1014089_Time_3000719,false);	//���ض�ʱ��
					Betting(CTN_1014089_CTN_3000509_BTN_3000513);	//��ע
					SetControlVisible(CTN_1014089_CTN_3000509,false);	//����Ͷע����

				}
				break;
			case CTN_1014089_CTN_3000522_BTN_3000543:	//��������- ������Ϸ
				{
					m_SoundEngineGame->play2D(szFileName,false);
					//���ÿͻ���״̬�����ݡ� ������Ϣ��������ʹ��������Ϸ״̬������
					m_pGameFrame->KillTimer(TIMER_SHOWSETTLE);	
					GameOverReset();				
				}
				break;
			case CTN_1014089_CTN_3000522_BTN_3000544:	//��������- �˳���Ϸ
				{
					m_SoundEngineGame->play2D(szFileName,false);
					m_pGameFrame->KillTimer(TIMER_SHOWSETTLE);
					m_pGameFrame->CloseClient();
				}
				break;
			case CTN_1014089_CTN_3000727_BTN_3000728:	//��ׯ
				{
					m_pGameFrame->KillTimer(TIMER_BETBANKER);
					TBetBanker tBetBanker;
					tBetBanker.isBet = true;
					tBetBanker.byDeskStation = m_pGameFrame->GetMyDeskStation();
					m_pGameFrame->SendGameData(&tBetBanker,sizeof(tBetBanker),MDM_GM_GAME_NOTIFY,GM_BETBANKER,0);

					SetControlVisible(CTN_1014089_CTN_3000727,false);
				}
				break;
			case CTN_1014089_CTN_3000727_BTN_3000729:	//����
				{
					m_pGameFrame->KillTimer(TIMER_BETBANKER);
					TBetBanker tBetBanker;
					tBetBanker.isBet = false;
					tBetBanker.byDeskStation = m_pGameFrame->GetMyDeskStation();
					m_pGameFrame->SendGameData(&tBetBanker,sizeof(tBetBanker),MDM_GM_GAME_NOTIFY,GM_BETBANKER,0);

					SetControlVisible(CTN_1014089_CTN_3000727,false);
				}
				break;
			#pragma region ����
			case CTN_1014089_BTN_3000755:	//���˰�ť
				{
					if(!m_isSuperDialogShow)
					{
						SetControlVisible(CTN_1014089_CTN_3000736,true);
					}
				}
				break;
			case CTN_1014089_CTN_3000736_BTN_3000754:	//����-ȷ����ť
				{
					TSuper tsuper;
					bool bSelectWin;
					bool bSelectLose;
					GetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000737,&bSelectWin);
					GetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000743,&bSelectLose);
					if(!bSelectLose && !bSelectWin)
					{
						tsuper.IsSetSuper = false;
					}
					else
					{
						tsuper.IsSetSuper = true;
						if(bSelectWin)
							tsuper.isWinOrLose = TRUE;
						else
							tsuper.isWinOrLose = FALSE;
						
					}
					m_pGameFrame->SendGameData(&tsuper,sizeof(TSuper),MDM_GM_GAME_NOTIFY,ASS_WINORLOSE,0);
					SetControlVisible(CTN_1014089_CTN_3000736,false);
					m_isSuperDialogShow = false;
				}
				break;
			case CTN_1014089_CTN_3000736_BTN_3000753:	//����-ȡ����ť
				{
					SetControlVisible(CTN_1014089_CTN_3000736,false);
					m_isSuperDialogShow = false;
				}
				break;
			case CTN_1014089_CTN_3000736_RButton_3000737:	//ׯӮ
				{
					SetWinOrLose(CTN_1014089_CTN_3000736_RButton_3000737,TRUE);
					SetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000743,false);
				}
				break;
			case CTN_1014089_CTN_3000736_RButton_3000743:	//ׯ��
				{
					SetWinOrLose(CTN_1014089_CTN_3000736_RButton_3000743,FALSE);
					SetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000737,false);
				}
				break;
			#pragma region comment

			//case CTN_1014089_CTN_3000736_RButton_3000739:	//��1Ӯ
			//	{
			//		SetWinOrLose(CTN_1014089_CTN_3000736_RButton_3000739,TRUE);
			//		SetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000744,false);
			//	}
			//	break;
			//case CTN_1014089_CTN_3000736_RButton_3000744://	��1��
			//	{
			//		SetWinOrLose(CTN_1014089_CTN_3000736_RButton_3000744,FALSE);
			//		SetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000739,false);
			//	}
			//	break;
			//case CTN_1014089_CTN_3000736_RButton_3000741:	//��2Ӯ
			//	{
			//		SetWinOrLose(CTN_1014089_CTN_3000736_RButton_3000741,TRUE);
			//		SetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000745,false);
			//	}
			//	break;
			//case CTN_1014089_CTN_3000736_RButton_3000745:	//��2��
			//	{
			//		SetWinOrLose(CTN_1014089_CTN_3000736_RButton_3000745,FALSE);
			//		SetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000741,false);
			//	}
			//	break;
			//case CTN_1014089_CTN_3000736_RButton_3000742:	//��3Ӯ
			//	{
			//		SetWinOrLose(CTN_1014089_CTN_3000736_RButton_3000742,TRUE);
			//		SetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000746,false);
			//	}
			//	break;
			//case CTN_1014089_CTN_3000736_RButton_3000746:	//��3��
			//	{
			//		SetWinOrLose(CTN_1014089_CTN_3000736_RButton_3000746,FALSE);
			//		SetRButtonSelected(CTN_1014089_CTN_3000736_RButton_3000742,false);
			//	}
			//	break;
			#pragma endregion comment
			#pragma endregion ����
			case CTN_2_BTN_14:	//���� -
				{
					irrklang::ik_f32 ivolume = m_SoundEnginBg->getSoundVolume();
					ivolume = (ivolume - 0.2) <=0 ? 0 :(ivolume - 0.2);
					m_SoundEnginBg->setSoundVolume(ivolume);
				}
				break;
			case CTN_2_BTN_13:	//����+
				{
					irrklang::ik_f32 ivolume = m_SoundEnginBg->getSoundVolume();
					ivolume = (ivolume + 0.2) >=1 ? 1 :(ivolume + 0.2);
					m_SoundEnginBg->setSoundVolume(ivolume);
				}
				break;
			case CTN_2_BTN_15:	//������
				{
					m_SoundEnginBg->setSoundVolume(0.0);
				}
				break;
			case CTN_2_BTN_16:	//������
				{
					m_SoundEnginBg->setSoundVolume(0.4);
				}
				break;
			case CTN_2_BTN_11:	//�رհ�ť
				{
					NetMessageHead tNetMessageHead;
					tNetMessageHead.bAssistantID = ASS_GM_FORCE_QUIT;
					m_pGameFrame->SendGameData(ASS_GM_FORCE_QUIT,&tNetMessageHead,sizeof(tNetMessageHead));
					m_pGameFrame->CloseClient();
				}
				break;
			}
		}
	//���Ŷ��������Ϣ
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		switch(pMessage->_uControlID)
		{
		}
	}
	return 0;
}

//��������Ƶ�����
int CNewGameClient::GetUpNums(const int iControlID)
{
	BYTE bCardList[13];
	int iNums=0;
	memset(bCardList,0,sizeof(BYTE)*13);
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID)); 
	if(NULL != pOperateCard)
	{
		pOperateCard->GetUpCardList(bCardList);
	}
	for(int i=0;i<13;i++)
	{
		if(m_Logic.TestData(bCardList[i],1))
		{
			iNums++;
		}
	}
	return iNums;
}

//��Ϸģʽѡ��ʧ��
void CNewGameClient::ExitWithoutSelectMode()
{
 	TGameStation tGameStation;
	m_pGameFrame->KillTimer(TIMER_SELECTGAMEMODE);	//�ر���Ϸģʽѡ��ʱ�䶨ʱ��
 	SetTimePlay(CTN_1014089_CTN_3000459_Time_3000483,false,m_ClockTickCount);	//ֹͣ���ŵ���ʱ
	m_ClockTickCount = 15;
 
 	tGameStation.enGameStation = GS_NOMODE;//GAMESTATION_EXITSELECTMODE;
 	tGameStation.enGameMode = GAMEMODE_INVALIDE;
 	tGameStation.byGameDeskNO = m_pGameFrame->GetMyDeskStation();
 	m_pGameFrame->SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GAMESTATIONCHANGED,0);
 
 	m_pGameFrame->CloseClient();						//�˳���Ϸ	
}

//���ű�����Ч
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	CString  folder ;
	CString strPath = CINIFile::GetAppPath() ;
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	folder.Format("./resources/music/��������.ogg");
	wsprintf(szFileName,"%s%s",strPath ,folder);
	m_SoundEngineGame->setSoundVolume(1.0);
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}
/// ������Ϸ�ĵ�ǰ״̬
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	SetControlVisible(CTN_1014089_CTN_3000459,false);	//ģʽ����
	SetControlVisible(CTN_1014089_CTN_3000478,false);	//ģʽ֪ͨ

	TGameStation *pGameStation = (TGameStation*)pBuffer;
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation();//�Լ�����λ��
	m_byGameStation = pGameStation->enGameStation;
	ENUM_GameMode gameMode ;	//��Ϸģʽ
	TCHAR buffer[100] = {0};	//��ʾ��Ϣ������
	

	switch(m_byGameStation)
	{
	case GS_NOMODE:	//û�����, ��ʾģʽ���öԻ���
		{
			TGameStation tGameStation;
			tGameStation.byGameDeskNO = m_iMyDeskStation;
			tGameStation.enGameStation = GS_WITHMODE;
			m_pGameFrame->SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GAMESTATIONCHANGED,0);		//�ı���Ϸ״̬
			//SetControlVisible(CTN_1014089_CTN_3000459,true);				//��ʾģʽ����
			//SetControlVisible(CTN_1014089_CTN_3000459_Time_3000483,true);	//��ʾ��ʱ��
			//SetTimePlay(CTN_1014089_CTN_3000459_Time_3000483,true,m_ClockTickCount);
			//
			//m_pGameFrame->SetTimer(TIMER_SELECTGAMEMODE,15*1000);
			//������ʱ��
			m_ClockTickCount = 15;
			m_pGameFrame->SetTimer(TIME_BEGIN,1*1000);
			SetControlVisible(CTN_1014089_BTN_3000492,true);	//׼����ť����
		}
		break;
	case GS_MODESETTING:	//ģʽ���� 
		{
			//SetControlVisible(CTN_1014089_CTN_3000478,true);	//��ʾģʽ֪ͨ
			//SetControlVisible(CTN_1014089_CTN_3000478_Time_3000484,true);	//ʱ���
			//SetTimePlay(CTN_1014089_CTN_3000478_Time_3000484,true,m_ClockTickCount);
			//SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("�������������Ϸģʽ�����Ե�!"),true);

			//m_pGameFrame->SetTimer(TIMER_SHOWGAMEMODEINFO,15*1000);
			//SetTimePlay(CTN_1014089_CTN_3000478_Time_3000484,true,m_ClockTickCount);
			//SetInumInfo(CTN_1014089_CTN_3000478_Time_3000484,m_ClockTickCount,true);
			SetControlVisible(CTN_1014089_BTN_3000492,true);	//׼����ť����
		}
		break;

	case GS_WITHMODE:		//��һ����ң�������һ�����׼�����ɿ�ʼ
		{	
			//gameMode = pGameStation->enGameMode;	
			//SetControlVisible(CTN_1014089_CTN_3000478,true);	//��ʾģʽ֪ͨ
			/*	if (gameMode == GAMEMODE_INVALIDE)
			{
			SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("�������������Ϸģʽ�����Ե�!"),true);
			}
			else if(gameMode == GAMEMODE_BANKER_BET)
			{
			SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ !"),true);
			SetTextShowText(CTN_1014089_TEXT_3000477,__TEXT("��ׯ"),true);
			}
			else if (gameMode == GAMEMODE_BANKER_CONST)
			{
			SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ !"),true);
			SetTextShowText(CTN_1014089_TEXT_3000477,__TEXT("��ׯ"),true);
			}
			else if (gameMode == GAMEMODE_BANKER_INTURN)
			{
			SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ !"),true);
			SetTextShowText(CTN_1014089_TEXT_3000477,__TEXT("��ׯ"),true);
			}*/
			//����׼����ť�ɼ�
			SetControlVisible(CTN_1014089_BTN_3000492,true);	//׼����ť����	
		}		
		break;
	case GS_BETTING:
	case GS_READY:			//׼��4
		//{
		//	SetControlVisible(CTN_1014089_BTN_3000492,true);	//׼����ť�ɼ�
		//}
		//break;
	case GS_GAMEBEGIN:			//��Ϸ��5
	case GS_WASHCARDS:			//ϴ����6
	case GS_SENDCARDS:			//������7
	//case GS_BETTING:
	case GS_FINDBANKER:
	case GS_PK:			//������
		{
			SetControlVisible(CTN_1014089_BTN_3000492,false);	//׼����ť���ɼ�
		}
		break;
	case GS_SETTLE:				//����8
	case GS_GAMEOVER:			//��Ϸ����9
		{
			SetControlVisible(CTN_1014089_BTN_3000492,true);	//׼����ť����

			//gameMode = pGameStation->enGameMode;	
			//SetControlVisible(CTN_1014089_CTN_3000478,true);	//��ʾģʽ֪ͨ
			//if (gameMode == GAMEMODE_INVALIDE)
			//{
			//	SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("�������������Ϸģʽ�����Ե�!"),true);
			//}
			//else if(gameMode == GAMEMODE_BANKER_BET)
			//{
			//	SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ !"),true);
			//	SetTextShowText(CTN_1014089_TEXT_3000477,__TEXT("��ׯ"),true);
			//}
			//else if (gameMode == GAMEMODE_BANKER_CONST)
			//{
			//	SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ !"),true);
			//	SetTextShowText(CTN_1014089_TEXT_3000477,__TEXT("��ׯ"),true);
			//}
			//else if (gameMode == GAMEMODE_BANKER_INTURN)
			//{
			//	SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ !"),true);
			//	SetTextShowText(CTN_1014089_TEXT_3000477,__TEXT("��ׯ"),true);
			//}
		}
		break;
	}
}

//-----------------------------------------------------------------------------------------
/// ��Ϸ��Ϣ
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

			if(pUser->bDeskStation == m_iMyDeskStation)
			{
				SetControlVisible(CTN_1014089_BTN_3000492,false);	//׼����ť�ɼ�
			}	
		}
		break;

	case ASS_GM_GAME_STATION:   ///<����״̬(ƽ̨���õ�)  -- ����ҽ�����Ϸ������ʱ��ƽ̨��ͻ��˷��͵���Ϣ���ÿͻ���������Ϸ״̬
		{
			SetGameStation(buffer,nLen);	
			break;  
		}
	case ASS_SUPER:
		{
			SetControlVisible(CTN_1014089_BTN_3000755,true);
		}
		break;
	case GS_NOMODE:
		{
			SetGameStation(buffer,nLen);
			break;
		}
	case GS_WITHMODE:
		{
			SetGameStation(buffer,nLen);
		}
		break;
	case GM_GAMEMODESELECTED:
		GameModeSelectedMessage(buffer);
		break;
	case GM_BETBANKER:
		{
			//��ʾ��ׯ����
			SetControlVisible(CTN_1014089_CTN_3000727,true);
			//������ʱ���������û����ׯ���Զ����Ͳ���ׯ��Ϣ
			m_pGameFrame->SetTimer(TIMER_BETBANKER,5000);
		}
		break;
	//��ע
	case GM_BET:
		{
			T_SC_WhoIsBanker *ptBanker = (T_SC_WhoIsBanker *)buffer;
			if(ptBanker == NULL)
				break;
			int iView = m_pGameFrame->DeskStation2View(ptBanker->byDeskStation);
			int ipicbase = CTN_1014089_IMG_3000732;
			SetControlVisible(ipicbase + iView,true);	//��ʾׯ��ͼƬ

			if((int)ptBanker->byDeskStation == m_pGameFrame->GetMyDeskStation())
			{
				//m_pGameFrame->ClickContinue();
				break;
			}
			else
			{
				m_pGameFrame->SetTimer(TIMER_BET,15*1000);
				SetControlVisible(CTN_1014089_Time_3000719,true);
				SetTimePlay(CTN_1014089_Time_3000719,true,m_ClockTickCount);
				ShowBet();
			}
		}
		break;

	case ASS_HANDSUP:
		{
			BYTE *pbyDeskStation = (BYTE*)buffer;
			if(pbyDeskStation == nullptr)
			{
				break;
			}
			/*if(*pbyDeskStation == m_pGameFrame->GetMyDeskStation())
			{*/
			m_pGameFrame->ClickContinue();
			//}
		}
		break;
	//ϴ��
	case GM_WASHCARD:
		{
			//1. ����׼����ť
			SetControlVisible(CTN_1014089_BTN_3000492,false);
			//����ϴ�ƶ���
		}
		break;

	//����
	case GM_SENDCARD:
		{
			SetControlVisible(CTN_1014089_BTN_3000492,false);
			T_SC_Cards *tCards = (T_SC_Cards*)buffer;
			if(tCards == NULL)
				break;
			else
			{
				SetControlVisible(CTN_1014089_OperateCard_3000489,true);
				ShowOperateCard(CTN_1014089_OperateCard_3000489,tCards->m_cards,5);	//��ʾ��Ч��
				
			}
			memset(m_byOwnCards,0,sizeof(m_byOwnCards));	//�����ڴ�
			memcpy(m_byOwnCards,tCards->m_cards,5);			//������ҵ���

			for (int i = 0; i <PLAY_COUNT; ++i)
			{
				UserInfoForGame tUserinfo;
				if(m_pGameFrame->GetUserInfo(i,tUserinfo))
				{
					int iView = m_pGameFrame->DeskStation2View(tUserinfo.bDeskStation);
					BYTE byCardList[5] = {80};
					ShowNoMoveCard(CTN_1014089_NoMoveCard_3000487 +iView,byCardList,5);

				}
			}
			//���ư�ť�ɼ�����������
			SetControlVisible(CTN_1014089_BTN_3000493,true);
			SetBtEnable(CTN_1014089_BTN_3000493,false);

			//������ʱ��
			m_pGameFrame->SetTimer(TIMER_THINK,15*1000);
			SetControlVisible(CTN_1014089_Time_3000719,true);
			SetTimePlay(CTN_1014089_Time_3000719,true,m_ClockTickCount);
			//SetInumInfo(CTN_1014089_Time_3000719,m_ClockTickCount,true);
		}
		break;

	case GM_SHOWTOALL:	//����
		{
			ShowCards(buffer);
		}
		break;
	//����
	case GM_RESULT:
		{
			ShowCardsResult(buffer);
		}
		break;
	//����
	case GM_SETTLE:
		{
			SetControlVisible(CTN_1014089_CTN_3000522,true);
			m_pGameFrame->SetTimer(TIMER_SHOWSETTLE,3000);
			OnSettle(buffer);

			//����ׯ�ұ�ʶ
			SetControlVisible(CTN_1014089_IMG_3000732,false);
			SetControlVisible(CTN_1014089_IMG_3000733,false);
			SetControlVisible(CTN_1014089_IMG_3000734,false);
			SetControlVisible(CTN_1014089_IMG_3000735,false);
		}
		break;
	case GM_RESET_STATION:
		{
			//GameOverReset();
			//m_pGameFrame->KillTimer(TIMER_BET);
			//m_pGameFrame->KillTimer(TIMER_THINK);
			//SetControlVisible(CTN_1014089_Time_3000719,false);	//���ض�ʱ��
			//SetControlVisible(CTN_1014089_CTN_3000509,false);

			//����ׯ�б�
			SetControlVisible(CTN_1014089_IMG_3000732,false);
			SetControlVisible(CTN_1014089_IMG_3000733,false);
			SetControlVisible(CTN_1014089_IMG_3000734,false);
			SetControlVisible(CTN_1014089_IMG_3000735,false);
			if(buffer == NULL)
				return 0;
			if(nLen != sizeof(TGameStation))
				return 0;

			TGameStation* gamestation = (TGameStation*)buffer;
			int iView = m_pGameFrame->DeskStation2View(gamestation->byGameDeskNO);
			SetControlVisible(CTN_1014089_IMG_3000732 + iView, true);
			SetControlVisible(CTN_1014089_BTN_3000492,true);
			/*SetImageLoadSrc(CTN_1014089_BTN_3000492,CString("./resources/image/׼��.png"),true);
			SetControlVisible(CTN_1014089_BTN_3000492,true);
			SetBtEnable(CTN_1014089_BTN_3000492,true);

			SetGameStation(buffer,nLen);*/
		}
		break;
	default:
		break;
	}
	return 0;
}


void CNewGameClient::OnSettle(void *pData)
{
	T_SC_Settle *ptSettle = (T_SC_Settle*)pData;
	if(ptSettle == NULL)
		return;
	int ibaseControlID = 3000700;

	IText *pText = NULL;
	for (int i = 0; i<PLAY_COUNT;++i)
	{
		char buffer[50] = {0};
		int ibaseControlIDnew = ibaseControlID + 4*i;
		WCHAR wbuffer[50] = {0};
		if(ptSettle->iScore[i] != 0)
		{
			//ׯ����
			memset(wbuffer,0,sizeof(wbuffer));
			GetPlayerRole(ptSettle->bIsBanker[i],wbuffer);
			pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(ibaseControlIDnew));
			if (pText == NULL)
				return;
			pText->SetText(wbuffer);	
// 			CString cs;
// 			cs.Format(_T("hs %s"),wbuffer);
// 			OutputDebugString(cs);

			//�ǳ�
			pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(ibaseControlIDnew+1));
			if (pText == NULL)
				return;
			pText->SetText(CA2W(ptSettle->cbNickName[i]));	

			//�÷�
			pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(ibaseControlIDnew +2));
			if (pText == NULL)
				return;
			sprintf(buffer,"%I64d",ptSettle->iScore[i]);
			pText->SetText(CA2W(buffer));

			//����
			memset(wbuffer,0,sizeof(wbuffer));
			pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(ibaseControlIDnew +3));
			if (pText == NULL)
				return;
			GetCardShapeStr(ptSettle->enCardShape[i],wbuffer);
			pText->SetText(wbuffer);
		}
	}


}

void CNewGameClient::GetPlayerRole(bool bBanker,WCHAR* wbuffer)
{
	if(bBanker)
		memcpy(wbuffer,L"ׯ��",sizeof(L"ׯ��"));
	else
		memcpy(wbuffer,L"�м�",sizeof(L"�м�"));
}

void CNewGameClient::GetCardShapeStr(EnCardShape enCardShape,WCHAR *buffer)
{
	switch(enCardShape)
	{
	case CS_NOTEN:
		memcpy(buffer,L"�޴�",sizeof(L"�޴�"));
		break;
	case CS_OT_ONE:
		memcpy(buffer,L"�д�һ��",sizeof(L"�д�һ��"));
		break;
	case CS_OT_TWO:
		memcpy(buffer,L"�дն���",sizeof(L"�дն���"));
		break;
	case CS_OT_THREE:
		memcpy(buffer,L"�д�����",sizeof(L"�д�����"));
		break;
	case CS_OT_FOUR:
		memcpy(buffer,L"�д��ĵ�",sizeof(L"�д��ĵ�"));
		break;
	case CS_OT_FIVE:
		memcpy(buffer,L"�д����",sizeof(L"�д����"));
		break;
	case CS_OT_SIX:
		memcpy(buffer,L"�д�����",sizeof(L"�д�����"));
		break;
	case CS_OT_SEVEN:
		memcpy(buffer,L"�д��ߵ�",sizeof(L"�д��ߵ�"));
		break;
	case CS_OT_EIGHT:
		memcpy(buffer,L"�дհ˵�",sizeof(L"�дհ˵�"));
		break;
	case CS_OT_NIGHT:
		memcpy(buffer,L"�дվŵ�",sizeof(L"�дվŵ�"));
		break;
	case CS_DOUBLETEN:
		memcpy(buffer,L"���",sizeof(L"���"));
		break;
	case CS_DTENANDDF:
		memcpy(buffer,L"ͭ��",sizeof(L"ͭ��"));
		break;
	case CS_SMALLHORSE:
		memcpy(buffer,L"С˳��",sizeof(L"С˳��"));
		break;
	case CS_BIGHORSE:
		memcpy(buffer,L"��˳��",sizeof(L"��˳��"));
		break;
	case CS_TENHORSE:
		memcpy(buffer,L"ʮ��",sizeof(L"ʮ��"));
		break;
	case CS_BOOM:
		memcpy(buffer,L"����",sizeof(L"����"));
		break;
	}
}

//��ʾ���ƽ��
void CNewGameClient::ShowCardsResult(void *pBuffer)
{
	T_SC_CardResult *ptResult = (T_SC_CardResult*)pBuffer;
	if(ptResult == NULL)
		return;

	int iControlID = 0;
	int iView = m_pGameFrame->DeskStation2View(ptResult->byDeskStation);
	switch(iView)
	{
	case 0:
		iControlID = CTN_1014089_IMG_3000518;
		break;
	case 1:
		iControlID = CTN_1014089_IMG_3000519;
		break;
	case 2:
		iControlID = CTN_1014089_IMG_3000508;
		break;
	case 3:
		iControlID = CTN_1014089_IMG_3000520;
		break;
	}

	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *> (m_pUI->GetIControlbyID(iControlID));
	if(pImage == NULL)
		return;

	SetControlVisible(iControlID,true);
	switch (ptResult->enCardShape)
	{
	case CS_NOTEN:	//�޴�
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/wucou.png")));
		break;
	case CS_OT_ONE:	//�д�1��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/1.png")));
		break;
	case CS_OT_TWO:	//�д�2��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/2.png")));
		break;
	case CS_OT_THREE:	//�д�3��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/3.png")));
		break;
	case CS_OT_FOUR:	//�д�4��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/4.png")));
		break;
	case CS_OT_FIVE:	//�д�5��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/5.png")));
		break;
	case CS_OT_SIX:		//�д�6��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/6.png")));
		break;
	case CS_OT_SEVEN:	//�д�7��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/7.png")));
		break;
	case CS_OT_EIGHT:	//�дհ˵�
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/8.png")));
		break;
	case CS_OT_NIGHT:	//�д�9��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/9.png")));
		break;
	case CS_DOUBLETEN:	//���
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/magu.png")));
		break;
	case CS_DTENANDDF:	//ͭ��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/tongchui.png")));
		break;
	case CS_SMALLHORSE:	//С˳��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/xiaoshunma.png")));
		break;
	case CS_BIGHORSE:	//��˳��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/dashunma.png")));
		break;
	case CS_TENHORSE:	//ʮ��
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/shima.png")));
		break;
	case CS_BOOM:	//����
		pImage->LoadPic(CA2W(CString("./resources/image/cardshape/sitiao.png")));
		break;
	case CS_INVALID:
		break;
	}
}

// ��Ϸģʽѡ��ɹ�
void CNewGameClient::GameModeSelectedMessage(BYTE *pData)
{
	
	TGameStation *pGameStation = (TGameStation*)pData;
	if (pGameStation == NULL)
	{
		OutputDebugString("hs Client recivce Empty Message");
		return ;
	}
	if (pGameStation == NULL)
		return;
// 	if(m_pGameFrame->GetIsWatching())
// 	{
// 
// 	}
	ENUM_GameMode gameMode = pGameStation->enGameMode;
		
	if (GetControlShow(CTN_1014089_CTN_3000478))
		SetControlVisible(CTN_1014089_CTN_3000478,false);
	SetControlVisible(CTN_1014089_CTN_3000478,true);	//��ʾģʽ֪ͨ
	SetControlVisible(CTN_1014089_CTN_3000478_Time_3000484,true);	//ʱ���
	SetTimePlay(CTN_1014089_CTN_3000478_Time_3000484,true,m_ClockTickCount);
	if (gameMode == GAMEMODE_INVALIDE)
	{
		SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("�������������Ϸģʽ�����Ե�!"),true);
		SetTextShowText(CTN_1014089_TEXT_3000477,__T("δ����"),true);
	}
	else if(gameMode == GAMEMODE_BANKER_BET)
	{
		SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ!"),true);
		SetTextShowText(CTN_1014089_TEXT_3000477,__T("��ׯ"),true);
	}
	else if (gameMode == GAMEMODE_BANKER_CONST)
	{
		SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ!"),true);
		SetTextShowText(CTN_1014089_TEXT_3000477,__T("��ׯ"),true);
	}
	else if (gameMode == GAMEMODE_BANKER_INTURN)
	{
		SetTextShowText(CTN_1014089_CTN_3000478_TEXT_3000481,__T("������ϷģʽΪ�� ��ׯ!"),true);
		SetTextShowText(CTN_1014089_TEXT_3000477,__T("��ׯ"),true);
	}
	m_pGameFrame->SetTimer(TIMER_SHOWGAMEMODEINFO,15*1000);
	//SetInumInfo(CTN_1014089_CTN_3000478_Time_3000484,m_ClockTickCount,true);
	SetTimePlay(CTN_1014089_CTN_3000478_Time_3000484,true,m_ClockTickCount);


	
}

//��������״̬
int	CNewGameClient::ResetGameFrame(void)
{

	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	int iView = m_pGameFrame->DeskStation2View(nIndex);
	SetControlVisible(CTN_1014089_IMG_3000732 + iView, false);

	if(m_bUserPrepare[nIndex])
	{
		int viewStation=m_pGameFrame->DeskStation2View(nIndex);
		//��ʾ׼��ͼƬ
		//SetControlVisible(CTN_1014089_IMG_2000288+viewStation,false);
	}
	if(m_pGameFrame->GetIsWatching())
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
	return false;
}

///����˳���Ϸ
bool CNewGameClient::OnGameQuiting()
{
	if(m_pGameFrame->GetIsWatching() || !m_bUserPrepare[m_iMyDeskStation])
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
	m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);

	bool   bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strKeyName ,1) ; 
	
	//if(GS_READY<m_byGameStation && m_byGameStation<GS_SETTLE)
	if(m_byGameStation > GS_WITHMODE && m_byGameStation < GS_GAMEOVER)
	{
		if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪�����ɱ����Ļ����˴���Ŷ��", "��ʾ",AFC_YESNO)) 
		{
			return false ; 
		}
		m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}
	return true;
}
/// ��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
	case TIMER_SELECTGAMEMODE:	//��Ϸģʽѡ��ʱ��
		{
			m_pGameFrame->KillTimer(TIMER_SELECTGAMEMODE);
			TGameStation tGameStation;
			tGameStation.byGameDeskNO = m_pGameFrame->GetMyDeskStation();
			tGameStation.enGameMode = GAMEMODE_INVALIDE;
			tGameStation.enGameStation = GS_NOMODE;
			m_pGameFrame->SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GAMESTATIONCHANGED,0);
			m_pGameFrame->SendGameData(NULL,0,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);		//�˳���Ϸ
			m_pGameFrame->CloseClient();
		}
		break;
	case TIMER_SHOWGAMEMODEINFO:	// ��Ϸģʽ֪ͨ��ʱ��
		{
			m_pGameFrame->KillTimer(TIMER_SHOWGAMEMODEINFO);
			SetControlVisible(CTN_1014089_CTN_3000478,false);
		}
		break;
	case TIMER_BET:		//�����û�е����ע�����Զ���ע
		{
			m_pGameFrame->KillTimer(TIMER_BET);
			//������ע��Ϣ
			UserInfoForGame tUserinfo;
			m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),tUserinfo);
			__int64 iUserMoney = tUserinfo.i64Money;
			__int64 iBetMoney  = iUserMoney / 100;

			T_CS_Bet tBet;
			tBet.iBetMony = iBetMoney;
			tBet.byDeskStation = m_pGameFrame->GetMyDeskStation();
			m_pGameFrame->SendGameData(&tBet,sizeof(tBet),MDM_GM_GAME_NOTIFY,GM_BET,0);

			int iButtonIDs = CTN_1014089_CTN_3000509_BTN_3000510;
			for (int i=0;i<4;++i)
			{
				IButton *pIButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(iButtonIDs+i));
				if(pIButton == NULL)
					return;
				SetControlVisible(iButtonIDs+i,true);
				WCHAR buffer[30] = {0};
				pIButton->SetText(buffer);
			}
			SetControlVisible(CTN_1014089_CTN_3000509,false);		//������ע����
			SetControlVisible(CTN_1014089_Time_3000719,false);		//���ؼ�ʱ��
			
			//m_pGameFrame->ClickContinue();	//����
		}
		break;
	case TIMER_THINK:
		{
			m_pGameFrame->KillTimer(TIMER_THINK);
			SetControlVisible(CTN_1014089_Time_3000719,false);	// view or deskstation?
			//ѡ�����ƣ����ͳ�ȥ
			T_CS_OutCard tOutCard;
			tOutCard.byDeskStation = m_pGameFrame->GetMyDeskStation();
			memcpy(tOutCard.firstHeap,m_byOwnCards,sizeof(BYTE)*3);
			memcpy(tOutCard.SecondHeap,&m_byOwnCards[3],sizeof(BYTE)*2);
			memcpy(tOutCard.allCards,m_byOwnCards,sizeof(BYTE)*5);
			tOutCard.cardCount = 5;
			m_pGameFrame->SendGameData(&tOutCard,sizeof(tOutCard),MDM_GM_GAME_NOTIFY,GM_PK,0);

			SetControlVisible(CTN_1014089_BTN_3000493,false);
		}
		break;
	case TIMER_NOTIFYMODE:
		{
			m_pGameFrame->KillTimer(TIMER_NOTIFYMODE);
		
			SetControlVisible(CTN_1014089_CTN_3000478,false);	//����֪ͨ�������ɼ�
			SetControlVisible(CTN_1014089_BTN_3000492,true);	//����׼����ť�ɼ�
		}
		break;
	case TIMER_BETBANKER:	//��ׯ��ʱ���������û����ׯʱ���Զ����Ͳ���ׯ��Ϣ
		{
			m_pGameFrame->KillTimer(TIMER_BETBANKER);

			TBetBanker tBetBanker;
			tBetBanker.isBet = false;
			tBetBanker.byDeskStation = m_pGameFrame->GetMyDeskStation();
			m_pGameFrame->SendGameData(&tBetBanker,sizeof(tBetBanker),MDM_GM_GAME_NOTIFY,GM_BETBANKER,0);
		//	m_pGameFrame->ClickContinue();

			SetControlVisible(CTN_1014089_CTN_3000727,false);
		}
		break;
	case TIMER_SHOWSETTLE:
		{
			m_pGameFrame->KillTimer(TIMER_SHOWSETTLE);
			SetControlVisible(CTN_1014089_BTN_3000492,true);	//׼����ť�ɼ�
			m_ClockTickCount = 15;
			m_pGameFrame->SetTimer(TIME_BEGIN,m_ClockTickCount * 1000);
		}
		break;
	case TIME_BEGIN:
		{
			SetControlVisible(CTN_1014089_Time_3000719,true);
			SetTimePlay(CTN_1014089_Time_3000719,true,m_ClockTickCount);
			m_ClockTickCount--;
			if(m_ClockTickCount <= 0)
				m_pGameFrame->CloseClient();
		}
		break;
	default:
		break;
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
	CString  folder ;
	CString strPath = CINIFile::GetAppPath() ;
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	UserInfoForGame Info;
	m_pGameFrame->GetUserInfo(m_iMyDeskStation,Info);
	if(Info.bBoy)
	{
		folder.Format("./resources/music/boy/");
	}
	else
	{
		folder.Format("./resources/music/girl/");
	}
	switch(SoundID)
	{
	case SOUND_BUTTON://��ť
		{
			folder.Empty();
			folder.Format("./resources/music/button.wav");
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

void CNewGameClient::CloneImage(const int iID, int iGoalID)
{
	IImage *_pControl = NULL;
	_pControl=dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iID));
	IImage *pControl = NULL;
	if (NULL != _pControl)
	{
		pControl = dynamic_cast<IImage *>(_pControl->Clone(iGoalID));//��¡
		if (NULL != pControl)
		{
			if (NULL == m_pUI->GetIControlbyID(pControl->GetGameControlID()))
			{
				m_pUI->IInsertControl(pControl);
			}
		}
	}
	return;
}

/*
@brief:���Ŷ���
@param:ACTION_ID,�����ؼ�ID��CONTROL_ID �����ƵĿؼ�,startXY �������,endXY �յ�����
@return:void
*/
void CNewGameClient::PlayIMoveAction(const int ACTION_ID,const int CONTROL_ID,POINT startXY,POINT endXY,int second)
{
	//��ö����ؼ�ID
	IMoveAction* pMoveAction = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,ACTION_ID);
	//��ñ����Ƶ�ID
	INoMoveCard *pNoMoveCard = NULL;
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(CONTROL_ID));
	if(NULL!=pMoveAction && NULL!=pNoMoveCard)
	{
		pMoveAction->SetControlingCtrlID(CONTROL_ID);
		pMoveAction->SetStartP(startXY);
		pMoveAction->SetEndP(endXY);
		pMoveAction->SetTotalMoveTime(second);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
	}
}

// ѡ����Ϸģʽ ȷ����ť
void CNewGameClient::OnBtnSelectGameModeOK()
{
	bool RBselect;
	TGameStation tGameStation;	//ģʽ��״̬����λ��
	tGameStation.byGameDeskNO = m_pGameFrame->GetMyDeskStation();//��λ��
	tGameStation.enGameStation = GS_WITHMODE;//GAMESTATION_GAMEMODESELECTED;

	if ((GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000465,&RBselect) == FALSE) &&
		(GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000466,&RBselect) == FALSE) &&
		(GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000467,&RBselect) == FALSE)
		)		//��δѡ���κ���Ϸģʽʱ�����ȷ����ť��Ч
		return;

	m_pGameFrame->KillTimer(TIMER_SELECTGAMEMODE);	//�ر���Ϸģʽѡ��ʱ�䶨ʱ��
	SetTimePlay(CTN_1014089_CTN_3000459_Time_3000483,false,m_ClockTickCount); //ֹͣ���ŵ���ʱ
	m_ClockTickCount = 15;

	//TODO: ֪ͨ��������Ϸģʽ���趨
	//if(GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000465,&RBselect))		//ѡ����ׯģʽ
	//{
	//	tGameStation.enGameMode = GAMEMODE_BANKER_INTURN;
	//}
	//else if (GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000466,&RBselect))	//ѡ��ׯģʽ
	//{
	//	tGameStation.enGameMode = GAMEMODE_BANKER_CONST;
	//}
	//else if (GetRButtonSelected(CTN_1014089_CTN_3000459_RButton_3000467,&RBselect))	//ѡ����ׯģʽ
	//{
	//	tGameStation.enGameMode = GAMEMODE_BANKER_BET;
	//}
	m_pGameFrame->SendGameData(&tGameStation,sizeof(tGameStation),MDM_GM_GAME_NOTIFY,GM_GAMESTATIONCHANGED,0);	//�������֪ͨ��Ϸģʽ���óɹ�����������������Ϸ״̬����Ϊ GAMESTATION_WATINGBEGIN��
	SetControlVisible(CTN_1014089_CTN_3000459,false);
	m_pGameFrame->SetTimer(TIMER_NOTIFYMODE,15*1000);
	SetTimePlay(CTN_1014089_CTN_3000478_Time_3000484,true,m_ClockTickCount); 
	
	//m_pGameFrame->ClickContinue();
	SetControlVisible(CTN_1014089_BTN_3000492,false);	//׼����ť�ɼ�

}
//���ܷ�����Ϣ������
void CNewGameClient::OnReveiveSendCard(void *pBuffer)
{
// 	T_SC_SendCard *pSendCard=(T_SC_SendCardd *)pBuffer;
// 	if(NULL==pSendCard)
// 	{
// 		return;
// 	}
// 	int iView=m_pGameFrame->DeskStation2View(0);
}


void CNewGameClient::SendCardToServer()
{
	//��ȡ��һ����
	GetUpCard(CTN_1014089_OperateCard_3000489,m_byFirstHeap);
	int counter = 0;
	for (int i = 0 ;i<3;++i)
	{
		if(m_byFirstHeap[i] != 0)
			++counter;
	}

	//��ȡ�ڶ�����
	vector<BYTE> car2;
	car2.clear();
	for (int i =0 ;i<5;++i)
	{
		int Count = 0;
		for (int j = 0;j<3;++j)
		{	
			if (m_byOwnCards[i] != m_byFirstHeap[j])
			{
				++Count;
				if(Count == 3)
					car2.push_back(m_byOwnCards[i]);	
			}
		}
	}
	if(car2.size() != 2)
	{
		char buffer[100] = {0};
		sprintf(buffer,"hs SendCard erorr, sizeof secondheap: %d",car2.size());
		OutputDebugString(buffer);
		return;
	}

	for (int i = 0;i< car2.size();++i)
		m_bySecondHeap[i] = car2[i];

	//�������ݷ��͸�������
	T_CS_OutCard tOutCard;
	tOutCard.cardCount = counter +2;
	tOutCard.byDeskStation = m_pGameFrame->GetMyDeskStation();
	memcpy(tOutCard.firstHeap,m_byFirstHeap,sizeof(BYTE)*3);		//������һ����
	memcpy(tOutCard.SecondHeap,m_bySecondHeap,sizeof(BYTE)*2);		//�����ڶ�����
	memcpy(tOutCard.allCards,m_byOwnCards,sizeof(BYTE)*5);			//����������

	m_pGameFrame->SendGameData(&tOutCard,sizeof(tOutCard),MDM_GM_GAME_NOTIFY,GM_PK,0);
}

void CNewGameClient::ShowBet()
{
	UserInfoForGame tUserinfo;
	m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),tUserinfo);

	__int64 iUserMoney = tUserinfo.i64Money;
	
	m_iBetMoney[0] = iUserMoney / 1000;
	m_iBetMoney[1] = iUserMoney / 500;
	m_iBetMoney[2] = iUserMoney / 200;
	m_iBetMoney[3] = iUserMoney / 100;

	SetControlVisible(CTN_1014089_CTN_3000509,true);

	IImage *pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(CTN_1014089_CTN_3000509_IMG_3000517));
	if(pImage == NULL)
	{
		OutputDebugString("hs can't get the image contorl");
		return ;
	}
	pImage->LoadPic(CA2W(CString("./resources/image/bettext.png")));
	
	int iButtonID = CTN_1014089_CTN_3000509_BTN_3000510;
	for (int i = 0;i<4;++i)
	{
		IButton *pIButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(iButtonID+i));
		if(pIButton == NULL)
			return;

		SetControlVisible(iButtonID+i,true);
		WCHAR buffer[30] = {0};
		swprintf_s(buffer,sizeof(buffer),L"%I64d",m_iBetMoney[i]);	//��ʾ��ע���

		pIButton->SetText(buffer);
		
	}
}

void CNewGameClient::Betting(int iButtonID)
{
	__int64 iBetMoney = 0;

	switch(iButtonID)
	{
	case CTN_1014089_CTN_3000509_BTN_3000510:
		iBetMoney = m_iBetMoney[0];
		break;
	case CTN_1014089_CTN_3000509_BTN_3000511:
		iBetMoney = m_iBetMoney[1];
		break;
	case CTN_1014089_CTN_3000509_BTN_3000512:
		iBetMoney = m_iBetMoney[2];
		break;
	case CTN_1014089_CTN_3000509_BTN_3000513:
		iBetMoney = m_iBetMoney[3];
		break;
	}
	if(iBetMoney == 0)	//Ͷעʧ��
	{
		OutputDebugString("hs Ͷעʧ��");
		return ;
	}

	T_CS_Bet tBet;
	tBet.iBetMony = iBetMoney;
	tBet.byDeskStation = m_pGameFrame->GetMyDeskStation();

	m_pGameFrame->SendGameData(&tBet,sizeof(tBet),MDM_GM_GAME_NOTIFY,GM_BET,0);
	
	//m_pGameFrame->ClickContinue();	//����

	SetControlVisible(CTN_1014089_CTN_3000509,true);	//������ע����ɼ�

	int iButtonIDs = CTN_1014089_CTN_3000509_BTN_3000510;
	for (int i=0;i<4;++i)
	{
		IButton *pIButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(iButtonIDs+i));
		if(pIButton == NULL)
			return;
		SetControlVisible(iButtonIDs+i,true);
		WCHAR buffer[30] = {0};
		pIButton->SetText(buffer);
	}
}

void CNewGameClient::ShowCards(BYTE* pData)
{
	T_SC_OutCard *ptCards = (T_SC_OutCard*)pData;
	if(ptCards == NULL)
		return;
	int iView = m_pGameFrame->DeskStation2View(ptCards->byDeskStation);


	switch (iView)
	{
	case 0:
		{
			SetControlVisible(CTN_1014089_NoMoveCard_3000487,false);
			//��ʾ���ɲ�����
			ShowNoMoveCard(CTN_1014089_NoMoveCard_3000499,ptCards->firstHeap,3);
			ShowNoMoveCard(CTN_1014089_NoMoveCard_3000504,ptCards->secondHeap,2);
			//��ʾ��������
			ShowCardShapeInfo(ptCards->enCardShape,CTN_1014089_IMG_3000518);
		}
		break;
	case 1:
		{
			SetControlVisible(CTN_1014089_NoMoveCard_3000488,false);
			//��ʾ���ɲ�����
			ShowNoMoveCard(CTN_1014089_NoMoveCard_3000505,ptCards->firstHeap,3);
			ShowNoMoveCard(CTN_1014089_NoMoveCard_3000501,ptCards->secondHeap,2);

			//��ʾ��������
			ShowCardShapeInfo(ptCards->enCardShape,CTN_1014089_IMG_3000519);
		}
		break;
	case 2:
		{
			SetControlVisible(CTN_1014089_OperateCard_3000489,false);
			//��ʾ���ɲ�����
			ShowNoMoveCard(CTN_1014089_NoMoveCard_3000502,ptCards->firstHeap,3);
			ShowNoMoveCard(CTN_1014089_NoMoveCard_3000506,ptCards->secondHeap,2);

			//��ʾ��������
			ShowCardShapeInfo(ptCards->enCardShape,CTN_1014089_IMG_3000508);
		}
		break;
	case 3:
		{
			SetControlVisible(CTN_1014089_NoMoveCard_3000490,false);
			//��ʾ���ɲ�����
			ShowNoMoveCard(CTN_1014089_NoMoveCard_3000503,ptCards->firstHeap,3);
			ShowNoMoveCard(CTN_1014089_NoMoveCard_3000507,ptCards->secondHeap,2);

			//��ʾ��������
			ShowCardShapeInfo(ptCards->enCardShape,CTN_1014089_IMG_3000520);
		}
		break;
	}
}

void CNewGameClient::ShowCardShapeInfo(EnCardShape enCardShape,int ControlID)
{
	CString appPath = CINIFile::GetAppPath();
	CString sndPaht = CString("/resources/sounds/");
	char szFilePath[MAX_PATH] = {0};
	m_SoundEngineGame->setSoundVolume(0.5);

	IImage *pImage = NULL;
	SetControlVisible(ControlID,true);
	switch (enCardShape)
	{
	case CS_NOTEN: 	//�޴�
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/wucou.png")));

			CString strone = CString("noc.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		}
		break;
	case CS_OT_ONE:			//�д�һ��
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/1.png")));

			CString strone = CString("yc1-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
			
		}
		break;
	case CS_OT_TWO:			//�дն���
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/2.png")));

			CString strone = CString("yc2-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
			
		}
		break;
	case CS_OT_THREE:		//�д�����
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/3.png")));

			CString strone = CString("yc3-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_OT_FOUR:			//�д��ĵ�
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/4.png")));

			CString strone = CString("yc4-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_OT_FIVE:			//�д����
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/5.png")));

			CString strone = CString("yc5-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
			
		}
		break;
	case CS_OT_SIX:			//�д�����
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/6.png")));

			CString strone = CString("yc6-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
			
		}
		break;
	case CS_OT_SEVEN:		//�д��ߵ�
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/7.png")));

			CString strone = CString("yc7-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
			
		}
		break;
	case CS_OT_EIGHT:		//�дհ˵�
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/8.png")));
			
			CString strone = CString("yc8-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_OT_NIGHT:		//�дվŵ�
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/9.png")));

			CString strone = CString("yc9-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_DOUBLETEN:		//���
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/magu.png")));

			CString strone = CString("magu-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_DTENANDDF:		//ͭ��
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/tongchui.png")));

			CString strone = CString("tongc-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_SMALLHORSE:		//С˳��
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/xiaoshunma.png")));

			CString strone = CString("xiaosm-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_BIGHORSE:		//��˳��
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/dashunma.png")));

			CString strone = CString("dasm-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_TENHORSE:			//ʮ��
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/shima.png")));

			CString strone = CString("shim-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
			
		}
		break;
	case CS_BOOM:			//����
		{
			pImage= dynamic_cast<IImage*>(m_pUI->GetIControlbyID(ControlID));
			if(pImage == NULL)
				return;
			pImage->LoadPic(CA2W(CString("./resources/image/cardshape/sitiao.png")));

			CString strone = CString("sit-ptnv1.wav");
			wsprintf(szFilePath,"%s%s%s",appPath,sndPaht,strone);
			m_SoundEngineGame->play2D(szFilePath,false);
		
		}
		break;
	case CS_INVALID:			//��Ч
		OutputDebugString("hs ��show cardshape inform��CS_INVALID");
		break;
	default:
		OutputDebugString("hs ��show cardshape inform��default:");
		break;
	}
}

//������Ӯ
void CNewGameClient::SetWinOrLose(int iControlID,BOOL IsWin)
{
	bool bSelect;

	GetRButtonSelected(iControlID,&bSelect);
	if(bSelect)
	{
		m_isWinOrLose = IsWin;
	}
	else
	{
		m_isWinOrLose = (!IsWin);
	}
}