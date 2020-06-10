// CSelectColorDlg.cpp : implementation file
//

#include "pch.h"
#include "BingoCaller.h"
#include "CSelectColorDlg.h"
#include "afxdialogex.h"


// CSelectColorDlg dialog
IMPLEMENT_DYNAMIC(CSelectColorDlg, CDialogEx)

CSelectColorDlg::CSelectColorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SELECT_COLOR, pParent)
{
	pGoldColorWnd = 0;
	pPurpleColorWnd = 0;
	pYellowColorWnd = 0;
	pGreenColorWnd = 0;
	pBlueColorWnd = 0;
	pPinkColorWnd = 0;
	pRedColorWnd = 0;
	pOrangeColorWnd = 0;
	pGreyColorWnd = 0;
	pBrownColorWnd = 0;
}

CSelectColorDlg::~CSelectColorDlg()
{
}

void CSelectColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectColorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CSelectColorDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectColorDlg message handlers
BOOL CSelectColorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_brGold.CreateSolidBrush(RGB(255, 215, 0));
	m_brPurple.CreateSolidBrush(RGB(128, 0, 128));
	m_brYellow.CreateSolidBrush(RGB(255, 255, 0));
	m_brPink.CreateSolidBrush(RGB(255, 192, 203));
	m_brGreen.CreateSolidBrush(RGB(0, 255, 0));
	m_brOrange.CreateSolidBrush(RGB(255, 166, 0));
	m_brBlue.CreateSolidBrush(RGB(0, 0, 255));
	m_brRed.CreateSolidBrush(RGB(255, 0, 0));
	m_brGrey.CreateSolidBrush(RGB(128, 128, 128));
	m_brBrown.CreateSolidBrush(RGB(165, 42, 42));
	m_brWhite.CreateSolidBrush(RGB(255, 255, 255));
	m_brSilver.CreateSolidBrush(RGB(192, 192, 192));
	m_brLightPurple.CreateSolidBrush(RGB(160, 32, 240));
	m_brDarkPurple.CreateSolidBrush(RGB(128, 0, 191));
	m_brLightYellow.CreateSolidBrush(RGB(255, 255, 224));
	m_brDarkYellow.CreateSolidBrush(RGB(102, 102, 0));
	m_brLightPink.CreateSolidBrush(RGB(255, 182, 193));
	m_brDarkPink.CreateSolidBrush(RGB(231, 84, 128));
	m_brLightGreen.CreateSolidBrush(RGB(144, 238, 144));
	m_brDarkGreen.CreateSolidBrush(RGB(0, 100, 0));
	m_brLightOrange.CreateSolidBrush(RGB(255, 207, 158));
	m_brDarkOrange.CreateSolidBrush(RGB(255, 140, 0));
	m_brLightBlue.CreateSolidBrush(RGB(173, 216, 230));
	m_brDarkBlue.CreateSolidBrush(RGB(0, 0, 139));
	m_brLightRed.CreateSolidBrush(RGB(255, 204, 203));
	m_brDarkRed.CreateSolidBrush(RGB(139, 0, 0));
	m_brLightGrey.CreateSolidBrush(RGB(211, 211, 211));
	m_brDarkGrey.CreateSolidBrush(RGB(169, 169, 169));
	m_brLightBrown.CreateSolidBrush(RGB(181, 101, 29));
	m_brDarkBrown.CreateSolidBrush(RGB(101, 67, 33));
	return TRUE;
}

void CSelectColorDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	HBITMAP hbm = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BACKGROUND));
	BITMAP bm;
	HDC hdcMem = CreateCompatibleDC(dc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
	GetObject(hbm, sizeof(bm), &bm);
	BitBlt(dc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	OnDraw(&dc);
}

void CSelectColorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (PtInRect(m_rectGold, point))
	{
		m_szCurrColor = _T("gold");
	}
	else if (PtInRect(m_rectPurple, point))
	{
		m_szCurrColor = _T("purple");
	}
	if (PtInRect(m_rectYellow, point))
	{
		m_szCurrColor = _T("yellow");
	}
	if (PtInRect(m_rectPink, point))
	{
		m_szCurrColor = _T("pink");
	}
	if (PtInRect(m_rectGreen, point))
	{
		m_szCurrColor = _T("green");
	}
	if (PtInRect(m_rectOrange, point))
	{
		m_szCurrColor = _T("orange");
	}
	if (PtInRect(m_rectBlue, point))
	{
		m_szCurrColor = _T("blue");
	}
	if (PtInRect(m_rectRed, point))
	{
		m_szCurrColor = _T("red");
	}
	if (PtInRect(m_rectGrey, point))
	{
		m_szCurrColor = _T("grey");
	}
	if (PtInRect(m_rectBrown, point))
	{
		m_szCurrColor = _T("brown");
	}
	if (PtInRect(m_rectWhite, point))
	{
		m_szCurrColor = _T("white");
	}
	else if (PtInRect(m_rectLightPurple, point))
	{
		m_szCurrColor = _T("lightpurple");
	}
	if (PtInRect(m_rectLightYellow, point))
	{
		m_szCurrColor = _T("lightyellow");
	}
	if (PtInRect(m_rectLightPink, point))
	{
		m_szCurrColor = _T("lightpink");
	}
	if (PtInRect(m_rectLightGreen, point))
	{
		m_szCurrColor = _T("lightgreen");
	}
	if (PtInRect(m_rectLightOrange, point))
	{
		m_szCurrColor = _T("lightorange");
	}
	if (PtInRect(m_rectLightBlue, point))
	{
		m_szCurrColor = _T("lightblue");
	}
	if (PtInRect(m_rectLightRed, point))
	{
		m_szCurrColor = _T("lightred");
	}
	if (PtInRect(m_rectLightGrey, point))
	{
		m_szCurrColor = _T("lightgrey");
	}
	if (PtInRect(m_rectLightBrown, point))
	{
		m_szCurrColor = _T("lightbrown");
	}
	if (PtInRect(m_rectSilver, point))
	{
		m_szCurrColor = _T("silver");
	}
	else if (PtInRect(m_rectDarkPurple, point))
	{
		m_szCurrColor = _T("darkpurple");
	}
	if (PtInRect(m_rectDarkYellow, point))
	{
		m_szCurrColor = _T("darkyellow");
	}
	if (PtInRect(m_rectDarkPink, point))
	{
		m_szCurrColor = _T("darkpink");
	}
	if (PtInRect(m_rectDarkGreen, point))
	{
		m_szCurrColor = _T("darkgreen");
	}
	if (PtInRect(m_rectDarkOrange, point))
	{
		m_szCurrColor = _T("darkorange");
	}
	if (PtInRect(m_rectDarkBlue, point))
	{
		m_szCurrColor = _T("darkblue");
	}
	if (PtInRect(m_rectDarkRed, point))
	{
		m_szCurrColor = _T("darkred");
	}
	if (PtInRect(m_rectDarkGrey, point))
	{
		m_szCurrColor = _T("darkgrey");
	}
	if (PtInRect(m_rectDarkBrown, point))
	{
		m_szCurrColor = _T("darkbrown");
	}
	Invalidate(TRUE);
}

