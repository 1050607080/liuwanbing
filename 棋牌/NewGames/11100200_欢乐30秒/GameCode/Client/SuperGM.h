#pragma once
#include "afxcmn.h"
#include "Resource.h"

// CSuperGM �Ի���

class CSuperGM : public CDialog
{
	DECLARE_DYNAMIC(CSuperGM)

public:
	CSuperGM(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSuperGM();

// �Ի�������
	enum { IDD = IDD_SUPERGM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchanged1071(NMHDR *pNMHDR, LRESULT *pResult);


public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedRadio1();
public:
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnBnClickedRadio3();
public:
	afx_msg void OnBnClickedRadio4();
public:
	afx_msg void OnBnClickedCheckRobotInfo();


public:
	CListCtrl m_ListCtrl;

	int		m_SelectedArea;

	bool	m_bShowRobotInfo;

	void UpdateListData(SUPERDATA *pSuperData);
	///���¿�����ʾ
	void UpdataCtrlText();
	///������ע��Ϣ
	void UpDataNoteRateInfo(__int64 iNtWin , __int64 iXianWin ,__int64 iHeWin) ; 
	///��ʾ����
	void ShowSuperWindow(bool show);
	///���Ϳ�����Ϣ
	void SendCtrlMsg();

	//���ÿ��ƽ��
	void SetCtrlResult(BYTE result);
	void OnSendDesktopXzInfo();
};
