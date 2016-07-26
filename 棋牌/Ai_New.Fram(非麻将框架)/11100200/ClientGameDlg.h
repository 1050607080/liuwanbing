// ClientGameDlg.h: Headers and defines for CClientGameDlg
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CLIENTGAMEDLG_H__INCLUDED_)
#define _CLIENTGAMEDLG_H__INCLUDED_

#include "StdAfx.h"
#include "upgrademessage.h"
#include "UpGradeLogic.h"

//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����

// ��ʱ�� ID
#define ID_BEGIN_TIME				10000			// ��ʼ��ʱ��
#define ID_SEND_CARD_TIMER          20000           // ���ƶ�ʱ��ID
#define ID_KAIPAI_TIME				10001			// ���ƶ�ʱ�� lym0514

#define ID_WAIT_TIME				100				// �ȴ���ʱ�� lym0514
#define ID_XIAZHU_TIME				101				// ��ע��ʱ�� lym0514
#define ID_XIAZHU_TOTAL_TIME		102				// ��ע��ʱ��
#define ID_XIAZHU_WAIT				5				// ��ע�ȴ�

#define ID_TEST_TIMER               20001           // ��ע���Զ�ʱ��ID

#define ID_RESULT_TIMER             20002           // ������ʾ��ʱ��ID

#define ID_RECT_TIMER               20003           // ��˸������ʾ��ʱ��ID

#define ID_XIANPAI					2004	//���Ʒ��мҵ���
#define ID_ZHUANGPAI				2005		//��ׯ�ҵ���

#define WM_USER_GAME_BEGIN        (WM_USER + 120)   // ��Ϸ��ʼ
#define WM_USER_TRUSTEE           (WM_USER + 121)   // �й�
#define WM_USER_HAVETHING         (WM_USER + 122)   // ����Ҫ��
#define WM_USER_AGREE_LEAVE       (WM_USER + 123)   // ����Ҫ�ߴ���ϢID

#define WM_USER_OUT_CARD		  (WM_USER + 124)	// �û�����

#define WM_USER_SEND_THING        (WM_USER + 130)	// ��������Ҫ����Ϣ
////��ע������Ϣ
#define WM_USER_XIA_ZHU           (WM_USER + 140)	// ���������ע��Ϣ
#define WM_USER_SHANG_ZHUANG      (WM_USER + 141)	// �����������ׯ��Ϣ
#define WM_USER_TEST              (WM_USER + 150)	// ���Ͳ�����Ϣ

struct XiaZhuStruct
{
	int Count;		// ��ע����
	int Money;		// ��עǮ
	int StopTime;	// ��עֹͣʱ��
	int MoneyType;	// ��ע��������
	int type;		//��ע����0-7
	int RunCount;	// ִ�д���
		
	// [@param Count] ��ע����
	// [@param Money] ��עǮ
	// [@param StopTime] ֹͣʱ��
	// [@param MoneyType] ��������
	// [@param type] ��ע����
	// [@param RunCount] ִ�д���
	int set(int Count, int Money, int StopTime, int MoneyType, int type, int RunCount=0)
	{
		this->Count = Count;
		this->Money = Money;
		this->StopTime = StopTime;
		this->MoneyType = MoneyType;
		this->type = type;
		this->RunCount = RunCount;

		return 0;
	}

	XiaZhuStruct()
	{
		Count = 0;
		Money = 0;
		StopTime = 0;
		RunCount = 0;
	}
};



class CClientGameDlg :public CLoveSendClass
{
public:
	CClientGameDlg(void);
	virtual ~CClientGameDlg(void);
	virtual BOOL OnInitDialog();

	XiaZhuStruct  m_XiaZhuStruct;

	DECLARE_MESSAGE_MAP()

	// ��Ϸ�Ļ�����Ϣ
	//GAME_BASE_INFO  m_GameBaseInfo;

	// �������λ��
	//BYTE m_abySeatID[PLAY_COUNT];
	//BYTE m_byViewOrder;

	// ����ͳһ�����߼�λ��
	//WORD m_wHandCard[PLAY_COUNT][USER_HAND_CARD_COUNT];   // ��������е���
	//BYTE m_byHandCardCount[PLAY_COUNT];                   // ��������е�������

	//bool m_bIsTrustee[PLAY_COUNT];              // �й�	
	//int	 m_iAwardPoint[PLAY_COUNT];				// ���μӷ�
	//BYTE m_bySequence[PLAY_COUNT];				// ���ҳ������Ⱥ�˳��
	//bool m_bGiveUp[PLAY_COUNT];                 // �Ƿ񲻳���
	//BYTE m_byCurOrientation;				    // ��ǰ�����߷���

