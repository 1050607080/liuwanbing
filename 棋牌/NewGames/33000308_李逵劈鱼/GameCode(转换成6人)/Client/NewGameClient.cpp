#include "stdafx.h"
#include "NewGameClient.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "LoveSendClassInExe.h"

/*------------------------------------------------------------------------*/
extern int screen_width_ = DEF_SCREEN_WIDTH;
extern int screen_height_ = DEF_SCREEN_HEIGHT;

extern	float g_hscale = 1.0f;
extern	float g_vscale = 1.0f;
/*------------------------------------------------------------------------*/
HGE* hge_ = NULL;
//MiniDumper* g_mini_dumper = NULL;

// ��Դ���������
volatile int loading_result = 0; // 0 ���ڼ��� 1 ���سɹ� -1 ����ʧ��
int loading_progress_ = 0;
hgeSprite* spr_logo_ = NULL;
HTEXTURE tex_logo_ = NULL;
hgeSprite* spr_loading_bar_ = NULL;
HTEXTURE tex_loading_bar_ = NULL;
extern	hgeSprite* spr_GameFrame_Top_ = NULL;
HTEXTURE tex_GameFrame_Top_ = NULL;
extern	hgeSprite* spr_GameFrame_Min_ = NULL;
HTEXTURE tex_GameFrame_Min_ = NULL;
extern	hgeSprite* spr_GameFrame_Exit_ = NULL;
HTEXTURE tex_GameFrame_Exit_ = NULL;

HANDLE loading_handle_ = NULL;
HANDLE StartHge_hanle_ = NULL;


bool   gbEndGame = false;
/*-----------------------------------------------------------------------------*/
CNewGameClient* CNewGameClient::m_pNewGameClient = NULL;
/*-----------------------------------------------------------------------------*/

