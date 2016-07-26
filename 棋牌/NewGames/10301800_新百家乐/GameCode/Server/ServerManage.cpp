#include "StdAfx.h"
#include "ServerManage.h"
#include "Shlwapi.h"
///
void DebugPrintf(const char *p, ...)
{
	return ;
	char szFilename[256];
	sprintf( szFilename, "%dServer.txt", NAME_ID);
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

__int64 my_atoi(const char *str)
{
	 __int64 result = 0;
	 int signal = 1;
	 /* Ĭ��Ϊ���� */
	 if((*str>='0'&&*str<='9')||*str=='-'||*str=='+')
	 {
		 if(*str=='-'||*str=='+')
		 {
			 if(*str=='-')
			 signal = -1; /* ���븺�� */
		 str++;
		 }
	 }
	 else return 0;/* ��ʼת�� */
	 while(*str>='0'&&*str<='9')
		 result = result*10+(*str++ -'0');
	 return signal*result;
 }
///���ƣ�CServerGameDesk
///���������������캯��
///@param ALL_ARGEE ��Ϸ��ʼģʽ���������ͬ�⼴�ɿ�ʼ��
///@return 
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_bGameStation=GS_WAIT_AGREE;  // ��ʼΪ�ȴ���ʼ״̬

							//�Ƿ�����	
	m_vtSuperUserID.clear();
	m_iZhuangBaShu=0;
				                    // ���Ƶ���Ŀ



	////////////////////////�ټ��ֳ�ʼ������lym//////////////////////////////////////////////////////////////////

	memset(m_iZhuangStation, -1, sizeof(m_iZhuangStation)); //ׯ���б��ܵ�ׯ��λ��
	memset(m_iZPaiXing, 0, sizeof(m_iZPaiXing));	 //0�ŵ�����1�����͡����ͣ�0������1������2�ͣ�3ͬ���
	memset(m_iXPaiXing, 0, sizeof(m_iXPaiXing));	//0�ŵ�����1�����͡����ͣ�0������1������2�ͣ�3ͬ���

	memset(m_iAIMoney, 0, sizeof(m_iAIMoney));	/**< �����˵���ע*/
	memset(m_iQuYuZhu, 0, sizeof(m_iQuYuZhu));	//����ÿ�������µ�ע��
	memset(m_i64MaxZhu, 0, sizeof(m_i64MaxZhu));	//����ÿ������������µ�ע��
	memset(m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���

	memset(m_iMax, 0, sizeof(m_iMax));			//����ÿ��������Ƶ������ע��

	m_DqLuziData.clear();
	memset(m_byUserCard, 255, sizeof(m_byUserCard));		    // �û����ϵ��˿�
	

	for(int i=0;i<PLAY_COUNT;i++)
	{
		for(int j=0;j<BET_ARES+1;j++)
		{
			m_iUserXiaZhuData[i][j]=0;
		}
	}
	m_iMaxZhuang = 30;			//ׯ��һ����������پ֣�Ĭ��30��

	m_iWinner = 0;				//Ӯ��1 ׯ��2�У�3��
	m_iKaiPai =0;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	m_bThreeCard=false;			//ׯ��������3����

	m_i64ZhongZhu=0;			//���ѵ�ǰ��ע��



	m_iZhuangBaShu=0;		//ׯ�ҽ����˼���

	m_iXiaZhuTime = 0;			//��עʱ��			
	m_iKaiPaiTime = 0;			//����ʱ��
	m_iFreeTime = 0;			//����ʱ��

	m_iNowNtStation=-1;			//��ǰׯ��λ��

	m_bXiaZhuang=false;			//��ǰׯ��������ׯ

	m_iShangZhuangLimit=0;		//��ׯ��Ҫ�����ٽ��
	m_i64NtWin =0;				//��ǰׯ��Ӯ�Ľ��
	m_bIsFirstLoadIni = true;	//��һ��ȥ�������ļ�

	////////////////////////////////////////////////////////////////////////////////////////////
	int i=0;
	for (i=0; i<PLAY_COUNT; ++i)
	{
		m_i64UserFen[i] =0;          //������ұ��ֵĵ÷�
		m_iUserPlayCount[i][0] = 0;
		m_iUserPlayCount[i][1] = 0;

	}
	
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_wSuperCardList[i],0,sizeof(m_wSuperCardList[i]));
	}
	m_bySuperStation = -1;
#endif



	m_bSuperSetting = FALSE;
	m_bSuperState = 0;



	m_bGamePlaying = false;			///lym2010-03-19��¼��Ϸ�Ƿ����ڽ�����



  	

	m_bCanNote = false;			//�Ƿ�����ע	

	
	memset(m_IsAIStation,0,sizeof(m_IsAIStation));///��¼�����˵�λ��
	///ׯ�ҳ�ˮ����
	m_byNtTax = 0;	
	///�����ע�ⶥ
	m_iMaxNote = 0;//Ĭ��Ϊ������

	m_bAIWinAndLostAutoCtrl = false;///�Ƿ�����������Ӯ�Զ�����
	m_iAIWantWinMoney = 0;		///������ҪӮ��Ǯ
	m_iAIHaveWinMoney = 0;		///�������Ѿ�Ӯ�˶���Ǯ
	m_iAIMaxWinInOne = 0;		///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�		


	m_iApplyNtAINum = 0;			///��Ҫ��ׯ�Ļ��������������ƻ�������ׯ��
	m_iAINtZhuangCount =0;		///���������ڵİ��������ƻ�������ׯ��

	m_iRobotSZCount = 0;
		
}
///���ƣ�~CServerGameDesk
///���������������캯��
///@param 
///@return 
CServerGameDesk::~CServerGameDesk(void)
{	
}
///���ƣ�InitDeskGameStation
///��������ʼ�����Ӻ�����һ����������������ļ�
///@param 
///@return 
bool CServerGameDesk::InitDeskGameStation()
{
	//���������ļ�
	LoadIni();
	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
///���ƣ�LoadExtIni
///���������ط�������ini�����ļ�������ʱ�䣬����֡��
///@param 
///@return 
BOOL CServerGameDesk::LoadIni()
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	TCHAR szSec[_MAX_PATH] = TEXT("game");
	///��ȡ��עʱ��
	m_iXiaZhuTime = f.GetKeyVal(szSec,"xiazhutime",15);	
	if(m_iXiaZhuTime<1)
	{
		m_iXiaZhuTime=10;
	}
	///��ȡ����ʱ��
	m_iKaiPaiTime = f.GetKeyVal(szSec,"kaipaitime",15);	
	if(m_iKaiPaiTime<1)
	{
		m_iKaiPaiTime=10;
	}
	m_iShowResult = f.GetKeyVal(szSec,"ShowResultTime",5);
	if (m_iShowResult <1)
	{
		m_iShowResult = 5;
	}
	///��ȡ����ʱ��
	m_iFreeTime = f.GetKeyVal(szSec,"freetime",10);		
	if(m_iFreeTime<1)
	{
		m_iFreeTime=10;
	}

	///��ȡ��ׯ��Ҫ�����ٽ��
	m_iShangZhuangLimit = f.GetKeyVal(szSec,"ShangZhuangMoney",1000);
	///ׯ��һ����������پ֣�Ĭ��30��
	m_iMaxZhuang = f.GetKeyVal(szSec,"maxzhuang",30);

	///����ׯ�������ע��
	m_iMax[GF_ZHUANG] = f.GetKeyVal(szSec,"max0",0);		
	///����ׯ�����������ע��
	m_iMax[GF_Z_TIANWANG] = f.GetKeyVal(szSec,"max1",0);	
	///����ׯ�����������ע��
	m_iMax[GF_Z_DUIZI] = f.GetKeyVal(szSec,"max2",0);		
	///�������������ע��
	m_iMax[GF_XIAN] = f.GetKeyVal(szSec,"max3",0);		
	///�����������������ע��
	m_iMax[GF_X_TIANWANG] = f.GetKeyVal(szSec,"max4",0);	
	///�����ж����������ע��
	m_iMax[GF_X_DUIZI] = f.GetKeyVal(szSec,"max5",0);		
	///���ƺ��������ע��
	m_iMax[GF_HE] = f.GetKeyVal(szSec,"max6",0);		
	///����ͬ����������ע��
	m_iMax[GF_T_HE] = f.GetKeyVal(szSec,"max7",0);
	//ׯ��
	m_iMax[GF_Z_DAN] = f.GetKeyVal(szSec,"max8",0);
	//ׯ˫
	m_iMax[GF_Z_SHUANG] = f.GetKeyVal(szSec,"max9",0);
	//�е�
	m_iMax[GF_X_DAN] = f.GetKeyVal(szSec,"max10",0);
	//��˫
	m_iMax[GF_X_SHUANG] = f.GetKeyVal(szSec,"max11",0);

	///ׯ�ҳ�ˮ����
	m_byNtTax = f.GetKeyVal(szSec,"NtTax",0);	
	///��ǰ����������µ���ע
	m_iMaxNote = f.GetKeyVal(szSec,"MaxNote",-1);//Ĭ��Ϊ500��	


	///��������Ӯ�Զ�����
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",0);	///�Ƿ�����������Ӯ�Զ�����
	m_iAIWantWinMoney = f.GetKeyVal(szSec,"AIWantWinMoney",0);				///������ҪӮ��Ǯ
	m_iAIMaxWinInOne = f.GetKeyVal(szSec,"AIMaxWinInOne",0);				///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	

	// ��ȡ��������ׯ�������
	m_iRobotSZCount = f.GetKeyVal(szSec, "RobotSZCount", 15);

	if (m_iRobotSZCount < 1)
	{
		m_iRobotSZCount = 15;
	}

	m_iLevelBase = f.GetKeyVal(szSec , "LevelBase" , 12) ; //ƽ�ı��� 

	//��ȡ�������� m_vtSuperUserID
	CString key = "SuperSet";
	// �жϳ����û��ڷ��������Ƿ�����
	int iCount = f.GetKeyVal(key, "SuperUserCount", 0);
	CString strText;
	for (int j = 1; j <= iCount; j++)
	{
		strText.Format("SuperUserID%d", j);
		long int  lUserID = f.GetKeyVal(key, strText, 0);
		m_vtSuperUserID.push_back(lUserID);
	}
	return TRUE;
}

