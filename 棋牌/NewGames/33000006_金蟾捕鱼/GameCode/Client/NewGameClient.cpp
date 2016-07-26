#include "stdafx.h"
#include "NewGameClient.h"
#include "../common/IGameFrame.h"
//#include "UI8_ControlsDefine.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "LoveSendClassInExe.h"
#include "define.h"
#include <windows.h>

//#include "LoveSendClassInExe.h"


IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
	return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
	delete *ppImp;
	*ppImp = NULL;
}
/*------------------------------------------------------------------------------*/
CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_GameScreen.SetGameClientP(this);
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;
	m_bGameStation = GS_WAIT_SETGAME;           //��Ϸ״̬
}
/*------------------------------------------------------------------------------*/
CNewGameClient::~CNewGameClient()
{

}
/*------------------------------------------------------------------------------*/
//��Ϸ�˳�
bool CNewGameClient::OnGameQuiting()
{
	return true;
}

//���������ļ�

/*------------------------------------------------------------------------------*/
/// ��ʼ��
int		CNewGameClient::Initial()
{
	LoadGameSet();
	//��Ϊ gamelauncher ���ɵĴ���û��ô�� ��������ǿ������ CLIENT_WIDTH*CLIENT_HEIGHT�Ĵ��� 

	RECT lpSrcWindow;
	if(GetWindowRect(m_pUI->GetWindowsHwnd(), &lpSrcWindow))
	{
		int iMoveX = (CLIENT_WIDTH-1024)/2;
		int iMoveY = (CLIENT_HEIGHT-738)/2;
		CWnd::FromHandle(m_pUI->GetWindowsHwnd())->MoveWindow(lpSrcWindow.left-iMoveX,lpSrcWindow.top-iMoveY,CLIENT_WIDTH,CLIENT_HEIGHT);
	}

	InitCommonControls();
	
    //���봴����Ϸ��ͼ����
	if(m_GameScreen.GetSafeHwnd()==NULL)
	{
		m_GameScreen.Create(AfxRegisterWndClass(0,NULL,NULL,NULL),NULL,WS_CHILD,CRect(0,0,CLIENT_WIDTH,CLIENT_HEIGHT),CWnd::FromHandle(m_pUI->GetWindowsHwnd()),0);
		
		if(!m_GameScreen.InitDxDlog())
		{
			AfxMessageBox("�豸����ʧ�ܣ����򼴽��˳���");
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
			return TRUE;  
		}
	    
		m_GameScreen.MoveWindow(0,0,CLIENT_WIDTH,CLIENT_HEIGHT);
		m_GameScreen.ShowWindow(SW_SHOW);
		m_GameScreen.SetWindowText("�������ͻ���");
	}
	return 0;
}
/*------------------------------------------------------------------------------*/
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	return 0;
}
/*------------------------------------------------------------------------------*/
/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	switch(nSubId)
	{
	case ASS_GM_GAME_STATION:           ///<����״̬
		{			    
			SetGameStation(buffer,nLen);
			break;
		}
	case ASS_S_USER_COME:		//��ҽ���/�뿪��Ϣ
		{
			if(nLen != sizeof(GameUserComeData))
			{
				return TRUE;
			}

			GameUserComeData *pGameUserCome = (GameUserComeData *)buffer;
			if(NULL == pGameUserCome)
			{
				return TRUE;
			}
			//��ҽ���
			m_GameScreen.UserComeInorLeave(SwitchChairID(pGameUserCome->byDeskStation),pGameUserCome->bComeOrLeave);
			//��ҵ��Ϸ���
			m_GameScreen.SetUserScore(SwitchChairID(pGameUserCome->byDeskStation),pGameUserCome->iScore);
			//��ҵ��ڻ���
			m_GameScreen.SetBeiLv(SwitchChairID(pGameUserCome->byDeskStation),pGameUserCome->iFireBase);

			break;
		}
	case ASS_BUILD_TRACE:	//���������Ϣ
		{
			if(nLen != sizeof(CMD_S_FishTrace))
			{
				return TRUE;
			}

			CMD_S_FishTrace *pFishTrace = (CMD_S_FishTrace *)buffer;
			if(NULL == pFishTrace)
			{
				return TRUE;
			}
			for(int i=0;i<5;i++)
			{
				//ѭ���������켣
				m_GameScreen.AddFish(pFishTrace->m_fishtrace[i].x,pFishTrace->m_fishtrace[i].y,pFishTrace->m_fishtrace[i].rotation,pFishTrace->m_fishtrace[i].movetime,pFishTrace->m_fishtrace[i].changetime,pFishTrace->m_fishtrace[0].m_fishtype,pFishTrace->m_fishtrace[0].m_ptindex,i,pFishTrace->m_fishtrace[0].m_fishid,pFishTrace->m_fishtrace[0].m_fudaifishtype,pFishTrace->m_fishtrace[i].m_speed);
			}
			break;
		}
	case ASS_CHANGE_SCENCE:		//�л�����
		{
			if(nLen != sizeof(CMD_S_ChangeScene))
			{
				return TRUE;
			}
			CMD_S_ChangeScene *pChangeScene=(CMD_S_ChangeScene *)buffer;
			if(NULL == pChangeScene)
			{
				return TRUE;
			}
			m_GameScreen.ChangeScreen(pChangeScene->SceneIndex);
			break;
		}
	case ASS_FISH_REG:		//����
		{
			if(nLen != sizeof(CMD_S_FishTrace))
			{
				return TRUE;
			}

			CMD_S_FishTrace *pFishTrace = (CMD_S_FishTrace *)buffer;
			if(NULL == pFishTrace)
			{
				return TRUE;
			}
			for(int i=0;i<5;i++)
			{
				//ѭ���������켣
				m_GameScreen.AddFish(pFishTrace->m_fishtrace[i].x,pFishTrace->m_fishtrace[i].y,pFishTrace->m_fishtrace[i].rotation,pFishTrace->m_fishtrace[i].movetime,pFishTrace->m_fishtrace[i].changetime,pFishTrace->m_fishtrace[0].m_fishtype,pFishTrace->m_fishtrace[0].m_ptindex,i,pFishTrace->m_fishtrace[0].m_fishid,pFishTrace->m_fishtrace[0].m_fudaifishtype,pFishTrace->m_fishtrace[i].m_speed);
			}
			break;
		}
	case ASS_S_BUY_BULLET:		//������·ֽ��
		{
			CDealUserBuyBullet(buffer, nLen);
			break;
		}
	case ASS_S_USER_SHOT:		//��ҷ����ӵ���Ϣ
		{
			CDealUserShot(buffer, nLen);
			break;
		}
	case ASS_S_USER_SHOT_ERROR:	//�ӵ��������
		{
			CDealUserShotError(buffer, nLen);
			break;
		}
	case ASS_S_ADD_FIRE:		//��ҼӼ��ڻ���
		{
			CDealUserAddFire(buffer, nLen);
			break;
		}
	case ASS_S_HIT_FISH:		//�������
		{
			CDealUserHitFish(buffer, nLen);
			break;
		}
	case ASS_S_CANCEL_SUPER:	//ȡ�������ڵ�
		{
			CDealCanCelSuperPao(buffer, nLen);
			break;
		}
	case ASS_S_CATCH_FISH_BROAD:	//�������㲥
		{
			CDealCatchBroadCast(buffer, nLen);
			break;
		}
	
	}
	return TRUE;
}
/*------------------------------------------------------------------------------*/
///������Ϸ��ͬ״̬ʱ���������ݣ���Ҫ���ڶ�������
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	if(nLen != sizeof(GameStation_Base))
	{
		return ;
	}

	GameStation_Base *pGameStation = (GameStation_Base *) pBuffer;
	if(NULL == pGameStation)
	{
		return;
	}
	//������Ϸ����
	memcpy(&m_GameData, pGameStation ,sizeof(m_GameData));

	//�Լ���λ��
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation();

	//�ڻ�λ
	m_GameScreen.SetCellScore(m_GameData.iFireBase);
	//��ҽ���
	m_GameScreen.UserComeInorLeave(SwitchChairID(m_iMyDeskStation),true);
	//������Ϣ
	CString sNickName = "";
	UserInfoForGame sUser;
	if(m_pGameFrame->GetUserInfo(m_iMyDeskStation,sUser))
	{
		sNickName.Format("%s",sUser.szNickName);
	}
	else
	{
		sNickName.Format("��ȡ�����Ϣ����");
		sUser.i64Money = 0;
	}
	m_GameScreen.SetMeInformation(SwitchChairID(m_iMyDeskStation),m_iMyDeskStation,sNickName,sUser.i64Money);
	//�Լ����·�
	m_GameScreen.SetUserScore(SwitchChairID(m_iMyDeskStation),m_GameData.iUserScore[m_iMyDeskStation]);
	//�Լ��ڻ���
	m_GameScreen.SetBeiLv(SwitchChairID(m_iMyDeskStation),m_GameData.iUserFireBase[m_iMyDeskStation]);


	//�����ڻ���
	for(int i=0;i<PLAY_COUNT;i++)
	{   
		if(i == m_iMyDeskStation)
		{
			continue;
		}
		memset(&sUser,0,sizeof(sUser));
		if(m_pGameFrame->GetUserInfo(i,sUser))
		{
			//���ø�λ������
			m_GameScreen.UserComeInorLeave(SwitchChairID(i),true);
			//����������·�
			m_GameScreen.SetUserScore(SwitchChairID(i),m_GameData.iUserScore[i]);
			//��ұ���
			m_GameScreen.SetBeiLv(SwitchChairID(i),m_GameData.iUserFireBase[i]);
		}
	}

	//�����Ƿ���Ϊ�����˻�������߼��жϿͻ���
	m_GameScreen.SetRobotLogic(m_GameData.bHitFlag, m_GameData.bHitDesk);


	//��ʶ�յ�������Ϸ״̬��Ϣ
	m_GameScreen.m_bHaveLinked = true;
}
/*------------------------------------------------------------------------------*/
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	return 0;
}
/// 
int CNewGameClient::ResetGameFrame(void)
{
	
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
/// ��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{

	switch(nTimerID)
	{
	
	case ID_TIME_CLOCK:
		{
			

			break;
		}
	case ID_PLAY_MOVE_VSCARD:
		{
			m_pGameFrame->KillTimer(ID_PLAY_MOVE_VSCARD);
	
		}
		break;
	case ID_TIME_LOOKCARD:
		{
	
		}
		break;
	}
}

