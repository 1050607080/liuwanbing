#include "Stdafx.h"
#include <afxdllx.h>
#include "Resource.h"
//#include "ClientGameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE GameClientDLL = { NULL, NULL };
//DWORD      g_bucFileHandle;  //buc�ļ����
//CGameImage g_gameImage;      //��ϷͼƬ�ļ�

//DLL ��ں���
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameClientDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameClientDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameClientDLL);
	}
	return 1;
}

/********************************************************************************************/

//�ȼ��жϺ����������Ϸ��
extern "C" __declspec(dllexport) TCHAR * GetGameOrderName(__int64 dwPoint)
{
	static __int64 uBasePoint[13]=
	{
		1,10001,100001,
		500001,1000001,
		2000001,5000001,
		10000001,20000001,
		50000001,100000001,
		500000001,1000000001

	};
	static TCHAR * szOrderName[13]=
	{
		TEXT("����"),TEXT("�ִ�"),TEXT("����"),TEXT("���"),
		TEXT("���Դ�"),TEXT("��ɣ"),TEXT("����"),TEXT("����"),
		TEXT("�µ�"),TEXT("����"),TEXT("����"),TEXT("����"),
		TEXT("��˹��˹")
	};
	for (int i=0;i<13;i++)	if (dwPoint<uBasePoint[i]) return szOrderName[i];
	return TEXT("��˹��˹");

	/*static __int64 uBasePoint[20]=
	{
		0,100,200,400,
		800,1600,3200,6400,
		12800,25600,51200,102400,
		204800,409600,819200,1638400,
		3276800,6553600,13107200,26214400
	};
	static TCHAR * szOrderName[20]=
	{
		TEXT("�׶�԰"),TEXT("ѧǰ��"),TEXT("Сѧ��"),TEXT("������"),
		TEXT("������"),TEXT("��ר��"),TEXT("��ר��"),	TEXT("��ѧ��"),
		TEXT("�о���"),TEXT("˶ʿ��"),TEXT("��ʿ��"),TEXT("��ʿ��"),
		TEXT("�����ʦ"),TEXT("�������"),	TEXT("���У��"),TEXT("���Ժ��"),
		TEXT("�����ʦ"),TEXT("��ɽ��ʦ"),TEXT("���ʴ�ʦ"),TEXT("�ռ���ʦ")
	};

	for (int i=0;i<20;i++)	
		if (dwPoint<uBasePoint[i]) 
			return szOrderName[i];
	return TEXT("����");*/

}

/********************************************************************************************/

//������Ϸ�ͻ���
//�ͻ�����������
GETCLIENTINFO;
GETGAMEICON(NAME_ID);
//GETFRAMEINTERFACE(CClientGameDlg)
GETORDERFUNCTION(GetGameOrderName);

/********************************************************************************************/