///���ƣ�LoadExtIni
///���������ݷ���ID���������ļ�
///@param iRoomID �����
///@return 
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.INI"));
	TCHAR szSec[_MAX_PATH] = TEXT("game");
	_stprintf_s(szSec, sizeof(szSec), _T("%s_%d"), SKIN_FOLDER,iRoomID);

	///��ȡ��עʱ��
	m_iXiaZhuTime = f.GetKeyVal(szSec,"xiazhutime",m_iXiaZhuTime);	
	if(m_iXiaZhuTime<10)
	{
		m_iXiaZhuTime=10;
	}
	///��ȡ����ʱ��
	m_iKaiPaiTime = f.GetKeyVal(szSec,"kaipaitime",m_iKaiPaiTime);	
	if(m_iKaiPaiTime<10)
	{
		m_iKaiPaiTime=10;
	}
	m_iShowResult = f.GetKeyVal(szSec,"ShowResultTime",m_iShowResult);
	if (m_iShowResult <1)
	{
		m_iShowResult = 5;
	}
	///��ȡ����ʱ��
	m_iFreeTime = f.GetKeyVal(szSec,"freetime",m_iFreeTime);	
	if(m_iFreeTime<10)
	{
		m_iFreeTime=10;
	}

	///��ȡ��ׯ��Ҫ�����ٽ��
	m_iShangZhuangLimit = f.GetKeyVal(szSec,"ShangZhuangMoney",m_iShangZhuangLimit);
	///ׯ��һ����������پ֣�Ĭ��30��
	m_iMaxZhuang = f.GetKeyVal(szSec,"maxzhuang",m_iMaxZhuang);		


	///����ׯ�������ע��
	m_iMax[GF_ZHUANG] = f.GetKeyVal(szSec,"max0",m_iMax[GF_ZHUANG]);		
	///����ׯ�����������ע��
	m_iMax[GF_Z_TIANWANG] = f.GetKeyVal(szSec,"max1",m_iMax[GF_Z_TIANWANG]);	
	///����ׯ�����������ע��
	m_iMax[GF_Z_DUIZI] = f.GetKeyVal(szSec,"max2",m_iMax[GF_Z_DUIZI]);		
	///�������������ע��
	m_iMax[GF_XIAN] = f.GetKeyVal(szSec,"max3",m_iMax[GF_XIAN]);		
	///�����������������ע��
	m_iMax[GF_X_TIANWANG] = f.GetKeyVal(szSec,"max4",m_iMax[GF_X_TIANWANG]);	
	///�����ж����������ע��
	m_iMax[GF_X_DUIZI] = f.GetKeyVal(szSec,"max5",m_iMax[GF_X_DUIZI]);		
	///���ƺ��������ע��
	m_iMax[GF_HE] = f.GetKeyVal(szSec,"max6",m_iMax[GF_HE]);		
	///����ͬ����������ע��
	m_iMax[GF_T_HE] = f.GetKeyVal(szSec,"max7",m_iMax[GF_T_HE]);	
	//ׯ��
	m_iMax[GF_Z_DAN] = f.GetKeyVal(szSec,"max8",m_iMax[GF_Z_DAN]);
	//ׯ˫
	m_iMax[GF_Z_SHUANG] = f.GetKeyVal(szSec,"max9",m_iMax[GF_Z_SHUANG]);
	//�е�
	m_iMax[GF_X_DAN] = f.GetKeyVal(szSec,"max10",m_iMax[GF_X_DAN]);
	//��˫
	m_iMax[GF_X_SHUANG] = f.GetKeyVal(szSec,"max11",m_iMax[GF_X_SHUANG]);
	
	///ׯ�ҳ�ˮ����
	m_byNtTax = f.GetKeyVal(szSec,"NtTax",m_byNtTax);	
	///��ǰ����������µ���ע
	m_iMaxNote = f.GetKeyVal(szSec,"MaxNote",m_iMaxNote);//Ĭ��Ϊ500��	

	///��������Ӯ�Զ�����
	m_bAIWinAndLostAutoCtrl = f.GetKeyVal(szSec,"AIWinAndLostAutoCtrl",m_bAIWinAndLostAutoCtrl);///�Ƿ�����������Ӯ�Զ�����
	m_iAIWantWinMoney = f.GetKeyVal(szSec,"AIWantWinMoney",m_iAIWantWinMoney);			///������ҪӮ��Ǯ
	m_iAIMaxWinInOne = f.GetKeyVal(szSec,"AIMaxWinInOne",m_iAIMaxWinInOne);				///������ƽ��ÿ�����ӮǮ��	��С�ڵ���0ʱΪ���ޣ�	

	// ��ȡ��������ׯ�������
	m_iRobotSZCount = f.GetKeyVal(szSec, "RobotSZCount", m_iRobotSZCount);
	if (m_iRobotSZCount <1)
	{
		m_iRobotSZCount = 15;
	}

	return TRUE;
}
//-------------------------------add-by-DWJ-20121108-----------------------------------------------------------------------------------------
//�������ļ��ж�ȡ�������Ѿ�Ӯ�˶���Ǯ����Ϊ��ÿ�̽����ʱ�򶼻Ὣ������ӮǮ����д�������ļ��С� 
//������ÿ�̿�ʼ��ʱ���ֶ�ȡ�£���Ϊ�������޸�����ô�ͻ�������Ǯ��.
void CServerGameDesk::GetAIContrlSetFromIni(int iRoomID)
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s +SKIN_FOLDER  + _T("_s.ini"));
	CString szSec = TEXT("game");
	szSec.Format("%s_%d",SKIN_FOLDER,iRoomID);

	m_iAIWantWinMoneyA1	= f.GetKeyVal(szSec,"AIWantWinMoneyA1 ",100000);		/**<������ӮǮ����1  */
	m_iAIWantWinMoneyA2	= f.GetKeyVal(szSec,"AIWantWinMoneyA2 ",1000000);		/**<������ӮǮ����2  */
	m_iAIWantWinMoneyA3	= f.GetKeyVal(szSec,"AIWantWinMoneyA3 ",10000000);		/**<������ӮǮ����3  */
	m_iAIWinLuckyAtA1	= f.GetKeyVal(szSec,"AIWinLuckyAtA1 ",90);				/**<������������1ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA2	= f.GetKeyVal(szSec,"AIWinLuckyAtA2 ",70);				/**<��������Ӯ���ƣ�������������2ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA3	= f.GetKeyVal(szSec,"AIWinLuckyAtA3 ",50);				/**<��������Ӯ���ƣ�������������3ӮǮ�ĸ���  */
	m_iAIWinLuckyAtA4	= f.GetKeyVal(szSec,"AIWinLuckyAtA4 ",30);				/**<��������Ӯ���ƣ�������������4ӮǮ�ĸ���  */
	m_iAddMoneyEveryDay	= f.GetKeyVal(szSec,"AddMoneyEveryDay ",0);				/**<��������Ӯ���ƣ�������ÿ����ӮǮA1��A2��A3���������Ǯ����ֵ  */
	m_iAIHaveWinMoney	= f.GetKeyVal(szSec,"ReSetAIHaveWinMoney ",0);			/**<��������Ӯ���ƣ�ֱ�Ӵ������ļ��ж�ȡ�������Ѿ�ӮǮ����Ŀ  */

	CheckNewDayReSetIni(&f,&szSec);
}
//---------------------------add-by-DWJ-20121108---------------------------------------------------------
//����Ƿ����һ��,����һ���Ҫ�޸������ļ���A1~A3�����ֵ
void CServerGameDesk::CheckNewDayReSetIni(CINIFile *f, CString *key)
{	//ddwwjj
	CTime Cttm;
	CString sDay,sValue;
	Cttm=CTime::GetCurrentTime();
	sDay  = Cttm.Format(("%Y%m%d"));
	if (m_bIsFirstLoadIni)
	{//��һ�ζ�ȡ�����ļ�������ֻ������ȡһ��
		m_iRecFroDayTime  = StrToInt(sDay);
		m_bIsFirstLoadIni = false;
	}
	if (m_iRecFroDayTime < StrToInt(sDay))
	{//�����¼��ʱ��С�ڵ�ǰ�����ڣ���ʾ��ǰʱ�����24�㣬�����Ѿ���һ��,���µ�һ�졣
		m_iRecFroDayTime = StrToInt(sDay);
		if (0 !=m_iAddMoneyEveryDay)
		{//���ÿ�յ���Ǯ���Ĳ�Ϊ0�ſ�����A1��A2��A3����������ܣ�д�������ļ�����
			sValue.Format("%d",(m_iAIWantWinMoneyA1+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA1 ",sValue);		/**<������ӮǮ����1  */

			sValue.Format("%d",(m_iAIWantWinMoneyA2+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA2 ",sValue);		/**<������ӮǮ����2  */

			sValue.Format("%d",(m_iAIWantWinMoneyA3+m_iAddMoneyEveryDay));
			f->SetKeyValString(*key,"AIWantWinMoneyA3 ",sValue);		/**<������ӮǮ����3  */
		}
	}	
}



/************************************************************************/
///���ƣ�GameBegin
///��������Ϸ��ʼ
///@param 
///@return 
bool	CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if(!m_bGamePlaying)
	{
		return false;
	}
	m_bGamePlaying = true;
	// ����ϵͳ��ʼ����Ϸ��ʼ״̬
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		return false;
	}
	m_bGameStation = GS_NOTE_STATE;         // ��Ϸһ��ʼ�ͽ�����ע״̬��
	ServerInit();
	//---------------------------ADD--BY---DWJDWJ------------------------------------
	//����ÿ�̸��ݷ���ID��ȡ�������ļ��е�ReSetAIHaveWinMoney ���� �趨�»�����Ӯ�˶���Ǯ
	GetAIContrlSetFromIni(m_pDataManage->m_InitData.uRoomID);

	m_bCanNote = true;			//�Ƿ�����ע

	m_iZhuangBaShu++;		//ׯ�ҽ����˼���
	//��ȡ�������������ע
	GetQuYuCanNote();

	// ֪ͨ������Ҹ�����ͼ
	BEGIN_DATA BeginData;
	BeginData.m_iXiaZhuTime = m_iXiaZhuTime;		//��עʱ��
	BeginData.m_iKaiPaiTime = m_iKaiPaiTime;		//����ʱ��
	BeginData.m_iShowResult = m_iShowResult;		//�������ʾʱ��
	BeginData.m_iFreeTime	= m_iFreeTime;			//����ʱ��

	BeginData.m_iNowNtStation	= m_iNowNtStation;
	BeginData.m_i64NtWin		= m_i64NtWin;//ׯ�ҵĳɼ�
	BeginData.m_iZhuangBaShu	= m_iZhuangBaShu;

	for(int i=0;i<BET_ARES;i++)
	{
		BeginData.m_iMaxZhu[i] = m_i64MaxZhu[i];
	}

	for(int i=0;i<=PLAY_COUNT;i++)//��ׯ���б�
	{
		BeginData.m_iNTlist[i] = m_iZhuangStation[i];
	}

	
	BeginData.m_iShangZhuangLimit = m_iShangZhuangLimit;//��ׯ����

	// add by wys ����ǰׯ�ҵ�ID����ƽ̨�����ׯ�Ҳ�������ׯ�ڼ��Ǯ������
	long iNTUserID = 0;
	if (NULL != m_pUserInfo[m_iNowNtStation])
	{
		iNTUserID = m_pUserInfo[m_iNowNtStation]->m_UserData.dwUserID;		
	}
	SetCurrentZhuangjia(iNTUserID);
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			m_iUserPlayCount[i][0] = 0;
			m_iUserPlayCount[i][1] = 0;
			continue;	
		}				
		if(m_pUserInfo[i]->m_UserData.isVirtual != 0)//��������Ӯ���ƣ���ȡ���������λ��
		{
			m_IsAIStation[i] = true;
		}		
		BeginData.iMaxNote = CanDouble(i)?2*m_iMaxNote:m_iMaxNote;				//������ע�ⶥ

		SendGameData(i, &BeginData, sizeof(BEGIN_DATA), MDM_GM_GAME_NOTIFY, ASS_BJL_BEGIN, 0);//������ע��Ϣ
	}

	m_bGameStation=GS_NOTE_STATE;//��ע״̬

	//������ע��ʱ��
	SetTimer(ID_TIMER_SEND_CARD, (m_iXiaZhuTime+1)*1000); 
	return true;
}
/*----------------------------------------------------------------------------*/

///���ƣ�GameFinish
///��������Ϸ����
///@param byDeskStation ����λ�ã�bCloseFlag ������ʽ
///@return 
bool CServerGameDesk::GameFinish(BYTE byDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:///lym0512����������Ϸ�����
		{
			m_bSuperSetting = FALSE;
			m_bSuperState = 0;
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			for (int i=0; i<PLAY_COUNT; i++)
			{
				temp_cut[i] = false;
			}		
			break;
		}
	case GF_SAFE:
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			if(m_bGameStation>=20 && m_bGameStation<=23)
			{
				//������ǿ������ʾǿ����Ϣ��add by wlr 20090807
				if (bCloseFlag == GFF_FORCE_FINISH)
				{
					int i = 0;
					// ���ǿ��
					GAME_FORCE_QUIT ForceQuit;
					ForceQuit.byDeskStation = byDeskStation;
					for (i=0; i<PLAY_COUNT; ++i)
					{
						if (NULL == m_pUserInfo[i])
						{
							continue;
						}
						SendGameData(i,&ForceQuit,sizeof(ForceQuit),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
					}

					bool bIsPlayerBetted = false;
					if (m_iUserXiaZhuData[byDeskStation][BET_ARES] != 0)
					{
						bIsPlayerBetted = true;
					}
					if ((byDeskStation != m_iNowNtStation)&& (!bIsPlayerBetted) && byDeskStation!=255)
					{						
						MakeUserOffLine(byDeskStation);
					}
				}
				return true;
			}
			m_bGameStation = GS_WAIT_AGREE;
			int i = 0;
			// ���ǿ��
			GAME_FORCE_QUIT CutEnd;
			CutEnd.byDeskStation = byDeskStation;
			if (GF_SAFE == bCloseFlag)
			{
				for (i=0; i<PLAY_COUNT; ++i)
				{
					if (NULL == m_pUserInfo[i])
					{
						continue;
					}
					SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SAFE_END,0);
				}
			}
			else
			{				
				bCloseFlag = GFF_FORCE_FINISH;
				bool bIsPlayerBetted = false;
				if (m_iUserXiaZhuData[byDeskStation][BET_ARES] != 0)
				{
					bIsPlayerBetted = true;
				}
				if ((byDeskStation != m_iNowNtStation)&& (!bIsPlayerBetted) && byDeskStation!=255)
				{
					MakeUserOffLine(byDeskStation);
				}
			}
			return true;
			break;
		}
	default:
		break;
	}
	m_bGamePlaying = false;
	ASSERT(CHECK_DEBUG);
	return __super::GameFinish(byDeskStation,bCloseFlag);
}


/**
* �����Ϸ���ݰ�������
*
* @param [in] bDeskStation ���λ��
*
* @param [in] pNetHead ��Ϣͷ
*
* @param [in] pData ��Ϣ��
*
* @param [in] uSize ��Ϣ���Ĵ�С
*
* @param [in] uSoketID Socket ID
*
* @param [in] bWatchUser �Թ�
*
* @return ���ز�������
*/
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:	 //ǿ���˳�
		{
			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

///���ƣ�HandleNotifyMessage
///��������Ϸ���ݰ������� 
///@param byDeskStation λ�� pNetHead ��Ϣͷ pData ��Ϣ�� uSize ��Ϣ���Ĵ�С uSocketID Socket ID  bWatchUser �Թ�
///@return 
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_SUPER_SETTING:  //��������
		{
			if (uSize != sizeof(SUPERSETTING))
			{
				return true;
			}

			SUPERSETTING * pSuperStting = (SUPERSETTING *)pData;
			if (NULL == pSuperStting)
			{
				 return false;
			}
			if (m_bGameStation == GS_NOTE_STATE)
			{
				OnSuperSetting(bDeskStation, pSuperStting->bState);
			}
			
			return true;
		}
	case ASS_SUPER_AI_INFO://////��������Ӯ���� �������ͻ��˿������û����˿��Ʋ���
		{
			if (sizeof(AICTRLINFO) != uSize)
			{
				return false;
			}			
			SetAICtrl(bDeskStation,pData,uSize);

			return true;
		}
	case ASS_XIA_ZHU:           // ��ע��Ϣ	DWJ
		{
			if (sizeof(XIA_ZHU) != uSize)
			{
				return false;
			}
			if (GS_NOTE_STATE!=m_bGameStation || NULL==m_pUserInfo[m_iNowNtStation] || !m_bCanNote)
			{
				return true;
			}
			HanleXiaZhu(bDeskStation,pData, uSize);
			return true;
		}
	case ASS_SHANG_ZHUANG:      // ��ׯ��Ϣ	DWJ
		{
			if (sizeof(SHANG_ZHUANG) != uSize)
			{
				return false;
			}			
			SHANG_ZHUANG *pdata = (SHANG_ZHUANG *)pData;			
			if(pdata->shang && m_pUserInfo[bDeskStation] != NULL)//��ׯ����
			{	
				if (m_pUserInfo[bDeskStation]->m_UserData.isVirtual>0)
				{					
					if (CountRobortNum()>m_iRobotSZCount)
					{
						return true;
					}
				}
				
				if(m_pUserInfo[bDeskStation]->m_UserData.i64Money>= m_iShangZhuangLimit && m_pUserInfo[bDeskStation]->m_UserData.i64Money<1500000000000000000)
				{
					HanleSzhuang(bDeskStation,pData, uSize);
				}
				else 
				{
					SHANG_ZHUANG lost;
					lost.shang = pdata->shang;
					lost.success= 1;
					lost.m_iNowNtStation = m_iNowNtStation;
				
					for(int i=0;i<=PLAY_COUNT;i++)
					{
						lost.zhuangstation[i] = m_iZhuangStation[i];
					}
					if (NULL !=m_pUserInfo[bDeskStation])
					{						
						if (bDeskStation == pdata->station)
						{
							SendGameData(bDeskStation,&lost,sizeof(lost),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG_ERR,0);//������ׯʧ����Ϣ
						}	
					}					
				}
			}
			else
			{
				HanleXzhuang(bDeskStation,pData, uSize);
			}
			return true;
		}
	default:
		break;
	}
	ASSERT(CHECK_DEBUG);
	return __super::HandleNotifyMessage(bDeskStation, pNetHead, pData, uSize, uSocketID, bWatchUser);
}
///���ƣ�IsPlayGame
///�������ж��Ƿ�������Ϸ
///@param byDeskStation λ��
///@return true ������Ϸ��false ������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if((m_bGameStation >= GS_NOTE_STATE)&& (m_bGameStation < GS_WAIT_NEXT))
	{
		//�����ׯ��
		if (bDeskStation == m_iNowNtStation)
		{
			return true;
		}
		//�����ע��
		if (m_iUserXiaZhuData[bDeskStation][BET_ARES] > 0)
		{
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}
///���ƣ�UserLeftDesk
///�������û��뿪��Ϸ�� 
///@param byDeskStation λ�� pUserInfo ����
///@return 
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{

	m_i64UserWin[bDeskStation] = 0;
	m_iUserPlayCount[bDeskStation][0] = 0;
	m_iUserPlayCount[bDeskStation][1] = 0;
	if(m_iNowNtStation==bDeskStation)//ׯ���뿪��
	{
		m_iNowNtStation	= -1;//���õ�ǰׯ��Ϊ-1	
		m_i64NtWin		= 0;	//��ǰׯ��Ӯ�Ľ��
		m_iZhuangBaShu	= 0;//ׯ�ҽ����˼���
		m_bXiaZhuang	= false;

		if(NULL==m_pUserInfo[m_iNowNtStation])//��ǰׯ�Ҳ�����
		{
			//������ׯ�б�
			MoveData(0);
			GetNextNT();
			if(m_pUserInfo[m_iNowNtStation])
			{
				//���ͻ�ׯ��Ϣ
				for(int i = 0; i < PLAY_COUNT; ++i)
				{
					if(NULL != m_pUserInfo[i])
					{
						SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_CHANGE_NT,0);
					}
				}
			}
		}
		//������ׯ�ȴ���Ϣ			
		tagNoZhuang TNoZhuang;
		TNoZhuang.iNtStation = -1;
		TNoZhuang.i64NtWin = 0;				//��ǰׯ��Ӯ�Ľ��
		TNoZhuang.iZhuangBaShu = 0;			//ׯ�ҽ����˼���
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL != m_pUserInfo[i])
			{
				SendGameData(i,&TNoZhuang,sizeof(TNoZhuang),MDM_GM_GAME_NOTIFY, ASS_WU_ZHUANG,0);////������ׯ�ȴ���Ϣ
			}
		}
	}
	else//�˳�����������ׯ�б����������ׯ�б�λ��
	{
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_iZhuangStation[i] == bDeskStation)
			{
				m_iZhuangStation[i]=-1;
				MoveData(0);
				break;
			}
		}
	}

	return __super::UserLeftDesk(bDeskStation, pUserInfo);
}
//�����ı��û����������ĺ���
bool	CServerGameDesk::CanNetCut(BYTE bDeskStation)
{
	return true;
}
///�û������뿪
bool  CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	return __super::UserNetCut(bDeskStation, pLostUserInfo);
}


