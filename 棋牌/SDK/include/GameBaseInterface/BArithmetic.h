#pragma once

//����ר�Ŵ�����Ϸ�������ڿ��������л��۵Ĵ����м�ֵ���ظ�ʹ���ʼ��ߵ��㷨
class BaseArithmetic
{
public:
	//���캯��
	BaseArithmetic();
	virtual ~BaseArithmetic();

public:
	//���������
	int MyRand();

	//��ȡȨ�����ֵ
	int GetWeight(unsigned int iPoint[], int iCount);

	//�����������λ��
	int GetNumPlace(int inum);
};

#include "BArithmetic.inl"