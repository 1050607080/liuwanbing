#include "StdAfx.h"
#include "ClientView.h"
#include "ClientGameDlg.h"
#include "resource.h"
#define IDR_FAPAI						1100	// ���ƶ�ʱ����5������������ж�������ʾ�����
#define IDR_XIANPAI						1200	// ���Ʒ��мҵ���
#define IDR_ZHUANGPAI					1300	// ��ׯ�ҵ���
#define IDR_UPDATEXIAZHU				1301	// ���½���
#define IDR_CTL_ZHUANG_BTN				1302	// lym���Ƶ����ׯ��ť�ļ�ʱ��


void MsgPrint(const char *lpszFormat, ...);



// ��Ϣӳ��
BEGIN_MESSAGE_MAP(CClientPlayView, CUsingSkinClass)

	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnClickedStart)
	ON_BN_CLICKED(IDC_BTN_LEAVE, OnBtnClickedLeave)
	ON_BN_CLICKED(IDD_CHOUMA_100, OnBtnClicked100)//100���밴ť
	ON_BN_CLICKED(IDD_CHOUMA_1000, OnBtnClicked1000)//1000���밴ť
	ON_BN_CLICKED(IDD_CHOUMA_W, OnBtnClicked1w)//1w���밴ť
	ON_BN_CLICKED(IDD_CHOUMA_10W, OnBtnClicked10w)//10w
	ON_BN_CLICKED(IDD_CHOUMA_100W, OnBtnClicked100w)//100w
	ON_BN_CLICKED(IDD_CHOUMA_500W, OnBtnClicked500w)//500w
	ON_BN_CLICKED(IDD_CHOUMA_LUZI, OnBtnClickedluzi)//��·���¼�
	ON_BN_CLICKED(IDD_CHOUMA_LEFT, OnBtnClickedInfoLeft)//��Ϣ��ť�¼�
	ON_BN_CLICKED(IDD_CHOUMA_RIGHT, OnBtnClickedInfoRight)//��Ϣ�Ұ�ť�¼�
	ON_BN_CLICKED(IDD_CHOUMA_SHANGZ, OnBtnClickedShangZhuang)//��ׯ��ť�¼�
	ON_BN_CLICKED(IDD_CHOUMA_BANK, OnBtnClickedBank)//���а�ť�¼�IDD_HIDE_MONEY
	ON_BN_CLICKED(IDD_HIDE_MONEY, OnBtnClickedHideMoney)//�����ͻ������λ�������ע�İ�ť
	ON_BN_CLICKED(IDD_SHOW_AI_INFO, OnBtnClickedShowAIInfo)//�����ͻ�����ʾ�����˿�����Ϣ�İ�ť

	ON_BN_CLICKED(IDD_SUPER_ZHUANG, OnBtnClickedCtrZhuang)//�����ͻ��˿��ƿ�ׯ�İ�ť
	ON_BN_CLICKED(IDD_SUPER_XIAN, OnBtnClickedCtrXian)//�����ͻ��˿��ƿ��еİ�ť
	ON_BN_CLICKED(IDD_SUPER_HE, OnBtnClickedCtrHe)//�����ͻ��˿��ƿ��͵İ�ť
	ON_BN_CLICKED(IDD_SUPER_NORMAL, OnBtnClickedCtrNormal)//�����ͻ���ȡ�����Ƶİ�ť
	ON_BN_CLICKED(IDD_SUPER_DLG, OnBtnClickedSuper)//��ʾ�����ͻ��˶Ի���

	ON_MESSAGE(WM_SHOW_ROBOT_CHIP, OnShowRobotChip)


	ON_BN_CLICKED(IDD_CHOUMA_TOP, OnBtnClickedNTTop)//��ׯ�����Ϸ���ť�¼�
	ON_BN_CLICKED(IDD_CHOUMA_BOTTOM, OnBtnClickedNTBottom)//��ׯ�����·���ť�¼�

	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()
CClientPlayView::CClientPlayView(void)
:CUsingSkinClass(),m_Result(this)/*,m_LuziDlg(this)*/
{
	////OutputDebugString("CClientPlayView::CClientPlayView");
	/////lym0512�ټ��ֿͻ�����Ϣ����
	m_Cursor[0]=LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_HAND1));
	m_bShowResult = false; //�Ƿ���ʾ������Ϣ
	m_bShowRect = false ;//����Ӯ�������˸
	m_iMoveTime =0 ;  //����ʱ��
	m_iMoveType = -1;  //�����Ļ�������
	m_bNowGameState = GS_WAIT_AGREE;        //��ǰ��Ϸ״̬
	m_iZwin =0 ;//ׯ��Ӯ�İٷֱȣ�Ϊ������������Ŵ�100����������ǰ����Ϊ��С�������
	m_iXwin =0 ;//�м�Ӯ�İٷֱ�
	m_iHwin =0 ;//��Ӯ�İٷֱ�
	m_iZDuizi =0 ;//ׯ����
	m_iXDuizi =0 ;//�ж���
	m_iZTianwan =0 ;//ׯ����
	m_iXTianwang =0 ;//������
	m_iPlayCount=0;
	m_iGameCount =0 ;			//�ܹ������˼���
	m_iShowCard = 0;//��ʾ��
	m_iAllJiBiCount=0;        //��ǰ�Ѿ�Ѻ�˶��ٽ�ң�
	m_bFullNoteDisplay = false;

	m_iMessageCount = 0;

	m_iWinPercent =0;       //��ǰ��ҵ�ʤ��
	m_iNowNTstation=-1;		//��ǰׯ�ҵ�λ��
	m_iNTju =0 ;			//��ǰׯ�ҽ��еľ���
	m_iNTmoney=0;			//��ǰׯ�ҵĽ��
	m_iNTwin=0;				//��ǰׯ�ҵĳɼ���Ӯ�Ľ�ң�
	//�Լ�����Ϣ
	m_iWinmoney=0;			//��ǰ�����Ӯ�Ľ��
	m_iMeWinCount=0;//����Լ�Ӯ�ľ���
	m_iMePlayCount=0;//����Լ����еľ���

	m_iLimitMoney=0;		//��ׯ��Ҫ�����ٽ��
	m_iZhongZhu=0;			//���ѵ�ǰ��ע��
	m_Cursor[0] = LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));//���1
	m_Cursor[1] = LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));//���2
	m_Cursor[2] = LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));//���3
	m_iCardZhen =-1;   //��ǰ���Ƶڼ�֡
	m_iShowInfoIndex = 0; //��Ϣ��ʾ����������0��20��

	m_iZhuangFen=0;		//����ׯ�ҵĵ÷�
	m_iXianFen=0;			//�����мҵĵ÷�
	m_iMeFen=0;			//��������Լ��ĵ÷�
	m_iMeFanFen = 0;		//������ҷ����ķ֣�����ʱ����
	m_nPowerOfGold=1;///��������
	m_isWinRed =false;
	m_Rcolor =0;
	m_Gcolor =0;
	m_Bcolor =0;

	m_SpeCount=4;

	int i = 0;
	for (i=0; i<PLAY_COUNT; ++i)
	{
		m_PtLogo[i] = CPoint(0,0);                // ͷ��
		m_PtCardPoint[i] = CPoint(0,0);           // �����
		m_PtHandCard[i] = CPoint(0,0);            // ����������
		m_PtTime[i] = CPoint(0,0);                // ��ʱ��
		m_PtCurOrientation[i] = CPoint(0,0);      // ָʾ��������
		m_PtName[i] = CPoint(0,0);                // �ǳ�
		m_PtAgree[i] = CPoint(0,0);               // ����
		m_PtPass[i] = CPoint(0,0);                // ����
		m_PtSequence[i] = CPoint(0,0);            // ������˳��
		m_PtTrustee[i] = CPoint(0,0);                // �й�
		m_PtNt[i] = CPoint(0,0);                  // ׯ������
		m_PtAwardPoint[i] = CPoint(0,0);          // AwardPoint
		m_PtSendCardStart[i] = CPoint(0,0);       // ������ʼλ���������м�
		m_PtSendCardAnimalEnd[i] = CPoint(0,0);   // �����յ�λ���û�������λ��
		m_PtGif[i] = CPoint(0,0);                 // �ͻ�
		m_iAwardPoint[i] = 0;                     // ������
		m_bIsTrustee[i] = false;                  // �Ƿ��й�
		m_bySequence[i] = PLAY_COUNT;             // ������˳��Ĭ��Ϊ���һ��
		m_bGiveUp[i] = false;
	}

	memset(m_iGameInfo, -1, sizeof(m_iGameInfo));//��Ϸʱׯ���Ƶ�������Ӯ��Ϣ
    for(int i=0;i<MAXCOUNT;i++)
	{		
		if(i<8)
		{
			m_iJinBiNum[i] = 0;//ÿ������Ľ����
			for(int j=0;j<MAXJINBI;j++)
			{
				m_iJinBi[i][j]=0;    //����ڰ˸������µĽ���������¼ǰ2000��
				m_pJinBi[i][j].x=0;  //����ڰ˸������µĽ��λ���������¼ǰ2000��
				m_pJinBi[i][j].y=0;  //����ڰ˸������µĽ��λ���������¼ǰ2000��
			}
		}
	}
	memset(m_iZhuangList,-1,sizeof(m_iZhuangList));//ׯ���б�
	m_bNowNtListIndex=0;

	ZRect.SetRect(555,235,760,432);  // ׯ��ע����
	ZTRect.SetRect(555,432,760,506); // ׯ������ע����
	ZDRcet.SetRect(555,165,760,237); // ׯ������ע����
	XRect.SetRect(38,235,245,432); //  ����ע����
	XTRect.SetRect(38,432,245,506); // ��������ע����
	XDRcet.SetRect(38,165,245,237); // �ж�����ע����
	HRect.SetRect(245,309,554,433); //  ����ע����
	THRect.SetRect(245,433,554,505);//  ͬ�����ע����

	m_pCardZEnd.SetPoint(245,215);         //ׯ���ƽ���λ��
	m_pCardXEnd.SetPoint(397,215);        //�м��ƽ���λ��
	m_pTextStation.SetPoint(270,90);		//�Ϸ���������λ��
	m_pMeInfo.SetPoint(8,597);		   //����Լ���Ϣ��ʾ�ĳ�ʼλ��
	m_pNtInfo.SetPoint(325,24);		   //ׯ����Ϣ��ʾ�ĳ�ʼλ��
	m_pXing.SetPoint(275,597);		   //����ͼƬ�ĳ�ʼλ��
	m_p10ResultInfo.SetPoint(38,89);   //���10�ֵ��Ƶ���Ϣ��ʼλ��
	m_pXZXianZhi.SetPoint(90,16);		 //�·�������ʾ�ĳ�ʼλ��
	m_pNtList.SetPoint(0,0);		   //ׯ���б�ĳ�ʼλ��
	m_pZXcount.SetPoint(0,0);        //���30�ֿ�ׯ���д�������Ϣ��ʼλ��
	m_pDitu.SetPoint(0,0);          //�����ͼ�ĳ�ʼλ��
	m_pViewCenter.SetPoint(0,0);          //��ͼ����λ��
	//m_pCursor.SetPoint(0,0);        //�������λ��
	//m_pCursor->x=0;
	//m_pCursor->y=0;

	m_pQYText[0].x = ZRect.left + ZRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[0].y = ZRect.top + ZRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[1].x = ZTRect.left + ZTRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[1].y = ZTRect.top + ZTRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[2].x = ZDRcet.left + ZDRcet.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[2].y = ZDRcet.top + ZDRcet.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[3].x = XRect.left + XRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[3].y = XRect.top + XRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[4].x = XTRect.left + XTRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[4].y = XTRect.top + XTRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[5].x = XDRcet.left + XDRcet.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[5].y = XDRcet.top + XDRcet.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[6].x = HRect.left + HRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[6].y = HRect.top + HRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[7].x = THRect.left + THRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[7].y = THRect.top + THRect.Height()/2 -22;		//����8��������ע���λ��

	m_iYUcenter[0].x = ZRect.left + ZRect.Width()/2;		
	m_iYUcenter[0].y = ZRect.top + ZRect.Height()/2;		//

	m_iYUcenter[1].x = ZTRect.left + ZTRect.Width()/2;		//
	m_iYUcenter[1].y = ZTRect.top + ZTRect.Height()/2;		//

	m_iYUcenter[2].x = ZDRcet.left + ZDRcet.Width()/2;		//����8��������ע���λ��
	m_iYUcenter[2].y = ZDRcet.top + ZDRcet.Height()/2;		//����8��������ע���λ��

	m_iYUcenter[3].x = XRect.left + XRect.Width()/2;		//����8��������ע���λ��
	m_iYUcenter[3].y = XRect.top + XRect.Height()/2;		//����8��������ע���λ��

	m_iYUcenter[4].x = XTRect.left + XTRect.Width()/2;		//����8��������ע���λ��
	m_iYUcenter[4].y = XTRect.top + XTRect.Height()/2;		//����8��������ע���λ��

	m_iYUcenter[5].x = XDRcet.left + XDRcet.Width()/2;		//����8��������ע���λ��
	m_iYUcenter[5].y = XDRcet.top + XDRcet.Height()/2;		//����8��������ע���λ��

	m_iYUcenter[6].x = HRect.left + HRect.Width()/2;		//����8��������ע���λ��
	m_iYUcenter[6].y = HRect.top + HRect.Height()/2;		//����8��������ע���λ��

	m_iYUcenter[7].x = THRect.left + THRect.Width()/2;		//����8��������ע���λ��
	m_iYUcenter[7].y = THRect.top + THRect.Height()/2;		//����8��������ע���λ��

	for(int i=0;i<8;i++)
	{
		//////////�ټ�����Ϣ////////////////////////////////////////////////////////
		m_iQuYuZhu[i]=0;    //����ÿ�������µ�ע��
		m_iMaxZhu[i]=0;     //����ÿ������������µ�ע��
		m_iMyZhu[i]=0;    //��������Լ���ÿ�������µ�ע��
		//m_iYUcenter[i].x=0;     //ÿ����������ĵ�λ��  
		//m_iYUcenter[i].y=0;     //ÿ����������ĵ�λ��  
		///////////////////////////////////////////////////////////////////////////
	}

	m_iCardNum=0;			//���ŵڼ����ƵĶ���
	m_bZhuang=false;			//������ׯ�һ����мҵ���
	m_bCardMove = false;				//�Ƿ����ڽ��з���
	m_bSendCard = false;			//ȷ���Ƿ��ƽ���
	memset(&m_iNowCard, 0, sizeof(m_iNowCard));		 //��ǰ�Ƶ�  
	
	memset(&m_CardCount, 0, sizeof(m_CardCount));		 //ׯ���Ƶ�����

	memset(&m_iResult, 0, sizeof(m_iResult));		 ////���汾�֣�ׯ�Ƶ㣬���Ƶ㣬ׯ���ͣ������ͣ�Ӯ�������Լ��ý�� 

	memset(&m_iZPaiXing, 0, sizeof(m_iZPaiXing)); //ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	memset(&m_iXPaiXing, 0, sizeof(m_iXPaiXing));//�м�����
	memset(&m_iMePlayData, 0, sizeof(m_iMePlayData));	 //����Լ�����Ӯ���
	memset(&m_byMePlayData, 0, sizeof(m_byMePlayData));	 //����Լ�����Ӯ���

	memset(&m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	memset(&m_iMyZhu, 0, sizeof(m_iMyZhu));	////��������Լ���ÿ�������µ�ע��

	m_iWinner = 0 ;				//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	m_iKaiPai = 0 ;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���

#ifdef VIDEO
	m_bWatch = true;    // �Լ��Ƿ�Ϊ��ս
#endif

	SetNameColor(RGB(255,255,0));	//����Ĭ���û�����ɫ
	m_iNtStation = -1;
	m_byCurOrientation = 255;
	m_byWaitingType = NO_WAITING;
	memset(m_uTime,0,sizeof(m_uTime));	
	m_iChooseNote =0;//��ǰѡ��ĳ��� 0û�У�1��100��2��1000��3��1w��4��10w��5��100w,6��500w
	m_iChooseRect = -1; //������ڵ��������
	m_byMyStation =255;//�Լ���λ��
	m_iSendCardZhen = 25;
///lym2010-03-19�ܷ�����ׯ��ť
	m_bCanShangZhuang = true;
/////////////////////////////////////////////////
	m_iNowJuShu = 0;	//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
	memset(&m_iAIMoney, 0, sizeof(m_iAIMoney));	///�����˵���ע
	m_bHideAIMoney = true;			///�Ƿ����ػ�������ע
	m_sSuperInfo = "";			///�����ͻ��˿�����Ϣ
	m_bShowSuper = false;			//��ʾ�����ͻ��˿�����Ϣ

	///��ǰ����������µ���ע
	m_iMaxNote = 0;	

	//������72����ע��Ϣ����Ӯ���
	memset(m_iMeXiaZhuInfo, 0, sizeof(m_iMeXiaZhuInfo));
	memset(m_iHistoryPlayData, 0, sizeof(m_iHistoryPlayData));
	memset(m_iHistoryDuiZi,0,sizeof(m_iHistoryDuiZi));


	CString s = CINIFile::GetAppPath ();/////����·��
	CBcfFile f( s + SKIN_FOLDER + _T(".bcf"));
	CString strKey = TEXT("config");
	m_iGamePictureType = f.GetKeyVal(strKey, "GamePictureType", 0);
	m_ZhuangStr = f.GetKeyVal(strKey, "ZhuangStr", "ׯ");
	m_XiangStr = f.GetKeyVal(strKey, "XiangStr", "��");
	m_bUse500WJeton =f.GetKeyVal("config","Use500WJeton",0)>0?true:false;

	m_bShowPromptOpenCard = false;	///< �Ƿ���ʾ��ʾ����ͼƬ
	m_bShowPromptChangeNt = false;	///< �Ƿ���ʾ��ʾ��ׯͼƬ
}

CClientPlayView::~CClientPlayView(void)
{
	// ����Ҫ��
	if (NULL != m_HaveThing.GetSafeHwnd())
	{
		m_HaveThing.DestroyWindow();
	}

	// ����Ҫ�ߴ𸴶Ի���
	if (NULL != m_ThingMessage.GetSafeHwnd())
	{
		m_ThingMessage.DestroyWindow();
	}
}

///�����û�����
void CClientPlayView::UpdateUserInfo(BYTE byViewStation)
{
	UpdateViewFace(NULL);
}

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
HRESULT  CClientPlayView::GetRelativePos(CPoint &PtItem, CBcfFile &f, const CString &csSecName,
						const CString &csKeyName, const int iUserView, const CRect &rectClient)
{
	PtItem.SetPoint(0,0);           // ��ʼ��ԭʼ����Ϊ0
	CPoint PtTemp = CPoint(0,0);    // ʹ����ʱ�������
	CString csValueName = _T("");   // �ؼ�����

	// x ���λ��
	csValueName.Format("%s%dx_type", csKeyName, iUserView);
	CString csTypeX = f.GetKeyVal(csSecName, csValueName, _T(""));
	csTypeX.Trim(_T(" "));
	if (0 == csTypeX.CompareNoCase(_T("Left")))                     // �����߿�
	{
		PtTemp.x = rectClient.left;
	}
	else if (0 == csTypeX.CompareNoCase(_T("Right")))               // ����ұ߿�
	{
		PtTemp.x = rectClient.right;
	}
	else if ((0 == csTypeX.CompareNoCase(_T("Mid")))
		|| (0 == csTypeX.CompareNoCase(_T("Mid.x"))))               // ����
	{
		PtTemp.x = rectClient.Width()/2;
	}
	else if (!csTypeX.IsEmpty())     // �������Ԫ��λ�ã����õݹ鷽ʽ���������ļ��в��ܴ���ѭ����
	{
		CPoint PtItemTemp(0,0);
		GetRelativePos(PtItemTemp, f, csSecName, csTypeX, iUserView, rectClient);
		PtTemp.x = PtItemTemp.x;
	}
	
	// ��ȡy����
	csValueName.Format("%s%dy_type", csKeyName, iUserView);
	CString csTypeY = f.GetKeyVal(csSecName, csValueName, _T(""));
	csTypeY.Trim(_T(" "));
	if (0 == csTypeY.CompareNoCase(_T("Top")))                     // ��Զ���
	{
		PtTemp.y = rectClient.top;
	}
	else if (0 == csTypeY.CompareNoCase(_T("Bottom")))             // ��Եײ�
	{
		PtTemp.y = rectClient.bottom;
	}
	else if ((0 == csTypeY.CompareNoCase(_T("Mid")))
		|| (0 == csTypeY.CompareNoCase(_T("Mid.y"))))              // ����
	{
		PtTemp.y = rectClient.Height()/2;
	}	
	else if (!csTypeY.IsEmpty())    // �������Ԫ��λ�ã����õݹ鷽ʽ���������ļ��в��ܴ���ѭ����
	{
		CPoint PtItemTemp(0,0);
		GetRelativePos(PtItemTemp, f, csSecName, csTypeY, iUserView, rectClient);
		PtTemp.y= PtItemTemp.y;
	}

	// ��ȡx������
	csValueName.Format("%s%dx", csKeyName, iUserView);
	PtTemp.x += f.GetKeyVal(csSecName, csValueName, rectClient.Width()/2);

	// ��ȡy������
	csValueName.Format("%s%dy", csKeyName, iUserView);
	PtTemp.y += f.GetKeyVal(csSecName, csValueName, rectClient.Height()/2);
	PtItem = PtTemp;
	return 0;
}