///���ƣ�OnGetGameStation
///��������ȡ��Ϸ״̬ 
///@param byDeskStation λ�� uSocketID socket id  bWatchUser �Ƿ������Թ�
///@return 
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	
	// ��֤�����û�Ȩ��
	AuthPermissions(bDeskStation);
	if(m_pUserInfo[bDeskStation] != NULL && m_pUserInfo[bDeskStation]->m_UserData.isVirtual != 0)//��������Ӯ���ƣ���ȡ���������λ��
	{
		m_IsAIStation[bDeskStation] = true;
	}
	//��������Ϸ��ʼ���������״̬û�����õ����������Ϸ�д�Ǯ�����ڴ���������
	if (m_bGameStation ==GS_NOTE_STATE ||m_bGameStation == GS_SEND_CARD||m_bGameStation == GS_OPEN_RESULT)
	{
		m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_PLAY_GAME;//���������Ϊͬ��״̬
	}
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_AGREE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:			//�ȴ���һ����Ϸ��ʼ
		{
			//�������ݷ��͵��ͻ���
			GameStationBase TGameStation;

			TGameStation.m_bGameStation	= m_bGameStation;		//��Ϸ״̬

			TGameStation.m_iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.m_iKaiPaiTime	= m_iKaiPaiTime;		//����ʱ��
			TGameStation.m_iShowResult	= m_iShowResult;		//�������ʾʱ��
			TGameStation.m_iFreeTime	= m_iFreeTime;			//����ʱ��

			TGameStation.m_iZhuangBaShu  = m_iZhuangBaShu;
			TGameStation.m_iNowNtStation = m_iNowNtStation;

			TGameStation.i64NtWin	= m_i64NtWin;					//ׯ����Ӯ
			TGameStation.i64MeWin	= m_i64UserWin[bDeskStation];	//�Լ���Ӯ

			//��ׯ�б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				TGameStation.iZhuangWaitList[i] = m_iZhuangStation[i];
			}

			TGameStation.m_iShangZhuangLimit = m_iShangZhuangLimit;	//��ׯ��Ҫ�����ٽ��
			TGameStation.iMaxNote = CanDouble(bDeskStation)?2*m_iMaxNote:m_iMaxNote;			//��ͨ��������ע

			//·����Ϣ
			for(int i=0;i<MAXCOUNT&&i<m_DqLuziData.size();i++)
			{
				TGameStation.TLuziData[i] = m_DqLuziData.at(i);
			}
										
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			//�����Щ������ׯ�ȴ�ʱ�䣬��ǿ��һ����������ׯ			
			for (int i=0;i<PLAY_COUNT; i++)
			{
				if (NULL == m_pUserInfo[i])			
					continue;				
				if(m_pUserInfo[i]->m_UserData.isVirtual != 0)//��������Ӯ���ƣ���ȡ���������λ��
				{
					m_IsAIStation[i] = true;					
				}
			}
			return true;
		}
	case GS_NOTE_STATE:	//��ע״̬
		{
			GameStation_Bet TGameStation;

			TGameStation.m_bGameStation	= m_bGameStation;		//��Ϸ״̬

			TGameStation.m_iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.m_iKaiPaiTime	= m_iKaiPaiTime;		//����ʱ��
			TGameStation.m_iShowResult	= m_iShowResult;		//�������ʾʱ��
			TGameStation.m_iFreeTime	= m_iFreeTime;			//����ʱ��

			TGameStation.m_iZhuangBaShu  = m_iZhuangBaShu;
			TGameStation.m_iNowNtStation = m_iNowNtStation;

			TGameStation.i64NtWin	= m_i64NtWin;					//ׯ����Ӯ
			TGameStation.i64MeWin	= m_i64UserWin[bDeskStation];	//�Լ���Ӯ

			//��ׯ�б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				TGameStation.iZhuangWaitList[i] = m_iZhuangStation[i];
			}

			TGameStation.m_iShangZhuangLimit = m_iShangZhuangLimit;	//��ׯ��Ҫ�����ٽ��
			TGameStation.iMaxNote = CanDouble(bDeskStation)?2*m_iMaxNote:m_iMaxNote;			//��ͨ��������ע

			//·����Ϣ
			for(int i=0;i<MAXCOUNT&&i<m_DqLuziData.size();i++)
			{
				TGameStation.TLuziData[i] = m_DqLuziData.at(i);
			}
			//��ע���
			for(int i=0; i<BET_ARES; i++)
			{
				TGameStation.i64AresMaxZhu[i]		= m_i64MaxZhu[i];			//����������ע�����
				TGameStation.i64XiaZhuData[i]		= m_iQuYuZhu[i];			//�����������ע
				TGameStation.i64UserXiaZhuData[i]	= m_iUserXiaZhuData[bDeskStation][i];		//����ڸ��������ע���
			}
			TGameStation.i64UserXiaZhuSum	= m_iUserXiaZhuData[bDeskStation][BET_ARES];	//��ҵ�����ע
			TGameStation.i64XiaZhuSum		= m_i64ZhongZhu;	//������ҵ�����ע

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			return true;
		}
	case GS_SEND_CARD:		//����״̬
		{
			GameStation_SendCard TGameStation;

			TGameStation.m_bGameStation	= m_bGameStation;		//��Ϸ״̬

			TGameStation.m_iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.m_iKaiPaiTime	= m_iKaiPaiTime;		//����ʱ��
			TGameStation.m_iShowResult	= m_iShowResult;		//�������ʾʱ��
			TGameStation.m_iFreeTime	= m_iFreeTime;			//����ʱ��

			TGameStation.m_iZhuangBaShu  = m_iZhuangBaShu;
			TGameStation.m_iNowNtStation = m_iNowNtStation;

			TGameStation.i64NtWin	= m_i64NtWin;					//ׯ����Ӯ
			TGameStation.i64MeWin	= m_i64UserWin[bDeskStation];	//�Լ���Ӯ

			//��ׯ�б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				TGameStation.iZhuangWaitList[i] = m_iZhuangStation[i];
			}

			TGameStation.m_iShangZhuangLimit = m_iShangZhuangLimit;	//��ׯ��Ҫ�����ٽ��
			TGameStation.iMaxNote = CanDouble(bDeskStation)?2*m_iMaxNote:m_iMaxNote;			//��ͨ��������ע

			//·����Ϣ
			for(int i=0;i<MAXCOUNT&&i<m_DqLuziData.size();i++)
			{
				TGameStation.TLuziData[i] = m_DqLuziData.at(i);
			}
			//��ע���
			for(int i=0; i<BET_ARES; i++)
			{
				TGameStation.i64AresMaxZhu[i]		= m_i64MaxZhu[i];			//����������ע�����
				TGameStation.i64XiaZhuData[i]		= m_iQuYuZhu[i];			//�����������ע
				TGameStation.i64UserXiaZhuData[i]	= m_iUserXiaZhuData[bDeskStation][i];		//����ڸ��������ע���
			}
			TGameStation.i64UserXiaZhuSum	= m_iUserXiaZhuData[bDeskStation][BET_ARES];	//��ҵ�����ע
			TGameStation.i64XiaZhuSum		= m_i64ZhongZhu;	//������ҵ�����ע

			//������
			for(int i=0; i<2; i++)
			{
				for(int j=0; j<3; j++)
				{
					TGameStation.byUserCard[i][j] = m_byUserCard[i][j];
				}
			}

			//�����ܵ���
			TGameStation.byZPaiXing =  m_iZPaiXing[1];
			TGameStation.byXPaiXing =  m_iXPaiXing[1];
			
			//��ȡӮ������
			for(int i=0;i<BET_ARES;i++)
			{
				TGameStation.byWinAreas[i] = m_iWinQuYu[i];			
			}

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));

			return true;
		}
	case GS_OPEN_RESULT:	//����״̬GameStation_Result
		{
			GameStation_Result TGameStation;

			TGameStation.m_bGameStation	= m_bGameStation;		//��Ϸ״̬

			TGameStation.m_iXiaZhuTime	= m_iXiaZhuTime;		//��עʱ��
			TGameStation.m_iKaiPaiTime	= m_iKaiPaiTime;		//����ʱ��
			TGameStation.m_iShowResult	= m_iShowResult;		//�������ʾʱ��
			TGameStation.m_iFreeTime	= m_iFreeTime;			//����ʱ��

			TGameStation.m_iZhuangBaShu  = m_iZhuangBaShu;
			TGameStation.m_iNowNtStation = m_iNowNtStation;

			TGameStation.i64NtWin	= m_i64NtWin;					//ׯ����Ӯ
			TGameStation.i64MeWin	= m_i64UserWin[bDeskStation];	//�Լ���Ӯ

			//��ׯ�б�
			for(int i=0;i<=PLAY_COUNT;i++)
			{
				TGameStation.iZhuangWaitList[i] = m_iZhuangStation[i];
			}

			TGameStation.m_iShangZhuangLimit = m_iShangZhuangLimit;	//��ׯ��Ҫ�����ٽ��
			TGameStation.iMaxNote = CanDouble(bDeskStation)?2*m_iMaxNote:m_iMaxNote;			//��ͨ��������ע

			//·����Ϣ
			for(int i=0;i<MAXCOUNT&&i<m_DqLuziData.size();i++)
			{
				TGameStation.TLuziData[i] = m_DqLuziData.at(i);
			}
			//��ע���
			for(int i=0; i<BET_ARES; i++)
			{
				TGameStation.i64AresMaxZhu[i]		= m_i64MaxZhu[i];			//����������ע�����
				TGameStation.i64XiaZhuData[i]		= m_iQuYuZhu[i];			//�����������ע
				TGameStation.i64UserXiaZhuData[i]	= m_iUserXiaZhuData[bDeskStation][i];		//����ڸ��������ע���
			}
			TGameStation.i64UserXiaZhuSum	= m_iUserXiaZhuData[bDeskStation][BET_ARES];	//��ҵ�����ע
			TGameStation.i64XiaZhuSum		= m_i64ZhongZhu;	//������ҵ�����ע		

			TGameStation.i64ZhuangScore	= m_i64UserFen[m_iNowNtStation];		//ׯ�ұ��ֵĵ÷�
			TGameStation.i64MeScore		= m_i64UserFen[bDeskStation];			//�Լ����ֵĵ÷�
			TGameStation.i64MeFanFen	= GetMeFanFen(bDeskStation);			//������ҷ����ķ֣�����ʱ����
			
			
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&TGameStation,sizeof(TGameStation));
			return true;
		}
	default:
		break;
	}

	ASSERT(CHECK_DEBUG);
	return true;
}

///���ƣ�ReSetGameState
///������������Ϸ״̬ 
///@param bLastStation ״̬
///@return 
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_wSuperCardList[i],0,sizeof(m_wSuperCardList[i]));
	}
	m_bySuperStation = -1;
#endif
	ASSERT(CHECK_DEBUG);
	return true;
}





/// ������ж�ʱ��
void  CServerGameDesk::KillAllTimer(void)
{
	
}

///���ƣ�OnTimer
///��������ʱ����Ϣ 
///@param uTimerID ��ʱ��id
///@return 
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch (uTimerID)
	{
	case ID_TIMER_SEND_CARD://��עʱ��
		{
			//���Ϳ�����Ϣ
			OnSendOpenCard();
			KillTimer(ID_TIMER_SEND_CARD);
			return true;
		}
	case ID_TIMER_SHOW_RESULT:
		{
			//��Ϸ����
			OnSendGameResult();
			KillTimer(ID_TIMER_SHOW_RESULT);
			return true;
		}
	case ID_TIMER_SHOW_FINISH:	//�������ʾ���� �ͽ�����Ϸ
		{
			OnSenFree();
			KillTimer(ID_TIMER_SHOW_FINISH);
			return true;
		}	
	case ID_TIMER_GAME_BEGIN:		// ����ʱ�������Ϸ��ʼ
		{
			//��Ϸ���¿�ʼ
			RestartGame();
			KillTimer(ID_TIMER_GAME_BEGIN);
			return true;
		}
	}
	return OnTimer(uTimerID);
}

