/*****************************************************************************
* * Filename:     Progress.h
* *
* * Created:      2004
* * Author:       J.Kotowski
* *
* * Description:  CButtonControl is a sample custom button control.
* *
* * Dependencies: VC 7.1, STL, GDIPLUS
* *
* * Credits: Heiko Eichhorn - RolloverButton example
* ****************************************************************************/

#pragma once
#include "controlbase.h"
#include <atlimage.h>

#define BUTTON_CLASSNAME    _T("JKButton")  // Window class name

class CButtonControl :
	public CControlBase
{
	DECLARE_DYNAMIC(CButtonControl)

protected:
	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnPaint();	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	CButtonControl(void);
	~CButtonControl(void);

	/** DrawButton: draws button to the specified Graphics buffer*/
	void DrawButton(CDC *pDC);
	/** CalculateDimensions: recalculates control region and drawing area */
	void ReCalculateDimensions();	
protected:
	/** m_pCachedBitmap: for better performace, we'll use this for double-buffering */
	CachedBitmap* m_pCachedBitmap; 
	/** m_bDirtyBuffer: when set to true, indicates that CachedBitmap needs to be updated */
	BOOL m_bDirtyBuffer;
	/** m_ClientRect: stores the client area dimensions */
	Rect m_ClientRect;
	/** m_hRGN: specifies the shape of the control */	
	HRGN m_hRGN;
	/** m_bMouseOver: set to true then the mouse coursor move over the control */
	BOOL m_bMouseOver;
	/** m_bTracking: helps in establishing mouse-over-control event*/
	BOOL m_bTracking;
	/** m_bMouseClicked: this variable toggles whenever user left clicks the control */
	BOOL m_bMouseClicked;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void DrawTransparentBitmap(CMemDC* pDC,
		int xStart, int yStart,
		int wWidth, int wHeight,
		CDC* pTmpDC,
		int xSource,
		int ySource);
private:
	bool m_bLButtonDown;
public:
	int m_nID;
	int m_nBitmap;
	int m_nClickedBitmap;
	TCHAR m_image_path1[MAX_PATH];
	TCHAR m_image_path2[MAX_PATH];
	TCHAR m_str[100];
	CImage m_img;
};
