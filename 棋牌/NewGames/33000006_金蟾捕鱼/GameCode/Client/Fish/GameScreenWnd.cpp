#include "StdAfx.h"
#include "gamescreenwnd.h"
#include "..\NewGameClient.h"

//����
//�����ڵ�
#define  TIMERIDSHOOT			1
//�ӵ�����
#define  TIMERIDBULLTE			2
//����ζ�����
#define  TIMERIDFISHMOVE		3
#define  TIMERIDNETOPEN			4
#define  TIMERIDFISHROUND		5
#define  TIMERIDMONEYACTION		6
//���Ǯ���ƶ�
#define  TIMERIDUSERMONEY		7
//�л���������
#define  TIMERIDCHANGESCREEN	8
//�Զ������ӵ�
#define  TIMERIDSENDSHOOT		9
//
//�Զ������ӵ�
#define  TIMERIDOUT				10
//�������ж�
#define  TIMERIDDING			11
//������ʱ��
#define	 TIME_SHAKE				12
//��ʾ�㲥
#define	 TIME_SHOW_MSG			13		//����

/*---------------------------------------------------------------------------------------------------------------*/
//��ͼ��С
////�ζ�ͼ
//const int GameScreenWnd::m_FishmoveCount[24] = {6,8,12,12,12,13,12,10,12,12,8,15,12,15,13,19,13,7,9,10,3,1,9,6};
////����ͼ
//const int GameScreenWnd::m_FishdeadCount[24] = {2,2,2,3,3,3,4,3,2,4,6,4,3,3,3,3,3,0,2,2,0,0,3,0};
//��ͼ��С
/*---------------------------------------------------------------------------------------------------------------*/
//�ζ�ͼ
const int GameScreenWnd::m_FishmoveCount[24] = {6, 8, 12, 12, 12, 13, 12, 10, 12, 12, 8,15,12,15,13,12,12,12,13,12,9,6,9,3};
//  const int GameScreenWnd::m_FishmoveCount[24] = {6, 8, 12, 12, 12, 13, 12, 10, 12, 12, 11, 15, 12, 15, 13, 12, 12, 12, 13, 12, 9, 6, 9, 3};
//����ͼ
const int GameScreenWnd::m_FishdeadCount[24] = {2, 2,  2,  3,  3,  3,  4,  3,  2,  4,  6, 4,   3,  3,  3,  3,  3,  3,  0,  3, 4, 0, 3, 0};
/*---------------------------------------------------------------------------------------------------------------*/
const int GameScreenWnd::m_FishmoveRec[24][2] = {{32,128},{32,64},{64,128},{64,128},{64,128},{128,128},{64,256},{128,256},{128,256},{128,256},{256,256},{256,256},{128,256},{256,256},{512,256},{256,512},{256,512},/*{256,256}*/{512,512},{256,512},{256,512},{256,512},/*{64,512}*/{512,256},{512,1024},{256,256}/*{512,256}*/};
//��ͼ��С
const int GameScreenWnd::m_FishDeadRec[24][2] = {{32,128},{32,64},{64,128},{64,128},{64,128},{128,128},{128,256},{128,256},{128,256},{256,256},{256,256},{256,256},{128,256},{256,256},{512,256},{256,512},{256,512},/*{256,256}*/{512,512},{256,512},{256,512},{256,512},/*{64,512}*/{512,256},{512,1024},{256,256}};
//
//const int GameScreenWnd::m_fishscore[24]={2,2,3,4,5,6,7,8,9,12,10,15,18,20,25,30,40,100,40,70,300,120,200,150};

/*---------------------------------------------------------------------------------------------------------------*/
//���ɽ�Ҹ���
const int GameScreenWnd::m_iFishMoneyCount[24]	={	1/*0*/,1/*1*/,2/*2*/,2/*3*/,3/*4*/,3/*5*/,3/*6*/,4/*7*/,4/*8*/,
													4/*9*/,4/*10*/,5/*11*/,5/*12*/,5/*13*/,5/*14*/,5/*15*/,6/*16*/,
													6/*17*/,6/*18*/,6/*19*/,6/*20*/,6/*21*/,7/*22*/,7/*23*/};
/*---------------------------------------------------------------------------------------------------------------*/
//����λ��
const int G_iShakePosition[5][2] = {{0,0},{-10,5},{-10,10},{10,15},{15,10}};
/*---------------------------------------------------------------------------------------------------------------*/
char G_szFishName[24][30] = {{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"�ڹ�"},
								{"С��"},{"С��"},{"С��"},{"����"},{"ħ����"},{"������"},{"�����"},{"����"},{"��Ϻ"},{"������"},
								{"����"},{"���"},{"���ͻƽ���"},{"�����"}};
//char G_szFishName[24][30] = {	{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"С��"},{"�ڹ�"},
//								{"С��"},{"С��"},{"С��"},{"����"},{"ħ����"},{"ը������"},{"ը���ƽ���"},{"�ƽ��"},{"˫ͷ�ĽŹ�"},{"�ƽ��ĽŹ�"},
//								{"�����"},{"�𹿰�"},{"���ͻƽ���"},{"���"}};
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
	m_sUserName.Format("��������ͷ��绰��4008819308");
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

	m_iMoveY = 0;
	m_iColorR = 255;
	m_iColorB = 10;
	m_iColorG = 10;

	//�㲥����
	m_dqBroadCast.clear();
}

