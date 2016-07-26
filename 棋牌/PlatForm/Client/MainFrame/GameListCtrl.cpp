#include "StdAfx.h"
#include "GamePlace.h"
#include "GameListCtrl.h"
#include "PlaceResource.h"
#include "GamePlaceMessage.h"
#include "AppealDlg.h"
#include "GameListTip.h"
#include "MainRoomEx.h"

extern void DebugPrintf(const char *p, ...);


BEGIN_MESSAGE_MAP(CGameListCtrl, CTreeCtrl)

	//{{AFX_MSG_MAP(CTreeCtrlEx) 
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CGameListCtrl::OnNMCustomdraw)
 	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CGameListCtrl::OnNMClick)
	//ON_NOTIFY_REFLECT_EX(NM_DBLCLK, &CGameListCtrl::OnNMDlbClick)
	ON_BN_CLICKED(IDC_BTN_Rule,&CGameListCtrl::OnClickReadRule)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP 


END_MESSAGE_MAP()

#define SET_LINE_COLOR          (TV_FIRST+40)		//������ɫ��Ϣ						
#define LS_TIME_OUT				60L		//�б�ʱ��									

//���ؼ�ָ��
CGameListCtrl					* CAFCGameListItem::m_pTreeCtrl=NULL;
std::map<UINT,CGameListCtrl::Color_Font>  CGameListCtrl::m_mapStatusColor;
std::map<void*,UINT> CGameListCtrl::m_mapHitemRoomID;
void CGameListCtrl::IEnterARoomOrLeave(UINT uRoomID,bool bEnter)//������뿪һ�����䡣��������ӷ���������������������
{
	CAFCRoomItem * pGameRoomItem = FindRoomItem(uRoomID);//��÷�����б��ָ��
	if (!pGameRoomItem)
		return;
	int dd = 0;
	if (bEnter) 
		dd = 1;
	else 
		dd = -1;

	UINT uOldPeopleCount = pGameRoomItem->m_RoomInfo.uPeopleCount;
	pGameRoomItem->m_RoomInfo.uPeopleCount += dd;
	
	if (pGameRoomItem->m_RoomInfo.uPeopleCount < 0 || pGameRoomItem->m_RoomInfo.uPeopleCount>0x7fffffff)
		pGameRoomItem->m_RoomInfo.uPeopleCount = 0;
	
	TCHAR szBuffer[100];
	memset(szBuffer,0,sizeof(szBuffer));
    //��������������������������

    // ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
    COLORREF statuscolor = RGB(0,0,0);
	CString strStatusText = "";
	if(!RoomOnLineShow(uOldPeopleCount,szBuffer, &pGameRoomItem->m_RoomInfo,statuscolor,strStatusText))
		return;

	SetItemText(pGameRoomItem->m_hCurrentItem,szBuffer);
	SetItemStatusColor(pGameRoomItem->m_hCurrentItem,pGameRoomItem->m_RoomInfo.uRoomID,RGB(0,0,0),statuscolor,strStatusText);
	CAFCNameItem *p = FindNameItem(pGameRoomItem->m_RoomInfo.uNameID);
	if(p)
	{
		p->m_NameInfo.m_uOnLineCount += dd;
		if (p->m_NameInfo.m_uOnLineCount < 0 || p->m_NameInfo.m_uOnLineCount>0x7FFFFFFF)
			p->m_NameInfo.m_uOnLineCount = 0;

		UpdateOnLineCount();

		///////////////////////////////////////////////////////////
		/// ��������Ϸ�б�
		bool canadd = true;
		for(int i=0;i<Glb().m_baFavorSave.GetCount();i++)
		{
			if(p->m_NameInfo.uNameID==Glb().m_baFavorSave[i])
			{
				canadd=false;
				break;
			}			
		}
		if(canadd)
			Glb().m_baFavorSave.Add(p->m_NameInfo.uNameID);
	}
}

//��������������
bool CGameListCtrl::UpdateOnLineCount()
{
	//ͳ������
	UINT uOnLineCount = 0L;
	CAFCGameListItem * pGameKindItem = GetKindParentItem();//��ȡ����
	for (INT_PTR i=0; i<pGameKindItem->m_ItemPtrArray.GetCount(); i++)
	{
		CAFCKindItem * pKindItem = (CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (NULL == pKindItem)
			continue;

		UINT kd = 0;

		for (INT_PTR j = 0; j < pKindItem->m_ItemPtrArray.GetCount(); j++)
		{
			CAFCKindItem * pKindItem2 = (CAFCKindItem *)pKindItem->m_ItemPtrArray.GetAt(j);

			// �ж�����Ϸ���ͻ�����Ϸ����
			if (pKindItem2->m_uDataType == GLK_GAME_KIND)
			{
				UINT kd2 = 0;

				// ����Ϸ����, �����Ŀ¼Ϊ����Ŀ¼, ��Ŀ¼�»�����Ϸ
				for (INT_PTR k = 0; k < pKindItem2->m_ItemPtrArray.GetCount(); k++)
				{
					CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem2->m_ItemPtrArray.GetAt(k);
					if (NULL != pNameItem)
					{
						if (pNameItem->m_NameInfo.m_uOnLineCount < 0 || pNameItem->m_NameInfo.m_uOnLineCount > 0x7fffffff)
						{	
							pNameItem->m_NameInfo.m_uOnLineCount = 0;
						}

						pNameItem->UpdateOnLineCount();
						kd2 += pNameItem->m_NameInfo.m_uOnLineCount;
					}
				}

				uOnLineCount += kd;

				if (kd < 0)
				{	
					kd = 0;
				}

				TCHAR szBuffer[100];
				ZeroMemory(szBuffer,sizeof(szBuffer));

				KindOnLineShow(szBuffer, pKindItem2->m_KindInfo.szKindName, kd2);

				SetItemText(pKindItem2->m_hCurrentItem, szBuffer);
			}
			else
			{
				CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (NULL != pNameItem)
				{
					if (pNameItem->m_NameInfo.m_uOnLineCount < 0 || pNameItem->m_NameInfo.m_uOnLineCount>0x7fffffff)
					{	
						pNameItem->m_NameInfo.m_uOnLineCount = 0;
					}

					pNameItem->UpdateOnLineCount();
					kd += pNameItem->m_NameInfo.m_uOnLineCount;
				}
			}
		}
		
		uOnLineCount += kd;
		if(kd<0)
			kd=0;
		
		TCHAR szBuffer[100];
		ZeroMemory(szBuffer,sizeof(szBuffer));
		
        // ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
        KindOnLineShow(szBuffer, pKindItem->m_KindInfo.szKindName,kd);

		SetItemText(pKindItem->m_hCurrentItem,szBuffer);
	}
	
	
	//������ʾ
	TCHAR szBuffer[100];
	ZeroMemory(szBuffer,sizeof(szBuffer));
    
    // �������ʾ���֣���ֻ��ʾ����
    RootOnLineShow(szBuffer, TEXT(Glb().m_Tree[0].GetBuffer()),uOnLineCount);
	SetItemText(pGameKindItem->m_hCurrentItem,szBuffer);
	///////////////////////////////////////////////////////////
	/// ��������Ϸ�б�
	if(Glb().m_baFavor.GetCount())
	{
		UINT uOnLineCount=0L;
		CAFCGameListItem * pGameKindItem=(CAFCGameListItem *)m_InsideArray.GetAt(0);//��ȡ����
		for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
		{
			CAFCNameItem * pNameItem=(CAFCNameItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
			if (pNameItem!=NULL)
			{
				if(pNameItem->m_NameInfo.m_uOnLineCount < 0 || pNameItem->m_NameInfo.m_uOnLineCount>0x7fffffff)
					pNameItem->m_NameInfo.m_uOnLineCount=0;
				pNameItem->UpdateOnLineCount();
			}
		}

	}	
	return true;
}


//��������
bool CGameListCtrl::HandleNameOnLineData(DL_GP_RoomListPeoCountStruct * pOnLineCountPtr, UINT uCount)
{
	
	for (UINT i=0;i<uCount;i++) 
	{
		CAFCNameItem * pNameItem=FindNameItem((pOnLineCountPtr+i)->uID);
		if (pNameItem!=NULL)
		{
			pNameItem->m_NameInfo.m_uOnLineCount=(pOnLineCountPtr+i)->uOnLineCount;
            pNameItem->UpdateOnLineCount();
			//pNameItem->ExpandItem (1);    // yjj 2008-12-23
		}

		pNameItem=FindNameItem((pOnLineCountPtr+i)->uID, 999);
		if (pNameItem!=NULL)
		{
			pNameItem->m_NameInfo.m_uOnLineCount=(pOnLineCountPtr+i)->uOnLineCount;
			pNameItem->UpdateOnLineCount();
			//pNameItem->ExpandItem (1);    // yjj 2008-12-23
		}
	}
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�
	if(Glb().m_baFavor.GetCount())
	{
		for (UINT i=0;i<uCount;i++) 
		{
			CAFCNameItem * pNameItem=FindNameItem((CAFCGameListItem *)m_InsideArray.GetAt(0),(pOnLineCountPtr+i)->uID);
			if (pNameItem!=NULL)
			{
				pNameItem->m_NameInfo.m_uOnLineCount=(pOnLineCountPtr+i)->uOnLineCount;
				pNameItem->UpdateOnLineCount();
				//pNameItem->ExpandItem (1);    // yjj 2008-12-23
			}
		}
	}	
	///////////////////////////////////////////////////////////
    UpdateOnLineCount();

	return true;
}

CAFCNameItem *CGameListCtrl::FindNameItem(UINT uID, UINT uKind)
{
	//ͳ������
	UINT uOnLineCount = 0L;
	CAFCGameListItem * pGameKindItem = GetKindParentItem();//��ȡ����

	if (uKind == 999)
	{
		CAFCKindItem *pKind = FindKindItem(uKind);
		if (pKind != NULL)
		{
			CAFCNameItem * pNameItem = FindNameItem(pKind, uID);
			if (pNameItem != NULL)
				return pNameItem;
		}
		return NULL;
	}

	CString sdp;
	sdp.Format("sdp_list kind����=%d", pGameKindItem->m_ItemPtrArray.GetCount());
	OutputDebugString(sdp);

	for (INT_PTR i=0; i<pGameKindItem->m_ItemPtrArray.GetCount(); i++)
	{
		CAFCKindItem * pKindItem = (CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);

		if (NULL != pKindItem)
		{
			sdp.Format("sdp_list uKindID=%u", pKindItem->m_KindInfo.uKindID);
			OutputDebugString(sdp);
		}

		if (NULL != pKindItem && pKindItem->m_KindInfo.uKindID != 999)
		{
			sdp.Format("sdp_list szKindName=%s", pKindItem->m_KindInfo.szKindName);
			OutputDebugString(sdp);

			for (INT_PTR j=0; j<pKindItem->m_ItemPtrArray.GetCount(); j++)
			{
				CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (NULL != pNameItem)
				{
					sdp.Format("sdp_list uNameID=%u", pNameItem->m_NameInfo.uNameID);
					OutputDebugString(sdp);

					if(pNameItem->m_NameInfo.uNameID == uID)
						return pNameItem;
				}
			}
		}
	}
	return NULL;
}

//��������
bool CGameListCtrl::HandleIOnLineData(MSG_GP_SR_OnLineStruct * pOnLineCountPtr, UINT uCount)
{
	for (UINT i=0;i<uCount;i++) 
	{
//		CAFCNameItem * pNameItem=FindNameItem(FindKindItem((pOnLineCountPtr+i)->uKindID),(pOnLineCountPtr+i)->uNameID);
		//if (pNameItem!=NULL) pNameItem->m_uOnLineCount=(pOnLineCountPtr+i)->uOnLineCount;
	}
	return true;
}

//չ������
bool CGameListCtrl::ExpandItemFirstRoom()
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����

	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (pNameItem!=NULL)
				{
					pNameItem->ExpandItem (4);
					return true;
				}
			}
		}
	}
	return true;
}

/*******************************************************************************************************/
CAFCRoomItem *CGameListCtrl::FindRoomItem(UINT uRoomID)
{
	//ͳ������
	UINT uOnLineCount=0L;
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (pNameItem!=NULL)
				{
					for (INT_PTR rr=0;rr<pNameItem->m_ItemPtrArray.GetCount();rr++)
					{
						CAFCRoomItem * proomItem=(CAFCRoomItem *)pNameItem->m_ItemPtrArray.GetAt (rr);
						if(proomItem != NULL && proomItem->m_RoomInfo.uRoomID == uRoomID)return proomItem;
					
					}
				}
			}
		}
	}
	return NULL;
}

//�û��ڹ����������ʱר��
CAFCRoomItem *CGameListCtrl::FindRoomItem(UINT uKindID, UINT uNameID, CAFCNameItem* &pNameItem)
{
	CAFCKindItem *pKindItem = FindKindItem(uKindID);
	if (pKindItem!=NULL && pKindItem->m_uDataType==GLK_GAME_KIND)
	{
		pNameItem = FindNameItem(pKindItem, uNameID);
		if (pNameItem!=NULL && pNameItem->m_uDataType==GLK_GAME_NAME)
		{
			CAFCRoomItem* pRoomItem= NULL;
			for (INT_PTR rr=0;rr<pNameItem->m_ItemPtrArray.GetCount();rr++)
			{
				pRoomItem = (CAFCRoomItem *)pNameItem->m_ItemPtrArray.GetAt (rr);
				if (pRoomItem != NULL)
				{
					if (pRoomItem->m_bIsContest)
					{
						return pRoomItem;
					}
				}
			}
		}
	}
	return NULL;
}

CAFCRoomItem *CGameListCtrl::FindRoomItemEx(UINT uRoomID, UINT uKind)
{
	//ͳ������
	UINT uOnLineCount=0L;
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����

	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL && (uKind == -1 || (uKind == 999 && pKindItem->m_KindInfo.uKindID == uKind)))
		{
			for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);

				if (pNameItem->m_uDataType != GLK_GAME_NAME)
				{
					continue;
				}

				if (pNameItem!=NULL)
				{
					for (INT_PTR rr=0;rr<pNameItem->m_ItemPtrArray.GetCount();rr++)
					{
						CAFCRoomItem * proomItem=(CAFCRoomItem *)pNameItem->m_ItemPtrArray.GetAt (rr);
						if (proomItem != NULL)
						{
							if (proomItem->m_bIsContest)
							{
								for (int k = 0; k < proomItem->m_RoomContest.size(); k++)
								{
									if (proomItem->m_RoomContest[k].uRoomID == uRoomID)
									{
										return proomItem;
									}

								}
							}
							else
							{
								if(proomItem->m_RoomInfo.uRoomID == uRoomID)
								{
									return proomItem;
								}
							}
						}
					}
				}
			}
		}
	}

	return NULL;
}

// ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
bool CGameListCtrl::m_bUseNumShow = false;
void CGameListCtrl::KindOnLineShow(TCHAR* pchDest, TCHAR* pchName, UINT uNum)
{
    // ʹ��������ʾ
    if(true == m_bUseNumShow)
    {
        wsprintf(pchDest,TEXT("%s(%d)"),pchName,uNum);
        return;
    }

    wsprintf(pchDest,TEXT("%s"),pchName);
}
bool CGameListCtrl::RoomOnLineShow(UINT uOldPeopleCount,TCHAR* pchDest, ComRoomInfo* pRoomInfo,COLORREF& statusColor,CString& statusText)
{
    UINT uPeopleCount = pRoomInfo->uPeopleCount;
    if(uPeopleCount > 0xFFFF) uPeopleCount = 0;

	static int iUpdataTag = 0;

	if(iUpdataTag % 10000 != 0 && uOldPeopleCount != 999)
		return false;

	++iUpdataTag;

    // ʹ��������ʾ
    if(true == m_bUseNumShow)
    {
		if(uOldPeopleCount == uPeopleCount && uOldPeopleCount != 999)//��ͬ������ˢ�� 999�Ƚ����⣬������ʼ���ģ����봦��
			return false;
        wsprintf(pchDest,TEXT("(%3d)%s"),uPeopleCount,pRoomInfo->szGameRoomName);
        return true;
    }
	
	CBcfFile fOrder(CBcfFile::GetAppPath()+"Order.bcf");
	UINT uLevel1 = fOrder.GetKeyVal("CrowdedShow", "Level1Count", 30);
	UINT uLevel2 = fOrder.GetKeyVal("CrowdedShow", "Level2Count", 80);
	UINT uLevel3 = fOrder.GetKeyVal("CrowdedShow", "Level3Count", 100);
	UINT uLevel4 = fOrder.GetKeyVal("CrowdedShow", "Level4Count", 100);

	UINT uFull = pRoomInfo->uDeskCount * pRoomInfo->uDeskPeople;
    if(0 == uFull) uFull = 1;

    float fRate = 0.0f;
    fRate = (float)uPeopleCount/uFull*100;
	float fRate2 = 0.0f;
	fRate2 = (float)uOldPeopleCount/uFull*100;

	if(fRate <= uLevel1)
	{
		if(fRate2 <= uLevel1 && uOldPeopleCount != 999)//��ͬ������ˢ�� 999�Ƚ����⣬������ʼ���ģ����봦��
			return false;
		statusColor = (COLORREF)fOrder.GetKeyVal("CrowdedShow", "Level1Color", 4227200);
		statusText = fOrder.GetKeyVal("CrowdedShow", "Level1Text", "����");
	}
	else if(fRate <= uLevel2)
	{
		if(fRate2 <= uLevel2 && uOldPeopleCount != 999)//��ͬ������ˢ�� 999�Ƚ����⣬������ʼ���ģ����봦��
			return false;
		statusColor = (COLORREF)fOrder.GetKeyVal("CrowdedShow", "Level2Color", 33023);
		statusText = fOrder.GetKeyVal("CrowdedShow", "Level2Text", "��æ");
	}
	else if(fRate <= uLevel3)
	{
		if(fRate2 <= uLevel3 && uOldPeopleCount != 999)//��ͬ������ˢ�� 999�Ƚ����⣬������ʼ���ģ����봦��
			return false;
		statusColor = (COLORREF)fOrder.GetKeyVal("CrowdedShow", "Level3Color", 255);
		statusText = fOrder.GetKeyVal("CrowdedShow", "Level3Text", "ӵ��");
	}
	else if(fRate <= uLevel4)
	{
		if(fRate2 <= uLevel4 && uOldPeopleCount != 999)//��ͬ������ˢ�� 999�Ƚ����⣬������ʼ���ģ����봦��
			return false;
		statusColor = (COLORREF)fOrder.GetKeyVal("CrowdedShow", "Level4Color", 4194432);
		statusText = fOrder.GetKeyVal("CrowdedShow", "Level4Text", "����");
	}
	else
	{
		statusColor = (COLORREF)fOrder.GetKeyVal("CrowdedShow", "Level4Color", 4194432);
		statusText = fOrder.GetKeyVal("CrowdedShow", "Level4Text", "����");
	}
	wsprintf(pchDest,TEXT("%s"), pRoomInfo->szGameRoomName);

	return true;
}
void CGameListCtrl::RootOnLineShow(TCHAR* pchDest, TCHAR* pchFormat, UINT uNum)
{
    // ʹ��������ʾ
    if(true == m_bUseNumShow)
    {
        CString strFormat;
		strFormat.Format(pchFormat);
		if (strFormat.Compare("��Ϸ����") >= 0)
		{
			strcpy(pchDest,TEXT("��Ϸ����"));
		}
		else
		{
			wsprintf(pchDest,pchFormat,uNum);
		}

        return;
    }

    // ����ʾ����
    TCHAR chBuffer[100] = {0};
    TCHAR* ptr = NULL;
    ptr = strstr(pchFormat, "(");
    if(NULL != ptr)
    {
        memcpy(chBuffer, pchFormat, (int)(ptr) - (int)pchFormat);
        strcpy(pchDest, chBuffer);
    }
    else
    {
        strcpy(pchDest,TEXT("��Ϸ����"));
    }
}

// ��Ⲣȷ����ʾ��ʽ
void CGameListCtrl::CheckIfUseNumShow()
{
    CBcfFile fOrder(CBcfFile::GetAppPath()+"Order.bcf");
    UINT uShow = fOrder.GetKeyVal("CrowdedShow", "PeopleCountShow", 1);
    m_bUseNumShow = uShow;
}

//��������
bool CGameListCtrl::HandleRoomOnLineData(DL_GP_RoomListPeoCountStruct * pOnLineCountPtr, UINT uCount)
{
	for (UINT i=0;i<uCount;i++) 
	{
		CAFCRoomItem * pRoomItem=FindRoomItemEx((pOnLineCountPtr+i)->uID);
		if (pRoomItem!=NULL)
		{
			if (!pRoomItem->m_bIsContest)
			{
				UINT uOldPeopleCount = pRoomItem->m_RoomInfo.uPeopleCount;
				pRoomItem->m_RoomInfo.uPeopleCount=(pOnLineCountPtr+i)->uOnLineCount;

				{
					TCHAR szBuffer[100];
					memset(szBuffer,0,sizeof(szBuffer));
					if(pRoomItem->m_RoomInfo.uPeopleCount<0 || pRoomItem->m_RoomInfo.uPeopleCount>0x7fffffff)
						pRoomItem->m_RoomInfo.uPeopleCount=0;
				
					// PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
					//wsprintf(szBuffer,TEXT("(%3d)%s"),pRoomItem->m_RoomInfo.uPeopleCount,pRoomItem->m_RoomInfo.szGameRoomName);
					COLORREF statuscolor = RGB(0,0,0);
					CString strStatusText = "";
					if(!RoomOnLineShow(uOldPeopleCount,szBuffer, &pRoomItem->m_RoomInfo,statuscolor,strStatusText))
						continue;

					SetItemText(pRoomItem->m_hCurrentItem,szBuffer);
					SetItemStatusColor(pRoomItem->m_hCurrentItem,pRoomItem->m_RoomInfo.uRoomID,RGB(0,0,0),statuscolor,strStatusText);
				}
			}
			else
			{
				ComRoomInfo* _p = FindContestRoomItem(pRoomItem, (pOnLineCountPtr+i)->uID);
				if (_p != NULL)
				{
					_p->uPeopleCount = (pOnLineCountPtr+i)->uOnLineCount;
					int _pcount = 0;
					for (int k = 0; k < pRoomItem->m_RoomContest.size(); k++)
					{
						_pcount += pRoomItem->m_RoomContest[k].uPeopleCount;
					}
					pRoomItem->m_iContestPeopleCount = _pcount;
				}
			}
		}


		pRoomItem=FindRoomItemEx((pOnLineCountPtr+i)->uID, 999);
		if (pRoomItem!=NULL)
		{
			if (!pRoomItem->m_bIsContest)
			{
				UINT uOldPeopleCount = pRoomItem->m_RoomInfo.uPeopleCount;
				pRoomItem->m_RoomInfo.uPeopleCount=(pOnLineCountPtr+i)->uOnLineCount;

				{
					TCHAR szBuffer[100];
					memset(szBuffer,0,sizeof(szBuffer));
					if(pRoomItem->m_RoomInfo.uPeopleCount<0 || pRoomItem->m_RoomInfo.uPeopleCount>0x7fffffff)
						pRoomItem->m_RoomInfo.uPeopleCount=0;

					// PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
					//wsprintf(szBuffer,TEXT("(%3d)%s"),pRoomItem->m_RoomInfo.uPeopleCount,pRoomItem->m_RoomInfo.szGameRoomName);
					COLORREF statuscolor = RGB(0,0,0);
					CString strStatusText = "";
					if(!RoomOnLineShow(uOldPeopleCount,szBuffer, &pRoomItem->m_RoomInfo,statuscolor,strStatusText))
						continue;

					SetItemText(pRoomItem->m_hCurrentItem,szBuffer);
					SetItemStatusColor(pRoomItem->m_hCurrentItem,pRoomItem->m_RoomInfo.uRoomID,RGB(0,0,0),statuscolor,strStatusText);
				}
			}
			else
			{
				ComRoomInfo* _p = FindContestRoomItem(pRoomItem, (pOnLineCountPtr+i)->uID);
				if (_p != NULL)
				{
					_p->uPeopleCount = (pOnLineCountPtr+i)->uOnLineCount;
					int _pcount = 0;
					for (int k = 0; k < pRoomItem->m_RoomContest.size(); k++)
					{
						_pcount += pRoomItem->m_RoomContest[k].uPeopleCount;
					}
					pRoomItem->m_iContestPeopleCount = _pcount;
				}
			}
		}
	}
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�

	if(Glb().m_baFavor.GetCount())
	{
		CAFCGameListItem * pGameKindItem=(CAFCGameListItem *)m_InsideArray.GetAt(0);//��ȡ����
		for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
		{
			CAFCNameItem * pNameItem=(CAFCNameItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
			if (pNameItem!=NULL) 
			{
				CAFCRoomItem * pRoomItem=FindRoomItem(pNameItem,(pOnLineCountPtr+i)->uID);
				if (pRoomItem!=NULL)
				{
					UINT uOldPeopleCount = pRoomItem->m_RoomInfo.uPeopleCount;
					pRoomItem->m_RoomInfo.uPeopleCount=(pOnLineCountPtr+i)->uOnLineCount;

					{
						TCHAR szBuffer[100];
						memset(szBuffer,0,sizeof(szBuffer));
						if(pRoomItem->m_RoomInfo.uPeopleCount<0 || pRoomItem->m_RoomInfo.uPeopleCount>0x7fffffff)
							pRoomItem->m_RoomInfo.uPeopleCount=0;
						
                        // PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
                        //wsprintf(szBuffer,TEXT("(%3d)%s"),pRoomItem->m_RoomInfo.uPeopleCount,pRoomItem->m_RoomInfo.szGameRoomName);
						COLORREF statuscolor = RGB(0,0,0);
						CString strStatusText = "";
						if(!RoomOnLineShow(uOldPeopleCount,szBuffer, &pRoomItem->m_RoomInfo,statuscolor,strStatusText))
							continue;

						SetItemText(pRoomItem->m_hCurrentItem,szBuffer);
						SetItemStatusColor(pRoomItem->m_hCurrentItem,pRoomItem->m_RoomInfo.uRoomID,RGB(0,0,0),statuscolor,strStatusText);
					}
				}
			}
		}
	}
	///////////////////////////////////////////////////////////

	return true;
}
//��������
CAFCGameListItem::CAFCGameListItem()
{
	m_uUpdateTime=0;
	m_iGameHotDegree = 0;
	m_iGameHotType =0;
	m_hCurrentItem=NULL;
	m_pParentItem=NULL;
	m_uDataType=GLK_NO_DATA;
	return;
}

//��������
CAFCGameListItem::~CAFCGameListItem()
{
	for (int i=0;i<m_ItemPtrArray.GetCount();i++)
	{
		delete (CAFCGameListItem *)m_ItemPtrArray.GetAt(i);
		m_ItemPtrArray.SetAt(i,NULL);
	}
	return;
}

//��ʼ����
bool CAFCGameListItem::Init(CAFCGameListItem * pParentItem)
{
	m_pParentItem=pParentItem;
	return true;
}

//�����ϲ���һ��Ҷ�ӽӵ�����
HTREEITEM CAFCGameListItem::AddTreeData(TCHAR * szText, UINT uIconIndex, LPARAM lParam, bool bNeedIco)
{
	TV_INSERTSTRUCT InsertInf;
	memset(&InsertInf,0,sizeof(InsertInf));
	InsertInf.item.cchTextMax=100;
	InsertInf.hParent=m_hCurrentItem; // ���ڵ�
	InsertInf.hInsertAfter=TVI_LAST;
	InsertInf.item.iImage=uIconIndex;
	InsertInf.item.iSelectedImage=uIconIndex;
	InsertInf.item.pszText=szText;
	InsertInf.item.lParam=lParam;
	CAFCGameListItem *pListItem = (CAFCGameListItem*)lParam;
	if (pListItem && pListItem->m_uDataType == GLK_GAME_ROOM)
	{
		InsertInf.itemex.iIntegral = 1;
	}
	else if (pListItem)
	{
		InsertInf.itemex.iIntegral = 2;
	}
	else
	{
		InsertInf.itemex.iIntegral = 1;
	}
	InsertInf.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_INTEGRAL;

	return m_pTreeCtrl->InsertItem(&InsertInf);
}

//չ��������
bool CAFCGameListItem::ExpandItem(int iExpendCount)
{
	if ((iExpendCount==-1)||(iExpendCount>0))//�������ŵ���-1��������Ŵ���0��������Ų�����0��
	{
		if (m_hCurrentItem!=NULL) m_pTreeCtrl->Expand(m_hCurrentItem,TVE_EXPAND);//�����ǰ�Ϊ�գ���ôչ����ǰ��
		//if ((iExpendCount>1)||(iExpendCount==-1))//�������ŵ���-1��������Ŵ���1��������Ų�����1��������0��
		//{
		//	if (iExpendCount>0) iExpendCount--;//�������Ŵ���0����ô����ż�1
		//	for (int i=0;i<m_ItemPtrArray.GetCount();i++)//������������
		//	{
		//		CAFCGameListItem * pListItem=(CAFCGameListItem *)m_ItemPtrArray.GetAt(i);//////////�õ� ��Ϸ�б�����
		//		//if (pListItem!=NULL) pListItem->ExpandItem(iExpendCount);//�����Ϸ�б����Ϊ�գ���ôչ����Ϸ�б�����
		//		//2008-12-23
		//	}
		//}
		return true;
	}
	return false;
}
//����ӷ�������ȡ������
bool CAFCGameListItem::UpdateGetListItem()//////////�����б�AddTreeData
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;
	
	if (m_uDataType == GLK_GAME_NAME)/////////////���������������Ϸ����
	{
		if (m_ItemPtrArray.GetCount()==0)////������
		{
			TCHAR szBuffer[100];
			memset(szBuffer,0,sizeof(szBuffer));
			if ((m_uUpdateTime+LS_TIME_OUT) <= (LONG)time(NULL)) 
			{
				str = fMsg.GetKeyVal("MainRoom","ReadingInfor","��ȡ��Ϣ...");
				wsprintf(szBuffer,str.GetBuffer());//�������ʱ��+�б�ʱ��С�ڵ�ǰʱ�䣬��ô�Ͷ�ȡ��Ϣ
			}
			else
			{
				str = fMsg.GetKeyVal("MainRoom","NoRooms","û�з���");
				wsprintf(szBuffer,str.GetBuffer());//������㳬ʱ����ʾû��ʵ��
			}
			TCHAR sz[200];
			wsprintf(sz,"%d,%d",m_uUpdateTime,(LONG)time(NULL));
			//Writelog(sz);
			if (m_hUpdateItem==NULL) 
				m_hUpdateItem=AddTreeData(szBuffer,IND_NO_ROOM,NULL);///�����ȡ�������ڿգ���ô��ȡ����=�������ʾ//û�з����ͼ��
			else
			{
				m_pTreeCtrl->SetItemText(m_hUpdateItem,szBuffer);//���ؼ�ָ��������ʾ����
			}
		}
		else if (m_hUpdateItem!=NULL) ///������
		{
			for (INT_PTR i=0;i<m_ItemPtrArray.GetCount();i++)//ѭ����������
			{
				CAFCRoomItem * pRoomItem=(CAFCRoomItem *)m_ItemPtrArray.GetAt(i);///��Ϸ�����������������
				pRoomItem->m_uUpdateTime=(LONG)time(NULL);//����ʱ��=��ǰʱ��
			}
			m_pTreeCtrl->DeleteItem(m_hUpdateItem);//ɾ����ȡ����
			m_hUpdateItem=NULL;////////��ȡ����Ϊ��
		}
	}
	return true;
}
//�Ƿ���Ҫ���� 
bool CAFCGameListItem::IsNeedUpdate()//�Ƿ���Ҫ����
{
	if ((m_uDataType==GLK_GAME_NAME)&&((m_uUpdateTime+LS_TIME_OUT)<=time(NULL))) return true;////////���������������Ϸ���֣����Ҹ���ʱ��+�б�ʱ��С�ڵ�ǰʱ��
	return false;
}
//ɾ����ʱ����
bool CAFCGameListItem::DeleteOutTimeItem()
{
	
	if (m_uDataType==GLK_GAME_NAME)//���������������Ϸ����
	{
		for (INT_PTR i=0;i<m_ItemPtrArray.GetCount();i++)//////////ѭ����������
		{
			CAFCRoomItem * pRoomItem=(CAFCRoomItem *)m_ItemPtrArray.GetAt(i);//��Ϸ�����������������
			if (pRoomItem != NULL)
			{
				bool bTimeOut = pRoomItem->m_uUpdateTime+LS_TIME_OUT<=(LONG)time(NULL);
				

				if (bTimeOut)//�����Ϸ�������Ϊ�գ������Ҹ���ʱ��+�б�ʱ��С�ڵ�ǰʱ��
				{
					m_ItemPtrArray.RemoveAt(i--);///////ɾ����������
					m_pTreeCtrl->DeleteItem(pRoomItem->m_hCurrentItem);/////ɾ��������
					delete pRoomItem;////ɾ����Ϸ��������
				}
			}

		}
		UpdateGetListItem();/////�����б�
	}
	return true;
}


