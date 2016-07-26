///
///ClientGameDlg.h: Headers and defines for CClientGameDlg
///
///

#if !defined(_CLIENTGAMEDLG_H__INCLUDED_)
#define _CLIENTGAMEDLG_H__INCLUDED_

#include "ClientView.h"
#include "../GameMessage/UpGradeLogic.h"
#include "LoveSendClassInExe.h"
#include "irrKlang/irrKlang.h"

using namespace irrklang;

///���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����

/// ��ʱ�� ID
/// ��ʼ��ʱ��
#define ID_BEGIN_TIME				10000			
/// ���ƶ�ʱ��ID
#define ID_SEND_CARD_TIMER          20000           
/// ���ƶ�ʱ�� lym0514
#define ID_KAIPAI_TIME				10001			
/// �ȴ���ʱ�� lym0514
#define ID_WAIT_TIME				10002			
/// ��ע��ʱ�� lym0514
#define ID_XIAZHU_TIME				10003		
/// ���Ƽ��
#define ID_SENDCARD_INTERVAL		10004			
 /// ��ע���Զ�ʱ��ID
#define ID_TEST_TIMER               20001       
/// ������ʾ��ʱ��ID
#define ID_RESULT_TIMER             20002          
 /// ��˸������ʾ��ʱ��ID
#define ID_RECT_TIMER               20003         
/// ���Ʒ��мҵ���
#define ID_XIANPAI					2004	
/// ��ׯ�ҵ���
#define ID_ZHUANGPAI				2005	
/// ��������
#define ID_PLAY_SOUND				2006	
//��ע��ʱ��������Ϣ
#define ID_FULLNOTE_TIME            2007            
/// ��Ϸ��ʼ
#define WM_USER_GAME_BEGIN        (WM_USER + 120)  
/// �й�
#define WM_USER_TRUSTEE           (WM_USER + 121)   
/// ����Ҫ��
#define WM_USER_HAVETHING         (WM_USER + 122)   
/// ����Ҫ�ߴ���ϢID
#define WM_USER_AGREE_LEAVE       (WM_USER + 123)   
/// �û�����
#define WM_USER_OUT_CARD		  (WM_USER + 124)	
/// ��������Ҫ����Ϣ
#define WM_USER_SEND_THING        (WM_USER + 130)	
///��ע������Ϣ
#define WM_USER_XIA_ZHU           (WM_USER + 140)	/// ���������ע��Ϣ
/// �����������ׯ��Ϣ
#define WM_USER_SHANG_ZHUANG      (WM_USER + 141)	
/// ���Ͳ�����Ϣ
#define WM_USER_TEST              (WM_USER + 150)	
/// ����������Ϣ
#define WM_USER_BANK              (WM_USER + 151)	
/// ���ͳ����ͻ��˿�����Ϣ
#define WM_SUPER_CTR              (WM_USER + 152)	
#define IDM_SEND_AI_CTRL			WM_USER + 153	//���ͻ����˿��Ƹı���Ϣ

/// �����������
#define ID_SOUND_MORE_500W        0x01   
/// ���������
#define ID_SOUND_500W             0x02   
/// һ��������
#define ID_SOUND_100W             0x03   
 /// ���������
#define ID_SOUND_DCM              0x04  
 /// ��������
#define ID_SOUND_CM               0x05  
/// ���Ƶļ��ʱ��
#define SEND_CARD_TIME            400   

/// ��ʾ��ׯ��Ϣ
#define PROMPT_CHANGE_NT_TIME     401   

/// CClientGameDlg �ͻ��ˣ���Ϸ����࣬������Ϸ��Ϣ
class CClientGameDlg :public CLoveSendClassInExe//CLoveSendClass
{
public:
	CClientGameDlg(void);
public:
	virtual ~CClientGameDlg(void);	

	CClientPlayView	  m_PlayView;					//��Ϸ��ͼ
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	///���30�ֵ���Ϣ��ׯ�Ƶ㣬���Ƶ㣬Ӯ����
	int	m_iGameInfo[MAXCOUNT][4]; 

protected:
	BYTE	m_byHighVersion;  // ��Ϸ�汾
	BYTE	m_byLowVersion;   // ��Ϸ�汾

