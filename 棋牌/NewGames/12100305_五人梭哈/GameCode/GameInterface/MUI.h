#pragma once

#include "..\..\..\..\sdk\include\GameBaseInterface\BUI.h"

class MUI : public BaseUI
{
public:
	MUI();
	virtual ~MUI();

	//���ƶ���
	void PlayCardInAnima(const int iMoveActionID, 
		const  int StarPointID, 
		const  int EndPointID, 
		const  int iBeMoveID, 
		int bView, 
		BYTE iCard[], 
		int iNum, 
		int iTime, 
		bool bFlag);

	//�����ͼƬ���ؼ��ƶ�����
	void PlayChipInAnima_Image(const int iMoveActionID[], 
		const  int StarPointID, 
		const  int EndPointID[], 
		const  int iBeMoveID[], 
		int bView, 
		int iNum, 
		int iTime, 
		bool bFlag);

	//�����ͼƬ���ؼ��ƶ�����(Ӯ�һ����ҳ�����)
	void PlayChipInAnimaEx_Image(const int iMoveActionID[], 
		const  int StarPointID[], 
		const  int EndPointID, 
		const  int iBeMoveID[], 
		int bView, 
		int iNum, 
		int iTime, 
		bool bFlag);
};

#include "MUI.inl"