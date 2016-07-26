#include "StdAfx.h"
#include "ServerManage.h"
#include "../../../../sdk/include/Common/writelog.h"
#include <math.h>


/*---------------------------------------------------------------------------*/
/*
* ��ӡ��־�ļ�
*/
void DebugPrintf(int iRoomID, const char *p, ...)
{
	return;
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
/*---------------------------------------------------------------------------*/
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bIsPlaying = false;		//��Ϸ�Ƿ��Ѿ���ʼ
	m_bHaveHitFlag = false;
	m_byHitDesk = 255;
	m_iBgindex	 = 0;			//��������	
	m_iCreateFishTime = 8;		//�������ʱ��
	m_iChangeScene = 15;		//�л�����ʱ��
	m_iRegfishcount = 0;		//���������
	m_iFireBase	 = 100;			//�����ڻ���
	m_iMaxFireBase = 1000;			//����ڻ���
	m_iMinFire = m_iFireBase;	//��С�ڻ���
	m_iBiliDuihuan = 1;			//�һ�����
	

	for(int i=0; i<26; i++)
	{
		//��ı���
		m_iFishBase[i] = G_iFishScore[i];
		//��ɱǹ��
		for(int j=0; j<3; j++)
		{
			m_fishShoot[i][j] = (i+1)*(j+1);
		}
	}

	//��Ӧ���������·�ߣ�5������ͽǶ�
	memset(m_fishtrace,0,sizeof(m_fishtrace));
	//����ӵ���Ϣ
	memset(m_UserShoot,0,sizeof(m_UserShoot));
	//��Ⱥ��ʶ
	m_dwFishID = 0;
	//��ҵ��ڻ���
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_iUserFireBase[i]	 = m_iFireBase;
		m_iUserScore[i]		 = 0;
		m_i64UserAllScore[i] = 0;
		m_i64UserMoney[i]	 = 0;
		m_iUserShootCount[i] = 0;

		m_bUserIsSuperPao[i] = false;

		m_iUserPercent[i]	= 100;
	}
	//�����û�
	m_lvSuperUserID.clear();
	//������
	m_lvBlackListID.clear();
	InitializeCriticalSection(&m_csForIPCMessage);
}
/*---------------------------------------------------------------------------*/
//��������
CServerGameDesk::~CServerGameDesk(void)
{
	DeleteCriticalSection(&m_csForIPCMessage);
}

/*---------------------------------------------------------------------------*/
bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadIni();
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_iUserFireBase[i]	 = m_iFireBase;		
	}
	return true;
}
/*---------------------------------------------------------------------------*/
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	if (!CINIFile::IsFileExist(temp))
	{
		return true;
	}
	CINIFile f(temp);
	CString key = TEXT("game");

	m_iCreateFishTime	= f.GetKeyVal(key,"CreateFishTime",8);		//�������ʱ��
	m_iChangeScene		= f.GetKeyVal(key,"ChangeSceneTime",15);	//�л�����ʱ��(����)
	m_iFireBase			= f.GetKeyVal(key,"FireBase",100);			//�����ڻ���
	m_iMinFire			= m_iFireBase;								//��С�ڻ���
	m_iMaxFireBase		= f.GetKeyVal(key,"MaxFireBase",1000);		//����ڻ���
	m_iBiliDuihuan		= f.GetKeyVal(key,"BiliDuihuan",1);			//�һ�����

	//���صȼ�
	G_i64RewardLevel[0] = f.GetKeyVal(key,"RewardLeve1",(__int64)10000000);
	G_i64RewardLevel[1] = f.GetKeyVal(key,"RewardLeve2",(__int64)100000000);
	G_i64RewardLevel[2] = f.GetKeyVal(key,"RewardLeve3",(__int64)1000000000);

	//���صȼ���Ӧ�ĸ���ֵ
	G_fRewardLevelProbability[0] = atof(f.GetKeyVal(key,"LevelProbability1","0"));
	G_fRewardLevelProbability[1] = atof(f.GetKeyVal(key,"LevelProbability2","0"));
	G_fRewardLevelProbability[2] = atof(f.GetKeyVal(key,"LevelProbability3","0"));

	//��ǰ���صĽ��
	G_i64CurrPoolMoney	= f.GetKeyVal(key,"CurrPoolMoney",(__int64)0);
	//��һ�εĽ��ؽ��
	G_i64PrePoolMoney	= G_i64CurrPoolMoney;


	//��Ļ���ǹ��
	key = TEXT("FishShoot");
	CString sText;
	for(int i=0; i<26;i++)
	{
		for(int j=0;j<3;j++)
		{
			sText.Format("Fish_%d_Shoot_%d",i,j);
			m_fishShoot[i][j] = f.GetKeyVal(key,sText,m_fishShoot[i][j]);	
		}
	}
	//�������
	key = TEXT("FishBase");
	for(int i=0; i<26;i++)
	{
		sText.Format("Fish_%d_Base",i);
		m_iFishBase[i] = f.GetKeyVal(key,sText,G_iFishScore[i]);	
	}

	int iCount	 = 0;
	long lUserID = 0;
	//�����û�����
	key = TEXT("SuperUser");
	iCount = f.GetKeyVal(key,"SuperUserCount",1);
	for(int i=1; i<=iCount;i++)
	{
		sText.Format("SuperUserID%d",i);
		lUserID = f.GetKeyVal(key,sText,1);
		m_lvSuperUserID.push_back(lUserID);
	}

	//������
	key = TEXT("BlackList");
	//����������
	iCount = f.GetKeyVal(key,"BlackListCount",1);
	for(int i=1; i<=iCount;i++)
	{
		sText.Format("BlackListID%d",i);
		lUserID = f.GetKeyVal(key,sText,1);
		m_lvBlackListID.push_back(lUserID);
	}


	return true;
}
/*---------------------------------------------------------------------------*/
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{

	CString s = CINIFile::GetAppPath ();/////����·��
	TCHAR skin[MAX_NAME_INFO];
	CString temp;
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	CINIFile f(temp);
	CString key;
	key.Format("%d_%d",NAME_ID,iRoomID);


	m_iCreateFishTime	= f.GetKeyVal(key,"CreateFishTime",m_iCreateFishTime);	//�������ʱ��
	m_iChangeScene		= f.GetKeyVal(key,"ChangeSceneTime",m_iChangeScene);	//�л�����ʱ��(����)
	m_iFireBase			= f.GetKeyVal(key,"FireBase",m_iFireBase);				//�����ڻ���
	m_iMinFire			= m_iFireBase;											//��С�ڻ���
	m_iMaxFireBase		= f.GetKeyVal(key,"MaxFireBase",m_iMaxFireBase);		//����ڻ���
	m_iBiliDuihuan		= f.GetKeyVal(key,"BiliDuihuan",m_iBiliDuihuan);		//�һ�����

	//���صȼ�
	G_i64RewardLevel[0] = f.GetKeyVal(key,"RewardLeve1",G_i64RewardLevel[0]);
	G_i64RewardLevel[1] = f.GetKeyVal(key,"RewardLeve2",G_i64RewardLevel[1]);
	G_i64RewardLevel[2] = f.GetKeyVal(key,"RewardLeve3",G_i64RewardLevel[2]);

	//���صȼ���Ӧ�ĸ���ֵ
	if("-1" != f.GetKeyVal(key,"LevelProbability1","-1"))
	{
		G_fRewardLevelProbability[0] = atof(f.GetKeyVal(key,"LevelProbability1","0"));
	}
	if("-1" != f.GetKeyVal(key,"LevelProbability2","-1"))
	{
		G_fRewardLevelProbability[1] = atof(f.GetKeyVal(key,"LevelProbability2","0"));
	}
	if("-1" != f.GetKeyVal(key,"LevelProbability3","-1"))
	{
		G_fRewardLevelProbability[2] = atof(f.GetKeyVal(key,"LevelProbability3","0"));
	}

	//��ǰ���صĽ��
	G_i64CurrPoolMoney	= f.GetKeyVal(key,"CurrPoolMoney",G_i64CurrPoolMoney);
	//��һ�εĽ��ؽ��
	G_i64PrePoolMoney	= G_i64CurrPoolMoney;
	
	return TRUE;
}
/*---------------------------------------------------------------------------*/
//��ո�λ�õ�����
void	CServerGameDesk::IniUserData(BYTE byStation)
{
	if(byStation < 0 || byStation >= PLAY_COUNT)
	{
		return;
	}

	m_iUserFireBase[byStation]	= m_iFireBase;	//��ҵ��ڻ���
	m_iUserScore[byStation]		= 0;			//����Ϸ���
	m_i64UserMoney[byStation]	= 0;			//������ϵĽ����
	m_i64UserAllScore[byStation]= 0;			//������Ϸ�
	m_iUserShootCount[byStation]= 0;			//��ҷ����ӵ��ĸ���
	m_bUserIsSuperPao[byStation]= false;		//�Ƿ񳬼��ڵ�
	for(int i=0; i<100; i++)
	{
		m_UserShoot[byStation][i].bHave = false; //����ӵ���Ϣ
	}

	if(m_byHitDesk == byStation)
	{
		m_bHaveHitFlag = false;
		m_byHitDesk = 255;
	}
	
}

