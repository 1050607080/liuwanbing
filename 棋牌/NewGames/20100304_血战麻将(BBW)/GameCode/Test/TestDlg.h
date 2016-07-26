// TestDlg.h : ͷ�ļ�
//

#pragma once

//#include "ChangeFile/CGameDataEx.h"
//#include "ChangeFile/CUserDataEx.h"
//#include "ChangeFile/CCheckHuPaiEx.h"

#include "ChangeFile/CSrvLogicEx.h"


#include "commonuse.h"
#include "resource.h"		// ������

// CTestDlg �Ի���
class CTestDlg : public CDialog
{
// ����
public:
	CTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	BYTE m_byArHandPai[4][HAND_CARD_NUM];


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()



public:

	BYTE m_byNowStation;				//��ǰѡ����ҽ�
	BYTE m_byHandPai[4][HAND_CARD_NUM];	//��ҵ�����
	GCPStruct m_stCPGData[4][5];		//��ҳ���������

	BYTE m_byHuStation1;//�������1
	BYTE m_byHuStation2;//�������2
	BYTE m_byHuStation3;//�������3

	BYTE m_byDianPao;//�������
	BYTE m_byLastPai;//���õ�����

	BYTE m_byMainJing;//����
	BYTE m_byLessJing;//����

	bool m_bZimo;//�Ƿ���������

	CString m_sData;//�����ַ���


	BYTE m_bGameStation;//��Ϸ״̬

public:

	///������Ϣ��ӵ��б�ؼ���
	void AddPaiStrToCom(CComboBox *pCtrl);
	///��λ����Ϣ��ӵ��б�ؼ���
	void AddStationStrToCom(CComboBox *pCtrl);
	///��������������Ϣ��ӵ��б�ؼ���
	void AddCPGTypeStrToCom(CComboBox *pCtrl);

	///��ȡ�����б��Ƶ�����(������)
	int GetComboPaiData(CComboBox * pCtrl);
	///��ȡ�����б�λ�õ�����(������)
	int GetComboStationData(CComboBox * pCtrl);
	///��ȡ�����б�����������͵�����(������)
	int GetComboCPGTypeData(CComboBox * pCtrl);

	///���ݵ�ǰ�������ÿؼ�״̬
	void SetCtrlStatue();

	//���ַ���������������
	BYTE ChangeStrToData(CString st);

	//��ȡ��������
	void GetHuPaiType(CString & str,BYTE hupaitype[]);

public:
	afx_msg void OnBnClickedCancel();



	afx_msg void OnBnClickedButHupai();
	afx_msg void OnBnClickedButTingpai();
	afx_msg void OnCbnSelchangeCoStCh();
	afx_msg void OnCbnSelchangeCoMainjing();
	afx_msg void OnCbnSelchangeCoLessjing();
	afx_msg void OnEnChangeEditData();
	afx_msg void OnBnClickedButMoreTest();
	afx_msg void OnBnClickedCheckDiaopao();



	CSrvLogicEx		m_SrvLogic;//�������߼�

	bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);

    ///������Ϸ״̬
    void SendGameStationEx(BYTE bDeskStation, bool bWatchUser, void * pData, UINT uSize);
    ///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
    void SendGameDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///�����Թ����� ��������Ϣ���Թ��ߣ�
    void SendWatchDataEx(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
    void SendGameDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
    ///�����Թ����� ��������Ϣ���Թ��ߣ�
    void SendWatchDataEx(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);


    void KillTimer(int nTimerID);
    void SetTimer(int nID, int nTime);

	//��ȡ��Ϸ״̬
	BYTE GetGameStation(){return m_bGameStation;};
	//������Ϸ״̬
	void SetGameStation(BYTE state){m_bGameStation = state;};
	//��ȡ��ҵ�ͬ��״̬
	void GetUserAgree(BYTE agree);
	//�Ƿ�����Ϸ��
	bool IsPlayGame(int bDeskStation);

};
