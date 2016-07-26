#include "stdafx.h"
//#include "../Client/stdafx.h"
#include "CGameData.h"

CGameData::CGameData(void)
{
	///����Ҫ�۵ķ���
	uRunPublish = 0;					
	///����ı���
	basepoint = 0;	
	///Ҫ������¼�id
	ApplyThingID = 255;
	m_byApplyMaxAction = 0;//��ҵ�ǰ���������¼�
	memset(m_byThingNext,255,sizeof(m_byThingNext));
	m_bShowUserId = false;				//�Ƿ���ʾ������ҵ�id
	InitData();
	LoadIni();
}

CGameData::~CGameData(void)
{

}


///���������¼��ĺ���¼�
void CGameData::SetThingNext()
{
	//��Ϸ��ʼ�¼�
	T_Begin.byNext = m_byThingNext[EVENT_BEGIN];
	//�׾��Զ�Ϊׯ�¼�	
	T_DongNt.byNext = m_byThingNext[EVENT_DONG_NT];
	//�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�	
	T_TwoSeziNt.byNext = m_byThingNext[EVENT_2SEZI_NT];
	//�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
	T_TwoSeziNtAndGetPai.byNext = m_byThingNext[EVENT_2SEZI_NT_GP];
	//�׾�������2��ɫ�Ӷ�ׯ���¼�	
	T_TurnSeziNt.byNext = m_byThingNext[EVENT_TURN_2SEZI_NT];
	//��2��ɫ���¼������Ʒ����¼�
	T_TwoSeziDir.byNext = m_byThingNext[EVENT_2SEZI_DIR];
	//��2��ɫ���¼������Ʒ�������Ƶ��¼�
	T_TwoSeziDirAndGetPai.byNext = m_byThingNext[EVENT_2SEZI_DIR_GP];
	//��2��ɫ���¼������Ƶ��¼�	
	T_TwoSeziGetPai.byNext = m_byThingNext[EVENT_2SEZI_GP];
//����ǰ�¼�
	//�����¼�	
	T_SendPai.byNext = m_byThingNext[EVENT_SEND_PAI];
	//��2��ɫ�Ӷ������¼�
	T_TwoSeziJing.byNext = m_byThingNext[EVENT_2SEZI_JING];
	//Ϊ������Ҳ����¼�
	T_AllBuHua.byNext = m_byThingNext[EVENT_ALL_BUHUA];
	//������Ҳ����¼�	
	T_OneBuHua.byNext = m_byThingNext[EVENT_ONE_BUHUA];
//���������¼�
	//ׯ�ҿ�ʼ����֪ͨ�¼�
	T_BeginOutPai.byNext = m_byThingNext[EVENT_BEGIN_OUT];
	//�����¼�
	T_OutPai.byNext = m_byThingNext[EVENT_OUT_PAI];
	//ץ���¼�
	T_ZhuaPai.byNext = m_byThingNext[EVENT_ZHUA_PAI];
	//�����¼�
	T_ChiPai.byNext = m_byThingNext[EVENT_CHI_PAI];
	//�����¼�
	T_PengPai.byNext = m_byThingNext[EVENT_PENG_PAI];
	//�����¼�
	T_GangPai.byNext = m_byThingNext[EVENT_GANG_PAI];
	//�����¼�
	T_TingPai.byNext = m_byThingNext[EVENT_TING_PAI];
//���Ƽ����ƺ��¼�
	//���¼�
	T_HuPai.byNext = m_byThingNext[EVENT_HU_PAI];
	//����¼�	
	T_CountFen.byNext = m_byThingNext[EVENT_COUNT_FEN];
	//���������¼�
	T_EndHandle.byNext = m_byThingNext[EVENT_END_HANDLE];
	//���������������¼�
	T_UnnormalEndHandle.byNext = m_byThingNext[EVENT_ENG_UN_NORMAL];
}
///����ĳ���¼��ĺ���¼�
void CGameData::SetThingNext(BYTE id)
{
	switch(id)
	{
//����ǰ�¼�
	case EVENT_BEGIN:			//��Ϸ��ʼ�¼�
		T_Begin.byNext = m_byThingNext[id];
		break;
	case EVENT_DONG_NT:			//�׾��Զ�Ϊׯ�¼�	
		T_DongNt.byNext = m_byThingNext[id];
		break;
	case EVENT_2SEZI_NT:		//�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�	
		T_TwoSeziNt.byNext = m_byThingNext[id];
		break;
	case EVENT_2SEZI_NT_GP:		//�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
		T_TwoSeziNtAndGetPai.byNext = m_byThingNext[id];
		break;
	case EVENT_TURN_2SEZI_NT:	//�׾�������2��ɫ�Ӷ�ׯ���¼�	
		T_TurnSeziNt.byNext = m_byThingNext[id];
		break;
	case EVENT_2SEZI_DIR:		//��2��ɫ���¼������Ʒ����¼�
		T_TwoSeziDir.byNext = m_byThingNext[id];
		break;
	case EVENT_2SEZI_DIR_GP:	//��2��ɫ���¼������Ʒ�������Ƶ��¼�
		T_TwoSeziDirAndGetPai.byNext = m_byThingNext[id];
		break;
	case EVENT_2SEZI_GP:		//��2��ɫ���¼������Ƶ��¼�	
		T_TwoSeziGetPai.byNext = m_byThingNext[id];
		break;
//����ǰ�¼�
	case EVENT_SEND_PAI:		//�����¼�	
		T_SendPai.byNext = m_byThingNext[id];
		break;
	case EVENT_2SEZI_JING:		//��2��ɫ�Ӷ������¼�
		T_TwoSeziJing.byNext = m_byThingNext[id];
		break;
	case EVENT_ALL_BUHUA:		//Ϊ������Ҳ����¼�
		T_AllBuHua.byNext = m_byThingNext[id];
		break;
	case EVENT_ONE_BUHUA:		//������Ҳ����¼�	
		T_OneBuHua.byNext = m_byThingNext[id];
		break;
//���������¼�
	case EVENT_BEGIN_OUT:		//ׯ�ҿ�ʼ����֪ͨ�¼�
		T_BeginOutPai.byNext = m_byThingNext[id];
		break;
	case EVENT_OUT_PAI:			//�����¼�
		T_OutPai.byNext =		m_byThingNext[id];
		break;
	case EVENT_ZHUA_PAI:		//ץ���¼�
		T_ZhuaPai.byNext = m_byThingNext[id];
		break;
	case EVENT_CPG_NOTIFY:		//�ܳ���֪ͨ�¼��¼�
		break;
	case EVENT_CHI_PAI:			//�����¼�
		T_ChiPai.byNext = m_byThingNext[id];
		break;
	case EVENT_PENG_PAI:		//�����¼�
		T_PengPai.byNext = m_byThingNext[id];
		break;
	case EVENT_GANG_PAI:		//�����¼�
		T_GangPai.byNext = m_byThingNext[id];
		break;
	case EVENT_TING_PAI:		//�����¼�
		T_TingPai.byNext = m_byThingNext[id];
		break;
//���������¼�
//���Ƽ����ƺ��¼�
	case EVENT_HU_PAI:			//���¼�
		T_HuPai.byNext = m_byThingNext[id];
		break;
	case EVENT_COUNT_FEN:		//����¼�	
		T_CountFen.byNext = m_byThingNext[id];
		break;
	case EVENT_END_HANDLE:		//���������¼�
		T_EndHandle.byNext = m_byThingNext[id];
		break;
	case EVENT_ENG_UN_NORMAL:	//���������������¼�
		T_UnnormalEndHandle.byNext = m_byThingNext[id];
		break;
	}
}

