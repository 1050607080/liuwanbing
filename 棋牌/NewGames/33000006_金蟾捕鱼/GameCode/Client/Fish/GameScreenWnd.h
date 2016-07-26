/*
����Ļ������,����ฺ������ͼƬ����ʾ
*/
#pragma once
#include "afxwin.h"
#include "gamestruct.h"
#include "../resource.h"
//#include "..\resource.h"
#include "configwnd.h"
#include "BassMusicManager.h"
#include "factory.h"
#include <deque>



//����DX�ļ�

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"d3d9.lib")
#include <vector>

using namespace std;
class CNewGameClient;


struct BroadCastData
{
	bool				bHave;	//�Ƿ��й㲥
	BYTE				byStation;					//�����λ
	char				szNickName[61];				///<�ǳ�  
	char				szRoomName[61];				///<�������� 
	char				szFishName[30];				///<��������  
	int					iDeskNo;					///<����ID��
	__int64				i64FishScore;				//��Ⱥ�÷�
};


//�������
class GameScreenWnd :public CWnd
{
public:
	//��������
	GameScreenWnd();
	//��������
	~GameScreenWnd(void);

public:
	//���ƽָ̨��
	void SetGameClientP(CNewGameClient *p);

public:
	//���ô���
	CConfigWnd              m_configDlg;
	CNewGameClient			*m_pGameClient;
private:

	//��Ա����
	//����ζ�ͼ����
	static const  int              m_FishmoveCount[24];
	///�������ͼ����
	static const  int              m_FishdeadCount[24];
	//��ͼ��С
	static const  int              m_FishmoveRec[24][2];
	//��ͼ��С
    static const  int              m_FishDeadRec[24][2];

	//������ɽ������
	static const  int              m_iFishMoneyCount[24];
	//���������
	POINT                   m_UserPT[PLAY_COUNT]; 
	//�ڹ�λ��
	POINT                   m_UserPaoPT[PLAY_COUNT]; 
	//�Լ���λ���к�
	int                     m_MeChariID;
	//�Լ���λ��ʵ��
	int                     m_MeRellayChairID;
	//��Ԫ��
	int                     m_CellScore;
	//�������
	bool                    m_HaveUser[PLAY_COUNT];
	//�ڹܽǶ�
	float                   m_UserPaoJiaodu[PLAY_COUNT];
	//����״̬
	bool                    m_UserSendstate[PLAY_COUNT];
	//�Լ��ӵ�
	Bullet                  m_MeBullet[200];
	//�����ӵ�
    Bullet                  m_OterBullet[100];
	//�Ϸ�����
	__int64                 m_UserScore[PLAY_COUNT];
	//�������
	Fish                    m_FishArr[250];
	//Ʈ��������
	Gold                    m_GoldArr[100];
	//��������
	FishTrace               m_FishTrace[250][5];
	//��ֵ��䶯������
	MuchMoneyaction         m_MuchmoneyAct[PLAY_COUNT];
	//
	Numstruct               m_NumArr[100];
	//����Ƿ��ƶ�����Ť��
	bool                    m_IsMouseOver[5];
	//��Ť����
	CRect                   m_BtRect[5];
	//��ұ���
	int                     m_BeiLv[PLAY_COUNT];
	//���Ǯ��
	MoneyDui                m_UserStruct[PLAY_COUNT][3];
	//�Ƿ�ס
	bool                    m_IsDingzhu;
	//�Ƿ񳬼���
	bool                    m_UserSuperPao[PLAY_COUNT];
	//�Ѿ���������
	int                     m_HaveLoadNum;
	//�Ƿ������Ϸ
	bool                    m_InitScreen;
	
	//�Ƿ����л�����״̬
	bool                    m_IsChangeScreen;
	//
	Soundmstruct            m_SoundArr[100]; 
	//�˳����� 
	WaterWavestruct         m_ChanwaveSt;
	//��������,����������������������Ը�����������������ӵ���ɫ�л���
	int                     m_ActionIndex;
	//
	int                     m_ActionIndex1;
	//����ʱ���������÷����ӵ����ʱ��
	int                     m_NowTime;
	//�Ƿ���갴��
	bool                    m_MouseDown;
	//�û���
	CString                 m_sUserName;
	//������Ϣ
	__int64                 m_i64MyMoney;
	//
	int                     m_SoundIndex;
	//
	CFactoryTemplate<Fish>	m_FishFactory;							//��Ⱥ����
	