/*---------------------------------------------------------------------------*/
bool	CServerGameDesk::OnTimer(UINT uTimerID)
{
	if ((uTimerID >= TIME_MAX) || (uTimerID <= TIME_MIN))
	{
		return __super::OnTimer(uTimerID);
	}

	switch (uTimerID)
	{
	case TIME_BUILD_TRACE:		//�������
		{
			KillTimer(TIME_BUILD_TRACE);
			CreatRegSmalFish();
			//����С��
			CreatSmalFish();
			//�������1
			CreateFishTraceOne();
			//�������2
			CreateFishTraceTwo();
			//�������3
			CreateFishTraceThree();
			//�������4
			CreateFishTraceFour();
			//����������ɼ�ʱ��
			SetTimer(TIME_BUILD_TRACE, m_iCreateFishTime*1000); 
			return true;
		}
	case TIME_CREAD_FISH:
		{
			KillTimer(TIME_CREAD_FISH);
			CreadFish();

			SetTimer(TIME_CREAD_FISH, m_iCreateFishTime*1100); 
			return true;
		}
	case TIME_CLEAR_TRACE:		//�������
		{
			ClearFishTrace();
			return true;
		}
	case TIME_CHANGE_SCENE:		//�л�����
		{
			ChangeScence();
			return true;
		}
	case TIME_REGULAR_FISH:		//����
		{
			KillTimer(TIME_REGULAR_FISH);
			if(m_iBgindex == 0)
			{
				RegFishone();
			}
			else if (m_iBgindex==1)
			{
				RegFishtwo();
			}
			else 
			{
				RegFishthree();
			}
			return true;
		}
	case TIME_SUPER_PAO_0:		//���������ڵ���ʱ��
		{
			KillTimer(TIME_SUPER_PAO_0);
			//ȡ����ҵĳ�����
			CanCelUserSuperPao(0);
			
			return true;
		}
	case TIME_SUPER_PAO_1:		//���������ڵ���ʱ��
		{
			KillTimer(TIME_SUPER_PAO_1);
			CanCelUserSuperPao(1);
			return true;
		}
	case TIME_SUPER_PAO_2:		//���������ڵ���ʱ��
		{
			KillTimer(TIME_SUPER_PAO_2);
			CanCelUserSuperPao(2);
			return true;
		}
	case TIME_SUPER_PAO_3:		//���������ڵ���ʱ��
		{
			KillTimer(TIME_SUPER_PAO_3);
			CanCelUserSuperPao(3);
			return true;
		}
	case TIME_SUPER_PAO_4:		//���������ڵ���ʱ��
		{
			KillTimer(TIME_SUPER_PAO_4);
			CanCelUserSuperPao(4);
			return true;
		}
	case TIME_SUPER_PAO_5:		//���������ڵ���ʱ��
		{
			KillTimer(TIME_SUPER_PAO_5);
			CanCelUserSuperPao(5);
			return true;
		}
	}

	return true;
}
/*---------------------------------------------------------------------------*/
//���ĸ����ϳ���
void	CServerGameDesk::CreadFish()
{
	CMD_S_FishTrace		TFishTrace;
	int tempcount = 0;
	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_isHave==true)continue;

		memset(&TFishTrace, 0, sizeof(TFishTrace));
		if(tempcount==0)
		{
			m_fishtrace[i][0].x = -200;
			m_fishtrace[i][0].y = -50;
			m_fishtrace[i][0].rotation = 125;
		}
		else if(tempcount==1)
		{
			m_fishtrace[i][0].x = 1280+200;
			m_fishtrace[i][0].y = 790;
			m_fishtrace[i][0].rotation = 305;

		}
		else if(tempcount==2)
		{
			m_fishtrace[i][0].x = -200;
			m_fishtrace[i][0].y = 790;
			m_fishtrace[i][0].rotation = 55;
		}
		else 
		{
			m_fishtrace[i][0].x = 1280+200;
			m_fishtrace[i][0].y = -50;
			m_fishtrace[i][0].rotation = 235;

		}

		int xj = 45;//rand()%40+30;
		int yj = 90;//rand()%30+30;
		int zhengfu = rand()%2;
		if(tempcount==0||tempcount==1)
		{
			xj =  -xj;
			yj =  -yj;
		}
		int tempd  = rand()%10;
		int fishtype = (rand()+tempcount)%14;
		if(tempd < 1)
		{
			fishtype = (rand()+tempcount)%24;
		}
		else if(tempd > 1  && tempd <=3)
		{
			fishtype = (rand()+tempcount)%14;
		}
		else
		{
			fishtype = (rand()+tempcount)%10+14;
		}
		m_fishtrace[i][0].m_fishtype	=fishtype;
		m_fishtrace[i][0].movetime		= 250+rand()%100;				   
		m_fishtrace[i][0].m_isHave		=true;
		m_fishtrace[i][0].m_ptindex		= i;
		m_fishtrace[i][0].m_BuildTime	=  GetTickCount();
		m_fishtrace[i][0].m_fishid		=  GetNewFishID();
		m_fishtrace[i][0].m_shootCount	= 0;
		m_fishtrace[i][0].m_speed = rand()%6+5;
		int mustshoot = rand()%10;
		if(mustshoot<3)mustshoot=0;
		else  if(mustshoot>=3&&mustshoot<6)mustshoot=1;
		else mustshoot= 2;
		m_fishtrace[i][0].m_mustShoot = m_fishShoot[m_fishtrace[i][0].m_fishtype][mustshoot];
		m_fishtrace[i][0].m_fudaifishtype = -1;

		m_fishtrace[i][1].movetime = 280+rand()%100;			
		m_fishtrace[i][1].rotation = (int(m_fishtrace[i][0].rotation)+xj)%360;
		m_fishtrace[i][1].changetime = 2;
		m_fishtrace[i][1].m_speed = rand()%4+5;

		m_fishtrace[i][2].movetime = 200+rand()%100;
		m_fishtrace[i][2].rotation = (int(m_fishtrace[i][0].rotation)+yj)%360;
		m_fishtrace[i][2].changetime = 4;
		m_fishtrace[i][2].m_speed = rand()%3+5;

		m_fishtrace[i][3].movetime = 100+rand()%100000;
		m_fishtrace[i][3].rotation = (int(m_fishtrace[i][0].rotation)+rand()%45)%360;
		m_fishtrace[i][3].changetime = 6;
		m_fishtrace[i][3].m_speed = rand()%4+5;

		m_fishtrace[i][4].movetime = 1000000000;
		m_fishtrace[i][4].rotation = (int(m_fishtrace[i][0].rotation)+rand()%45)%360;
		m_fishtrace[i][4].changetime = 6;
		m_fishtrace[i][4].m_speed = rand()%5+5;


		memcpy(&TFishTrace.m_fishtrace,&m_fishtrace[i],sizeof(TFishTrace.m_fishtrace));

		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTrace,sizeof(TFishTrace),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
			}
		}

		tempcount++;
		if(tempcount>5)break;
	}
}
/*---------------------------------------------------------------------------*/
//���ɹ���С��
void	CServerGameDesk::CreatRegSmalFish()
{
	int isrentur = rand()%8;
	int fishtye =  rand()%15;
	int fishcount = rand()%3+3;
	int fangxiang = 0;
	if(fishtye==9)return;


	CMD_S_FishTrace TFishTraceRegSmall;
	int tempcount = 0;
	if(isrentur==0)
	{
		for(int i=0;i<250;i++)
		{
			if(m_fishtrace[i][0].m_isHave==true)continue;
			memset(&TFishTraceRegSmall, 0,sizeof(TFishTraceRegSmall));
			if(fangxiang==0)
			{

				m_fishtrace[i][0].x = -200 +(-102* tempcount);
				m_fishtrace[i][0].y = 800 +(102* tempcount);
				m_fishtrace[i][0].rotation = 45;

			}

			m_fishtrace[i][0].m_fishtype = fishtye;
			m_fishtrace[i][0].movetime = 400+tempcount*100;
			m_fishtrace[i][0].m_isHave=true;
			m_fishtrace[i][0].m_ptindex = i;
			m_fishtrace[i][0].m_BuildTime =  GetTickCount();
			m_fishtrace[i][0].m_fishid =  GetNewFishID();
			m_fishtrace[i][0].m_shootCount = 0;
			m_fishtrace[i][0].m_speed = 5;
			int mustshoot = rand()%10;
			if(mustshoot<2)mustshoot=0;
			else  if(mustshoot>=3&&mustshoot<5)mustshoot=1;
			else mustshoot= 2;
			m_fishtrace[i][0].m_mustShoot = m_fishShoot[m_fishtrace[i][0].m_fishtype][mustshoot];
			m_fishtrace[i][0].m_fudaifishtype = -1;


			m_fishtrace[i][1].movetime = 420;
			m_fishtrace[i][1].m_speed = 6;
			m_fishtrace[i][1].changetime = 10;
			m_fishtrace[i][1].rotation = 90;

			m_fishtrace[i][2].movetime = 120000000;
			m_fishtrace[i][2].m_speed = 6;
			m_fishtrace[i][2].changetime = 10;
			m_fishtrace[i][2].rotation = 40;

			memcpy(&TFishTraceRegSmall.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceRegSmall.m_fishtrace));

			for(int m=0; m<PLAY_COUNT; m++)
			{
				if(NULL != m_pUserInfo[m])
				{
					SendGameData(m,&TFishTraceRegSmall,sizeof(TFishTraceRegSmall),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
				}
			}

			tempcount++;
			if(tempcount>fishcount)break;
		}
	}
	else if(isrentur==1)
	{
		for(int i=0;i<250;i++)
		{
			if(m_fishtrace[i][0].m_isHave==true)continue;

			memset(&TFishTraceRegSmall, 0,sizeof(TFishTraceRegSmall));
			if(fangxiang==0)
			{
				m_fishtrace[i][0].x = -200 +(-102* tempcount);
				m_fishtrace[i][0].y = 350;
				m_fishtrace[i][0].rotation = 90;
			}

			m_fishtrace[i][0].m_fishtype = fishtye;
			m_fishtrace[i][0].movetime = 300+tempcount*100;
			m_fishtrace[i][0].m_isHave=true;
			m_fishtrace[i][0].m_ptindex = i;
			m_fishtrace[i][0].m_BuildTime =  GetTickCount();
			m_fishtrace[i][0].m_fishid =  GetNewFishID();
			m_fishtrace[i][0].m_shootCount = 0;
			m_fishtrace[i][0].m_speed = 5;
			int mustshoot = rand()%10;
			if(mustshoot<2)mustshoot=0;
			else  if(mustshoot>=3&&mustshoot<5)mustshoot=1;
			else mustshoot= 2;
			m_fishtrace[i][0].m_mustShoot = m_fishShoot[m_fishtrace[i][0].m_fishtype][mustshoot];
			m_fishtrace[i][0].m_fudaifishtype = -1;


			m_fishtrace[i][1].movetime = 220;
			m_fishtrace[i][1].m_speed = 6;
			m_fishtrace[i][1].changetime = 10;
			m_fishtrace[i][1].rotation = 135;

			m_fishtrace[i][2].movetime = 120000000;
			m_fishtrace[i][2].m_speed = 6;
			m_fishtrace[i][2].changetime = 10;
			m_fishtrace[i][2].rotation = 45;

			memcpy(&TFishTraceRegSmall.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceRegSmall.m_fishtrace));

			for(int m=0; m<PLAY_COUNT; m++)
			{
				if(NULL != m_pUserInfo[m])
				{
					SendGameData(m,&TFishTraceRegSmall,sizeof(TFishTraceRegSmall),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
				}
			}

			tempcount++;
			if(tempcount>fishcount)break;
		}
	}
	else if(isrentur==2)
	{
		for(int i=0;i<250;i++)
		{
			if(m_fishtrace[i][0].m_isHave==true)continue;

			memset(&TFishTraceRegSmall, 0,sizeof(TFishTraceRegSmall));
			if(fangxiang==0)
			{

				m_fishtrace[i][0].x = 1480 +(102* tempcount);
				m_fishtrace[i][0].y = 350;
				m_fishtrace[i][0].rotation = 265;

			}

			m_fishtrace[i][0].m_fishtype = fishtye;
			m_fishtrace[i][0].movetime = 300+tempcount*100;
			m_fishtrace[i][0].m_isHave=true;
			m_fishtrace[i][0].m_ptindex = i;
			m_fishtrace[i][0].m_BuildTime =  GetTickCount();
			m_fishtrace[i][0].m_fishid =  GetNewFishID();
			m_fishtrace[i][0].m_shootCount = 0;
			m_fishtrace[i][0].m_speed = 5;
			int mustshoot = rand()%10;
			if(mustshoot<2)mustshoot=0;
			else  if(mustshoot>=3&&mustshoot<5)mustshoot=1;
			else mustshoot= 2;
			m_fishtrace[i][0].m_mustShoot = m_fishShoot[m_fishtrace[i][0].m_fishtype][mustshoot];
			m_fishtrace[i][0].m_fudaifishtype = -1;


			m_fishtrace[i][1].movetime = 225;
			m_fishtrace[i][1].m_speed = 6;
			m_fishtrace[i][1].changetime = 10;
			m_fishtrace[i][1].rotation = 280;

			m_fishtrace[i][2].movetime = 120000000;
			m_fishtrace[i][2].m_speed = 6;
			m_fishtrace[i][2].changetime = 10;
			m_fishtrace[i][2].rotation = 0;


			memcpy(&TFishTraceRegSmall.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceRegSmall.m_fishtrace));

			for(int m=0; m<PLAY_COUNT; m++)
			{
				if(NULL != m_pUserInfo[m])
				{
					SendGameData(m,&TFishTraceRegSmall,sizeof(TFishTraceRegSmall),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
				}
			}

			tempcount++;
			if(tempcount>fishcount)break;
		}
	}
	else if(isrentur==3)
	{
		for(int i=0;i<250;i++)
		{
			if(m_fishtrace[i][0].m_isHave==true)continue;

			memset(&TFishTraceRegSmall, 0,sizeof(TFishTraceRegSmall));
			if(fangxiang==0)
			{

				m_fishtrace[i][0].x = 400 ;
				m_fishtrace[i][0].y = 940 +(102* tempcount);
				m_fishtrace[i][0].rotation = 0;

			}

			m_fishtrace[i][0].m_fishtype = fishtye;
			m_fishtrace[i][0].movetime = 150+tempcount*100;
			m_fishtrace[i][0].m_isHave=true;
			m_fishtrace[i][0].m_ptindex = i;
			m_fishtrace[i][0].m_BuildTime =  GetTickCount();
			m_fishtrace[i][0].m_fishid =  GetNewFishID();
			m_fishtrace[i][0].m_shootCount = 0;
			m_fishtrace[i][0].m_speed = 5;
			int mustshoot = rand()%10;
			if(mustshoot<2)mustshoot=0;
			else  if(mustshoot>=3&&mustshoot<5)mustshoot=1;
			else mustshoot= 2;
			m_fishtrace[i][0].m_mustShoot = m_fishShoot[m_fishtrace[i][0].m_fishtype][mustshoot];
			m_fishtrace[i][0].m_fudaifishtype = -1;


			m_fishtrace[i][1].movetime = 205;
			m_fishtrace[i][1].m_speed = 6;
			m_fishtrace[i][1].changetime = 10;
			m_fishtrace[i][1].rotation = 45;

			m_fishtrace[i][2].movetime = 120000000;
			m_fishtrace[i][2].m_speed = 6;
			m_fishtrace[i][2].changetime = 10;
			m_fishtrace[i][2].rotation = 35;


			memcpy(&TFishTraceRegSmall.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceRegSmall.m_fishtrace));

			for(int m=0; m<PLAY_COUNT; m++)
			{
				if(NULL != m_pUserInfo[m])
				{
					SendGameData(m,&TFishTraceRegSmall,sizeof(TFishTraceRegSmall),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
				}
			}

			tempcount++;
			if(tempcount>fishcount)break;
		}
	}
}
/*---------------------------------------------------------------------------*/
//����С��
void	CServerGameDesk::CreatSmalFish()
{
	CMD_S_FishTrace TFishTraceSmall;
	srand((unsigned)time(NULL));
	int tempcount = 0;
	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_isHave==true)continue;

		memset(&TFishTraceSmall,0,sizeof(TFishTraceSmall));
		int Roation = rand()%4;
		float PointX;
		float PointY;
		int Jiaodu1;
		int zhengorfu = 0;
		//�ж���λ,���������ʼ����
		switch(Roation)
		{
		case 0:	//�����0Ϊ��
			{
				PointX = float(rand()%CLIENT_WIDTH);
				PointY = float(rand()%200+CLIENT_HEIGHT+200);
				Jiaodu1 = rand()%45;

				if(PointX>CLIENT_WIDTH/2){Jiaodu1=360-Jiaodu1;zhengorfu=1;}
				else {zhengorfu = 0;}
				break;
			}
		case 1:	//�����1Ϊ��
			{
				PointX = float(CLIENT_WIDTH + rand()%200+200);
				PointY = float(rand()%CLIENT_HEIGHT);
				Jiaodu1 = rand()%45;
				if(PointY>CLIENT_HEIGHT/2){Jiaodu1=270-Jiaodu1;zhengorfu=1;}
				else {Jiaodu1=270+Jiaodu1;zhengorfu=0;}
				break;
			}
		case 2:	//�����2Ϊ��
			{
				PointX = float(rand()%CLIENT_WIDTH);
				PointY = -200-rand()%200;
				Jiaodu1 = rand()%45;
				if(PointX>CLIENT_WIDTH/2){Jiaodu1=180+Jiaodu1;zhengorfu = 0;}
				else {Jiaodu1=180-Jiaodu1;zhengorfu=1;}
				break;
			}
		case 3:	//�����3Ϊ��
			{
				PointX = -200 - rand()%200;
				PointY = rand()%CLIENT_HEIGHT;
				Jiaodu1 = rand()%20+60;
				if(PointY>CLIENT_HEIGHT/2){Jiaodu1=180-Jiaodu1;zhengorfu = 1;}
				else {zhengorfu = 0;}
				break;
			}

		}


		//���ֵ�����
		int FishType = rand()% 2;
		int temptime= rand()%300+350;
		m_fishtrace[i][0].x = PointX;
		m_fishtrace[i][0].y = PointY;
		m_fishtrace[i][0].movetime = temptime;
		m_fishtrace[i][0].rotation = Jiaodu1;
		m_fishtrace[i][0].m_fishtype = FishType;
		m_fishtrace[i][0].m_isHave=true;
		m_fishtrace[i][0].m_ptindex = i;
		m_fishtrace[i][0].m_BuildTime =  GetTickCount();
		m_fishtrace[i][0].m_fishid =  GetNewFishID();
		m_fishtrace[i][0].m_shootCount = 0;
		//������������ڵ���
		int mustshoot = rand()%10;
		if(mustshoot<3)mustshoot=0;
		else  if(mustshoot>=3&&mustshoot<6)mustshoot=1;
		else mustshoot= 2;
		m_fishtrace[i][0].m_mustShoot = int(m_fishShoot[FishType][mustshoot]);
		int smallfishtype = rand()%12+3;
		if(smallfishtype==11)smallfishtype= 14;
		m_fishtrace[i][0].m_fudaifishtype = -1;
		m_fishtrace[i][0].m_speed = rand()%6+5;

		//��һ��ƫ�ƵĽǶ�
		int ct1 = rand()%6+1;
		int ax1 = rand()%2+3;
		int Roation1 = rand()%80; 
		int temptime1= rand()%100+440;
		if(zhengorfu==0)Roation1 = -Roation1;
		m_fishtrace[i][1].movetime = temptime1;
		m_fishtrace[i][1].rotation = (Jiaodu1+60)%360;
		m_fishtrace[i][1].changetime = ct1;
		m_fishtrace[i][1].m_speed = rand()%6+5;

		//�ڶ���ƫ�ƵĽǶ�
		int ct2 = rand()%4+2;
		int ax2 = rand()%2+5;
		int Roation2 = rand()%80; 
		int temptime2= rand()%100+350;
		if(zhengorfu==1)Roation2 = -Roation2;
		m_fishtrace[i][2].movetime = 60;
		m_fishtrace[i][2].rotation = (Jiaodu1-140)%360;
		m_fishtrace[i][2].changetime = ct2;
		m_fishtrace[i][2].m_speed = rand()%6+5;

		//������ƫ�ƵĽǶ�
		m_fishtrace[i][3].movetime = 250;
		m_fishtrace[i][3].rotation =  (Jiaodu1+40)%360;
		m_fishtrace[i][3].changetime = 5;
		m_fishtrace[i][3].m_speed = rand()%6+5;

		//���һ��ƫ����
		m_fishtrace[i][4].movetime = 1000000000;
		m_fishtrace[i][4].rotation =  (Jiaodu1-70)%360;
		m_fishtrace[i][4].changetime = 4;
		m_fishtrace[i][4].m_speed = rand()%6+5;


		memcpy(&TFishTraceSmall.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceSmall.m_fishtrace));

		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTraceSmall,sizeof(TFishTraceSmall),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
			}
		}

		tempcount++;
		if(tempcount>6)break;
	}


}
/*---------------------------------------------------------------------------*/
//�������1
void	CServerGameDesk::CreateFishTraceOne()
{
	CMD_S_FishTrace TFishTraceOne;
	memset(&TFishTraceOne, 0, sizeof(TFishTraceOne));
	srand((unsigned)time(NULL));

	int tempcount = 0;

	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_isHave==true)
		{
			continue;
		}
		memset(&TFishTraceOne, 0, sizeof(TFishTraceOne));
		int Roation = rand()%2;
		float PointX;
		float PointY;
		int Jiaodu1;
		int zhengorfu = 0;
		int smallfishtype = rand()%12+3;
		if(smallfishtype==11)smallfishtype= 14;
		//�ж���λ,���������ʼ����
		switch(Roation)
		{
		case 1:	//�����1Ϊ��
			{
				PointX = CLIENT_WIDTH + 200+rand()%200;
				PointY = CLIENT_HEIGHT/2-100+tempcount*300;
				Jiaodu1 =270;
				break;
			}
		case 0:	//�����3Ϊ��
			{
				PointX = -200 - rand()%200;;
				PointY = CLIENT_HEIGHT/2-100+tempcount*300;
				Jiaodu1 = 90;
				break;
			}
		}					  
		//���ֵ�����
		int FishType ; 
		int a = rand()% 10;
		if(a<=3)
		{
			FishType = rand()% 23;
		}
		else if(a>=5&&a<8)
		{
			FishType = rand()% 15;
		}
		else
		{
			FishType = rand()% 15;
		}

		int temptime= rand()%200+250;
		m_fishtrace[i][0].x = PointX;
		m_fishtrace[i][0].y = PointY;
		m_fishtrace[i][0].movetime = temptime;
		m_fishtrace[i][0].rotation = Jiaodu1;
		m_fishtrace[i][0].m_fishtype = FishType;
		m_fishtrace[i][0].m_isHave=true;
		m_fishtrace[i][0].m_ptindex = i;
		m_fishtrace[i][0].m_BuildTime =  GetTickCount();
		m_fishtrace[i][0].m_fishid =  GetNewFishID();
		m_fishtrace[i][0].m_shootCount = 0;
		m_fishtrace[i][0].m_speed = rand()%3+5;
		//������������ڵ���
		int mustshoot = rand()%10;
		if(mustshoot<3)mustshoot=0;
		else  if(mustshoot>=3&&mustshoot<6)mustshoot=1;
		else mustshoot= 2;
		m_fishtrace[i][0].m_mustShoot = int(m_fishShoot[FishType][mustshoot]);
		m_fishtrace[i][0].m_fudaifishtype = -1;

		//��һ��ƫ�ƵĽǶ�
		m_fishtrace[i][1].movetime = rand()%550+200;
		m_fishtrace[i][1].rotation = (Jiaodu1+rand()%30)%360;
		m_fishtrace[i][1].changetime = 5;
		m_fishtrace[i][1].m_speed = rand()%4+4;

		//�ڶ���ƫ�ƵĽǶ�
		m_fishtrace[i][2].movetime = rand()%350+300;
		m_fishtrace[i][2].rotation = (Jiaodu1-rand()%40)%360;
		m_fishtrace[i][2].changetime = 5;
		m_fishtrace[i][2].m_speed = rand()%3+5;

		//������ƫ�ƵĽǶ�
		m_fishtrace[i][3].movetime = rand()%350+200;
		m_fishtrace[i][3].rotation = (Jiaodu1+rand()%30)%360;
		m_fishtrace[i][3].changetime = 5;
		m_fishtrace[i][3].m_speed = rand()%3+5;

		//���һ��ƫ����
		m_fishtrace[i][4].movetime = 1000000000;
		m_fishtrace[i][4].rotation =  m_fishtrace[i][2].rotation;
		m_fishtrace[i][4].changetime = 4;
		m_fishtrace[i][4].m_speed = rand()%6+5;

		//CopyMemory(pFishTrace->m_fishtrace,&m_fishtrace[i],sizeof(FishTrace)*5);
		memcpy(&TFishTraceOne.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceOne.m_fishtrace));
		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTraceOne,sizeof(TFishTraceOne),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
			}
		}

		tempcount++;
		if(tempcount>=5)
		{
			break;
		}
	}
}
/*---------------------------------------------------------------------------*/
//�������2
void	CServerGameDesk::CreateFishTraceTwo()
{
	//����С��Ⱥ
	int Roation = rand()%4;
	int m_speed = rand()%4+5;
	int m_speed1 = rand()%6+5;
	int m_speed2 = rand()%4+5;
	int m_speed3 = rand()%5+5;
	float PointX;
	float PointY;
	int Jiaodu1;
	int tempcount = 0;
	//�ж���λ,���������ʼ����
	if(Roation==0)
	{
		PointX = -200;
		PointY = 0;
		Jiaodu1 = 150;

	}
	else if(Roation==1)
	{
		PointX = 1280+200;
		PointY = 730;
		Jiaodu1 = 315;

	}
	else if(Roation==2)
	{
		PointX = -50;
		PointY = 720;
		Jiaodu1 = 25;
	}
	else 
	{
		PointX = 1450;
		PointY = 100;
		Jiaodu1 = 260;
	}

	int xj = rand()%40+40;
	int yj = rand()%30+50;
	int zhengfu = rand()%2;
	if(tempcount==0||tempcount==1)
	{
		xj =  -xj;
		yj =  -yj;
	}



	CMD_S_FishTrace TFishTraceTwo;
	memset(&TFishTraceTwo, 0, sizeof(TFishTraceTwo));

	srand((unsigned)time(NULL));
	int Jiaodu1x2 = rand()%20+60;;
	int ct1 = rand()%4+3;
	int ax1 = rand()%2+3;
	int Roation1 = rand()%80; 
	int temptime1= rand()%250+230;
	if(ax1==0)Roation1 = -Roation1;

	if(Roation==0)Jiaodu1x2=180;
	if(Roation==1)Jiaodu1x2=270;
	if(Roation==2)Jiaodu1x2=0;
	if(Roation==3)Jiaodu1x2=90;
	int at = rand()%2;
	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_isHave==true)
		{
			continue;
		}

		memset(&TFishTraceTwo, 0, sizeof(TFishTraceTwo));

		int ybbei =  rand()%3;
		int tempa = 20;
		if(ybbei%2==0)tempa=-20;


		if(Roation==10||Roation==12)
		{ m_fishtrace[i][0].x = PointX + tempa*ybbei;}
		else
		{ m_fishtrace[i][0].x = PointX - tempcount*30 ;}//- tempcount*30;}

		if(Roation==1||Roation==3||Roation==0||Roation==2)
		{  m_fishtrace[i][0].y = PointY + tempa*ybbei;}
		else
		{  m_fishtrace[i][0].y = PointY  - tempcount*30 ;}


		m_fishtrace[i][0].movetime = temptime1;
		m_fishtrace[i][0].rotation = Jiaodu1;
		m_fishtrace[i][0].m_fishtype = ct1%2;
		m_fishtrace[i][0].m_isHave=true;
		m_fishtrace[i][0].m_ptindex = i;
		m_fishtrace[i][0].m_BuildTime =  GetTickCount();
		m_fishtrace[i][0].m_fishid =  GetNewFishID();
		m_fishtrace[i][0].m_shootCount = 0;
		m_fishtrace[i][0].m_speed = m_speed;
		//������������ڵ���
		int mustshoot = rand()%10;
		if(mustshoot<3)mustshoot=0;
		else  if(mustshoot>=3&&mustshoot<6)mustshoot=1;
		else mustshoot= 2;
		m_fishtrace[i][0].m_mustShoot = m_fishShoot[ct1%2][mustshoot];
		m_fishtrace[i][0].m_fudaifishtype = -1;

		m_fishtrace[i][1].movetime = 350;
		m_fishtrace[i][1].rotation = (Jiaodu1+xj)%360;
		m_fishtrace[i][1].changetime = ct1;
		m_fishtrace[i][1].m_speed = m_speed1;

		//�ڶ���ƫ�ƵĽǶ�
		m_fishtrace[i][2].movetime = 240;
		m_fishtrace[i][2].rotation = (Jiaodu1-yj)%360;
		m_fishtrace[i][2].changetime = 4;
		m_fishtrace[i][2].m_speed = m_speed2;

		//������ƫ�ƵĽǶ�
		//������ƫ�ƵĽǶ�
		if(at==0){yj= -yj;xj=-xj;}
		m_fishtrace[i][3].movetime = 450;
		m_fishtrace[i][3].rotation =  (Jiaodu1+yj+xj)%360;
		m_fishtrace[i][3].changetime = 5;
		m_fishtrace[i][3].m_speed = m_speed3;
		if(at==0){yj= -yj;xj=-xj;}

		//���һ��ƫ����
		m_fishtrace[i][4].movetime = 1000000000;
		m_fishtrace[i][4].rotation =  m_fishtrace[i][2].rotation;
		m_fishtrace[i][4].changetime = 4;
		m_fishtrace[i][4].m_speed = m_speed;

		
		memcpy(&TFishTraceTwo.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceTwo.m_fishtrace));

		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTraceTwo,sizeof(TFishTraceTwo),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
			}
		}

		tempcount++;
		if(tempcount>6)
		{
			break;
		}
	}
}
/*---------------------------------------------------------------------------*/
//�������3
void	CServerGameDesk::CreateFishTraceThree()
{

	CMD_S_FishTrace TFishTraceThree;
	memset(&TFishTraceThree, 0, sizeof(TFishTraceThree));
	//�����ɫȺ
	int Roation = rand()%4;
	int m_speed = rand()%3+5;
	int m_speed1 = rand()%3+5;
	int m_speed2 = rand()%3+5;
	int m_speed3 = rand()%3+5;
	
	int temptime = rand()%300+200;
	int	temptime1= rand()%300+100;
	int temptime2= rand()%300+300;
	int temptime3= rand()%300+500;
	int tempReturn =  rand()%3;

	int tempcount = 0;

	for(int i=0;i<250;i++)
	{
		if(tempReturn!=0)break;

		if(m_fishtrace[i][0].m_isHave==true)continue;

		memset(&TFishTraceThree, 0, sizeof(TFishTraceThree));
		int Roation = rand()%4;
		float PointX;
		float PointY;
		int Jiaodu1;
		int zhengorfu = 0;
		//�ж���λ,���������ʼ����
		switch(Roation)
		{
		case 0:	//�����0Ϊ��
			{
				PointX = float(rand()%CLIENT_WIDTH);
				PointY = -200;
				Jiaodu1 = rand()%45;
				if(PointX>CLIENT_WIDTH/2){Jiaodu1=180+Jiaodu1;zhengorfu = 0;}
				else {Jiaodu1=180-Jiaodu1;zhengorfu=1;}
				break;
			}
		case 1:	//�����1Ϊ��
			{
				PointX = float(CLIENT_WIDTH + rand()%200+200);
				PointY = float(rand()%CLIENT_HEIGHT);
				Jiaodu1 = rand()%45;
				if(PointY>CLIENT_HEIGHT/2){Jiaodu1=270-Jiaodu1;zhengorfu=1;}
				else {Jiaodu1=270+Jiaodu1;zhengorfu=0;}
				break;
			}
		case 2:	//�����2Ϊ��
			{
				PointX = float(rand()%CLIENT_WIDTH);
				PointY = 1000;
				Jiaodu1 = rand()%45;
				if(PointX>CLIENT_WIDTH/2){Jiaodu1=360-Jiaodu1;zhengorfu=1;}
				else {zhengorfu = 0;}
				break;
			}
		case 3:	//�����3Ϊ��
			{
				PointX = -200 - rand()%200;
				PointY = rand()%CLIENT_HEIGHT;
				Jiaodu1 = rand()%20+60;
				if(PointY>CLIENT_HEIGHT/2){Jiaodu1=180-Jiaodu1;zhengorfu = 1;}
				else {zhengorfu = 0;}
				break;
			}
		}

		//���ֵ�����
		m_fishtrace[i][0].x = PointX;
		m_fishtrace[i][0].y = PointY;
		m_fishtrace[i][0].movetime = temptime;
		m_fishtrace[i][0].rotation = Jiaodu1;
		int ftyep = (rand()+tempcount)%20+2;
		m_fishtrace[i][0].m_fishtype = ftyep;
		m_fishtrace[i][0].m_isHave=true;
		m_fishtrace[i][0].m_ptindex = i;
		m_fishtrace[i][0].m_BuildTime =  GetTickCount();
		m_fishtrace[i][0].m_fishid =  GetNewFishID();
		m_fishtrace[i][0].m_shootCount = 0;
		m_fishtrace[i][0].m_speed = m_speed;
		//������������ڵ���
		int mustshoot = rand()%10;
		if(mustshoot<2)mustshoot=0;
		else  if(mustshoot>=3&&mustshoot<6)mustshoot=1;
		else mustshoot= 2;
		m_fishtrace[i][0].m_mustShoot = m_fishShoot[ftyep][mustshoot]*5;
		m_fishtrace[i][0].m_fudaifishtype = rand()%2;

		//��һ��ƫ�ƵĽǶ�
		m_fishtrace[i][1].movetime = temptime;
		m_fishtrace[i][1].rotation = (Jiaodu1+45)%360;
		m_fishtrace[i][1].changetime = 0;
		m_fishtrace[i][1].m_speed = m_speed1;


		//�ڶ���ƫ�ƵĽǶ�
		m_fishtrace[i][2].movetime = temptime2;
		m_fishtrace[i][2].rotation = (Jiaodu1+20)%360;
		m_fishtrace[i][2].changetime = 0;
		m_fishtrace[i][2].m_speed = m_speed2;

		//������ƫ�ƵĽǶ�
		m_fishtrace[i][3].movetime = temptime3;
		m_fishtrace[i][3].rotation =  (Jiaodu1-20)%360;
		m_fishtrace[i][3].changetime = 5;
		m_fishtrace[i][3].m_speed = m_speed3;

		//���һ��ƫ����
		m_fishtrace[i][4].movetime = 1000000000;
		m_fishtrace[i][4].rotation =  m_fishtrace[i][1].rotation;
		m_fishtrace[i][4].changetime = 4;
		m_fishtrace[i][4].m_speed = m_speed;

		memcpy(&TFishTraceThree.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceThree.m_fishtrace));

		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTraceThree,sizeof(TFishTraceThree),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
			}
		}

		tempcount++;
		if(tempcount>2)
		{
			break;
		}
	}                   

}
/*---------------------------------------------------------------------------*/
//�������4
void	CServerGameDesk::CreateFishTraceFour()
{
	CMD_S_FishTrace TFishTraceFour;
	memset(&TFishTraceFour, 0, sizeof(TFishTraceFour));


	int Roation = rand()%4;
	int m_speed = rand()%3+5;
	int m_speed1 = rand()%3+5;
	int m_speed2 = rand()%3+5;
	int m_speed3 = rand()%3+5;

	int temptime = rand()%300+200;
	int	temptime1= rand()%300+100;
	int temptime2= rand()%300+300;
	int temptime3= rand()%300+500;
	int tempReturn =  rand()%3;

	int tempcount = 0;
	int Jiaodu1;
	int zhengorfu = 0;
	//�ж���λ,���������ʼ����
	int ftyep = (rand()+tempcount)%9+6;
	float PointX;
	float PointY;
	switch(Roation)
	{
		//�����0Ϊ��
	case 0:
		{
			PointX = float(rand()%CLIENT_WIDTH);
			PointY = -200;
			Jiaodu1 = rand()%45;
			if(PointX>CLIENT_WIDTH/2){Jiaodu1=180+Jiaodu1;zhengorfu = 0;}
			else {Jiaodu1=180-Jiaodu1;zhengorfu=1;}
			break;
		}
		//�����1Ϊ��
	case 1:
		{
			PointX = float(CLIENT_WIDTH + rand()%200+200);
			PointY = float(rand()%CLIENT_HEIGHT);
			Jiaodu1 = rand()%45;
			if(PointY>CLIENT_HEIGHT/2){Jiaodu1=270-Jiaodu1;zhengorfu=1;}
			else {Jiaodu1=270+Jiaodu1;zhengorfu=0;}
			break;
		}
		//�����2Ϊ��
	case 2:
		{
			PointX = float(rand()%CLIENT_WIDTH);
			PointY = 1000;
			Jiaodu1 = rand()%45;
			if(PointX>CLIENT_WIDTH/2){Jiaodu1=360-Jiaodu1;zhengorfu=1;}
			else {zhengorfu = 0;}
			break;
		}
		//�����3Ϊ��
	case 3:
		{
			PointX = -200 - rand()%200;
			PointY = rand()%CLIENT_HEIGHT;
			Jiaodu1 = rand()%20+60;
			if(PointY>CLIENT_HEIGHT/2){Jiaodu1=180-Jiaodu1;zhengorfu = 1;}
			else {zhengorfu = 0;}
			break;
		}

	}
	for(int i=0;i<250;i++)
	{
		if(tempReturn==0)break;
		if(m_fishtrace[i][0].m_isHave==true)continue;

		memset(&TFishTraceFour, 0, sizeof(TFishTraceFour));
		int tempa = 180;
		if(Roation==0||Roation==2)
		{ m_fishtrace[i][0].x = PointX + tempa*tempcount;}
		else
		{ m_fishtrace[i][0].x = PointX ;}//- tempcount*30;}

		if(Roation==1||Roation==3)
		{  m_fishtrace[i][0].y = PointY + tempa*tempcount;}
		else
		{  m_fishtrace[i][0].y = PointY ;}
		//���ֵ�����

		m_fishtrace[i][0].movetime = temptime;
		m_fishtrace[i][0].rotation = Jiaodu1;
		//int ftyep = rand()%11+2;
		m_fishtrace[i][0].m_fishtype = ftyep;
		m_fishtrace[i][0].m_isHave=true;
		m_fishtrace[i][0].m_ptindex = i;
		m_fishtrace[i][0].m_BuildTime =  GetTickCount();
		m_fishtrace[i][0].m_fishid =  GetNewFishID();
		m_fishtrace[i][0].m_shootCount = 0;
		m_fishtrace[i][0].m_speed = m_speed;
		//������������ڵ���
		int mustshoot = rand()%10;
		if(mustshoot<2)mustshoot=0;
		else  if(mustshoot>=3&&mustshoot<6)mustshoot=1;
		else mustshoot= 2;
		m_fishtrace[i][0].m_mustShoot = m_fishShoot[ftyep][mustshoot]*4;
		m_fishtrace[i][0].m_fudaifishtype = 0;

		//��һ��ƫ�ƵĽǶ�
		m_fishtrace[i][1].movetime = temptime;
		m_fishtrace[i][1].rotation = (Jiaodu1+45)%360;
		m_fishtrace[i][1].changetime = 0;
		m_fishtrace[i][1].m_ptindex = tempcount+10;
		m_fishtrace[i][1].m_speed = m_speed1;

		//�ڶ���ƫ�ƵĽǶ�
		m_fishtrace[i][2].movetime = temptime2;
		m_fishtrace[i][2].rotation = (Jiaodu1+20)%360;
		m_fishtrace[i][2].changetime = 0;
		m_fishtrace[i][2].m_speed = m_speed2;

		//������ƫ�ƵĽǶ�
		m_fishtrace[i][3].movetime = temptime3;
		m_fishtrace[i][3].rotation =  (Jiaodu1-20)%360;
		m_fishtrace[i][3].changetime = 5;
		m_fishtrace[i][3].m_speed = m_speed3;

		//���һ��ƫ����
		m_fishtrace[i][4].movetime = 1000000000;
		m_fishtrace[i][4].rotation =  m_fishtrace[i][1].rotation;
		m_fishtrace[i][4].changetime = 4;
		m_fishtrace[i][4].m_speed = m_speed;


		memcpy(&TFishTraceFour.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceFour.m_fishtrace));
		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTraceFour,sizeof(TFishTraceFour),MDM_GM_GAME_NOTIFY,ASS_BUILD_TRACE,0);
			}
		}

		tempcount++;
		if(tempcount>4)break;
	}                   
}
/*---------------------------------------------------------------------------*/
//�������ID��ʶ
DWORD	CServerGameDesk::GetNewFishID()
{
	//������ʶ
	m_dwFishID+=1;
	if (0L==m_dwFishID) 
	{
		m_dwFishID=1;
	}
	return m_dwFishID;
}
/*---------------------------------------------------------------------------*/
//������
void	CServerGameDesk::ClearFishTrace()
{
	//ö�ٱ���
	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_isHave==false)continue;
		DWORD dwNowTime=GetTickCount();
		if (dwNowTime>=(m_fishtrace[i][0].m_BuildTime+FISH_ALIVE_TIME))
		{
			m_fishtrace[i][0].m_isHave=false;
		}
	}
}
/*---------------------------------------------------------------------------*/
//�л�����
void	CServerGameDesk::ChangeScence()
{
	//�ر�ʱ��(��ֹ���л������ͷ����µ���Ⱥ)
	KillTimer(TIME_BUILD_TRACE);
	KillTimer(TIME_CREAD_FISH);
	//�ı䳡��
	m_iBgindex++;
	if(m_iBgindex>=3)
	{
		m_iBgindex=0;
	}
	//���������
	m_iRegfishcount = 0;

	//��������
	CMD_S_ChangeScene TChangeScene;
	//���ñ���
	TChangeScene.SceneIndex	= m_iBgindex;

	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(NULL != m_pUserInfo[i])
		{
			SendGameData(i,&TChangeScene,sizeof(TChangeScene),MDM_GM_GAME_NOTIFY,ASS_CHANGE_SCENCE,0);
		}
	}
	//�������������ʱ��
	SetTimer(TIME_REGULAR_FISH,10*1000);

}
/*---------------------------------------------------------------------------*/
//��������һ
void	CServerGameDesk::RegFishone()
{
	CreatSmalFish();
	CMD_S_FishTrace  TFishTraceRegOne;
	int tempcount = 0;
	int fishtyeparr[11]={13,14,15,16,18,19,17,23,23,22,22};
	int xbei = 0;
	int outcount =1;
	if(m_iRegfishcount>6)
	{
		outcount=0;
	}

	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_isHave==true)continue;

		memset(&TFishTraceRegOne, 0, sizeof(TFishTraceRegOne));

		m_fishtrace[i][0].x = -200;
		if(tempcount%2==0)
		{ 
			m_fishtrace[i][0].y = 450;
		}
		else
		{  
			m_fishtrace[i][0].y = 290;
		}
		if(m_iRegfishcount>6)
		{
			m_fishtrace[i][0].y = 400;
			outcount=0;
		}
		m_fishtrace[i][0].movetime = 50000000;
		m_fishtrace[i][0].rotation = 90;
		m_fishtrace[i][0].m_fishtype = fishtyeparr[m_iRegfishcount];
		m_fishtrace[i][0].m_isHave=true;
		m_fishtrace[i][0].m_ptindex = i;
		m_fishtrace[i][0].m_BuildTime =  GetTickCount();
		m_fishtrace[i][0].m_fishid =  GetNewFishID();
		m_fishtrace[i][0].m_shootCount = 0;
		m_fishtrace[i][0].m_speed = 6;
		m_fishtrace[i][0].m_fudaifishtype = -1;
		int mustshoot = rand()%10;
		if(mustshoot<2)
		{
			mustshoot=0;
		}
		else  if(mustshoot>=3&&mustshoot<5)
		{
			mustshoot=1;
		}
		else 
		{
			mustshoot= 2;
		}

		m_fishtrace[i][0].m_mustShoot = m_fishShoot[m_fishtrace[i][0].m_fishtype][mustshoot]*3;

		memcpy(&TFishTraceRegOne.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceRegOne.m_fishtrace));
		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTraceRegOne,sizeof(TFishTraceRegOne),MDM_GM_GAME_NOTIFY,ASS_FISH_REG,0);
			}
		}

		tempcount++;
		if(tempcount>outcount)break;
	}

	SetTimer(TIME_REGULAR_FISH,5000);
	if(m_iRegfishcount>9)
	{
		KillTimer(TIME_REGULAR_FISH);
		//�������������ʱ��
		SetTimer(TIME_BUILD_TRACE,20000);
		SetTimer(TIME_CREAD_FISH,20000);
	}
	m_iRegfishcount++;
}
/*---------------------------------------------------------------------------*/
//�������Ͷ�
void	CServerGameDesk::RegFishtwo()
{
	CreatSmalFish();
	//��2�ֶԲ���ʽ������
	CMD_S_FishTrace TFishTraceRegTwo;

	int tempcount = 0;
	//����15��-19��֮�����
	int fishtype =  rand()%5+15;
	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_isHave==true)continue;

		memset(&TFishTraceRegTwo, 0, sizeof(TFishTraceRegTwo));
		if(tempcount==0)
		{
			m_fishtrace[i][0].x = -200;
			m_fishtrace[i][0].y = 940;
			m_fishtrace[i][0].rotation = 45;
			m_fishtrace[i][0].m_fishtype = fishtype;
		}
		else if(tempcount==1)
		{
			m_fishtrace[i][0].x = 1280+200;
			m_fishtrace[i][0].y = -200;
			m_fishtrace[i][0].rotation = 225;
			m_fishtrace[i][0].m_fishtype = fishtype;

		}
		else if(tempcount==2)
		{
			m_fishtrace[i][0].x = -200;
			m_fishtrace[i][0].y = -200;
			m_fishtrace[i][0].rotation = 135;
			m_fishtrace[i][0].m_fishtype = fishtype;
		}
		else 
		{
			m_fishtrace[i][0].x = 1280+200;
			m_fishtrace[i][0].y = 940;
			m_fishtrace[i][0].rotation = 315;
			m_fishtrace[i][0].m_fishtype = fishtype;
		}

		m_fishtrace[i][0].movetime = 5000000000;				   
		m_fishtrace[i][0].m_isHave=true;
		m_fishtrace[i][0].m_ptindex = i;
		m_fishtrace[i][0].m_BuildTime =  GetTickCount();
		m_fishtrace[i][0].m_fishid =  GetNewFishID();
		m_fishtrace[i][0].m_shootCount = 0;
		m_fishtrace[i][0].m_speed = 6;
		m_fishtrace[i][0].m_fudaifishtype = -1;
		int mustshoot = rand()%10;
		if(mustshoot<2)mustshoot=0;
		else  if(mustshoot>=3&&mustshoot<5)mustshoot=1;
		else mustshoot= 2;
		m_fishtrace[i][0].m_mustShoot = m_fishShoot[m_fishtrace[i][0].m_fishtype][mustshoot]*2;


		memcpy(&TFishTraceRegTwo.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceRegTwo.m_fishtrace));
		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTraceRegTwo,sizeof(TFishTraceRegTwo),MDM_GM_GAME_NOTIFY,ASS_FISH_REG,0);
			}
		}

		tempcount++;
		if(tempcount>3)break;
	}


	SetTimer(TIME_REGULAR_FISH,3000);
	if(m_iRegfishcount>10)
	{
		KillTimer(TIME_REGULAR_FISH);
		//�������������ʱ��
		SetTimer(TIME_BUILD_TRACE,20000);
		SetTimer(TIME_CREAD_FISH,20000);
	}

	m_iRegfishcount++;
}
/*---------------------------------------------------------------------------*/
//����������
void	CServerGameDesk::RegFishthree()
{
	CreatSmalFish();
	//��3�ֶԲ���ʽ������
	CMD_S_FishTrace  TFishTraceRegThree;
	int tempcount = 0;
	//����15��-19��֮�����
	int fishtype =  rand()%17+2;
	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_isHave==true)continue;

		memset(&TFishTraceRegThree, 0, sizeof(TFishTraceRegThree));
		if(tempcount==0)
		{
			m_fishtrace[i][0].x = 400;
			m_fishtrace[i][0].y = 940;
			m_fishtrace[i][0].rotation = 0;
			m_fishtrace[i][0].m_fishtype = fishtype;
		}
		else if(tempcount==1)
		{
			m_fishtrace[i][0].x = 1280+200;
			m_fishtrace[i][0].y = 200;
			m_fishtrace[i][0].rotation = 270;
			m_fishtrace[i][0].m_fishtype = fishtype;

		}
		else if(tempcount==2)
		{
			m_fishtrace[i][0].x = -200;
			m_fishtrace[i][0].y = 450;
			m_fishtrace[i][0].rotation = 90;
			m_fishtrace[i][0].m_fishtype = fishtype;
		}
		else 
		{
			m_fishtrace[i][0].x = 750;
			m_fishtrace[i][0].y = -200;
			m_fishtrace[i][0].rotation = 180;
			m_fishtrace[i][0].m_fishtype = fishtype;
		}

		m_fishtrace[i][0].movetime = 5000000000;				   
		m_fishtrace[i][0].m_isHave=true;
		m_fishtrace[i][0].m_ptindex = i;
		m_fishtrace[i][0].m_BuildTime =  GetTickCount();
		m_fishtrace[i][0].m_fishid =  GetNewFishID();
		m_fishtrace[i][0].m_shootCount = 0;
		m_fishtrace[i][0].m_speed = 8;
		m_fishtrace[i][0].m_fudaifishtype = -1;
		int mustshoot = rand()%10;
		if(mustshoot<2)mustshoot=0;
		else  if(mustshoot>=3&&mustshoot<5)mustshoot=1;
		else mustshoot= 2;
		m_fishtrace[i][0].m_mustShoot = m_fishShoot[m_fishtrace[i][0].m_fishtype][mustshoot]*2;


		memcpy(&TFishTraceRegThree.m_fishtrace,&m_fishtrace[i],sizeof(TFishTraceRegThree.m_fishtrace));
		for(int m=0; m<PLAY_COUNT; m++)
		{
			if(NULL != m_pUserInfo[m])
			{
				SendGameData(m,&TFishTraceRegThree,sizeof(TFishTraceRegThree),MDM_GM_GAME_NOTIFY,ASS_FISH_REG,0);
			}
		}
	
		tempcount++;
		if(tempcount>3)break;
	}


	SetTimer(TIME_REGULAR_FISH,1500);
	if(m_iRegfishcount>20)
	{
		KillTimer(TIME_REGULAR_FISH);
		//�������������ʱ��
		SetTimer(TIME_BUILD_TRACE,20000);
		SetTimer(TIME_CREAD_FISH,20000);
	}
	m_iRegfishcount++;
}
/*---------------------------------------------------------------------------*/
//ȡ����ҵĳ�����
void	CServerGameDesk::CanCelUserSuperPao(BYTE	byStation)
{
	if(byStation < 0 || byStation >= PLAY_COUNT)
	{
		return;
	}

	m_bUserIsSuperPao[byStation] = false;

	CMD_S_CanCelSuperPao TCanCelSuperPao;
	TCanCelSuperPao.bCanSuperPao = m_bUserIsSuperPao[byStation];
	TCanCelSuperPao.byStation	 = byStation;

	for(int m=0; m<PLAY_COUNT; m++)
	{
		if(NULL != m_pUserInfo[m])
		{
			SendGameData(m,&TCanCelSuperPao,sizeof(TCanCelSuperPao),MDM_GM_GAME_NOTIFY,ASS_S_CANCEL_SUPER,0);
		}
	}	
}
/*---------------------------------------------------------------------------*/



bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{
			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}
//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if(bWatchUser)
	{
		return true;
	}

	if(bDeskStation < 0 || bDeskStation >=PLAY_COUNT)
	{
		return true;
	}

	switch (pNetHead->bAssistantID)
	{
	case ASS_C_BUY_BULLET:		//���·���Ϣ
		{
			//������·�
			OnHandleUserAddOrMoveBullet(bDeskStation, pData, uSize);
			return true;
		}
	case ASS_C_USER_SHOT:		//��ҷ����ӵ�
		{
			OnHandleUserShot(bDeskStation, pData, uSize);
			return true;
		}
	case ASS_C_ADD_FIRE:		//��Ҽ�/����
		{
			OnHandleUserAddFire(bDeskStation, pData, uSize);
			return true;
		}
	case ASS_C_HIT_FISH:		//�������
		{
			OnHandleHitFish(bDeskStation, pData, uSize);
			return true;
		}
	default:
		{
			return true;
		}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}
/*-----------------------------------------------------------------------------------------------*/
//������·�
void	CServerGameDesk::OnHandleUserAddOrMoveBullet(BYTE byStation, void * pData, UINT uSize)
{
	if(uSize != sizeof(CMD_C_BuyBullet))
	{
		return ;
	}
	CMD_C_BuyBullet *pBuyBullet = (CMD_C_BuyBullet *)pData;
	if(NULL == pBuyBullet)
	{
		return ;
	}

	//���ΪTRUE��ʾ�Ϸ�
	if(pBuyBullet->bAddOrMove)
	{
		if(pBuyBullet->iScore<0||pBuyBullet->iScore>m_iMaxFireBase)
		{
			return ;
		}
		int iAddscore = pBuyBullet->iScore*100;
		if(m_pUserInfo[byStation])
		{
			if(m_i64UserMoney[byStation] < pBuyBullet->iScore*100*m_iBiliDuihuan)
			{
				iAddscore = m_i64UserMoney[byStation]/m_iBiliDuihuan;
				//iAddscore = m_pUserInfo[byStation]->m_UserData.i64Money/m_iBiliDuihuan;
			}
			//����Ϸ��ۼ�
			m_i64UserAllScore[byStation] += iAddscore;
			//������ϵĽ��������
			m_i64UserMoney[byStation] -= iAddscore*m_iBiliDuihuan;
		}
		else
		{
			iAddscore = 0;
		}
	}
	else
	{
		if(0 == m_i64UserAllScore[byStation])
		{
			__int64 i64Addscore = m_i64UserMoney[byStation]/m_iBiliDuihuan;
			//����Ϸ��ۼ�
			m_i64UserAllScore[byStation] += i64Addscore;
			//������ϵĽ��������
			m_i64UserMoney[byStation] -= i64Addscore*m_iBiliDuihuan;
		}
		else
		{
			//������ϵĽ����������
			m_i64UserMoney[byStation] += m_i64UserAllScore[byStation]*m_iBiliDuihuan;
			//����Ϸ���0
			m_i64UserAllScore[byStation] = 0;
		}
		
		/*m_UserShootCount[chairID] = 0;
		for(int j=0;j<100;j++)
		{
			m_UserShoot[chairID][j].ishave = false;
			m_UserShoot[chairID][j].beilv = 0;
		}*/
	}

	CMD_S_BulletCount TBulletCount;
	TBulletCount.byDeskStation	= byStation;
	TBulletCount.bAddOrMove		= pBuyBullet->bAddOrMove;
	TBulletCount.i64UserScore	= m_i64UserAllScore[byStation];
	TBulletCount.i64UserMoney	= m_i64UserMoney[byStation];
	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		SendGameData(i,&TBulletCount,sizeof(TBulletCount),MDM_GM_GAME_NOTIFY,ASS_S_BUY_BULLET,0);
	}
	SendWatchData(m_bMaxPeople,&TBulletCount,sizeof(TBulletCount),MDM_GM_GAME_NOTIFY,ASS_S_BUY_BULLET,0);

}
/*-----------------------------------------------------------------------------------------------*/
//��ҷ����ӵ�
void	CServerGameDesk::OnHandleUserShot(BYTE byStation,void * pData, UINT uSize)
{
	if(uSize != sizeof(CMD_C_UserShoot))
	{
		return;
	}
	CMD_C_UserShoot *pUserShoot = (CMD_C_UserShoot *)pData;
	if(NULL == pUserShoot)
	{
		return;
	}

	//�ӵ�������
	if(m_i64UserAllScore[byStation] <= 0 || (m_i64UserAllScore[byStation] - m_iUserFireBase[byStation] < 0))
	{
		CMD_S_UserShootError TUserShootError;
		TUserShootError.byStation	= byStation;
		TUserShootError.iFireBase	= m_iUserFireBase[byStation];
		TUserShootError.i64UserScore= m_i64UserAllScore[byStation];	
		TUserShootError.bIsRobot = false;
		if(m_pUserInfo[byStation])
		{
			if(m_pUserInfo[byStation]->m_UserData.isVirtual > 0)
			{
				TUserShootError.bIsRobot = false;
			}
		}

		for (int i = 0; i < PLAY_COUNT; i ++) 
		{
			if(m_pUserInfo[i])
			{
				SendGameData(i,&TUserShootError,sizeof(TUserShootError),MDM_GM_GAME_NOTIFY,ASS_S_USER_SHOT_ERROR,0);
			}
		}

		return;
	}


	//�Ƿ�ȫ�����ǻ����� ���ȫ�����ǻ����� �ͷ��ͼٵĹ㲥��Ϣ
	if(IsAllRobot() && m_pUserInfo[byStation]->m_UserData.isVirtual > 0)
	{
		//���ͼٵĹ㲥��Ϣ
		if(2 >= rand()%10 )
		{
			OutputDebugString("dwjdntg::���ͼٹ㲥��Ϣ");
			CMD_S_CaptureFish TCaptureFish;

			int iFishType = rand()%24;
			TCaptureFish.byStation	=	byStation;
			TCaptureFish.lFishScore	=	m_iUserFireBase[byStation]*m_iFishBase[iFishType];

			//������㷢�͹㲥
			SendCatchBroadCast(iFishType,TCaptureFish);
		}
	}
	else
	{
		//�ӵ�����ȥ�ڻ���
		m_i64UserAllScore[byStation] -= m_iUserFireBase[byStation];

		//���÷��ͼ��(��ֹ�������)
		if(GetTickCount()- m_SendTime[byStation]<350)
		{
			//m_iUserShootCount[byStation] = 0;
			m_SendTime[byStation] = GetTickCount();
			for(int j=0;j<100;j++)
			{
				m_UserShoot[byStation][j].bHave = false;
			}
			return ;
		}
		else
		{
			//�����ӵ������ۼ�
			m_iUserShootCount[byStation]++;
			//�洢��ҷ�����ӵ���Ϣ
			for(int j=0;j<100;j++)
			{
				if(m_UserShoot[byStation][j].bHave == false)
				{
					m_UserShoot[byStation][j].bHave  = true;
					m_UserShoot[byStation][j].iBeilv = m_iUserFireBase[byStation];
					//�Ƿ񳬼��ڵ�
					m_UserShoot[byStation][j].bIsSuper = m_bUserIsSuperPao[byStation];
					break;
				}
			}
		}
		//��¼��ҷ����ӵ�ʱ��
		m_SendTime[byStation] = GetTickCount();

	}
	


	CMD_S_UserShoot TUserShoot;
	TUserShoot.byStation	= byStation;
	TUserShoot.fAngle		= pUserShoot->fAngle;
	TUserShoot.byShootCount	= 0;
	TUserShoot.iBulletCountKind=pUserShoot->iBulletCountKind;
	TUserShoot.i64UserScore = m_i64UserAllScore[byStation];	

	TUserShoot.bIsRobot = false;
	if(m_pUserInfo[byStation])
	{
		if(m_pUserInfo[byStation]->m_UserData.isVirtual > 0)
		{
			TUserShoot.bIsRobot = true;
		}
	}

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&TUserShoot,sizeof(TUserShoot),MDM_GM_GAME_NOTIFY,ASS_S_USER_SHOT,0);
		}
	}
	SendWatchData(m_bMaxPeople,&TUserShoot,sizeof(TUserShoot),MDM_GM_GAME_NOTIFY,ASS_S_USER_SHOT,0);


	//����ҷ����ӵ������ķ����ӵ����������
	//m_UerResultScore[mchairid] = m_UerResultScore[mchairid]  - m_lUserCellScore[mchairid];
	//if(!pIServerUserItem->IsAndroidUser()) //edit by guojm
	//{
	//	m_TimeToTimeIn = m_TimeToTimeIn + m_lUserCellScore[mchairid];
	//	m_lStockScore += m_lUserCellScore[mchairid]; //edit by guojm
	//}
}
/*-----------------------------------------------------------------------------------------------*/
//��ҼӼ���
void	CServerGameDesk::OnHandleUserAddFire(BYTE byStation,void * pData, UINT uSize)
{

	if(uSize != sizeof(CMD_C_AddFireBase))
	{
		return;
	}
	CMD_C_AddFireBase *pAddFireBase = (CMD_C_AddFireBase *)pData;
	if(NULL == pAddFireBase)
	{
		return;
	}
	int iBase = 0;
	//����
	if(pAddFireBase->bAdd)
	{
		if(m_iUserFireBase[byStation] >=1 && m_iUserFireBase[byStation] < 10)
		{
			iBase = 1;
		}
		else if(m_iUserFireBase[byStation] >=10 && m_iUserFireBase[byStation] < 100)
		{
			iBase = 10;
		}
		else if(m_iUserFireBase[byStation] >=100 && m_iUserFireBase[byStation] < 1000)
		{
			iBase = 100;
		}
		else if(m_iUserFireBase[byStation] >=1000 && m_iUserFireBase[byStation] < 10000)
		{
			iBase = 1000;
		}
		else if(m_iUserFireBase[byStation] >=10000 && m_iUserFireBase[byStation] < 100000)
		{
			iBase = 10000;
		}
		else if(m_iUserFireBase[byStation] >=100000 && m_iUserFireBase[byStation] < 1000000)
		{
			iBase = 100000;
		}
		else if(m_iUserFireBase[byStation] >=1000000 && m_iUserFireBase[byStation] < 10000000)
		{
			iBase = 1000000;
		}
		else if(m_iUserFireBase[byStation] >=10000000 && m_iUserFireBase[byStation] < 100000000)
		{
			iBase = 10000000;
		}
		else if(m_iUserFireBase[byStation] >=100000000 && m_iUserFireBase[byStation] < 1000000000)
		{
			iBase = 100000000;
		}
		else
		{
			iBase = m_iFireBase;
		}
		m_iUserFireBase[byStation] += iBase;
		if(m_iUserFireBase[byStation] > m_iMaxFireBase)
		{
			m_iUserFireBase[byStation] = m_iMinFire;
		}
	}
	else	//����
	{
		if(m_iUserFireBase[byStation] >=1 && m_iUserFireBase[byStation] < 10)
		{
			iBase = 1;
		}
		else if(m_iUserFireBase[byStation] >10 && m_iUserFireBase[byStation] <= 100)
		{
			iBase = 10;
		}
		else if(m_iUserFireBase[byStation] >100 && m_iUserFireBase[byStation] <= 1000)
		{
			iBase = 100;
		}
		else if(m_iUserFireBase[byStation] >1000 && m_iUserFireBase[byStation] <= 10000)
		{
			iBase = 1000;
		}
		else if(m_iUserFireBase[byStation] >10000 && m_iUserFireBase[byStation] <= 100000)
		{
			iBase = 10000;
		}
		else if(m_iUserFireBase[byStation] >100000 && m_iUserFireBase[byStation] <= 1000000)
		{
			iBase = 100000;
		}
		else if(m_iUserFireBase[byStation] >1000000 && m_iUserFireBase[byStation] <= 10000000)
		{
			iBase = 1000000;
		}
		else if(m_iUserFireBase[byStation] >10000000 && m_iUserFireBase[byStation] <= 100000000)
		{
			iBase = 10000000;
		}
		else if(m_iUserFireBase[byStation] >100000000 && m_iUserFireBase[byStation] <= 1000000000)
		{
			iBase = 100000000;
		}
		else
		{
			iBase = m_iFireBase;
		}
		m_iUserFireBase[byStation] -= iBase;
		if(m_iUserFireBase[byStation] < m_iMinFire)
		{
			m_iUserFireBase[byStation] = m_iMaxFireBase;
		}
	}

	CMD_S_AddFireBase TAddFireBase;
	TAddFireBase.byDeskStation	= byStation;
	TAddFireBase.iUserFireBase	= m_iUserFireBase[byStation];
	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&TAddFireBase,sizeof(TAddFireBase),MDM_GM_GAME_NOTIFY,ASS_S_ADD_FIRE,0);
		}
	}
	SendWatchData(m_bMaxPeople,&TAddFireBase,sizeof(TAddFireBase),MDM_GM_GAME_NOTIFY,ASS_S_ADD_FIRE,0);

}
/*-----------------------------------------------------------------------------------------------*/
//�������
void	CServerGameDesk::OnHandleHitFish(BYTE byStation,void * pData, UINT uSize)
{
	
	if(uSize != sizeof(CMD_C_HitFish))
	{
		return;
	}
	CMD_C_HitFish *pHitFish = (CMD_C_HitFish *)pData;
	if(NULL == pHitFish)
	{
		return;
	}
	

	/*if(pHitFish->bIsAndroid == true && byStation !=pHitFish->iBulletUserId)
	return OnSubHitFish1(pHitFish->iFishID, 0,pHitFish->iBulletUserId);
	else*/	//�����ڵ�
	{
		if(m_iUserShootCount[pHitFish->iBulletUserId]<=0) 
		{
	
			return ;
		}

		OnSubHitFish(pHitFish->iBulletUserId, pHitFish->iFishID, pHitFish->iBulletID);
	}		
}
/*-----------------------------------------------------------------------------------------------*/
bool	CServerGameDesk::OnSubHitFish(BYTE byStation, int iFishID, int iBulletID)
{
	if(iBulletID>=100 || iBulletID < 0) 
	{
		return true;
	}

	if(byStation < 0 || byStation >= PLAY_COUNT)
	{
		return true;
	}

    //�����������ڷ����
	//�ж��ӵ��Ƿ����
	if(m_UserShoot[byStation][iBulletID].bHave == false)
	{
	  return true;
	}
	//�Ƿ񳬼��ӵ�
	bool bIsSuperBullet = false;
	bIsSuperBullet = m_UserShoot[byStation][iBulletID].bIsSuper;
	//��������ӵ�
	m_UserShoot[byStation][iBulletID].bHave = false;

	//�ж���û��������
	bool ishavefish = false;
	int  fishindex = 0;
	for(int i=0;i<250;i++)
	{
		if(m_fishtrace[i][0].m_fishid == iFishID && m_fishtrace[i][0].m_isHave == true)
		{

			m_fishtrace[i][0].m_shootCount++;
			ishavefish = true;
			fishindex = i;
			break;
		}
	}    

	if(ishavefish==false)
	{
		return true;
	}
	//�Ƿ񳬼��û�
	if(IsSuperUser(byStation))
	{
		OutputDebugString("dwjjcby::�ǳ����û�");
	}
	else
	{
		//�ж��Ƿ�����Դ���
		if(!IsProbalityForDead(fishindex))
		{
			//���С����������˳�
			if(m_fishtrace[fishindex][0].m_shootCount < m_fishtrace[fishindex][0].m_mustShoot)
			{
				return  true;
			}
		}
	}

	//�Ƿ���������
	if(IsBlackListUser(byStation))
	{
		return true;
	}

	
	//�������û�û�з�����ӵ�ֱ���˳�
    if(m_iUserShootCount[byStation]<=0)
	{
		return true;
	}

	//����ӵ�����1
	m_iUserShootCount[byStation]--;
	srand((unsigned)time(NULL));
	int percent = rand()%100;
	if(percent < m_iUserPercent[byStation] || (m_fishtrace[fishindex][0].m_fishtype <3|| m_fishtrace[fishindex][0].m_fishtype == 100))
	{
		CMD_S_CaptureFish TCaptureFish;

		int fishkindscord = m_iFishBase[m_fishtrace[fishindex][0].m_fishtype];
		__int64 FishScore = m_UserShoot[byStation][iBulletID].iBeilv*m_iFishBase[m_fishtrace[fishindex][0].m_fishtype];
		//����Ǳ������40-200�ķ�Χ��������
		/*if(m_fishtrace[fishindex][0].m_fishtype==17)
		{
			fishkindscord=rand()%160+40;
			FishScore= m_UserShoot[byStation][iBulletID].iBeilv*fishkindscord;
		}*/

		int iDeadCount = 0;	//��������
		int iDeadID[250] = {-1};
		int iFishScore[250];
		
		if(0 == m_fishtrace[fishindex][0].m_fudaifishtype)
		{
			FishScore=m_UserShoot[byStation][iBulletID].iBeilv*m_iFishBase[m_fishtrace[fishindex][0].m_fishtype]*3;
			fishkindscord =m_iFishBase[m_fishtrace[fishindex][0].m_fishtype]*3; 
		}
		if(1 == m_fishtrace[fishindex][0].m_fudaifishtype )
		{
			FishScore = m_UserShoot[byStation][iBulletID].iBeilv*m_iFishBase[m_fishtrace[fishindex][0].m_fishtype];
			fishkindscord = FishScore/*rand()%10+20*/; 

			//һ����
			memset(iDeadID, -1, sizeof(iDeadID));
			memset(iFishScore, 0, sizeof(iFishScore));
			
			for(int i=0;i<250;i++)
			{
				if(m_fishtrace[i][0].m_fishid == iFishID)
				{
					continue;
				}
				if(m_fishtrace[i][0].m_fishtype == m_fishtrace[fishindex][0].m_fishtype && m_fishtrace[i][0].m_isHave == true)
				{
					if(iDeadCount < 150)
					{
						iDeadID[iDeadCount] = m_fishtrace[i][0].m_fishid;
						iFishScore[iDeadCount] = m_UserShoot[byStation][iBulletID].iBeilv*m_iFishBase[m_fishtrace[i][0].m_fishtype];
						iDeadCount++;
						m_fishtrace[i][0].m_isHave = false;
					}
				}
			}  
		}


		//�����ӵ��������
		if(bIsSuperBullet == true)
		{
			FishScore=FishScore*2;
		}

		//if(!pIServerUserItem->IsAndroidUser()) //edit by guojm
		//{
		//	m_TimeToTimeOut = m_TimeToTimeOut + FishScore;
		//	m_lStockScore -= FishScore; //edit by guojm
		//}

		if(m_fishtrace[fishindex][0].m_fudaifishtype==0)
		{
			if(m_fishtrace[fishindex][1].m_ptindex == 10)
			{
				int count = 0;
				for(int i=0;i<250;i++)
				{
					if(m_fishtrace[i][0].m_fishid==iFishID+1||m_fishtrace[i][0].m_fishid==iFishID+2)
					{
						m_fishtrace[i][0].m_isHave=false;
						CMD_S_CaptureFish TCaptureFish;
						TCaptureFish.iFishID	=	m_fishtrace[i][0].m_fishid;
						TCaptureFish.byStation	=	byStation;
						TCaptureFish.lFishScore	=	0;
						TCaptureFish.i64UserScore =	m_i64UserAllScore[byStation];

						for (int i = 0; i < PLAY_COUNT; i ++) 
						{
							if(m_pUserInfo[i])
							{
								SendGameData(i,&TCaptureFish,sizeof(TCaptureFish),MDM_GM_GAME_NOTIFY,ASS_S_HIT_FISH,0);
								SendWatchData(i,&TCaptureFish,sizeof(TCaptureFish),MDM_GM_GAME_NOTIFY,ASS_S_HIT_FISH,0);
							}
						}
		
						count++;
						if(count>=2)
						{
							break;
						}
					}
				} 
			}
			else if(m_fishtrace[fishindex][1].m_ptindex == 11)
			{
				int count = 0;
				for(int i=0;i<250;i++)
				{
					if(m_fishtrace[i][0].m_fishid==iFishID-1||m_fishtrace[i][0].m_fishid==iFishID+1)
					{
						m_fishtrace[i][0].m_isHave=false;
						CMD_S_CaptureFish TCaptureFish;
						TCaptureFish.iFishID	=	m_fishtrace[i][0].m_fishid;
						TCaptureFish.byStation	=	byStation;
						TCaptureFish.lFishScore	=	0;
						TCaptureFish.i64UserScore =	m_i64UserAllScore[byStation];
						//������Ϣ
						for (int i = 0; i < PLAY_COUNT; i ++) 
						{
							if(m_pUserInfo[i])
							{
								SendGameData(i,&TCaptureFish,sizeof(TCaptureFish),MDM_GM_GAME_NOTIFY,ASS_S_HIT_FISH,0);
								SendWatchData(i,&TCaptureFish,sizeof(TCaptureFish),MDM_GM_GAME_NOTIFY,ASS_S_HIT_FISH,0);
							}
						}
						count++;
						if(count>=2)break;

					}
				} 
			}
			else if (m_fishtrace[fishindex][1].m_ptindex == 12)
			{
				int count = 0;
				for(int i=0;i<250;i++)
				{
					if(m_fishtrace[i][0].m_fishid==iFishID-1||m_fishtrace[i][0].m_fishid==iFishID-2)
					{    
						m_fishtrace[i][0].m_isHave=false;
						CMD_S_CaptureFish TCaptureFish;
						TCaptureFish.iFishID	=	m_fishtrace[i][0].m_fishid;
						TCaptureFish.byStation	=	byStation;
						TCaptureFish.lFishScore	=	0;
						TCaptureFish.i64UserScore =	m_i64UserAllScore[byStation];
						//������Ϣ
						for (int i = 0; i < PLAY_COUNT; i ++) 
						{
							if(m_pUserInfo[i])
							{
								SendGameData(i,&TCaptureFish,sizeof(TCaptureFish),MDM_GM_GAME_NOTIFY,ASS_S_HIT_FISH,0);
								SendWatchData(i,&TCaptureFish,sizeof(TCaptureFish),MDM_GM_GAME_NOTIFY,ASS_S_HIT_FISH,0);
							}
						}
						count++;
						if(count>=2)break;
					}
				} 
			}
		}

		m_fishtrace[fishindex][0].m_isHave=false;
		

		//�����˴������һ���ĸ��ʻ�ó����ڵ�
		if((percent%3 == 0 && m_bUserIsSuperPao[byStation]==false)&&(m_fishtrace[fishindex][0].m_fishtype>=15))
		{
			switch(byStation)
			{
			case 0:
				{
					SetTimer(TIME_SUPER_PAO_0, 15*1000); 
					break;
				}
			case 1:
				{
					SetTimer(TIME_SUPER_PAO_1, 15*1000); 
					break;
				}
			case 2:
				{
					SetTimer(TIME_SUPER_PAO_2, 15*1000); 
					break;
				}
			case 3:
				{
					SetTimer(TIME_SUPER_PAO_3, 15*1000); 
					break;
				}
			case 4:
				{
					SetTimer(TIME_SUPER_PAO_4, 15*1000); 
					break;
				}
			case 5:
				{
					SetTimer(TIME_SUPER_PAO_5, 15*1000); 
					break;
				}
			}
			TCaptureFish.bCanSuperPao = true;
			m_bUserIsSuperPao[byStation]=true;

		}   
	
		TCaptureFish.iFishID	=	iFishID;
		TCaptureFish.byStation	=	byStation;
		
		
		//�Ƿ�һ����
		if(1 == m_fishtrace[fishindex][0].m_fudaifishtype && iDeadCount >0)
		{
			TCaptureFish.iCatchCount = iDeadCount;
			memcpy(TCaptureFish.iCatchFishID, iDeadID, sizeof(TCaptureFish.iCatchFishID));
			memcpy(TCaptureFish.iCatchScore, iFishScore, sizeof(TCaptureFish.iCatchScore));

			for(int m=0; m<iDeadCount; m++)
			{
				FishScore +=iFishScore[m];
			}
			OutputDebugString("dwjjcby::Sһ���򾡰�");
		}
		
		TCaptureFish.lFishScore	=	FishScore;
		TCaptureFish.iFishKindScore = fishkindscord;
		//�������
		m_i64UserAllScore[byStation] += TCaptureFish.lFishScore;
		TCaptureFish.i64UserScore = m_i64UserAllScore[byStation];

		for (int i = 0; i < PLAY_COUNT; i ++) 
		{
			if(m_pUserInfo[i])
			{
				SendGameData(i,&TCaptureFish,sizeof(TCaptureFish),MDM_GM_GAME_NOTIFY,ASS_S_HIT_FISH,0);
			}
		}
		//SendWatchData(m_bMaxPeople,&TCaptureFish,sizeof(TCaptureFish),MDM_GM_GAME_NOTIFY,ASS_S_HIT_FISH,0);

		//������㷢�͹㲥
		SendCatchBroadCast(m_fishtrace[fishindex][0].m_fishtype,TCaptureFish);
		
	}
	

	
	m_UserShoot[byStation][iBulletID].iBeilv = 0;
	m_UserShoot[byStation][iBulletID].bIsSuper = false;

	//�ж��Ƿ����һ��������

	return true;
}
/*-----------------------------------------------------------------------------------------------*/
//������㷢�͹㲥
void	CServerGameDesk::SendCatchBroadCast(int iFishType, CMD_S_CaptureFish TCaptureFish)
{
	if(iFishType < 15)
	{
		return;
	}

	CMD_S_CaptureFishBroadcast TCaptureFishBroadcast;
	if(m_pUserInfo[TCaptureFish.byStation])
	{
		//���Ӻ�
		TCaptureFishBroadcast.iDeskNo = m_pUserInfo[TCaptureFish.byStation]->m_UserData.bDeskNO;	
		///<�ǳ�
		memcpy(TCaptureFishBroadcast.szNickName,m_pUserInfo[TCaptureFish.byStation]->m_UserData.nickName ,sizeof(TCaptureFishBroadcast.szNickName));
		///<�������� 
		memcpy(TCaptureFishBroadcast.szRoomName,m_pDataManage->m_InitData.szGameRoomName,sizeof(TCaptureFishBroadcast.szRoomName));
		TCaptureFishBroadcast.byStation = TCaptureFish.byStation;
		TCaptureFishBroadcast.i64FishScore = TCaptureFish.lFishScore;
		TCaptureFishBroadcast.iFishType = iFishType; 
		m_pDataManage->m_TCPSocket.SendDataBatch(&TCaptureFishBroadcast,sizeof(TCaptureFishBroadcast),MDM_GM_GAME_NOTIFY,ASS_S_CATCH_FISH_BROAD,0);

	}
}
/*-----------------------------------------------------------------------------------------------*/
//�Ƿ�����Դ�����
bool	CServerGameDesk::IsProbalityForDead(int iFishIndex)
{
	int iRand = 0;
	if(0 != m_fishtrace[iFishIndex][0].m_mustShoot)
	{
		iRand = rand()%(m_fishtrace[iFishIndex][0].m_mustShoot*10);
	}
	//�жϽ������Ǹ������
	if(G_i64CurrPoolMoney > G_i64RewardLevel[0] && G_i64CurrPoolMoney <= G_i64RewardLevel[1])	//��1�׶�
	{
		if(iRand <= ((int)(G_fRewardLevelProbability[0] * 10.0)-1))
		{
			OutputDebugString("dwjdsnh::�����Դ�����1");
			return true;
		}
	}
	else if(G_i64CurrPoolMoney > G_i64RewardLevel[1] && G_i64CurrPoolMoney <= G_i64RewardLevel[2])//��2�׶�
	{
		if(iRand <= ((int)(G_fRewardLevelProbability[1]*10.0)-1))
		{
			OutputDebugString("dwjdsnh::�����Դ�����2");
			return true;
		}
	}
	else if(G_i64CurrPoolMoney > G_i64RewardLevel[2])	//��3�׶�
	{
		if(iRand <= ((int)(G_fRewardLevelProbability[2]*10.0)-1))
		{
			OutputDebugString("dwjdsnh::�����Դ�����3");
			return true;
		}
	}
	return false;
}
/*-----------------------------------------------------------------------------------------------*/
//�Ƿ񳬼��û�
bool	CServerGameDesk::IsSuperUser(BYTE byStation)
{
	if(byStation < 0 || byStation >= PLAY_COUNT)
	{
		return false;
	}

	for(int i=0; i<m_lvSuperUserID.size();i++)
	{
		if(m_pUserInfo[byStation])
		{
			if(m_pUserInfo[byStation]->m_UserData.dwUserID == m_lvSuperUserID.at(i))
			{
				return true;
			}
		}
	}
	return false;
}
/*-----------------------------------------------------------------------------------------------*/
bool	CServerGameDesk::IsBlackListUser(BYTE byStation)
{
	if(byStation < 0 || byStation >= PLAY_COUNT)
	{
		return true;
	}

	for(int i=0; i<m_lvBlackListID.size();i++)
	{
		if(m_pUserInfo[byStation])
		{
			if(m_pUserInfo[byStation]->m_UserData.dwUserID == m_lvBlackListID.at(i))
			{
				return true;
			}
		}
		
	}
	return false;
}



