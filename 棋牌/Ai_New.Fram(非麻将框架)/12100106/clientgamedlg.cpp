#include "StdAfx.h"
#include "clientgamedlg.h"
//#include "playcardsound.h"
#include <math.h>
#include "MultyTree.h"

//�Զ�����Ϣ��// To Be Continued..
enum MB_AFC_STYLE          //KT��Ϣ������
{
	AFC_CONFIRM = 500,       //ȷ�϶Ի���
	AFC_YESNO,               //��ʾ���ǡ������񡱶Ի���
	AFC_WARN                 //����Ի���
};
void DebugPrintf(const char *p, ...)
{
	char szFilename[256];
	sprintf( szFilename, "E:\\bzw\\DebugInfo\\AI_%d.txt", NAME_ID);
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start( arg, p );
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}
#define  SOUND_TYPE_CHIP  0
#define  SOUND_TYPE_WIN   1
#define  SOUND_TYPE_LOSE  2
#define  SOUND_TYPE_GROUP_OVER 3
BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGED()	// ���ڵ���Flash��������ڸ����ڵ�λ��
END_MESSAGE_MAP()
//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	m_iThinkTime=0;
	m_iCardCount=0;
	m_iNowOutPeople=0;
	m_iFirstOutPeople = 0;
	m_bTimeOutCount=0;
	//ϵͳ������
	//m_iSysCardCount=28;
	m_iNowBigNote=0;				//��ǰ���Ѻע��
	m_iThisTurnLimit=0;				//������ע
	//::memset(m_iBackCard,0,sizeof(m_iBackCard));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
	memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));
	memset(m_Ming,0,sizeof(m_Ming));
	memset(m_bOpenLose,0,sizeof(m_bOpenLose)); //�Ƿ�Ϊ����ʧ����
	memset(m_bIsGiveUp,0,sizeof(m_bIsGiveUp));
	m_iThisNoteTimes=0;
    m_mingPaiNum = 0;
	//lc090106��ʼ������
	m_iRoomBasePoint=0;
	m_iRunPublish=0;
	m_iLimitNote=0;
	m_iBaseNote=0;
	memset(m_DeskCard,0,sizeof(m_DeskCard));
	memset(m_UserCard,0,sizeof(m_UserCard));
	m_iFrontNote=0;
	m_iAfterWaitAction = 1;		//��ʼ��Ϊ�Զ���ʼ 090311a1 JJ
	//	m_nJiaoZu=0;
	m_bBgSound=true;
	m_hMciWnd1=NULL;
	m_hMciWnd2=NULL;
	m_iNoteTotalPoint=0;					//������ע
	memset(m_bMing,0,sizeof(m_bMing));
	m_iNoteLimit=0;							//��������
	m_bFirstGame = false;
	m_iMyAddNote = 0;
	memset(m_arrUFlashID,0,sizeof(m_arrUFlashID));
	m_iOldCardCtrlDistH = 30;
	m_iNewCardCtrlDistH = 50;
	m_iComWinStation = 255;
	m_iComLoseStation = 255;
	m_bReachLimitStation = 255;
	m_bWatchMode = false;

	m_byAddCount=0;				//���˼���
	m_byMaxCount=1;				//�����Ѻ��ƣ����ֵ��
	m_byCtrPet = 100;			//���ưٷֱȣ�������ֵ��
	m_bIsCtrl = false;			//�����Ƿ���ȫ����
	m_bMaxPaiAI = false;		//��������Ƿ������
	m_bMaxPaiMe = false;				//��������Ƿ��Լ�

}

//��������
CClientGameDlg::~CClientGameDlg()
{
	m_bGameFinishAndComp = false;
}

//�����Ϸ�汾
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{
	return TRUE;
}

//��ʱ��
void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}
void CClientGameDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);
}

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_OUT_CARD);
	KillTimer(ID_LOOK_BACK);	
	KillTimer(ID_BEGIN_ANIMAL);	
	KillTimer(ID_NOTE_ANIMAL);
	KillTimer(ID_SEND_CARD_ANIMAL);
	KillTimer(ID_COLLECT_JETON_ANIMAL);	
	return;
}

void CClientGameDlg::ShowStartButtons()
{
}

//�����˿˰�ť����  //թ�𻨴˹������� yjj 090505
LRESULT	CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	return 0;
}
// �ؼ���ʼ��ť�����
bool CClientGameDlg::OnControlHitBegin()
{
	if ((GetStationParameter()==GS_WAIT_SETGAME||GetStationParameter()==GS_WAIT_NEXT||GetStationParameter()==GS_WAIT_ARGEE)&&!m_bWatchMode)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}
	ResetGameStation(RS_GAME_BEGIN);
	KillAllTimer();
	return true;
}
//--------------------------------------------------------------------------------

