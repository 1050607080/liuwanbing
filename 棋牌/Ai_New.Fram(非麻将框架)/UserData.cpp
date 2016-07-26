
#include "Stdafx.h"
#include "UserData.h"

//��ʼ������
CUserData::CUserData(void)
{
	//��ҷ���
	m_byDirect = MjDirect::Unknown;
}

//��������
CUserData::~CUserData(void)
{

}

//��������ʼ��
void CUserData::Init()
{
	HalfInit();

	//��Ҷ�����ʱ��������ӵ���ֵ
	m_byDirectPoint[0] = 0;
	m_byDirectPoint[1] = 0;

	//����Ƿ���ɴ����Ӷ���λ����
	m_bAssignedDir = false;

	//��ҷ���
	m_byDirect = MjDirect::Unknown;

	//��Ϸ�����ָһ���齫�Ľ��
	m_nResult = MjResult::Unknown;

	//����Ƿ��Ѷ���
	m_bNetCut = false;

	//�����Ϣ
	m_pUserData = NULL;

	//���ӵ�еı�������
	m_iBaoziNums = 0;

	//ÿ����ҵķֳ�ֵ
	m_iPoolPoint = 0;

	//��ҵ��Ա�
	m_bBoy = false;
}

//���ʼ��
void CUserData::HalfInit()
{
	//�Լ���״̬
	m_nState = UNKNOWN;

	//����Ƿ���ɴ����Ӷ����Ʒ�������
	m_bFetchMethodOK = false;

	//���ǰ�����Ƴ�����
	::memset(m_byWallCards,255,MJ_MAX_WALLCARD);

	//���ǰ�����Ƴǳ���(һ����ż��)
	m_iWallSize = MJ_MAX_WALLCARD;

	//����ܷ�ץ��
	m_bCanFetch = true;

	//��Ҵ��Ƴ��е�ץ��˳��trueΪ˳ʱ�뷽��falseΪ��ʱ�뷽��
	m_bCWFetch = true;

	//�û�������
	::memset(m_byHandCards,0,sizeof(m_byHandCards));

	//�û���������
	m_iHandNums = 0;

	//�û���ɲ�������
	m_bBuHuaOK = false;

	//�û��Ļ��Ʊ�
	::memset(m_byHuaCards,0,sizeof(m_byHuaCards));

	//�û��Ļ��������������Ʊ���
	m_iHuaNums = 0;

	//ץ�ƴ���
	m_iFetchTimes = 0;

	//������
	m_bOutCardLock = true;

	//�ѳ��Ʊ�
	::memset(m_byOutedCards,0,sizeof(m_byOutedCards));

	//�ѳ�������
	m_iOutedNums = 0;

	//��ҵĶ�����Ϣ�ṹ
	m_tagActInfo.Clear();

	//���Ƽ�¼
	for (int i = 0; i < MJ_MAX_CGP; ++i)
	{
		m_lstCGP[i].Clear();
	}

	//����Ƿ�������
	m_bIsTing = false;

	//��������
	m_iCGPNums = 0;

	//��Һ�����
	m_byHuCard = 255;

	//��Һ������ͱ�
	::memset(m_nHuTypes,TMjActInfo::ht_Unknown,MJ_MAX_HUTYPE);

	//��Һ������ͱ���
	m_iHuTypeNums = 0;

	//��ҵ���������(����Ͽ���,���ܺ�,�ܳ��)
	m_nHuSpecType = TMjActInfo::hst_Unknown;

	//��ұ��ֵĵ�ʧ����
	m_iInComePoint = 0;

	//�������ʣ���Ǯ
	m_iLeftMoney = 0;

/*-��������-*/

	//�����ֵ��Ч(������255),��ץ��ȡǿ��ʹ�ô�ֵ
	m_byChFetch = 255;
}

//�õ���Ҷ���������ӵ�����
//@return ���������ӵ�����
BYTE CUserData::GetDirectPoint()
{
	return (m_byDirectPoint[0] + m_byDirectPoint[1]);
}