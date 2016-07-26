#pragma once
#include "Resource.h"

/// Clookinfo �Ի���,��ʾ·����Ϣ
/// �����ַ�ʽ��ʾ·����Ϣ
class Clookinfo : public CDialog
{
	DECLARE_DYNAMIC(Clookinfo)

public:
	Clookinfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Clookinfo();

// �Ի�������
	enum { IDD = IDD_LUZI };

	bool			bMouseDown;
public:
	/// ����ͼ
	CGameImage				m_bk;				
	//CNormalBitmapButtonEx   m_BmCancel;			// ��ť
	//CNormalBitmapButtonEx   m_BmOk;				// ��ť

	//////////////////////////////////////////////////////////
	CGameImage  m_imgheduizi;
	CGameImage  m_imgzhuangduizi;
	CGameImage  m_imgxianduizi;

	CGameImage  m_imghe1_dot;
	CGameImage  m_imgzhuang1_dot;
	CGameImage  m_imgxian1_dot;



	/////////////////////////////////////////////////////////
	/// ׯСͼ
	CGameImage				m_imgzhuang;			
	/// ׯ��ͼ
	CGameImage				m_imgzhuang1;		
	/// ��Сͼ
	CGameImage				m_imgxian;			
		/// �д�ͼ
	CGameImage				m_imgxian1;		
	/// ��Сͼ
	CGameImage				m_imghe;			
	/// �ʹ�ͼ
	CGameImage				m_imghe1;			

	///Ϊ72��һ������Ӱ�ťͼƬ/// ��һҳ
	CNormalBitmapButtonEx				m_BtnYiye;				
	/// �ڶ�ҳͼƬ
	CNormalBitmapButtonEx				m_BtnErye;				
	/// �رհ�ť
	CNormalBitmapButtonEx				m_BtnClose;			

	///��ͼ����λ��
	CPoint      m_pDaTu;
	///Сͼ����λ��
	CPoint		m_pXiaoTu;

	///��ͼ����
	int         m_iDaWidth;
	///Сͼ����
	int         m_iXiaoWidth;

	///ׯ��Ӯ��Ϊ������������Ŵ�100����������ǰ����Ϊ��С�������
	int			m_iZwin;
	///�м�Ӯ
	int			m_iXwin;
	///��Ӯ
	int			m_iHwin;
	///ׯ����
	int			m_iZDuizi;
	///�ж���
	int         m_iXDuizi;
	///ׯ����
	int			m_iZTianwan;
	///������
	int			m_iXTianwang;
	///������Ϸ���ܾ�����>=1 ���� <=30��
	int			m_iPlayCount;
	///������Ƶ�Ӯ����0û��ʹ�õ����ݣ�1��ׯ��2���У�3��
	int			m_iWinType[MAXCOUNT];
	///С·�ӵ�ÿһ������
	BYTE		m_bySLuziData[72][6];
	///�Ƿ���ʾ��һҳ��ͼƬ
	bool		m_bShowNext;

	int			m_iNowJuShu;				//���ֽ��еľ��������е��ڼ��֣�72��һ�֣�

	int         m_iGamePictureType; 

	//��ǰ�Ѿ��ж������Ѿ���ֵ��
	int         m_iLuziCol;


	int		m_iZPaiXing[5];			///<ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
	int		m_iXPaiXing[5];			///<�м�����
	
	///������Ƶ�Ӯ�����Ƿ񿪵��Ƕ���
	///С·�ӵ�ÿһ������
	int		m_bKaiPaiData[72][6];
	int     m_iKaiPaiType[MAXCOUNT];

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	///��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
//	afx_msg void OnBnClickedOk();
//public:
//	afx_msg void OnBnClickedCancel();
public:
//	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	///��ʼ������
	void Init();
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	///����·��
	void DrawLuzi(CDC*pDC);
	///����ת��,��·������ת����������ʽlym2010-03-18
	void ChangeData();

public:
	afx_msg void OnBnClickedButYiye();
public:
	afx_msg void OnBnClickedButErye();
public:
	afx_msg void OnBnClickedButClose();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
