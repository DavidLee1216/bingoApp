#pragma once


// CSelectColorDlg dialog

class CSelectColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectColorDlg)

public:
	CSelectColorDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSelectColorDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_COLOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnPaint();

	CWnd* pGoldColorWnd;
	CWnd* pPurpleColorWnd;
	CWnd* pYellowColorWnd;
	CWnd* pPinkColorWnd;
	CWnd* pGreenColorWnd;
	CWnd* pOrangeColorWnd;
	CWnd* pBlueColorWnd;
	CWnd* pRedColorWnd;
	CWnd* pGreyColorWnd;
	CWnd* pBrownColorWnd;
	CWnd* pWhiteColorWnd;
	CWnd* pLightPurpleColorWnd;
	CWnd* pLightYellowColorWnd;
	CWnd* pLightPinkColorWnd;
	CWnd* pLightGreenColorWnd;
	CWnd* pLightOrangeColorWnd;
	CWnd* pLightBlueColorWnd;
	CWnd* pLightRedColorWnd;
	CWnd* pLightGreyColorWnd;
	CWnd* pLightBrownColorWnd;
	CWnd* pSilverColorWnd;
	CWnd* pDarkPurpleColorWnd;
	CWnd* pDarkYellowColorWnd;
	CWnd* pDarkPinkColorWnd;
	CWnd* pDarkGreenColorWnd;
	CWnd* pDarkOrangeColorWnd;
	CWnd* pDarkBlueColorWnd;
	CWnd* pDarkRedColorWnd;
	CWnd* pDarkGreyColorWnd;
	CWnd* pDarkBrownColorWnd;
	CBrush m_brGold, m_brPurple, m_brYellow, m_brPink, m_brGreen, m_brOrange, m_brBlue, m_brRed, m_brGrey, m_brBrown;
	CBrush m_brWhite, m_brLightPurple, m_brLightYellow, m_brLightPink, m_brLightGreen, m_brLightOrange, m_brLightBlue, m_brLightRed, m_brLightGrey, m_brLightBrown;
	CBrush m_brSilver, m_brDarkPurple, m_brDarkYellow, m_brDarkPink, m_brDarkGreen, m_brDarkOrange, m_brDarkBlue, m_brDarkRed, m_brDarkGrey, m_brDarkBrown;

	CString m_szCurrColor;
	CRect m_rectGold, m_rectPurple, m_rectYellow, m_rectPink, m_rectGreen, m_rectOrange, m_rectBlue, m_rectRed, m_rectGrey, m_rectBrown;
	CRect m_rectWhite, m_rectLightPurple, m_rectLightYellow, m_rectLightPink, m_rectLightGreen, m_rectLightOrange, m_rectLightBlue, m_rectLightRed, m_rectLightGrey, m_rectLightBrown;
	CRect m_rectSilver, m_rectDarkPurple, m_rectDarkYellow, m_rectDarkPink, m_rectDarkGreen, m_rectDarkOrange, m_rectDarkBlue, m_rectDarkRed, m_rectDarkGrey, m_rectDarkBrown;

	void OnDraw(CDC* dc);
};