///����λ�õ���
void CClientPlayView::FixControlSize(int iWidth, int iHeight)
{
	//OutputDebugString("CClientPlayView::FixControlSize");
	if (NULL == m_hWnd)
	{
		return ;
	}

	CRect rect(0,0,iWidth, iHeight);
	//�ƶ�����
	int iMidLine=iWidth/2;			// x�м���
	int iBaseLine=iHeight/2;		// y�м���

	m_pViewCenter.SetPoint(iMidLine,iBaseLine);          //��ͼ����λ��

	//��ʱ��λ��lym0515
	for(int i=0;i<180;i++)
	{
		m_PtTime[i].x=iMidLine-55;
		m_PtTime[i].y=iBaseLine+200;
	}	

	ZRect.SetRect(iMidLine+156,iBaseLine-110,iMidLine+360,iBaseLine+82);  // ׯ��ע����
	ZTRect.SetRect(iMidLine+156,iBaseLine+85,iMidLine+360,iBaseLine+155); // ׯ������ע����
	ZDRcet.SetRect(iMidLine+156,iBaseLine-181,iMidLine+360,iBaseLine-113); // ׯ������ע����
	XRect.SetRect(iMidLine-359,iBaseLine-110,iMidLine-155,iBaseLine+82); //  ����ע����
	XTRect.SetRect(iMidLine-359,iBaseLine+85,iMidLine-155,iBaseLine+155); // ��������ע����
	XDRcet.SetRect(iMidLine-359,iBaseLine-181,iMidLine-155,iBaseLine-113); // �ж�����ע����
	HRect.SetRect(iMidLine-152,iBaseLine-39,iMidLine+153,iBaseLine+82); //  ����ע����
	THRect.SetRect(iMidLine-152,iBaseLine+85,iMidLine+153,iBaseLine+155);//  ͬ�����ע����


 //XiaZhuRect;//��ע���򣺣�0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���

	m_pMoveStart.SetPoint(iMidLine+247,iBaseLine-294);       //���ƶ����Ŀ�ʼλ��
	m_pCardZEnd.SetPoint(iMidLine+14,iBaseLine-138);         //ׯ���ƽ���λ��
	m_pCardXEnd.SetPoint(iMidLine-138+14,iBaseLine-138);        //�м��ƽ���λ��
	m_pTextStation.SetPoint(iMidLine-130,iBaseLine-260);		//�Ϸ���������λ��
	m_pMeInfo.SetPoint(iMidLine-391,iBaseLine+250);		   //����Լ���Ϣ��ʾ�ĳ�ʼλ��
	m_pNtList.SetPoint(iMidLine +170 ,iBaseLine -285 );		   //ׯ���б�ĳ�ʼλ��

	m_pNtInfo.SetPoint(iMidLine-75,iBaseLine-324);		   //ׯ����Ϣ��ʾ�ĳ�ʼλ��
	m_pXing.SetPoint(iMidLine-125,iBaseLine+251);		   //����ͼƬ�ĳ�ʼλ��
	m_p10ResultInfo.SetPoint(iMidLine-362,iBaseLine-260);   //���10�ֵ��Ƶ���Ϣ��ʼλ��
	m_pXZXianZhi.SetPoint(iMidLine-310,iBaseLine-330);		 //�·�������ʾ�ĳ�ʼλ��
	m_pZXcount.SetPoint(iMidLine +342 ,iBaseLine + 251);        //���30�ֿ�ׯ���д�������Ϣ��ʼλ��
	m_pDitu.SetPoint(iMidLine-155  ,iBaseLine - 183 );          //�����ͼ�ĳ�ʼλ��


	m_pQYText[0].x = ZRect.left + ZRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[0].y = ZRect.top + ZRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[1].x = ZTRect.left + ZTRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[1].y = ZTRect.top + ZTRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[2].x = ZDRcet.left + ZDRcet.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[2].y = ZDRcet.top + ZDRcet.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[3].x = XRect.left + XRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[3].y = XRect.top + XRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[4].x = XTRect.left + XTRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[4].y = XTRect.top + XTRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[5].x = XDRcet.left + XDRcet.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[5].y = XDRcet.top + XDRcet.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[6].x = HRect.left + HRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[6].y = HRect.top + HRect.Height()/2 -22;		//����8��������ע���λ��

	m_pQYText[7].x = THRect.left + THRect.Width()/2 - 8;		//����8��������ע���λ��
	m_pQYText[7].y = THRect.top + THRect.Height()/2 -22;		//����8��������ע���λ��

	m_iYUcenter[0].x = ZRect.left + ZRect.Width()/2 ;		//
	m_iYUcenter[0].y = ZRect.top + ZRect.Height()/2 ;		//

	m_iYUcenter[1].x = ZTRect.left + ZTRect.Width()/2 ;		//
	m_iYUcenter[1].y = ZTRect.top + ZTRect.Height()/2 ;		//

	m_iYUcenter[2].x = ZDRcet.left + ZDRcet.Width()/2 ;		//	
	m_iYUcenter[2].y = ZDRcet.top + ZDRcet.Height()/2 ;		//

	m_iYUcenter[3].x = XRect.left + XRect.Width()/2;		//
	m_iYUcenter[3].y = XRect.top + XRect.Height()/2;		//

	m_iYUcenter[4].x = XTRect.left + XTRect.Width()/2;		//
	m_iYUcenter[4].y = XTRect.top + XTRect.Height()/2;		//

	m_iYUcenter[5].x = XDRcet.left + XDRcet.Width()/2;		//
	m_iYUcenter[5].y = XDRcet.top + XDRcet.Height()/2;		//

	m_iYUcenter[6].x = HRect.left + HRect.Width()/2;		//
	m_iYUcenter[6].y = HRect.top + HRect.Height()/2;		//

	m_iYUcenter[7].x = THRect.left + THRect.Width()/2;		//
	m_iYUcenter[7].y = THRect.top + THRect.Height()/2;		//


	CRect rectClient;
	GetClientRect(&rectClient);

	//�ı���ְ�,x����
	m_showcoredlg.GetClientRect(&rect);
	m_showcoredlg.SetWindowPos(NULL,rectClient.right-10-rect.Width(),-m_showcoredlg.m_showh,0,0,SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE);

	// �ƶ���ť
	// ��ʼ��ť
	m_btnStart.GetClientRect(&rect);
	m_btnStart.MoveWindow(iMidLine-rect.Width()-20,iHeight-200,rect.Width(),rect.Height());
	m_btnLeave.MoveWindow(iMidLine+20,iHeight-200,rect.Width(),rect.Height());

	// �ƶ����ơ���������ʾ��ť
	m_btnOutCard.GetClientRect(&rect);
	m_btnOutCard.MoveWindow(iMidLine-2*rect.Width(), iHeight-200, rect.Width(),rect.Height());
	m_btnPass.MoveWindow(iMidLine-rect.Width()/2, iHeight-200, rect.Width(),rect.Height());
	m_btnHint.MoveWindow(iMidLine+rect.Width(), iHeight-200, rect.Width(),rect.Height());


///72��һ��
#ifdef SET72 
 	//���밴ťλ��
	m_btn100.GetClientRect(&rect);
	m_btn100.MoveWindow(iWidth/2+27 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());

	m_btn1000.GetClientRect(&rect);
	m_btn1000.MoveWindow(iWidth/2+90 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn1w.GetClientRect(&rect);
	m_btn1w.MoveWindow(iWidth/2+152 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn10w.GetClientRect(&rect);
	m_btn10w.MoveWindow(iWidth/2+213 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn100w.GetClientRect(&rect);
	m_btn100w.MoveWindow(iWidth/2+275 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn500w.GetClientRect(&rect);
	m_btn500w.MoveWindow(iWidth/2+336 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());    


#else

	//���밴ťλ��
	m_btn100.GetClientRect(&rect);
	m_btn100.MoveWindow(iWidth/2+43 , iHeight/2+185,rect.Width(),rect.Height());
	m_btn1000.GetClientRect(&rect);
	m_btn1000.MoveWindow(iWidth/2+94 , iHeight/2+181,rect.Width(),rect.Height());
	m_btn1w.GetClientRect(&rect);
	m_btn1w.MoveWindow(iWidth/2+149 , iHeight/2+181,rect.Width(),rect.Height());
	m_btn10w.GetClientRect(&rect);
	m_btn10w.MoveWindow(iWidth/2+203 , iHeight/2+177,rect.Width(),rect.Height());
	m_btn100w.GetClientRect(&rect);
	m_btn100w.MoveWindow(iWidth/2+261 , iHeight/2+174,rect.Width(),rect.Height());
	m_btn500w.GetClientRect(&rect);
	m_btn500w.MoveWindow(iWidth/2+322 , iHeight/2+170,rect.Width(),rect.Height());
#endif

	//���밴ťλ��
	m_btn100.GetClientRect(&rect);
	m_btn100.MoveWindow(iWidth/2+27 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn1000.GetClientRect(&rect);
	m_btn1000.MoveWindow(iWidth/2+90 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn1w.GetClientRect(&rect);
	m_btn1w.MoveWindow(iWidth/2+152 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn10w.GetClientRect(&rect);
	m_btn10w.MoveWindow(iWidth/2+213 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn100w.GetClientRect(&rect);
	m_btn100w.MoveWindow(iWidth/2+275 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());
	m_btn500w.GetClientRect(&rect);
	m_btn500w.MoveWindow(iWidth/2+336 , iHeight/2+234-rect.Height(),rect.Width(),rect.Height());  

	NoMouseRect.SetRect(iWidth/2+41-16,iHeight/2+234-rect.Height()-16,iWidth/2+315+rect.Width()+16,iHeight/2+224+16);			///<����ֹ��������

	//int iMidLine=iWidth/2;			// x�м���
	//int iBaseLine=iHeight/2;		// y�м���

	// ��Ϣ�ƶ���ť
	m_btnleft.GetClientRect(&rect);
	m_btnleft.MoveWindow(iMidLine-200, iBaseLine+270,rect.Width(),rect.Height());
	// ��Ϣ�ƶ��Ұ�ť
	m_btnright.GetClientRect(&rect);
	m_btnright.MoveWindow(iMidLine+302, iBaseLine+270,rect.Width(),rect.Height());
	//��·�Ӱ�ť
	m_btnluzi.GetClientRect(&rect);
	m_btnluzi.MoveWindow(iMidLine-400, iBaseLine+201,rect.Width(),rect.Height());
	NoMouseRect1.SetRect(iMidLine-400-16,iBaseLine+201-16,iMidLine-395+rect.Width()+16,iBaseLine+201+rect.Height()+16);

	//������ׯ��ť
	m_btnshangz.GetClientRect(&rect);
	m_btnshangz.MoveWindow(iMidLine+305, iBaseLine-340,rect.Width(),rect.Height());
	NoMouseRect2.SetRect(iMidLine+305-16,iBaseLine-340-16,iMidLine+305+rect.Width()+16,iBaseLine-340+rect.Height()+16);

	//���а�ť
	m_btnbank.GetClientRect(&rect);
	m_btnbank.MoveWindow(iMidLine-395+78, iBaseLine+201,rect.Width(),rect.Height());
	NoMouseRect3.SetRect(iMidLine-295-16,iBaseLine+201-16,iMidLine-295+rect.Width()+16,iBaseLine+201+rect.Height()+16);


	//��ׯ�б��Ϸ���ť
	m_btTop.GetClientRect(&rect);
	m_btTop.MoveWindow(m_pNtList.x+242-23, m_pNtList.y-27,rect.Width(),rect.Height());

	//��ׯ�б��·���ť
	m_btBottom.GetClientRect(&rect);
	m_btBottom.MoveWindow(m_pNtList.x+242-23, m_pNtList.y+61-20,rect.Width(),rect.Height());



	///<����ֹ��������

	//�����ͻ������λ�������ע�İ�ť
	m_btnHideAIMoney.GetClientRect(&rect);
	m_btnHideAIMoney.MoveWindow(iMidLine-395+170, iBaseLine+218,rect.Width(),rect.Height());

	//�����ͻ�������ʾ�����˿�����Ϣ��ť
	m_btnShowAIInfo.GetClientRect(&rect);
	m_btnShowAIInfo.MoveWindow(iMidLine-395+255, iBaseLine+218,rect.Width(),rect.Height());


	//�����ͻ��˿��ƿ�ׯ�İ�ť
	m_btnCtrZhuang.GetClientRect(&rect);
	m_btnCtrZhuang.MoveWindow(iMidLine-300, iBaseLine+218-45,rect.Width(),rect.Height());

	//�����ͻ��˿��ƿ��еİ�ť
	m_btnCtrXian.GetClientRect(&rect);
	m_btnCtrXian.MoveWindow(iMidLine-300+60, iBaseLine+218-45,rect.Width(),rect.Height());

	//�����ͻ��˿��ƿ��͵İ�ť
	m_btnCtrHe.GetClientRect(&rect);
	m_btnCtrHe.MoveWindow(iMidLine-300+120, iBaseLine+218-45,rect.Width(),rect.Height());

	///�����ͻ��˻ָ����Ƶİ�ť
	m_btnCtrNormal.GetClientRect(&rect);
	m_btnCtrNormal.MoveWindow(iMidLine-300+180, iBaseLine+218-45,rect.Width(),rect.Height());

	///��ʾ�����Ի���İ�ť
	m_btnCtrSuper.GetClientRect(&rect);
	m_btnCtrSuper.MoveWindow(iMidLine-300+180 + 100, iBaseLine+218-45,rect.Width(),rect.Height());


/////////���ø�����ע����

	CGameImage reimg;
	TCHAR path[MAX_PATH];
	wsprintf(path,TEXT("image\\result.bmp"));
	reimg.SetLoadInfo(path,true);	 
	CPoint point(m_pDitu.x,m_pDitu.y);
	//ClientToScreen(&point);
	//CRect rect(0,0,0,0);
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg, m_PlayView);
	if (NULL != pGameDlg)
	{
		pGameDlg->GetWindowRect(&rect);
	}

}

int CClientPlayView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	////OutputDebugString("CClientPlayView::OnCreate");

	if (CUsingSkinClass::OnCreate(lpCreateStruct) == -1)
		return -1;

	////OutputDebugString("CClientPlayView::OnCreate _22");

	// ����ͼƬ
	TCHAR szPath[MAX_PATH]={0};

	m_Result.Create(IDD_RESULT,this);
	//m_Info.Create(IDD_RESULT,this);

	//m_Result.ClearAll();

#ifdef VIDEO
	_stprintf_s(szPath, sizeof(szPath), TEXT("image\\VideoFrame.bmp"), SKIN_FOLDER);
	m_pVideoFrame.SetLoadInfo(szPath,false);
#else
	if(m_iUserFrame.GetWidth() <= 0)
	{
		_stprintf_s(szPath, sizeof(szPath), TEXT("image\\logoframe.bmp"),SKIN_FOLDER);
		SetFrameImage(szPath);
	}
#endif

	// ����ͼ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\BackColorOnly.bmp"), SKIN_FOLDER);
	m_imgBackground.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	// ��������ͼƬ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\centerlogo.bmp"), SKIN_FOLDER);
	m_imgCenterLogo.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	///< �����ͼͼƬCenterLogo2.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\centerlogo2.bmp"), SKIN_FOLDER);
	m_imgCenterLogo2.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	// ������ͼƬ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\AwardPoint.bmp"), SKIN_FOLDER);
	m_imgAwardPint.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	// ׯ��ͼƬ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\nt.bmp"), SKIN_FOLDER);
	m_imgNT.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	// �й�ͼƬ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\tuoguan.bmp"), SKIN_FOLDER);
	m_imgTrusteeLogo.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	// ����ͼƬ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\pass.bmp"), SKIN_FOLDER);
	m_imgPass.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	// ������˳��
	_stprintf_s(szPath, sizeof(szPath), _T("image\\finish_out1.bmp"), SKIN_FOLDER);
	m_imgPicSequence.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	// ����
	_stprintf_s(szPath, sizeof(szPath), _T("image\\CurOutCard.bmp"),SKIN_FOLDER);
	m_imgFangXiang.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	// �ȴ���ҿ�ʼͼƬ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\waiting.bmp"),SKIN_FOLDER);
	m_imgWaiting.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	//����ͼƬ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\xingxing.bmp"),SKIN_FOLDER);
	m_imgXing.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	
              
	//Ѻע���������ͼƬ // ��������ͼƬ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\xiazhu.bmp"),SKIN_FOLDER);
	m_imgChoumaNum.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	// ����Լ���ע�ĳ�������ͼƬgreen_num.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\green_num.bmp"),SKIN_FOLDER);
	m_imgMyNum.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	// ��ͼƬxiazhu.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("cardskin\\game_CARD_BACK_SELECT.bmp"),SKIN_FOLDER);
	m_imgWang.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	// ��ͨ��ͼƬxiazhu.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\%s\\GAME_CARD_SELECT.bmp"),SKIN_CARD);
	m_imgCard.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	// ����ͼƬ
	if(m_bUse500WJeton)
	{
    	_stprintf_s(szPath, sizeof(szPath), _T("image\\jeton1.bmp"),SKIN_FOLDER);
	}
	else
	{
		_stprintf_s(szPath, sizeof(szPath), _T("image\\jeton.bmp"),SKIN_FOLDER);
	}

	m_imgchouma.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	// �����ͼditu.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\ditu.bmp"),SKIN_FOLDER);
	m_imgditu.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);
	// �������ֵ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\dianshu.bmp"),SKIN_FOLDER);
	m_imgPointValue.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);
	// ������������
	_stprintf_s(szPath, sizeof(szPath), _T("image\\TwoKing.bmp"),SKIN_FOLDER);
	m_imgTwoKing.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);
	

	// ���ͼƬcursor.bmp m_imgCursor
	_stprintf_s(szPath, sizeof(szPath), _T("image\\cursor.bmp"),SKIN_FOLDER);
	m_imgCursor.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

    // ����ͼƬresult.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\result.bmp"),SKIN_FOLDER);
	m_imgResult.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

    // ����ͼƬresult1.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\result1.bmp"),SKIN_FOLDER);
	m_imgResult1.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	


	/////////////////////////////////////////////////////Jing

	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_zhuang_dot.bmp"),SKIN_FOLDER);
	m_imgShowZ_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_xian.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_xian_dot.bmp"),SKIN_FOLDER);
	m_imgShowX_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_he.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_he_dot.bmp"),SKIN_FOLDER);
	m_imgShowH_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_zhuang_win_dot.bmp"),SKIN_FOLDER);
	m_imgShowZ_Win_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_xian.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_xian_win_dot.bmp"),SKIN_FOLDER);
	m_imgShowX_Win_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_he.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_he_win_dot.bmp"),SKIN_FOLDER);
	m_imgShowH_Win_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾׯȦͼƬshow_zhuang.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_zhuang_x_dot.bmp"),SKIN_FOLDER);
	m_imgShowZ_x_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_xian.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_xian_x_dot.bmp"),SKIN_FOLDER);
	m_imgShowX_x_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_he.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_he_x_dot.bmp"),SKIN_FOLDER);
	m_imgShowH_x_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾׯȦͼƬ��Ӯ show_zhuang_lost_x.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_zhuang_lost_x_dot.bmp"),SKIN_FOLDER);
	m_imgShowZ_Lost_x_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬ��Ӯ show_xian_lost_x.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_xian_lost_x_dot.bmp"),SKIN_FOLDER);
	m_imgShowX_Lost_x_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬ��Ӯ show_he_lost_x.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_he_lost_x_dot.bmp"),SKIN_FOLDER);
	m_imgShowH_Lost_x_dot.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	
	
	///< ��ʾ����ͼƬPromptOpenCard.bmp add xyh 2011-12-7
	_stprintf_s(szPath, sizeof(szPath), _T("image\\PromptOpenCard.bmp"),SKIN_FOLDER);
	m_imgPromptOpenCard.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< ��ʾ��ׯͼƬPromptChangeNt.bmp add xyh 2011-12-7
	_stprintf_s(szPath, sizeof(szPath), _T("image\\PromptChangeNt.bmp"),SKIN_FOLDER);
	m_imgPromptChangeNt.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);


	/////////////////////////////////////////////////////

	///< �·���ʾׯȦͼƬshow_zhuang.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_zhuang.bmp"),SKIN_FOLDER);
	m_imgShowZ.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_xian.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_xian.bmp"),SKIN_FOLDER);
	m_imgShowX.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_he.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_he.bmp"),SKIN_FOLDER);
	m_imgShowH.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾׯȦͼƬ��Ӯ show_zhuang_win.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_zhuang_win.bmp"),SKIN_FOLDER);
	m_imgShowZ_Win.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬ��Ӯ show_xian_win.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_xian_win.bmp"),SKIN_FOLDER);
	m_imgShowX_Win.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬ��Ӯ show_he_win.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_he_win.bmp"),SKIN_FOLDER);
	m_imgShowH_Win.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	


	///< �·���ʾׯȦͼƬshow_zhuang.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_zhuang_x.bmp"),SKIN_FOLDER);
	m_imgShowZ_x.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_xian.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_xian_x.bmp"),SKIN_FOLDER);
	m_imgShowX_x.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬshow_he.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_he_x.bmp"),SKIN_FOLDER);
	m_imgShowH_x.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾׯȦͼƬ��Ӯ show_zhuang_lost_x.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_zhuang_lost_x.bmp"),SKIN_FOLDER);
	m_imgShowZ_Lost_x.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬ��Ӯ show_xian_lost_x.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_xian_lost_x.bmp"),SKIN_FOLDER);
	m_imgShowX_Lost_x.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	

	///< �·���ʾ��ȦͼƬ��Ӯ show_he_lost_x.bmp
	_stprintf_s(szPath, sizeof(szPath), _T("image\\show_he_lost_x.bmp"),SKIN_FOLDER);
	m_imgShowH_Lost_x.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);	


	// ������ť
	m_btnStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,72,31),this,IDC_BTN_START);   // ��ʼ��ť
	m_btnLeave.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,72,31),this,IDC_BTN_LEAVE);   // �뿪��ť

	//m_LuziDlg.SetWindowPos(&m_btnStart, 0,0,0,0, SWP_NOSIZE);
	//m_LuziDlg.MoveWindow(0,0,100,100);
	// ������Ϸ��ʼ���뿪��ť
	m_btnStart.ShowWindow(SW_HIDE);
	m_btnLeave.ShowWindow(SW_HIDE);

	///�ټ��ֳ��밴ť
	m_btn100.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_100);
	m_btn1000.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_1000);
	m_btn1w.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_W);
	m_btn10w.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_10W);
	m_btn100w.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_100W);
	m_btn500w.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_500W);
	/// ��Ϣ�ƶ���ť
	m_btnleft.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_LEFT);
	/// ��Ϣ�ƶ��Ұ�ť
	m_btnright.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_RIGHT);

	m_btnluzi.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_LUZI);
	///������ׯ��ť
	m_btnshangz.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_SHANGZ);
	///���а�ť
	m_btnbank.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_BANK);
	///�����ͻ������λ�������ע��ť
	m_btnHideAIMoney.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_HIDE_MONEY);
	///�����ͻ�����ʾ�����˿�����Ϣ�İ�ť
	m_btnShowAIInfo.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_SHOW_AI_INFO);


	///�����ͻ��˿��ƿ�ׯ�İ�ť
	m_btnCtrZhuang.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_SUPER_ZHUANG);
	///�����ͻ��˿��ƿ��еİ�ť
	m_btnCtrXian.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_SUPER_XIAN);
	///�����ͻ��˿��ƿ��͵İ�ť
	m_btnCtrHe.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_SUPER_HE);
	///�����ͻ��˻ָ����Ƶİ�ť
	m_btnCtrNormal.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_SUPER_NORMAL);

	///��ʾ�����Ի���İ�ť
	m_btnCtrSuper.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_SUPER_DLG);


	m_btTop.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_TOP);

	m_btBottom.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDD_CHOUMA_BOTTOM);

	m_btn100.ShowWindow(SW_SHOW);
	m_btn1000.ShowWindow(SW_SHOW);
	m_btn1w.ShowWindow(SW_SHOW);
	m_btn10w.ShowWindow(SW_SHOW);
	m_btn100w.ShowWindow(SW_SHOW);
	m_btn500w.ShowWindow(SW_SHOW);
	m_btnleft.ShowWindow(SW_SHOW);
	m_btnluzi.ShowWindow(SW_SHOW);
	m_btnright.ShowWindow(SW_SHOW);
	m_btnshangz.ShowWindow(SW_SHOW);
	m_btnbank.ShowWindow(SW_SHOW);
	m_btnHideAIMoney.ShowWindow(SW_HIDE);
	m_btnCtrZhuang.ShowWindow(SW_HIDE);
	m_btnCtrXian.ShowWindow(SW_HIDE);
	m_btnCtrHe.ShowWindow(SW_HIDE);
	m_btnCtrNormal.ShowWindow(SW_HIDE);
	m_btnCtrSuper.ShowWindow(SW_HIDE);
	m_btnShowAIInfo.ShowWindow(SW_HIDE);

	m_btn100.EnableWindow(true);
	m_btn1000.EnableWindow(true);
	m_btn1w.EnableWindow(true);
	m_btn10w.EnableWindow(true);
	m_btn100w.EnableWindow(true);
	m_btn500w.EnableWindow(true);
	m_btnleft.EnableWindow(true);
	m_btnright.EnableWindow(true);
	m_btnluzi.EnableWindow(true);
	m_btnshangz.EnableWindow(false);
	m_btnbank.EnableWindow(true);
	m_btnHideAIMoney.EnableWindow(false);
	m_btnShowAIInfo.EnableWindow(false);
	m_btnCtrZhuang.EnableWindow(false);
	m_btnCtrXian.EnableWindow(false);
	m_btnCtrHe.EnableWindow(false);
	m_btnCtrNormal.EnableWindow(false);
	m_btnCtrSuper.EnableWindow(false);
	m_btTop.EnableWindow(SW_SHOW);
	m_btBottom.EnableWindow(SW_SHOW);

	/// ����/������/��ʾ��ť
	m_btnOutCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDC_BTN_OUTCARD);
	m_btnPass.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDC_BTN_PASS);
	m_btnHint.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,72,31),this,IDC_BTN_HINT);
	m_btnOutCard.ShowWindow(SW_HIDE);
	m_btnPass.ShowWindow(SW_HIDE);
	m_btnHint.ShowWindow(SW_HIDE);


	/// �����Ʊ�(һ����Ҫ���͵���)
	m_SendBaseCard.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CRect(0,0,0,0), this, IDD_CENTER_CARD,NULL);
	m_SendBaseCard.SetShowCard(true);
	m_SendBaseCard.SetCardShowNum(1);
	m_SendBaseCard.InitGameCard(GetParent());
	m_SendBaseCard.SetUseMouse(false);
	m_SendBaseCard.SetCardParameter(2,1, 1);   // �����Ƽ����
	
	/// һ������
	m_MingCard.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CRect(0,0,0,0), this, IDD_CENTER_CARD+1,NULL);
	m_MingCard.SetShowCard(true);
	m_MingCard.InitGameCard(GetParent());
	m_MingCard.SetUseMouse(false);

	CGameImage imgTemp;
	HRGN hRgn = NULL;
	/// ��ʼ��ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\start_bt.bmp"), SKIN_FOLDER);
	m_btnStart.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnStart.SetWindowRgn(hRgn,true);
	}

	// �뿪��ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\leave_bt.bmp"), SKIN_FOLDER);
	m_btnLeave.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnLeave.SetWindowRgn(hRgn,true);
	}

	// ���ư�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\outcard_bt.bmp"), SKIN_FOLDER);
	m_btnOutCard.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnOutCard.SetWindowRgn(hRgn,true);
	}

	// ������ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\pass_bt.bmp"), SKIN_FOLDER);
	m_btnPass.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnPass.SetWindowRgn(hRgn,true);
	}

	// ��ʾ
	_stprintf_s(szPath, sizeof(szPath), _T("image\\hint_bt.bmp"), SKIN_FOLDER);
	m_btnHint.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnHint.SetWindowRgn(hRgn,true);
	}
