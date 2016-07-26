// SuperGM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SuperGM.h"
#include "ClientGameDlg.h"

// CSuperGM �Ի���

IMPLEMENT_DYNAMIC(CSuperGM, CDialog)

CSuperGM::CSuperGM(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperGM::IDD, pParent)
{
	m_bShowRobotInfo = false;
}

CSuperGM::~CSuperGM()
{
}

void CSuperGM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, 1071, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CSuperGM, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, 1071, &CSuperGM::OnLvnItemchanged1071)
	ON_BN_CLICKED(IDCANCEL, &CSuperGM::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_Z, &CSuperGM::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO_X, &CSuperGM::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO_H, &CSuperGM::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO_N, &CSuperGM::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_CHECK_ROBOT_INFO, &CSuperGM::OnBnClickedCheckRobotInfo)
END_MESSAGE_MAP()


// CSuperGM ��Ϣ�������

BOOL CSuperGM::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	int iBlockLen = 0;
	m_ListCtrl.GetClientRect(&rect);
	iBlockLen = (rect.right - rect.left) / 10;
	
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int count=0;
	m_ListCtrl.InsertColumn(count++, "�����", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "��ʼ���", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "�ܽ��", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "ׯ", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "ׯ����", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "ׯ����", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "��", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "������", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "�ж���", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "��", LVCFMT_LEFT, iBlockLen);
	m_ListCtrl.InsertColumn(count++, "ͬ���", LVCFMT_LEFT, iBlockLen);

	m_SelectedArea = -1;
	UpdataCtrlText();
	return TRUE;
}


void CSuperGM::OnLvnItemchanged1071(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CSuperGM::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CSuperGM::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SelectedArea = 1;
	SendCtrlMsg();///���Ϳ�����Ϣ

	CButton *pButton = NULL;
	pButton = (CButton *)GetDlgItem(IDC_RADIO_X);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_H);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_N);
	pButton->SetCheck(0);
}

void CSuperGM::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SelectedArea = 2;
	SendCtrlMsg();///���Ϳ�����Ϣ

	CButton *pButton = NULL;
	pButton	= (CButton *)GetDlgItem(IDC_RADIO_Z);
	pButton->SetCheck(0);

	pButton = (CButton *)GetDlgItem(IDC_RADIO_H);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_N);
	pButton->SetCheck(0);
}

void CSuperGM::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SelectedArea = 3;
	SendCtrlMsg();///���Ϳ�����Ϣ

	CButton *pButton = NULL;
	pButton	= (CButton *)GetDlgItem(IDC_RADIO_Z);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_X);
	pButton->SetCheck(0);

	pButton = (CButton *)GetDlgItem(IDC_RADIO_N);
	pButton->SetCheck(0);
}

void CSuperGM::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_SelectedArea = 0;
	SendCtrlMsg();///���Ϳ�����Ϣ

	CButton *pButton = NULL;
	pButton	= (CButton *)GetDlgItem(IDC_RADIO_Z);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_X);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_H);
	pButton->SetCheck(0);
}

//�Ƿ���ʾ��������Ϣ
void CSuperGM::OnBnClickedCheckRobotInfo()
{
	// TODO: Add your control notification handler code here

	CButton *pButton = NULL;

	pButton = (CButton *)GetDlgItem(IDC_CHECK_ROBOT_INFO);

	if(NULL != pButton)
	{
		m_bShowRobotInfo = pButton->GetCheck();
	}
}


void CSuperGM::OnSendDesktopXzInfo()
{
	if (m_bShowRobotInfo)
	{
		((CLoveSendClassInExe *)GetParent())->SendMessage(WM_SHOW_ROBOT_CHIP, 1, 0);
	}
	else
	{
		((CLoveSendClassInExe *)GetParent())->SendMessage(WM_SHOW_ROBOT_CHIP, 0, 0);
	}
}

