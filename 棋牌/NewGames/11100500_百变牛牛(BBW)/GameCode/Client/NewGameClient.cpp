#include "stdafx.h"
#include "NewGameClient.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "defines.h"
#include "afxmt.h"
#include <tchar.h>
#include <stdio.h>

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


	m_bNtApplyXZ = false;	//ׯ���Ƿ�������ׯ��
	m_iLuZiIndex = 0;

	::memset(m_UserInfo,NULL,sizeof(m_UserInfo));	/**<�����Ϣ*/

	m_SoundEngine = createIrrKlangDevice();		///��Чָ��
	m_soundEngineBg = createIrrKlangDevice() ;	//��������
	m_bGameStation = GS_WAIT_AGREE;           ///��Ϸ״̬

	m_byLgStation = 255;						///�Լ����߼�λ��
	m_iMaxZhu=0;								///�������µ�����ע(��ʼ��Ϊ0���ֵ�ôӷ�����������ֵΪ׼)

	m_iChooseNoteType=-1;


	m_bWatchMode = false;						///�Ƿ�Ϊ�Թ���


	m_bSuperShow = false;
	ZeroMemory(m_bSuperTag,sizeof(m_bSuperTag));	
	ZeroMemory(m_i64ChoumaValue,sizeof(m_i64ChoumaValue));	//��������ֵ��0
	memset(&m_idZhuangList,-1,sizeof(m_idZhuangList));
	

	memset(m_wBullCard,0x00,sizeof(m_wBullCard)); //ţ�� 3��
	memset(m_wResultCard,0x00,sizeof(m_wResultCard));		//�����Ҫ�Աȵ��� 2��


	m_iZhuangFen=0x00;				///����ׯ�ҵĵ÷�		
	m_iXianFen=0x00;				///�����мҵĵ÷�		
	m_iMeFen=0x00;					///������һ�����ע�ķ�
	m_i64MyChengji=0;				///��ҳɼ�
	m_iNTwin=0x00;					///ׯ�ҳɼ�
	m_iZhuangListInedx = 0;
	m_TongShaOrTongBeiFlag=-1;		//���Ŷ�����־λ

	m_bIsSuperUser = false;			//������ұ�־λ
	m_bShowRobortNote = true ; 
	m_bSuperSet = false;
	m_bCanSuper = false;
	m_bShowRect = false;
	
	

	//-------------------------------------------------------------------
	m_bNowGameState = GS_WAIT_AGREE;        //��ǰ��Ϸ״̬
	m_iNowNTstation = -1;					//��ǰׯ��λ��

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

	if(m_soundEngineBg)
	{
		m_soundEngineBg->drop();				//�ͷ���Ч�ļ�
		m_soundEngineBg = NULL;
	}
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

	m_bSound = f.GetKeyVal(key,"bSound",true);			/**<�Ƿ񲥷���Ч*/
	m_bSoundPlay = f.GetKeyVal(key,"SoundPlay",true);	//�Ƿ񲥷ű�������
	m_bWatch = f.GetKeyVal(key,"bWatch",true);		/**<�Ƿ�����Թ�*/

	CBcfFile fbcf(s + "Order.bcf"); 
	key = TEXT("VirtualMoney");



}

///����UI����Ԫ�ص�״̬
int CNewGameClient::InitUI()
{	
	LoadBcf();								//���������ļ�
	ResetUi();
	ShowWaitNtList(false);
	SetImageVisible(NT_FLAG,false);
	SetButtonVisible(CTN_11112000_BTN_12112205,false);
	//·��
	ShowLuZiInfo(false);
	//����
	ShowSuperDlag(false);
	return 0;	
}
//����UI����
void	CNewGameClient::ResetUi()
{
	//����ʾ�����
	ShowOrHideSettlementFrame(false);
	//��ճ���
	ClearAllDeskSHAndMoneyNumber();
	//��˸ͼƬ
	RectXiaZhuGuangFlash(false);
	//���ð�ť
	EnableXiaZhuButton(false);
	//����ͼƬ
	for(int i=0; i<5; i++)
	{
		SetImageVisible(SHAPE_TOP+i,false);
	}
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
	return true;
}

