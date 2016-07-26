#include "StdAfx.h"
#include "gamescreenwnd.h"
//#include "..\NewGameClient.h"

//����
//�����ڵ�
#define  TIMERIDSHOOT          1
//�ӵ�����
#define  TIMERIDBULLTE         2
//����ζ�����
#define  TIMERIDFISHMOVE       3
#define  TIMERIDNETOPEN        4
#define  TIMERIDFISHROUND      5
#define  TIMERIDMONEYACTION    6
//���Ǯ���ƶ�
#define  TIMERIDUSERMONEY      7
//�л���������
#define  TIMERIDCHANGESCREEN   8
//�Զ������ӵ�
#define  TIMERIDSENDSHOOT      9
//�Զ������ӵ�
#define  TIMERIDOUT            10
//�������ж�
#define  TIMERIDDING           11
//������ʱ��
#define	 TIME_SHAKE				12
//��ʾ�㲥
#define	 TIME_SHOW_MSG			13

/*---------------------------------------------------------------------------------------------------------------*/
//��ͼ��С
////�ζ�ͼ
const int GameScreenWnd::m_FishmoveCount[24] = {6,8,12,12,12,13,12,10,12,12,11,15,12,15,13,19,13,7,9,10,3,1,9,6};
////����ͼ
const int GameScreenWnd::m_FishdeadCount[24] = {2,2,2,3,3,3,4,3,2,4,6,4,3,3,3,3,3,0,2,2,0,0,3,0};
/*---------------------------------------------------------------------------------------------------------------*/
const int GameScreenWnd::m_FishmoveRec[24][2] = {{32,128},{32,64},{64,128},{64,128},{64,128},{128,128},{64,256},{128,256},{128,256},{128,256},{256,256},{256,256},{128,256},{256,256},{512,256},{256,512},{256,512},/*{256,256}*/{256,256},{256,512},{256,512},{512,512},{64,512}/*{512,256}*/,{512,1024},{512,256}/*{512,256}*/};
//��ͼ��С
const int GameScreenWnd::m_FishDeadRec[24][2] = {{32,128},{32,64},{64,128},{64,128},{64,128},{128,128},{128,256},{128,256},{128,256},{256,256},{256,256},{256,256},{128,256},{256,256},{512,256},{256,512},{256,512},/*{256,256}*/{512,512},{256,512},{256,512},{512,512},{64,512}/*{512,256}*/,{512,1024},{512,256}};
/*---------------------------------------------------------------------------------------------------------------*/
//���ɽ�Ҹ���
const int GameScreenWnd::m_iFishMoneyCount[24]	={	1/*0*/,1/*1*/,2/*2*/,2/*3*/,3/*4*/,3/*5*/,3/*6*/,4/*7*/,4/*8*/,
													4/*9*/,4/*10*/,5/*11*/,5/*12*/,5/*13*/,5/*14*/,5/*15*/,6/*16*/,
													6/*17*/,6/*18*/,6/*19*/,6/*20*/,6/*21*/,7/*22*/,7/*23*/};
/*---------------------------------------------------------------------------------------------------------------*/
//����λ��
const int G_iShakePosition[5][2] = {{0,0},{-10,5},{-10,10},{10,15},{15,10}};
/*---------------------------------------------------------------------------------------------------------------*/
//char G_szFishName[24][30] = {{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"�ڹ�"},
//								{"С��"},{"С��"},{"С��"},{"����"},{"ħ����"},{"������"},{"�����"},{"����"},{"��Ϻ"},{"������"},
//								{"����"},{"���"},{"���ͻƽ���"},{"�����"}};
char G_szFishName[24][30] = {	{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"�ڹ�"},
								{"С��"},{"С��"},{"С��"},{"����"},{"ħ����"},{"ը������"},{"ը���ƽ���"},{"�ƽ��"},{"˫ͷ�ĽŹ�"},{"�ƽ��ĽŹ�"},
								{"�����"},{"�𹿰�"},{"���ͻƽ���"},{"���"}};
/*---------------------------------------------------------------------------------------------------------------*/

GameScreenWnd::GameScreenWnd()
{
	_bMouseTrack=TRUE;
	//��ʼ����
	ZeroMemory(m_HaveUser,sizeof(m_HaveUser));
	//ZeroMemory(m_UserPaoJiaodu,sizeof(m_UserPaoJiaodu));
	ZeroMemory(m_UserSendstate,sizeof(m_UserSendstate));
	ZeroMemory(m_UserScore,sizeof(m_UserScore));
	ZeroMemory(m_MuchmoneyAct,sizeof(m_MuchmoneyAct));
	ZeroMemory(m_IsMouseOver,sizeof(m_IsMouseOver));
	ZeroMemory(m_BeiLv,sizeof(m_BeiLv));
	ZeroMemory(m_UserScore,sizeof(m_UserScore));
	ZeroMemory(m_UserSuperPao,sizeof(m_UserSuperPao));
	m_bHaveLinked = false;
	m_DingStruc.m_have = false;
	//�Ѿ���������
	m_HaveLoadNum = 0;
	m_InitScreen= false;
	m_Bgindex = 0;
	m_ActionIndex = 0;
	m_ActionIndex1 = 0;
	m_SoundIndex =0;
	m_IsMustOut = false;
	m_IsDingzhu = false;
	//��ȡ��ǰϵͳ����ʱ��
	m_NowTime =  GetTickCount();
	//�Ƿ��л�����
	m_IsChangeScreen  =  false;

    //��������
	for(int i=0;i<6;i++)
	{
	   m_UserPaoJiaodu[i]=0;
	   if(i>=3)m_UserPaoJiaodu[i]=180;
       m_UserScore[i]=0;
	   m_BeiLv[i] =  0;
	   for(int j=0;j<3;j++)
	   {
		   m_UserStruct[i][j].m_Have = false;
		   m_UserStruct[i][j].m_Money = 0;
		   m_UserStruct[i][j].m_Time = 0;
	   }
	}
   
	//�������
	m_UserPT[0].x  =  110;
	m_UserPT[0].y  =  560;
	m_UserPT[1].x  =  530;
	m_UserPT[1].y  =  560;
	m_UserPT[2].x  =  950;
	m_UserPT[2].y  =  560;
	m_UserPT[5].x  =  110;
	m_UserPT[5].y  =  -20;
	m_UserPT[4].x  =  530;
	m_UserPT[4].y  =  -20;
	m_UserPT[3].x  =  950;
	m_UserPT[3].y  =  -20;

   //��������
	CopyMemory(m_UserPaoPT,m_UserPT,sizeof(m_UserPaoPT));

	//�Լ���λ��,�����л���
	m_MeChariID =2;
	m_i64MyMoney = 888888888;
	m_OutTime = 120;
	
	//��ʼ����
	for(int i=0;i<100;i++)
	{
	    m_MeBullet[i].m_Have=false;
		m_OterBullet[i].m_Have=false;
		m_NumArr[i].m_Have = false;
		m_SoundArr[i].m_Have = false;
		m_GoldArr[i].m_Have=false;
	}

	//��������
	m_iShakex = 0;
	m_iShakey = 0;
	m_iShakeCount = 0;

	m_iMoveY  = 0;
	m_iColorR = 255;
	m_iColorB = 10;
	m_iColorG = 10;


}

