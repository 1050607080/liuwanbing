// Auto_AIDlg.h : ͷ�ļ�
//
#include "stdafx.h"
#include "resource.h"

#include "CTime_day.h"


#include <vector>
#include <functional>
#include <algorithm>
#include "afxwin.h"
using namespace std;

#pragma once

#define				 MAX_GAME_ROOM				2000			//��󷿼���
#define				 MAX_LOGON_COUNT			10
#define				 TIME_NEXT_ID				1				//��һ���ʺ�


#define				 TIME_LOGIN				2				//��¼�¼�
#define				 TIME_CHECKLOGIN		3				//��ʱ��¼
#define				 TIME_CHECKRELOGIN		4				//��ʱ��¼
#define				 TIME_CHECKRESOCKETERROR		5				//��ʱ��¼

#define					TIME_CHECKBCONNECT_TIMEOUT	6		//�z�yB�������B�ӳ��r
#define					TIME_CHECKZCONNECT_TIMEOUT	7		//�z�yZ�������B�ӳ��r

#define              TIME_AUTO_LOGIN            100               // һ��ʱ�����Զ����������
#define				 TIME_SPACE					1000
#define				 IDM_CLOSE_GAME_ROOM		WM_USER+200		//�رշ���
#define				 IDM_LOGON_FAIL				WM_USER+201		//��½ʧ��
#define				 IDM_UPDATE_USER			WM_USER+202		//���������ʾ

//#define DDZ
//��Ϸ������Ϣ
struct RoomInfoStruct
{
	//�ṹ����
	bool							bAccess;							//���ʱ�־
	UINT							uTabIndex;							//��������
	DWORD							dwGameMSVer;						//���汾��
	DWORD							dwGameLSVer;						//���汾��
	CBaseRoom						* pGameRoomWnd;						//����ָ��
	TCHAR							szProcessName[31];					//��������
	ComRoomInfo						stComRoomInfo;						//��������
	void operator=(RoomInfoStruct & paiData)
	{
		bAccess = paiData.bAccess;
		uTabIndex = paiData.uTabIndex;
        dwGameMSVer = paiData.dwGameMSVer;
		dwGameLSVer = paiData.dwGameLSVer;
		pGameRoomWnd = paiData.pGameRoomWnd;
		memcpy(&stComRoomInfo,&paiData.stComRoomInfo,sizeof(stComRoomInfo));
		memcpy(&szProcessName,&paiData.szProcessName,sizeof(szProcessName));
	}
};

//��Ϸ������Ϣ


struct TTimeRange;

struct TRobotItem
{
	int iIndex;
	int iUserID;
	RoomInfoStruct* pRoomItem;
	
	TTimeRange* pTimeRanges;
public:
	TRobotItem()
	{
		ZeroMemory(this, sizeof(TRobotItem));
	}
};


struct TTimeRange
{
	TTime_day	tBegin, tEnd;
	int		iCount;
	bool	iLogoned;
	bool GetValid()
	{
		CTime t = CTime::GetCurrentTime();
		TTime_day t_day(t.GetHour(), t.GetMinute());

		int r1 = CmpTime_dat(tBegin, t_day);
		int r2 = CmpTime_dat(t_day, tEnd);
		return (r1 <= 0 &&  r2 < 0);
	}
	vector<TRobotItem*>	m_Robots;
public:
	TTimeRange(TTime_day tbegin, TTime_day tend, int icount)
	{
		tBegin = tbegin;
		tEnd = tend;
		iCount = icount;
		m_Robots.clear();
		iLogoned = false;
	}
};


struct TTime_Envet: public TTime_day
{
	int iFlag;
	int iCount;
	CString GetStr()
	{
		CString r, tmp;
		if (iFlag == 0)
			tmp = "��¼";
		else
			tmp = "�˳���¼";
		r.Format("%s%s������%d��", getTimeStr(), tmp, iCount);
		return r;
	}
};


//



// CAuto_AIDlg �Ի���
class CAuto_AIDlg : public CBaseRoom
{
	// ����
public:
	//���캯��
	CAuto_AIDlg(CWnd * pParent=NULL);
	~CAuto_AIDlg();

	// �Ի�������
	enum { IDD = IDD_AUTO_AI_DIALOG };
	int		m_nInRoomCount;
	static int                      m_nRoomPeopleSize;                  //���巿�����������������������ֵʱ�������Զ��뿪����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON									m_hIcon;
	BYTE									m_bConnectToWitchServer;
	CString									m_CenterServerIPAddr;
	int										m_CenterServerPort;
	int										m_iEnterKindID;								//��Ϸ����ID							
	int										m_iEnterNameID;								//��Ϸ����ID
	int										m_iEnterRoomID;								//��½����ID
	CGameListCtrl							m_GameList;									//��Ϸ�б�
	MSG_GP_R_LogonResult					m_PlaceUserInfo;							//�û�����
	RoomInfoStruct							m_RoomInfo[MAX_GAME_ROOM];					//������Ϣ