//=============================================================================================
//������Ϸ״̬
//=============================================================================================
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	OutputDebugString("dwjnn::SetGameStation(0)");
	CString dwjlog;
	dwjlog.Format("dwjnn::GetStationParameter()=%d",GetStationParameter());
	OutputDebugString(dwjlog);
	switch (GetStationParameter())			//�����Ϸ״̬
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:		//�ȴ�ͬ��״̬
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
	case GS_FREE:
		{
			if (sizeof(TagGameStation_WaiteAgee) != nLen)
			{
				return;
			}
			SetGameStation_WaitNext(pBuffer);
			break;
		}
	case GS_BET:		//��ע״̬(��עʱ��)
		{
			if (sizeof(TagGameStation_Bet) != nLen)
			{
				return;
			}
			SetGameStation_Bet(pBuffer);
			break;
		}
	case GS_SEND_CARD:	//���ƿ���״̬
		{
			if (sizeof(TagGameStation_OpenCard) != nLen)
			{
				return;
			}
			SetGameStation_SendCard(pBuffer);
			break;
		}
	case GS_PLAY_GAME:		//��Ϸ������
		{
			if(sizeof(TagGameStation_ShowResult) == nLen)
			{
				return;
			}
			SetGameStation_ShowResult(pBuffer);
			break;
		}
	default:
		{
			break;
		}
	}
	//��ʾׯ��
	ShowNTInfo(true);
	//��ʾ�Լ���Ϣ
	ShowUserInfo(true);
	//��ʾ��ׯ�б�
	ShowWaitNtList(true,true);
	//��ʾ·����Ϣ
	ShowLuZiInfo(true,true);
	//��ʾ��ǰ��Ϣ����ǰ�����͵�ǰ������
	ShowCurrentInfo(true);	
	//��ʾһ��,��ׯ��Ҫ���ٽ��
	ShowShangZhuangNotice();
}
//====================================================================================
//���ֽ׶�
void	CNewGameClient::SetGameStation_WaitNext(void * pBuffer)
{
	TagGameStation_WaiteAgee *pGameStation = (TagGameStation_WaiteAgee *)pBuffer;
	if (NULL == pGameStation)
	{
		return;
	}
	SetStationParameter(GS_WAIT_AGREE);
	m_bGameStation	= pGameStation->byGameStaion;	//��Ϸ״̬

	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;	///��עʱ��
	m_iKaiPaiTime	= pGameStation->iKaiPaiTime;	///����ʱ��
	m_iFreeTime		= pGameStation->iFreeTime;		///����ʱ��	
	//m_iRemaindTime	= pGameStation->iRemaindTime;	///ʣ��ʱ��

	m_byMaxBase		= pGameStation->bMaxBase;		//�������
	m_iNowNTstation = pGameStation->iNowNtStation;	///��ǰׯ�ҵ�λ��
	m_iPlayCount	= pGameStation->iGameBeen;		///��ǰ����
	m_iNTju			= pGameStation->iZhuangBaShu;	///ׯ�ҽ����˼���	
	m_iGameCount	= pGameStation->iGameCount;		///��ǰ�Ѿ������˼���
	m_iNTwin		= pGameStation->i64NtWin;		//ׯ�ҵĳɼ�
	m_i64MaxNote	= pGameStation->i64MaxNote;		///������Ϸ�����ע��
	m_i64LimitMoney	= pGameStation->i64ShangZhuangLimit;///��ׯ��Ҫ�����ٽ��


	///������ע����
	memcpy(m_i64RectNoteMax,pGameStation->i64RectNoteMax,sizeof(m_i64RectNoteMax));
	// ���������ֵ
	memcpy(m_i64ChoumaValue,pGameStation->i64ChoumaValue,sizeof(m_i64ChoumaValue));

	//��ׯ�б�
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_idZhuangList[i][0]	= pGameStation->idzhuangstation[i][0];
		m_idZhuangList[i][1]	= pGameStation->idzhuangstation[i][1];
	}

	//·����Ϣ
	for(int i=0;i<GAME_SAVE_COUNT;i++)
	{
		for(int j=0;j<MAX_NOTE_COUNT;j++)
		{
			m_iResultInfo[i][j]	= pGameStation->iResultInfo[i][j];
		}
	}

	//��ʾ��ʾ��Ϣ
	SetTextInfoSz(STATICWORD,"����ʱ��",true);
	//��ʾ����ʱ
	CString dwjlog;
	dwjlog.Format("dwjnn::ʣ��ʱ��=%d",pGameStation->iRemaindTime);
	OutputDebugString(dwjlog);
	SetShowOrHideClock(pGameStation->iRemaindTime,0x00,true);				
}
//====================================================================================
//��ע�׶�
void	CNewGameClient::SetGameStation_Bet(void * pBuffer)
{
	OutputDebugString("dwjnn::SetGameStation_Bet----0");
	TagGameStation_Bet *pGameStation = (TagGameStation_Bet *)pBuffer;
	if (NULL == pGameStation)
	{
		return;
	}
	OutputDebugString("dwjnn::SetGameStation_Bet----1");
	SetStationParameter(GS_BET);
	m_bGameStation	= pGameStation->byGameStaion;	//��Ϸ״̬

	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;	///��עʱ��
	m_iKaiPaiTime	= pGameStation->iKaiPaiTime;	///����ʱ��
	m_iFreeTime		= pGameStation->iFreeTime;		///����ʱ��	
	//m_iRemaindTime	= pGameStation->iRemaindTime;	///ʣ��ʱ��

	m_byMaxBase		= pGameStation->bMaxBase;		//�������
	m_iNowNTstation = pGameStation->iNowNtStation;	///��ǰׯ�ҵ�λ��
	m_iPlayCount	= pGameStation->iGameBeen;		///��ǰ����
	m_iNTju			= pGameStation->iZhuangBaShu;	///ׯ�ҽ����˼���	
	m_iGameCount	= pGameStation->iGameCount;		///��ǰ�Ѿ������˼���
	m_iNTwin		= pGameStation->i64NtWin;		//ׯ�ҵĳɼ�
	m_i64MaxNote	= pGameStation->i64MaxNote;		///������Ϸ�����ע��
	m_i64LimitMoney	= pGameStation->i64ShangZhuangLimit;///��ׯ��Ҫ�����ٽ��

	///������ע����
	memcpy(m_i64RectNoteMax,pGameStation->i64RectNoteMax,sizeof(m_i64RectNoteMax));
	// ���������ֵ
	memcpy(m_i64ChoumaValue,pGameStation->i64ChoumaValue,sizeof(m_i64ChoumaValue));

	//��ׯ�б�
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_idZhuangList[i][0]	= pGameStation->idzhuangstation[i][0];
		m_idZhuangList[i][1]	= pGameStation->idzhuangstation[i][1];
	}

	//·����Ϣ
	for(int i=0;i<GAME_SAVE_COUNT;i++)
	{
		for(int j=0;j<MAX_NOTE_COUNT;j++)
		{
			m_iResultInfo[i][j]	= pGameStation->iResultInfo[i][j];
		}
	}

	//��ע��Ϣ
	m_iZhongZhu = pGameStation->iZhongZhu;			//���ѵ�ǰ��ע��
	for(int i=0;i<MAX_NOTE_COUNT;i++)
	{
		m_iMyZhu[i]		= pGameStation->iMyZhu[i];			//����������ע
		m_iQuYuZhu[i]	= pGameStation->iQuYuZhu[i];		///����ÿ�������µ�ע��
		m_iQuYuZhuTrue[i] = pGameStation->iQuYuZhuTrue[i];	///������ʵÿ�������µ�ע��

		//��ʾ�������
		SetShCtrlInfo(TIAN_SH+i,m_iQuYuZhu[i],true);
	}
	//��ʾ��ע��Ϣ
	ShowXiaZhuInfo(true);
	//��ť�Ƿ����
	CheckIsCanXiaZhu();
		
	//��ʾ��ʾ��Ϣ
	if (m_pGameFrame->GetMyDeskStation() == m_iNowNTstation)
	{
		SetTextInfoSz(STATICWORD,"��Ϸ��ʼ��",true);
	}
	else
	{
		SetTextInfoSz(STATICWORD,"��Ϸ��ʼ�ˣ�����Ѻע...",true);
	}
	CString dwjlog;
	dwjlog.Format("dwjnn::ʣ��ʱ��=%d",pGameStation->iRemaindTime);
	OutputDebugString(dwjlog);
	//��ʾ����ʱ
	SetShowOrHideClock(pGameStation->iRemaindTime,0x00,true);	

	if(pGameStation->iRemaindTime >= 4)
	{
		m_pGameFrame->SetTimer(ID_XIAZHU_TIME,1000);
		m_iXuaZhuTimeCount = pGameStation->iRemaindTime - 3;
	}

}
//====================================================================================
//���ƽ׶�
void	CNewGameClient::SetGameStation_SendCard(void * pBuffer)
{
	OutputDebugString("dwjnn::SetGameStation_SendCard----0");
	TagGameStation_OpenCard *pGameStation = (TagGameStation_OpenCard *)pBuffer;
	if (NULL == pGameStation)
	{
		return;
	}
	OutputDebugString("dwjnn::SetGameStation_SendCard----1");
	SetStationParameter(GS_BET);
	m_bGameStation	= pGameStation->byGameStaion;	//��Ϸ״̬

	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;	///��עʱ��
	m_iKaiPaiTime	= pGameStation->iKaiPaiTime;	///����ʱ��
	m_iFreeTime		= pGameStation->iFreeTime;		///����ʱ��	
	//m_iRemaindTime	= pGameStation->iRemaindTime;	///ʣ��ʱ��

	m_byMaxBase		= pGameStation->bMaxBase;		//�������
	m_iNowNTstation = pGameStation->iNowNtStation;	///��ǰׯ�ҵ�λ��
	m_iPlayCount	= pGameStation->iGameBeen;		///��ǰ����
	m_iNTju			= pGameStation->iZhuangBaShu;	///ׯ�ҽ����˼���	
	m_iGameCount	= pGameStation->iGameCount;		///��ǰ�Ѿ������˼���
	m_iNTwin		= pGameStation->i64NtWin;		//ׯ�ҵĳɼ�
	m_i64MaxNote	= pGameStation->i64MaxNote;		///������Ϸ�����ע��
	m_i64LimitMoney	= pGameStation->i64ShangZhuangLimit;///��ׯ��Ҫ�����ٽ��

	///������ע����
	memcpy(m_i64RectNoteMax,pGameStation->i64RectNoteMax,sizeof(m_i64RectNoteMax));
	// ���������ֵ
	memcpy(m_i64ChoumaValue,pGameStation->i64ChoumaValue,sizeof(m_i64ChoumaValue));

	//��ׯ�б�
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_idZhuangList[i][0]	= pGameStation->idzhuangstation[i][0];
		m_idZhuangList[i][1]	= pGameStation->idzhuangstation[i][1];
	}

	//·����Ϣ
	for(int i=0;i<GAME_SAVE_COUNT;i++)
	{
		for(int j=0;j<MAX_NOTE_COUNT;j++)
		{
			m_iResultInfo[i][j]	= pGameStation->iResultInfo[i][j];
		}
	}

	//��ע��Ϣ
	m_iZhongZhu = pGameStation->iZhongZhu;			//���ѵ�ǰ��ע��
	for(int i=0;i<MAX_NOTE_COUNT;i++)
	{
		m_iMyZhu[i]		= pGameStation->iMyZhu[i];			//����������ע
		m_iQuYuZhu[i]	= pGameStation->iQuYuZhu[i];		///����ÿ�������µ�ע��
		m_iQuYuZhuTrue[i] = pGameStation->iQuYuZhuTrue[i];	///������ʵÿ�������µ�ע��

		//��ʾ�������
		SetShCtrlInfo(TIAN_SH+i,m_iQuYuZhu[i],true);
	}
	//������
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<5; j++)
		{
			m_UserCard[i][j] = pGameStation->byCard[i][j];
		}
		m_CardCount[i] = pGameStation->byCardCount[i];
		//��ʾ����
		PutBullCard(i);   //��ʾ��ţ�����
		ShowUserCardShape(i , true ,false); //��ʾ����ͼƬ
	}
	//��ʾ��ע��Ϣ
	ShowXiaZhuInfo(true);

	//��ʾ��ʾ��Ϣ
	SetTextInfoSz(STATICWORD,"����ʱ��",true);
	//��ʾ����ʱ
	SetShowOrHideClock(pGameStation->iRemaindTime,0x00,true);	
}
//====================================================================================
//����׶�
void	CNewGameClient::SetGameStation_ShowResult(void * pBuffer)
{
	OutputDebugString("dwjnn::SetGameStation_ShowResult----0");
	TagGameStation_ShowResult *pGameStation = (TagGameStation_ShowResult *)pBuffer;
	if (NULL == pGameStation)
	{
		return;
	}
	OutputDebugString("dwjnn::SetGameStation_ShowResult----1");
	SetStationParameter(GS_BET);
	m_bGameStation	= pGameStation->byGameStaion;	//��Ϸ״̬

	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;	///��עʱ��
	m_iKaiPaiTime	= pGameStation->iKaiPaiTime;	///����ʱ��
	m_iFreeTime		= pGameStation->iFreeTime;		///����ʱ��	
	//m_iRemaindTime	= pGameStation->iRemaindTime;	///ʣ��ʱ��

	m_byMaxBase		= pGameStation->bMaxBase;		//�������
	m_iNowNTstation = pGameStation->iNowNtStation;	///��ǰׯ�ҵ�λ��
	m_iPlayCount	= pGameStation->iGameBeen;		///��ǰ����
	m_iNTju			= pGameStation->iZhuangBaShu;	///ׯ�ҽ����˼���	
	m_iGameCount	= pGameStation->iGameCount;		///��ǰ�Ѿ������˼���
	m_iNTwin		= pGameStation->i64NtWin;		//ׯ�ҵĳɼ�
	m_i64MaxNote	= pGameStation->i64MaxNote;		///������Ϸ�����ע��
	m_i64LimitMoney	= pGameStation->i64ShangZhuangLimit;///��ׯ��Ҫ�����ٽ��

	///������ע����
	memcpy(m_i64RectNoteMax,pGameStation->i64RectNoteMax,sizeof(m_i64RectNoteMax));
	// ���������ֵ
	memcpy(m_i64ChoumaValue,pGameStation->i64ChoumaValue,sizeof(m_i64ChoumaValue));

	//��ׯ�б�
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_idZhuangList[i][0]	= pGameStation->idzhuangstation[i][0];
		m_idZhuangList[i][1]	= pGameStation->idzhuangstation[i][1];
	}

	//·����Ϣ
	for(int i=0;i<GAME_SAVE_COUNT;i++)
	{
		for(int j=0;j<MAX_NOTE_COUNT;j++)
		{
			m_iResultInfo[i][j]	= pGameStation->iResultInfo[i][j];
		}
	}

	//��ע��Ϣ
	m_iZhongZhu = pGameStation->iZhongZhu;			//���ѵ�ǰ��ע��
	for(int i=0;i<MAX_NOTE_COUNT;i++)
	{
		m_iMyZhu[i]		= pGameStation->iMyZhu[i];			//����������ע
		m_iQuYuZhu[i]	= pGameStation->iQuYuZhu[i];		///����ÿ�������µ�ע��
		m_iQuYuZhuTrue[i] = pGameStation->iQuYuZhuTrue[i];	///������ʵÿ�������µ�ע��

		//��ʾ�������
		SetShCtrlInfo(TIAN_SH+i,m_iQuYuZhu[i],true);
	}
	//������
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<5; j++)
		{
			m_UserCard[i][j] = pGameStation->byCard[i][j];
		}
		m_CardCount[i] = pGameStation->byCardCount[i];
		//��ʾ����
		PutBullCard(i);   //��ʾ��ţ�����
		ShowUserCardShape(i , true ,false); //��ʾ����ͼƬ
	}

	//�мҵ÷�(��ע����)
	m_iUseFen	= pGameStation->iUserFen;
	m_iZhuangFen= pGameStation->iZhuangFen;

	//��ʾ��ע��Ϣ
	ShowXiaZhuInfo(true);
	//��ʾ�����
	ShowOrHideSettlementFrame(true);	

	//��ʾ��ʾ��Ϣ
	SetTextInfoSz(STATICWORD,"����ʱ��",true);
	//��ʾ����ʱ
	SetShowOrHideClock(pGameStation->iRemaindTime,0x00,true);
}
//====================================================================================
//��Ϸ��Ϣ������
//���մӷ���������������Ϣ
//====================================================================================
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	switch (nSubId)
	{
	case ASS_GM_GAME_STATION:		///<����״̬(ƽ̨���õ�)
		{
			SetGameStation(buffer, nLen);    
			break;        
		}
	case ASS_IS_SUPER_USER:						//�յ������ͻ�����Ϣ
		{
			if(sizeof(TagSuperUser) != nLen)
			{
				return  FALSE;
			}
			TagSuperUser *pSpuerData = (TagSuperUser *)buffer;
			if (NULL == pSpuerData)
			{
				return false;
			}
			m_bIsSuperUser = pSpuerData->bIsSuper;
			break;
		}
	case ASS_SUPER_SET:	//��������
		{
			if(sizeof(TagSuperSet) != nLen)
			{
				return  FALSE;
			}
			TagSuperSet *pSpuerSet = (TagSuperSet *)buffer;
			if (NULL == pSpuerSet)
			{
				return false;
			}
			memcpy(&m_TSuperSetResult,pSpuerSet,sizeof(m_TSuperSetResult));
			SetTextInfoSz(CTN_11112000_CTN_100800_TEXT_1008005,"���óɹ�",m_bIsSuperUser&&m_TSuperSetResult.bSuccesse);
			break;
		}
	case ASS_GAME_BEGIN:      //��Ϸ��ʼ������ע
		{
			OnHandleBet(buffer,nLen);
			break;
		}
	case ASS_KAI_PAI:					//������Ϣ
		{
			OnHandleKaiPai(buffer,nLen);
			break;
		}
	case ASS_RESULT:					//��Ϸ����
		{
			RecResultData(buffer,nLen);		//���ս�����Ϣ
			break;
		}
	case ASS_FREE_TIME:				//�ȴ���һ����Ϸ��ʼ
		{
			OnHandleFree(buffer,nLen);
			break;
		}
	case ASS_WU_ZHUANG://�յ���ׯ�ȴ���Ϣ
		{
			if (sizeof(DENG_DAI) > nLen)
			{
				return false;
			}
			DENG_DAI *pWait = (DENG_DAI *)buffer;
			if (NULL == pWait)
			{
				return false;
			}

			SetStationParameter(GS_WAIT_AGREE);//��Ϸ��Ϊ��ʼ״̬
			m_bNowGameState = GS_WAIT_AGREE;//��ǰ��Ϸ״̬
			m_iNowNTstation = -1;
			ResetGameData();

			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_idZhuangList[i][0] = pWait->dzhuangstation[i][0];
				m_idZhuangList[i][1] = pWait->dzhuangstation[i][1];
			}
			m_iNTju = pWait->iZhuangBaShu;//ׯ�ҵľ���
			m_iNTwin = pWait->iNtWin;//ׯ�ҵĳɼ�
			ShowNTInfo(true);
			ShowWaitNtList(true);
			SetTextInfoSz(ZHUANGNAME,"������ׯ",true);
			SetTextInfoSz(STATICWORD,"��ׯ�ȴ�",true);
			return true;
		}
	case ASS_XIA_ZHU:					//��ע�����Ϣ
		{
			OnHandleBetResult(buffer,nLen);
			break;
		}
	case ASS_SHANG_ZHUANG:      // ��ׯ��Ϣ
		{
			OnHandleShangZhuang(buffer,nLen);
			break;
		}
	default:
		{
			break;
		}
	}
	
	return 0;
}

//=======================================================================================