//ɾ������ʾ�ķ���
bool CAFCGameListItem::DeleteHideRoom()
{
	if (m_uDataType==GLK_GAME_NAME)//���������������Ϸ����
	{
		for (INT_PTR i=0;i<m_ItemPtrArray.GetCount();i++)//////////ѭ����������
		{
			CAFCRoomItem * pRoomItem=(CAFCRoomItem *)m_ItemPtrArray.GetAt(i);//��Ϸ�����������������
			if (pRoomItem != NULL)
			{
				bool bHide = true;//pRoomItem->m_RoomInfo.iAgencyID != 0 && GetMainRoom()->m_PlaceUserInfo.iAgencyID != pRoomItem->m_RoomInfo.iAgencyID;
				if (bHide)//�����Ϸ�������Ϊ�գ������Ҹ���ʱ��+�б�ʱ��С�ڵ�ǰʱ��
				{
					m_ItemPtrArray.RemoveAt(i--);///////ɾ����������
					m_pTreeCtrl->DeleteItem(pRoomItem->m_hCurrentItem);/////ɾ��������
					delete pRoomItem;////ɾ����Ϸ��������
				}
			}
		}
		m_uUpdateTime -= LS_TIME_OUT;
		UpdateGetListItem();/////�����б�
	}
	return true;
}
//������Ϸ����AddTreeData
bool CAFCGameListItem::AddGameKind(ComKindInfo * pGameKind,int initType)
{
	try
	{
		CAFCKindItem * pGameKindItem = new CAFCKindItem; // ����Ϸ��������
		if (pGameKindItem != NULL) // �����Ϸ�������Ϊ��
		{
			pGameKindItem->Init(this); 

			pGameKindItem->m_KindInfo = *pGameKind; // ��������

			pGameKindItem->m_uDataType = initType;

			/*
			TCHAR szBuffer[100];
			wsprintf(szBuffer,TEXT("%d"),pGameKindItem->m_KindInfo.Head.bDataType); // ��һ����������������
			AfxMessageBox(szBuffer);
			AfxMessageBox(pGameKindItem->m_KindInfo.szKindName);
			*/

			// ��������
			m_ItemPtrArray.Add(pGameKindItem); 
			
			// duanxiaohui 2011-11-21 ����Ŀ¼�����ⲿͼ��
			if (pGameKind->uParentKindID != 0)
			{
				CString strPath;
				strPath.Format("%s\\icon\\%s.ico",m_skinmgr.GetKeyVal(m_skinmgr.GetSkinBcfFileName()), pGameKind->szKindName);
				int nIconIndex = CPlaceResource::LoadExtendIcon(strPath);
				if (nIconIndex == 0)
				{
					nIconIndex = IND_KIND2_ICON;  // Ĭ�϶���Ŀ¼ͼ��
				}

				pGameKindItem->m_hCurrentItem = AddTreeData(pGameKindItem->m_KindInfo.szKindName, nIconIndex, (LPARAM)pGameKindItem); 
			}
			else
			{
				pGameKindItem->m_hCurrentItem = AddTreeData(pGameKindItem->m_KindInfo.szKindName, IND_KIND_ICON, (LPARAM)pGameKindItem); 
			}

			//pGameKindItem->m_hCurrentItem = AddTreeData(pGameKindItem->m_KindInfo.szKindName, IND_KIND_ICON, (LPARAM)pGameKindItem); 

			// end duanxiaohui 

			return true;
		}
	}
	catch (...) {}
	return false;
}

//������Ϸ����
CAFCNameItem* CAFCGameListItem::AddGameName(ComNameInfo * pGameName,int initType)
{
	try
	{
		CAFCNameItem * pGameNameItem=new CAFCNameItem;
		
		if (pGameNameItem!=NULL)
		{
			//��������
			pGameNameItem->Init(this);
			pGameNameItem->m_NameInfo=*pGameName;
			pGameNameItem->m_NameInfo.Head.bDataType = initType;
			pGameNameItem->m_uDataType = initType;
					
			//������Ϸ�ȶȺ���������	ADD BY DWJ
			SetGameDegreeAndType(pGameName->uNameID, &pGameNameItem->m_iGameHotDegree, &pGameNameItem->m_iGameHotType);

			m_ItemPtrArray.Add(pGameNameItem);
			pGameNameItem->m_hCurrentItem=AddTreeData(pGameNameItem->m_NameInfo.szGameName,CPlaceResource::GetListIconIndex(pGameName),(LPARAM)pGameNameItem);
			return pGameNameItem;
		}
	}
	catch (...) {}
	return NULL;
}

/*----------------------------------------------------------------------------*/
//�����б���Ϸ�ȶȺ���������	ADD BY DWJ 20140318
bool CAFCGameListItem::SetGameDegreeAndType(UINT	uGameNameID, int *iDegree, int *iType)
{
	if(NULL == iDegree || NULL == iType)
	{
		return false;
	}

	CString s=CBcfFile::GetAppPath (false);
	CBcfFile f( s + "bzgame.bcf");
	CString sGameID;
	sGameID.Format("%d",uGameNameID);
	//��Ϸ�ȶ�
	int iNum=f.GetKeyVal ("GameDegreeSet",sGameID,3);
	*iDegree = iNum;
	//��Ϸ�ȶ�����
	iNum=f.GetKeyVal ("GameHotTypeSet",sGameID,1);
	*iType = iNum;

	return true;
}
/*----------------------------------------------------------------------------*/
//������Ϸ����
bool CAFCGameListItem::AddGameRoom(ComRoomInfo * pGameRoom)
{
	try
	{
		CAFCRoomItem * pGameRoomItem=new CAFCRoomItem;
		if (pGameRoomItem!=NULL)
		{
			//��������		
			pGameRoomItem->Init(this);
			pGameRoomItem->m_RoomInfo=*pGameRoom;
			pGameRoomItem->m_uUpdateTime=(LONG)time(NULL);
			pGameRoomItem->m_uDataType = GLK_GAME_ROOM;
			//m_ItemPtrArray.Add(pGameRoomItem);
			TCHAR szBuffer[100];
			memset(szBuffer,0,sizeof(szBuffer));
			
            // PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
            //wsprintf(szBuffer,TEXT("(%3d)%s"),pGameRoom->uPeopleCount,pGameRoom->szGameRoomName);
			COLORREF statuscolor = RGB(0,0,0);
			CString strStatusText = "";
			CGameListCtrl::RoomOnLineShow(999,szBuffer, pGameRoom,statuscolor,strStatusText);

			//��Ϸ����״̬
			pGameRoomItem->m_sRoomState.Format("%s",strStatusText);
			m_ItemPtrArray.Add(pGameRoomItem);
			
			if (pGameRoom->bVIPRoom)
				pGameRoomItem->m_hCurrentItem=AddTreeData(szBuffer,7,(LPARAM)pGameRoomItem,false); ///< ���«2����VIP����
			else
				pGameRoomItem->m_hCurrentItem=AddTreeData(szBuffer,100/*IND_ROOM_ICON*/,(LPARAM)pGameRoomItem,false);
			
			if (m_pTreeCtrl != NULL)
				m_pTreeCtrl->SetItemStatusColor(pGameRoomItem->m_hCurrentItem,pGameRoomItem->m_RoomInfo.uRoomID,RGB(0,0,0),statuscolor,strStatusText);

			return true;
		}
	}
	catch (...) {}
	return false;
}

//�����������
bool CAFCGameListItem::AddContestItem(ComRoomInfo * pGameRoom)
{
		try
	{
		CAFCRoomItem * pGameRoomItem=new CAFCRoomItem;
		if (pGameRoomItem!=NULL)
		{
			//��������		
			pGameRoomItem->Init(this);
			pGameRoomItem->m_RoomInfo=*pGameRoom;
			pGameRoomItem->m_uUpdateTime=(LONG)time(NULL);
			m_ItemPtrArray.Add(pGameRoomItem);
			TCHAR szBuffer[100];
			memset(szBuffer,0,sizeof(szBuffer));

			pGameRoomItem->m_bIsContest = true;
			pGameRoomItem->m_RoomContest.push_back(*pGameRoom);
			
            // PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
            //wsprintf(szBuffer,TEXT("(%3d)%s"),pGameRoom->uPeopleCount,pGameRoom->szGameRoomName);
			COLORREF statuscolor = RGB(0,0,0);
			CString strStatusText = "";
			CGameListCtrl::RoomOnLineShow(999,szBuffer, pGameRoom,statuscolor,strStatusText);

			if (pGameRoom->bVIPRoom)
				pGameRoomItem->m_hCurrentItem=AddTreeData(szBuffer,7,(LPARAM)pGameRoomItem); ///< ���«2����VIP����
			else
				pGameRoomItem->m_hCurrentItem=AddTreeData(szBuffer,IND_ROOM_ICON,(LPARAM)pGameRoomItem);

			if (m_pTreeCtrl != NULL)
				m_pTreeCtrl->SetItemStatusColor(pGameRoomItem->m_hCurrentItem,pGameRoomItem->m_RoomInfo.uRoomID,RGB(0,0,0),statuscolor,strStatusText);

			return true;
		}
	}
	catch (...) {}
	return false;
}

//�����ڲ�����
bool CAFCGameListItem::AddInsideData(GameInsideInfo * pInsideData, UINT uIconIndex)
{
	try
	{
		CAFCInsideItem * pInsideItem=new CAFCInsideItem;
		if (pInsideItem!=NULL)
		{
			//��������
			pInsideItem->Init(this);
			pInsideItem->m_InsideInfo=*pInsideData;
			m_ItemPtrArray.Add(pInsideItem);
			pInsideItem->m_hCurrentItem=AddTreeData(pInsideItem->m_InsideInfo.szDisplayName,uIconIndex,(LPARAM)pInsideItem);
			return true;
		}
	}
	catch (...) {}
	return false;
}
	
/*******************************************************************************************************/

//���õ�����Ϸ������������
bool CAFCNameItem::UpdateOnLineCount()
{
	TCHAR szBuffer[100];
	memset(szBuffer,0,sizeof(szBuffer));
	if(m_NameInfo.m_uOnLineCount<0 || m_NameInfo.m_uOnLineCount>0x7fffffff)
		m_NameInfo.m_uOnLineCount=0;
	
    // PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
    //wsprintf(szBuffer,TEXT("%s(%d)"),m_NameInfo.szGameName,m_NameInfo.m_uOnLineCount);
    CGameListCtrl::KindOnLineShow(szBuffer, m_NameInfo.szGameName,m_NameInfo.m_uOnLineCount);
	
	m_pTreeCtrl->SetItemText(m_hCurrentItem,szBuffer);
	return true;
}

/*******************************************************************************************************/

//���캯��
CGameListCtrl::CGameListCtrl()
{
    OutputDebugString("wzd:client CGameListCtrl()");
    m_iCount = 0;
    m_bISInit     = true;
	m_hLastMMItem = NULL;
	for (int i=0; i!=m_vecGameImage.size(); ++i)
	{
		delete m_vecGameImage[i].pimg_GameEx;
	}
	m_vecGameImage.clear();	
	m_mapStatusColor.clear();
	m_mapHitemRoomID.clear();
}

//��������
CGameListCtrl::~CGameListCtrl()
{
	for (int i=0;i<m_InsideArray.GetCount();i++)
	{
		delete (CAFCGameListItem *)m_InsideArray.GetAt(i);
		m_InsideArray.SetAt(i,NULL);
	}
	m_mapStatusColor.clear();
	m_mapHitemRoomID.clear();
	return;
}

//��ʼ������,ͬʱ���Ӹ��ӵ�
bool CGameListCtrl::Init()
{
	//���ÿؼ�
	//SendMessage(SET_LINE_COLOR,0,(LPARAM)CGameImageLink::m_ColorResource.m_crListTxColor);

	//������Ϸ�б�����
	UINT uIconIndex[]=
	{
		IND_LIST_ROOT,IND_ROOM_NO_ICON,IND_LIST_ROOT,IND_LIST_NO_INSTALL,
		IND_LIST_ROOT,IND_LIST_ROOT,IND_LIST_ROOT,
	};
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�
	if(Glb().m_baFavor.GetCount())
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str = fMsg.GetKeyVal("MainRoom","RecentGameList","����������Ϸ�б�");

		GameInsideInfo InsideInfo;
		InsideInfo.Head.uSize=sizeof(InsideInfo);
		InsideInfo.Head.bDataType=GLK_GAME_USE;
		InsideInfo.uTrunID=1000;
		lstrcpy(InsideInfo.szDisplayName,str.GetBuffer());
		bool bAddExtendIcon=false;
		CString s=CBcfFile::GetAppPath();/////����·��
		CBcfFile f( s + "bzgame.bcf");

		CString strICO=f.GetKeyVal(Glb().m_key,"FavorICO","");
		//DebugPrintf("strICO = [%s]", strICO);
		if(strICO!="")
		{
			UINT uIconExtendIndex=CPlaceResource::LoadExtendIcon(strICO);
			//DebugPrintf("CPlaceResource::LoadExtendIcon(strICO) = [%d]", uIconExtendIndex);
			if(uIconExtendIndex>0)
			{
				if(AddRootItem(&InsideInfo,uIconExtendIndex))
					bAddExtendIcon=true;
			}
		}
		if(!bAddExtendIcon)
			AddRootItem(&InsideInfo,uIconIndex[0]);
	}	
	///////////////////////////////////////////////////////////

	//�����ϲ��������
	CAFCGameListItem::m_pTreeCtrl=this;

	for (int i=0;i<1;i++)                                                        //��Ϸ�������б��ʼ�� Tree2��Tree3���������б�
	{	
		GameInsideInfo InsideInfo;
		InsideInfo.Head.uSize=sizeof(InsideInfo);
		InsideInfo.Head.bDataType=GLK_GAME_USE;
		InsideInfo.uTrunID=1000+i;
		lstrcpy(InsideInfo.szDisplayName,Glb().m_Tree[i]);
		bool bAddExtendIcon=false;
		if(Glb().m_Tree[i+20]!="")
		{
			UINT uIconExtendIndex=CPlaceResource::LoadExtendIcon(Glb().m_Tree[i+20]);
			if(uIconExtendIndex>0)
			{
				if(AddRootItem(&InsideInfo,uIconExtendIndex))
					bAddExtendIcon=true;
			}
		}
		if(!bAddExtendIcon)
			AddRootItem(&InsideInfo,uIconIndex[0]);
	}

	//����������������ɫ
	CString s = CBcfFile::GetAppPath();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key= TEXT("GameListWnd");

	m_crGridLine = f.GetKeyVal(key,"list_itemline_color",(__int64)RGB(230,230,230));
	m_crGridHotBk1 = f.GetKeyVal(key,"list_itemhot_bkcolor1",(__int64)RGB(17,142,206));
	m_crGridHotBk2 = f.GetKeyVal(key,"list_itemhot_bkcolor2",(__int64)RGB(161,217,245));
	m_crGridHotTxt1 = f.GetKeyVal(key,"list_itemhot_textcolor1",(__int64)RGB(255,255,255));
	m_crGridHotTxt2 = f.GetKeyVal(key,"list_itemhot_textcolor2",(__int64)RGB(0,0,0));
	m_crGridBk = f.GetKeyVal(key,"list_bkcolor",(__int64)RGB(255,255,255));

	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	m_vecGameImage.clear();
	//��Ϸ���䱳��
	wsprintf(path,"%simage\\game\\GameRoom.bmp",s);
	m_imgGameRoom.SetLoadInfo(path, CGameImageLink::m_bAutoLock);
	//��Ϸ���ͱ���
	wsprintf(path,"%simage\\game\\GameKind.bmp",s);
	m_imgGameKind.SetLoadInfo(path, CGameImageLink::m_bAutoLock);
	//��Ϸ���Ʊ���
	for (int i=0; i!=IMG_GAME_NAME_COUNT; ++i)
	{
		wsprintf(path,"%simage\\game\\GameName_%d.bmp",s, i);
		m_ArrimgGameName[i].SetLoadInfo(path, CGameImageLink::m_bAutoLock);		
	}
	//�����б�����logo
	wsprintf(path,"%sleft\\list_logo_game.png",skinfolder);
	m_imgLogo.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	DWORD dwStyles=GetWindowLong(m_hWnd,GWL_STYLE);//��ȡ������ԭ���
	dwStyles|=TVS_FULLROWSELECT;
	SetWindowLong(m_hWnd,GWL_STYLE,dwStyles);//���÷��

	//�����б��۵�/���۵�logo
	wsprintf(path,"%sleft\\list_bn_fold.png",skinfolder);
	m_imgFold.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sleft\\list_bn_unfold.png",skinfolder);
	m_imgUnFold.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//�б�ָ��»���
	wsprintf(path,"%sleft\\UnderLine.png",skinfolder);
	m_imgUnderLine.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//��Ϸ�ȶ�
	wsprintf(path,"%sleft\\xing.png",skinfolder);
	m_imgHotDegree.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//��Ϸ�ȶ�logo
	wsprintf(path,"%sleft\\lv.png",skinfolder);
	m_imgHotLogo[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sleft\\huang.png",skinfolder);
	m_imgHotLogo[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sleft\\hong.png",skinfolder);
	m_imgHotLogo[2].SetLoadInfo(path,CGameImageLink::m_bAutoLock);


	//������Ϸ
	wsprintf(path,"%sleft\\re.png",skinfolder);
	m_imgHotGame.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//����Ϸ
	wsprintf(path,"%sleft\\xin.png",skinfolder);
	m_imgNewGame.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//��Ϸ���͵�ɫ��
	wsprintf(path,"%sleft\\BgColor.png",skinfolder);
	m_imgBgColor.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	
	wsprintf(path,"%sleft\\set_bt.bmp",skinfolder);
	m_imgAD.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//�����������
	m_curHand  = LoadCursor(NULL,MAKEINTRESOURCE(32649));
	m_curArrow = LoadCursor(NULL,IDC_ARROW);

	CBcfFile fi( s + "bzgame.bcf");
	SetItemHeight(fi.GetKeyVal(Glb().m_key,"GameTreeHeight",24));

    // PengJiLin, 2011-7-20, �����Ƿ���������ʽ��ʾ
    CheckIfUseNumShow();

	//�鿴����ť
	m_btRule.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BTN_Rule);

	wsprintf(path,"%sleft\\rule.png",skinfolder);
	m_btRule.LoadButtonBitmap(path,false);
	m_btRule.ShowWindow(SW_HIDE);



	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	bool bShow = fMsg.GetKeyVal("MainRoom","ShowLastLoginGame", 0);
	if (bShow)
	{
		ComKindInfo p;
		ZeroMemory(&p, sizeof(p));
		p.Head.bDataType = 1;
		p.Head.bDataType = 80;

		CString str = fMsg.GetKeyVal("MainRoom","ShowLastLoginGameText", "�������¼����Ϸ");
		strcpy(p.szKindName, str);

		p.uKindID = 999;

		HandleKindData(&p, 1);
	}


	return true;
}