	//�Ƿ���ʾʱ��
	bool                    m_IsMustOut;
	//��ǰ����ʣ���˳�ʱ��
	int                     m_OutTime;
	//
	CursorStruct            m_cursor;
	//
	int                     m_CursorInt;
	//
	Dingaction              m_DingStruc;

	deque	<BroadCastData> m_dqBroadCast;

	bool		m_bHitFlag;					//�Ƿ���ڻ����˻���������ж�
	BYTE		m_bHitDesk;						//�����˻���������жϵ�λ�ú�

private:
	//��������
	int		m_iShakex;
	int		m_iShakey;
	int		m_iShakeCount;

	int		m_iMoveY;
	int		m_iColorR;
	int		m_iColorB;
	int		m_iColorG;

	

private:
	//���ھ��
	HWND					m_hWnd;
	//���ƽ����߳̾��
	HANDLE 					m_hDxThread;	
	//�ж������߳̾��
	HANDLE 					m_hWorkThread;	
	//
	HANDLE 					m_hWorkThread1;	
	//DX�豸
    LPDIRECT3D9             m_pD3D;
	//��
	ID3DXFont*              m_Dxfont;
	//�㲥��
	ID3DXFont*              m_DxCastFont;
	//
	LPDIRECT3DDEVICE9       m_pd3dDevice;
	//�������ƾ���
	LPD3DXSPRITE            m_pSprite;
	//��������
	//

	//
//��ԴTEXTURE
private:
	//����ͼƬTEXT
	LPDIRECT3DTEXTURE9      m_bgImgTexture[3];   
	//��Ϸ�߿�TEXT
	LPDIRECT3DTEXTURE9      m_BorderImgTexture;   
	//�ڵ���
    LPDIRECT3DTEXTURE9      m_PaoConImgTexture;   
	//�ڱ���
	LPDIRECT3DTEXTURE9      m_PaoBgImgTexture;   
	//2����
	LPDIRECT3DTEXTURE9      m_PaoTwoImgTexture;   
	//
	LPDIRECT3DTEXTURE9      m_PaoTwoFireImgTexture;   
	//3����
    LPDIRECT3DTEXTURE9      m_PaoThreeImgTexture;   
	//2��������
	LPDIRECT3DTEXTURE9      m_PaoThreeFireImgTexture;  
	//3��������
	//���·ֵı���ͼ
    LPDIRECT3DTEXTURE9      m_allscoreBgImgTexture[PLAY_COUNT];   
	//����ͼƬ
	LPDIRECT3DTEXTURE9      m_NumImgTexture[5];
	//���ͼƬ
	LPDIRECT3DTEXTURE9      m_GoldImgTexture;
	//2���ӵ�ͼ
    LPDIRECT3DTEXTURE9      m_BulltTwoTexture[2];
	//2�����ӵ���ͼ
	LPDIRECT3DTEXTURE9      m_PaoTwoNetImgTexture;   
	//���ζ�ͼ
	LPDIRECT3DTEXTURE9      m_FishMoveTexture[23][20];
	//������ͼ
	LPDIRECT3DTEXTURE9      m_FishDeadTexture[23][10];
	//��ѳ�Ʊͼ
	LPDIRECT3DTEXTURE9      m_MuchMoneyTexture[22];
	///רȦ����
	LPDIRECT3DTEXTURE9      m_QuanTexture[3];
	///��Ťͼ
	LPDIRECT3DTEXTURE9      m_BtTexture[5];
	//СǮ��
	LPDIRECT3DTEXTURE9      m_BiTexture;
	//���뱳��
	LPDIRECT3DTEXTURE9      m_LoadBgTexture;
	//�˳�����
	LPDIRECT3DTEXTURE9      m_WaterWaveTexture[2];
	//���˱���
	LPDIRECT3DTEXTURE9      m_WaterTexture[32];
	//�������
	LPDIRECT3DTEXTURE9      m_LoadTexture;
	//�������ӵ�
	LPDIRECT3DTEXTURE9      m_SupButTexture[2];
	//����״̬ͼƬ
	LPDIRECT3DTEXTURE9      m_SupStatTexture;
	//����״̬ͼƬ
	LPDIRECT3DTEXTURE9      m_OutTexture;
	//�����ʽ
	LPDIRECT3DTEXTURE9      m_CursorTexture;
	//������ͼ
	LPDIRECT3DTEXTURE9      m_DingTexture[10];


//���ú���
public:
  
