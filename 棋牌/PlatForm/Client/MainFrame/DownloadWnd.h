#pragma once
#include "../utility/http_request_manager.h"
#include "DownloadListUI.h"

class CDownloadWnd :
	public BzDui::CWindowWnd,
	public BzDui::INotifyUI,
	public BzDui::IMessageFilterUI,
	public FCHttpRequestManager
{
public:
	explicit CDownloadWnd();

	~CDownloadWnd();

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void Notify(BzDui::TNotifyUI& msg);

public:

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

public:

	//�����������
	bool AddDownloadEx(ComNameInfo* pNameInfo);

	void DownloadNext();

	CString GetFormatKB (long long nByte)
	{
		TCHAR strRes[128] ;
		StrFormatKBSize(nByte, strRes, sizeof(strRes)) ;
		return strRes;
	}

protected:

	//������������ɺ���ô˺���
	virtual void OnAfterRequestSend(FCHttpRequest& rTask);

	//�ļ�������ɺ���ô˺���
	virtual void OnAfterRequestFinish(FCHttpRequest& rTask);

	//����������
	CString GetStatusText(FCHttpRequest& rTask);

	S_DowndloadItme* GetDownloadItemByTask(int iTaskID);

	void GetDownloadProcess(int iTaskID);

	static unsigned int __stdcall Thread_Install(LPVOID lparam);

protected:

	BzDui::CPaintManagerUI m_pm;

private:

	std::deque<ComNameInfo*>   m_wait_download;

	map<int, ComNameInfo*> m_mapNameTask;				

	bool m_bDownloading;								//��ʶ�����Ƿ���������

	int m_iCurTaskID;									//��ǰ�������ص�����ID

	std::string m_str_recv_data;						//�������ص�����
	
	CDownloadListUI* m_pList;
	
};