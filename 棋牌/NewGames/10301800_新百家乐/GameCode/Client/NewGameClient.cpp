#include "stdafx.h"
#include "NewGameClient.h"
#include "../common/IGameFrame.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "GameControlsIDDefine.h"
#include "Common/UIMessageDefines.h"
//#include "LoveSendClassInExe.h"


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

CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;
	m_SoundEngineGame = createIrrKlangDevice();
	m_SoundEnginBg = createIrrKlangDevice() ; 
	InitialGameData() ; 

	//������������ע����
	CreateBetRgn();
}

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
int		CNewGameClient::Initial()
{
	//����UI
	wchar_t wszUIFile[MAX_PATH];
	::swprintf_s(wszUIFile,L"UI_%d_BJL.dat",NAME_ID);
	m_pUI->LoadData(wszUIFile);
	return 0;
}
//-----------------------------------------------------------------------------------
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	//���Ͻ���ʾ��Ϣ
	for(int i=0; i<3; i++)
	{
		SetTextinfo(CTN_101000_TEXT_1010001+i,"",false);
	}
	for(int i=0; i<10; i++)
	{
		SetTextinfo(CTN_101000_TEXT_10100011+i,"",false);
		SetTextinfo(CTN_101000_TEXT_10100031+i,"",false);
		SetImageVisible(CTN_100600_IMG_10060011+i,false);
		SetImageVisible(CTN_100600_IMG_10060031+i,false);
		SetImageVisible(CTN_100600_IMG_10060051+i,false);
	}
	//ׯ����Ϣ
	for(int i=0; i<4; i++)
	{
		SetTextinfo(CTN_100500_TEXT_10050001+i,"",false);
	}
	//������Ϣ
	ShowMyInfo(false);
	//��ׯ�б�
	for(int i=0; i<5; i++)
	{
		SetTextinfo(CTN_100400_TEXT_10040021+i,"",false);
		SetTextinfo(CTN_100400_TEXT_10040031+i,"",false);
		SetTextinfo(CTN_100400_TEXT_10040041+i,"",false);
	}
	//��ע��ť������
	for (int i=0; i<6; i++)
	{
		SetBtEnable(CTN_100200_BTN_10020001+i,false);
	}
	SetBtEnable(CTN_100400_BTN_10040011,false);
	SetBtEnable(CTN_100400_BTN_10040012,false);
	for(int i=0; i<BET_ARES;i++)
	{
		//�������
		SetShCtrlInfo(CTN_100900_ShCtrl_1009011+i,0,true);
		//�н�������˸ͼƬ
		SetImageVisible(CTN_1000000_CTN_100700_IMG_10070001+i,false);

		//��ע
		SetTextinfo(CTN_100900_TEXT_1009051+i,"0.0",false);
		SetTextinfo(CTN_100900_TEXT_1009031+i,"0.0",false);

	}
	//���ñ���ͼƬ��Ӧ����ƶ�
	SetImageVisible(100001,true,true);
	ShowSuperResult(false);

	//��ʾ��ׯ��ʾ
	SetImageVisible(CTN_100100_IMG_1001002,false);
	SetImageVisible(CTN_100100_IMG_1001003,false);
	SetImageVisible(CTN_100100_IMG_1001004,false);
	SetBtVisible(BTN_1000010,false);
	SetBtEnable(BTN_1000010,false);
	//����ׯ����Ϣ
	ShowNtInfo(false);
	//������ׯ�б�
	ShowWaitNtInfo(false);
	//����·��
	ShowLuziPoint(false);
	//����������װ��ť
	SetBtVisible(CTN_100400_BTN_10040002,false);
	//����ʱ
	ShowOrHideTimer(false,0);
	//���ƿ򱳾�
	SetContainPosition(CTN_100300,m_iSendCardBGPoint[0],m_iSendCardBGPoint[1],false);
	//�����
	ShowGameResult(false);


	return 0;
}
//---------------------------------------------------------------------------------------
//����UI���� 
void	CNewGameClient::ReSetUiDlag()
{
	//���Ͻ���ʾ��Ϣ
	for(int i=0; i<3; i++)
	{
		SetTextinfo(CTN_101000_TEXT_1010001+i,"0.0",true);
	}
	
	//��ע��ť������
	for (int i=0; i<6; i++)
	{
		SetBtEnable(CTN_100200_BTN_10020001+i,false);
	}

	for(int i=0; i<BET_ARES;i++)
	{
		//�������
		SetShCtrlInfo(CTN_100900_ShCtrl_1009011+i,0,true);
		//�н�������˸ͼƬ
		SetImageVisible(CTN_1000000_CTN_100700_IMG_10070001+i,false);
		//��ע
		//SetInumInfo(CTN_100900_Num_1009021+i,0,false);
		SetTextinfo(CTN_100900_TEXT_1009051+i,"0.0",false);
		SetTextinfo(CTN_100900_TEXT_1009031+i,"0.0",false);
	}
	//��ʾ��ע
	SetTextinfo(CTN_100900_TEXT_10090003,"0.0",false);
	//�Լ�������ע
	SetTextinfo(CTN_100900_TEXT_10090043,"0.0",false);

	//������ׯ��ׯ��ť״̬
	SetBtEnable(CTN_100400_BTN_10040001,true);
	SetBtEnable(CTN_100400_BTN_10040002,true);

	//��ʾ��ׯ��ʾ
	SetImageVisible(CTN_100100_IMG_1001002,false);
	SetImageVisible(CTN_100100_IMG_1001003,false);
	SetImageVisible(CTN_100100_IMG_1001004,false);
	//���ƿ򱳾�
	//��ʾ��
	SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030001,NULL,0,false);
	SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030002,NULL,0,false);
	//��ʾ�����Ƶĵ���
	SetImageXYInfo(CTN_100300_IMG_10030021,m_iZCardInfo[0]*97,false);
	SetImageXYInfo(CTN_100300_IMG_10030022,m_iZCardInfo[0]*97,false);
	SetContainPosition(CTN_100300,m_iSendCardBGPoint[0],m_iSendCardBGPoint[1],false);
	//�����
	ShowGameResult(false);
}
//---------------------------------------------------------------------------------------
//������������ע����
void		CNewGameClient::CreateBetRgn()
{
	//0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	//�� 
	POINT ptPonit0[4];
	memset(ptPonit0,0,sizeof(ptPonit0));
	ptPonit0[0].x = 562;
	ptPonit0[0].y = 265;

	ptPonit0[1].x = 562;
	ptPonit0[1].y = 335;

	ptPonit0[2].x = 767;
	ptPonit0[2].y = 335;

	ptPonit0[3].x = 767;
	ptPonit0[3].y = 265;
	
	m_rgnNoteRect[0].CreatePolygonRgn(ptPonit0,4,ALTERNATE);

	//������
	POINT ptPonit1[4];
	ptPonit1[0].x = 562;
	ptPonit1[0].y = 461;

	ptPonit1[1].x = 562;
	ptPonit1[1].y = 533;

	ptPonit1[2].x = 767;
	ptPonit1[2].y = 533;

	ptPonit1[3].x = 767;
	ptPonit1[3].y = 461;
	m_rgnNoteRect[1].CreatePolygonRgn(ptPonit1,4,ALTERNATE);

	//����
	POINT ptPonit2[4];
	ptPonit2[0].x = 562;
	ptPonit2[0].y = 194;

	ptPonit2[1].x = 562;
	ptPonit2[1].y = 265;

	ptPonit2[2].x = 767;
	ptPonit2[2].y = 265;

	ptPonit2[3].x = 767;
	ptPonit2[3].y = 194;

	m_rgnNoteRect[2].CreatePolygonRgn(ptPonit2,4,ALTERNATE);


	//��
	POINT ptPonit3[4];
	ptPonit3[0].x = 45;
	ptPonit3[0].y = 265
		;

	ptPonit3[1].x = 45;
	ptPonit3[1].y = 335;

	ptPonit3[2].x = 250;
	ptPonit3[2].y = 335;

	ptPonit3[3].x = 250;
	ptPonit3[3].y = 265;
	m_rgnNoteRect[3].CreatePolygonRgn(ptPonit3,4,ALTERNATE);

	//������
	POINT ptPonit4[4];
	ptPonit4[0].x = 45;
	ptPonit4[0].y = 461;

	ptPonit4[1].x = 45;
	ptPonit4[1].y = 533;

	ptPonit4[2].x = 250;
	ptPonit4[2].y = 533;

	ptPonit4[3].x = 250;
	ptPonit4[3].y = 461;
	m_rgnNoteRect[4].CreatePolygonRgn(ptPonit4,4,ALTERNATE);

	//���
	POINT ptPonit5[4];
	ptPonit5[0].x = 45;
	ptPonit5[0].y = 194;

	ptPonit5[1].x = 45;
	ptPonit5[1].y = 265;

	ptPonit5[2].x = 250;
	ptPonit5[2].y = 265;

	ptPonit5[3].x = 250;
	ptPonit5[3].y = 194;


	m_rgnNoteRect[5].CreatePolygonRgn(ptPonit5,4,ALTERNATE);

	//��
	POINT ptPonit6[4];
	ptPonit6[0].x = 251;
	ptPonit6[0].y = 336;

	ptPonit6[1].x = 251;
	ptPonit6[1].y = 461;

	ptPonit6[2].x = 561;
	ptPonit6[2].y = 461;

	ptPonit6[3].x = 561;
	ptPonit6[3].y = 336;
	m_rgnNoteRect[6].CreatePolygonRgn(ptPonit6,4,ALTERNATE);

	//ͬ���
	POINT ptPonit7[4];
	ptPonit7[0].x = 251;
	ptPonit7[0].y = 461;

	ptPonit7[1].x = 251;
	ptPonit7[1].y = 533;

	ptPonit7[2].x = 561;
	ptPonit7[2].y = 533;

	ptPonit7[3].x = 561;
	ptPonit7[3].y = 461;
	m_rgnNoteRect[7].CreatePolygonRgn(ptPonit7,4,ALTERNATE);

	//ׯ��
	POINT ptPonit8[4];
	ptPonit8[0].x = 562;
	ptPonit8[0].y = 337;

	ptPonit8[1].x = 562;
	ptPonit8[1].y = 397;

	ptPonit8[2].x = 767;
	ptPonit8[2].y = 397;

	ptPonit8[3].x = 767;
	ptPonit8[3].y = 337;
	m_rgnNoteRect[8].CreatePolygonRgn(ptPonit8,4,ALTERNATE);

	//ׯ˫
	POINT ptPonit9[4];
	ptPonit9[0].x = 562;
	ptPonit9[0].y = 399;

	ptPonit9[1].x = 562;
	ptPonit9[1].y = 459;

	ptPonit9[2].x = 767;
	ptPonit9[2].y = 459;

	ptPonit9[3].x = 767;
	ptPonit9[3].y = 399;
	m_rgnNoteRect[9].CreatePolygonRgn(ptPonit9,4,ALTERNATE);

	//�е�
	POINT ptPonit10[4];
	ptPonit10[0].x = 45;
	ptPonit10[0].y = 337;

	ptPonit10[1].x = 45;
	ptPonit10[1].y = 397;

	ptPonit10[2].x = 250;
	ptPonit10[2].y = 397;

	ptPonit10[3].x = 250;
	ptPonit10[3].y = 337;
	m_rgnNoteRect[10].CreatePolygonRgn(ptPonit10,4,ALTERNATE);

	//��˫
	POINT ptPonit11[4];
	ptPonit11[0].x = 45;
	ptPonit11[0].y = 399;

	ptPonit11[1].x = 45;
	ptPonit11[1].y = 459;

	ptPonit11[2].x = 250;
	ptPonit11[2].y = 459;

	ptPonit11[3].x = 250;
	ptPonit11[3].y = 399;
	m_rgnNoteRect[11].CreatePolygonRgn(ptPonit11,4,ALTERNATE);
}
//---------------------------------------------------------------------------------------
///��ʼ����Ϸ�е�����
void	CNewGameClient::InitialGameData()
{
	m_bIsPlaying = false;
	m_byGameStation = GS_WAIT_AGREE;
	m_iMyDeskStation = -1;		///����Լ�����Ϸλ��
	m_bWatchMode = true;
	m_bIsSuper = false;

	m_iXiaZhuTime	= 15;			///��עʱ��	
	m_iKaiPaiTime	= 15;			///����ʱ��  
	m_iShowResult	= 5;			///��ʾ�����ʱ��
	m_iFreeTime		= 15;			///����ʱ��	

	m_iLimitMoney	= 0;			///<��ׯ��Ҫ�����ٽ��
	m_iMaxNote		= 0;			///��ǰ����������µ���ע

	m_iNTju			= 0;			///<��ǰׯ�ҽ��еľ���
	m_iNowNTstation	= 0;			///<��ǰׯ�ҵ�λ��

	m_i64MyWinMoney = 0;			//ׯ�ҵĳɼ�
	m_i64MyBetCount = 0;			//�Լ���ע����
	m_i64MyWinMoney	= 0;			//�Լ���Ӯ���
	m_i64ZhuangScore= 0;			//ׯ�ұ��ֵ÷�
	m_i64MeScore	= 0;			//��ұ��ֵĵ÷�
	m_i64MeFanFen	= 0;			//�����Լ��ķ�������
	m_iWinShowIndex	= 0;
	m_bySelBetAres	= 255;			//�н�������˸��ʶ
	m_bySelChouMaType = 255;
	m_iLuziIndex = 0;				//·���±�
	m_iWaitIndex = 0;
	m_iSendCardBGPoint[0]= 310;
	m_iSendCardBGPoint[1]= -250;	//���Ʊ���λ��

	m_iPlayCount	= 0;			//���˶�����
	m_iWinCount		= 0;			//Ӯ�˶�����
	
	m_DqLuziData.clear();		//·������

	ZeroMemory(m_i64MaxZhu,sizeof(m_i64MaxZhu));	//ÿ���������µ����ע	
	memset(m_iZhuangList,-1,sizeof(m_iZhuangList));	///ׯ���б�
	memset(m_byNowCard,-1,sizeof(m_byNowCard));		///<��ǰ����//ׯ�е��ƣ�0Ϊ�У�1Ϊׯ
	memset(m_iZCardInfo,0,sizeof(m_iZCardInfo));	///<ׯ������Ϣ��ׯ�ҵ�������1���Ƶĵ�����ǰ�����Ƶĵ���
	memset(m_iXCardInfo,0,sizeof(m_iXCardInfo));	///<ׯ������Ϣ��ׯ�ҵ�������1���Ƶĵ�����ǰ�����Ƶĵ���
	memset(m_iWinQuYu,0,sizeof(m_iWinQuYu));		///<��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���

}
//---------------------------------------------------------------------------------------
///��ʼ����������
void	CNewGameClient::ResetGameData()
{
	m_bIsPlaying = false;
	m_i64MyBetCount = 0;			//�Լ���ע����
	m_i64ZhuangScore= 0;			//ׯ�ұ��ֵ÷�
	m_i64MeScore	= 0;			//��ұ��ֵĵ÷�
	m_i64MeFanFen	= 0;			//�����Լ��ķ�������
	m_iWinShowIndex	= 0;			//�н�������˸��ʶ
	m_bySelBetAres	= 255;			
	m_bySelChouMaType = 255;

	ZeroMemory(m_i64MaxZhu,sizeof(m_i64MaxZhu));	//ÿ���������µ����ע	
	memset(m_byNowCard,-1,sizeof(m_byNowCard));		///<��ǰ����//ׯ�е��ƣ�0Ϊ�У�1ׯ
	memset(m_iZCardInfo,0,sizeof(m_iZCardInfo));	///<ׯ������Ϣ��ׯ�ҵ�������1���Ƶĵ�����ǰ�����Ƶĵ���
	memset(m_iXCardInfo,0,sizeof(m_iXCardInfo));	///<ׯ������Ϣ��ׯ�ҵ�������1���Ƶĵ�����ǰ�����Ƶĵ���
	memset(m_iWinQuYu,0,sizeof(m_iWinQuYu));		///<��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���

	
}

