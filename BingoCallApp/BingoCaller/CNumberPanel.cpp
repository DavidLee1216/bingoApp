#include "CNumberPanel.h"


IMPLEMENT_DYNAMIC(CNumberPanel, CWnd)

CNumberPanel::CNumberPanel(void):CControlBase(NUM_PANEL_CLASSNAME)
{
	m_hWnd = this->m_hWnd;
}

CNumberPanel::~CNumberPanel()
{

}

LRESULT CNumberPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::WindowProc(message, wParam, lParam);
}


BEGIN_MESSAGE_MAP(CNumberPanel, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CNumberPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages
	OnDraw(&dc);
}


void CNumberPanel::OnDraw(CDC* dc)
{
	CRect rect;
	GetClientRect(rect);
	HBITMAP hbm = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(m_nBitmap));
	BITMAP bm;
	HDC hdcMem = CreateCompatibleDC(dc->m_hDC);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
	GetObject(hbm, sizeof(bm), &bm);
	StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	CRgn rgn;
	rgn.CreateEllipticRgn(0, 0, rect.Width(), rect.Height());
	HPEN pen;
	pen = CreatePen(PS_SOLID, 7, RGB(100, 100, 10));
	SelectObject(dc->m_hDC, pen);
	SelectObject(dc->m_hDC, GetStockObject(HOLLOW_BRUSH));
	dc->Ellipse(0, 0, rect.Width(), rect.Height());
	dc->SetBkMode(TRANSPARENT);
	LOGFONT lf; CFont font, font1;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 50; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("Arial"));
	font.CreateFontIndirect(&lf);
	CFont* oldfont = dc->SelectObject(&font);
	DrawText(dc->m_hDC, m_szNumber, -1, rect, DT_CENTER| DT_SINGLELINE| DT_VCENTER);
	dc->SelectObject(&oldfont);
	font.DeleteObject();
	DeleteObject(pen);
	SetWindowRgn(rgn, TRUE);
}
