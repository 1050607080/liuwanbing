#include <atlstr.h>
#include "BUI.h"

#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\

//���캯��
BaseUI::BaseUI()
{
	return;
}

BaseUI::~BaseUI()
{
	return;
}

//���������֡������пؼ����õ��ϣ�------------------------------------------------------

//����ָ��m_pIUI �������ϷID
void BaseUI::SetpIUI(IUserInterface *pIUI, int iGameID)
{
	m_pIUI = pIUI;
	m_iGameID = iGameID;
	return;
}

//���ָ��m_pIUI
IUserInterface* BaseUI::GetpIUI()
{
	return m_pIUI;
}

/*************************************************
*Function: ����UI.data��������Ҫ������ʼ��һЩ��Ϸ�����еĽ�����ʾ�������ȵȡ�
*writer:
*		˧����
*Parameters:
*		void
*Return:
*		�����Ƿ�ɹ�
*************************************************/
int BaseUI::Initial()
{
	if (m_pIUI == NULL)
	{
		return -1;
	}
	wchar_t wszUIFile[MAX_PATH];
#ifdef UI2_5D
	::swprintf_s(wszUIFile,L"%d_UI_3D.dat",NAME_ID);
#else
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
#endif
	m_pIUI->LoadData(wszUIFile);
	return 1;
}

/*************************************************
*Function: ����UI.data��������Ҫ������ʼ��һЩ��Ϸ�����еĽ�����ʾ�������ȵȡ�
*writer:
*		˧����
*Parameters:
*		[TCHAR*]
*						szDataFileName UI.data���ļ���
*Return:
*		�����Ƿ�ɹ�
*************************************************/
int BaseUI::Initial(TCHAR *szDataFileName)
{
	if ((m_pIUI == NULL)
		|| (szDataFileName == NULL))
	{
		return -1;
	}
	TCHAR pchar[MAX_PATH] = {0};
	sprintf(pchar, "%s", szDataFileName);
	m_pIUI->LoadData(CA2W(pchar));
	return 1;
}

//����UI.data
int BaseUI::Initial(CString sFileName)
{
	if ((m_pIUI == NULL))
	{
		return -1;
	}
	m_pIUI->LoadData(CA2W(sFileName));
	return 1;
}

/*************************************************
*Function: ��ʾ/���� �ؼ�
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[int]
*						bFlag	�Ƿ���ʾ
*Return:
*		�����Ƿ�ɹ�
*************************************************/
BOOL BaseUI::ShowControl(const int iID, bool bFlag)
{
	IBCInterFace *pControl = NULL;
	GETCTRL(IBCInterFace,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->SetControlVisible(bFlag);
		return TRUE;
	}

	return FALSE;
}

/*************************************************
*Function: ��ѯ�ؼ��Ƿ���ʾ/����
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[int]
*						bFlag	�Ƿ���ʾ
*Return:
*		�����Ƿ�ɹ�
*************************************************/
BOOL BaseUI::GetControlShow(const int iID)
{
	IBCInterFace *pControl = NULL;
	GETCTRL(IBCInterFace,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		return pControl->GetVisible();
	}

	return FALSE;
}

/*************************************************
*Function: ���ÿؼ�����λ��
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[int]
*						ix,iy	����λ��
*Return:
*		�����Ƿ�ɹ�
*************************************************/
BOOL BaseUI::SetControlGameXY(const int iID, int ix, int iy)
{
	IBCInterFace *pControl = NULL;
	GETCTRL(IBCInterFace,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->SetGameXY(ix, iy);
		return TRUE;
	}

	return FALSE;
}

/*************************************************
*Function: ��ÿؼ�����λ��
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[int]
*						ix,iy	����λ��
*Return:
*		�����Ƿ�ɹ�
*************************************************/
BOOL BaseUI::GetControlGameXY(const int iID, int &ix, int &iy)
{
	ix = 0;
	iy = 0;

	IBCInterFace *pControl = NULL;
	GETCTRL(IBCInterFace,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		ix = pControl->GetGameX();
		iy = pControl->GetGameY();
		return TRUE;
	}

	return FALSE;
}

