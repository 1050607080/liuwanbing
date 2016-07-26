///
///ClientView.h: Headers and defines for CClientPlayView
///
///

#if !defined(_CLIENTVIEW_H__INCLUDED_)
#define _CLIENTVIEW_H__INCLUDED_

#include "GameCard.h"
#include "HaveThing.h"
#include "LeaveMessage.h"
#include "lookinfo.h"
#include "Result.h"
#include "ShowAIInfo.h"
#include "SuperGM.h"

#define WM_SHOW_ROBOT_CHIP		 (WM_USER + 155)	    /** ��ʾ�����˳��� */

/// ÿ��������Ƶ��������
#define MAXJINBI			250 
/// �������ľ�����Ϣ
//#define MAXCOUNT			72 

/// ��ʼ��ť
#define IDC_BTN_START      20000 
/// �뿪��ť
#define IDC_BTN_LEAVE      20001 
///#define IDC_BTN_SORT       20002 // ����
///#define IDC_BTN_TRUSTEE    20003 // �й�
///#define IDC_BTN_HAVE_THING 20004 // ����Ҫ��
/// ����
#define IDC_BTN_OUTCARD    20005 
/// ����
#define IDC_BTN_PASS       20006 
/// ��ʾ
#define IDC_BTN_HINT       20007 
/// ������ϵ���
#define IDD_HAND_CARD      21000 
/// �����������
#define IDD_OUT_CARD       21010 
/// ���ƶ���
#define IDD_CARD_ANIMAL    21020 
/// ������Ҫ������
#define IDD_CENTER_CARD    21030 

//���밴ťlym0521
/// 100����
#define IDD_CHOUMA_100     21040 
/// 1000����
#define IDD_CHOUMA_1000    21041 
/// 1�����
#define IDD_CHOUMA_W       21042 
/// 10�����
#define IDD_CHOUMA_10W     21043 
/// 100�����
#define IDD_CHOUMA_100W    21044 
/// 500�����
#define IDD_CHOUMA_500W    21045 
/// ·����Ϣ
#define IDD_CHOUMA_LUZI    21046 
/// ��Ϣ���
#define IDD_CHOUMA_LEFT    21047 
 /// ��Ϣ�Ҽ�
#define IDD_CHOUMA_RIGHT   21048
/// ��ׯ��ť
#define IDD_CHOUMA_SHANGZ  21049 
/// ���а�ť
#define IDD_CHOUMA_BANK    21050 
/// �����ͻ������λ�������ע�İ�ť
#define IDD_HIDE_MONEY     21051 
///�����ͻ��˿��ƿ�ׯ�İ�ť
#define IDD_SUPER_ZHUANG   21052 
///�����ͻ��˿��ƿ��еİ�ť
#define IDD_SUPER_XIAN     21053 
///�����ͻ��˿��ƿ��͵İ�ť
#define IDD_SUPER_HE	   21054 
///�����ͻ��˻ָ����Ƶİ�ť
#define IDD_SUPER_NORMAL   21055 
///�Ϸ���ť
#define IDD_CHOUMA_TOP     21056 
///�·���ť
#define IDD_CHOUMA_BOTTOM  21057 
/// �����ͻ�����ʾ�����˿�����Ϣ�İ�ť
#define IDD_SHOW_AI_INFO   21058
//��ʾ�����Ի���ť
#define IDD_SUPER_DLG      21059

#ifdef VIDEO
#define MAX_VIDEO_NICKNAME_LEN  28  // ��Ƶʱ�ǳƲ��˹���
#endif
/// CClientPlayView �ͻ��ˣ���Ϸ���������

class CClientPlayView :
	public CUsingSkinClass
{
public:
	CClientPlayView(void);
public:
	virtual ~CClientPlayView(void);

	/// �����ǳ���ɫ
	void SetNameColor(COLORREF rgb)	{m_clrNameColor=rgb;}

	/// ���õȴ�״̬
	void SetWaitingType(BYTE byType);

	/// ���÷���
	void SetCurOrientation(BYTE byCurOrientationViewSeat);

protected:

	DECLARE_MESSAGE_MAP()

public:
	/// ���Ż��㵽��ͼλ��
	virtual BYTE GetViewSeat(BYTE byDeskStation);

	//�����û�����
	virtual void UpdateUserInfo(BYTE byViewStation);
	///����λ�õ���
	virtual void FixControlSize(int iWidth, int iHeight);

	///�滭���溯��
	virtual void DrawViewFace(CDC * pDC, int iWidth, int iHeight);

	///����gif
	virtual void SetGif(BYTE bDeskView,int iValue,int iTimer);
	/// ������ʾʱ��
	virtual bool SetUserTimer(BYTE bViewStation, UINT uTime);

	/// ���ƶ���
	void SendCard(BYTE byViewSeat, int iTime);

	/// �����й�״̬
	void SetTrustee(BYTE byViewSeat, bool bIsTrustee);

protected:

	/// ���Ʊ���
	virtual	void DrawBackground(CDC * pDC, int iWidth, int iHeight);

	/****************************************************************
	* ������: HRESULT  GetRelativePos(CPoint &PtItem, CBcfFile &f, const Cstring &csSecName,
	*                  const CString &csKeyName, const int iUserView, const CRect &rectClient)
	* ����: ��ȡ�����ļ���Ԫ�ص�����λ��
	* ����: CPoint &PtItem     [out] ��ȡ������
	*       CBcfFile &f        [in]  ���������ļ�
	*       CString &csSecName [in]  ��ȡ���ֶ���
	*       CString &csKeyName [in]  ��ȡ��Ԫ������
	*       int iUserView      [in]  �����ͼλ��
	*       CRect &rectClient  [in]  ��Ϸ�ͻ��˽�������
	* ����: HRESULT  0
	****************************************************************/
	HRESULT  GetRelativePos(CPoint &PtItem, CBcfFile &f, const CString &csSecName,
		const CString &csKeyName, const int iUserView, const CRect &rectClient);

	///�潱�����ͳ�Ʊ�־
	void DrawAwardPoint(CDC * pDC, int x, int y,BYTE iNum);

	/// ����ׯ�ұ�־
	void DrawNt(CDC * pDC, int x, int y);

	/// �����й�
	void DrawTrustee(CDC * pDC, int x, int y);

	///���Ƴ����Ⱥ�˳��
	void DrawSequence(CDC * pDC, int x, int y,int iSequence);

	/// ���Ʋ�����־
	void DrawPass(CDC * pDC, int x, int y);

	/// ���Ʒ���
	void DrawOrientation(CDC *pDC, int x, int y, int iView);

	/// ���Ƶȴ�״̬ͼƬ
	void DrawWaitingImg(CDC * pDC, int iWidth, int iHeight);
    ///������ע��ʱ��������Ϣ
	void DrawFullNoteBubble(Graphics* pGDIAdd, int iWidth, int iHeight);

protected:
	COLORREF	m_clrNameColor;				///< ��ʾ�û���ɫ

	CGameImage  m_imgBackground;                ///< ����
	CGameImage  m_imgCenterLogo;                ///< ����ͼƬ

	CGameImage	m_imgAwardPint;				    ///< �������(ͼƬ)
	CGameImage	m_imgNT;						///< ׯ�ұ�ʶ	
	CGameImage	m_imgTrusteeLogo;					///<�йܱ�ʶ
	CGameImage	m_imgPass;						///< passͼʾ
	CGameImage	m_imgPicSequence;               ///< ������˳��
	CGameImage  m_imgFangXiang;                 ///< �����
	CGameImage  m_imgWaiting;                   ///< �ȴ�ʱ��ͼƬ
	CGameImage  m_imgXing;                      ///< ����ͼƬxiazhu.bmp
	CGameImage  m_imgChoumaNum;                 ///< ��������ͼƬxiazhu.bmp
	CGameImage  m_imgMyNum;                     ///< ����Լ���ע�ĳ�������ͼƬgreen_num.bmp

	CGameImage  m_imgWang;                      ///< ��ͼƬxiazhu.bmp
	CGameImage  m_imgCard;						///< ��ͨ��ͼƬxiazhu.bmp

	CGameImage  m_imgchouma;					///< ����ͼƬjeton.bmp

	CGameImage  m_imgditu;						///< �����ͼditu.bmp
	CGameImage	m_imgPointValue;				///< �������ֵdianshu.bmp
	CGameImage	m_imgTwoKing;					///< ������������TwoKing.bmp

	CGameImage  m_imgCursor;					///< ���ͼƬcursor.bmp

	CGameImage  m_imgResult;					///< ����ͼƬresult.bmp
	CGameImage  m_imgResult1;					///< ����ͼƬresult1.bmp

	////////////////////////////////////////////////////////////////////Jing
	CGameImage  m_imgShowZ_dot;
	CGameImage  m_imgShowX_dot;
	CGameImage  m_imgShowH_dot;

	CGameImage  m_imgShowZ_Win_dot;					///< �·���ʾׯȦͼƬ��Ӯ show_zhuang_win.bmp
	CGameImage  m_imgShowX_Win_dot;					///< �·���ʾ��ȦͼƬ��Ӯ show_xian_win.bmp
	CGameImage  m_imgShowH_Win_dot;					///< �·���ʾ��ȦͼƬ��Ӯ show_he_win.bmp


	CGameImage  m_imgShowZ_x_dot;					///< �·���ʾׯȦͼƬ(��ʾx)show_zhuang_x.bmp
	CGameImage  m_imgShowX_x_dot;					///< �·���ʾ��ȦͼƬshow_xian_x.bmp
	CGameImage  m_imgShowH_x_dot;					///< �·���ʾ��ȦͼƬshow_he_x.bmp

	CGameImage  m_imgShowZ_Lost_x_dot;				///< �·���ʾׯȦͼƬ����ע��(��ʾx) show_zhuang_lost_x.bmp
	CGameImage  m_imgShowX_Lost_x_dot;				///< �·���ʾ��ȦͼƬ����ע��(��ʾx) show_xian_lost_x.bmp
	CGameImage  m_imgShowH_Lost_x_dot;				///< �·���ʾ��ȦͼƬ����ע��(��ʾx) show_he_lost_x.bmp
	////////////////////////////////////////////////////////////////////

	CGameImage  m_imgShowZ;						///< �·���ʾׯȦͼƬshow_zhuang.bmp
	CGameImage  m_imgShowX;						///< �·���ʾ��ȦͼƬshow_xian.bmp
	CGameImage  m_imgShowH;						///< �·���ʾ��ȦͼƬshow_he.bmp

	CGameImage  m_imgShowZ_Win;					///< �·���ʾׯȦͼƬ��Ӯ show_zhuang_win.bmp
	CGameImage  m_imgShowX_Win;					///< �·���ʾ��ȦͼƬ��Ӯ show_xian_win.bmp
	CGameImage  m_imgShowH_Win;					///< �·���ʾ��ȦͼƬ��Ӯ show_he_win.bmp


	CGameImage  m_imgShowZ_x;					///< �·���ʾׯȦͼƬ(��ʾx)show_zhuang_x.bmp
	CGameImage  m_imgShowX_x;					///< �·���ʾ��ȦͼƬshow_xian_x.bmp
	CGameImage  m_imgShowH_x;					///< �·���ʾ��ȦͼƬshow_he_x.bmp

	CGameImage  m_imgShowZ_Lost_x;				///< �·���ʾׯȦͼƬ����ע��(��ʾx) show_zhuang_lost_x.bmp
	CGameImage  m_imgShowX_Lost_x;				///< �·���ʾ��ȦͼƬ����ע��(��ʾx) show_xian_lost_x.bmp
	CGameImage  m_imgShowH_Lost_x;				///< �·���ʾ��ȦͼƬ����ע��(��ʾx) show_he_lost_x.bmp

	CGameImage  m_imgCenterLogo2;				///< �����ͼͼƬCenterLogo2.bmp

	// λ������	
	CPoint m_PtCardPoint[PLAY_COUNT];           // �����
	CPoint m_PtHandCard[PLAY_COUNT];            // ����������
	CPoint m_PtCurOrientation[PLAY_COUNT];      // ָʾ��������	
	CPoint m_PtPass[PLAY_COUNT];                // ����
	CPoint m_PtSequence[PLAY_COUNT];            // ������˳��
	CPoint m_PtTrustee[PLAY_COUNT];             // �й�
	CPoint m_PtNt[PLAY_COUNT];                  // ׯ������
	CPoint m_PtAwardPoint[PLAY_COUNT];          // AwardPoint
	CPoint m_PtSendCardStart[PLAY_COUNT];       // ������ʼλ���������м�
	CPoint m_PtSendCardAnimalEnd[PLAY_COUNT];   // �����յ�λ���û�������λ��
	CPoint m_PtGif[PLAY_COUNT];                 // �ͻ�

	// ����ͳһ������ͼλ��
	int	 m_iAwardPoint[PLAY_COUNT];				// ���μӷ�
	int	 m_iNtStation;                          /// ׯ��λ��
	bool m_bIsTrustee[PLAY_COUNT];              // �й�
	BYTE m_bySequence[PLAY_COUNT];				// ���ҳ������Ⱥ�˳��
	bool m_bGiveUp[PLAY_COUNT];                 // �Ƿ񲻳���
	BYTE m_byCurOrientation;				    // ��ǰ�����߷���

	BYTE m_byWaitingType;                       // �ȴ�״̬

public:

	///�̶�����,��ʼ����ͱ��治��
	///��ע����λ��
	CRect	ZRect,ZTRect,ZDRcet,XRect,XTRect,XDRcet,HRect,THRect;
	CRect	XiaZhuRect;				///<��ע���򣺣�0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	CRect	NowResultRect;			///<��ǰ�����
	CRect   NoMouseRect,NoMouseRect1,NoMouseRect2,NoMouseRect3,NoMouseRect4;			///<����ֹ��������
	CPoint	m_iYUcenter[8];			///<ÿ����������ĵ�λ��  
	CPoint	m_pQYText[8];			///<����8��������ע���λ��
	CPoint	m_pMoveStart;			///<���ƶ����Ŀ�ʼλ��
	CPoint	m_pCardZEnd;			///<ׯ���ƽ���λ��
	CPoint	m_pCardXEnd;			///<�м��ƽ���λ��
	CPoint	m_pTextStation;			///<�Ϸ���������λ��
	CPoint	m_pMeInfo;				///<����Լ���Ϣ��ʾ�ĳ�ʼλ��
	CPoint	m_pNtInfo;				///<ׯ����Ϣ��ʾ�ĳ�ʼλ��
	CPoint	m_pNtList;				///<ׯ���б�ĳ�ʼλ��
	CPoint	m_pXZXianZhi;			///<�·�������ʾ�ĳ�ʼλ��
	CPoint	m_pXing;				///<����ͼƬ�ĳ�ʼλ��
	CPoint	m_p10ResultInfo;		///<���10�ֵ��Ƶ���Ϣ��ʼλ��
	CPoint	m_pZXcount;				///<���30�ֿ�ׯ���д�������Ϣ��ʼλ��
	CPoint	m_pDitu;				///<�����ͼ�ĳ�ʼλ��
	CPoint	m_pViewCenter;          ///<��ͼ����λ��
	//CPoint  m_pCursor;			///<�������λ��
	POINT  m_pCursor;				///<�������λ��
	__int64     m_iLimitMoney;			///<��ׯ��Ҫ�����ٽ��
	CResult	m_Result;				///<���� 

	int    m_iMeXiaZhuInfo[MAXCOUNT];	//������72����ע��Ϣ����Ӯ���
	int	   m_iResultInfoArea[MAXCOUNT];	//������ʷ��ׯ�պ��������ʷ��¼

	//��Թ̶����ݣ�����ÿ�Ѷ�����
	int		m_iGameInfo[MAXCOUNT][4]; ///<���30�ֵ���Ϣ��ׯ�Ƶ㣬���Ƶ㣬Ӯ����
	int		m_iTempGameInfo[MAXCOUNT][4]; ///<���30�ֵ���Ϣ��ׯ�Ƶ㣬���Ƶ㣬Ӯ����
	//int		m_iZhuangList[4];		///<ׯ���б�
	///ׯ���б�
	short int	    m_iZhuangList[PLAY_COUNT+1];	
	/// ��ǰׯ�б�����
	int     m_bNowNtListIndex;  

	int		m_iNowNTstation;		///<��ǰׯ�ҵ�λ��
	BYTE    m_byMyStation;			///<�Լ���λ��
	int		m_iNTju;				///<��ǰׯ�ҽ��еľ���
	__int64		m_iNTmoney;				///<��ǰׯ�ҵĽ��
	int		m_iWinPercent;			///<��ǰ��ҵ�ʤ��
	__int64		m_iNTwin;				///<��ǰׯ�ҵĳɼ���Ӯ�Ľ�ң�
	int		m_iPlayCount;			///<�Ѿ������˱��ֵĵڼ���
	int		m_iGameCount;			///<�ܹ������˼���
	int		m_iZwin;				///<ׯ��Ӯ��Ϊ������������Ŵ�100����������ǰ����Ϊ��С�������
	int		m_iXwin;				///<�м�Ӯ
	int		m_iHwin;				///<��Ӯ
	int		m_iZDuizi;				///<ׯ����
	int     m_iXDuizi;				///<�ж���
	int		m_iZTianwan;			///<ׯ����
	int		m_iXTianwang;			///<������

	int     m_iHistoryPlayData[MAXCOUNT];	//������ʷ��ׯ�м�¼
	int     m_iMePlayData[MAXCOUNT]; ///<����Լ�����Ӯ���
	BYTE    m_byMePlayData[MAXCOUNT][3]; ///<����Լ�����Ӯ���

	int     m_iHistoryDuiZi[MAXCOUNT];   // ��������

	int     m_iMeWinCount;			///<����Լ�Ӯ�ľ���
	int     m_iMePlayCount;			///<����Լ����еľ���
	__int64     m_iWinmoney;			///<��ǰ�����Ӯ�Ľ��
	int		m_iShowInfoIndex;		///<��Ϣ��ʾ����������0��20��

	int		m_iSendCount;			///<������Ϣ����
	bool	m_bSetUpdateTime;		///<���ø��¼�ʱ��

	int     m_iCardNum;				///<���ŵڼ����ƵĶ���
	bool    m_bZhuang;				///<������ׯ�һ����мҵ���
	bool    m_bCardMove;			///<�Ƿ����ڽ��з���
	bool    m_bSendCard;			///<ȷ���Ƿ��ƽ���
	WORD    m_iNowCard[2][3];		///<��ǰ�Ƶ�
	BYTE    m_CardCount[2];			///<ׯ���Ƶ�����
	BYTE    m_ContinueShowCardCount[2]; ///<ׯ���Ƶ�����

	int		m_iZPaiXing[5];			///<ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	int		m_iXPaiXing[5];			///<�м�����
	int		m_iWinner;				///<Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	int		m_iKaiPai;				///<���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ�7�ͣ�8ͬ���
	int		m_iWinQuYu[8];			///<��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���

	int     m_iResult[6];			///<���汾�֣�ׯ�Ƶ㣬���Ƶ㣬ׯ���ͣ������ͣ�Ӯ�������Լ��ý��
	int     m_iZCardInfo[5];		///<ׯ������Ϣ��ׯ�ҵ�������1���Ƶĵ�����ǰ�����Ƶĵ���
	int     m_iXCardInfo[5];		///<�е�����Ϣ��ׯ�ҵ��������мҵ�������
	int		m_iChooseNote;			///<��ǰѡ��ĳ���
	int		m_iChooseRect;			///<������ڵ��������
	int		m_iCardZhen;			///<��ǰ���Ƶڼ�֡
	int		m_iShowCard;			///<��ʾ��,0����ʾ��1��ʾһ�ţ�2��ʾ
	__int64		m_iZhongZhu;			///<���ѵ�ǰ��ע��
	__int64		m_iQuYuZhu[8];			///<����ÿ�������µ�ע�0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	__int64		m_iMyZhu[9];			///<��������Լ���ÿ�������µ�ע�0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	__int64		m_iMaxZhu[8];			///<����ÿ������������µ�ע��
	int		m_iJinBi[8][MAXJINBI];  ///<����ڰ˸������µĽ���������¼ǰ2000��
	CPoint	m_pJinBi[8][MAXJINBI];  ///<����ڰ˸������µĽ��λ���������¼ǰ2000��
	__int64		m_iQuYuJinBi[8][6];		///<ÿ�����¸��ࣨ��6�ࣩ��ҵĸ���

	__int64		m_iJinBiNum[8];         ///<�˸�����ÿ������Ľ����
	BYTE    m_bNowGameState;        ///<��ǰ��Ϸ״̬
	__int64     m_iAllJiBiCount;        ///<��ǰ�Ѿ�Ѻ�˶��ٽ�ң�
	__int64		m_iZhuangFen;			///<����ׯ�ҵĵ÷�
	__int64		m_iXianFen;				///<�����мҵĵ÷�
	__int64		m_iMeFen;				///<��������Լ��ĵ÷�
	__int64		m_iMeFanFen;			///<������ҷ����ķ֣�����ʱ����

	int    m_iMoveTime;				///<����ʱ��
	int    m_iMoveType;				///<�����Ļ�������

	bool   m_bShowResult;			///<�Ƿ���ʾ������Ϣ
	bool   m_bShowRect;				///<����Ӯ�������˸

	int     m_nPowerOfGold;         ///<��ұ���
	int		m_iSendCardZhen;		///<����֡��

	__int64     m_iAIMoney[8];			///�����˵���ע
	bool    m_bHideAIMoney;			///�Ƿ����ػ�������ע
	CString m_sSuperInfo;			///�����ͻ��˿�����Ϣ
	bool    m_bShowSuper;			//��ʾ�����ͻ��˿�����Ϣ

	CString m_ZhuangStr;;//ׯ����ʾ���� ��ʱ���ܻ�н���ͻ���غ͵�
	CString m_XiangStr;//�е���ʾ����
	///��ǰ����������µ���ע
	__int64		m_iMaxNote;	

	int     m_iGamePictureType;		//1Ϊ�졢�ء���ͼƬ�� 2Ϊׯ���С���ͼƬ����Ҫ�ǿ��ƺ����ʾ��ֵ��

	bool    m_isWinRed;		//Ӯ���Ƿ��ɫ������ʾ

	int     m_SpeCount;
	BYTE	m_Rcolor;		//��ɫֵ
	BYTE	m_Gcolor;		//��ɫֵ
	BYTE	m_Bcolor;		//��ɫֵ

	HCURSOR				m_Cursor[3];						///<�����


	bool               m_bUse500WJeton;          
	///////////////////////////////////////////

	int m_iWidth;
	int m_iHeight;
	CDC * m_pDC;
public:
	// ��ť
	CNormalBitmapButtonEx m_btnStart;		    // ��ʼ��ť
	CNormalBitmapButtonEx m_btnLeave;		    // �뿪��ť

	CNormalBitmapButtonEx m_btnOutCard;		    // ����
	CNormalBitmapButtonEx m_btnPass;		    // ������
	CNormalBitmapButtonEx m_btnHint;		    // ��ʾ

///�ټ��ֳ��밴ť,100,1000,1��10��100��500��
	/// 100���밴ť
	CNormalBitmapButtonEx m_btn100;	
	/// 1000���밴ť
	CNormalBitmapButtonEx m_btn1000;
	/// 1����밴ť
	CNormalBitmapButtonEx m_btn1w;	
	/// 10����밴ť
	CNormalBitmapButtonEx m_btn10w;	
	/// 100����밴ť
	CNormalBitmapButtonEx m_btn100w;
	/// 500����밴ť
	CNormalBitmapButtonEx m_btn500w;
	/// ��Ϣ�ƶ���ť
	CNormalBitmapButtonEx m_btnleft;
	/// ��Ϣ�ƶ��Ұ�ť
	CNormalBitmapButtonEx m_btnright;
	/// ��·�Ӱ�ť
	CNormalBitmapButtonEx m_btnluzi;
	/// ��ׯ���밴ť
	CNormalBitmapButtonEx m_btnshangz;
	/// ���а�ť
	CNormalBitmapButtonEx m_btnbank;
	/// �����ͻ������λ�������ע�İ�ť
	CNormalBitmapButtonEx m_btnHideAIMoney;
	/// �����ͻ�����ʾ�����˿�����Ϣ�İ�ť
	CNormalBitmapButtonEx m_btnShowAIInfo;

	///�����ͻ��˿��ƿ�ׯ�İ�ť
	CNormalBitmapButtonEx m_btnCtrZhuang;
	///�����ͻ��˿��ƿ��еİ�ť
	CNormalBitmapButtonEx m_btnCtrXian;
	///�����ͻ��˿��ƿ��͵İ�ť
	CNormalBitmapButtonEx m_btnCtrHe;
	///�����ͻ��˻ָ����Ƶİ�ť
	CNormalBitmapButtonEx m_btnCtrNormal;
	///��ʾ�����Ի���İ�ť
	CNormalBitmapButtonEx m_btnCtrSuper;

	/// �°�ť����ׯ����)
	CNormalBitmapButtonEx m_btTop;      
	/// �°�ť����ׯ����)
	CNormalBitmapButtonEx m_btBottom;    

	CUpGradeGameCard  m_HandCard[PLAY_COUNT];   // ������ϵ���
	CUpGradeGameCard  m_OutCard[PLAY_COUNT];    // �����������
	CUpGradeGameCard  m_SendCardAnimal[PLAY_COUNT];   // ���ƶ���
	CUpGradeGameCard  m_SendBaseCard;           // �����Ʊ�(һ����Ҫ���͵���)
	CUpGradeGameCard  m_MingCard;               // һ������

	CHaveThing		  m_HaveThing;				/// �����뿪
	CLeaveMessage	  m_ThingMessage;			/// �뿪��Ϣ
///��·�ӶԻ���
	Clookinfo		  m_LuziDlg;	

///��ʾ��������Ϣ�Ի���
	CShowAIInfo		  m_ShowAIInfo;	

///lym2010-03-19�ܷ�����ׯ��ť
	bool			m_bCanShangZhuang;
	//��Ϣ������¼����ֹ����ڵ��ʱ�������Ϣ���������Ϣ
	int             m_iMessageCount; 
///���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
	int				m_iNowJuShu;	

	bool          m_bFullNoteDisplay;       /*��ע��ʱ�Ƿ���ʾ����*/ 

#ifdef VIDEO
	////////////////////��Ƶ/////////////////////////
	CPlayVideo				m_PlayVideo;
	BOOL					m_bWatch;           // �Լ��Ƿ�Ϊ��ս
	CGameImage				m_pVideoFrame;      // ��Ƶ����
	CRect					m_rectVideoFrame[MAX_PEOPLE];  ///��Ƶ��������
	bool					GetVideoRect(CString  strNickname, RECT& rect, CString& pic);
	/////////////////////////////////////////////////////////////////////////
#endif

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnClickedStart(void);
	afx_msg void OnBtnClickedLeave(void);

	///���밴ť��Ϣ
	afx_msg void OnBtnClicked100(void);
	afx_msg void OnBtnClicked1000(void);
	afx_msg void OnBtnClicked1w(void);
	afx_msg void OnBtnClicked10w(void);
	afx_msg void OnBtnClicked100w(void);
	afx_msg void OnBtnClicked500w(void);
	///��Ϣ����¼�
	afx_msg void OnBtnClickedInfoLeft(void);
	///��Ϣ�Ҽ��¼�
	afx_msg void OnBtnClickedInfoRight(void);
	///��·�Ӱ�ť�¼�
	afx_msg void OnBtnClickedluzi(void);

	///��ׯ��ť�¼�
	afx_msg void OnBtnClickedShangZhuang(void);

	///���а�ť�¼�
	afx_msg void OnBtnClickedBank(void);
	///�����ͻ������λ�������ע�İ�ť
	afx_msg void OnBtnClickedHideMoney(void);

	///�����ͻ�����ʾ�����˿�����Ϣ�İ�ť
	afx_msg void OnBtnClickedShowAIInfo(void);

	///�����ͻ��˿��ƿ�ׯ�İ�ť
	afx_msg void OnBtnClickedCtrZhuang(void);
	///�����ͻ��˿��ƿ��еİ�ť
	afx_msg void OnBtnClickedCtrXian(void);
	///�����ͻ��˿��ƿ��͵İ�ť
	afx_msg void OnBtnClickedCtrHe(void);
	///�����ͻ���ȡ�����Ƶİ�ť
	afx_msg void OnBtnClickedCtrNormal(void);
	///��ʾ���س����ͻ���
	afx_msg void OnBtnClickedSuper(void);

	///����ƶ���Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	///������̧����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	///����Ҽ�����
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	///��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	///�ټ�����Ϣ����lym0512
	///������ע��Ϣlym0512
	void OnHandleXiazhu(void * pNetData, UINT uDataSize);
	
	///����ϵͳ������Ϣlym0514
	void OnHandleKaiPai(void * pNetData, UINT uDataSize);

	///lym0512���ƽ��
	void DrawJinBi(CDC*pDC, int iWidth, int iHeight);

	///lym0521����ѡ������
	void DrawRect(CDC*pDC, int iWidth, int iHeight);

	///lym0521����·����Ϣ
	void UpdateLuziData(void);

	///lym0530���µ�ǰ���Ƶ���Ϣ��Ϣ
	void UpdateNowData(void);

	///����ׯ��Ϣ��ׯ�б�
	void DrawNtInfo(CDC*pDC, int iWidth, int iHeight);

	///������ע��Ϣ���˸��������ע��ׯ�кͿ���ע���޶�
	void DrawXiaZhuInfo(CDC*pDC, int iWidth, int iHeight);

	///������ʷ�����Ϣ�����10�ֵ�ׯ�е��������30��ׯ�кͿ���
	void DrawResultInfo(CDC*pDC, int iWidth, int iHeight);

	/// ���Ƶ�ֵ
	void DrawPointValue(CDC * pDC, CDC &dc, int ix, int iy);

	///��������Լ�����Ϣ����ң������ʣ��ܼƳɼ�
	void DrawMeInfo(CDC*pDC, int iWidth, int iHeight);

	///���ƶ�̬��Ϣ����ׯ����ʾ������ע����ʾ��ע�������ƣ������ʾ������Ϣ��������ʱ��
	void DrawActiveInfo(CDC*pDC, int iWidth, int iHeight);

	///���Ƹ����������ע����
	void DrawXiaZhuNum(CDC*pDC,int num, int iWidth, int iHeight,int x,int y,bool my);

	///�����ƣ�������ʾ����
	void DrawCard(CDC*pDC,int iWidth, int iHeight);

	///�����ƣ��ƶ���
	void DrawMoveCard(CDC*pDC,int iWidth, int iHeight);

	//�������ƶ���
	void StartMove(bool zhuang,int num,int zhen);

	///��x,y����һ����Card
	void DrawACard(CDC*pDC,int iWidth, int iHeight,WORD Card,int x,int y);

	///������ע������ע
	void EnableXiaZhu(bool enable);

	///ÿ�ֽ������ʼ����ͼ����
	void Init();

	///����Լ��Ƿ���ׯ�һ���ׯ���б���
	bool CheckMeZhuang(void);

	///���½��㴰�ڵ�λ��
	void UpdateResultWindow(int x,int y);

	///��������500��Ķ���
	void StartChao500Move(int type);

	///���Ƴ���500��Ķ���
	void DrawChao500Move(CDC*pDC,int iWidth, int iHeight);

	///���ƹ��ͼƬ
	void DrawCursor(CDC*pDC,int iWidth, int iHeight);//���ƹ��ͼƬ

	///����͸�������
	void DrawResultFrame(CDC*pDC, int iXPos, int iYPos);

	/// ����������ׯ��ť
	void SetApplyBanker(bool bApply);

	/// �ָ��������״̬
	void OnRestoreCursor();

	/// �������״̬
	void OnShowCursor(BOOL bShow);

	//������ת�ɴ�С������ַ�����lym1015
	void NumToChar(__int64 result,CString* szText );

	///������������ʾ��ǰ��ע��
	void ShowNowNote();

	///������������ʾ��ǰ��ע�����õ�ǰ��ע������
	void SetNowNoteData(int type,int num5bw,int num1bw,int num10w,int num1w,int numq,int numb);

	///��ȡ����Լ���ǰ����ע�ܶ�
	__int64 GetMyAllNote();

	///��ʱ�����Լ�����ע��ť�����
	void SetXiaZhuBtn();

	///��ׯ�����·���ť�¼�
	void OnBtnClickedNTTop(void);

	///��ׯ�Ѷ����·���ť�¼�
	void OnBtnClickedNTBottom(void);

	///������ͨ���������ͻ�ȡ������
	__int64 GetMoneyFromType(__int64 moneytype);

public:  ///add xyh 2011-12-7
		CGameImage  m_imgPromptOpenCard;			///< ��ʾ����ͼƬPromptOpenCard.bmp 
		CGameImage  m_imgPromptChangeNt;			///< ��ʾ��ׯͼƬPromptChangeNt.bmp
		bool		m_bShowPromptOpenCard;			///< �Ƿ���ʾ��ʾ����ͼƬ
		bool		m_bShowPromptChangeNt;			///< �Ƿ���ʾ��ʾ��ׯͼƬ	

		///��ʾ����ͼƬ
		void DrawPromptOpenCard(CDC * pDC, int x, int y);
		
		///��ʾ��ׯͼƬ
		void DrawPromptChangeNt(CDC * pDC, int x, int y);

		///���ÿ�����ʾͼƬ
		void SetPromptOpenCard(bool bShowFlag);

		///���û�ׯ��ʾͼƬ
		void SetPromptChangeNt(bool bShowFlag);
public:

	//��ȡ������ҵ��ǳ�
	void GetAllUserNicheng();

	///����Ҹ�������ת���ַ���
	void ChangeNumToCStringByPower(__int64 nMoney ,bool bHaveSpace ,TCHAR *chSpace, CString &strRet) ; 

	///��ȡ��ʵ��Ǯ���߽�������ʾ��Ǯtrue��ʾ��������ʾ��Ǯ , false��ʾת������ʵ��Ǯ
	__int64 ChangeMoneyByPower(__int64  nMoney , bool  bUIMoney ) ;   
	

	SUPERDATA				m_SuperData;				/**< ����GM������ҵ�����*/
	CSuperGM				m_SuperDlg;					/**< ����GM������ʾdialog*/

	LRESULT OnShowRobotChip(WPARAM wParam, LPARAM lParam);
};
#endif // !defined(_CLIENTVIEW_H__INCLUDED_)