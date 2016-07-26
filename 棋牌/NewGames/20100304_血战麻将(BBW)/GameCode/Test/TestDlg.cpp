// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include ".\testdlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CTestDlg �Ի���

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUT_HUPAI, OnBnClickedButHupai)
	ON_BN_CLICKED(IDC_BUT_TINGPAI, OnBnClickedButTingpai)
	ON_CBN_SELCHANGE(IDC_CO_ST_CH, OnCbnSelchangeCoStCh)
	ON_CBN_SELCHANGE(IDC_CO_MAINJING, OnCbnSelchangeCoMainjing)
	ON_CBN_SELCHANGE(IDC_CO_LESSJING, OnCbnSelchangeCoLessjing)
	ON_EN_CHANGE(IDC_EDIT_DATA, OnEnChangeEditData)
	ON_BN_CLICKED(IDC_BUT_MORE_TEST, OnBnClickedButMoreTest)
	ON_BN_CLICKED(IDC_CHECK_DIAOPAO, OnBnClickedCheckDiaopao)
END_MESSAGE_MAP()

// CTestDlg ��Ϣ�������

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//((CButton *)GetDlgItem(IDC_CHECK_DIAOPAO))->SetSetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CHECK_DIAOPAO))->SetCheck(BST_UNCHECKED);	///���ں��� ��Ĭ���ޣ�

	m_SrvLogic.SetDeskPoint(this);//�����ܼ�ⷽ��(��ȡ����ָ��)


	CComboBox *pCtrl = NULL;
	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_MAINJING);		///����
	AddPaiStrToCom(pCtrl);

	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_LESSJING);		///����
	AddPaiStrToCom(pCtrl);


	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}


void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestDlg::OnPaint() 
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////////////

///������Ϣ��ӵ��б�ؼ���
void CTestDlg::AddPaiStrToCom(CComboBox *pCtrl)
{
	pCtrl->InsertString(0,"��");
	pCtrl->InsertString(1,"1��");
	pCtrl->InsertString(2,"2��");
	pCtrl->InsertString(3,"3��");
	pCtrl->InsertString(4,"4��");
	pCtrl->InsertString(5,"5��");
	pCtrl->InsertString(6,"6��");
	pCtrl->InsertString(7,"7��");
	pCtrl->InsertString(8,"8��");
	pCtrl->InsertString(9,"9��");

	pCtrl->InsertString(10,"1ͬ");
	pCtrl->InsertString(11,"2ͬ");
	pCtrl->InsertString(12,"3ͬ");
	pCtrl->InsertString(13,"4ͬ");
	pCtrl->InsertString(14,"5ͬ");
	pCtrl->InsertString(15,"6ͬ");
	pCtrl->InsertString(16,"7ͬ");
	pCtrl->InsertString(17,"8ͬ");
	pCtrl->InsertString(18,"9ͬ");

	pCtrl->InsertString(19,"1��");
	pCtrl->InsertString(20,"2��");
	pCtrl->InsertString(21,"3��");
	pCtrl->InsertString(22,"4��");
	pCtrl->InsertString(23,"5��");
	pCtrl->InsertString(24,"6��");
	pCtrl->InsertString(25,"7��");
	pCtrl->InsertString(26,"8��");
	pCtrl->InsertString(27,"9��");

	pCtrl->InsertString(28,"��");
	pCtrl->InsertString(29,"��");
	pCtrl->InsertString(30,"��");
	pCtrl->InsertString(31,"��");
	pCtrl->InsertString(32,"��");
	pCtrl->InsertString(33,"��");
	pCtrl->InsertString(34,"��");
}

///��λ����Ϣ��ӵ��б�ؼ���
void CTestDlg::AddStationStrToCom(CComboBox *pCtrl)
{
	pCtrl->InsertString(0,"-1");
	pCtrl->InsertString(1,"λ��0");
	pCtrl->InsertString(2,"λ��1");
	pCtrl->InsertString(3,"λ��2");
	pCtrl->InsertString(4,"λ��3");

}

