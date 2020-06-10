// ButtonControl.cpp : implementation file
#include "../pch.h"
// #include "../framework.h"
#include ".\buttoncontrol.h"
#include <math.h>

#define WM_START WM_USER + 1
#define WM_STOP WM_USER + 2
#define WM_CALL WM_USER + 3
#define WM_VERIFY WM_USER + 4
#define WM_RECALL WM_USER + 5

IMPLEMENT_DYNAMIC(CButtonControl, CWnd)

CButtonControl::CButtonControl(void) : CControlBase(BUTTON_CLASSNAME), 
	m_pCachedBitmap(NULL), m_bDirtyBuffer(TRUE), m_hRGN(NULL),
	m_bMouseOver(FALSE), m_bTracking(FALSE), m_bMouseClicked(FALSE)
{
}

CButtonControl::~CButtonControl(void)
{		
}

BEGIN_MESSAGE_MAP(CButtonControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CButtonControl::OnDestroy()
{
	if (m_pCachedBitmap)
	{
		delete m_pCachedBitmap;
		m_pCachedBitmap = NULL;
	}

	if (m_hRGN)
	{
		DeleteObject(m_hRGN);
		m_hRGN = NULL;
	}
}

void CButtonControl::OnSize(UINT, int, int) 
{
	m_bDirtyBuffer = TRUE;
}

BOOL CButtonControl::OnEraseBkgnd(CDC*)
{
	// Return true so that the control is not erased
	// (Aside: erase means paint the surface with the window's default brush)
	return TRUE;
}

LRESULT CButtonControl::OnMouseLeave(WPARAM, LPARAM)
{
	m_bDirtyBuffer = TRUE;
	m_bMouseOver = FALSE;
	m_bTracking = FALSE;	
	
	Invalidate(FALSE);	
	
	return 1;
}

LRESULT CButtonControl::OnMouseHover(WPARAM, LPARAM)
{
	m_bDirtyBuffer = TRUE;
	m_bMouseOver = TRUE;
	m_bDirtyBuffer = TRUE;
	
	Invalidate();	
	
	return 1;
}

void CButtonControl::OnMouseMove(UINT, CPoint)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);		
	}	
	if(m_bMouseOver == FALSE)
	{
		m_bMouseOver = TRUE;		
	}
}

void CButtonControl::OnLButtonDown(UINT, CPoint)
{	
	m_bMouseClicked = !m_bMouseClicked;
	m_bDirtyBuffer = TRUE;
	m_bLButtonDown = TRUE;
	Invalidate();	
}

void CButtonControl::ReCalculateDimensions()
{
	/***********************************************************

		NOTE from the author:

		GDI regions and GDI+ ROUND regions are NOT the same!
		That's why I am going through extra effort to create 
		the region using GDI+ in order for the control to 
		render properly around the edges.

	************************************************************/

	// Calculate CDialog offset
	POINT ptParent = {0, 0};
	::ClientToScreen(m_hWnd, &ptParent);
	::ScreenToClient(GetParent()->m_hWnd, &ptParent);

	// Get the device context & create Graphics class
	Graphics graphics(m_hWnd);

	// Obtain client rectangle
	RECT rect;
	GetClientRect(&rect);
	m_ClientRect = Rect(rect.left, rect.top, rect.right, rect.bottom);

	// Draw the region
	GraphicsPath path;
	path.AddEllipse(m_ClientRect);
	Region rgn(&path);
	SetWindowRgn(rgn.GetHRGN(&graphics), FALSE);

	// Apply CDialog offset
	rgn.Translate(ptParent.x, ptParent.y);
	m_hRGN = rgn.GetHRGN(&graphics);
}

void CButtonControl::OnPaint()
{
	// device context for painting
	CPaintDC dc(this);

	// Only redraw the control if the buffer is dirty
// 	if (m_bDirtyBuffer)
// 	{
		DrawButton(&dc);
// 		m_bDirtyBuffer = FALSE;
// 	}
}

void CButtonControl::DrawTransparentBitmap(CMemDC* pDC,
	int xStart, int yStart,
	int wWidth, int wHeight,
	CDC* pTmpDC,
	int xSource,
	int ySource)
	{

		// We are going to paint the two DDB's in sequence to the destination.
		// 1st the monochrome bitmap will be blitted using an AND operation to
		// cut a hole in the destination. The color image will then be ORed
		// with the destination, filling it into the hole, but leaving the
		// surrounding area untouched.

/*
		CDC hdcMem;
		hdcMem.CreateCompatibleDC(NULL);

		CBitmap* hbmT = hdcMem.SelectObject(&m_bmpMask);

		StretchBlt(pDC->GetDC(), xStart, yStart, wWidth, wHeight, hdcMem.m_hDC,
					 0, 0, SRCAND);


		// Also note the use of SRCPAINT rather than SRCCOPY.

		StretchBlt(pDC->GetDC(), xStart, yStart, wWidth, wHeight, pTmpDC->m_hDC,
					xSource, ySource,SRCPAINT);

		// Now, clean up.
		hdcMem.SelectObject(hbmT);
		hdcMem.DeleteDC();
*/
}

void CButtonControl::DrawButton(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
// 	HBITMAP hbm = m_bLButtonDown?LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(m_nClickedBitmap)): LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(m_nBitmap));
// 	BITMAP bm;
// 	HDC hdcMem = CreateCompatibleDC(pDC->m_hDC);
// 	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
// 	GetObject(hbm, sizeof(bm), &bm);
// 	StretchBlt(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	TCHAR* image_path = m_bLButtonDown ? (TCHAR*)m_image_path2 : (TCHAR*)m_image_path1;
	m_img.Load(image_path);
// 	m_img.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height());
 	m_img.StretchBlt(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	m_img.Detach();

// 	LOGFONT lf; CFont font, font1;
// 	memset(&lf, 0, sizeof(LOGFONT));
// 	lf.lfHeight = 60; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
// 	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));//Galano Grotesque Black Italic
// 	font.CreateFontIndirect(&lf);
// 	CFont* oldfont = pDC->SelectObject(&font);
// 	SetBkMode(pDC->m_hDC, TRANSPARENT);
// 	pDC->SetTextColor(RGB(255, 255, 255));
// 	int h = DrawText(pDC->m_hDC, m_str, -1, rect, DT_CENTER);
// 	pDC->SelectObject(&oldfont);

}


void CButtonControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = false;
	Invalidate();
	CWnd *pParent = GetParent();
	if (pParent)
	{
		if (m_nID == 0)
		{
			::SendMessage(pParent->m_hWnd, WM_START, 0, 0);
		}
		else if (m_nID == 1)
			::SendMessage(pParent->m_hWnd, WM_STOP, 0, 0);
		else if (m_nID == 2)
			::SendMessage(pParent->m_hWnd, WM_CALL, 0, 0);
		else if (m_nID == 3)
			::SendMessage(pParent->m_hWnd, WM_VERIFY, 0, 0);
		else if (m_nID == 4)
			::SendMessage(pParent->m_hWnd, WM_RECALL, 0, 0);
	}
	CControlBase::OnLButtonUp(nFlags, point);
}