	//int m_iSendCardTime;						// ���ƶ�������ʱ�䣬���ƶ�����
	
	//int  SoundbirdID;							//��ǰ����������id



	//WORD m_wCurSendPos;							// ��ǰ����λ��
	//WORD m_wSendCount;      // �ܹ���������
	//BYTE m_bySentCardCount[PLAY_COUNT];  // ������Ѿ����˵�������
	//BYTE m_byFirstSendPos;  // ��һ���������

	////////////////////////////////////////////////////////////////////
	BYTE m_byHighVersion;						// ��Ϸ�汾
	BYTE m_byLowVersion; 						// ��Ϸ�汾

	bool m_bCanXiaZhu;							// �ܷ���ע��

	__int64  m_iMaxDownMoney;					// ÿ������¾�
	int m_iZhuang;								// ��¼�Ͼֿ�����ׯ������
	int m_iNowNtStation;						// ��ǰׯ�ҵ�λ��	
	__int64 m_iMeZhu;								// ��ǰ�����µ�ע
	int m_iZnum;								// ׯ�Ҿ���
	int m_iXnum;								// ���о���
	int m_iZhuangBaShu;							// ׯ�ҽ����˼���
	__int64  m_iShangZhuangLimit;				// ��ׯ��Ҫ�Ľ����

	__int64 m_iAIMoney[8];						//��������ÿ�������µ�ע
	int m_iMaxZhuang;							// ׯ�ҽ��м��Ѿ���ׯ
	int m_iBaseNum;								// ���������ֻ��������Я����ҵļ���֮һ
	int  m_nPowerOfGold;//�Ŵ���
	///��ǰ����µ���ע
	__int64		m_iMeZongZhu;	
	///��ǰ����������µ���ע
	__int64		m_iMaxNote;	
	
	// ����ׯ����
	bool m_bSZSwitch;
	bool m_bXZSwitch;
	// ������ׯ������1�ڣ������ƻ�������ׯ�����1�����������������λ�õ���ֵ�������ׯ
	__int64 m_iMaxApplyNtMoney; 
	__int64 m_iMinApplyNtMoney; 

	// ��ׯ���˶���Ǯ��������ׯ��Ӯ�˶���Ǯ��Ҳ������ׯ
	__int64 m_iWinDownMoney; 
	__int64 m_iLostDownMoney; 

	// �����˿��������Զ���ע����ע���ҲҪ�����ù���Ա����һ����Χ����עҲ�ǰ������Χ����ע��������ע������ֵ�ĳ���
	int  m_iCanNoteValue; 

	int   m_byZTWangPct;						//ׯ������ע����(��ȷ��ǧ��֮һ)
	int   m_byZDZiPct;							//ׯ������ע����(��ȷ��ǧ��֮һ)
	int   m_byXTWangPct;						//��������ע����(��ȷ��ǧ��֮һ)
	int   m_byXDZiPct;							//�ж�����ע����(��ȷ��ǧ��֮һ)
	int   m_byHePct;							//����ע����	(��ȷ��ǧ��֮һ)
	int   m_byTDHePct;							//ͬ�����ע����(��ȷ��ǧ��֮һ)

	int m_probability[6][CHIP_NUM];//������ʹ�õĸ���

	//��������
	int                     m_iRobortSitZMax;            //�����������ׯ����
	int                     m_iRobortSitZMin;            //��������С��ׯ����	
	//int                     m_iRobotNotePercent;       //��ǰ��������ע�İٷֱ�����

	int                     m_iRobortSitCount;           //��ǰ��������ׯ�İ���
	BOOL                    m_bPlayerInList;             //�Ƿ��������ׯ���б���

	bool                    m_bUse500WJeton; 
    ///�Ƿ����ú��������ע
	bool                    m_bUseMilliSecond ;
    ///���õĺ��뵥λ
	int                     m_iMilliSecond ; 

protected:
	//�汾�˶�
	bool CheckVersion(BYTE byHighVersion,BYTE byLowVersion,TCHAR szUpDateHttp[100]);