///��������������Ϣ��ӵ��б�ؼ���
void CTestDlg::AddCPGTypeStrToCom(CComboBox *pCtrl)
{
	pCtrl->InsertString(0,"��");
	pCtrl->InsertString(1,"��");
	pCtrl->InsertString(2,"��");
	pCtrl->InsertString(3,"��");
	pCtrl->InsertString(4,"����");
	pCtrl->InsertString(5,"����");
	pCtrl->InsertString(6,"����");

}

///��ȡ�����б��Ƶ�����(������)
int CTestDlg::GetComboPaiData(CComboBox * pCtrl)
{
	int mj =255;
	int index=0;
	index = pCtrl->GetCurSel();
	if(index>0 && index <10)//��
	{
		mj = index;
	}
	else if(index >= 10 && index <= 18)//ͬ
	{
		mj = index+1;
	}
	else if(index >= 19 && index <= 27)//��
	{
		mj = index+2;
	}
	else if(index>=28 && index<=34)//����
	{
		mj = index+3;
	}
	return mj;
}

///��ȡ�����б�λ�õ�����(������)
int CTestDlg::GetComboStationData(CComboBox * pCtrl)
{
	int station = -1;
	int index=0;
	index = pCtrl->GetCurSel();
	station = index -1;
	return station;
}

///��ȡ�����б�����������͵�����(������)
int CTestDlg::GetComboCPGTypeData(CComboBox * pCtrl)
{
	int type = 0;
	int index=0;
	index = pCtrl->GetCurSel();
	switch(index)
	{
	case 0:
		type = ACTION_NO;//��
		break;
	case 1:
		type = ACTION_CHI;//���ƶ���
		break;
	case 2:
		type = ACTION_PENG;//���ƶ���
		break;
	case 3:
		type = ACTION_KAN;//�����ƶ���
		break;
	case 4:
		type = ACTION_AN_GANG;//���ܶ���
		break;
	case 5:
		type = ACTION_BU_GANG;//���ܶ���
		break;
	case 6:
		type = ACTION_MING_GANG;//���ܶ���
		break;
	}			
	return type;
}

///���ݵ�ǰ�������ÿؼ�״̬
void CTestDlg::SetCtrlStatue()
{
	CComboBox * pCtrl = NULL;
	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_ST_CH);		///ѡ��λ��
	if(m_byNowStation==255)
	{	
		pCtrl->SetCurSel(0);
	}
	else
	{
		pCtrl->SetCurSel(m_byNowStation+1);
	}

	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_MAINJING);		///����
	if(m_byMainJing==255)
	{	
		pCtrl->SetCurSel(0);
	}
	else
	{
		pCtrl->SetCurSel(m_byMainJing - (m_byMainJing-1)/9);
	}
	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_LESSJING);		///����
	if(m_byLessJing==255)
	{	
		pCtrl->SetCurSel(0);
	}
	else
	{
		pCtrl->SetCurSel(m_byLessJing - (m_byLessJing-1)/9);
	}
	if(m_byNowStation<4)//��ҳ���������
	{
		int index=0;
		for(int i=0;i<5;i++)
		{
			if(m_stCPGData[m_byNowStation][i].byType == 255)
				break;
			
			
			index++;
		}
		for(int i=index;i<5;i++)
		{
			m_stCPGData[m_byNowStation][i].Init();
		}
	}
	//bool m_bZimo;//�Ƿ���������
}