GameScreenWnd::~GameScreenWnd(void)
{
	CleanAllMemory();
}
/*---------------------------------------------------------*/
//��ÿͻ��˽ӿ�ָ��
void GameScreenWnd::SetGameClientP(CNewGameClient *p)
{
	m_pGameClient = p;
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
		pDxDialog->Render();
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

	//����cpuʹ����
	 pDxDialog->LoadImg();

	return;
}
/*---------------------------------------------------------*/
//������Դ
void GameScreenWnd::LoadImg()
{
   /*****��ʼ������Դ******/	

	//���뱳����Դ
	CString bgImgPath;

	//���뱳��
	char strPath[50] = {0};
	for(int i=0;i<3;i++)
	{
		sprintf(strPath,"Jcfish/Ui/BG/Bgimg%d.png",i);
		LoadImg(strPath,&m_bgImgTexture[i]);
		m_HaveLoadNum++;
		
	}
	//���ر߿�
	bgImgPath = "Jcfish/Ui/BG/Border.png";
    for(int n=0;n<10;n++)
	  if(!FAILED( (D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_BorderImgTexture))))
	      break;

	  m_HaveLoadNum++;
	//�����ڵ���
	
	sprintf(strPath,"Jcfish/Ui/PAO/PaoTai.png");
	LoadImg(strPath,&m_PaoConImgTexture);
	m_HaveLoadNum++;

	  //�����ڵ���
	sprintf(strPath,"Jcfish/Ui/PAO/PaoBG.png");
	LoadImg(strPath,&m_PaoBgImgTexture);
	m_HaveLoadNum++;

	  //�����ڵ���
	sprintf(strPath,"Jcfish/Ui/PAO/PaoTwo.png");
	LoadImg(strPath,&m_PaoTwoImgTexture);
	m_HaveLoadNum++;
	//�ڻ�ͼ
	sprintf(strPath,"Jcfish/Ui/Pao/PaoTwoFire.png");
	LoadImg(strPath,&m_PaoTwoFireImgTexture);
	m_HaveLoadNum++;
	//
	sprintf(strPath,"Jcfish/Ui/Pao/PaoThreeFire.png");
	LoadImg(strPath,&m_PaoThreeFireImgTexture);
	m_HaveLoadNum++;
	//
	sprintf(strPath,"Jcfish/Ui/Pao/PaoThree.png");
	LoadImg(strPath,&m_PaoThreeImgTexture);
	m_HaveLoadNum++;

    for(int i=0;i<PLAY_COUNT;i++)
	{
	   m_HaveLoadNum++;
	   bgImgPath.Format("Jcfish/Ui/shotNumBack/shotNumBack%d.png",i);
	   for(int n=0;n<10;n++)
	     if(!FAILED( (D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_allscoreBgImgTexture[i]))))
	        break;
	}
	//����ͼƬ
	for(int i=0;i<5;i++)
	{
		m_HaveLoadNum++;
	    bgImgPath.Format("Jcfish/Ui/Num/goldNum%d.png",i);
		for(int n=0;n<10;n++)
           if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_NumImgTexture[i])))
	          break;
	}
	//�ӵ�ͼ
	m_HaveLoadNum++;
	bgImgPath = "Jcfish/Ui/PAO/shot_0.png";
	for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_BulltTwoTexture[0])))
	        break;
	//����״̬ͼ
	m_HaveLoadNum++;
	bgImgPath = "Jcfish/Ui/PAO/supPao.png";
	for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_SupStatTexture)))
	        break;

	bgImgPath = "Jcfish/Ui/PAO/shot_1.png";
	for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_BulltTwoTexture[1])))
	        break;
	
	bgImgPath = "Jcfish/Ui/PAO/net_0.png";
	for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_PaoTwoNetImgTexture)))
	        break;
	bgImgPath = "Jcfish/Ui/other/GAME_TIP.png";
	for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_OutTexture)))
	        break;
    bgImgPath = "Jcfish/Ui/Num/gold.png";
	for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_GoldImgTexture)))
	        break;
	
	bgImgPath = "Jcfish/Ui/Num/qianbi.png";
    for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_BiTexture)))
			 break;

	bgImgPath = "Jcfish/Ui/other/cursor.png";
    for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_CursorTexture)))
			 break;
	

	m_HaveLoadNum++;
	//������Ǯ����ͼ
	for(int i=0;i<22;i++)
	{
		char strPath[50] = {0};
		sprintf(strPath,"Jcfish/Ui/qian/%d.png",i);
		LoadImg(strPath,&m_MuchMoneyTexture[i]);
		m_HaveLoadNum++;
		
	}
	//��ס����ͼ
	for(int i=0;i<10;i++)
	{
		char strPath[50] = {0};
		sprintf(strPath,"Jcfish/Ui/ding/%d.png",i);
		LoadImg(strPath,&m_DingTexture[i]);
		m_HaveLoadNum++;
		
	}

	//����ͼ
	//�˳�ͼ
	for(int i=0;i<2;i++)
	{
		m_HaveLoadNum++;
	    bgImgPath.Format("Jcfish/Ui/WaterWave/%d.png",i);
		for(int n=0;n<10;n++)
          if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xffffffff,NULL,NULL,&m_WaterWaveTexture[i])))
	          break;
	}
	//�����ӵ�ͼ
	for(int i=0;i<2;i++)
	{
		m_HaveLoadNum++;
	    bgImgPath.Format("Jcfish/Ui/Pao/shot_%d_8.png",i);
		for(int n=0;n<10;n++)
          if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xffffffff,NULL,NULL,&m_SupButTexture[i])))
	          break;
	}
    //ˮ�沨��
	for(int i=0;i<32;i++)
	{
	 m_HaveLoadNum++;
	 bgImgPath.Format("Jcfish/Ui/Water/%d.png",i);
	 for(int n=0;n<10;n++)
        if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_WaterTexture[i])))
			break;
	}

	//԰��ͼ
	for(int i=0;i<3;i++)
	{
		m_HaveLoadNum++;
	    bgImgPath.Format("Jcfish/Ui/quan/%d.png",i);
		for(int n=0;n<10;n++)
          if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_QuanTexture[i])))
			break;
	}
	//�������ζ�ͼ
   for(int i=0;i<24;i++)
	{
	    for(int j=0;j<m_FishmoveCount[i];j++)
		{
			    
			    sprintf(strPath,"Jcfish/Ui/fish/%d/%d.png",i,j);
				LoadImg(strPath,&m_FishMoveTexture[i][j]);
			    m_HaveLoadNum++;
		    
		}
		//����ͼ
		for(int j=0;j<m_FishdeadCount[i];j++)
		{
			
  		    sprintf(strPath,"Jcfish/Ui/fish/%d/d%d.png",i,j);
			LoadImg(strPath,&m_FishDeadTexture[i][j]);
			m_HaveLoadNum++;
		    
		}
	}
	
	//�ӵ�������
	SetTimer(TIMERIDBULLTE,1,NULL);
	//����ζ�ͼ����
	SetTimer(TIMERIDFISHMOVE,150,NULL);
	//�ӵ���������
	SetTimer(TIMERIDNETOPEN,20,NULL);
	//
	
	//��Ǯ����
	SetTimer(TIMERIDMONEYACTION,100,NULL);
	//
	SetTimer(TIMERIDUSERMONEY,100,NULL);
	//
	//
	SetTimer(TIMERIDOUT,1000,NULL);
	
    //��������
    LoadSound();

    m_InitScreen = true;
	
	/*****������Դ����******/	

}
//��ʼ����
bool GameScreenWnd::InitDxDlog()
{
    m_hWnd=this->GetSafeHwnd();
	
	if(m_hWnd==NULL)AfxMessageBox("����ʧ��!");

	if( SUCCEEDED( InitD3D( m_hWnd ) ) )
    {
	   if( SUCCEEDED( TRUE ) )
        {
			 m_CursorInt = ShowCursor(TRUE);
			 m_RunRender = true;
			 m_hDxThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DxThreadRender,this,0,0);	
			 m_hWorkThread1=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DxThreadRender1,this,0,0);	
			 //SetTimer(100,100,NULL);
			 SetTimer(TIMERIDFISHROUND,1,NULL);
			 m_hWorkThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DxThreadLoad,this,0,0);
			 //LoadImg();
		}
	}  
	//��Ť����
	m_BtRect[0].top = 3;m_BtRect[0].left = 1092;m_BtRect[0].right = m_BtRect[0].left+ 58;m_BtRect[0].bottom = m_BtRect[0].top+24;
	m_BtRect[2].top = 3;m_BtRect[2].left = 1151;m_BtRect[2].right = m_BtRect[2].left+ 49;m_BtRect[2].bottom = m_BtRect[2].top+24;
	m_BtRect[1].top = 3;m_BtRect[1].left = 1200;m_BtRect[1].right = m_BtRect[1].left+ 58;m_BtRect[1].bottom = m_BtRect[1].top+24;
	m_BtRect[3].top = 710;m_BtRect[3].left = 500;m_BtRect[3].right = m_BtRect[3].left+ 72;m_BtRect[3].bottom = m_BtRect[3].top+27;
	m_BtRect[4].top = 714;m_BtRect[4].left = 600;m_BtRect[4].right = m_BtRect[4].left+ 72;m_BtRect[4].bottom = m_BtRect[4].top+20;

    return true;
}

