#pragma once

class CConfigWnd
{
public:
	CConfigWnd(void);
	~CConfigWnd(void);
	//λ��X����
	int m_ptx;
	//λ��Y����
	int m_pty;
	//�Ƿ���ʾ
	bool m_isvisable;

	//��ʼ��
	void InitDlg(int x,int y);

	void  LMouseDown(int ptx,int pty);
	void  LMouseUp(int ptx,int pty);
	void  LMouseMove(int ptx,int pty);
	float   m_value1;
	float   m_value2;
	int     m_ptx1;
	int     m_ptx2;
	int   m_oldptx;
	bool  m_s1;
	bool  m_s2;

	//��Ϸ��Ч����
	bool	m_bBgSound;
	bool	m_bGameSound;
	bool	m_bShakeWindow;
	bool	m_bTipMsg;			//�Ƿ������ʾ
};