	/// ��Ϸ�Ļ�����Ϣ
	GAME_BASE_INFO  m_GameBaseInfo;

	/// �������λ��
	BYTE m_abySeatID[PLAY_COUNT];
	BYTE m_byViewOrder;

	/// ��������е���
	WORD m_wHandCard[PLAY_COUNT][USER_HAND_CARD_COUNT]; 
	/// ��������е�������
	BYTE m_byHandCardCount[PLAY_COUNT];               
    /// �����ͻ��˱�־
	BOOL m_bSuperState;                              
	/// ��������
	bool m_PlaySound;								
	/// �й�
	bool m_bIsTrustee[PLAY_COUNT];         			   
	 /// ���μӷ�
	int	 m_iAwardPoint[PLAY_COUNT];					
    /// ׯ��λ��
	int	 m_iNtStation;                     			 
	 /// ���ҳ������Ⱥ�˳��
	BYTE m_bySequence[PLAY_COUNT];					
	/// �Ƿ񲻳���
	bool m_bGiveUp[PLAY_COUNT];            			  
	/// ��ǰ�����߷���
	BYTE m_byCurOrientation;						
	/// ���ƶ�������ʱ�䣬���ƶ�����
	int m_iSendCardTime;								
	///��ǰ����������id
	int  SoundbirdID;								
	/// ��ǰ����λ��
	WORD m_wCurSendPos; 							  
	/// �ܹ���������
	WORD m_wSendCount;  							    
	/// ������Ѿ����˵�������
	BYTE m_bySentCardCount[PLAY_COUNT];					
	/// ��һ���������
	BYTE m_byFirstSendPos;							
	/// ����ͨ��ռ�����
	int  m_bPlayListHwnd[15];							
	/// ����ͨ������λ��
	int  m_bPlayListPos;								

	HWND m_hMciWnd1;	
	HWND m_hMciWnd2;
	HWND m_hMciWnd3;
	HWND m_hMciWnd4;
	HWND m_hMciWnd5;
public:
	///��ע����
	CArray<XIA_ZHU, XIA_ZHU&>  m_ArrNote; 
	///��ע����
	CArray<XIA_ZHU, XIA_ZHU&>  m_ArrPlay;
	///�߳��˳�
	bool m_bThreadExit;                  

public:
	///��עʱ��	
	int  m_iXiaZhuTime;		
    ///����ʱ��
	int  m_iKaiPaiTime;      
	///����ʱ��
	int  m_iFreeTime;		
	///����ʱ����
	int  m_iSendInterval;	
	/// ����ʱ��
	int  m_iTimeSendCard;	
	///�Ƿ��ʼ�����
	bool m_bInitFinish;		
    /// ��Ϸ������� 
	CString    m_MoneyType;
	/// Ӯ���Ƿ��ɫ������ʾ
	bool       m_isWinRed;
	int        m_SpeCount;

public:
	// ��Ϸ��Ч����
	ISoundEngine				* m_pSEBkMusic;

	// ������Ϸ����
	void PlayPublicSound(int SoundID);

	
protected:
	///�汾�˶�
	bool CheckVersion(BYTE byHighVersion,BYTE byLowVersion,TCHAR szUpDateHttp[100]);

	/// ֪ͨ������Ϸ����
	virtual bool OnNotifyUpdateClientView(void * pNetData, UINT uDataSize);

	///// ����������
	//virtual bool OnNotifySendCard(void * pNetData, UINT uDataSize);

	///// ����һ����
	//void OnNotifySendACard(void);

	///// ���ƽ���
	//void OnNotifySendCardFinish(void);

