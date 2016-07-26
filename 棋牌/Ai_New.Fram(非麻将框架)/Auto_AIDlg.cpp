// Auto_AIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Auto_AI.h"
#include "Auto_AIDlg.h"
#include "afcfunction.h"
#include "showhistory.h"
#include "iphlpapi.h"	//�������� zxj 2009-11-12

int CAuto_AIDlg::m_nRoomPeopleSize = 0;

#pragma comment(lib,"Iphlpapi.lib")	//�������� zxj 2009-11-12
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAuto_AIDlg �Ի���




CAuto_AIDlg::CAuto_AIDlg( CWnd * pParent)
: CBaseRoom(IDD_AUTO_AI_DIALOG)//Dialog(CAuto_AIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bConnectToWitchServer = 0;
	m_iEnterKindID = 10;
	m_iEnterNameID = 704;
	m_iEnterRoomID = 704;
	m_bNowRoomIndex = 0;
	m_iAccountsStart = 0;
	m_iCurrentAccounts = 0;
	m_CenterServerPort = 0;
	m_iRealLogonCount  = 0;
	::memset(&m_PlaceUserInfo,0,sizeof(m_PlaceUserInfo));
	::memset(m_CurrentLogonName,0,sizeof(m_CurrentLogonName));
	::memset(m_CurrentNickName,0,sizeof(m_CurrentNickName));
	::memset(m_szNickFix,0,sizeof(m_szNickFix));
	::memset(m_szNameFix,0,sizeof(m_szNameFix));

	m_bCountLogon = MAX_LOGON_COUNT;
	m_bEnterStyle = 0x01;
	m_bAutoRegister = 0;
	lstrcpy(m_CurrentLogonName,"dkl1001");
	lstrcpy(m_szNickFix,"���");
	lstrcpy(m_szNameFix,"dkl");
	m_bMachineAndPlay = true;//������������Ƿ�ͬ��
	m_bAutoDesk       = false;//�Զ�����
	m_bOpenBankBase	  = FALSE;//�Ƿ���������ȡ
	m_iMachineCheckTime  = 5000;//������UserID
	m_bMachineDeskCount= PLAY_COUNT;//ÿ��������������

	for (int i=0; i<MAX_GAME_ROOM; i++)
	{
		m_RoomInfo[i].pGameRoomWnd = NULL;
	}

	m_iRealLogonCount = 0;
	m_pGameRoom=NULL;

	m_bUseNew = 0;

	//��ʼ�� SOCKET
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return;
	}
}

CAuto_AIDlg::~CAuto_AIDlg()
{
	//new ����࣬��Ҫ������˵�Ĺ�ȥ��
	for (int i=0; i<m_bNowRoomIndex; i++)
	{
		if (m_RoomInfo[i].pGameRoomWnd != NULL)
		{
			delete m_RoomInfo[i].pGameRoomWnd;
			m_RoomInfo[i].pGameRoomWnd = NULL;
		}
		
	}

	::WSACleanup();
}

void CAuto_AIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_EventList);
}

BEGIN_MESSAGE_MAP(CAuto_AIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()

	ON_WM_CREATE()
	ON_MESSAGE(IDM_CLOSE_GAME_ROOM,OnCloseGameRoom)
	ON_MESSAGE(IDM_LOGON_FAIL, OnLogonFail)
	ON_BN_CLICKED(IDC_BUTTON1, &CAuto_AIDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BUTTON2, &CAuto_AIDlg::OnBnClickedMix)
	ON_MESSAGE(IDM_UPDATE_USER, OnUpdateMoney)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	ON_BN_CLICKED(IDC_BUTTON3, &CAuto_AIDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDCANCEL, &CAuto_AIDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK6, &CAuto_AIDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CAuto_AIDlg::OnBnClickedCheck7)
END_MESSAGE_MAP()


// CAuto_AIDlg ��Ϣ�������

BOOL CAuto_AIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)|WS_MINIMIZEBOX);
	//������������
	m_GameList.Create(TVS_HASBUTTONS | TVS_HASLINES | TVS_DISABLEDRAGDROP | TVS_TRACKSELECT | WS_VISIBLE | WS_TABSTOP,  CRect(0, 0,200,200), this, 200);
	m_GameList.ShowWindow(SW_HIDE);
	m_GameList.Init();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ

// �ټ���
#ifdef MY_11100200
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�ټ��ֻ����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"BJLRobotSet.ini");
#endif
	// �����°ټ���
#ifdef MY_10301800
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�ټ��ֻ����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"BJLRobotSet.ini");
#endif
	// 16�˰ټ���
#ifdef MY_11100116
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�ټ���(16��)�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"BJLRobotSet.ini");
#endif

	// �ٱ�ţţ
#ifdef MY_11100500
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�ٱ�ţţ�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"BBNNRobotSet.ini");
#endif
	// �ٱ�ţţ(16��)
#ifdef MY_11100416
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�ٱ�ţţ(16��)�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"BBNN(16)RobotSet.ini");
#endif
	// ������
#ifdef MY_10100003
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("���ֶ����������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"HLDDZRobotSet.ini");
#endif
// ����
#ifdef MY_12100106
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("���𻨻����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"ZJHRobotSet.ini");
#endif
// 21��
#ifdef MY_12100404
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("21������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"21DRobotSet.ini");
#endif
	// ţţ
#ifdef MY_12100604
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("ţţ�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"NNRobotSet.ini");
#endif
	// ͨ��ţţ
#ifdef MY_12100706
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("ͨ��ţţ�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"TBNNRobotSet.ini");
#endif
	//ţţ(2��)
#ifdef MY_12100502
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("2��ţţ�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"NN2RobotSet.ini");
#endif
	//�ٱ����
#ifdef MY_11100900
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�ٱ�����ƻ����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"BBEZPRobotSet.ini");
#endif
	//������
#ifdef MY_11100300
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�����������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"LHDRobotSet.ini");
#endif
	//������
#ifdef MY_31000000
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�����������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"PPCRobotSet.ini");
#endif
	//��������
#ifdef MY_31000100
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�������������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"HHSWRobotSet.ini");
#endif
	//�ٱ����(16��)
#ifdef MY_11100816
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("16�˶����ƻ�����(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"16REZPRobotSet.ini");
#endif
	//���(2��)
#ifdef MY_12100202
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("��������ƻ�����(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"2RSHRobotSet.ini");
#endif 
	//�������
#ifdef MY_12100304
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("���4���ƻ�����(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"4RSHRobotSet.ini");
#endif 
	//��������
#ifdef MY_12100804
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("��������������(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"4RSGHRobotSet.ini");
#endif 
	//��������
#ifdef MY_12100906
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("��������������(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"6RSGHRobotSet.ini");
#endif 
	//ͨ������
#ifdef MY_12110906
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("ͨ������������(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"TBSGHRobotSet.ini");
#endif 
	//��ƴ˫��
#ifdef MY_10200102
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("��ƴ˫�ۻ����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"HPSKRobotSet.ini");
#endif
	//28��
#ifdef MY_21100200
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("28G�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"28GRobotSet.ini");
#endif
	//�¿���˹�˿�
#ifdef MY_10100008
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�¿���˹�˿˻����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"DKSSPKRobotSet.ini");
#endif

#ifdef MY_ZW
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("ռλ�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"ZWRobotSet.ini");
#endif
	//���ݶ����齫
#ifdef MY_20100402
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("���ݶ����齫��½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"WZ2MJRobotSet.ini");
#endif
	//�����齫
#ifdef MY_20100004
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�����齫��½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"DZMJRobotSet.ini");
#endif
	//ʮ����
#ifdef MY_12100004
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("BBW13S��½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"13ZRobotSet.ini");
#endif
	//510K
#ifdef MY_10100403
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("510K��½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"510KRobot.ini");
#endif
	//����
#ifdef MY_11100916
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("���������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"HHRobotSet.ini");
#endif
	//�走
#ifdef MY_10200304	
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�走�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"GDRobotSet.ini");
#endif
	//��󸲶��
#ifdef MY_33000006
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("��󸲶������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"JCBYRobotSet.ini");
#endif
	//��ʥ�ֺ�
#ifdef MY_33000106
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("��ʥ�ֺ������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"DSNHRobotSet.ini");
#endif
	//�����칬
#ifdef MY_33000206
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("�����칬�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"DNTGRobotSet.ini");
#endif
	//��������
#ifdef MY_33000308
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("������������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"LKPYRobotSet.ini");
#endif
	//����С��
#ifdef MY_11100600	
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("����С�����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"BRXJRobotSet.ini");
#endif
	//ʮ���
#ifdef MY_12100005
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("ʮ������˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"10DBRobotSet.ini");
#endif
	//���
#ifdef MY_10100204
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("��ɻ����˵�½��(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"MGRobotSet.ini");
#endif
	//�������
#ifdef MY_12100305
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("���5�˻�����(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"5RSHRobotSet.ini");
#endif 

	//����
#ifdef MY_12100006
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("����������(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"CXRobotSet.ini");
#endif 
	//����
#ifdef MY_10200404
	((CWnd*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(TEXT("����������(V2.0)"));
	CINIFile iniFile(CINIFile::GetAppPath()+"SJRobotSet.ini");
#endif 

	CString str = iniFile.GetKeyVal("set", "GameKindID", "1");
	SetDlgItemText(IDC_EDIT1, str.GetBuffer());
	str = iniFile.GetKeyVal("set", "GameNameID", "30341800");
	SetDlgItemText(IDC_EDIT2, str.GetBuffer());
	str = iniFile.GetKeyVal("set", "GameRoomID", "30");
	SetDlgItemText(IDC_EDIT3, str.GetBuffer());
	str = iniFile.GetKeyVal("set", "LogonCount", "200");
	SetDlgItemText(IDC_EDIT5, str.GetBuffer());
	str = iniFile.GetKeyVal("set", "RobotNameHead", "dkl");
	SetDlgItemText(IDC_EDIT4, str.GetBuffer());
	str = iniFile.GetKeyVal("set", "RobotIDBeginAt", "10");
	SetDlgItemText(IDC_EDIT6, str.GetBuffer());
	

	str = iniFile.GetKeyVal("set", "RobotCheckMoneyMix", "10000");
	SetDlgItemText(IDC_EDIT7, str.GetBuffer());

	str = iniFile.GetKeyVal("set", "RobotCheckMoneyMax", "1000000");
	SetDlgItemText(IDC_EDIT8, str.GetBuffer());

	str = iniFile.GetKeyVal("set", "RobotMaxAiMoney", "1000000000");
	SetDlgItemText(IDC_EDIT11, str.GetBuffer());

	str = iniFile.GetKeyVal("set", "RobotLogonTime", "1");
	SetDlgItemText(IDC_EDIT12, str.GetBuffer());

	str = iniFile.GetKeyVal("set", "RobotPeopleSize", "180");
	SetDlgItemText(IDC_EDIT13, str.GetBuffer());

	//������ȡ����  Ĭ����һ�� 
	str = iniFile.GetKeyVal("set", "RobotSaveAndOutMoneyBase", "100");
	SetDlgItemText(IDC_EDIT_BASE, str.GetBuffer());

	GetDlgItem(IDC_EDIT_BASE)->EnableWindow(false);
	GetDlgItem(IDC_STATIC_BASE)->EnableWindow(false);


	auto fgetSetvaule = [&](CString keyname, int id)
	{
		str = iniFile.GetKeyVal("set", keyname, "0");
		::SetDlgItemText(m_hWnd, id, str.GetBuffer());
	};
	
	auto fgetSetTime = [&](int iIndex, const int id[5])
	{
		const CString keyname[5] = {"RobotBeginTimeH%d", "RobotBeginTimeM%d", "RobotEndTimeH%d", "RobotEndTimeM%d", "RobotLogonCount%d"};
		CString _tmp;
		for (int i = 0; i < 5; i++) 
		{
			_tmp.Format(keyname[i], iIndex);
			fgetSetvaule(_tmp, id[i]);
		}
	};

	const int id1[5] = {IDC_EDIT14, IDC_EDIT15, IDC_EDIT16, IDC_EDIT17, IDC_EDIT28};
	const int id2[5] = {IDC_EDIT18, IDC_EDIT19, IDC_EDIT20, IDC_EDIT21, IDC_EDIT29};
	const int id3[5] = {IDC_EDIT22, IDC_EDIT23, IDC_EDIT24, IDC_EDIT25, IDC_EDIT30};
	fgetSetTime(1, id1);
	fgetSetTime(2, id2);
	fgetSetTime(3, id3);
	fgetSetvaule("RobotEndTimeH0", IDC_EDIT26);
	fgetSetvaule("RobotEndTimeM0", IDC_EDIT27);
	fgetSetvaule("RobotLogonCount0", IDC_EDIT5);


		
	TCHAR sz[21];
	wsprintf(sz,"��ǰ��½������[ %d ]",m_iRealLogonCount);
	((CWnd*)GetDlgItem(IDC_STATIC_USERS))->SetWindowTextA(sz);
	wsprintf(sz,"����[ 0 ]");
	((CWnd*)GetDlgItem(IDC_STATIC_ACCOUNTS))->SetWindowTextA(sz);
	wsprintf(sz,"%d",m_iMachineCheckTime);
	((CWnd*)GetDlgItem(IDC_EDIT10))->SetWindowTextA(sz);
	wsprintf(sz,"%d",m_bMachineDeskCount);
	((CWnd*)GetDlgItem(IDC_EDIT9))->SetWindowTextA(sz);


	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(true);
#ifdef MY_ZW

	((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(false);

#else
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(true);

#endif
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(m_bMachineAndPlay);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(m_bAutoDesk);
	((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(false);   //����������ȡ����
	 
	//((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()

	if (((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck())
	{
		((CButton*)GetDlgItem(IDC_CHECK6))->EnableWindow(true);

	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK6))->EnableWindow(false);
	}


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAuto_AIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CAuto_AIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�Զ�ȷ��
void CAuto_AIDlg::OnAutoOK()
{
	m_iAccountsStart = m_iAccountsStartTemp;

	m_iCurrentAccounts = 0;

	m_nInRoomCount=0;
	CString str;
	str.Format("zhtlog:OnAutoOK m_iAccountsStart=%d",m_iAccountsStart);
	OutputDebugString(str);

	SetTimer(TIME_NEXT_ID,1000*m_nAiLogonTime,NULL);
	

}
//ȷ��
void CAuto_AIDlg::OnOK()
{
	if(CheckVersion() == FALSE)
	{
		ShowCue("����Ѿ�����");
		return ;
	}
	UpdateData(false);
	TCHAR sz[200]; 
	
	GetDlgItemText(IDC_EDIT1,sz,sizeof(sz));
	m_iEnterKindID = atoi(sz);
	
	GetDlgItemText(IDC_EDIT2,sz,sizeof(sz)) ;
	m_iEnterNameID = atoi(sz);

	GetDlgItemText(IDC_EDIT3,sz,sizeof(sz));
	m_iEnterRoomID = atoi(sz);
	
	GetDlgItemText(IDC_EDIT4,sz,sizeof(sz));
	lstrcpy(m_szNameFix,sz);
	
	GetDlgItemText(IDC_EDIT5,sz,sizeof(sz));
	m_bCountLogon = atoi(sz);

	GetDlgItemText(IDC_EDIT7,sz,sizeof(sz));
	m_i64CheckMoneyMix = _atoi64(sz);//ȡǮ��С��

	GetDlgItemText(IDC_EDIT8,sz,sizeof(sz));
	m_i64CheckMoneyMax = _atoi64(sz);;  //ȡǮ�����

	GetDlgItemText(IDC_EDIT11,sz,sizeof(sz));
	m_i64MaxAiMoney = _atoi64(sz);; //AI��������Ǯ�����ﵽ������ʹ�Ǯ

	GetDlgItemText(IDC_EDIT_BASE,sz,sizeof(sz));
	m_i64SaveAndOutMoneyBase = _atoi64(sz); //������������ȡǮ�Ļ��� 

	CString dwjlog;
	dwjlog.Format("dwjloglog::m_i64SaveAndOutMoneyBase=%d",m_i64SaveAndOutMoneyBase);
	OutputDebugString(dwjlog);


	//    m_bAiBank;                          //�Ƿ�����˲�������

	GetDlgItemText(IDC_EDIT13,sz,sizeof(sz));
	m_nRoomPeopleSize = _atoi64(sz);;

	GetDlgItemText(IDC_EDIT12,sz,sizeof(sz));
	m_nAiLogonTime = _atoi64(sz);;
//	if(m_bCountLogon > 30)
//		m_bCountLogon = 30;
	GetDlgItemText(IDC_EDIT6,sz,sizeof(sz));
	m_iAccountsStart = atoi(sz);
	m_iAccountsStartTemp = m_iAccountsStart;

	GetDlgItemText(IDC_EDIT10,sz,sizeof(sz));
	m_iMachineCheckTime = atoi(sz);

	GetDlgItemText(IDC_EDIT9,sz,sizeof(sz));
	m_bMachineDeskCount = atoi(sz);


	m_bMachineAndPlay = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();
	m_bAutoDesk		  = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();
	

	m_bAiBank        =  ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();
	m_bOpenBankBase  =  ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();	 //�Ƿ���������������ȡ
	
	m_iCurrentAccounts = 0;
	m_bEnterStyle = 1;
	m_bEnterStyle += (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()? 2 : 0);
	m_bEnterStyle += (((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ? 4 : 0);
	
	if(m_iEnterKindID <= 0|| m_iEnterNameID <= 0 || m_iEnterRoomID <= 0 || m_bCountLogon <= 0|| m_bCountLogon >= 300
		|| strlen(m_szNameFix) > 10 || m_bAutoRegister&&(strlen(m_szNameFix) > 10||strlen(m_szNickFix) > 10))
	{
		ShowCue("���ô���");
		return ;
	}
	if (m_bAiBank && m_bOpenBankBase && (m_i64SaveAndOutMoneyBase % 100 != 0) )
	{
		ShowCue("������ȡ�������ô���");
		return ;
	}
	
	m_bUseNew = true;
	m_nInRoomCount=0;
	if (!m_bUseNew)
	{
		SetTimer(TIME_NEXT_ID,1000*m_nAiLogonTime,NULL);

		CString ad=CINIFile::GetAppPath ();
		CINIFile f( ad + "bzgame.ini");
		int static iConnect=1;
		CString m_key = "BZW";

		BOOL  bAutoLogin = f.GetKeyVal(m_key,"AutoLogin",1);//�Զ�����ʱ��

		if (bAutoLogin)
		{
			int   nAutoLoginTime = f.GetKeyVal(m_key,"AutoLoginTime",30);
			SetTimer(TIME_AUTO_LOGIN,1000*60*nAutoLoginTime,NULL);
		}
	}
	else
	{
		InitRobot_New();
	}
		//SetTimer(TIME_LOGIN, 1000*m_nAiLogonTime, NULL);

	//OnConnectToCenterServer();
}

//ȡ��
void CAuto_AIDlg::OnCancel()
{
	OnBnClickedClose();
}

//��ʾ��ʾ
void CAuto_AIDlg::ShowCue(TCHAR *pMessage)
{
	SetDlgItemText(IDC_STATIC_CUE,pMessage);
	WriteStr(pMessage);
}

//��ʾ��ʾ
void CAuto_AIDlg::ShowCue(CString pMessage)
{
	SetDlgItemText(IDC_STATIC_CUE,pMessage);
	WriteStr(pMessage);
}


//�������ķ�����
void CAuto_AIDlg::OnConnectToCenterServer()
{
	CString ad=CINIFile::GetAppPath ();
	CINIFile f( ad + "bzgame.ini");
	int static iConnect=1;
	CString s;
	s.Format ("%d",iConnect);
	CString m_key = "BZW";
	m_CenterServerPort = CENTER_SERVER_PORT;
	m_CenterServerIPAddr = f.GetKeyVal(m_key,"LoginIP"+s,"127.0.0.1");//������IP
	//if(iConnect > 5|| m_CenterServerIPAddr == "") 
	//{
	//	wsprintf(m_ErrorMsg,"%s %s %d,%d,%s","�������ķ��������ɹ�",m_CenterServerIPAddr,m_CenterServerPort,iConnect,m_key);
	//	ShowCue(m_ErrorMsg);
	//	return;
	//}

	if(m_TCPSocket->Connect(m_CenterServerIPAddr,m_CenterServerPort)==false)
		this->OnConnectToCenterServer();

	iConnect = 0;
}

//���ӵ�½������
void CAuto_AIDlg::OnConnectToLogonServer()
{
	int static iConnect = 1;
	//m_TCPSocket->CloseSocket(false);
	CString s;
	s.Format("xxxxx OnConnectToLogonServer: IP = %s  Port = %d", m_CenterServerIPAddr,m_CenterServerPort);
	OutputDebugString(s);
	if(m_TCPSocket->Connect(m_CenterServerIPAddr,m_CenterServerPort)==false && iConnect ++ < 10)
	{
		this->OnConnectToLogonServer();
	}
	else if(iConnect > 10)
	{
		ShowCue("�������Z������ʧ�ܣ������������ַ�Ƿ���ȷ");
	}
	else
	{
		SetTimer(TIME_CHECKZCONNECT_TIMEOUT, 5 * 1000, NULL);
		iConnect = 1;
	}	
}


//�ӿں��� 
//����������Ϣ����
int CAuto_AIDlg::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_TCPSocket->SendData(bMainID,bAssistantID,bHandleCode);//����������
}

//����������Ϣ����
int CAuto_AIDlg::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_TCPSocket->SendData(pData,uBufLen,bMainID,bAssistantID,bHandleCode);
}

//�����ȡ��Ϣ
bool CAuto_AIDlg::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	wsprintf(m_ErrorMsg,"CAuto_AIDlg::OnSocketReadEvent bMainID = %d,bAssistantID = %d",pNetHead->bMainID,pNetHead->bAssistantID );
	WriteStr(m_ErrorMsg,1);
	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:	//������Ϣ 
		{
			return OnConnectMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}

	case MDM_GP_REQURE_GAME_PARA:	//���ķ���ȫ�ֲ���//������Ϸȫ�ֲ���
		{
			CenterServerMsg * msg=(CenterServerMsg *)pNetData;
			wsprintf(m_ErrorMsg,"uDataSize = %d,sizeof() = %d",uDataSize,sizeof(CenterServerMsg));
			WriteStr(m_ErrorMsg);
			if(msg)//�õ���½��������ַ
			{
				m_CenterServerIPAddr = msg->m_strMainserverIPAddr;
				m_CenterServerPort = msg->m_iMainserverPort;
			}

			m_TCPSocket->CloseSocket (true);		

			return true;
		}
	case MDM_GP_LOGON:	//��½��Ϣ
		{
			return OnLogonMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GP_LIST:	//��Ϸ�б�
		{
			return OnListMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GP_MESSAGE://�������Ϣ ���ţ�С����
		{
			return true;
		}
	case MDM_GP_GET_CONTEST_ROOMID:
		{
			if (!EnterContestRoom(pNetHead,pNetData,uDataSize,pClientSocket))
			{
				//���뷿��ʧ�ܣ����ö�ʱ��������ٵ�
				CString s;
				s.Format("robot---- %d ������µ�½", m_PlaceUserInfo.dwUserID);
				OutputDebugString(s);
				KillTimer(TIME_LOGIN);
				SetTimer(TIME_LOGIN, (m_nAiLogonTime+15)*1000, NULL);
			}
			return true;
		}
	}
	return true;
}

//����������Ϣ
bool CAuto_AIDlg::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	CString s;
	CString s1[3] = {"BServer", "δ֪", "ZServer"};
	s.Format("xxxxxx OnSocketConnect %s", s1[m_bConnectToWitchServer]);
	OutputDebugString(s);

	if(m_bConnectToWitchServer == 0)//���ķ�����
	{
		KillTimer(TIME_CHECKBCONNECT_TIMEOUT);
		m_bConnectToWitchServer = 1;
	}
	else if (m_bConnectToWitchServer == 2)
	{
		KillTimer(TIME_CHECKZCONNECT_TIMEOUT);
	}

	if(uErrorCode != 0)
	{
		ShowCue("OnSocketConnectEvent Error!");
		OnConnectToCenterServer();
	}
	return true;
}

//����ر���Ϣ
bool CAuto_AIDlg::OnSocketCloseEvent()
{

	CString s;
	CString s1[3] = {"δ֪", "BServer", "ZServer"};
	s.Format("xxxxxx OnSocketClose %s", s1[m_bConnectToWitchServer]);
	OutputDebugString(s);


	if(m_bConnectToWitchServer == 2)
	{
		/*m_nInRoomCount--;
		CString s;
		s.Format("�ڷ�������: %d",m_nInRoomCount);
		ShowCue(s);*/
		//GetDlgItem(IDC_ONROOM)->SetWindowText(s);
		if (m_bUseNew)
		{
			//LogonRobot_New();
			m_bConnectToWitchServer = 0;//����
		}
		else
			ReConnectToCenterServer();
		return true;
	}	
	
	if(m_bConnectToWitchServer == 1)//���ķ�����
	{
		m_bConnectToWitchServer = 2;

		OnConnectToLogonServer();
	}
	return true;
}

//��������
bool CAuto_AIDlg::ReConnectToCenterServer()
{

	m_bConnectToWitchServer = 0;//����
	
	m_TCPSocket->CloseSocket(true);

	//OnConnectToCenterServer();
	SetTimer(TIME_NEXT_ID,1000*m_nAiLogonTime,NULL);
	return true;
}

//������Ϣ����
bool CAuto_AIDlg::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_NET_TEST)	//���������Ϣ
	{
		pClientSocket->SendData(MDM_CONNECT,ASS_NET_TEST,0);//������Ϣ����
		return true;
	}
	else if ((pNetHead->bAssistantID==2)||(pNetHead->bAssistantID == 3))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * _p = (MSG_S_ConnectSuccess *)pNetData;
		if (_p != NULL)
		{
			pClientSocket->SetCheckCode(_p->i64CheckCode, SECRET_KEY);
		}

		if(m_bConnectToWitchServer == 1)//���ķ�����
		{
			m_TCPSocket->SendData(NULL,0,MDM_GP_REQURE_GAME_PARA,0,0);
			return true;
		}

		if (m_bUseNew)
			SendUserInfo_New();
		else
			SendUserInfo();
		return true;
	}
	return false;
}
//��ȡ��������غ��� zxj 2009-11-12 ��������
CString CAuto_AIDlg::innerGetMac()
{
	CString retMac="";
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter=NULL;
	DWORD dwRetVal=0;
	pAdapterInfo=(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen=sizeof(IP_ADAPTER_INFO);
	if(GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS)
	{
		free(pAdapterInfo);
		pAdapterInfo=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}
	if((dwRetVal=GetAdaptersInfo(pAdapterInfo,&ulOutBufLen))==NO_ERROR)
	{
		pAdapter=pAdapterInfo;
		CString temp;
		while(pAdapter)
		{
			if(pAdapter->Type==6)//pAdapter->Description�а���"PCI"Ϊ����������//pAdapter->Type��71Ϊ����������
			{
				for(UINT i=0;i<pAdapter->AddressLength;i++)
				{
					temp.Format("%02X",pAdapter->Address[i]);
					retMac+=temp;
					//	printf("%02X%c",pAdapter->Address[i],i==pAdapter->AddressLength-1?��n��:��-��);
				}
				break;
			}
			pAdapter=pAdapter->Next;
		}
	}
	if(pAdapterInfo)
		free(pAdapterInfo);
	return retMac;
}

//��ȡ��������غ��� zxj 2009-11-12 ��������
CString CAuto_AIDlg::coreGetCode()
{
	CString strRet="";
	CString str=innerGetMac(),s;
	if(str=="")
		return "δ��⵽ϵͳ����Ethernet�豸��";

	int r=0;
	int l=str.GetLength();
	for(int i=0;i<l;i+=2)
	{
		int r1=0,r2=0;
		s=str.Mid(i,1);
		if(s>="0" && s<="9")
			r1=atoi(s);
		if(s=="A" || s=="a")	r1=10;
		if(s=="B" || s=="b")	r1=11;
		if(s=="C" || s=="c")	r1=12;
		if(s=="D" || s=="d")	r1=13;
		if(s=="E" || s=="e")	r1=14;
		if(s=="F" || s=="f")	r1=15;
		s=str.Mid(i+1,1);
		if(s>="0" && s<="9")
			r2=atoi(s);
		if(s=="A" || s=="a")	r2=10;
		if(s=="B" || s=="b")	r2=11;
		if(s=="C" || s=="c")	r2=12;
		if(s=="D" || s=="d")	r2=13;
		if(s=="E" || s=="e")	r2=14;
		if(s=="F" || s=="f")	r2=15;

		CString t;
		r+=r1*16+r2;
		srand(r);
		t.Format("%s%04X%s-",str.Mid(l-i-1,1),rand(),str.Mid(l-i-2,1));
		strRet+=t;
	}
	if(strRet.Right(1)=="-")strRet=strRet.Left(strRet.GetLength()-1);
	return strRet;
}
//���������Ϣ
BOOL CAuto_AIDlg::SendUserInfo()
{
	if (PlayerOnLine(m_CurrentLogonName))
	{
		CString str;
		str.Format("zhtlog:%s�ظ�����",m_CurrentLogonName);
		OutputDebugString(str);
		ReConnectToCenterServer();
		return FALSE;
	}
	int rNum=0;
	CString TML_SN="";
	CString allword="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	for (int i = 0;i <= 120; i ++)
	{
		rNum =(rand() * 1000)% 62;
		TML_SN+=allword[rNum];
	}

	CString ad=CINIFile::GetAppPath ();
	CINIFile f( ad + "AIMD5Pass.ini");
    CString key = TEXT("MD5pass");
	CString MatchineCode= coreGetCode();	//ȡ�ñ��������� zxj 2009-11-12 ��������
	if(m_bAutoRegister)//�Զ�ע���
	{
		//���û�ע��
		MSG_GP_S_RegisterStruct UserRegister;
		::memset(&UserRegister,0,sizeof(UserRegister));
		UserRegister.uRoomVer = GAME_PLACE_MAX_VER;
		UserRegister.bBoy = (m_bNowRoomIndex % 2);
		UserRegister.bLogoID =( m_bNowRoomIndex % 10);
		//lstrcpy(UserRegister.szRegSQL,CLongonDialog::m_szRegSQL);
		lstrcpy(UserRegister.szName,m_CurrentLogonName);

		CString pass = f.GetKeyVal(key,"szMD5Pass","3acd0be86de7dcccdbf91b20f94a68cea535922d");//����������

		//CString pass = f.GetKeyVal(key,"szMD5Pass","d0970714757783e6cf17b26fb8e2298f");//����������

		wsprintf(UserRegister.szMD5Pass,"%s",pass);

		lstrcpy(UserRegister.nickName,m_CurrentNickName);

		m_TCPSocket->SendData(&UserRegister,sizeof(UserRegister),MDM_GP_LOGON,ASS_GP_LOGON_REG,0);

	}else
	{

	MSG_GP_S_LogonByNameStruct LogonByName;///����һ���ṹ
	::memset(&LogonByName,0,sizeof(LogonByName));
	LogonByName.uRoomVer=GAME_PLACE_MAX_VER;//�ṹ�İ汾
	lstrcpy(LogonByName.TML_SN,TML_SN);
	lstrcpy(LogonByName.szName,m_CurrentLogonName);//�ṹ���û���
	//lstrcpy(LogonByName.szMD5Pass,"d0970714757783e6cf17b26fb8e2298f");//�ṹ������
	CString pass = f.GetKeyVal(key,"szMD5Pass","3acd0be86de7dcccdbf91b20f94a68cea535922d");
	//���������� = f.GetKeyVal(key,"szMD5Pass","d0970714757783e6cf17b26fb8e2298f");//����������
	wsprintf(LogonByName.szMD5Pass,"%s",pass);
	lstrcpy(LogonByName.szMathineCode, MatchineCode);	//�����������ѱ��������봫�ݵ���¼���ݰ��� zxj 2009-11-12
	LogonByName.gsqPs = 5471;
	m_TCPSocket->SendData(&LogonByName,sizeof(LogonByName),MDM_GP_LOGON,ASS_GP_LOGON_BY_NAME,0);
	}
	return true;
}

//��½��Ϣ����
bool CAuto_AIDlg::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
//	m_nInRoomCount++;
//	CString s;
//	s.Format("�ڷ�������: %d",m_nInRoomCount);
//	GetDlgItem(IDC_ONROOM)->SetWindowText(s);
//	ShowCue(s);
	if (pNetHead->bAssistantID==ASS_GP_LOGON_SUCCESS)		//��½�ɹ�
	{



		//������Ϣ
		m_PlaceUserInfo = *((MSG_GP_R_LogonResult *)pNetData);

		CString ad = CINIFile::GetAppPath ();
		CINIFile f( ad + "AIMD5Pass.ini");
		CString key = TEXT("MD5pass");
		CString pass = f.GetKeyVal(key,"szMD5Pass","3acd0be86de7dcccdbf91b20f94a68cea535922d");
		wsprintf(m_PlaceUserInfo.szMD5Pass,"%s",pass);


		CString s;
		s.Format("robot---- %s ��¼ OnLogonMessage", m_PlaceUserInfo.szName);
		OutputDebugString(s);

		//��ȡ��Ϸ�б�
		return GetGameRoomList();
	}
	else if (pNetHead->bAssistantID==ASS_GP_LOGON_ERROR)	//��½ʧ��
	{


		//��ʾ������Ϣ
		CString strMessage;
		switch (pNetHead->bHandleCode)
		{
		case ERR_GP_USER_NO_FIND:
			{
				strMessage=TEXT("�ܱ�Ǹ�������ʺţ�");
				break;
			}

		case ERR_GP_USER_PASS_ERROR:
			{
				strMessage=TEXT("�ܱ�Ǹ���������");
				break;
			}
		case ERR_GP_USER_VALIDATA:
			{
				strMessage=TEXT("���ʺ��ѱ���ֹ��¼��");
				break;
			}
		case ERR_GP_USER_IP_LIMITED:
			{
				strMessage=TEXT("�����ڵ� IP ��ַ����ֹ��½��");
				break;
			}
		case ERR_GP_USER_EXIST:
			{
				strMessage=TEXT("�ܱ�Ǹ������û����Ѿ�����ע���ˣ�");
				break;
			}
		case ERR_GP_PASS_LIMITED:
			{
				strMessage=TEXT("������������");
				break;
			}
		case ERR_GP_IP_NO_ORDER:
			{
				strMessage=TEXT("����Ҫ��ָ���� IP ��ַ��ϵͳ��½��");
				break;
			}
		case ERR_GP_USER_LOGON:
			{
				strMessage=TEXT("���ʺ��Ѿ���¼��");
				break;
			}
		case ERR_GP_USERNICK_EXIST:
			{
				strMessage=TEXT("���ǳ��Ѿ����ڣ�");
				break;
			}		
		default:strMessage=TEXT("ϵͳ��¼���ִ����������Ա��ϵ��");
		}


		wsprintf(m_ErrorMsg,"%s%d %s", m_szNameFix, m_Robots[m_iCurLogonIndex].iIndex, strMessage);
		//wsprintf(m_ErrorMsg,"%s %s",m_CurrentLogonName,strMessage);
		ShowCue(m_ErrorMsg);
		//��������
		


		CString s;
		s.Format("xxxxx %s%d %s", m_szNameFix, m_Robots[m_iCurLogonIndex].iIndex, strMessage);
		OutputDebugString(s);
		//��������

		pClientSocket->CloseSocket(false);

		if (m_bUseNew)
			LogonRobot_New();
		else
			ReConnectToCenterServer();
		return true;
	}
	return true;
}

//��ȡ��Ϸ�б�
bool CAuto_AIDlg::GetGameRoomList()
{
	m_TCPSocket->SendData(MDM_GP_LIST,ASS_GP_LIST_KIND,0);//������Ϸ�б�
	return true;
}

//�б���Ϣ
bool CAuto_AIDlg::OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GP_LIST_KIND:		//��ȡ������Ϸ����
		{
			wsprintf(m_ErrorMsg,"ASS_GP_LIST_KIND uDataSize = %d,sizeof() = %d",uDataSize,sizeof(ComKindInfo));
			WriteStr(m_ErrorMsg,1);
			//��������
			m_GameList.HandleKindData((ComKindInfo *)pNetData,uDataSize/sizeof(ComKindInfo));//�����ϼ�����Ϸ����
			return true;
		}
	case ASS_GP_LIST_NAME:		//��ȡ����Ϸ����
		{
			wsprintf(m_ErrorMsg,"ASS_GP_LIST_NAME uDataSize = %d,sizeof() = %d",uDataSize,sizeof(ComNameInfo));
			WriteStr(m_ErrorMsg,1);
			//��������
			m_GameList.HandleNameData((ComNameInfo *)pNetData,uDataSize/sizeof(ComNameInfo));

			//������ʾ
			if (pNetHead->bHandleCode==ERR_GP_LIST_FINISH) //������Ϸ�����б������־
			{
				MSG_GP_SR_GetRoomStruct GetList;
				GetList.uKindID = m_iEnterKindID;
				GetList.uNameID = m_iEnterNameID;
				m_TCPSocket->SendData(&GetList,sizeof(GetList),MDM_GP_LIST,ASS_GP_LIST_ROOM,0);
			}
			return true;
		}
	case ASS_GP_LIST_ROOM:		//��ȡ����Ϸ����
		{
			//Ч������ 
			if (uDataSize<sizeof(MSG_GP_SR_GetRoomStruct))
				return false;
			MSG_GP_SR_GetRoomStruct * pNetRoomBuf=(MSG_GP_SR_GetRoomStruct *)pNetData;
			TCHAR sz[200];
			wsprintf(sz," sizeof(ComRoomInfo) = %d",sizeof(ComRoomInfo));
			WriteStr(sz);
			//��������
			m_GameList.HandleRoomData((ComRoomInfo *)((char *)pNetData+sizeof(MSG_GP_SR_GetRoomStruct)),
				(uDataSize-sizeof(MSG_GP_SR_GetRoomStruct))/sizeof(ComRoomInfo),pNetRoomBuf->uKindID,pNetRoomBuf->uNameID);

			if (pNetHead->bHandleCode == ERR_GP_LIST_FINISH) //������Ϸ�б������־
			{
				//���ӱ��������ж�
				CAFCRoomItem *pGameRoomItem = m_GameList.FindRoomItem(m_iEnterRoomID);
				if (NULL == pGameRoomItem)
					return true;
				if (pGameRoomItem->m_RoomInfo.dwRoomRule & GRR_CONTEST)
				{OutputDebugString("robot----------��ʼ���������ȡ����ID");
					GetContestRoomID(m_iEnterRoomID);
				}
				else if (m_bUseNew)
				{
					ConnectToW_New(m_iEnterRoomID);
				}
				else
				{
					EnterRoom(m_iEnterRoomID);
				}
			}
			return true;
		}
	case ASS_GP_LIST_COUNT:		//��������
		{
			return true;
		}
	}
	return TRUE;
}

//��ʱ��
void CAuto_AIDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIME_NEXT_ID:
		{
			CString str1;
			str1.Format("zhtlog:CGameRoomEx::m_nOnLinePeople=%d",CGameRoomEx::m_nOnLinePeople);
			OutputDebugString(str1);
			KillTimer(nIDEvent);
			if(m_bNowRoomIndex >= MAX_GAME_ROOM)
			{
				ShowCue("�ͻ�������Ա");
				m_TCPSocket->CloseSocket (true);
				break;
			}
			if (m_nRoomPeopleSize <= CGameRoomEx::m_nOnLinePeople)
			{
				ShowCue("���������Ѿ��ﵽ�趨����");
				m_TCPSocket->CloseSocket (true);
				break; 
			
			}
			if(m_iCurrentAccounts >= m_bCountLogon)
			{
				TCHAR sz[20];
				wsprintf(sz,"����[%d]",m_iAccountsStart + m_iCurrentAccounts);
				//wsprintf(sz,"����[ %d ]",m_bNowRoomIndex);
				((CWnd*)GetDlgItem(IDC_STATIC_ACCOUNTS))->SetWindowTextA(sz);
				//wsprintf(sz,"%d",m_bNowRoomIndex);
				wsprintf(sz,"%d",m_iAccountsStart + m_iCurrentAccounts);
				((CWnd*)GetDlgItem(IDC_EDIT6))->SetWindowTextA(sz);
				ShowCue("�����Ѿ����!");
				m_TCPSocket->CloseSocket (true);
				break;
			}
			wsprintf(m_CurrentLogonName,"%s%d",m_szNameFix,/*1001 +*/ m_iAccountsStart + m_iCurrentAccounts);
			wsprintf(m_CurrentNickName,"%s%d",m_szNickFix,/*1001 +*/ m_iAccountsStart + m_iCurrentAccounts);
			m_iCurrentAccounts++;
			OnConnectToCenterServer();
			CString str;
			str.Format("zhtlog:m_CurrentLogonName%s���� =%d ",m_CurrentLogonName,CGameRoomEx::m_nOnLinePeople);
			OutputDebugString(str);
			break;
		}
	case TIME_AUTO_LOGIN:
		{
			//KillTimer(nIDEvent);
			if (!m_bUseNew)
				OnAutoOK();
			break;
		}
	case TIME_LOGIN:
		{
			//KillTimer(nIDEvent);
			OnTimeLogon_New();
			break;
			//Set
		}
	case TIME_CHECKRELOGIN:
		{
			ReLogonRobot_New();
			break;
		}
	case TIME_CHECKLOGIN:
		{
			CheckLogonRobot_New();
			break;
		}
	case TIME_CHECKRESOCKETERROR:
		{
			CheckSocketError_New();
			break;
		}
	case TIME_CHECKBCONNECT_TIMEOUT:
		{
			KillTimer(TIME_CHECKBCONNECT_TIMEOUT);
			OutputDebugString("xxxxxx ����BServerʧ���ص�");
			ShowCue("����BServer��ʱ");

			if (m_TCPSocket != NULL)
			{
				delete m_TCPSocket;
				m_TCPSocket = new CTCPClientSocket(this);
			}
			ConnectToB_New();
			
			break;
		}
	case TIME_CHECKZCONNECT_TIMEOUT:
		{
			KillTimer(TIME_CHECKZCONNECT_TIMEOUT);
			OutputDebugString("xxxxxx ����ZServerʧ���ص�");
			ShowCue("����ZServer��ʱ");

			if (m_TCPSocket != NULL)
			{
				delete m_TCPSocket;
				m_TCPSocket = new CTCPClientSocket(this);
			}

			OnConnectToLogonServer();
			break;
		}
	}
	return __super::OnTimer(nIDEvent);
}

//���뷿��
bool CAuto_AIDlg::EnterRoom(int roomid)
{
	CAFCRoomItem *pRoomItem = m_GameList.FindRoomItem(roomid);
	if(!pRoomItem)
	{
		TCHAR sz[100];
		wsprintf(sz, _T("�Ҳ������� %d"), roomid);
		ShowCue(sz);
		return false;
	}

	CString str;
	str.Format("%s",pRoomItem->m_RoomInfo.szGameRoomName);
	SetDlgItemText(IDC_STATIC_ROOMNAME, str.GetBuffer());
	SetWindowTextA(str);
	return OnCentranceGameRoom(pRoomItem);
}

//������Ϸ����
bool CAuto_AIDlg::OnCentranceGameRoom(CAFCRoomItem * pGameRoomItem)
{
	//�ж��Ƿ����ⷿ��
	ComRoomInfo * pComRoomInfo=&pGameRoomItem->m_RoomInfo;
	if (((pComRoomInfo->uComType!=TY_NORMAL_GAME)&&(pComRoomInfo->uComType!=TY_MATCH_GAME)
		&&(pComRoomInfo->uComType!=TY_MONEY_GAME)))
	{
		ShowCue("�������������µĹ���ʵ��");
		m_TCPSocket->CloseSocket (true);
		AfxGetApp( )->m_pMainWnd->DestroyWindow ();
		return true;		
	}	
	//�ж���Ϸ�Ƿ�װ��Ч��汾
	TCHAR szProessName[31],szGameName[61];
	m_GameList.GetGameName(pGameRoomItem,szGameName,sizeof(szGameName));
	m_GameList.GetProcessName(pGameRoomItem,szProessName,sizeof(szProessName));

	if (m_bNowRoomIndex >= MAX_GAME_ROOM - 1)
	{
		return true;
	}


	//������Ϸ����

	int iNowSelect = -1;
	for (int i=0; i<m_bNowRoomIndex; i++)
	{
		if (m_RoomInfo[i].pGameRoomWnd == NULL)
		{
			iNowSelect = i;
			break;
		}
	}
	RoomInfoStruct * pRoomInfoItem;
	if (iNowSelect != -1)
	{
		pRoomInfoItem=&m_RoomInfo[iNowSelect];
	}
	else
		pRoomInfoItem=&m_RoomInfo[m_bNowRoomIndex++];
	try
	{
		pRoomInfoItem->bAccess=true;
		pRoomInfoItem->stComRoomInfo=*pComRoomInfo;
		lstrcpy(pRoomInfoItem->szProcessName,szProessName);

		if (pRoomInfoItem->pGameRoomWnd != NULL)
		{
			delete pRoomInfoItem->pGameRoomWnd;
			pRoomInfoItem->pGameRoomWnd = NULL;
		}
		pRoomInfoItem->pGameRoomWnd=new CGameRoomEx(szGameName,pRoomInfoItem,&m_PlaceUserInfo,m_bEnterStyle
			, m_bMachineAndPlay, m_iMachineCheckTime, m_bMachineDeskCount,m_bAutoDesk);

		CGameRoomEx * pGameRoom = (CGameRoomEx *)pRoomInfoItem->pGameRoomWnd;
		m_pGameRoom=(CGameRoomEx *)pRoomInfoItem->pGameRoomWnd;
		pGameRoom->SetBankInfo(m_bAiBank, m_bOpenBankBase, m_i64MaxAiMoney,m_i64CheckMoneyMax,m_i64CheckMoneyMix, m_i64SaveAndOutMoneyBase);


		pGameRoom->SetExitTime(m_Robots[m_iCurLogonIndex].pTimeRanges->tEnd);


		if (pRoomInfoItem->pGameRoomWnd->Create(IDD_GAME_ROOM,this)==FALSE) 
			throw TEXT("����A�����ʧ��");
	}
	catch (...)
	{
		//������Ϣ
		delete pRoomInfoItem->pGameRoomWnd;
		memset(pRoomInfoItem,0,sizeof(RoomInfoStruct));
		ShowCue("����A�����ʧ��");
		return false; 
	}
	TCHAR sz[MAX_PATH]={0};
	m_iRealLogonCount++;
	wsprintf(sz,"��ǰ��½������[ %d ]",m_iRealLogonCount);
	((CWnd*)GetDlgItem(IDC_STATIC_USERS))->SetWindowTextA(sz);

	wsprintf(m_ErrorMsg,"%s �ɹ���½",m_CurrentLogonName);
	ShowCue(m_ErrorMsg);
	m_TCPSocket->CloseSocket (true);
	return true;
}
//�رշ�������
LRESULT CAuto_AIDlg::OnCloseGameRoom(WPARAM wparam, LPARAM lparam)
{
	RoomInfoStruct * pRoomInfo=(RoomInfoStruct *)wparam;
	CString str;
	if ((pRoomInfo!=NULL)&&(pRoomInfo->bAccess)&&(pRoomInfo->pGameRoomWnd!=NULL)&&(IsWindow(pRoomInfo->pGameRoomWnd->GetSafeHwnd())))
	{
		
		for (int i = 0; i < m_iSumLogonCount; i++)
		{
			if (m_Robots[i].iUserID == lparam)
			{
				m_Robots[i].pRoomItem = NULL;
			}
		}
		if (pRoomInfo->stComRoomInfo.iContestID > 0)
		{
			//��������
			KillTimer(TIME_LOGIN);
			SetTimer(TIME_LOGIN, (m_nAiLogonTime+15)*1000, NULL);
		}
		delete pRoomInfo->pGameRoomWnd;
		pRoomInfo->pGameRoomWnd = NULL;
		memset(pRoomInfo,0,sizeof(RoomInfoStruct));
		
		if (m_iRealLogonCount > 0)
			m_iRealLogonCount--;

		TCHAR sz[MAX_PATH]={0};
		wsprintf(sz,"��ǰ��½������[ %d ]",m_iRealLogonCount);
		((CWnd*)GetDlgItem(IDC_STATIC_USERS))->SetWindowTextA(sz);

	}
	return 0;
}

bool CAuto_AIDlg::EnterContestRoom(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	TCHAR sz[MAX_PATH]={0};
	if (ERR_GP_GET_CONTEST_ROOMID == pNetHead->bAssistantID)
	{
		OutputDebugString("robot---------�����Ѿ���Ա");

		wsprintf(sz,"��Ϸ������������");
		((CWnd*)GetDlgItem(IDC_STATIC_USERS))->SetWindowTextA(sz);

		wsprintf(m_ErrorMsg,"��Ϸ������������");
		ShowCue(m_ErrorMsg);

		return false;
	}
	CAFCRoomItem *pRoomItem = m_GameList.FindRoomItem(m_iEnterRoomID);
	DL_GetContestRoomID *pContestRoom = (DL_GetContestRoomID*)pNetData;
	if (NULL == pContestRoom || NULL == pRoomItem)
	{
		OutputDebugString("robot---------1");
		return false;
	}
	if (!pRoomItem->m_bIsContest)
	{
		OutputDebugString("robot---------2");
		return false;
	}
	
	bool bEnterFlag = false;
	if (pContestRoom->iGameID == pRoomItem->m_RoomInfo.uNameID &&	pContestRoom->iContestID == pRoomItem->m_RoomInfo.iContestID)
	{//�ж��ڷ������ݵ�����ˣ�����˷����������ʱ���ڣ��û��Ƿ����л�����		
		RoomInfoStruct *pRoomInfoItem = &m_RoomInfo[0];					
		for (int i=0; i!=pRoomItem->m_RoomContest.size(); ++i)
		{
			if (pRoomItem->m_RoomContest[i].uRoomID == pContestRoom->iRoomID)
			{
				bEnterFlag = true;
				pRoomItem->m_iRoomIndex = i;
				pRoomItem->m_RoomInfo = pRoomItem->m_RoomContest[i];							
				if (m_bUseNew)
				{
					CString str;
					str.Format("robot------------��ȡ�ķ���ID=%d", pContestRoom->iRoomID);
					OutputDebugString(str);
					ConnectToW_New(pContestRoom->iRoomID);
				}
				else
					EnterRoom(pContestRoom->iRoomID);
				break;
			}
		}
		if (!bEnterFlag)
		{
			OutputDebugString("robot---------���������ʧ�ܣ������Ƿ��ѵ�¼�������䣡");

			wsprintf(sz,"���������ʧ�ܣ������Ƿ��ѵ�¼�������䣡");
			((CWnd*)GetDlgItem(IDC_STATIC_USERS))->SetWindowTextA(sz);

			wsprintf(m_ErrorMsg,"���������ʧ�ܣ������Ƿ��ѵ�¼�������䣡");
			ShowCue(m_ErrorMsg);
		}		
	}
	return bEnterFlag;
}
//
BOOL CAuto_AIDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CBaseRoom::PreTranslateMessage(pMsg);
}

//�汾�˶�
BOOL CAuto_AIDlg::CheckVersion()
{
	return TRUE;
	CString sn ="20081231";////��ֹ����
	
	long in=atol(sn.GetBuffer (sn.GetLength ()));
	if(in<=0)
		return FALSE;

	int y1=atoi(sn.Mid (0,4)),
		m1   =atoi(sn.Mid (4,2)),
		d1=atoi(sn.Mid (6,2));

	CTime t1(y1,m1,d1,0,0,0);
	time_t ct;  
    time( &ct ) ;

	CTime t2(ct);

	CTimeSpan tsp;
	tsp=t2-t1;//// ��ǰ����  - ��ֹ����

	LONGLONG dd=tsp.GetDays ();

	if(t2 > t1)//dd > 0)
	{
		return FALSE;
	}
	return TRUE;
}


int CAuto_AIDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseRoom::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CMenu m_Menu;
	m_Menu.CreateMenu();
	CMenu m_PopupMenu;
	m_PopupMenu.CreateMenu();
	m_PopupMenu.AppendMenuA(MF_STRING,IDC_UPDATE_HISTORY,TEXT("������ʷ"));
	m_Menu.AppendMenuA(MF_POPUP,(UINT_PTR)m_PopupMenu.GetSafeHmenu(),"����");

	//SetMenu(&m_Menu);
	return 0;
}

BOOL CAuto_AIDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(LOWORD(wParam))
	{
	case IDC_UPDATE_HISTORY:
		CShowHistory dlg;
		if(IDOK == dlg.DoModal())
		{
			return true;
		}
		return true;
	}
	return CBaseRoom::OnCommand(wParam, lParam);
}


void CAuto_AIDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������\
	//add by 20090211zht�޸Ĺر�ʱ�Ƚ�������Ϸ�еĻ������˳�
	for (int i=0; i<m_bNowRoomIndex; i++)
	{
		if (m_RoomInfo[i].pGameRoomWnd != NULL)
			::SendMessage(m_RoomInfo[i].pGameRoomWnd->GetSafeHwnd(),IDM_CLOSE,0,0);
	}
	//add by 20090211zht�޸Ĺر�ʱ�Ƚ�������Ϸ�еĻ������˳�
	PostQuitMessage(0);
}

void CAuto_AIDlg::OnBnClickedMix()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//PostMessage(IDM,0,0);
	::SendMessage(GetSafeHwnd(),WM_SYSCOMMAND,SC_MINIMIZE,0);
}

void CAuto_AIDlg::OnLButtonDown(UINT nFlags,CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}
//ĳ���ʺŵ�½ʧ��20081128
LRESULT CAuto_AIDlg::OnLogonFail(WPARAM wparam, LPARAM lparam)
{
	//m_iRealLogonCount--;
	
	TCHAR sz[MAX_PATH]={0};
	wsprintf(sz,"��ǰ��½������[ %d ]",m_iRealLogonCount);//20081128
	((CWnd*)GetDlgItem(IDC_STATIC_USERS))->SetWindowTextA(sz);
	return 0;
}

void CAuto_AIDlg::OnLButtonDblClk(UINT nFlags, CPoint point)


{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnBnClickedMix();
	CBaseRoom::OnLButtonDblClk(nFlags, point);
}
LRESULT CAuto_AIDlg::OnUpdateMoney(WPARAM wparam, LPARAM lparam)
{
	/*CString str;
	str.Format("%I64d",CGameRoomEx::m_i64BeginMoney);
	SetDlgItemText(IDC_EDIT12, str.GetBuffer());

	str.Format("%I64d(%I64d)",CGameRoomEx::m_i64NowMoney,CGameRoomEx::m_i64NowMoney-CGameRoomEx::m_i64BeginMoney);
	SetDlgItemText(IDC_EDIT13, str.GetBuffer());*/
	if (m_nRoomPeopleSize > CGameRoomEx::m_nOnLinePeople)
	{
		if (!m_bUseNew)
			OnAutoOK();
	}
	return 0;

}
///����Ƿ����ߣ����߾Ͳ��ص�
BOOL CAuto_AIDlg::PlayerOnLine(TCHAR szCurrentName[])
{

	for (int i=0; i<m_bNowRoomIndex; i++)
	{
		if (m_RoomInfo[i].pGameRoomWnd != NULL)
		{
	
			
			if (0 == strcmp((dynamic_cast<CGameRoomEx *>(m_RoomInfo[i].pGameRoomWnd))->m_pLogonInfo.szName,szCurrentName))
			{
				return TRUE;
			}
		}

	}

	char CurentName[61]={0};

	lstrcpy(CurentName,szCurrentName);
	//std::map<std::string,int>::iterator iter =  g_ErrorUser.find(CurentName);
	//if(iter != g_ErrorUser.end())
	//{
	//	return TRUE;
	//}
	return FALSE;
	
}
void CAuto_AIDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR sz[200]; 
	TCHAR sz1[200]; 
	GetDlgItemText(IDC_EDIT14,sz,sizeof(sz));
	int robotID =atoi(sz);//ȡǮ��С��
	GetDlgItemText(IDC_EDIT15,sz1,sizeof(sz1));
	__int64 robotmoney=_atoi64(sz1);
	if(m_pGameRoom)
	{
		m_pGameRoom->CheckEditMoney(robotmoney,robotID);
	}
}

void CAuto_AIDlg::LogonRobot_New()
{
	m_bConnectToWitchServer = 0;//����
	m_bLogoning = true;
	KillTimer(TIME_LOGIN);
	SetTimer(TIME_LOGIN, m_nAiLogonTime * 1000, NULL);
	m_bExistLogonTimer = true;
}

void CAuto_AIDlg::InitRobot_New()
{
	m_TimeRanges.clear();
	CTime _t = CTime::GetCurrentTime();
	

	int t_h1, t_h2, t_m1, t_m2, l_count;

	auto fgetint = [&](int id)->int
	{ 
		TCHAR sz[200];
		::GetDlgItemText(m_hWnd, id, sz, sizeof(sz));
		return atoi(sz);
	};

	auto fgetrange = [&](const int id[5])
	{
		t_h1 = fgetint(id[0]);
		t_m1 = fgetint(id[1]);
		t_h2 = fgetint(id[2]);
		t_m2 = fgetint(id[3]);
		l_count = fgetint(id[4]);
		m_TimeRanges.push_back(TTimeRange(TTime_day(t_h1, t_m1),  TTime_day(t_h2, t_m2), l_count));
	};

	t_h2 = fgetint(IDC_EDIT26);
	t_m2 = fgetint(IDC_EDIT27);
	l_count = fgetint(IDC_EDIT5);
	

	const int id1[5] = {IDC_EDIT14, IDC_EDIT15, IDC_EDIT16, IDC_EDIT17, IDC_EDIT28};
	const int id2[5] = {IDC_EDIT18, IDC_EDIT19, IDC_EDIT20, IDC_EDIT21, IDC_EDIT29};
	const int id3[5] = {IDC_EDIT22, IDC_EDIT23, IDC_EDIT24, IDC_EDIT25, IDC_EDIT30};
	m_TimeRanges.push_back(TTimeRange(TTime_day(0, 0),  TTime_day(t_h2, t_m2), l_count));
	fgetrange(id1);
	fgetrange(id2);
	fgetrange(id3);

	//m_TimeRanges.push_back(TTimeRange(TTime_day(_t.GetHour(), _t.GetMinute()),  TTime_day(_t.GetHour(), _t.GetMinute() + 3), 10));
	//m_TimeRanges.push_back(TTimeRange(TTime_day(_t.GetHour(), _t.GetMinute() + 3),  TTime_day(_t.GetHour(), _t.GetMinute() + 8), 11));
	//m_TimeRanges.push_back(TTimeRange(TTime_day(_t.GetHour(), _t.GetMinute() + 5),  TTime_day(_t.GetHour(), _t.GetMinute() + 10), 12));
	//m_TimeRanges.push_back(TTimeRange(TTime_day(_t.GetHour(), _t.GetMinute() + 7),  TTime_day(_t.GetHour(), _t.GetMinute() + 10), 13));


	for (int i = 1; i < m_TimeRanges.size(); i++)
	{
		if (!(m_TimeRanges[i].tBegin.GetValid() && 
			m_TimeRanges[i].tEnd.GetValid() && 
			CmpTime_dat(m_TimeRanges[i].tBegin, m_TimeRanges[i].tEnd) < 0))
		{
			CString s;
			s.Format("ʱ��%d���÷�Χ������", i);
			AfxMessageBox(s, MB_OK);
			return;
		}
	}


	m_iSumLogonCount = 0;
	m_EventList.ResetContent();
	m_EventTimes.clear();
	TTime_Envet te;
	for (int i = 0; i < m_TimeRanges.size(); i++)
	{
		for (int j = 0; j < m_TimeRanges[i].iCount; j++)
		{	
			int _index = m_iSumLogonCount;
			m_Robots[_index].iIndex = _index + m_iAccountsStart /*+ 1001*/;
			m_Robots[_index].pTimeRanges = &m_TimeRanges[i];
			if (m_Robots[_index].pRoomItem != NULL)
			{
				CGameRoomEx * pGameRoom = (CGameRoomEx *)m_Robots[_index].pRoomItem->pGameRoomWnd;
				if (pGameRoom)
					pGameRoom->SetExitTime(m_TimeRanges[i].tEnd);
				else
					m_Robots[_index].pRoomItem = NULL;
			}
			m_TimeRanges[i].m_Robots.push_back(&m_Robots[_index]);
			m_iSumLogonCount++;
		}
		//te.iHour = m_TimeRanges[i].tBegin.iHour;
		//te.iHour = m_TimeRanges[i].tBegin.iMinute;
		//te.iFlag = 0;
		//te.iCount = m_TimeRanges[i].iCount;
		//m_EventTimes.push_back(te);

		//te.iHour = m_TimeRanges[i].tBegin.iHour;
		//te.iHour = m_TimeRanges[i].tBegin.iMinute;
		//te.iFlag = 0;
		//te.iCount = m_TimeRanges[i].iCount;
		//m_EventTimes.push_back(te);
		auto f1 = [&](const TTime_day& t)
		{
			te.iHour = t.iHour;
			te.iMinute = t.iMinute;
		};
		auto f2 = [&](int flag)
		{
			te.iCount = m_TimeRanges[i].iCount;
			te.iFlag = flag;
		};
		f1(m_TimeRanges[i].tBegin);
		f2(0);
		m_EventTimes.push_back(te);
		f1(m_TimeRanges[i].tEnd);
		f2(1);
		m_EventTimes.push_back(te);
	}

	auto cmpf = [](const TTime_Envet& t1, const TTime_Envet& t2)
	{
		return (t1.iHour * 60 + t1.iMinute - (t2.iHour * 60 + t2.iMinute)) < 0;
	};
	sort(m_EventTimes.begin(), m_EventTimes.end(), cmpf);
	for (int i = 0; i < m_EventTimes.size(); i++)
	{
		m_EventList.AddString(m_EventTimes[i].GetStr());
	}

	m_bLogoning = false;
	m_iCurLogonIndex = -1;
	m_bExistLogonTimer = false;
	LogonRobot_New();
	StartRun_New();
}


void CAuto_AIDlg::OnTimeLogon_New()
{
	if (m_nRoomPeopleSize <= CGameRoomEx::m_nOnLinePeople)
	{
		ShowCue("���������Ѿ��ﵽ�趨����");
		m_TCPSocket->CloseSocket(true);
		return;
	}

	m_iCurLogonIndex = FindNextLogonIndex(m_iCurLogonIndex);
	if (m_iCurLogonIndex != -1)
	{
		KillTimer(TIME_LOGIN);
		m_bExistLogonTimer = false;
		if (m_TCPSocket != NULL)
		{
			delete m_TCPSocket;
			m_TCPSocket = new CTCPClientSocket(this);
		}

		CString s;
		s.Format("robot %s%d ��¼ OnTimeLogon_New", m_szNameFix, m_Robots[m_iCurLogonIndex].iIndex);
		OutputDebugString(s);

		ConnectToB_New();
	}
	else
	{
		KillTimer(TIME_LOGIN);
		m_bExistLogonTimer = false;
		m_bLogoning = false;
	}
}

void CAuto_AIDlg::OnLogon_New()
{

}
void CAuto_AIDlg::OnGameList_New()
{

}

void CAuto_AIDlg::SendUserInfo_New()
{
	CString LogonName, NickName;
	LogonName.Format("%s%d", m_szNameFix, m_Robots[m_iCurLogonIndex].iIndex);
	NickName.Format("%s%d", m_szNickFix, m_Robots[m_iCurLogonIndex].iIndex);


	int rNum=0;
	CString TML_SN="";
	CString allword="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	for (int i = 0;i <= 120; i ++)
	{
		rNum =(rand() * 1000)% 62;
		TML_SN+=allword[rNum];
	}

	CString ad=CINIFile::GetAppPath ();
	CINIFile f( ad + "AIMD5Pass.ini");
	CString key = TEXT("MD5pass");
	CString MatchineCode= coreGetCode();	//ȡ�ñ��������� zxj 2009-11-12 ��������
	if(m_bAutoRegister)//�Զ�ע���
	{
		//���û�ע��
		MSG_GP_S_RegisterStruct UserRegister;
		::memset(&UserRegister,0,sizeof(UserRegister));
		UserRegister.uRoomVer = GAME_PLACE_MAX_VER;
		UserRegister.bBoy = (m_bNowRoomIndex % 2);
		UserRegister.bLogoID =( m_bNowRoomIndex % 10);
		lstrcpy(UserRegister.szName, m_CurrentLogonName);

		CString pass = f.GetKeyVal(key,"szMD5Pass","3acd0be86de7dcccdbf91b20f94a68cea535922d");//����������
		wsprintf(UserRegister.szMD5Pass,"%s",pass);
		lstrcpy(UserRegister.nickName,NickName.GetBuffer());

		m_TCPSocket->SendData(&UserRegister,sizeof(UserRegister),MDM_GP_LOGON,ASS_GP_LOGON_REG,0);

	}
	else
	{

		MSG_GP_S_LogonByNameStruct LogonByName;///����һ���ṹ
		::memset(&LogonByName,0,sizeof(LogonByName));
		LogonByName.uRoomVer=GAME_PLACE_MAX_VER;//�ṹ�İ汾
		lstrcpy(LogonByName.TML_SN,TML_SN);
		lstrcpy(LogonByName.szName, LogonName);//�ṹ���û���
		CString pass = f.GetKeyVal(key,"szMD5Pass","3acd0be86de7dcccdbf91b20f94a68cea535922d");
		wsprintf(LogonByName.szMD5Pass,"%s",pass);
		lstrcpy(LogonByName.szMathineCode, MatchineCode);	//�����������ѱ��������봫�ݵ���¼���ݰ��� zxj 2009-11-12
		LogonByName.gsqPs = 5471;
		m_TCPSocket->SendData(&LogonByName,sizeof(LogonByName),MDM_GP_LOGON,ASS_GP_LOGON_BY_NAME,0);


		CString s;
		s.Format("xxxxx %s ��¼ SendUserInfo_New", LogonByName.szName);
		OutputDebugString(s);

	}
}

void CAuto_AIDlg::ConnectToB_New()
{
	CString ad=CINIFile::GetAppPath ();
	CINIFile f( ad + "bzgame.ini");
	int static iConnect=1;
	CString s;
	s.Format ("%d",iConnect);
	CString m_key = "BZW";
	m_CenterServerPort = CENTER_SERVER_PORT;
	m_CenterServerIPAddr = f.GetKeyVal(m_key,"LoginIP"+s,"127.0.0.1");//������IP

	s.Format("xxxxxx ConnectToB %s %d", m_CenterServerIPAddr,m_CenterServerPort);
	OutputDebugString(s);

	if(m_TCPSocket->Connect(m_CenterServerIPAddr,m_CenterServerPort)==false)
	{
		this->ConnectToB_New();
	}
	else
	{
		SetTimer(TIME_CHECKBCONNECT_TIMEOUT, 5 * 1000, NULL);
	}
}
void CAuto_AIDlg::ConnectToZ_New()
{

}
//��ȡ���ʵı�������ID
void CAuto_AIDlg::GetContestRoomID(int roomid)
{
	CAFCRoomItem *pGameRoomItem = m_GameList.FindRoomItem(roomid);
	if (!(pGameRoomItem->m_RoomInfo.dwRoomRule & GRR_CONTEST))
	{
		return;
	}
	MSG_GP_GetContestRoomID getContestRoomID;
	getContestRoomID.iUserID = m_PlaceUserInfo.dwUserID;
	getContestRoomID.iGameID = pGameRoomItem->m_RoomInfo.uNameID;
	getContestRoomID.iContestID = pGameRoomItem->m_RoomContest[0].iContestID;
	getContestRoomID.iUpPeople = pGameRoomItem->m_RoomInfo.iUpPeople;
	m_TCPSocket->SendData(&getContestRoomID, sizeof(MSG_GP_GetContestRoomID), MDM_GP_GET_CONTEST_ROOMID, 0, 0);
	OutputDebugString("robot-----------���ͷ����������");
}

void CAuto_AIDlg::ConnectToW_New(int roomid)
{
	CAFCRoomItem *pGameRoomItem = m_GameList.FindRoomItem(roomid);

	if (pGameRoomItem == NULL) 
	{
		TCHAR sz[100];
		wsprintf(sz, _T("�Ҳ������� %d"), roomid);
		ShowCue(sz);

		CString s;
		s.Format("xxxxx %s �Ҳ�������", m_PlaceUserInfo.szName);
		OutputDebugString(s);

		return;
	}
	
	CString str;
	str.Format("%s",pGameRoomItem->m_RoomInfo.szGameRoomName);
	SetDlgItemText(IDC_STATIC_ROOMNAME, str.GetBuffer());
	SetWindowTextA(str);

	//�ж��Ƿ����ⷿ��
	ComRoomInfo * pComRoomInfo=&pGameRoomItem->m_RoomInfo;
	if (((pComRoomInfo->uComType!=TY_NORMAL_GAME)&&(pComRoomInfo->uComType!=TY_MATCH_GAME)
		&&(pComRoomInfo->uComType!=TY_MONEY_GAME)))
	{
		ShowCue("�������������µĹ���ʵ��");
		m_TCPSocket->CloseSocket (true);
		AfxGetApp( )->m_pMainWnd->DestroyWindow ();
		return;		
	}	
	//�ж���Ϸ�Ƿ�װ��Ч��汾
	TCHAR szProessName[31],szGameName[61];
	m_GameList.GetGameName(pGameRoomItem,szGameName,sizeof(szGameName));
	m_GameList.GetProcessName(pGameRoomItem,szProessName,sizeof(szProessName));


	//������Ϸ����
	int iNowSelect = -1;
	for (int i=0; i< MAX_GAME_ROOM; i++)
	{
		if (m_RoomInfo[i].pGameRoomWnd == NULL)
		{
			iNowSelect = i;
			break;
		}
	}
	RoomInfoStruct * pRoomInfoItem = NULL;
	if (iNowSelect != -1)
	{
		pRoomInfoItem=&m_RoomInfo[iNowSelect];
	}

	try
	{
		pRoomInfoItem->bAccess=true;
		pRoomInfoItem->stComRoomInfo=*pComRoomInfo;
		lstrcpy(pRoomInfoItem->szProcessName,szProessName);

		if (pRoomInfoItem->pGameRoomWnd != NULL)
		{
			delete pRoomInfoItem->pGameRoomWnd;
			pRoomInfoItem->pGameRoomWnd = NULL;
		}
		pRoomInfoItem->pGameRoomWnd=new CGameRoomEx(szGameName,pRoomInfoItem,&m_PlaceUserInfo,m_bEnterStyle
			, m_bMachineAndPlay, m_iMachineCheckTime, m_bMachineDeskCount,m_bAutoDesk);

		CGameRoomEx * pGameRoom = (CGameRoomEx *)pRoomInfoItem->pGameRoomWnd;
		m_pGameRoom=(CGameRoomEx *)pRoomInfoItem->pGameRoomWnd;
		pGameRoom->SetExitTime(m_Robots[m_iCurLogonIndex].pTimeRanges->tEnd);

		pGameRoom->SetBankInfo(m_bAiBank, m_bOpenBankBase, m_i64MaxAiMoney,m_i64CheckMoneyMax,m_i64CheckMoneyMix, m_i64SaveAndOutMoneyBase);

		m_Robots[m_iCurLogonIndex].pRoomItem = pRoomInfoItem;
		m_Robots[m_iCurLogonIndex].iUserID = m_PlaceUserInfo.dwUserID;


		CString s;
		s.Format("xxxxx %s ��¼ ConnectToW_New", m_PlaceUserInfo.szName);
		OutputDebugString(s);

		if (pRoomInfoItem->pGameRoomWnd->Create(IDD_GAME_ROOM,this)==FALSE) 
			throw TEXT("����A�����ʧ��");
	}
	catch (...)
	{
		//������Ϣ
		delete pRoomInfoItem->pGameRoomWnd;
		memset(pRoomInfoItem,0,sizeof(RoomInfoStruct));
		ShowCue("����A�����ʧ��");
		return; 
	}
	TCHAR sz[MAX_PATH]={0};
	m_iRealLogonCount++;
	wsprintf(sz,"��ǰ��½������[ %d ]",m_iRealLogonCount);
	((CWnd*)GetDlgItem(IDC_STATIC_USERS))->SetWindowTextA(sz);

	wsprintf(m_ErrorMsg,"%s%d �ɹ���½", m_szNameFix, m_Robots[m_iCurLogonIndex].iIndex);
	ShowCue(m_ErrorMsg);

	CString s;
	s.Format("xxxxx %s ��¼�ɹ� ConnectToW_New", m_PlaceUserInfo.szName);
	OutputDebugString(s);

	LogonRobot_New();
	//m_TCPSocket->CloseSocket (true);
	return;
}

void CAuto_AIDlg::StartRun_New()
{
	KillTimer(TIME_CHECKRELOGIN);
	KillTimer(TIME_CHECKLOGIN);
	KillTimer(TIME_CHECKRESOCKETERROR);
	SetTimer(TIME_CHECKRELOGIN, 1000 * 60 * 1, NULL);
	SetTimer(TIME_CHECKLOGIN, 1000 * 2, NULL);
	SetTimer(TIME_CHECKRESOCKETERROR, 1000 * 2, NULL);
}

void CAuto_AIDlg::ReLogonRobot_New()
{
	CString str;
	str.Format("robot--------------ReLogonRobot_New-------�Ƿ����ڵ�¼:%d", m_bLogoning?1:0);
	OutputDebugString(str);
	if (!m_bLogoning)
	{
		OutputDebugString("xxxxx ReLogonRobot_New-------------------------");
		m_bLogoning = true;
		m_iCurLogonIndex = -1;
		KillTimer(TIME_LOGIN);
		SetTimer(TIME_LOGIN, m_nAiLogonTime * 1000, NULL);
		m_bExistLogonTimer = true;
	}
}

void CAuto_AIDlg::CheckLogonRobot_New()
{
	for (int i = 0; i < m_TimeRanges.size(); i++)
	{
		if (m_TimeRanges[i].GetValid() && m_TimeRanges[i].iLogoned)
			m_TimeRanges[i].iLogoned = false;
		if (m_TimeRanges[i].GetValid() && !m_TimeRanges[i].iLogoned && !m_bLogoning)
		{
			/*CString s;
			s.Format("xxxxx CheckLogonRobot_New %d (%s--%s)-----------------------------", i, m_TimeRanges[i].tBegin.getTimeStr(), m_TimeRanges[i].tEnd.getTimeStr());
			OutputDebugString(s);*/

			m_TimeRanges[i].iLogoned = true;
			m_bLogoning = true;
			m_iCurLogonIndex = -1;
			KillTimer(TIME_LOGIN);
			SetTimer(TIME_LOGIN, m_nAiLogonTime * 1000, NULL);
			m_bExistLogonTimer = true;
		}
	}
}

void CAuto_AIDlg::CheckSocketError_New()
{
	if (m_bLogoning && m_TCPSocket->GetSocketState() == NO_CONNECT)
	{
		OutputDebugString("xxxxx CheckSocketError_New------------------------------------");
		LogonRobot_New();
	}
}

int	CAuto_AIDlg::FindNextLogonIndex(int iIndex)
{
	for (int i = iIndex + 1; i < m_iSumLogonCount; i++)
	{
		if (m_Robots[i].pRoomItem == NULL && m_Robots[i].pTimeRanges->GetValid())
		{
			return i;
		}
	}
	return -1;
}

void CAuto_AIDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAuto_AIDlg::OnBnClickedCheck6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck())
	{
		((CButton*)GetDlgItem(IDC_CHECK7))->EnableWindow(true);
		if (((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck())
		{
			GetDlgItem(IDC_EDIT_BASE)->EnableWindow(true);//IDC_STATIC_BASE
			GetDlgItem(IDC_STATIC_BASE)->EnableWindow(true);
		}
		else
		{
			GetDlgItem(IDC_EDIT_BASE)->EnableWindow(false);//IDC_STATIC_BASE
			GetDlgItem(IDC_STATIC_BASE)->EnableWindow(false);
		}
		

	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK7))->EnableWindow(false);
		GetDlgItem(IDC_EDIT_BASE)->EnableWindow(false);
		GetDlgItem(IDC_STATIC_BASE)->EnableWindow(false);

	}
}


void CAuto_AIDlg::OnBnClickedCheck7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck())
	{
		GetDlgItem(IDC_EDIT_BASE)->EnableWindow(true);//IDC_STATIC_BASE
		GetDlgItem(IDC_STATIC_BASE)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_EDIT_BASE)->EnableWindow(false);//IDC_STATIC_BASE
		GetDlgItem(IDC_STATIC_BASE)->EnableWindow(false);
	}
}
