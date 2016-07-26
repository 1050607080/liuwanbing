// DlgRoomFee.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRoomFee.h"


#define   TIME_ENTERROOM			29

// CDlgRoomFee dialog

IMPLEMENT_DYNAMIC(CDlgRoomFee, CDialog)

CDlgRoomFee::CDlgRoomFee(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRoomFee::IDD, pParent)
{
	m_pGameRoom = (CGameRoomEx *)pParent;
	m_dwCount = TIME_ENTERROOM;
	int m_iLeft=0;
	int m_iTop=0;
	int m_iRight=0;
	int m_iBottom=0;
	bool m_bMouseInRect=false;
}

CDlgRoomFee::~CDlgRoomFee()
{
}

void CDlgRoomFee::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROOM_INFO, m_RoomInfo);
	DDX_Control(pDX, IDOK, m_btOK);
}


BEGIN_MESSAGE_MAP(CDlgRoomFee, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgRoomFee::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgRoomFee::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgRoomFee message handlers

void CDlgRoomFee::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		char TimeInfo[100] = {0};
		wsprintf(TimeInfo,"ȷ��(%d)",m_dwCount--);
		SetDlgItemText(IDOK,TimeInfo);
		m_btOK.SetWindowText(TimeInfo);
		if (m_dwCount == 0)
		{
			KillTimer(1);
			OnBnClickedOk();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgRoomFee::OnBnClickedOk()
{
	m_dwCount = TIME_ENTERROOM;
	if((((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck()==BST_CHECKED))
	{
		m_pGameRoom->m_bIsAsk = false;
	}
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CDlgRoomFee::OnInitDialog()
{
	SetWindowText("��ʾ�Ի���");
	char TimeInfo[100] = {0};
	SetTimer(1,1000,NULL);

    // ��ʾ��ߴ粻��
    CRect rect;
    GetDlgItem(IDC_ROOM_INFO)->GetWindowRect(&rect);
	ScreenToClient(&rect);
    rect.bottom += 20;
    GetDlgItem(IDC_ROOM_INFO)->MoveWindow(rect.left, rect.top+10, rect.Width(), rect.Height());

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str = fMsg.GetKeyVal("GameRoom","FeeDlgText","�ڱ���Ϸ���䣬ÿ�ֽ���������%d��ң�ף�������죡");

	char RoomInfo[200] = {0};
	wsprintf(RoomInfo,str.GetBuffer(),m_pGameRoom->m_GameTax);
	SetDlgItemText(IDC_ROOM_INFO,RoomInfo);
	
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin); 
	TCHAR path[MAX_PATH];
	CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());;

	//*-------��ʼ�� m_configDlg--------- 
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CDlgRoomFeeConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	//-----------------------------------------------------*/

	CRect ClientRect;
	GetClientRect(&ClientRect);
	CGameImage bt;
	m_btOK.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE,CRect(0,0,0,0),this,IDOK);
	wsprintf(pathConfig,"%sdialog\\RoomFeeBtOK.bmp",skinfolder);
	m_btOK.LoadButtonBitmap(pathConfig,false);
	bt.SetLoadInfo(pathConfig,CGameImageLink::m_bAutoLock);
	m_btOK.MoveWindow((ClientRect.right-ClientRect.left)/2-bt.GetWidth()/8, 50+61, bt.GetWidth()/4, bt.GetHeight());

	m_btCancle.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BUTTON1);
	wsprintf(pathConfig,"%sdialog\\log_exit_bt.bmp",skinfolder);
	m_btCancle.LoadButtonBitmap(pathConfig,false);//RoomFeeClose.bmp
	bt.SetLoadInfo(pathConfig,CGameImageLink::m_bAutoLock);//RoomFeeClose.bmp
	m_btCancle.MoveWindow(ClientRect.right-ClientRect.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());

	wsprintf(TimeInfo,"ȷ��(%d)",TIME_ENTERROOM+1);
	SetDlgItemText(IDOK,TimeInfo);
		
	SetWndRgn();// ���ô���Բ��
	return true;
}

void CDlgRoomFee::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CRect rect;
	TCHAR szBuffer[200];
	WCHAR wszBuffer[200];

	CWnd *pOKWnd = GetDlgItem(IDOK);
	pOKWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	int iImageHight = rect.Height();
	dc.SetBkMode(TRANSPARENT);

	CString		SkinFolder;
	CString		key = Glb().m_skin;

	CString		s = CINIFile::GetAppPath ();/////����·��
	CINIFile	f( s + m_skinmgr.GetSkinBcfFileName());
	SkinFolder	= f.GetKeyVal(key, "skinfolder", "image\\client\\skin0\\");
	/*
	// 2012.12.17 yyf ����
	CGameImageHelper helprb(&m_backPic);
	helprb.BitBlt(dc.GetSafeHdc(),0,0);
	*/
	//�滭��ܵ�ͼ 2012.10.15 yyf
	m_configDlg.DrawViewFrame(&dc);

	//��ȡ�����ҳͼƬ
	wsprintf(szBuffer,"%s%sdialog\\Getmoney.bmp",s, SkinFolder);
	MultiByteToWideChar(CP_ACP,MB_COMPOSITE,szBuffer,-1,wszBuffer,sizeof(wszBuffer));

	Graphics graphics(dc.GetSafeHdc()); // Create a GDI+ graphics object

#ifndef BZ_ZLIB
	Image image(wszBuffer); // Construct an image
#else
	CUnZipRes unZipRes;
	IStream * pStream=NULL;
	pStream=unZipRes.LoadFileResFromZip(szBuffer);
	Image image(pStream);
	if(pStream!=NULL)
	{
		pStream->Release();
		pStream=NULL;
	}
#endif
	int w=image.GetWidth();
	int h=image.GetHeight();
	iImageHight += h;

	m_iLeft = rect.left + rect.Width()/2 - w/2 - 139+23;
	m_iTop = rect.top - h - 5 - 21;
	m_iRight = m_iLeft + w;
	m_iBottom = m_iTop + h;
	//graphics.DrawImage(&image, m_iLeft, m_iTop, w,h); // 2012.12.17 yyf ����
}

