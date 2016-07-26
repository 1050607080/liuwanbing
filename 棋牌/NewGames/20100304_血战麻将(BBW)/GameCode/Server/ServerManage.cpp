#include "StdAfx.h"
#include "ServerManage.h"
#include <shlobj.h>

/*
* ��ӡ��־�ļ�
*/
void DebugPrintf(int iRoomID, const char *p, ...)
{
	return ;
	CTime time = CTime::GetCurrentTime();
	char szFilename[256];
	sprintf(szFilename, "%d_%d-%d%d%d-log.txt", NAME_ID, iRoomID, time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen(szFilename, "a");
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start(arg, p);
	vfprintf(fp, p, arg);
	fprintf(fp,"\n");
	fclose(fp);
}
/**
* ���캯��
*/
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{	
	m_uSocketID = 0;
	//
	m_iEnableWatch=0;
	m_iLeaveArgee=0;
	m_iLessCount=15;					//�ദ���� ���ƶ�ʱ�� ����ʱ�䣻
	m_iBeenPlayGame=0;
	m_bGameStation=GS_WAIT_ARGEE;
	m_byAgreeLeaveNum = 0;//ͬ�������Ϸ���������
	//m_SrvLogic.SetDeskPoint(this);//�¼�����(��ȡ����ָ��)

    sCPGAction.SetDeskPoint(this);//�����ܼ�ⷽ��(��ȡ����ָ��)
    sCheckHuPai.SetDeskPoint(this);//���Ƽ�ⷽ��(��ȡ����ָ��)
    sCountFen.SetDeskPoint(this);//��Ϸ���(��ȡ����ָ��)
    sThingHandle.SetDeskPoint(this);//�¼�����(��ȡ����ָ��)
}

/**
* ��������
*/
CServerGameDesk::~CServerGameDesk(void)
{
}

/**
* ��ʱ����Ϣ
* @param [in] uTimerID ��ʱ��ID
* @return ���ز�������
*/
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	//m_SrvLogic.OnGameTimer(uTimerID);
	if (m_bGameStation>=20 && m_bGameStation<23)
	{
		sThingHandle.OnGameTimer(uTimerID);
	}
	return __super::OnTimer(uTimerID);
}

/**
* ��ȡ��Ϸ״̬��Ϣ
* @param [in] bDeskStation ���λ��
* @param [in] uSoketID Socket ID
* @param [in] bWatchUser �Թ�
* @return ���ز�������
*/
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	m_uSocketID = uSocketID;//�����������ʱ��socket
	//m_SrvLogic.GetGameStaton(bDeskStation, bWatchUser,m_bGameStation);
	//����id
	m_RoomId = m_pDataManage->m_InitData.uRoomID;
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile pz(s + SKIN_FOLDER  + _T("_s.ini"));
	char key[500];
	sprintf(key,"game");
	sGameData.m_bShowTax	= pz.GetKeyVal(key,TEXT("bShowTax"),1);		//�Ƿ���ʾ̨�ѣ�Ĭ���У�
	sprintf(key,"%d_%d",NAME_ID,m_RoomId);
	sGameData.m_bShowTax	= pz.GetKeyVal(key,TEXT("bShowTax"),sGameData.m_bShowTax);		//�Ƿ���ʾ̨�ѣ�Ĭ���У�

	sGameData.m_bChengBao = pz.GetKeyVal(key,TEXT("bChengBao"),sGameData.m_bChengBao);		//�ֳа��ƺͼҼ���

	sThingHandle.GetGameStaton(bDeskStation, uSocketID, bWatchUser, m_bGameStation);	


	return true;
}

