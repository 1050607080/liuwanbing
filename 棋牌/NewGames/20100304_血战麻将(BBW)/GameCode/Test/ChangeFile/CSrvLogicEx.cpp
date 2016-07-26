#include "stdafx.h"
#include "CSrvLogicEx.h"
#include "../TestDlg.h"

CSrvLogicEx::CSrvLogicEx(void)
{	
}

CSrvLogicEx::~CSrvLogicEx(void)
{
}

///////////////////////////////��Ϣ����///////////////////////////////////////////////////
//���������¼�
bool CSrvLogicEx::GetGameStaton(BYTE bDeskStation, bool bWatchUser,BYTE GameStation)
{
	//return CSrvLogic::GetGameStaton(bDeskStation, bWatchUser, GameStation);

	GameStationDataEx GameStationData;
	GameStationData.Init();
	GameStationData.iVersion	=  MAIN_VERSION;
	GameStationData.iVersion2	=  ASS_VERSION;
	//��ȡ���䱶�����ܷ���
	GameStationData.Stationpara = pDesk->GetGameStation();		 //��Ϸ״̬

	m_UserData.CopyAllGCPData(GameStationData.m_UserGCPData);
	m_UserData.CopyAllOutPai(GameStationData.m_byArOutPai);
	m_UserData.CopyAllMenPai(GameStationData.m_iArMenPai);
	m_UserData.CopyAllHandPai(GameStationData.m_byArHandPai,bDeskStation,false);
	if(!bWatchUser)
	{
		m_UserData.SetUserState(bDeskStation,USER_STATE_CUT,false);//ȡ������״̬
		m_UserData.SetUserState(bDeskStation,USER_STATE_AUTO,false);//ȡ���й�״̬
	}

	GameStationData.m_bitUserState = m_UserData.m_bitUserState;//��ȡ���״̬
	GameStationData.m_byOtherOutPai = m_UserData.m_byOtherOutPai;
	GameStationData.m_byThingDoing = m_GameData.m_byThingDoing;
	GameStationData.byNtStation = m_UserData.m_byNtStation;
	GameStationData.m_byNowOutStation = m_UserData.m_byNowOutStation;
	GameStationData.m_StrJing = m_UserData.m_StrJing;
	memcpy(GameStationData.m_byMenPaiCount,m_UserData.m_byMenPaiCount,sizeof(m_UserData.m_byMenPaiCount));//������������

	pDesk->GetUserAgree(GameStationData.ucAgree);//��ȡ������ҵ�ͬ��״̬

	GameStationData.m_byMaster = m_GameData.m_byMaster;//����λ��
	GameStationData.m_iBaseFen = m_GameData.m_iBaseFen;//������
	memcpy(GameStationData.m_bReBese,m_GameData.m_bReBese,sizeof(m_GameData.m_bReBese));//������������

	char path[500];
	sprintf(path,"%s%s_s.ini",CINIFile::GetAppPath (),SKIN_FOLDER);
	CINIFile pz(path);
	char key[500];
	sprintf(key,"game");
	m_GameData.m_MjSet.bAutoBegin	= pz.GetKeyVal(key,TEXT("bAutoBegin"),0);		//�Ƿ��Զ���ʼ
	m_GameData.m_MjSet.byAutoTime = pz.GetKeyVal(key,TEXT("byAutoTime"),3);			//�йܳ���ʱ��
	m_GameData.m_MjSet.byBeginTime = pz.GetKeyVal(key,TEXT("byBeginTime"),30);		//��ʼ�ȴ�ʱ��
	m_GameData.m_MjSet.byBlockTime = pz.GetKeyVal(key,TEXT("byBlockTime"),15);		//���Ƶȴ�ʱ��
	m_GameData.m_MjSet.byOutcardTime = pz.GetKeyVal(key,TEXT("byOutcardTime"),20);	//���Ƶȴ�ʱ��
	m_GameData.m_MjSet.byYaZhuTime = pz.GetKeyVal(key,TEXT("byYaZhuTime"),10);	//Ѻע�ȴ�ʱ��

	memcpy(&GameStationData.m_MjSet,&m_GameData.m_MjSet,sizeof(GameStationData.m_MjSet));//�齫���ã���Ҫ��ʱ�����ã�
	switch (GameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
		{
			//��������
			SendGameStation(bDeskStation, bWatchUser, &GameStationData, sizeof(GameStationData));
			MessageStruct Mess;
			ZeroMemory(Mess.Message, 200);
			lstrcpy(Mess.Message, TEXT("�gӭ�����錢�Α�,ף������_�ģ�"));
			SendGameData(bDeskStation, &Mess, sizeof(Mess), ASS_GM_GAME_NOTIFY, ASS_MESSAGE, 0);
		}
		break;
	case GS_WAIT_ARGEE:	//�ȴ���ҿ�ʼ״̬
	case GS_SEND_CARD:	//����״̬(��δ����)
	case GS_PLAY_GAME:	//��Ϸ��״̬
	case GS_WAIT_NEXT:	//�ȴ���һ����Ϸ��ʼ
		{
			//��������
			SendGameStation(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
		}
		break;
	}
	return true;
}
// ��Ϸ���ݰ�������
int CSrvLogicEx::HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser)
{
	CSrvLogic::HandleNotifyMessage(bDeskStation, code,pData, uSize, bWatchUser);
	if(code == EVENT_YA_ZHU)//Ѻע�¼�
	{
		ReceiveUserYaZhu(bDeskStation, pData, uSize, bWatchUser);
	}
	else if(code == EVENT_MAKE_DIFEN)//ȷ���׷��¼�
	{
		ReceiveMakeDiFen(bDeskStation, pData, uSize, bWatchUser);//
	}
	return 0;
}