GameScreenWnd::~GameScreenWnd(void)
{
}
/*---------------------------------------------------------*/
//��ÿͻ��˽ӿ�ָ��
void GameScreenWnd::SetGameClientP(CClientGameDlg *p)
{
//	m_pGameClient = p;
}
/*---------------------------------------------------------*/
//��Ⱦ�߳�
void DxThreadRender(LPVOID pThreadData)
{
	//Ч�����
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;

	//����ת��
	GameScreenWnd *pDxDialog=(GameScreenWnd *)pThreadData;

	//����cpuʹ����
	while(NULL!=pDxDialog && pDxDialog->GetSafeHwnd()!=NULL&&pDxDialog->m_RunRender)// && pDxDialog->m_bD3DSucess)
	{
		Sleep(10);
	}
	return;
}
/*---------------------------------------------------------*/
//��Ⱦ�߳�
void DxThreadRender1(LPVOID pThreadData)
{
	//Ч�����
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;

	//����ת��
	GameScreenWnd *pDxDialog=(GameScreenWnd *)pThreadData;

	//����cpuʹ����
	while(NULL!=pDxDialog && pDxDialog->GetSafeHwnd()!=NULL&&pDxDialog->m_RunRender)// && pDxDialog->m_bD3DSucess)
	{
		
		pDxDialog->CheckHit();
		Sleep(10);
	}

	return;
}
/*---------------------------------------------------------*/
//�����߳�
void DxThreadLoad(LPVOID pThreadData)
{
	//Ч�����
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;

	//����ת��
	GameScreenWnd *pDxDialog=(GameScreenWnd *)pThreadData;


	return;
}
/*---------------------------------------------------------*/

/*---------------------------------------------------------------------*/
//��ʼ����
bool	GameScreenWnd::InitDxDlog()
{
	OutputDebugString("dwjdsnh::InitDxDlog-0");
	m_RunRender = true;

	//SetTimer(TIMERIDOUT,1000,NULL);

	return true;
}
/*---------------------------------------------------------------------*/


//����DX���ڼ�������Դ����ͼƬ
HRESULT GameScreenWnd::InitD3D( HWND hWnd )
{
	return S_OK;

}

//�����ӵ�
void GameScreenWnd::UserShoot(int PlayerID,float Roation,int RealChairID,bool isAndroid)
{
	if(m_InitScreen==false)return;

	if(PlayerID == m_MeChariID)
	{
		if(m_UserScore[m_MeChariID]- (__int64)m_BeiLv[m_MeChariID]<0)
			return;
	}

	SetTimer(TIMERIDSHOOT,100,NULL);
	
	
	//�������Ϊ����״̬
	m_UserSendstate[PlayerID] = true;
	//������λ��
	m_UserPaoPT[PlayerID] = m_UserPT[PlayerID];
	float jiaodu =  (int(Roation)+360)%360;
	if(PlayerID>=3)
	{
	  if(jiaodu>=270){jiaodu = 90+jiaodu-270;}
	  else if(jiaodu<=90){jiaodu = 180+jiaodu;}
	}
	m_UserPaoJiaodu[PlayerID] = jiaodu;

    //�жϷ������m,������Լ�
	if(PlayerID == m_MeChariID)
	{
		if(m_UserScore[m_MeChariID]-m_BeiLv[m_MeChariID]<0)return;
		m_OutTime = 120;
	   //�����ӵ�
		for(int i=0;i<100;i++)
		{
		  if(m_MeBullet[i].m_Have==true)continue;
		  m_MeBullet[i].m_isThree = false;
		  m_MeBullet[i].m_issuper = false;
		  //����ǳ����ھ������ӵ�Ϊ�����ӵ�
		  if(m_UserSuperPao[PlayerID])m_MeBullet[i].m_issuper = true;
		  //�������500���ʾ���3���ӵ�
		  if(m_BeiLv[PlayerID]>500)m_MeBullet[i].m_isThree=true;
	  	  m_MeBullet[i].m_ptx = m_UserPT[PlayerID].x+70;
		  m_MeBullet[i].m_pty = m_UserPT[PlayerID].y+70;
		  m_MeBullet[i].m_Have = true;
		  m_MeBullet[i].m_Stop = false;
		  m_MeBullet[i].m_Netindex = 0;
		  m_MeBullet[i].m_Senduser = PlayerID;
		  m_MeBullet[i].m_Roation = jiaodu;
		  m_MeBullet[i].m_isAndroid = false;
		  m_MeBullet[i].m_RealUser = RealChairID;

		  break;
		}
	}
	else
	{
	  
	   //�����ӵ�
		for(int i=100;i<200;i++)
		{
		  if(m_MeBullet[i].m_Have==true)continue;
		  m_MeBullet[i].m_isThree = false;
		  m_MeBullet[i].m_issuper = false;
		  //����ǳ����ھ������ӵ�Ϊ�����ӵ�
		  if(m_UserSuperPao[PlayerID])m_MeBullet[i].m_issuper = true;
		  //�������500���ʾ���3���ӵ�
		  if(m_BeiLv[PlayerID]>500)m_MeBullet[i].m_isThree=true;
	  	  m_MeBullet[i].m_ptx = m_UserPT[PlayerID].x+70;
		  m_MeBullet[i].m_pty = m_UserPT[PlayerID].y+70;
		  m_MeBullet[i].m_Have = true;
		  m_MeBullet[i].m_Stop = false;
		  m_MeBullet[i].m_Netindex = 0;
		  m_MeBullet[i].m_Senduser = PlayerID;
		  m_MeBullet[i].m_Roation = jiaodu;
		  m_MeBullet[i].m_isAndroid = isAndroid;
		  m_MeBullet[i].m_RealUser = RealChairID;
		  
		 // if()
		  //if(PlayerID>=3)m_MeBullet[i].m_Roation= Roation+180;
		  break;
		}
	
	}

}

