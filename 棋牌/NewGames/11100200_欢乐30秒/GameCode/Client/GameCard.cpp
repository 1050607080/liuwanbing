#include "Stdafx.h"
#include "GameCard.h"
#include "ClientGameDlg.h"

#define ID_TIMER_MOVE_CARD        2008    // �ƶ�ID
#define MOVE_CARD_TIME_INTERVAL   100     // ʱ����

//��̬����
CGameImage				CUpGradeGameCard::m_CardPic[6];								//������
BEGIN_MESSAGE_MAP(CUpGradeGameCard, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

//���캯��
CUpGradeGameCard::CUpGradeGameCard(BYTE bCardStyle)
{	
	//c++ test
	m_iCardHight = 0;
	m_iCardWidth = 0;
	m_iCardHSpace = 0;
	m_iCardVSpace = 0;
	m_iCardUpSpace = 0;
	m_bMove = false;
	m_CurCardIndex = 0;

	m_byCardCount=0;
	SetCardStyle(bCardStyle);
	SetCardParameter(15,13,20);
	m_pHandleWnd=NULL;
	m_bHorzLook=true;
	m_bShowCard=false;
	m_bUseMouse=false;
	m_wHotHitCard=255;
	m_bSelect=false;
	m_PtBasePoint.SetPoint(0,0);
	m_bFriendCardCount=0;			//��������
	m_bShowNum = 0;
	m_XShowMode=EXSM_CENTER;
	m_YShowMode=EYSM_CENTER;
	m_iSourcePoint = 0 ;
	m_iBackPoint = 0;
	m_iBackMultiple = 1;
	m_ptLeftHit = CPoint(0,0);
	m_bCanMove = FALSE;
	m_bMultiSel = false;
	m_bMoveSel = false;
	// �˿���Ŀ(��m_byCardCountView>m_byCardCount ���� m_byCardCount>0ʱ��ʾm_byCardCountView)
	// ������ʾm_byCardCount
	m_byCardCountView=0;
}

//��������
CUpGradeGameCard::~CUpGradeGameCard()
{
}

//��ʼ������
bool CUpGradeGameCard::InitGameCard(CWnd * pHandleWnd)
{
	m_pHandleWnd=pHandleWnd;
	return true;
}

//�����˿˲���
void CUpGradeGameCard::SetCardParameter(int iHSpace, int iVSpace, int iUpSpace)
{
	m_iCardHSpace=iHSpace;
	m_iCardVSpace=iVSpace;
	m_iCardUpSpace=iUpSpace;
	FixCardControl();
	return;
}

//��ȡ��С����
bool CUpGradeGameCard::GetCardSize(CSize & Size)
{
	if (m_byCardCount>0)
	{
		if (m_bHorzLook==true)
		{
			Size.cx=m_iCardWidth+(m_byCardCount-1)*m_iCardHSpace;
			Size.cy=m_iCardHight+m_iCardUpSpace;
		}
		else
		{
			Size.cx=m_iCardWidth;
			Size.cy=m_iCardHight+(m_byCardCount-1)*m_iCardVSpace;
		}
	}
	else
	{
		Size.cx=0;
		Size.cy=0;
	}
	return true;
}

//���û�׼�㺯��
void CUpGradeGameCard::SetBasePoint(CPoint & BasePoint, E_XSHOW_MODE XShowMode, E_YSHOW_MODE YShowMode)
{
	m_XShowMode=XShowMode;
	m_YShowMode=YShowMode;
	m_PtBasePoint=BasePoint;
	FixCardControl();
	return;
}

//������ʾ�˿�
void CUpGradeGameCard::SetShowCard(bool bShow)
{
	m_bShowCard=bShow;
	Invalidate(FALSE);
	return;
}

//����ʹ�����
void CUpGradeGameCard::SetUseMouse(bool bUserMouse)
{
	m_bUseMouse=bUserMouse;
	return;
}

//������ʾ��ʽ
void CUpGradeGameCard::SetLookMode(bool bHorzLook)
{
	m_bHorzLook=bHorzLook;
	return;
}

//�����˿�
bool CUpGradeGameCard::SetCard(WORD wCardList[], WORD wUpList[], BYTE bCardCount, BYTE byCardCountView)
{

	if(wCardList == NULL)
	{
		m_iSourcePoint = 0;
		m_iBackPoint = 0; 
		m_iBackMultiple = 1;
	}
	if (bCardCount>m_wCardArray.GetSize()) m_wCardArray.SetSize(bCardCount);
	if (bCardCount>m_wCardUpArray.GetSize()) m_wCardUpArray.SetSize(bCardCount);
	if (wCardList!=NULL) CopyMemory(m_wCardArray.GetData(),wCardList,sizeof(WORD)*bCardCount);
	if (wUpList!=NULL) CopyMemory(m_wCardUpArray.GetData(),wUpList,sizeof(WORD)*bCardCount);
	else memset(m_wCardUpArray.GetData(),0,sizeof(WORD)*bCardCount);
	if(m_wMoveSelectCardArray.GetSize()!=bCardCount)
	{
		m_wMoveSelectCardArray.SetSize(bCardCount);
		memset(m_wMoveSelectCardArray.GetData(),0,sizeof(WORD)*bCardCount);
	}
	m_byCardCount=bCardCount;
	m_byCardCountView = byCardCountView;
	FixCardControl();
	return true;
}

//��ȡ������˿�
BYTE CUpGradeGameCard::GetUpCard(WORD wCardList[])
{
	BYTE bUpCount=0;
	for (BYTE i=0;i<m_byCardCount;i++)
	{
		if (m_wCardUpArray.GetAt(i))
		{
			if (wCardList!=NULL)
				wCardList[bUpCount]=m_wCardArray.GetAt(i);
			bUpCount++;
		}
	}
	return bUpCount;
}

//��ȡ�˿���Ϣ
BYTE CUpGradeGameCard::GetCard(WORD wCardList[], WORD wUpList[])
{
	if (wCardList!=NULL)
		CopyMemory(wCardList,m_wCardArray.GetData(),m_byCardCount*sizeof(WORD));
	if (wUpList!=NULL)
		CopyMemory(wUpList,m_wCardUpArray.GetData(),m_byCardCount*sizeof(WORD));
	return m_byCardCount;
}

//��������
BYTE CUpGradeGameCard::HitCardTest(CPoint & Point)
{
	if (m_bHorzLook==true)
	{
		//Ѱ�Ұ����˿�
		BYTE bCardIndex=(BYTE)(Point.x/m_iCardHSpace);
		if (bCardIndex>=m_byCardCount)
			bCardIndex=m_byCardCount-1;

		//�жϰ���λ��
		bool bHitThisCard=true;
		WORD wCardUp=m_wCardUpArray.GetAt(bCardIndex);
		if ((wCardUp==TRUE)&&(Point.y>m_iCardHight)) 
			bHitThisCard=false;
		if ((wCardUp==FALSE)&&(Point.y<m_iCardUpSpace))
			bHitThisCard=false;

		//Ѱ��ǰ������˿�
		if (bHitThisCard==false)
		{
			BYTE bFindCount=m_iCardWidth/m_iCardHSpace+1;
			for (BYTE i=1;i<bFindCount;i++)
			{
				if (bCardIndex==0)
					break;
				if(bCardIndex-i < 0)
					break;
				wCardUp=m_wCardUpArray.GetAt(bCardIndex-i);
				if ((wCardUp==FALSE)&&(Point.y>m_iCardWidth)) 
					return bCardIndex-i;
				if ((wCardUp==TRUE)&&(Point.y<m_iCardUpSpace))
					return bCardIndex-i;
			}
			return 255;
		}
		return bCardIndex;
	}
	return 255;
}

//��������(�Ƿ���ĳ�����Ϸ�)
BYTE CUpGradeGameCard::HitCardTestAbove(CPoint &Point)
{
	if (m_bHorzLook==true)
	{
		//Ѱ�Ұ����˿�
		BYTE bCardIndex=(BYTE)(Point.x/m_iCardHSpace);
		if (bCardIndex>=m_byCardCount)
			bCardIndex=m_byCardCount-1;
		
		//�жϰ���λ��
		bool bHitThisCard=true;
		WORD wCardUp=m_wCardUpArray.GetAt(bCardIndex);
		if ((wCardUp==TRUE)&&(Point.y>m_iCardHight)) 
			bHitThisCard=false;
		if ((wCardUp==FALSE)&&(Point.y>m_iCardHight))
			bHitThisCard=false;

		//Ѱ��ǰ������˿�
		if (bHitThisCard==false)
		{
			BYTE bFindCount=m_iCardWidth/m_iCardHSpace+1;
			for (BYTE i=1;i<bFindCount;i++)
			{
				if (bCardIndex==0) break;
				if(bCardIndex-i < 0)
					break;
				wCardUp=m_wCardUpArray.GetAt(bCardIndex-i);
				if ((wCardUp==FALSE)&&(Point.y>m_iCardWidth)) return bCardIndex-i;
				if ((wCardUp==TRUE)&&(Point.y<m_iCardUpSpace)) return bCardIndex-i;
			}
			return 255;
		}
		return bCardIndex;
	}
	return 255;
}

//ɾ��������˿�
BYTE CUpGradeGameCard::RemoveUpCard()
{
	for (BYTE i=0;i<m_byCardCount;i++)
	{
		if (m_wCardUpArray.GetAt(i)==TRUE)
		{
			m_byCardCount--;
			if (m_byCardCountView > 0)
			{
				m_byCardCountView --;
			}
			m_wCardArray.RemoveAt(i);
			m_wCardUpArray.RemoveAt(i);
			i--;
		}
	}
	FixCardControl();
	return m_byCardCount;
}

//��������λ��
void CUpGradeGameCard::FixCardControl()
{
	if (!m_hWnd)
		return;
	//��ȡλ��
	CSize Size;
	GetCardSize(Size);

	//����λ��
	CRect CardRect;
	switch (m_XShowMode)
	{
	case EXSM_LEFT: { CardRect.left=m_PtBasePoint.x; break; }
	case EXSM_CENTER: {	CardRect.left=m_PtBasePoint.x-Size.cx/2; break; }
	case EXSM_RIGHT: { CardRect.left=m_PtBasePoint.x-Size.cx; break; }
	}

	switch (m_YShowMode)
	{
	case EYSM_TOP: { CardRect.top=m_PtBasePoint.y; break; }
	case EYSM_CENTER: {	CardRect.top=m_PtBasePoint.y-Size.cy/2;	break; }
	case EYSM_BOTTOM: { CardRect.top=m_PtBasePoint.y-Size.cy; break; }
	}

	//�ƶ�λ��
	CardRect.right=CardRect.left+Size.cx;
	CardRect.bottom=CardRect.top+Size.cy;
	MoveWindow(&CardRect);
	Invalidate(FALSE);
	//��������
	CRgn AllRgn,SignedRgn;
	AllRgn.CreateRectRgn(0,0,0,0);
	for (BYTE i=0;i<m_byCardCount;i++)
	{
		if (m_bHorzLook==true)
		{
			WORD wUpCard=m_wCardUpArray.GetAt(i);
			if(m_bUseMouse)
			{
				SignedRgn.CreateRoundRectRgn(i*m_iCardHSpace,0,
					i*m_iCardHSpace+m_iCardWidth+1,m_iCardUpSpace+m_iCardHight+1,2,2);
				AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);

			}
			else
			{
				SignedRgn.CreateRoundRectRgn(i*m_iCardHSpace,(wUpCard==TRUE)?0:m_iCardUpSpace,
					i*m_iCardHSpace+m_iCardWidth+1,(wUpCard==TRUE)?m_iCardHight+1:m_iCardUpSpace+m_iCardHight+1,2,2);
				AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);

			}
			SignedRgn.DeleteObject();
		}
		else 
		{
			SignedRgn.CreateRoundRectRgn(0,i*m_iCardVSpace,m_iCardWidth+1,i*m_iCardVSpace+m_iCardHight+1,2,2);
			AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
			SignedRgn.DeleteObject();
		}
	}

	//��������
	SetWindowRgn(AllRgn,TRUE);
	AllRgn.DeleteObject();
	//�ػ�����
	Invalidate(TRUE);
	return;
}