bool FrameFunc() 
{
	float delta_time = hge_->Timer_GetDelta();

	GuiManager::GetInstance().GetGui()->logic();

	if (loading_result == 1) 
	{
		if (GameManager::GetInstance().OnFrame(delta_time)) 
		{
			gbEndGame = true;
			return true;
		}
	} 
	else if (loading_result == -1) 
	{
		hge_->System_Log("��Դ����ʧ��");
		return true;
	}

	MP_Manager::GetInstance().UpdateByTimer();

	return false;
}
/*-----------------------------------------------------------------------------*/
bool RenderFunc() 
{
	hge_->Gfx_BeginScene();
	hge_->Gfx_Clear(0);


	if (loading_result == 1) 
	{
		GameManager::GetInstance().OnRender(static_cast<float>(screen_width_) / kResolutionWidth, static_cast<float>(screen_height_) / kResolutionHeight);
	} 
	else 
	{
		
		spr_logo_->RenderEx(0.0f, 0.0f, 0.0f, g_hscale, g_vscale);
		float bar_ypos = static_cast<float>(screen_height_) - hge_->Texture_GetHeight(tex_loading_bar_) * g_vscale;
		float bar_width = static_cast<float>(hge_->Texture_GetWidth(tex_loading_bar_));

		for (int i = 0; i < loading_progress_; ++i) 
		{
			spr_loading_bar_->RenderEx(i * bar_width * g_hscale, bar_ypos, 0.0f, g_hscale);
		}		
	}

	

	GuiManager::GetInstance().GetGui()->draw();

	// ֻ�м���Ч�� ֱ��д����.
	MP_Manager& MP=MP_Manager::GetInstance();
	HM_EMITTER hmEmitter=MP.GetFirstEmitter();
	while (hmEmitter) 
	{
		MP_Emitter* emitter=MP.GetEmitter(hmEmitter);
		emitter->Render();
		hmEmitter=MP.GetNextEmitter(hmEmitter);
	}

	hge_->Gfx_EndScene();

	return false;
}
/*-----------------------------------------------------------------------------*/
// ��Դ�����߳� ��ʱ�Դ���
// TODO:��content.res���resgroup����Ϊ��ͬ(��С����),Ȼ�����Precache��ʵ�ֽ���
unsigned __stdcall LoadingThread(void* param) 
{

	//if (GameManager::GetInstance().LoadGameResource(loading_progress_)) 
	//{
	//	loading_progress_ = 144;
	//	//Sleep(1000);
	//	loading_result = 1;
	//} 
	//else 
	//{
	//	loading_result = -1;
	//}

	int inum = 0;
	do 
	{
		if (GameManager::GetInstance().LoadGameResource(loading_progress_))
		{
			loading_progress_ = 144;
			loading_result = 1;
		}

		inum++;
	} while (inum > 2 || loading_result == 0);

	if (loading_result != 1)
	{
		loading_result = -1;
	}
	return 0;
}
/*-----------------------------------------------------------------------------*/
bool FocusGainFunc() 
{
		GameManager::GetInstance().set_game_active(true);
	return false;
}
/*-----------------------------------------------------------------------------*/
bool FocusLostFunc() 
{
		GameManager::GetInstance().set_game_active(false);
	return false;
}
/*-----------------------------------------------------------------------------*/
unsigned __stdcall StartHge(void* param) 
{
	//��ʼ�����DUMP�ļ�
	//g_mini_dumper = new MiniDumper(true);

	hge_ = hgeCreate(HGE_VERSION);

	//assert(0);  // for debug
	//���ô��ڴ�С
	RECT work_area;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &work_area, 0);
	int screen_real_height = work_area.bottom - work_area.top - GetSystemMetrics(SM_CYFIXEDFRAME) * 2 - GetSystemMetrics(SM_CYCAPTION);
	screen_width_ = GetSystemMetrics(SM_CXSCREEN);
	screen_height_ = GetSystemMetrics(SM_CXSCREEN) * kResolutionHeight / kResolutionWidth;
	if (screen_height_ > screen_real_height) 
	{
		screen_width_ = screen_real_height * kResolutionWidth / kResolutionHeight;
		screen_height_ = screen_real_height;
	}

	g_hscale = static_cast<float>(screen_width_) / kResolutionWidth;
	g_vscale = static_cast<float>(screen_height_) / kResolutionHeight;

	
	//g_rgnNoteRect[0].CreatePolygonRgn(ptPonit1,4,ALTERNATE);

	hge_->System_SetState(HGE_LOGFILE, "lkpy.log");		//HGE��־
	hge_->System_SetState(HGE_SHOWSPLASH, false);		//�ر�LOG
	hge_->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge_->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge_->System_SetState(HGE_FOCUSGAINFUNC, FocusGainFunc);
	hge_->System_SetState(HGE_FOCUSLOSTFUNC, FocusLostFunc);
	hge_->System_SetState(HGE_TITLE, "��������");		//����
	hge_->System_SetState(HGE_ICON,MAKEINTRESOURCEA(IDI_CLIENT_ROOM));
	hge_->System_SetState(HGE_USESOUND, true);				//ʹ����Ч
	hge_->System_SetState(HGE_WINDOWED, true);				//���ڻ�
	hge_->System_SetState(HGE_SCREENWIDTH, screen_width_);	//���ڿ��
	hge_->System_SetState(HGE_SCREENHEIGHT, screen_height_);//���ڸ߶�
	hge_->System_SetState(HGE_SCREENBPP, 32);				//��Ⱦɫ�����
	hge_->System_SetState(HGE_HIDEMOUSE, true);				//��ʾϵͳ���,��������Լ���Ⱦ���λͼ��Ӧ����Ϊtrue.������ʾϵͳ���.����Ϊbool.Ĭ��Ϊtrue.
	hge_->System_SetState(HGE_FPS, 30);
	hge_->System_SetState(HGE_DONTSUSPEND, true);
	hge_->System_SetState(HGE_RESENCRYPT, true);			//��Դ�Ƿ����

	
	// ��Դ��
	//hge_->Resource_AttachPack("fish\\resource.pak", "123456");
	//�����ʼ��
	if (hge_->System_Initiate()) 
	{
		HWND hwnd = hge_->System_GetState(HGE_HWND);
		//���ô��ڴ�С�̶�
		RECT rect;
		GetWindowRect(hwnd, &rect);
		SetWindowPos(hwnd, NULL, rect.left, 0, 0, 0, SWP_NOSIZE);
		//ShowWindow(hge_->System_GetState(HGE_HWND), SW_MINIMIZE);

		DWORD size;
		void* data;

		//����LOG0
		data = hge_->Resource_Load("lkpy\\images\\loading\\logo.fish", &size);
		tex_logo_ = hge_->Texture_Load(static_cast<const char*>(data), size);
		hge_->Resource_Free(data);
		spr_logo_ = new hgeSprite(tex_logo_, 0.f, 0.f, static_cast<float>(hge_->Texture_GetWidth(tex_logo_)),static_cast<float>(hge_->Texture_GetHeight(tex_logo_)));

		//������
		data = hge_->Resource_Load("lkpy\\images\\loading\\loading_bar.fish", &size);
		tex_loading_bar_ = hge_->Texture_Load(static_cast<const char*>(data), size);
		hge_->Resource_Free(data);
		spr_loading_bar_ = new hgeSprite(tex_loading_bar_, 0.f, 0.f, static_cast<float>(hge_->Texture_GetWidth(tex_loading_bar_)),static_cast<float>(hge_->Texture_GetHeight(tex_loading_bar_)));

		//�ϱ߿���
		data = hge_->Resource_Load("lkpy\\images\\Frame\\game_title_bg.fish", &size);
		tex_GameFrame_Top_ = hge_->Texture_Load(static_cast<const char*>(data), size);
		hge_->Resource_Free(data);
		spr_GameFrame_Top_ = new hgeSprite(tex_GameFrame_Top_, 0.f, 0.f, static_cast<float>(hge_->Texture_GetWidth(tex_GameFrame_Top_)),static_cast<float>(hge_->Texture_GetHeight(tex_GameFrame_Top_)));
		//��С��
		data = hge_->Resource_Load("lkpy\\images\\Frame\\min_window.fish", &size);
		tex_GameFrame_Min_ = hge_->Texture_Load(static_cast<const char*>(data), size);
		hge_->Resource_Free(data);
		spr_GameFrame_Min_ = new hgeSprite(tex_GameFrame_Min_, 0.f, 0.f, static_cast<float>(hge_->Texture_GetWidth(tex_GameFrame_Min_)),static_cast<float>(hge_->Texture_GetHeight(tex_GameFrame_Min_)));
		spr_GameFrame_Min_->SetTextureRect(0,0,42,31);
		//�ر�spr_GameFrame_Exit_
		data = hge_->Resource_Load("lkpy\\images\\Frame\\close_window.fish", &size);
		tex_GameFrame_Exit_ = hge_->Texture_Load(static_cast<const char*>(data), size);
		hge_->Resource_Free(data);
		spr_GameFrame_Exit_ = new hgeSprite(tex_GameFrame_Exit_, 0.f, 0.f, static_cast<float>(hge_->Texture_GetWidth(tex_GameFrame_Exit_)),static_cast<float>(hge_->Texture_GetHeight(tex_GameFrame_Exit_)));
		spr_GameFrame_Exit_->SetTextureRect(0,0,42,31);

		GuiManager::GetInstance().Initialize(screen_width_, screen_real_height);

		LPTSTR cmd_line;
		//��ʼ���ͻ���
		//GameManager::GetInstance().InitClientKernel(cmd_line);

		//����������Դ�߳�
		loading_handle_ = (HANDLE)::_beginthreadex(NULL, 0, LoadingThread, NULL, 0, NULL);
		//��������
		MP_Device_WRAP device(screen_width_,screen_height_, hge_);
		device.Create();

		MP_Manager& MP=MP_Manager::GetInstance();

		MP_Platform* platform=new MP_Platform_WINDOWS;
		MP.Initialization(platform, MAGIC_INTERPOLATION_ENABLE, MAGIC_NOLOOP, MAGIC_CHANGE_EMITTER_DEFAULT, 1366, 768, 1,(float)screen_width_ / (float)kResolutionWidth, 0.1f, true);
		//��������Ԫ��
		MP.LoadAllEmitters();

		MP.RefreshAtlas();

		MP.CloseFiles();

		MP.Stop();

		hge_->System_Start();

		MP.Destroy();
		device.Destroy();

		delete spr_loading_bar_;
		hge_->Texture_Free(tex_loading_bar_);
		delete spr_logo_;
		hge_->Texture_Free(tex_logo_);
		delete spr_GameFrame_Top_;
		hge_->Texture_Free(tex_GameFrame_Top_);
		delete spr_GameFrame_Min_;
		hge_->Texture_Free(tex_GameFrame_Min_);
		if (loading_result == 0) TerminateThread(loading_handle_, 0); // ��ʱ�Դ���
		if (loading_handle_ != NULL) ::CloseHandle(loading_handle_);

			GuiManager::GetInstance().DelInstance();
			GameManager::GetInstance().DelInstance();
	}

	//��ǿͻ��˹ر�;
	(CNewGameClient::GetMySelf())->m_bClose = true;

	hge_->System_Log("��ǿͻ��˹ر�");

	hge_->System_Shutdown();
	hge_->Release();

	return 0;
}
//-----------------------------------------------------------------------------------



