#include "IGameImpl.h"
#include "UI_ForLogic/IUserInterface.h"

#include "..\ChangeFile\UserDataEx.h"
#include "..\ChangeFile\GameDataEx.h"
#include "sounddefines.h"

#include <irrKlang.h>
using namespace irrklang;
//#include "SoundsDef.h"
#include <queue>;

interface IGameFrame;
interface IUserInterface;

const BYTE MY_VIEWSTATION = 4;				// �Լ�����ͼλ������4

#define     BN_VOLUM_UP            13      ///< ������
#define     BN_VOLUM_DOWN          14      ///< ������
#define     BN_VOLUM_OFF           15      ///< ���ֹ�
#define     BN_VOLUM_ON            16      ///< ���ֿ�



#define TIME_OUT_CARD			10		//���Ƽ�ʱ��
#define TIME_BLOCK_CARD			11		//���Ƽ�ʱ��
#define TIME_ZHUAPAI			12		// ץ�Ƽ�ʱ��
#define TIME_HIDE_SEZI_ANI		13      // ɫ�Ӷ������ź��������Ķ�ʱ��
#define TIME_BEGIN				14      // �Զ���ʼ
#define TIME_FAPAI				15      // ���ƶ�ʱ��
#define TIME_DING_QUE			16      // ��ȱ�Ŷ�ʱ��
#define ID_WARMING_TIME			17		// ���涨ʱ��

#define TIME_FENG_YU			20      // �η�����Ի���ͣ��ʱ��
#define TIME_SHOW_HU			21      // ��ʾ���ƶԻ���ʱ��


#define TIME_TALKING_0			25		// ���0������ʾ��ʱ��
#define TIME_TALKING_1			26		// ���1������ʾ��ʱ��
#define TIME_TALKING_2			27		// ���2������ʾ��ʱ��
#define TIME_TALKING_3			28		// ���3������ʾ��ʱ��


struct MjUiInfo
{
    int         m_nUserHandInfo[20];
    int         m_nHandCardCount;

    int         m_nUserOutInfo[OUT_CARD_NUM];
    int         m_nOutCardCount;

	bool        m_bIsBigGang[5];//�Ƿ���
    BYTE        m_ucCPGData[5][4];
    BYTE        m_ucSpeCPGData[5][4];//���������
    BYTE        m_ucSpeCPGNum[5][4];//������Ƶ�����
    BYTE        m_ucChair[5];
    BYTE        m_ucCPGTile[5];
    MJ_LOGIC_TYPE m_CPGtype[5];
    int         m_nCPGCount;
    
