#pragma once
#include <list>
#include "StdAfx.h"
#define lengthof(x) (sizeof(x)/sizeof(*x))
using namespace std;

namespace BzDui 
{
	class CMarqueeTextUI : public CTextUI
	{
	public:
		static const UINT m_timer_ad = 2000;//������ƶ���ʱ��
		static const UINT m_timer_ad2 = 2001;//������ƶ���ʱ��

		explicit CMarqueeTextUI()
		{
			m_ad_time = 50;//�ƶ�Ƶ��
			m_list_max = 10;//��׼��list�������
			m_ListCString_marquee.clear();
			m_bStop = false;
			m_dwHotTextColor = 0x0000ff00;
			m_bIsInit = false;

			SetBkColor(0xFF000000);
		}

		CMarqueeTextUI(RECT rcPos)
		{
			m_ad_time = 50;//�ƶ�Ƶ��
			m_list_max = 10;//��׼��list�������
			m_ListCString_marquee.clear();
			m_bStop = false;
			m_dwHotTextColor = 0x00000000;

			SIZE szXY = {rcPos.left >= 0 ? rcPos.left : rcPos.right, rcPos.top >= 0 ? rcPos.top : rcPos.bottom};
			
			SetFixedXY(szXY);
			SetFixedWidth(rcPos.right - rcPos.left);
			SetFixedHeight(rcPos.bottom - rcPos.top);
		}

		~CMarqueeTextUI()
		{
			m_pManager->KillTimer(this, m_timer_ad);
			m_ListCString_marquee.clear();
		}

		virtual void SetVisible(bool bVisible = true)
		{
			CTextUI::SetVisible(bVisible);
			if (!bVisible)
			{
				m_pManager->KillTimer(this, m_timer_ad);
			}
			else
			{
				m_pManager->KillTimer(this, m_timer_ad);
				//���������
				m_pManager->SetTimer(this, m_timer_ad, m_ad_time);
			}
		}

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if ( _tcscmp(pstrName, _T("frequency")) == 0 )//�ƶ�Ƶ��
			{
				m_ad_time = _ttoi(pstrValue);
			}
			else if ( _tcscmp(pstrName, _T("listmax")) == 0 )//��׼��list�������
			{
				m_list_max = _ttoi(pstrValue);
			}
			else if ( _tcscmp(pstrName, _T("hottextcolor")) == 0 )//��긡������������ɫ
			{
				m_dwHotTextColor = _ttoi(pstrValue);
			}
			CTextUI::SetAttribute(pstrName, pstrValue);
		}

		virtual void DoEvent(TEventUI& event)
		{
			if( event.Type == UIEVENT_MOUSEMOVE)
			{
				if(PtInRect(&m_addrawrect, event.ptMouse))
				{
					Invalidate();
					m_pManager->SendNotify(this, _T("mouseenter"), event.wParam, event.lParam);
				}
				else
				{
					Invalidate();
					m_pManager->SendNotify(this, _T("mouseleave"), event.wParam, event.lParam);
				}
			}
			if( event.Type == UIEVENT_MOUSEENTER )
			{
				if(PtInRect(&m_addrawrect, event.ptMouse))
				{
					Invalidate();
					m_pManager->SendNotify(this, _T("mouseenter"), event.wParam, event.lParam);
				}
			}
			if( event.Type == UIEVENT_MOUSELEAVE )
			{
				Invalidate();
				m_pManager->SendNotify(this, _T("mouseleave"), event.wParam, event.lParam);
			}
			if(event.Type == UIEVENT_TIMER)
			{
				if (event.wParam == m_timer_ad2)
				{
					m_pManager->KillTimer(this, m_timer_ad2);
					m_sText = m_sText2;
					Update_marquee();
				}
				else if (event.wParam == m_timer_ad)
				{
					UINT step = 3;//����
					static UINT tag = 0;
					if(!m_bStop)
					{
						if (tag == 0)
						{
							m_addrawrect.left-=step;
							m_addrawrect.right=m_addrawrect.left+/*m_UIMarAdsize.cx+500*/m_textLen;

							if(m_addrawrect.right<=GetPos().left)//����һ��
 							{
								//m_ListCString_marquee.pop_front();//�������ʾ����
								m_ListCString_marquee.push_back(m_ListCString_marquee.front());
								m_ListCString_marquee.pop_front();

								if(!m_ListCString_marquee.empty())
								{
									m_sText = m_ListCString_marquee.front();
									TCHAR ad[MAX_PATH] = {0};
									wsprintf(ad, "%s", m_sText);
									HDC hdc=::GetDC(NULL);
									GetTextExtentPoint32(hdc, ad, lstrlen(ad), &m_UIMarAdsize);
								}
								else//û��������
								{
									m_sText.Empty();
									m_UIMarAdsize.cx = 0;
									m_UIMarAdsize.cy = 0;
								}
								m_addrawrect=GetPos();
								m_addrawrect.left=GetPos().right;
								m_addrawrect.right=m_addrawrect.left+/*m_UIMarAdsize.cx*/m_textLen;
							}
						}

						SetVisible((m_sText.IsEmpty()==true)?false:true);//�յľ�����
						Invalidate(); 
					}
				}
			}
			CTextUI::DoEvent(event);
		}