/*************************************************
*Function: ���ÿؼ���С
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[int]
*						iW,iH	�ؼ���͸�
*Return:
*		�����Ƿ�ɹ�
*************************************************/
BOOL BaseUI::SetControlGameWH(const int iID, int iW, int iH)
{
	IBCInterFace *pControl = NULL;
	GETCTRL(IBCInterFace,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->SetGameW(iW);
		pControl->SetGameH(iH);
		return TRUE;
	}

	return FALSE;
}

/*************************************************
*Function: ��ÿؼ���С
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[int]
*						iW,iH	�ؼ���͸�
*Return:
*		�����Ƿ�ɹ�
*************************************************/
BOOL BaseUI::GetControlGameWH(const int iID, int &iW, int &iH)
{
	iW = 0;
	iH = 0;

	IBCInterFace *pControl = NULL;
	GETCTRL(IBCInterFace,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		iW = pControl->GetGameW();
		iH = pControl->GetGameH();
		return TRUE;
	}

	return FALSE;
}

//����ť���֡�---------------------------------------------------------------------------

/*************************************************
*Function: ���ð�ť�ϵ���
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[int]
*						num		����
*Return:
*		void
*************************************************/
void BaseUI::SetTextForBut(const int iID, int num)
{
	IButton *pControl = NULL;
	GETCTRL(IButton,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		TCHAR pchar[MAX_PATH] = {0};
		sprintf(pchar, "%d",num);
		pControl->SetText(CA2W(pchar));
	}

	return;
}

/*************************************************
*Function: ���ð�ť�ϵ���
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[int]
*						num		����
*Return:
*		void
*************************************************/
void BaseUI::SetTextForBut(const int iID, __int64 num)
{
	IButton *pControl = NULL;
	GETCTRL(IButton,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		TCHAR pchar[MAX_PATH] = {0};
		sprintf(pchar, "%I64d",num);
		pControl->SetText(CA2W(pchar));
	}

	return;
}

/*************************************************
*Function: ���ð�ť�ϵ���2
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[TCHAR]
*						Text[]	�ַ���
*Return:
*		void
*************************************************/
void BaseUI::SetTextForBut(const int iID, TCHAR Text[])
{
	IButton *pControl = NULL;
	GETCTRL(IButton,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->SetText(CA2W(Text));
	}

	return;
}

/*************************************************
*Function: ���ÿ���/���� ��ť
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*		[bool]
*						bFlag	�Ƿ�ʹ��
*Return:
*		�����Ƿ�ɹ�
*************************************************/
BOOL BaseUI::EnableControl(const int iID, bool bFlag)
{
	IButton *pControl = NULL;
	GETCTRL(IButton,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->SetEnable(bFlag);
		return TRUE;
	}

	return FALSE;
}

/*************************************************
*Function: ��ѯ��ť����/����
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*Return:
*		�����Ƿ�ɹ�
*************************************************/
BOOL BaseUI::GetControlEnable(const int iID)
{
	IButton *pControl = NULL;
	GETCTRL(IButton,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		return pControl->GetEnable();
	}

	return FALSE;
}

/*************************************************
*Function: �Ƿ��ǰ�ť
*writer:
*		˧����
*Parameters:
*		[int]
*						iID		�ؼ�ID
*Return:
*		�����Ƿ��ǰ�ť
*************************************************/
bool BaseUI::IsBut(const int iID)
{
	IButton *pControl = NULL;
	GETCTRL(IButton,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		return true;
	}
	return false;
}

//��ͼƬ���֡�---------------------------------------------------------------------------

/*************************************************
*Function: ����ͼƬ�ؼ������ƶ���ͼƬ
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[TCHAR]
*						TcNickName[]	·��
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::SetImagesLoadPic(const int iID, TCHAR TcNickName[], bool bFlag)
{
	IImage *pControl = NULL;
	GETCTRL(IImage,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->LoadPic(CA2W(TcNickName));
		pControl->SetControlVisible(bFlag);
	}

	return;
}

/*************************************************
*Function: ����ͼƬ�Ƿ�͸
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[bool]
*						bTouFlag		�Ƿ�͸
*Return:
*		void
*************************************************/
void BaseUI::SetImagesPenetrate(const int iID, bool bTouFlag)
{
	IImage *pControl = NULL;
	GETCTRL(IImage,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->SetMouseThough(bTouFlag);
	}

	return;
}