void CGameListCtrl::LoadSkin()
{
	//������Ϸ�б�����
	//UINT uIconIndex[]=
	//{
	//	IND_LIST_ROOT,IND_ROOM_NO_ICON,IND_LIST_ROOT,IND_LIST_NO_INSTALL,
	//	IND_LIST_ROOT,IND_LIST_ROOT,IND_LIST_ROOT,
	//};
	/////////////////////////////////////////////////////////////
	/////Kylin 20090107  ��������Ϸ�б�
	//if(Glb().m_baFavor.GetCount())
	//{
	//	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	//	CString str = fMsg.GetKeyVal("MainRoom","RecentGameList","����������Ϸ�б�");

	//	GameInsideInfo InsideInfo;
	//	InsideInfo.Head.uSize=sizeof(InsideInfo);
	//	InsideInfo.Head.bDataType=GLK_GAME_USE;
	//	InsideInfo.uTrunID=1000;
	//	lstrcpy(InsideInfo.szDisplayName,str.GetBuffer());
	//	bool bAddExtendIcon=false;
	//	CString s=CBcfFile::GetAppPath();/////����·��
	//	CBcfFile f( s + "bzgame.bcf");

	//	CString strICO=f.GetKeyVal(Glb().m_key,"FavorICO","");
	//	//DebugPrintf("strICO = [%s]", strICO);
	//	if(strICO!="")
	//	{
	//		UINT uIconExtendIndex=CPlaceResource::LoadExtendIcon(strICO);
	//		//DebugPrintf("CPlaceResource::LoadExtendIcon(strICO) = [%d]", uIconExtendIndex);
	//		if(uIconExtendIndex>0)
	//		{
	//			if(AddRootItem(&InsideInfo,uIconExtendIndex))
	//				bAddExtendIcon=true;
	//		}
	//	}
	//	if(!bAddExtendIcon)
	//		AddRootItem(&InsideInfo,uIconIndex[0]);
	//}	
	/////////////////////////////////////////////////////////////

	////�����ϲ��������
	//CAFCGameListItem::m_pTreeCtrl=this;

	//for (int i=0;i<1;i++)                                                        //��Ϸ�������б��ʼ�� Tree2��Tree3���������б�
	//{	
	//	GameInsideInfo InsideInfo;
	//	InsideInfo.Head.uSize=sizeof(InsideInfo);
	//	InsideInfo.Head.bDataType=GLK_GAME_USE;
	//	InsideInfo.uTrunID=1000+i;
	//	lstrcpy(InsideInfo.szDisplayName,Glb().m_Tree[i]);
	//	bool bAddExtendIcon=false;
	//	if(Glb().m_Tree[i+20]!="")
	//	{
	//		UINT uIconExtendIndex=CPlaceResource::LoadExtendIcon(Glb().m_Tree[i+20]);
	//		if(uIconExtendIndex>0)
	//		{
	//			if(AddRootItem(&InsideInfo,uIconExtendIndex))
	//				bAddExtendIcon=true;
	//		}
	//	}
	//	if(!bAddExtendIcon)
	//		AddRootItem(&InsideInfo,uIconIndex[0]);
	//}

	//����������������ɫ
	CString s = CBcfFile::GetAppPath();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key= TEXT("GameListWnd");

	m_crGridLine = f.GetKeyVal(key,"list_itemline_color",(__int64)RGB(230,230,230));
	m_crGridHotBk1 = f.GetKeyVal(key,"list_itemhot_bkcolor1",(__int64)RGB(17,142,206));
	m_crGridHotBk2 = f.GetKeyVal(key,"list_itemhot_bkcolor2",(__int64)RGB(161,217,245));
	m_crGridHotTxt1 = f.GetKeyVal(key,"list_itemhot_textcolor1",(__int64)RGB(255,255,255));
	m_crGridHotTxt2 = f.GetKeyVal(key,"list_itemhot_textcolor2",(__int64)RGB(0,0,0));
	m_crGridBk = f.GetKeyVal(key,"list_bkcolor",(__int64)RGB(255,255,255));

	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	//�����б�����logo
	wsprintf(path,"%sleft\\list_logo_game.png",skinfolder);
	m_imgLogo.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//�����б��۵�/���۵�logo
	wsprintf(path,"%sleft\\list_bn_fold.png",skinfolder);
	m_imgFold.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sleft\\list_bn_unfold.png",skinfolder);
	m_imgUnFold.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,"%sleft\\UnderLine.png",skinfolder);
	m_imgUnderLine.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//��Ϸ�ȶ�
	wsprintf(path,"%sleft\\xing.png",skinfolder);
	m_imgHotDegree.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//��Ϸ�ȶ�logo
	wsprintf(path,"%sleft\\lv.png",skinfolder);
	m_imgHotLogo[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sleft\\huang.png",skinfolder);
	m_imgHotLogo[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sleft\\hong.png",skinfolder);
	m_imgHotLogo[2].SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//������Ϸ
	wsprintf(path,"%sleft\\re.png",skinfolder);
	m_imgHotGame.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//����Ϸ
	wsprintf(path,"%sleft\\xin.png",skinfolder);
	m_imgNewGame.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//��Ϸ���͵�ɫ��
	wsprintf(path,"%sleft\\BgColor.png",skinfolder);
	m_imgBgColor.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,"%sleft\\set_bt.bmp",skinfolder);
	m_imgAD.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//�����������
	//m_curHand  = LoadCursor(NULL,MAKEINTRESOURCE(32649));
	//m_curArrow = LoadCursor(NULL,IDC_ARROW);

	CBcfFile fi( s + "bzgame.bcf");
	SetItemHeight(fi.GetKeyVal(Glb().m_key,"GameTreeHeight",24));

	// PengJiLin, 2011-7-20, �����Ƿ���������ʽ��ʾ
	//CheckIfUseNumShow();

	//�鿴����ť
	//m_btRule.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE,CRect(0,0,0,0),this,IDC_BTN_Rule);

	wsprintf(path,"%sleft\\rule.png",skinfolder);
	m_btRule.LoadButtonBitmap(path,false);
	m_btRule.ShowWindow(SW_HIDE);

	//CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	//bool bShow = fMsg.GetKeyVal("MainRoom","ShowLastLoginGame", 0);
	//if (bShow)
	//{
	//	ComKindInfo p;
	//	ZeroMemory(&p, sizeof(p));
	//	p.Head.bDataType = 1;
	//	p.Head.bDataType = 80;

	//	CString str = fMsg.GetKeyVal("MainRoom","ShowLastLoginGameText", "�������¼����Ϸ");
	//	strcpy(p.szKindName, str);

	//	p.uKindID = 999;

	//	HandleKindData(&p, 1);
	//}
}
//����
LRESULT CGameListCtrl::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	//OnPaint();
	Invalidate();
	return LRESULT(0);
}
//��ʼ������,ͬʱ���Ӹ��ӵ�
bool CGameListCtrl::Init2()
{
	//���ÿؼ�
	//SendMessage(SET_LINE_COLOR,0,(LPARAM)CGameImageLink::m_ColorResource.m_crListTxColor);
	
	//������Ϸ�б�����
	UINT uIconIndex[]=
	{
		IND_LIST_ROOT,IND_ROOM_NO_ICON,IND_LIST_ROOT,IND_LIST_NO_INSTALL,
		IND_LIST_ROOT,IND_LIST_ROOT,IND_LIST_ROOT,
	};
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�

	CAFCGameListItem::m_pTreeCtrl=this;
	if(Glb().m_baFavor.GetCount())
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str = fMsg.GetKeyVal("MainRoom","RecentGameList","����������Ϸ�б�");

		GameInsideInfo InsideInfo;
		InsideInfo.Head.uSize=sizeof(InsideInfo);
		InsideInfo.Head.bDataType=GLK_GAME_USE;
		InsideInfo.uTrunID=1000;
		lstrcpy(InsideInfo.szDisplayName,str.GetBuffer());
		AddRootItem(&InsideInfo,uIconIndex[0]);
	}	
	///////////////////////////////////////////////////////////

	//�����ϲ��������
	for (int i=0;i<Glb().m_TreeCount2;i++)
	{	
		GameInsideInfo InsideInfo;
		InsideInfo.Head.uSize=sizeof(InsideInfo);
		InsideInfo.Head.bDataType=GLK_GAME_USE;
		InsideInfo.uTrunID=1000+i;
		lstrcpy(InsideInfo.szDisplayName,Glb().m_Tree2[i]);
		if (!AddRootItem(&InsideInfo,uIconIndex[0])) return false;
	}

    // PengJiLin, 2011-7-20, �����Ƿ���������ʽ��ʾ
    CheckIfUseNumShow();

	return true;
}
//�������
bool CGameListCtrl::AddRootItem(GameInsideInfo * pInsideData, UINT uIconIndex)
{
	try
	{
		bool haveAddress = false;
		if (pInsideData->Head.bDataType == GLK_GAME_ADD)
		{
			haveAddress =true;
		}

		CAFCInsideItem * pInsideItem=new CAFCInsideItem();
		if (haveAddress)
		{
			pInsideItem->m_uDataType = GLK_GAME_ADD;
		}
		if (pInsideItem!=NULL)
		{
			//��������
			pInsideItem->Init(NULL);
			pInsideItem->m_InsideInfo=*pInsideData;
			m_InsideArray.Add(pInsideItem);

			//��������
			TV_INSERTSTRUCT InsertInf;
			memset(&InsertInf,0,sizeof(InsertInf));
			InsertInf.item.cchTextMax=100;
			InsertInf.hInsertAfter=TVI_LAST;
			InsertInf.itemex.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_INTEGRAL;
			InsertInf.itemex.iImage=uIconIndex;
			InsertInf.itemex.iSelectedImage=uIconIndex;//ѡ���ͼ��
			InsertInf.itemex.iIntegral = 1;
			InsertInf.itemex.pszText=pInsideData->szDisplayName;
			InsertInf.itemex.lParam=(LPARAM)pInsideItem;

			pInsideItem->m_hCurrentItem=InsertItem(&InsertInf);

			if (pInsideData->Head.bDataType == GLK_GAME_ADD)
			{
				SetItemColor(pInsideItem->m_hCurrentItem,RGB(255,0,0));
			}
			return true;
		}
	}
	catch (...) {}
	return false;
}


//��������//�����ϼ�����Ϸ����AddTreeData
bool CGameListCtrl::HandleKindData(ComKindInfo * pKindInfoPtr, UINT uKindCount)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (UINT i=0;i<uKindCount;i++) 
	{
		if ((pKindInfoPtr + i)->uParentKindID != 0)
		{
			// ����Ƿ���ڸ���ϷID����
			CAFCKindItem * pGameParentKindItem = FindKindItem((pKindInfoPtr + i)->uParentKindID);
			if (pGameParentKindItem != NULL)
			{
				if (FindKindItem((pKindInfoPtr+i)->uKindID) == NULL)
				{
					pGameParentKindItem->AddGameKind(pKindInfoPtr + i); // �����ϼ�������Ϸ����AddTreeData
				}

				continue ;
			}
		}

		if (FindKindItem((pKindInfoPtr+i)->uKindID)==NULL)
				pGameKindItem->AddGameKind(pKindInfoPtr+i);//�����ϼ�����Ϸ����AddTreeData
	}
	return true;
}

bool CGameListCtrl::HandleUrlData(CAFCGameUrlItem * pUrlInfoPtr, UINT uDatatype)
{

	HTREEITEM root = GetKindParentItem()->m_hCurrentItem;
	for (int i = 0; i < m_GameUrl.GetCount(); i++)
	{
		CAFCGameUrlItem* p = (CAFCGameUrlItem *)m_GameUrl.GetAt(i);
		if (p->m_uDataType == uDatatype) 
		{
			root = p->m_hCurrentItem;
		}
	}
	try
	{
		CAFCGameUrlItem* p = new CAFCGameUrlItem();
		if (p!=NULL)
		{ 
			//��������
			//*p = *pUrlInfoPtr;
			p->m_uDataType = pUrlInfoPtr->m_uDataType;
			p->m_stName = pUrlInfoPtr->m_stName;
			p->m_stUrl = pUrlInfoPtr->m_stUrl;

			m_GameUrl.Add(p);

			//��������
			TV_INSERTSTRUCT InsertInf;
			memset(&InsertInf,0,sizeof(InsertInf));
			InsertInf.hParent = root;
			InsertInf.item.cchTextMax = 100;
			InsertInf.hInsertAfter = TVI_LAST;
			InsertInf.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;

			// duanxiaohui 2011-11-21 �����ⲿicon
			CString strPath;
			strPath.Format("%s\\icon\\%s.ico", m_skinmgr.GetKeyVal(m_skinmgr.GetSkinBcfFileName()), pUrlInfoPtr->m_stName);
			int nIconIndex = CPlaceResource::LoadExtendIcon(strPath);
			if (nIconIndex == 0)
			{
				nIconIndex = IND_KIND_ICON;  // Ĭ��Iconͼ��
			}
			// end duanxiaohui

			InsertInf.item.iImage =  nIconIndex;
			InsertInf.item.iSelectedImage =  nIconIndex; //ѡ���ͼ��
			InsertInf.item.pszText = p->m_stName.GetBuffer();
			InsertInf.item.lParam = (LPARAM)p;
			p->m_hCurrentItem = InsertItem(&InsertInf);
			return true;
		}
	}
	catch (...) {}
}