///////���밴ť
		// 100��ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\100_bt.bmp"), SKIN_FOLDER);
	m_btn100.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btn100.SetWindowRgn(hRgn,true);
	}
		// 1000��ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\1000_bt.bmp"), SKIN_FOLDER);
	m_btn1000.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btn1000.SetWindowRgn(hRgn,true);
	}
		// 1w��ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\1W_bt.bmp"), SKIN_FOLDER);
	m_btn1w.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btn1w.SetWindowRgn(hRgn,true);
	}
		// 10w��ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\10W_bt.bmp"), SKIN_FOLDER);
	m_btn10w.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btn10w.SetWindowRgn(hRgn,true);
	}
		// 100w��ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\100W_bt.bmp"), SKIN_FOLDER);
	m_btn100w.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btn100w.SetWindowRgn(hRgn,true);
	}
	// 500w��ť
#ifdef SET72
	if(m_bUse500WJeton)
	{
		_stprintf_s(szPath, sizeof(szPath), _T("image\\500W_bt.bmp"), SKIN_FOLDER);
	}
	else
	{
		_stprintf_s(szPath, sizeof(szPath), _T("image\\1000W_bt.bmp"), SKIN_FOLDER); 
		MsgPrint("zhangjing::BBBBm_bUse500WJeton=%d",m_bUse500WJeton);
	}

	
#else
	_stprintf_s(szPath, sizeof(szPath), _T("image\\500W_bt.bmp"), SKIN_FOLDER);	
#endif
	
	m_btn500w.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btn500w.SetWindowRgn(hRgn,true);
	}
//
	// ��Ϣ�ƶ������ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\GoAhead_bt.bmp"), SKIN_FOLDER);
	m_btnleft.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnleft.SetWindowRgn(hRgn,true);
	}

	//��Ϣ�ƶ��Ҽ���ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\GoNext_bt.bmp"), SKIN_FOLDER);
	m_btnright.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnright.SetWindowRgn(hRgn,true);
	}

	// �鿴·�Ӱ�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\chakan_bt.bmp"), SKIN_FOLDER);
	m_btnluzi.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnluzi.SetWindowRgn(hRgn,true);
	}

	//������ׯ��ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\shenqing_bt.bmp"), SKIN_FOLDER);
	m_btnshangz.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnshangz.SetWindowRgn(hRgn,true);
	}
	
	///���а�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\bank_bt.bmp"), SKIN_FOLDER);
	m_btnbank.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnbank.SetWindowRgn(hRgn,true);
	}

	//�Ϸ���ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\top_bt.bmp"), SKIN_FOLDER);
	m_btTop.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btTop.SetWindowRgn(hRgn,true);
	}

	
	//�·���ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\bottom_bt.bmp"), SKIN_FOLDER);
	m_btBottom.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btBottom.SetWindowRgn(hRgn,true);
	}

	////OutputDebugString("CClientPlayView::OnCreate 3");
	/// �����ͻ������λ�������ע�İ�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\hide_bt.bmp"), SKIN_FOLDER);
	m_btnHideAIMoney.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnHideAIMoney.SetWindowRgn(hRgn,true);
	}
	m_btnHideAIMoney.SetWindowTextA("���ػ�����ע");

	/// �����ͻ�����ʾ�����˿�����Ϣ�İ�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\hide_bt.bmp"), SKIN_FOLDER);
	m_btnShowAIInfo.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnShowAIInfo.SetWindowRgn(hRgn,true);
	}
	m_btnShowAIInfo.SetWindowTextA("��������Ϣ");


	///�����ͻ��˿��ƿ�ׯ�İ�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\zhuang_bt.bmp"), SKIN_FOLDER);
	m_btnCtrZhuang.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnCtrZhuang.SetWindowRgn(hRgn,true);
	}

	///�����ͻ��˿��ƿ��еİ�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\xian_bt.bmp"), SKIN_FOLDER);
	m_btnCtrXian.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnCtrXian.SetWindowRgn(hRgn,true);
	}
	///�����ͻ��˿��ƿ��͵İ�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\he_bt.bmp"), SKIN_FOLDER);
	m_btnCtrHe.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnCtrHe.SetWindowRgn(hRgn,true);
	}

	///�����ͻ��˻ָ����Ƶİ�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\normal_bt.bmp"), SKIN_FOLDER);
	m_btnCtrNormal.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnCtrNormal.SetWindowRgn(hRgn,true);
	}

	///��ʾ�����Ի���İ�ť
	_stprintf_s(szPath, sizeof(szPath), _T("image\\superdlg_bt.bmp"), SKIN_FOLDER);
	m_btnCtrSuper.LoadButtonBitmap(szPath,false);
	imgTemp.SetLoadInfo(szPath,true);
	hRgn=AFCBmpToRgn(imgTemp,RGB(255,0,255),RGB(1,1,1));
	if(NULL != hRgn)
	{
		m_btnCtrSuper.SetWindowRgn(hRgn,true);
	}

	m_SuperDlg.Create(IDD_SUPERGM,this);
	m_SuperDlg.ShowWindow(SW_HIDE);	
	m_SuperDlg.ShowWindow(false);

	m_LuziDlg.Create(IDD_LUZI,this);
	m_LuziDlg.ShowWindow(SW_HIDE);	
	m_LuziDlg.ShowWindow(false);

	///��ʾ��������Ϣ�Ի���
	m_ShowAIInfo.Create(IDD_SHOW_AI_INFO,this);
	m_ShowAIInfo.ShowWindow(SW_HIDE);	
	m_ShowAIInfo.ShowWindow(false);

	return 0;
}

///�滭���溯��
void CClientPlayView::DrawViewFace(CDC * pDC, int iWidth, int iHeight)
{
	if (NULL == pDC)
	{
		return ;
	}
	
	m_pDC = pDC;
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	m_showcoredlg.ShowWindow(false);//���ػ��ְ�
	/// �Ȼ�����
	DrawBackground(pDC, iWidth, iHeight);	
	CFont font, *pOldFont;
	font.CreateFont(12,0,0,0,0,0,0,0,134,3,2,1,2,TEXT("����")); 
	pOldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255,255,102));
	/// ��������ͷ��
	CRect rectName(0,0,0,0);
	int i=0;
	for(i=0; i<PLAY_COUNT; i++)  // ����Ϊ��ͼ
	{
		if (NULL != m_pUserItem[i])
		{
			UserInfoStruct * pUserInfo=&m_pUserItem[i]->GameUserInfo;
			if (NULL == pUserInfo)
			{
				continue ;
			}
		}
	}
	///lym0512���ƽ��
	DrawJinBi(pDC, iWidth, iHeight);

	DrawRect(pDC, iWidth, iHeight);
	///����ׯ��Ϣ��ׯ�б�
	DrawNtInfo(pDC, iWidth, iHeight);

	///��������Լ�����Ϣ����ң������ʣ��ܼƳɼ�
	DrawMeInfo(pDC, iWidth, iHeight);

	//������ʷ�����Ϣ�����10�ֵ�ׯ�е��������30��ׯ�кͿ���
	DrawResultInfo(pDC, iWidth, iHeight);

	///������ע��Ϣ���˸��������ע��ׯ�кͿ���ע���޶�
	DrawXiaZhuInfo(pDC, iWidth, iHeight);

	///���ƶ�̬��Ϣ����ׯ����ʾ������ע����ʾ��ע�������ƣ������ʾ������Ϣ��������ʱ��
	DrawActiveInfo(pDC, iWidth, iHeight);

	///������
	DrawCard(pDC, iWidth, iHeight);

	///���ƶ���
	DrawMoveCard(pDC, iWidth, iHeight);
	
	/// ��ͼ�仯��
	if (m_byMyStation !=255 && 0 != m_uTime[m_byMyStation])
	{
		DrawTimer(pDC, m_PtTime[m_byMyStation].x, m_PtTime[m_byMyStation].y, m_uTime[m_byMyStation]);
	}

	DrawCursor(pDC, iWidth, iHeight);//���ƹ��ͼƬ

	//����͸�������
	DrawResultFrame(pDC, m_pDitu.x-5, m_pDitu.y+42);

	//�ǳ����ͻ��˾ͻ��ƿ�����Ϣ
	if(m_bShowSuper)
	{
		CFont font1, *pFont2;
		font1.CreateFont(12,0,0,0,0,0,0,0,134,3,2,1,2,TEXT("����")); 
		pFont2 = pDC->SelectObject(&font1);
		CRect rect;
		rect.SetRect(iWidth/2-300+60,iHeight/2+218-60,iWidth/2-300+220,iHeight/2+218-45);
		pDC->DrawText(m_sSuperInfo,m_sSuperInfo.GetLength(),rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		pDC->SelectObject(pFont2);
		font1.DeleteObject();
	}
	Graphics *pGDIAdd = Graphics::FromHDC(pDC->GetSafeHdc());
	DrawFullNoteBubble(pGDIAdd, iWidth, iHeight);
	pGDIAdd->ReleaseHDC(pDC->GetSafeHdc());
	pDC->SelectObject(pOldFont);	
	font.DeleteObject();

	///��ʾ����ͼƬ
	DrawPromptOpenCard(pDC,iWidth,iHeight);

	///��ʾ��ׯͼƬ
	DrawPromptChangeNt(pDC,iWidth,iHeight);
}
void CClientPlayView::DrawFullNoteBubble(Graphics* pGDIAdd, int iWidth, int iHeight)
{
	if (false == m_bFullNoteDisplay)
	{
		return;
	}
	TCHAR m_sz[MAX_PATH];
	sprintf(m_sz, "image\\fullnote_bubble.png");
#ifndef NO_PICTURE_COMPRESSION
	WCHAR wPath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, m_sz, sizeof(m_sz), wPath, sizeof(wPath));
	Bitmap * pbmTimerType = new Bitmap(wPath);
#else
	CUnZipRes m_unZip;
	Bitmap * pbmTimerType = new Bitmap(m_unZip.LoadFileResFromZip(m_sz));
#endif

	RectF rFXY(REAL((iWidth-pbmTimerType->GetWidth())/2-20), REAL((iHeight-pbmTimerType->GetHeight())/2+170),pbmTimerType->GetWidth(),pbmTimerType->GetHeight());	
	pGDIAdd->DrawImage(pbmTimerType, rFXY);
	delete pbmTimerType;
	pbmTimerType = NULL;
	return;
}

///  ����gif
void CClientPlayView::SetGif(BYTE bDeskView,int iValue,int iTimer)
{
	if (NULL == m_pUserItem[bDeskView])
	{
		return ;
	}
	// ��ͼת��Ϊ����λ��
	UserInfoStruct * pUserInfo=&m_pUserItem[bDeskView]->GameUserInfo;
	if (NULL != pUserInfo)
	{
		bDeskView =  pUserInfo->bDeskStation;   // ��ԭ������ͼλ��ת��Ϊ�߼�λ��
	}
	__super::SetGif(bDeskView, iValue, iTimer);
}

bool CClientPlayView::SetUserTimer(BYTE bViewStation, UINT uTime)
{
	// ��ͼת��Ϊ����λ��
	if (NULL == m_pUserItem[bViewStation])
	{
		return false;
	}
	else
	{
		UserInfoStruct * pUserInfo=&m_pUserItem[bViewStation]->GameUserInfo;
		if (NULL != pUserInfo)
		{
			bViewStation =  pUserInfo->bDeskStation;   // ��ԭ������ͼλ��ת��Ϊ�߼�λ��
		}
	}
	return __super::SetUserTimer(bViewStation, uTime);
}

/// ���Ż��㵽��ͼλ��
BYTE CClientPlayView::GetViewSeat(BYTE byDeskStation)
{
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg, m_PlayView);
	if (NULL == pGameDlg)
	{
		return 0;
	}
	byDeskStation = byDeskStation%PLAY_COUNT;
	return pGameDlg->GetViewSeat(byDeskStation);
}


///���ƣ�DrawBackground
///������������Ϸ����
///@param pDC ��ͼdc  iWidth ���  iHeight �߶�
///@return  
void CClientPlayView::DrawBackground(CDC * pDC, int iWidth, int iHeight)
{
	if (NULL == pDC)
	{
		return ;
	}
	int x = 0;
	int y = 0;

	// ���п�ߵ�����»��Ʊ���
	CGameImageHelper imgHelper(&m_imgBackground);

	if ((imgHelper.GetWidth() > 0) && (imgHelper.GetHeight() > 0))
	{		
		for (x=0; x<iWidth; (x += imgHelper.GetWidth()))
		{
			for (y=0; y<iHeight; (y += imgHelper.GetHeight()))
			{
				imgHelper.BitBlt(pDC->GetSafeHdc(), x, y);
			}
		}
	}
	// ���ƽ��汳��ͼ
	CGameImageHelper imgCenterLogo2(&m_imgCenterLogo2);
	x = (iWidth - imgCenterLogo2.GetWidth())/2;
	y = (iHeight - imgCenterLogo2.GetHeight())/2;
	imgCenterLogo2.BitBlt(pDC->GetSafeHdc(), x, y);	



	// ��������ͼƬ
	CGameImageHelper imgCenterLogo(&m_imgCenterLogo);
	x = (iWidth - imgCenterLogo.GetWidth())/2;
	y = (iHeight - imgCenterLogo.GetHeight())/2;
	imgCenterLogo.BitBlt(pDC->GetSafeHdc(), x, y);	
}


///���ƣ�DrawAwardPoint
///���������ƽ���
///@param pDC ��ͼdc �� x λ�� �� y λ�� �� iNum ������
///@return  
void CClientPlayView::DrawAwardPoint(CDC * pDC, int x, int y,BYTE iNum)
{
	if (NULL == pDC)
	{
		return ;
	}
	CGameImageHelper	imgAwardPint(&m_imgAwardPint);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgAwardPint);
	for(int i=0; i<iNum; ++i)
	{
		::TransparentBlt(pDC->GetSafeHdc(),x+i*30,y,imgAwardPint.GetWidth(),imgAwardPint.GetHeight(),
			dc.GetSafeHdc(),0,0,imgAwardPint.GetWidth(),imgAwardPint.GetHeight(),dc.GetPixel(0,0));
	}
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

///���ƣ�DrawNt
///�������滭 NT ��־
///@param pDC ��ͼdc �� x λ�� �� y λ�� �� 
///@return  
void CClientPlayView::DrawNt(CDC * pDC, int x, int y)
{
	if (NULL == pDC)
	{
		return ;
	}

	CGameImageHelper	imgNt(&m_imgNT);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *OldBmp = (CBitmap*)dc.SelectObject(imgNt);
	::TransparentBlt(pDC->GetSafeHdc(),x,y,imgNt.GetWidth(),imgNt.GetHeight(),
		dc.GetSafeHdc(),0,0,imgNt.GetWidth(),imgNt.GetHeight(),dc.GetPixel(0,0));
	dc.SelectObject(OldBmp);
	dc.DeleteDC();
	return;
}

///���ƣ�DrawTrustee
///�����������йܱ�־
///@param pDC ��ͼdc �� x λ�� �� y λ�� �� 
///@return  
void CClientPlayView::DrawTrustee(CDC * pDC, int x, int y)
{
	if (NULL == pDC)
	{
		return ;
	}

	CGameImageHelper	imgTrustee(&m_imgTrusteeLogo);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgTrustee);
	::TransparentBlt(pDC->GetSafeHdc(),x,y,imgTrustee.GetWidth(),imgTrustee.GetHeight(),
		dc.GetSafeHdc(),0,0,imgTrustee.GetWidth(),imgTrustee.GetHeight(),dc.GetPixel(0,0));
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

///���Ƴ����Ⱥ�˳��
void CClientPlayView::DrawSequence(CDC * pDC, int x, int y,int iSequence)
{
	TCHAR szName[MAX_PATH] = {0};
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	_stprintf_s(szName, sizeof(szName), _T("image\\finish_out%d.bmp"),SKIN_FOLDER, iSequence);
	m_imgPicSequence.SetLoadInfo(szName,false);		// ������˳��ͼƬ
	CGameImageHelper	PicSequence(&m_imgPicSequence);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(PicSequence);
	::TransparentBlt(pDC->GetSafeHdc(), x, y,PicSequence.GetWidth(),PicSequence.GetHeight(),
		dc.GetSafeHdc(),0,0,PicSequence.GetWidth(),PicSequence.GetHeight(),dc.GetPixel(0,0));
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

/// ���Ʋ�����־
void CClientPlayView::DrawPass(CDC * pDC, int x, int y)
{
	if (NULL == pDC)
	{
		return ;
	}
	
	CGameImageHelper	imgPass(&m_imgPass);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgPass);
	::TransparentBlt(pDC->GetSafeHdc(),x,y,imgPass.GetWidth(),imgPass.GetHeight(),
		dc.GetSafeHdc(),0,0,imgPass.GetWidth(),imgPass.GetHeight(),dc.GetPixel(0,0));
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

/// ���÷���
void CClientPlayView::SetCurOrientation(BYTE byCurOrientationViewSeat)
{
	m_byCurOrientation = byCurOrientationViewSeat;
	UpdateViewFace(NULL);
}

/// ���Ʒ���
void CClientPlayView::DrawOrientation(CDC *pDC, int x, int y, int iView)
{
	if (NULL == pDC)
	{
		return ;
	}

	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CGameImageHelper img(&m_imgFangXiang);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(img);
	TransparentBlt(pDC->GetSafeHdc(),m_PtCurOrientation[iView].x,m_PtCurOrientation[iView].y,
			img.GetWidth()/8,img.GetHeight(),dc.GetSafeHdc(),iView*img.GetWidth()/8,0,
			img.GetWidth()/8,img.GetHeight(),RGB(255,0,255));
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

/// ���õȴ�״̬
void CClientPlayView::SetWaitingType(BYTE byType)
{
	// ������Ϸ��ʼ���뿪��ť
	m_btnStart.ShowWindow(SW_HIDE);
	m_btnLeave.ShowWindow(SW_HIDE);

	// ������Ϸ�ȴ�״̬
	m_byWaitingType=byType;
	UpdateViewFace(NULL);
}

///���ƣ�DrawWaitingImg
///���������Ƶȴ�״̬ͼƬ
///@param pDC ��ͼdc �� iWidth ���ڿ�� �� iHeight ���ڸ߶� �� 
///@return  
void CClientPlayView::DrawWaitingImg(CDC * pDC, int iWidth, int iHeight)
{
	if (NULL == pDC)
	{
		return ;
	}
	CGameImageHelper	imgWaiting(&m_imgWaiting);

	int x = (iWidth - imgWaiting.GetWidth())/2;
	int y = (iHeight - imgWaiting.GetHeight())/2;
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgWaiting);
	::TransparentBlt(pDC->GetSafeHdc(),x,y,imgWaiting.GetWidth(),imgWaiting.GetHeight(),
		dc.GetSafeHdc(),0,0,imgWaiting.GetWidth(),imgWaiting.GetHeight(),dc.GetPixel(0,0));
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

void CClientPlayView::SendCard(BYTE byViewSeat, int iTime)
{
	WORD wCardList[1]={0};	
	m_SendCardAnimal[byViewSeat].MoveAnimal(m_PtSendCardStart[byViewSeat], m_PtSendCardAnimalEnd[byViewSeat], (long)iTime);
	m_SendCardAnimal[byViewSeat].SetCard(wCardList, NULL, 1);
}

/// �����й�״̬
void CClientPlayView::SetTrustee(BYTE byViewSeat, bool bIsTrustee)
{
	m_bIsTrustee[byViewSeat] = bIsTrustee;

	CRect rect;
	rect.SetRect(m_PtTrustee[byViewSeat].x,m_PtTrustee[byViewSeat].y,
		m_PtTrustee[byViewSeat].x+m_imgTrusteeLogo.GetWidth(),
		m_PtTrustee[byViewSeat].y+m_imgTrusteeLogo.GetHeight());
	UpdateViewFace(rect);

	// ������Լ����͵��й���Ϣ,�����йܰ�ťͼƬ
	if ((PLAY_COUNT/2) == byViewSeat)
	{
		TCHAR szName[MAX_PATH]={0};
		if (bIsTrustee)
		{
			_stprintf_s(szName, sizeof(szName), _T("image\\Trustee_less.bmp"),SKIN_FOLDER);
		}
		else
		{
			_stprintf_s(szName, sizeof(szName), _T("image\\Trustee_bt.bmp"),SKIN_FOLDER);
		}
		//m_btnTrustee.LoadButtonBitmap(szName,false);
	}
}


void CClientPlayView::OnBtnClickedLeave(void)
{
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if (NULL != pGameDlg)
	{
		pGameDlg->SendMessage(WM_CLOSE,0,0);
	}
}

///100������Ϣ
void CClientPlayView::OnBtnClicked100(void)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䲻��Ӧ
		return;
	if(	m_byMyStation==m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;

	//OnShowCursor(FALSE);
	m_iChooseNote =1;//��ǰѡ��ĳ��� 0û�У�1��100��2��1000��3��1w��4��10w��5��100w,6��500w
	EnableXiaZhu(true);
	m_btn100.EnableWindow(false);	// 100���밴ť

}
///1000������Ϣ
void CClientPlayView::OnBtnClicked1000(void)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䲻��Ӧ
		return;
	if(	m_byMyStation==m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;
	
	//SetCursor(LoadCursor(NULL, IDC_CROSS)); //���ù�ꡢ
	//SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
	//OnShowCursor(FALSE);
	m_iChooseNote =2;//��ǰѡ��ĳ��� 0û�У�1��100��2��1000��3��1w��4��10w��5��100w,6��500w
	EnableXiaZhu(true);
	m_btn1000.EnableWindow(false);	// 1000���밴ť

}
///1�������Ϣ
void CClientPlayView::OnBtnClicked1w(void)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䲻��Ӧ
		return;
	if(	m_byMyStation==m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;

	//SetCursor(LoadCursor(NULL, IDC_CROSS)); //���ù��
	//SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
	//OnShowCursor(FALSE);
	m_iChooseNote =3;//��ǰѡ��ĳ��� 0û�У�1��100��2��1000��3��1w��4��10w��5��100w,6��500w
	EnableXiaZhu(true);
	m_btn1w.EnableWindow(false);		// 1����밴ť

}
///10�������Ϣ
void CClientPlayView::OnBtnClicked10w(void)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䲻��Ӧ
		return;
	if(	m_byMyStation==m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;

	//SetCursor(LoadCursor(NULL, IDC_CROSS)); //���ù��
	//SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
	//OnShowCursor(FALSE);
	m_iChooseNote =4;//��ǰѡ��ĳ��� 0û�У�1��100��2��1000��3��1w��4��10w��5��100w,6��500w
	EnableXiaZhu(true);
	m_btn10w.EnableWindow(false);	// 10����밴ť
}
///100�������Ϣ
void CClientPlayView::OnBtnClicked100w(void)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䲻��Ӧ
		return;
	if(	m_byMyStation==m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;
	
	//SetCursor(LoadCursor(NULL, IDC_CROSS)); //���ù��
	//SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
	//OnShowCursor(FALSE);
	m_iChooseNote =5;//��ǰѡ��ĳ��� 0û�У�1��100��2��1000��3��1w��4��10w��5��100w,6��500w
	EnableXiaZhu(true);
	m_btn100w.EnableWindow(false);	// 100����밴ť
}
///500�������Ϣ
void CClientPlayView::OnBtnClicked500w(void)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䲻��Ӧ
		return;
	if(	m_byMyStation == m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;

	//SetCursor(LoadCursor(NULL, IDC_CROSS)); //���ù��
	//SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
	//OnShowCursor(FALSE);
	m_iChooseNote =6;//��ǰѡ��ĳ��� 0û�У�1��100��2��1000��3��1w��4��10w��5��100w,6��500w
	EnableXiaZhu(true);
	m_btn500w.EnableWindow(false);	// 500����밴ť
}

///���ƣ�OnBtnClickedInfoLeft
///�����������ʷ��¼��Ϣ��ť����Ϣ��ǰ�ƶ���
///@param 
///@return  
void CClientPlayView::OnBtnClickedInfoLeft(void)
{//��Ϣ
	int count=0;
	for(int i=0;i<MAXCOUNT;i++)
	{
		if(m_iGameInfo[i][2]>0)
			count++;
	}
	if(m_iShowInfoIndex<(count-10))
	{
		m_iShowInfoIndex++;
		m_btnright.EnableWindow(true);
	}
	else
	{
		if((count-10)>0)
		{	
			m_iShowInfoIndex = count-10;
		}
		else
		{
			m_iShowInfoIndex = 0;
		}
		m_btnleft.EnableWindow(false);

	}
	UpdateViewFace(NULL);
}
///���ƣ�OnBtnClickedInfoRight
///�����������ʷ��¼��Ϣ�Ұ�ť����Ϣ��ǰ�ƶ���
///@param 
///@return  
void CClientPlayView::OnBtnClickedInfoRight(void)
{
	if(m_iShowInfoIndex>0)
	{
		m_iShowInfoIndex--;
		m_btnleft.EnableWindow(true);
	}
	else 
	{
		m_btnright.EnableWindow(false);
		m_iShowInfoIndex=0;
	}
	UpdateViewFace(NULL);
}
///���ƣ�OnBtnClickedluzi
///����������鿴·�Ӱ�ť������·�ӶԻ���
///@param 
///@return  
void CClientPlayView::OnBtnClickedluzi(void)
{
	//m_Info.ShowWindow(true);
	//if(m_Info.DoModal()==IDOK)
	//{
	//}
	//�����鿴·�ӶԻ���
	CRect rect;
	GetClientRect(&rect);
	//m_LuziDlg.MoveWindow(rect.Width()/2-m_LuziDlg.m_bk.GetWidth()/2,rect.Height()/2-m_LuziDlg.m_bk.GetHeight()/2,m_LuziDlg.m_bk.GetWidth(),m_LuziDlg.m_bk.GetHeight(),TRUE);
	UpdateLuziData();
	if(m_LuziDlg.IsWindowVisible())
	{
		m_LuziDlg.ShowWindow(false);
	}
	else 
	{
		m_LuziDlg.ShowWindow(true);
	}
}
void CClientPlayView::OnBtnClickedStart(void)
{
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg, m_PlayView);
	if (NULL != pGameDlg)
	{
		pGameDlg->SendMessage(WM_USER_GAME_BEGIN,0,0);
	}
}

///���ƣ�OnBtnClickedShangZhuang
///�����������ׯ��ť�¼�
///@param 
///@return  
void CClientPlayView::OnBtnClickedShangZhuang(void)
{
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if(!m_bCanShangZhuang)
	{
		if (m_iMessageCount>=3)
		{
			return ;			
		}
		else
		{
			m_iMessageCount++;
		}
		pGameDlg->SendMessage(WM_USER_SHANG_ZHUANG,0,10);
		return ;
	}
///lym2010-03-19�ܷ�����ׯ��ť
	SetTimer(IDR_CTL_ZHUANG_BTN,2000,NULL);
	m_bCanShangZhuang = false;

	if (NULL != pGameDlg)
	{
		TCHAR szName[MAX_PATH];
		if(!CheckMeZhuang())
		{
			pGameDlg->SendMessage(WM_USER_SHANG_ZHUANG,0,0);
			//��Ϊ��ׯͼƬ					
			SetApplyBanker(false);				
		}
		else
		{
			if(m_pUserItem[m_byMyStation]!=NULL)
			{
				pGameDlg->SendMessage(WM_USER_SHANG_ZHUANG,0,1);
				//��Ϊ��ׯͼƬ
				SetApplyBanker(true);
			}
		}
	}
}

///���ƣ�OnBtnClickedBank
///������������а�ť�¼�
///@param 
///@return  
void CClientPlayView::OnBtnClickedBank(void)
{
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if (NULL != pGameDlg)
	{
		pGameDlg->SendMessage(WM_USER_BANK,0,0);
	}
}

///���ƣ�OnBtnClickedHideMoney
///��������������ͻ������λ�������ע�İ�ť
///@param 
///@return  
void CClientPlayView::OnBtnClickedHideMoney(void)
{
	m_bHideAIMoney = m_bHideAIMoney;///�Ƿ����ػ�������ע
	UpdateViewFace(NULL);
}

///�����ͻ�����ʾ�����˿�����Ϣ�İ�ť
///@param 
///@return  
void CClientPlayView::OnBtnClickedShowAIInfo(void)
{
	if(m_ShowAIInfo.IsWindowVisible())
	{
		m_ShowAIInfo.ShowWindow(false);
	}
	else 
	{
		m_ShowAIInfo.ShowAIInfoWindow();

	}

	UpdateViewFace(NULL);
}

///���ƣ�OnBtnClickedCtrZhuang
///��������������ͻ��˿��ƿ�ׯ�İ�ť
///@param 
///@return  
void CClientPlayView::OnBtnClickedCtrZhuang(void)
{
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if(pGameDlg!=NULL)
	{
		pGameDlg->SendMessage(WM_SUPER_CTR,1,0);
	}

}

///���ƣ�OnBtnClickedCtrXian
///��������������ͻ��˿��ƿ��еİ�ť
///@param 
///@return  
void CClientPlayView::OnBtnClickedCtrXian(void)
{
	
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if(pGameDlg!=NULL)
	{
		pGameDlg->SendMessage(WM_SUPER_CTR,2,0);
	}
}

///���ƣ�OnBtnClickedCtrHe
///��������������ͻ��˿��ƿ��͵İ�ť
///@param 
///@return  
void CClientPlayView::OnBtnClickedCtrHe(void)
{
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if(pGameDlg!=NULL)
	{
		pGameDlg->SendMessage(WM_SUPER_CTR,3,0);
	}	
}

///���ƣ�OnBtnClickedCtrNormal
///��������������ͻ���ȡ�����Ƶİ�ť
///@param 
///@return  
void CClientPlayView::OnBtnClickedCtrNormal(void)
{
	CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if(pGameDlg!=NULL)
	{
		pGameDlg->SendMessage(WM_SUPER_CTR,0,0);
	}	
}

///���ƣ�CheckMeZhuang
///����������Լ��Ƿ���ׯ�һ���ׯ���б���
///@param 
///@return  
bool CClientPlayView::CheckMeZhuang(void)
{

	//if(m_iNowNTstation<0)
		//return false;
	if(m_byMyStation == m_iNowNTstation)
		return true;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if (m_iZhuangList[i]<0)
		{
			continue;
		}
		if(m_byMyStation == m_iZhuangList[i])//����ׯ���б�
			return true; 
	}
	return false;
}