/**
* �����Ϸ���ݰ�������
* @param [in] bDeskStation ���λ��
* @param [in] pNetHead ��Ϣͷ
* @param [in] pData ��Ϣ��
* @param [in] uSize ��Ϣ���Ĵ�С
* @param [in] uSoketID Socket ID
* @param [in] bWatchUser �Թ�
* @return ���ز�������
*/
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{
			OutputDebugString("���Ʋ��ԣ�ASS_GM_FORCE_QUIT 00 ");
			//UserFourceQuit(bDeskStation);//ǿ�˴���������ʵ���йܵ�
			char cKey[10];
			CString sPath=CBcfFile::GetAppPath();
			int iResult = 1;
			if (CBcfFile::IsFileExist(sPath + "SpecialRule.bcf"))
			{
				CBcfFile fsr( sPath + "SpecialRule.bcf");
				sprintf(cKey, "%d", NAME_ID);
				iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 1);
			}
			OutputDebugString("���Ʋ��ԣ�ASS_GM_FORCE_QUIT 00 ");
			if (iResult == 0)//ǿ�˽���
			{
				GameFinish(bDeskStation,GFF_FORCE_FINISH);//ǿ�˽�����Ϸ
				return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser); 
			}
			return true;
		}
	}
	//�����ǿ�ˣ�������ǿ�˽����汾
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

/**
* ��Ϸ���ݰ�������
* @param [in] bDeskStation ���λ��
* @param [in] pNetHead ��Ϣͷ
* @param [in] pData ��Ϣ��
* @param [in] uSize ��Ϣ���Ĵ�С
* @param [in] uSoketID Socket ID
* @param [in] bWatchUser �Թ�
* @return ���ز�������
*/
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	KillTimer(TIME_CHECK_GAME_MSG);
	SetTimer(TIME_CHECK_GAME_MSG,TIME_DEFAULT_HANDLE);//�������ó�ʱ��ʱ��

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if (bWatchUser)
			{
				return FALSE;
			}
			if(NULL != m_pUserInfo[bDeskStation])
			{
				m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;  //�����Ϊͬ��״̬
			}
			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
	}
	if(pNetHead->bMainID == MDM_GM_GAME_NOTIFY)//������Ϸ��Ϣ
	{
		sThingHandle.HandleNotifyMessage(bDeskStation,pNetHead->bAssistantID,(BYTE*)pData,uSize,bWatchUser);
	}
	if(pNetHead->bMainID == 180)
		return true;
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

/**
* ������Ϸ״̬
* @param [in] bLastStation ��Ϸ������־
* @return ���ز�������
*/
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	return TRUE;
}

/**
* ��ʼ����Ϸ
* @return ���ز�������
*/
bool CServerGameDesk::InitDeskGameStation()
{
	return true;
}

/**
* ��Ϸ��ʼ
* @param [in] bBeginFlasg ��ʼģʽ��־
*/
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag) == false) 
	{
		GameFinish(0,GF_SALE);
		return false;
	}

	//����״̬
	m_byAgreeLeaveNum = 0;//ͬ�������Ϸ���������
	m_iBeenPlayGame++;
	m_bGameStation = GS_SEND_CARD;
    sGameData.ApplyThingID = THING_BEGIN;
    //�����¼�dwj1-1
    sThingHandle.ApplyAction(THING_BEGIN,200);
	return true;
}

