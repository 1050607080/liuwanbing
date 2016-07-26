
// ***************************************************************
//  UserData ServerObject   version:  1.0   ��  date: 2/12/2011
//  -------------------------------------------------------------

//	�˻��齫�������Ϸ����\
	NameID��20001400

//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

#include "comstruct.h"

#include "UpgradeMessage.h"

#include "MjCommonDef.h"
using namespace MjBase;

class CUserData
{

public:

	//��ʼ������
	CUserData(void);

	//�ͷ�����
	virtual~CUserData(void);

	//��ʼ��
	void Init();

	//���ʼ��
	void HalfInit();

/*-��ȡ��Ϣ-*/
public:

	//�õ���Ҷ���������ӵ�����
	BYTE GetDirectPoint();

/*-����-*/
public:


/*-�����Ϸ����-*/
public:

	//���״̬����Ҫ�����ж���Щ���ͬ��/��ͬ����Ϸ 
	enum emState
	{
		UNKNOWN = 0,		 //δ֪״̬
		AGREEGAME = 3,       //���ͬ����Ϸ
	};
	emState m_nState;

	//��Ҷ�����ʱ��������ӵ���ֵ
	BYTE m_byDirectPoint[2];

	//��ҷ��򣬶��������ֱ�Ϊ0,1,2,3��255Ϊδ������
	BYTE m_byDirect;

	//����Ƿ���ɴ����Ӷ���λ����
	bool m_bAssignedDir;

	//����Ƿ���ɴ����Ӷ����Ʒ�������
	bool m_bFetchMethodOK;

	//���ǰ�����Ƴ�����
	BYTE m_byWallCards[2][MJ_MAX_WALLDUN];

	//���ǰ�����Ƴǳ���(һ����ż��)
	int m_iWallSize;

	//����ܷ�ץ��
	bool m_bCanFetch;

	//��Ҵ��Ƴ��е�ץ��˳��trueΪ˳ʱ�뷽��falseΪ��ʱ�뷽��
	bool m_bCWFetch;

	//�û�������
	BYTE m_byHandCards[MJ_MAX_HANDCARD];

	//�û���������
	int m_iHandNums;

	//�û��Ļ��Ʊ�
	BYTE m_byHuaCards[MJ_MAX_HUA];

	//�û��Ļ��������������Ʊ���
	int m_iHuaNums;

	//�û���ɲ�������
	bool m_bBuHuaOK;

	//ץ�ƴ���
	int m_iFetchTimes;

	//������,����ҵõ����ƺ�,������,һ����ҳ��Ƴɹ���,����\
	  ��Ƹñ�����Ŀ���Ǳ���ͻ������ͨ���Ƿ��ֶ��ظ�����
	bool m_bOutCardLock;

	//�ѳ��Ʊ�
	BYTE m_byOutedCards[MJ_MAX_OUTED];

	//�ѳ�������
	int m_iOutedNums;

	//��ҵĶ�����Ϣ�ṹ
	MjBase::TMjActInfo m_tagActInfo;

	//���Ƽ�¼
	MjBase::TMjCGPNode m_lstCGP[MJ_MAX_CGP];

	//��������
	int m_iCGPNums;

	//����Ƿ�������
	bool m_bIsTing;

	//��Ϸ�����ָһ���齫�Ľ��
	BYTE m_nResult;

	//��Һ�����
	BYTE m_byHuCard;

	//��Һ������ͱ�
	TMjActInfo::emHuType m_nHuTypes[MJ_MAX_HUTYPE];

	//��Һ������ͱ���
	int m_iHuTypeNums;

	//��ҵ���������(����Ͽ���,���ܺ�,�ܳ��)
	TMjActInfo::emHuSpecialType m_nHuSpecType;

	//��ұ��ֵĵ�ʧ����,��Ϊ�����ĸ��ӷ���,ÿһ�ֿ�ʼ���0\
	  һ����������ܷ�	  
	int m_iInComePoint;	

/*-�˻��齫���е��������-*/
public:

	//���ӵ�еı�������
	int m_iBaoziNums;

	//ÿ����ҵķֳ�ֵ
	int m_iPoolPoint;

/*-��ҷ���Ϸ����-*/
public:

	//�����Ϣ
	const UserInfoStruct* m_pUserData;

	//��ҵ��Ա�,Ϊ���������Ϸ�ͻ��˲��ܻ�ȡ����Ա������
	bool m_bBoy;

	//����Ƿ��Ѷ���
	bool m_bNetCut;

	//�������ʣ���Ǯ
	int m_iLeftMoney;

/*-��������-*/
public:

	//�����ֵ��Ч(������255),��ץ��ȡǿ��ʹ�ô�ֵ,\
	��ֵһ����ʹ��
	BYTE m_byChFetch;
	
};