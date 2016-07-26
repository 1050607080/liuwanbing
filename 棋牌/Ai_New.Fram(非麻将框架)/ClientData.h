
// ***************************************************************
//  GameDataObject   version:  1.0   ��  date: 12/31/2010
//  -------------------------------------------------------------

//	�������������Ϸ����\
	NameID��50200260

//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once
#include "UserData.h"

class CClientData
{

public:

	//��ʼ������
	CClientData();

	//�ͷ�����
	virtual~CClientData(void);

/*-���õĿؼ�������-*/
public:

	//��ʼ��
	void Init();

	//���ʼ��
	void HalfInit();

	//Ϊĳ��������ϵ�������
	void SortCardsTo(BYTE bDeskStation);

/*----------------------------------------------------------------------------------------------------------------
	һϵ����Ϸ�ͻ��˸��µĽӿڣ�ÿ����Ϸ����ͬ
  ---------------------------------------------------------------------------------------------------------------*/

	//�����������������Ӻ����ݸ���
	void SaveSysDeskCfg(const LPCVOID lpData);

	//�������������������λ,���ݸ���
	void SaveAssignDirectInfo(const LPCVOID lpData);

	//���Է�������ׯ�ҵ���Ϣ����������
	void SaveNTInfo(const LPCVOID lpData);

	//�������������Ƶķ�������������
	void SaveFetchMothod(const LPCVOID lpData);

	//���Է������ķ��������ݣ����и���
	void SaveHandCards(const LPCVOID lpData, BYTE nDeskStation);

	//���Է��������������ݣ����и���
	void SaveTiaoCards(const LPCVOID lpData, BYTE nDeskStation);

	//�շ�������Ϊ����ҵĲ������ݣ����и���
	void SaveBuHuas(const LPCVOID lpData, BYTE nDeskStation);

	//�������·����ƣ���������
	void SaveTokenInfo(const LPCVOID lpData);

	//ĳ��ҳ�����,��������
	void SaveOutCardInfo(const LPCVOID lpData, BYTE nDeskStation);

	//���ж��������¶���
	void SaveActNotify(const LPCVOID lpData, BYTE nDeskStation);

	//�����ִ�ж�������������
	void SaveActInfo(const LPCVOID lpData, BYTE nDeskStation);

	//����Ұ��ܣ�Ҫ��ʱ���·�������������
	void SaveAnGangResult(const LPCVOID lpData);

	//�յ��غϽ�����Ϣ�������������ݣ���ʱ�����֮ǰ���ܵ������ѷ�����
	void SaveRoundFinish(const LPCVOID lpData);

	//�յ�������Ϣ�󣬸�������
	void SaveRoundResult(const LPCVOID lpData);

/*-��Ϸ״̬�ֳ��ָ�����-*/
public:

	//�ָ���������Ϸ����
	void RecurSaveBase(const LPCVOID lpData, BYTE nDeskStation);

	//�ָ�����ʱ����Ϸ����
	void RecurSavePlaying(const LPCVOID lpData, BYTE nDeskStation);

/*----------------------------------------------------------------------------------------------------------------
	���ݶ���
  ---------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------
	���ݶ����齫 ���й�����
------------------------------------------------------------------------------------------------------------------*/
	
	bool WzFengZiLocked(int nDeskStation, int iCaiShen);
	
	bool isWzFengZi(int iCard);

	bool WzFengZiInside(int nDeskStation, int iCaiShen, int iFengZi);

	bool isWzFengZiEx(int iCard);

/*-������������Ϸ-*/
public:

	//�û�����
	CUserData m_UserData[PLAY_COUNT];

	//��������
	TDeskCfg m_tagDeskCfg;

/*-������-*/
private:

/*-�������˻��齫������-*/
public:

	//���ƴ����ӵĵ���ֵ
	BYTE m_byFetchDice[2];

	//������������ʼ����
	BYTE m_byFetchUser;

	//���ƵĶ���
	int m_iFetchPos;

	//�õ����Ƶ��û������ɻ���û�
	BYTE m_byTokenUser;

	//ׯ��
	BYTE m_byNTUser;

};


