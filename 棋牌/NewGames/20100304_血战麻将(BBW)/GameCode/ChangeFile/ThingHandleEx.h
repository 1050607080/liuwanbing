#pragma once

#include "../GameMessage/ThingHandle.h"
///�¼������ࣨ���أ�
class ThingHandleEx:public ThingHandle
{
public:
	ThingHandleEx(void);
	~ThingHandleEx(void);
	
public:	
	
public:
	
///////�������ݴ���//////////////////////////////////////////////////////
	//���������¼�
	virtual bool GetGameStaton(BYTE bDeskStation, UINT uSocketID, bool bWatchUser,BYTE GameStation);
	// ��Ϸ���ݰ�������
	virtual int HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser);

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
	virtual int ReceiveUserPengCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserGangCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserTingCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
	// �û�������Ϣ
	virtual int ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);
    // �û���ȱ��Ϣ
    virtual int ReceiveUserDingQue(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser);

	///�����������һ������
	virtual void CheckNextAction();
	//��ȡ�������ҽ������:�ж��������ҵ�ʱ��
	virtual BYTE GetCanHuUser(BYTE outst,bool hu[],BYTE hust);
	///��ȡ�¼�λ��
	virtual BYTE GetNextStation(BYTE station,bool shun=false);
	///������ʱ����
	virtual void ClearTempData();

	///����Ƿ���Խ�����һ������
	virtual bool IsCanHandle();


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
	///��2��ɫ��ȷ��ׯ�Һ����Ʒ��򣨵�����ׯ�ң���㷽��
	virtual void ExecuteTwoSeziNtAndDir(BYTE &nextID,BYTE &time);	
	///������ɫ��ȷ��ׯ��
	virtual void ExecuteTurnSeziNt(BYTE &nextID,BYTE &time);	
	///��2��ɫ��ȷ�����Ʒ��򣨵�����ȷ������
	virtual void ExecuteTwoSeziDir(BYTE &nextID,BYTE &time);	
	///��2��ɫ��ȷ�����Ʒ�������ƶ����������ͷ���С�������
	virtual void ExecuteTwoSeziDirGp(BYTE &nextID,BYTE &time);	
	///��2��ɫ��ȷ�����ƶ����������Ͷ�����
	virtual void ExecuteTwoSeziGp(BYTE &nextID,BYTE &time);	
	///��ע�¼���ȷ��ׯ�Һ󣬿���ͨ��ѡ���ע������Ӯ��
	virtual void ExecuteAddNote(BYTE &nextID,BYTE &time);	
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
    ///��ȱ�¼�
    virtual void ExecuteDingQue(BYTE &nextID,BYTE &time);	
	///��ʼ����֪ͨ�¼�
	virtual void ExecuteBeginOut(BYTE &nextID,BYTE &time);	
	///ץ���¼�
	virtual void ExecuteZhuaPai(BYTE &nextID,BYTE &time);	
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
	virtual BYTE GetPai(int station,bool head,BYTE type=0,BYTE *pIndex=NULL);
	///������station��ǰ������Щ����
	virtual bool CheckAction(int station,BYTE pai,BYTE type);
	///�����Զ�������
	virtual void NetCutAutoHandle();
	//�ſ����Զ�����
	virtual void AutoHandle();

	///�Զ�������ƻ���ץ��
	void AutoOutOrGet();

//////�����������///////////////////////////////////////////////////////////

	///�����¼�
	virtual void HandleChiPai(BYTE &nextID,BYTE &time);	
	///�����¼�
	virtual void HandlePengPai(BYTE &nextID,BYTE &time);	
	///���¼�
	virtual void HandleGangPai(BYTE &nextID,BYTE &time);	
	///���¼�
	virtual void HandleTingPai(BYTE &nextID,BYTE &time);	
	///���¼�
	virtual void HandleHuPai(BYTE &nextID,BYTE &time);	
	///���
	virtual void HandleCountFen(BYTE &nextID,BYTE &time);	
	/////���������¼�
	//virtual void HandleEnd(BYTE &nextID,BYTE &time);	
	/////ǿ�˽����¼�
	//virtual void UnnormalEnd(BYTE &nextID,BYTE &time);	

    /// �����ʱ����Ϣ
    virtual void OnGameTimer(UINT uTimerID);


    void CountFenAndRecord(void);

private:
    /**
     * @brief ���õ�ǰ״̬
     * @param[in]       curState            Ҫ���ó�ʲô״̬
     * @param[in]       unLastingTime       ״̬����ʱ��
     * @param[in]       unWaitingTime       ״̬��ʼǰ�ĵȴ�
     * @param[in]       bNotify             �Ƿ���Ϣ֪ͨ�ͻ���
     */
    void SetGameCurState(MJTHINGS curState, BYTE ucCurChair, unsigned int unLastingTime, unsigned int unWaitingTime = 0,
        bool bNotify = true);

    void OnStateTimer();    
public:
    void StartStateTimer();
    void EndStateTimer();

	//�¼���¼
	void LogThing(BYTE thingId,BYTE pai=255,BYTE st1=255,BYTE st2=255,BYTE data=255);

	/// ���õ�ǰ���ڷ������¼�
	virtual void SetDoingThing(BYTE station,BYTE thing);


private:
    struct StateHandler
    {
        MJTHINGS                thingDoing;           // ��ǰ״̬
        BYTE                    ucCurChair;           // ��ǰ��˭����        
        unsigned int            unStartTime;          // ��ʼʱ��(ms)
        unsigned int            unEndTime;            // ����ʱ��(ms)        
        unsigned int            unWaitingTime;        // �ȴ�ʱ��(ms)
        bool                    bNotify;              // �Ƿ�֪ͨ�ͻ���
    };
    StateHandler                m_curState;
    unsigned int                m_unCurStateTime;
};

//ȫ�ֶ���
//extern ThingHandleEx g_ThingHandle;