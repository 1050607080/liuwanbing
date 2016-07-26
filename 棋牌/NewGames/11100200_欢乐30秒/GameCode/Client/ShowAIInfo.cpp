// ShowAIInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShowAIInfo.h"
#include "ClientGameDlg.h"
#define  BaseX  50
#define  BaseY  50
// CShowAIInfo �Ի���
///��������Ӯ���ƣ���Ϣ��ʾ��
IMPLEMENT_DYNAMIC(CShowAIInfo, CDialog)

CShowAIInfo::CShowAIInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CShowAIInfo::IDD, pParent)
	, m_GoalMoney(0)
	, m_iAveMoney(0)
{
	bIsClearAIWinMoney	= false;		//��ӮǮ����
	bIsSetAICtrl	= false;			//�޸�����
	bAIWinAndLostAutoCtrl	= false;	//��������
}

CShowAIInfo::~CShowAIInfo()
{
}

void CShowAIInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GOAL_MONEY, m_GoalMoney);
	DDX_Text(pDX, IDC_EDIT_AVE_MONEY, m_iAveMoney);
}


BEGIN_MESSAGE_MAP(CShowAIInfo, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUT_SEND_CHANGE, &CShowAIInfo::OnBnClickedButSendChange)
	ON_BN_CLICKED(IDC_CH_CHANGE, &CShowAIInfo::OnBnClickedChChange)
	ON_BN_CLICKED(IDC_CH_CLEAR_WIN, &CShowAIInfo::OnBnClickedChClearWin)
	ON_BN_CLICKED(IDC_CH_CHANGE_SET, &CShowAIInfo::OnBnClickedChChangeSet)
	ON_EN_CHANGE(IDC_EDIT_GOAL_MONEY, &CShowAIInfo::OnEnChangeEditGoalMoney)
	ON_EN_CHANGE(IDC_EDIT_AVE_MONEY, &CShowAIInfo::OnEnChangeEditAveMoney)
END_MESSAGE_MAP()


// CShowAIInfo ��Ϣ�������

BOOL CShowAIInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->CenterWindow(this);

	((CButton *)GetDlgItem(IDC_CH_SET_CTRL))->SetCheck(BST_CHECKED);	///��������

	m_GoalMoney = m_AIInfo.iAIWantWinMoney;
	m_iAveMoney = m_AIInfo.iAIMaxWinInOne;

	CString str="";
	str.Format("%d",m_GoalMoney);
	((CEdit *)GetDlgItem(IDC_EDIT_GOAL_MONEY))->SetWindowTextA(str);
	str.Format("%d",m_iAveMoney);
	((CEdit *)GetDlgItem(IDC_EDIT_AVE_MONEY))->SetWindowTextA(str);

	return TRUE;
}


void CShowAIInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CFont font, *pOldFont;
	font.CreateFont(-13,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	pOldFont = (CFont*)dc.SelectObject(&font);
	SetTextColor(dc.GetSafeHdc(), RGB(0,0,0));

	dc.SetBkMode(TRANSPARENT);
	CString str="��ǰ��������Ӯ��������";
	dc.TextOutA(BaseX,BaseY,str);

	if(m_AIInfo.bAIWinAndLostAutoCtrl)
	{
		str="��ǰ��������Ӯ���ƴ��ڿ���״̬";
	}
	else
	{
		str="��ǰ��������Ӯ���ƴ��ڹر�״̬";
	}
	dc.TextOutA(BaseX,BaseY+40,str);

	str.Format("������ӮǮĿ�꣺%d",m_AIInfo.iAIWantWinMoney);
	dc.TextOutA(BaseX,BaseY+60,str);

	str.Format("�������Ѿ�Ӯ�ˣ�%d",m_AIInfo.iAIHaveWinMoney);
	dc.TextOutA(BaseX,BaseY+80,str);

	str.Format("������ƽ��ÿ��ӮǮ����Ϊ��%d",m_AIInfo.iAIMaxWinInOne);
	dc.TextOutA(BaseX,BaseY+100,str);

	if(m_AIInfo.iAICtrlCount<=0)
		m_AIInfo.iAICtrlCount=1;
	str.Format("��ǰ������ӮǮƽ������%d",m_AIInfo.iAIHaveWinMoney/m_AIInfo.iAICtrlCount);
	dc.TextOutA(BaseX,BaseY+120,str);

	str.Format("�������Ѿ������Զ����Ƶİ�����%d",m_AIInfo.iAICtrlCount);
	dc.TextOutA(BaseX,BaseY+140,str);
	dc.SelectObject(pOldFont);
	font.DeleteObject();

}
//��ʾ�Ի���
void CShowAIInfo::ShowAIInfoWindow()
{

	((CButton *)GetDlgItem(IDC_CH_CLEAR_WIN))->EnableWindow(false);		///��ǰ����
	((CButton *)GetDlgItem(IDC_CH_CHANGE_SET))->EnableWindow(false);	///�޸�����
	((CButton *)GetDlgItem(IDC_BUT_SEND_CHANGE))->EnableWindow(false);	///������Ϣ��ť

	((CButton *)GetDlgItem(IDC_CH_SET_CTRL))->EnableWindow(false);		///��������
	((CEdit *)GetDlgItem(IDC_EDIT_GOAL_MONEY))->EnableWindow(false);	///ӮǮĿ��
	((CEdit *)GetDlgItem(IDC_EDIT_AVE_MONEY))->EnableWindow(false);		///ӮǮƽ��ֵ

	((CButton *)GetDlgItem(IDC_CH_CLEAR_WIN))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CH_CHANGE_SET))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_CH_CHANGE))->SetCheck(BST_UNCHECKED);	

	m_GoalMoney = m_AIInfo.iAIWantWinMoney;


	CString str="";
	str.Format("%d",m_GoalMoney);
	((CEdit *)GetDlgItem(IDC_EDIT_GOAL_MONEY))->SetWindowTextA(str);

	str="";
	m_iAveMoney = m_AIInfo.iAIMaxWinInOne;
	str.Format("%d",m_iAveMoney);
	((CEdit *)GetDlgItem(IDC_EDIT_AVE_MONEY))->SetWindowTextA(str);

	((CEdit *)GetDlgItem(IDC_EDIT_GOAL_MONEY))->UpdateData(false);
	((CEdit *)GetDlgItem(IDC_EDIT_GOAL_MONEY))->UpdateWindow();

	((CEdit *)GetDlgItem(IDC_EDIT_AVE_MONEY))->UpdateData(false);
	((CEdit *)GetDlgItem(IDC_EDIT_AVE_MONEY))->UpdateWindow();

	this->ShowWindow(true);
}
///������Ͱ�ť
void CShowAIInfo::OnBnClickedButSendChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(((CButton *)GetDlgItem(IDC_CH_CHANGE))->GetCheck() == BST_CHECKED)///
	{
		this->UpdateWindow();
		if(((CButton *)GetDlgItem(IDC_CH_CLEAR_WIN))->GetCheck() == BST_CHECKED)//����
		{
			bIsClearAIWinMoney = true;
		}
		if(((CButton *)GetDlgItem(IDC_CH_CHANGE_SET))->GetCheck() == BST_CHECKED)///�޸�����
		{
			bAIWinAndLostAutoCtrl = false;
			bIsSetAICtrl = true;
			if(((CButton *)GetDlgItem(IDC_CH_SET_CTRL))->GetCheck() == BST_CHECKED)///��������
			{
				bAIWinAndLostAutoCtrl = true;
			}
		}
		CString str="";
		str.Format("������Ϣ��Ŀ��%d",m_GoalMoney);
		OutputDebugString(str);
		///�����޸���Ϣ
		this->GetParent()->SendMessage(IDM_SEND_AI_CTRL,0,0);
	}

}
//������Ҫ������Ϣѡ��
void CShowAIInfo::OnBnClickedChChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(((CButton *)GetDlgItem(IDC_CH_CHANGE))->GetCheck() == BST_CHECKED)///
	{
		((CButton *)GetDlgItem(IDC_CH_CLEAR_WIN))->EnableWindow(true);	///��ǰ����
		((CButton *)GetDlgItem(IDC_CH_CHANGE_SET))->EnableWindow(true);	///�޸�����
		((CButton *)GetDlgItem(IDC_BUT_SEND_CHANGE))->EnableWindow(true);	///������Ϣ��ť
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CH_CLEAR_WIN))->EnableWindow(false);	///��ǰ����
		((CButton *)GetDlgItem(IDC_CH_CHANGE_SET))->EnableWindow(false);	///�޸�����
		((CButton *)GetDlgItem(IDC_BUT_SEND_CHANGE))->EnableWindow(false);	///������Ϣ��ť

		((CButton *)GetDlgItem(IDC_CH_SET_CTRL))->EnableWindow(false);	///��������
		((CEdit *)GetDlgItem(IDC_EDIT_GOAL_MONEY))->EnableWindow(false);	///ӮǮĿ��
		((CEdit *)GetDlgItem(IDC_EDIT_AVE_MONEY))->EnableWindow(false);	///ӮǮƽ��ֵ

		((CButton *)GetDlgItem(IDC_CH_CLEAR_WIN))->SetCheck(BST_UNCHECKED);
		((CButton *)GetDlgItem(IDC_CH_CHANGE_SET))->SetCheck(BST_UNCHECKED);
	}

}
///�Ƿ�����ӮǮ
void CShowAIInfo::OnBnClickedChClearWin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
//�Ƿ��޸ĵ�ǰ����
void CShowAIInfo::OnBnClickedChChangeSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(((CButton *)GetDlgItem(IDC_CH_CHANGE_SET))->GetCheck() == BST_CHECKED)///
	{
		((CButton *)GetDlgItem(IDC_CH_SET_CTRL))->EnableWindow(true);	///��������
		((CEdit *)GetDlgItem(IDC_EDIT_GOAL_MONEY))->EnableWindow(true);	///ӮǮĿ��
		((CEdit *)GetDlgItem(IDC_EDIT_AVE_MONEY))->EnableWindow(true);	///ӮǮƽ��ֵ
	}
	else 
	{
		((CButton *)GetDlgItem(IDC_CH_SET_CTRL))->EnableWindow(false);	///��������
		((CEdit *)GetDlgItem(IDC_EDIT_GOAL_MONEY))->EnableWindow(false);	///ӮǮĿ��
		((CEdit *)GetDlgItem(IDC_EDIT_AVE_MONEY))->EnableWindow(false);	///ӮǮƽ��ֵ
	}

}

void CShowAIInfo::OnEnChangeEditGoalMoney()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(true);
}

void CShowAIInfo::OnEnChangeEditAveMoney()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(true);
}