bool CGameListCtrl::HandleLoginGame(int iGameID)
{
	for (int i = 0; i < m_LoginGameInfo.size(); i++)
	{
		if (m_LoginGameInfo[i].iGameID == iGameID)
		{
			CAFCNameItem* _pName = FindNameItem(iGameID);
			CAFCKindItem* _pKind = FindKindItem(999);
			if (_pName == NULL || _pKind == NULL) return false;

			CAFCNameItem* _pNameAdd = FindNameItem(_pKind, iGameID);
			if (_pNameAdd != NULL) delete _pNameAdd;
			_pNameAdd = _pKind->AddGameName(&_pName->m_NameInfo);

			for (int i = 0; i < _pName->m_ItemPtrArray.GetCount(); i++)
			{
				CAFCRoomItem* _pRoom = (CAFCRoomItem*)_pName->m_ItemPtrArray.GetAt(i);
				if (_pRoom != NULL)
					_pNameAdd->AddGameRoom(&_pRoom->m_RoomInfo);
			}
		}
	}

	return true;
}

bool CGameListCtrl::HandleLoginGameName(ComNameInfo * pNameInfoPtr)
{
	CAFCKindItem* _pKind = FindKindItem(999);
	if (_pKind == NULL || pNameInfoPtr == NULL) 
		return false;
	if (FindNameItem(_pKind, pNameInfoPtr->uNameID)==NULL) 
		_pKind->AddGameName(pNameInfoPtr);
}

void CGameListCtrl::ClearLoginGameName()
{
	CAFCKindItem* _pKind = FindKindItem(999);

	for (INT_PTR i=0;i< _pKind->m_ItemPtrArray.GetCount();i++)//////////ѭ����������
	{
		CAFCNameItem * pName = (CAFCNameItem *)_pKind->m_ItemPtrArray.GetAt(i);//��Ϸ�����������������
		if (pName!=NULL)//�����Ϸ�������Ϊ�գ������Ҹ���ʱ��+�б�ʱ��С�ڵ�ǰʱ��
		{
			_pKind->m_ItemPtrArray.RemoveAt(i--);///////ɾ����������
			DeleteItem(pName->m_hCurrentItem);/////ɾ��������
			delete pName;////ɾ����Ϸ��������
		}
	}
}
 
//��������/////�����ϼ�����Ϸ����
bool CGameListCtrl::HandleNameData(ComNameInfo * pNameInfoPtr, UINT uNameCount)
{
	static bool FirstInit = false;
	int iCount = 0;
	int iDeleteCount = 0;

	CString s = CBcfFile::GetAppPath();
	TCHAR path[MAX_PATH];
	CGameImageEx *pimg_GameEx = NULL;
	ComNameInfo *pNameInfo = NULL;

	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����

	for (UINT i=0;i<uNameCount;i++)                      //ɾ�������б����ظ�����   add by wyl   11-5-16
	{
		for ( SELLLISTINFO::iterator it = m_SellListVec.begin(); it != m_SellListVec.end(); it++ )
		{
			iCount ++;
			if ((*it).NameID == (pNameInfoPtr+i)->uNameID)
			{
				(*it).NameID = 0;
				iDeleteCount++;
			}
		}

		//��¼��Ϸ�������Ϸ��������   yjj 090310
	//	CAppealDlg::WriteGameInfoFile((pNameInfoPtr+i)->szGameName,(pNameInfoPtr+i)->uNameID);

		CAFCKindItem * pKindItem=FindKindItem((pNameInfoPtr+i)->uKindID);
		if ((pKindItem!=NULL)&&(FindNameItem(pKindItem,(pNameInfoPtr+i)->uNameID)==NULL))
		{
			UINT uIndex = pKindItem->m_ItemPtrArray.GetCount()%IMG_GAME_NAME_COUNT;	//��Ϸ������ͼ�ֻ�
			pNameInfo = pNameInfoPtr + i;
			pKindItem->AddGameName(pNameInfoPtr+i);///////�����ϼ�����Ϸ����			
			//�����ϷͼƬ
			if (pNameInfo)
			{
				vector<CMyGameImageEx>::iterator iter = find_if(m_vecGameImage.begin(), m_vecGameImage.end(), GameImageExEQ(pNameInfo->uNameID));
				if (iter == m_vecGameImage.end())
				{
					wsprintf(path,"%simage\\game\\%d.png", s, pNameInfo->uNameID);
					if (CBcfFile::IsFileExist(path))
					{
						pimg_GameEx = new CGameImageEx;
						pimg_GameEx->SetLoadInfo(path, CGameImageLink::m_bAutoLock);

						CMyGameImageEx myGame(uIndex, pNameInfo->uNameID, pimg_GameEx);

						m_vecGameImage.push_back(myGame);
					}		
				}
			}

		}

		if ((pNameInfoPtr+i)->uKindID == 1)//��һ����Ϸ�����б��չ��(�����б����)
			pKindItem->ExpandItem(4);

		///////////////////////////////////////////////////////////
		///Kylin 20090107  ��������Ϸ�б�
		if(Glb().m_baFavor.GetCount())
		{
			bool canadd;

			canadd=false;
			for(int j=0;j<Glb().m_baFavor.GetCount();j++)
			{
				if(Glb().m_baFavor[j]==(pNameInfoPtr+i)->uNameID)
				{
					canadd=true;
					break;
				}
			}
			if(canadd)
			{
				CAFCGameListItem * pKindItem;
				pKindItem=(CAFCGameListItem *)m_InsideArray.GetAt(0);
				if ((pKindItem!=NULL)&&(FindNameItem(pKindItem,(pNameInfoPtr+i)->uNameID)==NULL)) 
				{
					pKindItem->AddGameName(pNameInfoPtr+i);///////�����ϼ�����Ϸ����
				}
			}
		}
		///////////////////////////////////////////////////////////
	}

	//���������б�				add by wyl    11-5-16
	s=CBcfFile::GetAppPath (false);
	CBcfFile f( s + "bzgame.bcf");

	int TreeCount=f.GetKeyVal ("BZW","TreeCount",1);

	if ( 2 <= TreeCount && ((CMainRoomEx*)GetMainRoom())->m_HaveSellList && !FirstInit)
	{
		FirstInit = true;
		UINT uIconIndex[]=
		{
			IND_LIST_ROOT,IND_ROOM_NO_ICON,IND_LIST_ROOT,IND_LIST_NO_INSTALL,
			IND_LIST_ROOT,IND_LIST_ROOT,IND_LIST_ROOT,
		};

		for (int i=1;i<3;i++)
		{	
			GameInsideInfo InsideInfo;
			InsideInfo.Head.uSize=sizeof(InsideInfo);
			if (i != 2)
			{
				InsideInfo.Head.bDataType=GLK_GAME_USE;
			}
			else
			{
				InsideInfo.Head.bDataType=GLK_GAME_ADD;
			}

			InsideInfo.uTrunID=1000+i;
			lstrcpy(InsideInfo.szDisplayName,Glb().m_Tree[i]);
			bool bAddExtendIcon=false;
			if(Glb().m_Tree[i+20]!="")
			{
				UINT uIconExtendIndex=CPlaceResource::LoadExtendIcon(Glb().m_Tree[i+20]);
				if(uIconExtendIndex>0)
				{
					if(AddRootItem(&InsideInfo,uIconExtendIndex))
						bAddExtendIcon=true;
				}
			}
			if(!bAddExtendIcon)
				AddRootItem(&InsideInfo,uIconIndex[0]);
		}
		//////////////////////////////////////////////////////////////////////////
		ComKindInfo *pSellInfo = new ComKindInfo;

		for ( SELLLISTINFO::iterator it = m_SellListVec.begin(); it != m_SellListVec.end(); it++ )
		{
			if ((*it).NameID == 0)
				continue;
			
			memcpy(pSellInfo->szKindName,(*it).szKindName,61);
			pSellInfo->uKindID = (*it).KindID;
			CAFCGameListItem * pGameKindItem=(CAFCGameListItem *)GetSellListParentItem();	//��ȡ���� 
			if (FindSellKindItem((*it).KindID) == NULL)
			{
				pGameKindItem->AddGameKind(pSellInfo);

				pGameKindItem->m_pTreeCtrl->SetItemColor(pGameKindItem->m_hCurrentItem,RGB(0,0,255));

				if (pGameKindItem->m_pTreeCtrl->ItemHasChildren(pGameKindItem->m_hCurrentItem))
				{
					HTREEITEM hItem = pGameKindItem->m_pTreeCtrl->GetChildItem(pGameKindItem->m_hCurrentItem);
					pGameKindItem->m_pTreeCtrl->SetItemColor(hItem,RGB(0,0,255));

					while (hItem != NULL)
					{
						hItem = pGameKindItem->m_pTreeCtrl->GetNextSiblingItem(hItem);
						pGameKindItem->m_pTreeCtrl->SetItemColor(hItem,RGB(0,0,255));
						CString buffer;
						buffer.Format("����ط���ɫ��Ϸ(%d)",iCount/uNameCount - iDeleteCount);
						pGameKindItem->m_pTreeCtrl->SetItemText(pGameKindItem->m_hCurrentItem,buffer);
					}
				}
				pGameKindItem->ExpandItem(2);
			}

			// ����Ϸ����
			pGameKindItem = FindSellKindItem((*it).KindID);
			ComNameInfo *pNameInfoPtrSellList = new ComNameInfo;
			pNameInfoPtrSellList->Head.bDataType = 10;
			pNameInfoPtrSellList->uKindID = (*it).KindID;
			memcpy(pNameInfoPtrSellList->szGameName,(*it).szGameName,60);
			char precessName[61]={0};
			wsprintf(precessName,"%d.exe",(*it).NameID);
			memcpy(pNameInfoPtrSellList->szGameProcess,precessName,60);
			pNameInfoPtrSellList->uNameID = (*it).NameID;
			pGameKindItem->AddGameName(pNameInfoPtrSellList,(*it).ParamID + 100);
			

			//������Ϸ�� ��ɫ����
			if (pGameKindItem->m_pTreeCtrl->ItemHasChildren(pGameKindItem->m_hCurrentItem))
			{
				HTREEITEM hItem = pGameKindItem->m_pTreeCtrl->GetChildItem(pGameKindItem->m_hCurrentItem);
				pGameKindItem->m_pTreeCtrl->SetItemColor(hItem,RGB(0,0,255));

				while (hItem != NULL)
				{
					hItem = pGameKindItem->m_pTreeCtrl->GetNextSiblingItem(hItem);
					pGameKindItem->m_pTreeCtrl->SetItemColor(hItem,RGB(0,0,255));
				}
			}
		}
	}

	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�

	if(Glb().m_baFavor.GetCount())
	{
		CAFCGameListItem * pGameKindItem=(CAFCGameListItem *)m_InsideArray.GetAt(0);//��ȡ����
		bool bdeal;
		do
		{
			bdeal=false;
			for(int j=0;j<Glb().m_baFavor.GetCount();j++)
			{
				if(FindNameItem(Glb().m_baFavor[j])==NULL)
				{
					Glb().m_baFavor.RemoveAt(j);
					bdeal=true;
					break;
				}
			}
			for(int j=0;j<Glb().m_baFavorSave.GetCount();j++)
			{
				if(FindNameItem(Glb().m_baFavorSave[j])==NULL)
				{
					Glb().m_baFavorSave.RemoveAt(j);
					bdeal=true;
					break;
				}
			}
			
		}
		while(bdeal);

	}
	///////////////////////////////////////////////////////////

	return true;
}
//��������
bool CGameListCtrl::HandleRoomData(ComRoomInfo * pRoomInfoPtr, UINT uRoomCount, UINT uKindID, UINT uNameID)
{
	CAFCKindItem* pKind = FindKindItem(uKindID);
	CAFCNameItem * pNameItem=FindNameItem(pKind,uNameID);///������Ϸ����
	if (pNameItem!=NULL)
	{
		for (UINT i=0;i<uRoomCount;i++)
		{

			CAFCRoomItem * pRoomItem = NULL;

			if ((pRoomInfoPtr+i)->dwRoomRule & GRR_CONTEST)
			{
				pRoomItem = FindRoomItemByContest(pNameItem, pRoomInfoPtr+i);
				if (pRoomItem != NULL)
				{
					ComRoomInfo* _p = FindContestRoomItem(pRoomItem, (pRoomInfoPtr+i)->uRoomID);
					if (_p != NULL)
					{
						pRoomItem->m_uUpdateTime=(LONG)time(NULL);
						lstrcpy(pRoomItem->m_RoomInfo.szGameRoomName,(pRoomInfoPtr+i)->szGameRoomName);
					}
					else
					{
						pRoomItem->m_RoomContest.push_back(*(pRoomInfoPtr+i));
					}
				}
				else
				{
					pNameItem->AddContestItem(pRoomInfoPtr+i);
				}
			}
			else
			{
				pRoomItem=FindRoomItem(pNameItem,(pRoomInfoPtr+i)->uRoomID);/////////�������Ϸ���������ٲ��ҷ���ID

				if (pRoomItem!=NULL)
				{
					TCHAR szBuffer[100];
					memset(szBuffer,0,sizeof(szBuffer));
					pRoomItem->m_uUpdateTime=(LONG)time(NULL);
					lstrcpy(pRoomItem->m_RoomInfo.szGameRoomName,(pRoomInfoPtr+i)->szGameRoomName);
					////////////////////////////////////////////
					///Kylin 20090204 ��ֹ������
				
					// PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
					//wsprintf(szBuffer,TEXT("(%3d)%s"),(pRoomInfoPtr+i)->uPeopleCount<0?0:(pRoomInfoPtr+i)->uPeopleCount,(pRoomInfoPtr+i)->szGameRoomName);

					////////////////////////////////////////////
					COLORREF statuscolor = RGB(0,0,0);
					CString strStatusText = "";
					RoomOnLineShow(999,szBuffer, (pRoomInfoPtr+i),statuscolor,strStatusText);

					SetItemText(pRoomItem->m_hCurrentItem,szBuffer);
					SetItemStatusColor(pRoomItem->m_hCurrentItem,pRoomItem->m_RoomInfo.uRoomID,RGB(0,0,0),statuscolor,strStatusText);
				}
				else pNameItem->AddGameRoom(pRoomInfoPtr+i);////////�������Ϸ�����������Ӳ��ҷ���ID
			}
		}
	}
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�

	if(Glb().m_baFavor.GetCount())
	{
		CAFCNameItem * pNameItem=FindNameItem((CAFCGameListItem *)m_InsideArray.GetAt(0),uNameID);///������Ϸ����
		if (pNameItem!=NULL)
		{
			for (UINT i=0;i<uRoomCount;i++)
			{
				CAFCRoomItem * pRoomItem=FindRoomItem(pNameItem,(pRoomInfoPtr+i)->uRoomID);/////////�������Ϸ���������ٲ��ҷ���ID

				if (pRoomItem!=NULL)
				{
					TCHAR szBuffer[100];
					memset(szBuffer,0,sizeof(szBuffer));
					pRoomItem->m_uUpdateTime=(LONG)time(NULL);
					lstrcpy(pRoomItem->m_RoomInfo.szGameRoomName,(pRoomInfoPtr+i)->szGameRoomName);
					////////////////////////////////////////////
					///Kylin 20090204 ��ֹ������
					
                    // PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
                    //wsprintf(szBuffer,TEXT("(%3d)%s"),(pRoomInfoPtr+i)->uPeopleCount<0?0:(pRoomInfoPtr+i)->uPeopleCount,(pRoomInfoPtr+i)->szGameRoomName);

					COLORREF statuscolor = RGB(0,0,0);
					CString strStatusText = "";
					RoomOnLineShow(999,szBuffer, (pRoomInfoPtr+i),statuscolor,strStatusText);

					SetItemText(pRoomItem->m_hCurrentItem,szBuffer);
					SetItemStatusColor(pRoomItem->m_hCurrentItem,pRoomItem->m_RoomInfo.uRoomID,RGB(0,0,0),statuscolor,strStatusText);
				}
				else pNameItem->AddGameRoom(pRoomInfoPtr+i);////////�������Ϸ�����������Ӳ��ҷ���ID
			}
		}
	}
	///////////////////////////////////////////////////////////

	return true;
}
//������������
CAFCKindItem * CGameListCtrl::FindKindItem(UINT uKindID)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);

		// duanxiaohui 20111107 add
		//if ((pKindItem!=NULL)&&(pKindItem->m_KindInfo.uKindID==uKindID)) return pKindItem;

		if (pKindItem != NULL)
		{
			if (pKindItem->m_KindInfo.uKindID == uKindID)
			{
				return pKindItem;
			}
			else if (pKindItem->m_KindInfo.uKindID != 999)
			{
				for (INT_PTR j = 0; j < pKindItem->m_ItemPtrArray.GetCount(); j++)
				{
					CAFCKindItem * pKindItem2 = (CAFCKindItem *)pKindItem->m_ItemPtrArray.GetAt(j);
					if ((pKindItem2 != NULL) && (pKindItem2->m_KindInfo.uKindID==uKindID))
					{
						return pKindItem2;
					}
				}
			}
		}
		// end duanxiaohui
	}

	return NULL;
}