//����DX���ڼ�������Դ����ͼƬ
HRESULT GameScreenWnd::InitD3D( HWND hWnd )
{
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
        return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//����Direct3D�豸����
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &m_pd3dDevice )))
    {
		AfxMessageBox("�豸����ʧ��!");
        return E_FAIL;
    }
	// �����㾫��
	if (FAILED(D3DXCreateSprite( m_pd3dDevice, &m_pSprite )))
	{
		AfxMessageBox("�����㾫��ʧ��!");
		return E_FAIL;
	}

	//��������
	m_Dxfont = NULL;
    D3DXFONT_DESCA lf;
    ZeroMemory(&lf, sizeof(D3DXFONT_DESCA));
    lf.Height = 12; //����߶�
    lf.Width = 0; // ������
    lf.Weight = 400; 
    lf.Italic = false;
	_sntprintf(lf.FaceName,sizeof(lf.FaceName),TEXT("����"));
    D3D_OK!=D3DXCreateFontIndirect(m_pd3dDevice, &lf, &m_Dxfont); //�����������


	m_DxCastFont = NULL;
	D3DXFONT_DESCA lf1;
	ZeroMemory(&lf1, sizeof(D3DXFONT_DESCA));
	lf1.Height = 40; //����߶�
	lf1.Width = 15; // ������
	lf1.Weight = 400; 
	lf1.Italic = false;
	_sntprintf(lf1.FaceName,sizeof(lf1.FaceName),TEXT("����"));
	D3D_OK!=D3DXCreateFontIndirect(m_pd3dDevice, &lf1, &m_DxCastFont); //�����������

	CString bgImgPath;
	//��Ťͼ
	for(int i=0;i<5;i++)
	{
	    bgImgPath.Format("Jcfish/Ui/bt/bt%d.png",i);
		for(int n=0;n<10;n++)
           if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,bgImgPath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_BtTexture[i])))
			   break;
	}

    D3DXCreateTextureFromFileEx(m_pd3dDevice,("Jcfish/Ui/BG/jindu.png"),D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_LoadTexture);
	D3DXCreateTextureFromFileEx(m_pd3dDevice,"Jcfish/Ui/bg/bg.jpg",D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffffff0,NULL,NULL,&m_LoadBgTexture);

	//��ʼ�����ô���
	m_configDlg.InitDlg(m_pd3dDevice,445,150);
	return S_OK;

}
//��Ⱦ����
void GameScreenWnd::Render()
{
	//��������豸
	m_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(100,100,100),1.0f,0);
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	//׼������
	m_pd3dDevice->BeginScene() ;
	//���þ���ģʽ
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	//���ؼ��ؽ���
	if(m_InitScreen==false || m_bHaveLinked == false)
	{
		m_pSprite->Draw(m_LoadBgTexture, NULL, NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
		//���ƽ���
		if(m_HaveLoadNum>360)m_HaveLoadNum = 360;
		CRect rcClient;
		rcClient.top= 0;
		rcClient.left=0;
		rcClient.right =  m_HaveLoadNum;
		rcClient.bottom = 15;
		m_pSprite->Draw(m_LoadTexture, rcClient, NULL, &D3DXVECTOR3(462, 621-190, 0), D3DCOLOR_ARGB(255,255,255,255));
		//���ư�Ť
		DrawBt();
		//������
		m_pSprite->Flush();
		//���ƽ���,ֹͣ����
		m_pSprite->End();
		//ֹͣ�豸
		m_pd3dDevice->EndScene();
		//�����豸����ǰ
		m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
		return;

	}

	//���Ʊ���
	//������л���������ʾ�˳�ͼ
	if(m_IsChangeScreen)
	{
		int pRreBgindex = 0;
		if(m_Bgindex==0)pRreBgindex=2;
		else if(m_Bgindex==1)pRreBgindex=0;
		else if(m_Bgindex==2)pRreBgindex=1;
		else pRreBgindex=0;

		m_pSprite->Draw(m_bgImgTexture[pRreBgindex], NULL, NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
		CRect rcClient;
		rcClient.top=0;
		rcClient.left=1280-(1280-m_ChanwaveSt.m_ptx-100);
		rcClient.right =  1280;
		rcClient.bottom =  740;
		int bgx = m_ChanwaveSt.m_ptx+100;
		if(bgx<0)bgx = 0;
		if( rcClient.left<0) rcClient.left = 0;
		m_pSprite->Draw(m_bgImgTexture[m_Bgindex], rcClient, NULL, &D3DXVECTOR3(bgx, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
		//���Ʋ���
		m_pSprite->Draw(m_WaterWaveTexture[m_ChanwaveSt.m_currImgIndex], NULL, NULL, &D3DXVECTOR3(m_ChanwaveSt.m_ptx, 0, 0), D3DCOLOR_ARGB(255,255,255,255));
	}
	else
	{
		m_pSprite->Draw(m_bgImgTexture[m_Bgindex], NULL, NULL, &D3DXVECTOR3(m_iShakex, m_iShakey, 0), D3DCOLOR_ARGB(255,255,255,255));
	}

	D3DXMATRIX mat;
	D3DXMATRIX mat1;
	D3DXMATRIX mat2;
	//�������
	try
	{
		Fish *tfish=NULL;
		for(int i=0;i<m_FishFactory.GetActiveCount();i++)
		{
			//ö����Ⱥ
			tfish=m_FishFactory.EnumActiveObject(i);
			if(tfish==NULL)continue;
			if(!tfish->m_Have)
			{
				m_FishFactory.FreeItem(tfish);continue;

			}
			if(GetTickCount()-tfish->m_creatTime>50000)
			{
				m_FishFactory.FreeItem(tfish);continue;

			}
			if(!tfish->m_Have)continue;
			int  fishindex = tfish->m_index;

			D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]/2,m_FishmoveRec[fishindex][1]/2),tfish->m_Roation*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx+m_iShakex,tfish->m_pty+m_iShakey));
			D3DXMatrixTransformation2D(&mat1,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(m_FishmoveRec[fishindex][0]/2,m_FishmoveRec[fishindex][1]/2),tfish->m_Roation*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx-10+m_iShakex,tfish->m_pty-10+m_iShakey));
			D3DXMatrixTransformation2D(&mat2,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(256/2,256/2),m_ActionIndex1%360*(D3DX_PI/180),&D3DXVECTOR2(tfish->m_ptx+m_FishmoveRec[fishindex][0]/2-256/2+m_iShakex,tfish->m_pty+m_FishmoveRec[fishindex][1]/2-256/2+m_iShakey));
			m_pSprite->Flush();
			//�����ԭ��
			if(tfish->m_smallFish!=-1)
			{
				m_pSprite->SetTransform(&mat2);
				m_pSprite->Draw(m_QuanTexture[tfish->m_smallFish],NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			}


			CRect rcClient;
			rcClient.top=0;
			rcClient.left=0; 
			if(tfish->m_isHit==false||m_FishdeadCount[fishindex]==0)
			{

				rcClient.right = rcClient.left+m_FishmoveRec[fishindex][0];
				rcClient.bottom = rcClient.top+m_FishmoveRec[fishindex][1];
				m_pSprite->SetTransform(&mat1);
				m_pSprite->Draw(m_FishMoveTexture[fishindex][tfish->m_actionindex%m_FishmoveCount[fishindex]],rcClient, NULL, NULL, D3DCOLOR_ARGB(80,0,0,0));
				m_pSprite->SetTransform(&mat);
				m_pSprite->Draw(m_FishMoveTexture[fishindex][tfish->m_actionindex%m_FishmoveCount[fishindex]],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

			}
			else
			{
				m_pSprite->SetTransform(&mat);
				rcClient.right = rcClient.left+m_FishDeadRec[fishindex][0];
				rcClient.bottom = rcClient.top+m_FishDeadRec[fishindex][1];
				m_pSprite->Draw(m_FishDeadTexture[fishindex][tfish->m_actionindex%m_FishdeadCount[fishindex]],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			}


		}
	}
	catch(...)
	{
		OutputDebugString("dwjjcby::��Ⱦ����1");
		//AfxMessageBox(L"����");
	}

	//�����Լ��ӵ�
	for(int i=0;i<200;i++)
	{
		if(!m_MeBullet[i].m_Have)continue;
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(32,31.0f),m_MeBullet[i].m_Roation*(D3DX_PI/180),&D3DXVECTOR2(m_MeBullet[i].m_ptx,m_MeBullet[i].m_pty));
		m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
		CRect rcClient;
		if(!m_MeBullet[i].m_Stop)
		{
			//��������ڷ�ɫ
			if(m_MeBullet[i].m_issuper)
			{
				rcClient.top=0;
				rcClient.left=(m_ActionIndex%2)*64;
				rcClient.right = rcClient.left+128/2;
				rcClient.bottom = rcClient.top+64;
				m_pSprite->Draw(m_SupButTexture[m_MeBullet[i].m_isThree],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

			}
			else
			{
				rcClient.top=m_MeBullet[i].m_Senduser*384/6;
				rcClient.left=(m_ActionIndex%2)*64;
				rcClient.right = rcClient.left+128/2;
				rcClient.bottom = rcClient.top+384/6;
				m_pSprite->Draw(m_BulltTwoTexture[m_MeBullet[i].m_isThree],rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			}
		}
		else
		{
			int  now_x = m_MeBullet[i].m_ptx - 161+32 ;
			int  now_y = m_MeBullet[i].m_pty - 161+32 ;

			D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(161,161.0f),m_MeBullet[i].m_Roation*(D3DX_PI/180),&D3DXVECTOR2(now_x,now_y));
			m_pSprite->SetTransform(&mat);
			rcClient.left=322*m_MeBullet[i].m_Netindex;
			rcClient.top=0;
			rcClient.right = rcClient.left+322;
			rcClient.bottom = rcClient.top+322;
			if(m_MeBullet[i].m_issuper)
			{
				m_pSprite->Draw(m_PaoTwoNetImgTexture,rcClient, NULL, NULL, D3DCOLOR_ARGB(255,230,140,0));
			}
			else
			{
				if(m_MeBullet[i].m_RealUser == m_MeRellayChairID)
				{
					m_pSprite->Draw(m_PaoTwoNetImgTexture,rcClient, NULL, NULL, D3DCOLOR_ARGB(240,0,250,250));
				}
				else
				{
					m_pSprite->Draw(m_PaoTwoNetImgTexture,rcClient, NULL, NULL, D3DCOLOR_ARGB(240,255,255,255));
				}
				
			}
		}
	}
	//����ˮ�沨�ˣ���Ҫƽ��15��ͼ
	int BowenX = 0;
	int BowenY = 0;
	//��ԭ�Ƕ�Ϊ����
	D3DXMATRIX matRot;
	D3DXMatrixIdentity(&matRot);//����
	m_pSprite->SetTransform(&matRot);
	for(int i=0;i<15;i++)
	{   
		BowenX = i * 300;
		BowenY = 0;
		if(i>4){BowenY=300;BowenX = (i-5) * 300;}
		if(i>9){BowenY=600;BowenX = (i-10) * 300;}
		m_pSprite->Draw(m_WaterTexture[m_ActionIndex%32], NULL, NULL, &D3DXVECTOR3(BowenX, BowenY, 0), D3DCOLOR_ARGB(200,255,255,255));

	}

	//���Ʋ����е�����ķ���
	for(int i=0;i<100;i++)
	{
		if(!m_NumArr[i].m_Have)continue;
		int nCellCount = 0;
		BYTE byCell[10];
		LONG lCellScore = m_NumArr[i].m_Numcount;
		if(lCellScore<=0)continue;

		if(lCellScore>0)
		{
			for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
			{          
				byCell[i]=lCellScore%10;//�Ӹ�λ��ʼ��ÿλ��
				nCellCount++;
			}
		}
		int temppointx = 0;
		int m_jiaodu = 0;
		//����Ƕ�����ô��Ҫת��180��
		if(m_NumArr[i].m_HitUser>=3){m_jiaodu=180;}
		int  m_xPianyi = nCellCount*15*m_NumArr[i].m_beishu/2;
		int  m_xWidht = 24;
		if(m_NumArr[i].m_beishu>1.5)m_xWidht = 54;
		for( int j = nCellCount-1; j >=0; j-- )
		{
			CRect rcClient;
			rcClient.top=0;
			rcClient.left=(byCell[j])*m_xWidht;
			rcClient.bottom= rcClient.top + m_xWidht ;
			rcClient.right=  rcClient.left +m_xWidht;
			int now_x = m_NumArr[i].m_ptx+temppointx-m_xPianyi;
			int now_y = m_NumArr[i].m_pty;

			if(m_NumArr[i].m_Time<20)
			{
				if(m_NumArr[i].m_HitUser>=3)
					now_y=now_y+(m_NumArr[i].m_Time*4);
				else
					now_y=now_y-(m_NumArr[i].m_Time*4);
			}
			else if(m_NumArr[i].m_Time>=20&&m_NumArr[i].m_Time<30)
			{
				if(m_NumArr[i].m_HitUser>=3)
					now_y=now_y+80-((m_NumArr[i].m_Time-20)*4);
				else
					now_y=now_y-80+((m_NumArr[i].m_Time-20)*4);
			}
			else
			{
				if(m_NumArr[i].m_HitUser>=3)
					now_y=now_y+40;
				else
					now_y=now_y-40;

			}
			if(m_NumArr[i].m_beishu>1.5)
				D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.8f,0.8f),&D3DXVECTOR2(53.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(now_x+m_iShakex,now_y+m_iShakey));
			else
				D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(m_NumArr[i].m_beishu,m_NumArr[i].m_beishu),&D3DXVECTOR2(53.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(now_x+m_iShakex,now_y+m_iShakey));
			m_pSprite->SetTransform(&mat);
			if(m_NumArr[i].m_beishu>1.5)
				m_pSprite->Draw(m_NumImgTexture[4], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			else
				m_pSprite->Draw(m_NumImgTexture[2], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	

			if(m_NumArr[i].m_HitUser>=3)
				temppointx = temppointx - 15*m_NumArr[i].m_beishu;
			else
				temppointx = temppointx + 15*m_NumArr[i].m_beishu;
		}

	}
	//��ס
	if(m_DingStruc.m_have)
	{
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(1024/2,512/2),0*(D3DX_PI/180),&D3DXVECTOR2(m_DingStruc.m_ptx,m_DingStruc.m_pty));
		m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
		//�����22����Ǯ
		if(m_DingStruc.m_nowindex<10)
		{
			m_pSprite->Draw(m_DingTexture[m_DingStruc.m_nowindex],NULL, NULL, NULL, D3DCOLOR_ARGB(255,200,200,255));
		}
	}
	//��Ǯ����
	for(int i=0;i<6;i++)
	{
		if(!m_MuchmoneyAct[i].m_have)continue;
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(1024/2,512/2),0*(D3DX_PI/180),&D3DXVECTOR2(m_MuchmoneyAct[i].m_ptx+m_iShakex,m_MuchmoneyAct[i].m_pty+m_iShakey));
		m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
		//�����22����Ǯ
		if(m_MuchmoneyAct[i].m_nowindex<22)
		{
			m_pSprite->Draw(m_MuchMoneyTexture[m_MuchmoneyAct[i].m_nowindex],NULL, NULL, NULL, D3DCOLOR_ARGB(255,200,200,255));
		}
		//����Ϳ�ʼת����
		else 
		{
			int ptx = m_UserPT[i].x-60;
			int pty = m_UserPT[i].y-255;
			if(i>=3)pty = pty +400;
			D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(310/2,310/2),m_MuchmoneyAct[i].m_Roation*(D3DX_PI/180),&D3DXVECTOR2(ptx+m_iShakex,pty+m_iShakey));
			m_pSprite->SetTransform(&mat);
			m_pSprite->Draw(m_QuanTexture[2],NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
			//���ƻ�������
			int nCellCount = 0;
			BYTE byCell[10];
			LONG lCellScore = m_MuchmoneyAct[i].m_DiuMoney;
			if(lCellScore>0)
			{
				for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
				{          
					byCell[i]=lCellScore%10;//�Ӹ�λ��ʼ��ÿλ��
					nCellCount++;
				}
			}
			else
			{
				nCellCount =1;
				byCell[0]=0;
			}
			int temppointx = 0;
			ptx =  ptx  + 310/2-(nCellCount*45)/2;
			if(i<3)
			{
				for( int j = nCellCount-1; j >=0; j-- )
				{
					CRect rcClient;
					rcClient.top=0;
					rcClient.left=(byCell[j])*45;
					rcClient.bottom= rcClient.top + 51 ;
					rcClient.right=  rcClient.left +45;
					D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(22.5f,25.5f),0*(D3DX_PI/180),&D3DXVECTOR2(ptx+temppointx+m_iShakex,pty+140+m_iShakey));
					m_pSprite->SetTransform(&mat);
					m_pSprite->Draw(m_NumImgTexture[1], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
					temppointx = temppointx + 45;
				}
			}
			else
			{
				for( int j = 0; j <nCellCount; j++ )
				{
					CRect rcClient;
					rcClient.top=0;
					rcClient.left=(byCell[j])*45;
					rcClient.bottom= rcClient.top + 51 ;
					rcClient.right=  rcClient.left +45;
					D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(22.5f,25.5f),180*(D3DX_PI/180),&D3DXVECTOR2(ptx+temppointx+m_iShakex,pty+135+m_iShakey));
					m_pSprite->SetTransform(&mat);
					m_pSprite->Draw(m_NumImgTexture[1], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
					temppointx = temppointx + 45;
				}
			}

		}
	}

	//�������
	for(int i=0;i<PLAY_COUNT;i++)
	{  
		//������̨
		if(!m_HaveUser[i])continue;
		int m_jiaodu = 0;
		int m_userpoajiaodu = int(m_UserPaoJiaodu[i]);
		int m_bgjuli = 60;
		int m_fenjuli  = 100;
		//����Ƕ�����ô��Ҫ����ת��180��
		if(i>=3){m_jiaodu=180;m_bgjuli =  40;m_fenjuli=77;}

		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(128.0f,128.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x-28,m_UserPT[i].y-28));
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(m_PaoConImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		//��̨
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(128.0f,128.0f),m_userpoajiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPaoPT[i].x-28,m_UserPaoPT[i].y-28));
		m_pSprite->SetTransform(&mat);
		//��
		if(m_UserSendstate[i])
		{
			if(m_BeiLv[i]>500) 
			{
				if(m_UserSuperPao[i])
				{
					D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(130.0f,130.0f),m_userpoajiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPaoPT[i].x-30,m_UserPaoPT[i].y-30));
					m_pSprite->SetTransform(&mat);
					m_pSprite->Draw(m_PaoThreeFireImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
				}
				else
				{
					m_pSprite->Draw(m_PaoThreeFireImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
				}
			}
			else
			{
				if(m_UserSuperPao[i])
				{
					D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(130.0f,130.0f),m_userpoajiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPaoPT[i].x-30,m_UserPaoPT[i].y-30));
					m_pSprite->SetTransform(&mat);
					m_pSprite->Draw(m_PaoThreeFireImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
				}
				else
				{
					m_pSprite->Draw(m_PaoThreeFireImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

				}


			}
		}
		if(m_BeiLv[i]>500) 
		{
			if(m_UserSuperPao[i])
			{

				D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(130.0f,130.0f),m_userpoajiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPaoPT[i].x-30,m_UserPaoPT[i].y-30));
				m_pSprite->SetTransform(&mat);
				m_pSprite->Draw(m_PaoThreeImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,0,0));
			}
			else
				m_pSprite->Draw(m_PaoThreeImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		}
		else
		{
			if(m_UserSuperPao[i])
			{
				D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(130.0f,130.0f),m_userpoajiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPaoPT[i].x-30,m_UserPaoPT[i].y-30));
				m_pSprite->SetTransform(&mat);
				m_pSprite->Draw(m_PaoTwoImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,0,0));
			}
			else
				m_pSprite->Draw(m_PaoTwoImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));


		}
		//���ʱ���
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(128.0f,128.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x-28,m_UserPT[i].y-28));
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(m_PaoBgImgTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
		//��ұ���
		{
			int nCellCount = 0;
			BYTE byCell[10];
			LONG lCellScore = m_BeiLv[i];
			if(lCellScore>0)
			{
				for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
				{          
					byCell[i]=lCellScore%10;//�Ӹ�λ��ʼ��ÿλ��
					nCellCount++;
				}
			}
			else
			{
				nCellCount =1;
				byCell[0]=0;
			}
			int temppointx = 0;
			int ajuli  = 12;
			if(i>=3)ajuli = -12;

			for( int j = nCellCount-1; j >=0; j-- )
			{
				int Thisx = m_UserPT[i].x + 100 - 16 * nCellCount/2 + temppointx - 8;
				if(i>=3)Thisx =  Thisx + nCellCount*16/2 + 16 ;
				CRect rcClient;
				rcClient.top=0;
				rcClient.left=(byCell[j])*24;
				rcClient.bottom= rcClient.top + 23 ;
				rcClient.right=  rcClient.left +24;
				D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(12.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(Thisx,m_UserPT[i].y+m_fenjuli+ajuli));
				m_pSprite->SetTransform(&mat);
				m_pSprite->Draw(m_NumImgTexture[2], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
				if(i>=3)
					temppointx = temppointx - 16;
				else
					temppointx = temppointx + 16;
			}

		}

		//���Ʒ�������
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(102.0f,51.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+150,m_UserPT[i].y+m_bgjuli));
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(m_allscoreBgImgTexture[i], NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));

		//����Ϸ�
		int nCellCount = 0;
		BYTE byCell[10];
		LONG lCellScore = m_UserScore[i];
		if(lCellScore>0)
		{
			for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
			{          
				byCell[i]=lCellScore%10;//�Ӹ�λ��ʼ��ÿλ��
				nCellCount++;
			}
		}
		else
		{
			nCellCount =1;
			byCell[0]=0;
		}
		int temppointx = 0;
		for( int j = nCellCount-1; j >=0; j-- )
		{
			CRect rcClient;
			rcClient.top=0;
			rcClient.left=(byCell[j])*16;
			rcClient.bottom= rcClient.top + 23 ;
			rcClient.right=  rcClient.left +16;
			D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(53.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+200+temppointx,m_UserPT[i].y+m_fenjuli));
			m_pSprite->SetTransform(&mat);
			m_pSprite->Draw(m_NumImgTexture[0], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			if(i>=3)
				temppointx = temppointx - 16;
			else
				temppointx = temppointx + 16;
		}
		//��ǰ���ĵ�

		D3DXMATRIX matRot;
		D3DXMatrixIdentity(&matRot);//����
		m_pSprite->SetTransform(&matRot);
		//���Ǯ��
		for(int j=0;j<3;j++)
		{
			m_pSprite->SetTransform(&matRot);
			if(!m_UserStruct[i][j].m_Have)continue;
			int m_count = m_UserStruct[i][j].m_Nowscore;
			//���ö��ŵı����50�� Ҫ���Ǿͳ嵽��
			if(m_count>50)m_count = 50;
			int m_henx = j * 40;
			int m_heny = 0-m_count * 4 - 20;;
			for(int n=0;n<m_count;n++)
			{
				int m_ydx = 140;
				int m_hen = 0;
				if(i>=3)
				{
					m_hen = m_hen + n*4;
					m_ydx = 54;
				}
				else 
				{
					m_hen = m_hen - n*4;
				}

				m_pSprite->Draw(m_BiTexture, NULL, NULL, &D3DXVECTOR3(m_UserPT[i].x-30-m_henx, m_UserPT[i].y +m_ydx+m_hen , 0), D3DCOLOR_ARGB(255,255,255,255));	

			}

			//����Ϸ�
			int nCellCount = 0;
			BYTE byCell[10];
			LONG lCellScore =m_UserStruct[i][j].m_Money;
			if(lCellScore>0)
			{
				for(int n = 0;lCellScore>0;lCellScore=lCellScore/10,n++)
				{          
					byCell[n]=lCellScore%10;//�Ӹ�λ��ʼ��ÿλ��
					nCellCount++;
				}
			}
			int temppointx = 0;
			if(i>=3)m_heny = -m_heny-140;
			for( int n = nCellCount-1; n >=0; n-- )
			{
				CRect rcClient;
				rcClient.top=0;
				rcClient.left=(byCell[n])*12;
				rcClient.bottom= rcClient.top + 24 ;
				rcClient.right=  rcClient.left +12;
				int m_ydx = 30;
				int m_ydy = 140;
				if(i>=3)
				{
					m_ydx = 1;
					m_ydy = 180;
				}
				else 
				{
					m_ydx = -30;
					m_ydy = 140;
				}
				D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(6.0f,12.0f),m_jiaodu*(D3DX_PI/180),&D3DXVECTOR2(m_UserPT[i].x+m_ydx-m_henx+temppointx,m_UserPT[i].y +m_ydy+m_heny));
				m_pSprite->SetTransform(&mat);
				m_pSprite->Draw(m_NumImgTexture[3], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
				if(i>=3)
					temppointx = temppointx - 12;
				else
					temppointx = temppointx + 12;
			}		

		}

	}

	//����Ʈ����
	for(int i=0;i<100;i++)
	{
		if(!m_GoldArr[i].m_Have)continue;
		D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(0.8f,0.8f),&D3DXVECTOR2(72,62.0f),0*(D3DX_PI/180),&D3DXVECTOR2(m_GoldArr[i].m_ptx+m_iShakex,m_GoldArr[i].m_pty+m_iShakey));
		m_pSprite->Flush();
		m_pSprite->SetTransform(&mat);
		CRect rcClient;
		rcClient.top=0;
		rcClient.left=1066/13*m_GoldArr[i].m_actionindex;
		rcClient.right = rcClient.left+1066/13;
		rcClient.bottom = rcClient.top+79;
		m_pSprite->Draw(m_GoldImgTexture,rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));


	}

	D3DXMatrixIdentity(&matRot);//����
	m_pSprite->SetTransform(&matRot);
	if(m_IsMustOut)
	{
		m_pSprite->Draw(m_OutTexture, NULL, NULL, &D3DXVECTOR3(180, 300, 0), D3DCOLOR_ARGB(255,255,255,255));
		int nCellCount = 0;
		BYTE byCell[10];
		LONG lCellScore = m_OutTime;
		if(lCellScore>0)
		{
			for(int i = 0;lCellScore>0;lCellScore=lCellScore/10,i++)
			{          
				byCell[i]=lCellScore%10;//�Ӹ�λ��ʼ��ÿλ��
				nCellCount++;
			}
		}
		else
		{
			nCellCount =1;
			byCell[0]=0;
		}
		int temppointx = 0;
		for( int j = nCellCount-1; j >=0; j-- )
		{
			CRect rcClient;
			rcClient.top=0;
			rcClient.left=(byCell[j])*16;
			rcClient.bottom= rcClient.top + 23 ;
			rcClient.right=  rcClient.left +16;
			D3DXMatrixTransformation2D(&mat,NULL,0.0f,&D3DXVECTOR2(1.0f,1.0f),&D3DXVECTOR2(53.0f,12.0f),NULL,&D3DXVECTOR2(740+temppointx,320));
			m_pSprite->SetTransform(&mat);
			m_pSprite->Draw(m_NumImgTexture[0], rcClient, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));	
			temppointx = temppointx + 16;
		}
	}
	//��ԭ�Ƕ�Ϊ����
	m_pSprite->SetTransform(&matRot);
	//���Ʊ߿�
	m_pSprite->Draw(m_BorderImgTexture, NULL, NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255,255,255,255));

	//�����ʻ���������Ϣ
	CRect rcClient;
	rcClient.top=719;
	rcClient.left=70;
	rcClient.bottom= rcClient.top + 24 ;
	rcClient.right=  rcClient.left +150;

	LPCWSTR strLpc = CA2W(m_sUserName);
	m_Dxfont->DrawTextW(m_pSprite,(strLpc),  wcslen(strLpc), &rcClient, DT_TOP | DT_LEFT, D3DCOLOR_ARGB(255,145,240,255));


	//����
	rcClient.top=719;
	rcClient.left=256;
	rcClient.bottom= rcClient.top + 24 ;
	rcClient.right=  rcClient.left +150;
	TCHAR strLpc1[32];
	wsprintf(strLpc1,"%I64d",m_i64MyMoney);
	m_Dxfont->DrawTextW(m_pSprite,CA2W(strLpc1),  wcslen(CA2W(strLpc1)), &rcClient, DT_TOP | DT_LEFT, D3DCOLOR_ARGB(255,0,255,255));

	//���ƹ㲥
	if(m_dqBroadCast.size()>0 && m_dqBroadCast.at(0).bHave)
	{
		rcClient.top=CLIENT_HEIGHT/2+m_iShakey - m_iMoveY;
		rcClient.left=10+m_iShakex;
		rcClient.bottom= rcClient.top + 100 ;
		rcClient.right=  CLIENT_WIDTH - 20;
		TCHAR strLpc2[MAX_PATH];
		wsprintf(strLpc2,"ϵͳ��Ϣ:%s%d����%s������%s,���%I64d��ҽ���!",m_dqBroadCast.at(0).szRoomName,(m_dqBroadCast.at(0).iDeskNo+1),m_dqBroadCast.at(0).szNickName,m_dqBroadCast.at(0).szFishName,m_dqBroadCast.at(0).i64FishScore);
		m_DxCastFont->DrawTextW(m_pSprite,CA2W(strLpc2),  wcslen(CA2W(strLpc2)), &rcClient, DT_TOP | DT_CENTER , D3DCOLOR_ARGB(255,m_iColorR,m_iColorG,m_iColorB));
	}
	

	if(m_cursor.m_have == true)
	{
		m_pSprite->Draw(m_CursorTexture, NULL, NULL, &D3DXVECTOR3(m_cursor.m_ptx, m_cursor.m_pty, 0), D3DCOLOR_ARGB(255,255,255,255));
	}

	//��������
	m_configDlg.InitRender(m_pSprite);

	//���ư�Ť
	DrawBt();
	//������
	m_pSprite->Flush();

	//���ƽ���,ֹͣ����
	m_pSprite->End();
	//ֹͣ�豸
	m_pd3dDevice->EndScene();
	//�����豸����ǰ
	m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

}
//���ư�Ť
void GameScreenWnd::DrawBt()
{
	CRect rcClient;
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[0])rcClient.left=58;
	rcClient.bottom= rcClient.top + 24 ;
	rcClient.right=  rcClient.left +58;
	m_pSprite->Draw(m_BtTexture[0], rcClient, NULL, &D3DXVECTOR3(1092, 3, 0), D3DCOLOR_ARGB(255,255,255,255));
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[1])rcClient.left=58;
	rcClient.bottom= rcClient.top + 24 ;
	rcClient.right=  rcClient.left +58;
	m_pSprite->Draw(m_BtTexture[1], rcClient, NULL, &D3DXVECTOR3(1200, 3, 0), D3DCOLOR_ARGB(255,255,255,255));
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[2])rcClient.left=49;
	rcClient.bottom= rcClient.top + 24 ;
	rcClient.right=  rcClient.left +49;
	m_pSprite->Draw(m_BtTexture[2], rcClient, NULL, &D3DXVECTOR3(1151, 3, 0), D3DCOLOR_ARGB(255,255,255,255));
	
	//�����δ�����꣬�Ͳ���ʾ����2����Ť
	if(!m_InitScreen)return;
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[3])rcClient.left=72;
	rcClient.bottom= rcClient.top + 27 ;
	rcClient.right=  rcClient.left +72;
	m_pSprite->Draw(m_BtTexture[3], rcClient, NULL, &D3DXVECTOR3(500, 708, 0), D3DCOLOR_ARGB(255,255,255,255));
	rcClient.top=0;
	rcClient.left=0;
	if(m_IsMouseOver[4])rcClient.left=72;
	rcClient.bottom= rcClient.top + 20 ;
	rcClient.right=  rcClient.left +72;
	m_pSprite->Draw(m_BtTexture[4], rcClient, NULL, &D3DXVECTOR3(600, 712, 0), D3DCOLOR_ARGB(255,255,255,255));
	

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
	if(m_UserSuperPao[PlayerID])
	{
		if(m_configDlg.m_ptx2>0)
			PlayGameSound(61);
	}
	else 
	{ 
		//if(CBassMusicManager::GetInstance()->IsPlaying(50+PlayerID))CBassMusicManager::GetInstance()->RePlay(50+PlayerID, false);
		if(m_configDlg.m_ptx2>0)
			PlayGameSound(51);
	}
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

	m_dqBroadCast.push_back(TBroadCast);

	if(1 == m_dqBroadCast.size())
	{
		m_iMoveY = 0;
		m_iColorR = 255;
		m_iColorB = 10;
		m_iColorG = 10;
		//SetTimer(TIME_SHOW_MSG,5000,NULL);
	}
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
void GameScreenWnd::UserAddMoney(int PlayerID,int Fishid,int Money,int FishKindScord,bool ishavaction ,bool bMoneyPile)
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

	//�Ƿ���
	if(Deadfish->m_index >=16 && m_configDlg.m_bShakeWindow)
	{
		OutputDebugString("dwjdsnh::����");
		SetTimer(TIME_SHAKE,15,NULL);
		PlayGameSound(35);
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
	if(musciindex<9)
	 {	
		if(m_configDlg.m_ptx2>0)
			PlayGameSound(20+musciindex);
	 }
	else
	 {
		 if(m_configDlg.m_ptx2>0)
			 PlayGameSound(29);
		    
	 }	
	 if(FishKindScord>=20&&ishavaction)
		 GiveMuchMoney(Fishid,PlayerID,Money,Deadfish,FishKindScord);
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
	 int iCount = 0;
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


	 if(bMoneyPile)
	 {
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
    
}
//������
void GameScreenWnd::GiveMuchMoney(int Fishindx,int PlyaerID,int Money,Fish *Deadfish,int fishscore)
{
	m_MuchmoneyAct[PlyaerID].m_have = true;
	m_MuchmoneyAct[PlyaerID].m_nowindex = 0;
	m_MuchmoneyAct[PlyaerID].m_DiuMoney = Money;
	m_MuchmoneyAct[PlyaerID].m_ptx = Deadfish->m_ptx+m_FishmoveRec[Deadfish->m_index][0]/2-1024/2;
	m_MuchmoneyAct[PlyaerID].m_pty = Deadfish->m_pty+m_FishmoveRec[Deadfish->m_index][1]/2-512/2;
	if(m_configDlg.m_ptx2>0)
		PlayGameSound(41);
  
}
//�л�����
void GameScreenWnd::ChangeScreen(int bgindex)
{

	if(m_configDlg.m_ptx2>0)
		PlayGameSound(40);
	//���ű�������
	PlayBgSound(bgindex);

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

	
	m_configDlg.LMouseMove(point.x,point.y);

	//SetCursor(LoadCursor((HINSTANCE)GetWindowLong(this->GetSafeHwnd(),GWL_HINSTANCE),MAKEINTRESOURCE(IDC_CURSOR1)));
	SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));

	/*if(LoadCursor(NULL,MAKEINTRESOURCE(IDC_CURSOR1)))
	{
		OutputDebugString("dwjjcby::--------");
	}	
	else
	{
		OutputDebugString("dwjjcby::-------2222-");
	}*/

	if(point.y>710||point.y<30||point.x<5||point.x>1275||m_configDlg.m_isvisable)
	{
		m_cursor.m_have = false;
		if(m_CursorInt<0)
		{
			m_CursorInt = ShowCursor(TRUE);
		}
	}
	else 
	{
		if(m_InitScreen)
		{
			if(m_CursorInt>=0)m_CursorInt = ShowCursor(FALSE);
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
	for(int i=0;i<5;i++)
	{
		if(m_BtRect[i].PtInRect(point))
			m_IsMouseOver[i]=true;
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
		m_pGameClient->OnMoveWindow(point);
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
			m_pGameClient->OnUserShot(false,m_UserPaoJiaodu[m_MeChariID]);
			UserShoot(m_MeChariID,m_UserPaoJiaodu[m_MeChariID],m_MeRellayChairID,false);
			m_NowTime = GetTickCount() ;

		}
		SetTimer(TIMERIDSENDSHOOT,400,NULL);
		//����Ϊ��갴��״̬
		m_MouseDown = true;
	}
	//��Ť�¼�
	if(m_InitScreen&&m_BtRect[4].PtInRect(point))
	{
		//�Ϸ�
		m_pGameClient->AddOrRemoveScore(true,m_BeiLv[m_MeChariID]);
		if(m_configDlg.m_ptx2>0)
			PlayGameSound(15);
	}
	if(m_InitScreen&&m_BtRect[3].PtInRect(point))
	{
		//�·�
		m_pGameClient->AddOrRemoveScore(false,m_BeiLv[m_MeChariID]);
		if(m_configDlg.m_ptx2>0)
			PlayGameSound(16);
	}
	//��С��
	if(m_BtRect[0].PtInRect(point))
	{
		m_pGameClient->OnMinWondow();
	}
	//�˳�
	if(m_BtRect[1].PtInRect(point))
	{
		//��û�����겻���˳� ��Ϊ�ᵼ�¿ͻ��˱���
		if(m_InitScreen==false || m_bHaveLinked == false)
		{
			return;
		}
		m_pGameClient->OnCloseWindow();
	}
	//����
	if(m_BtRect[2].PtInRect(point))
	{
		m_configDlg.m_isvisable = true;
	}
}
/*----------------------------------------------------------------------------------*/
void	GameScreenWnd::OnLButtonUp(UINT, CPoint ponit)
{
	m_configDlg.LMouseUp(ponit.x,ponit.y);
	//�������״̬Ϊ�ɿ�
	m_MouseDown =  false;
	
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
	if(!m_InitScreen || !m_bHaveLinked)
	{
		return ;
	}
	switch( (WCHAR)nChar )
	{
	case VK_SPACE://�ո���
		{
			if(GetTickCount() - m_NowTime > 400 )
			{
				//���ûǮֱ�ӵ���ȥ
				if(m_UserScore[m_MeChariID]<=0)break;
				if(m_BeiLv[m_MeChariID]>m_UserScore[m_MeChariID]) return ; //edit by guojm
				m_pGameClient->OnUserShot(false,m_UserPaoJiaodu[m_MeChariID]);
				UserShoot(m_MeChariID,m_UserPaoJiaodu[m_MeChariID],m_MeRellayChairID,false);
				m_NowTime = GetTickCount() ;
				KillTimer(TIMERIDSENDSHOOT);
			}
			//SetTimer(TIMERIDSENDSHOOT,400,NULL);
			break;
		}
	case VK_UP://�ϼ�����
		{
			m_pGameClient->OnUserAddFire(true);
			break;
		}
	case VK_DOWN://�¼���
		{
			m_pGameClient->OnUserAddFire(false);
			break;
		}
	case VK_LEFT://���Ϸ�
		{
			m_pGameClient->AddOrRemoveScore(true,m_BeiLv[m_MeChariID]);
			if(m_configDlg.m_ptx2>0)
			{
				PlayGameSound(15);
			}
			break;
		}
	case VK_RIGHT://���·�
		{
			m_pGameClient->AddOrRemoveScore(false,m_BeiLv[m_MeChariID]);
			if(m_configDlg.m_ptx2>0)
			{
				PlayGameSound(16);
			}
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
	OutputDebugString("dwjjcby::���̵���");
}
/*----------------------------------------------------------------------------------*/
//ʱ���¼�
void GameScreenWnd::OnTimer(UINT nIDEvent)
{
	if(TIME_SHOW_MSG == nIDEvent)	//��ʾ�㲥
	{
		if(m_dqBroadCast.size() > 0)
		{
			m_dqBroadCast.pop_front();
		}
		else
		{
			KillTimer(TIME_SHOW_MSG);
		}
	}
	else if(TIME_SHAKE == nIDEvent)
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

			float m_x = 8*sin(2*3.1415926*(m_MeBullet[i].m_Roation)/360.0);
			float m_y = 8*cos(2*3.1415926*(m_MeBullet[i].m_Roation)/360.0);
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


		if(m_dqBroadCast.size()>0)
		{
			m_iMoveY += 10;
			if(m_iMoveY >= CLIENT_HEIGHT/2)
			{
				m_iMoveY = 0;

				m_dqBroadCast.pop_front();
			}
			else
			{
				if(0 == m_iColorB)
				{
					m_iColorR = 255;
					m_iColorB = 255;
					m_iColorG = 255;
				}
				else
				{
					m_iColorR = 255;
					m_iColorB = 10;
					m_iColorG = 10;
				}
			}
		}
	}
	//���Ǯ���ƶ��������Աߴ���������3��Ǯ
	else if(nIDEvent == TIMERIDUSERMONEY)
	{
		for(int i=0;i<PLAY_COUNT;i++)
		{
			//�ж��Ƿ������
			if(!m_HaveUser[i])continue;
			for(int j=0;j<3;j++)
			{
				if(!m_UserStruct[i][j].m_Have)continue;
				if(j==0&&m_UserStruct[i][1].m_Have&&m_UserStruct[i][j].m_Time>=50)
				{
					m_UserStruct[i][j].m_Have = false;
					m_UserStruct[i][j].m_Have =  m_UserStruct[i][j+1].m_Have;
					m_UserStruct[i][j].m_Money =  m_UserStruct[i][j+1].m_Money;
					m_UserStruct[i][j].m_Time =  0;
					m_UserStruct[i][j].m_FishScore = m_UserStruct[i][j+1].m_FishScore;
					m_UserStruct[i][j].m_Nowscore =  m_UserStruct[i][j+1].m_Nowscore;
					m_UserStruct[i][j].m_Have = true;
					m_UserStruct[i][1].m_Have = false;

					if(m_UserStruct[i][2].m_Have)
					{
						m_UserStruct[i][1].m_Have = true;
						m_UserStruct[i][1].m_Have =  m_UserStruct[i][2].m_Have;
						m_UserStruct[i][1].m_Money =  m_UserStruct[i][2].m_Money;
						m_UserStruct[i][1].m_FishScore = m_UserStruct[i][2].m_FishScore;
						m_UserStruct[i][1].m_Nowscore =  m_UserStruct[i][2].m_Nowscore;
						m_UserStruct[i][1].m_Time =  0;
						m_UserStruct[i][2].m_Have = false;

					}

				}
				else if(j==0&&!m_UserStruct[i][1].m_Have&&m_UserStruct[i][j].m_Time>=50)
				{
					m_UserStruct[i][j].m_Have = false;
				}
				m_UserStruct[i][j].m_Time++;
			}
		}
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
			m_pGameClient->OnUserShot(false,m_UserPaoJiaodu[m_MeChariID]);
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
			m_pGameClient->OnCloseWindow();
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
    m_sUserName		= sNickName;
	m_i64MyMoney	= i64money;
	m_MeChariID		= MePlayerid;
	m_MeRellayChairID = MeRellid;
}
/*----------------------------------------------------------------------------------*/
//�������������ڴ棬�˳�ʱ��ʹ��
void GameScreenWnd::CleanAllMemory()
{
	m_RunRender = false;
   //�ͷ�����
	if(m_SupStatTexture != NULL)
		m_SupStatTexture->Release();

	if(m_LoadTexture != NULL)
		m_LoadTexture->Release();

	if(m_LoadBgTexture != NULL)
		m_LoadBgTexture->Release();

	if(m_BiTexture != NULL)
		m_BiTexture->Release();

	if(m_PaoTwoNetImgTexture != NULL)
		m_PaoTwoNetImgTexture->Release();

	if(m_GoldImgTexture != NULL)
		m_GoldImgTexture->Release();

	if(m_PaoThreeFireImgTexture != NULL)
		m_PaoThreeFireImgTexture->Release();

	if(m_PaoThreeImgTexture != NULL)
		m_PaoThreeImgTexture->Release();

	if(m_PaoTwoFireImgTexture != NULL)
		m_PaoTwoFireImgTexture->Release();

	if(m_PaoTwoImgTexture != NULL)
		m_PaoTwoImgTexture->Release();

	if(m_PaoBgImgTexture != NULL)
		m_PaoBgImgTexture->Release();

	if(m_PaoConImgTexture != NULL)
		m_PaoConImgTexture->Release();

	if(m_BorderImgTexture != NULL)
		m_BorderImgTexture->Release();


	for(int i=0;i<2;i++)
		if(m_SupButTexture[i] != NULL)
		   m_SupButTexture[i]->Release();
	for(int i=0;i<32;i++)
		if(m_WaterTexture[i] != NULL)
		   m_WaterTexture[i]->Release();

	for(int i=0;i<2;i++)
		if(m_WaterWaveTexture[i] != NULL)
		   m_WaterWaveTexture[i]->Release();

	for(int i=0;i<5;i++)
		if(m_BtTexture[i] != NULL)
		   m_BtTexture[i]->Release();

	for(int i=0;i<3;i++)
		if(m_QuanTexture[i] != NULL)
		   m_QuanTexture[i]->Release();

	for(int i=0;i<22;i++)
		if(m_MuchMoneyTexture[i] != NULL)
		   m_MuchMoneyTexture[i]->Release();

	for(int i=0;i<23;i++)
		 for(int j=0;j<10;j++)
			 if(m_FishDeadTexture[i][j] != NULL)
		         m_FishDeadTexture[i][j]->Release();

	for(int i=0;i<23;i++)
		 for(int j=0;j<10;j++)
			 if(m_FishMoveTexture[i][j] != NULL)
		         m_FishMoveTexture[i][j]->Release();


	for(int i=0;i<2;i++)
		if(m_BulltTwoTexture[i] != NULL)
		   m_BulltTwoTexture[i]->Release();

	for(int i=0;i<2;i++)
		if(m_NumImgTexture[i] != NULL)
		   m_NumImgTexture[i]->Release();

	for(int i=0;i<2;i++)
		if(m_bgImgTexture[i] != NULL)
		   m_bgImgTexture[i]->Release();

	//�ͷŵ㾫��
	if (m_pSprite != NULL)
		m_pSprite->Release();

	//�ͷ�Direct3D�豸����
    if( m_pd3dDevice != NULL ) 
        m_pd3dDevice->Release();

	//�ͷ�Direct3D����
    if( m_pD3D != NULL )       
        m_pD3D->Release();

}
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

					if(m_configDlg.m_ptx2>0)
						PlayGameSound(13);
					//������ײ���ӵ�״̬
					m_MeBullet[i].m_Netindex = 0;
					m_MeBullet[i].m_Stop = true;
					//���ñ������������
					//�������ӵ����ҷ����
					if(m_MeBullet[i].m_Senduser==m_MeChariID)
					{
						//������Ϣ�������
						//							m_baseSink->HitFirsh(tfish->m_fishID,i,m_MeBullet[i].m_RealUser,m_MeBullet[i].m_isAndroid);
						m_pGameClient->OnUserHitFish(tfish->m_fishID,i,m_MeBullet[i].m_RealUser,m_MeBullet[i].m_isAndroid);
						bShoot=true;
						//�ж��ж�
						break;
					}
					else if((m_MeBullet[i].m_isAndroid && m_bHitFlag && m_bHitDesk == m_MeRellayChairID))
					{
						//������Ϣ�������
						m_pGameClient->OnUserHitFish(tfish->m_fishID,i-100,m_MeBullet[i].m_RealUser,m_MeBullet[i].m_isAndroid);
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
void GameScreenWnd::LoadSound()
{
    CBassMusicManager::GetInstance()->Init(this->m_hWnd);
	//��������
	//��������
	CBassMusicManager::GetInstance()->LoadFromFile(0, "Jcfish/music/bg_01.mp3",true);
	CBassMusicManager::GetInstance()->LoadFromFile(1, "Jcfish/music/bg_02.mp3",true);
	CBassMusicManager::GetInstance()->LoadFromFile(2, "Jcfish/music/bg_03.mp3",true);
    //�����ӵ�����
	CBassMusicManager::GetInstance()->LoadFromFile(51, "Jcfish/music/shot8.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(61, "Jcfish/music/shot_8.mp3",false);

	//��������
    CBassMusicManager::GetInstance()->LoadFromFile(13, "Jcfish/music/net_8.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(14, "Jcfish/music/net_1.mp3",false);
	//��Ǯ����
    CBassMusicManager::GetInstance()->LoadFromFile(15, "Jcfish/music/jia.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(16, "Jcfish/music/jia1.mp3",false);
	//��������
	CBassMusicManager::GetInstance()->LoadFromFile(20, "Jcfish/music/die00.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(21, "Jcfish/music/die01.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(22, "Jcfish/music/die02.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(23, "Jcfish/music/die03.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(24, "Jcfish/music/die04.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(25, "Jcfish/music/die05.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(26, "Jcfish/music/die06.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(27, "Jcfish/music/die07.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(28, "Jcfish/music/die07.mp3",false);
	CBassMusicManager::GetInstance()->LoadFromFile(29, "Jcfish/music/fishdiescore.mp3",false);

	CBassMusicManager::GetInstance()->LoadFromFile(35, "Jcfish/music/bomb.ogg",false);	//��ը��
	//
	CBassMusicManager::GetInstance()->LoadFromFile(71, "Jcfish/music/laserShot.mp3",false);
	//�л���������
    CBassMusicManager::GetInstance()->LoadFromFile(40, "Jcfish/music/senceConver.mp3",false);
	//��������
    CBassMusicManager::GetInstance()->LoadFromFile(41, "Jcfish/music/Bigdie01.mp3",false);
	CBassMusicManager::GetInstance()->SetVolumn(100);
	//���ű�������
	PlayBgSound(1);
}

/*----------------------------------------------------------------------------------------------*/
//���ű�������
void	GameScreenWnd::PlayBgSound(int iIndex)
{
	if(!m_configDlg.m_bBgSound)
	{
		return;
	}
	CBassMusicManager::GetInstance()->Stop(0);
	CBassMusicManager::GetInstance()->Stop(1);
	CBassMusicManager::GetInstance()->Stop(2);
	CBassMusicManager::GetInstance()->Play(iIndex, false);
}
/*----------------------------------------------------------------------------------------------*/
//ֹͣ������Ч
void	GameScreenWnd::StopBgSound()
{
	CBassMusicManager::GetInstance()->Stop(0);
	CBassMusicManager::GetInstance()->Stop(1);
	CBassMusicManager::GetInstance()->Stop(2);
}
/*----------------------------------------------------------------------------------------------*/
//��Ч�Ƿ����ڲ���
bool	GameScreenWnd::IsPlaying(int iIndex)
{
	return	CBassMusicManager::GetInstance()->IsPlaying(iIndex);
}
/*----------------------------------------------------------------------------------------------*/
//������Ϸ��Ч
void	GameScreenWnd::PlayGameSound(int iIndex)
{
	if(!m_configDlg.m_bGameSound)
	{
		return;
	}
	CBassMusicManager::GetInstance()->Play(iIndex, false);
}
/*----------------------------------------------------------------------------------------------*/
//ֹͣ��Ϸ��Ч
void	GameScreenWnd::StopGameSound()
{
	return;
}
/*----------------------------------------------------------------------------------------------*/

//����ͼƬ����
void GameScreenWnd::LoadImg(const char* m_path,LPDIRECT3DTEXTURE9* ppTexture)
{
	unsigned long nSize = 0;
	FILE *fp = fopen(m_path, "rb");
	fseek(fp,0,SEEK_END);
	//��ȡ��С
	nSize = ftell(fp);
	//������������
	char * buffer = new char[4500000];
	//����
	FilesParmesA * PngBuffdata = (FilesParmesA *)buffer;
	//��ȡ����
	fseek(fp,0,SEEK_SET);
	nSize = fread(PngBuffdata,sizeof(char), nSize,fp);
	fclose(fp);
	//����ͼƬ��Ϣ������
	fseek(fp,0,SEEK_SET);
	//�������������ڴ�
	unsigned char* pBuffer = new unsigned char[PngBuffdata->m_PngSize];
	memcpy(pBuffer, PngBuffdata + 1, PngBuffdata->m_PngSize);
	nSize = PngBuffdata->m_PngSize;
	do{

		if(!FAILED(D3DXCreateTextureFromFileInMemory(m_pd3dDevice,pBuffer,nSize,ppTexture)))
			break;

	}while(true);
	delete pBuffer;
	delete buffer;
				
}