//---------------------------------------------------------------------------------------
///ɾ�����ж�ʱ��
void CNewGameClient:: KillAllGameTime() 
{
	
}
//-----------------------------------------------------------------------------------------------
/// ������Ϸ�ĵ�ǰ״̬
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 
	m_bWatchMode = m_pGameFrame->GetIsWatching() ;
	
	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_AGREE:
	case GS_WAIT_NEXT:			//�ȴ����̿�ʼ
		{
			if (nLen != sizeof(GameStationBase))
			{
				return;
			}
			GameStationBase * pGameStation=(GameStationBase *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			//��Ϸ״̬
			m_byGameStation	= GS_WAIT_AGREE;

			m_iXiaZhuTime	= pGameStation->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime	= pGameStation->m_iKaiPaiTime;      //����ʱ��
			m_iShowResult	= pGameStation->m_iShowResult;		
			m_iFreeTime		= pGameStation->m_iFreeTime;		//����ʱ��

			m_iNTju			= pGameStation->m_iZhuangBaShu;
			m_iNowNTstation = pGameStation->m_iNowNtStation;
			m_i64NtWinMoney	= pGameStation->i64NtWin;
			m_i64MyWinMoney = pGameStation->i64MeWin;

			m_iLimitMoney	= pGameStation->m_iShangZhuangLimit;//��ׯ�޶�
			m_iMaxNote		= pGameStation->iMaxNote;	

			///ׯ���б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_iZhuangList[i] = pGameStation->iZhuangWaitList[i];
			}
			//·��
			m_DqLuziData.clear();
			LuziData TLuziTmp;
			for(int i=0;i<MAXCOUNT;i++)
			{
				if (255 != pGameStation->TLuziData[i].byXpoint && 255 != pGameStation->TLuziData[i].byZPoint)
				{
					memcpy(&TLuziTmp,&pGameStation->TLuziData[i],sizeof(TLuziTmp));
					m_DqLuziData.push_back(TLuziTmp);
				}
			}
			
			//������ʾ��Ϣ
			char szMessage[MAX_PATH] = {0};
			CString strMixMoney;
			strMixMoney.Format("%I64d",m_iLimitMoney);
			_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ׯ������%s"),strMixMoney);
			//m_pGameFrame->InsertNormalMessage(szMessage);

			if (m_iMaxNote > 0)
			{
				CString strBaseMaxNote;
				
				strBaseMaxNote.Format("%I64d",m_iMaxNote);
				_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ͨ��������ע��%s"), strBaseMaxNote); 

				m_pGameFrame->InsertNormalMessage(szMessage);
			}
	
			break;
		}
	case GS_NOTE_STATE:	//��ע״̬
		{
			if (nLen != sizeof(GameStation_Bet))
			{
				return;
			}
			GameStation_Bet * pGameStation=(GameStation_Bet *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			//��Ϸ״̬
			m_byGameStation	= GS_NOTE_STATE;

			m_iXiaZhuTime	= pGameStation->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime	= pGameStation->m_iKaiPaiTime;      //����ʱ��
			m_iShowResult	= pGameStation->m_iShowResult;		
			m_iFreeTime		= pGameStation->m_iFreeTime;		//����ʱ��

			m_iNTju			= pGameStation->m_iZhuangBaShu;
			m_iNowNTstation = pGameStation->m_iNowNtStation;

			m_i64NtWinMoney	= pGameStation->i64NtWin;
			m_i64MyWinMoney = pGameStation->i64MeWin;

			m_iLimitMoney	= pGameStation->m_iShangZhuangLimit;//��ׯ�޶�
			m_iMaxNote		= pGameStation->iMaxNote;	


			///ׯ���б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_iZhuangList[i] = pGameStation->iZhuangWaitList[i];
			}
			//·��
			m_DqLuziData.clear();
			LuziData TLuziTmp;
			for(int i=0;i<MAXCOUNT;i++)
			{
				if (255 != pGameStation->TLuziData[i].byXpoint && 255 != pGameStation->TLuziData[i].byZPoint)
				{
					memcpy(&TLuziTmp,&pGameStation->TLuziData[i],sizeof(TLuziTmp));
					m_DqLuziData.push_back(TLuziTmp);
				}
			}
			//����ע���
			for(int i=0;i<BET_ARES;i++)
			{
				m_i64MaxZhu[i] = pGameStation->i64AresMaxZhu[i];
			}
			//��ʾ���Ͻ��������ע��Ϣ
			ShowCanBetInfo(true);
			//��ʾ��ע�������Ϣ
			for(int i=0;i<BET_ARES;i++)
			{
				//���������ע����
				SetShCtrlInfo(CTN_100900_ShCtrl_1009011+i,pGameStation->i64XiaZhuData[i],true);
				if (pGameStation->i64XiaZhuData[i]>0)
				{
					//��ʾ���������ע
					SetTextinfo(CTN_100900_TEXT_1009051+i,(pGameStation->i64XiaZhuData[i]),true);
					//SetInumInfo(CTN_100900_Num_1009021+i,pGameStation->i64XiaZhuData[i],true);
				}
				//�Լ��ĸ����������ע
				if (pGameStation->i64UserXiaZhuData[i]>0)
				{
					SetTextinfo(CTN_100900_TEXT_1009031+i,(pGameStation->i64UserXiaZhuData[i]),true);

				}
			}
			m_i64MyBetCount = pGameStation->i64UserXiaZhuSum;
			//��ʾ��ע
			SetTextinfo(CTN_100900_TEXT_10090003,(pGameStation->i64XiaZhuSum),true);
			//�Լ�������ע
			SetTextinfo(CTN_100900_TEXT_10090043,(pGameStation->i64UserXiaZhuSum),true);
			//��ʾ��ʱ��
			ShowOrHideTimer(true,m_iXiaZhuTime);
			break;
		}
	case GS_SEND_CARD:		//����״̬
		{
			if (nLen != sizeof(GameStation_SendCard))
			{
				return;
			}
			GameStation_SendCard * pGameStation=(GameStation_SendCard *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			//��Ϸ״̬
			m_byGameStation	= GS_SEND_CARD;

			m_iXiaZhuTime	= pGameStation->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime	= pGameStation->m_iKaiPaiTime;      //����ʱ��
			m_iShowResult	= pGameStation->m_iShowResult;		
			m_iFreeTime		= pGameStation->m_iFreeTime;		//����ʱ��

			m_iNTju			= pGameStation->m_iZhuangBaShu;
			m_iNowNTstation = pGameStation->m_iNowNtStation;

			m_i64NtWinMoney	= pGameStation->i64NtWin;
			m_i64MyWinMoney = pGameStation->i64MeWin;

			m_iLimitMoney	= pGameStation->m_iShangZhuangLimit;//��ׯ�޶�
			m_iMaxNote		= pGameStation->iMaxNote;	

			///ׯ���б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_iZhuangList[i] = pGameStation->iZhuangWaitList[i];
			}
			//·��
			m_DqLuziData.clear();
			LuziData TLuziTmp;
			for(int i=0;i<MAXCOUNT;i++)
			{
				if (255 != pGameStation->TLuziData[i].byXpoint && 255 != pGameStation->TLuziData[i].byZPoint)
				{
					memcpy(&TLuziTmp,&pGameStation->TLuziData[i],sizeof(TLuziTmp));
					m_DqLuziData.push_back(TLuziTmp);
				}
			}
			//����ע���
			for(int i=0;i<BET_ARES;i++)
			{
				m_i64MaxZhu[i] = pGameStation->i64AresMaxZhu[i];
			}
			//��ʾ���Ͻ��������ע��Ϣ
			ShowCanBetInfo(true);
			//��ʾ��ע�������Ϣ
			for(int i=0;i<BET_ARES;i++)
			{
				//���������ע����
				SetShCtrlInfo(CTN_100900_ShCtrl_1009011+i,pGameStation->i64XiaZhuData[i],true);
				if (pGameStation->i64XiaZhuData[i]>0)
				{
					//��ʾ���������ע
					SetTextinfo(CTN_100900_TEXT_1009051+i,(pGameStation->i64XiaZhuData[i]),true);
					//SetInumInfo(CTN_100900_Num_1009021+i,pGameStation->i64XiaZhuData[i],true);
				}
				//�Լ��ĸ����������ע
				if (pGameStation->i64UserXiaZhuData[i]>0)
				{
					SetTextinfo(CTN_100900_TEXT_1009031+i,(pGameStation->i64UserXiaZhuData[i]),true);
				}
			}
			m_i64MyBetCount = pGameStation->i64UserXiaZhuSum;
			//��ʾ��ע
			SetTextinfo(CTN_100900_TEXT_10090003,(pGameStation->i64XiaZhuSum),true);
			//�Լ�������ע
			SetTextinfo(CTN_100900_TEXT_10090043,(pGameStation->i64UserXiaZhuSum),true);
	

			//������
			for(int i=0; i<2; i++)
			{
				for(int j=0; j<3; j++)
				{
					m_byNowCard[i][j] = pGameStation->byUserCard[i][j];
				}
			}

			int iXcount = 0;
			int iZcount = 0;
			for(int i=0; i<3; i++)
			{
				if(m_byNowCard[0][i]!=255)
				{
					iXcount++;
				}

				if(m_byNowCard[1][i]!=255)
				{
					iZcount++;
				}
			}


			//���Ʊ�������
			SetContainPosition(CTN_100300,m_iSendCardBGPoint[0],0,true);

			//��ʾ��������
			SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030001,m_byNowCard[0],iXcount,true);
			//��ʾ�����
			SetImageXYInfo(CTN_100300_IMG_10030021,pGameStation->byXPaiXing*97,true);

			//��ʾ��������
			SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030002,m_byNowCard[1],iZcount,true);
			//��ʾ������
			SetImageXYInfo(CTN_100300_IMG_10030022,pGameStation->byZPaiXing*97,true);


			//��ȡӮ������
			for(int i=0;i<BET_ARES;i++)
			{
				m_iWinQuYu[i] = pGameStation->byWinAreas[i];			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			}
			//������˸�н������ʱ��
			m_iWinShowIndex = 0;
			m_pGameFrame->SetTimer(TIME_SHOW_WIN_ARES,500);

			//��ʾ��ʱ��
			ShowOrHideTimer(true,m_iKaiPaiTime);
			break;
		}
	case GS_OPEN_RESULT:	//��Ϸ����״̬
		{
			if (nLen != sizeof(GameStation_Result))
			{
				return;
			}
			GameStation_Result * pGameStation=(GameStation_Result *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			//��Ϸ״̬
			m_byGameStation	= GS_OPEN_RESULT;

			m_iXiaZhuTime	= pGameStation->m_iXiaZhuTime;		//��עʱ��			
			m_iKaiPaiTime	= pGameStation->m_iKaiPaiTime;      //����ʱ��
			m_iShowResult	= pGameStation->m_iShowResult;		
			m_iFreeTime		= pGameStation->m_iFreeTime;		//����ʱ��

			m_iNTju			= pGameStation->m_iZhuangBaShu;
			m_iNowNTstation = pGameStation->m_iNowNtStation;

			m_i64NtWinMoney	= pGameStation->i64NtWin;
			m_i64MyWinMoney = pGameStation->i64MeWin;

			m_iLimitMoney	= pGameStation->m_iShangZhuangLimit;//��ׯ�޶�
			m_iMaxNote		= pGameStation->iMaxNote;	


			///ׯ���б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				m_iZhuangList[i] = pGameStation->iZhuangWaitList[i];
			}
			//·��
			m_DqLuziData.clear();
			LuziData TLuziTmp;
			for(int i=0;i<MAXCOUNT;i++)
			{
				if (255 != pGameStation->TLuziData[i].byXpoint && 255 != pGameStation->TLuziData[i].byZPoint)
				{
					memcpy(&TLuziTmp,&pGameStation->TLuziData[i],sizeof(TLuziTmp));
					m_DqLuziData.push_back(TLuziTmp);
				}
			}
			//����ע���
			for(int i=0;i<BET_ARES;i++)
			{
				m_i64MaxZhu[i] = pGameStation->i64AresMaxZhu[i];
			}
			//��ʾ���Ͻ��������ע��Ϣ
			ShowCanBetInfo(true);
			//��ʾ��ע�������Ϣ
			for(int i=0;i<BET_ARES;i++)
			{
				//���������ע����
				SetShCtrlInfo(CTN_100900_ShCtrl_1009011+i,pGameStation->i64XiaZhuData[i],true);
				if (pGameStation->i64XiaZhuData[i]>0)
				{
					//��ʾ���������ע
					SetTextinfo(CTN_100900_TEXT_1009051+i,(pGameStation->i64XiaZhuData[i]),true);
					//SetInumInfo(CTN_100900_Num_1009021+i,pGameStation->i64XiaZhuData[i],true);
				}
				//�Լ��ĸ����������ע
				if (pGameStation->i64UserXiaZhuData[i]>0)
				{
					SetTextinfo(CTN_100900_TEXT_1009031+i,(pGameStation->i64UserXiaZhuData[i]),true);
				}
			}
			m_i64MyBetCount = pGameStation->i64UserXiaZhuSum;
			//��ʾ��ע
			SetTextinfo(CTN_100900_TEXT_10090003,(pGameStation->i64XiaZhuSum),true);
			//�Լ�������ע
			SetTextinfo(CTN_100900_TEXT_10090043,(pGameStation->i64UserXiaZhuSum),true);
			


			//��������
			m_i64ZhuangScore= pGameStation->i64ZhuangScore;		//ׯ�ұ��ֵ÷�
			m_i64MeScore	= pGameStation->i64MeScore;			//��ұ��ֵĵ÷�
			m_i64MeFanFen	= pGameStation->i64MeFanFen;		//�����Լ��ķ�������
			//��ʾ�����
			//��ʾ������Ϣ
			ShowGameResult(true);
			//��ʾ��ʱ��
			ShowOrHideTimer(true,m_iShowResult);
			break;
		}

	default:
		{
			break;
		}
	}

	//��ʾׯ����Ϣ
	ShowNtInfo(true);
	//��ʾ�Լ�����Ϣ
	ShowMyInfo(true);
	//��ʾ·�ӵ���
	ShowLuziPoint(true);
	//��ʾ·������
	ShowLuziAres(true);
	//��ʾ��ׯ�б�
	ShowWaitList(true);
	
	//������ʾ��Ϣ
	char szMessage[MAX_PATH] = {0};
	CString strMixMoney;
	strMixMoney.Format("%I64d",m_iLimitMoney);
	_stprintf_s(szMessage, sizeof(szMessage), TEXT("����ʾ����ׯ������%s"),strMixMoney);
	m_pGameFrame->InsertNormalMessage(szMessage);

	
	
	//���ű�����Ч
	PlayBgSound(true);
	return  ; 
}
//-----------------------------------------------------------------------------------------
/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 
	switch(nSubId)
	{
	case ASS_GM_GAME_STATION:   ///<����״̬
		{
			SetGameStation(buffer, nLen);    
			break;        
		}
	case ASS_BJL_BEGIN://�ͻ����յ��ټ��ֿ�ʼ��Ϣ
		{
			if (sizeof(BEGIN_DATA) != nLen)
			{
				return false;
			}
			BEGIN_DATA *pGameBegin = (BEGIN_DATA *)buffer;
			if (NULL == pGameBegin)
			{
				return false;
			}

			HandleGameBegin(pGameBegin);
			return true;
		}
	case ASS_KAI_PAI:			// ������Ϣ
		{
			if (sizeof(KAI_PAI) != nLen)
			{
				return false;
			}
			KAI_PAI *pKaipai = (KAI_PAI *)buffer;
			if (NULL == pKaipai)
			{
				return false;
			}
			OnHandleKaiPai(pKaipai);
			return true;
		}	
	case ASS_XIA_ZHU:		//�յ���ע��Ϣ
		{
			if (sizeof(XIA_ZHU) != nLen)
			{
				return false;
			}
			XIA_ZHU *pXiaZhu = (XIA_ZHU *)buffer;
			if (NULL == pXiaZhu)
			{
				return false;
			}

			OnHandleXiaZhu(pXiaZhu);
			break;
		}
	case ASS_SHOW_JS:		//��ʾ�����
		{
			if (sizeof(JIE_SUAN) != nLen)
			{
				return false;
			}

			JIE_SUAN *pJieSuan = (JIE_SUAN *)buffer;
			if (NULL == pJieSuan)
			{
				return false;
			}
			HandleGameResult(pJieSuan);
			break;
		}
	case ASS_FREE_STATE:	//��Ϸ���� ���н׶�
		{
			if (sizeof(DENG_DAI) != nLen)
			{
				return false;
			}
			DENG_DAI *pDengDai = (DENG_DAI *)buffer;
			if (NULL == pDengDai)
			{
				return false;
			}

			HandleGameFree(pDengDai);
			break;
		}
	case ASS_WU_ZHUANG:	//��ׯ��Ϣ֪ͨ
		{
			if (sizeof(tagNoZhuang) != nLen)
			{
				return false;
			}
			tagNoZhuang *pNoZhuang = (tagNoZhuang *)buffer;
			if (NULL == pNoZhuang)
			{
				return false;
			}
			m_iNowNTstation = pNoZhuang->iNtStation;
			m_i64NtWinMoney = pNoZhuang->i64NtWin;
			m_iNTju			= pNoZhuang->iZhuangBaShu;
			//��ׯ�Ͳ���ʾׯ����
			ShowNtInfo(true);
			//��ʾ��ǰ��ׯ
			SetImageVisible(CTN_100100_IMG_1001004,true);
			break;
		}
	case ASS_SHANG_ZHUANG:	//��ׯ�ɹ�
		{

			if (sizeof(SHANG_ZHUANG) != nLen)
			{
				return false;
			}
			SHANG_ZHUANG *pShangZhuang = (SHANG_ZHUANG *)buffer;
			if (NULL == pShangZhuang)
			{
				return false;
			}

			HandleApplyResult(pShangZhuang);
			break;
		}
	case ASS_SHANG_ZHUANG_ERR:		//��ׯʧ��
		{
			if (sizeof(SHANG_ZHUANG) != nLen)
			{
				return false;
			}
			SHANG_ZHUANG *pShangZhuangLost = (SHANG_ZHUANG *)buffer;
			if (NULL == pShangZhuangLost)
			{
				return false;
			}
			CString sMsg;
			if (1 == pShangZhuangLost->success)
			{
				sMsg.Format("��ׯʧ��:����Ҳ�������������ׯ����");
			}
			else if (2 == pShangZhuangLost->success)
			{
				sMsg.Format("��ׯʧ��:���Ѿ���ׯ�һ����Ѿ�����ׯ�б��У�");
			}
			else
			{
				sMsg.Format("��ׯʧ��:δ֪����");
			}
			m_pGameFrame->InsertNormalMessage(sMsg.GetBuffer());
			break;
		}
	case ASS_SUPER_STATE:	//�Ƿ񳬶�
		{
			if (sizeof(SUPERSTATE) != nLen)
			{
				return false;
			}
			SUPERSTATE *pSuperState = (SUPERSTATE *)buffer;
			if (NULL == pSuperState)
			{
				return false;
			}

			if (pSuperState->bDeskStation == m_iMyDeskStation && pSuperState->bEnable)
			{
				m_bIsSuper = true;
				SetBtVisible(BTN_1000010,true);
				SetBtEnable(BTN_1000010,false);
			}
			break;
		}
	case ASS_SUPER_SETTING:	//�������ó¹�
		{
			if (sizeof(SUPERSETTING) != nLen)
			{
				return false;
			}
			SUPERSETTING *pSuperSetResult = (SUPERSETTING *)buffer;
			if (NULL == pSuperSetResult)
			{
				return false;
			}
			if (pSuperSetResult->bDeskStation == m_iMyDeskStation)
			{
				CString sText;
				switch(pSuperSetResult->bState)
				{
				case 1:
					{
						sText.Format("���óɹ������ֿ�ׯ");
						break;
					}
				case 2:
					{
						sText.Format("���óɹ������ֿ���");
						break;
					}
				case 3:
					{
						sText.Format("���óɹ������ֿ���");
						break;
					}
				default:
					{
						sText.Format("�����������ڷ���ǰ����");
						break;
					}
				}

				SetTextinfo(CTN_101200_TEXT_1012003,sText,true);
			}
			break;
		}
	default:
		{
			break;
		}
		
	}
	return 0;
}
//------------------------------------------------------------------------------
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	///��ť�������Ϣ 
	if(pMessage->_uMessageType == UI_LBUTTONUP)
	{
		if (m_bWatchMode)
		{
			CString strInfo;
			strInfo.Format("�����Թ���ң��벻Ҫ�ҵ��") ;
			m_pGameFrame->InsertNormalMessage(strInfo.GetBuffer()) ; 
			return 0;
			
		}
		switch(pMessage->_uControlID)
		{
		case CTN_100400_BTN_10040001:	//������ׯ
			{
				UserApplyNtStation();			
				break;
			}
		case CTN_100400_BTN_10040002:	//������ׯ
			{
				UserApplyLiveNt();
				break;
			}
		case CTN_100200_BTN_10020001://�����ע��ť
		case CTN_100200_BTN_10020002:
		case CTN_100200_BTN_10020003:
		case CTN_100200_BTN_10020004:
		case CTN_100200_BTN_10020005:
		case CTN_100200_BTN_10020006:
			{
				//��ǰѡ��ĳ�������ֵ
				m_bySelChouMaType = pMessage->_uControlID - CTN_100200_BTN_10020001;
				CString sImageSrc;
				sImageSrc.Format(".\\resources\\Chouma\\chouma_%d.png",m_bySelChouMaType);
				m_pUI->GetICursor()->SetCursorImage(CA2W(sImageSrc));
				m_pUI->GetICursor()->SetSrcW(m_pUI->GetICursor()->GetCursorImageWidth()/4);
				m_pUI->GetICursor()->SetPointMode(1);
				m_pUI->GetICursor()->SetControlVisible(true);
				break;
			}
		case CTN_1000000_CTN_100700_IMG_10070001:	//��ע����
		case CTN_1000000_CTN_100700_IMG_10070002:
		case CTN_1000000_CTN_100700_IMG_10070003:
		case CTN_1000000_CTN_100700_IMG_10070004:
		case CTN_1000000_CTN_100700_IMG_10070005:
		case CTN_1000000_CTN_100700_IMG_10070006:
		case CTN_1000000_CTN_100700_IMG_10070007:
		case CTN_1000000_CTN_100700_IMG_10070008:
		case CTN_1000000_CTN_100700_IMG_10070009:
		case CTN_1000000_CTN_100700_IMG_10070010:
		case CTN_1000000_CTN_100700_IMG_10070011:
		case CTN_1000000_CTN_100700_IMG_10070012:
			{
				//��ע
				if (m_bySelChouMaType != 255 && 255 != m_bySelBetAres)
				{
					UserBet();
				}
				break;
			}
		case CTN_100600_BTN_10060001:	//·����
			{
				m_iLuziIndex--;
				ShowLuziAres(true);
				break;
			}
		case CTN_100600_BTN_10060002:	//·����
			{
				m_iLuziIndex++;
				ShowLuziAres(true);
				break;
			}
		case CTN_101200_RButton_10120001:   ///<ׯ  ��ѡ�� 
			{
				if (GetRadioBtIsSel(CTN_101200_RButton_10120001))
				{
					SetRadioButtonSeled(CTN_101200_RButton_10120001,true);
					SetRadioButtonSeled(CTN_101200_RButton_10120004,false);
					SetRadioButtonSeled(CTN_101200_RButton_10120007,false);
					//SetRadioButtonSeled(CTN_101200_RButton_10120008,false);
				}
				
				break;
			}
		case CTN_101200_RButton_10120004:   ///<��  ��ѡ�� 
			{
				if (GetRadioBtIsSel(CTN_101200_RButton_10120004))
				{
					SetRadioButtonSeled(CTN_101200_RButton_10120001,false);
					SetRadioButtonSeled(CTN_101200_RButton_10120004,true);
					SetRadioButtonSeled(CTN_101200_RButton_10120007,false);
					//SetRadioButtonSeled(CTN_101200_RButton_10120008,false);
				}
				break;
			}
		case CTN_101200_RButton_10120007:	///<��  ��ѡ�� 
			{
				if (GetRadioBtIsSel(CTN_101200_RButton_10120007))
				{
					SetRadioButtonSeled(CTN_101200_RButton_10120001,false);
					SetRadioButtonSeled(CTN_101200_RButton_10120004,false);
					SetRadioButtonSeled(CTN_101200_RButton_10120007,true);
					//SetRadioButtonSeled(CTN_101200_RButton_10120008,false);
				}
				break;
			}
		case CTN_101200_RButton_10120008:   ///<ͬ���  ��ѡ�� 
			{
				if (GetRadioBtIsSel(CTN_101200_RButton_10120008))
				{
					SetRadioButtonSeled(CTN_101200_RButton_10120001,false);
					SetRadioButtonSeled(CTN_101200_RButton_10120004,false);
					SetRadioButtonSeled(CTN_101200_RButton_10120007,true);
					//SetRadioButtonSeled(CTN_101200_RButton_10120008,true);
				}
				break;
			}
		case CTN_101200_RButton_10120002:   ///<ׯ����  ��ѡ�� 
		case CTN_101200_RButton_10120005:   ///<������  ��ѡ�� 
		case CTN_101200_RButton_10120003:   ///<ׯ��  ��ѡ�� 
		case CTN_101200_RButton_10120006:   ///<�ж�  ��ѡ�� 
			{
				break;
			}
		case BTN_1000010:	//���˰�ť
			{
				ShowSuperResult(m_bIsSuper);
				break;
			}
		case CTN_101200_BTN_1012001:	//ȷ��
			{
				if (GetRadioBtIsSel(CTN_101200_RButton_10120001)== false && false == GetRadioBtIsSel(CTN_101200_RButton_10120004) && false == GetRadioBtIsSel(CTN_101200_RButton_10120007))
				{
					break;
				}
				else
				{
					//���ͳ�������
					SUPERSETTING TSuperSet;
					TSuperSet.bDeskStation= m_iMyDeskStation;
					if (GetRadioBtIsSel(CTN_101200_RButton_10120001))
					{
						TSuperSet.bState = 1;
					}
					else if(GetRadioBtIsSel(CTN_101200_RButton_10120004))
					{
						TSuperSet.bState = 2;
					}
					else if (GetRadioBtIsSel(CTN_101200_RButton_10120007))
					{
						TSuperSet.bState = 3;
					}

					m_pGameFrame->SendGameData(&TSuperSet,sizeof(TSuperSet),MDM_GM_GAME_NOTIFY,ASS_SUPER_SETTING,0);

				}
				break;
			}
		case CTN_101200_BTN_1012002:	//ȡ��
			{
				ShowSuperResult(false);
				break;
			}
		
		default:
			{
				break;
			}
		}
	
		
	
		return 0;
	}
	//�һ�
	if(pMessage->_uMessageType == UI_RBUTTONDOWN)
	{
		if (m_pUI->GetICursor()->GetControlVisible())
		{
			m_pUI->GetICursor()->SetControlVisible(false);
		}
		
	}
	//���̰���
	if (pMessage->_uMessageType == UI_KEYDOWN)
	{
		//����F12 ��������
		//if (pMessage->_ch == VK_F12 && m_bIsSuperUser && m_byGameStation == GS_BET_STATUS)
		//{
		//	//��ʾ���˽������
		//	ShowSuperResult(true);
		//}
	}
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		switch(pMessage->_uControlID)
		{
		case  CTN_100300_MoveAction_10030011:	//�мҷ���һ�������
			{
				BYTE byTmpCard = m_byNowCard[1][0];
				POINT pPT;
				pPT.x = 570;
				pPT.y = 60;
				SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,false);
				//��ʾ��
				SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030001,m_byNowCard[0],1,true);

				if (0 != m_byNowCard[1][0])
				{
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,true);

					POINT pStart, pEnd;
					pStart.x = 605;
					pStart.y = 105;
					pEnd.x = 485;
					pEnd.y = 220;
					SetMoveActionPlay(CTN_100300_MoveAction_10030012,CTN_100300_NoMoveCard_10030003,pStart,pEnd,1000,true);
				}

				break;
			}
			case  CTN_100300_MoveAction_10030012:	//ׯ�ҷ���1�������
				{
					BYTE byTmpCard = m_byNowCard[0][1];
					POINT pPT;
					pPT.x = 570;
					pPT.y = 60;
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,false);
					//��ʾ��
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030002,m_byNowCard[1],1,true);

					if (0 != m_byNowCard[0][1])
					{
						SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,true);

						POINT pStart, pEnd;
						pStart.x = 605;
						pStart.y = 105;
						pEnd.x = 325;
						pEnd.y = 220;
						SetMoveActionPlay(CTN_100300_MoveAction_10030013,CTN_100300_NoMoveCard_10030003,pStart,pEnd,1000,true);
					}
					break;
				}
			case  CTN_100300_MoveAction_10030013:	//�мҷ���2�������
				{
					BYTE byTmpCard = m_byNowCard[1][1];
					POINT pPT;
					pPT.x = 570;
					pPT.y = 60;
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,false);
					//��ʾ��
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030001,m_byNowCard[0],2,true);
					//��ʾ����
					SetImageXYInfo(CTN_100300_IMG_10030021,m_iXCardInfo[0]*40,true);

					if (0 != m_byNowCard[1][1])
					{
						SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,true);

						POINT pStart, pEnd;
						pStart.x = 605;
						pStart.y = 105;
						pEnd.x = 485;
						pEnd.y = 220;
						SetMoveActionPlay(CTN_100300_MoveAction_10030014,CTN_100300_NoMoveCard_10030003,pStart,pEnd,1000,true);
					}
					break;
				}
			case  CTN_100300_MoveAction_10030014:	//ׯ�ҷ���2�������
				{
					BYTE byTmpCard = 0;
					POINT pPT;
					pPT.x = 570;
					pPT.y = 60;
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,false);
					//��ʾ��
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030002,m_byNowCard[1],2,true);
					//��ʾ�����Ƶĵ���
					SetImageXYInfo(CTN_100300_IMG_10030022,m_iZCardInfo[0]*40,true);
					if (0 != m_byNowCard[0][2] && 255 != m_byNowCard[0][2])	//�мҵ�������
					{
						byTmpCard = m_byNowCard[0][2];
						SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,true);

						POINT pStart, pEnd;
						pStart.x = 605;
						pStart.y = 105;
						pEnd.x = 325;
						pEnd.y = 220;
						SetMoveActionPlay(CTN_100300_MoveAction_10030015,CTN_100300_NoMoveCard_10030003,pStart,pEnd,1000,true);
					}
					else if (0 != m_byNowCard[1][2] && 255 != m_byNowCard[1][2])	//ׯ�ҵ�������
					{
						byTmpCard = m_byNowCard[1][2];
						SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,true);

						POINT pStart, pEnd;
						pStart.x = 605;
						pStart.y = 105;
						pEnd.x = 485;
						pEnd.y = 220;
						SetMoveActionPlay(CTN_100300_MoveAction_10030016,CTN_100300_NoMoveCard_10030003,pStart,pEnd,1000,true);
					}
					else
					{
						//���е��Ʒ����� �Ϳ�ʼ��˸�н����� dwjtest
						m_iWinShowIndex = 0;
						m_pGameFrame->SetTimer(TIME_SHOW_WIN_ARES,1000);
					}
					break;
				}
			case  CTN_100300_MoveAction_10030015:	//�мҵ����������
				{
					BYTE byTmpCard = 0;
					POINT pPT;
					pPT.x = 570;
					pPT.y = 60;
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,false);
					//��ʾ��
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030001,m_byNowCard[0],3,true);
					//��ʾ����
					SetImageXYInfo(CTN_100300_IMG_10030021,m_iXCardInfo[1]*40,true);

					//�ж�ׯ���Ƿ��е�������
					if (0 != m_byNowCard[1][2] && 255 != m_byNowCard[1][2])	//ׯ�ҵ�������
					{
						byTmpCard = m_byNowCard[1][2];
						SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,true);

						POINT pStart, pEnd;
						pStart.x = 605;
						pStart.y = 105;
						pEnd.x = 485;
						pEnd.y = 220;
						SetMoveActionPlay(CTN_100300_MoveAction_10030016,CTN_100300_NoMoveCard_10030003,pStart,pEnd,1000,true);
					}
					else
					{
						//���е��Ʒ����� �Ϳ�ʼ��˸�н�����
						m_iWinShowIndex = 0;
						m_pGameFrame->SetTimer(TIME_SHOW_WIN_ARES,1000);
					}
					break;
				}
			case  CTN_100300_MoveAction_10030016:	//ׯ�ҵ����������
				{
					BYTE byTmpCard = 0;
					POINT pPT;
					pPT.x = 570;
					pPT.y = 60;
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,false);
					//��ʾ��
					SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030002,m_byNowCard[1],3,true);
					//��ʾ�����Ƶĵ���
					SetImageXYInfo(CTN_100300_IMG_10030022,m_iZCardInfo[1]*40,true);

					//���е��Ʒ����� �Ϳ�ʼ��˸�н����� 
					m_iWinShowIndex = 0;
					m_pGameFrame->SetTimer(TIME_SHOW_WIN_ARES,1000);

					break;
				}
		default:
			{
				break;
			}
		}
		return 0 ; 
	}
	if (pMessage->_uMessageType == UI_MOUSEMOVE)
	{
		if (255 != m_bySelChouMaType)
		{
			POINT ptMouse;
			ptMouse.x = pMessage->_mouseX;
			ptMouse.y = pMessage->_mouseY;
			for(int i=0; i<BET_ARES; i++)
			{
				if (m_rgnNoteRect[i].PtInRegion(ptMouse))
				{
					SetImageVisible(CTN_1000000_CTN_100700_IMG_10070001+i,true);
					m_bySelBetAres = i;
				}
				else
				{
					SetImageVisible(CTN_1000000_CTN_100700_IMG_10070001+i,false);
				}
			}
		}
	}
	if(pMessage->_uMessageType == UI_MOUSEENTER)
	{
		switch(pMessage->_uControlID)
		{
		//case CTN_1000000_CTN_1000400_BTN_1000401:	//������ׯ
		//case CTN_1000000_CTN_1000400_BTN_1000402:	//������ׯ
		//case CTN_1000000_CTN_1000100_BTN_1000101:	//����
		//case CTN_1000000_CTN_1000100_BTN_1000102:	//����
		//case CTN_1000000_CTN_1000100_BTN_1000103:	//����
		//case CTN_1000000_CTN_1000100_BTN_1000104:	//����
		//case CTN_1000000_CTN_1000100_BTN_1000105:	//����
		//case CTN_1000000_CTN_1000100_BTN_1000106:	//����
		//case CTN_1000000_BTN_1001400:	//ȡǮ
		//case CTN_1000000_BTN_1001300:	//��Ǯ
		//	{
		//		//������Ч
		//		PlayGameSound(SOUND_MOUSE_ENTER);
		//		break;
		//	}
		default:
			{
				break;
			}
		}
		
		return 0;
	}

	return 0;
}
//---------------------------------------------------------------------------------
//��ȡ�Ƶĵ���
int		CNewGameClient::GetCardNum(BYTE byCard)
{
	int iNum = (byCard&UG_VALUE_MASK)+1;
	if (iNum >=10)
	{
		iNum = 0;
	}
	return iNum; 
}
//---------------------------------------------------------------------------------
//������ׯ
void	CNewGameClient::UserApplyNtStation()
{
	SHANG_ZHUANG zhuang;
	zhuang.shang=true;
	// ��������ׯ��Ϣ
	m_pGameFrame->SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
}
//--------------------------------------------------------------------------------
//������ׯ
void	CNewGameClient::UserApplyLiveNt()
{
	SHANG_ZHUANG zhuang;
	zhuang.shang=false;
	// ��������ׯ��Ϣ
	m_pGameFrame->SendGameData(&zhuang,sizeof(zhuang),MDM_GM_GAME_NOTIFY,ASS_SHANG_ZHUANG,0);
}
//--------------------------------------------------------------------------------
//�����ע
void	CNewGameClient::UserBet()
{
	XIA_ZHU  xiazhu;
	xiazhu.moneytype = m_bySelChouMaType;
	xiazhu.type = m_bySelBetAres;
	m_pGameFrame->SendGameData(&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY,ASS_XIA_ZHU,0);
}