    MjUiInfo()
    {
        memset(this, 0, sizeof(MjUiInfo));
    }
    void ClearHand()
    {
        memset(m_nUserHandInfo, 0, sizeof(m_nUserHandInfo));
        m_nHandCardCount = 0;
    }
    void GetFromByte(BYTE ucByteInfo[])
    {
        ClearHand();
        for (int i=0; i<17; ++i)
        {
            if (0==ucByteInfo[i] || 255==ucByteInfo[i])
            {
                break;
            }
            m_nHandCardCount++;
            m_nUserHandInfo[i] = ucByteInfo[i];
        }
    }
    void FillWithBack(int nNum)
    {
        memset(m_nUserHandInfo, 0, sizeof(m_nUserHandInfo));
        m_nHandCardCount = nNum;
    }
    void ClearOut()
    {
        memset(m_nUserOutInfo, 0, sizeof(m_nUserOutInfo));
        m_nOutCardCount = 0;
    }
    void GetOutFromByte(BYTE ucByteInfo[])
    {
        ClearOut();
        for (int i=0; i<OUT_CARD_NUM; ++i)
        {
            if (0==ucByteInfo[i] || 255==ucByteInfo[i])
            {
                break;
            }
            m_nUserOutInfo[i] = ucByteInfo[i];
			m_nOutCardCount++;
        }
    }
    void ClearCPG()
    {
        memset(m_bIsBigGang, 0, sizeof(m_bIsBigGang));//�Ƿ���
        memset(m_ucSpeCPGData, 0, sizeof(m_ucSpeCPGData));//���������
        memset(m_ucSpeCPGNum, 0, sizeof(m_ucSpeCPGNum));//������Ƶ�����
        memset(m_ucCPGData, 0, sizeof(m_ucCPGData));
        memset(m_ucChair, 0, sizeof(m_ucChair));
        memset(m_ucCPGTile, 0, sizeof(m_ucCPGTile));
        memset(m_CPGtype, LT_EMPTY, sizeof(m_CPGtype));
        m_nCPGCount = 0;
    }
    void GetFromCPGData(GCPStructEx cpgstr[],bool show=true)
    {
        ClearCPG();
		int index = 0;
        for (int i=0; i<5; ++i)
        {
			if(cpgstr[i].byType == 255)
				continue;
			if(cpgstr[i].bIsBigGang)
			{
				if (-1==cpgstr[i].byBigGang[0][0] || 255==cpgstr[i].byBigGang[0][0] || 0==cpgstr[i].byBigGang[0][0])
				{
					break;
				}
				for (int j=0; j<4; ++j)
				{
					m_ucSpeCPGData[i][j] = cpgstr[i].byBigGang[j][0];	
					m_ucSpeCPGNum[i][j]	 = cpgstr[i].byBigGang[j][1];	//ÿ���Ƶ�����
				} 
				m_bIsBigGang[i] = true;//��־���
			}
			else
			{
				if (-1==cpgstr[i].byData[0] || 255==cpgstr[i].byData[0] || 0==cpgstr[i].byData[0])
				{
					break;
				}
				for (int j=0; j<4; ++j)
				{
					m_ucCPGData[i][j] = cpgstr[i].byData[j];                
				} 
			}
            m_ucChair[i] = cpgstr[i].iBeStation;
            m_ucCPGTile[i] = cpgstr[i].iOutpai;
            m_nCPGCount ++;

            if (ACTION_CHI==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_COLLECT;
            }
            else if (ACTION_PENG==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_TRIPLET;
            }
            else if (ACTION_AN_GANG==cpgstr[i].byType)
            {
                for (int j=0; j<3; ++j)
                {
                    m_ucCPGData[i][j] = 0;
                }
				if(!show)
					m_ucCPGData[i][3] = 0;
                m_CPGtype[i] = LT_QUADRUPLET_CONCEALED;
            }
            else if (ACTION_BU_GANG==cpgstr[i].byType || ACTION_MING_GANG==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_QUADRUPLET_REVEALED;
            }
        }
    }
};


class CNewGameClient : public IGameImpl
{

public:

    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);

    virtual ~CNewGameClient();

private:

    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);

public:

#define SAFE_CTRL_OPERATE(CTRL_TYPE,CTRL_ID, OPERATE)\
	{\
	CTRL_TYPE* PRE=NULL; \
	PRE = dynamic_cast<CTRL_TYPE *>(m_pUI->GetIControlbyID(CTRL_ID));\
	if (PRE!=NULL) PRE->OPERATE; \
	}\

/*-----------------------------------------------------------------------------------------------------------
	һϵ��ƽ̨�ṩ����Ҫ������������Ϸ�ĺ����������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
  ----------------------------------------------------------------------------------------------------------*/
public:
    //��ʼ��
    virtual int Initial();
    //��ʼ��UI
    virtual int InitUI();
    //��Ϸ��Ϣ
    virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
    //������Ϣ
    virtual int OnUIMessage(TUIMessage* pMessage);
    //
    virtual int ResetGameFrame(void);
    //��ҽ���
    virtual int GameUserCome(void);
    //����뿪
    virtual int GameUserLeft(int nIndex);
    //��Ҷ���
    virtual int GameUserOffLine(void);
    //�Ƿ�������Ϸ������Ϸʵ��
    //�Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
    virtual bool IsPlayingGame();
    //��ʱ����Ϣ
    virtual void OnTimer(int nTimerID);
    //��״̬
    virtual void SetStationParameter(BYTE bGameStation);
	//������Ϸ״̬����Ҫ�Ƕ����������������Ϣ
    virtual void SetGameStation(void * pBuffer,int nLen);
	//�˳���Ϸ
	virtual bool OnGameQuiting();
	//��ȡ��Ϸ״̬
	BYTE GetStationParameter();

	///���¿���ʱ�ĳ�ʼ��
	void ReStartGameInitUI();