void CDlgRoomFee::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bMouseInRect = FALSE;
	if(point.x >= m_iLeft && point.x <= m_iRight && point.y >= m_iTop && point.y <= m_iBottom)
	{
		HCURSOR hCursor; 
		hCursor = AfxGetApp()->LoadCursor(IDC_HAND); 
		// �������ΪС��״ 
		SetCursor(hCursor); 
		m_bMouseInRect = TRUE;
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgRoomFee::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
	if(TRUE == m_bMouseInRect)
	{
		CString s=CBcfFile::GetAppPath();
		CBcfFile fini(s+"..\\bzgame.bcf");
		CString url=fini.GetKeyVal("BZW","getMoneyUrl","http://l92.168.0.235:8011/manage/pay");
		
		ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE);
	}

	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgRoomFee::OnBnClickedButton1()
{
	OnCancel();
	// TODO: Add your control notification handler code here
}

HBRUSH CDlgRoomFee::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH   hbr   =   CDialog::OnCtlColor(pDC,   pWnd,   nCtlColor); 

	if(pWnd-> GetDlgCtrlID()   ==   IDC_ROOM_INFO) 
	{ 
		pDC-> SetBkMode(TRANSPARENT); 
		pDC-> SetTextColor(RGB(0,0,0));

		return     HBRUSH(GetStockObject(HOLLOW_BRUSH)); 
	} 
	return   hbr; 
}

// ���ô���Բ�� 2012.10.15 yyf
void CDlgRoomFee::SetWndRgn(void)
{
	POINT m_arPoints[8];
	CRect winRect;
	GetWindowRect(&winRect);
	ScreenToClient(&winRect);
 
    //���Ͻ�
	m_arPoints[0].x = winRect.left;
	m_arPoints[0].y = winRect.top+3;
	m_arPoints[1].x = winRect.left+3;
	m_arPoints[1].y = winRect.top;

	//���Ͻ�
	m_arPoints[2].x = winRect.right-3;
	m_arPoints[2].y = winRect.top;
	m_arPoints[3].x = winRect.right;
	m_arPoints[3].y = winRect.top+3;

	//���½�
	m_arPoints[4].x = winRect.right;
	m_arPoints[4].y = winRect.bottom-4;
	m_arPoints[5].x = winRect.right-4;
	m_arPoints[5].y = winRect.bottom;

	//���½�
	m_arPoints[6].x = winRect.left+4;
	m_arPoints[6].y = winRect.bottom;
	m_arPoints[7].x = winRect.left;
	m_arPoints[7].y = winRect.bottom-4;

	HRGN hRgn = CreatePolygonRgn(m_arPoints, 8, WINDING);
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}
}