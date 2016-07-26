#pragma once
#include "../ChangeFile/UpgradeMessageEx.h"

///��Ϸ���ݹ�����:�������ݣ���Ϸ�������ݵȣ������ݽ��в����޸ĵȶ���
class GameData
{
public:
	GameData(void);
	~GameData(void);
	
public:	        
	/// �齫��������
	MjRuleSet				m_mjRule;					
	/// �齫��������
	ActionRuleSet			m_mjAction;               
	/// �齫�������
	CountFenRuleSet			m_mjFenRule;				
	/// �齫��������
	HuPaiRuleSet			m_mjHuRule;				
	/// �齫��������
	HuPaiTypeSet			m_mjHuType;				
	///��¼��Ϸ���е������¼���ץ�ƣ����ƣ������ܺ��������ȣ����һ��Ԫ��299��¼�¼�������
	BYTE					m_byThingRecord[300];

	/// ÿ���¼��ĺ���¼������100���¼���
	BYTE					m_byThingNext[100];

	bool					m_bShowUserId;				//�Ƿ���ʾ������ҵ�id

	////////////�������ݶ���/////////////////////////////////////////////////////////
	///�ϴη������¼���
	BYTE					m_byThingLast;		
	///���ڷ������¼���
	BYTE					m_byThingDoing;  
	///�Ƿ�����뿪��Ϸ
	bool					m_bCanLeaveGame;
	///��ִ���¼�
	BYTE					m_byNextAction;
	///����Ҫ�۵ķ���
	int						uRunPublish;					
	///����ı���
	int						basepoint;	
	///Ҫ������¼�id
	BYTE					ApplyThingID;

	///�������¼�,��ʱ����
	tagChiPaiEx				temp_ChiPai;
	///�������¼�,��ʱ����
	tagPengPaiEx			temp_PengPai;
	///�������¼�,��ʱ����
	tagGangPaiEx			temp_GangPai;
	////�������¼�,��ʱ����

	////////////�¼����ݶ�����///////////////////////////////////////////////////////

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
	///�����¼�
	tagTiaoPaiEx			T_TiaoPai;
	///��ɫ��2��ɫ�Ӷ������¼�
	tagTwoSeziJingEx		T_TwoSeziJing;
	///��ɫ��1��ɫ�Ӷ������¼�
	tagOneSeziJinEx			T_OneSeziJin;
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
	tagKanPaiEx				T_KanPai;
	///ɨ���¼�
	tagSaoHuEx				T_SaoHu;
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
	tagUnnormalEndHandleEx   T_UnnormalEndHandle;

	BYTE       m_byThingLog[10][5];//�¼���¼ֻ��¼��10���¼���0�¼��ţ�1�����ֵ��2���λ��1,3���λ��2��4��������
	bool					m_bGangState[4];//��¼����״̬��ӵ�ڸܿ��͸ܺ���

public:

	///���������¼��ĺ���¼�
	virtual void SetThingNext();
	///����ĳ���¼��ĺ���¼�
	virtual void SetThingNext(BYTE id);
	//virtual ��ʼ������
	virtual void InitData();
	///��ʼ������
	virtual void LoadIni();

};