///���ƣ�UpdateNowData
///���������µ�ǰ���Ƶ���Ϣ��Ϣ
///@param 
///@return  
void CClientPlayView::UpdateNowData(void)
{
	m_iGameCount++;
	m_iPlayCount++;
	
	/*CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg, m_PlayView);
	if (NULL != pGameDlg)
	{
		for(int i=0;i<MAXCOUNT;i++)
		{ 
			for(int j=0;j<4;j++)
			{
				m_iGameInfo[i][j] = pGameDlg->m_iGameInfo[i][j];
			}
		}
	}*/

	if(m_iGameInfo[MAXCOUNT-1][2]>0)
	{
		for(int i=1;i<MAXCOUNT;i++)
		{
			m_iMePlayData[i-1]=m_iMePlayData[i];
		}
	}

	for(int i=MAXCOUNT-1;i>=0;i--)
	{
		if(m_iGameInfo[i][2]>0)
		{
			m_iMePlayData[i] = 1;
			m_byMePlayData[i][0] = 1;
			m_byMePlayData[i][1] = 1;
			m_byMePlayData[i][2] = 1;
			if(m_iKaiPai == 1)
			{
				if(m_iMyZhu[0]>0)
				{
					m_iMePlayData[i] = 2;//��Ӯ
					m_byMePlayData[i][0] = 2;
				}
				else if(m_iMyZhu[1]>0 || m_iMyZhu[2]>0)//������������ע������
				{
					m_byMePlayData[i][0] = 3;
				}
				//��
				if(m_iMyZhu[3]>0 || m_iMyZhu[4]>0 || m_iMyZhu[5]>0)
				{
					m_byMePlayData[i][1] = 3;
				}
				//��
				if(m_iMyZhu[6]>0 || m_iMyZhu[7]>0)
				{
					m_byMePlayData[i][2] = 3;
				}
			}
			else if(m_iKaiPai == 2)
			{
				if(m_iMyZhu[0]>0 || m_iMyZhu[1]>0)
				{
					m_iMePlayData[i] = 2;//��Ӯ
					m_byMePlayData[i][0] = 2;
				}
				else if(m_iMyZhu[2]>0 )//������������ע������
				{
					m_byMePlayData[i][0] = 3;
				}

				//��
				if(m_iMyZhu[3]>0 || m_iMyZhu[4]>0 || m_iMyZhu[5]>0)
				{
					m_byMePlayData[i][1] = 3;
				}
				//��
				if(m_iMyZhu[6]>0 || m_iMyZhu[7]>0)
				{
					m_byMePlayData[i][2] = 3;
				}
			}
			else if(m_iKaiPai == 3)
			{
				if(m_iMyZhu[0]>0 || m_iMyZhu[2]>0)
				{
					m_iMePlayData[i] = 2;//��Ӯ
					m_byMePlayData[i][0] = 2;
				}
				else if(m_iMyZhu[1]>0 )//������������ע������
				{
					m_byMePlayData[i][0] = 3;
				}
				//��
				if(m_iMyZhu[3]>0 || m_iMyZhu[4]>0 || m_iMyZhu[5]>0)
				{
					m_byMePlayData[i][1] = 3;
				}
				//��
				if(m_iMyZhu[6]>0 || m_iMyZhu[7]>0)
				{
					m_byMePlayData[i][2] = 3;
				}

			}
			else if(m_iKaiPai == 4)
			{
				if(m_iMyZhu[3]>0)
				{
					m_iMePlayData[i] = 2;//��Ӯ
					m_byMePlayData[i][1] = 2;
				}
				else if(m_iMyZhu[4]>0 || m_iMyZhu[5]>0)//������������ע������
				{
					m_byMePlayData[i][1] = 3;//����
				}				
				//ׯ
				if(m_iMyZhu[0]>0 || m_iMyZhu[1]>0 || m_iMyZhu[2]>0)
				{
					m_byMePlayData[i][0] = 3;
				}
				//��
				if(m_iMyZhu[6]>0 || m_iMyZhu[7]>0)
				{
					m_byMePlayData[i][2] = 3;
				}

			}
			else if(m_iKaiPai == 5)
			{
				if(m_iMyZhu[3]>0 || m_iMyZhu[4]>0)
				{
					m_iMePlayData[i] = 2;//��Ӯ
					m_byMePlayData[i][1] = 2;
				}
				else if(m_iMyZhu[5]>0 )//������������ע������
				{
					m_byMePlayData[i][1] = 3;
				}	
				//ׯ
				if(m_iMyZhu[0]>0 || m_iMyZhu[1]>0 || m_iMyZhu[2]>0)
				{
					m_byMePlayData[i][0] = 3;
				}
				//��
				if(m_iMyZhu[6]>0 || m_iMyZhu[7]>0)
				{
					m_byMePlayData[i][2] = 3;
				}
			}
			else if(m_iKaiPai == 6)
			{
				if(m_iMyZhu[3]>0 || m_iMyZhu[5]>0)
				{
					m_iMePlayData[i] = 2;//��Ӯ
					m_byMePlayData[i][1] = 2;
				}
				else if(m_iMyZhu[4]>0 )//������������ע������
				{
					m_byMePlayData[i][1] = 3;
				}	
				//ׯ
				if(m_iMyZhu[0]>0 || m_iMyZhu[1]>0 || m_iMyZhu[2]>0)
				{
					m_byMePlayData[i][0] = 3;
				}
				//��
				if(m_iMyZhu[6]>0 || m_iMyZhu[7]>0)
				{
					m_byMePlayData[i][2] = 3;
				}
			}
			else if(m_iKaiPai == 7)//7��
			{
				if(m_iMyZhu[6]>0)
				{
					m_iMePlayData[i] = 2;//��Ӯ
					m_byMePlayData[i][2] = 2;
				}
				else if(m_iMyZhu[7]>0 )//������������ע������
				{
					m_byMePlayData[i][2] = 3;
				}	

				//ׯ
				if(m_iMyZhu[0]>0 || m_iMyZhu[1]>0 || m_iMyZhu[2]>0)
				{
					m_byMePlayData[i][0] = 3;
				}
				//��
				if(m_iMyZhu[3]>0 || m_iMyZhu[4]>0 || m_iMyZhu[5]>0)
				{
					m_byMePlayData[i][1] = 3;
				}
			}
			else if(m_iKaiPai == 8)//8ͬ���
			{
				if(m_iMyZhu[6]>0 || m_iMyZhu[7]>0)
				{
					m_iMePlayData[i] = 2;//��Ӯ
					m_byMePlayData[i][2] = 2;
				}
				//ׯ
				if(m_iMyZhu[0]>0 || m_iMyZhu[1]>0 || m_iMyZhu[2]>0)
				{
					m_byMePlayData[i][0] = 3;
				}
				//��
				if(m_iMyZhu[3]>0 || m_iMyZhu[4]>0 || m_iMyZhu[5]>0)
				{
					m_byMePlayData[i][1] = 3;
				}
			}
			/*CString str="";
			str.Format("---xqm��Ӯ���ݣ����� %d   : %d %d %d ",i,m_byMePlayData[i][0],m_byMePlayData[i][1],m_byMePlayData[i][2]);
			OutputDebugString(str);*/
			break;
		}
	}
	//m_iWinmoney += m_iMeFen;//�Լ��ĳɼ�

	//������ 
	m_iMePlayCount = 0;
	m_iMeWinCount = 0;
	bool bMePlayCountAdd=false;
	m_iMePlayCount=0;
	m_iMeWinCount=0;
	for(int i=0;i<MAXCOUNT;i++)
	{
		if(m_iMePlayData[i]>0)
		{
			m_iMePlayCount++;
		}
		if(m_iMePlayData[i]>=2)
		{
			m_iMeWinCount++;
		}
	}

	int count=0;
	for(int i=0;i<MAXCOUNT;i++)
	{
		if(m_iGameInfo[i][2]>0)
			count++;
	}
	if(m_iShowInfoIndex<=(count-10) && m_iShowInfoIndex>0)
	{
		m_btnright.EnableWindow(true);
	}
	if(m_iShowInfoIndex<(count-10) )
	{
		m_btnleft.EnableWindow(true);
	}

	UpdateLuziData();//���¿�·����Ϣ

	m_btnright.EnableWindow(false);
	m_iShowInfoIndex=0;

	UpdateViewFace(NULL);
}

///���ƣ�UpdateLuziData
///����������·����Ϣ
///@param 
///@return  
void CClientPlayView::UpdateLuziData(void)
{
	m_LuziDlg.Init();	
	m_LuziDlg.m_iPlayCount = MAXCOUNT;
	memset(m_LuziDlg.m_iWinType, 0, sizeof(m_LuziDlg.m_iWinType));

	memcpy(m_LuziDlg.m_iZPaiXing,m_iZPaiXing,sizeof(m_iZPaiXing)); //ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	memcpy(m_LuziDlg.m_iXPaiXing,m_iXPaiXing,sizeof(m_iXPaiXing));//�м�����
	for(int i=0; i<MAXCOUNT/*m_iPlayCount*/; i++)
	{
		m_LuziDlg.m_iWinType[i] = m_iHistoryPlayData[i];
		if (m_iHistoryPlayData[i] == 1)
		{
			m_LuziDlg.m_iZwin++;
		}
		else if (m_iHistoryPlayData[i] == 2)
		{
			m_LuziDlg.m_iXwin++;	//�м�Ӯ
		}
		else if (m_iHistoryPlayData[i] == 3)
		{
			m_LuziDlg.m_iHwin++;		//��Ӯ
		}

		
		m_LuziDlg.m_iKaiPaiType[i]=m_iHistoryDuiZi[i];
		//if(m_iXPaiXing[3]>0)
		//{
		//	m_LuziDlg.m_iKaiPaiType[i]=6;
		//	m_iXPaiXing[3]=0;
		//	MsgPrint("zhangjing::m_iXPaiXing=%d",m_iXPaiXing[3]);
		//}

	}

	m_LuziDlg.ChangeData();
	m_LuziDlg.m_iNowJuShu = m_iNowJuShu;	//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
	m_LuziDlg.Invalidate(true);
	
}

///���ƣ�OnTimer
///��������ʱ�������ƣ���˸���������
///@param nIDEvent ��ʱ�� id
///@return  
void CClientPlayView::OnTimer(UINT nIDEvent)
{		
	CString strLog;
	switch(nIDEvent)
	{
	case IDR_UPDATEXIAZHU:
		{
			KillTimer(IDR_UPDATEXIAZHU);
			UpdateViewFace(NULL);

			break;
		}
	case IDR_FAPAI:
		{
			m_iMoveTime--;  //����ʱ��
			if(	m_iMoveTime <= 0 )
			{
				m_iMoveType = -1;
				m_iMoveTime=0;
				KillTimer(IDR_FAPAI);
			}	
			//strLog.Format("֡�� %d ",m_iMoveTime);
			////OutputDebugString(strLog);
		}
		break;
	case IDR_XIANPAI:
		{

		}
		break;
	case IDR_ZHUANGPAI:
		{

		}
		UpdateViewFace(NULL);
		return;
		break;
	case IDR_CTL_ZHUANG_BTN:///������ׯ��ť�������
		{
		///lym2010-03-19�ܷ�����ׯ��ť			
			m_bCanShangZhuang = true;
			//m_iMessageCount = 0;
			KillTimer(IDR_CTL_ZHUANG_BTN);
		}
		break;
	default:
		break;
	}
	
	__super::OnTimer(nIDEvent);
}

///���ƣ�DrawMoveCard
///�����������ƶ���
///@param pDC ��ͼdc �� iWidth ���ڿ�ȡ� iHeight ���ڸ߶ȡ�
///@return  
void CClientPlayView::DrawMoveCard(CDC*pDC,int iWidth, int iHeight)
{
	if(m_iCardZhen<0)
		return ;
	if(!m_bCardMove)
		return;
	if(m_bZhuang)//ׯ�ķ��ƶ���
	{
		if(m_iCardZhen>0)
		{
			int x=0,y=0;
			int num = 0;
			x=m_pCardZEnd.x + m_iCardZhen*(m_pMoveStart.x - m_pCardZEnd.x)/m_iSendCardZhen;//���ƶ����Ŀ�ʼλ��
			y=m_pCardZEnd.y + m_iCardZhen*(m_pMoveStart.y - m_pCardZEnd.y)/m_iSendCardZhen;//���ƶ����Ŀ�ʼλ��
			if((m_iCardNum-1)>=0 && (m_iCardNum-1)<=2)
				num = m_iCardNum-1;
			DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][num],x,y);

		}
	}
	else //�мҵķ��ƶ���
	{
		if(m_iCardZhen>0)
		{
			int x=0,y=0;
			int num = 0;
			x=m_pCardXEnd.x+m_iCardZhen*(m_pMoveStart.x - m_pCardXEnd.x)/m_iSendCardZhen;//���ƶ����Ŀ�ʼλ��
			y=m_pCardXEnd.y+m_iCardZhen*(m_pMoveStart.y - m_pCardXEnd.y)/m_iSendCardZhen;//���ƶ����Ŀ�ʼλ��
			if((m_iCardNum-1)>=0 && (m_iCardNum-1)<=2)
				num = m_iCardNum-1;
			DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][num],x,y);
		}
	}

}

///���ƣ�StartMove
///�������������ƶ���
///@param zhuang�Ƿ�ׯ�� num�ڼ����� zhen ֡��
///@return 
void CClientPlayView::StartMove(bool zhuang,int num,int zhen)
{
	m_iNowCard[0][0] = CPokerGameLogic::CardToWord(1, E_CARD_SHAPE::ECS_SHAPE_DIAMONDS, E_CARD_NUMBER::ECN_NUM_A);
	m_iNowCard[0][1] = CPokerGameLogic::CardToWord(1, E_CARD_SHAPE::ECS_SHAPE_DIAMONDS, E_CARD_NUMBER::ECN_NUM_2);
	m_iNowCard[0][2] = CPokerGameLogic::CardToWord(1, E_CARD_SHAPE::ECS_SHAPE_DIAMONDS, E_CARD_NUMBER::ECN_NUM_3);
	m_iNowCard[1][0] = CPokerGameLogic::CardToWord(1, E_CARD_SHAPE::ECS_SHAPE_DIAMONDS, E_CARD_NUMBER::ECN_NUM_4);
	m_iNowCard[1][1] = CPokerGameLogic::CardToWord(1, E_CARD_SHAPE::ECS_SHAPE_DIAMONDS, E_CARD_NUMBER::ECN_NUM_5);
	m_iNowCard[1][2] = CPokerGameLogic::CardToWord(1, E_CARD_SHAPE::ECS_SHAPE_DIAMONDS, E_CARD_NUMBER::ECN_NUM_6);

	m_iCardZhen = 5;//����֡����
	m_iCardNum = num;

	if(zhuang)//����ׯ�ķ��ƶ���
	{	
		//AfxMessageBox("kaishi");
		m_bZhuang = true;
		CWnd::SetTimer(IDR_ZHUANGPAI,50,NULL);
		//m_bCardMove = true;
	}
	else //�����еķ��ƶ���
	{
		m_bZhuang = false;
		SetTimer(IDR_XIANPAI,50,NULL);
		//m_bCardMove = true;
	}
}

///���ƣ�DrawACard
///��������x,y����һ����Card
///@param pDC ��ͼdc �� iWidth ���ڿ�ȡ� iHeight ���ڸ߶ȡ�Card �����ݡ� x λ�� ��y λ��
///@return  
void CClientPlayView::DrawACard(CDC*pDC,int iWidth, int iHeight,WORD Card,int x,int y)
{
	// ��ͼƬxiazhu.bmp
	// ��ͨ��ͼƬxiazhu.bmp
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	int value = 0;
	int type = 0;
	value = CPokerGameLogic::GetCardNumber(Card)-1;//��ȡ�Ƶ�
	type = CPokerGameLogic::GetCardShape(Card)-1;//��ȡ�ƵĻ�ɫ
	if(value==13)//С��
	{
		CGameImageHelper	imgwang(&m_imgWang);
		CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgwang);
		::TransparentBlt(pDC->GetSafeHdc(),x,y-20,imgwang.GetWidth()/3,imgwang.GetHeight(),
			dc.GetSafeHdc(),imgwang.GetWidth()/3,0,imgwang.GetWidth()/3,imgwang.GetHeight(),RGB(255,0,255));
		dc.SelectObject(pOldBmp);

	}
	else if(value==14)//����
	{
		CGameImageHelper	imgwang(&m_imgWang);
		CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgwang);
		::TransparentBlt(pDC->GetSafeHdc(),x,y-20,imgwang.GetWidth()/3,imgwang.GetHeight(),
			dc.GetSafeHdc(),2*imgwang.GetWidth()/3,0,imgwang.GetWidth()/3,imgwang.GetHeight(),RGB(255,0,255));
		dc.SelectObject(pOldBmp);
	}
	else
	{
		CGameImageHelper	imgcard(&m_imgCard);
		CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgcard);
		::TransparentBlt(pDC->GetSafeHdc(),x,y-20,imgcard.GetWidth()/13,imgcard.GetHeight()/4,
			dc.GetSafeHdc(),value*imgcard.GetWidth()/13,type*imgcard.GetHeight()/4,imgcard.GetWidth()/13,imgcard.GetHeight()/4,RGB(255,0,255));
		dc.SelectObject(pOldBmp);
	}
	dc.DeleteDC();
}