///��ʼ������
void CGameData::InitData()
{
	m_byApplyMaxAction = 0;//��ҵ�ǰ���������¼�
	//�¼���¼ֻ��¼��10���¼���0�¼��ţ�1�����ֵ��2���λ��1,3���λ��2��4��������
	memset(m_byThingLog,255,sizeof(m_byThingLog));
	///��Ϸ��ǰ״̬������ǰ״̬������״̬������״̬������״̬����Ϸ����״̬������״̬
	m_byGameState = STATE_BEGIN_OUT;  
	///��ִ���¼�
	m_byNextAction = ACTION_NO;	
	///���ڷ������¼���
	m_byThingDoing = 255;  
	///�������¼�,��ʱ����
	temp_ChiPai.Clear();
	///�������¼�,��ʱ����
	temp_PengPai.Clear();
	///�������¼�,��ʱ����
	temp_GangPai.Clear();
	///��Ϸ��ʼ�¼�
	T_Begin.Clear();
	///�Զ�Ϊׯ�¼�
	T_DongNt.Clear();
	///��2��ɫ�ӵĵ�����Ϊׯ�¼�
	T_TwoSeziNt.Clear();
	///��2��ɫ��ȷ��ׯ�Һ����Ƶ�λ���¼�
	T_TwoSeziNtAndGetPai.Clear();
	///������2��ɫ��ȷ��ׯ��
	T_TurnSeziNt.Clear();
	///��2��ɫ��ȷ������λ���¼�
	T_TwoSeziDir.Clear();
	///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
	T_TwoSeziDirAndGetPai.Clear();
	///��2��ɫ��ȷ�����ƶ����¼�
	T_TwoSeziGetPai.Clear();
	///�����¼�
	T_SendPai.Clear();
	///��ɫ��2��ɫ�Ӷ������¼�
	T_TwoSeziJing.Clear();
	///������Ҳ����¼�
	T_AllBuHua.Clear();
	///������Ҳ����¼�
	T_OneBuHua.Clear();
	///��ʼ����֪ͨ�¼�
	T_BeginOutPai.Clear();
	///�����¼�
	T_OutPai.Clear();
	///ץ�����¼�
	T_ZhuaPai.Clear();
	/////�����ܺ���֪ͨ�¼����¼�
	//T_CPGNotify[4];

	///�������¼�
	T_ChiPai.Clear();
	///�������¼�
	T_PengPai.Clear();
	///�������¼�
	T_GangPai.Clear();
	///�������¼�
	T_TingPai.Clear();
	///�������¼�
	T_HuPai.Clear();
	///����¼�
	T_CountFen.Clear();
	///��Ϸ���������¼�
	T_EndHandle.Clear();
	///���������������¼�
	T_UnnormalEndHandle.Clear();

	SetThingNext();

}