void CClientGameDlg::SetCtrlBtn(bool bIsFirstNote)
{
	if (m_bWatchMode)
	{
		return;
	}
    if (GetStationParameter() != GS_PLAY_GAME)  //û����Ϸ״̬
    {
		DisableAllOperaBtn();
		return;
    }
	else if (GetMeUserInfo()->bDeskStation != m_iNowOutPeople)  //��������ע
	{
		DisableAllOperaBtn();
		return;

	}
	bool bCanOpen = false;
	BYTE bMeStation = GetMeUserInfo()->bDeskStation;
	int iPlayNum = GetTotalPlayNum();
	if ( bMeStation < PLAY_COUNT && m_iThisNoteTimes >= iPlayNum)
	{
		int iTempFront = (m_iFrontNote * 2);
		if (m_Ming[bMeStation])
		{
			iTempFront *= 2;
		}
		if (m_iTotalGameNote[bMeStation] + iTempFront <= m_iLimitNote)
		{
			bCanOpen = true;
		}
		else
		{
			bCanOpen = false;
		}		
	}
	if (bIsFirstNote) //�տ�ʼ��Ϸ
	{
		bool bLookCard = true;
		if (m_Ming[GetMeUserInfo()->bDeskStation])
		{
			bLookCard = false;
		}
		UpdateShowNoteBtn(bCanOpen,0,1,0,bLookCard,0); 
		UpdatAddNumBtn(true);
	}
	else  //֮�����ע
	{
		bool bLookCard = true;
		if (m_Ming[GetMeUserInfo()->bDeskStation])
		{
			bLookCard = false;
		}
	    UpdateShowNoteBtn(bCanOpen,0,1,1,bLookCard,0);
		UpdatAddNumBtn(false);
	}

}
//���ü�ע������ť����ʾ״̬
bool CClientGameDlg::UpdatAddNumBtn(bool bFirst)
{
	if (bFirst)  //��һ�Σ�������X1 X2 X3
	{
		m_AiOperateStation.SetNmuBtnEnableCount(3);
		return true;
	}
	else
	{
		int iLimet = GetLimit();
		if (m_iFrontNote + m_iBaseNote * 3 <= iLimet)
		{
			m_AiOperateStation.SetNmuBtnEnableCount(3);
		}
		else if (m_iFrontNote + m_iBaseNote * 2 <= iLimet)
		{
			m_AiOperateStation.SetNmuBtnEnableCount(2);
		}
		else if (m_iFrontNote + m_iBaseNote <= iLimet)
		{
			m_AiOperateStation.SetNmuBtnEnableCount(1);
		}
		else
		{
			m_AiOperateStation.SetNmuBtnEnableCount(0);
		}
		return true;
	}
}
//���ð�ť����״̬
void CClientGameDlg::UpdateShowNoteBtn(bool bOpen,bool bAdd,bool bGiveUp,bool bFollow,bool bLook,bool bChipIn)
{
	m_AiOperateStation.SetAIStation(bOpen,bAdd,bGiveUp,bFollow,bLook,bChipIn);
	return ;
}
//���а�ť��������
void CClientGameDlg::DisableAllOperaBtn()
{
	m_AiOperateStation.Init();
}
void CClientGameDlg::EnableAddOrNote()
{
}
//���ƺ����Ӧ����
void CClientGameDlg::OnFinshLookCard()
{
	SetCtrlBtn(m_bFirstGame);   //�������ò�����ť
}
void CClientGameDlg::OnFinishOpenCard()
{
	m_iComWinStation = 255;
	m_bReachLimitStation = 255;
	m_iComLoseStation = 255;
}
//-------------------������λ�õ��ؼ�λ��----------
BYTE CClientGameDlg::GetViewStaionToCtr(BYTE bDeskStation)
{
	if (bDeskStation >= PLAY_COUNT)
	{
		return 255;
	}
	return (ViewStation(bDeskStation) + PLAY_COUNT - 1) % (PLAY_COUNT);
}
//---------------------------------------------------------------------------------
 bool CClientGameDlg::OnCloseGame()           //�ر���Ϸ
 {
	 TCHAR sz[200];	
	 KillAllTimer();
	  __super::OnCancel();
	 return true;
 }
 void CClientGameDlg::OnCancel()//{OnCloseGame(); __super::OnCancel();}
 {
	 if (!OnCloseGame())
	 {
		 return;
	 }
	 __super::OnCancel();
 }
 //---------------��Ϸ����----------------------------------
 bool CClientGameDlg::OnChipIn()   //��ע����
 {	
	 tagUserProcess process;
	 process.iVrebType = TYPE_NOTE;//ret;				//��ע����
	 process.iNote=m_iMyAddNote;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)

	 CString str="";
	 str.Format("��ע���ԣ������� OnChipIn m_iMyAddNote=%d m_bFirstGame=%d",m_iMyAddNote,m_bFirstGame);
	 OutputDebugString(str);

	 process.bMing=m_Ming[GetMeUserInfo()->bDeskStation];
	 SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
	 DisableAllOperaBtn();   //�������еĲ�����ť
	  KillAllTimer();
	 return true;
 }
 bool CClientGameDlg::OnAdd()      //��ע����
 {
	 //�����ע�Ƿ񳬹�����
	 tagUserProcess process;
	 process.iVrebType = TYPE_ADD;//ret;				//��ע����
	 process.iNote=m_iMyAddNote;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)

	 CString str="";
	 str.Format("��ע���ԣ������˼�ע m_iMyAddNote=%d m_bFirstGame=%d",m_iMyAddNote,m_bFirstGame);
	 OutputDebugString(str);

	 process.bMing=m_Ming[GetMeUserInfo()->bDeskStation];
	 SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
	 DisableAllOperaBtn();    //�������еĲ�����ť
	 KillAllTimer();
	 //UpdateAddChipShow(0);
	 return true;
 }
 bool CClientGameDlg::OnFllow()    //��ע����
 {
	 tagUserProcess process;
	 process.iVrebType = TYPE_FOLLOW;				//��ע����
	 process.bMing=m_Ming[GetMeUserInfo()->bDeskStation];
	 process.iNote = 0;  //����ע��Ϊ0
	 //		process.iNote=100;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
	 SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
     DisableAllOperaBtn();     //�������Ĳ�����ť
	 KillAllTimer();

	 CString str="";
	 str.Format("��ע���ԣ������� OnFllow m_iMyAddNote=%d m_bFirstGame=%d",m_iMyAddNote,m_bFirstGame);
	 OutputDebugString(str);

	 return true;
 }

 bool CClientGameDlg::OnGiveUp()  //��������
 {
	 tagUserProcess process;
	 process.iVrebType = TYPE_GIVE_UP;				//��ע����
	 process.bMing=m_Ming[GetMeUserInfo()->bDeskStation];
	 process.iNote = 0;  //����ע��Ϊ0	
	 SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
	 DisableAllOperaBtn();     //�������Ĳ�����ť
	 KillAllTimer();
	 return true;
 }
 bool CClientGameDlg::OnLook()     //���Ʋ���  ��Ϊ������� 
 {
	 if(m_Ming[GetMeUserInfo()->bDeskStation])
	 { 
		return true;  //���ƹ��ˣ��Ͳ����ٷ�������Ϣ��
	 }
	 m_Ming[GetMeUserInfo()->bDeskStation]=true;
	 tagUserProcess process;
	 process.iVrebType = TYPE_LOOKCARD;				//��ע����
	 process.bMing=m_Ming[GetMeUserInfo()->bDeskStation];	 
	 SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);	
	 DisableAllOperaBtn();   //����
	 return true;
 }
 bool CClientGameDlg::OnOpen()     //���Ʋ���
 {
	 tagCompareCard process;
	 process.iVrebType = TYPE_COMPARE_CARD;
	 BYTE  bRet;
	 for(bRet =0;bRet<PLAY_COUNT;bRet++)
	 {
		 if(NULL != m_pUserInfo[bRet]&&bRet!=GetMeUserInfo()->bDeskStation)
			 break;
	 }
	 if(bRet>=PLAY_COUNT)		//���û���ҵ��ɱȵ���,��Ϊ�Ƿ�����,����Ϊ����
	 {
		 OnGiveUp();
		 return true;
	 }
	 process.byComparedStation = bRet;             
	 process.bMing=m_Ming[GetMeUserInfo()->bDeskStation];
	 process.iNote = 0;
	 SendGameData(&process,sizeof(process), MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, 0);
	 DisableAllOperaBtn();
	 KillAllTimer();
		//��ӿ��ƻ���
	 return true;
 }