/*-----------------------------------------------------------------------------------------------*/
//��ȡ��Ϸ״̬��Ϣ
bool	CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	if(bWatchUser)
	{
		return FALSE;
	}

	if(NULL != m_pUserInfo[bDeskStation] && m_bIsPlaying)
	{
		if(m_bIsPlaying)
		{
			//���������Ϊ��Ϸ��
			m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_PLAY_GAME;
		}
		else
		{
			//���������Ϊͬ��״̬
			m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;
		}
	}

	if(!m_bIsPlaying)
	{
		m_bIsPlaying = true;
		//��ʼ������Ϸ
		StartGame();
	}
	//������Ϸ
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_Base TGameStation;
			::memset(&TGameStation,0,sizeof(TGameStation));
			//��Ϸ�汾�˶�
			TGameStation.iVersion		= DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			TGameStation.iVersion2		= DEV_LOW_VERSION;

			TGameStation.iBgindex		= m_iBgindex;				//��������
			TGameStation.byGameStation	= m_bGameStation;			//��Ϸ״̬
			TGameStation.iFireBase		= m_iFireBase;				//��Ϸ�ڻ��������
			//����ڻ���
			for(int i=0;i<PLAY_COUNT; i++)
			{
				TGameStation.iUserFireBase[i]	= m_iUserFireBase[i];
				TGameStation.iUserScore[i]		= m_iUserScore[i];
			}

			if(m_pUserInfo[bDeskStation])
			{
				if(0 == m_pUserInfo[bDeskStation]->m_UserData.isVirtual)
				{
					if(!m_bHaveHitFlag && 255 == m_byHitDesk)
					{
						TGameStation.bHitFlag = true;
						TGameStation.bHitDesk = bDeskStation;
						m_bHaveHitFlag = true;
						m_byHitDesk = bDeskStation;
					}
				}
			}
			
		
			//���Լ���������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			GameUserComeData TGameUserCome;
			TGameUserCome.bComeOrLeave	= true;
			TGameUserCome.byDeskStation = bDeskStation;
			TGameUserCome.iFireBase		= m_iUserFireBase[bDeskStation];
			TGameUserCome.iScore		= m_iUserScore[bDeskStation];
			TGameUserCome.bIsRobot		= m_pUserInfo[bDeskStation]->m_UserData.isVirtual;

			//��������ҷ�����ҽ�������
			for(int i=0; i<PLAY_COUNT; i++)
			{
				if(i == bDeskStation)
				{
					continue;
				}
				if(NULL !=m_pUserInfo[i])
				{
					SendGameData(i,&TGameUserCome,sizeof(TGameUserCome),MDM_GM_GAME_NOTIFY,ASS_S_USER_COME,0);
				}
			}
			return TRUE;
		}
	}
	return false;
}
/*-----------------------------------------------------------------------------------------------*/
//��ʼ������Ϸ
void	CServerGameDesk::StartGame()
{
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(NULL != m_pUserInfo[i])
		{
			//���������Ϊͬ��״̬ 
			m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;
		}
	}
	GameBegin(ALL_ARGEE);
}
/*-----------------------------------------------------------------------------------------------*/
//��Ϸ��ʼ
bool	CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		return false;
	}
	
	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);

	//����������ɼ�ʱ��
	SetTimer(TIME_BUILD_TRACE, m_iCreateFishTime*1000); 
	//������ɼ�ʱ��2
	SetTimer(TIME_CREAD_FISH,m_iCreateFishTime*1100);
	//����������ټ�ʱ��
	SetTimer(TIME_CLEAR_TRACE, m_iCreateFishTime*1000); 
	//�л�������ʱ��
	SetTimer(TIME_CHANGE_SCENE,m_iChangeScene*60*1000);
	
	return TRUE;
}
/*-----------------------------------------------------------------------------------------------*/
//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillTimer(TIME_BUILD_TRACE);
	KillTimer(TIME_CREAD_FISH);
	KillTimer(TIME_CLEAR_TRACE);
	KillTimer(TIME_CHANGE_SCENE);
	KillTimer(TIME_REGULAR_FISH);
	KillTimer(TIME_SUPER_PAO_0);
	KillTimer(TIME_SUPER_PAO_1);
	KillTimer(TIME_SUPER_PAO_2);
	KillTimer(TIME_SUPER_PAO_3);
	KillTimer(TIME_SUPER_PAO_4);
	KillTimer(TIME_SUPER_PAO_5);


	m_bIsPlaying = false;		//��Ϸ�Ƿ��Ѿ���ʼ
	//��Ⱥ��ʶ
	m_dwFishID = 0;

	//��Ӧ���������·�ߣ�5������ͽǶ�
	memset(m_fishtrace,0,sizeof(m_fishtrace));
	//����ӵ���Ϣ
	memset(m_UserShoot,0,sizeof(m_UserShoot));
	
	//��ҵ��ڻ���
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_iUserFireBase[i]	 = m_iFireBase;
		m_iUserScore[i]		 = 0;
		m_i64UserAllScore[i] = 0;
		m_i64UserMoney[i]	 = 0;
		m_iUserShootCount[i] = 0;

		m_bUserIsSuperPao[i] = false;

		m_iUserPercent[i]	= 100;
	}
	return true;
}