void MsgPrint(const char *lpszFormat, ...)
{
	va_list pArg;
	va_start(pArg, lpszFormat);
	char szMessage[1024] = { 0 };
#ifdef WIN32
	_vsnprintf(szMessage, 1023, lpszFormat, pArg);
#else
	vsnprintf(szMessage, 1023, lpszFormat, pArg);
#endif
	va_end(pArg);
	OutputDebugString(szMessage);
}
//-----------------------------------------------------------------------------------
IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
	return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
//-----------------------------------------------------------------------------------
void DestroyNewGameImpl(IGameImpl **ppImp)
{
	delete *ppImp;
	*ppImp = NULL;
}
//-----------------------------------------------------------------------------------
CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;
	m_SoundEngineGame = createIrrKlangDevice();
	m_SoundEnginBg = createIrrKlangDevice() ; 
	m_pNewGameClient = this;
	m_bClose = false;
	m_iMyDeskStation = -1;
	m_GetGameStation = false;
	m_bIsWatching = false;//�Ƿ�Ϊ�Թ����
	::memset(m_UserInfo,NULL,sizeof(m_UserInfo));	/**<�����Ϣ*/
	InitialGameData() ; 

}
//-----------------------------------------------------------------------------------
CNewGameClient::~CNewGameClient()
{
	if(m_SoundEngineGame)
	{
		m_SoundEngineGame->drop();
	}

	if(m_SoundEnginBg)
	{
		m_SoundEnginBg->drop() ; 
	} 

}
//-----------------------------------------------------------------------------------
CNewGameClient * CNewGameClient::GetMySelf()
{
	if(m_pNewGameClient != NULL)
	{
		OutputDebugString("dwjgd::ָ�벻Ϊ��");
		return m_pNewGameClient;
	}
	else 
	{
		OutputDebugString("dwjgd::ָ�����");
		return NULL;
	}
}
//-----------------------------------------------------------------------------------
/// ��ʼ��
int		CNewGameClient::Initial()
{
	m_pUI->SetShow(false);
	return 0;
}
//-----------------------------------------------------------------------------------
/// ��ʼ��UI
int CNewGameClient::InitUI()
{	

	//����������Դ�߳�
	StartHge_hanle_ = (HANDLE)::_beginthreadex(NULL, 0, StartHge, NULL, 0, NULL);
	m_pUI->SetShow(false);
	SetThreadPriority(loading_handle_,THREAD_PRIORITY_HIGHEST);
	m_pGameFrame->SetTimer(TIME_CLOSE,1000);
	return 0;
}

