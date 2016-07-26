#pragma once
#include "StdAfx.h"
#include "Resource.h"
#include "UpgradeMessage.h"
#include "UpGradeLogic.h"

/*--------------------------------------------------------------------*/
//��ʱ��ID
#define TIMER_SELECTGAMEMODE		101		//ģʽѡ��
#define TIMER_NOTE					102		//��ע��ʱ��
#define TIMER_GETREADY				103		//׼����ʱ��
#define TIMER_BETTING				104		//��ע��ʱ��
/*--------------------------------------------------------------------*/


//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
public:
	CUpGradeGameLogic		m_Logic;					//�����߼�
	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();

protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//���غ���
public:
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������ý���
	virtual void ResetGameFrame();
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	virtual bool SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUserMilliSecond=false);
	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	//����������Ϸ����
	virtual void ResetGameStation(int iGameStation);
	//ɱ��ʱ��
	void KillAllTimer();

private:
	//ѡ����Ϸģʽ��ʱ��
	void OnTimerSelectGameMode(int iTimerID);

	//׼����ʱ��
	void OnTimerGetReady(int iTimerID);

	//��ע��ʱ��
	void OnTimerBetting(int iTimerID);

	void OnMessageBetting(void *pData,int iLen);

	DECLARE_MESSAGE_MAP()
};
