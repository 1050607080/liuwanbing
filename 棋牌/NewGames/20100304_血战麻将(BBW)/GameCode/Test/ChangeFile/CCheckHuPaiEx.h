#pragma once

#include "../GameMessage/CCheckHuPai.h"
///�¼������ࣨ���أ�
class CCheckHuPaiEx:public CCheckHuPai
{
public:
	CCheckHuPaiEx(void);
	~CCheckHuPaiEx(void);
	
public:
	//virtual 
	
	///����ܷ�����
	virtual bool CanTing(BYTE station,tagCPGNotifyEx &ResultData);
	//��ϸ����������
	virtual bool CanHu(BYTE station,BYTE lastpai,tagCPGNotifyEx &ResultData,bool zimo,BYTE dianpao);
	///ת���������ݵĴ洢��ʽ��Ϊ�˷�����Ƽ��
	virtual void ChangeHandPaiData(BYTE handpai[],BYTE pai);
	///ƽ�����
	virtual bool CheckPingHu(BYTE lastpai=255,bool zimo=false,bool jing=false,bool CheckTing=false);
	///��ȡ�����������
	virtual bool  GetMaxCombination(bool zimo,bool jing,BYTE lastpai = 255);
	///ƽ������
	virtual bool MakePingHu(CheckHuStruct &PaiData,PingHuStruct &hupaistruct,BYTE csnum=0);
	///��ʼ������
	virtual void InitData();
	///////////��ϸ��������//////////////////////////////////////////////


	//�߶�
	virtual int CheckQiDui();	
	//������
	virtual bool CheckPengPengHu();	
	//���
	virtual bool CheckTianHu();	
	//�غ�
	virtual bool CheckDiHu();	
	//���ϻ�
	virtual bool CheckGangKai();	
	//�ܺ���
	virtual bool CheckGangHouPao();	
	//���ܺ�
	virtual bool CheckQiangGangHe();	


};