/*-----------------------------------------------------------------------*/
//��Ϸ����
bool	CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	KillTimer(TIME_MY_TIMER);
	switch (bCloseFlag)
	{
	case GF_NORMAL:				//��Ϸ��������
	case GF_SALE:				//��Ϸ��ȫ����
	case GFF_FORCE_FINISH:		//�û������뿪
		{	
			m_bIsPlaying = false;
			m_bGameStation	=	GS_WAIT_SETGAME;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);

			return true;
		}
	}

	m_bIsPlaying = false;
	//��������
	ReSetGameState(bCloseFlag);
	__super::GameFinish(bDeskStation,bCloseFlag);

	return true;
}
/*------------------------------------------------------------------------------*/
//�ж��Ƿ�������Ϸ
bool	CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bIsPlaying )
	{
		return false;
	}
	else
	{
		return false;
	}
}
/*------------------------------------------------------------------------------*/
///�жϴ���Ϸ���Ƿ�ʼ��Ϸ (��Ϊ����Ƚ����� ����뿪������false ������뿪���� �����Ƕ���)
BOOL	CServerGameDesk::IsPlayingByGameStation()
{
	return FALSE;
}
/*------------------------------------------------------------------------------*/
//�û��뿪��Ϸ��
BYTE	CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	if(m_pUserInfo[bDeskStation])
	{
		//�Ϸּ��뵽��ҽ�ҵ���
		m_i64UserMoney[bDeskStation] += (m_i64UserAllScore[bDeskStation]*m_iBiliDuihuan);

		__int64 iTmpMoney = m_i64UserMoney[bDeskStation] - m_pUserInfo[bDeskStation]->m_UserData.i64Money;

		//д�����ݿ�				
		bool temp_cut[PLAY_COUNT];
		memset(temp_cut,0,sizeof(temp_cut));

		__int64	iTurePoint[PLAY_COUNT];
		memset(iTurePoint,0,sizeof(iTurePoint));
		iTurePoint[bDeskStation] = iTmpMoney;
		
		ChangeUserPointint64(iTurePoint, temp_cut);

		//��¼��ҽ�ұ仯
		__int64		iChangeMoney[PLAY_COUNT];				//��ҽ��
		memset(iChangeMoney, 0 , sizeof(iChangeMoney));
		__super::RecoderGameInfo(iChangeMoney);
	}

	//��ո�λ�õ�����
	IniUserData(bDeskStation);


	GameUserComeData TGameUserCome;
	TGameUserCome.bComeOrLeave	= false;
	TGameUserCome.byDeskStation = bDeskStation;
	TGameUserCome.iFireBase		= m_iUserFireBase[bDeskStation];
	TGameUserCome.iScore		= m_iUserScore[bDeskStation];

	//��������ҷ�������뿪��Ϣ
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == bDeskStation)
		{
			continue;
		}
		if(NULL !=m_pUserInfo[i])
		{
			SendGameData(i,&TGameUserCome,sizeof(TGameUserCome),MDM_GM_GAME_NOTIFY,ASS_S_USER_COME,0);
		}
	}

	__super::UserLeftDesk(bDeskStation,pUserInfo);

	//�ж��Ƿ����е���Ҷ��뿪��
	bool bAllLeft = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(m_pUserInfo[i])
		{
			bAllLeft = false;
			break;
		}
	}

	//���潱�ؼ�¼
	RecordPool();
	
	//�����˶��뿪�� �ͽ�����Ϸ
	if(bAllLeft)
	{
		GameFinish(bDeskStation,GF_NORMAL);
	}

	return true;
}
/*------------------------------------------------------------------------------*/
//�û�����
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	//����Ҳ�����뿪
	UserLeftDesk(bDeskStation,pUserInfo);
	return true;
}
/*------------------------------------------------------------------------------*/
///�û�������Ϸ��
BYTE	CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	//���һ���� �ͼ�¼����ҽ�����ʱ�� �����ж��ٽ��
	if(pUserSit && pUserInfo)
	{
		m_i64UserMoney[pUserSit->bDeskStation] = pUserInfo->m_UserData.i64Money/*/m_pDataManage->m_InitData.uBasePoint*/;
	}
	
	return __super::UserSitDesk(pUserSit,pUserInfo);
}
/*------------------------------------------------------------------------------*/
//���潱�ؼ�¼
void	CServerGameDesk::RecordPool()
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	if (!CINIFile::IsFileExist(temp))
	{
		return ;
	}
	CINIFile f(temp);
	CString key = TEXT("game");

	__int64 i64Tmp = f.GetKeyVal(key,"CurrPoolMoney",G_i64CurrPoolMoney);	

	//�����֮ǰ��¼�Ľ��ز�һ�µĻ� ˵���ֶ��޸���
	if(i64Tmp != G_i64PrePoolMoney)
	{
		G_i64PrePoolMoney	= i64Tmp;
		G_i64CurrPoolMoney	= i64Tmp;
	}
	else
	{
		G_i64PrePoolMoney = G_i64CurrPoolMoney;
	}

	CString strInfo;
	strInfo.Format("%I64d",G_i64CurrPoolMoney);
	f.SetKeyValString(key,"CurrPoolMoney",strInfo);

}
/*------------------------------------------------------------------------------*/
//�ж��Ƿ�ȫ�����ǻ�����
bool	CServerGameDesk::IsAllRobot()
{
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(!m_pUserInfo[i])
		{
			continue;
		}

		if(0 == m_pUserInfo[i]->m_UserData.isVirtual)
		{
			return false;
		}
	}

	return true;
}
/*------------------------------------------------------------------------------*/