/*----------------------------------------------------------------------------*/
//���Ϳ�����Ϣ
void CServerGameDesk::OnSendOpenCard()
{
	//���Ϳ�����Ϣ			
	m_bGameStation = GS_SEND_CARD;         // ��Ϸһ����״̬��
	m_bCanNote = false;

	//����
	SendCard();	
	//�����Ƿ������
	ConTrolZX();

	KAI_PAI KaiPaiData;
	//������
	KaiPaiData.pai[0][0] = m_byUserCard[0][0];
	KaiPaiData.pai[0][1] = m_byUserCard[0][1];
	KaiPaiData.pai[0][2] = m_byUserCard[0][2];

	KaiPaiData.pai[1][0] = m_byUserCard[1][0];
	KaiPaiData.pai[1][1] = m_byUserCard[1][1];
	KaiPaiData.pai[1][2] = m_byUserCard[1][2];

	/**< ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��*/
	for(int i=0;i<5;i++)
	{
		KaiPaiData.iZPaiXing[i] =  m_iZPaiXing[i];
		KaiPaiData.iXPaiXing[i] =  m_iXPaiXing[i];
	}
	//��ȡӮ������
	for(int i=0;i<BET_ARES;i++)
	{
		KaiPaiData.iWinQuYu[i] = m_iWinQuYu[i];			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	}
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL != m_pUserInfo[i])
		{
			SendGameData(i, &KaiPaiData, sizeof(KaiPaiData), MDM_GM_GAME_NOTIFY, ASS_KAI_PAI, 0);//���Ϳ�����Ϣ
		}
	}
	//�������ƽ���ʱ��
	SetTimer(ID_TIMER_SHOW_RESULT,(m_iKaiPaiTime+1)*1000);
}
/*----------------------------------------------------------------------------*/
//��Ϸ����
void	CServerGameDesk::OnSendGameResult()
{
	m_bGameStation = GS_OPEN_RESULT;
	
	if(m_iNowNtStation>=0)///û��ׯ�ҾͲ����
	{
		CountFen();
	}	

	bool temp_cut[PLAY_COUNT];
	__int64  i64UserMoney[PLAY_COUNT];			/**< ������ұ���Ӧ�Ľ��*/
	memset(&temp_cut, 0, sizeof(temp_cut));		//ׯ���б��ܵ�ׯ��λ��
	memset(&i64UserMoney, 0, sizeof(i64UserMoney)); //��ҽ��

	////д�����ݿ�
	if(m_iNowNtStation >= 0)	///û��ׯ�ҾͲ����
	{
		///2010-9-27 zht �޸�̨�ѿ۷����⣬û��ע���˵�ׯʱ�����˶�����ע����̨��
		CGameUserInfo	* pUserInfoTemp[MAX_PEOPLE];
		memcpy(pUserInfoTemp,m_pUserInfo, sizeof(m_pUserInfo));
		BOOL bHaveNote = FALSE; //������ע��
		for (int i=0; i<PLAY_COUNT && i<MAX_PEOPLE; i++)
		{
			if (m_iUserXiaZhuData[i][BET_ARES] == 0 && m_iNowNtStation != i)
			{
				//m_pUserInfo[i] = NULL;
			}
			else if(!bHaveNote && m_iNowNtStation != i)
			{
				bHaveNote = TRUE;  
			}
		}
		
		if(m_byNtTax==0)//ֻ��ׯӮ��ˮ
		{
			ChangeUserPointint64(m_i64UserFen, temp_cut,m_iNowNtStation);
		}
		else//������Ӯ�ҵ�ˮ
		{
			ChangeUserPointint64(m_i64UserFen, temp_cut);
		}
		__super::RecoderGameInfo(i64UserMoney);
	}
	
	//��¼����ҵ���Ӯ���
	for (int i=0;i<PLAY_COUNT; i++)
	{
		m_i64UserWin[i] += i64UserMoney[i];
	}
	m_i64NtWin += m_i64UserFen[m_iNowNtStation]; //ׯ����Ӯ���

	//��¼·��
	LuziData TLuziTmp;
	//��¼ׯ�и�����
	TLuziTmp.byZPoint = m_iZPaiXing[1];
	TLuziTmp.byXpoint = m_iXPaiXing[1];
	//��¼Ӯ������

	if (m_iWinQuYu[0] > 0)	// ׯ��Ӯ��
	{
		TLuziTmp.byWinResult = 0;
	}
	else if (m_iWinQuYu[3]>0)	//�м�Ӯ��
	{
		TLuziTmp.byWinResult = 1;
	}
	else if (m_iWinQuYu[6] > 0)	//����
	{
		TLuziTmp.byWinResult = 2;
	}

	if (m_DqLuziData.size()<MAXCOUNT)
	{
		m_DqLuziData.push_back(TLuziTmp);
	}
	else
	{
		m_DqLuziData.pop_front();
		m_DqLuziData.push_back(TLuziTmp);
	}

	
	JIE_SUAN	TJieSuanData;						//�������ݰ�
	TJieSuanData.i64NtWin		= m_i64NtWin;				//��ǰׯ��Ӯ�Ľ�ң��ɼ���
	TJieSuanData.i64ZhuangScore	= m_i64UserFen[m_iNowNtStation];		//ׯ�ұ��ֵĵ÷�


	for(int i=0;i<m_DqLuziData.size()&&i<MAXCOUNT;i++)
	{
		TJieSuanData.TLuziData[i] = m_DqLuziData.at(i);
	}
	
	for (int i=0;i<PLAY_COUNT; i++)
	{	
		if (NULL != m_pUserInfo[i])
		{
			//��ȡ�ҵķ�����
			TJieSuanData.i64MeFanFen = GetMeFanFen(i);			//������ҷ����ķ֣�����ʱ����				
			TJieSuanData.i64MeScore	 = m_i64UserFen[i];			//����Լ��ĵ÷�					
			TJieSuanData.i64UserWin	 = m_i64UserWin[i];
			TJieSuanData.iPlayCount	 = m_iUserPlayCount[i][0];		//���˶�����
			TJieSuanData.iWinCount	 = m_iUserPlayCount[i][1];			//Ӯ�˶�����
		
			SendGameData(i, &TJieSuanData, sizeof(TJieSuanData), MDM_GM_GAME_NOTIFY, ASS_SHOW_JS, 0);//���ͽ�����Ϣ				
		}
	}

	//������Ϸ������ʱ��
	SetTimer(ID_TIMER_SHOW_FINISH,(m_iShowResult+1)*1000);
}
/*----------------------------------------------------------------------------*/
//���Ϳ�����Ϣ
void	CServerGameDesk::OnSenFree()
{
	GameFinish(0,GF_NORMAL);				//����������Ϸ�����ͽ�����Ϣ
	//����ׯ��
	ClearNt();
	//������ׯ�б�
	MoveData(0);
	if(-1==m_iNowNtStation || NULL==m_pUserInfo[m_iNowNtStation])
	{
		GetNextNT();
		if(m_pUserInfo[m_iNowNtStation])
		{
			//���ͻ�ׯ��Ϣ
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL != m_pUserInfo[i])
				{
					SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_CHANGE_NT,0);
				}
			}
		}
	}

	DENG_DAI Wait;
	for(int i=0;i<=PLAY_COUNT;i++)
	{
		Wait.zhuangstation[i] = m_iZhuangStation[i];
	}
	Wait.iNtStation = m_iNowNtStation;
	Wait.i64NtWin = m_i64NtWin;				//��ǰׯ��Ӯ�Ľ��
	Wait.iZhuangBaShu = m_iZhuangBaShu;	//ׯ�ҽ����˼���
	//����׼����ʼ��Ϣ
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
		{			
			continue;
		}	
		SendGameData(i, &Wait, sizeof(Wait), MDM_GM_GAME_NOTIFY, ASS_FREE_STATE,0);//���Ϳ���ʱ����Ϣ
	}

	//�������м�ʱ��ID_TIMER_GAME_BEGIN
	//������Ϸ������ʱ��
	SetTimer(ID_TIMER_GAME_BEGIN,(m_iFreeTime+1)*1000);
}
//��Ϸ���¿�ʼ
void CServerGameDesk::RestartGame()
{
	m_bGameStation = GS_WAIT_NEXT;
	//����ׯ��
	ClearNt();
	//������ׯ�б�
	MoveData(0);
	if(NULL==m_pUserInfo[m_iNowNtStation])//��ǰׯ�Ҳ�����
	{
		GetNextNT();
		if(m_pUserInfo[m_iNowNtStation])
		{
			//���ͻ�ׯ��Ϣ
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(NULL != m_pUserInfo[i])
				{
					SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_CHANGE_NT,0);
				}
			}
		}
	}
	if(m_pUserInfo[m_iNowNtStation] && m_iNowNtStation != -1)
	{
		StartGame();//��ʼ��Ϸ
	}
	else
	{
		//������ׯ�ȴ���Ϣ			
		tagNoZhuang TNoZhuang;
		TNoZhuang.iNtStation = -1;
		TNoZhuang.i64NtWin = 0;				//��ǰׯ��Ӯ�Ľ��
		TNoZhuang.iZhuangBaShu = 0;			//ׯ�ҽ����˼���
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL != m_pUserInfo[i])
			{
				SendGameData(i,&TNoZhuang,sizeof(TNoZhuang),MDM_GM_GAME_NOTIFY, ASS_WU_ZHUANG,0);////������ׯ�ȴ���Ϣ
			}
		}
	}
}