CAFCKindItem * CGameListCtrl::FindSellKindItem(UINT uKindID)                         
{
	CAFCGameListItem * pGameKindItem=GetSellListParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		
		// duanxiaohui 20111107 add
		/*if ((pKindItem!=NULL)&&(pKindItem->m_KindInfo.uKindID==uKindID))
		return pKindItem;*/

		if (pKindItem != NULL)
		{
			if (pKindItem->m_KindInfo.uKindID == uKindID)
			{
				return pKindItem;
			}
			else
			{
				for (INT_PTR j = 0; j < pKindItem->m_ItemPtrArray.GetCount(); j++)
				{
					CAFCKindItem * pKindItem2 = (CAFCKindItem *)pKindItem->m_ItemPtrArray.GetAt(j);
					if ((pKindItem2 != NULL) && (pKindItem2->m_KindInfo.uKindID==uKindID))
					{
						return pKindItem2;
					}
				}
			}
		}
		// end duanxiaohui
	}
	return NULL;
}
//������������
CAFCNameItem * CGameListCtrl::FindNameItem(CAFCKindItem * pKindItem, UINT uNameID)
{
	if (pKindItem!=NULL)
	{
		for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount();i++)
		{
			CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
			if ((pNameItem!=NULL)&&(pNameItem->m_NameInfo.uNameID==uNameID)) return pNameItem;
		}
	}
	return NULL;
}

//������������
CAFCNameItem * CGameListCtrl::FindNameItem(CAFCGameListItem * pKindItem, UINT uNameID)
{
	if (pKindItem!=NULL)
	{
		for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount();i++)
		{
			CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
			if ((pNameItem!=NULL)&&(pNameItem->m_NameInfo.uNameID==uNameID)) return pNameItem;
		}
	}
	return NULL;
}
//���ҷ�������
CAFCRoomItem * CGameListCtrl::FindRoomItem(CAFCNameItem * pNameItem, UINT uRoomID, ComRoomInfo* pRoomInfo)
{
	if (pNameItem!=NULL)
	{
		//if (pRoomInfo->dwRoomRule & GRR_CONTEST)
		//{
		//	for (INT_PTR i=0;i<pNameItem->m_ItemPtrArray.GetCount();i++)
		//	{
		//		CAFCRoomItem * pRoomItem=(CAFCRoomItem *)pNameItem->m_ItemPtrArray.GetAt(i);
		//		if (pRoomItem != NULL && pRoomItem->m_bIsContest)
		//		{
		//			for (int k = 0; k < pRoomItem->m_RoomContest.size(); k++)
		//			{
		//				if (pRoomItem->m_RoomContest[k].uRoomID == uRoomID) return pRoomItem;
		//			}
		//		}
		//	}
		//}
		//else
		//{
			for (INT_PTR i=0;i<pNameItem->m_ItemPtrArray.GetCount();i++)
			{
				CAFCRoomItem * pRoomItem=(CAFCRoomItem *)pNameItem->m_ItemPtrArray.GetAt(i);
				if (pRoomItem!=NULL)
				{
					if (pRoomItem->m_RoomInfo.uRoomID==uRoomID) return pRoomItem;
				}
			}
		//}
	}
	return NULL;
}


//���ҷ�������
CAFCRoomItem * CGameListCtrl::FindRoomItemByContest(CAFCNameItem * pNameItem, ComRoomInfo* pRoomInfo)
{
	if (pNameItem!=NULL)
	{
		if (pRoomInfo->dwRoomRule & GRR_CONTEST)
		{
			for (INT_PTR i=0;i<pNameItem->m_ItemPtrArray.GetCount();i++)
			{
				CAFCRoomItem * pRoomItem=(CAFCRoomItem *)pNameItem->m_ItemPtrArray.GetAt(i);
				if (pRoomItem != NULL && pRoomItem->m_bIsContest && pRoomInfo->iContestID == pRoomItem->m_RoomInfo.iContestID)
				{
					return pRoomItem;
				}
			}
		}
	}
	return NULL;
}


//���ұ�����������
ComRoomInfo * CGameListCtrl::FindContestRoomItem(CAFCRoomItem * pRoomItem, int iRoomID)
{
	if (pRoomItem != NULL && pRoomItem->m_bIsContest)
	{
		for (int i = 0; i < pRoomItem->m_RoomContest.size(); i++)
		{
			if (pRoomItem->m_RoomContest[i].uRoomID == iRoomID)
				return &(pRoomItem->m_RoomContest[i]);
		}
	}
	return NULL;
}

//�����ȡ����
bool CGameListCtrl::FillGetListItem()
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR j = 0; j < pKindItem->m_ItemPtrArray.GetCount(); j++)
			{
				CAFCKindItem * pKindItem2 = (CAFCKindItem *)pKindItem->m_ItemPtrArray.GetAt(j);

				// �ж�����Ϸ���ͻ�����Ϸ����
				if (pKindItem2->m_uDataType == GLK_GAME_KIND)
				{
					// ����Ϸ����, �����Ŀ¼Ϊ����Ŀ¼, ��Ŀ¼�»�����Ϸ
					for (INT_PTR k = 0; k < pKindItem2->m_ItemPtrArray.GetCount(); k++)
					{
						CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem2->m_ItemPtrArray.GetAt(k);
						if (pNameItem != NULL) 
						{
							pNameItem->m_uUpdateTime=0;
							pNameItem->UpdateGetListItem();
						}
					}
				}
				else
				{
					CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
					if (pNameItem != NULL) 
					{
						pNameItem->m_uUpdateTime = 0;
						pNameItem->UpdateGetListItem();
					}
				}
			}
		}			
	}
	///////////////////////////////////////////////////////////
	///��������Ϸ�б�

	if(Glb().m_baFavor.GetCount())
	{
		CAFCGameListItem * pGameKindItem=(CAFCGameListItem *)m_InsideArray.GetAt(0);//��ȡ����
		for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
		{
			CAFCNameItem * pNameItem=(CAFCNameItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
			if (pNameItem!=NULL) 
			{
				pNameItem->m_uUpdateTime=0;
				pNameItem->UpdateGetListItem();
			}
		}
	}
	///////////////////////////////////////////////////////////
	return true;
}
//ɾ����ʱ���� 
bool CGameListCtrl::DeleteOutTimeRoom(UINT uKindID, UINT uNameID)
{
	CAFCNameItem * pNameItem=FindNameItem(FindKindItem(uKindID),uNameID);
	if (pNameItem!=NULL) pNameItem->DeleteOutTimeItem();
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�

	if(Glb().m_baFavor.GetCount())
	{
		CAFCNameItem * pNameItem=FindNameItem((CAFCGameListItem *)m_InsideArray.GetAt(0),uNameID);
		if (pNameItem!=NULL) pNameItem->DeleteOutTimeItem();
	}
	///////////////////////////////////////////////////////////

	return true;
}

//ɾ������ʾ�ķ���
bool CGameListCtrl::DeleteHideRoom()
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			// duanxiaohui 20111107 add
			for (INT_PTR j = 0; j < pKindItem->m_ItemPtrArray.GetCount(); j++)
			{
				CAFCKindItem * pKindItem2 = (CAFCKindItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (pKindItem2 != NULL)
				{
					// �ж�����Ϸ���ͻ�����Ϸ����
					if (pKindItem2->m_uDataType == GLK_GAME_KIND)
					{
						// ����Ϸ����, �����Ŀ¼Ϊ����Ŀ¼, ��Ŀ¼�»�����Ϸ
						for (INT_PTR k = 0; k < pKindItem2->m_ItemPtrArray.GetCount(); k++)
						{
							CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem2->m_ItemPtrArray.GetAt(k);
							if (pNameItem != NULL) 
							{
								pKindItem2->m_ItemPtrArray.RemoveAt(k--);///////ɾ����������
								DeleteItem(pNameItem->m_hCurrentItem);
								delete pNameItem;
								//pNameItem->DeleteHideRoom();
							}
						}
					}
					else
					{
						CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
						if (pNameItem != NULL) 
						{
							pKindItem->m_ItemPtrArray.RemoveAt(j--);///////ɾ����������
							DeleteItem(pNameItem->m_hCurrentItem);
							delete pNameItem;
							//pNameItem->DeleteHideRoom();
						}
					}
				}
			}
		}			
	}
	return true;
}

//��ȡ��������
bool CGameListCtrl::GetProcessName(CAFCRoomItem * pGameRoomItem, TCHAR * szProcessName, UINT uBufferSize)
{
	if ((pGameRoomItem->m_pParentItem!=NULL)&&(pGameRoomItem->m_pParentItem->m_uDataType==GLK_GAME_NAME))
	{
		CAFCNameItem * pGameName=(CAFCNameItem *)pGameRoomItem->m_pParentItem;
		CopyMemory(szProcessName,pGameName->m_NameInfo.szGameProcess,__min(uBufferSize,sizeof(pGameName->m_NameInfo.szGameProcess)));
		szProcessName[uBufferSize/sizeof(TCHAR)-1]=0;
		return true;
	}
	return false;
}
//��ȡ��Ϸ����
bool CGameListCtrl::GetGameName(CAFCRoomItem * pGameRoomItem, TCHAR * szGameName, UINT uBufferSize)
{
	if ((pGameRoomItem->m_pParentItem!=NULL)&&(pGameRoomItem->m_pParentItem->m_uDataType==GLK_GAME_NAME))
	{
		CAFCNameItem * pGameName=(CAFCNameItem *)pGameRoomItem->m_pParentItem;
		CopyMemory(szGameName,pGameName->m_NameInfo.szGameName,__min(uBufferSize,sizeof(pGameName->m_NameInfo.szGameName)));
		szGameName[uBufferSize/sizeof(TCHAR)-1]=0;
		return true;
	}
	return false;
}



//չ������
bool CGameListCtrl::ExpandItem(int iExpendCount)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	pGameKindItem->ExpandItem(iExpendCount);
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�

	if(Glb().m_baFavor.GetCount())
	{
		CAFCGameListItem * pGameKindItem=(CAFCGameListItem *)m_InsideArray.GetAt(0);//��ȡ����
		pGameKindItem->ExpandItem(iExpendCount);
	}
	///////////////////////////////////////////////////////////

	return true;
}

void CGameListCtrl::UpdataItem(CAFCGameListItem* pItem)
{
	CAFCNameItem * pNameItem;
	int i;
	for(i=0;i<=pItem->m_pParentItem->m_ItemPtrArray.GetCount();i++)
	{
		pNameItem = (CAFCNameItem *)(pItem->m_pParentItem->m_ItemPtrArray.GetAt(i));
		if(pNameItem->m_hCurrentItem==pItem->m_hCurrentItem)
			break;
	}
	int selimage=CPlaceResource::GetListIconIndex(&(pNameItem->m_NameInfo));
	TVITEM item;
	item.hItem=pItem->m_hCurrentItem;
	item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	item.iImage=selimage;
	item.iSelectedImage=selimage;
	TreeView_SetItem(m_hWnd,&item);
}

//��ȡItem�Ĳ��ֵ
int CGameListCtrl::GetItemLevel(HTREEITEM hItem)
{
	int iLevel = 0;

	HTREEITEM hNext = GetParentItem(hItem);

	while (hNext)
	{
		iLevel++;
		hNext = GetParentItem(hNext);
	}

	return iLevel;
}

void CGameListCtrl::UpdateAllIcon(void)
{
	int selimage;
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			// duanxiaohui 20111107 add
			for (INT_PTR j = 0; j < pKindItem->m_ItemPtrArray.GetCount(); j++)
			{
				CAFCKindItem * pKindItem2 = (CAFCKindItem *)pKindItem->m_ItemPtrArray.GetAt(j);

				// �ж�����Ϸ���ͻ�����Ϸ����
				if (pKindItem2->m_uDataType == GLK_GAME_KIND)
				{
					// ����Ϸ����, �����Ŀ¼Ϊ����Ŀ¼, ��Ŀ¼�»�����Ϸ
					for (INT_PTR k = 0; k < pKindItem2->m_ItemPtrArray.GetCount(); k++)
					{
						CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem2->m_ItemPtrArray.GetAt(k);
						if (pNameItem != NULL) 
						{
							selimage = CPlaceResource::GetListIconIndex(&(pNameItem->m_NameInfo));
							TVITEM item;
							item.hItem = pNameItem->m_hCurrentItem;
							item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
							item.iImage = selimage;
							item.iSelectedImage = selimage;
							TreeView_SetItem(m_hWnd, &item);
						}
					}
				}
				else
				{
					CAFCNameItem * pNameItem = (CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
					if (pNameItem != NULL) 
					{
						selimage = CPlaceResource::GetListIconIndex(&(pNameItem->m_NameInfo));
						TVITEM item;
						item.hItem = pNameItem->m_hCurrentItem;
						item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
						item.iImage = selimage;
						item.iSelectedImage = selimage;
						TreeView_SetItem(m_hWnd, &item);
					}
				}
			}

			//for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount();i++)
			//{
			//	CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
			//	if (pNameItem!=NULL)
			//	{
			//		selimage=CPlaceResource::GetListIconIndex(&(pNameItem->m_NameInfo));
			//		TVITEM item;
			//		item.hItem=pNameItem->m_hCurrentItem;
			//		item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			//		item.iImage=selimage;
			//		item.iSelectedImage=selimage;
			//		TreeView_SetItem(m_hWnd,&item);
			//	}
			//}

			// end duanxiaohui
		}
	}
}
void CGameListCtrl::JoinAllItem(void)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{

			CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
			if (pKindItem!=NULL)
			{
				for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount();i++)
				{
					CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
					if (pNameItem!=NULL)
					{
						pNameItem->m_pTreeCtrl->Expand(pNameItem->m_hCurrentItem,TVE_COLLAPSE);
					}
				}
				pKindItem->m_pTreeCtrl->Expand(pKindItem->m_hCurrentItem,TVE_COLLAPSE);
			}
	}
	for (INT_PTR i=0;i< m_GameUrl.GetCount();i++)
	{
		CAFCGameUrlItem *q = (CAFCGameUrlItem *)m_GameUrl.GetAt(i);
		this->Expand(q->m_hCurrentItem, TVE_COLLAPSE);
	}
}
bool CGameListCtrl::ExpandAll(bool ExpName)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount()&&ExpName;i++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
				if (pNameItem!=NULL)
				{
					pNameItem->m_pTreeCtrl->Expand(pNameItem->m_hCurrentItem,TVE_EXPAND);
				}
			}
			pKindItem->m_pTreeCtrl->Expand(pKindItem->m_hCurrentItem,TVE_EXPAND);
		}
	}

	for (INT_PTR i=0;i< m_GameUrl.GetCount();i++)
	{
		CAFCGameUrlItem *q = (CAFCGameUrlItem *)m_GameUrl.GetAt(i);
		this->Expand(q->m_hCurrentItem, TVE_EXPAND);
	}
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�

	if(Glb().m_baFavor.GetCount())
	{
		CAFCGameListItem * pGameKindItem=(CAFCGameListItem *)m_InsideArray.GetAt(0);//��ȡ����
		pGameKindItem->m_pTreeCtrl->Expand(pGameKindItem->m_hCurrentItem,TVE_EXPAND);
	}
	///////////////////////////////////////////////////////////

	return true;
}