//��עΪ����
 bool CClientGameDlg::OnAddCount(int bMutBase)
 {
     m_iMyAddNote = bMutBase * m_iBaseNote;
	 EnableAddOrNote(); //��ע����ʾ��ע����ע��ť
	 return true;
 }
 bool CClientGameDlg::OnAddCancle()
 {
	 m_iMyAddNote = 0;
	 EnableAddOrNote();
	 return true;
 }
//-------------------------------------------------�����˲���--------------------------
 void CClientGameDlg::AutoAI()
 {
	CString strbug="";
	strbug.Format("�����˲�����λ��%d AutoAI()",GetMeUserInfo()->bDeskStation);
	OutputDebugString(strbug);

	 if (GetMeUserInfo()->bDeskStation != m_iNowOutPeople)
	 {
		 return;
	 }
	 if (m_AiOperateStation.GetCanLook()/* && !m_bFirstGame*/)
	 {
         for(BYTE i=0;i<PLAY_COUNT;i++)
		 {
			 if(i != GetMeUserInfo()->bDeskStation&&m_pUserInfo[i]&&m_Ming[i])   //�������ƣ���Ҳ���ơ�
			 {
				 OnLook();  //����
				 return;
			 }
		 }
		 if(noPeopleKnow()) 
		 {
			 if(m_shenglv<10)           //��Ҫ������һ�����ɣ���Ϊ����ƽ���޹ʷ���
			 {
				 OnLook();  //����
				 return;
			 }
			 if(m_shenglv<20&&!m_bFirstGame)    //��Ҫ������һ�����ɣ���Ϊ����ƽ���޹ʷ���
			 {
				 OnLook();  //����
				 return;
			 }
		 }
		 else
		 {
			 if(m_shenglv<20)           //��Ҫ������һ�����ɣ���Ϊ����ƽ���޹ʷ���
			 {
				 OnLook();  //����
				 return;
			 }
			 if(m_shenglv<35&&!m_bFirstGame)    //��Ҫ������һ�����ɣ���Ϊ����ƽ���޹ʷ���
			 {
				 OnLook();  //����
				  return;
			 }
		 }
	 }
	 if(m_Ming[GetMeUserInfo()->bDeskStation])
		 m_mingPaiNum++;
	 else 
		 m_mingPaiNum = 0;
	 int iBtnCount = m_AiOperateStation.GetNmuBtnEnableCount();


//////lym���ݷ��������������жϣ��Լ��Ĵ�Сλ�ã��ƹ���ʹ���ȥ
//
	srand( GetMeUserInfo()->bDeskStation );
	m_byAddCount++;


	strbug.Format("�����˲�����λ��%d m_byAddCount=%d m_byMaxCount=%d m_bMaxPaiAI=%d m_bMaxPaiMe=%d",GetMeUserInfo()->bDeskStation, m_byAddCount,m_byMaxCount,m_bMaxPaiAI,m_bMaxPaiMe);
	OutputDebugString(strbug);
	int beishu = 1;
	//��������Ĳ������ˣ������䣩����
	if(m_bIsCtrl)//��ȫ����
	{
		if(m_bMaxPaiAI && m_bMaxPaiMe)//�������ƴ�,�ҵ������
		{					
			OnAddCount(1); 
			if (m_bFirstGame)
			{
				OnChipIn();
			}
			else
			{
				if(rand()%2==0)
				{
					strbug.Format("�����˲�����λ��%d m_byAddCount=%d ���� 00",GetMeUserInfo()->bDeskStation, m_byAddCount);
					OutputDebugString(strbug);
					OnFllow();
				}
				else
				{
					strbug.Format("�����˲�����λ��%d m_byAddCount=%d �� 00",GetMeUserInfo()->bDeskStation, m_byAddCount);
					OutputDebugString(strbug);
					OnAdd();
				}
			}
			return;
		}
		else if(m_bMaxPaiAI && !m_bMaxPaiMe)//�����������ƴ�
		{
			if(m_byAddCount>=m_byMaxCount)//�����㹻�İ���
			{
				if (m_Logic.GetCardShape(m_UserCard[GetMeUserInfo()->bDeskStation],m_iDeskCardCount[GetMeUserInfo()->bDeskStation]) >= SH_DOUBLE) //�����˵����Ƕ������Ͼ��ȱ���
				{
					OnOpen();
				}
				else
				{
					if (m_Logic.GetMaxCardNum(m_UserCard[GetMeUserInfo()->bDeskStation],m_iDeskCardCount[GetMeUserInfo()->bDeskStation])
						== 14 && rand()%2 == 1)//�������A�������һ����ʿ���
					{
						OnOpen();
					}
					else
					{
						OnGiveUp();
					}
					
				}
				
				return;
			}
			else
			{
				OnAddCount(1); 
				if (m_bFirstGame)
				{
					OnChipIn();
				}
				else
				{
					strbug.Format("�����˲�����λ��%d m_byAddCount=%d ���� 222",GetMeUserInfo()->bDeskStation, m_byAddCount);
					OutputDebugString(strbug);
					OnFllow();
				}
				return;
			}
		}
		else if(m_byAddCount>=m_byMaxCount)//�����������󣬸����˰���
		{
			if (m_Logic.GetCardShape(m_UserCard[GetMeUserInfo()->bDeskStation],m_iDeskCardCount[GetMeUserInfo()->bDeskStation]) >= SH_DOUBLE) //�����˵����Ƕ������Ͼ��ȱ���
			{
				OnOpen();
			}
			else
			{
				if (m_Logic.GetMaxCardNum(m_UserCard[GetMeUserInfo()->bDeskStation],m_iDeskCardCount[GetMeUserInfo()->bDeskStation])
					== 14 && rand()%2 == 1)//�������A�������һ����ʿ���
				{
					OnOpen();
				}
				else
				{
					OnGiveUp();
				}

			}
			return;
		}
		else//������������û�и���
		{
			OnAddCount(1); 
			if (m_bFirstGame)
			{
				OnChipIn();
			}
			else
			{
				strbug.Format("�����˲�����λ��%d m_byAddCount=%d ���� 444",GetMeUserInfo()->bDeskStation, m_byAddCount);
				OutputDebugString(strbug);
				OnFllow();
			}
			return;
		}
	}
	OutputDebugString("���𻨻����˲��ԣ������˷���333");
	OnGiveUp();
	return;
 }

 void CClientGameDlg::getShengLv()
 {
	 realData data;
	 memcpy(data.m_MyCard,m_UserCard[GetMeUserInfo()->bDeskStation],sizeof(data.m_MyCard));
     data.computeShengLv();
	 int duiShouNum = -1;          //���ڼ��������������Ϊ����������ʱ��ʤ�ʻ������仯
     for(BYTE i=0;i<PLAY_COUNT;i++)
	 {
		 if(NULL != m_pUserInfo[i])
			 duiShouNum++;
	 }
	 double shenglv = (double)data.shengli/(double)(data.shengli+data.shibai);
	 m_shenglv = pow(shenglv,duiShouNum);
	 m_shenglv *= 100.0;
	 CString str;
	 str.Format("log:%d,%d",data.shengli,data.shibai);
	 OutputDebugString(str);
	 str.Format("log:%d,%d,%d",data.m_MyCard[0],data.m_MyCard[1],data.m_MyCard[2]);
	 OutputDebugString(str);
	 str.Format("log:ʤ��%2.2f,%2.2f",shenglv,m_shenglv);
	 OutputDebugString(str);
 }