/**
* ��Ϸ����
* @param [in] bDeskStation ���λ��
* @param [in] bCloseFlag ��Ϸ������־
* @return ���ز�������
*/
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			m_bGameStation = GS_WAIT_ARGEE;			
    		//����ʤ������
			for (int i = 0;i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
				{
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
				}
			}

			__int64   i_ChangePoint[PLAY_COUNT];     //����
			__int64   i_ChangeMoney[PLAY_COUNT];     //���
			__int64   i_ReturnMoney[PLAY_COUNT];     //���ؽ��			

			memset(i_ChangePoint, 0, sizeof(i_ChangePoint));
			memset(i_ChangeMoney, 0, sizeof(i_ChangeMoney));
			memset(i_ReturnMoney, 0, sizeof(i_ReturnMoney));

			// ������д�����ݿ�
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut, 0, sizeof(temp_cut));
			/////2010-9-27 �޸�̨�ѿ۷����⣬û��ע���˵�ׯʱ�����˶�����ע����̨��
			//CGameUserInfo	* pUserInfoTemp[MAX_PEOPLE];
			//memcpy(pUserInfoTemp,m_pUserInfo, sizeof(m_pUserInfo));
			//���Ի�����
			for(int i=0;i<4;i++)
			{
				sGameData.T_CountFen.m_iZhuanYi[i]	*= sGameData.m_iBaseFen;//����ת�Ʒ�
				sGameData.T_CountFen.m_iHuaJiao[i]	*= sGameData.m_iBaseFen;//���з�
				sGameData.T_CountFen.iGangFen[i]	*= sGameData.m_iBaseFen;//�ܷ�
				sGameData.T_CountFen.iHuFen[i]		*= sGameData.m_iBaseFen;//����
				sGameData.T_CountFen.iZongFen[i]	*= sGameData.m_iBaseFen;//�ܷ�
			}
			for(int i=0;i<4;++i)
			{
				i_ChangePoint[i] = sGameData.T_CountFen.iZongFen[i];
			}
			// ������Ӯ��˰
			ChangeUserPointint64(i_ChangePoint, temp_cut);
			__super::RecoderGameInfo(i_ChangeMoney);

			for(int i=0;i<4;++i)
			{
				sGameData.T_CountFen.iMoney[i] = i_ChangeMoney[i];//524288+640
			}

			sGameData.T_CountFen.bIsShowTax = sGameData.m_bShowTax;//�Ƿ���ʾ�����
			sGameData.T_CountFen.iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����
			//m_SrvLogic.OnKillAllTimer();//ɾ�����м�ʱ������Ϸ������
			KillTimer(TIME_WAIT_MSG);
			KillTimer(10);//
				
			//������������
			sUserData.CopyHandPai(sGameData.T_CountFen.m_byArHandPai,0,true);
			sGameData.T_CountFen.byCloseFlag = bCloseFlag;
			//��������
			for (int i=0; i<PLAY_COUNT; i++)     
			{

				SendGameData(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_HANDLE, 0);   
				SendWatchDataEx(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_HANDLE, 0); 
			}

			bCloseFlag = GF_NORMAL;
			ReSetGameState(bCloseFlag);
			OutputDebugString("�¼�����Ϸ��������");
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_SAFE_FINISH:
		{
			if(bDeskStation==255)
				bDeskStation=0;
			KillTimer(TIME_WAIT_MSG);
			KillTimer(10);//

			KillTimer(TIME_CHECK_GAME_MSG);

			int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����
			for (int i=0; i<m_bMaxPeople; i++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}				
			bCloseFlag	= GFF_FORCE_FINISH;

			//д�����ݿ�				
			__int64 i_ChangePoint[PLAY_COUNT];
			__int64 iWinMoney[8] ={0};
			bool temp_cut[PLAY_COUNT];
			memset(i_ChangePoint,0,sizeof(i_ChangePoint));
			memset(temp_cut,0,sizeof(temp_cut));
			//���Ի�����
			//for (int i=0;i<PLAY_COUNT;i++)
			//{
			//	temp_point[i] = sGameData.T_CountFen.iZongFen[i];
			//}
			ChangeUserPointint64(i_ChangePoint, temp_cut);
			__super::RecoderGameInfo(iWinMoney);
			for (int i=0;i<PLAY_COUNT;i++)
			{
				sGameData.T_CountFen.iMoney[i] = iWinMoney[i]; 
			}
			sGameData.T_CountFen.bIsShowTax = sGameData.m_bShowTax;//�Ƿ���ʾ�����
			
			///������������
			sUserData.CopyHandPai(sGameData.T_CountFen.m_byArHandPai,0,true);
			sGameData.T_CountFen.byCloseFlag = bCloseFlag;
			///��������
			for (int i=0; i<PLAY_COUNT; i++)     
			{
				SendGameData(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_UN_NORMAL, 0);   
				SendWatchDataEx(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_UN_NORMAL, 0);
			}
		}break;
	case GFF_FORCE_FINISH:
		{//uRunPublish
			// Ϊ��������û��뿪������ӵĴ���
			// �жϸ�����Ƿ�ѹ��ע���Ƿ�ׯ��
			if(bDeskStation==255 || m_bGameStation == GS_WAIT_ARGEE || m_bGameStation == GS_WAIT_NEXT)
			{
				return true;
			}
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}

			KillTimer(TIME_WAIT_MSG);
			KillTimer(10);//
			KillTimer(TIME_CHECK_GAME_MSG);
			bCloseFlag	= GFF_FORCE_FINISH;
			//int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����

			//д�����ݿ�				
			__int64 i_ChangePoint[PLAY_COUNT]={0};
			__int64 iWinMoney[8] ={0};
			bool temp_cut[PLAY_COUNT] = {0};
			//���Ի�����
			i_ChangePoint[bDeskStation] = -(__int64)m_pDataManage->m_InitData.uRunPublish;
			temp_cut[bDeskStation] = true;


			char buff[500];
			sprintf(buff,"�¼������Ʋ��� ������ GFF_FORCE_FINISH Ԥ�ƿ۷�%I64d ",i_ChangePoint[bDeskStation] );
			OutputDebugString(buff);

			for (int i=0;i<PLAY_COUNT;i++)
			{
				sprintf(buff,"�¼������Ʋ��� ������ GFF_FORCE_FINISH ���%d ��ʼ���%I64d ",i,iWinMoney[i] );
				OutputDebugString(buff);
			}

			ChangeUserPointint64(i_ChangePoint, temp_cut);
			__super::RecoderGameInfo(iWinMoney);
			for (int i=0;i<PLAY_COUNT;i++)
			{
				sGameData.T_CountFen.iMoney[i] = iWinMoney[i]; 

				sprintf(buff,"�¼������Ʋ��� ������ GFF_FORCE_FINISH ���%d �ý��%I64d ",i,iWinMoney[i] );
				OutputDebugString(buff);

			}
			sGameData.T_CountFen.bIsShowTax = sGameData.m_bShowTax;//�Ƿ���ʾ�����

			sprintf(buff,"�¼������Ʋ��� ������ GFF_FORCE_FINISH ʵ�ʿ۷�%I64d ",i_ChangePoint[bDeskStation] );
			OutputDebugString(buff);

			///������������
			sUserData.CopyHandPai(sGameData.T_CountFen.m_byArHandPai,0,true);
			sGameData.T_CountFen.byCloseFlag = bCloseFlag;

			///��������
			for (int i=0; i<PLAY_COUNT; i++)     
			{
				SendGameData(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_UN_NORMAL, 0);   
				SendWatchDataEx(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_UN_NORMAL, 0);
			}
			ReSetGameState(bCloseFlag);
			OutputDebugString("�¼�����Ϸǿ�ƽ���");
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	default:
		{
			break;
		}
	}
	// ��������
	ReSetGameState(bCloseFlag);
	return __super::GameFinish(bDeskStation,bCloseFlag);
}