// �û�����
int CSrvLogicEx::ReceiveUserOutCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return CSrvLogic::ReceiveUserOutCard(bDeskStation, pData, uSize,bWatchUser);
}

// �û�Ѻע
int CSrvLogicEx::ReceiveUserYaZhu(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	tagYaZhuEx * pCmd = (tagYaZhuEx *)pData;
	if(pCmd == NULL || bWatchUser || pDesk->GetGameStation() < GS_SEND_CARD || uSize!=sizeof(tagYaZhuEx) || m_GameData.m_byThingDoing != EVENT_YA_ZHU)
	{//���Ϸ�����
		OutputDebugString("������ �¼���Ѻע��Ϣ���Ϸ�");
		return 0;
	}
	if(m_GameData.T_YaZhu.byUserNote[bDeskStation] != 255)//��Ӧ����Ϣ�Ͳ��ٽ���
	{
		OutputDebugString("������ �¼���Ѻע��Ϣ�Ի�Ӧ");
		return 0;
	}

	if(bDeskStation == m_UserData.m_byNtStation)//ׯ�һ�Ӧ
	{	
		m_GameData.T_YaZhu.byUserNote[bDeskStation] = pCmd->byNote;
		m_UserData.m_byYaZhu = pCmd->byNtNote;
		m_GameData.T_YaZhu.byUserNote[bDeskStation] = pCmd->byNote;
		if( pCmd->byNtNote == 0)//ׯ�Ҳ�Ѻע
		{
			memset(m_UserData.m_bYaZhu,0,sizeof(m_UserData.m_bYaZhu));
			ApplyAction(EVENT_BEGIN_OUT,2*100);//������һ���¼�����ʼ����

			OutputDebugString("������ �¼���ׯ�Ҳ�Ѻע");
		}
		else
		{
			//֪ͨ�м�Ѻע
			tagYaZhuEx yazhu;
			yazhu.Clear();
			yazhu.byNotify = 1;
			yazhu.byNtNote = pCmd->byNtNote;
			//������Ϸ��Ϣ
			SendAllGameData( &yazhu, sizeof(yazhu), ASS_GM_GAME_NOTIFY, EVENT_YA_ZHU, 0);
			OutputDebugString("������ �¼���֪ͨ�м�Ѻע");
			this->AutoHandle(true);//��һ�ζ��ߴ���
		}
	}
	else 
	{
		m_GameData.T_YaZhu.byUserNote[bDeskStation] = pCmd->byNote;
		m_UserData.m_bYaZhu[bDeskStation] = pCmd->byNote;
		for(int i=0;i<PLAY_COUNT;++i)
		{
			if(m_GameData.T_YaZhu.byUserNote[i] == 255)//�����û�л�Ӧ
				return 0;
		}
		//������Ҷ���Ӧ��
		ApplyAction(EVENT_BEGIN_OUT,2*100);//������һ���¼�����ʼ����
	}
}