/*************************************************
*Function: ���ý���ʾͼƬ��������
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[bool]
*						bFlag			�Ƿ���ʾ
*		[int]
*						ix,iy			����ͼƬ��ʾ��С
*Return:
*		void
*************************************************/
void BaseUI::SetShowImageXY(const int iID, bool bFlag, int ix, int iy)
{
	IImage *pControl = NULL;
	GETCTRL(IImage,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->SetSrcX(ix);
		pControl->SetSrcY(iy);
		pControl->SetControlVisible(bFlag);
	}

	return;
}

/*************************************************
*Function: ��¡ͼƬ
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[int]
*						iGoalID			��¡�Ŀؼ�ID
*Return:
*		void
*************************************************/
void BaseUI::CloneImage(const int iID, int iGoalID)
{
	IImage *_pControl = NULL;
	GETCTRL(IImage,_pControl,m_pIUI,iID);
	IImage *pControl = NULL;
	if (NULL != _pControl)
	{
		pControl = dynamic_cast<IImage *>(_pControl->Clone(iGoalID));//��¡
		if (NULL != pControl)
		{
			if (NULL == m_pIUI->GetIControlbyID(pControl->GetGameControlID()))
			{
				m_pIUI->IInsertControl(pControl);
			}
		}
	}

	return;
}

//����̬�ı����֡�-----------------------------------------------------------------------

/*************************************************
*Function: �����ı��ؼ���ʾ����(1)
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[TCHAR]
*						TcNickName[]	�ַ���
*		[bool]
*						bFlag			�Ƿ���ʾ
*		[FONTSHOWSTYLE]
*						showType		�������ģʽ
*Return:
*		void
*************************************************/
void BaseUI::SetTextInfo(const int iID, TCHAR TcNickName[], bool bFlag, FONTSHOWSTYLE showType)
{
	IText *pControl = NULL;
	GETCTRL(IText,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		pControl->SetText(CA2W(TcNickName));
		pControl->SetFontShowStyle(showType);
		pControl->SetControlVisible(bFlag);
	}

	return;
}

/*************************************************
*Function: �����ı��ؼ���ʾ����(2)
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[int]
*						iNum			����
*		[bool]
*						bFlag			�Ƿ���ʾ
*		[FONTSHOWSTYLE]
*						showType		�������ģʽ
*Return:
*		void
*************************************************/
void BaseUI::SetTextInfo(const int iID, int iNum, bool bFlag, FONTSHOWSTYLE showType)
{
	IText *pControl = NULL;
	GETCTRL(IText,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		TCHAR pchar[MAX_PATH] = {0};
		sprintf(pchar, TEXT("%d"), iNum);

		pControl->SetText(CA2W(pchar));
		pControl->SetFontShowStyle(showType);
		pControl->SetControlVisible(bFlag);
	}

	return;
}

/*************************************************
*Function: �����ı��ؼ���ʾ����(3)
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[__int64]
*						i64Num			����
*		[bool]
*						bFlag			�Ƿ���ʾ
*		[FONTSHOWSTYLE]
*						showType		�������ģʽ
*Return:
*		void
*************************************************/

void BaseUI::SetTextInfo(const int iID, __int64 i64Num, bool bFlag, bool bSymbol, FONTSHOWSTYLE showType)
{
	IText *pControl = NULL;
	GETCTRL(IText,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		TCHAR pchar[MAX_PATH] = {0};
		//�Ƿ��+-����
		if (bSymbol&&i64Num>0)
		{
			sprintf(pchar, TEXT("+%I64d"), i64Num);
		}
		else
		{
			sprintf(pchar, TEXT("%I64d"), i64Num);
		}
		

		pControl->SetText(CA2W(pchar));
		pControl->SetFontShowStyle(showType);
		pControl->SetControlVisible(bFlag);
	}

	return;
}

/*************************************************
*Function: �õ��ı��ؼ�������
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*Return:
*		�����ַ���
*************************************************/
wchar_t* BaseUI::GetTextString(const int iID)
{
	IText *pControl = NULL;
	GETCTRL(IText,pControl,m_pIUI,iID);
	if (NULL != pControl)
	{
		return pControl->GetText();
	}

	return NULL;
}

/*************************************************
*Function: �õ��ı��ؼ�������
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*Return:
*		�����ַ���
*************************************************/
int BaseUI::GetTextNum(const int iID)
{
	wchar_t* pWchar = GetTextString(iID);

	return _wtoi(pWchar);
}