bool CClientGameDlg::noPeopleKnow()
{
	 for(BYTE i=0;i<PLAY_COUNT;i++)
	 {
		 if(i != GetMeUserInfo()->bDeskStation&&NULL != m_pUserInfo[i]&&m_Ming[i])
			 return false;
	 }
	 return true;
}

 int CClientGameDlg::GetAIThinkTime()
 {
	 if(m_Ming[GetMeUserInfo()->bDeskStation])  
	 {
		 if(m_shenglv<35)
		 {
			 if(m_mingPaiNum==1)
		    	 return 1;
			 else
				 return rand() % 1 + 1;
		 }
		 else if(m_shenglv<78)
		 {
			 if(m_mingPaiNum==1)
		    	 return 1;
			 else
				 return rand() % 2 + 1;
		 }
		 else
		 {
			 return rand() % 5 + 1;
		 }
		 
	 }
	 return rand() % 2 + 1;

 }
//----------------------------------------------------------------------------------
//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:	//�ȴ�ͬ��״̬
		{	
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iPalyCardCount=pGameStation->iAllCardCount;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);	
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			m_iLimitNote=pGameStation->iLimitNote;			//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_iLimitPerNote =pGameStation->iLimtePerNote;   //��ע���ֵ
			m_iGuoDi = pGameStation->iGuoDi;     //����ֵ
			m_iFrontNote = 0;	
			SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% 3 +2,ID_BEGIN_TIME);
			return TRUE;
		}
	case GS_SEND_CARD:	//����״̬
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pStationData;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iPalyCardCount=pGameStation->iAllCardCount;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_iLimitNote=pGameStation->iLimitNote;			//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_iLimitPerNote =pGameStation->iLimtePerNote;   //��ע���ֵ
			m_iGuoDi = pGameStation->iGuoDi;                //����ֵ
			m_iFrontNote = 0;
			for (int i=0;i<PLAY_COUNT;i++)
			{	
				//��������
				m_iDeskCardCount[i]=pGameStation->iUserCardCount[i];
				::CopyMemory(m_UserCard[i],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
				::CopyMemory(m_DeskCard[i],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
				memset(m_DeskCard[i],0x00,sizeof(m_DeskCard[i]));
			}
			//������Ѻע���
			SetTotalNote(0, m_iBaseNote*PLAY_COUNT);
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pStationData;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iPalyCardCount=pGameStation->iAllCardCount;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			m_iLimitNote=pGameStation->iLimitNote;			//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_iLimitPerNote =pGameStation->iLimtePerNote;   //��ע���ֵ
			m_iGuoDi = pGameStation->iGuoDi;                //����ֵ
			m_bFirstGame = pGameStation->bIsFirstNote;     //�Ƿ�Ϊ��һ����ע

			m_bNtPeople = pGameStation->bNtPeople;
			//UpdateRoomInfo();
			m_iNowOutPeople=pGameStation->iOutCardPeople;
			m_iFrontNote = pGameStation->iCurNote;
			int iRestTime = pGameStation->iTimeRest;
			if (iRestTime <= 0)
			{
				iRestTime = 1;
			}
			m_iFirstOutPeople=pGameStation->iFirstOutPeople;
			//���浱ǰ�û���ע�ܶ�
			::CopyMemory(m_iThisGameNote,pGameStation->iTotalNote,sizeof(pGameStation->iTotalNote));
			::CopyMemory(m_Ming,pGameStation->iMing,sizeof(pGameStation->iMing));
			::CopyMemory(m_iThisGameNote,pGameStation->iThisGameNote,sizeof(pGameStation->iThisGameNote));
			::CopyMemory(m_iTotalGameNote,pGameStation->iTotalGameNote,sizeof(pGameStation->iTotalGameNote));
			::CopyMemory(m_bOpenLose,pGameStation->bOpenLose,sizeof(pGameStation->bOpenLose));
			::CopyMemory(m_bIsGiveUp,pGameStation->bIsGiveUp,sizeof(pGameStation->bIsGiveUp));
			m_iThisTurnLimit=m_iLimitNote;
			m_iNowBigNote=pGameStation->tag.iNowBigNote;
			SetTotalNote(0, pGameStation->tag.iTotalNote);
			//����ʱ��
			if (m_iNowOutPeople != -1) 
				SetGameTimer(m_iNowOutPeople,iRestTime,ID_OUT_CARD);
			m_iThisNoteTimes = pGameStation->tag.bAddTime;

			for (int i=0;i<PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i] == NULL)
				{
					continue;
				}
				//��������
				m_iDeskCardCount[i]=pGameStation->iUserCardCount[i];
				::CopyMemory(m_UserCard[i],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
				memset(m_DeskCard[i],0x00,sizeof(m_DeskCard[i]));
				SetMing(i,m_Ming[i]);
			}
			OnFinshLookCard();
			OnFinishOpenCard();
			SetCtrlBtn(m_bFirstGame);
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			GameStation_5 * pGameStation=(GameStation_5 *)pStationData;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iPalyCardCount=pGameStation->iAllCardCount;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		 //���ܿ۷�
			m_iLimitNote=pGameStation->iLimitNote;			 //���ע
			m_iBaseNote=pGameStation->iBaseNote;			 //��ע
			m_iLimitPerNote =pGameStation->iLimtePerNote;    //��ע���ֵ
			m_iGuoDi = pGameStation->iGuoDi;                 //����ֵ
			//UpdateRoomInfo();
			return TRUE;
		}
	}
	return false;
}