///���������ļ�
void CGameData::LoadIni()
{
	m_mjRule.byAllMjCount   = 136;		//�齫������ 
	m_mjRule.bHaveFlower	= 0;		//�Ƿ��л���
	m_mjRule.bGangZhong		= 0;		//�Ƿ�ܺ���
	m_mjRule.bGangFa		= 0;		//�Ƿ�ܷ���
	m_mjRule.bHaveFlower	= 0;		//�Ƿ�ܰװ�
	m_mjRule.bHaveHongZhong	= 1;		//�Ƿ��к���
	m_mjRule.bHaveFaCai		= 1;		//�Ƿ��з���
	m_mjRule.bHaveBaiBan	= 1;		//�Ƿ��аװ�
	m_mjRule.bHaveFengPai	= 1;		//�Ƿ��ж�������
	m_mjRule.bHaveFlower	= 0;		//�Ƿ��л���
	m_mjRule.byHandCardNum	= 13;		//��������
	m_mjRule.byGamePassNum	= 0;		//��������
	m_mjRule.byNextNtType	= 0;		//�¾�ׯ�ҵ�ȷ������
	m_mjRule.bHaveJing		= 0;		//���޲���

	m_mjRule.bHaveQFeng		= 0;		//�Ƿ���Ȧ��
	m_mjRule.byMenPaiNum[0]	= 17;		//����������
	m_mjRule.byMenPaiNum[1]	= 17;		//����������
	m_mjRule.byMenPaiNum[2]	= 17;		//����������
	m_mjRule.byMenPaiNum[3]	= 17;		//����������
	m_mjRule.byOtherJing	= 0;		//��������

	m_mjAction.bChi				= 1;	//��
	m_mjAction.bChiFeng			= 0;	//�Է���
	m_mjAction.bChiJian			= 0;	//���з���
	m_mjAction.bPeng			= 1;	//��
	m_mjAction.bGang			= 1;	//��
	m_mjAction.bBuGang			= 1;	//����
	m_mjAction.bTing			= 0;	//��
	m_mjAction.bQiangChi		= 0;	//����(�Ժ�����)

//
//#pragma warning(push)
//#pragma warning(disable:4800)
//	Tstring s = CINIFile::GetAppPath ();/////����·��
//
//	CINIFile pz(s + SKIN_FOLDER  + _T("_s.ini"));
//
//	Tstring key = TEXT("game");// �齫��������
//
//
//	m_mjRule.bNetCutTuoGuan	= pz.GetKeyVal(key,TEXT("bNetCutTuoGuan"),1);		//�Ƿ�����й�
//	m_mjRule.bForceTuoGuan	= pz.GetKeyVal(key,TEXT("bForceTuoGuan"),1);		//�Ƿ�ǿ���й�
//	m_bShowUserId =			pz.GetKeyVal(key,TEXT("bShowUserId"),0);			//�Ƿ���ʾ������ҵ�id
//
//
//	CINIFile f(s+_T("\\mj\\") +SKIN_FOLDER  + _T("_s_Rule.ini"));
//
//
//	key = TEXT("BaseRule");// �齫��������


//#pragma warning(pop)
}


//����������
BYTE CGameData::LoadPaiData(MjMenPaiData & menpai)
{
	BYTE index=0;
	memset(menpai.byMenPai,255,sizeof(menpai.byMenPai));//
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<27;j++)
		{
			menpai.byMenPai[index] = stcArr_A_Mj_WTT[j];
			index++;
		}
		if(m_mjRule.bHaveBaiBan)//�аװ�
		{
			menpai.byMenPai[index] = stcArr_A_Mj_ZI[2];
			index++;
		}
		if(m_mjRule.bHaveFaCai)//�з���
		{
			menpai.byMenPai[index] = stcArr_A_Mj_ZI[1];
			index++;
		}
		if(m_mjRule.bHaveHongZhong)//�к���
		{
			menpai.byMenPai[index] = stcArr_A_Mj_ZI[0];
			index++;
		}
		if(m_mjRule.bHaveFengPai)//�ж�������
		{
			for(int j=0;j<4;j++)
			{
				menpai.byMenPai[index] = stcArr_A_Mj_FENG[j];
				index++;
			}
		}
	}
	if(m_mjRule.bHaveFlower)//�л���
	{
		for(int i=0;i<8;i++)
		{
			menpai.byMenPai[index] = stcArr_A_Mj_Other[i];
			index++;
		}
	}
	menpai.byAllPaiNum = index;
	return menpai.byAllPaiNum;
}