	// ֪ͨ������Ϸ����
	//virtual bool OnNotifyUpdateClientView(void * pNetData, UINT uDataSize){ return true; }
	// ����������
	//virtual bool OnNotifySendCard(void * pNetData, UINT uDataSize){ return true; }
	// ����һ����
	//void OnNotifySendACard(void);
	// ���ƽ���
	//void OnNotifySendCardFinish(void);
	// ���ǿ�˽�����Ϸ
	//virtual bool OnNotifyCutEnd(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize){ return true; }
	// �й���Ϣ��Ӧ����
	//virtual void OnChangeTrustee(BYTE byDeskStation, bool bIsTrustee){}

public:
	// ������ж�ʱ��
	void KillAllTimer(void);	
	// ��ʼ��ť����
	//LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);

	// �йܰ�ť
	//LRESULT	OnBtnTrustee(WPARAM wparam, LPARAM lparam);
	// �����뿪��ť
	//LRESULT	OnBtnHaveThing(WPARAM wparam, LPARAM lparam);
	// �����뿪������
	//LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	// ͬ������뿪
	//LRESULT	OnAgreeUserLeave(WPARAM wparam, LPARAM lparam);

	// ��ע��ť����
	LRESULT	OnXiaZhu(WPARAM wparam, LPARAM lparam);
	// ����ׯ���밴ť����
	//LRESULT	OnShangZhuang(WPARAM wparam, LPARAM lparam);
	// ������Ϣ����
	LRESULT	OnTest(WPARAM wparam, LPARAM lparam);

// ����
public:
	// ��λ���㵽��ͼλ��
	//virtual BYTE GetViewSeat(BYTE byDeskStation){ return true; }
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	///��д���ö�ʱ��
	virtual bool SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID,bool bUserMilliSecond = false) ;
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE byDeskStation, UINT uTimeID, UINT uTimeCount);
	// ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin(){ return true; }
	// ��ȫ������Ϸ
	virtual bool OnControlSafeFinish();
	// �ı��Թ�����
	//virtual void OnWatchSetChange(void){};
	// �ı��Թ����� ��Ϸ�з��������ݣ����ڸ��½���
	//virtual void OnWatchSetChange(S_WATCH_SET_CHANGE &sWatchSetChange){};
	// ��Ϸ����
	//virtual void OnGameSetting(){};
	//����������Ϸ����
	virtual void ResetGameStation(int iGameStation);
	// ������һ�λ��������ʱ�롢˳ʱ�����
	//virtual void OnBeforeUpdateViewSeat(void){};


protected:
	virtual void OnCancel();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMove(int x, int y);
	// ϵͳ������Ϣlym0512
	void OnHandleKaiPai(void * pNetData, UINT uDataSize);
	// �����ע��Ϣ
	void OnHandleXiazhu(void * pNetData, UINT uDataSize);
	// ����ټ�����Ϸ��ʼ��Ϣlym0512
	void OnHandleBegin(void * pNetData, UINT uDataSize);
	// ����ټ�����ׯ��Ϣ
	void OnHandleApplyNt(void * pNetData, UINT uDataSize);
	//void PlayOtherSound(TCHAR* path);
	//������ͼת������
	//BYTE ViewStation(BYTE bDeskStation);
	//��������¼�
	//void OnBnClickedRest();
	//void OnBnClickedMax();
	//��ʼһ���ƶ����ĺ���
	//void StartMoveCard(bool zhuang,int num, int zhen);
	//void StartChao500Move(int type);//��������500��Ķ���
	//���Ƴ���500��Ķ���
	//void StartChao500Move(CDC*pDC,int iWidth, int iHeight);
	//void GetGameCode(char * szGameCode);
	void WriteInfo(TCHAR * pch);//add by wlr 20090806
	///////////////////////////////////////////////////
	//�����ע
	void OnSuiJiXiaZhu();

	//��ȡ�����ע����
	int GetRandType();
	int  Getchip(int line_num);			///����Ȩ�ظ����Ի�ȡһ��λ��
	//���������ļ�
	void LoadIniConfig();

	///�������Ƿ������ׯ
	inline  bool CheckApply(__int64 iMyMoney)
	{
		/*if (m_bSZSwitch)
		{
			return (iMyMoney >= m_iMinApplyNtMoney && iMyMoney <= m_iMaxApplyNtMoney) ; 
		}*/
		return true;
	}
	///���ׯ���Ƿ���Ҫ��ׯ
	inline  bool  CheckDownNt(__int64  iWinMoney)
	{
		//return (iWinMoney >= m_iWinDownMoney || iWinMoney <= (-m_iLostDownMoney)) ; 
		return true;
	}	 


	///��־��¼�ӿ�
	void  SrvTestlog(TCHAR *pMessage);

};
#endif // !defined(_CLIENTGAMEDLG_H__INCLUDED_)