#pragma once
#include "../ChangeFile/UpgradeMessageEx.h"

///������ݹ�����
class UserData
{
public:
	UserData(void);
	~UserData(void);
	
public:	
	/// ��������
	MjMenPaiData    m_MenPai;      

	///���Ʋ�����20�ţ�0Ϊ������־
	BYTE m_byArHandPai[4][HAND_CARD_NUM];
	///���Ʋ�����160�ţ�0Ϊ������־
	BYTE m_byArOutPai[4][OUT_CARD_NUM];
	///���Ʋ�����20�ţ�0Ϊ������־
	BYTE m_byArHuaPai[4][HUA_CARD_NUM];
	///���Ʋ�����40��
	BYTE m_iArMenPai[4][MEN_CARD_NUM];
	///ת�������������
	BYTE m_byMenPaiData[4][MEN_CARD_NUM/2][2];

	///��ϵ����
	BYTE		m_byGuanXi[4][4];
	///������
	GCPStructEx	m_UserGCPData[PLAY_COUNT][5];
	///�Ƿ���õ���
	bool		m_bEnableCard[PLAY_COUNT][HAND_CARD_NUM]; 

	///ׯ��λ��
	BYTE		m_byNtStation;	
	///��ǰ��ҵ��߼�λ�ã��ͻ���ʹ�ã�
	BYTE        m_byStation;
	///���������
	BYTE        m_byLastOutPai;
	///���ץ������
	BYTE		m_byLastZhuaPai[4];
	///��ǰ������ץ��
	BYTE        m_byMeZhuaPai;
	///��ǰ����λ��
	BYTE		m_byNowOutStation;
	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	bool		m_bZhuaPaiDir;
	///�ϴγ���λ��
	BYTE		m_byLastOutStation;
	///�������
	BYTE        m_byBaoPaiUser;
	///��ǰ������Ҵ������
	BYTE		m_byOtherOutPai;
	///ͣ�ƻ������������ܴ������
	BYTE       m_byCanOutCard[4][HAND_CARD_NUM];
	///��Ϸ�Ѿ����еľ�����ǿ�ˣ���ȫ�˳�����ɢ�����¿�ʼ���㣩
	BYTE		byPlayCount;
	///Ȧ��
	BYTE		m_byQuanFeng;
	///�ŷ�
	BYTE		m_byMenFeng[4];

	//����Ƿ񷵻���ɫ�ӽ�����Ϣ
	bool		m_bSiziFinish[4];
	//�Ƿ���ȴ�ɫ�Ӷ�������״̬
	bool		m_bWaitSezi;
	///����Ƿ��Ѿ�����
	bool        m_bFoceLeave[4];
	///����뿪������
	BYTE		m_byFoceLeavCount;
	///����Ƿ�����
	bool		m_bTing[4];
	///��ǰ����Ƿ�����
	bool		m_bisTing;
	//����Ƿ��й�
	bool		m_bTuoGuan[4];
	///����Ƿ�Ӯ��
	bool		m_bWinner[4];
	///�Ƿ�һ�ڶ���
	bool		m_bIsDuoXiang;
	//�Ƿ��ҳ���
	bool		m_bMeOutPai;
	//��¼����Ƿ��������
	bool		m_bHaveOut[4];

	///��ǰѡ��������
	BYTE        m_bySelectIndex;
	///��������
	JingStruct  m_StrJing;
	//ͬ������뿪������
	BYTE		m_byAgreeLeaveNum;


	///�ؼ�����
	bool                m_bIsBoy[4];
	
	///��������
	SuperStruct			m_SetData[4];

	///������һ�����ƣ�����ʹ�ã�
	BYTE				m_bySetMoPai[4];

	BYTE				m_byApplyMaxAction;//��ҵ�ǰ���������¼�
	BYTE				m_byCanDoMaxAction;//���μ����������������
public:
	
	//////////////////���Ʋ���/////////////////////////////////////

	///����Ƿ����ָ��������
	virtual bool IsHaveAHandPai(BYTE station,BYTE pai);
	///���������ĳ���Ƶĸ���
	virtual int GetAHandPaiCount(BYTE station,BYTE pai);
	///������Ƶ�����
	virtual int GetHandPaiCount(BYTE station);
	///������ҵ�����
	virtual int CopyHandPai(BYTE pai[][HAND_CARD_NUM],BYTE station,bool show);
	///����������ĳ����ҵ�����
	virtual int CopyOneUserHandPai(BYTE pai[HAND_CARD_NUM] ,BYTE station);
	///��������small�Ƿ��С������
	virtual void SortHandPai(BYTE station,bool big);
	///���һ���Ƶ�������
	virtual void AddToHandPai(BYTE station,BYTE pai);
	///������������
	virtual void SetHandPaiData(BYTE station,BYTE pai[]);
	///ɾ��һ��ָ��������
	virtual void DelleteAHandPai(BYTE station,BYTE pai);
	///���������Ʊ�
	virtual void SetHandPaiBack(BYTE station,BYTE num);
	///������һ������
	virtual BYTE GetLastHandPai(BYTE station);
	///���ѡ�е�һ������
	virtual BYTE GetSelectHandPai(BYTE station);
	///�Ƿ�������
	virtual bool IsOutPaiPeople(BYTE station);
	///����������Ƿ���ڻ���
	virtual bool IsHuaPaiInHand(BYTE station);
	///�������еĻ����Ƶ�����������
	virtual int MoveHuaPaiFormHand(BYTE station);
	///ͨ��������ȡһ������
	virtual BYTE GetHandPaiFromIndex(BYTE station,int index);
	///ͨ����������һ������
	virtual bool SetHandPaiFromIndex(BYTE station,int index,BYTE pai);
	///�����Ƿ�����
	virtual bool IsHaveHandPaiFromIndex(BYTE station,int index);
	//////////////////�������Ʋ���/////////////////////////////////////

