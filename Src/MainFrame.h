#pragma once
#include<afxwin.h>
#include"Tictactoe.h"
using namespace ttt;

class CMainFrame :public CFrameWnd
{
public:
	CMainFrame();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	DECLARE_MESSAGE_MAP();
private:
	CRect m_wr, m_crosses_button, m_noughts_button;
	int m_wr_cx, m_wr_cy;
	Mark m_side;
};

CMainFrame::CMainFrame() :m_side(Mark::empty)
{
	Create(NULL, L"Tic-tac-toe", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX, CRect(0, 0, 500, 530));

	GetClientRect(m_wr);
	m_wr.top += 50;
	m_wr.left += 50;
	m_wr.bottom -= 80;
	m_wr.right -= 50;
	m_wr.OffsetRect(0, 40);
	m_wr_cx = m_wr.Size().cx;
	m_wr_cy = m_wr.Size().cy;

	CClientDC dc(this);
	m_crosses_button = CRect(m_wr.left, 30, m_wr.left + dc.GetTextExtent("Crosses").cx + 10, 30 + dc.GetTextExtent("Crosses").cy + 10);

	int left = m_wr.left + dc.GetTextExtent("Crosses").cx + 10;
	m_noughts_button = CRect(left + 30, 30, left + 30 + dc.GetTextExtent("Crosses").cx + 10, 30 + dc.GetTextExtent("Noughts").cy + 10);

	CenterWindow();
}

inline void CMainFrame::OnPaint()
{
	CPaintDC dc(this);
	dc.Rectangle(m_wr);

	CRect vrect(m_wr.left + m_wr_cx / 3, m_wr.top, m_wr.right - m_wr_cx / 3, m_wr.bottom);
	dc.Rectangle(vrect);
	
	dc.SelectStockObject(HOLLOW_BRUSH);
	CRect hrect(m_wr.left, m_wr.top + m_wr_cy / 3, m_wr.right, m_wr.bottom - m_wr_cy / 3);
	dc.Rectangle(hrect);

	(m_side == Mark::cross) ? dc.SelectStockObject(BLACK_BRUSH) : dc.SelectStockObject(HOLLOW_BRUSH);
	dc.Rectangle(m_crosses_button);
	dc.DrawTextW("Crosses", m_crosses_button, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	(m_side == Mark::nought) ? dc.SelectStockObject(BLACK_BRUSH) : dc.SelectStockObject(HOLLOW_BRUSH);
	dc.Rectangle(m_noughts_button);
	dc.DrawTextW("Noughts", m_noughts_button, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

inline void CMainFrame::OnLButtonDown(UINT nFlags, CPoint pt)
{
	// Choosing side
	if (m_side == Mark::empty && m_crosses_button.PtInRect(pt))
		m_side = Mark::cross;
	else if (m_side == Mark::empty && m_noughts_button.PtInRect(pt))
		m_side = Mark::nought;

	Invalidate(TRUE);
}

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()