// ȷ���׷�
int CSrvLogicEx::ReceiveMakeDiFen(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)//
{
	tagFanDiFenEx * pCmd = (tagFanDiFenEx *)pData;
	if(pCmd == NULL || bWatchUser || pDesk->IsPlayGame(0) || uSize!=sizeof(tagFanDiFenEx) )
	{//���Ϸ�����
		OutputDebugString("������ �¼���ȷ���׷���Ϣ���Ϸ�");
		return 0;
	}
	if(m_GameData.m_iBaseFen > 0 && pCmd->byType == 0)//�������Ѿ������˵׷�
	{
		return 0;
	}
	if(pCmd->byType == 0)//���û�����
	{
		m_GameData.m_iBaseFen  = pCmd->iBaseFen;
		m_GameData.m_byMaster = bDeskStation;
		//���͸�����ҵ�ͬ��
		tagFanDiFenEx difen;
		difen.Clear();
		difen.byType = 1;
		difen.iBaseFen = pCmd->iBaseFen;
		difen.byMaster = bDeskStation;
		SendAllGameData(&difen,sizeof(tagFanDiFenEx),ASS_GM_GAME_NOTIFY,EVENT_MAKE_DIFEN,0);//����

		OutputDebugString("������ �¼���ȷ���׷� �ɹ� ");
	}
	m_GameData.m_bReBese[bDeskStation] = true;
}

// �û�������Ϣ
int CSrvLogicEx::ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	tagHuPaiEx * pHuCard = (tagHuPaiEx *)pData;
	if(pHuCard == NULL || bWatchUser || pDesk->GetGameStation() != GS_PLAY_GAME || uSize!=sizeof(tagHuPaiEx) || !m_GameData.T_CPGNotify[bDeskStation].bHu)
	{//���Ϸ�����
		return 0;
	}
	m_GameData.AcceptAction(pHuCard->byUser,ACTION_HU);//���ý��յĶ������ж���һ��������ʲôʱ��Ҫʹ�����õ�����

	m_GameData.T_HuPai.bZimo = m_GameData.T_CPGNotify[bDeskStation].bZimo;
	m_GameData.T_HuPai.byDianPao = m_GameData.T_CPGNotify[bDeskStation].m_byDianPao;
	m_GameData.T_HuPai.bCanH[pHuCard->byUser] = true;
	m_GameData.T_HuPai.byUserNum++;
	m_GameData.T_HuPai.byNt = m_UserData.m_byNtStation;		//ׯ��λ��
	m_GameData.T_HuPai.byPs = m_GameData.T_CPGNotify[bDeskStation].m_iHuPs;	//���ڵ���ֵ
	
	if(m_GameData.T_HuPai.byUser == 255)
	{
		m_GameData.T_HuPai.byUser = pHuCard->byUser;
	}
	else if(!m_GameData.T_HuPai.bZimo)//ѡ�񿿺�������������Һ���
	{//���ں��ƣ������ж����ҿ��Ժ���
		BYTE fanshu[4];
		memset(fanshu,0,sizeof(fanshu));
		for(int i=0;i<PLAY_COUNT;++i)
		{
			fanshu[i] = m_GameData.T_CPGNotify[i].m_byFan;
		}
		m_GameData.T_HuPai.byUser = CLogicBase::GetCanHuUser(m_GameData.T_CPGNotify[bDeskStation].m_byDianPao,m_GameData.T_HuPai.bCanH,m_GameData.T_HuPai.byUser,fanshu);//��ȡ�������ҽ������
	}

	memcpy(m_GameData.T_HuPai.byHuType[pHuCard->byUser],m_GameData.T_CPGNotify[pHuCard->byUser].byHuType,sizeof(m_GameData.T_CPGNotify[pHuCard->byUser].byHuType));//������������
	m_UserData.CopyAllGCPData(m_GameData.T_HuPai.m_UserGCPData);//��������������
	if(m_GameData.T_HuPai.bZimo)//���������������
	{
		ApplyAction(EVENT_HU_PAI,2*100);//������һ���¼�
	}
	else
	{
		//�����һ������
		CheckNextAction();
	}
	return 0;
}


