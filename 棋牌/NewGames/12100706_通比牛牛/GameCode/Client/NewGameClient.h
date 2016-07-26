#pragma once

#include "IGameImpl.h"
#include "IGameFrame.h"
#include "..\GameMessage\UpGradeLogic.h"
#include "irrKlang.h"
using namespace irrklang;
interface IGameFrame;
interface IUserInterface;

#define	 SOUND_START						50					//��Ϸ��ʼ
#define	 SOUND_DEAL							51					//����
#define  SOUND_HIT_CARD						52					//����
#define  SOUND_OUT_CARD						53					//����
#define	 SOUND_CLOCK						54					//����
#define	 SOUND_ESCAPLE						55					//����
#define	 SOUND_WIN							56					//Ӯ
#define	 SOUND_LOSE							57					//��
#define	 SOUND_NIUNIU						58					//ţţ

//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_THINK_TIME				103					//˼����ʱ��
#define ID_SCARD_TIME				104					//���ƶ�ʱ��
#define ID_RESULT_TIME				105					//������ʱ��
#define ID_SUPER_TIME				106					//����������ʾ��ʱ��
#define ID_SUPER2_TIME				107					//���˰�ť��ʾ��ʱ��

struct SoundNews
{
	BYTE				byDeskStation;		//����λ��
	BYTE				byCureentAction;	//��ǰ����
	BYTE				byCardShape;		//����
	int					iCardCount;			//������
	BYTE				byCardPoint;		//��ֵ
	bool				bExitProess;		//�˳��߳�

	SoundNews()
	{
		memset(this,NULL,sizeof(SoundNews));
	}
};

//��Ϸ����� 
class CNewGameClient : public IGameImpl
{
private:
	IGameFrame				*m_pGameFrame;						///��Ϸ���

	IUserInterface			*m_pUI;								///��ϷUI

	CUpGradeGameLogic		m_Logic;							///�߼�

	UserInfoForGame			m_UserInfo[PLAY_COUNT];			/**<�����Ϣ*/

	ISoundEngine			*m_SoundEngine;						///��Чָ��

	CWinThread				*m_tSond;							/**<�߳�ָ��*/

	SoundNews				m_pSoundNews;						/**<������Ϣ*/

	IniData					m_sIData;							///�����INI�ļ�����

	BData					m_sBData;							///��Ϸ��������

	BYTE					m_bGameStation;						///��Ϸ״̬

	BYTE					m_byLgStation;						///����߼�λ��

	bool					m_bWatchMode;						///�Ƿ�Ϊ�Թ���

	bool					m_bWatch;							///�Ƿ�����Թ�

	bool					m_bSound;							///�Ƿ񲥷���Ч

	bool					m_bShowComma;						///�Ƿ���ʾ����	

	BYTE					m_bySpaceBits;						///���÷ָ�λ��	1��9������

	int						m_iUCCount[PLAY_COUNT];				///�����������

	BYTE					m_byUCard[PLAY_COUNT][USER_HAND_CARD];	///����˿�
				
	BYTE                    m_byDownBCard[PLAY_COUNT][USER_HAND_CARD];	///�¶���

	int						m_iSCCount;							///��������

	//���˱���
	BYTE					m_bSuperTag[PLAY_COUNT];			//���˰�ť����(0Ϊ��ʼֵ��1ΪӮ��2Ϊ��)
	bool					m_bSuperSet;						//���������Ƿ�ɹ�
	bool					m_bSuperShow;						//��ʾ���˸������ð�ť
	bool					m_bCanSuper;						//�Լ��������ó���

	//��������
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);

#define SAFE_CTRL_OPERATE(CTRL_TYPE,CTRL_ID, OPERATE)\
	{\
		CTRL_TYPE* PRE=NULL; \
		PRE = dynamic_cast<CTRL_TYPE *>(m_pUI->GetIControlbyID(CTRL_ID));\
		if (PRE!=NULL) PRE->OPERATE; \
	}
public:  ///����ƽ̨����
    /// ��ʼ��
    virtual int Initial();

    /// ��ʼ��UI
    virtual int InitUI();

	/**
	* ���շ���˷��͹�������Ϣ����������Ϸ�ĵ���״̬��
	* ����������ʱ����룬��������Ϸ�ĵ�ǰ״̬
	*
	* @param [in] void * pBuffer ���ݰ�
	*
	* @param [in] int nLen ���ݰ��ĳ���
	*
	* @return void 
	*/
	virtual void SetGameStation(void * pBuffer,int nLen);

    /// ��Ϸ��Ϣ
    virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);

    /// ������Ϣ
    virtual int OnUIMessage(TUIMessage* pMessage);

    /// 
    virtual int ResetGameFrame(void);

    /// ��ҽ���
    virtual int GameUserCome(void);

    /// ����뿪
    virtual int GameUserLeft(int nIndex);

    /// ��Ҷ���
    virtual int GameUserOffLine(void);

    // �Ƿ�������Ϸ������Ϸʵ��
    // �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
    virtual bool IsPlayingGame(); 

    /// ��ʱ����Ϣ
    virtual void OnTimer(int nTimerID);

    /// ��״̬
    virtual void SetStationParameter(BYTE bGameStation);

	///ֹͣ��Ϸ
	virtual bool OnGameQuiting();


