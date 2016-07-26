/// lookinfo.cpp : ʵ���ļ�
///

#include "stdafx.h"
#include "lookinfo.h"

void MsgPrint(const char *lpszFormat, ...);


/// Clookinfo �Ի���

IMPLEMENT_DYNAMIC(Clookinfo, CDialog)
///���ƣ�Clookinfo
///������·�ӶԻ����캯��������ͼƬ����ʼ������
///@param 
///@return 
Clookinfo::Clookinfo(CWnd* pParent /*=NULL*/): CDialog(Clookinfo::IDD, pParent)
{
	// ��ע�ı�����
	TCHAR path[200];
	TCHAR skin[MAX_NAME_INFO];
	wsprintf(path,TEXT(".\\image\\luzi\\luzi_bk.bmp"));	//���뿴·�ӱ���ͼ
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);


//////////////////////////////////////////////////////////

	wsprintf(path,TEXT(".\\image\\luzi\\zhuang_dot.bmp"));	// ׯСͼ
	m_imgzhuangduizi.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\xian_dot.bmp"));	// ��Сͼ
	m_imgxianduizi.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\he_dot.bmp"));		// ��Сͼ
	m_imgheduizi.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	
	wsprintf(path,TEXT(".\\image\\luzi\\zhuang1_dot.bmp"));	// ׯСͼ
	m_imgzhuang1_dot.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\xian1_dot.bmp"));	// ��Сͼ
	m_imgxian1_dot.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\he1_dot.bmp"));		// ��Сͼ
	m_imghe1_dot.SetLoadInfo(path,CGameImageLink::m_bAutoLock);


	//////////////////////////////////////////////

	wsprintf(path,TEXT(".\\image\\luzi\\zhuang.bmp"));	// ׯСͼ
	m_imgzhuang.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\zhuang1.bmp"));	// ׯ��ͼ
	m_imgzhuang1.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\xian.bmp"));	// ��Сͼ
	m_imgxian.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\xian1.bmp"));	// �д�ͼ
	m_imgxian1.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\he.bmp"));		// ��Сͼ
	m_imghe.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\luzi\\he1.bmp"));		// �ʹ�ͼ
	m_imghe1.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		
	m_BtnYiye;				// ��һҳ
	m_BtnErye;				// �ڶ�ҳͼƬ

	m_iZwin =0 ;//ׯ��Ӯ�İٷֱȣ�Ϊ������������Ŵ�100����������ǰ����Ϊ��С�������
	m_iXwin =0 ;//�м�Ӯ�İٷֱ�
	m_iHwin =0 ;//��Ӯ�İٷֱ�
	m_iZDuizi =0 ;//ׯ����
	m_iXDuizi =0 ;//�ж���
	m_iZTianwan =0 ;//ׯ����
	m_iXTianwang =0 ;//������
	m_iPlayCount=0;

#ifdef SET72 

	m_pDaTu.SetPoint(49,216);//��ͼ����λ��
	m_pXiaoTu.SetPoint(17,93);//Сͼ����λ��

	m_iDaWidth = 32;//��ͼ����
	m_iXiaoWidth = 16;//Сͼ����

#else
	m_pDaTu.SetPoint(31,198);//��ͼ����λ��
	m_pXiaoTu.SetPoint(32,95);//Сͼ����λ��

	m_iDaWidth = 31;//��ͼ����
	m_iXiaoWidth = 16;//Сͼ����

#endif

	///�Ƿ���ʾ��һҳ��ͼƬ
	m_bShowNext = false;
	m_iNowJuShu = 0;	//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�

	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + SKIN_FOLDER + _T(".bcf"));
	CString strKey = TEXT("config");
	CPoint ptPoint = CPoint(0,0);
	TCHAR szValueName[_MAX_PATH] = {_T("GamePictureType")};
	m_iGamePictureType = f.GetKeyVal(strKey, szValueName, 0);

	m_iLuziCol = 0;
}

Clookinfo::~Clookinfo()
{
}

void Clookinfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDOK, m_BmCancel);
	//DDX_Control(pDX, IDCANCEL, m_BmOk);
	DDX_Control(pDX, IDC_BUT_YIYE, m_BtnYiye);
	DDX_Control(pDX, IDC_BUT_ERYE, m_BtnErye);
	DDX_Control(pDX, IDC_BUT_CLOSE, m_BtnClose);
}


BEGIN_MESSAGE_MAP(Clookinfo, CDialog)
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDOK, &Clookinfo::OnBnClickedOk)
	//ON_BN_CLICKED(IDCANCEL, &Clookinfo::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
//	ON_WM_WINDOWPOSCHANGED()
ON_WM_WINDOWPOSCHANGING()
ON_WM_WINDOWPOSCHANGED()
ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDC_BUT_YIYE, &Clookinfo::OnBnClickedButYiye)
ON_BN_CLICKED(IDC_BUT_ERYE, &Clookinfo::OnBnClickedButErye)
ON_BN_CLICKED(IDC_BUT_CLOSE, &Clookinfo::OnBnClickedButClose)
ON_WM_CREATE()
END_MESSAGE_MAP()

///���ƣ�OnInitDialog
///��������ʼ�����������ð�ť����״���ƶ���ťλ��
///@param 
///@return 
BOOL Clookinfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CGameImageHelper ImageHandle(&m_bk);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,RGB(255,0,255),RGB(1,0,1));

	// ���ô�����״
	if (hRgn!=NULL)
	{
	//	SetWindowPos(NULL,0,0,ImageHandle.GetWidth(),ImageHandle.GetHeight(),SWP_NOSIZE);
		SetWindowRgn(hRgn,TRUE);
	}

	CRect ClientRect;
	GetClientRect(&ClientRect);

	CGameImage m_tempbk;
	HRGN hRgn2;
	TCHAR szName[MAX_PATH]={0};
	CRect rect;

	TCHAR skin[MAX_NAME_INFO];

	wsprintf(szName,".\\image\\luzi\\diyiye.bmp");	//��һҳ��ť
	m_BtnYiye.LoadButtonBitmap(szName,false);	
	m_tempbk.SetLoadInfo(szName,true);
	hRgn2=AFCBmpToRgn(m_tempbk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
		m_BtnYiye.SetWindowRgn(hRgn2,true);

	wsprintf(szName,".\\image\\luzi\\dierye.bmp");	//�ڶ�ҳ��ť
	m_BtnErye.LoadButtonBitmap(szName,false);	
	m_tempbk.SetLoadInfo(szName,true);
	hRgn2=AFCBmpToRgn(m_tempbk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
		m_BtnErye.SetWindowRgn(hRgn2,true);
	
	wsprintf(szName,".\\image\\luzi\\game_close_bt.bmp");	//�رհ�ť
	m_BtnClose.LoadButtonBitmap(szName,false);	
	m_tempbk.SetLoadInfo(szName,true);
	hRgn2=AFCBmpToRgn(m_tempbk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
		m_BtnClose.SetWindowRgn(hRgn2,true);

	this->MapWindowPoints(this->GetParent(),&ClientRect);
	CGameImageHelper	help(&m_bk);
	this->MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());
	CenterWindow();	

	m_BtnYiye.GetClientRect(&rect);
	m_BtnYiye.MoveWindow(m_pXiaoTu.x,m_pXiaoTu.y-rect.Height()-2,rect.Width(),rect.Height());
	m_BtnYiye.ShowWindow(true);

	m_BtnErye.GetClientRect(&rect);
	m_BtnErye.MoveWindow(m_pXiaoTu.x+rect.Width(),m_pXiaoTu.y-rect.Height()-2,rect.Width(),rect.Height());
	m_BtnErye.ShowWindow(true);   

	CRect iRect;
	GetClientRect(&iRect);
	m_BtnClose.MoveWindow(iRect.Width()-27,5,20,16);

	//m_BmCancel.EnableWindow(TRUE);		
	//m_BmOk.EnableWindow(TRUE);
	return TRUE;
}
int Clookinfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  �ڴ������ר�õĴ�������	

	return 0;
}
///��Ϣ����
BOOL Clookinfo::PreTranslateMessage(MSG * pMsg)
{
	if(pMsg->message==WM_LBUTTONUP)
		bMouseDown=false;
	return __super::PreTranslateMessage(pMsg);
}