//���Ż��㵽��ͼλ��
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{	
	BYTE bViewStation=GetMeUserInfo()->bDeskStation;
	//�Լ�Ϊ  3
	switch(bViewStation)
	{
	case 0: { 
		if(bDeskStation==0)	
			return 3;
		if(bDeskStation==1)
			return 4;
		if(bDeskStation==2)
			return 5;
		if(bDeskStation==3)
			return 0;
		if(bDeskStation==4)
			return 1;
		if(bDeskStation==5)
			return 2;
		break;
			}
	case 1: {
		if(bDeskStation==0)	
			return 2;
		if(bDeskStation==1)
			return 3;
		if(bDeskStation==2)
			return 4;
		if(bDeskStation==3)
			return 5;
		if(bDeskStation==4)
			return 0;
		if(bDeskStation==5)
			return 1;
		break;
			}
	case 2: { 
		if(bDeskStation==0)	
			return 1;
		if(bDeskStation==1)
			return 2;
		if(bDeskStation==2)
			return 3;
		if(bDeskStation==3)
			return 4;
		if(bDeskStation==4)
			return 5;
		if(bDeskStation==5)
			return 0;
		break;
			}
	case 3: { 
		if(bDeskStation==0)	
			return 0;
		if(bDeskStation==1)
			return 1;
		if(bDeskStation==2)
			return 2;
		if(bDeskStation==3)
			return 3;
		if(bDeskStation==4)
			return 4;
		if(bDeskStation==5)
			return 5;
		break;
			}	
	case 4: { 
		if(bDeskStation==0)	
			return 5;
		if(bDeskStation==1)
			return 0;
		if(bDeskStation==2)
			return 1;
		if(bDeskStation==3)
			return 2;
		if(bDeskStation==4)
			return 3;
		if(bDeskStation==5)
			return 4;
		break;
			}
	case 5:
		{
			if(bDeskStation==0)	
				return 4;
			if(bDeskStation==1)
				return 5;
			if(bDeskStation==2)
				return 0;
			if(bDeskStation==3)
				return 1;
			if(bDeskStation==4)
				return 2;
			if(bDeskStation==5)
				return 3;
			break;
		}
	default:
		break;
	}
	return 0;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
			//�Q����
		case ASS_GM_SUPERCLIENT:
			{
				return true;
			}
		case ASS_UG_USER_SET:		//�û������ƾ�
			{								
				return true;
			}
		case ASS_BEGIN_UPGRADE:		//��Ϸ��ʼ
			{
				if (uDataSize!=sizeof(BeginUpgradeStruct))
					return FALSE;
				SetStationParameter(GS_SEND_CARD);
				m_bTimeOutCount=0;												//��ʱ����
				BeginUpgradeStruct * pBegin=(BeginUpgradeStruct *)pNetData;
				m_iLimitNote=pBegin->iLimitNote;								//���ע
				m_iBaseNote=pBegin->iBaseNote;					//��ע
				m_iLimitPerNote = pBegin->iLimtePerNote;
				m_iGuoDi = pBegin->iGuoDi;
				m_iFrontNote = 0;
				m_iMyAddNote = 0;

				for(int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i] == NULL)
						continue;
					m_iThisGameNote[i]+=m_iGuoDi;
					m_iTotalGameNote[i]+=m_iGuoDi;
					AddTotalNote(i,m_iGuoDi);
					//SetAddJeton(m_iBaseNote);
				}
				if (m_bWatchMode==TRUE) 
				{
					ResetGameStation(RS_GAME_BEGIN);
				}
				KillAllTimer();
				return true;
			}
		case ASS_SEND_CARD:			//����������
			{
				if (uDataSize!=sizeof(SendCardStruct)) 
					return FALSE;
				SendCardStruct * pSendCard=(SendCardStruct *)pNetData;
				//�û�������
				for (int i=0; i<PLAY_COUNT; i++)
				{
					if (m_pUserInfo[i] == NULL)
					{
						continue;
					}
					for (int j=0; j<MAX_CARD_COUNT; j++)
					{
						m_UserCard[i][m_iDeskCardCount[i]]=pSendCard->bCard[i][j];
						m_DeskCard[i][m_iDeskCardCount[i]++]=pSendCard->bCard[i][j];			//�û������ϵ���
					}
					memset(m_DeskCard[i],0x00,sizeof(m_DeskCard[i]));					
				}
                //getShengLv();
				return true;
			}
		case ASS_COLLECT_JETON://�ռ�����
			{
				return true;
			}
		/*case ASS_SEND_A_CARD:
			{
				return true;
			}*/
		case ASS_SEND_FINISH:		//�������
			{	
				return true;
			}
		case ASS_GAME_PLAY:		//��ʼ��Ϸ
			{
				//������ɣ���ʼ��ע
				if (uDataSize!=sizeof(BeginPlayStruct)) 
					return FALSE;


				CINIFile iniFile(CINIFile::GetAppPath()+"ZJHRobotSet.ini");	
				m_byCtrPet	= iniFile.GetKeyVal("zjh", "ctrlpercent",100);//������ȫ������


				//������Ϸ״̬
				SetStationParameter(GS_PLAY_GAME);
				BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)pNetData;

				m_bTimeOutCount=0;		//��ʱ��������			

				m_iNowOutPeople=m_iFirstOutPeople=pBeginInfo->iOutDeskStation;
				m_iNowBigNote=0;							//��ǰ���Ѻע��
				m_iThisTurnLimit=m_iLimitNote;//m_iLimitNote/32*(1<<1)/m_iBaseNote*m_iBaseNote;
				//��һ����ע
				m_iThisNoteTimes=0;
				m_bNtPeople = pBeginInfo->bNtPeople;   //ׯ��λ��
				
				//����ʱ�� 
				SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);

				BYTE bdesk = GetMeUserInfo()->bDeskStation;
				if ((m_bWatchMode==FALSE)&&(GetMeUserInfo()->bDeskStation==m_iNowOutPeople))
				{	
					m_bFirstGame = true;
					SetCtrlBtn(true);   //��һ����ע	
#ifdef AI_MACHINE
					m_iThinkTime = GetAIThinkTime();
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
#endif
				}
				//��ȡ������ݣ��Ƿ�����ˣ��ƴ�С����
				memset(byUserData,255,sizeof(byUserData));
				for(int i=0;i<PLAY_COUNT;i++)
				{
					byUserData[i][0] = pBeginInfo->byUserData[i][0]; 
					byUserData[i][1] = pBeginInfo->byUserData[i][1]; 
				}
				srand( GetMeUserInfo()->bDeskStation );
				m_byAddCount = 0;				//���˼���
				m_byMaxCount = rand()%3+1;		//�����Ѻ��ƣ����ֵ��
				if(m_byMaxCount== 255 || m_byMaxCount>5)
				{
					m_byMaxCount = 5;
				}
				if(rand()%100<m_byCtrPet)
				{
					m_bIsCtrl = true;			//�����Ƿ���ȫ����
				}
				else
				{
					m_bIsCtrl = false;			//�����Ƿ���ȫ����
				}
				m_bMaxPaiAI = false;		//��������Ƿ������
				m_bMaxPaiMe = false;				//��������Ƿ��Լ�
				for(int i=0;i<PLAY_COUNT;i++)
				{		
					if(byUserData[i][0] == 1 && byUserData[i][1] == 0)
					{
						m_bMaxPaiAI = true;
						if(i == GetMeUserInfo()->bDeskStation)
						{
							m_bMaxPaiMe = true;
						}
					}
				}

				return true;
			}
		case ASS_NOTE:								//���Ѻע���
			{

				//DebugPrintf(GetMeUserInfo()->bDeskStation,"ASS_NOTE____begin::%d,receive msg main %d,assin id %d",GetMeUserInfo()->dwUserID,pNetHead->bMainID,pNetHead->bAssistantID);
				if(uDataSize==sizeof(lookCardResult))
				{
					lookCardResult *pResult = (lookCardResult *) pNetData;	
					m_Ming[pResult->bDeskStation]=true;
					SetMing(pResult->bDeskStation,1);
					RedrawWindow();
					//�������ƶ���
					//PlayLookCard(pResult->bDeskStation);
					OnFinshLookCard();
#ifdef AI_MACHINE
					SetGameTimer(m_iNowOutPeople,5,ID_OUT_CARD);
#endif
					return true;
				}	
				if (uDataSize != sizeof(tagUserResult)) 
					return FALSE;
				tagUserResult *pResult = (tagUserResult *) pNetData;
				//�û���ע������ʾ��ע����,�ݲ���ֵ
				m_iNowOutPeople = pResult->iOutPeople;		//��ǰ��ע��				
				m_iThisTurnLimit=m_iLimitNote;				
				//ϵͳ������ע����
				m_iThisNoteTimes=pResult->bAddTime;
				//��ǰ���Ѻע
				m_iNowBigNote=pResult->iNowBigNote;
				m_bFirstGame = pResult->bIsFirstNote;  //�ж��Ƿ�Ϊ��һ����ע

				 CString str="";
				 str.Format("��ע���ԣ������� ASS_NOTE  m_bFirstGame=%d  %d",m_bFirstGame,pResult->bIsFirstNote);
				 OutputDebugString(str);

				//StopAllAnimateNodes();
				if (m_iNowOutPeople == GetMeUserInfo()->bDeskStation && !m_bWatchMode)
				{					
					SetCtrlBtn(m_bFirstGame);
				}
				//����ʱ��
				if (m_iNowOutPeople!=-1 || m_iNowOutPeople >= PLAY_COUNT) 
				{
#ifdef AI_MACHINE
					if (m_iNowOutPeople == GetMeUserInfo()->bDeskStation)
					{
						m_iThinkTime = GetAIThinkTime();
					}
#endif
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
					
				}
				return true;
			}
		case ASS_BIPAI_RESULT:
			{
				if (uDataSize!=sizeof(tagCompareCardResult)) 
					return FALSE;
				tagCompareCardResult *result =(tagCompareCardResult*)pNetData;
				TCHAR szMessage[100];
				AddTotalNote(result->iNoNt,result->iCurNote);
				//����ĳһ������ע�ܶ�
				m_iThisGameNote[result->iNoNt]+=result->iCurNote;
				m_iTotalGameNote[result->iNoNt]+=result->iCurNote;
				m_bOpenLose[result->iLoster] = true;
				m_iComLoseStation = result->iLoster;
				if (result->bAddStyle == TYPE_REACH_LIMITE_SELF)  //�ﵽ����
				{
					m_bReachLimitStation = result->iNt;
				}
				//m_iFrontNote = result->iNote;
				int iComRet = 0;
				if (result->iNoNt == result->iLoster)
				{
					iComRet = 1;
					m_iComWinStation = result->iNt;
				}
				else
				{
					m_iComWinStation = result->iNoNt;
				}
				m_bGameFinishAndComp = result->bGameFinish;
				OnFinishOpenCard();
				return true;
			}
		case ASS_NOTE_RESULT:											//Ѻע���
			{
				//DebugPrintf(GetMeUserInfo()->bDeskStation,"ASS_NOTE_RESULT____begin::%d,receive msg main %d,assin id %d",GetMeUserInfo()->dwUserID,pNetHead->bMainID,pNetHead->bAssistantID);
				if (uDataSize != sizeof(NoteResult)) 
					return FALSE;
				//DebugPrintf(GetMeUserInfo()->bDeskStation,"ASS_NOTE_RESULT-Enter normal");
				NoteResult *noteresult=(NoteResult*)pNetData;
				BYTE bDeskStation = noteresult->iOutPeople;
				if (bDeskStation >= PLAY_COUNT)
				{
					DebugPrintf("receive wrong station %d",bDeskStation);
					return true;
				}
				//������ʾ��Ϣ
				//����Ƿ���������ʾ����
				if(noteresult->bAddStyle==TYPE_GIVE_UP)
				{
					SetMing(bDeskStation,false);
					m_bIsGiveUp[bDeskStation] = true;  //������
				}
				else			//������ע
				{
//					AddNotePoint(bDeskStation,noteresult->iCurNote);
					AddTotalNote(bDeskStation,noteresult->iCurNote);
					//����ĳһ������ע�ܶ�
					m_iThisGameNote[noteresult->iOutPeople]+=noteresult->iCurNote;
					m_iTotalGameNote[noteresult->iOutPeople]+=noteresult->iCurNote;
					m_iFrontNote = noteresult->iNote;
				}
				KillAllTimer();
				return true;
			}

		case ASS_SALE_END:	//��Ϸ��ȫ����
		case ASS_CUT_END:	//�û�ǿ���뿪
		case ASS_CONTINUE_END:	//��Ϸ����
		case ASS_NO_CONTINUE_END:
			{
				//DebugPrintf("��Ϸ����������ASS_GM_AGREE_GAME��Ϣ");
				/*SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);*/
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(RS_GAME_BEGIN);
				
			//	if(!m_bWatchMode)
				{
					m_iAfterWaitAction = 1;
					m_iBeginTime = 1;
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% 3 +2,ID_BEGIN_TIME);					
				}
				return TRUE;
			}
		case ASS_FINISH_COMPARE:
			{				
				if (uDataSize!=sizeof(GameEndStruct)) 
					return FALSE;
				GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
				KillGameTimer(0);
				BYTE bWinStation = 255;				
				for (int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i] == NULL)
						continue;
					if (m_pUserInfo[i]!=NULL)
					{
						if (pGameEnd->iTurePoint[i] > 0)
						{
							bWinStation = i;
						}
					}
				}
				return TRUE;
			}
		case ASS_CHANGE_CARD:
			{
				return TRUE;
			}
		//case ASS_HAVE_THING:	//�û������뿪
		//	{
		//		return true;
		//	}
		//case ASS_LEFT_RESULT:	//�����뿪��� 
		//	{
		//        return true;
		//	}
		//case ASS_MESSAGE:
		//	{
		//		return true;
		//	}
		//case ASS_STOP_THING://�Լ�������ǰ����
		//	{
		//		return true;
		//	}
		//case ASS_AGREE_STOP://������ǰ�������
		//	{
		//		return true;
		//	}
		}
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize, pClientSocket);
}
//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	m_bTimeOutCount=0;
	m_iNowOutPeople=-1;
	m_iFirstOutPeople=-1;
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	//��������
	for (int i=0;i<PLAY_COUNT;i++)
	{
		SetMing(i,false);
	}

	return __super::ResetGameFrame();
}
//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{

	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			if (uTimeCount<=0)
			{
				//ʱ�䵽��
				if (m_iAfterWaitAction == 1)
				{
					OnControlHitBegin();
				}
				else
				{
					SendMessage(WM_CLOSE,0,0);
				}
				return true;
			}
			return true;
		}

	case ID_OUT_CARD:	//����
		{
			int iViewStation=ViewStation(bDeskStation);
			if (uTimeCount<=0)	//ʱ�䵽��
			{
				if ((m_iNowOutPeople==GetMeUserInfo()->bDeskStation)&&(m_bWatchMode==FALSE))
				{
					AutoAI();    //�������Զ�����
				}
				return true;
			}
			return true;
		}
	}
	return true;
}