//�����ֿؼ����֡�-----------------------------------------------------------------------

/*************************************************
*Function: ���ø������ֿؼ���ʾ
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[__int64]
*						byNum			����
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::SetNumVisible(const int iID, __int64 byNum, bool bFlag)
{
	INum *pControl = NULL;
	GETCTRL(INum, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetNum(byNum);
		pControl->SetControlVisible(bFlag);
	}
	return;
}

//����ʱ���ؼ����֡�---------------------------------------------------------------------

/*************************************************
*Function: ��ʾһ����ʱ���ؼ�
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[int]
*						iTime			ʱ��
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::SetClock(const int iID, int iTime, bool bFlag)
{
	ITime *pControl = NULL;
	GETCTRL(ITime, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetShowMaxNum(iTime);
		pControl->SetShowMinNum(0);
		pControl->SetPLay(bFlag);
		pControl->SetControlVisible(bFlag);
	}
	return;
}

//���ı����벿�֡�-----------------------------------------------------------------------

/*************************************************
*Function: ��ȡ�ı����������
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[wchar_t]
*						wText[]			�ַ���
*		[int]
*						iCount			�ַ�������
*Return:
*		void
*************************************************/
void BaseUI::GetTextEditInfo(const int iID, wchar_t wText[], int iCount)
{
	wchar_t* Data = NULL;
	memset(wText, 0, sizeof(wText));

	IEditorText* pControl = NULL;
	GETCTRL(IEditorText, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		Data = pControl->GetText();

		if (Data != NULL)
		{
			memcpy(wText, Data, iCount * sizeof(wchar_t));
		}
	}

	return;
}

/*************************************************
*Function: �����ı����������
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[wchar_t]
*						wText[]			�ַ���
*Return:
*		void
*************************************************/
void BaseUI::SetTextEditInfo(const int iID, wchar_t wText[])
{
	memset(wText, 0, sizeof(wText));
	IEditorText* pControl = NULL;
	GETCTRL(IEditorText, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetText(wText);
	}

	return;
}

/*************************************************
*Function: �����ı���ֻ��������
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*Return:
*		void
*************************************************/
void BaseUI::SetTextEditOnlyNum(const int iID)
{
	IEditorText* pControl = NULL;
	GETCTRL(IEditorText, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetEnableASCII(1, '0', '9');
	}

	return;
}

/*************************************************
*Function: �����ı�����С���ֵ
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[int]
*						iMin			��Сֵ
*		[int]
*						iMax			���ֵ
*Return:
*		void
*************************************************/
void BaseUI::SetTextMin_Max(const int iID, int iMin, int iMax)
{
	IEditorText* pControl = NULL;
	GETCTRL(IEditorText, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetNumberLimit(true, iMin, iMax);
	}

	return;
}

//���ƿؼ����֡�-------------------------------------------------------------------------

/*************************************************
*Function: �����ƿؼ���ʾ
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[BYTE]
*						iCard[]			������
*		[int]
*						iNum			����ֵ
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::SetCardInfo(const int iID, BYTE iCard[], int iNum, bool bFlag)
{
	ICardBase *pControl = NULL;
	GETCTRL(ICardBase, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetCardList(iCard, iNum);
		pControl->SetControlVisible(bFlag);
	}

	return;
}

/*************************************************
*Function: ��ȡ������˿�
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[BYTE]
*						iCard[]			������
*Return:
*		��������Ŀ
*************************************************/
BYTE BaseUI::GetUpCard(int iID, BYTE iCard[])
{
	BYTE bUpCount=0;
	IOperateCard *pControl = NULL;
	GETCTRL(IOperateCard, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		return pControl->GetUpCardList(iCard);
	}
	return 0;
}

/*************************************************
*Function: ����������˿�
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[BYTE]
*						iCard[]			������
*		[int]
*						inum			����Ŀ
*Return:
*		��������Ŀ
*************************************************/
void BaseUI::SetUpCard(int iID, BYTE iCard[], int inum)
{
	IOperateCard *pControl = NULL;
	GETCTRL(IOperateCard, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetUpCardList(iCard, inum);
	}
	return;
}

//�����벿�֡�---------------------------------------------------------------------------