/*-----------------------------------------------------------------------------------------------------------
	�����߼�����ģ��
  ----------------------------------------------------------------------------------------------------------*/

public:

	BYTE		 m_byShowFaPai;


private:

	//��Ϸ���
    IGameFrame* m_pGameFrame;
	//��ϷUI
    IUserInterface* m_pUI;
	//������������
	ISoundEngine* m_pUIBkMusic;
	//��Ϸ��Ч����
	ISoundEngine* m_pUIGameSound;


	//��Ϸ״̬
	BYTE m_bGameStation;
	//��Ϸ����������Ч����
	bool m_bBgMusic;
	//��Ϸ��������
	bool m_bBkMusic;
	//��Ϸ��Ч����
	bool m_bGameSound;
	//�ÿͻ����Ƿ������Թ�
	bool m_bAllowWatch;


    ///��Ϸ����
    MJ_Setting      m_MJSetting;
	///��ҵ����ݣ����ƣ����ƣ��������Ƶ�
	UserDataEx      m_UserData;	
	GameDataEx      m_GameData;	

    BYTE            m_byNtLogicPos; //ׯ��
    BYTE            m_ucJinPai[2];  //����
    BYTE            m_ucCurrentPlayer;  //��ֵ���
    BYTE            m_ucLastTile;       //�ϴγ�����
    BYTE            m_ucHandState;      //����״̬ 0����״̬ 1�����ѡ��״̬
	BYTE			m_nEnableTiles[MAX_TILES_INCTRL];//���Ըܵ���

    MjUiInfo        m_UserDataCard[PLAY_COUNT];
	
	///һЩ��Ϸ�п�����Ҫ�õ��ĸ�����Ϣ
	int             m_nDeskStation;    ///<�Լ��ڷ������ϵ���λ��
    int             m_nWhichTask;       /// ÿ������

    tagTwoSeziJingEx  m_tagJinPai;		
    tagTwoSeziGetPaiEx m_tagSeziGetPai;	
    tagTwoSeziNtEx    m_tagSeziNt;		
    tagTwoSeziDirEx   m_tagSeziDir;		
    tagSendPaiEx      m_tagSendpai;		
    tagCountFenEx     m_tagCountFen;	
	
	
    int               m_nTimeOutCnt;
    bool              m_bIsTuoGuan[PLAY_COUNT];

    ////////////////////////////////////////////////////////////////////////////////
    // ���ƶ�����
    BYTE            m_ucOpenDoor;
    BYTE            m_ucOpenDun;
    int             m_nStep;
    ////////////////////////////////////////////////////////////////////////////////
    // ץ�ƶ�����
    int             m_nZhuaViewStaiton;
    ////////////////////////////////////////////////////////////////////////////////
    // ����������
    int             m_nFanJinState;     // 0�Ͼ�����ǽ���м䣬1�Ͼ����м䵽���Ͻǣ�2�¾�����ǽ���м䣬3�¾����м�����ƣ�4���Ͼ������ϻ���ǽ��5������С��
    POINT           m_ptShangJin;
    POINT           m_ptXiaJin;
    POINT           m_ptCenter;
    POINT           m_ptJinLeftTop;
    int             m_nHandIndex;       // С������һ��
    int             m_nHandPos;         // С������һ��
    int             m_nJinWallIndex;    // ����λ�ã��գ� = -1;
    int             m_nJinDoor;         // ����λ��= -1;

	int				m_GangPai[2][4];
	BYTE			m_GangType[2];// ���Ƶ�����
	bool			m_bIsBigGang[2];//�Ƿ���
	BYTE			m_byCheckPai;//ѡ��ĸ���

    int				m_nInvalidTiles[MAX_TILES_INCTRL];//��ǰ���ܳ�����
	bool			m_bStopSendPai;//�����Ƿ����
	int				m_iFinshiX;//������ʼλ��
	int				m_iFinshiY;//������ʼλ��

	BOOL			m_bSound;                   ///�Ƿ�����Ч
	int				m_TimeClockCount;           ///���ӵ���ʱʱ��
	BYTE			m_bTimeOutCount;            ///��ʱ����
	int				m_iSendCardCount;//��ǰ������
	int				m_iTimeWarm;	//�����ʱ��ʱ��
	wchar_t			m_wszNiCheng[PLAY_COUNT][256];//��Ϸ�б�����ҵ��ǳ�

	