//==================================================================================
//������ʾ��ׯҪ���ٽ��
//==================================================================================
void	CNewGameClient::ShowShangZhuangNotice()
{
	CString sText;															
	sText.Format("%I64d",m_i64LimitMoney);
	CString strInfo ;
	strInfo.Format("����ʾ�����������ׯ����%s���!",sText) ;
	m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ;
}
//=====================================================================================
//������ʾһ������ؼ�
//=====================================================================================
//============================================================================
//��ׯ��Ϣ
//============================================================================
void	CNewGameClient::OnHandleShangZhuang(BYTE * buffer,int nLen)
{
	if (sizeof(SHANG_ZHUANG)!= nLen)
	{
		return ;
	}
	SHANG_ZHUANG *pSZ = (SHANG_ZHUANG *)buffer;
	if (NULL == pSZ)
	{
		return ;
	}
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_idZhuangList[i][0] = pSZ->dzhuangstation[i][0];
		m_idZhuangList[i][1] = pSZ->dzhuangstation[i][1];
	}
	//���ׯ����������ׯ
	if (m_pGameFrame->GetMyDeskStation() == pSZ->station && m_pGameFrame->GetMyDeskStation() == m_iNowNTstation && pSZ->shang == false)
	{
		m_bNtApplyXZ = pSZ->success;
	}
	//��ʾ��ׯ�б�
	ShowWaitNtList(true);
}
//===========================================================================================
//========================================================================================
//������Ϣ����
//========================================================================================
void CNewGameClient::OnHandleKaiPai(BYTE * buffer,int nLen)
{
	if(sizeof(KAI_PAI) != nLen)
	{
		return;
	}
	KAI_PAI *pKaiPaiData = (KAI_PAI*)buffer;
	if(NULL == pKaiPaiData)
	{
		return;
	}

	SetStationParameter(GS_PLAY_GAME);							//��Ϸ��Ϊ��ʼ״̬
	//ȡ���Ƶ�����
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			m_UserCard[i][j]=pKaiPaiData->pai[i][j];
		}
		m_CardCount[i]=pKaiPaiData->iCardCount[i];
	}
	
	//��ȡӮ������
	for(int i=0;i<MAX_NOTE_COUNT;i++)
	{
		m_iWinQuYu[i] = pKaiPaiData->iComputePoint[i];		
	}

	//��ʾ��ʾ��Ϣ
	SetTextInfoSz(STATICWORD,"����ʱ��",true);
	//������ע��ť��
	EnableXiaZhuButton(false);							

	m_iSendCardTime = 0;										/// ���ƶ�������ʱ�䣬���ƶ�����
	memset(m_CardSendCount,0,sizeof(m_CardSendCount));			//�Ƶ�������0
	m_pGameFrame->SetTimer(ID_SEND_CARD_TIMER,1000);			//�������ƶ�����ʱ��
	
	SetShowOrHideClock(m_iKaiPaiTime,0x00,true);				//������ʾʱ��

	if(m_soundEngineBg)
	{
		m_soundEngineBg->setAllSoundsPaused(true);	
		m_soundEngineBg->stopAllSounds();	
	}

	//���ó���ȷ����ť
	SetButtonEnable(CTN_11112000_CTN_100800_BTN_1008001,false);
}
//=======================================================================================
//���յ��ȴ���Ϣ
//=======================================================================================
void	CNewGameClient::OnHandleFree(BYTE * buffer,int nLen)
{
	if(sizeof(DENG_DAI)!=nLen)
	{
		return ;
	}
	DENG_DAI *pWait = (DENG_DAI *)buffer;
	if (NULL == pWait)
	{
		return;
	}
	SetStationParameter(GS_WAIT_NEXT);				//����Ϊ�ȴ�״̬

	for(int i=0;i<=PLAY_COUNT;i++)							//��ׯ�б���Ϣ
	{
		m_idZhuangList[i][0] = pWait->dzhuangstation[i][0];
		m_idZhuangList[i][1] = pWait->dzhuangstation[i][1];
	}

	m_iNowNTstation = pWait->iZhuang;
	m_iNTju = pWait->iZhuangBaShu;//ׯ�ҵľ���
	m_iNTwin = pWait->iNtWin;//ׯ�ҵĳɼ�
	m_iZhuangListInedx = 0;
			
	//��������
	ShowOrHideSettlementFrame(false);	
	//�������
	ClearAllDeskSHAndMoneyNumber();	
	//��ʾ�������
	ShowUserCardShape(0,false,true);
	//��ʾׯ��
	ShowNTInfo(true);
	//��ʾ��ׯ�б���Ϣ
	ShowWaitNtList(true);
	//������
	for(int i=0; i<5;i++)
	{
		SetOperateCardInfot(HAND_CARD_1+i,NULL,0,false);
		SetNoOperateCardInfo(UP_CARD_1+i,NULL,0,false);
	}
	SetTextInfoSz(STATICWORD,"����ʱ��",true);
	SetShowOrHideClock(m_iFreeTime,0,true);	//������ʾ��ʱ��

	//�������
	ResetUi();
	ResetGameData();
	
	ShowUserInfo(true);
}
//=======================================================================================
//������ע��Ϣ����
//=======================================================================================
void	CNewGameClient::OnHandleBetResult(BYTE * buffer,int nLen)
{
	if(sizeof(TagXiaZhu) != nLen)
	{
		return ;
	}							
	
	TagXiaZhu *pXiaZhuResult = (TagXiaZhu *)buffer;
	if (NULL == pXiaZhuResult)
	{
		return;
	}

	///���ѵ�ǰ��ע��  
	m_iZhongZhu = pXiaZhuResult->iZhongZhu;
	///���������ע	
	m_iMaxZhu	= pXiaZhuResult->iMaxZhu;
	for(int i = 0;i < MAX_NOTE_COUNT;i++)
	{
		if(pXiaZhuResult->byStation == m_pGameFrame->GetMyDeskStation())
		{
			m_iMyZhu[i]		= pXiaZhuResult->iUserXiaZhu[i];
			//�Լ�����ע
			m_iMyZhu[MAX_NOTE_COUNT]		= pXiaZhuResult->iUserXiaZhu[MAX_NOTE_COUNT];
		}
		m_iQuYuZhu[i]		= pXiaZhuResult->iQuYuZhu[i];					//�����������µ�ע
		m_iQuYuZhuTrue[i]	= pXiaZhuResult->iQuYuZhuTrue[i];				//���������µ�ע
	}

	//��ʾ��ע����
	SetShCtrlInfo(TIAN_SH+pXiaZhuResult->iBetArea,pXiaZhuResult->iMoney,true);
	//��ʾ��ע��Ϣ
	ShowXiaZhuInfo(true);
	//�������Ƿ���õ����
	CheckIsCanXiaZhu();

	if(pXiaZhuResult->iMoney <= 100000)
	{
		PlayMotionSoundOgg(m_SoundEngine,SOUND_NOTE,0x01);
	}
	else if(pXiaZhuResult->iMoney == 1000000)
	{
		PlayMotionSoundOgg(m_SoundEngine,SOUND_NOTE_100w,0x01);
	}
	else if(pXiaZhuResult->iMoney == 10000000)
	{
		PlayMotionSoundOgg(m_SoundEngine,SOUND_NOTE_1000w,0x01);
	}
}
//=====================================================================================
//��ʾ��ע��Ϣ
void		CNewGameClient::ShowXiaZhuInfo(bool bShow)
{
	for(int i = 0;i < MAX_NOTE_COUNT;i++)
	{
		//��������ע�ۺ�
		SetINumInfoNoSymbol(TIAN_ALLMONEY_NUM+i,0,false);
		SetINumInfoNoSymbol(TIAN_OWERMONEY_NUM+i,0,false);
	}

	for(int i = 0;i < MAX_NOTE_COUNT;i++)
	{

		//��������ע
		if (m_bShowRobortNote)
		{
			SetINumInfoNoSymbol(TIAN_ALLMONEY_NUM+i,m_iQuYuZhu[i],bShow&&m_iQuYuZhu[i]>0);
		}
		else
		{
			SetINumInfoNoSymbol(TIAN_ALLMONEY_NUM+i,m_iQuYuZhuTrue[i],bShow&&m_iQuYuZhuTrue[i]>0);
		}
		//�Լ�����ע��
		SetINumInfoNoSymbol(TIAN_OWERMONEY_NUM+i,m_iMyZhu[i],bShow&&m_iMyZhu[i]>0);
	}
}
//=====================================================================================
//���ս�������
//=====================================================================================
void CNewGameClient::RecResultData(BYTE * buffer,int nLen)		
{
	if(sizeof(tagResult) != nLen)
	{
		return ;
	}
	tagResult *pCmd = (tagResult *)buffer;
	if (NULL == pCmd)
	{
		return;
	}
	m_pGameFrame->KillTimer(ID_SHOW_WIN_AREAS);
	RectXiaZhuGuangFlash(false);					

	m_iZhuangFen	= pCmd->iZhuangFen;		//ׯ�ҵĵ÷�
	m_iXianFen		= pCmd->iXianFen;		//�мҵ��ܵ÷�
	m_iUseFen		= pCmd->iUserFen;		//��ҵĵ÷�
	m_i64MyChengji += pCmd->iUserFen;		//��ҵĳɼ�
	m_iNTwin		= pCmd->iNtWin;			//ׯ�ҵĳɼ�

	int iCount = 0;
	for(int i=0; i<GAME_SAVE_COUNT;i++)
	{
		for(int j=0; j<MAX_NOTE_COUNT; j++)
		{
			m_iResultInfo[i][j]		= pCmd->iResultInfo[i][j];
		}
	}

	//·���±��
	m_iLuZiIndex = 0;
	//��ʾ��ʾ��Ϣ
	SetTextInfoSz(STATICWORD,"����ʱ��",true);
	//��ʾׯ����Ϣ
	ShowNTInfo(true);			
	//��ʾ��ǰ��Ϣ����ǰ�����͵�ǰ������
	ShowCurrentInfo(true);
	//��ʾ�û���Ϣ
	ShowUserInfo(true);		
	//��ʾ�����
	ShowOrHideSettlementFrame(true);	
	//��ʾ·��
	ShowLuZiInfo(true,true);
	//������ʾʱ��
	SetShowOrHideClock(5,0x00,true);					
	
}
//========================================================================
//=======================================================================================
//���ڼ���Ƿ񻹿�����ע	
//=======================================================================================
void	CNewGameClient::CheckIsCanXiaZhu()					
{
	
	CString strInfo ;
	__int64 i64ChouMaMoney = 0;
	if(-1<m_iChooseNoteType && m_iChooseNoteType < 6)
	{
		i64ChouMaMoney = m_i64ChoumaValue[m_iChooseNoteType];		
	}
	else
	{
		i64ChouMaMoney = m_i64ChoumaValue[5];											//�����ǰû��ѡ�����,��Ĭ��ѡһ������
	}

	
	UserInfoForGame sMyInfo ;				//�����Ϣ
	BYTE myDesk = m_pGameFrame->GetMyDeskStation();
	if (!m_pGameFrame->GetUserInfo(myDesk,sMyInfo))
	{
		sMyInfo.i64Money = 0;
	}

	if (myDesk != m_iNowNTstation)
	{
		//���������Ƿ�������
		for(int i=0; i<6; i++)
		{
			if ( m_iMaxZhu < m_i64ChoumaValue[i] || sMyInfo.i64Money < m_i64ChoumaValue[i])
			{
				SetButtonEnable(HUNDRED_BUTTON+i,false);
			}
			else
			{
				SetButtonEnable(HUNDRED_BUTTON+i,true);
			}
		}
	}
	
	return;

	UserInfoForGame sNtInfo ;			//ׯ����Ϣ
	m_pGameFrame->GetUserInfo(m_iNowNTstation,sNtInfo);
	//����Լ����ܷ���ע
	__int64	i64MyZongZhu = m_iMyZhu[MAX_NOTE_COUNT];	
	//��ע
	__int64 iQuYuZhu = m_iZhongZhu;							
	//-----------------------------------------------------------------------------------------------------------------------
	//������ѵ�Ǯ����������
	if((i64MyZongZhu+i64ChouMaMoney)>sMyInfo.i64Money||											//��⵱ǰ���ѵ�Ǯ�����²�
		(m_i64MaxNote > 0 && ((m_i64MaxNote - i64MyZongZhu) < m_i64ChoumaValue[5])))
	{
		__int64 iExcessMoney = sMyInfo.i64Money - i64MyZongZhu;								//���ѻ��ж���Ǯ����ע
		__int64 tempmax = sMyInfo.i64Money;				
		if(m_i64MaxNote  > 0)
		{
			tempmax = m_i64MaxNote;							//���Ϊ�����ļ������õ�Ǯ(���ѿ���û����ô���Ǯ)
		}
		else
		{
			tempmax = sMyInfo.i64Money;						//����������ѵ�Ǯ
		}
		__int64 iExcessMaxMoney = (tempmax - i64MyZongZhu);		//��ʵ�����µ�ע(������Ǯ����û����ô��,�˷ѱ���)
		BOOL bIsExcessMaxMoney = FALSE;						//����Ϊ�����ע���
		if ((tempmax - i64MyZongZhu) < m_i64ChoumaValue[5])
		{
			if (iExcessMaxMoney < iExcessMoney && m_i64MaxNote > 0)
			{
				iExcessMoney = iExcessMaxMoney;
				bIsExcessMaxMoney = TRUE;
				if (iExcessMaxMoney < m_i64ChoumaValue[0])
				{
					strInfo.Format("����ʾ��������ע�Ѵﵽ���ޣ�Ŀǰ������ע!");
					m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
				}
			}
		}//end if
		//---------------------------------------------------
		if(iExcessMoney >= m_i64ChoumaValue[0])
		{
			int count = -1;
			do 
			{
				if(iExcessMoney<m_i64ChoumaValue[count+1])
					break;
				count++;
			} while (iExcessMoney >= m_i64ChoumaValue[0]);

			if (m_iChooseNoteType != -1 && m_iChooseNoteType > count)
			{
				m_iChooseNoteType = count;
				SetCurrentCursor(m_iChooseNoteType);
				if (bIsExcessMaxMoney)
				{
					strInfo.Format("����ʾ��������ע�������㣬ϵͳ�Զ�תΪ��һ����!");
					m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
				}
				else
				{
					strInfo.Format("����ʾ��������ע���㣬ϵͳ�Զ�תΪ��һ����!");
					m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
				}//end if else
			}//end if 
			RecXiaZhuDataAfterEnableXiaZhuButton(count);
		}
		else				//�ָ������Ĺ��
		{
			m_iChooseNoteType = -1;
			SetCurrentCursor(m_iChooseNoteType);	
			SetXiaZhuButtonEnable(false);
			strInfo.Format("����ʾ����ע�Ѵ�������ޣ��޷�������ע!");
			m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
		}//end if else
		
	}
	//-----------------------------------------------------------------------------------------------------------------------
	//��⵱ǰ��Ǯ��������
	else if((iQuYuZhu+i64ChouMaMoney + m_i64ChoumaValue[5]) > sNtInfo.i64Money)	
	{
		__int64 iExcessMoney = (sNtInfo.i64Money - (iQuYuZhu));		//ׯ�ҵ�Ǯ - ����ע��Ǯ = ��ҿ���ע��Ǯ
		if (iExcessMoney  >= m_i64ChoumaValue[0])
		{
			int count = -1;
			do 
			{
				if(iExcessMoney<m_i64ChoumaValue[count+1])
					break;
				count++;
			} while (iExcessMoney >= m_i64ChoumaValue[0]);
			if(m_iChooseNoteType != -1 && m_iChooseNoteType > count)
			{
				m_iChooseNoteType = count;
				SetCurrentCursor(m_iChooseNoteType);
				strInfo.Format("����ʾ��ׯ�����㣬ϵͳ�Զ�תΪ��һ����!");
				m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ;
			}
			RecXiaZhuDataAfterEnableXiaZhuButton(count);
		}
		else				//�ָ������Ĺ��
		{
			m_iChooseNoteType = -1;
			SetCurrentCursor(m_iChooseNoteType);	
			SetXiaZhuButtonEnable(false);
			strInfo.Format("����ʾ����ע�Ѵ�������ޣ��޷�������ע!");
			m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
		}
	}
}
//==============================================================================================
//������ʾ����
//==============================================================================================
void CNewGameClient::SetShowOrHidePai(int Desk,BYTE CardList[],int CardNum,bool flag)
{

	const int IOperateCarID[5]={HAND_CARD_1,HAND_CARD_2,HAND_CARD_3,HAND_CARD_4,HAND_CARD_5};
	const int InoMoveCarID[5]={UP_CARD_1,UP_CARD_2,UP_CARD_3,UP_CARD_4,UP_CARD_5};
	IOperateCard  *pOperateCard = NULL ; 
	INoMoveCard  *pNoMoveCard = NULL ;
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(IOperateCarID[Desk]));
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(InoMoveCarID[Desk]));

	if(CardNum<=2 && NULL!=pOperateCard)
	{
		pOperateCard->SetControlVisible(flag) ; 
		pOperateCard->SetCardList(CardList , CardNum) ;
	}
	else if(CardNum>2 && NULL!=pNoMoveCard)
	{
		pNoMoveCard->SetControlVisible(flag) ;     
		pNoMoveCard->SetCardList(&CardList[2] , CardNum-2) ;
	}
}
//
void CNewGameClient::ShowOrHideResultCard(int Desk,BYTE CardList[],int CardNum,bool flag,BYTE UpOrDownType)
{
	const int IOperateCarID[5]={HAND_CARD_1,HAND_CARD_2,HAND_CARD_3,HAND_CARD_4,HAND_CARD_5};
	const int InoMoveCarID[5]={UP_CARD_1,UP_CARD_2,UP_CARD_3,UP_CARD_4,UP_CARD_5};
	IOperateCard  *pOperateCard = NULL ; 
	INoMoveCard  *pNoMoveCard = NULL ;
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(IOperateCarID[Desk]));
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(InoMoveCarID[Desk]));
	if(NULL != pOperateCard && UpOrDownType == TYPE_DOWN)  //��ʾ������δ�������
	{
		pOperateCard->SetControlVisible(flag) ; 
		pOperateCard->SetCardList(CardList, CardNum) ;
	}
	if(NULL != pNoMoveCard && UpOrDownType == TYPE_UP) //��ʾ�������������
	{
		pNoMoveCard->SetControlVisible(flag) ; 
		pNoMoveCard->SetCardList(CardList, CardNum) ;
	}
}
//============================================================================
//���³�ʼ����Ϸ����
//return bool ��ʼ����Ϸ�����Ƿ�ɹ�
//============================================================================
bool CNewGameClient::InitGameData()
{
 	memset(&m_iQuYuZhu, 0, sizeof(m_iQuYuZhu));		//��������Լ���ÿ�������µ�ע��:���ţ����ţ�����
 	memset(&m_iMyZhu, 0, sizeof(m_iMyZhu));			//��������Լ���ÿ�������µ�ע��:���ţ����ţ�����
 	memset(&m_iWinQuYu, 0, sizeof(m_iWinQuYu));		//��־��Ӯ����
	return true;
}
//==========================================================================================
//��ʾ�����ؽ����
//==========================================================================================
void CNewGameClient::ShowOrHideSettlementFrame(bool bFlag)
{
	IContain *pIContain=NULL;
	pIContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(SETTLEMENTFRAME_PIC));
	if(NULL != pIContain)
	{
		pIContain->SetControlVisible(bFlag);
	}

	__int64 i64ReturnOwerMoney = 0x00;
	for(int i = 0x00; i < 0x04;i ++)									//���������صĽ��
	{
		if(m_iWinQuYu[i] > 0x00)
		{
			i64ReturnOwerMoney += m_iMyZhu[i];
		}
	}

	//�мҵ÷�(��ע����)
	SetINumSingle(SETTLEMENT_NUN_01,m_iUseFen,bFlag);	
	//�мҵ÷�(����ԭ��)
	SetINumSingle(SETTLEMENT_NUN_02,i64ReturnOwerMoney,bFlag);	
	//ׯ�ҵ÷�
	SetINumSingle(SETTLEMENT_NUN_03,m_iZhuangFen,bFlag);


	if(bFlag)
	{
		if(m_byLgStation == m_iNowNTstation)					//��ǰׯ��������
		{
			if(m_iZhuangFen < 0x00)
			{
				PlayMotionSoundOgg(m_SoundEngine,SOUND_LOSE,0x01);
			}
			else if(m_iZhuangFen > 0x00)
			{
				PlayMotionSoundOgg(m_SoundEngine,SOUND_WIN,0x01);
			}
			else
			{
				PlayMotionSoundOgg(m_SoundEngine,SOUND_HE,0x01);
			}
		}
		else
		{
			if(m_iUseFen < 0x00)
			{
				PlayMotionSoundOgg(m_SoundEngine,SOUND_LOSE,0x01);
			}
			else if(m_iUseFen > 0x00)
			{
				PlayMotionSoundOgg(m_SoundEngine,SOUND_WIN,0x01);
			}
			else
			{
				PlayMotionSoundOgg(m_SoundEngine,SOUND_HE,0x01);
			}
		}
	}
}
//=========================================================================================