/// ������Ϸ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_bGameStation = bGameStation;
	if (m_pGameFrame)
		m_pGameFrame->SetStationParameter(bGameStation);
}














//-----------------------------------��ʾ��ز���--------------------------------
/*-------------------------------------------------------------------*/
//������·�
void	CNewGameClient::AddOrRemoveScore(bool bAdd,int iScore)
{
	CMD_C_BuyBullet  TBuyBullet;

	TBuyBullet.bAddOrMove = bAdd;
	TBuyBullet.iScore = iScore;
	m_pGameFrame->SendGameData(&TBuyBullet,sizeof(TBuyBullet),MDM_GM_GAME_NOTIFY,ASS_C_BUY_BULLET,0);
}
/*-------------------------------------------------------------------*/
//������·ֽ��
void	CNewGameClient::CDealUserBuyBullet(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(CMD_S_BulletCount))
	{
		return;
	}

	CMD_S_BulletCount *pBulletCount = (CMD_S_BulletCount *)buffer;
	if(NULL == pBulletCount)
	{
		return;
	}
	//����������·�
	m_GameScreen.SetUserScore(SwitchChairID(pBulletCount->byDeskStation),pBulletCount->i64UserScore);
  
	//������Լ����·� ��Ҫˢ���Լ��Ľ����
	if(pBulletCount->byDeskStation == m_iMyDeskStation)
	{
		m_GameScreen.SetBaseScore(pBulletCount->i64UserMoney);
	}
}
/*-------------------------------------------------------------------*/
//��ҷ����ӵ�
void	CNewGameClient::OnUserShot(bool bIsSuper,float fRoalation)
{
	CMD_C_UserShoot TUserShoot;
	TUserShoot.fAngle=fRoalation;
	m_pGameFrame->SendGameData(&TUserShoot,sizeof(TUserShoot),MDM_GM_GAME_NOTIFY,ASS_C_USER_SHOT,0);
}
/*-------------------------------------------------------------------*/
//��ҷ����ӵ���Ϣ
void	CNewGameClient::CDealUserShot(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(CMD_S_UserShoot))
	{
		return;
	}

	CMD_S_UserShoot *pUserShoot = (CMD_S_UserShoot *)buffer;
	if(NULL == pUserShoot)
	{
		return;
	}

	//�����ڵ�
	if(pUserShoot->byStation != m_iMyDeskStation)
	{
		m_GameScreen.UserShoot(SwitchChairID(pUserShoot->byStation),pUserShoot->fAngle,pUserShoot->byStation,pUserShoot->bIsRobot);
	}
	//���÷�����ҵķ���
	m_GameScreen.SetUserScore(SwitchChairID(pUserShoot->byStation),pUserShoot->i64UserScore);
}
/*-------------------------------------------------------------------*/
//��ҷ����ӵ�����
void	CNewGameClient::CDealUserShotError(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(CMD_S_UserShootError))
	{
		return;
	}

	CMD_S_UserShootError *pUserShootError = (CMD_S_UserShootError *)buffer;
	if(NULL == pUserShootError)
	{
		return;
	}

	//�����ڵ�
	if(pUserShootError->byStation == m_iMyDeskStation)
	{
		//���÷�����ҵķ���
		m_GameScreen.SetUserScore(SwitchChairID(pUserShootError->byStation),pUserShootError->i64UserScore);
		m_GameScreen.SetBeiLv(SwitchChairID(pUserShootError->byStation),pUserShootError->iFireBase);
	}
	
}
/*-------------------------------------------------------------------*/
//��ҼӼ�����
void	CNewGameClient::OnUserAddFire(bool bAdd)
{
	CMD_C_AddFireBase TAddFire;
	TAddFire.bAdd = bAdd;
	TAddFire.byDeskStation = m_iMyDeskStation;

	m_pGameFrame->SendGameData(&TAddFire,sizeof(TAddFire),MDM_GM_GAME_NOTIFY,ASS_C_ADD_FIRE,0);
}
/*-------------------------------------------------------------------*/
//��ҼӼ��ڻ�
void	CNewGameClient::CDealUserAddFire(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(CMD_S_AddFireBase))
	{
		return;
	}

	CMD_S_AddFireBase *pAddFire = (CMD_S_AddFireBase *)buffer;
	if(NULL == pAddFire)
	{
		return;
	}

	m_GameScreen.SetBeiLv(SwitchChairID(pAddFire->byDeskStation),pAddFire->iUserFireBase);
}
/*-------------------------------------------------------------------*/
//�������
void	CNewGameClient::OnUserHitFish(int iFishID,int iBulletID,int iShootUSER,bool bIsAndroid)
{
	CMD_C_HitFish THitFish;
	THitFish.iBulletID	= iBulletID;
	THitFish.iFishID	= iFishID;
	THitFish.bIsAndroid = bIsAndroid;
	THitFish.iBulletUserId = iShootUSER;

	m_pGameFrame->SendGameData(&THitFish,sizeof(THitFish),MDM_GM_GAME_NOTIFY,ASS_C_HIT_FISH,0);

}
/*-------------------------------------------------------------------*/
//��һ��������
void	CNewGameClient::CDealUserHitFish(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(CMD_S_CaptureFish))
	{
		return;
	}

	CMD_S_CaptureFish *pHitFish = (CMD_S_CaptureFish *)buffer;
	if(NULL == pHitFish)
	{
		return;
	}


	//���ó����ڵ�
	if(pHitFish->bCanSuperPao)
	{
		m_GameScreen.SetSuperPao(SwitchChairID(pHitFish->byStation),pHitFish->iFishID);
	}
	else if(pHitFish->bCanSuperPao==false && pHitFish->lFishScore==0)
	{
		m_GameScreen.CancelSuperPao(SwitchChairID(pHitFish->byStation));
	}

	if(pHitFish->iFishKindScore ==-1)
	{
		m_GameScreen.UserAddMoney(SwitchChairID(pHitFish->byStation),pHitFish->iFishID,pHitFish->lFishScore,1000,true);
		m_GameScreen.SetBomAction(SwitchChairID(pHitFish->byStation),pHitFish->iFishID,pHitFish->lFishScore);
	}
	else
	{
		if(pHitFish->lFishScore==0)
		{
			//m_GameClientView.m_Cocos2dXWin.runclass->AddMoney(SwitchChairID(phitFish->wChairID),phitFish->dwFishID,phitFish->lFishScore,phitFish->FishKindscore,true);
			m_GameScreen.UserAddMoney(SwitchChairID(pHitFish->byStation),pHitFish->iFishID,pHitFish->lFishScore,pHitFish->iFishKindScore,true);
			//����
		}
		else if(pHitFish->lFishScore>0)
		{
			m_GameScreen.UserAddMoney(SwitchChairID(pHitFish->byStation),pHitFish->iFishID,pHitFish->lFishScore,pHitFish->iFishKindScore,true);
			//����
			m_GameScreen.SetUserScore(SwitchChairID(pHitFish->byStation),pHitFish->i64UserScore);

			//�Ƿ�һ����
			if(pHitFish->iCatchCount > 0)
			{
				for(int i=0; i<pHitFish->iCatchCount; i++)
				{
					m_GameScreen.UserAddMoney(SwitchChairID(pHitFish->byStation),pHitFish->iCatchFishID[i],pHitFish->iCatchScore[i],pHitFish->iFishKindScore,false,false);
				}
			}
		}
	}

}