//������Ϣ
int CUpGradeGameCard::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct)==-1) return -1;
	m_ToolTip.Create(this);
	m_ToolTip.SetDelayTime(ONE_SECOND);
	SetCardImage();
	SetCardWidthHeight(0,0);

	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	return 0;
}

//�ػ�����
void CUpGradeGameCard::OnPaint()
{
	if (NULL == m_hWnd)
	{
		return ;
	}

	CPaintDC dc(this);
	if (m_byCardCount==0)
	{
		return;
	}

	//��������
	CDC BufferDC;
	CSize CardSize;
	CBitmap BufferBmp;
	GetCardSize(CardSize);
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,CardSize.cx,CardSize.cy);
	CBitmap * pOldBmp=BufferDC.SelectObject(&BufferBmp);
	//���
	CGameImageHelper CardHandle(&m_CardPic[0]);
	CGameImageHelper BackHandle(&m_CardPic[1]);
	CGameImageHelper SmallHandle(&m_CardPic[2]);		//С��
	CGameImageHelper SmallBackHandle(&m_CardPic[3]);	//С�ƴ�Сè
	CGameImageHelper BkHandle(&m_bk);
	CGameImageHelper MoveSelectCardHandle(&m_CardPic[4]);
	CGameImageHelper MoveSelectBackHandle(&m_CardPic[5]);

	//�滭�˿�
	int iXPos=0,iYPos=0;
	CGameImageHelper * pActiveHandle=NULL;
	for(int i = 0; i < CardSize.cy; i += m_bk.GetHeight())
	{
		for(int j = 0; j < CardSize.cx; j += m_bk.GetWidth())
		{	
			BkHandle.BitBlt(BufferDC, j ,i);	
		}	
	}
	for (BYTE i=0;i<m_byCardCount;i++)
	{
		WORD wCard=m_wCardArray.GetAt(i);
		WORD wUpCard=m_wCardUpArray.GetAt(i);
		WORD wMoveSelect =m_wMoveSelectCardArray.GetAt(i);

		if ((m_bShowCard==true)&&wCard != 0)//(m_CardArray.GetAt(i)!=0))
		{
			BYTE byCard = (BYTE)(wCard&0x00FF);
			if ((byCard==0x0E)||(byCard==0x0F))
			{
				if(wMoveSelect==1)
				{
					iXPos=(byCard-0x0D)*m_iCardWidth;
					iYPos=0;
					pActiveHandle=&MoveSelectBackHandle;	
				}
				else if(IMAGE_SMALL == GetCardStyle() )
				{
					iXPos=(byCard-0x0D)*m_iCardWidth;
					iYPos=0;
					pActiveHandle=&SmallBackHandle;		
				}else
				{
					iXPos=(byCard-0x0D)*m_iCardWidth;
					iYPos=0;
					pActiveHandle=&BackHandle;		
				}
			}
			else
			{
				if(wMoveSelect==1)
				{
					pActiveHandle=&MoveSelectCardHandle;
					iXPos=GetCardXPos(wCard)*m_iCardWidth;
					iYPos=GetCardYPos(wCard)*m_iCardHight;
				}
				else if(IMAGE_SMALL == GetCardStyle() )
				{
					pActiveHandle=&SmallHandle;
					iXPos=GetCardXPos(wCard)*m_iCardWidth;
					iYPos=GetCardYPos(wCard)*m_iCardHight;
				}else
				{
					pActiveHandle=&CardHandle;
					iXPos=GetCardXPos(wCard)*m_iCardWidth;
					iYPos=GetCardYPos(wCard)*m_iCardHight;
				}
			}
		}
		else
		{
			iXPos=0;
			iYPos=0;
			if(wMoveSelect==1)
			{
				pActiveHandle=&MoveSelectBackHandle;
			}
			else if(IMAGE_SMALL == GetCardStyle() )
				pActiveHandle=&SmallBackHandle;
			else
				pActiveHandle=&BackHandle;
		}

		if (m_bHorzLook==true) 
			pActiveHandle->BitBlt(BufferDC,i*m_iCardHSpace,wUpCard?0:m_iCardUpSpace,m_iCardWidth,m_iCardHight,iXPos,iYPos);
		else 
			pActiveHandle->BitBlt(BufferDC,0,i*m_iCardVSpace,m_iCardWidth,m_iCardHight,iXPos,iYPos);
		//���ѡ����
		/*
		if(m_bMoveSel && i==m_CurCardIndex && !(m_CurCardIndex>= m_wStartIndex && m_CurCardIndex<=m_wEndIndex))
		{
		CRect rc;
		rc.left = i*m_iCardHSpace+4;
		rc.top = bUpCard?0:m_iCardUpSpace;
		rc.right = i*m_iCardHSpace+m_iCardHSpace;
		rc.bottom = rc.top+m_iCardHight;
		DrawTransparent(BufferDC.GetSafeHdc(), rc, RGB(125, 0, 125), 200);
		}*/
	}

	//ˢ�½���
	dc.BitBlt(0,0,CardSize.cx,CardSize.cy,&BufferDC,0,0,SRCCOPY);
	//͸������

	CRect rect,rect1;
	//���������
	rect.left=0;
	rect.right=CardSize.cx;
	if(m_bHorzLook)
		rect.top = CardSize.cy - m_iCardHight;
	else 
		rect.top =0;
	rect.bottom=CardSize.cy;

	switch(m_bShowNum)
	{
	case 1:
		if (m_bHorzLook==true) 		//С����
		{
			rect1.left =  rect.right - m_iCardWidth;
			rect1.top =  rect.top;
			rect1.bottom = rect.bottom;
			rect1.right = rect.right;
		}else
		{
			rect1.left =  rect.left ;
			rect1.top =  rect.bottom - m_iCardHight;
			rect1.bottom = rect.bottom ;
			rect1.right = rect.right;;
		}
		break;	
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
	int iNumWidth = 25,iNumHeight = 28;

	CDC NumDC;
	NumDC.CreateCompatibleDC(&dc);

	CGameImageHelper Num(&m_Num);
	NumDC.SelectObject(Num);
	// �˿���Ŀ(��m_byCardCountView>m_byCardCount ���� m_byCardCount>0ʱ��ʾm_byCardCountView)
	// ������ʾm_byCardCount
	int byCardCount=0;
	if ((m_byCardCountView>m_byCardCount) && (0 !=  m_byCardCount))
	{
		byCardCount = m_byCardCountView;
	}
	else
	{
		byCardCount=m_byCardCount;
	}

	// ֧��3λ��
	const int CARD_NUM_DIGIT = 3;

	// ��λ��ʮλ����λ
	int iCardNum[CARD_NUM_DIGIT]={(byCardCount/100),((byCardCount%100)/10),(byCardCount%10)};
	int iCount = 0;
	for (int i=0; i<CARD_NUM_DIGIT; ++i)
	{ 
		if (0 != iCardNum[i])
		{
			iCount = (CARD_NUM_DIGIT - i);
			break;
		}
	}

	int xoffset =(rect1.Width()-iCount*iNumWidth)/2;
	int yoffset = (rect1.Height()-iNumHeight)/2;
	if (m_bShowNum)
	{
		iCount = 0;
		for (int i=0; i<CARD_NUM_DIGIT; ++i)
		{ 
			if ((0 != iCardNum[i]) || (0 != iCount))
			{
				dc.TransparentBlt(rect1.left+xoffset+iCount*iNumWidth, rect1.top+yoffset,iNumWidth,iNumHeight,
					&NumDC,iCardNum[i]*iNumWidth,0,iNumWidth,iNumHeight,RGB(255,0,255));
				iCount ++;
			}
		}
	}
	//���ѡ����
	//ˢ�½���
	//if(m_bMultiSel)
	//{
	//	CRect rc(m_pStartPos.x, m_pStartPos.y, m_pEndPos.x, m_pEndPos.y);
	//	if(abs(rc.top -rc.bottom) < 2)
	//		rc.bottom = rc.top+2;
	//	DrawTransparent(dc.GetSafeHdc(), rc, RGB(125, 0, 125), 200);
	//	//dc.FillSolidRect(&rc, RGB(255, 0, 0));
	//}
	//������Դ
	BufferDC.SelectObject(pOldBmp);
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	return;
}