//--------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
//��ʾ���˽������
void	CNewGameClient::ShowSuperResult(bool bFlag)
{
	SetContainVisible(CTN_101200,bFlag);
	SetRadioVisible(CTN_101200_RButton_10120001,bFlag);
	SetRadioVisible(CTN_101200_RButton_10120004,bFlag);
	SetRadioVisible(CTN_101200_RButton_10120007,bFlag);
}
//-------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//�Ƿ�����ׯ�б���
bool	CNewGameClient::IsInWaitNtList()
{
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (m_iZhuangList[i] == m_iMyDeskStation)
		{
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------------
//��ȡ���ʣ��Ľ��
__int64		CNewGameClient::GetMyRemaindMoney()
{
	return 0;
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
	return 0;
}
//---------------------------------------------------------------------------------
/// ������Ϸ״̬
int CNewGameClient::ResetGameFrame(void)
{
	return 0;
}
/// ��ҽ���
int CNewGameClient::GameUserCome(void)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 
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
	//if (m_bIsPlaying)	//��Ϸ�Ƿ��Ѿ���ʼ
	//{
	//	return true;
	//}
	return false;
}
//--------------------------------------------------------------------
/// ��ʱ����Ϣ����Ϸ�еĶ�ʱ������
void	CNewGameClient::OnTimer(int nTimerID)
{
 	switch(nTimerID)
 	{
 	case  TIME_CARDBG_FALL:
 		{
			m_iSendCardBGPoint[1]	+= 10;
			if (m_iSendCardBGPoint[1]>=196)
			{
				m_iSendCardBGPoint[1] = 196;
				m_pGameFrame->KillTimer(TIME_CARDBG_FALL);

				//�������Ƽ�ʱ��
				m_pGameFrame->SetTimer(TIME_SEND_CARD,500);
			}
			SetContainPosition(CTN_100300,m_iSendCardBGPoint[0],m_iSendCardBGPoint[1],true);
 			break ; 
 		}
	case  TIME_CARDBG_UP:
		{
			m_iSendCardBGPoint[1]	-= 10;
			if (m_iSendCardBGPoint[1] <= 30)
			{
				m_iSendCardBGPoint[1] = 30;
				m_pGameFrame->KillTimer(TIME_CARDBG_UP);
				//����
				SetContainPosition(CTN_100300,m_iSendCardBGPoint[0],m_iSendCardBGPoint[1],false);
				//�������Ƽ�ʱ��
				m_pGameFrame->SetTimer(TIME_SHOW_RESULT,500);
			}
			else
			{
				SetContainPosition(CTN_100300,m_iSendCardBGPoint[0],m_iSendCardBGPoint[1],true);
			}
			break ; 
		}
	case TIME_SEND_CARD:
		{
			m_pGameFrame->KillTimer(TIME_SEND_CARD);
			if(m_byNowCard[0][0] != 0)	//�мҵ�һ����
			{
				BYTE byTmpCard = m_byNowCard[0][0];
				POINT pPT;
				pPT.x = 570;
				pPT.y = 60;
				SetNoMoveCardInfo(CTN_100300_NoMoveCard_10030003,&byTmpCard,1,pPT,true);
				POINT pStart, pEnd;
				pStart.x = 605;
				pStart.y = 105;
				pEnd.x = 325;
				pEnd.y = 220;
				SetMoveActionPlay(CTN_100300_MoveAction_10030011,CTN_100300_NoMoveCard_10030003,pStart,pEnd,1000,true);
			}
			break;
		}
	case TIME_SHOW_WIN_ARES:	//�н�������ʾ��˸
		{
			m_iWinShowIndex++;
			m_pGameFrame->KillTimer(TIME_SHOW_WIN_ARES);
			//��ʾ�н�����
			ShowWinAres(m_iWinShowIndex%2);
			m_pGameFrame->SetTimer(TIME_SHOW_WIN_ARES,500);
			break;
		}
	case TIME_SHOW_RESULT:	//��ʾ�����
		{
			m_pGameFrame->KillTimer(TIME_SHOW_RESULT);
			//��ʾ������Ϣ
			ShowGameResult(true);
			//����ׯ����Ϣ
			ShowNtInfo(true);
			//�����Լ�����Ϣ
			ShowMyInfo(true);
			//���Ͻ�
			ShowLuziPoint(true);
			//����·��
			ShowLuziAres(true);
			break;
		}
	
	default:
		{
			break;
		}
	}
	return ; 

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
//-------------------------------------------------------------------------------
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
 
 	if(m_bIsPlaying)
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
//---------------------------------------------------------------------------------------
//������Ϸ��Ч
void	CNewGameClient::PlayGameSound(int SoundID, bool bISLoop )
{
	if(!m_SoundEngineGame)
	{
		return ;
	}


	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	
	switch(SoundID)
	{
	case SOUND_GAME_BEGIN://��Ϸ��ʼ
		{
			wsprintf(szFileName,"music\\sound\\BeGin.ogg");
			break;
		}
	case SOUND_BEGIN_BET://��ע��ʼ����
		{
			wsprintf(szFileName,"music\\sound\\BetStart.ogg");
			break;
		}
	case SOUND_COUNTDWON://��ע����ʱ
		{
			wsprintf(szFileName,"music\\sound\\CountDwon.ogg");
			break;
		}
	case SOUND_MOUSE_ENTER://����ƶ�
		{
			wsprintf(szFileName,"music\\sound\\MouseMove.ogg");
			break;
		}
	case SOUND_WIN://Ӯ����Ч
		{
			wsprintf(szFileName,"music\\sound\\win.ogg");
			break;
		}
	case SOUN_LOST://������Ч
		{
			wsprintf(szFileName,"music\\sound\\lost.ogg");
			break;
		}
	case SOUND_BET://��ע��Ч
		{
			wsprintf(szFileName,"music\\sound\\Note.ogg");
			break;
		}
	default:
		{
			break;
		}
	}
	m_SoundEngineGame->setSoundVolume(1.0);
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}
//---------------------------------------------------------------------------------------
//���ű�����Ч
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	if(!m_SoundEnginBg)
	{
		return ;
	}
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	wsprintf(szFileName,"music\\sound\\BeiJing.ogg");
	m_SoundEnginBg->setSoundVolume(0.5);
	m_SoundEnginBg->play2D(szFileName,bISLoop);
}
//-------------------------------------------------------------------------
//��ʾ��ʾ������ׯ��Ϣ
void	CNewGameClient::ShowPromptDlg(bool bFlag)
{
	
}
//-------------------------------------------------------------------------
//��ʾ�Լ�����Ϣ
void	CNewGameClient::ShowMyInfo(bool bFalg)
{
	UserInfoForGame UserInfo ; 
	if (m_pGameFrame->GetUserInfo(m_iMyDeskStation ,UserInfo))
	{
		//�ǳ�
		CString sText;
		sText.Format(" %s",UserInfo.szNickName);
		SetTextinfo(CTN_100500_TEXT_10050011,sText,bFalg);
		__int64 i64MyRmaindMoney = 0;
		i64MyRmaindMoney = UserInfo.i64Money - m_i64MyBetCount;
		//�Լ��Ľ��
		sText.Format(" %I64d",(i64MyRmaindMoney));
		SetTextinfo(CTN_100500_TEXT_10050012,sText,bFalg);
		//ս��
		if (m_i64MyWinMoney > 0)
		{
			sText.Format(" +%I64d",(m_i64MyWinMoney));
		}
		else
		{
			sText.Format(" %I64d",(m_i64MyWinMoney));
		}
		SetTextinfo(CTN_100500_TEXT_10050013,sText,bFalg);
		//������
		if (m_iPlayCount > 0)
		{
			float fTmep = (((float)m_iWinCount)/((float)m_iPlayCount));
			fTmep *= 100; 
			sText.Format(" %.2f",fTmep);
			sText += "%";
			SetTextinfo(CTN_100500_TEXT_10050014,sText,bFalg);
		}
		else
		{
			SetTextinfo(CTN_100500_TEXT_10050014," 0.00%",bFalg);
		}
	}
}
//-------------------------------------------------------------------------
//��ʾ����ע����Ϣ
void		CNewGameClient::ShowCanBetInfo(bool bFalg)
{
	//������ע
	SetTextinfo(CTN_101000_TEXT_1010001,(m_i64MaxZhu[0]),bFalg);
	//�����ע
	SetTextinfo(CTN_101000_TEXT_1010002,(m_i64MaxZhu[3]),bFalg);
	//�Ϳ���ע
	SetTextinfo(CTN_101000_TEXT_1010003,(m_i64MaxZhu[6]),bFalg);
}

//-------------------------------------------------------------------------
//��ʾׯ����Ϣ
void	CNewGameClient::ShowNtInfo(bool bFalg)
{
	UserInfoForGame sUser;
	ZeroMemory(&sUser,sizeof(sUser));
	if (m_pGameFrame->GetUserInfo(m_iNowNTstation,sUser) && (-1 != m_iNowNTstation) )
	{
		//�ǳ�
		CString sText;
		sText.Format(" %s",sUser.szNickName);
		SetTextinfo(CTN_100500_TEXT_10050001,sText,bFalg);
		//���
		sText.Format(" %I64d",(sUser.i64Money));
		SetTextinfo(CTN_100500_TEXT_10050002,sText,bFalg);
		//�ɼ�
		if (m_i64NtWinMoney > 0)
		{
			sText.Format(" +%I64d",(m_i64NtWinMoney));
		}
		else
		{
			sText.Format(" %I64d",(m_i64NtWinMoney));
		}
		
		SetTextinfo(CTN_100500_TEXT_10050003,sText,bFalg);
		//ׯ�Ҿ���
		sText.Format(" %d",m_iNTju);
		SetTextinfo(CTN_100500_TEXT_10050004,sText,bFalg);
	}
	else
	{
		//�ǳ�
		CString sText;
		sText.Format("");
		SetTextinfo(CTN_100500_TEXT_10050001,sText,bFalg);
		//���
		sText.Format("");
		SetTextinfo(CTN_100500_TEXT_10050002,sText,bFalg);
		//�ɼ�
		sText.Format("");
		SetTextinfo(CTN_100500_TEXT_10050003,sText,bFalg);
		//ׯ�Ҿ���
		sText.Format("");
		SetTextinfo(CTN_100500_TEXT_10050004,sText,bFalg);
	}
}
//-------------------------------------------------------------------------
//��Ϸ��ʼ
void	CNewGameClient::HandleGameBegin(BEGIN_DATA *psGameBegin )
{
	if (psGameBegin == NULL)
	{
		return;
	}
	SetBtEnable(BTN_1000010,true);
	SetTextinfo(CTN_101200_TEXT_1012003,"�����������ڷ���ǰ����",false);
	ShowGameResult(false);
	m_byGameStation	= GS_SEND_CARD;
	m_bIsPlaying = true;

	m_iXiaZhuTime	= psGameBegin->m_iXiaZhuTime;		// ��עʱ��			
	m_iKaiPaiTime	= psGameBegin->m_iKaiPaiTime;       // ����ʱ��
	m_iShowResult	= psGameBegin->m_iShowResult;
	m_iFreeTime		= psGameBegin->m_iFreeTime;			// ����ʱ��
	m_iMaxNote		= psGameBegin->iMaxNote;			// ��ǰ����������µ���ע

	m_iNowNTstation = psGameBegin->m_iNowNtStation;		//��ȡׯ�ҵ�λ��
	m_i64NtWinMoney = psGameBegin->m_i64NtWin;			//ׯ�ҳɼ�
	m_iNTju			= psGameBegin->m_iZhuangBaShu;		//ׯ�ҵľ���
	m_iLimitMoney	= psGameBegin->m_iShangZhuangLimit;	//��ׯ�޶�

	//�˸��������ע��
	for(int i=0;i<BET_ARES;i++)
	{
		m_i64MaxZhu[i] = psGameBegin->m_iMaxZhu[i];
	}

	
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_iZhuangList[i] = psGameBegin->m_iNTlist[i];//��ȡׯ���б�����
	}

	//��ʾׯ����Ϣ
	ShowNtInfo(true);
	//��ʾ����ע����Ϣ
	ShowCanBetInfo(true);
	//��ע��ť�Ƿ����
	SetBetBtEnable(m_iMyDeskStation != m_iNowNTstation);
	//��ʾ��ׯ��ʾ
	SetImageVisible(CTN_100100_IMG_1001002,m_iMyDeskStation != m_iNowNTstation);
	SetImageVisible(CTN_100100_IMG_1001003,m_iMyDeskStation == m_iNowNTstation);
	SetImageVisible(CTN_100100_IMG_1001004,false);
	//��ׯ�б�
	ShowWaitList(true);
	//��������ʱ��ʱ��
	ShowOrHideTimer(true,m_iXiaZhuTime);
}
//-------------------------------------------------------------------------
//����
void		CNewGameClient::OnHandleKaiPai(KAI_PAI *pKaipai)
{
	if (NULL == pKaipai)
	{
		return;
	}

	//����������ע��ť
	SetBetBtEnable(false);
	SetBtEnable(BTN_1000010,false);
	//���س�������
	ShowSuperResult(false);
	//���״̬ 
	m_bySelBetAres = 255;
	m_bySelChouMaType = 255;
	if (m_pUI->GetICursor()->GetControlVisible())
	{
		m_pUI->GetICursor()->SetControlVisible(false);
	}
	ShowWinAres(false);

	
	//����������
	m_byNowCard[0][0] = pKaipai->pai[0][0];
	m_byNowCard[0][1] = pKaipai->pai[0][1];
	m_byNowCard[0][2] = pKaipai->pai[0][2];
	m_byNowCard[1][0] = pKaipai->pai[1][0];
	m_byNowCard[1][1] = pKaipai->pai[1][1];
	m_byNowCard[1][2] = pKaipai->pai[1][2];

	for(int i=0;i<5;i++)
	{
		m_iZCardInfo[i] = pKaipai->iZPaiXing[i];	//ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
		m_iXCardInfo[i] = pKaipai->iXPaiXing[i];	//�м�����,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	}

	//��ȡӮ������
	for(int i=0;i<BET_ARES;i++)
	{
		m_iWinQuYu[i] = pKaipai->iWinQuYu[i];			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	}

	
	//���Ʊ���������
	m_iSendCardBGPoint[0]	= 255;
	m_iSendCardBGPoint[1]	= 30;
	SetContainPosition(CTN_100300,m_iSendCardBGPoint[0],m_iSendCardBGPoint[1],true);
	m_pGameFrame->SetTimer(TIME_CARDBG_FALL,10);
	//��ʾ��ʱ��
	ShowOrHideTimer(true,m_iKaiPaiTime);

}
//-------------------------------------------------------------------------
//�յ���ע��Ϣ
void		CNewGameClient::OnHandleXiaZhu(XIA_ZHU *pXiaZhu)
{
	OutputDebugString("dwjbjl::OnHandleXiaZhu(0)");
	if (NULL == pXiaZhu)
	{
		return;
	}
	OutputDebugString("dwjbjl::OnHandleXiaZhu(1)");
	for(int i=0;i<BET_ARES;i++)
	{
		m_i64MaxZhu[i] = pXiaZhu->m_iMaxZhu[i];
	}
	//��ʾ���Ͻ��������ע��Ϣ
	ShowCanBetInfo(true);
	OutputDebugString("dwjbjl::OnHandleXiaZhu(2)");
	//��ʾ�������
	SetShCtrlInfo(CTN_100900_ShCtrl_1009011+pXiaZhu->type,pXiaZhu->money,true);

	//��ʾ��ע
	SetTextinfo(CTN_100900_TEXT_10090003,(pXiaZhu->iAllZhongZhu),true);
	//�Լ�������ע
	SetTextinfo(CTN_100900_TEXT_10090043,(pXiaZhu->iMeZhongZhu),true);


	m_i64MyBetCount = pXiaZhu->iMeZhongZhu;
	OutputDebugString("dwjbjl::OnHandleXiaZhu(3)");
	for(int i=0; i<BET_ARES; i++)
	{
		if (pXiaZhu->m_iQuYuZhu[i]>0)
		{
			//��ʾ���������ע
			SetTextinfo(CTN_100900_TEXT_1009051+i,(pXiaZhu->m_iQuYuZhu[i]),true);
			//SetInumInfo(CTN_100900_Num_1009021+i,pXiaZhu->m_iQuYuZhu[i],true);
		}
		
		//�����Լ��ĸ����������ע
		if (m_iMyDeskStation == pXiaZhu->station)
		{
			if (pXiaZhu->m_iUserXiaZhu[i]>0)
			{
				SetTextinfo(CTN_100900_TEXT_1009031+i,(pXiaZhu->m_iUserXiaZhu[i]),true);
			}
		}
	}
	OutputDebugString("dwjbjl::OnHandleXiaZhu(4)");
}
//-------------------------------------------------------------------------
//��Ϸ����
void		CNewGameClient::HandleGameResult(JIE_SUAN *pJieSuan)
{
	if (pJieSuan == NULL)
	{
		return;
	}

	m_i64MyBetCount = 0;
	
	m_i64NtWinMoney	= pJieSuan->i64NtWin;
	m_i64MyWinMoney = pJieSuan->i64UserWin;
	m_i64ZhuangScore= pJieSuan->i64ZhuangScore;		//ׯ�ұ��ֵ÷�
	m_i64MeScore	= pJieSuan->i64MeScore;			//��ұ��ֵĵ÷�
	m_i64MeFanFen	= pJieSuan->i64MeFanFen;		//�����Լ��ķ�������
	m_iPlayCount	= pJieSuan->iPlayCount;			//���˶�����
	m_iWinCount		= pJieSuan->iWinCount;			//Ӯ�˶�����

	//·��
	m_DqLuziData.clear();
	LuziData TLuziTmp;
	for(int i=0;i<MAXCOUNT;i++)
	{
		if (255 != pJieSuan->TLuziData[i].byXpoint && 255 != pJieSuan->TLuziData[i].byZPoint)
		{
			TLuziTmp = pJieSuan->TLuziData[i];
			m_DqLuziData.push_back(TLuziTmp);
		}
	}

	//ɱ����˸��ʱ��
	m_pGameFrame->KillTimer(TIME_SHOW_WIN_ARES);
	//���������ղ�����
	SetContainPosition(CTN_100300,m_iSendCardBGPoint[0],m_iSendCardBGPoint[1],true);
	m_pGameFrame->SetTimer(TIME_CARDBG_UP,10);

	//��ʾ��ʱ��
	ShowOrHideTimer(true,m_iShowResult);
}
//-------------------------------------------------------------------------
//������Ϣ
void		CNewGameClient::HandleGameFree(DENG_DAI *pDengDai)
{
	if (NULL == pDengDai)
	{
		return;
	}
	ShowGameResult(false);
	ResetGameData();
	m_bIsPlaying = false;
	//������װ�б�
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		m_iZhuangList[i] = pDengDai->zhuangstation[i];//��ȡׯ���б�����
	}
	m_iNowNTstation = pDengDai->iNtStation;		//��ȡׯ�ҵ�λ��
	m_i64NtWinMoney = pDengDai->i64NtWin;			//ׯ�ҳɼ�
	m_iNTju			= pDengDai->iZhuangBaShu;		//ׯ�ҵľ���
	//����ׯ����Ϣ
	ShowNtInfo(true);
	//������װ�б�
	ShowWaitList(true);
	//������Ϸ����
	ReSetUiDlag();
	//�������е���ʱ
	ShowOrHideTimer(true,m_iFreeTime);
}
//-------------------------------------------------------------------------
//������ׯ�����Ϣ
void	CNewGameClient::HandleApplyResult(SHANG_ZHUANG *pShangZhuang)
{
	if (pShangZhuang == NULL)
	{
		return;
	}

	//ׯ���б�����
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_iZhuangList[i] = pShangZhuang->zhuangstation[i];
	}
	//��ʾׯ���б�
	ShowWaitList(true);

	if (pShangZhuang->shang == false && pShangZhuang->station == m_iMyDeskStation && m_iNowNTstation == m_iMyDeskStation)
	{
		SetBtEnable(CTN_100400_BTN_10040002,false);
	}
}