///////////////////////////////�¼�ִ��/////////////////////////////////////////////////


///���ƣ�GameCPU
///��������Ϸ�¼��������ģ�
///@param typeҪ������¼�����
void CSrvLogicEx::GameCPU(int type)
{
	CSrvLogic::GameCPU(type);

	if(type == EVENT_YA_ZHU)//Ѻע�¼�
	{	
		char strdug[500];
		sprintf(strdug,"�¼���CSrvLogicEx::GameCPU type =%d ����λ�� %d",type,m_UserData.m_byNowOutStation);
		OutputDebugString(strdug);

		BYTE nextID = 255,time = 255;
		ExecuteYaZhu(nextID,time);

		m_GameData.ApplyThingID = nextID;
		ApplyAction(nextID,time*100);//������һ���¼�
	}

}

///��ʼ�¼�����
void CSrvLogicEx::ExecuteGameBegin(BYTE &nextID,BYTE &time)
{
	pDesk->KillTimer(TIME_WAIT_MSG);
	pDesk->SetTimer(TIME_WAIT_MSG,1000);//�������ó�ʱ��ʱ��

	if(m_UserData.m_byNtStation == 255)//��ׯ��
	{
		nextID = EVENT_DONG_NT;//�����ׯ��
	}
	else//��ׯ��,
	{
		nextID = EVENT_2SEZI_GP;//ɫ�Ӷ����ƶ���
	}

	time = 2;	//200������

	m_UserData.InitData();
	m_GameData.InitData();
	m_UserData.m_StrJing.AddJing(CMjEnum::MJ_TYPE_BAI);//�װ�Ϊ����

	m_GameData.T_Begin.byNt = m_UserData.m_byNtStation;			// ׯ��λ��			
	m_GameData.T_Begin.byPlayCount = m_UserData.byPlayCount;	// ��Ϸ���еľ���	
	m_GameData.T_Begin.byQuanFeng =  m_UserData.m_byQuanFeng;	// Ȧ������	

	//ȷ����λ���������� �̶�����
	m_UserData.m_byMenFeng[0] =0;//��λ�̶�����+
	m_GameData.T_Begin.m_byMenFeng[0] = 0;

	m_UserData.m_byMenFeng[1] =3;//��λ�̶�����+
	m_GameData.T_Begin.m_byMenFeng[1] = 3;

	m_UserData.m_byMenFeng[2] =2;//��λ�̶�����+
	m_GameData.T_Begin.m_byMenFeng[2] = 2;

	m_UserData.m_byMenFeng[3] =1;//��λ�̶�����+
	m_GameData.T_Begin.m_byMenFeng[3] = 1;

	BYTE count[4];
	memset(count,28,sizeof(count));//14��
	int index = rand()%4;
	count[index] = 26;//13��
	m_UserData.SetMenPaiCount(count);
	memcpy(m_GameData.T_Begin.m_byMenPaiCount,m_UserData.m_byMenPaiCount,sizeof(m_GameData.T_Begin.m_byMenPaiCount));
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<m_UserData.m_byMenPaiCount[i];++j)
		{
			m_GameData.T_Begin.byMenPai[i][j] = 0; //����
		}
	}
	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	m_UserData.m_bZhuaPaiDir = true;
	//���Ϳ�ʼ��Ϣ
	SendAllGameData(&m_GameData.T_Begin,sizeof(m_GameData.T_Begin),ASS_GM_GAME_NOTIFY,EVENT_BEGIN,0);
	//��ʱ���ã�
	SetDoingThing(EVENT_BEGIN,0);//�������ڷ������¼�
	SetGameState(STATE_BEGIN_OUT);//������Ϸ״̬
}

