#pragma once

#include "../GameMessage/CSrvLogic.h"
//class CServerGameDesk;

//�����������߼���

class CSrvLogicEx:public CSrvLogic
{
public:
	CSrvLogicEx(void);
	~CSrvLogicEx(void);
	
public:	
	//���������¼�
	virtual bool GetGameStaton(BYTE bDeskStation, bool bWatchUser,BYTE GameStation);
	// ��Ϸ���ݰ�������
	virtual int HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser=false);
	// �û�����
	virtual int ReceiveUserOutCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);


	//��Ϸ�¼����������ģ�ȷ����һ���¼���ʲô�������¼�ֵ,type������¼�����
	virtual void GameCPU(int type);
	///��ʼ�¼�����
	virtual void ExecuteGameBegin(BYTE &nextID,BYTE &time);
	///��2��ɫ��ȷ��ׯ�ң������齫һ��ɫ�Ӷ�ׯ��
	virtual void ExecuteDongNt(BYTE &nextID,BYTE &time);	
	///��2��ɫ��ȷ�����ƶ����������Ͷ�����
	virtual void ExecuteTwoSeziGp(BYTE &nextID,BYTE &time);	

	///����
	virtual void ExecuteSendPai(BYTE &nextID,BYTE &time);	
	///��ʼ����֪ͨ�¼�
	virtual void ExecuteBeginOut(BYTE &nextID,BYTE &time);	
	///ץ���¼�
	virtual void ExecuteZhuaPai(BYTE &nextID,BYTE &time);	

	//�����¼�
	virtual void ExecutePengPai(BYTE &nextID,BYTE &time);
	//�����¼�
	virtual void ExecuteGangPai(BYTE &nextID,BYTE &time);

	//�����¼�
	virtual void ExecuteHuPai(BYTE &nextID,BYTE &time);
	///����¼�
	virtual void ExecuteCountFen(BYTE &nextID,BYTE &time);
	///������Ϸ�����¼�
	virtual void ExecuteNormalEnd(BYTE &nextID,BYTE &time);


	///������station��ǰ������Щ����
	virtual bool CheckAction(int station,BYTE pai,BYTE type);
	//�ſ����Զ�����
	virtual void AutoHandle(bool bNetCut=false);
	///�Զ�������ƻ���ץ��
	virtual void AutoOutOrGet();

	/// �������
	virtual void UserSitDesk(BYTE bDeskStation);
	/// ���õ�ǰ���ڷ������¼�
	virtual void SetDoingThing(BYTE thingId,BYTE st1=255,BYTE st2=255,BYTE pai=255,BYTE data=255);
public:
	////////�Զ����¼�/////////////////////////

	// �û�Ѻע
	int ReceiveUserYaZhu(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// ȷ���׷�
	int ReceiveMakeDiFen(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);//

	///Ѻע�¼�
	void ExecuteYaZhu(BYTE &nextID,BYTE &time);


};