//-------------------------------------------------------------------------
//������ע��ť����
void		CNewGameClient::SetBetBtEnable(bool bFalg)
{
	//��ע��ť������
	for (int i=0; i<6; i++)
	{
		SetBtEnable(CTN_100200_BTN_10020001+i,bFalg);
	}
}
//-------------------------------------------------------------------------
//������ʾ��ʱ��
void		CNewGameClient::ShowOrHideTimer(bool bShow,BYTE byMaxTime, BYTE byMinTime)
{
	SetContainVisible(CTN_100800,bShow);
	SetTimePlay(CTN_100800_Time_10080001,bShow,byMaxTime,byMinTime);
}
//-------------------------------------------------------------------------
//��ׯ�б�
void		CNewGameClient::ShowWaitList(bool bFalg)
{
	for(int i=0; i<5; i++)
	{
		SetTextinfo(CTN_100400_TEXT_10040021+i,"",false);
		SetTextinfo(CTN_100400_TEXT_10040031+i,"",false);
		SetTextinfo(CTN_100400_TEXT_10040041+i,"",false);
	}
	//����ׯ���б��ж��ٸ�m_iWaitIndex
	int iCount = m_iZhuangList[PLAY_COUNT];

	if (iCount <= 5)
	{
		SetBtEnable(CTN_100400_BTN_10040011,false);
		SetBtEnable(CTN_100400_BTN_10040012,false);
		m_iWaitIndex = 0;
		int iIndex = 0;
		UserInfoForGame sUser;
		for(int i=m_iWaitIndex;i<=PLAY_COUNT;i++)
		{
			ZeroMemory(&sUser,sizeof(sUser));
			if (m_pGameFrame->GetUserInfo(m_iZhuangList[i],sUser))
			{
				iIndex++;
				SetTextinfo(CTN_100400_TEXT_10040021+i,iIndex,bFalg);
				SetTextinfo(CTN_100400_TEXT_10040031+i,sUser.szNickName,bFalg);
				SetTextinfo(CTN_100400_TEXT_10040041+i,(sUser.i64Money),bFalg);
			}
			if (iIndex>=5)
			{
				break;
			}
		}
	}
	else
	{
		if (m_iWaitIndex <=0)
		{
			m_iWaitIndex = 0;
			SetBtEnable(CTN_100400_BTN_10040011,false);
			SetBtEnable(CTN_100400_BTN_10040012,true);
		}
		else if (m_iWaitIndex >= iCount-5)
		{
			SetBtEnable(CTN_100400_BTN_10040011,true);
			SetBtEnable(CTN_100400_BTN_10040012,false);
			m_iWaitIndex = iCount-5;
		}
		else
		{
			SetBtEnable(CTN_100400_BTN_10040011,true);
			SetBtEnable(CTN_100400_BTN_10040012,true);
		}
		int iIndex = 0;
		UserInfoForGame sUser;
		for(int i=m_iWaitIndex;i<=PLAY_COUNT;i++)
		{
			ZeroMemory(&sUser,sizeof(sUser));
			if (m_pGameFrame->GetUserInfo(m_iZhuangList[i],sUser))
			{
				iIndex++;
				SetTextinfo(CTN_100400_TEXT_10040021+(i-m_iWaitIndex),iIndex,bFalg);
				SetTextinfo(CTN_100400_TEXT_10040031+(i-m_iWaitIndex),sUser.szNickName,bFalg);
				SetTextinfo(CTN_100400_TEXT_10040041+(i-m_iWaitIndex),(sUser.i64Money),bFalg);
			}
			if (iIndex>=5)
			{
				break;
			}
		}
	}
	

	if (bFalg)
	{
		//�ж��Ƿ���ׯ���б���
		bool bInWait = false;
		if (m_iMyDeskStation == m_iNowNTstation)
		{
			bInWait = true;
		}
		else
		{
			for(int i=0; i<PLAY_COUNT;i++)
			{
				if (m_iZhuangList[i] == m_iMyDeskStation)
				{
					bInWait = true;
					break;
				}
			}
		}

		if (bInWait)
		{
			SetBtVisible(CTN_100400_BTN_10040001,false);
			SetBtVisible(CTN_100400_BTN_10040002,true);
		}
		else
		{
			SetBtVisible(CTN_100400_BTN_10040001,true);
			SetBtVisible(CTN_100400_BTN_10040002,false);
		}
	}
	else
	{
		SetBtVisible(CTN_100400_BTN_10040001,true);
		SetBtVisible(CTN_100400_BTN_10040002,false);
	}	
}
//-------------------------------------------------------------------------
//��ʾ�н�����
void		CNewGameClient::ShowWinAres(bool bFalg)
{
	//��ȫ������
	for(int i=0; i<BET_ARES; i++)
	{
		SetImageVisible(CTN_1000000_CTN_100700_IMG_10070001+i,false);
	}

	for(int i=0; i<BET_ARES; i++)
	{
		SetImageVisible(CTN_1000000_CTN_100700_IMG_10070001+i,m_iWinQuYu[i] && bFalg);
	}
}
//-------------------------------------------------------------------------
//������ʾ·��
void		CNewGameClient::ShowLuziPoint(bool bFlag)
{
	//����·���ж��ٸ�
	int iCount = m_DqLuziData.size();
		
	if (iCount<=10)
	{
		for(int i=0; i<iCount;i++)
		{
			SetTextinfo(CTN_101000_TEXT_10100011+i,m_DqLuziData.at(i).byZPoint,bFlag);
			SetTextinfo(CTN_101000_TEXT_10100031+i,m_DqLuziData.at(i).byXpoint,bFlag);
		}
	}
	else
	{
		for(int i=iCount-10; i<iCount;i++)
		{
			SetTextinfo(CTN_101000_TEXT_10100011+(i-(iCount-10)),m_DqLuziData.at(i).byZPoint,bFlag);
			SetTextinfo(CTN_101000_TEXT_10100031+(i-(iCount-10)),m_DqLuziData.at(i).byXpoint,bFlag);
		}
	}
}
//-------------------------------------------------------------------------
//��ʾ·������
void		CNewGameClient::ShowLuziAres(bool bFlag)
{
	//������
	for(int i=0; i<10; i++)
	{
		SetImageVisible(CTN_100600_IMG_10060011+i,false);
		SetImageVisible(CTN_100600_IMG_10060031+i,false);
		SetImageVisible(CTN_100600_IMG_10060051+i,false);
	}
	SetTextinfo(CTN_100600_TEXT_10060071,(__int64)0,false);
	SetTextinfo(CTN_100600_TEXT_10060072,(__int64)0,false);
	SetTextinfo(CTN_100600_TEXT_10060073,(__int64)0,false);

	//����·���ж��ٸ�
	int iCount = m_DqLuziData.size();


	CString sFilePath;
	if (iCount<=10)
	{
		SetBtEnable(CTN_100600_BTN_10060001,false);
		SetBtEnable(CTN_100600_BTN_10060002,false);
		m_iLuziIndex = 0;
		for(int i=m_iLuziIndex; i<iCount;i++)
		{
			if (0 == m_DqLuziData.at(i).byWinResult)
			{
				sFilePath.Format(".\\resources\\Luzi\\Zhuang.png");
				SetImageLoadSrc(CTN_100600_IMG_10060011+i,sFilePath,bFlag);
			}
			else if (1 == m_DqLuziData.at(i).byWinResult)
			{
				sFilePath.Format(".\\resources\\Luzi\\Xian.png");
				SetImageLoadSrc(CTN_100600_IMG_10060031+i,sFilePath,bFlag);
			}
			else if (2 == m_DqLuziData.at(i).byWinResult)
			{
				sFilePath.Format(".\\resources\\Luzi\\He.png");
				SetImageLoadSrc(CTN_100600_IMG_10060051+i,sFilePath,bFlag);
			}
		}
	}
	else
	{
		if (m_iLuziIndex <= 0)
		{
			m_iLuziIndex = 0;
			SetBtEnable(CTN_100600_BTN_10060001,false);
			SetBtEnable(CTN_100600_BTN_10060002,true);
		}
		else if (m_iLuziIndex >= iCount-10)
		{
			m_iLuziIndex = iCount-10;
			SetBtEnable(CTN_100600_BTN_10060001,true);
			SetBtEnable(CTN_100600_BTN_10060002,false);
		}
		else
		{
			SetBtEnable(CTN_100600_BTN_10060001,true);
			SetBtEnable(CTN_100600_BTN_10060002,true);
		}
		for(int i=m_iLuziIndex; i<m_iLuziIndex+10;i++)
		{
			if (0 == m_DqLuziData.at(i).byWinResult)
			{
				sFilePath.Format(".\\resources\\Luzi\\Zhuang.png");
				SetImageLoadSrc(CTN_100600_IMG_10060011+(i-m_iLuziIndex),sFilePath,bFlag);
			}
			else if (1 == m_DqLuziData.at(i).byWinResult)
			{
				sFilePath.Format(".\\resources\\Luzi\\Xian.png");
				SetImageLoadSrc(CTN_100600_IMG_10060031+(i-m_iLuziIndex),sFilePath,bFlag);
			}
			else if (2 == m_DqLuziData.at(i).byWinResult)
			{
				sFilePath.Format(".\\resources\\Luzi\\He.png");
				SetImageLoadSrc(CTN_100600_IMG_10060051+(i-m_iLuziIndex),sFilePath,bFlag);
			}
		}
	}


	//����ͳ��
	int iCountZ = 0;
	int iCountX = 0;
	int iCountH = 0;
	for(int i=0; i<MAXCOUNT&&i<iCount; i++)
	{
		if (m_DqLuziData.at(i).byWinResult == 0)
		{
			iCountZ++;
		}
		else if (m_DqLuziData.at(i).byWinResult == 1)
		{
			iCountX++;
		}
		else if (m_DqLuziData.at(i).byWinResult == 2)
		{
			iCountH++;
		}
	}

	SetTextinfo(CTN_100600_TEXT_10060071,iCountZ,bFlag);
	SetTextinfo(CTN_100600_TEXT_10060072,iCountX,bFlag);
	SetTextinfo(CTN_100600_TEXT_10060073,iCountH,bFlag);
}
//-------------------------------------------------------------------------
//��ʾ��ׯ�б�
void	CNewGameClient::ShowWaitNtInfo(bool bFlag)
{
	
}
//-------------------------------------------------------------------------
///��ʾ�����
void		CNewGameClient::ShowGameResult(bool bFlag) 
{
	SetContainVisible(CTN_101100,bFlag);
	CString sText;
	///<����Լ���Ӯ
	if (m_i64MeScore > 0)
	{
		sText.Format("+%I64d",m_i64MeScore);
	}
	else
	{
		sText.Format("%I64d",m_i64MeScore);
	}
	SetTextinfo(CTN_101100_TEXT_1011011,(sText),bFlag);
	///<�Լ�����
	if (m_i64MeScore > 0)
	{
		sText.Format("+%I64d",m_i64MeFanFen);
	}
	else
	{
		sText.Format("%I64d",m_i64MeFanFen);
	}
	SetTextinfo(CTN_101100_TEXT_1011013,(sText),bFlag);
	///<ׯ����Ӯ
	if (m_i64MeScore > 0)
	{
		sText.Format("+%I64d",m_i64ZhuangScore);
	}
	else
	{
		sText.Format("%I64d",m_i64ZhuangScore);
	}
	SetTextinfo(CTN_101100_TEXT_1011012,(sText),bFlag);
}
//-------------------------------------------------------------------------

















