// Auto_AI.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Auto_AI.h"
#include "Auto_AIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LONG WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	static bool showex=false;
	if(!showex)
		showex=true;
	else
		return EXCEPTION_EXECUTE_HANDLER;

	TCHAR modulename[MAX_PATH],szError[1000],filename[MAX_PATH],*p;
	MEMORY_BASIC_INFORMATION mbinfo;
	VirtualQuery(pExceptionInfo->ExceptionRecord->ExceptionAddress,&mbinfo,sizeof(mbinfo));
	GetModuleFileName((HMODULE)mbinfo.AllocationBase,filename,MAX_PATH);
	p = strrchr(filename,'\\');
	p++;
	lstrcpy(modulename, p);
	try
	{
		SYSTEMTIME SysTime;
		GetLocalTime(&SysTime);
		char FileName[MAX_PATH];
		::CreateDirectory("Error", NULL);
		wsprintf(FileName, "Error\\errorlog-%d-%d.txt", SysTime.wYear, SysTime.wMonth);
		CFile SysLog;
		SysLog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite);
		SysLog.SeekToEnd();

		char szDate[40];
		wsprintf(szDate,"%d-%d-%d %d:%d:%d\r\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, 
			SysTime.wHour, SysTime.wMinute, SysTime.wSecond);

		SysLog.Write(szDate,strlen(szDate));
		wsprintf(szError,"�쳣: ģ��:%s, ģ���ַ:0x%x, �쳣����:0x%x, ָ���ַ:0x%x",
			modulename,mbinfo.AllocationBase,pExceptionInfo->ExceptionRecord->ExceptionCode,pExceptionInfo->ExceptionRecord->ExceptionAddress);
		SysLog.Write(szError,strlen(szError));
		char nextLine[] = "\r\n";
		SysLog.Write(nextLine,strlen(nextLine));
		SysLog.Close();
	}
	catch(CException *e)
	{
		e->Delete();
	}
	wsprintf(szError,"����������������Ҫ�ر�,�Դ˸�������Ĳ����ʾ��Ǹ.\nģ��:%s,ģ���ַ:0x%x,�쳣����:0x%x,ָ���ַ:0x%x",
		modulename, mbinfo.AllocationBase, pExceptionInfo->ExceptionRecord->ExceptionCode, pExceptionInfo->ExceptionRecord->ExceptionAddress);
	MessageBox(NULL,szError,"BZWϵͳ",MB_ICONERROR|MB_OK);
	return EXCEPTION_EXECUTE_HANDLER;
}
// CAuto_AIApp

BEGIN_MESSAGE_MAP(CAuto_AIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAuto_AIApp ����

CAuto_AIApp::CAuto_AIApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

}


// Ψһ��һ�� CAuto_AIApp ����

CAuto_AIApp theApp;


// CAuto_AIApp ��ʼ��

BOOL CAuto_AIApp::InitInstance()
{

	SetUnhandledExceptionFilter(ExceptionFilter);
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CAuto_AIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