		//�ռ����
		void CenterControl()//����Ǹ������ڵ�Ȼ���ã������ǳ�����Ƕ���򣬾�û�б�Ҫ��
		{
			if (this->GetParent())
			{
				RECT rcpos;
				rcpos.left = (GetParent()->GetFixedWidth()-m_cxyFixed.cx)/2;
				rcpos.top = (GetParent()->GetFixedHeight()-m_cxyFixed.cy)/2;
				rcpos.right = rcpos.left + m_cxyFixed.cx;
				rcpos.bottom = rcpos.top + m_cxyFixed.cy;
				SetPos(rcpos);
			}
		}

		//����ˢ������ƣ��ڿͻ��˴�С�����ı�ʱ��
		void Update_marquee()
		{
			if(m_sText.IsEmpty())
				return;

			TCHAR ad[MAX_PATH] = {0};
			wsprintf(ad, "%s", m_sText);
			HDC hdc=::GetDC(NULL);
			//GetTextExtentPoint32(hdc, ad, lstrlen(ad), &m_UIMarAdsize);
			TEXTMETRIC tm;
			GetTextMetrics(hdc,&tm);
			m_textLen = (tm.tmMaxCharWidth ) * lstrlen(ad);
			CString cs;
			cs.Format("chs len of string = %d",lstrlen(ad));
			OutputDebugString(cs);

			m_addrawrect=GetPos();
			m_addrawrect.left=GetPos().right;
			m_addrawrect.right=m_addrawrect.left+/*m_UIMarAdsize.cx*/m_textLen;

			m_pManager->KillTimer(this, m_timer_ad);
			//���������
			m_pManager->SetTimer(this, m_timer_ad, 500);
		}

		void SetAd(TCHAR ad[])
		{
			if(m_ListCString_marquee.size() < m_list_max)
			{
 				if(m_ListCString_marquee.empty())//��һ����ӵ������������ʾ�ַ������Ⱥ�����
 				{
					if (m_bIsInit)
						m_sText = ad;
					else
					{
						m_sText2 = ad;
					}
					HDC hdc=::GetDC(NULL);
// 					int iRet = GetTextExtentPoint32(hdc, ad, lstrlen(ad), &m_UIMarAdsize);
// 					m_addrawrect=GetPos();
					TEXTMETRIC tm;
					
					GetTextMetrics(hdc,&tm);
					m_textLen = (tm.tmMaxCharWidth)*(lstrlen(ad));


					m_addrawrect.left=GetPos().right + 10;
					m_addrawrect.right=m_addrawrect.left+/*m_UIMarAdsize.cx*/m_textLen;
					m_addrawrect.bottom = 25;
					m_ListCString_marquee.push_back(m_sText);
					
					if (m_bIsInit == false)
					{
						m_bIsInit = true;
						m_pManager->KillTimer(this, m_timer_ad2);
						m_pManager->SetTimer(this, m_timer_ad2, 500);
					}
					else
					{
						m_pManager->KillTimer(this, m_timer_ad);
						//���������
						m_pManager->SetTimer(this, m_timer_ad, m_ad_time);
					}
				}
				else
				{
					CStdString str;
					str = ad;
					m_ListCString_marquee.push_back(str);
				}
			}
		}

		void PaintText(HDC hDC)
		{
			if( m_sText.IsEmpty() ) {
				m_nLinks = 0;
				return;
			}

			if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
			if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

			if( m_sText.IsEmpty() ) 
			{
				return;
			}
			
			m_nLinks = lengthof(m_rcLinks);
			RECT rc = m_rcItem;

			if (rc.top == 0 && rc.left == 10)
			{
				return;
			}

			if( IsEnabled() ) 
			{
				if( m_bShowHtml )
					CRenderEngine::DrawHtmlText(hDC, m_pManager, /*rc*/m_addrawrect, m_sText, m_bStop==false?m_dwTextColor:m_dwHotTextColor, \
					m_rcLinks, m_sLinks, m_nLinks, m_uTextStyle);
				else
					CRenderEngine::DrawText(hDC, m_pManager, /*rc*/m_addrawrect, m_sText, m_bStop==false?m_dwTextColor:m_dwHotTextColor, \
					m_iFont, m_uTextStyle);
			}
			else 
			{
				if( m_bShowHtml )
					CRenderEngine::DrawHtmlText(hDC, m_pManager, /*rc*/m_addrawrect, m_sText, m_dwDisabledTextColor, \
					m_rcLinks, m_sLinks, m_nLinks, m_uTextStyle);
				else
					CRenderEngine::DrawText(hDC, m_pManager, /*rc*/m_addrawrect, m_sText, m_dwDisabledTextColor, \
					m_iFont, m_uTextStyle);
			}
			if (GetTextStyle() & DT_CALCRECT /*&& m_bSendNotify*/)
			{
				m_pManager->SendNotify(this, _T("settext"), WPARAM(rc.bottom-rc.top), LPARAM(rc.right-rc.left));
			}
		}

		void SetStop(bool bstop){m_bStop = bstop;}
		void SetMoveSpeed(UINT uiSpeed)	{ m_ad_time = uiSpeed;}

	private://������
		list<CStdString>	m_ListCString_marquee;		//�洢���������
		RECT				m_addrawrect;//���������ʾ����Ч����
		tagSIZE				m_UIMarAdsize;//�ַ�������

		UINT				m_ad_time;//�ƶ�Ƶ��
		UINT				m_list_max;//��׼��list�������

		bool				m_bStop;//��ͣ
		DWORD				m_dwHotTextColor;//�����������ʱ����ʾ��ɫ

		bool				m_bIsInit;	//�Ƿ��ʼ�����־���
		long				m_textLen;

		CStdString m_sText2;
	};
} // namespace BzDui
