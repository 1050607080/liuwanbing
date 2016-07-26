// Result.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Result.h"


// CResult �Ի���

IMPLEMENT_DYNAMIC(CResult, CDialog)

CResult::CResult(CWnd* pParent /*=NULL*/)
	: CDialog(CResult::IDD, pParent)
{
	TCHAR szName[MAX_PATH];
	wsprintf(szName,TEXT(".\\image\\result.bmp"),SKIN_FOLDER);//lu���뱳��ͼ
	m_img.SetLoadInfo(szName,CGameImageLink::m_bAutoLock);
}

CResult::~CResult()
{
}

void CResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResult, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CResult ��Ϣ�������
BOOL CResult::OnInitDialog()
{
	CDialog::OnInitDialog();
#ifndef FINISH_RECORD
  	;;SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
 	//-------------------------����͸��----------------------
	HINSTANCE hInst = LoadLibrary(TEXT("User32.DLL"));
	if(hInst != NULL) 
	{ 
	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//ȡ��SetLayeredWindowAttributes����ָ�� 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),0,220,2); 
		FreeLibrary(hInst); 
	}
#endif
	CGameImageHelper help(&m_img);
	HRGN hRgn=AFCBmpToRgn(help,RGB(255,0,255),RGB(1,1,1));     //���˵�RGB(255,0,255)����ɫ����
	if (hRgn!=NULL)
	{
 		SetWindowRgn(hRgn,TRUE);        //����Ϊû�й��˵�������
	}
	CenterWindow();
	return true;
}

void CResult::OnPaint()
{
	CPaintDC dc(this); // device context for painting
//	// TODO: �ڴ˴������Ϣ����������
//	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
//	CFont font;
//	font.CreateFont(-13,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
//	dc.SelectObject(&font);
//	CDC srcdc;
//	CGameImageHelper help(&m_img);
//	srcdc.CreateCompatibleDC(&dc);
//	srcdc.SelectObject(help);//luճ������ͼ
// 	::TransparentBlt(dc.GetSafeHdc(),0,0,help.GetWidth(),help.GetHeight(),srcdc,0,0,help.GetWidth(),help.GetHeight(),srcdc.GetPixel(0,0));
//	dc.SetBkMode(TRANSPARENT);
//	CRect rectName(0,0,0,0);
//
//	if(m_iMeStation == m_iNtStation)//�Լ���ׯ��
//	{
//		dc.SetTextColor(RGB(255,0,0));//�Լ��ú�ɫ
//		CString strname="",str="";
//		//�������
//		rectName.SetRect(11,54,57,76);
//		strname="�Լ�";
//		dc.DrawText(strname,strname.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		//����Ƶ�
//		rectName.SetRect(104,54,160,76);
//		str.Format("%d",m_iPaiDian[0]);
//		dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		//�������
//		rectName.SetRect(56,54,104,76);
//		if(m_iZPaiXing[0]>0)
//		{
//			if(m_iZPaiXing[1]>0)
//			{
//				str="��������";
//			}
//			else 
//			{
//				str="����";
//			}
//			dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		}
//		else if(m_iZPaiXing[1]>0)
//		{
//			str="�� ��";
//			dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		}  
//
//		//������
//		rectName.SetRect(160,54,273,76);
//		str.Format("%d",m_iMeWinMoney);
//		dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//
////////////////////////////////////////////////////////////////
//		dc.SetTextColor(RGB(255,255,102));//����ý�ɫ
//
//		rectName.SetRect(11,76,57,97);
//		strname="�м�";
//		dc.DrawText(strname,strname.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		//����Ƶ�
//		rectName.SetRect(104,76,160,97);
//		str.Format("%d",m_iPaiDian[1]);
//		dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		//�������
//		rectName.SetRect(56,76,104,97);
//		if(m_iXPaiXing[0]>0)
//		{
//			if(m_iXPaiXing[1]>0)
//			{
//				str="��������";
//			}
//			else 
//			{
//				str="����";
//			}
//			dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		}
//		else if(m_iXPaiXing[1]>0)
//		{
//			str="�� ��";
//			dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		}  
//
//		//������
//		rectName.SetRect(160,76,273,97);
//		str.Format("%d",m_iXianMoney);
//		dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//	}
//	else 
//	{
//		dc.SetTextColor(RGB(255,0,0));//�Լ��ú�ɫ
//		CString strname="",str="";
//		//�������
//		rectName.SetRect(11,54,57,76);
//		strname="�Լ�";
//		dc.DrawText(strname,strname.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		//����Ƶ�
//		rectName.SetRect(104,54,160,76);
//		str.Format("%d",m_iPaiDian[1]);
//		dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		//�������
//		rectName.SetRect(56,54,104,76);
//		if(m_iXPaiXing[0]>0)
//		{
//			if(m_iXPaiXing[1]>0)
//			{
//				str="��������";
//			}
//			else 
//			{
//				str="����";
//			}
//			dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		}
//		else if(m_iXPaiXing[1]>0)
//		{
//			str="�� ��";
//			dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		}  
//
//		//������
//		rectName.SetRect(160,54,273,76);
//		str.Format("%d",m_iMeWinMoney);
//		dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//
//
////////////////////////////////////////////////////////////////
//		dc.SetTextColor(RGB(255,255,102));//����ý�ɫ
//
//		rectName.SetRect(11,76,57,97);
//		strname="ׯ��";
//		dc.DrawText(strname,strname.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		//����Ƶ�
//		rectName.SetRect(104,76,160,97);
//		str.Format("%d",m_iPaiDian[0]);
//		dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		//�������
//		rectName.SetRect(56,76,104,97);
//		if(m_iZPaiXing[0]>0)
//		{
//			if(m_iZPaiXing[1]>0)
//			{
//				str="��������";
//			}
//			else 
//			{
//				str="����";
//			}
//			dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		}
//		else if(m_iZPaiXing[1]>0)
//		{
//			str="�� ��";
//			dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//		}  
//
//		//������
//		rectName.SetRect(160,76,273,97);
//		str.Format("%d",m_iNtWinMoney);
//		dc.DrawText(str,str.GetLength(),&rectName,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
//	}

}
//��������
void CResult::ClearAll()
{
	m_iNtStation = -1;//ׯ��λ��
	m_iMeStation = -1;//�Լ���λ��
	m_iNtWinMoney = 0;//ׯӮ��Ǯ
	m_iXianMoney = 0;//��Ӯ��Ǯ
	m_iMeWinMoney = 0;//�Լ�Ӯ��Ǯ
	m_iPaiDian[0] = 0;  //ׯ���Ƶ�
	m_iPaiDian[1] = 0;  //�е��Ƶ�

	m_iZPaiXing[0] = 0; //ׯ�ҵ�����
	m_iZPaiXing[1] = 0; //ׯ�ҵ�����

	m_iXPaiXing[0] = 0; //�мҵ�����
	m_iXPaiXing[1] = 0; //�мҵ�����
}