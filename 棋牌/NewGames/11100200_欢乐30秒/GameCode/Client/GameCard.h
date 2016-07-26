#pragma once

//��Ϣ����
#define	IDM_LEFT_HITCARD			WM_USER+110						//����˿�
#define	IDM_RIGHT_HITCARD			WM_USER+111						//�һ��˿�
//#define IDM_MOUSE_OUT				WM_USER+112
#define IDM_MOUSE_UP				WM_USER+113                     // �϶����˿�

//��������
#define	UG_HUA_MASK					0x00F0							//��ɫ����
#define	UG_VALUE_MASK				0x000F							//��������

//����
#define IMAGE_1024_768				0x00							//��
#define IMAGE_800_600				0x00							//��
#define IMAGE_SMALL					0x01							//��
#define IMAGE_VERY_SMALL			0x03							//��

//�˿˿�����
class CUpGradeGameCard : public CWnd
{
	//ö�ٶ���
public:
	typedef enum eXShowMode { EXSM_LEFT=0, EXSM_CENTER, EXSM_RIGHT  } E_XSHOW_MODE;
	typedef enum eYShowMode { EYSM_TOP=0,  EYSM_CENTER, EYSM_BOTTOM } E_YSHOW_MODE; 
	CWordArray						m_wCardArray;					// �˿�����
	CWordArray						m_wCardUpArray;					// �˿�����
	CWordArray						m_wMoveSelectCardArray;			// �ƶ�ѡ���˿�����

	//��������
protected:
	int								m_iCardHight;					//�˿˸߶�
	int								m_iCardWidth;					//�˿˿��
	int								m_iCardHSpace;					//�˿˼��
	int								m_iCardVSpace;					//�˿˼��
	int								m_iCardUpSpace;					//����߶�
//	CRgn							m_CardViewRgn;					//�˿�����
	CWnd							* m_pHandleWnd;					//������
	//��Ϣ����
protected:
	bool							m_bShowCard;					//��ʾ�˿�
	bool							m_bHorzLook;					//�Ƿ���
	bool							m_bUseMouse;					//��Ӧ���
	WORD							m_wHotHitCard;					//�����˿�
	BYTE							m_byCardCount;					//�˿���Ŀ
	// �˿���Ŀ(��m_byCardCountView>m_byCardCount ���� m_byCardCount>0ʱ��ʾm_byCardCountView)
	// ������ʾm_byCardCount
	BYTE							m_byCardCountView;				
	CPoint							m_PtBasePoint;					//��׼����
	E_XSHOW_MODE					m_XShowMode;					//��ʾģʽ
	E_YSHOW_MODE					m_YShowMode;					//��ʾģʽ
	BYTE							m_bShowNum;						//�Ƿ���ʾ����	
	BYTE							m_bCardStyle;					//ʹ�����Ƿ�ΪС��true��ʾʹ��С��,false��ʾʹ��Ĭ����
	BOOL							m_bSelect;						//�Ƿ���ĳ������
	CWordArray						m_wCardFriendArray;				//�˿�����(����������ֶ�����ʾ)
	BYTE							m_bFriendCardCount;				//��������
//	BOOL							m_bLButtonDown;					//���������
	WORD							m_wFirstCard;					//
	CPoint							m_ptLeftHit;					//��������µ�����
	CPoint							m_ptLeftHitMove;				//��������ƶ�����������
	CPoint							m_ptMovePoint;					//�ƶ���굱ǰ������
	BOOL							m_bCanMove;						//�Ƿ�Ϊ�ƶ�
	WORD                            m_wCanUp;                       //�Ƿ����𱸷�
	WORD							m_wMoveFirstCard;				//�ƶ��ĵ�һ���Ƽ�������������

	//�������÷���
	int								m_iSourcePoint;						//ԭ������
	int								m_iBackPoint;						//�����̵׷���
	int								m_iBackMultiple;					//���õױ���

	//����������
	bool							m_bMultiSel;
	CPoint							m_pStartPos;					// ��ʼ���λ��
	CPoint							m_pEndPos;						// ����λ��
	CPoint                          m_pMovePos;                     //�������λ��
	// ���ƹ���
	bool							m_bMoveSel;					
	bool							m_bMove;
	int								m_CurCardIndex;					// ��굱ǰ����card index
	WORD							m_wStartIndex;					// ��ʼ�ƶ���λ��
	WORD							m_wEndIndex;					// ��ʼ�ƶ���λ��
	/*bool                            m_bLDown;
	bool                            m_MouseMove;*/



	//ͼƬ����
protected:
//	CAFCImage						m_CardPic[2];					//�˿˿ؼ�
	static		CGameImage			m_CardPic[6];//[4];					//�˿˿ؼ�
	CGameImage						m_Num;								//����
	CGameImage m_bk;//����
	CToolTipCtrl					m_ToolTip;						//������ʾ

	// qy �����޸� 20090107
	CPoint m_ptStart;    // �ƶ��Ŀ�ʼ����
	CPoint m_ptEnd;      // �ƶ�Ŀ������
	long m_lMoveCount;   // �ƶ�����
	// end qy �����޸� 20090107

