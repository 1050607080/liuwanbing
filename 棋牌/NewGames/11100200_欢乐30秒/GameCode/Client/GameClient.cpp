// GameClient.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>
#include "Resource.h"
#include "ClientGameDlg.h"

#ifdef _MANAGED
#error ���Ķ� GameClient.cpp �е�˵����ʹ�� /clr ���б���
// ���Ҫ��������Ŀ����� /clr������ִ�����в���:
//	1. �Ƴ������� afxdllx.h �İ���
//	2. ��û��ʹ�� /clr ���ѽ���Ԥ����ͷ��
//	   ��Ŀ���һ�� .cpp �ļ������к��������ı�:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static AFX_EXTENSION_MODULE GameClientDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("GameClient.DLL ���ڳ�ʼ��!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(GameClientDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL (�� ActiveX �ؼ�)��ʽ���ӵ���
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(GameClientDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("GameClient.DLL ������ֹ!\n");

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(GameClientDLL);
	}
	return 1;   // ȷ��
}

//�ȼ��жϺ���
extern "C" __declspec(dllexport) TCHAR * GetGameOrderName(__int64 dwPoint)
{
	static __int64 uBasePoint[19]=
	{
		50,99,199,299,
		399,499,699,899,
		1099,1499,1899,2299,
		2699,3699,4699,5699,
		6699,8799,14999
	};
	static TCHAR * szOrderName[19]=
	{
		TEXT("��Ӷ��"),TEXT("�б�"),TEXT("�ϵȱ�"),TEXT("��ʿ"),
		TEXT("��ʿ"),TEXT("��ʿ"),	TEXT("��ξ"),	TEXT("��ξ"),
		TEXT("��ξ"),TEXT("��У"),TEXT("��У"),TEXT("��У"),
		TEXT("��У"),TEXT("�ٽ�"),	TEXT("�н�"),TEXT("�Ͻ�"),
		TEXT("��"),	TEXT("˾��"),TEXT("Ԫ˧")
	};
	for (int i=0;i<19;i++)	
		if (dwPoint<uBasePoint[i]) 
			return szOrderName[i];
	return TEXT("��ϯ");
}

//������Ϸ�ͻ���
//�ͻ�����������
GETCLIENTINFO;
GETGAMEICON(NAME_ID);
GETFRAMEINTERFACE(CClientGameDlg);
GETORDERFUNCTION(GetGameOrderName);

#ifdef _MANAGED
#pragma managed(pop)
#endif