///���ƣ�MoveData
///���������������е����ݣ������õ�����Ԫ����� 
///@param type ���� 0Ϊ��ׯ���б������1Ϊ�Խ������ݲ���
///@return 
void CServerGameDesk::MoveData(int type)
{
	if(type==0)//ׯ���б������ƶ�
	{  
		int num=0;
		int iNtList[PLAY_COUNT+1]; 
		memset(iNtList,-1,sizeof(iNtList));
		for(int i=0;i<PLAY_COUNT;i++)
		{	

			if (NULL != m_pUserInfo[m_iZhuangStation[i]] && m_pUserInfo[m_iZhuangStation[i]]->m_UserData.i64Money >= m_iShangZhuangLimit)//����һ�����Ϸ��
			{
				iNtList[num]=m_iZhuangStation[i];
				num++;
			}
		}
		iNtList[PLAY_COUNT]=num;
		memset(m_iZhuangStation,-1,sizeof(m_iZhuangStation));
		memcpy(m_iZhuangStation,iNtList,sizeof(m_iZhuangStation));
		
	}
}
///���ƣ�GetNextNT
///������ׯ����ׯ���ȡ��һ��ׯ���б��е�ׯ��
///@param 
///@return true �õ���һ��ׯ�� false û��ׯ����
bool CServerGameDesk::GetNextNT()
{
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iZhuangStation[i]>=0 && NULL!=m_pUserInfo[m_iZhuangStation[i]])
		{
			m_iNowNtStation = m_iZhuangStation[i];
			m_iZhuangStation[i]=-1;
			//������ׯ�б�
			MoveData(0);
			return true;
		}
	}
	return false;
}
///���ƣ�HanleXiaZhu
///������������ҵ���ע
///@param Station λ�� pData ���ݰ� uSize ������
///@return 
void CServerGameDesk::HanleXiaZhu(BYTE Station,void * pData, UINT uSize)
{
	OutputDebugString("dwjbjl::HanleXiaZhu(0)");
	XIA_ZHU *pdata = (XIA_ZHU *)pData;
	if (NULL == pdata)
	{
		return;
	}
	
	m_IsAIStation[Station] = false;/// ��¼�����˵�λ�� 
	if(pdata->m_bIsAI)
	{	
		m_IsAIStation[Station] = true;/// ��¼�����˵�λ�� 
	}
	OutputDebugString("dwjbjl::HanleXiaZhu(1)");
	if (m_iNowNtStation<0 || m_iNowNtStation >=PLAY_COUNT || NULL == m_pUserInfo[m_iNowNtStation])
		return;
	OutputDebugString("dwjbjl::HanleXiaZhu(2)");
	bool sucess=false;//�Ƿ��ܳɹ���ע
	__int64 money=0;
	__int64 temp=0;//�м����
	BYTE ran = 1;//���λ��ģ
	money = m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;//lym0512��ȡׯ�ҵĽ���� 
	
	if((m_iUserXiaZhuData[Station][8] + G_iChouMaValues[pdata->moneytype]) > m_pUserInfo[Station]->m_UserData.i64Money) //��ҵ���ע�ܶ������ҵĽ�����Ͳ�����ע
	{
		return;
	}
	OutputDebugString("dwjbjl::HanleXiaZhu(3)");
	if (CanDouble(Station))
	{
		if( (m_iUserXiaZhuData[Station][BET_ARES]+ G_iChouMaValues[pdata->moneytype])>2*m_iMaxNote && m_iMaxNote>0) //������ע�Ͳ�����
		{	
			return;
		}
	}
	else
	{
		if( (m_iUserXiaZhuData[Station][BET_ARES]+ G_iChouMaValues[pdata->moneytype])>m_iMaxNote && m_iMaxNote>0) //������ע�Ͳ�����
		{
			return;
		}
	}	
	OutputDebugString("dwjbjl::HanleXiaZhu(4)");
	long tempMoney = abs((long)(m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]));//ׯ����λ֮��
	switch(pdata->type)//��ע����
	{
	case GF_ZHUANG:		// ׯ
		{	
			if(m_iMax[GF_ZHUANG] > 0 && (m_iQuYuZhu[GF_ZHUANG] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_ZHUANG] )//��ע���ܳ������ƵĶ��
			{
				return;
			}
			temp = money - m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_Z_TIANWANG]*2 - m_iQuYuZhu[GF_Z_DUIZI]*11 - G_iChouMaValues[pdata->moneytype];//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 - m_iQuYuZhu[GF_XIAN]);
			if(temp >= 0 && m_iQuYuZhu[GF_ZHUANG] < (100000000000/2))
			{
				m_i64MaxZhu[GF_ZHUANG] = temp;   
				sucess = true;
			}
		}
		break;
	case GF_Z_TIANWANG:	// ׯ����
		{
			if(m_iMax[GF_Z_TIANWANG] > 0 && (m_iQuYuZhu[GF_Z_TIANWANG] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_Z_TIANWANG] )//��ע���ܳ������ƵĶ��
			{
				return;
			}
			temp = money - tempMoney/*m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]���Գ�ׯ���У���������ֻҪ������֮��Ϳ���*/ - m_iQuYuZhu[GF_Z_TIANWANG]*2 - m_iQuYuZhu[GF_Z_DUIZI]*11 - G_iChouMaValues[pdata->moneytype]*2;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 + m_iQuYuZhu[GF_HE]*8 + m_iQuYuZhu[GF_T_HE]*32);			
			if(temp >= 0 && m_iQuYuZhu[GF_Z_TIANWANG] < (100000000000/3))//Ϊ�˷�ֹ�������ע*���ʲ��ܴ���20��
			{
				m_i64MaxZhu[GF_Z_TIANWANG] = temp;   
				m_i64MaxZhu[GF_ZHUANG] = temp;   
				sucess = true;
			}
		}
		break;
	case GF_Z_DUIZI:	// ׯ����
		{	
			if(m_iMax[GF_Z_DUIZI] > 0 && (m_iQuYuZhu[GF_Z_DUIZI] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_Z_DUIZI])//��ע���ܳ������ƵĶ��
			{
				OutputDebugString("�����ˣ����������ܾ���ע  ���� 222 ");
				return;
			}

			temp = money - tempMoney/*m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]���Գ�ׯ���У���������ֻҪ������֮��Ϳ���*/ - m_iQuYuZhu[GF_Z_TIANWANG]*2 - m_iQuYuZhu[GF_Z_DUIZI]*11 - G_iChouMaValues[pdata->moneytype]*11;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 + m_iQuYuZhu[GF_HE]*8 + m_iQuYuZhu[GF_T_HE]*32);			
			if(temp >= 0 && m_iQuYuZhu[GF_Z_DUIZI] < (100000000000/12))
			{
				m_i64MaxZhu[GF_Z_DUIZI] = temp;   
				m_i64MaxZhu[GF_ZHUANG] = temp;   
				sucess = true;
			}
		}
		break;
	case GF_XIAN:		// ��
		{
			if(m_iMax[GF_XIAN] > 0 && (m_iQuYuZhu[GF_XIAN] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_XIAN])//��ע���ܳ������ƵĶ��
			{	
				return;
			}
			temp  = money - m_iQuYuZhu[GF_XIAN] - m_iQuYuZhu[GF_X_TIANWANG]*2 - m_iQuYuZhu[GF_X_DUIZI]*11 - G_iChouMaValues[pdata->moneytype];//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 - m_iQuYuZhu[GF_ZHUANG]);//���������������ע			
			if(temp >= 0 && m_iQuYuZhu[GF_XIAN]<(100000000000/2))
			{
				m_i64MaxZhu[GF_XIAN] = temp;   
				sucess = true;
			}
		}
		break;
	case GF_X_TIANWANG:	// ������
		{
			if(m_iMax[GF_X_TIANWANG] > 0 && (m_iQuYuZhu[GF_X_TIANWANG] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_X_TIANWANG])//��ע���ܳ������ƵĶ��
			{
				return;
			}
			temp  = money - tempMoney/*m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]���Գ�ׯ���У���������ֻҪ������֮��Ϳ���*/ - m_iQuYuZhu[GF_X_TIANWANG]*2 - m_iQuYuZhu[GF_X_DUIZI]*11 - G_iChouMaValues[pdata->moneytype]*2;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_HE]*8 + m_iQuYuZhu[GF_T_HE]*32);
			if(temp >= 0 && m_iQuYuZhu[GF_X_TIANWANG] < (100000000000/3))
			{
				m_i64MaxZhu[GF_X_TIANWANG] = temp;   
				m_i64MaxZhu[GF_XIAN] = temp;   
				sucess = true;
			}
		}
		break;
	case GF_X_DUIZI:	// �ж���
		{
			if(m_iMax[GF_X_DUIZI] > 0 && (m_iQuYuZhu[GF_X_DUIZI] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_X_DUIZI])//��ע���ܳ������ƵĶ��
			{
				return;
			}
			temp = money - tempMoney/*m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_XIAN]���Գ�ׯ���У���������ֻҪ������֮��Ϳ���*/ - m_iQuYuZhu[GF_X_TIANWANG]*2 - m_iQuYuZhu[GF_X_DUIZI]*11 - G_iChouMaValues[pdata->moneytype]*11;//����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_HE]*8 + m_iQuYuZhu[GF_T_HE]*32);			
			if(temp >= 0 && m_iQuYuZhu[GF_X_DUIZI] < (100000000000/(__int64)12))
			{
				m_i64MaxZhu[GF_X_DUIZI] = temp;   
				m_i64MaxZhu[GF_XIAN] = temp;   
				sucess = true;
			}
		}
		break;
	case GF_HE:			// ��
		{
			if(m_iMax[GF_HE] > 0 && (m_iQuYuZhu[GF_HE] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_HE])//��ע���ܳ������ƵĶ��
			{
				return;
			}
			temp = money - m_iQuYuZhu[GF_HE]*8 - m_iQuYuZhu[GF_T_HE]*32 - G_iChouMaValues[pdata->moneytype]*8;     //����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11);//���������������ע
			if(temp >= 0 && m_iQuYuZhu[GF_HE] < (100000000000/(__int64)9))
			{
				m_i64MaxZhu[GF_HE] = temp/8;   
				sucess = true;
			}
		}
		break;
	case GF_T_HE:		// ͬ���
		{
			if(m_iMax[GF_T_HE] > 0 && (m_iQuYuZhu[GF_T_HE] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_T_HE] )//��ע���ܳ������ƵĶ��
			{
				return;
			}
			temp  = money - m_iQuYuZhu[GF_HE]*8 - m_iQuYuZhu[GF_T_HE]*32 - G_iChouMaValues[pdata->moneytype]*32;     //����ÿ������������µ�ע��
			temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11);//���������������ע

			if(temp >= 0 && m_iQuYuZhu[GF_T_HE] < (100000000000/(__int64)33))
			{
				m_i64MaxZhu[GF_T_HE] = temp/8;
				m_i64MaxZhu[GF_HE] = temp/8;   
				sucess = true;
			}
		}
		break;
	case GF_Z_DAN:	/**<  ׯ��   */
		{
			if(m_iMax[GF_Z_DAN] > 0 && (m_iQuYuZhu[GF_Z_DAN] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_Z_DAN] )//��ע���ܳ������ƵĶ��
			{
				return;
			}

			__int64 i64Tmp = m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 ;

			if ((m_iQuYuZhu[GF_HE]*9)>(m_iQuYuZhu[GF_ZHUANG]*2) && (m_iQuYuZhu[GF_HE]*9)>(m_iQuYuZhu[GF_XIAN]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_Z_DAN]-m_iQuYuZhu[GF_HE]*9-m_iQuYuZhu[GF_T_HE]*32+m_iQuYuZhu[GF_ZHUANG]+m_iQuYuZhu[GF_XIAN]+m_iQuYuZhu[GF_Z_SHUANG]-abs(m_iQuYuZhu[GF_X_DAN] - m_iQuYuZhu[GF_X_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}
			else if ((m_iQuYuZhu[GF_ZHUANG]*2)>(m_iQuYuZhu[GF_HE]*9) && (m_iQuYuZhu[GF_ZHUANG]*2)>(m_iQuYuZhu[GF_XIAN]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_Z_DAN]-m_iQuYuZhu[GF_ZHUANG]*2+m_iQuYuZhu[GF_HE]+m_iQuYuZhu[GF_XIAN]+m_iQuYuZhu[GF_Z_SHUANG]-abs(m_iQuYuZhu[GF_X_DAN] - m_iQuYuZhu[GF_X_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}
			else if ((m_iQuYuZhu[GF_XIAN]*2)>(m_iQuYuZhu[GF_HE]*9) && (m_iQuYuZhu[GF_XIAN]*2)>(m_iQuYuZhu[GF_ZHUANG]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_Z_DAN]-m_iQuYuZhu[GF_XIAN]*2+m_iQuYuZhu[GF_HE]+m_iQuYuZhu[GF_ZHUANG]+m_iQuYuZhu[GF_Z_SHUANG]-abs(m_iQuYuZhu[GF_X_DAN] - m_iQuYuZhu[GF_X_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}

			if(temp >= 0 && m_iQuYuZhu[GF_Z_DAN] < (100000000000/2))
			{
				m_i64MaxZhu[GF_Z_DAN] = temp;   
				sucess = true;
			}

			break;
		}
	case GF_Z_SHUANG:			   /**<  ׯ˫   */
		{
			if(m_iMax[GF_Z_SHUANG] > 0 && (m_iQuYuZhu[GF_Z_SHUANG] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_Z_SHUANG] )//��ע���ܳ������ƵĶ��
			{
				return;
			}
			__int64 i64Tmp = m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 ;

			if ((m_iQuYuZhu[GF_HE]*9)>(m_iQuYuZhu[GF_ZHUANG]*2) && (m_iQuYuZhu[GF_HE]*9)>(m_iQuYuZhu[GF_XIAN]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_Z_SHUANG]-m_iQuYuZhu[GF_HE]*9-m_iQuYuZhu[GF_T_HE]*32+m_iQuYuZhu[GF_ZHUANG]+m_iQuYuZhu[GF_XIAN]+m_iQuYuZhu[GF_Z_DAN]-abs(m_iQuYuZhu[GF_X_DAN] - m_iQuYuZhu[GF_X_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}
			else if ((m_iQuYuZhu[GF_ZHUANG]*2)>(m_iQuYuZhu[GF_HE]*9) && (m_iQuYuZhu[GF_ZHUANG]*2)>(m_iQuYuZhu[GF_XIAN]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_Z_SHUANG]-m_iQuYuZhu[GF_ZHUANG]*2+m_iQuYuZhu[GF_HE]+m_iQuYuZhu[GF_XIAN]+m_iQuYuZhu[GF_Z_DAN]-abs(m_iQuYuZhu[GF_X_DAN] - m_iQuYuZhu[GF_X_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}
			else if ((m_iQuYuZhu[GF_XIAN]*2)>(m_iQuYuZhu[GF_HE]*9) && (m_iQuYuZhu[GF_XIAN]*2)>(m_iQuYuZhu[GF_ZHUANG]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_Z_SHUANG]-m_iQuYuZhu[GF_XIAN]*2+m_iQuYuZhu[GF_HE]+m_iQuYuZhu[GF_ZHUANG]+m_iQuYuZhu[GF_Z_DAN]-abs(m_iQuYuZhu[GF_X_DAN] - m_iQuYuZhu[GF_X_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}

			if(temp >= 0 && m_iQuYuZhu[GF_Z_SHUANG] < (100000000000/2))
			{
				m_i64MaxZhu[GF_Z_SHUANG] = temp;   
				sucess = true;
			}
			break;
		}
	case GF_X_DAN:					/**<	 �е�   */
		{
			if(m_iMax[GF_X_DAN] > 0 && (m_iQuYuZhu[GF_X_DAN] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_X_DAN] )//��ע���ܳ������ƵĶ��
			{
				return;
			}
			__int64 i64Tmp = m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 ;

			if ((m_iQuYuZhu[GF_HE]*9)>(m_iQuYuZhu[GF_ZHUANG]*2) && (m_iQuYuZhu[GF_HE]*9)>(m_iQuYuZhu[GF_XIAN]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_X_DAN]-m_iQuYuZhu[GF_HE]*9-m_iQuYuZhu[GF_T_HE]*32+m_iQuYuZhu[GF_ZHUANG]+m_iQuYuZhu[GF_XIAN]+m_iQuYuZhu[GF_X_SHUANG]-abs(m_iQuYuZhu[GF_Z_DAN] - m_iQuYuZhu[GF_Z_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}
			else if ((m_iQuYuZhu[GF_ZHUANG]*2)>(m_iQuYuZhu[GF_HE]*9) && (m_iQuYuZhu[GF_ZHUANG]*2)>(m_iQuYuZhu[GF_XIAN]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_X_DAN]-m_iQuYuZhu[GF_ZHUANG]*2+m_iQuYuZhu[GF_HE]+m_iQuYuZhu[GF_XIAN]+m_iQuYuZhu[GF_X_SHUANG]-abs(m_iQuYuZhu[GF_Z_DAN] - m_iQuYuZhu[GF_Z_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}
			else if ((m_iQuYuZhu[GF_XIAN]*2)>(m_iQuYuZhu[GF_HE]*9) && (m_iQuYuZhu[GF_XIAN]*2)>(m_iQuYuZhu[GF_ZHUANG]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_X_DAN]-m_iQuYuZhu[GF_XIAN]*2+m_iQuYuZhu[GF_HE]+m_iQuYuZhu[GF_ZHUANG]+m_iQuYuZhu[GF_X_SHUANG]-abs(m_iQuYuZhu[GF_Z_DAN] - m_iQuYuZhu[GF_Z_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}

			if(temp >= 0 && m_iQuYuZhu[GF_X_DAN] < (100000000000/2))
			{
				m_i64MaxZhu[GF_X_DAN] = temp;   
				sucess = true;
			}
			break;
		}
	case GF_X_SHUANG:			   /**<	 ��˫   */
		{
			if(m_iMax[GF_X_SHUANG] > 0 && (m_iQuYuZhu[GF_X_SHUANG] + G_iChouMaValues[pdata->moneytype]) > m_iMax[GF_X_SHUANG] )//��ע���ܳ������ƵĶ��
			{
				return;
			}
			__int64 i64Tmp = m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11 + m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11 ;

			if ((m_iQuYuZhu[GF_HE]*9)>(m_iQuYuZhu[GF_ZHUANG]*2) && (m_iQuYuZhu[GF_HE]*9)>(m_iQuYuZhu[GF_XIAN]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_X_SHUANG]-m_iQuYuZhu[GF_HE]*9-m_iQuYuZhu[GF_T_HE]*32+m_iQuYuZhu[GF_ZHUANG]+m_iQuYuZhu[GF_XIAN]+m_iQuYuZhu[GF_X_DAN]-abs(m_iQuYuZhu[GF_Z_DAN] - m_iQuYuZhu[GF_Z_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}
			else if ((m_iQuYuZhu[GF_ZHUANG]*2)>(m_iQuYuZhu[GF_HE]*9) && (m_iQuYuZhu[GF_ZHUANG]*2)>(m_iQuYuZhu[GF_XIAN]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_X_SHUANG]-m_iQuYuZhu[GF_ZHUANG]*2+m_iQuYuZhu[GF_HE]+m_iQuYuZhu[GF_XIAN]+m_iQuYuZhu[GF_X_DAN]-abs(m_iQuYuZhu[GF_Z_DAN] - m_iQuYuZhu[GF_Z_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}
			else if ((m_iQuYuZhu[GF_XIAN]*2)>(m_iQuYuZhu[GF_HE]*9) && (m_iQuYuZhu[GF_XIAN]*2)>(m_iQuYuZhu[GF_ZHUANG]*2))
			{
				temp = money - i64Tmp-m_iQuYuZhu[GF_X_SHUANG]-m_iQuYuZhu[GF_XIAN]*2+m_iQuYuZhu[GF_HE]+m_iQuYuZhu[GF_ZHUANG]+m_iQuYuZhu[GF_X_DAN]-abs(m_iQuYuZhu[GF_Z_DAN] - m_iQuYuZhu[GF_Z_SHUANG])- G_iChouMaValues[pdata->moneytype];
			}

			if(temp >= 0 && m_iQuYuZhu[GF_X_SHUANG] < (100000000000/2))
			{
				m_i64MaxZhu[GF_X_SHUANG] = temp;   
				sucess = true;
			}
			break;
		}
	default:
		break;
	}
	if(sucess)//��ע�ɹ�
	{
		m_iQuYuZhu[pdata->type] += G_iChouMaValues[pdata->moneytype];							//��������ע������
		m_iUserXiaZhuData[Station][pdata->type] += G_iChouMaValues[pdata->moneytype];			//����ڱ�������ע������
		m_iUserXiaZhuData[Station][BET_ARES] += G_iChouMaValues[pdata->moneytype];				//�����ע�ܶ�����
		
		//����ǻ�������ע
		if(pdata->m_bIsAI)
		{
			m_iAIMoney[pdata->type] += G_iChouMaValues[pdata->moneytype];		//��������ע������
		}

		//�����ע�ɹ��󣬸��¿ͻ������Ͻǵ�ׯ���С��Ϳ���ע��
		int temp = 0;
		temp = money - m_iQuYuZhu[GF_ZHUANG] - m_iQuYuZhu[GF_Z_TIANWANG]*2 - m_iQuYuZhu[GF_Z_DUIZI]*11; //����ׯ����������µ�ע��
		temp += m_iQuYuZhu[GF_XIAN];		//���������������ע
		temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11); 
		if(temp >= 0 && m_iQuYuZhu[GF_ZHUANG] < (100000000000/(__int64)2))				//ׯ��ע�������ע�����
		{
			m_i64MaxZhu[GF_ZHUANG] = temp;   			
		}	

		temp  = money - m_iQuYuZhu[GF_XIAN] - m_iQuYuZhu[GF_X_TIANWANG]*2 - m_iQuYuZhu[GF_X_DUIZI]*11;	//����������������µ�ע��
		temp += m_iQuYuZhu[GF_ZHUANG];		//���������������ע
		temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11);
		if(temp >= 0 && m_iQuYuZhu[GF_XIAN] < (100000000000/(__int64)2))				//����ע�������ע�����
		{
			m_i64MaxZhu[GF_XIAN] = temp;   			
		}

		temp = money - m_iQuYuZhu[GF_HE]*8 - m_iQuYuZhu[GF_T_HE]*32;		//����ÿ������������µ�ע��	
		temp -= (m_iQuYuZhu[GF_X_TIANWANG]*2 + m_iQuYuZhu[GF_X_DUIZI]*11);
		temp -= (m_iQuYuZhu[GF_Z_TIANWANG]*2 + m_iQuYuZhu[GF_Z_DUIZI]*11);
		if(temp >= 0 && m_iQuYuZhu[GF_HE] < (100000000000/(__int64)9))					//����ע�������ע�����
		{
			m_i64MaxZhu[GF_HE] = temp/8; 
		}

		m_i64ZhongZhu += G_iChouMaValues[pdata->moneytype];		//��ǰ����ע������
		//���ͳɹ���Ϣ
		XIA_ZHU xiazhu;
		xiazhu.money	= G_iChouMaValues[pdata->moneytype];
		xiazhu.station	= Station;
		xiazhu.type		= pdata->type;
		xiazhu.iAllZhongZhu = m_i64ZhongZhu;
		for(int i=0; i<BET_ARES; i++)
		{
			xiazhu.m_iMaxZhu[i]		= m_i64MaxZhu[i];			//������������ע
			xiazhu.m_iQuYuZhu[i]	= m_iQuYuZhu[i];		//�������������ע
			xiazhu.m_iAIMoney[i]	= m_iAIMoney[i];		/**< �����˵���ע*/
			xiazhu.m_iUserXiaZhu[i] = m_iUserXiaZhuData[Station][i];   //��ҵ�ǰ��ÿ�������µ�ע��
		}
		for (int i=0; i<PLAY_COUNT; i++)
		{
			if (NULL == m_pUserInfo[i])
			{
				continue;
			}
		
			xiazhu.iMeZhongZhu = m_iUserXiaZhuData[i][BET_ARES];
			SendGameData(i,&xiazhu,sizeof(xiazhu),MDM_GM_GAME_NOTIFY, ASS_XIA_ZHU,0);//���ͳɹ���Ϣ
		}
	}
}

///���ƣ�HanleSzhuang
///������������ҵ���ׯ��Ϣ
///@param Station λ�� pData ���ݰ� uSize ������
///@return 
void	CServerGameDesk::HanleSzhuang(BYTE Station,void * pData, UINT uSize)
{
	SHANG_ZHUANG *pdata = (SHANG_ZHUANG *)pData;
	int sucess=2;//�Ƿ��ܳɹ�


	if(m_pUserInfo[Station] == NULL)
		return;
	m_IsAIStation[Station] = false;/// ��¼�����˵�λ�� 
	if(pdata->isAI)
	{
		m_IsAIStation[Station] = true;/// ��¼�����˵�λ�� 		
	}
	MoveData(0);//��������
	//ׯ�Ҳ�����������ׯ
	if(m_iNowNtStation == Station)
	{
		return;
	}
	if(m_iNowNtStation<0)//û��ׯ��
	{
		m_iNowNtStation = Station;
		long iNTUserID = 0;
		if (m_iNowNtStation>=0)
		{
			iNTUserID = m_pUserInfo[m_iNowNtStation]->m_UserData.dwUserID;		
		}
		SetCurrentZhuangjia(iNTUserID);
		if(m_pUserInfo[m_iNowNtStation])
		{
			StartGame();//��ʼ��Ϸ
		}
		sucess = 0;
	}
	else
	{
		bool bIsList=false;
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_iZhuangStation[i] == Station)//���б��е���Ҳ�������
			{
				sucess = 2;
				bIsList=true;
				break;
			}
		}
		if(!bIsList)
		{
			for(int i=0;i<PLAY_COUNT;i++)
			{
				if(m_iZhuangStation[i]<0)
				{
					m_iZhuangStation[i]=Station;
					sucess = 0;
					break;
				}
			}
		}
	}



	//������ׯ��Ϣ
	SHANG_ZHUANG Tszhuang;
	Tszhuang.shang = true;
	Tszhuang.station = Station;
	Tszhuang.success = sucess;
	Tszhuang.m_iNowNtStation = m_iNowNtStation;//ׯ��
	if(sucess==0)//�ɹ��������
	{
		MoveData(0);//��������
		
		Tszhuang.success = 0;	
		for(int i=0;i<=PLAY_COUNT;i++)
		{
			Tszhuang.zhuangstation[i] = m_iZhuangStation[i];
		}
		for (int i=0;i<PLAY_COUNT; i++)
		{
			if (NULL == m_pUserInfo[i])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
			{
				continue;
			}
		
			SendGameData(i,&Tszhuang,sizeof(Tszhuang),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//���ͳɹ���Ϣ
		}
	}
	else 
	{
		//����ʧ����Ϣ
		if (NULL != m_pUserInfo[Station])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
		{
			SendGameData(Station,&Tszhuang,sizeof(Tszhuang),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG_ERR,0);//���ͳɹ���Ϣ
		}
	}
}	

///���ƣ�HanleXzhuang
///������������ҵ���ׯ��Ϣ
///@param Station λ�� pData ���ݰ� uSize ������
///@return 
void	CServerGameDesk::HanleXzhuang(BYTE Station,void * pData, UINT uSize)
{
	SHANG_ZHUANG *pdata = (SHANG_ZHUANG *)pData;

	if( m_pUserInfo[Station] == NULL)
		return;
	
	bool success = false;
	if(Station == m_iNowNtStation)//��ǰׯ�Ҳ���������ׯ
	{
		m_bXiaZhuang = true;
		success = true;
	}
	else
	{
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_iZhuangStation[i]==Station)//
			{
				m_iZhuangStation[i]=-1;
				success = true;
				break;
			}
		}
	}
	MoveData(0);//��������

	if(!success)
		return;

	m_IsAIStation[Station] = false;/// ��¼�����˵�λ�� 
	if(pdata->isAI)
	{
		m_IsAIStation[Station] = true;/// ��¼�����˵�λ�� 
	}
	//������ׯ��Ϣ
	SHANG_ZHUANG szhuang;
	szhuang.shang = false;
	szhuang.station = Station;
	szhuang.success = 0;
	szhuang.m_iNowNtStation = m_iNowNtStation;

	for(int i=0;i<=PLAY_COUNT;i++)
	{
		szhuang.zhuangstation[i] = m_iZhuangStation[i];
	}
	for (int i=0;i<PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])//���ø������ڵ���ҷ��Ϳ�ʼ��Ϣ
		{
			continue;
		}
		SendGameData(i,&szhuang,sizeof(szhuang),MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG,0);//���ͳɹ���Ϣ
	}
}	
//��ȡһ����
void	CServerGameDesk::GetACardValue(BYTE &byCard)
{
	for(int i=0; i<52; i++)
	{
		if (m_iTotalCard[i] != 255)
		{
			byCard = m_iTotalCard[i];
			m_iTotalCard[i] = 255;
			break;
		}
	}
}

///���ƣ�CountPaiZhi
///������������ֵ,ǰconut�ŵ�����ֵ
///@param count ǰ���ŵ���ֵ 2 ǰ���ţ�3 ��������ֵ
///@return 
void	CServerGameDesk::CountPaiZhi(int count)
{	
	int zhuang=0;//ׯ�Ƶ�
	int xian=0;  //���Ƶ�
	
	if(count==2)//ǰ�����Ƶ�ֵ
	{
		int value=0,value1=0;
		//��һ����
		value = m_Logic.GetCardNum(m_byUserCard[1][0]);//ׯ
		value1= m_Logic.GetCardNum(m_byUserCard[0][0]);//��
		if(value>=10)
		{
			value=0;
		}
		zhuang = value;
		if(value1>=10)
		{
			value1=0;
		}
		xian = value1;

		//�ڶ�����
		value = m_Logic.GetCardNum(m_byUserCard[1][1]);//ׯ
		value1= m_Logic.GetCardNum(m_byUserCard[0][1]);//��
		if(value>=10)
		{
			value=0;
		}
		zhuang += value;
		if(value1>=10)
		{
			value1=0;
		}
		xian += value1;

		if(zhuang>=10)
		{
			zhuang%=10;
		}
		if(xian>=10)
		{
			xian%=10;
		}
		m_iZPaiXing[0]=zhuang; //ǰ�����Ƶ��Ƶ�
		m_iXPaiXing[0]=xian;   //

	}
	else if(count==3)//�����Ƶ����Ƶ�
	{
		int value=0,value1=0;
		value = m_Logic.GetCardNum(m_byUserCard[1][2]);//ׯ
		value1= m_Logic.GetCardNum(m_byUserCard[0][2]);//��

		if(value>=10)
		{
			value=0;
		}
		if(value1>=10)
		{
			value1=0;
		}

		zhuang = value + m_iZPaiXing[0];	//ׯ�Ƶ�
		xian = value1 + m_iXPaiXing[0];		//���Ƶ�

		if(zhuang>=10)
		{
			zhuang%=10;
		}
		if(xian>=10)
		{
			xian%=10;
		}
		m_iZPaiXing[1]=zhuang;
		m_iXPaiXing[1]=xian;
	}

	if(zhuang>=8)//ׯ����
	{
		m_iZPaiXing[2]=zhuang;
		if(zhuang==xian)//����ͬ��
		{
			m_iZPaiXing[4]=zhuang;
		}
	}

	if(xian>=8)//������
	{
		m_iXPaiXing[2]=xian;
		if(zhuang==xian)//����ͬ��
		{
			m_iXPaiXing[4]=xian;
		}
	}

	if(m_Logic.GetCardNum(m_byUserCard[1][0])==m_Logic.GetCardNum(m_byUserCard[1][1]))//ׯ����
	{
		m_iZPaiXing[3]=zhuang;
		if (zhuang == 0)		//��Ҫ�����ڿ��Ƶ�����ֵΪ0ʱ��ǰ��������ͬ��Ȼ�Ƕ���
		{
			m_iZPaiXing[3] = 1;
		}
	}
	if(m_Logic.GetCardNum(m_byUserCard[0][0])==m_Logic.GetCardNum(m_byUserCard[0][1]))//�ж���
	{
		m_iXPaiXing[3]=xian;
		if (xian == 0)			//��Ҫ�����ڿ��Ƶ�����ֵΪ0ʱ��ǰ��������ͬ��Ȼ�Ƕ���
		{
			m_iXPaiXing[3] = 1;
		}
	}
}

///���ƣ�ChangeCard
///����������ׯ�е�ǰ������
///@param 
///@return 
void CServerGameDesk::ChangeCard()
{
	WORD temp=0;
	//��һ����
	temp=m_byUserCard[0][0];
	m_byUserCard[0][0]=m_byUserCard[1][0];
	m_byUserCard[1][0]=temp;
	//�ڶ�����
	temp=m_byUserCard[0][1];
	m_byUserCard[0][1]=m_byUserCard[1][1];
	m_byUserCard[1][1]=temp;
	//��յ�������
	m_byUserCard[0][2]=0;
	m_byUserCard[1][2]=0;
}


///���ƣ�ConTrolZX
///���������ƿ�ׯ����
///@param 
///@return 
void CServerGameDesk::ConTrolZX()
{
	if (m_bSuperSetting)
	{
		int probability=0;
		switch (m_bSuperState)
		{
		case 0:  //������Ϸ
			{
				probability = 0;  
				break;
			}
		case 1:  //��ׯ
			{
				probability = 1;  
				break;
			}
		case 2:  //����
			{
				probability = 2;  
				break;
			}
		case 3:  //����
			{
				probability = 3;  
				break;
			}
		}
		m_bSuperState = 0;


		if(probability == 0) //50%ʱ��������
		{
			//IAWinAutoCtrl();
		}
		else
		{
			//memset(&m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			if(probability == 1)//���ƿ�ׯ
			{
				if(0 == m_iWinQuYu[0])//���ֲ��ǿ�ׯ
				{
					for(int i=0;i<100;i++)
					{
						SendCard(i);
						if(0 != m_iWinQuYu[0])
						{
							break;
						}
					}
				}
			}
			else if(probability == 2)//���ƿ���
			{
				if(0 == m_iWinQuYu[3])//���ֲ��ǿ���
				{		
					for(int i=0;i<100;i++)
					{
						SendCard(i);
						if(0 != m_iWinQuYu[3])
						{
							break;
						}
					}
				}
			}
			else if(probability == 3)//���ƿ���
			{
				if(0 == m_iWinQuYu[6] )
				{		
					for(int i=0;i<100;i++)
					{
						SendCard(i);
						if(0 != m_iWinQuYu[6])
						{
							break;
						}
					}
				}
			}
		}
	}

}


///���ƣ�ClearWinData
///�����������Ѽ�����Ƶ�����
///@param all:true���������� ��������������
///@return 
void CServerGameDesk::ClearWinData(bool all)
{
	for(int i=0; i<8; i++)
	{
		m_iWinQuYu[0] = 0;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
		if(i<5)
		{
			m_iZPaiXing[i] = 0;
			m_iXPaiXing[i] = 0;
		}
	}
	m_iWinner = 0;
	m_iKaiPai = 0;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	if(all)
	{
		memset(&m_byUserCard, 255, sizeof(m_byUserCard));	// ׯ�����ϵ��˿�����

	}
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
///���ƣ�CheckTongDian
///������ͬ����
///@param num,���ǰ������
///@return true ͬ�㣬false ����ͬ��
bool CServerGameDesk::CheckTongDian(int num)
{
	int zhuang[3];
	int xian[3];
	int temp=0;
	for(int i=0;i<3;i++)
	{
		zhuang[i]= m_Logic.GetCardNum(m_byUserCard[1][i]);
		xian[i]  = m_Logic.GetCardNum(m_byUserCard[0][i]);
	}
	for(int i=0;i<2;i++)//����
	{
		for(int j=i+1;j<3;j++)
		{
			if(zhuang[i]<zhuang[j])
			{
				temp = zhuang[i];
				zhuang[i]=zhuang[j];
				zhuang[j]=temp;
			}
			if(xian[i]<xian[j])
			{
				temp = xian[i];
				xian[i]=xian[j];
				xian[j]=temp;
			}
		}
	}

	for(int i=0;i<num;i++)
	{
		if(zhuang[i] != xian[i])
		{
			return false;
		}
	}
	return true;
}
/*--------------------------------------------------------------------------*/
///��ȡӮǮ������
void	CServerGameDesk::GetWinAres()
{
	memset(&m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	if(!m_bThreeCard)//��ֻ����������,ǰ������ֵ�������Ƶ�ֵ
	{
		m_iZPaiXing[1] = m_iZPaiXing[0];
		m_iXPaiXing[1] = m_iXPaiXing[0];
	}

	if (m_iXPaiXing[1] == m_iZPaiXing[1])
	{
		m_iWinQuYu[6] = 9;	//��
		if (m_iZPaiXing[2]>0)	
		{
			m_iWinQuYu[1] = 3;	//ׯ����
		}
		if (m_iZPaiXing[3]>0)
		{
			m_iWinQuYu[2] = 12;	//ׯ����
		}

		if (m_iXPaiXing[2]>0)	
		{
			m_iWinQuYu[4] = 3;	//������
		}
		if (m_iXPaiXing[3]>0)
		{
			m_iWinQuYu[5] = 12;	//�ж���
		}
		//����Ƿ�ͬ���
		if (CheckTongDian(3))
		{
			m_iWinQuYu[7] = 33;
		}		
	}
	else if (m_iZPaiXing[1] > m_iXPaiXing[1])
	{
		m_iWinQuYu[0] = 2;
		if (m_iZPaiXing[2]>0)	
		{
			m_iWinQuYu[1] = 3;	//ׯ����
		}
		if (m_iZPaiXing[3]>0)
		{
			m_iWinQuYu[2] = 12;	//ׯ����
		}

		if (m_iXPaiXing[2]>0)	
		{
			m_iWinQuYu[4] = 3;	//������
		}
		if (m_iXPaiXing[3]>0)
		{
			m_iWinQuYu[5] = 12;	//�ж���
		}
	}
	else if (m_iZPaiXing[1] < m_iXPaiXing[1])
	{
		m_iWinQuYu[3] = 2;
		if (m_iZPaiXing[2]>0)	
		{
			m_iWinQuYu[1] = 3;	//ׯ����
		}
		if (m_iZPaiXing[3]>0)
		{
			m_iWinQuYu[2] = 12;	//ׯ����
		}

		if (m_iXPaiXing[2]>0)	
		{
			m_iWinQuYu[4] = 3;	//������
		}
		if (m_iXPaiXing[3]>0)
		{
			m_iWinQuYu[5] = 12;	//�ж���
		}
	}

	//�жϵ�˫
	if (m_iZPaiXing[1]%2==0)
	{
		m_iWinQuYu[9] = 2;
	}
	else
	{
		m_iWinQuYu[8] = 2;
	}

	if (m_iXPaiXing[1]%2==0)
	{
		m_iWinQuYu[11] = 2;
	}
	else
	{
		m_iWinQuYu[10] = 2;
	}

}
/*--------------------------------------------------------------------------*/
///���ƣ�GetWin
///��������ȡӮǮ������
///@param 
///@return 
void CServerGameDesk::GetWin()
{
	//������
	memset(&m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	if(!m_bThreeCard)//��ֻ����������,ǰ������ֵ�������Ƶ�ֵ
	{
		m_iZPaiXing[1] = m_iZPaiXing[0];
		m_iXPaiXing[1] = m_iXPaiXing[0];
	}
	if(m_iXPaiXing[2]>0 || m_iZPaiXing[2]>0)//������ʱ
	{
		if(m_iXPaiXing[2] == m_iZPaiXing[2])//�����ͣ���Ӯ��
		{
			//ͬ����
			m_iKaiPai =7;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			if(CheckTongDian(2))//ͬ��� ����Ӯ��ͬ���Ӯ��
			{
				m_iWinQuYu[7] = 33;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iKaiPai =8;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ�7�ͣ�8ͬ���
			}
			m_iWinQuYu[6] = 9;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			//lym0709Ц����Ҫ��ĳɣ����ͣ�ׯ�в��䲻Ӯ
			m_iWinQuYu[0] = 1;     //
			m_iWinQuYu[3] = 1;     //			
			m_iWinner=3;//��Ӯ
		}
		else if(m_iXPaiXing[2] > m_iZPaiXing[2])//����������Ӯ����������
		{
			m_iKaiPai =5;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			if(m_iXPaiXing[3]>0)		//�м����������ӣ���Ӯ��������Ӯ���ж���Ӯ��
			{				
				m_iWinQuYu[5] = 12;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iKaiPai =6;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			}
			m_iWinQuYu[3] = 2;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			m_iWinQuYu[4] = 3;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			m_iWinner=2;//��Ӯ
		}
		else //ׯ������ׯӮ��ׯ����Ӯ��
		{
			m_iKaiPai =2;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			if(m_iZPaiXing[3]>0)//ׯ�����������ӣ�ׯӮ��ׯ����Ӯ��ׯ����Ӯ��
			{
				m_iWinQuYu[2] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���				
				m_iKaiPai =3;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
			}
			m_iWinQuYu[0] = 2;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			m_iWinQuYu[1] = 3;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
			m_iWinner=1;//ׯӮ
		}
	}
	else//û������
	{
		if(m_bThreeCard)//ׯ��������3����ʹ��m_iXPaiXing[1]������Ƶ�
		{
			if(m_iXPaiXing[1] == m_iZPaiXing[1])//�� ������ͬ ��Ӯ��
			{
				m_iKaiPai =7;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//���ͬ���
				if(CheckTongDian(3))
				{
					m_iWinQuYu[7] = 33;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =8;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[6]=9;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				//lym0709Ц����Ҫ��ĳɣ����ͣ�ׯ�в��䲻Ӯ
				m_iWinQuYu[0] = 1;			//
				m_iWinQuYu[3] = 1;			//				
			    m_iWinner=3;//��Ӯ
			}
			else if(m_iXPaiXing[1] > m_iZPaiXing[1])//�мҴ���Ӯ��
			{
				m_iKaiPai =4;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//����ж���
				if(m_iXPaiXing[3]>0)//�м��Ƕ��ӣ���Ӯ���ж���Ӯ��
				{				
					m_iWinQuYu[5] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =6;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[3] = 2;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iWinner=2;//��Ӯ
			}
			else//ׯ�Ҵ�ׯӮ
			{
				m_iKaiPai =1;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//���ׯ����
				if(m_iZPaiXing[3]>0)//ׯ���Ƕ��ӣ�ׯӮ��ׯ����Ӯ��
				{				
					m_iWinQuYu[2] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =3;			//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[0] = 2;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iWinner=1;//ׯӮ
			}
		}
		else//û��������ׯ�е��ƶ���2�ţ�˵��ׯ�ж���6��7�� 
		{
			if(m_iXPaiXing[0] == m_iZPaiXing[0])//�� ������ͬ��Ӯ��
			{
				m_iKaiPai =7;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//���ͬ���
				if(CheckTongDian(2))//ͬ��ͣ���Ӯ��ͬ���Ӯ��
				{
					m_iWinQuYu[7] = 33;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =8;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[6] = 9;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				//lym0709Ц����Ҫ��ĳɣ����ͣ�ׯ�в��䲻Ӯ
				m_iWinQuYu[0] = 1;     //
				m_iWinQuYu[3] = 1;     //				
				m_iWinner=3;//��Ӯ
			}
			else if(m_iXPaiXing[0] > m_iZPaiXing[0])//�мҴ���Ӯ��
			{
				m_iKaiPai =4;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//����ж���
				if(m_iXPaiXing[3]>0)//�м��Ƕ��ӣ���Ӯ���ж���Ӯ��
				{	
					m_iWinQuYu[5] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =6;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[3] = 2;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iWinner=2;//��Ӯ
			}
			else//ׯ�Ҵ�ׯӮ
			{
				m_iKaiPai =1;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				//���ׯ����
				if(m_iZPaiXing[3]>0)//ׯ���Ƕ��ӣ�ׯӮ��ׯ����Ӯ��
				{			
					m_iWinQuYu[2] = m_iLevelBase;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
					m_iKaiPai =3;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
				}
				m_iWinQuYu[0] = 2;     //��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
				m_iWinner=1;//ׯӮ
			}
		}
	}

	//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	if (m_iXPaiXing[3] > 0)			//�м����������ӣ���Ӯ��������Ӯ���ж���Ӯ��
	{				
		m_iWinQuYu[5] = m_iLevelBase;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���		
	}	
	if (m_iZPaiXing[3] > 0)			//ׯ�����������ӣ�ׯӮ��ׯ����Ӯ��ׯ����Ӯ��
	{
		m_iWinQuYu[2] = m_iLevelBase;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���				
	}

	if (m_iZPaiXing[1] >= 8)		//�г� �������ߵ��� ʱ ׯ�Ҷ� Ҫ��Ǯ
	{
		m_iWinQuYu[1] = 3;
	}
	if (m_iXPaiXing[1] >= 8)
	{
		m_iWinQuYu[4] = 3; 
	}
}

///���ƣ�GetTheThird
///��������ȡ��������
///@param 
///@return true�е�������  false û�е�������
bool	CServerGameDesk::GetTheThird()
{
	if(m_iZPaiXing[2] > 0 || m_iXPaiXing[2] > 0)//ׯ������һ������������ץ��
		return false;

	if(m_iXPaiXing[0] >= 6)//�мҲ���ץ��
	{
		if(m_iZPaiXing[0] <= 5)//ׯ��5������Ҫ����
		{
			GetACardValue(m_byUserCard[1][2]);
					
			return true;
		}
	}
	else//�м�Ҫ��һ����
	{
		GetACardValue(m_byUserCard[0][2]);

		if(IsGetTheThird())//�����мҵĵ�������ֵȷ��ׯ���Ƿ�Ҫ����
		{
			GetACardValue(m_byUserCard[1][2]);	
		}
		return true;
	}
	return false;
}

///���ƣ�IsGetTheThird
///�������м����˵�����������£�ׯ����Ҫ����lym0511
///@param 
///@return true��  false ����
bool	CServerGameDesk::IsGetTheThird()
{
	int num=0;
	num=m_Logic.GetCardNum(m_byUserCard[0][2]);//�мҵĵ���������ֵ	
	if(num>=10)
	{
		num=0;
	}
	switch(num)
	{
	case 0:
	case 1:
		{
			if(m_iZPaiXing[0]<=3)//ׯ������3�����£�����
				return true;
		}
		break;
	case 2:
	case 3:
		{
			if(m_iZPaiXing[0]<=4)//ׯ������4�����£�����
				return true;
		}
		break;
	case 4:
	case 5:
		{
			if(m_iZPaiXing[0]<=5)//ׯ������5�����£�����
				return true;
		}
		break;
	case 6:
	case 7:
		{
			if(m_iZPaiXing[0]<=6)//ׯ������6�����£�����
				return true;
		}
		break;
	case 8:
		{
			if(m_iZPaiXing[0]<=2)//ׯ������2�����£�����
				return true;
		}
		break;
	case 9:
		{
			if(m_iZPaiXing[0]<=3)//ׯ������3�����£�����
				return true;
		}
		break;
	default:
		break;
	}
	return false;
}

///���ƣ�SendCard
///����������
///@param 
///@return 
void CServerGameDesk::SendCard(int iIndex)
{
	ZeroMemory(m_iZPaiXing,sizeof(m_iZPaiXing));
	ZeroMemory(m_iXPaiXing,sizeof(m_iXPaiXing));
	m_bThreeCard = false;
	//�ַ��˿�
	memset(m_byUserCard,255,sizeof(m_byUserCard));
	memset(m_iTotalCard,255,sizeof(m_iTotalCard));
	m_Logic.RandCard(m_iTotalCard,52,iIndex);
	for (int i=0; i<2; i++)
	{
		for (int j=0; j<2;j++)
		{
			GetACardValue(m_byUserCard[i][j]);
		}		
	}
	//������ֵ
	CountPaiZhi(2);

	//�ж��м��Ƿ���Ҫ��ȡ��������
	if (m_iXPaiXing[0] < 5)
	{
		//�е������ƾ����¼�����ֵ				
		m_bThreeCard=true;
		GetACardValue(m_byUserCard[0][2]);
	}
	if (m_iZPaiXing[0] < 5)
	{
		//�е������ƾ����¼�����ֵ				
		m_bThreeCard=true;
		GetACardValue(m_byUserCard[1][2]);
	}

	CountPaiZhi(3);//��������ֵ	

	//��ȡ�н�������
	GetWinAres();
}

///���ƣ�CountFen
///�������������
///@param 
///@return 
void CServerGameDesk::CountFen()
{
	if(m_iNowNtStation<0)
		return;
	memset(&m_i64UserFen, 0, sizeof(m_i64UserFen)); //��������
	
	m_i64UserFen[m_iNowNtStation] = m_i64ZhongZhu;//ׯ���Ȼ�ȡ��ע

	//ׯ���⸶ÿ�����
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_iNowNtStation)//ׯ�Ҳ������
		{
			continue;
		}
		if(NULL == m_pUserInfo[i])//��Ҳ����ھͲ��ü���
		{
			continue;
		}
		if(m_iUserXiaZhuData[i][BET_ARES] == 0)//���û����ע�Ͳ��ü���
		{
			continue;
		}
		//��ע�˶�����
		m_iUserPlayCount[i][0]++;
		__int64 win=0;
		__int64 zhu=0;
		
		for(int j=0; j<BET_ARES; j++)
		{
			zhu += m_iUserXiaZhuData[i][j];//����µ���ע��
			win += (m_iUserXiaZhuData[i][j]*m_iWinQuYu[j]);//���Ӯ�Ľ��	
		}
		if(zhu <= 0)
		{
			continue;
		}
		
		m_i64UserFen[i] = (win-zhu);//Ӯ��-�µ�ע������ұ��ֵľ�Ӯ�����
		m_i64UserFen[m_iNowNtStation] -= win;//ׯ��-���Ӯ�ķ֡�

		//����Ǻ� ��ô�ͷ���ׯ���е���ע������
		if (m_iWinQuYu[6]>0)
		{
			m_i64UserFen[i] += (m_iUserXiaZhuData[i][0]+m_iUserXiaZhuData[i][3] + m_iUserXiaZhuData[i][8]+ m_iUserXiaZhuData[i][9]+ m_iUserXiaZhuData[i][10]+ m_iUserXiaZhuData[i][11]);
			m_i64UserFen[m_iNowNtStation] -= (m_iUserXiaZhuData[i][0]+m_iUserXiaZhuData[i][3]+ m_iUserXiaZhuData[i][8]+ m_iUserXiaZhuData[i][9]+ m_iUserXiaZhuData[i][10]+ m_iUserXiaZhuData[i][11]);
		}

		//Ӯ�˶�����
		if (m_i64UserFen[i] >0)
		{
			m_iUserPlayCount[i][1]++;
		}
	}
}
//һ�ֽ��������˿���ʱ������Ƿ��ܹ�������ʼ��Ϸlym0512
bool CServerGameDesk::CheckCanBegin()
{
	return false;
}

///���ƣ�StartGame
///������ͨ��ȫ�����ֵķ�ʽ��ʼ��Ϸ
///@param 
///@return 
bool CServerGameDesk::StartGame()
{	
	KillTimer(ID_TIMER_GAME_FREE);
	m_bGamePlaying=true;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(NULL == m_pUserInfo[i])//��Ҳ����ھ�
			continue;
		m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;//���������Ϊͬ��״̬
	}	
	GameBegin(ALL_ARGEE);
	return true;
}



///���ƣ�ServerInit
///��������ʼ����������������
///@param 
///@return 
void CServerGameDesk::ServerInit()
{			
	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		m_i64UserFen[i] =0;          //������ұ��ֵĵ÷�
	}
	memset(&m_iAIMoney, 0, sizeof(m_iAIMoney));	/**< �����˵���ע*/

	for(int i=0;i<BET_ARES;i++)
	{
		m_iQuYuZhu[i]=0;    //����ÿ�������µ�ע��
		m_i64MaxZhu[i]=0;     //����ÿ������������µ�ע��
		m_iWinQuYu[i]=0;			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	}
	
	for(int i=0;i<5;i++)
	{	
		m_iZPaiXing[i]=0;         //ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
		m_iXPaiXing[i]=0;         //�м�����
	}		
	memset(&m_byUserCard, 255, sizeof(m_byUserCard));	//��ҵ���

	for(int i=0;i<PLAY_COUNT;i++)
	{
		for(int j=0;j<BET_ARES+1;j++)
		{
			m_iUserXiaZhuData[i][j]=0;
		}
	}
	m_iKaiPai =0;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	m_iWinner=0;				//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	m_i64ZhongZhu =0;				//���ѵ�ǰ��ע��

	
	m_bXiaZhuang=false;			//��ǰׯ��������ׯ
	m_bThreeCard=false;			//ׯ��������3����

}

///���ƣ�CanBeginGame
///������������Ϸ�ܷ�ʼ����������ֹ��ׯ�ļٿ�ʼ
///@param 
///@return false ���ܿ�ʼ ��������ƽ̨����
bool CServerGameDesk::CanBeginGame()
{
	if(m_iNowNtStation<0)
		return false;
	return __super::CanBeginGame();
}

///���ƣ�SrvTestlog
///��������־��¼�ӿ�
///@param pMessage ��־��Ϣ
///@return 
void  CServerGameDesk::SrvTestlog(TCHAR *pMessage)
{
	try
	{
		CString  tempstr = CINIFile::GetAppPath();
		SYSTEMTIME st;
		GetLocalTime(&st);
		TCHAR FileName[256];
		//wsprintf(FileName, TEXT("e:\\syslog%d.txt\0"),GetCurrentProcessId());
		wsprintf(FileName, TEXT("%s%d-%d-%d %d-bjl-test.txt"),tempstr,st.wYear,st.wMonth,st.wDay,st.wHour);
		CFile syslog;
		syslog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite);
		TCHAR szDate[256];
		wsprintf(szDate, TEXT("%d-%d-%d %d-%d-%d.%d\t"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		syslog.SeekToEnd();
		syslog.Write(szDate,lstrlen(szDate));
		syslog.SeekToEnd();
		syslog.Write(pMessage,lstrlen(pMessage));
		TCHAR nextLine[] = TEXT("\r\n");
		syslog.SeekToEnd();
		syslog.Write(nextLine,lstrlen(nextLine));

		syslog.Close();
	}
	catch(CException *e)
	{
		e->Delete();
		return ; 
	}
}

///���ƣ�OnSuperSetting
///������������Ӯ����
///@param bDeskStation λ�� bState ����״̬
///@return 
void CServerGameDesk::OnSuperSetting(BYTE bDeskStation, BYTE bState)
{
	if (IsSuperUser(bDeskStation))
	{
		m_bSuperSetting = TRUE;
		m_bSuperState = bState;

		SUPERSETTING TSuperSetResult;
		TSuperSetResult.bDeskStation = bDeskStation;
		TSuperSetResult.bState = bState;

		//�����������ͻ��˷��Ϳ�����Ϣ
		SendGameData(bDeskStation,&TSuperSetResult, sizeof(TSuperSetResult), MDM_GM_GAME_NOTIFY, ASS_SUPER_SETTING, 0);
	}
}

///���ƣ�SetAICtrl
///�����������ͻ������û����˿��Ʋ�������������Ӯ���� ��
///@param Station λ�� 
///@return 
void CServerGameDesk::SetAICtrl(BYTE Station,void * pData, UINT uSize)
{
	if (m_pUserInfo[Station] == NULL)
		return;
	OutputDebugString("������Ϣ���������յ���Ϣ");

	int iRoomID = m_pDataManage->m_InitData.uRoomID;
	CString key= "game";
	CString gameKey ="";
	gameKey.Format("%d_%d",NAME_ID, iRoomID);
	CString s = CBcfFile::GetAppPath ();	//����·��
	CINIFile f (s + SKIN_FOLDER +"_s.ini");
	CString strText;

	// �жϳ����û��ڷ��������Ƿ�����
	int iCount = f.GetKeyVal(key, "SuperUserCount", 5);
	for (int i = 1; i <= iCount; i++)
	{
		strText.Format("SuperUserID%d", i);
		long  lUserID = f.GetKeyVal(key, strText, 0);
		if (m_pUserInfo[Station]->m_UserData.dwUserID != lUserID)//�������ǳ����ͻ���
			continue;
		AICTRLINFO *pAIInfo = (AICTRLINFO *)pData;
		// ���û����˿��Ʋ���
		if(pAIInfo->bIsClearAIWinMoney)///���������Ӯ��Ǯ
		{
			m_iAIHaveWinMoney = 0;
		
		}
		if(pAIInfo->bIsSetAICtrl)//���û����˿���
		{
			CString value = "";
			if(pAIInfo->bAIWinAndLostAutoCtrl != m_bAIWinAndLostAutoCtrl)//�Ƿ��������˿���
			{
				value.Format("%d",pAIInfo->bAIWinAndLostAutoCtrl);
				f.SetKeyValString(gameKey,"AIWinAndLostAutoCtrl",value);
				m_bAIWinAndLostAutoCtrl = pAIInfo->bAIWinAndLostAutoCtrl;
			}
			if(pAIInfo->iAIWantWinMoney != m_iAIWantWinMoney)//������ҪӮ��Ǯ
			{
				value.Format("%d",pAIInfo->iAIWantWinMoney);
				f.SetKeyValString(gameKey,"AIWantWinMoney",value);
				m_iAIWantWinMoney = pAIInfo->iAIWantWinMoney;
			}
			if(pAIInfo->iAIMaxWinInOne != m_iAIMaxWinInOne)//ƽ��ÿ��Ӯ�����ֵ
			{
				value.Format("%d",pAIInfo->iAIMaxWinInOne);
				f.SetKeyValString(gameKey,"AIMaxWinInOne",value);
				m_iAIMaxWinInOne = pAIInfo->iAIMaxWinInOne;
			}
		}

		AICTRLINFO AIInfo;
		AIInfo.bDeskStation = Station;//λ��
		AIInfo.bAIWinAndLostAutoCtrl = m_bAIWinAndLostAutoCtrl;//�Ƿ�������

		AIInfo.iAIWantWinMoney = m_iAIWantWinMoney;	//�����˵�ӮǮĿ��
		AIInfo.iAIMaxWinInOne = m_iAIMaxWinInOne;	//������ƽ��ÿ��ӮǮ���ֵ
		AIInfo.iAIHaveWinMoney = m_iAIHaveWinMoney;	//�������Ѿ�Ӯ�˶���Ǯ
		
		//��������Ϣ�������������ͻ���
		for(int j=0;j<PLAY_COUNT;j++)
		{				///��������Ӯ���� ���򳬼��ͻ��˷��ͻ����˿�����Ϣ
//			if(m_bIsSuperStation[j])
			{
				strText.Format("������Ϣ��������Ϣ�������ͻ��� Ŀ��%d",pAIInfo->iAIWantWinMoney);
				OutputDebugString(strText);
				SendGameData(j,&AIInfo, sizeof(AIInfo), MDM_GM_GAME_NOTIFY, ASS_SUPER_AI_INFO, 0);
			}
		}
		return ;
	}
	
}

///���ƣ�AuthPermissions
///��������֤�����û�Ȩ�ޣ�����Ƿ񳬼��ͻ��ˣ��������ͻ�����ҷ�����Ϣ
///@param 
///@return 
void CServerGameDesk::AuthPermissions(BYTE bDeskStation)
{
	if (IsSuperUser(bDeskStation))
	{
		SUPERSTATE superState;
		// ���û�Ϊ�����û�, ֪ͨ�ͻ���
		superState.bDeskStation = bDeskStation;
		superState.bEnable = TRUE;  // �����ͻ���

		SendGameData(bDeskStation, &superState, sizeof(superState), MDM_GM_GAME_NOTIFY, ASS_SUPER_STATE, 0);
	}
}

//�ж��Ƿ񳬶����
bool CServerGameDesk::IsSuperUser(BYTE byDeskStation)
{
	if (NULL == m_pUserInfo[byDeskStation])
	{
		return false;
	}
	bool bIsSuper = false;
	for(int i=0; i<m_vtSuperUserID.size();i++)
	{
		if (m_pUserInfo[byDeskStation]->m_UserData.dwUserID == m_vtSuperUserID.at(i))
		{
			bIsSuper = true;
			break;
		}
	}
	return	bIsSuper;
}

///���ƣ�GetMeFanFen
///��������ȡ�����֣�����ʱ��ׯ���е���һ�ȡԭ���ķ���
///@param station ��ȡ���ֵ�λ��
///@return  ���صķ���
int CServerGameDesk::GetMeFanFen(int station)
{
	if(m_iNowNtStation<0)
		return 0;
	int fen = 0;
	if(m_pUserInfo[station] == NULL)
		return fen;
	if(station == m_iNowNtStation)
		return fen;

	if(0 == m_iWinQuYu[6])//���ǿ���
		return fen;
	fen = m_iUserXiaZhuData[station][0] + m_iUserXiaZhuData[station][3] + m_iUserXiaZhuData[station][8]+ m_iUserXiaZhuData[station][9]+ m_iUserXiaZhuData[station][10]+ m_iUserXiaZhuData[station][11];
	return fen;
}



/*
///���ƣ�GetIAWinQuYu
///��������������Ӯ���ƣ���������Ӯ�Զ�����
///@param 
///@return ����ֵ 0 ׯ��1 �У�2 ��
void CServerGameDesk::IAWinAutoCtrl()
{///���û�г����ͻ��˿��Ʋ��ҿ������Զ����ƣ�����л�������Ӯ�Զ�����
	if(!m_bAIWinAndLostAutoCtrl)
		return;
	bool bAIWin = false;
	CString str="";

	m_iAICtrlCount++;
	if(m_iAIHaveWinMoney < m_iAIWantWinMoney)
	{
		//��û��׬��Ǯ
		if(/*m_iAIMaxWinInOne<=0 || *//*( m_iAIMaxWinInOne>0 && m_iAIMaxWinInOne>(m_iAIHaveWinMoney/m_iAICtrlCount)))
		{///����ÿ�����ֵ����û�е����ֵ��������ҪӮ��ׯӮǮ
			bAIWin = true;
			//OutputDebugString("wysoutforƽ��ÿ�ѿ���");
		}	
	}
	//������û�п��Ƶ���������������˵���Ӯ���ϱ��ѵ���ӮС�뵱ǰ����Ӯ��Ǯ������Ҫ���ӮǮ
	if (!bAIWin&&CountAIWinMoney()<0)
	{
		if(m_iAIHaveWinMoney+CountAIWinMoney()<m_iAIWantWinMoney)
		{
			bAIWin = true;
		}
	}

	//CString CST;
	//CST.Format("wysoutfor::����ǰ�������Ѿ�Ӯ��Ǯ===%d,׼��Ӯ��Ǯ==%d,�����ֵ==%d,�������=%d",m_iAIHaveWinMoney,m_iAIWantWinMoney,CountAIWinMoney(),bAIWin);
	//OutputDebugString(CST);
	
	if(bAIWin)
	{//������ҪӮǮ
		/*str.Format("�������Զ����ƣ�������ҪӮ   ��Ӯ%d ƽ������%d ��ǰƽ��%d",m_iAIHaveWinMoney,m_iAIMaxWinInOne,m_iAIHaveWinMoney/m_iAICtrlCount);
		OutputDebugString(str);*//*
		if(CountAIWinMoney()<0)
		{
			for(int i=0; i<50; i++)
			{
				SendCard();
				if(CountAIWinMoney()>=0)
					break;
			}
		}
	}
	//else
	//{///������Ҫ��Ǯ
	//	/*str.Format("�������Զ����ƣ�������Ҫ��   ��Ӯ%d ƽ������%d ��ǰƽ��%d",m_iAIHaveWinMoney,m_iAIMaxWinInOne,m_iAIHaveWinMoney/m_iAICtrlCount);
	//	OutputDebugString(str);*//*
	//	if(CountAIWinMoney()>0)
	//	{
	//		for(int i=0; i<50; i++)
	//		{
	//			SendCard();
	//			if(CountAIWinMoney()<=0)
	//				break;
	//		}
	//	}
	//}
}
*/
//-------------------------------------------------------------------------------------------------------------------
///���ƣ�IAWinAutoCtrl
///��������������Ӯ���ƣ������˸����趨���������һ������Ӯ���ʣ��û�������Ӯ�����ȶ�
void CServerGameDesk::IAWinAutoCtrl()
{///���û�г����ͻ��˿��Ʋ��ҿ������Զ����ƣ�����л�������Ӯ�Զ�����
	if(!m_bAIWinAndLostAutoCtrl)
		return;
	bool bAIWin = false;
	

	srand((unsigned)GetTickCount());
	int iResult = rand()%100;
	
	if (0 >=m_iAIHaveWinMoney )
	{//������Ӯ��Ǯ����0 �����˱�Ӯ
		bAIWin = true;		
	}
	else if((0 < m_iAIHaveWinMoney) && (m_iAIHaveWinMoney < m_iAIWantWinMoneyA1))
	{// �����˵�ӮǮ��0-A1���� ���Ҹ��ʷ���m_iAIWinLuckyAtA1 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA1)
			bAIWin = true;								
	}
	else if ((m_iAIWantWinMoneyA1 <= m_iAIHaveWinMoney)  && (m_iAIHaveWinMoney <m_iAIWantWinMoneyA2))
	{// �����˵�ӮǮ��A1-A2���� ���Ҹ��ʷ���m_iAIWinLuckyAtA2 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA2)
			bAIWin = true;			
	}
	else if ((m_iAIWantWinMoneyA2 <= m_iAIHaveWinMoney)  && (m_iAIHaveWinMoney <m_iAIWantWinMoneyA3))
	{// �����˵�ӮǮ��A2-A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA3 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA3)
			bAIWin = true;
	}
	else
	{// �����˵�ӮǮ����A3���� ���Ҹ��ʷ���m_iAIWinLuckyAtA4 ������ҪӮ
		if (iResult <=m_iAIWinLuckyAtA4)
			bAIWin = true;
	}
		
	if(bAIWin)
	{//������ҪӮǮ
		if(CountAIWinMoney()<0)
		{
			for(int i=0; i<50; i++)
			{
				SendCard();
				if(CountAIWinMoney()>=0)
					break;
			}
		}
	}
	else
	{///������Ҫ��Ǯ//��������ӮǮ����0���ߵ��»�����ӮǮΪ��������Ҫ���·���
		if(CountAIWinMoney()>0 || (m_iAIHaveWinMoney +CountAIWinMoney())<0)
		{
			for(int i=0; i<50; i++)
			{
				SendCard();
				if(CountAIWinMoney()<=0)
				{
					if ((m_iAIHaveWinMoney +CountAIWinMoney())<0)
					{//����������»�������ӮǮΪ��������ô�˴��ؿ���ֱ���û�����Ӯ��
						//DWJ��������Ǯ�����һ�����ӮǮ��ȥ�������Ǯ����Ϊ����,����ط���50�λ����У�ֻ��˵��RP��������ߡ�����
						if (49 != i)
						{
							continue;
						}
						//���·��Ʒ���49�λ���û�з���Ҫ��ģ�ֻ���û�����Ӯ��
						for (int j=0; j<50; j++)
						{
							SendCard();
							if (CountAIWinMoney()>=0)
								break;
						}
					}
					break;
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
///���ƣ�CountAIWinMoney
///��������������Ӯ���ƣ����㵱ǰ�ƻ����˵���ӮǮ
///@param 
///@return ������Ӯ��Ǯ������Ϊ��Ǯ
int CServerGameDesk::CountAIWinMoney()
{
	__int64 money = 0;
	if(m_IsAIStation[m_iNowNtStation])
	{///�����������ׯ��,������ʵ��ҵ�Ǯ
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(m_IsAIStation[i] || NULL == m_pUserInfo[i] || m_iUserXiaZhuData[i][8] == 0)//�����˲��ü��� || ��Ҳ����ھͲ��ü��� || ���û����ע�Ͳ��ü���
				continue;
			for(int j=0;j<BET_ARES+1;j++)
			{			
				money += m_iUserXiaZhuData[i][j];
				money -= (m_iUserXiaZhuData[i][j]*m_iWinQuYu[j]);
			}
		}
	}
	else
	{///��������˲���ׯ��,��������˵�Ǯ
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(!m_IsAIStation[i])//�����˲��ü��� || ��Ҳ����ھͲ��ü��� || ���û����ע�Ͳ��ü���
				continue;
			for(int j=0;j<BET_ARES+1;j++)
			{			
				money -= m_iUserXiaZhuData[i][j];
				money += (m_iUserXiaZhuData[i][j]*m_iWinQuYu[j]);
			}
		}
	}
	return money;
}

///���ƣ�GetAnAIApplyStation
///��������ȡһ������������ׯ�����Ļ�����λ�ã���������ׯ���ƣ�
///@param 
///@return -1 û������������ >=0 �Ϸ�λ��ֵ
int CServerGameDesk::GetAnAIApplyStation()
{
	int station = -1;
	bool zhuang = false;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(!m_IsAIStation[i] || NULL == m_pUserInfo[i] || m_iNowNtStation == i||m_pUserInfo[i]->m_UserData.isVirtual<=0)
			continue;
		srand((unsigned)time(NULL)+i);
	    int iRand =(rand()%3+i)%3;
		if (iRand>1)
		{
			continue;
		}
		zhuang = false;
		for(int j=0; j<PLAY_COUNT; j++)
		{
			if(m_iZhuangStation[j] == i)
				zhuang = true;
		}
		if(zhuang)//�Ѿ���ׯ���б�
			continue;
		if(m_pUserInfo[i]->m_UserData.i64Money< m_iShangZhuangLimit|| m_pUserInfo[i]->m_UserData.i64Money>1500000000000000000)
			continue;
		station = i;
		break;
	}
	return station;
}


///���ƣ�GetQuYuCanNote
///��������ȡÿ�����������ע
///@param 
///@return 
void CServerGameDesk::GetQuYuCanNote()
{
	__int64 money = 0;
	__int64 note = 0;
	for(int i=0;i<BET_ARES;i++)
	{
		note += m_iQuYuZhu[i]; 		/**< ����ÿ�������µ�ע��		*/
		money += m_i64MaxZhu[i];  	/**< ����ÿ������������µ�ע�� */
	}
	if(m_iNowNtStation>=0 && m_iNowNtStation <PLAY_COUNT && NULL != m_pUserInfo[m_iNowNtStation] && note<=0 && money<=0)
	{
		m_i64MaxZhu[0]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;			//����ÿ������������µ�ע��
		m_i64MaxZhu[1]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/2;		//����ÿ������������µ�ע��
		m_i64MaxZhu[2]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/11;		//����ÿ������������µ�ע��
		m_i64MaxZhu[3]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money;			//����ÿ������������µ�ע��
		m_i64MaxZhu[4]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/2;		//����ÿ������������µ�ע��
		m_i64MaxZhu[5]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/11;		//����ÿ������������µ�ע��
		m_i64MaxZhu[6]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/8;		//����ÿ������������µ�ע��
		m_i64MaxZhu[7]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/32;		//����ÿ������������µ�ע��
		m_i64MaxZhu[8]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/2;		//����ÿ������������µ�ע��
		m_i64MaxZhu[9]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/2;		//����ÿ������������µ�ע��
		m_i64MaxZhu[10]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/2;		//����ÿ������������µ�ע��
		m_i64MaxZhu[11]=m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money/2;		//����ÿ������������µ�ע��
	}
}



//���������Ƿ���˫����ע
BOOL  CServerGameDesk::CanDouble(BYTE bDeskStation)
{
	return FALSE;
	//�Ƿ�Ƿ���������
	if (bDeskStation >= PLAY_COUNT||bDeskStation<0)
		return FALSE;

	if (NULL == m_pUserInfo[bDeskStation])
	{
		return FALSE;
	}

	

	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());
	int curtime=atoi(stime);

	return curtime < m_pUserInfo[bDeskStation]->m_UserData.iVipTime?TRUE:FALSE;

}
//ͳ�ƻ�������ׯ����
int CServerGameDesk::CountRobortNum()
{
	int iCount = 0;
	for (int i = 0;i<PLAY_COUNT;i++)
	{		
		if(m_iZhuangStation[i]<0)
		{
			continue;
		}
		if (NULL == m_pUserInfo[m_iZhuangStation[i]])
		{
			continue;
		}
		if (m_pUserInfo[m_iZhuangStation[i]]->m_UserData.isVirtual>0)
		{
			iCount++;
		}
	}
	return iCount;	
}
//����ׯ��
void CServerGameDesk::ClearNt()
{
	if(m_pUserInfo[m_iNowNtStation]==NULL || m_bXiaZhuang || m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money< m_iShangZhuangLimit
		|| m_iZhuangBaShu>=m_iMaxZhuang
		|| m_pUserInfo[m_iNowNtStation]->m_UserData.i64Money>1500000000000000000)//��ǰׯ������������ׯ��
	{
		m_iNowNtStation=-1;//���õ�ǰׯ��Ϊ-1	
		m_i64NtWin=0;	//��ǰׯ��Ӯ�Ľ��
		m_iZhuangBaShu=0;//ׯ�ҽ����˼���
		m_bXiaZhuang=false;
	}
}
