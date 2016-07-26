
#pragma once

#include "..\ChangeFile\CGameDataEx.h"
#include "..\ChangeFile\CUserDataEx.h"

#include "..\ChangeFile\CCheckCPGActionEx.h"
#include "..\ChangeFile\CCheckHuPaiEx.h"
#include "..\ChangeFile\CCountFenEx.h"

class CTestDlg;

#define TIME_CHECK_GAME_MSG         30              // �����Ϸ��Ϣ���ݰ�����ʱû���յ����ݰ�����Ϊ�ǿ�ס�ˣ�ϵͳ�Զ�����
#define TIME_WAIT_MSG				31              // �����¼�����Ӧ���ǳ�ʱ�Զ�����ϵͳ�Զ�����
#define TIME_DEFAULT_HANDLE         60000			// Ĭ��ʱ��60��

//�����������߼���

class CSrvLogic
{
public:
	CSrvLogic(void);
	~CSrvLogic(void);
	
public:	
	CTestDlg *pDesk;//���ࣨ���ӣ�


public:
	//��������ָ��
	void SetDeskPoint(CTestDlg *desk);

///////�������ݴ���//////////////////////////////////////////////////////

	//���������¼�
	virtual bool GetGameStaton(BYTE bDeskStation, bool bWatchUser,BYTE GameStation);
	// ��Ϸ���ݰ�������
	virtual int HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser=false);
	// �յ��û�ɫ�Ӷ���������Ϣ
	virtual int ReceiveSeziFinish(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �յ��й���Ϣ
	virtual int ReceiveUserTuoGuan(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�����
	virtual int ReceiveUserOutCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �յ��û�pass ��Ϣ
	virtual int ReceiveUserPassCheck(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	//�յ��û�ͬ����Ϸ��Ϣ
	virtual int ReceiveUserAgreeGame(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	//���������뿪
	virtual int ReceiveUserHaveThing(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	//ͬ���û��뿪
	virtual int ReceiveArgeeUserLeft(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserChiCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserPengCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserGangCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserTingCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	///�յ������ͻ�����������Ϣ
	virtual void ReceiveSuperChangePai(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);

	///�����������һ������
	virtual void CheckNextAction();
	///����Ƿ���Խ�����һ������
	virtual bool IsCanHandle(BYTE &mx);

////�����¼�����///////////////////////////////////////////////////////////////////////////////

	//�����¼�,typeҪ������¼�
	virtual void ApplyAction(int type,int time);
	//��Ϸ�¼����������ģ�ȷ����һ���¼���ʲô�������¼�ֵ,type������¼�����
	virtual void GameCPU(int type);
	///��ʼ�¼�����
	virtual void ExecuteGameBegin(BYTE &nextID,BYTE &time);
	///�Ե�һ�����Ϊׯ��
	virtual void ExecuteDongNt(BYTE &nextID,BYTE &time);
	///��2��ɫ��ȷ��ׯ�ң������ͣ�
	virtual void ExecuteTwoSeziNt(BYTE &nextID,BYTE &time);	
	///��2��ɫ��ȷ��ׯ�Һ����ƶ�����������ׯ�ң���������
	virtual void ExecuteTwoSeziNtAndGp(BYTE &nextID,BYTE &time);		
	///��2��ɫ��ȷ�����Ʒ��򣨵�����ȷ������
	virtual void ExecuteTwoSeziDir(BYTE &nextID,BYTE &time);	
	///��2��ɫ��ȷ�����Ʒ�������ƶ����������ͷ���С�������
	virtual void ExecuteTwoSeziDirGp(BYTE &nextID,BYTE &time);	
	///��2��ɫ��ȷ�����ƶ����������Ͷ�����
	virtual void ExecuteTwoSeziGp(BYTE &nextID,BYTE &time);	
	///��2��ɫ��ȷ��ׯ��+���Ʒ���+���ƶ�����������ׯ�ң���㷽��С�������
	virtual void ExecuteTwoSeziNtDirGp(BYTE &nextID,BYTE &time);	
	///����
	virtual void ExecuteSendPai(BYTE &nextID,BYTE &time);	
	///��2��ɫ�Ӷ����¼�
	virtual void ExecuteTwoSeziMakeJing(BYTE &nextID,BYTE &time);	
	///������Ҳ����¼�
	virtual void ExecuteAllBuHua(BYTE &nextID,BYTE &time);	
	///������Ҳ����¼�
	virtual void ExecuteOneBuHua(BYTE &nextID,BYTE &time);	
	///��ʼ����֪ͨ�¼�
	virtual void ExecuteBeginOut(BYTE &nextID,BYTE &time);	
	///ץ���¼�
	virtual void ExecuteZhuaPai(BYTE &nextID,BYTE &time);	
	//�����¼�
	virtual void ExecuteChiPai(BYTE &nextID,BYTE &time);
	//�����¼�
	virtual void ExecutePengPai(BYTE &nextID,BYTE &time);
	//�����¼�
	virtual void ExecuteGangPai(BYTE &nextID,BYTE &time);
	//�����¼�
	virtual void ExecuteHuPai(BYTE &nextID,BYTE &time);
	///����¼�
	virtual void ExecuteCountFen(BYTE &nextID,BYTE &time);
	///������Ϸ�����¼�
	virtual void ExecuteNormalEnd(BYTE &nextID,BYTE &time);
	///�쳣��Ϸ�����¼��¼�
	virtual void ExecuteUnNormalEnd(BYTE &nextID,BYTE &time);

		//���ƺ���
	virtual void DisPatchCard();
	///������ת����������,�������������е�����ת����ÿ����ҵ�����������
	virtual void ChangeMenPai();
	///������ץһ����
	virtual BYTE GetPai(BYTE &index,int station,bool head,BYTE type=0);
	///������station��ǰ������Щ����
	virtual bool CheckAction(int station,BYTE pai,BYTE type);
	//�ſ����Զ�����
	virtual void AutoHandle(bool bNetCut=false);
	///�Զ�������ƻ���ץ��
	virtual void AutoOutOrGet();

////////////////////��������///////////////////////////////////////////////

    /// ��Ҷ���
    virtual void UserNetCut(BYTE bDeskStation);
    /// �������
    virtual void UserLeftDesk(BYTE bDeskStation);
	/// �������
	virtual void UserSitDesk(BYTE bDeskStation);
    /// �������
    virtual void UserReCome(BYTE bDeskStation);
	/// �����ʱ����Ϣ
	virtual void OnGameTimer(UINT uTimerID);
	/// ��Ϸ����Ҫɾ�����м�ʱ��
	virtual void KillAllTimer();

	/// ���õ�ǰ���ڷ������¼�
	virtual void SetDoingThing(BYTE thingId,BYTE st1=255,BYTE st2=255,BYTE pai=255,BYTE data=255);
	/// ��ʱ����
	virtual void OutTimeHandle();
	/// ������Ϸ״̬
	virtual void SetGameState(BYTE state);

	//�¼���¼��ֻ��¼���10���¼�
	void RecordThing(BYTE thingId,BYTE st1=255,BYTE st2=255,BYTE pai=255,BYTE data=255);


	//��������ҷ�����Ϸ��Ϣ
	void SendGameStation(BYTE bDeskStation, bool bWatchUser, void * pData, UINT uSize);
	//��������ҷ�����Ϸ��Ϣ
	void SendGameData(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	//��������ҷ�����Ϸ��Ϣ
	void SendGameData(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	//��������ҷ�����Ϸ��Ϣ
	void SendAllGameData( BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	//��������ҷ�����Ϸ��Ϣ
	void SendAllGameData( void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);


public:
	//��������
	CGameDataEx m_GameData;//��Ϸ����
	CUserDataEx m_UserData;//�������

	CCheckCPGActionEx	m_CPGAction;//�����ܼ�ⷽ��
	CCheckHuPaiEx		m_CheckHuPai;//���Ƽ�ⷽ��
	CCountFenEx			m_CountFen;//���

};
