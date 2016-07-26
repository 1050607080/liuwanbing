#pragma once
//#include "../ChangeFile/UpgradeMessageEx.h"
#include "../GameMessage/GameData.h"

///��Ϸ���ݹ�����
class GameDataEx: public GameData
{
public:
	GameDataEx(void);
	~GameDataEx(void);
	
public:	

public:
	///���������¼��ĺ���¼�
	virtual void SetThingNext();
	///����ĳ���¼��ĺ���¼�
	virtual void SetThingNext(BYTE id);
	//virtual ��ʼ������
	virtual void InitData();
	///��ʼ������
	virtual void LoadIni();

	///��ʼ��������������
	void	LoadRoomIni(int id);

	bool	m_bChengBao;//�ֳа��ƺͼҼ���

	bool	m_bShowTax;//�Ƿ���ʾ̨��


    MJ_Setting &GetMjSetting();

	tagDingQueEx			T_DingQue;  //��ȱ�¼�

    ///�������¼�
    tagHuPaiEx				T_HuPaiXZMJ[PLAY_COUNT];        // �ĸ���ң����и��ĺ����¼�
    ///����¼�
    tagCountFenEx			T_CountFenXZMJ[PLAY_COUNT];     // �ĸ���ң����и�������¼�

	BYTE					m_byFengDing;	//�ⶥ����Ĭ��3��
	int						m_iBaseFen;		//��Ϸ�����֣�Ĭ��Ϊ1
	bool					m_bForceFinish;	//�Ƿ�ǿ�˽����汾


    MJ_Setting      m_MjSet;       // �齫����
	///�齫ʱ��ȴ���ʱ�䵽�˷�������Ϊ����
	MJ_WaitTimeEx   m_MjWait;
private:

};

//ȫ�ֶ���
extern GameDataEx g_GameData;