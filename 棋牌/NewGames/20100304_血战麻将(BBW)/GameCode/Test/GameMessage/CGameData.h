#pragma once
//#include "../ChangeFile/UpgradeMessageEx.h"
#include "CLogicBase.h"
#include "cstring.h"

///��Ϸ���ݹ�����:�������ݣ���Ϸ�������ݵȣ������ݽ��в����޸ĵȶ���
class CGameData
{
public:
	CGameData(void);
	~CGameData(void);
	
public:	        
	/// �齫��������
	MjRuleSet				m_mjRule;			
	/// �齫��������
	ActionRuleSet			m_mjAction;          				
	/// �齫��������
	HuPaiTypeSet			m_mjHuType;		
	/// ÿ���¼��ĺ���¼������100���¼���
	BYTE					m_byThingNext[100];
	//�Ƿ���ʾ������ҵ�id
	bool					m_bShowUserId;
	////////////�������ݶ���/////////////////////////////////////////////////////////	
	///���ڷ������¼���
	BYTE					m_byThingDoing;  
	///��Ϸ��ǰ״̬������ǰ״̬������״̬������״̬������״̬����Ϸ����״̬������״̬
	BYTE					m_byGameState; 

	///����Ҫ�۵ķ���
	int						uRunPublish;				
	///����ı���
	int						basepoint;
	///�ȴ�ִ�е��¼�id
	BYTE					ApplyThingID;
	//�¼���¼ֻ��¼��10���¼���0�¼��ţ�1�����ֵ��2���λ��1,3���λ��2��4��������
	BYTE					m_byThingLog[10][5];
	
	
	///�������¼�,��ʱ����
	tagChiPaiEx				temp_ChiPai;
	///�������¼�,��ʱ����
	tagPengPaiEx			temp_PengPai;
	///�������¼�,��ʱ����
	tagGangPaiEx			temp_GangPai;
	////�������¼�,��ʱ����


	//////////// �¼����ݶ����� ///////////////////////////////////////////////////////

	///��Ϸ��ʼ�¼�
	tagBeginEx				T_Begin;
	///�Զ�Ϊׯ�¼�
	tagDongNtEx				T_DongNt;
	///��2��ɫ�ӵĵ�����Ϊׯ�¼�
	tagTwoSeziNtEx			T_TwoSeziNt;
	///��2��ɫ��ȷ��ׯ�Һ����Ƶ�λ���¼�
	tagTwoSeziNtAndGetPaiEx	T_TwoSeziNtAndGetPai;
	///������2��ɫ��ȷ��ׯ��
	tagTurnSeziNtEx			T_TurnSeziNt;
	///��2��ɫ��ȷ������λ���¼�
	tagTwoSeziDirEx			T_TwoSeziDir;
	///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
	tagTwoSeziDirAndGetPaiEx	T_TwoSeziDirAndGetPai;
	///��2��ɫ��ȷ�����ƶ����¼�
	tagTwoSeziGetPaiEx		T_TwoSeziGetPai;
	///�����¼�
	tagSendPaiEx			T_SendPai;
	///��ɫ��2��ɫ�Ӷ������¼�
	tagTwoSeziJingEx		T_TwoSeziJing;
	///������Ҳ����¼�
	tagAllBuHuaEx			T_AllBuHua;
	///������Ҳ����¼�
	tagOneBuHuaEx			T_OneBuHua;
	///��ʼ����֪ͨ�¼�
	tagBeginOutPaiEx		T_BeginOutPai;
	///�����¼�
	tagOutPaiEx				T_OutPai;
	///ץ�����¼�
	tagZhuaPaiEx			T_ZhuaPai;
	///�����ܺ���֪ͨ�¼����¼�
	tagCPGNotifyEx			T_CPGNotify[4];
	///����Լ��ĳ��������ݣ��ͻ���ʹ�ã�
	tagCPGNotifyEx			T_Me_CPGNotify;
	///�������¼�
	tagChiPaiEx				T_ChiPai;
	///�������¼�
	tagPengPaiEx			T_PengPai;
	///�������¼�
	tagGangPaiEx			T_GangPai;
	///�������¼�
	tagTingPaiEx			T_TingPai;
	///�������¼�
	tagHuPaiEx				T_HuPai;
	///����¼�
	tagCountFenEx			T_CountFen;
	///��Ϸ���������¼�
	tagEndHandleEx			T_EndHandle;
	///���������������¼�
	tagUnnormalEndHandleEx  T_UnnormalEndHandle;
	
	BYTE					m_byApplyMaxAction;//��ҵ�ǰ���������¼�
	///��ִ���¼�
	BYTE					m_byNextAction;


	///�齫ʱ������
	MJ_SettingEx    m_MjSet;
	///�齫ʱ��ȴ���ʱ�䵽�˷�������Ϊ����
	MJ_WaitTimeEx   m_MjWait;
	
public:
	
	///���������¼��ĺ���¼�
	virtual void SetThingNext();
	///����ĳ���¼��ĺ���¼�
	virtual void SetThingNext(BYTE id);
	//virtual ��ʼ������
	virtual void InitData();
	///��ʼ������
	virtual void LoadIni();
	//����������
	virtual BYTE LoadPaiData(MjMenPaiData & menpai);
	
	
//////////////////��Ҳ���////////////////////////////////////////

	///�յ���ҵĶ�������
	virtual void AcceptAction(BYTE station,BYTE type);
	///��ȡ��������
	virtual BYTE GetMaxRequet(BYTE station);
	///ָ�������Ƿ�ȫ����Ӧ
	bool IsNotifyAllReply(BYTE station,BYTE request);
	/// ���ó�ʱ����
	virtual void SetOutTime(BYTE station,BYTE time,BYTE thing,bool effect);
	/// �ָ���ʱ����
	virtual void ReSetOutTime();
	
///////////////�ͻ��˳����ܲ���//////////////////////////////////////////////
	/// �����ҵĳ�������
	virtual int GetUserChiCount(BYTE station,BYTE handpai[]);
	/// �����ҵĳ�������
	virtual int GetUserGangCount(BYTE station,BYTE handpai[]);


	int GetCanGangCount(BYTE station);//����ܸ��Ƶ�����
	int GetCanChiCount(BYTE station);//����ܳ��Ƶ�����


	///������ʱ����
	void ClearTempData();

};

