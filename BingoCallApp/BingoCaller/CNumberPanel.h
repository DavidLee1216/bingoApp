#pragma once
#include <afxwin.h>
#include ".\Control\ControlBase.h"

#define NUM_PANEL_CLASSNAME    _T("NumPanel")  // Window class name

class CNumberPanel :
	public CControlBase
{
	DECLARE_DYNAMIC(CNumberPanel)
public:
	CNumberPanel();
	~CNumberPanel();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	int m_nBitmap;
	TCHAR m_szNumber[10];
	void OnDraw(CDC* dc);
};