//����
int CUpGradeGameCard::GetPoint()
{
	if(m_iSourcePoint && m_iBackPoint && m_iBackMultiple)
		return m_iSourcePoint +m_iBackPoint * m_iBackMultiple;
	int score = 0;
	for (BYTE i=0;i<m_byCardCount;i++)
	{
		WORD wCard=m_wCardArray.GetAt(i);
		if(GetCardValue(wCard) == 5 )
		score +=5;
		if(GetCardValue(wCard) == 10||GetCardValue( wCard ) == 13 )
			score +=10;
	}
	return score;
}
//�����Ϣ
void CUpGradeGameCard::OnLButtonDown(UINT nFlags, CPoint Point)
{
    if (m_bUseMouse==true)
    {
        m_ToolTip.AddTool(this,"�������ϵ��������ƣ������������Ҽ�����");
        m_ToolTip.Activate(TRUE);

        if (m_bHorzLook==true)
        {
            m_wHotHitCard=HitCardTest(Point);
            if(m_wHotHitCard == 255)
                return ;
            //Ѱ�Ұ����˿�
            WORD wCardUp=m_wCardUpArray.GetAt(m_wHotHitCard);
            bool AllUp = true;
            for(int i=0; i<m_wCardUpArray.GetCount(); i++)
            {
                if(!m_wCardUpArray.GetAt(i))
                {
                    AllUp = false;
                    break;
                }
            }
            if(!AllUp && wCardUp && Point.y<m_iCardUpSpace+m_iCardHight)//2008-08-20����m_iCardHight
            {
                m_bMoveSel = true;
                //			m_bLDown = true;
                m_pMovePos = Point;

                m_wCanUp = wCardUp;
                if (!wCardUp)
                {
                    m_wMoveFirstCard = m_wHotHitCard;
                    m_wCardUpArray.SetAt(m_wHotHitCard,1);
                }
                m_bMove = false;
                m_CurCardIndex = -1;
                m_wStartIndex = m_wHotHitCard;
                for(int i=m_wHotHitCard; i>=0; i--)
                {
                    if(m_wCardUpArray.GetAt(i))
                    {
                        m_wStartIndex = i;
                        continue;
                    }
                }
                m_wEndIndex = m_wHotHitCard;
                for(int i=m_wHotHitCard+1; i<m_byCardCount; i++)
                {
                    if(m_wCardUpArray.GetAt(i))
                    {
                        m_wEndIndex = i;
                        continue;
                    }
                }
            }
            else
            {
                m_pStartPos = Point;
                m_bMultiSel = true;
            }
            SetCapture();
        }
    }
	return;
}
//paramenter iSourcePoint Դ����
//paramenter iBackPoint		�׷���
//paramenter iBackMultiple	�̵ױ���
//�������÷�����
BOOL CUpGradeGameCard::SetBackPointAndMultiple(int iSourcePoint,int iBackPoint,int iBackMultiple)
{
	m_iSourcePoint = iSourcePoint;
	m_iBackPoint = iBackPoint;
	m_iBackMultiple = iBackMultiple;
	return true;
}
//�����Ϣ
void CUpGradeGameCard::OnLButtonUp(UINT nFlags, CPoint Point)
{
//	m_MouseMove = false;
//	m_bLDown = false;
	if (m_bUseMouse==true)
	{
		if(m_bMultiSel)
		{
			m_bMultiSel = false;
			ReleaseCapture();
			m_pEndPos = Point;
			if(m_pEndPos.x < 0)
				m_pEndPos.x = 0;
			int sindex = GetCardIndex(m_pStartPos);
			int eindex = GetCardIndex(m_pEndPos);
			bool bDiffer = false;
//			bool bUp = m_CardUpArray.GetAt(sindex);
			if(sindex > eindex)
			{
				int temp = sindex;
				sindex = eindex;
				eindex = temp;
			}
			/*
			for(int i=sindex; i<=eindex; i++)
			{
				if(bUp != m_CardUpArray.GetAt(i))
				{
					bDiffer = true;
					break;
				}
			}
			if(!bDiffer)
				bUp = !bUp;
				*/
			for(int i=sindex; i<=eindex; i++)
			{
				//m_CardUpArray.SetAt(i, bUp?TRUE:FALSE);
				m_wCardUpArray.SetAt(i, TRUE);
			}
			
			for(int i=0; i<m_byCardCount; i++)
			{
				m_wMoveSelectCardArray.SetAt(i,false);
			}

			FixCardControl();
		}
		else if(m_bMoveSel)
		{
			m_bMoveSel = false;
			if (!m_wCanUp)
				m_wCardUpArray.SetAt(m_wMoveFirstCard,0);
			ReleaseCapture();
			CRect rc;
			GetClientRect(&rc);
			if(Point.x < -40)
				m_CurCardIndex = 255;
			else if(Point.x <0)
			{
				Point.x = 0;
				m_CurCardIndex = -1;
			}
			else
			{
				m_CurCardIndex = Point.x/m_iCardHSpace;
			}

			if(Point.x > (rc.right+20))
				m_CurCardIndex = 255;
			if(Point.y < -20)
				m_CurCardIndex = 255;
			if(Point.y > (rc.bottom+/*20*/m_iCardHight))//2008-08-20
				m_CurCardIndex = 255;

			if (m_CurCardIndex!=255 && m_CurCardIndex >= m_byCardCount)
			{
				m_CurCardIndex = m_byCardCount-1;
			}
			if(!m_bMove && m_wHotHitCard==HitCardTest(Point) && m_CurCardIndex!=255)
			{
				m_wCardUpArray.SetAt(m_wHotHitCard, !m_wCardUpArray.GetAt(m_wHotHitCard));
			}	
			//���ƽ�����ָ�����״̬
			if(m_bMove)
			{
				for(int i=0; i<m_byCardCount; i++)
				{
					//m_CardUpArray.SetAt(i, bUp?TRUE:FALSE);
					m_wCardUpArray.SetAt(i, false);
				}
			}
/*			else if(m_CurCardIndex!=255)
			{
				BYTE card[100];
				BYTE up[100];
				bool bUp = false;
				if(m_CurCardIndex == -1)
					bUp = m_CardUpArray.GetAt(0);
				else
				{
					 bUp = m_CardUpArray.GetAt(m_CurCardIndex);
				}
				int cur = 0;

				if(m_CurCardIndex < m_wStartIndex)//����
				{

					for(int i=0; i<=m_CurCardIndex; i++)//�ƶ���֮ǰδ������
					{
						card[cur] = m_CardArray.GetAt(i);
						up[cur] = m_CardUpArray.GetAt(i);
						cur++;
					}
					
					for(int i = m_CurCardIndex+1; i < m_wStartIndex; i ++)//����֮ǰ�������������
					{
						if(m_CardUpArray.GetAt(i))
						{
							card[cur] = m_CardArray.GetAt(i);
							up[cur] = bUp;
							cur++;
						}
					}

					for(int i=m_wStartIndex; i<=m_wEndIndex; i++)//��������������
					{
						card[cur] = m_CardArray.GetAt(i);
						up[cur] = bUp;
						cur++;
					}
					for(int i=m_wEndIndex+1; i<m_byCardCount; i++)//�ƶ���֮��������������
					{
						if(m_CardUpArray.GetAt(i))
						{
							card[cur] = m_CardArray.GetAt(i);
							up[cur] = bUp;
							cur++;
						}
					}
					for(int i=m_CurCardIndex+1; i<m_wStartIndex; i++)//��ԭ�ƶ�����֮�󲹳�����
					{	
						if(m_CardUpArray.GetAt(i))
							continue;
						card[cur] = m_CardArray.GetAt(i);
						up[cur] = m_CardUpArray.GetAt(i);
						cur++;
					}
					for(int i=m_wEndIndex+1; i<m_byCardCount; i++)//�ƶ���֮��δ������
					{
						if(m_CardUpArray.GetAt(i))
							continue;
						card[cur] = m_CardArray.GetAt(i);
						up[cur] = m_CardUpArray.GetAt(i);
						cur++;
					}
				}
				else if(m_CurCardIndex > m_wEndIndex)//����
				{
					for(int i=0; i<m_wStartIndex; i++)//�ƶ���֮ǰδ������������
					{
						if(m_CardUpArray.GetAt(i))
							continue;
						card[cur] = m_CardArray.GetAt(i);
						up[cur] = m_CardUpArray.GetAt(i);
						cur++;
					}
					for(int i=m_wEndIndex+1; i<=m_CurCardIndex; i++)//�ƶ���֮��δ����������
					{
						if(m_CardUpArray.GetAt(i))
							continue;
						card[cur] = m_CardArray.GetAt(i);
						up[cur] = m_CardUpArray.GetAt(i);
						cur++;
					}
					for(int i=0; i<m_wStartIndex; i++)//�ƶ���֮ǰ������������
					{
						if(m_CardUpArray.GetAt(i))
						{
							card[cur] = m_CardArray.GetAt(i);
							up[cur] = bUp;
							cur++;
						}
					}

					for(int i=m_wStartIndex; i<=m_wEndIndex; i++)//�ƶ�������������
					{
						card[cur] = m_CardArray.GetAt(i);
						up[cur] = bUp;
						cur++;
					}
					for(int i=m_wEndIndex+1; i<=m_CurCardIndex; i++)//�ƶ���֮������������
					{
						if(m_CardUpArray.GetAt(i))
						{
							card[cur] = m_CardArray.GetAt(i);
							up[cur] = bUp;
							cur++;
						}
					}
					for(int i=m_CurCardIndex+1; i<m_byCardCount; i++)//�ƶ����δ������
					{
						card[cur] = m_CardArray.GetAt(i);
						up[cur] = m_CardUpArray.GetAt(i);
						cur++;
					}
				}
				else
				{
					goto end;//return ;
				}
				CopyMemory(m_CardArray.GetData(),card,sizeof(BYTE)*m_byCardCount);
				CopyMemory(m_CardUpArray.GetData(),up,sizeof(BYTE)*m_byCardCount);
			}*/
			FixCardControl();
		}
	}
//end:
	//BYTE bHitCard=HitCardTest(Point);
	if ((m_bUseMouse==true /*&& bHitCard != 255*/)&&(m_pHandleWnd!=NULL)) 
		m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD,0,0);
	if ((m_bUseMouse==true) &&(m_pHandleWnd!=NULL))
		m_pHandleWnd->SendMessage(IDM_MOUSE_UP,0,0);
	return ;
}
//�����Ϣ
void CUpGradeGameCard::OnRButtonUp(UINT nFlags, CPoint point)
{
    if (NULL != m_pHandleWnd)
    {
        ((CClientGameDlg*)m_pHandleWnd)->m_PlayView.m_btnOutCard.EnableWindow(FALSE);
        if (m_bUseMouse) 
        {
            for(int i=0; i<m_byCardCount; i++)
            {
                m_wCardUpArray.SetAt(i,false);
            }
        }
        FixCardControl();
    }
	return;
}
void CUpGradeGameCard::UnUpAllCard()
{
	if (NULL != m_pHandleWnd)
	{
		((CClientGameDlg*)m_pHandleWnd)->m_PlayView.m_btnOutCard.EnableWindow(0);
		if (m_bUseMouse) 
		{
			for(int i=0; i<m_byCardCount; i++)
			{
				m_wCardUpArray.SetAt(i,false);
			}
		}
		FixCardControl();
	}

}

