#pragma once
#include "Resource.h"

// CSuperSettingDlg �Ի���

class CSuperSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSuperSettingDlg)

public:
	CSuperSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSuperSettingDlg();

// �Ի�������
	enum { IDD = IDD_SUPER_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};
