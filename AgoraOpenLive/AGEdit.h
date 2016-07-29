#pragma once


// CAGEdit

class CAGEdit : public CEdit
{
	DECLARE_DYNAMIC(CAGEdit)

public:
	CAGEdit();
	virtual ~CAGEdit();

	void SetColor(COLORREF crBorder, COLORREF crBack, COLORREF crText);
	void SetBalloonTip(LPCTSTR lpszTitle, LPCTSTR lpszText, int ttiIcon);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()

private:
	COLORREF	m_crBorder;
	COLORREF	m_crBack;
	COLORREF	m_crText;

	CPen		m_penBorder;
	CBrush		m_brushBack;

	CString		m_strTitle;
	CString		m_strText;
	int			m_ttiIcon;

	BOOL		m_bTrackMouseEvent;
	BOOL		m_bOverControl;
};