bool CClientGameDlg::AFCCloseFrame()
{

	return __super::AFCCloseFrame();
}

// �������Ǿ�̬������������Flash����ʹ�õĻص�����
void CClientGameDlg::FlashEndMessage(CWnd * pWnd,UINT uID,CString msgString)
{
}

//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	//��������
	m_iNowOutPeople=-1;
	m_iFirstOutPeople=-1;
	memset(m_Ming,0,sizeof(m_Ming));
	switch (iGameStation)
	{
	case RS_ALL_VIEW:
		//			KillAllTimer();
		for (int i=0;i<PLAY_COUNT;i++) 
		{
			SetMing(i,0);
		}
		::memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));
		::memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
		m_iThisNoteTimes=0;
		DisableAllOperaBtn();    
		m_bGameFinishAndComp = false;

	case RS_GAME_CUT:
		{
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			//��ʼ����ע
			SetTotalNote(0,0);
			break;
		}
	case RS_GAME_END:
		{
			DisableAllOperaBtn();
			break;
		}
	case RS_GAME_BEGIN:
		{
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			::memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
			::memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));
			m_iThisNoteTimes=0;	
			m_iFrontNote = 0;
			memset(m_bOpenLose,0,sizeof(m_bOpenLose));
		    memset(m_bIsGiveUp,0,sizeof(m_bIsGiveUp));
			m_iComWinStation = 255;
			m_bReachLimitStation = 255;
			//��ʼ����ע
			SetTotalNote(0,0);

			for (int i=0;i<PLAY_COUNT;i++) 
			{
				SetMing(i,0);
			}
			//���������
			DisableAllOperaBtn(); 
			m_bGameFinishAndComp = false;
			break;
		}
	}

	return __super::ResetGameStation(iGameStation);
}

