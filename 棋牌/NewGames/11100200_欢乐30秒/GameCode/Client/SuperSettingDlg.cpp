// SuperSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SuperSettingDlg.h"
#include "LoveSendClassInExe.h"

// CSuperSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSuperSettingDlg, CDialog)

CSuperSettingDlg::CSuperSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperSettingDlg::IDD, pParent)
{

}

CSuperSettingDlg::~CSuperSettingDlg()
{
}

void CSuperSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSuperSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSuperSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSuperSettingDlg ��Ϣ�������
BOOL CSuperSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CheckRadioButton(IDC_RADIO_NORMAL, IDC_RADIO_ZHUANG_LOST, IDC_RADIO_NORMAL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSuperSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SUPERSETTING superSetting;

	switch (GetCheckedRadioButton(IDC_RADIO_NORMAL, IDC_RADIO_ZHUANG_LOST))
	{
	case IDC_RADIO_NORMAL:
		{
			superSetting.bState = 0;   
			break;
		}
	case IDC_RADIO_ZHUANG_WIN:
		{
			superSetting.bState = 1;   
			break;
		}
	case IDC_RADIO_ZHUANG_LOST:
		{
			superSetting.bState = 2;   
			break;
		}
	default:
		{
			superSetting.bState = 0;   
			break;
		}
	}

	((CLoveSendClassInExe *)GetParent())->SendGameData(&superSetting, sizeof(superSetting), MDM_GM_GAME_NOTIFY, ASS_SUPER_SETTING, 0);

	OnOK();
}