public:


    // �û��й�
    void OnUserTuoguan(void *buffer, int nLen);
    // �û�ͬ��
    void OnUserAgree(void *buffer, int len);

    // ��Ϸ��ʼ
    void OnGameStart(void *buffer, int len);
	/// ��0λ���Ϊׯ����Ϣ:��һ��
	void OnDongNt(void *buffer, int nLen);
    // �����Ƶ���
    void OnSeziSureOpenPoint(void *buffer, int nLen);
    // ������������
    // ע���ʱû�������ݣ����ƺ����
    void OnSendAllCard(void *buffer, int nLen);
    // ��Ϸ��ʼ������
    void OnGamePlay(void *buffer, int nLen);


    // ����
    void OnNotifyOutCard(void *buffer, int nLen);
    // ץ��
    void OnNotifyZhuaPai(void *buffer, int nLen);
    // ����
    void OnNotifyBlock(void *buffer, int nLen);
    // ����
    void OnNotifyChiPai(void *buffer, int nLen);
    // ����
    void OnNotifyPengPai(void *buffer, int nLen);
    // ����
    void OnNotifyGangPai(void *buffer, int nLen);


    // ����
    void OnNotifyHuPai(void *buffer, int nLen);    
    // ����
    void OnNotifyCancelBlock(void *buffer, int nLen);
    // ����
    void OnNotifyFinish(void *buffer, int nLen);
	 /// ǿ�˽�����Ϸ
	void OnForceFinish( void *buffer, int nLen);

	/// ��ȱ�¼�
    void OnNotifyDingQue(void *buffer, int nLen);  
	/// ����������
	void OnNotifySuperSetPai(void *buffer, int nLen);			
    // ��ʾ�����
    void UIShowResultDlg(tagCountFenEx * fen , tagHuPaiEx * hu ,BYTE type);

    // �������
    void RequestOutCard(BYTE ucTile);
    // �������
    void RequestBlockChi();
    // ��������
    void RequestBlockPeng();
    // �������
    void RequestBlockGang();
    // �������
    void RequestBlockHu();
    // ��������
    void RequestCancelBlock();
    // �����й�
    void RequestTuoGuan(bool bTuoguan = true);
	//�Ը�����
	void RequestBlockChiGang(UINT32 id);
//////////////////////////////////////////////////////////////////////

    // ��ʾ��ǽ
    void UIShowWall(BYTE bViewStation, bool bShow = true);
    // ��ʾ��ǽ����
    // ���ݸ�ʽ��255��û���ƣ� 0���Ʊ�������������һ��
    void UIShowWall(BYTE station, BYTE ucArrMenPai1[]);
    // ��ʾׯ�ұ�־
    void UIShowZhuang(BYTE bViewStation, bool bShow = true);
    // ������������
    void UISetHandCard(BYTE bViewStation, int nHandInfo[], int nCount, bool bShowGive = false);
    // ���ó���
    void UISetOutCard(BYTE bViewStation, int nOutInfo[], int nCount, bool bShowGive = false);
///////

    // ���Ӷ���
    void UIShowsaizi(BYTE bViewStation, int nPoint1, int nPoint2);
    // ������ʾ
    void UIShowLastTileTip(BYTE bViewStation, bool bShow,BYTE pai=255);
    // ��ʾ���ƿؼ�
    void UIShowGangCtrl(bool bShow,int pai[] = NULL,int pai2[]=NULL);
	// ��ʾ�Ը��ƿؼ�
	void UIShowChiGangCtrl(bool bShow,bool chi);