//ˢ����ʾ����
void CSuperGM::UpdateListData(SUPERDATA *pSuperData)
{
		CString str;
		//str.Format("shenzheng UpdateListData  insert allnote %d",pSuperData->sUserInfo[i].iAllNote);
		//OutputDebugString(str);

	////������˸
	//m_ListCtrl.LockWindowUpdate();
	//m_ListCtrl.SetRedraw(TRUE);
	m_ListCtrl.DeleteAllItems();
	//m_ListCtrl.UnlockWindowUpdate();	

	int num=0;
	for(int i=0;i<PLAY_COUNT ;++i)
	{
		//û����ע �� �ǻ����˲���Ҫ����ʾ��������Ϣ�Ķ�����ʾ
		if(pSuperData->sUserInfo[i].iAllNote<=0 || (pSuperData->sUserInfo[i].bIsRobot && !m_bShowRobotInfo))
			continue;

		SUPERGM userinfo = pSuperData->sUserInfo[i];
		TCHAR buf[512];
		int count = 1;
		if (count > 50)
		{
			return;
		}
		//�������
		m_ListCtrl.InsertItem(num, userinfo.cUserName);

		//��ҳ�ʼ���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iOldMoney);
		m_ListCtrl.SetItemText(num, count++, buf);


		//�������ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iAllNote);
		m_ListCtrl.SetItemText(num, count++, buf);

		//����0��ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iUserNote[0]);
		m_ListCtrl.SetItemText(num, count++, buf);

		//����1��ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iUserNote[1]);
		m_ListCtrl.SetItemText(num, count++, buf);

		//����2��ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iUserNote[2]);
		m_ListCtrl.SetItemText(num, count++, buf);

		//����3��ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iUserNote[3]);
		m_ListCtrl.SetItemText(num, count++, buf);

		//����4��ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iUserNote[4]);
		m_ListCtrl.SetItemText(num, count++, buf);

		//����5��ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iUserNote[5]);
		m_ListCtrl.SetItemText(num, count++, buf);

		//����6��ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iUserNote[6]);
		m_ListCtrl.SetItemText(num, count++, buf);

		//����7��ע���
		memset(buf, 0, 512 * sizeof(TCHAR));
		wsprintf(buf, _T("%I64d"), pSuperData->sUserInfo[i].iUserNote[7]);
		m_ListCtrl.SetItemText(num, count++, buf);

		num++;
		CString str;
		str.Format("shenzheng TTTTTTTTTTTTTTTT  insert dlg  -> %d", i);
		OutputDebugString(str);
	}
	m_ListCtrl.SetRedraw(TRUE);
}

///���¿�����ʾ
void CSuperGM::UpdataCtrlText()
{
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_INFO);
	switch(m_SelectedArea)
	{
	case 1://���ƿ�ׯ
		{
			pStatic->SetWindowTextA("��ǰ���ƿ�ׯ");
		}break;
	case 2://���ƿ���
		{
			pStatic->SetWindowTextA("��ǰ���ƿ���");
		}break;
	case 3://���ƿ���
		{
			pStatic->SetWindowTextA("��ǰ���ƿ���");
		}break;
	default:
		{
			pStatic->SetWindowTextA("��ǰ��������");
		}break;
	}
}
void CSuperGM::UpDataNoteRateInfo(__int64 iNtWin , __int64 iXianWin ,__int64 iHeWin) 
{
	CString strNoteInfo ; 

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_ZHUANG_TOTLE);
	if(NULL != pStatic)
	{
		strNoteInfo.Format("%I64d" , iNtWin) ; 
		pStatic->SetWindowTextA(strNoteInfo.GetBuffer()) ;
	}
    
	pStatic = (CStatic *)GetDlgItem(IDC_STATIC_XIAN_TOTLE);
	if(NULL != pStatic)
	{
		strNoteInfo.Format("%I64d" , iXianWin) ; 
		pStatic->SetWindowTextA(strNoteInfo.GetBuffer()) ;
	}
	
	pStatic = (CStatic *)GetDlgItem(IDC_STATIC_HE_TOTLE);
	if(NULL != pStatic)
	{
		strNoteInfo.Format("%I64d" , iHeWin) ; 
		pStatic->SetWindowTextA(strNoteInfo.GetBuffer()) ;
	}
}
///��ʾ����
void CSuperGM::ShowSuperWindow(bool show)
{
	ShowWindow(show);
	UpdataCtrlText();
}

///���Ϳ�����Ϣ
void CSuperGM::SendCtrlMsg()
{
	if(g_pCClientGameDlg != NULL)
	{
		g_pCClientGameDlg->OnSuperCtr(m_SelectedArea,0);
	}
}

//���ÿ��ƽ��
void CSuperGM::SetCtrlResult(BYTE result)
{
	m_SelectedArea = result;
	UpdataCtrlText();

	CButton *pButton = NULL;
	pButton	= (CButton *)GetDlgItem(IDC_RADIO_Z);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_X);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_H);
	pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_N);
	pButton->SetCheck(0);
	if(m_SelectedArea>=1 && m_SelectedArea<=3)
	{
		pButton	= (CButton *)GetDlgItem(IDC_RADIO_Z + m_SelectedArea -1);
		pButton->SetCheck(1);
	}
	else
	{
		pButton = (CButton *)GetDlgItem(IDC_RADIO_N);
		pButton->SetCheck(1);
	}
}