//���ַ���������������
BYTE CTestDlg::ChangeStrToData(CString st)
{
	CString str="";
	BYTE pai = 0;
	if(st == "df")
	{
		pai = 31;
	}
	else if(st == "nf")
	{
		pai = 32;
	}
	else if(st == "xf")
	{
		pai = 33;
	}
	else if(st == "bf")
	{
		pai = 34;
	}
	else if(st == "hz")
	{
		pai = 35;
	}
	else if(st == "fc")
	{
		pai = 36;
	}
	else if(st == "bb")
	{
		pai = 37;
	}
	else
	{
		if(st == "1w")
		{
			pai = 1;
		}
		else if(st == "2w")
		{
			pai = 2;
		}
		else if(st == "3w")
		{
			pai = 3;
		}
		else if(st == "4w")
		{
			pai = 4;
		}
		else if(st == "5w")
		{
			pai = 5;
		}
		else if(st == "6w")
		{
			pai = 6;
		}
		else if(st == "7w")
		{
			pai = 7;
		}
		else if(st == "8w")
		{
			pai = 8;
		}
		else if(st == "9w")
		{
			pai = 9;
		}
		else if(st == "1b")
		{
			pai = 11;
		}
		else if(st == "2b")
		{
			pai = 12;
		}
		else if(st == "3b")
		{
			pai = 13;
		}
		else if(st == "4b")
		{
			pai = 14;
		}
		else if(st == "5b")
		{
			pai = 15;
		}
		else if(st == "6b")
		{
			pai = 16;
		}
		else if(st == "7b")
		{
			pai = 17;
		}
		else if(st == "8b")
		{
			pai = 18;
		}
		else if(st == "9b")
		{
			pai = 19;
		}
		else if(st == "1t")
		{
			pai = 21;
		}
		else if(st == "2t")
		{
			pai = 22;
		}
		else if(st == "3t")
		{
			pai = 23;
		}
		else if(st == "4t")
		{
			pai = 24;
		}
		else if(st == "5t")
		{
			pai = 25;
		}
		else if(st == "6t")
		{
			pai = 26;
		}
		else if(st == "7t")
		{
			pai = 27;
		}
		else if(st == "8t")
		{
			pai = 28;
		}
		else if(st == "9t")
		{
			pai = 29;
		}
	}

	if(pai==0)
		pai = 255;
	return pai;
}

//ѡ�����λ��
void CTestDlg::OnCbnSelchangeCoStCh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//ѡ������
void CTestDlg::OnCbnSelchangeCoMainjing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox * pCtrl = NULL;
	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_MAINJING);		///����
	m_byMainJing = GetComboPaiData(pCtrl);//����
}

//ѡ�񸱾�
void CTestDlg::OnCbnSelchangeCoLessjing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox * pCtrl = NULL;
	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_LESSJING);		///����
	m_byLessJing = GetComboPaiData(pCtrl);//����
}


//����������
void CTestDlg::OnEnChangeEditData()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CEdit * pCtrl = NULL;
	pCtrl = (CEdit *)GetDlgItem(IDC_EDIT_DATA);		///�������ַ���
	pCtrl->GetWindowTextA(m_sData);

}


//���Ʋ��԰�ť
void CTestDlg::OnBnClickedButHupai()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BYTE hupaitype[20];//��������
	BYTE data[14];
	memset(data,255,sizeof(data));
	memset(hupaitype,255,sizeof(hupaitype));

	m_bZimo = false;
	if(((CButton *)GetDlgItem(IDC_CHECK_DIAOPAO))->GetCheck() == BST_UNCHECKED)	///���ں��� ��Ĭ���ޣ�
	{
		m_bZimo = true;
	}

	m_SrvLogic.m_UserData.InitData();
	//�����������
	CString str="";
	int index = 0;
	for(int i=0;i<28;i+=2)
	{
		str = m_sData.Mid(i,2);
		data[index] = ChangeStrToData(str);
		index++;
	}
	for(int i=0;i<14;i++)
	{
		m_SrvLogic.m_UserData.m_byArHandPai[0][i] = data[i];
	}

	CComboBox * pCtrl = NULL;
	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_MAINJING);		///����
	m_byMainJing = GetComboPaiData(pCtrl);//����
	pCtrl = (CComboBox *)GetDlgItem(IDC_CO_LESSJING);		///����
	m_byLessJing = GetComboPaiData(pCtrl);//����

	m_SrvLogic.m_UserData.m_StrJing.AddJing(m_byMainJing);//������
	m_SrvLogic.m_UserData.m_StrJing.AddJing(m_byLessJing);//������

	str.Format("���Ʋ��ԣ�pai ���� %d %d %d %d %d %d %d %d %d %d %d %d %d %d",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],data[12],data[13]);
	OutputDebugString(str);
	
	if(m_SrvLogic.m_CheckHuPai.CanHu(0,m_SrvLogic.m_UserData.GetLastHandPai(0),m_SrvLogic.m_GameData.T_CPGNotify[0],m_bZimo,0))
	{
		str = "";
		GetHuPaiType(str,m_SrvLogic.m_GameData.T_CPGNotify[0].byHuType);
		AfxMessageBox(str);

	}
	else
	{
		AfxMessageBox("�޷�����");
	}

}