	/// ���ǿ�˽�����Ϸ
	virtual bool OnNotifyCutEnd(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

	/// �й���Ϣ��Ӧ����
	virtual void OnChangeTrustee(BYTE byDeskStation, bool bIsTrustee);

public:
	/// ������ж�ʱ��
	void KillAllTimer(void);
	
	/// ��ע��ť����
	LRESULT	OnXiaZhu(WPARAM wparam, LPARAM lparam);

	/// ����ׯ���밴ť����
	LRESULT	OnShangZhuang(WPARAM wparam, LPARAM lparam);

	/// ������Ϣ����
	LRESULT	OnTest(WPARAM wparam, LPARAM lparam);

	/// ���а�ť����
	LRESULT	OnBank(WPARAM wparam, LPARAM lparam);
	/// ������ҿ�����Ϣ
	LRESULT	OnSuperCtr(WPARAM wparam, LPARAM lparam);
	
	///�����˿��Ƹı���Ϣ
	LRESULT	OnSendAICtrl(WPARAM wparam, LPARAM lparam);

// ����
public:
	/// ��λ���㵽��ͼλ��
	virtual BYTE GetViewSeat(BYTE byDeskStation);

	///������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);

	///��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);

	///��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE byDeskStation, UINT uTimeID, UINT uTimeCount);

	/// ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();

	/// ��ȫ������Ϸ
	virtual bool OnControlSafeFinish();

	/// �ı��Թ�����
	virtual void OnWatchSetChange(void);  // ƽ̨���͵���Ϣû��������

	/// �ı��Թ����� ��Ϸ�з��������ݣ����ڸ��½���
	virtual void OnWatchSetChange(S_WATCH_SET_CHANGE &sWatchSetChange);

	/// ��Ϸ����
	virtual void OnGameSetting();

	///����������Ϸ����
	virtual void ResetGameStation(int iGameStation);

	/// ������һ�λ��������ʱ�롢˳ʱ�����
	virtual void OnBeforeUpdateViewSeat(void);
	/// ���ز�������
	void BZSoundPlayGame(CWnd *pWnd, char *szFilename, DWORD dwPlayEnd, int nLoop=-1);

#ifdef VIDEO
	/// Ϊ��Ƶģ�����ش˺���
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);

	virtual bool AFCCloseFrame() ;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnMove(int x, int y);

	/// ϵͳ������Ϣ
	void OnHandleKaiPai(void * pNetData, UINT uDataSize);
	/// �����ע��Ϣ
	void OnHandleXiazhu(void * pNetData, UINT uDataSize);
	/// ϵͳ������Ϣ
	void OnHandleJieSuan(void * pNetData, UINT uDataSize);

	///������ע����������Ϣ
	void OnHandleXiazhuPlaySound(void * pNetData, UINT uDataSize);
	/// ����ټ�����Ϸ��ʼ��Ϣ
	void OnHandleBegin(void * pNetData, UINT uDataSize);
	/// �������ͻ��˿�����Ϣ
	void OnHandleSuperCtr(void * pNetData, UINT uDataSize);

	///��Ҷ�������ʱ��ͨ��������Ϣ��ȡ��Ϸ����
	void OnSetGameData(void * pNetData, UINT uDataSize);

	///��������
	void PlayOtherSound(TCHAR* path);

	///������ͼת������
	BYTE ViewStation(BYTE bDeskStation);

	///��������¼�
	void OnBnClickedRest();
	void OnBnClickedMax();

	///��ʼһ���ƶ����ĺ���
	void StartMoveCard(bool zhuang,int num, int zhen);

	void StartChao500Move(int type);//��������500��Ķ���

	///���Ƴ���500��Ķ���
	void StartChao500Move(CDC*pDC,int iWidth, int iHeight);

	void GetGameCode(char * szGameCode);

	void WriteInfo(TCHAR * pch);//add by wlr 20090806

	/// ������ע����
	BOOL PlayNoteSound(BYTE bSoundStyle);
	/// ������ע����
	BOOL PlayOutCardType(TCHAR FileName[],HWND hmciwnd);

	int m_iCountTimer;
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

};
///ȫ��ָ��
extern CClientGameDlg * g_pCClientGameDlg;

#endif // !defined(_CLIENTGAMEDLG_H__INCLUDED_)