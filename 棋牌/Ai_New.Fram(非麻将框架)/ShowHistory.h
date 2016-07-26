#pragma once


// CShowHistory �Ի���

class CShowHistory : public CDialog
{
	DECLARE_DYNAMIC(CShowHistory)

	int				m_iX,m_iY;
	int				m_iStartLine,m_iEndLine;
	TCHAR			m_szMsg[MAX_PATH];
public:
	CShowHistory(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowHistory();

// �Ի�������
	enum { IDD = IDD_SHOW_UPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
};