//�����Ϣ(������϶�,���Խ������������ƶ����ұ�)
void CUpGradeGameCard::OnMouseMove(UINT nFlags, CPoint Point)
{
	if (m_bUseMouse==true)
	{
		if(m_bMultiSel)
		{
			m_pEndPos = Point;
			if(m_pEndPos.x < 0)
				m_pEndPos.x = 0;

			int sindex = GetCardIndex(m_pStartPos);
			int eindex = GetCardIndex(m_pEndPos);
			
			if(sindex > eindex)
			{
				int temp = sindex;
				sindex = eindex;
				eindex = temp;
			}			

			for(int i=0; i<m_byCardCount; i++)
			{
				m_wMoveSelectCardArray.SetAt(i,false);
			}

			for(int i=sindex; i<=eindex; i++)
			{
				m_wMoveSelectCardArray.SetAt(i,true);
			}

			FixCardControl();
			UpdateWindow();
		}
		else if(m_bMoveSel)
		{
			CRect rc;
			GetClientRect(&rc);
			if(Point.x < -40)
				m_CurCardIndex = 255;
			else if(Point.x <0)
			{
				Point.x = 0;
				m_CurCardIndex = -1;
			}
			else
			{
				m_CurCardIndex = Point.x/m_iCardHSpace;
			}

			if(Point.x > (rc.right+20))
				m_CurCardIndex = 255;

			if(Point.y < -20)
				m_CurCardIndex = 255;
			if(Point.y > (rc.bottom+/*20*/m_iCardHight))//2008-08-20
				m_CurCardIndex = 255;

			if (m_CurCardIndex!=255 && m_CurCardIndex >= m_byCardCount)
			{
				m_CurCardIndex = m_byCardCount-1;
			}

			//if(m_CurCardIndex>= m_wStartIndex && m_CurCardIndex<=m_wEndIndex)
			//	m_CurCardIndex = 255;

			if(m_CurCardIndex == 255)
			{
				//SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
			else
			{
				m_bMove = true;
				//SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
				MouseMoveAjust(Point);
			}		
			Invalidate();
		}
else if(nFlags&MK_LBUTTON)
		{
			m_bMultiSel = true;
			m_pStartPos = Point;
			BYTE bCardIndex=(BYTE)(Point.x/m_iCardHSpace);
			if(bCardIndex >=m_byCardCount)
				bCardIndex = m_byCardCount-1;
			if (bCardIndex == m_byCardCount-1)
			{
				CRect rc;
				GetClientRect(&rc);
				m_pStartPos.x = rc.right-2;
			}
			if(bCardIndex == 0)
				m_pStartPos.x = 2;
			if((bCardIndex>0 && bCardIndex<m_byCardCount-1) && m_pStartPos.y>20)
				m_pStartPos.x = 2;
			SetCapture();
		}
	}
	CWnd::OnMouseMove(nFlags, Point);

}

//���Ʒ�ת
BOOL CUpGradeGameCard::Convert(WORD wCardList[],int iCardCount)
{
	for(int i = 0; i< iCardCount/2 ;i++)
	{
		WORD wCard = wCardList[i];
		wCardList[i] = wCardList[iCardCount-1];
		wCardList[iCardCount-1] = wCard;
	}
	return TRUE;
}

//�ƶ���λ��,��Դλ���ƶ���Ŀ��λ��
BOOL CUpGradeGameCard::ChangeIndexArray(BYTE DestIndex ,BYTE SrcIndex)
{
	WORD wMoveBlock[100]={0};
	int k = 0;
	int index = -1;						 //��һ�ŵ�λ��
	//������������
	for(int i = SrcIndex - 1;i > 0; i --)//Դ��߼��������Ƽ�����ǰ��
	{	//������������ȫ������
		if(m_wCardUpArray.GetAt(i) == TRUE)
		{
			if(index == -1)
				index = i;
			wMoveBlock[k++] = m_wCardArray.GetAt(i);
		}
		else 
			break;
	}
	//ǰ����
	if(index == -1)
		index = SrcIndex;
	else
	{
		//��ת
		Convert(wMoveBlock,k);
	}

	wMoveBlock[k++] = m_wCardArray.GetAt(SrcIndex);

	for(int i = SrcIndex+1;i < m_byCardCount; i ++)//Դ�ұ߼��������Ƽ����ں���
	{	//������������ȫ������
		if(m_wCardUpArray.GetAt(i) == TRUE)
		{
			wMoveBlock[k++] = m_wCardArray.GetAt(i);
		}
		else 
			break;
	}

	//Դ��Ŀ��֮���ֵ���������ƶ�
	if(DestIndex > SrcIndex)//����
	{
		for(int i = index ; i < index + k; i ++)					//�ճ�λ��
		{
			if(i + k  >= m_byCardCount) 
				break;
			m_wCardArray.SetAt(i,m_wCardArray.GetAt(i + k ));
			m_wCardUpArray.SetAt(i,m_wCardUpArray.GetAt(i + k ));
		}
		for(int i = DestIndex - k + 1; i <= DestIndex; i ++)				//�����������Ƽ���
		{
			if(i - (DestIndex - k + 1) < 0 ) 
				break;
			m_wCardArray.SetAt(i,wMoveBlock[i - (DestIndex - k + 1)]);
			m_wCardUpArray.SetAt(i,1);
		}
	}
	else if (DestIndex < SrcIndex)//����
	{
		for(int i = index + k - 1 ; i > index - 1 ;i--)				//�ճ�λ��
		{
			if( i - k < 0)
				break;
			m_wCardArray.SetAt(i,m_wCardArray.GetAt(i - k));
			m_wCardUpArray.SetAt(i,m_wCardUpArray.GetAt(i - k));
		}
		for(int i = DestIndex; i < DestIndex + k ; i ++)			//�����������Ƽ���
		{
			if(i - DestIndex < 0)
				break;
			m_wCardArray.SetAt(i,wMoveBlock[i - DestIndex]);
			m_wCardUpArray.SetAt(i,1);
		}
	}
	//�ػ�
	FixCardControl();
	if ((m_bUseMouse==true /*&& bHitCard != 255*/)&&(m_pHandleWnd!=NULL)) 
		m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD,0,0);
	return TRUE;
}