////////

   // @brief ��ʾ׼��
    void UIShowReady(BYTE bViewStation, bool bShow );
    //@brief ��ʾ�й�
    void UIShowTrustee(bool bShow);
    //@brief ��ʾ�ȴ�����
    void UIShowWaiting(bool bShow);
	///��ʾ�ⶥ
	void UIShowFengDing();
    // @brief ��ʾ�ƶ������ؼ�
    void UIShowZhuaMoveAction(POINT ptStart, POINT ptEnd, BYTE ucViewStatin, bool bShow);
    // ��ʾȱ��
    void UIShowQueMen(bool show,BYTE station = 255);
	//��ʾ�η�����Ի���
	void ShowGFXY(bool show,BYTE station = 255,BYTE bestation = 255,BYTE type = 255);

    // �������                
    void OnUIRequestOutCard(UINT32 uControlID,bool outtime = false);
    // ��������
    void OnRequestBlock(UINT32 uControlID);
    // ���齫�ؼ�������ʱ������ƶ����ؼ���
    int OnMoveOnMjCtrl(TUIMessage* pMessage);
    // ���齫�ؼ�������ʱ����굥���ؼ�
    int OnLButtonOnMjCtrl(TUIMessage* pMessage);
    // �����ؼ���Ϣ
    int OnCartoonFinish(TUIMessage *pMessage);



public:


	//��ʾ��ʼ����״̬����
	void UIshowBeginOut();
	//���Ŷ���
	void PlayAni(int Id);
	//������ҵĵ��ǳ�
	void GetUserName();

	//���ü�ʱ��
	void OnSetTimer(BYTE station,int type);
	//ɾ������ʱ��
	void OnKillAllTimer();
	/// ������ʱ��
	BOOL KillGameTimer(int TimerID);
    // ���ö�ʱ��
    void UIShowTimer(BYTE bViewStation, int nTime, bool bShow = true);



	//������Ϸ����:����֪ͨ��Ϣ������������
	void SendGameDataEx(BYTE id);
	//������Ϸ����
	void SendGameDataEx(BYTE id,void *pData,UINT uSize);
	//��˸����
	void FlashWindow();
	//������췢��UI��Ⱦ
	void ShowTalk(BYTE bDeskstation,wchar_t* pwszText,int iDispMillSeconds/* = 5000*/);
	//������췢��UI��Ⱦ����
	void ShowTalkFinish(BYTE bViewstation);


	//���ַ���ת����������
	void GetPaiFormChar(int &count ,BYTE data[],const wchar_t pszPai[],int type);
	//�������ַ�ת������
	BYTE ChangeChar(wchar_t data1,wchar_t data2);
	//��ȡ�ؼ�����,��������������
	void SendSuperData();
	//��ʾ�������س����ͻ��˷���
	void ShowSuperSetPai(bool show);


	//��ȡ��Ϸ״̬
	BYTE GetGameStationEx(){return m_bGameStation;};
	//������Ϸ״̬
	void SetGameStationEx(BYTE state)
	{
		m_bGameStation = state;
	SetStationParameter(state);};

	//������������á������Ӧ
	void OnClickedConfig();
	//������������á�ȷ����ť�����Ӧ
	void OnClickedConfigOK();
	//������������á�ȡ����ť�����Ӧ
	void OnClickedConfigCancel();
	//������Ϸ����
	void LoadGameConfig();
	//������Ϸ����
	void SaveGameConfig();
	//��ʼ������
	void InitGameDate();

// ����
private:
#define MAX_SOUND_RAND 4    // �м��ֲ�һ��������
    struct SOUND_INFO
    {
        TCHAR szFileName[MAX_SOUND_RAND][MAX_PATH];
        int nCount;       
        SOUND_INFO()
        {
            memset(szFileName, 0, sizeof(szFileName));
            nCount = 0;
        }
        SOUND_INFO(const SOUND_INFO &other)
        {
            memcpy(szFileName, other.szFileName, sizeof(szFileName));
            nCount = other.nCount;
        }
        SOUND_INFO &operator = (const SOUND_INFO &other)
        {
            if (&other==this)
            {
                return *this;
            }
            memcpy(szFileName, other.szFileName, sizeof(szFileName));
            nCount = other.nCount;
            return *this;
        }
    };
    std::map<BYTE,SOUND_INFO>           m_SoundMap;
private:
    /** @brief �������ļ��ж�ȡ�����ļ���
     */
    void GetSoundMapFromCfg();
    /** @brief ���ų�������
    */
    void PlayTileSound(int nDeskStation,BYTE ucTile);
    ///**
    //* @brief ������ͨ�������������ԣ�
    //*/
    void PlayNormalSound(const char *pszFileName);
	//������Ϸ����
	void PlayBkMusic(bool bOn);


};