/*-----------------------------------------------------------------------------*/
//������㲥
void	GameScreenWnd::CatchBroadCast(int iDeskNo,char szNickName[],char szRoomName[],int iFishType,__int64 i64FishScore)
{

	BroadCastData TBroadCast;
	TBroadCast.bHave = true;
	TBroadCast.iDeskNo = iDeskNo;
	TBroadCast.i64FishScore = i64FishScore;
	memcpy(TBroadCast.szNickName,szNickName,sizeof(TBroadCast.szNickName));
	memcpy(TBroadCast.szRoomName,szRoomName,sizeof(TBroadCast.szRoomName));

	//�������
	memcpy(TBroadCast.szFishName,G_szFishName[iFishType],sizeof(TBroadCast.szFishName));

	
}
/*-----------------------------------------------------------------------------*/
//��ס
void GameScreenWnd::SetDingAction(Fish *tfish)
{
    m_IsDingzhu =  true;
	m_DingStruc.m_have =  true;
	m_DingStruc.m_nowindex = 0;
	m_DingStruc.m_ptx = tfish->m_ptx + m_FishmoveRec[tfish->m_index][0]/2-1024/2;
	m_DingStruc.m_pty = tfish->m_pty + m_FishmoveRec[tfish->m_index][1]/2-512/2;

	SetTimer(TIMERIDDING,5000,NULL);

}
//ը����ը
void GameScreenWnd::SetBomAction(int PlayerId,int Fishid,int fishscore)
{
	INT_PTR nEnumIndex=0;
	Fish *tfish=NULL;
    do{
		tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		if (NULL==tfish) break;
		if(GetTickCount()-tfish->m_creatTime<10000)
		   UserAddMoney(PlayerId,tfish->m_fishID,fishscore,0,false);

	}while(true);
}
//��Ǯ�û�
void GameScreenWnd::UserAddMoney(int PlayerID,int Fishid,int Money,int FishKindScord,bool ishavaction )
{
	if(m_InitScreen==false)return;
	int Fishindx = -1;
	INT_PTR nEnumIndex=0;
	Fish *Deadfish=NULL;
	Fish *tfish=NULL;
	do{
		tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
		if (NULL==tfish) break;
		if(tfish->m_fishID==Fishid)
		{
		  Deadfish = tfish;
		  break;
		}
	}while(true);
	
	if(Deadfish==NULL)return;
	//�Ƿ��ǰ�����Ҫ��ס
	if(Deadfish->m_index==21) 
	{
	   SetDingAction(Deadfish);
	}



	//���������������״̬
	//m_FishArr[j].m_DeadMoney = 1000;
	Deadfish->m_HitUser = PlayerID;
	Deadfish->m_isHit = true;
	Deadfish->m_actionindex = 0;
	Deadfish->m_Nowindex = 0;
	//���ֻ���������� ��ֱ�ӷ���
	if(FishKindScord<=0)return;
	//�������
    //��������
    char musicPath[32]={0};
	int musciindex= rand()%20;
	

	 //��ʾ��ķ���
	 for(int ci=0;ci<100;ci++)
		 {
			 if(m_NumArr[ci].m_Have==true)continue;
			 m_NumArr[ci].m_Have = true;
			 m_NumArr[ci].m_ptx =  Deadfish->m_ptx + m_FishmoveRec[Deadfish->m_index][0]/2;
			 m_NumArr[ci].m_pty =  Deadfish->m_pty + m_FishmoveRec[Deadfish->m_index][1]/2;
			 m_NumArr[ci].m_Time = 0;
			 m_NumArr[ci].m_Numcount = Money;
			 m_NumArr[ci].m_HitUser=PlayerID;
			 m_NumArr[ci].m_beishu = 1.4f;
			 if(Money>=10000)m_NumArr[ci].m_beishu = 2.0f;
			 break;

		 }
						
	//����Ʈ����
	 int iCount =0;
	 for(int ci=0;ci<100;ci++)
	 {   
		 //����Ѿ�ռ��ѭ����ȥ
		 if(m_GoldArr[ci].m_Have==true)continue;
		 m_GoldArr[ci].m_Have = true;
		 m_GoldArr[ci].m_Player = PlayerID;
		 m_GoldArr[ci].m_actionindex =0;
		 m_GoldArr[ci].m_ptx =  Deadfish->m_ptx + m_FishmoveRec[Deadfish->m_index][0]/2 + (iCount*82.0);
		 m_GoldArr[ci].m_pty =  Deadfish->m_pty + m_FishmoveRec[Deadfish->m_index][1]/2;

		 //��Ƕ�
		 float px = m_GoldArr[ci].m_ptx-m_UserPT[PlayerID].x-100;
		 float py = m_UserPT[PlayerID].y-m_GoldArr[ci].m_pty+100;
		 float bili = float(px/py);
		 float jiaodu=atan2(px,py)*180.0/3.1415926;
		 m_GoldArr[ci].m_Roation=jiaodu;

		 iCount++;
		 if(iCount >= m_iFishMoneyCount[Deadfish->m_index])
		 {
			 break;
		 }
	 }


    m_UserScore[PlayerID] =m_UserScore[PlayerID] + Money;
	if(!m_UserStruct[PlayerID][0].m_Have)
	{
		m_UserStruct[PlayerID][0].m_Have=true;
		m_UserStruct[PlayerID][0].m_Money = Money;
		m_UserStruct[PlayerID][0].m_Time = 0;
		m_UserStruct[PlayerID][0].m_FishScore = FishKindScord;
		m_UserStruct[PlayerID][0].m_Nowscore = 0;


	}
	else if(m_UserStruct[PlayerID][0].m_Have&&!m_UserStruct[PlayerID][1].m_Have)
	{
		m_UserStruct[PlayerID][1].m_Have=true;
		m_UserStruct[PlayerID][1].m_Money = Money;
		m_UserStruct[PlayerID][1].m_Time = 0;
		m_UserStruct[PlayerID][1].m_FishScore = FishKindScord;
		m_UserStruct[PlayerID][1].m_Nowscore = 0;
	}
	else if(m_UserStruct[PlayerID][0].m_Have&&m_UserStruct[PlayerID][1].m_Have&&!m_UserStruct[PlayerID][2].m_Have)
	{
		m_UserStruct[PlayerID][2].m_Have=true;
		m_UserStruct[PlayerID][2].m_Money = Money;
		m_UserStruct[PlayerID][2].m_Time = 0;
		m_UserStruct[PlayerID][2].m_FishScore = FishKindScord;
		m_UserStruct[PlayerID][2].m_Nowscore = 0;
	}
	else
	{
		m_UserStruct[PlayerID][0].m_Have =  m_UserStruct[PlayerID][1].m_Have;
		m_UserStruct[PlayerID][0].m_Money =  m_UserStruct[PlayerID][1].m_Money;
		m_UserStruct[PlayerID][0].m_Time =  0;
		m_UserStruct[PlayerID][0].m_FishScore = m_UserStruct[PlayerID][1].m_FishScore;
		m_UserStruct[PlayerID][0].m_Nowscore = m_UserStruct[PlayerID][1].m_Nowscore;
        m_UserStruct[PlayerID][1].m_Have =  m_UserStruct[PlayerID][2].m_Have;
		m_UserStruct[PlayerID][1].m_Money =  m_UserStruct[PlayerID][2].m_Money;
		m_UserStruct[PlayerID][1].m_FishScore = m_UserStruct[PlayerID][2].m_FishScore;
		m_UserStruct[PlayerID][1].m_Nowscore = m_UserStruct[PlayerID][2].m_Nowscore;
		m_UserStruct[PlayerID][1].m_Time =  0;
		m_UserStruct[PlayerID][2].m_Have=true;
		m_UserStruct[PlayerID][2].m_Money = Money;
		m_UserStruct[PlayerID][2].m_Time = 0;
		m_UserStruct[PlayerID][2].m_FishScore = FishKindScord;
		m_UserStruct[PlayerID][2].m_Nowscore = 0;
	}

	
}
//�л�����
void GameScreenWnd::ChangeScreen(int bgindex)
{


	m_ChanwaveSt.m_currImgIndex = 0;
	m_ChanwaveSt.m_ptx = 1280;
	m_ChanwaveSt.m_pty = 0;
	m_ChanwaveSt.m_Time = 0;
	m_IsChangeScreen = true;
	m_Bgindex = bgindex;
	if(m_Bgindex>=3)m_Bgindex=0;
	//����������
	SetTimer(TIMERIDCHANGESCREEN,50,NULL);
	//������ʧ
	Fish *tfish=NULL;
	for(int i=0;i<m_FishFactory.GetActiveCount();i++)
	{
		//ö����Ⱥ
		tfish=m_FishFactory.EnumActiveObject(i);
		if (NULL==tfish) break;
		//�����ж�
		if(tfish->m_Have==false)continue;
		if(tfish->m_isHit==true)continue;
		tfish->m_speed  = 100;
	}  
}