//========================================================================================
///��Ϸ���水����Ϣ
//====================================================================================================
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	if(pMessage->_uMessageType == UI_LBUTTONDOWN && !m_bWatchMode)
	{
		switch(pMessage->_uControlID)
		{
		case CTN_11112000_CTN_100800_RButton_1008011:		///<ׯ��Ӯ  ��ѡ�� 
		case CTN_11112000_CTN_100800_RButton_1008012:		///<ׯ����  ��ѡ�� 
		case CTN_11112000_CTN_100800_RButton_1008013:		///<��Ӯ  ��ѡ�� 
		case CTN_11112000_CTN_100800_RButton_1008014:		///<����  ��ѡ�� 
			{
				if (GetOneRButton(pMessage->_uControlID));
				{
					SetOneRButton(pMessage->_uControlID+10,false);
				}
				break;
			}
		case CTN_11112000_CTN_100800_RButton_1008021:		///<ׯ��Ӯ  ��ѡ�� 
		case CTN_11112000_CTN_100800_RButton_1008022:		///<ׯ����  ��ѡ�� 
		case CTN_11112000_CTN_100800_RButton_1008023:		///<��Ӯ  ��ѡ�� 
		case CTN_11112000_CTN_100800_RButton_1008024:		///<����  ��ѡ�� 
			{
				if (GetOneRButton(pMessage->_uControlID));
				{
					SetOneRButton(pMessage->_uControlID-10,false);
				}
				break;
			}
		case CTN_11112000_CTN_100800_RButton_1008004:		//�Ƿ����λ�����
			{
				m_bShowRobortNote = !GetOneRButton(pMessage->_uControlID);
				ShowXiaZhuInfo(true);
				break;
			}
		case CTN_11112000_CTN_100800_BTN_1008001:			//����ȷ����ť
			{
				if (m_bIsSuperUser)
				{
					//���ͳ�������
					SendSuperSet();
				}
				break;
			}
		case CTN_11112000_CTN_100800_BTN_1008002:			//ȡ����ť
			{
				ShowSuperDlag(false);
				break;
			}
		default:
			{
				break;
			}
		}
	}
	if(pMessage->_uMessageType == UI_LBUTTONUP && !m_bWatchMode)
	{
		switch(pMessage->_uControlID)
		{
		case IDD_CHUOMA_QIANGZ:			//������ׯ
		case IDD_CHUOMA_XIAZ:			//��Ҫ��ׯ
			{
				ShangZhuangOrXiaZhuangButton(pMessage->_uControlID-IDD_CHUOMA_QIANGZ);
				break;
			}
		case HUNDRED_BUTTON:						//���밴ť
		case THOUSAND_BUTTON:		
		case TEN_THOUSAND_BUTTON:	
		case HUNDRED_THOUSAND_BUTTON:	 
		case MILLION_BUTTON:			
		case TEN_MILLION_BUTTON:
			{				
				SetCurrentCursor(pMessage->_uControlID-HUNDRED_BUTTON);	
				m_iChooseNoteType = pMessage->_uControlID-HUNDRED_BUTTON ; 
				break;				
			}
		case TIAN_XIAZHU_AREA_BNT:					///����ע����ť
		case DI_XIAZHU_AREA_BNT:					///����ע����ť
		case XUAN_XIAZHU_AREA_BNT:					///����ע����ť
		case HUANG_XIAZHU_AREA_BNT:					///����ע����ť
			{
				if(-1!=m_iChooseNoteType)
				{
					OutputDebugString("dwjnn::��ע111111");
					m_iChooseNoteRect = pMessage->_uControlID - TIAN_XIAZHU_AREA_BNT ; 
					OnXiaZhu(m_iChooseNoteRect) ; 
				}
				break;
			}
			
		case LEFTBUTTON:									//��·�Ӱ�ť
			{
				m_iLuZiIndex --;
				ShowLuZiInfo(true);
				break;
			}
		case RIGHTBUTTON:									//��·�Ӱ�ť
			{
				m_iLuZiIndex ++;
				ShowLuZiInfo(true);
				break;
			}
		case BANK_BUTTON:	//����(�Ѿ�����)
			{
				m_pGameFrame->OnVisitBank() ;
				break;
			}
		case UP_WAITING_LIST_BUTUUON:						//�Ϸ���ť
			{
				m_iZhuangListInedx--;
				ShowWaitNtList(true);
				break;
			}
		case DOWN_WAITING_LIST_BUTUUON:						//�·���ť
			{
				m_iZhuangListInedx++;
				ShowWaitNtList(true);
				break;
			}
		case BTN_SET_GAME_INFO:								//�������					
			{
				if(m_bIsSuperUser)
				{
					ShowSuperDlag(true); //����
				}
				else
				{
					SetGameInfordDlg(m_bSound,m_bSoundPlay,true);
				}									
				break;
			}
		case BTN_SET_GAME_OK:
			ButtonDownOK();
			break;
		case BTN_SET_GAME_CANCEL:
			ButtonCancel();
			break;
		default:
			 break;
		}
	}
	if(pMessage->_uMessageType == UI_RBUTTONDOWN && !m_bWatchMode)						//ȡ����ע
	{
		if(-1!=m_iChooseNoteType)
		{
			m_iChooseNoteType=-1;
			SetCurrentCursor(m_iChooseNoteType);
		}
	}

	return 0;
}
//========================================================================================
//����ׯ����ׯ��ť
//========================================================================================
void CNewGameClient::ShangZhuangOrXiaZhuangButton(const int MsgNum)
{
	SHANG_ZHUANG qiangzhuang;
	if(!CheckMeZhuang())		//��������Ƿ���ׯ�һ�����ׯ�б���
	{
		if(m_pGameFrame->GetUserInfo(m_byLgStation,m_UserInfo[m_byLgStation]))
		{
			if(m_UserInfo[m_byLgStation].i64Money>m_i64LimitMoney)
			{
				qiangzhuang.shang=true;
				m_pGameFrame->SendGameData(&qiangzhuang,sizeof(qiangzhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);	//����ׯ��Ϣ
			}
			else
			{
				CString sText;															
				sText.Format("%I64d",m_i64LimitMoney);
				CString strInfo ;
				strInfo.Format("����ʾ�����������ׯ����%s���!",sText) ;
				m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
			}
		}
	}
	else
	{
		qiangzhuang.shang=false;
		m_pGameFrame->SendGameData(&qiangzhuang,sizeof(qiangzhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);	//����ׯ��Ϣ
		if(m_byLgStation==m_iNowNTstation)
		{
			CString strInfo ;
			strInfo.Format("����ʾ������ǰ����ׯ�ϣ��¾ֿ�ʼǰ�Զ���ׯ!");
			m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
		}
	}
}
//========================================================================================
///����Լ��Ƿ���ׯ�һ���ׯ���б���
//========================================================================================
bool CNewGameClient::CheckMeZhuang(void)
{
	if(m_byLgStation == m_iNowNTstation)
		return true;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_byLgStation == m_idZhuangList[i][0])//����ׯ���б�
			return true; 
	}
	return false;
}
///====================================================================
///��ҵ����ע
///iChooseRect:��ע������
//=====================================================================
void	CNewGameClient::OnXiaZhu(int iChooseRect)
{
	// ������ע��Ϣ
	BYTE myDesk = m_pGameFrame->GetMyDeskStation();
	if(myDesk == m_iNowNTstation)
	{
		return ;									//ׯ�Ҳ�����ע
	}
	if(iChooseRect<0x00 || iChooseRect>=0x05)			//������ֵ����Ч��
	{
		return ;
	}
	__int64 iMyZhuSum = 0;
	for(int i = 0x00;i < MAX_NOTE_COUNT;i++)			//�õ��Ѿ����˵�ע�ܺ�
	{
		iMyZhuSum += m_iMyZhu[i];
	}
	UserInfoForGame TInfo;
	m_pGameFrame->GetUserInfo(myDesk,TInfo);
	if((iMyZhuSum+m_i64ChoumaValue[m_iChooseNoteType])*m_byMaxBase > TInfo.i64Money)	//��⵱ǰ���ѵ�Ǯ����������
	{
		return ;
	}
	if(m_i64MaxNote >0 && m_i64MaxNote < m_i64ChoumaValue[m_iChooseNoteType])	//��ǰ�����¶��ٽ�� m_iMaxNote:�����µĽ���
	{
		return ;
	}
	if((m_i64MaxNote < (iMyZhuSum+m_i64ChoumaValue[m_iChooseNoteType]))*m_byMaxBase && //��⿴�Ƿ�����ע
		(m_i64MaxNote>0))													//m_iMaxNote:������µ������,-1Ϊ������
	{
		return ;
	}

	TagXiaZhu  TXiaZhu;
	TXiaZhu.byStation	= m_pGameFrame->GetMyDeskStation();
	TXiaZhu.iChouMaType = m_iChooseNoteType;	//�õ�����ֵ
	TXiaZhu.iBetArea	= iChooseRect;			//��������
	TXiaZhu.bIsAi		= false;
	m_pGameFrame->SendGameData(&TXiaZhu,sizeof(TXiaZhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
}
//========================================================================================

//==============================================================================================
//���ȷ����ť
//==============================================================================================
void CNewGameClient::ButtonDownOK()	
{
	IRadioButton *pRadioBtnVoice = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(BTN_SET_VOICE)) ;
	IRadioButton *pRadioBtnBGVoice = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(BTN_SET_BKVOICE)) ;

	if(NULL != pRadioBtnVoice&&NULL != pRadioBtnBGVoice)
	{
		m_bSound =pRadioBtnVoice->GetIsSelect() ; 
		m_bSoundPlay =pRadioBtnBGVoice->GetIsSelect() ; 
		CString strCfgName;
		strCfgName.Format("%s%d.bcf",CINIFile::GetAppPath(),NAME_ID);
		DWORD dwCfgFile = ::cfgOpenFile(strCfgName);
		if(dwCfgFile >=0x10) //�ļ��򿪳ɹ�
		{
			//����������Ч
			::cfgSetValue(dwCfgFile,"config","PlaySound",m_bSound);
			//����������Ч
			::cfgSetValue(dwCfgFile,"config","BGPlaySound",m_bSoundPlay);
			::cfgClose(dwCfgFile);						
		}
	}
	SetGameInfordDlg(m_bSound,m_bSoundPlay,false);

	if(m_bSound)			//��Ч
	{
		m_SoundEngine->setAllSoundsPaused(false);
	}
	else
	{
		m_SoundEngine->setAllSoundsPaused(true);
		m_SoundEngine->stopAllSounds();	
	}//end if else

	if(m_bSoundPlay)		//��������
	{
		m_soundEngineBg->setAllSoundsPaused(false);
	}
	else
	{
		m_soundEngineBg->setAllSoundsPaused(true) ; 
		m_soundEngineBg->stopAllSounds() ;
	}//end if else
}
//==============================================================================================
//���������ť
//==============================================================================================
void CNewGameClient::ButtonCancel()													
{
	SetGameInfordDlg(m_bSound,m_bSoundPlay,false);
}
//===============================================================================================
//������������ؼ�������
//===============================================================================================
void	CNewGameClient::ClearAllDeskSHAndMoneyNumber()
{	
	int i=0x00;
	IShCtrl *pSHCtl = NULL ; 
	INum *pNum = NULL;
	IImage *pImage = NULL;

	for(i=0;i<MAX_NOTE_COUNT;i++)
	{
		pSHCtl = dynamic_cast<IShCtrl *>(m_pUI->GetIControlbyID(TIAN_SH+i));
		if(NULL != pSHCtl )
		{
			pSHCtl->SetControlVisible(true) ; 
			pSHCtl->SetShNote(0x00) ;					//���4������ؼ�
		}
	}

	for(i=0x00;i<MAX_NOTE_COUNT;i++)
	{
		pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(TIAN_OWERMONEY_NUM+i));
		if(NULL!=pNum)
		{
			pNum->SetControlVisible(false);
			pNum->SetNewNum(0x00);
		}
		pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(TIAN_ALLMONEY_NUM+i));
		if(pNum!=NULL)
		{
			pNum->SetControlVisible(false);
			pNum->SetNewNum(0x00);
		}
		//-------------------------------------------
		pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(ZHONG_ZHU_TITU+i));
		if(NULL!=pImage)
		{
			pImage->SetControlVisible(false);
		}
		
	}
}
//==============================================================================================
//������ʾ��Ϸ���öԻ���
//bSoundEnable : ��������(��Ч)
//bBGSoundEnable : ������������
//bShow	:	��ʾ��������
//==============================================================================================
void CNewGameClient::SetGameInfordDlg(bool bSoundEnable,bool bBGSoundEnable,bool bShow)
{
	IContain  *pConTain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(CONTAIN_GAME_SET));
	if(NULL!=pConTain )																					//������ʾ����
	{
		pConTain->SetControlVisible(bShow); 
	}
	IRadioButton *pRadioBtnVoice = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(BTN_SET_VOICE)) ;
	if(NULL != pRadioBtnVoice)
	{
		pRadioBtnVoice->SetIsSelect(bSoundEnable) ; 
		pRadioBtnVoice->SetControlVisible(bShow);
	}

	IRadioButton *pRadioBtnBGVoice = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(BTN_SET_BKVOICE)) ;
	if(NULL != pRadioBtnVoice)
	{
		pRadioBtnBGVoice->SetIsSelect(bBGSoundEnable) ; 
		pRadioBtnBGVoice->SetControlVisible(bShow);
	}
}
//=====================================================================================