	///��ȡ��Ҹ��Ƶĸ���(���ܣ����ܣ�����)
	virtual int GetUserGangNum(BYTE station,BYTE &an,BYTE &ming , BYTE &bu);
	///��ҳ��ƴ���
	virtual int GetUserChiNum(BYTE station);
	///��Ҹ��ƴ���
	virtual int GetUserPengNum(BYTE station);
	///��Ҹ���ĳ����
	virtual bool IsUserHaveGangPai(BYTE station,BYTE pai,BYTE &type);
	///����Ƿ�����ĳ����
	virtual bool IsUserHavePengPai(BYTE station,BYTE pai);
	///����Ƿ�Թ�ĳ����
	virtual bool IsUserHaveChiPai(BYTE station,BYTE pai);
	///���һ�����ݵ��ܳ���������
	virtual void AddToGCP(BYTE station,GCPStructEx *gcpData);
	///ɾ���ճ���������ָ����������
	virtual void DelleteAGCPData(BYTE station,BYTE type ,BYTE pai);
	///�����ܳ��������е�������
	virtual int CopyGCPData(BYTE station,GCPStructEx gcpData[]);
	///���ó��������е�������
	virtual void SetGCPData(BYTE station,GCPStructEx gcpData[]);
	///��ȡ�ܳ����Ľڵ����
	virtual int GetGCPCount(BYTE station);
	///��������ȡ�ܳ�������ĳ���Ƶ�����
	virtual int GetOnePaiGCPCount(BYTE station,BYTE pai);
	///����ܳ�������
	virtual void ShortGCP(BYTE station);
	///��������ʼ���ܳ�������
	virtual void InitGCP(BYTE station);

	///////////////////���Ʋ���///////////////////////////////////////////////////////

	///���һ���Ƶ�����������
	virtual void AddToOutPai(BYTE station,BYTE pai);
	///ɾ�����һ�ų���
	virtual void DelleteLastOutPai(BYTE station);
	///���ó�������
	virtual void SetOutPaiData(BYTE station,BYTE pai[]);
	///������ҵĳ���
	virtual int CopyOutPai(BYTE station,BYTE pai[]);
	///��ȡ��ҵĳ�������
	virtual int GetOutPaiCount(BYTE station);
	///��������ȡ���ĳ���Ƶĳ�������
	virtual int GetOneOutPaiCount(BYTE station,BYTE pai);

	//////////////////���Ʋ���///////////////////////////////////////////////////////

	///���һ�Ż��Ƶ�����������
	virtual void AddToHuaPai(BYTE station,BYTE pai);
	///���û�������
	virtual void SetHuaPaiData(BYTE station,BYTE pai[]);
	///������ҵĻ���
	virtual int CopyHuaPai(BYTE station,BYTE pai[]);

	///����Ƿ���
	virtual bool CheckIsHuaPai(BYTE pai);

	///////////////////�����Ʋ���///////////////////////////////////////////////////////

	///������������
	virtual void SetMenPaiData(BYTE station,BYTE pai[]);
	///������ҵ�����
	virtual int CopyMenPai(BYTE station,BYTE pai[]);
	///����������ת��
	virtual void ChangeMenPai(BYTE station);
	///��������ȡĳ��ҵ���������
	virtual int GetMenPaiCount(BYTE station);

	///////////////////�������//////////////////////////////////////////////
	/////���pai�Ƿ����
	//virtual bool CheckIsCaiShen(BYTE pai);
	/////��ղ���ṹ
	//virtual void InitCaiShen();

	/////////////////�����ͻ��������Ʋ���///////////////////////
	///����������
	virtual void ChangeAllHandPai(BYTE station,BYTE pai[],BYTE count);
	///��ָ������
	virtual void ChangeSomeHandPai(BYTE station,BYTE pai[],BYTE handpai[],BYTE count);
	///�����Լ�����һ����
	virtual void SetMeNextPai(BYTE station,BYTE pai);
	///������ǽ����һ����
	virtual void SetGameNextPai(BYTE pai);

/////////////////////////////////////////////////////

	///��ȡ���������Ч������
	virtual int GetPaiNum(BYTE station,BYTE pai[],BYTE count);
	///��ʼ������
	virtual void InitData();
	
};