///���ƣ�OnPaint
///������·�ӶԻ����ͼ��ť
///@param 
///@return 
void Clookinfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CGameImageHelper	bk(&m_bk);
	bk.BitBlt(dc.GetSafeHdc(),0,0);
//////////////////////////
	CDC*pDC;
	pDC = GetDC();	
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);
//����72��һ�ֵ�·��lym2010-03-18
#ifdef SET72 

	DrawLuzi(pDC);

#else

	CRect  rectName(0,0,0,0);
///������Ӯ������Ϣ
	///ׯӮ����
	if(m_iPlayCount<=0)
	{
		m_iPlayCount=1;
	}
	else if(m_iPlayCount>1)
	{
		m_iPlayCount-=1;
	}
	int count =0;
	count=m_iZwin+m_iXwin+m_iHwin;
	if(count<=0)
		count=1;
	int temp=0;
	CString stb="",stp="",str="";
	temp = m_iZwin*100/count;
	stp.Format("%d",temp);//ǰ��λ
	temp = m_iZwin*100*100/count;
	temp %= 100;//�Ŵ�100��,��ȡ������� 
	stb.Format("%d",temp);//ǰ��λ
	str=stp+"."+stb+"%";
	//���str;
	//pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	rectName.SetRect(435,44,485,60);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	str.Format("%d",m_iZwin);//ׯӮ�˼���
	//���str;	
	rectName.SetRect(400,44,426,60);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	///��Ӯ����
	temp = m_iXwin*100/count;
	stp.Format("%d",temp);//ǰ��λ
	temp = m_iXwin*100*100/count;
	temp %= 100;//�Ŵ�100��,��ȡ������� 
	stb.Format("%d",temp);//ǰ��λ
	str=stp+"."+stb+"%";;
	//���str;
	rectName.SetRect(105,44,155,60);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	str.Format("%d",m_iXwin);//ׯӮ�˼���
	//���str;
	rectName.SetRect(65,44,95,60);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	///��Ӯ�ı���
	temp = m_iHwin*100/count;
	stp.Format("%d",temp);//ǰ��λ
	temp = m_iHwin*100*100/count;
	temp %= 100;//�Ŵ�100��,��ȡ������� 
	stb.Format("%d",temp);//ǰ��λ
	str=stp+"."+stb+"%";;
	///���str;
	rectName.SetRect(275,44,325,60);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	str.Format("%d",m_iHwin);//ׯӮ�˼���
	///���str;
	rectName.SetRect(240,44,265,60);
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	///�ж���
	rectName.SetRect(95,75,130,90);
	str.Format("%d",m_iXDuizi);//ׯӮ�˼���
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	///ׯ����
	rectName.SetRect(215,75,265,90);
	str.Format("%d",m_iZDuizi);//ׯӮ�˼���
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	///������
	rectName.SetRect(355,75,385,90);
	str.Format("%d",m_iXTianwang);//ׯӮ�˼���
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	///ׯ����
	rectName.SetRect(450,75,490,90);
	str.Format("%d",m_iZTianwan);//ׯӮ�˼���
	pDC->DrawText(str,str.GetLength(),&rectName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	CGameImageHelper	zhuang(&m_imgzhuang);
	CGameImageHelper	xian(&m_imgxian);
	CGameImageHelper	he(&m_imghe);
	
///ճ���Ϸ�·����Ϣ
	int numx=0,numy=0,type=0;
	for(int i=0;i<count;i++)
	{
		if(m_iWinType[i] != type && type!=0 && m_iWinType[i]!=3)
		{
			type = m_iWinType[i];
			numx++;//����
			numy=0;
		}
		else if(m_iWinType[i] != type && type==0)
		{
			type = m_iWinType[i];
		}
		else
		{
			numy++;//��
		}
		if(numy==6)
		{
			numy=0;
			numx++;
		}

		//����numy��numx���ͼƬ
		if(m_iWinType[i]==1 )//��ׯ
		{
			zhuang.BitBlt(dc.GetSafeHdc(),m_pXiaoTu.x + numx*m_iXiaoWidth,m_pXiaoTu.y + numy*m_iXiaoWidth);
		}
		else if(m_iWinType[i]==2)//����
		{
			xian.BitBlt(dc.GetSafeHdc(),m_pXiaoTu.x + numx*m_iXiaoWidth,m_pXiaoTu.y + numy*m_iXiaoWidth);
		}
		else if(m_iWinType[i]==3)//����
		{
			he.BitBlt(dc.GetSafeHdc(),m_pXiaoTu.x + numx*m_iXiaoWidth,m_pXiaoTu.y + numy*m_iXiaoWidth);
		}
	}
	CGameImageHelper	zhuang1(&m_imgzhuang1);
	CGameImageHelper	xian1(&m_imgxian1);
	CGameImageHelper	he1(&m_imghe1);
///ճ���·�·����Ϣ
	numy=0;
	numx=0;
	for(int i=0;i<count;i++)
	{
		numy = i%6;
		numx = i/6;
		//����m_iWinType[i]��ֵ��ͼ
		if(m_iWinType[i]==1)//��ׯ
		{
			zhuang1.BitBlt(dc.GetSafeHdc(),m_pDaTu.x + numx*m_iDaWidth,m_pDaTu.y + numy*m_iDaWidth);
		}
		else if(m_iWinType[i]==2)//����
		{
			xian1.BitBlt(dc.GetSafeHdc(),m_pDaTu.x + numx*m_iDaWidth,m_pDaTu.y + numy*m_iDaWidth);
		}
		else if(m_iWinType[i]==3)//����
		{
			he1.BitBlt(dc.GetSafeHdc(),m_pDaTu.x + numx*m_iDaWidth,m_pDaTu.y + numy*m_iDaWidth);
		}
	}
#endif
	pDC->DeleteDC();

}