///���ƣ�OnMouseMove
///�����������Ϣ(�Ƶ�)����עʱ���ж�������Ǹ���ע����������Ӧ��״̬
///@param nFlags ��Ϣ���� �� point ���λ�á�
///@return  
void CClientPlayView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䷵��
		return;

	if(	m_byMyStation==m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;

	m_iChooseRect = -1;
	if(ZRect.PtInRect(point))//ׯ��ע����
	{
		m_iChooseRect = 0;
	}
	else if(ZTRect.PtInRect(point))// ׯ������ע����
	{
		m_iChooseRect = 1;
	}
	else if(ZDRcet.PtInRect(point))// ׯ������ע����
	{
		m_iChooseRect = 2;
	}
	else if(XRect.PtInRect(point))//  ����ע����
	{
		m_iChooseRect = 3;
	}
	else if(XTRect.PtInRect(point))// ��������ע����
	{
		m_iChooseRect = 4;
	}
	else if(XDRcet.PtInRect(point))// �ж�����ע����
	{
		m_iChooseRect = 5;
	}
	else if(HRect.PtInRect(point))//  ����ע����
	{
		m_iChooseRect = 6;
	}
	else if(THRect.PtInRect(point))//  ͬ�����ע����
	{
		m_iChooseRect = 7;
	}

	if(m_iChooseNote>0)
	{

		if(	NoMouseRect.PtInRect(point)||NoMouseRect1.PtInRect(point)||NoMouseRect2.PtInRect(point)||NoMouseRect3.PtInRect(point))///<����ֹ��������
		{
			m_pCursor.x = -100;
			m_pCursor.y = -100;
		}	
		else
		{
			//SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND1))); //���ù��
			GetCursorPos(&m_pCursor);        //�������λ��);
			ScreenToClient(&m_pCursor);
		}
		UpdateViewFace(NULL);
	}
	else
	{
		ShowCursor(TRUE);
	}
	UpdateViewFace(NULL);
}

///���ƣ�OnSetCursor
///������������꺯����Ϣ������ڱ�Ŀؼ���ʱ��ʾ���ñ�־
///@param 
///@return  
BOOL CClientPlayView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{

	if (m_bNowGameState == GS_SEND_CARD && m_byMyStation != m_iNowNTstation && m_iChooseNote > 0)
	{
		
		if (m_iChooseRect != -1)
		{
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649))); //���ù��
			return TRUE;
		}
		else
		{
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32648))); //���ù��
			return TRUE;
		}
	}

	return CUsingSkinClass::OnSetCursor(pWnd, nHitTest, message);
}

///���ƣ�OnMouseMove
///������������̧����Ϣ����עʱ������ע
///@param nFlags ��Ϣ���� �� point ���λ�á�
///@return  
void CClientPlayView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䷵��
		return;
	if(	m_byMyStation == m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;
	m_iChooseRect = -1;
	if(ZRect.PtInRect(point))//ׯ��ע����
	{
		m_iChooseRect = 0;
	}
	else if(ZTRect.PtInRect(point))// ׯ������ע����
	{
		m_iChooseRect = 1;
	}
	else if(ZDRcet.PtInRect(point))// ׯ������ע����
	{
		m_iChooseRect = 2;
	}
	else if(XRect.PtInRect(point))//  ����ע����
	{
		m_iChooseRect = 3;
	}
	else if(XTRect.PtInRect(point))// ��������ע����
	{
		m_iChooseRect = 4;
	}
	else if(XDRcet.PtInRect(point))// �ж�����ע����
	{
		m_iChooseRect = 5;
	}
	else if(HRect.PtInRect(point))//  ����ע����
	{
		m_iChooseRect = 6;
	}
	else if(THRect.PtInRect(point))//  ͬ�����ע����
	{
		m_iChooseRect = 7;
	}
	//������ע��Ϣ
	if(m_iChooseRect>=0 && m_iChooseNote>0)
	{
		CClientGameDlg *pGameDlg=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
		if (NULL != pGameDlg)
		{
			pGameDlg->SendMessage(WM_USER_XIA_ZHU,m_iChooseNote,m_iChooseRect);
		}
	}

	if((m_iMaxZhu[m_iChooseRect]-GetMoneyFromType(m_iChooseNote))<GetMoneyFromType(m_iChooseNote))
	{//��ǰ����������ô���ע���ͻ�һ��С�ĳ���
		switch(m_iChooseNote)
		{
		case 2:
			{
				OnBtnClicked100();
			}
			break;
		case 3:
			{
				OnBtnClicked1000();
			}
			break;
		case 4:
			{
				OnBtnClicked1w();
			}
			break;
		case 5:
			{
				OnBtnClicked10w();
			}
			break;
		case 6:
			{
				OnBtnClicked100w();
			}
			break;
		}
	}

	UpdateViewFace(NULL);
}
///���ƣ�GetMoneyFromType
///������ͨ���������ͻ�ȡ������
///@param moneytype ��������
///@return �����Ӧ�Ľ��
__int64 CClientPlayView::GetMoneyFromType(__int64 moneytype)
{
	__int64 money = 0;
	__int64 nTempMoney=1; 
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nTempMoney*=10;
	}
	if(moneytype==1)
	{
		money = 100;
	}
	else if(moneytype==2)
	{
		money = 1000;
	}
	else if(moneytype==3)
	{
		money = 10000;
	}
	else if(moneytype==4)
	{
		money = 100000;
	}
	else if(moneytype==5)
	{
		money = 1000000;
	}
	else if(moneytype==6)
	{
///72��һ��
#ifdef SET72 
		money = 10000000;
#else
		money = 5000000;
#endif
	}
	money = money/nTempMoney;
	return money;
}
///���ƣ�OnRButtonDown
///�������Ҽ�����ȡ������ѡȡ����
///@param nFlags ��Ϣ���� �� point ���λ�á�
///@return  
void CClientPlayView::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnRestoreCursor();

	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䲻��Ӧ
		return;
	if(	m_byMyStation==m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;

	m_iChooseNote = 0;
	EnableXiaZhu(true);
	UpdateViewFace(NULL);

}

///���ƣ�OnHandleKaiPai
///����������ϵͳ������Ϣ
///@param pNetData ��Ϣ�� �� uDataSize �����ȡ�
///@return  
void CClientPlayView::OnHandleKaiPai(void * pNetData, UINT uDataSize)
{
	KAI_PAI *paidata = (KAI_PAI *)pNetData;
	//����������
	m_iNowCard[0][0] = paidata->pai[0][0];
	m_iNowCard[0][1] = paidata->pai[0][1];
	m_iNowCard[0][2] = paidata->pai[0][2];
	m_iNowCard[1][0] = paidata->pai[1][0];
	m_iNowCard[1][1] = paidata->pai[1][1];
	m_iNowCard[1][2] = paidata->pai[1][2];
	//m_CardCount
	m_CardCount[0]=paidata->m_CardCount[0];
	m_CardCount[1]=paidata->m_CardCount[1];

	m_iWinner = paidata->m_iWinner;				//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	//m_iKaiPai = paidata->m_iKaiPai;	

	for(int i=0;i<5;i++)//ׯ������
	{
		m_iZPaiXing[i] =  paidata->m_iZPaiXing[i];
		m_iXPaiXing[i] =  paidata->m_iXPaiXing[i];
	}
	//��ȡӮ������
	for(int i=0;i<8;i++)
	{
		m_iWinQuYu[i] = paidata->m_iWinQuYu[i];			//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	}

	m_bShowResult = false; //�Ƿ���ʾ������Ϣ
	m_bShowRect = false ;//����Ӯ�������˸

	for(int i=0;i<5;i++)
	{
		m_iZCardInfo[i] = paidata->m_iZPaiXing[i];	//ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
		m_iXCardInfo[i] = paidata->m_iXPaiXing[i];	//�м�����,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	}

	for (int i=0; i<MAXCOUNT; i++)
	{
		m_iHistoryPlayData[i] = paidata->m_iHistoryPlayData[i];
	}	

	// �ָ��������״̬
	OnRestoreCursor();
}


///���ƣ�OnHandleXiazhu
///������������ע��Ϣ
///@param pNetData ��Ϣ�� �� uDataSize �����ȡ�
///@return  
void CClientPlayView::OnHandleXiazhu(void * pNetData, UINT uDataSize)
{
	XIA_ZHU *pXiazhu = (XIA_ZHU *)pNetData;
	int count=0;//��Ҹ���

	for(int i=0;i<MAXJINBI-1;i++)
	{
		if(m_iJinBi[pXiazhu->type][i]<=0)
			break;
		count++;
	}
	if(count>=MAXJINBI-1)
	{
		for(int i=0;i<MAXJINBI-1;i++)
		{
			m_iJinBi[pXiazhu->type][i] = m_iJinBi[pXiazhu->type][i+1];
			m_pJinBi[pXiazhu->type][i] = m_pJinBi[pXiazhu->type][i+1];
		}
	}
	for(int i=0;i<8;i++)
	{
		m_iMaxZhu[i] = pXiazhu->m_iMaxZhu[i];     //����ÿ������������µ�ע��
	}

	m_iJinBi[pXiazhu->type][count] = pXiazhu->moneytype; //����ڰ˸������µĽ���������¼ǰ2000��  
	m_pJinBi[pXiazhu->type][count].SetPoint(pXiazhu->point.x,pXiazhu->point.y);  //����ڰ˸������µĽ��λ���������¼ǰ2000��

	for(int i=0;i<8;i++)
	{
		m_iQuYuZhu[i] = pXiazhu->m_iQuYuZhu[i];//����ע
		m_iAIMoney[i] = pXiazhu->m_iAIMoney[i];	///�����˵���ע
	}

	m_iMyZhu[8] = pXiazhu->m_iMeZhongZhu;
	if(pXiazhu->station == m_byMyStation)//��ע�������Լ�
	{
		m_iMyZhu[pXiazhu->type] += pXiazhu->money;	
		SetXiaZhuBtn();
	}

	m_SuperDlg.UpDataNoteRateInfo(m_iQuYuZhu[0]*2,m_iQuYuZhu[3]*2,m_iQuYuZhu[6]*9) ; 
	
	switch (pXiazhu->type)
	{
	case 0:	UpdateViewFace(ZRect);	break;	// ׯ
	case 1:	UpdateViewFace(ZTRect);	break;	// ׯ����
	case 2:	UpdateViewFace(ZDRcet);	break;	// ׯ����

	case 3:	UpdateViewFace(XRect);	break;	// ��
	case 4:	UpdateViewFace(XTRect);	break;	// ������
	case 5:	UpdateViewFace(XDRcet);	break;	// ׯ����

	case 6:	UpdateViewFace(HRect);	break;	// ��
	case 7:	UpdateViewFace(THRect);	break;	// ͬ���
	}
}

///���ƣ�DrawJinBi
///���������ƽ��ͼƬ
///@param pDC ����dc �� iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return  
void CClientPlayView::DrawJinBi(CDC*pDC, int iWidth, int iHeight)
{
	CGameImageHelper	imgchouma(&m_imgchouma);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgchouma);

	//����������ƽ��
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<MAXJINBI;j++)
		{
			if(m_iJinBi[i][j]<=0)
				break;
			//���ƽ��
			CPoint dp(0,0);
			dp.x = m_iYUcenter[i].x + m_pJinBi[i][j].x;     //ÿ����������ĵ�λ��  
			dp.y = m_iYUcenter[i].y + m_pJinBi[i][j].y;     //ÿ����������ĵ�λ��  

			::TransparentBlt(pDC->GetSafeHdc(), dp.x, dp.y, imgchouma.GetWidth()/6,imgchouma.GetHeight(),
			dc.GetSafeHdc(), (m_iJinBi[i][j]-1)*imgchouma.GetWidth()/6,0, imgchouma.GetWidth()/6,imgchouma.GetHeight(),RGB(255,0,255));
		}
	}
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

///���ƣ�DrawJinBi
///���������Ƹ������Σ���עʱ����Ƶ������򣬽���ʱ��˸��Ӯ������
///@param pDC ����dc �� iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return  
void CClientPlayView::DrawRect(CDC*pDC, int iWidth, int iHeight)
{
	CRect rect(0,0,0,0);
	if(m_bNowGameState==GS_SEND_CARD)
	{
		switch(m_iChooseRect)
		{	
		case 0:
			{
				rect = ZRect;
			}
			break;
		case 1:
			{
				rect= ZTRect;
			}
			break;
		case 2:
			{
				rect= ZDRcet;
			}
			break;
		case 3:
			{
				rect= XRect;
			}
			break;
		case 4:
			{
				rect= XTRect;
			}
			break;
		case 5:
			{
				rect= XDRcet;
			}
			break;
		case 6:
			{
				rect= HRect;
			}
			break;
		case 7:
			{
				rect= THRect;
			}
			break;
		default:
			break;
		}
		///lym2010-03-19����ѡ�������ڲ�����
		if(m_iChooseRect>=0 && m_iChooseRect<8)
		{
			pDC->Draw3dRect(&rect,RGB(255,255,0),RGB(255,255,0));
			rect.top+=1;
			rect.bottom-=1;
			rect.left+=1;
			rect.right-=1;
			pDC->Draw3dRect(&rect,RGB(255,255,0),RGB(255,255,0));
			rect.top+=1;
			rect.bottom-=1;
			rect.left+=1;
			rect.right-=1;
			pDC->Draw3dRect(&rect,RGB(255,255,0),RGB(255,255,0));
		}
	}

	if(m_bNowGameState==GS_PLAY_GAME && !m_bSendCard && m_bShowRect)
	{
		for(int i=0;i<8;i++)
		{
			if(m_iWinQuYu[i]<=1)
				continue;
			switch(i)
			{	
			case 0:
				{
					rect = ZRect;
				}
				break;
			case 1:
				{
					rect= ZTRect;
				}
				break;
			case 2:
				{
					rect= ZDRcet;
				}
				break;
			case 3:
				{
					rect= XRect;
				}
				break;
			case 4:
				{
					rect= XTRect;
				}
				break;
			case 5:
				{
					rect= XDRcet;
				}
				break;
			case 6:
				{
					rect= HRect;
				}
				break;
			case 7:
				{
					rect= THRect;
				}
				break;
			default:
				break;
			}
			pDC->Draw3dRect(&rect,RGB(255,255,0),RGB(255,255,0));
			rect.top+=1;
			rect.bottom-=1;
			rect.left+=1;
			rect.right-=1;
			pDC->Draw3dRect(&rect,RGB(255,255,0),RGB(255,255,0));
			rect.top+=1;
			rect.bottom-=1;
			rect.left+=1;
			rect.right-=1;
			pDC->Draw3dRect(&rect,RGB(255,255,0),RGB(255,255,0));
		}
	}
}
///���ƣ�DrawNtInfo
///��������Լ�����Ϣ����ң������ʣ��ܼƳɼ�DrawMeInfo
///������
///������ʷ�����Ϣ�����10�ֵ�ׯ�е��������30��ׯ�кͿ���DrawResultInfo
///������ע��Ϣ���˸��������ע��ׯ�кͿ���ע���޶�DrawXiaZhuInfo
///���ƶ�̬��Ϣ����ׯ����ʾ������ע����ʾ��ע�������ƣ������ʾ������Ϣ��������ʱ��DrawActiveInfo
///����ׯ��Ϣ��ׯ�б�DrawNtInfo
///@param pDC ����dc �� iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return  
void CClientPlayView::DrawNtInfo(CDC *pDC, int iWidth, int iHeight)
{

	pDC->SetTextColor(RGB(85,155,236));
	CString str="--";
	CString stra="";
	CString strRank ; 
	int temp=0;
	CRect  rectName(0,0,0,0);

	//ׯ����Ϣ���سƣ����о�������������ɼ�������Ӯ���ٽ�ң�m_iNowNTstation
	rectName.top= m_pNtInfo.y;//�س�
	rectName.bottom = rectName.top+15;
	rectName.left = m_pNtInfo.x ;
	rectName.right = rectName.left + 100;
	if(m_pUserItem[m_iNowNTstation]!=NULL)
	{
		str.Format("%s",m_pUserItem[m_iNowNTstation]->GameUserInfo.nickName);
	}
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	str="0";
	if(m_pUserItem[m_iNowNTstation]!=NULL)
	{
		str.Format("%d",m_iNTju);//����
	}
	rectName.top= m_pNtInfo.y;
	rectName.bottom = rectName.top+15;
	rectName.left = m_pNtInfo.x + 145;
	rectName.right = rectName.left + 80;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	__int64 ntmoney=0;
	if(m_pUserItem[m_iNowNTstation]!=NULL)
	{
		ntmoney = m_pUserItem[m_iNowNTstation]->GameUserInfo.i64Money;
	}
	str="";
	//NumToChar(ntmoney,&str);
	ChangeNumToCStringByPower(ntmoney ,true ,"," ,str) ;

	rectName.top= m_pNtInfo.y +29;
	rectName.bottom = rectName.top+15;
	rectName.left = m_pNtInfo.x ;
	rectName.right = rectName.left + 100;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	//lym1115�Ŵ�100��,��ʾ��100��
	str="0";
	if(m_pUserItem[m_iNowNTstation]!=NULL)
	{
		//NumToChar(m_iNTwin,&str);;//�ɼ�
		ChangeNumToCStringByPower(m_iNTwin ,true ,"," ,str) ;
	}
	rectName.top= m_pNtInfo.y +29;
	rectName.bottom = rectName.top+15;
	rectName.left = m_pNtInfo.x + 145;
	rectName.right = rectName.left + 80;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	//ׯ���б��سƣ����
	//m_iZhuangList//m_pNtList
	if (m_iZhuangList[PLAY_COUNT] <= 5)
	{
		m_bNowNtListIndex = 0;
	}
	for(int i=m_bNowNtListIndex,j=0;i<PLAY_COUNT&&j<5;i++)//ֻ��ʾ5��
	{
		if(m_iZhuangList[i]>=0)
		{
			if (NULL != m_pUserItem[m_iZhuangList[i]])
			{
				__int64 money=0;
				TCHAR szText[_MAX_PATH]={0};
				rectName.top= m_pNtList.y + (j++)*15-7;
				rectName.bottom = rectName.top+15;
				rectName.left = m_pNtList.x +10;
				rectName.right = rectName.left + 100;

				strRank.Format("%d" , i + 1) ; 
				pDC->DrawText(strRank,strRank.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

				rectName.OffsetRect(40 ,0) ; 

				str.Format("%s",m_pUserItem[m_iZhuangList[i]]->GameUserInfo.nickName);
				pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

				money=m_pUserItem[m_iZhuangList[i]]->GameUserInfo.i64Money;
				//lym1115�Ŵ�100��,��ʾ��100��
				str="";
				//NumToChar(money,&str);
				ChangeNumToCStringByPower(money ,true ,"," ,str) ;

				rectName.OffsetRect(85 , 0) ;
				
				pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
			}
		}
		else 
		{
			break;
		}
	}
	pDC->SetTextColor(RGB(255,255,102));
}
///��ׯ�����·���ť�¼�
void CClientPlayView::OnBtnClickedNTTop(void)
{
	m_bNowNtListIndex--;
	if (m_bNowNtListIndex < 0)
	{
		m_bNowNtListIndex = 0;
	}
	CRect rect;
	rect.SetRect(m_pNtList.x,m_pNtList.y,m_pNtList.x+253,m_pNtList.y+90);
	UpdateViewFace(NULL);
}
///��ׯ�Ѷ����·���ť�¼�
void CClientPlayView::OnBtnClickedNTBottom(void)
{


	if (m_iZhuangList[PLAY_COUNT] <= 5)
	{
		m_bNowNtListIndex = 0;
	}
	else if (m_iZhuangList[PLAY_COUNT] - m_bNowNtListIndex > 5)
	{
		m_bNowNtListIndex++;
	}
	CRect rect;
	rect.SetRect(m_pNtList.x,m_pNtList.y,m_pNtList.x+253,m_pNtList.y+90);
	UpdateViewFace(NULL);
	
}
///���ƣ�DrawXiaZhuInfo
///������������ע��Ϣ���˸��������ע��ׯ�кͿ���ע���޶�DrawXiaZhuInfo
///@param pDC ����dc �� iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return  
void CClientPlayView::DrawXiaZhuInfo(CDC*pDC, int iWidth, int iHeight)
{
	//����ע״̬
	//return;
	CRect  rectName(0,0,0,0);
	CString stra="";
	int temp=0;

	__int64 nTempMoney=1; 
	for(int i=0; i<m_nPowerOfGold; i++)
	{
		nTempMoney*=10;
	}

	pDC->SetTextColor(RGB(255,255,255));

	//ׯ����Ϣ��ׯ�кͿ���ע���޶�//����ı�	m_pXZXianZhi.SetPoint(85,17);		 //�·�������ʾ�ĳ�ʼλ��
	CString str="";
	temp=0;
	
	str="";
	//NumToChar(m_iMaxZhu[0],&str);//ׯ��ע�޶�
	ChangeNumToCStringByPower(m_iMaxZhu[0] ,true ,"," ,str) ;

	rectName.top= m_pXZXianZhi.y ;
	rectName.bottom = rectName.top+15;
	rectName.left = m_pXZXianZhi.x ;
	rectName.right = rectName.left + 150;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);


	//lym1115�Ŵ�100��,��ʾ��100��
	str="";
	//NumToChar(m_iMaxZhu[6],&str);//����ע�޶�	
	ChangeNumToCStringByPower(m_iMaxZhu[6] ,true ,"," ,str) ;

	rectName.top= m_pXZXianZhi.y +21;
	rectName.bottom = rectName.top+15;
	rectName.left = m_pXZXianZhi.x ;
	rectName.right = rectName.left + 150;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	//lym1115�Ŵ�100��,��ʾ��100��
	str="";
	//NumToChar(m_iMaxZhu[3],&str);//����ע�޶�
	ChangeNumToCStringByPower(m_iMaxZhu[3] ,true ,"," ,str) ;

	rectName.top= m_pXZXianZhi.y + 42;
	rectName.bottom = rectName.top+15;
	rectName.left = m_pXZXianZhi.x ;
	rectName.right = rectName.left + 150;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);


	if(m_iNowNTstation>=0 && m_iNowNTstation<PLAY_COUNT && m_byMyStation != m_iNowNTstation)
	{
		__int64 llMyTotleNote = 0 ; 

		for(int i = 0 ; i< 8 ; i++)
		{
			llMyTotleNote += m_iMyZhu[i] ; 
		}

		CString strInfo ; 
		strInfo.Format("��ǰ�Լ���ע�ܶ") ; 
		str = "" ;
		ChangeNumToCStringByPower(llMyTotleNote ,true ,"," ,str) ;
		str = strInfo + str ;  

		rectName.top= m_pXZXianZhi.y + 505;
		rectName.bottom = rectName.top + 15;
		rectName.left = m_pXZXianZhi.x -80;
		rectName.right = rectName.left + 250;
		pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	pDC->SetTextColor(RGB(255,255,102));

	//�˸��������ע//��ͼ
	for(int i=0;i<8;i++)
	{
		if(m_iQuYuZhu[i]>0)
		{
			//__int64 iTempmoney=nTempMoney*m_iQuYuZhu[i];

			__int64 iTempmoney = ChangeMoneyByPower(m_iQuYuZhu[i] ,true);

			if(m_bHideAIMoney && m_SuperDlg.IsWindowVisible())///���ػ�������ע
			{
				//iTempmoney=nTempMoney*(m_iQuYuZhu[i] - m_iAIMoney[i]);
				iTempmoney=ChangeMoneyByPower(m_iQuYuZhu[i] - m_iAIMoney[i] , true);
			}
			DrawXiaZhuNum(pDC,iTempmoney,iWidth,iHeight,m_pQYText[i].x,m_pQYText[i].y-5,false);
		}
	}
	//����Լ�����ע
	for(int i=0;i<8;i++)
	{
		if(m_iMyZhu[i]>0)
		{
			//int iTempmoney=nTempMoney*m_iMyZhu[i];
			int iTempmoney=ChangeMoneyByPower(m_iMyZhu[i] ,true);

			DrawXiaZhuNum(pDC,iTempmoney,iWidth,iHeight,m_pQYText[i].x,m_pQYText[i].y+25,true);
		}
	}
}