//--------------------------------------------------------------------------------
/// ������Ϸ״̬
int CNewGameClient::ResetGameFrame(void)
{
	OutputDebugString("dwjlkpy::ResetGameFrame(0)");
	//����������Դ�߳�
	//StartHge_hanle_ = (HANDLE)::_beginthreadex(NULL, 0, StartHge, NULL, 0, NULL);
	//m_pUI->SetShow(false);
	//SetThreadPriority(loading_handle_,THREAD_PRIORITY_ABOVE_NORMAL);
	//m_pGameFrame->SetTimer(TIME_CLOSE,10);
	return 0;
}

//---------------------------------------------------------------------------------------
///��ʼ����Ϸ�е�����
void	CNewGameClient::InitialGameData()
{
	
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	m_bIsPlaying = false;

	//��Ϸ����
	LoadGameSet();
}
//---------------------------------------------------------------------------------------
///��ʼ����������
void	CNewGameClient::ResetGameData()
{
	m_bIsPlaying = false;
	
}
//---------------------------------------------------------------------------------------
//��ȡ��Ϸ����
void	CNewGameClient::LoadGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	
	//������Ч
	m_bSoundBgPlay	= ("1" == f.GetKeyVal(key,"BgSound","1"));
	//��Ϸ��Ч
	m_bSoundPlay	= ("1" == f.GetKeyVal(key,"GameSound","1"));

}
//---------------------------------------------------------------------------------------
//������Ϸ����
void	CNewGameClient::SaveGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	

	IRadioButton *pRadioButton = NULL;

	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			//����û�в��� ��ô�����Ҫ��ʼ���ű���������
			if (!m_bSoundBgPlay)
			{
				m_bSoundBgPlay = true;
				PlayBgSound(true);
			}
			
			f.SetKeyValString(key,"BgSound","1");
		}
		else
		{
			//������ڲ��� ��ô��Ҫֹͣ��
			if (m_bSoundBgPlay)
			{
				m_bSoundBgPlay = false;
				if (NULL != m_SoundEnginBg)
				{
					m_SoundEnginBg->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"BgSound","0");
		}
	}
	//��Ϸ��Ч

	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			m_bSoundPlay = true;
			f.SetKeyValString(key,"GameSound","1");
		}
		else
		{
			//������ڲ�����Ч ��ʱ��Ҫֹͣ
			if (m_bSoundPlay)
			{
				m_bSoundPlay = false;
				if (NULL != m_SoundEngineGame)
				{
					m_SoundEngineGame->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"GameSound","0");
		}
	}
}
//---------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
/// ������Ϸ�ĵ�ǰ״̬
void CNewGameClient::SetGameStation()
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ;

	SetStationParameter(m_bufferForGameStation.game_status);

	switch(GetStationParameter())
	{
	case GAME_STATUS_FREE:
	case GAME_STATUS_PLAY:
		{
			if (NULL != GameManager::GetpGameManager())
			{
				//��������
				CMD_S_GameConfig* pCmd1 = &m_bufferForGameStation.GameConfig;
				GameManager::GetInstance().OnEventSocket(MDM_GM_GAME_NOTIFY, SUB_S_GAME_CONFIG, (void *)pCmd1, sizeof(CMD_S_GameConfig));
				//״̬����
				CMD_S_GameStatus* pCmd2 = &m_bufferForGameStation.GameStatus;
				GameManager::GetInstance().OnEventSceneMessage(GetStationParameter(), false, (void *)pCmd2, sizeof(CMD_S_GameStatus));
			}

			break;
		}
	default:
		{
			break;
		}
	}

	m_GetGameStation=true;
	//���ű�����Ч
	//PlayBgSound(true);
	return  ; 
}
//-----------------------------------------------------------------------------------------
/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 

	if (!m_GetGameStation && nSubId != ASS_GM_GAME_STATION)//�ڵȴ���Դ����ʱ������յ��Ĳ��Ƕ����ػ���Ϣ����
	{
		return true;
	}

	if (gbEndGame)
	{
		OutputDebugString("sdp_ ��Ϸ���ڹر� �ͻ��� ��Ȼ�յ���Ϣ");
		return true;
	}

	//GameManager::GetInstance().ShakeScreen();

	switch(nSubId)
	{
	case SUB_S_GAME_CONFIG:
	case SUB_S_FISH_TRACE://��������Ĺ켣
	case SUB_S_EXCHANGE_FISHSCORE:
	case SUB_S_USER_FIRE://������
	case SUB_S_CATCH_FISH:
	case SUB_S_BULLET_ION_TIMEOUT://�ӵ���ΧʧЧ
	case SUB_S_LOCK_TIMEOUT://������ʧЧʱ��
	case SUB_S_CATCH_SWEEP_FISH:
	case SUB_S_CATCH_SWEEP_FISH_RESULT:
	case SUB_S_HIT_FISH_LK:
	case SUB_S_SWITCH_SCENE:
	case SUB_S_STOCK_OPERATE_RESULT:
	case SUB_S_SCENE_END:
	case SUB_S_GAMEOUT://�����ǿ���ÿͻ����˳�
	case SUB_S_CATCH_SWEEP_FISH_RESULT_EX:
		{
			if (loading_result != 1) //��Դ��û�м������
			{
				return true;
			}

			if (NULL != GameManager::GetpGameManager())
			{
				GameManager::GetInstance().OnEventSocket(MDM_GM_GAME_NOTIFY, nSubId,(void*)buffer, nLen);
			}
			return TRUE;
		}
	case ASS_GM_GAME_STATION:   ///<����״̬
		{
			if(nLen != sizeof(GameStation))
			{
				return TRUE;
			}

			GameStation * pCmd=(GameStation *)buffer;
			m_bIsWatching = m_pGameFrame->GetIsWatching();
			CopyMemory(&m_bufferForGameStation, pCmd, sizeof(GameStation));
			m_pGameFrame->SetTimer(TIME_WAIT_LOAD,50);
			return TRUE;      
		}
	case ASS_GM_AGREE_GAME:
		{		
			return TRUE;
		}
	default:
		{
			break;
		}
		
	}
	return 0;
}
//------------------------------------------------------------------------------
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	return 0;
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------

