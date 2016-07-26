#pragma once
#include "..\ChangeFile\CGameDataEx.h"
#include "..\ChangeFile\CUserDataEx.h"
#include "CLogicBase.h"

class CSrvLogic;

///���Ƽ����
class CCheckHuPai
{
public:
	CCheckHuPai(void);
	~CCheckHuPai(void);
	
public:	
	CSrvLogic  *pDesk;//���ࣨ���ӣ�

	///��ǰ�����ҵ�λ��
	BYTE            m_byStation;
	///��ǰ���Ƶ�λ��
	BYTE            m_byOutStation;
	///�������ṹ����
	PingHuStruct	hupaistruct;
	///���Ƽ����ʱ����
	CheckHuStruct	HuTempData;		
	///���Ƽ����ʱ����(�޲���)
	CheckHuStruct	NoJingHuTempData;		
	///��������
	BYTE			m_byJingNum;
	///��������
	BYTE			m_byHuType[MAX_HUPAI_TYPE];
	//��ǰ�����ֵ
	int				m_iMaxFan;
	//�����������ڵ���
	BYTE			m_byLastPai;
	//���Ʋ�����20�ţ�0Ϊ������־
	BYTE			m_byArHandPai[HAND_CARD_NUM];
	//�Ƿ����������ж�
	bool			m_bZimo;


	///��������
	BYTE		   m_byShunNum;			//˳�ӵĸ���
	BYTE           m_byShunData[4];		//˳�ӵ�����
	BYTE		   m_byKeziNum;			//���ӵĸ���
	BYTE           m_byKeziData[4];		//���ӵ�����
	BYTE		   m_byGangNum;			//���Ƶĸ���
	BYTE           m_byGangData[4][2];	//���Ƶ�����
	BYTE		   m_byJiang;			//��������

	BYTE		   m_byChiNum;			//���Ƶĸ���
	BYTE		   m_byPengNum;			//���Ƶĸ���

public:
	//��������ָ��
	void SetDeskPoint(CSrvLogic *desk);

	///����ܷ�����
	virtual bool CanTing(BYTE station,tagCPGNotifyEx &ResultData);
	///����ܷ�����ƣ��Ժ�����
	virtual bool ChiTing(BYTE station,tagCPGNotifyEx &cpg,BYTE CanOut[]);
	///����ܷ������ƣ���������
	virtual bool PengTing(BYTE station,tagCPGNotifyEx &cpg,BYTE CanOut[]);
	///���ܺ��ܷ񱣴�����״̬
	virtual bool GangTing(BYTE station,tagCPGNotifyEx &cpg ,BYTE ps = 255,bool zimo=false);
	///�������Ƽ��
	virtual bool CheckTing(BYTE pai[]);
	///������ܺ�ָ�����
	virtual void ResetHandPai(BYTE station,BYTE pai[]);
	//��ϸ����������
	virtual bool CanHu(BYTE station,BYTE lastpai,tagCPGNotifyEx &ResultData,bool zimo,BYTE dianpao);

	///ת���������ݵĴ洢��ʽ��Ϊ�˷�����Ƽ��
	virtual void ChangeHandPaiData(BYTE handpai[],BYTE pai);

	///ƽ�����
	virtual bool CheckPingHu(BYTE lastpai=255,bool zimo=false,bool jing=false,bool CheckTing=false);

	///��ȡ�����������
	virtual bool  GetMaxCombination(bool zimo,bool jing,BYTE lastpai = 255);

	///ƽ������
	virtual bool MakePingHu(CheckHuStruct &PaiData,PingHuStruct &hupaistruct,int csnum=0);

	///�ָ�����,�Ѽ������ƻ�ԭ
	virtual void ReSetPaiNum(BYTE num[],CheckHuStruct &TempPai);

	///��������Ƿ�ͬһ��ɫ
	virtual bool CheckTongSe(BYTE pai1,BYTE pai2);

	///��������Ƿ�����
	virtual bool CheckLianPai(BYTE pai1,BYTE pai2,BYTE num=1);

	///�����������������
	virtual void ClearPingHuData();

	///���ƽ��������ϣ����ƣ��ԣ����ܣ�˳�ӣ����̵�
	virtual void CheckPingHuData();

	///���һ�ֺ�������
	virtual void SetAHupaiType(BYTE type,BYTE hupai[]);

	///����һ�ֺ�������
	virtual void ReSetAHupaiType(BYTE type,BYTE hupai[]);

	///������������к�������
	virtual void ClearHupaiType(BYTE hupai[]);

	///����Ƿ����ĳ�ֺ�������
	virtual bool CheckHupaiType(BYTE type,BYTE hupai[]);