///���ƣ�DrawXiaZhuNum
///���������Ƹ����������ע���֣��ܵ���ע���Լ�����ע
///@param pDC ����dc ��num ��ע� iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�xλ�� ��int λ�á� my �Ƿ��Լ�����ע
///@return  
void CClientPlayView::DrawXiaZhuNum(CDC*pDC,int num, int iWidth, int iHeight,int x,int y,bool my)
{	
	if(my)//�����Լ�����ע
	{	
		CGameImageHelper	imgnum(&m_imgMyNum);
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgnum);
		int temp = num; 
		int value=0;//ÿһλ��ֵ
		int index=0;
		int width=15;//���16
		int len=0;//��Գ���
		//x+=30;
		CString str="";
		str.Format("%d",num);
		int strlen = str.GetLength();
		x+=strlen*8;

		for(int i=0;i<12;i++)
		{
			value = temp%10;
			temp/=10;
			if(index>=8)
			{
				len=16;
			}
			else if(index>=4)
			{
				len=8;
			}
			::TransparentBlt(pDC->GetSafeHdc(), x-len-index*width,y, imgnum.GetWidth()/10,imgnum.GetHeight(),
				dc.GetSafeHdc(), value*imgnum.GetWidth()/10,0, imgnum.GetWidth()/10,imgnum.GetHeight(),dc.GetPixel(0,0));
			if(temp<=0)
				break;
			index++;
		}
		dc.SelectObject(pOldBmp);
		dc.DeleteDC();
	}
	else
	{
		CGameImageHelper	imgnum(&m_imgChoumaNum);
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgnum);
		int temp = num; 

		int value=0;//ÿһλ��ֵ
		int index=0;
		int width=16;//���16
		int len=0;//��Գ���
		//x+=30;
		CString str="";
		str.Format("%d",num);
		int strlen = str.GetLength();
		x+=strlen*8;

		for(int i=0;i<12;i++)
		{
			value = temp%10;
			temp/=10;
			if(index>=8)
			{
				len=20;
			}
			else if(index>=4)
			{
				len=10;
			}
			::TransparentBlt(pDC->GetSafeHdc(), x-len-index*width,y, imgnum.GetWidth()/10,imgnum.GetHeight(),
				dc.GetSafeHdc(), value*imgnum.GetWidth()/10,0, imgnum.GetWidth()/10,imgnum.GetHeight(),dc.GetPixel(0,0));
			if(temp<=0)
				break;
			index++;
		}
		dc.SelectObject(pOldBmp);
		dc.DeleteDC();
	}

}

///���ƣ�DrawPointValue
///���������ƿ��Ƶ���ֵ
///@param pDC ����dc ��xλ�� ��int λ�á� my �Ƿ��Լ�����ע
///@return  
void CClientPlayView::DrawPointValue(CDC * pDC, CDC &dc, int ix, int iy)
{
	// �е���
	BYTE value = 0, temp = 0;
	for (BYTE i=0; i<m_ContinueShowCardCount[1]; i++)
	{
		temp = CPokerGameLogic::GetCardNumber(m_iNowCard[1][i]);
		value += (temp>=10 ? 0 : temp);
	}

	// ׯ����
	BYTE valueBanker = 0;
	for (BYTE i=0; i<m_ContinueShowCardCount[0]; i++)
	{
		temp = CPokerGameLogic::GetCardNumber(m_iNowCard[0][i]);
		valueBanker += (temp>=10 ? 0 : temp);
	}


	switch(m_iShowCard)
	{
	case 1:
		{
			temp =  CPokerGameLogic::GetCardNumber(m_iNowCard[1][0]);
			value=(temp>=10 ? 0 : temp);
		}
		break;
	case 2:
		{
			temp =  CPokerGameLogic::GetCardNumber(m_iNowCard[1][0]);
			value=(temp>=10 ? 0 : temp);

			temp = CPokerGameLogic::GetCardNumber(m_iNowCard[0][0]);
			valueBanker=(temp>=10 ? 0 : temp);
		}
		break;
	case 3:
		{
			value = m_iXPaiXing[0];
			temp = CPokerGameLogic::GetCardNumber(m_iNowCard[0][0]);
			valueBanker = (temp>=10 ? 0 : temp);
		}
		break;
	case 4:
		{
			value = m_iXPaiXing[0];
			valueBanker = m_iZPaiXing[0];
		}
		break;
	case 5://�м�3����
		{
			value = m_iXPaiXing[1];
			valueBanker = m_iZPaiXing[0];
		}
		break;
	case 6://�м�û�������ƣ�ׯ����
		{
			value = m_iXPaiXing[1];
			valueBanker = m_iZPaiXing[1];
		}
		break;
	case 7:
		{
			value = m_iXPaiXing[1];
			valueBanker = m_iZPaiXing[1];
		}
		break;
	}

	value>=10 ? value%=10 : value;
	valueBanker>=10 ? valueBanker%=10 : valueBanker;

	// �С�ׯ����
	for (BYTE i=0;i<2;i++)
	{
		if (1>m_iShowCard || (1==i && 2>m_iShowCard)) break; 
		if (1==i)
		{
			value = valueBanker;
			ix += 139;
		}

		if (0==i && 0>=m_ContinueShowCardCount[1]) continue;
		if (1==i && 0>=m_ContinueShowCardCount[0]) continue;

		if ((0==i && 6==m_iKaiPai && 8<=value && m_iShowCard>=3)  ||  (1==i && 3==m_iKaiPai && 8<=value && m_iShowCard>=4) )
		{
			// ������������
			CGameImageHelper ImgTwoKing(&m_imgTwoKing);
			CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(ImgTwoKing);
			::TransparentBlt(pDC->GetSafeHdc(), ix+32, iy+9, ImgTwoKing.GetWidth(), \
				ImgTwoKing.GetHeight(), dc.GetSafeHdc(), 0, 0, ImgTwoKing.GetWidth(), \
				ImgTwoKing.GetHeight(), RGB(255,0,255));
			dc.SelectObject(pOldBmp);
		}
		else if ((0==i && 6==m_iKaiPai&& m_iShowCard>=3)  ||  (1==i && 3==m_iKaiPai&& m_iShowCard>=4))
		{
			// �С�ׯ����
			CGameImageHelper ImgTwoKing(&m_imgTwoKing);
			CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(ImgTwoKing);
			::TransparentBlt(pDC->GetSafeHdc(), ix+32+20, iy+9, ImgTwoKing.GetWidth()/2, \
				ImgTwoKing.GetHeight(), dc.GetSafeHdc(), 40, 0, ImgTwoKing.GetWidth()/2, \
				ImgTwoKing.GetHeight(), RGB(255,0,255));
			dc.SelectObject(pOldBmp);
		}
		else if (((0==i && 5==m_iKaiPai&& m_iShowCard>=3) || (1==i && 2==m_iKaiPai&& m_iShowCard>=4))   &&   8<=value)
		{
			// �С�ׯ����
			CGameImageHelper ImgTwoKing(&m_imgTwoKing);
			CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(ImgTwoKing);
			::TransparentBlt(pDC->GetSafeHdc(), ix+32+20, iy+9, ImgTwoKing.GetWidth()/2, \
				ImgTwoKing.GetHeight(), dc.GetSafeHdc(), 0, 0, ImgTwoKing.GetWidth()/2, \
				ImgTwoKing.GetHeight(), RGB(255,0,255));
			dc.SelectObject(pOldBmp);
		}
		else
		{
			// ���Ƶ�ֵ
			CGameImageHelper ImgPointValue(&m_imgPointValue);
			CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(ImgPointValue);
			::TransparentBlt(pDC->GetSafeHdc(), ix+52, iy+9, ImgPointValue.GetWidth()/10, \
				ImgPointValue.GetHeight(), dc.GetSafeHdc(), value*ImgPointValue.GetWidth()/10, \
				0, ImgPointValue.GetWidth()/10, ImgPointValue.GetHeight(), RGB(255,0,255));
			dc.SelectObject(pOldBmp);
		}
	}
	dc.DeleteDC();
}
///���ƣ�DrawResultInfo
///������������ʷ�����Ϣ�����10�ֵ�ׯ�е��������30��ׯ�кͿ���
///@param pDC ����dc ��iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return 
void CClientPlayView::DrawResultInfo(CDC*pDC, int iWidth, int iHeight)
{	
	
	if(m_bNowGameState==GS_PLAY_GAME && m_iShowCard!=0)
	{
		///���Ʊ��ֵĽ����ͼ
		CGameImageHelper	ditu(&m_imgditu);	// ���ͼƬcursor.bmp m_imgCursor
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		CBitmap *OldBmp1 = dc.SelectObject(ditu);
		::TransparentBlt(pDC->GetSafeHdc(), m_pDitu.x+14, m_pDitu.y-20, ditu.GetWidth(), ditu.GetHeight(),	\
			dc.GetSafeHdc(), 0, 0, ditu.GetWidth(),ditu.GetHeight(), RGB(255,0,255));

		// ���Ʊ����Ƶ���������
		DrawPointValue(pDC, dc, m_pDitu.x+14,m_pDitu.y-20);
		dc.SelectObject(OldBmp1);
		dc.DeleteDC();
	}
	CRect  rectName(0,0,0,0);
	CString str="";
	int index=0;
	int CountNum = 0x00;
	int iEndNumber = 0x00;
	for(int i = 0x00;i < MAXCOUNT;i++)
	{
		if(m_iGameInfo[i][2] > 0 && m_iGameInfo[i][2] !=255)//������
		{
			CountNum ++;
		}
	}
	iEndNumber = CountNum;
	if(CountNum > 10)
	{
		CountNum = CountNum - 10;
	}
	else
	{
		CountNum = 0x00;
	}
	//���10�ֵ�ׯ�е���
	for(int i=0x00; i < 10; i++)
	{
		if(m_iGameInfo[CountNum][2] > 0 && m_iGameInfo[CountNum][2] !=255)//������
		{
			if(CountNum==iEndNumber - 0x01)
			{
				pDC->SetTextColor(RGB(128,169,209));
			}

			str.Format("%d",m_iGameInfo[CountNum][0]);	//ׯ����
			rectName.top = m_p10ResultInfo.y;
			rectName.bottom = rectName.top+18;
			rectName.left = m_p10ResultInfo.x + i*22;
			rectName.right = rectName.left + 18;
			pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

			str.Format("%d",m_iGameInfo[CountNum][1]);	//�е���
			rectName.top = m_p10ResultInfo.y + 22;
			rectName.bottom = rectName.top+18;
			pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
			CountNum++;
			pDC->SetTextColor(RGB(255,255,102));
		}
	}
	pDC->SetTextColor(RGB(255,255,102));
	//���30��ׯ�кͿ���(��ͼ�Ǻ�)
	CGameImageHelper	imgXing(&m_imgXing);

#ifdef SET72 
	//��ͼƬ
	CGameImageHelper	imgZhuang(&m_imgShowZ);
	CGameImageHelper	imgXian(&m_imgShowX);
	CGameImageHelper	imgHe(&m_imgShowH);

	//��ͼƬ
	CGameImageHelper	imgZhuang_x(&m_imgShowZ_x);
	CGameImageHelper	imgXian_x(&m_imgShowX_x);
	CGameImageHelper	imgHe_x(&m_imgShowH_x);


	///�����Ӵ��
	CGameImageHelper	imgZhuang_dot(&m_imgShowZ_dot);
	CGameImageHelper	imgXian_dot(&m_imgShowX_dot);
	CGameImageHelper	imgHe_dot(&m_imgShowH);

	//��ͼƬ
	CGameImageHelper	imgZhuang_x_dot(&m_imgShowZ_x_dot);
	CGameImageHelper	imgXian_x_dot(&m_imgShowX_x_dot);
	CGameImageHelper	imgHe_x_dot(&m_imgShowH_x_dot);


#endif

	index=0;
	int x = (iWidth - imgXing.GetWidth())/2;
	int y = (iHeight - imgXing.GetHeight())/2;
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *OldBmp = dc.SelectObject(imgXing);
	int allcount=0;
	for(int i=0;i<MAXCOUNT;i++)
	{
		if (m_iResultInfoArea[i] > 0)
			allcount++;									//·����Ϣ�ж��ٸ�
	}
	if(allcount > 0x0a)
	{
		allcount = allcount - 10 - m_iShowInfoIndex;
	}
	else
	{
		allcount = 0x00;
	}
	int show=0;	
	int i = allcount;
	for(int j = 0x00;j < 0x0a;j ++,i++)
	{
		if(m_iResultInfoArea[i] == 1)//������Ԫ����Ӯ����1ׯ��2�У�3��
		{
			x=m_pXing.x + (index)*42;			
			y=m_pXing.y-3;
#ifdef SET72 
			if (m_iMeXiaZhuInfo[i] == 1)
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgZhuang_dot.OpenGDIHandle(&m_imgShowZ_Win_dot);
				}
				else
				{
					imgZhuang.OpenGDIHandle(&m_imgShowZ_Win);
				}				
			}
			else if (m_iMeXiaZhuInfo[i] == 2)
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgZhuang_dot.OpenGDIHandle(&m_imgShowZ_Lost_x_dot);
				}
				else
				{
				imgZhuang.OpenGDIHandle(&m_imgShowZ_Lost_x);
				}
			}
			else
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgZhuang_dot.OpenGDIHandle(&m_imgShowZ_dot);
				}
				else
				{
				imgZhuang.OpenGDIHandle(&m_imgShowZ);
				}
			}
			if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
			{
				x+=(42-imgXian_dot.GetWidth())/2;
				dc.SelectObject(imgZhuang_dot);
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgZhuang_dot.GetWidth(),imgZhuang_dot.GetHeight(),
					dc.GetSafeHdc(),0,0,imgZhuang_dot.GetWidth(),imgZhuang_dot.GetHeight(),RGB(255,0,255));
			}
			else
			{
				x+=(42-imgXian.GetWidth())/2;
				dc.SelectObject(imgZhuang);
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgZhuang.GetWidth(),imgZhuang.GetHeight(),
					dc.GetSafeHdc(),0,0,imgZhuang.GetWidth(),imgZhuang.GetHeight(),RGB(255,0,255));
			}



#else
			//����Լ�Ӯ
			if(m_iMeXiaZhuInfo[i]==2)
			{
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgXing.GetWidth()/2,imgXing.GetHeight(),
					dc.GetSafeHdc(),imgXing.GetWidth()/2,0,imgXing.GetWidth()/2,imgXing.GetHeight(),dc.GetPixel(0,0));
			}
			else
			{
				//����
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgXing.GetWidth()/2,imgXing.GetHeight(),
					dc.GetSafeHdc(),0,0,imgXing.GetWidth()/2,imgXing.GetHeight(),dc.GetPixel(0,0));
			}
#endif
		}
		else if(m_iResultInfoArea[i] == 2)//��
		{
			x=m_pXing.x + (index)*42;			
			y=m_pXing.y+30-2;

#ifdef SET72 
			
			if (m_iMeXiaZhuInfo[i] == 1)
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgXian_dot.OpenGDIHandle(&m_imgShowX_Win_dot);
				}
				else
				{
				imgXian.OpenGDIHandle(&m_imgShowX_Win);
				}
			}
			else if (m_iMeXiaZhuInfo[i] == 2)
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgXian_dot.OpenGDIHandle(&m_imgShowX_Lost_x_dot);
				}
				else
				{
				imgXian.OpenGDIHandle(&m_imgShowX_Lost_x);
				}
			}
			else
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgXian_dot.OpenGDIHandle(&m_imgShowX_dot);
				}
				else
				{
				imgXian.OpenGDIHandle(&m_imgShowX);
				}
			}
			if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
			{
				x+=(42-imgXian_dot.GetWidth())/2;
				dc.SelectObject(imgXian_dot);
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgXian_dot.GetWidth(),imgXian_dot.GetHeight(),
					dc.GetSafeHdc(),0,0,imgXian_dot.GetWidth(),imgXian_dot.GetHeight(),RGB(255,0,255));
			}
			else
			{
			x+=(42-imgXian.GetWidth())/2;
			dc.SelectObject(imgXian);
			::TransparentBlt(pDC->GetSafeHdc(),x,y,imgXian.GetWidth(),imgXian.GetHeight(),
				dc.GetSafeHdc(),0,0,imgXian.GetWidth(),imgXian.GetHeight(),RGB(255,0,255));
			}

#else
			//����Լ�Ӯ
			if(m_iMeXiaZhuInfo[i]==2)
			{
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgXing.GetWidth()/2,imgXing.GetHeight(),
					dc.GetSafeHdc(),imgXing.GetWidth()/2,0,imgXing.GetWidth()/2,imgXing.GetHeight(),dc.GetPixel(0,0));
			}
			else
			{
			//����
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgXing.GetWidth()/2,imgXing.GetHeight(),
					dc.GetSafeHdc(),0,0,imgXing.GetWidth()/2,imgXing.GetHeight(),dc.GetPixel(0,0));
			}
#endif
		}
		else if(m_iResultInfoArea[i] == 3)//��
		{
			x=m_pXing.x + (index)*42;			
			y=m_pXing.y + 60 - 2;
#ifdef SET72 	

			if (m_iMeXiaZhuInfo[i] == 1)
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgHe_dot.OpenGDIHandle(&m_imgShowH_Win_dot);
				}
				else
				{
				imgHe.OpenGDIHandle(&m_imgShowH_Win);
				}
			}
			else if (m_iMeXiaZhuInfo[i] == 2)
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgHe_dot.OpenGDIHandle(&m_imgShowH_Lost_x_dot);
				}
				else
				{
				imgHe.OpenGDIHandle(&m_imgShowH_Lost_x);
				}
			}
			else
			{
				if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
				{
					imgHe_dot.OpenGDIHandle(&m_imgShowH_dot);
				}
				else
				{
				imgHe.OpenGDIHandle(&m_imgShowH);
				}
			}
			if(m_iHistoryDuiZi[i]==3||m_iHistoryDuiZi[i]==6)
			{
				x+=(42-imgXian_dot.GetWidth())/2;
				dc.SelectObject(imgHe_dot);
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgHe_dot.GetWidth(),imgHe_dot.GetHeight(),
					dc.GetSafeHdc(),0,0,imgHe_dot.GetWidth(),imgHe_dot.GetHeight(),RGB(255,0,255));
			}
			else
			{

			x+=(42-imgXian.GetWidth())/2;
			dc.SelectObject(imgHe);
			::TransparentBlt(pDC->GetSafeHdc(),x,y,imgHe.GetWidth(),imgHe.GetHeight(),
				dc.GetSafeHdc(),0,0,imgHe.GetWidth(),imgHe.GetHeight(),RGB(255,0,255));
			}

#else
			//����Լ�Ӯ
			if(m_iMeXiaZhuInfo[i]>=2)
			{
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgXing.GetWidth()/2,imgXing.GetHeight(),
					dc.GetSafeHdc(),imgXing.GetWidth()/2,0,imgXing.GetWidth()/2,imgXing.GetHeight(),dc.GetPixel(0,0));
			}
			else
			{
				//����
				::TransparentBlt(pDC->GetSafeHdc(),x,y,imgXing.GetWidth()/2,imgXing.GetHeight(),
					dc.GetSafeHdc(),0,0,imgXing.GetWidth()/2,imgXing.GetHeight(),dc.GetPixel(0,0));
			}
#endif
		}
		index++;
		if (index > 9)
		{
			break;
		}
	}

    dc.SelectObject(OldBmp);
	dc.DeleteDC();
	//���30�ֿ�ׯ�к͵Ĵ������ı����
	int count[3];
	count[0]=0;
	count[1]=0;
	count[2]=0;
	for(int i=0;i<MAXCOUNT;i++)
	{
		if(m_iResultInfoArea[i] == 1)//ׯ
		{
			count[0]++;
		}
		else if(m_iResultInfoArea[i] == 2)//��
		{
			count[1]++;
		}
		else if(m_iResultInfoArea[i] == 3)//��
		{
			count[2]++;
		}		
	}
	str.Format("%d",count[0]);//��ׯ����
	rectName.SetRect(m_pZXcount.x, m_pZXcount.y-3, m_pZXcount.x+48, m_pZXcount.y+32-3);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	str.Format("%d",count[1]);//���д���
	rectName.SetRect(m_pZXcount.x,m_pZXcount.y+32-4,m_pZXcount.x+48, m_pZXcount.y+64-4);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	str.Format("%d",count[2]);//���ʹ���
	rectName.SetRect(m_pZXcount.x,m_pZXcount.y+64-6,m_pZXcount.x+48, m_pZXcount.y+96-6);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

}

///���ƣ�DrawMeInfo
///��������������Լ�����Ϣ����ң������ʣ��ܼƳɼ�
///@param pDC ����dc ��iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return 
void CClientPlayView::DrawMeInfo(CDC*pDC, int iWidth, int iHeight)
{
	CRect rectName(0,0,0,0);
	pDC->SetTextColor(RGB(255,255,255));

	char Text[128];
	memset(&Text,0,sizeof(Text));
	//NumToChar(m_iNTmoney,Text);

	//����Լ�����Ϣ
	//�ǳ�
	//m_pUserItem[m_byMyStation]
	CString str="--";
	//m_PlayView.m_byMyStation
	if(m_pUserItem[m_byMyStation]!=NULL && m_byMyStation!=255)
	{
		str.Format("%s",m_pUserItem[m_byMyStation]->GameUserInfo.nickName);
	}
	rectName.top=m_pMeInfo.y;
	rectName.bottom = rectName.top+28;
	rectName.left = m_pMeInfo.x + 55;
	rectName.right = rectName.left + 130;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	//���
	__int64 mymoney=0;      //ע���滻��ֵ
	int temp=0;
	if(m_pUserItem[m_byMyStation]!=NULL)
	{
		if((!m_bShowResult && m_bNowGameState==GS_PLAY_GAME) || m_bNowGameState==GS_SEND_CARD)
		{
			mymoney = m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - GetMyAllNote();
		}
		else
		{
			mymoney = m_pUserItem[m_byMyStation]->GameUserInfo.i64Money;
		}
	}
	rectName.top=m_pMeInfo.y +24 ;
	rectName.bottom = rectName.top+22;
	rectName.left = m_pMeInfo.x+55;
	rectName.right = rectName.left + 130;
	str="";
	
	//NumToChar(mymoney,&str);
     ChangeNumToCStringByPower(mymoney ,true ,"," ,str) ; 
	 pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	//m_iWinmoney=0;			//��ǰ�����Ӯ�Ľ��
	//m_iMeWinCount=0;		//����Լ�Ӯ�ľ���
	//m_iMePlayCount=1;		//����Լ����еľ���


	if(m_iMePlayCount>0&&m_byMyStation!=m_iNowNTstation)
	{
		temp=0;
		temp = m_iMeWinCount*100/m_iMePlayCount;
		CString str1="",str2="";
		str1.Format("%d",temp);
		temp = m_iMeWinCount*100*100/m_iMePlayCount;
		temp %= 100; 
		if(temp<10)
		{
			str2.Format("0%d",temp);
		}
		else
		{
			str2.Format("%d",temp);
		}

		str=str1+ "."+str2 + "%";
		CString outstr;
		outstr.Format("zhtout:%s m_iMeWinCount=%d m_iMePlayCount=%d %s",str,m_iMeWinCount,m_iMePlayCount,m_pUserItem[m_byMyStation]->GameUserInfo.nickName);
		OutputDebugString(outstr);
	}
	else
	{
		str="0.00%";
	}
	rectName.top=m_pMeInfo.y +48 ;
	rectName.bottom = rectName.top+20;
	rectName.left = m_pMeInfo.x+55;
	rectName.right = rectName.left + 100;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);


	  // ���
	//�ϼƣ�����Ӯ��ң�
	str="";
	//NumToChar(m_iWinmoney,&str);
	 ChangeNumToCStringByPower(m_iWinmoney ,true ,"," ,str) ; 

	rectName.top=m_pMeInfo.y +68 ;
	rectName.bottom = rectName.top+20;
	rectName.left = m_pMeInfo.x+55;
	rectName.right = rectName.left + 100;
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	pDC->SetTextColor(RGB(255,255,102));
}