//���Ʋ���
void CTestDlg::OnBnClickedButTingpai()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BYTE hupaitype[20];
	BYTE data[14];
	BYTE CanOutData[14];//�����ܳ�����
	memset(data,255,sizeof(data));
	memset(CanOutData,255,sizeof(CanOutData));
	memset(hupaitype,255,sizeof(hupaitype));

	m_SrvLogic.m_UserData.InitData();
	//�����������

	CString str="";
	int index = 0;
	for(int i=0;i<28;i+=2)
	{
		str = m_sData.Mid(i,2);
		data[index] = ChangeStrToData(str);
		index++;
	}
	for(int i=0;i<14;i++)
	{
		m_SrvLogic.m_UserData.m_byArHandPai[0][i] = data[i];
	}

	m_SrvLogic.m_UserData.m_StrJing.Init();//û�в���

	if(m_SrvLogic.m_CheckHuPai.CanTing(0,m_SrvLogic.m_GameData.T_CPGNotify[0]))
	{
		str.Format("���Ʋ��ԣ��ܳ����ƣ� %d %d %d %d %d %d %d",CanOutData[0],CanOutData[1],CanOutData[2],CanOutData[3],CanOutData[4],CanOutData[5],CanOutData[6]);
		AfxMessageBox(str);
	}
	else
	{
		AfxMessageBox("�޷�����");
	}
}

//����������
void CTestDlg::OnBnClickedButMoreTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString s = CINIFile::GetAppPath ();//����·��   
	CINIFile f(s  + _T("hipaitest.ini"));
	CString key="count";
	int count = f.GetKeyVal(key,"count",0);//����������
	BYTE handpai[17],lastpai=255 ,mainjing = 255,lessjing = 255;
	memset(handpai,255,sizeof(handpai));
	CString strdata="",sHuType="",schecktype0="",schecktype1="",schecktype2="",str="";
	int index = 0;
	bool zimo = false;
	BYTE hupaitype[20];
	memset(hupaitype,255,sizeof(hupaitype));

	for(int i=0;i<count;++i)
	{
		m_SrvLogic.m_UserData.InitData();
		memset(hupaitype,255,sizeof(hupaitype));

		key.Format("kind_%d",i);
		//������
		strdata = f.GetKeyVal(key,"pai","");
		index = 0;
		for(int j=0;j<28;j+=2)
		{
			str = strdata.Mid(j,2);
			m_SrvLogic.m_UserData.m_byArHandPai[0][index] = ChangeStrToData(str);
			index++;
		}

		if(index%3 != 2)//��������
		{
			f.SetKeyValString(key,"hutype","�����������ԣ���ȷ������2,5,8,11,14");//�����������
			continue;
		}

		//������
		lastpai = f.GetKeyVal(key,"lastpai",255);
		//�Ƿ�����
		zimo = f.GetKeyVal(key,"zimo",0);
		//����
		mainjing = f.GetKeyVal(key,"mainjing",255);
		//����
		lessjing = f.GetKeyVal(key,"lessjing",255);
		m_SrvLogic.m_UserData.m_StrJing.AddJing(mainjing);//
		m_SrvLogic.m_UserData.m_StrJing.AddJing(lessjing);//
		//Ҫ���ĺ�������:���֧��ͬʱ���3������
		schecktype0 = f.GetKeyVal(key,"checktype0","");
		schecktype1 = f.GetKeyVal(key,"checktype1","");
		schecktype2 = f.GetKeyVal(key,"checktype2","");
		//���  ��������
		if(m_SrvLogic.m_CheckHuPai.CanHu(0,lastpai,m_SrvLogic.m_GameData.T_CPGNotify[0],zimo,0))
		{//���Ƴɹ��������������
			GetHuPaiType(sHuType,hupaitype);
			f.SetKeyValString(key,"hutype",sHuType);//�����������

			if(schecktype0 != "")//�м������
			{
				if(sHuType.Find(schecktype0) != -1)//�����ú�������
				{
					schecktype0 += " ����";
					f.SetKeyValString(key,"outtype0",schecktype0);//�����������
				}
				else
				{
					schecktype0 += " ������";
					f.SetKeyValString(key,"outtype0",schecktype0);//�����������
				}
			}

			if(schecktype1 != "")//�м������
			{
				if(sHuType.Find(schecktype1) != -1)//�����ú�������
				{
					schecktype1 += " ����";
					f.SetKeyValString(key,"outtype1",schecktype1);//�����������
				}
				else
				{
					schecktype1 += " ������";
					f.SetKeyValString(key,"outtype1",schecktype1);//�����������
				}
			}

			if(schecktype2 != "")//�м������
			{
				if(sHuType.Find(schecktype2) != -1)//�����ú�������
				{
					schecktype2 += " ����";
					f.SetKeyValString(key,"outtype2",schecktype2);//�����������
				}
				else
				{
					schecktype2 += " ������";
					f.SetKeyValString(key,"outtype2",schecktype2);//�����������
				}
			}
		}
		else
		{//�޷�����		
			f.SetKeyValString(key,"hutype","�޷�����");//�����������
		}
	}

}