	///��ʼ������
	virtual void InitData();



/////////////////��ϸ�������ͼ��///////////////////////////////////////////////////////////////////////////////	
	//ƽ��
	virtual bool CheckIsPingHu();	
	//���
	virtual bool CheckTianHu();	
	//�غ�
	virtual bool CheckDiHu();	
	//���ϻ�
	virtual bool CheckGangKai();	
	//�ܺ���
	virtual bool CheckGangHouPao();	
	//һ���
	virtual bool CheckYiBanGao();	
	//ϲ���
	virtual bool CheckXiXiangFeng();	
	//����
	virtual bool CheckLianLiu();	
	//���ٸ�
	virtual bool CheckLaoShaoFu();	
	//�۾ſ�
	virtual bool CheckYaoJiuKe();	
	//����
	virtual bool CheckMingGang();	
	//ȱһ��
	virtual bool CheckQueYiMen();	
	//����
	virtual bool CheckWuZi();	
	//����
	virtual bool CheckBianZhang();	
	//����
	virtual bool CheckKanZhang();	
	//������
	virtual bool CheckDanDiaoJiang();	
	//����
	virtual bool CheckJianKe();	
	//Ȧ���
	virtual bool CheckQuanFengKe();	
	//�ŷ��
	virtual bool CheckMenFengKe();	
	//��ǰ��
	virtual bool CheckMenQianQing();	
	//�Ĺ�һ
	virtual bool CheckSiGuiYi();	
	//˫ͬ��
	virtual bool CheckShuangTongKe();	
	//˫����
	virtual bool CheckShuangAnGang();	
	//����
	virtual bool CheckAnGang();	
	//����
	virtual bool CheckDuanYao();	
	//ȫ����
	virtual bool CheckQuanDaiYao();	
	//������
	virtual bool CheckBuQiuRen();	
	//˫����
	virtual bool CheckShuangMingGang();	
	//�;���
	virtual bool CheckHuJueZhang();	
	//������
	virtual bool CheckPengPengHu();	
	//һɫ�Ƽ�⣨������ƺͳ������ƣ����ﶼ���ã���0����һɫ�ƣ�1��һɫ��2��һɫ��
	virtual int CheckYiSe();
	//��һɫ
	virtual bool CheckHunYiSe();	
	//��ɫ������
	virtual bool CheckSanSeSanBuGao();	
	//������
	virtual bool CheckWuMenQi();	
	//ȫ����
	virtual bool CheckQuanQiuRen();	
	//˫����
	virtual bool CheckShuangAnKe();	
	//˫����
	virtual bool CheckShuangJianKe();	
	//����
	virtual bool CheckHuaLong();	
	//�Ʋ���
	virtual bool CheckTuiBuDao();	
	//��ɫ��ͬ˳
	virtual bool CheckSanSeSanTongShun();	
	//��ɫ���ڸ�
	virtual bool CheckSanSeSanJieGao();	
	//�޷���
	virtual bool CheckWuFanHe();	
	//���ֻش�
	virtual bool CheckMiaoShouHuiChun();	
	//��������
	virtual bool CheckHaiDiLaoYue();	
	//���ܺ�
	virtual bool CheckQiangGangHe();	
	//������
	virtual bool CheckDaYuWu();	
	//С����
	virtual bool CheckXiaoYuWu();	
	//�����
	virtual bool CheckSanFengKe();	
	//����
	virtual bool CheckQingLong();	
	//��ɫ˫����
	virtual bool CheckSanSeShuangLongHui();	
	//һɫ������
	virtual bool CheckYiSeSanBuGao();	
	//ȫ����
	virtual bool CheckQuanDaiWu();	
	//��ͬ��
	virtual bool CheckSanTongKe();	
	//������
	virtual bool CheckSanAnKe();	
	//ȫ˫��
	virtual bool CheckQuanShuangKe();	
	//��һɫ
	virtual bool CheckQingYiSe();	
	//һɫ��ͬ˳
	virtual bool CheckYiSeSanTongShun();	
	//һɫ���ڸ�
	virtual bool CheckYiSeSanJieGao();	
	//ȫ��
	virtual bool CheckQuanDa();	
	//ȫ��
	virtual bool CheckQuanZhong();	
	//ȫС
	virtual bool CheckQuanXiao();	
	//һɫ�Ĳ���
	virtual bool CheckYiSeSiBuGao();	
	//����
	virtual bool CheckSanGang();	
	//���۾�
	virtual bool CheckHunYaoJiu();	
	//һɫ��ͬ˳
	virtual bool CheckYiSeSiTongShun();	
	//һɫ�Ľڸ�
	virtual bool CheckYiSeSiJieGao();	
	//���۾�
	virtual bool CheckQingYaoJiu();	
	//С��ϲ
	virtual bool CheckXiaoSiXi();	
	//С��Ԫ
	virtual bool CheckXiaoSanYuan();	
	//��һɫ
	virtual bool CheckZiYiSe();	
	//�İ���
	virtual bool CheckSiAnKe();	
	//һɫ˫����
	virtual bool CheckYiSeShuangLongHui();	
	//����ϲ
	virtual bool CheckDaSiXi();	
	//����Ԫ
	virtual bool CheckDaSanYuan();	
	//��һɫ
	virtual bool CheckLvYiSe();	
	//�ĸܣ�ʮ���޺���
	virtual bool CheckSiGang();	

	/////////////////////////��������///////////////////////////////////////////////////////////////////////////////////////////////

	//��������
	virtual bool CheckJiuLianBaoDeng();	
	//ȫ��������ʮ���ã�
	virtual bool CheckQuanBuKao();	
	//�����
	virtual bool CheckZuHeLong();	
	//�߶�
	virtual int CheckQiDui();	
	//���ǲ���������ʮ���ã�
	virtual bool CheckQiXingBuKao();	

	//���߶ԣ��߶Ե����⣩
	virtual bool CheckLianQiDui();	
	//ʮ����
	virtual bool CheckShiSanYao();	

};