//========================================================================================
//ʱ�䶨ʱ
//========================================================================================
void CNewGameClient::OnTimer(int nTimerID)
{
	int iShapeDian;
	switch(nTimerID)
	{
	case ID_SEND_CARD_TIMER:				//�������ƶ�ʱ��
		{
			m_pGameFrame->KillTimer(ID_SEND_CARD_TIMER);
			SendCardTimer();					//��������
			break;
		}
	case ID_PLAY_ANIMAL_TIMER:		//���ƶ���
		{
			ControlPlayAnimal();
			break;
		}
	case ID_SHOW_WIN_AREAS:		//�н�������˸
		{
			m_pGameFrame->KillTimer(ID_SHOW_WIN_AREAS);
			m_pGameFrame->SetTimer(ID_SHOW_WIN_AREAS,300);
			m_bShowRect=!m_bShowRect;
			RectXiaZhuGuangFlash(m_bShowRect);				//��ע����
			break;
		}
	case ID_XIAZHU_TIME:
		{
			if(m_iXuaZhuTimeCount>0x00)
			{
				m_iXuaZhuTimeCount--;
			}
			else
			{
				m_iXuaZhuTimeCount=0x00;
				m_pGameFrame->KillTimer(ID_XIAZHU_TIME);
				break;
			}
			if(m_iXuaZhuTimeCount>0x00&&m_iXuaZhuTimeCount<0x06)
			{
				if(GS_BET==m_bGameStation)						//ֻ������ע��ʱ���ڻ�����
					PlayMotionSoundOgg(m_SoundEngine,SOUND_CLOCK,0x01); 
			}
		}
		break;
	default:break;
	}
}
//=============================================================================
//����ʱ�����
//=============================================================================
void	CNewGameClient::SendCardTimer()
{

	BYTE byStation = m_iSendCardTime%5;
	
	if(3 == m_CardSendCount[byStation])		//��4���Ʋ���������ʾ
	{
		m_UserSendCard[byStation][3] = 80;				
	}
	else
	{
		m_UserSendCard[byStation][m_CardSendCount[byStation]] = m_UserCard[byStation][m_CardSendCount[byStation]];
	}

	m_CardSendCount[byStation]++;
	if(m_CardSendCount[byStation]<4)   //�������������ʾ
	{
		//����ǰ������
		SetNoOperateCardInfo(UP_CARD_1+byStation,m_UserSendCard[byStation],m_CardSendCount[byStation],true);
	}
	else				//����ʱ���������ʾ
	{
		BYTE byTmpCard[2] ;
		byTmpCard[0] = m_UserSendCard[byStation][3];
		byTmpCard[1] = m_UserSendCard[byStation][4];
		SetOperateCardInfot(HAND_CARD_1+byStation,byTmpCard,m_CardSendCount[byStation]-3,true);	
	}
	
	//-------------------------------------------------------
	m_iSendCardTime++;     //������λ�ŵı���

	int iAllSendFinish = 0;   //����Ƿ��Ѿ������ƵĿ��Ʊ���
	for(int i=0x00;i<5;i++)
	{
		if(m_CardSendCount[i] == 5)
		{
			++ iAllSendFinish;
		}
	}
	//����Ƿ��Ѿ���������
	if(iAllSendFinish == 5)			
	{
		m_iSendCardTime = 1;	//��ʼ���� ���쿪ʼ
		m_pGameFrame->KillTimer(ID_SEND_CARD_TIMER);						//�رն������ƶ�ʱ��
		m_pGameFrame->KillTimer(ID_PLAY_ANIMAL_TIMER);
		m_pGameFrame->SetTimer(ID_PLAY_ANIMAL_TIMER, 50);

		m_iCardWith = 20;
		m_iControlDecOrAdd = 0x00;
	}
	else
	{
		m_pGameFrame->SetTimer(ID_SEND_CARD_TIMER,300);      //�������ƶ�����ʱ��
	}
	PlayMotionSoundOgg(m_SoundEngine,SOUND_SEND_CARD,0x01);
}
void CNewGameClient::AutoMoveCardSpacing(BYTE iStation,int m_iMoveSpaceCount,bool flag)
{
	
	const int IOperateCardID[5]={HAND_CARD_1,HAND_CARD_2,HAND_CARD_3,HAND_CARD_4,HAND_CARD_5};
	IOperateCard  *pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(IOperateCardID[iStation%5]));
	if(flag && NULL != pOperateCard && m_iMoveSpaceCount<=3)//���ƾ�������
	{
		pOperateCard->SetCardHSpace(15+m_iMoveSpaceCount*8);  //15Ϊ�ƿؼ����Ƽ�࣬8Ϊÿ�ζ�ʱ������ʱ���ƶ�����
		
	}
	else if(NULL != pOperateCard && m_iMoveSpaceCount>3)  //���ƾ����С
	{
		pOperateCard->SetCardHSpace(39-(m_iMoveSpaceCount-3)*8);
	}

}
// =============================================================================
// ��ע������
// =============================================================================
void	CNewGameClient::RectXiaZhuGuangFlash(bool bShow)
{
	IImage *pImage = NULL ;
	int i=0x00;
	for(i=0x00;i<MAX_NOTE_COUNT;i++)
	{
		pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(TIAN_XIAZHU_AREA_PIC+i)) ;
		if(NULL!=pImage)
		{
			pImage->SetControlVisible(bShow && m_iWinQuYu[i]>0);
		}
	}	
}


// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	return true;
}

/// ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_bGameStation=bGameStation;
}

/**
* ��ȡ��Ϸ״̬ 
*
*return BYTE ��Ϸ״̬
*/
BYTE CNewGameClient::GetStationParameter()
{
	return m_bGameStation;
}	