//����
void CTestDlg::OnBnClickedCheckDiaopao()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


//��ȡ��������
void CTestDlg::GetHuPaiType(CString & str,BYTE hupaitype[])
{
	CString  strtype="";
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_HU_PAI,hupaitype))//����
	{
		strtype = " ����"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_ZI_MO,hupaitype))//����
	{
		strtype = " ����"; str += strtype;
	}

	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_QING_YI_SE,hupaitype))//��һɫ
	{
		strtype = " ��һɫ"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_PENG_PENG_HU,hupaitype))//������
	{
		strtype = " ������"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_QI_DUI,hupaitype))//�߶�
	{
		strtype = " �߶�"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_HAO_HUA_QI_DUI,hupaitype))//�����߶�
	{
		strtype = " �����߶�"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_TIAN_HU,hupaitype))//���
	{
		strtype = " ���"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_DI_HU,hupaitype))//�غ�
	{
		strtype = " �غ�"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_QIANG_GANG,hupaitype))//����
	{
		strtype = " ����"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_GANG_KAI,hupaitype))//�ܿ�
	{
		strtype = " �ܿ�"; str += strtype;
	}
	if(m_SrvLogic.m_CheckHuPai.CheckHupaiType(HUPAI_GANG_PAO,hupaitype))//�ܺ���
	{
		strtype = " �ܺ���"; str += strtype;
	}
	
	
}


///���ƣ�GameFinish
///��������Ϸ�������ٴ˺����е��üƷֺ�����
bool CTestDlg::GameFinish(BYTE nDeskStation, BYTE bCloseFlag)
{
	return true;
}

///������Ϸ״̬
void CTestDlg::SendGameStationEx(BYTE nDeskStation, bool bWatchUser, void * pStationData, UINT uSize)
{
}
///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
void CTestDlg::SendGameDataEx(BYTE nDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
}
///�����Թ����� ��������Ϣ���Թ��ߣ�
void CTestDlg::SendWatchDataEx(BYTE nDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
}
///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
void CTestDlg::SendGameDataEx(BYTE nDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
}
///�����Թ����� ��������Ϣ���Թ��ߣ�
void CTestDlg::SendWatchDataEx(BYTE nDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
}

//��ȡ��ҵ�ͬ��״̬
void CTestDlg::GetUserAgree(BYTE agree)
{
}

//�Ƿ�����Ϸ��
bool CTestDlg::IsPlayGame(int bDeskStation)
{
	if(m_bGameStation>= 20 && m_bGameStation<23)
		return true;
	return false;
}

void CTestDlg::KillTimer(int nTimerID)
{
    KillTimer( nTimerID);
}
void CTestDlg::SetTimer(int nID, int nTime)
{
    SetTimer(nID, nTime);
}