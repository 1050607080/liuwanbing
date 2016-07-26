#include "stdafx.h"
//#include "../Client/stdafx.h"
#include "GameDataEx.h"

//ȫ�ֶ���
GameDataEx g_GameData;

GameDataEx::GameDataEx(void)
{
	GameData::GameData();


    for (int i=0; i<PLAY_COUNT; ++i)
    {
        T_HuPaiXZMJ[i].Clear();
        T_CountFenXZMJ[i].Clear();
    }
	m_byFengDing = 20;//�ⶥ����Ĭ��3��
	m_bShowTax = 0;//�Ƿ���ʾ̨��
	m_iBaseFen = 1;//��Ϸ�����֣�Ĭ��Ϊ1
	m_bForceFinish = false;//�Ƿ�ǿ�˽����汾
	LoadIni();
	m_bChengBao = false;//�ֳа��ƺͼҼ���
}

GameDataEx::~GameDataEx(void)
{
	GameData::~GameData();
}

	  
///���������¼��ĺ���¼�
void GameDataEx::SetThingNext()
{
	GameData::SetThingNext();
	m_bChengBao = true;//�ֳа��ƺͼҼ���
}

///����ĳ���¼��ĺ���¼�
void GameDataEx::SetThingNext(BYTE id)
{
	GameData::SetThingNext(id);
}

//virtual ��ʼ������
void GameDataEx::InitData()
{
    memset(m_byThingRecord,255,sizeof(m_byThingRecord));
    ///��ִ���¼�
    m_byNextAction = ACTION_NO;
    ///�ϴη������¼���
    m_byThingLast = 255;		
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

    ///�����¼�
    T_TiaoPai.Clear();

    ///��ɫ��2��ɫ�Ӷ������¼�
    T_TwoSeziJing.Clear();

    ///��ɫ��1��ɫ�Ӷ������¼�
    T_OneSeziJin.Clear();

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
    T_KanPai.Clear();

    ///ɨ���¼�
    T_SaoHu.Clear();

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

    for (int i=0; i<PLAY_COUNT; ++i)
    {
        T_HuPaiXZMJ[i].Clear();
        T_CountFenXZMJ[i].Clear();
    }

	//��ȱ�¼�
	T_DingQue.Clear();
}

///��ʼ������
void GameDataEx::LoadIni()
{
	GameData::LoadIni();
	m_mjAction.bTing	= 0;			//��
	//m_mjRule.byGamePassNum	= 50;		//��������,����
	m_mjRule.byNextNtType	= 1;	//��ׯģʽ��ׯ����������ׯ���к��¼���ׯ
}

///��ʼ��������������
void GameDataEx::LoadRoomIni(int id)
{
	//m_byFengDing = ;//�ⶥ����Ĭ��3��

 //   Tstring s = CINIFile::GetAppPath ();/////����·��    
	//CINIFile f(s +_T("\\")+SKIN_FOLDER  + _T("_s.ini"));
	//m_byFengDing = f.GetKeyVal("test","baopai",pDesk->sUserData.m_StrJing.byPai[0]);

}

MJ_Setting &GameDataEx::GetMjSetting()
{
    return m_MjSet;
}