///ֹͣ��Ϸ
bool	CNewGameClient::OnGameQuiting()
{
	if(m_pGameFrame->GetIsWatching())
	{
		return true ; 
	}
	if(m_bGameStation >= GS_BET && m_bGameStation < GS_WAIT_NEXT)
	{
		CString strInfoMsg ;
		if(m_iMyZhu[MAX_NOTE_COUNT]<=0 && m_pGameFrame->GetMyDeskStation() != m_iNowNTstation)
		{
			strInfoMsg.Format("��ǰ��Ϸ����û��ע���Ƿ��뿪�����ˣ�");
		}
		else
		{
			strInfoMsg.Format("��������Ϸ�У������˳������йܣ�ȷ��Ҫ�˳���");
		}
		if(IDOK != AFCMessageBox(strInfoMsg, "��ʾ",AFC_YESNO)) 
		{
			return false ; 
		}	
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

	
	return 0;
}

int CNewGameClient::GameUserCome()
{
	m_byLgStation = m_pGameFrame->GetMyDeskStation();	//������ѵ�λ��
	m_bWatchMode = m_pGameFrame->GetIsWatching();		//��ȡ�Ƿ�Ϊ�Թ�״̬

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

//���Ż��㵽��ͼλ��
BYTE CNewGameClient::ViewStation(BYTE bDeskStation)
{
	return m_pGameFrame->DeskStation2View(bDeskStation);
}

/**
* ������ͨ��Ϣ
* @param CString strTemp [in] ��Ϣ����
* @param BYTE bDeskStation [in] ˵�����λ��
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

//=======================================================================================
//���ű�������
//=======================================================================================
void CNewGameClient::SetPlaySoundBgByOgg(irrklang::ISoundEngine *engine,BYTE bSoundType)
{
	if(!engine)
	{
		return ;
	}
	if(false == m_bSoundPlay)
	{
		return ;
	}
	CString folder,filename;
	folder= "./music/"; 

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 

	switch(bSoundType)
	{
	case  SOUND_NOTE_BG:
		{
			filename = "xiazhubeijing" ; 
			break ; 
		}
	default:
		{
			return ;	
		}
	}
	wsprintf(szFileName,"%s%s%s",folder,filename,".ogg");

	engine->play2D(szFileName,true);
}

/**
* ���Ŷ�����
* @param [in] int MotionType ���嶯��
* @param [in] bool playLooped ����ģʽ;true:ѭ����ģʽ,false:ֻ����һ��
*/
void CNewGameClient::PlayMotionSoundOgg(irrklang::ISoundEngine *engine,BYTE bySoundType,int iSelect)
{
	if(false == m_bSound || NULL == m_SoundEngine)
	{
		return;
	}
	CString folder= "./music/"; 
	CString filename;
	if(iSelect == 0x00)									//���ŵ���ţ������
	{
		filename.Format("n%d.ogg",bySoundType) ;
	}
	else
	{
		switch(bySoundType)
		{
		case SOUND_NOTE:
			{
				filename = "chouma.ogg"; 
				break ;
			}
		case SOUND_CLOCK:
			{
				filename = "Warning.ogg";
				break;
			}
		case SOUND_GAME_OVER:
			{
				filename = "gameover.ogg";
				break;
			}
		case SOUND_XIA_ZHU_BEING:
			{
				filename = "beginGame.ogg";
				break;
			}
		case SOUND_SEND_CARD:
			{
				filename = "sendcard.ogg";
				break;
			}
		case SOUND_NOTE_100w:
			{
				filename = "sound100w.ogg" ;
				break;
			}
		case SOUND_NOTE_1000w:
			{
				filename = "sound1000w.ogg" ;
				break ; 
			}
		case SOUND_WIN:												//Ӯ
			{
				filename = "win.ogg" ;
				break;
			}
		case SOUND_LOSE:											//��
			{
				filename = "lost.ogg" ;
				break;
			}
		case SOUND_HE:												//��
			{
				filename = "he.ogg" ;
				break;
			}
		default:
			break;
		}
	}

	TCHAR fi[MAX_PATH+1] = {0};

	wsprintf(fi,"%s%s",folder,filename);

	m_SoundEngine->play2D(fi,false);

	return;
}


//======================================================================================
///ÿ�ֽ������ʼ����ͼ����
void	CNewGameClient::ResetGameData()
{
	m_bShowRect = false ;//����Ӯ�������˸
	EnableXiaZhuButton(false);
	m_TSuperSetResult.init();
	memset(&m_CardCount, 0, sizeof(m_CardCount));		 //ׯ���Ƶ�����
	memset(&m_iQuYuZhu, 0, sizeof(m_iQuYuZhu));		//����ÿ�������µ�ע�
	memset(&m_iMyZhu, 0, sizeof(m_iMyZhu));			//��������Լ���ÿ�������µ�ע�
	memset(&m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 

	m_iZhongZhu =0;			 //���ѵ�ǰ��ע��	
	m_iZhuangFen=0;		//����ׯ�ҵĵ÷�
	m_iXianFen=0;			//�����мҵĵ÷�
	m_iMeFen=0;			//��������Լ��ĵ÷�

}
///������ע������ע
///@param [in] bool enable �Ƿ����λ���
///@return void
void CNewGameClient::EnableXiaZhuButton(bool bFlag)
{
	if (m_iNowNTstation > PLAY_COUNT							//���ׯ�Һ��Ƿ�Խ��
		|| m_iNowNTstation < 0									//����Ƿ���ׯ��
		||false==m_pGameFrame->GetUserInfo(m_iNowNTstation,m_UserInfo[m_iNowNTstation])
		|| m_bGameStation != GS_BET)
	{
		bFlag = false;
	}
	//-------------------------------------------------------------------------------------
	if(bFlag)
	{
		int i=0;
		__int64 iMyZhu = 0;
		__int64 iQuYuZhu = 0;
		for (i=0; i<MAX_NOTE_COUNT; i++)
		{
			iMyZhu += m_iMyZhu[i];					//���������ע�Ľ��
			iQuYuZhu +=m_iQuYuZhu[i];				//�����������ע���
		}
		BOOL bCanMax=TRUE;

		if(m_pGameFrame->GetUserInfo(m_iNowNTstation,m_UserInfo[m_iNowNTstation])	//����Ƿ���ׯ
			&&m_pGameFrame->GetUserInfo(m_byLgStation,m_UserInfo[m_byLgStation]))	//����Ƿ������
		{
			//--------------------------------------------------
			if(m_UserInfo[m_byLgStation].i64Money >= (m_i64ChoumaValue[5]+ iMyZhu) * m_byMaxBase
			&&m_UserInfo[m_iNowNTstation].i64Money >= (iQuYuZhu +m_i64ChoumaValue[5]) * m_byMaxBase) 
			{
				EnableOneXiaZhuButton(TEN_MILLION_BUTTON,true);	// 1000����밴ť
			}
			else
			{
				EnableOneXiaZhuButton(TEN_MILLION_BUTTON,false);	// 1000����밴ť
			}
			//--------------------------------------------------
			if(m_UserInfo[m_byLgStation].i64Money >= (m_i64ChoumaValue[4]+iMyZhu) * m_byMaxBase
			&&m_UserInfo[m_iNowNTstation].i64Money >= (iQuYuZhu+m_i64ChoumaValue[4]) * m_byMaxBase) 
			{
				EnableOneXiaZhuButton(MILLION_BUTTON,true);	// 100����밴ť
			}
			else
			{
				EnableOneXiaZhuButton(MILLION_BUTTON,false);	// 100����밴ť
			}
			//--------------------------------------------------
			if(m_UserInfo[m_byLgStation].i64Money >= (m_i64ChoumaValue[3]+ iMyZhu) * m_byMaxBase
			&&m_UserInfo[m_iNowNTstation].i64Money >= (iQuYuZhu +m_i64ChoumaValue[3]) * m_byMaxBase) 
			{
				EnableOneXiaZhuButton(HUNDRED_THOUSAND_BUTTON,true);	// 10����밴ť
			}
			else
			{
				EnableOneXiaZhuButton(HUNDRED_THOUSAND_BUTTON,false);	// 10����밴ť
			}
			//--------------------------------------------------
			if(m_UserInfo[m_byLgStation].i64Money >= (m_i64ChoumaValue[2]+ iMyZhu) * m_byMaxBase
			&&m_UserInfo[m_iNowNTstation].i64Money >= (iQuYuZhu +m_i64ChoumaValue[2]) * m_byMaxBase) 
			{
				EnableOneXiaZhuButton(TEN_THOUSAND_BUTTON,true);	// 1����밴ť
			}
			else
			{
				EnableOneXiaZhuButton(TEN_THOUSAND_BUTTON,false);	// 1����밴ť
			}
			//--------------------------------------------------
			if(m_UserInfo[m_byLgStation].i64Money  >= (m_i64ChoumaValue[1]+ iMyZhu) * m_byMaxBase
			&&m_UserInfo[m_iNowNTstation].i64Money >= (iQuYuZhu +m_i64ChoumaValue[1]) * m_byMaxBase) 
			{
				EnableOneXiaZhuButton(THOUSAND_BUTTON,true);	// 1000���밴ť
			}
			else
			{
				EnableOneXiaZhuButton(THOUSAND_BUTTON,false);	// 1000���밴ť
			}
			//--------------------------------------------------
			if(m_UserInfo[m_byLgStation].i64Money >= (m_i64ChoumaValue[0]+ iMyZhu) * m_byMaxBase
			&&m_UserInfo[m_iNowNTstation].i64Money >= (iQuYuZhu +m_i64ChoumaValue[0]) * m_byMaxBase) 
			{
				EnableOneXiaZhuButton(HUNDRED_BUTTON,true);	// 100���밴ť
			}
			else
			{
				EnableOneXiaZhuButton(HUNDRED_BUTTON,false);	// 100���밴ť
			}
		}
		else
		{
			SetXiaZhuButtonEnable(true);		//���е���ע��ť��������ע
		}
		//---------------------------end if else _00
	}
	else
	{
		SetXiaZhuButtonEnable(false);		//�������е���ע��ť
	}

}

//=====================================================================================
//����һ����ע��ť���Ƿ����
//=====================================================================================
void CNewGameClient::EnableOneXiaZhuButton(const int iButtonNum,bool bEnFlag)
{
	IButton *pButton = NULL ;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonNum));
	if(pButton!=NULL)
	{
		pButton->SetControlVisible(true);
		pButton->SetEnable(bEnFlag);
	}
}
//=======================================================================
//�򿪻�ر���ע��ť�Ĺ���
//=======================================================================
void CNewGameClient::SetXiaZhuButtonEnable(bool bFlage)
{
	int i=0x00;
	IButton *pButton = NULL ;

	m_byLgStation = m_pGameFrame->GetMyDeskStation();			//�õ��û����ѵ�λ��

	if(m_iNowNTstation==m_byLgStation)							//�����ׯ�ҵ�λ��������ע
	{
		bFlage=false;
	}

	for(i=0x00;i<ID_XIAZHUBUTTON_NUM;i++)					//��������ע��ť
	{
		pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(HUNDRED_BUTTON+i));
		if(pButton!=NULL)
		{
			pButton->SetControlVisible(true);
			pButton->SetEnable(bFlage);
		}
	}
}
//=======================================================================================
//=======================================================================================
//��ʼ��Ϸ(��עʱ��)
//=======================================================================================
void CNewGameClient::OnHandleBet(BYTE * buffer,int nLen)
{
	if (sizeof(BEGIN_DATA) != nLen)
	{
		return;
	}
	SetStationParameter(GS_BET);				//��Ϸ��Ϊ��ע״̬
	BEGIN_DATA *pBegin = (BEGIN_DATA *)buffer;
	if (NULL == pBegin)
	{
		return;
	}
	
	m_iXiaZhuTime		= pBegin->iXiaZhuTime;		//��עʱ��	
	m_iXuaZhuTimeCount = pBegin->iXiaZhuTime;			
	m_iKaiPaiTime		= pBegin->iKaiPaiTime;		//����ʱ��
	m_iFreeTime			= pBegin->iFreeTime;		//����ʱ��
	m_iNTwin		= pBegin->iNTdata[2];			//ׯ�ҵĳɼ�
	m_iNTju			= pBegin->iZhuangBaShu;			//ׯ�ҵİ���
	m_iPlayCount	= pBegin->iGameBeen;			//�Ѿ������˵ڼ���
	m_iGameCount	= pBegin->iGameCount ;			//�ܹ������˼���

	m_iMaxZhu		= pBegin->iMaxZhu;				///���ֿ��µ����ע
	m_iNowNTstation = pBegin->iNowNtStation;		///ȡ��ׯ�ҵ�λ��

	m_iSendCardTime = 0;							// ���ƶ�������ʱ�䣬���ƶ�����
	m_bNtApplyXZ	= false;						//ׯ��û��������ׯ
	//��ׯ�б�
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_idZhuangList[i][0] = pBegin->iNTlist[i][0];
		m_idZhuangList[i][1] = pBegin->iNTlist[i][1];
	}
	
	//���³�ʼ������
	InitGameData();							
	//��ʾׯ����Ϣ
	ShowNTInfo(true);
	//��ׯ�б���Ϣ
	ShowWaitNtList(true);
	//��ʾ������Ϣ
	ShowCurrentInfo(true);
	
	//������ʾ��ʱ��
	SetShowOrHideClock(m_iXiaZhuTime,0,true);	
	//������ע��ť��״̬
	if(-1!=m_iNowNTstation && m_pGameFrame->GetMyDeskStation()!=m_iNowNTstation)
	{
		//��ʾ��ʾ��Ϣ
		SetTextInfoSz(STATICWORD,"��Ϸ��ʼ�ˣ�����Ѻע...",true);
		EnableXiaZhuButton(true);							//�мҿ��Դ�
	}
	else
	{
		//��ʾ��ʾ��Ϣ
		SetTextInfoSz(STATICWORD,"��Ϸ��ʼ��",true);
		EnableXiaZhuButton(false);							//ׯ�Ҿ�û�б�Ҫ����ע��ť��
	}

	//���������ע����ʾ
	if (m_iMaxZhu >0)
	{
		CString sMsg;
		sMsg.Format("[��ʾ]���������ע����:%I64d",m_iMaxZhu);
		m_pGameFrame->InsertNormalMessage(sMsg.GetBuffer());
	}

	//������ʾ
	SetTextInfoSz(CTN_11112000_CTN_100800_TEXT_1008005,"���óɹ�",m_bIsSuperUser&&m_TSuperSetResult.bSuccesse);
	SetButtonEnable(CTN_11112000_CTN_100800_BTN_1008001,true);
	for(int i=0; i<4; i++)
	{
		SetOneRButton(CTN_11112000_CTN_100800_RButton_1008011+i,false);
		SetOneRButton(CTN_11112000_CTN_100800_RButton_1008021+i,false);
	}
	
	//=================================================================
	if(m_soundEngineBg)					//��ע�ı�����Ч
	{
		m_soundEngineBg->setAllSoundsPaused(false);
		SetPlaySoundBgByOgg(m_soundEngineBg,SOUND_NOTE_BG) ;
	}
	PlayMotionSoundOgg(m_SoundEngine,SOUND_XIA_ZHU_BEING,0x01);
	m_pGameFrame->SetTimer(ID_XIAZHU_TIME,1000);
}
//==============================================================
//������ʾʱ��
//==============================================================
void	CNewGameClient::SetShowOrHideClock(int iMaxNum,int iMinNum,bool bflag)
{
	ITime *pTime = NULL ;
	IContain *pIContain=NULL;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(CLOCK));
	pIContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(CLOCKPIC));

	if(NULL != pTime)
	{
		pTime->SetShowMaxNum(iMaxNum);
		pTime->SetShowMinNum(iMinNum);
		pTime->SetPLay(bflag);
		pTime->SetControlVisible(bflag);
		pIContain->SetControlVisible(bflag);
	}
}
//============================================================================