/// ��ҽ���
int CNewGameClient::GameUserCome(void)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 	
	
	//��ȡ�����Ϣ ����¼����Ƿ�����Ϸ��
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		m_pGameFrame->GetUserInfo(i,m_UserInfo[i]);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
/// ����뿪
int		CNewGameClient::GameUserLeft(int nIndex)
{
	return 0;
}
/*--------------------------------------------------------------------------*/

/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	return 0;
}
// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	if (m_bIsPlaying)	//��Ϸ�Ƿ��Ѿ���ʼ
	{
		return true;
	}
	return false;
}
//--------------------------------------------------------------------
/// ��ʱ����Ϣ����Ϸ�еĶ�ʱ������
void CNewGameClient::OnTimer(int nTimerID)
{
 	switch(nTimerID)
 	{
	case TIME_WAIT_LOAD:
		{
			m_pGameFrame->KillTimer(TIME_WAIT_LOAD);
			if (NULL != GameManager::GetpGameManager())
			{
				if (GameManager::GetInstance().GetGameBeReady())
				{
					SetGameStation();
					return;
				}
			}


			m_pGameFrame->SetTimer(TIME_WAIT_LOAD,50);
		}
		break;
	case TIME_CLOSE:
		{
			m_pGameFrame->KillTimer(TIME_CLOSE);
			if(m_bClose)
			{
				if (StartHge_hanle_ == 0) TerminateThread(StartHge_hanle_, 0); // ��ʱ�Դ���
				if (StartHge_hanle_ != NULL) ::CloseHandle(StartHge_hanle_);
				m_pUI->SetShow(true);
				m_pGameFrame->KillTimer(TIME_CLOSE);
				m_pGameFrame->CloseClient();

				return;
			}
			m_pGameFrame->SetTimer(TIME_CLOSE,1000);
			break;
		}
	default:
		{
			break;
		}
	}
	return ; 

}
//-------------------------------------------------------------------------------
/// ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_byGameStation = bGameStation ; 
}
//-------------------------------------------------------------------------------
///��ȡ��Ϸ״̬
BYTE CNewGameClient::GetStationParameter()
{
	return m_byGameStation;
}
//-------------------------------------------------------------------------------
///����˳���Ϸ
bool CNewGameClient::OnGameQuiting()
{
 	if(m_pGameFrame->GetIsWatching())
 	{
 		return true ; 
 	}
	return true;
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//���ű�����Ч
void	CNewGameClient::PlayBgSound(bool bISLoop, int imusictag)
{
	if(!m_SoundEnginBg)
	{
		return ;
	}
	if(!m_bSoundBgPlay)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	wsprintf(szFileName,"lkpy\\sounds\\bgm\\bgm%d.wav",imusictag+1);

	m_SoundEnginBg->setSoundVolume(0.5);
	m_SoundEnginBg->play2D(szFileName,bISLoop);
}
//-------------------------------------------------------------------------
//��ͼλ��
int CNewGameClient::ViewStation(int desk)
{
	if (NULL != m_pGameFrame)
	{
		return m_pGameFrame->DeskStation2View(desk);
	}

	return 0;
}

//����Ƿ����
bool CNewGameClient::isPlayer(int desk)
{
	UserInfoForGame UserInfo;
	if (NULL != m_pGameFrame)
	{
		if (m_pGameFrame->GetUserInfo(desk, UserInfo))
		{
			return true;
		}
	}

	return false;
}

bool CNewGameClient::GetPlayerInfo(int desk, UserInfoForGame& UserInfo)
{
	UserInfoForGame UserInfo2;
	if (NULL != m_pGameFrame)
	{
		if (m_pGameFrame->GetUserInfo(desk, UserInfo2))
		{
			CopyMemory(&UserInfo, &UserInfo2, sizeof(UserInfoForGame));
			return true;
		}
	}

	return false;
}



/*-------------------------------------------------------------------*/
//ת����λ
int		CNewGameClient::SwitchChairID(int m_Currchairid)
{
	int changChairID = m_Currchairid;
	switch(m_pGameFrame->GetMyDeskStation())
	{
	case 0:
	case 1:
	case 2:
		{
			switch(m_Currchairid)
			{
			case 0:return 3;
			case 1:return 4;
			case 2:return 5;
			case 3:return 0;
			case 4:return 1;
			case 5:return 2;
			}

		}
	case 3:
	case 4:
	case 5:
		{
			switch(m_Currchairid)
			{
			case 0:return 0;
			case 1:return 1;
			case 2:return 2;
			case 3:return 3;
			case 4:return 4;
			case 5:return 5;
			}

		}
	}
	return changChairID;
}