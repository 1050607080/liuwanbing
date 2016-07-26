#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpGradeLogic.h"
#include "UpGradeMessage.h"


#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա

//��ʱ�� ID
#define ID_BEGIN_TIME				101					//��ʼ��ʱ��
#define ID_BET_TIMER				102					//��ע��ʱ��
#define ID_APPLYNT_TIMER			103					//������ׯ




//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����

//6������ֵ
const	int	G_iChouMaValues[6] = {100,1000,10000,100000,1000000,10000000};

//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	//��������
public:

	BYTE                m_byGameStation ;       ///��ǰ����Ϸ״̬


	int					m_iXiaZhuTime;			///��עʱ��	
	int					m_iKaiPaiTime;			///����ʱ��  
	int					m_iShowResult;			///��ʾ�����ʱ��
	int					m_iFreeTime;			///����ʱ��	

	__int64				m_iLimitMoney;			///<��ׯ��Ҫ�����ٽ��
	__int64				m_iMaxNote;				///��ǰ����������µ���ע

	__int64				m_i64MaxZhu[BET_ARES];       //ÿ���������µ����ע	

	short int			m_iZhuangList[PLAY_COUNT+1];		///ׯ���б�


	int					m_iNTju;				///<��ǰׯ�ҽ��еľ���
	int					m_iNowNTstation;		///<��ǰׯ�ҵ�λ��

	__int64				m_i64MyBetCount;		//�Լ���ע����
	__int64				m_i64NtWinMoney;		//ׯ�ҵĳɼ�
	__int64				m_i64MyWinMoney;		//�Լ���Ӯ���


	BYTE				m_byNowCard[2][3];		///<��ǰ����//ׯ�е��ƣ�0Ϊ�У�1Ϊׯ

	__int64				m_iRobotMaxBet;			//�����������ע

	//�ؼ�����
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
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	virtual void OnWatchSetChange(void);
	virtual void OnGameSetting();
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
	//������ж�ʱ��
	void KillAllTimer();
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};
	//�û��뿪
	virtual bool GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);


	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);


	void ResetGameStation(int iGameStation);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	//	afx_msg void OnClose();
protected:
	virtual void OnCancel();


private:
	//�����ȡ��ע����
	int		GetSelArea();
	//�����ע����
	int		GetSelChouMa();



};