public:
	/**
	* ���ط����������ļ�
	*
	* return   void
	**/
	void LoadBcf();

	/**
	*��ʼ����Ϸ����
	*
	*return bool ��ʼ����Ϸ�����Ƿ�ɹ�
	*/
	bool InitGameData();

	/**
	* ��������UI��״̬
	*
	* @param BYTE bDeskStation [in] ���λ��
	*
	* @param bool bSetCommon [in] �Ƿ����ù�ͬ����
	*
	*return bool true:�������óɹ���false:��������ʧ��
	*/
	bool  ReSetGameUIStation(BYTE bDeskStation=255,bool bSetCommon = true,bool bAgreeStation = false); 

	/**
	* ��ȡ��Ϸ״̬
	*
	*return BYTE ��Ϸ״̬
	*/
	BYTE  GetStationParameter();

	void OnHitBegin(void);			//��ʼ��ť����

	/**
	* ������������¼�
	*
	* @return void
	*/
	void OnLButtonUpOnCard();

	/**
	* ��ʾ
	*/
	void OnHitCue(bool bShowChongPai = true); 

	/**
	* �ذ�
	*/
	void OnHitChongBai();

	/**
	* ̯��
	*/
	void OnHitTanPai();

	/**
	* ����й� �� ȡ���й� ��ť
	*
	* @param [in] bool bAutoCard; true: �й�;false:ȡ���й�
	*
	* @return void
	*/
	void OnHitOkOrCancleAutoBtn(bool bAutoCard);

	
	BYTE ViewStation(BYTE bDeskStation);

	/**
	* �����Ƿ�����Թ�����
	*
	* @return void
	*/
	void SendIsWatchData();

	/**
	* ������ͨ��Ϣ
	*
	* @param CString strTemp [in] ��Ϣ����
	*
	* @param BYTE bDeskStation [in] ˵�����λ��
	*
	* @return void
	*/
	void InsertNormalMessage(const char *p, ...);

	//�Qϵ�y��
	BOOL ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard);

	static UINT PlaySond(LPVOID lpParam);

	/**
	* ����������Ч
	*
	* @return void
	*/
	void PlayCardShapSound();

	/**
	* ���Ŷ�����Ч
	*
	* @return void
	*/
	void PlayMotionSound();

	/**
	* ֹͣ������Ч
	*
	* @param [in] bool bFlag  �Ƿ���ͣ
	*
	* @return void
	*/
	void StopPlaySound(bool bFlag );