///���ƣ�Init
///��������ʼ�����ݣ�ׯ�п�����Ϣ�Ͱٷֱ�
///@param 
///@return 
void Clookinfo::Init()
{
	m_iZwin =0 ;//ׯ��Ӯ�İٷֱȣ�Ϊ������������Ŵ�100����������ǰ����Ϊ��С�������
	m_iXwin =0 ;//�м�Ӯ�İٷֱ�
	m_iHwin =0 ;//��Ӯ�İٷֱ�
	m_iZDuizi =0 ;//ׯ����
	m_iXDuizi =0 ;//�ж���
	m_iZTianwan =0 ;//ׯ����
	m_iXTianwang =0 ;//������
	m_iPlayCount=0;
	memset(m_bySLuziData,255,sizeof(m_bySLuziData));
	//lym2010-03-17
	memset(m_iWinType,0,sizeof(m_iWinType));
	///�Ƿ���ʾ��һҳ��ͼƬ

	memset(m_bKaiPaiData,255,sizeof(m_bKaiPaiData));
	memset(m_iKaiPaiType,0,sizeof(m_iKaiPaiType));

	m_bShowNext = false;

	memset(&m_iZPaiXing, 0, sizeof(m_iZPaiXing)); //ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	memset(&m_iXPaiXing, 0, sizeof(m_iXPaiXing));//�м�����
	//m_iNowJuShu = 0;	//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�
	
}
BOOL Clookinfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE; //CDialog::OnEraseBkgnd(pDC);
}

void Clookinfo::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanging(lpwndpos);

	// TODO: �ڴ˴������Ϣ����������
	Invalidate();
}

