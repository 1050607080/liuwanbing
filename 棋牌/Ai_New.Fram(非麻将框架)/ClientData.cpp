
#include "StdAfx.h"
#include "ClientData.h"
#include "GameAlgorithm.h"
#include "UpgradeMessage.h"

///���ƣ�CClientDataObject
///��������ʼ���������ã��������͵�
///@param 
///@return 
CClientData::CClientData()
{	
}

///���ƣ���������
///������~CClientDataObject
///@param 
///@return 
CClientData::~CClientData(void)
{

}

///��������ʼ��
void CClientData::Init()
{
	HalfInit();

	//�����û�����
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		m_UserData[i].Init();
	}

	//ׯ��
	m_byNTUser = 255;

}

///���������ʼ�������ڻغ�֮��������ȫ�Ĳ���������Ϊ�е�������Ҫ������
void CClientData::HalfInit()
{
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		//�����û�����
		m_UserData[i].HalfInit();

		//������ҵ�ʣ����
	}

	//���ƴ����ӵĵ���ֵ
	m_byFetchDice[0] = 0;
	m_byFetchDice[1] = 0;

	//������������ʼ����
	m_byFetchUser = 255;

	//���ƵĶ���
	m_iFetchPos = 0;

	//��ǰ����û�
	m_byTokenUser = 255;

}

///Ϊĳ��������ϵ�������
void CClientData::SortCardsTo(BYTE bDeskStation)
{
	//GAlg::SortCard(m_UserData[bDeskStation].m_byMyCards,NULL,m_UserData[bDeskStation].m_iCardsNum,&m_tagDeskCfg.Rule);

	return;
}

/*----------------------------------------------------------------------------------------------------------------
	һϵ����Ϸ�ͻ��˸��µĽӿڣ�ÿ����Ϸ����ͬ
---------------------------------------------------------------------------------------------------------------*/

//�����������������Ӻ����ݸ���
void CClientData::SaveSysDeskCfg(const LPCVOID lpData)
{
	const TDeskCfg* pDeskCfg = (TDeskCfg*)lpData;

	::memcpy(&m_tagDeskCfg,pDeskCfg,sizeof(TDeskCfg));
}

//�������������������λ,���ݸ���
void CClientData::SaveAssignDirectInfo(const LPCVOID lpData)
{
	const TAssignDirect* pAssignDir = (TAssignDirect*)lpData;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		//�õ�i������
		m_UserData[i].m_byDirect = pAssignDir->direct[i];

		//��ʼ����ҵķֳ�ֵ
		m_UserData[i].m_iPoolPoint = m_tagDeskCfg.dz.iPool;
	}
}

//���Է�������ׯ�ҵ���Ϣ����������
void CClientData::SaveNTInfo(const LPCVOID lpData)
{
	const TMakeNTInfo* pNTInfo = (TMakeNTInfo*)lpData;

	//���ׯ��λ��
	m_byNTUser = pNTInfo->byUser;

	return;
}

//�������������Ƶķ�������������
void CClientData::SaveFetchMothod(const LPCVOID lpData)
{
	const TFetchMethod* pFetchMothod = (TFetchMethod*)lpData;

	//�������ֵ
	m_byFetchDice[0] = pFetchMothod->byDice[0];
	m_byFetchDice[1] = pFetchMothod->byDice[1];

	//������Ʒ���
	m_byFetchUser = pFetchMothod->byUser;

	//������ƵĶ���
	m_iFetchPos = pFetchMothod->iPos;

	return;
}

//���Է������ķ��������ݣ����и���
void CClientData::SaveHandCards(const LPCVOID lpData, BYTE nDeskStation)
{
	const TFetchHands* pFetchHands = (TFetchHands*)lpData;

	int iDeskStation = pFetchHands->iDeskStation;

	CString str;
	str.Format("wzmj:    SaveHandCards--->iCardNum -> %d", pFetchHands->iCardNum);
	OutputDebugString(str);

	//�������Լ�������������������
	::memcpy(m_UserData[iDeskStation].m_byHandCards,pFetchHands->byCards,pFetchHands->iCardNum);
	m_UserData[iDeskStation].m_iHandNums = pFetchHands->iCardNum;

	CUserData *p = &m_UserData[iDeskStation];
	for (int i = 0; i < p->m_iHandNums; i ++) {
		str.Format("wzmj:   SaveHandCards-> card->%d  fetchcard->%d i->%d   iDeskStation->%d", p->m_byHandCards[i], pFetchHands->byCards[i], i, iDeskStation);
		OutputDebugString(str);
	}

	return;
}

