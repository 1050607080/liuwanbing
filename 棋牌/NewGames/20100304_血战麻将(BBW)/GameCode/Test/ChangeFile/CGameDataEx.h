#pragma once
//#include "../ChangeFile/UpgradeMessageEx.h"
#include "../GameMessage/CGameData.h"

///��Ϸ���ݹ�����
class CGameDataEx: public CGameData
{
public:
	CGameDataEx(void);
	~CGameDataEx(void);
	
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
	void LoadRoomIni(int id);
	//����������
	virtual BYTE LoadPaiData(MjMenPaiData & menpai);

	void ClearBaseFen();//��ջ�������Ϣ

	///�����¼����ݰ�
	tagFanDiFenEx  T_DiFen;
	//Ѻע���ݰ�
	tagYaZhuEx     T_YaZhu;

	//��Ϸ�׷�
	int				m_iBaseFen;
	BYTE			m_byMaster;//����
	bool			m_bReBese[PLAY_COUNT];//�Ƿ��Ӧ�˻���������
	
	
};