//-----------------------------------------------------------
//��ʼ����ע
void CClientGameDlg::SetTotalNote(BYTE iViewStation,int iNoteTotal)
{
	m_iNoteTotalPoint=iNoteTotal;	
	return ;
}
//��ע
void CClientGameDlg::AddTotalNote(BYTE iViewStation,int iNoteTotal)
{
	m_iNoteTotalPoint += iNoteTotal;
	
	return ;
}

void CClientGameDlg::SetMing(BYTE bDeskStation,BOOL iGiveup)
{
	m_bMing[bDeskStation]=iGiveup;	
	return ;
}


/*

�����ƶ���
static const BYTE m_CardArray[54]={
0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
0x4E, 0x4F};																		//С�����

//flash �ƶ��� 0 - 51  ���� flash �е�  ��Ƭ A ��  ����  13
*/
int CClientGameDlg::GetFlashCard(int iCard)
{
	if (iCard == 0x4F)
	{
		return 54;
	}
	else if (iCard == 0x4E)
	{
		return 53;
	}
	int iHs = iCard / 16;
	int iPd = iCard % 16;

	if (iHs < 4 && iPd > 13)
	{
		return 52;   //Ϊ�Ʊ���
	}
	if (iPd < 13)
	{
		return iCard - iHs * 3;
	}
	else if (iPd == 13)
	{
		return iCard - 13 - iHs *3;
	}
	return 52;
}
int CClientGameDlg::GetTotalPlayNum()
{
	int iCount = 0;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			iCount++;
		}
	}
	return iCount;

}

BOOL CClientGameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	return __super::PreTranslateMessage(pMsg);
}
BOOL CClientGameDlg::OnInitDialog()
{
	BOOL bRet = __super::OnInitDialog();
	return bRet;
}

int CClientGameDlg::AnalyseNote(int iTotalNote,int arryHaveNote[],BYTE bAnayResult[])
{
	int total=0;    // �ܹ��ĳ�������
	::memset(bAnayResult,0,sizeof(bAnayResult));
	int iNoteCount = NOTE_NUM - 1;

	while(iTotalNote>0)
	{
		if(iTotalNote>=arryHaveNote[iNoteCount])
		{
			iTotalNote-=arryHaveNote[iNoteCount];
			bAnayResult[iNoteCount]++;
			total++;
		}
		else
		{
			iNoteCount--;
		}
	}
	return total;
}

void CClientGameDlg::PalyExtAddSound(int iSoundType)
{
	return;
}