///�յ���ҵĶ�������
void CGameData::AcceptAction(BYTE station,BYTE type)
{
	if(station<0 || station>=PLAY_COUNT || type<ACTION_NO || type>ACTION_HU)//���Ϸ�������
		return;
	T_CPGNotify[station].bHaveReturn = true;//��¼��һ�Ӧ
	T_CPGNotify[station].m_byMaxThing = type;//�ı���������������
	if(m_byNextAction < type || m_byNextAction == 255)
	{
		m_byNextAction = type;
	}
	if(m_byApplyMaxAction < type|| m_byApplyMaxAction == 255)
	{
		m_byApplyMaxAction = type;//��ҵ�ǰ���������¼�
	}
}


///��ȡ��������
BYTE CGameData::GetMaxRequet(BYTE station)
{
	BYTE max = 0;
	for(int i=0;i<4;++i)//���ҳ�����¼�
	{
		if(i==station || T_CPGNotify[i].m_byMaxThing <= max || T_CPGNotify[i].m_byMaxThing==255)
			continue;
		max = T_CPGNotify[i].m_byMaxThing;
	}
	return max;
}

///ָ�������Ƿ�ȫ����Ӧ
bool CGameData::IsNotifyAllReply(BYTE station,BYTE request)
{
	BYTE num = 0,re=0;
	for(int i=0;i<PLAY_COUNT;++i)//����¼�������������¼���Ӧ�ĸ���
	{
		if(i==station)
			continue;
		if(request == T_CPGNotify[i].m_byMaxThing && T_CPGNotify[i].m_byMaxThing!=255)
		{
			num++;
			if(T_CPGNotify[i].bHaveReturn)
				re++;
		}
	}
	return num==re;
}

/// ���ó�ʱ����
void CGameData::SetOutTime(BYTE station,BYTE time,BYTE thing,bool effect)
{
	m_MjWait.byWaitThing = thing;
	m_MjWait.byCurChair = station;
	m_MjWait.byTime = time + 2;
	m_MjWait.bEffect = effect;
}
/// �ָ���ʱ����
void CGameData::ReSetOutTime()
{
	m_MjWait.Clear();
}


/// �����ҵĳ�������������Ե���
int CGameData::GetUserChiCount(BYTE station,BYTE handpai[])
{
	int count = 0,num=0;
	if(!T_CPGNotify[station].bChi)
		return 0;
	for(int i=0;i<3;++i)
	{	
		if( T_CPGNotify[station].m_iChi[i][0]== 255)
			continue;
		count++;
		for(int j=0;j<3;++j)
		{
			handpai[num] = T_CPGNotify[station].m_iChi[i][j];
			num++;
		}
	}
	return count;
}

/// �����ҵĸ������������ܸܵ���
int CGameData::GetUserGangCount(BYTE station,BYTE handpai[])
{
	int count = 0,num=0;
	BYTE tmp = 255;
	if(!T_CPGNotify[station].bGang)
		return 0;
	for(int i=0;i<4;++i)
	{	
		if(T_CPGNotify[station].m_iGangData[i][1]== 255)
			continue;
		count++;
		handpai[num] = T_CPGNotify[station].m_iGangData[i][1];
		num++;
	}
	return count;
}

//����ܸ��Ƶ�����
int CGameData::GetCanGangCount(BYTE station)
{
	int count =0;
	for(int i=0;i<4;++i)
	{	
		if(T_CPGNotify[station].m_iGangData[i][1]== 255)
			continue;
		count++;
	}
	return count;
}
//����ܳ��Ƶ�����
int CGameData::GetCanChiCount(BYTE station)
{
	int count =0;
	for(int i=0;i<3;++i)
	{	
		if(T_CPGNotify[station].m_iChi[i][0]== 255)
			continue;
		count++;
	}
	return count;
}


///������ʱ����
void CGameData::ClearTempData()
{
	//�������¼�,��ʱ����
	temp_ChiPai.Clear();
	//�������¼�,��ʱ����
	temp_PengPai.Clear();
	//�������¼�,��ʱ����
	temp_GangPai.Clear();	
	//�����������
	T_GangPai.Clear();	
}