///��2��ɫ��ȷ��ׯ�ң������齫һ��ɫ�Ӷ�ׯ��
void CSrvLogicEx::ExecuteDongNt(BYTE &nextID,BYTE &time)
{	
	nextID = EVENT_2SEZI_GP;//��ׯ�����2��ɫ�ӷ��ƣ��Ͷ�����С�㶨���ƶ���
	time = 2;	//5����
	srand(GetTickCount());
	m_UserData.m_byNtStation = rand()%PLAY_COUNT;//ׯ��λ�ã����
	m_GameData.T_DongNt.byNt = m_UserData.m_byNtStation;
	m_UserData.m_MenPai.byGetPaiDir = m_UserData.m_byNtStation;//���Ʒ���(ÿ�ֳ�ʼ��Ϊׯ����ǰ����)

	//������Ϸ��Ϣ
	SendAllGameData( &m_GameData.T_DongNt, sizeof(m_GameData.T_DongNt), ASS_GM_GAME_NOTIFY, EVENT_DONG_NT, 0);
	//��ʱ����
	SetDoingThing(EVENT_DONG_NT,0);//�������ڷ������¼�
}

///��2��ɫ��ȷ�����ƶ����������Ͷ�����
void CSrvLogicEx::ExecuteTwoSeziGp(BYTE &nextID,BYTE &time)
{
	CSrvLogic::ExecuteTwoSeziGp(nextID,time);
}

///����
void CSrvLogicEx::ExecuteSendPai(BYTE &nextID,BYTE &time)	
{
	CSrvLogic::ExecuteSendPai(nextID,time);
	nextID = EVENT_YA_ZHU;//��һ���¼��Ĵ��ţ�Ѻע
	time = 20;//��2�뷢�ƶ���ʱ��
}

///��ʼ����֪ͨ�¼�
void CSrvLogicEx::ExecuteBeginOut(BYTE &nextID,BYTE &time)
{
	CSrvLogic::ExecuteBeginOut(nextID,time);
}

///ץ���¼�
void CSrvLogicEx::ExecuteZhuaPai(BYTE &nextID,BYTE &time)
{
	CSrvLogic::ExecuteZhuaPai(nextID,time);
}

//�����¼�
void CSrvLogicEx::ExecutePengPai(BYTE &nextID,BYTE &time)
{
	m_UserData.ReSetQiHu(m_GameData.temp_PengPai.byUser);//���������
	CSrvLogic::ExecutePengPai(nextID,time);
}

//�����¼�
void CSrvLogicEx::ExecuteGangPai(BYTE &nextID,BYTE &time)
{
	m_UserData.ReSetQiHu(m_GameData.temp_GangPai.byUser);//���������
	CSrvLogic::ExecuteGangPai(nextID,time);
}

//�����¼�
void CSrvLogicEx::ExecuteHuPai(BYTE &nextID,BYTE &time)
{
	CSrvLogic::ExecuteHuPai(nextID,time);
}

///����¼�
void CSrvLogicEx::ExecuteCountFen(BYTE &nextID,BYTE &time)
{
	CSrvLogic::ExecuteCountFen(nextID,time);
}

///������Ϸ�����¼�
void CSrvLogicEx::ExecuteNormalEnd(BYTE &nextID,BYTE &time)
{
	CSrvLogic::ExecuteNormalEnd(nextID,time);
}

///Ѻע�¼�
void CSrvLogicEx::ExecuteYaZhu(BYTE &nextID,BYTE &time)
{
	//����ׯ��Ѻע��Ϣ
	tagYaZhuEx yazhu;
	yazhu.Clear();
	yazhu.byNotify = 0;
	//������Ϸ��Ϣ
	SendAllGameData( &yazhu, sizeof(yazhu), ASS_GM_GAME_NOTIFY, EVENT_YA_ZHU, 0);
	//��ʱ����
	SetDoingThing(EVENT_YA_ZHU,0);//�������ڷ������¼�
}


