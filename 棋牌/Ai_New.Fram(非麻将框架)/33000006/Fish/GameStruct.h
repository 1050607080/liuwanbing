//���ֽṹ�嶨��
//�ӵ��ṹ

struct FilesParmesA
{
    int                             m_PngSize;                              //�ļ���С
};
struct Bullet
{
	 //�ӵ��Ƿ����
	 bool	m_Have;
	 //�ӵ��ٶ�
	 int	m_Speed;
	 //�ӵ��Ƕ�
	 float	m_Roation;
	 //�ӵ�����
	 POINT	m_Point;
	 //�������
	 int	m_Senduser;
	 //�ӵ�״̬
	 bool	m_Stop;
	 //��״����
	 int	m_Netindex;
	 //
	 float	m_ptx;
	 //
	 float	m_pty;
	 //�Ƿ񳬼��ӵ�
	 bool	m_issuper;
	 //�ӵ�3��
	 bool	m_isThree;
	 //�Ƿ�����˷����
	 bool	m_isAndroid;
	 //������ʵ
	 int	m_RealUser;
};
//�������
struct Fish
{
     //�������
	 int m_index;
	 //���ID
	 int m_fishID;
	 //��ǰ��������
	 int m_actionindex;
	 //��ǰ�Ƿ񱻲���
	 bool m_isHit;
	 //�Ƿ����
	 bool m_Have;
	 //��ǰ����
	 POINT m_Point;
	 //��ǰ�Ƕ�
	 float m_Roation;
	 //�������,���������ʱ���ɷ���˷��͹�����
	 int m_DeadMoney;
	 //�������,�����������ң��ɷ��������͹���
	 int m_HitUser;
	 //˽��
	 int m_Nowindex;
	 //
	 int m_PtIndex;
	 //
	 int m_Currtime;
	 //
	 int m_speed;
	 //
	 float m_ptx;
	 //
	 float m_pty;
	 //����С��
	 int  m_smallFish;
	 //
	 int  m_traceIndex;
	 //
	 int  m_creatTime;


};
//Ʈ����ͼƬ
struct Gold
{
	 //Ʈ�����
	 int m_Player;
	 //�Ƿ����
	 bool m_Have;
	 //��ǰ����
	 POINT m_Point;
	 //��ǰ�Ƕ�
	 float m_Roation;
	 //�������,���������ʱ���ɷ���˷��͹�����
	 int m_DeadMoney;
	  //��ǰ��������
	 int m_actionindex;
	 //
	 float m_ptx;
	 //
	 float m_pty;


};
//��Ǯ����
struct MuchMoneyaction
{   
	 //�Ƿ����
     bool m_have;
	 //����X
	 int m_ptx;
	 //
	 int m_pty;
	 //
	 int m_nowindex;
	 //������Ǯ
	 int m_DiuMoney;
	 //ת���Ƕ�
	 int m_Roation;


};
//��ס����
struct Dingaction
{   
	 //�Ƿ����
     bool m_have;
	 //����X
	 int m_ptx;
	 //
	 int m_pty;
	 //
	 int m_nowindex;
	
};
//���Ǯ������,ÿ�����Ӧ����3�����������ݽṹ
struct MoneyDui
{
	 int m_Have;
	 //
	 int m_Money;
	 //
	 int m_Time;
	 //
	 //
	 int m_FishScore;
	 //
	 int m_Nowscore;
    
};
//�˳��л�����
struct WaterWavestruct
{
	  //�˳�����
      int m_ptx;
	  //
	  int m_pty;
	  //ʱ������
	  int m_Time;
	  //��ǰͼƬ
	  int m_currImgIndex;
}
;
//���������ʾ����
struct Numstruct
{
	  int m_Have;
	  //�˳�����
      int m_ptx;
	  //
	  int m_pty;
	  //ʱ������
	  int m_Time;
	  //��ǰͼƬ
	  int m_currImgIndex;
	  //
	  int m_Numcount;
	  //
	  int  m_HitUser;
	  //
	  float m_beishu;

}
;
//���������ʾ����
struct Soundmstruct
{
	  int m_Have;
	  int m_Time;
      LPCTSTR m_path;
	 

}
;
struct CursorStruct
{
      int m_ptx;
	  int m_pty;
	  int m_have;
};

