/**
* ��Ϸ�߼���
*
* @author duanxiaohui
* @version 1.0
* @date 2009.12.15
*/

#pragma once
#ifndef EZP_GAMELOGIC_H
#define EZP_GAMELOGIC_H

#include "GameMessage.h"

/// ���Ʒ���ֵ
enum BIPAI_RET
{
	BIPAI_STATION_ERROR = -1,   /**< ������Ϣ���� */
	BIPAI_FIRST_WIN = 1,		/**< ��һ���˵��ƴ� */
	BIPAI_SECONED_WIN = 2,      /**< �ڶ����˵��ƴ� */
	BIPAI_SAME =0				/**< ����������� */
};

/// ��������
enum PAI_XING
{

	PAI_ERROR = 0,              /**< û������ */

	PAI_TIANJIUWANG = 4,        /**< ����� */
	PAI_TIANGANG = 3 ,		    /**< ��� */
	PAI_DIGANG = 2,             /**< �ظ� */
	PAI_TIANPAIJIU = 1          /**< ���ƾ� */
};

/// �������ͣ��Ӵ�С
enum PAI_DOUBLE_NAME
{
	PAI_KIND_HUANGDI = 12,       /**< �ʵ� */
	PAI_KIND_DUITIAN = 11,       /**< ���� */
	PAI_KIND_DUIDI = 10,         /**< �Ե� */
	PAI_KIND_HUANGDI_EX = 9,     /**< ��չ�ʵ�(���72��Ϊһ�ֵİ汾) */
	PAI_KIND_DUIREN = 8,	     /**< ���� */
	PAI_KIND_DUIHE = 7,		     /**< �Ժ� */

	PAI_KIND_DUIZI_1 = 6,        /**< �Ժ��ġ��Ժ������Ժ�ʮ */
	PAI_KIND_DUIZI_2 = 5,        /**< �Ժ������Ժ��ߡ��Ժ�ʮ���Ժ�J */
	PAI_KIND_DUIZI_3 = 4,        /**< �Ժ��塢�Ժ��ߡ��Ժڰˡ��Ժ�� */

	PAI_KIND_Q9 =  3,            /**< Q + 9��� *///�����
	PAI_KIND_Q8 =  2,            /**< Q + 8��� *///���
	PAI_KIND_28 =  1,            /**< 2 + 8��� *///���

	PAI_DUIZI_ERROR = 0          /**< �������� */
};								

/// ������������
static char * g_PaiXing_Name[5] =
{
	TEXT("û����"), TEXT("���ƾ�"), TEXT("�ظ�"), TEXT("���"), TEXT("�����")
};

/// ��Ϸ�߼���
class GameLogic
{
public:
	BYTE            m_cardAry[PAI_COUNT];            /**< ��Ϸ�е������� */
	PaiData         m_userData[4];                   /**< ÿ����ҵ���,ÿ��Ԫ�ض�Ӧ1��λ�� */
	PaiData         m_GameCard[GAME_XIPAI_COUNT][4]; /**< ����72�������� */

protected:
	BYTE            m_bGameVersion;                  /**< ��Ϸ�汾 0: ��ͨ 1: ������ 2: ���«2��*/
	BOOL            m_bKingAndThreeBig;              /**< ���ִ�������3 �� ˫��Q ��С: TRUE: ������3��� FALSE: ˫��Q���*/
	BOOL            m_bUseCard;                      /**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */
	BOOL            m_bRedJOrBlackJ;                 /**< ��J���Ǻ�J FALSE: ��J TRUE: ��J */
public:
	/// ���캯��
	GameLogic(void);

	/// ��������
	~GameLogic(void);

	/// ��ʼ������
	void InitGame();

	/// ��ʱ�뷽������Լ�����һ��
	static BYTE NiNextStation(BYTE  station)
	{
		return (station + 3) % 4;
	}

	/// ��ʱ�뷽��,����ׯ��λ�����ָ������num ��λ��
	static BYTE NiNextNumStation(BYTE zhuangStation,BYTE num)
	{
		if (num == 1)
		{
			return NiNextStation(zhuangStation);
		}

		BYTE retStation = zhuangStation ;
		for(int i = 1;i <= num -1;i++)
		{
			retStation = NiNextStation(retStation);
		}
		return retStation;
	}

	/// ˳ʱ�뷽����һ��
	static BYTE ShunNextStation(BYTE station)
	{
		return (station + 1) / 4;
	}

public:
	/// ��ȡ�ƻ�ɫ
	inline BYTE     GetShape(BYTE bValue) { return (bValue&0xF0)>>4; }

	/// ��ȡ�Ƶ�    
	inline BYTE     GetNumber(BYTE bValue) { return (bValue&0x0F)-1; }

	/// ������Ϸģʽ
	void            SetGameMode(BYTE bGameVersion, BOOL bUseCard, BOOL bKingAndThreeBig = TRUE,BOOL bRedJOrBlackJ = TRUE);

	/// ϴ��,ϴ�����Զ�����
	void            GameXiPai(BOOL bRecordCard = FALSE);

	/// ���һ�����˿�
	BYTE			RandACard();

	/// ��ʮ����������ת����16��������, ����ʮ������ֵ
	BYTE			GetValue(BYTE bValue);

	/// ����ĳ������������
	BOOL            SetUserCard(BYTE userStation,BYTE firstCard,BYTE secondCard);

	/// ����ĳ������������
	BOOL            SetUserCard(BYTE userStation,PaiData & paiData);

	/// ����û������Ƿ��Ƕ���
	BOOL            IsCardDouble(const PaiData & paiData);

	/// �ж������
	BOOL            IsCompagasCard(const PaiData & paiData);

	/// �õ��û����е��ƶ��ӵĴ�Сֵ, ���Ʋ��Ƕ��ӣ��򷵻� PAI_DUIZI_ERROR
	PAI_DOUBLE_NAME GetCardDouble(const PaiData & paiData, BYTE bGameMode = GAME_VERSION_1);

	/// ��ö�����������һ�ŵ��ƻ����ĵ���λ��
	int             GetSingleCardMax(const PaiData & paiData, BOOL bSeat = FALSE);

	/// �����û��Ƶ���
	int             CalcCardValue(const PaiData & paiData);

	/// ��ȡ�Ƶĵ���
	int             GetCardValue(const BYTE card);

	/// ��ȡ�Ƶ���������
	CString         GetPaiValueName(PaiData & paiData);

	/// ��ȡ�Ƶ���������
	CString         GetPaiValueName(BYTE staion);

	/// ��ȡ��������Ƶ�����
	PAI_XING        GetCardPaiXing(const PaiData & paiData);

	/// ��ȡ�����Ƶ�����
	char *          GetDuiZiName(const PAI_DOUBLE_NAME paiDuiZi);
	/// ��ȡ��������
	char *          GetPaiXingName(const PAI_XING & paiXing);

	/// �Ƚ�������ҵ��ƴ�С
	BIPAI_RET       BiPaiFromStation(const BYTE station1, const BYTE station2);

protected:
	/// �����ֵ�Ƿ�Ϸ�
	BOOL            IsPaiValue(const PaiData & paiData);

	/// ����
	void            GameFaPai();

	/// �Ƚ�������ҵ��ƴ�С
	BIPAI_RET       BiPaiFromData(const PaiData & paiData1,const PaiData & paiData2);

	/// �Ƚ������˵��ƵĴ�С
	BIPAI_RET       DanPaiBiPai(const PaiData & paiData1,const PaiData & paiData2, BYTE bGameMode = GAME_VERSION_1);
};

#endif