void CSelectColorDlg::OnDraw(CDC* dc)
{
	CRect rect, rect1;
	GetClientRect(rect);
	pGoldColorWnd = GetDlgItem(IDC_STATIC_GOLD);
	pPurpleColorWnd = GetDlgItem(IDC_STATIC_PURPLE);
	pYellowColorWnd = GetDlgItem(IDC_STATIC_YELLOW);
	pPinkColorWnd = GetDlgItem(IDC_STATIC_PINK);
	pOrangeColorWnd = GetDlgItem(IDC_STATIC_ORANGE);
	pGreenColorWnd = GetDlgItem(IDC_STATIC_GREEN);
	pBlueColorWnd = GetDlgItem(IDC_STATIC_BLUE);
	pRedColorWnd = GetDlgItem(IDC_STATIC_RED);
	pGreyColorWnd = GetDlgItem(IDC_STATIC_LILAC);
	pBrownColorWnd = GetDlgItem(IDC_STATIC_BROWN);
	pWhiteColorWnd = GetDlgItem(IDC_STATIC_WHITE);
	pSilverColorWnd = GetDlgItem(IDC_STATIC_SILVER);
	pLightPurpleColorWnd = GetDlgItem(IDC_STATIC_LIGHT_PURPLE);
	pDarkPurpleColorWnd = GetDlgItem(IDC_STATIC_DARK_PURPLE);
	pLightYellowColorWnd = GetDlgItem(IDC_STATIC_LIGHT_YELLOW);
	pDarkYellowColorWnd = GetDlgItem(IDC_STATIC_DARK_YELLOW);
	pLightPinkColorWnd = GetDlgItem(IDC_STATIC_LIGHT_PINK);
	pDarkPinkColorWnd = GetDlgItem(IDC_STATIC_DARK_PINK);
	pLightOrangeColorWnd = GetDlgItem(IDC_STATIC_LIGHT_ORANGE);
	pDarkOrangeColorWnd = GetDlgItem(IDC_STATIC_DARK_ORANGE);
	pLightGreenColorWnd = GetDlgItem(IDC_STATIC_LIGHT_GREEN);
	pDarkGreenColorWnd = GetDlgItem(IDC_STATIC_DARK_GREEN);
	pLightBlueColorWnd = GetDlgItem(IDC_STATIC_LIGHT_BLUE);
	pDarkBlueColorWnd = GetDlgItem(IDC_STATIC_DARK_BLUE);
	pLightRedColorWnd = GetDlgItem(IDC_STATIC_LIGHT_RED);
	pDarkRedColorWnd = GetDlgItem(IDC_STATIC_DARK_RED);
	pLightGreyColorWnd = GetDlgItem(IDC_STATIC_LIGHT_GREY);
	pDarkGreyColorWnd = GetDlgItem(IDC_STATIC_DARK_GREY);
	pLightBrownColorWnd = GetDlgItem(IDC_STATIC_LIGHT_BROWN);
	pDarkBrownColorWnd = GetDlgItem(IDC_STATIC_DARK_BROWN);
// 	CPen pen;
// 	pen.CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
// 	CPen *oldPen;
	CBrush outline_br;
	outline_br.CreateSolidBrush(RGB(255, 255, 255));

	LOGFONT lf; CFont font;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 20; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("Arial"));//GalanoGrotesque-BlackItalic
	font.CreateFontIndirect(&lf);
	CFont* oldfont = dc->SelectObject(&font);

	CBrush* oldBrush = dc->SelectObject(&m_brGold);
	pGoldColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectGold = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("gold"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectGold, &outline_br);
	}
	dc->SetBkMode(TRANSPARENT);
	DrawText(dc->m_hDC, _T("gold"), -1, rect1, DT_CENTER | DT_SINGLELINE| DT_VCENTER);

	dc->SelectObject(&m_brWhite);
	pWhiteColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectWhite = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("white"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectWhite, &outline_br);
	}
	dc->SetBkMode(TRANSPARENT);
	DrawText(dc->m_hDC, _T("white"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SelectObject(&m_brSilver);
	pSilverColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectSilver = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("silver"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectSilver, &outline_br);
	}
	dc->SetBkMode(TRANSPARENT);
	DrawText(dc->m_hDC, _T("silver"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brPurple);
	pPurpleColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectPurple = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("purple"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectPurple, &outline_br);
	}
	DrawText(dc->m_hDC, _T("purple"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brYellow);
	pYellowColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectYellow = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("yellow"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectYellow, &outline_br);
	}
	DrawText(dc->m_hDC, _T("yellow"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brPink);
	pPinkColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectPink = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("pink"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectPink, &outline_br);
	}
	DrawText(dc->m_hDC, _T("pink"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brGreen);
	pGreenColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectGreen = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("green"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectGreen, &outline_br);
	}
	DrawText(dc->m_hDC, _T("green"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brOrange);
	pOrangeColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectOrange = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("orange"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectOrange, &outline_br);
	}
	DrawText(dc->m_hDC, _T("orange"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brBlue);
	pBlueColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectBlue = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("blue"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectBlue, &outline_br);
	}
	DrawText(dc->m_hDC, _T("blue"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brRed);
	pRedColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectRed = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("red"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectRed, &outline_br);
	}
	DrawText(dc->m_hDC, _T("red"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brGrey);
	pGreyColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectGrey = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("grey"))
	{
		dc->SetTextColor(RGB(0, 0, 0));
		dc->FrameRect(m_rectGrey, &outline_br);
	}
	DrawText(dc->m_hDC, _T("grey"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brBrown);
	pBrownColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectBrown = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("brown"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectBrown, &outline_br);
	}
	DrawText(dc->m_hDC, _T("brown"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightPurple);
	pLightPurpleColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightPurple = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightpurple"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightPurple, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light purple"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkPurple);
	pDarkPurpleColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkPurple = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkpurple"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkPurple, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark purple"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightRed);
	pLightRedColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightRed = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightred"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightRed, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light red"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkRed);
	pDarkRedColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkRed = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkred"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkRed, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark red"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightPink);
	pLightPinkColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightPink = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightpink"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightPink, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light pink"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkPink);
	pDarkPinkColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkPink = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkpink"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkPink, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark pink"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightGreen);
	pLightGreenColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightGreen = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightgreen"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightGreen, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light green"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkGreen);
	pDarkGreenColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkGreen = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkgreen"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkGreen, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark green"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightBlue);
	pLightBlueColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightBlue = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightblue"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightBlue, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light blue"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkBlue);
	pDarkBlueColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkBlue = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkblue"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkBlue, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark blue"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightBrown);
	pLightBrownColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightBrown = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightbrown"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightBrown, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light brown"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkBrown);
	pDarkBrownColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkBrown = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkbrown"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkBrown, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark brown"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightGrey);
	pLightGreyColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightGrey = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightgrey"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightGrey, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light grey"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkGrey);
	pDarkGreyColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkGrey = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkgrey"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkGrey, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark grey"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightYellow);
	pLightYellowColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightYellow = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightyellow"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightYellow, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light yellow"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkYellow);
	pDarkYellowColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkYellow = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkyellow"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkYellow, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark yellow"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brLightOrange);
	pLightOrangeColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectLightOrange = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("lightorange"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectLightOrange, &outline_br);
	}
	DrawText(dc->m_hDC, _T("light orange"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SetTextColor(RGB(0, 0, 0));
	dc->SelectObject(&m_brDarkOrange);
	pDarkOrangeColorWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_rectDarkOrange = rect1;
	dc->Rectangle(rect1);
	if (m_szCurrColor == _T("darkorange"))
	{
		dc->SetTextColor(RGB(255, 255, 255));
		dc->FrameRect(m_rectDarkOrange, &outline_br);
	}
	DrawText(dc->m_hDC, _T("dark orange"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SelectObject(oldfont);
	dc->SelectObject(oldBrush);
// 	dc->SelectObject(oldPen);

}

void CSelectColorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