///���ƣ�CheckAction �����ܶ������
///@param ��station ���λ��,pai �����Ҵ�����ƣ�type ���� ���� 0 ץ�ƣ�1�ܿ���2 ���ڣ�3����
///@return true ����������false ����������
bool CSrvLogicEx::CheckAction(int station,BYTE pai,BYTE type)
{
	for(int i=0;i<4;++i)
	{
		m_GameData.T_CPGNotify[i].Clear();//����
		m_GameData.T_CPGNotify[i].byOutStation = station;//��¼����λ��
	}
	bool action = false;
	switch(type)
	{
	case 0://����:�Լ�ץ�ƶ������
	case 1://�ܿ�:�������
		{
			m_CheckHuPai.CanHu(station,pai,m_GameData.T_CPGNotify[station],true,station);//�����
			m_CPGAction.CanGang(station,pai,m_GameData.T_CPGNotify[station],true);//�ܼ��
			m_CheckHuPai.CanTing(station,m_GameData.T_CPGNotify[station]);//�����
			if(m_GameData.T_CPGNotify[station].bHu || m_GameData.T_CPGNotify[station].bGang || m_GameData.T_CPGNotify[station].bTing)
			{
				m_GameData.T_CPGNotify[station].bZimo = true;
				action=true;
				m_GameData.T_CPGNotify[station].bCanAction = true;
				m_GameData.T_CPGNotify[station].byPs = pai;
			}
			//�ܺ��ƾ���������
			m_UserData.ReSetQiHu(station);//���������
			if(m_GameData.T_CPGNotify[station].bHu )
			{
				m_UserData.SetQiHu(station,m_GameData.T_CPGNotify[station].m_byFan);
			}
		}
		break;
	case 2://����:���˳��ƺ������
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(i==station || m_UserData.IsUserState(i,USER_STATE_CUT))//������Ҳ����
					continue;
				m_CPGAction.CanChi(i,pai,m_GameData.T_CPGNotify[i]);//�Լ��
				m_CPGAction.CanPeng(i,pai,m_GameData.T_CPGNotify[i]);//�����
				m_CPGAction.CanGang(i,pai,m_GameData.T_CPGNotify[i],false);//�ܼ��
				m_CheckHuPai.CanHu(i,pai,m_GameData.T_CPGNotify[i],false,station);//�����
				//���Ƽ������
				if(m_GameData.T_CPGNotify[i].bHu )
				{
					if(m_UserData.CheckQiHu(i,m_GameData.T_CPGNotify[i].m_byFan))
					{//�������ܺ���
						m_GameData.T_CPGNotify[i].bHu = false;
						m_GameData.T_CPGNotify[i].m_byFan = 0;
					}
					else
					{//������������
						m_UserData.SetQiHu(i,m_GameData.T_CPGNotify[i].m_byFan);
					}
				}
				if(m_GameData.T_CPGNotify[i].bHu || m_GameData.T_CPGNotify[i].bPeng || m_GameData.T_CPGNotify[i].bGang )
				{
					action=true;
					m_GameData.T_CPGNotify[i].bZimo = false;
					m_GameData.T_CPGNotify[i].byPs = pai;
					m_GameData.T_CPGNotify[i].bCanAction = true;
				}
			}
		}
		break;
	case 3://���ܣ����˲���ʱ�������
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(i==station || m_UserData.IsUserState(i,USER_STATE_CUT))//������Ҳ����
					continue;
				m_CheckHuPai.CanHu(i,pai,m_GameData.T_CPGNotify[i],false,station);
				if(m_GameData.T_CPGNotify[i].bHu)
				{
					if(m_UserData.CheckQiHu(i,m_GameData.T_CPGNotify[i].m_byFan))
					{//�������ܺ���
						m_GameData.T_CPGNotify[i].bHu = false;
						m_GameData.T_CPGNotify[i].m_byFan = 0;
					}
					else
					{//������������
						m_UserData.SetQiHu(i,m_GameData.T_CPGNotify[i].m_byFan);
					}
				}
				if(m_GameData.T_CPGNotify[i].bHu)//��
				{
					action=true;
					m_GameData.T_CPGNotify[i].byPs = pai;
					m_GameData.T_CPGNotify[i].bZimo = false;
					m_GameData.T_CPGNotify[i].bCanAction = true;
				}
			}
		}
		break;
	}
	return action;
}


