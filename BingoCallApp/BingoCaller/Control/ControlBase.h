
#pragma once

#include "../framework.h"

class CControlBase : public CWnd
{
	DECLARE_DYNAMIC(CControlBase)

public:	
	/** Constructor: takes one parameter - the name of the window class. */
	CControlBase(LPCTSTR lpszClassName);
	/** Destructor */
	virtual ~CControlBase();

	/** Create: window create function. */
	BOOL Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle = WS_VISIBLE);
	
protected:
	/** RegisterWindowClass: window create function. */
	BOOL RegisterWindowClass();
protected:
	/** m_strClassName: name used to register the window class. */
	const CString m_strClassName;
};


