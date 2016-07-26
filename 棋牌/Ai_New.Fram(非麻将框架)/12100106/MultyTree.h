


///  MultyTree.h

/*
* ��·���߷������Ľ���
*/
#include "realData.h"
#include <queue>
using namespace std;

/// һ����·���߷������Ľ��
class Mnode
{
	friend class multyTree;
///������Ա
	///����һ����·���Ľ��
	Mnode(realData &t,Mnode *p=NULL)     
	{
		data = t;
        pfather = p;
	}
    ///�Լ����˵�ʱ��Ҫɱ�����е�����
    ~Mnode()                            
	{
        killMySun();
	}
    
	/// ɱ���Լ����еĺ���
	void killMySun()
	{
		list<Mnode*>::iterator it;
		for(it=lnode.begin();it!=lnode.end();it++)
		{
			delete *it;
		}
		lnode.clear();
	}

    ///����һ������
	Mnode *born(realData &t)             
	{
		Mnode *mySun = new Mnode(t,this);
		lnode.push_back(mySun);
		return mySun;
	}
    
	///�����Լ��ĺ���
	void born()
	{
		list<realData> lData;
		data.born(lData);
		list<realData>::iterator it;
		for(it=lData.begin();it!=lData.end();it++)
		{
			born(*it);
		}
	}

///����
	list<Mnode*> lnode;                 ///�Լ��ĺ��ӽ��
    Mnode* pfather;                     ///���ڵ�
    realData data;                             ///���ڵ���Я����״̬����
};
///��·��
class multyTree
{
public:
	///���캯��,���ɸ��ڵ�
	multyTree(realData &t)               
	{
		root = new Mnode(t);
	}
    
	///���캯��
	multyTree(Mnode* r) 
	{
		root = r;
	}

    ~multyTree()
	{
		delete root;
	}
	/// ���ݷ�ʵ��
	bool depth(realData &t)
	{
		bool result = depth();         ///Ϊ���㷨��ͨ���ԣ������Ƿ��ҵ���ȷ�ĽⶼҪ�õ��ڵ�����
		t = root->data;
		return result;
	}

protected:
    /// ���ݷ�ʵ�֣�������ȷ��ͷ���true�����򷵻�false
	bool depth()
	{
		if(root->data.isRightResult())               ///����Լ�����һ����ȷ�Ľⷵ��
			return true;
		
		root->born();          ///����һЩ�ӽڵ�����
        multyTree* pTree = NULL;
        list<Mnode*>::iterator it;
		for(it = root->lnode.begin();it!=root->lnode.end();it++)
		{
			pTree = new multyTree(*it);
			if(pTree->depth())                         ///�������ҵ�
			{
				root->data.acess(pTree->root->data);
				return true;
			}
			root->data.acess(pTree->root->data);
			
			delete pTree;
		}
        root->lnode.clear();       ///��Ϊ���Ķ����Ѿ����ˣ�����Ҫȥ�����ӵ�����
		return false;
	}
	///��֧�޽�ʱ��ʵ��,����Ѱ�������һ����
	void extent()
	{

	}
protected:
	Mnode* root;
};