//�ƶ���λ��,��Դλ���ƶ���Ŀ��λ��
BOOL CUpGradeGameCard::ChangeIndex(BYTE DestIndex ,BYTE SrcIndex)
{
	WORD wTemp = m_wCardArray.GetAt(SrcIndex);
	WORD wTempup = m_wCardUpArray.GetAt(SrcIndex);

	//Դ��Ŀ��֮���ֵ���������ƶ�
	if(DestIndex > SrcIndex)								//����
	{
		for(int i = SrcIndex ; i < DestIndex ;i++)
		{
			if(i + 1 >= m_byCardCount )
				break;
			m_wCardArray.SetAt(i,m_wCardArray.GetAt(i + 1));
			m_wCardUpArray.SetAt(i,m_wCardUpArray.GetAt(i + 1));
		}
	}else if (DestIndex < SrcIndex)							//����
	{
		for(int i = SrcIndex ; i > DestIndex ;i--)
		{
			if(i - 1 < 0)
				break;
			m_wCardArray.SetAt(i,m_wCardArray.GetAt(i - 1));
			m_wCardUpArray.SetAt(i,m_wCardUpArray.GetAt(i - 1));
		}
	}
	m_wCardArray.SetAt(DestIndex,wTemp);
	m_wCardUpArray.SetAt(DestIndex,wTempup);

	//�ػ�
	FixCardControl();
	return TRUE;
}

