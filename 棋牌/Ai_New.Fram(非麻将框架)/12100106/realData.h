
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

#include "clientgamedlg.h"

///ʵ�����Ľ������
CUpGradeGameLogic logic;
class realData
{
public:
	realData& operator= (realData& d)
	{

		memcpy(m_MyCard,d.m_MyCard,sizeof(m_MyCard));

		isLeaf = d.isLeaf;
		shengli = d.shengli;
		shibai = d.shibai;
        
		return *this;
	}
	realData()
	{
		isLeaf = false;
		shengli = 0;
		shibai = 0;
	}
	 ~realData()
	 {
		
	 }

	bool isRightResult()                       ///�����ж��Լ��Ƿ���һ��Ҫ�ҵ��Ľ�
	{
		return false;
	}

    void acess(realData& d)                     ///�����Լ��ĺ��ӣ����ڻ��ݽ������������
	{	
		shengli += d.shengli;
		shibai += d.shibai;
	}

	void born(list<realData> &lData)            ///���������Լ��ĺ��ӽ�������
	{
		///���ɵ��������ڵ���3ʱ���Ʋſ�ʼ���ˣ�����3�������ڵ�����������ʱ���ʤ�����岻��
        

	}


	BYTE					m_MyCard[3];			//�Լ���������
	BYTE                    m_DuiShouCard[3];       //���ֵ���
	
	///��¼�Ƿ�ΪҶ�ӽڵ�
    bool                    isLeaf;

	///ʤ���Ĵ���
	int                     shengli;
	///ʧ�ܵĴ���
	int                     shibai;
		
	///��Ϊʵ�����õ���ֻ�������ƣ�������Ƚ����ˣ���Ҫ������˼�����Կ���ֱ�Ӽ���ʤ�ʣ�����ͨ�����ķ�������ʤ���Ŀ�����
	void computeShengLv()                      
	{
		vector<BYTE> vCard;
		getLeftCard(vCard);
        
		int leftCardNum = vCard.size();
		for(int i=0;i<leftCardNum-2;i++)
		{
			m_DuiShouCard[0] = vCard[i];
			for(int j=i+1;j<leftCardNum-1;j++)
			{
				m_DuiShouCard[1] = vCard[j];
				for(int k=j+1;k<leftCardNum;k++)
				{
					m_DuiShouCard[2] = vCard[k];
					if(logic.CompareCard(m_MyCard,3,m_DuiShouCard,3)>0)
					{
						shengli++;
					}
					else
					{
						shibai++;
					}
				}
			}
		}
	}

private:
	///�����ڲ�����
	

    
	///����ʣ�µ���
	void getLeftCard(vector<BYTE>& vCard)
	{
		static const BYTE m_CardArray[52] =
		{
			0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
			0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
			0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
			0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D 		//���� 2 - A
		};
		vCard.resize(52);
        for (int i=0;i<52;i++)
		{
			vCard[i] = m_CardArray[i];
		}	
		/// ȥ�����е���
		vector<BYTE>::iterator it;
		for(int i=0;i<3;i++)
		{
			it = find(vCard.begin(),vCard.end(),m_MyCard[i]);
			vCard.erase(it);
		}
	}
};

