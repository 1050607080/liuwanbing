#pragma once

#include "..\NewGameFrame\Client\UI_ForLogic\IUserInterface.h"

class BaseUI
{
private://����Ϊ˽�г�Ա���������Ϊ���г�Ա
	IUserInterface *m_pIUI;
	
	int m_iGameID;		//��ϷID

public:
	//���캯��
	BaseUI();
	virtual ~BaseUI();

public://�����ǹ��г�Ա����
	//���������֡������пؼ����õ��ϣ�------------------------------------------------------

	//����ָ��m_pIUI �������ϷID
	void SetpIUI(IUserInterface *pIUI, int iGameID);
	//���ָ��m_pIUI
	IUserInterface* GetpIUI();
	//����UI.data
	int Initial();
	//����UI.data
	int Initial(TCHAR *szDataFileName);
	//����UI.data
	int Initial(CString sFileName);
	//��ʾ/���� �ؼ�
	BOOL ShowControl(const int iID, bool bFlag);
	//��ѯ�ؼ��Ƿ���ʾ/����
	BOOL GetControlShow(const int iID);
	//���ÿؼ�����λ��
	BOOL SetControlGameXY(const int iID, int ix, int iy);
	//��ÿؼ�����λ��
	BOOL GetControlGameXY(const int iID, int &ix, int &iy);
	//���ÿؼ���С
	BOOL SetControlGameWH(const int iID, int iW, int iH);
	//��ÿؼ���С
	BOOL GetControlGameWH(const int iID, int &iW, int &iH);

	//����ť���֡�---------------------------------------------------------------------------

	//���ð�ť�ϵ���
	void SetTextForBut(const int iID, int num);
	//���ð�ť�ϵ���
	void SetTextForBut(const int iID, __int64 num);
	//���ð�ť�ϵ���2
	void SetTextForBut(const int iID, TCHAR Text[]);
	//���ÿ���/���� ��ť
	BOOL EnableControl(const int iID, bool bFlag);
	//��ѯ��ť����/����
	BOOL GetControlEnable(const int iID);
	//�Ƿ��ǰ�ť
	bool IsBut(const int iID);

	//��ͼƬ���֡�---------------------------------------------------------------------------

	//����ͼƬ�ؼ������ƶ���ͼƬ
	void SetImagesLoadPic(const int iID, TCHAR TcNickName[], bool bFlag);
	//����ͼƬ�Ƿ�͸
	void SetImagesPenetrate(const int iID, bool bTouFlag);
	//���ý���ʾͼƬ��������
	void SetShowImageXY(const int iID, bool bFlag = true, int ix = 0, int iy = 0);
	//��¡ͼƬ
	void CloneImage(const int iID, int iGoalID);

	//����̬�ı����֡�-----------------------------------------------------------------------

	//�����ı��ؼ���ʾ����(1)
	void SetTextInfo(const int iID, TCHAR TcNickName[], bool bFlag, FONTSHOWSTYLE showType = AlignmentLEFT);
	//�����ı��ؼ���ʾ����(2)
	void SetTextInfo(const int iID, int iNum, bool bFlag, FONTSHOWSTYLE showType = AlignmentLEFT);
	//�����ı��ؼ���ʾ����(3)
	void SetTextInfo(const int iID, __int64 i64Num, bool bFlag, bool bSymbol = false ,FONTSHOWSTYLE showType = AlignmentLEFT);

	//�õ��ı��ؼ�������
	wchar_t* GetTextString(const int iID);
	//�õ��ı��ռ������
	int GetTextNum(const int iID);

	//�����ֿؼ����֡�-----------------------------------------------------------------------

	//���ø������ֿؼ���ʾ
	void SetNumVisible(const int iID, __int64 byNum, bool bFlag);

	//����ʱ���ؼ����֡�---------------------------------------------------------------------

	//��ʾһ����ʱ���ؼ�
	void SetClock(const int iID, int iTime, bool bFlag = true);

	//���ı����벿�֡�-----------------------------------------------------------------------

	//��ȡ�ı����������
	void GetTextEditInfo(const int iID, wchar_t wText[], int iCount);
	//�����ı����������
	void SetTextEditInfo(const int iID, wchar_t wText[]);
	//�����ı���ֻ��������
	void SetTextEditOnlyNum(const int iID);
	//�����ı�����С���ֵ
	void SetTextMin_Max(const int iID, int iMin, int iMax);

	//���ƿؼ����֡�-------------------------------------------------------------------------
	
	//�����ƿؼ���ʾ
	void SetCardInfo(const int iID, BYTE iCard[], int iNum, bool bFlag);
	//��ȡ������˿�
	BYTE GetUpCard(const int iID, BYTE iCard[]);
	//����������˿�
	void SetUpCard(const int iID, BYTE iCard[], int inum);

	//�����벿�֡�---------------------------------------------------------------------------

	//��ʾ����ؼ�
	void SetCouMa(const int iID, int iNum, bool bFlag);

	//����ѡ��ѡ��ؼ���---------------------------------------------------------------------
	
	//�����Ƿ�ѡ��
	void SetIsSelect(const int iID, bool bFlag);
	//��ȡ�Ƿ�ѡ��
	bool GetIsSelect(const int iID);

	//�������ؼ����֡�(�ⲿ����)-------------------------------------------------------------

	//����ָ��������ʾָ����֡��
	void SetAnimateShowFram(const int iID, int iFramNum, bool bFlag);
	//���ò����ⲿ���� bLop-�Ƿ�ѭ�� iPlayCount-���Ŵ���(�ɼ�����Դ)
	void SetAnimatePlay(const int iID, TCHAR szAnimaPath[], bool bFlag, bool bLop = false, int iPlayCount = 1);
	//�����ⲿ����(���β���)
	void PlayAnimation(const int iID, bool bFlag = true, bool bIsLoopFlag = false);

	//�������ؼ����֡�(MoveAction)-----------------------------------------------------------

	//����ؼ��ƶ�����
	void PlayChipInAnima(const int iMoveActionID, const  int StarPointID, const  int EndPointID, const  int iBeMoveID, int bView, int iNum, int iTime, bool bFlag);
	//����ؼ��ƶ�����(Ӯ�һ����ҳ�����)
	void PlayChipInAnimaEx(const int iMoveActionID, const  int StarPointID, const  int EndPointID, const  int iBeMoveID, int bView, int iNum, int iTime, bool bFlag);
	//���ƶ���
	void PlayCardInAnima(const int iMoveActionID, const  int StarPointID, const  int EndPointID, const  int iBeMoveID, int bView, BYTE iCard[], int iNum, int iTime, bool bFlag);

	//�����ؼ����֡�-----------------------------------------------------------------------

	//���ù��
	void SetICursor(const wchar_t* pCurImg, bool bHide = true, int PointMode = 0, bool bFlag = true);
};

#include "BUI.inl"