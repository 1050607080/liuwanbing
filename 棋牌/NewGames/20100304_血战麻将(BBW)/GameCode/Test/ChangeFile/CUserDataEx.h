#pragma once
//#include "../ChangeFile/UpgradeMessageEx.h"
#include "../GameMessage/CUserData.h"

///������ݹ�����
class CUserDataEx: public CUserData
{
public:
	CUserDataEx(void);
	~CUserDataEx(void);
	
public:	
	
	//ׯ��Ѻע�����255δ������Ѻע��0��Ѻע��1ѹһ�룬2ѹȫ��
	BYTE	m_byYaZhu;
	//�м�ͬ��Ѻע���
	bool	m_bYaZhu[PLAY_COUNT];
	//��������ķ���
	BYTE	m_byQiHuFan[PLAY_COUNT];
	
	//�ĸ��������Ƶ�����
	BYTE		m_byMenPaiCount[4];

public:
	
	//////////////////���Ʋ���/////////////////////////////////////

	///����Ƿ����ָ��������
	virtual bool IsHaveAHandPai(BYTE station,BYTE pai);
	///���������ĳ���Ƶĸ���
	virtual int GetAHandPaiCount(BYTE station,BYTE pai);
	///������Ƶ�����
	virtual int GetHandPaiCount(BYTE station);
	///������ҵ�����
	virtual int CopyAllHandPai(BYTE pai[][HAND_CARD_NUM],BYTE station,bool show);
	///����������ĳ����ҵ�����
	virtual int CopyOneUserHandPai(BYTE pai[] ,BYTE station);
	///����������ĳ����ҵĲ�������
	virtual int CopyOneUserHandPai(BYTE pai[] ,BYTE station, BYTE size);
	///��������small�Ƿ��С������
	virtual void SortHandPai(BYTE station,bool big);
	///���һ���Ƶ�������
	virtual void AddToHandPai(BYTE station,BYTE pai);
	///������������
	virtual void SetHandPaiData(BYTE station,BYTE pai[]);
	///ɾ��һ��ָ��������
	virtual void DelleteAHandPai(BYTE station,BYTE pai);
	///���������Ʊ�
	virtual void SetHandPaiBack(BYTE station,BYTE num);
	///������һ������
	virtual BYTE GetLastHandPai(BYTE station);
	///���ѡ�е�һ������
	virtual BYTE GetSelectHandPai(BYTE station);
	///�Ƿ�������
	virtual bool IsOutPaiPeople(BYTE station);
	///����������Ƿ���ڻ���
	virtual bool IsHuaPaiInHand(BYTE station);
	///�������еĻ����Ƶ�����������
	virtual int MoveHuaPaiFormHand(BYTE station);


	//////////////////�������Ʋ���/////////////////////////////////////

	///��ȡ��Ҹ��Ƶĸ���(���ܣ����ܣ�����)
	virtual int GetUserGangNum(BYTE station,BYTE &an,BYTE &ming , BYTE &bu);
	///��ҳ��ƴ���
	virtual int GetUserChiNum(BYTE station);
	///��Ҹ��ƴ���
	virtual int GetUserPengNum(BYTE station);
	///��Ҹ���ĳ����
	virtual bool IsUserHaveGangPai(BYTE station,BYTE pai,BYTE &type);
	///����Ƿ�����ĳ����
	virtual bool IsUserHavePengPai(BYTE station,BYTE pai);
	///����Ƿ�Թ�ĳ����
	virtual bool IsUserHaveChiPai(BYTE station,BYTE pai);
	///���һ�����ݵ��ܳ���������
	virtual void AddToGCP(BYTE station,GCPStructEx *gcpData);
	///ɾ���ճ���������ָ����������
	virtual void DelleteAGCPData(BYTE station,BYTE type ,BYTE pai);
	///�����ܳ��������е�������
	virtual int CopyGCPData(BYTE station,GCPStructEx gcpData[]);
	///���ó��������е�������
	virtual void SetGCPData(BYTE station,GCPStructEx gcpData[]);
	///��ȡ�ܳ����Ľڵ����
	virtual int GetGCPCount(BYTE station);
	///��������ȡ�ܳ�������ĳ���Ƶ�����
	virtual int GetOnePaiGCPCount(BYTE station,BYTE pai);
	///����ܳ�������
	virtual void ShortGCP(BYTE station);
	///��������ʼ���ܳ�������
	virtual void InitGCP(BYTE station);


	///////////////////���Ʋ���///////////////////////////////////////////////////////

	///���һ���Ƶ�����������
	virtual void AddToOutPai(BYTE station,BYTE pai);
	///ɾ�����һ�ų���
	virtual void DelleteLastOutPai(BYTE station);
	///���ó�������
	virtual void SetOutPaiData(BYTE station,BYTE pai[]);
	///������ҵĳ���
	virtual int CopyOutPai(BYTE station,BYTE pai[]);
	///��ȡ��ҵĳ�������
	virtual int GetOutPaiCount(BYTE station);
	///��������ȡ���ĳ���Ƶĳ�������
	virtual int GetOneOutPaiCount(BYTE station,BYTE pai);

	//////////////////���Ʋ���///////////////////////////////////////////////////////

	///���һ�Ż��Ƶ�����������
	virtual void AddToHuaPai(BYTE station,BYTE pai);
	///���û�������
	virtual void SetHuaPaiData(BYTE station,BYTE pai[]);
	///������ҵĻ���
	virtual int CopyHuaPai(BYTE station,BYTE pai[]);

	///����Ƿ���
	virtual bool CheckIsHuaPai(BYTE pai);

	///////////////////�����Ʋ���///////////////////////////////////////////////////////

	///������������
	virtual void SetMenPaiData(BYTE station,BYTE pai[]);
	///������ҵ�����
	virtual int CopyMenPai(BYTE station,BYTE pai[]);
	///����������ת��
	virtual void ChangeMenPai();

/////////////////////////////////////////////////////
	
	///��ȡ���������Ч������
	virtual int GetPaiNum(BYTE station,BYTE pai[],BYTE count);
	
	///��ʼ������
	virtual void InitData();
	
		
	//��������״̬
	void SetQiHu(BYTE station,BYTE fan);
	//�������״̬
	void ReSetQiHu(BYTE station);
	//����Ƿ�����״̬(����״̬���ܵ��ں���)
	bool CheckQiHu(BYTE station,BYTE fan);

	//���ø����������Ƶ�����
	void	SetMenPaiCount(BYTE count[]);

};
