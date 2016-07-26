#pragma once

#include "StdAfx.h"

#include "CMD_Fish.h"
#include "Include\Macro.h"
#include "Include\Define.h"
#include "UpgradeMessage.h"

//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
private:
	int exchange_ratio_userscore_;
	int exchange_ratio_fishscore_;
	int exchange_count_;

	int min_bullet_multiple_;
	int max_bullet_multiple_;

	BulletKind current_bullet_kind_;
	int current_bullet_mulriple_;
	bool allow_fire_;
	bool m_tag_fire;//�Ƿ������˿����Ъ��ʱ��

	SCORE exchange_fish_score_;
	SCORE fish_score_;//���

	float last_fire_angle_;
	int exchange_times_;

	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();
    int GetGameStatus();

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

	//��ʱ��
	bool SetGameTimerEx(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT nIDEvent);

public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);

	//��Ϸ�¼�
public:
	//��Ϸ��Ϣ
	bool OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//������Ϣ
	bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	// ��Ϣ����
protected:
	bool OnSubGameConfig(void* data, WORD data_size);
	bool OnSubExchangeFishScore(void* data, WORD data_size);
	bool OnSubUserFire(void* data, WORD data_size);
	bool OnSubCatchFish(void* data, WORD data_size);
	bool OnSubBulletIonTimeout(void* data, WORD data_size);
	bool OnSubCatSweepFishResult(void* data, WORD data_size);
	bool OnSubSwitchScene(void* data, WORD data_size);

private:
	void ExchangeFishScore();
	void Fire();
	float GetAngle(WORD chair_id);


public:
	//���������
	int MyRand();
};
