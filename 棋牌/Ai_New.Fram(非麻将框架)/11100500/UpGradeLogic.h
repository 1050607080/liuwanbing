#pragma once

//��������
#define	UG_HUA_MASK					0xF0			//1111 0000
#define	UG_VALUE_MASK				0x0F			//0000 1111

//�˿˻�ɫ
#define UG_FANG_KUAI				0x00			//����	0000 0000
#define UG_MEI_HUA					0x10			//÷��	0001 0000
#define UG_HONG_TAO					0x20			//����	0010 0000
#define UG_HEI_TAO					0x30			//����	0011 0000
#define UG_NT_CARD					0x40			//����	0100 0000
#define UG_ERROR_HUA				0xF0			//����  1111 0000

//�˿˳�������
#define UG_ERROR_KIND				0				//����
#define UG_ONLY_ONE					1				//����
#define UG_DOUBLE					2				//����
#define UG_THREE					3				//����
#define UG_DOUBLE_TRACKOR			4				//����������
#define UG_THREE_TARCKOR			5				//����������
#define	UG_SHOW_CARD				6				//˦��


//�˿˷����ṹ	��ֻ���ڷ������Ա��˿ˣ�
struct CardAnalyseStruct
{
	int		iSignedCount;					//������Ŀ
	int		iDoubleCount;					//������Ŀ
	int		iThreeCount;					//������Ŀ
	int		iDoubleTractorCount;			//������������Ŀ
	int		iThreeTractorCount;				//������������Ŀ
	BYTE	iCardHuaKind;					//�˿˻�ɫ

	BYTE	iBigSignedCard;					//�����
	BYTE	iLessSignedCard;				//��С����
	BYTE	iDoubleArray[19];				//��������
	BYTE	iThreeArray[13];				//��������
	BYTE	iDoubleTractorArray[54];		//�������������� ��0 �ָ���ȣ����������ݣ�
	BYTE	iThreeTractorArray[48];			//�������������� ��0 �ָ���ȣ����������ݣ�
};

//�����߼��� ��֧�� 2 ���� 3 ���˿ˣ�
class CUpGradeGameLogic
{
	//��������
private:
	bool					m_bTwoIsNT;
	BYTE					m_iNTNum;							//��������
	BYTE					m_iNTHuaKind;						//���ƻ�ɫ
	int						m_iStation[5];						//���λ�ã����飬÷�������ң����ң����ƣ�
	BOOL					m_bHaveKing;						//�Ƿ�����
	//��������
public:
	//���캯��
	CUpGradeGameLogic(void);

	//���ܺ���������������
public:
	//����2�Ƿ�Ϊ����
	void SetTwoIsNT(bool bTwoIsNT){m_bTwoIsNT=bTwoIsNT;}
	//��ȡ2�Ƿ�Ϊ��
	bool GetTwoIsNT(){return m_bTwoIsNT;}
	//�����Ƿ�������
	void SetHaveKing(BOOL bHaveKing = false){m_bHaveKing = bHaveKing;}
	//��ȡ����
	BOOL GetHaveKing(){ return m_bHaveKing ;}
	//�������ƻ�ɫ
	void SetNTHuaKind(BYTE iNTHuaKind);
	//��ȡ���ƻ�ɫ
	inline BYTE GetNTHuaKind() { return m_iNTHuaKind; }
	//������������
	inline void SetNTCardNum(BYTE iNTNum) { m_iNTNum=iNTNum; }
	//��ȡ��������
	inline BYTE GetNTCardNum() { return m_iNTNum; }
	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK)+1; }
	//��ȡ�˿˻�ɫ
	BYTE GetCardHuaKind(BYTE iCard, BOOL bTrueHua = true);
	//�����˿�
	BOOL SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount);
	//ɾ���˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);

	//��ȡָ������
	BYTE TackOutBySpecifyCard(BYTE iCardList[], int iCardCount,BYTE bCardBuffer[],int &iResultCardCount,BYTE bCard);
	//��ȡָ���Ƹ���
	BYTE GetCountBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE bCard);

	//��������
public:
	//�Ƿ���
	inline BOOL IsOnlyOne(BYTE iCardList[], int iCardCount) { return iCardCount==1; };
	//�Ƿ����
	BOOL IsDouble(BYTE iCardList[], int iCardCount);
	//�Ƿ�����
	BOOL IsThree(BYTE iCardList[], int iCardCount);
	//�Ƿ��������߼�
	BOOL IsTrackorRule(BYTE iFirstCard, BYTE iNextCard);
	//�Ƿ���������������
	BOOL IsDoubleTrackor(BYTE iCardList[], int iCardCount);
	//�Ƿ���������������
	BOOL IsThreeTrackor(BYTE iCardList[], int iCardCount);
	//�Ƿ�˦��
	BOOL IsShowCard(BYTE iCardList[], int iCardCount);
	//�õ����л�ɫ
	BYTE GetCardListHua(BYTE iCardList[], int iCardCount);
    //��ȡ����
	BYTE GetCardShape(BYTE iCardList[], int iCardCount);
	//��ȡ�˿���Դ�С
	int GetCardBulk(BYTE iCard, BOOL bExtVol);
	//�Աȵ���
	BOOL CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard);

	//��������
public:
	//��ȡ�˿�(���ݻ�ɫ,����ȷ����)
	BYTE GetCardFromHua(int iHuaKind, int iNum);
	//���ҷ���
	int FindPoint(BYTE iCardList[], int iCardCount);
	//��������
	BOOL AnalyseCard(BYTE iCardList[], int iCardCount, CardAnalyseStruct & Analyse);

	//������ר�ú���
public:
	//�ԱȺ���
	int CompareCard(BYTE * iCardListArray[], int iCardCount);
	//�Ա�˦��
	int CompareShowCard(BYTE * iCardListArray[], int iCardCount);
	//�Ƿ����˦��
	BOOL CanShowCard(BYTE iOutCard[], int iOutCardCount, int iOutStation, BYTE * iCardArray[], int iCardArrayCount, BYTE iResultCard[], int & iResultCardCount);
	//�����˿�
	BYTE RandCard(BYTE iCard[], int iCardCount,BOOL bHaveKing =true);
	//�仯�˿˸�ʽ
	BOOL FixCardType(BYTE iCardList[], int iCardCount, CardAnalyseStruct & BaseAnalyse, CardAnalyseStruct & OutAnalyse);

	//�ͻ��˺���
public:
	//�Զ����ƺ���
	BOOL AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut);
	//�Ƿ���Գ���
	BOOL CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], int iBaseCount, BYTE iHandCard[], int iHandCount);

	//�ڲ�����
private:
	//��� 0 λ�˿�
	int RemoveNummCard(BYTE iCardList[], int iCardCount);
	//��ȡ���ƺ���
	int TackOutDouble(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], BOOL bRemoveCard);
	//��ȡ��������������
	int TackOutDoubleTrackor(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], BOOL bRemoveCard);
	//��ȡ��������
	int TackOutThree(BYTE iCardList[], int iCardCount, BYTE iThreeBuffer[], BOOL bRemoveCard);
	//��ȡ��������������
	int TackOutThreeTrackor(BYTE iCardList[], int iCardCount, BYTE iThreeBuffer[], BOOL bRemoveCard);
	//��ȡͬһ��ɫ�˿�
public:
	int TackOutCardByHua(BYTE iCardList[], int iCardCount, BYTE iHuaKind, BYTE iCardBuffer[], BOOL bRemoveCard);
	//�������������
	int	MaxThreeTractorLong(BYTE iCardList[], int iCardCount, BYTE iTrackorBuffer[]);
	//�������������
	int	MaxDoubleTractorLong(BYTE iCardList[], int iCardCount, BYTE iTrackorBuffer[]);
	//�����û�������������
	int FindTractorNum(BYTE iCardList[],int iCardCount);
	//��������ĳ��������
	BOOL TackOutByCondition(BYTE iCardList[],int iCardCount, BYTE iResultCard[],
								int & iResultCardCount,
								BYTE iConditionCard[],int iConditionCardCount);
	//����==iCard�ĵ�������iCardList�е����(��ʼλ��,���K�cλ��)
	int GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard);

};