//��������
BOOL CUpGradeGameCard::SetFriendArray(WORD wCardList[], WORD wUpList[], BYTE bCardCount)
{
	m_wCardFriendArray.SetSize(bCardCount);
	if (wCardList!=NULL) CopyMemory(m_wCardFriendArray.GetData(),wCardList,sizeof(WORD)*bCardCount);
	m_bFriendCardCount=bCardCount;
	return TRUE;
}
//�����������в���ĳ���Ƶ���Ŀ
BYTE CUpGradeGameCard::FindACardNumInFriendList(BYTE iCardNum)
{
	int FriendCount=0;
	if(m_bFriendCardCount>0&&m_byCardCount>0)
	{
		for(int i=0;i<m_bFriendCardCount;i++)
		{
			if(GetCardNum(m_wCardFriendArray.GetAt(i))==iCardNum)
				FriendCount++;
		}	
	}
	return FriendCount;
}
//���õ���ͼƬ
void CUpGradeGameCard::SetCardImage()
{
	TCHAR path[MAX_PATH]={0};
	switch(GetCardStyle())
	{
	case 2:
	case IMAGE_SMALL:
			wsprintf(path,TEXT("%s\\game_CARD_SMALL.bmp"),SKIN_CARD);
			m_CardPic[2].SetLoadInfo(path,CGameImageLink::m_bAutoLock);			
			wsprintf(path,TEXT("%s\\game_CARD_SMALL_back.bmp"),SKIN_CARD);
			m_CardPic[3].SetLoadInfo(path,CGameImageLink::m_bAutoLock);		
		break;
	case IMAGE_VERY_SMALL:
			wsprintf(path,TEXT("%s\\very_small.bmp"),SKIN_CARD);
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		break;
	default:
		if(GetSystemMetrics(SM_CXSCREEN)>=1024)
		{
			wsprintf(path,TEXT("%s\\game_CARD.bmp"),SKIN_CARD);
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);			
			wsprintf(path,TEXT("%s\\game_CARD_back.bmp"),SKIN_CARD);
			m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			//ѡ�����ͼ
			wsprintf(path,TEXT("image\\%s\\GAME_CARD_SELECT.bmp"),SKIN_CARD);
			m_CardPic[4].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			wsprintf(path,TEXT("%s\\game_CARD_BACK_SELECT.bmp"),SKIN_CARD);
			m_CardPic[5].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		}
		else
		{
			wsprintf(path,TEXT("%s\\game_CARD_800.bmp"),SKIN_CARD);
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);			
			wsprintf(path,TEXT("%s\\game_CARD_800_back.bmp"),SKIN_CARD);
			m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			
			//ѡ�����ͼ
			wsprintf(path,TEXT("%s\\Game_Card_800_SELECT.bmp"),SKIN_CARD);
			m_CardPic[4].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			wsprintf(path,TEXT("%s\\game_card_800_back_SELECT.bmp"),SKIN_CARD);
			m_CardPic[5].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		}
		break;
	}
	wsprintf(path,TEXT("image\\cardnum.bmp"),SKIN_CARD);
	m_Num.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,TEXT("image\\BackColorOnly.bmp"),SKIN_FOLDER);
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);	
	return ;
}
//�����˿˸ߺͿ�
void  CUpGradeGameCard::SetCardWidthHeight(int iWidth,int iHeight)
{
	switch(GetCardStyle())
	{
	case IMAGE_SMALL:
		m_iCardHight=57;
		m_iCardWidth=42;
		break;
	case IMAGE_VERY_SMALL:
		m_iCardHight=12;
		m_iCardWidth=25;
		break;
	default:
		if(GetSystemMetrics(SM_CXSCREEN)>=1024)
		{	
			m_iCardHight=95;
			m_iCardWidth=70;	
		}
		else
		{
			m_iCardHight=73;
			m_iCardWidth=54;
		}
	}
	return ;
}