void Clookinfo::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	// TODO: �ڴ˴������Ϣ����������
}

void Clookinfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	bMouseDown=true;
	//savePoint=point;

	CDialog::OnLButtonDown(nFlags, point);
}
///���ƣ�DrawLuzi
///����������·�ӵ�ͼƬ��������Ϣ
///@param pDC ��ͼ�豸
///@return 
void Clookinfo::DrawLuzi(CDC*pDC)
{
	CRect  rectName(0,0,0,0);
	//////������Ӯ������Ϣ
	///ׯӮ����
	if(m_iPlayCount <= 0)
	{
		m_iPlayCount = 1;
	}
	else if(m_iPlayCount > 1)
	{
		m_iPlayCount -= 1;
	}
	int count = 0;
	count = m_iZwin+m_iXwin+m_iHwin;
	if(count <= 0)
		count = 1;
	CString str = "";
	rectName.SetRect(155,43,212,61);
	int iSmallCount = 38; //
	int iBigCount = 18;
	int iBigPicHeight = 49;
	if (m_iGamePictureType > 0)
	{
		rectName.SetRect(155-62,43-27+2,212-62,61-27+2);
		///��Ӯ�˼���
		str.Format("%d",m_iXwin);		
		pDC->DrawText(str, str.GetLength(), &rectName, DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		///��Ӯ�˼���
		str.Format("%d",m_iHwin);		
		rectName.OffsetRect(170,0);
		pDC->DrawText(str, str.GetLength(), &rectName, DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		///ׯӮ�˼���	
		str.Format("%d",m_iZwin);		
		rectName.OffsetRect(163,0);
		pDC->DrawText(str, str.GetLength(), &rectName, DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		m_pDaTu.SetPoint(49-24+4-1,216-20+2-31);	//��ͼ����λ��
		m_pXiaoTu.SetPoint(17+13-1,93-29);			//Сͼ����λ��
		m_iDaWidth = 31;
	   iSmallCount = 29;
	   iBigCount = 15;
	   iBigPicHeight = 31;
	}
	else
	{
		///ׯӮ�˼���	
		str.Format("%d",m_iZwin);
		rectName.SetRect(488,43,544,61);
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		///��Ӯ�˼���
		str.Format("%d",m_iXwin);
		rectName.SetRect(155,43,212,61);
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		///��Ӯ�˼���
		str.Format("%d",m_iHwin);
		rectName.SetRect(325,43,381,61);
		pDC->DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}
	CGameImageHelper	zhuang(&m_imgzhuang);
	CGameImageHelper	xian(&m_imgxian);
	CGameImageHelper	he(&m_imghe);


	CGameImageHelper	zhuangduizi(&m_imgzhuangduizi);
	CGameImageHelper	xianduizi(&m_imgxianduizi);
	CGameImageHelper	heduizi(&m_imgheduizi);



	///ճ���Ϸ�·����Ϣ
	//��д��·��ԲȦ�Ĳ���
	int iBeginCol = 0; // ��ʼ��λ��
	if (m_iLuziCol > iSmallCount)
	{
		iBeginCol = m_iLuziCol - iSmallCount;
	}
	else
	{
		iBeginCol = 0;
	}
	for(int i=iBeginCol; i<iBeginCol+iSmallCount; i++)
	{
		for(int j=0; j<6; j++)
		{
			
			if(m_bySLuziData[i][j] == 1)//��ׯ
			{
				//MsgPrint("zhangjing::m_bKaiPaiData[i][j]=%d",m_bKaiPaiData[i][j]);
				
				if(m_bKaiPaiData[i][j]==3||m_bKaiPaiData[i][j]==6)
				{
				zhuangduizi.BitBlt(pDC->GetSafeHdc(),m_pXiaoTu.x + (i - iBeginCol) * m_iXiaoWidth,m_pXiaoTu.y + j*m_iXiaoWidth);
				}
				else
				{
				zhuang.BitBlt(pDC->GetSafeHdc(),m_pXiaoTu.x + (i - iBeginCol) * m_iXiaoWidth,m_pXiaoTu.y + j*m_iXiaoWidth);
				}
				
			}
			else if(m_bySLuziData[i][j] == 2)//����
			{
				//MsgPrint("zhangjing::AAAAAm_bKaiPaiData[i][j]=%d",m_bKaiPaiData[i][j]);
				if(m_bKaiPaiData[i][j]==3||m_bKaiPaiData[i][j]==6)
				{
					xianduizi.BitBlt(pDC->GetSafeHdc(),m_pXiaoTu.x + (i - iBeginCol) * m_iXiaoWidth,m_pXiaoTu.y + j*m_iXiaoWidth);
				}
				else
				{
					xian.BitBlt(pDC->GetSafeHdc(),m_pXiaoTu.x + (i - iBeginCol) * m_iXiaoWidth,m_pXiaoTu.y + j*m_iXiaoWidth);
				}
			}
			else if(m_bySLuziData[i][j] == 3)//����
			{
				//MsgPrint("zhangjing::m_bKaiPaiData[i][j]=%d",m_bKaiPaiData[i][j]);
				if(m_bKaiPaiData[i][j]==3||m_bKaiPaiData[i][j]==6)
				{
					heduizi.BitBlt(pDC->GetSafeHdc(),m_pXiaoTu.x + (i - iBeginCol) * m_iXiaoWidth,m_pXiaoTu.y + j*m_iXiaoWidth);
				}
				else
				{
					he.BitBlt(pDC->GetSafeHdc(),m_pXiaoTu.x + (i - iBeginCol) * m_iXiaoWidth,m_pXiaoTu.y + j*m_iXiaoWidth);
				}
				
			}
		}			
	}	

	CGameImageHelper	zhuang1(&m_imgzhuang1);
	CGameImageHelper	xian1(&m_imgxian1);
	CGameImageHelper	he1(&m_imghe1);

	CGameImageHelper	zhuang1_dot(&m_imgzhuang1_dot);
	CGameImageHelper	xian1_dot(&m_imgxian1_dot);
	CGameImageHelper	he1_dot(&m_imghe1_dot);


	///ճ���·�·����Ϣ
	CFont font, *OldFont = NULL;
	font.CreateFont(11,0,0,0,0,0,0,0,134,3,2,1,2,TEXT("����")); 
	OldFont = (CFont*)pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(0,0,0));
	count = 0;
	count = m_iZwin + m_iXwin + m_iHwin;
	if(count <= 0)
		count = 1;

	int numy = 0, numx = 0, ju = 0;	
	CString strJ = "", strT1 = "", strJ2 = "";	
	ju = m_iNowJuShu % 100;
	if(ju >= 10)
	{
		strJ.Format("%d",ju);
	}
	else
	{
		strT1.Format("%d",ju);
		strJ = "0" + strT1;
	}
	for(int i=0; i<MAXCOUNT/*count*/; i++)
	{
		numy = i % iBigCount;
		numx = i / iBigCount;
		//����m_iWinType[i]��ֵ��ͼ
		if(m_iWinType[i] == 1)		//��ׯ
		{
			if(m_iKaiPaiType[i]==3||m_iKaiPaiType[i]==6)
			{
				zhuang1_dot.BitBlt(pDC->GetSafeHdc(),m_pDaTu.x + numy*m_iDaWidth,m_pDaTu.y + numx*iBigPicHeight);
			}
			else
			{
				zhuang1.BitBlt(pDC->GetSafeHdc(),m_pDaTu.x + numy*m_iDaWidth,m_pDaTu.y + numx*iBigPicHeight);
			}
			
		}
		else if(m_iWinType[i] == 2)	//����
		{
			if(m_iKaiPaiType[i]==3||m_iKaiPaiType[i]==6)
			{
				xian1_dot.BitBlt(pDC->GetSafeHdc(),m_pDaTu.x + numy*m_iDaWidth,m_pDaTu.y + numx*iBigPicHeight);
			}
			else
			{
			xian1.BitBlt(pDC->GetSafeHdc(),m_pDaTu.x + numy*m_iDaWidth,m_pDaTu.y + numx*iBigPicHeight);
			}
		}
		else if(m_iWinType[i] == 3)	//����
		{
			if(m_iKaiPaiType[i]==3||m_iKaiPaiType[i]==6)
			{
				he1_dot.BitBlt(pDC->GetSafeHdc(),m_pDaTu.x + numy*m_iDaWidth,m_pDaTu.y + numx*iBigPicHeight);
			}
			else
			{
			he1.BitBlt(pDC->GetSafeHdc(),m_pDaTu.x + numy*m_iDaWidth,m_pDaTu.y + numx*iBigPicHeight);
			}
		}
		if (0 == m_iGamePictureType)
		{
			///���ƽ���m_iNowJuShu
			if(m_iWinType[i] > 0)
			{
				strJ2 = "";
				if(i >= 9)
				{
					strT1.Format("%d", i+1);
					strJ2 = strJ + strT1;
				}
				else
				{
					strT1.Format("%d", i+1);
					strJ2 = strJ + "0" + strT1;
				}
				rectName.SetRect(49+numy*m_iDaWidth,199+numx*49,79+numy*m_iDaWidth,214+numx*49);
				pDC->DrawText(strJ2,strJ2.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
				OutputDebugString(strJ2);
			}
		}	
	}
	pDC->SelectObject(OldFont);
	font.DeleteObject();
}

///���ƣ�ChangeData
///����������ת������������Ϣת������һ�ִ�Ÿ�ʽ�������ͼʹ��
///@param 
///@return 
void Clookinfo::ChangeData()
{
	memset(m_bySLuziData, 255, sizeof(m_bySLuziData));
	memset(m_bKaiPaiData,255,sizeof(m_bKaiPaiData));
	int numx=0,numy=0,type=0;
	type = m_iWinType[0];
	for(int i=0; i<m_iPlayCount; i++)
	{
		if ( m_iWinType[i] == 0)
			continue;
		if(type != m_iWinType[i] && m_iWinType[i] != 3)
		{
			numx++;
			numy = 0;
			type = m_iWinType[i];
		}
		else
		{
			if(i != 0)
			{
				numy++;//��
			}
		}
		if(numy == 6)
		{
			numy = 0;
			numx++;
		}
		m_bySLuziData[numx][numy] = m_iWinType[i];
		m_bKaiPaiData[numx][numy] = m_iKaiPaiType[i];
	}
	m_iLuziCol = numx + 1;	
	///�����һҳ�������ڶ�ҳ��û�б�Ҫ��ʾ��
	if(m_bySLuziData[37][0] == 255)
	{
		m_bShowNext = false;
		m_BtnYiye.EnableWindow(false);				// ��һҳ
		m_BtnErye.EnableWindow(false);				// �ڶ�ҳͼƬ
	}
}

///�����һҳ�¼�����ʾ��һҳ��·����Ϣ
void Clookinfo::OnBnClickedButYiye()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	///�Ƿ���ʾ��һҳ��ͼƬ
	m_bShowNext = false;
	m_BtnYiye.EnableWindow(false);				// ��һҳ
	m_BtnErye.EnableWindow(true);				// �ڶ�ҳͼƬ
}
///����ڶ�ҳ�¼�����ʾ�ڶ�ҳ��·����Ϣ
void Clookinfo::OnBnClickedButErye()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	///�Ƿ���ʾ��һҳ��ͼƬ
	m_bShowNext = true;
	m_BtnYiye.EnableWindow(true);				// ��һҳ
	m_BtnErye.EnableWindow(false);				// �ڶ�ҳͼƬ
}
///�ر�·�ӶԻ���
void Clookinfo::OnBnClickedButClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->ShowWindow(false);
}