//���Է��������������ݣ����и���
void CClientData::SaveTiaoCards(const LPCVOID lpData, BYTE nDeskStation)
{
	const TTiaoCards* pTiaoCards = (TTiaoCards*)lpData;

	BYTE nMeStation = nDeskStation;

	CUserData* pUser = NULL;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		pUser = &m_UserData[i];
		
		if (pTiaoCards->tagFetch[i].val != 255)
		{
			//��ӵ�һ������
			pUser->m_byHandCards[pUser->m_iHandNums] = pTiaoCards->tagFetch[i].val;

			//�������Ƶ�����
			pUser->m_iHandNums++;
		}
	}

	//ׯ�ң�����ӵڶ�������
	pUser = &m_UserData[m_byNTUser];

	//��ӵ�һ������
	pUser->m_byHandCards[pUser->m_iHandNums] = pTiaoCards->tagFetch[2].val;

	//�������Ƶ�����
	pUser->m_iHandNums++;

	return;
}

//�շ�������Ϊ����ҵĲ������ݣ����и���
void CClientData::SaveBuHuas(const LPCVOID lpData, BYTE nDeskStation)
{
	const TBuHuas* pBuHuas = (TBuHuas*)lpData;

	BYTE nMeStation = nDeskStation;

	CUserData* pUser = NULL;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (pBuHuas->iNum[i] > 0) //���i�л�
		{
			pUser = &m_UserData[i];

			//�������i�Ļ��Ʊ�
			::memcpy(pUser->m_byHuaCards,pBuHuas->byHuaList[i],pBuHuas->iNum[i]);

			//�������i�Ļ�������
			pUser->m_iHuaNums = pBuHuas->iNum[i];
		}
	}

	pUser = &m_UserData[nMeStation];

	//����Լ��л�,��Ҫ�����Լ����������ݣ�
	//�������û�������ݣ�û�и�������
	if (pBuHuas->iNum[nMeStation] > 0)
	{
		//��ȡ���ƣ��л��Ƶ���ȥ��
		for (int i = 0; i < pBuHuas->iNum[nMeStation]; ++i)
		{
			for (int j = 0; j < pUser->m_iHandNums; ++j)
			{
				if (pUser->m_byHandCards[j] == pBuHuas->byHuaList[nMeStation][i])
				{
					pUser->m_byHandCards[j] = pBuHuas->byBuList[i];
					break;
				}
			}
		}
		
	}

	//������������
	CGameAlgorithm::sort(pUser->m_byHandCards,pUser->m_iHandNums,false);

	return;
}

//�������·����ƣ���������
void CClientData::SaveTokenInfo(const LPCVOID lpData)
{
	const TToken* pToken = (TToken*)lpData;

	CUserData* pUser = &m_UserData[pToken->byUser];

	//��¼��ǰ�����Ƶ����
	m_byTokenUser = pToken->byUser;

	if (pToken->bFetch && pToken->Fetcher.val != 0) //�����ץ��
	{
		pUser->m_byHandCards[pUser->m_iHandNums] = pToken->Fetcher.val;
		pUser->m_iHandNums ++;
	}

	return;
}