// ����͸����
void CUpGradeGameCard::DrawTransparent(HDC destdc, CRect &rc, COLORREF color, int Alpha)
{
	int temp;
	if(rc.left > rc.right)
	{
		temp = rc.left;
		rc.left = rc.right;
		rc.right = temp;
	}
	if(rc.top > rc.bottom)
	{
		temp = rc.top;
		rc.top = rc.bottom;
		rc.bottom = temp;
	}
	int cx = rc.Width();
	int cy = rc.Height();
	int dx = rc.left;
	int dy = rc.top;
	HDC dc;
	dc = CreateCompatibleDC(destdc);
	HBRUSH brush = CreateSolidBrush(TRANSPARENT);
	HBITMAP bitmap;
	bitmap = CreateCompatibleBitmap(destdc,cx,cy);
	SelectObject(dc,bitmap);
	CRect rect(0, 0, cx, cy);
	::FillRect(dc, &rect, brush);

	BLENDFUNCTION bl;
	bl.BlendOp = AC_SRC_OVER;
	bl.BlendFlags = 0;
	bl.SourceConstantAlpha = Alpha;
	bl.AlphaFormat = 0; 
	AlphaBlend(destdc,dx,dy,cx,cy,dc,0,0,cx,cy,bl);
	DeleteObject(bitmap);
	DeleteDC(dc);
	DeleteObject(brush);
}