//========================================================================================================
//�����ı��ؼ���ʾ����(��ʾ����)
//========================================================================================================
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
//======================================================================================
void	CNewGameClient::SetTextInfoSz(const int iTextID,CString sText, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//======================================================================================
//========================================================================
//������ע��������ʾ����ʾ�벻��ʾ
//========================================================================
void CNewGameClient::SetXiaZhuAreaShowOrHide(bool bFlage)
{
	int i=0x00;
	IButton *pButton = NULL ;
	m_byLgStation = m_pGameFrame->GetMyDeskStation();			//�õ��û����ѵ�λ��

	if(m_iNowNTstation==m_byLgStation)					//�����ׯ�ҵ�λ��������ʾ
	{
		bFlage=false;
	}
	for(i=0x00;i<ID_XIAZHUAREA_NUM;i++)					//��������ע����
	{
		pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(TIAN_XIAZHU_AREA_BNT+i));
		if(pButton!=NULL)
		{
			pButton->SetControlVisible(bFlage);
			pButton->SetEnable(bFlage);
		}
	}
}
//==================================================================================
///��ʾ������
//==================================================================================
int		CNewGameClient::SetCurrentCursor(int uID)
{
	IXUICursor *pCursorMouse = m_pUI->GetICursor() ;
	CString sImgeSrc;
	sImgeSrc.Format(".\\resources\\image\\ChouMa\\ChouMa_%d.bmp",uID);
	pCursorMouse->SetCursorImage(CA2W(sImgeSrc));

	
	int iWidth  = pCursorMouse->GetCursorImageWidth()/4;
	int iHeight = pCursorMouse->GetCursorImageHeight(); 
	pCursorMouse->SetPointMode(1);

	pCursorMouse->SetSrcX(iWidth);
	pCursorMouse->SetSrcY(0);
	pCursorMouse->SetSrcW(iWidth);
	pCursorMouse->SetSrcH(iHeight);
	pCursorMouse->SetControlVisible(true);
	
	return 0 ; 
}
//==========================================================================================
//������ʾׯ����Ϣ
//==========================================================================================
void	CNewGameClient::ShowNTInfo(bool bShow)
{
	UserInfoForGame sNtInfo ;
	if(m_pGameFrame->GetUserInfo(m_iNowNTstation,sNtInfo))
	{
		CString sText;
		sText.Format("%s",sNtInfo.szNickName);
		SetTextInfoSz(ZHUANGNAME,sText,bShow);
		SetTextinfo(ZHUANGMONEY,sNtInfo.i64Money,bShow);	//��ʾׯ�ҵ�Ǯ			
		SetTextinfo(ZHUANGSOCE,m_iNTwin,bShow);				//ׯ�ҵĳɼ�
		SetTextinfo(ZHUANGPLAYCOUNT,m_iNTju,bShow);			//��ʾ��ǰ�İ���
	}
	else
	{
		SetTextInfoSz(ZHUANGNAME,"",bShow);
		SetTextinfo(ZHUANGMONEY,0,bShow);		//��ʾׯ�ҵ�Ǯ			
		SetTextinfo(ZHUANGSOCE,0,bShow);		//ׯ�ҵĳɼ�
		SetTextinfo(ZHUANGPLAYCOUNT,0,bShow);	//��ʾ��ǰ�İ���
	}

	if (m_pGameFrame->GetMyDeskStation() == m_iNowNTstation)
	{
		SetButtonVisible(IDD_CHUOMA_QIANGZ,false);
		SetButtonVisible(IDD_CHUOMA_XIAZ,true);
	}
	SetImageVisible(NT_FLAG,m_pGameFrame->GetMyDeskStation() == m_iNowNTstation);

}
//=====================================================================================================
//��ʾ��ׯ�б�
void		CNewGameClient::ShowWaitNtList(bool bShow,bool bShowNew)
{
	//������
	for (int i=0; i<5; i++)
	{
		SetTextInfoSz(CTN_11112000_CTN_1000100_TEXT_10001001+i,"",false);
		//���
		SetTextinfo(CTN_11112000_CTN_1000100_TEXT_10001011+i,0,false);
	}


	int iCount = m_idZhuangList[PLAY_COUNT][0];
	if (iCount <= 5)
	{
		m_iZhuangListInedx = 0;
	}
	else
	{
		//��ʾ����
		if (bShowNew)
		{
			m_iZhuangListInedx = 0;
		}
		else 
		{
			if (m_iZhuangListInedx <= 0)
			{
				m_iZhuangListInedx = 0;
			}
			else
			{
				if (m_iZhuangListInedx+5 >= iCount)
				{
					m_iZhuangListInedx = iCount - 5;
				}
			}
		}
	}



	UserInfoForGame sUser;
	int iShowCount = 0;
	for(int i=m_iZhuangListInedx;i<iCount && iShowCount<5;i++)
	{
		if (-1 != m_idZhuangList[i][0])
		{
			CString sText;
			memset(&sUser,0,sizeof(sUser));
			if (m_pGameFrame->GetUserInfo(m_idZhuangList[i][0],sUser))
			{
				//�ǳ�
				sText.Format("%s",sUser.szNickName);
				SetTextInfoSz(CTN_11112000_CTN_1000100_TEXT_10001001+iShowCount,sText,bShow);
				//���
				SetTextinfo(CTN_11112000_CTN_1000100_TEXT_10001011+iShowCount,sUser.i64Money,bShow);
				iShowCount++;
			}
			//else
			//{
			//	sText.Format("�������");
			//	SetTextInfoSz(CTN_11112000_CTN_1000100_TEXT_10001001+i-m_iZhuangListInedx,sText,bShow);
			//	//���
			//	SetTextinfo(CTN_11112000_CTN_1000100_TEXT_10001011+i-m_iZhuangListInedx,0,bShow);
			//}
		}
	}


	if (bShow)
	{
		if (m_iNowNTstation == m_pGameFrame->GetMyDeskStation())
		{
			SetButtonVisible(IDD_CHUOMA_QIANGZ,false);
			SetButtonVisible(IDD_CHUOMA_XIAZ,true);
			SetButtonEnable(IDD_CHUOMA_XIAZ,!m_bNtApplyXZ);
		}
		else
		{
			ShowShangZhuangOrXiaZhuangBt();
		}
	}
}
//=====================================================================================================
//��ʾ·�� bShowNew-��ʾ���µ�
void		CNewGameClient::ShowLuZiInfo(bool bShow,bool bShowNew)
{
	//������
	for(int i=0; i<9;i++)
	{
		SetImageVisible(CTN_11112000_CTN_1000200_IMG_10002001+i,false);
		SetImageVisible(CTN_11112000_CTN_1000200_IMG_10002011+i,false);
		SetImageVisible(CTN_11112000_CTN_1000200_IMG_10002021+i,false);
		SetImageVisible(CTN_11112000_CTN_1000200_IMG_10002031+i,false);
	}
	if (!bShow)
	{
		return;
	}
	
	//�ȼ����ж��ٸ�
	int iCount = 0;
	for(int i=0; i<GAME_SAVE_COUNT;i++)
	{
		if (m_iResultInfo[i][0] != 0 && m_iResultInfo[i][0] !=0 && m_iResultInfo[i][2] !=0 && m_iResultInfo[i][3] !=0)
		{
			iCount++;
		}
	}

	if (iCount <= 9)
	{
		m_iLuZiIndex = 0;
	}
	else
	{
		//��ʾ����
		if (bShowNew)
		{
			m_iLuZiIndex = iCount -9;
		}
		else 
		{
			if (m_iLuZiIndex <= 0)
			{
				m_iLuZiIndex = 0;
			}
			else
			{
				if (m_iLuZiIndex+9 >= iCount)
				{
					m_iLuZiIndex = iCount - 9;
				}
			}
		}
	}


	if (iCount <= 9)
	{
		//��ʾ�����9��
		for(int i=m_iLuZiIndex; i<iCount;i++)
		{
			for(int j=0; j<MAX_NOTE_COUNT;j++)
			{
				//Ӯ��
				if (m_iResultInfo[i][j] > 0)
				{
					SetImageSrcXY(CTN_11112000_CTN_1000200_IMG_10002001+(j*10)+(i),0,0,bShow);
				}
				else if (m_iResultInfo[i][j] < 0)	//����
				{
					SetImageSrcXY(CTN_11112000_CTN_1000200_IMG_10002001+(j*10)+(i),32,0,bShow);
				}
				else
				{
					SetImageSrcXY(CTN_11112000_CTN_1000200_IMG_10002001+(j*10)+(i),32,0,false);
				}
			}		
		}
	}
	else
	{
		//��ʾ�����9��
		for(int i=m_iLuZiIndex; i<iCount;i++)
		{
			for(int j=0; j<MAX_NOTE_COUNT;j++)
			{
				if (m_iResultInfo[i][j] > 0)	//Ӯ��
				{
					SetImageSrcXY(CTN_11112000_CTN_1000200_IMG_10002001+(j*10)+(i-m_iLuZiIndex),0,0,bShow);
				}
				else if (m_iResultInfo[i][j] < 0)	//����
				{
					SetImageSrcXY(CTN_11112000_CTN_1000200_IMG_10002001+(j*10)+(i-m_iLuZiIndex),32,0,bShow);
				}
				else
				{
					SetImageSrcXY(CTN_11112000_CTN_1000200_IMG_10002001+(j*10)+(i-m_iLuZiIndex),32,0,false);
				}
			}		
		}
	}	
}
//=====================================================================================================
//��ʾ�û�����Ϸ��Ϣ
//=====================================================================================================
void	CNewGameClient::ShowUserInfo(bool bShow)
{
	UserInfoForGame sUserInfo ;
	BYTE myDesk = m_pGameFrame->GetMyDeskStation();	
	if(m_pGameFrame->GetUserInfo(myDesk,sUserInfo))
	{
		SetTextShowText(USERNAME,sUserInfo.szNickName, true);					//��ʾ�û���
	}
	SetTextinfo(USERMONEY,sUserInfo.i64Money,true);							//��ʾ�û���Ǯ
	SetTextinfo(USERSOCE,m_i64MyChengji,true);								//�мҵĵ÷�
}
/*-----------------------------------------------------------------------------*/
//��ʾ��ǰ��Ϣ����ʾ��ǰ�����������
void	CNewGameClient::ShowCurrentInfo(bool bShow)
{
	SetTextinfo(CURRENTJUSHU,m_iPlayCount,bShow);				//��ʾ��ǰ�ľ���
	SetTextinfo(CURRENTBUSHU,m_iGameCount,bShow);				//��ʾ��ǰ�İ���
}
//==================================================================================


//========================================================================================================
//�����ı��ؼ���ʾ����(��ʾ�ַ�)
//========================================================================================================
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

//��ţ
void CNewGameClient::PutBullCard(BYTE iDesk)
{	

	int iShape = m_Logic.GetShape(m_UserCard[iDesk],m_CardCount[iDesk]);
	if (iShape >= EBCT_TYPE_BULL_ONE)
	{
		BYTE BullCard[5] = {0};		// �����������е�3��ţ��
		if(m_Logic.GetBull(m_UserCard[iDesk],5,BullCard) && 1==m_Logic.CountKing(m_UserCard[iDesk],5)) //����һ��ţ�Ƶ����
		{
			BYTE TempHand[5] = {0};	
			for(int j=0;j<5;j++)
			{
				TempHand[j] = m_UserCard[iDesk][j];
			}
			m_Logic.RemoveCard(BullCard,2,TempHand,5);

			ShowOrHideResultCard(iDesk,BullCard,0x03,true,TYPE_UP);	//������ʾ�������		
			ShowOrHideResultCard(iDesk,TempHand,0x02,true,TYPE_DOWN);	//������ʾδ�������	
		}
		else if (m_Logic.GetBull(m_UserCard[iDesk],5,BullCard) && 1!=m_Logic.CountKing(m_UserCard[iDesk],5))
		{
			BYTE TempHand[5] = {0};		// �����������е�2�Ž�����
			for (int j=0; j<5; j++)
			{
				TempHand[j] = m_UserCard[iDesk][j];
			}
			m_Logic.RemoveCard(BullCard,3,TempHand,5);				//��3��ţ�Ƴ�ȥ��ʣ���2���Ʊ��浽TempHand��

			ShowOrHideResultCard(iDesk,BullCard,0x03,true,TYPE_UP);	//������ʾ�������		
			ShowOrHideResultCard(iDesk,TempHand,0x02,true,TYPE_DOWN);	//������ʾδ�������					
		}
		else
		{
			ShowOrHideResultCard(iDesk,&m_UserCard[iDesk][2],0x03,true,TYPE_UP);	//������ʾ�������
			ShowOrHideResultCard(iDesk,&m_UserCard[iDesk][0],0x02,true,TYPE_DOWN);	//������ʾδ�������
		}	
	}
	else
	{
		SetNoOperateCardInfo(UP_CARD_1+iDesk,m_UserCard[iDesk],m_CardCount[iDesk],true);
		SetOperateCardInfot(HAND_CARD_1+iDesk,NULL,0,false);	
	}	
}
//��ʾ��������ͼƬ
int CNewGameClient::ShowOrHideHandImg(BYTE iStation,bool flag)
{
	IImage *pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(HAND_IMG0+iStation));	
	if(NULL != pImage)
	{
		pImage->SetControlVisible(flag);
	}
	return 0x00;
}
///��ʾ��ҵ�����
int CNewGameClient::ShowUserCardShape(BYTE iStaion , bool bshow ,bool bAllHand)
{		
	IImage * pImg = NULL;
	if(bAllHand)
	{
		for(int i = 0 ; i<5 ;i++)
		{
			pImg = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(SHAPE_TOP+i));
			if(NULL != pImg)
			{
				wchar_t pchar[256] = {0};
				CString strTemp;
				GetCardShapePath(i,strTemp,bAllHand);
				pImg->LoadPic(CA2W(strTemp));
				pImg->SetGameW(pImg->GetPicWidth());
				pImg->SetGameH(pImg->GetPicHeight());
				pImg->SetControlVisible(bshow);				
			}		
		}
		return 0;
	}	
	pImg = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(SHAPE_TOP+iStaion));
	if(NULL != pImg )
	{	
		wchar_t pchar[256] = {0};
		CString strTemp;
		GetCardShapePath(iStaion,strTemp,bAllHand);
		pImg->LoadPic(CA2W(strTemp));
		pImg->SetGameW(pImg->GetPicWidth());
		pImg->SetGameH(pImg->GetPicHeight());
		pImg->SetControlVisible(bshow);		
	}
	return 0 ; 
}
//ȡ��ͼƬ��·��
int CNewGameClient::GetCardShapePath(BYTE iStation ,CString &str,bool bAllHand)
{
	if(iStation<0||iStation>4)
	{
		return 0 ; 
	}
	str = ".\\resources\\image\\";
	int iPaiDian = 0;
	int Shape = m_Logic.GetShape(m_UserCard[iStation],m_CardCount[iStation]);	
	switch(Shape)
	{
	case EBCT_TYPE_BULL_joker:
	case EBCT_TYPE_BULL_JOKER:
	case EBCT_TYPE_BULL_BULL:
		iPaiDian = 10;
		break;
	case EBCT_TYPE_BULL_NINE:
		iPaiDian = 9;
		break;
	case EBCT_TYPE_BULL_EIGHT:
		iPaiDian = 8;
		break;
	case EBCT_TYPE_BULL_SEVEN:
		iPaiDian = 7;
		break;
	case EBCT_TYPE_BULL_SIX:
		iPaiDian = 6;
		break;
	case EBCT_TYPE_BULL_FIVE:
		iPaiDian = 5;
		break;
	case EBCT_TYPE_BULL_FOUR:
		iPaiDian = 4;
		break;
	case EBCT_TYPE_BULL_THREE:
		iPaiDian = 3;
		break;
	case EBCT_TYPE_BULL_TWO:
		iPaiDian = 2;
		break;
	case EBCT_TYPE_BULL_ONE:
		iPaiDian = 1;
		break;
	case EBCT_TYPE_NO_POINT:
		iPaiDian = 0;
		break;
	default:
		break;
	}
	str.Format(".\\resources\\image\\n%d.bmp",iPaiDian) ;
	m_byCardShap = iPaiDian;
	
	return 0;
}
//========================================================================================
//��ʾ������ׯ����Ҫ��ׯ
//========================================================================================
void	CNewGameClient::ShowShangZhuangOrXiaZhuangBt()				
{
	IButton *pButton = NULL ;
	if(CheckMeZhuang())					//��������Ƿ���ׯ��
	{
		SetButtonVisible(IDD_CHUOMA_QIANGZ,false);
		SetButtonVisible(IDD_CHUOMA_XIAZ,true);
		SetButtonEnable(IDD_CHUOMA_XIAZ,true);
	}
	else
	{
		SetButtonVisible(IDD_CHUOMA_QIANGZ,true);
		SetButtonEnable(IDD_CHUOMA_QIANGZ,true);
		SetButtonVisible(IDD_CHUOMA_XIAZ,false);
	}
}
//============================================================================================
//���յ���ע��Ϣ֮����ע��ť�Ĵ���
//============================================================================================
void CNewGameClient::RecXiaZhuDataAfterEnableXiaZhuButton(const int count)
{
	switch (count)
	{
	case 0:
		{		
			EnableOneXiaZhuButton(TEN_MILLION_BUTTON,false);			// 1000W���밴ť
			EnableOneXiaZhuButton(MILLION_BUTTON,false);				// 100W���밴ť
			EnableOneXiaZhuButton(HUNDRED_THOUSAND_BUTTON,false);		// 10W���밴ť
			EnableOneXiaZhuButton(TEN_THOUSAND_BUTTON,false);			// 1W���밴ť
			EnableOneXiaZhuButton(THOUSAND_BUTTON,false);				// 1000���밴ť
			break;
		}
	case 1:
		{
			EnableOneXiaZhuButton(TEN_MILLION_BUTTON,false);			// 1000W���밴ť
			EnableOneXiaZhuButton(MILLION_BUTTON,false);				// 100W���밴ť
			EnableOneXiaZhuButton(HUNDRED_THOUSAND_BUTTON,false);		// 10W���밴ť
			EnableOneXiaZhuButton(TEN_THOUSAND_BUTTON,false);			// 1W���밴ť
			break;
		}
	case 2:
		{
			EnableOneXiaZhuButton(TEN_MILLION_BUTTON,false);			// 1000W���밴ť
			EnableOneXiaZhuButton(MILLION_BUTTON,false);				// 100W���밴ť
			EnableOneXiaZhuButton(HUNDRED_THOUSAND_BUTTON,false);		// 10W���밴ť
			break;
		}
	case 3:
		{
			EnableOneXiaZhuButton(TEN_MILLION_BUTTON,false);			// 1000W���밴ť
			EnableOneXiaZhuButton(MILLION_BUTTON,false);				// 100W���밴ť
			break;
		}
	case 4:
		{
			EnableOneXiaZhuButton(TEN_MILLION_BUTTON,false);			// 1000W���밴ť
			break;
		}
	default:
		{
			EnableOneXiaZhuButton(TEN_MILLION_BUTTON,false);			// 1000W���밴ť
			EnableOneXiaZhuButton(MILLION_BUTTON,false);				// 100W���밴ť
			EnableOneXiaZhuButton(HUNDRED_THOUSAND_BUTTON,false);		// 10W���밴ť
			EnableOneXiaZhuButton(TEN_THOUSAND_BUTTON,false);			// 1W���밴ť
			EnableOneXiaZhuButton(THOUSAND_BUTTON,false);				// 1000���밴ť
			EnableOneXiaZhuButton(HUNDRED_BUTTON,false);				// 100���밴ť
			CString strInfo ;
			strInfo.Format("����ʾ����ע�Ѵ�������ޣ��޷�������ע!");
			m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
			break;
		}
	}//end switch
}

