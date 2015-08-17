#pragma once
#include<afxwin.h>

class CMainFrame :public CFrameWnd
{
public:
	CMainFrame();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP();
private:
	int m_cx;
	int m_cy;
};

CMainFrame::CMainFrame() :m_cx(106*4), m_cy(106*4)
{
	Create(NULL, L"Malevich's Black Square", WS_OVERLAPPEDWINDOW, CRect(0, 0, m_cx, m_cy));
	CenterWindow();
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this);
	CRect wr;

	GetWindowRect(wr);
	m_cx = wr.Size().cx;
	m_cy = wr.Size().cy;
	if (m_cx != m_cy)
	{
		int size = m_cx > m_cy ? m_cx : m_cy;
		SetWindowPos(NULL, 0, 0, size, size, SWP_NOMOVE);
	}

	// --The Black Square--
	GetClientRect(wr);
	m_cx = wr.Size().cx;
	m_cy = wr.Size().cy;
	int min_dim = m_cx < m_cy ? m_cx : m_cy;
	int side_length = min_dim * 80.0 / 106.0;
	int center_x = m_cx / 2, center_y = m_cy / 2;
	dc.SelectStockObject(BLACK_BRUSH);
	dc.Rectangle(CRect(center_x - side_length/2, center_y - side_length/2, 
					   center_x + side_length/2, center_y + side_length/2));
	// --------------------
}

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()