// �õ���index
BYTE CUpGradeGameCard::GetCardIndex(CPoint &Point)
{
	BYTE index = HitCardTest(Point);
	if(index == 255)
	{
		index=(BYTE)(Point.x/m_iCardHSpace);
		if (index!=255 && index>=m_byCardCount)
			index=m_byCardCount-1;
	}
	return index;
}
////�����ƶ���
void CUpGradeGameCard::MouseMoveAjust(CPoint Point)
{
	if(m_bMoveSel && m_CurCardIndex>=0 && m_CurCardIndex<m_byCardCount && m_CurCardIndex>=0 && m_CurCardIndex!=m_wHotHitCard)
	{
		WORD card[100]={0};
		WORD up[100]={0};
		int cur = 0;
		if(m_CurCardIndex==0)//����ڵ�һ����λ��ʱ�Ĵ���
		{			
			//�����м䲿��
			for(int i=0;i<m_byCardCount;i++)
			{
				if(m_wCardUpArray.GetAt(i))
				{
					card[cur] = m_wCardArray.GetAt(i);
					up[cur] = m_wCardUpArray.GetAt(i);
					cur++;
				}
			}
			//������沿��
			for(int i=0;i<m_byCardCount;i++)
			{
				if(!m_wCardUpArray.GetAt(i))
				{
					card[cur] = m_wCardArray.GetAt(i);
					up[cur] = m_wCardUpArray.GetAt(i);
					cur++;
				}
			}
		}
		else if(m_CurCardIndex==(m_byCardCount-1))//��������һ����λ��ʱ�Ĵ���
		{			
			//������沿��
			for(int i=0;i<m_byCardCount;i++)
			{
				if(!m_wCardUpArray.GetAt(i))
				{
					card[cur] = m_wCardArray.GetAt(i);
					up[cur] = m_wCardUpArray.GetAt(i);
					cur++;
				}
			}
			//�����м䲿��
			for(int i=0;i<m_byCardCount;i++)
			{
				if(m_wCardUpArray.GetAt(i))
				{
					card[cur] = m_wCardArray.GetAt(i);
					up[cur] = m_wCardUpArray.GetAt(i);
					cur++;
				}
			}
		}
		else //�����һ��λ��ʱ�Ĵ���
		{						
			if(m_CurCardIndex>m_wHotHitCard)
			{
				//���ƶ�
				//����ǰ����
				for(int i=0;i<=m_CurCardIndex;i++)
				{
					if(!m_wCardUpArray.GetAt(i))
					{
						card[cur] = m_wCardArray.GetAt(i);
						up[cur] = m_wCardUpArray.GetAt(i);
						cur++;
					}
				}
				//�����м䲿��
				for(int i=0;i<m_byCardCount;i++)
				{
					if(m_wCardUpArray.GetAt(i))
					{
						card[cur] = m_wCardArray.GetAt(i);
						up[cur] = m_wCardUpArray.GetAt(i);
						cur++;
					}
				}
				//������沿��
				for(int i=m_CurCardIndex+1;i<m_byCardCount;i++)
				{
					if(!m_wCardUpArray.GetAt(i))
					{
						card[cur] = m_wCardArray.GetAt(i);
						up[cur] = m_wCardUpArray.GetAt(i);
						cur++;
					}
				}
			}
			else if(m_CurCardIndex<m_wHotHitCard)
			{
				//���ƶ�
				//����ǰ����
				for(int i=0;i<m_CurCardIndex;i++)
				{
					if(!m_wCardUpArray.GetAt(i))
					{
						card[cur] = m_wCardArray.GetAt(i);
						up[cur] = m_wCardUpArray.GetAt(i);
						cur++;
					}
				}
				//�����м䲿��
				for(int i=0;i<m_byCardCount;i++)
				{
					if(m_wCardUpArray.GetAt(i))
					{
						card[cur] = m_wCardArray.GetAt(i);
						up[cur] = m_wCardUpArray.GetAt(i);
						cur++;
					}
				}
				//������沿��
				for(int i=m_CurCardIndex;i<m_byCardCount;i++)
				{
					if(!m_wCardUpArray.GetAt(i))
					{
						card[cur] = m_wCardArray.GetAt(i);
						up[cur] = m_wCardUpArray.GetAt(i);
						cur++;
					}
				}
			}
		}
		//�������ƿ�ʼλ��
		m_wStartIndex=m_wHotHitCard;
		for(int i=0; i<m_byCardCount; i++)
		{
			if(up[i])
			{
				m_wStartIndex = i;
				break;
			}
		}
		//�������ƽ���λ��
		for(int i=m_wStartIndex+1; i<m_byCardCount; i++)
		{
			if(up[i])
			{
				m_wEndIndex = i;
				continue;
			}
		}		
		//���ü�����ƣ����������ݣ�ˢ����ʾ
		m_wHotHitCard=m_CurCardIndex;
		CopyMemory(m_wCardArray.GetData(),card,sizeof(WORD)*m_byCardCount);
		CopyMemory(m_wCardUpArray.GetData(),up,sizeof(WORD)*m_byCardCount);
		FixCardControl();
		UpdateWindow();
	}	
	else
	{
		return ;
	}
	return;
}

BOOL CUpGradeGameCard::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_ToolTip.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}

void CUpGradeGameCard::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (ID_TIMER_MOVE_CARD == nIDEvent)
	{
		// �����������ƶ�
		if (NULL == m_hWnd)
		{
			return ;
		}
		CRect rect;
		GetClientRect(&rect);
		m_lMoveCount --;
		CPoint ptMoveTo(0,0);
		if (m_lMoveCount < 1)
		{
			m_ptStart = m_ptEnd;
		}
		else
		{
			ptMoveTo = m_ptEnd - m_ptStart;
			if (0 != ptMoveTo.x)
			{
				long lTmp = ptMoveTo.x/m_lMoveCount;
				if (0 == lTmp)
				{
					lTmp = (ptMoveTo.x>0)?1:-1;
				}
				else
				{
					ptMoveTo.x = lTmp;
				}
			}
			if (0 != ptMoveTo.y)
			{
				long lTmp = ptMoveTo.y/m_lMoveCount;
				if (0 == lTmp)
				{
					lTmp = (ptMoveTo.y>0)?1:-1;
				}
				else
				{
					ptMoveTo.y = lTmp;
				}
			}
			m_ptStart += ptMoveTo;			
		}
		if (m_ptStart == m_ptEnd)
		{
			KillTimer(ID_TIMER_MOVE_CARD);
		}
		SetBasePoint(m_ptStart, m_XShowMode, m_YShowMode);
	}

	CWnd::OnTimer(nIDEvent);
}

// �ƶ�
void CUpGradeGameCard::MoveAnimal(CPoint &ptStart, CPoint &ptEnd, long lTime)
{
	SetBasePoint(ptStart, m_XShowMode, m_YShowMode);
	m_ptStart = ptStart;         // ���
	m_ptEnd = ptEnd;             // �յ�
	m_lMoveCount = lTime/MOVE_CARD_TIME_INTERVAL; // �ƶ�����
	if (0 == m_lMoveCount)
	{
		m_lMoveCount = 1;
	}
	SetTimer(ID_TIMER_MOVE_CARD, MOVE_CARD_TIME_INTERVAL, NULL);
}
void CUpGradeGameCard::StopMove(void)
{
	KillTimer(ID_TIMER_MOVE_CARD);
}
void CUpGradeGameCard::GetBasePoint(CPoint &ptBasePoint)
{
	ptBasePoint = m_PtBasePoint;
}