//ĳ��ҳ�����,��������
void CClientData::SaveOutCardInfo(const LPCVOID lpData, BYTE nDeskStation)
{
	const TOutCard* pOCInfo = (TOutCard*)lpData;

	CUserData* pUser = &m_UserData[pOCInfo->byUser];

	if (pOCInfo->byUser == nDeskStation)
	{

		CString str;
		str.Format("XHMJClient: server hands = ");
		for (int i = 0; i < pOCInfo->iHandNums; ++i)
		{
			str.Format("%s %02d",str,pOCInfo->byCards[i]);
		}
		OutputDebugString(str);
		
		str.Format("XHMJClient: before hands = ");
		for (int i = 0; i < pUser->m_iHandNums; ++i)
		{
			str.Format("%s %02d",str,pUser->m_byHandCards[i]);
		}
		OutputDebugString(str);

		//���Լ�������������ɾ����������
		if (CGameAlgorithm::RemoveOneCard(pOCInfo->byCard,pUser->m_byHandCards,pUser->m_iHandNums))
		{
			pUser->m_iHandNums--;

			//��������������
			CGameAlgorithm::sort(pUser->m_byHandCards,pUser->m_iHandNums,false);

			str.Format("XHMJClient:  after hands = ");
			for (int i = 0; i < pUser->m_iHandNums; ++i)
			{
				str.Format("%s %02d",str,pUser->m_byHandCards[i]);
			}
			OutputDebugString(str);

			for (int i = 0; i < pOCInfo->iHandNums; ++i)
			{
				if (pUser->m_byHandCards[i] != pOCInfo->byCards[i])
				{
					OutputDebugString("XHMJClient:�����������������ͬ����***********************************************************");
					break;
				}
			}
		}
	}
	else
	{
		//�����������û��ʵ�ڵ�����ֱ�Ӹ�����������
		pUser->m_iHandNums--;
	}

	//����pUser�ĳ��Ʊ������
	pUser->m_byOutedCards[pUser->m_iOutedNums++] = pOCInfo->byCard;

	return;
}

//���ж��������¶���
void CClientData::SaveActNotify(const LPCVOID lpData, BYTE nDeskStation)
{
	const TActNotify* pActNotify = (TActNotify*)lpData;

	BYTE nMeStation = nDeskStation;

	//�����Լ��Ķ�����Ϣ
	if (pActNotify->info.usFlags > 0)
	{
		::memcpy(&m_UserData[nMeStation].m_tagActInfo,&pActNotify->info,sizeof(MjBase::TMjActInfo));
	}
	else
	{
		//�ն���ʱ����Լ��Ķ�����Ϣ
		m_UserData[nMeStation].m_tagActInfo.Clear();
	}

	return;
}

//�����ִ�ж�������������
void CClientData::SaveActInfo(const LPCVOID lpData, BYTE nDeskStation)
{
	const TActInfo* pActInfo = (TActInfo*)lpData;

	CUserData* pUser = NULL;

	//�����Ƶĳ��֣�Ҫ�������Ƽ���������
	if (TMjCGPNode::UNKNOWN != pActInfo->CGPNode.nType)
	{
		//�õ����������ߵ�����
		pUser = &m_UserData[pActInfo->byUsers[0]];

		//�����ܶ������¶�������ӵ����Ʊ�\
		  ���ܶ������޸����Ʊ�
		if (TMjCGPNode::BuGang != pActInfo->CGPNode.nType)
		{
			pUser->m_lstCGP[pUser->m_iCGPNums++] = pActInfo->CGPNode;
		}
		else
		{
			pUser->m_lstCGP[pActInfo->CGPNode.nIdx] = pActInfo->CGPNode;
		}
		
		//������������
		pUser->m_iHandNums = pActInfo->iHandNums;

		//������������,ֻ�����Լ���,���������û�����ݵģ�û���±�Ҫ
		if (nDeskStation == pActInfo->byUsers[0])
		{
			::memcpy(pUser->m_byHandCards,pActInfo->byHands,pActInfo->iHandNums);
		}

		//���͸ܶ�������Ҫ�۳�������ҳ��Ʊ�һ����
		if (TMjCGPNode::Peng == pActInfo->CGPNode.nType 
				|| TMjCGPNode::MingGang == pActInfo->CGPNode.nType
				|| TMjCGPNode::Chi == pActInfo->CGPNode.nType)
		{
			//�õ��������ߵ�����
			pUser = &m_UserData[pActInfo->CGPNode.from];

			pUser->m_iOutedNums--;
		}
	}

	//�����Ƴ���,����������������Ʊ�
	if(MjActFlag::usTing == pActInfo->usActFlag)
	{
		;
	}

	//�к��Ƴ���,������������
	if (MjActFlag::usHu == pActInfo->usActFlag)
	{
		BYTE nMeStation =nDeskStation;

		//�õ��Լ�������
		pUser = &m_UserData[nMeStation];

		//������������
		::memcpy(pUser->m_byHandCards,pActInfo->byHands,pActInfo->iHandNums);

		//������������
		pUser->m_iHandNums = pActInfo->iHandNums;
	}

	return;
}

