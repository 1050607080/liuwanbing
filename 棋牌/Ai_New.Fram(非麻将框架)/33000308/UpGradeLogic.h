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
	/// ���캯��
	GameLogic(void);

	/// ��������
	~GameLogic(void);
};

#endif