/*************************************************
*Function: ��ʾ����ؼ�
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[int]
*						iNum			����ֵ
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::SetCouMa(const int iID, int iNum, bool bFlag)
{
	IShCtrl *pControl = NULL;
	GETCTRL(IShCtrl, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetShNumber(9);
		pControl->SetShNoteType(4);
		pControl->SetShNote(iNum);
		pControl->SetControlVisible(bFlag);
	}

	return;
}

//����ѡ��ѡ��ؼ���---------------------------------------------------------------------

/*************************************************
*Function: �����Ƿ�ѡ��
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*						bFlag			�Ƿ�ѡ��
*Return:
*		void
*************************************************/
void BaseUI::SetIsSelect(const int iID, bool bFlag)
{
	IRadioButton *pControl = NULL;
	GETCTRL(IRadioButton, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetIsSelect(bFlag);
	}

	return;
}

/*************************************************
*Function: ��ȡ�Ƿ�ѡ��
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*Return:
*		�Ƿ�ѡ��
*************************************************/
bool BaseUI::GetIsSelect(const int iID)
{
	IRadioButton *pControl = NULL;
	GETCTRL(IRadioButton, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		return pControl->GetIsSelect();
	}

	return false;
}

//�������ؼ����֡�(�ⲿ����)-------------------------------------------------------------

/*************************************************
*Function: ����ָ��������ʾָ����֡��
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[int]
*						iFramNum		�����ڼ�֡
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::SetAnimateShowFram(const int iID, int iFramNum, bool bFlag)
{
	IExAnimate *pControl = NULL;
	GETCTRL(IExAnimate, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetIsLoop(false);
		pControl->SetCurveInstance(CT_NONE);
		pControl->SetPlayState(false);
		//�ڼ�֡
		pControl->SetShowFrame(iFramNum);
		pControl->SetControlVisible(bFlag);
	}
	return;
}

/*************************************************
*Function: ���ò����ⲿ���� bLop-�Ƿ�ѭ�� iPlayCount-���Ŵ���(�ɼ�����Դ)
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[TCHAR]
*						szAnimaPath		������Դ·��
*		[bool]
*						bFlag			�Ƿ���ʾ
*		[bool]
*						bLop			�Ƿ�ѭ��
*		[int]
*						iPlayCount		���Ŵ���
*Return:
*		void
*************************************************/
void BaseUI::SetAnimatePlay(const int iID, TCHAR szAnimaPath[], bool bFlag, bool bLop, int iPlayCount)
{
	IExAnimate *pControl = NULL;
	GETCTRL(IExAnimate, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetExtFilePath(CA2W(szAnimaPath));
		if (pControl->LoadFromFile())
		{
			pControl->SetControlVisible(bFlag);
			pControl->SetCurveInstance(CT_NONE);//���ƶ�
			pControl->SetIsLoop(bLop);
			if (!bLop)
			{
				pControl->SetPlayNum(iPlayCount);
			}
			pControl->SetPlayState(bFlag);
		}

	}

	return;
}

/*************************************************
*Function: �����ⲿ����(���β���)
*writer:
*		˧����
*Parameters:
*		[int]
*						iID				�ؼ�ID
*		[bool]
*						bFlag			�Ƿ���ʾ
*		[bool]
*						bIsLoopFlag		�Ƿ�ѭ��
*Return:
*		void
*************************************************/
void BaseUI::PlayAnimation(const int iID, bool bFlag, bool bIsLoopFlag)
{
	IExAnimate *pControl = NULL;
	GETCTRL(IExAnimate, pControl, m_pIUI, iID);
	if (NULL != pControl)
	{
		pControl->SetIsLoop(bIsLoopFlag);
		pControl->SetControlVisible(bFlag);
		pControl->SetPlayState(bFlag);//�����Ƿ�ʼ����
	}

	return;
}

//�������ؼ����֡�(MoveAction)-----------------------------------------------------------