//�������Զ�����2������û���յ��κ���Ϸ��Ϣ����Ϊ����Ϸ���������ñ����������ݵ�ǰִ���¼�����ҵ�״̬�����Զ������ָ���Ϸ����
void CSrvLogicEx::AutoHandle(bool bNetCut)
{
	CSrvLogic::AutoHandle( bNetCut);
	if(m_GameData.m_byThingDoing == EVENT_YA_ZHU)//Ѻע�¼�����ʱ
	{
		if(!bNetCut)
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(m_GameData.T_YaZhu.byUserNote[i] == 255)//�����û�л�Ӧ
				{
					m_UserData.m_bYaZhu[i] = false;
				}
			}
			ApplyAction(EVENT_BEGIN_OUT,20);//��������¼�
		}
		else//���ߴ���
		{
			if(m_UserData.IsUserState(m_UserData.m_byNtStation,USER_STATE_CUT))//ׯ�Ҷ���
			{
				if(m_UserData.m_byYaZhu == 255)//ׯ��û�л�Ӧ
				{//ģ��һ��������Ϣ
					tagYaZhuEx yazhu;
					yazhu.Clear();
					yazhu.byNote = 0;
					yazhu.byNtNote = 0;
					ReceiveUserYaZhu(m_UserData.m_byNtStation,&yazhu,sizeof(tagYaZhuEx),false);
				}
			}
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(i == m_UserData.m_byNtStation)
					continue;
				if(!m_UserData.IsUserState(i,USER_STATE_CUT))
					continue;
				tagYaZhuEx yazhu;
				yazhu.Clear();
				yazhu.byNote = 0;
				ReceiveUserYaZhu(i,&yazhu,sizeof(tagYaZhuEx),false);
			}
		}
	}
}

///�Զ�������ƻ���ץ��
void CSrvLogicEx::AutoOutOrGet()
{
	bool haveout = false;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(!m_UserData.IsOutPaiPeople(i))//�������
			continue;
		haveout = true;
		tagOutPaiEx outpai;
		outpai.Clear();
		outpai.byUser = i;
		outpai.byPs = m_UserData.GetLastHandPai(i);
		if(outpai.byPs == CMjEnum::MJ_TYPE_BAI)//�װ��ǲ����ܴ��
		{
			for(int j=0;j<m_UserData.GetHandPaiCount(i)-1;++j)
			{
				if(m_UserData.GetHandPaiFromIndex(i,j) !=  CMjEnum::MJ_TYPE_BAI)
				{
					outpai.byPs = m_UserData.GetHandPaiFromIndex(i,j);
					ReceiveUserOutCard(i,&outpai,sizeof(outpai),false);
					return;
				}
			}
		}
		ReceiveUserOutCard(i,&outpai,sizeof(outpai),false);
		break;
	}
	if(!haveout)//û�г��Ƶ���ң�����ץ���¼�
	{
		ApplyAction(EVENT_ZHUA_PAI,200);//������һ���¼�
	}
}

/// �������
void CSrvLogicEx::UserSitDesk(BYTE bDeskStation)
{
	//֪ͨ������õ׷�
	if(m_GameData.m_iBaseFen <= 0)//û�е׷�
	{
		//���͸�����ҵ�ͬ��
		tagFanDiFenEx difen;
		difen.Clear();
		difen.byType = 0;
		difen.iBaseFen = m_GameData.m_iBaseFen;
		difen.byMaster = m_GameData.m_byMaster;
		//������Ϸ��Ϣ
		SendGameData( bDeskStation,&difen,sizeof(difen), ASS_GM_GAME_NOTIFY, EVENT_MAKE_DIFEN, 0);
	}
}

/// ���õ�ǰ���ڷ������¼���ͬʱ����һ���ȴ�ʱ�䣬����ʱ��ͻ���û����Ӧ���Զ�����
void CSrvLogicEx::SetDoingThing(BYTE thingId,BYTE st1,BYTE st2,BYTE pai,BYTE data)
{
	CSrvLogic::SetDoingThing(thingId, st1, st2, pai, data);
	if(thingId == EVENT_YA_ZHU)//Ѻע�¼�
	{
		m_GameData.SetOutTime(st1,m_GameData.m_MjSet.byYaZhuTime *2,thingId,true);
	}
}