	TRobotItem								m_Robots[MAX_GAME_ROOM];			
	vector<TTimeRange>						m_TimeRanges;


	int							        	m_bNowRoomIndex;							//��ǰ��������
	int										m_iAccountsStart;							//�ʺ���ʼ
	int                                     m_iAccountsStartTemp;                       //�ʺ���ʼ����ʱ��¼
	int										m_iCurrentAccounts;							//��ǰ�ʺ�
	int										m_bCountLogon;								//׼����½���ٺ�
	TCHAR									m_CurrentLogonName[21];						//��ǰ��½�ǳ�
	TCHAR									m_CurrentNickName[21];						//��ǰ��½�ǳ�
	BYTE									m_bEnterStyle;								//��½����
	TCHAR									m_szNickFix[21];							//�ǳ�ǰ׺
	TCHAR									m_szNameFix[21];							//ע����ǰ׺
	bool									m_bAutoRegister;							//ע��
	bool                                    m_bMachineAndPlay;							//������������Ƿ�ͬ��
	bool                                    m_bAutoDesk;								//�������Զ�����
	
	int                                     m_iMachineCheckTime;//�����˼����
	BYTE                                    m_bMachineDeskCount;//ÿ�������˸���
	int										m_iRealLogonCount;							//ʵ�ʳɹ���½����
	__int64									m_i64MaxAiMoney;                    //AI��������Ǯ�����ﵽ������ʹ�Ǯ
	__int64									m_i64CheckMoneyMax;                 //ȡǮ�����
	__int64									m_i64CheckMoneyMix;                 //ȡǮ��С��
	__int64									m_i64SaveAndOutMoneyBase;			//��ȡǮ������������ȡ������
	BOOL                            m_bAiBank;                          //�Ƿ�����˲�������
	BOOL							m_bOpenBankBase;							//�Ƿ���������������ȡ
	int                             m_nAiLogonTime;                     //�����˽��뷿����

	CGameRoomEx * m_pGameRoom  ;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();

	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
	//ȷ������
	virtual void OnOK() ;

	void OnAutoOK();
	//ȡ������
	virtual void OnCancel();
	//��ʾ��ʾ
	void ShowCue(TCHAR *pMessage);
	//
	void ShowCue(CString pMessage);
	//���� 
	void OnConnectToCenterServer();
	//��½������
	void OnConnectToLogonServer();
	//������Ϣ����
	bool OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��½ ��Ϣ����
	bool OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�б���Ϣ
	bool OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������˷��͵ı�������ID
	bool EnterContestRoom(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϸ����
	bool OnCentranceGameRoom(CAFCRoomItem * pGameRoomItem);
	//�رշ�������
	LRESULT OnCloseGameRoom(WPARAM wparam, LPARAM lparam);
	//ֱ�ӵ�½
	BOOL SendUserInfo();////�����û���Ϣ
	//ĳ���ʺŵ�½ʧ�� 20081128 by Shao
	LRESULT OnLogonFail(WPARAM wparam, LPARAM lparam);
	LRESULT OnUpdateMoney(WPARAM wparam, LPARAM lparam);

	///����Ƿ����ߣ����߾Ͳ��ص�
	BOOL PlayerOnLine(TCHAR szCurrentName[]);

	//20091123
	CString coreGetCode();
	CString innerGetMac();
	//��ȡ��Ϸ�б�
	bool GetGameRoomList();
	//���뷿��
	bool EnterRoom(int roomid);
	//��ʱ��
	void OnTimer(UINT_PTR nIDEvent);
	//��������
	bool ReConnectToCenterServer();
	//�ӿں���
public:
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent();
protected:
	//����������Ϣ����
	inline virtual int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	inline virtual int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//�˶�
	BOOL CheckVersion();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedClose();
public:
	afx_msg void OnBnClickedMix();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	

public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedOk();



public:
	bool m_bUseNew;
	bool m_bLogoning;
	int m_iCurLogonIndex;
	int	m_iSumLogonCount;
	bool m_bExistLogonTimer;
	void LogonRobot_New();
	void InitRobot_New();
	void OnTimeLogon_New();
	void OnLogon_New();
	void OnGameList_New();
	void SendUserInfo_New();
	void ConnectToB_New();
	void ConnectToZ_New();
	void ConnectToW_New(int roomid);
	void GetContestRoomID(int roomid);//���������ȡ��������ID
	void StartRun_New();
	void ReLogonRobot_New();
	void CheckLogonRobot_New();
	void CheckSocketError_New();
	int	FindNextLogonIndex(int iIndex);
	CListBox m_EventList;
	vector<TTime_Envet> m_EventTimes;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
};