	//��������
public:
	//���캯��
	CUpGradeGameCard(BYTE bCardStyle=0);
	//��������
	virtual ~CUpGradeGameCard();

	//���ú���
public:
	//��ʼ������
	bool InitGameCard(CWnd * pHandleWnd);
	//�����˿˲���
	void SetCardParameter(int iHSpace, int iVSpace, int iUpSpace);
	//�����ƿ�͸�
	void SetCardWidthHeight(int iWidth,int iHeight);
	//���û�׼�㺯��
	void SetBasePoint(CPoint & BasePoint, E_XSHOW_MODE XShowMode, E_YSHOW_MODE YShowMode);

	// ��ȡ��ǰλ��
	void GetBasePoint(CPoint &ptBasePoint);

	/****************************************************************
	* ������: void MoveAnimal(CPoint &ptStart, CPoint &ptEnd, long lTime)
	* ����: ��ʼ�ƶ����룬�����ƶ���ʼ���յ㡢��ʱ��
	* ����: CPoint &ptStart    [in]  ��ʼλ��
	*       CPoint &ptEnd      [in]  �յ�
	*       long lTime         [in]  �ƶ���ʱ�� (ms)
	* ����: N/A
	****************************************************************/
	void MoveAnimal(CPoint &ptStart, CPoint &ptEnd, long lTime);

	// ֹͣ�ƶ�
	void StopMove(void);	

	//������ʾ�˿�
	void SetShowCard(bool bShow);
	//����ʹ�����
	void SetUseMouse(bool bUserMouse=false);
	//������ʾ��ʽ
	void SetLookMode(bool bHorzLook);
	//���õ�����ģʽ
	void SetCardStyle(BYTE bCardStyle=0){m_bCardStyle=bCardStyle;}
	//������ģʽ
	BYTE GetCardStyle(){return m_bCardStyle;}
	//���õ�����ģʽ
	void SetCardShowNum(BYTE bShowNum = 0){m_bShowNum = bShowNum;}
	//������ģʽ
	BYTE GetCardShowNum(){return m_bShowNum;}
	//���ܺ���
public:
	//�����˿�
	bool SetCard(WORD wCardList[], WORD wUpList[], BYTE bCardCount, BYTE byCardCountView=0);
	//ɾ��������˿�
	BYTE RemoveUpCard();
	//��ȡ������˿�
	BYTE GetUpCard(WORD wCardList[]);
	//��ȡ�˿���Ϣ
	BYTE GetCard(WORD wCardList[], WORD wUpList[]);
	//��������
	int GetPoint();
	//�������÷���
	BOOL SetBackPointAndMultiple(int iSourcePoint = 0,int iBackPoint = 0,int iBackMultiple = 1);
	//��������
public:
	//��ȡ�˿���Ŀ
	BYTE GetCardCount() { return m_byCardCount; }
	//��ȡ�˿���ֵ
	BYTE GetCardValue(WORD wCard) 
	{ 
		//return (bCard&UG_VALUE_MASK);
		BYTE bkk=(BYTE)(wCard&UG_VALUE_MASK);
		return bkk; 
	}
	//��ȡ�˿˻�ɫ
	BYTE GetHuaKind(WORD wCard) { return (BYTE)(wCard&UG_HUA_MASK)-1; }
	//��ȡ�˿˴�С
	BYTE GetCardNum(WORD wCard) { return (BYTE)(wCard&UG_VALUE_MASK); }	
	//��ȡ�˿�λ��
	BYTE GetCardXPos(WORD wCard) { return GetCardValue(wCard)-1; };
	//��ȡ�˿�λ��
	BYTE GetCardYPos(WORD wCard) { return GetHuaKind(wCard)>>4; };

	//�ڲ�����
protected:
	//��������
	BYTE HitCardTest(CPoint & Point);
	//��������
	BYTE HitCardTestAbove(CPoint &Point);
	//��������λ��
	void FixCardControl();
	//��ȡ��С����
	bool GetCardSize(CSize & Size);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags,CPoint Point);
	
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	//������������
	BOOL SetFriendArray(WORD wCardList[], WORD wUpList[], BYTE bCardCount);

	void UnUpAllCard();
protected:
	//��������������ĳһ���Ƶĸ���
	BYTE FindACardNumInFriendList(BYTE iCardNum);
	//����ͼƬ
	void SetCardImage();
	//����������λ��
	BOOL ChangeIndex(BYTE DestIndex ,BYTE SrcIndex);
	//��������
	BOOL ChangeIndexArray(BYTE DestIndex,BYTE SrcIndex);
	//���Ʒ�ת
	BOOL Convert(WORD wCardList[],int iCardCount);
	// ����͸����
	void DrawTransparent(HDC destdc, CRect &rc, COLORREF color, int Alpha);
	// �õ���index
	BYTE GetCardIndex(CPoint &Point);

	//�ջ����е���

//public:
//	//����ƶ�������
	void MouseMoveAjust(CPoint Point);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