/**
* �ж��Ƿ�������Ϸ
* @param [in] bDeskStation ���λ��
* @return ���ص�ǰ��Ϸ״̬
*/
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	return 	m_bGameStation >= 20 && m_bGameStation < 23;
}

/**
* �û��뿪��Ϸ��
* @return �����û��뿪״̬
*/
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	OutputDebugString("������ �¼���UserLeftDesk");
	sThingHandle.UserLeftDesk(bDeskStation);
	if((m_bGameStation >= 20 || m_bGameStation < 23) )//������Ϸ�������뿪
	{
		//return false;
	}
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//�û������뿪
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	OutputDebugString("�¼������߲��� UserNetCut 000");
	if (m_bGameStation>=20 && m_bGameStation<23 )  //����Ϸ�У��ж����йܾͽ����й�
	{
		sThingHandle.UserNetCut(bDeskStation);

		OutputDebugString("�¼������߲��� UserNetCut 111");
		sUserData.m_byFoceLeavCount = 0;
		for(int i=0;i<4;++i)
		{
			if(sUserData.m_bFoceLeave[i])//ǿ��
			{
				sUserData.m_byFoceLeavCount++;
			}
		}
        if (sUserData.m_byFoceLeavCount >= PLAY_COUNT && sUserData.m_byFoceLeavCount != 255) //�ļҶ�ǿ���뿪��
        {
            GameFinish(0,GF_NORMAL); //�û�ǿ���뿪
			OutputDebugString(TEXT("��Ϸ���������߲��� UserNetCut --- "));
            return true;
        }
	}
	//��¼������ң�������ǰ��������ҷ�����Ϣ��
	return __super::UserNetCut(bDeskStation, pLostUserInfo);
}