/*-------------------------------------------------------------------*/
//ȡ�������ڵ�
void	CNewGameClient::CDealCanCelSuperPao(BYTE * buffer,int nLen)
{
	if(nLen != sizeof(CMD_S_CanCelSuperPao))
	{
		return;
	}

	CMD_S_CanCelSuperPao *pCanCelSuper = (CMD_S_CanCelSuperPao *)buffer;
	if(NULL == pCanCelSuper)
	{
		return;
	}

	//���ó����ڵ�
	if(!pCanCelSuper->bCanSuperPao)
	{
		m_GameScreen.CancelSuperPao(SwitchChairID(pCanCelSuper->byStation));
	}
}
/*-------------------------------------------------------------------*/
//�������㲥
void	CNewGameClient::CDealCatchBroadCast(BYTE * buffer,int nLen)
{
	if(!m_GameScreen.m_configDlg.m_bTipMsg)
	{
		return;
	}
	if(nLen != sizeof(CMD_S_CaptureFishBroadcast))
	{
		return;
	}

	CMD_S_CaptureFishBroadcast *pCaptureBroadcast = (CMD_S_CaptureFishBroadcast *)buffer;
	if(NULL == pCaptureBroadcast)
	{
		return;
	}
	
	m_GameScreen.CatchBroadCast(pCaptureBroadcast->iDeskNo,pCaptureBroadcast->szNickName,pCaptureBroadcast->szRoomName,pCaptureBroadcast->iFishType,pCaptureBroadcast->i64FishScore);


}













