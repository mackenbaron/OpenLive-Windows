// AGEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "AGEdit.h"


// CAGEdit

IMPLEMENT_DYNAMIC(CAGEdit, CEdit)

CAGEdit::CAGEdit()
: m_crBorder(RGB(0xC8, 0xC8, 0xC8))
, m_crBack(RGB(0xF5, 0xF5, 0xF5))
, m_crText(RGB(0x00, 0x00, 0x00))
{
	m_penBorder.CreatePen(PS_SOLID, 1, m_crBorder);
	m_brushBack.CreateSolidBrush(m_crBack);
}

CAGEdit::~CAGEdit()
{
	m_penBorder.DeleteObject();
	m_brushBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAGEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
//	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CAGEdit 消息处理程序
void CAGEdit::SetBalloonTip(LPCTSTR lpszTitle, LPCTSTR lpszText, int ttiIcon)
{
	m_strTitle = lpszTitle;
	m_strText = lpszText;
	m_ttiIcon = ttiIcon;
}

void CAGEdit::SetColor(COLORREF crBorder, COLORREF crBack, COLORREF crText)
{
	if (crBorder != m_crBorder){
		m_penBorder.DeleteObject();
		m_crBorder = crBorder;
		m_penBorder.CreatePen(PS_SOLID, 1, m_crBorder);
	}

	if (crBack != m_crBack){
		m_brushBack.DeleteObject();
		m_crBack = crBack;
		m_brushBack.CreateSolidBrush(m_crBack);
	}

	Invalidate();
}

BOOL CAGEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rcWindow;
	
	GetWindowRect(&rcWindow);

	CPen *lpDefaultPen = pDC->SelectObject(&m_penBorder);
	pDC->Rectangle(&rcWindow);
	pDC->SelectObject(lpDefaultPen);

	return CEdit::OnEraseBkgnd(pDC);
}

HBRUSH CAGEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何属性
	pDC->SetBkColor(m_crBack);
	pDC->SetTextColor(m_crText);

	// TODO:  如果不应调用父级的处理程序，则返回非 null 画笔
	return (HBRUSH)m_brushBack;	//编辑框底色画刷
}

void CAGEdit::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	ShowBalloonTip(m_strTitle, m_strText, m_ttiIcon);

	CEdit::OnMouseHover(nFlags, point);
}


void CAGEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!m_bTrackMouseEvent) {
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		TrackMouseEvent.hwndTrack = GetSafeHwnd();
		TrackMouseEvent.dwFlags = TME_LEAVE;
		TrackMouseEvent.dwHoverTime = 1;

		ShowBalloonTip(m_strTitle, m_strText, m_ttiIcon);

		m_bTrackMouseEvent = ::TrackMouseEvent(&TrackMouseEvent);
	}

	CEdit::OnMouseMove(nFlags, point);
}


void CAGEdit::OnMouseLeave()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bTrackMouseEvent = FALSE;

	HideBalloonTip();
	CEdit::OnMouseLeave();
}