//����Ұ��ܣ�Ҫ��ʱ���·�������������
void CClientData::SaveAnGangResult(const LPCVOID lpData)
{
	const TAnGangResult* pAnGangResult = (TAnGangResult*)lpData;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		CUserData* pUser = &m_UserData[i];

		//�޸ķֳ�ֵ
		pUser->m_iPoolPoint = pAnGangResult->iPools[i];

		//�޸����ʣ��Ľ��
		pUser->m_iLeftMoney = pAnGangResult->iLeftMoney[i];
	}

	return;
}

//�յ��غϽ�����Ϣ�������������ݣ���ʱ�����֮ǰ���ܵ������ѷ�����
void CClientData::SaveRoundFinish(const LPCVOID lpData)
{
	const TRoundFinish* pRoundFinish = (TRoundFinish*)lpData;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		CUserData* pUser = &m_UserData[i];

		//������ҵ�����
		::memcpy(pUser->m_byHandCards,pRoundFinish->byHandCards[i],pRoundFinish->iHandNums[i]);

		//�õ���������
		pUser->m_iHandNums = pRoundFinish->iHandNums[i];

		//�õ����
		pUser->m_nResult = pRoundFinish->nResult[i];

		//�õ���Һ�����
		pUser->m_byHuCard = pRoundFinish->byHuCard[i];
	}

	return;
}

//�յ�������Ϣ�󣬸�������
void CClientData::SaveRoundResult(const LPCVOID lpData)
{
	const TResult* pResult = (TResult*)lpData;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		CUserData* pUser = &m_UserData[i];

		//�������ʣ��Ľ��
		pUser->m_iLeftMoney = pResult->iLeftMoney[i];

		//���·ֳ�
		pUser->m_iPoolPoint = pResult->iPoolPoint[i];
	
	}

	return;
}

//�ָ���������Ϸ����
void CClientData::RecurSaveBase(const LPCVOID lpData, BYTE nDeskStation)
{
	const TGSBase* pGSBase = (TGSBase*)lpData;

	BYTE nMeStation = nDeskStation;

	//��ԭ������������
	::memcpy(&m_tagDeskCfg,&pGSBase->tagDeskCfg,sizeof(TDeskCfg));

	//��ԭׯ��λ������
	m_byNTUser = pGSBase->byNTUser;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		//��ԭ����ҵ���λ��������
		m_UserData[i].m_byDirect = pGSBase->direct[i];

		//��ԭ��ȡ����ҵķֳ�ֵ����
		m_UserData[i].m_iPoolPoint = pGSBase->iPoolPoint[i];

		//��ԭ��ȡ���������ʣ���Ǯ����
		m_UserData[i].m_iLeftMoney = pGSBase->iLeftMoney[i];

		//��ԭ��������ϵ�������
		m_UserData[i].m_iHandNums = pGSBase->iHandNums[i];

		//��ԭ����һ��Ʊ����ݣ����䳤������
		::memcpy(m_UserData[i].m_byHuaCards,pGSBase->byHuaCards[i],pGSBase->iHuaNums[i]);
		m_UserData[i].m_iHuaNums = pGSBase->iHuaNums[i];

	}

	//��ԭ�Լ���������
	::memcpy(m_UserData[nMeStation].m_byHandCards,pGSBase->byHandCards,\
		pGSBase->iHandNums[nMeStation]);

	return;
}

