#pragma once

//#include "mj.h"
#include "../ChangeFile/UpgradeMessageEx.h"

//�����߼����߼��� 
//�������ݻ�������������ɾ������ѯ��
class CLogicBase
{
public:
	CLogicBase(void);
	~CLogicBase(void);

public:
	
	
	///���������һ������
	///@param  arr[] ����, data Ҫ��ӵ�����, ArrLen ����ĳ���
	///@return true ��ӳɹ���false �������ˣ���Ӳ��ɹ�
	static bool AddOneDataToArr(BYTE arr[],BYTE data,BYTE ArrLen)
	{
		for(int i=0;i<ArrLen;i++)
		{
			if(arr[i]==255)
			{
				arr[i] = data;
				return true;
			}
		}
		return false;
	}

	///��������ɾ��һ������
	///@param  arr[] ����, data Ҫɾ��������, ArrLen ����ĳ���
	///@return true ��ӳɹ���false �������ˣ���Ӳ��ɹ�
	static bool DeleteOneDataFromArr(BYTE arr[],BYTE data,BYTE ArrLen)
	{
		for(int i=0;i<ArrLen;i++)
		{
			if(arr[i]==data)
			{
				arr[i] = 255;
				return true;
			}
		}
		return false;
	}

	///��ȡ��ɫ�ӵ��������
	///@param  data0 ɫ��1, data1 ɫ��2, data2 ɫ��3
	static void GetSeziData(BYTE *data0 = NULL, BYTE *data1 = NULL, BYTE *data2 = NULL)
	{
		srand((unsigned)time(NULL));
		if(data0 != NULL)
		{
			*data0 = rand()%6+1;
		}
		if(data1 != NULL)
		{
			Sleep(50);
			*data1 = rand()%6+1;
		}
		if(data2 != NULL)
		{
			Sleep(50);
			*data2 = rand()%6+1;
		}
	}
	
	///��ȡ�������ЧԪ�ظ���
	///@param  arr[] Ŀ������,data ��Чֵ, ArrLen ���鳤��
	///@return ��ЧԪ�ظ���
	static int GetArrNum(BYTE arr[],BYTE data,BYTE ArrLen)
	{
		int num = 0;
		for(int i=0;i<ArrLen;i++)
		{
			if(arr[i] != data)
			{
				num++;
			}
		}
		return num;
	}

	///�ж��������Ƿ����ĳ��Ԫ��
	///@param  arr[] Ŀ������,data ��Чֵ, ArrLen ���鳤��
	///@return ��ЧԪ�ظ���
	static bool IsHaveInArr(BYTE arr[],BYTE data,BYTE ArrLen)
	{
		for(int i=0;i<ArrLen;i++)
		{
			if(arr[i] == data)
			{
				return true;
			}
		}
		return false;
	}
	
	///��ȡ�¼�λ��
	static BYTE GetNextStation(BYTE station,bool shun=false,bool hu[] = NULL)
	{
		BYTE re = 0;
		if(station == 0)
		{
			re = 1;
		}
		return re;
	}

	//��ȡ����ĺ������
	static BYTE GetCanHuUser(BYTE outst,bool hu[],BYTE hust,BYTE fan[]=NULL)//��ȡ�������ҽ������
	{
		//BYTE station = outst;
		//BYTE maxfan =0,st=hust;
		//if(fan != NULL)//�����뷬������ѡ���Ʒ������
		//{
		//	for(int i=0;i<PLAY_COUNT;++i)
		//	{
		//		if(i == station || !hu[i])
		//			continue;
		//		if(fan[i] > maxfan)
		//		{
		//			maxfan = fan[i];
		//			st = i;
		//		}
		//	}
		//	return st;
		//}
		//for(int i=0;i<PLAY_COUNT-1;++i)
		//{
		//	station = GetNextStation(station);
		//	if(hu[station])
		//		return station;
		//}
		return hust;
	}

	//�������ݿ���
	static void CopyChiData(GCPStructEx &dest,tagChiPaiEx &sorce)//��ȡ�������ҽ������
	{
		dest.Init();
		dest.byType = ACTION_CHI;
		dest.iBeStation = sorce.byBeChi;
		dest.iOutpai = sorce.byPs;
		dest.iStation = sorce.byUser;
		dest.byData[0] = sorce.byChiPs[0];
		dest.byData[1] = sorce.byChiPs[1];
		dest.byData[2] = sorce.byChiPs[2];
	}

	//�������ݿ���
	static void CopyPengData(GCPStructEx &dest,tagPengPaiEx &sorce)//��ȡ�������ҽ������
	{
		dest.Init();
		dest.byType = ACTION_PENG;
		dest.iBeStation = sorce.byBePeng;
		dest.iOutpai = sorce.byPs;
		dest.iStation = sorce.byUser;
		dest.byData[0] = dest.byData[1] = dest.byData[2] = sorce.byPs;
	}

	//�������ݿ���
	static void CopyGangData(GCPStructEx &dest,tagGangPaiEx &sorce)//��ȡ�������ҽ������
	{
		dest.Init();
		dest.byType = sorce.byType;
		dest.iBeStation = sorce.byBeGang;
		dest.iOutpai = sorce.byPs;
		dest.iStation = sorce.byUser;
		dest.byData[0] = dest.byData[1] = dest.byData[2] = dest.byData[3] = sorce.byPs;
	}

	//��ȡ��һ�ֵ�ׯ��
	static void GetNextNt(tagCountFenEx& CountFen,BYTE &newNt,BYTE &LianZhuang,BYTE type)
	{
		bool lian = true;
		LianZhuang++;
		switch(type)
		{
		case 0://ׯӮ��ׯ�������¼���ׯ
			{
				if(!CountFen.bHu[newNt] || CountFen.byUser==255)//������û��ׯ�һ�������
				{
					newNt = GetNextStation(newNt);
					lian = false;
				}
			}
			break;
		case 1:///ׯӮ��������ׯ�������¼���ׯ
			{
				if(!CountFen.bHu[newNt] && CountFen.byUser!=255)//������û��ׯ�һ�������
				{
					newNt = GetNextStation(newNt);
					lian = false;
				}
			}
			break;
		case 2:///˭Ӯ˭��ׯ�����ֻ�ׯӮׯ����ׯ
			{
				if(CountFen.byUser!=newNt)
				{
					newNt = CountFen.byUser;
					lian = false;
				}
			}
			break;
		case 3:///˭Ӯ˭��ׯ�������¼���ׯ
			{
				if(CountFen.byUser==255)//����
				{
					newNt = GetNextStation(newNt);
					lian = false;
				}
				else
				{
					if(newNt != CountFen.byUser)
					{
						lian = false;
					}
					newNt= CountFen.byUser;
				}
			}
			break;
		case 4://���ܽṹ��������¼���ׯ
			{
				newNt = GetNextStation(newNt);
				lian = false;
			}
			break;
		}
		if(!lian)
		{
			LianZhuang=1;
		}
	}
	




};