//--------------------------------------------------------------------------------------------------------
//���ö����ؼ����Ŷ���
void	CNewGameClient::SetAnimatePlay(const int AnimateID,bool bFlag,BYTE byPlayNum, bool bIsLoop)
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetPlayNum(byPlayNum);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}
	
}
//--------------------------------------------------------------------------------------------------------
//���ö����ؼ�����ָ������
void	CNewGameClient::SetAnimatePlay(const int AnimateID,wchar_t wsFilePath[],bool bFlag,BYTE byPlayNum , bool bIsLoop )
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetExtFilePath(wsFilePath) ;
		pAnimate->LoadFromFile();
		pAnimate->SetPlayNum(byPlayNum);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}
}
//--------------------------------------------------------------------------------------------------------
//���ö�����ʾĳһ֡
void	CNewGameClient::SetAnimateShowFrame(const int AnimateID,bool bFlag,int iFrame)
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
	
		pAnimate->SetPlayState(bFlag);
		if (-1 == iFrame)
		{
			pAnimate->SetShowFrame(pAnimate->GetFrameNum()-1);
		}
		else
		{
			pAnimate->SetShowFrame(iFrame);
		}
		pAnimate->SetControlVisible(bFlag);

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
//����ͼƬ�ؼ��Ƿ�ɼ�
void	CNewGameClient::SetImageVisible(const int iImageID, bool bFlag,bool bMouseMove)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetMouseMove(bMouseMove);
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
//����ͼƬ�ؼ�������ʾָ�����ļ�
void	CNewGameClient::SetImageXYInfo(const int iImageID,int iSrcX,bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetSrcX(iSrcX);
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
//���������ؼ��ɼ�
void	CNewGameClient::SetContainPosition(const int iContainID,int iX,int iY,bool bFlag)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		pContain->SetControlVisible(bFlag);
		pContain->SetGameXY(iX,iY);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����
void	CNewGameClient::SetTextinfo(const int iTextID,__int64 iNum, bool bFlag,bool bSymbol)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		if(bSymbol && iNum>=0)
		{
			sText.Format("+%I64d",iNum);
		}
		else
		{
			sText.Format("%I64d",iNum);
		}
		
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����
void	CNewGameClient::SetTextinfo(const int iTextID,TCHAR sChar[], bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%s",sChar);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����
void	CNewGameClient::SetTextinfo(const int iTextID,CString sText, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�������ֿռ���ʾ����
void	CNewGameClient::SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag,bool bSymbol)
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
//���õ�ѡ��ؼ��Ƿ�ѡ��
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected, bool bFlag)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		pRadioButton->SetIsSelect(bSelected);
		pRadioButton->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ƶ��ؼ�����
void	CNewGameClient::SetMoveActionPlay(const int iMoveActionID,const int iControlID,POINT pStart, POINT pEnd,unsigned int uTime,bool bPlay)
{
	IMoveAction* pMoveAction  = NULL;
	pMoveAction = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(iMoveActionID));
	if(pMoveAction != NULL)
	{
		pMoveAction->SetControlingCtrlID(iControlID);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pEnd);
		pMoveAction->SetTotalMoveTime(uTime);
		pMoveAction->SetControlVisible(bPlay);
		pMoveAction->SetPlayState(bPlay);
	}
}
/*-------------------------------------------------------------------------------*/
//���ò��ɲ����ƿؼ�����
void	CNewGameClient::SetNoMoveCardInfo(const int iNoMoveCardID,BYTE byCardList[],BYTE byCardCount,POINT pPT, bool bShow)
{
	INoMoveCard *pNoMoveCard = NULL;
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iNoMoveCardID)); 
	if(NULL != pNoMoveCard)
	{
		pNoMoveCard->SetGameXY(pPT.x,pPT.y);
		pNoMoveCard->SetCardList(byCardList,byCardCount);
		pNoMoveCard->SetControlVisible(bShow);
	}
}
//���ò��ɲ����ƿؼ�����
void	CNewGameClient::SetNoMoveCardInfo(const int iNoMoveCardID,BYTE byCardList[],BYTE byCardCount,bool bShow)
{
	INoMoveCard *pNoMoveCard = NULL;
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iNoMoveCardID)); 
	if(NULL != pNoMoveCard)
	{
		pNoMoveCard->SetCardList(byCardList,byCardCount);
		pNoMoveCard->SetControlVisible(bShow);
	}
}

/*-------------------------------------------------------------------------------*/
//���õ�ѡ��
void	CNewGameClient::SetRadioButtonSeled(const int iRadioID,bool bSeled)
{
	IRadioButton *pRadioBt = NULL;
	pRadioBt = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRadioID));
	if (NULL != pRadioBt)
	{
		pRadioBt->SetIsSelect(bSeled);
	}
}


//��ȡ��ѡ���Ƿ�ѡ��
bool	CNewGameClient::GetRadioBtIsSel(const int iRadioID)
{
	IRadioButton *pRadioBt = NULL;
	pRadioBt = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRadioID));
	if (NULL != pRadioBt)
	{
		return pRadioBt->GetIsSelect();
	}
	return false;
}

//��ȡ��ѡ���Ƿ�ѡ��
void	CNewGameClient::SetRadioVisible(const int iRadioID,bool bShow)
{
	IRadioButton *pRadioBt = NULL;
	pRadioBt = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRadioID));
	if (NULL != pRadioBt)
	{
		pRadioBt->SetControlVisible(bShow);
	}
}