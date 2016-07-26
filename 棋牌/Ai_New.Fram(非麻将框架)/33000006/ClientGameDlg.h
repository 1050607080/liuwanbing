#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpgradeMessage.h"
#include "fish/GameScreenWnd.h"

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա

//��ʱ�� ID
#define TIME_FISH_ROUND			101			//����Ƕ�ת����ʱ��
#define TIME_BULLTE_RUN			102			//�ӵ���������м�ʱ��
#define TIME_FISH_MOVE			103			//����ζ����м�ʱ�� delete
#define TIME_NET_OPEN			104			//�ӵ�������ʱ��
#define TIME_HIT_FISH			105			//�������

#define TIME_ADD_FIRE			106			//����
#define TIME_ADD_SCORE			107			//�Ϸ�
#define TIME_SHOT				108			//�����ڵ�





//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{

	//�ؼ�����
public:

	int						m_iVersion;
	int						m_iVersion2;
public: 

	BYTE                m_byGameStation ;         ///��ǰ����Ϸ״̬
	int                 m_iMyDeskStation ;       ///����Լ�����Ϸλ��
	bool				m_bIsPlaying;				//��Ϸ�Ƿ��Ѿ���ʼ

private:
	//��Ϸ������
	GameScreenWnd		m_GameScreen;
	//��Ϸ����
	GameStation_Base	m_GameData;

	__int64		m_i64MinAddScore;		//��С�Ϸ�
	__int64		m_i64MaxAddScore;		//����Ϸ�
	int			m_iMinFire;				//��С�ڻ���
	int			m_iMaxFire;				//����ڻ���
	int			m_iAutoAddScoreSet;		//�Զ��Ϸ�����

	int			m_iMinShotTime;			//��췢����
	int			m_iMaxShotTime;			//����������

	int			m_iTimeContrl;			//�����ӵ��������


	__int64		m_i64RandAddScore;
	int			m_iRandFire;

	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();
 
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

	
	void	LoadRobotSet();
private:
	//�л���λ
	int		SwitchChairID(int m_Currchairid);

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


};