/*************************************************
*Function: ����ؼ��ƶ�����
*writer:
*		˧����
*Parameters:
*		[int]
*						iMoveActionID	MoveAction�ؼ�ID
*		[int]
*						StarPointID		��ʼ��ؼ�ID
*		[int]
*						EndPointID		��ֹ��ؼ�ID
*		[int]
*						iBeMoveID		���ƶ��ؼ�ID
*		[int]
*						bView			�����ͼλ��
*		[int]
*						iNum			����
*		[int]
*						iNum			����ʱ�䣨���룩
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::PlayChipInAnima(const int iMoveActionID, const  int StarPointID, const  int EndPointID, const  int iBeMoveID, int bView, int iNum, int iTime, bool bFlag)
{
	if (bFlag)
	{
		//��ʼλ��
		POINT StarPoint;
		POINT EndPoint;
		ICardBase * pICardBase = NULL;
		IShCtrl * pShCtrl1 = NULL;

		GETCTRL(ICardBase, pICardBase, m_pIUI, StarPointID + bView);//��ÿ���˵�����λ��Ϊ���
		if (NULL != pICardBase)
		{
			StarPoint.x = pICardBase->GetGameX();
			StarPoint.y = pICardBase->GetGameY();
		}

		GETCTRL(IShCtrl, pShCtrl1, m_pIUI, EndPointID);//�����ĳ���λ��Ϊ�յ�
		if (NULL != pShCtrl1)
		{
			EndPoint.x = pShCtrl1->GetGameX();
			EndPoint.y = pShCtrl1->GetGameY();
		}

		IShCtrl * pShCtrl = NULL;
		GETCTRL(IShCtrl, pShCtrl, m_pIUI, iBeMoveID + bView);
		if (NULL != pShCtrl)
		{
			pShCtrl->SetShNote(iNum);
			pShCtrl->SetGameXY(StarPoint.x, StarPoint.y);
			pShCtrl->SetControlVisible(true);
		}

		IMoveAction* pMoveAction = NULL;
		GETCTRL(IMoveAction, pMoveAction, m_pIUI, iMoveActionID + bView);//�ƶ�����
		if (NULL != pMoveAction)
		{
			pMoveAction->SetControlingCtrlID(iBeMoveID + bView);
			pMoveAction->SetCurveInstance(CT_STRAIGHT);

			pMoveAction->SetStartP(StarPoint);
			pMoveAction->SetEndP(EndPoint);
			pMoveAction->SetTotalMoveTime(iTime);

			pMoveAction->SetControlVisible(bFlag);
			pMoveAction->SetPlayState(bFlag);
		}

	}

	return;
}

/*************************************************
*Function: ����ؼ��ƶ�����(Ӯ�һ����ҳ�����)
*writer:
*		˧����
*Parameters:
*		[int]
*						iMoveActionID	MoveAction�ؼ�ID
*		[int]
*						StarPointID		��ʼ��ؼ�ID
*		[int]
*						EndPointID		��ֹ��ؼ�ID
*		[int]
*						iBeMoveID		���ƶ��ؼ�ID
*		[int]
*						bView			�����ͼλ��
*		[int]
*						iNum			����
*		[int]
*						iNum			����ʱ�䣨���룩
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::PlayChipInAnimaEx(const int iMoveActionID, const  int StarPointID, const  int EndPointID, const  int iBeMoveID, int bView, int iNum, int iTime, bool bFlag)
{
	if (bFlag)
	{
		//��ʼλ��
		POINT StarPoint;
		POINT EndPoint;
		ICardBase * pICardBase = NULL;
		IShCtrl * pShCtrl1 = NULL;

		GETCTRL(ICardBase, pICardBase, m_pIUI, EndPointID + bView);//��ÿ���˵�����λ��Ϊ�յ�
		if (NULL != pICardBase)
		{
			EndPoint.x = pICardBase->GetGameX();
			EndPoint.y = pICardBase->GetGameY();
		}

		GETCTRL(IShCtrl, pShCtrl1, m_pIUI, StarPointID);//�����ĳ���λ��Ϊ���
		if (NULL != pShCtrl1)
		{
			StarPoint.x = pShCtrl1->GetGameX();
			StarPoint.y = pShCtrl1->GetGameY();
		}

		IShCtrl * pShCtrl = NULL;
		GETCTRL(IShCtrl, pShCtrl, m_pIUI, iBeMoveID + bView);
		if (NULL != pShCtrl)
		{
			pShCtrl->SetShNote(iNum);
			pShCtrl->SetGameXY(StarPoint.x, StarPoint.y);
			pShCtrl->SetControlVisible(true);
		}


		IMoveAction* pMoveAction = NULL;
		GETCTRL(IMoveAction, pMoveAction, m_pIUI, iMoveActionID + bView);//�ƶ�����
		if (NULL != pMoveAction)
		{
			pMoveAction->SetControlingCtrlID(iBeMoveID + bView);
			pMoveAction->SetCurveInstance(CT_STRAIGHT);

			pMoveAction->SetStartP(StarPoint);
			pMoveAction->SetEndP(EndPoint);
			pMoveAction->SetTotalMoveTime(iTime);

			pMoveAction->SetControlVisible(bFlag);
			pMoveAction->SetPlayState(bFlag);
		}

	}

	return;
}

/*************************************************
*Function: ���ƶ���
*writer:
*		˧����
*Parameters:
*		[int]
*						iMoveActionID	MoveAction�ؼ�ID
*		[int]
*						StarPointID		��ʼ��ؼ�ID
*		[int]
*						EndPointID		��ֹ��ؼ�ID
*		[int]
*						iBeMoveID		���ƶ��ؼ�ID
*		[int]
*						bView			�����ͼλ��
*		[BYTE]
*						iCard[]			��ֵ
*		[int]
*						iNum			������
*		[int]
*						iNum			����ʱ�䣨���룩
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::PlayCardInAnima(
	const int iMoveActionID, 
	const  int StarPointID, 
	const  int EndPointID, 
	const  int iBeMoveID, 
	int bView, 
	BYTE iCard[], 
	int iNum, 
	int iTime, 
	bool bFlag)
{
	if (bFlag)
	{
		//��ʼλ��
		POINT StarPoint;
		POINT EndPoint;
		ICardBase * pICardBase = NULL;
		IBCInterFace * pControl = NULL;

		GETCTRL(ICardBase, pICardBase, m_pIUI, EndPointID + bView);//��ÿ���˵�����λ��Ϊ�յ�
		if (NULL != pICardBase)
		{
			EndPoint.x = pICardBase->GetGameX();
			EndPoint.y = pICardBase->GetGameY();
		}

		GETCTRL(IBCInterFace, pControl, m_pIUI, StarPointID);//�Է�����λ��Ϊ���
		if (NULL != pControl)
		{
			StarPoint.x = pControl->GetGameX();
			StarPoint.y = pControl->GetGameY();
		}

		ICardBase * _pICardBase = NULL;
		GETCTRL(ICardBase, _pICardBase, m_pIUI, iBeMoveID);//�ƶ�����
		if (NULL != _pICardBase)
		{
			_pICardBase->SetCardList(iCard , iNum);
			_pICardBase->SetGameXY(StarPoint.x, StarPoint.y);
			_pICardBase->SetControlVisible(true);
		}


		IMoveAction* pMoveAction = NULL;
		GETCTRL(IMoveAction, pMoveAction, m_pIUI, iMoveActionID);//�ƶ�����
		if (NULL != pMoveAction)
		{
			pMoveAction->SetControlingCtrlID(iBeMoveID);
			pMoveAction->SetCurveInstance(CT_STRAIGHT);

			pMoveAction->SetStartP(StarPoint);
			pMoveAction->SetEndP(EndPoint);
			pMoveAction->SetTotalMoveTime(iTime);

			pMoveAction->SetControlVisible(bFlag);
			pMoveAction->SetPlayState(bFlag);
		}

	}

	return;
}

//�����ؼ����֡�-----------------------------------------------------------------------

/*************************************************
*Function: ���ù��
*writer:
*		˧����
*Parameters:
*		[wchar_t*]
*						pCurImg			���ͼƬ��Դ
*		[bool]
*						bHide			������ʾʱ�Ƿ��������
*		[int]
*						PointMode		��������ģʽ
*										0�����Ͻ�Ϊ���
*										1������Ϊ���
*										2�����Ͻ�Ϊ���
*		[bool]
*						bFlag			�Ƿ���ʾ
*Return:
*		void
*************************************************/
void BaseUI::SetICursor(const wchar_t* pCurImg, bool bHide, int PointMode, bool bFlag)
{
	m_pIUI->GetICursor()->SetCursorImage(pCurImg);
	m_pIUI->GetICursor()->SetPointMode(PointMode);
	m_pIUI->GetICursor()->SetHideCursor(bHide);
	m_pIUI->GetICursor()->SetControlVisible(bFlag);

	return;
}