bool CGameListCtrl::ExpandItem(CAFCGameListItem *pListItem)
{
	if (!pListItem)
	{
		return false;
	}

	HTREEITEM hItemParent = NULL;
	if (NULL != pListItem->m_hCurrentItem)
	{
		hItemParent = pListItem->m_pTreeCtrl->GetParentItem(pListItem->m_hCurrentItem);
	}
	else if (GLK_GAME_KING_WEBGAME == pListItem->m_uDataType)
	{
		hItemParent = GetKindParentItem()->m_hCurrentItem;
	}
		
	if (NULL != hItemParent)
	{
		//�ҵ�����
		HTREEITEM hItemChild = pListItem->m_pTreeCtrl->GetChildItem(hItemParent);
		while (NULL != hItemChild)
		{
			if (pListItem != (CAFCGameListItem*)(pListItem->m_pTreeCtrl->GetItemData(hItemChild)))
			{
				pListItem->m_pTreeCtrl->Expand(hItemChild, TVE_COLLAPSE);
			}
			hItemChild = pListItem->m_pTreeCtrl->GetNextItem(hItemChild, TVGN_NEXT);
		}
	}	
	return true;
}

BEGIN_MESSAGE_MAP(CFunListCtrl, CTreeCtrl)
END_MESSAGE_MAP()

//���캯��
CFunListCtrl::CFunListCtrl()
{
}

//��������
CFunListCtrl::~CFunListCtrl()
{
	return;
}
void CFunListCtrl::LoadSkin()
{
	//������Ϸ�б�����
	UINT uIconIndex[]=
	{
		IND_LIST_ROOT,IND_ROOM_NO_ICON,IND_LIST_ROOT,IND_LIST_NO_INSTALL,
		IND_LIST_ROOT,IND_LIST_ROOT,IND_LIST_ROOT,
	};
	CString s=CBcfFile::GetAppPath();/////����·��
	int nCount;
	CBcfFile f( s + "bzgame.bcf");
	CBcfFile fs(s + m_skinmgr.GetSkinBcfFileName());
	CString key=TEXT("GameListControl");

	int r,g,b;
	r=fs.GetKeyVal(key,"gamelbr",0);
	g=fs.GetKeyVal(key,"gamelbg",255);
	b=fs.GetKeyVal(key,"gamelbb",0);
	this->SetBkColor(RGB(r,g,b));

	nCount=f.GetKeyVal (Glb().m_key,"TreeCount3",0);
	for (int i=0;i<nCount;i++)
	{	
		CString msg,strIcon;
		UINT uTempIconIndex;

		msg.Format("Tree3Icon%d",i);
		strIcon=f.GetKeyVal(Glb().m_key,msg,"");
		uTempIconIndex=IND_LIST_ROOT;
		if(strIcon!="")
		{
			uTempIconIndex=CPlaceResource::LoadExtendIcon(strIcon);
			if(uTempIconIndex<1)
				uTempIconIndex=IND_LIST_ROOT;
		}
		//��������
		TV_INSERTSTRUCT InsertInf;
		memset(&InsertInf,0,sizeof(InsertInf));
		InsertInf.item.cchTextMax=100;
		InsertInf.hInsertAfter=TVI_LAST;
		InsertInf.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
		InsertInf.item.iImage=uTempIconIndex;
		InsertInf.item.iSelectedImage=uTempIconIndex;//ѡ���ͼ��
		msg.Format("3Tree%d",i);
		CString temp=f.GetKeyVal (Glb().m_key,msg,"");
		//strncpy(InsertInf.item.pszText,strIcon,strIcon.GetLength());
		InsertInf.item.pszText=temp.GetBuffer();
		msg.Format("Tree3URLid%d",i);
		//CString temp2=f.GetKeyVal (Glb().m_key,msg,"");
		m_saUrl.Add(f.GetKeyVal (Glb().m_key,msg,""));
		InsertInf.item.lParam=(LPARAM)i;
		InsertItem(&InsertInf);
	}

	long dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle|TVS_FULLROWSELECT);
}
bool CFunListCtrl::Init()
{
	//���ÿؼ�
	//SendMessage(SET_LINE_COLOR,0,(LPARAM)CGameImageLink::m_ColorResource.m_crListTxColor);

	//������Ϸ�б�����
	UINT uIconIndex[]=
	{
		IND_LIST_ROOT,IND_ROOM_NO_ICON,IND_LIST_ROOT,IND_LIST_NO_INSTALL,
		IND_LIST_ROOT,IND_LIST_ROOT,IND_LIST_ROOT,
	};
	CString s=CBcfFile::GetAppPath();/////����·��
	int nCount;
	CBcfFile f( s + "bzgame.bcf");

	CBcfFile fs(s + m_skinmgr.GetSkinBcfFileName());
	CString key=TEXT("GameListControl");

	int r,g,b;
	r=fs.GetKeyVal(key,"gamelbr",0);
	g=fs.GetKeyVal(key,"gamelbg",255);
	b=fs.GetKeyVal(key,"gamelbb",0);
	this->SetBkColor(RGB(r,g,b));

	nCount=f.GetKeyVal (Glb().m_key,"TreeCount3",0);
	for (int i=0;i<nCount;i++)
	{	
		CString msg,strIcon;
		UINT uTempIconIndex;

		msg.Format("Tree3Icon%d",i);
		strIcon=f.GetKeyVal(Glb().m_key,msg,"");
		uTempIconIndex=IND_LIST_ROOT;
		if(strIcon!="")
		{
			uTempIconIndex=CPlaceResource::LoadExtendIcon(strIcon);
			if(uTempIconIndex<1)
				uTempIconIndex=IND_LIST_ROOT;
		}
		//��������
		TV_INSERTSTRUCT InsertInf;
		memset(&InsertInf,0,sizeof(InsertInf));
		InsertInf.item.cchTextMax=100;
		InsertInf.hInsertAfter=TVI_LAST;
		InsertInf.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
		InsertInf.item.iImage=uTempIconIndex;
		InsertInf.item.iSelectedImage=uTempIconIndex;//ѡ���ͼ��
		msg.Format("3Tree%d",i);
		CString temp=f.GetKeyVal (Glb().m_key,msg,"");
		//strncpy(InsertInf.item.pszText,strIcon,strIcon.GetLength());
		InsertInf.item.pszText=temp.GetBuffer();
		msg.Format("Tree3URLid%d",i);
		//CString temp2=f.GetKeyVal (Glb().m_key,msg,"");
		m_saUrl.Add(f.GetKeyVal (Glb().m_key,msg,""));
		InsertInf.item.lParam=(LPARAM)i;
		InsertItem(&InsertInf);
	}
	return true;
}

//���ؼ���ɫ����
void   CGameListCtrl::SetItemFont(HTREEITEM   hItem,   LOGFONT&   logfont) 
{ 
	Color_Font   cf; 
	if(   !m_mapColorFont.Lookup(   hItem,   cf   )   ) 
		cf.color   =   (COLORREF)-1; 
	cf.logfont   =   logfont; 
	m_mapColorFont[hItem]   =   cf; 
} 

////////////////////////////////////////////////////////////////////// 
void   CGameListCtrl::SetItemBold(HTREEITEM   hItem,   BOOL   bBold) 
{ 
	SetItemState(hItem,   bBold   ?   TVIS_BOLD:   0,   TVIS_BOLD); 
} 

////////////////////////////////////////////////////////////////////// 
void   CGameListCtrl::SetItemColor(HTREEITEM   hItem,   COLORREF   color) 
{ 
	Color_Font   cf; 
	if(!m_mapColorFont.Lookup(hItem,   cf)) 
		cf.logfont.lfFaceName[0]   =   '\0 '; 
	cf.color   =   color; 
	m_mapColorFont[hItem]   =   cf; 
} 

void  CGameListCtrl::SetItemStatusColor(HTREEITEM hItem,UINT roomID,const COLORREF& color,const COLORREF& statusColor,const CString& strStatusText) 
{ 
	Color_Font   cf;
	cf.color = color;
	cf.logfont.lfFaceName[0]   = '\0';
	cf.statusfont.lfFaceName[0] = '\0';
	cf.statuscolor = statusColor;
	wsprintf(cf.statusfont.lfFaceName,"%s",strStatusText);
	
	m_mapHitemRoomID[hItem] = roomID;
	m_mapStatusColor[roomID] = cf;
}
////////////////////////////////////////////////////////////////////// 
BOOL   CGameListCtrl::GetItemFont(HTREEITEM   hItem,   LOGFONT   *   plogfont) 
{ 
	Color_Font   cf; 
	if(!m_mapColorFont.Lookup(hItem,   cf)) 
		return   FALSE; 
	if(cf.logfont.lfFaceName[0]   ==   '\0 ')   
		return   FALSE; 
	*plogfont   =   cf.logfont; 
	return   TRUE; 

} 

////////////////////////////////////////////////////////////////////// 
BOOL   CGameListCtrl::GetItemBold(HTREEITEM   hItem) 
{ 
	return   GetItemState(hItem,   TVIS_BOLD)   &   TVIS_BOLD; 
} 

////////////////////////////////////////////////////////////////////// 
COLORREF   CGameListCtrl::GetItemColor(HTREEITEM   hItem) 
{ 
	//   Returns   (COLORREF)-1   if   color   was   not   set 
	Color_Font   cf; 
	if(!m_mapColorFont.Lookup(hItem,   cf)) 
		return   (COLORREF)   -   1; 
	return   cf.color; 

} 

////////////////////////////////////////////////////////////////////// 
void CGameListCtrl::OnPaint() 
{ 
	CPaintDC dc(this); 
    
    //�ս����������Ϸ�б�������������� 20130829 wzd
    m_iCount++;    
    CString str;
    str.Format("wzd:client CGameListCtrl::OnPaint,m_iCount=%d",m_iCount);
    OutputDebugString(str);    
    if (m_bISInit || (3 == m_iCount))        
    {
        OutputDebugString("wzd:client CGameListCtrl::OnPaint1");
        HTREEITEM hitem=GetRootItem();
        if(hitem!=NULL)
        {
            OutputDebugString("wzd:client CGameListCtrl::OnPaint2");
            EnsureVisible(hitem);
        }
        m_bISInit = false;
        OutputDebugString("wzd:client CGameListCtrl::OnPaint3");
    }
    
	// Create a memory DC compatible with the paint DC 
	CDC memDC; 
	memDC.CreateCompatibleDC(&dc); 

	CRect rcClip, rcClient; 
	dc.GetClipBox( &rcClip ); 
	GetClientRect(&rcClient); 

	// Select a compatible bitmap into the memory DC 
	CBitmap bitmap; 
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() ); 
	memDC.SelectObject( &bitmap ); 

	//dc.SetBkMode(TRANSPARENT);

	// Set clip region to be same as that in paint DC 
	CRgn rgn; 
	rgn.CreateRectRgnIndirect( &rcClip ); 
	memDC.SelectClipRgn(&rgn); 
	rgn.DeleteObject(); 

	// First let the control do its default drawing. 
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0); 

	HTREEITEM hItem = GetFirstVisibleItem(); 

	int iItemCount = GetVisibleCount() + 1; 
	while(hItem && iItemCount--) 
	{ 
		CRect rect; 

		// Do not meddle with selected items or drop highlighted items 
		UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED; 
		Color_Font cf; 


		if ((GetItemState(hItem, selflag) & selflag) 
			&& ::GetFocus() == m_hWnd) 
			; 
		else if (m_mapColorFont.Lookup(hItem, cf)) 
		{ 
			CFont *pFontDC; 
			CFont fontDC; 
			LOGFONT logfont; 

			if(cf.logfont.lfFaceName[0] != '\0 ') 
				logfont = cf.logfont; 
			else { 
				// No font specified, so use window font 
				CFont *pFont = GetFont();
				pFont-> GetLogFont( &logfont ); 
			} 

			if(GetItemBold(hItem)) 
				logfont.lfWeight = 700; 

			fontDC.CreateFontIndirect(&logfont); 
			pFontDC = memDC.SelectObject(&fontDC ); 

			if(cf.color != (COLORREF) - 1) 
				memDC.SetTextColor(cf.color); 
			else 
				memDC.SetTextColor(GetSysColor(COLOR_WINDOWTEXT)); 


			CString sItem = GetItemText(hItem); 

			GetItemRect(hItem, &rect, TRUE); 
			//memDC.SetBkColor( GetSysColor(COLOR_WINDOW )); 
			memDC.SetBkMode(TRANSPARENT);
			memDC.TextOut(rect.left + 2, rect.top + 5, sItem); 

			memDC.SelectObject(pFontDC); 
		} 

		hItem = GetNextVisibleItem(hItem); 
	} 


	dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, 
		rcClip.left, rcClip.top, SRCCOPY); 

	memDC.DeleteDC(); 
} 

// void CGameListCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
// 	// TODO: Add your control notification handler code here
// 
// 	POINT pt = GetCurrentMessage()->pt; 
// 	ScreenToClient(&pt); 
// 
// 	TVHITTESTINFO tvht;
// 	tvht.pt = pt;
// 	HitTest(&tvht);
// 
// 	if (!GetParentItem(tvht.hItem))
// 	{
// 		*pResult = 0;
// 	}
// }

void CGameListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

  /*��ͬ�ؼ���Ϣ��ͬ����TreeView�ؼ�����NM_CUSTOMDRAW֪ͨ��ʱ��lParamָ���
    ��NMTVCUSTOMDRAW�ṹ�壬����ṹ��ĵ�һ����Ա��NMCUSTOMDRAW�ṹ��*/
    NMCUSTOMDRAW* pNMCustomDraw = (NMCUSTOMDRAW*)pNMHDR;
    NMTVCUSTOMDRAW* pNMTVCustomDraw = (NMTVCUSTOMDRAW*)pNMHDR;

	switch(pNMCustomDraw->dwDrawStage)
	{
	case CDDS_PREPAINT: //����ѭ����ʼʱ
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		break;

    case CDDS_ITEMPREPAINT :  //����itemǰ���ɻ��Ʊ���
        {
			//�Լ�����������NM_CUSTOMDRAW,����item����NM_CUSTOMDRAW
            HTREEITEM hItem = (HTREEITEM)pNMCustomDraw->dwItemSpec;
            //CRect rcItem = pNMCustomDraw->rc;

            CRect rtemp;
            GetItemRect(hItem,&rtemp,TRUE);

            if (rtemp.IsRectEmpty())
            {        
				//����Ҫ�ػ�
                *pResult = CDRF_DODEFAULT;
                break;
            }
             *pResult = (CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT);
        }
        break;

	case CDDS_ITEMPOSTPAINT: //����item��
		{
			HTREEITEM hItem = (HTREEITEM)pNMCustomDraw->dwItemSpec;
			TCHAR path[MAX_PATH];
			CDC dc;
			dc.Attach(pNMCustomDraw->hdc);

			CRect rcItem;
			int nImage,nImageSel;
			UINT uItemState;

			GetItemRect(hItem,&rcItem,FALSE);
			GetItemImage(hItem,nImage,nImageSel);
			uItemState = GetItemState(hItem,TVIS_EXPANDED);

			CRect rcDraw = rcItem;

			if (pNMTVCustomDraw->iLevel == 0 || pNMTVCustomDraw->iLevel == 1)
			{
				rcDraw.left = 10;
			}
			else
			{
				rcDraw.left = (pNMTVCustomDraw->iLevel-1) * 16 - 6;
			}
			

			CFont* pFont = NULL;
			pFont = GetFont();
			LOGFONT lf;
			pFont->GetLogFont(&lf);

			if (pNMTVCustomDraw->iLevel == 0)
			{
				lf.lfHeight = 14;
				strcpy(lf.lfFaceName,_TEXT("����"));
				lf.lfWeight = 900;
			}
			else if (pNMTVCustomDraw->iLevel == 1)
			{
				lf.lfHeight = 20;
				strcpy(lf.lfFaceName,_TEXT("����"));
				lf.lfWeight = 900;
			}
			else
			{
				lf.lfHeight = 14;
				lf.lfWeight = 400;
				strcpy(lf.lfFaceName,_TEXT("����"));
			}

			//��ȡ�������
			CAFCNameItem * pGameItem=(CAFCNameItem *)GetItemData(hItem);
			CAFCKindItem * pKindItem=(CAFCKindItem *)GetItemData(hItem);
			//�����Item�Ϸ�ʱ
			if(pNMCustomDraw->uItemState & CDIS_HOT)
			{
				if (pNMTVCustomDraw->iLevel == 0)
				{
					CGameImageHelper bitmap(&m_imgGameKind);
					bitmap.BitBlt(dc.GetSafeHdc(), rcItem,CPoint(bitmap.GetWidth()/2,0));
					dc.SetTextColor(RGB(255,255,255));
				}
				else
				{
					if (pNMTVCustomDraw->iLevel == 1)
					{
						CGameImageHelper bitmap(&m_imgGameKind);
						bitmap.BitBlt(dc.GetSafeHdc(), rcItem,CPoint(bitmap.GetWidth()/2,0));
						dc.SetTextColor(RGB(255,255,255));

						//�����Ϸ������ʾͼƬ
						if (pGameItem && pGameItem->m_uDataType == GLK_GAME_KIND)
						{
								//��Ϸ������ʾͼ��(��̬)
								CString s = CBcfFile::GetAppPath();/////����·��
								wsprintf(path,"%simage\\game\\GameKind%d.png",s,pKindItem->m_KindInfo.uKindID);
								m_imgGameKindMg.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
								CGameImageHelper imgHelper(&m_imgGameKindMg);
								imgHelper.BitBlt(dc.GetSafeHdc(),rcItem.left+40,rcItem.top+10);
						}
					}
					else if (pNMTVCustomDraw->iLevel == 2)
					{
						if (pGameItem && pGameItem->m_uDataType == GLK_GAME_NAME)
						{							
							vector<CMyGameImageEx>::iterator iter = find_if(m_vecGameImage.begin(), m_vecGameImage.end(), GameImageExEQ(((CAFCNameItem*)pGameItem)->m_NameInfo.uNameID));
							if (iter != m_vecGameImage.end())
							{
								CGameImageHelper bitmap(&m_ArrimgGameName[iter->uIndex]);
								bitmap.BitBlt(dc.GetSafeHdc(), rcItem,CPoint(bitmap.GetWidth()/2,0));
								if (iter->pimg_GameEx)
								{
									iter->pimg_GameEx->BitBlt(dc.GetSafeHdc(), rcItem,CPoint(0,0));
								}
							}
						}
						dc.SetTextColor(RGB(0,0,0));
					}
					else
					{
						CGameImageHelper bitmap(&m_imgGameRoom);
						bitmap.BitBlt(dc.GetSafeHdc(), rcItem,CPoint(bitmap.GetWidth()/2,0));
						dc.SetTextColor(RGB(0,0,0));
					}
				}
			}
			else
			{
				if (pNMTVCustomDraw->iLevel == 0 || pNMTVCustomDraw->iLevel == 1)
				{
					m_imgGameKind.BitBlt(dc.GetSafeHdc(), rcItem,CPoint(0,0));
					dc.SetTextColor(RGB(0,0,0));
				}
				else if (pNMTVCustomDraw->iLevel == 2)
				{
					if (pGameItem && pGameItem->m_uDataType == GLK_GAME_NAME)
					{
						vector<CMyGameImageEx>::iterator iter = find_if(m_vecGameImage.begin(), m_vecGameImage.end(), GameImageExEQ(((CAFCNameItem*)pGameItem)->m_NameInfo.uNameID));
						if (iter != m_vecGameImage.end())
						{
							m_ArrimgGameName[iter->uIndex].BitBlt(dc.GetSafeHdc(), rcItem,CPoint(0,0));
							if (iter->pimg_GameEx)
							{
								iter->pimg_GameEx->BitBlt(dc.GetSafeHdc(), rcItem,CPoint(0,0));
							}
						}
					}					
					dc.SetTextColor(RGB(0,0,0));
				}
				else
				{
					m_imgGameRoom.BitBlt(dc.GetSafeHdc(), rcItem,CPoint(0,0));
					dc.SetTextColor(RGB(0,0,0));
				}
			}

			////��+-��ť
			//if (ItemHasChildren(hItem) && pNMTVCustomDraw->iLevel > 0)
			//{
			//	CGameImageEx* pImage = NULL;
			//	if (uItemState & TVIS_EXPANDED)
			//	{
			//		pImage = &m_imgFold;
			//	}
			//	else
			//	{
			//		pImage = &m_imgUnFold;
			//	}

			//	CGameImageHelper imgHelper(pImage);
			//	imgHelper.BitBlt(dc.GetSafeHdc(),rcDraw.left,rcDraw.top + 6);
			//}

			//��Ϸ�ȶ�	ADD BY DWJ
			CGameImageEx* pImage = NULL;
			if (pGameItem != NULL && pGameItem->m_uDataType == GLK_GAME_NAME)
			{
				//���б�ָ��»���
				if(pGameItem->m_iGameHotDegree <=5 && pGameItem->m_iGameHotDegree >0)
				{
					//�����ȶ�����
					for(int i=0; i<pGameItem->m_iGameHotDegree;i++)
					{
						pImage = &m_imgHotDegree;
						CGameImageHelper imgHelper(pImage);
						imgHelper.BitBlt(dc.GetSafeHdc(),rcItem.right-20-(i%3*20),rcItem.top + i/3 * m_imgHotDegree.GetHeight() + 3);
					}
				}

				//������
				if(1 == pGameItem->m_iGameHotType)
				{
					pImage = &m_imgNewGame;
					CGameImageHelper imgHelper(pImage);
					imgHelper.BitBlt(dc.GetSafeHdc(),rcItem.left,rcItem.top+2);
				}
				else if(2 == pGameItem->m_iGameHotType)	//������
				{
					pImage = &m_imgHotGame;
					CGameImageHelper imgHelper(pImage);
					imgHelper.BitBlt(dc.GetSafeHdc(),rcItem.left,rcItem.top+2);
				}
			}

			//������
			rcDraw.left += 15;
			CString strText = GetItemText(hItem);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);

			CFont newFont;
			newFont.CreateFontIndirect(&lf);
			CFont* pOldFont = dc.SelectObject(&newFont);

			if (pNMTVCustomDraw->iLevel == 0)
			{
				CRect rcDraw2 = rcDraw;
				rcDraw2.left = rcDraw.left - 20;
				CRect rcDrawRoot(rcDraw2);
				rcDrawRoot.top = 0;

				if (strText.Compare("��Ϸ����") >= 0)
				{
					if(1 == CGamePlaceApp::GetModuleStatus(_T("GameList"),_T("IsShowOnLineCount"),0))
					{
						CString strOnLineUser;
						strOnLineUser.Format("[%d��]",GetMainRoom()->GetonlineUserCount());
						strText.Append(strOnLineUser);
						dc.DrawText(strText, &rcDrawRoot,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
					}
					else
					{
						dc.DrawText(strText, &rcDrawRoot,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
					}
				}
			}
			else 	if (pNMTVCustomDraw->iLevel == 1)
			{
				CRect rcDraw2 = rcDraw;
				rcDraw2.left = rcDraw.left + 50;
				dc.DrawText(strText, &rcDraw2,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
				std::map<void*,UINT>::iterator mapRoomIDIter =  m_mapHitemRoomID.find(hItem);
				if (mapRoomIDIter != m_mapHitemRoomID.end())
				{
					dc.SetTextColor(m_mapStatusColor[mapRoomIDIter->second].statuscolor);
					dc.DrawText(m_mapStatusColor[mapRoomIDIter->second].statusfont.lfFaceName, &rcDraw2,DT_VCENTER|DT_SINGLELINE|DT_RIGHT|DT_END_ELLIPSIS);
				}
			}
			else if (pNMTVCustomDraw->iLevel == 3)
			{
				CRect rcDraw2 = rcDraw;
				CRect rcDraw3 = rcDraw;
				rcDraw2.left = rcDraw.left - 20;
				rcDraw2.top = rcDraw.top + 5;
				dc.DrawText(strText, &rcDraw2,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
				std::map<void*,UINT>::iterator mapRoomIDIter =  m_mapHitemRoomID.find(hItem);
				if (mapRoomIDIter != m_mapHitemRoomID.end())
				{
					rcDraw3.right = rcDraw.right - 25;
					dc.SetTextColor(m_mapStatusColor[mapRoomIDIter->second].statuscolor);
					dc.DrawText(m_mapStatusColor[mapRoomIDIter->second].statusfont.lfFaceName, &rcDraw3,DT_VCENTER|DT_SINGLELINE|DT_RIGHT|DT_END_ELLIPSIS);
				}
			}

			dc.SelectObject(pOldFont);
			dc.SetBkMode(nOldBkMode);
			
			dc.Detach();

			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		break;

	default:
		{
			*pResult = CDRF_DODEFAULT;      
		}
		break;
	}
}

BOOL CGameListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	// JianGuankun 2011.11.19
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	POINT pt = GetCurrentMessage()->pt; 
	ScreenToClient(&pt);

	TVHITTESTINFO tvht;
	tvht.pt = pt;
	HTREEITEM iItem = HitTest(&tvht); 

	if(GetItemLevel(tvht.hItem) == 0) //�б���ⲻ�ܵ��
	{
		*pResult = 0;
		return FALSE;
	}
	
	if(GetItemLevel(tvht.hItem) == 2) //��Ϸ���ֵ������ʾ��Ϸ����
	{
		if (iItem!=NULL)
		{
			Select(iItem,TVGN_CARET);
			CAFCGameListItem * pItem=(CAFCGameListItem *)GetItemData(iItem);
			if ((pItem!=NULL)&&(pItem->m_uDataType==GLK_GAME_NAME))
			{
				//����Ϸ���ֽڵ���ͬ����ʾ��Ϸ����
				CAFCNameItem * pNameItem=(CAFCNameItem *)pItem;
				CString url;
				static UINT uNameID = 0;

				if(uNameID != pNameItem->m_NameInfo.uNameID)
				{
					url.Format("%sGameInfo3.aspx?id=%d",Glb().m_CenterServerPara.m_strHomeADDR,pNameItem->m_NameInfo.uNameID);
					GetMainRoom()->IE(url);
					uNameID = pNameItem->m_NameInfo.uNameID;
				}
			}
		}
	}

	if (!(TVIS_EXPANDED & GetItemState(tvht.hItem, TVIS_EXPANDED)))
	{
		CAFCGameListItem * pItem=(CAFCGameListItem *)GetItemData(tvht.hItem);
		if (NULL != pItem)
		{//ͬ�������������𣬱����
			ExpandItem(pItem);
		}
	}

	if(!(tvht.flags & (TVHT_NOWHERE | TVHT_ONITEMBUTTON))) 
	{
// 		CString str;
// 		str.Format("UIClient:OnNMClick TVHT_ABOVE Text = %s tvht.flags = 0x%08x",GetItemText(tvht.hItem),tvht.flags);
// 		OutputDebugString(str);
		if (tvht.hItem)
		{
			Expand(tvht.hItem,TVE_TOGGLE);
		}
	}

	*pResult = 0;

	return FALSE;
}

BOOL CGameListCtrl::OnNMDlbClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	// JianGuankun 2011.11.19

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	POINT pt = GetCurrentMessage()->pt; 
	ScreenToClient(&pt);

	TVHITTESTINFO tvht;
	tvht.pt = pt;
	HitTest(&tvht); 

	if(GetItemLevel(tvht.hItem) == 0) //�б���ⲻ�ܵ��
	{
		*pResult = 1;
		return TRUE;    // modified by LWF 2013.6.13 �������Ϸ�����������¼������Ϸ���м���Ե�����ͻ���exe�����������ť�ᵼ������б���ʧ���ˡ���
	}

	*pResult = 0;

	return FALSE;
}

void CGameListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	//ScreenToClient(&pt); 

	TVHITTESTINFO tvht;
	tvht.pt = pt;
	HitTest(&tvht); 

	if (!(tvht.flags & TVHT_NOWHERE) && GetParentItem(tvht.hItem))
	{
		::SetCursor(m_curHand);
	}
	else
	{
		::SetCursor(m_curArrow);
		return;
	}

	if (tvht.hItem != m_hLastMMItem && m_hLastMMItem)
	{
		//if(!(tvht.flags & TVHT_ONITEM)) 
		{
			NMTVCUSTOMDRAW* ptvDraw = new NMTVCUSTOMDRAW();
			ptvDraw->nmcd.hdc = GetDC()->GetSafeHdc();
			ptvDraw->nmcd.dwDrawStage = CDDS_ITEMPOSTPAINT;
			ptvDraw->nmcd.uItemState |= CDIS_HOT;
			ptvDraw->nmcd.dwItemSpec = (DWORD_PTR)tvht.hItem;
			ptvDraw->iLevel = GetItemLevel(tvht.hItem);

			LRESULT lResult = 0;

			OnNMCustomdraw((NMHDR*)ptvDraw,&lResult);

			ptvDraw->nmcd.uItemState &= (~CDIS_HOT);
			ptvDraw->nmcd.dwItemSpec = (DWORD_PTR)m_hLastMMItem;
			ptvDraw->iLevel = GetItemLevel(m_hLastMMItem);

			lResult = 0;
			OnNMCustomdraw((NMHDR*)ptvDraw,&lResult);
		
			//::SendMessage(GetParent()->m_hWnd,NM_CUSTOMDRAW,IDC_GAME_LIST,(LPARAM)ptvDraw);

			//SendMessage(NM_CUSTOMDRAW,IDC_GAME_LIST,(LPARAM)ptvDraw);

			delete ptvDraw;

// 			CString str;
// 			str.Format("UIClient:OnNMClick OnMouseMove Text = %s tvht.flags = 0x%08x",GetItemText(tvht.hItem),tvht.flags);
// 			OutputDebugString(str);
		}
	}

	m_hLastMMItem = tvht.hItem;

	__super::OnMouseMove(nFlags, point);
}

void CGameListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//lpMeasureItemStruct->itemHeight = 50;
}

void CGameListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	__super::OnMeasureItem(nIDCtl,lpMeasureItemStruct);
}

LRESULT CGameListCtrl::OnLayout(WPARAM wParam,LPARAM lParam)
{
	//LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam;
	RECT *prc = hdl.prc;
	WINDOWPOS *pwpos = hdl.pwpos;
	int nHeight = (int)(/*pwpos->cy **/ 50); 
	pwpos->cy = nHeight;
	prc->top = nHeight;
	return 0;
}

void CGameListCtrl::OnClickReadRule()
{
	CPoint Point;
	GetCursorPos(&Point);
	ScreenToClient(&Point);
	HTREEITEM hItemHit = HitTest(Point);
	HTREEITEM hItemSel = GetSelectedItem();

	CAFCNameItem* pNameItem = (CAFCNameItem*)GetItemData(hItemHit);
	if (pNameItem != NULL)
	{
		CString strURL;
		if (pNameItem->m_NameInfo.uNameID!=0)
		{
			strURL.Format(TEXT("/app/showgame.%s?id=%d"),Glb().m_urlpostfix,pNameItem->m_NameInfo.uNameID);
			GetMainRoom()->IE(Glb().m_CenterServerPara.m_strWebRootADDR + strURL);
		}
	}
}

// void CGameListCtrl::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
// 	// TODO: Add your control notification handler code here
// 
// 	OutputDebugString("UIClient:OnTvnItemexpanded");
// 
// 	*pResult = 0;
// }
// 