/*-------------------------------------------------------------------*/
//ת����λ
int		CNewGameClient::SwitchChairID(int m_Currchairid)
{
	int changChairID = m_Currchairid;
	switch(m_pGameFrame->GetMyDeskStation())
	{
	case 0:
	case 1:
	case 2:
		{
			switch(m_Currchairid)
			{
			case 0:return 0;
			case 1:return 1;
			case 2:return 2;
			case 3:return 3;
			case 4:return 4;
			case 5:return 5;
			}

		}
	case 3:
	case 4:
	case 5:
		{
			switch(m_Currchairid)
			{
			case 0:return 3;
			case 1:return 4;
			case 2:return 5;
			case 3:return 0;
			case 4:return 1;
			case 5:return 2;
			}

		}
	}
	return changChairID;
}
/*-------------------------------------------------------------------*/
void	CNewGameClient::OnMoveWindow(CPoint point)
{
	CWnd::FromHandle(m_pUI->GetWindowsHwnd())->PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
}
/*-------------------------------------------------------------------*/
//��С������
void	CNewGameClient::OnMinWondow()
{
	CWnd::FromHandle(m_pUI->GetWindowsHwnd())->ShowWindow(SW_MINIMIZE);
}
/*-------------------------------------------------------------------*/
//�˳��ر���Ϸ
void	CNewGameClient::OnCloseWindow()
{
	m_pGameFrame->CloseClient();
}
/*-------------------------------------------------------------------*/
//��ȡ�����ļ�
void	CNewGameClient::LoadGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	
	//������Ч
	m_GameScreen.m_configDlg.m_bBgSound	= ("1" == f.GetKeyVal(key,"BgSound","1"));
	//��Ϸ��Ч
	m_GameScreen.m_configDlg.m_bGameSound	= ("1" == f.GetKeyVal(key,"GameSound","1"));
	//����
	m_GameScreen.m_configDlg.m_bShakeWindow = ("1" == f.GetKeyVal(key,"ShakeWindow","0"));
	//������������Ź㲥��ʾ
	m_GameScreen.m_configDlg.m_bTipMsg = ("1" == f.GetKeyVal(key,"TipMsg","0"));
	
}
/*-------------------------------------------------------------------*/
//������Ϸ����
void	CNewGameClient::SaveGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	


	//������Ч
	if (m_GameScreen.m_configDlg.m_bBgSound)
	{
		//����û�в��� ��ô�����Ҫ��ʼ���ű���������
		f.SetKeyValString(key,"BgSound","1");
		if(!m_GameScreen.IsPlaying(m_GameScreen.m_Bgindex))
		{
			m_GameScreen.PlayBgSound(m_GameScreen.m_Bgindex);
		}
	}
	else
	{
		//������ڲ��� ��ô��Ҫֹͣ��
		f.SetKeyValString(key,"BgSound","0");
		m_GameScreen.StopBgSound();
	}

	//��Ϸ��Ч
	if (m_GameScreen.m_configDlg.m_bGameSound)
	{
		f.SetKeyValString(key,"GameSound","1");
	}
	else
	{
		//������ڲ�����Ч ��ʱ��Ҫֹͣ
		f.SetKeyValString(key,"GameSound","0");
	}

}