///���ƣ�DrawActiveInfo
///���������ƶ�̬��Ϣ����ׯ����ʾ������ע����ʾ��ע�������ƣ������ʾ������Ϣ��������ʱ��
///@param pDC ����dc ��iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return 
void CClientPlayView::DrawActiveInfo(CDC*pDC, int iWidth, int iHeight)
{
	CString str="";

	//��ׯ
	if(m_iNowNTstation<0)
	{//��ǰ��ׯ�����뼴����ׯ
		str="��ǰ��ׯ...";//m_pTextStation
		pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);
		str="���뼴����ׯ";//
		pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+16,str);
	}
	else if(m_bNowGameState==GS_SEND_CARD)////��ע
	{//z��Ϸ��ʼ�������·�
		if(m_byMyStation == m_iNowNTstation)
		{
			str="��Ϸ��ʼ�ˣ������ȴ��м���ע...";//m_pTextStation
		}
		else
		{
			str="��Ϸ��ʼ�ˣ�������ע...";//m_pTextStation
		}
		pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);
	}
	else if(m_bNowGameState==GS_PLAY_GAME)//����
	{
		if(m_iShowCard==0)//���ݿ��Ƹ���
		{
			str="��ʼ����...";//m_pTextStation
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);
		}
		else if(m_iShowCard==1)//��1��
		{
			str="��ʼ����...";//m_pTextStation
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);
		}
		else if(m_iShowCard==2)//ׯ�и�1��
		{
			str="��ʼ����...";//m_pTextStation
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);

		}
		else if(m_iShowCard==3)//�����ţ�ׯ1��
		{
			str="��ʼ����...";//m_pTextStation
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);
		}
		else if(m_iShowCard==4)//ׯ��������
		{
			int value1 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][0]);//��ȡ�Ƶ�
			int value2 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][1]);//��ȡ�Ƶ�

			if(value1>=10) {value1=0;}
			if(value2>=10) {value2=0;}
			value1 += value2;
			if(value1>=10) {value1%=10;}

			value1 = m_iXPaiXing[0];//ֱ��ȡ��ǰ�����Ƶ�lym2010-03-18

			if(value1>=8)
			{
				str.Format("%s������%d ����  ˫��ͣ��",m_XiangStr,value1);
				if (m_iGamePictureType)
				{
					str.Format("�ص�����%d ����  ˫��ͣ��",value1);
				}
			}
			else if(value1>=6)
			{
				str.Format("%s������%d  ͣ��",m_XiangStr,value1);
				if (m_iGamePictureType)
				{
					str.Format("�ص�����%d  ͣ��",value1);
				}
			}
			else 
			{
				str.Format("%s������%d ",m_XiangStr,value1);
				if (m_iGamePictureType)
				{
					str.Format("�ص�����%d ",value1);
				}				
			}
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);

			int value4 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][0]);//��ȡ�Ƶ�
			int value5 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][1]);//��ȡ�Ƶ�
			if(value4>=10) {value4=0;}
			if(value5>=10) {value5=0;}
			value4 += value5;
			if(value4>=10) {value4 %= 10;}

			value4 = m_iZPaiXing[0];//ֱ��ȡׯǰ�����Ƶ�lym2010-03-18

			if(value4>=8)
			{
				str.Format("%s������%d ����  ˫��ͣ��",m_ZhuangStr,value4);
				if (m_iGamePictureType)
				{
					str.Format("�������%d ����  ˫��ͣ��",value4);
				}
			}
			else if(value4>=6 && value1>=6)
			{
				str.Format("%s������%d  ͣ��",m_ZhuangStr,value4);
				if (m_iGamePictureType)
				{
					str.Format("�������%d  ͣ��",value4);
				}
			}
			else 
			{
				str.Format("%s������%d ",m_ZhuangStr,value4);
				if (m_iGamePictureType)
				{
					str.Format("�������%d ",value4);
				}
			}
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+16,str);
			str="";
			if(value1>=6 && value1<=7 && value4<=5)
			{
				str.Format("%s����",m_ZhuangStr);
				if (m_iGamePictureType)
				{
					str.Format("������");
				}
			}
			if(m_CardCount[0]==2 && m_CardCount[1]==2)//����������
			{
				str="";
				if(m_iWinner==1)//ׯӮ
				{
					str.Format("%sӮ",m_ZhuangStr);
					if (m_iGamePictureType)
					{
						str.Format("��Ӯ");
					}
				}
				else if(m_iWinner==2)//��Ӯ
				{
					str.Format("%sӮ",m_XiangStr);
					if (m_iGamePictureType)
					{
						str.Format("��Ӯ");
					}
				}
				else if(m_iWinner==3)
				{
					str.Format("��Ӯ");
				}

			}
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+32,str);
		}
		else if(m_iShowCard==5)//��3���ƣ�
		{
			int value1 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][0]);//��ȡ�Ƶ�
			int value2 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][1]);//��ȡ�Ƶ�
			int value3 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][2]);//��ȡ�Ƶ�
			if(value1>=10) {value1=0;}
			if(value2>=10) {value2=0;}
			if(value3>=10) {value3=0;}
			value1 += (value2+value3);
			if(value1>=10) 
			{
				value1%=10;
			}
			value1 = m_iXPaiXing[1];//ֱ��ȡ��ǰ�����Ƶ�lym2010-03-18
			if (value1>=8)
			{
				str.Format("%s������%d ���� ",m_XiangStr,value1);
				if (m_iGamePictureType)
				{
					str.Format("�ص�����%d ���� ",value1);
				}
			}
			else
			{
				str.Format("%s������%d ",m_XiangStr,value1);
				if (m_iGamePictureType)
				{
					str.Format("�ص�����%d ",value1);
				}
			}	
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);
			int value4 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][0]);//��ȡ�Ƶ�
			int value5 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][1]);//��ȡ�Ƶ�
			if(value4>=10) {value4=0;}
			if(value5>=10) {value5=0;}
			value4 += value5;
			if(value4>=10) {value4 %= 10;}

			value4 = m_iZPaiXing[1];//ֱ��ȡׯǰ�����Ƶ�lym2010-03-18

			str.Format("%s������%d ",m_ZhuangStr,value4);
			if (m_iGamePictureType)
			{
				str.Format("�������%d ",value4);
			}
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+16,str);
			if(m_bSendCard)//����û�н���,˵��ׯ�һ�������
			{
				str.Format("%s�������Ƶ㣺%d  %s����",m_XiangStr,value3,m_XiangStr);
				if (m_iGamePictureType)
				{
					str.Format("�ص������Ƶ㣺%d  ������",value3);
				}
				pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+32,str);
			}
			else 
			{
				str="";
				if(m_iWinner==1)//ׯӮ
				{
					str.Format("%sӮ",m_ZhuangStr);
					if (m_iGamePictureType)
					{
						str.Format("��Ӯ");
					}
				}
				else if(m_iWinner==2)//��Ӯ
				{
					str.Format("%sӮ",m_XiangStr);
					if (m_iGamePictureType)
					{
						str.Format("��Ӯ");
					}
				}
				else if(m_iWinner==3)
				{
					str.Format("��Ӯ");
				}
				pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+32,str);
			}
		}
		else if(m_iShowCard==6)//��2���ƣ�ׯ3����
		{
			int value1 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][0]);//��ȡ�Ƶ�
			int value2 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][1]);//��ȡ�Ƶ�
			if(value1>=10) {value1=0;}
			if(value2>=10) {value2=0;}
			value1 += value2;
			value1 = m_iXPaiXing[1];//ֱ��ȡ��ǰ�����Ƶ�lym2010-03-18

			if(value1>=10) {value1%=10;}
			str.Format("%s������%d ",m_XiangStr,value1);
			if (m_iGamePictureType)
			{	
				str.Format("�ص�����%d ",value1);
			}
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);

			int value4 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][0]);//��ȡ�Ƶ�
			int value5 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][1]);//��ȡ�Ƶ�
			int value6 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][2]);//��ȡ�Ƶ�
			if(value4>=10) {value4=0;}
			if(value5>=10) {value5=0;}
			if(value6>=10) {value6=0;}
			value4 += (value5+value6);
			value4 = m_iZPaiXing[1];//ֱ��ȡ��ǰ�����Ƶ�lym2010-03-18
			if(value4>=10) {value4%=10;}
			if (value4>=8)
			{
				str.Format("%s������%d ���� ",m_ZhuangStr,value4);
				if (m_iGamePictureType)
				{	
					str.Format("�������%d ���� ",value4);
				}
			}
			else
			{
				str.Format("%s������%d ",m_ZhuangStr,value4);
				if (m_iGamePictureType)
				{	
					str.Format("�������%d ",value4);
				}
			}			
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+16,str);
			str="";
			if(m_iWinner==1)//ׯӮ
			{
				str.Format("%sӮ",m_ZhuangStr);
				if (m_iGamePictureType)
				{	
					str.Format("��Ӯ");
				}
			}
			else if(m_iWinner==2)//��Ӯ
			{
				str.Format("%sӮ",m_XiangStr);
				if (m_iGamePictureType)
				{	
					str.Format("��Ӯ");
				}
			}
			else if(m_iWinner==3)
			{
				str.Format("��Ӯ");
			}
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+32,str);
		}
		else if(m_iShowCard==7)//ׯ��3����
		{
			int value1 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][0]);//��ȡ�Ƶ�
			int value2 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][1]);//��ȡ�Ƶ�
			int value3 = CPokerGameLogic::GetCardNumber(m_iNowCard[1][2]);//��ȡ�Ƶ�
			if(value1>=10) {value1=0;}
			if(value2>=10) {value2=0;}
			if(value3>=10) {value3=0;}
			value1 += (value2+value3);
			value1 = m_iXPaiXing[1];//ֱ��ȡ��ǰ�����Ƶ�lym2010-03-18
			if(value1>=10) {value1 %= 10;}
			if (value1>=8)
			{
				str.Format("%s������%d ���� ",m_XiangStr,value1);
				if (m_iGamePictureType)
				{	
					str.Format("�ص�����%d ���� ",value1);
				}
			}
			else
			{
				str.Format("%s������%d ",m_XiangStr,value1);
				if (m_iGamePictureType)
				{	
					str.Format("�ص�����%d ",value1);
				}
			}			
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);

			value1 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][0]);//��ȡ�Ƶ�
			value2 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][1]);//��ȡ�Ƶ�
			value3 = CPokerGameLogic::GetCardNumber(m_iNowCard[0][2]);//��ȡ�Ƶ�
			if(value1>=10) {value1=0;}
			if(value2>=10) {value2=0;}
			if(value3>=10) {value3=0;}
			value1 += (value2+value3);
			value1 = m_iZPaiXing[1];//ֱ��ȡ��ǰ�����Ƶ�lym2010-03-18
			if(value1>=10) 
			{
				value1 %= 10;
			}
			if (value1>=8)
			{
				str.Format("%s������%d ����",m_ZhuangStr,value1);
				if (m_iGamePictureType)
				{	
					str.Format("�������%d ����",value1);
				}
			}
			else
			{
				str.Format("%s������%d ",m_ZhuangStr,value1);
				if (m_iGamePictureType)
				{	
					str.Format("�������%d ",value1);
				}
			}			
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+16,str);
			str="";
			if(m_iWinner==1)//ׯӮ
			{
				str.Format("%sӮ",m_ZhuangStr);
				if (m_iGamePictureType)
				{	
					str.Format("��Ӯ");
				}
			}
			else if(m_iWinner==2)//��Ӯ
			{
				str.Format("%sӮ",m_XiangStr);
				if (m_iGamePictureType)
				{	
					str.Format("��Ӯ");
				}
			}
			else if(m_iWinner==3)
			{
				str.Format("��Ӯ");
			}
			pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y+32,str);
		}
	}
	else if(m_bNowGameState==GS_WAIT_NEXT)//����״̬
	{
		str="����ʱ��...";//m_pTextStation
		pDC->TextOutA(m_pTextStation.x ,m_pTextStation.y,str);
	}
}

///���ƣ�DrawCard
///�����������ƣ�������ʾ����
///@param pDC ����dc ��iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return 
void CClientPlayView::DrawCard(CDC*pDC,int iWidth, int iHeight)
{
	if(m_iShowCard==0)
		return;
	int x=0,y=0;
	if(m_iShowCard==1)//
	{
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][0],m_pCardXEnd.x,m_pCardXEnd.y);
	}
	else if(m_iShowCard==2)
	{
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][0],m_pCardXEnd.x,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][0],m_pCardZEnd.x,m_pCardZEnd.y);
	}
	else if(m_iShowCard==3)
	{
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][0],m_pCardXEnd.x,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][1],m_pCardXEnd.x+20,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][0],m_pCardZEnd.x,m_pCardZEnd.y);
	}
	else if(m_iShowCard==4)
	{
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][0],m_pCardXEnd.x,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][1],m_pCardXEnd.x+20,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][0],m_pCardZEnd.x,m_pCardZEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][1],m_pCardZEnd.x+20,m_pCardZEnd.y);
	}
	else if(m_iShowCard==5)//��3���ƣ�ׯ������
	{
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][0],m_pCardXEnd.x,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][1],m_pCardXEnd.x+20,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][2],m_pCardXEnd.x+40,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][0],m_pCardZEnd.x,m_pCardZEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][1],m_pCardZEnd.x+20,m_pCardZEnd.y);
	}
	else if(m_iShowCard==6)//��2���ƣ�ׯ3����
	{
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][0],m_pCardXEnd.x,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][1],m_pCardXEnd.x+20,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][0],m_pCardZEnd.x,m_pCardZEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][1],m_pCardZEnd.x+20,m_pCardZEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][2],m_pCardZEnd.x+40,m_pCardZEnd.y);
	}
	else if(m_iShowCard==7)//ׯ��3����
	{
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][0],m_pCardXEnd.x,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][1],m_pCardXEnd.x+20,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[1][2],m_pCardXEnd.x+40,m_pCardXEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][0],m_pCardZEnd.x,m_pCardZEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][1],m_pCardZEnd.x+20,m_pCardZEnd.y);
		DrawACard(pDC,iWidth,iHeight,m_iNowCard[0][2],m_pCardZEnd.x+40,m_pCardZEnd.y);
	}
}

///���ƣ�EnableXiaZhu
///������������ע������ע
///@param enable true ʹ��ť���� ��false ʹ��ť������
///@return 
void CClientPlayView::EnableXiaZhu(bool enable)
{
	///20100207zht�޸İ���
	__int64 maxcannote = 0;
	for(int i=0;i<8;i++)
	{
		if(maxcannote<m_iMaxZhu[i])
			maxcannote = m_iMaxZhu[i];///<����ÿ������������µ�ע��	
	}
	__int64 nTempMoney=1; 
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nTempMoney*=10;
	}
	if(enable && m_iNowNTstation>=0)//������ע
	{

		__int64 money = GetMyAllNote();
		if(m_pUserItem[m_byMyStation]!=NULL)
		{
				OutputDebugString("������ׯ����44444");

		CString str="";
		str.Format("������ׯ���� ��mymoney=%i64d  money=%i64d m_iMaxNote=%i64d   maxcannote=%i64d   %i64d",m_pUserItem[m_byMyStation]->GameUserInfo.i64Money ,money,m_iMaxNote,maxcannote,m_iMyZhu[8]);
		OutputDebugString(str);

		str.Format("������ׯ���� 000 ��%d    %d    %d ",(m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - money) >= 100/nTempMoney,(m_iMaxNote<=0 ||(m_iMaxNote-m_iMyZhu[8])>=100/nTempMoney ),maxcannote >= 100/nTempMoney);
		OutputDebugString(str);

			int cannote=0;
			//EnableXiaZhu(false);
			if((m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - money) >= 100/nTempMoney  && (m_iMaxNote<=0 ||(m_iMaxNote-m_iMyZhu[8])>=100/nTempMoney ) && maxcannote >= 100/nTempMoney )
			{
				OutputDebugString("������ׯ����33333");
				m_btn100.EnableWindow(true);	// 100���밴ť
				cannote=1;
			}
			else
			{
				m_btn100.EnableWindow(false);	// 100���밴ť
			}
			if((m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - money) >= 1000/nTempMoney && (m_iMaxNote<=0 ||(m_iMaxNote-m_iMyZhu[8])>=1000/nTempMoney) && maxcannote >= 1000/nTempMoney )
			{
				m_btn1000.EnableWindow(true);	// 1000���밴ť
				cannote=2;
			}
			else
			{
				m_btn1000.EnableWindow(false);	// 1000���밴ť
			}
			if((m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - money) >= 10000/nTempMoney && (m_iMaxNote<=0 ||(m_iMaxNote-m_iMyZhu[8])>=10000/nTempMoney) && maxcannote >= 10000/nTempMoney )
			{
				m_btn1w.EnableWindow(true);		// 1����밴ť
				cannote=3;
			}
			else
			{
				m_btn1w.EnableWindow(false);		// 1����밴ť
			}
			if((m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - money) >= 100000/nTempMoney && (m_iMaxNote<=0 ||(m_iMaxNote-m_iMyZhu[8])>=100000/nTempMoney) && maxcannote >= 100000/nTempMoney )
			{
				m_btn10w.EnableWindow(true);	// 10����밴ť
				cannote=4;
			}
			else
			{
				m_btn10w.EnableWindow(false);	// 10����밴ť
			}
			if((m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - money) >= 1000000/nTempMoney && (m_iMaxNote<=0 ||(m_iMaxNote-m_iMyZhu[8])>=1000000/nTempMoney)  && maxcannote >= 1000000/nTempMoney )
			{
				CString str="";
				str.Format("��ע���ԣ�m_iMaxNote=%i64d  tt=%i64d  gg=%i64d  m_iMyZhu[8]=%i64d",m_iMaxNote,m_iMaxNote-m_iMyZhu[8],1000000/nTempMoney,m_iMyZhu[8]);
				OutputDebugString(str);
				m_btn100w.EnableWindow(true);	// 100����밴ť
				cannote=5;
			}
			else
			{
				m_btn100w.EnableWindow(false);	// 100����밴ť
			}
///72��һ��
#ifdef SET72 

			int iMaxjeton = 10000000 ;
			/*if(m_bUse500WJeton)
			{
				iMaxjeton = 5000000 ; 
			}*/

			if((m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - money) >= iMaxjeton/nTempMoney && (m_iMaxNote<=0 ||(m_iMaxNote-m_iMyZhu[8])>=iMaxjeton/nTempMoney ) && maxcannote >= iMaxjeton/nTempMoney )
			{
				m_btn500w.EnableWindow(true);	// 500����밴ť
				cannote=6;
			}
			else
			{
				m_btn500w.EnableWindow(false);	// 500����밴ť
			}
#else
			if((m_pUserItem[m_byMyStation]->GameUserInfo.i64Money - money) >= 5000000/nTempMoney && (m_iMaxNote<=0 ||(m_iMaxNote-m_iMyZhu[8])>=5000000/nTempMoney ) && maxcannote >= 5000000/nTempMoney )
			{
				m_btn500w.EnableWindow(true);	// 500����밴ť
				cannote=6;
			}
			else
			{
				m_btn500w.EnableWindow(false);	// 500����밴ť
			}
#endif

			if(m_iChooseNote>cannote)
			{				
				//m_iChooseNote=cannote;
				//OnShowCursor(FALSE);
				OnRestoreCursor();
				switch(cannote)
				{
				case 1:
					OnBtnClicked100();	// 100���밴ť
					break;
				case 2:
					OnBtnClicked1000();	// 1000���밴ť
					break;
				case 3:
					OnBtnClicked1w();		// 1����밴ť
					break;
				case 4:
					OnBtnClicked10w();	// 100����밴ť
					break;
				case 5:
					OnBtnClicked100w();	// 500����밴ť
					break;
				}

			}
		}
		else
		{
			m_btn100.EnableWindow(true);	// 100���밴ť
			m_btn1000.EnableWindow(true);	// 1000���밴ť
			m_btn1w.EnableWindow(true);		// 1����밴ť
			m_btn10w.EnableWindow(true);	// 10����밴ť
			m_btn100w.EnableWindow(true);	// 100����밴ť
			m_btn500w.EnableWindow(true);	// 500����밴ť
		}
	}
	else //������ע
	{
		m_btn100.EnableWindow(false);	// 100���밴ť
		m_btn1000.EnableWindow(false);	// 1000���밴ť
		m_btn1w.EnableWindow(false);		// 1����밴ť
		m_btn10w.EnableWindow(false);	// 10����밴ť
		m_btn100w.EnableWindow(false);	// 100����밴ť
		m_btn500w.EnableWindow(false);	// 500����밴ť
	}
}
///��������500��Ķ���
void CClientPlayView::StartChao500Move(int type)
{
	//SetTimer(IDR_FAPAI,100,NULL);
	//m_iMoveTime=10;
	m_iMoveType = type+1;

}
///���Ƴ���500��Ķ���
void CClientPlayView::DrawChao500Move(CDC*pDC,int iWidth, int iHeight)
{
	if(m_iMoveTime>0)
	{
		CPoint piont(0,0);
		piont=m_iYUcenter[m_iMoveType-1];
	/////���ƶ���֡
	}
}

///���ƣ�DrawCursor
///���������ƹ�궯��
///@param pDC ����dc ��iWidth ���ڿ�ȡ�iHeight ���ڸ߶ȡ�
///@return 
void CClientPlayView::DrawCursor(CDC*pDC,int iWidth, int iHeight)
{
	if(m_bNowGameState != GS_SEND_CARD)//����עʱ�䲻��Ӧ
		return;
	if(	m_byMyStation==m_iNowNTstation)//�Լ���λ��,ׯ�Ҳ�����ע
		return;

	if(m_iChooseNote>0 && m_byMyStation!=m_iNowNTstation)
	{
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		CGameImageHelper	cursor(&m_imgchouma);
		CBitmap *pOldBmp = dc.SelectObject(cursor);
		::TransparentBlt(pDC->GetSafeHdc(),m_pCursor.x-16,m_pCursor.y-16,cursor.GetWidth()/6,cursor.GetHeight(),
			dc.GetSafeHdc(),(m_iChooseNote-1)*cursor.GetWidth()/6,0,cursor.GetWidth()/6,cursor.GetHeight(),RGB(255,0,255));
		dc.SelectObject(pOldBmp);
		dc.DeleteDC();
	}
	else
	{
	//	ShowCursor(TRUE);
	}
}
///���½��㴰�ڵ�λ��
void CClientPlayView::UpdateResultWindow(int x,int y)
{
//m_pViewCenter
	//CRect rect(0,0,0,0);
	//GetClientRect(&rect);
	//CGameImage reimg;
	//TCHAR path[MAX_PATH];
	//wsprintf(path,TEXT("image\\result.bmp"));
	//reimg.SetLoadInfo(path,true);
	//CPoint point(m_pDitu.x+135-126,m_pDitu.y +35+y+48);
	//ClientToScreen(&point);
	//SafeMoveWindow(&m_Result, point.x,m_pDitu.y +35+y+48/*m_pDitu.y +35+y+48*/ , reimg.GetWidth(),reimg.GetHeight());
	//UpdateViewFace(NULL);
}

///���ƣ�DrawResultFrame
///����������͸�������
///@param pDC ����dc ��iXPos λ�á�iYPos λ�á�
///@return 
void CClientPlayView::DrawResultFrame(CDC*pDC, int iXPos, int iYPos)
{
	if(!m_bShowResult)
		return;
	CGameImageHelper help(&m_imgResult);
	CGameImageHelper help1(&m_imgResult1);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(help);

	CDC memdc;
	memdc.CreateCompatibleDC(pDC);	
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, help.GetWidth(), help.GetHeight());
	CBitmap *pOldBmp1 = (CBitmap*)memdc.SelectObject( &bitmap );
	memdc.SetBkMode(TRANSPARENT);

	// ���ϱ���ͼƬ
	memdc.BitBlt(0,0, help.GetWidth(), help.GetHeight(), pDC, iXPos, iYPos, SRCCOPY);

	// ������
	::TransparentBlt(memdc.GetSafeHdc(),0,0,help.GetWidth(), help.GetHeight(),dc.GetSafeHdc(),0,0,help.GetWidth(),help.GetHeight(),RGB(255,0,255));	

	// ����͸��
	BLENDFUNCTION rBlendProps;
	rBlendProps.BlendOp = AC_SRC_OVER;
	rBlendProps.BlendFlags = 0;
	rBlendProps.AlphaFormat = 0;
	rBlendProps.SourceConstantAlpha = 150;//
	AlphaBlend( pDC->GetSafeHdc(), iXPos, iYPos, help.GetWidth(), help.GetHeight(), memdc.GetSafeHdc(), 0, 0, 
		help.GetWidth(), help.GetHeight(), rBlendProps );
	dc.SelectObject(help1);
	// ������
	::TransparentBlt(pDC->GetSafeHdc(),iXPos+1,iYPos,help1.GetWidth(), help1.GetHeight(),dc.GetSafeHdc(),0,0,help1.GetWidth(),help1.GetHeight(),RGB(255,0,255));	

	memdc.SelectObject(pOldBmp1);
	memdc.DeleteDC();
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
/////////////////////////////////////////
	//CString strme="",strnt="",strxian="",stra=""; 
	//int money=0;
	//int temp=0;
	////money = m_iMeFen;
	//strme="";
	//NumToChar(m_iMeFen,&strme);
	//strnt="";
	//NumToChar(m_iZhuangFen,&strnt);
	//strxian="";
	//NumToChar(m_iXianFen,&strxian);


	//����������Ϣm_pDitu
	CFont font, *pOldFont;
	font.CreateFont(16,0,0,0,700,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	pOldFont = (CFont*)pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);
	CRect rectName(0,0,0,0);
	
	CString str="";
	str.Format("���㣺ׯ��=%d �ҷ�=%d",m_iZhuangFen,m_iMeFen);
	OutputDebugString(str);

