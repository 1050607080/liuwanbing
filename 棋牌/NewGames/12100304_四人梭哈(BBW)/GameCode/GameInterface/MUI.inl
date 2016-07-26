#include "StdAfx.h"
#include "MUI.h"

MUI::MUI()
{

	return;
}
MUI::~MUI()
{

	return;
}

/*************************************************
*Function: ���ƶ���(��д�������ƿؼ��м�)
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
void MUI::PlayCardInAnima(
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

		GETCTRL(ICardBase, pICardBase, GetpIUI(), EndPointID + bView);//��ÿ���˵�����λ��Ϊ�յ�
		if (NULL != pICardBase)
		{
			int iw = 0;
			iw = pICardBase->GetGameW();
			EndPoint.x = pICardBase->GetGameX() + iw / 2 - 20;
			EndPoint.y = pICardBase->GetGameY();
		}

		GETCTRL(IBCInterFace, pControl, GetpIUI(), StarPointID);//�Է�����λ��Ϊ���
		if (NULL != pControl)
		{
			StarPoint.x = pControl->GetGameX();
			StarPoint.y = pControl->GetGameY();
		}

		ICardBase * _pICardBase = NULL;
		GETCTRL(ICardBase, _pICardBase, GetpIUI(), iBeMoveID);//�ƶ�����
		if (NULL != _pICardBase)
		{
			_pICardBase->SetCardList(iCard , iNum);
			_pICardBase->SetGameXY(StarPoint.x, StarPoint.y);
			_pICardBase->SetControlVisible(true);
		}


		IMoveAction* pMoveAction = NULL;
		GETCTRL(IMoveAction, pMoveAction, GetpIUI(), iMoveActionID);//�ƶ�����
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

/*************************************************
*Function: ���(ͼƬ)�ؼ��ƶ�����
*writer:
*		˧����
*Parameters:
*		[int]
*						iMoveActionID[2]MoveAction�ؼ�ID
*		[int]
*						StarPointID		��ʼ��ؼ�ID
*		[int]
*						EndPointID		��ֹ��ؼ�ID
*		[int]
*						iBeMoveID[2]	���ƶ��ؼ�ID(0 ͼƬ 1 ����)
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
void MUI::PlayChipInAnima_Image(const int iMoveActionID[], const  int StarPointID, const  int EndPointID[], const  int iBeMoveID[], int bView, int iNum, int iTime, bool bFlag)
{
	if (bFlag)
	{
		//��ʼλ��
		POINT StarPoint;
		POINT EndPoint;
		ICardBase * pICardBase = NULL;
		IBCInterFace * pControl = NULL;
		IMoveAction* pMoveAction = NULL;
		GETCTRL(ICardBase, pICardBase, GetpIUI(), StarPointID + bView);//��ÿ���˵�����λ��Ϊ���
		if (NULL != pICardBase)
		{
			StarPoint.x = pICardBase->GetGameX();
			StarPoint.y = pICardBase->GetGameY();
		}

		for (int i = 0; i < 2; ++i)
		{
			GETCTRL(IBCInterFace, pControl, GetpIUI(), EndPointID[i] + bView);//�Ը������עλ��Ϊ�յ�
			if (NULL != pControl)
			{
				EndPoint.x = pControl->GetGameX();
				EndPoint.y = pControl->GetGameY();
			}

			SetControlGameXY(iBeMoveID[i], StarPoint.x, StarPoint.y);
			ShowControl(iBeMoveID[i], true);
			
			GETCTRL(IMoveAction, pMoveAction, GetpIUI(), iMoveActionID[i]);//�ƶ�����
			if (NULL != pMoveAction)
			{
				pMoveAction->SetControlingCtrlID(iBeMoveID[i]);
				pMoveAction->SetCurveInstance(CT_STRAIGHT);

				pMoveAction->SetStartP(StarPoint);
				pMoveAction->SetEndP(EndPoint);
				pMoveAction->SetTotalMoveTime(iTime);

				pMoveAction->SetControlVisible(bFlag);
				pMoveAction->SetPlayState(bFlag);
			}
		}
	}

	return;
}

/*************************************************
*Function: ���(ͼƬ)�ؼ��ƶ�����(Ӯ�һ����ҳ�����)
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
void MUI::PlayChipInAnimaEx_Image(const int iMoveActionID[], const  int StarPointID[], const  int EndPointID, const  int iBeMoveID[],  int bView, int iNum, int iTime, bool bFlag)
{
	if (bFlag)
	{
		//��ʼλ��
		POINT StarPoint;
		POINT EndPoint;
		ICardBase * pICardBase = NULL;
		IBCInterFace * pControl = NULL;
		IMoveAction* pMoveAction = NULL;

		GETCTRL(ICardBase, pICardBase, GetpIUI(), EndPointID + bView);//��ÿ���˵�����λ��Ϊ�յ�
		if (NULL != pICardBase)
		{
			EndPoint.x = pICardBase->GetGameX();
			EndPoint.y = pICardBase->GetGameY();
		}

		for (int i = 0; i < 2; ++i)
		{
			GETCTRL(IImage, pControl, GetpIUI(), StarPointID[i] + bView);//�Ը������עλ��Ϊ���
			if (NULL != pControl)
			{
				StarPoint.x = pControl->GetGameX();
				StarPoint.y = pControl->GetGameY();
			}

			SetControlGameXY(iBeMoveID[i], StarPoint.x, StarPoint.y);
			ShowControl(iBeMoveID[i], true);

			GETCTRL(IMoveAction, pMoveAction, GetpIUI(), iMoveActionID[i]);//�ƶ�����
			if (NULL != pMoveAction)
			{
				pMoveAction->SetControlingCtrlID(iBeMoveID[i]);
				pMoveAction->SetCurveInstance(CT_STRAIGHT);

				pMoveAction->SetStartP(StarPoint);
				pMoveAction->SetEndP(EndPoint);
				pMoveAction->SetTotalMoveTime(iTime);

				pMoveAction->SetControlVisible(bFlag);
				pMoveAction->SetPlayState(bFlag);
			}
		}
	}

	return;
}