/**
* �������
* @return �����û�����״̬
*/
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	BYTE result = __super::UserSitDesk(pUserSit, pUserInfo);;
	return result;
}

///������Ϸ��ʱ��
void CServerGameDesk::SetGameTimer(UINT iId,int iTime)
{
	SetTimer(iId,iTime);
}
//ɾ��ָ����ʱ��
void CServerGameDesk::KillGameTimer(UINT iId)
{
	KillTimer(iId);
}


///������Ϸ״̬
void CServerGameDesk::SendGameStationEx(BYTE nDeskStation, bool bWatchUser, void * pStationData, UINT uSize)
{
	// ��������
	SendGameStation(nDeskStation,m_uSocketID,bWatchUser,pStationData,uSize);
}
///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
void CServerGameDesk::SendGameDataEx(BYTE nDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendGameData(nDeskStation,MDM_GM_GAME_NOTIFY,bAssID,0) ; 
}
///�����Թ����� ��������Ϣ���Թ��ߣ�
void CServerGameDesk::SendWatchDataEx(BYTE nDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendWatchData(nDeskStation,MDM_GM_GAME_NOTIFY,bAssID,0) ; 
}
///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
void CServerGameDesk::SendGameDataEx(BYTE nDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendGameData(nDeskStation, pData, uSize, MDM_GM_GAME_NOTIFY, bAssID, 0);
}
///�����Թ����� ��������Ϣ���Թ��ߣ�
void CServerGameDesk::SendWatchDataEx(BYTE nDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendWatchData(nDeskStation, pData, uSize, MDM_GM_GAME_NOTIFY, bAssID, 0);
}




//��ȡ���ͬ��״̬
void CServerGameDesk::GetUserAgree(BYTE &agree)
{
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		if(m_pUserInfo[i] == NULL)
			continue;
        if (m_pUserInfo[i]->m_UserData.bUserState == USER_ARGEE)  // ͬ��
        {
            agree |= (1<<i);
        }
        else                            // δͬ��
        {
            agree &= (~(1<<i));
        }
    }
}
///��ȡ�������
UINT CServerGameDesk::GetRoomRule()
{
	return m_pDataManage->m_InitData.dwRoomRule;//�������
}
///��ȡ����id
UINT CServerGameDesk::GetRoomID()
{
	return m_pDataManage->m_InitData.uRoomID;//����id
}

//��ȡ���䱶��
int CServerGameDesk::GetRoomPoint()
{
	return GetTableBasePoint();
}
//��ȡ��ҵ��ǳ�

void CServerGameDesk::GetUserNickName(BYTE station,char name[])

{

    if (station<0 || station>=PLAY_COUNT|| m_pUserInfo[station]==NULL)  // 

		return;

	memcpy(name,m_pUserInfo[station]->m_UserData.nickName,sizeof(m_pUserInfo[station]->m_UserData.nickName));

}

