#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "UpgradeMessage.h"
#include "UpGradeLogic.h"
//��Ϣ����
//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_OUT_CARD					103					//���Ƶȴ�ʱ��
#define ID_CALL_SCORE_TIME			107					//�зֶ�ʱ��
#define ID_ROB_NT_TIME				108					//��������ʱ��
#define ID_ADD_DOUBLE_TIME			109					//�Ӱ�
#define ID_SHOW_CARD				102
#define ID_LEAVE_TIME				101

/// Ԥ��������
struct guessCard
{
	BYTE data;     // �ƵĻ�ɫ
	BYTE desk;     // Ԥ��������ڵ�λ�ã����Ϊ255�����������ǳ����Լ�֮����κ���
	guessCard(){data=0;desk=255;}
};

#define GAME_RAND_TIME				2					//����ʱ��
class realData;
//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass//CGameFrameDlg
{
	friend bool peiCardAnalQiangDiZhu(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg);
	friend bool peiCardAnalJiaBei(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg);
	friend bool peiCardFirstOutCard(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg);
	int						m_iVersion;					//�ڲ������汾��
	int						m_iVersion2;				//�ڲ������汾��
	BYTE					m_iPalyCard;				//��Ϸ�˿˸���
	int						m_iPlayCount;				//��Ϸ�˿�����
	BYTE					m_iBackCount;				//������
	int						m_iRoomBasePoint;			//���䱶��
	int						m_iRunPublish;				//���ܿ۷�

	//	BYTE					m_iBeginNt;					//��ʼλ��
	//	BYTE					m_iEndNt;					//����λ�� 
	//=====================ʱ�����Ϣ===================
	BYTE					m_iThinkTime;				//������Ϸ����
	BYTE					m_iGiveBackTime;			//����ʱ��
	BYTE					m_iBeginTime;				//��Ϸ��ʼʱ��
	BYTE					m_iCallScoreTime;				//��Ϸ��ʼʱ��
	BYTE					m_iAddDoubleTime;				//��Ϸ��ʼʱ��

	BYTE					m_iBeenPlayGame;				//����Ϸ����
	//�Լ��˿�����
	BYTE					m_iMeCardCount;				//�Լ��˿˵���Ŀ
	BYTE					m_iMeCardList[45];			//�Լ��˿��б�
	BYTE					m_Card[PLAY_COUNT][45];				//�������е��˿�����
	BYTE					m_CardCount[PLAY_COUNT];				//�������е�����	

	//������Ϣ
	BYTE					m_iNtPeople;				//��������Ϸ��
	BYTE					m_iBackCard[12];				//�����˿�
	//������Ϣ
	BYTE					m_iDeskCardCount[PLAY_COUNT];		//ÿ�������˿˵���Ŀ
	BYTE					m_DeskCard[PLAY_COUNT][45];			//ÿ��������˿�

	//������Ϣ
	BYTE					m_bCurrentOperationStation;		//�F�ڽз���
	BYTE					m_iNowOutPeople;				//���ڳ�����
	BYTE					m_iBaseCount;					//��һ������������Ŀ
	BYTE					m_iFirstOutPeople;				//��һ��������
	BYTE					m_iBigOutPeople;				//��ǰ��������
	

	BYTE					m_iBaseCard[45];				//��һ�������ߵ��˿�

	//������ʾ����
	BYTE					m_bShowLast;					//�Ƿ������ʾ�����˿�
	BYTE					m_bShowIngLast;					//�Ƿ�����ʾ�����˿�
	BYTE					m_iLastCardCount[PLAY_COUNT];		//�����˿˵���Ŀ
	BYTE					m_iLastOutCard[PLAY_COUNT][45];		//���ֵ��˿�
	BYTE					m_iAIStation[PLAY_COUNT];			//�������й�
	////��ʱ������
	BYTE					m_bTimeOutCount;			//��ʱ
	int						m_btAutoTime;
	BYTE					m_bSortCard;				//����ʽ

	DWORD					m_dwCardShape;	  				//����
public:
	CUpGradeGameLogic		m_Logic;					//�����߼�
	bool					m_bAutoCard;				//�Ƿ��й�
	int						m_ResultCallScore;			//�зֽ��
	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();

protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//���غ���
public:
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������ý���
	virtual void ResetGameFrame();
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	//����������Ϸ����
	virtual void ResetGameStation(int iGameStation);
	//�з�
	LRESULT OnCallScore(WPARAM wparam,LPARAM lparam);
	//������
	LRESULT	OnRobNT(WPARAM wparam, LPARAM lparam);
	//�Ӱ�
	LRESULT	OnAddDouble(WPARAM wparam, LPARAM lparam);

	LRESULT OnShowCard(WPARAM wparam, LPARAM lparam);
	//�Ñ�����
	BOOL UserOutCard();

	//���������ܳ���
	bool	DwjAiOutCard();
	//��ȡ��һ�����λ��
	BYTE	GetNextDeskStation(BYTE bDeskStation);
	//��ö��ѵ�λ��
	BYTE	GetFriendStation(BYTE bDeskStation);

	//���������ť
	bool OnControlHitPass(void);

	//ɱ��ʱ��
	void KillAllTimer();
	DECLARE_MESSAGE_MAP()

///һ�¾��ǻ����˲�������

private:
	/// ��ͬ����ƺ�,�Լ����ܱ�֤��Ӯ������µĴ���
	void AI_DaTongHuo();

    ///������ʱ�ڵ�AI����
	void AI_QiangDiZhu();
	/// �����жϱ����Ƿ���������������ǰ��ձȽϴ������£�����Ҫ��������
	/// ��Ϊû��ͬ���Ԯ�����ڵ�����˵��ȡ��ʤ���ǳ���
	bool canQiangDiZhu();

	///�ӱ����ʱ�ڵ�AI����
	void AI_JiaBei();
    /// �����ж��Ƿ񽫷����ӱ�������Լ�ʵ����ǿ��ֻ����Ϊ���᲻��û���ϵ��������ӱ�����
	/// �Եù��ڼ٣������Ҫ�ж��Ƿ��ʸ�ӱ����������
	bool canJiaBei();
    
	/// �Լ�ռȦ֮��ĳ���
	void AI_zhanQuanOutCard();

	/// �Լ�û��ռȦ֮��ĳ���
	void AI_buZhanQuanQutCard();

    int AI_getThinkTime();

	/// ���㵥�ŵ��ŵ����� 10���µ� 
	int computeDanDiaoZhang(BYTE* handCard,int num);
    /// ������ӵ��ŵ����� 7����
	int computeduiDiaoZhang(std::vector<peiOutCardStruct>& myCard);

	/// ���㽫��˳������������ը��������µ��ŵ�����
	int computeDiaoZhang(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard);
    
	/// ���㶨�ŵ�������2����ը��
	int computeDingzhang(std::vector<peiOutCardStruct>& myCard);
private:
	/// ��ʼ���µ���
	void initGuessCard();
	/// ȥ���Ѿ��������ߴ������
	void deleteGuessCard(BYTE *delCard,int num);
    /// ���ƵĴ�����Ҫ���ڼ�¼���Ƶ���Ϣ,�͸������Ƶ���Ϣ
	void outCardResult(OutCardMsg * pOutCardInfo);
	/// ������������ŵ��ƣ�����������Լ���ֱ��ȥ���������ƣ������ڲ����н���������ȷ����������˭��
	void diPai(BackCardExStruct * pBackCard);

	/// ���ڳ�ʼ���ƾ�������״̬�ڵ��״̬��Ϣ
    void initRealData(realData &data);

private:

	
	/// ��¼�Լ����������˳������ƣ���Ҫ���ڷ����Է����е��ƣ����˽�Է����Լ�����ʶ
	std::vector<OutCardStruct>     vChuCard[PLAY_COUNT];   
	/// ���������������е��Ƶ�ȫ���������Ҫ���ڲ���
	std::vector<guessCard>              vLeftCard;
    /// ����Լ���λ��״̬��0Ϊ������1Ϊ�����¼ң�2Ϊ�����ϼ�
	BYTE                                m_index;
	bool                                m_qiangDiZhu;               //���ڼ�¼�Ƿ�߱��ʸ�������
	bool                                m_jiaBei;                   //���ڼ�¼�����Ƿ�߱��ӱ��ʸ�

    OutCardStruct                       m_myOutCard;                //������Ҫ��һ�������Ǳ���AI����Ľ����Ҫ�����ƴ��
    int                                 m_diaoZhang;                // ���ڼ�¼Ŀǰ��ɵ��Ƶ��е����������ٵģ���ȷ���������ŵ����Ʒ���
};


/// ��õ��Ƶ���ʽ���ж�

/// �ж���õ����Ƿ�߱����������ʸ�
bool peiCardAnalQiangDiZhu(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg);
/// �ж���õ����Ƿ�߱��ӱ��������ʸ�
bool peiCardAnalJiaBei(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg);
/// �ж���õ����Ƿ�����Ҫ��,���������Ҫ������Ӧ�Ĺ���������Ǹ���
bool peiCardFirstOutCard(BYTE* handCard,int num,std::vector<peiOutCardStruct>& myCard,CClientGameDlg *pClientGameDlg);