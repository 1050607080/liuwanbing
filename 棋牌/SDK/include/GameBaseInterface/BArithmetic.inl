#include <winbase.h>
#include "BArithmetic.h"

//���캯��
BaseArithmetic::BaseArithmetic()
{
	return;
}

BaseArithmetic::~BaseArithmetic()
{
	return;
}

/*************************************************
*Function: ���������
*explain:�ú�������һ���̶���ֵ����ȡ���ֵ����Ч��������������ʱ���ֹ���������
*writer:
*		˧����
*Parameters:
*		void
*Return:
*		���������
*************************************************/
int BaseArithmetic::MyRand()
{
	static int innerSeed = GetTickCount();

	return (((innerSeed = innerSeed * 214013L + 2531011L) >> 16) & 0x7fff );
}

/*************************************************
*Function: ��ȡȨ�����ֵ
*explain:�ú���ͨ��һ��Ȩ��ֵ������ĳһ��Χ�����ֵ�����ɸ��ʣ�
*����ֵ���ܳ����꡾MAX_PATH����Ȩ��ֵ������ڵ���0
*writer:
*		˧����
*Parameters:
*		void
*Return:
*		���������
*************************************************/
int BaseArithmetic::GetWeight(unsigned int iPoint[], int iCount)
{
	unsigned int idata[MAX_PATH + 1] = {0};//���öϵ� ��Ŀ��ָ��������Чֵ��һ��
	unsigned int imidd = 0;
	if (iCount > MAX_PATH)
	{
		return 0;
	}

	for (int i = 0; i < iCount; ++i)//����������Чֵ�����ֶϵ�
	{
		idata[i] = imidd;
		imidd += iPoint[i];
	}

	idata[iCount] = imidd;//�����һ���ϵ�ӽ���

	unsigned int tag = MyRand() % imidd;//��ָ����Χ���������ֵ

	for (int i = 0; i < iCount; ++i)
	{
		if (tag >= idata[i] && tag <= idata[i+1])//�ڶϵ�֮�䣬���ظöϵ�ֵ
		{
			return i;//��Ȩ���ҵ������ֵ
		}
	}

	return 0;//�쳣��������ʼֵ
}

/*************************************************
*Function: �����������λ��
*explain:�ú�����������������λ��
*writer:
*		˧����
*Parameters:
*		��Ҫ���������
*Return:
*		����λ��
*************************************************/
int BaseArithmetic::GetNumPlace(int inum)
{
	int iPlace = 0;
	inum = abs(inum);//ȡ����ֵ
	while(inum > 0)
	{
		inum = inum / 10;
		iPlace++;
	}

	return iPlace;
}