//======================================================================================
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
//==================================================================================
//����ͨɱ��ͨ��
//==================================================================================
int CNewGameClient::PlayAllWinOrLostAni(BYTE iGameStyle)
{
	m_TongShaOrTongBeiFlag = CheckGameResult();
	if(-1==m_TongShaOrTongBeiFlag)
		return 0x00;
	IExAnimate  *pGameAni = NULL ;
	if(0 == m_TongShaOrTongBeiFlag)				//ͨɱ
	{
		pGameAni = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(EXT_WIN_ALL_ANI));
		if(NULL != pGameAni)
		{
			pGameAni->SetControlVisible(true) ; 
			pGameAni->SetIsSymmetrical(false) ; 
			pGameAni->SetPlayState(true) ;
			pGameAni->SetPlayNum(6) ;
			pGameAni->SetIsLoop(false) ;
		}
		//PlayMotionSoundOgg(m_SoundEngine,SOUND_TONG_SHA);
	}
	else if(1 == m_TongShaOrTongBeiFlag)					//ͨ��
	{
		pGameAni = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(EXT_LOST_ALL_ANI));
		if(NULL != pGameAni)
		{
			pGameAni->SetControlVisible(true) ; 
			pGameAni->SetIsSymmetrical(false) ; 
			pGameAni->SetPlayState(true) ; 
			pGameAni->SetPlayNum(6) ;
			pGameAni->SetIsLoop(false) ;
		}
		//PlayMotionSoundOgg(m_SoundEngine,SOUND_TONG_PEI);
	}
	else
	{
		m_TongShaOrTongBeiFlag=-1;				//����֮��͹���
		return 0 ; 
	}

	m_TongShaOrTongBeiFlag=-1;				//����֮��͹���
	return 0 ;
}
//======================================================================================================
//��⵱ǰ����Ϸ���
//======================================================================================================
int CNewGameClient::CheckGameResult()
{
	int i=0x00;
	int j=0x00;
	int temp=-1;
	if (m_iResultInfo[GAME_SAVE_COUNT-1][0] != 0||m_iResultInfo[GAME_SAVE_COUNT-1][1] != 0||m_iResultInfo[GAME_SAVE_COUNT-1][2] != 0)
	{
		if((m_iResultInfo[GAME_SAVE_COUNT-1][0]<0x00)&&
			(m_iResultInfo[GAME_SAVE_COUNT-1][1]<0x00)&&
			(m_iResultInfo[GAME_SAVE_COUNT-1][2]<0x00))
		{
			temp=0x00;
			return temp;
		}
		if((m_iResultInfo[GAME_SAVE_COUNT-1][0]>0x00)&&
			(m_iResultInfo[GAME_SAVE_COUNT-1][1]>0x00)&&
			(m_iResultInfo[GAME_SAVE_COUNT-1][2]>0x00))
		{
			temp=0x01;
			return temp;
		}
		return temp;
	}
	for(i=0x00;i<GAME_SAVE_COUNT;i++)
	{
		if(m_iResultInfo[i][0] == 0||m_iResultInfo[i][1] == 0||m_iResultInfo[i][2] == 0)
		{
			if((m_iResultInfo[i-1][0]<0x00)&&
				(m_iResultInfo[i-1][1]<0x00)&&
				(m_iResultInfo[i-1][2]<0x00))
			{
				temp=0x00;
				return temp;
			}//end if
			if((m_iResultInfo[i-1][0]>0x00)&&
				(m_iResultInfo[i-1][1]>0x00)&&
				(m_iResultInfo[i-1][2]>0x00))
			{
				temp=0x01;
				return temp;
			}//end if
			return temp;
		}//end if
	}//end for
	return temp;
}


void CNewGameClient::PlayAnimal(int iDesk,int iCardLeafWith)
{
	IOperateCard *pOperateCard = NULL;
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(HAND_CARD_1 + iDesk));
	if(pOperateCard != NULL)
	{
		pOperateCard->SetCardHSpace(iCardLeafWith);
		BYTE byTmpCard[2];
		byTmpCard[0] = m_UserCard[iDesk][3];
		byTmpCard[1] = m_UserCard[iDesk][4];
		pOperateCard->SetCardList(byTmpCard , 2) ;
	}
}

//======================================================================================
//�ؼ����Ŷ���
//======================================================================================
void	CNewGameClient::ControlPlayAnimal()
{
	int iDesk = m_iSendCardTime%5;
	bool AnimalFinish = true;
	ShowOrHideHandImg(m_iSendCardTime,true); //��ʾ����ͼƬ
	if(m_iControlDecOrAdd == 0x00)
	{
		m_iCardWith += 3;
	}
	else if(m_iControlDecOrAdd == 0x01)
	{
		m_iCardWith -= 3;
	}
	if(m_iCardWith >= 60)
	{
		m_iControlDecOrAdd = 0x01;
	}
	else if(m_iCardWith < 20)
	{	
		ShowOrHideHandImg(m_iSendCardTime,false); //��������ͼƬ
		PutBullCard(m_iSendCardTime);   //��ʾ��ţ�����
		ShowUserCardShape(m_iSendCardTime , true ,false); //��ʾ����ͼƬ
		PlayMotionSoundOgg(m_SoundEngine,m_byCardShap,0x00);

		m_iControlDecOrAdd = 0x00;
		m_iCardWith = 20;
		AnimalFinish = false;
		m_iSendCardTime ++;
		m_iSendCardTime = m_iSendCardTime % 0x05;
		if(m_iSendCardTime==0x01) //5������ȫ��������
		{
			m_iSendCardTime=0x00;
			m_pGameFrame->KillTimer(ID_PLAY_ANIMAL_TIMER);
			m_pGameFrame->SetTimer(ID_SHOW_WIN_AREAS,1000);		//������˸��ʱ��		
		}		
	}
	if(AnimalFinish)
	{
		PlayAnimal(iDesk,m_iCardWith);
	}
}
//======================================================================================================
//��ʾ���˶Ի���
void		CNewGameClient::ShowSuperDlag(bool bShow)
{
	SetContainVisible(CTN_11112000_CTN_100800,bShow);
	SetTextInfoSz(CTN_11112000_CTN_100800_TEXT_1008005,"���óɹ�",bShow&&m_TSuperSetResult.bSuccesse);
	for(int i=0; i<4; i++)
	{
		SetOneRButton(CTN_11112000_CTN_100800_RButton_1008011+i,false);
		SetOneRButton(CTN_11112000_CTN_100800_RButton_1008021+i,false);
	}
	
	//�Ƿ��Ѿ�������
	for(int i=0; i<4; i++)
	{
		if (1 == m_TSuperSetResult.iAreaContrl[i])
		{
			SetOneRButton(CTN_11112000_CTN_100800_RButton_1008011+i,bShow);
			SetOneRButton(CTN_11112000_CTN_100800_RButton_1008021+i,false);
		}
		else if (-1 == m_TSuperSetResult.iAreaContrl[i])
		{
			SetOneRButton(CTN_11112000_CTN_100800_RButton_1008011+i,false);
			SetOneRButton(CTN_11112000_CTN_100800_RButton_1008021+i,bShow);
		}
	}
	SetOneRButton(CTN_11112000_CTN_100800_RButton_1008004,bShow&&(!m_bShowRobortNote));

}
//======================================================================================================
//���ͳ�������
void		CNewGameClient::SendSuperSet()
{
	TagSuperSet	TSuperSet;
	TSuperSet.byStation = m_pGameFrame->GetMyDeskStation();
	for(int i=0; i<4; i++)
	{
		if (GetOneRButton(CTN_11112000_CTN_100800_RButton_1008011+i))
		{
			TSuperSet.iAreaContrl[i] = 1;
		}
		else if (GetOneRButton(CTN_11112000_CTN_100800_RButton_1008021+i))
		{
			TSuperSet.iAreaContrl[i] = -1;
		}
	}
	m_pGameFrame->SendGameData(&TSuperSet,sizeof(TSuperSet),MDM_GM_GAME_NOTIFY,ASS_SUPER_SET,0);
}
//======================================================================================================
//============================================
//==============================================================================================
//����һ����ѡ��ť
//==============================================================================================
void CNewGameClient::SetOneRButton(const int iRbuttonID,bool bSelect)
{
	IRadioButton *pRadioBtn = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRbuttonID));
	if(NULL != pRadioBtn)
	{
		pRadioBtn->SetIsSelect(bSelect) ;
	}
}
//==============================================================================================
//ȡ��һ����ѡ��ť�ĵ�ǰ��״̬
//==============================================================================================
bool CNewGameClient::GetOneRButton(const int iRbuttonID)
{
	IRadioButton *pRadioBtn = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRbuttonID));
	if(NULL != pRadioBtn)
	{
		return pRadioBtn->GetIsSelect() ;
	}
	return false;
}
//======================================================================================================









//���ÿɲ������ƿؼ���Ϣ
void	CNewGameClient::SetOperateCardInfot(const int iID,BYTE CardList[],int CardNum,bool bShow)
{
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iID));
	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(bShow) ; 
		pOperateCard->SetCardList(CardList, CardNum) ;
	}
}

//���ò��ɲ����ƿؼ���Ϣ
void	CNewGameClient::SetNoOperateCardInfo(const int iID,BYTE CardList[],int CardNum,bool bShow)
{
	INoMoveCard  *pNoMoveCard = NULL ;
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iID));
	if(NULL != pNoMoveCard) //��ʾ�������������
	{
		pNoMoveCard->SetControlVisible(bShow) ; 
		pNoMoveCard->SetCardList(CardList, CardNum) ;
	}
}

//���ð�ť�Ƿ����
void	CNewGameClient::SetButtonEnable(const int iID,bool bEnable)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iID));
	if(NULL != pButton) //��ʾ�������������
	{
		pButton->SetEnable(bEnable);
	}
}


//���ð�ť�Ƿ�ɼ�
void	CNewGameClient::SetButtonVisible(const int iID,bool bVisible)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iID));
	if(NULL != pButton) //��ʾ�������������
	{
		pButton->SetControlVisible(bVisible);
	}
}


//����ͼƬ�Ƿ�ɼ�
void	CNewGameClient::SetImageVisible(const int iImageID,bool bShow)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if(NULL != pImage) //��ʾ�������������
	{
		pImage->SetControlVisible(bShow);
	}
}

//����ͼƬ��Ϣ
void	CNewGameClient::SetImageSrcXY(const int iImageID, int iSrcX, int iSrcY,bool bShow)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if(NULL != pImage) //��ʾ�������������
	{
		pImage->SetControlVisible(bShow);
		pImage->SetSrcX(iSrcX);
		pImage->SetSrcY(iSrcY);
	}
}

//�����������ؼ���Ϣ
void	CNewGameClient::SetShCtrlInfo(const int iShCrtlID,int iNum,bool bShow)
{
	IShCtrl *pSHCtl = NULL ; 
	pSHCtl = dynamic_cast<IShCtrl *>(m_pUI->GetIControlbyID(iShCrtlID));
	if(NULL != pSHCtl )
	{
		pSHCtl->SetControlVisible(bShow) ; 
		pSHCtl->SetShNote(iNum) ;
	}
}

//�����޷������ֿؼ�����
void	CNewGameClient::SetINumInfoNoSymbol(const int iINumID,__int64 i64Num,bool bShow)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iINumID));
	if(NULL!=pNum)
	{
		pNum->SetControlVisible(bShow);
		pNum->SetNum(i64Num);
	}
}

//���������Ƿ�ɼ�
void	CNewGameClient::SetContainVisible(const int iID,bool bVisible)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iID));
	if(NULL!=pContain)
	{
		pContain->SetControlVisible(bVisible);
	}
}