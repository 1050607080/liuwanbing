#pragma once
#include "GameListCtrl.h"
#include "GameElementUI.h"
#include "UIMarqueeText.h"

class CListBodyExUI : public BzDui::CListBodyUI
{
public:
	CListBodyExUI(BzDui::CListUI* pOwner) : BzDui::CListBodyUI(pOwner)
	{

	}
	void SetScrollPos(SIZE szPos);
	void SetPos(RECT rc);
	void DoEvent(BzDui::TEventUI& event);
	virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
};

class CScrollBarExUI : public BzDui::CScrollBarUI
{
public:

	void DoEvent(BzDui::TEventUI& event);
};

class CGameListUI : public BzDui::CListUI
{
public:
	CGameListUI(BzDui::CPaintManagerUI &pm);
	~CGameListUI(void);

	bool Add(BzDui::CControlUI* pControl);

	bool AddAt(BzDui::CControlUI* pControl, int iIndex);

	bool Remove(BzDui::CControlUI* pControl);

	bool RemoveAt(int iIndex);

	void RemoveAll();

	void DoEvent(BzDui::TEventUI& event);

	void ShowNameItems(int iShowIndex);

	Node* GetRoot();

	Node* AddGameNode(ComNameInfo* pNameInfo, int iKindIndex);
	Node* AddGameNode(Node* pNode);
	Node* AddRoomNode(ComRoomInfo* pRoomInfo, Node* parent=NULL);	

	bool RemoveNode(Node* node);

	void SetChildVisible(Node* node, bool visible);

	bool CanExpand(Node* node) const;

	void EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal);
/*
	//����������
	void SortAllGroups();

	//��ĳ����������
	void SortGroup(int iGID);

	//��ĳһ��������
	void RenameItem(CListContainerElementUI* pListEle, LPCTSTR lpNewName);

	//������������������͸���������
	void CalFriends(Node* node);

	//���º��ѷ�����Ϣ
	void UpdateFriendRoomStatues(IM_GP_ENTER_LEAVE_ROOM *pEnterLeaveRoom, Node* pNode);*/

private:
	Node*	root_node_;
	BzDui::CRect	text_padding_;
	BzDui::CPaintManagerUI& m_PaintManager;
	map<UINT, Node*> m_mapNameNode;
	UINT	m_uKindID;
};

class CDialogBuilderCallbackEx : public BzDui::IDialogBuilderCallback
{
public:
	CDialogBuilderCallbackEx(BzDui::CPaintManagerUI& pManager):m_PaintManager(pManager){}
	BzDui::CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if(_tcscmp(pstrClass, _T("GameList")) == 0)
		{
			CGameListUI *pControl = new CGameListUI(m_PaintManager);
			return pControl;
		}
		else if (_tcscmp(pstrClass, _T("GameElement")) == 0)
		{
			CGameElementUI *pControl = new CGameElementUI;
			return pControl;
		}
		else if (_tcscmp(pstrClass, _T("RoomElement")) == 0)
		{
			CRoomElementUI *pControl = new CRoomElementUI;
			return pControl;
		}
		else if(_tcscmp(pstrClass,_T("MarqueeText")) == 0)
		{
			BzDui::CMarqueeTextUI *pControl = new BzDui::CMarqueeTextUI;
			return pControl;
		}
		return NULL;
	}
private:
	BzDui::CPaintManagerUI& m_PaintManager;
};

