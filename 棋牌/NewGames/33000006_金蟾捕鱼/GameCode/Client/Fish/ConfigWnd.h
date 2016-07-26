#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"d3d9.lib")
#include "BassMusicManager.h"
class CConfigWnd
{
public:
	CConfigWnd(void);
	~CConfigWnd(void);
	//λ��X����
	int m_ptx;
	//λ��Y����
	int m_pty;
	//�Ƿ���ʾ
	bool m_isvisable;
	//
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	//��ʼ��
	void InitDlg(LPDIRECT3DDEVICE9 g_pd3dDevice,int x,int y);
	//��ʼ����
	void InitRender(LPD3DXSPRITE  m_pSprite);
	//
	//���뱳��
	LPDIRECT3DTEXTURE9      m_LoadBgTexture;
	//��Ť
	LPDIRECT3DTEXTURE9      m_btText[2];
	//����������
	LPDIRECT3DTEXTURE9      g_sliderbgTex ;  
	//����������
	LPDIRECT3DTEXTURE9      g_sliderperTex ;  
	//��������Ť
	LPDIRECT3DTEXTURE9      g_sliderbuttonTex ;  
	void  LMouseDown(int ptx,int pty);
	void  LMouseUp(int ptx,int pty);
	void  LMouseMove(int ptx,int pty);
	float   m_value1;
	float   m_value2;
	int     m_ptx1;
	int     m_ptx2;
	int   m_oldptx;
	bool  m_s1;
	bool  m_s2;

	//��Ϸ��Ч����
	bool	m_bBgSound;
	bool	m_bGameSound;
	bool	m_bShakeWindow;
	bool	m_bTipMsg;			//�Ƿ������ʾ
};