public:		//�������
	/**
	* ��ʾ/���� �ؼ�
	*
	* @param [in] int iControlID �ؼ�ID
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return BOOL �Ƿ����óɹ�
	*/
	BOOL ShowControl(int iControlID,bool bFlag);

	/**
	* ���ÿ���/���� �ؼ���ֻ֧�ְ�ť��
	*
	* @param [in] int iControlID �ؼ�ID
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return BOOL �Ƿ����óɹ�
	*/
	BOOL EnableControl(int iControlID,bool bFlag);

	/**
	* ��ѯ��ť����/���� �ؼ���ֻ֧�ְ�ť��
	*
	* @param [in] int iControlID �ؼ�ID
	*
	* @return BOOL ���ؿؼ�״̬ TRUE:���ã�FALSE:������
	*/
	BOOL GetControlEnable(int iControlID);

	/**
	* ��ѯ��ť��ʾ/���� �ؼ���ֻ֧�ְ�ť��
	*
	* @param [in] int iControlID �ؼ�ID
	*
	* @return BOOL ���ؿؼ�״̬ TRUE:��ʾ��FALSE:����ʾ
	*/
	BOOL GetControlShow(int iControlID);

	/**
	* ��ʾ/���� �ؼ���ֻ֧�ְ�ť��
	*
	* @param [in] int iControlID �ؼ�ID
	*
	* @param [in] bool sFlag ��ʾ��־true:��ʾ��false:����
	*
	* @param [in] bool eFlag ���ñ�־true:���ã�false:����
	*
	* @return void
	*/
	void ShowOrEnableControl(int iControlID,bool sFlag,bool eFlag);

	/**
	* ��ʾ/���� ���ֿؼ�
	*
	* @param [in] int iControlID �ؼ�ID
	*
	* @param [in] __int64 i64Num ��ʾ��ֵ
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @param [in] bool bSign �Ƿ��з���
	*
	* @return void
	*/
	void ShowOrHideNumControl(int iControlID,__int64 i64Num,bool bFlag,bool bSign = false);

		/**
	* �����ⲿ����
	*
	* @param [in] int iControlID �����ؼ�ID
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @param [in] CString StrTemp ����·��
	*
	* @return void
	*/
	void PlayExAnimate(int iControlID, bool bFlag, CString StrTemp);

	/**
	* ��ʾ�����ص���ʱʱ��
	*	
	* @param [in] BYTE bDeskStation ���λ��
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @param [in] int nShowMaxNum ���ֵ
	*
	* @param [in] int nShowMinNum ��СֵĬ��Ϊ0
	*
	* @param [in] int iTimeID ��ʱ��ID Ĭ��ֵΪ 0 ��Ч
	*
	* @return void
	*/
	void ShowOrHideTime(BYTE bDeskStation,bool bFlag,int nShowMaxNum,int nShowMinNum=0,int iTimeID=0);

	/**
	* ��ʾ/���� ׼��ͼ��
	*
	* @param [in] BYTE bDeskStation ���λ��
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return void
	*/
	void SOrHReadyMark(BYTE bDeskStation,bool bFlag);

	/**
	* ��ʾ/���ص׷�
	*
	* @param [in] __int64 i64Num ��ʾ��ֵ
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @param [in] bool bHaveSign �Ƿ��з��� true:�У�false:û��
	*
	* @return void
	*/
	void SOrHBasePoint(__int64 i64Num,bool bFlag,bool bHaveSign=false);

	/**
	* ��ʾ�����������ƿؼ�
	*
	* @param [in] BYTE bDeskStation ���λ��
	*
	* @param [in] BYTE iCard[] ������
	*
	* @param [in] BYTE iCardCount ������
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return void
	*/
	void ShowOrHideHandCard(BYTE bDeskStation,BYTE iCard[],BYTE iCardCount,bool bFlag);

	/**
	* ��ʾ�����ذ��ƺ�����ƺ����ƿؼ�
	*
	* @param [in] BYTE bDeskStation ���λ��
	*
	* @param [in] BYTE iDownCard[] ������
	*
	* @param [in] BYTE iDownCCount ������
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return void
	*/
	void SOrHUAndDCard(BYTE bDeskStation,BYTE iDownCard[],BYTE iDownCCount,bool bFlag);

	/**
	* ��ʾ��������������� ��������ǰ�Ͱ��ƺ�
	*
	* @param [in] BYTE bDeskStation ���λ��
	*
	* @param [in] BYTE bType 1:����ǰ״̬ 2:���ƺ�״̬ 
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return void
	*/
	void SOrHCard(BYTE bDeskStation,BYTE bType,bool bFlag);

	/**
	* ��ʾ������ ̯�Ʊ�־
	*
	* @param [in]  BYTE bDeskStation ���λ��
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return void
	*/
	void SorHTanPaiMark(BYTE bDeskStation,bool bFlag);

	/**
	* ��ʾ������ �������
	*
	* @param [in]  BYTE bDeskStation ���λ��
	*
	* @param [in]  int iCardShape ����
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return void
	*/
	void SorHCardShape(BYTE bDeskStation,int iCardShape,bool bFlag);

	/**
	* ��ʾ������ ��������
	*
	* @param [in] GameEndStruct *pGameEnd ��Ϸ����ͳ�����ݰ�
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return void
	*/
	void SOrHGResultData(GameEndStruct *pGameEnd,bool bFlag);

	/**
	* ��ʾ������ �йܰ�ť
	*
	* @return void
	*/
	void SOrHAutoBtn();

	/**
	* ��ʾ������ �й�ͼƬ
	*
	* @param [in] BYTE bDeskStation ���λ��
	*
	* @param [in] bool bFlag ��ʾ��־true:��ʾ��false:����
	*
	* @return void
	*/
	void ShowOrHideAutoImage(BYTE bDeskStation,bool bFlag);

	/**
	* �ж��ǲ��Ǵ��ڸ����
	*
	* @param [in] BYTE bDeskStation ���λ��
	*
	* @return �������
	*/
	bool IsPlayer(BYTE bDeskStation);

	/**
	* ��ʼ����������
	*
	* @param [bool] bFlag �Բ���ʾ ����
	*
	* @return void
	*/
	void InitialSuper(bool bFlag = false);

	/**
	* ������ó������ð�ť
	*
	* @param void
	*
	* @return void
	*/
	void OnHitResetSuperBut();

	/**
	* �����������Ӯ��ť
	*
	* @param void
	*
	* @return void
	*/
	void OnHitSuperWinBut(int iControlID);

	/**
	* �������������Ч��ť
	*
	* @param void
	*
	* @return void
	*/
	void OnHitSuperOKBut();

	/**
	* ������˰�ť
	*
	* @param void
	*
	* @return void
	*/
	void OnHitSuperBut();
};