/*----------------------------------------------------------------------------------*/
//��Ϣӳ��
BEGIN_MESSAGE_MAP(GameScreenWnd, CWnd)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()
/*----------------------------------------------------------------------------------*/
//������ ��ý���
void	GameScreenWnd::OnMouseHover(UINT, CPoint)
{
	this->SetFocus();
}
/*----------------------------------------------------------------------------------*/
//����뿪 ʧȥ����
void	GameScreenWnd::OnMouseLeave()
{
	_bMouseTrack = TRUE;
	m_bMoveWindow = false;
	KillTimer(TIMERIDSENDSHOOT);
}
/*----------------------------------------------------------------------------------*/
void	GameScreenWnd::OnMouseMove(UINT, CPoint point)
{
	this->SetFocus();
	//�������׷��
	if (_bMouseTrack)     // ������ ׷�٣��� 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd ;		// ָ��Ҫ ׷�� �Ĵ��� 
		csTME.dwHoverTime = 5;			// ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent (&csTME);	// ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧��
		_bMouseTrack=FALSE ;			// ���Ѿ� ׷�� ����ֹͣ ׷�� 
	}

	if(m_bMoveWindow)
	{
//		m_pGameClient->OnMoveWindow(point);
		return;
	}
	m_configDlg.LMouseMove(point.x,point.y);

	//SetCursor(LoadCursor((HINSTANCE)GetWindowLong(this->GetSafeHwnd(),GWL_HINSTANCE),MAKEINTRESOURCE(IDC_CURSOR1)));
	SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));


	if(point.y>710||point.y<30||point.x<5||point.x>1275||m_configDlg.m_isvisable)
	{
		
	}
	else 
	{
		if(m_InitScreen)
		{
			m_cursor.m_have = true;
			m_cursor.m_ptx = point.x-28;
			m_cursor.m_pty = point.y-28;
		}
	}


	//�����Լ��ڵ��ƶ��Ƕ�
	float px=point.x-m_UserPT[m_MeChariID].x-100;
	float py=m_UserPT[m_MeChariID].y-point.y+100;
	float bili = float(px/py);
	float jiaodu=atan2(px,py)*180.0/3.1415926;
	if(jiaodu<-90)jiaodu=-90;
	if(jiaodu>90)jiaodu=90;
	if(jiaodu<0)jiaodu=360+jiaodu;
	m_UserPaoJiaodu[m_MeChariID]=jiaodu;

	for(int i=0;i<5;i++)
	{
		m_IsMouseOver[i]=false;
	}

}
/*----------------------------------------------------------------------------------*/
void	GameScreenWnd::OnLButtonDown(UINT, CPoint point)
{
	m_configDlg.LMouseDown(point.x,point.y);
	if(m_configDlg.m_isvisable)
	{
		return ;
	}
	//�ƶ�����
	if(point.y<30)
	{
		/*POINT pMouse;
		CPoint cpoint;
		if(GetCursorPos(&pMouse))
		{
			cpoint.x = pMouse.x;
			cpoint.y = pMouse.y;
		}
		else
		{
			cpoint = point;
		}*/
//		m_pGameClient->OnLeftButtonDown(point);
		m_bMoveWindow = true;
	}

	if(point.y>30&&point.y<710&&m_InitScreen)
	{
		//����
		if(GetTickCount() - m_NowTime > 400 )
		{
			//���ûǮֱ�ӵ���ȥ
			if(m_UserScore[m_MeChariID]<=0)
			{
				return ;
			}
			if(m_BeiLv[m_MeChariID]>m_UserScore[m_MeChariID])
			{
				return ; 
			}
//			m_pGameClient->OnUserShot(false,m_UserPaoJiaodu[m_MeChariID]);
			UserShoot(m_MeChariID,m_UserPaoJiaodu[m_MeChariID],m_MeRellayChairID,false);
			m_NowTime = GetTickCount() ;

		}
		SetTimer(TIMERIDSENDSHOOT,200,NULL);
		//����Ϊ��갴��״̬
		m_MouseDown = true;
	}
	//��Ť�¼�

	{
		//�Ϸ�
//		m_pGameClient->AddOrRemoveScore(true,m_BeiLv[m_MeChariID]);
	
	}

	{
		//�·�
//		m_pGameClient->AddOrRemoveScore(false,m_BeiLv[m_MeChariID]);
			
	}
	
}
/*----------------------------------------------------------------------------------*/
void	GameScreenWnd::OnLButtonUp(UINT, CPoint ponit)
{
	m_configDlg.LMouseUp(ponit.x,ponit.y);
	//�������״̬Ϊ�ɿ�
	m_MouseDown =  false;
	m_bMoveWindow = false;
	//ֹͣ�ӵ�����
	KillTimer(TIMERIDSENDSHOOT);
}
/*----------------------------------------------------------------------------------*/
void	GameScreenWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(m_configDlg.m_isvisable)
	{
		return ;
	}
	
	switch( (WCHAR)nChar )
	{
	case VK_SPACE://�ո���
		{
			if(GetTickCount() - m_NowTime > 190 )
			{
				//���ûǮֱ�ӵ���ȥ
				if(m_UserScore[m_MeChariID]<=0)break;
				if(m_BeiLv[m_MeChariID]>m_UserScore[m_MeChariID]) return ; //edit by guojm
//				m_pGameClient->OnUserShot(false,m_UserPaoJiaodu[m_MeChariID]);
				UserShoot(m_MeChariID,m_UserPaoJiaodu[m_MeChariID],m_MeRellayChairID,false);
				m_NowTime = GetTickCount() ;
				KillTimer(TIMERIDSENDSHOOT);
			}
			break;
		}
	case VK_UP://�ϼ�����
		{
//			m_pGameClient->OnUserAddFire(true);
			break;
		}
	case VK_DOWN://�¼���
		{
//			m_pGameClient->OnUserAddFire(false);
			break;
		}
	case VK_LEFT://���Ϸ�
		{
//			m_pGameClient->AddOrRemoveScore(true,m_BeiLv[m_MeChariID]);
		
			break;
		}
	case VK_RIGHT://���·�
		{
//			m_pGameClient->AddOrRemoveScore(false,m_BeiLv[m_MeChariID]);
			
			break;
		}
	case VK_ESCAPE:
		{
			return ;
		}
	}		
}
/*----------------------------------------------------------------------------------*/
//���̵���
void	GameScreenWnd::OnkeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}
/*----------------------------------------------------------------------------------*/
//ʱ���¼�
void GameScreenWnd::OnGameTimer(UINT nIDEvent)
{
	
	if(TIME_SHAKE == nIDEvent)
	{
		m_iShakeCount++;
		if(m_iShakeCount > 15)
		{
			m_iShakex = 0;
			m_iShakey = 0;
			m_iShakeCount = 0;
			KillTimer(TIME_SHAKE);
		}
		else
		{
			m_iShakex = G_iShakePosition[m_iShakeCount%5][0];
			m_iShakey = G_iShakePosition[m_iShakeCount%5][1];
		}
	}
	else if(TIMERIDSHOOT==nIDEvent)
	{   
		for(int i= 0 ;i < 6;i++)
		{
			if(m_UserSendstate[i])
			{
				float m_x = 7*sin(2*3.1415926*(m_UserPaoJiaodu[i])/360.0);
				float m_y = 7*cos(2*3.1415926*(m_UserPaoJiaodu[i])/360.0);
				m_UserPaoPT[i].y =m_UserPaoPT[i].y+m_y;
				m_UserPaoPT[i].x =m_UserPaoPT[i].x-m_x;
				m_UserSendstate[i]=false;

			}
			else 
			{
				m_UserPaoPT[i].y =m_UserPT[i].y;
				m_UserPaoPT[i].x =m_UserPT[i].x;
				//KillTimer(TIMERIDSHOOT);
			}
		}
	}
	else if(TIMERIDBULLTE==nIDEvent)
	{
		//�ƶ��Լ����ӵ�
		for(int i=0;i<200;i++)
		{
			if(m_MeBullet[i].m_Have==false)continue;
			if(m_MeBullet[i].m_Stop==true)continue;		 

			//�жϱ�Ե
			if(m_MeBullet[i].m_ptx<0)//��߳���
			{
				m_MeBullet[i].m_Roation = 360-m_MeBullet[i].m_Roation;
			}
			if(m_MeBullet[i].m_ptx+64>CLIENT_WIDTH)//�ұ߳���
			{
				m_MeBullet[i].m_Roation = 360-m_MeBullet[i].m_Roation;
			}
			if(m_MeBullet[i].m_pty+64>CLIENT_HEIGHT)//�ײ�����
			{
				m_MeBullet[i].m_Roation = 180-m_MeBullet[i].m_Roation;
			}
			if(m_MeBullet[i].m_pty<0)//��������
			{
				m_MeBullet[i].m_Roation = 180-m_MeBullet[i].m_Roation;
			}

			if(m_MeBullet[i].m_ptx<0&&m_MeBullet[i].m_pty<0)
			{
				m_MeBullet[i].m_Roation = 135;
				m_MeBullet[i].m_ptx = 20;
				m_MeBullet[i].m_pty = 20;
			}
			else if(m_MeBullet[i].m_ptx<0&&m_MeBullet[i].m_pty>CLIENT_HEIGHT)
			{
				m_MeBullet[i].m_Roation = 45;
				m_MeBullet[i].m_ptx = 20;
				m_MeBullet[i].m_pty = 720;
			}
			else if(m_MeBullet[i].m_ptx>CLIENT_WIDTH&&m_MeBullet[i].m_pty>CLIENT_HEIGHT)
			{
				m_MeBullet[i].m_Roation = 315;
				m_MeBullet[i].m_ptx = 1260;
				m_MeBullet[i].m_pty = 720;
			}
			else if(m_MeBullet[i].m_ptx>CLIENT_WIDTH&&m_MeBullet[i].m_pty<0)
			{
				m_MeBullet[i].m_Roation = 225;
				m_MeBullet[i].m_ptx = 1260;
				m_MeBullet[i].m_pty = 20;
			}

			float m_x = 0.0;
			float m_y = 0.0;
			if(m_MeBullet[i].m_issuper)
			{
				m_x = 14*sin(2*3.1415926*(m_MeBullet[i].m_Roation)/360.0);
				m_y = 14*cos(2*3.1415926*(m_MeBullet[i].m_Roation)/360.0);
			}
			else
			{
				m_x = 10*sin(2*3.1415926*(m_MeBullet[i].m_Roation)/360.0);
				m_y = 10*cos(2*3.1415926*(m_MeBullet[i].m_Roation)/360.0);
			}
			
			m_MeBullet[i].m_ptx = m_MeBullet[i].m_ptx + m_x;
			m_MeBullet[i].m_pty = m_MeBullet[i].m_pty - m_y;

		}
		//�ƶ��������
		Fish *tfish=NULL;
		for(int i=0;i<m_FishFactory.GetActiveCount();i++)
		{
			//ö����Ⱥ
			if(m_IsDingzhu)break;//���Ŀǰ��סֱ�ӵ���ȥ
			tfish=m_FishFactory.EnumActiveObject(i);
			if (NULL==tfish) break;
			//�����ж�
			if(tfish->m_Have==false)continue;
			if(tfish->m_isHit==true)continue;
			float m_x = tfish->m_speed*sin(2*3.1415926*(tfish->m_Roation)/360.0)/3.5;
			float m_y = tfish->m_speed*cos(2*3.1415926*(tfish->m_Roation)/360.0)/3.5;
			tfish->m_ptx = tfish->m_ptx + m_x;
			tfish->m_pty = tfish->m_pty - m_y;
		}
		//Ʈ���������ƶ�
		for(int i=0;i<100;i++)
		{
			if(m_GoldArr[i].m_Have==false)continue;
			float m_x = 12*sin(2*3.1415926*(m_GoldArr[i].m_Roation)/360.0)/4;
			float m_y = 12*cos(2*3.1415926*(m_GoldArr[i].m_Roation)/360.0)/4;
			m_GoldArr[i].m_ptx = m_GoldArr[i].m_ptx - m_x;
			m_GoldArr[i].m_pty = m_GoldArr[i].m_pty + m_y;

			//�ж�
			CRect m_rct;
			m_rct.top = m_UserPT[m_GoldArr[i].m_Player].y+50;
			m_rct.left = m_UserPT[m_GoldArr[i].m_Player].x+50;
			m_rct.right =  m_rct.left + 100;
			m_rct.bottom =  m_rct.top + 100;
			m_GoldArr[i].m_Point.x = m_GoldArr[i].m_ptx;
			m_GoldArr[i].m_Point.y = m_GoldArr[i].m_pty;
			bool is_hit =  m_rct.PtInRect(m_GoldArr[i].m_Point);
			if(is_hit||m_GoldArr[i].m_ptx<0||m_GoldArr[i].m_ptx>1280||m_GoldArr[i].m_pty<0||m_GoldArr[i].m_pty>740)
				m_GoldArr[i].m_Have = false;
		}
	}
	//������������л�
	else if(TIMERIDFISHMOVE==nIDEvent)
	{
		OutputDebugString("dwjdsnh::TIMERIDFISHMOVE-0");
		//���ﴦ��ȫ�ֶ�������
		m_ActionIndex ++ ;
		if(m_ActionIndex>100000000)m_ActionIndex = 0;
		INT_PTR nEnumIndex=0;
		Fish *tfish=NULL;
		do
		{
			tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
			//ö����Ⱥ
			if (NULL==tfish) break;
			if(tfish->m_Have==false)continue;
			if(tfish->m_isHit)
			{
				tfish->m_Nowindex++;
				if(tfish->m_Nowindex>=5)
				{
					tfish->m_actionindex++;
					if(tfish->m_actionindex>=m_FishdeadCount[tfish->m_index]*3)tfish->m_Have = false;
				}
			}
			else
			{
				if(!m_IsDingzhu)
				{
					tfish->m_actionindex++;
					if(tfish->m_actionindex>=m_FishmoveCount[tfish->m_index])tfish->m_actionindex=0;
				}
			}
		}while(true);
	}
	//չ���ӵ�����
	else if(nIDEvent == TIMERIDNETOPEN)
	{
		//�ƶ��Լ����ӵ�
		for(int i=0;i<200;i++)
		{
			if(m_MeBullet[i].m_Have==false)continue;
			if(m_MeBullet[i].m_Stop==false)continue;
			m_MeBullet[i].m_Netindex++;
			if(m_MeBullet[i].m_Netindex>=19){m_MeBullet[i].m_Have = false;}
		}
		//����Ʈ����
		for(int i=0;i<100;i++)
		{
			if(m_GoldArr[i].m_Have==false)continue;
			m_GoldArr[i].m_actionindex++;
			if(m_GoldArr[i].m_actionindex >= 13){ m_GoldArr[i].m_actionindex = 0;}
		}
	}
	//�����ת���Ƕȶ���
	else if(nIDEvent == TIMERIDFISHROUND)
	{
		m_ActionIndex1 ++;
		if(m_ActionIndex1>100000000)m_ActionIndex1 = 0;
		//ѭ����
		Fish *tfish=NULL;
		for(int i=0;i<m_FishFactory.GetActiveCount();i++)
		{
			//ö����Ⱥ
			if(m_IsDingzhu)break;//���Ŀǰ��סֱ�ӵ���ȥ
			tfish=m_FishFactory.EnumActiveObject(i);
			if (NULL==tfish) break;
			if(tfish->m_Have==false)continue;
			if(tfish->m_isHit==true)continue;

			tfish->m_Currtime = tfish->m_Currtime+ 1;
			if(tfish->m_Currtime>=m_FishTrace[tfish->m_traceIndex][tfish->m_PtIndex].movetime)
			{
				tfish->m_PtIndex++;
				tfish->m_Currtime = 0;
				tfish->m_speed = m_FishTrace[tfish->m_traceIndex][tfish->m_PtIndex].m_speed;
			}
			if(tfish->m_Roation!=m_FishTrace[tfish->m_traceIndex][tfish->m_PtIndex].rotation)
			{
				if(tfish->m_Roation>270&&m_FishTrace[tfish->m_traceIndex][tfish->m_PtIndex].rotation<90)
				{
					m_FishTrace[tfish->m_traceIndex][tfish->m_PtIndex].rotation = 360 + m_FishTrace[tfish->m_traceIndex][tfish->m_PtIndex].rotation;
				}

				float tempr = (m_FishTrace[tfish->m_traceIndex][tfish->m_PtIndex].rotation - tfish->m_Roation) / 50;
				tfish->m_Roation = tfish->m_Roation +tempr;
			}
			//������и��������תȦ������ô��ִ������Ĳ���		     
		}
		for(int ci=0;ci<100;ci++)
		{   
			//����Ѿ�ռ��ѭ����ȥ
			if(m_NumArr[ci].m_Have==false)continue;
			m_NumArr[ci].m_Time++;
			if(m_NumArr[ci].m_Time>=150)
			{
				m_NumArr[ci].m_Have=false;
			}
		}
		for(int i=0;i<PLAY_COUNT;i++)
		{
			//�ж��Ƿ������
			if(!m_HaveUser[i])continue;
			for(int j=0;j<3;j++)
			{
				if(!m_UserStruct[i][j].m_Have)continue;
				if(m_UserStruct[i][j].m_Nowscore<m_UserStruct[i][j].m_FishScore)
					m_UserStruct[i][j].m_Nowscore++;
			}
		}
	}
	//��������תȦ����Ǯ����
	else if(nIDEvent == TIMERIDMONEYACTION)
	{
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_MuchmoneyAct[i].m_have==false)continue;
			m_MuchmoneyAct[i].m_nowindex++;
			//������д���21��ת���ɸ���ͼƬ�Ƕ�
			if(m_MuchmoneyAct[i].m_nowindex>21)
			{
				m_MuchmoneyAct[i].m_Roation = m_MuchmoneyAct[i].m_Roation + 30 ;
			}
			//������д�����100��ô���л�Ϊ����ʾ��
			if(m_MuchmoneyAct[i].m_nowindex>90)
			{
				m_MuchmoneyAct[i].m_have=false;
			}			
		}
		if(m_DingStruc.m_have == true)
		{
			m_DingStruc.m_nowindex++;
			if(m_DingStruc.m_nowindex>60)m_DingStruc.m_have=false;
		}


		
	}
	//���Ǯ���ƶ��������Աߴ���������3��Ǯ
	else if(nIDEvent == TIMERIDUSERMONEY)
	{
		
	}
	//�����л�����
	else if(TIMERIDCHANGESCREEN==nIDEvent)
	{

		m_ChanwaveSt.m_currImgIndex = 1-m_ChanwaveSt.m_currImgIndex;
		m_ChanwaveSt.m_ptx = m_ChanwaveSt.m_ptx-20;
		m_ChanwaveSt.m_pty = 0;
		m_ChanwaveSt.m_Time++;
		if(m_ChanwaveSt.m_ptx<-500)
		{
			//����Ѿ�������Ե��ôֹͣ����
			m_IsChangeScreen = false;
			KillTimer(TIMERIDCHANGESCREEN);
		}
	}
	//�����ڵ�
	else if(TIMERIDSENDSHOOT==nIDEvent)
	{
		//�����ӵ�
		if(m_UserScore[m_MeChariID]>0&&m_BeiLv[m_MeChariID]<m_UserScore[m_MeChariID])  //edit by guojm)
		{
//			m_pGameClient->OnUserShot(false,m_UserPaoJiaodu[m_MeChariID]);
			UserShoot(m_MeChariID,m_UserPaoJiaodu[m_MeChariID],m_MeRellayChairID,false);
		}
		else
		{
			KillTimer(TIMERIDSENDSHOOT);
		}
	}
	else if(TIMERIDOUT==nIDEvent)
	{
		m_OutTime--;
		if(m_OutTime<=60)
		{
			m_IsMustOut =  true;
		}
		else
		{
			m_IsMustOut =  false;
		}
		//�˳�
		if( m_OutTime<=0)
		{
//			m_pGameClient->OnCloseWindow();
		}
	}
	else if(TIMERIDDING==nIDEvent)
	{
		m_IsDingzhu =  false;
		KillTimer(TIMERIDDING);
	}

	CWnd::OnTimer(nIDEvent);
}
/*----------------------------------------------------------------------------------*/
//��ҽ�����߳�ȥ
void	GameScreenWnd::UserComeInorLeave(int PlayerID,bool ComeOrLeave)
{
	//������λ�Ƿ������
	m_HaveUser[PlayerID] = ComeOrLeave; 
}
/*----------------------------------------------------------------------------------*/
//�Լ���Ϣ
void	GameScreenWnd::SetMeInformation(int MePlayerid,int MeRellid ,CString sNickName , __int64 i64money)
{
	//��ֵ��Ϣ
	m_i64MyMoney	= i64money;
	m_MeChariID		= MePlayerid;
	m_MeRellayChairID = MeRellid;
}
/*----------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------*/
void GameScreenWnd::AddFish(int traceX,int traceY,float roation,float movetime,float changetime,int fishtype,int ptindex,int nowindex,int fishid,int smallfishtype,int fishspeed)
{
	//�������
	m_FishArr[ptindex].m_Have = false;

	//����켣����
	if(m_MeRellayChairID>=3)
	{
		float temp = roation+180;
		if(temp>360)temp=temp-360;
		//�����ж�
		if(temp<0)temp=360-abs(temp);
		int tx,ty;
		if(traceX<0&&traceY<=0)
		{
			tx = CLIENT_WIDTH +abs(traceX);
			ty = CLIENT_HEIGHT +abs(traceY);

		}
		else  if(traceX<0&&traceY>0)
		{
			tx = CLIENT_WIDTH +abs(traceX);
			ty = CLIENT_HEIGHT -abs(traceY);

		}
		else  if(traceX>0&&traceY>0)
		{
			tx = CLIENT_WIDTH -abs(traceX);
			ty = CLIENT_HEIGHT -abs(traceY);

		}
		else  if(traceX>0&&traceY<0)
		{
			tx = CLIENT_WIDTH -abs(traceX);
			ty = CLIENT_HEIGHT +abs(traceY);

		}

		//����ֵ
		m_FishTrace[ptindex][nowindex].x = tx;
		m_FishTrace[ptindex][nowindex].y = ty;
		m_FishTrace[ptindex][nowindex].movetime = movetime;
		m_FishTrace[ptindex][nowindex].rotation = temp;
		m_FishTrace[ptindex][nowindex].changetime = changetime;
		m_FishTrace[ptindex][nowindex].m_fudaifishtype = smallfishtype;
		m_FishTrace[ptindex][nowindex].m_speed = fishspeed;
		m_FishTrace[ptindex][nowindex].m_fishid = fishid;
		m_FishTrace[ptindex][nowindex].m_fishtype = fishtype;
		m_FishTrace[ptindex][nowindex].m_fudaifishtype = smallfishtype;

	}
	else
	{
		//����ֵ
		m_FishTrace[ptindex][nowindex].x = traceX;
		m_FishTrace[ptindex][nowindex].y = traceY;
		m_FishTrace[ptindex][nowindex].movetime = movetime;
		m_FishTrace[ptindex][nowindex].rotation = roation;
		m_FishTrace[ptindex][nowindex].changetime = changetime;
		m_FishTrace[ptindex][nowindex].m_fudaifishtype = smallfishtype;
		m_FishTrace[ptindex][nowindex].m_speed = fishspeed;
		m_FishTrace[ptindex][nowindex].m_fishid = fishid;
		m_FishTrace[ptindex][nowindex].m_fishtype = fishtype;
		m_FishTrace[ptindex][nowindex].m_fudaifishtype = smallfishtype;

	}
	//����Ѿ����˵�4���켣������ͻ���
	if(nowindex==4)
	{

		bool isHace = false;
		if(!isHace)
		{
			//��ȡ����
			Fish *tempFish=m_FishFactory.ActiveItem();
			if (NULL==tempFish) return ;
			tempFish->m_actionindex = 0;
			tempFish->m_Nowindex = 0;
			tempFish->m_HitUser = -1;
			tempFish->m_Currtime = 0;
			tempFish->m_fishID = m_FishTrace[ptindex][0].m_fishid;
			tempFish->m_Have = true;
			tempFish->m_index = m_FishTrace[ptindex][0].m_fishtype;
			tempFish->m_PtIndex = 0;
			tempFish->m_isHit = false;
			tempFish->m_ptx = m_FishTrace[ptindex][0].x - m_FishmoveRec[m_FishTrace[ptindex][0].m_fishtype][0]/2 ;
			tempFish->m_pty = m_FishTrace[ptindex][0].y - m_FishmoveRec[m_FishTrace[ptindex][0].m_fishtype][1]/2;
			tempFish->m_Roation = m_FishTrace[ptindex][0].rotation;
			tempFish->m_speed = m_FishTrace[ptindex][0].m_speed;
			tempFish->m_smallFish = 0;
			tempFish->m_smallFish = m_FishTrace[ptindex][0].m_fudaifishtype;
			tempFish->m_traceIndex = ptindex;
			tempFish->m_creatTime = GetTickCount();

		}
	}  
}
/*----------------------------------------------------------------------------------*/
void GameScreenWnd::CheckHit()
{
	if(m_InitScreen==false)return;
	//�ж��ӵ��Ƿ��������
	try{
		Fish *tfish=NULL;
		for(int i=0;i<200;i++)
		{
			if(m_MeBullet[i].m_Have==false)continue;
			INT_PTR nEnumIndex=0;
			Fish *tfish=NULL;
			bool bShoot=false;
			do
			{
				tfish=m_FishFactory.EnumActiveObject(nEnumIndex++);
				//ö����Ⱥ

				if (NULL==tfish) break;
				if(tfish->m_Have==false)continue;
				if(tfish->m_isHit==true)continue;
				//if(GetTickCount()-tfish->m_creatTime>50000)continue;
				//��������������
				CRgn  nit;
				//�����ĸ���������
				CPoint VertexPoint[4];
				//�����һ�������
				long x2;
				float co;
				float si;
				//����������ر���
				int m_BliCountx = 3;
				int m_BliCounty = 2;
				if(tfish->m_index>=7&&tfish->m_index<12)
				{
					m_BliCountx = 5;
					m_BliCounty = 4;
				}
				else if(tfish->m_index>12&&tfish->m_index<13)
				{
					m_BliCountx = 3;
					m_BliCounty = 2;
				}
				else if(tfish->m_index>=13&&tfish->m_index<14)
				{
					m_BliCountx = 4;
					m_BliCounty = 3;
				}
				else if(tfish->m_index>=14&&tfish->m_index<=16)
				{
					m_BliCountx = 6;
					m_BliCounty = 5;
				}

				else if(tfish->m_index>=17&&tfish->m_index<18)
				{
					m_BliCountx = 4;
					m_BliCounty = 4;
				}
				else if(tfish->m_index>=18&&tfish->m_index<19)
				{
					m_BliCountx = 6;
					m_BliCounty = 5;
				}
				else if(tfish->m_index>=19&&tfish->m_index<20)
				{
					m_BliCountx = 6;
					m_BliCounty = 3;
				}
				else if(tfish->m_index>=20&&tfish->m_index<21)
				{
					m_BliCountx = 4;
					m_BliCounty = 4;
				}
				else if(tfish->m_index>=21&&tfish->m_index<22)
				{
					m_BliCountx = 2;
					m_BliCounty = 4;
				}
				else if(tfish->m_index>=22&&tfish->m_index<23)
				{
					m_BliCountx = 7;
					m_BliCounty = 6;
				}
				else if(tfish->m_index>=23&&tfish->m_index<24)
				{
					m_BliCountx = 6;
					m_BliCounty = 2;
				}
				//
				co=cos(2*3.1415926*(tfish->m_Roation)/360.0);
				si=sin(2*3.1415926*(tfish->m_Roation)/360.0);
				//��ת�����ĵ�
				CPoint Center;
				//���ĵ���������X����+������Ŀ�ȵ�һ��
				Center.x= tfish->m_ptx + m_FishmoveRec[tfish->m_index][0]/2;
				Center.y= tfish->m_pty + m_FishmoveRec[tfish->m_index][1]/2;
				//����Ҫ��ת�ĵ�
				CPoint Center1;
				Center1.x= Center.x - (m_FishmoveRec[tfish->m_index][0]/m_BliCountx);
				Center1.y= Center.y - m_FishmoveRec[tfish->m_index][1]/m_BliCounty;
				x2=Center1.x;
				VertexPoint[0].x=(long)((double)(Center1.x-Center.x)*co-(double)(Center1.y-Center.y)*si+Center.x);
				VertexPoint[0].y=(long)((double)(x2-Center.x)*si+(double)(Center1.y-Center.y)*co+Center.y);

				Center1.x= Center.x - m_FishmoveRec[tfish->m_index][0]/m_BliCountx;
				Center1.y= Center.y + m_FishmoveRec[tfish->m_index][1]/m_BliCounty;
				x2=Center1.x;
				VertexPoint[1].x=(long)((double)(Center1.x-Center.x)*co-(double)(Center1.y-Center.y)*si+Center.x);
				VertexPoint[1].y=(long)((double)(x2-Center.x)*si+(double)(Center1.y-Center.y)*co+Center.y);

				Center1.x= Center.x + m_FishmoveRec[tfish->m_index][0]/m_BliCountx;
				Center1.y= Center.y + m_FishmoveRec[tfish->m_index][1]/m_BliCounty;
				x2=Center1.x;
				VertexPoint[2].x=(long)((double)(Center1.x-Center.x)*co-(double)(Center1.y-Center.y)*si+Center.x);
				VertexPoint[2].y=(long)((double)(x2-Center.x)*si+(double)(Center1.y-Center.y)*co+Center.y);

				Center1.x= Center.x + m_FishmoveRec[tfish->m_index][0]/m_BliCountx;
				Center1.y= Center.y - m_FishmoveRec[tfish->m_index][1]/m_BliCounty;
				x2=Center1.x;
				VertexPoint[3].x=(long)((double)(Center1.x-Center.x)*co-(double)(Center1.y-Center.y)*si+Center.x);
				VertexPoint[3].y=(long)((double)(x2-Center.x)*si+(double)(Center1.y-Center.y)*co+Center.y);
				nit.CreatePolygonRgn(VertexPoint, 4, ALTERNATE);

				m_MeBullet[i].m_Point.x = m_MeBullet[i].m_ptx+32;
				m_MeBullet[i].m_Point.y = m_MeBullet[i].m_pty+32;

				//�ж��ӵ��Ƿ���ǹ�ں���
				CRect m_rect;
				m_rect.left = m_UserPT[m_MeBullet[i].m_Senduser].x+30;
				m_rect.top =  m_UserPT[m_MeBullet[i].m_Senduser].y+10;
				m_rect.right = m_rect.left + 100;
				m_rect.bottom = m_rect.top + 190;
				bool is_No =m_rect.PtInRect(m_MeBullet[i].m_Point);


				//��ȡ����������
				//��ʼ��ȡ�ӵ�����       
				bool is_hit =  nit.PtInRegion(m_MeBullet[i].m_Point);	
				nit.DeleteObject();
				if(is_hit&&m_MeBullet[i].m_Stop==false&&!is_No)
				{
					//��������


					//������ײ���ӵ�״̬
					m_MeBullet[i].m_Netindex = 0;
					m_MeBullet[i].m_Stop = true;
					//���ñ������������
					//�������ӵ����ҷ����
					if(m_MeBullet[i].m_Senduser==m_MeChariID||m_MeBullet[i].m_isAndroid)
					{
						//������Ϣ�������
						bShoot=true;
						//�ж��ж�
						break;
					}
				}
			}while(true);
		}
	}
	catch(...)
	{
		//  AfxMessageBox(L"��Ϸ��������Ӱ����Ϸ���У�����ϵ������QQ��157480296");
	}
}

/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/