    //�����ӵ�
    void	UserShoot(int PlayerID,float Roation,int RealChairID,bool isAndroid);
	//��Ǯ
	void	UserAddMoney(int PlayerID,int Fishid,int Money,int FishKindScord,bool ishavaction,bool bMoneyPile = true);
	//�л�����
	void	ChangeScreen(int bgindex);
	//��ҽ�����߳�ȥ
	void	UserComeInorLeave(int PlayerID,bool ComeOrLeave);
	//�����Լ���Ϣ
	void	SetMeInformation(int MePlayerid,int MeRellid ,CString sNickName , __int64 i64money);
	//������ұ���
	void	SetBeiLv(int PlayerId,int BeiScore){m_BeiLv[PlayerId] = BeiScore;if(m_configDlg.m_ptx2>0)PlayGameSound(71);};
	//������ҷ���
	void	SetUserScore(int PlayerId,__int64 Score){m_UserScore[PlayerId] = Score;};
	//��Ԫ��
	void	SetCellScore(int cellscore){m_CellScore = cellscore;};
	//�����Լ�����
	void	SetBaseScore(__int64 myscore){m_i64MyMoney =  myscore;}
	//���û������߼��ͻ���
	void	SetRobotLogic(bool bHitFlag, BYTE byHitDesk){m_bHitFlag = bHitFlag; m_bHitDesk = byHitDesk;};
	//�����
	void	AddFish(int traceX,int traceY,float roation,float movetime,float changetime,int fishtype,int ptindex,int nowindex,int fishid,int smallfishtype,int fishspeed);
	//����������·�
	//������������
	void	CleanAllMemory();
	//���ó��������
	void	SetSuperPao(int PlayerId,int Fishid){m_UserSuperPao[PlayerId]=true;}
	//ȡ�����������
	void	CancelSuperPao(int PlayerId){m_UserSuperPao[PlayerId]=false;}
	//���ӱ����б�ը
	void	SetBomAction(int PlayerId,int Fishid,int fishscore);
	//���Ӷ�ס
	void	SetDingAction(Fish *tfish);
	//������㲥
	void	CatchBroadCast(int iDeskNo,char szNickName[],char szRoomName[],int iFishType,__int64 i64FishScore);

private:
	//��ֱ�Ǯ
	void GiveMuchMoney(int Fishindex,int PlayerID,int Money,Fish *Deadfish,int fishscore);
	//����ͼƬ����
	void	LoadImg(const char* m_path,LPDIRECT3DTEXTURE9*  ppTexture);
    //��������
	void	LoadSound();

public:
	//
	bool        m_RunRender;
	//�Ƿ��Ѿ��ͷ����ͨ����
	bool		m_bHaveLinked;
	//��������
	int         m_Bgindex;

	//��ʼ������
	bool		InitDxDlog();
	//����DX���ڼ�������Դ
	HRESULT		InitD3D( HWND hWnd );
	
	//
	void	CheckHit();
	//��Ⱦ����
	void	Render();
	//��������
	void	LoadImg();
	//���ư�Ť
	void	DrawBt();

	//���ű�������
	void	PlayBgSound(int iIndex);
	//ֹͣ������Ч
	void	StopBgSound();
	//��Ч�Ƿ����ڲ���
	bool	IsPlaying(int iIndex);
	//������Ϸ��Ч
	void	PlayGameSound(int iIndex);
	//ֹͣ��Ϸ��Ч
	void	StopGameSound();

	//���׷��
	BOOL	_bMouseTrack;

	DECLARE_MESSAGE_MAP()
    //ʱ���¼�
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void	OnMouseMove(UINT, CPoint);
	afx_msg void	OnLButtonDown(UINT, CPoint);
	afx_msg void	OnLButtonUp(UINT, CPoint);
	afx_msg void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg	void	OnkeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnMouseHover(UINT, CPoint);
	afx_msg void	OnMouseLeave();
  
};