//�ָ�����ʱ����Ϸ����
void CClientData::RecurSavePlaying(const LPCVOID lpData, BYTE nDeskStation)
{
	const TGSPlaying* pPlaying = (TGSPlaying*)lpData;

	BYTE nMeStation = nDeskStation;

	//��ԭ���Ƶ�������
	m_byTokenUser = pPlaying->byTokenUser;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		//��ԭ�����б�����,���䳤��
		::memcpy(m_UserData[i].m_byOutedCards,pPlaying->byOutedCards[i],pPlaying->iOutedNums[i]);
		m_UserData[i].m_iOutedNums = pPlaying->iOutedNums[i];

		//��ԭ���Ʊ����䳤��
		::memcpy(m_UserData[i].m_lstCGP,pPlaying->CGPList[i],sizeof(MjBase::TMjCGPNode) * pPlaying->iCGPNums[i]);
		m_UserData[i].m_iCGPNums = pPlaying->iCGPNums[i];
	}

	//�����Լ��Ķ�����Ϣ
	if (pPlaying->ActInfo.usFlags > 0)
	{
		::memcpy(&m_UserData[nMeStation].m_tagActInfo,&pPlaying->ActInfo,sizeof(MjBase::TMjActInfo));
	}

	return;
}

/*----------------------------------------------------------------------------------------------------------------
	���ݶ����齫 ���й�����
------------------------------------------------------------------------------------------------------------------*/

bool CClientData::WzFengZiLocked(int nDeskStation, int iCaiShen)
{
	BYTE byHandCard[MJ_MAX_HANDCARD];
	memset(byHandCard, 255, sizeof(byHandCard));
	
	memcpy(byHandCard, m_UserData[nDeskStation].m_byHandCards, sizeof(m_UserData[nDeskStation].m_byHandCards));

	int FengZiGrid[7];
	memset(FengZiGrid, 0, sizeof(FengZiGrid));
	int iCardNumber = m_UserData[nDeskStation].m_iHandNums;
	for (int i = 0; i < iCardNumber; i ++)
	{
		if (byHandCard[i] == 31 && iCaiShen != 31)
		{
			FengZiGrid[0] ++;
		}
		else if (byHandCard[i] == 32 && iCaiShen != 32)
		{
			FengZiGrid[1] ++;
		}
		else if (byHandCard[i] == 33 && iCaiShen != 33)
		{
			FengZiGrid[2] ++;
		}
		else if (byHandCard[i] == 34 && iCaiShen != 34)
		{
			FengZiGrid[3] ++;
		}
		else if (byHandCard[i] == 35 && iCaiShen != 35)
		{
			FengZiGrid[4] ++;
		}
		else if (byHandCard[i] == 36 && iCaiShen != 36)
		{
			FengZiGrid[5] ++;
		}
		else if (byHandCard[i] == 37)
		{
			if (iCaiShen >= 31 && iCaiShen < 37)
				FengZiGrid[6] ++;
		}
	}

	if (FengZiGrid[0] == 1 || FengZiGrid[1] == 1 || FengZiGrid[2] == 1 ||
		FengZiGrid[3] == 1 || FengZiGrid[4] == 1 || FengZiGrid[5] == 1 || FengZiGrid[6] == 1)
	{
		return true;
	}

	return false;
}

bool CClientData::WzFengZiInside(int nDeskStation, int iCaiShen, int iFengZi)
{
	BYTE byHandCard[MJ_MAX_HANDCARD];
	memset(byHandCard, 255, sizeof(byHandCard));

	memcpy(byHandCard, m_UserData[nDeskStation].m_byHandCards, sizeof(m_UserData[nDeskStation].m_byHandCards));
	
	//ȥ������Ĳ��� �� �Ѱװ廻Ϊ����
	int iCardNumber = m_UserData[nDeskStation].m_iHandNums;
	for (int i = 0; i < iCardNumber; i ++)
	{
		if (byHandCard[i] == (BYTE)iCaiShen)
			byHandCard[i] = 255;
		if (byHandCard[i] == 37)
			byHandCard[i] = (BYTE)iCaiShen;
	}

	CGameAlgorithm::sort(byHandCard, iCardNumber, false);
	for (int i = 0; i < iCardNumber; i ++)
	{
		if ((BYTE)iFengZi == byHandCard[i])
			return true;
	}

	return false;
}

bool CClientData::isWzFengZi(int iCard)
{
	if (iCard>=31 && iCard<=37)
		return true;
	return false;
}

bool CClientData::isWzFengZiEx(int iCard)
{
	if (iCard>=31 && iCard<=36)
		return true;
	return false;
}