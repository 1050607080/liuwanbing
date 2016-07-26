#pragma once

enum E_DOWNLOAD_STATE
{
	E_OK,	//�������
	E_ERROR,//���ش���
	E_WAIT,	//�ȴ�����
	E_ING	//��������	
};
struct S_DowndloadItme
{	
	char	szGameName[61];	//��Ϸ����	
	char szUrl[MAX_PATH];	//���ص�ַ
	char	szErr[50];		//������Ϣ
	float	fSize;			//�ļ��ܴ�С
	float	fDownSize;		//�����ش�С	
	float	fSpeed;		//�����ٶ�
	UINT	uNameID;		//��Ϸ���� ID ����
	UINT	uiProgress;		//���ؽ���
	E_DOWNLOAD_STATE e_state;
	S_DowndloadItme()
	{ 
		ZeroMemory(this,sizeof(S_DowndloadItme));
	}
};

typedef map<UINT, BzDui::CListContainerElementUI*> map_GameItem;

class CDownloadListUI: public BzDui::CListUI
{
public:
	CDownloadListUI(void);
	~CDownloadListUI(void);

	bool AddGame(ComNameInfo* pNameInfo, const char* lpstrUrl);
		
	void UpdateData(BzDui::CListContainerElementUI *pElement);

	bool UpdateGame(S_DowndloadItme* pDownloadItem);

	S_DowndloadItme* GetItem(UINT uNameID);

private:

	map_GameItem m_mapGame;
};

class CProgressExUI : public BzDui::CProgressUI
{
public:
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

class CDialogBuilderCallbackEx_DownLoad : public BzDui::IDialogBuilderCallback
{
public:
	BzDui::CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if(_tcscmp(pstrClass, _T("DownloadList")) == 0)
		{			
			return new CDownloadListUI;
		}
		else if(_tcscmp(pstrClass, _T("ProgressEx")) == 0)
		{
			return new CProgressExUI;
		}
		return NULL;
	}
};
