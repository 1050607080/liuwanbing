
// LKFishTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLKFishToolApp:
// �йش����ʵ�֣������ LKFishTool.cpp
//

class CLKFishToolApp : public CWinApp
{
public:
	CLKFishToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLKFishToolApp theApp;