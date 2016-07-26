#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpgradeMessage.h"

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա

//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_WAIT_NEXT				101					//��һ�ֶ�ʱ��
#define ID_OUT_CARD					102					//���ƶ�ʱ��
#define ID_NOTE                     105                 //�ȴ��м���ע




//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	//�ؼ�����
public:

	int						m_iVersion;
	int						m_iVersion2;
public: 
	BYTE                m_byGameStation ;										//��ǰ����Ϸ״̬
	int                 m_iMyDeskStation ;										//����Լ�����Ϸλ��
	BYTE				m_bNtPeople;											//����ׯ��

	BYTE				m_bScorePeople;											//��ǰ�з���
	BYTE				m_bGameBeginWaitTime;									//��Ϸ��ʼ�ȴ�ʱ��
	BYTE				m_bGameBetTime;											//��Ϸ��עʱ��
	BYTE				m_bCallCardTime;										//��Ϸ����ʱ��
	BYTE				m_iUserAlreadySend[PLAY_COUNT][13];						//�û��Ѿ������˿�
	__int64				mi64UserMaxBet;											//�û������ע��
	__int64				m_i64UserCanBet[3];										//�û���ע�����ֵ
	__int64				m_i64CurrentNote;										//��ǰѡ�����ע
	__int64				m_i64UserBet[PLAY_COUNT];								//�û�����ע

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
	virtual bool SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUseMillSecond = false) ; 
	//������ж�ʱ��
	void KillAllTimer();
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};
	//�ܷ��뿪����
	virtual BOOL CanLeftDesk();
	//���Ż��㵽��ͼλ��
	virtual BYTE ViewStation(BYTE bDeskStation);
	//��Ϣ������
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//�˳�����
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT nIDEvent);

private:
	//���������ļ�
	void LoadGameIni();
private:


public:
	///��ʼ����������
	void	ResetGameData();

	//��������
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);

	//������Ϸ
	void ResetGameStation(int iGameStation);

public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);

public:
	
	//���������ͬ����Ϸ��Ϣ
	void OnHandleAgreeGame(void *buffer);

	//�����������������ȷ��ׯ����Ϣ
	void OnHandleNt(void *buffer);

	//�������������������ע��Ϣ
	void OnHandleBet(void *buffer);

	//����������������ķ�����Ϣ
	void OnHandleSendCard(void *buffer);

	//����������������Ŀ�ʼ��Ϸ��Ϣ
	void OnHandlePlay(void *buffer);

	//�������������������Ϸ��ȫ������Ϣ
	void OnHandleSafeFinish(void *buffer);

	//�������������������ע��Ϣ
	void OnHandleBetResult(void *buffer);

	//����������������Ľ�����Ϣ
	void OnHandleCall(void *buffer);

	//�������������������Ϸ����������Ϣ
	void OnHandleNormalFinish(void *buffer);

	//����������������ĳ����û���Ϣ
	void HandleSuperMessage(void *buffer);

};