///////////////////////////////////////////////////////////
	if(m_byMyStation == m_iNowNTstation)//�Լ���ׯ��
	{
		pDC->SetTextColor(RGB(255,255,100));//�Լ��ú�ɫ
		CString str="";
		char szChar[MAX_PATH] ={ 0 } ; 
		//NumToChar(m_iMeFen,&str);

		 ChangeNumToCStringByPower(m_iMeFen ,true ,"," ,str) ; 

		//���Ʊ����ĵ÷�
		rectName.SetRect(iXPos+48,iYPos+38,iXPos+150+25,iYPos+67);
		if(m_isWinRed && m_iMeFen>0)
			pDC->SetTextColor(RGB(255,0,0));//�Լ��ú�ɫ
		else
			pDC->SetTextColor(RGB(m_Rcolor,m_Gcolor,m_Bcolor));
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//���Ʊ����ķ�����m_iMeFanFen
		//NumToChar(m_iMeFanFen,&str);
		 ChangeNumToCStringByPower(m_iMeFanFen ,true ,"," ,str) ; 

		rectName.SetRect(iXPos+154+20,iYPos+38,iXPos+278+40,iYPos+67);
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//////////
		//ׯ�ҵ÷�
		//NumToChar(m_iZhuangFen,&str);
		ChangeNumToCStringByPower(m_iZhuangFen ,true ,"," ,str) ;  

		//���Ʊ����ĵ÷�
		rectName.SetRect(iXPos+48,iYPos+71+2,iXPos+150+25,iYPos+100+2);
		if(m_isWinRed && m_iZhuangFen>0)
			pDC->SetTextColor(RGB(255,0,0));//�Լ��ú�ɫ
		else
			pDC->SetTextColor(RGB(m_Rcolor,m_Gcolor,m_Bcolor));
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}
	else 
	{
		pDC->SetTextColor(RGB(255,255,100));//�Լ��ú�ɫ
		CString str="";
		char szChar[MAX_PATH] ={ 0 } ; 

		//NumToChar(m_iMeFen,&str);
		ChangeNumToCStringByPower(m_iMeFen ,true ,"," ,str) ;

		//���Ʊ����ĵ÷�
		rectName.SetRect(iXPos+48,iYPos+38,iXPos+150+25,iYPos+67);
		if(m_isWinRed && m_iMeFen>0)
			pDC->SetTextColor(RGB(255,0,0));//�Լ��ú�ɫ
		else
			pDC->SetTextColor(RGB(m_Rcolor,m_Gcolor,m_Bcolor));
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		//���Ʊ����ķ�����m_iMeFanFen
		//NumToChar(m_iMeFanFen,&str);
		ChangeNumToCStringByPower(m_iMeFanFen ,true ,"," ,str) ;

		rectName.SetRect(iXPos+154+20,iYPos+38,iXPos+278+40,iYPos+67);
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		//////////
		//ׯ�ҵ÷�
		//NumToChar(m_iZhuangFen,&str);
		ChangeNumToCStringByPower(m_iZhuangFen ,true ,"," ,str) ; 

		//���Ʊ����ĵ÷�
		rectName.SetRect(iXPos+48,iYPos+71+2,iXPos+150+25,iYPos+100+2);
		if(m_isWinRed && m_iZhuangFen>0)
			pDC->SetTextColor(RGB(255,0,0));//�Լ��ú�ɫ
		else
			pDC->SetTextColor(RGB(m_Rcolor,m_Gcolor,m_Bcolor));
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}
	pDC->SelectObject(pOldFont);
	font.DeleteObject();

}

///���ƣ�ShowNowNote
///������������������ʾ��ǰ��ע�ĳ���
///@param 
///@return 
void CClientPlayView::ShowNowNote()
{ 
	///20100207zht�޸İ���
	__int64 nTempMoney=1; 
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nTempMoney*=10;
	}

				
	for(int i=0;i<8;i++)//�˸�����Ľ��
	{
		if(m_iQuYuZhu[i]>0)
		{
			__int64 temp=0;
			__int64 num500w=0,num100w=0,num10w=0,num1w=0,num1000=0,num100=0;
			temp = m_iQuYuZhu[i];
			num500w = temp/(5000000/nTempMoney);
			temp%=(5000000/nTempMoney);
			num100w = temp/(1000000/nTempMoney);
			temp%=(1000000/nTempMoney);
			num10w = temp/(100000/nTempMoney);
			temp%=(100000/nTempMoney);
			num1w = temp/(10000/nTempMoney);
			temp%=(10000/nTempMoney);
			num1000 = temp/(1000/nTempMoney);
			temp%=(1000/nTempMoney);
			num100 = temp/(100/nTempMoney);
			SetNowNoteData(i,num500w,num100w,num10w,num1w,num1000,num100);
			CString str;
			str.Format("zhtout:m_iQuYuZhu[%d]=%i64d %i64d,%i64d,%i64d,%i64d,%i64d,%i64d",i,m_iQuYuZhu[i],num500w,num100w,num10w,num1w,num1000,num100);
			OutputDebugString(str);
		}
	}


}

///���ƣ�SetNowNoteData
///������������������ʾ��ǰ��ע�����õ�ǰ��ע������
///@param type���� ��num5bw 500��ĳ���������num1bw 100�����������num10w 10�����������num1w 1�����������numq 1000����������numb 100����������
///@return 
void CClientPlayView::SetNowNoteData(int type,int num5bw,int num1bw,int num10w,int num1w,int numq,int numb)
{
	__int64 numnote = 0;
	numnote = num5bw + num1bw + num10w + num1w + numq + numb;
	CPoint point(0,0);
	for(int i=0;i<numnote;i++)
	{
		point.SetPoint(0,0);
		switch(type)
		{
		case 0://ׯ
			{
				point.x=(rand()%20 - 10)*5;
				point.y=(rand()%16 - 8)*4;
			}
			break;
		case 1://ׯ����
			{
				point.x=(rand()%20 - 10)*5;
				point.y=(rand()%6 - 3)-5;
			}
			break;
		case 2://ׯ����
			{
				point.x=(rand()%25 - 12)*5;
				point.y=(rand()%6 - 3)*3;
			}
			break;
		case 3://��
			{
				point.x=(rand()%20 - 10)*5;
				point.y=(rand()%16 - 8)*4;
			}
			break;
		case 4://������
			{
				point.x=(rand()%20 - 10)*5;
				point.y=(rand()%6 - 3)-5;
			}
			break;
		case 5://�ж���
			{
				point.x=(rand()%20 - 10)*5;
				point.y=(rand()%6 - 3)*3;
			}
			break;
		case 6://��
			{
				point.x=(rand()%20 - 10)*5;
				point.y=(rand()%12 - 6)*4;
			}
			break;
		case 7://ͬ���
			{
				point.x=(rand()%20 - 10)*5;
				point.y=(rand()%6 - 3)*3;
			}
			break;
		}
		m_pJinBi[type][i].x = point.x;     //��ҵ�ƫ���� 
		m_pJinBi[type][i].y = point.y;     //��ҵ�ƫ����  

		if(num5bw>0 && i<num5bw)//��500��Ľ��
		{
			m_iJinBi[type][i] = 6;
		}
		else if(num1bw && i<(num5bw + num1bw))
		{
			m_iJinBi[type][i] = 5;
		}
		else if(num10w && i<(num5bw + num1bw + num10w))
		{
			m_iJinBi[type][i] = 4;
		}
		else if(num1w && i<(num5bw + num1bw + num10w + num1w))
		{
			m_iJinBi[type][i] = 3;
		}
		else if(numq && i<(num5bw + num1bw + num10w + num1w + numq))
		{
			m_iJinBi[type][i] = 2;
		}
		else
		{
			m_iJinBi[type][i] = 1;
		}
	}

}

///���ƣ�Init
///������ÿ�ֽ������ʼ����ͼ����
///@param 
///@return 
void CClientPlayView::Init()
{
	//m_LuziDlg.ShowWindow(false);
	m_bShowRect = false ;//����Ӯ�������˸
	memset(&m_iZPaiXing, 0, sizeof(m_iZPaiXing)); //ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	memset(&m_iXPaiXing, 0, sizeof(m_iXPaiXing));//�м�����
	m_iWinner = 0 ;				//Ӯ��1 ׯ��2�У�3�ͣ���Ӯ��
	m_iKaiPai = 0 ;				//���ѿ�������1ׯ��2ׯ������3ׯ���ӣ�4�У�5��������6�ж��ӣ��ͣ�ͬ���
	m_bShowResult = false; //�Ƿ���ʾ������Ϣ
	memset(&m_CardCount, 0, sizeof(m_CardCount));		 //ׯ���Ƶ�����
	memset(&m_iNowCard, 0, sizeof(m_iNowCard));		 //��ǰ�Ƶ�  
	memset(&m_iResult, -1, sizeof(m_iResult));		 //���汾�֣�ׯ�Ƶ㣬���Ƶ㣬ׯ���ͣ������ͣ�Ӯ�������Լ��ý��
	memset(&m_iZCardInfo, -1, sizeof(m_iZCardInfo));		  //ׯ������Ϣ��ׯ�ҵ�������1���Ƶĵ�����ǰ�����Ƶĵ��� 
	memset(&m_iXCardInfo, -1, sizeof(m_iXCardInfo));		  //�е�����Ϣ��ׯ�ҵ��������мҵ�������
	memset(&m_iQuYuZhu, 0, sizeof(m_iQuYuZhu));		//����ÿ�������µ�ע�0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	memset(&m_iMyZhu, 0, sizeof(m_iMyZhu));			//��������Լ���ÿ�������µ�ע�0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	memset(&m_iMaxZhu, 0, sizeof(m_iMaxZhu));	//����ÿ������������µ�ע��
	memset(&m_iJinBi, 0, sizeof(m_iJinBi));	//����ڰ˸������µĽ���������¼ǰ2000��
	memset(&m_pJinBi, 0, sizeof(m_pJinBi));	//����ڰ˸������µĽ��λ���������¼ǰ2000��
	memset(&m_iJinBiNum, 0, sizeof(m_iJinBiNum));	 //�˸�����ÿ������Ľ����
	//memset(&m_iMePlayData, 0, sizeof(m_iMePlayData));	 //����Լ�����Ӯ���
	memset(&m_iWinQuYu, 0, sizeof(m_iWinQuYu));	//��Ϸ��ӮǮ���� 0ׯ��1ׯ������2ׯ���ӣ�3�У�4��������5�ж��ӣ�6�ͣ�7ͬ���
	// �����ʾ������
	::ZeroMemory(m_ContinueShowCardCount, sizeof(m_ContinueShowCardCount));

	::ZeroMemory(m_iTempGameInfo,sizeof(m_iTempGameInfo));
	
	m_iMoveTime =0 ;  //����ʱ��
	m_iMoveType = -1;  //�����Ļ�������

	m_iShowCard = 0;//��ʾ��
	m_iCardNum =0;				//���ŵڼ����ƵĶ���
	m_bZhuang=false;				//������ׯ�һ����мҵ���
	m_bCardMove=false;			//�Ƿ����ڽ��з���
	m_bSendCard =false;			//ȷ���Ƿ��ƽ���
	m_iChooseNote=0;			///��ǰѡ��ĳ���


	m_iChooseRect=-1;			//������ڵ��������
	m_iCardZhen =-1;			//��ǰ���Ƶڼ�֡
	m_iShowCard=0;			//��ʾ��,0����ʾ��1��ʾһ�ţ�2��ʾ
	m_iZhongZhu =0;			 //���ѵ�ǰ��ע��	
	m_iAllJiBiCount=0;        //��ǰ�Ѿ�Ѻ�˶��ٽ�ң�

	m_iZhuangFen=0;		//����ׯ�ҵĵ÷�
	m_iXianFen=0;			//�����мҵĵ÷�
	m_iMeFen=0;			//��������Լ��ĵ÷�
	m_iMeFanFen = 0;		//������ҷ����ķ֣�����ʱ����
	
	if(!CheckMeZhuang())
	{
		//��Ϊ��ׯͼƬ
		SetApplyBanker(true);
	}
	else
	{
		//��Ϊ��ׯͼƬ
		SetApplyBanker(false);
	}
///lym2010-03-19�ܷ�����ׯ��ť
	m_bCanShangZhuang = true;
	m_iMessageCount = 0;
	//m_iNowJuShu = 0;				//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
	memset(&m_iAIMoney, 0, sizeof(m_iAIMoney));	///�����˵���ע
	m_bHideAIMoney = true;			///�Ƿ����ػ�������ע
	for(int i=0;i<8;i++)
	{
		m_iJinBiNum[i] = 0;//ÿ������Ľ����
		for(int j=0;j<MAXJINBI;j++)
		{
			m_iJinBi[i][j]=0;    //����ڰ˸������µĽ���������¼ǰ2000��
			m_pJinBi[i][j].x=0;  //����ڰ˸������µĽ��λ���������¼ǰ2000��
			m_pJinBi[i][j].y=0;  //����ڰ˸������µĽ��λ���������¼ǰ2000��
		}
	}
	
}

/// ����������ׯ��ť
///@param bApply true��ׯ ,false ��ׯ
void CClientPlayView::SetApplyBanker(bool bApply)
{
	/*CString fdsa;
	fdsa.Format("MSG_%d,%d_____123",m_bNowGameState,ff);
	OutputDebugString(fdsa);*/

	TCHAR szName[MAX_PATH];
	wsprintf(szName, bApply ? TEXT("image\\shenqing_bt.bmp") : TEXT("image\\xiazhuan_bt.bmp"));
	m_btnshangz.LoadButtonBitmap(szName,false);
}

/// �ָ��������״̬
void CClientPlayView::OnRestoreCursor()
{ 
	OnShowCursor(TRUE);
	SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND1))); //���ù��
	GetCursorPos(&m_pCursor);        //�������λ��);
	ScreenToClient(&m_pCursor);
}

/// �������״̬
///@param bShow true��ʾ��� ,false �������
void CClientPlayView::OnShowCursor(BOOL bShow)
{
	int iCount=ShowCursor(bShow);
	while ((bShow && 0>iCount) || (!bShow && 0<=iCount))
	{
		iCount = ShowCursor(bShow);
	}
	
}

///������ת�ɴ��ַ���
///@param result Ҫת������  szText ת������ַ���
void CClientPlayView::NumToChar(__int64 result,CString* szText)
{
	CString str1="0",str2="";
	CString str="";
	str.Format("%I64d",result);

	if (m_nPowerOfGold<=0||0 == result)
	{
		str2="";
	}
	else
	{
		for (int i=m_nPowerOfGold;i>0;i--)
		{
			str2+=str1;
		}
	}

// 	if(result==0)
// 	{
// 		str2 = "";
// 	}
	*szText = str + str2;


	
	
	//��ӡ������ָ���

	if(m_SpeCount==4)
	{

		if(result>0)//����
		{
			if(szText->GetLength()>8)
			{
				szText->Insert(szText->GetLength()-4,',');
				szText->Insert(szText->GetLength()-9,',');
			}
			else if(szText->GetLength()>4)
			{
				szText->Insert(szText->GetLength()-4,',');
			}
		}
		else
		{
			if(szText->GetLength()>9)
			{
				szText->Insert(szText->GetLength()-4,',');
				szText->Insert(szText->GetLength()-9,',');
			}
			else if(szText->GetLength()>5)
			{
				szText->Insert(szText->GetLength()-4,',');
			}
		}

	}

	else if(m_SpeCount== 3)
	{

		if(result>0)//����
		{
			if(szText->GetLength()>9)
			{		
				szText->Insert(szText->GetLength()-3,',');
				szText->Insert(szText->GetLength()-7,',');
				szText->Insert(szText->GetLength()-11,',');
			}
			else if(szText->GetLength()>6)
			{
				szText->Insert(szText->GetLength()-3,',');
				szText->Insert(szText->GetLength()-7,',');
			}
			else if(szText->GetLength()>3)
			{
				szText->Insert(szText->GetLength()-3,',');
			}
		}
		else
		{
			if(szText->GetLength()>9)
			{		
				szText->Insert(szText->GetLength()-3,',');
				szText->Insert(szText->GetLength()-7,',');
				szText->Insert(szText->GetLength()-11,',');
			}
			else if(szText->GetLength()>7)
			{
				szText->Insert(szText->GetLength()-3,',');
				szText->Insert(szText->GetLength()-7,',');
			}
			else if(szText->GetLength()>4)
			{
				szText->Insert(szText->GetLength()-3,',');
			}
		}

	}
	else
	{

		if(result>0)//����
		{
			if(szText->GetLength()>8)
			{
				szText->Insert(szText->GetLength()-4,',');
				szText->Insert(szText->GetLength()-9,',');
			}
			else if(szText->GetLength()>4)
			{
				szText->Insert(szText->GetLength()-4,',');
			}
		}
		else
		{
			if(szText->GetLength()>9)
			{
				szText->Insert(szText->GetLength()-4,',');
				szText->Insert(szText->GetLength()-9,',');
			}
			else if(szText->GetLength()>5)
			{
				szText->Insert(szText->GetLength()-4,',');
			}
		}

	}

}
///��ȡ����Լ���ǰ����ע�ܶ�
///@return ������ҵ�ǰ��ע���ܶ�
__int64 CClientPlayView::GetMyAllNote()
{
	__int64 note=0;
	for(int i=0;i<8;i++)
	{
		note+=m_iMyZhu[i];
	}
	return note;
}
///��ʱ�����Լ�����ע��ť�����
void CClientPlayView::SetXiaZhuBtn()
{
	//if(m_iChooseNote == 0)//��ǰѡ��ĳ��� 0û�У�1��100��2��1000��3��1w��4��10w��5��100w,6��500w
	//	return;
	if(m_byMyStation==m_iNowNTstation)
	{
		EnableXiaZhu(false);
		return;
	}
	///20100207zht�޸İ���
	__int64 nTempMoney=1; 
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nTempMoney*=10;
	}

	__int64 money = GetMyAllNote();
	__int64 nownote = 0;
	switch(m_iChooseNote)
	{
	case 1:
		nownote = 100/nTempMoney;
		break;
	case 2:
		nownote = 1000/nTempMoney;
		break;
	case 3:
		nownote = 10000/nTempMoney;
		break;
	case 4:
		nownote = 100000/nTempMoney;
		break;
	case 5:
		nownote = 1000000/nTempMoney;
		break;
	case 6:
///72��һ��
#ifdef SET72 
		/*if(m_bUse500WJeton)
		{
			nownote = 5000000/nTempMoney;
		}
		else
		{*/
			nownote = 10000000/nTempMoney;
		//}
		
#else
		nownote = 5000000/nTempMoney;
#endif
		break;
	}
	if(m_pUserItem[m_byMyStation] != NULL)
	{
		CString str="";
		str.Format("������ׯ���� ��ע����111�� m_iMyZhu[8]=%i64d m_iMaxNote=%i64d money=%i64d  nownote=%i64d  st=%d nt=%i64d",m_iMyZhu[8],m_iMaxNote,m_iMaxNote-m_iMyZhu[8],nownote,m_byMyStation,m_iNowNTstation);
		OutputDebugString(str);
		if(m_byMyStation!=m_iNowNTstation && m_iNowNTstation>=0)
			EnableXiaZhu(true);
	}

	UpdateViewFace(NULL);
}

#ifdef VIDEO
///�����ǳƵ������Ƶ���ھ���д�����rect,������TRUE;û�ҵ�����falst
bool CClientPlayView::GetVideoRect(CString  strNickname, RECT& rect, CString& pic)
{
	CString strLocalName = _T("");
	CString logopathstr = _T("");
	for (int iCount=0; iCount<MAX_PEOPLE; ++iCount)
	{
		if (m_pUserItem[iCount])
		{    
			strLocalName = m_pUserItem[iCount]->GameUserInfo.nickName;
			if(strLocalName.Left(MAX_VIDEO_NICKNAME_LEN) == strNickname)
			{
				rect=m_rectVideoFrame[GetViewSeat(m_pUserItem[iCount]->GameUserInfo.bDeskStation)];
				logopathstr.Format("image\\log\\gamelog%d.bmp", m_pUserItem[iCount]->GameUserInfo.bLogoID);
				CString  tempstr = CBcfFile::GetAppPath()+logopathstr;
				pic = tempstr;
				return true;
			}
		}
	}
	return false; //û�ҵ�
}
#endif


///��ʾ����ͼƬ
void CClientPlayView::DrawPromptOpenCard(CDC * pDC, int x, int y)
{
	if (NULL == pDC)
	{
		return ;
	}

	CGameImageHelper imgPromptOpenCard(&m_imgPromptOpenCard);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgPromptOpenCard);
	if(m_bShowPromptOpenCard)
	{
		::TransparentBlt(pDC->GetSafeHdc(),m_pDitu.x + 5,m_pDitu.y + 100,imgPromptOpenCard.GetWidth(),imgPromptOpenCard.GetHeight(),
			dc.GetSafeHdc(),0,0,imgPromptOpenCard.GetWidth(),imgPromptOpenCard.GetHeight(),dc.GetPixel(0,0));
	}
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

///��ʾ��ׯͼƬ
void CClientPlayView::DrawPromptChangeNt(CDC * pDC, int x, int y)
{
	if (NULL == pDC)
	{
		return ;
	}

	CGameImageHelper imgPromptChangeNt(&m_imgPromptChangeNt);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = (CBitmap*)dc.SelectObject(imgPromptChangeNt);
	if(m_bShowPromptChangeNt)
	{
		::TransparentBlt(pDC->GetSafeHdc(),m_pDitu.x + 5,m_pDitu.y + 100,imgPromptChangeNt.GetWidth(),imgPromptChangeNt.GetHeight(),
			dc.GetSafeHdc(),0,0,imgPromptChangeNt.GetWidth(),imgPromptChangeNt.GetHeight(),dc.GetPixel(0,0));
	}
	dc.SelectObject(pOldBmp);
	dc.DeleteDC();
}

///���ÿ�����ʾͼƬ
void CClientPlayView::SetPromptOpenCard(bool bShowFlag)
{
	m_bShowPromptOpenCard = bShowFlag;
	UpdateViewFace(NULL);
}

///���û�ׯ��ʾͼƬ
void CClientPlayView::SetPromptChangeNt(bool bShowFlag)
{
	m_bShowPromptChangeNt = bShowFlag;
	UpdateViewFace(NULL);
}


///��ʾ���س����ͻ���
void CClientPlayView::OnBtnClickedSuper(void)
{
//m_LuziDlg
	if(m_SuperDlg.IsWindowVisible())
	{
		m_SuperDlg.ShowWindow(false);
	}
	else 
	{
		m_SuperDlg.ShowWindow(true);
	}
}
//��ȡ������ҵ��ǳ�
void CClientPlayView::GetAllUserNicheng()
{
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(m_pUserItem[i] == NULL)
			continue;
		memcpy(m_SuperData.sUserInfo[i].cUserName,m_pUserItem[i]->GameUserInfo.nickName,sizeof(m_pUserItem[i]->GameUserInfo.nickName));
	}
}
///�����ϵ�Ǯ����ʵ��Ǯת��
__int64 CClientPlayView::ChangeMoneyByPower(__int64  nMoney , bool  bUIMoney) 
{
	__int64  nTempMoney = nMoney ; 

	__int64   nPower = 1 ;

	for(int i = 0 ; i < abs(m_nPowerOfGold) ; i++ )
	{
		nPower *= 10 ;
	}

	if(m_nPowerOfGold >=0)
	{
		if(bUIMoney)
		{
			nTempMoney*=nPower ; 
		}
		else
		{
			nTempMoney /=nPower ;  
		}
	}
	else
	{
		if(bUIMoney)
		{
			nTempMoney/=nPower ; 
		}
		else
		{
			nTempMoney *=nPower ;  
		}
	}

	return nTempMoney ; 
}
///ת���ַ���
void  CClientPlayView::ChangeNumToCStringByPower(__int64 nMoney ,bool bHaveSpace ,TCHAR *chSpace, CString &strRet)
{
	char szChar[MAX_PATH] = {0} ; 
	GlbGetNumString(szChar ,nMoney ,m_nPowerOfGold ,bHaveSpace ,chSpace) ;
	strRet.Format("%s" , szChar) ; 

	return ; 
}

LRESULT CClientPlayView::OnShowRobotChip(WPARAM wParam, LPARAM lParam)
{
	 m_bHideAIMoney = (int) wParam;
	// UpdateViewFace(NULL);
	return 0;
}